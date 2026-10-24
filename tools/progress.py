#!/usr/bin/env python3
"""Decomp progress for the main executable.

    python3 tools/progress.py            # summary
    python3 tools/progress.py --csv      # also append a row to docs/progress.csv
    python3 tools/progress.py --list [--min N] [--max N] [--range LO-HI]   # remaining game functions by size
    python3 tools/progress.py --unnamed [--bin B]   # unnamed functions/globals, most-referenced first

Naming progress: a game function is "named" once it no longer has its func_XXXXXXXX placeholder,
a global once its D_XXXXXXXX is gone from src/ and include/ (see tools/rename.py).

A function is "done" when it has no INCLUDE_ASM line in src/. PsyQ library functions
(config/symbols.psyq.us.txt) are counted separately and excluded from the game totals.
Run after a split: sizes come from asm/us/main/nonmatchings/.

The battle effect modules (fx_XXX binaries, FF9.IMG dir 13; docs/effect-modules.md) are reported in a
block of their own and are not part of the game totals or docs/progress.csv. Their functions are indexed
in config/functions.fxmod.us.txt.
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
NAMING_CSV = "docs/naming.csv"
PLACEHOLDER_FUNC = re.compile(r"^func_[0-9A-F]{8}$")
PLACEHOLDER_REF = re.compile(r"\b(func|D)_([0-9A-F]{8})\b")
MAIN_END = 0x800A7000  # main exe (code, data, bss) lies below the overlay area


INDEX = "config/functions.us.txt"
FX_INDEX = "config/functions.fxmod.us.txt"


def is_fx(binary):
    return binary.startswith("fx_")


def functions():
    """{(binary, name): (size, addr)} for every function. Read from the committed indexes
    (written by --write-index after a full split), because a plain split only emits asm
    for functions still INCLUDE_ASM'd."""
    return functions_indexed() if os.path.exists(INDEX) else functions_from_asm()


def functions_indexed():
    out = {}
    for path in (INDEX, FX_INDEX):
        if not os.path.exists(path):
            continue
        for line in open(path):
            f = line.split()
            if len(f) == 4 and not line.startswith("#"):
                out[(f[0], f[1])] = (int(f[2], 16), int(f[3], 16))
    return out


def functions_from_asm():
    out = {}
    for path in glob.glob(ASM_GLOB):
        text = open(path).read()
        m = re.search(r"^nonmatching (\w+), (0x[0-9A-Fa-f]+)", text, re.M)
        if m:
            # Search after the text `nonmatching` line: rodata that migrated into the file (jump tables,
            # strings) comes first and would give the rodata address.
            addr = re.search(r"/\* [0-9A-F]+ ([0-9A-F]{8}) ", text[m.end():])
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


LIB_LIST = "config/library_funcs.us.txt"


def library():
    """Names excluded from game totals: PsyQ signature matches plus the hand-made list."""
    names = set()
    for path in glob.glob(LIB_GLOB):
        for line in open(path):
            m = re.match(r"(\w+) = 0x", line)
            if m:
                names.add(m.group(1))
    if os.path.exists(LIB_LIST):
        for line in open(LIB_LIST):
            f = line.split("#")[0].split()
            if len(f) >= 2:
                names.add(f[1])
    return names


def placeholder_refs():
    """{(binary, name): reference count} for func_/D_ placeholders in src/ and include/.
    Addresses below MAIN_END belong to main whichever file mentions them."""
    refs = {}
    for path in glob.glob(SRC_GLOB, recursive=True) + glob.glob("include/*.h"):
        m = re.match(r"(?:src|include)/(?:fxmod/)?(ovl_[0-9a-f]{2}|fx_[0-9a-f]{3}(?:_\d)?)", path)
        local = m.group(1) if m else "main"
        for kind, addr in PLACEHOLDER_REF.findall(open(path).read()):
            b = "main" if int(addr, 16) < MAIN_END else local
            key = (b, f"{kind}_{addr}")
            refs[key] = refs.get(key, 0) + 1
    return refs


def unknown_fields():
    """unkXX struct fields still unnamed in include/ (declarations only)."""
    return sum(len(re.findall(r"^\s+[\w ]+[\s*]unk[0-9A-Fa-f]+\b", open(p).read(), re.M))
               for p in glob.glob("include/*.h"))


def named_fields():
    return sum(len(re.findall(r"^\s+(?:struct\s+)?\w[\w ]*[\s*]+(?!unk[0-9A-Fa-f]+\b|pad)[a-z_A-Z]\w*\s*(?:\[[^]]*\])*(?::\s*\d+)?;",
                              open(p).read(), re.M))
               for p in glob.glob("include/*.h"))


def naming(funcs, lib):
    """Per-binary [named funcs, game funcs, named globals, globals]."""
    refs = placeholder_refs()
    out = {}
    for (b, n) in funcs:
        if n in lib:
            continue
        r = out.setdefault(b, [0, 0, 0, 0])
        r[1] += 1
        r[0] += not PLACEHOLDER_FUNC.match(n)
    for path in glob.glob("config/symbols*.txt"):
        if "psyq" in path:
            continue
        m = re.search(r"symbols\.(ovl_[0-9a-f]{2})\.", path)
        for line in open(path):
            sm = re.match(r"(\w+) = 0x([0-9A-Fa-f]+);", line)
            if sm and "type:func" not in line and not re.match(r"(D|func)_[0-9A-F]{8}$", sm.group(1)):
                b = m.group(1) if m else "main"
                out.setdefault(b, [0, 0, 0, 0])[2] += 1
    for (b, n) in refs:
        if n.startswith("D_") and not is_fx(b):
            out.setdefault(b, [0, 0, 0, 0])[3] += 1
    for r in out.values():
        r[3] += r[2]
    return out


def main():
    ap = argparse.ArgumentParser()
    ap.add_argument("--csv", action="store_true")
    ap.add_argument("--list", action="store_true")
    ap.add_argument("--min", type=lambda x: int(x, 0), default=0)
    ap.add_argument("--max", type=lambda x: int(x, 0), default=1 << 30)
    ap.add_argument("--range", default="0-0xFFFFFFFF")
    ap.add_argument("--unnamed", action="store_true")
    ap.add_argument("--bin", help="restrict to one binary (main, ovl_0a, ...)")
    ap.add_argument("--write-index", action="store_true",
                    help="rewrite config/functions.us.txt from asm/ (needs a FULL split: make resplit)")
    ap.add_argument("--force", action="store_true", help="--write-index even if asm/ lacks indexed functions")
    a = ap.parse_args()

    if a.write_index:
        funcs = functions_from_asm()
        # A plain split only writes asm for functions still INCLUDE_ASM'd, so indexed functions that are
        # missing from asm/ but defined in C mean the split wasn't full. (Functions that are gone from
        # both, e.g. merged by a boundary change, may drop out.)
        have = {b for b, _ in funcs}
        lost, src = [], {}
        for (b, n) in functions_indexed():
            if b in have and (b, n) not in funcs:
                if b not in src:
                    src[b] = "".join(open(f).read() for f in glob.glob(f"src/**/{b}/*.c", recursive=True))
                if re.search(r"\b" + re.escape(n) + r"\s*\(", src[b]):
                    lost.append(f"{b}:{n}")
        if lost and not a.force:
            raise SystemExit(f"refusing to write the index: {len(lost)} indexed functions defined in C have no asm "
                             f"(e.g. {', '.join(lost[:5])}), so asm/ comes from a plain split. Run "
                             f"tools/docker.sh make resplit first, or pass --force if they really are gone.")
        for path, keep in ((INDEX, lambda b: not is_fx(b)), (FX_INDEX, is_fx)):
            sel = {k: v for k, v in funcs.items() if keep(k[0])}
            if path == FX_INDEX and not sel:
                continue  # effect modules not extracted: keep the committed index
            with open(path, "w") as f:
                f.write("# binary function size addr -- every function, from a full split; tools/progress.py --write-index\n")
                for (b, n), (size, addr) in sorted(sel.items(), key=lambda kv: (kv[0][0], kv[1][1])):
                    f.write(f"{b} {n} {size:#x} {addr:#010x}\n")
            print(f"wrote {len(sel)} functions to {path}")
        return

    funcs, todo, lib = functions(), remaining(), library()
    if not funcs:
        raise SystemExit("no asm found; run tools/docker.sh make split first")

    if a.bin:
        funcs = {k: v for k, v in funcs.items() if k[0] == a.bin}

    if a.unnamed:
        refs = placeholder_refs()
        rows = sorted(((c, b, n) for (b, n), c in refs.items() if not a.bin or b == a.bin), reverse=True)
        for c, b, n in rows[:200]:
            print(f"{n:14} {b:7} {c:5d} refs")
        return

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
    fx = [k for k in funcs if is_fx(k[0])]
    funcs_all = funcs
    funcs = {k: v for k, v in funcs.items() if not is_fx(k[0])}
    game = [k for k in funcs if k[1] not in lib]
    libs = [k for k in funcs if k[1] in lib]
    for b in sorted({k[0] for k in game}, key=lambda b: (b != "main", b)):
        row(b, stats([k for k in game if k[0] == b]))
    g, l = stats(game), stats(libs)
    print("-" * 70)
    row("game total", g)
    print(f"library   : {l[1]:5d} funcs, {l[3]:7d} bytes (PsyQ signatures + config/library_funcs.us.txt; not decompiled)")

    if fx:
        # Effect modules: separate binaries, not in the totals above (docs/effect-modules.md).
        funcs = funcs_all
        mods = sorted({k[0] for k in fx})
        full = [b for b in mods if all(k not in todo for k in fx if k[0] == b)]
        print(f"\neffect modules ({len(mods)} binaries, {len(full)} fully decompiled):")
        for b in mods:
            s = stats([k for k in fx if k[0] == b])
            if s[0]:
                row(b, s)
        print("-" * 70)
        row("fx total", stats(fx))
        funcs = {k: v for k, v in funcs.items() if not is_fx(k[0])}

    nm = naming(funcs, lib)
    print("\nnaming    :  funcs named         globals named (D_ still referenced from C counts as unnamed)")
    for b in sorted(nm, key=lambda b: (b != "main", b)):
        r = nm[b]
        print(f"{b:10}: {r[0]:5d}/{r[1]:5d} ({pct(r[0], r[1]):5.2f}%)  {r[2]:5d}/{r[3]:5d} ({pct(r[2], r[3]):5.2f}%)")
    t = [sum(r[i] for r in nm.values()) for i in range(4)]
    uf, nf = unknown_fields(), named_fields()
    print("-" * 70)
    print(f"{'total':10}: {t[0]:5d}/{t[1]:5d} ({pct(t[0], t[1]):5.2f}%)  {t[2]:5d}/{t[3]:5d} ({pct(t[2], t[3]):5.2f}%)")
    print(f"struct fields: {nf} named, {uf} still unkXX")

    if a.csv:
        commit = subprocess.run(["git", "rev-parse", "--short", "HEAD"], capture_output=True,
                                 text=True).stdout.strip()
        new = not os.path.exists(CSV)
        os.makedirs(os.path.dirname(CSV), exist_ok=True)
        with open(CSV, "a") as f:
            if new:
                f.write("date,commit,game_funcs_done,game_funcs_total,game_bytes_done,game_bytes_total,lib_funcs\n")
            f.write(f"{datetime.date.today()},{commit},{g[0]},{g[1]},{g[2]},{g[3]},{l[1]}\n")
        new = not os.path.exists(NAMING_CSV)
        with open(NAMING_CSV, "a") as f:
            if new:
                f.write("date,commit,funcs_named,funcs_total,globals_named,globals_total,fields_named,fields_unk\n")
            f.write(f"{datetime.date.today()},{commit},{t[0]},{t[1]},{t[2]},{t[3]},{nf},{uf}\n")


if __name__ == "__main__":
    main()
