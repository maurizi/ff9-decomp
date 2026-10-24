#include "fxmod/fx_17d_3.h"

INCLUDE_RODATA("asm/us/fx_17d_3/nonmatchings/fx_17d_3", fx_17d_3_headerPtr);

s32 fx_17d_3_op80(s32 mode, Fx17d3Work80 *work, Unk800F78A4 *env, Unk80100C78Entry *entry, s32 *arg4) {
    BtlfxExports *ex = BTLFX_EXPORTS(env);
    VECTOR sc;
    VECTOR pos;
    SVECTOR rot;
    Pool6CEntry *p;
    s32 frame;
    s32 t;
    s32 s;
    s32 c;

    if (mode == 0) {
        *arg4 = 0x80;
    } else if (mode == 1) {
        work->unk0 = 0;
        work->unk4 = 0;
        D_801EEC84 = env;
        ex->func_800E3C58();
        ex->btlfx_initEffModels(D_801EEC94, 5);
        work->unk68 = ex->btlfx_registerEffectModel((Unk800E8F50 *)ex->btlfx_getFileData(entry->unk8, 0x83), 0);
        work->unk6C = ex->btlfx_registerEffectModel((Unk800E8F50 *)ex->btlfx_getFileData(entry->unk8, 0x81), 1);
        work->unk70 = ex->btlfx_registerEffectModel((Unk800E8F50 *)ex->btlfx_getFileData(entry->unk8, 0x82), 0);
        work->unk74 = ex->btlfx_registerEffectModel((Unk800E8F50 *)ex->btlfx_getFileData(entry->unk8, 0x80), 0);
        ex->Hi_SetSummonMotion(SUMMON(env)->unk54->unk180[1], SUMMON(env)->unk60);
        work->unk48[1] = 0;
        work->unk48[0] = 0;
        work->unk48[2] = 0;
        work->unk54[0] = work->unk54[1] = work->unk54[2] = 0x1800;
        work->unk60.vx = 0;
        work->unk60.vy = 0;
        work->unk60.vz = 0;
        work->unk34.vx = 0;
        work->unk34.vy = 0;
        work->unk34.vz = 0;
        work->unk3C[0] = work->unk3C[1] = work->unk3C[2] = 0x800;
        work->unk28[0] = work->unk48[0];
        work->unk28[1] = work->unk48[1];
        work->unk28[2] = work->unk48[2];
        work->unk28[1] -= 0x200;
    } else {
        frame = entry->unk0;
        switch (work->unk0) {
        case 0: {
            VECTOR pp;

            t = (frame << 12) / 24;
            if (frame == 0) {
                ex->btlfx_drawScreenTint((u32 *)(env->ot + 1), 0xFF, 0xFF, 0xFF, 0xFF);
            }
            ex->Hi_SetSummonMotFrame(SUMMON(env)->unk60, 0);
            pp.vx = work->unk48[0];
            pp.vy = work->unk48[1];
            pp.vz = work->unk48[2];
            pp.vx += ex->btlfx_randCentered(0xE);
            pp.vy += ex->btlfx_randCentered(0xE);
            pp.vz += ex->btlfx_randCentered(0xE);
            ex->Hi_DrawSummonModel((SVECTOR *)&pp, (VECTOR *)work->unk48, (s32)work->unk54, SUMMON(env)->unk60, 0);
            ex->Hi_GetSummonBonePos(SUMMON(env)->unk60, D_801EEC2C[1], (u16 *)&work->unk20);
            work->unk20.vy -= frame * 12 - 0x2BC;
            env->camera->points[3].vx = work->unk20.vx;
            env->camera->points[3].vy = work->unk20.vy;
            env->camera->points[3].vz = work->unk20.vz;
            BTLFX_COPY_SVEC(&work->unk10, &work->unk20);
            work->unk10.vy += 0x300;
            if (frame == 0) {
                p = ex->func_800E3CEC(ex->btlfx_getFileData(entry->unk8, 2), (s32)&work->unk10);
                if (p != NULL) {
                    p->unk14 = (s32)D_801EF084;
                    p->unk12 = 0x10;
                    p->unkC = 0xCC;
                    p->unk22 = 0x40;
                }
            }
            sc.vx = sc.vz = 0x555;
            sc.vy = 0xAAA;
            pos.vx = work->unk20.vx;
            pos.vy = work->unk20.vy;
            pos.vz = work->unk20.vz;
            pos.vx += 0x100 + ((0x1000 - t) >> 5);
            pos.vy += 0x100;
            rot.vx = 0;
            rot.vy = 0;
            rot.vz = 0;
            pos.vz -= 0x200;
            if (t < 0x800) {
                s = 0x1000;
            } else {
                s = 0x1000 - (t - 0x800) * 3 / 2;
            }
            ex->Hi_DrawMorphEffModel(&rot, &pos, (s32)&sc, work->unk70, work->unk74, s);
            ex->Hi_ModifyEffModelAbr(work->unk70, 1);
            ex->Hi_ModifyEffModelRGB(work->unk70, 0, 0, 0);
            ex->Hi_DrawMorphEffModel(&rot, &pos, (s32)&sc, work->unk70, work->unk74, s);
            ex->Hi_ModifyEffModelAbr(work->unk70, 2);
            ex->Hi_ModifyEffModelRGB(work->unk70, 0, 0, 0);
            ex->Hi_DrawEffModel(&work->unk34, (VECTOR *)work->unk28, (s32)work->unk3C, work->unk68);
            ex->func_800E6624(frame * 0x18, 0x300, 0x140, 0x20, 0x80, 0x320, 0x140);
            ex->func_800E6624(frame * 0x10, 0x2C0, 0x100, 0x40, 0x80, 0x2C0, 0x180);
            {
            CVECTOR col;
            SVECTOR r2;
            VECTOR s2;

            col.r = col.g = col.b = ((0x1000 - t) * 100) >> 12;
            s2.vx = s2.vy = s2.vz = 0x1040;
            r2.vx = 0;
            r2.vy = 0;
            r2.vz = 0;
            ex->func_800E6764(1, 0xA0, 0x70, 0x50, 0x38, 4, 4, 2, 2, &r2, &s2, &col, 1, 0);
            }
            if (frame >= 0x18) {
                work->unk0 = 1;
                entry->unk0 = -1;
                ex->func_800E3C58();
            }
            break;
        }
        case 1: {
            VECTOR pp;

            t = (frame << 12) / 24;
            ex->Hi_SetSummonMotFrame(SUMMON(env)->unk60, 0);
            pp.vx = work->unk48[0];
            pp.vy = work->unk48[1];
            pp.vz = work->unk48[2];
            pp.vx += ex->btlfx_randCentered(0xE);
            pp.vy += ex->btlfx_randCentered(0xE);
            pp.vz += ex->btlfx_randCentered(0xE);
            ex->Hi_DrawSummonModel((SVECTOR *)&pp, (VECTOR *)work->unk48, (s32)work->unk54, SUMMON(env)->unk60, 0);
            ex->Hi_GetSummonBonePos(SUMMON(env)->unk60, D_801EEC2C[0], (u16 *)&work->unk20);
            work->unk20.vy -= frame * 12 - 0x2BC;
            env->camera->points[3].vx = work->unk20.vx;
            env->camera->points[3].vy = work->unk20.vy;
            env->camera->points[3].vz = work->unk20.vz;
            BTLFX_COPY_SVEC(&work->unk10, &work->unk20);
            work->unk10.vy += 0x300;
            if (frame == 0) {
                p = ex->func_800E3CEC(ex->btlfx_getFileData(entry->unk8, 2), (s32)&work->unk10);
                if (p != NULL) {
                    p->unk14 = (s32)D_801EF084;
                    p->unk12 = 0x10;
                    p->unkC = 0xCC;
                    p->unk22 = 0x40;
                }
            }
            sc.vx = sc.vz = 0x555;
            sc.vy = 0xAAA;
            pos.vx = work->unk20.vx;
            pos.vy = work->unk20.vy;
            pos.vz = work->unk20.vz;
            pos.vx -= 0x100 + ((0x1000 - t) >> 5);
            pos.vy += 0x100;
            rot.vx = 0;
            rot.vy = 0;
            rot.vz = 0;
            pos.vz -= 0x200;
            if (t < 0x800) {
                s = 0x1000;
            } else {
                s = 0x1000 - (t - 0x800) * 3 / 2;
            }
            ex->Hi_DrawMorphEffModel(&rot, &pos, (s32)&sc, work->unk70, work->unk74, s);
            ex->Hi_ModifyEffModelAbr(work->unk70, 1);
            ex->Hi_ModifyEffModelRGB(work->unk70, 0, 0, 0);
            ex->Hi_DrawMorphEffModel(&rot, &pos, (s32)&sc, work->unk70, work->unk74, s);
            ex->Hi_ModifyEffModelAbr(work->unk70, 2);
            ex->Hi_ModifyEffModelRGB(work->unk70, 0, 0, 0);
            ex->Hi_DrawEffModel(&work->unk34, (VECTOR *)work->unk28, (s32)work->unk3C, work->unk68);
            ex->func_800E6624(frame * 0x18, 0x300, 0x140, 0x20, 0x80, 0x320, 0x140);
            ex->func_800E6624(frame * 0x10, 0x2C0, 0x100, 0x40, 0x80, 0x2C0, 0x180);
            {
            CVECTOR col;
            SVECTOR r2;
            VECTOR s2;

            col.r = col.g = col.b = ((0x1000 - t) * 100) >> 12;
            s2.vx = s2.vy = s2.vz = 0x1040;
            r2.vx = 0;
            r2.vy = 0;
            r2.vz = 0;
            ex->func_800E6764(1, 0xA0, 0x70, 0x50, 0x38, 4, 4, 2, 2, &r2, &s2, &col, 1, 0);
            }
            if (frame >= 0x18) {
                work->unk0 = 2;
                entry->unk0 = -1;
                ex->func_800E3C58();
                work->unk78 = work->unk7C = NULL;
                ex->Hi_SetSummonMotion(SUMMON(env)->unk54->unk180[2], SUMMON(env)->unk60);
            }
            break;
        }
        case 2: {
            VECTOR pp;
            SVECTOR v;
            VECTOR s2;

            if (frame < 10) {
                ex->Hi_SetSummonMotFrame(SUMMON(env)->unk60, 0);
            }
            if (frame >= 20) {
                ex->Hi_SetSummonMotFrame(SUMMON(env)->unk60, 10);
            }
            pp.vx = work->unk48[0];
            pp.vy = work->unk48[1];
            pp.vz = work->unk48[2];
            pp.vx += ex->btlfx_randCentered(8);
            pp.vy += ex->btlfx_randCentered(8);
            pp.vz += ex->btlfx_randCentered(8);
            ex->Hi_DrawSummonModel((SVECTOR *)&pp, (VECTOR *)work->unk48, (s32)work->unk54, SUMMON(env)->unk60, 0);
            if (frame == 0) {
                ex->Hi_GetSummonBonePos(SUMMON(env)->unk60, D_801EEC30[0], (u16 *)&work->unk20);
                work->unk20.vx = work->unk48[0];
                work->unk20.vz = work->unk48[2];
                env->camera->points[3].vx = work->unk20.vx;
                env->camera->points[3].vy = work->unk20.vy;
                env->camera->points[3].vz = work->unk20.vz;
            }
            ex->Hi_GetSummonBonePos(SUMMON(env)->unk60, D_801EEC30[0], (u16 *)&work->unk10);
            ex->Hi_GetSummonBonePos(SUMMON(env)->unk60, D_801EEC30[1], (u16 *)&work->unk18);
            ex->Hi_DrawEffModel(&work->unk34, (VECTOR *)work->unk28, (s32)work->unk3C, work->unk68);
            s2.vx = s2.vz = work->unk3C[0] / 4;
            s2.vy = work->unk3C[1] / 2;
            ex->Hi_DrawEffModel(&work->unk34, (VECTOR *)work->unk28, (s32)&s2, work->unk6C);
            if (frame == 0xC) {
                work->unk78 = ex->func_800E3CEC(ex->btlfx_getFileData(entry->unk8, 1), (s32)&work->unk10);
                if (work->unk78 != NULL) {
                    work->unk78->unk14 = (s32)D_801EFD44;
                    work->unk78->unk12 = 0x32;
                    work->unk78->unkC = 0x1C;
                }
                if (work->unk78 != NULL) {
                    work->unk78->unk22 = 0x20;
                }
                work->unk7C = ex->func_800E3CEC(ex->btlfx_getFileData(entry->unk8, 1), (s32)&work->unk18);
                if (work->unk7C != NULL) {
                    work->unk7C->unk14 = (s32)D_801F02C4;
                    work->unk7C->unk12 = 0x32;
                    work->unk7C->unkC = 0x1C;
                }
                if (work->unk7C != NULL) {
                    work->unk7C->unk22 = 0x20;
                }
            }
            v.vx = 0xA00;
            v.vy = 0;
            v.vz = 0;
            if (work->unk78 != NULL) {
                if (work->unk78->unk30 == -1) {
                    work->unk78 = NULL;
                }
                if (work->unk78 != NULL) {
                    work->unk78->unk5C.vx = v.vx;
                    work->unk78->unk5C.vy = v.vy;
                    work->unk78->unk5C.vz = v.vz;
                }
            }
            if (work->unk7C != NULL) {
                if (work->unk7C->unk30 == -1) {
                    work->unk7C = NULL;
                }
                if (work->unk7C != NULL) {
                    work->unk7C->unk5C.vx = v.vx;
                    work->unk7C->unk5C.vy = v.vy;
                    work->unk7C->unk5C.vz = v.vz;
                }
            }
            ex->func_800E6624(frame * 0x10, 0x2C0, 0x100, 0x40, 0x80, 0x2C0, 0x180);
            ex->func_800E6624(work->unk4 * 0x10, 0x340, 0x100, 0x20, 0x80, 0x360, 0x100);
            if (frame >= 0x15) {
                work->unk0 = 3;
                entry->unk0 = -1;
                work->unk78 = work->unk7C = NULL;
                work->unk28[1] -= 0x64;
                ex->Hi_SetSummonMotion(SUMMON(env)->unk54->unk180[4], SUMMON(env)->unk60);
            }
            break;
        }
        case 3: {
            VECTOR pp;
            VECTOR s2;
            SVECTOR v;

            if (frame < 10) {
                ex->Hi_SetSummonMotFrame(SUMMON(env)->unk60, 0);
            }
            if (frame >= 17) {
                ex->Hi_SetSummonMotFrame(SUMMON(env)->unk60, 7);
            }
            pp.vx = work->unk48[0];
            pp.vy = work->unk48[1];
            pp.vz = work->unk48[2];
            pp.vx += ex->btlfx_randCentered(6);
            pp.vy += ex->btlfx_randCentered(6);
            pp.vz += ex->btlfx_randCentered(6);
            ex->Hi_DrawSummonModel((SVECTOR *)&pp, (VECTOR *)work->unk48, (s32)work->unk54, SUMMON(env)->unk60, 0);
            ex->Hi_GetSummonBonePos(SUMMON(env)->unk60, D_801EEC2C[0], (u16 *)&work->unk10);
            ex->Hi_GetSummonBonePos(SUMMON(env)->unk60, D_801EEC2C[1], (u16 *)&work->unk18);
            if (frame < 18) {
                work->unk20.vy += (ex->math_cos((frame << 10) / 18) * 0x29) >> 12;
                env->camera->points[3].vx = work->unk20.vx;
                env->camera->points[3].vy = work->unk20.vy;
                env->camera->points[3].vz = work->unk20.vz;
            }
            ex->Hi_DrawEffModel(&work->unk34, (VECTOR *)work->unk28, (s32)work->unk3C, work->unk68);
            s2.vx = s2.vz = work->unk3C[0] / 2;
            s2.vy = work->unk3C[1] / 2;
            ex->Hi_DrawEffModel(&work->unk34, (VECTOR *)work->unk28, (s32)&s2, work->unk6C);
            if (frame == 0xC) {
                work->unk78 = ex->func_800E3CEC(ex->btlfx_getFileData(entry->unk8, 6), (s32)&work->unk10);
                if (work->unk78 != NULL) {
                    work->unk78->unk14 = (s32)D_801EFD44;
                    work->unk78->unk12 = 0x32;
                    work->unk78->unkC = 0x1C;
                }
                if (work->unk78 != NULL) {
                    work->unk78->unk22 = 0x20;
                }
                work->unk7C = ex->func_800E3CEC(ex->btlfx_getFileData(entry->unk8, 6), (s32)&work->unk18);
                if (work->unk7C != NULL) {
                    work->unk7C->unk14 = (s32)D_801F02C4;
                    work->unk7C->unk12 = 0x32;
                    work->unk7C->unkC = 0x1C;
                }
                if (work->unk7C != NULL) {
                    work->unk7C->unk22 = 0x20;
                }
            }
            v.vx = 0xA00;
            v.vy = 0;
            v.vz = 0;
            if (work->unk78 != NULL) {
                if (work->unk78->unk30 == -1) {
                    work->unk78 = NULL;
                }
                if (work->unk78 != NULL) {
                    work->unk78->unk5C.vx = v.vx;
                    work->unk78->unk5C.vy = v.vy;
                    work->unk78->unk5C.vz = v.vz;
                }
            }
            if (work->unk7C != NULL) {
                if (work->unk7C->unk30 == -1) {
                    work->unk7C = NULL;
                }
                if (work->unk7C != NULL) {
                    work->unk7C->unk5C.vx = v.vx;
                    work->unk7C->unk5C.vy = v.vy;
                    work->unk7C->unk5C.vz = v.vz;
                }
            }
            ex->func_800E6624(frame * 0x10, 0x2C0, 0x100, 0x40, 0x80, 0x2C0, 0x180);
            ex->func_800E6624(work->unk4 * 0x10, 0x340, 0x100, 0x20, 0x80, 0x360, 0x100);
            if (frame >= 0x15) {
                work->unk0 = 4;
                entry->unk0 = -1;
                work->unk78 = work->unk7C = NULL;
                ex->Hi_SetSummonMotion(SUMMON(env)->unk54->unk180[5], SUMMON(env)->unk60);
                work->unk28[1] -= 0xC18;
                ex->Hi_GetSummonBonePos(SUMMON(env)->unk60, 0x21, (u16 *)&work->unk20);
                work->unk20.vy -= 0x46;
                env->camera->points[2].vx = work->unk20.vx;
                env->camera->points[2].vy = work->unk20.vy;
                env->camera->points[2].vz = work->unk20.vz;
            }
            break;
        }
        case 4: {
            VECTOR pp;
            VECTOR s2;
            SVECTOR v;

            if (frame == 0) {
                ex->func_800E3C58();
            }
            pp.vx = work->unk48[0];
            pp.vy = work->unk48[1];
            pp.vz = work->unk48[2];
            pp.vx += ex->btlfx_randCentered(2);
            pp.vy += ex->btlfx_randCentered(2);
            pp.vz += ex->btlfx_randCentered(2);
            ex->Hi_DrawSummonModel((SVECTOR *)&pp, (VECTOR *)work->unk48, (s32)work->unk54, SUMMON(env)->unk60, 0);
            ex->Hi_GetSummonBonePos(SUMMON(env)->unk60, D_801EEC34[0], (u16 *)&work->unk10);
            ex->Hi_GetSummonBonePos(SUMMON(env)->unk60, D_801EEC34[1], (u16 *)&work->unk18);
            if (frame == 6) {
                work->unk78 = ex->func_800E3CEC(ex->btlfx_getFileData(entry->unk8, 6), (s32)&work->unk10);
                if (work->unk78 != NULL) {
                    work->unk78->unk14 = (s32)D_801EFD44;
                    work->unk78->unk12 = 0x32;
                    work->unk78->unkC = 0x1C;
                }
                if (work->unk78 != NULL) {
                    work->unk78->unk22 = 0x20;
                }
                work->unk7C = ex->func_800E3CEC(ex->btlfx_getFileData(entry->unk8, 6), (s32)&work->unk18);
                if (work->unk7C != NULL) {
                    work->unk7C->unk14 = (s32)D_801F02C4;
                    work->unk7C->unk12 = 0x32;
                    work->unk7C->unkC = 0x1C;
                }
                if (work->unk7C != NULL) {
                    work->unk7C->unk22 = 0x20;
                }
            }
            v.vx = 0x400;
            v.vy = 0;
            v.vz = 0;
            if (work->unk78 != NULL) {
                if (work->unk78->unk30 == -1) {
                    work->unk78 = NULL;
                }
                if (work->unk78 != NULL) {
                    work->unk78->unk5C.vx = v.vx;
                    work->unk78->unk5C.vy = v.vy;
                    work->unk78->unk5C.vz = v.vz;
                }
            }
            if (work->unk7C != NULL) {
                if (work->unk7C->unk30 == -1) {
                    work->unk7C = NULL;
                }
                if (work->unk7C != NULL) {
                    work->unk7C->unk5C.vx = v.vx;
                    work->unk7C->unk5C.vy = v.vy;
                    work->unk7C->unk5C.vz = v.vz;
                }
            }
            ex->Hi_DrawEffModel(&work->unk34, (VECTOR *)work->unk28, (s32)work->unk3C, work->unk68);
            s2.vx = s2.vz = work->unk3C[0] / 2;
            s2.vy = work->unk3C[1] / 2;
            ex->Hi_DrawEffModel(&work->unk34, (VECTOR *)work->unk28, (s32)&s2, work->unk6C);
            ex->func_800E6624(frame * 0x10, 0x2C0, 0x100, 0x40, 0x80, 0x2C0, 0x180);
            ex->func_800E6624(work->unk4 * 0x10, 0x340, 0x100, 0x20, 0x80, 0x360, 0x100);
            if (frame >= 0x18) {
                work->unk0 = 5;
                entry->unk0 = -1;
                ex->func_800E3C58();
                work->unk78 = work->unk7C = NULL;
                work->unk28[1] -= 0x384;
                ex->Hi_SetSummonMotFrame(SUMMON(env)->unk60, 0x18);
            }
            break;
        }
        case 5: {
            VECTOR pp;
            VECTOR s2;
            SVECTOR v;

            pp.vx = work->unk48[0];
            pp.vy = work->unk48[1];
            pp.vz = work->unk48[2];
            pp.vx += ex->btlfx_randCentered(1);
            pp.vy += ex->btlfx_randCentered(1);
            pp.vz += ex->btlfx_randCentered(1);
            ex->Hi_DrawSummonModel((SVECTOR *)&pp, (VECTOR *)work->unk48, (s32)work->unk54, SUMMON(env)->unk60, 0);
            ex->Hi_GetSummonBonePos(SUMMON(env)->unk60, D_801EEC38[0], (u16 *)&work->unk20);
            work->unk20.vy -= 0xAA;
            env->camera->points[3].vx = work->unk20.vx;
            env->camera->points[3].vy = work->unk20.vy;
            env->camera->points[3].vz = work->unk20.vz;
            ex->Hi_GetSummonBonePos(SUMMON(env)->unk60, D_801EEC38[0], (u16 *)&work->unk10);
            BTLFX_COPY_SVEC(&work->unk18, &work->unk10);
            work->unk18.vy -= 0x46;
            work->unk18.vz -= 0x80;
            if (frame == 0xB) {
                work->unk78 = ex->func_800E3CEC(ex->btlfx_getFileData(entry->unk8, 4), (s32)&work->unk10);
                if (work->unk78 != NULL) {
                    work->unk78->unk14 = (s32)D_801EFD44;
                    work->unk78->unk12 = 0x32;
                    work->unk78->unkC = 0x1C;
                }
                if (work->unk78 != NULL) {
                    work->unk78->unk22 = -0x80;
                }
                ex->func_800E3CEC(ex->btlfx_getFileData(entry->unk8, 0xE), (s32)&work->unk10);
                if (work->unk78 != NULL) {
                    work->unk78->unk22 = -0x80;
                }
            }
            v.vx = 0x400;
            v.vy = 0;
            v.vz = 0;
            if (work->unk78 != NULL) {
                if (work->unk78->unk30 == -1) {
                    work->unk78 = NULL;
                }
                if (work->unk78 != NULL) {
                    work->unk78->unk5C.vx = v.vx;
                    work->unk78->unk5C.vy = v.vy;
                    work->unk78->unk5C.vz = v.vz;
                }
            }
            if (frame == 0) {
                work->unk7C = ex->func_800E3CEC(ex->btlfx_getFileData(entry->unk8, 6), (s32)&work->unk18);
                if (work->unk7C != NULL) {
                    work->unk7C->unk14 = (s32)D_801F02C4;
                    work->unk7C->unk12 = 0x32;
                    work->unk7C->unkC = 0x1C;
                }
                if (work->unk7C != NULL) {
                    work->unk7C->unk22 = 0x20;
                }
            }
            v.vx = 0x400;
            v.vy = 0;
            v.vz = 0;
            if (work->unk7C != NULL) {
                if (work->unk7C->unk30 == -1) {
                    work->unk7C = NULL;
                }
                if (work->unk7C != NULL) {
                    work->unk7C->unk5C.vx = v.vx;
                    work->unk7C->unk5C.vy = v.vy;
                    work->unk7C->unk5C.vz = v.vz;
                }
            }
            ex->Hi_DrawEffModel(&work->unk34, (VECTOR *)work->unk28, (s32)work->unk3C, work->unk68);
            s2.vx = s2.vz = work->unk3C[0] / 3;
            s2.vy = work->unk3C[1] / 2;
            ex->Hi_DrawEffModel(&work->unk34, (VECTOR *)work->unk28, (s32)&s2, work->unk6C);
            ex->func_800E6624(frame * 0x10, 0x2C0, 0x100, 0x40, 0x80, 0x2C0, 0x180);
            ex->func_800E6624(work->unk4 * 0x10, 0x340, 0x100, 0x20, 0x80, 0x360, 0x100);
            if (frame >= 0x15) {
                work->unk0 = 6;
                entry->unk0 = -1;
                work->unk78 = work->unk7C = NULL;
                ex->func_800E3C58();
                ex->Hi_SetSummonMotFrame(SUMMON(env)->unk60, 0x18);
                ex->Hi_GetSummonBonePos(SUMMON(env)->unk60, D_801EEC38[1], (u16 *)&work->unk20);
                work->unk20.vy -= 0xAA;
                env->camera->points[3].vx = work->unk20.vx;
                env->camera->points[3].vy = work->unk20.vy;
                env->camera->points[3].vz = work->unk20.vz;
            }
            break;
        }
        case 6: {
            VECTOR pp;
            VECTOR s2;
            SVECTOR v;

            pp.vx = work->unk48[0];
            pp.vy = work->unk48[1];
            pp.vz = work->unk48[2];
            pp.vx += ex->btlfx_randCentered(1);
            pp.vy += ex->btlfx_randCentered(1);
            pp.vz += ex->btlfx_randCentered(1);
            ex->Hi_DrawSummonModel((SVECTOR *)&pp, (VECTOR *)work->unk48, (s32)work->unk54, SUMMON(env)->unk60, 0);
            ex->Hi_GetSummonBonePos(SUMMON(env)->unk60, D_801EEC38[1], (u16 *)&work->unk20);
            work->unk20.vy -= 0xAA;
            env->camera->points[3].vx = work->unk20.vx;
            env->camera->points[3].vy = work->unk20.vy;
            env->camera->points[3].vz = work->unk20.vz;
            ex->Hi_GetSummonBonePos(SUMMON(env)->unk60, D_801EEC38[1], (u16 *)&work->unk10);
            BTLFX_COPY_SVEC(&work->unk18, &work->unk10);
            work->unk18.vy -= 0x46;
            work->unk18.vz -= 0x80;
            if (frame == 0xB) {
                work->unk78 = ex->func_800E3CEC(ex->btlfx_getFileData(entry->unk8, 4), (s32)&work->unk10);
                if (work->unk78 != NULL) {
                    work->unk78->unk14 = (s32)D_801EFD44;
                    work->unk78->unk12 = 0x32;
                    work->unk78->unkC = 0x1C;
                }
                if (work->unk78 != NULL) {
                    work->unk78->unk22 = -0x80;
                }
                ex->func_800E3CEC(ex->btlfx_getFileData(entry->unk8, 0xE), (s32)&work->unk10);
                if (work->unk78 != NULL) {
                    work->unk78->unk22 = -0x80;
                }
            }
            v.vx = 0x400;
            v.vy = 0;
            v.vz = 0;
            if (work->unk78 != NULL) {
                if (work->unk78->unk30 == -1) {
                    work->unk78 = NULL;
                }
                if (work->unk78 != NULL) {
                    work->unk78->unk5C.vx = v.vx;
                    work->unk78->unk5C.vy = v.vy;
                    work->unk78->unk5C.vz = v.vz;
                }
            }
            if (frame == 0) {
                work->unk7C = ex->func_800E3CEC(ex->btlfx_getFileData(entry->unk8, 6), (s32)&work->unk18);
                if (work->unk7C != NULL) {
                    work->unk7C->unk14 = (s32)D_801F02C4;
                    work->unk7C->unk12 = 0x32;
                    work->unk7C->unkC = 0x1C;
                }
                if (work->unk7C != NULL) {
                    work->unk7C->unk22 = 0x20;
                }
            }
            v.vx = 0x400;
            v.vy = 0;
            v.vz = 0;
            if (work->unk7C != NULL) {
                if (work->unk7C->unk30 == -1) {
                    work->unk7C = NULL;
                }
                if (work->unk7C != NULL) {
                    work->unk7C->unk5C.vx = v.vx;
                    work->unk7C->unk5C.vy = v.vy;
                    work->unk7C->unk5C.vz = v.vz;
                }
            }
            ex->Hi_DrawEffModel(&work->unk34, (VECTOR *)work->unk28, (s32)work->unk3C, work->unk68);
            s2.vx = s2.vz = work->unk3C[0] / 3;
            s2.vy = work->unk3C[1] / 2;
            ex->Hi_DrawEffModel(&work->unk34, (VECTOR *)work->unk28, (s32)&s2, work->unk6C);
            ex->func_800E6624(frame * 0x10, 0x2C0, 0x100, 0x40, 0x80, 0x2C0, 0x180);
            ex->func_800E6624(work->unk4 * 0x10, 0x340, 0x100, 0x20, 0x80, 0x360, 0x100);
            if (frame >= 0x15) {
                work->unk0 = 7;
                entry->unk0 = -1;
                ex->func_800E3C58();
                work->unk78 = work->unk7C = NULL;
                ex->Hi_SetSummonMotion(SUMMON(env)->unk54->unk180[6], SUMMON(env)->unk60);
                ex->Hi_GetSummonBonePos(SUMMON(env)->unk60, 4, (u16 *)&work->unk20);
                work->unk20.vy -= 0x380;
                env->camera->points[3].vx = work->unk20.vx;
                env->camera->points[3].vy = work->unk20.vy;
                env->camera->points[3].vz = work->unk20.vz;
            }
            break;
        }
        case 7: {
            VECTOR pp;
            VECTOR s2;
            SVECTOR v;

            t = frame << 7;
            if (frame >= 0x17) {
                ex->Hi_SetSummonMotFrame(SUMMON(env)->unk60, 0x17);
            }
            pp.vx = work->unk48[0];
            pp.vy = work->unk48[1];
            pp.vz = work->unk48[2];
            ex->Hi_DrawSummonModel((SVECTOR *)&pp, (VECTOR *)work->unk48, (s32)work->unk54, SUMMON(env)->unk60, 0);
            ex->Hi_GetSummonBonePos(SUMMON(env)->unk60, 4, (u16 *)&work->unk20);
            if (t < 0x800) {
                work->unk20.vy -= ((0x1000 - ex->math_sin(t >> 1)) >> 2) - 0x80;
            } else {
                work->unk20.vy += 0x80;
            }
            env->camera->points[3].vx = work->unk20.vx;
            env->camera->points[3].vy = work->unk20.vy;
            env->camera->points[3].vz = work->unk20.vz;
            ex->Hi_GetSummonBonePos(SUMMON(env)->unk60, 4, (u16 *)&work->unk10);
            work->unk10.vz += 0x226;
            if (frame == 0xA) {
                work->unk78 = ex->func_800E3CEC(ex->btlfx_getFileData(entry->unk8, 9), (s32)&work->unk10);
                if (work->unk78 != NULL) {
                    work->unk78->unk22 = 0x80;
                }
            }
            v.vx = -0x270;
            v.vy = 0;
            v.vz = 0;
            if (work->unk78 != NULL) {
                if (work->unk78->unk30 == -1) {
                    work->unk78 = NULL;
                }
                if (work->unk78 != NULL) {
                    work->unk78->unk5C.vx = v.vx;
                    work->unk78->unk5C.vy = v.vy;
                    work->unk78->unk5C.vz = v.vz;
                }
            }
            ex->Hi_DrawEffModel(&work->unk34, (VECTOR *)work->unk28, (s32)work->unk3C, work->unk68);
            s2.vx = s2.vz = work->unk3C[0] / 3;
            s2.vy = work->unk3C[1] / 2;
            ex->Hi_DrawEffModel(&work->unk34, (VECTOR *)work->unk28, (s32)&s2, work->unk6C);
            ex->func_800E6624(frame * 0x10, 0x2C0, 0x100, 0x40, 0x80, 0x2C0, 0x180);
            ex->func_800E6624(work->unk4 * 0x10, 0x340, 0x100, 0x20, 0x80, 0x360, 0x100);
            if (frame >= 0x20) {
                ex->func_800E3C58();
                ex->btlfx_clearEffModels();
                return 1;
            }
            break;
        }
        }
        env->unk28 = 0;
        work->unk4++;
    }
    return 0;
}
