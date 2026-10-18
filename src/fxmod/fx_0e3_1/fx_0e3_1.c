#include "fxmod/fx_0e3_1.h"

INCLUDE_ASM("asm/us/fx_0e3_1/nonmatchings/fx_0e3_1", func_801EC71C);

INCLUDE_ASM("asm/us/fx_0e3_1/nonmatchings/fx_0e3_1", func_801EC910);

INCLUDE_ASM("asm/us/fx_0e3_1/nonmatchings/fx_0e3_1", func_801ECAF8);

INCLUDE_ASM("asm/us/fx_0e3_1/nonmatchings/fx_0e3_1", func_801ECCF4);

INCLUDE_ASM("asm/us/fx_0e3_1/nonmatchings/fx_0e3_1", func_801ECDEC);

INCLUDE_ASM("asm/us/fx_0e3_1/nonmatchings/fx_0e3_1", func_801ECFE4);

void func_801ED708(VECTOR *pos, SVECTOR *rot) {
    Unk800F78A4 *env = D_801F0A04;
    BtlfxExports *ex = BTLFX_EXPORTS(env);
    SVECTOR v;
    s32 tmp;
    s32 a;

    v.vx = pos->vx;
    v.vy = pos->vy;
    v.vz = pos->vz;
    ex->func_800D0674(&env->unk2C->unk54, &v, rot, (s32)&tmp, (s32)&tmp, &a);
    rot->vz = -a + 0x400;
}

INCLUDE_RODATA("asm/us/fx_0e3_1/nonmatchings/fx_0e3_1", fx_0e3_1_headerPtr);

INCLUDE_ASM("asm/us/fx_0e3_1/nonmatchings/fx_0e3_1", fx_0e3_1_op80);
