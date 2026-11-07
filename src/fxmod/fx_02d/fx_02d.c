#include "fxmod/fx_02d.h"

s32 fx_02d_op80(s32 mode, SVECTOR *work, Unk800F78A4 *env, Unk80100C78Entry *entry, s32 *args) {
    BtlfxExports *ex = BTLFX_EXPORTS(env);

    if (mode == 0) {
        *args = sizeof(SVECTOR);
    } else if (mode == 1) {
        work->vx = env->targets->centerX;
        work->vy = -FX_ANGELS_SNACK_HEIGHT;
        work->vz = env->targets->centerZ;
        fx_02d_env = env;
        ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, FX_ANGELS_SNACK_OLIVE_RINGS), (s32)work);
        ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, FX_ANGELS_SNACK_GOLD_FLARES), (s32)work);
    } else {
        return entry->frame >= FX_ANGELS_SNACK_FRAMES;
    }
    return 0;
}

INCLUDE_RODATA("asm/us/fx_02d/nonmatchings/fx_02d", fx_02d_headerPtr);
