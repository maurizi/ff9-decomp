#include "fxmod/fx_0ae.h"

s32 fx_0ae_op80(s32 mode, Fx0aeWork *work, Unk800F78A4 *env, Unk80100C78Entry *entry, s32 *args) {
    BtlfxExports *ex = BTLFX_EXPORTS(env);
    BtlBone bone;
    Pool6CEntry *emitter;
    s32 frame;

    if (mode == 0) {
        *args = sizeof(Fx0aeWork);
    } else if (mode == 1) {
        work->state0 = FX_VENOM_BREATH_STATE_BLOW;
        fx_0ae_env = env;
        ex->btlfx_getUnitBodyPos(0, 0, (s16 *)&work->pos10);
        ex->btlfx_getUnitPos(0, (s16 *)&work->pos18);
        ex->btlfx_getUnitBoneMatrix(0x10, env->targets->fxArgs[0], &bone);
        ex->btlfx_matrixPointAhead((MATRIX *)&bone, (s16)env->targets->fxParam, (s32)&work->pos8);
        ex->btlfx_aimAngles((s16 *)&work->pos8, (s16 *)&work->pos10, (s16 *)&work->aim20);
        work->emit4 = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, FX_VENOM_BREATH_VENOM_CLOUD), (s32)&work->pos8);
        if (work->emit4 != NULL) {
            work->emit4->trails = (s32)fx_0ae_trails;
            work->emit4->trailSlots = FX_VENOM_BREATH_VENOM_CLOUD_TRAIL_SLOTS;
            work->emit4->trailStride = FX_VENOM_BREATH_VENOM_CLOUD_TRAIL_STRIDE;
        }
        if (work->emit4 != NULL) {
            work->emit4->otZ = 0x20;
        }
    } else {
        frame = entry->frame;
        switch (work->state0) {
        case FX_VENOM_BREATH_STATE_BLOW:
            ex->btlfx_getUnitBoneMatrix(0x10, env->targets->fxArgs[0], &bone);
            ex->btlfx_matrixPointAhead((MATRIX *)&bone, (s16)env->targets->fxParam, (s32)&work->pos8);
            if (work->emit4 != NULL) {
                if (work->emit4->state == BTLFX_EMIT_DONE) {
                    work->emit4 = NULL;
                }
                if (work->emit4 != NULL) {
                    work->emit4->rot.vx = work->aim20.vx;
                    work->emit4->rot.vy = work->aim20.vy;
                    work->emit4->rot.vz = work->aim20.vz;
                }
            }
            if (frame == FX_VENOM_BREATH_SPLASH_FRAME) {
                emitter = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, FX_VENOM_BREATH_VENOM_SPLASH), (s32)&work->pos18);
                if (emitter != NULL) {
                    emitter->otZ = ex->btlfx_addRadiusSixth(0, 0x20);
                }
            }
            if (frame >= FX_VENOM_BREATH_FRAMES) {
                return 1;
            }
            break;
        }
    }
    return 0;
}

INCLUDE_RODATA("asm/us/fx_0ae/nonmatchings/fx_0ae", fx_0ae_headerPtr);
