#include "fxmod/fx_181.h"

s32 fx_181_op80(s32 mode, SVECTOR *work, Unk800F78A4 *env, Unk80100C78Entry *entry, s32 *arg4) {
    BtlfxExports *ex = BTLFX_EXPORTS(env);
    Pool6CEntry *p;
    s32 frame;

    if (mode == 0) {
        *arg4 = 8;
    } else if (mode == 1) {
        D_801E78B4 = env;
        ex->btlfx_getUnitBodyPos(0, 0, (s16 *)work);
        p = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, 2), (s32)work);
        if (p != NULL) {
            p->unk22 = ex->btlfx_addRadiusSixth(0, 0x20);
        }
    } else {
        frame = entry->frame;
        if (frame == 0x28) {
            p = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, 3), (s32)work);
            if (p != NULL) {
                p->unk22 = ex->btlfx_addRadiusSixth(0, 0x20);
            }
        }
        if (frame >= 0x28) {
            ex->btlfx_drawBackdrop((BtlBgLayers *)ex->btlfx_getFileData(entry->file, 1), frame);
        }
        return frame >= 0x50;
    }
    return 0;
}

INCLUDE_RODATA("asm/us/fx_181/nonmatchings/fx_181", fx_181_headerPtr);
