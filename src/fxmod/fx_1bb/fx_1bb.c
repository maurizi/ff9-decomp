#include "fxmod/fx_1bb.h"

s32 fx_1bb_op80(s32 mode, SVECTOR *work, Unk800F78A4 *env, Unk80100C78Entry *entry, s32 *arg4) {
    BtlfxExports *ex = BTLFX_EXPORTS(env);

    if (mode == 0) {
        *arg4 = 8;
    } else if (mode == 1) {
        work->vx = env->targets->centerX;
        work->vz = env->targets->centerZ;
        work->vy = -0x190;
        D_801E7804 = env;
        ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->unk8, 1), (s32)work);
    } else {
        return entry->unk0 >= 0x20;
    }
    return 0;
}

INCLUDE_RODATA("asm/us/fx_1bb/nonmatchings/fx_1bb", fx_1bb_headerPtr);
