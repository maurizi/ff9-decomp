#include "fxmod/fx_029.h"

s32 fx_029_op80(s32 mode, Fx029Work *work, Unk800F78A4 *env, Unk80100C78Entry *entry, s32 *arg4) {
    BtlfxExports *ex = BTLFX_EXPORTS(env);
    s32 t;
    s32 frame;

    if (mode == 0) {
        *arg4 = 0x10;
    } else if (mode == 1) {
        work->unk0 = 0;
        D_801E7828 = env;
        ((s32 *)&work->unk8)[0] = ((s32 *)&env->targets->centerX)[0];
        t = ((s32 *)&env->targets->centerX)[1];
        work->unk8.vy -= 0xFA;
        ((s32 *)&work->unk8)[1] = t;
        work->unk4 = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, 7), (s32)&work->unk8);
        if (work->unk4 != NULL) {
            work->unk4->otZ = 0x80;
        }
    } else {
        frame = entry->frame;
        if (work->unk0 == 0 && frame >= 0x3C) {
            return 1;
        }
    }
    return 0;
}

INCLUDE_RODATA("asm/us/fx_029/nonmatchings/fx_029", fx_029_headerPtr);
