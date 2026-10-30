#!/usr/bin/env python3
"""enter_field.py FIELD [--disc N] [--base-state S] [--frames N] [--save NAME] [--poke ADDR=HEX ...] [--shot P]
Adapted from ff9-ds tools/psx_field_fixture.py, keeping the save's own party and event variables (no host export):
from a paused loaded field, force the field exit, break at main's dispatcher, write the target field's CD extents
(FF9.IMG dirs 4/2), GameData.fieldId and unk3A (area), mode 1, then let the normal field loader run."""
import argparse, os, struct, sys, subprocess
ROOT = os.path.dirname(os.path.dirname(os.path.dirname(os.path.abspath(__file__))))
sys.path.insert(0, ROOT + '/tools/rt')
from emu_lib import *

SECTOR = 2048
DISPATCH = 0x80012834
BASE = {1: 'field_dali_inn', 2: 'field_iifa_tree', 3: 'disc3_pandemonium_lv99_field', 4: 'ds_perf_disc4_treno'}


def directory(image, index):
    image.seek(0)
    header = image.read(SECTOR)
    kind, count, table, _ = struct.unpack_from('<4I', header, 16 + index * 16)
    image.seek(table * SECTOR)
    data = image.read((count + 1) * 8)
    return [struct.unpack_from('<HHI', data, i * 8) for i in range(count + 1)]


def field_extents(path, field):
    with open(path, 'rb') as image:
        fields = directory(image, 4)
        for i, (fid, link, sector) in enumerate(fields[:-1]):
            if fid == field:
                fsec, flen = sector, fields[i + 1][2] - sector
                break
        else:
            raise SystemExit(f'field {field} not on this disc')
        areas = directory(image, 2)
        for i, (aid, _, sector) in enumerate(areas[:-1]):
            if aid == link:
                return link, [sector, fsec], [areas[i + 1][2] - sector, flen]
    raise SystemExit('area not found')


def enter(field, disc=1, base=None, frames=600, pokes=(), pre_pokes=()):
    area, sectors, lengths = field_extents(f'{ROOT}/disks/us/disc{disc}/FF9.IMG', field)
    if base != '-':
        emu.http('/state/load?name=' + (base or BASE[disc]))
        emu.http('/execution-flow?function=pause', 'POST')
    g = gs(); d = gd()
    for a, h in pre_pokes:
        wr(a, bytes.fromhex(h))
    bp = B('addBp', DISPATCH, 'Exec', 4, 'break', 1, 'enter_field')
    try:
        wr(g + 10, b'\x01')
        w32(g, r32(g) | 2)
        run(600)
        if B('status')['pc'] != DISPATCH:
            raise SystemExit('field exit did not reach the dispatcher')
    finally:
        B('removeBp', bp)
    for i in range(2):
        w32(d + 0x7FC + i * 4, sectors[i])
        w32(d + 0x804 + i * 4, lengths[i])
    wr(d + 12, struct.pack('<H', field))
    wr(d + 0x3A, struct.pack('<H', area))
    for a, h in pokes:
        wr(a, bytes.fromhex(h))
    wr(g + 8, b'\x01')
    wr(g + 10, b'\x01')
    if frames:
        run(frames)
    return area


if __name__ == '__main__':
    p = argparse.ArgumentParser()
    p.add_argument('field', type=int)
    p.add_argument('--disc', type=int, default=1)
    p.add_argument('--base-state')
    p.add_argument('--frames', type=int, default=600)
    p.add_argument('--save')
    p.add_argument('--poke', action='append', default=[])
    p.add_argument('--pre-poke', action='append', default=[])
    p.add_argument('--shot')
    a = p.parse_args()
    pk = [(emu.parse_addr(x.split('=')[0]), x.split('=')[1]) for x in a.poke]
    ppk = [(emu.parse_addr(x.split('=')[0]), x.split('=')[1]) for x in a.pre_poke]
    area = enter(a.field, a.disc, a.base_state, a.frames, pk, ppk)
    print('area', area, 'fieldId now', r16(gd() + 12), 'mode', rd(gs() + 8, 1).hex(), 'frame', B('status')['frame'])
    if a.shot:
        subprocess.run([ROOT + '/tools/emu.py', 'shot', a.shot], stdout=subprocess.DEVNULL)
    if a.save:
        emu.http('/state/save?name=' + a.save)
        print('saved', a.save)
