#include "fxmod/fx_09b.h"

s32 fxm_pinParticle0(Pool30Entry *pt, s32 arg1, SVECTOR *pos, s32 arg3, s32 frame, s32 n) {
    return fxm_pinParticle(pt, pos, &fx_09b_shotPos[0], frame, n);
}

s32 fxm_pinParticle1(Pool30Entry *pt, s32 arg1, SVECTOR *pos, s32 arg3, s32 frame, s32 n) {
    return fxm_pinParticle(pt, pos, &fx_09b_shotPos[1], frame, n);
}

s32 fxm_pinParticle2(Pool30Entry *pt, s32 arg1, SVECTOR *pos, s32 arg3, s32 frame, s32 n) {
    return fxm_pinParticle(pt, pos, &fx_09b_shotPos[2], frame, n);
}

s32 fxm_pinParticle3(Pool30Entry *pt, s32 arg1, SVECTOR *pos, s32 arg3, s32 frame, s32 n) {
    return fxm_pinParticle(pt, pos, &fx_09b_shotPos[3], frame, n);
}

s32 fxm_pinParticle4(Pool30Entry *pt, s32 arg1, SVECTOR *pos, s32 arg3, s32 frame, s32 n) {
    return fxm_pinParticle(pt, pos, &fx_09b_shotPos[4], frame, n);
}

static s32 fxm_pinParticle(Pool30Entry *pt, SVECTOR *pos, SVECTOR *src, s32 frame, s32 n) {
    pos->vx = src->vx;
    pos->vy = src->vy;
    pos->vz = src->vz;
    pt->pos.vx = pos->vx << 12;
    pt->pos.vy = pos->vy << 12;
    pt->pos.vz = pos->vz << 12;
    if (frame >= n) {
        return -1;
    }
    return 0;
}

s32 fx_09b_op80(s32 mode, Fx09bWork *work, Unk800F78A4 *env, Unk80100C78Entry *entry, s32 *arg4) {
    BtlfxExports *ex = BTLFX_EXPORTS(env);
    SVECTOR ang;
    s32 frame;

    if (mode == 0) {
        *arg4 = 0x74;
    } else if (mode == 1) {
        s32 n = *arg4;
        fx_09b_env = env;
        work->instance6C = n;
        if (n >= FX_09B_SHOTS) {
            return 1;
        }
        if (n == 0) {
            ex->btlfx_initEffModels(fx_09b_effModels, 5);
        }
        ex->btlfx_getUnitBodyPos(0, 0, (s16 *)&work->pos54);
        work->pos64.vx = work->pos54.vx;
        work->pos64.vy = work->pos54.vy;
        work->pos64.vz = work->pos54.vz;
        if (ex->btlfx_rand() & 1) {
            work->pos64.vx += ex->btlfx_rand() % 100;
        } else {
            work->pos64.vx -= ex->btlfx_rand() % 100;
        }
        if (ex->btlfx_rand() & 1) {
            work->pos64.vy += ex->btlfx_rand() % 100;
        } else {
            work->pos64.vy -= ex->btlfx_rand() % 100;
        }
        ex->btlfx_getUnitArgBonePos(0x10, 0, (u16 *)&work->pos3C);
        work->fromPos5C.vx = work->pos3C.vx;
        work->fromPos5C.vy = work->pos3C.vy;
        work->fromPos5C.vz = work->pos3C.vz;
        env->camera->points[0].vx = work->pos3C.vx;
        env->camera->points[0].vy = work->pos3C.vy;
        env->camera->points[0].vz = work->pos3C.vz;
        work->rot14.vx = 0;
        work->rot14.vy = 0;
        work->rot14.vz = 0;
        work->pos1C.vx = 0;
        work->pos1C.vy = 0;
        work->pos1C.vz = 0;
        work->scale2C.vx = 0x1000;
        work->scale2C.vy = 0x1000;
        work->scale2C.vz = 0x1000;
        work->model70 = ex->btlfx_registerEffectModel((Unk800E8F50 *)ex->btlfx_getFileData(entry->file, 6), 1);
        work->emit0 = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, 1), (s32)&work->pos3C);
        work->emit4 = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, fx_09b_shotBlocks[work->instance6C]),
                                       (s32)&fx_09b_emitStartPos[work->instance6C]);
        if (work->emit4 != NULL) {
            work->emit4->trails = (s32)fx_09b_trails[work->instance6C];
            work->emit4->trailSlots = 0xE;
            work->emit4->trailStride = 0x4C;
        }
        if (work->emit4 != NULL) {
            work->emit4->file->m.module.update = fx_09b_pinParticles[work->instance6C];
        }
        if (work->emit0 != NULL) {
            work->emit0->otZ = ex->btlfx_addRadiusSixth(0x10, 0x80);
        }
    } else {
        frame = entry->frame;
        switch (frame) {
        case 2:
            ex->btlfx_aimAngles((s16 *)&work->pos3C, (s16 *)&work->pos64, (s16 *)&ang);
            ex->btlfx_pointAhead((s16 *)&work->pos64, (s16 *)&ang, -0xC8, (s16 *)&work->pos44);
            work->emitC = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, 4), (s32)&work->pos44);
            if (work->emitC != NULL) {
                work->emitC->otZ = ex->btlfx_addRadiusSixth(0, 0x80);
            }
            break;
        case 4:
            work->pos4C.vx = work->pos64.vx;
            work->pos4C.vy = work->pos64.vy;
            work->pos4C.vz = work->pos64.vz;
            work->pos4C.vy = 0;
            work->emit8 = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, 3), (s32)&work->pos64);
            work->emit10 = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, 5), (s32)&work->pos4C);
            if (work->emit8 != NULL) {
                work->emit8->otZ = ex->btlfx_addRadiusSixth(0, 0x80);
            }
            break;
        case 6:
            ex->btlfx_playFileSound(0x13, entry->file, -1);
            ex->btlfx_playFileSound(0x14, entry->file, -1);
            ex->btlfx_playFileSound(0x15, entry->file, -1);
            break;
        case 8:
            ex->btlfx_stopEmitter(work->emit4);
            break;
        }
        if ((u32)frame < 6) {
            ex->btlfx_lerpVec((s16 *)&work->fromPos5C, (s16 *)&work->pos64, (frame << 12) / 5,
                              (s16 *)&fx_09b_shotPos[work->instance6C]);
            work->pos1C.vx = fx_09b_shotPos[work->instance6C].vx;
            work->pos1C.vy = fx_09b_shotPos[work->instance6C].vy;
            work->pos1C.vz = fx_09b_shotPos[work->instance6C].vz;
            ex->Hi_DrawEffModel(&work->rot14, &work->pos1C, (s32)&work->scale2C, work->model70);
            if (frame == 5 && work->instance6C != FX_09B_LAST_SHOT) {
                fxm_startHitMotionIfActive(0);
            }
        }
        if (frame >= 0x28) {
            if (work->instance6C == FX_09B_LAST_SHOT) {
                ex->btlfx_clearEffModels();
                return 1;
            }
            return 1;
        }
    }
    return 0;
}

static void fxm_startUnitMotion(s32 unit, s32 motion) {
    BtlfxExports *ex = BTLFX_EXPORTS(fx_09b_env);

    ex->btlfx_setUnitMotion(unit, ex->btlfx_getUnitMotion(unit, motion));
    ex->btlfx_setMotionFrame(unit, 0);
}

static s32 fxm_isMotionDone(s32 unit) {
    BtlfxExports *ex = BTLFX_EXPORTS(fx_09b_env);

    s32 r;

    if (ex->btlfx_isUnitActive(unit)) {
        r = ex->btlfx_getMotionFrame(unit) == ex->btlfx_getMotionLength(unit) - 1;
    } else {
        r = 0;
    }
    return r;
}

static void fxm_startHitMotionIfActive(s32 unit) {
    if (BTLFX_EXPORTS(fx_09b_env)->btlfx_isUnitActive(unit)) {
        fxm_startUnitMotion(unit, BTL_MOTION_HIT);
    }
}

static void fxm_startHeavyHitMotionIfActive(s32 unit) {
    if (BTLFX_EXPORTS(fx_09b_env)->btlfx_isUnitActive(unit)) {
        fxm_startUnitMotion(unit, BTL_MOTION_HIT_HEAVY);
    }
}

INCLUDE_RODATA("asm/us/fx_09b/nonmatchings/fx_09b", fx_09b_headerPtr);
