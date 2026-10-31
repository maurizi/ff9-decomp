"""menuscan.py [--discs 1,2,3,4]: every MENU (0x75) instruction in every field script, decoded with evtdis, plus the
talk-function recipe: actor entry (NEW_ACTOR) -> tag 3 (and 8) -> MENU, following CALL ops 0x10/0x12/0x14 (and
0x16/0x18/0x1A = call back the caller) into other entries' functions."""
import os, sys, struct, argparse, collections
sys.path.insert(0, os.path.dirname(os.path.abspath(__file__)))
sys.path.insert(0, os.path.dirname(os.path.dirname(os.path.abspath(__file__))))
from encounter_catalog import Img, scripts, script_functions, find_image
from evtdis import dis

p = argparse.ArgumentParser(); p.add_argument('--discs', default='1,2,3,4'); p.add_argument('--raw', action='store_true')
p.add_argument('--verbose', action='store_true')
a = p.parse_args()
TYPES = {0: 'main', 1: 'name', 2: 'shop', 3: 'party', 4: 'save', 5: 'chocograph', 8: 'disc'}

seen_fields = {}
summary = []
allmenus = collections.Counter()
rawhits = collections.Counter()
for disc in [int(x) for x in a.discs.split(',')]:
    img = Img(find_image(disc))
    for fid, sec, n in img.files(4):
        data = img.read(sec * 2048, n * 2048)
        for k, s in scripts(data):
            if k >> 16 != 5: continue
            field = k & 0xFFFF
            funcs = {}
            for e, tag, code in script_functions(data, s):
                funcs[(e, tag)] = code
            key = (field, hash(tuple(sorted((kk, v) for kk, v in funcs.items()))))
            if field in seen_fields and seen_fields[field][0] == key[1]:
                continue
            seen_fields[field] = (key[1], disc)
            # decode everything
            ins = {}
            bad = []
            for (e, tag), code in funcs.items():
                try:
                    ins[(e, tag)] = list(dis(code))
                except Exception as ex:
                    ins[(e, tag)] = []
                    bad.append((e, tag, str(ex)))
                for x in ins[(e, tag)]:
                    if x[1] is not None and x[3] == ['??'] and x[0] < len(code) - 4:
                        bad.append((e, tag, f'?? at {x[0]:#x}'))
                if a.raw:
                    starts = {x[0] for x in ins[(e, tag)]}
                    i = code.find(b'\x75')
                    while i >= 0:
                        if i not in starts and i + 3 < len(code) and code[i + 2] in (2, 4) and code[i + 1] == 0:
                            rawhits[(field, e, tag, code[i:i + 4].hex(' '))] += 1
                        i = code.find(b'\x75', i + 1)
            # uids: entry created as uid (0 -> uid = entry)
            uid2entry = {}
            player_entries = set()
            for (e, tag), lst in ins.items():
                for off, op, fl, args, raw in lst:
                    if op in (7, 8, 9):
                        ent, uid = args
                        uid2entry.setdefault(uid or ent, set()).add(ent)
                    if op == 0x2C:
                        player_entries.add(e)
            actors = sorted({ee for (e, tag), lst in ins.items() for off, op, fl, args, raw in lst if op == 9 for ee in [args[0]]})
            regions = sorted({ee for (e, tag), lst in ins.items() for off, op, fl, args, raw in lst if op == 8 for ee in [args[0]]})
            kindof = {**{r: 'region' for r in regions}, **{x: 'actor' for x in actors}}
            # all MENU instructions
            menus = {}
            for (e, tag), lst in ins.items():
                for off, op, fl, args, raw in lst:
                    if op == 0x75:
                        menus.setdefault((e, tag), []).append((off, fl, args, raw))
                        allmenus[(TYPES.get(args[0], args[0]) if isinstance(args[0], int) else 'expr', fl)] += 1

            def reach(e, tag, depth, path, calls_from):
                """MENUs reachable from function (e, tag): direct, or through CALL* to other objects."""
                out = []
                for off, fl, args, raw in menus.get((e, tag), []):
                    out.append((path + [(e, tag, off)], args, raw))
                if depth == 0: return out
                for off, op, fl, args, raw in ins.get((e, tag), []):
                    if op in (0x10, 0x12, 0x14):
                        uid, t = args[1], args[2]
                        if not isinstance(uid, int): continue
                        if uid == 0xFF: tgt = {e}
                        elif uid == 0xFA: tgt = player_entries
                        elif 0xFB <= uid <= 0xFE: tgt = player_entries
                        else: tgt = uid2entry.get(uid, {uid})
                        for te in tgt:
                            out += reach(te, t, depth - 1, path + [(e, tag, off, f'{op:02X}->uid{uid:#x}')], e)
                    elif op in (0x16, 0x18, 0x1A) and calls_from is not None:
                        out += reach(calls_from, args[1], depth - 1, path + [(e, tag, off, f'{op:02X}->caller')], e)
                return out

            found = []
            for ent in sorted(kindof):
                for tag in ((3, 8, 2) if kindof[ent] == 'region' else (3, 8)):
                    if (ent, tag) in funcs:
                        for pth, args, raw in reach(ent, tag, 3, [], None):
                            found.append((ent, tag, pth, args, raw))
            reached = {(pth[-1][0], pth[-1][1], pth[-1][2]) for ent, tag, pth, args, raw in found}
            unreached = [(e, t, off, args, raw) for (e, t), l in menus.items() for off, fl, args, raw in l if (e, t, off) not in reached]
            if found or unreached or bad:
                print(f'field {field} (disc {disc}) player entries {sorted(player_entries)}')
                for ent, tag, pth, args, raw in found:
                    ty = TYPES.get(args[0], args[0]) if isinstance(args[0], int) else args[0]
                    ar = args[1] if not isinstance(args[1], int) else f'{args[1]:#x}'
                    route = ' > '.join(f'e{x[0]}t{x[1]}+{x[2]:#x}' + (f'[{x[3]}]' if len(x) > 3 else '') for x in pth)
                    owner = pth[-1][0]
                    extra = ''
                    if not isinstance(args[1], int) and args[1].startswith('expr[d2 '):
                        idx = int(args[1].split()[1], 16)
                        pat = bytes([5, 0xd2, idx, 0x7d])
                        vals = sorted({struct.unpack_from('<h', c, i + 4)[0] for (ee, tt), c in funcs.items() if ee == owner
                                       for i in range(len(c) - 8) if c[i:i + 4] == pat and c[i + 6:i + 8] == b'\x2c\x7f'})
                        extra = f' local{idx} literal values {vals}'
                    print(f'  {kindof[ent]} entry {ent} tag {tag}: MENU {ty} arg {ar} ({raw.hex(" ")}) owner entry {owner} via {route}{extra}')
                    summary.append((field, disc, kindof[ent], ent, tag, owner, ty, ar, len(pth) > 1, extra))
                for e, t, off, args, raw in unreached:
                    ty = TYPES.get(args[0], args[0]) if isinstance(args[0], int) else args[0]
                    print(f'  UNREACHED from actor talk: entry {e} tag {t} +{off:#x}: MENU {ty} {args[1]} ({raw.hex(" ")})')
                for b in bad:
                    print('  BAD', b)
import json
if os.environ.get('MENUSCAN_JSON'):
    json.dump(summary, open(os.environ['MENUSCAN_JSON'], 'w'))
print('MENU totals (type, flags):', dict(allmenus))
if a.raw:
    print('raw 75 00 02/04 hits not at an instruction start:', len(rawhits))
    for k, v in sorted(rawhits.items())[:40]: print('  ', k, v)
