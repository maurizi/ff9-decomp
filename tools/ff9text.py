#!/usr/bin/env python3
"""Decode FF9 (NTSC-U) text and dump the game's id -> name tables.

    python3 tools/ff9text.py font [--png OUT]        # the font's glyph sheet, labelled with codes (needs FF9.IMG, PIL)
    python3 tools/ff9text.py decode 22 3F 3F 2D      # hex bytes -> text
    python3 tools/ff9text.py dump items              # id -> name; tables: items keyitems abilities support
                                                     #   commands messages cards chrnames
    python3 tools/ff9text.py dump items --macros ITEM_   # the same as C macro names

Encoding (read from the font itself: `font` renders every glyph next to its code). The font is item 0x0D0000 of
FF9.IMG dir index 1 file 0x271A (game_loadCommonImages), glyph sheet = TIM item 0x040000 of the same file (4 bpp,
VRAM 896,256). Font.glyphs[code] = u | v << 8 | width << 16 | advance << 28 for codes 0x00-0xB1 (Font.unk8 = 0xB2);
text_drawChar draws code c from glyphs[c], 13 px high. Code side: text_formatNumber writes digit d as code d and
pads with 0x0F (space); 0xFF ends a string, 0xF7 is a newline, 0xF9 / 0xFA take one argument byte
(text_buildBlock, msg_emitItemName).

Tables (offsets of a name pool + a record table holding u16 name offsets, all static data):
    items      main g_itemTable (0x12-byte records, name +0) + g_itemNames, ids 0x00-0xFF
    keyitems   main g_keyItemTable (8-byte records, name +0) + g_keyItemNames, ids 0x00-0xFF (msg ids 0x100+)
    abilities  ovl_33 s_menuAbilityActionTable (0x10-byte records, name +0xE) + s_menuAbilityActionNames, 0x00-0xBF
    support    main g_supportAbilities (8-byte records, name +2) + ovl_33 s_menuAbilitySupportNames, ids 0xC0-0xFF
    commands   ovl_0b g_btlCmdTable (0xC-byte records, name +0) + g_btlCmdNames, ids 0x00-0x2F
    cmdabilities  ovl_0b g_btlCmdTable count/first + g_btlCmdAbilities: the abilities each command lists
    messages   ovl_0b g_btlMessages (text table: u32 count?, u32 offsets from +4) - element/status words
    cards      main g_cardNames (text table, card_getName)
    chrnames   main g_chrDefaultNames (text table, chr_setDefaultName)
"""
import argparse
import os
import re
import struct
import sys

ROOT = os.path.dirname(os.path.dirname(os.path.abspath(__file__)))

# code -> text, from the glyph sheet (`font`). Glyphs without an obvious character are left as <XX>.
CHARMAP = {}
for _i in range(10):
    CHARMAP[_i] = str(_i)
CHARMAP.update({0x0A: '+', 0x0B: '-', 0x0C: '=', 0x0D: '*', 0x0E: '%', 0x0F: ' '})
for _i in range(26):
    CHARMAP[0x10 + _i] = chr(ord('A') + _i)
    CHARMAP[0x30 + _i] = chr(ord('a') + _i)
CHARMAP.update({0x2A: '(', 0x2B: '!', 0x2C: '?', 0x2D: '"', 0x2E: ':', 0x2F: '.',
                0x4A: ')', 0x4B: ',', 0x4C: '/', 0x4D: '·', 0x4E: '~', 0x4F: '&'})
for _i, _c in enumerate('ÀÁÂÄÈÉÊËÌÍÎÏÒÓÔÖ'
                        'ÙÚÛÜàáâäèéêëìíîï'
                        'òóôöùúûüÇÑçñŒß'):
    CHARMAP[0x50 + _i] = _c
CHARMAP.update({0x7E: "'", 0x7F: '“', 0x80: '_', 0x81: ']', 0x82: '[', 0x83: '∴', 0x84: '∵',
                0x85: '♪', 0x86: '→', 0x88: '×', 0x89: '◆', 0x8A: '§', 0x8B: '<', 0x8C: '>',
                0x8D: '←', 0x8F: '↑', 0x90: '△', 0x91: '□', 0x92: '∞', 0x93: '♥',
                0xA1: '«', 0xA2: '»', 0xA3: '↓', 0xA4: '―', 0xA5: '°', 0xA6: '★',
                0xAB: '„', 0xAC: '’', 0xAD: '#', 0xAF: ';', 0xB0: '¡', 0xB1: '¿'})

TEXT_END, TEXT_NEWLINE = 0xFF, 0xF7
ARG_CODES = (0xF9, 0xFA)  # take one argument byte

EXE_VRAM, EXE_HDR = 0x80010000, 0x800
OVL_VRAM = {'0b': 0x800A7000, '33': 0x801EDF00, '38': 0x801EDF00, '65': 0x801EDF00}


def decode(b, o=0, end=None):
    """Text at b[o:] up to 0xFF (or end)."""
    out = []
    end = len(b) if end is None else end
    while o < end and b[o] != TEXT_END:
        c = b[o]
        if c in ARG_CODES and o + 1 < end:
            out.append(f'<{c:02X}{b[o + 1]:02X}>')
            o += 2
            continue
        out.append('\n' if c == TEXT_NEWLINE else CHARMAP.get(c, f'<{c:02X}>'))
        o += 1
    return ''.join(out)


class Image:
    def __init__(self, path, vram, hdr=0):
        with open(path, 'rb') as f:
            self.b = f.read()
        self.vram, self.hdr = vram, hdr

    def off(self, addr):
        return addr - self.vram + self.hdr

    def u16(self, addr):
        return struct.unpack_from('<H', self.b, self.off(addr))[0]

    def u32(self, addr):
        return struct.unpack_from('<I', self.b, self.off(addr))[0]

    def text(self, addr):
        return decode(self.b, self.off(addr))


def disks():
    for d in (os.environ.get('FF9_DISKS'), os.path.join(ROOT, 'disks', 'us')):
        if d and os.path.exists(os.path.join(d, 'SLUS_012.51')):
            return d
    sys.exit('no disks/us/SLUS_012.51 (set FF9_DISKS)')


def exe():
    return Image(os.path.join(disks(), 'SLUS_012.51'), EXE_VRAM, EXE_HDR)


def ovl(n):
    return Image(os.path.join(disks(), 'ovl', f'{n}.bin'), OVL_VRAM[n])


def symbols(path):
    out = {}
    for line in open(os.path.join(ROOT, 'config', path)):
        m = re.match(r'\s*(\w+)\s*=\s*(0x[0-9A-Fa-f]+)\s*;', line)
        if m:
            out[m.group(1)] = int(m.group(2), 16)
    return out


def record_table(img, table, size, name_ofs, pool, count, first=0, pool_img=None):
    pool_img = pool_img or img
    return [(first + i, pool_img.text(pool + img.u16(table + i * size + name_ofs))) for i in range(count)]


def text_table(img, addr, count=None):
    """text_getFrom: entry k = table + 4 + (u32 at table + 4 + 4k & 0xFFFF)."""
    base = addr + 4
    if count is None:  # the offsets run up to the first string
        count = (img.u32(base) & 0xFFFF) // 4
    return [(k, img.text(base + (img.u32(base + 4 * k) & 0xFFFF))) for k in range(count)]


def table(kind):
    m = symbols('symbols.us.txt')
    if kind == 'items':
        e = exe()
        return record_table(e, m['g_itemTable'], 0x12, 0, m['g_itemNames'], 0x100)
    if kind == 'keyitems':
        e = exe()
        return record_table(e, m['g_keyItemTable'], 8, 0, m['g_keyItemNames'], 0x100)
    if kind == 'abilities':
        s = symbols('symbols.ovl_33.us.txt')
        o = ovl('33')
        return record_table(o, s['s_menuAbilityActionTable'], 0x10, 0xE, s['s_menuAbilityActionNames'], 0xC0)
    if kind == 'support':
        s = symbols('symbols.ovl_33.us.txt')
        return record_table(exe(), m['g_supportAbilities'], 8, 2, s['s_menuAbilitySupportNames'], 0x40, 0xC0,
                            pool_img=ovl('33'))
    if kind == 'commands':
        s = symbols('symbols.ovl_0b.us.txt')
        o = ovl('0b')
        return record_table(o, s['g_btlCmdTable'], 0xC, 0, s['g_btlCmdNames'], 0x30)
    if kind == 'cmdabilities':  # command -> the abilities its menu lists (single-ability commands: one id)
        s = symbols('symbols.ovl_0b.us.txt')
        o = ovl('0b')
        names = dict(table('abilities'))
        out = []
        for c, cname in table('commands'):
            r = s['g_btlCmdTable'] + c * 0xC
            count, first = o.b[o.off(r + 7)], o.u32(r + 8)
            ids = [o.b[o.off(s['g_btlCmdAbilities'] + first + k)] for k in range(count)] if count else [first]
            out.append((c, f'{cname}: ' + ', '.join(f'{a:#04x} {names.get(a, "?")}' for a in ids)))
        return out
    if kind == 'messages':
        s = symbols('symbols.ovl_0b.us.txt')
        return text_table(ovl('0b'), s['g_btlMessages'])
    if kind == 'cards':
        return text_table(exe(), m['g_cardNames'])
    if kind == 'chrnames':
        return text_table(exe(), m['g_chrDefaultNames'])
    sys.exit(f'unknown table {kind}')


def macro_name(prefix, text):
    t = text.replace('&', ' AND ').replace('+', ' PLUS ').replace("'", '').replace('’', '')
    t = ''.join(c if c.isascii() else {'é': 'E', 'è': 'E'}.get(c, '_') for c in t)
    t = re.sub(r'[^A-Za-z0-9]+', '_', t).strip('_').upper()
    return prefix + t


def font(png):
    sys.path.insert(0, os.path.dirname(os.path.abspath(__file__)))
    from encounter_catalog import Img, db_find, find_image, u16, u32
    from PIL import Image as PImage, ImageDraw
    img = Img(find_image(1))
    d = img.file(1, 0x271A)
    f = db_find(d, 0xD0000)
    count = u16(d, f + 8)
    o = db_find(d, 0x40000)
    p = o + 8
    if u32(d, o + 4) & 8:
        p += u32(d, p)
    w = u16(d, p + 8)
    pix = d[p + 12:]

    def px(u, v):
        b = pix[v * w * 2 + u // 2]
        return (b >> 4 if u & 1 else b) & 3

    s = 6
    out = PImage.new('L', (16 * 14 * s, ((count + 15) // 16) * 16 * s), 255)
    dr = ImageDraw.Draw(out)
    for c in range(count):
        g = u32(d, f + 12 + 4 * c)
        u, v, gw = g & 0xFF, g >> 8 & 0xFF, g >> 16 & 0xF
        ox, oy = c % 16 * 14 * s, c // 16 * 16 * s
        for yy in range(13):
            for xx in range(gw):
                val = px(u + xx, v + yy)
                if val:
                    dr.rectangle([ox + xx * s, oy + (yy + 2) * s, ox + xx * s + s - 1, oy + (yy + 2) * s + s - 1],
                                 fill=[255, 0, 90, 170][val])
        dr.text((ox, oy), f'{c:02X} {CHARMAP.get(c, "")}'.encode('latin-1', 'replace').decode('latin-1'), fill=0)
    out.save(png)
    print(f'{count} glyphs -> {png}')


def main():
    ap = argparse.ArgumentParser(description=__doc__, formatter_class=argparse.RawDescriptionHelpFormatter)
    sub = ap.add_subparsers(dest='cmd', required=True)
    p = sub.add_parser('font')
    p.add_argument('--png', default='build/font.png')
    p = sub.add_parser('decode')
    p.add_argument('bytes', nargs='+')
    p = sub.add_parser('dump')
    p.add_argument('table')
    p.add_argument('--macros', metavar='PREFIX')
    a = ap.parse_args()
    if a.cmd == 'font':
        font(a.png)
    elif a.cmd == 'decode':
        print(decode(bytes(int(x, 16) for x in ' '.join(a.bytes).split())))
    else:
        for i, name in table(a.table):
            if a.macros:
                print(f'#define {macro_name(a.macros, name)} 0x{i:02X}')
            else:
                print(f'{i:#05x} {name!r}')


if __name__ == '__main__':
    main()
