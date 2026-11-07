#include "fxmod/fx_18a.h"

s32 fx_18a_op80(s32 mode, Fx18aWork80 *work, Unk800F78A4 *env, Unk80100C78Entry *entry, s32 *args) {
    BtlfxExports *ex = BTLFX_EXPORTS(env);
    SVECTOR rot;
    VECTOR sc;
    VECTOR pos;
    CVECTOR col;
    SVECTOR ang;
    VECTOR scale;
    SVECTOR rot2;
    s32 t;
    s32 glow;
    s32 state;
    s32 ringSize;
    s32 swellAng;
    s32 frame;
    s32 pulseSize;
    s32 fade;
    s32 burstT;
    s32 flashAng;
    s32 tintBase;
    s32 shrinkAng;
    s32 unused;
    s32 ringGrow;
    Pool6CEntry *emitter;

    if (mode == 0) {
        *args = sizeof(Fx18aWork80);
    } else if (mode == 1) {
        work->state0 = 0;
        fx_18a_env = env;
        ex->btlfx_getUnitPos(BTLFX_UNIT_CASTER, (s16 *)&work->posC);
        ex->btlfx_getUnitBodyPos(BTLFX_UNIT_CASTER, 0, (s16 *)&work->pos14);
        work->unk20 = 0;
        work->frame4 = 0;
        fx_18a_op80Done = 0;
        ex->btlfx_initEffModels(fx_18a_effModels, 1);
        work->model24 = ex->Hi_RegisterTexEffModel((BtlModelHeader *)ex->btlfx_getFileData(entry->file, FX_TRANSFORM_AURA_CYLINDER), FX_TRANSFORM_AURA_TPAGE, FX_TRANSFORM_AURA_CLUT, FX_TRANSFORM_AURA_U, FX_TRANSFORM_AURA_V);
    } else {
        glow = 0;
        state = work->state0;
        frame = entry->frame;
        switch (state) {
        case FX_TRANSFORM_STATE_SWELL:
            t = frame << 8;
            swellAng = t >> 2;
            ringSize = (ex->math_sin(swellAng) * 0x1400) >> 12;
            env->otZ = 0x100;
            ex->btlfx_drawRings((u8 *)ex->btlfx_getFileData(entry->file, FX_TRANSFORM_VIOLET_RINGS_2), &work->pos14, NULL, ringSize, 0, BTLFX_ABR_ADD, t);
            ex->btlfx_drawRings((u8 *)ex->btlfx_getFileData(entry->file, FX_TRANSFORM_BLUE_RING), &work->pos14, NULL, 0x1000, 0, BTLFX_ABR_ADD, t);
            env->otZ = 0;
            rot.vx = 0x400;
            rot.vz = 0;
            rot.vy = 0;
            rot.pad = 1;
            ex->btlfx_drawRings((u8 *)ex->btlfx_getFileData(entry->file, FX_TRANSFORM_VIOLET_RINGS), &work->posC, &rot, ringSize, 0, BTLFX_ABR_ADD, t);
            sc.vx = sc.vz = 0x200;
            sc.vy = ex->math_sin(swellAng) / 5;
            glow = t;
            if (frame >= FX_TRANSFORM_SWELL_FRAMES) {
                work->state0 = FX_TRANSFORM_STATE_PULSE;
                entry->frame = -1;
                work->emit8 = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, FX_TRANSFORM_RED_SWEEP), (s32)&work->pos14);
                if (work->emit8 != NULL) {
                    work->emit8->otZ = ex->btlfx_addRadiusSixth(BTLFX_UNIT_CASTER, 0x20);
                }
                work->frame1C = 0;
            }
            break;
        case FX_TRANSFORM_STATE_PULSE:
            t = ((++work->frame1C % FX_TRANSFORM_PULSE_FRAMES) << 12) / FX_TRANSFORM_PULSE_FRAMES;
            pulseSize = (ex->math_sin(t) >> 4) + 0x1400;
            env->otZ = 0x100;
            ex->btlfx_drawRings((u8 *)ex->btlfx_getFileData(entry->file, FX_TRANSFORM_VIOLET_RINGS_2), &work->pos14, NULL, pulseSize, 0, BTLFX_ABR_ADD, 0x1000);
            ex->btlfx_drawRings((u8 *)ex->btlfx_getFileData(entry->file, FX_TRANSFORM_BLUE_RING), &work->pos14, NULL, 0x1000, 0, BTLFX_ABR_ADD, 0x1000);
            env->otZ = 0;
            rot.vx = 0x400;
            rot.vz = 0;
            rot.vy = 0;
            rot.pad = 1;
            ex->btlfx_drawRings((u8 *)ex->btlfx_getFileData(entry->file, FX_TRANSFORM_VIOLET_RINGS), &work->posC, &rot, pulseSize, 0, BTLFX_ABR_ADD, 0x1000);
            sc.vx = sc.vz = 0x200;
            sc.vy = (ex->math_sin(t) >> 7) + 0x333;
            glow = 0x1000;
            if (work->emit8 != NULL && work->emit8->state == BTLFX_EMIT_DONE) {
                work->emit8 = NULL;
            }
            if (frame >= FX_TRANSFORM_PULSE_MIN_FRAMES && t == 0) {
                work->state0 = FX_TRANSFORM_STATE_TRANSFORM;
                entry->frame = -1;
                env->camera->paused = 0;
                ex->btlfx_fadeUnitById(BTLFX_UNIT_CASTER, -1);
                ex->btlfx_fadeUnitById(0, -2);
                work->posC.vz += 0x64;
                ex->btlfx_setUnitPos(0, (s16 *)&work->posC);
                ex->btlfx_stopEmitter(work->emit8);
                emitter = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, FX_TRANSFORM_PINK_STREAKS), (s32)&work->posC);
                if (emitter != NULL) {
                    emitter->otZ = ex->btlfx_addRadiusSixth(BTLFX_UNIT_CASTER, 0x20);
                }
                emitter = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, FX_TRANSFORM_RED_STREAKS), (s32)&work->posC);
                if (emitter != NULL) {
                    emitter->otZ = ex->btlfx_addRadiusSixth(BTLFX_UNIT_CASTER, 0x20);
                }
                emitter = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, FX_TRANSFORM_RED_SPECKS), (s32)&work->posC);
                if (emitter != NULL) {
                    emitter->otZ = ex->btlfx_addRadiusSixth(BTLFX_UNIT_CASTER, 0x20);
                }
            }
            break;
        case FX_TRANSFORM_STATE_TRANSFORM:
            ang.vx = 0;
            ang.vy = 0;
            ang.vz = 0;
            t = (frame << 12) / FX_TRANSFORM_FRAMES;
            col.r = col.g = col.b = (((0x1000 - t) << 6) >> 12) - 0x74;
            if (frame < FX_TRANSFORM_BURST_FRAME) {
                scale.vx = scale.vy = scale.vz = (ex->math_cos(t >> 2) >> 3) + 0x1000;
            } else {
                scale.vx = scale.vy = scale.vz = (ex->math_cos(((frame - FX_TRANSFORM_BURST_FRAME) << 10) / (FX_TRANSFORM_FRAMES - FX_TRANSFORM_BURST_FRAME)) >> 2) + 0x1000;
            }
            ex->btlfx_drawScreenGrid(1, 0xA0, 0x70, 0x50, 0x38, 4, 4, 2, 2, &ang, &scale, &col, 0, 0);
            if (frame >= FX_TRANSFORM_BURST_FRAME) {
                rot2.vx = 0x400;
                rot2.pad = 1;
                rot2.vz = 0;
                rot2.vy = 0;
                env->otZ = 0x20;
                burstT = ((frame - FX_TRANSFORM_BURST_FRAME) << 12) / (FX_TRANSFORM_FRAMES - FX_TRANSFORM_BURST_FRAME);
                ex->btlfx_drawRings((u8 *)ex->btlfx_getFileData(entry->file, FX_TRANSFORM_VIOLET_RING), &work->pos14, &rot2,
                                  ex->math_sin(burstT >> 2) * 5, 0, BTLFX_ABR_ADD, ex->math_cos(burstT >> 2));
                ex->btlfx_drawRings((u8 *)ex->btlfx_getFileData(entry->file, FX_TRANSFORM_VIOLET_RING), &work->pos14, &rot2,
                                  ex->math_sin(burstT >> 2) * 2, 0, BTLFX_ABR_ADD, ex->math_cos(burstT >> 2));
                rot2.vx += 0x200;
                ex->btlfx_drawRings((u8 *)ex->btlfx_getFileData(entry->file, FX_TRANSFORM_VIOLET_RING), &work->pos14, &rot2,
                                  ex->math_sin(burstT >> 2) * 3, 0, BTLFX_ABR_ADD, ex->math_cos(burstT >> 2));
                rot2.vx -= 0x400;
                ex->btlfx_drawRings((u8 *)ex->btlfx_getFileData(entry->file, FX_TRANSFORM_VIOLET_RING), &work->pos14, &rot2,
                                  ex->math_sin(burstT >> 2) * 3, 0, BTLFX_ABR_ADD, ex->math_cos(burstT >> 2));
                env->otZ = 0;
            }
            fade = 0x1000 - t;
            env->otZ = 0x100;
            ringGrow = t + 0x1400;
            ex->btlfx_drawRings((u8 *)ex->btlfx_getFileData(entry->file, FX_TRANSFORM_VIOLET_RINGS_2), &work->pos14, NULL, ringGrow, 0, BTLFX_ABR_ADD, fade);
            ex->btlfx_drawRings((u8 *)ex->btlfx_getFileData(entry->file, FX_TRANSFORM_BLUE_RING), &work->pos14, NULL, 0x1000, 0, BTLFX_ABR_ADD, fade);
            if (frame < FX_TRANSFORM_FLASH_FRAMES) {
                flashAng = (frame << 10) / FX_TRANSFORM_FLASH_FRAMES;
                ex->btlfx_drawRings((u8 *)ex->btlfx_getFileData(entry->file, FX_TRANSFORM_BLUE_RING), &work->pos14, NULL,
                                  ex->math_sin(flashAng) * 2, 0, BTLFX_ABR_ADD, ex->math_cos(flashAng));
            }
            env->otZ = 0;
            if (frame == FX_TRANSFORM_CUE_FRAME) {
                ex->btlfx_signalScriptCue();
            }
            if (frame < FX_TRANSFORM_TINT_FRAMES) {
                tintBase = ex->math_cos(frame << 8) * 0xFF;
                fade = tintBase >> 12;
                ex->btlfx_drawScreenTint((u32 *)(env->ot + 1), BTLFX_ABR_ADD, fade, tintBase >> 14, tintBase >> 13);
            }
            rot.vx = 0x400;
            rot.vz = 0;
            rot.vy = 0;
            rot.pad = 1;
            ex->btlfx_drawRings((u8 *)ex->btlfx_getFileData(entry->file, FX_TRANSFORM_VIOLET_RINGS), &work->posC, &rot, ringGrow, 0, BTLFX_ABR_ADD, fade);
            if (frame < FX_TRANSFORM_SHRINK_FRAMES) {
                shrinkAng = (frame << 12) / FX_TRANSFORM_SHRINK_FRAMES >> 2;
                glow = ex->math_cos(shrinkAng);
                sc.vx = sc.vz = (ex->math_sin(shrinkAng) >> 2) + 0x174;
                sc.vy = ex->math_cos(shrinkAng) / 4;
            }
            if (frame >= FX_TRANSFORM_FRAMES) {
                fx_18a_op80Done = 1;
                ex->btlfx_clearEffModels();
                return 1;
            }
            break;
        }
        if (glow != 0) {
            pos.vx = work->posC.vx;
            pos.vy = work->posC.vy;
            pos.vz = work->posC.vz;
            rot.vx = 0;
            rot.vy = work->frame4 * 32;
            rot.vz = 0;
            ex->Hi_DrawEffModel(&rot, &pos, (s32)&sc, work->model24);
            ex->Hi_ModifyEffModelRGB(work->model24, (glow >> 5) - 0x80, (glow >> 5) - 0x80, (glow >> 5) - 0x80);
            sc.vy >>= 1;
            sc.vx = sc.vz = sc.vx * 3 / 2;
            ex->Hi_DrawEffModel(&rot, &pos, (s32)&sc, work->model24);
            ex->Hi_ModifyEffModelRGB(work->model24, (glow >> 5) - 0x80, (glow >> 5) - 0x80, (glow >> 5) - 0x80);
        }
        ex->btlfx_drawScrollMove(work->frame4 * 4, FX_TRANSFORM_AURA_VRAM_X, FX_TRANSFORM_AURA_VRAM_Y, FX_TRANSFORM_AURA_VRAM_W, FX_TRANSFORM_AURA_VRAM_H, FX_TRANSFORM_AURA_SRC_X, FX_TRANSFORM_AURA_SRC_Y);
        work->frame4++;
    }
    return 0;
}

static s32 fx_18a_op82PoolTask(s32 mode, Fx18aSpark *spark, Unk800F78A4 *env, Unk80100C78Entry *entry, s32 frame) {
    BtlfxExports *ex = BTLFX_EXPORTS(env);
    s32 zero[2];
    SVECTOR *pt;
    s32 fade;

    if (mode == 1) {
        spark->drag12 = ex->btlfx_randCentered(0x200) + 0xE00;
    } else {
        fade = 0x1000 - (frame << 12) / FX_TRANSFORM_SPARK_FRAMES;
        pt = spark->point18;
        zero[0] = 0;
        zero[1] = 0;
        pt->pad = 0x1A;
        ex->btlfx_stepModuleParticle((Pool6CEntry *)spark, 0, (SVECTOR *)zero, pt);
        if (spark->index16 == 0) {
            TRAIL(spark)->lastSlot0 = TRAIL(spark)->count2 = FX_TRANSFORM_TRAIL_POINTS - 1;
            TRAIL(spark)->head1 = 0;
            ex->btlfx_drawTrail((u8 *)ex->btlfx_getFileData(entry->file, FX_TRANSFORM_VIOLET_TRAIL), 0, NULL, 5, 0x1000, fade, BTLFX_ABR_ADD, (u8 *)spark->point18 - 4, 0);
        }
        if (frame >= FX_TRANSFORM_SPARK_FRAMES) {
            return 1;
        }
    }
    return 0;
}

void fx_18a_resetTrails(void) {
    fx_18a_nextTrail = 0;
}

void fx_18a_spawnTrail(Fx18aInst *inst, SVECTOR *origin, s32 reach, SVECTOR *from, SVECTOR *to) {
    Fx18aTrail *trail = &fx_18a_trails[fx_18a_nextTrail];
    SVECTOR *pt = &trail->points4[FX_TRANSFORM_TRAIL_POINTS - 1];
    BtlfxExports *ex = BTLFX_EXPORTS(fx_18a_env);
    SVECTOR dir;
    Fx18aSpark *spark;
    s32 i;
    s32 t;

    trail->lastSlot0 = trail->count2 = FX_TRANSFORM_TRAIL_POINTS - 1;
    trail->head1 = 0;
    for (i = 0; i < FX_TRANSFORM_TRAIL_POINTS; i++, pt--) {
        t = 0x1000 - (i << 12) / (FX_TRANSFORM_TRAIL_POINTS - 1);
        ex->btlfx_lerpVec((s16 *)from, (s16 *)to, t, (s16 *)&dir);
        dir.vx += ex->btlfx_randCentered(0x20);
        dir.vy += ex->btlfx_randCentered(0x20);
        ex->btlfx_pointAhead((s16 *)origin, (s16 *)&dir, reach + ex->btlfx_randCentered(0x10) + (ex->math_sin(t >> 1) >> 3), (s16 *)pt);
        spark = ex->btlfx_poolSpawn(inst->pool10);
        if (spark != NULL) {
            spark->slot14 = fx_18a_nextTrail;
            spark->index16 = FX_TRANSFORM_TRAIL_POINTS - 1 - i;
            spark->point18 = pt;
            spark->pos0 = pt->vx << 12;
            spark->pos4 = pt->vy << 12;
            spark->pos8 = pt->vz << 12;
            spark->velC = 0;
            spark->velE = 0;
            spark->vel10 = 0;
            spark->velC = ((pt->vx - origin->vx) >> 4) + ex->btlfx_randCentered(0x20);
            spark->velE = ((pt->vy - origin->vy) >> 3) + ex->btlfx_randCentered(0x20);
            spark->vel10 = ((pt->vz - origin->vz) >> 4) + ex->btlfx_randCentered(0x20);
        }
    }
    fx_18a_nextTrail = (fx_18a_nextTrail + 1) % FX_TRANSFORM_TRAILS;
}

s32 fx_18a_op82(s32 mode, Fx18aWork82 *work, Unk800F78A4 *env, Fx18aInst *inst, EffInit *init) {
    BtlfxExports *ex = BTLFX_EXPORTS(env);
    SVECTOR from;
    s32 frame;
    s32 active;

    if (mode == 0) {
        init->workSize = sizeof(Fx18aWork82);
        init->elemSize = sizeof(Fx18aSpark);
        init->elemFn = (s32)fx_18a_op82PoolTask;
        init->poolCount = FX_TRANSFORM_SPARKS;
    } else if (mode == 1) {
        fx_18a_env = env;
        fx_18a_resetTrails();
        ex->btlfx_getUnitBodyPos(BTLFX_UNIT_CASTER, 0, (s16 *)&work->pos0);
        work->rot8.vx = 0;
        work->rot8.vy = 0;
        work->rot8.vz = 0;
    } else {
        frame = inst->frame0;
        active = frame < FX_TRANSFORM_TRAILS_FRAMES;
        if (active) {
            BTLFX_COPY_SVEC(&from, &work->rot8);
            work->rot8.vx += 0x300 + (ex->btlfx_rand() & 0x7F);
            work->rot8.vy += 0x400 + (ex->btlfx_rand() & 0x7F);
            fx_18a_spawnTrail(inst, &work->pos0, frame + 0x40, &from, &work->rot8);
        }
        work->rot8.vx &= 0xFFF;
        work->rot8.vy &= 0xFFF;
        if (!active) {
            return 1;
        }
    }
    return 0;
}

INCLUDE_RODATA("asm/us/fx_18a/nonmatchings/fx_18a", fx_18a_headerPtr);
