#include "fxmod/fx_037.h"

s32 fx_037_op80(s32 mode, Fx037Work *work, Unk800F78A4 *env, Unk80100C78Entry *entry, s32 *args) {
    BtlfxExports *ex = BTLFX_EXPORTS(env);
    s32 frame;

    if (mode == 0) {
        *args = sizeof(Fx037Work);
    } else if (mode == 1) {
        fx_037_env = env;
        ex->btlfx_getUnitBodyPos(0, 0, (s16 *)&work->pos14);
        work->emit0 = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, FX_DARKNESS_GREEN_HAZE), (s32)&work->pos14);
        if (work->emit0 != NULL) {
            work->emit0->trails = (s32)fx_037_trails;
            work->emit0->trailSlots = FX_DARKNESS_GREEN_HAZE_TRAIL_SLOTS;
            work->emit0->trailStride = FX_DARKNESS_GREEN_HAZE_TRAIL_STRIDE;
        }
    } else {
        frame = entry->frame;
        switch (frame) {
        case FX_DARKNESS_STREAK_FRAME:
            work->emit8 = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, FX_DARKNESS_MAGENTA_STREAK), (s32)&work->pos14);
            if (work->emit8 != NULL) {
                work->emit8->trails = (s32)fx_037_trails2;
                work->emit8->trailSlots = FX_DARKNESS_MAGENTA_STREAK_TRAIL_SLOTS;
                work->emit8->trailStride = FX_DARKNESS_MAGENTA_STREAK_TRAIL_STRIDE;
            }
            break;
        case FX_DARKNESS_ORBS_FRAME:
            work->emit4 = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, FX_DARKNESS_DARK_ORBS), (s32)&work->pos14);
            work->emitC = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, FX_DARKNESS_DARK_CRESCENTS), (s32)&work->pos14);
            if (work->emit4 != NULL) {
                work->emit4->otZ = ex->btlfx_addRadiusSixth(0, 0x20);
            }
            break;
        case FX_DARKNESS_GLOW_FRAME:
            work->emit10 = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, FX_DARKNESS_WHITE_GLOW), (s32)&work->pos14);
            if (work->emit10 != NULL) {
                work->emit10->otZ = ex->btlfx_addRadiusSixth(0, 0x20);
            }
            break;
        }
        return frame >= FX_DARKNESS_FRAMES;
    }
    return 0;
}

INCLUDE_RODATA("asm/us/fx_037/nonmatchings/fx_037", fx_037_headerPtr);
