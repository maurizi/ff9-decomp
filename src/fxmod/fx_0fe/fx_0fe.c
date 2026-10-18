#include "fxmod/fx_0fe.h"

INCLUDE_ASM("asm/us/fx_0fe/nonmatchings/fx_0fe", fx_0fe_op80);

void func_801E7B58(SVECTOR *a, SVECTOR *b, s32 t, SVECTOR *out) {
    BTLFX_EXPORTS(D_801E7C44)->LoadAverageShort12(a, b, 0x1000 - t, t, out);
}

s32 func_801E7B9C(s32 from, s32 to, s32 t, s32 n) {
    return (from * (n - t) + to * t) / n;
}

INCLUDE_RODATA("asm/us/fx_0fe/nonmatchings/fx_0fe", fx_0fe_headerPtr);
