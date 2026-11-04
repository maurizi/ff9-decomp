#include "fxmod/fx_02a.h"

s32 fx_02a_op80(s32 mode, Fx02aWork *work, Unk800F78A4 *env, Unk80100C78Entry *entry, s32 *args) {
    BtlfxExports *ex = BTLFX_EXPORTS(env);
    Pool6CEntry *emitter;
    s32 frame;

    if (mode == 0) {
        *args = 0x10;
    } else if (mode == 1) {
        fx_02a_env = env;
        work->hold0 = 0;
        ex->btlfx_getUnitBodyPos(0, 0, (s16 *)&work->pos4);
        emitter = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, FX_SILENCE_GLYPH_GLOW), (s32)&work->pos4);
        if (emitter != NULL) {
            emitter->otZ = ex->btlfx_addRadiusSixth(0, 0x40);
        }
        work->unkC = 0;
    } else {
        frame = entry->frame;
        if (work->hold0 == 0) {
            if (frame == 8) {
                emitter = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, FX_SILENCE_GLYPH), (s32)&work->pos4);
                if (emitter != NULL) {
                    emitter->otZ = ex->btlfx_addRadiusSixth(0, 0x40);
                }
                emitter = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, FX_SILENCE_RUNE_SEAL), (s32)&work->pos4);
                if (emitter != NULL) {
                    emitter->otZ = ex->btlfx_addRadiusSixth(0, 0x40);
                }
            }
            if (frame >= 8) {
                ex->btlfx_drawLayerTexture((BtlBgLayers *)ex->btlfx_getFileData(entry->file, FX_SILENCE_NEBULA_TEXTURE), frame);
            }
            if (frame >= 0x28) {
                return 1;
            }
        }
    }
    return 0;
}

INCLUDE_RODATA("asm/us/fx_02a/nonmatchings/fx_02a", fx_02a_headerPtr);
