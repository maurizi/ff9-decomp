#include "fxmod/fx_00b.h"

s32 fx_00b_op80(s32 mode, Fx00bWork *work, Unk800F78A4 *env, Unk80100C78Entry *entry, s32 *args) {
    BtlfxExports *ex = BTLFX_EXPORTS(env);
    Pool6CEntry *emitter;
    s32 frame;

    if (mode == 0) {
        *args = 0x10;
    } else if (mode == 1) {
        fx_00b_env = env;
        work->hold0 = 0;
        ex->btlfx_getUnitBodyPos(0, 0, (s16 *)&work->pos4);
        emitter = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, FX_ESUNA_LIGHT_SHOWER), (s32)&work->pos4);
        if (emitter != NULL) {
            emitter->otZ = ex->btlfx_addRadiusSixth(0, 4);
        }
        work->unkC = 0;
    } else {
        frame = entry->frame;
        if (work->hold0 == 0) {
            ex->btlfx_drawLayerTexture((BtlBgLayers *)ex->btlfx_getFileData(entry->file, FX_ESUNA_ORB_TEXTURE), frame);
            if (frame >= 0x28) {
                return 1;
            }
        }
    }
    return 0;
}

INCLUDE_RODATA("asm/us/fx_00b/nonmatchings/fx_00b", fx_00b_headerPtr);
