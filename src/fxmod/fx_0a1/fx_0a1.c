#include "fxmod/fx_0a1.h"

s32 fx_0a1_op80(s32 mode, Fx0a1Work *work, Unk800F78A4 *env, Unk80100C78Entry *entry, s32 *args) {
    BtlfxExports *ex = BTLFX_EXPORTS(env);
    SVECTOR pos;
    Pool6CEntry *emitter;
    s32 frame;
    s32 t;

    if (mode == 0) {
        *args = sizeof(Fx0a1Work);
    } else if (mode == 1) {
        work->state0 = FX_BLASTER_STATE_BEAM;
        work->unk4 = 0;
        fx_0a1_env = env;
        ex->btlfx_getUnitBodyPos(0, 0, (s16 *)&work->pos8);
        ex->btlfx_getUnitPos(0, (s16 *)&work->pos10);
        ex->btlfx_getUnitArgBonePos(BTLFX_UNIT_CASTER, 0, (u16 *)&work->pos18);
        ex->btlfx_aimAngles((s16 *)&work->pos8, (s16 *)&work->pos18, (s16 *)&work->aim28);
    } else {
        frame = entry->frame;
        switch (work->state0) {
        case FX_BLASTER_STATE_BEAM:
            t = (frame << 12) / FX_BLASTER_BEAM_FRAMES;
            ex->btlfx_getUnitArgBonePos(BTLFX_UNIT_CASTER, 0, (u16 *)&work->pos20);
            if (frame == 0) {
                emitter = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, FX_BLASTER_VIOLET_RING), (s32)&work->pos20);
                if (emitter != NULL) {
                    emitter->otZ = ex->btlfx_addRadiusSixth(BTLFX_UNIT_CASTER, 0x40);
                }
            }
            ex->btlfx_lerpVec((s16 *)&work->pos18, (s16 *)&work->pos8, 0x1000 - ex->math_cos(t >> 2),
                              (s16 *)&pos);
            if (frame == 0) {
                ex->btlfx_drawTrail((u8 *)ex->btlfx_getFileData(entry->file, FX_BLASTER_TRAIL_2), 0, NULL, -1, 0x1000, 0, BTLFX_ABR_ADD, fx_0a1_trail, 0);
            }
            ex->btlfx_drawTrail((u8 *)ex->btlfx_getFileData(entry->file, FX_BLASTER_TRAIL_2), (s32)&pos, NULL, 0, 0x1000, 0x1000, BTLFX_ABR_ADD,
                              fx_0a1_trail, 0);
            if (frame >= FX_BLASTER_BEAM_FRAMES) {
                work->state0 = FX_BLASTER_STATE_BURST;
                entry->frame = -1;
                emitter = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, FX_BLASTER_PINK_HALO), (s32)&work->pos8);
                if (emitter != NULL) {
                    emitter->otZ = ex->btlfx_addRadiusSixth(0, 0x40);
                }
                emitter = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, FX_BLASTER_PINK_HALO_2), (s32)&work->pos8);
                if (emitter != NULL) {
                    emitter->otZ = ex->btlfx_addRadiusSixth(0, 0x40);
                }
                emitter = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, FX_BLASTER_LIGHT_CONE), (s32)&work->pos8);
                if (emitter != NULL) {
                    emitter->otZ = ex->btlfx_addRadiusSixth(0, 0x40);
                }
            }
            break;
        case FX_BLASTER_STATE_BURST:
            if (frame == FX_BLASTER_STARBURST_FRAME) {
                emitter = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, FX_BLASTER_PINK_STARBURST), (s32)&work->pos8);
                if (emitter != NULL) {
                    emitter->otZ = ex->btlfx_addRadiusSixth(0, 0x40);
                    emitter->trails = (s32)fx_0a1_trails;
                    emitter->trailSlots = FX_BLASTER_PINK_STARBURST_TRAIL_SLOTS;
                    emitter->trailStride = FX_BLASTER_PINK_STARBURST_TRAIL_STRIDE;
                }
                emitter = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, FX_BLASTER_VIOLET_PETALS), (s32)&work->pos8);
                if (emitter != NULL) {
                    emitter->otZ = ex->btlfx_addRadiusSixth(0, 0x40);
                }
            }
            if (frame == FX_BLASTER_TEX_RING_FRAME) {
                emitter = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, FX_BLASTER_TEX_RING), (s32)&work->pos8);
                if (emitter != NULL) {
                    emitter->otZ = ex->btlfx_addRadiusSixth(0, 0x40);
                }
            }
            if (frame == FX_BLASTER_HAZE_FRAME) {
                emitter = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, FX_BLASTER_WHITE_HAZE), (s32)&work->pos8);
                if (emitter != NULL) {
                    emitter->otZ = ex->btlfx_addRadiusSixth(0, 0x40);
                    emitter->trails = (s32)fx_0a1_trails2;
                    emitter->trailSlots = FX_BLASTER_WHITE_HAZE_TRAIL_SLOTS;
                    emitter->trailStride = FX_BLASTER_WHITE_HAZE_TRAIL_STRIDE;
                }
                ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, FX_BLASTER_GREY_RING), (s32)&work->pos10);
            }
            if (frame >= FX_BLASTER_BURST_FRAMES) {
                return 1;
            }
            break;
        }
    }
    return 0;
}

INCLUDE_RODATA("asm/us/fx_0a1/nonmatchings/fx_0a1", fx_0a1_headerPtr);
