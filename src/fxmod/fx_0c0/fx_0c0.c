#include "fxmod/fx_0c0.h"

s32 fx_0c0_op80(s32 mode, Fx0c0Work *work, Unk800F78A4 *env, Unk80100C78Entry *entry, s32 *arg4) {
    BtlfxExports *ex = BTLFX_EXPORTS(env);
    s32 frame;

    if (mode == 0) {
        *arg4 = 0x20;
    } else if (mode == 1) {
        D_801E7974 = env;
        ex->btlfx_getUnitBodyPos(0x10, 0, (s16 *)&work->unk10);
        ex->btlfx_getUnitBodyPos(0, 0, (s16 *)&work->unk18);
        work->unk0 = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, 9), (s32)&work->unk10);
        work->unk4 = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, 0xD), (s32)&work->unk10);
        if (work->unk0 != NULL) {
            work->unk0->otZ = ex->btlfx_addRadiusSixth(0x10, 0x80);
        }
        if (work->unk4 != NULL) {
            work->unk4->otZ = ex->btlfx_addRadiusSixth(0x10, 0x80);
        }
    } else {
        frame = entry->frame;
        if (frame == 0x24) {
            work->unk8 = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, 0xB), (s32)&work->unk18);
            work->unkC = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, 7), (s32)&work->unk18);
            if (work->unk8 != NULL) {
                work->unk8->otZ = ex->btlfx_addRadiusSixth(0, 0x80);
            }
            if (work->unkC != NULL) {
                work->unkC->otZ = ex->btlfx_addRadiusSixth(0, 0x80);
            }
        }
        return frame >= 0x46;
    }
    return 0;
}

INCLUDE_RODATA("asm/us/fx_0c0/nonmatchings/fx_0c0", fx_0c0_headerPtr);
