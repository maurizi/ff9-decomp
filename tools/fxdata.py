#!/usr/bin/env python3
"""Battle effect data: list, classify and render the data entries of FF9.IMG dir 13 effect files.

    python3 tools/fxdata.py fx_004                      # entry table of the module's file(s) (+ C usage)
    python3 tools/fxdata.py 0x4 0x4d --render           # also PNGs: build/fxdata/<file>/<idx>[_<what>].png
    python3 tools/fxdata.py fx_004 --json out.json      # everything as JSON
    python3 tools/fxdata.py fx_004 fx_04d --md out.md   # markdown summary
    python3 tools/fxdata.py --vram fx_004               # the file's VRAM uploads as build/fxdata/<file>/vram.png

Run it in Docker (`tools/docker.sh python3 tools/fxdata.py ...`); stdlib only (PNGs are written with zlib). Reads
disks/us/disc1/FF9.IMG (--disks DIR), the effect modules' C (src/fxmod/<module>/*.c) and docs/effect-modules.md.
Arguments are module names (fx_004, fx_17d_4) or dir-13 file ids (0x4); a module stands for every file id that
shares its image (config/fxmods.us.txt).

What an effect file holds (docs/ff9img.md "Battle effect files", docs/fxdata.md):
- chunk type 0 / 1: textures. Type 0 = {u32 next, u32 imgs, u32 nimgs} + images {u16 x, y, w, h, pixels} (VRAM
  rects, mostly CLUTs), then at `next` {u32 pixels, u32 ?, RECT rects[]}: `flag` 64x128 16-bit pages go to the
  rects (a 256-high rect takes two pages); type 1 chunks carry `flag` more pages for the same rects
  (btlfx_uploadTextures). The tool replays them into a 1024x512 VRAM, plus the common effect pages every battle
  keeps (FF9.IMG dir 1 file 0x2BF, btlfx_reloadCommonData: four pages at (0x300, 0x100), CLUT rows at y 0xF0).
- chunk type 2 (+10 appended): effect data, read by btlfx_getFileData(slot, i) = base + base[i]: a table of s32
  offsets (count = first offset / 4); entry i runs to the next larger offset. Entry 0 is the slot's heap.
- shared data (btlfx_getFileData's other bases, listed after the file's own entries with the full index):
  0x40+ the ring start (the first ring-start type 2 chunk, or its extra-sector common part; only when the C uses
  it), 0x80+ the summon data = g_btlfxLoader.summon->data[0] = the last type 6 chunk (BTLFX_LDCMD_SUMMON_DATA)
  loaded up to the module's group, 0xC0+ the battle load buffer when a type 8 chunk fills it.
- summon files also upload: type 4 (BTLFX_LDCMD_SUMMON_TEX: texCount 64x128 pages at the header's tpage/y, CLUT
  rows at (0x100, 0xE6)) and type 9 (BTLFX_LDCMD_COMMON_TEX: pages by mask at (0x300/0x340, 0x100) and 0x140 /
  0x180); the tool replays both into the VRAM.
- the effect script: 3-byte ops at offset 0x400 of the header sector. Ops naming data: 0x23 / 0x29 camera,
  0x2D sound, 0x63 / 0x64 party motion (b), control 0x0F vibration (a, when b = 0); 0x80+ run the module's ops.

Kinds (content checks, then how the module's C uses the entry, which wins):
  heap, sound (AKAO), emitter (EffEmit + EffElemHdr + EffElem[] + EffTail, btlfx_spawnEmitter), somodel ('so'
  header, btlfx_registerEffectModel), model (BtlModelHeader, Hi_RegisterTexEffModel), camera (btlcam script),
  motion, vibration, sprite sets by draw call (rings, trail, sprite, quads, fan), table (anything else).
Rendering: emitter elements draw frame 0..7 of their sprite data (kinds 0-3: 10-byte quads / 3-byte sprites,
trails: the strip texture, rings: the texture UV box or the ring colours, fans: the textured frame rect or the
gouraud triangles of btlfx_emitFrameFan; kind-3 sprites with tpage 0 are btlfx_emitDots' 1-pixel dots), models
(BtlModelHeader and 'so') front / top / oblique textured renders (per-polygon texture from the 'so' texture list or
a Hi_RegisterTexListModel table read from the module binary) plus an oblique wireframe and the UV boxes,
standalone sprite sets the same as emitter elements. Sprites read pixels from the replayed VRAM with their
tpage/CLUT; texels nobody uploaded show as a magenta checker. Sheet labels: R/S/L prefix = ring / summon / load
buffer index.
"""
import argparse
import json
import os
import re
import struct
import sys
import zlib

sys.path.insert(0, os.path.dirname(os.path.abspath(__file__)))
from ff9img import Img, SECTOR  # noqa: E402
import effect_modules as EM  # noqa: E402

ROOT = os.path.dirname(os.path.dirname(os.path.abspath(__file__)))
DIR_COMMON_IDX = 1
FILE_BTLFX_COMMON = 0x2BF
SCRIPT_OFS = 0x400

# ---------------------------------------------------------------------------------------------------------------
# PNG / drawing


def write_png(path, w, h, rgb):
    """rgb: bytearray w*h*3."""
    raw = b''.join(b'\0' + bytes(rgb[y * w * 3:(y + 1) * w * 3]) for y in range(h))

    def chunk(t, d):
        c = struct.pack('>I', len(d)) + t + d
        return c + struct.pack('>I', zlib.crc32(t + d) & 0xFFFFFFFF)
    png = b'\x89PNG\r\n\x1a\n' + chunk(b'IHDR', struct.pack('>IIBBBBB', w, h, 8, 2, 0, 0, 0))
    png += chunk(b'IDAT', zlib.compress(raw, 6)) + chunk(b'IEND', b'')
    os.makedirs(os.path.dirname(path), exist_ok=True)
    with open(path, 'wb') as f:
        f.write(png)


class Canvas:
    def __init__(self, w, h, bg=(0, 0, 0)):
        self.w, self.h = w, h
        self.px = bytearray(bytes(bg) * (w * h))

    def put(self, x, y, c, add=True):
        if 0 <= x < self.w and 0 <= y < self.h:
            o = (y * self.w + x) * 3
            if add:
                for k in range(3):
                    self.px[o + k] = min(255, self.px[o + k] + c[k])
            else:
                self.px[o:o + 3] = bytes(c)

    def line(self, x0, y0, x1, y1, c):
        dx, dy = abs(x1 - x0), -abs(y1 - y0)
        sx, sy = (1 if x0 < x1 else -1), (1 if y0 < y1 else -1)
        err = dx + dy
        for _ in range(4096):
            self.put(x0, y0, c, add=False)
            if x0 == x1 and y0 == y1:
                break
            e2 = 2 * err
            if e2 >= dy:
                err += dy
                x0 += sx
            if e2 <= dx:
                err += dx
                y0 += sy

    def blit(self, other, ox, oy):
        for y in range(other.h):
            for x in range(other.w):
                o = (y * other.w + x) * 3
                self.put(ox + x, oy + y, other.px[o:o + 3], add=False)

    def save(self, path):
        write_png(path, self.w, self.h, self.px)


def upscale(c, target=96, max_w=1600):
    """Nearest-neighbour upscale so small sprites are visible."""
    k = max(1, min(8, target // max(c.h, 1), max_w // max(c.w, 1)))
    return scale_canvas(c, k)


def scale_canvas(c, k):
    if k == 1:
        return c
    out = Canvas(c.w * k, c.h * k)
    for y in range(out.h):
        for x in range(out.w):
            o = ((y // k) * c.w + x // k) * 3
            p = (y * out.w + x) * 3
            out.px[p:p + 3] = c.px[o:o + 3]
    return out


# ---------------------------------------------------------------------------------------------------------------
# VRAM


class Vram:
    W, H = 1024, 512

    def __init__(self):
        self.mem = [0] * (self.W * self.H)
        self.set = bytearray(self.W * self.H)

    def load(self, x, y, w, h, data, off=0):
        for j in range(h):
            row = struct.unpack_from(f'<{w}H', data, off + j * w * 2)
            for i in range(w):
                xx, yy = (x + i) & 1023, (y + j) & 511
                self.mem[yy * 1024 + xx] = row[i]
                self.set[yy * 1024 + xx] = 1
        return off + w * h * 2

    def texel(self, tpage, clut, u, v):
        """(r, g, b) or None (transparent) or 'missing'."""
        tx, ty, mode = (tpage & 0xF) * 64, ((tpage >> 4) & 1) * 256, (tpage >> 7) & 3
        u &= 0xFF
        v &= 0xFF
        y = ty + v
        if mode == 0:
            a = y * 1024 + tx + (u >> 2)
            if not self.set[a]:
                return 'missing'
            idx = (self.mem[a] >> ((u & 3) * 4)) & 0xF
        elif mode == 1:
            a = y * 1024 + tx + (u >> 1)
            if not self.set[a]:
                return 'missing'
            idx = (self.mem[a] >> ((u & 1) * 8)) & 0xFF
        else:
            a = y * 1024 + tx + u
            if not self.set[a]:
                return 'missing'
            return c15(self.mem[a])
        ca = (clut >> 6) * 1024 + (clut & 0x3F) * 16 + idx
        if ca >= len(self.mem) or not self.set[ca]:
            return 'missing'
        return c15(self.mem[ca])

    def save(self, path, mask_only=False):
        c = Canvas(self.W, self.H)
        for a in range(self.W * self.H):
            if self.set[a]:
                col = c15(self.mem[a]) or (0, 0, 0)
                c.px[a * 3:a * 3 + 3] = bytes(col)
            else:
                c.px[a * 3:a * 3 + 3] = b'\x20\x00\x20'
        c.save(path)


def c15(p):
    if p == 0:
        return None
    r, g, b = p & 31, (p >> 5) & 31, (p >> 10) & 31
    return (r << 3 | r >> 2, g << 3 | g >> 2, b << 3 | b >> 2)


def tex_rect(vram, tpage, clut, u0, v0, w, h, tint=(128, 128, 128)):
    """Canvas of a texture rect (u0, v0, w, h); missing texels = magenta checker. Returns (canvas, missing count)."""
    w, h = max(1, min(w, 256)), max(1, min(h, 256))
    c = Canvas(w, h)
    miss = 0
    for y in range(h):
        for x in range(w):
            t = vram.texel(tpage, clut, u0 + x, v0 + y)
            if t == 'missing':
                miss += 1
                t = (255, 0, 255) if (x // 4 + y // 4) & 1 else (96, 0, 96)
            elif t is None:
                continue
            else:
                t = tuple(min(255, t[k] * tint[k] // 128) for k in range(3))
            c.put(x, y, t, add=False)
    return c, miss


# ---------------------------------------------------------------------------------------------------------------
# effect file


def u16(d, o):
    return struct.unpack_from('<H', d, o)[0]


def s16(d, o):
    return struct.unpack_from('<h', d, o)[0]


def u32(d, o):
    return struct.unpack_from('<I', d, o)[0]


def s32(d, o):
    return struct.unpack_from('<i', d, o)[0]


def parse_header(h):
    """[{kind, chunks: [{type, flag, sectors, extra}]}] (the loader's command list)."""
    ng = u16(h, 0)
    p = 2
    groups = []
    for _ in range(ng):
        kind, nch = struct.unpack_from('<2H', h, p)
        p += 4
        ch = []
        for _ in range(nch):
            ty, fl, ns = struct.unpack_from('<BBH', h, p)
            p += 4
            extra = 0
            if ty == 2 and fl:
                extra = u16(h, p)
                p += 2
            ch.append({'type': ty, 'flag': fl, 'sectors': ns, 'extra': extra})
        groups.append({'kind': kind, 'chunks': ch})
    return groups


class FxFile:
    def __init__(self, img, fid, common_vram=None):
        files = {f: (s, n) for f, s, n in EM.fx_files(img)}
        if fid not in files:
            raise KeyError(f'no dir-13 file {fid:#x}')
        self.fid = fid
        sec, nsec = files[fid]
        self.header = img.read(sec * SECTOR, SECTOR)
        self.groups = parse_header(self.header)
        cur = sec + 1
        self.vram = Vram()
        if common_vram is not None:
            self.vram.mem = list(common_vram.mem)
            self.vram.set = bytearray(common_vram.set)
        self.common_set = bytearray(self.vram.set)
        self.slots = []        # per group: data bytes of the slot (type 2 + appended type 10); extra slots follow
        self.common = None     # data before the ring base (extra sectors), if any
        self.summon_hdr = None  # type 4: the summon file header (Unk80100DC054) whose textures were uploaded
        self.common_tex = []   # type 9: [(x, y)] common pages the file uploads
        summon_raw, loadbuf, ring_group = [], None, None
        rects, tex_index = [], 0
        # the data ring as btlfx_updateLoader fills it: a flagged type 2 chunk restarts it (ringBase = RING + its
        # common part, ringSize = 0xC800 - that), each type 2 chunk goes at the cursor, which wraps to ringBase when
        # it reaches the end; a type 10 chunk goes at the (possibly wrapped) cursor, so it can land *below* its
        # group's data, which then indexes it with negative offsets (Ark groups 2, 4, 7).
        ring_cur, ring_size = None, 0xC800
        self.slot_cuts = []    # per slot: extra entry bounds (ends of the pieces of such a split slot)
        self.slot_base = []    # per slot: buffer offset of the data start (bytes of type 10 data placed below it)
        for gi, g in enumerate(self.groups):
            data = None
            data_pos, below = None, []
            for c in g['chunks']:
                n = c['sectors'] + c['extra']
                raw = img.read(cur * SECTOR, n * SECTOR)
                c['sector'] = cur
                cur += n
                if c['type'] == 0:
                    nxt, imgs, nimg = struct.unpack_from('<3I', raw, 0)
                    o = imgs
                    c['images'] = []
                    for _ in range(nimg):
                        x, y, w, hh = struct.unpack_from('<4H', raw, o)
                        c['images'].append((x, y, w, hh))
                        o = self.vram.load(x, y, w, hh, raw, o + 8)
                    pix = u32(raw, nxt)
                    rects = []
                    q = nxt + 8
                    while q + 8 <= len(raw) and len(rects) < 64:
                        r = struct.unpack_from('<4h', raw, q)
                        if r[2] != 64 or r[3] not in (128, 256):
                            break
                        rects.append(r)
                        q += 8
                    c['rects'] = rects
                    tex_index = 0
                    rects, tex_index = self._pages(raw, nxt + pix, c['flag'], rects, tex_index)
                elif c['type'] == 1:
                    rects, tex_index = self._pages(raw, 0, c['flag'], rects, tex_index)
                elif c['type'] == 2:
                    ex = c['extra'] * SECTOR
                    if ex:
                        self.common = raw[:ex]
                    data = bytearray(raw[ex:])
                    if c['flag'] and ring_group is None:
                        ring_group = gi
                    if c['flag']:
                        ring_cur, ring_size = 0, 0xC800 - ex
                    data_pos = ring_cur
                    if ring_cur is not None:
                        ring_cur += len(data)
                        if ring_cur >= ring_size:
                            ring_cur = 0
                elif c['type'] == 10 and data is not None:
                    if ring_cur is not None and data_pos is not None and ring_cur != data_pos + len(data):
                        below.append((ring_cur - data_pos, raw))
                    else:
                        data += raw
                    if ring_cur is not None:
                        ring_cur += len(raw)
                elif c['type'] == 4:
                    self._summon_tex(raw)
                elif c['type'] == 6:
                    summon_raw.append((gi, raw))
                elif c['type'] == 8:
                    loadbuf = raw
                elif c['type'] == 9:
                    self._common_tex(raw, c['flag'])
            base, cuts = 0, set()
            if data is not None and below:
                lo = min(0, min(r for r, _ in below))
                hi = max(len(data), max(r + len(b) for r, b in below))
                buf = bytearray(hi - lo)
                base = -lo
                buf[base:base + len(data)] = data
                for r, b in below:
                    buf[base + r:base + r + len(b)] = b
                    cuts.add(base + r + len(b))
                cuts.add(base)
                data = buf
            self.slots.append(bytes(data) if data is not None else None)
            self.slot_base.append(base)
            self.slot_cuts.append(cuts)
        # extra slots for the shared data (btlfx_getFileData 0x40+ / 0x80+ / 0xC0+)
        self.ring_slot = None   # 0x40-0x7F: RAM_BTLFX_RING = the first ring-start data chunk (its common part)
        if self.common:
            self.ring_slot = len(self.slots)
            self.slots.append(bytes(self.common))
        elif ring_group is not None:
            self.ring_slot = ring_group
        self.summon_slots = []  # [(group, slot)]: 0x80-0xBF = summon->data[0] = the type 6 chunk (SUMMON_DATA)
        for gi, raw in summon_raw:
            self.summon_slots.append((gi, len(self.slots)))
            self.slots.append(bytes(raw))
        self.loadbuf_slot = None  # 0xC0+: the battle load buffer, which a type 8 chunk fills
        if loadbuf is not None:
            self.loadbuf_slot = len(self.slots)
            self.slots.append(bytes(loadbuf))
        self.slot_base += [0] * (len(self.slots) - len(self.slot_base))
        self.slot_cuts += [set()] * (len(self.slots) - len(self.slot_cuts))
        self.script = self._script()

    def summon_slot(self, group):
        """Slot of the summon data a group's module sees (the last type 6 chunk loaded up to that group)."""
        s = [slot for gi, slot in self.summon_slots if gi <= group]
        return s[-1] if s else None

    def _summon_tex(self, raw):
        """BTLFX_LDCMD_SUMMON_TEX: texCount 64x128 pages at the header's tpage / y, CLUT rows at (0x100, 0xE6)."""
        data_ofs, motions, ntex, clut_rows = struct.unpack_from('<hhhH', raw, 0)
        clut_ofs = s32(raw, 8)
        tp = struct.unpack_from('<6H', raw, 0x18)
        ty = struct.unpack_from('<6H', raw, 0x30)
        self.summon_hdr = {'dataOfs': data_ofs, 'motions': motions, 'textures': ntex, 'clutRows': clut_rows,
                           'tpages': list(tp[:max(0, min(ntex, 6))])}
        for i in range(max(0, min(ntex, 6))):
            src = data_ofs + i * 0x4000
            if src + 0x4000 <= len(raw):
                self.vram.load((tp[i] & 0xF) << 6, ty[i] + ((tp[i] & 0x10) << 4), 64, 128, raw, src)
        src = data_ofs + clut_ofs
        if 0 < clut_rows <= 26 and src + clut_rows * 0x200 <= len(raw):
            self.vram.load(0x100, 0xE6, 0x100, clut_rows, raw, src)

    def _common_tex(self, raw, mask):
        """BTLFX_LDCMD_COMMON_TEX: mask bits 0/1 = two pages each at (0x300 / 0x340, 0x100), bits 2-5 one page each
        at (0x140, 0x100), (0x140, 0x180), (0x180, 0x100), (0x180, 0x180)."""
        bit, off = 1, 0
        for i in range(8):
            if bit & mask:
                x = ((i >> 1) << 6) + 0x300 if i < 4 else (((i - 4) >> 1) << 6) + 0x140
                y = ((i & 1) << 7) + 0x100
                if off + 0x4000 <= len(raw):
                    self.vram.load(x, y, 64, 128, raw, off)
                    self.common_tex.append((x, y))
                off += 0x4000
            if i >= 4 or (i & 1):
                bit <<= 1

    def _pages(self, raw, off, count, rects, tex_index):
        for _ in range(count):
            if not rects:
                break
            r = rects[0]
            tex_index += 1
            if tex_index >= 2:
                x, y = r[0], r[1] + 0x80
                tex_index = 0
                rects = rects[1:]
            else:
                x, y = r[0], r[1]
                if r[3] == 0x80:
                    tex_index = 0
                    rects = rects[1:]
            if off + 0x4000 <= len(raw):
                self.vram.load(x, y, 64, 128, raw, off)
            off += 0x4000
        return rects, tex_index

    def _script(self):
        ops = []
        for p in range(SCRIPT_OFS, SECTOR - 2, 3):
            op, a, b = self.header[p:p + 3]
            ops.append((p, op, a, b))
            if op == 0 and a == 0:
                break
        return ops

    def entries(self, slot=0):
        """[(index, offset, size)] of a slot's data table."""
        d = self.slots[slot]
        if not d:
            return []
        base = self.slot_base[slot]
        first = u32(d, base)
        if first == 0 or first % 4 or base + first > len(d):
            return []
        # buffer offsets: the table's offsets are relative to the data start (negative = type 10 data below it)
        offs = [base + (s32(d, base + i * 4) if base else u32(d, base + i * 4)) for i in range(first // 4)]
        end = len(d)
        # the table's last offset is usually the end of the data; trailing 0xFF / 0 fill is not data
        bounds = sorted(set(o for o in offs if o <= end) | {end} | self.slot_cuts[slot])
        out = []
        for i, o in enumerate(offs):
            if o > end:
                out.append((i, o, 0))
                continue
            nxt = next(b for b in bounds if b > o) if o < end else end
            out.append((i, o, nxt - o))
        return out


def load_common_vram(img):
    v = Vram()
    d = img.dirs[DIR_COMMON_IDX]
    for fid, sec, size in img.files(d):
        if fid == FILE_BTLFX_COMMON:
            raw = img.read(sec * SECTOR, size)
            for i in range(4):
                v.load((i >> 1) * 64 + 0x300, (i & 1) * 128 + 0x100, 64, 128, raw, i * 0x4000)
            if len(raw) >= 0x10000 + 256 * 2 * 2:
                v.load(0, 0xF0, 256, 2, raw, 0x10000)
            return v
    return v


# ---------------------------------------------------------------------------------------------------------------
# content checks


EMIT_SIZE = {0xFF: 0x10}


def parse_emitter(d, o, size):
    """Emitter description at d[o:o+size] (btlfx_spawnEmitter's walk) or None."""
    if size < 0x28 + 0x14:
        return None
    k = d[o]
    if k not in (0, 1, 2, 3, 4, 5, 0xFF):
        return None
    p = o + (0x10 if k == 0xFF else 0x28)
    sub = d[o + 1]
    if sub not in (0, 1):
        return None
    if sub:
        p += 0x38
        if d[p] not in (0, 1, 2, 3, 4, 5, 0xFF):
            return None
        p += 0x28
    if p + 0x14 > o + size:
        return None
    wc, nel = s32(d, p), s16(d, p + 4)
    if not (1 <= nel <= 16) or not (0 <= wc <= 16):
        return None
    hdr = p
    p += 0x14
    if p + nel * 0x28 + 4 > o + size:
        return None
    elems = []
    for i in range(nel):
        e = p + i * 0x28
        kind, kind2, blend, flags = d[e:e + 4]
        if kind > 9 or (kind2 > 9 and kind2 != 0xFF):
            return None
        tex, tex2 = s32(d, e + 0x1C), s32(d, e + 0x20)
        for kk, t in ((kind, tex), (kind2, tex2)):
            if kk in (9, 0xFF) or t == -1 or t & 0x80000000:
                continue
            if not (0 <= t < size):
                return None
        elems.append({'kind': kind, 'kind2': kind2, 'blend': blend, 'flags': flags,
                      'frameMode': s16(d, e + 4), 'frameCount': s16(d, e + 8),
                      'tex': tex, 'tex2': tex2})
    tail = p + nel * 0x28
    shape = s32(d, tail)
    if shape not in (-1, 0, 1, 2, 3, 4):
        return None
    dur = s16(d, tail + 4) if tail + 6 <= o + size else None
    return {'motion': k, 'sub': bool(sub), 'elems': elems, 'shape': shape,
            'scale': (u16(d, hdr + 6), u16(d, hdr + 8)), 'alpha': (u16(d, hdr + 10), u16(d, hdr + 12)),
            'life': (s16(d, o + 2), s16(d, o + 4)), 'duration': dur}


MOTION = {0: 'ballistic', 1: 'orbit', 2: 'spiral', 3: 'wobble', 4: 'helix', 5: 'homing', 0xFF: 'module'}
SHAPE = {-1: 'module', 0: 'sphere', 1: 'cylinder', 2: 'axis', 3: 'tube', 4: 'cone'}
ELEMK = {0: 'particles', 1: 'sprite', 2: 'animquads', 3: 'sprites', 4: 'trail3d', 5: 'trailflat', 6: 'rings',
         7: 'fan', 8: 'trailscreen', 9: 'callback', 0xFF: 'none'}


def check_model(d, o, size):
    """BtlModelHeader at d[o] with offsets inside the entry: {bones, meshes, ...} or None."""
    if size < 0x18:
        return None
    flags, nb, nm, packet = struct.unpack_from('<HBBH', d, o)
    bofs, mofs = s32(d, o + 0xC), s32(d, o + 0x10)
    if flags != 0 or not (1 <= nb <= 64) or not (1 <= nm <= 16):
        return None
    if not (0x14 <= bofs < size and 0x14 <= mofs < size and mofs + nm * 0x28 <= size):
        return None
    meshes = []
    for i in range(nm):
        m = o + mofs + i * 0x28
        counts = struct.unpack_from('<8H', d, m + 2)
        ptrs = struct.unpack_from('<5I', d, m + 0x14)
        if any(p > size for p in ptrs):
            return None
        meshes.append({'counts': counts, 'ptrs': ptrs})
    return {'bones': nb, 'meshes': meshes, 'bones_ofs': bofs, 'height': u16(d, o + 8), 'radius': u16(d, o + 10)}


PRIMS = [('FT4', 0x18, 4, 0, 8, 0x13), ('FT3', 0x14, 3, 0, 0xC, 0x6), ('GT4', 0x20, 4, 0, 8, 0x1D),
         ('GT3', 0x18, 3, 0, 6, 0x16), ('G4', 0x18, 4, 0, None, None), ('G3', 0x14, 3, 0, None, None),
         ('F4', 0x10, 4, 0, None, None), ('F3', 0xC, 3, 0, None, None)]


# per primitive kind: colour field offset and whether it holds rgb directly (flat) or colour-table indices (gouraud)
PRIM_COLOR = {'FT4': (0x10, 'rgb'), 'FT3': (8, 'rgb'), 'GT4': (0x10, 'idx'), 'GT3': (0x10, 'idx'), 'G4': (8, 'idx'),
              'G3': (8, 'idx'), 'F4': (8, 'rgb'), 'F3': (8, 'rgb')}


def model_polys(d, o, m):
    """(vertices, [{'name', 'v': [vi], 'uv': [(u, v)], 'tex': texture index or None, 'col': [(r, g, b)]}]): the
    model's polygons with what btl_applyModelTextures / btl_drawMesh read (WmSrc* layouts, include/ovl_0c.h)."""
    nb = m['bones']
    bone_z = [0] * nb
    for b in range(nb):
        length, _, parent = struct.unpack_from('<hBB', d, o + m['bones_ofs'] + b * 4)
        bone_z[b] = (bone_z[parent] if b and parent < b else 0) + length
    verts, polys = [], []
    for mesh in m['meshes']:
        vc_o, v_o, p_o, uv_o, col_o = (o + x for x in mesh['ptrs'])
        base = len(verts)
        counts = struct.unpack_from(f'<{nb}H', d, vc_o)
        k = 0
        for b, n in enumerate(counts):
            for _ in range(n):
                x, y, z = struct.unpack_from('<3h', d, v_o + k * 8)
                verts.append((x, y, z + bone_z[b]))
                k += 1
        p = p_o
        for (name, sz, nv, vf, uf, tf), n in zip(PRIMS, mesh['counts']):
            co, ck = PRIM_COLOR[name]
            for _ in range(n):
                vi = [base + i for i in struct.unpack_from(f'<{nv}H', d, p + vf)]
                uvs = []
                if uf is not None:
                    for ui in struct.unpack_from(f'<{nv}H', d, p + uf):
                        uvs.append(tuple(d[uv_o + ui * 2:uv_o + ui * 2 + 2]))
                if ck == 'rgb':
                    cols = [tuple(d[p + co:p + co + 3])] * nv
                else:
                    cols = [tuple(d[col_o + ci * 4:col_o + ci * 4 + 3])
                            for ci in struct.unpack_from(f'<{nv}H', d, p + co)]
                polys.append({'name': name, 'v': vi, 'uv': uvs, 'tex': d[p + tf] if tf is not None else None,
                              'col': cols})
                p += sz
    return verts, polys


def parse_somodel(d, o, size):
    """'so' effect model (btlfx_registerEffectModel, Unk800E8F50): {textured, tex: [(tpage, clut, du, dv)],
    model_ofs, model} or None. Texture i is what a polygon's texture byte selects; (du, dv) is added to its uvs."""
    if not check_somodel(d, o, size):
        return None
    textured, mofs, pofs = u16(d, o + 2), u16(d, o + 4), u16(d, o + 6)
    if not (8 <= mofs < size):
        return None
    ntex = max(0, (mofs - 8) >> 3)
    tex = []
    for i in range(min(ntex, 16)):
        tp, clut = u16(d, o + 8 + i * 4), u16(d, o + 10 + i * 4)
        du, dv = (s16(d, o + pofs + i * 4), s16(d, o + pofs + i * 4 + 2)) if pofs + i * 4 + 4 <= size else (0, 0)
        tex.append((tp, clut, du, dv))
    mdl = check_model(d, o + mofs, size - mofs)
    return {'textured': bool(textured), 'tex': tex, 'model_ofs': mofs, 'model': mdl}


def check_akao(d, o, size):
    return size >= 4 and d[o:o + 4] == b'AKAO'


def check_somodel(d, o, size):
    return size >= 12 and d[o:o + 2] == b'so'


# ---------------------------------------------------------------------------------------------------------------
# sprite data rendering (formats: include/ovl_0b.h Unk800D1BC4, Unk800D1D74Hdr, Unk800D1494, Unk800D23D8Hdr)


def sprite_info(d, t, kind):
    """{'kind', 'frames', 'tex': [(tpage, clut)], 'uvbox'} summary of sprite data at d[t] for an element kind."""
    try:
        if kind in (0, 1):
            nf = u16(d, t) & 0x7FFF
            clut = u16(d, t + 2)
            pages = set()
            for f in range(min(nf, 64)):
                p = t + u16(d, t + 6 + f * 2)
                w = u16(d, p)
                p += 2 + (4 if w & 0x8000 else 0) + (4 if w & 0x4000 else 0)
                for r in range(min(w & 0x3FFF, 64)):
                    pages.add((u16(d, p + r * 10 + 8), clut))
            return {'frames': nf, 'tex': sorted(pages)}
        if kind in (2, 3):
            nf = u16(d, t) & 0x7FFF
            out = {'frames': nf, 'tex': [(u16(d, t + 2), u16(d, t + 4))], 'size': (d[t + 7], d[t + 6])}
            if kind == 3 and u16(d, t + 2) == 0:
                out['dots'] = True  # btlfx_drawSprites mode 0: untextured 1-pixel dots
            return out
        if kind in (4, 5, 8):
            return {'frames': u16(d, t), 'tex': [(u16(d, t + 4), u16(d, t + 6))], 'size': (d[t + 9], d[t + 8])}
        if kind == 6:
            return rings_info(d, t)
        if kind == 7:
            return fan_info(d, t)
    except struct.error:
        return None
    return {}


def fan_info(d, t):
    """btlfx_drawFan data (Unk800D2938 {frameCount, otZ, gouraud, pointCount} + 12-byte part + per-point colours
    (4 bytes, gouraud: 8 = inner / outer) + per-frame scale (vz, vx) + per-frame uv (textured))."""
    nf, otz, gour, npt = u16(d, t), s16(d, t + 2), u16(d, t + 4), u16(d, t + 6)
    out = {'frames': nf, 'points': npt, 'gouraud': bool(gour)}
    b = t + 8 + 0xC + npt * (8 if gour else 4)
    out['scale'] = [(u16(d, b + f * 4 + 2), u16(d, b + f * 4)) for f in range(min(nf, 64))]  # (x, z)
    if gour:
        out['shape'] = [s16(d, t + 8 + i * 2) * 4 for i in range(5)]
        out['colors'] = [(tuple(d[t + 0x14 + i * 8:t + 0x17 + i * 8]), tuple(d[t + 0x18 + i * 8:t + 0x1B + i * 8]))
                         for i in range(min(npt, 16))]
    else:
        out['tex'] = [(u16(d, t + 8), u16(d, t + 10))]
        out['size'] = (u16(d, t + 16), u16(d, t + 18))
        out['quad'] = (u16(d, t + 14) * 2, u16(d, t + 12) * 4)
        out['colors'] = [tuple(d[t + 0x14 + i * 4:t + 0x17 + i * 4]) for i in range(min(npt, 16))]
        uvb = b + nf * 4
        out['uv'] = [(d[uvb + f * 2], d[uvb + f * 2 + 1]) for f in range(min(nf, 64))]
    return out


def render_fan(vram, d, t, max_frames=8):
    """Frames of a fan: textured = the frame's texture rect tinted with its point colour, stretched to the frame's
    scale; gouraud = the three triangles of btlfx_emitFrameFan in the x/z plane with their colours."""
    fi = fan_info(d, t)
    nf = fi['frames']
    if not nf or nf > 512:
        return None
    picks = sorted(set(int(i * nf / max_frames) for i in range(max_frames))) if nf > max_frames else range(nf)
    frames, miss = [], 0
    npt = max(1, fi['points'])
    for f in picks:
        pi = f * (npt - 1) // (nf - 1) if nf > 1 else 0
        sx, sz = fi['scale'][min(f, len(fi['scale']) - 1)] if fi['scale'] else (0x1000, 0x1000)
        if not fi['gouraud']:
            tp, clut = fi['tex'][0]
            w, h = fi['size']
            u, v = fi['uv'][min(f, len(fi['uv']) - 1)]
            col = fi['colors'][min(pi, len(fi['colors']) - 1)] if fi['colors'] else (128, 128, 128)
            c, m = tex_rect(vram, tp, clut, u, v, w, h, col)
            miss += m
            frames.append(c)
            continue
        p0, p1, p2, p3, p4 = fi['shape']
        inner, outer = fi['colors'][min(pi, len(fi['colors']) - 1)]
        # btlfx_emitFrameFan: (x, z) corners and colours of its three triangles (ot z offset left out)
        A, B, Cc, D = (0, 0), (-p1, p2), (-p3, p4), (p1, p0)
        tris = [((A, (0, 0, 0)), (B, (0, 0, 0)), (Cc, outer)), ((B, (0, 0, 0)), (D, (0, 0, 0)), (Cc, outer)),
                ((Cc, outer), (D, (0, 0, 0)), (A, inner))]
        tris = [tuple(((x * sx // 0x1000, z * sz // 0x1000), col) for (x, z), col in tri) for tri in tris]
        xs = [p[0][0] for tri in tris for p in tri]
        zs = [p[0][1] for tri in tris for p in tri]
        span = max(max(xs) - min(xs), max(zs) - min(zs), 1)
        S = 96
        c = Canvas(S, S)
        k = (S - 5) / span
        for tri in tris:
            P = [((x - min(xs)) * k + 2, (S - 3) - (z - min(zs)) * k, col) for (x, z), col in tri]
            (ax_, ay_, ca), (bx, by, cb), (cx, cy, cc) = P
            den = (by - cy) * (ax_ - cx) + (cx - bx) * (ay_ - cy)
            if abs(den) < 1e-6:
                continue
            for yy in range(S):
                for xx in range(S):
                    w0 = ((by - cy) * (xx - cx) + (cx - bx) * (yy - cy)) / den
                    w1 = ((cy - ay_) * (xx - cx) + (ax_ - cx) * (yy - cy)) / den
                    w2 = 1 - w0 - w1
                    if min(w0, w1, w2) < -0.01:
                        continue
                    c.put(xx, yy, tuple(max(0, min(255, int(2 * (ca[q] * w0 + cb[q] * w1 + cc[q] * w2))))
                                        for q in range(3)))
        frames.append(c)
    W = sum(c.w for c in frames) + 4 * (len(frames) - 1)
    out = Canvas(W, max(c.h for c in frames), (24, 24, 24))
    x = 0
    for c in frames:
        out.blit(c, x, 0)
        x += c.w + 4
    return out, nf, miss


def rings_info(d, t):
    nf, flags, typ, cnt, w = u16(d, t), d[t + 4], d[t + 5], d[t + 6], d[t + 7]
    s1 = t + 8 + (nf * 4 if flags & 2 else 0)
    ncol = u16(d, s1 + 2)
    out = {'frames': nf, 'rings': cnt, 'segments': w, 'type': typ}
    if typ == 1:
        fd = s1 + (ncol + 1) * 4
        k = u16(d, fd)
        out['tex'] = [(u16(d, fd + 4), u16(d, fd + 6))]
        uvs = [u16(d, fd + 8 + i * 2) for i in range((cnt + 1) * (k + 2))]
        us, vs = [x & 0xFF for x in uvs], [x >> 8 for x in uvs]
        out['uvbox'] = (min(us), min(vs), max(us) + 1, max(vs) + 1)
        out['colors'] = [d[s1 + 4 + i * 4:s1 + 7 + i * 4] for i in range(min(ncol, 8))]
    else:
        m2 = 2 if u16(d, s1) else 1
        out['colors'] = [d[s1 + 4 + i * 4:s1 + 7 + i * 4] for i in range(min(ncol * m2 * (cnt + 1), 16))]
    return out


def render_frame(vram, d, t, kind, f):
    """Canvas of frame f of sprite data (kinds 0-3), at 1 px = 4 units (3D) / 1 screen px; (canvas, missing)."""
    sprites = []  # (x, y, w, h, tpage, clut, u0, v0, u1, v1, tint)
    if kind in (0, 1):
        nf = u16(d, t) & 0x7FFF
        clut = u16(d, t + 2)
        f = min(f, nf - 1)
        p = t + u16(d, t + 6 + f * 2)
        w = u16(d, p)
        p += 2
        tint = (128, 128, 128)
        if w & 0x8000:
            tint = tuple(d[p:p + 3])
            p += 4
        if w & 0x4000:
            p += 4
        for r in range(min(w & 0x3FFF, 256)):
            q = p + r * 10
            x, y = struct.unpack_from('<bb', d, q)
            sw, sh = d[q + 2], d[q + 3]
            uv0, uv1, tp = u16(d, q + 4), u16(d, q + 6), u16(d, q + 8)
            if kind == 0:
                u0, v0, u1, v1 = uv0 & 0xFF, uv0 >> 8, uv1 & 0xFF, uv1 >> 8
                sprites.append((x * 4, y * 4, sw, sh, tp, clut, u0, v0, u1, v1, tint))
            else:
                u0, v0 = uv0 & 0xFF, uv0 >> 8
                sprites.append((x * 4, y * 4, sw, sh, tp, clut, u0, v0, u0 + sw, v0 + sh, tint))
    elif kind in (2, 3):
        nf = u16(d, t) & 0x7FFF
        tp, clut, hh, ww = u16(d, t + 2), u16(d, t + 4), d[t + 6], d[t + 7]
        f = f % nf
        pal = t + 8 + nf * 2
        cnt = u16(d, pal)
        uvtab = pal + 2
        coltab = uvtab + (cnt + 1) * 2
        p = t + u16(d, t + 8 + f * 2)
        n = d[p]
        p += 1
        for r in range(n):
            x, y = struct.unpack_from('<bb', d, p + r * 3)
            b = d[p + r * 3 + 2]
            uv = u16(d, uvtab + (b & 0xF) * 2)
            tint = tuple(d[coltab + (b >> 4) * 4:coltab + (b >> 4) * 4 + 3])
            u0, v0 = uv & 0xFF, uv >> 8
            if kind == 3 and tp == 0:  # btlfx_drawSprites mode 0: untextured 1-pixel dots (TILE_1, btlfx_emitDots)
                sprites.append((x, y, 2, 2, None, None, 0, 0, 0, 0, tint))
                continue
            sprites.append((x - ww // 2, y - hh // 2, ww, hh, tp, clut, u0, v0, u0 + ww, v0 + hh, tint))
    if not sprites:
        return None, 0
    x0 = min(s[0] for s in sprites)
    y0 = min(s[1] for s in sprites)
    x1 = max(s[0] + s[2] for s in sprites)
    y1 = max(s[1] + s[3] for s in sprites)
    W, H = max(1, min(x1 - x0, 512)), max(1, min(y1 - y0, 512))
    c = Canvas(W, H)
    miss = 0
    for (x, y, sw, sh, tp, clut, u0, v0, u1, v1, tint) in sprites:
        if tp is None:
            for j in range(sh):
                for i in range(sw):
                    c.put(x - x0 + i, y - y0 + j, tuple(min(255, 2 * t) for t in tint))
            continue
        for j in range(sh):
            for i in range(sw):
                uu = u0 + ((u1 - u0) * i // sw if sw else 0)
                vv = v0 + ((v1 - v0) * j // sh if sh else 0)
                tx = vram.texel(tp, clut, uu, vv)
                if tx == 'missing':
                    miss += 1
                    tx = (255, 0, 255) if ((i // 4 + j // 4) & 1) else (96, 0, 96)
                    c.put(x - x0 + i, y - y0 + j, tx, add=False)
                    continue
                if tx is None:
                    continue
                c.put(x - x0 + i, y - y0 + j, tuple(min(255, tx[k] * tint[k] // 128) for k in range(3)))
    return c, miss


def render_sprite_data(vram, d, t, kind, max_frames=8):
    """A strip of up to max_frames frames (evenly picked); (canvas, frames, missing) or None."""
    info = sprite_info(d, t, kind) or {}
    if kind in (0, 1, 2, 3):
        nf = info.get('frames', 0)
        if not nf:
            return None
        picks = sorted(set(int(i * nf / max_frames) for i in range(max_frames))) if nf > max_frames else range(nf)
        frames, miss = [], 0
        for f in picks:
            c, m = render_frame(vram, d, t, kind, f)
            if c:
                frames.append(c)
                miss += m
        if not frames:
            return None
        W = sum(c.w for c in frames) + 4 * (len(frames) - 1)
        H = max(c.h for c in frames)
        out = Canvas(W, H, (24, 24, 24))
        x = 0
        for c in frames:
            out.blit(c, x, 0)
            x += c.w + 4
        return out, nf, miss
    if kind in (4, 5, 8):
        nf, tp, clut, hh, ww, k = u16(d, t), u16(d, t + 4), u16(d, t + 6), d[t + 8], d[t + 9], u16(d, t + 10)
        base = t + 12
        rects = []
        for f in range(min(nf, 4)):
            q = base + k * 8 + f * 4
            rects.append((d[q + 1], d[q]))
        cs = [tex_rect(vram, tp, clut, u, v, ww, hh) for (u, v) in rects]
        out = Canvas(sum(c.w for c, _ in cs) + 4 * (len(cs) - 1), max(c.h for c, _ in cs), (24, 24, 24))
        x = 0
        for c, _ in cs:
            out.blit(c, x, 0)
            x += c.w + 4
        return out, nf, sum(m for _, m in cs)
    if kind == 'line':
        # Unk800D3B6C: 0x18-byte header, count 12-byte entries {s32 spriteOfs, ...}: screen sprites (kind 1)
        cnt = u16(d, t + 2)
        ofs = sorted({s32(d, t + 0x18 + i * 12) for i in range(min(cnt, 32))})
        parts = [render_sprite_data(vram, d, t + o, 1, 4) for o in ofs]
        parts = [p for p in parts if p]
        if not parts:
            return None
        out = Canvas(max(p[0].w for p in parts), sum(p[0].h + 4 for p in parts), (24, 24, 24))
        y = 0
        for c, _, _ in parts:
            out.blit(c, 0, y)
            y += c.h + 4
        return out, cnt, sum(p[2] for p in parts)
    if kind == 7:
        return render_fan(vram, d, t, max_frames)
    if kind == 6:
        ri = rings_info(d, t)
        if 'uvbox' in ri:
            tp, clut = ri['tex'][0]
            u0, v0, u1, v1 = ri['uvbox']
            tint = tuple(ri['colors'][0]) if ri.get('colors') else (128, 128, 128)
            c, m = tex_rect(vram, tp, clut, u0, v0, u1 - u0, v1 - v0, tint)
            return c, ri['frames'], m
        cols = ri.get('colors') or []
        if not cols:
            return None
        c = Canvas(16 * len(cols), 16)
        for i, col in enumerate(cols):
            for y in range(16):
                for x in range(16):
                    c.put(i * 16 + x, y, tuple(min(255, v * 2) for v in col), add=False)
        return c, ri['frames'], 0
    return None


def project(verts, ax, S=160):
    """Screen points (x, y, depth) of the vertices fitted into S x S. ax: 'front' (x, y), 'top' (x, z), 'side'
    (z, y), 'oblique' (turned 35 degrees about y, tilted 25 degrees)."""
    import math
    if ax == 'oblique':
        ca, sa, cb, sb = math.cos(0.61), math.sin(0.61), math.cos(0.44), math.sin(0.44)
        pv = []
        for x, y, z in verts:
            x, z = x * ca + z * sa, -x * sa + z * ca
            y, z = y * cb - z * sb, y * sb + z * cb
            pv.append((x, y, z))
    else:
        i, j, k = {'front': (0, 1, 2), 'top': (0, 2, 1), 'side': (2, 1, 0)}[ax]
        pv = [(v[i], v[j], v[k] if ax != 'top' else -v[k]) for v in verts]
    xs, ys = [v[0] for v in pv], [v[1] for v in pv]
    span = max(max(xs) - min(xs), max(ys) - min(ys), 1)
    sc = (S - 9) / span
    ox = (S - (max(xs) - min(xs)) * sc) / 2 - min(xs) * sc
    oy = (S - (max(ys) - min(ys)) * sc) / 2 - min(ys) * sc
    return [(v[0] * sc + ox, v[1] * sc + oy, v[2]) for v in pv]


def raster_view(vram, verts, polys, texfn, ax, S=160, bg=(16, 16, 32)):
    """Painter's-order textured / gouraud render of the polygons (see project for ax); (canvas, missing texels)."""
    pts = project(verts, ax, S)
    c = Canvas(S, S, bg)
    miss = 0
    order = []
    for p in polys:
        if any(i >= len(pts) for i in p['v']):
            continue
        order.append((-sum(pts[i][2] for i in p['v']) / len(p['v']), p))
    order.sort(key=lambda t: t[0])
    for _, p in order:
        a, b, cc = (verts[i] for i in p['v'][:3])
        ux, uy, uz = b[0] - a[0], b[1] - a[1], b[2] - a[2]
        vx, vy, vz = cc[0] - a[0], cc[1] - a[1], cc[2] - a[2]
        nx, ny, nz = uy * vz - uz * vy, uz * vx - ux * vz, ux * vy - uy * vx
        nl = (nx * nx + ny * ny + nz * nz) ** 0.5 or 1
        shade = 0.45 + 0.55 * abs(nx * 0.4 - ny * 0.7 + nz * 0.6) / nl / 1.0099  # flat light from up-front-right
        n = len(p['v'])
        tris = [(0, 1, 2)] if n == 3 else [(0, 1, 2), (1, 3, 2)]
        tx = texfn(p) if p['uv'] else None
        for tri in tris:
            P = [pts[p['v'][t]] for t in tri]
            UV = [p['uv'][t] for t in tri] if tx else None
            C = [p['col'][t] for t in tri]
            x0, x1 = max(0, int(min(q[0] for q in P))), min(S - 1, int(max(q[0] for q in P)) + 1)
            y0, y1 = max(0, int(min(q[1] for q in P))), min(S - 1, int(max(q[1] for q in P)) + 1)
            (ax_, ay_, _), (bx, by, _), (cx, cy, _) = P
            den = (by - cy) * (ax_ - cx) + (cx - bx) * (ay_ - cy)
            if abs(den) < 1e-6:
                continue
            for yy in range(y0, y1 + 1):
                for xx in range(x0, x1 + 1):
                    px, py = xx + 0.5, yy + 0.5
                    w0 = ((by - cy) * (px - cx) + (cx - bx) * (py - cy)) / den
                    w1 = ((cy - ay_) * (px - cx) + (ax_ - cx) * (py - cy)) / den
                    w2 = 1 - w0 - w1
                    if w0 < -0.01 or w1 < -0.01 or w2 < -0.01:
                        continue
                    col = tuple(max(0, min(255, int(C[0][q] * w0 + C[1][q] * w1 + C[2][q] * w2))) for q in range(3))
                    if tx:
                        tp, clut, du, dv = tx
                        u = int(UV[0][0] * w0 + UV[1][0] * w1 + UV[2][0] * w2) + du
                        v = int(UV[0][1] * w0 + UV[1][1] * w1 + UV[2][1] * w2) + dv
                        t = vram.texel(tp, clut, u, v)
                        if t == 'missing':
                            miss += 1
                            c.put(xx, yy, (255, 0, 255) if (xx // 4 + yy // 4) & 1 else (96, 0, 96), add=False)
                            continue
                        if t is None:
                            continue
                        col = tuple(min(255, t[q] * col[q] // 128) for q in range(3)) if any(col) else t
                    c.put(xx, yy, tuple(int(q * shade) for q in col), add=False)
    return c, miss


def render_textured_model(vram, verts, polys, texfn, uvboxes=()):
    """Front / top / oblique renders and an oblique wireframe, then the texture boxes [(tpage, clut, u0, v0, w, h)]."""
    views, miss = [], 0
    for ax in ('front', 'top', 'oblique'):
        c, m = raster_view(vram, verts, polys, texfn, ax)
        views.append(c)
        miss += m
    wire = Canvas(160, 160, (16, 16, 32))
    pts = project(verts, 'oblique')
    for p in polys:
        vi = p['v'] if len(p['v']) == 3 else [p['v'][0], p['v'][1], p['v'][3], p['v'][2]]
        for a, b in zip(vi, vi[1:] + vi[:1]):
            if a < len(pts) and b < len(pts):
                wire.line(int(pts[a][0]), int(pts[a][1]), int(pts[b][0]), int(pts[b][1]), (200, 200, 255))
    views.append(wire)
    for (tp, clut, u0, v0, w, h) in uvboxes:
        t, m = tex_rect(vram, tp, clut, u0, v0, w, h)
        views.append(scale_canvas(t, 2) if t.w <= 80 and t.h <= 80 else t)
    W = sum(v.w for v in views) + 4 * (len(views) - 1)
    out = Canvas(W, max(v.h for v in views), (24, 24, 24))
    x = 0
    for v in views:
        out.blit(v, x, 0)
        x += v.w + 4
    return out, miss


def model_info(verts, polys):
    xs, ys, zs = [v[0] for v in verts], [v[1] for v in verts], [v[2] for v in verts]
    return {'vertices': len(verts), 'polys': len(polys),
            'size': (max(xs) - min(xs), max(ys) - min(ys), max(zs) - min(zs))}


def render_model(vram, d, o, m, tex, texlist=None):
    """BtlModelHeader drawn with one texture (Hi_RegisterTexEffModel: tex = (tpage, clut, u, v) the C passes; else
    untextured): front / top / oblique renders, oblique wireframe and the UV box."""
    verts, polys = model_polys(d, o, m)
    if not verts:
        return None, {}
    info = model_info(verts, polys)
    uv = [x for p in polys for x in p['uv']]
    boxes = []
    if uv and tex:
        tp, clut, uo, vo = tex
        us, vs = [a[0] for a in uv], [a[1] for a in uv]
        box = (min(us), min(vs), max(us) + 1, max(vs) + 1)
        info['uvbox'] = box
        boxes.append((tp, clut, uo + box[0], vo + box[1], box[2] - box[0], box[3] - box[1]))
    if texlist:
        def texfn(p):
            return texlist[p['tex']] if p['tex'] is not None and p['tex'] < len(texlist) else None
        boxes = []
        for ti, (tp, cl, du, dv) in enumerate(texlist[:3]):
            tuv = [x for p in polys if p['tex'] == ti for x in p['uv']]
            if tuv:
                us, vs = [a[0] for a in tuv], [a[1] for a in tuv]
                boxes.append((tp, cl, du + min(us), dv + min(vs), max(us) + 1 - min(us), max(vs) + 1 - min(vs)))
    else:
        def texfn(p):
            return tex
    c, miss = render_textured_model(vram, verts, polys, texfn, boxes)
    info['tex_missing'] = miss
    return c, info


def render_somodel(vram, d, o, size):
    """'so' effect model: the model drawn with its texture list (texture byte -> (tpage, clut) + uv offset)."""
    so = parse_somodel(d, o, size)
    if not so or not so['model']:
        return None, {}
    mo = o + so['model_ofs']
    verts, polys = model_polys(d, mo, so['model'])
    if not verts:
        return None, {}
    info = model_info(verts, polys)
    info['textured'] = so['textured']
    info['tex'] = [f'{tp:#x}/{cl:#x}+({du},{dv})' for tp, cl, du, dv in so['tex']]
    texl = so['tex'] if so['textured'] else []

    def texfn(p):
        return texl[p['tex']] if p['tex'] is not None and p['tex'] < len(texl) else None
    boxes = []
    for ti, (tp, cl, du, dv) in enumerate(texl[:3]):
        uv = [x for p in polys if p['tex'] == ti for x in p['uv']]
        if uv:
            us, vs = [a[0] for a in uv], [a[1] for a in uv]
            boxes.append((tp, cl, du + min(us), dv + min(vs), max(us) + 1 - min(us), max(vs) + 1 - min(vs)))
    c, miss = render_textured_model(vram, verts, polys, texfn, boxes)
    info['tex_missing'] = miss
    return c, info


# ---------------------------------------------------------------------------------------------------------------
# C usage


GETFD = re.compile(r'getFileData\(\s*([^,()]+(?:\([^()]*\))?)\s*,\s*(0x[0-9A-Fa-f]+|\d+|[A-Z_][A-Z0-9_]*)\s*\)')
CALL = re.compile(r'(?:->|\b)([A-Za-z_][A-Za-z0-9_]*)\s*$')
CAST_CALL = re.compile(r'\(\s*\(\s*\w+\s*\*?\s*\)\s*(?:\w+->)?(\w+)\s*\)\s*$')


def enclosing_call(src, pos):
    """(name, start of its argument list) of the innermost call whose parentheses contain pos."""
    depth = 0
    i = pos - 1
    while i >= 0:
        ch = src[i]
        if ch == ')':
            depth += 1
        elif ch == '(':
            if depth == 0:
                m = CALL.search(src[max(0, i - 80):i])
                if not m:  # ((FnType)ex->name)(...): a call through a cast export
                    m = CAST_CALL.search(src[max(0, i - 120):i])
                if m and m.group(1) not in ('if', 'while', 'for', 'switch', 'return', 'sizeof'):
                    return m.group(1), i
                if not m:
                    pass  # a cast or grouping paren: keep going out
            else:
                depth -= 1
        elif ch in ';{}' and depth == 0:
            return None, None
        i -= 1
    return None, None


def call_args(src, open_paren):
    """Top-level argument strings of the call whose '(' is at open_paren."""
    depth, args, cur = 0, [], ''
    for ch in src[open_paren + 1:]:
        if ch in '([{':
            depth += 1
        elif ch in ')]}':
            if depth == 0:
                args.append(cur.strip())
                return args
            depth -= 1
        if ch == ',' and depth == 0:
            args.append(cur.strip())
            cur = ''
            continue
        cur += ch
    return args


def func_at(src, pos):
    best = None
    for m in re.finditer(r'^[A-Za-z_][\w \*]*?\b(\w+)\s*\([^;{]*\)\s*\{', src[:pos], re.M):
        best = m.group(1)
    return best


def defines(paths):
    out = {}
    for p in paths:
        if os.path.exists(p):
            for m in re.finditer(r'#define\s+(FX\w+)\s+(0x[0-9A-Fa-f]+|\d+)', open(p).read()):
                out[m.group(1)] = int(m.group(2), 0)
    return out


def c_usage(module):
    """{index: [{func, call, arg, args, var}]} from src/fxmod/<module>/*.c."""
    d = os.path.join(ROOT, 'src', 'fxmod', module)
    if not os.path.isdir(d):
        return {}
    names = defines([os.path.join(ROOT, 'include', 'fxmod', module + '.h'),
                     os.path.join(ROOT, 'include', 'btlfx_mod.h')])
    out = {}
    for fn in sorted(os.listdir(d)):
        if not fn.endswith('.c'):
            continue
        src = open(os.path.join(d, fn)).read()
        for m in GETFD.finditer(src):
            tok = m.group(2)
            idx = int(tok, 0) if tok[0].isdigit() else names.get(tok)
            if idx is None:
                continue
            name, op = enclosing_call(src, m.start())
            rec = {'func': func_at(src, m.start()), 'call': name, 'name': tok if not tok[0].isdigit() else None}
            if name and op is not None:
                args = call_args(src, op)
                rec['args'] = args
                for k, a in enumerate(args):
                    if m.group(0) in a.replace('\n', ' ') or 'getFileData' in a:
                        rec['arg'] = k
                        break
            else:
                line = src[src.rfind('\n', 0, m.start()) + 1:src.find('\n', m.end())]
                vm = re.match(r'\s*(?:\w+[\s\*]+)?([\w\.\->\[\]]+)\s*=', line)
                rec['var'] = vm.group(1) if vm else line.strip()
                # follow the variable to the first export call that takes it
                if vm:
                    var = re.escape(vm.group(1))
                    for cm in re.finditer(r'->(\w+)\(', src[m.end():m.end() + 1500]):
                        op = m.end() + cm.end() - 1
                        args = call_args(src, op)
                        if any(re.fullmatch(r'(\([\w\s\*]+\))?\s*' + var, a) for a in args):
                            rec['call'] = cm.group(1)
                            rec['args'] = args
                            break
            out.setdefault(idx, []).append(rec)
    return out


def module_table(module, sym, count, fmt='<2h'):
    """count records of a module's data symbol, read from disks/us/fxmod/<module>.bin at its address
    (config/fxmod/<module>.syms.txt), or None."""
    sp = os.path.join(ROOT, 'config', 'fxmod', module + '.syms.txt')
    bp = os.path.join(ROOT, 'disks', 'us', 'fxmod', module + '.bin')
    if not (os.path.exists(sp) and os.path.exists(bp)):
        return None
    m = re.search(r'^' + re.escape(sym) + r'\s*=\s*(0x[0-9A-Fa-f]+)', open(sp).read(), re.M)
    vram = next((int(line.split()[1], 16) for line in open(os.path.join(ROOT, 'config', 'fxmods.us.txt'))
                 if line.split()[:1] == [module]), None)
    if not m or vram is None:
        return None
    img = open(bp, 'rb').read()
    o, sz = int(m.group(1), 16) - vram, struct.calcsize(fmt)
    if not (0 <= o and o + count * sz <= len(img)):
        return None
    return [struct.unpack_from(fmt, img, o + i * sz) for i in range(count)]


def texlist_from_use(module, uses):
    """[(tpage, clut, du, dv)] of a Hi_RegisterTexListModel call whose BtlTexInfo / offsets are module symbols."""
    for u in uses:
        a = u.get('args') or []
        if u.get('call') == 'Hi_RegisterTexListModel' and len(a) >= 3 and module:
            syms = [re.sub(r'^\(.*?\)\s*|^&', '', x).strip() for x in a[1:3]]
            pages = module_table(module, syms[0], 6, '<2H')
            ofs = module_table(module, syms[1], 6) if syms[1] != 'NULL' else [(0, 0)] * 6
            if pages and ofs:
                return [(tp, cl, du, dv) for (tp, cl), (du, dv) in zip(pages, ofs)]
    return None


USE_KIND = {
    'btlfx_startEmitter': 'emitter', 'Hi_RegisterTexEffModel': 'model', 'Hi_RegisterTexListEffModel': 'model',
    'Hi_RegisterTexListModel': 'model', 'Hi_RegisterTexPtrModel': 'model',
    'Hi_RegisterGouEffModel': 'model', 'Hi_RegisterSolidEffModel': 'model', 'btlfx_registerEffectModel': 'somodel',
    'btlfx_drawRings': 'rings', 'btlfx_drawTrail': 'trail', 'btlfx_drawSprite': 'sprite',
    'btlfx_drawSpriteAtOt': 'sprite', 'btlfx_drawParticles': 'particles', 'btlfx_drawAnimQuads': 'animquads',
    'btlfx_drawSprites': 'sprites', 'btlfx_drawFan': 'fan', 'btlfx_sampleGradient': 'gradient', 'btlfx_drawSpriteLine': 'spriteline',
}
DRAW_KIND = {'fan': 7, 'rings': 6, 'trail': 4, 'sprite': 1, 'particles': 0, 'animquads': 2, 'sprites': 3, 'spriteline': 'line'}


# ---------------------------------------------------------------------------------------------------------------
# per file


def script_refs(fx):
    """{index: [what]} named by the effect script, and the module ops it runs [(op, a, b)]."""
    refs, mods = {}, []
    for p, op, a, b in fx.script:
        if op == 0x23 and a != 0xFF:
            refs.setdefault(a, []).append('camera (script op 0x23)')
        elif op == 0x29 and b == 0:
            refs.setdefault(a, []).append('camera (script op 0x29)')
        elif op == 0x2D:
            refs.setdefault(a, []).append('sound (script op 0x2D)')
        elif op in (0x63, 0x64):
            refs.setdefault(b, []).append(f'party motion (script op {op:#x})')
        elif op == 0x0F and b == 0:
            refs.setdefault(a, []).append('vibration (script control op 0x0F)')
        elif op >= 0x80:
            mods.append((op, a, b))
    return refs, mods


def classify(fx, slot, idx, o, size, use, sref):
    d = fx.slots[slot]
    rec = {'index': idx, 'offset': o, 'size': size}
    calls = sorted({u['call'] for u in use if u.get('call')})
    kinds = sorted({USE_KIND[c] for c in calls if c in USE_KIND})
    kind = None
    if size == 0:
        kind = 'empty'
    elif idx == 0:
        kind = 'heap'
    elif check_akao(d, o, size):
        kind = 'sound'
    elif check_somodel(d, o, size):
        kind = 'somodel'
    em = parse_emitter(d, o, size) if size else None
    mdl = check_model(d, o, size) if size else None
    if kind is None:
        if 'emitter' in kinds and em:
            kind = 'emitter'
        elif 'model' in kinds and mdl:
            kind = 'model'
        elif kinds:
            kind = kinds[0]
        elif any('camera' in s for s in sref):
            kind = 'camera'
        elif any('motion' in s for s in sref):
            kind = 'motion'
        elif any('vibration' in s for s in sref):
            kind = 'vibration'
        elif em:
            kind = 'emitter?'
        elif mdl:
            kind = 'model?'
        else:
            kind = 'table'
    rec['kind'] = kind
    rec['uses'] = [{k: v for k, v in u.items() if k != 'args'} for u in use]
    rec['calls'] = calls
    rec['script'] = sref
    if kind.startswith('emitter') and em:
        rec['emitter'] = {
            'motion': MOTION.get(em['motion']), 'sub': em['sub'], 'shape': SHAPE.get(em['shape']),
            'life': em['life'], 'elems': []}
        for e in em['elems']:
            el = {'kind': ELEMK.get(e['kind']), 'kind2': ELEMK.get(e['kind2']), 'blend': e['blend'],
                  'frames': e['frameCount']}
            if e['kind'] != 9 and e['tex'] != -1:
                if e['tex'] & 0x80000000:
                    el['tex'] = f'{e["tex"] & 0xFFFFFFFF:#x} (absolute: common data)'
                else:
                    si = sprite_info(d, o + e['tex'], e['kind'])
                    el['sprite'] = jsonable(si)
            rec['emitter']['elems'].append(el)
    if kind.startswith('model') and mdl:
        rec['model'] = {'bones': mdl['bones'], 'meshes': len(mdl['meshes']),
                        'polys': sum(sum(m['counts']) for m in mdl['meshes'])}
        for u in use:
            a = u.get('args')
            if (u.get('call') or '').startswith('Hi_Register') and a and len(a) >= 5:
                rec['model']['tex'] = a[1:5]
    if kind == 'somodel':
        so = parse_somodel(d, o, size)
        if so:
            rec['somodel'] = {'textured': so['textured'],
                              'tex': [f'{tp:#x}/{cl:#x}+({du},{dv})' for tp, cl, du, dv in so['tex']]}
            if so['model']:
                rec['somodel']['polys'] = sum(sum(m['counts']) for m in so['model']['meshes'])
    if kind in DRAW_KIND:
        try:
            rec['sprite'] = jsonable(sprite_info(d, o, DRAW_KIND[kind]))
        except (struct.error, IndexError):
            rec['sprite'] = None
    return rec


def idx_label(idx):
    """Sheet / file label of a getFileData index: R = ring data (0x40+), S = summon data (0x80+), L = battle load
    buffer (0xC0+)."""
    return ('R' if idx < 0x80 else 'S' if idx < 0xC0 else 'L') + f'{idx:02X}' if idx >= 0x40 else f'{idx:02X}'


def jsonable(x):
    if isinstance(x, dict):
        return {k: jsonable(v) for k, v in x.items()}
    if isinstance(x, (list, tuple)):
        return [jsonable(v) for v in x]
    if isinstance(x, (bytes, bytearray)):
        return list(x)
    return x


def lit(s):
    try:
        return int(s, 0)
    except (ValueError, TypeError):
        return None


def render_entry(fx, slot, rec, outdir, sheet=None):
    """PNGs for one entry; returns the written file names. sheet: list that gets (label, canvas) per image."""
    d = fx.slots[slot]
    o, idx = rec['offset'], rec['index']
    written = []
    pre = os.path.join(outdir, f'{idx:02x}')
    if rec['kind'].startswith('emitter'):
        em = parse_emitter(d, o, rec['size'])
        for i, e in enumerate(em['elems'] if em else []):
            for which, kk, t in (('', e['kind'], e['tex']), ('b', e['kind2'], e['tex2'])):
                if kk in (9, 0xFF) or t == -1 or t & 0x80000000:
                    continue
                try:
                    r = render_sprite_data(fx.vram, d, o + t, kk)
                except (struct.error, IndexError, ZeroDivisionError):
                    r = None
                if r:
                    c, nf, miss = r
                    name = f'{pre}_e{i}{which}_{ELEMK[kk]}.png'
                    upscale(c).save(name)
                    written.append((os.path.basename(name), nf, miss))
                    if sheet is not None:
                        sheet.append((f'{idx_label(idx)} E{i}{which.upper()}', upscale(c, 64, 1100)))
    elif rec['kind'].startswith('model'):
        mdl = check_model(d, o, rec['size'])
        tex = None
        t = rec.get('model', {}).get('tex')
        if t and all(lit(x) is not None for x in t):
            tex = tuple(lit(x) for x in t)
        try:
            tl = rec.get('model', {}).get('texlist')
            c, info = render_model(fx.vram, d, o, mdl, tex, [tuple(t) for t in tl] if tl else None)
        except (struct.error, IndexError):
            c, info = None, {}
        if c:
            name = f'{pre}_model.png'
            c.save(name)
            written.append((os.path.basename(name), info.get('polys'), info.get('tex_missing', 0)))
            if sheet is not None:
                sheet.append((f'{idx_label(idx)} M', c))
            rec.setdefault('model', {}).update(jsonable(info))
    elif rec['kind'] == 'somodel':
        try:
            c, info = render_somodel(fx.vram, d, o, rec['size'])
        except (struct.error, IndexError):
            c, info = None, {}
        if c:
            name = f'{pre}_somodel.png'
            c.save(name)
            written.append((os.path.basename(name), info.get('polys'), info.get('tex_missing', 0)))
            if sheet is not None:
                sheet.append((f'{idx_label(idx)} M', c))
            rec.setdefault('somodel', {}).update(jsonable(info))
    elif rec['kind'] in DRAW_KIND:
        try:
            r = render_sprite_data(fx.vram, d, o, DRAW_KIND[rec['kind']])
        except (struct.error, IndexError, ZeroDivisionError):
            r = None
        if r:
            c, nf, miss = r
            name = f'{pre}_{rec["kind"]}.png'
            upscale(c).save(name)
            written.append((os.path.basename(name), nf, miss))
            if sheet is not None:
                sheet.append((f'{idx_label(idx)} D', upscale(c, 64, 1100)))
    rec['png'] = [w[0] for w in written]
    rec['png_missing_texels'] = sum(w[2] or 0 for w in written)
    return written


# 3x5 bitmap font for the contact sheet labels
GLYPH = {
    '0': [7, 5, 5, 5, 7], '1': [2, 6, 2, 2, 7], '2': [7, 1, 7, 4, 7], '3': [7, 1, 7, 1, 7], '4': [5, 5, 7, 1, 1],
    '5': [7, 4, 7, 1, 7], '6': [7, 4, 7, 5, 7], '7': [7, 1, 1, 1, 1], '8': [7, 5, 7, 5, 7], '9': [7, 5, 7, 1, 7],
    'A': [2, 5, 7, 5, 5], 'B': [6, 5, 6, 5, 6], 'C': [7, 4, 4, 4, 7], 'D': [6, 5, 5, 5, 6], 'E': [7, 4, 6, 4, 7],
    'F': [7, 4, 6, 4, 4], 'M': [5, 7, 7, 5, 5], 'S': [7, 4, 7, 1, 7], 'L': [4, 4, 4, 4, 7], 'R': [6, 5, 6, 5, 5],
    ' ': [0, 0, 0, 0, 0],
}


def draw_text(c, x, y, text, k=3, col=(255, 255, 0)):
    for ch in text:
        for j, row in enumerate(GLYPH.get(ch, GLYPH[' '])):
            for i in range(3):
                if row >> (2 - i) & 1:
                    for a in range(k):
                        for b in range(k):
                            c.put(x + i * k + a, y + j * k + b, col, add=False)
        x += 4 * k


def scale_down(c, h):
    k = (c.h + h - 1) // h
    out = Canvas(max(1, c.w // k), max(1, c.h // k))
    for y in range(out.h):
        for x in range(out.w):
            o = ((y * k) * c.w + x * k) * 3
            out.px[(y * out.w + x) * 3:(y * out.w + x) * 3 + 3] = c.px[o:o + 3]
    return out


def save_sheet(sheet, path):
    """Every image of the entries the module's C uses, one row each, labelled 'IDX E<elem>[B]' (emitter element,
    B = its second draw), 'IDX M' (model) or 'IDX D' (sprite data drawn directly)."""
    if not sheet:
        return
    LW = 100
    W = min(1300, LW + max(c.w for _, c in sheet))
    H = sum(min(c.h, 200) + 6 for _, c in sheet)
    out = Canvas(W, H, (40, 40, 40))
    y = 0
    for label, c in sheet:
        draw_text(out, 4, y + 4, label)
        out.blit(c if c.h <= 200 else scale_down(c, 200), LW, y)
        y += min(c.h, 200) + 6
    out.save(path)


# ---------------------------------------------------------------------------------------------------------------
# modules <-> file ids


def module_files():
    """{module: [file ids]} from config/fxmods.us.txt."""
    out = {}
    for line in open(os.path.join(ROOT, 'config', 'fxmods.us.txt')):
        if line.startswith('#') or not line.strip():
            continue
        name = line.split()[0]
        ids = [int(name[3:6], 16)]
        if '=' in line:
            ids += [int(a[3:6], 16) for a in line.split('=', 1)[1].split()]
        out[name] = ids
    return out


def labels():
    out = {}
    p = os.path.join(ROOT, 'docs', 'effect-modules.md')
    for line in open(p):
        if line.startswith('| fx_'):
            cols = [c.strip() for c in line.strip().strip('|').split('|')]
            out[cols[0]] = cols[-1]
    return out


def resolve(args):
    """[(module or None, file id, group)]."""
    mods = module_files()
    out = []
    for a in args:
        if a.startswith('fx_'):
            if a not in mods:
                sys.exit(f'unknown module {a}')
            grp = int(a.split('_')[2]) if a.count('_') == 2 else 0
            for fid in mods[a]:
                out.append((a, fid, grp))
        else:
            fid = int(a, 16) if not a.startswith('0x') else int(a, 0)
            mod = next((m for m, ids in mods.items() if fid in ids and m.count('_') == 1), None)
            if mod is None:
                mod = next((m for m, ids in mods.items() if fid in ids), None)
            out.append((mod, fid, 0))
    return out


def data_slots(fx, group, use):
    """[(slot, first index, what)]: the group's own data, then the shared data its module can index: the ring
    (0x40+, only when the C uses it), the summon data (0x80+) and the load buffer (0xC0+)."""
    out = [(group, 0, 'file data')]
    if fx.ring_slot is not None and any(0x40 <= i < 0x80 for i in use):
        out.append((fx.ring_slot, 0x40, 'ring data (RAM_BTLFX_RING' + (', common part)' if fx.common else ')')))
    s = fx.summon_slot(group)
    if s is not None:
        g = next(gi for gi, sl in fx.summon_slots if sl == s)
        out.append((s, 0x80, f'summon data (type 6 chunk of group {g}: g_btlfxLoader.summon->data[0])'))
    if fx.loadbuf_slot is not None:
        out.append((fx.loadbuf_slot, 0xC0, 'load buffer (type 8 chunk)'))
    return out


def analyse(img, common, module, fid, group, outdir, render):
    fx = FxFile(img, fid, common)
    use = c_usage(module) if module else {}
    sref, mods = script_refs(fx)
    if group >= len(fx.groups):
        group = 0
    recs, slot_of, parts = [], {}, []
    for slot, base, what in data_slots(fx, group, use):
        ents = fx.entries(slot)
        if base:
            ents = ents[:0x40]
        parts.append({'base': base, 'what': what, 'entries': len(ents)})
        for k, o, size in ents:
            idx = base + k
            r = classify(fx, slot, idx, o, size, use.get(idx, []), sref.get(idx, []))
            if r['kind'].startswith('model'):
                for u in use.get(idx, []):
                    if (u.get('call') or '').startswith('Hi_Register') and len(u.get('args', [])) >= 5:
                        r.setdefault('model', {})['tex'] = u['args'][1:5]
                tl = texlist_from_use(module, use.get(idx, []))
                if tl:
                    r.setdefault('model', {})['texlist'] = [list(t) for t in tl]
            recs.append(r)
            slot_of[idx] = slot
    fdir = os.path.join(outdir, f'{fid:03x}' + (f'_{group}' if group else ''))
    if render:
        os.makedirs(fdir, exist_ok=True)
        fx.vram.save(os.path.join(fdir, 'vram.png'))
        sheet = []
        for r in recs:
            render_entry(fx, slot_of[r['index']], r, fdir, sheet if r['uses'] else None)
        save_sheet(sheet, os.path.join(fdir, 'sheet.png'))
    used = sorted(set(use) - {r['index'] for r in recs})
    return {'module': module, 'file': fid, 'group': group, 'groups': len(fx.groups),
            'chunks': [[{k: v for k, v in c.items() if k in ('type', 'flag', 'sectors', 'extra', 'images', 'rects')}
                        for c in g['chunks']] for g in fx.groups],
            'entries': recs, 'parts': parts, 'module_ops': mods, 'indices_not_in_table': used,
            'common_tex': fx.common_tex, 'summon_tex': fx.summon_hdr,
            'script': [(op, a, b) for _, op, a, b in fx.script], 'pngdir': fdir if render else None}


def fmt_entry(r):
    bits = []
    if 'emitter' in r:
        e = r['emitter']
        els = ', '.join(f"{x['kind']}" + (f"+{x['kind2']}" if x['kind2'] not in ('none', None) else '') +
                        (f" {x['sprite'].get('frames')}f" if x.get('sprite') and x['sprite'].get('frames') else '')
                        for x in e['elems'])
        bits.append(f"{e['motion']}/{e['shape']}{' +sub' if e['sub'] else ''} [{els}]")
    if 'model' in r:
        m = r['model']
        bits.append(f"{m.get('polys')} polys" + (f" tex {','.join(m['tex'])}" if m.get('tex') else '') +
                    (' texlist ' + ' '.join(f'{t[0]:#x}/{t[1]:#x}+({t[2]},{t[3]})' for t in m['texlist'])
                     if m.get('texlist') else ''))
    if 'somodel' in r:
        m = r['somodel']
        bits.append(f"{m.get('polys')} polys " + (f"tex {' '.join(m['tex'])}" if m.get('textured') else 'gouraud'))
    if r.get('sprite'):
        s = r['sprite']
        bits.append(' '.join(f'{k}={v}' for k, v in s.items()
                             if k in ('frames', 'rings', 'segments', 'type', 'points', 'gouraud')))
    uses = sorted({f"{u.get('call') or ('=' + str(u.get('var')))}@{u.get('func')}" for u in r['uses']})
    return ' '.join(bits), uses


def main():
    ap = argparse.ArgumentParser(description=__doc__, formatter_class=argparse.RawDescriptionHelpFormatter)
    ap.add_argument('what', nargs='+', help='module names (fx_004) or dir-13 file ids (0x4)')
    ap.add_argument('--disks', default=os.path.join(ROOT, 'disks', 'us'))
    ap.add_argument('--render', action='store_true', help='write PNGs')
    ap.add_argument('--vram', action='store_true', help='only write each file\'s VRAM image')
    ap.add_argument('--out', default=os.path.join(ROOT, 'build', 'fxdata'))
    ap.add_argument('--json')
    ap.add_argument('--md')
    args = ap.parse_args()
    img = Img(os.path.join(args.disks, 'disc1', 'FF9.IMG'))
    common = load_common_vram(img)
    lab = labels()
    results = []
    for module, fid, group in resolve(args.what):
        if args.vram:
            fx = FxFile(img, fid, common)
            p = os.path.join(args.out, f'{fid:03x}', 'vram.png')
            os.makedirs(os.path.dirname(p), exist_ok=True)
            fx.vram.save(p)
            print(p)
            continue
        res = analyse(img, common, module, fid, group, args.out, args.render)
        res['label'] = lab.get(module or '', '')
        results.append(res)
    if args.vram:
        return
    md = []
    for res in results:
        md.append(f"## {res['module']} (file {res['file']:#05x}, group {res['group']}): {res['label']}\n")
        md.append(f"module ops run by the script: {', '.join(f'{o:#x}({a},{b})' for o, a, b in res['module_ops'])}")
        for pt in res['parts'][1:]:
            md.append(f"{pt['base']:#x}+: {pt['what']}, {pt['entries']} entries")
        if res['common_tex']:
            md.append('common pages the file uploads (type 9): ' +
                      ' '.join(f'({x:#x}, {y:#x})' for x, y in res['common_tex']))
        if res['summon_tex']:
            st = res['summon_tex']
            md.append(f"summon textures (type 4): {st['textures']} pages, tpages "
                      f"{' '.join(f'{t:#x}' for t in st['tpages'])}, {st['clutRows']} CLUT rows at y 0xE6")
        if res['indices_not_in_table']:
            md.append(f"indices the C uses that are not in the table: {res['indices_not_in_table']}")
        md.append('')
        md.append('| idx | size | kind | detail | C use / script | png |')
        md.append('|---|---|---|---|---|---|')
        for r in res['entries']:
            det, uses = fmt_entry(r)
            md.append(f"| {r['index']:#x} | {r['size']:#x} | {r['kind']} | {det} | "
                      f"{'; '.join(uses + r['script'])} | {' '.join(r.get('png', []))} |")
        md.append('')
    text = '\n'.join(md)
    if args.md:
        with open(args.md, 'w') as f:
            f.write(text + '\n')
    if args.json:
        with open(args.json, 'w') as f:
            json.dump(jsonable(results), f, indent=1)
    if not args.md:
        print(text)


if __name__ == '__main__':
    main()
