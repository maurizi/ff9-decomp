#include "fxmod/fx_1b6.h"

INCLUDE_ASM("asm/us/fx_1b6/nonmatchings/fx_1b6", func_801E7704);

INCLUDE_ASM("asm/us/fx_1b6/nonmatchings/fx_1b6", func_801E774C);

INCLUDE_ASM("asm/us/fx_1b6/nonmatchings/fx_1b6", func_801E7830);

INCLUDE_ASM("asm/us/fx_1b6/nonmatchings/fx_1b6", func_801E7868);

INCLUDE_ASM("asm/us/fx_1b6/nonmatchings/fx_1b6", func_801E7AAC);

INCLUDE_ASM("asm/us/fx_1b6/nonmatchings/fx_1b6", fx_1b6_op80);

s32 fx_1b6_op83(s32 mode, void *work, Unk800F78A4 *env, Unk80100C78Entry *entry, s32 *arg4) {
    BtlfxExports *ex = BTLFX_EXPORTS(env);

    if (mode == 0) {
        *arg4 = 0;
    } else if (mode == 1) {
        D_801E85B4 = env;
        ex->func_800E8274(0x10);
    } else {
        return 1;
    }
    return 0;
}

INCLUDE_ASM("asm/us/fx_1b6/nonmatchings/fx_1b6", fx_1b6_op87);

INCLUDE_RODATA("asm/us/fx_1b6/nonmatchings/fx_1b6", fx_1b6_headerPtr);
