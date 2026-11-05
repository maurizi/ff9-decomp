#include "fxmod/fx_1f6.h"

s32 fx_1f6_op80(s32 mode, Fx1f6Work *work, Unk800F78A4 *env, Unk80100C78Entry *entry, s32 *args) {
    BtlfxExports *ex = BTLFX_EXPORTS(env);
    Pool6CEntry *emitter;
    s32 frame;
    s32 i;
    s32 shade;

    if (mode == 0) {
        *args = 0xB4;
    } else if (mode == 1) {
        fx_1f6_env = env;
        for (i = 7; i >= 0; i--) {
            work->timer0[i] = 0;
        }
        ex->btlfx_initEffModels(fx_1f6_effModels, 3);
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
        case 0x0:
            emitter = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, FX_AERA_GREEN_WIND_BLADES), (s32)&work->posA4);
            if (emitter != NULL) {
                emitter->trails = (s32)fx_1f6_op80Trails0;
                emitter->trailSlots = 0x26;
                emitter->trailStride = 0x7C;
            }
            break;
        case 0x8:
            emitter = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, FX_AERA_GREEN_WIND_BLADES), (s32)&work->posA4);
            if (emitter != NULL) {
                emitter->trails = (s32)fx_1f6_op80Trails1;
                emitter->trailSlots = 0x26;
                emitter->trailStride = 0x7C;
            }
            break;
        case 0x10:
            emitter = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, FX_AERA_WIND_RING), (s32)&work->posA4);
            if (emitter != NULL) {
                emitter->otZ = ex->btlfx_addRadiusSixth(0, -0x200);
            }
            work->timer0[0] = 0x20;
            break;
        case 0x28:
            emitter = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, FX_AERA_GREEN_BURST), (s32)&work->posA4);
            if (emitter != NULL) {
                emitter->otZ = ex->btlfx_addRadiusSixth(0, -0x200);
            }
            work->timer0[1] = 0x30;
            break;
        case 0x38:
            work->timer0[2] = 0x18;
            break;
        case 0x50:
            ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, FX_AERA_GUST_RINGS), (s32)&work->posA4);
            ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, FX_AERA_GREEN_RING), (s32)&work->posA4);
            break;
        case 0x58:
            ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, FX_AERA_SPRITE_BURST), (s32)&work->posA4);
            break;
        }
        if (work->timer0[0] != 0) {
            work->timer0[0]--;
            work->models20[2].rot4.vy += 0x80;
            work->models20[2].scale1C.vx = work->models20[2].scale1C.vz = ex->btlfx_lerpRatio(0x2000, 0x1000, work->timer0[0], 0x20);
            work->models20[2].scale1C.vy = ex->btlfx_lerpRatio(0, 0x4000, work->timer0[0], 0x20);
            ex->Hi_DrawEffModel(&work->models20[2].rot4, &work->models20[2].posC, (s32)&work->models20[2].scale1C,
                                work->models20[2].model0);
            if (work->timer0[0] >= 0x19) {
                shade = -((work->timer0[0] - 0x18) << 5);
                ex->Hi_ModifyEffModelRGB(work->models20[2].model0, shade, shade, shade);
            }
            if (work->timer0[0] < 8) {
                shade = (work->timer0[0] << 5) - 0x100;
                ex->Hi_ModifyEffModelRGB(work->models20[2].model0, shade, shade, shade);
            }
        }
        if (work->timer0[1] != 0) {
            work->timer0[1]--;
            work->models20[0].rot4.vy += 0x80;
            work->models20[0].scale1C.vx = work->models20[0].scale1C.vz = ex->btlfx_lerpRatio(0, 0x2000, work->timer0[1], 0x30);
            ex->Hi_DrawEffModel(&work->models20[0].rot4, &work->models20[0].posC, (s32)&work->models20[0].scale1C,
                                work->models20[0].model0);
            if (work->timer0[1] >= 0x29) {
                shade = -((work->timer0[1] - 0x28) << 5);
                ex->Hi_ModifyEffModelRGB(work->models20[0].model0, shade, shade, shade);
            }
            if (work->timer0[1] < 8) {
                shade = work->timer0[1] * 0x10 - 0x80;
                ex->Hi_ModifyEffModelRGB(work->models20[0].model0, shade, shade, shade);
            }
        }
        if (work->timer0[2] != 0) {
            work->timer0[2]--;
            work->models20[1].rot4.vy += 0x80;
            work->models20[1].scale1C.vx = work->models20[1].scale1C.vz = ex->btlfx_lerpRatio(0x4000, 0, work->timer0[2], 0x18);
            ex->Hi_DrawEffModel(&work->models20[1].rot4, &work->models20[1].posC, (s32)&work->models20[1].scale1C,
                                work->models20[1].model0);
            shade = -0x40;
            if (work->timer0[2] >= 0x11) {
                shade = -((work->timer0[2] - 0x10) * 8) - 0x40;
            }
            if (work->timer0[2] < 8) {
                shade = work->timer0[2] * 8 - 0x80;
            }
            ex->Hi_ModifyEffModelRGB(work->models20[1].model0, shade, shade, shade);
        }
        return frame >= 0x80;
    }
    return 0;
}

INCLUDE_RODATA("asm/us/fx_1f6/nonmatchings/fx_1f6", fx_1f6_headerPtr);
