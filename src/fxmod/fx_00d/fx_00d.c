#include "fxmod/fx_00d.h"

s32 fx_00d_op80(s32 mode, Fx00dWork *work, Unk800F78A4 *env, Unk80100C78Entry *entry, s32 *args) {
    BtlfxExports *ex = BTLFX_EXPORTS(env);
    s32 frame;

    if (mode == 0) {
        *args = sizeof(Fx00dWork);
    } else if (mode == 1) {
        fx_00d_env = env;
        ex->btlfx_getUnitBodyPos(0, 0, (s16 *)&work->posC);
        work->emit0 = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, FX_STONA_LIGHT_TRAIL), (s32)&work->posC);
        if (work->emit0 != NULL) {
            work->emit0->trails = (s32)fx_00d_trails;
            work->emit0->trailSlots = FX_STONA_LIGHT_TRAIL_TRAIL_SLOTS;
            work->emit0->trailStride = FX_STONA_LIGHT_TRAIL_TRAIL_STRIDE;
        }
    } else {
        frame = entry->frame;
        if (frame == FX_STONA_BLUE_ORBS_FRAME) {
            work->emit4 = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, FX_STONA_BLUE_ORBS), (s32)&work->posC);
            work->emit8 = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, FX_STONA_GOLD_RINGS), (s32)&work->posC);
            if (work->emit4 != NULL) {
                work->emit4->otZ = ex->btlfx_addRadiusSixth(0, 0x80);
            }
            if (work->emit8 != NULL) {
                work->emit8->otZ = ex->btlfx_addRadiusSixth(0, 0x80);
            }
        }
        return frame >= FX_STONA_FRAMES;
    }
    return 0;
}

INCLUDE_RODATA("asm/us/fx_00d/nonmatchings/fx_00d", fx_00d_headerPtr);
