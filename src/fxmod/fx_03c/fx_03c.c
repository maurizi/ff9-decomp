#include "fxmod/fx_03c.h"

s32 fx_03c_op80(s32 mode, Fx03cWork *work, Unk800F78A4 *env, Unk80100C78Entry *entry, s32 *arg4) {
    BtlfxExports *ex = BTLFX_EXPORTS(env);

    if (mode == 0) {
        *arg4 = 0x24;
    } else if (mode == 1) {
        D_801E786C = env;
        ex->btlfx_getUnitBodyPos(0x10, 0, (s16 *)&work->unk4);
        ex->btlfx_getUnitBodyPos(0, 0, (s16 *)&work->unkC);
        ex->btlfx_getUnitArgBonePos(0x10, 0, (u16 *)&work->unk14);
        work->unk0 = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, 2), (s32)&work->unkC);
        if (work->unk0 != NULL) {
            work->unk0->unk14 = (s32)D_801E787C;
            work->unk0->unk12 = 0x2B;
            work->unk0->unkC = 0x40;
        }
    } else {
        return entry->frame >= 0x28;
    }
    return 0;
}

INCLUDE_RODATA("asm/us/fx_03c/nonmatchings/fx_03c", fx_03c_headerPtr);
