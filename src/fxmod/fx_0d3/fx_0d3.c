#include "fxmod/fx_0d3.h"

INCLUDE_ASM("asm/us/fx_0d3/nonmatchings/fx_0d3", func_801E7720);

INCLUDE_ASM("asm/us/fx_0d3/nonmatchings/fx_0d3", func_801E7CBC);

INCLUDE_ASM("asm/us/fx_0d3/nonmatchings/fx_0d3", func_801E8470);

INCLUDE_ASM("asm/us/fx_0d3/nonmatchings/fx_0d3", func_801E84C0);

INCLUDE_ASM("asm/us/fx_0d3/nonmatchings/fx_0d3", func_801E8548);

INCLUDE_ASM("asm/us/fx_0d3/nonmatchings/fx_0d3", func_801E85FC);

INCLUDE_ASM("asm/us/fx_0d3/nonmatchings/fx_0d3", func_801E8628);

void func_801E8A10(s32 a0, s32 frame, s32 a2, s32 a3, s16 *v) {
    frame += 7;
    v[0] = (v[0] * frame) >> 3;
    v[1] = (v[1] * frame) >> 3;
}

INCLUDE_ASM("asm/us/fx_0d3/nonmatchings/fx_0d3", fx_0d3_op80);

s32 func_801E9050(SVECTOR *v) {
    s32 i = D_801EB098;
    Unk800F78A4 *env = D_801ED534;
    BtlfxExports *ex = BTLFX_EXPORTS(env);

    v->pad = 1;
    ex->Hi_GetSummonBonePos(((s32 *)env->unk4)[0x60 / 4], D_801EB084[i], (u16 *)v);
    i++;
    if (i >= 5) {
        i = 0;
    }
    D_801EB098 = i;
    return 0;
}

void func_801E90D4(s16 *v) {
    s32 t = (BTLFX_EXPORTS(D_801ED534)->math_sin(D_801EDF48 + (v[1] << 8)) * D_801EDF44) >> 12;
    s32 x = v[0];

    v[0] = x + t;
}

INCLUDE_RODATA("asm/us/fx_0d3/nonmatchings/fx_0d3", fx_0d3_headerPtr);

INCLUDE_ASM("asm/us/fx_0d3/nonmatchings/fx_0d3", fx_0d3_op81);

s32 fx_0d3_op85(s32 mode, void *work, Unk800F78A4 *env, Unk80100C78Entry *entry, s32 *arg4) {
    if (mode == 0) {
        *arg4 = 0;
    } else if (mode == 1) {
        D_801ED534 = env;
        env->unk2C->unk52 = 0;
    } else {
        return 1;
    }
    return 0;
}

s32 fx_0d3_op87(s32 mode, SVECTOR *work, Unk800F78A4 *env, Unk80100C78Entry *entry, s32 *arg4) {
    BtlfxExports *ex = BTLFX_EXPORTS(env);
    s32 frame;

    if (mode == 0) {
        *arg4 = 0x10;
    } else if (mode == 1) {
        D_801ED534 = env;
    } else {
        frame = entry->unk0;
        func_801E8470((u8 *)ex->btlfx_getFileData(entry->unk8, D_801EB0C4[1]), frame, &work[0]);
        env->unk2C->unk0[1].vx = work[0].vx;
        env->unk2C->unk0[1].vy = work[0].vy;
        env->unk2C->unk0[1].vz = work[0].vz;
        func_801E8470((u8 *)ex->btlfx_getFileData(entry->unk8, D_801EB0C4[0]), frame, &work[1]);
        env->unk2C->unk0[0].vx = work[1].vx;
        env->unk2C->unk0[0].vy = work[1].vy;
        env->unk2C->unk0[0].vz = work[1].vz;
        if (frame >= D_801EB0C4[2]) {
            return 1;
        }
    }
    return 0;
}
