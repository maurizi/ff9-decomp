#include "fxmod/fx_1fc.h"

s32 fx_1fc_op80(s32 mode, void *work, Unk800F78A4 *env, Unk80100C78Entry *entry, s32 *args) {
    BtlfxExports *ex = BTLFX_EXPORTS(env);
    s32 frame;
    s32 i;

    if (mode == 0) {
        *args = 0;
    } else if (mode == 1) {
        fx_1fc_env = env;
        ex->btlfx_initEffModels(fx_1fc_effModels, FX_FENRIR_MODEL_COUNT);
        for (i = 0; i < env->targets->targetCount; i++) {
            ex->btlfx_getUnitPos(i, (s16 *)&fx_1fc_unitPos[i]);
            ex->btlfx_getUnitScale(i, (s32 *)&fx_1fc_unitScale[i]);
        }
        ex->btlfx_drawBgOffscreen(FX_FENRIR_BG_VRAM_X, FX_FENRIR_BG_VRAM_Y, FX_FENRIR_BG_VRAM_W, FX_FENRIR_BG_VRAM_H, fx_1fc_bgOt, 0, 0x200, 0x2BC);
        fxm_camChanReset();
        fxm_camChanAdd(0, 1, 0, (s16 *)ex->btlfx_getFileData(entry->file, FX_FENRIR_CAMERA_KEYS_1));
        fxm_camChanAdd(0, 2, 0, (s16 *)ex->btlfx_getFileData(entry->file, FX_FENRIR_CAMERA_KEYS_2));
        fxm_camChanAdd(0, 3, 0, (s16 *)ex->btlfx_getFileData(entry->file, FX_FENRIR_CAMERA_KEYS_3));
    } else {
        frame = entry->frame;
        env->camera->pointRots[1].vz = env->camera->points[2].vz;
        fxm_camChanUpdate(0);
        return frame >= FX_FENRIR_CAMERA_FRAMES;
    }
    return 0;
}

s32 fx_1fc_op81(s32 mode, void *work, Unk800F78A4 *env, Unk80100C78Entry *entry, s32 *args) {
    if (mode == 0) {
        *args = 0;
    } else if (mode == 1) {
        fx_1fc_env = env;
    } else {
        return entry->frame > 0;
    }
    return 0;
}

s32 fx_1fc_op82(s32 mode, void *work, Unk800F78A4 *env, Unk80100C78Entry *entry, s32 *args) {
    if (mode == 0) {
        *args = 0;
    } else if (mode == 1) {
        fx_1fc_env = env;
    } else {
        return entry->frame > 0;
    }
    return 0;
}

INCLUDE_RODATA("asm/us/fx_1fc/nonmatchings/fx_1fc", fx_1fc_headerPtr);

s32 fx_1fc_op83(s32 mode, Fx1fcWork83 *work, Unk800F78A4 *env, Unk80100C78Entry *entry, s32 *args) {
    BtlfxExports *ex = BTLFX_EXPORTS(env);
    VECTOR sum;
    SVECTOR slicePos;
    SVECTOR morphPos;
    SVECTOR piecePos;
    SVECTOR unitOfs;
    VECTOR scaleMul;
    s32 morph;
    s32 frame;
    s32 i;
    s32 modelIdx;
    s32 pathFrame;
    s32 unit;
    s32 pieceFrame;
    s32 unused0;
    s32 fromModel;
    s32 toModel;
    u16 model;

    if (mode == 0) {
        *args = sizeof(Fx1fcWork83);
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
        fx_1fc_env = env;
        fxm_rotPosPathInit((Fx1fcPath *)&fx_1fc_rotPosPath0, (u16 *)ex->btlfx_getFileData(entry->file, FX_FENRIR_SLICE_PATH_2));
        work->model136[0] = ex->btlfx_registerEffectModel(&fx_1fc_sliceModelData, 1);
        work->rot8.vx = 0;
        work->rot8.vy = 0x800;
        work->rot8.vz = 0;
        work->pos10.vx = 0;
        work->pos10.vy = 0;
        work->pos10.vz = 0;
        work->scale20.vx = 0x1000;
        work->scale20.vy = 0x1000;
        work->scale20.vz = 0x1000;
        fxm_rotPosPathInit((Fx1fcPath *)&fx_1fc_rotPosPath1, (u16 *)ex->btlfx_getFileData(entry->file, FX_FENRIR_MORPH_PATH));
        for (i = 0; i < FX_FENRIR_MORPH_MODELS; i++) {
            model = ex->Hi_RegisterTexPtrModel((BtlModelHeader *)ex->btlfx_getFileData(entry->file, fx_1fc_modelFileIds[i]),
                                               fx_1fc_texPages, fx_1fc_texUv);
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
        for (i = 0; i < FX_FENRIR_PIECE_MODELS; i++) {
            work->model136[i + 1 + FX_FENRIR_MORPH_MODELS] = ex->btlfx_registerEffectModel((Unk800E8F50 *)fxm_offsetTableEntry(fx_1fc_effModelTable, i), 1);
        }
        for (i = 0; i < FX_FENRIR_PIECES; i++) {
            fxm_rotPosPathInit((Fx1fcPath *)&fx_1fc_rotPosPath2[i],
                          (u16 *)fxm_offsetTableEntry((u8 *)ex->btlfx_getFileData(entry->file, FX_FENRIR_UNIT_PATHS), i));
        }
        fxm_posPathInit((Fx1fcPath *)&fx_1fc_posPath, (u16 *)ex->btlfx_getFileData(entry->file, FX_FENRIR_UNIT_OFFSET_PATH));
        fxm_vecPathInit((Fx1fcPath *)&fx_1fc_vecPath, (u16 *)ex->btlfx_getFileData(entry->file, FX_FENRIR_UNIT_SCALE_PATH));
        work->timer130 = -1;
        work->timer132 = -1;
        work->active134 = 0;
    } else {
        frame = entry->frame;
        switch (frame) {
        case FX_FENRIR_DEBRIS_FRAME:
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
        case FX_FENRIR_BLUE_RINGS_FRAME:
            sum.vx = 0;
            sum.vy = 0;
            sum.vz = 0;
            for (unit = 0; unit < env->targets->targetCount; unit++) {
                sum.vx += work->pos58[unit].vx;
                sum.vy += (&work->pos58[unit])->vy;
                sum.vz += (&work->pos58[unit])->vz;
            }
            work->pos120.vx = sum.vx / env->targets->targetCount;
            work->pos120.vy = sum.vy / env->targets->targetCount;
            work->pos120.vz = sum.vz / env->targets->targetCount;
            work->emit4 = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, FX_FENRIR_BLUE_RINGS), (s32)&work->pos120);
            break;
        case FX_FENRIR_DEBRIS_STOP_FRAME:
            ex->btlfx_stopEmitter(work->emit0);
            break;
        case FX_FENRIR_WHITE_IN_FRAME:
            work->timer130 = FX_FENRIR_WHITE_FADE_FRAMES;
            break;
        case FX_FENRIR_WHITE_OUT_FRAME:
            work->timer132 = FX_FENRIR_WHITE_FADE_FRAMES;
            work->active134 = 0;
            for (unit = 0; unit < env->targets->targetCount; unit++) {
                ex->btlfx_setUnitScale(unit, (s32 *)&fx_1fc_unitScale[unit]);
            }
            env->camera->points[0].vx = 0;
            env->camera->points[0].vy = 0;
            env->camera->points[0].vz = 0x4B0;
            break;
        }
        pathFrame = frame - 1;
        if ((u32)pathFrame < FX_FENRIR_GROUND_SLICE_FRAMES) {
            fxm_rotPosPathGet((Fx1fcPath_0b4 *)&fx_1fc_rotPosPath0, pathFrame, &work->rot8, &slicePos);
            work->pos10.vx = slicePos.vx;
            work->pos10.vy = slicePos.vy;
            work->pos10.vz = slicePos.vz;
            work->rot8.vz = -work->rot8.vz;
            ex->Hi_SetEffModelSlice(work->model136[0], work->pos10.vy);
            ex->Hi_DrawSliceEffModel(&work->rot8, (VECTOR *)&work->pos10, (s32)&work->scale20, work->model136[0]);
        }
        if ((u32)pathFrame < FX_FENRIR_MORPH_FRAMES) {
            switch (pathFrame) {
            case 0:
                fxm_valuePathInit((Fx1fcPath *)&fx_1fc_valuePath, (u16 *)ex->btlfx_getFileData(entry->file, FX_FENRIR_MORPH_CURVE));
                break;
            case FX_FENRIR_MORPH_2_FRAME:
                fxm_valuePathInit((Fx1fcPath *)&fx_1fc_valuePath, (u16 *)ex->btlfx_getFileData(entry->file, FX_FENRIR_MORPH_CURVE_2));
                break;
            }
            if (pathFrame < FX_FENRIR_MORPH_2_FRAME) {
                fromModel = 1;
                toModel = 2;
            } else {
                fromModel = 2;
                toModel = 3;
            }
            fxm_rotPosPathGet((Fx1fcPath_0b4 *)&fx_1fc_rotPosPath1, pathFrame, &work->rot8, &morphPos);
            work->pos10.vx = morphPos.vx;
            work->pos10.vy = morphPos.vy;
            work->pos10.vz = morphPos.vz;
            work->rot8.vz = -work->rot8.vz;
            fxm_valuePathGet(&fx_1fc_valuePath, pathFrame, &morph);
            ex->Hi_DrawMorphEffModel(&work->rot8, (VECTOR *)&work->pos10, (s32)&work->scale20, work->model136[fromModel],
                                     work->model136[toModel], morph);
        }
        if ((u32)(frame - FX_FENRIR_PIECES_FRAME) < FX_FENRIR_PIECES_FRAMES) {
            pieceFrame = frame - FX_FENRIR_PIECES_FRAME;
            modelIdx = 1 + FX_FENRIR_MORPH_MODELS;
            for (i = 0; i < FX_FENRIR_PIECES; i++) {
                fxm_rotPosPathGet((Fx1fcPath_0b4 *)&fx_1fc_rotPosPath2[i], pieceFrame, &work->rot30, &piecePos);
                work->pos38.vx = piecePos.vx;
                work->pos38.vy = piecePos.vy;
                work->pos38.vz = piecePos.vz;
                ex->Hi_DrawEffModel(&work->rot30, (VECTOR *)&work->pos38, (s32)&work->scale48, work->model136[modelIdx]);
                modelIdx++;
                if (modelIdx >= 1 + FX_FENRIR_MORPH_MODELS + FX_FENRIR_PIECE_MODELS) {
                    modelIdx = 1 + FX_FENRIR_MORPH_MODELS;
                }
            }
        }
        if ((u32)frame < FX_FENRIR_UNIT_PATH_FRAMES) {
            fxm_posPathGet((Fx1fcPath *)&fx_1fc_posPath, frame, &unitOfs);
            fxm_vecPathGet((Fx1fcTrack6 *)&fx_1fc_vecPath, frame, &scaleMul);
            for (unit = 0; unit < env->targets->targetCount; unit++) {
                work->pos58[unit].vx = fx_1fc_unitPos[unit].vx + unitOfs.vx;
                work->pos58[unit].vy = fx_1fc_unitPos[unit].vy + unitOfs.vy;
                work->pos58[unit].vz = fx_1fc_unitPos[unit].vz + unitOfs.vz;
                ex->btlfx_setUnitPos(unit, (s16 *)&work->pos58[unit]);
                work->scale98[unit].vx = (fx_1fc_unitScale[unit].vx * scaleMul.vx) / 4096;
                work->scale98[unit].vy = (fx_1fc_unitScale[unit].vy * scaleMul.vy) / 4096;
                work->scale98[unit].vz = (fx_1fc_unitScale[unit].vz * scaleMul.vz) / 4096;
                ex->btlfx_setUnitScale(unit, (s32 *)&work->scale98[unit]);
            }
        }
        if ((u32)(frame - FX_FENRIR_DEBRIS_FRAME) < FX_FENRIR_DEBRIS_STOP_FRAME - FX_FENRIR_DEBRIS_FRAME && work->emit0 != NULL) {
            if (work->emit0->state == BTLFX_EMIT_DONE) {
                work->emit0 = NULL;
            }
            if (work->emit0 != NULL) {
                work->emit0->rot.vx = work->rot128.vx;
                work->emit0->rot.vy = work->rot128.vy;
                work->emit0->rot.vz = work->rot128.vz;
            }
        }
        if (work->timer130 >= 0) {
            fxm_tintScreenLerp(0, 0xFF, FX_FENRIR_WHITE_FADE_FRAMES - work->timer130, FX_FENRIR_WHITE_FADE_FRAMES, BTLFX_ABR_ADD, 0);
            if (--work->timer130 == -1) {
                work->active134 = 1;
            }
        }
        if (work->timer132 >= 0) {
            fxm_tintScreenLerp(0xFF, 0, FX_FENRIR_WHITE_FADE_FRAMES - work->timer132, FX_FENRIR_WHITE_FADE_FRAMES, BTLFX_ABR_ADD, 0);
            work->timer132--;
        }
        if (work->active134 != 0) {
            ex->btlfx_drawScreenTint((u32 *)env->ot, BTLFX_ABR_ADD, FX_FENRIR_WHITE_RGB);
        }
        return frame >= FX_FENRIR_GROUND_FRAMES;
    }
    return 0;
}

s32 fx_1fc_op84(s32 mode, Fx1fcWork84 *work, Unk800F78A4 *env, Unk80100C78Entry *entry, s32 *args) {
    BtlfxExports *ex = BTLFX_EXPORTS(env);
    s32 frame;
    s32 i;
    s32 j;
    SVECTOR *homePos;
    SVECTOR *emitPos;

    if (mode == 0) {
        *args = sizeof(Fx1fcWork84);
    } else if (mode == 1) {
        fx_1fc_env = env;
        for (j = 0; j < env->targets->targetCount; j++) {
            work->doneA0[j] = 0;
            ex->btlfx_getUnitHomePos(j, (s16 *)&work->pos20[j]);
            work->pos20[j].vy = -FX_FENRIR_DROP_HEIGHT;
            ex->btlfx_setUnitPos(j, (s16 *)&work->pos20[j]);
        }
    } else {
        frame = entry->frame;
        for (i = 0; i < env->targets->targetCount; i++) {
            if (work->doneA0[i] == 0 && fxm_moveUnitHome(i, frame, FX_FENRIR_DROP_FRAMES, &work->pos20[i]) != 0) {
                work->doneA0[i] = 1;
                ex->btlfx_getUnitHomePos(i, (s16 *)&work->pos20[i]);
                ex->btlfx_setUnitPos(i, (s16 *)&work->pos20[i]);
                homePos = (SVECTOR *)((u8 *)work + i * 8 + 0x20);
                emitPos = (SVECTOR *)((u8 *)work + i * 8 + 0x60);
                emitPos->vx = homePos->vx;
                emitPos->vy = homePos->vy;
                emitPos->vz = homePos->vz;
                work->emit0[i] = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, FX_FENRIR_DUST_CLOUD), (s32)&work->emitPos60[i]);
                if (work->emit0[i] != NULL) {
                    work->emit0[i]->otZ = 0x80;
                }
            }
        }
        return frame >= FX_FENRIR_LAND_FRAMES;
    }
    return 0;
}

static s32 fxm_offsetTableEntry(u8 *base, u16 idx) {
    return (s32)base + ((s32 *)base)[(u16)idx + 1];
}

static void fxm_lerpSvec(SVECTOR *from, SVECTOR *to, s32 t, SVECTOR *out) {
    BTLFX_EXPORTS(fx_1fc_env)->LoadAverageShort12(from, to, 0x1000 - t, t, out);
}

static s32 fxm_lerpRatio(s32 from, s32 to, s32 t, s32 n) {
    return (from * (n - t) + to * t) / n;
}

static void fxm_posPathInit(Fx1fcPath *path, u16 *data) {
    data++;
    path->count2 = *data++;
    path->keys4 = (Fx1fcPoint *)data;
}

static s32 fxm_posPathGet(Fx1fcPath *path, s32 t, SVECTOR *out) {
    s32 last = path->count2 - 1;
    s32 done = 1;
    Fx1fcPoint *key;

    if (last < t) {
        t = last;
    } else {
        done = 0;
    }
    key = &path->keys4[t];
    out->vx = key->x;
    out->vy = key->y;
    out->vz = key->z;
    return done;
}

static void fxm_rotPosPathInit(Fx1fcPath *path, u16 *data) {
    data++;
    path->count2 = *data++;
    path->keys4 = (Fx1fcPoint *)data;
}

static s32 fxm_rotPosPathGet(Fx1fcPath_0b4 *path, s32 t, SVECTOR *rot, SVECTOR *pos) {
    Fx1fcKey *key;
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

static void fxm_valuePathInit(Fx1fcPath *path, u16 *data) {
    data++;
    path->count2 = *data++;
    path->keys4 = (Fx1fcPoint *)data;
}

static s32 fxm_valuePathGet(Fx1fcKeys *keys, s32 t, s32 *out) {
    s32 done = 1;

    if (t > keys->count2 - 1) {
        t = keys->count2 - 1;
    } else {
        done = 0;
    }
    *out = *(s16 *)((u8 *)keys->keys4 + (t << 1));
    return done;
}

static void fxm_vecPathInit(Fx1fcPath *path, u16 *data) {
    data++;
    path->count2 = *data++;
    path->keys4 = (Fx1fcPoint *)data;
}

static s32 fxm_vecPathGet(Fx1fcTrack6 *track, s32 t, VECTOR *out) {
    s32 last = track->count2 - 1;
    s32 done = 1;
    Fx1fcKey6 *key;

    if (last < t) {
        t = last;
    } else {
        done = 0;
    }
    key = &track->keys4[t];
    out->vx = key->x;
    out->vy = key->y;
    out->vz = key->z;
    return done;
}

static void fxm_tintScreenLerp(s32 from, s32 to, s32 t, s32 n, s32 abr, s32 ot) {
    s32 bright = (from * (n - t) + to * t) / n;

    BTLFX_EXPORTS(fx_1fc_env)->btlfx_drawScreenTint((u32 *)(fx_1fc_env->ot + ot), abr, bright, bright, bright);
}

static s32 fxm_moveUnitHome(s32 unit, s32 t, s32 n, SVECTOR *start) {
    BtlfxExports *ex = BTLFX_EXPORTS(fx_1fc_env);
    SVECTOR home;
    SVECTOR out;

    ex->btlfx_getUnitHomePos(unit, (s16 *)&home);
    fxm_lerpSvec(start, &home, (t << 12) / n, &out);
    ex->btlfx_setUnitPos(unit, (s16 *)&out);
    return t >= n;
}

static void fxm_camChanReset(void) {
    s32 i;
    s32 freeMark = FX_1FC_KEY_FREE;

    for (i = FX_FENRIR_CAM_KEY_SLOTS - 1; i >= 0; i--) {
        fx_1fc_camKeys[i].timer6 = freeMark;
    }
    for (i = FX_FENRIR_CAM_CHANNELS - 1; i >= 0; i--) {
        fx_1fc_camChanFrames[i] = 0;
    }
}

static void fxm_camChanAdd(s16 start, u16 dst, s16 ch, s16 *data) {
    s32 i;
    s32 count;

    data++;
    count = *data;
    for (i = 0; i < FX_FENRIR_CAM_KEY_SLOTS; i++) {
        if (fx_1fc_camKeys[i].timer6 == FX_1FC_KEY_FREE) {
            fx_1fc_camKeys[i].timer6 = 0;
            fx_1fc_camKeys[i].keys0 = data;
            fx_1fc_camKeys[i].startFrame4 = start;
            fx_1fc_camKeys[i].slotsA = dst;
            fx_1fc_camKeys[i].indexC = 0;
            fx_1fc_camKeys[i].countE = count;
            fx_1fc_camKeys[i].channel8 = ch;
            return;
        }
    }
}

static void fxm_camChanUpdate(s32 ch) {
    Unk800F78A4 *env = fx_1fc_env;
    s32 i;
    s32 k;
    s32 dstSlot;

    for (i = 0; i < FX_FENRIR_CAM_KEY_SLOTS; i++) {
        if (fx_1fc_camKeys[i].channel8 == ch) {
            if (fx_1fc_camKeys[i].timer6 == 0 && fx_1fc_camKeys[i].startFrame4 == fx_1fc_camChanFrames[ch]) {
                fx_1fc_camKeys[i].timer6 = *fx_1fc_camKeys[i].keys0;
                fx_1fc_camKeys[i].keys0++;
            }
            if (fx_1fc_camKeys[i].timer6 > 0) {
                fx_1fc_camKeys[i].timer6--;
                for (k = 0; k < 3; k++) {
                    dstSlot = BTLFX_CAM_SLOT(fx_1fc_camKeys[i].slotsA, k);
                    if (dstSlot >= 0) {
                        ((SVECTOR *)env->camera)[dstSlot].vx = fx_1fc_camKeys[i].keys0[0];
                        ((SVECTOR *)env->camera)[dstSlot].vy = fx_1fc_camKeys[i].keys0[1];
                        ((SVECTOR *)env->camera)[dstSlot].vz = fx_1fc_camKeys[i].keys0[2];
                        if (fx_1fc_camKeys[i].indexC < fx_1fc_camKeys[i].countE) {
                            fx_1fc_camKeys[i].keys0 += 3;
                            fx_1fc_camKeys[i].indexC++;
                        }
                    }
                }
            }
        }
    }
    fx_1fc_camChanFrames[ch]++;
}
