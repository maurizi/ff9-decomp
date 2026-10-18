#include "fxmod/fx_19c.h"

INCLUDE_ASM("asm/us/fx_19c/nonmatchings/fx_19c", fx_19c_op80);

s32 fx_19c_op81(s32 mode, SVECTOR *work, Unk800F78A4 *env, Unk80100C78Entry *entry, s32 *arg4) {
    BtlfxExports *ex = BTLFX_EXPORTS(env);
    s32 frame;

    if (mode == 0) {
        *arg4 = 8;
    } else if (mode == 1) {
        D_801E7970 = env;
    } else {
        frame = entry->unk0;
        ex->btlfx_getUnitPos(0x10, (s16 *)work);
        env->unk2C->unk0[0].vx = work->vx;
        env->unk2C->unk0[0].vy = work->vy;
        env->unk2C->unk0[0].vz = work->vz;
        return frame >= 7;
    }
    return 0;
}

INCLUDE_RODATA("asm/us/fx_19c/nonmatchings/fx_19c", fx_19c_headerPtr);
