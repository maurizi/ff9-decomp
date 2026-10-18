/* Draft of func_801E7910 (module fx_0e0). Status: 70/127 aligned instructions with ccprobe; scheduling of the second table load against the loop increment (same body in fx_134 func_801E7940).
 * Declarations it needs that are not yet in include/fxmod/fx_0e0.h are repeated below. */
#include "fxmod/fx_0e0.h"

void func_801E7910(s32 ot, s32 x, s32 y, s32 w, s32 h, s32 c, s32 tpage, s32 clut, s32 u, s32 v, s32 abr, s32 flip) {
    s32 dx;
    s32 w2;
    POLY_FT4 *p;
    BtlfxExports *ex;
    u8 col[4];
    s32 n;
    s32 i;
    s32 u1;
    s32 v1;
    s32 y1;

    n = 2;
    ex = BTLFX_EXPORTS(D_801EA918);
    if (flip != 0) {
        n = 4;
    }
    p = (POLY_FT4 *)D_801EA918->unk24;
    D_801EA918->unk24 = (u8 *)(p + n);
    col[0] = col[1] = col[2] = c;
    if (abr != 0xFF) {
        col[3] = 0x2E;
    } else {
        col[3] = 0x2C;
    }
    tpage |= (abr & 3) << 5;
    dx = -0x140;
    if (flip != 0) {
        dx = 0xA0;
    }
    w2 = 0x140;
    if (flip != 0) {
        w2 = 0xA0;
    }
    for (i = 0; i < n; i++) {
        p->x0 = p->x2 = x;
        p->y0 = p->y1 = y;
        p->y2 = p->y3 = y + h;
        p->x1 = p->x3 = x + w2;
        if (flip != 0) {
            p->u0 = p->u2 = u;
            p->u1 = p->u3 = u + w - 1;
        } else {
            p->u0 = p->u2 = u + w - 1;
            p->u1 = p->u3 = u;
        }
        p->v0 = p->v1 = v;
        p->v2 = p->v3 = v + h - 1;
        p->tpage = tpage;
        p->clut = clut;
        *(u32 *)&p->r0 = *(u32 *)col;
        ex->btlfx_addPrim(0x09000000, ot, p);
        p++;
        x += dx;
    }
}
