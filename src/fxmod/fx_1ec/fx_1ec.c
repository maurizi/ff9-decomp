#include "fxmod/fx_1ec.h"

static s32 fx_1ec_op80ParticleMotion(Pool30Entry *pt, Pool6CEntry *owner, SVECTOR *pos, s32 trail, s32 frame, s32 n) {
    SVECTOR pathPos;

    fxm_posPathGet(&fx_1ec_posPath0[(s16)(pt->slot - 1)], frame, &pathPos);
    pathPos.vy = pathPos.vy * 4 / 5;
    pathPos.vz = pathPos.vz * 3 / 4;
    pathPos.vx += fx_1ec_op80Origin.vx;
    pathPos.vy += fx_1ec_op80Origin.vy;
    pathPos.vz += fx_1ec_op80Origin.vz;
    pos->vx = pathPos.vx;
    pos->vy = pathPos.vy;
    pos->vz = pathPos.vz;
    if (frame >= n) {
        return -1;
    }
    return 0;
}

static s32 fx_1ec_op81ParticleMotion(Pool30Entry *pt, Pool6CEntry *owner, SVECTOR *pos, s32 trail, s32 frame, s32 n) {
    SVECTOR pathPos;

    fxm_posPathGet(&fx_1ec_posPath1[(s16)(pt->slot - 1)], frame, &pathPos);
    pathPos.vx += fx_1ec_op81Origin.vx;
    pathPos.vy += fx_1ec_op81Origin.vy;
    pathPos.vz += fx_1ec_op81Origin.vz;
    pos->vx = pathPos.vx;
    pos->vy = pathPos.vy;
    pos->vz = pathPos.vz;
    if (frame >= n) {
        return -1;
    }
    return 0;
}

s32 fx_1ec_op80(s32 mode, Fx1ecWork80 *work, Unk800F78A4 *env, Unk80100C78Entry *entry, s32 *args) {
    BtlfxExports *ex = BTLFX_EXPORTS(env);
    s32 frame;
    s32 i;

    if (mode == 0) {
        *args = sizeof(Fx1ecWork80);
    } else if (mode == 1) {
        fx_1ec_env = env;
        ex->btlfx_getUnitBodyPos(BTLFX_UNIT_CASTER, 0, (s16 *)&work->pos24);
        work->pos2C.vx = env->targets->centerX;
        work->pos2C.vy = env->targets->centerY;
        work->pos2C.vz = env->targets->centerZ;
        work->pos34.vx = work->pos24.vx;
        work->pos34.vy = work->pos24.vy;
        work->pos34.vz = work->pos24.vz;
        work->unk3C.vx = work->pos24.vx;
        work->unk3C.vy = work->pos24.vy;
        work->unk3C.vz = work->pos24.vz;
        fx_1ec_op80Origin.vx = work->pos24.vx;
        fx_1ec_op80Origin.vy = work->pos24.vy;
        fx_1ec_op80Origin.vz = work->pos24.vz;
        work->pos34.vy = 0;
        for (i = 0; i < FX_ULTIMA_PATHS; i++) {
            fxm_posPathInit(&fx_1ec_posPath0[i], fxm_offsetTableEntry((u8 *)ex->btlfx_getFileData(entry->file, FX_ULTIMA_PATH_SET), i));
        }
        work->camPos4C.vx = work->pos2C.vx;
        work->camPos4C.vy = work->pos2C.vy;
        work->camPos4C.vz = work->pos2C.vz;
        work->camPos54.vx = work->pos2C.vx;
        work->camPos54.vy = work->pos2C.vy;
        work->camPos54.vz = work->pos2C.vz;
        work->camPos5C.vx = work->pos2C.vx;
        work->camPos5C.vy = work->pos2C.vy;
        work->camPos5C.vz = work->pos2C.vz;
        work->camPos4C.vy -= 0x2AF8;
        work->camPos4C.vz -= 0xFA0;
        work->camPos54.vy -= 0xC80;
        work->camPos54.vz -= 0xFA0;
        work->camPos5C.vy -= 0x2AF8;
        env->camera->points[0].vx = work->camPos4C.vx;
        env->camera->points[0].vy = work->camPos4C.vy;
        env->camera->points[0].vz = work->camPos4C.vz;
        env->camera->points[1].vx = work->camPos54.vx;
        env->camera->points[1].vy = work->camPos54.vy;
        env->camera->points[1].vz = work->camPos54.vz;
        env->camera->points[2].vx = work->camPos5C.vx;
        env->camera->points[2].vy = work->camPos5C.vy;
        env->camera->points[2].vz = work->camPos5C.vz;
    } else {
        frame = entry->frame;
        switch (frame) {
        case FX_ULTIMA_FLARE_FRAME:
            work->emit0[0] = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, FX_ULTIMA_RED_FLARE_2), (s32)&work->pos24);
            work->emit0[3] = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, FX_ULTIMA_GREY_TRAIL), (s32)&work->pos24);
            if (work->emit0[3] != NULL) {
                work->emit0[3]->trails = (s32)fx_1ec_op80Trails0;
                work->emit0[3]->trailSlots = FX_ULTIMA_GREY_TRAIL_TRAIL_SLOTS;
                work->emit0[3]->trailStride = FX_ULTIMA_GREY_TRAIL_TRAIL_STRIDE;
            }
            if (work->emit0[0] != NULL) {
                work->emit0[0]->otZ = ex->btlfx_addRadiusSixth(BTLFX_UNIT_CASTER, 0x80);
            }
            break;
        case FX_ULTIMA_HAZE_FRAME:
            work->emit0[1] = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, FX_ULTIMA_VIOLET_HAZE), (s32)&work->pos24);
            work->emit0[2] = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, FX_ULTIMA_ROCK_DEBRIS), (s32)&work->pos34);
            break;
        case FX_ULTIMA_LENS_FLARE_FRAME:
            work->emit0[5] = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, FX_ULTIMA_RED_LENS_FLARE), (s32)&work->pos24);
            work->emit0[6] = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, FX_ULTIMA_PINK_RING), (s32)&work->pos24);
            work->emit0[8] = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, FX_ULTIMA_ROCK_DEBRIS_2), (s32)&work->pos34);
            work->emit0[7] = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, FX_ULTIMA_ROCK_TRAIL), (s32)&fx_1ec_emitPos0);
            if (work->emit0[7] != NULL) {
                work->emit0[7]->trails = (s32)fx_1ec_op80Trails1;
                work->emit0[7]->trailSlots = FX_ULTIMA_ROCK_TRAIL_TRAIL_SLOTS;
                work->emit0[7]->trailStride = FX_ULTIMA_ROCK_TRAIL_TRAIL_STRIDE;
                if (work->emit0[7] != NULL) {
                    work->emit0[7]->file->m.module.update = fx_1ec_op80ParticleMotion;
                }
            }
            if (work->emit0[5] != NULL) {
                work->emit0[5]->otZ = 0x600;
            }
            if (work->emit0[6] != NULL) {
                work->emit0[6]->otZ = 0x600;
            }
            if (work->emit0[7] != NULL) {
                work->emit0[7]->otZ = 0x600;
            }
            break;
        case FX_ULTIMA_ROCK_TRAIL_STOP_FRAME:
            ex->btlfx_stopEmitter(work->emit0[7]);
            break;
        case FX_ULTIMA_FLASH_FRAME:
            ex->btlfx_drawScreenTint((u32 *)env->ot, BTLFX_ABR_ADD, FX_ULTIMA_WHITE_RGB);
            break;
        }
        ex->btlfx_drawLayerTexture((BtlBgLayers *)ex->btlfx_getFileData(entry->file, FX_ULTIMA_BACKDROP), frame);
        return frame >= FX_ULTIMA_OP80_FRAMES;
    }
    return 0;
}

INCLUDE_RODATA("asm/us/fx_1ec/nonmatchings/fx_1ec", fx_1ec_headerPtr);

s32 fx_1ec_op81(s32 mode, Fx1ecWork81 *work, Unk800F78A4 *env, Unk80100C78Entry *entry, s32 *args) {
    BtlfxExports *ex = BTLFX_EXPORTS(env);
    s32 frame;
    s32 i;

    if (mode == 0) {
        *args = sizeof(Fx1ecWork81);
    } else if (mode == 1) {
        work->pos2C.vx = env->targets->centerX;
        work->pos2C.vy = env->targets->centerY;
        work->pos2C.vz = env->targets->centerZ;
        fx_1ec_env = env;
        work->pos34.vx = work->pos2C.vx; work->pos34.vy = work->pos2C.vy; work->pos34.vz = work->pos2C.vz;
        work->pos3C.vx = work->pos2C.vx; work->pos3C.vy = work->pos2C.vy; work->pos3C.vz = work->pos2C.vz;
        work->pos44.vx = work->pos2C.vx; work->pos44.vy = work->pos2C.vy; work->pos44.vz = work->pos2C.vz;
        work->pos34.vy -= 0xC80;
        work->pos34.vz -= 0xFA0;
        work->pos3C.vy -= 0x2AF8;
        work->pos3C.vz -= 0xFA0;
        work->pos44.vy -= 0x2AF8;
        fx_1ec_op81Origin.vx = work->pos3C.vx;
        fx_1ec_op81Origin.vy = work->pos3C.vy;
        fx_1ec_op81Origin.vz = work->pos3C.vz;
        for (i = 0; i < FX_ULTIMA_PATHS_2; i++) {
            fxm_posPathInit(&fx_1ec_posPath1[i], fxm_offsetTableEntry((u8 *)ex->btlfx_getFileData(entry->file, FX_ULTIMA_PATH_SET_2), i));
        }
    } else {
    frame = entry->frame;
    switch (frame) {
    case 0:
        work->emit0[0] = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, FX_ULTIMA_WHITE_BEAM), (s32)&work->pos34);
        if (work->emit0[0] != NULL) {
            work->emit0[0]->trails = (s32)fx_1ec_op81Trails0;
            work->emit0[0]->trailSlots = FX_ULTIMA_WHITE_BEAM_TRAIL_SLOTS;
            work->emit0[0]->trailStride = FX_ULTIMA_WHITE_BEAM_TRAIL_STRIDE;
        }
        break;
    case FX_ULTIMA_BLAST_RINGS_FRAME:
        work->emit0[1] = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, FX_ULTIMA_BLAST_RINGS), (s32)&work->pos3C);
        break;
    case FX_ULTIMA_GREY_TRAIL_2_FRAME:
        work->emit0[2] = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, FX_ULTIMA_GREY_TRAIL_2), (s32)&work->pos3C);
        if (work->emit0[2] != NULL) {
            work->emit0[2]->trails = (s32)fx_1ec_op81Trails2;
            work->emit0[2]->trailSlots = FX_ULTIMA_GREY_TRAIL_2_TRAIL_SLOTS;
            work->emit0[2]->trailStride = FX_ULTIMA_GREY_TRAIL_2_TRAIL_STRIDE;
        }
        break;
    case FX_ULTIMA_COLOR_RING_FRAME:
        work->emit0[3] = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, FX_ULTIMA_COLOR_RING), (s32)&work->pos3C);
        break;
    case FX_ULTIMA_SPARKS_FRAME:
        work->emit0[4] = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, FX_ULTIMA_PINK_SPARKS), (s32)&work->pos3C);
        work->emit0[5] = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, FX_ULTIMA_VIOLET_GLOW), (s32)&work->pos3C);
        work->emit0[7] = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, FX_ULTIMA_RED_FLARE), (s32)&work->pos3C);
        work->emit0[8] = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, FX_ULTIMA_CRIMSON_STREAK), (s32)&work->pos3C);
        break;
    case FX_ULTIMA_FLASH_2_FRAME:
        ex->btlfx_drawScreenTint((u32 *)env->ot, BTLFX_ABR_ADD, FX_ULTIMA_WHITE_RGB);
        break;
    case FX_ULTIMA_PINK_TRAIL_FRAME:
        work->emit0[9] = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, FX_ULTIMA_PINK_TRAIL), (s32)&fx_1ec_emitPos1);
        if (work->emit0[9] != NULL) {
            work->emit0[9]->trails = (s32)fx_1ec_op81Trails3;
            work->emit0[9]->trailSlots = FX_ULTIMA_PINK_TRAIL_TRAIL_SLOTS;
            work->emit0[9]->trailStride = FX_ULTIMA_PINK_TRAIL_TRAIL_STRIDE;
            if (work->emit0[9] != NULL) {
                work->emit0[9]->file->m.module.update = fx_1ec_op81ParticleMotion;
            }
        }
        break;
    case FX_ULTIMA_PINK_TRAIL_2_FRAME:
        work->pos4C.vx = work->pos44.vx;
        work->pos4C.vy = work->pos44.vy;
        work->pos4C.vz = work->pos44.vz;
        work->pos4C.vy = -0xFA0;
        work->emit0[10] = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, FX_ULTIMA_PINK_TRAIL_2), (s32)&work->pos4C);
        if (work->emit0[10] != NULL) {
            work->emit0[10]->trails = (s32)fx_1ec_op81Trails1;
            work->emit0[10]->trailSlots = FX_ULTIMA_PINK_TRAIL_2_TRAIL_SLOTS;
            work->emit0[10]->trailStride = FX_ULTIMA_PINK_TRAIL_2_TRAIL_STRIDE;
        }
        break;
    case FX_ULTIMA_PINK_TRAIL_STOP_FRAME:
        ex->btlfx_stopEmitter(work->emit0[9]);
        break;
    }
    ex->btlfx_drawLayerTexture((BtlBgLayers *)ex->btlfx_getFileData(entry->file, FX_ULTIMA_BACKDROP), frame);
    return frame >= FX_ULTIMA_OP81_FRAMES;
    }
    return 0;
}

s32 fx_1ec_op82(s32 mode, Fx1ecWork82 *work, Unk800F78A4 *env, Unk80100C78Entry *entry, s32 *args) {
    BtlfxExports *ex = BTLFX_EXPORTS(env);
    s32 frame;
    s32 elapsed;
    s32 morph;
    s32 i;

    if (mode == 0) {
        *args = sizeof(Fx1ecWork82);
    } else if (mode == 1) {
        fx_1ec_env = env;
        ex->btlfx_initEffModels(fx_1ec_effModels, FX_ULTIMA_MODEL_SLOTS);
        work->pos4C.vx = env->targets->centerX;
        work->pos4C.vy = env->targets->centerY;
        work->pos4C.vz = env->targets->centerZ;
        work->rot20.vx = 0;
        work->rot20.vy = 0;
        work->rot20.vz = 0;
        work->pos28.vx = work->pos4C.vx;
        work->pos28.vy = 0;
        work->pos28.vz = work->pos4C.vz;
        work->scale38.vx = 0x4000;
        work->scale38.vy = 0x4000;
        work->scale38.vz = 0x4000;
        work->model6C = ex->btlfx_registerEffectModel((Unk800E8F50 *)ex->btlfx_getFileData(entry->file, FX_ULTIMA_SPIKE_CROWN), 1);
        work->model6E = ex->Hi_RegisterGouEffModel((BtlModelHeader *)ex->btlfx_getFileData(entry->file, FX_ULTIMA_GLOW_DOME));
        work->timer64 = -1;
        work->timer68 = -1;
        work->flag6A = 0;
    } else {
        frame = entry->frame;
        switch (frame) {
        case 0:
            work->pos54.vx = work->pos4C.vx;
            work->pos54.vy = work->pos4C.vy;
            work->pos54.vz = work->pos4C.vz;
            work->pos54.vy = -0x157C;
            work->emit0[0] = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, FX_ULTIMA_PINK_SPIKES), (s32)&work->pos54);
            if (work->emit0[0] != NULL) {
                work->emit0[0]->otZ = 0x500;
            }
            break;
        case FX_ULTIMA_ORB_BURST_FRAME:
            work->emit0[1] = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, FX_ULTIMA_PINK_ORB_BURST), (s32)&work->pos4C);
            if (work->emit0[1] != NULL) {
                work->emit0[1]->otZ = 0x500;
            }
            break;
        case FX_ULTIMA_SPIKES_2_FRAME:
            work->pos5C.vx = work->pos4C.vx;
            work->pos5C.vy = work->pos4C.vy;
            work->pos5C.vz = work->pos4C.vz;
            work->pos5C.vy = -0x1F40;
            work->emit0[2] = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, FX_ULTIMA_PINK_SPIKES_2), (s32)&work->pos5C);
            work->emit0[3] = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, FX_ULTIMA_RED_BURST), (s32)&work->pos4C);
            if (work->emit0[2] != NULL) {
                work->emit0[2]->otZ = 0x500;
            }
            break;
        case FX_ULTIMA_ROCK_SWIRL_FRAME:
            work->emit0[4] = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, FX_ULTIMA_ROCK_SWIRL_2), (s32)&work->pos4C);
            work->emit0[5] = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, FX_ULTIMA_ROCK_SWIRL), (s32)&work->pos4C);
            work->emit0[7] = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, FX_ULTIMA_VIOLET_GLOW_2), (s32)&work->pos4C);
            if (work->emit0[4] != NULL) {
                work->emit0[4]->trails = (s32)fx_1ec_op82Trails0;
                work->emit0[4]->trailSlots = FX_ULTIMA_ROCK_SWIRL_2_TRAIL_SLOTS;
                work->emit0[4]->trailStride = FX_ULTIMA_ROCK_SWIRL_2_TRAIL_STRIDE;
            }
            if (work->emit0[5] != NULL) {
                work->emit0[5]->trails = (s32)fx_1ec_op82Trails1;
                work->emit0[5]->trailSlots = FX_ULTIMA_ROCK_SWIRL_TRAIL_SLOTS;
                work->emit0[5]->trailStride = FX_ULTIMA_ROCK_SWIRL_TRAIL_STRIDE;
            }
            if (work->emit0[7] != NULL) {
                work->emit0[7]->otZ = 0x200;
            }
            break;
        case FX_ULTIMA_WHITE_OUT_FRAME:
            work->timer68 = FX_ULTIMA_WHITE_OUT_FRAMES;
            for (i = 0; i < env->targets->targetCount; i++) {
                fxm_startHeavyHitMotionIfActive(i);
            }
            break;
        case FX_ULTIMA_DARKEN_FRAME:
            work->timer64 = FX_ULTIMA_DARKEN_FRAMES;
            break;
        }
        elapsed = frame - FX_ULTIMA_ROCK_SWIRL_FRAME;
        if ((u32)elapsed < FX_ULTIMA_MORPH_FRAMES) {
            if (elapsed < 0) {
                morph = 0x1000;
            } else if (elapsed >= FX_ULTIMA_MORPH_FRAMES) {
                morph = 0;
            } else {
                morph = 0x1000 - (elapsed << 12) / FX_ULTIMA_MORPH_FRAMES;
            }
            ex->Hi_DrawMorphEffModel(&work->rot20, &work->pos28, (s32)&work->scale38, work->model6C, work->model6E, morph);
            if (work->timer64 >= 0) {
                work->rgb48 = fxm_lerpRatio(0, -0xFF, FX_ULTIMA_DARKEN_FRAMES - work->timer64, FX_ULTIMA_DARKEN_FRAMES);
                work->timer64--;
                ex->Hi_ModifyEffModelRGB(work->model6C, work->rgb48, work->rgb48, work->rgb48);
            }
        }
        if (work->timer68 >= 0) {
            fxm_tintScreenLerp(0, 0xFF, FX_ULTIMA_WHITE_OUT_FRAMES - work->timer68, FX_ULTIMA_WHITE_OUT_FRAMES, BTLFX_ABR_ADD, 0);
            work->timer68--;
            if (work->timer68 == -1) {
                work->flag6A = 1;
                ex->btlfx_signalScriptCue();
            }
        } else if (work->flag6A != 0) {
            ex->btlfx_drawScreenTint((u32 *)env->ot, BTLFX_ABR_ADD, FX_ULTIMA_WHITE_RGB);
        }
        if (frame >= FX_ULTIMA_WHITE_OUT_FRAME) {
            for (i = 0; i < env->targets->targetCount; i++) {
                if (fxm_isMotionDone(i) != 0) {
                    fxm_startUnitMotion(i, BTL_MOTION_KO);
                    ex->btlfx_holdMotion(i);
                }
            }
        }
        if (frame >= FX_ULTIMA_OP82_FRAMES) {
            ex->btlfx_clearEffModels();
            return 1;
        }
    }
    return 0;
}

static u16 *fxm_offsetTableEntry(u8 *base, u16 idx) {
    return (u16 *)((s32)base + ((s32 *)base)[(u16)idx + 1]);
}

static void fxm_lerpSvec(SVECTOR *from, SVECTOR *to, s32 t, SVECTOR *out) {
    BTLFX_EXPORTS(fx_1ec_env)->LoadAverageShort12(from, to, 0x1000 - t, t, out);
}

static s32 fxm_lerpRatio(s32 from, s32 to, s32 t, s32 n) {
    return (from * (n - t) + to * t) / n;
}

static void fxm_tintScreenLerp(s32 from, s32 to, s32 t, s32 n, s32 abr, s32 ot) {
    s32 bright = (from * (n - t) + to * t) / n;

    BTLFX_EXPORTS(fx_1ec_env)->btlfx_drawScreenTint((u32 *)(fx_1ec_env->ot + ot), abr, bright, bright, bright);
}

static void fxm_posPathInit(Fx1ecPath *path, u16 *data) {
    data++;
    path->count2 = *data++;
    path->keys4 = (Fx1ecPoint *)data;
}

static s32 fxm_posPathGet(Fx1ecPath *path, s32 t, SVECTOR *out) {
    s32 last = path->count2 - 1;
    s32 done = 1;
    Fx1ecPoint *key;

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

static void fxm_startUnitMotion(s32 unit, s32 motion) {
    BtlfxExports *ex = BTLFX_EXPORTS(fx_1ec_env);

    ex->btlfx_setUnitMotion(unit, ex->btlfx_getUnitMotion(unit, motion));
    ex->btlfx_setMotionFrame(unit, 0);
}

static s32 fxm_isMotionDone(s32 unit) {
    BtlfxExports *ex = BTLFX_EXPORTS(fx_1ec_env);

    s32 done;

    if (ex->btlfx_isUnitActive(unit)) {
        done = ex->btlfx_getMotionFrame(unit) == ex->btlfx_getMotionLength(unit) - 1;
    } else {
        done = 0;
    }
    return done;
}

static void fxm_startHitMotionIfActive(s32 unit) {
    if (BTLFX_EXPORTS(fx_1ec_env)->btlfx_isUnitActive(unit)) {
        fxm_startUnitMotion(unit, BTL_MOTION_HIT);
    }
}

static void fxm_startHeavyHitMotionIfActive(s32 unit) {
    if (BTLFX_EXPORTS(fx_1ec_env)->btlfx_isUnitActive(unit)) {
        fxm_startUnitMotion(unit, BTL_MOTION_HIT_HEAVY);
    }
}
