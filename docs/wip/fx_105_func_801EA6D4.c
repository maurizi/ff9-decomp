/* Draft of func_801EA6D4 (module fx_105). Status: 134/237 aligned instructions with ccprobe; c (u8 parameter) and n swap: the original keeps n in fp and reloads c from the stack.
 * Declarations it needs that are not yet in include/fxmod/fx_105.h are repeated below. */
#include "fxmod/fx_105.h"

void func_801EA6D4(s16 *data, u8 c, u8 c2, s32 n) {
    Unk800F78A4 *env;
    BtlfxExports *ex;
    SVECTOR v;
    SVECTOR r;
    SVECTOR a;
    SVECTOR b;
    MATRIX m;
    POLY_FT4 *prim;
    Fx105RibbonPt *cur;
    s16 *p;
    s32 i;

    p = data + 2;
    env = D_801EAB18;
    ex = BTLFX_EXPORTS(env);
    a.vy = -0xC0;
    a.vx = a.vz = b.vx = b.vz = 0;
    b.vy = 0xC0;
    for (i = 0; i < 0x40; i++) {
        v.vx = *p++;
        v.vy = *p++;
        v.vz = *p++ + env->unkC->unk3C;
        r.vx = *p++;
        r.vy = *p++;
        r.vz = *p++;
        gte_SetRotMatrix(env->unk14);
        gte_SetTransMatrix(env->unk14);
        gte_ldv0(&v);
        gte_rtv0tr();
        gte_stlvnl(m.t);
        ex->func_800316D8(&r, &m);
        ex->MulRotMatrix(&m);
        gte_SetRotMatrix(&m);
        gte_SetTransMatrix(&m);
        gte_ldv0(&a);
        gte_rtps();
        gte_stsxy(&D_801EC378[i].xy0);
        gte_ldv0(&b);
        gte_rtps();
        gte_stsxy(&D_801EC378[i].xy1);
        gte_stszotz(&D_801EC378[i].z);
    }
    if (n != 0) {
        prim = (POLY_FT4 *)env->unk24;
        env->unk24 = (u8 *)(prim + 63);
        for (i = 0; i < n; prim++, i++) {
            cur = &D_801EC378[i];
            prim->clut = 0x3DC0;
            prim->tpage = 0xB8;
            prim->b0 = c;
            prim->g0 = c;
            prim->code = 0x2E;
            prim->r0 = c;
            prim->x0 = cur->xy0.vx;
            prim->y0 = cur->xy0.vy;
            prim->x1 = cur->xy1.vx;
            prim->y1 = cur->xy1.vy;
            prim->x2 = D_801EC378[i + 1].xy0.vx;
            prim->y2 = D_801EC378[i + 1].xy0.vy;
            prim->x3 = D_801EC378[i + 1].xy1.vx;
            prim->y3 = D_801EC378[i + 1].xy1.vy;
            prim->v2 = 0xDF;
            prim->v0 = 0xDF;
            prim->v3 = 0xA0;
            prim->v1 = 0xA0;
            prim->u0 = prim->u1 = ex->btlfx_lerpRatio(0xBF, 0, i, n);
            prim->u2 = prim->u3 = ex->btlfx_lerpRatio(0xBF, 0, i + 1, n);
            if (cur->xy0.vx < D_801EC378[i + 1].xy0.vx) {
                ex->btlfx_addPrim(0x09000000, 0, prim);
            } else {
                ex->btlfx_addPrim(0x09000000, (cur->z + D_801EC378[i + 1].z) >> 1, prim);
            }
        }
    }
}
