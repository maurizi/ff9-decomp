#include "fxmod/fx_182.h"

s32 fx_182_op80(s32 mode, SVECTOR *work, Unk800F78A4 *env, Unk80100C78Entry *entry, s32 *arg4) {
    BtlfxExports *ex = BTLFX_EXPORTS(env);
    Pool6CEntry *p;
    s32 frame;

    if (mode == 0) {
        *arg4 = 8;
    } else if (mode == 1) {
        work->vx = env->targets->centerX;
        work->vz = env->targets->centerZ;
        work->vy = -0x200;
        D_801E78A0 = env;
        p = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, 3), (s32)work);
        if (p != NULL) {
            p->otZ = 0x40;
        }
    } else {
        frame = entry->frame;
        if (frame == 0x28) {
            p = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, 6), (s32)work);
            if (p != NULL) {
                p->otZ = 0x40;
            }
        }
        if (frame >= 0x28) {
            ex->btlfx_drawBackdrop((BtlBgLayers *)ex->btlfx_getFileData(entry->file, 1), frame);
        }
        return frame >= 0x50;
    }
    return 0;
}

INCLUDE_RODATA("asm/us/fx_182/nonmatchings/fx_182", fx_182_headerPtr);
