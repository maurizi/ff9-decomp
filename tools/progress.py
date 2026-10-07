#!/usr/bin/env python3
"""Decomp progress for the main executable.

    python3 tools/progress.py            # summary
    python3 tools/progress.py --csv      # also append a row to docs/progress.csv
    python3 tools/progress.py --list [--min N] [--max N] [--range LO-HI]   # remaining game functions by size

A function is "done" when it has no INCLUDE_ASM line in src/. PsyQ library functions
(config/symbols.psyq.us.txt) are counted separately and excluded from the game totals.
Run after a split: sizes come from asm/us/main/nonmatchings/.
"""
import argparse
import datetime
import glob
import os
import re
import subprocess

ASM_GLOB = "asm/us/*/nonmatchings/*/*.s"
SRC_GLOB = "src/**/*.c"
LIB_GLOB = "config/symbols.psyq*.txt"
CSV = "docs/progress.csv"


def functions():
    """{(binary, name): (size, addr)}; binary is the asm/us/<dir> name (main, ovl_0a, ...)."""
    out = {}
    for path in glob.glob(ASM_GLOB):
        text = open(path).read()
        m = re.search(r"^nonmatching (\w+), (0x[0-9A-Fa-f]+)", text, re.M)
        if m:
            addr = re.search(r"/\* [0-9A-F]+ ([0-9A-F]{8}) ", text)
            binary = path.split("/")[2]
            out[(binary, m.group(1))] = (int(m.group(2), 16), int(addr.group(1), 16) if addr else 0)
    return out


def remaining():
    """{(binary, name)} still INCLUDE_ASM'd. The asm path in the macro names the binary."""
    names = set()
    for path in glob.glob(SRC_GLOB, recursive=True):
        for d, n in re.findall(r'^INCLUDE_ASM\("asm/us/(\w+)/[^"]*",\s*(\w+)\);', open(path).read(), re.M):
            names.add((d, n))
    return names


def library():
    names = set()
    for path in glob.glob(LIB_GLOB):
        for line in open(path):
            m = re.match(r"(\w+) = 0x", line)
            if m:
                names.add(m.group(1))
    return names


def main():
    ap = argparse.ArgumentParser()
    ap.add_argument("--csv", action="store_true")
    ap.add_argument("--list", action="store_true")
    ap.add_argument("--min", type=lambda x: int(x, 0), default=0)
    ap.add_argument("--max", type=lambda x: int(x, 0), default=1 << 30)
    ap.add_argument("--range", default="0-0xFFFFFFFF")
    ap.add_argument("--bin", help="restrict to one binary (main, ovl_0a, ...)")
    a = ap.parse_args()

    funcs, todo, lib = functions(), remaining(), library()
    if not funcs:
        raise SystemExit("no asm found; run tools/docker.sh make split first")

    if a.bin:
        funcs = {k: v for k, v in funcs.items() if k[0] == a.bin}

    if a.list:
        lo, hi = (int(x, 0) for x in a.range.split("-"))
        rows = [(b, addr, size, n) for (b, n), (size, addr) in funcs.items()
                if (b, n) in todo and n not in lib and a.min <= size <= a.max and lo <= addr <= hi]
        for b, addr, size, n in sorted(rows):
            print(f"{n:32} 0x{addr:08X} {size:6d} {b}")
        return

    def stats(keys):
        done = [k for k in keys if k not in todo]
        return len(done), len(keys), sum(funcs[k][0] for k in done), sum(funcs[k][0] for k in keys)

    pct = lambda d, t: 100.0 * d / t if t else 0.0
    row = lambda label, s: print(f"{label:10}: {s[0]:5d}/{s[1]:5d} funcs ({pct(s[0], s[1]):5.2f}%)  "
                                 f"{s[2]:7d}/{s[3]:7d} bytes ({pct(s[2], s[3]):5.2f}%)")
    game = [k for k in funcs if k[1] not in lib]
    libs = [k for k in funcs if k[1] in lib]
    for b in sorted({k[0] for k in game}, key=lambda b: (b != "main", b)):
        row(b, stats([k for k in game if k[0] == b]))
    g, l = stats(game), stats(libs)
    print("-" * 70)
    row("game total", g)
    print(f"psyq libs : {l[1]:5d} funcs, {l[3]:7d} bytes (identified by signature, not decompiled)")

    if a.csv:
        commit = subprocess.run(["git", "rev-parse", "--short", "HEAD"], capture_output=True,
                                 text=True).stdout.strip()
        new = not os.path.exists(CSV)
        os.makedirs(os.path.dirname(CSV), exist_ok=True)
        with open(CSV, "a") as f:
            if new:
                f.write("date,commit,game_funcs_done,game_funcs_total,game_bytes_done,game_bytes_total,lib_funcs\n")
            f.write(f"{datetime.date.today()},{commit},{g[0]},{g[1]},{g[2]},{g[3]},{l[1]}\n")


if __name__ == "__main__":
    main()
