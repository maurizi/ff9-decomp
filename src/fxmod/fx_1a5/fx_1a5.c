#include "fxmod/fx_1a5.h"

s32 fx_1a5_op80(s32 mode, Fx1a5Work *work, Unk800F78A4 *env, Unk80100C78Entry *entry, s32 *args) {
    BtlfxExports *ex = BTLFX_EXPORTS(env);
    s32 frame;

    if (mode == 0) {
        *args = sizeof(Fx1a5Work);
    } else if (mode == 1) {
        s32 unit = *args;
        work->unit10 = unit;
        fx_1a5_env = env;
        if (unit >= env->targets->targetCount) {
            fxm_setSignal(env, BTLFX_SIG_WAIT, 1);
            return 1;
        }
        fxm_setSignal(env, BTLFX_SIG_WAIT, 0);
        ex->btlfx_playFileSound(6, entry->file, -1);
        ex->btlfx_playFileSound(7, entry->file, -1);
        ex->btlfx_playFileSound(8, entry->file, -1);
        ex->btlfx_getUnitPos(work->unit10, (s16 *)&work->pos8);
        work->emit0 = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, FX_REVIVE_LIGHT_SWIRL), (s32)&work->pos8);
        if (work->emit0 != NULL) {
            work->emit0->otZ = ex->btlfx_addRadiusSixth(work->unit10, 0x20);
        }
    } else {
        frame = entry->frame;
        if (frame == FX_REVIVE_FEATHERS_FRAME) {
            work->emit4 = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, FX_REVIVE_FEATHERS), (s32)&work->pos8);
            if (work->emit4 != NULL) {
                work->emit4->otZ = ex->btlfx_addRadiusSixth(work->unit10, 0x20);
            }
        }
        if (frame == FX_REVIVE_SIGNAL_FRAME) {
            fxm_setSignal(env, BTLFX_SIG_WAIT, 1);
        }
        return frame >= FX_REVIVE_FRAMES;
    }
    return 0;
}

static void fxm_setSignal(Unk800F78A4 *env, s32 signal, s32 value) {
    env->state->signals[signal] = value;
}

INCLUDE_RODATA("asm/us/fx_1a5/nonmatchings/fx_1a5", fx_1a5_headerPtr);
