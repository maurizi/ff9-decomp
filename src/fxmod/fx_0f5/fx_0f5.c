#include "fxmod/fx_0f5.h"

void func_801E7704(SVECTOR *pos, SVECTOR *rot, VECTOR *scale, s32 r0, s32 r1, s32 c, s32 tpage, s32 clut, s32 u, s32 v,
                   s32 dr, s32 ang, s32 dang, s32 dseg, s32 abr, s32 flicker) {
    Unk800F78A4 *env = D_801E8A74;
    BtlfxExports *ex = BTLFX_EXPORTS(env);
    CVECTOR col0;
    CVECTOR col1;
    CVECTOR col2;
    MATRIX m;
    SVECTOR p[4];
    s32 otz;
    POLY_GT4 *gt4;
    POLY_FT4 *ft4;
    s32 ra;
    s32 rb;
    s32 j;
    s32 ang2;
    s32 rin;
    s32 i1;
    s32 z1;
    s32 z0;
    s32 i;
    s32 seg;
    s32 rout;
    s32 cx;
    s32 cy;
    s32 k;
    s32 kj;
    s32 u0;
    s32 u1;
    s32 v0;
    s32 v1;
    s32 ft;
    s32 sn;

    gte_SetRotMatrix(env->unk14);
    gte_SetTransMatrix(env->unk14);
    gte_ldv0(pos);
    gt4 = (POLY_GT4 *)env->unk24;
    ft4 = (POLY_FT4 *)(gt4 + 32);
    env->unk24 = (u8 *)(ft4 + 96);
    if (flicker == 0) {
        col0.r = col0.g = col0.b = c;
        col1.r = col1.g = col1.b = c;
    } else {
        D_801E8A28 &= 0x1F;
        ex->func_800E651C(D_801E89F8, &col0, D_801E8A28);
        col0.r = (col0.r * c) >> 7;
        col0.g = (col0.g * c) >> 7;
        col0.b = (col0.b * c) >> 7;
        ex->func_800E651C(D_801E89F8, &col1, D_801E8A28);
        D_801E8A28++;
        col1.r = (col1.r * c) >> 7;
        col1.g = (col1.g * c) >> 7;
        col1.b = (col1.b * c) >> 7;
    }
    *(s32 *)&col2 = 0;
    col0.cd = abr != 0xFF ? 0x2E : 0x2C;
    col2.cd = abr != 0xFF ? 0x3E : 0x3C;
    tpage |= (abr & 3) << 5;
    col1.cd = col2.cd;
    gte_rtv0tr_b();
    ex->func_800316D8(rot, &m);
    ex->ScaleMatrix(&m, scale);
    gte_stlvnl(m.t);
    ex->MulRotMatrix(&m);
    gte_SetRotMatrix(&m);
    gte_SetTransMatrix(&m);
    seg = 0;
    i = 0;
    rout = dr;
    rin = 0;
    i1 = 1;
    z1 = 0x90;
    z0 = 0;
    for (; i < 8; i++, rin += dr, rout += dr, i1++, z1 += 0x90, z0 += 0x90) {
        seg += dseg;
        ang += dang;
        ra = ex->btlfx_lerp(r0, r1, i << 9);
        rb = ex->btlfx_lerp(r0, r1, i1 << 9);
        ang2 = ang + dang;
        for (j = 0; j < 16; j++) {
            seg += dseg;
            cx = (ex->math_cos(ang2) * (rout / 8)) >> 12;
            sn = ex->math_sin(ang2);
            /* The loop notes stop cse1 from reusing the division above; cse2 merges the second one later, which
             * leaves the original's dead reload of rin in the inner ring below. */
            do {
            } while (0);
            cy = (sn * (rout / 8)) >> 12;
            p[0].vx = cx + ((ex->math_cos(seg) * rb) >> 12);
            p[0].vy = cy + ((ex->math_sin(seg) * rb) >> 12);
            p[1].vx = cx + ((ex->math_cos(seg + 0x100) * rb) >> 12);
            p[1].vy = cy + ((ex->math_sin(seg + 0x100) * rb) >> 12);
            p[0].vz = p[1].vz = z1;
            seg -= dseg;
            cx = (ex->math_cos(ang) * (rin / 8)) >> 12;
            sn = ex->math_sin(ang);
            do {
            } while (0);
            cy = (sn * (rin / 8)) >> 12;
            p[2].vx = cx + ((ex->math_cos(seg) * ra) >> 12);
            p[2].vy = cy + ((ex->math_sin(seg) * ra) >> 12);
            p[3].vx = cx + ((ex->math_cos(seg + 0x100) * ra) >> 12);
            p[3].vy = cy + ((ex->math_sin(seg + 0x100) * ra) >> 12);
            p[2].vz = p[3].vz = z0;
            gte_ldv3c(p);
            gte_rtpt();
            k = i & 3;
            kj = j & 3;
            u0 = u + (kj << 5);
            v0 = v + (k << 5);
            u1 = u0 + 0x20;
            if (kj == 3) {
                u1 = u0 + 0x1F;
            }
            v1 = v0 + 0x20;
            if (k == 3) {
                v1 = v0 + 0x1F;
            }
            gte_nclip();
            if (i == 0) {
                *(s32 *)&gt4->r0 = *(s32 *)&gt4->r1 = *(s32 *)&col1;
                ft = 0;
                *(s32 *)&gt4->r2 = *(s32 *)&gt4->r3 = *(s32 *)&col2;
            } else {
                ft = 1;
                if (i >= 7) {
                    *(s32 *)&gt4->r2 = *(s32 *)&gt4->r3 = *(s32 *)&col1;
                    ft = 0;
                    *(s32 *)&gt4->r0 = *(s32 *)&gt4->r1 = *(s32 *)&col2;
                } else {
                    *(s32 *)&ft4->r0 = *(s32 *)&col0;
                }
            }
            gte_stopz(&otz);
            if (otz != 0) {
                if (ft) {
                    ft4->u0 = ft4->u2 = u0;
                    ft4->u1 = ft4->u3 = u1;
                    ft4->v2 = ft4->v3 = v0;
                    ft4->v0 = ft4->v1 = v1;
                    ft4->tpage = tpage;
                    ft4->clut = clut;
                    gte_stsxy3(&ft4->x0, &ft4->x1, &ft4->x2);
                } else {
                    gt4->u0 = gt4->u2 = u0;
                    gt4->u1 = gt4->u3 = u1;
                    gt4->v2 = gt4->v3 = v0;
                    gt4->v0 = gt4->v1 = v1;
                    gt4->tpage = tpage;
                    gt4->clut = clut;
                    gte_stsxy3(&gt4->x0, &gt4->x1, &gt4->x2);
                }
                gte_avsz3();
                gte_stszotz(&otz);
                gte_ldv0(&p[3]);
                gte_rtps();
                if (ft) {
                    ex->btlfx_addPrim(0x09000000, otz, ft4);
                    gte_stsxy(&ft4->x3);
                    ft4++;
                } else {
                    ex->btlfx_addPrim(0x0C000000, otz, gt4);
                    gte_stsxy(&gt4->x3);
                    gt4++;
                }
            }
            seg += 0x100;
        }
    }
    /* Dead test, removed after reload: one more reference ranks rout between i and rin in global-alloc (rout
     * gets $fp, rin is spilled). */
    if (rout != 0) {
        k = 0;
    }
}

INCLUDE_ASM("asm/us/fx_0f5/nonmatchings/fx_0f5", fx_0f5_op80);

INCLUDE_RODATA("asm/us/fx_0f5/nonmatchings/fx_0f5", fx_0f5_headerPtr);
