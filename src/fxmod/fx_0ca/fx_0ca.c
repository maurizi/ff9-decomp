#include "fxmod/fx_0ca.h"

s32 fx_0ca_op80(s32 mode, Fx0caWork *work, Unk800F78A4 *env, Unk80100C78Entry *entry, s32 *arg4) {
    BtlfxExports *ex = BTLFX_EXPORTS(env);
    s32 frame;

    if (mode == 0) {
        *arg4 = 0x18;
    } else if (mode == 1) {
        work->unk10.vx = env->targets->centerX;
        work->unk10.vy = env->targets->centerY;
        work->unk10.vz = env->targets->centerZ;
        work->unk8.vx = work->unk10.vx;
        work->unk8.vy = work->unk10.vy;
        work->unk8.vz = work->unk10.vz;
        work->unk8.vy -= 0x190;
        D_801E78B8 = env;
        work->unk0 = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->unk8, 2), (s32)&work->unk8);
        if (work->unk0 != NULL) {
            work->unk0->unk14 = (s32)D_801E78C8;
            work->unk0->unk12 = 0x1E;
            work->unk0->unkC = 0x30;
        }
        if (work->unk0 != NULL) {
            work->unk0->unk22 = 0x80;
        }
    } else {
        frame = entry->unk0;
        if (frame == 0x19) {
            work->unk4 = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->unk8, 1), (s32)&work->unk10);
        }
        return frame >= 0x46;
    }
    return 0;
}

INCLUDE_RODATA("asm/us/fx_0ca/nonmatchings/fx_0ca", fx_0ca_headerPtr);
