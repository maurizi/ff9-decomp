/* Draft of func_801E9F28 (module fx_105). Status: 128/216 aligned instructions with ccprobe; the original compares abr == 0xFF without folding the constant 1 into a3.
 * Declarations it needs that are not yet in include/fxmod/fx_105.h are repeated below. */
#include "fxmod/fx_105.h"

void func_801E9F28(s32 c) {
    Unk800F78A4 *env = D_801EAB18;
    BtlfxExports *ex = BTLFX_EXPORTS(env);
    MATRIX m;
    SVECTOR v[4];
    SVECTOR rot;
    u8 uv[4][8];
    s32 otz;
    POLY_FT4 *p;
    s32 i;
    s32 w;
    s32 abr;
    s32 tp;
    s32 code;

    uv[1][0] = uv[3][0] = 0x4F;
    uv[0][1] = uv[1][1] = 0xA0;
    uv[0][0] = uv[2][0] = 0;
    uv[2][1] = uv[3][1] = 0xEF;
    gte_SetRotMatrix(env->unk14);
    gte_SetTransMatrix(env->unk14);
    gte_ldv0(&env->unk2C->unk0[2]);
    p = (POLY_FT4 *)env->unk24;
    tp = 0x3A;
    env->unk24 = (u8 *)(p + 32);
    gte_rtv0tr_b();
    rot.vx = 0;
    rot.vy = 0;
    rot.vz = 0;
    ex->func_800316D8(&rot, &m);
    gte_stlvnl(m.t);
    ex->MulRotMatrix(&m);
    gte_SetRotMatrix(&m);
    gte_SetTransMatrix(&m);
    w = -0x5E8;
    for (i = 0; i < 32; i++, w += 0x10) {
        v[0].vx = v[2].vx = D_801EC168[i].vx - 0x400;
        v[1].vx = v[3].vx = D_801EC168[i].vx;
        v[0].vz = v[1].vz = D_801EC168[i].vz + w;
        v[2].vz = v[3].vz = D_801EC168[i].vz - w;
        v[0].vy = v[2].vy = D_801EC168[i].vy - 0x600;
        v[1].vy = v[3].vy = D_801EC168[i].vy;
        gte_ldv3c(v);
        gte_rtpt();
        abr = 1;
        code = 0x2E;
        if (abr == 0xFF) {
            code = 0x2C;
        }
        p->code = code;
        p->r0 = p->g0 = p->b0 = (c * 0x70) >> 12;
        gte_stsxy3(&p->x0, &p->x1, &p->x2);
        gte_avsz3();
        *(u16 *)&p->u0 = *(u16 *)uv[0];
        *(u16 *)&p->u1 = *(u16 *)uv[1];
        *(u16 *)&p->u2 = *(u16 *)uv[2];
        *(u16 *)&p->u3 = *(u16 *)uv[3];
        gte_stszotz(&otz);
        gte_ldv0(&v[3]);
        gte_rtps();
        p->clut = 0x3D00;
        p->tpage = tp;
        gte_stsxy(&p->x3);
        ex->btlfx_addPrim(0x09000000, otz >> 1, p);
        p++;
        D_801EC168[i].vz -= (D_801EC168[i].vx + 0x3000) >> 6;
        if (D_801EC168[i].vz < env->unk2C->unk0[2].vz - 0x1000) {
            D_801EC168[i].vz = 0x2000;
        }
    }
}
