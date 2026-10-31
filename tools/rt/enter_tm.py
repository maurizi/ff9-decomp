"""Force a Tetra Master game from a paused field: field cleanup to main's dispatcher, then mode 9 + cardOpponent.
usage: enter_tm.py [OPPONENT] [FRAMES]"""
import os, sys, struct
sys.path.insert(0, os.path.dirname(os.path.abspath(__file__)))
from emu_lib import *
opp = int(sys.argv[1], 0) if len(sys.argv) > 1 else 12
frames = int(sys.argv[2]) if len(sys.argv) > 2 else 0
g_s = gs(); g = gd()
assert rd(g_s + 8, 1) == b'\x01', 'not in field'
bp = B('addBp', 0x80012834, 'Exec', 4, 'break', 1, 'after_field_cleanup')
try:
    wr(g_s + 8, b'\x02'); wr(g_s + 10, b'\x01'); w32(g_s, r32(g_s) | 2)
    emu.run_frames(600)
    assert B('status')['pc'] == 0x80012834, 'no dispatcher'
finally:
    B('removeBp', bp)
wr(g + 0x8b2, struct.pack('<h', opp))
wr(g_s + 8, b'\x09'); wr(g_s + 10, b'\x01')
if frames:
    run(frames)
print('ok mode', rd(g_s + 8, 1).hex(), 'opp', r16(g + 0x8b2))
