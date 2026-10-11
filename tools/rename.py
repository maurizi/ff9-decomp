#!/usr/bin/env python3
"""Give func_XXXXXXXX / D_XXXXXXXX placeholders real names.

    python3 tools/rename.py func_80052D40 rsin D_80077170 g_primPtr   # pairs
    python3 tools/rename.py --from build/names.txt                    # "OLD NEW" per line, # comments
    python3 tools/rename.py --bin ovl_0b D_800F78A4 g_btl ...         # overlay symbols

For each pair it records the name in the binary's symbols file (config/symbols.us.txt or
config/symbols.ovl_XX.us.txt), renames every use in the C sources and headers that can see the
symbol (main: all of src/ and include/; an overlay: src/ovl_XX/ and include/ovl_XX.h), and updates
config/functions.us.txt. File-local helpers named after a function (func_80047E1C_approach) follow it.
A trailing comment in --from lines (`OLD NEW  # why`) goes into the symbols file.

No resplit is needed: names don't move function boundaries. Afterwards run
`tools/docker.sh make split` (regenerates asm/ with the new names, never touches src/)
and `tools/docker.sh make -j8 verify`; --build does both.
"""
import argparse
import glob
import os
import re
import subprocess
import sys

MAIN_END = 0x800A7000
PLACEHOLDER = re.compile(r"^(func|D)_([0-9A-F]{8})$")
STRING = re.compile(r'"(?:[^"\\\n]|\\.)*"')
COMMENT = re.compile(r"/\*.*?\*/|//[^\n]*", re.S)
IDENT = re.compile(r"^[A-Za-z_]\w*$")
INDEX = "config/functions.us.txt"
LIB_LIST = "config/library_funcs.us.txt"


def scope_files(binary):
    if binary == "main":
        return glob.glob("src/**/*.c", recursive=True) + glob.glob("include/*.h")
    return glob.glob(f"src/{binary}/*.c") + glob.glob(f"include/{binary}.h")


def symbols_file(binary):
    return "config/symbols.us.txt" if binary == "main" else f"config/symbols.{binary}.us.txt"


def guess_binary(old, addr):
    if addr < MAIN_END:
        return "main"
    word = re.compile(rf"\b{old}\b")
    hits = {p.split("/")[1] for p in glob.glob("src/ovl_*/*.c") if word.search(open(p).read())}
    hits |= {os.path.basename(p)[:-2] for p in glob.glob("include/ovl_*.h") if word.search(open(p).read())}
    if len(hits) != 1:
        sys.exit(f"{old}: used in {sorted(hits) or 'no overlay'}; pass --bin")
    return hits.pop()


def taken(name, binary):
    word = re.compile(rf"\b{re.escape(name)}\b")
    files = scope_files(binary) + [symbols_file(binary), "config/symbols.us.txt",
                                   "config/symbols.psyq.us.txt"]
    if binary == "main":  # a main name must not shadow anything an overlay declares
        files += glob.glob("config/symbols.*.us.txt")
    # Strings and comments don't count: a function's own error message often spells its name.
    return any(os.path.exists(f) and word.search(COMMENT.sub(" ", STRING.sub('""', open(f).read())))
               for f in set(files))


def main():
    ap = argparse.ArgumentParser(description=__doc__, formatter_class=argparse.RawDescriptionHelpFormatter)
    ap.add_argument("pairs", nargs="*")
    ap.add_argument("--from", dest="src")
    ap.add_argument("--bin")
    ap.add_argument("--force", action="store_true",
                    help="allow a name that already appears (e.g. a psyq.h prototype for a BIOS stub)")
    ap.add_argument("--build", action="store_true", help="then make split and make verify (in Docker)")
    a = ap.parse_args()

    jobs = []  # (old, new, comment)
    if a.src:
        for line in open(a.src):
            body, _, comment = line.partition("#")
            f = body.split()
            if f:
                if len(f) != 2:
                    sys.exit(f"bad line: {line.strip()}")
                jobs.append((f[0], f[1], comment.strip()))
    if len(a.pairs) % 2:
        sys.exit("names come in OLD NEW pairs")
    jobs += [(a.pairs[i], a.pairs[i + 1], "") for i in range(0, len(a.pairs), 2)]
    if not jobs:
        ap.print_help()
        return

    news = [n for _, n, _ in jobs]
    if len(set(news)) != len(news):
        sys.exit("duplicate new names")
    plan = []
    for old, new, comment in jobs:
        m = PLACEHOLDER.match(old)
        if not m:
            sys.exit(f"{old}: only func_/D_ placeholders can be renamed (rename real names by hand)")
        if not IDENT.match(new) or PLACEHOLDER.match(new):
            sys.exit(f"{new}: not a usable name")
        addr = int(m.group(2), 16)
        binary = a.bin or guess_binary(old, addr)
        if binary != "main" and addr < MAIN_END:
            sys.exit(f"{old} is a main-exe address; drop --bin for it")
        if not a.force and taken(new, binary):
            sys.exit(f"{new}: already used in {binary}'s scope")
        plan.append((old, new, comment, m.group(1) == "func", addr, binary))

    for old, new, comment, is_func, addr, binary in plan:
        # Symbols file: rename an existing entry in place (keeping its comment), else append.
        path = symbols_file(binary)
        lines = open(path).read().splitlines() if os.path.exists(path) else []
        entry = re.compile(rf"^{old}\s*=")
        for i, line in enumerate(lines):
            if entry.match(line):
                lines[i] = entry.sub(f"{new} =", line)
                break
        else:
            # splat reads `key:value` pairs from these comments, so a free-text comment loses its colons (and semicolons).
            tags = [t for t in ("type:func" if is_func else "", comment.replace(":", "").replace(";", ",")) if t]
            lines.append(f"{new} = 0x{addr:08X};" + (" // " + " ".join(tags) if tags else ""))
        open(path, "w").write("\n".join(lines) + "\n")

        word = re.compile(rf"\b{old}(_\w+)?\b")
        changed = 0
        for f in scope_files(binary):
            text = open(f).read()
            out, n = word.subn(lambda mm: new + (mm.group(1) or ""), text)
            if n:
                open(f, "w").write(out)
                changed += n

        if is_func:
            for path in (INDEX, LIB_LIST):
                idx = open(path).read()
                idx = re.sub(rf"^{binary} {old} ", f"{binary} {new} ", idx, flags=re.M)
                open(path, "w").write(idx)
        print(f"{binary:7} {old} -> {new}  ({changed} uses)")

    if a.build:
        for cmd in (["tools/docker.sh", "make", "split"], ["tools/docker.sh", "make", "-j8", "verify"]):
            r = subprocess.run(cmd, capture_output=True, text=True)
            if r.returncode:
                sys.exit(f"{' '.join(cmd)} failed:\n{r.stdout[-3000:]}{r.stderr[-3000:]}")
        print(r.stdout.count(": OK"), "binaries OK")


if __name__ == "__main__":
    main()
