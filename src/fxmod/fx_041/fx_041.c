#include "fxmod/fx_041.h"

static s32 fx_041_op80ParticleMotion(VECTOR *out, s32 owner, SVECTOR *pos, s32 trail, s32 t, s32 n) {
    SVECTOR v;

    fxm_posPathGet(&fx_041_path, t, &v);
    v.vy = -300;
    fx_041_lastPathPos.vy = -300;
    fx_041_lastPathPos.vx = v.vx;
    fx_041_lastPathPos.vz = v.vz;
    v.vx += fx_041_targetPos.vx;
    v.vz += fx_041_targetPos.vz;
    pos->vx = v.vx;
    pos->vy = v.vy;
    pos->vz = v.vz;
    out->vx = pos->vx << 12;
    out->vy = pos->vy << 12;
    out->vz = pos->vz << 12;
    if (t >= n) {
        return -1;
    }
    return 0;
}

s32 fx_041_op80(s32 mode, Fx041Work80 *work, Unk800F78A4 *env, Unk80100C78Entry *entry, s32 *args) {
    BtlfxExports *ex = BTLFX_EXPORTS(env);
    s32 frame;

    if (mode == 0) {
        *args = 0x30;
    } else if (mode == 1) {
        fx_041_env = env;
        ex->btlfx_getUnitBodyPos(0, 0, (s16 *)&work->pos18);
        ex->btlfx_getUnitPos(0, (s16 *)&work->pos20);
        fx_041_targetPos.vx = work->pos20.vx;
        fx_041_targetPos.vy = work->pos20.vy;
        fx_041_targetPos.vz = work->pos20.vz;
        fxm_posPathInit(&fx_041_path, (u16 *)ex->btlfx_getFileData(entry->file, 8));
    } else {
        frame = entry->frame;
        switch (frame) {
        case 3:
            ex->btlfx_getUnitPos(0x10, (s16 *)&work->pos28);
            work->emit0 = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, 7), (s32)&work->pos28);
            work->emit4 = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, 2), (s32)&work->pos20);
            if (work->emit0 != NULL) {
                work->emit0->otZ = ex->btlfx_addRadiusSixth(0x10, 0x20);
            }
            if (work->emit4 != NULL) {
                work->emit4->otZ = ex->btlfx_addRadiusSixth(0x10, 0x20);
            }
            break;
        case 4:
            fxm_startHitMotionIfActive(0);
            work->emit8 = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, 0xF), (s32)&work->pos18);
            work->emitC = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, 0x10), (s32)&work->pos18);
            if (work->emit8 != NULL) {
                work->emit8->otZ = ex->btlfx_addRadiusSixth(0, 0x20);
            }
            if (work->emitC != NULL) {
                work->emitC->otZ = ex->btlfx_addRadiusSixth(0, 0x20);
            }
            break;
        case 0xF:
            ex->btlfx_getUnitPos(0x10, (s16 *)&work->pos28);
            work->emit10 = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, 3), (s32)&work->pos28);
            if (work->emit10 != NULL) {
                work->emit10->otZ = ex->btlfx_addRadiusSixth(0x10, 0x20);
            }
            break;
        case 0x12:
            work->emit14 = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, 4), (s32)&fx_041_emitPos);
            if (work->emit14 != NULL) {
                work->emit14->trails = (s32)fx_041_trails;
                work->emit14->trailSlots = 1;
                work->emit14->trailStride = 0xD0;
            }
            if (work->emit14 != NULL) {
                ((s32 *)work->emit14->file)[2] = (s32)fx_041_op80ParticleMotion;
            }
            break;
        }
        if (frame >= 4 && fxm_isMotionDone(0)) {
            fxm_startUnitMotion(0, 0);
        }
        return frame >= 0x28;
    }
    return 0;
}

s32 fx_041_op81(s32 mode, Fx041Work81 *work, Unk800F78A4 *env, Unk80100C78Entry *entry, s32 *args) {
    BtlfxExports *ex = BTLFX_EXPORTS(env);

    if (mode == 0) {
        *args = 0x1C;
    } else if (mode == 1) {
        fx_041_env = env;
        ex->btlfx_getUnitPos(0, (s16 *)&work->posC);
        work->pos14.vx = work->posC.vx;
        work->pos14.vy = work->posC.vy;
        work->pos14.vz = work->posC.vz;
        work->pos14.vy = fx_041_lastPathPos.vy;
        work->emit0 = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, 5), (s32)&work->pos14);
        work->emit4 = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, 6), (s32)&work->pos14);
        work->emit8 = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, 1), (s32)&work->pos14);
    } else {
        return entry->frame >= 0x28;
    }
    return 0;
}

static void fxm_posPathInit(Fx041Path *path, u16 *data) {
    data++;
    path->count2 = *data++;
    path->points4 = (Fx041Point *)data;
}

static s32 fxm_posPathGet(Fx041Path *path, s32 t, SVECTOR *out) {
    s32 last = path->count2 - 1;
    s32 done = 1;
    Fx041Point *key;

    if (last < t) {
        t = last;
    } else {
        done = 0;
    }
    key = &path->points4[t];
    out->vx = key->x;
    out->vy = key->y;
    out->vz = key->z;
    return done;
}

static void fxm_startUnitMotion(s32 unit, s32 motion) {
    BtlfxExports *ex = BTLFX_EXPORTS(fx_041_env);

    ex->btlfx_setUnitMotion(unit, ex->btlfx_getUnitMotion(unit, motion));
    ex->btlfx_setMotionFrame(unit, 0);
}

static s32 fxm_isMotionDone(s32 unit) {
    BtlfxExports *ex = BTLFX_EXPORTS(fx_041_env);

    s32 r;

    if (ex->btlfx_isUnitActive(unit)) {
        r = ex->btlfx_getMotionFrame(unit) == ex->btlfx_getMotionLength(unit) - 1;
    } else {
        r = 0;
    }
    return r;
}

static void fxm_startHitMotionIfActive(s32 unit) {
    if (BTLFX_EXPORTS(fx_041_env)->btlfx_isUnitActive(unit)) {
        fxm_startUnitMotion(unit, BTL_MOTION_HIT);
    }
}

static void fxm_startHeavyHitMotionIfActive(s32 unit) {
    if (BTLFX_EXPORTS(fx_041_env)->btlfx_isUnitActive(unit)) {
        fxm_startUnitMotion(unit, BTL_MOTION_HIT_HEAVY);
    }
}

INCLUDE_RODATA("asm/us/fx_041/nonmatchings/fx_041", fx_041_headerPtr);
