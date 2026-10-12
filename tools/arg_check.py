#!/usr/bin/env python3
"""Find argument pass-through that only works on MIPS: functions whose code reads more arguments than
their C declares, and C calls that pass fewer arguments than the callee reads.

    tools/docker.sh python3 tools/arg_check.py [--bin ovl_0a]

Example: `s32 func_800AA3F4(void)` calls `func_800AAEF8()`, and func_800AAEF8 reads $a0. The C matches
because $a0 still holds func_800AA3F4's caller's argument; compiled for another CPU, the value is lost.

From the asm of the 20 built ELFs it computes, for every function, which of $a0-$a3 and which stack
argument slots (sp + frame + 16...) it reads before writing them on some path. A jal counts as reading
whatever its callee reads that is still unwritten, so pass-through propagates (iterated to a fixpoint).
jalr (function pointers) and jr through jump tables are handled conservatively (no reads / every
instruction a successor). Then:
  - DECL: a function's C definition (or, for asm functions, its prototype) has fewer parameters than it reads;
  - CALL: a C call passes fewer arguments than its callee reads.
Calls are found by a regex over src/ (calls inside macros and comments too), so check each report.
"""
import argparse
import glob
import re
import struct
import subprocess
from collections import defaultdict

ELFS = ["build/us/slus_012.51.elf"] + sorted(glob.glob("build/us/ovl_*.elf"))
INS = re.compile(r"^\s*([0-9a-f]+):\s+(\S+)\s*(.*)$")
ARGS = ("a0", "a1", "a2", "a3")
BRANCH = {"beq", "bne", "beqz", "bnez", "blez", "bgtz", "bltz", "bgez", "beql", "bnel"}
REG = re.compile(r"\b(zero|at|v[01]|a[0-3]|t[0-9]|s[0-8]|k[01]|gp|sp|fp|ra)\b")
SRC_ONLY = ("s", "beq", "bne", "bl", "bg", "mt", "j", "mult", "div")
NOT_SRC_ONLY = ("sll", "srl", "sra", "slt", "sub", "seb", "seh")


def reads_writes(mn, ops):
    r = REG.findall(ops)
    if not r:
        return [], []
    if mn.startswith(SRC_ONLY) and not mn.startswith(NOT_SRC_ONLY):
        return r, []
    if mn in ("mfc2", "cfc2", "mflo", "mfhi"):
        return [], r[:1]
    return r[1:], r[:1]


def load_image(elf):
    d = open(elf, "rb").read()
    phoff, = struct.unpack_from("<I", d, 0x1C)
    phentsize, phnum = struct.unpack_from("<HH", d, 0x2A)
    segs = []
    for i in range(phnum):
        typ, off, vaddr, _, filesz, _ = struct.unpack_from("<6I", d, phoff + i * phentsize)
        if typ == 1:
            segs.append((vaddr, d[off:off + filesz]))

    def word(a):
        for v, data in segs:
            if v <= a < v + len(data) - 3:
                return struct.unpack_from("<I", data, a - v)[0]
        return None
    return word


def jump_table(ins, i, word):
    """Targets of the jr at ins[i] (None if the table isn't found). The table address is built as
    `lui X,hi` + `lw R,lo(X)`, or `lui X,hi` + `addiu X,X,lo` + `lw R,0(...)`; `sltiu _,_,N` bounds it."""
    window = ins[max(0, i - 16):i]
    hi = lo = count = None
    for _, mn, ops in window:
        o = ops.replace(" ", "")
        m = re.match(r"^(\w+),0x([0-9a-f]+)$", o)
        if mn == "lui" and m:
            hi, hreg, lo = int(m.group(2), 16), m.group(1), None
            continue
        if hi is not None:
            m = re.match(r"^(\w+),(\w+),(-?\d+)$", o)
            if mn == "addiu" and m and m.group(1) == hreg and m.group(2) == hreg:
                lo = int(m.group(3))
            m = re.match(r"^\w+,(-?\d+)\((\w+)\)$", o)
            if mn == "lw" and m and m.group(2) == hreg and lo is None:
                lo = int(m.group(1))
        m = re.match(r"^\w+,\w+,(\d+)$", o)
        if mn == "sltiu" and m:
            count = int(m.group(1))
    if hi is None or lo is None:
        return None
    addr = ((hi << 16) + lo) & 0xFFFFFFFF
    lo_f, hi_f = ins[0][0], ins[-1][0]
    out = []
    while count is None or len(out) < count:
        w = word(addr)
        if w is None or not (lo_f <= w <= hi_f) or len(out) > 512:
            break
        out.append(w)
        addr += 4
    return out or None


def load_funcs(elf):
    dis = subprocess.run(["mipsel-linux-gnu-objdump", "-d", "--no-show-raw-insn", elf],
                         capture_output=True, text=True).stdout
    funcs, cur = {}, None
    for line in dis.splitlines():
        m = re.match(r"^([0-9a-f]+) <([\w.]+)>:", line)
        if m:
            cur = m.group(2)
            funcs[cur] = []
            continue
        m = INS.match(line)
        if m and cur:
            funcs[cur].append((int(m.group(1), 16), m.group(2), m.group(3)))
    return {k: v for k, v in funcs.items() if v and not k.endswith(".NON_MATCHING")}


def callee_of(ops):
    m = re.search(r"<([\w.]+)>", ops)
    return m.group(1) if m and "+" not in m.group(1) else None


def analyse(ins, reads_of, why=None, word=None):
    """Set of argument indices (0-3 registers, 4+ stack slots) read before written."""
    n = len(ins)
    index = {a: i for i, (a, _, _) in enumerate(ins)}
    frame = 0
    for a, mn, ops in ins[:8]:
        m = re.match(r"sp,sp,(-\d+)$", ops.replace(" ", ""))
        if mn == "addiu" and m:
            frame = -int(m.group(1))
            break
    full = frozenset(range(4))
    state = [None] * n  # maybe-unwritten arg registers before instruction i
    state[0] = full
    work = [0]
    reads = set()

    def push(j, s):
        if 0 <= j < n:
            old = state[j]
            new = s if old is None else old | s
            if new != old:
                state[j] = new
                work.append(j)

    def step(i, s):
        a, mn, ops = ins[i]
        rd, wr = reads_writes(mn, ops)
        h = re.match(r"^(\w+),(-?\d+)\(sp\)$", ops.replace(" ", ""))
        if h and mn in ("sw", "sh", "sb") and frame <= int(h.group(2)) < frame + 16:
            rd = []  # spilling an argument register to its home slot (varargs, &param) isn't a use
        for r in rd:
            if r in ARGS and ARGS.index(r) in s:
                reads.add(ARGS.index(r))
                if why is not None:
                    why.add(f"{a:08X} {mn} {ops}")
        m = re.match(r"^\w+,(-?\d+)\(sp\)$", ops.replace(" ", ""))
        if m and mn.startswith("l") and frame and int(m.group(1)) >= frame + 16:
            reads.add((int(m.group(1)) - frame) // 4)
        return s - {ARGS.index(r) for r in wr if r in ARGS}

    while work:
        i = work.pop()
        s = state[i]
        a, mn, ops = ins[i]
        if mn in BRANCH or mn in ("j", "b", "jal", "jr", "jalr", "bal"):
            s1 = step(i, s)
            if i + 1 < n:
                s2 = step(i + 1, s1)
            else:
                s2 = s1
            if mn == "jal":
                c = callee_of(ops)
                for k in reads_of.get(c, ()):
                    if k < 4 and k in s2:
                        reads.add(k)
                        if why is not None:
                            why.add(f"{a:08X} jal {c} (reads a{k})")
                push(i + 2, frozenset())
            elif mn == "jalr":
                push(i + 2, frozenset())
            elif mn == "jr":
                if not ops.strip().startswith("ra"):
                    tgts = jump_table(ins, i, word) if word else None
                    for j in ([index[t] for t in tgts if t in index] if tgts else range(n)):
                        push(j, s2)
            else:
                t = re.match(r"^(?:.*,)?\s*([0-9a-f]+)\s*<", ops)
                tgt = int(t.group(1), 16) if t else None
                if tgt in index:
                    push(index[tgt], s2)
                elif mn in ("j", "b") and tgt is not None:
                    pass  # tail call out of the function
                if mn not in ("j", "b"):
                    push(i + 2, s2)
        else:
            push(i + 1, step(i, s))
    return reads


SCALAR = {"void", "s8", "u8", "s16", "u16", "s32", "u32", "s64", "u64", "f32", "int", "char", "short", "long",
          "unsigned", "signed", "float", "double", "static", "inline", "extern", "const", "volatile", "register"}


def c_params():
    """binary -> name -> (param count or None for varargs, where)."""
    def scan(files, out):
        for f in files:
            text = open(f).read()
            for m in re.finditer(r"^[A-Za-z_][\w \t\*]*?\b(\w+)\s*\(", text, re.M):
                name = m.group(1)
                i = m.end()
                depth, j = 1, i
                while j < len(text) and depth:
                    depth += {"(": 1, ")": -1}.get(text[j], 0)
                    j += 1
                args = text[i:j - 1].strip()
                rest = text[j:j + 40].lstrip()
                if not (rest.startswith("{") or rest.startswith(";")):
                    continue
                if args == "" and not rest.startswith("{"):
                    continue  # `f();` says nothing about the parameters
                if args in ("", "void"):
                    n = 0
                elif "..." in args:
                    n = None
                else:
                    n = depth_commas(args) + 1
                ret = m.group(0)[:m.group(0).rindex(name)].split()
                if n is not None and "*" not in "".join(ret) and not set(ret) <= SCALAR:
                    n += 1  # a struct return value: the caller passes the result's address in $a0
                line = text.count("\n", 0, m.start()) + 1
                if rest.startswith("{") or name not in out:
                    out[name] = (n, f"{f}:{line}", n is not None and "*" not in "".join(ret) and not set(ret) <= SCALAR)
    common = {}
    scan(["include/psyq.h", "include/ff9.h"] + glob.glob("src/main/*.c"), common)
    out = {"main": common}
    for elf in ELFS[1:]:
        b = elf.split("/")[-1][:-4]
        d = dict(common)
        scan(glob.glob(f"include/{b}.h") + glob.glob(f"src/{b}/*.c"), d)
        out[b] = d
    return out


def depth_commas(s):
    depth, n = 0, 0
    for ch in s:
        if ch in "([{":
            depth += 1
        elif ch in ")]}":
            depth -= 1
        elif ch == "," and depth == 0:
            n += 1
    return n


def c_calls(binary):
    files = glob.glob(f"src/{binary}/*.c") + (["include/ff9.h"] if binary == "main" else [f"include/{binary}.h"])
    for f in files:
        blank = lambda m: re.sub(r"[^\n]", " ", m.group(0))
        text = re.sub(r"/\*.*?\*/|//[^\n]*", blank, open(f).read(), flags=re.S)
        text = re.sub(r'"(?:[^"\\\n]|\\.)*"', lambda m: '"' + " " * (len(m.group(0)) - 2) + '"', text)
        for m in re.finditer(r"\b(\w+)\s*\(", text):
            ls = text.rfind("\n", 0, m.start()) + 1
            if text[ls] not in " \t" or m.group(1) in ("if", "while", "for", "switch", "return", "sizeof"):
                continue  # a definition or declaration at column 0, or a keyword
            i = m.end()
            depth, j = 1, i
            while j < len(text) and depth:
                depth += {"(": 1, ")": -1}.get(text[j], 0)
                j += 1
            args = text[i:j - 1].strip()
            n = 0 if args == "" else depth_commas(args) + 1
            yield m.group(1), n, f"{f}:{text.count(chr(10), 0, m.start()) + 1}"


def main():
    ap = argparse.ArgumentParser()
    ap.add_argument("--bin")
    ap.add_argument("--why", help="explain one function's reads (needs --bin)")
    a = ap.parse_args()
    funcs, words = {}, {}
    for elf in ELFS:
        b = "main" if "slus" in elf else elf.split("/")[-1][:-4]
        funcs[b] = load_funcs(elf)
        words[b] = load_image(elf)
    params = c_params()
    lib = set(open("config/library_funcs.us.txt").read().split()) if glob.glob("config/library_funcs.us.txt") else set()
    lib |= {l.split("=")[0].strip() for f in glob.glob("config/symbols.psyq*.txt") for l in open(f) if "=" in l}
    # Library and varargs callees take exactly what C passes them; their asm spills every register.
    opaque = lib | {n for d in params.values() for n, (k, _, _) in d.items() if k is None}
    reads = {b: {} for b in funcs}
    for _ in range(30):
        changed = False
        for b, fs in funcs.items():
            visible = dict(reads["main"])
            visible.update(reads[b])
            for name, ins in fs.items():
                r = set() if name in opaque else analyse(ins, visible, None, words[b])
                if r != reads[b].get(name):
                    reads[b][name] = r
                    visible[name] = r
                    changed = True
        if not changed:
            break
    if a.why:
        visible = dict(reads["main"])
        visible.update(reads[a.bin or "main"])
        w = set()
        r = analyse(funcs[a.bin or "main"][a.why], visible, w, words[a.bin or "main"])
        print(f"{a.why} reads {sorted(r)}:")
        print("\n".join(sorted(w)))
        return
    found = 0
    for b in funcs:
        if a.bin and b != a.bin:
            continue
        for name, r in sorted(reads[b].items()):
            p = params[b].get(name)
            if not r or p is None or p[0] is None or name in lib:
                continue
            need = max(r) + 1
            if need > p[0]:
                found += 1
                print(f"DECL {b:7} {name:28} reads arg {need - 1} ({'a%d' % (need - 1) if need <= 4 else 'stack'}) "
                      f"but {p[1]} declares {p[0]}")
        visible = dict(reads["main"])
        visible.update(reads[b])
        for callee, n, where in c_calls(b):
            p = params[b].get(callee)
            if p and p[0] is not None and p[2]:
                n += 1
            r = visible.get(callee)
            if r and max(r) + 1 > n and callee not in lib:
                found += 1
                print(f"CALL {b:7} {where}: {callee}() gets {n} args, reads {max(r) + 1}")
    print(f"{found} findings")


if __name__ == "__main__":
    main()
