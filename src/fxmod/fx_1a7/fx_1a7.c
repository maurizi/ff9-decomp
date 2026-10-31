#include "fxmod/fx_1a7.h"

s32 fx_1a7_op80ParticleMotion0(Pool30Entry *e, Pool6CEntry *owner, SVECTOR *pos, s32 arg3, s32 frame, s32 frames) {
    return fxm_lineOrbitParticle0(e, pos, D_801E8950[0], D_801E8990[0], frame, frames);
}

s32 fx_1a7_op80ParticleMotion1(Pool30Entry *e, Pool6CEntry *owner, SVECTOR *pos, s32 arg3, s32 frame, s32 frames) {
    return fxm_lineOrbitParticle0(e, pos, D_801E8950[1], D_801E8990[1], frame, frames);
}

s32 fx_1a7_op80ParticleMotion2(Pool30Entry *e, Pool6CEntry *owner, SVECTOR *pos, s32 arg3, s32 frame, s32 frames) {
    return fxm_lineOrbitParticle0(e, pos, D_801E8950[2], D_801E8990[2], frame, frames);
}

s32 fx_1a7_op80ParticleMotion3(Pool30Entry *e, Pool6CEntry *owner, SVECTOR *pos, s32 arg3, s32 frame, s32 frames) {
    return fxm_lineOrbitParticle0(e, pos, D_801E8950[3], D_801E8990[3], frame, frames);
}

s32 fx_1a7_op80ParticleMotion4(Pool30Entry *e, Pool6CEntry *owner, SVECTOR *pos, s32 arg3, s32 frame, s32 frames) {
    return fxm_lineOrbitParticle0(e, pos, D_801E8950[4], D_801E8990[4], frame, frames);
}

s32 fx_1a7_op80ParticleMotion5(Pool30Entry *e, Pool6CEntry *owner, SVECTOR *pos, s32 arg3, s32 frame, s32 frames) {
    return fxm_lineOrbitParticle0(e, pos, D_801E8950[5], D_801E8990[5], frame, frames);
}

s32 fx_1a7_op80ParticleMotion6(Pool30Entry *e, Pool6CEntry *owner, SVECTOR *pos, s32 arg3, s32 frame, s32 frames) {
    return fxm_lineOrbitParticle0(e, pos, D_801E8950[6], D_801E8990[6], frame, frames);
}

s32 fx_1a7_op80ParticleMotion7(Pool30Entry *e, Pool6CEntry *owner, SVECTOR *pos, s32 arg3, s32 frame, s32 frames) {
    return fxm_lineOrbitParticle0(e, pos, D_801E8950[7], D_801E8990[7], frame, frames);
}

s32 fx_1a7_op80ParticleMotion8(Pool30Entry *e, Pool6CEntry *owner, SVECTOR *pos, s32 arg3, s32 frame, s32 frames) {
    return fxm_lineOrbitParticle0(e, pos, D_801E8950[8], D_801E8990[8], frame, frames);
}

static s32 fxm_lineOrbitParticle0(Pool30Entry *e, SVECTOR *pos, s32 angle, s32 radius, s32 frame, s32 frames) {
    BtlfxExports *ex = BTLFX_EXPORTS(fx_1a7_env);

    ex->btlfx_lerpVec((s16 *)&D_801EA5C0, (s16 *)&D_801EA5D0, (frame << 12) / 12, (s16 *)pos);
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

s32 fx_1a7_op81ParticleMotion0(Pool30Entry *e, Pool6CEntry *owner, SVECTOR *pos, s32 arg3, s32 frame, s32 frames) {
    return fxm_lineOrbitParticle1(e, pos, D_801E8970[0], D_801E89B0[0], frame, frames);
}

s32 fx_1a7_op81ParticleMotion1(Pool30Entry *e, Pool6CEntry *owner, SVECTOR *pos, s32 arg3, s32 frame, s32 frames) {
    return fxm_lineOrbitParticle1(e, pos, D_801E8970[1], D_801E89B0[1], frame, frames);
}

s32 fx_1a7_op81ParticleMotion2(Pool30Entry *e, Pool6CEntry *owner, SVECTOR *pos, s32 arg3, s32 frame, s32 frames) {
    return fxm_lineOrbitParticle1(e, pos, D_801E8970[2], D_801E89B0[2], frame, frames);
}

s32 fx_1a7_op81ParticleMotion3(Pool30Entry *e, Pool6CEntry *owner, SVECTOR *pos, s32 arg3, s32 frame, s32 frames) {
    return fxm_lineOrbitParticle1(e, pos, D_801E8970[3], D_801E89B0[3], frame, frames);
}

s32 fx_1a7_op81ParticleMotion4(Pool30Entry *e, Pool6CEntry *owner, SVECTOR *pos, s32 arg3, s32 frame, s32 frames) {
    return fxm_lineOrbitParticle1(e, pos, D_801E8970[4], D_801E89B0[4], frame, frames);
}

s32 fx_1a7_op81ParticleMotion5(Pool30Entry *e, Pool6CEntry *owner, SVECTOR *pos, s32 arg3, s32 frame, s32 frames) {
    return fxm_lineOrbitParticle1(e, pos, D_801E8970[5], D_801E89B0[5], frame, frames);
}

s32 fx_1a7_op81ParticleMotion6(Pool30Entry *e, Pool6CEntry *owner, SVECTOR *pos, s32 arg3, s32 frame, s32 frames) {
    return fxm_lineOrbitParticle1(e, pos, D_801E8970[6], D_801E89B0[6], frame, frames);
}

s32 fx_1a7_op81ParticleMotion7(Pool30Entry *e, Pool6CEntry *owner, SVECTOR *pos, s32 arg3, s32 frame, s32 frames) {
    return fxm_lineOrbitParticle1(e, pos, D_801E8970[7], D_801E89B0[7], frame, frames);
}

s32 fx_1a7_op81ParticleMotion8(Pool30Entry *e, Pool6CEntry *owner, SVECTOR *pos, s32 arg3, s32 frame, s32 frames) {
    return fxm_lineOrbitParticle1(e, pos, D_801E8970[8], D_801E89B0[8], frame, frames);
}

static s32 fxm_lineOrbitParticle1(Pool30Entry *e, SVECTOR *pos, s32 angle, s32 radius, s32 frame, s32 frames) {
    BtlfxExports *ex = BTLFX_EXPORTS(fx_1a7_env);

    ex->btlfx_lerpVec((s16 *)&D_801EA5C8, (s16 *)&D_801EA5D8, (frame << 12) / 12, (s16 *)pos);
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

s32 fx_1a7_op80(s32 mode, Fx1a7Work80 *work, Unk800F78A4 *env, Unk80100C78Entry *entry, s32 *arg4) {
    BtlfxExports *ex = BTLFX_EXPORTS(env);
    MATRIX m;
    MATRIX m2;
    s32 frame;
    s32 t;
    s32 i;

    if (mode == 0) {
        *arg4 = 0x3C;
    } else if (mode == 1) {
        fx_1a7_env = env;
        ex->btlfx_getUnitBodyPos(0, 0, (s16 *)&work->pos2C);
        D_801EA5D0.vx = work->pos2C.vx;
        D_801EA5D0.vy = work->pos2C.vy;
        D_801EA5D0.vz = work->pos2C.vz;
        work->emit0 = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, 1), (s32)&D_801EA5E0);
        if (work->emit0 != NULL) {
            work->emit0->trails = (s32)D_801E89D0;
            work->emit0->trailSlots = 0x10;
            work->emit0->trailStride = 0x60;
        }
        work->emit4 = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, 2), (s32)&D_801EA5E0);
        if (work->emit4 != NULL) {
            work->emit4->otZ = ex->btlfx_addRadiusSixth(0x10, 0x80);
        }
    } else {
        frame = entry->frame;
        ex->btlfx_getUnitBoneMatrix(0x10, 0x1C, (BtlBone *)&m);
        ex->btlfx_matrixPointAhead(&m, 0x190, (s32)&work->unk34);
        if (frame == 0x32) {
            ex->btlfx_getUnitBoneMatrix(0x10, 0x1C, (BtlBone *)&m2);
            ex->btlfx_matrixPointAhead(&m2, 0xC8, (s32)&D_801EA5C0);
            D_801EA5C0.vy -= 0xC8;
        }
        if (frame >= 0x32) {
            t = frame - 0x32;
            for (i = 0; i < 9; i++) {
                if (t == D_801E8874[i]) {
                    work->emit8[i] = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, D_801E8860[i]), (s32)&D_801EA5E0);
                    if (work->emit8[i] != NULL) {
                        work->emit8[i]->trails = (s32)D_801E8FD0[i];
                        work->emit8[i]->trailSlots = 1;
                        work->emit8[i]->trailStride = 0x38;
                    }
                    if (work->emit8[i] != NULL) {
                        work->emit8[i]->file->m.module.update = fx_1a7_op80ParticleMotions[i];
                    }
                    D_801E8950[i] = i * 0x155 + ex->btlfx_rand() % 4096;
                    D_801E8990[i] = i * 10;
                } else if (t >= D_801E8874[i]) {
                    D_801E8950[i] += 0x200;
                    D_801E8990[i] += (i / 2 + 1) * 8;
                }
            }
        }
        if (frame >= 0 && frame < 0x18) {
            if (work->emit0 != NULL && work->emit0->state == BTLFX_EMIT_DONE) {
                work->emit0 = NULL;
            }
            ex->btlfx_moveEmitterTo((s32)work->emit0, (s32)&work->unk34);
            if (work->emit4 != NULL && work->emit4->state == BTLFX_EMIT_DONE) {
                work->emit4 = NULL;
            }
            ex->btlfx_moveEmitterTo((s32)work->emit4, (s32)&work->unk34);
        }
        return frame >= 0x50;
    }
    return 0;
}

s32 fx_1a7_op81(s32 mode, Fx1a7Work81 *work, Unk800F78A4 *env, Unk80100C78Entry *entry, s32 *arg4) {
    BtlfxExports *ex = BTLFX_EXPORTS(env);
    s32 frame;
    s32 t;
    s32 i;
    s32 k;

    if (mode == 0) {
        *arg4 = 0x64;
    } else if (mode == 1) {
        fx_1a7_env = env;
        ex->btlfx_getUnitBodyPos(0x10, 0, (s16 *)&work->pos34);
        D_801EA5D8.vx = work->pos34.vx;
        D_801EA5D8.vy = work->pos34.vy;
        D_801EA5D8.vz = work->pos34.vz;
        ex->btlfx_getUnitBodyPos(0, 0, (s16 *)&work->pos3C);
        ex->btlfx_getUnitPos(0, (s16 *)&work->pos44);
        D_801EA5C8.vx = work->pos3C.vx;
        D_801EA5C8.vy = work->pos3C.vy;
        D_801EA5C8.vz = work->pos3C.vz;
        work->pos54.vx = work->pos3C.vx;
        work->pos54.vy = work->pos3C.vy;
        work->pos54.vz = work->pos3C.vz;
        work->pos54.vy -= 0xC8;
        work->emit0 = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, 4), (s32)&work->pos54);
        if (work->emit0 != NULL) {
            work->emit0->trails = (s32)D_801E91D0;
            work->emit0->trailSlots = 0x16;
            work->emit0->trailStride = 0x38;
        }
        work->emit4 = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, 5), (s32)&work->pos44);
        ex->btlfx_aimAngles((s16 *)&work->pos44, (s16 *)&work->pos34, (s16 *)&work->unk4C);
        ex->btlfx_pointAhead((s16 *)&work->pos34, (s16 *)&work->unk4C, -0x190, (s16 *)&work->pos5C);
        D_801EA5D8.vx = work->pos5C.vx;
        D_801EA5D8.vy = work->pos5C.vy;
        D_801EA5D8.vz = work->pos5C.vz;
    } else {
        frame = entry->frame;
        if (frame >= 0x14) {
            t = frame - 0x14;
            for (i = 0; i < 9; i++) {
                if (t == D_801E889C[i]) {
                    work->emit8[i] = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, D_801E8888[i]), (s32)&D_801EA5E0);
                    if (work->emit8[i] != NULL) {
                        work->emit8[i]->trails = (s32)D_801E96A0[i];
                        work->emit8[i]->trailSlots = 1;
                        work->emit8[i]->trailStride = 0xB0;
                    }
                    if (work->emit8[i] != NULL) {
                        work->emit8[i]->file->m.module.update = fx_1a7_op81ParticleMotions[i];
                    }
                    D_801E8970[i] = i * 0x155 + ex->btlfx_rand() % 4096;
                    D_801E89B0[i] = ex->btlfx_rand() % 200 + 0x64;
                } else if (t >= D_801E889C[i]) {
                    D_801E8970[i] += 0x80;
                    k = 0x78;
                    if (t - D_801E889C[i] >= 5) {
                        k = 0x28;
                    }
                    if (ex->btlfx_rand() & 1) {
                        D_801E89B0[i] += ex->btlfx_rand() % k;
                    } else {
                        D_801E89B0[i] -= ex->btlfx_rand() % k;
                    }
                }
            }
        }
        if (frame == 0x1C) {
            work->emit2C = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, 7), (s32)&work->pos34);
            if (work->emit2C != NULL) {
                work->emit2C->trails = (s32)D_801E9CD0;
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
