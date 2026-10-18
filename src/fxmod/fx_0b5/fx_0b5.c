#include "fxmod/fx_0b5.h"

s32 fx_0b5_op80(s32 mode, Fx0b5Work *work, Unk800F78A4 *env, Unk80100C78Entry *entry, s32 *arg4) {
    BtlfxExports *ex = BTLFX_EXPORTS(env);
    Pool6CEntry *p;
    s32 frame;

    if (mode == 0) {
        *arg4 = 0x10;
    } else if (mode == 1) {
        D_801E7900 = env;
        ex->btlfx_getUnitBodyPos(0, 0, (s16 *)&work->unk8);
        ex->btlfx_getUnitPos(0, (s16 *)&work->unk0);
        p = ex->func_800E3CEC(ex->btlfx_getFileData(entry->unk8, 1), (s32)&work->unk0);
        if (p != NULL) {
            p->unk22 = ex->func_800E7CC4(0, 0x20);
            p->unk14 = (s32)D_801E7910;
            p->unk12 = 0x24;
            p->unkC = 0x44;
        }
        p = ex->func_800E3CEC(ex->btlfx_getFileData(entry->unk8, 2), (s32)&work->unk0);
        if (p != NULL) {
            p->unk22 = ex->func_800E7CC4(0, 0x20);
        }
    } else {
        frame = entry->unk0;
        if (frame == 0xE) {
            p = ex->func_800E3CEC(ex->btlfx_getFileData(entry->unk8, 5), (s32)&work->unk8);
            if (p != NULL) {
                p->unk22 = ex->func_800E7CC4(0, 0x20);
            }
        }
        if (frame > 0x28) {
            return 1;
        }
    }
    return 0;
}

INCLUDE_RODATA("asm/us/fx_0b5/nonmatchings/fx_0b5", fx_0b5_headerPtr);
