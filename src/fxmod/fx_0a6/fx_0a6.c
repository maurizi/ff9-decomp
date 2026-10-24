#include "fxmod/fx_0a6.h"

s32 fx_0a6_op80(s32 mode, Fx0a6Work *work, Unk800F78A4 *env, Unk80100C78Entry *entry, s32 *arg4) {
    BtlfxExports *ex = BTLFX_EXPORTS(env);
    s32 frame;

    if (mode == 0) {
        *arg4 = 0x24;
    } else if (mode == 1) {
        work->unk14.vx = env->targets->centerX;
        work->unk14.vy = env->targets->centerY;
        work->unk14.vz = env->targets->centerZ;
        work->unk1C.vx = env->targets->centerX;
        work->unk1C.vy = env->targets->centerY;
        work->unk1C.vz = env->targets->centerZ;
        work->unk14.vy -= 0xC8;
        D_801E7974 = env;
    } else {
        frame = entry->unk0;
        if (frame == 0) {
            work->unk0 = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->unk8, 1), (s32)&work->unk14);
            work->unk4 = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->unk8, 2), (s32)&work->unk14);
            work->unk8 = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->unk8, 3), (s32)&work->unk14);
            work->unkC = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->unk8, 4), (s32)&work->unk1C);
            if (work->unk4 != NULL) {
                work->unk4->unk14 = (s32)D_801E7984;
                work->unk4->unk12 = 0x12;
                work->unk4->unkC = 0x60;
            }
        }
        if (frame == 6) {
            work->unk10 = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->unk8, 5), (s32)&work->unk14);
        }
        return frame >= 0x28;
    }
    return 0;
}

INCLUDE_RODATA("asm/us/fx_0a6/nonmatchings/fx_0a6", fx_0a6_headerPtr);
