# Near misses and puzzles (candidates for a dedicated hard-case pass)

Functions agents got close on but couldn't finish. Drafts for main are in the main checkout's
build/drafts/main/ (gitignored; v*.c). Append as agents report.

## main
- Pass 8 matched 9 (80054650, 8005DB40, 8005E3FC, 80058118, 80057D80, 80054294, 80024BD8,
  80048E80, 80062E3C). What worked:
  - Variable reuse is the strongest lever. 80057D80: `pan` reused as the pitch scale factor k (the
    permuter found it; mflo a2 everywhere then follows). 80058118: pan reused for the LFO depth
    temp in the first block (`pan = (s16)...; x = pan; if (pan < 0) x = pan >> 1;`). 8005E3FC: one
    `op` for opcode/key/note/flags, one `vel` for the 0xFE index, velocity and both pitches.
    80054294: the second grid loop uses ex/ey as counters (not i/j), plus a per-row `q = py`.
    build/reuse.py (this worktree) generates merges and live-range renames of every variable pair;
    many outputs change semantics, so check each hit by hand.
  - Statement-order sweeps over small blocks (build/cv.py: combinations of text substitutions;
    gen*.py in build/a): 8005E3FC's `c->unk10A = ...` before `note = ...`, 80058118's
    `pan = ... + 0x80; pan &= 0xFF;` in an if/else with `pan = 0x80` in the else.
  - Pointer vs array access: 80054650 writes the prims through `u32 *p` (p[0]..p[9], setaddr(p - 10,
    p)) so the stores alias D_8007BD14 and it is reloaded in the inner loop; the vertex pointer
    `v++` per prim; tp computed before u0/u1; `p = D_8007BD10; ... p += 10;`; the trailer via
    setaddr/getaddr. 80054294's loop 3 needs `ys[i]` (not py[i]) with `py = ys` still assigned
    inside the loop (it feeds loop 4).
  - 8005DB40: two jump tables. The C emits both tables in its own .rodata, so the
    INCLUDE_RODATA of D_80012198 (second table) was removed from main.c. A future resplit may put
    it back; delete it again if so. `if (*p++ <= D_80080A10->unk6C)` (not `>=` with swapped
    operands) stops loop.c hoisting the D_80080A10 load; `c->unkCC = (u16)c->unkCC & ~5` gives
    andi 0xFFFA instead of a hoisted -6; `n--; n &= 3;`; `op = p[0] + (p[1] << 8); p += (s16)op;`.
  - `while (--i != -1)` gives gcc's precheck + compare-to--1 count-down loop (8004E248 draft).
  - 80024BD8: `p->rgbc0` first, then `p->xy2 = 0x800000; p->xy3 = w | 0x800000;` and
    `->wh = w | 0x800000` (no `w |=`), found by moving single statements (build/mvall.py moves
    every single-line statement within its straight-line run; it also took 8005681C 51 -> 14).
  - 80048E80: `i = 3; D_8007AE10[i] = func_800BEC64(1); m = (u16)D_8007AE10[i];` (the loop
    counter as the index) keeps the `la` base for `sh v0,6(v1)`; no inline needed.
  - 80062E3C: `q->unk4_0 = 0` first among the slot header stores (mvall), then the permuter split
    getClut into `clut = cy << 6; clut |= (cx >> 4) & 0x3F;`. cx is reused for the tpage value.
  - build/splitx.py splits `x = a OP b` into `x = a; x OP= b;` at every top-level operator; no hits
    on the remaining drafts.
  - Drafts updated: 8004E248 (38, ~11 of them symbol-only), 8001DE50 (19), 8005681C (14, new),
    8005A320 (5), 80059414 (9). Headers have the details.
- Pass 7 matched 4 (8004B85C, 800378C4, 80035214, 80038618). What worked:
  - Permuter first, then a dup sweep on its best output: 800378C4 (the permuter found the slot
    parameter reused as the key, `slot = func_8004B85C_key(slot)`), 80038618 (permuter:
    `s = q->unkF; if (best) f = s; else f = q->unkF;`, then dup: identical-branch `if (a)` around
    `rr = p->unk3C * 4`).
  - 8004B85C: `lt = kind < 14;` then an identical-branch `if (kind) ge = lt ^ 1; else ge = lt ^ 1;`
    and the `slot >= 8` test written first. The search is now `func_8004B85C_find`/`_key` in ff9.h.
  - 80035214: one temp `v` for the loaded coordinate in both compare blocks (`v = m->unk2; t = c->y
    + 16; d2 = v - t; ...`), and in the second block `x = D_80077DE8.unk0 - 19` (reloaded after the
    `|= 1` store) but `x = v + 18`. `&D_80077DE8.unk10` links to the original's D_80077DF8
    reference, so verify passes; cmp.py still flags those two rows.
  - Draft updates: 8002995C 5 -> 4 (identical-branch `if (x)` around `s++`), 8005A320 9 -> 6 with
    the symbol-aware cmp (`~(D_80080A70.unk0 | D_80083174)` order and a do-while), 80059414 12 -> 11
    (loop counter v reused for `D_80080A10->unk8`), 8004DAD0 30 -> 28 (an if/else-if chain with one
    shared `return 1` keeps the first SE call's own jalr). Headers have the details.
  - No progress: 80024BD8 (dup/wrap sweeps, rgbc0/xy3 orders), 8003595C (dup sweep makes it worse,
    720 orders of the six setup statements give 12 at best), 8002E718 (unk5 local, operand orders,
    flag variable; the permuter only reaches 7 with `new_var2 = 6`), 80047B90 (moving `pv = &v` does
    nothing: pv is rematerialised as a frame address, and the original keeps `&v` in s0 while
    `&mat` is rematerialised per call), 80048E80.
  - 80062E3C: reusing the u/v params for u0/v0 matches the original's a3/t1. The original also
    copies arg0 to t2 first (`prim = arg0`, then `s = prim`, `prim += n * 40`), but that form
    scores worse (134). The header's word 1 still gets a separate s+4 giv.
- Pass 6 matched 10 (8002ECA0, 80029D60, 80029B4C, 8001C8B0, 80039444, 80031284, 800585E0,
  8005CF04, 8004F9A0, 80038278). What worked:
  - The identical-branch if is a general register-allocation lever, not just an rsin quirk:
    `if (var) { stmt } else { stmt }` (jump2 merges the arms) adds a reference to var and splits the
    block, which changes global-alloc priorities and makes block-local pseudos global. 80031284
    (`if (mode)` around `sy = -sy`), 800585E0 (around the func_80057898 call; with bit = 1 written
    before i = 0), 8005CF04 (`if (v)` around `mask ^= k`), 8004F9A0 (q, the second prim, has to be
    a global pseudo to land in a2). build/dup.py in this worktree generates every
    statement x variable variant (`AUTO` picks params and locals); a sweep is ~100-300 compiles.
  - Reusing one variable for two jobs raises its priority the same way: 80029D60 (`hi =
    getaddr(ot)` in the final addPrim), 8005CF04 (one counter for the 32-voice loop and the bit
    loop), 80038278 (one pointer for the slot and the search, found by the permuter).
  - Separate calls per branch, cross-jumped at the jal: 8002995C's three func_8002B7B0 calls,
    8005CF04's second func_80057640 call (one per branch, not shared l/r variables).
  - Tail recursion: `return f(...)` of the function itself becomes a `j`; the original keeps the
    jal, so write `r = f(...); return r;` (8001C8B0). Its switch also needed the cases in layout
    order (9 before 8) and an empty do-while in case 4 so loop.c hoists the jump-table shift.
  - fold reassociates `a - (b + 16)` and `(a - 10) - b`; temps (`t = c->y + 16; d = a - t;`) or an
    (s16) cast keep the original operand order (80035214).
  - `tbl = D_800686D0;` right before `tbl[46].clut` gives `lhu 0x172(base)` with the base kept for
    the later `&D_800686D0[c - 8]` (80029B4C).
  - build/cmp.py in the main checkout masks relocations, so a swapped symbol (e.g. D_80083174 vs
    D_80080A70 in 8005A320) still scores as a match; the worktree copy compares the symbol names too.
- Pass 5 matched 9 (8002B644, 8005B1B8, 8005AEC8, 8005963C, 80028DB0, 8005C8BC, 80030EE4,
  80030B34, 80031718). What worked:
  - Cross-jumping: the original sometimes has one call per branch whose identical tails jump2
    merged (8005B1B8/8005AEC8: `func_8005A284(s, D_8007BD48)` in each branch, not a shared call
    with a pointer variable). Conversely, an `if (mode) { X } else { X }` with identical bodies
    leaves only a dead param copy (`move t5,a2`) and an extra saved s-reg: that is the signature
    of the rsin matrix functions 80030B34/80030EE4.
  - Statement-order sweeps of declaration initialisers (move them into statements and try all
    orders): 80028DB0 (dst before w), 8005963C (bit, i, c, ofs), 8005C8BC (blk = &D_80080970 after
    the first call, so its hi part no longer crosses a call and gets a3).
  - Reading a value back through the struct instead of a u8 temp (`v->unk0 = q; ... m->unk3 -
    v->unk0`): CSE turns the reload into a fresh `andi v0,a0,0xFF` (8002ECA0 first branch).
  - Constant in a temp (`x = 0xFFFFFF;` then `... & x`) fixes `~a & (~(b|c) & 0xFFFFFF)`
    association (8005963C, 8005A320, 80059414's early lui).
  - Per-angle temps (tx/ty/tz instead of one t) and separate `*p = ...` stores in each branch
    (80028DB0) change allocation in the right direction.
- rsin/GTE template (80030B34, 80030EE4, 80031284, 80031718): `ISIN(out, a, mk)` macro and
  `D_8006C1C4` in ff9.h; `gte_ldir0123`, `gte_ldir12`, `gte_stmac123`, `gte_stmac12`,
  `gte_gpf12_b/_n1` in psyq.h. Sin for x uses the raw `r->vx` (`ISIN(sx, r->vx, x)`), y/z use the
  masked value. 80031718 needed a `do { } while (0)` around the second gpf block, 80031284 an
  identical-branch `if (mode)` around the y quadrant negation.
- Drafts in docs/wip/main_*.c (first line gives the status; updated in pass 7): 8002995C (4),
  8005A320 (6), 80024BD8 (9), 80048E80 (10), 80047B90 (11), 80059414 (11), 8003595C (12),
  8002E718 (16), 8004DAD0 (28), 80062E3C and 80054650 (first drafts, ~100 rows). 8004B85C,
  80038618 and 80035214 are matched.
  - 8004B85C: the same kind < 14 search is inlined in 800378C4, so fixing the lt/ge register pair
    fixes both. Tried ~25 formulations (bool temps, ^ 1, ?:, ==/!= of two compares, slot >= 8
    branch order) and dup/wrap sweeps; best is 3-4 rows.
  - 80047B90: the original addresses `mat.m[1][1]` as `16(s0)` with s0 = &v. Writing the store as
    `*(s16 *)((u8 *)pv + 16) = 0x1000` after `pv = &v` reproduces that, but we then keep &mat in
    an extra s-reg (12 rows); the original rematerialises `addiu a0,sp,40` per call.
  - 80048E80: the `sh v0,6(v1)` through a la base after the loop-invariant setup looks like the
    store was emitted after loop.c's pre-header insns; no C form found.
  - 8004DAD0: needs the first SE call (unk26 == 0xFF path) not to be cross-jumped with the three
    list-cursor SE calls; an inline wrapper did not stop it.
- Tricks from earlier passes (matched 8001D32C, 80036650, 8004D2C0, 8004DCEC, 80035108,
  8004F74C, 800227C8, 80062118, 800621D4, 8005F718, 800616F8, 8002FED4):
  - An empty `do { } while (0);` (a disabled debug macro) changes register allocation and the CSE
    of address pseudos. Try every position (build/search/main/dw.py, dw2.py).
  - Loop-invariant hoisting depends on loop size: giving a fall-through switch case its own copy
    of the body (8004D2C0) stopped the outer jump-table address from being hoisted into an s-reg.
  - Copies like `move a1,s0` come from re-reading a global the compiler already holds
    (`nn->unk8 = D_8007AECC->unk15D0`, `D->unk1C = x; gd = D->unk1C;`), or from block-scoped locals.
  - `addu v1,v1,v0` vs `addu v1,v0,v1`: assign the result to a separate variable inside the if/else
    (8004F74C: `x = p->x0; if (k) y = x + w; else y = x - w;`).
  - Duplicating a compare in both branches beat any merged form (800227C8).
  - `func_80061BC8` takes one argument. Passing a second kept reorg from filling the beqz delay slot
    (80062118/800621D4).
  - The scheduler reverses pairs of independent stores, so write field stores in the opposite
    order from the asm (8005A320, 8004DCEC).
  - Rodata: a jump table after a string or a second jump table needs
    `__asm__(".section .rodata\n\t.word 0\n\t.section .text");` padding (8004D2C0, after 8004DCEC).
- gcc 2.8.1 cross-function leak (explains the old "order dependency" of 800584C8): combine.c
  only resets its static `mem_last_set` *after* the initial nonzero-bits scan, so during that scan
  the cuid of the previous function's last store/call is still there. A `(reg) = (mem)` whose insn
  cuid is <= that stale value is treated as clobbered, `nonzero_bits` gives the full mask, and a
  later `zero_extend` of that u16/u8 register is not removed (extra `andi`). Any predecessor
  that ends with a store leaks; an empty function in between resets it. Fix in C: write such
  functions so the extension is not needed (800584C8 hoists `m = (u16)e->unk4`, keeps `s32 min`).
  80057D80 and 80058118 still open: product temps are `mflo a2` in the original (global.c prefers
  the register with the fewest local refs) but `a1` for us.
- Old large-function drafts (main checkout build/drafts/main/nm_*.c, rows differing): 8005E3FC (67),
  8001DE50 (89), 800283E0 (97), 8004E248 (98), 800589F0 (122), 80023A6C (156), 80025ED4 (178),
  8002920C (231). (8005CF04 matched in pass 6: its draft had an uninitialised `old`.) Notes from
  earlier passes, still open: 800378C4 (f<14 branch shape, same search as 8004B85C), 8005DB40
  (two jump tables, AKAO look-ahead), 80054650 and 80062E3C (first drafts in docs/wip).

## ovl_0b (battle)
- Register allocation / scheduling, no draft yet (left as INCLUDE_ASM): C9A28 (2 insns), C9794,
  ED3F8, D0E14, AB810, ED02C, ED7C0, E1534, E4A3C (odd
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
- Pass 7 (matched: DA92C, E9000, E8B28, AE4FC, D7094, C9A28, ED7C0, ED3F8). What fixed them:
  - sched1 "birthing" priority: an insn that sets a pseudo with a single set (REG_N_SETS == 1) and no deaths
    gets max priority, so splitting a reused variable into a fresh one reorders loads (DA92C: fresh cnt2 for
    hdr[2]); moving constant-holding locals (ED3F8 tp/stp) or the xy/wh computations changes the order too.
  - ARRAY_REF vs pointer arithmetic: `base[c ? 1 : 0]` through a pointer gets folded into `c ? base+20 : base`;
    indexing an array member of a struct (BtlUnit9C50Arr) keeps the 0/0x14 value materialised (E8B28).
  - Per-branch copies that cross-jump: `u = t = &c[k]` in both branches leaves a `move t0,t2` at the join (ED7C0).
  - Two nested empty `do { } while (0)` around `return -1` (permuter find) fixed the table-base register (AE4FC).
  - Loop pointer + separate n = 0 after it (D7094); `for (...; i++, s++) *dst = *s` (E9000);
    in-place `m = x & ~0x300; m |= ...; u->unk68 = m` (C9A28); `if (f()) v = K; else v = 0` so v isn't live
    across the call (C9A28); a reloaded pointer read after the first store (D9D58 draft).
  - Loop giv base follows the last e->field referenced in the loop (E1534 draft).
  - jump.c cross-jumping (B1B48 notes): the processed jump is replaced; a conditional jump around the block counts
    as a match, so a lone `v0 = 0; j epi` merges into the first chain candidate.
  New drafts with headers: AB810 8, C9794 26, D0E14 3, D9D58 5, E1534 4, ED02C 4, AA504 3, C9374 2, EFD1C 16.
- Pass 8 (matched: C9374, D0E14, ED02C). What fixed them:
  - One variable for two roles: C9374's f holds the relocated rect pointer and then each frame-table pointer
    (r = (TexAnimRect *)f is the loop base), which puts the frame-table load straight into f's register.
  - Load early into a temp, store late: D0E14 needed `inv = arg1[1]` read right after c.unk28 and `c.unk2C = inv`
    as the last store (found by brute-forcing 600 orders with build/mv.py-style specs).
  - Build a value in place: ED02C's uvclut is `w = unk0; w |= unk1 << 8; p->uvclut = w | clut`.
  - An unmodified stack-passed parameter is rematerialised from its slot where it is used; `x %= n` on the parameter
    itself makes it load at entry (D1D74: copy to a local first).
  - Locals whose address reaches GTE asm live in memory; in D2DBC the colour plus the mode/ctab/clut values the loop
    reloads from sp+0x18..0x20 are one struct.
  - Parameters kept in their stack homes (E4A3C) come from taking their addresses; an inline that copies through the
    pointers gets everything but the entry `sw a3` and a dead load of the 5th slot.
  New drafts: E4A3C 8, D1D74 22, D2DBC 63. Updated headers: AA504, C0C00, C3F78, D9D58, DA138, E1534.
- Pass 9 (matched: CCDB8, BE1E0, C7730, D9D58). What fixed them:
  - A separate local per switch case for the same `arg2 * 20` offset (BE1E0 cases 0x3D/0x3E/0x3F): one shared
    variable is global-allocated and splits its multiply temps off; per-case locals stay local and keep the whole
    chain in one register with the (base + off) operand order. Its jump table needs the 8-byte padding word
    before and after (`__asm__(".section .rodata\n\t.word 0...")`).
  - shorten_binary_op: `(u16)a % (u16 & 0x7FFF)` becomes divu; put the dividend in an s32 local first (CCDB8).
  - Types decide copies: `s16 y, z` holding `(ty[k] << 24) >> 20` gives the original's y0 -> y copies (CCDB8); the
    row index used inline (no k variable) and `(rot.pad = e->unkF) != 0`; `for (e = tbl, i = 11; ...; e++, i--)`.
  - C7730 (-O2 despite sitting in the old -O1 range): `arg0->unk0 |= 2` must come before the other stores
    (else flow deletes it as dead before `unk0 = 0`), relocation stores in field order, the scratch pointer
    `mark = (u8 *)D_80070520` set inside the outer loop, and k reused for the clear loop.
  - D9D58: `*(volatile s16 *)&u->unk86[2] = a->unk24` keeps that store ahead of the D_800FEC56 store.
  - One variable for two roles again (CD210 draft): the bone index variable reused as the level keeps the
    (s16) extension on the index; in BFF20 the D7C counter n doubles as the float flag.
  New drafts: BFF20 7, CD210 25, D1FFC 27, E57B0 34. AB07C 51 -> 24 (separate loop pointers pa/pb/pr set from
  b, r, r + n; getTPage/getClut; permuter: 14 with a do/while(0) around the tail).
- Pass 10 (matched: EFD1C, E57B0). What fixed them:
  - A separate variable for each list walk (EFD1C): the two `for (u = ...6F8; ...)` loops in the original use
    different pseudos (a1, then a0); one shared u forces one register for both.
  - Assignment inside the test: `if (arg0 == (k = cur->unk68 & 1)) return 0; side = !k;` with s32 k (EFD1C).
    k as u8/s16 narrows the load to lbu/lhu; `side = !k` before the test keeps k live and moves the temp.
  - Store order inside switch cases (E57B0, found by build/sweep.py's single-statement moves): `unk8` before
    `unkC = a`, `unkA` before `unk8 = a`, `unkE` before `unk4 = b`, the unk1C.vz word copy before `vy -= 0xC0`,
    `unk14 = 0` after `unk2`. 38 -> 0 rows.
  - cmp.py masks branch offsets: E57B0 scored 0 rows but failed verify because the original stores
    `e->unk18->unk8 = *p & 0x7FFF` outside the `if (e != NULL)` (its beqz jumps into the middle of the block).
  - flow.c weights REG_N_REFS by loop depth, so a do { } while (0) around code (it emits loop notes) doubles every
    ref inside it; that is why the AB07C permuter wrap helped. The goto loop itself has no loop notes.
  - A by-value struct parameter (E4A3C): a 12-byte struct in a2/a3/stack gives the stack homes without the entry
    `sw a3` (flow deletes the dead store); read fields as scalars (`*(s32 *)&arg2.b`) so the load can be scheduled
    above struct stores. 8 -> 7 rows (one copy missing). Callers pass two pointers, so integrating needs the prototype
    changed or an asm-label declaration.
  - build/sweep.py: single-edit sweeps (declaration moves, statement moves, do/while(0) wraps, identical-arm if/else
    on every variable) scored in parallel inside Docker; it doesn't check semantics, so read each hit.
  Status now: AA504 3, B1B48 3, C3F78 3 (-O1), E1534 4, C0C00 5, DA138 6, BFF20 6 (+2 symbol-name rows), E4A3C 7,
  AB810 8, AB07C 24 (10 with the do/while(0) wrap plus `yy = y;`, 1 with an extra identical-arm if; both hacks),
  C9794 14 (was 26), DA1F4 14 (now same length), D1D74 22, CD210 25, D1FFC 27, CBA58 45, D2DBC 63, AC37C ~175,
  C67BC ~300. DE11C and ABBC4 have no drafts.
- Pass 11 (big functions; matched ABBC4, 0x680). What fixed it:
  - Index form inside the texture loop (`a[j]`, `b[j]`, `r[j]` instead of pointer increments): loop.c then makes
    the reduced givs with `move s3,a0`-style copies from the bases, and `r[j].unk2` stops getting its own +2 giv
    (that giv was the extra spill that made the frame 8 bytes too big).
  - The search pointer is reused for the hit: `e = t->unk4; for (...) if (e[i].unk0 == id) { e = &e[i]; ...}`.
    Since e is set inside the loop it is not invariant, so only i*12 is reduced (`addu a2,a1,zero` init) and the
    address is rebuilt each iteration (`addu a0,a2,t0`).
  - Invariant hoisting depends on movable order: `arg0 * 3` is hoisted only if it comes before x/c in the inner
    loop; spill slots follow pseudo numbers, so c must be an expression hoisted after k3, and
    `(s16)(((x & 0x3FF) >> 6) | 0x80)` keeps fold from pulling the `| 0x80` back into the loop.
  - `s = r[j].unk2 + 0xE5; y = arg0 * 3 + s;` gives `addu s0,t2,s0` (a separate target keeps expand_binop from
    swapping the operands); a separate variable per switch case for the file id and the anim pointer (one shared
    variable crossed calls and took an s-register); read f->unk4/unkC and D_8006794C->unk20 into locals before the
    request stores.
  - Colour copies in the model renderers (DE11C, C67BC): the original copies each colour through one reused
    pointer (`cp = (u32 *)col[i]; p->rN = *cp;`). A multi-set pseudo is not "birthing", so sched1 pipelines the
    address/load/store chain exactly like the original; setlen/setcode go after the last colour copy (the prim giv
    base follows the last field referenced, +7 = code).
  - DE11C 3-vertex loops index the Z table with `i2 & 0xFFFF` (CSE keeps two `sll`, combine then drops the and);
    C67BC's GT3/G3 recompute `andi`+`sll` for i0 and do `andi a0,a0` in place: both point at u16 index variables
    (`u16 i0..i3` gives C67BC exactly 989 insns but needs the index statements reworked).
  Status: DE11C 430 rows (18 with registers masked, global allocation right; WIP docs/wip/ovl_0b_800DE11C.c),
  C67BC 234 (was ~300; 16 masked), AC37C 175 (notes on the tu/tv registers and the D_8006794C lo_sum CSE in the WIP
  header), CBA58 45 (notes added). D2DBC not attempted.
- Pass 12: matched C67BC (0xF74) and CBA58 (0x158). What fixed them:
  - C67BC: the index words are loaded into the index variables and masked in place
    (`i0 = W0; i2 = W1; i1 = i0 >> 16; i3 = i2 >> 16; i0 &= 0xFFFF; i2 &= 0xFFFF;`), and the 3-vertex loops use the
    same form for their third index (`i2 = *(u32 *)&s->unk0[2]; ... i2 &= 0xFFFF;`, which combine turns back into a
    lhu). The GT3/G3 Z loads use `D_80070520[(u16)i0]`: i0 has full-word sets, so after the cull label the and is kept
    and CSE does not reuse i0*4. Last step was i1 (t1) vs i2 (t0): global priorities 3.387 vs 3.360, and the
    word-plus-mask form gives i2 four more refs. Before that, a refs/length check showed that single-line moves never
    change these priorities without also changing the code: sched1 normalizes the order, so look for a different RTL
    (types, forms), not a different statement order.
  - CBA58: `u16 b` (the second operand word) instead of s32 changes sched1's order of the header loads. `u16 ang`
    gives the a2 -> a1 copy. `s16 x, z` for the differences with `if (x == 0 && z == 0) ang = unk86; else
    ang = ratan2(d[0], d[1]);`, so the test uses the registers and the ratan2 arguments are reloaded with lh. `s16 w`
    for the unk86[1] copy (lhu). The tail is `ang = (ang + b) & 0xFFF;`.
  - Tools for this (in the agent worktree's build/): vp.py/vtp.py (variant specs with refs/len/priority of chosen
    pseudos, parallel), psweep.py (single-line moves with the priorities of two pseudos), prefs.py (greg conflicts
    and preferences: which hard regs other allocnos prefer).
  Status: DE11C 79 rows (was 430; 8 masked). The first four loops match apart from the FT3/GT3 tail sra. The draft still
  has two invented variables (zg = otz of the G/F loops, i2 = the second colour word of GT4/G4); see the WIP header.
  D2DBC 63 (pure allocation; the frame/cnt register t2 also holds `(b & 0xF) * 2`), AC37C unchanged (175).
- Pass 13: matched DE11C (0x1040), D2DBC (0x274), D1D74 (0x288), D1FFC (0x3DC), C9794 (0x294), C0C00 (0x7C).
  What fixed them:
  - DE11C: one `otz` for all eight loops, with `addPrim(&ot[otz >> 4], p)` in the 3-vertex loops (no in-place
    `otz >>= 4`). That drops the shared pseudo from 120 refs (1.59, took t2) to 104 refs / 450 insns = 1.39, and
    the first-half conflicts (otz vs b in t1) keep it off t1 in the second half, so it lands in t3 everywhere (a split
    zg had no conflict with b and 1.97, so it took a2, or t1 when its priority was lowered). The second colour/uv word of GT4/G4 goes into i1 (dead by then; the original's a0), and `col[a & 0xFFFF]`
    works as well as the byte-offset form. Found with build/gsim.py (below), not by search.
  - build/gsim.py (agent worktree agent-aa3230c8e7e5ba3f3): replays global-alloc from a `-dg -dl` dump (conflicts, hard conflicts, printed preferences,
    regs_used_so_far for pass 0) with overrides: `REG=pri`, `conf:A,B` (extra conflict), `pref:A,HARD` (extra full
    preference), `merge:A,B` (one allocno). gsweep2.sh sweeps one pseudo's priority. It reproduces the real dispositions
    of DE11C exactly (apart from HI/LO and spilled pseudos), so it answers "which priority/conflict would give the
    original registers" before writing any C. For DE11C, `merge:85,86` at priority 1.0-1.2 gave the original assignment.
  - One variable for several roles, again: D2DBC's n (count, n*2+8, frame offset, n++) and f (frame, %, count,
    `(b & 0xF) * 2`); D1D74/D1FFC's i (frame, then the palette count `i = *p++`); C9794's i for all three loops.
  - D2DBC: mode/ctab/clut are plain locals that lose global allocation (refs <= 4 * calls, so no caller-save) and
    are reloaded through reload's t4: `clut = h->unk4 << 16` as one statement (5 refs made it caller-saved in t2).
    `mode[1] = tpage | 0xE1000000 | ((tp & 3) << 5)` gives the original OR association; `num = arg0[n++]; arg0 += n;`;
    `for (i = 0; ...; i++, prim += 20)` orders the giv increments.
  - sched1 ties (D2DBC, D1D74, D1FFC): when two launched insns tie, the one later in the RTL is placed lower. The frame
    offset must be computed before the palette pointer and added after it: `off = f * 2; q = (u16 *)(arg0 + n);
    n = *(u16 *)((u8 *)h->unk8 + off);`. D1D74/D1FFC also need `pal = arg0 + n; p = (u16 *)pal;` (permuter find; the
    copy changes the order) and D1FFC `mode = h->unk2` before `c.unk14 = h->unk4`.
  - C0C00: `v = ... * 4` in each branch with `u8 k`: with an s32 k, reload_cse turns the k == 2 branch's `sll 2` into
    `sllv a3` (k holds 2) and the branches no longer cross-jump.
  - C9794: `for (i = 0, cmd = src; ...; i++, cmd++)` puts the i increment before cmd's.
  Status of the rest: AA504 3, B1B48 3, C3F78 3 (-O1), E1534 4, DA138 6 (gsim: needs sz > p > i, or sz preferring v1
  and p a0), BFF20 6, AB810 6 (was 8: pass e->unk4 to the first func_8001C7FC; the loop entry test still folds),
  E4A3C 7, DA1F4 14, AB07C 24, CD210 25, AC37C 175. 15-20 min permuter runs on AA504, E1534, DA1F4 found nothing.

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
- Large-function pass 6 (drafts committed as docs/wip/ovl_0a_*.c / ovl_0c_*.c with notes): 0c AD738 1 insn (one
  reorg delay slot after `if (D_800CE0EC == 1)`: original fills it from the target), 0c C53DC 3 insns (dead
  `s7 = 0` + its save, frame 0x38), 0c B0378 ~36 rows (product tied to LO -> mflo t2; else-branch constant regs),
  0a AA4E8 ~93 rows (lh/lhu order of the FieldVramPos loads; s-reg swaps), 0a BB714 = 0c CA1D8 (twins, 772/778:
  3-vertex loops recompute `i2 * 4`; register shifts). Matched 0a BA560 and 0c BEECC.
  What made the difference (worth trying on other register-allocation misses):
  * locals reused for unrelated values (BA560: key2/key/arg1/rot reused as bone/column pointers in the second pass;
    BEECC: one loop index for both loops, best/ang reused as |dx|/|dz|; B0378: dx/dy/dz reused in the else branch);
  * goto loops instead of do/while (no loop.c givs/hoisting, and refs are no longer weighted by loop depth);
  * an address-taken pair as one aggregate (`VECTOR q; f(&q.vx); g(&q.vy)`) explains unused frame space;
  * `x >= 0 ? x : -x` vs `if (x < 0) x = -x` vs ABS() give three different abs sequences.
- Pass 7 (close-out): matched 0c AD738, C53DC, B0378, B9808 (and 0d B1A84, A9B2C, B1110, 15 A7ABC, see below).
  Drafts left in docs/wip/ with headers: 0a AB3CC (4 rows), C4EC8 (1), B4A5C (13), C31F8 (19), BB714 (~260,
  was ~930), AA4E8 (unchanged). What worked:
  * `abs()` (the builtin) emits the abssi2 insn: same `bgez; nop; negu` as `if (v < 0) v = -v`, but it is one
    insn, not a jump, so reorg's register-liveness scan (find_dead_or_set_registers, max ~2 conditional
    jumps) sees further. That fixed AD738's last delay slot (filled from the branch target);
  * a dead `u8 flag = 0` whose only use is `if (flag) {...}` explains a saved s-register set to 0 and never
    read (C53DC): combine folds the test through nonzero_bits, but the pseudo keeps flow's stale life info
    across the loop and gets a callee-saved register; the body is dropped by jump2;
  * a product that ends up as `mflo tN` + reload instead of a plain register: `m = a * b; m >>= 12; x += m;`
    (B0378). Variable reuse found by build/rename.py-style search: B0378's else branch keeps the x offset in
    `ez` and z in a fresh local;
  * `(WmPoly *)((p << 4) + (s32)polys)` for an `addu` with the index register first (B9808);
  * early spill stores move with the statement (B9808 `fy = 0` after the other inits).
  New scripts in the agent worktree's build/: cmp.py-based vt.py (score files), try.py/tryv.py (apply
  replacements), perm.py/perm2.py (greedy permutations of contiguous lines), movel.py (move one line through
  a range), combo.py (2^k on/off sites), rename.py (rename locals inside a range).
- Pass 8 (big skipped functions): matched 0c func_800AB0C0 (0xF60) and 0a func_800C2AF0 (0x708). WIPs:
  docs/wip/ovl_0a_800BF0C4.c (1 row, `addu` operand order), ovl_0a_800BF660.c (39 rows, x/base register swap
  plus fold reassociation of `wx - (lim - 160)`), ovl_0a_800B0124.c (17 rows, first `D_8006794C` load of the
  kind-9 block lands in v0 instead of a1). Not attempted: C519C, BE158, 15 A9488 (Blackjack), BB714.
  What worked:
  * global-alloc priority is floor_log2(refs) * refs / live_length; `cc1 -dl -dg` gives refs/length in .lreg
    and dispositions in .greg (build/prio.py). That shows which statement order changes a pseudo's s-reg;
  * a param that is spilled and reloaded but never used afterwards points to a dead test that the original
    kept (B0124: `if ((arg0 & 0x3F) == 1)` in the kind switch);
  * CSE keeps a copy into a user variable only when the variable lives beyond the extended basic block, so
    `g = D_8006794C->unk1C` must also be used later (B0124 state 2);
  * GNU case ranges (`case 1 ... 3:`) give a decision tree around a small inner switch (fewer than 5 cases);
  * `s16 *arg2` instead of a struct pointer changed alias classes (MEM_IN_STRUCT) and fixed C2AF0's
    scheduling; mixed `pp = &p` / `p.vx` access also mattered;
  * jump tables at 8-byte offsets in the original object need `__asm__(".rdata\n\t.word 0\n\t.text");` pads
    at the start of the switching case, not on a label next to delay slots (AB0C0, B0124);
  * a `return -1` vs `break` choice decides which duplicate tail jump2 cross-jumps (AB0C0).
  * tools/permute.py strips `static inline` bodies (also those from headers), so the permuter scores the
    wrong code for any function using an inline helper; build/fixinl.py in this worktree restores them before
    running decomp-permuter directly. Earlier permuter runs on such functions may have been meaningless.
- Pass 9: matched 0a BF0C4, BFEE0, AB3CC. New drafts in docs/wip/: C519C (140 rows, was not attempted;
  control flow and frame match, register allocation left), BE158 (194 rows, first draft; n must not
  need a callee-saved register). B0124 17 -> 12 rows (`d = g->unk850; g->unk826[0] = d;`). Not
  improved: C4EC8 (1), B4A5C (13), C31F8 (19), BF660 (39), BB714/CA1D8, AA4E8. Not attempted: 15 A9488.
  What worked:
  * sched1 ranks a `birthing` insn (sets a pseudo with REG_N_SETS == 1, live at block end) at
    0x7f000001, so a set-once local is scheduled after multi-set ones regardless of statement order
    (`cc1 -dS` prints the ready lists). AB3CC: `lim = 100` came after `j = 0` until lim was also the
    later part count (s4 holds both in the original), i.e. set twice;
  * a loop counter compared/used through the index (`&o[j]`, `D_800C9DA4.unk1C + j * 3`) instead of
    walking pointers: loop.c's giv inits then appear as the original's copies after the loop guard
    (`addu s2,s3` = k init 0 rewritten by reload_cse, `addu s0,a0` = o init) (BFEE0);
  * a dead `switch` whose results are overwritten later keeps only its loads and the hoisted case
    constants; it must assign a variable that is live afterwards (BFEE0 assigns j, reset to 0 later);
  * `do { } while (0)` around a whole loop (not one statement) raised the loop depth weighting of
    its pseudos and fixed the counter/limit registers (BFEE0, found by the permuter; build/dwblk.py in
    the pass 9 worktree wraps every statement or block in turn and scores them in parallel);
  * an extra copy of a loop bound before the second loop (`cnt = lim; for (j1 = 0; j1 < cnt; ...)`)
    fixed AB3CC's last s4/s5 swap (permuter);
  * reusing a result variable for an operand (`d = q[x]; d = ... * (p[x] - d);`) made the original's
    spills (edge to 0x68, &q[x] to 0x70) appear in C519C (237 -> 147 rows);
  * an `ABS(d) < ae` the original compiles as two compare-and-branch arms (`bltz d`, then
    `slt; beqz cont; j next` and `negu; slt; bnez next`) only came out of explicit gotos
    (`if (d >= 0) { if (d >= ae) goto c; goto next; } if (-d < ae) goto next; c:`); every expression
    form is cross-jumped into one bnez (C519C);
  * `s = (s32)h - -s;` keeps `addu a1, s4, a1` operand order where `(s32)h + s` gets swapped by
    expand_binop (BF0C4; ugly but the only form found);
  * a block-scope prototype (`void f(T *, u8);` inside the function) gives one function the u8
    argument conversion without changing an earlier-matched caller that relies on the implicit
    int declaration (AB3CC / AC02C and func_800B61A4);
  * fake permuter outputs are hints: B3198's `if (y) y = t + 0x12; else y = t + 0x12;` meant the
    assignment belongs in both branches (`y = a->unk10 + 0x12;` / `y = b->unk10 + 0x12;`).
- Pass 10: matched 0a C4EC8, C31F8, B3F8C. Improved drafts (docs/wip/): BF660 4 rows, B4A5C 5,
  C07D8 11, BD130 12, C0038 19 (was 39/13/35/48/44). Unchanged: B0124 (12), BB714/CA1D8, C519C,
  BE158, AA4E8, C970C (26), C65CC (104). What worked:
  * reload_cse only reuses a register holding the same constant in the same or a wider mode.
    C4EC8's `t2 = t7` copy with `li t6, 0` next to it meant r was a narrow type: `u8 r` with
    `r = !r` (a `^= 1` keeps the return andi, `!r` doesn't);
  * C31F8's `addu t2, t6` (i = 0 copied from any) came from `any = 0; i = 0;` at the top of the
    else arm: reload_cse sees both in one block, then reorg moves `any = 0` into the branch's
    delay slot;
  * sched2 runs before jump2's cross-jumping. Statements written at the end of both if/else arms
    are cross-jumped into one tail afterwards but were scheduled as separate blocks, so the call
    setup after the join can't fill their load delays (C31F8's two nops);
  * gcc 2.8's MEM_IN_STRUCT_P heuristic lets a store to a scalar global pass a struct-field store
    through a pointer; `*(s32 *)((u8 *)p + off)` (not in-struct) keeps them in order;
  * refs of a variable that combine later deletes still count for global-alloc priority:
    `key = func_800B3B64(); if (key == 0)` (B3F8C, three calls) or a temp of the same variable
    (`key = x & 0xFFFF; if (key < 9000 && ... grp(key))`, permuter) moved key above e;
  * a dead `p = (u8 *)&pad;` right after the call that takes &clut/&img puts pad's stack slot after
    theirs (B4A5C's 8-byte hole); `(void)&pad` is never expanded and does nothing;
  * clamps `if (v < c->lo) ... else if (c->hi < v) ...` re-reading the limit in each arm (jump2
    cross-jumps the tails, reorg fills both delay slots) instead of `(t = c->lo) ... (t = c->hi)`
    (BF660, C0038);
  * reusing a variable that an earlier statement reads (WAR) pins sched1's order of two
    independent computations (BF660 `x = A - 112; y += x;` after `cx = lim + x`);
  * `GetTPage(tile->tp ? 1 : 0, ...)` came out as a branch only with the call in both arms (BD130).

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
- Large-function pass (0d/14/15), drafts in docs/wip/: 0d B1A84 (~98%, else-branch CSE of the
  0x3A40 clut / ot + 0xFFA), 0d AC708 (t * 42 partly hoisted out of the loop; stack layout and flow
  right), 15 A7ABC (~95%, reload scheduling of the spilled y/c args). Not attempted: 15 A9488
  (6300 B blackjack main loop, heavily cross-jumped).
  Tricks that matched things in this pass: a `u8` brightness local assigned from the unclamped
  expression (`c = t * 8; if (t * 8 >= 0x100) c = 0xFF;`) gives the original's copy-then-compare
  (0d AFCE4); `(arr[buf] + 1)->field` instead of `arr[buf][1].field` keeps `&arr[0][1]` as its own
  hoisted base (14 A9E14, 15 AB490), and tpages there are `setDrawTPage(p, 1, 1, tpage)`, whose
  `((u32 *)p)[1]` store schedules differently from a `->code[0]` store; a global that loads are not
  moved above OT stores is a struct member in the original (14 D_800AB2D0); a nested `if (a) if (b) ...` chain stops gcc folding
  `x != 0x7C && ... != 0x7F` into a range check (0d AA790); `c = cond ? 1 : 2` used as an index folds
  into byte offsets 4/2 (14 A9220); computing a call argument in its own statement first changes
  the order of hoisted constants (15 AD7DC).
- Pass 7 (0d/15 close-out): matched 0d B1A84 (the else branch's third sprite uses the same two-armed
  if/else colour block as the second, which cross-jumps to the original's shape and stops CSE across it),
  15 A7ABC (spilled-arg reloads can't move above earlier stores, so the statements that use y/c go first:
  greedy permutation of each sprite's stores, build/perm2.py), 0d A9B2C (new; a per-arm `if (s->unk4 == 3)`
  that cross-jumps into one compare, `k = s->unk564 * 12 - 0x78; r.x -= k;`, `code` stored right after
  setlen) and 0d B1110 (the draft only lacked the `s` argument of func_800ACFC8).
  Drafts in docs/wip/ with headers: 0d ACFFC (2 rows: preheader order of `y = 0x1A`), B3198 (2 rows, permuter
  find `y = b->unkC; x = y + 8;`), B030C (20 rows: 0x40 kept in fp), AC708 (unchanged; a goto loop loses the
  original's giv). 0d A93A0 (40 B): every spelling of `tbl[i*2] * 0x800 + 0x80104A00` CSEs the two
  0x80104A00 constants. 0d AC060 compiles with `void *vp` (91 rows, a should outrank s for s0).
- Pass 9 (0d/14/15): matched 0d B3198 (y assigned in both branches, see the ovl_0a pass 9 notes) and
  0d A93A0 (`s32 *p = (s32 *)0x80104A00; p += i * 2; return (u8 *)0x80104A00 + *p * 0x800;`: the
  pointer variable is modified, so CSE no longer knows the first constant and rematerialises the
  second). Still open: 0d ACFFC (2 rows; with -dS the preheader block has all-equal priorities, so
  sched keeps source order and `y = 0x1A` always precedes loop.c's hoisted `li fp, 0x10` / giv init;
  in the original it comes after them, so it is probably not a plain preheader assignment), 0d B030C
  (20), 14 A741C (the original enters both search loops with `j cmp` into a `while (e->unk0 != key)
  { e--; if (e < start) goto skip; }` shape; jump.c duplicates the exit test here instead, 59 rows
  with that form vs 49 for the goto draft), 15 ADE30 (28: x/f/s/p s-register order and the
  `x += w` delay slot).
- Pass 10 (0d): matched 0d ACFFC, B030C, AD400; AEE2C matches but is not integrated (its "NO" label
  D_800A7398 is rodata migrated into func_800AEE2C.s; needs `// force_not_migration:True` and a
  resplit, see docs/wip/ovl_0d_800AEE2C.c). Improved drafts: AC060 19 rows (compiles now), B16A4 37.
  What worked:
  * a value set after loop.c's movables is a strength-reduced giv init: ACFFC's `li s5, 0x1A` is
    `c->unk10 = n * 14 + 0x1A; n++;` (no y variable);
  * update_equiv_regs doubles the live length of a pseudo with a REG_EQUIV constant, which lost
    B030C's hoisted 0x40 its fp. The original stores v0 = 0x40 in both arms of the SPRT u0 if/else
    (more refs; jump2 cross-jumps the stores) - the permuter's fake `if (p) x = 0x40; else x = 0x40;`
    pointed at it;
  * `do { } while (0)` around a use raises its pseudo's refs (loop-depth weighting) - also a hint
    that the real fix is a second use;
  * AEE2C: c0 also holds the func_800AF268(s) results passed straight to the next call (combine
    deletes the copies, the refs stay), which lifts it above s and m;
  * AD400: `cards[i].field` in most places instead of `c->` lowers c's priority; the FT4 uv stores
    are u0, u1, v0, v1, u2, v2, u3, v3 (brute force over orders).
- 14 A9220 needs a symbol at 0x800AB2CE (second half of the D_800AB2CC block). ovl_14.c defines it
  with a top-level `.set`; adding `D_800AB2CE = 0x800AB2CE;` to config/symbols.ovl_14.us.txt would
  let that go.
- 38 EF258 matches (build/ready_38_EF258.c) but links against D_8006D128, which ovl_38's
  undefined_syms_auto lacks; add `D_8006D128 = 0x8006D128;` there (config change) to land it.
- Menus/FMV pass 2 (drafts in docs/wip/ovl_XX_<addr>.c, each with a status line): 64 A5B0 (3 rows,
  constant 1 vs lui in a delay slot), 64 99B4 (MBG_init; the original rematerialises each big
  constant in a3 right before use, not reproduced), 64 B7C4 / 3a 4238 (the original hoists fewer
  loop invariants than ours), 64 C434 (CD ready callback, CSE reach), 36 1A48 (5), 36 2664 (7),
  33 EF900 (13), 3c 6D34 (25, goto loop), 38 EE740 (6, was 17), 39 F973C (3, CRC type).
  65 F2560 (1 row): the original has the lui of 0x98967E in the first branch's delay slot, we
  get a nop. Not an assembler issue: 17 matched functions have the same reorder-mode branch +
  lui-only `li` and their originals have the nop. gcc's delay-slot filler placed it, so the C
  still differs (see the WIP header).
  Tricks that matched things in this pass:
  - A pointer that the original computes into a temp and then copies to an s-register
    (`a1` -> `addu s3,a1`) comes from `m2 = &X; m2->f = n; m = m2;` (3c F6F00).
  - Constants the original keeps in registers come from locals: `x = 0x18; y = 0x26;
    y = row * 15 + (v->unk2 + y);` then `x + 0x72` (3c F5BCC).
  - A loop offset that the original initialises in the loop guard's delay slot is a giv of the
    counter (`dx = i * 4 + 0x2E;`, `y = i * 15 + 6;`), and `dy = i + 9; y = r->y + dy;` keeps
    the add unfolded (36 F12B8, 38 EE740).
  - A loop-invariant constant assigned inside the loop body after the first call (`y = 0x9E;`)
    is hoisted in the original's order, which also fixed the biv/giv register order (3c 51B8,
    3c 6714).
  - A small loop whose address arithmetic is not strength-reduced and whose constants are not
    hoisted was not optimised by loop.c: write it as a goto loop (3c 6D34).
  - An uninitialised local that the original passes on one path holds whatever its register
    had, so assigning it that value (`info = (void *)&D_8019DE10;`) reproduces it (64 BCE8).
  - A call result used without `andi` means the callee was declared returning int there
    (3c 42A8 now returns s32; 3c 51B8 declares func_80028954 s32).
  - Store order inside a run of struct stores matters even though the scheduler moves loads
    past them (64 C1B0, found with a bounded statement permutation).
- Menus/FMV pass 3: matched 32 F8AB0, 33 F0024/F15E0/F1A94/F20A0, 36 F0F9C, 39 F4B18/F6694/F9120/FA794,
  65 F3488/EFC68/F2FE8, 64 990A0/99554/9A5B0. New drafts in docs/wip/: 34 EFC20 (= 39 F7FD4, 62,
  register pressure), 37 F13F8 (41), 36 F1778 (30, s-regs only; second half is func_801F2D88_inline),
  65 F1848 (35), 65 F2E58 (38), 64 99B28 (63, shares its first-frame loop with 99F30), 3a F459C (89).
  Not attempted: 64 99F30 (0x680; do 99B28 first, the decode loop is the same code), 36 F08F4 (its
  .rodata blob D_801EDF18 holds "-" plus a 0x298-byte table/text block; it needs a config symbol at
  0x801EDF1C with force_not_migration so the blob stays INCLUDE_RODATA before the string can move to C).
  Tricks that matched things in this pass:
  - Rematerialised constants: `k = 1; A = k; B = k;` (64 9A5B0) and `r->y + (y3 + (k = 0x26))` /
    `k = 0x26; ... (y3 + k)` (33 F1A94) keep a constant out of a fold or force it into a register
    first; `r->unk2 - (k1 = off - 0x28)` keeps `y - (off - c)` from becoming `(y + c) - off` (65 F2FE8).
  - A copy of a call result that the original makes in the next call's delay slot (instead of right
    after the call) comes from an inline wrapper returning it (`static inline s32 w(u8 *s) { return
    func_8002BA68(s); }`, 36 F0F9C); the copy from v0 stays grouped with the call otherwise.
  - `x = func(); ...; if (cond) x = ...` CSE: a pointer local that the original rebuilds from a
    fresh %hi/%lo in a later block means that block has two predecessors in the original
    (`if (a && b) {...} else if (c && d)` instead of nested ifs, 33 F15E0).
  - A store whose %hi is in a branch delay slot: compute the destination first
    (`u8 **d = &D_80077DF0; *d = ...;`, 33 F0024); a base that the original materialises before an
    index (`lui; addiu; addiu -0xC0; sll; addu`) comes from an inline helper doing `i -= 0xC0;`
    on its parameter after loading the base (33 F0024).
  - Loop-carried y offsets: `y = i * 15 + 7;` givs placed after the first statement of the body
    (65 F2FE8), `r->x + 6 + j * 0x21` instead of a separate counter (39 F4B18).
  - A struct copy whose destination the original addresses from a separate symbol base
    (`D_801FB1E0 + k*0x98`, with byte stores from `D_801FB1A8 + k*0x98`) needs the copy through a
    pointer: `p = &D_801FB1A8.unk34[k].unk4.unk0; *p = d->unk0;` (39 F6694).
  - Dead stores to a local RECT / pair that the original keeps (64 990A0, 99B28) stay if the local
    is a struct; scalar copies of the same value are CSEd away.
- ovl_64 library code (not in config yet, evidence for the coordinator):
  - 8019D020-8019D36C are libpress's static MDEC helpers, right after DecDCToutCallback in the
    same object: D020 = MDEC_reset (rodata "MDEC_rest:bad option(%d)\n", writes the MDEC
    control registers through D_8019E008/D_8019DFDC/D_8019DFE8), D110 = MDEC_in (DMA0
    setup, calls D22C), D1A0 = MDEC_out (DMA1 setup, calls D2C0), D22C = MDEC_in_sync
    (string "MDEC_in_sync", 0x100000 spin with timeout), D2C0 = MDEC_out_sync ("MDEC_out_sync"),
    D36C = timeout handler ("%s timeout:\n", printf then the same reset sequence as D020).
    The unscheduled `lw; nop` code and the strings match Sony's libpress, not Square's
    compiler output. D354 (already in C) is next to them and is likely libpress too.
  - 8019AE44-8019B3D4 are hand-written assembly: no frames (`sw $ra,0($sp)` without moving
    $sp), `bgez $zero` jumps between functions, scratchpad (0x1F800000) tables. AE44 builds the
    VLC table into the scratchpad (DecDCTvlcBuild), AE98 swaps the max-size word D_8019DDD4 and
    returns the old one (DecDCTvlcSize), AEC0/B00C/B188 are DecDCTvlc's decoder with shared tails,
    B3D4 is another frameless routine (sw $ra,0($sp), $t4 used before set, fills 0xE0 runs of
    words with a mask), most likely the libpress run-level/VLC helper or Square's own asm.
    None of them can come from gcc; they should be named as library/asm in config and dropped
    from progress.
- Menus/FMV pass 4: matched 36 F08F4 (new, after the D_801EDF1C split), 36 F2664, 65 F2560, 65 F2E58,
  38 EE740, 33 EF900, 33 F2AB4 (new), 33 F1150 (new), 3c F6D34. New drafts in docs/wip/: 33 EF644 (33),
  65 EF890 (72, s-register naming). Improved: 65 F1848 (35 -> 17), 34 EFC20 (62 -> 46), 3a F459C (89 -> 57). Unchanged: 36 F1A48 (5), 39 F973C (3).
  Tricks that matched things in this pass:
  - A function called with a u16-returning prototype in main.c but used without `andi` here is
    declared s32 in the overlay header (36 F08F4: func_8002C7F0/func_8002C830).
  - A loop that gcc reverses (dbra, counter 99 -> bgez) with a strength-reduced pointer is an
    ascending `for (i = 0; i < 100; i++) a[i] = 0;` (36 F08F4).
  - Register swaps around a table load: hold the loaded value in its own local inside the condition,
    `(need = D_8006DF5C[c->unkB]) <= c->unkC` (65 F2560).
  - A u8 flag returned as `!f` instead of `f ^ 1` (65 F2E58); that alone fixed the eager andi copy.
  - A loop-invariant base that the original keeps in an s-register while a second one is rebuilt each
    iteration: assign the pointer inside the loop (`tp = t.v;`) and compute the byte offset in its own
    local (`off = idx * sizeof(StatusEntry); k = ((StatusEntry *)((u8 *)tp + off))->unk0;`), which also
    gives the original's `addu base, off` operand order (38 EE740).
  - `do { f(x); } while (0);` around one call in an if/else (36 F2664) and around `ok = 0;` (3c F6D34)
    fixes s-register assignment; found by the permuter. build/dwsweep.py and build/dwgreedy.py
    (agent worktree scratch) wrap each statement in turn and score it.
  - A parameter reused as a loop counter (`for (slot = 0; ...)` in 33 EF900) plus the loop preheader
    order `m2 = m; slot = 0; a = ...; for (; ...)` (found with a bounded line permutation,
    build/permlines.py).
  - A local holding a call's constant argument (`k = 1; ... func_8002BAE4(r->unk0, buf, 5, k);`) and
    a copy of a flag into a reused local before the final calls (`id = ok;`) fixed 3c F6D34's
    s1/s2/s3 order (permuter).
  - A store whose address the original computes before the `if` that sets the value goes through a
    byte-offset struct pointer: `d = (AbilityMenu *)((u8 *)&D_801FC840 + i); ... d->unk25C[0] = lv;`
    (33 F1150).
  - `(l = &D_801FC840.unk4)->unk2` inside the condition gives the original's `addiu v1, a0, 4` without
    disturbing the a0/a1 assignment that a separate `l = ...;` statement changes (33 F2AB4).
- Menus/FMV pass 5: matched 39 F973C (CRC-16) and 64 999B4 (MBG_init). New drafts: 15 A9488 (Blackjack
  main loop, 413 of 1575 rows; integrating it needs a rodata split for its UI strings, see its header),
  15 ADE30 (28), 14 A741C (36). Improved: 64 99B28 (63 -> 46). Unchanged: 36 F1A48 (5), 36 F1778 (30),
  64 B7C4, 65 F1848 (permuter 20 min found nothing valid).
  Tricks that matched things in this pass:
  - An accumulator whose upper bits gcc must not know (the original keeps `andi 0xFFFF` before the
    shift and at the return): a union with an initializer, `union { s32 w; u16 h; } c = { 0xFFFF };`,
    then `c.w = tbl[(c.w ^ *p++) & 0xFF] ^ (c.h >> 8); return c.w;` (39 F973C). The initializer emits a
    clobber, so combine's nonzero-bits tracking gives up; `c.h = 0xFFFF` alone gives `ori v1, v1`.
  - Large constants reloaded right before each use into the same register: one local assigned before
    each use (`off = 0x43E20; p->unk5C = b + off;`) (64 999B4).
  - A `v0` that holds 1 from a mid-function store to the end with no return statement in the draft: the
    function returns that constant (`return 1;`, 64 999B4).
  - Loop layout where the "found" block sits after the loop: `do { d = f(); if (d != NULL) goto found;
    if (flag == 1) { res = -1; goto check; } } while (--tries != 0); goto check; found: ...` (64 99B28).
  - A random byte whose `&a[i]` address the original computes before the refill check: use a statement
    expression with an s32 temp, `(({ s32 r; if (D_8007B720 >= D_8007B92D) func_8004BF20(); r =
    *D_8007B720++; r; }) % 3)`. The ff9.h inline function evaluates the rhs first, and a plain u8
    statement expression gives multu (15 A9488).
  - `x -= (h.unk0 & 4) ? bet * 2 : bet;` on a global makes gcc keep the global's address in a0 and store
    through it after the branch (`addiu a0, t1, %lo(X)` ... `sw v1, 0(a0)`) (15 A9488).
  - Calls without `andi 0xFF` on the result of a u8 function defined later in the same file: the
    original had no prototype (implicit int), so leave them undeclared (15 A9488: func_800AB1B8,
    func_800ACF48).
- Pass 11 (overlays except 0b; row counts from the new build/search/cmp.py, which also compares symbol
  names: its rows for `%hi/%lo(func_X)` are false positives because ours drop func_ names outside jal,
  and offset-named symbols like D_800CA06E vs D_800CA068+6 count as rows but link identically).
  Matched 15 ADE30. Improved drafts: 14 A741C 34, 37 F13F8 40, 65 F1848 20, 0a BD130 12 (see their
  headers). No change on the rest; dup.py (identical-arm `if (var)`), build/sink.py (move the statement
  after / before an if chain into every arm), declaration shuffles and 25-minute permuter runs were tried
  on every draft in docs/wip/ for these overlays. What worked:
  * a statement after an if/else written at the end of both arms (`x += w;` in both glyph branches of
    ADE30) - jump2 merges the copies, but the arms were allocated with the extra references; the dup.py
    hit `if (p) { x += w; } else { x += w; }` pointed at it;
  * `*(f->unkC + c)` instead of `f->unkC[c]` gives the `addu v0, v0, s3` (index first) operand order
    (ADE30; main.c's func_80029058 has the same form);
  * a per-iteration element pointer (`BgSprt *sp = &arg1[i];`) changes the order of loop.c's giv
    increments at the loop end (BD130);
  * a struct field read through the global (`D_801F7A78.unk18[i]`) instead of a pointer local lets CSE
    derive the base from a related register (`addiu s5, s1, -0x1D8`), as the original does (F1848).
- Pass 12 (overlays except 0b; gcc 2.8.1 source read alongside the `cc1 -dS -dl -dg` dumps:
  build/search/dumpf.sh dumps every pass, build/search/galloc.py prints global-alloc's allocation order
  `floor_log2(refs) * refs / live_length` with the hard reg each pseudo got). Matched 36 F1A48, 0a BF660,
  0d AC060, 36 F1778. Improved: 0a C0038 19 -> 11, 33 EF644 33 -> 31. Mechanisms that decided them:
  * sched1 (sched.c) is a *backward* list scheduler. When an insn is scheduled, each predecessor that
    becomes ready gets LAUNCH priority (0x7f000001) only if it is a "birthing" insn: it sets a pseudo
    with REG_N_SETS == 1 that is live there. Launched insns are placed right next to their consumer;
    everything else (multi-set user variables, hard-reg argument loads of a function with two calls
    setting the same arg register, stores) keeps its plain priority (longest chain of >1-latency ops
    from the block start) and floats to the block top or into load-delay slots, ties by later LUID.
    Register allocation then sees those positions, so a constant argument load that floats up makes a
    local temp conflict with a1..a3 (F1A48: two copies of the func_80022C60 call in an if/else, cross-
    jumped by jump2, give REG_N_SETS(a1) == 2 and put x in t0).
  * local-alloc allocates qtys by `floor_log2(refs) * refs / length`, ties to the earlier-born qty, and
    takes the lowest free hard reg; a qty born in the insn where another dies shares its register. So
    `cy = t + y` with a block-local `t = (h >> 1) + y0 - 112` lands t in place in v0 and the add is
    `addu a2, v0, a2` (temp first); a reused global `x` for the same value takes a0 (BF660, C0038).
    expand_binop puts the operand that equals the target first, so `y = x + y` becomes `y + x`.
  * global-alloc: pass 0 only considers registers already used (regs_used_so_far) and not preferred by
    a lower-priority conflicting allocno; pass 1 takes the first non-conflicting register. A separate
    loser-card variable `l` (`l = a` in one arm coalesces into a deleted no-op move, `l = b` in the
    other) has the highest priority, takes s0, and its conflict with `e` pushes e to s3 (AC060).
  * Two stores through the same base with the stored values in registers vs. computed: a RECT whose
    w/h constants live in s-registers and whose `&a` is an argument gets its w/h stores addressed from
    the argument register (a0 + 0xC), so they get output dependences on the sp-relative x/y stores and
    schedule after them: write the fields in x, y, w, h order (F1778's inlined func_801F2D88 copy).
  * jump.c duplicate_loop_exit_test only runs when NOTE_INSN_LOOP_BEG is directly followed by a jump
    (a rotated for/while); a `goto test` into a do/while keeps a single copy of the loop test (A741C).
  Left with notes in their headers: B4A5C (filler slots), C07D8 (lbu placement), A741C (n/blk and
  id/base/gs order), B16A4 (ot s1 vs s3), B0124 (kind-9 block, condition spellings score 14-20).
  * loop.c moves an invariant only if `threshold * savings * lifetime >= insn_count` (threshold =
    (1 + non-fixed regs) with calls in the loop, minus 3 per insn already moved; lifetime = LUID
    distance between the pseudo's set and last use, deleted-insn notes count). A `%hi` for a global
    store is emitted before the stored expression, so `D_801F8200 = r = &D_801F7A78.unkAC[i + 2].unk4;`
    (chained) gives it a lifetime of several insns and it is hoisted into s6, while `D_801F8200 = r;`
    (lifetime 1) stays in the loop. `rr = &D_801F81F8;` written inside the block (hoisted too) keeps
    its preheader order after that lui (65 F1848, matched this pass).
