#include "fxmod/fx_01b.h"

s32 fx_01b_op80(s32 mode, Fx01bWork *work, Unk800F78A4 *env, Unk80100C78Entry *entry, s32 *arg4) {
    BtlfxExports *ex = BTLFX_EXPORTS(env);
    Pool6CEntry *p;
    s32 frame;

    if (mode == 0) {
        *arg4 = 0xC;
    } else if (mode == 1) {
        work->unk0 = 0;
        D_801E78CC = env;
        ex->btlfx_getUnitPos(0, (s16 *)&work->unk4);
        p = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, 2), (s32)&work->unk4);
        if (p != NULL) {
            p->otZ = ex->btlfx_addRadiusSixth(0, 0x40);
        }
        p = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, 1), (s32)&work->unk4);
        if (p != NULL) {
            p->otZ = -0x40;
        }
        p = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, 3), (s32)&work->unk4);
        if (p != NULL) {
            p->otZ = -0x80;
        }
    } else {
        frame = entry->frame;
        if (work->unk0 == 0) {
            ex->btlfx_drawBackdrop((BtlBgLayers *)ex->btlfx_getFileData(entry->file, 4), frame);
            if (frame >= 0x3C) {
                return 1;
            }
        }
    }
    return 0;
}

INCLUDE_RODATA("asm/us/fx_01b/nonmatchings/fx_01b", fx_01b_headerPtr);
