"""Dump the Tetra Master TmState (tm_mainLoop's local) from the running emulator.
usage: tmdump.py [ADDR]"""
import os, sys, struct
sys.path.insert(0, os.path.dirname(os.path.abspath(__file__)))
from emu_lib import *
S = int(sys.argv[1], 0) if len(sys.argv) > 1 else 0x801FF9E0
b = rd(S, 0x5D0)
def i32(o): return struct.unpack_from('<i', b, o)[0]
def u32(o): return struct.unpack_from('<I', b, o)[0]
TASKS = ['PLAYER_SELECT','PLAYER_SET','ENEMY_SELECT','RESULT','INIT','FLASH','BATTLE_SELECT','BATTLE','TURN','BATTLE_WIN',
         'BATTLE_LOSE','TRY_AGAIN','EXIT','CARD_CHANGE','CARD_MOVE','FLASH2','TRADE','CHANGE2','CHOICE','CHOICE2',
         'STAGE_SELECT','FIELD_FADE','AWAY_INFO','CARD_AWAY','CARD_AWAY2','CARD_LESS5','EXTENSION','END_FADE','COIN',
         'FLASH_ARROW','ENEMY_THINK','?31','?32','ENEMY_APPEAR','GET_SELECT','GAME_QUIT']
t = i32(0)
print(f'TmState @{S:08X} task={t} {TASKS[t] if 0 <= t < len(TASKS) else "?"} cursor={i32(4)} board=({i32(8)},{i32(12)}) '
      f'timer={i32(0x2E8)} phase={i32(0x418)} turn={i32(0x434)} result={i32(0x45C)} opp={i32(0x450)} deck={i32(0x448)} ai={i32(0x44C)}')
print(f' stock col={i32(0x474)} row={i32(0x478)} handCount={i32(0x47C)} copyIdx={i32(0x4B4)} getCursor={i32(0x5B0)} quitChoice={i32(0x5B4)} '
      f'src=({i32(0x424)},{i32(0x428)}) dst=({i32(0x42C)},{i32(0x430)}) numTargets={i32(0x4AC)} wins/losses/draws={i32(0x43C)}/{i32(0x440)}/{i32(0x444)} '
      f'show score/board/blocks={i32(0x464)}/{i32(0x468)}/{i32(0x46C)} unk5C8={struct.unpack_from("<4h", b, 0x5C8)}')
for k in range(10):
    o = 0x10 + k * 0x3C
    cid, arr, pw, ty, pd, md = b[o:o + 6]
    print(f' card{k} @{S+o:08X} id={cid:3d} arrows={arr:08b} stats={pw>>4:X}{"PMXA"[ty&3]}{pd>>4:X}{md>>4:X} (raw {pw},{ty},{pd},{md}) hidden={i32(o+8)} '
          f'xy=({i32(o+12)},{i32(o+16)}) owner={i32(o+20)} onBoard={i32(o+24)} small={i32(o+28)} depth={i32(o+0x30)} visible={i32(o+0x34)}')
for y in range(4):
    row = []
    for x in range(4):
        o = 0x268 + (y * 4 + x) * 8
        st, cp = i32(o), u32(o + 4)
        if st == 0:
            k = (cp - S - 0x10) // 0x3C
            row.append(f'c{k}')
        else:
            row.append(['C', '..', 'XX', 'off'][st])
    print(' board', ' '.join(f'{c:>3}' for c in row))
