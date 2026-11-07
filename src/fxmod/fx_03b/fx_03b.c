#include "fxmod/fx_03b.h"

s32 fx_03b_op80(s32 mode, Fx03bWork *work, Unk800F78A4 *env, Unk80100C78Entry *entry, s32 *args) {
    BtlfxExports *ex = BTLFX_EXPORTS(env);
    s32 frame;

    if (mode == 0) {
        *args = sizeof(Fx03bWork);
    } else if (mode == 1) {
        fx_03b_env = env;
        ex->btlfx_getUnitPos(0, (s16 *)&work->pos10);
        work->pos18.vx = work->pos10.vx;
        work->pos18.vy = work->pos10.vy;
        work->pos18.vz = work->pos10.vz;
        work->pos18.vy -= FX_TROUBLE_JUICE_TRAIL_HEIGHT;
        ex->btlfx_getUnitBodyPos(0, 1, (s16 *)&work->pos20);
        ex->btlfx_getUnitArgBonePos(0x10, 0, (u16 *)&work->pos28);
        work->emit0 = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, FX_TROUBLE_JUICE_SPLASH), (s32)&work->pos28);
        if (work->emit0 != NULL) {
            work->emit0->otZ = ex->btlfx_addRadiusSixth(0, 0x80);
        }
    } else {
        frame = entry->frame;
        if (frame == FX_TROUBLE_JUICE_TRAIL_2_FRAME) {
            work->emitC = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, FX_TROUBLE_JUICE_SCREEN_TRAIL_2), (s32)&work->pos20);
            if (work->emitC != NULL) {
                work->emitC->trails = (s32)fx_03b_trails;
                work->emitC->trailSlots = FX_TROUBLE_JUICE_SCREEN_TRAIL_2_TRAIL_SLOTS;
                work->emitC->trailStride = FX_TROUBLE_JUICE_SCREEN_TRAIL_2_TRAIL_STRIDE;
            }
        }
        if (frame == FX_TROUBLE_JUICE_RINGS_FRAME) {
            work->emit4 = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, FX_TROUBLE_JUICE_SCRIBBLE_RINGS), (s32)&work->pos10);
            work->emit8 = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, FX_TROUBLE_JUICE_SCREEN_TRAIL), (s32)&work->pos18);
            if (work->emit8 != NULL) {
                work->emit8->trails = (s32)fx_03b_trails2;
                work->emit8->trailSlots = FX_TROUBLE_JUICE_SCREEN_TRAIL_TRAIL_SLOTS;
                work->emit8->trailStride = FX_TROUBLE_JUICE_SCREEN_TRAIL_TRAIL_STRIDE;
            }
        }
        return frame >= FX_TROUBLE_JUICE_FRAMES;
    }
    return 0;
}

INCLUDE_RODATA("asm/us/fx_03b/nonmatchings/fx_03b", fx_03b_headerPtr);
