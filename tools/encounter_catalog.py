#!/usr/bin/env python3
"""Which field or world-map region starts which encounter, with which battle background.

    python3 tools/encounter_catalog.py                       # summary for every disc in disks/us/discN
    python3 tools/encounter_catalog.py --json build/encounters.json
    python3 tools/encounter_catalog.py --disc 3 --encounter 944              # places that start it
    python3 tools/encounter_catalog.py --encounters --bg-absent --json -     # per (disc, encounter), only those
                                                                             # whose background is not on the disc

Disc images: --img DIR (or an FF9.IMG with one --disc), else $FF9_DISKS, else disks/ in this checkout or in any
directory above it (find_image), so a submodule copy finds its superproject's disks/.

Formats and code paths: docs/encounters.md. In short:
- Every encounter bundle (dir index 5) names its background (dependency key 0x0C0000|bg). On a disc that lacks
  that background file the entry is sector 0 / length 0, and no field or world map on that disc starts the
  encounter; `bg_present_on_discs` says where it can be started.
- Every field file (FF9.IMG dir index 4) and world-map file (dir index 3, ids 9000+) starts with a dependency
  table (DB block 0x14, `GameData.unk7E4` at run time). An encounter the place can start has an entry with key
  0x110000|battle whose link field names the background key 0x0C0000|bg. fld_depFindBackground (ovl_0a) and
  wm_depFindBackground (ovl_0c) resolve exactly this when the battle starts and store the background's
  (sector, length) in GameData.unk7FC[1]/unk804[1].
- Field random tables come from the field's event script: opcode 0x3C (pattern, 4 battle ids), 0x57 (frequency).
  Scripted battles are opcodes 0x2A/0x8C. The script is scanned for these instructions with literal arguments
  (flags byte 0) and every hit is checked against the dependency table, so a scan hit is only kept when the
  place really lists that battle.
- World random tables are a pack inside dir index 1 (item 0x41B, or 0x41E after story 0x2B52): 10-byte entries
  {u16 battle[4], u8 topography<<2|pattern, u8 zone<<1|fog}, grouped by zone; area -> zone comes from ovl_0c.
Rows whose instruction could not be found (expression arguments, or not started by a script at all) keep
kind "listed".
"""
import argparse
import json
import os
import struct
import sys
from collections import defaultdict

SECTOR = 2048
ROOT = os.path.dirname(os.path.dirname(os.path.abspath(__file__)))

DIR_OVERLAYS, DIR_WORLDDATA, DIR_WORLDS, DIR_FIELDS, DIR_ENCOUNTERS = 0, 1, 3, 4, 5
KIND_FIELD, KIND_AREA, KIND_BACKGROUND, KIND_DEPS, KIND_BATTLE = 0x05, 0x06, 0x0C, 0x14, 0x11

# D_8006E648 (main exe): cumulative thresholds against one random byte, per pattern (func_80038D3C_pick)
PATTERNS = [(0x60, 0xA8, 0xE0), (0x40, 0x80, 0xC0), (0x5A, 0xA7, 0xF4), (0x73, 0xD9, 0xF4)]
# ovl_0c .data: D_800CCFB4 area -> zone (64 bytes), D_800CCFF4 entries/2 per zone (26 bytes)
OVL0C_AREA_ZONE, OVL0C_ZONE_COUNT, OVL0C_VRAM = 0x800CCFB4, 0x800CCFF4, 0x800A7000
LATE_WORLD_STORY = 0x2B52  # func_800B69B4: story counter >= this selects the late world data (0x41E)


def u16(b, o):
    return struct.unpack_from('<H', b, o)[0]


def u32(b, o):
    return struct.unpack_from('<I', b, o)[0]


DISKS_ENV = 'FF9_DISKS'
# layouts under a disks directory (or the checkout / superproject that holds one)
DISC_LAYOUTS = ('us/disc{n}/FF9.IMG', 'disc{n}/FF9.IMG', 'disks/us/disc{n}/FF9.IMG')


def disk_dirs():
    """Where the disc images may be, in order: $FF9_DISKS, this checkout's disks/, then every directory above
    the checkout (and an `ff9-decomp` checkout next to each), so a copy used as a git submodule (e.g. the DS
    port's decomp/) finds the superproject's or a sibling checkout's disks/."""
    out = []
    if os.environ.get(DISKS_ENV):
        out.append(os.environ[DISKS_ENV])
    d = ROOT
    while True:
        out += [os.path.join(d, 'disks'), os.path.join(d, 'ff9-decomp', 'disks')]
        up = os.path.dirname(d)
        if up == d:
            return out
        d = up


def find_image(disc, img=None):
    """FF9.IMG of a disc. `img` (the tools' --img) is either the FF9.IMG itself or a directory in one of the
    DISC_LAYOUTS; without it, disk_dirs() is searched. Returns the path, or None (with the places tried on
    stderr)."""
    tried = []
    for base in [img] if img else disk_dirs():
        if img and os.path.isfile(img):
            return img
        for lay in DISC_LAYOUTS:
            p = os.path.join(base, lay.format(n=disc))
            if os.path.isfile(p):
                return p
            tried.append(p)
    print(f'disc {disc}: no FF9.IMG (set --img DIR or ${DISKS_ENV}); tried ' + ', '.join(tried[:6])
          + (f' and {len(tried) - 6} more' if len(tried) > 6 else ''), file=sys.stderr)
    return None


def slot_shares(pattern):
    t = PATTERNS[pattern & 3]
    return [t[0] / 256, (t[1] - t[0]) / 256, (t[2] - t[1]) / 256, (256 - t[2]) / 256]


class Img:
    def __init__(self, path):
        self.f = open(path, 'rb')
        hdr = self.read(0, SECTOR)
        if hdr[:4] != b'FF9 ':
            raise ValueError(f'{path}: not FF9.IMG')
        self.dirs = [struct.unpack_from('<4I', hdr, 16 + i * 16) for i in range(u32(hdr, 8))]

    def read(self, off, n):
        self.f.seek(off)
        return self.f.read(n)

    def files(self, index):
        """[(file id, sector, sectors)] of a type-2 directory (third header word = table sector)."""
        kind, count, table, _ = self.dirs[index]
        if kind != 2:
            return []
        raw = self.read(table * SECTOR, (count + 1) * 8)
        ents = [struct.unpack_from('<2I', raw, i * 8) for i in range(count + 1)]
        return [(ents[i][0], ents[i][1], ents[i + 1][1] - ents[i][1]) for i in range(count)]

    def file(self, index, fid):
        for f, sec, n in self.files(index):
            if f == fid:
                return self.read(sec * SECTOR, n * SECTOR)
        return None


def db_items(data, base=0):
    """Items of a DB container (db_lookup in main): [(key, offset)], key = kind << 16 | id."""
    if data[base:base + 1] != b'\xdb':
        return []
    out = []
    for i in range(data[base + 1]):
        ent = base + 4 + i * 4
        v = u32(data, ent)
        blk = ent + (v & 0xFFFFFF)
        cnt = data[blk + 1]
        ids = struct.unpack_from(f'<{cnt}H', data, blk + 4)
        ofs = (blk + 4 + cnt * 2 + 3) & ~3
        for j, id_ in enumerate(ids):
            o = ofs + 4 * j
            out.append(((v >> 24 & 0x7F) << 16 | id_, o + (u32(data, o) & 0xFFFFFF)))
    return out


def db_find(data, key, base=0):
    for k, off in db_items(data, base):
        if k == key:
            return off
    return None


def dep_table(data):
    """DB block 0x14: {u16 ?, u16 n, n x {u32 key, u32 sector, u32 sectors, u32 link}}; first entry per key wins
    (the lookups scan from the start and stop at the first match)."""
    items = [off for k, off in db_items(data) if k >> 16 == KIND_DEPS]
    if not items:
        return {}
    d = items[0]
    out = {}
    for j in range(u16(data, d + 2)):
        key, sec, n, link = struct.unpack_from('<4I', data, d + 4 + j * 16)
        out.setdefault(key, (sec, n, link))
    return out


def scripts(data):
    """Event scripts ("EV" items) inside the file's nested containers."""
    found = []
    for k, off in db_items(data):
        for k2, off2 in db_items(data, off):
            if data[off2:off2 + 2] == b'EV':
                found.append((k2, off2))
    return found


def script_functions(data, s):
    """(entry, tag, code bytes): script + 0x80 holds 8-byte entries {u16 offset, u16 size, ...}; an entry starts
    with u16 (nfunc << 8) and nfunc {u16 tag, u16 offset}, code at entry + 2 + offset (func_800384D0_find)."""
    tab = s + 0x80
    for e in range(data[s + 3]):
        off, size = struct.unpack_from('<HH', data, tab + e * 8)
        h = tab + off
        nf = u16(data, h) >> 8
        fs = sorted((u16(data, h + 4 + 4 * j), u16(data, h + 2 + 4 * j)) for j in range(nf))
        for j, (fo, tag) in enumerate(fs):
            end = h + 2 + fs[j + 1][0] if j + 1 < nf else h + size
            yield e, tag, data[h + 2 + fo:end]


def scan_script(data, s, battles):
    """Literal-argument battle instructions: 0x3C random table, 0x57 frequency, 0x2A/0x8C battle."""
    tables, scripted, freqs = [], [], []
    for e, tag, code in script_functions(data, s):
        for i in range(len(code) - 3):
            op, flags = code[i], code[i + 1]
            if flags:
                continue
            where = {'entry': e, 'tag': tag, 'offset': i}
            if op == 0x3C and i + 11 <= len(code) and code[i + 2] <= 3:
                ids = list(struct.unpack_from('<4H', code, i + 3))
                if all(x in battles for x in ids):
                    tables.append(dict(where, pattern=code[i + 2], battles=ids))
                    if i >= 3 and code[i - 3] == 0x57 and code[i - 2] == 0:
                        freqs.append(code[i - 1])
            elif op in (0x2A, 0x8C):
                o = i + (3 if op == 0x2A else 4)
                if o + 2 <= len(code):
                    b = u16(code, o)
                    if b & 0x7FFF in battles:
                        scripted.append(dict(where, op=op, battle=b & 0x7FFF, flag15=b >> 15))
    return tables, scripted, freqs


def bg_of(deps, battle):
    """fld_depFindBattle + fld_depFindBackground: the battle entry's link, resolved in the same table."""
    ent = deps.get(KIND_BATTLE << 16 | battle)
    if ent is None:
        return None
    link = ent[2]
    if link >> 16 != KIND_BACKGROUND:
        return {'background': None, 'link': link}
    bg = deps.get(link)
    return {'background': link & 0xFFFF, 'bg_sector': bg[0] if bg else 0, 'bg_sectors': bg[1] if bg else 0}


def place_rows(disc, place, deps, data):
    battles = {k & 0xFFFF for k, (sec, n, link) in deps.items() if k >> 16 == KIND_BATTLE and sec}
    tables, scripted, freqs = [], [], []
    for _, s in scripts(data):
        t, sc, fr = scan_script(data, s, battles)
        tables += t
        scripted += sc
        freqs += fr
    rows = []
    random_ids = defaultdict(list)
    for t in tables:
        shares = slot_shares(t['pattern'])
        for slot, b in enumerate(t['battles']):
            random_ids[b].append({'pattern': t['pattern'], 'slot': slot, 'share': shares[slot],
                                  'script': [t['entry'], t['tag'], t['offset']]})
    scripted_ids = defaultdict(list)
    for s in scripted:
        scripted_ids[s['battle']].append([s['entry'], s['tag'], s['offset'], s['op']])
    for b in sorted(battles):
        base = dict(disc=disc, **place, encounter=b, **bg_of(deps, b))
        if b in random_ids:
            per_table = defaultdict(float)
            for r in random_ids[b]:
                per_table[tuple(r['script'])] += r['share']
            share = max(per_table.values())
            rows.append(dict(base, kind='random', share=round(share, 4), frequency=sorted(set(freqs)),
                             tables=random_ids[b]))
        if b in scripted_ids:
            rows.append(dict(base, kind='scripted', script=scripted_ids[b]))
        if b not in random_ids and b not in scripted_ids:
            rows.append(dict(base, kind='listed'))
    return rows


def world_tables(img, disc):
    """World random tables: both data sets present on this disc, plus ovl_0c's area -> zone map."""
    ovl = img.file(DIR_OVERLAYS, 0x0C)
    area_zone = list(ovl[OVL0C_AREA_ZONE - OVL0C_VRAM:][:64])
    counts = list(ovl[OVL0C_ZONE_COUNT - OVL0C_VRAM:][:26])
    starts = [sum(counts[:i]) * 2 for i in range(27)]
    out = []
    for fid, sec, n in img.files(DIR_WORLDDATA):
        data = img.read(sec * SECTOR, n * SECTOR)
        for key, late in ((0x41B, False), (0x41E, True)):
            p = db_find(data, key)
            if p is None:
                continue
            npk = u32(data, p) & 0x7FFFFFFF
            ofs = [u32(data, p + 4 + 4 * i) for i in range(npk)]
            tb, end = p + ofs[3], p + ofs[4]
            ents = [struct.unpack_from('<4HBB', data, tb + 10 * i) for i in range((end - tb) // 10)]
            rows4 = data[p + ofs[4]:p + ofs[5]]
            # func_800BAD98: when event bit k (vars 0xC2 | 0xC6 << 8) is set, the entries in row k
            # (1-based, 0xFFFF = none) get battle[3] = battle[2]
            drop = defaultdict(list)
            for r in range(len(rows4) // 24):
                for v in struct.unpack_from('<12H', rows4, r * 24):
                    if v != 0xFFFF:
                        drop[v - 1].append(r)
            for zone in range(26):
                for i in range(starts[zone], starts[zone + 1]):
                    # func_800BBBA8: late data and i < 100 -> entry i + 0xFE
                    j = i + 0xFE if late and i < 100 else i
                    if j >= len(ents):
                        continue
                    b0, b1, b2, b3, tp, zf = ents[i]
                    eb = ents[j][:4]
                    out.append({'data': f'{fid}:{key:#x}', 'late': late, 'entry': j, 'zone': zone,
                                'areas': [a for a in range(64) if area_zone[a] == zone],
                                'topography': tp >> 2, 'fog': zf & 1, 'pattern': ents[j][4] & 3,
                                'battles': list(eb), 'slot3_replaced_by_event_bits': drop.get(j, [])})
    return out


def world_rows(img, disc, world_deps):
    rows = []
    bgs = {}
    for fid, deps in world_deps.items():
        for k, (sec, n, link) in deps.items():
            if k >> 16 == KIND_BATTLE and sec:
                bgs.setdefault(k & 0xFFFF, (bg_of(deps, k & 0xFFFF), []))[1].append(fid & 0xFFFF)
    for t in world_tables(img, disc):
        shares = slot_shares(t['pattern'])
        for slot, b in enumerate(t['battles']):
            if b in t['battles'][:slot]:
                continue
            share = sum(shares[s] for s, x in enumerate(t['battles']) if x == b)
            bg, worlds = bgs.get(b, ({'background': None}, []))
            region = {k: t[k] for k in ('data', 'late', 'entry', 'zone', 'areas', 'topography', 'fog', 'pattern')}
            if t['slot3_replaced_by_event_bits']:
                region['slot3_replaced_by_event_bits'] = t['slot3_replaced_by_event_bits']
            rows.append(dict(disc=disc, source='world', region=region, encounter=b, **bg, kind='random',
                             share=round(share, 4), worlds=sorted(set(worlds))))
    return rows


def own_backgrounds(img):
    """Encounter bundles (dir index 5): {battle: (background id or None, present on this disc)}. Every bundle names
    its background (key 0x0C0000|bg); on discs where that background file is absent the entry has sector and
    length 0, which is also what the starting place's table holds there."""
    out = {}
    for fid, sec, n in img.files(DIR_ENCOUNTERS):
        data = img.read(sec * SECTOR, min(n, 8) * SECTOR)
        deps = dep_table(data) if data[:1] == b'\xdb' else {}
        bgs = [(k & 0xFFFF, m > 0) for k, (s, m, _) in deps.items() if k >> 16 == KIND_BACKGROUND]
        out[fid] = bgs[0] if bgs else (None, False)
    return out


def catalog(path, disc):
    img = Img(path)
    rows = []
    for fid, sec, n in img.files(DIR_FIELDS):
        data = img.read(sec * SECTOR, n * SECTOR)
        deps = dep_table(data)
        if deps:
            rows += place_rows(disc, {'source': 'field', 'field': fid & 0xFFFF, 'area': fid >> 16}, deps, data)
    world_deps, world_script = {}, {}
    for fid, sec, n in img.files(DIR_WORLDS):
        if not n:
            continue
        data = img.read(sec * SECTOR, n * SECTOR)
        deps = dep_table(data)
        if not deps:
            continue
        world_deps[fid] = deps
        # script-started battles on the world map, merged over the world files (9000+) that list them
        for r in place_rows(disc, {'source': 'world'}, deps, data):
            if r['kind'] != 'random':  # the world map's random battles come from world_tables, not 0x3C
                key = (r['encounter'], r['kind'])
                world_script.setdefault(key, dict(r, worlds=[]))['worlds'].append(fid & 0xFFFF)
    rows += world_rows(img, disc, world_deps)
    rnd = {r['encounter'] for r in rows if r['source'] == 'world'}
    rows += [r for (b, kind), r in sorted(world_script.items()) if kind == 'scripted' or b not in rnd]
    for r in rows:
        r['bg_present'] = bool(r.get('bg_sectors'))
    return rows, own_backgrounds(img)


def encounter_view(per_disc):
    """One record per (disc, encounter): its background, whether that file is on the disc, the places that start
    it there, and the discs where its background is present (where a fixture can start it)."""
    present = defaultdict(list)
    for disc, (rows, own) in per_disc.items():
        for b, (bg, ok) in own.items():
            if ok:
                present[b].append(disc)
    out = []
    for disc, (rows, own) in per_disc.items():
        starters = defaultdict(list)
        for r in rows:
            place = ({'field': r['field']} if r['source'] == 'field' else
                     {'world_region': r['region']} if 'region' in r else {'worlds': r['worlds']})
            starters[r['encounter']].append(dict(place, kind=r['kind'], background=r.get('background'),
                                                 bg_present=r['bg_present']))
        for b, (bg, ok) in sorted(own.items()):
            out.append({'disc': disc, 'encounter': b, 'background': bg, 'bg_present': ok,
                        'bg_present_on_discs': present[b], 'starters': starters.get(b, [])})
    return out


def summary(rows, own, disc):
    absent = {b for b, (bg, ok) in own.items() if not ok}
    started = {r['encounter'] for r in rows if r['bg_present']}
    mismatch = {r['encounter'] for r in rows if r.get('background') is not None and r['encounter'] in own
                and own[r['encounter']][0] not in (None, r['background'])}
    by = defaultdict(set)
    for r in rows:
        by[(r['source'], r['kind'])].add(r['encounter'])
    parts = ', '.join(f'{s} {k} {len(v)}' for (s, k), v in sorted(by.items()))
    print(f'disc {disc}: {len(own)} encounters, {len(absent)} whose background is not on this disc '
          f'({len(absent & started)} of them started here anyway), {len(started)} started here with a present '
          f'background, {len(mismatch)} place/bundle background mismatches; distinct encounters per source: {parts}')


def main():
    ap = argparse.ArgumentParser(description=__doc__, formatter_class=argparse.RawDescriptionHelpFormatter)
    ap.add_argument('--disc', type=int, action='append', choices=range(1, 5))
    ap.add_argument('--img', help='FF9.IMG (with a single --disc) or a directory holding discN/FF9.IMG, us/discN/ '
                    'or disks/us/discN/ (default: $FF9_DISKS, else disks/ here or in a directory above)')
    ap.add_argument('--encounter', type=int)
    ap.add_argument('--encounters', action='store_true',
                    help='one record per (disc, encounter) instead of one per (place, encounter)')
    ap.add_argument('--bg-absent', action='store_true', help='only encounters whose background is not on the disc')
    ap.add_argument('--json', help='write the records to this file ("-" = stdout)')
    a = ap.parse_args()
    per_disc = {}
    for disc in a.disc or [1, 2, 3, 4]:
        path = find_image(disc, a.img)
        if path is None:
            continue
        per_disc[disc] = catalog(path, disc)
        if a.json != '-':
            summary(*per_disc[disc], disc)
    if a.encounters:
        out = encounter_view(per_disc)
        if a.bg_absent:
            out = [r for r in out if not r['bg_present']]
    else:
        out = [r for rows, own in per_disc.values() for r in rows]
        if a.bg_absent:
            out = [r for r in out if not per_disc[r['disc']][1].get(r['encounter'], (None, False))[1]]
    if a.encounter is not None:
        out = [r for r in out if r['encounter'] == a.encounter]
    if a.json:
        text = json.dumps(out, indent=1)
        if a.json == '-':
            print(text)
        else:
            open(a.json, 'w').write(text + '\n')
    elif a.encounter is not None and a.encounters:
        for r in out:
            print(f"disc {r['disc']}  enc {r['encounter']:4}  bg {r['background']}"
                  f"{'' if r['bg_present'] else ' (absent)'}  present on discs {r['bg_present_on_discs']}"
                  f"  {len(r['starters'])} starters")
    elif a.encounter is not None:
        for r in out:
            place = (f"field {r['field']}" if r['source'] == 'field' else
                     f"world zone {r['region']['zone']} topo {r['region']['topography']} fog {r['region']['fog']}"
                     if 'region' in r else f"world {r['worlds']}")
            print(f"disc {r['disc']}  {place:34} enc {r['encounter']:4}  bg {r.get('background')}"
                  f"{'' if r['bg_present'] else ' (absent)'}  {r['kind']}"
                  f"{'  share %.3f' % r['share'] if 'share' in r else ''}")


if __name__ == '__main__':
    main()
