#include "fxmod/fx_18f.h"

s32 fx_18f_op80(s32 mode, Fx18fWork *work, Unk800F78A4 *env, Unk80100C78Entry *entry, s32 *args) {
    BtlfxExports *ex = BTLFX_EXPORTS(env);

    if (mode == 0) {
        *args = sizeof(Fx18fWork);
    } else if (mode == 1) {
        work->pos4.vy = -FX_WHATS_THAT_STREAKS_HEIGHT;
        work->pos4.vx = 0;
        work->pos4.vz = FX_WHATS_THAT_STREAKS_Z;
        env->camera->points[0].vx = work->pos4.vx;
        env->camera->points[0].vy = work->pos4.vy;
        env->camera->points[0].vz = work->pos4.vz;
        fx_18f_env = env;
        work->emit0 = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, FX_WHATS_THAT_LIGHT_STREAKS), (s32)&work->pos4);
    } else {
        return entry->frame >= FX_WHATS_THAT_FRAMES;
    }
    return 0;
}

INCLUDE_RODATA("asm/us/fx_18f/nonmatchings/fx_18f", fx_18f_headerPtr);
