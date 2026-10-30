#include "fxmod/fx_02a.h"

s32 fx_02a_op80(s32 mode, Fx02aWork *work, Unk800F78A4 *env, Unk80100C78Entry *entry, s32 *arg4) {
    BtlfxExports *ex = BTLFX_EXPORTS(env);
    Pool6CEntry *p;
    s32 frame;

    if (mode == 0) {
        *arg4 = 0x10;
    } else if (mode == 1) {
        D_801E791C = env;
        work->unk0 = 0;
        ex->btlfx_getUnitBodyPos(0, 0, (s16 *)&work->unk4);
        p = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, 3), (s32)&work->unk4);
        if (p != NULL) {
            p->otZ = ex->btlfx_addRadiusSixth(0, 0x40);
        }
        work->unkC = 0;
    } else {
        frame = entry->frame;
        if (work->unk0 == 0) {
            if (frame == 8) {
                p = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, 1), (s32)&work->unk4);
                if (p != NULL) {
                    p->otZ = ex->btlfx_addRadiusSixth(0, 0x40);
                }
                p = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, 0xA), (s32)&work->unk4);
                if (p != NULL) {
                    p->otZ = ex->btlfx_addRadiusSixth(0, 0x40);
                }
            }
            if (frame >= 8) {
                ex->btlfx_drawBackdrop((BtlBgLayers *)ex->btlfx_getFileData(entry->file, 4), frame);
            }
            if (frame >= 0x28) {
                return 1;
            }
        }
    }
    return 0;
}

INCLUDE_RODATA("asm/us/fx_02a/nonmatchings/fx_02a", fx_02a_headerPtr);
