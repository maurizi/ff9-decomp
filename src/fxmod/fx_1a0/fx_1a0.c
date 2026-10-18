#include "fxmod/fx_1a0.h"

INCLUDE_ASM("asm/us/fx_1a0/nonmatchings/fx_1a0", func_801E7704);

INCLUDE_ASM("asm/us/fx_1a0/nonmatchings/fx_1a0", func_801E7748);

INCLUDE_ASM("asm/us/fx_1a0/nonmatchings/fx_1a0", func_801E778C);

INCLUDE_ASM("asm/us/fx_1a0/nonmatchings/fx_1a0", func_801E77D0);

INCLUDE_ASM("asm/us/fx_1a0/nonmatchings/fx_1a0", func_801E7814);

INCLUDE_ASM("asm/us/fx_1a0/nonmatchings/fx_1a0", func_801E7858);

s32 func_801E789C(Pool30Entry *e, SVECTOR *pos, Fx1a0Path *path, s32 t, s32 frame, s32 frames) {
    SVECTOR v;

    func_801E7E94(path, t, &v);
    v.vx += D_801E8280.vx;
    v.vy += D_801E8280.vy;
    v.vz += D_801E8280.vz;
    pos->vx = v.vx;
    pos->vy = v.vy;
    pos->vz = v.vz;
    e->unk0.vx = pos->vx << 12;
    e->unk0.vy = pos->vy << 12;
    e->unk0.vz = pos->vz << 12;
    if (frame >= frames) {
        return -1;
    }
    return 0;
}

INCLUDE_ASM("asm/us/fx_1a0/nonmatchings/fx_1a0", fx_1a0_op80);

INCLUDE_ASM("asm/us/fx_1a0/nonmatchings/fx_1a0", fx_1a0_op81);

s32 fx_1a0_op82(s32 mode, Fx1a0Work82 *work, Unk800F78A4 *env, Unk80100C78Entry *entry, s32 *arg4) {
    BtlfxExports *ex = BTLFX_EXPORTS(env);

    if (mode == 0) {
        *arg4 = 0xC;
    } else if (mode == 1) {
        D_801E7F88 = env;
        ex->btlfx_getUnitBodyPos(0, 0, (s16 *)&work->unk4);
        work->unk0 = ex->func_800E3CEC(ex->btlfx_getFileData(entry->unk8, 0x13), (s32)&work->unk4);
    } else {
        return entry->unk0 >= 0x14;
    }
    return 0;
}

INCLUDE_ASM("asm/us/fx_1a0/nonmatchings/fx_1a0", func_801E7E7C);

INCLUDE_ASM("asm/us/fx_1a0/nonmatchings/fx_1a0", func_801E7E94);

INCLUDE_RODATA("asm/us/fx_1a0/nonmatchings/fx_1a0", fx_1a0_headerPtr);
