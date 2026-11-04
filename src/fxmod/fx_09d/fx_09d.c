#include "fxmod/fx_09d.h"

s32 fx_09d_op80(s32 mode, Fx09dWork *work, Unk800F78A4 *env, Unk80100C78Entry *entry, s32 *args) {
    BtlfxExports *ex = BTLFX_EXPORTS(env);
    Pool6CEntry *emitter;
    s32 frame;

    if (mode == 0) {
        *args = 8;
    } else if (mode == 1) {
        work->pos0.vx = env->targets->centerX;
        work->pos0.vy = -0x190;
        work->pos0.vz = env->targets->centerZ;
        fx_09d_env = env;
        emitter = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, FX_BLIND_VORTEX_2), (s32)&work->pos0);
        if (emitter != NULL) {
            emitter->otZ = ex->btlfx_addRadiusSixth(0, 0x80);
        }
    } else {
        frame = entry->frame;
        if (frame == 0x18) {
            emitter = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, FX_BLIND_VORTEX), (s32)&work->pos0);
            if (emitter != NULL) {
                emitter->otZ = ex->btlfx_addRadiusSixth(0, 0x80);
            }
        }
        return frame >= 0x38;
    }
    return 0;
}

INCLUDE_RODATA("asm/us/fx_09d/nonmatchings/fx_09d", fx_09d_headerPtr);
