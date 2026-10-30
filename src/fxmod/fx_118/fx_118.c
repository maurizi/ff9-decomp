#include "fxmod/fx_118.h"

s32 fx_118_op80(s32 mode, Fx118Work *work, Unk800F78A4 *env, Unk80100C78Entry *entry, s32 *arg4) {
    BtlfxExports *ex = BTLFX_EXPORTS(env);

    if (mode == 0) {
        *arg4 = 0xC;
    } else if (mode == 1) {
        D_801E783C = env;
        env->camera->paused = 0;
        ex->btlfx_getUnitBodyPos(0, 1, (s16 *)&work->unk4);
        work->unk0 = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, 4), (s32)&work->unk4);
        if (work->unk0 != NULL) {
            work->unk0->otZ = ex->btlfx_addRadiusSixth(0, 0x80);
        }
    } else {
        return entry->frame >= 0x28;
    }
    return 0;
}

INCLUDE_RODATA("asm/us/fx_118/nonmatchings/fx_118", fx_118_headerPtr);
