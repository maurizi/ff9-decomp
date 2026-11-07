#include "fxmod/fx_017.h"

s32 fx_017_op80(s32 mode, Fx017Work *work, Unk800F78A4 *env, Unk80100C78Entry *entry, s32 *args) {
    BtlfxExports *ex = BTLFX_EXPORTS(env);
    s32 frame;

    if (mode == 0) {
        *args = sizeof(Fx017Work);
    } else if (mode == 1) {
        fx_017_env = env;
        ex->btlfx_getUnitArgBonePos(BTLFX_UNIT_CASTER, 0, (u16 *)&work->pos14);
        ex->btlfx_getUnitBodyPos(0, 0, (s16 *)&work->posC);
        work->pos1C.vx = work->posC.vx;
        work->pos1C.vy = work->posC.vy;
        work->pos1C.vz = work->posC.vz;
        work->pos1C.vy = 0;
        work->emit0 = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, FX_DEVILS_CANDLE_FLAME_2), (s32)&work->pos14);
        if (work->emit0 != NULL) {
            work->emit0->otZ = ex->btlfx_addRadiusSixth(0, 0x10);
        }
    } else {
        frame = entry->frame;
        if (frame == FX_DEVILS_CANDLE_FLAME_FRAME) {
            work->emit4 = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, FX_DEVILS_CANDLE_FLAME), (s32)&work->pos1C);
            work->emit8 = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, FX_DEVILS_CANDLE_BLUE_STARBURST), (s32)&work->posC);
        }
        return frame >= FX_DEVILS_CANDLE_FRAMES;
    }
    return 0;
}

INCLUDE_RODATA("asm/us/fx_017/nonmatchings/fx_017", fx_017_headerPtr);
