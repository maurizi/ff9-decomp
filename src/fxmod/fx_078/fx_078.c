#include "fxmod/fx_078.h"

s32 fx_078_op80(s32 mode, Fx078Work *work, Unk800F78A4 *env, Unk80100C78Entry *entry, s32 *arg4) {
    BtlfxExports *ex = BTLFX_EXPORTS(env);
    s32 frame;
    s32 k;
    s32 i;
    SVECTOR p;

    if (mode == 0) {
        *arg4 = 0x60;
    } else if (mode == 1) {
        fx_078_env = env;
        ex->btlfx_initEffModels(fx_078_effModels, 0xA);
        work->pos10.vx = env->targets->centerX;
        work->pos10.vy = env->targets->centerY;
        work->pos10.vz = env->targets->centerZ;
        work->rot18.vx = 0;
        work->rot18.vy = 0;
        work->rot18.vz = 0;
        work->pos20.vx = 0;
        work->pos20.vy = 0;
        work->pos20.vz = 0;
        work->scale30.vx = 0x1000;
        work->scale30.vy = 0x1000;
        work->scale30.vz = 0x1000;
        for (i = 0; i < 10; i++) {
            work->model4C[i] = ex->Hi_RegisterTexEffModel((BtlUnit9C8 *)ex->btlfx_getFileData(entry->file, D_801E7D18[i]),
                                                        0x97, 0x3D40, 0, 0);
        }
        fxm_rotPosTrackInit(&D_801E8EAC, (u16 *)ex->btlfx_getFileData(entry->file, 0xD));
        fxm_vecTrackInit(&D_801E8EB4, (u16 *)ex->btlfx_getFileData(entry->file, 0xC));
        fxm_posTrackInit(&D_801E8EBC, (u16 *)ex->btlfx_getFileData(entry->file, 0x15));
    } else {
        frame = entry->frame;
        switch (frame) {
        case 0:
            work->emit4 = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, 0x14), (s32)&D_801E8EC4);
            work->emit0 = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, 2), (s32)&work->pos10);
            break;
        case 0x26:
            work->emit8 = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, 0x16), (s32)&work->pos10);
            work->emitC = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, 0x17), (s32)&work->pos10);
            if (work->emit8 != NULL) {
                work->emit8->trails = (s32)D_801E854C;
                work->emit8->trailSlots = 0x3C;
                work->emit8->trailStride = 0x28;
            }
            if (work->emit8 != NULL) {
                work->emit8->otZ = 0x80;
            }
            if (work->emitC != NULL) {
                work->emitC->otZ = 0x80;
            }
            break;
        }
        if (frame >= 0 && frame < 0x2D) {
            fxm_rotPosTrackGet(&D_801E8EAC, frame, &work->rot18, &p);
            work->pos20.vx = p.vx;
            work->pos20.vy = p.vy;
            work->pos20.vz = p.vz;
            work->pos20.vx += work->pos10.vx;
            work->pos20.vy += work->pos10.vy;
            work->pos20.vz += work->pos10.vz;
            fxm_vecTrackGet(&D_801E8EB4, frame, &work->scale30);
            k = frame % 10;
            ex->Hi_DrawEffModel(&work->rot18, &work->pos20, (s32)&work->scale30, work->model4C[k]);
            ex->Hi_ModifyEffModelAbr(work->model4C[k], 1);
        }
        fxm_posTrackGet(&D_801E8EBC, frame, &work->unk44);
        work->unk44.vx += work->pos10.vx;
        work->unk44.vy += work->pos10.vy;
        work->unk44.vz += work->pos10.vz;
        if (work->emit4 != NULL && work->emit4->state == BTLFX_EMIT_DONE) {
            work->emit4 = NULL;
        }
        ex->btlfx_moveEmitterTo((s32)work->emit4, (s32)&work->unk44);
        if (frame >= 0x3C) {
            ex->btlfx_clearEffModels();
            return 1;
        }
    }
    return 0;
}

static void fxm_rotPosTrackInit(Fx078Track12 *track, u16 *data) {
    track->unk0 = *data++;
    track->unk4 = (Fx078Key12 *)data;
}

static s32 fxm_rotPosTrackGet(Fx078Track12 *track, s32 t, SVECTOR *rot, SVECTOR *pos) {
    s32 last = track->unk0 - 1;
    s32 done = 1;
    Fx078Key12 *k;

    if (last < t) {
        t = last;
    } else {
        done = 0;
    }
    k = &track->unk4[t];
    if (rot != NULL) {
        rot->vx = k->unk6;
        rot->vy = k->unk8;
        rot->vz = k->unkA;
    }
    if (pos != NULL) {
        pos->vx = -k->unk0;
        pos->vy = k->unk2;
        pos->vz = -k->unk4;
    }
    return done;
}

static void fxm_posTrackInit(Fx078Track6 *track, u16 *data) {
    track->unk0 = *data++;
    track->unk4 = (Fx078Key6 *)data;
}

static s32 fxm_posTrackGet(Fx078Track6 *track, s32 t, SVECTOR *out) {
    s32 last = track->unk0 - 1;
    s32 done = 1;
    Fx078Key6 *k;

    if (last < t) {
        t = last;
    } else {
        done = 0;
    }
    k = &track->unk4[t];
    out->vx = -k->x;
    out->vy = k->y;
    out->vz = -k->z;
    return done;
}

static void fxm_vecTrackInit(Fx078Track6 *track, u16 *data) {
    track->unk0 = *data++;
    track->unk4 = (Fx078Key6 *)data;
}

static s32 fxm_vecTrackGet(Fx078Track6 *track, s32 t, VECTOR *out) {
    s32 last = track->unk0 - 1;
    s32 done = 1;
    Fx078Key6 *k;

    if (last < t) {
        t = last;
    } else {
        done = 0;
    }
    k = &track->unk4[t];
    out->vx = k->x;
    out->vy = k->y;
    out->vz = k->z;
    return done;
}

INCLUDE_RODATA("asm/us/fx_078/nonmatchings/fx_078", fx_078_headerPtr);
