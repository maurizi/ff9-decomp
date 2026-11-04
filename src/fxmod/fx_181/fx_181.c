#include "fxmod/fx_181.h"

s32 fx_181_op80(s32 mode, SVECTOR *work, Unk800F78A4 *env, Unk80100C78Entry *entry, s32 *args) {
    BtlfxExports *ex = BTLFX_EXPORTS(env);
    Pool6CEntry *emitter;
    s32 frame;

    if (mode == 0) {
        *args = 8;
    } else if (mode == 1) {
        fx_181_env = env;
        ex->btlfx_getUnitBodyPos(0, 0, (s16 *)work);
        emitter = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, FX_COUNTDOWN_GOLD_SHARDS), (s32)work);
        if (emitter != NULL) {
            emitter->otZ = ex->btlfx_addRadiusSixth(0, 0x20);
        }
    } else {
        frame = entry->frame;
        if (frame == 0x28) {
            emitter = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, FX_COUNTDOWN_LENS_FLASH), (s32)work);
            if (emitter != NULL) {
                emitter->otZ = ex->btlfx_addRadiusSixth(0, 0x20);
            }
        }
        if (frame >= 0x28) {
            ex->btlfx_drawLayerTexture((BtlBgLayers *)ex->btlfx_getFileData(entry->file, FX_COUNTDOWN_BACKDROP), frame);
        }
        return frame >= 0x50;
    }
    return 0;
}

INCLUDE_RODATA("asm/us/fx_181/nonmatchings/fx_181", fx_181_headerPtr);
