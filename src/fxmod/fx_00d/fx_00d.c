#include "fxmod/fx_00d.h"

s32 fx_00d_op80(s32 mode, Fx00dWork *work, Unk800F78A4 *env, Unk80100C78Entry *entry, s32 *arg4) {
    BtlfxExports *ex = BTLFX_EXPORTS(env);
    s32 frame;

    if (mode == 0) {
        *arg4 = 0x14;
    } else if (mode == 1) {
        D_801E7908 = env;
        ex->btlfx_getUnitBodyPos(0, 0, (s16 *)&work->unkC);
        work->unk0 = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, 2), (s32)&work->unkC);
        if (work->unk0 != NULL) {
            work->unk0->unk14 = (s32)D_801E7918;
            work->unk0->unk12 = 0x13;
            work->unk0->unkC = 0x34;
        }
    } else {
        frame = entry->frame;
        if (frame == 0xF) {
            work->unk4 = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, 3), (s32)&work->unkC);
            work->unk8 = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, 9), (s32)&work->unkC);
            if (work->unk4 != NULL) {
                work->unk4->unk22 = ex->func_800E7CC4(0, 0x80);
            }
            if (work->unk8 != NULL) {
                work->unk8->unk22 = ex->func_800E7CC4(0, 0x80);
            }
        }
        return frame >= 0x32;
    }
    return 0;
}

INCLUDE_RODATA("asm/us/fx_00d/nonmatchings/fx_00d", fx_00d_headerPtr);
