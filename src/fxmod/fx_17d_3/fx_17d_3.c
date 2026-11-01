#include "fxmod/fx_17d_3.h"

INCLUDE_RODATA("asm/us/fx_17d_3/nonmatchings/fx_17d_3", fx_17d_3_headerPtr);

s32 fx_17d_3_op80(s32 mode, Fx17d3Work80 *work, Unk800F78A4 *env, Unk80100C78Entry *entry, s32 *args) {
    BtlfxExports *ex = BTLFX_EXPORTS(env);
    VECTOR sc;
    VECTOR pos;
    SVECTOR rot;
    Pool6CEntry *emitter;
    s32 frame;
    s32 t;
    s32 morph;
    s32 c;

    if (mode == 0) {
        *args = 0x80;
    } else if (mode == 1) {
        work->state0 = 0;
        work->frame4 = 0;
        fx_17d_3_env = env;
        ex->btlfx_initParticleSystem();
        ex->btlfx_initEffModels(fx_17d_3_effModels, 5);
        work->model68 = ex->btlfx_registerEffectModel((Unk800E8F50 *)ex->btlfx_getFileData(entry->file, 0x83), 0);
        work->model6C = ex->btlfx_registerEffectModel((Unk800E8F50 *)ex->btlfx_getFileData(entry->file, 0x81), 1);
        work->model70 = ex->btlfx_registerEffectModel((Unk800E8F50 *)ex->btlfx_getFileData(entry->file, 0x82), 0);
        work->model74 = ex->btlfx_registerEffectModel((Unk800E8F50 *)ex->btlfx_getFileData(entry->file, 0x80), 0);
        ex->Hi_SetSummonMotion(SUMMON(env)->summon54->motion180[1], SUMMON(env)->model60);
        work->pos48[1] = 0;
        work->pos48[0] = 0;
        work->pos48[2] = 0;
        work->scale54[0] = work->scale54[1] = work->scale54[2] = 0x1800;
        work->unk60.vx = 0;
        work->unk60.vy = 0;
        work->unk60.vz = 0;
        work->rot34.vx = 0;
        work->rot34.vy = 0;
        work->rot34.vz = 0;
        work->scale3C[0] = work->scale3C[1] = work->scale3C[2] = 0x800;
        work->pos28[0] = work->pos48[0];
        work->pos28[1] = work->pos48[1];
        work->pos28[2] = work->pos48[2];
        work->pos28[1] -= 0x200;
    } else {
        frame = entry->frame;
        switch (work->state0) {
        case 0: {
            VECTOR shakeRot;

            t = (frame << 12) / 24;
            if (frame == 0) {
                ex->btlfx_drawScreenTint((u32 *)(env->ot + 1), 0xFF, 0xFF, 0xFF, 0xFF);
            }
            ex->Hi_SetSummonMotFrame(SUMMON(env)->model60, 0);
            shakeRot.vx = work->pos48[0];
            shakeRot.vy = work->pos48[1];
            shakeRot.vz = work->pos48[2];
            shakeRot.vx += ex->btlfx_randCentered(0xE);
            shakeRot.vy += ex->btlfx_randCentered(0xE);
            shakeRot.vz += ex->btlfx_randCentered(0xE);
            ex->Hi_DrawSummonModel((SVECTOR *)&shakeRot, (VECTOR *)work->pos48, (s32)work->scale54, SUMMON(env)->model60, 0);
            ex->Hi_GetSummonBonePos(SUMMON(env)->model60, fx_17d_3_bones0[1], (u16 *)&work->pos20);
            work->pos20.vy -= frame * 12 - 0x2BC;
            env->camera->points[3].vx = work->pos20.vx;
            env->camera->points[3].vy = work->pos20.vy;
            env->camera->points[3].vz = work->pos20.vz;
            BTLFX_COPY_SVEC(&work->pos10, &work->pos20);
            work->pos10.vy += 0x300;
            if (frame == 0) {
                emitter = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, 2), (s32)&work->pos10);
                if (emitter != NULL) {
                    emitter->trails = (s32)fx_17d_3_op80Trails0;
                    emitter->trailSlots = 0x10;
                    emitter->trailStride = 0xCC;
                    emitter->otZ = 0x40;
                }
            }
            sc.vx = sc.vz = 0x555;
            sc.vy = 0xAAA;
            pos.vx = work->pos20.vx;
            pos.vy = work->pos20.vy;
            pos.vz = work->pos20.vz;
            pos.vx += 0x100 + ((0x1000 - t) >> 5);
            pos.vy += 0x100;
            rot.vx = 0;
            rot.vy = 0;
            rot.vz = 0;
            pos.vz -= 0x200;
            if (t < 0x800) {
                morph = 0x1000;
            } else {
                morph = 0x1000 - (t - 0x800) * 3 / 2;
            }
            ex->Hi_DrawMorphEffModel(&rot, &pos, (s32)&sc, work->model70, work->model74, morph);
            ex->Hi_ModifyEffModelAbr(work->model70, 1);
            ex->Hi_ModifyEffModelRGB(work->model70, 0, 0, 0);
            ex->Hi_DrawMorphEffModel(&rot, &pos, (s32)&sc, work->model70, work->model74, morph);
            ex->Hi_ModifyEffModelAbr(work->model70, 2);
            ex->Hi_ModifyEffModelRGB(work->model70, 0, 0, 0);
            ex->Hi_DrawEffModel(&work->rot34, (VECTOR *)work->pos28, (s32)work->scale3C, work->model68);
            ex->btlfx_drawScrollMove(frame * 0x18, 0x300, 0x140, 0x20, 0x80, 0x320, 0x140);
            ex->btlfx_drawScrollMove(frame * 0x10, 0x2C0, 0x100, 0x40, 0x80, 0x2C0, 0x180);
            {
            CVECTOR col;
            SVECTOR gridRot;
            VECTOR gridScale;

            col.r = col.g = col.b = ((0x1000 - t) * 100) >> 12;
            gridScale.vx = gridScale.vy = gridScale.vz = 0x1040;
            gridRot.vx = 0;
            gridRot.vy = 0;
            gridRot.vz = 0;
            ex->btlfx_drawScreenGrid(1, 0xA0, 0x70, 0x50, 0x38, 4, 4, 2, 2, &gridRot, &gridScale, &col, 1, 0);
            }
            if (frame >= 0x18) {
                work->state0 = 1;
                entry->frame = -1;
                ex->btlfx_initParticleSystem();
            }
            break;
        }
        case 1: {
            VECTOR shakeRot;

            t = (frame << 12) / 24;
            ex->Hi_SetSummonMotFrame(SUMMON(env)->model60, 0);
            shakeRot.vx = work->pos48[0];
            shakeRot.vy = work->pos48[1];
            shakeRot.vz = work->pos48[2];
            shakeRot.vx += ex->btlfx_randCentered(0xE);
            shakeRot.vy += ex->btlfx_randCentered(0xE);
            shakeRot.vz += ex->btlfx_randCentered(0xE);
            ex->Hi_DrawSummonModel((SVECTOR *)&shakeRot, (VECTOR *)work->pos48, (s32)work->scale54, SUMMON(env)->model60, 0);
            ex->Hi_GetSummonBonePos(SUMMON(env)->model60, fx_17d_3_bones0[0], (u16 *)&work->pos20);
            work->pos20.vy -= frame * 12 - 0x2BC;
            env->camera->points[3].vx = work->pos20.vx;
            env->camera->points[3].vy = work->pos20.vy;
            env->camera->points[3].vz = work->pos20.vz;
            BTLFX_COPY_SVEC(&work->pos10, &work->pos20);
            work->pos10.vy += 0x300;
            if (frame == 0) {
                emitter = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, 2), (s32)&work->pos10);
                if (emitter != NULL) {
                    emitter->trails = (s32)fx_17d_3_op80Trails0;
                    emitter->trailSlots = 0x10;
                    emitter->trailStride = 0xCC;
                    emitter->otZ = 0x40;
                }
            }
            sc.vx = sc.vz = 0x555;
            sc.vy = 0xAAA;
            pos.vx = work->pos20.vx;
            pos.vy = work->pos20.vy;
            pos.vz = work->pos20.vz;
            pos.vx -= 0x100 + ((0x1000 - t) >> 5);
            pos.vy += 0x100;
            rot.vx = 0;
            rot.vy = 0;
            rot.vz = 0;
            pos.vz -= 0x200;
            if (t < 0x800) {
                morph = 0x1000;
            } else {
                morph = 0x1000 - (t - 0x800) * 3 / 2;
            }
            ex->Hi_DrawMorphEffModel(&rot, &pos, (s32)&sc, work->model70, work->model74, morph);
            ex->Hi_ModifyEffModelAbr(work->model70, 1);
            ex->Hi_ModifyEffModelRGB(work->model70, 0, 0, 0);
            ex->Hi_DrawMorphEffModel(&rot, &pos, (s32)&sc, work->model70, work->model74, morph);
            ex->Hi_ModifyEffModelAbr(work->model70, 2);
            ex->Hi_ModifyEffModelRGB(work->model70, 0, 0, 0);
            ex->Hi_DrawEffModel(&work->rot34, (VECTOR *)work->pos28, (s32)work->scale3C, work->model68);
            ex->btlfx_drawScrollMove(frame * 0x18, 0x300, 0x140, 0x20, 0x80, 0x320, 0x140);
            ex->btlfx_drawScrollMove(frame * 0x10, 0x2C0, 0x100, 0x40, 0x80, 0x2C0, 0x180);
            {
            CVECTOR col;
            SVECTOR gridRot;
            VECTOR gridScale;

            col.r = col.g = col.b = ((0x1000 - t) * 100) >> 12;
            gridScale.vx = gridScale.vy = gridScale.vz = 0x1040;
            gridRot.vx = 0;
            gridRot.vy = 0;
            gridRot.vz = 0;
            ex->btlfx_drawScreenGrid(1, 0xA0, 0x70, 0x50, 0x38, 4, 4, 2, 2, &gridRot, &gridScale, &col, 1, 0);
            }
            if (frame >= 0x18) {
                work->state0 = 2;
                entry->frame = -1;
                ex->btlfx_initParticleSystem();
                work->emit78 = work->emit7C = NULL;
                ex->Hi_SetSummonMotion(SUMMON(env)->summon54->motion180[2], SUMMON(env)->model60);
            }
            break;
        }
        case 2: {
            VECTOR shakeRot;
            SVECTOR emitRot;
            VECTOR haloScale;

            if (frame < 10) {
                ex->Hi_SetSummonMotFrame(SUMMON(env)->model60, 0);
            }
            if (frame >= 20) {
                ex->Hi_SetSummonMotFrame(SUMMON(env)->model60, 10);
            }
            shakeRot.vx = work->pos48[0];
            shakeRot.vy = work->pos48[1];
            shakeRot.vz = work->pos48[2];
            shakeRot.vx += ex->btlfx_randCentered(8);
            shakeRot.vy += ex->btlfx_randCentered(8);
            shakeRot.vz += ex->btlfx_randCentered(8);
            ex->Hi_DrawSummonModel((SVECTOR *)&shakeRot, (VECTOR *)work->pos48, (s32)work->scale54, SUMMON(env)->model60, 0);
            if (frame == 0) {
                ex->Hi_GetSummonBonePos(SUMMON(env)->model60, fx_17d_3_bones1[0], (u16 *)&work->pos20);
                work->pos20.vx = work->pos48[0];
                work->pos20.vz = work->pos48[2];
                env->camera->points[3].vx = work->pos20.vx;
                env->camera->points[3].vy = work->pos20.vy;
                env->camera->points[3].vz = work->pos20.vz;
            }
            ex->Hi_GetSummonBonePos(SUMMON(env)->model60, fx_17d_3_bones1[0], (u16 *)&work->pos10);
            ex->Hi_GetSummonBonePos(SUMMON(env)->model60, fx_17d_3_bones1[1], (u16 *)&work->pos18);
            ex->Hi_DrawEffModel(&work->rot34, (VECTOR *)work->pos28, (s32)work->scale3C, work->model68);
            haloScale.vx = haloScale.vz = work->scale3C[0] / 4;
            haloScale.vy = work->scale3C[1] / 2;
            ex->Hi_DrawEffModel(&work->rot34, (VECTOR *)work->pos28, (s32)&haloScale, work->model6C);
            if (frame == 0xC) {
                work->emit78 = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, 1), (s32)&work->pos10);
                if (work->emit78 != NULL) {
                    work->emit78->trails = (s32)fx_17d_3_op80Trails1;
                    work->emit78->trailSlots = 0x32;
                    work->emit78->trailStride = 0x1C;
                }
                if (work->emit78 != NULL) {
                    work->emit78->otZ = 0x20;
                }
                work->emit7C = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, 1), (s32)&work->pos18);
                if (work->emit7C != NULL) {
                    work->emit7C->trails = (s32)fx_17d_3_op80Trails2;
                    work->emit7C->trailSlots = 0x32;
                    work->emit7C->trailStride = 0x1C;
                }
                if (work->emit7C != NULL) {
                    work->emit7C->otZ = 0x20;
                }
            }
            emitRot.vx = 0xA00;
            emitRot.vy = 0;
            emitRot.vz = 0;
            if (work->emit78 != NULL) {
                if (work->emit78->state == BTLFX_EMIT_DONE) {
                    work->emit78 = NULL;
                }
                if (work->emit78 != NULL) {
                    work->emit78->rot.vx = emitRot.vx;
                    work->emit78->rot.vy = emitRot.vy;
                    work->emit78->rot.vz = emitRot.vz;
                }
            }
            if (work->emit7C != NULL) {
                if (work->emit7C->state == BTLFX_EMIT_DONE) {
                    work->emit7C = NULL;
                }
                if (work->emit7C != NULL) {
                    work->emit7C->rot.vx = emitRot.vx;
                    work->emit7C->rot.vy = emitRot.vy;
                    work->emit7C->rot.vz = emitRot.vz;
                }
            }
            ex->btlfx_drawScrollMove(frame * 0x10, 0x2C0, 0x100, 0x40, 0x80, 0x2C0, 0x180);
            ex->btlfx_drawScrollMove(work->frame4 * 0x10, 0x340, 0x100, 0x20, 0x80, 0x360, 0x100);
            if (frame >= 0x15) {
                work->state0 = 3;
                entry->frame = -1;
                work->emit78 = work->emit7C = NULL;
                work->pos28[1] -= 0x64;
                ex->Hi_SetSummonMotion(SUMMON(env)->summon54->motion180[4], SUMMON(env)->model60);
            }
            break;
        }
        case 3: {
            VECTOR shakeRot;
            VECTOR haloScale;
            SVECTOR emitRot;

            if (frame < 10) {
                ex->Hi_SetSummonMotFrame(SUMMON(env)->model60, 0);
            }
            if (frame >= 17) {
                ex->Hi_SetSummonMotFrame(SUMMON(env)->model60, 7);
            }
            shakeRot.vx = work->pos48[0];
            shakeRot.vy = work->pos48[1];
            shakeRot.vz = work->pos48[2];
            shakeRot.vx += ex->btlfx_randCentered(6);
            shakeRot.vy += ex->btlfx_randCentered(6);
            shakeRot.vz += ex->btlfx_randCentered(6);
            ex->Hi_DrawSummonModel((SVECTOR *)&shakeRot, (VECTOR *)work->pos48, (s32)work->scale54, SUMMON(env)->model60, 0);
            ex->Hi_GetSummonBonePos(SUMMON(env)->model60, fx_17d_3_bones0[0], (u16 *)&work->pos10);
            ex->Hi_GetSummonBonePos(SUMMON(env)->model60, fx_17d_3_bones0[1], (u16 *)&work->pos18);
            if (frame < 18) {
                work->pos20.vy += (ex->math_cos((frame << 10) / 18) * 0x29) >> 12;
                env->camera->points[3].vx = work->pos20.vx;
                env->camera->points[3].vy = work->pos20.vy;
                env->camera->points[3].vz = work->pos20.vz;
            }
            ex->Hi_DrawEffModel(&work->rot34, (VECTOR *)work->pos28, (s32)work->scale3C, work->model68);
            haloScale.vx = haloScale.vz = work->scale3C[0] / 2;
            haloScale.vy = work->scale3C[1] / 2;
            ex->Hi_DrawEffModel(&work->rot34, (VECTOR *)work->pos28, (s32)&haloScale, work->model6C);
            if (frame == 0xC) {
                work->emit78 = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, 6), (s32)&work->pos10);
                if (work->emit78 != NULL) {
                    work->emit78->trails = (s32)fx_17d_3_op80Trails1;
                    work->emit78->trailSlots = 0x32;
                    work->emit78->trailStride = 0x1C;
                }
                if (work->emit78 != NULL) {
                    work->emit78->otZ = 0x20;
                }
                work->emit7C = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, 6), (s32)&work->pos18);
                if (work->emit7C != NULL) {
                    work->emit7C->trails = (s32)fx_17d_3_op80Trails2;
                    work->emit7C->trailSlots = 0x32;
                    work->emit7C->trailStride = 0x1C;
                }
                if (work->emit7C != NULL) {
                    work->emit7C->otZ = 0x20;
                }
            }
            emitRot.vx = 0xA00;
            emitRot.vy = 0;
            emitRot.vz = 0;
            if (work->emit78 != NULL) {
                if (work->emit78->state == BTLFX_EMIT_DONE) {
                    work->emit78 = NULL;
                }
                if (work->emit78 != NULL) {
                    work->emit78->rot.vx = emitRot.vx;
                    work->emit78->rot.vy = emitRot.vy;
                    work->emit78->rot.vz = emitRot.vz;
                }
            }
            if (work->emit7C != NULL) {
                if (work->emit7C->state == BTLFX_EMIT_DONE) {
                    work->emit7C = NULL;
                }
                if (work->emit7C != NULL) {
                    work->emit7C->rot.vx = emitRot.vx;
                    work->emit7C->rot.vy = emitRot.vy;
                    work->emit7C->rot.vz = emitRot.vz;
                }
            }
            ex->btlfx_drawScrollMove(frame * 0x10, 0x2C0, 0x100, 0x40, 0x80, 0x2C0, 0x180);
            ex->btlfx_drawScrollMove(work->frame4 * 0x10, 0x340, 0x100, 0x20, 0x80, 0x360, 0x100);
            if (frame >= 0x15) {
                work->state0 = 4;
                entry->frame = -1;
                work->emit78 = work->emit7C = NULL;
                ex->Hi_SetSummonMotion(SUMMON(env)->summon54->motion180[5], SUMMON(env)->model60);
                work->pos28[1] -= 0xC18;
                ex->Hi_GetSummonBonePos(SUMMON(env)->model60, 0x21, (u16 *)&work->pos20);
                work->pos20.vy -= 0x46;
                env->camera->points[2].vx = work->pos20.vx;
                env->camera->points[2].vy = work->pos20.vy;
                env->camera->points[2].vz = work->pos20.vz;
            }
            break;
        }
        case 4: {
            VECTOR shakeRot;
            VECTOR haloScale;
            SVECTOR emitRot;

            if (frame == 0) {
                ex->btlfx_initParticleSystem();
            }
            shakeRot.vx = work->pos48[0];
            shakeRot.vy = work->pos48[1];
            shakeRot.vz = work->pos48[2];
            shakeRot.vx += ex->btlfx_randCentered(2);
            shakeRot.vy += ex->btlfx_randCentered(2);
            shakeRot.vz += ex->btlfx_randCentered(2);
            ex->Hi_DrawSummonModel((SVECTOR *)&shakeRot, (VECTOR *)work->pos48, (s32)work->scale54, SUMMON(env)->model60, 0);
            ex->Hi_GetSummonBonePos(SUMMON(env)->model60, fx_17d_3_bones2[0], (u16 *)&work->pos10);
            ex->Hi_GetSummonBonePos(SUMMON(env)->model60, fx_17d_3_bones2[1], (u16 *)&work->pos18);
            if (frame == 6) {
                work->emit78 = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, 6), (s32)&work->pos10);
                if (work->emit78 != NULL) {
                    work->emit78->trails = (s32)fx_17d_3_op80Trails1;
                    work->emit78->trailSlots = 0x32;
                    work->emit78->trailStride = 0x1C;
                }
                if (work->emit78 != NULL) {
                    work->emit78->otZ = 0x20;
                }
                work->emit7C = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, 6), (s32)&work->pos18);
                if (work->emit7C != NULL) {
                    work->emit7C->trails = (s32)fx_17d_3_op80Trails2;
                    work->emit7C->trailSlots = 0x32;
                    work->emit7C->trailStride = 0x1C;
                }
                if (work->emit7C != NULL) {
                    work->emit7C->otZ = 0x20;
                }
            }
            emitRot.vx = 0x400;
            emitRot.vy = 0;
            emitRot.vz = 0;
            if (work->emit78 != NULL) {
                if (work->emit78->state == BTLFX_EMIT_DONE) {
                    work->emit78 = NULL;
                }
                if (work->emit78 != NULL) {
                    work->emit78->rot.vx = emitRot.vx;
                    work->emit78->rot.vy = emitRot.vy;
                    work->emit78->rot.vz = emitRot.vz;
                }
            }
            if (work->emit7C != NULL) {
                if (work->emit7C->state == BTLFX_EMIT_DONE) {
                    work->emit7C = NULL;
                }
                if (work->emit7C != NULL) {
                    work->emit7C->rot.vx = emitRot.vx;
                    work->emit7C->rot.vy = emitRot.vy;
                    work->emit7C->rot.vz = emitRot.vz;
                }
            }
            ex->Hi_DrawEffModel(&work->rot34, (VECTOR *)work->pos28, (s32)work->scale3C, work->model68);
            haloScale.vx = haloScale.vz = work->scale3C[0] / 2;
            haloScale.vy = work->scale3C[1] / 2;
            ex->Hi_DrawEffModel(&work->rot34, (VECTOR *)work->pos28, (s32)&haloScale, work->model6C);
            ex->btlfx_drawScrollMove(frame * 0x10, 0x2C0, 0x100, 0x40, 0x80, 0x2C0, 0x180);
            ex->btlfx_drawScrollMove(work->frame4 * 0x10, 0x340, 0x100, 0x20, 0x80, 0x360, 0x100);
            if (frame >= 0x18) {
                work->state0 = 5;
                entry->frame = -1;
                ex->btlfx_initParticleSystem();
                work->emit78 = work->emit7C = NULL;
                work->pos28[1] -= 0x384;
                ex->Hi_SetSummonMotFrame(SUMMON(env)->model60, 0x18);
            }
            break;
        }
        case 5: {
            VECTOR shakeRot;
            VECTOR haloScale;
            SVECTOR emitRot;

            shakeRot.vx = work->pos48[0];
            shakeRot.vy = work->pos48[1];
            shakeRot.vz = work->pos48[2];
            shakeRot.vx += ex->btlfx_randCentered(1);
            shakeRot.vy += ex->btlfx_randCentered(1);
            shakeRot.vz += ex->btlfx_randCentered(1);
            ex->Hi_DrawSummonModel((SVECTOR *)&shakeRot, (VECTOR *)work->pos48, (s32)work->scale54, SUMMON(env)->model60, 0);
            ex->Hi_GetSummonBonePos(SUMMON(env)->model60, fx_17d_3_bones3[0], (u16 *)&work->pos20);
            work->pos20.vy -= 0xAA;
            env->camera->points[3].vx = work->pos20.vx;
            env->camera->points[3].vy = work->pos20.vy;
            env->camera->points[3].vz = work->pos20.vz;
            ex->Hi_GetSummonBonePos(SUMMON(env)->model60, fx_17d_3_bones3[0], (u16 *)&work->pos10);
            BTLFX_COPY_SVEC(&work->pos18, &work->pos10);
            work->pos18.vy -= 0x46;
            work->pos18.vz -= 0x80;
            if (frame == 0xB) {
                work->emit78 = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, 4), (s32)&work->pos10);
                if (work->emit78 != NULL) {
                    work->emit78->trails = (s32)fx_17d_3_op80Trails1;
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
            emitRot.vx = 0x400;
            emitRot.vy = 0;
            emitRot.vz = 0;
            if (work->emit78 != NULL) {
                if (work->emit78->state == BTLFX_EMIT_DONE) {
                    work->emit78 = NULL;
                }
                if (work->emit78 != NULL) {
                    work->emit78->rot.vx = emitRot.vx;
                    work->emit78->rot.vy = emitRot.vy;
                    work->emit78->rot.vz = emitRot.vz;
                }
            }
            if (frame == 0) {
                work->emit7C = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, 6), (s32)&work->pos18);
                if (work->emit7C != NULL) {
                    work->emit7C->trails = (s32)fx_17d_3_op80Trails2;
                    work->emit7C->trailSlots = 0x32;
                    work->emit7C->trailStride = 0x1C;
                }
                if (work->emit7C != NULL) {
                    work->emit7C->otZ = 0x20;
                }
            }
            emitRot.vx = 0x400;
            emitRot.vy = 0;
            emitRot.vz = 0;
            if (work->emit7C != NULL) {
                if (work->emit7C->state == BTLFX_EMIT_DONE) {
                    work->emit7C = NULL;
                }
                if (work->emit7C != NULL) {
                    work->emit7C->rot.vx = emitRot.vx;
                    work->emit7C->rot.vy = emitRot.vy;
                    work->emit7C->rot.vz = emitRot.vz;
                }
            }
            ex->Hi_DrawEffModel(&work->rot34, (VECTOR *)work->pos28, (s32)work->scale3C, work->model68);
            haloScale.vx = haloScale.vz = work->scale3C[0] / 3;
            haloScale.vy = work->scale3C[1] / 2;
            ex->Hi_DrawEffModel(&work->rot34, (VECTOR *)work->pos28, (s32)&haloScale, work->model6C);
            ex->btlfx_drawScrollMove(frame * 0x10, 0x2C0, 0x100, 0x40, 0x80, 0x2C0, 0x180);
            ex->btlfx_drawScrollMove(work->frame4 * 0x10, 0x340, 0x100, 0x20, 0x80, 0x360, 0x100);
            if (frame >= 0x15) {
                work->state0 = 6;
                entry->frame = -1;
                work->emit78 = work->emit7C = NULL;
                ex->btlfx_initParticleSystem();
                ex->Hi_SetSummonMotFrame(SUMMON(env)->model60, 0x18);
                ex->Hi_GetSummonBonePos(SUMMON(env)->model60, fx_17d_3_bones3[1], (u16 *)&work->pos20);
                work->pos20.vy -= 0xAA;
                env->camera->points[3].vx = work->pos20.vx;
                env->camera->points[3].vy = work->pos20.vy;
                env->camera->points[3].vz = work->pos20.vz;
            }
            break;
        }
        case 6: {
            VECTOR shakeRot;
            VECTOR haloScale;
            SVECTOR emitRot;

            shakeRot.vx = work->pos48[0];
            shakeRot.vy = work->pos48[1];
            shakeRot.vz = work->pos48[2];
            shakeRot.vx += ex->btlfx_randCentered(1);
            shakeRot.vy += ex->btlfx_randCentered(1);
            shakeRot.vz += ex->btlfx_randCentered(1);
            ex->Hi_DrawSummonModel((SVECTOR *)&shakeRot, (VECTOR *)work->pos48, (s32)work->scale54, SUMMON(env)->model60, 0);
            ex->Hi_GetSummonBonePos(SUMMON(env)->model60, fx_17d_3_bones3[1], (u16 *)&work->pos20);
            work->pos20.vy -= 0xAA;
            env->camera->points[3].vx = work->pos20.vx;
            env->camera->points[3].vy = work->pos20.vy;
            env->camera->points[3].vz = work->pos20.vz;
            ex->Hi_GetSummonBonePos(SUMMON(env)->model60, fx_17d_3_bones3[1], (u16 *)&work->pos10);
            BTLFX_COPY_SVEC(&work->pos18, &work->pos10);
            work->pos18.vy -= 0x46;
            work->pos18.vz -= 0x80;
            if (frame == 0xB) {
                work->emit78 = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, 4), (s32)&work->pos10);
                if (work->emit78 != NULL) {
                    work->emit78->trails = (s32)fx_17d_3_op80Trails1;
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
            emitRot.vx = 0x400;
            emitRot.vy = 0;
            emitRot.vz = 0;
            if (work->emit78 != NULL) {
                if (work->emit78->state == BTLFX_EMIT_DONE) {
                    work->emit78 = NULL;
                }
                if (work->emit78 != NULL) {
                    work->emit78->rot.vx = emitRot.vx;
                    work->emit78->rot.vy = emitRot.vy;
                    work->emit78->rot.vz = emitRot.vz;
                }
            }
            if (frame == 0) {
                work->emit7C = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, 6), (s32)&work->pos18);
                if (work->emit7C != NULL) {
                    work->emit7C->trails = (s32)fx_17d_3_op80Trails2;
                    work->emit7C->trailSlots = 0x32;
                    work->emit7C->trailStride = 0x1C;
                }
                if (work->emit7C != NULL) {
                    work->emit7C->otZ = 0x20;
                }
            }
            emitRot.vx = 0x400;
            emitRot.vy = 0;
            emitRot.vz = 0;
            if (work->emit7C != NULL) {
                if (work->emit7C->state == BTLFX_EMIT_DONE) {
                    work->emit7C = NULL;
                }
                if (work->emit7C != NULL) {
                    work->emit7C->rot.vx = emitRot.vx;
                    work->emit7C->rot.vy = emitRot.vy;
                    work->emit7C->rot.vz = emitRot.vz;
                }
            }
            ex->Hi_DrawEffModel(&work->rot34, (VECTOR *)work->pos28, (s32)work->scale3C, work->model68);
            haloScale.vx = haloScale.vz = work->scale3C[0] / 3;
            haloScale.vy = work->scale3C[1] / 2;
            ex->Hi_DrawEffModel(&work->rot34, (VECTOR *)work->pos28, (s32)&haloScale, work->model6C);
            ex->btlfx_drawScrollMove(frame * 0x10, 0x2C0, 0x100, 0x40, 0x80, 0x2C0, 0x180);
            ex->btlfx_drawScrollMove(work->frame4 * 0x10, 0x340, 0x100, 0x20, 0x80, 0x360, 0x100);
            if (frame >= 0x15) {
                work->state0 = 7;
                entry->frame = -1;
                ex->btlfx_initParticleSystem();
                work->emit78 = work->emit7C = NULL;
                ex->Hi_SetSummonMotion(SUMMON(env)->summon54->motion180[6], SUMMON(env)->model60);
                ex->Hi_GetSummonBonePos(SUMMON(env)->model60, 4, (u16 *)&work->pos20);
                work->pos20.vy -= 0x380;
                env->camera->points[3].vx = work->pos20.vx;
                env->camera->points[3].vy = work->pos20.vy;
                env->camera->points[3].vz = work->pos20.vz;
            }
            break;
        }
        case 7: {
            VECTOR shakeRot;
            VECTOR haloScale;
            SVECTOR emitRot;

            t = frame << 7;
            if (frame >= 0x17) {
                ex->Hi_SetSummonMotFrame(SUMMON(env)->model60, 0x17);
            }
            shakeRot.vx = work->pos48[0];
            shakeRot.vy = work->pos48[1];
            shakeRot.vz = work->pos48[2];
            ex->Hi_DrawSummonModel((SVECTOR *)&shakeRot, (VECTOR *)work->pos48, (s32)work->scale54, SUMMON(env)->model60, 0);
            ex->Hi_GetSummonBonePos(SUMMON(env)->model60, 4, (u16 *)&work->pos20);
            if (t < 0x800) {
                work->pos20.vy -= ((0x1000 - ex->math_sin(t >> 1)) >> 2) - 0x80;
            } else {
                work->pos20.vy += 0x80;
            }
            env->camera->points[3].vx = work->pos20.vx;
            env->camera->points[3].vy = work->pos20.vy;
            env->camera->points[3].vz = work->pos20.vz;
            ex->Hi_GetSummonBonePos(SUMMON(env)->model60, 4, (u16 *)&work->pos10);
            work->pos10.vz += 0x226;
            if (frame == 0xA) {
                work->emit78 = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, 9), (s32)&work->pos10);
                if (work->emit78 != NULL) {
                    work->emit78->otZ = 0x80;
                }
            }
            emitRot.vx = -0x270;
            emitRot.vy = 0;
            emitRot.vz = 0;
            if (work->emit78 != NULL) {
                if (work->emit78->state == BTLFX_EMIT_DONE) {
                    work->emit78 = NULL;
                }
                if (work->emit78 != NULL) {
                    work->emit78->rot.vx = emitRot.vx;
                    work->emit78->rot.vy = emitRot.vy;
                    work->emit78->rot.vz = emitRot.vz;
                }
            }
            ex->Hi_DrawEffModel(&work->rot34, (VECTOR *)work->pos28, (s32)work->scale3C, work->model68);
            haloScale.vx = haloScale.vz = work->scale3C[0] / 3;
            haloScale.vy = work->scale3C[1] / 2;
            ex->Hi_DrawEffModel(&work->rot34, (VECTOR *)work->pos28, (s32)&haloScale, work->model6C);
            ex->btlfx_drawScrollMove(frame * 0x10, 0x2C0, 0x100, 0x40, 0x80, 0x2C0, 0x180);
            ex->btlfx_drawScrollMove(work->frame4 * 0x10, 0x340, 0x100, 0x20, 0x80, 0x360, 0x100);
            if (frame >= 0x20) {
                ex->btlfx_initParticleSystem();
                ex->btlfx_clearEffModels();
                return 1;
            }
            break;
        }
        }
        env->otZ = 0;
        work->frame4++;
    }
    return 0;
}
