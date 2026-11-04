#include "fxmod/fx_0d2.h"

static void fxm_buildPathTrail(Fx0d2Keys *keys, u8 *buf, s32 size, s32 start, s32 n, s32 from, s32 to, s32 clip) {
    SVECTOR pos;
    SVECTOR *pt;
    s32 count;
    s32 i;
    s32 keyIdx;
    s32 next;

    count = (size - 4) / 8;
    pt = (SVECTOR *)(buf + 4);
    buf[0] = buf[2] = count - 1;
    buf[1] = 0;
    if (n < count) {
        for (i = 0; i < count - n; i++) {
            pt->pad = 0;
            pt++;
        }
    } else {
        n = count;
    }
    for (i = 0; i < n; i++) {
        next = i + 1;
        keyIdx = n - next + start;
        if (clip != 0 && keyIdx >= keys->count2) {
            break;
        }
        fxm_posPathGet(keys, keyIdx, &pos);
        pt->vx = pos.vx;
        pt->vy = pos.vy;
        pt->vz = pos.vz;
        pt->pad = from + (to - from) / keys->count2 * (n - i);
        pt++;
    }
}

s32 fx_0d2_op80(s32 mode, Fx0d2Work80 *work, Unk800F78A4 *env, Unk80100C78Entry *entry, s32 *args) {
    BtlfxExports *ex = BTLFX_EXPORTS(env);
    SVECTOR pathPos;
    SVECTOR slicePos;
    s32 frame;
    s32 elapsed;

    if (mode == 0) {
        *args = sizeof(Fx0d2Work80);
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
        work->modelA4 = ex->btlfx_registerEffectModel((Unk800E8F50 *)ex->btlfx_getFileData(entry->file, FX_FENRIR_SUMMON_MODEL_5), 1);
        fxm_camChanReset();
        fxm_camChanAdd(0, 1, 0, (s16 *)ex->btlfx_getFileData(entry->file, FX_FENRIR_SUMMON_CAMERA_KEYS_26));
        fxm_camChanAdd(0, 2, 0, (s16 *)ex->btlfx_getFileData(entry->file, FX_FENRIR_SUMMON_CAMERA_KEYS_22));
        fxm_camChanAdd(0, 3, 0, (s16 *)ex->btlfx_getFileData(entry->file, FX_FENRIR_SUMMON_CAMERA_KEYS_23));
        fxm_camChanAdd(0, 4, 0, (s16 *)ex->btlfx_getFileData(entry->file, FX_FENRIR_SUMMON_CAMERA_KEYS_20));
        fxm_camChanAdd(0, 9, 0, (s16 *)ex->btlfx_getFileData(entry->file, FX_FENRIR_SUMMON_CAMERA_KEYS_29));
        fxm_camChanAdd(0, 0xA, 0, (s16 *)ex->btlfx_getFileData(entry->file, FX_FENRIR_SUMMON_CAMERA_KEYS_19));
        fxm_camChanAdd(0x68, 1, 0, (s16 *)ex->btlfx_getFileData(entry->file, FX_FENRIR_SUMMON_CAMERA_KEYS_25));
        fxm_camChanAdd(0x68, 2, 0, (s16 *)ex->btlfx_getFileData(entry->file, FX_FENRIR_SUMMON_CAMERA_KEYS_21));
        fxm_camChanAdd(0x67, 3, 0, (s16 *)ex->btlfx_getFileData(entry->file, FX_FENRIR_SUMMON_CAMERA_KEYS_28));
        fxm_camChanAdd(0x67, 4, 0, (s16 *)ex->btlfx_getFileData(entry->file, FX_FENRIR_SUMMON_CAMERA_KEYS_27));
        fxm_camChanAdd(0x68, 0xA, 0, (s16 *)ex->btlfx_getFileData(entry->file, FX_FENRIR_SUMMON_CAMERA_KEYS_24));
        fxm_camChanAdd(0xB6, 1, 0, (s16 *)ex->btlfx_getFileData(entry->file, FX_FENRIR_SUMMON_CAMERA_KEYS_32));
        fxm_camChanAdd(0xB6, 2, 0, (s16 *)ex->btlfx_getFileData(entry->file, FX_FENRIR_SUMMON_CAMERA_KEYS_33));
        fxm_camChanAdd(0xB5, 3, 0, (s16 *)ex->btlfx_getFileData(entry->file, FX_FENRIR_SUMMON_CAMERA_KEYS_34));
        fxm_camChanAdd(0xB5, 4, 0, (s16 *)ex->btlfx_getFileData(entry->file, FX_FENRIR_SUMMON_CAMERA_KEYS_35));
        fxm_camChanAdd(0xDE, 1, 0, (s16 *)ex->btlfx_getFileData(entry->file, FX_FENRIR_CAMERA_KEYS_1));
        fxm_camChanAdd(0xDE, 2, 0, (s16 *)ex->btlfx_getFileData(entry->file, FX_FENRIR_CAMERA_KEYS_2));
        fxm_camChanAdd(0xDD, 3, 0, (s16 *)ex->btlfx_getFileData(entry->file, FX_FENRIR_CAMERA_KEYS_3));
        fxm_camChanAdd(0xDD, 4, 0, (s16 *)ex->btlfx_getFileData(entry->file, FX_FENRIR_CAMERA_KEYS_4));
    } else {
        frame = entry->frame;
        switch (frame) {
        case 1:
            work->motion94 = 0;
            work->loop98 = frame;
            ex->Hi_SetSummonMotion(((Fx0d2Summon *)env->loader)->summon54->motion180, ((Fx0d2Summon *)env->loader)->model60);
            work->pathFrame9C = frame;
            fxm_posPathInit((Fx0d2Path *)&fx_0d2_summonPath, (u16 *)ex->btlfx_getFileData(entry->file, FX_FENRIR_SUMMON_PATH_18));
            break;
        case 0x29:
            work->motion94 = 1;
            work->loop98 = 0;
            ex->Hi_SetSummonMotion(((Fx0d2Summon *)env->loader)->summon54->motion184, ((Fx0d2Summon *)env->loader)->model60);
            break;
        case 0x51:
            work->motion94 = 2;
            work->loop98 = 1;
            ex->Hi_SetSummonMotion(((Fx0d2Summon *)env->loader)->summon54->motion188, ((Fx0d2Summon *)env->loader)->model60);
            break;
        case 0x69:
            work->summonScale2C.vx = 0x433;
            work->summonScale2C.vy = 0x433;
            work->summonScale2C.vz = 0x433;
            work->scale54.vx = 0x1C00;
            work->scale54.vy = 0x1C00;
            work->scale54.vz = 0x1C00;
            work->pathFrame9C = 0;
            fxm_posPathInit((Fx0d2Path *)&fx_0d2_summonPath, (u16 *)ex->btlfx_getFileData(entry->file, FX_FENRIR_SUMMON_PATH_31));
            break;
        case 0xB6:
            fxm_rotPosPathInit((Fx0d2Path *)&fx_0d2_slicePath, (u16 *)ex->btlfx_getFileData(entry->file, FX_FENRIR_SUMMON_SLICE_PATH_39));
            break;
        }
        switch (frame) {
        case 1:
            work->emit0 = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, FX_FENRIR_SUMMON_EMITTER_36), (s32)&fx_0d2_emitPos);
            break;
        case 0x52:
            work->pos74.vx = 0x88;
            work->pos74.vy = -0x366;
            work->pos74.vz = -0x41F;
            work->pos7C.vx = -0x88;
            work->pos7C.vy = -0x366;
            work->pos7C.vz = -0x41F;
            work->emit4 = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, FX_FENRIR_SUMMON_EMITTER_37), (s32)&work->pos74);
            work->emit8 = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, FX_FENRIR_SUMMON_EMITTER_37), (s32)&work->pos7C);
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
            fxm_posPathInit((Fx0d2Path *)&fx_0d2_emitPath, (u16 *)ex->btlfx_getFileData(entry->file, FX_FENRIR_SUMMON_EMIT_PATH_2));
            work->emitC = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, FX_FENRIR_SUMMON_EMITTER_3), (s32)&fx_0d2_emitPos2[0]);
            work->emit10 = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, FX_FENRIR_SUMMON_EMITTER_3), (s32)&fx_0d2_emitPos2[1]);
            if (work->emitC != NULL) {
                work->emitC->otZ = 0x100;
            }
            if (work->emit10 != NULL) {
                work->emit10->otZ = 0x100;
            }
            break;
        }
        elapsed = frame - 1;
        if ((u32)elapsed < 0x83) {
            fxm_posPathGet((Fx0d2Path *)&fx_0d2_summonPath, work->pathFrame9C, &pathPos);
            work->summonPos1C.vx = pathPos.vx;
            work->summonPos1C.vy = pathPos.vy;
            work->summonPos1C.vz = pathPos.vz;
            ex->Hi_DrawSummonModel(&work->rot14, (VECTOR *)&work->summonPos1C, (s32)&work->summonScale2C,
                                   ((Fx0d2Summon *)env->loader)->model60, work->loop98);
            work->pathFrame9C++;
        }
        if ((u32)elapsed < 0xDE) {
            if (frame < 0xB6) {
                work->pos44.vx = env->camera->pointRots[3].vx;
                work->pos44.vy = env->camera->pointRots[3].vy;
                work->pos44.vz = env->camera->pointRots[3].vz;
            } else {
                fxm_rotPosPathGet((Fx0d2KeyPath *)&fx_0d2_slicePath, frame - 0xB6, &work->rot3C, &slicePos);
                work->pos44.vx = slicePos.vx;
                work->pos44.vy = slicePos.vy;
                work->pos44.vz = slicePos.vz;
            }
            ex->Hi_SetEffModelSlice(work->modelA4, work->pos44.vy);
            ex->Hi_DrawSliceEffModel(&work->rot3C, (VECTOR *)&work->pos44, (s32)&work->scale54, work->modelA4);
            if (elapsed < 0x69) {
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

s32 fx_0d2_op81(s32 mode, Fx0d2Work81 *work, Unk800F78A4 *env, Unk80100C78Entry *entry, s32 *args) {
    BtlfxExports *ex = BTLFX_EXPORTS(env);
    SVECTOR slicePos;
    s32 frame;
    s32 pathFrame;
    s32 i;
    s32 slice;
    s32 j;
    s32 idx;
    s32 trailLen;
    s32 maxLen;
    s32 k;
    u8 *buf;
    Pool6CEntry *emitter;

    if (mode == 0) {
        *args = sizeof(Fx0d2Work81);
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
                          (u16 *)fxm_offsetTableEntry((u8 *)ex->btlfx_getFileData(entry->file, FX_FENRIR_SUMMON_SLICE_PATHS_16), k));
            work->model44[k] = ex->btlfx_registerEffectModel((Unk800E8F50 *)ex->btlfx_getFileData(entry->file, fx_0d2_sliceBlocks[k]), 1);
        }
        fx_0d2_streak.lengthA = 6;
        fx_0d2_streak.start8 = 0;
        fx_0d2_streak.fadeC = 0x1000;
        fx_0d2_streak.from10 = 0x40;
        fx_0d2_streak.scaleE = 0x1000;
        fx_0d2_streak.to12 = 0x20;
        fx_0d2_streak2.from10 = 0x80;
        fx_0d2_streak2.start8 = 0;
        fx_0d2_streak2.lengthA = 0;
        fx_0d2_streak2.fadeC = 0x1000;
        fx_0d2_streak2.scaleE = 0x1000;
        fx_0d2_streak2.to12 = 0x60;
    } else {
        frame = entry->frame;
        switch (frame) {
        case 0:
            work->pos34.vx = 0;
            work->pos34.vy = 0;
            work->pos34.vz = -0x7D4;
            work->emit0 = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, FX_FENRIR_SUMMON_EMITTER_38), (s32)&work->pos34);
            if (work->emit0 != NULL) {
                work->emit0->otZ = 0x100;
            }
            break;
        case 0x43:
            work->pos3C.vx = 0;
            work->pos3C.vy = 0;
            work->pos3C.vz = 0x435;
            work->emit4 = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, FX_FENRIR_FAN_EMITTER), (s32)&work->pos3C);
            work->emit8 = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, FX_FENRIR_ORANGE_RING), (s32)&work->pos3C);
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
            for (idx = 0; idx < env->targets->targetCount; idx++) {
                ex->btlfx_getUnitPos(idx, (s16 *)&fx_0d2_unitPos[idx]);
                ex->btlfx_getUnitScale(idx, &fx_0d2_unitScale[idx]);
            }
            break;
        case 0x19:
            for (idx = 0; idx < 6; idx++) {
                ex->Hi_FreeEffModel(work->model44[idx]);
            }
            break;
        }
        if ((u32)frame < 0x19) {
            pathFrame = frame;
            for (slice = 0; slice < 6; slice++) {
                fxm_rotPosPathGet((Fx0d2KeyPath *)&fx_0d2_slicePaths[slice], pathFrame, &work->rotC, &slicePos);
                work->pos14.vx = slicePos.vx;
                work->pos14.vy = slicePos.vy;
                work->pos14.vz = slicePos.vz;
                ex->Hi_SetEffModelSlice(work->model44[slice], work->pos14.vy);
                ex->Hi_DrawSliceEffModel(&work->rotC, (VECTOR *)&work->pos14, (s32)&work->scale24, work->model44[slice]);
            }
        }
        if ((u32)(frame - 0x40) < 0x15) {
            fx_0d2_streak.lengthA = (frame - 0x40) * 4;
            fx_0d2_streak.lengthA = (fx_0d2_streak.lengthA > 0x10) ? 0x10 : fx_0d2_streak.lengthA;
            for (i = 0; i < 9; i++) {
                fxm_posPathInit((Fx0d2Path *)&fx_0d2_streak,
                              (u16 *)fxm_offsetTableEntry((u8 *)ex->btlfx_getFileData(entry->file, FX_FENRIR_SUMMON_STREAK_PATHS_0), i));
                maxLen = fx_0d2_streak.lengthA;
                trailLen = (fx_0d2_streak.keys.count2 >= maxLen) ? maxLen : fx_0d2_streak.keys.count2;
                /* Dead test, gone after jump2: the extra reference ranks maxLen above trailLen in global-alloc, so trailLen lands in a3. */
                if (maxLen != 7) {
                    slice = 0;
                }
                fxm_buildPathTrail(&fx_0d2_streak.keys, fx_0d2_streakBufs[i], 0x90, fx_0d2_streak.start8, trailLen, fx_0d2_streak.from10, fx_0d2_streak.to12, 1);
                ex->btlfx_drawTrail((u8 *)ex->btlfx_getFileData(entry->file, FX_FENRIR_ROCK_STREAK), 0, NULL, 0, fx_0d2_streak.scaleE,
                                    fx_0d2_streak.fadeC, 1, fx_0d2_streakBufs[i], 0);
            }
        }
        if ((u32)(frame - 0x43) < 0xF) {
            fx_0d2_streak2.lengthA = (frame - 0x43) * 2;
            fx_0d2_streak2.lengthA = (fx_0d2_streak2.lengthA > 9) ? 9 : fx_0d2_streak2.lengthA;
            for (j = 0; j < 11; j++) {
                fxm_posPathInit((Fx0d2Path *)&fx_0d2_streak2,
                              (u16 *)fxm_offsetTableEntry((u8 *)ex->btlfx_getFileData(entry->file, FX_FENRIR_SUMMON_STREAK_PATHS_1), j));
                fxm_buildPathTrail(&fx_0d2_streak2.keys, fx_0d2_streakBufs2[j], 0x50, fx_0d2_streak2.start8, fx_0d2_streak2.lengthA, fx_0d2_streak2.from10,
                              fx_0d2_streak2.to12, 0);
                ex->btlfx_drawTrail((u8 *)ex->btlfx_getFileData(entry->file, FX_FENRIR_ROCK_STREAK_2), 0, NULL, 0, fx_0d2_streak2.scaleE,
                                    fx_0d2_streak2.fadeC, 1, fx_0d2_streakBufs2[j], 0);
            }
        }
        ex->btlfx_drawBackdrop((BtlBgLayers *)ex->btlfx_getFileData(entry->file, FX_FENRIR_BACKDROP), frame);
        return frame >= 0x64;
    }
    return 0;
}

s32 fx_0d2_op82(s32 mode, Fx0d2Work82 *work, Unk800F78A4 *env, Unk80100C78Entry *entry, s32 *args) {
    BtlfxExports *ex = BTLFX_EXPORTS(env);
    SVECTOR ringPos;
    SVECTOR slicePos;
    s32 morphT;
    s32 frame;
    s32 i;
    s32 j;
    s32 k;
    s32 slice;
    Fx0d2Spawn *sp;
    SVECTOR *rp;

    if (mode == 0) {
        *args = sizeof(Fx0d2Work82);
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
        fxm_rotPosPathInit(&fx_0d2_ringPath, (u8 *)ex->btlfx_getFileData(entry->file, FX_FENRIR_SUMMON_RING_PATH_4));
        work->modelF4[0] = ex->btlfx_registerEffectModel((Unk800E8F50 *)ex->btlfx_getFileData(entry->file, FX_FENRIR_SUMMON_MODEL_14), 1);
        work->xform1C[1].rot0.vx = 0;
        work->xform1C[1].rot0.vy = 0;
        work->xform1C[1].rot0.vz = 0;
        work->xform1C[1].pos8.vx = 0;
        work->xform1C[1].pos8.vy = 0;
        work->xform1C[1].pos8.vz = 0;
        work->xform1C[1].scale18.vx = 0x1000;
        work->xform1C[1].scale18.vy = 0x1000;
        work->xform1C[1].scale18.vz = 0x1000;
        fxm_valuePathInit(&fx_0d2_morphCurve, (u8 *)ex->btlfx_getFileData(entry->file, FX_FENRIR_SUMMON_MORPH_CURVE_15));
        work->modelF4[1] = ex->btlfx_registerEffectModel((Unk800E8F50 *)ex->btlfx_getFileData(entry->file, FX_FENRIR_SUMMON_MODEL_6), 1);
        work->modelF4[2] = ex->btlfx_registerEffectModel((Unk800E8F50 *)ex->btlfx_getFileData(entry->file, FX_FENRIR_SUMMON_MODEL_13), 1);
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
            fxm_rotPosPathInit(&fx_0d2_slicePaths[k], (u8 *)fxm_offsetTableEntry((u8 *)ex->btlfx_getFileData(entry->file, FX_FENRIR_SUMMON_SLICE_PATHS_17), k));
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
        work->modelF4[9] = ex->btlfx_registerEffectModel((Unk800E8F50 *)ex->btlfx_getFileData(entry->file, FX_FENRIR_SUMMON_MODEL_30), 1);
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
            fxm_rotPosPathGet(&fx_0d2_ringPath, frame, rp, &ringPos);
            work->xform1C[0].pos8.vx = ringPos.vx;
            work->xform1C[0].pos8.vy = ringPos.vy;
            work->xform1C[0].pos8.vz = ringPos.vz;
            work->xform1C[0].rot0.vz = -work->xform1C[0].rot0.vz;
            ex->Hi_SetEffModelSlice(work->modelF4[0], work->xform1C[0].pos8.vy - 0x2D5);
            ex->Hi_DrawSliceEffModel(rp, (VECTOR *)&work->xform1C[0].pos8, (s32)&work->xform1C[0].scale18,
                                     work->modelF4[0]);
            fxm_valuePathGet(&fx_0d2_morphCurve, frame, &morphT);
            ex->Hi_DrawMorphEffModel(&work->xform1C[1].rot0, (VECTOR *)&work->xform1C[1].pos8, (s32)&work->xform1C[1].scale18,
                                     work->modelF4[1], work->modelF4[2], morphT);
            for (slice = 0; slice < 6; slice++) {
                fxm_rotPosPathGet(&fx_0d2_slicePaths[slice], frame, &work->xform1C[2].rot0, &slicePos);
                work->xform1C[2].pos8.vx = slicePos.vx;
                work->xform1C[2].pos8.vy = slicePos.vy;
                work->xform1C[2].pos8.vz = slicePos.vz;
                ex->Hi_SetEffModelSlice(work->modelF4[slice + 3], work->xform1C[2].pos8.vy);
                ex->Hi_DrawSliceEffModel(&work->xform1C[2].rot0, (VECTOR *)&work->xform1C[2].pos8,
                                         (s32)&work->xform1C[2].scale18, work->modelF4[slice + 3]);
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

s32 fx_0d2_op83(s32 mode, Fx0d2Work83 *work, Unk800F78A4 *env, Unk80100C78Entry *entry, s32 *args) {
    BtlfxExports *ex = BTLFX_EXPORTS(env);
    VECTOR sum;
    SVECTOR slicePos;
    SVECTOR morphPos;
    SVECTOR piecePos;
    SVECTOR unitOff;
    VECTOR unitScale;
    s32 morphT;
    s32 frame;
    s32 i;
    s32 modelIdx;
    s32 elapsed;
    s32 unit;
    s32 pieceFrame;
    s32 pathFrame;
    s32 morphFrom;
    s32 morphTo;
    u16 model;

    if (mode == 0) {
        *args = sizeof(Fx0d2Work83);
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
        fxm_rotPosPathInit((Fx0d2Path *)&fx_0d2_slicePath2, (u16 *)ex->btlfx_getFileData(entry->file, FX_FENRIR_SLICE_PATH_2));
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
        fxm_rotPosPathInit((Fx0d2Path *)&fx_0d2_morphPath, (u16 *)ex->btlfx_getFileData(entry->file, FX_FENRIR_MORPH_PATH));
        for (i = 0; i < 3; i++) {
            model = ex->Hi_RegisterTexPtrModel((BtlModelHeader *)ex->btlfx_getFileData(entry->file, fx_0d2_morphBlocks[i]),
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
                          (u16 *)fxm_offsetTableEntry((u8 *)ex->btlfx_getFileData(entry->file, FX_FENRIR_UNIT_PATHS), i));
        }
        fxm_posPathInit((Fx0d2Path *)&fx_0d2_unitOffsetPath, (u16 *)ex->btlfx_getFileData(entry->file, FX_FENRIR_UNIT_OFFSET_PATH));
        fxm_vecPathInit((Fx0d2Path *)&fx_0d2_unitScalePath, (u16 *)ex->btlfx_getFileData(entry->file, FX_FENRIR_UNIT_SCALE_PATH));
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
            work->emit0 = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, FX_FENRIR_DUST_DEBRIS), (s32)&work->pos118);
            if (work->emit0 != NULL) {
                work->emit0->otZ = 0x118;
            }
            break;
        case 12:
            sum.vx = 0;
            sum.vy = 0;
            sum.vz = 0;
            for (unit = 0; unit < env->targets->targetCount; unit++) {
                sum.vx += work->unitPos58[unit].vx;
                sum.vy += (&work->unitPos58[unit])->vy;
                sum.vz += (&work->unitPos58[unit])->vz;
            }
            work->pos120.vx = sum.vx / env->targets->targetCount;
            work->pos120.vy = sum.vy / env->targets->targetCount;
            work->pos120.vz = sum.vz / env->targets->targetCount;
            work->emit4 = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, FX_FENRIR_BLUE_RINGS), (s32)&work->pos120);
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
            for (unit = 0; unit < env->targets->targetCount; unit++) {
                ex->btlfx_setUnitScale(unit, (s32 *)&fx_0d2_unitScale[unit]);
            }
            env->camera->points[0].vx = 0;
            env->camera->points[0].vy = 0;
            env->camera->points[0].vz = 0x4B0;
            break;
        }
        elapsed = frame - 1;
        if ((u32)elapsed < 0x17) {
            fxm_rotPosPathGet((Fx0d2KeyPath *)&fx_0d2_slicePath2, frame, &work->rot8, &slicePos);
            work->pos10.vx = slicePos.vx;
            work->pos10.vy = slicePos.vy;
            work->pos10.vz = slicePos.vz;
            work->rot8.vz = -work->rot8.vz;
            ex->Hi_SetEffModelSlice(work->model136[0], work->pos10.vy);
            ex->Hi_DrawSliceEffModel(&work->rot8, (VECTOR *)&work->pos10, (s32)&work->scale20, work->model136[0]);
        }
        if ((u32)elapsed < 0x27) {
            pathFrame = frame;
            switch (frame) {
            case 1:
                fxm_valuePathInit((Fx0d2Path *)&fx_0d2_morphCurve2, (u16 *)ex->btlfx_getFileData(entry->file, FX_FENRIR_MORPH_CURVE));
                break;
            case 4:
                fxm_valuePathInit((Fx0d2Path *)&fx_0d2_morphCurve2, (u16 *)ex->btlfx_getFileData(entry->file, FX_FENRIR_MORPH_CURVE_2));
                break;
            }
            if (pathFrame < 4) {
                morphFrom = 1;
                morphTo = 2;
            } else {
                morphFrom = 2;
                morphTo = 3;
            }
            fxm_rotPosPathGet((Fx0d2KeyPath *)&fx_0d2_morphPath, pathFrame, &work->rot8, &morphPos);
            work->pos10.vx = morphPos.vx;
            work->pos10.vy = morphPos.vy;
            work->pos10.vz = morphPos.vz;
            work->rot8.vz = -work->rot8.vz;
            fxm_valuePathGet(&fx_0d2_morphCurve2, pathFrame + 1, &morphT);
            ex->Hi_DrawMorphEffModel(&work->rot8, (VECTOR *)&work->pos10, (s32)&work->scale20, work->model136[morphFrom],
                                     work->model136[morphTo], morphT);
        }
        if ((u32)(frame - 2) < 0x18) {
            pieceFrame = frame - 2;
            modelIdx = 4;
            for (i = 0; i < 10; i++) {
                fxm_rotPosPathGet((Fx0d2KeyPath *)&fx_0d2_piecePaths[i], pieceFrame, &work->rot30, &piecePos);
                work->pos38.vx = piecePos.vx;
                work->pos38.vy = piecePos.vy;
                work->pos38.vz = piecePos.vz;
                ex->Hi_DrawEffModel(&work->rot30, (VECTOR *)&work->pos38, (s32)&work->scale48, work->model136[modelIdx]);
                modelIdx++;
                if (modelIdx >= 9) {
                    modelIdx = 4;
                }
            }
        }
        if ((u32)frame < 0x1C) {
            fxm_posPathGet((Fx0d2Path *)&fx_0d2_unitOffsetPath, frame, &unitOff);
            fxm_vecPathGet((Fx0d2Path *)&fx_0d2_unitScalePath, frame, &unitScale);
            for (unit = 0; unit < env->targets->targetCount; unit++) {
                work->unitPos58[unit].vx = fx_0d2_unitPos[unit].vx + unitOff.vx;
                work->unitPos58[unit].vy = fx_0d2_unitPos[unit].vy + unitOff.vy;
                work->unitPos58[unit].vz = fx_0d2_unitPos[unit].vz + unitOff.vz;
                ex->btlfx_setUnitPos(unit, (s16 *)&work->unitPos58[unit]);
                work->unitScale98[unit].vx = (fx_0d2_unitScale[unit].vx * unitScale.vx) / 4096;
                work->unitScale98[unit].vy = (fx_0d2_unitScale[unit].vy * unitScale.vy) / 4096;
                work->unitScale98[unit].vz = (fx_0d2_unitScale[unit].vz * unitScale.vz) / 4096;
                ex->btlfx_setUnitScale(unit, (s32 *)&work->unitScale98[unit]);
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

s32 fx_0d2_op84(s32 mode, Fx0d2Work84 *work, Unk800F78A4 *env, Unk80100C78Entry *entry, s32 *args) {
    BtlfxExports *ex = BTLFX_EXPORTS(env);
    s32 frame;
    s32 i;
    s32 j;
    Pool6CEntry *emitter;

    if (mode == 0) {
        *args = sizeof(Fx0d2Work84);
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
                emitter = ex->btlfx_startEmitter((s32)ex->btlfx_getFileData(entry->file, FX_FENRIR_DUST_CLOUD), (s32)&work->emitPos60[i]);
                work->emit0[i] = emitter;
                if (emitter != NULL) {
                    emitter->otZ = 0x80;
                }
            }
        }
        return frame >= 0x28;
    }
    return 0;
}

s32 fx_0d2_op85(s32 mode, s16 *work, Unk800F78A4 *env, Unk80100C78Entry *entry, s32 *args) {
    s32 frame;

    if (mode == 0) {
        *args = 2;
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

static s32 fxm_offsetTableEntry(u8 *base, u16 idx) {
    return (s32)base + ((s32 *)base)[(u16)idx + 1];
}

static void fxm_lerpSvec(SVECTOR *from, SVECTOR *to, s32 t, SVECTOR *out) {
    BTLFX_EXPORTS(fx_0d2_env)->LoadAverageShort12(from, to, 0x1000 - t, t, out);
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
    Fx0d2Point *key;

    if (last < t) {
        t = last;
    } else {
        done = 0;
    }
    key = &path->points4[t];
    out->vx = key->x;
    out->vy = key->y;
    out->vz = key->z;
    return done;
}

static void fxm_rotPosPathInit(Fx0d2Path *path, u16 *data) {
    data++;
    path->count2 = *data++;
    path->points4 = (Fx0d2Point *)data;
}

static s32 fxm_rotPosPathGet(Fx0d2KeyPath *path, s32 t, SVECTOR *rot, SVECTOR *pos) {
    Fx0d2Key *key;
    s32 done = 1;

    if (t > path->count2 - 1) {
        t = path->count2 - 1;
    } else {
        done = 0;
    }
    key = &path->keys4[t];
    if (rot != NULL) {
        rot->vx = key->rot6[0];
        rot->vy = key->rot6[1];
        rot->vz = key->rot6[2];
    }
    if (pos != NULL) {
        pos->vx = key->pos0[0];
        pos->vy = key->pos0[1];
        pos->vz = key->pos0[2];
    }
    return done;
}

static void fxm_valuePathInit(Fx0d2Path *path, u16 *data) {
    data++;
    path->count2 = *data++;
    path->points4 = (Fx0d2Point *)data;
}

static s32 fxm_valuePathGet(Fx0d2Keys *keys, s32 t, s32 *out) {
    s32 done = 1;

    if (t > keys->count2 - 1) {
        t = keys->count2 - 1;
    } else {
        done = 0;
    }
    *out = *(s16 *)((u8 *)keys->keys4 + (t << 1));
    return done;
}

static void fxm_vecPathInit(Fx0d2Path *path, u16 *data) {
    data++;
    path->count2 = *data++;
    path->points4 = (Fx0d2Point *)data;
}

static s32 fxm_vecPathGet(Fx0d2Path *track, s32 t, VECTOR *out) {
    s32 last = track->count2 - 1;
    s32 done = 1;
    Fx0d2Point *key;

    if (last < t) {
        t = last;
    } else {
        done = 0;
    }
    key = &track->points4[t];
    out->vx = key->x;
    out->vy = key->y;
    out->vz = key->z;
    return done;
}

static void fxm_tintScreenLerp(s32 from, s32 to, s32 t, s32 n, s32 abr, s32 ot) {
    s32 bright = (from * (n - t) + to * t) / n;

    BTLFX_EXPORTS(fx_0d2_env)->btlfx_drawScreenTint((u32 *)(fx_0d2_env->ot + ot), abr, bright, bright, bright);
}

static s32 fxm_moveUnitHome(s32 unit, s32 t, s32 n, SVECTOR *start) {
    BtlfxExports *ex = BTLFX_EXPORTS(fx_0d2_env);
    SVECTOR home;
    SVECTOR out;

    ex->btlfx_getUnitHomePos(unit, (s16 *)&home);
    fxm_lerpSvec(start, &home, (t << 12) / n, &out);
    ex->btlfx_setUnitPos(unit, (s16 *)&out);
    return t >= n;
}

static void fxm_camChanReset(void) {
    s32 i;
    s32 freeMark = FX_0D2_KEY_FREE;

    for (i = 63; i >= 0; i--) {
        fx_0d2_camChans[i].left6 = freeMark;
    }
    for (i = 3; i >= 0; i--) {
        fx_0d2_camChanFrame[i] = 0;
    }
}

static void fxm_camChanAdd(s16 start, u16 dst, s16 ch, s16 *data) {
    s32 i;
    s32 count;

    data++;
    count = *data;
    for (i = 0; i < 64; i++) {
        if (fx_0d2_camChans[i].left6 == FX_0D2_KEY_FREE) {
            fx_0d2_camChans[i].left6 = 0;
            fx_0d2_camChans[i].data0 = data;
            fx_0d2_camChans[i].start4 = start;
            fx_0d2_camChans[i].dstA = dst;
            fx_0d2_camChans[i].keyC = 0;
            fx_0d2_camChans[i].keyCountE = count;
            fx_0d2_camChans[i].chan8 = ch;
            return;
        }
    }
}

static void fxm_camChanUpdate(s32 ch) {
    Unk800F78A4 *env = fx_0d2_env;
    s32 i;
    s32 k;
    s32 dstSlot;

    for (i = 0; i < 64; i++) {
        if (fx_0d2_camChans[i].chan8 == ch) {
            if (fx_0d2_camChans[i].left6 == 0 && fx_0d2_camChans[i].start4 == fx_0d2_camChanFrame[ch]) {
                fx_0d2_camChans[i].left6 = *fx_0d2_camChans[i].data0;
                fx_0d2_camChans[i].data0++;
            }
            if (fx_0d2_camChans[i].left6 > 0) {
                fx_0d2_camChans[i].left6--;
                for (k = 0; k < 3; k++) {
                    dstSlot = BTLFX_CAM_SLOT(fx_0d2_camChans[i].dstA, k);
                    if (dstSlot >= 0) {
                        ((SVECTOR *)env->camera)[dstSlot].vx = fx_0d2_camChans[i].data0[0];
                        ((SVECTOR *)env->camera)[dstSlot].vy = fx_0d2_camChans[i].data0[1];
                        ((SVECTOR *)env->camera)[dstSlot].vz = fx_0d2_camChans[i].data0[2];
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
