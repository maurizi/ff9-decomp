#include "fxmod/fx_1cc.h"

INCLUDE_ASM("asm/us/fx_1cc/nonmatchings/fx_1cc", func_801E7720);

INCLUDE_ASM("asm/us/fx_1cc/nonmatchings/fx_1cc", func_801E7CBC);

void func_801E8284(s16 *from, s16 *to, s32 i, s16 *out) {
    BTLFX_EXPORTS(D_801E9C34)->func_800CFD78(from, to, -(D_801E9BD0[i] * 4), out);
}

INCLUDE_ASM("asm/us/fx_1cc/nonmatchings/fx_1cc", func_801E82D0);

INCLUDE_RODATA("asm/us/fx_1cc/nonmatchings/fx_1cc", fx_1cc_headerPtr);

INCLUDE_ASM("asm/us/fx_1cc/nonmatchings/fx_1cc", fx_1cc_op80);
