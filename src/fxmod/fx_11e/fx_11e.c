#include "fxmod/fx_11e.h"

INCLUDE_ASM("asm/us/fx_11e/nonmatchings/fx_11e", fx_11e_op80);

void func_801E7F70(Unk800F78A4 *env, s32 i, s32 v) {
    *((u8 *)env->unk0 + i + 0x10) = v;
}

INCLUDE_RODATA("asm/us/fx_11e/nonmatchings/fx_11e", fx_11e_headerPtr);
