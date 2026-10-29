#!/usr/bin/env python3
"""Which model file each encounter's enemy types load, and how heavy each model is.

    python3 tools/battle_models.py                         # one row per enemy model, heaviest first, all discs
    python3 tools/battle_models.py --name Hecteyes --name Ochu
    python3 tools/battle_models.py --encounters --disc 3   # one row per (encounter, enemy type)
    python3 tools/battle_models.py --model 512 --meshes    # per mesh group detail
    python3 tools/battle_models.py --json build/battle_models.json

Reads disks/us/discN/FF9.IMG (no emulator). Formats and code paths: docs/battle-models.md, docs/encounters.md.
- Encounter bundle (dir index 5, file id = battle id): its first item of DB block 0x10 is the encounter header
  (`Battle11C020`: u8 ?, u8 ngroups, u8 ntypes, u8 nabilities, ...; then ngroups 0x38-byte groups, then ntypes
  0x74-byte enemy type records). The record's u16 at +0x1E is the type's **model id** (`btl_getEnemyModelId`; the
  six u16 at +0x20 are its motion ids, keys 0x03xxxx, `btl_initEnemyType`). Names: block 0x06's first item, u16
  offsets at +4 (one per type, then one per ability), text at +4 + offset, FF9 encoding (letter = byte + 0x31).
- `btl_loadEnemyModelsStep` loads every key 0x02xxxx of the bundle's dependency table (block 0x14): entry
  `{key, sector, sectors, split}`, read as two chunks: `split` sectors of model data (a DB container whose block
  0x1B item 0 is a nested container with the model header 0x02|id and its motions 0x03xxxx), then the rest
  (block 0x1B item 1: TIM textures 0x04xxxx). The entry is FF9.IMG **dir index 7, file id = model id**; this tool
  checks that for every entry.
- The model header (`BtlModelHeader`) and its mesh groups are counted here as the loader leaves them.
"""
import argparse
import json
import os
import struct
import sys

sys.path.insert(0, os.path.dirname(os.path.abspath(__file__)))
from encounter_catalog import Img, SECTOR, ROOT, db_items, db_find, u16, u32  # noqa: E402

DIR_ENCOUNTERS, DIR_MODELS = 5, 7
KIND_MODEL, KIND_DEPS, KIND_ENC_HEADER, KIND_TEXTS, KIND_PACK = 0x02, 0x14, 0x10, 0x06, 0x1B
GROUP_SIZE, TYPE_SIZE = 0x38, 0x74
# Eight primitive lists in file order: (name, source record size, vertices per polygon, GPU packet size)
PRIM_TYPES = [('FT4', 0x18, 4, 0x28), ('FT3', 0x14, 3, 0x20), ('GT4', 0x20, 4, 0x34), ('GT3', 0x18, 3, 0x28),
              ('G4', 0x18, 4, 0x24), ('G3', 0x14, 3, 0x1C), ('F4', 0x10, 4, 0x18), ('F3', 0xC, 3, 0x14)]
# Offsets inside a source record: vertex indices, uv indices (textured), colour indices (gouraud)
PRIM_FIELDS = {'FT4': (0x0, 0x8, None), 'FT3': (0x0, 0xC, None), 'GT4': (0x0, 0x8, 0x10), 'GT3': (0x0, 0x6, 0x10),
               'G4': (0x0, None, 0x8), 'G3': (0x0, None, 0x8), 'F4': (0x0, None, None), 'F3': (0x0, None, None)}
VERTEX_BUFFER = 0x321  # g_vtxScreenZ holds 802 entries; GeoSplitVertices clamps a group to 0x321 vertices


def ff9_text(b, o):
    out = []
    while o < len(b) and b[o] != 0xFF:
        c = b[o]
        out.append(chr(c + 0x31) if 0x10 <= c <= 0x49 else ' ' if c == 0x0F else f'<{c:02X}>')
        o += 1
    return ''.join(out)


def dep_entries(data):
    """All entries of the bundle's dependency table, in order (the enemy loader takes every 0x02 key)."""
    for k, off in db_items(data):
        if k >> 16 == KIND_DEPS:
            return [struct.unpack_from('<4I', data, off + 4 + j * 16) for j in range(u16(data, off + 2))]
    return []


def encounter(data):
    """(groups, types) of an encounter bundle; types: [{model, motions, name}]."""
    h = db_find_first(data, KIND_ENC_HEADER)
    if h is None:
        return None
    ngroups, ntypes = data[h + 1], data[h + 2]
    groups = []
    for g in range(ngroups):
        o = h + 8 + g * GROUP_SIZE
        groups.append({'weight': data[o], 'enemies': [
            {'type': data[o + 8 + 12 * j], 'no_model': bool(j > 0 and data[o + 9 + 12 * j] & 2)}
            for j in range(min(data[o + 1], 4))]})
    texts = db_find_first(data, KIND_TEXTS)
    types = []
    for i in range(ntypes):
        r = h + 8 + ngroups * GROUP_SIZE + i * TYPE_SIZE
        name = ff9_text(data, texts + 4 + u16(data, texts + 4 + 4 * i)) if texts is not None else None
        types.append({'model': u16(data, r + 0x1E), 'motions': list(struct.unpack_from('<6H', data, r + 0x20)),
                      'name': name})
    return groups, types


def db_find_first(data, kind, base=0):
    """Item 0 of block `kind` (db_lookup mode 2 index 0, what db_lookupRegistered(9, key, kind << 16) resolves)."""
    for k, off in db_items(data, base):
        if k >> 16 == kind:
            return off
    return None


def model_stats(data, mid):
    """Counts of the model header 0x02|mid inside a model file (dir index 7)."""
    pack = db_find(data, KIND_PACK << 16)
    if pack is None:
        return None
    h = db_find(data, KIND_MODEL << 16 | mid, pack)
    if h is None:
        return None
    flags, nbones, nmesh, packet = struct.unpack_from('<HBBH', data, h)
    height, radius, bones_ofs, mesh_ofs = struct.unpack_from('<HHiI', data, h + 8)
    out = {'bones': nbones, 'meshes': [], 'packet_size': packet, 'height': height, 'radius': radius,
           'problems': []}
    psum = 0
    for i in range(nmesh):
        m = h + mesh_ofs + i * 0x28
        msize = u16(data, m)
        counts = struct.unpack_from('<8H', data, m + 2)
        otbias = struct.unpack_from('<b', data, m + 0x13)[0]
        vc_o, v_o, p_o, uv_o, c_o = (h + x for x in struct.unpack_from('<5I', data, m + 0x14))
        vcounts = struct.unpack_from(f'<{nbones}H', data, vc_o)
        nv = sum(vcounts)
        mesh = {'packet_size': msize, 'ot_bias': otbias, 'vertices': nv,
                'polys': dict(zip([t[0] for t in PRIM_TYPES], counts)),
                'bone_vertices': {b: n for b, n in enumerate(vcounts) if n},
                'uvs': (c_o - uv_o) // 2, 'vertex_bytes': p_o - v_o}
        # walk the sources: index ranges and the packet size the counts imply
        p = p_o
        maxv = maxuv = maxc = -1
        texs = set()
        want = 0
        for (name, size, nvert, pkt), n in zip(PRIM_TYPES, counts):
            vf, uf, cf = PRIM_FIELDS[name]
            for _ in range(n):
                maxv = max(maxv, *struct.unpack_from(f'<{nvert}H', data, p + vf))
                if uf is not None:
                    maxuv = max(maxuv, *struct.unpack_from(f'<{nvert}H', data, p + uf))
                if cf is not None:
                    maxc = max(maxc, *struct.unpack_from(f'<{nvert}H', data, p + cf))
                tex = {'FT4': 0x13, 'FT3': 0x6, 'GT4': 0x1D, 'GT3': 0x16}.get(name)
                if tex is not None:
                    texs.add(data[p + tex])
                p += size
            want += n * pkt
        mesh.update({'max_vertex_index': maxv, 'max_uv_index': maxuv, 'max_colour_index': maxc,
                     'textures': sorted(texs), 'prim_bytes': p - p_o})
        if p != uv_o:
            out['problems'].append(f'mesh {i}: sources end at {p - h:#x}, uvs start at {uv_o - h:#x}')
        if v_o + nv * 8 != p_o:
            out['problems'].append(f'mesh {i}: {nv} vertices but {p_o - v_o:#x} vertex bytes')
        if want != msize:
            out['problems'].append(f'mesh {i}: packet size {msize:#x}, counts imply {want:#x}')
        if maxv >= nv:
            out['problems'].append(f'mesh {i}: vertex index {maxv} >= {nv} vertices')
        if nv > VERTEX_BUFFER:
            out['problems'].append(f'mesh {i}: {nv} vertices > vertex buffer {VERTEX_BUFFER}')
        psum += msize
        out['meshes'].append(mesh)
    if psum != packet:
        out['problems'].append(f'header packet size {packet:#x}, meshes sum to {psum:#x}')
    out['vertices'] = sum(m['vertices'] for m in out['meshes'])
    out['polys'] = {t[0]: sum(m['polys'][t[0]] for m in out['meshes']) for t in PRIM_TYPES}
    out['poly_total'] = sum(out['polys'].values())
    return out


def scan_disc(path, disc):
    img = Img(path)
    where = {}
    for i in range(len(img.dirs)):
        for fid, sec, n in img.files(i):
            where[sec] = (i, fid, n)
    models, rows, problems = {}, [], []
    for battle, sec, n in img.files(DIR_ENCOUNTERS):
        data = img.read(sec * SECTOR, n * SECTOR)
        if data[:1] != b'\xdb':
            continue
        enc = encounter(data)
        if enc is None:
            continue
        groups, types = enc
        deps = {}
        for key, msec, nsec, split in dep_entries(data):
            if key >> 16 == KIND_MODEL:
                deps.setdefault(key & 0xFFFF, (msec, nsec, split))
        for mid, (msec, nsec, split) in deps.items():
            if (disc, mid) in models or nsec == 0:
                continue
            d, fid, fsec = where.get(msec, (None, None, None))
            if (d, fid) != (DIR_MODELS, mid):
                problems.append(f'battle {battle}: model {mid} at sector {msec} is dir {d} file {fid}')
            mdata = img.read(msec * SECTOR, split * SECTOR)
            st = model_stats(mdata, mid)
            models[(disc, mid)] = {'disc': disc, 'model': mid, 'dir': d, 'file': fid, 'sector': msec,
                                   'sectors': nsec, 'model_sectors': split, 'stats': st}
        for t, ty in enumerate(types):
            # drawn copies per group: placements of this type that have their own model (btl_initEnemies)
            copies = max([sum(1 for e in g['enemies'] if e['type'] == t and not e['no_model']) for g in groups] or [0])
            present = ty['model'] in deps and deps[ty['model']][1] > 0
            rows.append({'disc': disc, 'battle': battle, 'type': t, 'name': ty['name'], 'model': ty['model'],
                         'model_loaded': present, 'max_copies': copies, 'motions': ty['motions']})
    return models, rows, problems


def main():
    ap = argparse.ArgumentParser(description=__doc__, formatter_class=argparse.RawDescriptionHelpFormatter)
    ap.add_argument('--disc', type=int, action='append', choices=range(1, 5))
    ap.add_argument('--img', help='FF9.IMG path (with a single --disc)')
    ap.add_argument('--name', action='append', help='only enemy types whose name contains this (any case)')
    ap.add_argument('--model', type=int, action='append', help='only these model ids')
    ap.add_argument('--battle', type=int, action='append', help='only these encounters')
    ap.add_argument('--encounters', action='store_true', help='one row per (disc, encounter, enemy type)')
    ap.add_argument('--meshes', action='store_true', help='per mesh group counts under each model')
    ap.add_argument('--top', type=int, help='only the N heaviest models (by vertices)')
    ap.add_argument('--json', help='write {models, enemies} to this file ("-" = stdout)')
    a = ap.parse_args()
    models, rows, problems = {}, [], []
    for disc in a.disc or [1, 2, 3, 4]:
        path = a.img or os.path.join(ROOT, f'disks/us/disc{disc}/FF9.IMG')
        if not os.path.exists(path):
            print(f'disc {disc}: {path} missing', file=sys.stderr)
            continue
        m, r, p = scan_disc(path, disc)
        models.update(m)
        rows += r
        problems += [f'disc {disc}: {x}' for x in p]
    if a.name:
        rows = [r for r in rows if r['name'] and any(n.lower() in r['name'].lower() for n in a.name)]
    if a.model:
        rows = [r for r in rows if r['model'] in a.model]
    if a.battle:
        rows = [r for r in rows if r['battle'] in a.battle]
    keep = {(r['disc'], r['model']) for r in rows}
    if a.json:
        out = {'models': [m for k, m in sorted(models.items()) if k in keep], 'enemies': rows,
               'problems': problems}
        text = json.dumps(out, indent=1)
        if a.json == '-':
            print(text)
        else:
            open(a.json, 'w').write(text + '\n')
        return
    for p in problems:
        print('problem:', p, file=sys.stderr)
    for m in models.values():
        for p in (m['stats'] or {}).get('problems', []):
            print(f"problem: disc {m['disc']} model {m['model']}: {p}", file=sys.stderr)
    types = ' '.join(f'{t[0]:>4}' for t in PRIM_TYPES)
    if a.encounters:
        print(f"disc battle t {'name':16} model  verts polys  {types}  copies")
        for r in rows:
            st = (models.get((r['disc'], r['model'])) or {}).get('stats')
            if st is None:
                print(f"{r['disc']:4} {r['battle']:6} {r['type']} {r['name'] or '':16} {r['model']:5}  (no model loaded)")
                continue
            print(f"{r['disc']:4} {r['battle']:6} {r['type']} {r['name'] or '':16} {r['model']:5} {st['vertices']:6}"
                  f" {st['poly_total']:5}  " + ' '.join(f"{st['polys'][t[0]]:4}" for t in PRIM_TYPES)
                  + f"  x{r['max_copies']}")
        return
    # one row per model id (identical on every disc that has it)
    by_model = {}
    for r in rows:
        e = by_model.setdefault(r['model'], {'names': set(), 'battles': set(), 'discs': set(), 'copies': 0})
        e['copies'] = max(e['copies'], r['max_copies'])
        if r['name']:
            e['names'].add(r['name'])
        e['battles'].add(r['battle'])
        if r['model_loaded']:
            e['discs'].add(r['disc'])
    out = []
    for mid, e in by_model.items():
        m = next((models[(d, mid)] for d in sorted(e['discs']) if (d, mid) in models), None)
        if m is None or m['stats'] is None:
            continue
        out.append((m, e))
    out.sort(key=lambda x: -x[0]['stats']['vertices'])
    if a.top:
        out = out[:a.top]
    print(f"model dir/file d:sector  sects bones mesh  verts polys  {types}  max copies  names / battles")
    for m, e in out:
        st = m['stats']
        battles = sorted(e['battles'])
        bl = ' '.join(map(str, battles[:8])) + (f' +{len(battles) - 8}' if len(battles) > 8 else '')
        print(f"{m['model']:5} {m['dir']}/{m['file']:<5} {m['disc']}:{m['sector']:<6} {m['sectors']:4} {st['bones']:5}"
              f" {len(st['meshes']):4} {st['vertices']:6} {st['poly_total']:5}  "
              + ' '.join(f"{st['polys'][t[0]]:4}" for t in PRIM_TYPES)
              + f"  x{e['copies']}  {', '.join(sorted(e['names']))} / discs {''.join(map(str, sorted(e['discs'])))} battles {bl}")
        if a.meshes:
            for i, g in enumerate(st['meshes']):
                bones = ' '.join(f'{b}:{n}' for b, n in g['bone_vertices'].items())
                print(f"      mesh {i}: {g['vertices']} vertices, {g['uvs']} uvs, ot bias {g['ot_bias']}, "
                      f"textures {g['textures']}, polys "
                      + ' '.join(f"{k} {v}" for k, v in g['polys'].items() if v) + f"; bone:vertices {bones}")


if __name__ == '__main__':
    main()
