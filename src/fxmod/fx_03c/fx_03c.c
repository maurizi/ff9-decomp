#include "fxmod/fx_03c.h"

s32 fx_03c_op80(s32 mode, Fx03cWork *work, Unk800F78A4 *env, Unk80100C78Entry *entry, s32 *arg4) {
    BtlfxExports *ex = BTLFX_EXPORTS(env);

    if (mode == 0) {
        *arg4 = 0x24;
    } else if (mode == 1) {
        fx_03c_env = env;
        ex->btlfx_getUnitBodyPos(0x10, 0, (s16 *)&work->unk4);
        ex->btlfx_getUnitBodyPos(0, 0, (s16 *)&work->posC);
        ex->btlfx_getUnitArgBonePos(0x10, 0, (u16 *)&work->unk14);
        work->emit0 = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, 2), (s32)&work->posC);
        if (work->emit0 != NULL) {
            work->emit0->trails = (s32)D_801E787C;
            work->emit0->trailSlots = 0x2B;
            work->emit0->trailStride = 0x40;
        }
    } else {
        return entry->frame >= 0x28;
    }
    return 0;
}

INCLUDE_RODATA("asm/us/fx_03c/nonmatchings/fx_03c", fx_03c_headerPtr);
