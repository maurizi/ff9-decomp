#include "fxmod/fx_127.h"

static s32 fx_127_op80ParticleMotion0(VECTOR *out, s32 owner, SVECTOR *pos, s32 trail, s32 frame, s32 n) {
    BtlfxExports *ex = BTLFX_EXPORTS(fx_127_env);
    s32 status = fxm_lerpParticle(out, pos, &fx_127_lerpFrom, &fx_127_lerpTo, 5, frame, n);

    fx_127_emitterPos.vx = pos->vx;
    fx_127_emitterPos.vy = 0;
    fx_127_emitterPos.vz = pos->vz;
    if (fx_127_emitter != NULL) {
        if (fx_127_emitter->state == BTLFX_EMIT_DONE) {
            fx_127_emitter = NULL;
        }
    }
    ex->btlfx_moveEmitterTo((s32)fx_127_emitter, (s32)&fx_127_emitterPos);
    return status;
}

static s32 fx_127_op80ParticleMotion1(VECTOR *out, s32 owner, SVECTOR *pos, s32 trail, s32 frame, s32 n) {
    return fxm_lerpParticle(out, pos, &fx_127_lerpFrom, &fx_127_lerpTo, 5, frame, n);
}

static s32 fxm_lerpParticle(VECTOR *out, SVECTOR *pos, SVECTOR *from, SVECTOR *to, s32 len, s32 frame, s32 n) {
    BTLFX_EXPORTS(fx_127_env)->btlfx_lerpVec((s16 *)from, (s16 *)to, (frame << 12) / len, (s16 *)pos);
    out->vx = pos->vx << 12;
    out->vy = pos->vy << 12;
    out->vz = pos->vz << 12;
    if (frame >= n) {
        return -1;
    }
    return 0;
}

s32 fx_127_op80(s32 mode, Fx127Work80 *work, Unk800F78A4 *env, Unk80100C78Entry *entry, s32 *args) {
    BtlfxExports *ex = BTLFX_EXPORTS(env);
    MATRIX boneMat;
    MATRIX releaseMat;
    MATRIX fromMat;
    s32 frame;

    if (mode == 0) {
        *args = 0x84;
    } else if (mode == 1) {
        fx_127_env = env;
        ex->btlfx_getUnitPos(0x10, (s16 *)&work->pos24);
        ex->btlfx_getUnitRot(0x10, (s16 *)&work->rot2C);
        ex->btlfx_getUnitBodyPos(0, 0, (s16 *)&work->pos34);
        ex->btlfx_getUnitPos(0, (s16 *)&work->pos3C);
    } else {
        frame = entry->frame;
        ex->btlfx_getUnitBoneMatrix(0x10, 0x3C, (BtlBone *)&boneMat);
        ex->btlfx_matrixPointAhead(&boneMat, 0, (s32)&work->pos44);
        ex->btlfx_aimAngles((s16 *)&work->pos44, (s16 *)&work->pos34, (s16 *)&work->rot74);
        switch (frame) {
        case 0xA:
            work->emit0 = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, FX_WAVE_WHITE_RINGS), (s32)&fx_127_emitPos0);
            work->emit4 = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, FX_WAVE_FAN_EMITTER), (s32)&fx_127_emitPos0);
            work->emit8 = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, FX_WAVE_FAN_EMITTER_2), (s32)&fx_127_emitPos0);
            work->pos4C.vx = work->pos44.vx;
            work->pos4C.vy = work->pos44.vy;
            work->pos4C.vz = work->pos44.vz;
            work->pos4C.vy = 0;
            work->emitC = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, FX_WAVE_WHITE_RINGS_2), (s32)&work->pos4C);
            if (work->emit0 != NULL) {
                work->emit0->otZ = ex->btlfx_addRadiusSixth(0x10, 0x20);
            }
            if (work->emit4 != NULL) {
                work->emit4->otZ = ex->btlfx_addRadiusSixth(0x10, 0x80);
            }
            break;
        case 0x22:
            ex->btlfx_aimAngles((s16 *)&work->pos34, (s16 *)&work->pos44, (s16 *)&work->rot54);
            work->rot5C.vx = work->rot54.vx;
            work->rot5C.vy = work->rot54.vy;
            work->rot5C.vz = work->rot54.vz;
            work->rot5C.vy += 0x800;
            fx_127_aimRot.vx = work->rot5C.vx;
            fx_127_aimRot.vy = work->rot5C.vy;
            fx_127_aimRot.vz = work->rot5C.vz;
            ex->btlfx_getUnitBoneMatrix(0x10, 0x3C, (BtlBone *)&releaseMat);
            ex->btlfx_matrixPointAhead(&releaseMat, 0, (s32)&work->pos64);
            ex->btlfx_getUnitBoneMatrix(0x10, 0x3C, (BtlBone *)&fromMat);
            ex->btlfx_matrixPointAhead(&fromMat, 0, (s32)&fx_127_lerpFrom);
            fx_127_lerpTo.vx = work->pos34.vx;
            fx_127_lerpTo.vy = work->pos34.vy;
            fx_127_lerpTo.vz = work->pos34.vz;
            work->emit10 = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, FX_WAVE_STRIPED_RINGS), (s32)&fx_127_emitPos1);
            work->emit14 = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, FX_WAVE_WATER_TRAIL), (s32)&fx_127_emitPos2);
            fx_127_emitter = work->emit18 = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, FX_WAVE_STRIPED_RINGS_2), (s32)&fx_127_emitPos3);
            if (work->emit10 != NULL) {
                work->emit10->trails = (s32)fx_127_op80Trails0;
                work->emit10->trailSlots = 0x112;
                work->emit10->trailStride = 0x18;
            }
            if (work->emit14 != NULL) {
                work->emit14->trails = (s32)fx_127_op80Trails1;
                work->emit14->trailSlots = 0x28;
                work->emit14->trailStride = 0x40;
            }
            if (work->emit10 != NULL) {
                ((Fx127EffHdr *)work->emit10->file)->motionFn8 = fx_127_op80ParticleMotion0;
            }
            if (work->emit14 != NULL) {
                ((Fx127EffHdr *)work->emit14->file)->motionFn8 = fx_127_op80ParticleMotion1;
            }
            if (work->emit10 != NULL) {
                work->emit10->otZ = ex->btlfx_addRadiusSixth(0, 0x20);
            }
            if (work->emit14 != NULL) {
                work->emit14->otZ = ex->btlfx_addRadiusSixth(0, 0x20);
            }
            break;
        case 0x27:
            ex->btlfx_pointAhead((s16 *)&work->pos34, (s16 *)&work->rot5C, -0xC8, (s16 *)&work->pos6C);
            fx_127_aimPoint.vx = work->pos6C.vx;
            fx_127_aimPoint.vy = work->pos6C.vy;
            fx_127_aimPoint.vz = work->pos6C.vz;
            break;
        }
        if (frame >= 0xA && frame < 0xE) {
            if (work->emit0 != NULL && work->emit0->state == BTLFX_EMIT_DONE) {
                work->emit0 = NULL;
            }
            ex->btlfx_moveEmitterTo((s32)work->emit0, (s32)&work->pos44);
            if (work->emit4 != NULL && work->emit4->state == BTLFX_EMIT_DONE) {
                work->emit4 = NULL;
            }
            ex->btlfx_moveEmitterTo((s32)work->emit4, (s32)&work->pos44);
            if (work->emit8 != NULL && work->emit8->state == BTLFX_EMIT_DONE) {
                work->emit8 = NULL;
            }
            ex->btlfx_moveEmitterTo((s32)work->emit8, (s32)&work->pos44);
            ex->btlfx_aimAngles((s16 *)&work->pos44, (s16 *)&work->pos34, (s16 *)&work->rot74);
            work->rot7C.vx = work->rot74.vx;
            work->rot7C.vy = work->rot74.vy;
            work->rot7C.vz = work->rot74.vz;
            work->rot7C.vy += 0x800;
        }
        if (frame >= 0xA) {
            if (work->emit0 != NULL) {
                if (work->emit0->state == BTLFX_EMIT_DONE) {
                    work->emit0 = NULL;
                }
                if (work->emit0 != NULL) {
                    work->emit0->rot.vx = work->rot74.vx;
                    work->emit0->rot.vy = work->rot74.vy;
                    work->emit0->rot.vz = work->rot74.vz;
                }
            }
            if (work->emit4 != NULL) {
                if (work->emit4->state == BTLFX_EMIT_DONE) {
                    work->emit4 = NULL;
                }
                if (work->emit4 != NULL) {
                    work->emit4->rot.vx = work->rot74.vx;
                    work->emit4->rot.vy = work->rot74.vy;
                    work->emit4->rot.vz = work->rot74.vz;
                }
            }
            if (work->emit8 != NULL) {
                if (work->emit8->state == BTLFX_EMIT_DONE) {
                    work->emit8 = NULL;
                }
                if (work->emit8 != NULL) {
                    work->emit8->rot.vx = work->rot7C.vx;
                    work->emit8->rot.vy = work->rot7C.vy;
                    work->emit8->rot.vz = work->rot7C.vz;
                }
            }
        }
        return frame >= 0x50;
    }
    return 0;
}

s32 fx_127_op81(s32 mode, Fx127Work81 *work, Unk800F78A4 *env, Unk80100C78Entry *entry, s32 *args) {
    BtlfxExports *ex = BTLFX_EXPORTS(env);
    s32 frame;

    if (mode == 0) {
        *args = 0x40;
    } else if (mode == 1) {
        fx_127_env = env;
        ex->btlfx_getUnitBodyPos(0, 0, (s16 *)&work->pos10);
        work->rot18.vx = fx_127_aimRot.vx;
        work->rot18.vy = fx_127_aimRot.vy;
        work->rot18.vz = fx_127_aimRot.vz;
        work->rot20.vx = fx_127_aimRot.vx;
        work->rot20.vy = fx_127_aimRot.vy;
        work->rot20.vz = fx_127_aimRot.vz;
        work->pos30.vx = work->pos10.vx;
        work->pos30.vy = work->pos10.vy;
        work->pos30.vz = work->pos10.vz;
        work->rot20.vy += 0x800;
        work->pos28.vx = fx_127_aimPoint.vx;
        work->pos28.vy = fx_127_aimPoint.vy;
        work->pos28.vz = fx_127_aimPoint.vz;
        work->pos38.vx = work->pos10.vx;
        work->pos38.vy = work->pos10.vy;
        work->pos38.vz = work->pos10.vz;
        work->pos38.vy = 0;
    } else {
        frame = entry->frame;
        switch (frame) {
        case 0:
            work->emit0 = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, FX_WAVE_WHITE_RINGS_3), (s32)&work->pos30);
            work->emit4 = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, FX_WAVE_WHITE_RINGS_4), (s32)&work->pos38);
            if (work->emit0 != NULL) {
                work->emit0->otZ = ex->btlfx_addRadiusSixth(0, 0x80);
            }
            break;
        case 4:
            work->emitC = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, FX_WAVE_WHITE_RING), (s32)&work->pos38);
            break;
        }
        if (frame >= 0) {
            if (work->emit0 != NULL) {
                if (work->emit0->state == BTLFX_EMIT_DONE) {
                    work->emit0 = NULL;
                }
                if (work->emit0 != NULL) {
                    work->emit0->rot.vx = work->rot18.vx;
                    work->emit0->rot.vy = work->rot18.vy;
                    work->emit0->rot.vz = work->rot18.vz;
                }
            }
        }
        return frame >= 0x50;
    }
    return 0;
}

INCLUDE_RODATA("asm/us/fx_127/nonmatchings/fx_127", fx_127_headerPtr);
