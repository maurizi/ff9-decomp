#include "fxmod/fx_0a6.h"

s32 fx_0a6_op80(s32 mode, Fx0a6Work *work, Unk800F78A4 *env, Unk80100C78Entry *entry, s32 *args) {
    BtlfxExports *ex = BTLFX_EXPORTS(env);
    s32 frame;

    if (mode == 0) {
        *args = sizeof(Fx0a6Work);
    } else if (mode == 1) {
        work->pos14.vx = env->targets->centerX;
        work->pos14.vy = env->targets->centerY;
        work->pos14.vz = env->targets->centerZ;
        work->pos1C.vx = env->targets->centerX;
        work->pos1C.vy = env->targets->centerY;
        work->pos1C.vz = env->targets->centerZ;
        work->pos14.vy -= FX_SWEEP_HEIGHT;
        fx_0a6_env = env;
    } else {
        frame = entry->frame;
        if (frame == 0) {
            work->emit0 = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, FX_SWEEP_DUST_RINGS), (s32)&work->pos14);
            work->emit4 = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, FX_SWEEP_WIND_TRAIL), (s32)&work->pos14);
            work->emit8 = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, FX_SWEEP_DUST_PUFF), (s32)&work->pos14);
            work->emitC = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, FX_SWEEP_TAN_RING), (s32)&work->pos1C);
            if (work->emit4 != NULL) {
                work->emit4->trails = (s32)fx_0a6_trails;
                work->emit4->trailSlots = FX_SWEEP_WIND_TRAIL_TRAIL_SLOTS;
                work->emit4->trailStride = FX_SWEEP_WIND_TRAIL_TRAIL_STRIDE;
            }
        }
        if (frame == FX_SWEEP_CLOUD_FRAME) {
            work->emit10 = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, FX_SWEEP_DUST_CLOUD), (s32)&work->pos14);
        }
        return frame >= FX_SWEEP_FRAMES;
    }
    return 0;
}

INCLUDE_RODATA("asm/us/fx_0a6/nonmatchings/fx_0a6", fx_0a6_headerPtr);
