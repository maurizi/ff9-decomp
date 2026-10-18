/* Draft of func_801E78FC (module fx_0b2: draws a ring of POLY_GT4/POLY_FT4 segments with the GTE). The prologue
 * through the matrix setup matches; the loops differ in which loop invariants gcc hoists and in register
 * allocation (the original spills row and keeps the FT4 pointer in fp; frame 0xD0 vs 0xD8 here).
 * tools/permute.py handles the gte_* macros (late defines). */
#include "fxmod/fx_0b2.h"

typedef struct {
    u8 r;
    u8 g;
    u8 b;
    u8 code;
} Fx0b2Rgbc;

void func_801E78FC(SVECTOR *arg0, SVECTOR *arg1, VECTOR *arg2, s32 arg3, s32 arg4, s32 arg5, s32 arg6, s32 arg7, s32 arg8,
                   s32 arg9, s32 arg10, s32 arg11, s32 arg12, s32 arg13, s32 arg14, s32 arg15) {
    BtlfxExports *ex = BTLFX_EXPORTS(D_801E90A0);
    Fx0b2Rgbc c0;
    Fx0b2Rgbc c1;
    Fx0b2Rgbc c2;
    MATRIX m;
    SVECTOR v[4];
    s32 z;
    u8 *gt4;
    u8 *ft4;
    s32 r0;
    s32 r1;
    s32 j;
    s32 row;
    s32 rad0;
    s32 rad1;
    s32 rowNext;
    s32 h0;
    s32 h1;
    s32 ang;
    s32 a;
    s32 x;
    s32 y;
    s32 d;
    s32 ft;
    u8 *q4;
    u8 *q3;
    s32 u0;
    s32 v0;
    s32 u1;
    s32 v1;

    gte_SetRotMatrix(D_801E90A0->unk14);
    gte_SetTransMatrix(D_801E90A0->unk14);
    gte_ldv0(arg0);
    gt4 = D_801E90A0->unk24;
    *(s32 *)&c2 = 0;
    ft4 = gt4 + arg15 * 0x68;
    D_801E90A0->unk24 = ft4 + arg15 * 0xC8;
    c0.r = c0.g = c0.b = arg5;
    c1.r = c1.g = c1.b = arg5;
    c0.code = arg14 != 0xFF ? 0x2E : 0x2C;
    c2.code = arg14 != 0xFF ? 0x3E : 0x3C;
    arg6 |= (arg14 & 3) << 5;
    c1.code = c2.code;
    gte_rtv0tr_b();
    ex->func_800316D8(arg1, &m);
    ex->ScaleMatrix(&m, arg2);
    gte_stlvnl(m.t);
    ex->MulRotMatrix(&m);
    gte_SetRotMatrix(&m);
    gte_SetTransMatrix(&m);
    ang = 0;
    rad0 = 0;
    rad1 = arg10;
    rowNext = 1;
    h1 = 0;
    h0 = 0x90;
    for (row = 0; row < 7; row++) {
        ang += arg13;
        arg11 += arg12;
        r0 = ex->btlfx_lerp(arg3, arg4, (row << 12) / 7);
        r1 = ex->btlfx_lerp(arg3, arg4, (rowNext << 12) / 7);
        q3 = ft4 + 0x20;
        q4 = gt4 + 0x2C;
        for (j = 0; j < arg15; j++) {
            {
                ang += arg13;
                x = (ex->math_cos(arg11 + arg12) * (rad1 / 7)) >> 12;
                y = (ex->math_sin(arg11 + arg12) * (rad1 / 7)) >> 12;
                v[0].vx = x + ((ex->math_cos(ang) * r1) >> 12);
                d = 0x1000 / arg15;
                v[0].vy = y + ((ex->math_sin(ang) * r1) >> 12);
                a = ang + d;
                v[1].vx = x + ((ex->math_cos(a) * r1) >> 12);
                v[1].vy = y + ((ex->math_sin(a) * r1) >> 12);
                v[0].vz = v[1].vz = h0;
                ang -= arg13;
                d = ang + d;
                x = (ex->math_cos(arg11) * (rad0 / 7)) >> 12;
                y = (ex->math_sin(arg11) * (rad0 / 7)) >> 12;
                v[2].vx = x + ((ex->math_cos(ang) * r0) >> 12);
                v[2].vy = y + ((ex->math_sin(ang) * r0) >> 12);
                v[3].vx = x + ((ex->math_cos(d) * r0) >> 12);
                v[3].vy = y + ((ex->math_sin(d) * r0) >> 12);
                v[2].vz = v[3].vz = h1;
                gte_ldv3c(v);
                gte_rtpt();
                u0 = arg8 + ((j & 3) * 7 * 16 >> 2);
                v0 = arg9 + ((row & 3) * 7 * 16 >> 2);
                u1 = (j & 3) == 3 ? u0 + 0x1B : u0 + 0x1C;
                v1 = (row & 3) == 3 ? v0 + 0x1B : v0 + 0x1C;
                gte_nclip();
                if (row == 0) {
                    ft = 0;
                    ((s32 *)q4)[-7] = ((s32 *)q4)[-10] = *(s32 *)&c1;
                    ((s32 *)q4)[-1] = ((s32 *)q4)[-4] = *(s32 *)&c2;
                } else if (row >= 6) {
                    ft = 0;
                    ((s32 *)q4)[-1] = ((s32 *)q4)[-4] = *(s32 *)&c1;
                    ((s32 *)q4)[-7] = ((s32 *)q4)[-10] = *(s32 *)&c2;
                } else {
                    ft = 1;
                    ((s32 *)q3)[-7] = *(s32 *)&c0;
                }
                gte_stopz(&z);
                if (z != 0) {
                    if (ft) {
                        q3[-4] = q3[-0x14] = u0;
                        q3[4] = q3[-0xC] = u1;
                        q3[5] = q3[-3] = v0;
                        q3[-0xB] = q3[-0x13] = v1;
                        ((u16 *)q3)[-5] = arg6;
                        ((u16 *)q3)[-9] = arg7;
                        gte_stsxy3(ft4 + 8, ft4 + 0x10, ft4 + 0x18);
                    } else {
                        q4[-8] = q4[-0x20] = u0;
                        q4[4] = q4[-0x14] = u1;
                        q4[5] = q4[-7] = v0;
                        q4[-0x13] = q4[-0x1F] = v1;
                        ((u16 *)q4)[-9] = arg6;
                        ((u16 *)q4)[-15] = arg7;
                        gte_stsxy3(gt4 + 8, gt4 + 0x14, gt4 + 0x20);
                    }
                    gte_avsz3();
                    gte_stszotz(&z);
                    gte_ldv0(&v[3]);
                    gte_rtps();
                    if (ft) {
                        ex->btlfx_addPrim(0x09000000, z, ft4);
                        gte_stsxy(q3);
                        q3 += 0x28;
                        ft4 += 0x28;
                    } else {
                        ex->btlfx_addPrim(0x0C000000, z, gt4);
                        gte_stsxy(q4);
                        q4 += 0x34;
                        gt4 += 0x34;
                    }
                }
                ang += 0x1000 / arg15;
            }
        }
        rad0 += arg10;
        rad1 += arg10;
        rowNext++;
        h1 += 0x90;
        h0 += 0x90;
    }
}
