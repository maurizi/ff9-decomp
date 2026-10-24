#include "fxmod/fx_056.h"

s32 fx_056_op80(s32 mode, Fx056Work *work, Unk800F78A4 *env, Unk80100C78Entry *entry, s32 *arg4) {
    BtlfxExports *ex = BTLFX_EXPORTS(env);
    s32 frame;

    if (mode == 0) {
        *arg4 = 0x14;
    } else if (mode == 1) {
        s32 n = *arg4;

        work->unk10 = n;
        D_801E79A8 = env;
        if (n >= env->targets->targetCount) {
            func_801E794C(env, 7, 1);
            return 1;
        }
        func_801E794C(env, 7, 0);
        ex->btlfx_playFileSound(1, entry->unk8, -1);
        ex->btlfx_playFileSound(5, entry->unk8, -1);
        ex->btlfx_playFileSound(7, entry->unk8, -1);
        ex->btlfx_getUnitPos(work->unk10, (s16 *)&work->unk8);
        work->unk0 = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->unk8, 8), (s32)&work->unk8);
        if (work->unk0 != NULL) {
            work->unk0->unk14 = (s32)D_801E79B8[work->unk10];
            work->unk0->unk12 = 0xA;
            work->unk0->unkC = 0x90;
        }
        if (work->unk0 != NULL) {
            work->unk0->unk22 = ex->func_800E7CC4(work->unk10, 0x20);
        }
    } else {
        frame = entry->unk0;
        if (frame == 0xF) {
            work->unk4 = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->unk8, 6), (s32)&work->unk8);
            if (work->unk4 != NULL) {
                work->unk4->unk22 = ex->func_800E7CC4(work->unk10, 0x20);
            }
        }
        if (frame == 0xA) {
            func_801E794C(env, 7, 1);
        }
        return frame >= 0x32;
    }
    return 0;
}

void func_801E794C(Unk800F78A4 *env, s32 i, s32 v) {
    *((u8 *)env->unk0 + i + 0x10) = v;
}

INCLUDE_RODATA("asm/us/fx_056/nonmatchings/fx_056", fx_056_headerPtr);
