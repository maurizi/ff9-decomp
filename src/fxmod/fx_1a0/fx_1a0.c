#include "fxmod/fx_1a0.h"

s32 fx_1a0_op81ParticleMotion0(Pool30Entry *pt, Pool6CEntry *owner, SVECTOR *pos, s32 trail, s32 frame, s32 n) {
    s32 pathTime = fx_1a0_pathFrames[0] * 2;

    return fxm_followPathParticle(pt, pos, &fx_1a0_posPaths[0], pathTime, frame, n);
}

s32 fx_1a0_op81ParticleMotion1(Pool30Entry *pt, Pool6CEntry *owner, SVECTOR *pos, s32 trail, s32 frame, s32 n) {
    s32 pathTime = fx_1a0_pathFrames[1] * 2;

    return fxm_followPathParticle(pt, pos, &fx_1a0_posPaths[1], pathTime, frame, n);
}

s32 fx_1a0_op81ParticleMotion2(Pool30Entry *pt, Pool6CEntry *owner, SVECTOR *pos, s32 trail, s32 frame, s32 n) {
    s32 pathTime = fx_1a0_pathFrames[2] * 2;

    return fxm_followPathParticle(pt, pos, &fx_1a0_posPaths[2], pathTime, frame, n);
}

s32 fx_1a0_op81ParticleMotion3(Pool30Entry *pt, Pool6CEntry *owner, SVECTOR *pos, s32 trail, s32 frame, s32 n) {
    s32 pathTime = fx_1a0_pathFrames[3] * 2;

    return fxm_followPathParticle(pt, pos, &fx_1a0_posPaths[3], pathTime, frame, n);
}

s32 fx_1a0_op81ParticleMotion4(Pool30Entry *pt, Pool6CEntry *owner, SVECTOR *pos, s32 trail, s32 frame, s32 n) {
    s32 pathTime = fx_1a0_pathFrames[4] * 2;

    return fxm_followPathParticle(pt, pos, &fx_1a0_posPaths[4], pathTime, frame, n);
}

s32 fx_1a0_op81ParticleMotion5(Pool30Entry *pt, Pool6CEntry *owner, SVECTOR *pos, s32 trail, s32 frame, s32 n) {
    s32 pathTime = fx_1a0_pathFrames[5] * 2;

    return fxm_followPathParticle(pt, pos, &fx_1a0_posPaths[5], pathTime, frame, n);
}

static s32 fxm_followPathParticle(Pool30Entry *pt, SVECTOR *pos, Fx1a0Path *path, s32 t, s32 frame, s32 frames) {
    SVECTOR pathPos;

    fxm_posPathGet(path, t, &pathPos);
    pathPos.vx += fx_1a0_pathOrigin.vx;
    pathPos.vy += fx_1a0_pathOrigin.vy;
    pathPos.vz += fx_1a0_pathOrigin.vz;
    pos->vx = pathPos.vx;
    pos->vy = pathPos.vy;
    pos->vz = pathPos.vz;
    pt->pos.vx = pos->vx << 12;
    pt->pos.vy = pos->vy << 12;
    pt->pos.vz = pos->vz << 12;
    if (frame >= frames) {
        return -1;
    }
    return 0;
}

s32 fx_1a0_op80(s32 mode, Fx1a0Work80 *work, Unk800F78A4 *env, Unk80100C78Entry *entry, s32 *args) {
    BtlfxExports *ex = BTLFX_EXPORTS(env);
    Fx1a0Bone *bone;
    MATRIX m;
    s32 frame;
    s32 i;

    if (mode == 0) {
        *args = sizeof(Fx1a0Work80);
    } else if (mode == 1) {
        fx_1a0_env = env;
    } else {
        frame = entry->frame;
        if (frame == 0) {
            for (i = 0; i < FX_FLAME_SLASH_BURSTS; i++) {
                work->emit0[i] = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, FX_FLAME_SLASH_FIRE_BURST), (s32)&fx_1a0_emitPos[i]);
                if (work->emit0[i] != NULL) {
                    work->emit0[i]->otZ = 0x20;
                }
            }
        }
        if (frame >= 0 && frame < FX_FLAME_SLASH_BURSTS_FOLLOW_FRAMES) {
            bone = fx_1a0_op80Bones;
            for (i = 0; i < FX_FLAME_SLASH_BURSTS; i++, bone++) {
                ex->btlfx_getUnitBoneMatrix(BTLFX_UNIT_CASTER, bone->bone0, (BtlBone *)&m);
                ex->btlfx_matrixPointAhead(&m, bone->dist2, (s32)&work->pos18[i]);
                if (work->emit0[i] != NULL && work->emit0[i]->state == BTLFX_EMIT_DONE) {
                    work->emit0[i] = NULL;
                }
                ex->btlfx_moveEmitterTo((s32)work->emit0[i], (s32)&work->pos18[i]);
            }
        }
        return frame >= FX_FLAME_SLASH_BURSTS_FRAMES;
    }
    return 0;
}

s32 fx_1a0_op81(s32 mode, Fx1a0Work81 *work, Unk800F78A4 *env, Unk80100C78Entry *entry, s32 *args) {
    BtlfxExports *ex = BTLFX_EXPORTS(env);
    s32 frame;

    if (mode == 0) {
        *args = sizeof(Fx1a0Work81);
    } else if (mode == 1) {
        s32 pathIdx = args[0];
        s32 extraEmit = args[1];

        fx_1a0_env = env;
        work->index14 = pathIdx;
        work->flag18 = extraEmit;
        ex->btlfx_getUnitBodyPos(0, 0, (s16 *)&work->posC);
        fx_1a0_pathOrigin.vx = work->posC.vx;
        fx_1a0_pathOrigin.vy = work->posC.vy;
        fx_1a0_pathOrigin.vz = work->posC.vz;
        fxm_posPathInit(&fx_1a0_posPaths[work->index14], (u8 *)ex->btlfx_getFileData(entry->file, fx_1a0_pathFileIds[work->index14]));
    } else {
        frame = entry->frame;
        fx_1a0_pathFrames[work->index14] = frame;
        if (frame == 0) {
            work->emit0 = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, fx_1a0_op81EmitFileIds[work->index14]),
                                           (s32)&fx_1a0_emitPos1[work->index14]);
            if (work->emit0 != NULL) {
                work->emit0->trails = (s32)fx_1a0_op81Trails[work->index14];
                work->emit0->trailSlots = FX_FLAME_SLASH_PATH_TRAIL_SLOTS;
                work->emit0->trailStride = FX_FLAME_SLASH_PATH_TRAIL_STRIDE;
            }
            if (work->emit0 != NULL) {
                work->emit0->file->m.module.update = fx_1a0_op81ParticleMotions[work->index14];
            }
            if (work->emit0 != NULL) {
                work->emit0->otZ = 0x20;
            }
            if (work->flag18 != 0) {
                work->emit4 = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, FX_FLAME_SLASH_FLAME_FLASH), (s32)&work->posC);
                work->emit8 = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, FX_FLAME_SLASH_RED_RINGS), (s32)&work->posC);
                if (work->emit4 != NULL) {
                    work->emit4->otZ = 0x80;
                }
                if (work->emit8 != NULL) {
                    work->emit8->otZ = 0x80;
                }
            }
        }
        return frame >= FX_FLAME_SLASH_PATH_FRAMES;
    }
    return 0;
}

s32 fx_1a0_op82(s32 mode, Fx1a0Work82 *work, Unk800F78A4 *env, Unk80100C78Entry *entry, s32 *args) {
    BtlfxExports *ex = BTLFX_EXPORTS(env);

    if (mode == 0) {
        *args = sizeof(Fx1a0Work82);
    } else if (mode == 1) {
        fx_1a0_env = env;
        ex->btlfx_getUnitBodyPos(0, 0, (s16 *)&work->pos4);
        work->emit0 = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, FX_FLAME_SLASH_FLAME_RINGS), (s32)&work->pos4);
    } else {
        return entry->frame >= FX_FLAME_SLASH_RINGS_FRAMES;
    }
    return 0;
}

static void fxm_posPathInit(Fx1a0Path *path, u16 *data) {
    data++;
    path->count2 = *data++;
    path->keys4 = (Fx1a0Point *)data;
}

static s32 fxm_posPathGet(Fx1a0Path *path, s32 t, SVECTOR *out) {
    s32 last = path->count2 - 1;
    s32 done = 1;
    Fx1a0Point *key;

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

INCLUDE_RODATA("asm/us/fx_1a0/nonmatchings/fx_1a0", fx_1a0_headerPtr);
