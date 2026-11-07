#include "fxmod/fx_1e7.h"

static s32 fx_1e7_op80ParticleMotion0(Pool30Entry *pt, s32 owner, SVECTOR *pos, s32 trail, s32 frame, s32 n) {
    return fxm_followPathParticle(pt, pos, &fx_1e7_posPath0[0], frame, n);
}

static s32 fx_1e7_op80ParticleMotion1(Pool30Entry *pt, s32 owner, SVECTOR *pos, s32 trail, s32 frame, s32 n) {
    return fxm_followPathParticle(pt, pos, &fx_1e7_posPath0[1], frame, n);
}

static s32 fxm_followPathParticle(Pool30Entry *pt, SVECTOR *pos, Fx1e7Path *path, s32 frame, s32 frames) {
    BtlfxExports *ex = BTLFX_EXPORTS(fx_1e7_env);
    MATRIX m;
    VECTOR world;
    SVECTOR pathPos;

    fxm_posPathGet(path, frame * 4, &pathPos);
    if (path == fx_1e7_posPath0) {
        pathPos.vx += 0x50;
    }
    ex->math_rotMatrixYXZ(&fx_1e7_op80PathRot, &m);
    gte_SetRotMatrix(&m);
    gte_ldtr0();
    gte_ldv0(&pathPos);
    gte_rtv0tr();
    gte_stlvnl(&world);
    world.vx += fx_1e7_op80Origin.vx;
    world.vy += fx_1e7_op80Origin.vy;
    world.vz += fx_1e7_op80Origin.vz;
    pos->vx = world.vx;
    pos->vy = world.vy;
    pos->vz = world.vz;
    pt->pos.vx = pos->vx << 12;
    pt->pos.vy = pos->vy << 12;
    pt->pos.vz = pos->vz << 12;
    if (frame >= frames) {
        return -1;
    }
    return 0;
}

s32 fx_1e7_op81SpawnPoint0(SVECTOR *out, s32 dir, s32 i) {
    fx_1e7_getSpawnPos(out, &fx_1e7_op81Paths[0], i);
    return 0;
}

s32 fx_1e7_op81SpawnPoint1(SVECTOR *out, s32 dir, s32 i) {
    fx_1e7_getSpawnPos(out, &fx_1e7_op81Paths[1], i);
    return 0;
}

s32 fx_1e7_op81SpawnPoint2(SVECTOR *out, s32 dir, s32 i) {
    fx_1e7_getSpawnPos(out, &fx_1e7_op81Paths[2], i);
    return 0;
}

s32 fx_1e7_op81SpawnPoint3(SVECTOR *out, s32 dir, s32 i) {
    fx_1e7_getSpawnPos(out, &fx_1e7_op81Paths[3], i);
    return 0;
}

s32 fx_1e7_op81SpawnPoint4(SVECTOR *out, s32 dir, s32 i) {
    fx_1e7_getSpawnPos(out, &fx_1e7_op81Paths[4], i);
    return 0;
}

void fx_1e7_getSpawnPos(SVECTOR *out, Fx1e7Path *path, s32 t) {
    SVECTOR pathPos;

    fxm_posPathGet(path, t, &pathPos);
    pathPos.vx += fx_1e7_op81Origin.vx;
    pathPos.vy += fx_1e7_op81Origin.vy;
    pathPos.vz += fx_1e7_op81Origin.vz;
    out->vx = pathPos.vx;
    out->vy = pathPos.vy;
    out->vz = pathPos.vz;
}

s32 fx_1e7_op80(s32 mode, Fx1e7Work80 *work, Unk800F78A4 *env, Unk80100C78Entry *entry, s32 *args) {
    BtlfxExports *ex = BTLFX_EXPORTS(env);
    s32 frame;

    if (mode == 0) {
        *args = sizeof(Fx1e7Work80);
    } else if (mode == 1) {
        fx_1e7_env = env;
        ex->btlfx_getUnitPos(0, (s16 *)&work->pos8);
        work->pos8.vy -= 0x12C;
        fx_1e7_op80Origin.vx = work->pos8.vx;
        fx_1e7_op80Origin.vy = work->pos8.vy;
        fx_1e7_op80Origin.vz = work->pos8.vz;
        fxm_posPathInit(&fx_1e7_posPath0[0], (u16 *)ex->btlfx_getFileData(entry->file, FX_WRONG_PATH_KEYS));
        fxm_posPathInit(&fx_1e7_posPath, (u16 *)ex->btlfx_getFileData(entry->file, FX_WRONG_PATH_KEYS_2));
        work->unk10 = 0x1000;
    } else {
        frame = entry->frame;
        if (frame != 0) {
            if (frame == FX_WRONG_TRAIL_FRAME) {
                work->emit0 = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, FX_WRONG_WHITE_TRAIL), (s32)&fx_1e7_emitPos0);
                if (work->emit0 != NULL) {
                    work->emit0->trails = (s32)fx_1e7_op80Trails0;
                    work->emit0->trailSlots = FX_WRONG_TRAIL_SLOTS;
                    work->emit0->trailStride = FX_WRONG_TRAIL_STRIDE;
                }
                if (work->emit0 != NULL) {
                    work->emit0->file->m.module.update = fx_1e7_op80ParticleMotion0;
                }
                if (work->emit0 != NULL) {
                    work->emit0->otZ = ex->btlfx_addRadiusSixth(0, 0x80);
                }
            }
        } else {
            work->emit4 = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, FX_WRONG_WHITE_TRAIL_2), (s32)&fx_1e7_emitPos0);
            if (work->emit4 != NULL) {
                work->emit4->trails = (s32)fx_1e7_op80Trails1;
                work->emit4->trailSlots = FX_WRONG_TRAIL_SLOTS;
                work->emit4->trailStride = FX_WRONG_TRAIL_STRIDE;
            }
            if (work->emit4 != NULL) {
                work->emit4->file->m.module.update = fx_1e7_op80ParticleMotion1;
            }
            if (work->emit4 != NULL) {
                work->emit4->otZ = ex->btlfx_addRadiusSixth(0, 0x80);
            }
        }
        if (frame == FX_WRONG_TRAILS_STOP_FRAME) {
            ex->btlfx_stopEmitter(work->emit0);
            ex->btlfx_stopEmitter(work->emit4);
        }
        ex->btlfx_aimAngles((s16 *)&env->camera->eye, (s16 *)&work->pos8, (s16 *)&fx_1e7_op80PathRot);
        return frame >= FX_WRONG_TRAILS_FRAMES;
    }
    return 0;
}

s32 fx_1e7_op81(s32 mode, Fx1e7Work81 *work, Unk800F78A4 *env, Unk80100C78Entry *entry, s32 *args) {
    BtlfxExports *ex = BTLFX_EXPORTS(env);
    s32 frame;
    s32 i;

    if (mode == 0) {
        *args = sizeof(Fx1e7Work81);
    } else if (mode == 1) {
        fx_1e7_env = env;
        ex->btlfx_getUnitBodyPos(0, 0, (s16 *)&work->pos20);
        ex->btlfx_getUnitPos(0, (s16 *)&work->pos28);
        fx_1e7_op81Origin.vx = work->pos28.vx;
        fx_1e7_op81Origin.vy = work->pos28.vy;
        fx_1e7_op81Origin.vz = work->pos28.vz;
        work->emit0 = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, FX_WRONG_RED_BURST), (s32)&work->pos28);
        work->emit18 = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, FX_WRONG_RED_RINGS), (s32)&work->pos28);
        work->emit1C = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, FX_WRONG_RED_SMOKE), (s32)&work->pos28);
        if (work->emit0 != NULL) {
            work->emit0->otZ = ex->btlfx_addRadiusSixth(0, 0x20);
        }
        if (work->emit1C != NULL) {
            work->emit1C->otZ = ex->btlfx_addRadiusSixth(0, 0x20);
        }
        for (i = 0; i < FX_WRONG_STREAMS; i++) {
            fxm_posPathInit(&fx_1e7_op81Paths[i], (u16 *)ex->btlfx_getFileData(entry->file, fx_1e7_op81PathFileIds[i]));
            work->emit4[i] = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, fx_1e7_op81EmitFileIds[i]), (s32)&fx_1e7_emitPos1[i]);
            if (work->emit4[i] != NULL) {
                work->emit4[i]->tail->shape = -1;
                work->emit4[i]->tail->s.module.spawnFn = fx_1e7_op81SpawnPoints[i];
            }
        }
        work->unk30 = 0;
        work->flag34 = ex->btlfx_isUnitActive(0);
        if (work->flag34 != 0) {
            fxm_startHeavyHitMotionIfActive(0);
        }
    } else {
        frame = entry->frame;
        fx_1e7_op81Frame = frame;
        if (work->flag34 != 0 && ex->btlfx_isUnitActive(0) != 0 && fxm_isMotionDone(0) != 0) {
            fxm_startUnitMotion(0, BTL_MOTION_KO);
            work->flag34 = 0;
        }
        ex->btlfx_getUnitPos(0, (s16 *)&work->pos28);
        work->pos28.vz -= FX_WRONG_KNOCKBACK_STEP;
        ex->btlfx_setUnitPos(0, (s16 *)&work->pos28);
        return frame >= FX_WRONG_KNOCKBACK_FRAMES;
    }
    return 0;
}

static void fxm_posPathInit(Fx1e7Path *path, u16 *data) {
    data++;
    path->count2 = *data++;
    path->keys4 = (Fx1e7Point *)data;
}

static s32 fxm_posPathGet(Fx1e7Path *path, s32 t, SVECTOR *out) {
    s32 last = path->count2 - 1;
    s32 done = 1;
    Fx1e7Point *key;

    if (last < t) {
        t = last;
    } else {
        done = 0;
    }
    key = &path->keys4[t];
    out->vx = key->x;
    out->vy = key->y;
    out->vz = key->z;
    return done;
}

static void fxm_startUnitMotion(s32 unit, s32 motion) {
    BtlfxExports *ex = BTLFX_EXPORTS(fx_1e7_env);

    ex->btlfx_setUnitMotion(unit, ex->btlfx_getUnitMotion(unit, motion));
    ex->btlfx_setMotionFrame(unit, 0);
}

static s32 fxm_isMotionDone(s32 unit) {
    BtlfxExports *ex = BTLFX_EXPORTS(fx_1e7_env);

    s32 done;

    if (ex->btlfx_isUnitActive(unit)) {
        done = ex->btlfx_getMotionFrame(unit) == ex->btlfx_getMotionLength(unit) - 1;
    } else {
        done = 0;
    }
    return done;
}

static void fxm_startHitMotionIfActive(s32 unit) {
    if (BTLFX_EXPORTS(fx_1e7_env)->btlfx_isUnitActive(unit)) {
        fxm_startUnitMotion(unit, BTL_MOTION_HIT);
    }
}

static void fxm_startHeavyHitMotionIfActive(s32 unit) {
    if (BTLFX_EXPORTS(fx_1e7_env)->btlfx_isUnitActive(unit)) {
        fxm_startUnitMotion(unit, BTL_MOTION_HIT_HEAVY);
    }
}

static void fxm_setSignal(Unk800F78A4 *env, s32 signal, s32 value) {
    env->state->signals[signal] = value;
}

INCLUDE_RODATA("asm/us/fx_1e7/nonmatchings/fx_1e7", fx_1e7_headerPtr);
