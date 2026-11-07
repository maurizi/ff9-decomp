#include "fxmod/fx_1f9.h"

s32 fx_1f9_op80(s32 mode, void *work, Unk800F78A4 *env, Unk80100C78Entry *entry, s32 *args) {
    BtlfxExports *ex = BTLFX_EXPORTS(env);

    if (mode == 0) {
        *args = 0;
    } else if (mode == 1) {
        fx_1f9_emitPos0.vx = 0;
        fx_1f9_emitPos0.vy = 0;
        fx_1f9_emitPos0.vz = -0x618;
        fx_1f9_env = env;
        ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, FX_CARBUNCLE_ORBIT_GLOW_2), (s32)&fx_1f9_emitPos0);
    } else {
        return entry->frame >= FX_CARBUNCLE_GLOW_FRAMES;
    }
    return 0;
}

s32 fx_1f9_op81(s32 mode, void *work, Unk800F78A4 *env, Unk80100C78Entry *entry, s32 *args) {
    BtlfxExports *ex = BTLFX_EXPORTS(env);
    s32 i;

    if (mode == 0) {
        *args = 0;
    } else if (mode == 1) {
        fx_1f9_env = env;
        for (i = 0; i < env->targets->targetCount; i++) {
            ex->btlfx_getUnitBodyPos(i, 0, (s16 *)&fx_1f9_emitPos1[i]);
            ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, FX_CARBUNCLE_BLUE_RINGS), (s32)&fx_1f9_emitPos1[i]);
        }
    } else {
        return entry->frame >= FX_CARBUNCLE_RINGS_FRAMES;
    }
    return 0;
}

INCLUDE_RODATA("asm/us/fx_1f9/nonmatchings/fx_1f9", fx_1f9_headerPtr);
