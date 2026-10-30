#include "fxmod/fx_0f1.h"

s32 fx_0f1_op80(s32 mode, Fx0f1Work *work, Unk800F78A4 *env, Unk80100C78Entry *entry, s32 *arg4) {
    BtlfxExports *ex = BTLFX_EXPORTS(env);

    if (mode == 0) {
        *arg4 = 0xC;
    } else if (mode == 1) {
        D_801E7834 = env;
        ex->btlfx_getUnitBodyPos(0, 0, (s16 *)&work->unk4);
        work->unk0 = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, 2), (s32)&work->unk4);
        if (work->unk0 != NULL) {
            work->unk0->otZ = ex->btlfx_addRadiusSixth(0, 0x80);
        }
    } else {
        return entry->frame >= 0x1E;
    }
    return 0;
}

INCLUDE_RODATA("asm/us/fx_0f1/nonmatchings/fx_0f1", fx_0f1_headerPtr);
