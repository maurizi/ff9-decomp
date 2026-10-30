#include "fxmod/fx_01f.h"

s32 fx_01f_op80(s32 mode, Fx01fWork *work, Unk800F78A4 *env, Unk80100C78Entry *entry, s32 *arg4) {
    BtlfxExports *ex = BTLFX_EXPORTS(env);
    Pool6CEntry *p;
    s32 frame;
    s32 t;

    if (mode == 0) {
        *arg4 = 0xC;
    } else if (mode == 1) {
        work->unk0 = 0;
        D_801E7958 = env;
        ((s32 *)&work->unk4)[0] = ((s32 *)&env->targets->centerX)[0];
        t = ((s32 *)&env->targets->centerX)[1];
        work->unk4.vy -= 0x180;
        ((s32 *)&work->unk4)[1] = t;
        p = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, 1), (s32)&work->unk4);
        if (p != NULL) {
            p->otZ = 0xA0;
        }
        p = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, 0xC), (s32)&work->unk4);
        if (p != NULL) {
            p->otZ = 0xA0;
        }
    } else {
        frame = entry->frame;
        if (work->unk0 == 0) {
            if (frame == 0x18) {
                p = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, 3), (s32)&work->unk4);
                if (p != NULL) {
                    p->otZ = 0xC8;
                }
                p = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, 4), (s32)&work->unk4);
                if (p != NULL) {
                    p->otZ = 0xC8;
                }
                p = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, 0xB), (s32)&work->unk4);
                if (p != NULL) {
                    p->otZ = 0xC8;
                }
            }
            ex->btlfx_drawBackdrop((BtlBgLayers *)ex->btlfx_getFileData(entry->file, 0xD), frame);
            if (frame >= 0x3C) {
                return 1;
            }
        }
    }
    return 0;
}

INCLUDE_RODATA("asm/us/fx_01f/nonmatchings/fx_01f", fx_01f_headerPtr);
