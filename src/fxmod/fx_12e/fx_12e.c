#include "fxmod/fx_12e.h"

s32 fx_12e_op80(s32 mode, void *work, Unk800F78A4 *env, Unk80100C78Entry *entry, s32 *arg4) {
    BtlfxExports *ex = BTLFX_EXPORTS(env);

    if (mode == 0) {
        *arg4 = 0;
    } else if (mode == 1) {
        D_801E77A8 = env;
    } else {
        ex->btlfx_fadeUnitById(0, -1);
        return 1;
    }
    return 0;
}

INCLUDE_RODATA("asm/us/fx_12e/nonmatchings/fx_12e", fx_12e_headerPtr);
