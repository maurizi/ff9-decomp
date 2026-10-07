#!/usr/bin/env python3
"""Re-run splat without losing decompiled C.

splat never rewrites an existing .c file, so after symbol/boundary changes the
INCLUDE_ASM list in src/main/*.c goes stale. This moves each C file aside, lets
splat regenerate it, then merges back: the old preamble (everything before the
first INCLUDE_ASM or function), and every decompiled function body replacing
its INCLUDE_ASM line. Run inside the container: tools/docker.sh make resplit
"""
import os
import re
import subprocess
import sys

SRC_DIR = "src/main"
YAML = "config/slus_012.51.yaml"

FUNC_DEF = re.compile(r"^[A-Za-z_][^;{}()]*?\b([A-Za-z_]\w*)\s*\([^;{}]*\)\s*\{", re.M)
INCLUDE = re.compile(r'^INCLUDE_(?:ASM|RODATA)\("[^"]+",\s*(\w+)\);\n', re.M)


def split_c(text):
    """Return (preamble, {name: body}) for a C file."""
    starts = [m.start() for m in INCLUDE.finditer(text)] + [m.start() for m in FUNC_DEF.finditer(text)]
    first = min(starts) if starts else len(text)
    preamble = text[:first]
    bodies = {}
    for m in FUNC_DEF.finditer(text):
        end = re.compile(r"^\}\n", re.M).search(text, m.end())
        if not end:
            sys.exit(f"unterminated function {m.group(1)}")
        bodies[m.group(1)] = text[m.start():end.end()]
    return preamble, bodies


def main():
    old = {}
    for fn in os.listdir(SRC_DIR):
        if fn.endswith(".c"):
            path = os.path.join(SRC_DIR, fn)
            old[path] = open(path).read()
            os.rename(path, path + ".bak")
    try:
        subprocess.run(["rm", "-rf", "asm/us"], check=True)
        subprocess.run([sys.executable, "-m", "splat", "split", YAML], check=True)
    except BaseException:
        for path in old:
            os.replace(path + ".bak", path)
        raise
    for path, text in old.items():
        preamble, bodies = split_c(text)
        if not os.path.exists(path):
            sys.exit(f"splat no longer generates {path}; old copy kept at {path}.bak")
        new = open(path).read()
        new_pre, _ = split_c(new)
        new = preamble + new[len(new_pre):]
        missing = []
        _, generated = split_c(new)  # splat emits trivial (empty) functions as C itself
        for name, body in bodies.items():
            if name in generated:
                continue
            pat = re.compile(r'^INCLUDE_ASM\("[^"]+",\s*' + re.escape(name) + r"\);\n", re.M)
            new, n = pat.subn(lambda _: body, new)
            if n != 1:
                missing.append(name)
        open(path, "w").write(new)
        if missing:
            sys.exit(f"{path}: decompiled functions with no INCLUDE_ASM slot after split "
                     f"(renamed or merged?): {missing}; old copy at {path}.bak")
        os.remove(path + ".bak")
        print(f"{path}: kept {len(bodies) - len(generated)} decompiled functions")


if __name__ == "__main__":
    main()
