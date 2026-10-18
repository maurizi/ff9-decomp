#include "fxmod/fx_19e.h"

INCLUDE_ASM("asm/us/fx_19e/nonmatchings/fx_19e", fx_19e_op80);

s32 fx_19e_op81(s32 mode, Fx19eWork81 *work, Unk800F78A4 *env, Unk80100C78Entry *entry, s32 *arg4) {
    BtlfxExports *ex = BTLFX_EXPORTS(env);

    if (mode == 0) {
        *arg4 = 0x30;
    } else if (mode == 1) {
        D_801E7EC8 = env;
        ex->btlfx_getUnitBodyPos(0x10, 0, (s16 *)&work->unk0);
        work->unk18.vx = env->unkC->unk38;
        work->unk18.vy = env->unkC->unk3A;
        work->unk18.vz = env->unkC->unk3C;
        ex->btlfx_aimAngles((s16 *)&work->unk0, (s16 *)&work->unk18, (s16 *)&work->unk8);
        work->unk8.vy += 0x800;
        ex->func_800CFD78((s16 *)&work->unk18, (s16 *)&work->unk8, 0x514, (s16 *)&work->unk10);
    } else {
        s32 frame = entry->unk0;
        return func_801E7990(0x10, &work->unk10, frame, 7, 0, &work->unk20) != 0;
    }
    return 0;
}

void func_801E78F4(SVECTOR *a, SVECTOR *b, s32 t, SVECTOR *out) {
    BTLFX_EXPORTS(D_801E7EC8)->LoadAverageShort12(a, b, 0x1000 - t, t, out);
}

s32 func_801E7938(s32 from, s32 to, s32 t, s32 n) {
    return (from * (n - t) + to * t) / n;
}

INCLUDE_ASM("asm/us/fx_19e/nonmatchings/fx_19e", func_801E7990);

INCLUDE_ASM("asm/us/fx_19e/nonmatchings/fx_19e", func_801E7AAC);

INCLUDE_ASM("asm/us/fx_19e/nonmatchings/fx_19e", func_801E7C28);

INCLUDE_RODATA("asm/us/fx_19e/nonmatchings/fx_19e", fx_19e_headerPtr);
