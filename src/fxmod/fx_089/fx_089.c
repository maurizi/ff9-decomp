#include "fxmod/fx_089.h"

s32 fx_089_op80(s32 mode, Fx089Work *work, Unk800F78A4 *env, Unk80100C78Entry *entry, s32 *args) {
    BtlfxExports *ex = BTLFX_EXPORTS(env);
    s32 frame;

    if (mode == 0) {
        *args = 0x10;
    } else if (mode == 1) {
        fx_089_env = env;
        ex->btlfx_getUnitPos(0, (s16 *)&work->pos8);
        work->emit0 = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, FX_REVIVE_LIGHT_SWIRL), (s32)&work->pos8);
        if (work->emit0 != NULL) {
            work->emit0->otZ = ex->btlfx_addRadiusSixth(0, 0x20);
        }
    } else {
        frame = entry->frame;
        if (frame == 0xF) {
            work->emit4 = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, FX_REVIVE_FEATHERS), (s32)&work->pos8);
            if (work->emit4 != NULL) {
                work->emit4->otZ = 0x20;
            }
        }
        return frame >= 0x28;
    }
    return 0;
}

INCLUDE_RODATA("asm/us/fx_089/nonmatchings/fx_089", fx_089_headerPtr);
