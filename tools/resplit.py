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

# ANSI or K&R definitions (no `:`, so a `label:` above an `if (...) {` is not one) (`f(a, b)\n    s32 a;\n    u16 b;\n{`).
FUNC_DEF = re.compile(
    r"^[A-Za-z_][^;{}():]*?\b([A-Za-z_]\w*)\s*\([^;{}]*\)\s*(?:[A-Za-z_][^;{}()]*;\s*)*\{", re.M)
ASM_SLOT = re.compile(r'^INCLUDE_ASM\("[^"]+",\s*(\w+)\);', re.M)
INCLUDE = re.compile(r'^INCLUDE_(?:ASM|RODATA)\("[^"]+",\s*(\w+)\);\n', re.M)
# Data converted from INCLUDE_RODATA to C, e.g. `const u8 D_801EDF84[0x1260] = {...};`
# Any name, so data keeps its body after tools/rename.py gives it a real one.
DATA_DEF = re.compile(r"^(?:static\s+)?(?:const\s+)?[A-Za-z_][\w \*]*?\b([A-Za-z_]\w*)\s*(?:\[[^\]\n]*\]\s*)*=\s*\{", re.M)
# Comments and INCLUDE_RODATA lines between a glued helper and the function that uses it.
SKIP_COMMENTS = re.compile(r"\A(?:\s*(?:/\*.*?\*/|//[^\n]*|INCLUDE_RODATA\([^)]*\);))*\s*", re.S)
LEADING_COMMENT = re.compile(r"(?:^(?:/\*.*\*/|//.*)\n)+\Z", re.M)


def _with_comment(text, start):
    """Extend start backwards over comment lines directly above a definition."""
    m = LEADING_COMMENT.search(text, 0, start)
    return m.start() if m and m.end() == start else start


def split_c(text):
    """Return (preamble, {name: body}) for a C file."""
    starts = [m.start() for m in INCLUDE.finditer(text)] + [m.start() for m in FUNC_DEF.finditer(text)]
    first = min(starts) if starts else len(text)
    first = _with_comment(text, first)
    preamble = text[:first]
    bodies = {}
    data = set()
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
        data.add(m.group(1))
    # Rodata that splat migrated into a function's asm has no slot of its own; keep it
    # glued to the function that directly follows it.
    # Same for file-local helpers (`static [inline] f(...) {...}`) written next to their user.
    for name in [n for n in bodies if n in data or bodies[n].lstrip().startswith(("static", "/*"))
                 and re.search(r"^static\b", bodies[n], re.M)]:
        body = bodies[name]
        after = text[text.index(body) + len(body):]
        nxt = FUNC_DEF.match(SKIP_COMMENTS.sub("", after, count=1))
        if nxt and nxt.group(1) in bodies:
            bodies[nxt.group(1)] = body + "\n" + bodies[nxt.group(1)]
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

    # Rodata the old sources no longer INCLUDE_RODATA (now emitted by C, e.g. string literals)
    # must not come back as a slot.
    RODATA_SLOT = re.compile(r'^INCLUDE_RODATA\("[^"]+",\s*(\w+)\);\n(?:\n)?', re.M)
    for npath in new:
        olds = [old[p] for p in old if os.path.dirname(p) == os.path.dirname(npath)]
        if not olds:
            continue
        had = set().union(*(set(RODATA_SLOT.findall(t)) for t in olds))
        new[npath] = RODATA_SLOT.sub(lambda m: m.group(0) if m.group(1) in had else "", new[npath])

    # Comments documenting still-unmatched functions sit directly above their INCLUDE_ASM line;
    # carry them over (deduplicated).
    for path, text in old.items():
        for m in re.finditer(r'((?:^(?:/\*.*\*/|//.*)\n)+)INCLUDE_ASM\("[^"]+",\s*(\w+)\);', text, re.M):
            lines = list(dict.fromkeys(m.group(1).splitlines(keepends=True)))
            pat = re.compile(r'^(INCLUDE_ASM\("[^"]+",\s*' + re.escape(m.group(2)) + r'\);)', re.M)
            for npath in [p for p in new if os.path.dirname(p) == os.path.dirname(path)]:
                new[npath], n = pat.subn(lambda mm: "".join(lines) + mm.group(1), new[npath], count=1)
                if n:
                    break

    # Put every hand-written body back over its INCLUDE_ASM slot, in whichever file it now lives.
    # Empty bodies are skipped: splat regenerates trivial functions as C itself.
    missing, kept = [], {}
    for path, text in old.items():
        _, bodies = split_c(text)
        for name, body in bodies.items():
            if EMPTY_BODY.search(body):
                # splat regenerates trivial functions as `void f(void) {}`; keep the
                # hand-written signature if it differs.
                for npath in [p for p in new if os.path.dirname(p) == os.path.dirname(path)]:
                    _, nb = split_c(new[npath])
                    if name in nb and EMPTY_BODY.search(nb[name]):
                        if nb[name] != body:
                            new[npath] = new[npath].replace(nb[name], body, 1)
                        break
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

    # Safety net: anything that was C before must not come back as INCLUDE_ASM.
    for path, text in old.items():
        was_asm = set(ASM_SLOT.findall(text))
        siblings = [p for p in new if os.path.dirname(p) == os.path.dirname(path)]
        now_asm = set().union(*(set(ASM_SLOT.findall(new[p])) for p in siblings)) if siblings else set()
        old_asm_all = set().union(*(set(ASM_SLOT.findall(old[p])) for p in old
                                    if os.path.dirname(p) == os.path.dirname(path)))
        for name in sorted(now_asm - old_asm_all):
            if re.search(r"\b" + re.escape(name) + r"\s*\(", text):
                missing.append((path, name + " (was C, would revert to asm)"))

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
