#include "fxmod/fx_056.h"

s32 fx_056_op80(s32 mode, Fx056Work *work, Unk800F78A4 *env, Unk80100C78Entry *entry, s32 *args) {
    BtlfxExports *ex = BTLFX_EXPORTS(env);
    s32 frame;

    if (mode == 0) {
        *args = sizeof(Fx056Work);
    } else if (mode == 1) {
        s32 target = *args;

        work->target10 = target;
        fx_056_env = env;
        if (target >= env->targets->targetCount) {
            fxm_setSignal(env, BTLFX_SIG_WAIT, 1);
            return 1;
        }
        fxm_setSignal(env, BTLFX_SIG_WAIT, 0);
        ex->btlfx_playFileSound(1, entry->file, -1);
        ex->btlfx_playFileSound(5, entry->file, -1);
        ex->btlfx_playFileSound(7, entry->file, -1);
        ex->btlfx_getUnitPos(work->target10, (s16 *)&work->pos8);
        work->emit0 = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, FX_CHAKRA_STAR_GLINTS), (s32)&work->pos8);
        if (work->emit0 != NULL) {
            work->emit0->trails = (s32)fx_056_trails[work->target10];
            work->emit0->trailSlots = FX_CHAKRA_STAR_GLINTS_TRAIL_SLOTS;
            work->emit0->trailStride = FX_CHAKRA_STAR_GLINTS_TRAIL_STRIDE;
        }
        if (work->emit0 != NULL) {
            work->emit0->otZ = ex->btlfx_addRadiusSixth(work->target10, 0x20);
        }
    } else {
        frame = entry->frame;
        if (frame == FX_CHAKRA_DOT_RINGS_FRAME) {
            work->emit4 = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, FX_CHAKRA_DOT_RINGS), (s32)&work->pos8);
            if (work->emit4 != NULL) {
                work->emit4->otZ = ex->btlfx_addRadiusSixth(work->target10, 0x20);
            }
        }
        if (frame == FX_CHAKRA_RELEASE_FRAME) {
            fxm_setSignal(env, BTLFX_SIG_WAIT, 1);
        }
        return frame >= FX_CHAKRA_FRAMES;
    }
    return 0;
}

static void fxm_setSignal(Unk800F78A4 *env, s32 signal, s32 value) {
    env->state->signals[signal] = value;
}

INCLUDE_RODATA("asm/us/fx_056/nonmatchings/fx_056", fx_056_headerPtr);
