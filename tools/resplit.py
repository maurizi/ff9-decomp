#!/usr/bin/env python3
"""Re-run splat without losing decompiled C.

splat never rewrites an existing .c file, so after symbol/boundary changes the
INCLUDE_ASM list in src/**/*.c goes stale. Usage: resplit.py [yaml ...] This moves each C file aside, lets
splat regenerate it, then merges back: the old preamble (everything before the
first INCLUDE_ASM or function), and every decompiled function body replacing
its INCLUDE_ASM line. Run inside the container: tools/docker.sh make resplit
"""
import glob
import os
import re
import subprocess
import sys

DEFAULT_YAMLS = ["config/slus_012.51.yaml"]

FUNC_DEF = re.compile(r"^[A-Za-z_][^;{}()]*?\b([A-Za-z_]\w*)\s*\([^;{}]*\)\s*\{", re.M)
INCLUDE = re.compile(r'^INCLUDE_(?:ASM|RODATA)\("[^"]+",\s*(\w+)\);\n', re.M)
# Data converted from INCLUDE_RODATA to C, e.g. `const u8 D_801EDF84[0x1260] = {...};`
DATA_DEF = re.compile(r"^(?:static\s+)?(?:const\s+)?[A-Za-z_][\w \*]*?\b(D_[0-9A-F]{8})\b[^;={(]*=\s*\{", re.M)
LEADING_COMMENT = re.compile(r"(?:^(?:/\*.*\*/|//.*)\n)+\Z", re.M)


def _with_comment(text, start):
    """Extend start backwards over comment lines directly above a definition."""
    m = LEADING_COMMENT.search(text, 0, start)
    return m.start() if m and m.end() == start else start


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
        bodies[m.group(1)] = text[_with_comment(text, m.start()):end.end()]
    for m in DATA_DEF.finditer(text):
        if m.start() < first:
            continue  # declarations in the preamble stay there
        end = re.compile(r"^\};\n", re.M).search(text, m.end())
        if not end:
            sys.exit(f"unterminated data definition {m.group(1)}")
        bodies[m.group(1)] = text[_with_comment(text, m.start()):end.end()]
    # Rodata that splat migrated into a function's asm has no slot of its own; keep it
    # glued to the function that directly follows it.
    for name in [n for n in bodies if n.startswith("D_")]:
        data = bodies[name]
        after = text[text.index(data) + len(data):]
        nxt = FUNC_DEF.match(after.lstrip("\n"))
        if nxt and nxt.group(1) in bodies:
            bodies[nxt.group(1)] = data + "\n" + bodies[nxt.group(1)]
            del bodies[name]
    return preamble, bodies


EMPTY_BODY = re.compile(r"\{\s*\}\n?$")


def main():
    yamls = sys.argv[1:] or DEFAULT_YAMLS
    stale = glob.glob("src/**/*.bak", recursive=True)
    if stale:
        sys.exit(f"refusing to run: leftover backups {stale}; resolve them first")

    old = {}
    for path in glob.glob("src/**/*.c", recursive=True):
        old[path] = open(path).read()
        os.rename(path, path + ".bak")
    try:
        subprocess.run(["rm", "-rf", "asm/us"], check=True)
        for y in yamls:
            subprocess.run([sys.executable, "-m", "splat", "split", y], check=True)
    except BaseException:
        for path in old:
            os.replace(path + ".bak", path)
        raise

    # Fresh splat output, with each old file's preamble restored.
    new = {}
    for path in glob.glob("src/**/*.c", recursive=True):
        text = open(path).read()
        if path in old:
            pre, _ = split_c(old[path])
            new_pre, _ = split_c(text)
            text = pre + text[len(new_pre):]
        new[path] = text

    # Put every hand-written body back over its INCLUDE_ASM slot, in whichever file it now lives.
    # Empty bodies are skipped: splat regenerates trivial functions as C itself.
    missing, kept = [], {}
    for path, text in old.items():
        _, bodies = split_c(text)
        for name, body in bodies.items():
            if EMPTY_BODY.search(body):
                continue
            pat = re.compile(r'^INCLUDE_(?:ASM|RODATA)\("[^"]+",\s*' + re.escape(name) + r"\);\n", re.M)
            # Only files of the same binary: overlays at the same address reuse names.
            for npath in [p for p in new if os.path.dirname(p) == os.path.dirname(path)]:
                new[npath], n = pat.subn(lambda _: body, new[npath])
                if n:
                    kept[npath] = kept.get(npath, 0) + 1
                    break
            else:
                missing.append((path, name))

    for path, text in new.items():
        open(path, "w").write(text)
    for path, n in sorted(kept.items()):
        print(f"{path}: kept {n} decompiled functions")
    if missing:
        sys.exit(f"decompiled functions with no INCLUDE_ASM slot after split (renamed or merged?): "
                 f"{missing}. Old files kept as *.bak; merge by hand, then delete the backups.")
    for path in old:
        os.remove(path + ".bak")

if __name__ == "__main__":
    main()
