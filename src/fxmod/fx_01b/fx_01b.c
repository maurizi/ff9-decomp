#include "fxmod/fx_01b.h"

s32 fx_01b_op80(s32 mode, Fx01bWork *work, Unk800F78A4 *env, Unk80100C78Entry *entry, s32 *args) {
    BtlfxExports *ex = BTLFX_EXPORTS(env);
    Pool6CEntry *emitter;
    s32 frame;

    if (mode == 0) {
        *args = sizeof(Fx01bWork);
    } else if (mode == 1) {
        work->hold0 = 0;
        fx_01b_env = env;
        ex->btlfx_getUnitPos(0, (s16 *)&work->pos4);
        emitter = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, FX_FLOAT_LIFT_RINGS), (s32)&work->pos4);
        if (emitter != NULL) {
            emitter->otZ = ex->btlfx_addRadiusSixth(0, 0x40);
        }
        emitter = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, FX_FLOAT_CLOUD), (s32)&work->pos4);
        if (emitter != NULL) {
            emitter->otZ = -0x40;
        }
        emitter = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, FX_FLOAT_CLOUD_2), (s32)&work->pos4);
        if (emitter != NULL) {
            emitter->otZ = -0x80;
        }
    } else {
        frame = entry->frame;
        if (work->hold0 == 0) {
            ex->btlfx_drawLayerTexture((BtlBgLayers *)ex->btlfx_getFileData(entry->file, FX_FLOAT_CLOUD_TEXTURE), frame);
            if (frame >= FX_FLOAT_FRAMES) {
                return 1;
            }
        }
    }
    return 0;
}

INCLUDE_RODATA("asm/us/fx_01b/nonmatchings/fx_01b", fx_01b_headerPtr);
