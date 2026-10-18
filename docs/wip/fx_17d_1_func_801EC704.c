/* Draft of func_801EC704 (module fx_17d_1). Status: 100 aligned mismatches, 32 ignoring registers and spill offsets
 * (spnorm 0x10). Permuter find: dw initialized through m (m = w >> 2; dw = m;), which changes the flag's allocation. Fixed since the 146-row draft: xs = dw * i and xa = xs + dw per iteration (the original multiplies
 * them by 3 each time; accumulating xa/xs made loop.c reduce xa * 3), k = i < 3 then i++, and the r0 word store after
 * clut (the q giv is then based at the r0 word, as in the original). Open: the original keeps the j < 3 flag
 * (slti/xori/subu) inside the inner loop where loop.c hoists it here; its q+4 giv starts at q+4 and is incremented at
 * the bottom of the loop (here at q-36, incremented at the top); prologue scheduling of w >> 1 / h >> 2.
 * Earlier: branchless flags uu - (k ^ 1), q = p++ at the top. */
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
    m = w >> 2;
    dw = m;
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
        for (i = 0; i < 4;) {
            col[0] = col[1] = col[2] = c;
            if (abr != 0xFF) {
                col[3] = 0x2E;
            } else {
                col[3] = 0x2C;
            }
            q = p++;
            xs = dw * i;
            xa = xs + dw;
            ta = xa * 3;
            ts = xs * 3;
            k = i < 3;
            i++;
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
            q->clut = clut;
            *(u32 *)&q->r0 = *(u32 *)col;
            ex->btlfx_addPrim(0x09000000, ot, q);
        }
        v += dh;
        y0 += dh;
    }
}
