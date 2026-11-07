#include "fxmod/fx_187.h"

s32 fx_187_op80(s32 mode, Fx187Work *work, Unk800F78A4 *env, Unk80100C78Entry *entry, s32 *args) {
    BtlfxExports *ex = BTLFX_EXPORTS(env);
    SVECTOR handPos0;
    SVECTOR handPos1;
    Pool6CEntry *emitter;
    s32 frame;
    s32 t;
    s32 i;

    if (mode == 0) {
        *args = sizeof(Fx187Work);
    } else if (mode == 1) {
        work->state0 = FX_FIRE_BLADES_STATE_IGNITE;
        fx_187_env = env;
        BTLFX_COPY_SVEC(&work->pos14, &env->targets->centerX);
        BTLFX_COPY_SVEC(&work->pos1C, &work->pos14);
        work->pos14.vy -= 0xF0;
        BTLFX_COPY_SVEC(&work->pos34, &work->pos14);
        BTLFX_COPY_SVEC(&work->pos3C, &work->pos14);
        work->pos34.vx -= 0x300;
        work->pos3C.vx += 0x300;
        ex->btlfx_getUnitBonePos(BTLFX_UNIT_CASTER, fx_187_bones[0], (u16 *)&work->pos24);
        ex->btlfx_getUnitBonePos(BTLFX_UNIT_CASTER, fx_187_bones[1], (u16 *)&work->pos2C);
    } else {
        frame = entry->frame;
        ex->btlfx_getUnitBonePos(BTLFX_UNIT_CASTER, fx_187_bones[0], (u16 *)&work->pos24);
        ex->btlfx_getUnitBonePos(BTLFX_UNIT_CASTER, fx_187_bones[1], (u16 *)&work->pos2C);
        switch (work->state0) {
        case FX_FIRE_BLADES_STATE_IGNITE:
            if (frame == 0) {
                emitter = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, FX_FIRE_BLADES_FLAME_BURST), (s32)&work->pos24);
                if (emitter != NULL) {
                    emitter->otZ = 0x80;
                }
                emitter = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, FX_FIRE_BLADES_FLAME_BURST_2), (s32)&work->pos2C);
                if (emitter != NULL) {
                    emitter->otZ = 0x80;
                }
            }
            if (frame >= FX_FIRE_BLADES_IGNITE_FRAMES) {
                work->state0 = FX_FIRE_BLADES_STATE_SWING;
                entry->frame = -1;
                emitter = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, FX_FIRE_BLADES_FIRE_BLADE_2), (s32)&work->pos24);
                if (emitter != NULL) {
                    emitter->trails = (s32)fx_187_op80Trails0;
                    emitter->trailSlots = FX_FIRE_BLADES_FIRE_BLADE_TRAIL_SLOTS;
                    emitter->trailStride = FX_FIRE_BLADES_FIRE_BLADE_TRAIL_STRIDE;
                    emitter->otZ = 0x46;
                }
                emitter = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, FX_FIRE_BLADES_SPARKS_2), (s32)&work->pos24);
                if (emitter != NULL) {
                    emitter->otZ = 0x80;
                }
                emitter = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, FX_FIRE_BLADES_FIRE_BLADE), (s32)&work->pos2C);
                if (emitter != NULL) {
                    emitter->trails = (s32)fx_187_op80Trails1;
                    emitter->trailSlots = FX_FIRE_BLADES_FIRE_BLADE_TRAIL_SLOTS;
                    emitter->trailStride = FX_FIRE_BLADES_FIRE_BLADE_TRAIL_STRIDE;
                    emitter->otZ = 0x46;
                }
                emitter = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, FX_FIRE_BLADES_SPARKS), (s32)&work->pos2C);
                if (emitter != NULL) {
                    emitter->otZ = 0x80;
                }
            }
            break;
        case FX_FIRE_BLADES_STATE_SWING:
            if (frame < FX_FIRE_BLADES_SWING_EASE_FRAMES) {
                t = (0x1000 - ex->math_cos(frame << 7)) >> 1;
            } else {
                t = 0x1000;
            }
            ex->btlfx_lerpVec((s16 *)&work->pos24, (s16 *)&work->pos34, t, (s16 *)&handPos0);
            ex->btlfx_lerpVec((s16 *)&work->pos2C, (s16 *)&work->pos3C, t, (s16 *)&handPos1);
            if (frame == 0) {
                BTLFX_COPY_SVEC(&work->pos44, &handPos0);
                work->emit4[0] = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, FX_FIRE_BLADES_FIRE_BLADE_3), (s32)&work->pos44);
                if (work->emit4[0] != NULL) {
                    work->emit4[0]->trails = (s32)fx_187_op80Trails2;
                    work->emit4[0]->trailSlots = FX_FIRE_BLADES_FIRE_BLADE_3_TRAIL_SLOTS;
                    work->emit4[0]->trailStride = FX_FIRE_BLADES_FIRE_BLADE_3_TRAIL_STRIDE;
                }
                if (work->emit4[0] != NULL) {
                    work->emit4[0]->otZ = 0x18;
                }
                BTLFX_COPY_SVEC(&work->pos4C, &handPos0);
                work->emit4[1] = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, FX_FIRE_BLADES_SPARKS_3), (s32)&work->pos4C);
                if (work->emit4[1] != NULL) {
                    work->emit4[1]->otZ = 0x18;
                }
                BTLFX_COPY_SVEC(&work->pos54, &handPos1);
                work->emitC[0] = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, FX_FIRE_BLADES_FIRE_BLADE_4), (s32)&work->pos54);
                if (work->emitC[0] != NULL) {
                    work->emitC[0]->trails = (s32)fx_187_op80Trails3;
                    work->emitC[0]->trailSlots = FX_FIRE_BLADES_FIRE_BLADE_3_TRAIL_SLOTS;
                    work->emitC[0]->trailStride = FX_FIRE_BLADES_FIRE_BLADE_3_TRAIL_STRIDE;
                }
                if (work->emitC[0] != NULL) {
                    work->emitC[0]->otZ = 0x18;
                }
                BTLFX_COPY_SVEC(&work->pos5C, &handPos1);
                work->emitC[1] = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, FX_FIRE_BLADES_SPARKS_4), (s32)&work->pos5C);
                if (work->emitC[1] != NULL) {
                    work->emitC[1]->otZ = 0x18;
                }
            } else {
                for (i = 0; i < 2; i++) {
                    if (work->emit4[i] != NULL && work->emit4[i]->state == BTLFX_EMIT_DONE) {
                        work->emit4[i] = NULL;
                    }
                    ex->btlfx_moveEmitterTo((s32)work->emit4[i], (s32)&handPos0);
                    if (work->emitC[i] != NULL && work->emitC[i]->state == BTLFX_EMIT_DONE) {
                        work->emitC[i] = NULL;
                    }
                    ex->btlfx_moveEmitterTo((s32)work->emitC[i], (s32)&handPos1);
                }
            }
            if (frame == FX_FIRE_BLADES_BURST_FRAME) {
                emitter = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, FX_FIRE_BLADES_FLAME_BURST_3), (s32)&work->pos14);
                if (emitter != NULL) {
                    emitter->otZ = 0x80;
                }
                ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, FX_FIRE_BLADES_RED_RING), (s32)&work->pos1C);
                emitter = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, FX_FIRE_BLADES_FLAME_TRAIL), (s32)&work->pos14);
                if (emitter != NULL) {
                    emitter->trails = (s32)fx_187_op80Trails4;
                    emitter->trailSlots = FX_FIRE_BLADES_FLAME_TRAIL_TRAIL_SLOTS;
                    emitter->trailStride = FX_FIRE_BLADES_FLAME_TRAIL_TRAIL_STRIDE;
                    emitter->otZ = 0x80;
                }
            }
            if (frame >= FX_FIRE_BLADES_SWING_FRAMES) {
                return 1;
            }
            break;
        }
    }
    return 0;
}

INCLUDE_RODATA("asm/us/fx_187/nonmatchings/fx_187", fx_187_headerPtr);
