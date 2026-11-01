#include "fxmod/fx_0b4.h"

s32 fx_0b4_op80ParticleMotion0(Pool30Entry *pt, s32 owner, SVECTOR *pos, s32 trail, s32 frame, s32 n) {
    return fxm_followPathParticle(pt, pos, &fx_0b4_timers[0], &fx_0b4_paths[0], frame, n);
}

s32 fx_0b4_op80ParticleMotion1(Pool30Entry *pt, s32 owner, SVECTOR *pos, s32 trail, s32 frame, s32 n) {
    return fxm_followPathParticle(pt, pos, &fx_0b4_timers[1], &fx_0b4_paths[1], frame, n);
}

s32 fx_0b4_op80ParticleMotion2(Pool30Entry *pt, s32 owner, SVECTOR *pos, s32 trail, s32 frame, s32 n) {
    return fxm_followPathParticle(pt, pos, &fx_0b4_timers[2], &fx_0b4_paths[2], frame, n);
}

s32 fx_0b4_op80ParticleMotion3(Pool30Entry *pt, s32 owner, SVECTOR *pos, s32 trail, s32 frame, s32 n) {
    return fxm_followPathParticle(pt, pos, &fx_0b4_timers[3], &fx_0b4_paths[3], frame, n);
}

s32 fx_0b4_op80ParticleMotion4(Pool30Entry *pt, s32 owner, SVECTOR *pos, s32 trail, s32 frame, s32 n) {
    return fxm_followPathParticle(pt, pos, &fx_0b4_timers[4], &fx_0b4_paths[4], frame, n);
}

s32 fx_0b4_op80ParticleMotion5(Pool30Entry *pt, s32 owner, SVECTOR *pos, s32 trail, s32 frame, s32 n) {
    return fxm_followPathParticle(pt, pos, &fx_0b4_timers[5], &fx_0b4_paths[5], frame, n);
}

static s32 fxm_followPathParticle(Pool30Entry *pt, SVECTOR *pos, Fx0b4Timer *timer, Fx0b4Path *path, s32 frame, s32 n) {
    SVECTOR rot;
    SVECTOR pathPos;

    fxm_rotPosPathGet(path, frame, &rot, &pathPos);
    pathPos.vx /= 2;
    pathPos.vz /= 2;
    pathPos.vx = pathPos.vx * 12 / 10;
    pathPos.vy = pathPos.vy * 12 / 10;
    pathPos.vz = pathPos.vz * 12 / 10;
    pathPos.vx += fx_0b4_pathOrigin.vx;
    pathPos.vy += fx_0b4_pathOrigin.vy;
    pathPos.vz += fx_0b4_pathOrigin.vz;
    pos->vx = pathPos.vx;
    pos->vy = pathPos.vy;
    pos->vz = pathPos.vz;
    pt->pos.vx = pos->vx << 12;
    pt->pos.vy = pos->vy << 12;
    pt->pos.vz = pos->vz << 12;
    if (frame >= n) {
        return -1;
    }
    return 0;
}

INCLUDE_RODATA("asm/us/fx_0b4/nonmatchings/fx_0b4", fx_0b4_headerPtr);

s32 fx_0b4_op80(s32 mode, Fx0b4Work *work, Unk800F78A4 *env, Unk80100C78Entry *entry, s32 *args) {
    BtlfxExports *ex = BTLFX_EXPORTS(env);
    s32 frame;
    s32 i;
    s32 k;
    s32 s;
    Fx0b4Timer *tm;
    Fx0b4Timer *tq;
    u16 *cnt;
    Pool6CEntry **pp;
    s32 *anim;
    SVECTOR *pos;
    u16 *file;

    if (mode == 0) {
        *args = 0x3C;
    } else if (mode == 1) {
        fx_0b4_env = env;
        ex->btlfx_getUnitBodyPos(0, 0, (s16 *)&work->pos2C);
        ex->btlfx_getUnitPos(0, (s16 *)&work->pos34);
        fx_0b4_pathOrigin.vx = work->pos34.vx;
        fx_0b4_pathOrigin.vy = work->pos34.vy;
        fx_0b4_pathOrigin.vz = work->pos34.vz;
        for (k = 0, tq = fx_0b4_timers; k < 6; k++, tq++) {
            tq->start0 = k * 2 + 0x14;
            fxm_rotPosPathInit(&fx_0b4_paths[k], ex->btlfx_getFileData(entry->file, fx_0b4_pathBlocks[k]));
        }
        return 0;
    } else {
        frame = entry->frame;
        switch (frame) {
        case 0:
            work->emit1C = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, 0x11), (s32)&work->pos34);
            break;
        case 8:
            work->emit20 = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, 0xF), (s32)&work->pos34);
            break;
        case 20:
            work->emit18 = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, 0x19), (s32)&work->pos34);
            if (work->emit18 != NULL) {
                work->emit18->trails = (s32)fx_0b4_trails2;
                work->emit18->trailSlots = 0x12;
                work->emit18->trailStride = 0x98;
            }
            break;
        case 25:
            work->emit24 = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, 0x10), (s32)&work->pos2C);
            break;
        case 45:
            work->emit28 = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, 0xE), (s32)&work->pos34);
            break;
        }
        tm = fx_0b4_timers;
        for (i = 0, cnt = &tm->count2; i < 6; i++, cnt += 2, tm++) {
            if (frame == tm->start0) {
                work->emit0[i] = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, fx_0b4_emitBlocks[i]), (s32)&fx_0b4_emitStartPos[i]);
                if (work->emit0[i] != NULL) {
                    work->emit0[i]->trails = (s32)fx_0b4_trails[i];
                    work->emit0[i]->trailSlots = 0x18;
                    work->emit0[i]->trailStride = 0x90;
                }
                if (work->emit0[i] != NULL) {
                    work->emit0[i]->file->m.module.update = fx_0b4_op80ParticleMotions[i];
                }
            }
            if (frame >= tm->start0) {
                (*cnt)++;
            }
        }
        if (frame >= 0) {
            if (work->emit1C != NULL) {
                if (work->emit1C->state == BTLFX_EMIT_DONE) {
                    work->emit1C = NULL;
                }
                if (work->emit1C != NULL) {
                    work->emit1C->rot.vx = fx_0b4_emitRot.vx;
                    work->emit1C->rot.vy = fx_0b4_emitRot.vy;
                    work->emit1C->rot.vz = fx_0b4_emitRot.vz;
                }
            }
        }
        if (frame >= 8) {
            if (work->emit20 != NULL) {
                if (work->emit20->state == BTLFX_EMIT_DONE) {
                    work->emit20 = NULL;
                }
                if (work->emit20 != NULL) {
                    work->emit20->rot.vx = fx_0b4_emitRot.vx;
                    work->emit20->rot.vy = fx_0b4_emitRot.vy;
                    work->emit20->rot.vz = fx_0b4_emitRot.vz;
                }
            }
        }
        if (frame >= 0x14) {
            if (work->emit18 != NULL) {
                if (work->emit18->state == BTLFX_EMIT_DONE) {
                    work->emit18 = NULL;
                }
                if (work->emit18 != NULL) {
                    work->emit18->rot.vx = fx_0b4_emitRot.vx;
                    work->emit18->rot.vy = fx_0b4_emitRot.vy;
                    work->emit18->rot.vz = fx_0b4_emitRot.vz;
                }
            }
        }
        if (frame >= 0x2D) {
            if (work->emit28 != NULL) {
                if (work->emit28->state == BTLFX_EMIT_DONE) {
                    work->emit28 = NULL;
                }
                if (work->emit28 != NULL) {
                    work->emit28->rot.vx = fx_0b4_emitRot.vx;
                    work->emit28->rot.vy = fx_0b4_emitRot.vy;
                    work->emit28->rot.vz = fx_0b4_emitRot.vz;
                }
            }
        }
        return frame >= 0x5A;
    }
    return 0;
}

static void fxm_rotPosPathInit(Fx0b4Path *path, u16 *data) {
    data++;
    path->count2 = *data++;
    path->keys4 = (Fx0b4Point *)data;
}

static s32 fxm_rotPosPathGet(Fx0b4Path *path, s32 t, SVECTOR *rot, SVECTOR *pos) {
    Fx0b4Key *key;
    s32 done = 1;

    if (t > path->count2 - 1) {
        t = path->count2 - 1;
    } else {
        done = 0;
    }
    key = &path->keys4[t];
    if (rot != NULL) {
        rot->vx = key->rot6[0];
        rot->vy = key->rot6[1];
        rot->vz = key->rot6[2];
    }
    if (pos != NULL) {
        pos->vx = key->pos0[0];
        pos->vy = key->pos0[1];
        pos->vz = key->pos0[2];
    }
    return done;
}
