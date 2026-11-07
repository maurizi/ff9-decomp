#include "fxmod/fx_0bc.h"

s32 fx_0bc_op80(s32 mode, Fx0bcWork80 *work, Unk800F78A4 *env, Unk80100C78Entry *entry, s32 *args) {
    BtlfxExports *ex = BTLFX_EXPORTS(env);

    if (mode == 0) {
        *args = sizeof(Fx0bcWork80);
    } else if (mode == 1) {
        fx_0bc_env = env;
        ex->btlfx_getUnitBodyPos(0, 0, (s16 *)&work->pos4);
        work->emit0 = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, FX_DARKSIDE_SPARKLE_BURST), (s32)&work->pos4);
        if (work->emit0 != NULL) {
            work->emit0->otZ = ex->btlfx_addRadiusSixth(0, 0x80);
        }
    } else {
        return entry->frame >= FX_DARKSIDE_FRAMES;
    }
    return 0;
}

s32 fx_0bc_op81(s32 mode, Fx0bcWork81 *work, Unk800F78A4 *env, Unk80100C78Entry *entry, s32 *args) {
    BtlfxExports *ex = BTLFX_EXPORTS(env);
    s32 frame;

    if (mode == 0) {
        *args = sizeof(Fx0bcWork81);
    } else if (mode == 1) {
        fx_0bc_env = env;
    } else {
        frame = entry->frame;
        ex->btlfx_getUnitPos(0x10, (s16 *)&work->pos0);
        env->camera->points[0].vx = work->pos0.vx;
        env->camera->points[0].vy = work->pos0.vy;
        env->camera->points[0].vz = work->pos0.vz;
        return frame >= FX_DARKSIDE_CAMERA_FRAMES;
    }
    return 0;
}

INCLUDE_RODATA("asm/us/fx_0bc/nonmatchings/fx_0bc", fx_0bc_headerPtr);
