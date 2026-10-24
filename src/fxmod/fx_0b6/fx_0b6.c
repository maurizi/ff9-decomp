#include "fxmod/fx_0b6.h"

s32 fx_0b6_op80(s32 mode, Fx0b6Work *work, Unk800F78A4 *env, Unk80100C78Entry *entry, s32 *arg4) {
    BtlfxExports *ex = BTLFX_EXPORTS(env);
    Pool6CEntry *p;

    if (mode == 0) {
        *arg4 = 8;
    } else if (mode == 1) {
        D_801E78BC = env;
        ex->btlfx_getUnitArgBonePos(0, 0, (u16 *)&work->unk0);
        p = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->unk8, 1), (s32)&work->unk0);
        if (p != NULL) {
            p->unk22 = ex->func_800E7CC4(0, 0xC8);
        }
        p = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->unk8, 2), (s32)&work->unk0);
        if (p != NULL) {
            p->unk22 = ex->func_800E7CC4(0, 0xC8);
        }
        p = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->unk8, 7), (s32)&work->unk0);
        if (p != NULL) {
            p->unk22 = ex->func_800E7CC4(0, 0xC8);
        }
    } else {
        return entry->unk0 >= 0x42;
    }
    return 0;
}

INCLUDE_RODATA("asm/us/fx_0b6/nonmatchings/fx_0b6", fx_0b6_headerPtr);
