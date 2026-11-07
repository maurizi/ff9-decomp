#include "fxmod/fx_002.h"

s32 fx_002_op80(s32 mode, Fx002Work *work, Unk800F78A4 *env, Unk80100C78Entry *entry, s32 *args) {
    BtlfxExports *ex = BTLFX_EXPORTS(env);
    Pool6CEntry *emitter;
    s32 frame;
    s32 scale;
    SVECTOR rot;
    s32 data;
    s32 t;

    if (mode == 0) {
        *args = sizeof(Fx002Work);
    } else if (mode == 1) {
        work->hold0 = 0;
        fx_002_env = env;
        ex->btlfx_getUnitPos(0, (s16 *)&work->pos4);
        ex->btlfx_getUnitBodyPos(0, 0, (s16 *)&work->posC);
        emitter = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, FX_FIRA_FLAME_BURST), (s32)&work->posC);
        if (emitter != NULL) {
            emitter->otZ = ex->btlfx_addRadiusSixth(0, 0x20);
        }
    } else {
        frame = entry->frame;
        if (work->hold0 == 0) {
            if (frame < FX_FIRA_GROUND_RING_FRAMES) {
                t = (frame << 12) / FX_FIRA_GROUND_RING_FRAMES;
                if (frame < FX_FIRA_RING_PULSE_FRAME) {
                    scale = ex->math_sin(frame << 6) / 4 + 0x800;
                } else {
                    scale = ex->math_sin(((frame - FX_FIRA_RING_PULSE_FRAME) << 10) / (FX_FIRA_GROUND_RING_FRAMES - FX_FIRA_RING_PULSE_FRAME)) * 2 / 3 + 0xC00;
                }
                rot.vx = 0x400;
                rot.vz = 0;
                rot.vy = 0;
                rot.pad = 1;
                data = ex->btlfx_getFileData(entry->file, FX_FIRA_GROUND_RING);
                ex->btlfx_drawRings((u8 *)data, &work->pos4, &rot, scale * 2 / 3, 0xC, BTLFX_ABR_ADD, ex->math_sin(t >> 1));
            }
            if (frame == FX_FIRA_SMOKE_FRAME) {
                emitter = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, FX_FIRA_SMOKE), (s32)&work->posC);
                if (emitter != NULL) {
                    emitter->otZ = ex->btlfx_addRadiusSixth(0, 0x20);
                }
            }
            if (frame == FX_FIRA_FLAME_SWIRL_FRAME) {
                Pool6CEntry *em;

                em = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, FX_FIRA_FLAME_SWIRL), (s32)&work->pos4);
                if (em != NULL) {
                    em->otZ = 0x80;
                    em->trails = (s32)fx_002_trails;
                    em->trailSlots = FX_FIRA_FLAME_SWIRL_TRAIL_SLOTS;
                    em->trailStride = FX_FIRA_FLAME_SWIRL_TRAIL_STRIDE;
                }
                em = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, FX_FIRA_FLAME_BURST_2), (s32)&work->posC);
                if (em != NULL) {
                    em->otZ = 0x80;
                }
                ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, FX_FIRA_HEAT_RING), (s32)&work->pos4);
            }
            if (frame == FX_FIRA_SMOKE_2_FRAME) {
                emitter = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, FX_FIRA_SMOKE_2), (s32)&work->posC);
                if (emitter != NULL) {
                    emitter->otZ = ex->btlfx_addRadiusSixth(0, 0x20);
                }
            }
            if (frame >= FX_FIRA_FRAMES) {
                return 1;
            }
        }
    }
    return 0;
}

INCLUDE_RODATA("asm/us/fx_002/nonmatchings/fx_002", fx_002_headerPtr);
