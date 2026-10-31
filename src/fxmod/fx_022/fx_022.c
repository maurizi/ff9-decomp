#include "fxmod/fx_022.h"

s32 fx_022_op80(s32 mode, Fx022Work *work, Unk800F78A4 *env, Unk80100C78Entry *entry, s32 *arg4) {
    BtlfxExports *ex = BTLFX_EXPORTS(env);
    s32 frame;

    if (mode == 0) {
        *arg4 = 0x14;
    } else if (mode == 1) {
        fx_022_env = env;
        ex->btlfx_getUnitArgBonePos(0x10, 0, (u16 *)&work->pos4);
        ex->btlfx_getUnitArgBonePos(0x10, 1, (u16 *)&work->unkC);
        env->camera->points[0].vx = work->pos4.vx;
        env->camera->points[0].vy = work->pos4.vy;
        env->camera->points[0].vz = work->pos4.vz;
        work->emit0 = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, 1), (s32)&work->pos4);
    } else {
        frame = entry->frame;
        if (work->emit0 != NULL) {
            if (work->emit0->state == BTLFX_EMIT_DONE) {
                work->emit0 = NULL;
            }
            if (work->emit0 != NULL) {
                work->emit0->target = (s32)&work->unkC;
            }
        }
        return frame >= 0x28;
    }
    return 0;
}

INCLUDE_RODATA("asm/us/fx_022/nonmatchings/fx_022", fx_022_headerPtr);
