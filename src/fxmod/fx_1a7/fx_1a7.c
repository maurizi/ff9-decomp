#include "fxmod/fx_1a7.h"

s32 fx_1a7_op80ParticleMotion0(Pool30Entry *e, Pool6CEntry *owner, SVECTOR *pos, s32 trail, s32 frame, s32 frames) {
    return fxm_lineOrbitParticle0(e, pos, fx_1a7_op80Angles[0], fx_1a7_op80Radii[0], frame, frames);
}

s32 fx_1a7_op80ParticleMotion1(Pool30Entry *e, Pool6CEntry *owner, SVECTOR *pos, s32 trail, s32 frame, s32 frames) {
    return fxm_lineOrbitParticle0(e, pos, fx_1a7_op80Angles[1], fx_1a7_op80Radii[1], frame, frames);
}

s32 fx_1a7_op80ParticleMotion2(Pool30Entry *e, Pool6CEntry *owner, SVECTOR *pos, s32 trail, s32 frame, s32 frames) {
    return fxm_lineOrbitParticle0(e, pos, fx_1a7_op80Angles[2], fx_1a7_op80Radii[2], frame, frames);
}

s32 fx_1a7_op80ParticleMotion3(Pool30Entry *e, Pool6CEntry *owner, SVECTOR *pos, s32 trail, s32 frame, s32 frames) {
    return fxm_lineOrbitParticle0(e, pos, fx_1a7_op80Angles[3], fx_1a7_op80Radii[3], frame, frames);
}

s32 fx_1a7_op80ParticleMotion4(Pool30Entry *e, Pool6CEntry *owner, SVECTOR *pos, s32 trail, s32 frame, s32 frames) {
    return fxm_lineOrbitParticle0(e, pos, fx_1a7_op80Angles[4], fx_1a7_op80Radii[4], frame, frames);
}

s32 fx_1a7_op80ParticleMotion5(Pool30Entry *e, Pool6CEntry *owner, SVECTOR *pos, s32 trail, s32 frame, s32 frames) {
    return fxm_lineOrbitParticle0(e, pos, fx_1a7_op80Angles[5], fx_1a7_op80Radii[5], frame, frames);
}

s32 fx_1a7_op80ParticleMotion6(Pool30Entry *e, Pool6CEntry *owner, SVECTOR *pos, s32 trail, s32 frame, s32 frames) {
    return fxm_lineOrbitParticle0(e, pos, fx_1a7_op80Angles[6], fx_1a7_op80Radii[6], frame, frames);
}

s32 fx_1a7_op80ParticleMotion7(Pool30Entry *e, Pool6CEntry *owner, SVECTOR *pos, s32 trail, s32 frame, s32 frames) {
    return fxm_lineOrbitParticle0(e, pos, fx_1a7_op80Angles[7], fx_1a7_op80Radii[7], frame, frames);
}

s32 fx_1a7_op80ParticleMotion8(Pool30Entry *e, Pool6CEntry *owner, SVECTOR *pos, s32 trail, s32 frame, s32 frames) {
    return fxm_lineOrbitParticle0(e, pos, fx_1a7_op80Angles[8], fx_1a7_op80Radii[8], frame, frames);
}

static s32 fxm_lineOrbitParticle0(Pool30Entry *e, SVECTOR *pos, s32 angle, s32 radius, s32 frame, s32 frames) {
    BtlfxExports *ex = BTLFX_EXPORTS(fx_1a7_env);

    ex->btlfx_lerpVec((s16 *)&fx_1a7_op80MotionFrom, (s16 *)&fx_1a7_op80MotionTo, (frame << 12) / 12, (s16 *)pos);
    pos->vx += radius * ex->math_sin(angle) / 4096;
    pos->vy += radius * ex->math_cos(angle) / 4096;
    e->pos.vx = pos->vx << 12;
    e->pos.vy = pos->vy << 12;
    e->pos.vz = pos->vz << 12;
    if (frame >= frames) {
        return -1;
    }
    return 0;
}

s32 fx_1a7_op81ParticleMotion0(Pool30Entry *e, Pool6CEntry *owner, SVECTOR *pos, s32 trail, s32 frame, s32 frames) {
    return fxm_lineOrbitParticle1(e, pos, fx_1a7_op81Angles[0], fx_1a7_op81Radii[0], frame, frames);
}

s32 fx_1a7_op81ParticleMotion1(Pool30Entry *e, Pool6CEntry *owner, SVECTOR *pos, s32 trail, s32 frame, s32 frames) {
    return fxm_lineOrbitParticle1(e, pos, fx_1a7_op81Angles[1], fx_1a7_op81Radii[1], frame, frames);
}

s32 fx_1a7_op81ParticleMotion2(Pool30Entry *e, Pool6CEntry *owner, SVECTOR *pos, s32 trail, s32 frame, s32 frames) {
    return fxm_lineOrbitParticle1(e, pos, fx_1a7_op81Angles[2], fx_1a7_op81Radii[2], frame, frames);
}

s32 fx_1a7_op81ParticleMotion3(Pool30Entry *e, Pool6CEntry *owner, SVECTOR *pos, s32 trail, s32 frame, s32 frames) {
    return fxm_lineOrbitParticle1(e, pos, fx_1a7_op81Angles[3], fx_1a7_op81Radii[3], frame, frames);
}

s32 fx_1a7_op81ParticleMotion4(Pool30Entry *e, Pool6CEntry *owner, SVECTOR *pos, s32 trail, s32 frame, s32 frames) {
    return fxm_lineOrbitParticle1(e, pos, fx_1a7_op81Angles[4], fx_1a7_op81Radii[4], frame, frames);
}

s32 fx_1a7_op81ParticleMotion5(Pool30Entry *e, Pool6CEntry *owner, SVECTOR *pos, s32 trail, s32 frame, s32 frames) {
    return fxm_lineOrbitParticle1(e, pos, fx_1a7_op81Angles[5], fx_1a7_op81Radii[5], frame, frames);
}

s32 fx_1a7_op81ParticleMotion6(Pool30Entry *e, Pool6CEntry *owner, SVECTOR *pos, s32 trail, s32 frame, s32 frames) {
    return fxm_lineOrbitParticle1(e, pos, fx_1a7_op81Angles[6], fx_1a7_op81Radii[6], frame, frames);
}

s32 fx_1a7_op81ParticleMotion7(Pool30Entry *e, Pool6CEntry *owner, SVECTOR *pos, s32 trail, s32 frame, s32 frames) {
    return fxm_lineOrbitParticle1(e, pos, fx_1a7_op81Angles[7], fx_1a7_op81Radii[7], frame, frames);
}

s32 fx_1a7_op81ParticleMotion8(Pool30Entry *e, Pool6CEntry *owner, SVECTOR *pos, s32 trail, s32 frame, s32 frames) {
    return fxm_lineOrbitParticle1(e, pos, fx_1a7_op81Angles[8], fx_1a7_op81Radii[8], frame, frames);
}

static s32 fxm_lineOrbitParticle1(Pool30Entry *e, SVECTOR *pos, s32 angle, s32 radius, s32 frame, s32 frames) {
    BtlfxExports *ex = BTLFX_EXPORTS(fx_1a7_env);

    ex->btlfx_lerpVec((s16 *)&fx_1a7_op81MotionFrom, (s16 *)&fx_1a7_op81MotionTo, (frame << 12) / 12, (s16 *)pos);
    pos->vx += radius * ex->math_sin(angle) / 4096;
    pos->vy += radius * ex->math_cos(angle) / 4096;
    e->pos.vx = pos->vx << 12;
    e->pos.vy = pos->vy << 12;
    e->pos.vz = pos->vz << 12;
    if (frame >= frames) {
        return -1;
    }
    return 0;
}

s32 fx_1a7_op80(s32 mode, Fx1a7Work80 *work, Unk800F78A4 *env, Unk80100C78Entry *entry, s32 *args) {
    BtlfxExports *ex = BTLFX_EXPORTS(env);
    MATRIX boneMtx;
    MATRIX launchMtx;
    s32 frame;
    s32 elapsed;
    s32 i;

    if (mode == 0) {
        *args = 0x3C;
    } else if (mode == 1) {
        fx_1a7_env = env;
        ex->btlfx_getUnitBodyPos(0, 0, (s16 *)&work->pos2C);
        fx_1a7_op80MotionTo.vx = work->pos2C.vx;
        fx_1a7_op80MotionTo.vy = work->pos2C.vy;
        fx_1a7_op80MotionTo.vz = work->pos2C.vz;
        work->emit0 = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, 1), (s32)&fx_1a7_emitPos);
        if (work->emit0 != NULL) {
            work->emit0->trails = (s32)fx_1a7_op80Trails0;
            work->emit0->trailSlots = 0x10;
            work->emit0->trailStride = 0x60;
        }
        work->emit4 = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, 2), (s32)&fx_1a7_emitPos);
        if (work->emit4 != NULL) {
            work->emit4->otZ = ex->btlfx_addRadiusSixth(0x10, 0x80);
        }
    } else {
        frame = entry->frame;
        ex->btlfx_getUnitBoneMatrix(0x10, 0x1C, (BtlBone *)&boneMtx);
        ex->btlfx_matrixPointAhead(&boneMtx, 0x190, (s32)&work->pos34);
        if (frame == 0x32) {
            ex->btlfx_getUnitBoneMatrix(0x10, 0x1C, (BtlBone *)&launchMtx);
            ex->btlfx_matrixPointAhead(&launchMtx, 0xC8, (s32)&fx_1a7_op80MotionFrom);
            fx_1a7_op80MotionFrom.vy -= 0xC8;
        }
        if (frame >= 0x32) {
            elapsed = frame - 0x32;
            for (i = 0; i < 9; i++) {
                if (elapsed == fx_1a7_op80StartFrames[i]) {
                    work->emit8[i] = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, fx_1a7_op80EmitFileIds[i]), (s32)&fx_1a7_emitPos);
                    if (work->emit8[i] != NULL) {
                        work->emit8[i]->trails = (s32)fx_1a7_op80Trails1[i];
                        work->emit8[i]->trailSlots = 1;
                        work->emit8[i]->trailStride = 0x38;
                    }
                    if (work->emit8[i] != NULL) {
                        work->emit8[i]->file->m.module.update = fx_1a7_op80ParticleMotions[i];
                    }
                    fx_1a7_op80Angles[i] = i * 0x155 + ex->btlfx_rand() % 4096;
                    fx_1a7_op80Radii[i] = i * 10;
                } else if (elapsed >= fx_1a7_op80StartFrames[i]) {
                    fx_1a7_op80Angles[i] += 0x200;
                    fx_1a7_op80Radii[i] += (i / 2 + 1) * 8;
                }
            }
        }
        if (frame >= 0 && frame < 0x18) {
            if (work->emit0 != NULL && work->emit0->state == BTLFX_EMIT_DONE) {
                work->emit0 = NULL;
            }
            ex->btlfx_moveEmitterTo((s32)work->emit0, (s32)&work->pos34);
            if (work->emit4 != NULL && work->emit4->state == BTLFX_EMIT_DONE) {
                work->emit4 = NULL;
            }
            ex->btlfx_moveEmitterTo((s32)work->emit4, (s32)&work->pos34);
        }
        return frame >= 0x50;
    }
    return 0;
}

s32 fx_1a7_op81(s32 mode, Fx1a7Work81 *work, Unk800F78A4 *env, Unk80100C78Entry *entry, s32 *args) {
    BtlfxExports *ex = BTLFX_EXPORTS(env);
    s32 frame;
    s32 elapsed;
    s32 i;
    s32 radiusJitter;

    if (mode == 0) {
        *args = 0x64;
    } else if (mode == 1) {
        fx_1a7_env = env;
        ex->btlfx_getUnitBodyPos(0x10, 0, (s16 *)&work->pos34);
        fx_1a7_op81MotionTo.vx = work->pos34.vx;
        fx_1a7_op81MotionTo.vy = work->pos34.vy;
        fx_1a7_op81MotionTo.vz = work->pos34.vz;
        ex->btlfx_getUnitBodyPos(0, 0, (s16 *)&work->pos3C);
        ex->btlfx_getUnitPos(0, (s16 *)&work->pos44);
        fx_1a7_op81MotionFrom.vx = work->pos3C.vx;
        fx_1a7_op81MotionFrom.vy = work->pos3C.vy;
        fx_1a7_op81MotionFrom.vz = work->pos3C.vz;
        work->pos54.vx = work->pos3C.vx;
        work->pos54.vy = work->pos3C.vy;
        work->pos54.vz = work->pos3C.vz;
        work->pos54.vy -= 0xC8;
        work->emit0 = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, 4), (s32)&work->pos54);
        if (work->emit0 != NULL) {
            work->emit0->trails = (s32)fx_1a7_op81Trails0;
            work->emit0->trailSlots = 0x16;
            work->emit0->trailStride = 0x38;
        }
        work->emit4 = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, 5), (s32)&work->pos44);
        ex->btlfx_aimAngles((s16 *)&work->pos44, (s16 *)&work->pos34, (s16 *)&work->angle4C);
        ex->btlfx_pointAhead((s16 *)&work->pos34, (s16 *)&work->angle4C, -0x190, (s16 *)&work->pos5C);
        fx_1a7_op81MotionTo.vx = work->pos5C.vx;
        fx_1a7_op81MotionTo.vy = work->pos5C.vy;
        fx_1a7_op81MotionTo.vz = work->pos5C.vz;
    } else {
        frame = entry->frame;
        if (frame >= 0x14) {
            elapsed = frame - 0x14;
            for (i = 0; i < 9; i++) {
                if (elapsed == fx_1a7_op81StartFrames[i]) {
                    work->emit8[i] = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, fx_1a7_op81EmitFileIds[i]), (s32)&fx_1a7_emitPos);
                    if (work->emit8[i] != NULL) {
                        work->emit8[i]->trails = (s32)fx_1a7_op81Trails1[i];
                        work->emit8[i]->trailSlots = 1;
                        work->emit8[i]->trailStride = 0xB0;
                    }
                    if (work->emit8[i] != NULL) {
                        work->emit8[i]->file->m.module.update = fx_1a7_op81ParticleMotions[i];
                    }
                    fx_1a7_op81Angles[i] = i * 0x155 + ex->btlfx_rand() % 4096;
                    fx_1a7_op81Radii[i] = ex->btlfx_rand() % 200 + 0x64;
                } else if (elapsed >= fx_1a7_op81StartFrames[i]) {
                    fx_1a7_op81Angles[i] += 0x80;
                    radiusJitter = 0x78;
                    if (elapsed - fx_1a7_op81StartFrames[i] >= 5) {
                        radiusJitter = 0x28;
                    }
                    if (ex->btlfx_rand() & 1) {
                        fx_1a7_op81Radii[i] += ex->btlfx_rand() % radiusJitter;
                    } else {
                        fx_1a7_op81Radii[i] -= ex->btlfx_rand() % radiusJitter;
                    }
                }
            }
        }
        if (frame == 0x1C) {
            work->emit2C = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, 7), (s32)&work->pos34);
            if (work->emit2C != NULL) {
                work->emit2C->trails = (s32)fx_1a7_op81Trails2;
                work->emit2C->trailSlots = 0xD;
                work->emit2C->trailStride = 0xB0;
            }
            work->emit30 = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, 0x18), (s32)&work->pos5C);
        }
        return frame >= 0x28;
    }
    return 0;
}

INCLUDE_RODATA("asm/us/fx_1a7/nonmatchings/fx_1a7", fx_1a7_headerPtr);
