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

ASM_GLOB = "asm/us/main/nonmatchings/*/*.s"
SRC_GLOB = "src/**/*.c"
LIB_SYMS = "config/symbols.psyq.us.txt"
CSV = "docs/progress.csv"


def functions():
    out = {}
    for path in glob.glob(ASM_GLOB):
        m = re.search(r"^nonmatching (\w+), (0x[0-9A-Fa-f]+)", open(path).read(), re.M)
        if m:
            addr = re.search(r"/\* [0-9A-F]+ ([0-9A-F]{8}) ", open(path).read())
            out[m.group(1)] = (int(m.group(2), 16), int(addr.group(1), 16) if addr else 0)
    return out


def remaining():
    names = set()
    for path in glob.glob(SRC_GLOB, recursive=True):
        names |= set(re.findall(r'^INCLUDE_ASM\("[^"]+",\s*(\w+)\);', open(path).read(), re.M))
    return names


def library():
    names = set()
    if os.path.exists(LIB_SYMS):
        for line in open(LIB_SYMS):
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
    a = ap.parse_args()

    funcs, todo, lib = functions(), remaining(), library()
    if not funcs:
        raise SystemExit("no asm found; run tools/docker.sh make split first")

    if a.list:
        lo, hi = (int(x, 0) for x in a.range.split("-"))
        rows = [(addr, size, n) for n, (size, addr) in funcs.items()
                if n in todo and n not in lib and a.min <= size <= a.max and lo <= addr <= hi]
        for addr, size, n in sorted(rows):
            print(f"{n:32} 0x{addr:08X} {size:6d}")
        return

    def stats(names):
        done = [n for n in names if n not in todo]
        return len(done), len(names), sum(funcs[n][0] for n in done), sum(funcs[n][0] for n in names)

    game = [n for n in funcs if n not in lib]
    libs = [n for n in funcs if n in lib]
    g, l = stats(game), stats(libs)
    pct = lambda d, t: 100.0 * d / t if t else 0.0
    print(f"game code : {g[0]:5d}/{g[1]:5d} funcs ({pct(g[0], g[1]):5.2f}%)  "
          f"{g[2]:7d}/{g[3]:7d} bytes ({pct(g[2], g[3]):5.2f}%)")
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
