#include "fxmod/fx_051.h"

s32 fx_051_op80(s32 mode, Fx051Work *work, Unk800F78A4 *env, Unk80100C78Entry *entry, s32 *args) {
    BtlfxExports *ex = BTLFX_EXPORTS(env);
    s32 frame;

    if (mode == 0) {
        *args = 0x1C;
    } else if (mode == 1) {
        s32 target = *args;

        work->target18 = target;
        fx_051_env = env;
        if (target >= env->targets->targetCount) {
            fxm_setSignal(env, BTLFX_SIG_WAIT, 1);
            return 1;
        }
        fxm_setSignal(env, BTLFX_SIG_WAIT, 0);
        ex->btlfx_playFileSound(1, entry->file, -1);
        ex->btlfx_playFileSound(5, entry->file, -1);
        ex->btlfx_playFileSound(6, entry->file, -1);
        ex->btlfx_getUnitBodyPos(work->target18, 0, (s16 *)&work->pos8);
        work->emit0 = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, 7), (s32)&work->pos8);
        if (work->emit0 != NULL) {
            work->emit0->otZ = ex->btlfx_addRadiusSixth(work->target18, 0x20);
        }
        ex->btlfx_getUnitRot(0, (s16 *)&work->rot10);
        fx_051_emitRot.vy = work->rot10.vy;
    } else {
        frame = entry->frame;
        if (frame == 0x12) {
            work->emit4 = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, 8), (s32)&work->pos8);
            if (work->emit4 != NULL) {
                work->emit4->otZ = ex->btlfx_addRadiusSixth(work->target18, 0x80);
            }
        }
        if (frame >= 0x12 && work->emit4 != NULL) {
            if (work->emit4->state == BTLFX_EMIT_DONE) {
                work->emit4 = NULL;
            }
            if (work->emit4 != NULL) {
                work->emit4->rot.vx = fx_051_emitRot.vx;
                work->emit4->rot.vy = fx_051_emitRot.vy;
                work->emit4->rot.vz = fx_051_emitRot.vz;
            }
        }
        if (frame == 0xC) {
            fxm_setSignal(env, BTLFX_SIG_WAIT, 1);
        }
        return frame >= 0x32;
    }
    return 0;
}

static void fxm_setSignal(Unk800F78A4 *env, s32 i, s32 v) {
    env->state->signals[i] = v;
}

INCLUDE_RODATA("asm/us/fx_051/nonmatchings/fx_051", fx_051_headerPtr);
