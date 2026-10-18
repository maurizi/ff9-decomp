#include "fxmod/fx_122.h"

INCLUDE_ASM("asm/us/fx_122/nonmatchings/fx_122", fx_122_op80);

void func_801E7BF8(SVECTOR *a, SVECTOR *b, s32 t, SVECTOR *out) {
    BTLFX_EXPORTS(D_801E7D44)->LoadAverageShort12(a, b, 0x1000 - t, t, out);
}

s32 func_801E7C3C(s32 from, s32 to, s32 t, s32 n) {
    return (from * (n - t) + to * t) / n;
}

INCLUDE_RODATA("asm/us/fx_122/nonmatchings/fx_122", fx_122_headerPtr);
