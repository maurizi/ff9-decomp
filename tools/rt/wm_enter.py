#!/usr/bin/env python3
"""wm_enter.py [--base-state field_ice_300] [--hold down] [--poke ADDR=HEX ...] [--frames N] [--save NAME] [--shot P]
Disc 1: walk out of a field onto the world map (hold a direction until main's dispatcher 0x80012834 breaks after the
field cleanup), apply the pokes there (story counter, event bytes: nothing of the field reacts to them any more),
then let ovl_0c's wm_init run. Logs func_800C45D8's return value (bp at wm_init+0x10C) and GameData.flags."""
import argparse, os, subprocess, sys
ROOT = os.path.dirname(os.path.dirname(os.path.dirname(os.path.abspath(__file__))))
sys.path.insert(0, ROOT + '/tools/rt')
from emu_lib import *
p = argparse.ArgumentParser()
p.add_argument('--base-state', default='field_ice_300'); p.add_argument('--hold', default='down')
p.add_argument('--poke', action='append', default=[]); p.add_argument('--frames', type=int, default=400)
p.add_argument('--trace', action='append', default=[]); p.add_argument('--world', type=int, help='world file 9000+k: load dir-3 file k instead (GameData.unk7FC[1]/unk804[1])'); p.add_argument('--save'); p.add_argument('--shot')
a = p.parse_args()
emu.http('/state/load?name=' + a.base_state); emu.http('/execution-flow?function=pause', 'POST')
bp = B('addBp', 0x80012834, 'Exec', 4, 'break', 1, 'wm_enter')
try:
    for i in range(40):
        B('hold', [a.hold.upper()], 20); run(20)
        if B('status')['pc'] == 0x80012834: break
    else:
        raise SystemExit('no mode switch')
finally:
    B('removeBp', bp)
if a.world is not None:
    sys.path.insert(0, ROOT + '/tools')
    from encounter_catalog import Img
    f = Img(ROOT + '/disks/us/disc1/FF9.IMG').files(3)[a.world - 9000]
    w32(gd() + 0x800, f[1]); w32(gd() + 0x808, f[2]); print('world file', hex(f[0]), f[1], f[2])
for pk in a.poke:
    ad, h = pk.split('='); wr(emu.parse_addr(ad), bytes.fromhex(h))
lg = B('addBp', 0x800A8C20, 'Exec', 4, 'log', 4, 'wm_init_ret')
tr = {t: B('addBp', emu.parse_addr(t), 'Exec', 4, 'log', 40, t) for t in a.trace}
run(a.frames)
log = B('takeLog', lg); B('removeBp', lg)
for t, i in tr.items():
    l = B('takeLog', i); B('removeBp', i)
    print(t, l['hits'], [(h['frame'], hex(h['a0']), hex(h['a1']), hex(h['a2']), hex(h['a3'])) for h in l['log']][:12])
print('func_800C45D8 returned', [(h['frame'], h['v0']) for h in log['log']], 'hits', log['hits'])
print('GameData.flags %08x' % r32(gd()), 'story %04x' % r16(0x8007AEF0), 'b[0xBE] %02x' % rd(0x8007AFAE, 1)[0], 'mode', rd(gs() + 8, 1).hex())
if a.shot: subprocess.run([ROOT + '/tools/emu.py', 'shot', a.shot], stdout=subprocess.DEVNULL)
if a.save: print(emu.http('/state/save?name=' + a.save).decode().strip())
