#include "fxmod/fx_03d.h"

s32 fx_03d_op80(s32 mode, Fx03dWork80 *work, Unk800F78A4 *env, Unk80100C78Entry *entry, s32 *args) {
    BtlfxExports *ex = BTLFX_EXPORTS(env);
    s32 frame;

    if (mode == 0) {
        *args = sizeof(Fx03dWork80);
    } else if (mode == 1) {
        fx_03d_env = env;
        ex->btlfx_getUnitBodyPos(BTLFX_UNIT_CASTER, 0, (s16 *)&work->pos8);
        ex->btlfx_getUnitRot(BTLFX_UNIT_CASTER, (s16 *)&work->rot10);
        work->emit0 = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, FX_LUNA_LIGHT_STREAK), (s32)&work->pos8);
        work->emit4 = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, FX_LUNA_SPARKLE_CIRCLE), (s32)&work->pos8);
        if (work->emit0 != NULL) {
            work->emit0->otZ = ex->btlfx_addRadiusSixth(BTLFX_UNIT_CASTER, 0x80);
        }
        if (work->emit4 != NULL) {
            work->emit4->otZ = ex->btlfx_addRadiusSixth(BTLFX_UNIT_CASTER, 0x80);
        }
    } else {
        frame = entry->frame;
        if (work->emit0 != NULL) {
            if (work->emit0->state == BTLFX_EMIT_DONE) {
                work->emit0 = NULL;
            }
            if (work->emit0 != NULL) {
                work->emit0->rot.vx = work->rot10.vx;
                work->emit0->rot.vy = work->rot10.vy;
                work->emit0->rot.vz = work->rot10.vz;
            }
        }
        if (work->emit4 != NULL) {
            if (work->emit4->state == BTLFX_EMIT_DONE) {
                work->emit4 = NULL;
            }
            if (work->emit4 != NULL) {
                work->emit4->rot.vx = work->rot10.vx;
                work->emit4->rot.vy = work->rot10.vy;
                work->emit4->rot.vz = work->rot10.vz;
            }
        }
        return frame >= FX_LUNA_FRAMES;
    }
    return 0;
}

s32 fx_03d_op81(s32 mode, Fx03dWork81 *work, Unk800F78A4 *env, Unk80100C78Entry *entry, s32 *args) {
    BtlfxExports *ex = BTLFX_EXPORTS(env);
    s32 frame;

    if (mode == 0) {
        *args = sizeof(Fx03dWork81);
    } else if (mode == 1) {
        work->pos18.vx = 0;
        work->pos18.vy = 0;
        work->pos18.vz = 0;
        fx_03d_env = env;
        work->emit0 = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, FX_LUNA_RED_TRAIL), (s32)&work->pos18);
        if (work->emit0 != NULL) {
            work->emit0->trails = (s32)fx_03d_trails;
            work->emit0->trailSlots = FX_LUNA_RED_TRAIL_TRAIL_SLOTS;
            work->emit0->trailStride = FX_LUNA_RED_TRAIL_TRAIL_STRIDE;
        }
    } else {
        frame = entry->frame;
        if (frame == FX_LUNA_GOLD_RINGS_FRAME) {
            work->emit4 = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, FX_LUNA_GOLD_RINGS), (s32)&work->pos18);
        }
        if (frame == FX_LUNA_CONES_FRAME) {
            work->emit8 = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, FX_LUNA_MAGENTA_RING), (s32)&work->pos18);
            work->emitC = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, FX_LUNA_LIGHT_CONES), (s32)&work->pos18);
        }
        return frame >= FX_LUNA_RINGS_FRAMES;
    }
    return 0;
}

s32 fx_03d_op82(s32 mode, Fx03dWork82 *work, Unk800F78A4 *env, Unk80100C78Entry *entry, s32 *args) {
    BtlfxExports *ex = BTLFX_EXPORTS(env);
    s32 frame;
    s32 scale;

    if (mode == 0) {
        *args = sizeof(Fx03dWork82);
    } else if (mode == 1) {
        fx_03d_env = env;
        ex->btlfx_initEffModels(fx_03d_effModels, FX_LUNA_MODEL_COUNT);
        work->pos8.vx = 0;
        work->pos8.vy = 0;
        work->pos8.vz = 0;
        work->rot0.vx = 0;
        work->rot0.vy = 0;
        work->rot0.vz = 0;
        work->scale18.vx = 0x1000;
        work->scale18.vy = 0x1000;
        work->scale18.vz = 0x1000;
        work->scale28.vx = 0x1000;
        work->scale28.vy = 0x1000;
        work->scale28.vz = 0x1000;
        work->model40 = ex->btlfx_registerEffectModel((Unk800E8F50 *)ex->btlfx_getFileData(entry->file, FX_LUNA_FLAME_BOWL), 1);
        work->model42 = ex->btlfx_registerEffectModel((Unk800E8F50 *)ex->btlfx_getFileData(entry->file, FX_LUNA_FLAME_RING), 1);
        work->fadeIn3C = -1;
        work->fadeOut3E = -1;
    } else {
        frame = entry->frame;
        switch (frame) {
        case 0:
            work->fadeIn3C = FX_LUNA_FADE_FRAMES;
            break;
        case FX_LUNA_FADE_OUT_FRAME:
            work->fadeOut3E = FX_LUNA_FADE_FRAMES;
            break;
        }
        if (frame >= 0 && frame < FX_LUNA_GROW_FRAMES + FX_LUNA_SPREAD_FRAMES) {
            if (frame >= 0 && frame < FX_LUNA_GROW_FRAMES) {
                scale = fxm_lerpRatio(0, 0x1800, frame, FX_LUNA_GROW_FRAMES);
                work->scale18.vx = scale;
                work->scale18.vy = scale;
                work->scale18.vz = scale;
                work->scale28.vx = scale;
                work->scale28.vy = scale;
                work->scale28.vz = scale;
            }
            if (frame >= FX_LUNA_GROW_FRAMES && frame < FX_LUNA_GROW_FRAMES + FX_LUNA_SPREAD_FRAMES) {
                scale = fxm_lerpRatio(0x1800, 0x2000, frame - FX_LUNA_GROW_FRAMES, FX_LUNA_SPREAD_FRAMES);
                work->scale18.vx = scale;
                work->scale18.vz = scale;
                scale = fxm_lerpRatio(0x1800, 0x2800, frame - FX_LUNA_GROW_FRAMES, FX_LUNA_SPREAD_FRAMES);
                work->scale28.vx = scale;
                work->scale28.vz = scale;
            }
            ex->Hi_DrawEffModel(&work->rot0, &work->pos8, (s32)&work->scale18, work->model40);
            ex->Hi_DrawEffModel(&work->rot0, &work->pos8, (s32)&work->scale28, work->model42);
            if (work->fadeIn3C >= 0 || work->fadeOut3E >= 0) {
                if (work->fadeIn3C >= 0) {
                    work->rgb38 = fxm_lerpRatio(-0x80, 0, FX_LUNA_FADE_FRAMES - work->fadeIn3C, FX_LUNA_FADE_FRAMES);
                    work->fadeIn3C--;
                }
                if (work->fadeOut3E >= 0) {
                    work->rgb38 = fxm_lerpRatio(0, -0x80, FX_LUNA_FADE_FRAMES - work->fadeOut3E, FX_LUNA_FADE_FRAMES);
                    work->fadeOut3E--;
                }
                ex->Hi_ModifyEffModelRGB(work->model40, work->rgb38, work->rgb38, work->rgb38);
                ex->Hi_ModifyEffModelRGB(work->model42, work->rgb38, work->rgb38, work->rgb38);
            }
        }
        if (frame >= FX_LUNA_FLAMES_FRAMES) {
            ex->btlfx_clearEffModels();
            return 1;
        }
    }
    return 0;
}

static void fxm_lerpSvec(SVECTOR *from, SVECTOR *to, s32 t, SVECTOR *out) {
    BTLFX_EXPORTS(fx_03d_env)->LoadAverageShort12(from, to, 0x1000 - t, t, out);
}

static s32 fxm_lerpRatio(s32 from, s32 to, s32 t, s32 n) {
    return (from * (n - t) + to * t) / n;
}

INCLUDE_RODATA("asm/us/fx_03d/nonmatchings/fx_03d", fx_03d_headerPtr);
