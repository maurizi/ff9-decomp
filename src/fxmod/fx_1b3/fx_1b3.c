#include "fxmod/fx_1b3.h"

INCLUDE_ASM("asm/us/fx_1b3/nonmatchings/fx_1b3", func_801E773C);

s32 func_801E77CC(s32 curve, s32 n, s32 t, s32 from, s32 to) {
    BtlfxExports *ex = BTLFX_EXPORTS(D_801EB074);
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

INCLUDE_ASM("asm/us/fx_1b3/nonmatchings/fx_1b3", fx_1b3_op80);

INCLUDE_RODATA("asm/us/fx_1b3/nonmatchings/fx_1b3", fx_1b3_headerPtr);

INCLUDE_ASM("asm/us/fx_1b3/nonmatchings/fx_1b3", func_801EA39C);

INCLUDE_ASM("asm/us/fx_1b3/nonmatchings/fx_1b3", func_801EA7E4);
