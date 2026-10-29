#!/usr/bin/env python3
"""Polygon / vertex budget of each battle: background + enemies (+ party), and the battle backgrounds themselves.

    python3 tools/battle_budget.py                          # one row per encounter, heaviest first
    python3 tools/battle_budget.py --battle 678 --battle 679
    python3 tools/battle_budget.py --name Clipper --sort bg
    python3 tools/battle_budget.py --top 15 --sort total
    python3 tools/battle_budget.py --backgrounds             # one row per background file, heaviest first
    python3 tools/battle_budget.py --backgrounds --bg 94 --records    # per group / record detail
    python3 tools/battle_budget.py --party                   # the party estimate and the models it uses
    python3 tools/battle_budget.py --json build/battle_budget.json

Disc images: --img DIR, $FF9_DISKS, or disks/ in this checkout or a directory above it (see
encounter_catalog.find_image). Formats: docs/battle-backgrounds.md (backgrounds), docs/battle-models.md (models),
docs/encounters.md (which background an encounter uses).

- Background: FF9.IMG dir index 6, file id = the id of the encounter bundle's dependency key 0x0C0000|bg. Counted:
  every record of every group (quads, triangles, projected vertices; a record's vertex count is padded to a
  multiple of 6, `used` counts the distinct vertices its polygons reference). Counts are for the whole arena,
  front and back faces: the game culls back faces (NCLIP) per frame.
- Enemies: the heaviest enemy group of the encounter (by polygons) that btl_pickEnemyGroup can pick (weights are
  percentages; a group left with no share is only started by event op 0x8C, which forces a group: `--any-group`
  counts those too, and the row notes a heavier forced group). Each placement that has its own model (the
  "copies" of battle_models.py) adds its type's model once. battle_models.py's per-type "max copies" can add up
  types that never share a group (e.g. Ghost x3 + Ladybug x3 in 221/223/840).
- Party: not in the encounter data (it depends on who is in the party). The constant column is an upper bound:
  the four heaviest of the 19 body models in g_btlCharModelIds (ovl_0b 0x800F5200, btl_getPartyModelId; FF9.IMG
  dir index 10, not 7) plus four times the heaviest weapon model (dir index 8). `--party` lists them.
A polygon is one FT4 or FT3 (a quad counts once), as in battle_models.py.
"""
import argparse
import json
import os
import struct
import sys
from collections import Counter, defaultdict

sys.path.insert(0, os.path.dirname(os.path.abspath(__file__)))
from encounter_catalog import Img, SECTOR, db_items, find_image, own_backgrounds  # noqa: E402
from battle_models import model_stats, scan_disc  # noqa: E402

DIR_OVERLAYS, DIR_BACKGROUNDS, DIR_WEAPONS, DIR_PARTY = 0, 6, 8, 10
KIND_TIM, KIND_GEOMETRY, KIND_TEXANIM, KIND_HEADER = 0x04, 0x0C, 0x19, 0x1A
GROUP_SIZE, REC_SIZE, HEADER_SIZE, SCROLL_SIZE = 0x18, 0x10, 0x10, 8
SCRATCH_VERTICES = 0x200 // 4   # btlbg_projectVertices: SXY at scratchpad + 4i, SZ at + 0x200 + 4i
RESIDENT = 0x6000               # btl_allocBuffers: bgData .. heapStart; the heap (models) overwrites the rest
LOAD_AREA = 0x6000 + 0x35800    # bgData .. end of the heap the whole file is first read into
LAYERS = {8: 'sky', 4: 'l4', 2: 'l2', 0: 'scene'}
# g_btlCharModelIds (ovl_0b .data): 0x13 body models, 0x13 other (trance) models, one more
OVL0B_VRAM, CHAR_MODEL_IDS, CHAR_MODELS = 0x800A7000, 0x800F5200, 0x13


def bg_stats(data, fid):
    """Header, groups and records of a background file (DB container: 0x0C geometry, 0x1A header, 0x19 texture
    animations, 0x04 TIMs), with consistency checks against what btlbg_draw / btlbg_drawRecord read."""
    items = db_items(data)
    first = {}
    for k, off in items:
        first.setdefault(k >> 16, (k, off))
    probs = []
    if KIND_GEOMETRY not in first or KIND_HEADER not in first:
        return {'problems': ['no 0x0C / 0x1A item']}
    g0 = first[KIND_GEOMETRY][1]
    if first[KIND_GEOMETRY][0] & 0xFFFF != fid:
        probs.append(f'geometry key {first[KIND_GEOMETRY][0]:#x}, file {fid}')
    h = first[KIND_HEADER][1]
    sid, ntexanim, skyspeed, ownproj, lastgroup, nscroll = struct.unpack_from('<6h', data, h)
    light = list(data[h + 0xC:h + 0xF])
    tims = sorted(off for k, off in items if k >> 16 == KIND_TIM)
    resident = (tims[0] if tims else len(data))
    out = {'file': fid, 'scene_id': sid, 'groups': [], 'tex_anims': ntexanim, 'sky_speed': skyspeed,
           'sky_own_projection': bool(ownproj), 'scrolls': nscroll, 'light': light, 'shadow': data[h + 0xF],
           'tims': len(tims), 'resident_bytes': resident, 'file_bytes': len(data), 'problems': probs}
    if resident > RESIDENT:
        probs.append(f'items before the TIMs end at {resident:#x} > {RESIDENT:#x}')
    if len(data) > LOAD_AREA:
        probs.append(f'file {len(data):#x} bytes > load area {LOAD_AREA:#x}')
    if (KIND_TEXANIM in first) != bool(ntexanim):
        probs.append(f'{ntexanim} texture animations, 0x19 item {"present" if KIND_TEXANIM in first else "absent"}')
    uv_ranges = []
    g = g0
    for gi in range(lastgroup + 1):
        (size, ncorner, nrec, recofs, ntex, texofs, nvert, vofs, npoly, cofs, iofs, uofs) = \
            struct.unpack_from('<12H', data, g)
        grp = {'records': [], 'tex_count': ntex}
        if recofs != GROUP_SIZE or texofs != recofs + nrec * REC_SIZE or vofs != texofs + ntex * 4:
            probs.append(f'group {gi}: offsets {recofs:#x} {texofs:#x} {vofs:#x}')
        if vofs & 3:
            probs.append(f'group {gi}: vertices at {vofs:#x}, not word aligned')
        vnext, cnext, inext, unext = 0, cofs, iofs, uofs
        for r in range(nrec):
            ro = g + recofs + r * REC_SIZE
            layer, nv, v0, rc, ri, ru, nq, nt = struct.unpack_from('<8H', data, ro)
            rc, ri, ru = (x + ro - g for x in (rc, ri, ru))
            where = f'group {gi} record {r}'
            if nv == 0 or nv % 6 or nv > SCRATCH_VERTICES:
                probs.append(f'{where}: {nv} vertices (needs a non-zero multiple of 6, at most {SCRATCH_VERTICES})')
            if layer not in LAYERS:
                probs.append(f'{where}: layer {layer}')
            unext = unext + 3 & ~3   # each record's index and uv arrays start on a word (read as words)
            if (v0, rc, ri, ru) != (vnext, cnext, inext, unext):
                probs.append(f'{where}: arrays at v{v0} {rc:#x} {ri:#x} {ru:#x}, expected v{vnext} {cnext:#x} '
                             f'{inext:#x} {unext:#x} (word aligned)')
            corners = nq * 4 + nt * 3
            idx = struct.unpack_from(f'<{corners}H', data, g + ri)
            used = {x >> 2 for x in idx}
            if any(x & 3 for x in idx) or max(used, default=0) >= nv:
                probs.append(f'{where}: vertex index out of range')
            cols = struct.unpack_from(f'<{nq + nt}I', data, g + rc)
            texs = {c >> 24 & 0x1F for c in cols}
            if max(texs, default=0) >= ntex:
                probs.append(f'{where}: texture {max(texs)} >= {ntex}')
            semi = sum(1 for c in cols if c >> 29 & 1)
            grp['records'].append({'layer': layer, 'vertices': nv, 'used_vertices': len(used), 'quads': nq,
                                   'triangles': nt, 'textures': sorted(texs), 'semi_transparent': semi})
            uv_ranges.append((g - g0 + ru, g - g0 + ru + corners * 2))
            vnext, cnext, inext, unext = v0 + nv, rc + (nq + nt) * 4, ri + (corners * 2 + 3 & ~3), ru + corners * 2
        recs = grp['records']
        sums = (sum(x['vertices'] for x in recs), sum(x['quads'] + x['triangles'] for x in recs),
                sum(x['quads'] * 4 + x['triangles'] * 3 for x in recs))
        if sums != (nvert, npoly, ncorner):
            probs.append(f'group {gi}: header counts {nvert} {npoly} {ncorner}, records sum to {sums}')
        if cofs != (vofs + nvert * 6 + 3) & ~3 or (unext + 3) & ~3 != size:
            probs.append(f'group {gi}: colors at {cofs:#x}, uvs end at {unext:#x}, size {size:#x}')
        out['groups'].append(grp)
        g += size
    if g > h:
        probs.append(f'groups end at {g:#x}, past the header at {h:#x}')
    for s in range(nscroll):
        o, mode = struct.unpack_from('<hB', data, h + HEADER_SIZE + s * SCROLL_SIZE)
        if mode > 1 or not any(a <= o < b for a, b in uv_ranges):
            probs.append(f'scroll {s}: uv offset {o:#x}, mode {mode}')
    recs = [r for grp in out['groups'] for r in grp['records']]
    out['records'] = len(recs)
    for k in ('quads', 'triangles', 'vertices', 'used_vertices', 'semi_transparent'):
        out[k] = sum(r[k] for r in recs)
    out['polys'] = out['quads'] + out['triangles']
    out['layer_polys'] = {LAYERS.get(lay, str(lay)): sum(r['quads'] + r['triangles'] for r in recs
                                                         if r['layer'] == lay)
                          for lay in sorted({r['layer'] for r in recs}, reverse=True)}
    return out


def scan_backgrounds(img):
    out = {}
    for fid, sec, n in img.files(DIR_BACKGROUNDS):
        if n:
            out[fid] = bg_stats(img.read(sec * SECTOR, n * SECTOR), fid)
    return out


def party_models(img):
    """Body models of g_btlCharModelIds (dir index 10: file id word = split sectors << 16 | model id) and the
    heaviest weapon (dir index 8, model data in the first 0x1000 bytes, btl_loadWeaponModelsStep)."""
    ovl = img.file(DIR_OVERLAYS, 0x0B)
    ids = struct.unpack_from(f'<{2 * CHAR_MODELS}H', ovl, CHAR_MODEL_IDS - OVL0B_VRAM)
    files = {f & 0xFFFF: (f >> 16, sec) for f, sec, n in img.files(DIR_PARTY)}
    bodies = []
    for i, mid in enumerate(ids):
        if mid not in files:
            continue
        split, sec = files[mid]
        st = model_stats(img.read(sec * SECTOR, split * SECTOR), mid)
        if st:
            bodies.append({'index': i, 'model': mid, 'other': i >= CHAR_MODELS, 'polys': st['poly_total'],
                           'vertices': st['vertices']})
    weapons = []
    for f, sec, n in img.files(DIR_WEAPONS):
        st = model_stats(img.read(sec * SECTOR, 0x1000), f & 0xFFFF)
        if st:
            weapons.append({'model': f & 0xFFFF, 'polys': st['poly_total'], 'vertices': st['vertices']})
    seen, top = set(), []
    for b in sorted((b for b in bodies if not b['other']), key=lambda b: -b['polys']):
        if b['model'] not in seen:
            seen.add(b['model'])
            top.append(b)
    top = top[:4]
    w = max(weapons, key=lambda x: x['polys'])
    est = {'polys': sum(b['polys'] for b in top) + 4 * w['polys'],
           'vertices': sum(b['vertices'] for b in top) + 4 * w['vertices'], 'bodies': top, 'weapon': w}
    return {'bodies': bodies, 'weapons': weapons, 'estimate': est}


def encounter_rows(disc, path, bgs_all, any_group=False):
    img = Img(path)
    groups = {}
    models, rows, problems = scan_disc(path, disc, groups)
    own = own_backgrounds(img)
    types = defaultdict(dict)
    for r in rows:
        types[r['battle']][r['type']] = r
    out = []
    for battle, grps in sorted(groups.items()):
        bg, present = own.get(battle, (None, False))
        bst = bgs_all.get(bg)
        cands, total = [], 0
        for gi, g in enumerate(grps):
            # btl_pickEnemyGroup: r = rand * 100 / 256 picks the first group whose running weight sum exceeds r;
            # a group left with no share is only started by op 0x8C's group argument (GameData.unk848)
            share = max(0, min(total + g['weight'], 100) - min(total, 100))
            total += g['weight']
            cnt = Counter(e['type'] for e in g['enemies'] if not e['no_model'])
            ens, polys, verts = [], 0, 0
            for t, n in sorted(cnt.items()):
                ty = types[battle].get(t)
                st = (models.get((disc, ty['model'])) or {}).get('stats') if ty and ty['model_loaded'] else None
                p, v = (st['poly_total'], st['vertices']) if st else (0, 0)
                ens.append({'type': t, 'name': ty['name'] if ty else None, 'model': ty['model'] if ty else None,
                            'polys': p, 'vertices': v, 'copies': n, 'loaded': st is not None})
                polys += p * n
                verts += v * n
            cands.append({'group': gi, 'share': share, 'enemies': ens, 'polys': polys, 'vertices': verts})
        none = {'group': None, 'share': 0, 'enemies': [], 'polys': 0, 'vertices': 0}
        best = max((c for c in cands if c['share'] or any_group), key=lambda c: c['polys'], default=none)
        forced = max(cands, key=lambda c: c['polys'], default=none)
        bp, bv = (bst['polys'], bst['vertices']) if bst else (0, 0)
        out.append({'disc': disc, 'battle': battle, 'background': bg, 'bg_present': present,
                    'bg_polys': bp, 'bg_vertices': bv, 'enemy_group': best['group'], 'enemies': best['enemies'],
                    'enemy_polys': best['polys'], 'enemy_vertices': best['vertices'],
                    'total_polys': bp + best['polys'], 'total_vertices': bv + best['vertices'],
                    'enemy_group_share': best['share'], 'enemy_groups': cands,
                    'forced_group': forced if forced['polys'] > best['polys'] else None})
    return out, problems


def main():
    ap = argparse.ArgumentParser(description=__doc__, formatter_class=argparse.RawDescriptionHelpFormatter)
    ap.add_argument('--disc', type=int, action='append', choices=range(1, 5))
    ap.add_argument('--img', help='FF9.IMG (with a single --disc) or a directory holding discN/FF9.IMG, us/discN/ '
                    'or disks/us/discN/ (default: $FF9_DISKS, else disks/ here or in a directory above)')
    ap.add_argument('--battle', type=int, action='append', help='only these encounters')
    ap.add_argument('--bg', type=int, action='append', help='only these backgrounds')
    ap.add_argument('--name', action='append', help='only encounters with an enemy whose name contains this')
    ap.add_argument('--all', action='store_true', help='also (disc, encounter) pairs whose background is not on '
                    'that disc (the game never starts them there, docs/encounters.md)')
    ap.add_argument('--any-group', action='store_true', help='heaviest of all enemy groups, including those only '
                    'a script forces (op 0x8C); default: only groups btl_pickEnemyGroup can pick')
    ap.add_argument('--sort', choices=('total', 'bg', 'enemies', 'battle'), default='total')
    ap.add_argument('--verts', action='store_true', help='sort by vertices instead of polygons')
    ap.add_argument('--top', type=int, help='only the first N rows')
    ap.add_argument('--per-disc', action='store_true', help='one row per (disc, encounter), not per encounter')
    ap.add_argument('--backgrounds', action='store_true', help='one row per background file')
    ap.add_argument('--records', action='store_true', help='with --backgrounds: per group and record detail')
    ap.add_argument('--party', action='store_true', help='the party models behind the party column')
    ap.add_argument('--json', help='write {backgrounds, encounters, party} to this file ("-" = stdout)')
    a = ap.parse_args()

    bgs, bg_discs, rows, problems, party = {}, defaultdict(list), [], [], None
    for disc in a.disc or [1, 2, 3, 4]:
        path = find_image(disc, a.img)
        if path is None:
            continue
        img = Img(path)
        for fid, st in scan_backgrounds(img).items():
            bgs.setdefault(fid, st)
            bg_discs[fid].append(disc)
            problems += [f'disc {disc} background {fid}: {p}' for p in st['problems']]
        if party is None:
            party = party_models(img)
        r, p = encounter_rows(disc, path, bgs, a.any_group)
        rows += r
        problems += [f'disc {disc}: {x}' for x in p]
    if not rows:
        sys.exit('no disc images found')
    for r in rows:   # backgrounds seen on a later disc only
        st = bgs.get(r['background'])
        if st and not r['bg_polys']:
            r.update(bg_polys=st['polys'], bg_vertices=st['vertices'], total_polys=st['polys'] + r['enemy_polys'],
                     total_vertices=st['vertices'] + r['enemy_vertices'])
    pe = party['estimate']
    for r in rows:
        r['with_party_polys'] = r['total_polys'] + pe['polys']
        r['with_party_vertices'] = r['total_vertices'] + pe['vertices']
    used_by = defaultdict(lambda: defaultdict(list))
    for r in rows:
        if r['bg_present']:
            used_by[r['background']][r['disc']].append(r['battle'])
    if not a.all:
        rows = [r for r in rows if r['bg_present']]
    if a.battle:
        rows = [r for r in rows if r['battle'] in a.battle]
    if a.bg:
        rows = [r for r in rows if r['background'] in a.bg]
    if a.name:
        rows = [r for r in rows if any(e['name'] and n.lower() in e['name'].lower()
                                       for e in r['enemies'] for n in a.name)]
    unit = 'vertices' if a.verts else 'polys'
    key = {'total': lambda r: -r['total_' + unit], 'bg': lambda r: -r['bg_' + unit],
           'enemies': lambda r: -r['enemy_' + unit], 'battle': lambda r: (r['battle'], r['disc'])}[a.sort]
    if not a.per_disc:   # one row for the discs where an encounter is the same (the bundles are)
        merged = {}
        for r in rows:
            sig = json.dumps({k: v for k, v in r.items() if k != 'disc'}, sort_keys=True)
            m = merged.setdefault(sig, dict(r, discs=[]))
            m['discs'].append(r['disc'])
        rows = list(merged.values())
    else:
        for r in rows:
            r['discs'] = [r['disc']]
    rows.sort(key=key)
    if a.top:
        rows = rows[:a.top]
    if a.json:
        out = {'backgrounds': [dict(st, discs=bg_discs[f], encounters={d: v for d, v in used_by[f].items()})
                               for f, st in sorted(bgs.items())],
               'encounters': rows, 'party': party, 'problems': problems}
        text = json.dumps(out, indent=1)
        if a.json == '-':
            print(text)
        else:
            open(a.json, 'w').write(text + '\n')
        return
    for p in problems:
        print('problem:', p, file=sys.stderr)
    if a.party:
        print(f"party estimate: {pe['polys']} polygons, {pe['vertices']} vertices = bodies "
              + ' + '.join(f"model {b['model']} ({b['polys']}/{b['vertices']})" for b in pe['bodies'])
              + f" + 4 x weapon model {pe['weapon']['model']} ({pe['weapon']['polys']}/{pe['weapon']['vertices']})")
        print('g_btlCharModelIds index: model polys/vertices (dir index 10; 0x13+ = the other model)')
        print('  ' + ', '.join(f"{b['index']:#x}: {b['model']} {b['polys']}/{b['vertices']}" for b in party['bodies']))
        ws = sorted(w['polys'] for w in party['weapons'])
        print(f"weapons (dir index 8): {len(ws)} models, {ws[0]}..{ws[-1]} polygons")
        return
    if a.backgrounds:
        sel = [f for f in bgs if not a.bg or f in a.bg]
        sel.sort(key=lambda f: -bgs[f][unit if unit == 'polys' else 'vertices'])
        if a.top:
            sel = sel[:a.top]
        print('  bg scene discs grp rec quads  tris polys verts  used  sky/l4/l2/scene polys  resident  encounters')
        for f in sel:
            st = bgs[f]
            lp = st['layer_polys']
            lay = '/'.join(str(lp.get(k, 0)) for k in ('sky', 'l4', 'l2', 'scene'))
            same = defaultdict(list)   # discs with the same encounter list
            for d, v in sorted(used_by[f].items()):
                same[tuple(sorted(v))].append(str(d))
            enc = '; '.join(f"disc {''.join(ds)}: " + ' '.join(map(str, v[:12]))
                            + (f' +{len(v) - 12}' if len(v) > 12 else '') for v, ds in same.items())
            print(f"{f:4} {st['scene_id']:5} {''.join(map(str, bg_discs[f])):>5} {len(st['groups']):3} "
                  f"{st['records']:3} {st['quads']:5} {st['triangles']:5} {st['polys']:5} {st['vertices']:5} "
                  f"{st['used_vertices']:5}  {lay:>19}  {st['resident_bytes']:#7x}  {enc or '-'}")
            if a.records:
                for gi, g in enumerate(st['groups']):
                    for ri, r in enumerate(g['records']):
                        print(f"        group {gi} record {ri}: layer {r['layer']}, {r['quads']} quads, "
                              f"{r['triangles']} triangles, {r['vertices']} vertices ({r['used_vertices']} used), "
                              f"textures {r['textures']}, {r['semi_transparent']} semi-transparent")
        return
    print(f"party column: +{pe['polys']} polygons / +{pe['vertices']} vertices (upper bound, --party)")
    print('discs battle   bg  bg polys/verts  enemy polys/verts  total polys/verts  +party   enemies (heaviest group, '
          'its chance)')
    for r in rows:
        ens = ', '.join(f"{e['name'] or '?'} {e['polys']}x{e['copies']}" + ('' if e['loaded'] else ' (no model)')
                        for e in r['enemies']) or '-'
        ens += f" ({r['enemy_group_share']}%)" if r['enemies'] else ''
        f = r['forced_group']
        if f:
            ens += f"; forced group {f['group']}: " + ', '.join(f"{e['name'] or '?'} {e['polys']}x{e['copies']}"
                                                            for e in f['enemies']) + f" = {f['polys']}"
        bg = '-' if r['background'] is None else str(r['background'])
        print(f"{''.join(map(str, r['discs'])):>5} {r['battle']:6} {bg:>4} {r['bg_polys']:6}/{r['bg_vertices']:<6}"
              f" {r['enemy_polys']:7}/{r['enemy_vertices']:<8} {r['total_polys']:7}/{r['total_vertices']:<8}"
              f" {r['with_party_polys']:6}   {ens}" + ('' if r['bg_present'] else '  (background not on this disc)'))


if __name__ == '__main__':
    main()
