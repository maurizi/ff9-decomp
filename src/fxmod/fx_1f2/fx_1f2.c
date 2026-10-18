#include "fxmod/fx_1f2.h"

void func_801E7704(s32 t, s32 n, s32 from, s32 to, s32 arg4, s32 ot) {
    Unk800F78A4 *env = D_801EA260;
    BtlfxExports *ex = BTLFX_EXPORTS(env);
    s32 c = ex->btlfx_lerpRatio(from, to, t, n);

    ex->btlfx_drawScreenTint((u32 *)(env->unk20 + ot), arg4, c, c, c);
}

s32 func_801E7794(s32 curve, s32 n, s32 t, s32 from, s32 to) {
    BtlfxExports *ex = BTLFX_EXPORTS(D_801EA260);
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

INCLUDE_ASM("asm/us/fx_1f2/nonmatchings/fx_1f2", fx_1f2_op80);

INCLUDE_RODATA("asm/us/fx_1f2/nonmatchings/fx_1f2", fx_1f2_headerPtr);
