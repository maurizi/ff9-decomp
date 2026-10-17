/*
 * ovl_0a func_800BD130 (0x270 bytes): builds the two (double-buffered) sprite lists of a background
 * layer: per tile a SPRT (with the semi-trans bit from the tile) plus a DR_TPAGE merged with
 * MargePrim, tracking the highest CLUT row in D_800CA108.
 * WIP (pass 10): 156 of 156 instructions, 12 aligned diff rows (was 48).
 * Fixed: the tpage call is made in both arms of `if (tile[i].tp == 0) ... GetTPage(0, ...) else
 * GetTPage(1, ...)` with a u16 result (jump2 cross-jumps the identical argument setup, leaving the
 * original's branch for the 0/1 instead of a store-flag); the outer loop is a for loop; prologue
 * statement order w, pos, tile, unk18, unk1A, n, unk25 (brute force).
 * Left: prologue scheduling (the original loads D_800C9DB4 and D_800CA108 first, D_800CA108 into
 * a1, and n = unk26 straight into s4) and the order of the pos/arg1 giv increments at the loop end
 * (original: s2 (pos) before s0 (arg1)). Permuter hint: `n = arg0->unk2C; tile = ... + n;` puts
 * arg0/n in s5/s4 like the original but breaks the prologue order (24 rows).
 * Pass 11 (new build/search/cmp.py): a per-iteration `BgSprt *sp = &arg1[i];` fixes the giv
 * increment order at the loop end (pos before arg1), 14 -> 12 rows. Left: only the prologue (the
 * original loads D_800C9DB4 before D_800CA108 and computes pos and tile back to back); all 5040 orders
 * of the seven prologue statements, variable reuse for the unk28/unk2C/D_800CA108 loads and declaration
 * shuffles keep 12.
 * Pass 13: pos/tile are reload spills (the stack stores follow their defs); the original's chain has
 * both computations adjacent and before the unk18 loads (so the spill stores precede the lhu loads,
 * which after reload depend on any sp-relative store), i.e. sched1 did not float tile's computation
 * down as a free filler. Making pos/tile memory variables (struct, array, address-taken, volatile)
 * adds loop reloads (83-104 rows). Same filler-LUID class as B4A5C.
 * Pass 19: 12 -> 9 rows with `Unk800C9DB4 *bg = D_800C9DB4;` as a local (D_800C9DB4 now loads before
 * D_800CA108, into a0, like the original), then the 7! orders of the prologue lines (build/pp.py) with tile
 * before pos. Left: the original loads D_800CA108 (lbu a1) at the top but adds 0xDF only after the pos/tile
 * addu pair, and both addu's (and their spill stores) precede the unkC/unk30 loads. The lbu is QImode, so
 * it cannot pass the `arg0->unk30 = arg1` store (true_dependence's in-struct exception excludes QImode):
 * the w statement must precede that store in sched1, but every such order (and `w = D_800CA108; ... w += 0xDF`,
 * a u8 temp) puts tile's addu after the unk18 block again (9-21 rows). Permuter (15 min on this base) best:
 * the unk18/unk1A stores before w, n right after (permuter score 515, 15 cmp rows), not kept.
 * Pass 21: no change (9 rows). Splitting the D_800CA108 read from the + 0xDF (`x = D_800CA108; ... w = x + 0xDF;`,
 * also through w, a u8 or an s32 temp) at every pair of prologue positions (180 variants) and single-line moves
 * within each straight-line run give 9 at best.
 * Pass 23: the param lever does not apply: the three arg copies are pinned at the top (sched1 skips SETs from hard
 * regs up to NOTE_INSN_FUNCTION_BEG), arg2 is spilled to its home slot as in the original, and combine deletes no
 * arg copy. Note that w (`w = D_800CA108 + 0xDF`) is multi-set (the max update), so its addiu is not "birthing" in
 * sched1 and sorts by priority/LUID against the pos/tile adds.
 * Pass 24: the draft below (9 rows) is BEHAVIOURALLY EQUIVALENT to the original (tools/equiv.py: 67/67 trials
 * agree, 15 paths, 100% of the original's insns ran). New structure found, 10 rows but everything from 0x50 on
 * byte-exact (only the four top loads' order and a0/a1 swapped):
 *     bg = D_800C9DB4;  u = D_800CA108;  u += 0xDF;  n = arg0->unk26;  arg0->unk30 = arg1;
 *     pos = ...unk28;  tile = ...unk2C;  w = u;  unk18;  unk1A;  unk25 = 0;  unk25_1 = 0;
 *   (u a block-local s32). Why each piece:
 *  - `n = arg0->unk26` early: n's lhu (birthing, low LUID) fills the sched1 stall after the unk18 add, so pos/tile
 *    (initially-ready insns keep their raw priority 2 - adjust_priority only boosts insns as they become ready)
 *    are scheduled before the unk18 block, with their spills, as in the original (pos before tile in source);
 *  - the D_800CA108 value must be a *local* pseudo with two sets: non-birthing, so its lbu stays at the very top
 *    (above the unk30 store it may alias as a QImode load) and local, so it can take a1 before local-alloc gives
 *    a1 to the 0xFFFF01FF constant (K, the unk25_1 mask). Through a global (w, or the loop's x) K takes a1 (11 rows,
 *    then everything else matches too).
 *  - left: local-alloc ranks u (4 refs, 11 insns: 3636) above bg (5 refs, 15 insns: 3333), so u gets a0 and bg a1.
 *    build/search/lsim.py confirms bg+1 ref or u-1 ref gives bg a0 / u a1 / K a2; no natural spelling found.
 *    Making the arg1 copy late (`s32 f(.., BgSprt *sp1, ..) { BgSprt *arg1; ... arg1 = sp1; arg0->unk30 = arg1;`
 *    after unk25_1; a parm copy that combine merges into a local is no longer pinned at the top) keeps a1 busy at
 *    K's birth: with the loop's x as the D_800CA108 holder that gives K a2 / x a1 / bg a0 (top exact) but x is
 *    then a1 in the loop too (orig v1). Copying arg0 instead halves arg0's live length and reorders the s-regs.
 *  - simulated annealing over these line orders (build/w/sa.py, ~10k compiles), the arg1 copy line, buf = 0
 *    placements, do-while wraps: 10 at best in that regime.
 * Pass 26: 9 -> 4 rows; everything except the first four loads is byte-exact (bg a0, u a1, K a2 now right).
 * STATUS: 4 rows. The lever is a second bg pointer: `bg2 = bg;` after the tile line, with bg2 used for the
 * unk30/unk32 reads, and a dead `bg2 = NULL;` at the end so cse keeps the copy (bg2 becomes canonical because
 * it is mentioned after the cse block; a dead initializer at the top does not work here since block 0 starts
 * the function). local-alloc ties bg and bg2 (bg dies in the copy), the tied qty has 7 refs, so it ranks
 * above u (bg 4667 vs u 3636) and gets a0; the copy is then a0 -> a0 and reload deletes it.
 * Left: %hi(D_800CA108) is v0 instead of v1, so sched2 keeps lui/lbu of D_800CA108 above the D_800C9DB4 pair.
 * For v1, HB's life (lui v0, %hi(D_800C9DB4) .. lw bg) must overlap HU's (lui %hi(D_800CA108) .. lbu) with HB
 * allocated first, e.g. sched1 order HU x HB BL UL or HB HU BL UL. In the backward list scheduler HB is launched
 * (single-set) the moment BL is scheduled and HU the moment UL is scheduled, and UL (non-birthing, priority 1)
 * is always scheduled after BL and after the unk30 store (it must stay above that store), so sched1 always
 * gives HU UL .. HB BL (no overlap). SA over the 13 prologue lines with the bg2 copy (3000 orders), do-while
 * wraps of the bg/u loads, u single-set (birthing; the lbu then sinks below the unk30 store, 8 rows), the
 * `x op= K` / `&= 0xFF` levers on u (16), dead tests on bg/u (39+, a block end changes the whole sched1
 * order of block 0): nothing below 4. Still BEHAVIOURALLY EQUIVALENT (tools/equiv.py: 27/27 trials agree,
 * 6 paths, 100% of the original's insns ran).
 *  - A dead test right after `u = D_800CA108;` (`if (bg != NULL) { x = 0; }`, deleted by jump2) cuts block 0
 *    there and sched1 then gives exactly the original's HB HU BL UL with %hi(D_800CA108) in v1 (18 rows), but
 *    the 0xFFFF01FF mask K (a single `li` before the unk25_1 `and` that sched1 splits into lui/ori and floats
 *    to the top of its block) can no longer reach block 0a, and u/bg become globals (u in v0). A K computed
 *    before the cut would need the unk24..unk26 word masked by hand instead of the bitfield stores.
 *  - A lone CLOBBER (dead block-local `struct { s32 x; } q = { i };`, the lever that matched B4A5C) is a free
 *    priority-1 filler in both schedulers, but its pseudo has no GR uses (class ST_REGS or none), so reload
 *    gives it a stack slot; this frame has no free word, so it costs 8 bytes of frame here.
 *  - The top four insns come out exactly right (HB HU BL UL, v0/v1/a0) with dead address-of statements that
 *    pin the %hi LUIDs: `pb = &D_800C9DB4; pu = &D_800CA108;` first (cse reuses their lui for the later loads,
 *    flow deletes the dead lo_sums), then `bg = D_800C9DB4; u = D_800CA108 + 0xDF;` (u single-set, so the lbu
 *    is launched) and `w = u;` right after (10 rows: build/w/vb12.py). sched1 then picks UL, BL, HU, HB as
 *    needed, but u lives only from the lbu to the w add and gets v0 (original a1: there the add comes after the
 *    pos/tile adds). Moving `w = u` later makes the launched lbu ready too early (it is picked before BL and
 *    HU follows it). All 720 orders of N/S30/P/T/Z/W in that setup: 9 rows at best.
 */
#include "ovl_0a.h"

s32 func_800BD130(Unk800C9DB4Obj *arg0, BgSprt *arg1, BgSprt **arg2) {
    BgPos *pos;
    BgTile *tile;
    u32 i;
    u32 buf;
    s32 n;
    s32 w;
    s32 x;
    u16 t;

    Unk800C9DB4 *bg;
    Unk800C9DB4 *bg2; /* see the pass 26 note */
    s32 u;

    bg = D_800C9DB4;
    u = D_800CA108;
    u += 0xDF;
    n = arg0->unk26;
    arg0->unk30 = arg1;
    pos = (BgPos *)((u8 *)bg + arg0->unk28);
    tile = (BgTile *)((u8 *)bg + arg0->unk2C);
    bg2 = bg;
    w = u;
    arg0->unk18 = arg0->unkC + bg2->unk30;
    arg0->unk1A = arg0->unkE + bg2->unk32;
    arg0->unk25 = 0;
    arg0->unk25_1 = 0;
    for (buf = 0; buf < 2; buf++) {
        for (i = 0; i < n; i++) {
            BgSprt *sp = &arg1[i];
            setlen(&sp->unk8, 3);
            sp->unk8.code = 0x7C;
            sp->unk8.r0 = 0x80;
            sp->unk8.g0 = 0x80;
            sp->unk8.b0 = 0x80;
            sp->unk8.x0 = arg0->unk18 + pos[i].x;
            sp->unk8.y0 = arg0->unk1A + pos[i].y;
            sp->unk8.u0 = tile[i].u;
            sp->unk8.v0 = tile[i].v;
            if (tile[i].trans) {
                sp->unk8.code |= 2;
            } else {
                sp->unk8.code &= ~2;
            }
            sp->unk8.clut = GetClut(tile[i].clutX << 4, tile[i].clutY);
            if (tile[i].tp == 0) {
                t = GetTPage(0, tile[i].abr, tile[i].tpX << 6, tile[i].tpY << 8);
            } else {
                t = GetTPage(1, tile[i].abr, tile[i].tpX << 6, tile[i].tpY << 8);
            }
            setDrawTPage(&sp->unk0, 1, 1, t);
            MargePrim(&sp->unk0, &sp->unk8);
            x = tile[i].clutY;
            if (w < x) {
                w = x;
            }
        }
        arg1 += n;
    }
    *arg2 = arg1;
    D_800CA108 = w + 0x21;
    bg2 = NULL;
    return 1;
}
