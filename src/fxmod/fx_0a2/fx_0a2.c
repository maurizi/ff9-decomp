#include "fxmod/fx_0a2.h"

s32 fx_0a2_op80(s32 mode, Fx0a2Work *work, Unk800F78A4 *env, Unk80100C78Entry *entry, s32 *args) {
    BtlfxExports *ex = BTLFX_EXPORTS(env);
    s32 frame;

    if (mode == 0) {
        *args = 0x10;
    } else if (mode == 1) {
        work->pos8.vx = env->targets->centerX;
        work->pos8.vy = env->targets->centerY;
        work->pos8.vz = env->targets->centerZ;
        fx_0a2_env = env;
        work->emit0 = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, FX_CURA_STAR_TRAIL), (s32)&work->pos8);
        if (work->emit0 != NULL) {
            work->emit0->trails = (s32)fx_0a2_trails;
            work->emit0->trailSlots = 0xB;
            work->emit0->trailStride = 0xA0;
        }
    } else {
        frame = entry->frame;
        if (frame == 0x14) {
            work->emit4 = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, FX_CURA_SPARKLE_BURST), (s32)&work->pos8);
        }
        return frame >= 0x3C;
    }
    return 0;
}

INCLUDE_RODATA("asm/us/fx_0a2/nonmatchings/fx_0a2", fx_0a2_headerPtr);
