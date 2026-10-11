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
    mode = D_8006794C->unk8;
    if (mode == 5) {
        a = (Unk8004AB64 *)D_8006794C->unk1C->unk8E0[D_8006794C->unk1C->unk8E4.field->unk8].unkC;
    } else {
        a = NULL;
        if (mode == 1) {
            a = func_8004AE00();
        }
    }
    bg = D_800C9DB4;
    st = &D_800CA068;
    c = (Unk800C9DB4Cam *)((u8 *)bg + bg->unk18);
    *(s32 *)&st->unk10 = *(s32 *)&st->unk4;
    v.vx = a->unk14.vx;
    v.vy = a->unk14.vy - st->unk76;
    c = &c[st->unk75];
    v.vz = a->unk14.vz;
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
