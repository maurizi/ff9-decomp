#include "fxmod/fx_01f.h"

s32 fx_01f_op80(s32 mode, Fx01fWork *work, Unk800F78A4 *env, Unk80100C78Entry *entry, s32 *args) {
    BtlfxExports *ex = BTLFX_EXPORTS(env);
    Pool6CEntry *emitter;
    s32 frame;
    s32 zWord;

    if (mode == 0) {
        *args = 0xC;
    } else if (mode == 1) {
        work->hold0 = 0;
        fx_01f_env = env;
        ((s32 *)&work->pos4)[0] = ((s32 *)&env->targets->centerX)[0];
        zWord = ((s32 *)&env->targets->centerX)[1];
        work->pos4.vy -= 0x180;
        ((s32 *)&work->pos4)[1] = zWord;
        emitter = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, FX_01F_PINK_VORTEX), (s32)&work->pos4);
        if (emitter != NULL) {
            emitter->otZ = 0xA0;
        }
        emitter = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, FX_01F_RED_GLOW_2), (s32)&work->pos4);
        if (emitter != NULL) {
            emitter->otZ = 0xA0;
        }
    } else {
        frame = entry->frame;
        if (work->hold0 == 0) {
            if (frame == 0x18) {
                emitter = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, FX_01F_WHITE_RINGS), (s32)&work->pos4);
                if (emitter != NULL) {
                    emitter->otZ = 0xC8;
                }
                emitter = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, FX_01F_RED_GLOW), (s32)&work->pos4);
                if (emitter != NULL) {
                    emitter->otZ = 0xC8;
                }
                emitter = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, FX_01F_PINK_RINGS), (s32)&work->pos4);
                if (emitter != NULL) {
                    emitter->otZ = 0xC8;
                }
            }
            ex->btlfx_drawLayerTexture((BtlBgLayers *)ex->btlfx_getFileData(entry->file, FX_01F_BACKDROP), frame);
            if (frame >= 0x3C) {
                return 1;
            }
        }
    }
    return 0;
}

INCLUDE_RODATA("asm/us/fx_01f/nonmatchings/fx_01f", fx_01f_headerPtr);
