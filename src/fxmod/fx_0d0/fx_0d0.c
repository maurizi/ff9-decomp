#include "fxmod/fx_0d0.h"

s32 fx_0d0_op80(s32 mode, Fx0d0Work *work, Unk800F78A4 *env, Unk80100C78Entry *entry, s32 *args) {
    BtlfxExports *ex = BTLFX_EXPORTS(env);
    s32 frame;

    if (mode == 0) {
        *args = sizeof(Fx0d0Work);
    } else if (mode == 1) {
        fx_0d0_env = env;
        ex->btlfx_getUnitBodyPos(0x10, 0, (s16 *)&work->pos10);
        ex->btlfx_getUnitBodyPos(0, 0, (s16 *)&work->pos18);
        work->emit0 = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, FX_POWER_BREAK_ENERGY_ORB), (s32)&work->pos10);
        work->emit4 = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, FX_POWER_BREAK_SPARKLES), (s32)&work->pos10);
        if (work->emit0 != NULL) {
            work->emit0->otZ = ex->btlfx_addRadiusSixth(0x10, 0x80);
        }
        if (work->emit4 != NULL) {
            work->emit4->otZ = ex->btlfx_addRadiusSixth(0x10, 0x80);
        }
    } else {
        frame = entry->frame;
        if (frame == FX_POWER_BREAK_STREAK_FRAME) {
            work->emit8 = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, FX_POWER_BREAK_RED_SWIRL), (s32)&work->pos18);
            work->emitC = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, FX_POWER_BREAK_LIGHT_STREAK), (s32)&work->pos18);
            if (work->emitC != NULL) {
                work->emitC->trails = (s32)fx_0d0_trails;
                work->emitC->trailSlots = FX_POWER_BREAK_LIGHT_STREAK_TRAIL_SLOTS;
                work->emitC->trailStride = FX_POWER_BREAK_LIGHT_STREAK_TRAIL_STRIDE;
            }
            if (work->emit8 != NULL) {
                work->emit8->otZ = ex->btlfx_addRadiusSixth(0, 0x20);
            }
            if (work->emitC != NULL) {
                work->emitC->otZ = ex->btlfx_addRadiusSixth(0, 0x20);
            }
        }
        return frame >= FX_POWER_BREAK_FRAMES;
    }
    return 0;
}

INCLUDE_RODATA("asm/us/fx_0d0/nonmatchings/fx_0d0", fx_0d0_headerPtr);
