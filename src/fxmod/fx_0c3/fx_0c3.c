#include "fxmod/fx_0c3.h"

s32 fx_0c3_op80(s32 mode, Fx0c3Work *work, Unk800F78A4 *env, Unk80100C78Entry *entry, s32 *arg4) {
    BtlfxExports *ex = BTLFX_EXPORTS(env);

    if (mode == 0) {
        *arg4 = 0x10;
    } else if (mode == 1) {
        D_801E7834 = env;
        ex->btlfx_getUnitPos(0, (s16 *)&work->unk8);
        work->unk0 = ex->func_800E3CEC(ex->btlfx_getFileData(entry->unk8, 8), (s32)&work->unk8);
        work->unk4 = ex->func_800E3CEC(ex->btlfx_getFileData(entry->unk8, 6), (s32)&work->unk8);
    } else {
        return entry->unk0 >= 0x32;
    }
    return 0;
}

INCLUDE_RODATA("asm/us/fx_0c3/nonmatchings/fx_0c3", fx_0c3_headerPtr);
