#include "fxmod/fx_1b9.h"

s32 fx_1b9_op80(s32 mode, Fx1b9Work *work, Unk800F78A4 *env, Unk80100C78Entry *entry, s32 *arg4) {
    BtlfxExports *ex = BTLFX_EXPORTS(env);
    s32 frame;

    if (mode == 0) {
        *arg4 = 0x28;
    } else if (mode == 1) {
        fx_1b9_env = env;
        ex->btlfx_getUnitPos(0x10, (s16 *)&work->unk10);
        work->unk18.vx = work->unk10.vx;
        work->unk18.vy = work->unk10.vy;
        work->unk18.vz = work->unk10.vz;
        work->unk20.vx = work->unk10.vx;
        work->unk20.vy = work->unk10.vy;
        work->unk20.vz = work->unk10.vz;
    } else {
        frame = entry->frame;
        switch (frame) {
        case 0:
            work->emit0 = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, 2), (s32)&work->unk20);
            work->emit4 = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, 7), (s32)&work->unk20);
            work->emit8 = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, 6), (s32)&work->unk18);
            if (work->emit0 != NULL) {
                work->emit0->otZ = ex->btlfx_addRadiusSixth(0x10, 0x20);
            }
            if (work->emit4 != NULL) {
                work->emit4->otZ = ex->btlfx_addRadiusSixth(0x10, 0x20);
            }
            break;
        case 8:
            work->emitC = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, 1), (s32)&work->unk18);
            break;
        }
        return frame >= 0x50;
    }
    return 0;
}

INCLUDE_RODATA("asm/us/fx_1b9/nonmatchings/fx_1b9", fx_1b9_headerPtr);
