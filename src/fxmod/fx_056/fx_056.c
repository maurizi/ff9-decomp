#include "fxmod/fx_056.h"

s32 fx_056_op80(s32 mode, Fx056Work *work, Unk800F78A4 *env, Unk80100C78Entry *entry, s32 *arg4) {
    BtlfxExports *ex = BTLFX_EXPORTS(env);
    s32 frame;

    if (mode == 0) {
        *arg4 = 0x14;
    } else if (mode == 1) {
        s32 n = *arg4;

        work->unk10 = n;
        fx_056_env = env;
        if (n >= env->targets->targetCount) {
            fxm_setSignal(env, 7, 1);
            return 1;
        }
        fxm_setSignal(env, 7, 0);
        ex->btlfx_playFileSound(1, entry->file, -1);
        ex->btlfx_playFileSound(5, entry->file, -1);
        ex->btlfx_playFileSound(7, entry->file, -1);
        ex->btlfx_getUnitPos(work->unk10, (s16 *)&work->pos8);
        work->emit0 = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, 8), (s32)&work->pos8);
        if (work->emit0 != NULL) {
            work->emit0->trails = (s32)D_801E79B8[work->unk10];
            work->emit0->trailSlots = 0xA;
            work->emit0->trailStride = 0x90;
        }
        if (work->emit0 != NULL) {
            work->emit0->otZ = ex->btlfx_addRadiusSixth(work->unk10, 0x20);
        }
    } else {
        frame = entry->frame;
        if (frame == 0xF) {
            work->emit4 = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, 6), (s32)&work->pos8);
            if (work->emit4 != NULL) {
                work->emit4->otZ = ex->btlfx_addRadiusSixth(work->unk10, 0x20);
            }
        }
        if (frame == 0xA) {
            fxm_setSignal(env, 7, 1);
        }
        return frame >= 0x32;
    }
    return 0;
}

static void fxm_setSignal(Unk800F78A4 *env, s32 i, s32 v) {
    *((u8 *)env->state + i + 0x10) = v;
}

INCLUDE_RODATA("asm/us/fx_056/nonmatchings/fx_056", fx_056_headerPtr);
