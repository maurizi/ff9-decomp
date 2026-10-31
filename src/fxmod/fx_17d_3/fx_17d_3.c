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
        fx_17d_3_env = env;
        ex->btlfx_initParticleSystem();
        ex->btlfx_initEffModels(fx_17d_3_effModels, 5);
        work->model68 = ex->btlfx_registerEffectModel((Unk800E8F50 *)ex->btlfx_getFileData(entry->file, 0x83), 0);
        work->model6C = ex->btlfx_registerEffectModel((Unk800E8F50 *)ex->btlfx_getFileData(entry->file, 0x81), 1);
        work->model70 = ex->btlfx_registerEffectModel((Unk800E8F50 *)ex->btlfx_getFileData(entry->file, 0x82), 0);
        work->model74 = ex->btlfx_registerEffectModel((Unk800E8F50 *)ex->btlfx_getFileData(entry->file, 0x80), 0);
        ex->Hi_SetSummonMotion(SUMMON(env)->unk54->motion180[1], SUMMON(env)->unk60);
        work->unk48[1] = 0;
        work->unk48[0] = 0;
        work->unk48[2] = 0;
        work->unk54[0] = work->unk54[1] = work->unk54[2] = 0x1800;
        work->unk60.vx = 0;
        work->unk60.vy = 0;
        work->unk60.vz = 0;
        work->rot34.vx = 0;
        work->rot34.vy = 0;
        work->rot34.vz = 0;
        work->unk3C[0] = work->unk3C[1] = work->unk3C[2] = 0x800;
        work->unk28[0] = work->unk48[0];
        work->unk28[1] = work->unk48[1];
        work->unk28[2] = work->unk48[2];
        work->unk28[1] -= 0x200;
    } else {
        frame = entry->frame;
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
            BTLFX_COPY_SVEC(&work->pos10, &work->unk20);
            work->pos10.vy += 0x300;
            if (frame == 0) {
                p = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, 2), (s32)&work->pos10);
                if (p != NULL) {
                    p->trails = (s32)D_801EF084;
                    p->trailSlots = 0x10;
                    p->trailStride = 0xCC;
                    p->otZ = 0x40;
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
            ex->Hi_DrawMorphEffModel(&rot, &pos, (s32)&sc, work->model70, work->model74, s);
            ex->Hi_ModifyEffModelAbr(work->model70, 1);
            ex->Hi_ModifyEffModelRGB(work->model70, 0, 0, 0);
            ex->Hi_DrawMorphEffModel(&rot, &pos, (s32)&sc, work->model70, work->model74, s);
            ex->Hi_ModifyEffModelAbr(work->model70, 2);
            ex->Hi_ModifyEffModelRGB(work->model70, 0, 0, 0);
            ex->Hi_DrawEffModel(&work->rot34, (VECTOR *)work->unk28, (s32)work->unk3C, work->model68);
            ex->btlfx_drawScrollMove(frame * 0x18, 0x300, 0x140, 0x20, 0x80, 0x320, 0x140);
            ex->btlfx_drawScrollMove(frame * 0x10, 0x2C0, 0x100, 0x40, 0x80, 0x2C0, 0x180);
            {
            CVECTOR col;
            SVECTOR r2;
            VECTOR s2;

            col.r = col.g = col.b = ((0x1000 - t) * 100) >> 12;
            s2.vx = s2.vy = s2.vz = 0x1040;
            r2.vx = 0;
            r2.vy = 0;
            r2.vz = 0;
            ex->btlfx_drawScreenGrid(1, 0xA0, 0x70, 0x50, 0x38, 4, 4, 2, 2, &r2, &s2, &col, 1, 0);
            }
            if (frame >= 0x18) {
                work->unk0 = 1;
                entry->frame = -1;
                ex->btlfx_initParticleSystem();
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
            BTLFX_COPY_SVEC(&work->pos10, &work->unk20);
            work->pos10.vy += 0x300;
            if (frame == 0) {
                p = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, 2), (s32)&work->pos10);
                if (p != NULL) {
                    p->trails = (s32)D_801EF084;
                    p->trailSlots = 0x10;
                    p->trailStride = 0xCC;
                    p->otZ = 0x40;
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
            ex->Hi_DrawMorphEffModel(&rot, &pos, (s32)&sc, work->model70, work->model74, s);
            ex->Hi_ModifyEffModelAbr(work->model70, 1);
            ex->Hi_ModifyEffModelRGB(work->model70, 0, 0, 0);
            ex->Hi_DrawMorphEffModel(&rot, &pos, (s32)&sc, work->model70, work->model74, s);
            ex->Hi_ModifyEffModelAbr(work->model70, 2);
            ex->Hi_ModifyEffModelRGB(work->model70, 0, 0, 0);
            ex->Hi_DrawEffModel(&work->rot34, (VECTOR *)work->unk28, (s32)work->unk3C, work->model68);
            ex->btlfx_drawScrollMove(frame * 0x18, 0x300, 0x140, 0x20, 0x80, 0x320, 0x140);
            ex->btlfx_drawScrollMove(frame * 0x10, 0x2C0, 0x100, 0x40, 0x80, 0x2C0, 0x180);
            {
            CVECTOR col;
            SVECTOR r2;
            VECTOR s2;

            col.r = col.g = col.b = ((0x1000 - t) * 100) >> 12;
            s2.vx = s2.vy = s2.vz = 0x1040;
            r2.vx = 0;
            r2.vy = 0;
            r2.vz = 0;
            ex->btlfx_drawScreenGrid(1, 0xA0, 0x70, 0x50, 0x38, 4, 4, 2, 2, &r2, &s2, &col, 1, 0);
            }
            if (frame >= 0x18) {
                work->unk0 = 2;
                entry->frame = -1;
                ex->btlfx_initParticleSystem();
                work->emit78 = work->emit7C = NULL;
                ex->Hi_SetSummonMotion(SUMMON(env)->unk54->motion180[2], SUMMON(env)->unk60);
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
            ex->Hi_GetSummonBonePos(SUMMON(env)->unk60, D_801EEC30[0], (u16 *)&work->pos10);
            ex->Hi_GetSummonBonePos(SUMMON(env)->unk60, D_801EEC30[1], (u16 *)&work->pos18);
            ex->Hi_DrawEffModel(&work->rot34, (VECTOR *)work->unk28, (s32)work->unk3C, work->model68);
            s2.vx = s2.vz = work->unk3C[0] / 4;
            s2.vy = work->unk3C[1] / 2;
            ex->Hi_DrawEffModel(&work->rot34, (VECTOR *)work->unk28, (s32)&s2, work->model6C);
            if (frame == 0xC) {
                work->emit78 = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, 1), (s32)&work->pos10);
                if (work->emit78 != NULL) {
                    work->emit78->trails = (s32)D_801EFD44;
                    work->emit78->trailSlots = 0x32;
                    work->emit78->trailStride = 0x1C;
                }
                if (work->emit78 != NULL) {
                    work->emit78->otZ = 0x20;
                }
                work->emit7C = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, 1), (s32)&work->pos18);
                if (work->emit7C != NULL) {
                    work->emit7C->trails = (s32)D_801F02C4;
                    work->emit7C->trailSlots = 0x32;
                    work->emit7C->trailStride = 0x1C;
                }
                if (work->emit7C != NULL) {
                    work->emit7C->otZ = 0x20;
                }
            }
            v.vx = 0xA00;
            v.vy = 0;
            v.vz = 0;
            if (work->emit78 != NULL) {
                if (work->emit78->state == BTLFX_EMIT_DONE) {
                    work->emit78 = NULL;
                }
                if (work->emit78 != NULL) {
                    work->emit78->rot.vx = v.vx;
                    work->emit78->rot.vy = v.vy;
                    work->emit78->rot.vz = v.vz;
                }
            }
            if (work->emit7C != NULL) {
                if (work->emit7C->state == BTLFX_EMIT_DONE) {
                    work->emit7C = NULL;
                }
                if (work->emit7C != NULL) {
                    work->emit7C->rot.vx = v.vx;
                    work->emit7C->rot.vy = v.vy;
                    work->emit7C->rot.vz = v.vz;
                }
            }
            ex->btlfx_drawScrollMove(frame * 0x10, 0x2C0, 0x100, 0x40, 0x80, 0x2C0, 0x180);
            ex->btlfx_drawScrollMove(work->unk4 * 0x10, 0x340, 0x100, 0x20, 0x80, 0x360, 0x100);
            if (frame >= 0x15) {
                work->unk0 = 3;
                entry->frame = -1;
                work->emit78 = work->emit7C = NULL;
                work->unk28[1] -= 0x64;
                ex->Hi_SetSummonMotion(SUMMON(env)->unk54->motion180[4], SUMMON(env)->unk60);
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
            ex->Hi_GetSummonBonePos(SUMMON(env)->unk60, D_801EEC2C[0], (u16 *)&work->pos10);
            ex->Hi_GetSummonBonePos(SUMMON(env)->unk60, D_801EEC2C[1], (u16 *)&work->pos18);
            if (frame < 18) {
                work->unk20.vy += (ex->math_cos((frame << 10) / 18) * 0x29) >> 12;
                env->camera->points[3].vx = work->unk20.vx;
                env->camera->points[3].vy = work->unk20.vy;
                env->camera->points[3].vz = work->unk20.vz;
            }
            ex->Hi_DrawEffModel(&work->rot34, (VECTOR *)work->unk28, (s32)work->unk3C, work->model68);
            s2.vx = s2.vz = work->unk3C[0] / 2;
            s2.vy = work->unk3C[1] / 2;
            ex->Hi_DrawEffModel(&work->rot34, (VECTOR *)work->unk28, (s32)&s2, work->model6C);
            if (frame == 0xC) {
                work->emit78 = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, 6), (s32)&work->pos10);
                if (work->emit78 != NULL) {
                    work->emit78->trails = (s32)D_801EFD44;
                    work->emit78->trailSlots = 0x32;
                    work->emit78->trailStride = 0x1C;
                }
                if (work->emit78 != NULL) {
                    work->emit78->otZ = 0x20;
                }
                work->emit7C = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, 6), (s32)&work->pos18);
                if (work->emit7C != NULL) {
                    work->emit7C->trails = (s32)D_801F02C4;
                    work->emit7C->trailSlots = 0x32;
                    work->emit7C->trailStride = 0x1C;
                }
                if (work->emit7C != NULL) {
                    work->emit7C->otZ = 0x20;
                }
            }
            v.vx = 0xA00;
            v.vy = 0;
            v.vz = 0;
            if (work->emit78 != NULL) {
                if (work->emit78->state == BTLFX_EMIT_DONE) {
                    work->emit78 = NULL;
                }
                if (work->emit78 != NULL) {
                    work->emit78->rot.vx = v.vx;
                    work->emit78->rot.vy = v.vy;
                    work->emit78->rot.vz = v.vz;
                }
            }
            if (work->emit7C != NULL) {
                if (work->emit7C->state == BTLFX_EMIT_DONE) {
                    work->emit7C = NULL;
                }
                if (work->emit7C != NULL) {
                    work->emit7C->rot.vx = v.vx;
                    work->emit7C->rot.vy = v.vy;
                    work->emit7C->rot.vz = v.vz;
                }
            }
            ex->btlfx_drawScrollMove(frame * 0x10, 0x2C0, 0x100, 0x40, 0x80, 0x2C0, 0x180);
            ex->btlfx_drawScrollMove(work->unk4 * 0x10, 0x340, 0x100, 0x20, 0x80, 0x360, 0x100);
            if (frame >= 0x15) {
                work->unk0 = 4;
                entry->frame = -1;
                work->emit78 = work->emit7C = NULL;
                ex->Hi_SetSummonMotion(SUMMON(env)->unk54->motion180[5], SUMMON(env)->unk60);
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
                ex->btlfx_initParticleSystem();
            }
            pp.vx = work->unk48[0];
            pp.vy = work->unk48[1];
            pp.vz = work->unk48[2];
            pp.vx += ex->btlfx_randCentered(2);
            pp.vy += ex->btlfx_randCentered(2);
            pp.vz += ex->btlfx_randCentered(2);
            ex->Hi_DrawSummonModel((SVECTOR *)&pp, (VECTOR *)work->unk48, (s32)work->unk54, SUMMON(env)->unk60, 0);
            ex->Hi_GetSummonBonePos(SUMMON(env)->unk60, D_801EEC34[0], (u16 *)&work->pos10);
            ex->Hi_GetSummonBonePos(SUMMON(env)->unk60, D_801EEC34[1], (u16 *)&work->pos18);
            if (frame == 6) {
                work->emit78 = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, 6), (s32)&work->pos10);
                if (work->emit78 != NULL) {
                    work->emit78->trails = (s32)D_801EFD44;
                    work->emit78->trailSlots = 0x32;
                    work->emit78->trailStride = 0x1C;
                }
                if (work->emit78 != NULL) {
                    work->emit78->otZ = 0x20;
                }
                work->emit7C = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, 6), (s32)&work->pos18);
                if (work->emit7C != NULL) {
                    work->emit7C->trails = (s32)D_801F02C4;
                    work->emit7C->trailSlots = 0x32;
                    work->emit7C->trailStride = 0x1C;
                }
                if (work->emit7C != NULL) {
                    work->emit7C->otZ = 0x20;
                }
            }
            v.vx = 0x400;
            v.vy = 0;
            v.vz = 0;
            if (work->emit78 != NULL) {
                if (work->emit78->state == BTLFX_EMIT_DONE) {
                    work->emit78 = NULL;
                }
                if (work->emit78 != NULL) {
                    work->emit78->rot.vx = v.vx;
                    work->emit78->rot.vy = v.vy;
                    work->emit78->rot.vz = v.vz;
                }
            }
            if (work->emit7C != NULL) {
                if (work->emit7C->state == BTLFX_EMIT_DONE) {
                    work->emit7C = NULL;
                }
                if (work->emit7C != NULL) {
                    work->emit7C->rot.vx = v.vx;
                    work->emit7C->rot.vy = v.vy;
                    work->emit7C->rot.vz = v.vz;
                }
            }
            ex->Hi_DrawEffModel(&work->rot34, (VECTOR *)work->unk28, (s32)work->unk3C, work->model68);
            s2.vx = s2.vz = work->unk3C[0] / 2;
            s2.vy = work->unk3C[1] / 2;
            ex->Hi_DrawEffModel(&work->rot34, (VECTOR *)work->unk28, (s32)&s2, work->model6C);
            ex->btlfx_drawScrollMove(frame * 0x10, 0x2C0, 0x100, 0x40, 0x80, 0x2C0, 0x180);
            ex->btlfx_drawScrollMove(work->unk4 * 0x10, 0x340, 0x100, 0x20, 0x80, 0x360, 0x100);
            if (frame >= 0x18) {
                work->unk0 = 5;
                entry->frame = -1;
                ex->btlfx_initParticleSystem();
                work->emit78 = work->emit7C = NULL;
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
            ex->Hi_GetSummonBonePos(SUMMON(env)->unk60, D_801EEC38[0], (u16 *)&work->pos10);
            BTLFX_COPY_SVEC(&work->pos18, &work->pos10);
            work->pos18.vy -= 0x46;
            work->pos18.vz -= 0x80;
            if (frame == 0xB) {
                work->emit78 = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, 4), (s32)&work->pos10);
                if (work->emit78 != NULL) {
                    work->emit78->trails = (s32)D_801EFD44;
                    work->emit78->trailSlots = 0x32;
                    work->emit78->trailStride = 0x1C;
                }
                if (work->emit78 != NULL) {
                    work->emit78->otZ = -0x80;
                }
                ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, 0xE), (s32)&work->pos10);
                if (work->emit78 != NULL) {
                    work->emit78->otZ = -0x80;
                }
            }
            v.vx = 0x400;
            v.vy = 0;
            v.vz = 0;
            if (work->emit78 != NULL) {
                if (work->emit78->state == BTLFX_EMIT_DONE) {
                    work->emit78 = NULL;
                }
                if (work->emit78 != NULL) {
                    work->emit78->rot.vx = v.vx;
                    work->emit78->rot.vy = v.vy;
                    work->emit78->rot.vz = v.vz;
                }
            }
            if (frame == 0) {
                work->emit7C = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, 6), (s32)&work->pos18);
                if (work->emit7C != NULL) {
                    work->emit7C->trails = (s32)D_801F02C4;
                    work->emit7C->trailSlots = 0x32;
                    work->emit7C->trailStride = 0x1C;
                }
                if (work->emit7C != NULL) {
                    work->emit7C->otZ = 0x20;
                }
            }
            v.vx = 0x400;
            v.vy = 0;
            v.vz = 0;
            if (work->emit7C != NULL) {
                if (work->emit7C->state == BTLFX_EMIT_DONE) {
                    work->emit7C = NULL;
                }
                if (work->emit7C != NULL) {
                    work->emit7C->rot.vx = v.vx;
                    work->emit7C->rot.vy = v.vy;
                    work->emit7C->rot.vz = v.vz;
                }
            }
            ex->Hi_DrawEffModel(&work->rot34, (VECTOR *)work->unk28, (s32)work->unk3C, work->model68);
            s2.vx = s2.vz = work->unk3C[0] / 3;
            s2.vy = work->unk3C[1] / 2;
            ex->Hi_DrawEffModel(&work->rot34, (VECTOR *)work->unk28, (s32)&s2, work->model6C);
            ex->btlfx_drawScrollMove(frame * 0x10, 0x2C0, 0x100, 0x40, 0x80, 0x2C0, 0x180);
            ex->btlfx_drawScrollMove(work->unk4 * 0x10, 0x340, 0x100, 0x20, 0x80, 0x360, 0x100);
            if (frame >= 0x15) {
                work->unk0 = 6;
                entry->frame = -1;
                work->emit78 = work->emit7C = NULL;
                ex->btlfx_initParticleSystem();
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
            ex->Hi_GetSummonBonePos(SUMMON(env)->unk60, D_801EEC38[1], (u16 *)&work->pos10);
            BTLFX_COPY_SVEC(&work->pos18, &work->pos10);
            work->pos18.vy -= 0x46;
            work->pos18.vz -= 0x80;
            if (frame == 0xB) {
                work->emit78 = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, 4), (s32)&work->pos10);
                if (work->emit78 != NULL) {
                    work->emit78->trails = (s32)D_801EFD44;
                    work->emit78->trailSlots = 0x32;
                    work->emit78->trailStride = 0x1C;
                }
                if (work->emit78 != NULL) {
                    work->emit78->otZ = -0x80;
                }
                ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, 0xE), (s32)&work->pos10);
                if (work->emit78 != NULL) {
                    work->emit78->otZ = -0x80;
                }
            }
            v.vx = 0x400;
            v.vy = 0;
            v.vz = 0;
            if (work->emit78 != NULL) {
                if (work->emit78->state == BTLFX_EMIT_DONE) {
                    work->emit78 = NULL;
                }
                if (work->emit78 != NULL) {
                    work->emit78->rot.vx = v.vx;
                    work->emit78->rot.vy = v.vy;
                    work->emit78->rot.vz = v.vz;
                }
            }
            if (frame == 0) {
                work->emit7C = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, 6), (s32)&work->pos18);
                if (work->emit7C != NULL) {
                    work->emit7C->trails = (s32)D_801F02C4;
                    work->emit7C->trailSlots = 0x32;
                    work->emit7C->trailStride = 0x1C;
                }
                if (work->emit7C != NULL) {
                    work->emit7C->otZ = 0x20;
                }
            }
            v.vx = 0x400;
            v.vy = 0;
            v.vz = 0;
            if (work->emit7C != NULL) {
                if (work->emit7C->state == BTLFX_EMIT_DONE) {
                    work->emit7C = NULL;
                }
                if (work->emit7C != NULL) {
                    work->emit7C->rot.vx = v.vx;
                    work->emit7C->rot.vy = v.vy;
                    work->emit7C->rot.vz = v.vz;
                }
            }
            ex->Hi_DrawEffModel(&work->rot34, (VECTOR *)work->unk28, (s32)work->unk3C, work->model68);
            s2.vx = s2.vz = work->unk3C[0] / 3;
            s2.vy = work->unk3C[1] / 2;
            ex->Hi_DrawEffModel(&work->rot34, (VECTOR *)work->unk28, (s32)&s2, work->model6C);
            ex->btlfx_drawScrollMove(frame * 0x10, 0x2C0, 0x100, 0x40, 0x80, 0x2C0, 0x180);
            ex->btlfx_drawScrollMove(work->unk4 * 0x10, 0x340, 0x100, 0x20, 0x80, 0x360, 0x100);
            if (frame >= 0x15) {
                work->unk0 = 7;
                entry->frame = -1;
                ex->btlfx_initParticleSystem();
                work->emit78 = work->emit7C = NULL;
                ex->Hi_SetSummonMotion(SUMMON(env)->unk54->motion180[6], SUMMON(env)->unk60);
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
            ex->Hi_GetSummonBonePos(SUMMON(env)->unk60, 4, (u16 *)&work->pos10);
            work->pos10.vz += 0x226;
            if (frame == 0xA) {
                work->emit78 = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, 9), (s32)&work->pos10);
                if (work->emit78 != NULL) {
                    work->emit78->otZ = 0x80;
                }
            }
            v.vx = -0x270;
            v.vy = 0;
            v.vz = 0;
            if (work->emit78 != NULL) {
                if (work->emit78->state == BTLFX_EMIT_DONE) {
                    work->emit78 = NULL;
                }
                if (work->emit78 != NULL) {
                    work->emit78->rot.vx = v.vx;
                    work->emit78->rot.vy = v.vy;
                    work->emit78->rot.vz = v.vz;
                }
            }
            ex->Hi_DrawEffModel(&work->rot34, (VECTOR *)work->unk28, (s32)work->unk3C, work->model68);
            s2.vx = s2.vz = work->unk3C[0] / 3;
            s2.vy = work->unk3C[1] / 2;
            ex->Hi_DrawEffModel(&work->rot34, (VECTOR *)work->unk28, (s32)&s2, work->model6C);
            ex->btlfx_drawScrollMove(frame * 0x10, 0x2C0, 0x100, 0x40, 0x80, 0x2C0, 0x180);
            ex->btlfx_drawScrollMove(work->unk4 * 0x10, 0x340, 0x100, 0x20, 0x80, 0x360, 0x100);
            if (frame >= 0x20) {
                ex->btlfx_initParticleSystem();
                ex->btlfx_clearEffModels();
                return 1;
            }
            break;
        }
        }
        env->otZ = 0;
        work->unk4++;
    }
    return 0;
}
