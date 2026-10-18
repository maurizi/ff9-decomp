#include "fxmod/fx_0bb.h"

s32 fx_0bb_op80(s32 mode, Fx0bbWork80 *work, Unk800F78A4 *env, Unk80100C78Entry *entry, s32 *arg4) {
    BtlfxExports *ex = BTLFX_EXPORTS(env);

    if (mode == 0) {
        *arg4 = 0xC;
    } else if (mode == 1) {
        D_801E79A0 = env;
        ex->btlfx_getUnitBodyPos(0, 0, (s16 *)&work->unk4);
        work->unk0 = ex->func_800E3CEC(ex->btlfx_getFileData(entry->unk8, 7), (s32)&work->unk4);
        if (work->unk0 != NULL) {
            work->unk0->unk22 = ex->func_800E7CC4(0, 0x80);
        }
    } else {
        return entry->unk0 >= 0x78;
    }
    return 0;
}

s32 fx_0bb_op81(s32 mode, Fx0bbWork81 *work, Unk800F78A4 *env, Unk80100C78Entry *entry, s32 *arg4) {
    BtlfxExports *ex = BTLFX_EXPORTS(env);
    s32 frame;

    if (mode == 0) {
        *arg4 = 0x10;
    } else if (mode == 1) {
        D_801E79A0 = env;
        ex->btlfx_getUnitBodyPos(0, 0, (s16 *)&work->unk8);
        work->unk0 = ex->func_800E3CEC(ex->btlfx_getFileData(entry->unk8, 0xA), (s32)&work->unk8);
        if (work->unk0 != NULL) {
            work->unk0->unk22 = ex->func_800E7CC4(0, 0x80);
        }
    } else {
        frame = entry->unk0;
        if (frame == 5) {
            work->unk4 = ex->func_800E3CEC(ex->btlfx_getFileData(entry->unk8, 9), (s32)&work->unk8);
            if (work->unk4 != NULL) {
                work->unk4->unk22 = ex->func_800E7CC4(0, 0x80);
            }
        }
        ex->btlfx_drawBackdrop((BtlBgLayers *)ex->btlfx_getFileData(entry->unk8, 1), frame);
        return frame >= 0x32;
    }
    return 0;
}

INCLUDE_RODATA("asm/us/fx_0bb/nonmatchings/fx_0bb", fx_0bb_headerPtr);
