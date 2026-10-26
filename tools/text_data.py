#!/usr/bin/env python3
"""Find data kept inside .text: words the code reads or writes as data rather than executes.

    tools/docker.sh python3 tools/text_data.py [--bin ovl_0b] [--md docs/text-data.md]

A port that reuses the RAM of an overlay's MIPS code (it never executes it) must keep these bytes. For every
built binary it takes the .text ranges from the link map, then scans the disassembly for addresses formed
with lui + addiu/load/store (%hi/%lo pairs; gp is 0 in this game) that land inside .text:
  - a load or store through %lo(T) is a data access of that width at T;
  - an addiu that forms T is followed (same function, up to 16 instructions, through addu/addiu copies) to the
    loads and stores that use it as a base: data at T + offset, or a table starting at T when an index was
    added first. If the address is only passed on (an argument, stored, or jumped to) it counts as a code
    pointer, unless T is not the start of a function (then it is reported as "address taken").
Words that splat already split out of the code (`textbin` subsegments) are listed from the yamls. The
extent of a table is the size of the symbol that labels it. Data reached only through computed addresses
the scan can't follow would be missed; tools/emu.py `watch` can confirm a range at run time.
"""
import argparse
import glob
import re
import subprocess
from collections import defaultdict

LOADS = {"lb": 1, "lbu": 1, "lh": 2, "lhu": 2, "lw": 4, "lwl": 4, "lwr": 4, "lwc2": 4}
STORES = {"sb": 1, "sh": 2, "sw": 4, "swl": 4, "swr": 4, "swc2": 4}
INS = re.compile(r"^\s*([0-9a-f]+):\s+(\S+)\s*(.*)$")
LABEL = re.compile(r"^([0-9a-f]+) <([\w.]+)>:")
REG = re.compile(r"\b(zero|at|v[01]|a[0-3]|t[0-9]|s[0-8]|k[01]|gp|sp|fp|ra)\b")
MEM = re.compile(r"^(\w+),(-?\d+)\((\w+)\)$")


def binaries():
    out = [("main", "build/us/slus_012.51.elf", "build/us/slus_012.51.map", "config/splat.us.yaml")]
    for elf in sorted(glob.glob("build/us/ovl_*.elf")):
        b = elf.split("/")[-1][:-4]
        out.append((b, elf, elf[:-4] + ".map", f"config/{b}.yaml"))
    for elf in sorted(glob.glob("build/us/fx_*.elf")):
        b = elf.split("/")[-1][:-4]
        out.append((b, elf, elf[:-4] + ".map", f"config/fxmod/{b}.yaml"))
    return out


def text_ranges(mapfile):
    rs = []
    for m in re.finditer(r"^ \.text\s+0x([0-9a-f]+)\s+0x([0-9a-f]+)\s+(\S+)", open(mapfile).read(), re.M):
        a, n = int(m.group(1), 16), int(m.group(2), 16)
        if n and a:
            rs.append((a, a + n, m.group(3)))
    return sorted(rs)


def symbols(elf):
    """address -> (name, size, nm type); text symbols win over linker-script aliases (type A)."""
    out = {}
    for line in subprocess.run(["mipsel-linux-gnu-nm", "-n", "-S", "--defined-only", elf], capture_output=True,
                               text=True).stdout.splitlines():
        p = line.split()
        if len(p) == 4:
            a, entry = int(p[0], 16), (p[3], int(p[1], 16), p[2])
        elif len(p) == 3:
            a, entry = int(p[0], 16), (p[2], 0, p[1])
        else:
            continue
        if entry[0].endswith(".NON_MATCHING"):
            continue
        if a not in out or out[a][2] in "Aa" and entry[2] not in "Aa":
            out[a] = entry
    return out


def regs_rw(mn, ops):
    r = REG.findall(ops)
    if not r:
        return [], []
    if mn in STORES or mn.startswith(("b", "j", "mt", "mult", "div", "ctc", "sw")) or mn in ("jr", "jalr"):
        return r, ([] if mn != "jalr" else ["ra"])
    return r[1:], r[:1]


def scan(b, elf, mapfile):
    ranges = text_ranges(mapfile)
    in_text = lambda a: any(s <= a < e for s, e, _ in ranges)
    dis = subprocess.run(["mipsel-linux-gnu-objdump", "-d", "--no-show-raw-insn", elf], capture_output=True,
                         text=True).stdout
    funcs = []  # [(name, start, [(addr, mn, ops)])]
    for line in dis.splitlines():
        m = LABEL.match(line)
        if m:
            funcs.append((m.group(2), int(m.group(1), 16), []))
            continue
        m = INS.match(line)
        if m and funcs:
            funcs[-1][2].append((int(m.group(1), 16), m.group(2), m.group(3)))
    starts = {s for _, s, ins in funcs if ins}
    hits = defaultdict(set)  # address -> {(kind, size, where)}
    bounds = {}               # start -> end: a loop walks a register from one text address to another
    for name, _start, ins in funcs:
        if not ins or not in_text(ins[0][0]):
            continue
        hi = {}
        formed = {}  # register -> the text address it was formed from (kept through self-increments)
        for i, (addr, mn, ops) in enumerate(ins):
            rd, wr = regs_rw(mn, ops)
            if mn in ("bne", "beq") and len(rd) >= 2 and rd[0] in formed and rd[1] in formed:
                a, e = sorted((formed[rd[0]], formed[rd[1]]))
                if a != e:
                    bounds[a] = e
            self_inc = mn == "addiu" and len(ops.split(",")) == 3 and ops.split(",")[0] == ops.split(",")[1] \
                and ops.split(",")[0] in formed
            if mn == "lui":
                r, v = ops.split(",")
                hi[r] = int(v, 0) << 16
                continue
            mm = MEM.match(ops)
            if mm and mm.group(3) in hi and (mn in LOADS or mn in STORES):
                t = (hi[mm.group(3)] + int(mm.group(2))) & 0xFFFFFFFF
                if in_text(t):
                    hits[t].add(("load" if mn in LOADS else "store", LOADS.get(mn) or STORES.get(mn), f"{name}@{addr:08X}"))
            elif mn == "addiu":
                p = ops.split(",")
                if len(p) == 3 and p[1] in hi:
                    t = (hi[p[1]] + int(p[2], 0)) & 0xFFFFFFFF
                    if in_text(t):
                        follow(t, p[0], ins[i + 1:i + 17], name, hits, starts)
                        hi.pop(p[0], None)
                        formed[p[0]] = t
                        continue
            if not self_inc:
                for r in wr:
                    formed.pop(r, None)
            for r in wr:
                if r in hi and mn != "lui":
                    del hi[r]
    return ranges, hits, starts, bounds


def follow(t, reg, seq, name, hits, starts):
    """the register holds address t (offset known) or t + index (offset None)"""
    track = {reg: 0}
    used = False
    for addr, mn, ops in seq:
        rd, wr = regs_rw(mn, ops)
        mm = MEM.match(ops)
        if mm and mm.group(3) in track and (mn in LOADS or mn in STORES):
            off = track[mm.group(3)]
            kind = "load" if mn in LOADS else "store"
            if off is None:
                hits[t].add(("table", LOADS.get(mn) or STORES.get(mn), f"{name}@{addr:08X}"))
            else:
                hits[t + off + int(mm.group(2))].add((kind, LOADS.get(mn) or STORES.get(mn), f"{name}@{addr:08X}"))
            used = True
        elif mn in ("jr", "jalr") and rd and rd[0] in track:
            hits[t].add(("computed jump (code)", 0, f"{name}@{addr:08X}"))
            used = True
            break
        elif mn in STORES and mm and mm.group(1) in track:
            hits[t].add(("pointer stored (code)" if track[mm.group(1)] == 0 else "pointer stored", 0,
                         f"{name}@{addr:08X}"))
            used = True
            continue
        elif mn in ("addu", "subu") and any(r in track for r in rd):
            track[wr[0]] = None
            continue
        elif mn == "addiu" and rd and rd[0] in track and wr:
            o = track[rd[0]]
            track[wr[0]] = None if o is None else o + int(ops.split(",")[2], 0)
            continue
        elif mn == "move" and rd and rd[0] in track:
            track[wr[0]] = track[rd[0]]
            continue
        for r in wr:
            track.pop(r, None)
        if mn in ("jal", "jalr", "jr", "j") or mn.startswith("b"):
            # one delay slot more, then stop following
            pass
        if not track:
            break
    if not used and t not in starts:
        hits[t].add(("address taken (code?)", 0, name))


def textbins(yaml, ranges, syms):
    out = []
    if not glob.glob(yaml):
        return out
    rows = re.findall(r"^\s*- \[0x([0-9A-Fa-f]+),\s*(\w+)", open(yaml).read(), re.M)
    vram = re.search(r"vram:\s*0x([0-9A-Fa-f]+)", open(yaml).read())
    base = int(vram.group(1), 16) if vram else 0
    start_off = re.search(r"start:\s*0x([0-9A-Fa-f]+)", open(yaml).read())
    s0 = int(start_off.group(1), 16) if start_off else 0
    for k, (off, kind) in enumerate(rows):
        if kind == "textbin":
            a = base + int(off, 16) - s0
            e = base + int(rows[k + 1][0], 16) - s0 if k + 1 < len(rows) else a
            out.append((a, e, syms.get(a, ("?", 0, ""))[0]))
    return out


DATA_KINDS = ("load", "store", "table")


def main():
    ap = argparse.ArgumentParser()
    ap.add_argument("--bin")
    ap.add_argument("--code", action="store_true", help="also list code addresses taken (callbacks, computed jumps)")
    ap.add_argument("--md", help="write the markdown table to this file (between its text_data.py markers)")
    args = ap.parse_args()
    rows = []
    for b, elf, mapfile, yaml in binaries():
        if args.bin and b != args.bin:
            continue
        ranges, hits, starts, bounds = scan(b, elf, mapfile)
        syms = symbols(elf)
        bins = textbins(yaml, ranges, syms)
        for a, e, name in bins:
            rows.append((b, a, e, name, "textbin (split out of the code)", ""))
        covered = lambda t: any(a <= t < e for a, e, _ in bins)
        for t in sorted(hits):
            kinds = {k[0] for k in hits[t]}
            data = any(k in DATA_KINDS for k in kinds) or t in bounds
            if covered(t) or (not data and not args.code):
                continue
            if t in bounds and any(a < t < e for a, e in bounds.items()):
                continue  # the end of one copy range is the start of the next one only when both are read
            name, size, typ = syms.get(t, (None, 0, ""))
            if "table" in kinds and typ in ("A", "a", "") and not kinds & {"load", "store"} and t not in bounds:
                # an index base the compiler folded (&tbl[i - k]), not a table at this address
                if not args.code:
                    continue
                kinds = {"folded index base (not data)"}
            if name is None:
                prev = max((x for x in syms if x <= t), default=None)
                name = f"{syms[prev][0]}+0x{t - prev:X}" if prev is not None else f"0x{t:08X}"
            width = max(k[1] for k in hits[t]) or 4
            if t in bounds:
                end, kinds = bounds[t], kinds | {"copy/compare loop"}
            elif "table" in kinds and size:
                end = t + size
            else:
                end = t + width
            where = sorted({k[2] for k in hits[t]})
            rows.append((b, t, end, name, ", ".join(sorted(k for k in kinds if args.code or k in DATA_KINDS
                                                          or k == "copy/compare loop" or "folded" in k)),
                         " ".join(where[:4]) + (" ..." if len(where) > 4 else "")))
    for b, a, e, name, kind, where in rows:
        print(f"{b:7} {a:08X}-{e:08X} {name:28} {kind:30} {where}")
    print(f"{len(rows)} ranges")
    if args.md:
        write_md(args.md, rows)


def write_md(path, rows):
    s = open(path).read() if glob.glob(path) else ""
    table = ["| binary | range | symbol | how it's used | accessed by |", "|---|---|---|---|---|"]
    for b, a, e, name, kind, where in rows:
        table.append(f"| {b} | 0x{a:08X}-0x{e:08X} | `{name}` | {kind} | {where} |")
    begin, end = "<!-- text_data.py begin -->", "<!-- text_data.py end -->"
    block = begin + "\n" + "\n".join(table) + "\n" + end
    if begin in s:
        s = s[:s.index(begin)] + block + s[s.index(end) + len(end):]
    else:
        s = s.rstrip("\n") + "\n\n" + block + "\n"
    open(path, "w").write(s)


if __name__ == "__main__":
    main()
