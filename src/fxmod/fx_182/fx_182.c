#include "fxmod/fx_182.h"

s32 fx_182_op80(s32 mode, SVECTOR *work, Unk800F78A4 *env, Unk80100C78Entry *entry, s32 *args) {
    BtlfxExports *ex = BTLFX_EXPORTS(env);
    Pool6CEntry *emitter;
    s32 frame;

    if (mode == 0) {
        *args = 8;
    } else if (mode == 1) {
        work->vx = env->targets->centerX;
        work->vz = env->targets->centerZ;
        work->vy = -0x200;
        fx_182_env = env;
        emitter = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, FX_COUNTDOWN_GOLD_SHARDS), (s32)work);
        if (emitter != NULL) {
            emitter->otZ = 0x40;
        }
    } else {
        frame = entry->frame;
        if (frame == 0x28) {
            emitter = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, FX_COUNTDOWN_LENS_FLASH), (s32)work);
            if (emitter != NULL) {
                emitter->otZ = 0x40;
            }
        }
        if (frame >= 0x28) {
            ex->btlfx_drawBackdrop((BtlBgLayers *)ex->btlfx_getFileData(entry->file, FX_COUNTDOWN_BACKDROP), frame);
        }
        return frame >= 0x50;
    }
    return 0;
}

INCLUDE_RODATA("asm/us/fx_182/nonmatchings/fx_182", fx_182_headerPtr);
