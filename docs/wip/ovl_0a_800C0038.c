/*
 * ovl_0a func_800C0038 (0x298 bytes): starts a field camera scroll towards the player actor
 * (projects it, clamps to the camera limits, stores the target in D_800CA068.unk14/16).
 * WIP (pass 12): 166 of 166 instructions, 11 aligned diff rows (was 19 in pass 10, 44 before).
 * Pass 12: x/y are built as { s32 t = (c->unk24 >> 1) - 0xA0; x = t + xy[0]; } with a block-local t
 * (as in BF660, now matched): t dies into the add, x gets a fresh register and xy[0] loads early.
 * Still left: (1) the st->unk4 / a->unk14 loads swap v0/v1 at 0xA8; (3) the flag block (f in a1,
 * f | 3 in v1 stored in the bne delay slot, no cross-jump of the two stores). Tried this pass: the
 * if-arm as |= 8 (cse folds it to the same f | 0xB), a t = f | 3 temp, early return 1 after the
 * f | 3 store: all 11. The flag block needs v0 busy while f | 3 is born (local-alloc gives the
 * earlier-born qty v0 on ties), i.e. the li v0, 8 compare constant allocated before f | 3.
 * Fixed: the clamps are if / else if with the limit re-read in each arm (as in BF660);
 * `x = (c->unk24 >> 1) - 0xA0; x += xy[0];` (same for y); the ~0xF mask of the flag block
 * goes in its own statement (`f = ~0xF; f &= D_800CA068.unk0;`) so it is built in the delay slot.
 * Left: (1) the a->unk14 / st->unk4 loads swap v0/v1 at 0xA8; (2) x/y: xy[0] should load into
 * a0 and x into a1; (3) the flag block: in the original f is in a1 and f | 3 in v1, so the
 * arg1 == -1 store is not cross-jumped with the f | 0xB store; here f lands in v1, f | 3 in v0,
 * and jump2 merges the two `sw v0` stores. Storing f | 3 in both arms of an if/else stops
 * the cross-jump but loses the unconditional store. Has GTE asm, so the permuter can't parse it.
 * Pass 13: the flag block's allocation needs `f | 3` (v1) to overlap the compare constant 8 (v0), so f
 * is pushed to a1. In every spelling tried the constant's set is single-set/single-use, so sched1
 * launches it right before the bne (after the `f | 3` store; update_equiv_regs would move it there
 * anyway), the two locals never overlap and both take v0. Tried: `k = arg1 == 8` before the store
 * (becomes xori/bnez), the constant in a reused variable (`r = 8; ... r = 1; return r;`: cse folds r),
 * `g = f | 3; unk0 = g; if (arg1 == 8) unk0 = g | 8;` (combine rewrites g | 8 to f | 0xB and g dies at
 * the store), a stored `g` in both arms (flow deletes the back-to-back store). What would work: a
 * compare whose constant has 2 uses or 2 sets, or a store of `f | 3` that sits after the bne in RTL
 * yet in the same block (none found).
 * Pass 18: flag block fixed, 4 rows left: `f = unk0 & ~0xF; unk0 = f | 3; if (arg1 == 8) unk0 = f | 8 | 3;
 * else unk0 = f | 3;` (a redundant else store, like the matched sibling C0DF4).
 * Left: the st->unk4 copy / a->unk14.vx loads swap v0/v1 at 0xA8. The struct copy `st->unk10 = st->unk4`
 * (C0DF4's spelling) gets the registers right, but its lw then lands after the lhu (with vx first: 4 rows).
 * hill.py over the setup block, declmove, and every st-vs-D_800CA068 spelling: no gain.
 * Pass 19 (local-alloc + sched trace, build/lasim.py in the pass-19 worktree simulates the block's
 * local-alloc from an insn order and reproduces the observed dispositions). Insns: HB = lui %hi(D_800C9DB4),
 * HS/ST = &D_800CA068, BL = bg load, CL/CS = copy load/store, VL/VS = vx load/store, UL = bg->unk18 load,
 * ADD = c. All six locals are 2-ref qtys except bg (3), so qty_compare is pure live length with ties to
 * the earlier birth. Our sched1 order is HB HS ST BL CL UL VL CS VS ADD (copy v1, vx v0). The order
 * `c = ...; v.vx = ...; st->unk10 copy;` (C X P) gives HB HS ST BL VL UL CL VS CS ADD: every register is
 * the original's (copy v0, vx v1, bg a0, unk18 a2), 4 rows, but sched2 keeps VL before CL and VS before CS
 * (after reload the sp store and the st store are output-dependent, so their sched1 order is final; CL
 * then ranks below VL because CS was the last insn scheduled). The original needs a sched1 order with
 * CS before VS and the right registers; of the 70 such orders lasim finds, the plausible one is
 * HB HS ST BL UL CL VL CS VS ADD, i.e. the unk18 load issued before both loads. In the backward list
 * scheduler UL only becomes ready after ADD is scheduled, and the ready list is re-sorted only when new
 * insns arrive (SCHED_SORT), so the class/LUID ranking at T-46 decides it; no statement order gets there
 * (all 7! orders of the setup lines, plus 168 block/bg/temp variants: best 4). A block-local
 * `{ s16 vx = a->unk14.vx; c = ...; v.vx = vx; }` after the copy gets the final load order right but
 * gives bg a2 / unk18 a0 and changes the y clamp (9-10 rows). The body below is the C X P order.
 * Pass 20: copy spellings `st->unk10 = st->unk4` (6), `D_800CA068.unk10 = D_800CA068.unk4` (4),
 * `s32 *cp = &st->unk4; cp[3] = cp[0]` (4), an early `s32 off = bg->unk18` with the copy before c (4), and
 * a block-local s32 / Pair temp for the copy load (13, two extra insns): no gain.
 * Pass 21: no change (4 rows). All 280 topological orders of the setup lines with the bg->unk18 load as its own
 * statement (`off = bg->unk18; c = (Cam *)((u8 *)bg + off);`), and single-line moves within every straight-line
 * run (build/w/mvrun.py), stay at 4.
 * Pass 23: the param lever does not apply (arg0/arg1 are only used at the end, their entry copies already match).
 * The inline `func_800C9DB4_cam(0)` + `c += st->unk75` (as in C0F08) compiles identically (4); the inline with
 * the index (`func_800C9DB4_cam(st->unk75)`) re-reads D_800C9DB4 (169 insns, 40-45 rows).
 */
#include "ovl_0a.h"

s32 func_800C0038(s32 arg0, s32 arg1) {
    SVECTOR v;
    s16 xy[2];
    Unk800C9DB4Cam *c;
    Unk8004AB64 *a;
    u8 mode;
    s32 x;
    s32 y;
    s32 t;
    s32 f;
    s32 ox;
    s32 oy;
    Unk800C9DB4 *bg;
    Unk800CA068 *st;

    if (!(D_800CA068.unk0 & 0x40)) {
        return 0;
    }
    mode = g_gameState->mode;
    if (mode == 5) {
        a = (Unk8004AB64 *)g_gameState->unk1C->unk8E0[g_gameState->unk1C->unk8E4.field->unk8].unkC;
    } else {
        a = NULL;
        if (mode == 1) {
            a = func_8004AE00();
        }
    }
    bg = D_800C9DB4;
    st = &D_800CA068;
    c = (Unk800C9DB4Cam *)((u8 *)bg + bg->unk18);
    v.vx = a->pos.vx;
    *(s32 *)&st->unk10 = *(s32 *)&st->unk4;
    v.vy = a->pos.vy - st->unk76;
    c = &c[st->unk75];
    v.vz = a->pos.vz;
    ox = (s16)c->unk20;
    oy = (s16)c->unk22;
    gte_SetGeomOffset(ox + 0xA0, oy + 0x70);
    gte_SetRotMatrix(st->unk1C);
    gte_SetTransMatrix(st->unk1C);
    gte_ldv0(&v);
    gte_rtps();
    gte_stsxy(xy);
    {
        s32 t = (c->unk24 >> 1) - 0xA0;
        x = t + xy[0];
    }
    {
        s32 t = (c->unk26 >> 1) - 0x70;
        y = t + xy[1];
    }
    if (x < c->unk28) {
        x = c->unk28;
    } else if (c->unk2A < x) {
        x = c->unk2A;
    }
    if (y < c->unk2C) {
        y = c->unk2C;
    } else if (c->unk2E < y) {
        y = c->unk2E;
    }
    D_800CA068.unk14 = x;
    D_800CA068.unk16 = y;
    if (arg0 == -1) {
        D_800CA068.unk1A = 0x1E;
    } else {
        D_800CA068.unk1A = arg0;
    }
    D_800CA068.unk18 = 1;
    if (arg1 == -1) {
        D_800CA068.unk0 = (D_800CA068.unk0 & ~7) | 3;
    } else {
        f = D_800CA068.unk0 & ~0xF;
        D_800CA068.unk0 = f | 3;
        if (arg1 == 8) {
            D_800CA068.unk0 = f | 8 | 3;
        } else {
            D_800CA068.unk0 = f | 3;
        }
    }
    return 1;
}
