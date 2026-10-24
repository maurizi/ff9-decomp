#include "fxmod/fx_1f9.h"

s32 fx_1f9_op80(s32 mode, void *work, Unk800F78A4 *env, Unk80100C78Entry *entry, s32 *arg4) {
    BtlfxExports *ex = BTLFX_EXPORTS(env);

    if (mode == 0) {
        *arg4 = 0;
    } else if (mode == 1) {
        D_801E78F0.vx = 0;
        D_801E78F0.vy = 0;
        D_801E78F0.vz = -0x618;
        D_801E78E8 = env;
        ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->unk8, 3), (s32)&D_801E78F0);
    } else {
        return entry->unk0 >= 0x40;
    }
    return 0;
}

s32 fx_1f9_op81(s32 mode, void *work, Unk800F78A4 *env, Unk80100C78Entry *entry, s32 *arg4) {
    BtlfxExports *ex = BTLFX_EXPORTS(env);
    s32 i;

    if (mode == 0) {
        *arg4 = 0;
    } else if (mode == 1) {
        D_801E78E8 = env;
        for (i = 0; i < env->targets->targetCount; i++) {
            ex->btlfx_getUnitBodyPos(i, 0, (s16 *)&D_801E78F8[i]);
            ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->unk8, 1), (s32)&D_801E78F8[i]);
        }
    } else {
        return entry->unk0 >= 0x30;
    }
    return 0;
}

INCLUDE_RODATA("asm/us/fx_1f9/nonmatchings/fx_1f9", fx_1f9_headerPtr);
