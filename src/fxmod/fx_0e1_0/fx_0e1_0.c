#include "fxmod/fx_0e1_0.h"

INCLUDE_ASM("asm/us/fx_0e1_0/nonmatchings/fx_0e1_0", func_801E7718);

INCLUDE_ASM("asm/us/fx_0e1_0/nonmatchings/fx_0e1_0", func_801E7CB4);

INCLUDE_ASM("asm/us/fx_0e1_0/nonmatchings/fx_0e1_0", func_801E80A8);

s32 func_801E8844(SVECTOR *out) {
    Unk800F78A4 *env = D_801E9E38;
    BtlfxExports *ex = BTLFX_EXPORTS(env);
    s32 n;

    if (D_801E9DBC != 0) {
        out->pad = 1;
        n = D_801E9DB8;
        ex->Hi_GetSummonBonePos(((Fx0e10EnvB *)env->unk4)->unk60, D_801E9D84[n++], (u16 *)out);
        if (n >= 5) {
            n = 0;
        }
        D_801E9DB8 = n;
    } else {
        out->vx = 10000;
        out->vy = 10000;
        out->vz = 10000;
        out->pad = 1;
    }
    return 0;
}

void func_801E88F0(s32 arg0, s32 arg1, s32 arg2, s32 arg3, s16 *v) {
    v[0] = (v[0] * 5) >> 2;
    v[1] = (v[1] * 5) >> 2;
}

INCLUDE_RODATA("asm/us/fx_0e1_0/nonmatchings/fx_0e1_0", fx_0e1_0_headerPtr);

INCLUDE_ASM("asm/us/fx_0e1_0/nonmatchings/fx_0e1_0", fx_0e1_0_op80);
