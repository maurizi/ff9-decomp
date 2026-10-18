#include "fxmod/fx_037.h"

s32 fx_037_op80(s32 mode, Fx037Work *work, Unk800F78A4 *env, Unk80100C78Entry *entry, s32 *arg4) {
    BtlfxExports *ex = BTLFX_EXPORTS(env);
    s32 frame;

    if (mode == 0) {
        *arg4 = 0x1C;
    } else if (mode == 1) {
        D_801E79B4 = env;
        ex->btlfx_getUnitBodyPos(0, 0, (s16 *)&work->unk14);
        work->unk0 = ex->func_800E3CEC(ex->btlfx_getFileData(entry->unk8, 2), (s32)&work->unk14);
        if (work->unk0 != NULL) {
            work->unk0->unk14 = (s32)D_801E79C4;
            work->unk0->unk12 = 0x15;
            work->unk0->unkC = 0x60;
        }
    } else {
        frame = entry->unk0;
        switch (frame) {
        case 8:
            work->unk8 = ex->func_800E3CEC(ex->btlfx_getFileData(entry->unk8, 5), (s32)&work->unk14);
            if (work->unk8 != NULL) {
                work->unk8->unk14 = (s32)D_801E81A4;
                work->unk8->unk12 = 0x2C;
                work->unk8->unkC = 0x8C;
            }
            break;
        case 10:
            work->unk4 = ex->func_800E3CEC(ex->btlfx_getFileData(entry->unk8, 4), (s32)&work->unk14);
            work->unkC = ex->func_800E3CEC(ex->btlfx_getFileData(entry->unk8, 1), (s32)&work->unk14);
            if (work->unk4 != NULL) {
                work->unk4->unk22 = ex->func_800E7CC4(0, 0x20);
            }
            break;
        case 20:
            work->unk10 = ex->func_800E3CEC(ex->btlfx_getFileData(entry->unk8, 3), (s32)&work->unk14);
            if (work->unk10 != NULL) {
                work->unk10->unk22 = ex->func_800E7CC4(0, 0x20);
            }
            break;
        }
        return frame >= 0x3C;
    }
    return 0;
}

INCLUDE_RODATA("asm/us/fx_037/nonmatchings/fx_037", fx_037_headerPtr);
