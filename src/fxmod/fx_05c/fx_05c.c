#include "fxmod/fx_05c.h"

s32 fx_05c_op80(s32 mode, Fx05cWork *work, Unk800F78A4 *env, Unk80100C78Entry *entry, s32 *arg4) {
    BtlfxExports *ex = BTLFX_EXPORTS(env);
    s32 frame;

    if (mode == 0) {
        *arg4 = 0x1C;
    } else if (mode == 1) {
        fx_05c_env = env;
        ex->btlfx_getUnitBodyPos(0x10, 0, (s16 *)&work->unkC);
        ex->btlfx_getUnitBodyPos(0, 0, (s16 *)&work->unk14);
        work->emit0 = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, 0xB), (s32)&work->unkC);
        work->emit4 = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, 0xC), (s32)&work->unkC);
        if (work->emit0 != NULL) {
            work->emit0->otZ = ex->btlfx_addRadiusSixth(0x10, 0x80);
        }
        if (work->emit4 != NULL) {
            work->emit4->otZ = ex->btlfx_addRadiusSixth(0x10, 0x80);
        }
    } else {
        frame = entry->frame;
        if (frame == 0x24) {
            work->emit8 = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, 7), (s32)&work->unk14);
            if (work->emit8 != NULL) {
                work->emit8->otZ = ex->btlfx_addRadiusSixth(0, 0x80);
            }
        }
        if (frame >= 0x5A) {
            ex->btlfx_stopEmitter(work->emit8);
            return 1;
        }
    }
    return 0;
}

INCLUDE_RODATA("asm/us/fx_05c/nonmatchings/fx_05c", fx_05c_headerPtr);
