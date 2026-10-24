#include "fxmod/fx_0b7.h"

s32 fx_0b7_op80(s32 mode, Fx0b7Work *work, Unk800F78A4 *env, Unk80100C78Entry *entry, s32 *arg4) {
    BtlfxExports *ex = BTLFX_EXPORTS(env);
    Pool6CEntry *p;

    if (mode == 0) {
        *arg4 = 8;
    } else if (mode == 1) {
        D_801E786C = env;
        ex->btlfx_getUnitPos(0, (s16 *)&work->unk0);
        p = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->unk8, 2), (s32)&work->unk0);
        if (p != NULL) {
            p->unk22 = ex->func_800E7CC4(0, 0x40);
        }
        p = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->unk8, 6), (s32)&work->unk0);
        if (p != NULL) {
            p->unk22 = ex->func_800E7CC4(0, 0x40);
        }
    } else {
        return entry->unk0 >= 0x45;
    }
    return 0;
}

INCLUDE_RODATA("asm/us/fx_0b7/nonmatchings/fx_0b7", fx_0b7_headerPtr);
