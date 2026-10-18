#include "fxmod/fx_1b1.h"

s32 fx_1b1_op80(s32 mode, Fx1b1Work *work, Unk800F78A4 *env, Unk80100C78Entry *entry, s32 *arg4) {
    BtlfxExports *ex = BTLFX_EXPORTS(env);
    s32 frame;

    if (mode == 0) {
        *arg4 = 0x18;
    } else if (mode == 1) {
        D_801E78B0 = env;
        ex->btlfx_getUnitBodyPos(0, 0, (s16 *)&work->unk8);
        work->unk8.vy += 0xC8;
        ex->btlfx_getUnitPos(0, (s16 *)&work->unk10);
        work->unk0 = ex->func_800E3CEC(ex->btlfx_getFileData(entry->unk8, 1), (s32)&work->unk8);
        if (work->unk0 != NULL) {
            work->unk0->unk22 = ex->func_800E7CC4(0, 0x20);
        }
    } else {
        frame = entry->unk0;
        if (frame == 0x18) {
            work->unk4 = ex->func_800E3CEC(ex->btlfx_getFileData(entry->unk8, 5), (s32)&work->unk10);
            if (work->unk4 != NULL) {
                work->unk4->unk22 = ex->func_800E7CC4(0, 0x20);
            }
        }
        return frame >= 0x50;
    }
    return 0;
}

INCLUDE_RODATA("asm/us/fx_1b1/nonmatchings/fx_1b1", fx_1b1_headerPtr);
