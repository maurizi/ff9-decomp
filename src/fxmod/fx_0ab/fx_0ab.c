#include "fxmod/fx_0ab.h"

s32 fx_0ab_op80(s32 mode, Fx0abWork *work, Unk800F78A4 *env, Unk80100C78Entry *entry, s32 *args) {
    BtlfxExports *ex = BTLFX_EXPORTS(env);
    s32 frame;

    if (mode == 0) {
        *args = 0x28;
    } else if (mode == 1) {
        fx_0ab_env = env;
        ex->btlfx_getUnitPos(0, (s16 *)&work->pos10);
        work->pos18.vx = work->pos10.vx;
        work->pos18.vy = work->pos10.vy;
        work->pos18.vz = work->pos10.vz;
        work->pos20.vx = work->pos10.vx;
        work->pos20.vy = work->pos10.vy;
        work->pos20.vz = work->pos10.vz;
        work->pos18.vy -= 0xC8;
        work->pos20.vy -= 0x190;
        work->emit0 = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, FX_MAELSTROM_WIND_TRAIL), (s32)&work->pos10);
        if (work->emit0 != NULL) {
            work->emit0->trails = (s32)fx_0ab_trails;
            work->emit0->trailSlots = 0x11;
            work->emit0->trailStride = 0x60;
        }
        work->emit8 = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, FX_MAELSTROM_SWIRL), (s32)&work->pos10);
    } else {
        frame = entry->frame;
        if (frame == 0xC) {
            work->emit4 = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, FX_MAELSTROM_RED_STREAKS), (s32)&work->pos18);
        }
        if (frame == 0x10) {
            work->emitC = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, FX_MAELSTROM_RED_PUFFS), (s32)&work->pos20);
        }
        return frame >= 0x28;
    }
    return 0;
}

INCLUDE_RODATA("asm/us/fx_0ab/nonmatchings/fx_0ab", fx_0ab_headerPtr);
