#include "fxmod/fx_060.h"

s32 fx_060_op80(s32 mode, Fx060Work *work, Unk800F78A4 *env, Unk80100C78Entry *entry, s32 *arg4) {
    BtlfxExports *ex = BTLFX_EXPORTS(env);
    Pool6CEntry *p;
    s32 frame;

    if (mode == 0) {
        *arg4 = 0xC;
    } else if (mode == 1) {
        D_801E7910 = env;
        work->unk0 = 0;
        ex->btlfx_getUnitBodyPos(0, 0, (s16 *)&work->unk4);
        p = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, 1), (s32)&work->unk4);
        if (p != NULL) {
            p->unk22 = ex->func_800E7CC4(0, 0x18);
            p->unk14 = (s32)D_801E7EF0;
            p->unk12 = 0x13;
            p->unkC = 0x6C;
        }
        p = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, 9), (s32)&work->unk4);
        if (p != NULL) {
            p->unk22 = ex->func_800E7CC4(0, 0x20);
            p->unk14 = (s32)D_801E7920;
            p->unk12 = 0xC;
            p->unkC = 0x7C;
        }
    } else {
        frame = entry->frame;
        if (work->unk0 == 0) {
            if (frame == 0x10) {
                p = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, 2), (s32)&work->unk4);
                if (p != NULL) {
                    p->unk22 = ex->func_800E7CC4(0, 0x20);
                }
            }
            if (frame >= 0x31) {
                return 1;
            }
        }
    }
    return 0;
}

INCLUDE_RODATA("asm/us/fx_060/nonmatchings/fx_060", fx_060_headerPtr);
