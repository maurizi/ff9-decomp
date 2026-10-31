#include "fxmod/fx_047.h"

s32 fx_047_op80(s32 mode, Fx047Work *work, Unk800F78A4 *env, Unk80100C78Entry *entry, s32 *arg4) {
    BtlfxExports *ex = BTLFX_EXPORTS(env);
    Pool6CEntry *p;
    s32 frame;
    s32 t;

    if (mode == 0) {
        *arg4 = 0x18;
    } else if (mode == 1) {
        work->unk0 = 0;
        fx_047_env = env;
        ((s32 *)&work->posC)[0] = ((s32 *)&env->targets->centerX)[0];
        t = ((s32 *)&env->targets->centerX)[1];
        work->posC.vy = 0;
        ((s32 *)&work->posC)[1] = t;
        ((s32 *)&work->pos4)[0] = ((s32 *)&work->posC)[0];
        ((s32 *)&work->pos4)[1] = ((s32 *)&work->posC)[1];
        work->pos4.vy -= 0x12C;
        p = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, 3), (s32)&work->pos4);
        if (p != NULL) {
            p->otZ = 0x80;
        }
        work->emit14 = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, 2), (s32)&work->posC);
        if (work->emit14 != NULL) {
            work->emit14->otZ = 0x80;
        }
    } else {
        frame = entry->frame;
        if (work->unk0 == 0) {
            if (frame == 5) {
                p = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, 4), (s32)&work->posC);
                if (p != NULL) {
                    p->otZ = 0x80;
                }
            }
            if (frame == 0x11) {
                p = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, 1), (s32)&work->posC);
                if (p != NULL) {
                    p->otZ = 0x80;
                    p->trails = (s32)fx_047_trails;
                    p->trailSlots = 0x1D;
                    p->trailStride = 0xAC;
                }
                p = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, 5), (s32)&work->posC);
                if (p != NULL) {
                    p->otZ = 0x80;
                }
            }
            if (frame >= 0x32) {
                return 1;
            }
        }
    }
    return 0;
}

INCLUDE_RODATA("asm/us/fx_047/nonmatchings/fx_047", fx_047_headerPtr);
