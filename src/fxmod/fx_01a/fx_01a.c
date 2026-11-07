#include "fxmod/fx_01a.h"

s32 fx_01a_op80(s32 mode, Fx01aWork *work, Unk800F78A4 *env, Unk80100C78Entry *entry, s32 *args) {
    BtlfxExports *ex = BTLFX_EXPORTS(env);
    s32 frame;

    if (mode == 0) {
        *args = sizeof(Fx01aWork);
    } else if (mode == 1) {
        fx_01a_env = env;
        ex->btlfx_getUnitPos(0, (s16 *)&work->posC);
        work->emit0 = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, FX_SLEEP_LIGHT_SPIRAL), (s32)&work->posC);
        if (work->emit0 != NULL) {
            work->emit0->trails = (s32)fx_01a_trails;
            work->emit0->trailSlots = FX_SLEEP_LIGHT_SPIRAL_TRAIL_SLOTS;
            work->emit0->trailStride = FX_SLEEP_LIGHT_SPIRAL_TRAIL_STRIDE;
        }
    } else {
        frame = entry->frame;
        switch (frame) {
        case FX_SLEEP_SPIRAL_2_FRAME:
            work->emit4 = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, FX_SLEEP_LIGHT_SPIRAL_2), (s32)&work->posC);
            if (work->emit4 != NULL) {
                work->emit4->trails = (s32)fx_01a_trails2;
                work->emit4->trailSlots = FX_SLEEP_LIGHT_SPIRAL_2_TRAIL_SLOTS;
                work->emit4->trailStride = FX_SLEEP_LIGHT_SPIRAL_2_TRAIL_STRIDE;
            }
            break;
        case FX_SLEEP_MOON_FRAME:
            work->emit8 = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, FX_SLEEP_MOON), (s32)&work->posC);
            break;
        }
        if (frame >= FX_SLEEP_SPIRAL_2_FRAME) {
            ex->btlfx_drawLayerTexture((BtlBgLayers *)ex->btlfx_getFileData(entry->file, FX_SLEEP_MOON_TEXTURE), frame - FX_SLEEP_SPIRAL_2_FRAME);
        }
        return frame >= FX_SLEEP_FRAMES;
    }
    return 0;
}

INCLUDE_RODATA("asm/us/fx_01a/nonmatchings/fx_01a", fx_01a_headerPtr);
