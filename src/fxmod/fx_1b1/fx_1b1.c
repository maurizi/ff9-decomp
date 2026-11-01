#include "fxmod/fx_1b1.h"

s32 fx_1b1_op80(s32 mode, Fx1b1Work *work, Unk800F78A4 *env, Unk80100C78Entry *entry, s32 *args) {
    BtlfxExports *ex = BTLFX_EXPORTS(env);
    s32 frame;

    if (mode == 0) {
        *args = 0x18;
    } else if (mode == 1) {
        fx_1b1_env = env;
        ex->btlfx_getUnitBodyPos(0, 0, (s16 *)&work->pos8);
        work->pos8.vy += 0xC8;
        ex->btlfx_getUnitPos(0, (s16 *)&work->pos10);
        work->emit0 = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, 1), (s32)&work->pos8);
        if (work->emit0 != NULL) {
            work->emit0->otZ = ex->btlfx_addRadiusSixth(0, 0x20);
        }
    } else {
        frame = entry->frame;
        if (frame == 0x18) {
            work->emit4 = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, 5), (s32)&work->pos10);
            if (work->emit4 != NULL) {
                work->emit4->otZ = ex->btlfx_addRadiusSixth(0, 0x20);
            }
        }
        return frame >= 0x50;
    }
    return 0;
}

INCLUDE_RODATA("asm/us/fx_1b1/nonmatchings/fx_1b1", fx_1b1_headerPtr);
