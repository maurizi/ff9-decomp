#include "fxmod/fx_1f6.h"

s32 fx_1f6_op80(s32 mode, Fx1f6Work *work, Unk800F78A4 *env, Unk80100C78Entry *entry, s32 *args) {
    BtlfxExports *ex = BTLFX_EXPORTS(env);
    Pool6CEntry *emitter;
    s32 frame;
    s32 i;
    s32 shade;

    if (mode == 0) {
        *args = sizeof(Fx1f6Work);
    } else if (mode == 1) {
        fx_1f6_env = env;
        for (i = FX_AERA_TIMERS - 1; i >= 0; i--) {
            work->timer0[i] = 0;
        }
        ex->btlfx_initEffModels(fx_1f6_effModels, FX_AERA_MODEL_COUNT);
        ex->btlfx_getUnitPos(0, (s16 *)&work->posA4);
        ex->btlfx_getUnitBodyPos(0, 0, (s16 *)&work->posAC);
        work->models20[0].model0 = ex->btlfx_registerEffectModel((Unk800E8F50 *)ex->btlfx_getFileData(entry->file, FX_AERA_GREEN_TORNADO), 1);
        work->models20[1].model0 = ex->btlfx_registerEffectModel((Unk800E8F50 *)ex->btlfx_getFileData(entry->file, FX_AERA_GREY_ARC), 1);
        work->models20[2].model0 = ex->btlfx_registerEffectModel((Unk800E8F50 *)ex->btlfx_getFileData(entry->file, FX_AERA_GREEN_WIND_RING), 1);
        work->models20[2].rot4.vx = 0;
        work->models20[2].rot4.vy = 0;
        work->models20[2].rot4.vz = 0;
        work->models20[2].posC.vx = work->posA4.vx;
        work->models20[2].posC.vy = work->posA4.vy;
        work->models20[2].posC.vz = work->posA4.vz;
        work->models20[2].scale1C.vx = 0x1000;
        work->models20[2].scale1C.vy = 0x1000;
        work->models20[2].scale1C.vz = 0x1000;
        work->models20[1].rot4.vx = 0;
        work->models20[1].rot4.vy = 0;
        work->models20[1].rot4.vz = 0;
        work->models20[1].posC.vx = work->posAC.vx;
        work->models20[1].posC.vy = work->posAC.vy;
        work->models20[1].posC.vz = work->posAC.vz;
        work->models20[1].scale1C.vx = 0x1000;
        work->models20[1].scale1C.vy = 0x1000;
        work->models20[1].scale1C.vz = 0x1000;
        work->models20[0].rot4.vx = 0;
        work->models20[0].rot4.vy = 0;
        work->models20[0].rot4.vz = 0;
        work->models20[0].posC.vx = work->posA4.vx;
        work->models20[0].posC.vy = work->posA4.vy;
        work->models20[0].posC.vz = work->posA4.vz;
        work->models20[0].scale1C.vx = 0x1000;
        work->models20[0].scale1C.vy = 0x1000;
        work->models20[0].scale1C.vz = 0x1000;
    } else {
        frame = entry->frame;
        switch (frame) {
        case 0:
            emitter = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, FX_AERA_GREEN_WIND_BLADES), (s32)&work->posA4);
            if (emitter != NULL) {
                emitter->trails = (s32)fx_1f6_op80Trails0;
                emitter->trailSlots = FX_AERA_GREEN_WIND_BLADES_TRAIL_SLOTS;
                emitter->trailStride = FX_AERA_GREEN_WIND_BLADES_TRAIL_STRIDE;
            }
            break;
        case FX_AERA_BLADES_2_FRAME:
            emitter = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, FX_AERA_GREEN_WIND_BLADES), (s32)&work->posA4);
            if (emitter != NULL) {
                emitter->trails = (s32)fx_1f6_op80Trails1;
                emitter->trailSlots = FX_AERA_GREEN_WIND_BLADES_TRAIL_SLOTS;
                emitter->trailStride = FX_AERA_GREEN_WIND_BLADES_TRAIL_STRIDE;
            }
            break;
        case FX_AERA_WIND_RING_FRAME:
            emitter = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, FX_AERA_WIND_RING), (s32)&work->posA4);
            if (emitter != NULL) {
                emitter->otZ = ex->btlfx_addRadiusSixth(0, -0x200);
            }
            work->timer0[FX_AERA_T_WIND_RING] = FX_AERA_WIND_RING_FRAMES;
            break;
        case FX_AERA_TORNADO_FRAME:
            emitter = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, FX_AERA_GREEN_BURST), (s32)&work->posA4);
            if (emitter != NULL) {
                emitter->otZ = ex->btlfx_addRadiusSixth(0, -0x200);
            }
            work->timer0[FX_AERA_T_TORNADO] = FX_AERA_TORNADO_FRAMES;
            break;
        case FX_AERA_ARC_FRAME:
            work->timer0[FX_AERA_T_ARC] = FX_AERA_ARC_FRAMES;
            break;
        case FX_AERA_GUST_FRAME:
            ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, FX_AERA_GUST_RINGS), (s32)&work->posA4);
            ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, FX_AERA_GREEN_RING), (s32)&work->posA4);
            break;
        case FX_AERA_SPRITE_BURST_FRAME:
            ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, FX_AERA_SPRITE_BURST), (s32)&work->posA4);
            break;
        }
        if (work->timer0[FX_AERA_T_WIND_RING] != 0) {
            work->timer0[FX_AERA_T_WIND_RING]--;
            work->models20[2].rot4.vy += 0x80;
            work->models20[2].scale1C.vx = work->models20[2].scale1C.vz = ex->btlfx_lerpRatio(0x2000, 0x1000, work->timer0[FX_AERA_T_WIND_RING], FX_AERA_WIND_RING_FRAMES);
            work->models20[2].scale1C.vy = ex->btlfx_lerpRatio(0, 0x4000, work->timer0[FX_AERA_T_WIND_RING], FX_AERA_WIND_RING_FRAMES);
            ex->Hi_DrawEffModel(&work->models20[2].rot4, &work->models20[2].posC, (s32)&work->models20[2].scale1C,
                                work->models20[2].model0);
            if (work->timer0[FX_AERA_T_WIND_RING] >= FX_AERA_WIND_RING_FRAMES - FX_AERA_MODEL_FADE_FRAMES + 1) {
                shade = -((work->timer0[FX_AERA_T_WIND_RING] - (FX_AERA_WIND_RING_FRAMES - FX_AERA_MODEL_FADE_FRAMES)) << 5);
                ex->Hi_ModifyEffModelRGB(work->models20[2].model0, shade, shade, shade);
            }
            if (work->timer0[FX_AERA_T_WIND_RING] < FX_AERA_MODEL_FADE_FRAMES) {
                shade = (work->timer0[FX_AERA_T_WIND_RING] << 5) - 0x100;
                ex->Hi_ModifyEffModelRGB(work->models20[2].model0, shade, shade, shade);
            }
        }
        if (work->timer0[FX_AERA_T_TORNADO] != 0) {
            work->timer0[FX_AERA_T_TORNADO]--;
            work->models20[0].rot4.vy += 0x80;
            work->models20[0].scale1C.vx = work->models20[0].scale1C.vz = ex->btlfx_lerpRatio(0, 0x2000, work->timer0[FX_AERA_T_TORNADO], FX_AERA_TORNADO_FRAMES);
            ex->Hi_DrawEffModel(&work->models20[0].rot4, &work->models20[0].posC, (s32)&work->models20[0].scale1C,
                                work->models20[0].model0);
            if (work->timer0[FX_AERA_T_TORNADO] >= FX_AERA_TORNADO_FRAMES - FX_AERA_MODEL_FADE_FRAMES + 1) {
                shade = -((work->timer0[FX_AERA_T_TORNADO] - (FX_AERA_TORNADO_FRAMES - FX_AERA_MODEL_FADE_FRAMES)) << 5);
                ex->Hi_ModifyEffModelRGB(work->models20[0].model0, shade, shade, shade);
            }
            if (work->timer0[FX_AERA_T_TORNADO] < FX_AERA_MODEL_FADE_FRAMES) {
                shade = work->timer0[FX_AERA_T_TORNADO] * 0x10 - 0x80;
                ex->Hi_ModifyEffModelRGB(work->models20[0].model0, shade, shade, shade);
            }
        }
        if (work->timer0[FX_AERA_T_ARC] != 0) {
            work->timer0[FX_AERA_T_ARC]--;
            work->models20[1].rot4.vy += 0x80;
            work->models20[1].scale1C.vx = work->models20[1].scale1C.vz = ex->btlfx_lerpRatio(0x4000, 0, work->timer0[FX_AERA_T_ARC], FX_AERA_ARC_FRAMES);
            ex->Hi_DrawEffModel(&work->models20[1].rot4, &work->models20[1].posC, (s32)&work->models20[1].scale1C,
                                work->models20[1].model0);
            shade = -0x40;
            if (work->timer0[FX_AERA_T_ARC] >= FX_AERA_ARC_FRAMES - FX_AERA_MODEL_FADE_FRAMES + 1) {
                shade = -((work->timer0[FX_AERA_T_ARC] - (FX_AERA_ARC_FRAMES - FX_AERA_MODEL_FADE_FRAMES)) * 8) - 0x40;
            }
            if (work->timer0[FX_AERA_T_ARC] < FX_AERA_MODEL_FADE_FRAMES) {
                shade = work->timer0[FX_AERA_T_ARC] * 8 - 0x80;
            }
            ex->Hi_ModifyEffModelRGB(work->models20[1].model0, shade, shade, shade);
        }
        return frame >= FX_AERA_FRAMES;
    }
    return 0;
}

INCLUDE_RODATA("asm/us/fx_1f6/nonmatchings/fx_1f6", fx_1f6_headerPtr);
