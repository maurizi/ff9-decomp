#include "fxmod/fx_0cd.h"

s32 fx_0cd_op80(s32 mode, Fx0cdWork *work, Unk800F78A4 *env, Unk80100C78Entry *entry, s32 *args) {
    BtlfxExports *ex = BTLFX_EXPORTS(env);
    Pool6CEntry *emitter;
    s32 frame;
    s32 i;
    s32 *word;
    s32 s;
    s32 rgb;

    if (mode == 0) {
        *args = sizeof(Fx0cdWork);
    } else if (mode == 1) {
        fx_0cd_env = env;
        for (i = 7, word = &((s32 *)work)[7]; i >= 0; i--, word--) {
            *word = 0;
        }
        ex->btlfx_initEffModels(fx_0cd_effModels, FX_AERA_MODEL_COUNT);
        ex->btlfx_getUnitPos(0, (s16 *)&work->posA4);
        ex->btlfx_getUnitBodyPos(0, 0, (s16 *)&work->posAC);
        work->model20.model0 = ex->btlfx_registerEffectModel((Unk800E8F50 *)ex->btlfx_getFileData(entry->file, FX_AERA_GREEN_TORNADO), 1);
        work->model4C.model0 = ex->btlfx_registerEffectModel((Unk800E8F50 *)ex->btlfx_getFileData(entry->file, FX_AERA_GREY_ARC), 1);
        work->model78.model0 = ex->btlfx_registerEffectModel((Unk800E8F50 *)ex->btlfx_getFileData(entry->file, FX_AERA_GREEN_WIND_RING), 1);
        work->model78.rot4.vx = 0;
        work->model78.rot4.vy = 0;
        work->model78.rot4.vz = 0;
        work->model78.scale1C.vx = 0x1000;
        work->model78.scale1C.vy = 0x1000;
        work->model78.scale1C.vz = 0x1000;
        work->model4C.rot4.vx = 0;
        work->model4C.rot4.vy = 0;
        work->model4C.rot4.vz = 0;
        work->model4C.scale1C.vx = 0x1000;
        work->model4C.scale1C.vy = 0x1000;
        work->model4C.scale1C.vz = 0x1000;
        work->model20.rot4.vx = 0;
        work->model20.rot4.vy = 0;
        work->model20.rot4.vz = 0;
        work->model78.posC.vx = work->posA4.vx;
        work->model78.posC.vy = work->posA4.vy;
        work->model78.posC.vz = work->posA4.vz;
        work->model4C.posC.vx = work->posAC.vx;
        work->model4C.posC.vy = work->posAC.vy;
        work->model4C.posC.vz = work->posAC.vz;
        work->model20.posC.vx = work->posA4.vx;
        work->model20.posC.vy = work->posA4.vy;
        work->model20.posC.vz = work->posA4.vz;
        work->model20.scale1C.vx = 0x1000;
        work->model20.scale1C.vy = 0x1000;
        work->model20.scale1C.vz = 0x1000;
    } else {
        frame = entry->frame;
        switch (frame) {
        case 0x0:
            emitter = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, FX_AERA_GREEN_WIND_BLADES), (s32)&work->posA4);
            if (emitter != NULL) {
                emitter->trails = (s32)fx_0cd_trails;
                emitter->trailSlots = FX_AERA_GREEN_WIND_BLADES_TRAIL_SLOTS;
                emitter->trailStride = FX_AERA_GREEN_WIND_BLADES_TRAIL_STRIDE;
            }
            break;
        case FX_AERA_BLADES_2_FRAME:
            emitter = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, FX_AERA_GREEN_WIND_BLADES), (s32)&work->posA4);
            if (emitter != NULL) {
                emitter->trails = (s32)fx_0cd_trails2;
                emitter->trailSlots = FX_AERA_GREEN_WIND_BLADES_TRAIL_SLOTS;
                emitter->trailStride = FX_AERA_GREEN_WIND_BLADES_TRAIL_STRIDE;
            }
            break;
        case FX_AERA_WIND_RING_FRAME:
            emitter = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, FX_AERA_WIND_RING), (s32)&work->posA4);
            if (emitter != NULL) {
                emitter->otZ = ex->btlfx_addRadiusSixth(0, -0x200);
            }
            work->timer0 = FX_AERA_WIND_RING_FRAMES;
            break;
        case FX_AERA_TORNADO_FRAME:
            emitter = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, FX_AERA_GREEN_BURST), (s32)&work->posA4);
            if (emitter != NULL) {
                emitter->otZ = ex->btlfx_addRadiusSixth(0, -0x200);
            }
            work->timer4 = FX_AERA_TORNADO_FRAMES;
            break;
        case FX_AERA_ARC_FRAME:
            work->timer8 = FX_AERA_ARC_FRAMES;
            break;
        case FX_AERA_GUST_FRAME:
            ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, FX_AERA_GUST_RINGS), (s32)&work->posA4);
            ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, FX_AERA_GREEN_RING), (s32)&work->posA4);
            break;
        case FX_AERA_SPRITE_BURST_FRAME:
            ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, FX_AERA_SPRITE_BURST), (s32)&work->posA4);
            break;
        }
        if (work->timer0 != 0) {
            work->timer0--;
            work->model78.rot4.vy += 0x80;
            work->model78.scale1C.vx = work->model78.scale1C.vz = ex->btlfx_lerpRatio(0x2000, 0x1000, work->timer0, FX_AERA_WIND_RING_FRAMES);
            work->model78.scale1C.vy = ex->btlfx_lerpRatio(0, 0x4000, work->timer0, FX_AERA_WIND_RING_FRAMES);
            ex->Hi_DrawEffModel(&work->model78.rot4, &work->model78.posC, (s32)&work->model78.scale1C, work->model78.model0);
            if (work->timer0 > FX_AERA_WIND_RING_FRAMES - FX_AERA_MODEL_FADE_FRAMES) {
                rgb = -((work->timer0 - (FX_AERA_WIND_RING_FRAMES - FX_AERA_MODEL_FADE_FRAMES)) << 5);
                ex->Hi_ModifyEffModelRGB(work->model78.model0, rgb, rgb, rgb);
            }
            if (work->timer0 < FX_AERA_MODEL_FADE_FRAMES) {
                rgb = (work->timer0 << 5) - 0x100;
                ex->Hi_ModifyEffModelRGB(work->model78.model0, rgb, rgb, rgb);
            }
        }
        if (work->timer4 != 0) {
            work->timer4--;
            work->model20.rot4.vy += 0x80;
            work->model20.scale1C.vx = work->model20.scale1C.vz = ex->btlfx_lerpRatio(0, 0x2000, work->timer4, FX_AERA_TORNADO_FRAMES);
            ex->Hi_DrawEffModel(&work->model20.rot4, &work->model20.posC, (s32)&work->model20.scale1C, work->model20.model0);
            if (work->timer4 > FX_AERA_TORNADO_FRAMES - FX_AERA_MODEL_FADE_FRAMES) {
                rgb = -((work->timer4 - (FX_AERA_TORNADO_FRAMES - FX_AERA_MODEL_FADE_FRAMES)) << 5);
                ex->Hi_ModifyEffModelRGB(work->model20.model0, rgb, rgb, rgb);
            }
            if (work->timer4 < FX_AERA_MODEL_FADE_FRAMES) {
                rgb = work->timer4 * 16 - 0x80;
                ex->Hi_ModifyEffModelRGB(work->model20.model0, rgb, rgb, rgb);
            }
        }
        if (work->timer8 != 0) {
            work->timer8--;
            work->model4C.rot4.vy += 0x80;
            work->model4C.scale1C.vx = work->model4C.scale1C.vz = ex->btlfx_lerpRatio(0x4000, 0, work->timer8, FX_AERA_ARC_FRAMES);
            ex->Hi_DrawEffModel(&work->model4C.rot4, &work->model4C.posC, (s32)&work->model4C.scale1C, work->model4C.model0);
            rgb = -0x40;
            if (work->timer8 > FX_AERA_ARC_FRAMES - FX_AERA_MODEL_FADE_FRAMES) {
                rgb = -((work->timer8 - (FX_AERA_ARC_FRAMES - FX_AERA_MODEL_FADE_FRAMES)) * 8) - 0x40;
            }
            if (work->timer8 < FX_AERA_MODEL_FADE_FRAMES) {
                rgb = work->timer8 * 8 - 0x80;
            }
            ex->Hi_ModifyEffModelRGB(work->model4C.model0, rgb, rgb, rgb);
        }
        return frame >= FX_AERA_FRAMES;
    }
    return 0;
}

INCLUDE_RODATA("asm/us/fx_0cd/nonmatchings/fx_0cd", fx_0cd_headerPtr);
