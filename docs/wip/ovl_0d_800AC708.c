/* ovl_0d func_800AC708 (1688 B): WIP, not matching (pass 21: 363 cmp rows).
 * Control flow and stack layout are right; 426 vs 422 insns, ~370 diff rows, mostly register allocation.
 * Main problem: gcc hoists the loop-invariant part of t * 42 (21t) out of the
 * loop into a stack slot, which the original never does. Splitting it as
 * `w = t * 42; w /= 28;` stops the full hoist but 21t still moves.
 * Original keeps t in fp, e in s2, the prim pointer in s4 (giv p+6 in s0)
 * and spills i/x/w/h/len/magic/ex/ey at sp+0xF0..0x10C. Needs TmFlashTbl in ovl_0d.h.
 * Pass 7: a goto loop (no loop.c) drops to 397 insns and ~500 rows, so the original does use
 * loop.c (the p+6 giv); the hoisting difference has to come from elsewhere.
 * Pass 15: the original's pre-loop code materialises the /28 magic (0x92492493) first, spills it (0x104) and only
 * then computes len = t * 51 / 28 with that register (0x100): that is loop.c's preheader, i.e. len (and the
 * divisions' magic) are loop invariants written inside the loop and hoisted by loop.c, not a pre-loop statement.
 * `len = t * 51 / 28;` as the first statement of the loop body reproduces that preheader shape (magic, i = 0
 * spill, len spill), but t then gets spilled and 21t is still hoisted; the rows (373 -> 409-443) are all
 * register allocation, so it is not in the body below yet. Pass 17: added the missing #include "ovl_0d.h".
 * Pass 19: loop.c's dump (`dumpf.sh f.c -dL`) shows the case-1 t * 42 chain (4 insns, savings 4 because the
 * case-2 copy matches it) moved to the preheader; the original keeps it in both cases. 64 spellings of the two
 * w computations (t * 42 / 28, `w = t; w = w * 42 / 28`, t * 21 * 2, ...) and a user-variable start (`w = t;
 * w *= 42;`, lever: loop.c does not hoist through a user variable) never get below 373 rows / 426 insns
 * (341 rows at 429 insns with `w = t; w = w * 42 / 28;` in both cases).
 * Pass 20 (loop.c arithmetic, `dumpf.sh f.c -dL`): move_movables moves a movable when
 * threshold * savings * lifetime >= insn_count, threshold = (call ? 1 : 2) * (1 + n_non_fixed_regs) - 3 per insn
 * already moved. Our loop has 225 insns; moves: 197 (savings 1, life 92), 228 (3, 3), 233 (4, 4), then the 21t chain
 * 420 (4, 4). From 228 being moved here and "not desirable" in the len-in-loop variant (233 insns), the initial
 * threshold is 28-31, so 420 stays in the loop only if 5-6 movables are moved before it (or its savings*life
 * drops to 12 with 4 moves before). The original's preheader is the /28 magic (lui/ori) *before* the t * 51 chain,
 * then mult/mfhi/sra for len: so the magic's first occurrence in the loop body precedes len's t * 51, i.e. len is
 * probably computed in the loop after a `/ 28` that appears earlier in the body (case 1's w), not as its first
 * statement. Not tried yet.
 * Pass 21: `len = t * 51 / 28;` moved into the loop (first in the loop, in the e->unk0 block, at each point of the
 * unk504 == 2 block before the switch, or per case right before its first use) gives 353-477 rows and 425-441
 * insns, all worse than 373. The original's e is s2 (`addu s2,s0,zero` from &s->unk350 in s0, which is then
 * reused for the p + 6 giv), ours spills e to 0xF0 where the original spills i; the original's frame is 8 bytes
 * smaller (0x138) and it zeroes only three of x/y/w/h on the stack. Single-line moves (456 compiles): the
 * unk504 == 1 block's `p->y0` store right after `p->x0` gives 373 -> 363 (in the body below).
 */
#include "ovl_0d.h"

typedef struct {
    s32 unk0; /* 0: horizontal bar, 1: corner, 2: vertical bar */
    s32 unk4;
    s32 unk8;
    s32 unkC;
    s32 unk10;
    s32 unk14; /* x offset */
    s32 unk18; /* y offset */
} TmFlash;

typedef struct {
    TmFlash v[8];
} TmFlashTbl;
extern TmFlashTbl D_800A72B8;

void *func_800AC708(u32 *ot, TILE *p, TmState *s) {
    TmFlashTbl tbl;
    TmFlash *d;
    TmUnk350 *e;
    s32 i;
    s32 t;
    s32 c;
    s32 len;
    s32 bx;
    s32 dx;
    s32 dy;
    s32 ex;
    s32 ey;
    s32 by;
    s32 x;
    s32 y;
    s32 w;
    s32 h;

    tbl = D_800A72B8;
    y = 0;
    h = 0;
    w = 0;
    x = 0;
    e = s->unk350;
    t = s->unk2E8;
    if (s->unk418 == 0) {
        c = t * 30;
        if (c >= 0x100) {
            c = 0xFF;
            if (s->unk504 == 2) {
                s->unk4F0 = 0x32;
                s->unk57C = 0;
                s->unk4FC = s->unk500;
                func_800B01E8(10);
            }
            s->unk418 = 1;
        }
    } else {
        c = 0xFF - (t - 10) * 15;
        if (c < 0) {
            c = 0;
            if (s->unk420 == -1) {
                func_800ABEC4(s);
            } else {
                s->unk0 = s->unk420;
            }
        }
    }
    len = t * 51 / 28;
    for (i = 0; i < 10; i++, e++) {
        if (e->unk0 != 0) {
            if (s->unk418 == 1) {
                func_800B19D4(s, e->unk4, e->unk8)->unk4->unk14 = s->unk41C;
            }
            if (s->unk504 == 1) {
                setlen(p, 3);
                p->code = 0x62;
                p->x0 = e->unk4 * 43 + 0x48;
                p->y0 = e->unk8 * 52 + 8;
                p->w = 0x2A;
                p->h = 0x33;
                p->r0 = c;
                p->g0 = c;
                p->b0 = c;
                AddPrim(ot + 0x1E, p);
                p++;
            }
            if (s->unk504 == 2) {
                c = 0xFF - t * 9;
                d = &tbl.v[e->unkC];
                dx = d->unk4;
                dy = d->unk8;
                ex = d->unkC;
                ey = d->unk10;
                bx = e->unk4 * 43 + (d->unk14 + 0x48);
                by = e->unk8 * 52 + (d->unk18 + 8);
                if (c < 0) {
                    c = 0;
                }
                switch (d->unk0) {
                case 0:
                    x = bx;
                    w = 0x2A;
                    h = 4;
                    y = by - len * dy;
                    break;
                case 1:
                    w = t * 42;
                    w /= 28;
                    h = 4;
                    y = by - len * dy;
                    x = bx + w * dx;
                    if (dy == -1) {
                        y -= 4;
                    }
                    setlen(p, 3);
                    p->code = 0x62;
                    p->y0 = y;
                    p->x0 = x;
                    p->w = w;
                    p->r0 = c;
                    p->g0 = c;
                    p->b0 = c;
                    p->h = h;
                    AddPrim(ot + 0x1E, p);
                    p++;
                    x = bx + w * ex;
                    w = 4;
                    h = len;
                    y = by - len * ey;
                    if (ex == 1) {
                        x -= 4;
                    }
                    if (ey != 0) {
                        y += 4;
                    }
                    if (h >= 4) {
                        h -= 4;
                    }
                    break;
                case 2:
                    w = t * 42;
                    w /= 28;
                    y = by;
                    h = 0x33;
                    x = bx + w * dx;
                    w = 4;
                    break;
                }
                setlen(p, 3);
                p->code = 0x62;
                p->y0 = y;
                p->x0 = x;
                p->w = w;
                p->r0 = c;
                p->g0 = c;
                p->b0 = c;
                p->h = h;
                AddPrim(ot + 0x1E, p);
                p++;
            }
            if (s->unk504 == 3) {
                setlen(p, 3);
                p->code = 0x62;
                p->x0 = e->unk4 * 43 + 0x48;
                p->w = 0x2A;
                p->h = 0x33;
                p->y0 = e->unk8 * 52 + 8;
                c = 0x80;
                if (!(s->unk2E8 & 4)) {
                    c = 0x76;
                }
                p->r0 = c;
                p->g0 = c;
                p->b0 = c;
                AddPrim(ot + 0x1E, p);
                p++;
            }
        }
    }
    SetDrawTPage((DR_TPAGE *)p, 0, 0, GetTPage(0, 1, 0, 0));
    AddPrim(ot + 0x1E, p);
    return (DR_TPAGE *)p + 1;
}
