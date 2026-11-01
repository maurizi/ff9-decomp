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
        *args = 0x28;
    } else if (mode == 1) {
        work->state0 = 0;
        fx_101_env = env;
        ex->btlfx_getUnitPos(0x10, (s16 *)&work->posC);
        ex->btlfx_getUnitBodyPos(0x10, 0, (s16 *)&work->pos14);
        work->flag20 = 0;
        work->frame4 = 0;
        fx_101_stopBolts = 0;
        ex->btlfx_initEffModels(fx_101_effModels, 1);
        work->model24 = ex->Hi_RegisterTexEffModel((BtlModelHeader *)ex->btlfx_getFileData(entry->file, 3), 0xB7, 0x3D80, 0, 0);
    } else {
    glowBright = 0;
    frame = entry->frame;
    switch (work->state0) {
    case 0:
        t = frame << 8;
        growScale = (ex->math_sin(t >> 2) * 0x10FF) >> 12;
        env->otZ = 0x7C;
        ex->btlfx_drawRings((u8 *)ex->btlfx_getFileData(entry->file, 0xA), &work->pos14, NULL, growScale, 0, 1, t);
        ex->btlfx_drawRings((u8 *)ex->btlfx_getFileData(entry->file, 1), &work->pos14, NULL, 0x1000, 0, 1, t);
        env->otZ = 0;
        rot.vx = 0x400;
        rot.vz = 0;
        rot.vy = 0;
        rot.pad = 1;
        ex->btlfx_drawRings((u8 *)ex->btlfx_getFileData(entry->file, 9), &work->posC, &rot, growScale, 0, 1, t);
        scale.vx = scale.vz = 0x174;
        scale.vy = ex->math_sin(t >> 2) / 6;
        glowBright = t;
        if (frame >= 0x10) {
            work->state0 = 1;
            entry->frame = -1;
            ((Fx101BattleState *)env->state)->signal10 = 0;
            ex->btlfx_fadeUnitById(0x10, 0);
            work->emit8 = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, 5), (s32)&work->pos14);
            if (work->emit8 != NULL) {
                work->emit8->otZ = ex->btlfx_addRadiusSixth(0x10, 0x20);
            }
            work->frame1C = 0;
        }
        break;
    case 1:
        tick = ++work->frame1C;
        t = ((tick % 10) << 12) / 10;
        pulseScale = (ex->math_sin(t) >> 4) + 0x10FF;
        env->otZ = 0x7C;
        ex->btlfx_drawRings((u8 *)ex->btlfx_getFileData(entry->file, 0xA), &work->pos14, NULL, pulseScale, 0, 1, 0x1000);
        ex->btlfx_drawRings((u8 *)ex->btlfx_getFileData(entry->file, 1), &work->pos14, NULL, 0x1000, 0, 1, 0x1000);
        env->otZ = 0;
        rot.vx = 0x400;
        rot.vz = 0;
        rot.vy = 0;
        rot.pad = 1;
        ex->btlfx_drawRings((u8 *)ex->btlfx_getFileData(entry->file, 9), &work->posC, &rot, pulseScale, 0, 1, 0x1000);
        scale.vx = scale.vz = 0x174;
        scale.vy = (ex->math_sin(t) >> 7) + 0x2AA;
        glowBright = 0x1000;
        if (work->emit8 != NULL && work->emit8->state == BTLFX_EMIT_DONE) {
            work->emit8 = NULL;
        }
        if (frame != 0 && t == 0 && ((Fx101BattleState *)env->state)->signal10 != 0) {
            work->state0 = 2;
            entry->frame = -1;
            env->camera->paused = 0;
            ex->btlfx_fadeUnitById(0x10, 0x80);
            ex->btlfx_stopEmitter(work->emit8);
            emitter = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, 0xF), (s32)&work->posC);
            if (emitter != NULL) {
                emitter->otZ = ex->btlfx_addRadiusSixth(0x10, 0x20);
            }
            emitter = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, 0x10), (s32)&work->posC);
            if (emitter != NULL) {
                emitter->otZ = ex->btlfx_addRadiusSixth(0x10, 0x20);
            }
            emitter = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, 0xC), (s32)&work->posC);
            if (emitter != NULL) {
                emitter->otZ = ex->btlfx_addRadiusSixth(0x10, 0x20);
            }
        }
        break;
    case 2:
        t = (frame << 12) / 47;
        fade = 0x1000 - t;
        env->otZ = 0x7C;
        fadeScale = t + 0x10FF;
        ex->btlfx_drawRings((u8 *)ex->btlfx_getFileData(entry->file, 0xA), &work->pos14, NULL, fadeScale, 0, 1, fade);
        ex->btlfx_drawRings((u8 *)ex->btlfx_getFileData(entry->file, 1), &work->pos14, NULL, 0x1000, 0, 1, fade);
        env->otZ = 0;
        rot.vx = 0x400;
        rot.vz = 0;
        rot.vy = 0;
        rot.pad = 1;
        ex->btlfx_drawRings((u8 *)ex->btlfx_getFileData(entry->file, 9), &work->posC, &rot, fadeScale, 0, 1, fade);
        if (frame < 0x10) {
            ang = (frame << 8) >> 2;
            glowBright = ex->math_cos(ang);
            scale.vx = scale.vz = (ex->math_sin(ang) >> 3) + 0x174;
            scale.vy = ex->math_cos(ang) / 6;
        }
        if (frame == 3 && ex->btlfx_resetIdleMotion(1, 0x10) == 0) {
            work->flag20 = 1;
        }
        if (work->flag20 != 0 && frame >= 0x12) {
            ex->btlfx_resetIdleMotion(0, 0x10);
            work->flag20 = 0;
        }
        if (frame >= 0x2F) {
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
    ex->btlfx_drawScrollMove(work->frame4 * 4, 0x1C0, 0x120, 0x20, 0x40, 0x1E0, 0x120);
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
            ((Fx101Bolt *)((s32 *)p->pos18 - 4))->lastSlot0 = ((Fx101Bolt *)((s32 *)p->pos18 - 4))->count2 = 0xB;
            ((Fx101Bolt *)((s32 *)p->pos18 - 4))->head1 = 0;
            ex->btlfx_drawTrail((u8 *)ex->btlfx_getFileData(entry->file, 8), 0, 0, 5, 0x1000, fade, 1,
                              (u8 *)p->pos18 - 4, 0);
        }
        if (frame >= 8) {
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
    SVECTOR *pt = &bolt->pos4[11];
    BtlfxExports *ex = BTLFX_EXPORTS(fx_101_env);
    SVECTOR dir;
    Fx101Spark *spark;
    s32 i;
    s32 t;

    bolt->count2 = 0xB;
    bolt->lastSlot0 = 0xB;
    bolt->head1 = 0;
    for (i = 0; i < 12; i++, pt--) {
        t = 0x1000 - (i << 12) / 11;
        ex->btlfx_lerpVec((s16 *)from, (s16 *)to, t, (s16 *)&dir);
        dir.vx += ex->btlfx_randCentered(0x18);
        dir.vy += ex->btlfx_randCentered(0x18);
        t = ang + ex->btlfx_randCentered(0xF) + (ex->math_sin(t >> 1) >> 4);
        ex->btlfx_pointAhead((s16 *)origin, (s16 *)&dir, t, (s16 *)pt);
        spark = ex->btlfx_poolSpawn((s16 *)entry->pool);
        if (spark != NULL) {
            spark->index14 = fx_101_nextBolt;
            spark->index16 = 0xB - i;
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
    fx_101_nextBolt %= 8;
}

s32 fx_101_op82(s32 mode, Fx101Work82 *work, Unk800F78A4 *env, Unk80100C78Entry *entry, Fx101PoolCfg *cfg) {
    BtlfxExports *ex = BTLFX_EXPORTS(env);
    SVECTOR prev;

    if (mode == 0) {
        cfg->workSize0 = 0x10;
        cfg->elemSize8 = 0x1C;
        cfg->elemFn14 = fx_101_op82PoolTask;
        cfg->poolCount4 = 0x60;
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
            fx_101_spawnBolt(entry, &work->pos0, 0x9B, &prev, &work->angle8);
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
