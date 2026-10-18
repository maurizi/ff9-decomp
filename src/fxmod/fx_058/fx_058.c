#include "fxmod/fx_058.h"

INCLUDE_ASM("asm/us/fx_058/nonmatchings/fx_058", func_801E7704);

INCLUDE_ASM("asm/us/fx_058/nonmatchings/fx_058", fx_058_op80);

s32 fx_058_op81(s32 mode, void *work, Unk800F78A4 *env, Unk80100C78Entry *entry, s32 *arg4) {
    BtlfxExports *ex = BTLFX_EXPORTS(env);

    if (mode == 1) {
        D_801E7E64 = env;
        ex->btlfx_clearEffModels();
    }
    return 1;
}

INCLUDE_RODATA("asm/us/fx_058/nonmatchings/fx_058", fx_058_headerPtr);
