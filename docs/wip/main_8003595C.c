/* 6 rows (pass 19; 12 before), registers only: unk2/unk3 land in t6/t3 here instead of t3/t6, and the y sum is y + (unk3 + dy) (the original adds unk3 first). Everything else (P2 in LO -> t9, P1 in t5, the D_80077170 hi in t7, the 0x2C808080 constant in t4) now matches. Older notes: Needed: all AnimCell loads before setlen (the sb blocks them otherwise), dx/dy temps so -0x80 is not reassociated, and the tpage bits in a temp. Pass 15: D_8007716C/D_80077170 as one struct { ot; prim; } makes it much worse (the base address is materialised). Pass 16: in local-alloc the two products prefer LO (94 gets LO in pass 1, 102 falls back to GR in pass 2), so the t5/t9 choice is decided by which product wins LO and by reload's mflo register. The addPrim/advance through D_80077170 instead of p (the 80025ED4 fix) changes nothing. Pass 17: in sched1 order P1 (mult insn 23 -> sra insn 24) spans 15 insns and P2 (42 -> 43) spans 16, so P1 has the higher QTY_CMP_PRI and wins LO; for P2 to win (as in the original, where P2 is reloaded from LO into t9 and P1 is GR t5) P2's span has to be strictly shorter. Tried: sw/sh/dx/dy statement orders (12-78), variable reuse (dy into dx, tp into t or dx: 37-91), a 25-min permuter run (base 65, no improvement). Pass 18: `scale(a, b) = (a * b) >> 12` as a static inline (s32 params) or a macro changes nothing (12); u16 params give 21. No neighbour shares the code: 800357E0 (the unscaled SPRT version) is matched but has no product. Pass 19 (6 rows, the draft below): with t; sw; tp; dx; sh; x = x + unk2 + dx; dy; y += unk3 + dy, P1 spans 4..19 and P2 9..23 in sched1 order, so P2 wins LO as in the original. y written as y + (unk3 + dy) ties unk3 to the sum (a 4-ref qty), which takes t3 first. That's why unk2/unk3 come out swapped. With the original's (y + unk3) + dy (12 rows), local-alloc QTY_CMP_PRI in sched1 order is: sw and sh 3 refs/16 = 1875; the 0x2C808080 constant (5..18) and unk2 (11..24) both 2/13 = 1538, an exact tie that goes to the constant (lower qty number), so the constant takes t3; P1 1333. The original needs unk2 allocated before sw and sh: unk2 in t3 across sw's birth pushes sw to t4, sh to t5, the constant to t4 and P1 to t5. That needs unk2's span <= 10 insns (2000 > 1875). unk2 can't tie to the sum, since x + unk2 reads hard a0 first and only records an a0 suggestion. Sweeps (bounded, random, dependency-respecting): 3000 orders of the 11 setup statements x 3 sum forms (best 6, this draft); 4000 orders including the xy/uv stores with the (y + unk3) + dy form (best 12); 3000 orders with x += unk2 as its own movable statement and three y forms (best 12; x += unk2 first gets unk2 into t3, but then P1/the D_80077170 hi/unk3 move: 14). s16/u16 x, y params: 75-83. 15 min of permuter on each of the 12-row and 6-row bases: only junk (c->unk0.unkA++; c->unk0.unkA--;). */
#include "ff9.h"

void func_8003595C(s32 x, s32 y, s32 otz, Unk80035B90 *unused, AnimCell *c) {
    u32 t;
    s32 sw;
    s32 dx, dy;
    u32 tp;
    s32 sh;
    PolyFT4Words *p;

    t = c->unk0.unk4;
    sw = (c->unk0.unk8 * c->unk10) >> 12;
    tp = ((t >> 16) & 0x1F) | (((t >> 21) & 3) << 5) | (((t >> 23) & 3) << 7);
    dx = ((c->unk0.unk8 - sw) >> 1) - 0x80;
    sh = (c->unk0.unkA * c->unk12) >> 12;
    x = x + c->unk0.unk2 + dx;
    dy = ((c->unk0.unkA - sh) >> 1) - 0x80;
    y += c->unk0.unk3 + dy;
    setlen(D_80077170, 9);
    p = (PolyFT4Words *)D_80077170;
    p->rgbc = 0x2C808080;
    p->xy0 = (x & 0xFFFF) | (y << 16);
    p->xy1 = ((x + sw) & 0xFFFF) | (y << 16);
    p->xy2 = (x & 0xFFFF) | ((y + sh) << 16);
    p->xy3 = ((x + sw) & 0xFFFF) | ((y + sh) << 16);
    p->uv0clut = c->unk0.unk0 | (c->unk0.unk1 << 8) | ((u16)c->unk0.unk4 << 16);
    p->uv1tpage = (c->unk0.unk0 + c->unk0.unk8 - 1) | (c->unk0.unk1 << 8) | (tp << 16);
    p->uv2 = c->unk0.unk0 | ((c->unk0.unk1 + c->unk0.unkA - 1) << 8);
    p->uv3 = (c->unk0.unk0 + c->unk0.unk8 - 1) | ((c->unk0.unk1 + c->unk0.unkA - 1) << 8);
    addPrim(&D_8007716C[otz], p);
    D_80077170 = (u8 *)(p + 1);
}
