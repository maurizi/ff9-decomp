#include "fxmod/fx_1bd.h"

INCLUDE_ASM("asm/us/fx_1bd/nonmatchings/fx_1bd", func_801E77C0);

INCLUDE_ASM("asm/us/fx_1bd/nonmatchings/fx_1bd", fx_1bd_op80);

INCLUDE_ASM("asm/us/fx_1bd/nonmatchings/fx_1bd", fx_1bd_op81);

INCLUDE_RODATA("asm/us/fx_1bd/nonmatchings/fx_1bd", fx_1bd_headerPtr);

INCLUDE_ASM("asm/us/fx_1bd/nonmatchings/fx_1bd", fx_1bd_op82);

INCLUDE_ASM("asm/us/fx_1bd/nonmatchings/fx_1bd", fx_1bd_op83);

void func_801E8D5C(SVECTOR *a, SVECTOR *b, s32 t, SVECTOR *out) {
    BTLFX_EXPORTS(D_801E9E54)->LoadAverageShort12(a, b, 0x1000 - t, t, out);
}

s32 func_801E8DA0(s32 from, s32 to, s32 t, s32 n) {
    return (from * (n - t) + to * t) / n;
}

INCLUDE_ASM("asm/us/fx_1bd/nonmatchings/fx_1bd", func_801E8DF8);

INCLUDE_ASM("asm/us/fx_1bd/nonmatchings/fx_1bd", func_801E8E0C);

INCLUDE_ASM("asm/us/fx_1bd/nonmatchings/fx_1bd", func_801E8E78);

INCLUDE_ASM("asm/us/fx_1bd/nonmatchings/fx_1bd", func_801E8E90);

INCLUDE_ASM("asm/us/fx_1bd/nonmatchings/fx_1bd", func_801E8EF4);

INCLUDE_ASM("asm/us/fx_1bd/nonmatchings/fx_1bd", func_801E8F08);

INCLUDE_ASM("asm/us/fx_1bd/nonmatchings/fx_1bd", func_801E8FA4);

INCLUDE_ASM("asm/us/fx_1bd/nonmatchings/fx_1bd", func_801E8FB8);

INCLUDE_ASM("asm/us/fx_1bd/nonmatchings/fx_1bd", func_801E901C);

INCLUDE_ASM("asm/us/fx_1bd/nonmatchings/fx_1bd", func_801E9034);

INCLUDE_ASM("asm/us/fx_1bd/nonmatchings/fx_1bd", func_801E90CC);

INCLUDE_ASM("asm/us/fx_1bd/nonmatchings/fx_1bd", func_801E9118);

INCLUDE_ASM("asm/us/fx_1bd/nonmatchings/fx_1bd", func_801E9174);

INCLUDE_ASM("asm/us/fx_1bd/nonmatchings/fx_1bd", func_801E92D4);

INCLUDE_ASM("asm/us/fx_1bd/nonmatchings/fx_1bd", func_801E9874);
