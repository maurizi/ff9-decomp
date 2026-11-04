#include "fxmod/fx_188.h"

static s32 fx_188_op80PoolTask(s32 mode, Fx188Elem *it, Unk800F78A4 *env, Unk80100C78Entry *entry, s32 frame) {
    BtlfxExports *ex = BTLFX_EXPORTS(env);
    SVECTOR ang;
    s32 size;

    if (mode == 1) {
        it->state0 = 0;
        it->emit1C = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, FX_LV3_DEF_LESS_VIOLET_ORBS), (s32)&it->posC);
        if (it->emit1C != NULL) {
            it->emit1C->otZ = 0;
        }
        it->speed4 = (ex->btlfx_rand() & 0x1F) + 0x27;
        it->turn8 = (ex->btlfx_rand() & 0x1F) + 0x20;
    } else if (it->state0 == 0) {
        it->speed4 += 0xC;
        if (it->speed4 > 0x140) {
            it->speed4 = 0x140;
        }
        it->turn8 += 0x1D;
        if (it->turn8 > 0x22C) {
            it->turn8 = 0x22D;
        }
        size = 0x1000;
        if (frame < 5) {
            size = frame << 10;
        }
        size = (size * fx_188_particleScale) >> 12;
        ex->btlfx_aimAngles((s16 *)&it->posC, (s16 *)&it->target20, (s16 *)&ang);
        ex->btlfx_turnToward((s16 *)&ang, (s16 *)&it->rot14, it->turn8);
        ex->btlfx_pointAhead((s16 *)&it->posC, (s16 *)&it->rot14, it->speed4, (s16 *)&it->posC);
        ex->btlfx_drawParticles((u8 *)ex->btlfx_getFileData(entry->file, FX_LV3_DEF_LESS_VIOLET_ORB), &it->posC, 0, size + 0x800, frame, 1, -1, 0, 0);
        ex->btlfx_drawParticles((u8 *)ex->btlfx_getFileData(entry->file, FX_LV3_DEF_LESS_PARTICLE), &it->posC, 0, size + 0x800, frame, 2, -1, 0, 0);
        if (it->emit1C != NULL && it->emit1C->state == BTLFX_EMIT_DONE) {
            it->emit1C = NULL;
        }
        if (fx_188_particleScale == 0) {
            if (it->emit1C != NULL) {
                it->emit1C->frame = it->emit1C->tail->anim.duration;
            }
            it->state0 = 1;
            return 1;
        }
    }
    return 0;
}

s32 fx_188_op80(s32 mode, Fx188Work *work, Unk800F78A4 *env, Unk80100C78Entry *entry, EffInit *init) {
    BtlfxExports *ex = BTLFX_EXPORTS(env);
    Fx188Elem *it;
    SVECTOR ringRot;
    Pool6CEntry *emitter;
    s32 frame;
    s32 ringScale;
    s32 ringAlpha;
    s32 cell;

    if (mode == 0) {
        init->workSize = 0x20;
        init->elemSize = 0x28;
        init->elemFn = (s32)fx_188_op80PoolTask;
        init->poolCount = 0xC;
    } else if (mode == 1) {
        work->state0 = 0;
        work->count1C = 0;
        fx_188_env = env;
        BTLFX_COPY_SVEC(&work->pos4, &env->targets->centerX);
        work->pos4.vy -= 0x200;
        ex->btlfx_getUnitBodyPos(0x10, 0, (s16 *)&work->posC);
        ex->btlfx_getUnitRot(0x10, (s16 *)&work->rot14);
        fx_188_particleScale = 0x1000;
        work->rot14.vy += 0x800;
    } else {
        frame = entry->frame;
        switch (work->state0) {
        case 0:
            if (frame & 1) {
                if (work->count1C < 0xC) {
                    work->count1C++;
                    it = ex->btlfx_poolSpawn((s16 *)entry->pool);
                    if (it != NULL) {
                        BTLFX_COPY_SVEC(&it->posC, &work->posC);
                        it->target20.vx = work->pos4.vx;
                        it->target20.vz = work->pos4.vz;
                        it->target20.vy = work->pos4.vy;
                        cell = work->count1C & 3;
                        it->rot14.vy = (s16)(work->rot14.vy - 0x300) + (cell << 9);
                        cell = work->count1C >> 2;
                        it->rot14.vx = work->rot14.vx + cell * 0x180;
                        it->rot14.vz = 0;
                    }
                }
            }
            if (frame >= 0x3E) {
                work->state0 = 1;
                entry->frame = -1;
            }
            break;
        case 1:
            ringAlpha = frame << 9;
            if (frame < 8) {
                ringScale = 0x1000;
                fx_188_particleScale = ex->math_cos(frame << 7);
            } else {
                ringScale = ex->math_cos((frame - 8) << 7);
                ringAlpha = 0x1000;
                fx_188_particleScale = 0;
            }
            ringScale = ringScale * 3 + 0x400;
            ringRot.vx = 0x400;
            ringRot.vz = 0;
            ringRot.vy = 0;
            ringRot.pad = 1;
            env->otZ = 0x20;
            ex->btlfx_drawRings((u8 *)ex->btlfx_getFileData(entry->file, FX_LV3_DEF_LESS_RING), &work->pos4, &ringRot, ringScale, 0, 1, ringAlpha);
            env->otZ = 0;
            ex->btlfx_drawLayerTexture((BtlBgLayers *)ex->btlfx_getFileData(entry->file, FX_LV3_DEF_LESS_BACKDROP), frame);
            if (frame >= 0x10) {
                work->state0 = 2;
                entry->frame = -1;
                emitter = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, FX_LV3_DEF_LESS_SMOKE_RINGS), (s32)&work->pos4);
                if (emitter != NULL) {
                    emitter->otZ = 0x80;
                }
                emitter = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, FX_LV3_DEF_LESS_SMOKE_RINGS_2), (s32)&work->pos4);
                if (emitter != NULL) {
                    emitter->otZ = 0x80;
                }
                emitter = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, FX_LV3_DEF_LESS_BLUE_RINGS), (s32)&work->pos4);
                if (emitter != NULL) {
                    emitter->otZ = 0x80;
                }
                emitter = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, FX_LV3_DEF_LESS_ORANGE_TRAILS), (s32)&work->pos4);
                if (emitter != NULL) {
                    emitter->trails = (s32)fx_188_op80Trails;
                    emitter->trailSlots = 0x19;
                    emitter->otZ = 0x20;
                    emitter->trailStride = 0x54;
                }
                emitter = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, FX_LV3_DEF_LESS_CRYSTAL_SHARDS), (s32)&work->pos4);
                if (emitter != NULL) {
                    emitter->otZ = 0x20;
                }
            }
            break;
        case 2:
            if (frame >= 0x20) {
                work->state0 = 2;
                return 1;
            }
            break;
        }
    }
    return 0;
}

INCLUDE_RODATA("asm/us/fx_188/nonmatchings/fx_188", fx_188_headerPtr);
