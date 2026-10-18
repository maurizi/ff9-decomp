#include "fxmod/fx_1ec.h"

INCLUDE_ASM("asm/us/fx_1ec/nonmatchings/fx_1ec", func_801E78E0);

INCLUDE_ASM("asm/us/fx_1ec/nonmatchings/fx_1ec", func_801E79DC);

INCLUDE_ASM("asm/us/fx_1ec/nonmatchings/fx_1ec", fx_1ec_op80);

INCLUDE_RODATA("asm/us/fx_1ec/nonmatchings/fx_1ec", fx_1ec_headerPtr);

INCLUDE_ASM("asm/us/fx_1ec/nonmatchings/fx_1ec", fx_1ec_op81);

INCLUDE_ASM("asm/us/fx_1ec/nonmatchings/fx_1ec", fx_1ec_op82);

INCLUDE_ASM("asm/us/fx_1ec/nonmatchings/fx_1ec", func_801E8AFC);

void func_801E8B14(SVECTOR *a, SVECTOR *b, s32 t, SVECTOR *out) {
    BTLFX_EXPORTS(D_801E8E94)->LoadAverageShort12(a, b, 0x1000 - t, t, out);
}

s32 func_801E8B58(s32 from, s32 to, s32 t, s32 n) {
    return (from * (n - t) + to * t) / n;
}

INCLUDE_ASM("asm/us/fx_1ec/nonmatchings/fx_1ec", func_801E8BB0);

INCLUDE_ASM("asm/us/fx_1ec/nonmatchings/fx_1ec", func_801E8C48);

INCLUDE_ASM("asm/us/fx_1ec/nonmatchings/fx_1ec", func_801E8C60);

INCLUDE_ASM("asm/us/fx_1ec/nonmatchings/fx_1ec", func_801E8CC4);

INCLUDE_ASM("asm/us/fx_1ec/nonmatchings/fx_1ec", func_801E8D30);

INCLUDE_ASM("asm/us/fx_1ec/nonmatchings/fx_1ec", func_801E8DB4);

INCLUDE_ASM("asm/us/fx_1ec/nonmatchings/fx_1ec", func_801E8E00);
