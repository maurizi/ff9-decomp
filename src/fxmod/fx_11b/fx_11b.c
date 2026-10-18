#include "fxmod/fx_11b.h"

s32 fx_11b_op80(s32 mode, Fx11bWork *work, Unk800F78A4 *env, Unk80100C78Entry *entry, s32 *arg4) {
    BtlfxExports *ex = BTLFX_EXPORTS(env);

    if (mode == 0) {
        *arg4 = 0xC;
    } else if (mode == 1) {
        D_801E783C = env;
        env->unk2C->unk52 = 0;
        ex->btlfx_getUnitBodyPos(0, 0, (s16 *)&work->unk4);
        work->unk0 = ex->func_800E3CEC(ex->btlfx_getFileData(entry->unk8, 1), (s32)&work->unk4);
        if (work->unk0 != NULL) {
            work->unk0->unk22 = ex->func_800E7CC4(0, 0x80);
        }
    } else {
        return entry->unk0 >= 0x32;
    }
    return 0;
}

INCLUDE_RODATA("asm/us/fx_11b/nonmatchings/fx_11b", fx_11b_headerPtr);
