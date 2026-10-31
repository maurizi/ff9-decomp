#!/usr/bin/env python3
"""What plays each battle effect: labels for the FF9.IMG dir 13 effect files, computed from the game data.

    python3 tools/effect_labels.py                  # effect id -> label, every id something plays
    python3 tools/effect_labels.py --id 0xB2        # one id
    python3 tools/effect_labels.py --enemy 899      # the effect ops of one encounter's enemy sequences
    python3 tools/effect_labels.py --json OUT       # raw per-id sources as JSON

Effect id = dir-13 file id (btlfx_start -> btlfx_resolveFirstUse -> btlfx_loadFile; docs/runtime-findings.md,
"Round 31 runtime pass"). A label joins every source that plays the id, in this order:
- party abilities: ovl_33 s_menuAbilityActionTable (BtlCmd8 records: effect = bits 8-16 of word 0, altEffect u16
  +0xC). Target type 3 plays `effect` for several targets and `altEffect` for one ("all targets" / "one target");
  others play altEffect for short summons ("short"), the Seiken commands ("Seiken") and a few others ("alt").
- the fixed ids of btl_startCmdEffect (Attack by appearance kind, Steal, Throw by weapon icon, Trance) below;
- items: main g_usableItemTable (12-byte records, effect = bits 8-16 of word 0), items 0xE0+;
- enemies: every encounter bundle (FF9.IMG dir index 5) on every disc: kind 0x10 header {u8 ?, groups, types,
  abilities}, enemy ability k -> sequence k of the kind 0x11 bundle, whose btlseq ops 0x06 / 0x08 / 0x1A carry an
  effect id (op lengths as g_btlseqOps); names from the kind 6 text block (enemy type names, then ability names).
Single-target spells on a large target (radius 0x385+) play the all-targets file instead (g_btlfxLargeTargetFx).

Text comes from tools/ff9text.py (the exe / overlays under disks/us, or $FF9_DISKS), discs via
tools/encounter_catalog.py find_image (--disks DIR holding discN/FF9.IMG, else $FF9_DISKS, else disks/). Labels are
decoded game text: print or use them, never commit them as a table (tools/effect_modules.py catalogue calls label()).
stdlib only, runs on the host.
"""
import argparse
import collections
import json
import os
import re
import struct
import sys

sys.path.insert(0, os.path.dirname(os.path.abspath(__file__)))
import ff9text as T  # noqa: E402
from encounter_catalog import SECTOR, Img, find_image  # noqa: E402

ENC_DIR = 5  # FF9.IMG dir index of the encounter bundles (file id = battle id)
# btlseq opcode -> instruction length (g_btlseqOps); ops 0x00, 0x18 and >= 0x22 end a sequence
SEQ_LEN = {0: 0, 1: 2, 2: 1, 3: 4, 4: 2, 5: 2, 6: 5, 7: 1, 8: 9, 9: 1, 0xA: 1, 0xB: 1, 0xC: 4, 0xD: 3, 0xE: 2, 0xF: 3,
           0x10: 2, 0x11: 2, 0x12: 2, 0x13: 8, 0x14: 6, 0x15: 2, 0x16: 2, 0x17: 2, 0x18: 0, 0x19: 6, 0x1A: 9,
           0x1B: 8, 0x1C: 2, 0x1D: 2, 0x1E: 4, 0x1F: 2, 0x20: 2, 0x21: 2}
SEQ_FX_OPS = (0x06, 0x08, 0x1A)  # ops whose u16 at +1 is an effect id
NO_REF = 'no reference found (no ability, item, enemy sequence or code id plays it)'

# Ability ids whose menu name needs a qualifier (several abilities share a name).
SWORD_MAGIC = (0x32, 0x34, 0x36, 0x38, 0x39, 0x3B, 0x3D, 0x3F, 0x41, 0x4B, 0x4C, 0xBD, 0xBF)
ABILITY_NAME = {0x43: 'Fenrir (2nd)', 0x45: 'Carbuncle (2)', 0x46: 'Carbuncle (3)', 0x47: 'Carbuncle (4)',
                0xBA: 'Spear (2nd)', 0xB2: 'Jump (2nd)'}
ELAN = range(0x85, 0x8D)      # Eiko/Quina-style "(Elan)" duplicates of the Trance commands
SHORT = range(0x31, 0x4B)     # summons: altEffect = the short version
SEIKEN = range(0x95, 0x99)    # Seiken commands: altEffect = the Seiken version
GENERIC_CMDS = ('None', 'Attack', 'Defend', 'Flee', 'Change', 'Throw', 'Void')
ALT_NONE = 0xA                # altEffect value that means "none" in the table

# btl_startCmdEffect: ids it plays itself.
FIXED = {
    0x64: 'Attack Zidane (dagger)', 0x65: 'Attack Zidane (thief sword)', 0x66: 'Attack Vivi',
    0x67: 'Attack Dagger (rod, long hair)', 0x68: 'Attack Dagger (racket, long hair)',
    0x69: 'Attack Dagger (rod, short hair)', 0x6A: 'Attack Dagger (racket, short hair)', 0x6B: 'Attack Steiner',
    0x6C: 'Attack (appearance kind 8, unused)', 0x6D: 'Attack Quina', 0x6E: 'Attack Eiko (rod)',
    0x6F: 'Attack Eiko (racket)', 0x70: 'Attack Freya', 0x71: 'Attack Amarant', 0x72: 'Attack Cinna',
    0x73: 'Attack Marcus', 0x74: 'Attack Blank', 0x75: 'Attack Blank (story 1500-1599)', 0x76: 'Attack Beatrix',
    0x77: 'enemy/Marcus Attack, Marcus Steal',
    0xC8: 'Steal Zidane (dagger)', 0x111: 'Steal Zidane (thief sword)', 0x13: 'Steal Cinna', 0x14: 'Steal (other kinds)',
    0x110: 'Throw dagger', 0x10A: 'Throw thief sword', 0x10B: 'Throw sword', 0x10C: 'Throw spear',
    0x10D: 'Throw claws', 0x109: 'Throw racket', 0x10E: 'Throw rod/flute/staff', 0x10F: 'Throw fork',
    0x115: 'Throw pinwheel/rising sun', 0x1E9: 'Trance command (not in Trance)', 0x101: 'Trance transformation',
}
# Enemy ability names that say nothing about the effect (used only when nothing better exists).
GENERIC = re.compile(r'^(Battlemes|MES|Attack\d?$|Counter$|<|Continue|No action|None|Death\d?$|End\d)')

PRIO_PARTY, PRIO_ITEM, PRIO_ENEMY = 0, 1, 2


def u16(d, o):
    return struct.unpack_from('<H', d, o)[0]


def u32(d, o):
    return struct.unpack_from('<I', d, o)[0]


def db_kinds(d):
    """{kind: offset of the first item of the (last) block of that kind} of a DB container (db_lookup's layout)."""
    out = {}
    for i in range(d[1]):
        v = u32(d, 4 + i * 4)
        b = 4 + i * 4 + (v & 0xFFFFFF)
        n = d[b + 1]
        ofs = (b + 4 + n * 2 + 3) & ~3
        out[v >> 24 & 0x7F] = ofs + (u32(d, ofs) & 0xFFFFFF)
    return out


def encounters(path):
    """(battle id, bundle bytes) of every present encounter file of one FF9.IMG."""
    img = Img(path)
    for fid, sec, n in img.files(ENC_DIR):
        if n > 0:
            yield fid, img.read(sec * SECTOR, n * SECTOR)


def parse_encounter(d):
    """(enemy type names, [(ability name, [(op, effect id)])])."""
    bl = db_kinds(d)
    hdr, text, seq = bl[0x10], bl[6], bl[0x11]
    ntypes, nabil = d[hdr + 2], d[hdr + 3]

    def name(j):
        return T.decode(d, text + 4 + u16(d, text + 4 + j * 4))

    enemies = [name(j) for j in range(ntypes)]
    h = seq + u16(d, seq)
    nseq = u16(d, h)
    out = []
    for k in range(nabil):
        try:
            an = name(ntypes + k)
        except Exception:
            an = '?'
        fx = []
        if k < nseq:
            p = h + u16(d, h + 4 + k * 2)
            for _ in range(400):
                op = d[p]
                if op not in SEQ_LEN or op in (0, 0x18):
                    break
                if op in SEQ_FX_OPS:
                    fx.append((op, u16(d, p + 1)))
                p += SEQ_LEN[op]
        out.append((an, fx))
    return enemies, out


def enemy_sources(disks=None):
    """{effect id: {'abil': {name: count}, 'enemies': [...], 'n_enc': n}} over all discs found."""
    fx = collections.defaultdict(dict)  # ordered sets: ties keep disc / battle / ability order
    errs = 0
    for disc in (1, 2, 3, 4):
        path = find_image(disc, disks)
        if not path:
            continue
        for key, d in encounters(path):
            try:
                enemies, abil = parse_encounter(d)
            except Exception:
                errs += 1
                continue
            group = '/'.join(sorted(set(enemies)))
            for an, ops in abil:
                for _, i in ops:
                    fx[i][(an, group, key)] = None
    if errs:
        print(f'effect_labels: {errs} encounter bundles did not parse', file=sys.stderr)
    res = {}
    for i in sorted(fx):
        names = collections.Counter(a for a, _, _ in fx[i])
        res[i] = {'abil': dict(names), 'enemies': sorted(set(e for _, e, _ in fx[i]))[:6],
                  'n_enc': len(set(k for _, _, k in fx[i]))}
    return res


def sources(disks=None):
    """{effect id: [(priority, text)]} from every source."""
    L = collections.defaultdict(list)
    s = T.symbols('symbols.ovl_33.us.txt')
    o = T.ovl('33')
    names = dict(T.table('abilities'))
    tab = s['s_menuAbilityActionTable']
    for i in range(0xC0):
        w = o.u32(tab + i * 16)
        ttype, eff, alt = w & 15, (w >> 8) & 0x1FF, o.u16(tab + i * 16 + 0xC)
        n = names[i]
        if i in SWORD_MAGIC:
            n = 'Sword Magic ' + n
        elif i in ELAN:
            n = n + ' (Elan)'
        elif i in ABILITY_NAME:
            n = ABILITY_NAME[i]
        if (n in GENERIC_CMDS and i >= 0xA1) or i == 0:
            continue
        if ttype == 3:
            L[eff].append((PRIO_PARTY, f'{n} (all targets)'))
            L[alt].append((PRIO_PARTY, f'{n} (one target)'))
        else:
            L[eff].append((PRIO_PARTY, n))
            if alt != ALT_NONE and alt != eff:
                tag = 'short' if i in SHORT else 'Seiken' if i in SEIKEN else 'alt'
                L[alt].append((PRIO_PARTY, f'{n} ({tag})'))
    m = T.symbols('symbols.us.txt')
    e = T.exe()
    items = dict(T.table('items'))
    for i in range(0x1F):
        eff = (e.u32(m['g_usableItemTable'] + i * 12) >> 8) & 0x1FF
        if eff:
            L[eff].append((PRIO_ITEM, f'item {items[0xE0 + i]}'))
    for k, v in FIXED.items():
        L[k].append((PRIO_PARTY, v))
    for i, v in enemy_sources(disks).items():
        ab = collections.Counter(v['abil'])
        good = [a for a, _ in ab.most_common() if not GENERIC.match(a)]
        if not good:
            good = [a for a, _ in ab.most_common()][:1]
        ens = [x for x in v['enemies'] if not x.startswith('<')]
        tag = ', '.join(good[:4]) + (' ...' if len(good) > 4 else '')
        who = (' (' + ens[0] + (', ...' if len(ens) > 1 else '') + ')') if ens else ''
        L[i].append((PRIO_ENEMY, f'enemy {tag}{who}'))
    return L


class Labels:
    def __init__(self, disks=None):
        self.src = sources(disks)

    def label(self, i):
        """'; '-joined sources of effect id i ('' if nothing plays it)."""
        seen = []
        for _, t in sorted(self.src.get(i, [])):
            if t not in seen:
                seen.append(t)
        return '; '.join(seen)

    def row(self, ids, group=None, limit=120):
        """The effect-column text of a module shared by `ids` (catalogue rows)."""
        lab = '; '.join(filter(None, (self.label(i) for i in ids))) or NO_REF
        if group is not None:
            lab = f'{lab} [group {group}]'
        if len(lab) > limit:
            lab = lab[:limit].rsplit(',', 1)[0] + ' ...'
        return lab.replace('|', '/')


def main():
    ap = argparse.ArgumentParser(description=__doc__, formatter_class=argparse.RawDescriptionHelpFormatter)
    ap.add_argument('--disks', help='directory holding discN/FF9.IMG')
    ap.add_argument('--id', type=lambda x: int(x, 0))
    ap.add_argument('--enemy', type=int, metavar='BATTLE', help="one encounter's enemy sequences (every disc)")
    ap.add_argument('--json', metavar='OUT')
    a = ap.parse_args()
    if a.enemy is not None:
        for disc in (1, 2, 3, 4):
            path = find_image(disc, a.disks)
            for key, d in encounters(path) if path else ():
                if key == a.enemy:
                    enemies, abil = parse_encounter(d)
                    print(f'disc {disc}: {enemies}')
                    for k, (an, fx) in enumerate(abil):
                        print(f'  {k:2} {an!r} ' + ' '.join(f'op{op:02X}:{i:#x}' for op, i in fx))
        return
    if a.json:
        with open(a.json, 'w') as f:
            json.dump({hex(k): v for k, v in enemy_sources(a.disks).items()}, f, indent=0)
        return
    lab = Labels(a.disks)
    ids = [a.id] if a.id is not None else sorted(lab.src)
    for i in ids:
        print(f'{i:#05x} {lab.label(i) or NO_REF}')


if __name__ == '__main__':
    main()
