/* Draft of fx_0fb_1 fx_0fb_1_op80. Status: 27 lines, 18 ignoring registers (build/w/nd.py, which now masks all |imm| >= 10000 data offsets). Cases 3-6 use fx_17a op80's final variable roles (src/fxmod/fx_17a); mode 0/1 are if/else-if branches with one return 0 (as fx_17a); further splits found by build/w/pgreedy.py + gen_regions.py. */
#include "fxmod/fx_0fb_1.h"

typedef struct {
    s32 vx, vy, vz;
} Fx0fb1V3;

typedef struct {
    s32 unk0; /* state */
    s32 unk4; /* frame counter */
    u8 pad8[4];
    SVECTOR unkC; /* position */
    SVECTOR unk14;
    Fx0fb1V3 unk1C; /* summon position */
    Fx0fb1V3 unk28; /* summon scale */
    SVECTOR unk34; /* summon rotation */
    SVECTOR unk3C; /* camera */
    u8 pad44[8];
    s32 unk4C[2]; /* models */
    Fx0fb1V3 unk54; /* model position */
    Fx0fb1V3 unk60; /* model scale */
    SVECTOR unk6C; /* model rotation */
    Fx0fb1V3 unk74;
    Fx0fb1V3 unk80;
    SVECTOR unk8C;
    Fx0fb1V3 unk94;
    s32 unkA0; /* models */
    s32 unkA4;
    s32 unkA8;
    s32 unkAC;
    s32 unkB0;
    s32 unkB4;
    s32 unkB8;
    s32 unkBC;
    s32 unkC0;
    SVECTOR unkC4[4];
    Pool6CEntry *unkE4;
} Fx0fb1Op80Work; /* 0xE8 */

typedef struct {
    u8 pad0[4];
    SVECTOR unk4[0x40];
    u8 pad204[8];
} Fx0fb1Ring; /* 0x20C */

typedef struct {
    u8 pad0[0x38];
    BtlModel *unk38;
    u8 pad3C[0x54 - 0x3C];
    struct {
        u8 pad0[0x18C];
        BtlUnit9CC *unk18C;
    } *unk54;
    u8 pad58[8];
    s32 unk60;
} Fx0fb1Summon;

extern SVECTOR D_801F082C;
extern u8 D_801F0834[];
extern s16 D_801F0864[2];
extern s16 D_801F0868[];
extern Fx0fb1Ring D_801F0914[2];
extern BtlModel D_801F0D3C[];
s32 func_801ECBE0();

s32 fx_0fb_1_op80(s32 mode, Fx0fb1Op80Work *work, Unk800F78A4 *env, Unk80100C78Entry *entry, EffInit *init) {
    BtlfxExports *ex = BTLFX_EXPORTS(env);
    Pool6CEntry *p;
    Fx0fb1Rock *q;
    s32 frame;
    s32 t;
    s32 s;
    s32 trail;
    s32 glow;
    s32 g;
    s32 c;
    s32 i;
    s32 j;
    s32 r;
    s32 a;
    s32 b;
    s32 g2;
    s32 k;
    s32 x2;
    s32 n;
    s32 f1;
    s32 f2;
    s32 f3;
    s32 f4;
    s32 f5;
    s32 f6;
    s32 c1;
    s32 ph;
    s32 m;
    s32 sp;
    s32 zz1;
    s32 zz2;
    s32 zz3;
    s32 zz4;
    s32 zz5;
    s32 zz6;
    Fx0fb1Rock *zz7;
    s32 zz8;
    s32 zz9;

    if (mode == 0) {
        init->unk0 = 0xE8;
        init->unk8 = 0x28;
        init->unk14 = (s32)func_801ECBE0;
        init->unk4 = 0x30;
    } else if (mode == 1) {
        work->unk0 = 0;
        work->unk4 = 0;
        D_801F0D2C = env;
        ex->btlfx_initEffModels(((Fx0fb1Summon *)env->unk4)->unk38, 0x1F);
        for (i = 0; i < 2; i++) {
            work->unk4C[i] = ex->Hi_RegisterTexEffModel((BtlUnit9C8 *)ex->btlfx_getFileData(entry->unk8, D_801F0864[i]),
                                                        0x9A, 0x3E00, 0, 0);
        }
        work->unkA0 = ex->Hi_RegisterTexEffModel((BtlUnit9C8 *)ex->btlfx_getFileData(entry->unk8, 0x82), 0x96, 0x3D40, 0, 0);
        work->unkA4 = ex->Hi_RegisterTexEffModel((BtlUnit9C8 *)ex->btlfx_getFileData(entry->unk8, 0x87), 0x9A, 0x3E00, 0, 0);
        work->unkA8 = ex->Hi_RegisterTexEffModel((BtlUnit9C8 *)ex->btlfx_getFileData(entry->unk8, 0x90), 0xBD, 0x3E40, 0, 0);
        work->unkAC = ex->Hi_RegisterTexEffModel((BtlUnit9C8 *)ex->btlfx_getFileData(entry->unk8, 0x8F), 0xBD, 0x3E40, 0, 0);
        work->unkB0 = ex->Hi_RegisterTexEffModel((BtlUnit9C8 *)ex->btlfx_getFileData(entry->unk8, 0x89), 0xBD, 0x3E40, 0, 0);
        work->unkB4 = ex->Hi_RegisterTexEffModel((BtlUnit9C8 *)ex->btlfx_getFileData(entry->unk8, 0x13), 0xBD, 0x3EC0, 0, 0x80);
        work->unkB8 = ex->Hi_RegisterTexEffModel((BtlUnit9C8 *)ex->btlfx_getFileData(entry->unk8, 0x23), 0x3D, 0x3E81, 0, 0x80);
        work->unkBC = ex->Hi_RegisterTexEffModel((BtlUnit9C8 *)ex->btlfx_getFileData(entry->unk8, 3), 0x138, 0xFFFF, 0, 0x80);
        work->unkC0 = ex->Hi_RegisterTexEffModel((BtlUnit9C8 *)ex->btlfx_getFileData(entry->unk8, 0x8C), 0x3A, 0x3D0A, 0, 0x80);
        work->unk8C.vz = 0;
        work->unk80.vz = 0x1000;
        work->unkC.vx = 0;
        work->unkC.vz = 0;
        work->unk74.vx = work->unkC.vx;
        work->unkC.vy = -0x1000;
        work->unk74.vx += 0x5000;
        work->unk74.vy = work->unkC.vy;
        work->unk8C.vx = -0x400;
        work->unk8C.vy = 0x200;
        work->unk80.vx = 0x1800;
        work->unk54.vy = work->unkC.vy;
        work->unk54.vx = work->unkC.vx;
        work->unk80.vy = 0x1800;
        work->unk60.vx = 0x400;
        work->unk60.vy = 0x400;
        work->unk74.vy += 0x4000;
        work->unk60.vz = 0x400;
        work->unk74.vz = work->unkC.vz;
        work->unk74.vz -= 0x3000;
        work->unk54.vz = work->unkC.vz;
        work->unk54.vy += 0x1000;
        work->unk54.vz -= 0x2000;
        {
            SVECTOR v;

            v.vx = work->unk54.vx;
            v.vy = work->unk54.vy;
            v.vz = work->unk54.vz;
            ex->btlfx_aimAngles((s16 *)&work->unkC, (s16 *)&v, (s16 *)&work->unk6C);
            work->unk6C.vz = -0x100;
            env->unk2C->unk0[3].vx = work->unk54.vx;
            env->unk2C->unk0[3].vy = work->unk54.vy;
            env->unk2C->unk0[3].vz = work->unk54.vz;
            BTLFX_COPY_SVEC(&work->unk3C, &work->unk54);
            D_801F0D30 = 0;
            D_801F0D38 = 0;
            D_801F0D34 = 0x1000;
        }
    } else {
    s = 0;
    trail = 0;
    glow = 0;
    g = 0;
    frame = entry->unk0;
    {
    switch (work->unk0) {
    case 0:
        if (frame == 0) {
            work->unk54.vy += 0x200;
            work->unk54.vz -= 0x400;
        }
        work->unk54.vy -= 0x20;
        work->unk54.vz += 0x40;
        ex->Hi_DrawEffModel(&work->unk6C, (VECTOR *)&work->unk54, (s32)&work->unk60, work->unkA8);
        work->unk6C.vz += 0x400;
        ex->Hi_DrawEffModel(&work->unk6C, (VECTOR *)&work->unk54, (s32)&work->unk60, work->unkA8);
        s = 0x1000;
        work->unk6C.vz -= 0x400;
        glow = 0x97;
        zz7 = ex->btlfx_poolSpawn((s16 *)entry->unk10);
        if (zz7 != NULL) {
            zz7->unk0 = frame % 3;
            zz7->unkC.vx = work->unk54.vx;
            zz7->unkC.vy = work->unk54.vy;
            zz7->unkC.vz = work->unk54.vz;
            if (zz7->unk0 == 2) {
                zz7->unk0 = 3;
                f4 = ex->btlfx_rand();
                zz7->unkC.vx += (ex->math_sin(f4) * 2) / 3;
                c = ex->math_cos(f4);
                zz7->unkC.vz += 0x800;
                zz7->unkC.vy = (s16)(zz7->unkC.vy - 0x400) - (c * 2) / 3;
            } else {
                zz7->unkC.vx += ex->btlfx_randCentered(0x800);
                zz7->unkC.vy = (s16)(zz7->unkC.vy - 0x400) - ex->btlfx_randCentered(0x800);
                zz7->unkC.vz += 0x800;
            }
            zz7->unkA = ex->btlfx_randCentered(0x800) + 0x1800;
        }
        if (frame >= 0x1C) {
            work->unk0 = 1;
            entry->unk0 = -1;
            ex->btlfx_poolClear(entry->unk10);
            ex->func_800E3C58();
            work->unk60.vx = 0x400;
            work->unk60.vy = 0x249;
            work->unk60.vz = 0x400;
            work->unk54.vx = work->unkC.vx;
            work->unk54.vy = work->unkC.vy;
            work->unk54.vz = work->unkC.vz;
            work->unk54.vy += 0xB84;
            work->unk54.vz -= 0x1708;
            for (i = 0, zz4 = 0; i < 0x19; i += 1 + (zz4 & 1)) {
                zz7 = ex->btlfx_poolSpawn((s16 *)entry->unk10);
                if (zz7 != NULL) {
                    s32 kk;

                    zz7->unk0 = i & 1;
                    zz7->unkC.vx = work->unkC.vx;
                    zz7->unkC.vy = work->unkC.vy;
                    zz7->unkC.vz = work->unkC.vz;
                    kk = i + 1;
                    zz7->unkC.vy -= D_801F0868[i * 2] * 2 - (kk %= 7) * 0x140;
                    zz7->unkC.vz -= (kk << 10) + ex->btlfx_randCentered(0x200);
                    zz7->unkC.vx -= D_801F0868[i * 2 + 1] * 2;
                    zz7->unkA = ex->btlfx_randCentered(0x800) + 0x1000;
                }
                zz4++;
            }
            env->unk2C->unk0[3].vx = work->unkC.vx;
            env->unk2C->unk0[3].vy = work->unkC.vy;
            D_801F0D38 = 1;
            env->unk2C->unk0[3].vz = work->unkC.vz;
            work->unk8C.vy = 0x800;
            work->unk8C.vx = 0;
            work->unk8C.vz = 0;
            work->unk74.vx = work->unkC.vx;
            work->unk74.vy = work->unkC.vy;
            work->unk74.vz = work->unkC.vz + 0x4000;
            work->unk74.vy += 0x2000;
        }
        break;
    case 1:
        glow = 0xAD;
        work->unk54.vy -= 0x96;
        work->unk54.vz += 0x12C;
        work->unk6C.vz += 0x10;
        t = (frame << 12) / 22;
        if (frame < 5) {
            ex->Hi_DrawEffModel(&work->unk6C, (VECTOR *)&work->unk54, (s32)&work->unk60, work->unkA8);
            work->unk6C.vz += 0x400;
            ex->Hi_DrawEffModel(&work->unk6C, (VECTOR *)&work->unk54, (s32)&work->unk60, work->unkA8);
            work->unk6C.vz -= 0x400;
        } else {
            s32 m;

            m = 0x1000 - ex->math_sin(((frame - 4) << 10) / 18);
            ex->Hi_DrawMorphEffModel(&work->unk6C, (VECTOR *)&work->unk54, (s32)&work->unk60, work->unkA8, work->unkAC, m);
            work->unk6C.vz += 0x400;
            ex->Hi_DrawMorphEffModel(&work->unk6C, (VECTOR *)&work->unk54, (s32)&work->unk60, work->unkA8, work->unkAC, m);
            work->unk6C.vz -= 0x400;
        }
        ex->btlfx_drawScreenTint((u32 *)(env->unk20 + 1), 1, t >> 8, t >> 7, t >> 6);
        if (frame >= 0x13) {
            s32 c1 = ((frame - 0x12) * 0xFF) >> 2;
            ex->btlfx_drawScreenTint((u32 *)(env->unk20 + 1), 1, c1, c1, c1);
        }
        if (t >= 0x800) {
            D_801F0D34 = 0x1000 - (t - 0x800) * 2;
        }
        if (frame >= 0x16) {
            work->unk0 = 2;
            entry->unk0 = -1;
            ex->btlfx_poolClear(entry->unk10);
            ex->func_800E3C58();
            work->unk54.vx = work->unkC.vx;
            work->unk6C.vx = 0;
            work->unk6C.vy = 0;
            work->unk6C.vz = 0;
            D_801F0D34 = 0x1000;
            work->unk34.vx = 0;
            work->unk94.vx = 0x800;
            work->unk34.vy = 0;
            work->unk34.vz = 0;
            work->unk28.vx = 0x1000;
            work->unk54.vy = work->unkC.vy;
            work->unk28.vy = 0x1000;
            work->unk54.vz = work->unkC.vz;
            work->unk14.vx = work->unkC.vx;
            work->unk94.vy = 0x800;
            work->unk94.vz = 0x800;
            work->unk14.vy = work->unkC.vy;
            work->unk14.vz = work->unkC.vz - 0x2555;
            work->unk1C.vz = work->unkC.vz - 0x2800;
            work->unk1C.vy = work->unkC.vy + 0x14C0;
            work->unk28.vz = 0x1000;
            work->unk14.vy += 0xCAC;
            ex->Hi_SetSummonMotion(((Fx0fb1Summon *)env->unk4)->unk54->unk18C, ((Fx0fb1Summon *)env->unk4)->unk60);
            ex->Hi_SetSummonMotFrame(((Fx0fb1Summon *)env->unk4)->unk60, 0);
        }
        break;
    case 2:
        t = (frame << 12) / 24;
        c = t >> 2;
        zz5 = ex->math_cos(c);
        env->unk2C->unk0[3].vx = work->unkC.vx;
        s = zz5;
        env->unk2C->unk0[3].vy = work->unkC.vy;
        glow = (t * 0xAA) >> 12;
        env->unk2C->unk0[3].vz = work->unkC.vz;
        env->unk2C->unk0[3].vy += ex->math_sin(c);
        trail = 0x1000;
        if (frame >= 4) {
            work->unk1C.vx = work->unkC.vx;
            ex->Hi_DrawSummonModel(&work->unk34, (VECTOR *)&work->unk1C, (s32)&work->unk28,
                                   ((Fx0fb1Summon *)env->unk4)->unk60, 1);
            work->unk1C.vy += 0x10E;
            ex->Hi_DrawEffModel(&work->unk34, (VECTOR *)&work->unk1C, (s32)&work->unk94, work->unkA4);
            work->unk1C.vy -= 0x10E;
        }
        work->unk6C.vx += 0x20;
        work->unk6C.vy += 0x10;
        work->unk60.vx = work->unk60.vy = work->unk60.vz = 0xAAA - t / 4;
        ex->Hi_DrawEffModel(&work->unk6C, (VECTOR *)&work->unk54, (s32)&work->unk60, work->unkB0);
        work->unk6C.vz += 0x400;
        ex->Hi_DrawEffModel(&work->unk6C, (VECTOR *)&work->unk54, (s32)&work->unk60, work->unkB0);
        work->unk6C.vz -= 0x400;
        zz5 = ex->math_sin(c) + 0x2000;
        ex->func_800D23D8((u8 *)ex->btlfx_getFileData(entry->unk8, 8), &work->unkC, NULL, zz5, frame, 1, -1);
        if (frame < 4) {
            s32 zz5 = 0xFF - ((frame * 0xFF) >> 2);
            ex->btlfx_drawScreenTint((u32 *)(env->unk20 + 1), 1, zz5, zz5, zz5);
        }
        if (frame >= 0x18) {
            work->unk0 = 3;
            D_801F0D30 = 1;
            entry->unk0 = -1;
            for (zz6 = 0; zz6 < 4; zz6++) {
                work->unkC4[zz6].vx = 0x800 - zz6 * 0x180;
                work->unkC4[zz6].vy = zz6 * 0xC0;
                work->unkC4[zz6].vz = zz6 * 0x60;
            }
        }
        break;
    case 3: {
        VECTOR sc;
        s32 ca;
        s32 cb;

        zz1 = (frame << 12) / 37;
        trail = ex->math_cos(zz1 >> 2);
        g = (zz1 >> 4) + 0x80;
        ex->Hi_DrawSummonModel(&work->unk34, (VECTOR *)&work->unk1C, (s32)&work->unk28, ((Fx0fb1Summon *)env->unk4)->unk60, 0);
        ca = ((0x1000 - (zz1 * 2) / 3) >> 5) - 0x80;
        ex->Hi_ModifySummonModelRGB(((Fx0fb1Summon *)env->unk4)->unk60, ca, ca, ca);
        work->unk1C.vy += 0x10E;
        ex->Hi_DrawEffModel(&work->unk34, (VECTOR *)&work->unk1C, (s32)&work->unk94, work->unkA4);
        cb = ((0x1000 - zz1) >> 5) - 0x80;
        ex->Hi_ModifyEffModelRGB(work->unkA4, cb, cb, cb);
        glow = 0xAA;
        work->unk1C.vy -= 0x10E;
        work->unk6C.vx += 0x20;
        work->unk6C.vy += 0x10;
        if (frame < 0xF) {
            b = 0x6AA;
        } else if (frame < 0x17) {
            b = 0x6AA - ((frame - 0xF) * 0x6AA) / 8;
        } else {
            b = 0;
        }
        if (b != 0) {
            work->unk60.vx = work->unk60.vy = work->unk60.vz = b;
            ex->Hi_DrawEffModel(&work->unk6C, (VECTOR *)&work->unk54, (s32)&work->unk60, work->unkB0);
            work->unk6C.vz += 0x400;
            ex->Hi_DrawEffModel(&work->unk6C, (VECTOR *)&work->unk54, (s32)&work->unk60, work->unkB0);
            work->unk6C.vz -= 0x400;
        }
        b = b * 5 + 0x1000;
        ex->func_800D23D8((u8 *)ex->btlfx_getFileData(entry->unk8, 8), &work->unkC, NULL, b, frame, 1, -1);
        if ((u32)(frame - 0xF) < 6) {
            b = 0x1000 - (((frame - 0xF) << 12) / 6);
            ex->func_800D23D8((u8 *)ex->btlfx_getFileData(entry->unk8, 0xF), &work->unk14, NULL, b, frame, 1, 0x1000);
        }
        if (frame >= 0x11) {
            ph = ((frame - 0x11) << 10) / 20;
            b = ex->math_sin(ph) * 2 + 0x2000;
            k = ex->math_sin(ph);
            ex->func_800D23D8((u8 *)ex->btlfx_getFileData(entry->unk8, 0x1A), &work->unk14, NULL, b, 0, 1, k);
        }
        if (frame >= 0x15) {
            zz2 = ((frame - 0x14) << 12) / 17;
            r = zz2 >> 2;
            sc.vx = sc.vy = sc.vz = zz2;
            for (b = 0; b < 4; b++) {
                work->unkC4[b].vz = (s16)(work->unkC4[b].vz - 0x18) - b * 0x30;
                ex->Hi_DrawEffModel(&work->unkC4[b], (VECTOR *)&work->unk54, (s32)&sc, work->unkB4);
                sc.vx = sc.vy = sc.vz = (sc.vx * 0x12) >> 4;
                ex->Hi_ModifyEffModelRGB(work->unkB4, ((ex->math_sin(r) * 2) / 3 >> 5) - 0x80,
                                         ((ex->math_sin(r) * 2) / 3 >> 5) - 0x80,
                                         ((ex->math_sin(r) * 2) / 3 >> 5) - 0x80);
            }
        }
        if (frame == 0x18) {
            p = ex->func_800E3CEC(ex->btlfx_getFileData(entry->unk8, 0x15), (s32)&work->unk14);
            if (p != NULL) {
                p->unk22 = 0x80;
            }
        }
        k = (ex->math_sin(zz1 >> 2) * 0x46) >> 12;
        ex->btlfx_drawScreenTint((u32 *)(env->unk20 + 1), 1, k >> 2, k >> 1, k);
        if (frame >= 0x22) {
            k = ((frame - 0x21) * 0xFF) / 4;
            ex->btlfx_drawScreenTint((u32 *)(env->unk20 + 1), 1, k, k, k);
        }
        if (frame >= 0x15) {
            zz9 = frame - 0x14;
            k = ex->math_sin((zz9 << 10) / 17);
            ex->func_800D3B6C((u8 *)ex->btlfx_getFileData(entry->unk8, 0xC), &work->unkC, zz9, k);
        }
        if (frame >= 0x25) {
            work->unk0 = 4;
            entry->unk0 = -1;
            BTLFX_COPY_SVEC(&work->unk3C, &D_801F082C);
            work->unk3C.vy += 0x400;
            env->unk2C->unk0[3].vx = work->unk3C.vx;
            env->unk2C->unk0[3].vy = work->unk3C.vy;
            env->unk2C->unk0[3].vz = work->unk3C.vz;
            work->unk54.vx = D_801F082C.vx;
            work->unk54.vy = D_801F082C.vy;
            work->unk54.vz = D_801F082C.vz;
            BTLFX_COPY_SVEC(&work->unkC, &D_801F082C);
            BTLFX_COPY_SVEC(&work->unk14, &D_801F082C);
            p = work->unkE4 = ex->func_800E3CEC(ex->btlfx_getFileData(entry->unk8, 0x15), (s32)&work->unk14);
            if (p != NULL) {
                p->unk22 = 0x80;
            }
        }
        break;
    }
    case 4:
        BTLFX_COPY_SVEC(&work->unk3C, &D_801F082C);
        t = (frame << 12) / 10;
        work->unk3C.vy += (0x1000 - t) >> 2;
        env->unk2C->unk0[3].vx = work->unk3C.vx;
        glow = 0xF0;
        env->unk2C->unk0[3].vy = work->unk3C.vy;
        g = 0x300;
        env->unk2C->unk0[3].vz = work->unk3C.vz;
        work->unk6C.vx += 0x20;
        work->unk6C.vy += 0x10;
        if (frame < 0x10) {
            s32 y;
            s32 v;

            c = (frame << 12) >> 4;
            a = ex->math_sin((frame << 12) >> 6) >> 1;
            work->unk60.vx = work->unk60.vy = work->unk60.vz = a;
            ex->Hi_DrawEffModel(&work->unk6C, (VECTOR *)&work->unk54, (s32)&work->unk60, work->unkB0);
            v = ((0x1000 - c) >> 5) - 0x80;
            ex->Hi_ModifyEffModelRGB(work->unkB0, v, v, v);
        }
        env->unk28 = 0x100;
        c = ex->math_cos(t >> 2);
        a = ex->math_sin(t >> 2) + 0x400;
        ex->func_800D23D8((u8 *)ex->btlfx_getFileData(entry->unk8, 8), &work->unkC, NULL, a, frame, 1, -1);
        ex->func_800D23D8((u8 *)ex->btlfx_getFileData(entry->unk8, 0xF), &work->unkC, NULL, a, frame, 1, c);
        env->unk28 = 0;
        if (frame < 6) {
            c1 = 0xFF - ((frame * 0xFF) / 6);
            ex->btlfx_drawScreenTint((u32 *)(env->unk20 + 1), 1, c1, c1, c1);
        }
        if (frame >= 7) {
            c1 = (frame - 7) * 0x55;
            ex->btlfx_drawScreenTint((u32 *)(env->unk20 + 1), 2, c1, c1, c1);
        }
        if (frame >= 0xA) {
            work->unk0 = 5;
            entry->unk0 = -1;
            ex->func_800E3D54(work->unkE4);
            work->unk8C.vy = 0x400;
            work->unk6C.vy = 0x400;
            work->unk8C.vx = 0;
            work->unk8C.vz = 0;
            work->unk80.vx = 0x1000;
            work->unk80.vy = 0x1000;
            work->unk80.vz = 0x1000;
            work->unk6C.vx = 0;
            work->unk6C.vz = 0;
            work->unk60.vx = 0x333;
            work->unk60.vy = 0x333;
            work->unk60.vz = 0x333;
            work->unk54.vx = work->unkC.vx;
            work->unk54.vy = work->unkC.vy;
            work->unk54.vz = work->unkC.vz;
            p = ex->func_800E3CEC(ex->btlfx_getFileData(entry->unk8, 2), (s32)&work->unk14);
            if (p != NULL) {
                p->unk22 = 0x80;
            }
            p = ex->func_800E3CEC(ex->btlfx_getFileData(entry->unk8, 5), (s32)&work->unk14);
            if (p != NULL) {
                p->unk22 = 0x80;
            }
            ex->btlfx_drawScreenTint((u32 *)(env->unk20 + 1), 2, 0xFF, 0xFF, 0xFF);
        }
        break;
    case 5: {
        SVECTOR dir;
        CVECTOR col;
        SVECTOR rot;
        VECTOR sc;
        SVECTOR ang;
        SVECTOR at;
        SVECTOR ang2;
        SVECTOR at2;
        CVECTOR col2;
        SVECTOR rot2;
        VECTOR sc2;
        s32 w;

        t = frame << 6;
        env->unk28 = 0x100;
        f5 = t >> 2;
        r = ex->math_cos(f5);
        a = ex->math_sin(f5) + 0xC00;
        ex->func_800D23D8((u8 *)ex->btlfx_getFileData(entry->unk8, 8), &work->unkC, NULL, a, frame, 1, 0xB86);
        ex->func_800D23D8((u8 *)ex->btlfx_getFileData(entry->unk8, 8), &work->unkC, NULL, a / 2, frame, 1, -1);
        ex->func_800D23D8((u8 *)ex->btlfx_getFileData(entry->unk8, 6), &work->unkC, NULL, (a * 2) / 3, frame, 1, r);
        a = ex->math_sin(f5) * 2 + 0x3000;
        ex->func_800D23D8((u8 *)ex->btlfx_getFileData(entry->unk8, 0x1A), &work->unkC, NULL, a, 0, 1, -1);
        env->unk28 = 0;
        if (frame < 0x20) {
            f4 = frame << 12;
            f4 >>= 7;
            a = (ex->math_sin(f4) * 3) >> 1;
            n = ex->math_cos(f4);
            dir.vx = 0x200;
            dir.vy = 0x280;
            dir.vz = work->unk4 * 32;
            dir.pad = 1;
            ex->func_800D23D8((u8 *)ex->btlfx_getFileData(entry->unk8, 0xD), &work->unkC, &dir, a, 0, 1, n);
            dir.vy += 0x800;
            ex->func_800D23D8((u8 *)ex->btlfx_getFileData(entry->unk8, 0xD), &work->unkC, &dir, a, 0, 1, n);
        }
        if (frame == 0) {
            ex->btlfx_drawScreenTint((u32 *)(env->unk20 + 1), 0xFF, 0xA0, 0, 0xFF);
        } else if (frame == 1) {
            ex->btlfx_drawScreenTint((u32 *)(env->unk20 + 1), 2, 0xFF, 0xFF, 0);
        } else if (frame < 6) {
            zz8 = 0xFF - ((frame * 0xFF) / 6);
            ex->btlfx_drawScreenTint((u32 *)(env->unk20 + 1), 1, zz8 >> 1, zz8 >> 1, zz8);
        }
        work->unk8C.vz += 0x18;
        ex->func_800E651C(D_801F0834, &col, frame);
        f6 = t >> 2;
        func_801EC738(&work->unkC, &work->unk8C, (VECTOR *)&work->unk80, 0x1C00, 0x1800, &col, 0x1B, 0x3E80, 0, 0,
                      ((ex->math_sin(f6) * 0x58) >> 12) + 0x40, 0xFF);
        ex->func_800E6624(work->unk4 * 7, 0x2C0, 0x100, 0x20, 0x80, 0x360, 0x100);
        if (frame < 0x20) {
            work->unk6C.vz += 0xC;
            c = 0x1000 - (frame << 7);
            ex->Hi_DrawEffModel(&work->unk6C, (VECTOR *)&work->unk54, (s32)&work->unk60, work->unkB8);
            ex->Hi_ModifyEffModelRGB(work->unkB8, ((ex->math_sin(c >> 2) - 0x800) >> 5) - 0x80,
                                     ((ex->math_sin(c >> 2) - 0x800) >> 5) - 0x80,
                                     ((ex->math_sin(c >> 2) - 0x800) >> 5) - 0x80);
        }
        rot.vx = 0x980;
        rot.vy = 0x400;
        rot.vz = 0;
        sc.vx = sc.vy = sc.vz = ex->math_sin(f6) + 0x800;
        ex->Hi_DrawEffModel(&rot, (VECTOR *)&work->unk54, (s32)&sc, work->unkBC);
        ex->Hi_ModifyEffModelRGB(work->unkBC, (((ex->math_cos(f6) >> 1) + 0x800) >> 5) - 0x80,
                                 (((ex->math_cos(f6) >> 1) + 0x800) >> 5) - 0x80,
                                 (((ex->math_cos(f6) >> 1) + 0x800) >> 5) - 0x80);
        ex->btlfx_drawBackdrop((BtlBgLayers *)ex->btlfx_getFileData(entry->unk8, 0x11), work->unk4);
        ex->func_800E6624(work->unk4 * 10, 0x360, 0x180, 0x10, 0x40, 0x370, 0x180);
        g2 = (ex->math_cos(f6) >> 1) + 0x800;
        if (frame < 0x18) {
            c = ((ex->math_sin((frame << 10) / 24) * 0x76C) >> 12) + 0x200;
        } else {
            c = 0x96C;
        }
        ang.vx = (ex->math_sin(t >> 2) * 2) / 3;
        ang.vy = work->unk4 << 3;
        ang.vz = 0;
        for (zz8 = 0, f4 = work->unk4 * 0xC; zz8 < 0x80; zz8++, f4 += 0x20) {
            ex->func_800D044C(&work->unkC, (s32)&at, 0, (XY32 *)&ang, c, f4);
            ex->btlfx_drawParticles((u8 *)ex->btlfx_getFileData(entry->unk8, 7), &at, NULL, 0x260E, 0, 1, g2, 0, 0);
            ang.vx += 0x40;
            ang.vy -= 0x80;
        }
        f3 = (ex->math_cos(t >> 2) >> 1) + 0x1000;
        if (frame < 0x18) {
            w = (ex->math_sin((frame << 10) / 24) >> 2) + 0x6A4;
        } else {
            w = 0xAA4;
        }
        for (m = 0; m < 2; m++) {
            if (frame == 0) {
                ex->btlfx_drawTrail((u8 *)ex->btlfx_getFileData(entry->unk8, 0x14), (s32)&work->unkC, (s32 *)&work->unkC,
                                    -1, f3, ex->math_cos(t >> 2), 1, (u8 *)&D_801F0914[m], 1);
            }
            {
                SVECTOR *pp = D_801F0914[m].unk4;

                for (k = 0; k < 0x40; k++) {
                    pp->vy -= 0x40;
                    pp++;
                }
            }
        }
        ang2.vx = ex->math_sin(work->unk4 << 7) >> 4;
        ang2.vy = work->unk4 * 0x52 - 0x180;
        ang2.vz = 0;
        for (k = 0, j = work->unk4 * 0x5C; k < 2; k++, j += 0x800) {
            ex->func_800D044C(&work->unkC, (s32)&at2, 0, (XY32 *)&ang2, w, j);
            ex->btlfx_drawTrail((u8 *)ex->btlfx_getFileData(entry->unk8, 0x14), (s32)&at2, (s32 *)&work->unkC, 0, f3,
                                (ex->math_sin(t >> 1) >> 1) + 0x800, 1, (u8 *)&D_801F0914[k], 1);
        }
        if (frame < 0x18) {
            k = ex->math_cos((frame << 10) / 24);
            ex->func_800D3B6C((u8 *)ex->btlfx_getFileData(entry->unk8, 0x17), &work->unkC, frame, k);
        }
        if (frame >= 8) {
            for (sp = 0; sp < 3; sp++) {
                q = ex->btlfx_poolSpawn((s16 *)entry->unk10);
                if (q != NULL) {
                    s32 d;
                    s32 e;
                    q->unk0 = 4;
                    BTLFX_COPY_SVEC(&q->unkC, &work->unkC);
                    e = ex->btlfx_rand();
                    d = (ex->btlfx_rand() & 0x1FF) + 0x180;
                    q->unkC.vx = (s16)(q->unkC.vx - 0x100) + ex->btlfx_randCentered(0x200);
                    q->unkC.vy += (ex->math_cos(e) * d) >> 12;
                    q->unkC.vz += (ex->math_sin(e) * d) >> 12;
                    q->unkA = ex->btlfx_rand() % 70;
                    q->unk4 = (ex->btlfx_rand() & 0xFFF) + 0x1000;
                }
            }
        }
        if (frame == 0x38) {
            p = ex->func_800E3CEC(ex->btlfx_getFileData(entry->unk8, 0x15), (s32)&work->unk14);
            if (p != NULL) {
                p->unk22 = 0x80;
            }
        }
        if (frame >= 0x39) {
            x2 = (frame - 0x38) << 12;
            n = x2 >> 7;
            if (n >= 0x100) {
                n = 0xFF;
            }
            rot2.vz = 0x20;
            sc2.vy = sc2.vx = (x2 >> 5) + 0x1000;
            sc2.vz = 0x1000;
            col2.r = col2.g = col2.b = n;
            rot2.vx = 0;
            rot2.vy = 0;
            ex->func_800E6764(2, 0xA0, 0x70, 0x4D, 0x37, 4, 4, 2, 2, &rot2, &sc2, &col2, 1, 0);
        }
        if (frame >= 0x40) {
            work->unk0 = 6;
            entry->unk0 = -1;
            ex->btlfx_poolClear(entry->unk10);
            p = ex->func_800E3CEC(ex->btlfx_getFileData(entry->unk8, 5), (s32)&D_801F082C);
            if (p != NULL) {
                p->unk22 = 0x80;
            }
        }
        break;
    }
    case 6:
        BTLFX_COPY_SVEC(&work->unk3C, &D_801F082C);
        env->unk2C->unk0[3].vx = work->unk3C.vx;
        env->unk2C->unk0[3].vy = work->unk3C.vy;
        t = frame << 8;
        env->unk2C->unk0[3].vz = work->unk3C.vz;
        env->unk28 = 0x180;
        f1 = ex->math_cos(t >> 2) + 0x400;
        c = ex->math_cos(t >> 2);
        ex->func_800D23D8((u8 *)ex->btlfx_getFileData(entry->unk8, 8), &D_801F082C, NULL, f1, 0, 1, c);
        env->unk28 = 0;
        c = (ex->math_cos(t >> 2) * 0xFF) >> 12;
        ex->btlfx_drawScreenTint((u32 *)(env->unk20 + 1), 1, c, c, c);
        if (frame >= 0x10) {
            ex->btlfx_clearEffModels();
            return 1;
        }
        break;
    }
    }
    if (s != 0) {
        if (work->unk0 == 0) {
            work->unk8C.vz += work->unk4 + 0x26;
        } else {
            work->unk8C.vz += 8;
        }
        ex->Hi_DrawEffModel(&work->unk8C, (VECTOR *)&work->unk74, (s32)&work->unk80, work->unkA0);
        if (s != 0x1000) {
            s32 v = (s >> 5) - 0x80;

            ex->Hi_ModifyEffModelRGB(work->unkA0, v, v, v);
        }
    }
    if (trail != 0) {
        if (work->unk0 == 0) {
            f2 = (10000 - (work->unk4 << 6)) % 240;
        } else {
            f2 = (work->unk4 * 0xE) % 240;
        }
        ex->func_800D1BC4(8, (u8 *)ex->btlfx_getFileData(entry->unk8, 1), 0xA0 - f2, 0xA0, 0, 3, trail);
        ex->func_800D1BC4(8, (u8 *)ex->btlfx_getFileData(entry->unk8, 1), 0x280 - f2, 0xA0, 0, 3, trail);
    }
    if (glow != 0) {
        CVECTOR col;
        SVECTOR rot;
        VECTOR sc;

        col.r = col.g = col.b = glow;
        rot.vx = 0;
        rot.vy = 0;
        rot.vz = 0;
        sc.vy = sc.vx = g + 0x1020;
        sc.vz = 0x1000;
        ex->func_800E6764(0x200, 0xA0, 0x70, 0x4D, 0x37, 4, 4, 2, 2, &rot, &sc, &col, 0, 0);
    }
    if (work->unk0 < 2) {
        ex->func_800E6624(work->unk4 * 5, 0x2A0, 0x180, 0x10, 0x40, 0x2B0, 0x180);
    }
    work->unk4++;
    }
    return 0;
}
