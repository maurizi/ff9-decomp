#include "fxmod/fx_0a6.h"

s32 fx_0a6_op80(s32 mode, Fx0a6Work *work, Unk800F78A4 *env, Unk80100C78Entry *entry, s32 *arg4) {
    BtlfxExports *ex = BTLFX_EXPORTS(env);
    s32 frame;

    if (mode == 0) {
        *arg4 = 0x24;
    } else if (mode == 1) {
        work->pos14.vx = env->targets->centerX;
        work->pos14.vy = env->targets->centerY;
        work->pos14.vz = env->targets->centerZ;
        work->pos1C.vx = env->targets->centerX;
        work->pos1C.vy = env->targets->centerY;
        work->pos1C.vz = env->targets->centerZ;
        work->pos14.vy -= 0xC8;
        fx_0a6_env = env;
    } else {
        frame = entry->frame;
        if (frame == 0) {
            work->emit0 = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, 1), (s32)&work->pos14);
            work->emit4 = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, 2), (s32)&work->pos14);
            work->emit8 = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, 3), (s32)&work->pos14);
            work->emitC = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, 4), (s32)&work->pos1C);
            if (work->emit4 != NULL) {
                work->emit4->trails = (s32)D_801E7984;
                work->emit4->trailSlots = 0x12;
                work->emit4->trailStride = 0x60;
            }
        }
        if (frame == 6) {
            work->emit10 = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, 5), (s32)&work->pos14);
        }
        return frame >= 0x28;
    }
    return 0;
}

INCLUDE_RODATA("asm/us/fx_0a6/nonmatchings/fx_0a6", fx_0a6_headerPtr);
