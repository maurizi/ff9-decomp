#include "fxmod/fx_04e.h"

s32 fx_04e_op80(s32 mode, Fx04eWork *work, Unk800F78A4 *env, Unk80100C78Entry *entry, s32 *arg4) {
    BtlfxExports *ex = BTLFX_EXPORTS(env);
    s32 frame;

    if (mode == 0) {
        *arg4 = 0x14;
    } else if (mode == 1) {
        work->unkC.vx = env->targets->centerX;
        work->unkC.vy = env->targets->centerY;
        work->unkC.vz = env->targets->centerZ;
        D_801E7930 = env;
        work->unk0 = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->unk8, 6), (s32)&work->unkC);
        if (work->unk0 != NULL) {
            work->unk0->unk14 = (s32)D_801E7940;
            work->unk0->unk12 = 0xE;
            work->unk0->unkC = 0x50;
        }
    } else {
        frame = entry->unk0;
        switch (frame) {
        case 0x13:
            work->unk4 = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->unk8, 8), (s32)&work->unkC);
            if (work->unk4 != NULL) {
                work->unk4->unk14 = (s32)D_801E7DA0;
                work->unk4->unk12 = 0x1E;
                work->unk4->unkC = 0x50;
            }
            break;
        case 0x27:
            work->unk8 = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->unk8, 7), (s32)&work->unkC);
            break;
        }
        if (frame >= 0x13) {
            ex->btlfx_drawBackdrop((BtlBgLayers *)ex->btlfx_getFileData(entry->unk8, 9), frame - 0x13);
        }
        return frame >= 0x50;
    }
    return 0;
}

INCLUDE_RODATA("asm/us/fx_04e/nonmatchings/fx_04e", fx_04e_headerPtr);
