/* Draft of func_801EC704 (module fx_17d_1). Status: 11/145 aligned instructions with ccprobe; register allocation only.
 * Declarations it needs that are not yet in include/fxmod/fx_17d_1.h are repeated below. */
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
        for (i = 0; i < 4; i++) {
            col[0] = col[1] = col[2] = c;
            if (abr != 0xFF) {
                col[3] = 0x2E;
            } else {
                col[3] = 0x2C;
            }
            p->u0 = p->u2 = uu;
            uu += dw;
            p->x0 = p->x2 = x0 + xs * 3 / 2;
            p->v2 = p->v3 = v1 - (j >= 3);
            p->u1 = p->u3 = uu - (i >= 3);
            p->v0 = p->v1 = v;
            p->x1 = p->x3 = x0 + xa * 3 / 2;
            p->y0 = p->y1 = y0;
            p->y2 = p->y3 = y1;
            tpage |= abrBits;
            p->tpage = tpage;
            *(u32 *)&p->r0 = *(u32 *)col;
            p->clut = clut;
            ex->btlfx_addPrim(0x09000000, ot, p);
            p++;
            xa += dw;
            xs += dw;
        }
        v += dh;
        y0 += dh;
    }
}
