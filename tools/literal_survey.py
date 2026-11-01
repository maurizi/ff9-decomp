#!/usr/bin/env python3
"""Where are the unnamed numeric constants? Classifies every numeric literal in the C sources by context.

    python3 tools/literal_survey.py                 # totals per context and binary
    python3 tools/literal_survey.py --top 40        # also the most frequent (context, function/field) keys
    python3 tools/literal_survey.py --bin ovl_0b --ctx arg --top 30
    python3 tools/literal_survey.py --summary       # one line (for make progress)

Contexts: compare (==, !=, <, >, <=, >= against a literal), mask (&, |, ^, ~, &=, |=), shift (<<, >> amounts),
case (switch labels), arg (a call argument: key = callee#index), assign (= literal; key = the assigned field or
variable), index ([literal]), arith (+ - * / % with a literal), init (initializers), other.
Skipped as noise: 0, 1, -1, 2, 0xFF/0xFFFF/0xFFFFFFFF masks, literals inside #define bodies, comments, strings,
INCLUDE_ASM/INCLUDE_RODATA lines and asm statements. Small arithmetic and shift amounts are counted but are
mostly fine as numbers (fixed-point shifts, sizes); the actionable contexts are compare, mask, case, arg and
assign. A literal keeps its meaning hidden only when its context has a domain (an id, a flag, a mode): use
--top to find those clusters.
"""
import argparse
import collections
import glob
import os
import re

ROOT = os.path.dirname(os.path.dirname(os.path.abspath(__file__)))
NUM = re.compile(r"(?<![\w.])(0[xX][0-9A-Fa-f]+|\d+)[uUlL]*(?![\w.])")
NOISE = {"0", "1", "2", "0xFF", "0xFFFF", "0xFFFFFFFF", "0xff", "0xffff"}
FUNC = re.compile(r"^[A-Za-z_][\w \*]*?\b(\w+)\s*\([^;]*\)\s*\{")
CALL = re.compile(r"\b([A-Za-z_]\w*)\s*\(")
KEYWORDS = {"if", "while", "for", "switch", "return", "sizeof", "case"}


def strip(line):
    line = re.sub(r'"(?:[^"\\]|\\.)*"', '""', line)
    line = re.sub(r"'(?:[^'\\]|\\.)*'", "''", line)
    return re.sub(r"//.*", "", line)


def call_arg(text, pos):
    """If `pos` is inside a call's argument list, return 'callee#index'."""
    depth, i, idx = 0, pos - 1, 0
    while i >= 0:
        c = text[i]
        if c in ")]":
            depth += 1
        elif c in "([":
            if depth == 0:
                if c == "[":
                    return None
                m = re.search(r"([A-Za-z_]\w*)\s*$", text[:i])
                if m and m.group(1) not in KEYWORDS:
                    return f"{m.group(1)}#{idx}"
                return None
            depth -= 1
        elif c == "," and depth == 0:
            idx += 1
        elif c in ";{}" and depth == 0:
            return None
        i -= 1
    return None


def classify(line, m):
    before = line[:m.start()].rstrip()
    after = line[m.end():].lstrip()
    if re.match(r"^\s*case\b", line):
        return "case", None
    if re.search(r"(==|!=|<=|>=|<|>)$", before) and not before.endswith(("<<", ">>")) or re.match(r"(==|!=|<=|>=|<(?!<)|>(?!>))", after):
        return "compare", None
    if re.search(r"(<<|>>)=?$", before):
        return "shift", None
    if re.search(r"(&|\||\^|~|&=|\|=|\^=)$", before) or re.match(r"(&(?!&)|\|(?!\|)|\^)", after):
        return "mask", None
    if before.endswith("["):
        return "index", None
    key = call_arg(line, m.start())
    if key:
        return "arg", key
    am = re.search(r"([\w\]\)]+(?:(?:->|\.)\w+)*)\s*=$", before)
    if am and not before.endswith(("==", "!=", "<=", ">=")):
        return "assign", re.sub(r".*(?:->|\.)", "", am.group(1))
    if re.search(r"[-+*/%]$", before) or re.match(r"[-+*/%](?!=)", after):
        return "arith", None
    if before.endswith(("{", ",")) or re.search(r"=\s*\{", before):
        return "init", None
    return "other", None


def survey(bins):
    rows = []
    for path in sorted(glob.glob(f"{ROOT}/src/*/*.c") + glob.glob(f"{ROOT}/src/fxmod/*/*.c")):
        rel = os.path.relpath(path, ROOT)
        b = "fxmod" if "/fxmod/" in rel else rel.split("/")[1]
        if bins and b not in bins:
            continue
        if os.path.basename(path).startswith("psyq_"):
            continue
        fn, in_comment, in_define = None, False, False
        for line in open(path, errors="replace"):
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
            fm = FUNC.match(line)
            if fm and fm.group(1) not in KEYWORDS:
                fn = fm.group(1)
            line = strip(line)
            for m in NUM.finditer(line):
                v = m.group(1)
                if v in NOISE:
                    continue
                if line[:m.start()].rstrip().endswith("-") and v in ("1",):
                    continue
                ctx, key = classify(line, m)
                rows.append((b, ctx, key or fn or "?", v))
    return rows


def main():
    ap = argparse.ArgumentParser(description=__doc__, formatter_class=argparse.RawDescriptionHelpFormatter)
    ap.add_argument("--bin", action="append", help="limit to binaries (main, ovl_0b, fxmod, ...)")
    ap.add_argument("--ctx", help="limit --top to one context")
    ap.add_argument("--top", type=int, default=0)
    ap.add_argument("--summary", action="store_true")
    a = ap.parse_args()
    rows = survey(set(a.bin) if a.bin else None)
    actionable = ("compare", "mask", "case", "arg", "assign")
    if a.summary:
        game = [r for r in rows if r[0] != "fxmod"]
        n = sum(1 for r in game if r[1] in actionable)
        nf = sum(1 for r in rows if r[0] == "fxmod" and r[1] in actionable)
        print(f"literals  : {n} in game code, {nf} in effect modules (compare/mask/case/arg/assign contexts; "
              "tools/literal_survey.py --top N)")
        return
    by = collections.Counter((r[0], r[1]) for r in rows)
    bins = sorted({r[0] for r in rows})
    ctxs = ["compare", "mask", "case", "arg", "assign", "index", "shift", "arith", "init", "other"]
    print(f"{'bin':8}" + "".join(f"{c:>9}" for c in ctxs))
    for b in bins:
        print(f"{b:8}" + "".join(f"{by[(b, c)]:9d}" for c in ctxs))
    print(f"{'total':8}" + "".join(f"{sum(by[(b, c)] for b in bins):9d}" for c in ctxs))
    if a.top:
        keys = collections.Counter()
        vals = collections.defaultdict(collections.Counter)
        for b, ctx, key, v in rows:
            if (a.ctx and ctx != a.ctx) or (not a.ctx and ctx not in actionable):
                continue
            k = (b, ctx, key)
            keys[k] += 1
            vals[k][v] += 1
        print()
        for (b, ctx, key), n in keys.most_common(a.top):
            ex = ",".join(v for v, _ in vals[(b, ctx, key)].most_common(6))
            print(f"{n:5d} {b:7} {ctx:7} {key:36} e.g. {ex}")


if __name__ == "__main__":
    main()
