#include "fxmod/fx_0f9.h"

s32 fx_0f9_op80(s32 mode, SVECTOR *work, Unk800F78A4 *env, Unk80100C78Entry *entry, s32 *args) {
    BtlfxExports *ex = BTLFX_EXPORTS(env);
    Pool6CEntry *emitter;

    if (mode == 0) {
        *args = 8;
    } else if (mode == 1) {
        work->vx = env->targets->centerX;
        work->vy = 0;
        work->vz = env->targets->centerZ;
        fx_0f9_env = env;
        emitter = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, FX_FLEE_SPARKLE_BURST), (s32)work);
        if (emitter != NULL) {
            emitter->otZ = ex->btlfx_addRadiusSixth(0x10, 0x80);
        }
    } else {
        return entry->frame >= 0x40;
    }
    return 0;
}

INCLUDE_RODATA("asm/us/fx_0f9/nonmatchings/fx_0f9", fx_0f9_headerPtr);
