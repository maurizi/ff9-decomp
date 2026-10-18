/* Draft of fx_195 fx_195_op80. Status: 231 aligned mismatches (about 120 ignoring registers/stack slots).
 * Structure complete; the stack layout matches (case 4's structs at function scope, case 5's first SVECTOR is
 * case 4's r3). What got it here: per-web variables (build/webs.py + greedy), y = x >> 12 in the tint tail (puts a
 * pseudo in t0 so reloads use t1), u = 0x1000 - q in one expression, (a >> 2) inline in the cos calls.
 * Open: register choice in several case-5 blocks (frame < 0x41 g/a/c, the 6-step loop i/c, the 23D8 pairs) and
 * the spill slot at 0x140 (unused in the original). */
#include "fxmod/fx_195.h"

typedef struct {
    s32 vx;
    s32 vy;
    s32 vz;
} Fx195Vec3;

typedef struct {
    s32 unk0; /* state */
    s32 unk4; /* counter */
    SVECTOR unk8;
    SVECTOR unk10; /* position */
    SVECTOR unk18;
    SVECTOR unk20; /* camera */
    u8 pad28[0x30 - 0x28];
    s16 unk30;
    s16 unk32;
    u8 pad34[0x44 - 0x34];
    s32 unk44;
    u8 pad48[0x54 - 0x48];
    Pool6CEntry *unk54;
    Pool6CEntry *unk58;
    s32 unk5C; /* model */
    Fx195Vec3 unk60; /* model position */
    Fx195Vec3 unk6C; /* model scale */
    SVECTOR unk78; /* model rotation */
    s32 unk80; /* models */
    s32 unk84;
    s32 unk88;
    s32 unk8C;
    s32 unk90;
    s32 unk94;
} Fx195Work80; /* 0x98 */

extern BtlModel D_801EA288[];
extern SVECTOR D_801EA228;
extern Fx195Trail D_801EA808[];
extern Fx195Trail57 D_801EAB08[];
extern Fx195Trail57 D_801EAEA0;
void func_801E78A4(Unk80100C78Entry *entry, Fx195Trail57 *trail, SVECTOR *from, SVECTOR *to, s32 arg4, s32 arg5,
                   s32 arg6, s32 arg7, s32 arg8, s32 arg9, s32 arg10);
void func_801E7AA0(Unk80100C78Entry *entry, Fx195Trail *trail, SVECTOR *from, SVECTOR *to, s32 arg4, s32 arg5, s32 arg6,
                   s32 arg7, s32 arg8, s32 arg9, s32 arg10);
void func_801E7C98(SVECTOR *pos, SVECTOR *rot, VECTOR *scale, s32 r0, s32 r1, s32 w0, s32 w1, s32 c, s32 tpage,
                   s32 clut, s32 u, s32 v, s32 ang, s32 dy, s32 abr, s32 dang);

s32 fx_195_op80(s32 mode, Fx195Work80 *work, Unk800F78A4 *env, Unk80100C78Entry *entry, s32 *arg4) {
    BtlfxExports *ex = BTLFX_EXPORTS(env);
    SVECTOR rot;
    VECTOR sc;
    CVECTOR col;
    SVECTOR rot2;
    VECTOR sc2;
    SVECTOR v1;
    SVECTOR v2;
    SVECTOR r3;
    s32 frame;
    s32 t;
    s32 tint1;
    s32 tint2;
    Pool6CEntry *p;

    if (mode == 0) {
        *arg4 = 0x98;
    } else if (mode == 1) {
        work->unk0 = 0;
        work->unk4 = 0;
        D_801EA278 = env;
        ex->btlfx_initEffModels(D_801EA288, 7);
        work->unk80 = ex->func_800E8F50((Unk800E8F50 *)ex->btlfx_getFileData(entry->unk8, 0x11), 1);
        work->unk84 = ex->func_800E8F50((Unk800E8F50 *)ex->btlfx_getFileData(entry->unk8, 0x14), 1);
        work->unk5C = ex->func_800E8F50((Unk800E8F50 *)ex->btlfx_getFileData(entry->unk8, 0x1C), 0);
        work->unk88 = ex->func_800E8F50((Unk800E8F50 *)ex->btlfx_getFileData(entry->unk8, 0x16), 1);
        work->unk8C = ex->func_800E8F50((Unk800E8F50 *)ex->btlfx_getFileData(entry->unk8, 0x15), 0);
        work->unk90 = ex->func_800E8F50((Unk800E8F50 *)ex->btlfx_getFileData(entry->unk8, 0x1A), 0);
        work->unk94 = ex->func_800E8F50((Unk800E8F50 *)ex->btlfx_getFileData(entry->unk8, 0x13), 1);
        BTLFX_COPY_SVEC(&work->unk8, &D_801EA228);
        BTLFX_COPY_SVEC(&work->unk20, &D_801EA228);
        work->unk20.vy -= 0x980;
        work->unk10.vx = D_801EA228.vx;
        work->unk10.vy = D_801EA228.vy;
        work->unk10.vz = D_801EA228.vz;
        work->unk78.vx = -0x400;
        work->unk78.vy = 0;
        work->unk78.vz = 0;
    } else {
        tint1 = 0;
        tint2 = 0;
        frame = entry->unk0;
        switch (work->unk0) {
        case 0:
            env->unk2C->unk0[3].vx = work->unk20.vx;
            env->unk2C->unk0[3].vy = work->unk20.vy;
            env->unk2C->unk0[3].vz = work->unk20.vz;
            if (frame >= 2) {
                work->unk0 = 4;
                entry->unk0 = -1;
                work->unk44 = 0;
            }
            break;
        case 4: {
            s32 k;
            s32 kk;
            s32 v;
            s32 w1;
            s32 w1_200;
            s32 w2;
            s32 w2_202;
            s32 u;
            s32 u4;
            s32 c;
            s32 a;
            s32 i;
            s32 ang;
            s32 r;

            t = (frame << 12) / 14;
            tint2 = ex->math_cos(t >> 2);
            if (frame == 0) {
                work->unk10.vx = D_801EA228.vx;
                work->unk10.vy = D_801EA228.vy;
                work->unk10.vz = D_801EA228.vz;
                work->unk60.vx = D_801EA228.vx;
                work->unk60.vy = D_801EA228.vy;
                work->unk60.vz = D_801EA228.vz;
            }
            if (frame < 4) {
                tint1 = 0x1000 - ((frame << 12) >> 2);
                work->unk6C.vx = work->unk6C.vy = 0x1200;
                work->unk6C.vz = 0x1555;
            } else {
                kk = frame - 4;
                if (frame == 4) {
                    work->unk6C.vx = work->unk6C.vy = 0x4000;
                } else if (kk < 11) {
                    v = ((0x1000 - (kk << 12) / 10) * 3) >> 1;
                    work->unk6C.vx = work->unk6C.vy = v;
                }
            }
            if (frame < 5) {
                work->unk20.vy = work->unk10.vy;
                work->unk20.vz = work->unk10.vz;
                work->unk20.vx = work->unk10.vx;
                work->unk20.vy += -0x180 - ((0x1000 - ((frame << 12) >> 2)) >> 1);
                env->unk2C->unk0[3].vx = work->unk20.vx;
                env->unk2C->unk0[3].vy = work->unk20.vy;
                env->unk2C->unk0[3].vz = work->unk20.vz;
            }
            if (frame == 4) {
                ex->btlfx_drawScreenTint((u32 *)(env->unk20 + 2), 1, 0x20, 0x80, 0xFF);
            } else if ((u32)(frame - 5) < 7) {
                u = 0x1000 - ((frame - 5) << 12) / 7;
                r = u >> 5;
                work->unk20.vx = D_801EA228.vx + ex->btlfx_randCentered(r);
                work->unk20.vy = D_801EA228.vy + ex->btlfx_randCentered(r);
                work->unk20.vy = D_801EA228.vy - 0x200 + (u >> 6) * ((frame & 1) * 2 - 1);
                env->unk2C->unk0[3].vx = work->unk20.vx;
                env->unk2C->unk0[3].vy = work->unk20.vy;
                env->unk2C->unk0[3].vz = work->unk20.vz;
            }
            if (work->unk6C.vx != 0) {
                func_801E83BC((VECTOR *)&work->unk60, &work->unk78);
                ex->Hi_DrawEffModel(&work->unk78, (VECTOR *)&work->unk60, (s32)&work->unk6C, work->unk5C);
            }
            if (frame >= 4) {
                u4 = ((frame - 4) << 12) / 10;
                rot.vx = 0;
                rot.vz = 0;
                rot.vy = u4 >> 3;
                sc.vx = sc.vz = (u4 >> 2) + 0x555;
                sc.vy = (u4 >> 1) + 0x400;
                c = ex->math_cos(u4 >> 2);
                ex->Hi_DrawEffModel(&rot, (VECTOR *)&work->unk60, (s32)&sc, work->unk80);
                ex->Hi_ModifyEffModelRGB(work->unk80, (c >> 5) - 0x80, (c >> 5) - 0x80, (c >> 5) - 0x80);
                sc.vx = sc.vz = sc.vx * 2;
                sc.vy >>= 2;
                ex->Hi_DrawEffModel(&rot, (VECTOR *)&work->unk60, (s32)&sc, work->unk80);
                ex->Hi_ModifyEffModelRGB(work->unk80, (c >> 6) - 0x80, (c >> 6) - 0x80, (c >> 6) - 0x80);
            }
            if (frame == 0) {
                work->unk54 = ex->func_800E3CEC(ex->btlfx_getFileData(entry->unk8, 0xE), (s32)&work->unk10);
                work->unk58 = ex->func_800E3CEC(ex->btlfx_getFileData(entry->unk8, 0xC), (s32)&work->unk10);
                ex->func_800E3CEC(ex->btlfx_getFileData(entry->unk8, 0xD), (s32)&work->unk10);
            }
            col.r = col.g = col.b = 0x37;
            sc2.vx = 0x1400;
            sc2.vy = 0x1400;
            sc2.vz = 0x1000;
            rot2.vx = 0;
            rot2.vy = 0;
            rot2.vz = 0;
            ex->func_800E6764(3, 0xA0, 0x70, 0x50, 0x38, 4, 4, 2, 2, &rot2, &sc2, &col, 1, NULL);
            BTLFX_COPY_SVEC(&v1, &work->unk10);
            ex->func_800CFD78((s16 *)&v1, (s16 *)&work->unk78, t - 0x2800, (s16 *)&v2);
            r = (t >> 2) + 0x400;
            for (i = 0, ang = work->unk4 << 8; i < 3; i++, ang += 0x554) {
                func_801E78A4(entry, &D_801EAB08[i], &v1, &v2, r, r, ang, 0xC0, 0, ex->btlfx_randCentered(0x400) + 0xC00,
                              0x84);
            }
            env->unk28 = 0x200;
            w1_200 = ex->math_sin(t >> 2) << 1;
            w1_200 += 0x1000;
            w2_202 = (ex->math_cos(t >> 2) >> 1) + 0x800;
            ex->func_800D23D8((u8 *)ex->btlfx_getFileData(entry->unk8, 9), &work->unk10, NULL, w1_200, 0, 1, w2_202);
            w2 = ex->math_sin(t >> 2) * 3 + 0x1000;
            w1 = ex->math_cos(t >> 2);
            ex->func_800D23D8((u8 *)ex->btlfx_getFileData(entry->unk8, 8), &work->unk10, NULL, w2, 0, 1, w1);
            if (frame >= 4 && (k = frame - 4) < 10) {
                r3.vx = 0x400;
                r3.vy = 0;
                r3.vz = 0;
                r3.pad = 1;
                ex->func_800D23D8((u8 *)ex->btlfx_getFileData(entry->unk8, 0xA), &work->unk10, &r3, 0x4000, k, 1, 0x1000);
            }
            env->unk28 = 0;
            if (frame >= 14) {
                work->unk0 = 5;
                entry->unk0 = -1;
                ex->func_800E3D54(work->unk54);
                ex->func_800E3D54(work->unk58);
                ex->btlfx_drawScreenTint((u32 *)(env->unk20 + 1), 1, 0, 0x80, 0xFF);
                p = ex->func_800E3CEC(ex->btlfx_getFileData(entry->unk8, 0xF), (s32)&work->unk10);
                if (p != NULL) {
                    p->unk22 = 0x300;
                }
                p = ex->func_800E3CEC(ex->btlfx_getFileData(entry->unk8, 0xB), (s32)&work->unk10);
                if (p != NULL) {
                    p->unk22 = 0x300;
                }
                p = ex->func_800E3CEC(ex->btlfx_getFileData(entry->unk8, 0x17), (s32)&work->unk10);
                if (p != NULL) {
                    p->unk22 = 0x400;
                }
                work->unk30 = ex->func_800E9930(0x22, entry->unk8, -1);
                work->unk32 = ex->func_800E9930(0x23, entry->unk8, -1);
                ex->func_800E9930(0x20, entry->unk8, -1);
                ex->func_800E9930(0x21, entry->unk8, -1);
                env->unk2C->unk0[2].vx = 0;
                env->unk2C->unk0[2].vy = -0xC8;
                env->unk2C->unk0[2].vz = 0x300;
            }
            break;
        }
        case 5: {
            VECTOR sc;
            SVECTOR rotA;
            VECTOR scA;
            VECTOR posA;
            SVECTOR rotC;
            VECTOR scC;
            VECTOR posC;
            SVECTOR rotF;
            VECTOR scF;
            VECTOR posF;
            SVECTOR v1;
            SVECTOR v2;
            SVECTOR rotG;
            s32 c;
            s32 c_321;
            s32 c_359;
            s32 c_407;
            s32 v;
            s32 b;
            s32 b2;
            s32 s;
            s32 s_421;
            s32 a;
            s32 a_331;
            s32 a_340;
            s32 a_405;
            s32 d;
            s32 e;
            s32 f;
            s32 g;
            s32 g_356;
            s32 i;
            s32 k;
            s32 n;
            s32 ang;
            s32 r;
            s32 r_375;

            t = (frame << 12) / 77;
            if (frame == 0) {
                work->unk10.vx = D_801EA228.vx;
                work->unk10.vy = D_801EA228.vy;
                work->unk10.vz = D_801EA228.vz;
                work->unk60.vx = D_801EA228.vx;
                work->unk60.vy = D_801EA228.vy;
                work->unk60.vz = D_801EA228.vz;
                work->unk78.vx = -0x400;
                work->unk78.vy = 0;
                work->unk78.vz = 0;
                work->unk6C.vx = 0x4000;
                work->unk6C.vy = 0x4000;
                work->unk6C.vz = 0x4000;
            }
            work->unk20.vx = work->unk10.vx;
            work->unk20.vy = work->unk10.vy;
            work->unk20.vz = work->unk10.vz;
            work->unk20.vy = -0x2D4 - ((t * 0x76C) >> 12);
            env->unk2C->unk0[3].vx = work->unk20.vx;
            env->unk2C->unk0[3].vy = work->unk20.vy;
            env->unk2C->unk0[3].vz = work->unk20.vz;
            if (frame < 2) {
                ex->btlfx_drawScreenTint((u32 *)(env->unk20 + 2), 2, 0xFF - (frame << 7), 0xFF - (frame << 7), 0xFF);
            } else if (frame == 2) {
                ex->btlfx_drawScreenTint((u32 *)(env->unk20 + 2), 1, 0xFF, 0xFF, 0xFF);
            } else if (frame < 0x13) {
                c = ((0x1000 - (((frame - 3) << 12) >> 4)) * 0xA0) >> 12;
                ex->btlfx_drawScreenTint((u32 *)(env->unk20 + 2), 1, c, c, c);
            }
            if (frame == 0x14) {
                ex->btlfx_drawScreenTint((u32 *)(env->unk20 + 2), 1, 0xD7, 0x64, 0x64);
            }
            if (frame == 0x1D) {
                ex->btlfx_drawScreenTint((u32 *)(env->unk20 + 2), 1, 0x97, 0x4D, 0x4D);
            }
            if (frame == 0x20) {
                ex->btlfx_drawScreenTint((u32 *)(env->unk20 + 2), 1, 0x80, 0x40, 0x40);
            }
            if (frame < 0x49) {
                if (frame < 7) {
                    v = ex->math_sin((frame << 10) / 7);
                    b2 = 0x2000;
                } else {
                    v = (frame - 7) * 0x1124 / 65 * 7;
                    b2 = 0x7800;
                }
                s = b2 - v;
                if ((u32)(frame - 5) < 7) {
                    c_321 = ex->math_sin(((frame - 5) << 11) >> 12) * 0xF5;
                    ex->btlfx_drawScreenTint((u32 *)(env->unk20 + 0x400), 1, 0, c_321 >> 14, c_321 >> 12);
                }
                work->unk6C.vx = work->unk6C.vy = s;
                func_801E83BC((VECTOR *)&work->unk60, &work->unk78);
                work->unk78.vz -= 0x400;
                ex->Hi_DrawEffModel(&work->unk78, (VECTOR *)&work->unk60, (s32)&work->unk6C, work->unk88);
                ex->Hi_DrawEffModel(&work->unk78, (VECTOR *)&work->unk60, (s32)&work->unk6C, work->unk8C);
            }
            if (frame >= 0x3B) {
                a_331 = ((frame - 0x3B) << 12) / 18;
                work->unk6C.vx = work->unk6C.vy = 0x1000;
                func_801E83BC((VECTOR *)&work->unk60, &work->unk78);
                work->unk78.vz += 0x400;
                ex->Hi_DrawEffModel(&work->unk78, (VECTOR *)&work->unk60, (s32)&work->unk6C, work->unk90);
                ex->Hi_ModifyEffModelRGB(work->unk90, (ex->math_cos(a_331 >> 2) >> 5) - 0x80,
                                         (ex->math_cos(a_331 >> 2) >> 5) - 0x80, (ex->math_cos(a_331 >> 2) >> 5) - 0x80);
            }
            a_340 = t >> 2;
            b = (t >> 1) + 0xE00;
            c = ex->math_cos(a_340);
            sc.vx = 0x1000;
            sc.vy = 0x1000;
            sc.vz = 0x1000;
            e = (c >> 2) + 0x100;
            r3.vx = 0x400;
            r3.vy = 0;
            r3.vz = 0;
            d = (((t >> 3) + 0x500) * 4) / 3;
            f = ex->math_cos(a_340) >> 5;
            func_801E7C98(&work->unk10, &r3, &sc, b, a_340 + 0xA00, e >> 1, e, f, 0x18, 0x3E81, 0, 0xC0, -frame << 7, d, 1,
                          1);
            func_801E7C98(&work->unk10, &r3, &sc, b, a_340 + 0xA00, e >> 1, e, f, 0x18, 0x3E81, 0, 0xC0, frame * 0xA0, d, 1,
                          -1);
            g_356 = frame << 6;
            if (frame < 0x41) {
                a = g_356 >> 2;
                c_359 = ex->math_cos(a) - 0x5DC;
                rotA.vx = 0;
                rotA.vz = 0;
                rotA.vy = frame << 5;
                scA.vx = scA.vz = ((ex->math_sin(a) * 3) >> 2) + 0xA00;
                g_356 += 0x300;
                scA.vy = g_356 * 2;
                posA.vx = work->unk10.vx;
                posA.vy = work->unk10.vy;
                posA.vz = work->unk10.vz;
                posA.vy -= (ex->math_sin(a) * 0xFA0) >> 12;
                ex->Hi_DrawEffModel(&rotA, &posA, (s32)&scA, work->unk84);
                ex->Hi_ModifyEffModelRGB(work->unk84, (c_359 >> 5) - 0x80, (c_359 >> 5) - 0x80, (c_359 >> 5) - 0x80);
            }
            rotC.vx = 0;
            rotC.vz = 0;
            rotC.vy = frame << 6;
            r_375 = 0x4000;
            posC.vx = work->unk10.vx;
            posC.vy = work->unk10.vy;
            posC.vz = work->unk10.vz;
            for (i = 0, n = 7; i < 6; i++, n += 7) {
                k = frame - n;
                if ((u32)k < 0x20) {
                    c = ex->math_cos((k << 12) >> 7);
                    scC.vx = scC.vz = r_375;
                    if (k < 4) {
                        scC.vy = ex->math_sin((k << 10) >> 2) * 6;
                    } else {
                        scC.vy = 0x6000 - ((k - 4) << 8);
                    }
                    scC.vy = (scC.vy * (8 - i)) >> 3;
                    if (i < 2) {
                        scC.vy = (scC.vy * 5) >> 2;
                    }
                    ex->Hi_DrawEffModel(&rotC, &posC, (s32)&scC, work->unk94);
                    ex->Hi_ModifyEffModelRGB(work->unk94, (c >> 5) - 0x80, (c >> 5) - 0x80, (c >> 5) - 0x80);
                }
                r_375 = (r_375 * 9) >> 3;
                rotC.vy = (rotC.vy * 3) >> 2;
            }
            if (frame < 0x10) {
                g = frame << 12;
                rotF.vy = frame * 0x37;
                rotF.vx = 0;
                rotF.vz = 0;
                scF.vx = scF.vz = (g >> 5) + 0x1200;
                a_405 = g >> 6;
                scF.vy = ex->math_sin(a_405) + 0x1800;
                c_407 = ex->math_cos(a_405);
                posF.vx = work->unk10.vx;
                posF.vy = work->unk10.vy;
                posF.vz = work->unk10.vz;
                ex->Hi_DrawEffModel(&rotF, &posF, (s32)&scF, work->unk80);
                ex->Hi_ModifyEffModelRGB(work->unk80, (c_407 >> 5) - 0x80, (c_407 >> 5) - 0x80, (c_407 >> 5) - 0x80);
            }
            if (frame == 0x3C) {
                ex->func_800E99B0(0x24, entry->unk8, -1, work->unk30);
                ex->func_800E99B0(0x25, entry->unk8, -1, work->unk32);
            }
            BTLFX_COPY_SVEC(&v1, &work->unk10);
            BTLFX_COPY_SVEC(&v2, &work->unk10);
            v2.vy -= 0x1800;
            s_421 = 0x1000 - ((t * 0xDAC) >> 12);
            ang = (work->unk4 << 9) + (ex->btlfx_rand() & 0x1FF);
            r = (ex->math_cos(t >> 2) * 0xF5) >> 12;
            for (i = 0; i < 2; i++, ang += 0x800) {
                func_801E78A4(entry, &D_801EAB08[i], &v1, &v2, (s_421 * 3) >> 2, s_421, ang, 0xC0, 0, 0x1000, r);
            }
            if (frame < 0x26) {
                s_421 = (s_421 * 3) >> 1;
                func_801E78A4(entry, &D_801EAEA0, &v1, &v2, (s_421 * 3) >> 2, s_421, -ang, 0xC0, 0, 0x1000,
                              (ex->math_cos(t >> 1) * 0xF5) >> 12);
            }
            s = 0x1000 - t;
            env->unk28 = 0x300;
            ang = work->unk4 << 7;
            r = ((ex->math_cos(t >> 2) * 0xEB) >> 12) + 0x20;
            for (i = 0; i < 5; i++) {
                v1.vx = work->unk10.vx;
                v1.vy = work->unk10.vy;
                v1.vz = work->unk10.vz;
                v1.vx += (ex->math_sin(ang) * s) >> 12;
                v1.vz += (ex->math_cos(ang) * s) >> 12;
                v2.vx = work->unk10.vx;
                v2.vy = work->unk10.vy;
                v2.vz = work->unk10.vz;
                v2.vx += ex->math_sin(ang);
                v2.vz += ex->math_cos(ang);
                v2.vy -= 0xF00;
                func_801E7AA0(entry, &D_801EA808[i], &v2, &v1, 0x80, 0x200, ang, 0xFF, 0, ex->math_cos(t >> 2), r);
                ang = ang + 0x333 + ex->btlfx_randCentered(0x20);
            }
            env->unk28 = 0;
            if (frame == 0x31) {
                BTLFX_COPY_SVEC(&work->unk18, &work->unk10);
                work->unk18.vy -= 0x400;
                p = ex->func_800E3CEC(ex->btlfx_getFileData(entry->unk8, 0x10), (s32)&work->unk18);
                if (p != NULL) {
                    p->unk22 = 0x180;
                }
            }
            env->unk28 = 0x300;
            rotG.vx = 0x400;
            rotG.vy = 0;
            rotG.vz = 0;
            rotG.pad = 1;
            a = t >> 2;
            ex->func_800D23D8((u8 *)ex->btlfx_getFileData(entry->unk8, 9), &work->unk10, NULL, ex->math_cos(a) + 0x1800, 0,
                              1, ((ex->math_cos(a) * 3) >> 2) + 0x400);
            ex->func_800D23D8((u8 *)ex->btlfx_getFileData(entry->unk8, 2), &work->unk10, &rotG, 0x5000 - (t << 1), 0, 1,
                              ex->math_cos(a));
            env->unk28 = 0;
            ex->btlfx_drawBackdrop((BtlBgLayers *)ex->btlfx_getFileData(entry->unk8, 7), frame);
            ex->func_800E6624(work->unk4 * 5, 0x200, 0x100, 0x20, 0xC0, 0x220, 0x100);
            if (frame >= 0x4D) {
                ex->btlfx_clearEffModels();
                return 1;
            }
            break;
        }
        }
        if (tint1 != 0) {
            ex->btlfx_drawScreenTint((u32 *)(env->unk20 + 0xFFE), 0xFF, 0, 0, (tint1 * 0x4D) >> 12);
        }
        if (tint2 != 0) {
            s32 x = tint2 << 7;
            s32 y = x >> 12;

            if (work->unk4 % 6 == 0) {
                ex->btlfx_drawScreenTint((u32 *)(env->unk20 + 2), 1, 0, (y * 2) / 3, y);
            }
            if (((work->unk4 - 3) & 7) == 0) {
                ex->btlfx_drawScreenTint((u32 *)(env->unk20 + 2), 2, x >> 13, x >> 13, 0);
            }
        }
        work->unk4++;
    }
    return 0;
}
