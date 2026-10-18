#include "fxmod/fx_192.h"

s32 fx_192_op80(s32 mode, u16 *work, Unk800F78A4 *env, Unk80100C78Entry *entry, s32 *arg4) {
    BtlfxExports *ex = BTLFX_EXPORTS(env);

    if (mode == 0) {
        *arg4 = 8;
    } else if (mode == 1) {
        D_801E7800 = env;
        ex->func_800DC818(0x10, 0, work);
        ex->func_800E3CEC(ex->btlfx_getFileData(entry->unk8, 1), (s32)work);
    } else {
        return entry->unk0 >= 0x40;
    }
    return 0;
}

INCLUDE_RODATA("asm/us/fx_192/nonmatchings/fx_192", fx_192_headerPtr);
