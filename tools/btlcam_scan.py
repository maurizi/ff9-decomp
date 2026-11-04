#!/usr/bin/env python3
"""Battle camera scripts: parse every one on the discs and report the command flags they use.

    python3 tools/btlcam_scan.py                      # summary + every script with flag 0x4000 (the cue)
    python3 tools/btlcam_scan.py --battle 21          # every scene camera of one encounter, decoded
    python3 tools/btlcam_scan.py --flag 0x1000        # scripts using another flag bit instead of the cue
    python3 tools/btlcam_scan.py --json out.json      # every parsed script

Format (ovl_0b btlcam_loadScript / btlcam_step):
- header: s16 flags. Bits 0-3: which of 4 u16 offsets follow (variant 0-2 command lists, [3] the variant
  selector word); then always one u16 offset to the point data; bits 4-7: which of 4 6-byte points it holds.
  Bit 9: nextVariant = (flags >> (variant*2 + 10)) & 3. Offsets are relative to the script start.
- command: u16 c0 (bits 0-9 frame, bits 10-15 = flag bits 16-21), u16 c1 (flag bits 0-15), then by flag:
  per path i = 0, 1 mode = (flags >> 3i) & 7: modes 1-3 a 6-byte key, modes 2-4 4 bytes of timing;
  0x40 accel (2), 0x200 sway (2), 0x400 idle sway (2), 0x800 zoom (4), 0x1000 shake (4), 0x4000 cue (2: byte
  1 = g_btlCamera.endIntro, anything else Battle.subPhase++), 0x8000 back to the idle camera (3 bytes read,
  the script ends there). 0x80, 0x100100, 0x2000, 0x80000 take nothing. The list ends at a u16 0.
- A command runs when the script frame (starting at 1, +1 per step) equals its frame, at most 10 per step.

Sources:
- scene data (encounter bundle, FF9.IMG dir 5, DB kind 0x11: u16[1] = offset of a self-relative s16 table,
  btl_getSceneData): 0 intro (btl_startBattle), 1 idle (btlcam_endEffect / flag 0x8000), 2 victory
  (btl_victory), 3/4 party command cameras (btlfx_playCmdCamera), 5+k enemy type k (btlseq camera op through
  btlcam_playSeq, and btlfx_playCmdCamera for enemy casters); BTL_SETVAR_PLAY_CAMERA plays any n from an AI
  script while Battle.phase is BTL_PHASE_SCRIPT.
- effect data (FF9.IMG dir 13, type-2/10 chunks: self-relative s32 offset table, btlfx_getFileData): the ids
  each effect's own script names with ops 0x23 / 0x29 (all below 0x40 on the discs, i.e. the file's own data),
  looked up in each of the file's data chunks. Resident effects (header 0x83FF) name no cameras.
- common effect data (ovl_0b g_btlfxCommonData, btlfx_getCommonData): the ids in g_btlfxCmdCameras and the two
  bounce cameras (btlfx_playCmdCamera, btlfx_playBounceCamera).
- Battle AI scripts play scene cameras 9-12 (the entries after the 9 fixed ones) via BTL_SETVAR_PLAY_CAMERA.

stdlib only; runs on the host (prefix `ulimit -v 4000000;`) or in Docker.
"""
import argparse
import json
import os
import struct
import sys

sys.path.insert(0, os.path.dirname(os.path.abspath(__file__)))
from effect_labels import encounters, db_kinds  # noqa: E402
from effect_modules import fx_files, parse_file  # noqa: E402
from ff9img import Img, SECTOR  # noqa: E402

ROOT = os.path.dirname(os.path.dirname(os.path.abspath(__file__)))
OVL0B_VRAM = 0x800A7000
COMMON_DATA = 0x800F78B4  # g_btlfxCommonData
CMD_CAMERAS = 0x800F8D78  # g_btlfxCmdCameras[5], then bounce party/enemy at +8/+9
KNOWN = 0x40 | 0x80 | 0x100 | 0x200 | 0x400 | 0x800 | 0x1000 | 0x2000 | 0x4000 | 0x8000 | 0x80000 | 0x100000 \
    | 0x3F  # path modes
ARGS = {0x40: 2, 0x200: 2, 0x400: 2, 0x800: 4, 0x1000: 4, 0x4000: 2}
SCENE_ROLE = {0: 'intro', 1: 'idle', 2: 'victory', 3: 'cmd cam (party target)', 4: 'cmd cam (enemy target)'}


def u16(d, o):
    return struct.unpack_from('<H', d, o)[0]


def parse_list(d, p, end):
    """Command list from p: (commands, end reason, problems). Each command: (frame, flags, {flag: arg bytes})."""
    cmds, probs, last = [], [], 0
    for _ in range(2000):
        if p + 2 > end:
            return cmds, 'overrun', probs
        c0 = u16(d, p)
        if c0 == 0:
            return cmds, 'end', probs
        if p + 4 > end:
            return cmds, 'overrun', probs
        frame = c0 & 0x3FF
        flags = u16(d, p + 2) | (c0 >> 10) << 16
        if frame == 0 or frame < last:
            probs.append(f'frame {frame} after {last}')
        last = frame
        if flags & ~KNOWN:
            probs.append(f'unknown flags {flags & ~KNOWN:#x}')
        q = p + 4
        for i in range(2):
            mode = flags >> (3 * i) & 7
            if 1 <= mode <= 3:
                q += 6
            if 2 <= mode <= 4:
                q += 4
        args = {}
        for bit in (0x40, 0x200, 0x400, 0x800, 0x1000, 0x4000):
            if flags & bit:
                args[bit] = d[q:q + ARGS[bit]]
                q += ARGS[bit]
        if flags & 0x8000:
            args[0x8000] = d[q:q + 3]
        if q > end:
            return cmds, 'overrun', probs
        cmds.append((frame, flags, args))
        if flags & 0x8000:
            return cmds, 'idle', probs
        p = q
    return cmds, 'too long', probs


def parse_script(d, base, end):
    """A camera script at d[base:end]: dict, or None when the header does not fit."""
    if base + 4 > end:
        return None
    flags = struct.unpack_from('<h', d, base)[0]
    o = base + 2
    pcs = []
    for i in range(4):
        if flags >> i & 1:
            if o + 2 > end:
                return None
            pcs.append(u16(d, o))
            o += 2
        else:
            pcs.append(None)
    if o + 2 > end:
        return None
    pts = u16(d, o)
    npts = bin(flags >> 4 & 0xF).count('1')
    probs = []
    # points past the entry (the next offset-table entry) only cost a bcopy of other data: a warning
    warn = ['points past the entry'] if base + pts + 6 * npts > end else []
    out = {'flags': flags & 0xFFFF, 'selector': None, 'variants': {}, 'problems': probs, 'warnings': warn}
    if pcs[3] is not None:
        if base + pcs[3] + 2 > end:
            probs.append('selector out of range')
        else:
            out['selector'] = u16(d, base + pcs[3])
    if not any(x is not None for x in pcs[:3]):
        warn.append('no command list (the camera does nothing)')
    for v in range(3):
        if pcs[v] is None:
            continue
        if base + pcs[v] >= end:
            probs.append(f'variant {v} out of range')
            continue
        cmds, why, p2 = parse_list(d, base + pcs[v], end)
        out['variants'][v] = {'cmds': cmds, 'end': why}
        if why not in ('end', 'idle'):
            probs.append(f'variant {v}: {why}')
        probs += [f'variant {v}: {x}' for x in p2]
    return out


def clean(s):
    return s is not None and not s['problems']


def uses(s, bit):
    """[(variant, frame, arg bytes)] of the commands with flag `bit`."""
    return [(v, f, a.get(bit, b'')) for v, x in s['variants'].items() for f, fl, a in x['cmds'] if fl & bit]


def offset_entries(d, table, end, width):
    """Entries of a self-relative offset table at d[table:end]: [(index, start, end)]. The table ends where the
    first entry starts; an entry ends at the next larger offset."""
    fmt = '<h' if width == 2 else '<i'
    first = None
    offs = []
    i = 0
    while first is None or table + i * width < first:
        if table + (i + 1) * width > end:
            break
        o = table + struct.unpack_from(fmt, d, table + i * width)[0]
        if not (table < o <= end):
            break
        offs.append(o)
        first = o if first is None else min(first, o)
        i += 1
    bounds = sorted(set(offs + [end]))
    return [(i, o, next(b for b in bounds if b > o) if o < end else end) for i, o in enumerate(offs)]


def scene_scripts(d):
    """[(n, start, end)] of an encounter bundle's scene data, plus the enemy type count."""
    k = db_kinds(d)
    bundle = k[0x11]
    ntypes = d[k[0x10] + 2]
    tbl = bundle + u16(d, bundle + 2)
    # the bundle item ends where the next DB item starts; take the end of the file data as the bound
    return offset_entries(d, tbl, len(d), 2), ntypes


def fmt_uses(us):
    return ', '.join(f'v{v} f{f} arg {a.hex()}' for v, f, a in us)


def scan_scenes(disks, bit):
    res = {}
    for disc in range(1, 5):
        path = os.path.join(disks, f'disc{disc}', 'FF9.IMG')
        if not os.path.exists(path):
            continue
        for bid, d in encounters(path):
            try:
                ents, ntypes = scene_scripts(d)
            except Exception as e:  # noqa: BLE001
                res.setdefault(bid, {'error': str(e), 'discs': []})['discs'].append(disc)
                continue
            scripts = {}
            for n, s, e in ents:  # past 5 + ntypes: only BTL_SETVAR_PLAY_CAMERA reaches them
                scripts[n] = parse_script(d, s, e)
            key = bid
            if key in res and res[key].get('scripts') != scripts:
                key = f'{bid}/disc{disc}'  # differs from the earlier disc's copy
            if key in res:
                res[key]['discs'].append(disc)
                continue
            res[key] = {'discs': [disc], 'ntypes': ntypes, 'nentries': len(ents), 'scripts': scripts}
    return res


def fx_script_cameras(h):
    """Camera ids an effect script names: op 0x23 (btlfx_opAddCameraChoice, a = id, 0xFF clears) and op 0x29
    (btlfx_opPlayCamera: b = 0 plays id a, 1-3 one of the 0x23 choices). The script is 3-byte ops at offset 0x400
    of the file's header sector (g_btlfxFileScript), up to op 0 (end)."""
    ids, modes = set(), set()
    for p in range(0x400, len(h) - 2, 3):
        op, a, b = h[p], h[p + 1], h[p + 2]
        if op == 0:
            break
        if op == 0x23 and a != 0xFF:
            ids.add(a)
        if op == 0x29:
            modes.add(b)
            if b == 0:
                ids.add(a)
    return ids, modes


def chunk_extra(h):
    """[(group, chunk, common-data sectors)] from the header sector, as parse_file reads it."""
    p, out = 2, []
    for gi in range(struct.unpack_from('<H', h, 0)[0]):
        nch = struct.unpack_from('<H', h, p + 2)[0]
        p += 4
        for ci in range(nch):
            ty, fl = h[p], h[p + 1]
            p += 4
            extra = 0
            if ty == 2 and fl:
                extra = struct.unpack_from('<H', h, p)[0]
                p += 2
            out.append((gi, ci, extra))
    return out


def scan_effects(disks, bit):
    """Every camera an effect file's script names, looked up in each of its effect-data chunks (ids below 0x40
    index the current slot's data; a summon has several). Returns (scripts, problems)."""
    img = Img(os.path.join(disks, 'disc1', 'FF9.IMG'))
    out, missing = [], []
    for fid, sec, n in fx_files(img):
        h = img.read(sec * SECTOR, SECTOR)
        ids, modes = fx_script_cameras(h)
        if not ids:
            continue
        extra = {(g, c): e for g, c, e in chunk_extra(h)}
        groups, _ = parse_file(img, sec)
        found = set()
        for gi, g in enumerate(groups):
            for ci, c in enumerate(g['chunks']):
                if c['type'] not in (2, 10):
                    continue
                d = img.read(c['sector'] * SECTOR, c['sectors'] * SECTOR)
                # a flagged chunk carries common data first; the file's own table follows it
                for base in sorted({0, extra.get((gi, ci), 0) * SECTOR}):
                    ents = {i: (s, e) for i, s, e in offset_entries(d, base, len(d), 4)}
                    for i in sorted(ids):
                        if i in ents:
                            sc = parse_script(d, *ents[i])
                            if clean(sc):
                                found.add(i)
                                out.append({'file': fid, 'group': gi, 'chunk': ci, 'base': base, 'index': i,
                                            'script': sc})
        for i in sorted(ids - found):
            missing.append((fid, i))
    return out, missing


def scan_common():
    ovl = open(os.path.join(ROOT, 'disks/us/ovl/0b.bin'), 'rb').read()
    base = COMMON_DATA - OVL0B_VRAM
    cams = list(ovl[CMD_CAMERAS - OVL0B_VRAM:CMD_CAMERAS - OVL0B_VRAM + 5])
    bounce = list(ovl[CMD_CAMERAS - OVL0B_VRAM + 8:CMD_CAMERAS - OVL0B_VRAM + 10])
    # the common table's extent: up to the last entry's data; bound it by the first offset-table entry order
    ents = offset_entries(ovl, base, len(ovl), 4)
    out = {}
    for i, s, e in ents:
        out[i] = parse_script(ovl, s, e)
    return out, cams, bounce


def describe(s):
    lines = [f"  header {s['flags']:#06x} selector {s['selector'] if s['selector'] is None else hex(s['selector'])}"
             + (f"  PROBLEMS {s['problems']}" if s['problems'] else '')]
    for v, x in s['variants'].items():
        lines.append(f"  variant {v} ({x['end']}):")
        for f, fl, a in x['cmds']:
            extra = ' '.join(f'{b:#x}={a[b].hex()}' for b in a)
            lines.append(f'    frame {f:4d} flags {fl:#08x} {extra}')
    return '\n'.join(lines)


def main():
    ap = argparse.ArgumentParser(description=__doc__, formatter_class=argparse.RawDescriptionHelpFormatter)
    ap.add_argument('--disks', default=os.path.join(ROOT, 'disks/us'))
    ap.add_argument('--flag', type=lambda x: int(x, 0), default=0x4000)
    ap.add_argument('--battle', type=int)
    ap.add_argument('--json')
    ap.add_argument('-v', '--verbose', action='store_true', help='list every hit, not only the unusual ones')
    a = ap.parse_args()
    bit = a.flag

    scenes = scan_scenes(a.disks, bit)
    if a.battle is not None:
        r = scenes[a.battle]
        print(f"battle {a.battle}: discs {r['discs']}, {r['ntypes']} enemy types, {r['nentries']} table entries")
        for n, s in r['scripts'].items():
            role = SCENE_ROLE.get(n, f'enemy type {n - 5}' if n < 5 + r['ntypes'] else 'extra: AI script only')
            print(f'scene {n} ({role}):')
            print(describe(s) if s else '  (no header)')
        return

    total = ok = 0
    hits = []
    for bid, r in sorted(scenes.items(), key=lambda x: str(x[0]).zfill(12)):
        if 'error' in r:
            print(f'battle {bid}: {r["error"]}')
            continue
        for n, s in r['scripts'].items():
            total += 1
            ok += bool(clean(s))
            if s and uses(s, bit):
                hits.append((bid, n, s))
    print(f'scene data: {len(scenes)} encounters (a battle id twice when a disc has a different copy), '
          f'{total} scripts, {ok} parse cleanly')
    for bid, r in scenes.items():
        for n, s in r.get('scripts', {}).items():
            if not clean(s):
                print(f'  not clean: battle {bid} scene {n}: {s and s["problems"]}')
    # the usual cases (flag 0x4000: intro arg 1, victory arg 2) are counted; everything else is listed
    common = {}
    for bid, n, s in hits:
        args = sorted({a.hex() for _, _, a in uses(s, bit)})
        role = SCENE_ROLE.get(n, 'enemy type') if n < 5 else 'enemy type'
        if a.verbose or not (bit == 0x4000 and ((n == 0 and args == ['0100']) or (n == 2 and args == ['0200']))):
            bad = '' if clean(s) else f' [problems: {s["problems"]}]'
            role = SCENE_ROLE.get(n, f'enemy type {n - 5}' if n < 5 + scenes[bid]['ntypes'] else 'extra: AI only')
            print(f'  battle {bid} scene {n} ({role}): '
                  f'{fmt_uses(uses(s, bit))}{bad}')
        else:
            common[(n, role, tuple(args))] = common.get((n, role, tuple(args)), 0) + 1
    for (n, role, args), c in sorted(common.items()):
        print(f'  scene {n} ({role}) with arg {"/".join(args)}: {c} encounters')

    fx, missing = scan_effects(a.disks, bit)
    files = len({x['file'] for x in fx})
    print(f'effect data (disc 1): {len({(x["file"], x["index"]) for x in fx})} cameras named by effect scripts '
          f'parse cleanly ({len(fx)} chunk lookups), in {files} files; {len(missing)} named ids found nowhere'
          + (f': {missing[:20]}' if missing else ''))
    for x in fx:
        us = uses(x['script'], bit)
        if us:
            print(f"  effect {x['file']:3d} group {x['group']} chunk {x['chunk']} base {x['base']:#x} "
                  f"camera {x['index']:#x}: {fmt_uses(us)}")

    common, cams, bounce = scan_common()
    print(f'common data: {len(common)} entries, {sum(bool(clean(s)) for s in common.values())} parse cleanly; '
          f'command cameras {cams}, bounce cameras {bounce}')
    for i in sorted(set(cams + bounce)):
        s = common.get(i)
        print(f'  common {i}: {"clean" if clean(s) else "NOT clean " + str(s and s["problems"])}'
              + (f'; uses: {fmt_uses(uses(s, bit))}' if s and uses(s, bit) else ''))
    for i, s in common.items():
        if clean(s) and uses(s, bit) and i not in cams + bounce:
            print(f'  common {i} (not a known camera id): {fmt_uses(uses(s, bit))}')

    if a.json:
        def enc(s):
            if not s:
                return None
            return {'flags': s['flags'], 'selector': s['selector'], 'problems': s['problems'],
                    'variants': {v: {'end': x['end'], 'cmds': [[f, fl, {hex(b): a[b].hex() for b in a}]
                                                                 for f, fl, a in x['cmds']]}
                                 for v, x in s['variants'].items()}}
        out = {'scenes': {bid: {n: enc(s) for n, s in r.get('scripts', {}).items()} for bid, r in scenes.items()},
               'effects': [dict(x, script=enc(x['script'])) for x in fx],
               'common': {i: enc(s) for i, s in common.items()}}
        with open(a.json, 'w') as f:
            json.dump(out, f, indent=0)


if __name__ == '__main__':
    main()
