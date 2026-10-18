#include "fxmod/fx_11a.h"

s32 fx_11a_op80(s32 mode, Fx11aWork *work, Unk800F78A4 *env, Unk80100C78Entry *entry, s32 *arg4) {
    BtlfxExports *ex = BTLFX_EXPORTS(env);

    if (mode == 0) {
        *arg4 = 0xC;
    } else if (mode == 1) {
        D_801E7814 = env;
        env->unk2C->unk52 = 0;
        ex->btlfx_getUnitPos(0, (s16 *)&work->unk4);
        work->unk0 = ex->func_800E3CEC(ex->btlfx_getFileData(entry->unk8, 1), (s32)&work->unk4);
    } else {
        return entry->unk0 >= 0x28;
    }
    return 0;
}

INCLUDE_RODATA("asm/us/fx_11a/nonmatchings/fx_11a", fx_11a_headerPtr);
