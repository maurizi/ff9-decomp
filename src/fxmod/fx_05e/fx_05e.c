#include "fxmod/fx_05e.h"

INCLUDE_ASM("asm/us/fx_05e/nonmatchings/fx_05e", func_801E7704);

/* transform a point by a bone of the summon model, then restore the camera matrix */
void func_801E7B74(s32 model, s32 bone, SVECTOR *in, SVECTOR *out) {
    Unk800F78A4 *env = D_801E8E74;
    MATRIX m;

    BTLFX_EXPORTS(env)->Hi_GetSummonBoneMatrix(model, bone, (BtlBone *)&m);
    gte_SetRotMatrix(&m);
    gte_SetTransMatrix(&m);
    gte_ldv0(in);
    gte_rtv0tr();
    gte_stlvnl(m.t);
    out->vx = m.t[0];
    out->vy = m.t[1];
    out->vz = m.t[2];
    gte_SetRotMatrix(env->unk14);
    gte_SetTransMatrix(env->unk14);
}

INCLUDE_ASM("asm/us/fx_05e/nonmatchings/fx_05e", fx_05e_op80);

INCLUDE_RODATA("asm/us/fx_05e/nonmatchings/fx_05e", fx_05e_headerPtr);
