#!/usr/bin/env python3
"""Move function definitions from a scratch batch file into src/main/main.c.

    python3 tools/integrate.py build/batchN.c [func_X ...] [--into src/ovl_0a/ovl_0a.c]

Each definition replaces its INCLUDE_ASM line. Without names, every function in
the batch is moved. Declarations are NOT copied: put types/externs/prototypes in
include/ headers so the batch and main.c share them. Run make verify afterwards.
"""
import re
import sys

sys.path.insert(0, "tools")
from resplit import split_c  # noqa: E402

TARGET = "src/main/main.c"


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
    src = open(target).read()
    moved, missing = [], []
    for name in names:
        if name not in bodies:
            sys.exit(f"{name} not defined in {sys.argv[1]}")
        pat = re.compile(r'^INCLUDE_ASM\("[^"]+",\s*' + re.escape(name) + r"\);\n", re.M)
        src, n = pat.subn(lambda _: bodies[name], src)
        (moved if n == 1 else missing).append(name)
    open(target, "w").write(src)
    print(f"moved {len(moved)} functions into {target}")
    if missing:
        sys.exit(f"no INCLUDE_ASM slot for: {missing}")


if __name__ == "__main__":
    main()
