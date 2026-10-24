#!/usr/bin/env python3
"""Move function definitions from a scratch batch file into the C files of a binary.

    python3 tools/integrate.py build/batchN.c [func_X ...] [--into src/ovl_0a]

Each definition replaces its INCLUDE_ASM line, in whichever C file of the binary has it
(`--into` takes the binary's src directory or one C file; default src/main). Binaries are
split into one C file per original translation unit where known (docs/source-files.md).
Without names, every function in the batch is moved. Declarations are NOT copied: put
types/externs/prototypes in include/ headers so the batch and the sources share them.
Run make verify afterwards.
"""
import glob
import os
import re
import sys

sys.path.insert(0, "tools")
from resplit import split_c  # noqa: E402

TARGET = "src/main"


def main():
    args = sys.argv[1:]
    target = TARGET
    if "--into" in args:
        i = args.index("--into")
        target = args[i + 1]
        del args[i:i + 2]
    if not args:
        sys.exit(__doc__)
    _, bodies = split_c(open(args[0]).read())
    names = args[1:] or list(bodies)
    files = sorted(glob.glob(os.path.join(target, "*.c"))) if os.path.isdir(target) else [target]
    srcs = {f: open(f).read() for f in files}
    moved, missing, touched = [], [], set()
    for name in names:
        if name not in bodies:
            sys.exit(f"{name} not defined in {sys.argv[1]}")
        pat = re.compile(r'^INCLUDE_(?:ASM|RODATA)\("[^"]+",\s*' + re.escape(name) + r"\);\n", re.M)
        n = 0
        for f in files:
            srcs[f], n = pat.subn(lambda _: bodies[name], srcs[f])
            if n:
                touched.add(f)
                break
        (moved if n == 1 else missing).append(name)
    for f in touched:
        open(f, "w").write(srcs[f])
    print(f"moved {len(moved)} functions into {', '.join(sorted(touched)) or target}")
    if missing:
        sys.exit(f"no INCLUDE_ASM slot for: {missing}")


if __name__ == "__main__":
    main()
