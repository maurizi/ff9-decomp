#include "fxmod/fx_04e.h"

s32 fx_04e_op80(s32 mode, Fx04eWork *work, Unk800F78A4 *env, Unk80100C78Entry *entry, s32 *arg4) {
    BtlfxExports *ex = BTLFX_EXPORTS(env);
    s32 frame;

    if (mode == 0) {
        *arg4 = 0x14;
    } else if (mode == 1) {
        work->posC.vx = env->targets->centerX;
        work->posC.vy = env->targets->centerY;
        work->posC.vz = env->targets->centerZ;
        fx_04e_env = env;
        work->emit0 = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, 6), (s32)&work->posC);
        if (work->emit0 != NULL) {
            work->emit0->trails = (s32)fx_04e_trails;
            work->emit0->trailSlots = 0xE;
            work->emit0->trailStride = 0x50;
        }
    } else {
        frame = entry->frame;
        switch (frame) {
        case 0x13:
            work->emit4 = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, 8), (s32)&work->posC);
            if (work->emit4 != NULL) {
                work->emit4->trails = (s32)fx_04e_trails2;
                work->emit4->trailSlots = 0x1E;
                work->emit4->trailStride = 0x50;
            }
            break;
        case 0x27:
            work->emit8 = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, 7), (s32)&work->posC);
            break;
        }
        if (frame >= 0x13) {
            ex->btlfx_drawBackdrop((BtlBgLayers *)ex->btlfx_getFileData(entry->file, 9), frame - 0x13);
        }
        return frame >= 0x50;
    }
    return 0;
}

INCLUDE_RODATA("asm/us/fx_04e/nonmatchings/fx_04e", fx_04e_headerPtr);
