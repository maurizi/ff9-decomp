#!/usr/bin/env python3
"""Re-run splat without losing decompiled C.

splat never rewrites an existing .c file, so after symbol/boundary changes the
INCLUDE_ASM list in src/**/*.c goes stale. Usage: resplit.py [yaml ...] This moves each C file aside, lets
splat regenerate it, then merges back: the old preamble (everything before the
first INCLUDE_ASM or function), and every decompiled function body replacing
its INCLUDE_ASM line. Text between items (multi-line comments, #defines, top-level
__asm__ padding, comments above INCLUDE_RODATA lines) travels with the item below it,
so an unchanged layout round-trips byte for byte. A definition declared under an asm
label (`T f_def(...) __asm__("f");`) fills f's slot. Run inside the container: tools/docker.sh make resplit
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
INCLUDE = re.compile(r'^INCLUDE_(ASM|RODATA)\("[^"]+",\s*(\w+)\);\n', re.M)
# Data converted from INCLUDE_RODATA to C, e.g. `const u8 D_801EDF84[0x1260] = {...};`
# Any name, so data keeps its body after tools/rename.py gives it a real one.
DATA_DEF = re.compile(r"^(?:static\s+)?(?:const\s+)?[A-Za-z_][\w \*]*?\b([A-Za-z_]\w*)\s*(?:\[[^\]\n]*\]\s*)*=\s*\{", re.M)
LEADING_COMMENT = re.compile(r"(?:^(?:/\*.*\*/|//.*)\n)+\Z", re.M)
# A declaration giving a C name an asm label, e.g. `T *func_X_def(s32, S) __asm__("func_X");`: the
# definition of func_X_def is the body for func_X's slot.
ASM_ALIAS = re.compile(r'\b([A-Za-z_]\w*)\s*\([^;{}]*\)\s*(?:__asm__|asm)\s*\(\s*"(\w+)"\s*\)\s*;')
EMPTY_BODY = re.compile(r"\{\s*\}\n?$")


def asm_aliases(texts):
    """{C name: asm label} from every `__asm__("label")` declaration in headers and the given sources."""
    out = {}
    headers = [open(p).read() for p in glob.glob("include/**/*.h", recursive=True)]
    for text in headers + list(texts):
        for m in ASM_ALIAS.finditer(text):
            if m.group(1) != m.group(2):
                out[m.group(1)] = m.group(2)
    return out


def _with_comment(text, start):
    """Extend start backwards over comment lines directly above a definition."""
    while start:
        prev = text.rfind("\n", 0, start - 1) + 1
        if not LEADING_COMMENT.fullmatch(text, prev, start):
            break
        start = prev
    return start


class Item:
    """A top-level INCLUDE_ASM/INCLUDE_RODATA line ("asm"/"rodata") or C definition ("def").
    lead: the text between the previous item and this one, minus the one blank separator line splat writes."""

    def __init__(self, kind, name, start, end, text, data=False):
        self.kind, self.name, self.start, self.end, self.data = kind, name, start, end, data
        self.body = text[start:end]
        self.lead = ""

    @property
    def chunk(self):
        return self.lead + self.body


def parse(text):
    """Return (preamble, [Item], tail) for a C file; tail is the text after the last item."""
    found = [(m.start(), m.end(), m.group(1).lower(), m.group(2)) for m in INCLUDE.finditer(text)]
    for m in FUNC_DEF.finditer(text):
        end = re.compile(r"^\}\n", re.M).search(text, m.end())
        if not end:
            sys.exit(f"unterminated function {m.group(1)}")
        found.append((m.start(), end.end(), "def", m.group(1)))
    first = _with_comment(text, min(f[0] for f in found)) if found else len(text)
    for m in DATA_DEF.finditer(text):
        if m.start() < first:
            continue  # declarations in the preamble stay there
        end = re.compile(r"^\};\n", re.M).search(text, m.end())
        if not end:
            sys.exit(f"unterminated data definition {m.group(1)}")
        found.append((m.start(), end.end(), "data", m.group(1)))
    items, pos = [], first
    for start, end, kind, name in sorted(found):
        if start < pos:
            continue  # inside an earlier definition
        if kind in ("def", "data"):
            start = max(_with_comment(text, start), pos)
        it = Item("def" if kind == "data" else kind, name, start, end, text, data=kind == "data")
        gap = text[pos:start]
        it.lead = gap[1:] if items and gap.startswith("\n") else gap
        items.append(it)
        pos = end
    return text[:first], items, text[pos:]


def glue(items):
    """Rodata that splat migrated into a function's asm has no slot of its own; keep it glued to the
    function that directly follows it (only INCLUDE_RODATA lines in between). Same for file-local
    helpers (`static [inline] f(...) {...}`) written next to their user. Returns (items, names of
    INCLUDE_RODATA lines now carried inside a glued chunk)."""
    out, absorbed, i = [], set(), 0
    while i < len(items):
        it = items[i]
        helper = it.kind == "def" and (it.data or it.body.lstrip().startswith(("static", "/*"))
                                       and re.search(r"^static\b", it.body, re.M))
        j = i + 1
        while j < len(items) and items[j].kind == "rodata":
            j += 1
        if helper and j < len(items) and items[j].kind == "def":
            # The next function keeps its name and slot and carries everything from `it` on (a helper
            # glued onto another helper is carried along by the next iteration).
            absorbed.update(r.name for r in items[i + 1:j])
            items[j].lead = it.chunk + "".join("\n" + r.chunk for r in items[i + 1:j]) + "\n" + items[j].lead
            i = j
            continue
        out.append(it)
        i += 1
    return out, absorbed


def split_c(text):
    """Return (preamble, {name: body}) for a C file (bodies with glued helpers and comments)."""
    preamble, items, _ = parse(text)
    items, _ = glue(items)
    return preamble, {it.name: it.chunk for it in items if it.kind == "def"}


def slot_re(name, kinds="ASM|RODATA"):
    return re.compile(r'^INCLUDE_(?:' + kinds + r')\("[^"]+",\s*' + re.escape(name) + r"\);\n", re.M)


ASM_ADDR = re.compile(r"^\s*/\* [0-9A-F]+ ([0-9A-F]{8}) ", re.M)


def rodata_ranges(texts):
    """[(asm dir of the binary, lo, hi)] vram ranges of the asm files the given sources INCLUDE_RODATA (read before re-splitting)."""
    out = []
    for text in texts:
        for d, name in re.findall(r'^INCLUDE_RODATA\("([^"]+)",\s*(\w+)\);', text, re.M):
            try:
                addrs = [int(a, 16) for a in ASM_ADDR.findall(open(f"{d}/{name}.s").read())]
            except OSError:
                continue
            if addrs:
                out.append((d.split("/nonmatchings")[0], min(addrs), max(addrs)))
    return out


def asm_start(d, name):
    try:
        m = ASM_ADDR.search(open(f"{d}/{name}.s").read())
    except OSError:
        return None
    return int(m.group(1), 16) if m else None


def merge(old, fresh, aliases, old_ranges=()):
    """old, fresh: {path: text}. old_ranges: vram ranges of the rodata the old sources INCLUDE_RODATA'd.
    Returns (new texts, {path: kept count}, missing list)."""
    # Fresh splat output, with each old file's preamble restored.
    new = {}
    for path, text in fresh.items():
        if path in old:
            text = parse(old[path])[0] + text[len(parse(text)[0]):]
        new[path] = text
    same_bin = lambda path, files: [p for p in files if os.path.dirname(p) == os.path.dirname(path)]

    # INCLUDE_RODATA lines keep their place in front of the item that followed them (their order
    # against the C functions' own rodata matters), as long as splat still emits them.
    RODATA_ANY = re.compile(r'^INCLUDE_RODATA\("[^"]+",\s*(\w+)\);', re.M)
    parsed = {}
    for path, text in old.items():
        _, items, tail = parse(text)
        items, absorbed = glue(items)
        still = set().union(*(set(RODATA_ANY.findall(new[p])) for p in same_bin(path, new)))
        out, run = [], []
        for it in items:
            if it.kind == "rodata":
                run.append(it)
                continue
            if run:
                it.lead = "".join(r.chunk + "\n" if r.name in still else r.lead for r in run) + it.lead
                absorbed.update(r.name for r in run)
                run = []
            out.append(it)
        parsed[path] = out + run, absorbed, tail

    # Rodata the old sources no longer INCLUDE_RODATA (now emitted by C, e.g. string literals), or that
    # a glued chunk now carries itself, must not come back as a slot. A new symbol that starts inside
    # rodata an old slot covered (a new subsegment boundary split that symbol) keeps its slot.
    RODATA_SLOT = re.compile(r'^INCLUDE_RODATA\("([^"]+)",\s*(\w+)\);\n(?:\n)?', re.M)

    def keep(m, had, gone):
        d, name = m.group(1), m.group(2)
        if name in had:
            return name not in gone
        a = asm_start(d, name)
        b = d.split("/nonmatchings")[0]
        return a is not None and any(ob == b and lo < a <= hi for ob, lo, hi in old_ranges)
    for npath in new:
        olds = same_bin(npath, old)
        if not olds:
            continue
        had = set(n for p in olds for _, n in RODATA_SLOT.findall(old[p]))
        gone = set().union(*(parsed[p][1] for p in olds))
        new[npath] = RODATA_SLOT.sub(lambda m: m.group(0) if keep(m, had, gone) else "", new[npath])

    # Put every item back over its slot, in whichever file of the same binary it now lives (overlays at
    # the same address reuse names). Slots still INCLUDE'd keep their lead (comments above them).
    missing, kept = [], {}
    for path, (items, _, _) in parsed.items():
        for it in items:
            name = aliases.get(it.name, it.name)  # the slot is named after the asm label
            if it.kind != "def":
                if not it.lead:
                    continue
                pat = slot_re(name, it.kind.upper())
                for npath in same_bin(path, new):
                    new[npath], n = pat.subn(lambda m: it.lead + m.group(0), new[npath], count=1)
                    if n:
                        break
                continue
            if EMPTY_BODY.search(it.body):
                # splat regenerates trivial functions as `void f(void) {}`; keep the
                # hand-written signature (and what precedes it).
                for npath in same_bin(path, new):
                    hit = [n for n in parse(new[npath])[1]
                           if n.kind == "def" and n.name == name and EMPTY_BODY.search(n.body)]
                    if hit:
                        new[npath] = new[npath][:hit[0].start] + it.chunk + new[npath][hit[0].end:]
                        break
                continue
            for npath in same_bin(path, new):
                new[npath], n = slot_re(name).subn(lambda _: it.chunk, new[npath], count=1)
                if n:
                    kept[npath] = kept.get(npath, 0) + 1
                    break
            else:
                missing.append((path, it.name))

    # Text after the last item (trailing comments or padding) stays at the end of its file.
    for path, (_, _, tail) in parsed.items():
        if path in new:
            new[path] = new[path].rstrip("\n") + "\n" + tail

    # Items that moved to another file (e.g. a C subsegment split in two) carry their INCLUDE_RODATA
    # lines along; point them at the new file's asm directory.
    for path in new:
        base = os.path.splitext(os.path.basename(path))[0]
        new[path] = re.sub(r'^(INCLUDE_(?:ASM|RODATA)\("[^"]*/nonmatchings/)[^"/]+"',
                           lambda m: m.group(1) + base + '"', new[path], flags=re.M)

    # Safety net: anything that was C before must not come back as INCLUDE_ASM.
    for path, text in old.items():
        siblings = same_bin(path, new)
        now_asm = set().union(*(set(ASM_SLOT.findall(new[p])) for p in siblings)) if siblings else set()
        old_asm_all = set().union(*(set(ASM_SLOT.findall(old[p])) for p in same_bin(path, old)))
        for name in sorted(now_asm - old_asm_all):
            if re.search(r"\b" + re.escape(name) + r"\s*\(", text):
                missing.append((path, name + " (was C, would revert to asm)"))
    return new, kept, missing


def main():
    yamls = sys.argv[1:] or DEFAULT_YAMLS
    stale = glob.glob("src/**/*.bak", recursive=True)
    if stale:
        sys.exit(f"refusing to run: leftover backups {stale}; resolve them first")

    # Only the C files of the binaries being split (src_path of each yaml): e.g. the effect modules'
    # src/fxmod/ is left alone when their images aren't extracted and their yamls aren't passed.
    dirs = set()
    for y in yamls:
        m = re.search(r"^\s*src_path:\s*(\S+)", open(y).read(), re.M)
        dirs.add(os.path.normpath(m.group(1)) if m else "src")
    mine = lambda path: any(os.path.normpath(path).startswith(d + os.sep) for d in dirs)
    old = {}
    for path in glob.glob("src/**/*.c", recursive=True):
        if not mine(path):
            continue
        old[path] = open(path).read()
        os.rename(path, path + ".bak")
    old_ranges = rodata_ranges(old.values())
    try:
        subprocess.run(["rm", "-rf", "asm/us"], check=True)
        # The exe and overlays one by one (their output is worth reading); the many small effect
        # module configs (config/fxmod/) in parallel, quietly.
        for y in [y for y in yamls if "/fxmod/" not in y]:
            subprocess.run([sys.executable, "-m", "splat", "split", y], check=True)
        fx = [y for y in yamls if "/fxmod/" in y]
        if fx:
            from concurrent.futures import ThreadPoolExecutor
            run = lambda y: subprocess.run([sys.executable, "-m", "splat", "split", y], capture_output=True)
            with ThreadPoolExecutor(8) as ex:
                for y, r in zip(fx, ex.map(run, fx)):
                    if r.returncode:
                        sys.stderr.write(r.stdout.decode() + r.stderr.decode())
                        raise SystemExit(f"splat failed on {y}")
    except BaseException:
        for path in old:
            os.replace(path + ".bak", path)
        raise

    fresh = {p: open(p).read() for p in glob.glob("src/**/*.c", recursive=True) if mine(p)}
    new, kept, missing = merge(old, fresh, asm_aliases(old.values()), old_ranges)
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
