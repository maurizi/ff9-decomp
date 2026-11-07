#include "fxmod/fx_101.h"

s32 fx_101_op80(s32 mode, Fx101Work80 *work, Unk800F78A4 *env, Unk80100C78Entry *entry, s32 *args) {
    BtlfxExports *ex = BTLFX_EXPORTS(env);
    SVECTOR rot;
    VECTOR scale;
    VECTOR pos;
    s32 glowBright;
    s32 frame;
    s32 t;
    s32 ang;
    s32 growScale;
    s32 pulseScale;
    s32 fadeScale;
    s32 rgb;
    s32 fade;
    s32 tick;
    Pool6CEntry *emitter;

    if (mode == 0) {
        *args = sizeof(Fx101Work80);
    } else if (mode == 1) {
        work->state0 = FX_TRANCE_STATE_CHARGE;
        fx_101_env = env;
        ex->btlfx_getUnitPos(0x10, (s16 *)&work->posC);
        ex->btlfx_getUnitBodyPos(0x10, 0, (s16 *)&work->pos14);
        work->flag20 = 0;
        work->frame4 = 0;
        fx_101_stopBolts = 0;
        ex->btlfx_initEffModels(fx_101_effModels, 1);
        work->model24 = ex->Hi_RegisterTexEffModel((BtlModelHeader *)ex->btlfx_getFileData(entry->file, FX_TRANCE_AURA_CYLINDER), FX_TRANCE_AURA_CYLINDER_TPAGE, FX_TRANCE_AURA_CYLINDER_CLUT, FX_TRANCE_AURA_CYLINDER_U, FX_TRANCE_AURA_CYLINDER_V);
    } else {
    glowBright = 0;
    frame = entry->frame;
    switch (work->state0) {
    case FX_TRANCE_STATE_CHARGE:
        t = frame << 8;
        growScale = (ex->math_sin(t >> 2) * FX_TRANCE_RING_SCALE) >> 12;
        env->otZ = 0x7C;
        ex->btlfx_drawRings((u8 *)ex->btlfx_getFileData(entry->file, FX_TRANCE_WHITE_RINGS), &work->pos14, NULL, growScale, 0, BTLFX_ABR_ADD, t);
        ex->btlfx_drawRings((u8 *)ex->btlfx_getFileData(entry->file, FX_TRANCE_BLUE_RING), &work->pos14, NULL, 0x1000, 0, BTLFX_ABR_ADD, t);
        env->otZ = 0;
        rot.vx = 0x400;
        rot.vz = 0;
        rot.vy = 0;
        rot.pad = 1;
        ex->btlfx_drawRings((u8 *)ex->btlfx_getFileData(entry->file, FX_TRANCE_LILAC_RINGS), &work->posC, &rot, growScale, 0, BTLFX_ABR_ADD, t);
        scale.vx = scale.vz = FX_TRANCE_AURA_WIDTH;
        scale.vy = ex->math_sin(t >> 2) / 6;
        glowBright = t;
        if (frame >= FX_TRANCE_CHARGE_FRAMES) {
            work->state0 = FX_TRANCE_STATE_HOLD;
            entry->frame = -1;
            ((Fx101BattleState *)env->state)->signal10 = 0;
            ex->btlfx_fadeUnitById(0x10, 0);
            work->emit8 = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, FX_TRANCE_THIN_STREAKS), (s32)&work->pos14);
            if (work->emit8 != NULL) {
                work->emit8->otZ = ex->btlfx_addRadiusSixth(0x10, 0x20);
            }
            work->frame1C = 0;
        }
        break;
    case FX_TRANCE_STATE_HOLD:
        tick = ++work->frame1C;
        t = ((tick % FX_TRANCE_PULSE_FRAMES) << 12) / FX_TRANCE_PULSE_FRAMES;
        pulseScale = (ex->math_sin(t) >> 4) + FX_TRANCE_RING_SCALE;
        env->otZ = 0x7C;
        ex->btlfx_drawRings((u8 *)ex->btlfx_getFileData(entry->file, FX_TRANCE_WHITE_RINGS), &work->pos14, NULL, pulseScale, 0, BTLFX_ABR_ADD, 0x1000);
        ex->btlfx_drawRings((u8 *)ex->btlfx_getFileData(entry->file, FX_TRANCE_BLUE_RING), &work->pos14, NULL, 0x1000, 0, BTLFX_ABR_ADD, 0x1000);
        env->otZ = 0;
        rot.vx = 0x400;
        rot.vz = 0;
        rot.vy = 0;
        rot.pad = 1;
        ex->btlfx_drawRings((u8 *)ex->btlfx_getFileData(entry->file, FX_TRANCE_LILAC_RINGS), &work->posC, &rot, pulseScale, 0, BTLFX_ABR_ADD, 0x1000);
        scale.vx = scale.vz = FX_TRANCE_AURA_WIDTH;
        scale.vy = (ex->math_sin(t) >> 7) + 0x2AA;
        glowBright = 0x1000;
        if (work->emit8 != NULL && work->emit8->state == BTLFX_EMIT_DONE) {
            work->emit8 = NULL;
        }
        if (frame != 0 && t == 0 && ((Fx101BattleState *)env->state)->signal10 != 0) {
            work->state0 = FX_TRANCE_STATE_RELEASE;
            entry->frame = -1;
            env->camera->paused = 0;
            ex->btlfx_fadeUnitById(0x10, 0x80);
            ex->btlfx_stopEmitter(work->emit8);
            emitter = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, FX_TRANCE_PINK_STREAKS), (s32)&work->posC);
            if (emitter != NULL) {
                emitter->otZ = ex->btlfx_addRadiusSixth(0x10, 0x20);
            }
            emitter = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, FX_TRANCE_VIOLET_STREAKS), (s32)&work->posC);
            if (emitter != NULL) {
                emitter->otZ = ex->btlfx_addRadiusSixth(0x10, 0x20);
            }
            emitter = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, FX_TRANCE_SPARKLE_SPIRAL), (s32)&work->posC);
            if (emitter != NULL) {
                emitter->otZ = ex->btlfx_addRadiusSixth(0x10, 0x20);
            }
        }
        break;
    case FX_TRANCE_STATE_RELEASE:
        t = (frame << 12) / FX_TRANCE_RELEASE_FRAMES;
        fade = 0x1000 - t;
        env->otZ = 0x7C;
        fadeScale = t + FX_TRANCE_RING_SCALE;
        ex->btlfx_drawRings((u8 *)ex->btlfx_getFileData(entry->file, FX_TRANCE_WHITE_RINGS), &work->pos14, NULL, fadeScale, 0, BTLFX_ABR_ADD, fade);
        ex->btlfx_drawRings((u8 *)ex->btlfx_getFileData(entry->file, FX_TRANCE_BLUE_RING), &work->pos14, NULL, 0x1000, 0, BTLFX_ABR_ADD, fade);
        env->otZ = 0;
        rot.vx = 0x400;
        rot.vz = 0;
        rot.vy = 0;
        rot.pad = 1;
        ex->btlfx_drawRings((u8 *)ex->btlfx_getFileData(entry->file, FX_TRANCE_LILAC_RINGS), &work->posC, &rot, fadeScale, 0, BTLFX_ABR_ADD, fade);
        if (frame < FX_TRANCE_GLOW_FRAMES) {
            ang = (frame << 8) >> 2;
            glowBright = ex->math_cos(ang);
            scale.vx = scale.vz = (ex->math_sin(ang) >> 3) + FX_TRANCE_AURA_WIDTH;
            scale.vy = ex->math_cos(ang) / 6;
        }
        if (frame == FX_TRANCE_MOTION_FRAME && ex->btlfx_resetIdleMotion(1, 0x10) == 0) {
            work->flag20 = 1;
        }
        if (work->flag20 != 0 && frame >= FX_TRANCE_MOTION_END_FRAME) {
            ex->btlfx_resetIdleMotion(0, 0x10);
            work->flag20 = 0;
        }
        if (frame >= FX_TRANCE_RELEASE_FRAMES) {
            fx_101_stopBolts = 1;
            ex->btlfx_clearEffModels();
            return 1;
        }
        break;
    }
    if (glowBright != 0) {
        pos.vx = work->posC.vx;
        pos.vy = work->posC.vy;
        pos.vz = work->posC.vz;
        rot.vx = 0;
        rot.vy = work->frame4 * 32;
        rot.vz = 0;
        ex->Hi_DrawEffModel(&rot, &pos, (s32)&scale, work->model24);
        rgb = (glowBright >> 5) - 0x80;
        ex->Hi_ModifyEffModelRGB(work->model24, rgb, rgb, rgb);
        scale.vy >>= 1;
        scale.vx = scale.vz = scale.vx * 3 / 2;
        ex->Hi_DrawEffModel(&rot, &pos, (s32)&scale, work->model24);
        ex->Hi_ModifyEffModelRGB(work->model24, rgb, rgb, rgb);
    }
    ex->btlfx_drawScrollMove(work->frame4 * 4, FX_TRANCE_AURA_VRAM_X, FX_TRANCE_AURA_VRAM_Y, FX_TRANCE_AURA_VRAM_W, FX_TRANCE_AURA_VRAM_H, FX_TRANCE_AURA_SRC_X, FX_TRANCE_AURA_SRC_Y);
    work->frame4++;
    }
    return 0;
}

s32 fx_101_op82PoolTask(s32 mode, Fx101Spark *p, Unk800F78A4 *env, Unk80100C78Entry *entry, s32 frame) {
    BtlfxExports *ex = BTLFX_EXPORTS(env);
    s32 bias[2];
    SVECTOR *pt;
    s32 fade;

    if (mode == 1) {
        p->drag12 = ex->btlfx_randCentered(0x200) + 0xE00;
    } else {
        fade = 0x1000 - (frame << 9);
        pt = p->pos18;
        bias[0] = 0;
        bias[1] = 0;
        pt->pad = 0x14;
        ex->btlfx_stepModuleParticle((Pool6CEntry *)p, 0, (SVECTOR *)bias, pt);
        if (p->index16 == 0) {
            ((Fx101Bolt *)((s32 *)p->pos18 - 4))->lastSlot0 = ((Fx101Bolt *)((s32 *)p->pos18 - 4))->count2 = FX_TRANCE_BOLT_POINTS - 1;
            ((Fx101Bolt *)((s32 *)p->pos18 - 4))->head1 = 0;
            ex->btlfx_drawTrail((u8 *)ex->btlfx_getFileData(entry->file, FX_TRANCE_VIOLET_TRAIL), 0, 0, 5, 0x1000, fade, BTLFX_ABR_ADD,
                              (u8 *)p->pos18 - 4, 0);
        }
        if (frame >= FX_TRANCE_SPARK_FRAMES) {
            return 1;
        }
    }
    return 0;
}

void fx_101_resetBolts(void) {
    fx_101_nextBolt = 0;
}

void fx_101_spawnBolt(Unk80100C78Entry *entry, SVECTOR *origin, s32 ang, SVECTOR *from, SVECTOR *to) {
    Fx101Bolt *bolt = &fx_101_bolts[fx_101_nextBolt];
    SVECTOR *pt = &bolt->pos4[FX_TRANCE_BOLT_POINTS - 1];
    BtlfxExports *ex = BTLFX_EXPORTS(fx_101_env);
    SVECTOR dir;
    Fx101Spark *spark;
    s32 i;
    s32 t;

    bolt->count2 = FX_TRANCE_BOLT_POINTS - 1;
    bolt->lastSlot0 = FX_TRANCE_BOLT_POINTS - 1;
    bolt->head1 = 0;
    for (i = 0; i < FX_TRANCE_BOLT_POINTS; i++, pt--) {
        t = 0x1000 - (i << 12) / (FX_TRANCE_BOLT_POINTS - 1);
        ex->btlfx_lerpVec((s16 *)from, (s16 *)to, t, (s16 *)&dir);
        dir.vx += ex->btlfx_randCentered(0x18);
        dir.vy += ex->btlfx_randCentered(0x18);
        t = ang + ex->btlfx_randCentered(0xF) + (ex->math_sin(t >> 1) >> 4);
        ex->btlfx_pointAhead((s16 *)origin, (s16 *)&dir, t, (s16 *)pt);
        spark = ex->btlfx_poolSpawn((s16 *)entry->pool);
        if (spark != NULL) {
            spark->index14 = fx_101_nextBolt;
            spark->index16 = FX_TRANCE_BOLT_POINTS - 1 - i;
            spark->pos18 = pt;
            spark->posX0 = pt->vx << 12;
            spark->posY4 = pt->vy << 12;
            spark->posZ8 = pt->vz << 12;
            spark->velXC = 0;
            spark->velYE = 0;
            spark->velZ10 = 0;
            spark->velXC = ((pt->vx - origin->vx) >> 3) + ex->btlfx_randCentered(5);
            spark->velYE = ((pt->vy - origin->vy) >> 3) + ex->btlfx_randCentered(0xF);
            spark->velZ10 = ((pt->vz - origin->vz) >> 3) + ex->btlfx_randCentered(5);
        }
    }
    fx_101_nextBolt++;
    fx_101_nextBolt %= FX_TRANCE_BOLT_COUNT;
}

s32 fx_101_op82(s32 mode, Fx101Work82 *work, Unk800F78A4 *env, Unk80100C78Entry *entry, Fx101PoolCfg *cfg) {
    BtlfxExports *ex = BTLFX_EXPORTS(env);
    SVECTOR prev;

    if (mode == 0) {
        cfg->workSize0 = sizeof(Fx101Work82);
        cfg->elemSize8 = sizeof(Fx101Spark);
        cfg->elemFn14 = fx_101_op82PoolTask;
        cfg->poolCount4 = FX_TRANCE_BOLT_COUNT * FX_TRANCE_BOLT_POINTS;
    } else if (mode == 1) {
        fx_101_env = env;
        fx_101_resetBolts();
        ex->btlfx_getUnitBodyPos(0x10, 0, (s16 *)&work->pos0);
        work->angle8.vx = 0;
        work->angle8.vy = 0;
        work->angle8.vz = 0;
    } else {
        if (fx_101_stopBolts == 0) {
            BTLFX_COPY_SVEC(&prev, &work->angle8);
            work->angle8.vx += 0x300 + (ex->btlfx_rand() & 0x7F);
            work->angle8.vy += 0x500 + (ex->btlfx_rand() & 0x7F);
            fx_101_spawnBolt(entry, &work->pos0, FX_TRANCE_BOLT_REACH, &prev, &work->angle8);
        }
        work->angle8.vx &= 0xFFF;
        work->angle8.vy &= 0xFFF;
        if (fx_101_stopBolts != 0) {
            return 1;
        }
    }
    return 0;
}

INCLUDE_RODATA("asm/us/fx_101/nonmatchings/fx_101", fx_101_headerPtr);
