#include "fxmod/fx_1a5.h"

s32 fx_1a5_op80(s32 mode, Fx1a5Work *work, Unk800F78A4 *env, Unk80100C78Entry *entry, s32 *arg4) {
    BtlfxExports *ex = BTLFX_EXPORTS(env);
    s32 frame;

    if (mode == 0) {
        *arg4 = 0x14;
    } else if (mode == 1) {
        s32 unit = *arg4;
        work->unk10 = unit;
        D_801E7950 = env;
        if (unit >= env->targets->targetCount) {
            func_801E78F4(env, 7, 1);
            return 1;
        }
        func_801E78F4(env, 7, 0);
        ex->btlfx_playFileSound(6, entry->unk8, -1);
        ex->btlfx_playFileSound(7, entry->unk8, -1);
        ex->btlfx_playFileSound(8, entry->unk8, -1);
        ex->btlfx_getUnitPos(work->unk10, (s16 *)&work->unk8);
        work->unk0 = ex->func_800E3CEC(ex->btlfx_getFileData(entry->unk8, 1), (s32)&work->unk8);
        if (work->unk0 != NULL) {
            work->unk0->unk22 = ex->func_800E7CC4(work->unk10, 0x20);
        }
    } else {
        frame = entry->unk0;
        if (frame == 0xF) {
            work->unk4 = ex->func_800E3CEC(ex->btlfx_getFileData(entry->unk8, 2), (s32)&work->unk8);
            if (work->unk4 != NULL) {
                work->unk4->unk22 = ex->func_800E7CC4(work->unk10, 0x20);
            }
        }
        if (frame == 0xA) {
            func_801E78F4(env, 7, 1);
        }
        return frame >= 0x28;
    }
    return 0;
}

void func_801E78F4(Unk800F78A4 *env, s32 i, s32 v) {
    env->unk0->pad10[i] = v;
}

INCLUDE_RODATA("asm/us/fx_1a5/nonmatchings/fx_1a5", fx_1a5_headerPtr);
