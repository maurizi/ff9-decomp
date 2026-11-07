#include "fxmod/fx_191.h"

s32 fx_191_op80ParticleMotion0(Pool30Entry *pt, Pool6CEntry *owner, SVECTOR *pos, s32 trail, s32 frame, s32 frames) {
    SVECTOR zero;

    zero.vx = 0;
    zero.vy = 0;
    zero.vz = 0;
    fx_191_op80Offsets[0].vx -= 0x96;
    return fxm_lerpParticle(pt, pos, &fx_191_op80Offsets[0], &fx_191_op80Targets[0], frame, frames);
}

s32 fx_191_op80ParticleMotion1(Pool30Entry *pt, Pool6CEntry *owner, SVECTOR *pos, s32 trail, s32 frame, s32 frames) {
    SVECTOR zero;

    zero.vx = 0;
    zero.vy = 0;
    zero.vz = 0;
    fx_191_op80Offsets[1].vx += 0x96;
    return fxm_lerpParticle(pt, pos, &fx_191_op80Offsets[1], &fx_191_op80Targets[1], frame, frames);
}

s32 fx_191_op80ParticleMotion2(Pool30Entry *pt, Pool6CEntry *owner, SVECTOR *pos, s32 trail, s32 frame, s32 frames) {
    SVECTOR zero;

    zero.vx = 0;
    zero.vy = 0;
    zero.vz = 0;
    fx_191_op80Offsets[2].vx -= 0x32;
    return fxm_lerpParticle(pt, pos, &fx_191_op80Offsets[2], &fx_191_op80Targets[2], frame, frames);
}

s32 fx_191_op80ParticleMotion3(Pool30Entry *pt, Pool6CEntry *owner, SVECTOR *pos, s32 trail, s32 frame, s32 frames) {
    SVECTOR zero;

    zero.vx = 0;
    zero.vy = 0;
    zero.vz = 0;
    fx_191_op80Offsets[3].vx += 0x32;
    return fxm_lerpParticle(pt, pos, &fx_191_op80Offsets[3], &fx_191_op80Targets[3], frame, frames);
}

static s32 fxm_lerpParticle(Pool30Entry *pt, SVECTOR *pos, SVECTOR *base, SVECTOR *to, s32 frame, s32 frames) {
    BtlfxExports *ex = BTLFX_EXPORTS(fx_191_env);
    SVECTOR start;
    SVECTOR cur;
    s32 t;

    start.vx = fx_191_op80Origin.vx + base->vx;
    t = (fx_191_op80Frame << 12) / FX_LANCER_STREAK_FRAMES;
    start.vy = fx_191_op80Origin.vy + base->vy;
    start.vz = fx_191_op80Origin.vz + base->vz;
    ex->btlfx_lerpVec((s16 *)&start, (s16 *)to, t, (s16 *)&cur);
    pos->vx = cur.vx;
    pos->vy = cur.vy;
    pos->vz = cur.vz;
    pt->pos.vx = pos->vx << 12;
    pt->pos.vy = pos->vy << 12;
    pt->pos.vz = pos->vz << 12;
    if (frame >= frames) {
        return -1;
    }
    return 0;
}

static s32 fx_191_op81ParticleMotion0(Pool30Entry *pt, Pool6CEntry *owner, SVECTOR *pos, s32 trail, s32 frame, s32 frames) {
    SVECTOR zero;

    zero.vx = 0;
    zero.vy = 0;
    zero.vz = 0;
    return fxm_followPathParticle0(pt, pos, &zero, frame, frames);
}

static s32 fx_191_op81ParticleMotion1(Pool30Entry *pt, Pool6CEntry *owner, SVECTOR *pos, s32 trail, s32 frame, s32 frames) {
    SVECTOR zero;

    zero.vx = 0;
    zero.vy = 0;
    zero.vz = 0;
    return fxm_followPathParticle1(pt, pos, &zero, frame, frames);
}

static s32 fxm_followPathParticle0(Pool30Entry *pt, SVECTOR *pos, SVECTOR *ofs, s32 frame, s32 frames) {
    SVECTOR pathPos;

    fxm_posPathGet(&fx_191_posPath0, fx_191_op81Frame * 3, &pathPos);
    pathPos.vx += fx_191_op81PathOrigin.vx;
    pathPos.vy += fx_191_op81PathOrigin.vy;
    pathPos.vz += fx_191_op81PathOrigin.vz;
    pathPos.vx += ofs->vx;
    pathPos.vy += ofs->vy;
    pathPos.vz += ofs->vz;
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

static s32 fxm_followPathParticle1(Pool30Entry *pt, SVECTOR *pos, SVECTOR *ofs, s32 frame, s32 frames) {
    SVECTOR pathPos;

    fxm_posPathGet(&fx_191_posPath1, (fx_191_op81Frame - FX_LANCER_GOLD_TRAIL_2_FRAME) * 3, &pathPos);
    pathPos.vx += fx_191_op81PathOrigin.vx;
    pathPos.vy += fx_191_op81PathOrigin.vy;
    pathPos.vz += fx_191_op81PathOrigin.vz;
    pathPos.vx += ofs->vx;
    pathPos.vy += ofs->vy;
    pathPos.vz += ofs->vz;
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

s32 fx_191_op80(s32 mode, Fx191Work80 *work, Unk800F78A4 *env, Unk80100C78Entry *entry, s32 *args) {
    BtlfxExports *ex = BTLFX_EXPORTS(env);
    s32 frame;
    s32 i;

    if (mode == 0) {
        *args = sizeof(Fx191Work80);
    } else if (mode == 1) {
        fx_191_env = env;
        ex->btlfx_getUnitPos(0x10, (s16 *)&work->pos10);
        ex->btlfx_getUnitPos(0, (s16 *)&work->pos18);
        fx_191_op80Origin.vx = work->pos10.vx;
        fx_191_op80Origin.vy = work->pos10.vy;
        fx_191_op80Origin.vz = work->pos10.vz;
        for (i = 0; i < FX_LANCER_STREAKS; i++) {
            work->emit0[i] = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, fx_191_emitFileIds[i]), (s32)&fx_191_emitPos0);
            if (work->emit0[i] != NULL) {
                work->emit0[i]->trails = (s32)fx_191_op80Trails[i];
                work->emit0[i]->trailSlots = FX_LANCER_STREAK_TRAIL_SLOTS;
                work->emit0[i]->trailStride = FX_LANCER_STREAK_TRAIL_STRIDE;
            }
            if (work->emit0[i] != NULL) {
                work->emit0[i]->file->m.module.update = fx_191_op80ParticleMotions[i];
            }
            fx_191_op80Targets[i].vx = work->pos18.vx;
            fx_191_op80Targets[i].vy = work->pos18.vy;
            fx_191_op80Targets[i].vz = work->pos18.vz;
        }
        fx_191_op80Targets[0].vx -= 0xC8;
        fx_191_op80Targets[0].vy -= 0xC8;
        fx_191_op80Targets[1].vx += 0xC8;
        fx_191_op80Targets[0].vy -= 0xC8;
        fx_191_op80Targets[2].vy -= 0x320;
        fx_191_op80Targets[3].vy -= 0xC8;
        ex->btlfx_aimAngles((s16 *)&work->pos10, (s16 *)&work->pos18, (s16 *)&work->rot20);
    } else {
        frame = entry->frame;
        fx_191_op80Frame = frame;
        return frame >= FX_LANCER_STREAKS_FRAMES;
    }
    return 0;
}

s32 fx_191_op81(s32 mode, Fx191Work81 *work, Unk800F78A4 *env, Unk80100C78Entry *entry, s32 *args) {
    BtlfxExports *ex = BTLFX_EXPORTS(env);
    s32 frame;
    s32 t;

    if (mode == 0) {
        *args = sizeof(Fx191Work81);
    } else if (mode == 1) {
        fx_191_env = env;
        ex->btlfx_getUnitBodyPos(0, 0, (s16 *)&work->pos18);
        fx_191_op81PathOrigin.vx = work->pos18.vx;
        fx_191_op81PathOrigin.vy = work->pos18.vy;
        fx_191_op81PathOrigin.vz = work->pos18.vz;
        work->emit0 = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, FX_LANCER_GOLD_TRAIL), (s32)&fx_191_emitPos0);
        if (work->emit0 != NULL) {
            work->emit0->trails = (s32)fx_191_op81Trails0;
            work->emit0->trailSlots = FX_LANCER_GOLD_TRAIL_TRAIL_SLOTS;
            work->emit0->trailStride = FX_LANCER_GOLD_TRAIL_TRAIL_STRIDE;
        }
        if (work->emit0 != NULL) {
            work->emit0->file->m.module.update = fx_191_op81ParticleMotion0;
        }
        if (work->emit0 != NULL) {
            work->emit0->otZ = ex->btlfx_addRadiusSixth(0, 0x80);
        }
        fxm_posPathInit(&fx_191_posPath0, (u8 *)ex->btlfx_getFileData(entry->file, FX_LANCER_PATH));
        fxm_posPathInit(&fx_191_posPath1, (u8 *)ex->btlfx_getFileData(entry->file, FX_LANCER_PATH_2));
    } else {
        frame = entry->frame;
        fx_191_op81Frame = frame;
        switch (frame) {
        case FX_LANCER_GOLD_TRAIL_2_FRAME:
            work->emit4 = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, FX_LANCER_GOLD_TRAIL_2), (s32)&fx_191_emitPos1);
            if (work->emit4 != NULL) {
                work->emit4->trails = (s32)fx_191_op81Trails1;
                work->emit4->trailSlots = FX_LANCER_GOLD_TRAIL_TRAIL_SLOTS;
                work->emit4->trailStride = FX_LANCER_GOLD_TRAIL_TRAIL_STRIDE;
            }
            if (work->emit4 != NULL) {
                work->emit4->file->m.module.update = fx_191_op81ParticleMotion1;
            }
            if (work->emit4 != NULL) {
                work->emit4->otZ = ex->btlfx_addRadiusSixth(0, 0x80);
            }
            break;
        case FX_LANCER_CYAN_SPARKS_FRAME:
            work->pos30.vx = -0x140;
            work->pos30.vy = 0x8C;
            work->pos38.vx = 0x154;
            work->pos38.vy = -0x82;
            work->pos30.vz = 0;
            work->pos38.vz = 0;
            work->pos30.vx += work->pos18.vx;
            work->pos30.vy += work->pos18.vy;
            work->pos30.vz += work->pos18.vz;
            work->pos38.vx += work->pos18.vx;
            work->pos38.vy += work->pos18.vy;
            work->pos38.vz += work->pos18.vz;
            work->emit8 = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, FX_LANCER_CYAN_SPARKS), (s32)&work->pos30);
            work->emitC = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, FX_LANCER_GREEN_RINGS), (s32)&work->pos18);
            if (work->emit8 != NULL) {
                work->emit8->otZ = ex->btlfx_addRadiusSixth(0, 0x80);
            }
            if (work->emitC != NULL) {
                work->emitC->otZ = ex->btlfx_addRadiusSixth(0, 0x80);
            }
            break;
        case FX_LANCER_RED_SPARKS_FRAME:
            work->pos40.vx = 0xFA;
            work->pos40.vy = -0x12C;
            work->pos48.vx = -0x12C;
            work->pos48.vy = 0x140;
            work->pos40.vz = 0;
            work->pos48.vz = 0;
            work->pos40.vx += work->pos18.vx;
            work->pos40.vy += work->pos18.vy;
            work->pos40.vz += work->pos18.vz;
            work->pos48.vx += work->pos18.vx;
            work->pos48.vy += work->pos18.vy;
            work->pos48.vz += work->pos18.vz;
            work->emit10 = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, FX_LANCER_RED_SPARKS), (s32)&work->pos40);
            work->emit14 = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, FX_LANCER_ORANGE_RINGS), (s32)&work->pos18);
            if (work->emit10 != NULL) {
                work->emit10->otZ = ex->btlfx_addRadiusSixth(0, 0x80);
            }
            if (work->emit14 != NULL) {
                work->emit14->otZ = ex->btlfx_addRadiusSixth(0, 0x80);
            }
            break;
        }
        if (frame >= FX_LANCER_CYAN_SPARKS_FRAME) {
            t = (frame - FX_LANCER_CYAN_SPARKS_FRAME) << 10;
            if (t > 0x1000) {
                t = 0x1000;
            }
            ex->btlfx_lerpVec((s16 *)&work->pos30, (s16 *)&work->pos38, t, (s16 *)&work->target50);
            if (work->emit8 != NULL && work->emit8->state == BTLFX_EMIT_DONE) {
                work->emit8 = NULL;
            }
            if (work->emit8 != NULL) {
                work->emit8->target = (s32)&work->target50;
            }
        }
        if (frame >= FX_LANCER_RED_SPARKS_FRAME) {
            t = (frame - FX_LANCER_RED_SPARKS_FRAME) << 10;
            if (t > 0x1000) {
                t = 0x1000;
            }
            ex->btlfx_lerpVec((s16 *)&work->pos40, (s16 *)&work->pos48, t, (s16 *)&work->target58);
            if (work->emit10 != NULL && work->emit10->state == BTLFX_EMIT_DONE) {
                work->emit10 = NULL;
            }
            if (work->emit10 != NULL) {
                work->emit10->target = (s32)&work->target58;
            }
        }
        return frame >= FX_LANCER_FRAMES;
    }
    return 0;
}

static void fxm_posPathInit(Fx191Path *path, u16 *data) {
    data++;
    path->count2 = *data++;
    path->keys4 = (Fx191Point *)data;
}

static s32 fxm_posPathGet(Fx191Path *path, s32 t, SVECTOR *out) {
    s32 last = path->count2 - 1;
    s32 done = 1;
    Fx191Point *key;

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

INCLUDE_RODATA("asm/us/fx_191/nonmatchings/fx_191", fx_191_headerPtr);
