#include "fxmod/fx_1b7.h"

INCLUDE_ASM("asm/us/fx_1b7/nonmatchings/fx_1b7", func_801E7704);

INCLUDE_ASM("asm/us/fx_1b7/nonmatchings/fx_1b7", func_801E774C);

INCLUDE_ASM("asm/us/fx_1b7/nonmatchings/fx_1b7", func_801E7830);

INCLUDE_ASM("asm/us/fx_1b7/nonmatchings/fx_1b7", func_801E7868);

INCLUDE_ASM("asm/us/fx_1b7/nonmatchings/fx_1b7", func_801E7ACC);

INCLUDE_ASM("asm/us/fx_1b7/nonmatchings/fx_1b7", fx_1b7_op80);

s32 fx_1b7_op83(s32 mode, void *work, Unk800F78A4 *env, Unk80100C78Entry *entry, s32 *arg4) {
    BtlfxExports *ex = BTLFX_EXPORTS(env);

    if (mode == 0) {
        *arg4 = 0;
    } else if (mode == 1) {
        D_801E87E0 = env;
        ex->func_800E8274(0x10);
    } else {
        return 1;
    }
    return 0;
}

INCLUDE_RODATA("asm/us/fx_1b7/nonmatchings/fx_1b7", fx_1b7_headerPtr);
