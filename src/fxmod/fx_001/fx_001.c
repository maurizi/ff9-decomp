#include "fxmod/fx_001.h"

INCLUDE_RODATA("asm/us/fx_001/nonmatchings/fx_001", fx_001_headerPtr);

INCLUDE_ASM("asm/us/fx_001/nonmatchings/fx_001", fx_001_op80);

void func_801E7D00(SVECTOR *a, SVECTOR *b, s32 t, SVECTOR *out) {
    BTLFX_EXPORTS(D_801E7E20)->LoadAverageShort12(a, b, 0x1000 - t, t, out);
}

s32 func_801E7D44(s32 from, s32 to, s32 t, s32 n) {
    return (from * (n - t) + to * t) / n;
}
