#include "fxmod/fx_0b4.h"

s32 fx_0b4_op80ParticleMotion0(Pool30Entry *pt, s32 arg1, SVECTOR *pos, s32 arg3, s32 frame, s32 n) {
    return fxm_followPathParticle(pt, pos, &D_801E80EC[0], &D_801E81C4[0], frame, n);
}

s32 fx_0b4_op80ParticleMotion1(Pool30Entry *pt, s32 arg1, SVECTOR *pos, s32 arg3, s32 frame, s32 n) {
    return fxm_followPathParticle(pt, pos, &D_801E80EC[1], &D_801E81C4[1], frame, n);
}

s32 fx_0b4_op80ParticleMotion2(Pool30Entry *pt, s32 arg1, SVECTOR *pos, s32 arg3, s32 frame, s32 n) {
    return fxm_followPathParticle(pt, pos, &D_801E80EC[2], &D_801E81C4[2], frame, n);
}

s32 fx_0b4_op80ParticleMotion3(Pool30Entry *pt, s32 arg1, SVECTOR *pos, s32 arg3, s32 frame, s32 n) {
    return fxm_followPathParticle(pt, pos, &D_801E80EC[3], &D_801E81C4[3], frame, n);
}

s32 fx_0b4_op80ParticleMotion4(Pool30Entry *pt, s32 arg1, SVECTOR *pos, s32 arg3, s32 frame, s32 n) {
    return fxm_followPathParticle(pt, pos, &D_801E80EC[4], &D_801E81C4[4], frame, n);
}

s32 fx_0b4_op80ParticleMotion5(Pool30Entry *pt, s32 arg1, SVECTOR *pos, s32 arg3, s32 frame, s32 n) {
    return fxm_followPathParticle(pt, pos, &D_801E80EC[5], &D_801E81C4[5], frame, n);
}

static s32 fxm_followPathParticle(Pool30Entry *pt, SVECTOR *pos, Fx0b4Timer *timer, Fx0b4Path *path, s32 frame, s32 n) {
    SVECTOR rot;
    SVECTOR p;

    fxm_rotPosPathGet(path, frame, &rot, &p);
    p.vx /= 2;
    p.vz /= 2;
    p.vx = p.vx * 12 / 10;
    p.vy = p.vy * 12 / 10;
    p.vz = p.vz * 12 / 10;
    p.vx += D_801E818C.vx;
    p.vy += D_801E818C.vy;
    p.vz += D_801E818C.vz;
    pos->vx = p.vx;
    pos->vy = p.vy;
    pos->vz = p.vz;
    pt->pos.vx = pos->vx << 12;
    pt->pos.vy = pos->vy << 12;
    pt->pos.vz = pos->vz << 12;
    if (frame >= n) {
        return -1;
    }
    return 0;
}

INCLUDE_RODATA("asm/us/fx_0b4/nonmatchings/fx_0b4", fx_0b4_headerPtr);

s32 fx_0b4_op80(s32 mode, Fx0b4Work *work, Unk800F78A4 *env, Unk80100C78Entry *entry, s32 *arg4) {
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
        *arg4 = 0x3C;
    } else if (mode == 1) {
        fx_0b4_env = env;
        ex->btlfx_getUnitBodyPos(0, 0, (s16 *)&work->pos2C);
        ex->btlfx_getUnitPos(0, (s16 *)&work->pos34);
        D_801E818C.vx = work->pos34.vx;
        D_801E818C.vy = work->pos34.vy;
        D_801E818C.vz = work->pos34.vz;
        for (k = 0, tq = D_801E80EC; k < 6; k++, tq++) {
            tq->unk0 = k * 2 + 0x14;
            fxm_rotPosPathInit(&D_801E81C4[k], ex->btlfx_getFileData(entry->file, D_801E8110[k]));
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
                work->emit18->trails = (s32)D_801ED2F4;
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
        tm = D_801E80EC;
        for (i = 0, cnt = &tm->unk2; i < 6; i++, cnt += 2, tm++) {
            if (frame == tm->unk0) {
                work->emit0[i] = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, D_801E8104[i]), (s32)&D_801E8194[i]);
                if (work->emit0[i] != NULL) {
                    work->emit0[i]->trails = (s32)D_801E81F4[i];
                    work->emit0[i]->trailSlots = 0x18;
                    work->emit0[i]->trailStride = 0x90;
                }
                if (work->emit0[i] != NULL) {
                    work->emit0[i]->file->m.module.update = fx_0b4_op80ParticleMotions[i];
                }
            }
            if (frame >= tm->unk0) {
                (*cnt)++;
            }
        }
        if (frame >= 0) {
            if (work->emit1C != NULL) {
                if (work->emit1C->state == BTLFX_EMIT_DONE) {
                    work->emit1C = NULL;
                }
                if (work->emit1C != NULL) {
                    work->emit1C->rot.vx = D_801E811C.vx;
                    work->emit1C->rot.vy = D_801E811C.vy;
                    work->emit1C->rot.vz = D_801E811C.vz;
                }
            }
        }
        if (frame >= 8) {
            if (work->emit20 != NULL) {
                if (work->emit20->state == BTLFX_EMIT_DONE) {
                    work->emit20 = NULL;
                }
                if (work->emit20 != NULL) {
                    work->emit20->rot.vx = D_801E811C.vx;
                    work->emit20->rot.vy = D_801E811C.vy;
                    work->emit20->rot.vz = D_801E811C.vz;
                }
            }
        }
        if (frame >= 0x14) {
            if (work->emit18 != NULL) {
                if (work->emit18->state == BTLFX_EMIT_DONE) {
                    work->emit18 = NULL;
                }
                if (work->emit18 != NULL) {
                    work->emit18->rot.vx = D_801E811C.vx;
                    work->emit18->rot.vy = D_801E811C.vy;
                    work->emit18->rot.vz = D_801E811C.vz;
                }
            }
        }
        if (frame >= 0x2D) {
            if (work->emit28 != NULL) {
                if (work->emit28->state == BTLFX_EMIT_DONE) {
                    work->emit28 = NULL;
                }
                if (work->emit28 != NULL) {
                    work->emit28->rot.vx = D_801E811C.vx;
                    work->emit28->rot.vy = D_801E811C.vy;
                    work->emit28->rot.vz = D_801E811C.vz;
                }
            }
        }
        return frame >= 0x5A;
    }
    return 0;
}

static void fxm_rotPosPathInit(Fx0b4Path *path, u16 *data) {
    data++;
    path->unk2 = *data++;
    path->unk4 = (Fx0b4Point *)data;
}

static s32 fxm_rotPosPathGet(Fx0b4Path *path, s32 i, SVECTOR *rot, SVECTOR *pos) {
    Fx0b4Key *k;
    s32 r = 1;

    if (i > path->unk2 - 1) {
        i = path->unk2 - 1;
    } else {
        r = 0;
    }
    k = &path->unk4[i];
    if (rot != NULL) {
        rot->vx = k->unk6[0];
        rot->vy = k->unk6[1];
        rot->vz = k->unk6[2];
    }
    if (pos != NULL) {
        pos->vx = k->unk0[0];
        pos->vy = k->unk0[1];
        pos->vz = k->unk0[2];
    }
    return r;
}
