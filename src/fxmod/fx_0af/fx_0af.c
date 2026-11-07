#include "fxmod/fx_0af.h"

s32 fx_0af_op80(s32 mode, Fx0afWork *work, Unk800F78A4 *env, Unk80100C78Entry *entry, s32 *args) {
    BtlfxExports *ex = BTLFX_EXPORTS(env);
    Pool6CEntry *emitter;
    s32 frame;

    if (mode == 0) {
        *args = sizeof(Fx0afWork);
    } else if (mode == 1) {
        work->state0 = FX_ZOMBIE_BREATH_STATE_BLOW;
        fx_0af_env = env;
        ex->btlfx_getUnitBodyPos(0, 0, (s16 *)&work->pos10);
        ex->btlfx_getUnitArgBonePos(0x10, 0, (u16 *)&work->pos8);
        ex->btlfx_aimAngles((s16 *)&work->pos8, (s16 *)&work->pos10, (s16 *)&work->aim18);
        work->emit4 = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, FX_ZOMBIE_BREATH_BREATH_TRAILS), (s32)&work->pos8);
        if (work->emit4 != NULL) {
            work->emit4->trails = (s32)fx_0af_trails;
            work->emit4->trailSlots = FX_ZOMBIE_BREATH_TRAIL_SLOTS;
            work->emit4->trailStride = FX_ZOMBIE_BREATH_TRAIL_STRIDE;
        }
        if (work->emit4 != NULL) {
            work->emit4->otZ = 0x20;
        }
    } else {
        frame = entry->frame;
        switch (work->state0) {
        case FX_ZOMBIE_BREATH_STATE_BLOW:
            if (work->emit4 != NULL) {
                work->emit4->rot.vx = work->aim18.vx;
                work->emit4->rot.vy = work->aim18.vy;
                work->emit4->rot.vz = work->aim18.vz;
            }
            if (frame == FX_ZOMBIE_BREATH_GLOW_FRAME) {
                emitter = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, FX_ZOMBIE_BREATH_GREEN_GLOW), (s32)&work->pos10);
                if (emitter != NULL) {
                    emitter->otZ = ex->btlfx_addRadiusSixth(0, 0x20);
                }
                emitter = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, FX_ZOMBIE_BREATH_BREATH_TRAILS_2), (s32)&work->pos10);
                if (emitter != NULL) {
                    emitter->trails = (s32)fx_0af_trails2;
                    emitter->trailSlots = FX_ZOMBIE_BREATH_TRAIL_SLOTS;
                    emitter->trailStride = FX_ZOMBIE_BREATH_TRAIL_STRIDE;
                    emitter->otZ = 0x20;
                }
            }
            if (frame >= FX_ZOMBIE_BREATH_FRAMES) {
                return 1;
            }
            break;
        }
    }
    return 0;
}

INCLUDE_RODATA("asm/us/fx_0af/nonmatchings/fx_0af", fx_0af_headerPtr);
