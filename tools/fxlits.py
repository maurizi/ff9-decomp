#!/usr/bin/env python3
"""Per-module literal counts for the battle effect modules (src/fxmod/fx_XXX/*.c).

    python3 tools/fxlits.py fx_006 fx_03e          # actionable literal counts per module
    python3 tools/fxlits.py --pilot                 # the 20 pilot modules (docs/fxdata.md) + total
    python3 tools/fxlits.py --all                   # every module, and the total
    python3 tools/fxlits.py fx_006 --list           # every actionable literal with its line
    python3 tools/fxlits.py fx_006 --list --key btlfx_drawScrollMove#1

Counts use tools/literal_survey.py's classifier (same noise rules: 0, 1, -1, 2 and full-byte masks skipped,
#define bodies and comments ignored), restricted to the actionable contexts compare / mask / case / arg / assign,
so the numbers add up to literal_survey.py --bin fxmod. The choreography-literal scheme these counts track is
docs/fxdata.md, "Choreography literals (pilot)".
"""
import argparse
import collections
import glob
import os
import re
import sys

sys.path.insert(0, os.path.dirname(os.path.abspath(__file__)))
import literal_survey as ls  # noqa: E402

ROOT = ls.ROOT
ACTIONABLE = ("compare", "mask", "case", "arg", "assign")
PILOT = ["fx_000", "fx_046", "fx_002", "fx_047", "fx_007", "fx_048", "fx_006", "fx_03e", "fx_004", "fx_04d",
         "fx_097", "fx_098", "fx_003", "fx_042", "fx_005", "fx_049", "fx_018", "fx_085", "fx_008", "fx_009"]


def scan(path):
    """Yield (lineno, ctx, key, value, source line) for every counted literal of one C file."""
    fn, in_comment, in_define = None, False, False
    for no, line in enumerate(open(path, errors="replace"), 1):
        raw = line
        if in_comment:
            if "*/" not in line:
                continue
            line = line.split("*/", 1)[1]
            in_comment = False
        line = re.sub(r"/\*.*?\*/", "", line)
        if "/*" in line:
            line, in_comment = line.split("/*", 1)[0], True
        if in_define or line.lstrip().startswith("#"):
            in_define = raw.rstrip().endswith("\\")
            continue
        if "INCLUDE_ASM" in line or "INCLUDE_RODATA" in line or "__asm__" in line:
            continue
        fm = ls.FUNC.match(line)
        if fm and fm.group(1) not in ls.KEYWORDS:
            fn = fm.group(1)
        line = ls.strip(line)
        for m in ls.NUM.finditer(line):
            v = m.group(1)
            if v in ls.NOISE:
                continue
            if line[:m.start()].rstrip().endswith("-") and v in ("1",):
                continue
            ctx, key = ls.classify(line, m)
            yield no, ctx, key or fn or "?", v, raw.rstrip()


def module_files(mod):
    return sorted(glob.glob(f"{ROOT}/src/fxmod/{mod}/*.c"))


def main():
    ap = argparse.ArgumentParser(description=__doc__, formatter_class=argparse.RawDescriptionHelpFormatter)
    ap.add_argument("modules", nargs="*")
    ap.add_argument("--pilot", action="store_true", help="the 20 pilot modules")
    ap.add_argument("--all", action="store_true", help="every module")
    ap.add_argument("--list", action="store_true", help="print each actionable literal")
    ap.add_argument("--key", help="with --list: only this context key (callee#index, field name, ...)")
    a = ap.parse_args()
    mods = list(a.modules)
    if a.pilot:
        mods += PILOT
    if a.all:
        mods += sorted(os.path.basename(d) for d in glob.glob(f"{ROOT}/src/fxmod/fx_*"))
    if not mods:
        ap.error("name modules, or --pilot / --all")
    total = collections.Counter()
    print(f"{'module':10}" + "".join(f"{c:>8}" for c in ACTIONABLE) + f"{'total':>8}")
    for mod in mods:
        c = collections.Counter()
        for path in module_files(mod):
            for no, ctx, key, v, src in scan(path):
                if ctx not in ACTIONABLE:
                    continue
                c[ctx] += 1
                if a.list and (not a.key or key == a.key):
                    print(f"  {os.path.basename(path)}:{no:<5} {ctx:7} {key:32} {v:>8}  {src.strip()[:110]}")
        total.update(c)
        print(f"{mod:10}" + "".join(f"{c[x]:8d}" for x in ACTIONABLE) + f"{sum(c.values()):8d}")
    if len(mods) > 1:
        print(f"{'total':10}" + "".join(f"{total[x]:8d}" for x in ACTIONABLE) + f"{sum(total.values()):8d}")


if __name__ == "__main__":
    main()
