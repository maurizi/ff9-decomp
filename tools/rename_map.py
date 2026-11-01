#!/usr/bin/env python3
"""Which identifiers changed inside each function between two commits? For ports that patch decomp function bodies:
after a rename-only change (locals, parameters, globals, macros), map every old name to its new one per function.

    python3 tools/rename_map.py OLD_REV [NEW_REV]          # NEW_REV defaults to the working tree
    python3 tools/rename_map.py 4fe324ce HEAD --tsv > renames.tsv

For every function defined in src/**/*.c at both revisions (matched by name; a function renamed in between is
matched through docs/wip/names_from_port.txt), the two bodies are tokenized (comments and string literals dropped).
If they have the same token count and the same non-identifier tokens, the identifiers are zipped and each
consistent old→new pair is reported. Bodies whose structure changed (not a pure rename: e.g. a duplicated body
replaced by a helper call) are listed as `changed` so a port knows to re-read them by hand.
Output columns: file, function, old, new (or `changed` / `added` / `removed` in the old column).
"""
import argparse
import collections
import os
import re
import subprocess

ROOT = os.path.dirname(os.path.dirname(os.path.abspath(__file__)))
TOKEN = re.compile(r"[A-Za-z_]\w*|0[xX][0-9A-Fa-f]+|\d+|->|\+\+|--|<<=|>>=|<<|>>|<=|>=|==|!=|&&|\|\||[-+*/%&|^]=|\S")
FUNC = re.compile(r"^[A-Za-z_][\w \*]*?\b(\w+)\s*\(([^;]*?)\)\s*\{", re.M)
KEYWORDS = {"if", "else", "while", "for", "do", "switch", "case", "default", "return", "break", "continue", "goto",
            "sizeof", "struct", "union", "enum", "typedef", "static", "inline", "const", "volatile", "register",
            "extern", "unsigned", "signed", "void", "char", "short", "int", "long", "float", "double"}


def git_files(rev):
    if rev is None:
        out = subprocess.check_output(["git", "ls-files", "src"], cwd=ROOT, text=True)
        return {p: open(os.path.join(ROOT, p), errors="replace").read() for p in out.split() if p.endswith(".c")}
    out = subprocess.check_output(["git", "ls-tree", "-r", "--name-only", rev, "src"], cwd=ROOT, text=True)
    files = {}
    for p in out.split():
        if p.endswith(".c"):
            files[p] = subprocess.check_output(["git", "show", f"{rev}:{p}"], cwd=ROOT, text=True, errors="replace")
    return files


def strip(text):
    text = re.sub(r"/\*.*?\*/", lambda m: "\n" * m.group(0).count("\n"), text, flags=re.S)
    text = re.sub(r"//[^\n]*", "", text)
    return re.sub(r'"(?:[^"\\\n]|\\.)*"', '""', text)


def functions(files):
    out = {}
    for path, text in files.items():
        text = strip(text)
        for m in FUNC.finditer(text):
            name = m.group(1)
            if name in KEYWORDS:
                continue
            depth, i = 0, m.end() - 1
            while i < len(text):
                if text[i] == "{":
                    depth += 1
                elif text[i] == "}":
                    depth -= 1
                    if depth == 0:
                        break
                i += 1
            out.setdefault((path, name), text[m.start():i + 1])
    return out


def renames():
    old2new = {}
    p = os.path.join(ROOT, "docs/wip/names_from_port.txt")
    if os.path.exists(p):
        for line in open(p):
            m = re.match(r"(\w+)\s+(\w+)\s*(#|$)", line)
            if m and m.group(1) != m.group(2) and not line.startswith("#"):
                old2new[m.group(1)] = m.group(2)
    return old2new


def main():
    ap = argparse.ArgumentParser(description=__doc__, formatter_class=argparse.RawDescriptionHelpFormatter)
    ap.add_argument("old")
    ap.add_argument("new", nargs="?")
    ap.add_argument("--tsv", action="store_true")
    a = ap.parse_args()
    old, new = functions(git_files(a.old)), functions(git_files(a.new))
    ren = renames()
    rows = []
    seen_new = set()
    byname = collections.defaultdict(list)
    for (p, n) in new:
        byname[n].append(p)
    for (opath, name), obody in sorted(old.items()):
        nname = ren.get(name, name) if (opath, name) not in new and name not in byname else name
        npath = opath if (opath, nname) in new else (byname[nname][0] if len(byname.get(nname, [])) == 1 else None)
        if npath is None:
            rows.append((opath, name, "removed", ""))
            continue
        seen_new.add((npath, nname))
        nbody = new[(npath, nname)]
        if nname != name:
            rows.append((npath, nname, name, nname))
        ot, nt = TOKEN.findall(obody), TOKEN.findall(nbody)
        ident = lambda t: re.match(r"[A-Za-z_]", t) is not None and t not in KEYWORDS
        if len(ot) != len(nt) or any(ident(x) != ident(y) or (not ident(x) and x != y) for x, y in zip(ot, nt)):
            if ot != nt:
                rows.append((npath, nname, "changed", ""))
            continue
        pairs = collections.OrderedDict()
        for x, y in zip(ot, nt):
            if ident(x) and x != y and x != name:
                pairs.setdefault(x, set()).add(y)
        for x, ys in pairs.items():
            rows.append((npath, nname, x, ",".join(sorted(ys))))
    for (npath, name) in sorted(new):
        if (npath, name) not in seen_new:
            rows.append((npath, name, "added", ""))
    for r in rows:
        print("\t".join(r) if a.tsv else f"{r[0]}  {r[1]}: {r[2]} -> {r[3]}" if r[2] not in ("changed", "added", "removed") else f"{r[0]}  {r[1]}: {r[2]}")
    counts = collections.Counter("rename" if r[2] not in ("changed", "added", "removed") else r[2] for r in rows)
    print(f"# {dict(counts)}", file=__import__("sys").stderr)


if __name__ == "__main__":
    main()
