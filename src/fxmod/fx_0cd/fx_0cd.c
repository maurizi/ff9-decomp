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
        *args = 0xB4;
    } else if (mode == 1) {
        fx_0cd_env = env;
        for (i = 7, word = &((s32 *)work)[7]; i >= 0; i--, word--) {
            *word = 0;
        }
        ex->btlfx_initEffModels(fx_0cd_effModels, 3);
        ex->btlfx_getUnitPos(0, (s16 *)&work->posA4);
        ex->btlfx_getUnitBodyPos(0, 0, (s16 *)&work->posAC);
        work->model20.model0 = ex->btlfx_registerEffectModel((Unk800E8F50 *)ex->btlfx_getFileData(entry->file, 0xD), 1);
        work->model4C.model0 = ex->btlfx_registerEffectModel((Unk800E8F50 *)ex->btlfx_getFileData(entry->file, 3), 1);
        work->model78.model0 = ex->btlfx_registerEffectModel((Unk800E8F50 *)ex->btlfx_getFileData(entry->file, 1), 1);
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
            emitter = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, 8), (s32)&work->posA4);
            if (emitter != NULL) {
                emitter->trails = (s32)fx_0cd_trails;
                emitter->trailSlots = 0x26;
                emitter->trailStride = 0x7C;
            }
            break;
        case 0x8:
            emitter = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, 8), (s32)&work->posA4);
            if (emitter != NULL) {
                emitter->trails = (s32)fx_0cd_trails2;
                emitter->trailSlots = 0x26;
                emitter->trailStride = 0x7C;
            }
            break;
        case 0x10:
            emitter = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, 5), (s32)&work->posA4);
            if (emitter != NULL) {
                emitter->otZ = ex->btlfx_addRadiusSixth(0, -0x200);
            }
            work->timer0 = 0x20;
            break;
        case 0x28:
            emitter = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, 7), (s32)&work->posA4);
            if (emitter != NULL) {
                emitter->otZ = ex->btlfx_addRadiusSixth(0, -0x200);
            }
            work->timer4 = 0x30;
            break;
        case 0x38:
            work->timer8 = 0x18;
            break;
        case 0x50:
            ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, 0xA), (s32)&work->posA4);
            ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, 4), (s32)&work->posA4);
            break;
        case 0x58:
            ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, 0xC), (s32)&work->posA4);
            break;
        }
        if (work->timer0 != 0) {
            work->timer0--;
            work->model78.rot4.vy += 0x80;
            work->model78.scale1C.vx = work->model78.scale1C.vz = ex->btlfx_lerpRatio(0x2000, 0x1000, work->timer0, 0x20);
            work->model78.scale1C.vy = ex->btlfx_lerpRatio(0, 0x4000, work->timer0, 0x20);
            ex->Hi_DrawEffModel(&work->model78.rot4, &work->model78.posC, (s32)&work->model78.scale1C, work->model78.model0);
            if (work->timer0 > 0x18) {
                rgb = -((work->timer0 - 0x18) << 5);
                ex->Hi_ModifyEffModelRGB(work->model78.model0, rgb, rgb, rgb);
            }
            if (work->timer0 < 8) {
                rgb = (work->timer0 << 5) - 0x100;
                ex->Hi_ModifyEffModelRGB(work->model78.model0, rgb, rgb, rgb);
            }
        }
        if (work->timer4 != 0) {
            work->timer4--;
            work->model20.rot4.vy += 0x80;
            work->model20.scale1C.vx = work->model20.scale1C.vz = ex->btlfx_lerpRatio(0, 0x2000, work->timer4, 0x30);
            ex->Hi_DrawEffModel(&work->model20.rot4, &work->model20.posC, (s32)&work->model20.scale1C, work->model20.model0);
            if (work->timer4 > 0x28) {
                rgb = -((work->timer4 - 0x28) << 5);
                ex->Hi_ModifyEffModelRGB(work->model20.model0, rgb, rgb, rgb);
            }
            if (work->timer4 < 8) {
                rgb = work->timer4 * 16 - 0x80;
                ex->Hi_ModifyEffModelRGB(work->model20.model0, rgb, rgb, rgb);
            }
        }
        if (work->timer8 != 0) {
            work->timer8--;
            work->model4C.rot4.vy += 0x80;
            work->model4C.scale1C.vx = work->model4C.scale1C.vz = ex->btlfx_lerpRatio(0x4000, 0, work->timer8, 0x18);
            ex->Hi_DrawEffModel(&work->model4C.rot4, &work->model4C.posC, (s32)&work->model4C.scale1C, work->model4C.model0);
            rgb = -0x40;
            if (work->timer8 > 0x10) {
                rgb = -((work->timer8 - 0x10) * 8) - 0x40;
            }
            if (work->timer8 < 8) {
                rgb = work->timer8 * 8 - 0x80;
            }
            ex->Hi_ModifyEffModelRGB(work->model4C.model0, rgb, rgb, rgb);
        }
        return frame >= 0x80;
    }
    return 0;
}

INCLUDE_RODATA("asm/us/fx_0cd/nonmatchings/fx_0cd", fx_0cd_headerPtr);
