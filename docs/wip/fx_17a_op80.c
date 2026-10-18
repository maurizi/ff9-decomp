/* Draft of fx_17a fx_17a_op80. Status: 113 lines, 43 ignoring registers (build/w/nd.py). */
#include "fxmod/fx_17a.h"

typedef struct {
    s32 vx;
    s32 vy;
    s32 vz;
} Fx17aVec3;

typedef struct {
    s32 unk0; /* state */
    s32 unk4; /* frame counter */
    u8 pad8[4];
    SVECTOR unkC; /* position */
    SVECTOR unk14;
    SVECTOR unk1C;
    SVECTOR unk24;
    u8 pad2C[8];
    Fx17aVec3 unk34; /* model position */
    Fx17aVec3 unk40; /* model scale */
    SVECTOR unk4C; /* model rotation */
    u8 pad54[0xC];
    Fx17aVec3 unk60;
    SVECTOR unk6C;
    u8 pad74[0xC];
    s32 unk80; /* models */
    s32 unk84;
    s32 unk88;
    s32 unk8C;
    SVECTOR unk90[4];
    Pool6CEntry *unkB0;
} Fx17aWork80; /* 0xB4 */

typedef struct {
    u8 pad0[4];
    SVECTOR unk4[0x40]; /* points; vy decays */
    u8 pad204[8];
} Fx17aRing; /* 0x20C */

extern SVECTOR D_801E9AC4;
extern u8 D_801E9ACC[];
extern Fx17aRing D_801E9B44[2];
extern BtlModel D_801E9F64[];

s32 fx_17a_op80(s32 mode, Fx17aWork80 *work, Unk800F78A4 *env, Unk80100C78Entry *entry, EffInit *init) {
    BtlfxExports *ex = BTLFX_EXPORTS(env);
    Pool6CEntry *p;
    Fx17aPart *q;
    s32 frame;
    s32 t;
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

    if (mode == 0) {
        init->unk0 = 0xB4;
        init->unk8 = 0x28;
        init->unk14 = (s32)func_801E7BC0;
        init->unk4 = 0x30;
    } else if (mode == 1) {
        work->unk0 = 2;
        work->unk4 = 0;
        D_801E9F5C = env;
        ex->btlfx_initEffModels(D_801E9F64, 6);
        work->unk80 = ex->Hi_RegisterTexEffModel((BtlUnit9C8 *)ex->btlfx_getFileData(entry->unk8, 0xA), 0xBA, 0x3FC0, 0, 0);
        work->unk84 = ex->Hi_RegisterTexEffModel((BtlUnit9C8 *)ex->btlfx_getFileData(entry->unk8, 7), 0xBA, 0x3EC0, 0, 0x80);
        work->unk88 = ex->Hi_RegisterTexEffModel((BtlUnit9C8 *)ex->btlfx_getFileData(entry->unk8, 8), 0x3A, 0x3E81, 0, 0x80);
        work->unk8C = ex->Hi_RegisterTexEffModel((BtlUnit9C8 *)ex->btlfx_getFileData(entry->unk8, 9), 0x138, 0xFFFF, 0, 0x80);
        BTLFX_COPY_SVEC(&work->unkC, &D_801E9AC4);
        work->unk4C.vx = 0;
        work->unk4C.vy = 0;
        work->unk4C.vz = 0;
        work->unk34.vx = work->unkC.vx;
        work->unk34.vy = work->unkC.vy;
        work->unk34.vz = work->unkC.vz;
        work->unk14.vx = work->unkC.vx;
        work->unk14.vy = work->unkC.vy;
        work->unk14.vz = work->unkC.vz - 0x2555;
        work->unk1C.vx = work->unkC.vx;
        work->unk1C.vy = work->unkC.vy;
        work->unk1C.vz = work->unkC.vz - 0x1C00;
    } else {
        glow = 0;
        g = 0;
        frame = entry->unk0;
        {
        switch (work->unk0) {
        case 2:
            env->unk2C->unk0[3].vx = work->unkC.vx;
            env->unk2C->unk0[3].vy = work->unkC.vy;
            env->unk2C->unk0[3].vz = work->unkC.vz;
            work->unk4C.vx += 0x20;
            work->unk4C.vy += 0x10;
            t = (frame << 12) / 18;
            work->unk40.vx = work->unk40.vy = work->unk40.vz = (0x1000 - t) * 2;
            glow = (t * 0xAA) >> 12;
            ex->Hi_DrawEffModel(&work->unk4C, (VECTOR *)&work->unk34, (s32)&work->unk40, work->unk80);
            ex->Hi_ModifyEffModelRGB(work->unk80, (ex->math_sin(t >> 2) >> 5) - 0x80, (ex->math_sin(t >> 2) >> 5) - 0x80,
                                     (ex->math_sin(t >> 2) >> 5) - 0x80);
            work->unk4C.vz += 0x400;
            ex->Hi_DrawEffModel(&work->unk4C, (VECTOR *)&work->unk34, (s32)&work->unk40, work->unk80);
            ex->Hi_ModifyEffModelRGB(work->unk80, (ex->math_sin(t >> 2) >> 5) - 0x80, (ex->math_sin(t >> 2) >> 5) - 0x80,
                                     (ex->math_sin(t >> 2) >> 5) - 0x80);
            work->unk4C.vz -= 0x400;
            env->unk28 = 0x100;
            f2 = ex->math_sin(t >> 2) * 2;
            ex->func_800D23D8((u8 *)ex->btlfx_getFileData(entry->unk8, 3), &work->unkC, NULL, f2, frame, 1, -1);
            env->unk28 = 0;
            if (frame >= 0x12) {
                work->unk0 = 3;
                entry->unk0 = -1;
                for (f3 = 0; f3 < 4; f3++) {
                    work->unk90[f3].vx = 0x800 - f3 * 0x180;
                    work->unk90[f3].vy = f3 * 0xC0;
                    work->unk90[f3].vz = f3 * 0x60;
                }
                p = ex->func_800E3CEC(ex->btlfx_getFileData(entry->unk8, 0x13), (s32)&work->unk1C);
                if (p != NULL) {
                    p->unk22 = 0x80;
                }
            }
            break;
        case 3: {
            VECTOR sc;

            glow = 0xAA;
            work->unk4C.vx += 0x20;
            work->unk4C.vy += 0x10;
            t = (frame << 12) / 24;
            g = (t >> 4) + 0x80;
            if (frame < 0xC) {
                b = 0x6AA;
            } else if (frame < 0x14) {
                b = 0x6AA - ((frame - 0xC) * 0x6AA) / 8;
            } else {
                b = 0;
            }
            if (b != 0) {
                work->unk40.vx = work->unk40.vy = work->unk40.vz = b;
                ex->Hi_DrawEffModel(&work->unk4C, (VECTOR *)&work->unk34, (s32)&work->unk40, work->unk80);
                work->unk4C.vz += 0x400;
                ex->Hi_DrawEffModel(&work->unk4C, (VECTOR *)&work->unk34, (s32)&work->unk40, work->unk80);
                work->unk4C.vz -= 0x400;
            }
            b = b * 5 + 0x1000;
            ex->func_800D23D8((u8 *)ex->btlfx_getFileData(entry->unk8, 3), &work->unkC, NULL, b, frame, 1, -1);
            if ((u32)(frame - 0xF) < 6) {
                b = 0x1000 - (((frame - 0xF) << 12) / 6);
                ex->func_800D23D8((u8 *)ex->btlfx_getFileData(entry->unk8, 1), &work->unk14, NULL, b, frame, 1, 0x1000);
            }
            if (frame >= 0xC) {
                r = ((frame - 0xC) << 10) / 12;
                b = ex->math_sin(r) * 2 + 0x2000;
                i = ex->math_sin(r);
                ex->func_800D23D8((u8 *)ex->btlfx_getFileData(entry->unk8, 4), &work->unk14, NULL, b, 0, 1, i);
            }
            if (frame >= 0xB) {
                x2 = ((frame - 0xA) << 12) / 14;
                r = x2 >> 2;
                sc.vx = sc.vy = sc.vz = (x2 * 0xB) / 12;
                for (a = 0, n = 0; a < 4; a++, n += 0x30) {
                    work->unk90[a].vz = (s16)(work->unk90[a].vz - 0x18) - n;
                    ex->Hi_DrawEffModel(&work->unk90[a], (VECTOR *)&work->unk34, (s32)&sc, work->unk84);
                    sc.vx = sc.vy = sc.vz = (sc.vx * 0x12) >> 4;
                    ex->Hi_ModifyEffModelRGB(work->unk84, ((ex->math_sin(r) * 2) / 3 >> 5) - 0x80,
                                             ((ex->math_sin(r) * 2) / 3 >> 5) - 0x80,
                                             ((ex->math_sin(r) * 2) / 3 >> 5) - 0x80);
                }
            }
            if (frame == 0xC) {
                p = ex->func_800E3CEC(ex->btlfx_getFileData(entry->unk8, 0xD), (s32)&work->unk14);
                if (p != NULL) {
                    p->unk22 = 0x80;
                }
            }
            i = (ex->math_sin(t >> 2) * 0x46) >> 12;
            ex->btlfx_drawScreenTint((u32 *)(env->unk20 + 1), 1, i >> 2, i >> 1, i);
            if (frame >= 0x15) {
                i = ((frame - 0x14) * 0xFF) / 4;
                ex->btlfx_drawScreenTint((u32 *)(env->unk20 + 1), 1, i, i, i);
            }
            if (frame >= 0x11) {
                r = frame - 0x14;
                i = ex->math_sin(r << 8);
                ex->func_800D3B6C((u8 *)ex->btlfx_getFileData(entry->unk8, 0xC), &work->unkC, r, i);
            }
            if (frame >= 0x18) {
                work->unk0 = 4;
                entry->unk0 = -1;
                BTLFX_COPY_SVEC(&work->unk24, &D_801E9AC4);
                work->unk24.vy += 0x400;
                env->unk2C->unk0[3].vx = work->unk24.vx;
                env->unk2C->unk0[3].vy = work->unk24.vy;
                env->unk2C->unk0[3].vz = work->unk24.vz;
                work->unk34.vx = D_801E9AC4.vx;
                work->unk34.vy = D_801E9AC4.vy;
                work->unk34.vz = D_801E9AC4.vz;
                BTLFX_COPY_SVEC(&work->unkC, &D_801E9AC4);
                BTLFX_COPY_SVEC(&work->unk14, &D_801E9AC4);
                p = work->unkB0 = ex->func_800E3CEC(ex->btlfx_getFileData(entry->unk8, 0xD), (s32)&work->unk14);
                if (p != NULL) {
                    p->unk22 = 0x80;
                }
            }
            break;
        }
        case 4:
            BTLFX_COPY_SVEC(&work->unk24, &D_801E9AC4);
            t = (frame << 12) / 10;
            work->unk24.vy += (0x1000 - t) >> 2;
            env->unk2C->unk0[3].vx = work->unk24.vx;
            glow = 0xF0;
            env->unk2C->unk0[3].vy = work->unk24.vy;
            g = 0x300;
            env->unk2C->unk0[3].vz = work->unk24.vz;
            work->unk4C.vx += 0x20;
            work->unk4C.vy += 0x10;
            if (frame < 0x10) {
                s32 v;

                n = (frame << 12) >> 4;
                c = ex->math_sin((frame << 12) >> 6) >> 1;
                work->unk40.vx = work->unk40.vy = work->unk40.vz = c;
                ex->Hi_DrawEffModel(&work->unk4C, (VECTOR *)&work->unk34, (s32)&work->unk40, work->unk80);
                v = ((0x1000 - n) >> 5) - 0x80;
                ex->Hi_ModifyEffModelRGB(work->unk80, v, v, v);
            }
            env->unk28 = 0x100;
            n = t >> 2;
            a = ex->math_cos(n);
            c = ex->math_sin(n) + 0x400;
            ex->func_800D23D8((u8 *)ex->btlfx_getFileData(entry->unk8, 3), &work->unkC, NULL, c, frame, 1, -1);
            ex->func_800D23D8((u8 *)ex->btlfx_getFileData(entry->unk8, 1), &work->unkC, NULL, c, frame, 1, a);
            env->unk28 = 0;
            if (frame < 6) {
                s32 a = 0xFF - ((frame * 0xFF) / 6);
                ex->btlfx_drawScreenTint((u32 *)(env->unk20 + 1), 1, a, a, a);
            }
            if (frame >= 7) {
                a = (frame - 7) * 0x55;
                ex->btlfx_drawScreenTint((u32 *)(env->unk20 + 1), 2, a, a, a);
            }
            if (frame >= 0xA) {
                work->unk0 = 5;
                entry->unk0 = -1;
                ex->func_800E3D54(work->unkB0);
                work->unk6C.vy = 0x400;
                work->unk4C.vy = 0x400;
                work->unk6C.vx = 0;
                work->unk6C.vz = 0;
                work->unk60.vx = 0x1000;
                work->unk60.vy = 0x1000;
                work->unk60.vz = 0x1000;
                work->unk4C.vx = 0;
                work->unk4C.vz = 0;
                work->unk40.vx = 0x333;
                work->unk40.vy = 0x333;
                work->unk40.vz = 0x333;
                work->unk34.vx = work->unkC.vx;
                work->unk34.vy = work->unkC.vy;
                work->unk34.vz = work->unkC.vz;
                p = ex->func_800E3CEC(ex->btlfx_getFileData(entry->unk8, 0x13), (s32)&work->unk14);
                if (p != NULL) {
                    p->unk22 = 0x80;
                }
                p = ex->func_800E3CEC(ex->btlfx_getFileData(entry->unk8, 0xF), (s32)&work->unk14);
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
            s32 u;
            s32 w;
            s32 h;

            u = frame << 12;
            env->unk28 = 0x100;
            t = u / 51;
            f5 = t >> 2;
            r = ex->math_cos(f5);
            c = ex->math_sin(f5) + 0xC00;
            ex->func_800D23D8((u8 *)ex->btlfx_getFileData(entry->unk8, 3), &work->unkC, NULL, c, frame, 1, 0xB86);
            ex->func_800D23D8((u8 *)ex->btlfx_getFileData(entry->unk8, 3), &work->unkC, NULL, c / 2, frame, 1, -1);
            ex->func_800D23D8((u8 *)ex->btlfx_getFileData(entry->unk8, 6), &work->unkC, NULL, (c * 2) / 3, frame, 1, r);
            c = ex->math_sin(f5) * 2 + 0x3000;
            ex->func_800D23D8((u8 *)ex->btlfx_getFileData(entry->unk8, 4), &work->unkC, NULL, c, 0, 1, -1);
            env->unk28 = 0;
            if (frame < 0x20) {
                f4 = u >> 7;
                c = (ex->math_sin(f4) * 3) >> 1;
                r = ex->math_cos(f4);
                dir.vx = 0x200;
                dir.vy = 0x280;
                dir.pad = 1;
                dir.vz = work->unk4 * 32;
                ex->func_800D23D8((u8 *)ex->btlfx_getFileData(entry->unk8, 5), &work->unkC, &dir, c, 0, 1, r);
                dir.vy += 0x800;
                ex->func_800D23D8((u8 *)ex->btlfx_getFileData(entry->unk8, 5), &work->unkC, &dir, c, 0, 1, r);
            }
            if (frame == 0) {
                ex->btlfx_drawScreenTint((u32 *)(env->unk20 + 1), 0xFF, 0xA0, 0, 0xFF);
            } else if (frame == 1) {
                ex->btlfx_drawScreenTint((u32 *)(env->unk20 + 1), 2, 0xFF, 0xFF, 0);
            } else if (frame < 6) {
                i = 0xFF - ((frame * 0xFF) / 6);
                ex->btlfx_drawScreenTint((u32 *)(env->unk20 + 1), 1, i >> 1, i >> 1, i);
            }
            work->unk6C.vz += 0x18;
            ex->func_800E651C(D_801E9ACC, &col, frame);
            f6 = t >> 2;
            func_801E7718(&work->unkC, &work->unk6C, (VECTOR *)&work->unk60, 0x1C00, 0x1800, &col, 0x17, 0x3E80, 0, 0,
                          ((ex->math_sin(f6) * 0x58) >> 12) + 0x40, 0xFF);
            ex->func_800E6624(work->unk4 * 7, 0x1C0, 0x100, 0x20, 0x80, 0x2A0, 0x100);
            if (frame < 0x19) {
                s32 x;

                work->unk4C.vz += 0xC;
                x = 0x1000 - ((frame << 12) / 25);
                ex->Hi_DrawEffModel(&work->unk4C, (VECTOR *)&work->unk34, (s32)&work->unk40, work->unk88);
                ex->Hi_ModifyEffModelRGB(work->unk88, ((ex->math_sin(x >> 2) - 0x800) >> 5) - 0x80,
                                         ((ex->math_sin(x >> 2) - 0x800) >> 5) - 0x80,
                                         ((ex->math_sin(x >> 2) - 0x800) >> 5) - 0x80);
            }
            rot.vx = 0x980;
            rot.vy = 0x400;
            rot.vz = 0;
            sc.vx = sc.vy = sc.vz = ex->math_sin(f6) + 0x800;
            ex->Hi_DrawEffModel(&rot, (VECTOR *)&work->unk34, (s32)&sc, work->unk8C);
            ex->Hi_ModifyEffModelRGB(work->unk8C, (((ex->math_cos(f6) >> 1) + 0x800) >> 5) - 0x80,
                                     (((ex->math_cos(f6) >> 1) + 0x800) >> 5) - 0x80,
                                     (((ex->math_cos(f6) >> 1) + 0x800) >> 5) - 0x80);
            ex->btlfx_drawBackdrop((BtlBgLayers *)ex->btlfx_getFileData(entry->unk8, 0x12), work->unk4);
            ex->func_800E6624(work->unk4 * 10, 0x2A0, 0x180, 0x10, 0x40, 0x2B0, 0x180);
            g2 = (ex->math_cos(f6) >> 1) + 0x800;
            if (frame < 0x18) {
                a = ((ex->math_sin((frame << 10) / 24) * 0x76C) >> 12) + 0x200;
            } else {
                a = 0x96C;
            }
            ang.vx = (ex->math_sin(t >> 2) * 2) / 3;
            ang.vz = 0;
            ang.vy = work->unk4 << 3;
            for (i = 0, f4 = work->unk4 * 0xC; i < 0x80; i++) {
                ex->func_800D044C(&work->unkC, (s32)&at, 0, (XY32 *)&ang, a, f4);
                f4 += 0x20;
                ex->btlfx_drawParticles((u8 *)ex->btlfx_getFileData(entry->unk8, 0xE), &at, NULL, 0x2800, 0, 1, g2, 0,
                                        0);
                ang.vx += 0x40;
                ang.vy -= 0x80;
            }
            h = (ex->math_cos(t >> 2) >> 1) + 0x1000;
            if (frame < 0x18) {
                w = (ex->math_sin((frame << 10) / 24) >> 2) + 0x6A4;
            } else {
                w = 0xAA4;
            }
            for (k = 0; k < 2; k++) {
                if (frame == 0) {
                    ex->btlfx_drawTrail((u8 *)ex->btlfx_getFileData(entry->unk8, 0x11), (s32)&work->unkC,
                                        (s32 *)&work->unkC, -1, h, ex->math_cos(t >> 2), 1, (u8 *)&D_801E9B44[k], 1);
                }
                for (b = 0; b < 0x40; b++) {
                    (&D_801E9B44[k].unk4[b])->vy -= 0x40;
                }
            }
            ang2.vx = ex->math_sin(work->unk4 << 7) >> 4;
            ang2.vz = 0;
            ang2.vy = work->unk4 * 0x52 - 0x180;
            for (k = 0, j = work->unk4 * 0x5C; k < 2; k++) {
                ex->func_800D044C(&work->unkC, (s32)&at2, 0, (XY32 *)&ang2, w, j);
                j += 0x800;
                ex->btlfx_drawTrail((u8 *)ex->btlfx_getFileData(entry->unk8, 0x11), (s32)&at2, (s32 *)&work->unkC, 0, h,
                                    (ex->math_sin(t >> 1) >> 1) + 0x60E, 1, (u8 *)&D_801E9B44[k], 1);
            }
            if (frame < 0x18) {
                k = ex->math_cos((frame << 10) / 24);
                ex->func_800D3B6C((u8 *)ex->btlfx_getFileData(entry->unk8, 0xB), &work->unkC, frame, k);
            }
            if (frame >= 8) {
                for (i = 0; i < 3; i++) {
                    q = ex->btlfx_poolSpawn((s16 *)entry->unk10);
                    if (q != NULL) {
                        q->unk0 = 4;
                        BTLFX_COPY_SVEC(&q->unkC, &work->unkC);
                        a = ex->btlfx_rand();
                        c = (ex->btlfx_rand() & 0x1FF) + 0x180;
                        q->unkC.vx = q->unkC.vx - 0x100 + ex->btlfx_randCentered(0x200);
                        q->unkC.vy += (ex->math_cos(a) * c) >> 12;
                        q->unkC.vz += (ex->math_sin(a) * c) >> 12;
                        q->unkA = ex->btlfx_rand() % 70;
                        q->unk4 = (ex->btlfx_rand() & 0xFFF) + 0x1000;
                    }
                }
            }
            if (frame == 0x2B) {
                p = ex->func_800E3CEC(ex->btlfx_getFileData(entry->unk8, 0xD), (s32)&work->unk14);
                if (p != NULL) {
                    p->unk22 = 0x80;
                }
            }
            if (frame >= 0x2C) {
                x2 = (frame - 0x2B) << 12;
                k = x2 >> 7;
                if (k >= 0x100) {
                    k = 0xFF;
                }
                rot2.vz = 0x20;
                sc2.vy = sc2.vx = (x2 >> 5) + 0x1000;
                sc2.vz = 0x1000;
                col2.r = col2.g = col2.b = k;
                rot2.vx = 0;
                rot2.vy = 0;
                ex->func_800E6764(2, 0xA0, 0x70, 0x4D, 0x37, 4, 4, 2, 2, &rot2, &sc2, &col2, 1, 0);
            }
            if (frame >= 0x33) {
                work->unk0 = 6;
                entry->unk0 = -1;
                ex->btlfx_poolClear(entry->unk10);
                p = ex->func_800E3CEC(ex->btlfx_getFileData(entry->unk8, 0xF), (s32)&D_801E9AC4);
                if (p != NULL) {
                    p->unk22 = 0x80;
                }
            }
            break;
        }
        case 6:
            BTLFX_COPY_SVEC(&work->unk24, &D_801E9AC4);
            env->unk2C->unk0[3].vx = work->unk24.vx;
            env->unk2C->unk0[3].vy = work->unk24.vy;
            t = frame << 8;
            env->unk2C->unk0[3].vz = work->unk24.vz;
            env->unk28 = 0x180;
            a = t >> 2;
            f1 = ex->math_cos(a) + 0x400;
            c = ex->math_cos(a);
            ex->func_800D23D8((u8 *)ex->btlfx_getFileData(entry->unk8, 3), &D_801E9AC4, NULL, f1, 0, 1, c);
            env->unk28 = 0;
            c = (ex->math_cos(a) * 0xFF) >> 12;
            ex->btlfx_drawScreenTint((u32 *)(env->unk20 + 1), 1, c, c, c);
            if (frame >= 0x10) {
                ex->btlfx_clearEffModels();
                return 1;
            }
            break;
        }
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
        work->unk4++;
    }
    return 0;
}
