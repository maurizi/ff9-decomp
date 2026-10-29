#!/usr/bin/env python3
"""Pose a battle model: its bone matrices and vertex positions for one motion frame, exactly as the PS1 does.

    python3 tools/battle_pose.py --model 96                        # idle (motionIds[0]) frame 0, summary
    python3 tools/battle_pose.py --model 96 --motion 2 --frame 5 --bones
    python3 tools/battle_pose.py --model 96 --key 0x030c43 --frame 0 --obj build/malboro.obj
    python3 tools/battle_pose.py --model 96 --list                 # the model file's motions, the enemy's six
    python3 tools/battle_pose.py --check-all                       # every enemy model's idle frame 0 (sanity scan)

Module use (the DS port's tools/simplify_battle_models.py imports it):

    import battle_pose
    meshes, bones = battle_pose.pose(model_id, motion=0, frame=0, disc=None, img=None)
    # meshes[g][v] = (x, y, z): vertex v of mesh group g in model space (ints, bone matrix applied as the GTE does)
    # bones[b] = (R, T): R 3x3 ints (4096 = 1.0, R[row][col]), T (x, y, z) ints, bone b's model-space matrix

- `motion` is an index into the enemy type's six motionIds (0 = idle; see docs/battle-models.md "Idle stance"),
  found from the first encounter on the disc whose enemy type uses this model; or pass `key=0x03xxxx` (any motion
  in the model file, e.g. a party member's motion) instead.
- `base=(R, T)` replaces the model matrix (BtlUnit9C.matrix; default identity, T 0: model space). With the unit's
  own matrix the result is what btl_calcBoneMatrices leaves in BtlUnit9C.bones (world space).
- Lower level: `load_model(model_id, disc, img)` -> Model (bones, meshes, motions), `Model.bone_matrices(key,
  frame, base)`, `Model.vertices(bones)`, `decode_frame(motion bytes, boneCount, frame)` -> root translation and
  per-bone SVECTOR angles, `rot_matrix_zyx(vx, vy, vz)` (math_rotMatrixZYX_gte bit-exact).

Arithmetic follows src_equiv/ovl_0b/bones.c (btl_calcBoneMatrices) and main math_buildRotMatrixZYX (mode 1, GPF
products) with the GTE's rounding: MVMVA/GPF with sf=1 shift the 44-bit sum right by 12 (floor), IR results
saturate to s16 (lm=0), matrix entries are stored as s16. g_sinTable (main 0x8006C1C4) is exactly
round(4096 * sin(i * pi / 2048)), i = 0..1024, so it is computed here. Format: docs/battle-models.md "Motions".
Vertex positions: MAC of MVMVA(bone R, vertex, bone T) = T + floor(R . v / 4096), the value
btlfx_projectSliceMesh reads (btl_projectMesh folds the camera in first, so the game never stores this one).
"""
import argparse
import math
import os
import struct
import sys

sys.path.insert(0, os.path.dirname(os.path.abspath(__file__)))
from encounter_catalog import Img, SECTOR, db_items, db_find, find_image, u16  # noqa: E402
from battle_models import DIR_ENCOUNTERS, DIR_MODELS, KIND_MODEL, KIND_PACK, encounter  # noqa: E402

KIND_MOTION = 0x03
DIR_PARTY_MODELS = 10  # btl_loadPartyModelsStep
SIN = [round(4096 * math.sin(i * math.pi / 2048)) for i in range(0x401)]  # = g_sinTable
IDENTITY = (((4096, 0, 0), (0, 4096, 0), (0, 0, 4096)), (0, 0, 0))


def s8(v):
    v &= 0xFF
    return v - 0x100 if v & 0x80 else v


def s16(v):
    v &= 0xFFFF
    return v - 0x10000 if v & 0x8000 else v


def s32(v):
    v &= 0xFFFFFFFF
    return v - 0x100000000 if v & 0x80000000 else v


def sat16(v):
    """IR1-3 saturation (lm=0)."""
    return -0x8000 if v < -0x8000 else 0x7FFF if v > 0x7FFF else v


def isin(a):
    """ISIN (ff9.h): sine of a 12-bit angle (4096 = 360 degrees) from g_sinTable."""
    a &= 0xFFF
    v = SIN[0x400 - (a & 0x3FF)] if a & 0x400 else SIN[a & 0x3FF]
    return -v if a & 0x800 else v


def icos(a):
    return isin(0x400 - (a & 0xFFF))


def rot_matrix_zyx(vx, vy, vz):
    """math_rotMatrixZYX_gte (math_buildRotMatrixZYX mode 1): R = Rz(vz) . Ry(vy) . Rx(vx), with the GPF products
    in the order the routine issues them (each a >> 12 on the full product). Returns R[row][col], s16."""
    sx, cx = isin(vx), icos(vx)
    sy, cy = isin(vy), icos(vy)
    sz, cz = isin(vz), icos(vz)

    def gpf(ir0, *ir):  # GPF sf=1: MAC_i = IR0 * IR_i >> 12 (IR registers hold s16)
        return [s16(ir0) * s16(x) >> 12 for x in ir]
    d, e, f = gpf(cy, cz, sz, sx)
    m00, m10, m21 = d, e, f
    m20 = -sy
    a = (cz * sx) >> 12
    b = (sz * sx) >> 12
    d, c, t = gpf(cx, cy, sz, cz)
    m22 = d
    d, e, f = gpf(sy, a, b, t)
    m01 = d - c
    m11 = e + t
    d, e = gpf(sy, t, c)
    m02 = d + b
    m12 = e - a
    return tuple(tuple(s16(x) for x in row) for row in ((m00, m01, m02), (m10, m11, m12), (m20, m21, m22)))


def decode_frame(anim, nbones, frame):
    """One frame of a motion (bytes of the 0x03xxxx item): ((root x, y, z), [(vx, vy, vz)] per bone).
    Angles are the SVECTOR halfwords btl_calcBoneMatrices hands math_rotMatrixZYX_gte (only the low 12 bits count)."""
    rx, ry, rz, cmask = struct.unpack_from('<4H', anim, 4)
    keys, lo = struct.unpack_from('<2i', anim, 0xC)

    def root(v, bit):
        return s16(v) if cmask & bit else struct.unpack_from('<h', anim, v + (frame & 0xFFFF) * 2)[0]
    rootT = (root(rx, 1), root(ry, 2), root(rz, 4))
    half, odd = frame >> 1, frame & 1
    angles = []
    for i in range(max(nbones, 1)):  # do-while: a 0-bone model still decodes one key
        k = struct.unpack_from('<4H', anim, keys + 8 * i)
        hi = [k[j] if k[3] >> j & 1 else s8(anim[k[j] + frame]) for j in range(3)]
        nib = [0, 0, 0]
        if lo:
            n = struct.unpack_from('<4H', anim, lo + 8 * i)
            for j in range(3):
                v = n[j] if n[3] >> j & 1 else (s8(anim[n[j] + half]) & 0xFFFFFFFF) >> (4 if odd else 0)
                nib[j] = v & 0xF
        angles.append(tuple(s16(((h << 4) & 0xFFFF) | nb) for h, nb in zip(hi, nib)))
    return rootT, angles


def compose_col(P, L):
    """MVMVA sf=1 RT=P on each column of L, IR saturated: (P . L) >> 12."""
    return tuple(tuple(sat16(sum(P[r][k] * L[k][c] for k in range(3)) >> 12) for c in range(3)) for r in range(3))


def mvmva_tr(R, T, v):
    """MVMVA sf=1 RT, V0, TR: MAC_i = (T_i << 12 + R_i . v) >> 12 (32-bit MAC)."""
    return tuple(s32(((T[r] << 12) + sum(R[r][k] * v[k] for k in range(3))) >> 12) for r in range(3))


def bone_matrices(anim, bonedefs, frame, base=None):
    """btl_calcBoneMatrices for a model without a parent model: [(R, T)] per bone, `base` = BtlUnit9C.matrix
    (default identity). bonedefs: [(length, unk2, parent)] per bone."""
    n = len(bonedefs)
    rootT, angles = decode_frame(anim, n, frame)
    local = [rot_matrix_zyx(*a) for a in angles[:n]]
    bR, bT = base or IDENTITY
    out = [None] * n
    out[0] = (compose_col(bR, local[0]), mvmva_tr(bR, bT, tuple(s16(x) for x in rootT)))
    garbage = (0, 0, 0)  # T of a bone not built yet (scratchpad leftovers on the PS1); never used on the discs
    for i in range(1, n):
        length, _, p = bonedefs[i]
        PR, PT = out[p] if out[p] is not None else (local[p], garbage)
        out[i] = (compose_col(PR, local[i]), mvmva_tr(PR, PT, (0, 0, length)))
    return out


class Model:
    """A battle model file (FF9.IMG dir index 7, or any file whose block 0x1B item 0 is a model container)."""

    def __init__(self, data, model_id):
        self.data, self.id = data, model_id
        pack = db_find(data, KIND_PACK << 16)
        if pack is None:
            raise ValueError(f'model {model_id}: no block 0x1B')
        self.motions = {}
        h = None
        for k, off in db_items(data, pack):
            if k == KIND_MODEL << 16 | model_id:
                h = off
            elif k >> 16 == KIND_MOTION:
                self.motions[k] = off
        if h is None:
            raise ValueError(f'model {model_id}: no header 0x{KIND_MODEL << 16 | model_id:06x}')
        self.header = h
        _, nbones, nmesh = struct.unpack_from('<HBB', data, h)
        bones_ofs, mesh_ofs = struct.unpack_from('<iI', data, h + 0xC)
        self.bones = [struct.unpack_from('<hBB', data, h + bones_ofs + 4 * i) for i in range(nbones)]
        self.meshes = []  # per group: [(bone, (x, y, z))]
        for g in range(nmesh):
            m = h + mesh_ofs + g * 0x28
            vc_o, v_o = (h + x for x in struct.unpack_from('<2I', data, m + 0x14))
            counts = struct.unpack_from(f'<{nbones}H', data, vc_o)
            verts = []
            for b, c in enumerate(counts):
                for _ in range(c):
                    verts.append((b, struct.unpack_from('<3h', data, v_o)))
                    v_o += 8
            self.meshes.append(verts)

    def motion(self, key):
        """Bytes of motion item `key` (0x03xxxx) from its start (offsets inside are relative to it)."""
        if key not in self.motions:
            raise KeyError(f'model {self.id}: no motion 0x{key:06x}')
        return memoryview(self.data)[self.motions[key]:]

    def frame_count(self, key):
        return u16(self.motion(key), 2)

    def bone_matrices(self, key, frame, base=None):
        return bone_matrices(self.motion(key), self.bones, frame, base)

    def vertices(self, bones):
        """Every vertex in model space: [[(x, y, z)] per group]."""
        return [[mvmva_tr(*bones[b], v) for b, v in g] for g in self.meshes]


def _image(disc, img):
    for d in ([disc] if disc else [1, 2, 3, 4]):
        p = find_image(d, img)
        if p is not None:
            return d, Img(p)
    raise FileNotFoundError('no FF9.IMG (pass img= / --img, or set $FF9_DISKS)')


_cache = {}


def load_model(model_id, disc=None, img=None):
    """Model `model_id` from FF9.IMG dir index 7 (enemies), else dir index 10 (party members: the file id's low 16
    bits are the model id); the first disc found when disc is None."""
    key = ('model', model_id, disc, img)
    if key not in _cache:
        d, im = _image(disc, img)
        data = im.file(DIR_MODELS, model_id)
        if data is None:
            for fid, sec, n in im.files(DIR_PARTY_MODELS):
                if fid & 0xFFFF == model_id:
                    data = im.read(sec * SECTOR, n * SECTOR)
                    break
        if data is None:
            raise KeyError(f'disc {d}: no model file 7/{model_id} or 10/*{model_id:04x}')
        _cache[key] = Model(data, model_id)
    return _cache[key]


def enemy_types(disc=None, img=None):
    """{model id: (battle, type index, name, motionIds[6])} from the first encounter using each model."""
    key = ('types', disc, img)
    if key not in _cache:
        _, im = _image(disc, img)
        out = {}
        for battle, sec, n in im.files(DIR_ENCOUNTERS):
            data = im.read(sec * SECTOR, n * SECTOR)
            if data[:1] != b'\xdb':
                continue
            enc = encounter(data)
            if enc is None:
                continue
            for t, ty in enumerate(enc[1]):
                out.setdefault(ty['model'], (battle, t, ty['name'], ty['motions']))
        _cache[key] = out
    return _cache[key]


def motion_key(model_id, motion=0, disc=None, img=None):
    """The motion key of the enemy type's motionIds[motion] (0 = idle)."""
    t = enemy_types(disc, img).get(model_id)
    if t is None:
        raise KeyError(f'model {model_id}: no encounter uses it as an enemy; pass key=0x03xxxx')
    return KIND_MOTION << 16 | t[3][motion]


def pose(model_id, motion=0, frame=0, disc=None, img=None, key=None, base=None):
    """(meshes, bones): meshes[g] = [(x, y, z)] per vertex of mesh group g, bones[b] = (R, T); see the module doc."""
    m = load_model(model_id, disc, img)
    if key is None:
        key = motion_key(model_id, motion, disc, img)
    bones = m.bone_matrices(key, frame, base)
    return m.vertices(bones), bones


def write_obj(path, model, verts, name='model'):
    """Wavefront OBJ: one object per mesh group, FT4 / FT3 / ... polygons as faces (quads split as the GPU does)."""
    from battle_models import PRIM_TYPES
    d, h = model.data, model.header
    nbones, nmesh = d[h + 2], d[h + 3]
    mesh_ofs = struct.unpack_from('<I', d, h + 0x10)[0]
    with open(path, 'w') as f:
        f.write(f'# {name}: y negated (PS1 y points down)\n')
        base = 1
        for g in range(nmesh):
            m = h + mesh_ofs + g * 0x28
            counts = struct.unpack_from('<8H', d, m + 2)
            p = h + struct.unpack_from('<I', d, m + 0x1C)[0]
            f.write(f'o group{g}\n')
            for x, y, z in verts[g]:
                f.write(f'v {x} {-y} {z}\n')
            for (pname, size, nvert, _), n in zip(PRIM_TYPES, counts):
                for _ in range(n):
                    vi = struct.unpack_from(f'<{nvert}H', d, p)
                    tris = [(0, 1, 2)] if nvert == 3 else [(0, 1, 2), (1, 3, 2)]
                    for tri in tris:  # winding flipped with y so faces keep their side
                        f.write('f ' + ' '.join(str(base + vi[k]) for k in reversed(tri)) + '\n')
                    p += size
            base += len(verts[g])


def check_all(disc, img):
    """Idle frame 0 of every enemy model on the disc: bone order (parent < bone), saturation, key ranges."""
    types = enemy_types(disc, img)
    probs = 0
    for mid, (battle, t, name, mids) in sorted(types.items()):
        try:
            m = load_model(mid, disc, img)
        except (KeyError, ValueError) as e:
            print(f'model {mid}: {e}')
            continue
        bad = [i for i, (_, _, p) in enumerate(m.bones) if i and p >= i]
        for i, k in enumerate(mids):
            key = KIND_MOTION << 16 | k
            if key not in m.motions:
                if i == 0:
                    print(f'model {mid} ({name}): idle motion 0x{key:06x} missing')
                    probs += 1
                continue
            mo = m.motion(key)
            if u16(mo, 0) != 0:
                print(f'model {mid}: motion 0x{key:06x} +0 = {u16(mo, 0):#x}')
                probs += 1
            rk, lo = struct.unpack_from('<2i', mo, 0xC)
            if lo and lo > rk and (lo - rk) // 8 < len(m.bones):
                print(f'model {mid}: motion 0x{key:06x} has {(lo - rk) // 8} keys for {len(m.bones)} bones')
                probs += 1
            for fr in range(m.frame_count(key)):
                bones = m.bone_matrices(key, fr)
                if any(abs(x) >= 0x7FFF for R, _ in bones for row in R for x in row):
                    print(f'model {mid}: motion 0x{key:06x} frame {fr}: saturated matrix entry')
                    probs += 1
                    break
        if bad:
            print(f'model {mid} ({name}): bones with parent >= self: {bad}')
            probs += 1
    print(f'{len(types)} enemy models, {probs} problems')


def main():
    ap = argparse.ArgumentParser(description=__doc__, formatter_class=argparse.RawDescriptionHelpFormatter)
    ap.add_argument('--model', type=int)
    ap.add_argument('--disc', type=int, choices=range(1, 5))
    ap.add_argument('--img', help='FF9.IMG or a directory with discN/FF9.IMG (as battle_models.py)')
    ap.add_argument('--motion', type=int, default=0, help="index into the enemy type's motionIds (0 = idle)")
    ap.add_argument('--key', type=lambda s: int(s, 0), help='motion key 0x03xxxx (overrides --motion)')
    ap.add_argument('--frame', type=int, default=0)
    ap.add_argument('--bones', action='store_true', help='print the bone matrices')
    ap.add_argument('--vertices', action='store_true', help='print every vertex')
    ap.add_argument('--obj', help='write a Wavefront OBJ of the pose')
    ap.add_argument('--list', action='store_true', help="list the file's motions and the enemy's motionIds")
    ap.add_argument('--check-all', action='store_true')
    a = ap.parse_args()
    if a.check_all:
        return check_all(a.disc, a.img)
    if a.model is None:
        ap.error('--model is required')
    m = load_model(a.model, a.disc, a.img)
    t = enemy_types(a.disc, a.img).get(a.model)
    if a.list:
        if t:
            print(f'enemy type {t[2]!r} (battle {t[0]}, type {t[1]}): motionIds ' + ' '.join(f'{x:#x}' for x in t[3]))
        for k in sorted(m.motions):
            idx = [i for i, x in enumerate(t[3]) if KIND_MOTION << 16 | x == k] if t else []
            print(f'  0x{k:06x}: {m.frame_count(k):3} frames' + (f'  motionIds{idx}' if idx else ''))
        return
    key = a.key if a.key is not None else motion_key(a.model, a.motion, a.disc, a.img)
    n = m.frame_count(key)
    if not 0 <= a.frame < n:
        sys.exit(f'motion 0x{key:06x} has {n} frames')
    bones = m.bone_matrices(key, a.frame)
    verts = m.vertices(bones)
    print(f'model {a.model}' + (f' ({t[2]})' if t else '') + f': motion 0x{key:06x} frame {a.frame}/{n}, '
          f'{len(bones)} bones, {sum(map(len, verts))} vertices')
    if a.bones:
        for i, (R, T) in enumerate(bones):
            print(f'  bone {i:2} parent {m.bones[i][2]:2} len {m.bones[i][0]:5}: R {R} T {T}')
    if a.vertices:
        for g, vs in enumerate(verts):
            for i, v in enumerate(vs):
                print(f'  group {g} vertex {i} bone {m.meshes[g][i][0]}: {v}')
    allv = [v for g in verts for v in g]
    if allv:
        lo = [min(v[i] for v in allv) for i in range(3)]
        hi = [max(v[i] for v in allv) for i in range(3)]
        print(f'  bounds x {lo[0]}..{hi[0]}  y {lo[1]}..{hi[1]}  z {lo[2]}..{hi[2]}')
    if a.obj:
        write_obj(a.obj, m, verts, f'model {a.model} motion 0x{key:06x} frame {a.frame}')
        print(f'  wrote {a.obj}')


if __name__ == '__main__':
    main()
