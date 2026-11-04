#include "fxmod/fx_1ac.h"

static s32 fxm_easeLerp(s32 curve, s32 n, s32 t, s32 from, s32 to) {
    BtlfxExports *ex = BTLFX_EXPORTS(fx_1ac_env);
    s32 invSq = 0x20000000 / (n * n);
    s32 ratio = 0;

    switch (curve) {
    case 0:
        ratio = 0x1000 - (((0x10000000 / n) * t) >> 16);
        break;
    case 1:
        ratio = (invSq * (n - t) * (n - t)) >> 17;
        break;
    case 2:
        ratio = 0x1000 - ((invSq * t * t) >> 17);
        break;
    }
    return ex->btlfx_lerp(from, to, ratio);
}

INCLUDE_RODATA("asm/us/fx_1ac/nonmatchings/fx_1ac", fx_1ac_headerPtr);

s32 fx_1ac_op80(s32 mode, Fx1acWork *work, Unk800F78A4 *env, Unk80100C78Entry *entry, s32 *args) {
    BtlfxExports *ex = BTLFX_EXPORTS(env);
    SVECTOR unitPos;
    SVECTOR boneAng;
    Pool6CEntry *emitter;
    s32 frame;
    s32 i;
    s32 jumpFrame;

    if (mode == 0) {
        *args = 0x60;
    } else if (mode == 1) {
        fx_1ac_env = env;
        for (i = 7; i >= 0; i--) {
            work->timer0[i] = 0;
        }
        ex->btlfx_getUnitRot(0, (s16 *)&work->rot38);
        ex->btlfx_getUnitRot(0, (s16 *)&work->rot44);
        work->emit4C[0] = NULL;
        work->emit4C[1] = NULL;
        work->emit4C[2] = NULL;
        work->emit4C[3] = NULL;
        work->emit4C[4] = NULL;
    } else {
        frame = entry->frame;
        if (work->emit4C[0] != NULL && work->emit4C[0]->state == BTLFX_EMIT_DONE) {
            work->emit4C[0] = NULL;
        }
        if (work->emit4C[1] != NULL && work->emit4C[1]->state == BTLFX_EMIT_DONE) {
            work->emit4C[1] = NULL;
        }
        if (work->emit4C[2] != NULL && work->emit4C[2]->state == BTLFX_EMIT_DONE) {
            work->emit4C[2] = NULL;
        }
        if (work->emit4C[3] != NULL && work->emit4C[3]->state == BTLFX_EMIT_DONE) {
            work->emit4C[3] = NULL;
        }
        if (work->emit4C[4] != NULL && work->emit4C[4]->state == BTLFX_EMIT_DONE) {
            work->emit4C[4] = NULL;
        }
        ex->btlfx_getUnitBonePos(0x10, 4, (u16 *)&work->pos30);
        switch (frame) {
        case 0:
            ex->btlfx_getUnitPos(0x10, (s16 *)&work->pos20);
            ex->btlfx_getUnitPos(0, (s16 *)&work->pos28);
            fx_1ac_emitter = emitter = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, FX_VIRUS_CRUNCH_GREY_TRAIL), (s32)&work->pos28);
            if (emitter != NULL) {
                emitter->trails = (s32)fx_1ac_op80Trails;
                emitter->trailSlots = 4;
                emitter->trailStride = 0x9C;
                emitter->file->m.module.update = fx_1ac_op80ParticleMotion;
            }
            work->timer0[0] = 0xE;
            break;
        case 14:
            ex->btlfx_fadeUnitById(0, 0);
            ex->btlfx_getUnitHomePos(0, (s16 *)&unitPos);
            ex->btlfx_setUnitPos(0, (s16 *)&unitPos);
            break;
        case 20:
            ex->btlfx_getUnitBonePos(0x10, 4, (u16 *)&fx_1ac_emitPos0);
            work->emit4C[0] = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, FX_VIRUS_CRUNCH_MOTHS), (s32)&fx_1ac_emitPos0);
            work->timer0[3] = 0x10;
            break;
        case 26:
            ex->btlfx_getUnitBonePos(0x10, 4, (u16 *)&fx_1ac_emitPos1);
            work->emit4C[1] = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, FX_VIRUS_CRUNCH_MOTHS), (s32)&fx_1ac_emitPos1);
            work->timer0[4] = 0x10;
            break;
        case 32:
            ex->btlfx_getUnitBonePos(0x10, 4, (u16 *)&fx_1ac_emitPos2);
            work->emit4C[2] = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, FX_VIRUS_CRUNCH_MOTHS), (s32)&fx_1ac_emitPos2);
            work->timer0[5] = 0x10;
            break;
        case 46:
            ex->btlfx_getUnitBonePos(0x10, 4, (u16 *)&fx_1ac_emitPos3);
            work->emit4C[3] = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, FX_VIRUS_CRUNCH_MOTHS_2), (s32)&fx_1ac_emitPos3);
            work->timer0[6] = 0x10;
            break;
        case 50:
            work->timer0[2] = 2;
            ex->btlfx_fadeUnitById(0, 0x80);
            ex->btlfx_setUnitRot(0, (s16 *)&work->rot44);
            ex->btlfx_getUnitHomePos(0, (s16 *)&fx_1ac_emitPos4);
            work->emit4C[4] = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, FX_VIRUS_CRUNCH_CYAN_CLOUD), (s32)&fx_1ac_emitPos4);
            work->timer0[7] = 0x10;
            break;
        }
        if (work->timer0[0] != 0) {
            work->timer0[0]--;
            jumpFrame = 0xE - work->timer0[0];
            fx_1ac_targetPos.vx = ex->btlfx_lerpRatio(work->pos28.vx, work->pos30.vx, jumpFrame, 0xE);
            fx_1ac_targetPos.vz = ex->btlfx_lerpRatio(work->pos28.vz, work->pos30.vz, jumpFrame, 0xE);
            fx_1ac_targetPos.vy = work->pos28.vy - (jumpFrame * 0x258 - (jumpFrame * 0x25) * jumpFrame);
            ex->btlfx_setUnitPos(0, (s16 *)&fx_1ac_targetPos);
            work->rot38.vx -= 0xAA;
            ex->btlfx_setUnitRot(0, (s16 *)&work->rot38);
        }
        if (work->timer0[3] != 0) {
            MATRIX boneMtx;

            work->timer0[3]--;
            ex->btlfx_getUnitBoneMatrix(0x10, 4, (BtlBone *)&boneMtx);
            ex->btlfx_matrixAngles(&boneMtx, (s16 *)&boneAng);
            if (work->emit4C[0] != NULL) {
                work->emit4C[0]->rot.vx = boneAng.vx;
                work->emit4C[0]->rot.vy = boneAng.vy;
                work->emit4C[0]->rot.vz = boneAng.vz;
            }
            ex->btlfx_getUnitBonePos(0x10, 4, (u16 *)&unitPos);
            ex->btlfx_moveEmitterTo((s32)work->emit4C[0], (s32)&unitPos);
        }
        if (work->timer0[4] != 0) {
            MATRIX boneMtx;

            work->timer0[4]--;
            ex->btlfx_getUnitBoneMatrix(0x10, 4, (BtlBone *)&boneMtx);
            ex->btlfx_matrixAngles(&boneMtx, (s16 *)&boneAng);
            if (work->emit4C[1] != NULL) {
                work->emit4C[1]->rot.vx = boneAng.vx;
                work->emit4C[1]->rot.vy = boneAng.vy;
                work->emit4C[1]->rot.vz = boneAng.vz;
            }
            ex->btlfx_getUnitBonePos(0x10, 4, (u16 *)&unitPos);
            ex->btlfx_moveEmitterTo((s32)work->emit4C[1], (s32)&unitPos);
        }
        if (work->timer0[5] != 0) {
            MATRIX boneMtx;

            work->timer0[5]--;
            ex->btlfx_getUnitBoneMatrix(0x10, 4, (BtlBone *)&boneMtx);
            ex->btlfx_matrixAngles(&boneMtx, (s16 *)&boneAng);
            if (work->emit4C[2] != NULL) {
                work->emit4C[2]->rot.vx = boneAng.vx;
                work->emit4C[2]->rot.vy = boneAng.vy;
                work->emit4C[2]->rot.vz = boneAng.vz;
            }
            ex->btlfx_getUnitBonePos(0x10, 4, (u16 *)&unitPos);
            ex->btlfx_moveEmitterTo((s32)work->emit4C[2], (s32)&unitPos);
        }
        if (work->timer0[6] != 0) {
            MATRIX boneMtx;

            work->timer0[6]--;
            ex->btlfx_getUnitBoneMatrix(0x10, 4, (BtlBone *)&boneMtx);
            ex->btlfx_matrixAngles(&boneMtx, (s16 *)&boneAng);
            if (work->emit4C[3] != NULL) {
                work->emit4C[3]->rot.vx = boneAng.vx;
                work->emit4C[3]->rot.vy = boneAng.vy;
                work->emit4C[3]->rot.vz = boneAng.vz;
            }
            ex->btlfx_getUnitBonePos(0x10, 4, (u16 *)&unitPos);
            ex->btlfx_moveEmitterTo((s32)work->emit4C[3], (s32)&unitPos);
        }
        if (work->timer0[7] != 0) {
            work->timer0[7]--;
            boneAng.vx = 0xC00;
            boneAng.vz = 0;
            boneAng.vy = 0;
            if (work->emit4C[4] != NULL) {
                work->emit4C[4]->rot.vx = boneAng.vx;
                work->emit4C[4]->rot.vy = boneAng.vy;
                work->emit4C[4]->rot.vz = boneAng.vz;
            }
        }
        if (work->timer0[2] != 0) {
            work->timer0[2]--;
            if (work->timer0[2] != 0) {
                ex->btlfx_lerpVec((s16 *)&work->pos28, (s16 *)&work->pos20, (work->timer0[2] << 23) >> 12, (s16 *)&unitPos);
            } else {
                ex->btlfx_getUnitHomePos(0, (s16 *)&unitPos);
            }
            ex->btlfx_setUnitPos(0, (s16 *)&unitPos);
        }
        return frame >= 0x46;
    }
    return 0;
}

static s32 fx_1ac_op80ParticleMotion(Pool30Entry *pt, Pool6CEntry *owner, SVECTOR *pos, s32 trail, s32 frame, s32 n) {
    SVECTOR followPos;

    switch (pt->slot - 1) {
    case 0: {
        SVECTOR *src = &fx_1ac_targetPos;
        s16 x = src->vx;
        s16 y = src->vy;
        s16 z = src->vz;

        /* both cases copy through their own locals; jump2's cross-jumping merges the identical tails */
        followPos.vx = x;
        followPos.vy = y;
        followPos.vz = z;
        break;
    }
    default: {
        SVECTOR *src = &fx_1ac_lastParticlePos;
        s16 x = src->vx;
        s16 y = src->vy;
        s16 z = src->vz;

        followPos.vx = x;
        followPos.vy = y;
        followPos.vz = z;
        break;
    }
    }
    fx_1ac_lastParticlePos.vx = pt->pos.vx >> 12;
    fx_1ac_lastParticlePos.vy = pt->pos.vy >> 12;
    fx_1ac_lastParticlePos.vz = pt->pos.vz >> 12;
    pos->vx = followPos.vx;
    pos->vy = followPos.vy;
    pos->vz = followPos.vz;
    pt->pos.vx = followPos.vx << 12;
    pt->pos.vy = followPos.vy << 12;
    pt->pos.vz = followPos.vz << 12;
    if (frame >= n) {
        return -1;
    }
    return 0;
}
