#include "fxmod/fx_09e.h"

s32 fx_09e_op80(s32 mode, Fx09eWork *work, Unk800F78A4 *env, Unk80100C78Entry *entry, s32 *args) {
    BtlfxExports *ex = BTLFX_EXPORTS(env);
    Pool6CEntry *emitter;
    s32 frame;

    if (mode == 0) {
        *args = sizeof(Fx09eWork);
    } else if (mode == 1) {
        fx_09e_env = env;
        ex->btlfx_getUnitBodyPos(0, 0, (s16 *)&work->pos0);
        emitter = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, FX_BLIND_VORTEX_2), (s32)&work->pos0);
        if (emitter != NULL) {
            emitter->otZ = ex->btlfx_addRadiusSixth(0, 0x80);
        }
    } else {
        frame = entry->frame;
        if (frame == FX_BLIND_VORTEX_FRAME) {
            emitter = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, FX_BLIND_VORTEX), (s32)&work->pos0);
            if (emitter != NULL) {
                emitter->otZ = ex->btlfx_addRadiusSixth(0, 0x80);
            }
        }
        return frame >= FX_BLIND_FRAMES;
    }
    return 0;
}

INCLUDE_RODATA("asm/us/fx_09e/nonmatchings/fx_09e", fx_09e_headerPtr);
