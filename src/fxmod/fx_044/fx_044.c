#include "fxmod/fx_044.h"

s32 fx_044_op80(s32 mode, Fx044Work80 *work, Unk800F78A4 *env, Unk80100C78Entry *entry, s32 *args) {
    BtlfxExports *ex = BTLFX_EXPORTS(env);
    s32 frame;

    if (mode == 0) {
        *args = sizeof(Fx044Work80);
    } else if (mode == 1) {
        fx_044_env = env;
        ex->btlfx_getUnitBodyPos(0, 0, (s16 *)&work->pos8);
        ex->btlfx_getUnitArgBonePos(0x10, 0, (u16 *)&work->pos10);
        work->emit0 = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, FX_ULTRA_SOUND_WAVE_PINK_RINGS), (s32)&work->pos10);
        work->emit4 = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, FX_ULTRA_SOUND_WAVE_VIOLET_HALO), (s32)&work->pos10);
        ex->btlfx_aimAngles((s16 *)&work->pos10, (s16 *)&work->pos8, (s16 *)&work->aim18);
    } else {
        frame = entry->frame;
        if (work->emit4 != NULL) {
            if (work->emit4->state == BTLFX_EMIT_DONE) {
                work->emit4 = NULL;
            }
            if (work->emit4 != NULL) {
                work->emit4->rot.vx = work->aim18.vx;
                work->emit4->rot.vy = work->aim18.vy;
                work->emit4->rot.vz = work->aim18.vz;
            }
        }
        return frame >= FX_ULTRA_SOUND_WAVE_FRAMES;
    }
    return 0;
}

s32 fx_044_op81(s32 mode, Fx044Work81 *work, Unk800F78A4 *env, Unk80100C78Entry *entry, s32 *args) {
    BtlfxExports *ex = BTLFX_EXPORTS(env);

    if (mode == 0) {
        *args = sizeof(Fx044Work81);
    } else if (mode == 1) {
        fx_044_env = env;
        ex->btlfx_getUnitBodyPos(0, 0, (s16 *)&work->posC);
        ex->btlfx_getUnitPos(0, (s16 *)&work->pos14);
        work->pos1C.vx = work->posC.vx;
        work->pos1C.vy = work->posC.vy;
        work->pos1C.vz = work->posC.vz;
        work->pos1C.vy -= FX_ULTRA_SOUND_WAVE_RING_HEIGHT;
        work->emit0 = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, FX_ULTRA_SOUND_WAVE_RAINBOW_RING), (s32)&work->pos1C);
        work->emit4 = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, FX_ULTRA_SOUND_WAVE_SWIRL), (s32)&work->pos14);
        work->emit8 = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, FX_ULTRA_SOUND_WAVE_WISPS), (s32)&work->posC);
        if (work->emit0 != NULL) {
            work->emit0->trails = (s32)fx_044_trails;
            work->emit0->trailSlots = FX_ULTRA_SOUND_WAVE_RAINBOW_RING_TRAIL_SLOTS;
            work->emit0->trailStride = FX_ULTRA_SOUND_WAVE_RAINBOW_RING_TRAIL_STRIDE;
        }
    } else {
        return entry->frame >= FX_ULTRA_SOUND_WAVE_HIT_FRAMES;
    }
    return 0;
}

INCLUDE_RODATA("asm/us/fx_044/nonmatchings/fx_044", fx_044_headerPtr);
