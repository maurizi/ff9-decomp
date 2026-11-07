#include "fxmod/fx_011.h"

s32 fx_011_op80(s32 mode, SVECTOR *work, Unk800F78A4 *env, Unk80100C78Entry *entry, s32 *args) {
    BtlfxExports *ex = BTLFX_EXPORTS(env);
    Pool6CEntry *emitter;
    s32 frame;

    if (mode == 0) {
        *args = sizeof(SVECTOR);
    } else if (mode == 1) {
        fx_011_env = env;
        ex->btlfx_getUnitBodyPos(0, 0, (s16 *)work);
        emitter = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, FX_LIFE_HALO_RINGS), (s32)work);
        if (emitter != NULL) {
            emitter->otZ = ex->btlfx_addRadiusSixth(0, 0x80);
        }
        emitter = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, FX_LIFE_FEATHER_TRAILS), (s32)work);
        if (emitter != NULL) {
            emitter->trails = (s32)fx_011_trails;
            emitter->trailSlots = FX_LIFE_FEATHER_TRAILS_TRAIL_SLOTS;
            emitter->trailStride = FX_LIFE_FEATHER_TRAILS_TRAIL_STRIDE;
            emitter->otZ = ex->btlfx_addRadiusSixth(0, 0x80);
        }
    } else {
        frame = entry->frame;
        if (frame == FX_LIFE_WINGS_FRAME) {
            emitter = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, FX_LIFE_WINGS), (s32)work);
            if (emitter != NULL) {
                emitter->otZ = ex->btlfx_addRadiusSixth(0, 0x80);
            }
        }
        return frame >= FX_LIFE_FRAMES;
    }
    return 0;
}

INCLUDE_RODATA("asm/us/fx_011/nonmatchings/fx_011", fx_011_headerPtr);
