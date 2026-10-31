#include "fxmod/fx_191.h"

s32 fx_191_op80ParticleMotion0(Pool30Entry *e, Pool6CEntry *owner, SVECTOR *pos, s32 arg3, s32 frame, s32 frames) {
    SVECTOR z;

    z.vx = 0;
    z.vy = 0;
    z.vz = 0;
    D_801E84E4[0].vx -= 0x96;
    return fxm_lerpParticle(e, pos, &D_801E84E4[0], &D_801E857C[0], frame, frames);
}

s32 fx_191_op80ParticleMotion1(Pool30Entry *e, Pool6CEntry *owner, SVECTOR *pos, s32 arg3, s32 frame, s32 frames) {
    SVECTOR z;

    z.vx = 0;
    z.vy = 0;
    z.vz = 0;
    D_801E84E4[1].vx += 0x96;
    return fxm_lerpParticle(e, pos, &D_801E84E4[1], &D_801E857C[1], frame, frames);
}

s32 fx_191_op80ParticleMotion2(Pool30Entry *e, Pool6CEntry *owner, SVECTOR *pos, s32 arg3, s32 frame, s32 frames) {
    SVECTOR z;

    z.vx = 0;
    z.vy = 0;
    z.vz = 0;
    D_801E84E4[2].vx -= 0x32;
    return fxm_lerpParticle(e, pos, &D_801E84E4[2], &D_801E857C[2], frame, frames);
}

s32 fx_191_op80ParticleMotion3(Pool30Entry *e, Pool6CEntry *owner, SVECTOR *pos, s32 arg3, s32 frame, s32 frames) {
    SVECTOR z;

    z.vx = 0;
    z.vy = 0;
    z.vz = 0;
    D_801E84E4[3].vx += 0x32;
    return fxm_lerpParticle(e, pos, &D_801E84E4[3], &D_801E857C[3], frame, frames);
}

static s32 fxm_lerpParticle(Pool30Entry *e, SVECTOR *pos, SVECTOR *base, SVECTOR *to, s32 frame, s32 frames) {
    BtlfxExports *ex = BTLFX_EXPORTS(fx_191_env);
    SVECTOR a;
    SVECTOR b;
    s32 t;

    a.vx = D_801E8574.vx + base->vx;
    t = (D_801E8D9C << 12) / 12;
    a.vy = D_801E8574.vy + base->vy;
    a.vz = D_801E8574.vz + base->vz;
    ex->btlfx_lerpVec((s16 *)&a, (s16 *)to, t, (s16 *)&b);
    pos->vx = b.vx;
    pos->vy = b.vy;
    pos->vz = b.vz;
    e->pos.vx = pos->vx << 12;
    e->pos.vy = pos->vy << 12;
    e->pos.vz = pos->vz << 12;
    if (frame >= frames) {
        return -1;
    }
    return 0;
}

static s32 fx_191_op81ParticleMotion0(Pool30Entry *e, Pool6CEntry *owner, SVECTOR *pos, s32 arg3, s32 frame, s32 frames) {
    SVECTOR z;

    z.vx = 0;
    z.vy = 0;
    z.vz = 0;
    return fxm_followPathParticle0(e, pos, &z, frame, frames);
}

static s32 fx_191_op81ParticleMotion1(Pool30Entry *e, Pool6CEntry *owner, SVECTOR *pos, s32 arg3, s32 frame, s32 frames) {
    SVECTOR z;

    z.vx = 0;
    z.vy = 0;
    z.vz = 0;
    return fxm_followPathParticle1(e, pos, &z, frame, frames);
}

static s32 fxm_followPathParticle0(Pool30Entry *e, SVECTOR *pos, SVECTOR *ofs, s32 frame, s32 frames) {
    SVECTOR v;

    fxm_posPathGet(&D_801E8564, D_801E8DA0 * 3, &v);
    v.vx += D_801E859C.vx;
    v.vy += D_801E859C.vy;
    v.vz += D_801E859C.vz;
    v.vx += ofs->vx;
    v.vy += ofs->vy;
    v.vz += ofs->vz;
    pos->vx = v.vx;
    pos->vy = v.vy;
    pos->vz = v.vz;
    e->pos.vx = pos->vx << 12;
    e->pos.vy = pos->vy << 12;
    e->pos.vz = pos->vz << 12;
    if (frame >= frames) {
        return -1;
    }
    return 0;
}

static s32 fxm_followPathParticle1(Pool30Entry *e, SVECTOR *pos, SVECTOR *ofs, s32 frame, s32 frames) {
    SVECTOR v;

    fxm_posPathGet(&D_801E856C, (D_801E8DA0 - 5) * 3, &v);
    v.vx += D_801E859C.vx;
    v.vy += D_801E859C.vy;
    v.vz += D_801E859C.vz;
    v.vx += ofs->vx;
    v.vy += ofs->vy;
    v.vz += ofs->vz;
    pos->vx = v.vx;
    pos->vy = v.vy;
    pos->vz = v.vz;
    e->pos.vx = pos->vx << 12;
    e->pos.vy = pos->vy << 12;
    e->pos.vz = pos->vz << 12;
    if (frame >= frames) {
        return -1;
    }
    return 0;
}

s32 fx_191_op80(s32 mode, Fx191Work80 *work, Unk800F78A4 *env, Unk80100C78Entry *entry, s32 *arg4) {
    BtlfxExports *ex = BTLFX_EXPORTS(env);
    s32 frame;
    s32 i;

    if (mode == 0) {
        *arg4 = 0x28;
    } else if (mode == 1) {
        fx_191_env = env;
        ex->btlfx_getUnitPos(0x10, (s16 *)&work->pos10);
        ex->btlfx_getUnitPos(0, (s16 *)&work->pos18);
        D_801E8574.vx = work->pos10.vx;
        D_801E8574.vy = work->pos10.vy;
        D_801E8574.vz = work->pos10.vz;
        for (i = 0; i < 4; i++) {
            work->emit0[i] = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, D_801E84DC[i]), (s32)&D_801E85CC);
            if (work->emit0[i] != NULL) {
                work->emit0[i]->trails = (s32)D_801E85DC[i];
                work->emit0[i]->trailSlots = 4;
                work->emit0[i]->trailStride = 0x60;
            }
            if (work->emit0[i] != NULL) {
                work->emit0[i]->file->m.module.update = fx_191_op80ParticleMotions[i];
            }
            D_801E857C[i].vx = work->pos18.vx;
            D_801E857C[i].vy = work->pos18.vy;
            D_801E857C[i].vz = work->pos18.vz;
        }
        D_801E857C[0].vx -= 0xC8;
        D_801E857C[0].vy -= 0xC8;
        D_801E857C[1].vx += 0xC8;
        D_801E857C[0].vy -= 0xC8;
        D_801E857C[2].vy -= 0x320;
        D_801E857C[3].vy -= 0xC8;
        ex->btlfx_aimAngles((s16 *)&work->pos10, (s16 *)&work->pos18, (s16 *)&work->unk20);
    } else {
        frame = entry->frame;
        D_801E8D9C = frame;
        return frame >= 0x28;
    }
    return 0;
}

s32 fx_191_op81(s32 mode, Fx191Work81 *work, Unk800F78A4 *env, Unk80100C78Entry *entry, s32 *arg4) {
    BtlfxExports *ex = BTLFX_EXPORTS(env);
    s32 frame;
    s32 t;

    if (mode == 0) {
        *arg4 = 0x60;
    } else if (mode == 1) {
        fx_191_env = env;
        ex->btlfx_getUnitBodyPos(0, 0, (s16 *)&work->pos18);
        D_801E859C.vx = work->pos18.vx;
        D_801E859C.vy = work->pos18.vy;
        D_801E859C.vz = work->pos18.vz;
        work->emit0 = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, 2), (s32)&D_801E85CC);
        if (work->emit0 != NULL) {
            work->emit0->trails = (s32)D_801E8BDC;
            work->emit0->trailSlots = 2;
            work->emit0->trailStride = 0x70;
        }
        if (work->emit0 != NULL) {
            work->emit0->file->m.module.update = fx_191_op81ParticleMotion0;
        }
        if (work->emit0 != NULL) {
            work->emit0->otZ = ex->btlfx_addRadiusSixth(0, 0x80);
        }
        fxm_posPathInit(&D_801E8564, (u8 *)ex->btlfx_getFileData(entry->file, 4));
        fxm_posPathInit(&D_801E856C, (u8 *)ex->btlfx_getFileData(entry->file, 5));
    } else {
        frame = entry->frame;
        D_801E8DA0 = frame;
        switch (frame) {
        case 5:
            work->emit4 = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, 9), (s32)&D_801E85D4);
            if (work->emit4 != NULL) {
                work->emit4->trails = (s32)D_801E8CBC;
                work->emit4->trailSlots = 2;
                work->emit4->trailStride = 0x70;
            }
            if (work->emit4 != NULL) {
                work->emit4->file->m.module.update = fx_191_op81ParticleMotion1;
            }
            if (work->emit4 != NULL) {
                work->emit4->otZ = ex->btlfx_addRadiusSixth(0, 0x80);
            }
            break;
        case 3:
            work->pos30.vx = -0x140;
            work->pos30.vy = 0x8C;
            work->unk38.vx = 0x154;
            work->unk38.vy = -0x82;
            work->pos30.vz = 0;
            work->unk38.vz = 0;
            work->pos30.vx += work->pos18.vx;
            work->pos30.vy += work->pos18.vy;
            work->pos30.vz += work->pos18.vz;
            work->unk38.vx += work->pos18.vx;
            work->unk38.vy += work->pos18.vy;
            work->unk38.vz += work->pos18.vz;
            work->emit8 = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, 0x12), (s32)&work->pos30);
            work->emitC = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, 0x11), (s32)&work->pos18);
            if (work->emit8 != NULL) {
                work->emit8->otZ = ex->btlfx_addRadiusSixth(0, 0x80);
            }
            if (work->emitC != NULL) {
                work->emitC->otZ = ex->btlfx_addRadiusSixth(0, 0x80);
            }
            break;
        case 8:
            work->pos40.vx = 0xFA;
            work->pos40.vy = -0x12C;
            work->unk48.vx = -0x12C;
            work->unk48.vy = 0x140;
            work->pos40.vz = 0;
            work->unk48.vz = 0;
            work->pos40.vx += work->pos18.vx;
            work->pos40.vy += work->pos18.vy;
            work->pos40.vz += work->pos18.vz;
            work->unk48.vx += work->pos18.vx;
            work->unk48.vy += work->pos18.vy;
            work->unk48.vz += work->pos18.vz;
            work->emit10 = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, 0xF), (s32)&work->pos40);
            work->emit14 = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, 0x10), (s32)&work->pos18);
            if (work->emit10 != NULL) {
                work->emit10->otZ = ex->btlfx_addRadiusSixth(0, 0x80);
            }
            if (work->emit14 != NULL) {
                work->emit14->otZ = ex->btlfx_addRadiusSixth(0, 0x80);
            }
            break;
        }
        if (frame >= 3) {
            t = (frame - 3) << 10;
            if (t > 0x1000) {
                t = 0x1000;
            }
            ex->btlfx_lerpVec((s16 *)&work->pos30, (s16 *)&work->unk38, t, (s16 *)&work->unk50);
            if (work->emit8 != NULL && work->emit8->state == BTLFX_EMIT_DONE) {
                work->emit8 = NULL;
            }
            if (work->emit8 != NULL) {
                work->emit8->target = (s32)&work->unk50;
            }
        }
        if (frame >= 8) {
            t = (frame - 8) << 10;
            if (t > 0x1000) {
                t = 0x1000;
            }
            ex->btlfx_lerpVec((s16 *)&work->pos40, (s16 *)&work->unk48, t, (s16 *)&work->unk58);
            if (work->emit10 != NULL && work->emit10->state == BTLFX_EMIT_DONE) {
                work->emit10 = NULL;
            }
            if (work->emit10 != NULL) {
                work->emit10->target = (s32)&work->unk58;
            }
        }
        return frame >= 0x28;
    }
    return 0;
}

static void fxm_posPathInit(Fx191Path *path, u16 *data) {
    data++;
    path->unk2 = *data++;
    path->unk4 = (Fx191Point *)data;
}

static s32 fxm_posPathGet(Fx191Path *path, s32 t, SVECTOR *out) {
    s32 last = path->unk2 - 1;
    s32 done = 1;
    Fx191Point *p;

    if (last < t) {
        t = last;
    } else {
        done = 0;
    }
    p = &path->unk4[t];
    out->vx = p->x;
    out->vy = p->y;
    out->vz = p->z;
    return done;
}

INCLUDE_RODATA("asm/us/fx_191/nonmatchings/fx_191", fx_191_headerPtr);
