#include "fxmod/fx_1e6.h"

INCLUDE_ASM("asm/us/fx_1e6/nonmatchings/fx_1e6", func_801E7724);

INCLUDE_ASM("asm/us/fx_1e6/nonmatchings/fx_1e6", fx_1e6_op80);

INCLUDE_RODATA("asm/us/fx_1e6/nonmatchings/fx_1e6", fx_1e6_headerPtr);

INCLUDE_ASM("asm/us/fx_1e6/nonmatchings/fx_1e6", fx_1e6_op81);

void func_801E81E8(SVECTOR *a, SVECTOR *b, s32 t, SVECTOR *out) {
    BTLFX_EXPORTS(D_801E8348)->LoadAverageShort12(a, b, 0x1000 - t, t, out);
}

s32 func_801E822C(s32 from, s32 to, s32 t, s32 n) {
    return (from * (n - t) + to * t) / n;
}

INCLUDE_ASM("asm/us/fx_1e6/nonmatchings/fx_1e6", func_801E8284);

INCLUDE_ASM("asm/us/fx_1e6/nonmatchings/fx_1e6", func_801E829C);
