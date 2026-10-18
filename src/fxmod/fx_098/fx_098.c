#include "fxmod/fx_098.h"

INCLUDE_ASM("asm/us/fx_098/nonmatchings/fx_098", func_801E7720);

INCLUDE_ASM("asm/us/fx_098/nonmatchings/fx_098", func_801E7CBC);

void func_801E8284(s16 *a, s16 *b, s32 i, s16 *out) {
    BTLFX_EXPORTS(D_801E9D1C)->func_800CFD78(a, b, -(D_801E9CB8[i] * 3), out);
}

INCLUDE_ASM("asm/us/fx_098/nonmatchings/fx_098", func_801E82DC);

INCLUDE_RODATA("asm/us/fx_098/nonmatchings/fx_098", fx_098_headerPtr);

INCLUDE_ASM("asm/us/fx_098/nonmatchings/fx_098", fx_098_op80);
