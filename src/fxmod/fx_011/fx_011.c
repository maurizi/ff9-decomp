#include "fxmod/fx_011.h"

s32 fx_011_op80(s32 mode, SVECTOR *work, Unk800F78A4 *env, Unk80100C78Entry *entry, s32 *arg4) {
    BtlfxExports *ex = BTLFX_EXPORTS(env);
    Pool6CEntry *p;
    s32 frame;

    if (mode == 0) {
        *arg4 = 8;
    } else if (mode == 1) {
        D_801E78EC = env;
        ex->btlfx_getUnitBodyPos(0, 0, (s16 *)work);
        p = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, 8), (s32)work);
        if (p != NULL) {
            p->otZ = ex->btlfx_addRadiusSixth(0, 0x80);
        }
        p = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, 7), (s32)work);
        if (p != NULL) {
            p->unk14 = (s32)D_801E78FC;
            p->unk12 = 0x19;
            p->unkC = 0x34;
            p->otZ = ex->btlfx_addRadiusSixth(0, 0x80);
        }
    } else {
        frame = entry->frame;
        if (frame == 6) {
            p = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, 9), (s32)work);
            if (p != NULL) {
                p->otZ = ex->btlfx_addRadiusSixth(0, 0x80);
            }
        }
        return frame >= 0x30;
    }
    return 0;
}

INCLUDE_RODATA("asm/us/fx_011/nonmatchings/fx_011", fx_011_headerPtr);
