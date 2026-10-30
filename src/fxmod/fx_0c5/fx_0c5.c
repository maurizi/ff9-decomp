#include "fxmod/fx_0c5.h"

s32 fx_0c5_op80(s32 mode, Fx0c5Work *work, Unk800F78A4 *env, Unk80100C78Entry *entry, s32 *arg4) {
    BtlfxExports *ex = BTLFX_EXPORTS(env);
    s32 frame;

    if (mode == 0) {
        *arg4 = 0x10;
    } else if (mode == 1) {
        D_801E7888 = env;
        ex->btlfx_getUnitPos(0, (s16 *)&work->unk8);
        work->unk0 = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, 8), (s32)&work->unk8);
        if (work->unk0 != NULL) {
            work->unk0->otZ = ex->btlfx_addRadiusSixth(0, 0x20);
        }
    } else {
        frame = entry->frame;
        if (frame == 0x12) {
            work->unk4 = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, 7), (s32)&work->unk8);
            if (work->unk4 != NULL) {
                work->unk4->otZ = ex->btlfx_addRadiusSixth(0, 0x80);
            }
        }
        return frame >= 0x32;
    }
    return 0;
}

INCLUDE_RODATA("asm/us/fx_0c5/nonmatchings/fx_0c5", fx_0c5_headerPtr);
