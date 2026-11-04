#include "fxmod/fx_1a6.h"

s32 fx_1a6_op80ParticleMotion0(Pool30Entry *pt, s32 owner, SVECTOR *pos, s32 trail, s32 frame, s32 n) {
    return fxm_lerpParticle(pt, pos, &fx_1a6_motionFrom[0], &fx_1a6_motionTo[0], frame, n);
}

s32 fx_1a6_op80ParticleMotion1(Pool30Entry *pt, s32 owner, SVECTOR *pos, s32 trail, s32 frame, s32 n) {
    return fxm_lerpParticle(pt, pos, &fx_1a6_motionFrom[1], &fx_1a6_motionTo[1], frame, n);
}

s32 fx_1a6_op80ParticleMotion2(Pool30Entry *pt, s32 owner, SVECTOR *pos, s32 trail, s32 frame, s32 n) {
    return fxm_lerpParticle(pt, pos, &fx_1a6_motionFrom[2], &fx_1a6_motionTo[2], frame, n);
}

static s32 fxm_lerpParticle(Pool30Entry *pt, SVECTOR *pos, SVECTOR *from, SVECTOR *to, s32 frame, s32 frames) {
    BtlfxExports *ex = BTLFX_EXPORTS(fx_1a6_env);
    s32 t = frame << 9;

    if (t > 0x1000) {
        t = 0x1000;
    }
    ex->btlfx_lerpVec((s16 *)from, (s16 *)to, t, (s16 *)pos);
    pt->pos.vx = pos->vx << 12;
    pt->pos.vy = pos->vy << 12;
    pt->pos.vz = pos->vz << 12;
    if (frame >= frames) {
        return -1;
    }
    return 0;
}

s32 fx_1a6_op80(s32 mode, Fx1a6Work80 *work, Unk800F78A4 *env, Unk80100C78Entry *entry, s32 *args) {
    BtlfxExports *ex = BTLFX_EXPORTS(env);
    s32 frame;

    if (mode == 0) {
        *args = 0x4C;
    } else if (mode == 1) {
        s32 shot = *args;
        fx_1a6_env = env;
        work->index48 = shot;
        if (shot >= FX_1A6_SHOTS) {
            return 1;
        }
        ex->btlfx_playFileSound(2, entry->file, -1);
        ex->btlfx_playFileSound(4, entry->file, -1);
        ex->btlfx_playFileSound(7, entry->file, -1);
        ex->btlfx_getUnitBodyPos(0x10, 0, (s16 *)&work->pos10);
        ex->btlfx_getUnitBodyPos(0, 0, (s16 *)&work->pos18);
        ex->btlfx_getUnitArgBonePos(0x10, 0, (u16 *)&work->pos20);
        if (ex->btlfx_rand() & 1) {
            work->pos18.vx += 0x32 + (s32)ex->btlfx_rand % 200;
        } else {
            work->pos18.vx -= 0x32 + (s32)ex->btlfx_rand % 200;
        }
        if (ex->btlfx_rand() & 1) {
            work->pos18.vy += 0x32 + (s32)ex->btlfx_rand % 200;
        } else {
            work->pos18.vy -= 0x32 + (s32)ex->btlfx_rand % 200;
        }
        ex->btlfx_aimAngles((s16 *)&work->pos20, (s16 *)&work->pos18, (s16 *)&work->angle38);
        ex->btlfx_aimAngles((s16 *)&work->pos18, (s16 *)&work->pos20, (s16 *)&work->angle40);
        ex->btlfx_pointAhead((s16 *)&work->pos20, (s16 *)&work->angle38, 0x12C, (s16 *)&work->pos28);
        ex->btlfx_pointAhead((s16 *)&work->pos20, (s16 *)&work->angle38, 0xC8, (s16 *)&fx_1a6_motionFrom[work->index48]);
        fx_1a6_motionTo[work->index48].vx = work->pos18.vx;
        fx_1a6_motionTo[work->index48].vy = work->pos18.vy;
        fx_1a6_motionTo[work->index48].vz = work->pos18.vz;
        work->emit0 = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, FX_WATER_GUN_PARTICLE), (s32)&work->pos28);
        work->emit4 = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, fx_1a6_emitFileIds[work->index48]),
                                       (s32)&fx_1a6_emitPos0[work->index48]);
        if (work->emit0 != NULL) {
            work->emit0->trails = (s32)fx_1a6_op80Trails0[work->index48];
            work->emit0->trailSlots = 0xF;
            work->emit0->trailStride = 0x18;
        }
        if (work->emit4 != NULL) {
            work->emit4->trails = (s32)fx_1a6_op80Trails1[work->index48];
            work->emit4->trailSlots = 0x1D;
            work->emit4->trailStride = 0x40;
        }
        if (work->emit4 != NULL) {
            work->emit4->file->m.module.update = fx_1a6_op80ParticleMotions[work->index48];
        }
        if (work->emit0 != NULL) {
            work->emit0->otZ = ex->btlfx_addRadiusSixth(0x10, 0x20);
        }
        if (work->emit4 != NULL) {
            work->emit4->otZ = ex->btlfx_addRadiusSixth(0, 0x20);
        }
    } else {
        frame = entry->frame;
        switch (frame) {
        case 6:
            if (work->index48 != FX_1A6_LAST_SHOT) {
                fxm_startHitMotionIfActive(0);
            }
            work->emit8 = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, FX_WATER_GUN_WATER_JET), (s32)&work->pos18);
            work->emitC = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, FX_WATER_GUN_SPLASH), (s32)&work->pos18);
            if (work->emit8 != NULL) {
                work->emit8->trails = (s32)fx_1a6_op80Trails2[work->index48];
                work->emit8->trailSlots = 0x56;
                work->emit8->trailStride = 0x40;
            }
            if (work->emit8 != NULL) {
                work->emit8->otZ = ex->btlfx_addRadiusSixth(0, 0x80);
            }
            if (work->emitC != NULL) {
                work->emitC->otZ = ex->btlfx_addRadiusSixth(0, 0x80);
            }
            break;
        case 8:
            ex->btlfx_playFileSound(8, entry->file, -1);
            ex->btlfx_playFileSound(0xD, entry->file, -1);
            ex->btlfx_playFileSound(0xE, entry->file, -1);
            break;
        }
        if (work->emit0 != NULL) {
            if (work->emit0->state == BTLFX_EMIT_DONE) {
                work->emit0 = NULL;
            }
            if (work->emit0 != NULL) {
                work->emit0->rot.vx = work->angle40.vx;
                work->emit0->rot.vy = work->angle40.vy;
                work->emit0->rot.vz = work->angle40.vz;
            }
        }
        if (frame >= 6) {
            if (work->emitC != NULL) {
                if (work->emitC->state == BTLFX_EMIT_DONE) {
                    work->emitC = NULL;
                }
                if (work->emitC != NULL) {
                    work->emitC->rot.vx = work->angle38.vx;
                    work->emitC->rot.vy = work->angle38.vy;
                    work->emitC->rot.vz = work->angle38.vz;
                }
            }
            if (frame >= 6 && work->index48 != 2 && fxm_isMotionDone(0) != 0) {
                fxm_startUnitMotion(0, 0);
            }
        }
        return frame >= 0x28;
    }
    return 0;
}

s32 fx_1a6_op81(s32 mode, Fx1a6Work *work, Unk800F78A4 *env, Unk80100C78Entry *entry, s32 *args) {
    BtlfxExports *ex = BTLFX_EXPORTS(env);

    if (mode == 0) {
        *args = 0xC;
    } else if (mode == 1) {
        fx_1a6_env = env;
        ex->btlfx_getUnitPos(0, (s16 *)&work->pos4);
        work->emit0 = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, FX_WATER_GUN_BUBBLES), (s32)&work->pos4);
    } else {
        return entry->frame >= 0x28;
    }
    return 0;
}

static void fxm_startUnitMotion(s32 unit, s32 motion) {
    BtlfxExports *ex = BTLFX_EXPORTS(fx_1a6_env);

    ex->btlfx_setUnitMotion(unit, ex->btlfx_getUnitMotion(unit, motion));
    ex->btlfx_setMotionFrame(unit, 0);
}

static s32 fxm_isMotionDone(s32 unit) {
    BtlfxExports *ex = BTLFX_EXPORTS(fx_1a6_env);

    s32 done;

    if (ex->btlfx_isUnitActive(unit)) {
        done = ex->btlfx_getMotionFrame(unit) == ex->btlfx_getMotionLength(unit) - 1;
    } else {
        done = 0;
    }
    return done;
}

static void fxm_startHitMotionIfActive(s32 unit) {
    if (BTLFX_EXPORTS(fx_1a6_env)->btlfx_isUnitActive(unit)) {
        fxm_startUnitMotion(unit, BTL_MOTION_HIT);
    }
}

static void fxm_startHeavyHitMotionIfActive(s32 unit) {
    if (BTLFX_EXPORTS(fx_1a6_env)->btlfx_isUnitActive(unit)) {
        fxm_startUnitMotion(unit, BTL_MOTION_HIT_HEAVY);
    }
}

INCLUDE_RODATA("asm/us/fx_1a6/nonmatchings/fx_1a6", fx_1a6_headerPtr);
