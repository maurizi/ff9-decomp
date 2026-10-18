#include "fxmod/fx_1af.h"

INCLUDE_ASM("asm/us/fx_1af/nonmatchings/fx_1af", fx_1af_op80);

s32 fx_1af_op83(s32 mode, void *work, Unk800F78A4 *env, Unk80100C78Entry *entry, s32 *arg4) {
    BtlfxExports *ex = BTLFX_EXPORTS(env);

    if (mode == 0) {
        *arg4 = 0;
    } else if (mode == 1) {
        D_801E7F48 = env;
        ex->func_800E8274(0x10);
    } else {
        return 1;
    }
    return 0;
}

INCLUDE_ASM("asm/us/fx_1af/nonmatchings/fx_1af", fx_1af_op87);

INCLUDE_RODATA("asm/us/fx_1af/nonmatchings/fx_1af", fx_1af_headerPtr);
