#include "fxmod/fx_1ed.h"

INCLUDE_ASM("asm/us/fx_1ed/nonmatchings/fx_1ed", func_801E7704);

INCLUDE_ASM("asm/us/fx_1ed/nonmatchings/fx_1ed", func_801E7738);

INCLUDE_ASM("asm/us/fx_1ed/nonmatchings/fx_1ed", func_801E7788);

s32 func_801E792C(s32 curve, s32 n, s32 t, s32 from, s32 to) {
    BtlfxExports *ex = BTLFX_EXPORTS(D_801E8910);
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

INCLUDE_ASM("asm/us/fx_1ed/nonmatchings/fx_1ed", fx_1ed_op80);

s32 fx_1ed_op81(s32 mode, void *work, Unk800F78A4 *env, Unk80100C78Entry *entry, s32 *arg4) {
    BtlfxExports *ex = BTLFX_EXPORTS(env);
    s32 i;

    if (mode == 0) {
        *arg4 = 0;
    } else if (mode == 1) {
        D_801E8910 = env;
        for (i = 0; i < env->unkC->unk24; i++) {
            ex->btlfx_getUnitBodyPos(i, 0, (s16 *)&D_801EA7E0[i]);
            ex->func_800E3CEC(ex->btlfx_getFileData(entry->unk8, 1), (s32)&D_801EA7E0[i]);
        }
    } else {
        return entry->unk0 >= 0x30;
    }
    return 0;
}

INCLUDE_RODATA("asm/us/fx_1ed/nonmatchings/fx_1ed", fx_1ed_headerPtr);
