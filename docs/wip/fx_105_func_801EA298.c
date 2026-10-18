/* Draft of func_801EA298 (module fx_105). Status: 128/271 aligned instructions with ccprobe; the original hoists the tpage constant 0xB8 into s6 before the second loop; ours stores it from a temporary.
 * Declarations it needs that are not yet in include/fxmod/fx_105.h are repeated below. */
#include "fxmod/fx_105.h"

void func_801EA298(s16 *data, s32 c, s32 h, s32 n) {
    Unk800F78A4 *env = D_801EAB18;
    BtlfxExports *ex = BTLFX_EXPORTS(env);
    SVECTOR v;
    SVECTOR rot;
    SVECTOR cen;
    SVECTOR ang;
    SVECTOR a;
    SVECTOR b;
    MATRIX m;
    POLY_FT4 *prim;
    s32 r;
    s32 tp;
    s32 i;

    a.vx = a.vz = b.vx = b.vz = 0;
    a.vy = -h;
    b.vy = h;
    ex->btlfx_aimAngles(&env->unk2C->unk0[0].vx, &env->unk2C->unk0[1].vx, &ang.vx);
    i = 0;
    cen.vy = env->unk2C->unk0[0].vy - ((ex->math_sin(ang.vx) * 0x800) >> 12);
    data += 2;
    r = (ex->math_cos(ang.vx) * 0x800) >> 12;
    if (r < 0) {
        r = -r;
    }
    cen.vx = env->unk2C->unk0[0].vx + ((ex->math_sin(ang.vy) * r) >> 12);
    cen.vz = env->unk2C->unk0[0].vz + ((ex->math_cos(ang.vy) * r) >> 12);
    for (; i < 0x11; i++) {
        v.vx = cen.vx - *data++;
        v.vy = *data++ + cen.vy;
        v.vz = cen.vz - *data++;
        rot.vx = *data++ + ang.vx;
        rot.vy = *data++ + ang.vy;
        rot.vz = *data++ + ang.vz;
        gte_SetRotMatrix(env->unk14);
        gte_SetTransMatrix(env->unk14);
        gte_ldv0(&v);
        gte_rtv0tr();
        gte_stlvnl(m.t);
        ex->func_800316D8(&rot, &m);
        ex->MulRotMatrix(&m);
        gte_SetRotMatrix(&m);
        gte_SetTransMatrix(&m);
        gte_ldv0(&a);
        gte_rtps();
        gte_stsxy(&D_801EC2E8[i].xy0);
        gte_ldv0(&b);
        gte_rtps();
        gte_stsxy(&D_801EC2E8[i].xy1);
    }
    if (n != 0) {
        prim = (POLY_FT4 *)env->unk24;
        env->unk24 = (u8 *)(prim + 16);
        for (i = 0, tp = 0xB8; i < n; prim++) {
            prim->clut = 0x3D80;
            prim->tpage = tp;
            prim->b0 = c;
            prim->g0 = c;
            prim->r0 = c;
            prim->code = 0x2E;
            prim->x0 = D_801EC2E8[i].xy0.vx;
            prim->y0 = D_801EC2E8[i].xy0.vy;
            prim->x1 = D_801EC2E8[i].xy1.vx;
            prim->y1 = D_801EC2E8[i].xy1.vy;
            prim->x2 = D_801EC2E8[i + 1].xy0.vx;
            prim->y2 = D_801EC2E8[i + 1].xy0.vy;
            prim->x3 = D_801EC2E8[i + 1].xy1.vx;
            prim->y3 = D_801EC2E8[i + 1].xy1.vy;
            prim->u2 = 0xFF;
            prim->u0 = 0xFF;
            prim->u3 = 0xC0;
            prim->u1 = 0xC0;
            prim->v0 = prim->v1 = ex->btlfx_lerpRatio(0, 0xFF, i, n);
            i++;
            prim->v2 = prim->v3 = ex->btlfx_lerpRatio(0, 0xFF, i, n);
            ex->btlfx_addPrim(0x09000000, 0, prim);
        }
    }
}
