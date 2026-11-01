#!/usr/bin/env python3
"""Field walkmeshes and exit regions, read offline from the disc images (FF9.IMG), as JSON.

    python3 tools/field_maps.py list [--disc N]               # field ids of a disc (default: every disc found)
    python3 tools/field_maps.py dump FIELD [--disc N]         # one field: walkmesh, regions, exits, ops (JSON)
    python3 tools/field_maps.py dump FIELD --no-mesh          # the same without the walkmesh arrays
    python3 tools/field_maps.py check [--disc N] [--json F]   # parse every field of every disc, print statistics

Disc images: --img DIR (or an FF9.IMG with one --disc), else $FF9_DISKS, else disks/ in this checkout or a directory
above it (encounter_catalog.find_image). Formats, code paths and statistics: docs/r32/3ds-requests-9.md.

Where things are: a field is FF9.IMG dir index 4, file id = area << 16 | field. The file is a DB container; its item
0x1B0001 (DB_PACK | 1) is the nested container fld_loadFile registers. In it, the field background is the first
item of kind 0x0A (fld_getBgKey), the walkmesh the first item of kind 0x0B (fld_setupBackground:
DB_GET_KEY_AT_IN(bg key, DB_WALKMESH)), the event script item 0x50000 | field (fld_findEventScript). The walkmesh item
is a u32 magic 0xACDCDEAD followed by the BgiHeader block that bgi_init uses in place: every table is an offset from
the header (no pointers, no relocation).

Script analysis: every function is disassembled with tools/evtscript/evtdis.py and given a control-flow graph
(JMP/JMP_IF/JMP_IFNOT/SWITCH targets as evt_runScripts computes them). Branches on a constant EXPR are folded and
instructions the function start cannot reach are ignored (dead GOTOs, polygons, creations). A guard of instruction
X is a live branch with one successor that reaches X and one that does not; its class comes from the variables the
EXPR right before the branch reads. Exits are the GOTOs reachable from a region's range function (tag 2) or action
functions (tags 8, 3), directly or through CALL/CALL_WAIT_FREE/CALL_SYNC (0x10/0x12/0x14) to depth 2.

JSON interface (stable; the 3DS port builds against these keys)
---------------------------------------------------------------
`list` -> [{disc, field, area, file_id, sector, sectors, name}]  (name = script debug name, script + 4)

`dump` -> {
  disc, field, area, file_id, sector, sectors, name,
  walkmesh: null | {
    item_key,                       # DB key 0x0Bxxxx of the walkmesh item
    header: {dataSize, org[3], cur[3], min[3], max[3], charPos[3], activeFloor, activeTriangle,
             triangleCount, edgeCount, animCount, floorCount, normalCount, vertexCount},
    vertices: [[x, y, z]],          # s16, relative: world = header.cur + floors[tri.floor].cur + vertex
    triangles: [{flags,             # u16 as stored; bit 0 BGI_FLAG_ACTIVE (walkable), high byte = mask byte
                 active,            # walkable right after bgi_init (anims: frame 0 on, other frames off)
                 mask,              # flags >> 8: 0x80 blocks the player, 0x40 blocks other actors,
                                    #   0x10 selects the second footstep sound
                 data, floor, normal, thetaX, thetaZ, vertices[3], edges[3], neighbors[3] (-1 none),
                 center[3], d,
                 world: [[x, y, z] x 3]}],   # vertices in field coordinates (stored cur positions)
    edges: [{flags, clone}],        # per triangle side (triangle.edges[i]); clone = side index in the neighbour
    floors: [{flags, index, org[3], cur[3], min[3], max[3], triangles: [ids]}],
    normals: [{nx, ny, nz, invNy}],  # 16.16 plane normals; triangle.normal -1 = flat (y from vertex 0)
    anims: [{flags, frameCount, frameRate, counter, curFrame,
             frames: [{flags, value, triangles: [ids]}]}]   # walkmesh keyframe anims (switch triangle sets)
  },
  regions: [{entry, uid,            # script entry and object uid (NEW_REGION 08 entry uid; uid 0 = entry)
             created_at: [[entry, tag, offset, story_guarded]],
             polygons: [{entry, tag, offset, op,         # 0x29 SET_REGION (or 0x76/0x77/0x78 sequence: op 'edit')
                         points: [[x, z]] | null,       # null when an argument is an expression
                         exprs: [...], story_guarded}],
             functions: [tags],
             exit_triggers: ['range' | 'action']}],   # which of its triggers lead to a GOTO (see exits)
  exits: [{entry, uid,
           trigger,                 # 'range': tag 2, runs while the player stands inside (walk-out exits);
                                    # 'action': tag 8 / 3, Square / Cross pressed inside (ladders, doors)
           polygon: [[x, z]] | null, polygon_count,
           destinations: [{kind: 'field' | 'world', id: int | null, expr: str | null,
                           arrival: int | null,          # literal Gs16[2] (entry point) set just before, if any
                           at: [entry, tag, offset], via: [[entry, tag, offset, call op]],
                           guards: [{at, class, expr}]}], # branches that decide whether this GOTO runs;
                                    # class: story (reads a global var), field (field var, e.g. the player's
                                    # facing Fs16[6]), local (object var), object (another object's property,
                                    # e.g. the player's position), system (sysvar only, e.g. control enabled),
                                    # constant / unknown (no EXPR right before the branch)
           conditional,             # a guard other than system/constant decides whether a GOTO runs
           story_dependent,         # a global-variable guard, or several destinations, or an expression destination
           created_story_guarded}], # the NEW_REGION (or the SET_REGION) itself sits behind a global-variable guard,
                                    # so the exit only exists in some story states
  other_gotos: [{kind, id, expr, at, owner_kind}],  # GOTO_FIELD/WORLD not reached from a region trigger
                                    # (cutscenes, main-loop code); owner_kind: actor, code, main, party, region, unused
  walkmesh_ops: [{op,                # hex string: '9A', 'CB', 'FF0A', ... (docs/r32/3ds-requests-9.md section 3)
                  name, args, at: [entry, tag, offset], literal, story_guarded}],
  npcs: [{entry, uid, menus: [[type, arg]],       # actors whose talk (tag 3/8) reaches MENU: type 2 shop, 4 save
          positions: [{op, pos, at}]}],           # SET_POS_XZ [x, z] / SET_POS [x, y, z] in the actor's tag 0
  problems: [str]                   # parse problems (none on the four NTSC-U discs)
}
Expression operands appear as infix strings ('{(Gu16[0x0] < 5990)}', tools/evtscript/edis.py notation) in args /
expr / exprs, and as null in points / pos. story_guarded = some guard of that instruction reads a global variable.
"""
import argparse
import json
import os
import struct
import sys
from collections import Counter, defaultdict

HERE = os.path.dirname(os.path.abspath(__file__))
sys.path.insert(0, HERE)
sys.path.insert(0, os.path.join(HERE, 'evtscript'))
from encounter_catalog import Img, SECTOR, db_items, find_image, script_functions  # noqa: E402
import evtdis  # noqa: E402
_raw_expr = evtdis.expr
import edis  # noqa: E402  (infix expression printer; importing it patches evtdis.expr, undone below)
evtdis.expr = _raw_expr

DIR_FIELDS = 4
KIND_FIELD, KIND_BG, KIND_WALKMESH, KIND_PACK = 0x05, 0x0A, 0x0B, 0x1B
BGI_MAGIC = 0xACDCDEAD
HDR_FMT = '<H15h2h12H'  # BgiHeader, 0x3C bytes
HDR_SIZE = struct.calcsize(HDR_FMT)

OP_NEW_CODE, OP_NEW_REGION, OP_NEW_ACTOR = 0x07, 0x08, 0x09
OP_SET_REGION, OP_REGION_START, OP_REGION_MOVE_LAST, OP_REGION_DUP_LAST = 0x29, 0x76, 0x77, 0x78
OP_GOTO_FIELD, OP_GOTO_WORLD = 0x2B, 0xB6
CALL_OPS = (0x10, 0x12, 0x14)
OP_MENU = 0x75
POS_OPS = {0x1D: 'SET_POS_XZ', 0xA1: 'SET_POS'}
FUNC_INIT, FUNC_RANGE, FUNC_TALK, FUNC_TALK2 = 0, 2, 3, 8
WALKMESH_OPS = {0x27: 'BG_MASK_FILTER', 0x9A: 'BGI_TRI_ACTIVE', 0xA8: 'BGI_ACTOR_ACTIVE', 0xCB: 'BGI_FLOOR_ACTIVE',
                0xFF00: 'BGI_FLOOR_MOVE_START', 0xFF01: 'BGI_FLOOR_MOVE_STOP', 0xFF02: 'BGI_FLOOR_MOVE_PLAYING',
                0xFF03: 'BGI_FLOOR_MOVE_MODE', 0xFF04: 'BGI_FLOOR_MOVE_FLOOR', 0xFF05: 'BGI_FLOOR_MOVE_SPEED',
                0xFF06: 'BGI_FLOOR_MOVE_CURVE', 0xFF07: 'BGI_FLOOR_MOVE_RANGE', 0xFF08: 'BGI_FLOOR_MOVE_AXIS',
                0xFF09: 'BGI_ANIM_FRAME', 0xFF0A: 'BGI_ANIM_SHOW_FRAME', 0xFF0B: 'BGI_ANIM_PLAYING',
                0xFF0C: 'BGI_ANIM_MODE', 0xFF0D: 'BGI_ANIM_RATE', 0xFF0E: 'BGI_ANIM_FRAME_VALUES',
                0xFF0F: 'BGI_ANIM_FRAME_VALUE', 0xFF10: 'BGI_ANIM_RANGE', 0xFF11: 'BGI_ANIM_RESET'}
# ops that change which triangles are walkable (BGI_ACTOR_ACTIVE only detaches an actor; the mask filter only
# changes which mask bits count, BG_MASK_FILTER 0x7F is what every exit runs before fading out)
AREA_OPS = {0x9A, 0xCB, 0xFF09, 0xFF0A, 0xFF0B, 0xFF10, 0xFF11}
MOVE_OPS = set(range(0xFF00, 0xFF09))
COND_CLASSES = ('story', 'field', 'local', 'object', 'unknown')  # guards that make an exit conditional


def s16(v):
    return v - 0x10000 if isinstance(v, int) and v >= 0x8000 else v


# ---------------------------------------------------------------- disc / container

def field_files(img):
    """[(field id, area, file id, sector, sectors)] of dir 4."""
    return [(fid & 0xFFFF, fid >> 16, fid, sec, n) for fid, sec, n in img.files(DIR_FIELDS)]


def field_pack(data):
    """Offset of the nested container 0x1B0001 (the one fld_loadFile registers), or None."""
    for k, o in db_items(data):
        if k == KIND_PACK << 16 | 1:
            return o
    return None


def pack_items(data, pack):
    """{key: (offset, end)}, an item ends where the next item (in file order) starts."""
    items = db_items(data, pack)
    offs = sorted(o for _, o in items)
    nxt = {o: (offs[i + 1] if i + 1 < len(offs) else len(data)) for i, o in enumerate(offs)}
    return items, {k: (o, nxt[o]) for k, o in items}


def first_of_kind(items, kind):
    """DB_GET_KEY_AT with index 0: the first item of that kind in container order."""
    for k, o in items:
        if k >> 16 == kind:
            return k, o
    return None, None


# ---------------------------------------------------------------- walkmesh (BGI)

def parse_bgi(data, item, end):
    """The walkmesh as stored. `item` = item offset (magic), header at item + 4."""
    probs = []
    if struct.unpack_from('<I', data, item)[0] != BGI_MAGIC:
        return None, ['walkmesh magic is not 0xACDCDEAD']
    h = item + 4
    v = struct.unpack_from(HDR_FMT, data, h)
    (size, ox, oy, oz, cx, cy, cz, nx, ny, nz, mx, my, mz, chx, chy, chz, afl, atr,
     tcnt, toff, ecnt, eoff, acnt, aoff, fcnt, foff, ncnt, noff, vcnt, voff) = v
    if h + size > end:
        probs.append(f'dataSize {size} runs past the item end')
    for name, cnt, off, sz in (('triangle', tcnt, toff, 0x28), ('edge', ecnt, eoff, 4), ('anim', acnt, aoff, 0x10),
                               ('floor', fcnt, foff, 0x20), ('normal', ncnt, noff, 0x10), ('vertex', vcnt, voff, 6)):
        if off + cnt * sz > size:
            probs.append(f'{name} table {off:#x}+{cnt}x{sz:#x} past dataSize {size:#x}')
    if probs:
        return None, probs

    def ids(off, cnt):
        if off + 4 * cnt > size:
            probs.append(f'index list {off:#x}+{cnt}x4 past dataSize')
            return []
        return list(struct.unpack_from(f'<{cnt}i', data, h + off))

    verts = [list(struct.unpack_from('<3h', data, h + voff + 6 * i)) for i in range(vcnt)]
    floors = []
    for i in range(fcnt):
        f = struct.unpack_from('<2H3h3H3h3h2H', data, h + foff + 0x20 * i)
        floors.append({'flags': f[0], 'index': f[1], 'org': list(f[2:5]), 'cur': [s16(x) for x in f[5:8]],
                       'min': list(f[8:11]), 'max': list(f[11:14]), 'triangles': ids(f[15], f[14])})
    normals = [dict(zip(('nx', 'ny', 'nz', 'invNy'), struct.unpack_from('<4i', data, h + noff + 16 * i)))
               for i in range(ncnt)]
    edges = [dict(zip(('flags', 'clone'), struct.unpack_from('<Hh', data, h + eoff + 4 * i))) for i in range(ecnt)]
    anims = []
    for i in range(acnt):
        fl, fc, rate, ctr, cur, foff2 = struct.unpack_from('<2H2h2i', data, h + aoff + 0x10 * i)
        frames = []
        for j in range(fc):
            kf, val, tc, to = struct.unpack_from('<Hh2H', data, h + foff2 + 8 * j)
            frames.append({'flags': kf, 'value': val, 'triangles': ids(to, tc)})
        anims.append({'flags': fl, 'frameCount': fc, 'frameRate': rate, 'counter': ctr, 'curFrame': cur,
                      'frames': frames})
    origin = [cx, cy, cz]
    tris = []
    for i in range(tcnt):
        t = struct.unpack_from('<2H2hH13hi', data, h + toff + 0x28 * i)
        tri = {'flags': t[0], 'active': bool(t[0] & 1), 'mask': t[0] >> 8, 'data': t[1], 'floor': t[2],
               'normal': t[3], 'thetaX': t[4], 'thetaZ': t[5], 'vertices': list(t[6:9]), 'edges': list(t[9:12]),
               'neighbors': list(t[12:15]), 'center': list(t[15:18]), 'd': t[18]}
        if not 0 <= tri['floor'] < fcnt:
            probs.append(f'triangle {i}: floor {tri["floor"]} of {fcnt}')
            tri['world'] = None
        elif not all(0 <= x < vcnt for x in tri['vertices']):
            probs.append(f'triangle {i}: vertex index out of range')
            tri['world'] = None
        else:
            fc_ = floors[tri['floor']]['cur']
            tri['world'] = [[origin[a] + fc_[a] + verts[x][a] for a in range(3)] for x in tri['vertices']]
        for nb in tri['neighbors']:
            if not -1 <= nb < tcnt:
                probs.append(f'triangle {i}: neighbour {nb} of {tcnt}')
        if tri['normal'] >= ncnt:
            probs.append(f'triangle {i}: normal {tri["normal"]} of {ncnt}')
        tris.append(tri)
    for a in anims:  # bgi_init -> bgi_initAnim: frame 0's triangles on, every other frame's off
        for j, fr in enumerate(a['frames']):
            for t in fr['triangles']:
                if 0 <= t < tcnt:
                    tris[t]['active'] = j == 0
                else:
                    probs.append(f'anim triangle {t} of {tcnt}')
    for f in floors:
        if any(not 0 <= t < tcnt for t in f['triangles']):
            probs.append('floor triangle index out of range')
    header = {'dataSize': size, 'org': [ox, oy, oz], 'cur': origin, 'min': [nx, ny, nz], 'max': [mx, my, mz],
              'charPos': [chx, chy, chz], 'activeFloor': afl, 'activeTriangle': atr, 'triangleCount': tcnt,
              'edgeCount': ecnt, 'animCount': acnt, 'floorCount': fcnt, 'normalCount': ncnt, 'vertexCount': vcnt}
    return {'header': header, 'vertices': verts, 'triangles': tris, 'edges': edges, 'floors': floors,
            'normals': normals, 'anims': anims}, probs


# ---------------------------------------------------------------- event script

def expr_bytes(arg):
    """Raw bytes of an expression argument as evtdis prints it ('expr[aa bb ...]')."""
    if isinstance(arg, str) and arg.startswith('expr['):
        return bytes.fromhex(arg[5:-1])
    return None


def expr_vars(b):
    """Classes of what an expression reads: 'G' global, 'F' field, 'L' local var, 'S' system var, 'O' other obj."""
    out = set()
    i = 0
    while i < len(b):
        c = b[i]
        i += 1
        if c == 0x7F:
            break
        if c >= 0x80:
            out.add('GFLL'[c & 3])
            i += 2 if c & 0x20 else 1
        elif c in (0x7D, 0x78):
            if c == 0x78:
                out.add('O')
            i += 2
        elif c == 0x7E:
            i += 4
        elif c in (0x79, 0x7A, 0x29, 0x5F):
            out.add('S' if c == 0x7A else 'O')
            i += 1
    return out


def expr_text(b):
    try:
        return edis.ex(b, 0)[1]
    except (IndexError, KeyError, struct.error):
        return 'expr[' + b.hex(' ') + ']'


def guard_class(vars_):
    if 'G' in vars_:
        return 'story'
    if 'F' in vars_:
        return 'field'
    if 'L' in vars_:
        return 'local'
    if 'O' in vars_:
        return 'object'
    if 'S' in vars_:
        return 'system'
    return 'constant'


class Func:
    """One decoded script function with its control-flow graph."""

    def __init__(self, entry, tag, code):
        self.entry, self.tag, self.code = entry, tag, code
        self.ins = []
        self.bad = None
        try:
            for x in evtdis.dis(code):
                if x[3] == ['??']:
                    if any(code[x[0]:]):
                        self.bad = f'unknown op {x[1]:#x} at {x[0]:#x}'
                    break
                self.ins.append(x)
        except (IndexError, struct.error) as e:
            self.bad = f'decode error {e}'
        self.at = {x[0]: i for i, x in enumerate(self.ins)}
        self._reach = {}
        self._targets = None

    def const_before(self, i):
        """Value of a constant EXPR (05 7D s16 7F / 05 7E s32 7F) right before branch i, when no jump lands on i."""
        if i == 0 or i in self.targets():
            return None
        raw = self.ins[i - 1][4]
        if len(raw) == 5 and raw[:2] == b'\x05\x7d' and raw[4] == 0x7F:
            return struct.unpack_from('<h', raw, 2)[0]
        if len(raw) == 7 and raw[:2] == b'\x05\x7e' and raw[6] == 0x7F:
            return struct.unpack_from('<i', raw, 2)[0]
        return None

    def succ(self, i, fold=True):
        """Successor instruction indexes; branches on a constant EXPR are folded (fold=True)."""
        off, op, _, _, raw = self.ins[i]
        nxt = [i + 1] if i + 1 < len(self.ins) else []
        if op == 4 or op in (OP_GOTO_FIELD, OP_GOTO_WORLD):
            return []
        end = off + len(raw)
        c = self.const_before(i) if fold and op in (2, 3, 6, 0x0B, 0x0D) else None
        if op in (1, 2, 3):
            t = self.at.get(end + struct.unpack_from('<h', raw, 1)[0])
            tl = [t] if t is not None else []
            if op == 1:
                return tl
            if c is not None:
                return tl if (c == 0) == (op == 2) else nxt
            return tl + nxt
        if op == 6:
            k = raw[1]
            vals = [struct.unpack_from('<H', raw, 4 + 4 * j)[0] for j in range(k)]
            offs = [struct.unpack_from('<H', raw, 2)[0]] + [struct.unpack_from('<H', raw, 6 + 4 * j)[0] for j in range(k)]
            if c is not None:
                offs = [offs[1 + vals.index(c & 0xFFFF)] if c & 0xFFFF in vals else offs[0]]
            return sorted({self.at[off + 4 + o] for o in offs if off + 4 + o in self.at})
        if op in (0x0B, 0x0D):
            k = raw[1] if op == 0x0B else struct.unpack_from('<H', raw, 1)[0]
            r = off + (1 if op == 0x0B else 2)
            base = raw[r - off + 1] | struct.unpack_from('<b', raw, r - off + 2)[0] << 8
            tab = r - off + 5
            offs = [struct.unpack_from('<H', raw, r - off + 3)[0]] + \
                [struct.unpack_from('<H', raw, tab + 2 * j)[0] for j in range(k)]
            if c is not None:
                offs = [offs[1 + c - base] if 0 <= c - base < k else offs[0]]
            return sorted({self.at[r + o] for o in offs if r + o in self.at})
        return nxt

    def targets(self):
        """Instruction indexes some jump lands on (unfolded)."""
        if self._targets is None:
            self._targets = set()
            for j in range(len(self.ins)):
                self._targets |= {t for t in self.succ(j, fold=False) if t != j + 1}
        return self._targets

    def live(self, i):
        """Instruction i can run (reachable from the function start, constant branches folded)."""
        return i in self.reach(0)

    def reach(self, start):
        if start not in self._reach:
            self._reach[start] = self.reach_from([start])
        return self._reach[start]

    def reach_from(self, starts, block=None):
        seen = set()
        todo = [s for s in starts if s != block]
        while todo:
            n = todo.pop()
            if n in seen:
                continue
            seen.add(n)
            todo += [s for s in self.succ(n) if s != block and s not in seen]
        return seen

    def guards(self, i):
        """Branches that decide whether instruction i runs: reachable from the start, with one successor that
        reaches i and one that does not. The condition is the EXPR right before the branch."""
        live = self.reach(0) if self.ins else set()
        out = []
        for b in sorted(live):
            op = self.ins[b][1]
            if op not in (2, 3, 6, 0x0B, 0x0D):
                continue
            succ = self.succ(b)
            hits = [i in self.reach(s) for s in succ]
            if any(hits) and not all(hits):
                cond = self.ins[b - 1] if b > 0 and self.ins[b - 1][1] == 5 else None
                eb = cond[4][1:] if cond else b''
                cls = guard_class(expr_vars(eb)) if cond else 'unknown'
                out.append({'at': [self.entry, self.tag, self.ins[b][0]], 'class': cls,
                            'expr': expr_text(eb) if cond else None})
        return out

    def arrival(self, i):
        """Literal Gs16[2] assignment (05 d8 02 7d lo hi 2c 7f) in the straight-line code before instruction i."""
        targets = self.targets()
        j = i - 1
        while j >= 0:
            raw = self.ins[j][4]
            if raw[:4] == b'\x05\xd8\x02\x7d' and raw[6:8] == b'\x2c\x7f' and len(raw) == 8:
                return struct.unpack_from('<h', raw, 4)[0]
            if self.ins[j][1] in (1, 2, 3, 4, 6, 0x0B, 0x0D) or j in targets:
                return None
            j -= 1
        return None


class Script:
    def __init__(self, data, s):
        self.count = data[s + 3]
        self.name = data[s + 4:s + 0x2C].split(b'\0')[0].decode('shift_jis', 'replace')
        self.funcs = {}
        for e, tag, code in script_functions(data, s):
            self.funcs[(e, tag)] = Func(e, tag, code)
        self.created = defaultdict(list)  # entry -> [(kind op, uid, creator entry, tag, offset, story guarded)]
        self.uid2entry = defaultdict(set)
        self.player_entries = set()
        for (e, tag), f in self.funcs.items():
            for i, (off, op, fl, args, raw) in enumerate(f.ins):
                if op in (OP_NEW_CODE, OP_NEW_REGION, OP_NEW_ACTOR) and f.live(i):
                    ent, uid = args
                    uid = uid or ent
                    sg = any(g['class'] == 'story' for g in f.guards(i))
                    self.created[ent].append((op, uid, e, tag, off, sg))
                    self.uid2entry[uid].add(ent)
                elif op == 0x2C:
                    self.player_entries.add(e)

    def kind(self, ent):
        """region / actor / code (created by 08 / 09 / 07), main (entry 0, started by evt_init), party (the last
        nine entries, evt_init creates them as actors), unused (never created)."""
        ops = {c[0] for c in self.created.get(ent, [])}
        if OP_NEW_REGION in ops:
            return 'region'
        if OP_NEW_ACTOR in ops:
            return 'actor'
        if OP_NEW_CODE in ops:
            return 'code'
        return 'main' if ent == 0 else 'party' if ent >= self.count - 9 else 'unused'

    def tags(self, ent):
        return sorted(t for (e, t) in self.funcs if e == ent)

    def targets(self, cur, uid):
        if uid == 0xFF:
            return {cur}
        if 0xFA <= uid <= 0xFE:
            return set(self.player_entries)
        return self.uid2entry.get(uid, {uid})

    def walk(self, ent, tag, depth=2, via=()):
        """(func, instruction index, via) for every instruction of (ent, tag) and of functions it CALLs."""
        f = self.funcs.get((ent, tag))
        if f is None:
            return
        for i, (off, op, fl, args, raw) in enumerate(f.ins):
            if not f.live(i):
                continue
            yield f, i, via
            if op in CALL_OPS and depth and isinstance(args[1], int):
                for te in sorted(self.targets(ent, args[1])):
                    if (te, args[2]) != (ent, tag):
                        yield from self.walk(te, args[2], depth - 1, via + ([ent, tag, off, f'{op:02X}'],))


def goto_record(f, i, via):
    off, op, fl, args, raw = f.ins[i]
    eb = expr_bytes(args[0])
    return {'kind': 'field' if op == OP_GOTO_FIELD else 'world', 'id': None if eb else args[0],
            'expr': expr_text(eb) if eb else None, 'arrival': f.arrival(i), 'at': [f.entry, f.tag, off],
            'via': [list(v) for v in via], 'guards': f.guards(i)}


def region_polygons(sc, ent):
    out = []
    for tag in sc.tags(ent):
        f = sc.funcs[(ent, tag)]
        cur = None
        for i, (off, op, fl, args, raw) in enumerate(f.ins):
            if not f.live(i):
                continue
            sg = None
            if op == OP_SET_REGION:
                n = args[0]
                pts = args[1:]
                exprs = [expr_text(expr_bytes(a)) for p in pts for a in p if expr_bytes(a)]
                if expr_bytes(n):
                    exprs.insert(0, expr_text(expr_bytes(n)))
                lit = not exprs
                cur = {'entry': ent, 'tag': tag, 'offset': off, 'op': 'SET_REGION',
                       'points': [[s16(x), s16(z)] for x, z in pts] if lit else None, 'exprs': exprs}
                sg = True
            elif op in (OP_REGION_START, OP_REGION_MOVE_LAST, OP_REGION_DUP_LAST):
                # straight-line emulation of the point-edit ops (event.c EVT_OP_REGION_*)
                if cur is None or cur.get('op') != 'edit' or cur['tag'] != tag:
                    cur = {'entry': ent, 'tag': tag, 'offset': off, 'op': 'edit', 'points': [], 'exprs': []}
                    sg = True
                pts = cur['points']
                ex = [expr_text(expr_bytes(a)) for a in args if expr_bytes(a)]
                cur['exprs'] += ex
                if pts is not None and not ex:
                    if op == OP_REGION_START:
                        pts[:] = [[s16(args[0]), s16(args[1])]] * 2
                    elif op == OP_REGION_MOVE_LAST:
                        pts[max(len(pts), 1) - 1:] = [[s16(args[0]), s16(args[1])]]
                    elif 1 <= len(pts) < 8:
                        pts.append(list(pts[-1]))
                else:
                    cur['points'] = None
            if sg:
                cur['story_guarded'] = any(g['class'] == 'story' for g in f.guards(i))
                out.append(cur)
    return out


def analyse_script(sc):
    regions, exits, problems = [], [], []
    region_gotos = set()
    for ent in sorted(e for e in sc.created if sc.kind(e) == 'region'):
        uid = sorted({c[1] for c in sc.created[ent]})
        polys = region_polygons(sc, ent)
        reg = {'entry': ent, 'uid': uid[0] if len(uid) == 1 else uid,
               'created_at': [[c[2], c[3], c[4], c[5]] for c in sc.created[ent]],
               'polygons': polys, 'functions': sc.tags(ent), 'exit_triggers': []}
        lit = [p for p in polys if p['points'] is not None]
        for trigger, tags in (('range', (FUNC_RANGE,)), ('action', (FUNC_TALK2, FUNC_TALK))):
            dests = []
            for tag in tags:
                for f, i, via in sc.walk(ent, tag):
                    if f.ins[i][1] in (OP_GOTO_FIELD, OP_GOTO_WORLD):
                        dests.append(goto_record(f, i, via))
                        region_gotos.add((f.entry, f.tag, f.ins[i][0]))
            if not dests:
                continue
            ids = {(d['kind'], d['id']) for d in dests}
            reg['exit_triggers'].append(trigger)
            exits.append({'entry': ent, 'uid': reg['uid'], 'trigger': trigger,
                          'polygon': lit[0]['points'] if lit else None, 'polygon_count': len(polys),
                          'destinations': dests,
                          'conditional': any(g['class'] in COND_CLASSES for d in dests for g in d['guards']),
                          'story_dependent': len(ids) > 1 or any(d['id'] is None for d in dests) or
                          any(g['class'] == 'story' for d in dests for g in d['guards']),
                          'created_story_guarded': any(c[5] for c in sc.created[ent]) or
                          any(p['story_guarded'] for p in polys)})
        regions.append(reg)
    other, wops, npcs = [], [], []
    for (e, tag), f in sorted(sc.funcs.items()):
        if f.bad:
            problems.append(f'entry {e} tag {tag}: {f.bad}')
        for i, (off, op, fl, args, raw) in enumerate(f.ins):
            if not f.live(i):
                continue
            if op in (OP_GOTO_FIELD, OP_GOTO_WORLD) and (e, tag, off) not in region_gotos:
                g = goto_record(f, i, ())
                other.append({'kind': g['kind'], 'id': g['id'], 'expr': g['expr'], 'at': g['at'],
                              'owner_kind': sc.kind(e)})
            if op in WALKMESH_OPS:
                lit = not any(expr_bytes(a) for a in args)
                wops.append({'op': f'{op:02X}', 'name': WALKMESH_OPS[op],
                             'args': [a if isinstance(a, int) else expr_text(expr_bytes(a)) for a in args],
                             'at': [e, tag, off], 'literal': lit,
                             'story_guarded': any(g['class'] == 'story' for g in f.guards(i))})
    for ent in sorted(e for e in sc.created if sc.kind(e) == 'actor'):
        menus = []
        for tag in (FUNC_TALK, FUNC_TALK2):
            for f, i, via in sc.walk(ent, tag):
                off, op, fl, args, raw = f.ins[i]
                if op == OP_MENU:
                    menus.append([a if isinstance(a, int) else expr_text(expr_bytes(a)) for a in args])
        if not menus:
            continue
        pos = []
        f = sc.funcs.get((ent, FUNC_INIT))
        for off, op, fl, args, raw in (f.ins if f else []):
            if op in POS_OPS:
                pos.append({'op': POS_OPS[op], 'pos': [s16(a) if isinstance(a, int) else None for a in args],
                            'at': [ent, FUNC_INIT, off]})
        uid = sorted({c[1] for c in sc.created[ent]})
        npcs.append({'entry': ent, 'uid': uid[0] if len(uid) == 1 else uid,
                     'menus': [list(m) for m in dict.fromkeys(tuple(m) for m in menus)], 'positions': pos})
    return regions, exits, other, wops, npcs, problems


# ---------------------------------------------------------------- per field

def read_field(img, fid, sec, n):
    data = img.read(sec * SECTOR, n * SECTOR)
    pack = field_pack(data)
    return data, pack


def dump_field(img, disc, ent, mesh=True):
    field, area, fid, sec, n = ent
    data, pack = read_field(img, fid, sec, n)
    out = {'disc': disc, 'field': field, 'area': area, 'file_id': fid, 'sector': sec, 'sectors': n, 'name': None,
           'walkmesh': None, 'regions': [], 'exits': [], 'other_gotos': [], 'walkmesh_ops': [], 'npcs': [],
           'problems': []}
    if pack is None:
        out['problems'].append('no container 0x1B0001')
        return out
    items, ext = pack_items(data, pack)
    bkey, _ = first_of_kind(items, KIND_BG)
    wkey, woff = first_of_kind(items, KIND_WALKMESH)
    if bkey is None:
        out['problems'].append('no background item (kind 0x0A)')
    if wkey is None:
        out['problems'].append('no walkmesh item (kind 0x0B)')
    else:
        bgi, probs = parse_bgi(data, woff, ext[wkey][1])
        out['problems'] += probs
        if bgi is not None:
            bgi['item_key'] = wkey
            out['walkmesh'] = bgi
    skey = KIND_FIELD << 16 | field
    if skey not in ext or data[ext[skey][0]:ext[skey][0] + 2] != b'EV':
        out['problems'].append(f'no event script {skey:#x}')
        return out
    sc = Script(data, ext[skey][0])
    out['name'] = sc.name
    (out['regions'], out['exits'], out['other_gotos'], out['walkmesh_ops'], out['npcs'],
     probs) = analyse_script(sc)
    out['problems'] += probs
    if not mesh and out['walkmesh']:
        w = out['walkmesh']
        out['walkmesh'] = {'item_key': w['item_key'], 'header': w['header']}
    return out


def open_discs(discs, img_arg):
    out = []
    for d in discs:
        p = find_image(d, img_arg)
        if p:
            out.append((d, Img(p)))
    return out


# ---------------------------------------------------------------- check / statistics

def on_mesh(pts, w):
    """Does a region polygon (field x/z) touch the walkmesh: a corner or the centroid inside some triangle's x/z."""
    cx = sum(p[0] for p in pts) / len(pts)
    cz = sum(p[1] for p in pts) / len(pts)
    for t in w['triangles']:
        if t['world'] is None:
            continue
        (ax, _, az), (bx, _, bz), (qx, _, qz) = t['world']
        for x, z in pts + [[cx, cz]]:
            d1 = (x - bx) * (az - bz) - (ax - bx) * (z - bz)
            d2 = (x - qx) * (bz - qz) - (bx - qx) * (z - qz)
            d3 = (x - ax) * (qz - az) - (qx - ax) * (z - az)
            if not ((d1 < 0 or d2 < 0 or d3 < 0) and (d1 > 0 or d2 > 0 or d3 > 0)):
                return True
    return False


def check(discs):
    st = Counter()
    per_disc = {}
    seen = {}
    changing = defaultdict(set)
    story_changing = set()
    all_ids = set()
    moving = set()
    anim_fields = set()
    problems = []
    op_use = Counter()
    op_fields = defaultdict(set)
    tri_flag_vals = Counter()
    mask_vals = Counter()
    for disc, img in discs:
        ents = field_files(img)
        per_disc[disc] = {'files': len(ents), 'fields': len({e[0] for e in ents}),
                          'areas': len({e[1] for e in ents}), 'min': min(e[0] for e in ents),
                          'max': max(e[0] for e in ents)}
        all_ids |= {e[0] for e in ents}
        for ent in ents:
            d = dump_field(img, disc, ent)
            st['files'] += 1
            for p in d['problems']:
                problems.append(f'disc {disc} field {d["field"]}: {p}')
            w = d['walkmesh']
            key = (d['field'], json.dumps([d['regions'], d['walkmesh_ops'], w and w['header']], sort_keys=True))
            dup = d['field'] in seen and seen[d['field']] == key[1]
            seen[d['field']] = key[1]
            if w:
                st['walkmeshes'] += 1
                st['triangles'] += w['header']['triangleCount']
                st['max_triangles'] = max(st['max_triangles'], w['header']['triangleCount'])
                st['max_vertices'] = max(st['max_vertices'], w['header']['vertexCount'])
                st['max_floors'] = max(st['max_floors'], w['header']['floorCount'])
                if w['header']['org'] != w['header']['cur']:
                    st['header_cur_ne_org'] += 1
                if any(f['org'] != f['cur'] for f in w['floors']):
                    st['floor_cur_ne_org'] += 1
                if w['anims']:
                    st['meshes_with_anims'] += 1
                    anim_fields.add(d['field'])
                for t in w['triangles']:
                    tri_flag_vals[t['flags'] & 0xFF] += 1
                    mask_vals[t['mask']] += 1
                    if not t['active']:
                        st['inactive_after_load'] += 1
            if dup:
                st['dup_scripts'] += 1
                continue
            st['scripts'] += 1
            for r in d['regions']:
                st['regions'] += 1
                ps = r['polygons']
                if not ps:
                    st['regions_no_polygon'] += 1
                elif all(p['points'] is not None for p in ps):
                    st['regions_literal'] += 1
                else:
                    st['regions_expr'] += 1
                if any(p['op'] == 'edit' for p in ps):
                    st['regions_edit_ops'] += 1
                if len(ps) > 1:
                    st['regions_multi_polygon'] += 1
                for p in ps:
                    if p['points'] is not None:
                        if len(p['points']) < 3:
                            st['polygons_lt3'] += 1
                        st['polygons_literal'] += 1
            for x in d['exits']:
                st['exits'] += 1
                st[f'exits_trigger_{x["trigger"]}'] += 1
                ds = x['destinations']
                st['exit_gotos'] += len(ds)
                if x['polygon'] is not None:
                    st['exits_literal_polygon'] += 1
                    if w and x['polygon'] and on_mesh(x['polygon'], w):
                        st['exits_polygon_on_mesh'] += 1
                if all(g['id'] is not None for g in ds):
                    st['exits_literal_dest'] += 1
                else:
                    st['exits_expr_dest'] += 1
                ids = {(g['kind'], g['id']) for g in ds}
                if len(ids) > 1:
                    st['exits_multi_dest'] += 1
                if x['conditional']:
                    st['exits_conditional'] += 1
                if x['story_dependent']:
                    st['exits_story_dependent'] += 1
                if any(g['via'] for g in ds):
                    st['exits_via_call'] += 1
                if any(g['kind'] == 'world' for g in ds):
                    st['exits_to_world'] += 1
                if x['polygon_count'] > 1:
                    st['exits_multi_polygon'] += 1
                if not x['conditional'] and len(ids) == 1:
                    st['exits_simple'] += 1
                if any(g['arrival'] is not None for g in ds):
                    st['exits_with_arrival'] += 1
                if x['created_story_guarded']:
                    st['exits_created_story_guarded'] += 1
                if x['story_dependent'] or x['created_story_guarded']:
                    st['exits_story_any'] += 1
            for g in d['other_gotos']:
                st['other_gotos'] += 1
                st[f'other_gotos_{g["owner_kind"]}'] += 1
            for o in d['walkmesh_ops']:
                op = int(o['op'], 16)
                op_use[o['name']] += 1
                op_fields[o['name']].add(d['field'])
                if op in AREA_OPS:
                    changing[d['field']].add(o['name'])
                    if o['story_guarded']:
                        story_changing.add(d['field'])
                if op in MOVE_OPS:
                    moving.add(d['field'])
            for n in d['npcs']:
                st['npcs_menu'] += 1
                types = {m[0] for m in n['menus']}
                for t, nm in ((2, 'shop'), (4, 'save')):
                    if t in types:
                        st[f'npcs_menu_{nm}'] += 1
                if n['positions']:
                    st['npcs_with_init_pos'] += 1
    st['field_ids_all_discs'] = len(all_ids)
    return {'per_disc': per_disc, 'stats': dict(st), 'problems': problems,
            'area_changing_story_fields': sorted(story_changing),
            'walkmesh_ops': {k: {'uses': v, 'fields': len(op_fields[k])} for k, v in op_use.most_common()},
            'area_changing_fields': {f: sorted(v) for f, v in sorted(changing.items())},
            'floor_move_fields': sorted(moving), 'anim_mesh_fields': sorted(anim_fields),
            'tri_flags_low_byte': {f'{k:#04x}': v for k, v in sorted(tri_flag_vals.items())},
            'tri_mask_byte': {f'{k:#04x}': v for k, v in sorted(mask_vals.items())}}


def main():
    ap = argparse.ArgumentParser(description=__doc__, formatter_class=argparse.RawDescriptionHelpFormatter)
    ap.add_argument('cmd', choices=['list', 'dump', 'check'])
    ap.add_argument('field', type=int, nargs='?')
    ap.add_argument('--disc', type=int, action='append', choices=range(1, 5))
    ap.add_argument('--img', help='FF9.IMG (with one --disc) or a directory holding discN/FF9.IMG')
    ap.add_argument('--no-mesh', action='store_true', help='dump: only the walkmesh header')
    ap.add_argument('--json', help='check: also write the full result here')
    ap.add_argument('--indent', type=int, default=None)
    a = ap.parse_args()
    discs = open_discs(a.disc or [1, 2, 3, 4], a.img)
    if not discs:
        sys.exit('no disc image found')
    if a.cmd == 'list':
        out = []
        for disc, img in discs:
            for ent in field_files(img):
                data, pack = read_field(img, ent[2], ent[3], ent[4])
                name = None
                if pack is not None:
                    _, ext = pack_items(data, pack)
                    k = KIND_FIELD << 16 | ent[0]
                    if k in ext:
                        s = ext[k][0]
                        name = data[s + 4:s + 0x2C].split(b'\0')[0].decode('shift_jis', 'replace')
                out.append({'disc': disc, 'field': ent[0], 'area': ent[1], 'file_id': ent[2], 'sector': ent[3],
                            'sectors': ent[4], 'name': name})
        print(json.dumps(out, indent=a.indent))
    elif a.cmd == 'dump':
        if a.field is None:
            sys.exit('dump needs a field id')
        for disc, img in discs:
            for ent in field_files(img):
                if ent[0] == a.field:
                    print(json.dumps(dump_field(img, disc, ent, not a.no_mesh), indent=a.indent))
                    return
        sys.exit(f'field {a.field} not on disc(s) {[d for d, _ in discs]}')
    else:
        r = check(discs)
        for d, v in r['per_disc'].items():
            print(f'disc {d}: {v["files"]} field files, {v["fields"]} field ids ({v["min"]}-{v["max"]}), '
                  f'{v["areas"]} areas')
        for k, v in sorted(r['stats'].items()):
            print(f'  {k}: {v}')
        print('walkmesh ops:', r['walkmesh_ops'])
        print('fields whose walkable area changes:', len(r['area_changing_fields']), sorted(r['area_changing_fields']))
        print('  of those, behind a story guard:', len(r['area_changing_story_fields']), r['area_changing_story_fields'])
        print('floor-move fields:', r['floor_move_fields'])
        print('fields with walkmesh anims:', len(r['anim_mesh_fields']))
        print('triangle flags low byte:', r['tri_flags_low_byte'])
        print('triangle mask byte:', r['tri_mask_byte'])
        print(f'problems: {len(r["problems"])}')
        for p in r['problems'][:40]:
            print('  ', p)
        if a.json:
            with open(a.json, 'w') as f:
                json.dump(r, f, indent=1)
        sys.exit(1 if r['problems'] else 0)


if __name__ == '__main__':
    main()
