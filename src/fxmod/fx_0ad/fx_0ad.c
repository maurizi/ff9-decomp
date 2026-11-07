#include "fxmod/fx_0ad.h"

s32 fx_0ad_op80(s32 mode, Fx0adWork *work, Unk800F78A4 *env, Unk80100C78Entry *entry, s32 *args) {
    BtlfxExports *ex = BTLFX_EXPORTS(env);
    Pool6CEntry *emitter;
    s32 frame;

    if (mode == 0) {
        *args = sizeof(Fx0adWork);
    } else if (mode == 1) {
        work->state0 = FX_AERO_BREATH_STATE_BLOW;
        fx_0ad_env = env;
        ex->btlfx_getUnitBodyPos(0, 0, (s16 *)&work->pos10);
        ex->btlfx_getUnitArgBonePos(BTLFX_UNIT_CASTER, 0, (u16 *)&work->pos8);
        ex->btlfx_aimAngles((s16 *)&work->pos8, (s16 *)&work->pos10, (s16 *)&work->aim18);
        work->emit4 = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, FX_AERO_BREATH_WIND_TRAILS), (s32)&work->pos8);
        if (work->emit4 != NULL) {
            work->emit4->trails = (s32)fx_0ad_trails;
            work->emit4->trailSlots = FX_AERO_BREATH_WIND_TRAILS_TRAIL_SLOTS;
            work->emit4->trailStride = FX_AERO_BREATH_WIND_TRAILS_TRAIL_STRIDE;
        }
        if (work->emit4 != NULL) {
            work->emit4->otZ = 0x20;
        }
    } else {
        frame = entry->frame;
        switch (work->state0) {
        case FX_AERO_BREATH_STATE_BLOW:
            if (work->emit4 != NULL) {
                work->emit4->rot.vx = work->aim18.vx;
                work->emit4->rot.vy = work->aim18.vy;
                work->emit4->rot.vz = work->aim18.vz;
            }
            if (frame == FX_AERO_BREATH_SWIRL_FRAME) {
                emitter = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, FX_AERO_BREATH_SWIRL), (s32)&work->pos10);
                if (emitter != NULL) {
                    emitter->otZ = ex->btlfx_addRadiusSixth(0, 0x10);
                }
            }
            if (frame >= FX_AERO_BREATH_FRAMES) {
                return 1;
            }
            break;
        }
    }
    return 0;
}

INCLUDE_RODATA("asm/us/fx_0ad/nonmatchings/fx_0ad", fx_0ad_headerPtr);
