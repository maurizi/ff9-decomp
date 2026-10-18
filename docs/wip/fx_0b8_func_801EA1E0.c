/* Draft of fx_0b8 func_801EA1E0. Status: 14 aligned mismatches, prologue scheduling only: the original computes
 * x | ~0xFF (x is a byte; no andi) and ot * 4 later/earlier than ours, and stores the uv zeros after the 0xFF ones.
 * Statement order of the prologue makes no difference (all 120 orders tried). Equivalent by construction. */
#include "fxmod/fx_0b8.h"

void func_801EA1E0(s32 ot, s32 y, s32 h, s32 x, s32 c0, s32 c1, s32 tpage, s32 clut, s32 abr) {
    Fx0b8UV uv0;
    Fx0b8UV uv1;
    Fx0b8UV uv2;
    Fx0b8UV uv3;
    BtlfxExports *ex;
    Unk800F78A4 *env;
    POLY_GT4 *gt4;
    POLY_F4 *f4;
    s32 i;

    env = D_801EA7A0;
    ex = BTLFX_EXPORTS(env);
    uv1.u = uv3.u = 0xFF;
    uv0.v = uv1.v = 0x80;
    uv0.u = uv2.u = 0;
    uv2.v = uv3.v = 0xFF;
    tpage |= (abr & 3) << 5;
    x |= ~0xFF;
    gt4 = (POLY_GT4 *)env->unk24;
    f4 = (POLY_F4 *)(gt4 + 3);
    env->unk24 = (u8 *)(f4 + 3);
    for (i = 0; i < 3; gt4++, i++, f4++) {
        gt4->clut = clut;
        gt4->tpage = tpage;
        gt4->code = (abr != 0xFF) ? 0x3E : 0x3C;
        f4->code = (abr != 0xFF) ? 0x2A : 0x28;
        gt4->r0 = gt4->r1 = gt4->g0 = gt4->g1 = gt4->b0 = gt4->b1 = c0;
        f4->r0 = f4->g0 = f4->b0 = gt4->r2 = gt4->r3 = gt4->g2 = gt4->g3 = gt4->b2 = gt4->b3 = c1;
        *(u16 *)&gt4->u0 = *(u16 *)&uv0;
        *(u16 *)&gt4->u1 = *(u16 *)&uv1;
        *(u16 *)&gt4->u2 = *(u16 *)&uv2;
        *(u16 *)&gt4->u3 = *(u16 *)&uv3;
        f4->x0 = f4->x2 = gt4->x0 = gt4->x2 = x;
        f4->x1 = f4->x3 = gt4->x1 = gt4->x3 = gt4->x0 + 0x100;
        gt4->y0 = gt4->y1 = y;
        f4->y0 = f4->y1 = gt4->y2 = gt4->y3 = y + h;
        f4->y2 = f4->y3 = gt4->y2 + 0x130;
        x += 0x100;
        ex->btlfx_addPrim(0x0C000000, ot * 4, gt4);
        ex->btlfx_addPrim(0x05000000, ot * 4, f4);
    }
}
