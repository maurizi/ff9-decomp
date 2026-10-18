#include "fxmod/fx_0ff.h"

INCLUDE_ASM("asm/us/fx_0ff/nonmatchings/fx_0ff", fx_0ff_op80);

void func_801E7BCC(SVECTOR *a, SVECTOR *b, s32 t, SVECTOR *out) {
    BTLFX_EXPORTS(D_801E7CB8)->LoadAverageShort12(a, b, 0x1000 - t, t, out);
}

s32 func_801E7C10(s32 from, s32 to, s32 t, s32 n) {
    return (from * (n - t) + to * t) / n;
}

INCLUDE_RODATA("asm/us/fx_0ff/nonmatchings/fx_0ff", fx_0ff_headerPtr);
