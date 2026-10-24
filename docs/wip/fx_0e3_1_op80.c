/* Draft of fx_0e3_1 fx_0e3_1_op80 (summon: hand lightning, bolt rings, final blast).
 * Status (pass 3): 66 aligned mismatches by build/w/cmp.py -b (ignoring branch targets; was 244), same length as
 * the original (3210 instructions). Pass 3 levers:
 * - a call result passed to a later call in a single-set variable (qa, qb, q4b, x2k; block-local in { }): sched1's
 *   birthing boost then puts the v0 copy right before the next call (its delay slot), as in the original.
 * - `shake = t;` after the func_800D23D8 call; y12 as `y12 >>= 5; y12 *= 2; y12 += 0x900;` (combine no longer folds
 *   >> 5 << 1 into >> 4); x12's 0x1000 - ... as its own statement; the -0x200 term grouped with the product.
 * - shared loop variables split per loop (ang1, ang2/j1/j2, i4/ang4, i9 kept), case 4's k7a, kb for k13 >> 6,
 *   the frame == 0x26 call result in a block-local p26, y10 reused for the second ring's cos value (the original
 *   keeps both in s4).
 * - case 2's r2.vz = work->unkBC.vz survives flow's dead-store removal only when another stack store sits between
 *   it and r2.vz = (frame << 5) + 0x400 (here s2.vz = 0).
 * - a dead `struct { s32 x; } dq = { frame };` block at the start of the else branch: its CLOBBER gets the spill
 *   slot at 0x280 that the original never references (all later spill slots then line up).
 * Open: case 1's lui of the /6 constant before the a = 0x80 store; y12 in s3 instead of sharing s0 with x2k;
 *   register permutations in the i9/y10 ring loop and case 5 tail; a few arg-register orders.
 * D_801F0A14 is used with a 0x6C stride (13 points) and a 0x2C stride (5 points). */
#include "fxmod/fx_0e3_1.h"

typedef struct {
    s32 vx;
    s32 vy;
    s32 vz;
} Fx0e31Vec3;

typedef struct {
    u32 unk0; /* state */
    s32 unk4; /* frame counter */
    SVECTOR unk8; /* centre */
    Fx0e31Vec3 unk10; /* summon position */
    Fx0e31Vec3 unk1C; /* summon scale */
    SVECTOR unk28; /* summon rotation */
    SVECTOR unk30; /* hand position */
    SVECTOR unk38;
    SVECTOR unk40; /* camera target */
    SVECTOR unk48;
    u8 pad50[8];
    SVECTOR unk58; /* hand rotation */
    Pool6CEntry *unk60;
    Pool6CEntry *unk64;
    Pool6CEntry *unk68;
    s32 unk6C;
    Pool6CEntry *unk70;
    Pool6CEntry *unk74;
    s16 unk78;
    s16 unk7A;
    s32 unk7C; /* model */
    SVECTOR unk80;
    Fx0e31Vec3 unk88;
    Fx0e31Vec3 unk94;
    s32 unkA0; /* model */
    Fx0e31Vec3 unkA4;
    Fx0e31Vec3 unkB0;
    SVECTOR unkBC;
    s32 unkC4; /* models */
    s32 unkC8;
    s32 unkCC;
    s32 unkD0;
    s32 unkD4;
    s32 unkD8;
    s32 unkDC;
    s32 unkE0;
} Fx0e31Work80; /* 0xE4 */

typedef struct {
    u8 unk0;
    u8 unk1;
    u8 unk2;
    u8 unk3;
    SVECTOR unk4[13];
} Fx0e31Bolt13; /* 0x6C */

typedef struct {
    u8 unk0;
    u8 unk1;
    u8 unk2;
    u8 unk3;
    SVECTOR unk4[5];
} Fx0e31Bolt5; /* 0x2C */

typedef struct {
    u8 pad0[0x198];
    BtlUnit9CC *unk198;
    BtlUnit9CC *unk19C;
} Fx0e31SummonData;

typedef struct {
    u8 pad0[0x54];
    Fx0e31SummonData *unk54;
    u8 pad58[8];
    s32 unk60; /* summon model */
} Fx0e31Summon;

#define SUMMON(env) ((Fx0e31Summon *)(env)->unk4)

extern SVECTOR D_801F09B4;
extern Fx0e31Bolt13 D_801F0A14[];
extern Fx0e31Bolt D_801F0D14[3];

s32 fx_0e3_1_op80(s32 mode, Fx0e31Work80 *work, Unk800F78A4 *env, Unk80100C78Entry *entry, s32 *arg4) {
    BtlfxExports *ex = BTLFX_EXPORTS(env);
    MATRIX m;
    MATRIX m2;
    MATRIX m3;
    SVECTOR from;
    SVECTOR to;
    MATRIX m4;
    MATRIX m5;
    MATRIX m6;
    VECTOR s1;
    SVECTOR r1;
    SVECTOR a1;
    SVECTOR b1;
    SVECTOR r2;
    SVECTOR p2;
    VECTOR s2;
    VECTOR v2;
    SVECTOR a2;
    SVECTOR b2;
    SVECTOR r3;
    VECTOR s3;
    CVECTOR c3;
    SVECTOR r4;
    VECTOR s4;
    SVECTOR a4;
    SVECTOR b4;
    SVECTOR r5;
    VECTOR ss;
    SVECTOR r6;
    VECTOR s6;
    VECTOR p6;
    SVECTOR r7;
    VECTOR s7;
    VECTOR p7;
    SVECTOR r8;
    VECTOR s8;
    VECTOR p8;
    SVECTOR f9;
    SVECTOR t9;
    SVECTOR r9;
    s32 frame;
    s32 t;
    s32 tC;
    s32 tB;
    s32 tA;
    s32 a;
    s32 fade;
    s32 tint;
    s32 shake;
    s32 x1;
    s32 q1;
    s32 q2;
    s32 kk;
    s32 k2k;
    s32 q3;
    s32 q4;
    s32 x2;
    s32 x2k;
    s32 x4;
    s32 x5;
    s32 x7;
    s32 x8;
    s32 x9;
    s32 x10;
    s32 x11;
    s32 x12;
    s32 x13;
    s32 x15;
    s32 x16;
    s32 x18;
    s32 x19;
    s32 x20;
    s32 x21;
    s32 y1;
    s32 y2;
    s32 y3;
    s32 y4;
    s32 y5;
    s32 y7;
    s32 y8;
    s32 y9;
    s32 y10;
    s32 y11;
    s32 y12;
    s32 y13;
    s32 k1;
    s32 k2;
    s32 k3;
    s32 k4;
    s32 k5;
    s32 k6;
    s32 k7;
    s32 kd;
    s32 k8;
    s32 k9;
    s32 k10;
    s32 k11;
    s32 k12;
    s32 k13;
    s32 k14;
    s32 k15;
    s32 k16;
    s32 g1;
    s32 g2;
    s32 g3;
    s32 z1;
    s32 n1;
    s32 n2;
    s32 w1;
    s32 w2;
    s32 w3;
    s32 w4;
    s32 t_s2;
    s32 i;
    s32 i5;
    s32 i4;
    s32 j;
    s32 j2;
    s32 j1;
    s32 ang;
    s32 ang3;
    s32 ang1;
    s32 ang4;
    s32 ang2;
    Fx0e31Bolt *bl;
    Pool6CEntry *p;

    if (mode == 0) {
        *arg4 = 0xE4;
    } else if (mode == 1) {
        work->unk0 = 0;
        work->unk4 = 0;
        D_801F0A04 = env;
        work->unkC4 = ex->func_800E8F50((Unk800E8F50 *)ex->btlfx_getFileData(entry->unk8, 0x84), 1);
        work->unkC8 = ex->func_800E8F50((Unk800E8F50 *)ex->btlfx_getFileData(entry->unk8, 0x25), 1);
        work->unkCC = ex->func_800E8F50((Unk800E8F50 *)ex->btlfx_getFileData(entry->unk8, 1), 1);
        work->unk7C = ex->func_800E8F50((Unk800E8F50 *)ex->btlfx_getFileData(entry->unk8, 0x85), 0);
        work->unkA0 = ex->func_800E8F50((Unk800E8F50 *)ex->btlfx_getFileData(entry->unk8, 0x21), 0);
        work->unkD0 = ex->func_800E8F50((Unk800E8F50 *)ex->btlfx_getFileData(entry->unk8, 0xF), 1);
        work->unkD4 = ex->func_800E8F50((Unk800E8F50 *)ex->btlfx_getFileData(entry->unk8, 0xD), 0);
        work->unkD8 = ex->func_800E8F50((Unk800E8F50 *)ex->btlfx_getFileData(entry->unk8, 0x26), 0);
        work->unkDC = ex->func_800E8F50((Unk800E8F50 *)ex->btlfx_getFileData(entry->unk8, 8), 1);
        work->unkE0 = ex->func_800E8F50((Unk800E8F50 *)ex->btlfx_getFileData(entry->unk8, 0x81), 0);
        BTLFX_COPY_SVEC(&work->unk8, &D_801F09B4);
        work->unk28.vy = 0x800;
        work->unk28.vx = 0;
        work->unk28.vz = 0;
        work->unk1C.vz = work->unk1C.vy = work->unk1C.vx = 0x1800;
        work->unk10.vx = work->unk8.vx;
        work->unk10.vy = work->unk8.vy;
        work->unk10.vz = work->unk8.vz;
        work->unk10.vy -= 0x3000;
        work->unk10.vz -= 0x2000;
        ex->Hi_SetSummonMotion(SUMMON(env)->unk54->unk198, SUMMON(env)->unk60);
        ex->Hi_SetSummonMotFrame(SUMMON(env)->unk60, 0);
        work->unk94.vz = work->unk94.vy = work->unk94.vx = 0x1000;
        work->unk80.vx = -0x400;
        work->unk80.vy = 0;
        work->unk80.vz = 0;
        work->unk88.vx = work->unk10.vx;
        work->unk88.vy = work->unk10.vy;
        work->unk88.vz = work->unk10.vz;
    } else {
        {
        struct { s32 x; } dq = { frame }; /* dead: its CLOBBER takes the original's unused spill slot 0x280 */
        }
        a = 0;
        fade = 0;
        tint = 0;
        shake = 0;
        frame = entry->unk0;
        switch (work->unk0) {
            s32 y6_l; /* declared here: its pseudo (spill slot) comes after the function-level ones */
        case 0:
            t = (frame << 12) / 35;
            ex->Hi_DrawSummonModel(&work->unk28, (VECTOR *)&work->unk10, (s32)&work->unk1C, SUMMON(env)->unk60, 1);
            ex->Hi_GetSummonBoneMatrix(SUMMON(env)->unk60, 0x3D, (BtlBone *)&m);
            ex->func_800CFE88(&m, 0x40, (s32)&work->unk40);
            work->unk40.vy += 0x1000 - ex->math_sin(t >> 2);
            env->unk2C->unk0[3].vx = work->unk40.vx;
            env->unk2C->unk0[3].vy = work->unk40.vy;
            a = 0x100;
            env->unk2C->unk0[3].vz = work->unk40.vz;
            if (frame >= 0x23) {
                work->unk0 = 1;
                entry->unk0 = -1;
                work->unk60 = NULL;
            }
            break;
        case 1:
            a = 0x80;
            if (frame < 6) {
                fade = (frame << 11) / 6;
            } else if (frame < 0x2C) {
                fade = ex->math_cos(((frame - 6) << 10) / 38) >> 1;
            }
            work->unk80.vz += 8;
            ex->Hi_DrawSummonModel(&work->unk28, (VECTOR *)&work->unk10, (s32)&work->unk1C, SUMMON(env)->unk60, 0);
            ex->Hi_GetSummonBoneMatrix(SUMMON(env)->unk60, 0x3D, (BtlBone *)&m2);
            ex->func_800CFE88(&m2, 0x5C, (s32)&work->unk30);
            ex->Hi_GetSummonBoneMatrix(SUMMON(env)->unk60, 0x3D, (BtlBone *)&m3);
            ex->func_800CFF40(&m3, &work->unk58.vx);
            work->unk58.vx -= 0x800;
            if (frame >= 0x10) {
                from.vx = work->unk30.vx;
                tA = (frame - 0x10) << 7;
                from.vy = work->unk30.vy;
                from.vz = work->unk30.vz;
                w1 = (tA >> 7) + 0x10;
                ang1 = work->unk4 << 8;
                n1 = (ex->math_sin(tA >> 2) * 6) >> 12;
                for (i = 0; i < n1; i++) {
                    k1 = ex->btlfx_rand();
                    to.vx = work->unk30.vx;
                    to.vy = work->unk30.vy;
                    to.vz = work->unk30.vz;
                    to.vy += ex->math_sin(k1) >> 2;
                    to.vx += ex->math_cos(k1) >> 2;
                    func_801EC71C(entry, (Fx0e31Bolt *)&D_801F0A14[i], &from, &to, w1, w1, ang1, 0xFF, 0, tA);
                }
            }
            ex->Hi_GetSummonBoneMatrix(SUMMON(env)->unk60, 0x3D, (BtlBone *)&m4);
            ex->func_800CFE88(&m4, 0x40, (s32)&work->unk40);
            env->unk2C->unk0[3].vx = work->unk40.vx;
            env->unk2C->unk0[3].vy = work->unk40.vy;
            env->unk2C->unk0[3].vz = work->unk40.vz;
            if (frame == 0) {
                work->unk60 = ex->func_800E3CEC(ex->btlfx_getFileData(entry->unk8, 0x19), (s32)&work->unk30);
                work->unk64 = ex->func_800E3CEC(ex->btlfx_getFileData(entry->unk8, 9), (s32)&work->unk30);
            }
            if (work->unk60 != NULL) {
                work->unk60->unk5C.vx = work->unk58.vx;
                work->unk60->unk5C.vy = work->unk58.vy;
                work->unk60->unk5C.vz = work->unk58.vz;
            }
            if (work->unk64 != NULL) {
                work->unk64->unk5C.vx = work->unk58.vx;
                work->unk64->unk5C.vy = work->unk58.vy;
                work->unk64->unk5C.vz = work->unk58.vz;
            }
            env->unk28 = 0x80;
            t_s2 = ((frame << 12) / 48) >> 2;
            x1 = (ex->math_sin(t_s2) >> 1) + 0x800;
            q1 = ex->math_sin(t_s2);
            ex->func_800D23D8((u8 *)ex->btlfx_getFileData(entry->unk8, 0xB), &work->unk30, NULL, x1 >> 1, 0, 1, q1);
            if (frame >= 0x2B) {
                tB = (((frame - 0x2B) << 12) / 5) >> 2;
                q2 = ex->math_sin(tB) / 3;
                ex->func_800D23D8((u8 *)ex->btlfx_getFileData(entry->unk8, 0x1F), &work->unk30, NULL, q2, 0, 1, 0x1000);
                ex->func_800D3B6C((u8 *)ex->btlfx_getFileData(entry->unk8, 7), &work->unk30, 0xB, ex->math_sin(tB));
            }
            if (frame >= 0x10) {
                k2 = ex->math_sin(((frame - 0x10) << 7) >> 2);
                ex->func_800D23D8((u8 *)ex->btlfx_getFileData(entry->unk8, 0x1A), &work->unk30, NULL, k2, 0, 1, k2);
            }
            if (frame == 0x26) {
                Pool6CEntry *p26 = ex->func_800E3CEC(ex->btlfx_getFileData(entry->unk8, 0x1D), (s32)&work->unk30);
                if (p26 != NULL) {
                    p26->unk22 = 0x80;
                }
            }
            env->unk28 = 0;
            if (frame == 0x10) {
                work->unk68 = p = ex->func_800E3CEC(ex->btlfx_getFileData(entry->unk8, 0x17), (s32)&work->unk30);
                if (p != NULL) {
                    p->unk22 = 0x80;
                }
            }
            if (frame == 0x2F) {
                ex->btlfx_drawScreenTint((u32 *)(env->unk20 + 1), 0xFF, 0x80, 0, 0);
            }
            if (frame >= 0x28) {
                tC = ((frame - 0x28) << 12) >> 5;
                { s32 q4b;
                q3 = (ex->math_cos(tC) * 3) >> 1;
                q4b = ex->math_sin(tC);
                ex->func_800D23D8((u8 *)ex->btlfx_getFileData(entry->unk8, 0x12), &work->unk30, &work->unkBC, q3, 0, 1, q4b); }
            }
            if (frame >= 0x30) {
                work->unk0 = 2;
                entry->unk0 = -1;
                ex->func_800E3C58();
                ex->Hi_SetSummonMotion(SUMMON(env)->unk54->unk19C, SUMMON(env)->unk60);
                ex->Hi_SetSummonMotFrame(SUMMON(env)->unk60, 0);
                ex->btlfx_drawScreenTint((u32 *)(env->unk20 + 1), 2, 0, 0xFF, 0xFF);
            }
            break;
        case 2:
            t = (frame << 12) / 28;
            if (frame >= 0xE) {
                tint = ((frame - 0xE) << 12) / 14;
            }
            ex->Hi_DrawSummonModel(&work->unk28, (VECTOR *)&work->unk10, (s32)&work->unk1C, SUMMON(env)->unk60, 1);
            if (frame == 1) {
                ex->Hi_ModifySummonModelRGB(SUMMON(env)->unk60, -0x60, -0x60, -0x60);
            }
            if (frame == 0) {
                ex->Hi_GetSummonBoneMatrix(SUMMON(env)->unk60, 0x3D, (BtlBone *)&m5);
                ex->func_800CFE88(&m5, 0x80, (s32)&work->unk40);
                ex->Hi_GetSummonBoneMatrix(SUMMON(env)->unk60, 0x3D, (BtlBone *)&m6);
                ex->func_800CFF40(&m6, &work->unkBC.vx);
            }
            if (frame < 0x12) {
                ex->Hi_GetSummonBoneMatrix(SUMMON(env)->unk60, 0x3D, (BtlBone *)&m5);
                ex->func_800CFE88(&m5, 0x5C, (s32)&work->unk30);
                ex->Hi_GetSummonBoneMatrix(SUMMON(env)->unk60, 0x3D, (BtlBone *)&m6);
                ex->func_800CFE88(&m6, 0x80, (s32)&work->unk40);
            } else {
                if (frame == 0x12) {
                    ex->Hi_GetSummonBoneMatrix(SUMMON(env)->unk60, 0x3D, (BtlBone *)&m);
                    ex->func_800CFE88(&m, 0x80, (s32)&work->unk48);
                }
                ex->func_800CFD78(&work->unk48.vx, &work->unkBC.vx,
                                  -((ex->math_sin(((frame - 0xE) << 10) / 14) * 3) / 2), &work->unk40.vx);
            }
            env->unk2C->unk0[3].vx = work->unk40.vx;
            env->unk2C->unk0[3].vy = work->unk40.vy;
            env->unk2C->unk0[3].vz = work->unk40.vz;
            if (frame == 1) {
                work->unkB0.vx = work->unkB0.vy = 0x249;
            } else if (frame < 0x11) {
                work->unkB0.vx = work->unkB0.vy = ((0x1000 - ex->math_sin((frame << 8) >> 2)) * 4) + 0x400;
            } else {
                work->unkB0.vx = work->unkB0.vy = 0x400;
            }
            work->unkB0.vz = 0xAAA;
            work->unkA4.vx = work->unk30.vx;
            work->unkA4.vy = work->unk30.vy;
            work->unkA4.vz = work->unk30.vz;
            func_801ED708((VECTOR *)&work->unkA4, &work->unkBC);
            ex->Hi_DrawEffModel(&work->unkBC, (VECTOR *)&work->unkA4, (s32)&work->unkB0, work->unkA0);
            if (frame < 0x18 && frame != 1) {
                k3 = (frame << 10) / 24;
                s1.vx = s1.vy = ex->math_sin(k3) / 2 + 0x800;
                s1.vz = 0x1000;
                r1.vx = work->unkBC.vx;
                r1.vy = work->unkBC.vy;
                r1.vz = frame << 5;
                ex->Hi_DrawEffModel(&r1, (VECTOR *)&work->unkA4, (s32)&s1, work->unkE0);
                ex->Hi_ModifyEffModelRGB(work->unkE0, ((ex->math_cos(k3) - 0x800) >> 5) - 0x80,
                                         ((ex->math_cos(k3) - 0x800) >> 5) - 0x80, ((ex->math_cos(k3) - 0x800) >> 5) - 0x80);
            }
            BTLFX_COPY_SVEC(&a1, &work->unk30);
            ex->func_800CFD78(&a1.vx, &work->unkBC.vx, -(t + 0x1000), &b1.vx);
            for (kk = 0, ang3 = -work->unk4 << 7; kk < 3; kk++, ang3 += 0x554) {
                g1 = ex->btlfx_randCentered(0x400) + 0xC00;
                func_801ECAF8(entry, &D_801F0D14[kk], &a1, &b1, 0x7C, 0x200, ang3, 0xC0, 0, g1, 0x1D);
                for (j1 = 0; j1 < 4; j1++) {
                    func_801ECCF4(entry, &D_801F0D14[kk], (Fx0e31Bolt *)((Fx0e31Bolt5 *)D_801F0A14 + j1), g1);
                }
            }
            if (frame < 0x21) {
                s32 ya;
                s32 yb;
                ya = frame << 12;
                yb = ya >> 5;
                x2 = ex->math_cos(ya >> 7);
                i = 0;
                r2.vx = work->unkBC.vx;
                r2.vy = work->unkBC.vy;
                r2.vz = work->unkBC.vz;
                s2.vz = 0;
                s2.vx = s2.vy = yb * 2 + 0x900;
                r2.vz = (frame << 5) + 0x400;
                ex->func_800CFD78(&work->unk30.vx, &work->unkBC.vx, -0x80, &p2.vx);
                y12 = (x2 >> 5) - 0x80;
                v2.vx = p2.vx;
                v2.vy = p2.vy;
                v2.vz = p2.vz;
                for (; i < 3; i++) {
                    ex->Hi_DrawEffModel(&r2, &v2, (s32)&s2, work->unkC4);
                    ex->Hi_ModifyEffModelRGB(work->unkC4, y12, y12, y12);
                    s2.vx = s2.vy = (s2.vx * 3) >> 2;
                    r2.vz += 0x550;
                }
            }
            env->unk28 = 0x190;
            if (frame < 0x10 && frame != 1) {
                x4 = (ex->math_cos(t >> 2) >> 2) + 0x555;
                y1 = ex->math_cos((frame << 8) >> 2);
                ex->func_800D23D8((u8 *)ex->btlfx_getFileData(entry->unk8, 0x1F), &work->unk30, NULL, x4, 0, 1, y1);
                work->unkBC.pad = 1;
                ex->func_800D23D8((u8 *)ex->btlfx_getFileData(entry->unk8, 0x14), &work->unk30, &work->unkBC, 0x1000,
                                  frame, 1, y1);
                ex->btlfx_drawSprites((u8 *)ex->btlfx_getFileData(entry->unk8, 0x18), &work->unk30, &work->unkBC, 0x2000,
                                      frame, 1, y1);
                ex->func_800D3B6C((u8 *)ex->btlfx_getFileData(entry->unk8, 7), &work->unk30, frame - 1, y1);
            }
            if (frame < 0x11) {
                x5 = (ex->math_cos(t >> 2) >> 2) + 0x800;
            } else {
                x5 = 0x800;
            }
            ex->func_800D23D8((u8 *)ex->btlfx_getFileData(entry->unk8, 0x1C), &work->unk30, NULL, (x5 * 3) >> 1, 0, 1,
                              0x1000);
            shake = t;
            k5 = t >> 2;
            x7 = ex->math_cos(k5) + 0x1000;
            {
                s32 qa;
                qa = ex->math_cos(k5);
                ex->func_800D23D8((u8 *)ex->btlfx_getFileData(entry->unk8, 0x1A), &work->unk30, NULL, x7, 0, 1, qa);
            }
            env->unk28 = 0;
            if (frame >= 0x1C) {
                work->unk0 = 3;
                entry->unk0 = -1;
            }
            break;
        case 3:
            tint = 0x1000;
            if (frame == 0) {
                work->unk30.vx = D_801F09B4.vx;
                work->unk30.vy = D_801F09B4.vy;
                work->unk30.vz = D_801F09B4.vz;
                work->unk30.vy -= 0x1000;
                work->unkBC.vx = -0x400;
                work->unkBC.vy = 0;
                work->unkBC.vz = 0;
                work->unkA4.vx = D_801F09B4.vx;
                work->unkA4.vy = D_801F09B4.vy;
                work->unkA4.vz = D_801F09B4.vz;
                work->unkA4.vy -= 0x1000;
                work->unkB0.vx = 0x1200;
                work->unkB0.vy = 0x1200;
                work->unkB0.vz = 0x1555;
            }
            work->unk40.vy = work->unk30.vy;
            work->unk40.vx = work->unk30.vx;
            work->unk40.vz = work->unk30.vz;
            work->unk40.vy -= 0x1000;
            env->unk2C->unk0[3].vx = work->unk40.vx;
            env->unk2C->unk0[3].vy = work->unk40.vy;
            env->unk2C->unk0[3].vz = work->unk40.vz;
            func_801ED708((VECTOR *)&work->unkA4, &work->unkBC);
            ex->Hi_DrawEffModel(&work->unkBC, (VECTOR *)&work->unkA4, (s32)&work->unkB0, work->unkA0);
            BTLFX_COPY_SVEC(&a2, &work->unk30);
            ex->func_800CFD78(&a2.vx, &work->unkBC.vx, -0x2800, &b2.vx);
            for (k2k = 0, ang2 = -work->unk4 << 7; k2k < 3; k2k++, ang2 += 0x554) {
                g2 = ex->btlfx_randCentered(0x400) + 0xC00;
                func_801ECAF8(entry, &D_801F0D14[k2k], &a2, &b2, 0x400, 0x528, ang2, 0xC0, 0, g2, 0x84);
                for (j2 = 0; j2 < 4; j2++) {
                    func_801ECCF4(entry, &D_801F0D14[k2k], (Fx0e31Bolt *)((Fx0e31Bolt5 *)D_801F0A14 + j2), g2);
                }
            }
            shake = 0x1000;
            if (frame >= 2) {
                work->unk0 = 4;
                entry->unk0 = -1;
            }
            break;
        case 4:
            t = (frame << 12) / 14;
            if (frame == 5) {
                ex->func_800E9930(0x28, entry->unk8, -1);
                ex->func_800E9930(0x2A, entry->unk8, -1);
                ex->func_800E9930(0x2B, entry->unk8, -1);
                ex->func_800E9930(0x2C, entry->unk8, -1);
            }
            shake = ex->math_cos(t >> 2);
            if (frame == 0) {
                work->unk30.vx = D_801F09B4.vx;
                work->unk30.vy = D_801F09B4.vy;
                work->unk30.vz = D_801F09B4.vz;
                work->unkA4.vx = D_801F09B4.vx;
                work->unkA4.vy = D_801F09B4.vy;
                work->unkA4.vz = D_801F09B4.vz;
            }
            if (frame < 4) {
                tint = 0x1000 - ((frame << 12) >> 2);
                work->unkB0.vx = work->unkB0.vy = 0x1200;
                work->unkB0.vz = 0x1555;
            } else if (frame == 4) {
                work->unkB0.vx = work->unkB0.vy = 0x4000;
            } else if ((kd = frame - 4) < 0xB) {
                kd = (kd << 12) / 10;
                work->unkB0.vx = work->unkB0.vy = ((0x1000 - kd) * 3) >> 1;
            }
            if (frame < 5) {
                work->unk40.vx = work->unk30.vx;
                work->unk40.vy = work->unk30.vy;
                work->unk40.vz = work->unk30.vz;
                work->unk40.vy -= 0x180 + (((0x1000 - ((frame << 12) >> 2)) * 3) >> 1);
                env->unk2C->unk0[3].vx = work->unk40.vx;
                env->unk2C->unk0[3].vy = work->unk40.vy;
                env->unk2C->unk0[3].vz = work->unk40.vz;
            }
            if (frame == 4) {
                ex->btlfx_drawScreenTint((u32 *)(env->unk20 + 2), 1, 0x20, 0x80, 0xFF);
            } else if ((u32)(frame - 5) < 7) {
                k6 = 0x1000 - (((frame - 5) << 12) / 7);
                x8 = k6 >> 5;
                work->unk40.vx = D_801F09B4.vx + ex->btlfx_randCentered(x8);
                work->unk40.vy = D_801F09B4.vy + ex->btlfx_randCentered(x8);
                {
                    s32 dd = (k6 >> 6) * ((frame & 1) * 2 - 1) - 0x200;
                    work->unk40.vy = D_801F09B4.vy + dd;
                }
                env->unk2C->unk0[3].vx = work->unk40.vx;
                env->unk2C->unk0[3].vy = work->unk40.vy;
                env->unk2C->unk0[3].vz = work->unk40.vz;
            }
            if (work->unkB0.vx != 0) {
                func_801ED708((VECTOR *)&work->unkA4, &work->unkBC);
                ex->Hi_DrawEffModel(&work->unkBC, (VECTOR *)&work->unkA4, (s32)&work->unkB0, work->unkA0);
            }
            if (frame >= 4) {
                s32 k7a;
                r3.vx = 0;
                r3.vz = 0;
                k7a = ((frame - 4) << 12) / 10;
                x9 = k7a >> 2;
                r3.vy = k7a >> 3;
                s3.vx = s3.vz = x9 + 0x555;
                s3.vy = (k7a >> 1) + 0x400;
                y2 = ex->math_cos(x9);
                ex->Hi_DrawEffModel(&r3, (VECTOR *)&work->unkA4, (s32)&s3, work->unkC8);
                ex->Hi_ModifyEffModelRGB(work->unkC8, (y2 >> 5) - 0x80, (y2 >> 5) - 0x80, (y2 >> 5) - 0x80);
                s3.vx = s3.vz = s3.vx * 2;
                s3.vy >>= 2;
                ex->Hi_DrawEffModel(&r3, (VECTOR *)&work->unkA4, (s32)&s3, work->unkC8);
                ex->Hi_ModifyEffModelRGB(work->unkC8, (y2 >> 6) - 0x80, (y2 >> 6) - 0x80, (y2 >> 6) - 0x80);
            }
            if (frame == 0) {
                work->unk70 = ex->func_800E3CEC(ex->btlfx_getFileData(entry->unk8, 6), (s32)&work->unk30);
                work->unk74 = ex->func_800E3CEC(ex->btlfx_getFileData(entry->unk8, 0xC), (s32)&work->unk30);
                ex->func_800E3CEC(ex->btlfx_getFileData(entry->unk8, 0xE), (s32)&work->unk30);
            }
            c3.r = c3.g = c3.b = 0x37;
            s4.vx = 0x1400;
            s4.vy = 0x1400;
            s4.vz = 0x1000;
            r4.vx = 0;
            r4.vy = 0;
            r4.vz = 0;
            ex->func_800E6764(3, 0xA0, 0x70, 0x50, 0x38, 4, 4, 2, 2, &r4, &s4, &c3, 1, NULL);
            BTLFX_COPY_SVEC(&a4, &work->unk30);
            ex->func_800CFD78(&a4.vx, &work->unkBC.vx, t - 0x2800, &b4.vx);
            w2 = (t >> 2) + 0x400;
            for (i4 = 0, ang4 = work->unk4 << 8; i4 < 3; i4++, ang4 += 0x554) {
                func_801ECAF8(entry, &D_801F0D14[i4], &a4, &b4, w2, w2, ang4, 0xC0, 0, ex->btlfx_randCentered(0x400) + 0xC00,
                              0x84);
            }
            env->unk28 = 0x200;
            k8 = t >> 2;
            x10 = (ex->math_sin(k8) * 2) + 0x1000;
            y3 = (ex->math_cos(k8) >> 1) + 0x800;
            ex->func_800D23D8((u8 *)ex->btlfx_getFileData(entry->unk8, 0x1C), &work->unk30, NULL, x10, 0, 1, y3);
            x11 = (ex->math_sin(k8) * 3) + 0x1000;
            {
                s32 qb;
                qb = ex->math_cos(k8);
                ex->func_800D23D8((u8 *)ex->btlfx_getFileData(entry->unk8, 0x1A), &work->unk30, NULL, x11, 0, 1, qb);
            }
            if (frame >= 4) {
                k7 = frame - 4;
                if (k7 < 0xA) {
                    r5.vx = 0x400;
                    r5.vy = 0;
                    r5.vz = 0;
                    r5.pad = 1;
                    ex->func_800D23D8((u8 *)ex->btlfx_getFileData(entry->unk8, 0x14), &work->unk30, &r5, 0x4000, k7, 1,
                                      0x1000);
                }
            }
            env->unk28 = 0;
            if (frame >= 0xE) {
                work->unk0 = 5;
                entry->unk0 = -1;
                ex->func_800E3D54(work->unk70);
                ex->func_800E3D54(work->unk74);
                ex->btlfx_drawScreenTint((u32 *)(env->unk20 + 1), 1, 0, 0x80, 0xFF);
                p = ex->func_800E3CEC(ex->btlfx_getFileData(entry->unk8, 5), (s32)&work->unk30);
                if (p != NULL) {
                    p->unk22 = 0x300;
                }
                p = ex->func_800E3CEC(ex->btlfx_getFileData(entry->unk8, 4), (s32)&work->unk30);
                if (p != NULL) {
                    p->unk22 = 0x300;
                }
                p = ex->func_800E3CEC(ex->btlfx_getFileData(entry->unk8, 0x29), (s32)&work->unk30);
                if (p != NULL) {
                    p->unk22 = 0x400;
                }
                work->unk78 = ex->func_800E9930(0x30, entry->unk8, -1);
                work->unk7A = ex->func_800E9930(0x31, entry->unk8, -1);
                ex->func_800E9930(0x2D, entry->unk8, -1);
                ex->func_800E9930(0x2E, entry->unk8, -1);
                env->unk2C->unk0[2].vx = 0;
                env->unk2C->unk0[2].vy = -0xC8;
                env->unk2C->unk0[2].vz = 0x300;
            }
            break;
        case 5:
            t = (frame << 12) / 77;
            if (frame == 0) {
                work->unk30.vx = D_801F09B4.vx;
                work->unk30.vy = D_801F09B4.vy;
                work->unk30.vz = D_801F09B4.vz;
                work->unkA4.vx = D_801F09B4.vx;
                work->unkA4.vy = D_801F09B4.vy;
                work->unkA4.vz = D_801F09B4.vz;
                work->unkBC.vx = -0x400;
                work->unkBC.vy = 0;
                work->unkBC.vz = 0;
                work->unkB0.vx = 0x4000;
                work->unkB0.vy = 0x4000;
                work->unkB0.vz = 0x4000;
            }
            work->unk40.vx = work->unk30.vx;
            work->unk40.vy = work->unk30.vy;
            work->unk40.vz = work->unk30.vz;
            work->unk40.vy = -0x2D4 - ((t * 0x76C) >> 12);
            env->unk2C->unk0[3].vx = work->unk40.vx;
            env->unk2C->unk0[3].vy = work->unk40.vy;
            env->unk2C->unk0[3].vz = work->unk40.vz;
            if (frame < 2) {
                ex->btlfx_drawScreenTint((u32 *)(env->unk20 + 2), 2, 0xFF - (frame << 7), 0xFF - (frame << 7), 0xFF);
            } else if (frame == 2) {
                ex->btlfx_drawScreenTint((u32 *)(env->unk20 + 2), 1, 0xFF, 0xFF, 0xFF);
            } else if (frame < 0x13) {
                x12 = 0x1000 - (((frame - 3) << 12) >> 4);
                x12 = (x12 * 0xA0) >> 12;
                ex->btlfx_drawScreenTint((u32 *)(env->unk20 + 2), 1, x12, x12, x12);
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
                    x13 = 0x2000 - ex->math_sin((frame << 10) / 7);
                } else {
                    x13 = 0x7800 - ((frame - 7) * 0x1124) / 65 * 7;
                }
                if ((u32)(frame - 5) < 7) {
                    y4 = ex->math_sin(((frame - 5) << 11) >> 12) * 0xF5;
                    ex->btlfx_drawScreenTint((u32 *)(env->unk20 + 0x400), 1, 0, y4 >> 14, y4 >> 12);
                }
                work->unkB0.vx = work->unkB0.vy = x13;
                func_801ED708((VECTOR *)&work->unkA4, &work->unkBC);
                work->unkBC.vz -= 0x400;
                ex->Hi_DrawEffModel(&work->unkBC, (VECTOR *)&work->unkA4, (s32)&work->unkB0, work->unkD0);
                ex->Hi_DrawEffModel(&work->unkBC, (VECTOR *)&work->unkA4, (s32)&work->unkB0, work->unkD4);
            }
            if (frame >= 0x3B) {
                k9 = ((frame - 0x3B) << 12) / 18;
                work->unkB0.vx = work->unkB0.vy = 0x1000;
                func_801ED708((VECTOR *)&work->unkA4, &work->unkBC);
                work->unkBC.vz += 0x400;
                ex->Hi_DrawEffModel(&work->unkBC, (VECTOR *)&work->unkA4, (s32)&work->unkB0, work->unkD8);
                ex->Hi_ModifyEffModelRGB(work->unkD8, (ex->math_cos(k9 >> 2) >> 5) - 0x80, (ex->math_cos(k9 >> 2) >> 5) - 0x80,
                                         (ex->math_cos(k9 >> 2) >> 5) - 0x80);
            }
            k10 = t >> 2;
            z1 = (t >> 1) + 0xE00;
            y5 = ex->math_cos(k10);
            ss.vx = 0x1000;
            ss.vy = 0x1000;
            ss.vz = 0x1000;
            g3 = (y5 >> 2) + 0x100;
            r5.vx = 0x400;
            r5.vy = 0;
            r5.vz = 0;
            n2 = (((t >> 3) + 0x500) * 4) / 3;
            y6_l = ex->math_cos(k10) >> 5;
            func_801ECFE4(&work->unk30, &r5, &ss, z1, k10 + 0xA00, g3 >> 1, g3, y6_l, 0x1D, 0x3E81, 0, 0xC0, -frame << 7, n2, 1, 1);
            func_801ECFE4(&work->unk30, &r5, &ss, z1, k10 + 0xA00, g3 >> 1, g3, y6_l, 0x1D, 0x3E81, 0, 0xC0, frame * 0xA0, n2, 1, -1);
            if (frame < 0x41) {
                k11 = (frame << 6) >> 2;
                y7 = ex->math_cos(k11) - 0x5DC;
                r6.vx = 0;
                r6.vz = 0;
                r6.vy = frame << 5;
                s6.vx = s6.vz = ((ex->math_sin(k11) * 3) >> 2) + 0xA00;
                s6.vy = ((frame << 6) + 0x300) * 2;
                p6.vx = work->unk30.vx;
                p6.vy = work->unk30.vy;
                p6.vz = work->unk30.vz;
                p6.vy -= (ex->math_sin(k11) * 0xFA0) >> 12;
                ex->Hi_DrawEffModel(&r6, &p6, (s32)&s6, work->unkCC);
                x15 = (y7 >> 5) - 0x80;
                ex->Hi_ModifyEffModelRGB(work->unkCC, x15, x15, x15);
            }
            r7.vx = 0;
            r7.vz = 0;
            r7.vy = frame << 6;
            w3 = 0x4000;
            p7.vx = work->unk30.vx;
            p7.vy = work->unk30.vy;
            p7.vz = work->unk30.vz;
            for (i = 0, j = 7; i < 6; j += 7, i++) {
                k12 = frame - j;
                if ((u32)k12 < 0x20) {
                    y8 = ex->math_cos((k12 << 12) >> 7);
                    s7.vx = s7.vz = w3;
                    if (k12 < 4) {
                        s7.vy = ex->math_sin((k12 << 10) >> 2) * 6;
                    } else {
                        s7.vy = 0x6000 - ((k12 - 4) << 8);
                    }
                    s7.vy = (s7.vy * (8 - i)) >> 3;
                    if (i < 2) {
                        s7.vy = (s7.vy * 5) >> 2;
                    }
                    ex->Hi_DrawEffModel(&r7, &p7, (s32)&s7, work->unkDC);
                    ex->Hi_ModifyEffModelRGB(work->unkDC, (y8 >> 5) - 0x80, (y8 >> 5) - 0x80, (y8 >> 5) - 0x80);
                }
                w3 = (w3 * 9) >> 3;
                r7.vy = (r7.vy * 3) >> 2;
            }
            if (frame < 0x10) {
                k13 = frame << 12;
                r8.vy = frame * 0x37;
                r8.vx = 0;
                r8.vz = 0;
                s8.vx = s8.vz = (k13 >> 5) + 0x1200;
                {
                    s32 kb = k13 >> 6;
                    s8.vy = ex->math_sin(kb) + 0x1800;
                    y9 = ex->math_cos(kb);
                }
                p8.vx = work->unk30.vx;
                p8.vy = work->unk30.vy;
                p8.vz = work->unk30.vz;
                ex->Hi_DrawEffModel(&r8, &p8, (s32)&s8, work->unkC8);
                ex->Hi_ModifyEffModelRGB(work->unkC8, (y9 >> 5) - 0x80, (y9 >> 5) - 0x80, (y9 >> 5) - 0x80);
            }
            ((s32 *)&t9)[0] = ((s32 *)&f9)[0] = ((s32 *)&work->unk30)[0];
            ((s32 *)&t9)[1] = ((s32 *)&f9)[1] = ((s32 *)&work->unk30)[1];
            t9.vy -= 0x1800;
            x16 = 0x1000 - ((t * 0xDAC) >> 12);
            ang = (work->unk4 << 9) + (ex->btlfx_rand() & 0x1FF);
            y10 = (ex->math_cos(t >> 2) * 0xF5) >> 12;
            for (i5 = 0; i5 < 2; i5++, ang += 0x800) {
                func_801ECAF8(entry, &D_801F0D14[i5], &f9, &t9, (x16 * 3) >> 2, x16, ang, 0xC0, 0, 0x1000, y10);
            }
            if (frame < 0x26) {
                x16 = (x16 * 3) >> 1;
                y10 = (ex->math_cos(t >> 1) * 0xF5) >> 12;
                func_801ECAF8(entry, &D_801F0D14[2], &f9, &t9, (x16 * 3) >> 2, x16, -ang, 0xC0, 0, 0x1000, y10);
            }
            x18 = 0x1000 - t;
            env->unk28 = 0x300;
            ang = work->unk4 << 7;
            y11 = ((ex->math_cos(t >> 2) * 0xEB) >> 12) + 0x20;
            for (i5 = 0; i5 < 5; i5++, ang += 0x333 + ex->btlfx_randCentered(0x20)) {
                f9.vx = work->unk30.vx;
                f9.vy = work->unk30.vy;
                f9.vz = work->unk30.vz;
                f9.vx += (ex->math_sin(ang) * x18) >> 12;
                f9.vz += (ex->math_cos(ang) * x18) >> 12;
                t9.vx = work->unk30.vx;
                t9.vy = work->unk30.vy;
                t9.vz = work->unk30.vz;
                t9.vx += ex->math_sin(ang);
                t9.vz += ex->math_cos(ang);
                t9.vy -= 0xF00;
                func_801ECDEC(entry, (Fx0e31Bolt *)&D_801F0A14[i5], &t9, &f9, 0x80, 0x200, ang, 0xFF, 0,
                              ex->math_cos(t >> 2), y11);
            }
            env->unk28 = 0;
            if (frame == 0x31) {
                BTLFX_COPY_SVEC(&work->unk38, &work->unk30);
                work->unk38.vy -= 0x400;
                p = ex->func_800E3CEC(ex->btlfx_getFileData(entry->unk8, 0x20), (s32)&work->unk38);
                if (p != NULL) {
                    p->unk22 = 0x180;
                }
            }
            env->unk28 = 0x300;
            r9.vx = 0x400;
            r9.vy = 0;
            r9.vz = 0;
            r9.pad = 1;
            k14 = t >> 2;
            x19 = ex->math_cos(k14) + 0x1800;
            y13 = ((ex->math_cos(k14) * 3) >> 2) + 0x400;
            ex->func_800D23D8((u8 *)ex->btlfx_getFileData(entry->unk8, 0x1C), &work->unk30, NULL, x19, 0, 1, y13);
            x19 = 0x5000 - t * 2;
            y13 = ex->math_cos(k14);
            ex->func_800D23D8((u8 *)ex->btlfx_getFileData(entry->unk8, 0x1B), &work->unk30, &r9, x19, 0, 1, y13);
            env->unk28 = 0;
            ex->btlfx_drawBackdrop((BtlBgLayers *)ex->btlfx_getFileData(entry->unk8, 0x16), frame);
            ex->func_800E6624(work->unk4 * 8, 0x340, 0x100, 0x20, 0xC0, 0x360, 0x100);
            if (frame >= 0x4D) {
                ex->btlfx_clearEffModels();
                return 1;
            }
            break;
        }
        if (a != 0) {
            k15 = work->unk4 << 8;
            ex->Hi_GetSummonBonePos(SUMMON(env)->unk60, ex->btlfx_rand() % 92, (u16 *)&m);
            ex->Hi_GetSummonBonePos(SUMMON(env)->unk60, ex->btlfx_rand() % 92, (u16 *)((SVECTOR *)&m + 1));
            func_801EC71C(entry, (Fx0e31Bolt *)D_801F0A14, (SVECTOR *)&m, (SVECTOR *)&m + 1, a, a, k15, 0x100, 0,
                          ex->btlfx_randCentered(0x400) + 0xC00);
        }
        if (fade != 0) {
            ex->Hi_DrawEffModel(&work->unk80, (VECTOR *)&work->unk88, (s32)&work->unk94, work->unk7C);
            ex->Hi_ModifyEffModelRGB(work->unk7C, (fade >> 5) - 0x80, (fade >> 5) - 0x80, (fade >> 5) - 0x80);
        }
        if (tint != 0) {
            ex->btlfx_drawScreenTint((u32 *)(env->unk20 + 0xFFE), 0xFF, 0, 0, (tint * 0x4D) >> 12);
        }
        if (shake != 0) {
            k16 = shake << 7;
            x21 = k16 >> 12;
            if (work->unk4 == work->unk4 / 6 * 6) {
                ex->btlfx_drawScreenTint((u32 *)(env->unk20 + 2), 1, 0, x21 * 2 / 3, x21);
            }
            if (!((work->unk4 - 3) & 7)) {
                ex->btlfx_drawScreenTint((u32 *)(env->unk20 + 2), 2, k16 >> 13, k16 >> 13, 0);
            }
        }
        work->unk4++;
    }
    return 0;
}
