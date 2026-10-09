# Near misses and puzzles (candidates for a dedicated hard-case pass)

Functions agents got close on but couldn't finish. Drafts for main are in the main checkout's
build/drafts/main/ (gitignored; v*.c). Append as agents report.

## main
- Register allocation / scheduling / branch order (>=90%): 80018F50 (126/128), 8002C308, 80029058,
  8004F2B4, 80054E88, 80057534, 8001C7FC, 80021B34, 800221C0, 800554D4, 80055230, 800343C0, 8005F718,
  800616F8, 8002FED4, 8002ECA0, 8002E718, 80062118, 800621D4, 8005B1B8, 8005AEC8, 8002B644.
- Order dependency: 800577EC / 80057898 match alone but break 800584C8; need 800579B4, 80057D80,
  80058118 matched first (work in address order).
- Loop shapes (search loop without gcc's empty-list shortcut; bit scans): 8004B040, 8004AE00,
  8004B098, 8004B4C4, 80038150, 80038278, 8004B85C, 8004AB7C, 80037738, 80049B48, 8002AE7C.
- Large-function pass (drafts in the agent worktree's build/nm_*.c, scores matched/total insns):
  8001C8B0 (prologue arg moves), 8002995C 59/125 (s1/s2 swap), 800378C4 77/127 (f<14 branch shape),
  80029B4C 108/133 (needs u16 clut view of D_800686D0; ovl_0b uses Unk800686E8), 8005DB40 (loop
  hoisting of D_80080A10 load), 80035214 120/135 (compare scheduling), 80039444 (reassociation),
  8004DAD0 (cross-jumped SE calls), 80038618 137/221 (inline-param copy), 8004F9A0 (extra prim ptr
  copy), 80054650 74/142 (a3 vs t2), 80062E3C (separate giv for node+4), 8003595C.
- 800579B4 matches alone (243/243) but breaks 800584C8 unless 80057D80 (215/230) and 80058118
  (229/236) are also C; both differ only in LFO temp regs (a0 vs v1). Drafts build/b36.c, b37.c.
