#include "fxmod/fx_00d.h"

s32 fx_00d_op80(s32 mode, Fx00dWork *work, Unk800F78A4 *env, Unk80100C78Entry *entry, s32 *arg4) {
    BtlfxExports *ex = BTLFX_EXPORTS(env);
    s32 frame;

    if (mode == 0) {
        *arg4 = 0x14;
    } else if (mode == 1) {
        fx_00d_env = env;
        ex->btlfx_getUnitBodyPos(0, 0, (s16 *)&work->posC);
        work->emit0 = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, 2), (s32)&work->posC);
        if (work->emit0 != NULL) {
            work->emit0->unk14 = (s32)D_801E7918;
            work->emit0->unk12 = 0x13;
            work->emit0->unkC = 0x34;
        }
    } else {
        frame = entry->frame;
        if (frame == 0xF) {
            work->emit4 = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, 3), (s32)&work->posC);
            work->emit8 = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, 9), (s32)&work->posC);
            if (work->emit4 != NULL) {
                work->emit4->otZ = ex->btlfx_addRadiusSixth(0, 0x80);
            }
            if (work->emit8 != NULL) {
                work->emit8->otZ = ex->btlfx_addRadiusSixth(0, 0x80);
            }
        }
        return frame >= 0x32;
    }
    return 0;
}

INCLUDE_RODATA("asm/us/fx_00d/nonmatchings/fx_00d", fx_00d_headerPtr);
