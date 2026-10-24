#include "fxmod/fx_0c4.h"

s32 fx_0c4_op80(s32 mode, Fx0c4Work *work, Unk800F78A4 *env, Unk80100C78Entry *entry, s32 *arg4) {
    BtlfxExports *ex = BTLFX_EXPORTS(env);

    if (mode == 0) {
        *arg4 = 0x10;
    } else if (mode == 1) {
        work->unk8.vx = env->targets->centerX;
        work->unk8.vy = env->targets->centerY;
        work->unk8.vz = env->targets->centerZ;
        D_801E785C = env;
        work->unk0 = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, 6), (s32)&work->unk8);
        work->unk4 = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, 8), (s32)&work->unk8);
    } else {
        return entry->frame >= 0x50;
    }
    return 0;
}

INCLUDE_RODATA("asm/us/fx_0c4/nonmatchings/fx_0c4", fx_0c4_headerPtr);
