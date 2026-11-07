#include "fxmod/fx_1b2.h"

s32 fx_1b2_op80(s32 mode, Fx1b2Work80 *work, Unk800F78A4 *env, Unk80100C78Entry *entry, s32 *args) {
    BtlfxExports *ex = BTLFX_EXPORTS(env);
    s32 frame;
    s32 i;
    s32 pathFrame;

    if (mode == 0) {
        *args = sizeof(Fx1b2Work80);
    } else if (mode == 1) {
        work->pos18.vx = 0;
        work->pos18.vy = 0;
        work->pos18.vz = -FX_TIDAL_WAVE_MODEL_Z;
        work->rot10.vx = 0;
        work->rot10.vy = 0;
        work->rot10.vz = 0;
        work->scale28.vx = 0x1000;
        work->scale28.vy = 0x1000;
        work->scale28.vz = 0x1000;
        work->pos54.vx = work->pos18.vx;
        work->pos54.vy = work->pos18.vy;
        work->pos54.vz = work->pos18.vz;
        fx_1b2_env = env;
        fxm_vecPathInit(&fx_1b2_vecPath, (u8 *)ex->btlfx_getFileData(entry->file, FX_TIDAL_WAVE_PATH));
        work->model6C = ex->Hi_RegisterTexEffModel((BtlModelHeader *)ex->btlfx_getFileData(entry->file, FX_TIDAL_WAVE_WATER_CURL), FX_TIDAL_WAVE_CURL_TPAGE, FX_TIDAL_WAVE_CURL_CLUT, FX_TIDAL_WAVE_CURL_U, FX_TIDAL_WAVE_CURL_V);
        work->rot3C.vx = -0x200;
        work->rot3C.vy = 0;
        work->rot3C.vz = 0;
        work->count64 = 0;
        work->index68 = 0;
        work->timer5C = 1;
        work->timer60 = -1;
        work->emit4 = NULL;
        work->emit8 = NULL;
    } else {
        frame = entry->frame;
        if (!(frame & (FX_TIDAL_WAVE_INTERVAL - 1))) {
            work->timer5C = FX_TIDAL_WAVE_CURL_FRAMES;
            work->emit0 = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, FX_TIDAL_WAVE_WATER_ORBS), (s32)&work->pos54);
            work->emitC = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, FX_TIDAL_WAVE_PALE_RINGS), (s32)&work->pos54);
            if (work->emit0 != NULL) {
                work->emit0->otZ = 0x200;
            }
            if (work->emitC != NULL) {
                work->emitC->otZ = 0x180;
            }
            for (i = 0; i < env->targets->targetCount; i++) {
                if (work->count64 != FX_TIDAL_WAVE_LAST_WAVE) {
                    fxm_startHitMotionIfActive(i);
                }
            }
            work->count64++;
            if (!(frame & (FX_TIDAL_WAVE_INTERVAL - 1))) {
                ex->btlfx_getUnitBonePos(BTLFX_UNIT_CASTER, FX_TIDAL_WAVE_TRAIL_BONE, (u16 *)&work->pos44);
                ex->btlfx_getUnitBonePos(BTLFX_UNIT_CASTER, FX_TIDAL_WAVE_TRAIL_2_BONE, (u16 *)&work->pos4C);
                work->emit4 = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, FX_TIDAL_WAVE_WATER_TRAIL), (s32)&work->pos44);
                work->emit8 = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, FX_TIDAL_WAVE_WATER_TRAIL), (s32)&work->pos4C);
                if (work->emit4 != NULL) {
                    work->emit4->trails = (s32)fx_1b2_op80Trails0[work->index68];
                    work->emit4->trailSlots = FX_TIDAL_WAVE_WATER_TRAIL_TRAIL_SLOTS;
                    work->emit4->trailStride = FX_TIDAL_WAVE_WATER_TRAIL_TRAIL_STRIDE;
                }
                if (work->emit8 != NULL) {
                    work->emit8->trails = (s32)fx_1b2_op80Trails1[work->index68];
                    work->emit8->trailSlots = FX_TIDAL_WAVE_WATER_TRAIL_TRAIL_SLOTS;
                    work->emit8->trailStride = FX_TIDAL_WAVE_WATER_TRAIL_TRAIL_STRIDE;
                }
                work->index68++;
            }
        }
        if (work->timer5C >= 0) {
            pathFrame = FX_TIDAL_WAVE_CURL_FRAMES - work->timer5C;
            if (pathFrame == FX_TIDAL_WAVE_CURL_FADE_FRAME) {
                work->timer60 = FX_TIDAL_WAVE_CURL_FADE_FRAMES;
            }
            if (work->timer60 >= 0) {
                work->fade38 = fxm_lerpRatio(0, -0x80, FX_TIDAL_WAVE_CURL_FADE_FRAMES - work->timer60, FX_TIDAL_WAVE_CURL_FADE_FRAMES);
                work->timer60--;
            }
            fxm_vecPathGet(&fx_1b2_vecPath, pathFrame, &work->scale28);
            ex->Hi_DrawEffModel(&work->rot10, &work->pos18, (s32)&work->scale28, work->model6C);
            ex->Hi_ModifyEffModelAbr(work->model6C, BTLFX_ABR_ADD);
            if (work->timer60 >= 0) {
                ex->Hi_ModifyEffModelRGB(work->model6C, work->fade38, work->fade38, work->fade38);
            }
            ex->btlfx_drawLayerTexture((BtlBgLayers *)ex->btlfx_getFileData(entry->file, FX_TIDAL_WAVE_BACKDROP), pathFrame);
            for (i = 0; i < env->targets->targetCount; i++) {
                if (fxm_isMotionDone(i) != 0) {
                    fxm_startUnitMotion(i, 0);
                }
            }
            work->timer5C--;
        }
        if (work->emit4 != NULL) {
            if (work->emit4->state == BTLFX_EMIT_DONE) {
                work->emit4 = NULL;
            }
            if (work->emit4 != NULL) {
                work->emit4->rot.vx = work->rot3C.vx;
                work->emit4->rot.vy = work->rot3C.vy;
                work->emit4->rot.vz = work->rot3C.vz;
            }
        }
        if (work->emit8 != NULL) {
            if (work->emit8->state == BTLFX_EMIT_DONE) {
                work->emit8 = NULL;
            }
            if (work->emit8 != NULL) {
                work->emit8->rot.vx = work->rot3C.vx;
                work->emit8->rot.vy = work->rot3C.vy;
                work->emit8->rot.vz = work->rot3C.vz;
            }
        }
        return frame >= FX_TIDAL_WAVE_FRAMES;
    }
    return 0;
}

s32 fx_1b2_op81(s32 mode, Fx1b2Work81 *work, Unk800F78A4 *env, Unk80100C78Entry *entry, s32 *args) {
    BtlfxExports *ex = BTLFX_EXPORTS(env);
    s32 frame;

    if (mode == 0) {
        *args = sizeof(Fx1b2Work81);
    } else if (mode == 1) {
        fx_1b2_env = env;
        ex->btlfx_initEffModels(fx_1b2_effModels, FX_TIDAL_WAVE_MODELS);
        ex->btlfx_getUnitPos(BTLFX_UNIT_CASTER, (s16 *)&work->posC);
    } else {
        frame = entry->frame;
        switch (frame) {
        case FX_TIDAL_WAVE_FOAM_FRAME:
            work->emit0 = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, FX_TIDAL_WAVE_FOAM), (s32)&work->posC);
            break;
        case FX_TIDAL_WAVE_RED_TRAIL_FRAME:
            ex->btlfx_getUnitBonePos(BTLFX_UNIT_CASTER, FX_TIDAL_WAVE_TRAIL_BONE, (u16 *)&work->pos14);
            ex->btlfx_getUnitBonePos(BTLFX_UNIT_CASTER, FX_TIDAL_WAVE_TRAIL_2_BONE, (u16 *)&work->pos1C);
            work->emit4 = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, FX_TIDAL_WAVE_RED_TRAIL), (s32)&work->pos14);
            work->emit8 = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, FX_TIDAL_WAVE_RED_TRAIL), (s32)&work->pos1C);
            if (work->emit4 != NULL) {
                work->emit4->trails = (s32)fx_1b2_op81Trails0;
                work->emit4->trailSlots = FX_TIDAL_WAVE_RED_TRAIL_TRAIL_SLOTS;
                work->emit4->trailStride = FX_TIDAL_WAVE_RED_TRAIL_TRAIL_STRIDE;
            }
            if (work->emit8 != NULL) {
                work->emit8->trails = (s32)fx_1b2_op81Trails1;
                work->emit8->trailSlots = FX_TIDAL_WAVE_RED_TRAIL_TRAIL_SLOTS;
                work->emit8->trailStride = FX_TIDAL_WAVE_RED_TRAIL_TRAIL_STRIDE;
            }
            break;
        }
        return frame >= FX_TIDAL_WAVE_TRAILS_FRAMES;
    }
    return 0;
}

s32 fx_1b2_op82(s32 mode, Fx1b2Work82 *work, Unk800F78A4 *env, Unk80100C78Entry *entry, s32 *args) {
    BtlfxExports *ex = BTLFX_EXPORTS(env);
    s32 frame;
    s32 bright;

    if (mode == 0) {
        *args = sizeof(Fx1b2Work82);
    } else if (mode == 1) {
        work->pos8.vz = -FX_TIDAL_WAVE_MODEL_Z;
        work->rot0.vx = 0;
        work->rot0.vy = 0;
        work->rot0.vz = 0;
        work->pos8.vx = 0;
        work->pos8.vy = 0;
        work->scale18.vx = 0x1000;
        work->scale18.vy = 0x1000;
        work->scale18.vz = 0x1000;
        fx_1b2_env = env;
        work->model34 = ex->Hi_RegisterTexEffModel((BtlModelHeader *)ex->btlfx_getFileData(entry->file, FX_TIDAL_WAVE_SURGE), FX_TIDAL_WAVE_SURGE_TPAGE, FX_TIDAL_WAVE_SURGE_CLUT, FX_TIDAL_WAVE_SURGE_U, FX_TIDAL_WAVE_SURGE_V);
        work->timer2C = -1;
        work->timer30 = -1;
    } else {
        frame = entry->frame;
        switch (frame) {
        case 0:
            work->timer2C = FX_TIDAL_WAVE_SURGE_FADE_IN_FRAMES;
            break;
        case FX_TIDAL_WAVE_SURGE_FADE_OUT_FRAME:
            work->timer30 = FX_TIDAL_WAVE_SURGE_FADE_OUT_FRAMES;
            break;
        }
        if (frame >= 0 && frame < FX_TIDAL_WAVE_SURGE_FADE_OUT_FRAME + FX_TIDAL_WAVE_SURGE_FADE_OUT_FRAMES) {
            if (work->timer2C >= 0) {
                work->fade28 = fxm_lerpRatio(-0x80, 0, FX_TIDAL_WAVE_SURGE_FADE_IN_FRAMES - work->timer2C, FX_TIDAL_WAVE_SURGE_FADE_IN_FRAMES);
                work->timer2C--;
            }
            if (work->timer30 >= 0) {
                work->fade28 = fxm_lerpRatio(0, -0x80, FX_TIDAL_WAVE_SURGE_FADE_OUT_FRAMES - work->timer30, FX_TIDAL_WAVE_SURGE_FADE_OUT_FRAMES);
                work->timer30--;
            }
            ex->Hi_DrawEffModel(&work->rot0, &work->pos8, (s32)&work->scale18, work->model34);
            ex->Hi_ModifyEffModelAbr(work->model34, BTLFX_ABR_ADD);
            if (work->timer2C > 0 || work->timer30 >= 0) {
                bright = work->fade28 + 6;
                ex->Hi_ModifyEffModelRGB(work->model34, bright, bright, bright);
            } else {
                ex->Hi_ModifyEffModelRGB(work->model34, 6, 6, 6);
            }
        }
        if (frame >= FX_TIDAL_WAVE_SURGE_FRAMES) {
            ex->btlfx_clearEffModels();
            return 1;
        }
    }
    return 0;
}

s32 fx_1b2_op83(s32 mode, void *work, Unk800F78A4 *env, Unk80100C78Entry *entry, s32 *args) {
    BtlfxExports *ex = BTLFX_EXPORTS(env);
    s32 frame;

    if (mode == 0) {
        *args = 0;
    } else if (mode == 1) {
        fx_1b2_env = env;
    } else {
        frame = entry->frame;
        if (frame == 0) {
            ex->btlfx_drawBgOffscreen(FX_TIDAL_WAVE_BG_VRAM_X, FX_TIDAL_WAVE_BG_VRAM_Y, FX_TIDAL_WAVE_BG_VRAM_W, FX_TIDAL_WAVE_BG_VRAM_H, fx_1b2_bgOt, 0, -0x3E8, 0x1770);
        }
        return frame >= FX_TIDAL_WAVE_BG_FRAMES;
    }
    return 0;
}

static void fxm_lerpSvec(SVECTOR *from, SVECTOR *to, s32 t, SVECTOR *out) {
    BTLFX_EXPORTS(fx_1b2_env)->LoadAverageShort12(from, to, 0x1000 - t, t, out);
}

static s32 fxm_lerpRatio(s32 from, s32 to, s32 t, s32 n) {
    return (from * (n - t) + to * t) / n;
}

static void fxm_vecPathInit(Fx1b2Path *path, u16 *data) {
    data++;
    path->count2 = *data++;
    path->keys4 = (Fx1b2Point *)data;
}

static s32 fxm_vecPathGet(Fx1b2Track6 *track, s32 t, VECTOR *out) {
    s32 last = track->count2 - 1;
    s32 done = 1;
    Fx1b2Key6 *key;

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

    BTLFX_EXPORTS(fx_1b2_env)->btlfx_drawScreenTint((u32 *)(fx_1b2_env->ot + ot), abr, bright, bright, bright);
}

static void fxm_startUnitMotion(s32 unit, s32 motion) {
    BtlfxExports *ex = BTLFX_EXPORTS(fx_1b2_env);

    ex->btlfx_setUnitMotion(unit, ex->btlfx_getUnitMotion(unit, motion));
    ex->btlfx_setMotionFrame(unit, 0);
}

static s32 fxm_isMotionDone(s32 unit) {
    BtlfxExports *ex = BTLFX_EXPORTS(fx_1b2_env);

    s32 done;

    if (ex->btlfx_isUnitActive(unit)) {
        done = ex->btlfx_getMotionFrame(unit) == ex->btlfx_getMotionLength(unit) - 1;
    } else {
        done = 0;
    }
    return done;
}

static void fxm_startHitMotionIfActive(s32 unit) {
    if (BTLFX_EXPORTS(fx_1b2_env)->btlfx_isUnitActive(unit)) {
        fxm_startUnitMotion(unit, BTL_MOTION_HIT);
    }
}

static void fxm_startHeavyHitMotionIfActive(s32 unit) {
    if (BTLFX_EXPORTS(fx_1b2_env)->btlfx_isUnitActive(unit)) {
        fxm_startUnitMotion(unit, BTL_MOTION_HIT_HEAVY);
    }
}

INCLUDE_RODATA("asm/us/fx_1b2/nonmatchings/fx_1b2", fx_1b2_headerPtr);
