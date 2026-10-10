# Near misses and puzzles (candidates for a dedicated hard-case pass)

Functions agents got close on but couldn't finish. Drafts for main are in the main checkout's
build/drafts/main/ (gitignored; v*.c). Append as agents report.

## main
- Tricks that matched 12 main functions (8001D32C, 80036650, 8004D2C0, 8004DCEC, 80035108,
  8004F74C, 800227C8, 80062118, 800621D4, 8005F718, 800616F8, 8002FED4). Search scripts are in
  the agent worktree's build/: hc.py (greedy statement moves), hcm.py (permute independent blocks),
  dw.py/dw2.py (insert one or two `do { } while (0);`), dd.py (aligned diff, `--bin main`).
  - An empty `do { } while (0);` (a disabled debug macro) changes register allocation and the CSE
    of address pseudos. It matched 80035108 and 8002FED4 and got 8005B1B8 and 8002B644 within
    2 rows; try every position.
  - Loop-invariant hoisting depends on loop size: giving a fall-through switch case its own copy
    of the body (8004D2C0) stopped the outer jump-table address from being hoisted into an s-reg.
  - Copies like `move a1,s0` come from re-reading a global the compiler already holds
    (`nn->unk8 = D_8007AECC->unk15D0`, `D->unk1C = x; gd = D->unk1C;`), or from block-scoped locals.
  - `addu v1,v1,v0` vs `addu v1,v0,v1`: assign the result to a separate variable inside the if/else
    (8004F74C: `x = p->x0; if (k) y = x + w; else y = x - w;`).
  - Duplicating a compare in both branches (`if (prev == 0) { if (n != a) err = 1; } else { if (n != prev + 1) err = 1; }`)
    beat any merged form (800227C8).
  - `func_80061BC8` takes one argument. Passing a second kept reorg from filling the beqz delay slot
    (80062118/800621D4).
  - The scheduler reverses pairs of independent stores, so write field stores in the opposite
    order from the asm (8005A320, 8004DCEC).
  - Rodata: a jump table after a string or a second jump table needs
    `__asm__(".section .rodata\n\t.word 0\n\t.section .text");` padding (8004D2C0, after 8004DCEC).
- Drafts in docs/wip/main_*.c (each first line gives the differing rows): 8002B644 (1),
  8005B1B8 (2), 800585E0 (4), 8005AEC8 (8), 80024BD8 (9), 8005A320 (9), 80029D60 (10),
  80048E80 (10), 80047B90 (11), 80059414 (13), 8002ECA0 (13), 80028DB0 (15), 8005C8BC (15),
  8002E718 (23).
  8005A320 and 8005963C share the `~active & (~(D_80083174 | D_80080A70.unk0) & 0xFFFFFF)` problem:
  the original ANDs the mask into the nor first. A static inline returning the masked value gets
  the association right but moves `~active`.
- 80030B34, 80030EE4, 80031284, 80031718: one template (rsin/rcos table D_8006C1C4, then a C or GTE
  matrix path picked by the third argument). They sit next to libgte and are large; no draft beyond
  the old nm_80030EE4 (185/216 rows differ).
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
- Register allocation / scheduling, no draft yet (left as INCLUDE_ASM): C9A28 (2 insns), C9794, BFF20,
  ED3F8, CCDB8, D9D58, D0E14, AB810, ED02C, ED7C0, E1534, BE1E0 (big switch), E4A3C (odd
  argument-slot stack use).
- Large-function pass 2 (drafts in the agent worktree's build/nm_<addr>.c; "~N" = differing insns):
  80047B90 ~35 (needs &v held in s0 with m[1][1] stored via s0+0x10), 80024BD8 ~12 (POLY_G4 store
  scheduling), 8005A320 ~17 (D_80080A70.unkC mask association, relocation scheduling), 8005C8BC ~15
  (r/s1 and %hi(D_80080970)/a3 allocation), 8004D2C0 ~21 (jump-table address hoisted out of loop),
  8005963C ~50, 8005681C ~66 and 80023A6C (store/load scheduling), 800283E0 (sprite pair, inline
  helper shape unknown), 8001DE50 (draft only), 80030EE4 (dead copy of arg2; maybe libgte RotMatrix*).
  Also 8005CF04 ~75 (v/old vs %hi(D_80080A10) s-register priority), 8004DCEC (window-init inline
  boundary; caller stores get rescheduled), 8005D6BC ~76 (&D_80083158 hoisted to fp, tick count spilled).
- Pass 5 (matched: AD90C, E4C88, D650C, E2000, D0B18, E6764).
  Tricks that fixed the matched ones: reuse a variable or add a fresh one to change live ranges (AD90C k2,
  E4C88), msg/ok assignments placed where reorg wants them, duplicated case bodies instead of fallthrough to
  raise a pointer's priority (E2000), r * arg10 instead of a running sum and biv increment order via the for
  clause (D0B18), and a struct-typed store ((XY32 *)&v[0])->w to create the original's alias dependency (E6764).
- Pass 6 (matched: D2938, AF800, DA4C4, C1594, E3B70, F28F0, D07B4, EE154, EE24C, EE7F8, EE938, E651C, DCCE4,
  F5064).
  What fixed them:
  - Variable reuse again, mostly: k reused for arg4*4 (D2938); the size parameter reused as the loop counter
    and the loop pointer as the wrap temp (E3B70); the counter reused as the running start (E651C); a
    2-step `off = i * 4; off = (off + i) * 4` per branch so the join can reuse it (DCCE4).
  - Statement order the scheduler can't undo: `unk56F |= 0x40` before `unk56E = 1` (AF800: the scheduler
    swaps the stores back, but CSE then no longer forwards unk56F to the later |= 8); `r = 1` after the call
    (EE7F8); `(n = p->unk1) - f(p->unk0)` so the load follows the call (EE7F8/EE938).
  - Shared done = 0 via one combined condition instead of nested ifs, so only the tail cross-jumps (AF800).
  - A goto loop (no loop notes, so no rotation or strength reduction) plus one byte pointer for both walks (E651C);
    a constant held in a variable set first (C1594) and an explicit pointer for the loop (C1594).
  - Return the old pointer: DA4C4 is a bump allocator (`p = unk58; unk5C -= n; unk58 = p + n; return p`).
  - addPrimLenIn (ovl_0b.h): addPrim asm with len as a plain input, so a constant len gets its own register
    (D07B4). func_800D08C4 calls D07B4 with 2 args, so its call goes through an unprototyped cast.
  - F5064: the else-branch rect goes through a temp (`t = &D_800FDE58; t->h = ...; r = t;`) to get the
    v0 -> s0 copy, and if/else instead of assign-then-override.
  - EE154/EE24C needed nothing special (if/else-if on D_80104FC0 fields); F28F0 wants a RECT * to w->unk8 and
    `else if (st == 1)` nested rather than one || chain.
  WIP drafts in docs/wip/ovl_0b_<addr>.c (first line = status), rows = aligned differing insns:
  DA92C 2 (case 0 header loads: hdr[1] is always scheduled second; cases 4/5/9 fixed by reusing dst/base/m),
  E9000 2 (i++ placement in the copy loop), B1B48 3 (which return-0 block the later returns cross-jump to;
  turning the goto-fail sites into direct call+return doesn't change it), C3F78 3 (-O1: case-5 test sense,
  table base before index scale), C0C00 5, AE4FC 5, DA138 6, DA1F4 14, AA504 18 (main checkout
  build/drafts/ovl_0b; the collect loop is not the AA24C inline), E8B28 17, EFD1C 18, D7094 30, C9374 32, AB07C 51, CBA58 60,
  AC37C ~150, C67BC ~300.

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
- Large/GTE pass (drafts in the main checkout's build/drafts/ovl_0b/, diff = differing insns): C7730 ~18 (relocation prologue regs/delay slot), AA504 23 (collect loop regs a0/v1),
  D1D74 ~30 (frame index n/k regs), D2DBC ~65 (header load scheduling), CD210 ~36 (bone index needs sext,
  q copied to a 2nd reg), D1FFC ~60 (like D1D74),
  E57B0 ~117 (constant 1 kept in s0 and used by sllv),
  ABBC4 ~220 (regs in case 5 texture loop; frame 8 bytes too big), DE11C ~690 (model
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

## menus / minigames
- Drafts in the agent worktree's build/nm_<ovl>_<addr>.c. 0d B1110 (prologue: stack-arg load of `mode`
  scheduled first), 0d B3198 (y temp in a3 instead of v1), 0d B030C (0x40 constant not hoisted into fp),
  0d B16A4 (s-register priorities), 0d AC060 (s-register priorities: card ptr should be s0), 0d ACFFC
  (constant 0x10 not hoisted into fp; y/dx/dy one reg off), 0d A93A0 (needs two un-CSE'd 0x80104A00
  constants), 36 1A48 (u8 reload of unk3 needs andi), 36 2664 (cur/c s0/s1 swap), 39 973C (CRC: andi
  placement), 38 EE740 (pool address hoisted instead of table base), 33 F2AB4 (a0/a1 swap), 33 F1150
  (lv=0 / address scheduling), 33 EF644 (store scheduling of D_80077DE8), 65 F2560 (constant lui not in
  delay slot), 65 F1848 (loop givs), 15 ADE30 (s-register swaps), 3c F5BCC (`y + 0x26 + row*15` folded),
  14 A741C (search-loop layout), 0d AD400 (FT4 store scheduling), 36 F12B8 (dx=0x2E init hoisted above
  calls), 36 F1778 (one extra s-reg), 3c F6D34 (like F5BCC), 34 EFC20 / 39 F7FD4 (same gauge function
  in both overlays; prim-packet pointer regs), 65 F2E58 (TextSize fields not reloaded), 65 F2FE8
  (loop givs), 3a F459C (folding of row/column sums), 39 F9120 (title digit stores order).
- Not attempted: 36 F08F4 (rodata blob lives in the function file), 64 libpress-internal functions
  (8019AE44-8019B3D4 hand-written DecDCTvlc, 8019D020-8019D36C MDEC_* internals) which look like
  library code that config/library_funcs.us.txt does not list yet.
- 38 EF258 matches (build/ready_38_EF258.c) but links against D_8006D128, which ovl_38's
  undefined_syms_auto lacks; add `D_8006D128 = 0x8006D128;` there (config change) to land it.
