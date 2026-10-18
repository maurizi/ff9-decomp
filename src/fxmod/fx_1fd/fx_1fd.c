#include "fxmod/fx_1fd.h"

INCLUDE_ASM("asm/us/fx_1fd/nonmatchings/fx_1fd", func_801E7704);

INCLUDE_ASM("asm/us/fx_1fd/nonmatchings/fx_1fd", func_801E7AF8);

INCLUDE_ASM("asm/us/fx_1fd/nonmatchings/fx_1fd", func_801E7E18);

INCLUDE_ASM("asm/us/fx_1fd/nonmatchings/fx_1fd", fx_1fd_op80);

INCLUDE_ASM("asm/us/fx_1fd/nonmatchings/fx_1fd", fx_1fd_op81);

INCLUDE_ASM("asm/us/fx_1fd/nonmatchings/fx_1fd", fx_1fd_op82);

INCLUDE_ASM("asm/us/fx_1fd/nonmatchings/fx_1fd", fx_1fd_op83);

INCLUDE_ASM("asm/us/fx_1fd/nonmatchings/fx_1fd", fx_1fd_op84);

INCLUDE_ASM("asm/us/fx_1fd/nonmatchings/fx_1fd", func_801E9710);

void func_801E9728(SVECTOR *a, SVECTOR *b, s32 t, SVECTOR *out) {
    BTLFX_EXPORTS(D_801E9E4C)->LoadAverageShort12(a, b, 0x1000 - t, t, out);
}

s32 func_801E976C(s32 from, s32 to, s32 t, s32 n) {
    return (from * (n - t) + to * t) / n;
}

INCLUDE_ASM("asm/us/fx_1fd/nonmatchings/fx_1fd", func_801E97C4);

INCLUDE_ASM("asm/us/fx_1fd/nonmatchings/fx_1fd", func_801E97DC);

INCLUDE_ASM("asm/us/fx_1fd/nonmatchings/fx_1fd", func_801E9840);

INCLUDE_ASM("asm/us/fx_1fd/nonmatchings/fx_1fd", func_801E9858);

INCLUDE_ASM("asm/us/fx_1fd/nonmatchings/fx_1fd", func_801E98EC);

INCLUDE_ASM("asm/us/fx_1fd/nonmatchings/fx_1fd", func_801E9984);

INCLUDE_ASM("asm/us/fx_1fd/nonmatchings/fx_1fd", func_801E9A50);

INCLUDE_ASM("asm/us/fx_1fd/nonmatchings/fx_1fd", func_801E9A9C);

INCLUDE_ASM("asm/us/fx_1fd/nonmatchings/fx_1fd", func_801E9AF8);

INCLUDE_ASM("asm/us/fx_1fd/nonmatchings/fx_1fd", func_801E9C58);

INCLUDE_RODATA("asm/us/fx_1fd/nonmatchings/fx_1fd", fx_1fd_headerPtr);
