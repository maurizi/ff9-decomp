#include "fxmod/fx_08a.h"

s32 fx_08a_op80(s32 mode, Fx08aWork *work, Unk800F78A4 *env, Unk80100C78Entry *entry, s32 *arg4) {
    BtlfxExports *ex = BTLFX_EXPORTS(env);
    Pool6CEntry *p;
    s32 frame;

    if (mode == 0) {
        *arg4 = 0xC;
    } else if (mode == 1) {
        fx_08a_env = env;
        work->state0 = 0;
        ex->btlfx_getUnitBodyPos(0, 0, (s16 *)&work->pos4);
        p = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, 7), (s32)&work->pos4);
        if (p != NULL) {
            p->otZ = ex->btlfx_addRadiusSixth(0, 0x20);
        }
        p = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, 6), (s32)&work->pos4);
        if (p != NULL) {
            p->otZ = 0x20;
            p->trails = (s32)fx_08a_trails;
            p->trailSlots = 0x19;
            p->trailStride = 0x34;
        }
    } else {
        frame = entry->frame;
        switch (work->state0) {
        case 0:
            if (frame == 6) {
                p = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, 0xA), (s32)&work->pos4);
                if (p != NULL) {
                    p->otZ = ex->btlfx_addRadiusSixth(0, 0x40);
                }
                p = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, 9), (s32)&work->pos4);
                if (p != NULL) {
                    p->otZ = ex->btlfx_addRadiusSixth(0, 0x40);
                }
            }
            if (frame >= 0x28) {
                return 1;
            }
            break;
        }
    }
    return 0;
}

INCLUDE_RODATA("asm/us/fx_08a/nonmatchings/fx_08a", fx_08a_headerPtr);
