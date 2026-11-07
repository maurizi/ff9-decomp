#include "fxmod/fx_12f.h"

s32 fx_12f_op80(s32 mode, Fx12fWork *work, Unk800F78A4 *env, Unk80100C78Entry *entry, s32 *args) {
    BtlfxExports *ex = BTLFX_EXPORTS(env);
    Pool6CEntry *emitter;
    s32 frame;
    s32 i;

    if (mode == 0) {
        *args = sizeof(Fx12fWork);
    } else if (mode == 1) {
        fx_12f_env = env;
        ex->btlfx_getUnitArgBonePos(BTLFX_UNIT_CASTER, 0, (u16 *)&work->pos8);
        work->pos18.vx = env->targets->centerX;
        work->pos18.vz = env->targets->centerZ;
        work->pos18.vy = 0;
        work->emit40 = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, FX_PAPER_STORM_PAPER_SHEETS), (s32)&work->pos8);
        ex->btlfx_aimAngles((s16 *)&work->pos10, (s16 *)&work->pos18, (s16 *)&work->rot0);
    } else {
        frame = entry->frame;
        if (work->emit40 != NULL) {
            if (work->emit40->state == BTLFX_EMIT_DONE) {
                work->emit40 = NULL;
            }
        }
        ex->btlfx_getUnitArgBonePos(BTLFX_UNIT_CASTER, 0, (u16 *)&work->pos10);
        ex->btlfx_moveEmitterTo((s32)work->emit40, (s32)&work->pos10);
        if (work->emit40 != NULL) {
            work->emit40->rot.vx = work->rot0.vx;
            work->emit40->rot.vy = work->rot0.vy;
            work->emit40->rot.vz = work->rot0.vz;
        }
        if (frame == FX_PAPER_STORM_SWIRL_FRAME) {
            ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, FX_PAPER_STORM_PAPER_SWIRL), (s32)&work->pos18);
            for (i = 0; i < env->targets->targetCount; i++) {
                ex->btlfx_getUnitPos(i, (s16 *)&work->pos20[i]);
                emitter = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, FX_PAPER_STORM_ORANGE_BURST), (s32)&work->pos20[i]);
                if (emitter != NULL) {
                    emitter->otZ = ex->btlfx_addRadiusSixth(i, 0x80);
                }
            }
        }
        if (frame == FX_PAPER_STORM_BURST_FRAME) {
            for (i = 0; i < env->targets->targetCount; i++) {
                emitter = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, FX_PAPER_STORM_GREEN_BURST), (s32)&work->pos20[i]);
                if (emitter != NULL) {
                    emitter->otZ = ex->btlfx_addRadiusSixth(i, 0x80);
                }
            }
        }
        return frame >= FX_PAPER_STORM_FRAMES;
    }
    return 0;
}

INCLUDE_RODATA("asm/us/fx_12f/nonmatchings/fx_12f", fx_12f_headerPtr);
