#include "fxmod/fx_00c.h"

s32 fx_00c_op80(s32 mode, Fx00cWork *work, Unk800F78A4 *env, Unk80100C78Entry *entry, s32 *arg4) {
    BtlfxExports *ex = BTLFX_EXPORTS(env);

    if (mode == 0) {
        *arg4 = 0x18;
    } else if (mode == 1) {
        D_801E787C = env;
        ex->btlfx_getUnitPos(0, (s16 *)&work->unk8);
        ex->btlfx_getUnitBodyPos(0, 0, (s16 *)&work->unk10);
        work->unk0 = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, 8), (s32)&work->unk8);
        work->unk4 = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, 7), (s32)&work->unk10);
        if (work->unk4 != NULL) {
            work->unk4->unk22 = ex->btlfx_addRadiusSixth(0, 0x20);
        }
    } else {
        return entry->frame >= 0x28;
    }
    return 0;
}

INCLUDE_RODATA("asm/us/fx_00c/nonmatchings/fx_00c", fx_00c_headerPtr);
