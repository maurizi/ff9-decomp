#include "fxmod/fx_0d1.h"

s32 fx_0d1_op80(s32 mode, Fx0d1Work *work, Unk800F78A4 *env, Unk80100C78Entry *entry, s32 *arg4) {
    BtlfxExports *ex = BTLFX_EXPORTS(env);
    s32 frame;

    if (mode == 0) {
        *arg4 = 0x24;
    } else if (mode == 1) {
        D_801E7990 = env;
        ex->btlfx_getUnitBodyPos(0x10, 0, (s16 *)&work->unk14);
        ex->btlfx_getUnitBodyPos(0, 0, (s16 *)&work->unk1C);
        work->unk0 = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, 0xD), (s32)&work->unk14);
        work->unk4 = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, 0xE), (s32)&work->unk14);
        if (work->unk0 != NULL) {
            work->unk0->unk22 = ex->func_800E7CC4(0x10, 0x80);
        }
        if (work->unk4 != NULL) {
            work->unk4->unk22 = ex->func_800E7CC4(0x10, 0x80);
        }
    } else {
        frame = entry->frame;
        if (frame == 0x24) {
            work->unk8 = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, 4), (s32)&work->unk1C);
            if (work->unk8 != NULL) {
                work->unk8->unk22 = 0x100;
            }
        }
        if (frame == 0x38) {
            work->unkC = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, 9), (s32)&work->unk1C);
            work->unk10 = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, 7), (s32)&work->unk1C);
            if (work->unkC != NULL) {
                work->unkC->unk22 = 0x100;
            }
            if (work->unk10 != NULL) {
                work->unk10->unk22 = 0x100;
            }
        }
        return frame >= 0x50;
    }
    return 0;
}

INCLUDE_RODATA("asm/us/fx_0d1/nonmatchings/fx_0d1", fx_0d1_headerPtr);
