#!/usr/bin/env python3
"""Field models: which file each one comes from, how heavy it is, and which fields load it.

    python3 tools/field_models.py                        # one row per field model, heaviest first, all discs
    python3 tools/field_models.py --top 20               # the 20 heaviest models
    python3 tools/field_models.py --party                # the party's models (every form)
    python3 tools/field_models.py --field 1554 --disc 2  # one field: its actors' models
    python3 tools/field_models.py --fields --top 15      # heaviest fields (sum over the models they load)
    python3 tools/field_models.py --model 98 --meshes    # per mesh group detail
    python3 tools/field_models.py --check                # validation pass over every disc (exit 1 on a problem)
    python3 tools/field_models.py --json build/field_models.json
    python3 tools/field_models.py --pose 98 --field 2600 [--anim 0x30019] [--frame 0] [--bones] [--obj out.obj]

Reads discN/FF9.IMG (no emulator) from --img DIR, $FF9_DISKS, or disks/ in this checkout or a directory above it
(encounter_catalog.find_image). Formats and code paths: docs/field-models.md. In short:
- A field file (FF9.IMG dir index 4, file id = area << 16 | field) starts with a dependency table (DB block 0x14,
  GameData.depTable). Every model the field can show has an entry {0x02xxxx | model id, sector, sectors, link}:
  `link` is the event-script entry (the actor) that loads it (fld_queueModelBundle -> fld_depFindByLink).
  - sector 0 / sectors 0: the model is in the field file itself, item 0x02xxxx of its nested container
    0x1B0001 (registered by fld_loadFile), with its motions (0x03xxxx), model info (0x120000) and texture
    animations (0x19xxxx). Its textures are TIMs in the field's container 0x1B0000.
  - otherwise: FF9.IMG **dir index 10**, file id & 0xFFFF = model id (file_find(10, key) in fld_loadModelBundle).
    Only the party's models are stored there (they are cached in RAM across fields, g_fieldModelCache).
- Script entries scriptCount-9 .. scriptCount-1 are the party characters 0..8 (evt_getObjectChrIndex), so a link
  in that range names a party form.
- The model header (WmModel = battle BtlUnit9C8) and its groups have exactly the battle layout
  (docs/battle-models.md); counts come from battle_models.header_stats. Motions have the battle format too, so
  --pose reuses tools/battle_pose.py.
"""
import argparse
import hashlib
import json
import os
import struct
import sys
from collections import defaultdict

sys.path.insert(0, os.path.dirname(os.path.abspath(__file__)))
from encounter_catalog import Img, SECTOR, db_items, db_find, find_image, scripts, u16  # noqa: E402
from battle_models import PRIM_TYPES, header_stats  # noqa: E402

DIR_OVERLAYS, DIR_WORLDS, DIR_FIELDS, DIR_CHAR_MODELS = 0, 3, 4, 10
KIND_MODEL, KIND_MOTION, KIND_INFO, KIND_DEPS, KIND_TEXANIM, KIND_PACK = 0x02, 0x03, 0x12, 0x14, 0x19, 0x1B
PARTY_SLOTS = 9  # evt_getObjectChrIndex: scriptIdx + 9 - scriptCount < 9 is a party character
# ovl_0a .data g_fieldModelCache (0x800C9CCC): 10 slots {u16 flags, u16 id, u32 addr, u32 size}, ids fixed in the file
OVL0A_VRAM, OVL0A_MODEL_CACHE, MODEL_CACHE_SLOTS = 0x800A7000, 0x800C9CCC, 10
# fld_remapModelId: model 185 is loaded as 76 once g_evtGlobalVars.h[0] (story progress) >= 0x283C
REMAP = {185: (76, 0x283C)}
# party character index (GameData.characters[i]) -> who holds it, from GameData.characters[i].name at run time
# (docs/field-models.md "Party models"): slots 5-7 hold Cinna / Marcus / Blank early on (Redux state field_dali_inn,
# disc 1) and Quina / Eiko / Amarant later (disc3_pandemonium_lv99_field)
CHAR_NAMES = ['Zidane', 'Vivi', 'Garnet', 'Steiner', 'Freya', 'Quina/Cinna', 'Eiko/Marcus', 'Amarant/Blank',
              'Beatrix']
GROUP_SIZE, BONE_MATRIX, GROUP_OUT = 0x28, 0x20, 0xC
VERTEX_BUFFER = 0x321


def dep_entries(data):
    for k, off in db_items(data):
        if k >> 16 == KIND_DEPS:
            return [struct.unpack_from('<4I', data, off + 4 + 16 * j) for j in range(u16(data, off + 2))]
    return []


def item_extents(data, base):
    """{key: (offset, end)} of a container's items; an item ends where the next one (in file order) starts."""
    items = db_items(data, base)
    offs = sorted(o for _, o in items)
    nxt = {o: (offs[i + 1] if i + 1 < len(offs) else len(data)) for i, o in enumerate(offs)}
    return {k: (o, nxt[o]) for k, o in items}


def model_info(data, base):
    """Model info item 0x120000 of a container: {model id: entry} (WmAnimTableEntry: u16 model, u16 default motion,
    u32 offset:24 | texture count:8, u16 texture animation id (0xFFFF none)); the offset (from the entry) leads to
    texture count {tpage, clut}, then {du, dv}, then {srcX, srcY, destX, destY} entries (fld_getModelTables)."""
    o = db_find(data, KIND_INFO << 16, base)
    if o is None:
        return {}
    out = {}
    for i in range(data[o + 1]):
        e = o + 4 + 12 * i
        mid, anim, w, ta = struct.unpack_from('<HHIH', data, e)
        out[mid] = {'anim': None if anim == 0xFFFF else KIND_MOTION << 16 | anim, 'tables': e + (w & 0xFFFFFF), 'textures': w >> 24,
                    'texanim': None if ta == 0xFFFF else KIND_TEXANIM << 16 | ta}
    return out


def model_record(data, base, mid, ext):
    """Header stats plus what the field code needs from the same container (fld_setupActorModel)."""
    key = KIND_MODEL << 16 | mid
    h, end = ext[key]
    info = model_info(data, base).get(mid)
    st = header_stats(data, h, info['textures'] if info else None)
    psum = sum(m['packet_size'] for m in st['meshes'])
    if psum > 0xFFFF and psum & 0xFFFF == u16(data, h + 4):
        # the header's u16 total wraps; the field code sums the groups' own sizes (fld_setupActorModel)
        st['problems'] = [p for p in st['problems'] if not p.startswith('header packet size')]
        st.setdefault('notes', []).append(f'header packet size wraps (groups sum to {psum:#x})')
    nb = data[h + 2]
    bones_ofs = struct.unpack_from('<i', data, h + 0xC)[0]
    parents = [data[h + bones_ofs + 4 * i + 3] for i in range(nb)]
    st['look_at_bone'] = data[h + 6]
    st['parents'] = parents
    if any(p >= i for i, p in enumerate(parents) if i):
        st['problems'].append('a bone has parent >= itself (fldmdl_calcBoneMatrices builds bones in order)')
    if len(st['meshes']) > 32:
        st['problems'].append(f'{len(st["meshes"])} groups > 32 (ModelObj.hiddenGroups bits)')
    if info is None:
        st['problems'].append('no model info entry (0x120000) in its container')
    else:
        if info['anim'] is not None and info['anim'] not in ext:
            st.setdefault('notes', []).append(f"default motion 0x{info['anim']:06x} not in the model's own container")
        if info['texanim'] is not None and info['texanim'] not in ext:
            st['problems'].append(f"texture animation 0x{info['texanim']:06x} missing")
    if CHECK_MOTIONS and info and info['anim'] in ext:
        check_motion(data, base, mid, info['anim'], st)
    st['info'] = info and {k: v for k, v in info.items() if k != 'tables'}
    st['packet_bytes'] = sum(m['packet_size'] for m in st['meshes'])
    # fld_setupActorModel: 2 x packets (double buffer) + 32 per bone matrix + 12 per group (WmModelOut)
    st['ram_per_actor'] = 2 * st['packet_bytes'] + BONE_MATRIX * nb + GROUP_OUT * len(st['meshes'])
    # the model's own bytes: up to the last group's last used colour (or its uvs); the item itself may run on
    # with padding that differs between copies
    last = h + struct.unpack_from('<I', data, h + 0x10)[0] + (len(st['meshes']) - 1) * GROUP_SIZE
    uv_o, c_o = (h + x for x in struct.unpack_from('<2I', data, last + 0x20))
    g = st['meshes'][-1] if st['meshes'] else None
    mend = c_o + 4 * (g['max_colour_index'] + 1) if g and g['max_colour_index'] >= 0 else c_o
    if mend > end:
        st['problems'].append(f'model data ends at +{mend - h:#x}, past its item (+{end - h:#x})')
    st['hash'] = hashlib.sha1(data[h:min(mend, end)]).hexdigest()[:12]
    st['item_bytes'] = end - h
    return st


CHECK_MOTIONS = False


def check_motion(data, base, mid, key, st):
    """--check: the default motion decodes for every frame like a battle motion (battle_pose): enough keys for the
    bones, no saturated matrix entry."""
    import battle_pose
    if (mid, key) in _checked:
        st['problems'] += _checked[(mid, key)]
        return
    n0 = len(st['problems'])
    m = battle_pose.Model(data, mid, pack=base)
    mo = m.motion(key)
    if u16(mo, 0) != 0 or u16(mo, 0xA) & ~7:
        st['problems'].append(f'motion 0x{key:06x}: header words {u16(mo, 0):#x} / constMask {u16(mo, 0xA):#x}')
    rk, lo = struct.unpack_from('<2i', mo, 0xC)
    if lo and lo > rk and (lo - rk) // 8 < len(m.bones):
        st['problems'].append(f'motion 0x{key:06x}: {(lo - rk) // 8} keys for {len(m.bones)} bones')
    for fr in range(m.frame_count(key)):
        if any(abs(x) >= 0x7FFF for R, _ in m.bone_matrices(key, fr) for row in R for x in row):
            st['problems'].append(f'motion 0x{key:06x} frame {fr}: saturated matrix entry')
            break
    _checked[(mid, key)] = st['problems'][n0:]


_checked = {}


def model_cache_ids(img):
    ovl = img.file(DIR_OVERLAYS, 0x0A)
    o = OVL0A_MODEL_CACHE - OVL0A_VRAM
    return [u16(ovl, o + 12 * i + 2) for i in range(MODEL_CACHE_SLOTS)]


def scan_disc(path, disc, worlds=False):
    """{'fields': [...], 'models': {(mid, source): record}, 'problems': [...]}; source = 'dir10' or 'local'."""
    img = Img(path)
    problems = []
    dir10 = {fid & 0xFFFF: (fid, sec, n) for fid, sec, n in img.files(DIR_CHAR_MODELS)}
    by_sector = {sec: (fid, n) for fid, sec, n in img.files(DIR_CHAR_MODELS)}
    dir10_cache = {}

    def dir10_model(mid):
        if mid not in dir10_cache:
            fid, sec, n = dir10[mid]
            data = img.read(sec * SECTOR, n * SECTOR)
            pk = db_find(data, KIND_PACK << 16)
            tex = db_find(data, KIND_PACK << 16 | 1)
            rec = model_record(data, pk, mid, item_extents(data, pk))
            rec.update({'file': f'10/{fid:#x}', 'sector': sec, 'sectors': n, 'model_sectors': fid >> 16})
            if tex is None or tex // SECTOR != fid >> 16 or tex % SECTOR:
                rec['problems'].append(f'textures (0x1B0001) not at sector {fid >> 16} (file id high half)')
            dir10_cache[mid] = (rec, data, pk)
        return dir10_cache[mid]

    models, fields = {}, []
    places = [(DIR_FIELDS, f) for f in img.files(DIR_FIELDS)]
    if worlds:
        places += [(DIR_WORLDS, f) for f in img.files(DIR_WORLDS) if f[2]]
    for dirx, (fid, sec, n) in places:
        data = img.read(sec * SECTOR, n * SECTOR)
        deps = dep_entries(data)
        c1 = db_find(data, KIND_PACK << 16 | 1)
        if c1 is None:
            continue
        ext = item_extents(data, c1)
        local = {k & 0xFFFF for k in ext if k >> 16 == KIND_MODEL}
        sc = scripts(data)
        nent = data[sc[0][1] + 3] if sc else 0
        place = {'disc': disc, 'kind': 'field' if dirx == DIR_FIELDS else 'world', 'id': fid & 0xFFFF,
                 'area': fid >> 16, 'script_entries': nent, 'models': []}
        where = f"disc {disc} {place['kind']} {place['id']}"
        seen = set()
        for key, msec, nsec, link in deps:
            if key >> 16 != KIND_MODEL:
                continue
            mid = key & 0xFFFF
            seen.add(mid)
            slot = link - (nent - PARTY_SLOTS)
            ent = {'model': mid, 'actor_entry': link, 'party_slot': slot if 0 <= slot < PARTY_SLOTS else None}
            if msec:
                if by_sector.get(msec, (None,))[0] is None or by_sector[msec][0] & 0xFFFF != mid:
                    problems.append(f'{where}: model {mid} at sector {msec} is not dir 10 file *{mid:04x}')
                    continue
                if nsec != by_sector[msec][1]:
                    problems.append(f'{where}: model {mid}: {nsec} sectors, file has {by_sector[msec][1]}')
                if mid in local:
                    problems.append(f'{where}: model {mid} is both in dir 10 and in the field file')
                ent['source'] = 'dir10'
                rec = dir10_model(mid)[0]
                if mid in REMAP:
                    alt, story = REMAP[mid]
                    ent['remap'] = {'model': alt, 'story_at_least': story}
                    if alt in dir10:
                        dir10_model(alt)
                    else:
                        problems.append(f'{where}: remapped model {alt} not in dir 10')
            else:
                if nsec:
                    problems.append(f'{where}: model {mid}: sector 0 but {nsec} sectors')
                if mid not in local:
                    problems.append(f'{where}: model {mid} (sector 0) is not in the field file')
                    continue
                ent['source'] = 'local'
                rec = model_record(data, c1, mid, ext)
            if not (0 < link < nent):
                problems.append(f'{where}: model {mid} linked to entry {link} of {nent}')
            prev = models.get((mid, ent['source']))
            if prev is None:
                rec = dict(rec, model=mid, source=ent['source'], places=[])
                models[(mid, ent['source'])] = rec
                prev = rec
            elif prev['hash'] != rec['hash']:
                problems.append(f"{where}: model {mid} differs from its copy in {prev['places'][0]}")
            prev['places'].append(f"{disc}:{place['kind'][0]}{place['id']}")
            place['models'].append(ent)
        for mid in sorted(local - seen):
            problems.append(f'{where}: model {mid} is in the field file but not in its dependency table')
        fields.append(place)
    for (mid, src), r in models.items():
        problems += [f'disc {disc} model {mid} ({src}): {p}' for p in r['problems']]
    for mid, (rec, _, _) in dir10_cache.items():
        if (mid, 'dir10') not in models:
            models[(mid, 'dir10')] = dict(rec, model=mid, source='dir10', places=[])
    cache = model_cache_ids(img)
    party = defaultdict(set)
    for f in fields:
        for e in f['models']:
            if e['party_slot'] is not None:
                party[e['model']].add(e['party_slot'])
                if 'remap' in e:
                    party[e['remap']['model']].add(e['party_slot'])
    for mid in cache:
        if mid not in dir10:
            problems.append(f'disc {disc}: cached model {mid} (g_fieldModelCache) is not in dir 10')
        else:
            dir10_model(mid)
            if (mid, 'dir10') not in models:
                models[(mid, 'dir10')] = dict(dir10_cache[mid][0], model=mid, source='dir10', places=[])
    for mid in party:
        if (mid, 'dir10') in models and mid not in cache:
            problems.append(f'disc {disc}: dir-10 party model {mid} is not in g_fieldModelCache')
    return {'fields': fields, 'models': models, 'problems': problems, 'party': party, 'cache': cache}


def load_discs(discs, img, worlds=False):
    out = {}
    for d in discs:
        p = find_image(d, img)
        if p is not None:
            out[d] = scan_disc(p, d, worlds)
    return out


def merged_models(per_disc):
    """One record per (model id, source) over all discs, with the discs and places that load it."""
    out = {}
    for d, s in per_disc.items():
        for k, r in s['models'].items():
            m = out.setdefault(k, dict(r, discs=[], places=[], party_slots=set()))
            m['discs'].append(d)
            m['places'] = m['places'] + r['places'] if m['discs'] != [d] else list(r['places'])
            m['party_slots'] |= s['party'].get(k[0], set())
            if m['hash'] != r['hash']:
                m.setdefault('problems', []).append(f'disc {d}: differs from disc {m["discs"][0]}')
    # the same model data under other ids (e.g. local copies of a party model, used by a non-party actor)
    by_hash = defaultdict(list)
    for k, m in out.items():
        by_hash[m['hash']].append(k[0])
    for k, m in out.items():
        m['same_data_as'] = sorted(x for x in by_hash[m['hash']] if x != k[0])
    party_ids = {k[0]: m['party_slots'] for k, m in out.items() if m['party_slots']}
    for k, m in out.items():
        m['party_copy_of'] = sorted(x for x in m['same_data_as'] if x in party_ids)
        m['party'] = bool(m['party_slots'] or m['party_copy_of'] or m['source'] == 'dir10')
    return out


def fmt_counts(st):
    return ' '.join(f"{st['polys'][t[0]]:4}" for t in PRIM_TYPES)


def party_label(m):
    if m['party_slots']:
        return '+'.join(CHAR_NAMES[s] for s in sorted(m['party_slots']))
    if m['party_copy_of']:
        return '= ' + ' '.join(map(str, m['party_copy_of']))
    return 'cached' if m['party'] else ''


def field_totals(place, models):
    vs = ps = pk = 0
    for e in place['models']:
        st = models[(e['model'], e['source'])]
        vs += st['vertices']
        ps += st['poly_total']
        pk += st['ram_per_actor']
    return vs, ps, pk


def pose_model(per_disc, mid, field, disc, anim, frame):
    """battle_pose.Model for a field model, with the motions of its own container and of the field's container."""
    import battle_pose
    for d, s in sorted(per_disc.items()):
        if disc and d != disc:
            continue
        img = Img(find_image(d, None))
        fdata = fpack = None
        if field is not None:
            for fid, sec, n in img.files(DIR_FIELDS):
                if fid & 0xFFFF == field:
                    fdata = img.read(sec * SECTOR, n * SECTOR)
                    fpack = db_find(fdata, KIND_PACK << 16 | 1)
            if fdata is None:
                continue
        if (mid, 'dir10') in s['models']:
            for fid, sec, n in img.files(DIR_CHAR_MODELS):
                if fid & 0xFFFF == mid:
                    data = img.read(sec * SECTOR, n * SECTOR)
            pk = db_find(data, KIND_PACK << 16)
            srcs = [(fdata, fpack)] if fdata is not None else []
        elif (mid, 'local') in s['models']:
            if fdata is None or db_find(fdata, KIND_MODEL << 16 | mid, fpack) is None:
                place = s['models'][(mid, 'local')]['places'][0]
                fid_ = int(place.split(':')[1][1:])
                for fid, sec, n in img.files(DIR_FIELDS):
                    if fid & 0xFFFF == fid_:
                        fdata = img.read(sec * SECTOR, n * SECTOR)
                        fpack = db_find(fdata, KIND_PACK << 16 | 1)
            data, pk, srcs = fdata, fpack, []
        else:
            continue
        m = battle_pose.Model(data, mid, pack=pk, motion_sources=srcs)
        info = model_info(data, pk).get(mid)
        key = anim if anim is not None else info['anim']
        if key is None:
            sys.exit(f'model {mid} has no default motion (model info 0xFFFF: drawn unposed, ModelObj.anim NULL); '
                     'pass --anim')
        return d, m, key
    sys.exit(f'model {mid} not found' + (f' with field {field}' if field is not None else ''))


def main():
    ap = argparse.ArgumentParser(description=__doc__, formatter_class=argparse.RawDescriptionHelpFormatter)
    ap.add_argument('--disc', type=int, action='append', choices=range(1, 5))
    ap.add_argument('--img', help='FF9.IMG (with a single --disc) or a directory holding discN/FF9.IMG '
                    '(default: $FF9_DISKS, else disks/ here or in a directory above)')
    ap.add_argument('--worlds', action='store_true', help='also the world-map files (dir index 3), same format')
    ap.add_argument('--model', type=int, action='append', help='only these model ids')
    ap.add_argument('--party', action='store_true', help='only models a party slot loads')
    ap.add_argument('--field', type=int, help='one field: its models, actor entries and party slots')
    ap.add_argument('--fields', action='store_true', help='one row per (disc, field), heaviest first')
    ap.add_argument('--by-non-party', action='store_true', help='--fields: sort by the non-party models\' vertices')
    ap.add_argument('--meshes', action='store_true', help='per mesh group detail')
    ap.add_argument('--top', type=int, help='only the N heaviest rows')
    ap.add_argument('--check', action='store_true', help='validation summary; exit status 1 on any problem')
    ap.add_argument('--json', help='write everything to this file ("-" = stdout)')
    ap.add_argument('--pose', type=int, metavar='MODEL', help='pose a model (needs --field for party models\' '
                    'field motions; default motion = its model info entry\'s)')
    ap.add_argument('--anim', type=lambda s: int(s, 0), help='motion key 0x03xxxx for --pose')
    ap.add_argument('--frame', type=int, default=0)
    ap.add_argument('--bones', action='store_true', help='--pose: print the bone matrices')
    ap.add_argument('--obj', help='--pose: write a Wavefront OBJ')
    a = ap.parse_args()
    discs = a.disc or [1, 2, 3, 4]
    global CHECK_MOTIONS
    CHECK_MOTIONS = a.check
    per_disc = load_discs(discs, a.img, a.worlds)
    if a.pose is not None:
        import battle_pose
        d, m, key = pose_model(per_disc, a.pose, a.field, a.disc[0] if a.disc else None, a.anim, a.frame)
        n = m.frame_count(key)
        if not 0 <= a.frame < n:
            sys.exit(f'motion 0x{key:06x} has {n} frames')
        bones = m.bone_matrices(key, a.frame)
        verts = m.vertices(bones)
        print(f'model {a.pose} (disc {d}): motion 0x{key:06x} frame {a.frame}/{n}, {len(bones)} bones, '
              f'{sum(map(len, verts))} vertices')
        if a.bones:
            for i, (R, T) in enumerate(bones):
                print(f'  bone {i:2} parent {m.bones[i][2]:2} len {m.bones[i][0]:5}: R {R} T {T}')
        allv = [v for g in verts for v in g]
        if allv:
            lo = [min(v[i] for v in allv) for i in range(3)]
            hi = [max(v[i] for v in allv) for i in range(3)]
            print(f'  bounds x {lo[0]}..{hi[0]}  y {lo[1]}..{hi[1]}  z {lo[2]}..{hi[2]}')
        if a.obj:
            battle_pose.write_obj(a.obj, m, verts, f'field model {a.pose} motion 0x{key:06x} frame {a.frame}')
            print(f'  wrote {a.obj}')
        return
    problems = [p for s in per_disc.values() for p in s['problems']]
    models = merged_models(per_disc)
    is_party = {k: m['party'] for k, m in models.items()}
    if a.json:
        keep = [r for k, r in sorted(models.items()) if (not a.model or k[0] in a.model) and
                (not a.party or r['party'])]
        out = {'discs': {d: {'fields': s['fields'], 'model_cache': s['cache'],
                             'party': {m: sorted(v) for m, v in s['party'].items()}}
                         for d, s in per_disc.items()},
               'models': [dict(r, party_slots=sorted(r['party_slots'])) for r in keep],
               'char_names': CHAR_NAMES, 'problems': problems}
        text = json.dumps(out, indent=1, default=str)
        if a.json == '-':
            print(text)
        else:
            open(a.json, 'w').write(text + '\n')
        return
    if a.check:
        nf = sum(len(s['fields']) for s in per_disc.values())
        ne = sum(len(f['models']) for s in per_disc.values() for f in s['fields'])
        for p in problems:
            print('problem:', p)
        print(f"discs {''.join(map(str, per_disc))}: {nf} places, {ne} model entries, {len(models)} distinct "
              f"(model, source) pairs ({sum(1 for k in models if k[1] == 'dir10')} in dir 10), "
              f"{len(problems)} problems")
        sys.exit(1 if problems else 0)
    for p in problems:
        print('problem:', p, file=sys.stderr)
    types = ' '.join(f'{t[0]:>4}' for t in PRIM_TYPES)
    if a.field is not None:
        for d, s in per_disc.items():
            for f in s['fields']:
                if f['id'] != a.field:
                    continue
                vs, ps, pk = field_totals(f, s['models'])
                print(f"disc {d} {f['kind']} {f['id']} (area {f['area']}, {f['script_entries']} script entries): "
                      f"{len(f['models'])} models, {vs} vertices, {ps} polygons, {pk} bytes of model RAM (fld_setupActorModel: packets x2 + bone matrices + group records)")
                print(f"  model source entry party          bones grp  verts polys  {types}  packets")
                for e in sorted(f['models'], key=lambda e: -s['models'][(e['model'], e['source'])]['vertices']):
                    st = s['models'][(e['model'], e['source'])]
                    slot = (CHAR_NAMES[e['party_slot']] if e['party_slot'] is not None else
                            party_label(models[(e['model'], e['source'])]))
                    rm = f"  (as {e['remap']['model']} from story {e['remap']['story_at_least']:#x})" if 'remap' in e else ''
                    print(f"  {e['model']:5} {e['source']:6} {e['actor_entry']:5} {slot:14} {st['bones']:5}"
                          f" {len(st['meshes']):3} {st['vertices']:6} {st['poly_total']:5}  {fmt_counts(st)}"
                          f"  {st['packet_bytes']:6}{rm}")
        return
    if a.fields:
        rows = []
        for d, s in per_disc.items():
            for f in s['fields']:
                vs, ps, pk = field_totals(f, s['models'])
                npar = sum(1 for e in f['models'] if is_party[(e['model'], e['source'])])
                nonp = [s['models'][(e['model'], e['source'])]['vertices'] for e in f['models']
                        if not is_party[(e['model'], e['source'])]]
                rows.append((vs, d, f, ps, pk, npar, sum(nonp)))
        rows.sort(key=lambda r: -r[6] if a.by_non_party else -r[0])
        print('disc field  area models party  verts polys  non-party verts  ram (packets x2 + bones)')
        for vs, d, f, ps, pk, npar, nv in rows[:a.top] if a.top else rows:
            print(f"{d:4} {f['kind'][0]}{f['id']:<5} {f['area']:4} {len(f['models']):6} {npar:5} {vs:6} {ps:5}"
                  f"  {nv:15}  {pk}")
        return
    rows = sorted(models.values(), key=lambda r: -r['vertices'])
    if a.model:
        rows = [r for r in rows if r['model'] in a.model]
    if a.party:
        rows = [r for r in rows if r['party']]
    if a.top:
        rows = rows[:a.top]
    print(f"model source  bones grp  verts polys  {types}  packets  party            places")
    for r in rows:
        pl = r['places']
        fl = ' '.join(pl[:6]) + (f' +{len(pl) - 6}' if len(pl) > 6 else '')
        print(f"{r['model']:5} {r['source']:6} {r['bones']:6} {len(r['meshes']):3} {r['vertices']:6} "
              f"{r['poly_total']:5}  {fmt_counts(r)}  {r['packet_bytes']:7}  {party_label(r):15}"
              f"  {len(pl):3}: {fl}")
        if a.meshes:
            for i, g in enumerate(r['meshes']):
                bones = ' '.join(f'{b}:{n}' for b, n in g['bone_vertices'].items())
                print(f"      group {i}: {g['vertices']} vertices, {g['uvs']} uvs, ot bias {g['ot_bias']}, "
                      f"textures {g['textures']}, {g['double_sided']} double-sided, {g['semi_transparent']} "
                      f"semi-transparent, polys " + ' '.join(f"{k} {v}" for k, v in g['polys'].items() if v)
                      + f"; bone:vertices {bones}")


if __name__ == '__main__':
    main()
