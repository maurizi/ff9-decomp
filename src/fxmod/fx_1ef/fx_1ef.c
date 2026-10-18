#include "fxmod/fx_1ef.h"

INCLUDE_ASM("asm/us/fx_1ef/nonmatchings/fx_1ef", func_801E7704);

INCLUDE_ASM("asm/us/fx_1ef/nonmatchings/fx_1ef", func_801E7738);

INCLUDE_ASM("asm/us/fx_1ef/nonmatchings/fx_1ef", func_801E7788);

s32 func_801E792C(s32 curve, s32 n, s32 t, s32 from, s32 to) {
    BtlfxExports *ex = BTLFX_EXPORTS(D_801E8B9C);
    s32 k = 0x20000000 / (n * n);
    s32 r = 0;

    switch (curve) {
    case 0:
        r = 0x1000 - (((0x10000000 / n) * t) >> 16);
        break;
    case 1:
        r = (k * (n - t) * (n - t)) >> 17;
        break;
    case 2:
        r = 0x1000 - ((k * t * t) >> 17);
        break;
    }
    return ex->btlfx_lerp(from, to, r);
}

INCLUDE_ASM("asm/us/fx_1ef/nonmatchings/fx_1ef", fx_1ef_op80);

INCLUDE_ASM("asm/us/fx_1ef/nonmatchings/fx_1ef", fx_1ef_op81);

INCLUDE_RODATA("asm/us/fx_1ef/nonmatchings/fx_1ef", fx_1ef_headerPtr);
