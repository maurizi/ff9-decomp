#include "fxmod/fx_1ea.h"

INCLUDE_ASM("asm/us/fx_1ea/nonmatchings/fx_1ea", func_801E7804);

INCLUDE_ASM("asm/us/fx_1ea/nonmatchings/fx_1ea", func_801E79D0);

INCLUDE_ASM("asm/us/fx_1ea/nonmatchings/fx_1ea", fx_1ea_op80);

INCLUDE_RODATA("asm/us/fx_1ea/nonmatchings/fx_1ea", fx_1ea_headerPtr);

INCLUDE_ASM("asm/us/fx_1ea/nonmatchings/fx_1ea", fx_1ea_op81);

INCLUDE_ASM("asm/us/fx_1ea/nonmatchings/fx_1ea", fx_1ea_op82);

void func_801E8B00(SVECTOR *a, SVECTOR *b, s32 t, SVECTOR *out) {
    BTLFX_EXPORTS(D_801E8DD8)->LoadAverageShort12(a, b, 0x1000 - t, t, out);
}

s32 func_801E8B44(s32 from, s32 to, s32 t, s32 n) {
    return (from * (n - t) + to * t) / n;
}

INCLUDE_ASM("asm/us/fx_1ea/nonmatchings/fx_1ea", func_801E8B9C);

INCLUDE_ASM("asm/us/fx_1ea/nonmatchings/fx_1ea", func_801E8C34);

INCLUDE_ASM("asm/us/fx_1ea/nonmatchings/fx_1ea", func_801E8C4C);

INCLUDE_ASM("asm/us/fx_1ea/nonmatchings/fx_1ea", func_801E8CB0);

INCLUDE_ASM("asm/us/fx_1ea/nonmatchings/fx_1ea", func_801E8CC8);
