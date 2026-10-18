#include "fxmod/fx_1a4.h"

INCLUDE_ASM("asm/us/fx_1a4/nonmatchings/fx_1a4", fx_1a4_op80);

INCLUDE_ASM("asm/us/fx_1a4/nonmatchings/fx_1a4", fx_1a4_op81);

void func_801E8084(SVECTOR *a, SVECTOR *b, s32 t, SVECTOR *out) {
    BTLFX_EXPORTS(D_801E8168)->LoadAverageShort12(a, b, 0x1000 - t, t, out);
}

s32 func_801E80C8(s32 from, s32 to, s32 t, s32 n) {
    return (from * (n - t) + to * t) / n;
}

INCLUDE_RODATA("asm/us/fx_1a4/nonmatchings/fx_1a4", fx_1a4_headerPtr);
