#include "fxmod/fx_097.h"

INCLUDE_ASM("asm/us/fx_097/nonmatchings/fx_097", func_801E7720);

INCLUDE_ASM("asm/us/fx_097/nonmatchings/fx_097", func_801E7CBC);

void func_801E8284(s16 *a, s16 *b, s32 i, s16 *out) {
    BTLFX_EXPORTS(D_801E9DB4)->func_800CFD78(a, b, -((D_801E9D50[i] * 0x455) >> 9), out);
}

INCLUDE_ASM("asm/us/fx_097/nonmatchings/fx_097", func_801E82F4);

INCLUDE_RODATA("asm/us/fx_097/nonmatchings/fx_097", fx_097_headerPtr);

INCLUDE_ASM("asm/us/fx_097/nonmatchings/fx_097", fx_097_op80);
