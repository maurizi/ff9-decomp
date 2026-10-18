#include "fxmod/fx_0c6.h"

s32 fx_0c6_op80(s32 mode, Fx0c6Work *work, Unk800F78A4 *env, Unk80100C78Entry *entry, s32 *arg4) {
    BtlfxExports *ex = BTLFX_EXPORTS(env);
    s32 frame;

    if (mode == 0) {
        *arg4 = 0x14;
    } else if (mode == 1) {
        s32 n = *arg4;
        work->unk10 = n;
        D_801E7950 = env;
        if (n >= env->unkC->unk24) {
            func_801E78F4(env, 7, 1);
            return 1;
        }
        func_801E78F4(env, 7, 0);
        ex->func_800E9930(4, entry->unk8, -1);
        ex->func_800E9930(6, entry->unk8, -1);
        ex->func_800E9930(7, entry->unk8, -1);
        ex->btlfx_getUnitPos(work->unk10, (s16 *)&work->unk8);
        work->unk0 = ex->func_800E3CEC(ex->btlfx_getFileData(entry->unk8, 5), (s32)&work->unk8);
        if (work->unk0 != NULL) {
            work->unk0->unk22 = ex->func_800E7CC4(work->unk10, 0x20);
        }
    } else {
        frame = entry->unk0;
        if (frame == 0x12) {
            work->unk4 = ex->func_800E3CEC(ex->btlfx_getFileData(entry->unk8, 8), (s32)&work->unk8);
            if (work->unk4 != NULL) {
                work->unk4->unk22 = ex->func_800E7CC4(work->unk10, 0x80);
            }
        }
        if (frame == 0xC) {
            func_801E78F4(env, 7, 1);
        }
        return frame >= 0x3C;
    }
    return 0;
}

void func_801E78F4(Unk800F78A4 *env, s32 i, s32 v) {
    *((u8 *)env->unk0 + i + 0x10) = v;
}

INCLUDE_RODATA("asm/us/fx_0c6/nonmatchings/fx_0c6", fx_0c6_headerPtr);
