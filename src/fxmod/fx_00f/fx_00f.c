#include "fxmod/fx_00f.h"

s32 fx_00f_op80(s32 mode, Fx00fWork *work, Unk800F78A4 *env, Unk80100C78Entry *entry, s32 *arg4) {
    BtlfxExports *ex = BTLFX_EXPORTS(env);
    Pool6CEntry *p;
    s32 frame;

    if (mode == 0) {
        *arg4 = 0xC;
    } else if (mode == 1) {
        D_801E7938 = env;
        work->unk0 = 0;
        ex->btlfx_getUnitBodyPos(0, 0, (s16 *)&work->unk4);
        p = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, 0xA), (s32)&work->unk4);
        if (p != NULL) {
            p->unk22 = ex->btlfx_addRadiusSixth(0, 0x20);
        }
        p = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, 7), (s32)&work->unk4);
        if (p != NULL) {
            p->unk22 = 0x20;
            p->unk14 = (s32)D_801E7948;
            p->unk12 = 0x14;
            p->unkC = 0x34;
        }
    } else {
        frame = entry->frame;
        if (work->unk0 == 0) {
            if (frame == 6) {
                p = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, 9), (s32)&work->unk4);
                if (p != NULL) {
                    p->unk22 = ex->btlfx_addRadiusSixth(0, 0x40);
                }
                p = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, 6), (s32)&work->unk4);
                if (p != NULL) {
                    p->unk22 = ex->btlfx_addRadiusSixth(0, 0x40);
                }
            }
            if (frame >= 0x28) {
                return 1;
            }
        }
    }
    return 0;
}

INCLUDE_RODATA("asm/us/fx_00f/nonmatchings/fx_00f", fx_00f_headerPtr);
