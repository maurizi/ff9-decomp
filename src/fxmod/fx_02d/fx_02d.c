#include "fxmod/fx_02d.h"

s32 fx_02d_op80(s32 mode, SVECTOR *work, Unk800F78A4 *env, Unk80100C78Entry *entry, s32 *arg4) {
    BtlfxExports *ex = BTLFX_EXPORTS(env);

    if (mode == 0) {
        *arg4 = 8;
    } else if (mode == 1) {
        work->vx = env->targets->centerX;
        work->vy = -0xC8;
        work->vz = env->targets->centerZ;
        D_801E7838 = env;
        ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, 2), (s32)work);
        ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, 7), (s32)work);
    } else {
        return entry->frame >= 0x28;
    }
    return 0;
}

INCLUDE_RODATA("asm/us/fx_02d/nonmatchings/fx_02d", fx_02d_headerPtr);
