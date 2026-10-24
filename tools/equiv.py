#!/usr/bin/env python3
"""Check that a non-matching draft behaves like the original function (differential testing).

    tools/docker.sh python3 tools/equiv.py docs/wip/<draft>.c [--func func_800C65CC] [--trials 200]

Compiles the draft like ccprobe, links it at 0x80600000 against the built ELFs' symbols, then runs the
original and the draft in a small MIPS-I interpreter from the same random states and compares what each
one does that a caller can see:
  - the calls it makes (target, and the arguments the callee's prototype says it takes),
  - every byte it leaves in memory outside its own stack frame,
  - v0 (unless the function is void), and whether it stops on a `break` (divide guard).
Memory nobody wrote reads as values derived from the address and the trial seed (mostly small numbers,
some pointer-like, some random), except the binary's code/rodata (and main's), which read as the real image. External
calls return values derived from (call number, target, arguments) and clobber the caller-saved registers.
GTE commands set the output registers to a hash of the whole GTE register file, so identical inputs give
identical outputs in both versions; exact GTE maths isn't modelled.

Register allocation, scheduling and stack layout don't show up; a real difference (a wrong operator,
a missing store, a swapped argument) usually does within a few trials. A clean result is evidence, not
proof: paths the random states never reach aren't tested (the tool prints how much of the original ran).

Known false positive: an index that random memory makes far out of range into a local array (a stack
table copied from rodata, say) reads different garbage in each version, because their frames differ.
Check a reported difference's addresses before calling the draft wrong.
"""
import argparse
import hashlib
import os
import re
import struct
import subprocess
import sys
import tempfile

sys.path.insert(0, os.path.dirname(__file__))
import ccprobe  # noqa: E402

DRAFT_BASE = 0x80600000  # outside RAM, but in the same 256 MB jal region
STACK_TOP = 0x801FFF00
RA_SENTINEL = 0x80FFFFF0
MAX_STEPS = 400000
MASK = 0xFFFFFFFF


def run(cmd):
    p = subprocess.run(cmd, capture_output=True, text=True)
    if p.returncode:
        sys.exit(f"{cmd[0]} failed:\n{p.stderr.strip()[:2000]}")
    return p.stdout


def elf_loads(path):
    """PT_LOAD segments of a 32-bit LE ELF as (vaddr, bytes, memsz)."""
    d = open(path, "rb").read()
    phoff, = struct.unpack_from("<I", d, 0x1C)
    phentsize, phnum = struct.unpack_from("<HH", d, 0x2A)
    out = []
    for i in range(phnum):
        typ, off, vaddr, _, filesz, memsz = struct.unpack_from("<6I", d, phoff + i * phentsize)
        if typ == 1 and memsz:
            out.append((vaddr, d[off:off + filesz], memsz))
    return out


def nm(path):
    syms = {}
    for line in run(["mipsel-linux-gnu-nm", path]).splitlines():
        p = line.split()
        if len(p) == 3 and "." not in p[2]:
            syms.setdefault(p[2], int(p[0], 16))
    return syms


def prototypes(ipath):
    """name -> number of argument words (None = varargs or unknown), void-ness, from the preprocessed draft."""
    text = open(ipath).read()
    text = re.sub(r"\{[^{}]*\}", ";", text)  # crude: drop struct bodies so their members don't look like decls
    out = {}
    for m in re.finditer(r"(\b[\w\s\*]+?)\b(\w+)\s*\(([^()]*(?:\([^()]*\)[^()]*)*)\)\s*[;{]", text):
        ret, name, args = m.group(1).split(), m.group(2), m.group(3).strip()
        if name in ("if", "while", "for", "switch", "return", "sizeof"):
            continue
        if args in ("", "void"):
            n = 0
        elif "..." in args:
            n = None
        else:
            n = args.count(",") + 1
        out[name] = (n, ret[-1:] == ["void"] and "*" not in m.group(1))
    return out


class Mem:
    def __init__(self, seed, images, sparse=False):
        self.seed = seed
        self.sparse = sparse
        self.images = images  # list of (start, bytes)
        self.w = {}
        self.cache = {}

    def base_word(self, a):
        for start, data in self.images:
            if start <= a < start + len(data) - 3:
                return struct.unpack_from("<I", data, a - start)[0]
        v = self.cache.get(a)
        if v is None:
            h = int.from_bytes(hashlib.blake2b(struct.pack("<II", self.seed, a), digest_size=8).digest(), "little")
            k = h % 100
            if self.sparse and k < 55:
                k = 55  # no zero or tiny words, so loaded pointers (almost) never overlap
            if k < 30:
                v = 0
            elif k < 55:
                v = (h >> 8) % 16
            elif k < 67:
                v = 0x80000000 | ((h >> 8) & 0x1FFFFC)
            elif k < 80:
                v = (h >> 16) & MASK
            else:  # byte fields: each lane 0, small or random on its own
                v = 0
                for lane in range(4):
                    b = (h >> (8 + 10 * lane)) & 0x3FF
                    v |= (0 if b % 3 == 0 else (b >> 2) % 8 if b % 3 == 1 else b >> 2) << (8 * lane)
            self.cache[a] = v
        return v

    def fetch(self, a):
        """Instructions come from the image even if the function stored over them."""
        return self.base_word(a)

    def rb(self, a):
        a &= MASK
        v = self.w.get(a)
        if v is not None:
            return v
        return (self.base_word(a & ~3) >> (8 * (a & 3))) & 0xFF

    def read(self, a, n):
        v = 0
        for i in range(n):
            v |= self.rb(a + i) << (8 * i)
        return v

    def write(self, a, v, n):
        for i in range(n):
            self.w[(a + i) & MASK] = (v >> (8 * i)) & 0xFF


class AddressError(RuntimeError):
    pass


def sx16(v):
    return (v & 0xFFFF) - 0x10000 if v & 0x8000 else v & 0xFFFF


def s32(v):
    return v - 0x100000000 if v & 0x80000000 else v


GTE_OUT = set(range(7, 28)) - {6}


class Cpu:
    def __init__(self, mem, lo, hi, protos, addr_names, seed, args, stops=()):
        self.m = mem
        self.lo, self.hi = lo, hi  # code range executed in place (everything else is an external call)
        self.stops = stops  # entry points inside that range that still count as external calls
        self.protos, self.names = protos, addr_names
        self.r = [0] * 32
        for i in range(1, 32):
            self.r[i] = 0xD0000000 + i * 0x100
        self.r[4:8] = args
        self.r[29] = STACK_TOP
        self.r[31] = RA_SENTINEL
        self.r[30] = 0x5E5E0030
        self.rhi = self.rlo = 0
        self.g = [0] * 64
        for i in range(64):
            self.g[i] = int.from_bytes(hashlib.blake2b(struct.pack("<II", seed, 0x6E000 + i), digest_size=4).digest(), "little")
        self.calls = []
        self.events = []
        self.path = []
        self.seen = set()

    def gte_cmd(self, word):
        h = hashlib.shake_256(struct.pack("<I64I", word, *self.g)).digest(128)
        for i, reg in enumerate(sorted(GTE_OUT)):
            self.g[reg] = struct.unpack_from("<I", h, i * 4)[0]
        self.g[63] = struct.unpack_from("<I", h, 124)[0]

    def external(self, target):
        name = self.names.get(target, f"0x{target:08X}")
        n, _ = self.protos.get(name, (None, False))
        words = [self.r[4], self.r[5], self.r[6], self.r[7]]
        sp = self.r[29]
        nargs = 4 if n is None else n
        args = [words[i] if i < 4 else self.m.read(sp + 4 * i, 4) for i in range(min(nargs, 8))]
        self.calls.append((name, tuple(args)))
        h = hashlib.blake2b(repr((len(self.calls), name, args)).encode(), digest_size=8).digest()
        v = int.from_bytes(h[:4], "little")
        self.r[2] = (0, v % 16, v)[h[4] % 3]
        self.r[3] = int.from_bytes(h[4:], "little")
        for i in [1] + list(range(4, 16)) + [24, 25]:
            self.r[i] = 0xC0000000 + len(self.calls) * 0x100 + i

    def run(self, pc):
        r, m = self.r, self.m
        steps = 0
        delay = None
        while True:
            steps += 1
            if steps > MAX_STEPS:
                self.events.append("step limit")
                return
            if pc == RA_SENTINEL:
                return
            if not (self.lo <= pc < self.hi) or pc in self.stops:
                self.external(pc)
                pc = r[31]
                continue
            ins = m.fetch(pc)
            self.seen.add(pc)
            npc = delay if delay is not None else pc + 4
            delay = None
            op = ins >> 26
            rs, rt, rd = (ins >> 21) & 31, (ins >> 16) & 31, (ins >> 11) & 31
            imm = ins & 0xFFFF
            simm = sx16(imm)
            target = None
            if op == 0:
                fn, sa = ins & 63, (ins >> 6) & 31
                a, b = r[rs], r[rt]
                v = None
                if fn == 0x00: v = (b << sa) & MASK
                elif fn == 0x02: v = b >> sa
                elif fn == 0x03: v = (s32(b) >> sa) & MASK
                elif fn == 0x04: v = (b << (a & 31)) & MASK
                elif fn == 0x06: v = b >> (a & 31)
                elif fn == 0x07: v = (s32(b) >> (a & 31)) & MASK
                elif fn == 0x08: target = a
                elif fn == 0x09:
                    target = a
                    r[rd] = (pc + 8) & MASK
                elif fn == 0x0D:
                    self.events.append(f"break {(ins >> 6) & 0xFFFFF}")
                    return
                elif fn == 0x0C:
                    self.events.append("syscall")
                    return
                elif fn == 0x10: v = self.rhi
                elif fn == 0x11: self.rhi = a
                elif fn == 0x12: v = self.rlo
                elif fn == 0x13: self.rlo = a
                elif fn == 0x18:
                    p = s32(a) * s32(b)
                    self.rlo, self.rhi = p & MASK, (p >> 32) & MASK
                elif fn == 0x19:
                    p = a * b
                    self.rlo, self.rhi = p & MASK, (p >> 32) & MASK
                elif fn == 0x1A:
                    if b == 0:
                        self.rlo, self.rhi = (1 if s32(a) < 0 else MASK), a
                    else:
                        q = abs(s32(a)) // abs(s32(b))
                        if (s32(a) < 0) != (s32(b) < 0):
                            q = -q
                        self.rlo, self.rhi = q & MASK, (s32(a) - q * s32(b)) & MASK
                elif fn == 0x1B:
                    if b == 0:
                        self.rlo, self.rhi = MASK, a
                    else:
                        self.rlo, self.rhi = a // b, a % b
                elif fn in (0x20, 0x21): v = (a + b) & MASK
                elif fn in (0x22, 0x23): v = (a - b) & MASK
                elif fn == 0x24: v = a & b
                elif fn == 0x25: v = a | b
                elif fn == 0x26: v = a ^ b
                elif fn == 0x27: v = ~(a | b) & MASK
                elif fn == 0x2A: v = int(s32(a) < s32(b))
                elif fn == 0x2B: v = int(a < b)
                else:
                    raise RuntimeError(f"special {fn:#x} at {pc:08X}")
                if v is not None and rd:
                    r[rd] = v
            elif op == 1:
                a = s32(r[rs])
                if rt in (0x10, 0x11):
                    r[31] = (pc + 8) & MASK
                cond = a < 0 if rt in (0, 0x10) else a >= 0
                self.path.append(cond)
                if cond:
                    target = (pc + 4 + (simm << 2)) & MASK
            elif op in (2, 3):
                if op == 3:
                    r[31] = (pc + 8) & MASK
                target = ((pc + 4) & 0xF0000000) | ((ins & 0x3FFFFFF) << 2)
            elif op in (4, 5, 6, 7):
                a, b = r[rs], r[rt]
                cond = {4: a == b, 5: a != b, 6: s32(a) <= 0, 7: s32(a) > 0}[op]
                self.path.append(cond)
                if cond:
                    target = (pc + 4 + (simm << 2)) & MASK
            elif op in (8, 9):
                if rt: r[rt] = (r[rs] + simm) & MASK
            elif op == 0x0A:
                if rt: r[rt] = int(s32(r[rs]) < simm)
            elif op == 0x0B:
                if rt: r[rt] = int(r[rs] < (simm & MASK))
            elif op == 0x0C:
                if rt: r[rt] = r[rs] & imm
            elif op == 0x0D:
                if rt: r[rt] = r[rs] | imm
            elif op == 0x0E:
                if rt: r[rt] = r[rs] ^ imm
            elif op == 0x0F:
                if rt: r[rt] = imm << 16
            elif op == 0x12:
                sub = rs
                if ins & (1 << 25):
                    self.gte_cmd(ins)
                elif sub == 0:
                    if rt: r[rt] = self.g[rd]
                elif sub == 2:
                    if rt: r[rt] = self.g[32 + rd]
                elif sub == 4:
                    self.g[rd] = r[rt]
                elif sub == 6:
                    self.g[32 + rd] = r[rt]
                else:
                    raise RuntimeError(f"cop2 {ins:08X} at {pc:08X}")
            elif op in (0x20, 0x21, 0x23, 0x24, 0x25, 0x22, 0x26):
                addr = (r[rs] + simm) & MASK
                if op == 0x20: v = m.read(addr, 1); v = v - 0x100 if v & 0x80 else v
                elif op == 0x24: v = m.read(addr, 1)
                elif op in (0x21, 0x25, 0x23) and addr & (1 if op != 0x23 else 3):
                    raise AddressError(f"unaligned load at {pc:08X}")
                elif op == 0x21: v = sx16(m.read(addr, 2))
                elif op == 0x25: v = m.read(addr, 2)
                elif op == 0x23: v = m.read(addr, 4)
                elif op == 0x22:  # lwl
                    al = addr & ~3; sh = addr & 3; w = m.read(al, 4)
                    v = ((w << (8 * (3 - sh))) | (r[rt] & ((1 << (8 * (3 - sh))) - 1))) & MASK
                else:  # lwr
                    al = addr & ~3; sh = addr & 3; w = m.read(al, 4)
                    v = (w >> (8 * sh)) | (r[rt] & ~(MASK >> (8 * sh)) & MASK)
                if rt: r[rt] = v & MASK
            elif op in (0x28, 0x29, 0x2B, 0x2A, 0x2E):
                addr = (r[rs] + simm) & MASK
                v = r[rt]
                if op in (0x29, 0x2B) and addr & (1 if op == 0x29 else 3):
                    raise AddressError(f"unaligned store at {pc:08X}")
                if op == 0x28: m.write(addr, v, 1)
                elif op == 0x29: m.write(addr & ~1, v, 2)
                elif op == 0x2B: m.write(addr & ~3, v, 4)
                elif op == 0x2A:  # swl
                    sh = addr & 3
                    for i in range(sh + 1):
                        m.write((addr & ~3) + i, v >> (8 * (3 - sh + i)), 1)
                else:  # swr
                    sh = addr & 3
                    for i in range(4 - sh):
                        m.write(addr + i, v >> (8 * i), 1)
            elif op in (0x32, 0x3A):  # lwc2, swc2
                addr = (r[rs] + simm) & MASK
                if addr & 3:
                    raise AddressError(f"unaligned cop2 access at {pc:08X}")
                if op == 0x32:
                    self.g[rt] = m.read(addr, 4)
                else:
                    m.write(addr, self.g[rt], 4)
            else:
                raise RuntimeError(f"op {op:#x} ({ins:08X}) at {pc:08X}")
            r[0] = 0
            if target is not None:
                delay = target & MASK
            pc = npc


def main():
    ap = argparse.ArgumentParser()
    ap.add_argument("src")
    ap.add_argument("--func", help="default: the func_XXXXXXXX in the file name")
    ap.add_argument("--bin", help="default: from the file name (main_8005681C.c, ovl_0a_800C65CC.c, fx_0b2_801E78FC.c)")
    ap.add_argument("--trials", type=int, default=200)
    ap.add_argument("--flags", default="-O2 -G0")
    ap.add_argument("--sparse", action="store_true",
                    help="no zero/small words in unwritten memory: rules out differences that need two pointers "
                         "to overlap (loads and stores may be reordered across such pairs)")
    ap.add_argument("--ignore", action="append", default=[],
                    help="START-END (hex) memory range whose writes aren't compared (e.g. a hand-written "
                         "routine's register save area in the scratchpad)")
    ap.add_argument("--arg-mask", action="append", default=[], metavar="N:MASK",
                    help="AND argument N (0-3) with MASK (hex) in every trial, e.g. 1:0xF for a loop count that "
                         "would otherwise run into the step limit")
    ap.add_argument("--end", help="run the original up to this address (hex) instead of the next symbol: for an "
                                  "entry that jumps into a following function's body (shared tails)")
    ap.add_argument("--uncovered", action="store_true", help="list the original's instructions no trial ran")
    ap.add_argument("-v", action="store_true", help="print the first difference in full")
    a = ap.parse_args()
    base = os.path.basename(a.src)
    mm = re.match(r"(main|ovl_[0-9a-f]{2}|fx_[0-9a-f]{3}(?:_\d)?)_([0-9A-F]{8})", base)
    binary = a.bin or (mm and mm.group(1)) or "main"
    func = a.func or (mm and f"func_{mm.group(2)}")
    if not func:
        sys.exit("give --func")

    belf = "build/us/slus_012.51.elf" if binary == "main" else f"build/us/{binary}.elf"
    index = {}
    for line in run(["mipsel-linux-gnu-nm", "-S", belf]).splitlines():
        p = line.split()
        if len(p) == 4 and p[2] in "tT":
            index[p[3]] = (int(p[0], 16), int(p[1], 16))
    if func not in index:
        sys.exit(f"{func} not in {belf}")
    oaddr, osize = index[func]
    # nm sizes can stop short of a function's switch cases; run up to the next function instead
    osize = max(osize, min((v for v, _ in index.values() if v > oaddr), default=oaddr + osize) - oaddr)
    if a.end:
        osize = int(a.end, 16) - oaddr

    # Only rodata + text are real; .data/.bss read as random like everything else.
    images = []
    exe = open(ccprobe.EXE, "rb").read()
    syms = nm("build/us/slus_012.51.elf")
    images.append((0x80010000, exe[0x800:0x800 + syms["main_DATA_START"] - 0x80010000]))
    if binary != "main":
        osyms = nm(f"build/us/{binary}.elf")
        start = osyms[f"{binary}_RODATA_START"]
        if binary.startswith("fx_"):  # battle effect module (docs/ff9img.md)
            data = open(f"disks/us/fxmod/{binary}.bin", "rb").read()
        else:
            data = open(f"disks/us/ovl/{binary[4:]}.bin", "rb").read()
        images.insert(0, (start, data[:osyms[f"{binary}_DATA_START"] - start]))
        syms.update(osyms)

    with tempfile.TemporaryDirectory() as tmp:
        ccprobe.compile_c(a.src, "gcc-2.8.1-psx", a.flags, "2.77", True, tmp)
        obj, ipath = os.path.join(tmp, "x.o"), os.path.join(tmp, "x.i")
        undef = [l.split()[-1] for l in run(["mipsel-linux-gnu-nm", "-u", obj]).splitlines()]
        missing = [u for u in undef if u not in syms]
        if missing:
            sys.exit(f"unresolved: {' '.join(missing)}")
        ld = os.path.join(tmp, "x.ld")
        with open(ld, "w") as f:
            for u in undef:
                f.write(f"{u} = 0x{syms[u]:08X};\n")
            f.write(f"SECTIONS {{ . = 0x{DRAFT_BASE:08X}; .text : {{ *(.text) }} .rodata : {{ *(.rodata*) }} "
                    ".data : { *(.data*) } .bss : { *(.bss*) *(COMMON) } /DISCARD/ : { *(*) } }\n")
        elf = os.path.join(tmp, "x.elf")
        run(["mipsel-linux-gnu-ld", "-T", ld, "-o", elf, obj])
        dsyms = nm(elf)
        dglobal_lines = [l for l in run(["mipsel-linux-gnu-nm", elf]).splitlines() if len(l.split()) == 3]
        protos = prototypes(ipath)
        dimg = [(v, d + bytes(ms - len(d))) for v, d, ms in elf_loads(elf)]
    if func not in dsyms:
        sys.exit(f"{func} not defined by {a.src}")
    dlo = min(v for v, _ in dimg)
    dhi = max(v + len(d) for v, d in dimg)
    names = {v: k for k, v in syms.items()}
    # The draft file's other global functions stand for the originals of the same name: calls to them are
    # compared as calls, not run (the original calls its own copy).
    dglobal = {l.split()[2]: int(l.split()[0], 16) for l in dglobal_lines if l.split()[1] == "T"}
    stops = {v for k, v in dglobal.items() if k != func}
    for k, v in dglobal.items():
        if k != func:
            names[v] = k
    n_args, is_void = protos.get(func, (4, False))
    n_args = 4 if n_args is None else n_args

    ignore = [tuple(int(x, 16) for x in r.split("-")) for r in a.ignore]
    paths, bad, covered, skipped = set(), 0, set(), 0
    for t in range(a.trials):
        res = []
        for which in ("orig", "draft"):
            mem = Mem(t, (dimg if which == "draft" else []) + images, a.sparse)
            args = [mem.base_word(0x7FF00000 + 4 * i) for i in range(4)]
            for i in range(n_args, 4):
                args[i] = 0xA0000000 + i
            for am in a.arg_mask:
                i, msk = am.split(":")
                args[int(i)] &= int(msk, 16)
            lo, hi = (oaddr, oaddr + osize) if which == "orig" else (dlo, dhi)
            cpu = Cpu(mem, lo, hi, protos, names, t, args, stops if which == "draft" else ())
            try:
                cpu.run(oaddr if which == "orig" else dsyms[func])
            except AddressError as e:
                cpu.events.append("address error")
                cpu.fault = str(e)
            except RuntimeError as e:
                cpu.events.append(str(e))
            written = {k: v for k, v in mem.w.items() if not (STACK_TOP - 0x10000 <= k < STACK_TOP + 16)
                       and not any(lo_ <= k < hi_ for lo_, hi_ in ignore)}
            res.append((cpu, written))
        (co, wo), (cd, wd) = res
        if "address error" in co.events:
            skipped += 1  # the original faults (unaligned random pointer): not a state the game produces
            continue
        if "step limit" in co.events:
            skipped += 1  # the original didn't finish (random loop counts): the two runs stop at different points
            continue
        paths.add(tuple(co.path[:4000]))
        covered |= co.seen
        diffs = []
        if co.calls != cd.calls:
            i = next((k for k in range(min(len(co.calls), len(cd.calls))) if co.calls[k] != cd.calls[k]),
                     min(len(co.calls), len(cd.calls)))
            diffs.append(f"call #{i}: orig {co.calls[i] if i < len(co.calls) else None} "
                         f"draft {cd.calls[i] if i < len(cd.calls) else None}")
        if wo != wd:
            ks = sorted(k for k in set(wo) | set(wd) if wo.get(k) != wd.get(k))
            diffs.append(f"{len(ks)} memory bytes differ, first at {ks[0]:08X}: "
                         f"orig {wo.get(ks[0])} draft {wd.get(ks[0])}")
        if not is_void and co.r[2] != cd.r[2]:
            diffs.append(f"v0: orig {co.r[2]:08X} draft {cd.r[2]:08X}")
        if co.events != cd.events:
            diffs.append(f"events: orig {co.events} draft {cd.events}")
        if diffs:
            bad += 1
            if bad <= (5 if a.v else 1):
                print(f"trial {t}: " + "; ".join(diffs))
    print(f"{func}: {a.trials - skipped - bad}/{a.trials - skipped} trials agree"
          + (f" ({skipped} skipped: the original faults on an unaligned address or hits the step limit)" if skipped else "")
          + f", {len(paths)} distinct paths, "
          f"{100 * len(covered) // (osize // 4)}% of the original's instructions ran")
    if a.uncovered:
        print("never ran:", " ".join(f"{x:08X}" for x in range(oaddr, oaddr + osize, 4) if x not in covered))
    sys.exit(1 if bad else 0)


if __name__ == "__main__":
    main()
