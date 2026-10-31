#include "fxmod/fx_055.h"

s32 fx_055_op80(s32 mode, Fx055Work *work, Unk800F78A4 *env, Unk80100C78Entry *entry, s32 *arg4) {
    BtlfxExports *ex = BTLFX_EXPORTS(env);
    s32 frame;

    if (mode == 0) {
        *arg4 = 0x10;
    } else if (mode == 1) {
        fx_055_env = env;
        ex->btlfx_getUnitPos(0, (s16 *)&work->unk8);
        work->emit0 = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, 7), (s32)&work->unk8);
        if (work->emit0 != NULL) {
            work->emit0->unk14 = (s32)D_801E78CC;
            work->emit0->unk12 = 0xA;
            work->emit0->unkC = 0x90;
        }
        if (work->emit0 != NULL) {
            work->emit0->otZ = ex->btlfx_addRadiusSixth(0, 0x20);
        }
    } else {
        frame = entry->frame;
        if (frame == 0xF) {
            work->emit4 = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, 6), (s32)&work->unk8);
            if (work->emit4 != NULL) {
                work->emit4->otZ = ex->btlfx_addRadiusSixth(0, 0x20);
            }
        }
        return frame >= 0x32;
    }
    return 0;
}

INCLUDE_RODATA("asm/us/fx_055/nonmatchings/fx_055", fx_055_headerPtr);
