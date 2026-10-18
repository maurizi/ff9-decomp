/* Draft of func_801EC704 (module fx_17d_1). Status: 84 aligned mismatches (sdiff --count; registers + shape).
 * Found: the original uses branchless flags uu - (k ^ 1) with k = i++ < 3 computed mid-body, q = p++ at the top,
 * and does NOT strength-reduce xa * 3 / xs * 3 (here loop.c makes givs of them) nor hoist the j < 3 flag out of
 * the inner loop. Its caller-saves (t0-t6, a3 around addPrim) also differ. *
 * Later finding: loop.c's move/reduce heuristic (lifetime * threshold * benefit < insn_count) is what keeps the
 * original from reducing xa * 3 / xs * 3 and hoisting the j < 3 flag: four dead statements in the inner loop
 * (dd = xa * 7 + xs; ...) stop the xa * 3 reduction here (84 -> 75 rows ignoring registers), so the original inner
 * loop is larger at loop time than this one (PsyQ set* macros? extra temporaries?). Its giv for the q fields is
 * based at q + 4 (the r0 word), ours at q - 26. */
#include "fxmod/fx_17d_1.h"

void func_801EC704(s32 ot, s32 x, s32 y, s32 w, s32 h, s32 c, s32 tpage, s32 clut, s32 u, s32 v, s32 abr) {
    POLY_FT4 *p;
    BtlfxExports *ex;
    u8 col[4];
    s32 x0;
    s32 y0;
    s32 y1;
    s32 dw;
    s32 dh;
    s32 v1;
    s32 abrBits;
    s32 uu;
    s32 xa;
    s32 xs;
    s32 i;
    s32 j;
    s32 k;
    s32 m;
    s32 ta;
    s32 ts;
    POLY_FT4 *q;

    x0 = x - (w >> 1) * 3 / 2;
    dw = w >> 2;
    dh = h >> 2;
    abrBits = (abr & 3) << 5;
    y0 = y - (h >> 1);
    p = (POLY_FT4 *)D_801ED420->unk24;
    ex = BTLFX_EXPORTS(D_801ED420);
    D_801ED420->unk24 = (u8 *)(p + 16);
    for (j = 0; j < 4; j++) {
        y1 = y0 + dh;
        v1 = v + dh;
        uu = u;
        xa = dw;
        xs = 0;
        for (i = 0; i < 4;) {
            col[0] = col[1] = col[2] = c;
            if (abr != 0xFF) {
                col[3] = 0x2E;
            } else {
                col[3] = 0x2C;
            }
            q = p++;
            ta = xa * 3;
            xa += dw;
            ts = xs * 3;
            xs += dw;
            k = i++ < 3;
            q->x0 = q->x2 = x0 + ts / 2;
            q->u0 = q->u2 = uu;
            uu += dw;
            q->u1 = q->u3 = uu - (k ^ 1);
            m = j < 3;
            q->v2 = q->v3 = v1 - (m ^ 1);
            q->x1 = q->x3 = x0 + ta / 2;
            q->v0 = q->v1 = v;
            q->y0 = q->y1 = y0;
            q->y2 = q->y3 = y1;
            tpage |= abrBits;
            q->tpage = tpage;
            *(u32 *)&q->r0 = *(u32 *)col;
            q->clut = clut;
            ex->btlfx_addPrim(0x09000000, ot, q);
        }
        v += dh;
        y0 += dh;
    }
}
