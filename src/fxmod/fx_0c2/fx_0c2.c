#include "fxmod/fx_0c2.h"

s32 fx_0c2_op80(s32 mode, Fx0c2Work80 *work, Unk800F78A4 *env, Unk80100C78Entry *entry, s32 *args) {
    BtlfxExports *ex = BTLFX_EXPORTS(env);
    BtlBone bone;

    if (mode == 0) {
        *args = sizeof(Fx0c2Work80);
    } else if (mode == 1) {
        fx_0c2_env = env;
        ex->btlfx_getUnitBoneMatrix(BTLFX_UNIT_CASTER, FX_NO_MERCY_BONE, &bone);
        ex->btlfx_matrixPointAhead((MATRIX *)&bone, 0x64, (s32)&work->pos4);
        work->emit0 = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, FX_NO_MERCY_BLUE_RING), (s32)&work->pos4);
        if (work->emit0 != NULL) {
            work->emit0->otZ = ex->btlfx_addRadiusSixth(BTLFX_UNIT_CASTER, 0x20);
        }
    } else {
        return entry->frame >= FX_NO_MERCY_RING_FRAMES;
    }
    return 0;
}

static s32 fx_0c2_op81ParticleMotion(Pool30Entry *pt, s32 owner, SVECTOR *pos, s32 trail, s32 frame, s32 n) {
    BTLFX_EXPORTS(fx_0c2_env)->btlfx_lerpVec((s16 *)&fx_0c2_pathFrom, (s16 *)&fx_0c2_pathTo, (frame << 12) / FX_NO_MERCY_FLASH_PATH_FRAMES, (s16 *)pos);
    pt->pos.vx = pos->vx << 12;
    pt->pos.vy = pos->vy << 12;
    pt->pos.vz = pos->vz << 12;
    if (frame >= n) {
        return -1;
    }
    return 0;
}

s32 fx_0c2_op81(s32 mode, Fx0c2Work81 *work, Unk800F78A4 *env, Unk80100C78Entry *entry, s32 *args) {
    BtlfxExports *ex = BTLFX_EXPORTS(env);
    SVECTOR ang;
    s32 frame;

    if (mode == 0) {
        *args = sizeof(Fx0c2Work81);
    } else if (mode == 1) {
        s32 unit = *args;
        work->unit4C = unit;
        fx_0c2_env = env;
        if (unit >= env->targets->targetCount) {
            fxm_setSignal(env, BTLFX_SIG_WAIT, 1);
            return 1;
        }
        fxm_setSignal(env, BTLFX_SIG_WAIT, 0);
        fxm_startUnitMotion(BTLFX_UNIT_CASTER, BTL_MOTION_CAST_END);
        ex->btlfx_getUnitPos(BTLFX_UNIT_CASTER, (s16 *)&work->posC);
        ex->btlfx_getUnitRot(BTLFX_UNIT_CASTER, (s16 *)&work->rot14);
        ex->btlfx_getUnitBodyPos(work->unit4C, 0, (s16 *)&work->pos1C);
        ex->btlfx_getUnitRot(work->unit4C, (s16 *)&work->rot24);
    } else {
        frame = entry->frame;
        if (frame == FX_NO_MERCY_DASH_FRAME) {
            ex->btlfx_getUnitBonePos(BTLFX_UNIT_CASTER, FX_NO_MERCY_BONE, (u16 *)&work->pos34);
            fx_0c2_pathFrom.vx = work->pos34.vx;
            fx_0c2_pathFrom.vy = work->pos34.vy;
            fx_0c2_pathFrom.vz = work->pos34.vz;
            ex->btlfx_getUnitRot(BTLFX_UNIT_CASTER, (s16 *)&work->rot14);
            ex->btlfx_pointAhead((s16 *)&work->posC, (s16 *)&work->rot14, FX_NO_MERCY_DUST_REACH, (s16 *)&work->pos2C);
            ex->btlfx_aimAngles((s16 *)&work->posC, (s16 *)&work->pos1C, (s16 *)&ang);
            ex->btlfx_pointAhead((s16 *)&work->pos1C, (s16 *)&ang, 0, (s16 *)&fx_0c2_pathTo);
            work->emit0 = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, FX_NO_MERCY_DUST_PLUME), (s32)&work->pos2C);
            work->emit4 = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, FX_NO_MERCY_FLASH_BURST), (s32)&fx_0c2_emitPos);
            if (work->emit4 != NULL) {
                work->emit4->file->m.module.update = fx_0c2_op81ParticleMotion;
            }
            if (work->emit4 != NULL) {
                work->emit4->otZ = ex->btlfx_addRadiusSixth(BTLFX_UNIT_CASTER, 0x80);
            }
            ex->btlfx_aimAngles((s16 *)&work->pos34, (s16 *)&work->pos1C, (s16 *)&work->rot44);
            work->rot3C.vx = work->rot44.vx;
            work->rot3C.vy = work->rot44.vy;
            work->rot3C.vz = work->rot44.vz;
        }
        if (frame == FX_NO_MERCY_IMPACT_FRAME) {
            work->emit8 = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, FX_NO_MERCY_IMPACT_BURST), (s32)&work->pos1C);
            if (work->emit8 != NULL) {
                work->emit8->otZ = ex->btlfx_addRadiusSixth(work->unit4C, 0x80);
            }
        }
        switch (frame) {
        case FX_NO_MERCY_DASH_SOUND_FRAME:
            ex->btlfx_playFileSound(7, entry->file, -1);
            ex->btlfx_playFileSound(8, entry->file, -1);
            ex->btlfx_playFileSound(9, entry->file, -1);
            break;
        case FX_NO_MERCY_IMPACT_FRAME:
            ex->btlfx_playFileSound(0xE, entry->file, -1);
            ex->btlfx_playFileSound(0xF, entry->file, -1);
            ex->btlfx_playFileSound(0x10, entry->file, -1);
            break;
        }
        if (frame >= FX_NO_MERCY_DASH_FRAME) {
            if (work->emit0 != NULL) {
                if (work->emit0->state == BTLFX_EMIT_DONE) {
                    work->emit0 = NULL;
                }
                if (work->emit0 != NULL) {
                    work->emit0->rot.vx = work->rot3C.vx;
                    work->emit0->rot.vy = work->rot3C.vy;
                    work->emit0->rot.vz = work->rot3C.vz;
                }
            }
            if (work->emit4 != NULL) {
                if (work->emit4->state == BTLFX_EMIT_DONE) {
                    work->emit4 = NULL;
                }
                if (work->emit4 != NULL) {
                    work->emit4->rot.vx = work->rot44.vx;
                    work->emit4->rot.vy = work->rot44.vy;
                    work->emit4->rot.vz = work->rot44.vz;
                }
            }
        }
        if (fxm_isMotionDone(BTLFX_UNIT_CASTER)) {
            fxm_startUnitMotion(BTLFX_UNIT_CASTER, BTL_MOTION_READY);
        }
        if (frame >= FX_NO_MERCY_FRAMES) {
            fxm_setSignal(env, BTLFX_SIG_WAIT, 1);
            return 1;
        }
    }
    return 0;
}

s32 fx_0c2_op82(s32 mode, Fx0c2Work82 *work, Unk800F78A4 *env, Unk80100C78Entry *entry, s32 *args) {
    s32 frame;

    if (mode == 0) {
        *args = sizeof(Fx0c2Work82);
    } else if (mode == 1) {
        work->pos8.vx = env->targets->centerX;
        work->pos8.vy = env->targets->centerY;
        fx_0c2_env = env;
        work->pos8.vz = env->targets->centerZ;
    } else {
        frame = entry->frame;
        return fxm_turnUnitBy(BTLFX_UNIT_CASTER, frame, FX_NO_MERCY_TURN_FRAMES, &work->pos8, &work->turn0) != 0;
    }
    return 0;
}

s32 fx_0c2_op83(s32 mode, Fx0c2Work83 *work, Unk800F78A4 *env, Unk80100C78Entry *entry, s32 *args) {
    BtlfxExports *ex = BTLFX_EXPORTS(env);
    s32 frame;

    if (mode == 0) {
        *args = sizeof(Fx0c2Work83);
    } else if (mode == 1) {
        s32 unit = *args;
        work->unit10 = unit;
        fx_0c2_env = env;
        if (unit >= env->targets->targetCount) {
            return 1;
        }
        ex->btlfx_getUnitPos(unit, (s16 *)&work->pos8);
    } else {
        frame = entry->frame;
        return fxm_turnUnitToward(BTLFX_UNIT_CASTER, work->unit10, frame, FX_NO_MERCY_FACE_FRAMES, 0, 1, &work->turn0) != 0;
    }
    return 0;
}

static s32 fxm_turnUnitBy(s32 unit, s32 frame, s32 n, SVECTOR *unused, SVECTOR *delta) {
    BtlfxExports *ex = BTLFX_EXPORTS(fx_0c2_env);
    SVECTOR rot;
    SVECTOR pos;

    ex->btlfx_getUnitPos(unit, (s16 *)&pos);
    if (frame == 0) {
        ex->btlfx_getUnitRot(unit, (s16 *)&rot);
    }
    ex->btlfx_getUnitRot(unit, (s16 *)&rot);
    rot.vx += delta->vx / n;
    rot.vy += delta->vy / n;
    rot.vz += delta->vz / n;
    ex->btlfx_setUnitRot(unit, (s16 *)&rot);
    return frame >= n - 1;
}

static s32 fxm_turnUnitToward(s32 unit, s32 target, s32 frame, s32 n, s32 turnBack, s32 relative, SVECTOR *ang) {
    BtlfxExports *ex = BTLFX_EXPORTS(fx_0c2_env);
    SVECTOR from;
    SVECTOR rot;
    SVECTOR to;

    if (frame == 0) {
        ex->btlfx_getUnitPos(unit, (s16 *)&from);
        ex->btlfx_getUnitPos(target, (s16 *)&to);
        ex->btlfx_aimAngles((s16 *)&from, (s16 *)&to, (s16 *)ang);
        if (relative) {
            ex->btlfx_getUnitRot(unit, (s16 *)&rot);
            ang->vx -= rot.vx;
            ang->vy -= rot.vy;
            ang->vz -= rot.vz;
            if (ang->vy >= 0x800) {
                ang->vy = 0x1000 - ang->vy;
            } else if (ang->vy < -0x7FF) {
                ang->vy = ang->vy + 0x1000;
            }
        }
        if (turnBack == 1) {
            ang->vy += 0x800;
        }
    }
    ex->btlfx_getUnitRot(unit, (s16 *)&rot);
    rot.vx += ang->vx / n;
    rot.vy += ang->vy / n;
    rot.vz += ang->vz / n;
    ex->btlfx_setUnitRot(unit, (s16 *)&rot);
    return frame >= n - 1;
}

static void fxm_startUnitMotion(s32 unit, s32 motion) {
    BtlfxExports *ex = BTLFX_EXPORTS(fx_0c2_env);

    ex->btlfx_setUnitMotion(unit, ex->btlfx_getUnitMotion(unit, motion));
    ex->btlfx_setMotionFrame(unit, 0);
}

static s32 fxm_isMotionDone(s32 unit) {
    BtlfxExports *ex = BTLFX_EXPORTS(fx_0c2_env);

    s32 done;

    if (ex->btlfx_isUnitActive(unit)) {
        done = ex->btlfx_getMotionFrame(unit) == ex->btlfx_getMotionLength(unit) - 1;
    } else {
        done = 0;
    }
    return done;
}

static void fxm_startHitMotionIfActive(s32 unit) {
    if (BTLFX_EXPORTS(fx_0c2_env)->btlfx_isUnitActive(unit)) {
        fxm_startUnitMotion(unit, BTL_MOTION_HIT);
    }
}

static void fxm_startHeavyHitMotionIfActive(s32 unit) {
    if (BTLFX_EXPORTS(fx_0c2_env)->btlfx_isUnitActive(unit)) {
        fxm_startUnitMotion(unit, BTL_MOTION_HIT_HEAVY);
    }
}

static void fxm_setSignal(Unk800F78A4 *env, s32 signal, s32 value) {
    env->state->signals[signal] = value;
}

INCLUDE_RODATA("asm/us/fx_0c2/nonmatchings/fx_0c2", fx_0c2_headerPtr);
