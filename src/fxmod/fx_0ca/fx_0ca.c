#include "fxmod/fx_0ca.h"

s32 fx_0ca_op80(s32 mode, Fx0caWork *work, Unk800F78A4 *env, Unk80100C78Entry *entry, s32 *args) {
    BtlfxExports *ex = BTLFX_EXPORTS(env);
    s32 frame;

    if (mode == 0) {
        *args = sizeof(Fx0caWork);
    } else if (mode == 1) {
        work->pos10.vx = env->targets->centerX;
        work->pos10.vy = env->targets->centerY;
        work->pos10.vz = env->targets->centerZ;
        work->pos8.vx = work->pos10.vx;
        work->pos8.vy = work->pos10.vy;
        work->pos8.vz = work->pos10.vz;
        work->pos8.vy -= FX_POISON_SWIRL_HEIGHT;
        fx_0ca_env = env;
        work->emit0 = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, FX_POISON_SWIRL), (s32)&work->pos8);
        if (work->emit0 != NULL) {
            work->emit0->trails = (s32)fx_0ca_trails;
            work->emit0->trailSlots = FX_POISON_SWIRL_TRAIL_SLOTS;
            work->emit0->trailStride = FX_POISON_SWIRL_TRAIL_STRIDE;
        }
        if (work->emit0 != NULL) {
            work->emit0->otZ = 0x80;
        }
    } else {
        frame = entry->frame;
        if (frame == FX_POISON_BUBBLES_FRAME) {
            work->emit4 = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, FX_POISON_BUBBLES), (s32)&work->pos10);
        }
        return frame >= FX_POISON_FRAMES;
    }
    return 0;
}

INCLUDE_RODATA("asm/us/fx_0ca/nonmatchings/fx_0ca", fx_0ca_headerPtr);
