#include "fxmod/fx_0bd.h"

s32 fx_0bd_op80(s32 mode, Fx0bdWork80 *work, Unk800F78A4 *env, Unk80100C78Entry *entry, s32 *args) {
    BtlfxExports *ex = BTLFX_EXPORTS(env);

    if (mode == 0) {
        *args = 0xC;
    } else if (mode == 1) {
        fx_0bd_env = env;
        ex->btlfx_getUnitBodyPos(0, 0, (s16 *)&work->pos4);
        work->emit0 = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, FX_MINUS_STRIKE_SLASH_ARC), (s32)&work->pos4);
        if (work->emit0 != NULL) {
            work->emit0->otZ = ex->btlfx_addRadiusSixth(0, 0x80);
        }
    } else {
        return entry->frame >= 0x1E;
    }
    return 0;
}

s32 fx_0bd_op81(s32 mode, Fx0bdWork81 *work, Unk800F78A4 *env, Unk80100C78Entry *entry, s32 *args) {
    BtlfxExports *ex = BTLFX_EXPORTS(env);
    s32 frame;

    if (mode == 0) {
        *args = 8;
    } else if (mode == 1) {
        fx_0bd_env = env;
    } else {
        frame = entry->frame;
        ex->btlfx_getUnitPos(0x10, (s16 *)&work->pos0);
        env->camera->points[0].vx = work->pos0.vx;
        env->camera->points[0].vy = work->pos0.vy;
        env->camera->points[0].vz = work->pos0.vz;
        return frame >= 0xC;
    }
    return 0;
}

INCLUDE_RODATA("asm/us/fx_0bd/nonmatchings/fx_0bd", fx_0bd_headerPtr);
