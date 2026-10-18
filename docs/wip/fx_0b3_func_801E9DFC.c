/* Draft of fx_0b3 func_801E9DFC (wave grid: 18x19 cells of POLY_FT4+POLY_F4, column 0 POLY_GT4+POLY_G4, with
 * edge positions written to D_801EE340). Status: 41 lines (build/w/nd.py). Blocker: the original keeps abr = 1 in
 * $a2 unfolded through both "abr != 0xFF ? 0x2A : 0x28" / "abr == 0xFF ? 0x38 : 0x3A" ternaries and
 * "tpage |= abr << 5"; here cse2 (following the path past the loop notes) folds the first ternary. With
 * "if (c != 0) abr = 1; else abr = 1;" (jump.c hoists one set) the first survives but cse's path following then
 * folds the second (build/w/gb3*.py). Other open rows: the p[].vy stores are scheduled before the first math_sin
 * call (original: after), the spill slots of the u/v halfwords, the loop tail. Fixed: D_801EE340[j] indexing
 * instead of an out pointer (the original's s5 row pointer at offset 0). */
#include "fxmod/fx_0b3.h"

void func_801E9DFC(SVECTOR *pos, SVECTOR *rot, VECTOR *scale, s32 c, s32 c2, s32 tpage, s32 clut, s32 u, s32 v,
                   s32 ay, s32 ph, s32 dph, s32 az, s32 phz, s32 dphz) {
    Unk800F78A4 *env = D_801ED5E0;
    BtlfxExports *ex = BTLFX_EXPORTS(env);
    CVECTOR col0;
    CVECTOR col1;
    CVECTOR col2;
    CVECTOR col3;
    CVECTOR col4;
    CVECTOR col5;
    MATRIX m;
    MATRIX m2;
    SVECTOR p[4];
    VECTOR lv;
    DVECTOR sxy;
    s32 opz;
    s32 otz;
    POLY_FT4 *ft4;
    POLY_G4 *g4;
    POLY_F4 *f4;
    s32 xx;
    s32 zz;
    s32 j;
    s32 y0;
    s32 y1;
    POLY_GT4 *gt4;
    s32 i;
    s32 x;
    s32 k;
    s32 k2;
    s32 u0;
    s32 u1;
    s32 v0;
    s32 v1;
    s32 ft;
    s32 abr;
    s32 x0;
    s32 z0;

    abr = 1;
    gte_SetRotMatrix(env->unk14);
    gte_SetTransMatrix(env->unk14);
    gte_ldv0(pos);
    gt4 = (POLY_GT4 *)env->unk24;
    *(s32 *)&col2 = 0;
    *(s32 *)&col5 = 0;
    ft4 = (POLY_FT4 *)(gt4 + 18);
    g4 = (POLY_G4 *)(ft4 + 324);
    f4 = (POLY_F4 *)(g4 + 18);
    env->unk24 = (u8 *)(f4 + 324);
    col2.cd = 0x3E;
    col0.cd = 0x2E;
    col0.r = col0.g = col0.b = c;
    col1.r = col1.g = col1.b = c;
    col3.r = col3.g = col3.b = c2;
    col4.r = col4.g = col4.b = c2;
    col1.cd = 0x3E;
    do {
    } while (0);
    col3.cd = abr != 0xFF ? 0x2A : 0x28;
    col5.cd = abr == 0xFF ? 0x38 : 0x3A;
    tpage |= abr << 5;
    col4.cd = col5.cd;
    gte_rtv0tr_b();
    ex->func_800316D8(rot, &m);
    ex->ScaleMatrix(&m, scale);
    m2 = m;
    m2.t[0] = pos->vx;
    m2.t[1] = pos->vy;
    m2.t[2] = pos->vz;
    gte_stlvnl(m.t);
    ex->MulRotMatrix(&m);
    gte_SetRotMatrix(&m);
    gte_SetTransMatrix(&m);
    x0 = -0x24EA;
    z0 = -0x17CD;
    zz = 0;
    for (j = 0; j < 18; j++, zz += 0x2A5) {
        ph += dph;
        y0 = z0 + zz;
        y1 = y0 + 0x2A5;
        xx = 0;
        for (i = 0; i < 19; i++, xx += 0x41A) {
            x = x0 + xx;
            p[0].vx = x + ((ex->math_sin(ph) * ay) >> 12);
            p[1].vx = p[0].vx + 0x41A;
            p[0].vy = p[1].vy = y0;
            p[2].vy = p[3].vy = y1;
            x += (ex->math_sin(ph + dph) * ay) >> 12;
            p[2].vx = x;
            x += 0x41A;
            p[3].vx = x;
            p[0].vz = p[2].vz = (func_801E9DB8(phz + i * dphz) * az) >> 12;
            p[1].vz = p[3].vz = (func_801E9DB8(phz + (i + 1) * dphz) * az) >> 12;
            gte_ldv3c(p);
            gte_rtpt();
            k = i & 1;
            u0 = u + (k << 6);
            u1 = u0 + 0x40;
            if (k != 0) {
                u1 = u0 + 0x3F;
            }
            k2 = j & 1;
            v0 = v + (k2 << 6);
            v1 = v0 + 0x40;
            if (k2 != 0) {
                v1 = v0 + 0x3F;
            }
            gte_nclip();
            gte_stopz(&opz);
            if (opz != 0) {
                gte_avsz3();
                gte_stszotz(&otz);
                ft = 1;
                if (i == 0) {
                    *(s32 *)&gt4->r0 = *(s32 *)&gt4->r2 = *(s32 *)&col2;
                    *(s32 *)&gt4->r1 = *(s32 *)&gt4->r3 = *(s32 *)&col1;
                    *(s32 *)&g4->r0 = *(s32 *)&g4->r2 = *(s32 *)&col5;
                    ft = 0;
                    *(s32 *)&g4->r1 = *(s32 *)&g4->r3 = *(s32 *)&col4;
                } else {
                    *(s32 *)&ft4->r0 = *(s32 *)&col0;
                    *(s32 *)&f4->r0 = *(s32 *)&col3;
                }
                if (opz > 0) {
                    if (ft) {
                        ft4->r0 >>= 1;
                        ft4->g0 >>= 1;
                    } else {
                        gt4->r0 >>= 1;
                        gt4->g0 >>= 1;
                        gt4->r1 >>= 1;
                        gt4->g1 >>= 1;
                        gt4->r2 >>= 1;
                        gt4->g2 >>= 1;
                        gt4->r3 >>= 1;
                        gt4->g3 >>= 1;
                    }
                }
                if (ft) {
                    ft4->u0 = ft4->u2 = u0;
                    ft4->u1 = ft4->u3 = u1;
                    ft4->v0 = ft4->v1 = v0;
                    ft4->v2 = ft4->v3 = v1;
                    ft4->tpage = tpage;
                    ft4->clut = clut;
                    gte_stsxy3(&ft4->x0, &ft4->x1, &ft4->x2);
                    *(s32 *)&f4->x0 = *(s32 *)&ft4->x0;
                    *(s32 *)&f4->x1 = *(s32 *)&ft4->x1;
                    *(s32 *)&f4->x2 = *(s32 *)&ft4->x2;
                } else {
                    gt4->u0 = gt4->u2 = u0;
                    gt4->u1 = gt4->u3 = u1;
                    gt4->v0 = gt4->v1 = v0;
                    gt4->v2 = gt4->v3 = v1;
                    gt4->tpage = tpage;
                    gt4->clut = clut;
                    gte_stsxy3(&gt4->x0, &gt4->x1, &gt4->x2);
                    *(s32 *)&g4->x0 = *(s32 *)&gt4->x0;
                    *(s32 *)&g4->x1 = *(s32 *)&gt4->x1;
                    *(s32 *)&g4->x2 = *(s32 *)&gt4->x2;
                    gte_SetRotMatrix(&m2);
                    gte_SetTransMatrix(&m2);
                    gte_ldv0(&p[0]);
                    gte_rtv0tr();
                    gte_stlvnl(&lv);
                    D_801EE340[j].vx = lv.vx;
                    D_801EE340[j].vy = lv.vy;
                    D_801EE340[j].vz = lv.vz;
                    gte_SetRotMatrix(&m);
                    gte_SetTransMatrix(&m);
                    gte_ldv0(&p[0]);
                    gte_rtps();
                    gte_stsxy(&sxy);
                    if ((u16)(sxy.vx + 0x20) > 0x180 || sxy.vy < -0x20 || sxy.vy > 0x110) {
                        D_801EE340[j].pad = 0;
                    } else {
                        D_801EE340[j].pad = 1;
                    }
                }
                gte_ldv0(&p[3]);
                gte_rtps();
                if (ft) {
                    ex->btlfx_addPrim(0x09000000, otz, ft4);
                    ex->btlfx_addPrimAbr(0x05000000, otz, f4, 2);
                    gte_stsxy(&ft4->x3);
                    *(s32 *)&f4->x3 = *(s32 *)&ft4->x3;
                    ft4++;
                    f4++;
                } else {
                    ex->btlfx_addPrim(0x0C000000, otz, gt4);
                    ex->btlfx_addPrimAbr(0x08000000, otz, g4, 2);
                    gte_stsxy(&gt4->x3);
                    *(s32 *)&g4->x3 = *(s32 *)&gt4->x3;
                    gt4++;
                    g4++;
                }
            }
        }
    }
}
