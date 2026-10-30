#include "fxmod/fx_09f.h"

s32 fx_09f_op80(s32 mode, Fx09fWork *work, Unk800F78A4 *env, Unk80100C78Entry *entry, s32 *arg4) {
    BtlfxExports *ex = BTLFX_EXPORTS(env);
    Pool6CEntry *p;
    s32 frame;

    if (mode == 0) {
        *arg4 = 0x14;
    } else if (mode == 1) {
        D_801E79B4 = env;
        work->unk0 = 0;
        ex->btlfx_getUnitBodyPos(0, 0, (s16 *)&work->unkC);
        ex->btlfx_getUnitPos(0, (s16 *)&work->unk4);
        p = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, 7), (s32)&work->unk4);
        if (p != NULL) {
            p->unk22 = ex->btlfx_addRadiusSixth(0, 0x10);
        }
        p = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, 4), (s32)&work->unk4);
        if (p != NULL) {
            p->unk22 = ex->btlfx_addRadiusSixth(0, 0x10);
        }
        p = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, 2), (s32)&work->unk4);
        if (p != NULL) {
            p->unk22 = ex->btlfx_addRadiusSixth(0, 0x10);
        }
        p = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, 0xC), (s32)&work->unkC);
        if (p != NULL) {
            p->unk22 = ex->btlfx_addRadiusSixth(0, 0x10);
        }
        ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, 5), (s32)&work->unk4);
    } else {
        frame = entry->frame;
        switch (work->unk0) {
        case 0:
            if (frame == 8) {
                p = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, 3), (s32)&work->unk4);
                if (p != NULL) {
                    p->unk22 = ex->btlfx_addRadiusSixth(0, 0x20);
                }
            }
            if (frame >= 0x3C) {
                return 1;
            }
            break;
        }
    }
    return 0;
}

INCLUDE_RODATA("asm/us/fx_09f/nonmatchings/fx_09f", fx_09f_headerPtr);
