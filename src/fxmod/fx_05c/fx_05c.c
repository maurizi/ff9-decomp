#include "fxmod/fx_05c.h"

s32 fx_05c_op80(s32 mode, Fx05cWork *work, Unk800F78A4 *env, Unk80100C78Entry *entry, s32 *arg4) {
    BtlfxExports *ex = BTLFX_EXPORTS(env);
    s32 frame;

    if (mode == 0) {
        *arg4 = 0x1C;
    } else if (mode == 1) {
        D_801E7924 = env;
        ex->btlfx_getUnitBodyPos(0x10, 0, (s16 *)&work->unkC);
        ex->btlfx_getUnitBodyPos(0, 0, (s16 *)&work->unk14);
        work->unk0 = ex->func_800E3CEC(ex->btlfx_getFileData(entry->unk8, 0xB), (s32)&work->unkC);
        work->unk4 = ex->func_800E3CEC(ex->btlfx_getFileData(entry->unk8, 0xC), (s32)&work->unkC);
        if (work->unk0 != NULL) {
            work->unk0->unk22 = ex->func_800E7CC4(0x10, 0x80);
        }
        if (work->unk4 != NULL) {
            work->unk4->unk22 = ex->func_800E7CC4(0x10, 0x80);
        }
    } else {
        frame = entry->unk0;
        if (frame == 0x24) {
            work->unk8 = ex->func_800E3CEC(ex->btlfx_getFileData(entry->unk8, 7), (s32)&work->unk14);
            if (work->unk8 != NULL) {
                work->unk8->unk22 = ex->func_800E7CC4(0, 0x80);
            }
        }
        if (frame >= 0x5A) {
            ex->func_800E3D54(work->unk8);
            return 1;
        }
    }
    return 0;
}

INCLUDE_RODATA("asm/us/fx_05c/nonmatchings/fx_05c", fx_05c_headerPtr);
