#!/usr/bin/env python3
"""Active Time Events (ATEs) in the field scripts, from FF9.IMG (static).

    python3 tools/ate_list.py [--disc N ...]            # every ATE menu, title card and mask write
    python3 tools/ate_list.py --disc 1 --field 552      # one field
    python3 tools/ate_list.py --json OUT.json

What it finds (docs/wip/ates.md explains the script side):
- ATE menus: `7C MSG_CHOICE_MASK` then `1F MSG_WAIT win, 0x40, LIST` (window flag 0x40 = "ACTIVE TIME EVENT" title)
  then a switch on sys.msgChoice. Choice k of LIST = bit k of EVT_GB_ATE_BITS. Each case shows a title card
  (`20 MSG win, 0x40, CARD`) and ends in `GOTO_FIELD`.
- Title cards outside a menu (MSG / MSG_WAIT with flag 0x40, not a choice list): ATEs the script starts by itself,
  i.e. required ones; the GOTO_FIELD that follows is the ATE's field.
- Writes of EVT_GB_ATE_BITS (Gu16[0xEC]) with the if-conditions around them (story counter = Gs16/Gu16[0x0]).
- Field-local masks (Dali, Cleyra, Madain Sari, Alexandria and Lindblum on disc 3): how entry 0 builds them, and the
  return code (Gs16[0xEF]) each field's init stores.
Text: the area's message table (FF9.IMG dir index 2 file AREA, DB item 0x60000 | AREA, fld_findAreaItem),
`{u32 count; {u16 header, u16, u16 text, u16}[count]}`, offsets from +4. Field name: script header +0x2C.
"""
import argparse
import json
import os
import re
import struct
import sys

ROOT = os.path.dirname(os.path.dirname(os.path.abspath(__file__)))
sys.path.insert(0, os.path.join(ROOT, 'tools'))
sys.path.insert(0, os.path.join(ROOT, 'tools', 'evtscript'))
from encounter_catalog import Img, scripts, script_functions, find_image, db_find  # noqa: E402
import ff9text as T  # noqa: E402
import edis  # noqa: E402  (installs the infix expression printer into evtdis)
from evtdis import dis  # noqa: E402

DIR_AREAS, DIR_FIELDS = 2, 4      # Img.files() indices
WIN_ATE = 0x40                    # MSG window flag: "ACTIVE TIME EVENT" title (g_msgAteTitle)
OP_JMP, OP_JMP_IFNOT, OP_EXPR, OP_SWITCH, OP_SWITCH_TABLE = 0x01, 0x02, 0x05, 0x06, 0x0B
OP_MSG_WAIT, OP_MSG, OP_GOTO_FIELD, OP_CHOICE_MASK, OP_ATE_PROMPT = 0x1F, 0x20, 0x2B, 0x7C, 0xD7
ESC_ARGS = {0x00: 1, 0x01: 1, 0x02: 1, 0x03: 4, 0x06: 2, 0x07: 2, 0x0E: 1, 0x0F: 1, 0x40: 1, 0x41: 1, 0x43: 1,
            0x44: 1, 0x49: 1, 0x4A: 1, 0x4B: 1, 0x4C: 1, 0x4D: 1, 0x4E: 1, 0x4F: 2, 0x5F: 6, 0x70: 1}
ESC_ARGS.update({k: 1 for k in range(0x68, 0x70)})
CHR_NAMES = ['Zidane', 'Vivi', 'Dagger', 'Steiner', 'Freya', 'Quina', 'Eiko', 'Amarant']  # default names, TEXT_CHAR_NAME + n


def decode(b, o=0):
    """Message bytes -> (plain text, list of lines). Control codes are dropped (choice markers kept as lines)."""
    out = []
    while o < len(b) and b[o] != 0xFF:
        c = b[o]
        if c == 0xF9:
            e = b[o + 1]; o += 2
            if 0x10 <= e <= 0x17 or 0x80 <= e <= 0x87:
                out.append(CHR_NAMES[e & 7]); continue
            if 0x18 <= e <= 0x1B:
                out.append(f'[party{e - 0x18}]'); continue
            if e == 0x48:                      # u16 width, then codes up to FF
                o += 2
                while b[o] != 0xFF: o += 1
                o += 1
            elif e == 0x0D:                    # {u16, codes up to FF}..., u16 FFFF
                while not (b[o] == 0xFF and b[o + 1] == 0xFF):
                    o += 2
                    while b[o] != 0xFF: o += 1
                    o += 1
                o += 2
            else:
                o += ESC_ARGS.get(e, 0)
            continue
        if 0xFA <= c <= 0xFD:
            out.append('?'); o += 2; continue
        out.append('\n' if c in (0xF7, 0xFE) else '' if c == 0xF8 else T.CHARMAP.get(c, '?')); o += 1
    s = ''.join(out)
    return s, [x.strip() for x in s.split('\n') if x.strip()]


class Disc:
    def __init__(self, disc):
        self.disc = disc
        self.img = Img(find_image(disc))
        self.fields = {fid & 0xFFFF: (fid >> 16, sec, n) for fid, sec, n in self.img.files(DIR_FIELDS)}
        self.areas = {fid & 0xFFFF: (sec, n) for fid, sec, n in self.img.files(DIR_AREAS)}
        self._msgs = {}

    def script(self, field):
        area, sec, n = self.fields[field]
        data = self.img.read(sec * 2048, n * 2048)
        for k, s in scripts(data):
            if k == 0x50000 | field:
                return area, data, s
        return area, None, None

    def name(self, field):
        _, data, s = self.script(field)
        return decode(data, s + 0x2C)[0].replace('\n', ' ') if data else '?'

    def messages(self, area):
        if area not in self._msgs:
            msgs = []
            if area in self.areas:
                sec, n = self.areas[area]
                d = self.img.read(sec * 2048, n * 2048)
                o = db_find(d, 0x60000 | area)
                if o is not None:
                    cnt = struct.unpack_from('<I', d, o)[0]
                    base = o + 4
                    for i in range(cnt):
                        msgs.append(decode(d, base + struct.unpack_from('<H', d, base + i * 8 + 4)[0])[1])
            self._msgs[area] = msgs
        return self._msgs[area]


def conditions(ins, pos):
    """If-conditions whose block holds offset pos: [(text, holds)], outermost first. holds=False means pos is in the
    else part (the JMP at the end of the then-part jumps over it)."""
    conds = []
    for j, (s, op, fl, args, raw) in enumerate(ins):
        if s >= pos: break
        if op == OP_JMP_IFNOT and j and ins[j - 1][1] == OP_EXPR:
            tgt = int(args[0][2:], 16)
            if s < pos < tgt:
                conds.append((ins[j - 1][3][0], True, s))
            else:
                # else part: the instruction before tgt is a JMP over [tgt, end)
                k = next((k for k, x in enumerate(ins) if x[0] >= tgt), None)
                if k and ins[k - 1][1] == OP_JMP and tgt <= pos < int(ins[k - 1][3][0][2:], 16):
                    conds.append((ins[j - 1][3][0], False, s))
    return [(c, h) for c, h, _ in conds]


def switch_cases(ins, j):
    """Case targets of the switch at ins[j]: {value: offset} (default under 'default')."""
    s, op, fl, args, raw = ins[j]
    out = {}
    if op == OP_SWITCH_TABLE:
        n = raw[1]; base = raw[2] | (struct.unpack_from('<b', raw, 3)[0] << 8); r = s + 1
        out['default'] = r + struct.unpack_from('<H', raw, 4)[0]
        for k in range(n):
            out[base + k] = r + struct.unpack_from('<H', raw, 6 + 2 * k)[0]
    elif op == OP_SWITCH:
        for a in args[2:]:
            v, t = a.split('->'); out[int(v)] = int(t, 16)
        out['default'] = int(args[1].split('->')[1], 16)
    return out


def scan_case(ins, start, stop):
    """Title card and GOTO_FIELD targets between offsets start and stop (linear)."""
    card, gotos, sets = None, [], []
    for s, op, fl, args, raw in ins:
        if s < start or s >= stop: continue
        if op in (OP_MSG, OP_MSG_WAIT) and isinstance(args[1], int) and args[1] & WIN_ATE and card is None:
            card = args[2]
        if op == OP_GOTO_FIELD:
            gotos.append(args[0])
        if op == OP_EXPR and ('Gs16[0x2] =' in args[0] or 'Gu16[0xec]' in args[0]):
            sets.append(args[0])
    return card, gotos, sets


def scan_field(d, field):
    area, data, s = d.script(field)
    if data is None:
        return None
    msgs = d.messages(area)
    txt = lambda m: msgs[m] if isinstance(m, int) and m < len(msgs) else ['?']
    res = {'disc': d.disc, 'field': field, 'area': area, 'name': d.name(field), 'menus': [], 'cards': [],
           'mask_writes': [], 'prompt': set(), 'mask_source': set(), 'prompt_if': [], 'story_tests': set()}
    for e, tag, code in script_functions(data, s):
        ins = list(dis(code))
        menu_ranges = []
        for j, (o, op, fl, args, raw) in enumerate(ins):
            if op == OP_ATE_PROMPT:
                res['prompt'].add(args[0] if isinstance(args[0], int) else str(args[0]))
                if args[0] == 1 and e == 0 and tag == 0:
                    res['prompt_if'] = [c if h else '!' + c for c, h in conditions(ins, o)]
            if op == OP_EXPR:
                for m in re.finditer(r'Gu16\[0x0\] (<|>|<=|>=|==|!=) (\d+)|(\d+) (<|<=) Gu16\[0x0\]', args[0]):
                    res['story_tests'].add(int(m.group(2) or m.group(3)))
                m = re.match(r'\{?\(Gs16\[0xf1\] = (.*)\)\}?$', args[0])
                if m and m.group(1) != '0':
                    res['mask_source'].add(m.group(1))
            if op == OP_EXPR and 'Gu16[0xec]' in args[0] and any(x in args[0] for x in
                                                                  ('(Gu16[0xec] =', '(Gu16[0xec] |=', '(Gu16[0xec] &=', '(Gu16[0xec] ^=')):
                res['mask_writes'].append({'entry': e, 'tag': tag, 'ofs': o, 'expr': args[0],
                                           'if': conditions(ins, o)})
            if op == OP_MSG_WAIT and isinstance(args[1], int) and args[1] & WIN_ATE and j and ins[j - 1][1] == OP_CHOICE_MASK:
                lst = args[2]
                rows = txt(lst)
                # the switch on sys.msgChoice right after
                k = j + 1
                while k < len(ins) and ins[k][1] not in (OP_SWITCH, OP_SWITCH_TABLE): k += 1
                cases = switch_cases(ins, k) if k < len(ins) else {}
                tg = sorted(set(cases.values()))
                end = max(tg) + 1 if tg else o
                menu = {'entry': e, 'tag': tag, 'ofs': o, 'list': lst, 'rows': rows, 'choices': []}
                for v, t in sorted((v, t) for v, t in cases.items() if v != 'default'):
                    nxt = min([x for x in tg if x > t] + [len(code)])
                    if nxt == len(code):
                        # last case: up to the default target if that is after it, else to the end
                        nxt = cases.get('default', len(code)) if cases.get('default', 0) > t else len(code)
                    card, gotos, sets = scan_case(ins, t, nxt)
                    menu['choices'].append({'bit': v, 'card': card, 'title': ' '.join(txt(card)) if card is not None else None,
                                            'goto': gotos, 'sets': sets})
                    end = max(end, nxt)
                menu_ranges.append((o, end))
                res['menus'].append(menu)
        for j, (o, op, fl, args, raw) in enumerate(ins):
            if op in (OP_MSG, OP_MSG_WAIT) and isinstance(args[1], int) and args[1] & WIN_ATE:
                if any(a <= o < b for a, b in menu_ranges): continue
                if j and ins[j - 1][1] == OP_CHOICE_MASK: continue
                gotos = [x[3][0] for x in ins if x[0] > o and x[1] == OP_GOTO_FIELD][:1]
                res['cards'].append({'entry': e, 'tag': tag, 'ofs': o, 'msg': args[2], 'title': ' '.join(txt(args[2])),
                                     'goto': gotos, 'if': conditions(ins, o)})
    res['prompt'] = sorted(res['prompt'], key=str)
    res['mask_source'] = sorted(res['mask_source'])
    res['story_tests'] = sorted(res['story_tests'])
    # field-local masks: how this field's entry 0 computes them, and the return code its init stores
    res['local_mask'], res['return_code'] = [], []
    local = [m for m in res['mask_source'] if re.fullmatch(r'[FL]u(8|16)\[0x[0-9a-f]+\]', m)]
    for e, tag, code in script_functions(data, s):
        if e != 0:
            continue
        ins = list(dis(code))
        for o, op, fl, args, raw in ins:
            if op != OP_EXPR:
                continue
            m = re.match(r'\{?\(Gs16\[0xef\] = (-?\d+)\)', args[0])
            if m and tag == 0:
                res['return_code'].append(int(m.group(1)))
            for v in local:
                m = re.match(r'\{?\(' + re.escape(v) + r' (\|=|=|&=) (-?\d+)\)', args[0])
                if m:
                    res['local_mask'].append({'var': v, 'op': m.group(1), 'value': int(m.group(2)), 'tag': tag,
                                              'ofs': o, 'if': [c if h else '!' + c for c, h in conditions(ins, o)]})
    return res


def main():
    ap = argparse.ArgumentParser(description=__doc__, formatter_class=argparse.RawDescriptionHelpFormatter)
    ap.add_argument('--disc', type=int, action='append')
    ap.add_argument('--field', type=int, action='append')
    ap.add_argument('--json')
    ap.add_argument('--all', action='store_true', help='also list fields that only show the prompt')
    a = ap.parse_args()
    out = []
    for dn in a.disc or [1, 2, 3, 4]:
        d = Disc(dn)
        for f in sorted(d.fields):
            if a.field and f not in a.field: continue
            r = scan_field(d, f)
            if not r: continue
            if not (r['menus'] or r['cards'] or r['mask_writes'] or (a.all and r['prompt'])): continue
            out.append(r)
            print(f"disc {dn} field {f} (area {r['area']}) {r['name']}  prompt modes {r['prompt']}  mask "
                  f"{r['mask_source']}  return code {r['return_code']}")
            for w in r['local_mask']:
                print(f"  local {w['var']} {w['op']} {w['value']}  if {' && '.join(w['if'])}")
            for m in r['menus']:
                print(f"  menu e{m['entry']} t{m['tag']} +{m['ofs']:#x} list msg {m['list']:#x}: {m['rows']}")
                for c in m['choices']:
                    print(f"    bit {c['bit']}: card {c['card']} {c['title']!r} -> {c['goto']} {c['sets']}")
            for c in r['cards']:
                print(f"  card e{c['entry']} t{c['tag']} +{c['ofs']:#x} msg {c['msg']} {c['title']!r} -> {c['goto']}")
                for t, h in c['if']:
                    print(f"      {'if' if h else 'else of'} {t}")
            for w in r['mask_writes']:
                print(f"  mask e{w['entry']} t{w['tag']} +{w['ofs']:#x} {w['expr']}")
                for t, h in w['if']:
                    print(f"      {'if' if h else 'else of'} {t}")
    if a.json:
        json.dump(out, open(a.json, 'w'), indent=1, default=str)


if __name__ == '__main__':
    main()
