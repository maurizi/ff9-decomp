#include "fxmod/fx_199.h"

s32 fx_199_op80(s32 mode, Fx199Work *work, Unk800F78A4 *env, Unk80100C78Entry *entry, s32 *args) {
    BtlfxExports *ex = BTLFX_EXPORTS(env);

    if (mode == 0) {
        *args = sizeof(Fx199Work);
    } else if (mode == 1) {
        fx_199_env = env;
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

s32 fx_199_op81(s32 mode, SVECTOR *work, Unk800F78A4 *env, Unk80100C78Entry *entry, s32 *args) {
    BtlfxExports *ex = BTLFX_EXPORTS(env);
    s32 frame;

    if (mode == 0) {
        *args = sizeof(SVECTOR);
    } else if (mode == 1) {
        fx_199_env = env;
    } else {
        frame = entry->frame;
        ex->btlfx_getUnitPos(BTLFX_UNIT_CASTER, (s16 *)work);
        env->camera->points[0].vx = work->vx;
        env->camera->points[0].vy = work->vy;
        env->camera->points[0].vz = work->vz;
        return frame >= FX_DARKSIDE_CAMERA_FRAMES;
    }
    return 0;
}

INCLUDE_RODATA("asm/us/fx_199/nonmatchings/fx_199", fx_199_headerPtr);
