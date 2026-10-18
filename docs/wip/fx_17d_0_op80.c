/* Draft of fx_17d_0 op80 (summon: four states, the piece models, the ring, the body). Status: 275 aligned
 * mismatches, about 40 of them only the spill register (t1 here, t0 in the original). The original's constant 1
 * from mode == 1 is spilled and rematerialised (it also feeds case 1's dispatch compare, case 0xA's and case 0's
 * last func_801E79E8 argument and the * 12's sllv); here it gets s1 and case 0's argument reads abr. Other open
 * points: the file pointer f lands in t0 (a3 in the original) and the two DrawMorph calls of case 2 are not
 * cross-jumped. Levers that got here: SVECTOR rot declared at the switch-body top (shared by cases 0xA and 0, freed
 * at the switch end so the final blocks reuse 0x38..), one loop counter per loop, code before abr before the * 12
 * (gives the sllv), frame % 10, A - (C + B) / A - (B - C) to beat fold's reassociation, (x / 7) >> 2 instead of
 * x / 28, pointer-reassign reads *(f += 2 + n * 3), one file-pointer variable. */
#include "fxmod/fx_17d_0.h"

typedef struct {
    s32 vx;
    s32 vy;
    s32 vz;
} Fx17d0Vec3;

typedef struct {
    s32 unk0; /* state */
    s32 unk4; /* frame */
    SVECTOR unk8; /* summon position */
    SVECTOR unk10; /* pieces origin */
    Fx17d0Vec3 unk18; /* model position */
    u8 pad24[0x30 - 0x24];
    SVECTOR unk30;
    SVECTOR unk38; /* model rotation */
    SVECTOR unk40; /* spawn position */
    SVECTOR unk48; /* camera */
    u8 pad50[0x58 - 0x50];
    s32 unk58; /* spark count */
    s16 unk5C[21]; /* piece models */
    s16 unk86[21]; /* piece morph models */
    s16 unkB0[5]; /* body models */
    s16 unkBA; /* model */
} Fx17d0Work80; /* 0xBC */

typedef struct {
    u8 pad0[0x18];
    u16 unk18[6];
    u16 unk24[6];
    u16 unk30[6];
} Fx17d0SummonData;

typedef struct {
    u8 pad0[0x54];
    Fx17d0SummonData *unk54;
} Fx17d0Summon;

extern SVECTOR D_801E9368;
extern s16 D_801E9418[];
extern s16 D_801E9424[];
extern SVECTOR D_801E9480[];
extern BtlModel D_801EA380[];
extern VECTOR D_801EBC80;

s32 fx_17d_0_op80(s32 mode, Fx17d0Work80 *work, Unk800F78A4 *env, Unk80100C78Entry *entry, EffInit *arg4) {
    BtlfxExports *ex = BTLFX_EXPORTS(env);
    s32 sx;
    s32 code;
    s32 abr;
    s32 alpha;
    s32 frame;
    s32 c;
    s32 t;
    s32 i;
    s32 i0;
    s32 i3;

    if (mode == 0) {
        arg4->unk0 = 0xBC;
        arg4->unk8 = 0x10;
        arg4->unk14 = (s32)func_801E7FE0;
        arg4->unk4 = 0x20;
    } else if (mode == 1) {
        work->unk0 = 0xA;
        work->unk4 = 0;
        D_801E9470 = env;
        ex->btlfx_initEffModels(D_801EA380, 0x20);
        work->unkBA = ex->func_800E8F50((Unk800E8F50 *)ex->btlfx_getFileData(entry->unk8, 0x1C), 0);
        BTLFX_COPY_SVEC(&work->unk8, &D_801E9368);
        work->unk18.vy = -0x1000;
        work->unk18.vx = 0;
        work->unk18.vz = 0;
        work->unk30.vx = 0;
        work->unk30.vy = 0;
        work->unk30.vz = 0;
        D_801EBC80.vx = 0x1000;
        D_801EBC80.vy = 0x1000;
        D_801EBC80.vz = 0x1000;
        BTLFX_COPY_SVEC(&work->unk48, &work->unk8);
        work->unk48.vy -= 0x180;
        env->unk2C->unk0[3].vx = work->unk48.vx;
        env->unk2C->unk0[3].vy = work->unk48.vy;
        env->unk2C->unk0[3].vz = work->unk48.vz;
        func_801E7894(0x140, 0x200, D_801E9480);
        work->unk58 = 0;
    } else {
        sx = 0;
        code = 0;
        abr = 0;
        alpha = 0;
        frame = entry->unk0;
        c = 0;
        switch (work->unk0) {
            SVECTOR rot;

        case 0xA: {
            CVECTOR col;
            s32 h;

            t = frame << 7;
            col.r = 0x80;
            col.g = 0xC8;
            col.b = 0xFF;
            h = t >> 1;
            rot.vx = ((0x1000 - ex->math_cos(h)) >> 1) / 3;
            rot.vy = ex->math_sin(t >> 2) >> 2;
            rot.vz = work->unk4 * 4;
            work->unk38.vx = rot.vx >> 1;
            work->unk38.vy = rot.vy >> 1;
            work->unk38.vz = rot.vz >> 1;
            func_801E79E8((u32 *)env->unk20 + 8, 0x140, 0x80, ((ex->math_sin(h) * 0x18) >> 12) - 1, &rot, D_801E9480,
                          &col, t, 1);
            if (frame >= 0x19) {
                code = 1;
                abr = 1;
                c = (frame - 0x18) * 12;
                sx = 0;
            }
            if (frame >= 0x20) {
                work->unk0 = 0;
                entry->unk0 = -1;
            }
            break;
        }
        case 0: {
            CVECTOR col;
            s32 q;
            s32 h;
            s32 a;
            s32 b;

            t = (frame << 12) / 80;
            q = t >> 2;
            alpha = ex->math_sin(q);
            col.r = 0x80;
            col.g = 0xC8;
            col.b = 0xFF;
            h = t >> 1;
            rot.vx = (0x1000 - ((0x1000 - ex->math_cos(h)) >> 1)) / 3;
            code = 1;
            abr = 1;
            sx = t >> 7;
            rot.vy = (0x1000 - ex->math_sin(q)) >> 2;
            rot.vz = work->unk4 * 4;
            work->unk38.vx = rot.vx >> 1;
            work->unk38.vy = rot.vy >> 1;
            work->unk38.vz = rot.vz >> 1;
            func_801E79E8((u32 *)env->unk20 + 8, 0x140, 0x80, ~((t * 20) >> 12), &rot, D_801E9480, &col, 0x1000, 1);
            a = ((0x1000 - ex->math_sin(q)) >> 5) + 0xA0;
            b = 0x6E - ((0x1000 - ((0x1000 - ex->math_cos(h)) >> 1)) >> 3);
            ex->func_800D1BC4(0xC00, (u8 *)ex->btlfx_getFileData(entry->unk8, 0x1D), a, b, 0, 0xFF, t);
            c = 0x60 - sx;
            if (frame >= 0x50) {
                work->unk0 = code;
                entry->unk0 = -1;
                for (i0 = 0; i0 < 21; i0++) {
                    s32 k;
                    s32 v;

                    work->unk5C[i0] = ex->func_800E8F50(
                        (Unk800E8F50 *)ex->btlfx_getFileData(entry->unk8, ((s16 *)D_801E9370)[i0 * 4]), 0);
                    k = ((s16 *)D_801E9370)[i0 * 4 + 1];
                    v = -1;
                    if (k >= 0) {
                        v = ex->func_800E8F50((Unk800E8F50 *)ex->btlfx_getFileData(entry->unk8, k), 0);
                    }
                    work->unk86[i0] = v;
                }
            }
            break;
        }
        case 1: {
            SVECTOR rot;
            CVECTOR col;
            s32 q;
            s32 b;
            s32 d;
            s32 e;

            t = (frame << 12) / 88;
            t = 0x1000 - ex->math_cos(t >> 2);
            if (frame == 0) {
                Pool6CEntry *p;

                BTLFX_COPY_SVEC(&work->unk40, &work->unk8);
                work->unk40.vy -= 0x140;
                work->unk40.vz += 0x1000;
                p = ex->func_800E3CEC(ex->btlfx_getFileData(entry->unk8, 0x23), (s32)&work->unk40);
                if (p != NULL) {
                    p->unk22 = -0x100;
                }
            }
            q = t >> 2;
            alpha = 0x1000;
            work->unk38.vx -= 1;
            work->unk38.vy += 1;
            b = (0x1000 - ex->math_cos((0x1000 - ex->math_cos(q)) >> 2)) >> 2;
            work->unk10.vx = 0;
            work->unk10.vy = 0x200 - ((ex->math_cos(b) * 0x300) >> 12);
            work->unk10.vz = ex->math_cos(b);
            col.r = 0x80;
            col.g = 0xC8;
            col.b = 0xFF;
            rot.vx = 0;
            rot.vy = 0;
            rot.vz = work->unk4 * 4;
            d = (t >> 8) + 0x14;
            func_801E79E8((u32 *)env->unk20 + 0x400, 0x140, 0x80, -d, &rot, D_801E9480, &col,
                          (ex->math_cos(q) >> 1) + 0x800, 1);
            b = 0x1000 - ex->math_cos(q);
            b = 0x1000 - ex->math_cos(b >> 2);
            if (t > 0xC00) {
                b = b - (ex->math_cos(t - 0xC00) - 0x1000);
            }
            func_801E7D18(entry, &work->unk10, frame % 10, (s32)work->unk5C, work->unk86, b);
            if (frame < 0x58) {
                e = (frame >> 2) + 0x6E;
                ex->func_800D1BC4(0xC00, (u8 *)ex->btlfx_getFileData(entry->unk8, 0x1D), 0xA0, e, 0, 0xFF, alpha);
            }
            code = 1;
            abr = 1;
            sx = (t >> 4) + 0x20;
            c = 0x40;
            if (frame >= 0x58) {
                u16 r0[2][2];
                u16 r1[2][2];

                work->unk0 = 2;
                entry->unk0 = -1;
                work->unk10.vy = 0;
                BTLFX_COPY_SVEC(&work->unk8, &work->unk18);
                for (i = 0; i < 2; i++) {
                    r0[i][0] = ((Fx17d0Summon *)env->unk4)->unk54->unk18[D_801E9424[i]];
                    r0[i][1] = ((Fx17d0Summon *)env->unk4)->unk54->unk24[D_801E9424[i]];
                    r1[i][0] = 0;
                    r1[i][1] = ((Fx17d0Summon *)env->unk4)->unk54->unk30[D_801E9424[i]];
                }
                for (i = 0; i < 5; i++) {
                    work->unkB0[i] = ((s32 (*)(void *, void *, void *))ex->Hi_RegisterTexListModel)(
                        (void *)ex->btlfx_getFileData(entry->unk8, D_801E9418[i]), r0, r1);
                }
            }
            break;
        }
        case 2: {
            VECTOR pos;
            SVECTOR rot;
            s16 *f;
            s32 n;

            alpha = 0x1000;
            work->unk38.vx += 0xC;
            work->unk38.vy += 6;
            func_801E7D18(entry, &work->unk10, frame, (s32)work->unk5C, work->unk86, 0x1000);
            if (frame >= 0x79) {
                s32 a;
                s32 b;
                s32 m;

                f = (s16 *)ex->btlfx_getFileData(entry->unk8, 0x1E);
                n = frame;
                if (n >= f[1]) {
                    n = f[1] - 1;
                }
                f = f + 2 + n * 6;
                pos.vx = f[0];
                pos.vy = f[1];
                pos.vz = f[2];
                rot.vx = f[3];
                rot.vy = f[4];
                rot.vz = -f[5];
                f = (s16 *)ex->btlfx_getFileData(entry->unk8, 0x1B);
                if (frame >= f[1]) {
                    f = (s16 *)ex->btlfx_getFileData(entry->unk8, 0x20);
                    n = frame - (f[1] - 1);
                    if (n >= f[1]) {
                        n = f[1] - 1;
                    }
                    ex->Hi_DrawMorphEffModel(&rot, &pos, (s32)&D_801EBC80, work->unkB0[1], work->unkB0[2],
                                             0x1000 - *(f += 2 + n * 3));
                } else {
                    ex->Hi_DrawMorphEffModel(&rot, &pos, (s32)&D_801EBC80, work->unkB0[0], work->unkB0[1],
                                             *(f += 2 + frame * 3));
                }
            }
            if (frame >= 0x8E) {
                f = (s16 *)ex->btlfx_getFileData(entry->unk8, 0x1F);
                n = frame;
                if (n >= f[1]) {
                    n = f[1] - 1;
                }
                f = f + 2 + n * 6;
                pos.vx = f[0];
                pos.vy = f[1];
                pos.vz = f[2];
                rot.vx = f[3];
                rot.vy = f[4];
                rot.vz = -f[5];
                f = (s16 *)ex->btlfx_getFileData(entry->unk8, 0x21);
                n = frame;
                if (n >= f[1]) {
                    n = f[1] - 1;
                }
                ex->Hi_DrawMorphEffModel(&rot, &pos, (s32)&D_801EBC80, work->unkB0[3], work->unkB0[4], *(f += 2 + n * 3));
            }
            if (frame < 0x9D) {
                for (i3 = 0; i3 < 2; i3++) {
                    Fx17d0Part *p = ex->btlfx_poolSpawn((s16 *)entry->unk10);

                    if (p != NULL) {
                        s32 a;
                        s32 r;

                        BTLFX_COPY_SVEC(&p->unk8, &work->unk18);
                        a = ex->btlfx_rand();
                        r = (ex->btlfx_rand() & 0x7F) + 0x400;
                        p->unk8.vz -= 0x5DC;
                        p->unk8.vx += (ex->math_cos(a) * r) >> 12;
                        p->unk8.vy = p->unk8.vy + (((ex->math_sin(a) * r) >> 12) - 0x19A);
                        p->unk2 = ex->btlfx_rand() & 0x7F;
                        p->unk4 = work->unk58 & 0x1F;
                        work->unk58++;
                    }
                }
            }
            if (frame >= 0xA1) {
                func_801E7704((u32 *)env->unk20 + 1, 1,
                              ((0x1000 - ex->math_cos((((frame - 0xA1) << 12) / 7) >> 2)) * 0xE0) >> 12, 0xF0, 0xF0,
                              0xF0);
            }
            if (frame >= 0xA3) {
                s32 k = ((frame - 0xA3) * 0xFF) / 6;

                ex->btlfx_drawScreenTint((u32 *)env->unk20 + 1, 1, k, k, k);
            }
            if (frame >= 0xA9) {
                ex->btlfx_poolClear((Unk800DA1B8 *)entry->unk10);
                ex->btlfx_clearEffModels();
                return 1;
            }
            break;
        }
        }
        if (c != 0) {
            CVECTOR col;
            SVECTOR rot;
            VECTOR sc;

            if (abr >= 0) {
                col.r = col.g = col.b = c;
            } else {
                abr = 0xFF;
                col.b = c;
                col.g = 0;
                col.r = 0;
            }
            rot.vx = 0;
            rot.vy = 0;
            rot.vz = 0;
            sc.vx = sc.vy = sx + 0x1000;
            sc.vz = 0x1000;
            ex->func_800E6764(code, 0xA0, 0x70, 0x50, 0x38, 4, 4, 2, 2, &rot, &sc, &col, abr, 0);
        }
        if (alpha != 0) {
            VECTOR pos;
            VECTOR sc;
            s32 s;
            s32 k;

            pos.vx = work->unk18.vx;
            pos.vy = work->unk18.vy;
            pos.vz = work->unk18.vz;
            s = 0x4000;
            if (work->unk0 == 2) {
                s = 0xAAA;
            }
            sc.vx = sc.vy = sc.vz = s;
            ex->Hi_DrawEffModel(&work->unk38, &pos, (s32)&sc, work->unkBA);
            k = (alpha >> 5) - 0x80;
            ex->Hi_ModifyEffModelRGB(work->unkBA, k, k, k);
        }
        work->unk4++;
    }
    return 0;
}
