#include "fxmod/fx_18f.h"

s32 fx_18f_op80(s32 mode, Fx18fWork *work, Unk800F78A4 *env, Unk80100C78Entry *entry, s32 *arg4) {
    BtlfxExports *ex = BTLFX_EXPORTS(env);

    if (mode == 0) {
        *arg4 = 0xC;
    } else if (mode == 1) {
        work->unk4.vy = -0x1B58;
        work->unk4.vx = 0;
        work->unk4.vz = 0x7D0;
        env->camera->points[0].vx = work->unk4.vx;
        env->camera->points[0].vy = work->unk4.vy;
        env->camera->points[0].vz = work->unk4.vz;
        D_801E7820 = env;
        work->unk0 = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, 1), (s32)&work->unk4);
    } else {
        return entry->frame >= 0x32;
    }
    return 0;
}

INCLUDE_RODATA("asm/us/fx_18f/nonmatchings/fx_18f", fx_18f_headerPtr);
