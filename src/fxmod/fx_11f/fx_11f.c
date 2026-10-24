#include "fxmod/fx_11f.h"

s32 fx_11f_op80(s32 mode, Fx11fWork *work, Unk800F78A4 *env, Unk80100C78Entry *entry, s32 *arg4) {
    BtlfxExports *ex = BTLFX_EXPORTS(env);
    s32 frame;

    if (mode == 0) {
        *arg4 = 0x18;
    } else if (mode == 1) {
        D_801E7990 = env;
        env->camera->unk52 = 0;
        ex->btlfx_getUnitBodyPos(0, 0, (s16 *)&work->unk10);
        work->unk0 = ex->func_800E3CEC(ex->btlfx_getFileData(entry->unk8, 1), (s32)&work->unk10);
        if (work->unk0 != NULL) {
            work->unk0->unk22 = ex->func_800E7CC4(0, 0x80);
        }
    } else {
        frame = entry->unk0;
        if (frame == 0xC) {
            work->unk4 = ex->func_800E3CEC(ex->btlfx_getFileData(entry->unk8, 2), (s32)&work->unk10);
            if (work->unk4 != NULL) {
                work->unk4->unk22 = ex->func_800E7CC4(0, 0x80);
            }
        }
        if (frame == 0x12) {
            work->unk8 = ex->func_800E3CEC(ex->btlfx_getFileData(entry->unk8, 3), (s32)&work->unk10);
            work->unkC = ex->func_800E3CEC(ex->btlfx_getFileData(entry->unk8, 4), (s32)&work->unk10);
            if (work->unk8 != NULL) {
                work->unk8->unk14 = (s32)D_801E79A0;
                work->unk8->unk12 = 0x14;
                work->unk8->unkC = 0x90;
                if (work->unk8 != NULL) {
                    work->unk8->unk22 = ex->func_800E7CC4(0, 0x80);
                }
            }
            if (work->unkC != NULL) {
                work->unkC->unk22 = ex->func_800E7CC4(0, 0x80);
            }
        }
        return frame >= 0x32;
    }
    return 0;
}

INCLUDE_RODATA("asm/us/fx_11f/nonmatchings/fx_11f", fx_11f_headerPtr);
