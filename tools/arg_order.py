#!/usr/bin/env python3
"""List calls whose result could depend on the order the arguments are evaluated in.

    python3 tools/arg_order.py [--all] [files...]

C leaves that order unspecified. gcc 2.8 for MIPS, ARM gcc and x86 gcc don't agree: in evt_execInstr,
`evt_callObjectFunc(evt_getObjectArg(), a, *g_evtCurObj->unk0++, 0)` read the script stream in two arguments,
and an x86 build read the key byte first (the DS port's tools/arg_order.py found it). Sequence such a call
with a local (`o = evt_getObjectArg(); f(o, a, *ip++, 0);`); the PS1 output usually stays the same.

Reported: an argument modifies something (++/--, assignment) that another argument reads, or that another
argument could read through a call (anything but a plain local); or two arguments call functions that
consume a stream or advance state (names matching rand/evt_get/Arg/_pop/_read/_next). --all lists every
call with two or more arguments that have side effects of any kind (the DS port's rule; mostly harmless
lookups). Calls inside macros aren't seen. Check each report in the asm before changing it.
Within one expression (not arguments), modern gcc's -Wsequence-point finds the rest, e.g. RAND16 in ovl_0b.h.
"""
import argparse
import glob
import re

PURE = {"sizeof", "ABS", "MIN", "MAX", "rsin", "rcos", "ratan2", "SquareRoot0", "SquareRoot12", "math_sin",
        "math_cos", "getTPage", "getClut"}
# functions that consume a stream or advance state, so two of them in one argument list depend on the order
STATEFUL = re.compile(r"rand|Rand|evt_get|Arg|_pop|_read|_next")
CALL = re.compile(r"\b([A-Za-z_]\w*)\s*\(")
KEYWORDS = {"if", "while", "for", "switch", "return", "sizeof"}


def split_args(text):
    args, depth, cur = [], 0, ""
    for ch in text:
        if ch in "([{":
            depth += 1
        elif ch in ")]}":
            depth -= 1
        if ch == "," and depth == 0:
            args.append(cur)
            cur = ""
        else:
            cur += ch
    args.append(cur)
    return args


def effects(arg):
    """(has side effects, objects it modifies)."""
    a = re.sub(r'"(\\.|[^"])*"', "", arg)
    mods = set()
    for m in re.finditer(r"([\w.\->\[\]]+)\s*(\+\+|--)|(\+\+|--)\s*([\w.\->\[\]]+)", a):
        mods.add(m.group(1) or m.group(4))
    for m in re.finditer(r"([\w.\->\[\]]+)\s*(?<![=!<>+\-*/%&|^])=(?!=)", a):
        mods.add(m.group(1))
    calls = [n for n in CALL.findall(a) if n not in PURE and n not in KEYWORDS]
    return bool(mods or calls), mods, calls


def calls(code):
    for m in CALL.finditer(code):
        name = m.group(1)
        if name in KEYWORDS:
            continue
        i, depth = m.end(), 1
        while i < len(code) and depth:
            depth += code[i] == "("
            depth -= code[i] == ")"
            i += 1
        yield name, code[m.end():i - 1], m.start()


def base(expr):
    """the variable an lvalue expression starts from: g_evtCurObj->unk0 -> g_evtCurObj"""
    m = re.match(r"\s*\**\s*([A-Za-z_]\w*)", expr)
    return m.group(1) if m else expr


def main():
    ap = argparse.ArgumentParser()
    ap.add_argument("--all", action="store_true")
    ap.add_argument("files", nargs="*")
    args = ap.parse_args()
    hits = 0
    for path in sorted(args.files or glob.glob("src/**/*.c", recursive=True)):
        code = open(path, errors="replace").read()
        code = re.sub(r"/\*.*?\*/", lambda m: re.sub(r"[^\n]", " ", m.group(0)), code, flags=re.S)
        code = re.sub(r"//[^\n]*", "", code)
        for name, text, at in calls(code):
            # skip definitions and prototypes: a call is followed by neither '{' nor a K&R parameter list
            parts = split_args(text)
            if len(parts) < 2:
                continue
            fx = [effects(p) for p in parts]
            if args.all:
                report = sum(e[0] for e in fx) >= 2
            else:
                report = False
                for i, (_, mods, _c) in enumerate(fx):
                    others = [(parts[k], fx[k]) for k in range(len(parts)) if k != i]
                    for v in mods:
                        shared = not re.fullmatch(r"[a-z]\w*", v.strip()) or v.strip().startswith(("g_", "s_"))
                        # another argument reads the same object, or (for shared state) calls a function
                        if any(re.search(r"(?<![\w>.])" + re.escape(v.strip()) + r"(?![\w\[]|\s*=[^=])", o) for o, _ in others) or \
                                (shared and any(e[2] for _, e in others)):
                            report = True
                stateful = [k for k, e in enumerate(fx) if any(STATEFUL.search(c) for c in e[2])]
                if len(stateful) >= 2:
                    report = True
            if report:
                line = code.count("\n", 0, at) + 1
                hits += 1
                print(f"{path}:{line}: {name}({' '.join(text.split())[:120]})")
    print(f"{hits} calls")


if __name__ == "__main__":
    main()
