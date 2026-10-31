#include "fxmod/fx_1a6.h"

s32 fx_1a6_op80ParticleMotion0(Pool30Entry *pt, s32 arg1, SVECTOR *pos, s32 arg3, s32 frame, s32 n) {
    return fxm_lerpParticle(pt, pos, &D_801EDD28[0], &D_801EDD48[0], frame, n);
}

s32 fx_1a6_op80ParticleMotion1(Pool30Entry *pt, s32 arg1, SVECTOR *pos, s32 arg3, s32 frame, s32 n) {
    return fxm_lerpParticle(pt, pos, &D_801EDD28[1], &D_801EDD48[1], frame, n);
}

s32 fx_1a6_op80ParticleMotion2(Pool30Entry *pt, s32 arg1, SVECTOR *pos, s32 arg3, s32 frame, s32 n) {
    return fxm_lerpParticle(pt, pos, &D_801EDD28[2], &D_801EDD48[2], frame, n);
}

static s32 fxm_lerpParticle(Pool30Entry *e, SVECTOR *pos, SVECTOR *from, SVECTOR *to, s32 frame, s32 frames) {
    BtlfxExports *ex = BTLFX_EXPORTS(fx_1a6_env);
    s32 t = frame << 9;

    if (t > 0x1000) {
        t = 0x1000;
    }
    ex->btlfx_lerpVec((s16 *)from, (s16 *)to, t, (s16 *)pos);
    e->pos.vx = pos->vx << 12;
    e->pos.vy = pos->vy << 12;
    e->pos.vz = pos->vz << 12;
    if (frame >= frames) {
        return -1;
    }
    return 0;
}

s32 fx_1a6_op80(s32 mode, Fx1a6Work80 *work, Unk800F78A4 *env, Unk80100C78Entry *entry, s32 *arg4) {
    BtlfxExports *ex = BTLFX_EXPORTS(env);
    s32 frame;

    if (mode == 0) {
        *arg4 = 0x4C;
    } else if (mode == 1) {
        s32 n = *arg4;
        fx_1a6_env = env;
        work->unk48 = n;
        if (n >= 3) {
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
        ex->btlfx_aimAngles((s16 *)&work->pos20, (s16 *)&work->pos18, (s16 *)&work->unk38);
        ex->btlfx_aimAngles((s16 *)&work->pos18, (s16 *)&work->pos20, (s16 *)&work->unk40);
        ex->btlfx_pointAhead((s16 *)&work->pos20, (s16 *)&work->unk38, 0x12C, (s16 *)&work->pos28);
        ex->btlfx_pointAhead((s16 *)&work->pos20, (s16 *)&work->unk38, 0xC8, (s16 *)&D_801EDD28[work->unk48]);
        D_801EDD48[work->unk48].vx = work->pos18.vx;
        D_801EDD48[work->unk48].vy = work->pos18.vy;
        D_801EDD48[work->unk48].vz = work->pos18.vz;
        work->emit0 = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, 3), (s32)&work->pos28);
        work->emit4 = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, D_801E823C[work->unk48]),
                                       (s32)&D_801EDD68[work->unk48]);
        if (work->emit0 != NULL) {
            work->emit0->trails = (s32)fx_1a6_op80Trails0[work->unk48];
            work->emit0->trailSlots = 0xF;
            work->emit0->trailStride = 0x18;
        }
        if (work->emit4 != NULL) {
            work->emit4->trails = (s32)fx_1a6_op80Trails1[work->unk48];
            work->emit4->trailSlots = 0x1D;
            work->emit4->trailStride = 0x40;
        }
        if (work->emit4 != NULL) {
            ((EffMotion *)work->emit4->file)->update = fx_1a6_op80ParticleMotions[work->unk48];
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
            if (work->unk48 != 2) {
                fxm_startHitMotionIfActive(0);
            }
            work->emit8 = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, 6), (s32)&work->pos18);
            work->emitC = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, 5), (s32)&work->pos18);
            if (work->emit8 != NULL) {
                work->emit8->trails = (s32)fx_1a6_op80Trails2[work->unk48];
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
                work->emit0->rot.vx = work->unk40.vx;
                work->emit0->rot.vy = work->unk40.vy;
                work->emit0->rot.vz = work->unk40.vz;
            }
        }
        if (frame >= 6) {
            if (work->emitC != NULL) {
                if (work->emitC->state == BTLFX_EMIT_DONE) {
                    work->emitC = NULL;
                }
                if (work->emitC != NULL) {
                    work->emitC->rot.vx = work->unk38.vx;
                    work->emitC->rot.vy = work->unk38.vy;
                    work->emitC->rot.vz = work->unk38.vz;
                }
            }
            if (frame >= 6 && work->unk48 != 2 && fxm_isMotionDone(0) != 0) {
                fxm_startUnitMotion(0, 0);
            }
        }
        return frame >= 0x28;
    }
    return 0;
}

s32 fx_1a6_op81(s32 mode, Fx1a6Work *work, Unk800F78A4 *env, Unk80100C78Entry *entry, s32 *arg4) {
    BtlfxExports *ex = BTLFX_EXPORTS(env);

    if (mode == 0) {
        *arg4 = 0xC;
    } else if (mode == 1) {
        fx_1a6_env = env;
        ex->btlfx_getUnitPos(0, (s16 *)&work->pos4);
        work->emit0 = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, 1), (s32)&work->pos4);
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

    s32 r;

    if (ex->btlfx_isUnitActive(unit)) {
        r = ex->btlfx_getMotionFrame(unit) == ex->btlfx_getMotionLength(unit) - 1;
    } else {
        r = 0;
    }
    return r;
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
