# Near misses and puzzles (candidates for a dedicated hard-case pass)

Functions agents got close on but couldn't finish. Drafts for main are in the main checkout's
build/drafts/main/ (gitignored; v*.c). Append as agents report.

## main
- Register allocation / scheduling / branch order (>=90%):
  8005F718,
  800616F8, 8002FED4, 8002ECA0, 8002E718, 80062118, 800621D4, 8005B1B8, 8005AEC8, 8002B644.
- gcc 2.8.1 cross-function leak (explains the old "order dependency" of 800584C8): combine.c
  only resets its static `mem_last_set` *after* the initial nonzero-bits scan, so during that scan
  the cuid of the previous function's last store/call is still there. A `(reg) = (mem)` whose insn
  cuid is <= that stale value is treated as clobbered, `nonzero_bits` gives the full mask, and a
  later `zero_extend` of that u16/u8 register is not removed (extra `andi`). Any predecessor
  that ends with a store leaks; an empty function in between resets it. The original build was
  multi-file, so some functions were compiled in a fresh state. Fix in C: write such functions so
  the extension is not needed (800584C8 now hoists `m = (u16)e->unk4` and keeps `s32 min`).
  80057D80 (97/226 draft build/hc/d80b.c) and 80058118 still open: product temps are `mflo a2` in
  the original (global.c prefers the register with the fewest local refs) but `a1` for us.
- Loop shapes (search loop without gcc's empty-list shortcut; bit scans): 80038278, 8004B85C.
  (8004B040, 8004AE00, 8004B098, 8004B4C4, 80038150, 80049B48, 8002AE7C, 80037738, 8004AB7C
  are matched.) The script bit-scan idiom (one peeled iteration, `srav`/`and` by the register
  holding 1) is `i = 0; if (!(mask & 1)) { i = 1; mask >>= 1; while (i < 8 && !(mask & 1)) {
  mask >>= 1; i++; } }` (statement order of `i = 1`/`mask >>= 1` decides `sra` vs `srav`).
  8004B85C is at 42/46 (build/hc/k/d1.c): the loop lives in an inline helper
  `find(key, slot)` called as `find(key_inline(slot), slot)` so the key temp is copied into a
  fresh pseudo (`addu a3,v1,zero`), and the kind test is `ge = kind >= 14; if (slot >= 8) { if
  (kind < 14) continue; } else { if (ge) continue; } break;`. Only the registers of `slti`/`xori`
  are swapped (we get lt=v0, ge=v1): `lt` inherits a v0 preference from the local `kind`
  (global.c set_preference), the original must have lacked it; not found.
- Large-function pass (drafts in the agent worktree's build/nm_*.c, scores matched/total insns):
  8001C8B0 (prologue arg moves), 8002995C 59/125 (s1/s2 swap), 800378C4 77/127 (f<14 branch shape),
  80029B4C 108/133 (needs u16 clut view of D_800686D0; ovl_0b uses Unk800686E8), 8005DB40 (loop
  hoisting of D_80080A10 load), 80035214 120/135 (compare scheduling), 80039444 (reassociation),
  8004DAD0 (cross-jumped SE calls), 80038618 137/221 (inline-param copy), 8004F9A0 (extra prim ptr
  copy), 80054650 74/142 (a3 vs t2), 80062E3C (separate giv for node+4), 8003595C.

## ovl_0b (battle)
- Register allocation / scheduling (left as INCLUDE_ASM): C9A28 (2 insns), AB07C, F5064, CBA58, C9794,
  BFF20, ED3F8, CCDB8, DA4C4, C1594 (strength-reduced loop), C0C00, DA138, E3B70, D07B4 (39/41), F28F0,
  DA1F4, E9000, EE24C, EE154, D9D58, E651C, D7094 (unfilled delay slot after jalr), C3F78 (one branch
  inverted), E8B28, EE7F8, EE938, D0E14, AB810, EFD1C, C9374, AE4FC, ED02C, ED7C0, E1534.
- Operand order seems to need byte-offset pointer arithmetic: DCCE4, BE1E0.
- Odd argument-slot stack use: E4A3C.
- Large-function pass 2 (drafts in the agent worktree's build/nm_<addr>.c; "~N" = differing insns):
  80047B90 ~35 (needs &v held in s0 with m[1][1] stored via s0+0x10), 80024BD8 ~12 (POLY_G4 store
  scheduling), 8005A320 ~17 (D_80080A70.unkC mask association, relocation scheduling), 8005C8BC ~15
  (r/s1 and %hi(D_80080970)/a3 allocation), 8004D2C0 ~21 (jump-table address hoisted out of loop),
  8005963C ~50, 8005681C ~66 and 80023A6C (store/load scheduling), 800283E0 (sprite pair, inline
  helper shape unknown), 8001DE50 (draft only), 80030EE4 (dead copy of arg2; maybe libgte RotMatrix*).
  Also 8005CF04 ~75 (v/old vs %hi(D_80080A10) s-register priority), 8004DCEC (window-init inline
  boundary; caller stores get rescheduled), 8005D6BC ~76 (&D_80083158 hoisted to fp, tick count spilled).
- Pass 4 (drafts in the agent worktree's build/nm_*.c): E4C88 4 rows (copy of the E788C result scheduled early), AD90C ~44 (unk851 test inverted to bnez, kind-7 delay slot, gx/m regs), DA92C ~80 (temp regs t0/v0, loop copies), D650C ~70 (op/a copied into a0 for the switches, cross-jump of cont=0 tails).

## ovl_0a / ovl_0c (field / world map)
- 0a C4EC8, C31F8: 1 instruction each (constant load vs register copy).
- 0a B4A5C: identical except the stack frame is 8 bytes too small.
- 0a C0038 (11 diff rows), C07D8 (24), C970C, BA560, BD130, C65CC, BFEE0 (36-64).
- 0c C53DC and BEECC (~110-120); twin pair 0a BB714 / 0c CA1D8 (large diffs).
- Pass 5 (drafts in the agent worktree's build/nm_0a_<addr>.c; rows = aligned diff rows, build/ad.py): AB3CC 4 (only
  the prologue order of `li s4,100` / `move s1,zero`; needs c = list[i], lim/cnt/j/j1 as separate vars, two s16
  locals ax/az for the frame), B3F8C 12-13 (B3B88 loop: key/e s1/s2 swap; id must be `(x & 0xFFFF)` expressions,
  group /50 via an inline s32 helper), C4EC8 1 (t6 = t7 copy vs li 0; draft uses an s16 * 4th param), C31F8 19
  (tail stores scheduled into the load delays; t7/t8 swap), B4A5C 25 (frame 8 bytes short: the hole sits above
  the addressable img/clut slots; setRECT self-assignments explain the dead lhu loads), C970C 25 (f/n regs v1/a1),
  BFEE0 36 (first-loop regs, header stores block the unk6 load in the original), C07D8 ~35, C0038 ~44 (x/clamp
  regs, flag block not cross-jumped), BD130 ~48 (`tp ? 1 : 0` is a branch in the original, store-flag here),
  C65CC ~104.
  Two matched in this pass by brute-forcing statement order / variable splits with small scripts
  (build/multiperm.py, merge.py, partvar.py, roles.py, declperm.py, variants.py): store order (0c BA1E4) and the
  tpage/clut/semitrans order (0c BF544) fixed register allocation that the permuter never found.
- Large/GTE pass (drafts in the main checkout's build/drafts/ovl_0b/, diff = differing insns): D2938 7 (base
  pointer temp regs), C7730 ~18 (relocation prologue regs/delay slot), AA504 23 (collect loop regs a0/v1),
  D1D74 ~30 (frame index n/k regs), D2DBC ~65 (header load scheduling), CD210 ~36 (bone index needs sext,
  q copied to a 2nd reg), E6764 9 (first inner-loop stores scheduled), D1FFC ~60 (like D1D74),
  E57B0 ~117 (constant 1 kept in s0 and used by sllv), AF800 ~135 (case 0 branch order, 56F not reloaded),
  ABBC4 ~220 (regs in case 5 texture loop), B1B48 ~215 (return-0 block placement), DE11C ~690 (model
  renderer: all 8 primitive loops written, register allocation/spills differ).
- Large-function pass 3 (drafts in the agent worktree's build/nm_<addr>.c): 80054294 159/238
  (needs a pointer to ys set inside the 3rd loop; py/x/y s-register priority), 800475D8 (dir/collision
  block: the o=NULL init must stay live while &dist is CSE'd into s0; run-flag `!run` vs xori),
  8004E248 (choice loops count down to -1; p accessed through s1 only in some reads), 800589F0
  (unk8 reloaded between `m &= ~unk8` and `unk10 &= ~unk8`; s-register order), 80025ED4 (x/y s1/s2
  swap, `x + (w + 2)` reassociated), 8005E3FC (~20 regs/scheduling in the note-on path),
  8002920C (%hi(D_800770AC) not hoisted into fp; font stays in a register).

- Large-function pass 3 (drafts in the agent worktree's build/nm_0a_*.c / nm_0c_*.c): 0a AB3CC (prologue order of
  two s-reg inits only), 0a B3F8C (s0/s1 swap between loop index and temp), 0a AA4E8 (lh/lhu load order), 0a BF0C4
  (parallax loop cross-jump + regs), 0c B0378 (~90 diff rows: else-branch temps, sqrt block hi-part reg), 0c AD738
  (~78 rows: local regs a0/a3, prologue loads of the second block).
  Also 0c B20B8 (2 spots: %hi(D_800CE3C0) kept in s0 across the call vs v1; srlv operand regs in the inlined
  bit test), 0c B9808 (2 spots: fy=0 spill store scheduled early; `&polys[p]` addu operand order).
