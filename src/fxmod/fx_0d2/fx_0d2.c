#include "fxmod/fx_0d2.h"

static void fxm_buildPathTrail(Fx0d2Keys *keys, u8 *buf, s32 size, s32 start, s32 n, s32 from, s32 to, s32 clip) {
    SVECTOR v;
    SVECTOR *p;
    s32 cnt;
    s32 i;
    s32 j;
    s32 k;

    cnt = (size - 4) / 8;
    p = (SVECTOR *)(buf + 4);
    buf[0] = buf[2] = cnt - 1;
    buf[1] = 0;
    if (n < cnt) {
        for (i = 0; i < cnt - n; i++) {
            p->pad = 0;
            p++;
        }
    } else {
        n = cnt;
    }
    for (i = 0; i < n; i++) {
        k = i + 1;
        j = n - k + start;
        if (clip != 0 && j >= keys->count2) {
            break;
        }
        fxm_posPathGet(keys, j, &v);
        p->vx = v.vx;
        p->vy = v.vy;
        p->vz = v.vz;
        p->pad = from + (to - from) / keys->count2 * (n - i);
        p++;
    }
}

s32 fx_0d2_op80(s32 mode, Fx0d2Work80 *work, Unk800F78A4 *env, Unk80100C78Entry *entry, s32 *arg4) {
    BtlfxExports *ex = BTLFX_EXPORTS(env);
    SVECTOR v;
    SVECTOR w;
    s32 frame;
    s32 k;

    if (mode == 0) {
        *arg4 = sizeof(Fx0d2Work80);
    } else if (mode == 1) {
        fx_0d2_env = env;
        ex->btlfx_initEffModels(fx_0d2_effModels, 0x20);
        work->rot14.vx = 0;
        work->rot14.vy = 0x800;
        work->rot14.vz = 0;
        work->summonScale2C.vx = 0x266;
        work->summonScale2C.vy = 0x266;
        work->summonScale2C.vz = 0x266;
        work->summonPos1C.vx = 0;
        work->summonPos1C.vy = 0;
        work->summonPos1C.vz = 0;
        work->rot3C.vx = 0;
        work->rot3C.vy = 0x800;
        work->rot3C.vz = 0;
        work->pos44.vx = 0;
        work->pos44.vy = 0;
        work->pos44.vz = 0;
        work->scale54.vx = 0x1000;
        work->scale54.vy = 0x1000;
        work->scale54.vz = 0x1000;
        work->modelA4 = ex->btlfx_registerEffectModel((Unk800E8F50 *)ex->btlfx_getFileData(entry->file, 0x85), 1);
        fxm_camChanReset();
        fxm_camChanAdd(0, 1, 0, (s16 *)ex->btlfx_getFileData(entry->file, 0x9A));
        fxm_camChanAdd(0, 2, 0, (s16 *)ex->btlfx_getFileData(entry->file, 0x96));
        fxm_camChanAdd(0, 3, 0, (s16 *)ex->btlfx_getFileData(entry->file, 0x97));
        fxm_camChanAdd(0, 4, 0, (s16 *)ex->btlfx_getFileData(entry->file, 0x94));
        fxm_camChanAdd(0, 9, 0, (s16 *)ex->btlfx_getFileData(entry->file, 0x9D));
        fxm_camChanAdd(0, 0xA, 0, (s16 *)ex->btlfx_getFileData(entry->file, 0x93));
        fxm_camChanAdd(0x68, 1, 0, (s16 *)ex->btlfx_getFileData(entry->file, 0x99));
        fxm_camChanAdd(0x68, 2, 0, (s16 *)ex->btlfx_getFileData(entry->file, 0x95));
        fxm_camChanAdd(0x67, 3, 0, (s16 *)ex->btlfx_getFileData(entry->file, 0x9C));
        fxm_camChanAdd(0x67, 4, 0, (s16 *)ex->btlfx_getFileData(entry->file, 0x9B));
        fxm_camChanAdd(0x68, 0xA, 0, (s16 *)ex->btlfx_getFileData(entry->file, 0x98));
        fxm_camChanAdd(0xB6, 1, 0, (s16 *)ex->btlfx_getFileData(entry->file, 0xA0));
        fxm_camChanAdd(0xB6, 2, 0, (s16 *)ex->btlfx_getFileData(entry->file, 0xA1));
        fxm_camChanAdd(0xB5, 3, 0, (s16 *)ex->btlfx_getFileData(entry->file, 0xA2));
        fxm_camChanAdd(0xB5, 4, 0, (s16 *)ex->btlfx_getFileData(entry->file, 0xA3));
        fxm_camChanAdd(0xDE, 1, 0, (s16 *)ex->btlfx_getFileData(entry->file, 0x1F));
        fxm_camChanAdd(0xDE, 2, 0, (s16 *)ex->btlfx_getFileData(entry->file, 0x1D));
        fxm_camChanAdd(0xDD, 3, 0, (s16 *)ex->btlfx_getFileData(entry->file, 0x1E));
        fxm_camChanAdd(0xDD, 4, 0, (s16 *)ex->btlfx_getFileData(entry->file, 0x20));
    } else {
        frame = entry->frame;
        switch (frame) {
        case 1:
            work->unk94 = 0;
            work->unk98 = frame;
            ex->Hi_SetSummonMotion(((Fx0d2Summon *)env->loader)->unk54->motion180, ((Fx0d2Summon *)env->loader)->model60);
            work->pathFrame9C = frame;
            fxm_posPathInit((Fx0d2Path *)&fx_0d2_summonPath, (u16 *)ex->btlfx_getFileData(entry->file, 0x92));
            break;
        case 0x29:
            work->unk94 = 1;
            work->unk98 = 0;
            ex->Hi_SetSummonMotion(((Fx0d2Summon *)env->loader)->unk54->motion184, ((Fx0d2Summon *)env->loader)->model60);
            break;
        case 0x51:
            work->unk94 = 2;
            work->unk98 = 1;
            ex->Hi_SetSummonMotion(((Fx0d2Summon *)env->loader)->unk54->motion188, ((Fx0d2Summon *)env->loader)->model60);
            break;
        case 0x69:
            work->summonScale2C.vx = 0x433;
            work->summonScale2C.vy = 0x433;
            work->summonScale2C.vz = 0x433;
            work->scale54.vx = 0x1C00;
            work->scale54.vy = 0x1C00;
            work->scale54.vz = 0x1C00;
            work->pathFrame9C = 0;
            fxm_posPathInit((Fx0d2Path *)&fx_0d2_summonPath, (u16 *)ex->btlfx_getFileData(entry->file, 0x9F));
            break;
        case 0xB6:
            fxm_rotPosPathInit((Fx0d2Path *)&fx_0d2_slicePath, (u16 *)ex->btlfx_getFileData(entry->file, 0xA7));
            break;
        }
        switch (frame) {
        case 1:
            work->emit0 = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, 0xA4), (s32)&fx_0d2_emitPos);
            break;
        case 0x52:
            work->pos74.vx = 0x88;
            work->pos74.vy = -0x366;
            work->pos74.vz = -0x41F;
            work->pos7C.vx = -0x88;
            work->pos7C.vy = -0x366;
            work->pos7C.vz = -0x41F;
            work->emit4 = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, 0xA5), (s32)&work->pos74);
            work->emit8 = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, 0xA5), (s32)&work->pos7C);
            if (work->emit4 != NULL) {
                work->emit4->otZ = 0x80;
            }
            if (work->emit8 != NULL) {
                work->emit8->otZ = 0x80;
            }
            break;
        case 0x69:
            ex->btlfx_stopEmitter(work->emit0);
            ex->btlfx_stopEmitter(work->emit4);
            ex->btlfx_stopEmitter(work->emit8);
            fxm_posPathInit((Fx0d2Path *)&fx_0d2_emitPath, (u16 *)ex->btlfx_getFileData(entry->file, 0x82));
            work->emitC = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, 0x83), (s32)&fx_0d2_emitPos2[0]);
            work->emit10 = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, 0x83), (s32)&fx_0d2_emitPos2[1]);
            if (work->emitC != NULL) {
                work->emitC->otZ = 0x100;
            }
            if (work->emit10 != NULL) {
                work->emit10->otZ = 0x100;
            }
            break;
        }
        k = frame - 1;
        if ((u32)k < 0x83) {
            fxm_posPathGet((Fx0d2Path *)&fx_0d2_summonPath, work->pathFrame9C, &v);
            work->summonPos1C.vx = v.vx;
            work->summonPos1C.vy = v.vy;
            work->summonPos1C.vz = v.vz;
            ex->Hi_DrawSummonModel(&work->rot14, (VECTOR *)&work->summonPos1C, (s32)&work->summonScale2C,
                                   ((Fx0d2Summon *)env->loader)->model60, work->unk98);
            work->pathFrame9C++;
        }
        if ((u32)k < 0xDE) {
            if (frame < 0xB6) {
                work->pos44.vx = env->camera->pointRots[3].vx;
                work->pos44.vy = env->camera->pointRots[3].vy;
                work->pos44.vz = env->camera->pointRots[3].vz;
            } else {
                fxm_rotPosPathGet((Fx0d2KeyPath *)&fx_0d2_slicePath, frame - 0xB6, &work->rot3C, &w);
                work->pos44.vx = w.vx;
                work->pos44.vy = w.vy;
                work->pos44.vz = w.vz;
            }
            ex->Hi_SetEffModelSlice(work->modelA4, work->pos44.vy);
            ex->Hi_DrawSliceEffModel(&work->rot3C, (VECTOR *)&work->pos44, (s32)&work->scale54, work->modelA4);
            if (k < 0x69) {
                ex->Hi_ModifyEffModelRGB(work->modelA4, -0x30, -0x30, -0x30);
            }
        }
        if (frame > 0) {
            work->pos6C.vx = env->camera->pointRots[2].vx;
            work->pos6C.vy = env->camera->pointRots[2].vy;
            work->pos6C.vz = env->camera->pointRots[2].vz;
            if (work->emit0 != NULL && work->emit0->state == BTLFX_EMIT_DONE) {
                work->emit0 = NULL;
            }
            ex->btlfx_moveEmitterTo((s32)work->emit0, (s32)&work->pos6C);
        }
        if (frame >= 0x69) {
            fxm_posPathGet((Fx0d2Path *)&fx_0d2_emitPath, frame - 0x69, &work->pos8C);
            work->pos84.vx = work->pos8C.vx;
            work->pos84.vy = work->pos8C.vy;
            work->pos84.vz = work->pos8C.vz;
            work->pos84.vx = -work->pos84.vx;
            if (work->emitC != NULL && work->emitC->state == BTLFX_EMIT_DONE) {
                work->emitC = NULL;
            }
            ex->btlfx_moveEmitterTo((s32)work->emitC, (s32)&work->pos84);
            if (work->emit10 != NULL && work->emit10->state == BTLFX_EMIT_DONE) {
                work->emit10 = NULL;
            }
            ex->btlfx_moveEmitterTo((s32)work->emit10, (s32)&work->pos8C);
        }
        env->camera->pointRots[1].vz = env->camera->points[2].vz;
        env->camera->projDist = env->camera->points[3].vx;
        fxm_camChanUpdate(0);
        if (frame >= 0x12C) {
            ex->btlfx_clearEffModels();
            return 1;
        }
    }
    return 0;
}

s32 fx_0d2_op81(s32 mode, Fx0d2Work81 *work, Unk800F78A4 *env, Unk80100C78Entry *entry, s32 *arg4) {
    BtlfxExports *ex = BTLFX_EXPORTS(env);
    SVECTOR v;
    s32 frame;
    s32 f;
    s32 i;
    s32 i5;
    s32 j;
    s32 i2;
    s32 n;
    s32 m;
    s32 k;
    u8 *buf;
    Pool6CEntry *p;

    if (mode == 0) {
        *arg4 = sizeof(Fx0d2Work81);
    } else if (mode == 1) {
        fx_0d2_env = env;
        work->rotC.vx = 0;
        work->rotC.vy = 0;
        work->rotC.vz = 0;
        work->pos14.vx = 0;
        work->pos14.vy = 0;
        work->pos14.vz = 0;
        work->scale24.vx = 0x1C00;
        work->scale24.vy = 0x1C00;
        work->scale24.vz = 0x1C00;
        for (k = 0; k < 6; k++) {
            fxm_rotPosPathInit((Fx0d2Path *)&fx_0d2_slicePaths[k],
                          (u16 *)fxm_offsetTableEntry((u8 *)ex->btlfx_getFileData(entry->file, 0x90), k));
            work->model44[k] = ex->btlfx_registerEffectModel((Unk800E8F50 *)ex->btlfx_getFileData(entry->file, fx_0d2_sliceBlocks[k]), 1);
        }
        fx_0d2_streak.lengthA = 6;
        fx_0d2_streak.start8 = 0;
        fx_0d2_streak.unkC = 0x1000;
        fx_0d2_streak.from10 = 0x40;
        fx_0d2_streak.unkE = 0x1000;
        fx_0d2_streak.to12 = 0x20;
        fx_0d2_streak2.from10 = 0x80;
        fx_0d2_streak2.start8 = 0;
        fx_0d2_streak2.lengthA = 0;
        fx_0d2_streak2.unkC = 0x1000;
        fx_0d2_streak2.unkE = 0x1000;
        fx_0d2_streak2.to12 = 0x60;
    } else {
        frame = entry->frame;
        switch (frame) {
        case 0:
            work->pos34.vx = 0;
            work->pos34.vy = 0;
            work->pos34.vz = -0x7D4;
            work->emit0 = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, 0xA6), (s32)&work->pos34);
            if (work->emit0 != NULL) {
                work->emit0->otZ = 0x100;
            }
            break;
        case 0x43:
            work->pos3C.vx = 0;
            work->pos3C.vy = 0;
            work->pos3C.vz = 0x435;
            work->emit4 = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, 1), (s32)&work->pos3C);
            work->emit8 = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, 4), (s32)&work->pos3C);
            if (work->emit4 != NULL) {
                work->emit4->otZ = 0x100;
            }
            if (work->emit8 != NULL) {
                work->emit8->otZ = 0x100;
            }
            break;
        case 0x41:
            ex->btlfx_drawBgOffscreen(0x2C0, 0x100, 0x80, 0x80, fx_0d2_bgBuf, 0, 0x200, 0x2BC);
            break;
        case 0x2C:
            for (i2 = 0; i2 < env->targets->targetCount; i2++) {
                ex->btlfx_getUnitPos(i2, (s16 *)&fx_0d2_unitPos[i2]);
                ex->btlfx_getUnitScale(i2, &fx_0d2_unitScale[i2]);
            }
            break;
        case 0x19:
            for (i2 = 0; i2 < 6; i2++) {
                ex->Hi_FreeEffModel(work->model44[i2]);
            }
            break;
        }
        if ((u32)frame < 0x19) {
            f = frame;
            for (i5 = 0; i5 < 6; i5++) {
                fxm_rotPosPathGet((Fx0d2KeyPath *)&fx_0d2_slicePaths[i5], f, &work->rotC, &v);
                work->pos14.vx = v.vx;
                work->pos14.vy = v.vy;
                work->pos14.vz = v.vz;
                ex->Hi_SetEffModelSlice(work->model44[i5], work->pos14.vy);
                ex->Hi_DrawSliceEffModel(&work->rotC, (VECTOR *)&work->pos14, (s32)&work->scale24, work->model44[i5]);
            }
        }
        if ((u32)(frame - 0x40) < 0x15) {
            fx_0d2_streak.lengthA = (frame - 0x40) * 4;
            fx_0d2_streak.lengthA = (fx_0d2_streak.lengthA > 0x10) ? 0x10 : fx_0d2_streak.lengthA;
            for (i = 0; i < 9; i++) {
                fxm_posPathInit((Fx0d2Path *)&fx_0d2_streak,
                              (u16 *)fxm_offsetTableEntry((u8 *)ex->btlfx_getFileData(entry->file, 0x80), i));
                m = fx_0d2_streak.lengthA;
                n = (fx_0d2_streak.keys.count2 >= m) ? m : fx_0d2_streak.keys.count2;
                /* Dead test, gone after jump2: the extra reference ranks m above n in global-alloc, so n lands in a3. */
                if (m != 7) {
                    i5 = 0;
                }
                fxm_buildPathTrail(&fx_0d2_streak.keys, fx_0d2_streakBufs[i], 0x90, fx_0d2_streak.start8, n, fx_0d2_streak.from10, fx_0d2_streak.to12, 1);
                ex->btlfx_drawTrail((u8 *)ex->btlfx_getFileData(entry->file, 0x1B), 0, NULL, 0, fx_0d2_streak.unkE,
                                    fx_0d2_streak.unkC, 1, fx_0d2_streakBufs[i], 0);
            }
        }
        if ((u32)(frame - 0x43) < 0xF) {
            fx_0d2_streak2.lengthA = (frame - 0x43) * 2;
            fx_0d2_streak2.lengthA = (fx_0d2_streak2.lengthA > 9) ? 9 : fx_0d2_streak2.lengthA;
            for (j = 0; j < 11; j++) {
                fxm_posPathInit((Fx0d2Path *)&fx_0d2_streak2,
                              (u16 *)fxm_offsetTableEntry((u8 *)ex->btlfx_getFileData(entry->file, 0x81), j));
                fxm_buildPathTrail(&fx_0d2_streak2.keys, fx_0d2_streakBufs2[j], 0x50, fx_0d2_streak2.start8, fx_0d2_streak2.lengthA, fx_0d2_streak2.from10,
                              fx_0d2_streak2.to12, 0);
                ex->btlfx_drawTrail((u8 *)ex->btlfx_getFileData(entry->file, 0x25), 0, NULL, 0, fx_0d2_streak2.unkE,
                                    fx_0d2_streak2.unkC, 1, fx_0d2_streakBufs2[j], 0);
            }
        }
        ex->btlfx_drawBackdrop((BtlBgLayers *)ex->btlfx_getFileData(entry->file, 5), frame);
        return frame >= 0x64;
    }
    return 0;
}

s32 fx_0d2_op82(s32 mode, Fx0d2Work82 *work, Unk800F78A4 *env, Unk80100C78Entry *entry, s32 *arg4) {
    BtlfxExports *ex = BTLFX_EXPORTS(env);
    SVECTOR v;
    SVECTOR w;
    s32 t;
    s32 frame;
    s32 i;
    s32 j;
    s32 k;
    s32 n;
    Fx0d2Spawn *sp;
    SVECTOR *rp;

    if (mode == 0) {
        *arg4 = sizeof(Fx0d2Work82);
    } else if (mode == 1) {
        work->xform1C[0].rot0.vx = 0;
        work->xform1C[0].rot0.vy = 0x800;
        work->xform1C[0].rot0.vz = 0;
        work->xform1C[0].pos8.vx = 0;
        work->xform1C[0].pos8.vy = 0;
        work->xform1C[0].pos8.vz = 0;
        work->xform1C[0].scale18.vx = 0x1C00;
        work->xform1C[0].scale18.vy = 0x1C00;
        work->xform1C[0].scale18.vz = 0x1C00;
        fx_0d2_env = env;
        fxm_rotPosPathInit(&fx_0d2_ringPath, (u8 *)ex->btlfx_getFileData(entry->file, 0x84));
        work->modelF4[0] = ex->btlfx_registerEffectModel((Unk800E8F50 *)ex->btlfx_getFileData(entry->file, 0x8E), 1);
        work->xform1C[1].rot0.vx = 0;
        work->xform1C[1].rot0.vy = 0;
        work->xform1C[1].rot0.vz = 0;
        work->xform1C[1].pos8.vx = 0;
        work->xform1C[1].pos8.vy = 0;
        work->xform1C[1].pos8.vz = 0;
        work->xform1C[1].scale18.vx = 0x1000;
        work->xform1C[1].scale18.vy = 0x1000;
        work->xform1C[1].scale18.vz = 0x1000;
        fxm_valuePathInit(&fx_0d2_morphCurve, (u8 *)ex->btlfx_getFileData(entry->file, 0x8F));
        work->modelF4[1] = ex->btlfx_registerEffectModel((Unk800E8F50 *)ex->btlfx_getFileData(entry->file, 0x86), 1);
        work->modelF4[2] = ex->btlfx_registerEffectModel((Unk800E8F50 *)ex->btlfx_getFileData(entry->file, 0x8D), 1);
        ex->Hi_SetEffModelOffset(work->modelF4[1], 0x32);
        work->xform1C[2].rot0.vx = 0;
        work->xform1C[2].rot0.vy = 0;
        work->xform1C[2].rot0.vz = 0;
        work->xform1C[2].pos8.vx = 0;
        work->xform1C[2].pos8.vy = 0;
        work->xform1C[2].pos8.vz = 0;
        work->xform1C[2].scale18.vx = 0x1C00;
        work->xform1C[2].scale18.vy = 0x1C00;
        work->xform1C[2].scale18.vz = 0x1C00;
        for (k = 0; k < 6; k++) {
            fxm_rotPosPathInit(&fx_0d2_slicePaths[k], (u8 *)fxm_offsetTableEntry((u8 *)ex->btlfx_getFileData(entry->file, 0x91), k));
            work->modelF4[k + 3] = ex->btlfx_registerEffectModel((Unk800E8F50 *)ex->btlfx_getFileData(entry->file, fx_0d2_sliceBlocks[k]), 1);
        }
        work->xform1C[3].rot0.vx = 0;
        work->xform1C[3].rot0.vy = 0;
        work->xform1C[3].rot0.vz = 0;
        work->xform1C[3].pos8.vx = 0;
        work->xform1C[3].pos8.vy = 0;
        work->xform1C[3].pos8.vz = 0;
        work->xform1C[3].scale18.vx = 0x1000;
        work->xform1C[3].scale18.vy = 0x1000;
        work->xform1C[3].scale18.vz = 0x1000;
        work->modelF4[9] = ex->btlfx_registerEffectModel((Unk800E8F50 *)ex->btlfx_getFileData(entry->file, 0x9E), 1);
    } else {
        frame = entry->frame;
        if (frame == 0x29) {
            ex->Hi_FreeEffModel(work->modelF4[0]);
            ex->Hi_FreeEffModel(work->modelF4[1]);
            ex->Hi_FreeEffModel(work->modelF4[2]);
            for (i = 0; i < 6; i++) {
                ex->Hi_FreeEffModel(work->modelF4[i + 3]);
            }
            ex->Hi_FreeEffModel(work->modelF4[9]);
        }
        for (sp = fx_0d2_spawns, i = 0; i < 7; i++, sp++) {
            if (sp->done4 == 0) {
                if (frame == sp->frame2) {
                    (&work->posBC[i])->vx = sp->pos6[0], (&work->posBC[i])->vy = sp->pos6[1],
                    (&work->posBC[i])->vz = sp->pos6[2];
                    work->emit0[i] = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, sp->block0), (s32)&work->posBC[i]);
                    if (i == 6 && work->emit0[6] != NULL) {
                        work->emit0[6]->otZ = 0x100;
                    }
                } else if (frame == 0x29) {
                    sp->done4 = 1;
                    ex->btlfx_stopEmitter(work->emit0[i]);
                }
                if (frame >= sp->frame2 && work->emit0[i] != NULL) {
                    if (work->emit0[i]->state == BTLFX_EMIT_DONE) {
                        work->emit0[i] = NULL;
                    }
                    if (work->emit0[i] != NULL) {
                        work->emit0[i]->rot.vx = fx_0d2_spawnRot.vx;
                        work->emit0[i]->rot.vy = fx_0d2_spawnRot.vy;
                        work->emit0[i]->rot.vz = fx_0d2_spawnRot.vz;
                    }
                }
            }
        }
        if (frame >= 1 && frame < 0x29) {
            rp = &work->xform1C[0].rot0;
            fxm_rotPosPathGet(&fx_0d2_ringPath, frame, rp, &v);
            work->xform1C[0].pos8.vx = v.vx;
            work->xform1C[0].pos8.vy = v.vy;
            work->xform1C[0].pos8.vz = v.vz;
            work->xform1C[0].rot0.vz = -work->xform1C[0].rot0.vz;
            ex->Hi_SetEffModelSlice(work->modelF4[0], work->xform1C[0].pos8.vy - 0x2D5);
            ex->Hi_DrawSliceEffModel(rp, (VECTOR *)&work->xform1C[0].pos8, (s32)&work->xform1C[0].scale18,
                                     work->modelF4[0]);
            fxm_valuePathGet(&fx_0d2_morphCurve, frame, &t);
            ex->Hi_DrawMorphEffModel(&work->xform1C[1].rot0, (VECTOR *)&work->xform1C[1].pos8, (s32)&work->xform1C[1].scale18,
                                     work->modelF4[1], work->modelF4[2], t);
            for (n = 0; n < 6; n++) {
                fxm_rotPosPathGet(&fx_0d2_slicePaths[n], frame, &work->xform1C[2].rot0, &w);
                work->xform1C[2].pos8.vx = w.vx;
                work->xform1C[2].pos8.vy = w.vy;
                work->xform1C[2].pos8.vz = w.vz;
                ex->Hi_SetEffModelSlice(work->modelF4[n + 3], work->xform1C[2].pos8.vy);
                ex->Hi_DrawSliceEffModel(&work->xform1C[2].rot0, (VECTOR *)&work->xform1C[2].pos8,
                                         (s32)&work->xform1C[2].scale18, work->modelF4[n + 3]);
            }
            if (frame >= 1 && frame < 0x29) {
                ex->Hi_DrawEffModel(&work->xform1C[3].rot0, (VECTOR *)&work->xform1C[3].pos8, (s32)&work->xform1C[3].scale18,
                                    work->modelF4[9]);
            }
        }
        return frame >= 0x2D;
    }
    return 0;
}

INCLUDE_RODATA("asm/us/fx_0d2/nonmatchings/fx_0d2", fx_0d2_headerPtr);

s32 fx_0d2_op83(s32 mode, Fx0d2Work83 *work, Unk800F78A4 *env, Unk80100C78Entry *entry, s32 *arg4) {
    BtlfxExports *ex = BTLFX_EXPORTS(env);
    VECTOR sum;
    SVECTOR a;
    SVECTOR b;
    SVECTOR c;
    SVECTOR d;
    VECTOR e;
    s32 t;
    s32 frame;
    s32 i;
    s32 j;
    s32 k;
    s32 q;
    s32 k2;
    s32 f;
    s32 m0;
    s32 m1;
    u16 model;

    if (mode == 0) {
        *arg4 = sizeof(Fx0d2Work83);
    } else if (mode == 1) {
        work->rot8.vx = 0;
        work->rot8.vy = 0x800;
        work->rot8.vz = 0;
        work->pos10.vx = 0;
        work->pos10.vy = 0;
        work->pos10.vz = 0;
        work->scale20.vx = 0x1000;
        work->scale20.vy = 0x1000;
        work->scale20.vz = 0x1000;
        fx_0d2_env = env;
        fxm_rotPosPathInit((Fx0d2Path *)&fx_0d2_slicePath2, (u16 *)ex->btlfx_getFileData(entry->file, 0x28));
        work->model136[0] = ex->btlfx_registerEffectModel(&fx_0d2_sliceModel, 1);
        work->rot8.vx = 0;
        work->rot8.vy = 0x800;
        work->rot8.vz = 0;
        work->pos10.vx = 0;
        work->pos10.vy = 0;
        work->pos10.vz = 0;
        work->scale20.vx = 0x1000;
        work->scale20.vy = 0x1000;
        work->scale20.vz = 0x1000;
        fxm_rotPosPathInit((Fx0d2Path *)&fx_0d2_morphPath, (u16 *)ex->btlfx_getFileData(entry->file, 9));
        for (i = 0; i < 3; i++) {
            model = ex->Hi_RegisterTexPtrModel((BtlUnit9C8 *)ex->btlfx_getFileData(entry->file, fx_0d2_morphBlocks[i]),
                                               fx_0d2_texPages, fx_0d2_uvOffsets);
            work->model136[i + 1] = model;
        }
        work->rot30.vx = 0;
        work->rot30.vy = 0;
        work->rot30.vz = 0;
        work->pos38.vx = 0;
        work->pos38.vy = 0;
        work->pos38.vz = 0;
        work->scale48.vx = 0x1000;
        work->scale48.vy = 0x1000;
        work->scale48.vz = 0x1000;
        for (i = 0; i < 5; i++) {
            work->model136[i + 4] = ex->btlfx_registerEffectModel((Unk800E8F50 *)fxm_offsetTableEntry(fx_0d2_pieceModels, i), 1);
        }
        for (i = 0; i < 10; i++) {
            fxm_rotPosPathInit((Fx0d2Path *)&fx_0d2_piecePaths[i],
                          (u16 *)fxm_offsetTableEntry((u8 *)ex->btlfx_getFileData(entry->file, 8), i));
        }
        fxm_posPathInit((Fx0d2Path *)&fx_0d2_unitOffsetPath, (u16 *)ex->btlfx_getFileData(entry->file, 0x14));
        fxm_vecPathInit((Fx0d2Path *)&fx_0d2_unitScalePath, (u16 *)ex->btlfx_getFileData(entry->file, 0x15));
        work->fadeOut130 = -1;
        work->fadeIn132 = -1;
        work->white134 = 0;
    } else {
        frame = entry->frame;
        switch (frame) {
        case 3:
            work->pos118.vx = -0x1C;
            work->pos118.vy = -0x267;
            work->pos118.vz = 0x342;
            work->rot128.vx = 0x1CF;
            work->rot128.vy = -0x7B7;
            work->rot128.vz = 0x27B;
            work->emit0 = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, 0x21), (s32)&work->pos118);
            if (work->emit0 != NULL) {
                work->emit0->otZ = 0x118;
            }
            break;
        case 12:
            sum.vx = 0;
            sum.vy = 0;
            sum.vz = 0;
            for (q = 0; q < env->targets->targetCount; q++) {
                sum.vx += work->unitPos58[q].vx;
                sum.vy += (&work->unitPos58[q])->vy;
                sum.vz += (&work->unitPos58[q])->vz;
            }
            work->pos120.vx = sum.vx / env->targets->targetCount;
            work->pos120.vy = sum.vy / env->targets->targetCount;
            work->pos120.vz = sum.vz / env->targets->targetCount;
            work->emit4 = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, 3), (s32)&work->pos120);
            break;
        case 38:
            ex->btlfx_stopEmitter(work->emit0);
            break;
        case 22:
            work->fadeIn132 = 0x10;
            break;
        case 42:
            work->fadeOut130 = 0x10;
            work->white134 = 0;
            for (q = 0; q < env->targets->targetCount; q++) {
                ex->btlfx_setUnitScale(q, (s32 *)&fx_0d2_unitScale[q]);
            }
            env->camera->points[0].vx = 0;
            env->camera->points[0].vy = 0;
            env->camera->points[0].vz = 0x4B0;
            break;
        }
        k = frame - 1;
        if ((u32)k < 0x17) {
            fxm_rotPosPathGet((Fx0d2KeyPath *)&fx_0d2_slicePath2, frame, &work->rot8, &a);
            work->pos10.vx = a.vx;
            work->pos10.vy = a.vy;
            work->pos10.vz = a.vz;
            work->rot8.vz = -work->rot8.vz;
            ex->Hi_SetEffModelSlice(work->model136[0], work->pos10.vy);
            ex->Hi_DrawSliceEffModel(&work->rot8, (VECTOR *)&work->pos10, (s32)&work->scale20, work->model136[0]);
        }
        if ((u32)k < 0x27) {
            f = frame;
            switch (frame) {
            case 1:
                fxm_valuePathInit((Fx0d2Path *)&fx_0d2_morphCurve2, (u16 *)ex->btlfx_getFileData(entry->file, 0x19));
                break;
            case 4:
                fxm_valuePathInit((Fx0d2Path *)&fx_0d2_morphCurve2, (u16 *)ex->btlfx_getFileData(entry->file, 0x1A));
                break;
            }
            if (f < 4) {
                m0 = 1;
                m1 = 2;
            } else {
                m0 = 2;
                m1 = 3;
            }
            fxm_rotPosPathGet((Fx0d2KeyPath *)&fx_0d2_morphPath, f, &work->rot8, &b);
            work->pos10.vx = b.vx;
            work->pos10.vy = b.vy;
            work->pos10.vz = b.vz;
            work->rot8.vz = -work->rot8.vz;
            fxm_valuePathGet(&fx_0d2_morphCurve2, f + 1, &t);
            ex->Hi_DrawMorphEffModel(&work->rot8, (VECTOR *)&work->pos10, (s32)&work->scale20, work->model136[m0],
                                     work->model136[m1], t);
        }
        if ((u32)(frame - 2) < 0x18) {
            k2 = frame - 2;
            j = 4;
            for (i = 0; i < 10; i++) {
                fxm_rotPosPathGet((Fx0d2KeyPath *)&fx_0d2_piecePaths[i], k2, &work->rot30, &c);
                work->pos38.vx = c.vx;
                work->pos38.vy = c.vy;
                work->pos38.vz = c.vz;
                ex->Hi_DrawEffModel(&work->rot30, (VECTOR *)&work->pos38, (s32)&work->scale48, work->model136[j]);
                j++;
                if (j >= 9) {
                    j = 4;
                }
            }
        }
        if ((u32)frame < 0x1C) {
            fxm_posPathGet((Fx0d2Path *)&fx_0d2_unitOffsetPath, frame, &d);
            fxm_vecPathGet((Fx0d2Path *)&fx_0d2_unitScalePath, frame, &e);
            for (q = 0; q < env->targets->targetCount; q++) {
                work->unitPos58[q].vx = fx_0d2_unitPos[q].vx + d.vx;
                work->unitPos58[q].vy = fx_0d2_unitPos[q].vy + d.vy;
                work->unitPos58[q].vz = fx_0d2_unitPos[q].vz + d.vz;
                ex->btlfx_setUnitPos(q, (s16 *)&work->unitPos58[q]);
                work->unitScale98[q].vx = (fx_0d2_unitScale[q].vx * e.vx) / 4096;
                work->unitScale98[q].vy = (fx_0d2_unitScale[q].vy * e.vy) / 4096;
                work->unitScale98[q].vz = (fx_0d2_unitScale[q].vz * e.vz) / 4096;
                ex->btlfx_setUnitScale(q, (s32 *)&work->unitScale98[q]);
            }
        }
        if ((u32)(frame - 3) < 0x23 && work->emit0 != NULL) {
            if (work->emit0->state == BTLFX_EMIT_DONE) {
                work->emit0 = NULL;
            }
            if (work->emit0 != NULL) {
                work->emit0->rot.vx = work->rot128.vx;
                work->emit0->rot.vy = work->rot128.vy;
                work->emit0->rot.vz = work->rot128.vz;
            }
        }
        if (work->fadeIn132 >= 0) {
            fxm_tintScreenLerp(0, 0xFF, 0x10 - work->fadeIn132, 0x10, 1, 0);
            if (--work->fadeIn132 == -1) {
                work->white134 = 1;
            }
        }
        if (work->fadeOut130 >= 0) {
            fxm_tintScreenLerp(0xFF, 0, 0x10 - work->fadeOut130, 0x10, 1, 0);
            work->fadeOut130--;
        }
        if (work->white134 != 0) {
            ex->btlfx_drawScreenTint((u32 *)env->ot, 1, 0xFF, 0xFF, 0xFF);
        }
        return frame >= 0x3D;
    }
    return 0;
}

s32 fx_0d2_op84(s32 mode, Fx0d2Work84 *work, Unk800F78A4 *env, Unk80100C78Entry *entry, s32 *arg4) {
    BtlfxExports *ex = BTLFX_EXPORTS(env);
    s32 frame;
    s32 i;
    s32 j;
    Pool6CEntry *e;

    if (mode == 0) {
        *arg4 = sizeof(Fx0d2Work84);
    } else if (mode == 1) {
        fx_0d2_env = env;
        for (j = 0; j < env->targets->targetCount; j++) {
            work->doneA0[j] = 0;
            ex->btlfx_getUnitHomePos(j, (s16 *)&work->pos20[j]);
            work->pos20[j].vy = -3000;
            ex->btlfx_setUnitPos(j, (s16 *)&work->pos20[j]);
        }
    } else {
        frame = entry->frame;
        for (i = 0; i < env->targets->targetCount; i++) {
            if (work->doneA0[i] == 0 && fxm_moveUnitHome(i, frame, 4, &work->pos20[i]) != 0) {
                work->doneA0[i] = 1;
                ex->btlfx_getUnitHomePos(i, (s16 *)&work->pos20[i]);
                ex->btlfx_setUnitPos(i, (s16 *)&work->pos20[i]);
                /* PsyQ copyVector() */
                (&work->emitPos60[i])->vx = (&work->pos20[i])->vx, (&work->emitPos60[i])->vy = (&work->pos20[i])->vy,
                (&work->emitPos60[i])->vz = (&work->pos20[i])->vz;
                e = ex->btlfx_startEmitter((s32)ex->btlfx_getFileData(entry->file, 2), (s32)&work->emitPos60[i]);
                work->emit0[i] = e;
                if (e != NULL) {
                    e->otZ = 0x80;
                }
            }
        }
        return frame >= 0x28;
    }
    return 0;
}

s32 fx_0d2_op85(s32 mode, s16 *work, Unk800F78A4 *env, Unk80100C78Entry *entry, s32 *arg4) {
    s32 frame;

    if (mode == 0) {
        *arg4 = 2;
    } else if (mode == 1) {
        fx_0d2_env = env;
        *work = -1;
    } else {
        frame = entry->frame;
        if (frame == 0) {
            *work = 0x10;
        }
        if (*work >= 0) {
            fxm_tintScreenLerp(0xFF, 0, 0x10 - *work, 0x10, 2, 0);
            (*work)--;
        }
        return frame >= 0x10;
    }
    return 0;
}

static s32 fxm_offsetTableEntry(u8 *base, u16 i) {
    return (s32)base + ((s32 *)base)[(u16)i + 1];
}

static void fxm_lerpSvec(SVECTOR *a, SVECTOR *b, s32 t, SVECTOR *out) {
    BTLFX_EXPORTS(fx_0d2_env)->LoadAverageShort12(a, b, 0x1000 - t, t, out);
}

static s32 fxm_lerpRatio(s32 from, s32 to, s32 t, s32 n) {
    return (from * (n - t) + to * t) / n;
}

static void fxm_posPathInit(Fx0d2Path *path, u16 *data) {
    data++;
    path->count2 = *data++;
    path->points4 = (Fx0d2Point *)data;
}

static s32 fxm_posPathGet(Fx0d2Path *path, s32 t, SVECTOR *out) {
    s32 last = path->count2 - 1;
    s32 done = 1;
    Fx0d2Point *p;

    if (last < t) {
        t = last;
    } else {
        done = 0;
    }
    p = &path->points4[t];
    out->vx = p->x;
    out->vy = p->y;
    out->vz = p->z;
    return done;
}

static void fxm_rotPosPathInit(Fx0d2Path *path, u16 *data) {
    data++;
    path->count2 = *data++;
    path->points4 = (Fx0d2Point *)data;
}

static s32 fxm_rotPosPathGet(Fx0d2KeyPath *path, s32 i, SVECTOR *rot, SVECTOR *pos) {
    Fx0d2Key *k;
    s32 r = 1;

    if (i > path->count2 - 1) {
        i = path->count2 - 1;
    } else {
        r = 0;
    }
    k = &path->keys4[i];
    if (rot != NULL) {
        rot->vx = k->rot6[0];
        rot->vy = k->rot6[1];
        rot->vz = k->rot6[2];
    }
    if (pos != NULL) {
        pos->vx = k->pos0[0];
        pos->vy = k->pos0[1];
        pos->vz = k->pos0[2];
    }
    return r;
}

static void fxm_valuePathInit(Fx0d2Path *path, u16 *data) {
    data++;
    path->count2 = *data++;
    path->points4 = (Fx0d2Point *)data;
}

static s32 fxm_valuePathGet(Fx0d2Keys *keys, s32 i, s32 *out) {
    s32 r = 1;

    if (i > keys->count2 - 1) {
        i = keys->count2 - 1;
    } else {
        r = 0;
    }
    *out = *(s16 *)((u8 *)keys->keys4 + (i << 1));
    return r;
}

static void fxm_vecPathInit(Fx0d2Path *path, u16 *data) {
    data++;
    path->count2 = *data++;
    path->points4 = (Fx0d2Point *)data;
}

static s32 fxm_vecPathGet(Fx0d2Path *track, s32 t, VECTOR *out) {
    s32 last = track->count2 - 1;
    s32 done = 1;
    Fx0d2Point *k;

    if (last < t) {
        t = last;
    } else {
        done = 0;
    }
    k = &track->points4[t];
    out->vx = k->x;
    out->vy = k->y;
    out->vz = k->z;
    return done;
}

static void fxm_tintScreenLerp(s32 from, s32 to, s32 t, s32 n, s32 arg4, s32 ot) {
    s32 c = (from * (n - t) + to * t) / n;

    BTLFX_EXPORTS(fx_0d2_env)->btlfx_drawScreenTint((u32 *)(fx_0d2_env->ot + ot), arg4, c, c, c);
}

static s32 fxm_moveUnitHome(s32 a0, s32 t, s32 n, SVECTOR *b) {
    BtlfxExports *ex = BTLFX_EXPORTS(fx_0d2_env);
    SVECTOR a;
    SVECTOR out;

    ex->btlfx_getUnitHomePos(a0, (s16 *)&a);
    fxm_lerpSvec(b, &a, (t << 12) / n, &out);
    ex->btlfx_setUnitPos(a0, (s16 *)&out);
    return t >= n;
}

static void fxm_camChanReset(void) {
    s32 i;
    s32 m = -1;

    for (i = 63; i >= 0; i--) {
        fx_0d2_camChans[i].left6 = m;
    }
    for (i = 3; i >= 0; i--) {
        fx_0d2_camChanFrame[i] = 0;
    }
}

static void fxm_camChanAdd(s16 start, u16 dst, s16 ch, s16 *data) {
    s32 i;
    s32 c;

    data++;
    c = *data;
    for (i = 0; i < 64; i++) {
        if (fx_0d2_camChans[i].left6 == -1) {
            fx_0d2_camChans[i].left6 = 0;
            fx_0d2_camChans[i].data0 = data;
            fx_0d2_camChans[i].start4 = start;
            fx_0d2_camChans[i].dstA = dst;
            fx_0d2_camChans[i].keyC = 0;
            fx_0d2_camChans[i].keyCountE = c;
            fx_0d2_camChans[i].chan8 = ch;
            return;
        }
    }
}

static void fxm_camChanUpdate(s32 ch) {
    Unk800F78A4 *env = fx_0d2_env;
    s32 i;
    s32 k;
    s32 d;

    for (i = 0; i < 64; i++) {
        if (fx_0d2_camChans[i].chan8 == ch) {
            if (fx_0d2_camChans[i].left6 == 0 && fx_0d2_camChans[i].start4 == fx_0d2_camChanFrame[ch]) {
                fx_0d2_camChans[i].left6 = *fx_0d2_camChans[i].data0;
                fx_0d2_camChans[i].data0++;
            }
            if (fx_0d2_camChans[i].left6 > 0) {
                fx_0d2_camChans[i].left6--;
                for (k = 0; k < 3; k++) {
                    d = ((fx_0d2_camChans[i].dstA >> (k * 4)) & 0xF) - 1;
                    if (d >= 0) {
                        ((SVECTOR *)env->camera)[d].vx = fx_0d2_camChans[i].data0[0];
                        ((SVECTOR *)env->camera)[d].vy = fx_0d2_camChans[i].data0[1];
                        ((SVECTOR *)env->camera)[d].vz = fx_0d2_camChans[i].data0[2];
                        if (fx_0d2_camChans[i].keyC < fx_0d2_camChans[i].keyCountE) {
                            fx_0d2_camChans[i].data0 += 3;
                            fx_0d2_camChans[i].keyC++;
                        }
                    }
                }
            }
        }
    }
    fx_0d2_camChanFrame[ch]++;
}
