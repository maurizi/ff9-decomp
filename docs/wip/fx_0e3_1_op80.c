/* Draft of fx_0e3_1 fx_0e3_1_op80 (summon: hand lightning, bolt rings, final blast). Status: first draft from m2c
 * + asm, untested and far: ~2400 rows ignoring registers and spill offsets. Global allocation spills work, env, entry
 * and ex here (the original keeps ex in fp and work in s7 and spills frame/t/a/fade/tint/shake to 0x268-0x27C, and
 * the func_801ECFE4 arguments in case 5 to 0x284-0x294), which adds a reload before almost every access. Several
 * divisions are probably written as (x / n) >> k in the original (m2c folds them, e.g. case 1's / 192 is / 48 >> 2).
 * The local bolt buffers: D_801F0A14 is used with a 0x6C stride (13 points) and a 0x2C stride (5 points). */
#include "fxmod/fx_0e3_1.h"

typedef struct {
    u32 unk0; /* state */
    s32 unk4; /* frame counter */
    SVECTOR unk8; /* centre */
    VECTOR unk10; /* summon position */
    VECTOR unk1C; /* summon scale */
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
    VECTOR unk88;
    VECTOR unk94;
    s32 unkA0; /* model */
    VECTOR unkA4;
    VECTOR unkB0;
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
    SVECTOR from;
    SVECTOR to;
    MATRIX m3;
    MATRIX m4;
    MATRIX m5;
    MATRIX m6;
    VECTOR s1;
    SVECTOR r1;
    SVECTOR a1;
    SVECTOR b1;
    SVECTOR r2;
    VECTOR s2;
    SVECTOR p2;
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
    SVECTOR rr;
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
    s32 a;
    s32 fade;
    s32 tint;
    s32 shake;
    s32 i;
    s32 j;
    s32 k;
    s32 n;
    s32 w;
    s32 x;
    s32 y;
    s32 z;
    s32 g;
    s32 ang;
    Fx0e31Bolt *bl;
    Pool6CEntry *p;

    if (mode == 0) {
        *arg4 = 0xE4;
        return 0;
    }
    if (mode == 1) {
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
        work->unk1C.vx = work->unk1C.vy = work->unk1C.vz = 0x1800;
        work->unk10.vx = work->unk8.vx;
        work->unk10.vy = work->unk8.vy;
        work->unk10.vz = work->unk8.vz;
        work->unk10.vy -= 0x3000;
        work->unk10.vz -= 0x2000;
        ex->Hi_SetSummonMotion(SUMMON(env)->unk54->unk198, SUMMON(env)->unk60);
        ex->Hi_SetSummonMotFrame(SUMMON(env)->unk60, 0);
        work->unk94.vx = work->unk94.vy = work->unk94.vz = 0x1000;
        work->unk80.vx = -0x400;
        work->unk80.vy = 0;
        work->unk80.vz = 0;
        work->unk88.vx = work->unk10.vx;
        work->unk88.vy = work->unk10.vy;
        work->unk88.vz = work->unk10.vz;
        return 0;
    }
    a = 0;
    fade = 0;
    tint = 0;
    shake = 0;
    frame = entry->unk0;
    switch (work->unk0) {
    case 0:
        t = (frame << 12) / 35;
        ex->Hi_DrawSummonModel(&work->unk28, &work->unk10, (s32)&work->unk1C, SUMMON(env)->unk60, 1);
        ex->Hi_GetSummonBoneMatrix(SUMMON(env)->unk60, 0x3D, (BtlBone *)&m);
        ex->func_800CFE88(&m, 0x40, (s32)&work->unk40);
        work->unk40.vy = work->unk40.vy + 0x1000 - ex->math_sin(t >> 2);
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
        ex->Hi_DrawSummonModel(&work->unk28, &work->unk10, (s32)&work->unk1C, SUMMON(env)->unk60, 0);
        ex->Hi_GetSummonBoneMatrix(SUMMON(env)->unk60, 0x3D, (BtlBone *)&m2);
        ex->func_800CFE88(&m2, 0x5C, (s32)&work->unk30);
        ex->Hi_GetSummonBoneMatrix(SUMMON(env)->unk60, 0x3D, (BtlBone *)&m3);
        ex->func_800CFF40(&m3, &work->unk58.vx);
        work->unk58.vx -= 0x800;
        if (frame >= 0x10) {
            from.vx = work->unk30.vx;
            t = (frame - 0x10) << 7;
            from.vy = work->unk30.vy;
            from.vz = work->unk30.vz;
            w = (t >> 7) + 0x10;
            ang = work->unk4 << 8;
            n = (ex->math_sin(t >> 2) * 6) >> 12;
            for (i = 0; i < n; i++) {
                k = ex->btlfx_rand();
                to.vx = work->unk30.vx;
                to.vy = work->unk30.vy;
                to.vz = work->unk30.vz;
                to.vy += ex->math_sin(k) >> 2;
                to.vx += ex->math_cos(k) >> 2;
                func_801EC71C(entry, (Fx0e31Bolt *)&D_801F0A14[i], &from, &to, w, w, ang, 0xFF, 0, t);
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
        t = (frame << 12) / 192;
        x = (ex->math_sin(t) >> 1) + 0x800;
        ex->func_800D23D8((u8 *)ex->btlfx_getFileData(entry->unk8, 0xB), &work->unk30, NULL, x >> 1, 0, 1,
                          ex->math_sin(t));
        if (frame >= 0x2B) {
            t = ((frame - 0x2B) << 12) / 20;
            ex->func_800D23D8((u8 *)ex->btlfx_getFileData(entry->unk8, 0x1F), &work->unk30, NULL, ex->math_sin(t) / 3,
                              0, 1, 0x1000);
            ex->func_800D3B6C((u8 *)ex->btlfx_getFileData(entry->unk8, 7), &work->unk30, 0xB, ex->math_sin(t));
        }
        if (frame >= 0x10) {
            k = ex->math_sin(((frame - 0x10) << 7) >> 2);
            ex->func_800D23D8((u8 *)ex->btlfx_getFileData(entry->unk8, 0x1A), &work->unk30, NULL, k, 0, 1, k);
        }
        if (frame == 0x26) {
            p = ex->func_800E3CEC(ex->btlfx_getFileData(entry->unk8, 0x1D), (s32)&work->unk30);
            if (p != NULL) {
                p->unk22 = 0x80;
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
            t = ((frame - 0x28) << 12) >> 5;
            ex->func_800D23D8((u8 *)ex->btlfx_getFileData(entry->unk8, 0x12), &work->unk30, &work->unkBC,
                              (ex->math_cos(t) * 3) >> 1, 0, 1, ex->math_sin(t));
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
        ex->Hi_DrawSummonModel(&work->unk28, &work->unk10, (s32)&work->unk1C, SUMMON(env)->unk60, 1);
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
        func_801ED708(&work->unkA4, &work->unkBC);
        ex->Hi_DrawEffModel(&work->unkBC, &work->unkA4, (s32)&work->unkB0, work->unkA0);
        if (frame < 0x18 && frame != 1) {
            k = (frame << 10) / 24;
            s1.vx = s1.vy = ex->math_sin(k) / 2 + 0x800;
            s1.vz = 0x1000;
            r1.vx = work->unkBC.vx;
            r1.vz = frame << 5;
            r1.vy = work->unkBC.vy;
            ex->Hi_DrawEffModel(&r1, &work->unkA4, (s32)&s1, work->unkE0);
            ex->Hi_ModifyEffModelRGB(work->unkE0, ((ex->math_cos(k) - 0x800) >> 5) - 0x80,
                                     ((ex->math_cos(k) - 0x800) >> 5) - 0x80, ((ex->math_cos(k) - 0x800) >> 5) - 0x80);
        }
        ((s32 *)&a1)[1] = ((s32 *)&work->unk30)[1];
        ((s32 *)&a1)[0] = ((s32 *)&work->unk30)[0];
        ex->func_800CFD78(&a1.vx, &work->unkBC.vx, -(t + 0x1000), &b1.vx);
        for (bl = D_801F0D14, ang = work->unk4 * -0x80; bl < &D_801F0D14[3]; bl++, ang += 0x554) {
            g = ex->btlfx_randCentered(0x400) + 0xC00;
            func_801ECAF8(entry, bl, &a1, &b1, 0x7C, 0x200, ang, 0xC0, 0, g, 0x1D);
            for (j = 0; j < 4; j++) {
                func_801ECCF4(entry, bl, (Fx0e31Bolt *)((Fx0e31Bolt5 *)D_801F0A14 + j), g);
            }
        }
        if (frame < 0x21) {
            k = frame << 12;
            x = ex->math_cos(k >> 7);
            r2.vx = work->unkBC.vx;
            r2.vy = work->unkBC.vy;
            s2.vx = s2.vy = ((k >> 5) * 2) + 0x900;
            s2.vz = 0;
            r2.vz = work->unkBC.vz;
            r2.vz = (frame << 5) + 0x400;
            ex->func_800CFD78(&work->unk30.vx, &work->unkBC.vx, -0x80, &p2.vx);
            x = (x >> 5) - 0x80;
            v2.vx = p2.vx;
            v2.vy = p2.vy;
            v2.vz = p2.vz;
            for (i = 0; i < 3; i++) {
                ex->Hi_DrawEffModel(&r2, &v2, (s32)&s2, work->unkC4);
                ex->Hi_ModifyEffModelRGB(work->unkC4, x, x, x);
                s2.vx = s2.vy = (s2.vx * 3) >> 2;
                r2.vz += 0x550;
            }
        }
        env->unk28 = 0x190;
        if (frame < 0x10 && frame != 1) {
            x = (ex->math_cos(t >> 2) >> 2) + 0x555;
            y = ex->math_cos((frame << 8) >> 2);
            ex->func_800D23D8((u8 *)ex->btlfx_getFileData(entry->unk8, 0x1F), &work->unk30, NULL, x, 0, 1, y);
            work->unkBC.pad = 1;
            ex->func_800D23D8((u8 *)ex->btlfx_getFileData(entry->unk8, 0x14), &work->unk30, &work->unkBC, 0x1000,
                              frame, 1, y);
            ex->btlfx_drawSprites((u8 *)ex->btlfx_getFileData(entry->unk8, 0x18), &work->unk30, &work->unkBC, 0x2000,
                                  frame, 1, y);
            ex->func_800D3B6C((u8 *)ex->btlfx_getFileData(entry->unk8, 7), &work->unk30, frame - 1, y);
        }
        if (frame < 0x11) {
            x = (ex->math_cos(t >> 2) >> 2) + 0x800;
        } else {
            x = 0x800;
        }
        shake = t;
        ex->func_800D23D8((u8 *)ex->btlfx_getFileData(entry->unk8, 0x1C), &work->unk30, NULL, (x * 3) >> 1, 0, 1,
                          0x1000);
        k = t >> 2;
        x = ex->math_cos(k) + 0x1000;
        ex->func_800D23D8((u8 *)ex->btlfx_getFileData(entry->unk8, 0x1A), &work->unk30, NULL, x, 0, 1,
                          ex->math_cos(k));
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
            work->unkBC.vx = -0x400;
            work->unkBC.vy = 0;
            work->unkBC.vz = 0;
            work->unk30.vy = D_801F09B4.vy - 0x1000;
            work->unk30.vz = D_801F09B4.vz;
            work->unkA4.vx = D_801F09B4.vx;
            work->unkA4.vy = D_801F09B4.vy;
            work->unkB0.vx = 0x1200;
            work->unkB0.vy = 0x1200;
            work->unkB0.vz = 0x1555;
            work->unkA4.vz = D_801F09B4.vz;
            work->unkA4.vy -= 0x1000;
        }
        work->unk40.vy = work->unk30.vy;
        work->unk40.vx = work->unk30.vx;
        work->unk40.vz = work->unk30.vz;
        work->unk40.vy -= 0x1000;
        env->unk2C->unk0[3].vx = work->unk40.vx;
        env->unk2C->unk0[3].vy = work->unk40.vy;
        env->unk2C->unk0[3].vz = work->unk40.vz;
        func_801ED708(&work->unkA4, &work->unkBC);
        ex->Hi_DrawEffModel(&work->unkBC, &work->unkA4, (s32)&work->unkB0, work->unkA0);
        ((s32 *)&a2)[1] = ((s32 *)&work->unk30)[1];
        ((s32 *)&a2)[0] = ((s32 *)&work->unk30)[0];
        ex->func_800CFD78(&a2.vx, &work->unkBC.vx, -0x2800, &b2.vx);
        for (bl = D_801F0D14, ang = work->unk4 * -0x80; bl < &D_801F0D14[3]; bl++, ang += 0x554) {
            g = ex->btlfx_randCentered(0x400) + 0xC00;
            func_801ECAF8(entry, bl, &a2, &b2, 0x400, 0x528, ang, 0xC0, 0, g, 0x84);
            for (j = 0; j < 4; j++) {
                func_801ECCF4(entry, bl, (Fx0e31Bolt *)((Fx0e31Bolt5 *)D_801F0A14 + j), g);
            }
        }
        shake = 0x1000;
        if (frame >= 2) {
            work->unk0 = 4;
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
        } else if (frame - 4 < 0xB) {
            work->unkB0.vx = work->unkB0.vy = ((0x1000 - (((frame - 4) << 12) / 10)) * 3) >> 1;
        }
        if (frame < 5) {
            work->unk40.vz = work->unk30.vz;
            work->unk40.vy = work->unk30.vy;
            work->unk40.vx = work->unk30.vx;
            work->unk40.vy = work->unk40.vy - 0x180 - (((0x1000 - ((frame << 12) >> 2)) * 3) >> 1);
            env->unk2C->unk0[3].vx = work->unk40.vx;
            env->unk2C->unk0[3].vy = work->unk40.vy;
            env->unk2C->unk0[3].vz = work->unk40.vz;
        }
        if (frame == 4) {
            ex->btlfx_drawScreenTint((u32 *)(env->unk20 + 2), 1, 0x20, 0x80, 0xFF);
        } else if ((u32)(frame - 5) < 7) {
            k = 0x1000 - (((frame - 5) << 12) / 7);
            x = k >> 5;
            work->unk40.vx = D_801F09B4.vx + ex->btlfx_randCentered(x);
            work->unk40.vy = D_801F09B4.vy + ex->btlfx_randCentered(x);
            work->unk40.vy = D_801F09B4.vy + (k >> 6) * ((frame & 1) * 2 - 1) - 0x200;
            env->unk2C->unk0[3].vx = work->unk40.vx;
            env->unk2C->unk0[3].vy = work->unk40.vy;
            env->unk2C->unk0[3].vz = work->unk40.vz;
        }
        if (work->unkB0.vx != 0) {
            func_801ED708(&work->unkA4, &work->unkBC);
            ex->Hi_DrawEffModel(&work->unkBC, &work->unkA4, (s32)&work->unkB0, work->unkA0);
        }
        if (frame >= 4) {
            k = frame - 4;
            r3.vx = 0;
            r3.vz = 0;
            x = (k << 12) / 40;
            r3.vy = (k << 12) / 80;
            s3.vx = s3.vz = x + 0x555;
            s3.vy = ((k << 12) / 20) + 0x400;
            y = ex->math_cos(x);
            ex->Hi_DrawEffModel(&r3, &work->unkA4, (s32)&s3, work->unkC8);
            ex->Hi_ModifyEffModelRGB(work->unkC8, (y >> 5) - 0x80, (y >> 5) - 0x80, (y >> 5) - 0x80);
            s3.vx = s3.vz = s3.vx * 2;
            s3.vy >>= 2;
            ex->Hi_DrawEffModel(&r3, &work->unkA4, (s32)&s3, work->unkC8);
            ex->Hi_ModifyEffModelRGB(work->unkC8, (y >> 6) - 0x80, (y >> 6) - 0x80, (y >> 6) - 0x80);
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
        ((s32 *)&a4)[1] = ((s32 *)&work->unk30)[1];
        ((s32 *)&a4)[0] = ((s32 *)&work->unk30)[0];
        ex->func_800CFD78(&a4.vx, &work->unkBC.vx, t - 0x2800, &b4.vx);
        w = (t >> 2) + 0x400;
        for (i = 0, ang = work->unk4 << 8; i < 3; i++, ang += 0x554) {
            func_801ECAF8(entry, &D_801F0D14[i], &a4, &b4, w, w, ang, 0xC0, 0, ex->btlfx_randCentered(0x400) + 0xC00,
                          0x84);
        }
        env->unk28 = 0x200;
        k = t >> 2;
        x = (ex->math_sin(k) * 2) + 0x1000;
        y = (ex->math_cos(k) >> 1) + 0x800;
        ex->func_800D23D8((u8 *)ex->btlfx_getFileData(entry->unk8, 0x1C), &work->unk30, NULL, x, 0, 1, y);
        x = (ex->math_sin(k) * 3) + 0x1000;
        ex->func_800D23D8((u8 *)ex->btlfx_getFileData(entry->unk8, 0x1A), &work->unk30, NULL, x, 0, 1,
                          ex->math_cos(k));
        if (frame >= 4 && frame - 4 < 0xA) {
            r5.vx = 0x400;
            r5.vy = 0;
            r5.vz = 0;
            r5.pad = 1;
            ex->func_800D23D8((u8 *)ex->btlfx_getFileData(entry->unk8, 0x14), &work->unk30, &r5, 0x4000, frame - 4, 1,
                              0x1000);
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
            work->unkBC.vx = -0x400;
            work->unkBC.vy = 0;
            work->unkBC.vz = 0;
            work->unkB0.vx = 0x4000;
            work->unkB0.vy = 0x4000;
            work->unkB0.vz = 0x4000;
            work->unkA4.vz = D_801F09B4.vz;
        }
        work->unk40.vx = work->unk30.vx;
        work->unk40.vy = work->unk30.vy;
        work->unk40.vz = work->unk30.vz;
        work->unk40.vy = -0x2D4 - ((t * 0x76C) >> 12);
        env->unk2C->unk0[3].vx = work->unk40.vx;
        env->unk2C->unk0[3].vy = work->unk40.vy;
        env->unk2C->unk0[3].vz = work->unk40.vz;
        if (frame < 2) {
            ex->btlfx_drawScreenTint((u32 *)(env->unk20 + 2), 2, 0xFF - (frame << 7), 0xFF - (frame << 7), 0);
        } else if (frame == 2) {
            ex->btlfx_drawScreenTint((u32 *)(env->unk20 + 2), 1, 0xFF, 0xFF, 0);
        } else if (frame < 0x13) {
            x = ((0x1000 - (((frame - 3) << 12) >> 4)) * 0xA0) >> 12;
            ex->btlfx_drawScreenTint((u32 *)(env->unk20 + 2), 1, x, x, 0);
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
                x = 0x2000 - ex->math_sin((frame << 10) / 7);
            } else {
                x = 0x7800 - ((frame - 7) * 0x1124) / 65 * 7;
            }
            if ((u32)(frame - 5) < 7) {
                y = ex->math_sin(((frame - 5) << 11) >> 12) * 0xF5;
                ex->btlfx_drawScreenTint((u32 *)(env->unk20 + 0x400), 1, 0, y >> 14, y >> 12);
            }
            work->unkB0.vx = work->unkB0.vy = x;
            func_801ED708(&work->unkA4, &work->unkBC);
            work->unkBC.vz -= 0x400;
            ex->Hi_DrawEffModel(&work->unkBC, &work->unkA4, (s32)&work->unkB0, work->unkD0);
            ex->Hi_DrawEffModel(&work->unkBC, &work->unkA4, (s32)&work->unkB0, work->unkD4);
        }
        if (frame >= 0x3B) {
            work->unkB0.vx = work->unkB0.vy = 0x1000;
            func_801ED708(&work->unkA4, &work->unkBC);
            work->unkBC.vz += 0x400;
            ex->Hi_DrawEffModel(&work->unkBC, &work->unkA4, (s32)&work->unkB0, work->unkD8);
            k = ((frame - 0x3B) << 12) / 72;
            ex->Hi_ModifyEffModelRGB(work->unkD8, (ex->math_cos(k) >> 5) - 0x80, (ex->math_cos(k) >> 5) - 0x80,
                                     (ex->math_cos(k) >> 5) - 0x80);
        }
        k = t >> 2;
        z = (t >> 1) + 0xE00;
        y = ex->math_cos(k);
        ss.vx = 0x1000;
        ss.vy = 0x1000;
        ss.vz = 0x1000;
        g = (y >> 2) + 0x100;
        rr.vx = 0x400;
        rr.vy = 0;
        rr.vz = 0;
        n = (((t >> 3) + 0x500) * 4) / 3;
        y = ex->math_cos(k) >> 5;
        func_801ECFE4(&work->unk30, &rr, &ss, z, k + 0xA00, g >> 1, g, y, 0x1D, 0x3E81, 0, 0xC0, frame * -0x80, n, 1, 1);
        func_801ECFE4(&work->unk30, &rr, &ss, z, k + 0xA00, g >> 1, g, y, 0x1D, 0x3E81, 0, 0xC0, frame * 0xA0, n, 1, -1);
        if (frame < 0x41) {
            k = (frame << 6) >> 2;
            y = ex->math_cos(k);
            r6.vx = 0;
            r6.vz = 0;
            r6.vy = frame << 5;
            s6.vx = s6.vz = ((ex->math_sin(k) * 3) >> 2) + 0xA00;
            s6.vy = ((frame << 6) + 0x300) * 2;
            p6.vx = work->unk30.vx;
            p6.vy = work->unk30.vy;
            p6.vz = work->unk30.vz;
            p6.vy -= (ex->math_sin(k) * 0xFA0) >> 12;
            ex->Hi_DrawEffModel(&r6, &p6, (s32)&s6, work->unkCC);
            x = ((y - 0x5DC) >> 5) - 0x80;
            ex->Hi_ModifyEffModelRGB(work->unkCC, x, x, x);
        }
        r7.vx = 0;
        r7.vz = 0;
        r7.vy = frame << 6;
        w = 0x4000;
        p7.vx = work->unk30.vx;
        p7.vy = work->unk30.vy;
        p7.vz = work->unk30.vz;
        for (i = 0, j = 7; i < 6; i++, j += 7) {
            k = frame - j;
            if ((u32)k < 0x20) {
                y = ex->math_cos((k << 12) >> 7);
                s7.vx = s7.vz = w;
                if (k < 4) {
                    s7.vy = ex->math_sin((k << 10) >> 2) * 6;
                } else {
                    s7.vy = 0x6000 - ((k - 4) << 8);
                }
                s7.vy = (s7.vy * (8 - i)) >> 3;
                if (i < 2) {
                    s7.vy = (s7.vy * 5) >> 2;
                }
                ex->Hi_DrawEffModel(&r7, &p7, (s32)&s7, work->unkDC);
                ex->Hi_ModifyEffModelRGB(work->unkDC, (y >> 5) - 0x80, (y >> 5) - 0x80, (y >> 5) - 0x80);
            }
            w = (w * 9) >> 3;
            r7.vy = (r7.vy * 3) >> 2;
        }
        if (frame < 0x10) {
            k = frame << 12;
            r8.vy = frame * 0x37;
            r8.vx = 0;
            r8.vz = 0;
            s8.vx = s8.vz = (k >> 5) + 0x1200;
            k >>= 6;
            s8.vy = ex->math_sin(k) + 0x1800;
            y = ex->math_cos(k);
            p8.vx = work->unk30.vx;
            p8.vy = work->unk30.vy;
            p8.vz = work->unk30.vz;
            ex->Hi_DrawEffModel(&r8, &p8, (s32)&s8, work->unkC8);
            ex->Hi_ModifyEffModelRGB(work->unkC8, (y >> 5) - 0x80, (y >> 5) - 0x80, (y >> 5) - 0x80);
        }
        BTLFX_COPY_SVEC(&f9, &work->unk30);
        BTLFX_COPY_SVEC(&t9, &work->unk30);
        t9.vy -= 0x1800;
        x = 0x1000 - ((t * 0xDAC) >> 12);
        ang = (work->unk4 << 9) + (ex->btlfx_rand() & 0x1FF);
        y = (ex->math_cos(t >> 2) * 0xF5) >> 12;
        for (i = 0; i < 2; i++, ang += 0x800) {
            func_801ECAF8(entry, &D_801F0D14[i], &f9, &t9, (x * 3) >> 2, x, ang, 0xC0, 0, 0x1000, y);
        }
        if (frame < 0x26) {
            x = (x * 3) >> 1;
            func_801ECAF8(entry, &D_801F0D14[2], &f9, &t9, (x * 3) >> 2, x, -ang, 0xC0, 0, 0x1000,
                          (ex->math_cos(t >> 1) * 0xF5) >> 12);
        }
        x = 0x1000 - t;
        env->unk28 = 0x300;
        ang = work->unk4 << 7;
        y = ((ex->math_cos(t >> 2) * 0xEB) >> 12) + 0x20;
        for (i = 0; i < 5; i++) {
            f9.vx = work->unk30.vx;
            f9.vy = work->unk30.vy;
            f9.vz = work->unk30.vz;
            f9.vx += (ex->math_sin(ang) * x) >> 12;
            f9.vz += (ex->math_cos(ang) * x) >> 12;
            t9.vx = work->unk30.vx;
            t9.vy = work->unk30.vy;
            t9.vz = work->unk30.vz;
            t9.vx += ex->math_sin(ang);
            t9.vz += ex->math_cos(ang);
            t9.vy -= 0xF00;
            func_801ECDEC(entry, (Fx0e31Bolt *)&D_801F0A14[i], &t9, &f9, 0x80, 0x200, ang, 0xFF, 0,
                          ex->math_cos(t >> 2), y);
            ang = ang + 0x333 + ex->btlfx_randCentered(0x20);
        }
        env->unk28 = 0;
        if (frame == 0x31) {
            work->unk38.vx = work->unk30.vx;
            work->unk38.vy = work->unk30.vy;
            work->unk38.vz = work->unk30.vz;
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
        k = t >> 2;
        x = ex->math_cos(k) + 0x1800;
        y = ((ex->math_cos(k) * 3) >> 2) + 0x400;
        ex->func_800D23D8((u8 *)ex->btlfx_getFileData(entry->unk8, 0x1C), &work->unk30, NULL, x, 0, 1, y);
        ex->func_800D23D8((u8 *)ex->btlfx_getFileData(entry->unk8, 0x1B), &work->unk30, &r9, 0x5000 - t * 2, 0, 1,
                          ex->math_cos(k));
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
        k = work->unk4 << 8;
        ex->Hi_GetSummonBonePos(SUMMON(env)->unk60, ex->btlfx_rand() % 92, (u16 *)&from);
        ex->Hi_GetSummonBonePos(SUMMON(env)->unk60, ex->btlfx_rand() % 92, (u16 *)&to);
        func_801EC71C(entry, (Fx0e31Bolt *)D_801F0A14, &from, &to, a, a, k, 0x100, 0,
                      ex->btlfx_randCentered(0x400) + 0xC00);
    }
    if (fade != 0) {
        ex->Hi_DrawEffModel(&work->unk80, &work->unk88, (s32)&work->unk94, work->unk7C);
        ex->Hi_ModifyEffModelRGB(work->unk7C, (fade >> 5) - 0x80, (fade >> 5) - 0x80, (fade >> 5) - 0x80);
    }
    if (tint != 0) {
        ex->btlfx_drawScreenTint((u32 *)(env->unk20 + 0xFFE), 0xFF, 0, 0, (tint * 0x4D) >> 12);
    }
    if (shake != 0) {
        k = shake << 7;
        x = k >> 12;
        if (work->unk4 == work->unk4 / 6 * 6) {
            ex->btlfx_drawScreenTint((u32 *)(env->unk20 + 2), 1, 0, x * 2 / 3, x);
        }
        if (!((work->unk4 - 3) & 7)) {
            ex->btlfx_drawScreenTint((u32 *)(env->unk20 + 2), 2, k >> 13, k >> 13, 0);
        }
    }
    work->unk4++;
    return 0;
}
