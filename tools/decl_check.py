#!/usr/bin/env python3
"""Check that every C declaration of a function agrees with its definition, across all binaries.

    tools/docker.sh python3 tools/decl_check.py [--bin ovl_0a] [--types] [--implicit]

Needs a built tree (`make`): it runs gcc 2.8.1's cc1 with `-aux-info` on every preprocessed `.i` under
build/us/src, which lists each function prototype a TU declares (including implicit `int f()`
declarations and those coming from headers) and each function it defines, with types as written.

A declaration seen in a TU of binary B resolves to: a static definition in the same TU; else B's
definition; else main's; else every other binary defining that name (overlays call each other by name,
e.g. main's script interpreter calls ovl_0a's field functions; same-address placeholder names in
overlays that overlap are compared only when the declaration can't be tied to one binary).

Reported (all should be 0; the PS1 build matches either way, a port compiled from src/ does not):
  IMPLICIT  a call without any prototype in scope (gcc 2.8 assumes `int f()`; modern gcc rejects it
            when the definition follows with another type).
  RETURN    declaration and definition disagree on void / non-void / pointer / integer return.
  NPARAMS   different parameter counts (variadic and unprototyped `()` declarations excepted).
  PARAM     a parameter is a pointer on one side and an integer on the other (wrong on 64-bit hosts).
With --types, integer width/sign differences in returns and parameters are listed too (informational).
"""
import argparse
import glob
import os
import re
import subprocess
import tempfile
from collections import defaultdict

CC1 = "/opt/cc/gcc-2.8.1-psx/cc1"
AUX = re.compile(r"^/\* (\S+):(\d+):([NOI])([CF]) \*/ (.*?);(?: /\*.*\*/)?$")
INTS = {"char": (8, None), "short": (16, True), "int": (32, True), "long": (32, True),
        "s8": (8, True), "u8": (8, False), "s16": (16, True), "u16": (16, False), "s32": (32, True),
        "u32": (32, False)}


def binary_of(path):
    p = path.split("/")
    i = p.index("src")
    return p[i + 2] if p[i + 1] == "fxmod" else p[i + 1]


def split_params(s):
    out, depth, cur = [], 0, ""
    for c in s:
        if c == "," and depth == 0:
            out.append(cur.strip())
            cur = ""
            continue
        depth += c in "(["
        depth -= c in ")]"
        cur += c
    if cur.strip():
        out.append(cur.strip())
    return out


def parse_proto(text):
    """'extern void foo (int a, char *b)' -> (name, ret, params or None, variadic, static)."""
    static = text.startswith("static ")
    text = re.sub(r"^(extern|static)\s+", "", text)
    # the parameter list is the last top-level (...) group
    depth, end = 0, len(text) - 1
    while text[end] != ")":
        end -= 1
    i = end
    while i >= 0:
        depth += text[i] == ")"
        depth -= text[i] == "("
        if depth == 0:
            break
        i -= 1
    head, params = text[:i].strip(), text[i + 1:end].strip()
    m = re.match(r"^(.*?)\(?\s*\*?\s*(\w+)\s*$", head)
    if head.endswith(")"):  # function returning a function pointer: 'int (*foo (args)) (...)'
        return None
    m = re.match(r"^(.*?)(\w+)$", head)
    name, ret = m.group(2), m.group(1).strip()
    if params == "/* ??? */":
        return name, ret, None, False, static
    plist = split_params(params)
    variadic = bool(plist) and plist[-1] == "..."
    if variadic:
        plist = plist[:-1]
    if plist == ["void"]:
        plist = []
    return name, ret, plist, variadic, static


def kind(t):
    """Category of a type as written: void / ptr / (bits, signed) / struct / other."""
    t = re.sub(r"\b(const|volatile|register|struct|union|enum)\b", lambda m: m.group(0) if m.group(1) in ("struct", "union", "enum") else "", t)
    t = " ".join(t.split())
    if "*" in t or "[" in t or "(" in t:
        return "ptr"
    words = t.split()
    if not words:
        return ("int", 32, True)
    if words[-1] in ("void",) and len(words) == 1:
        return "void"
    if words[0] in ("struct", "union"):
        return "struct"
    if words[0] == "enum":
        return ("int", 32, True)
    signed = None
    if "unsigned" in words:
        signed = False
    elif "signed" in words:
        signed = True
    base = [w for w in words if w not in ("unsigned", "signed")]
    b = base[-1] if base else "int"
    if b in INTS:
        bits, s = INTS[b]
        return ("int", bits, s if signed is None else signed)
    return ("type", b)  # a typedef we don't resolve


def param_type(p):
    # drop the parameter name: the last identifier, unless the whole thing is a type
    p = re.sub(r"\[[^\]]*\]$", "*", p.strip())
    if "(" in p:
        return "ptr"
    m = re.match(r"^(.*?)(\w+)$", p)
    if m and m.group(1).strip() and m.group(2) not in INTS and m.group(2) not in ("void",):
        return m.group(1)
    return p


def run_aux(files):
    out = []
    with tempfile.TemporaryDirectory() as d:
        for n, f in enumerate(files):
            aux = os.path.join(d, f"{n}.aux")
            subprocess.run([CC1, "-quiet", "-O2", "-G0", "-aux-info", aux, f, "-o", "/dev/null"],
                           capture_output=True)
            if os.path.exists(aux):
                out.append((f, open(aux).read().splitlines()))
    return out


def main():
    ap = argparse.ArgumentParser()
    ap.add_argument("--bin", help="only report problems in this binary's sources")
    ap.add_argument("--types", action="store_true", help="also list integer width/sign differences")
    ap.add_argument("--implicit", action="store_true", help="list every implicit call site, not one per name")
    args = ap.parse_args()

    files = sorted(glob.glob("build/us/src/**/*.c.i", recursive=True))
    if not files:
        raise SystemExit("no build/us/src/**/*.i: build first (tools/docker.sh make)")
    defs = defaultdict(list)       # name -> [(binary, tu, loc, ret, params, variadic, static)]
    decls = {}                     # (loc, name) -> (binaries, tus, ret, params, variadic)
    implicit = defaultdict(set)    # (binary, name) -> {loc}
    for f, lines in run_aux(files):
        b = binary_of(f)
        tu = f
        for line in lines:
            m = AUX.match(line)
            if not m:
                continue
            path, ln, style, what, proto = m.groups()
            if path.startswith("/") or "<" in path:
                continue
            p = parse_proto(proto)
            if p is None:
                continue
            name, ret, params, variadic, static = p
            loc = f"{path}:{ln}"
            if style == "I":
                implicit[(b, name)].add(loc)
                continue
            if what == "F":
                defs[name].append((b, tu, loc, ret, params, variadic, static))
                continue
            key = (loc, name)
            if key not in decls:
                decls[key] = (set(), set(), ret, params, variadic)
            decls[key][0].add(b)
            decls[key][1].add(tu)

    # unique definitions per name
    for n in defs:
        seen, u = set(), []
        for d in defs[n]:
            if d[2] not in seen:
                seen.add(d[2])
                u.append(d)
        defs[n] = u

    def resolve(name, binary, tus):
        ds = defs.get(name, [])
        st = [d for d in ds if d[6] and d[1] in tus]
        if st:
            return st
        ds = [d for d in ds if not d[6]]
        for pick in (binary, "main"):
            got = [d for d in ds if d[0] == pick]
            if got:
                return got
        return ds

    problems = defaultdict(list)
    seen = set()
    for (loc, name), (bins, tus, ret, params, variadic) in sorted(decls.items()):
        for b in sorted(bins, key=lambda x: (x.startswith("fx_"), x)):
            if args.bin and b != args.bin and not any(d[0] == args.bin for d in defs.get(name, [])):
                continue
            for d in resolve(name, b, tus):
                db, _tu, dloc, dret, dparams, dvar, _st = d
                if loc == dloc:
                    continue
                key = (loc, dloc)
                if key in seen:
                    continue
                seen.add(key)
                where = f"{name}: decl {loc} ({b}) vs def {dloc} ({db})"
                kr, kd = kind(ret), kind(dret)
                if (kr == "void") != (kd == "void") or (kr == "ptr") != (kd == "ptr") and "struct" not in (kr, kd):
                    if not (isinstance(kr, tuple) and kr[0] == "type" or isinstance(kd, tuple) and kd[0] == "type") \
                            or "void" in (kr, kd):
                        problems["RETURN"].append(f"{where}: '{ret}' vs '{dret}'")
                        continue
                if args.types and kr != kd and isinstance(kr, tuple) and isinstance(kd, tuple):
                    problems["TYPE"].append(f"{where}: returns '{ret}' vs '{dret}'")
                if params is None or dparams is None:
                    continue
                if variadic or dvar:
                    if len(params) != len(dparams) and not (variadic and dvar):
                        pass
                    continue
                if len(params) != len(dparams):
                    problems["NPARAMS"].append(f"{where}: {len(params)} vs {len(dparams)} params")
                    continue
                for i, (a, c) in enumerate(zip(params, dparams)):
                    ka, kc = kind(param_type(a)), kind(param_type(c))
                    if (ka == "ptr") != (kc == "ptr") and all(isinstance(k, tuple) and k[0] == "int" or k == "ptr"
                                                              for k in (ka, kc)):
                        problems["PARAM"].append(f"{where}: param {i + 1} '{a}' vs '{c}'")
                    elif args.types and ka != kc and isinstance(ka, tuple) and isinstance(kc, tuple) \
                            and ka[0] == kc[0] == "int":
                        problems["TYPE"].append(f"{where}: param {i + 1} '{a}' vs '{c}'")

    for (b, name), locs in sorted(implicit.items()):
        if args.bin and b != args.bin:
            continue
        shown = sorted(locs) if args.implicit else sorted(locs)[:1]
        problems["IMPLICIT"].append(f"{b:7} {name:32} {len(locs):3} site(s): {' '.join(shown)}")

    total = 0
    for cat in ("IMPLICIT", "RETURN", "NPARAMS", "PARAM", "TYPE"):
        for p in problems.get(cat, []):
            print(f"{cat:8} {p}")
        if cat != "TYPE":
            total += len(problems.get(cat, []))
    print(" ".join(f"{c}={len(problems.get(c, []))}" for c in ("IMPLICIT", "RETURN", "NPARAMS", "PARAM"))
          + (f" TYPE={len(problems.get('TYPE', []))}" if args.types else ""))
    print(f"{total} declaration problems")


if __name__ == "__main__":
    main()
