#include "fxmod/fx_0d4.h"

s32 fx_0d4_op80(s32 mode, Fx0d4Work80 *work, Unk800F78A4 *env, Unk80100C78Entry *entry, s32 *arg4) {
    BtlfxExports *ex = BTLFX_EXPORTS(env);
    s32 frame;

    if (mode == 0) {
        *arg4 = 0x18;
    } else if (mode == 1) {
        D_801E7C2C = env;
        work->unk0 = 0;
        ex->btlfx_getUnitBodyPos(0, 0, (s16 *)&work->unk8);
        ex->btlfx_getUnitPos(0, (s16 *)&work->unk10);
        work->unk4 = ex->func_800E3CEC(ex->btlfx_getFileData(entry->unk8, 1), (s32)&work->unk8);
        if (work->unk4 != NULL) {
            work->unk4->unk22 = ex->func_800E7CC4(0, 0x20);
        }
        work->unk4 = ex->func_800E3CEC(ex->btlfx_getFileData(entry->unk8, 9), (s32)&work->unk10);
    } else {
        frame = entry->unk0;
        switch (work->unk0) {
        case 0:
            if (frame == 2) {
                work->unk4 = ex->func_800E3CEC(ex->btlfx_getFileData(entry->unk8, 2), (s32)&work->unk8);
                if (work->unk4 != NULL) {
                    work->unk4->unk22 = ex->func_800E7CC4(0, 0x20);
                }
            }
            if (frame >= 0x3C) {
                return 1;
            }
            break;
        }
    }
    return 0;
}

INCLUDE_ASM("asm/us/fx_0d4/nonmatchings/fx_0d4", fx_0d4_op84);

INCLUDE_RODATA("asm/us/fx_0d4/nonmatchings/fx_0d4", fx_0d4_headerPtr);
