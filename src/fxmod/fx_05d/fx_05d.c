#include "fxmod/fx_05d.h"

INCLUDE_RODATA("asm/us/fx_05d/nonmatchings/fx_05d", fx_05d_headerPtr);

s32 fx_05d_op80(s32 mode, Fx05dWork *work, Unk800F78A4 *env, Unk80100C78Entry *entry, s32 *args) {
    BtlfxExports *ex = BTLFX_EXPORTS(env);
    s32 frame;
    s32 i;
    s32 morphFrom;
    s32 morphTo;
    s32 n;
    s32 morph;
    SVECTOR trackPos;

    if (mode == 0) {
        *args = 0x68;
    } else if (mode == 1) {
        fx_05d_env = env;
        ex->btlfx_initEffModels(fx_05d_effModels, 6);
        ex->btlfx_getUnitPos(0, (s16 *)&work->pos10);
        work->rot18.vx = 0;
        work->rot18.vy = 0;
        work->rot18.vz = 0;
        work->pos20.vx = 0;
        work->pos20.vy = 0;
        work->pos20.vz = 0;
        work->scale30.vx = 0x1000;
        work->scale30.vy = 0x1000;
        work->scale30.vz = 0x1000;
        for (i = 0; i < 6; i++) {
            work->model5C[i] = ex->Hi_RegisterTexEffModel((BtlModelHeader *)ex->btlfx_getFileData(entry->file, fx_05d_morphBlocks[i]),
                                                        0x97, 0x3D40, 0, 0);
        }
        fxm_rotPosTrackInit(&fx_05d_rotPosKeys, (u16 *)ex->btlfx_getFileData(entry->file, 1));
        fxm_rotPosTrackInit(&fx_05d_rotPosKeys2, (u16 *)ex->btlfx_getFileData(entry->file, 0x10));
        fxm_vecTrackInit(&fx_05d_scaleKeys, (u16 *)ex->btlfx_getFileData(entry->file, 2));
        fxm_vecTrackInit(&fx_05d_scaleKeys2, (u16 *)ex->btlfx_getFileData(entry->file, 0xF));
        work->unk58 = -1;
    } else {
        frame = entry->frame;
        switch (frame) {
        case 0:
            work->emit4 = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, 0x14), (s32)&fx_05d_emitPos);
            work->emit0 = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, 0x12), (s32)&work->pos10);
            break;
        case 0x23:
            fxm_rotPosTrackGet(&fx_05d_rotPosKeys, 0x23, NULL, &work->pos4C);
            work->pos4C.vx += work->pos10.vx;
            work->pos4C.vy += work->pos10.vy;
            work->pos4C.vz += work->pos10.vz;
            work->emit8 = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, 0x11), (s32)&work->pos4C);
            work->emitC = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, 0x13), (s32)&work->pos4C);
            if (work->emit8 != NULL) {
                work->emit8->trails = (s32)fx_05d_trails;
                work->emit8->trailSlots = 0x35;
                work->emit8->trailStride = 0x28;
            }
            if (work->emit8 != NULL) {
                work->emit8->otZ = ex->btlfx_addRadiusSixth(0, 0x80);
            }
            if (work->emitC != NULL) {
                work->emitC->otZ = ex->btlfx_addRadiusSixth(0, 0x80);
            }
            break;
        }
        if (frame >= 0 && frame < 0x26) {
            switch (frame) {
            case 0:
            case 2:
            case 4:
            case 6:
            case 8:
                work->morphFrame54 = 0;
                break;
            case 30:
                break;
            }
            if (frame < 2) {
                morphFrom = 0;
                morphTo = 1;
                n = 2;
            } else if (frame < 4) {
                morphFrom = 1;
                morphTo = 2;
                n = 2;
            } else if (frame < 6) {
                morphFrom = 2;
                morphTo = 3;
                n = 2;
            } else if (frame < 8) {
                morphFrom = 3;
                morphTo = 4;
                n = 2;
            } else if (frame < 0x1E) {
                morphFrom = 4;
                morphTo = 5;
                n = 0xC;
            } else {
                morphFrom = 5;
                morphTo = 5;
                n = 0;
            }
            if (morphFrom == morphTo) {
                morph = 0x1000;
            } else if (n < work->morphFrame54) {
                morph = 0;
            } else {
                morph = 0x1000 - (work->morphFrame54 << 12) / n;
            }
            fxm_rotPosTrackGet(&fx_05d_rotPosKeys, frame, &work->rot18, &trackPos);
            work->pos20.vx = trackPos.vx;
            work->pos20.vy = trackPos.vy;
            work->pos20.vz = trackPos.vz;
            work->pos20.vx += work->pos10.vx;
            work->pos20.vy += work->pos10.vy;
            work->pos20.vz += work->pos10.vz;
            env->camera->points[0].vx = work->pos20.vx;
            env->camera->points[0].vy = work->pos20.vy;
            env->camera->points[0].vz = work->pos20.vz;
            fxm_vecTrackGet(&fx_05d_scaleKeys, frame, &work->scale30);
            work->scale30.vx = work->scale30.vx * 2 / 3;
            work->scale30.vy = work->scale30.vy * 2 / 3;
            work->scale30.vz = work->scale30.vz * 2 / 3;
            ex->Hi_DrawMorphEffModel(&work->rot18, &work->pos20, (s32)&work->scale30, work->model5C[morphFrom], work->model5C[morphTo], morph);
            ex->Hi_ModifyEffModelAbr(work->model5C[morphFrom], 1);
            fxm_rotPosTrackGet(&fx_05d_rotPosKeys2, frame, &work->rot18, &trackPos);
            work->pos20.vx = trackPos.vx;
            work->pos20.vy = trackPos.vy;
            work->pos20.vz = trackPos.vz;
            work->pos20.vx += work->pos10.vx;
            work->pos20.vy += work->pos10.vy;
            work->pos20.vz += work->pos10.vz;
            fxm_vecTrackGet(&fx_05d_scaleKeys2, frame, &work->scale30);
            work->rot18.vx -= 0x400;
            work->scale30.vx = work->scale30.vx * 2 / 3;
            work->scale30.vy = work->scale30.vy * 2 / 3;
            work->scale30.vz = work->scale30.vz * 2 / 3;
            ex->Hi_DrawMorphEffModel(&work->rot18, &work->pos20, (s32)&work->scale30, work->model5C[morphFrom], work->model5C[morphTo], morph);
            ex->Hi_ModifyEffModelAbr(work->model5C[morphFrom], 1);
            work->pos44.vx = work->pos20.vx;
            work->pos44.vy = work->pos20.vy;
            work->pos44.vz = work->pos20.vz;
            if (frame >= 0) {
                if (work->emit4 != NULL && work->emit4->state == BTLFX_EMIT_DONE) {
                    work->emit4 = NULL;
                }
                ex->btlfx_moveEmitterTo((s32)work->emit4, (s32)&work->pos44);
            }
            work->morphFrame54++;
        }
        if (frame >= 0x3C) {
            ex->btlfx_clearEffModels();
            return 1;
        }
    }
    return 0;
}

static void fxm_rotPosTrackInit(Fx05dTrack12 *track, u16 *data) {
    track->count0 = *data++;
    track->keys4 = (Fx05dKey12 *)data;
}

static s32 fxm_rotPosTrackGet(Fx05dTrack12 *track, s32 t, SVECTOR *rot, SVECTOR *pos) {
    s32 last = track->count0 - 1;
    s32 done = 1;
    Fx05dKey12 *key;

    if (last < t) {
        t = last;
    } else {
        done = 0;
    }
    key = &track->keys4[t];
    if (rot != NULL) {
        rot->vx = key->rx6;
        rot->vy = key->ry8;
        rot->vz = key->rzA;
    }
    if (pos != NULL) {
        pos->vx = -key->px0;
        pos->vy = key->py2;
        pos->vz = -key->pz4;
    }
    return done;
}

static void fxm_vecTrackInit(Fx05dTrack6 *track, u16 *data) {
    track->count0 = *data++;
    track->keys4 = (Fx05dKey6 *)data;
}

static s32 fxm_vecTrackGet(Fx05dTrack6 *track, s32 t, VECTOR *out) {
    s32 last = track->count0 - 1;
    s32 done = 1;
    Fx05dKey6 *key;

    if (last < t) {
        t = last;
    } else {
        done = 0;
    }
    key = &track->keys4[t];
    out->vx = key->x;
    out->vy = key->y;
    out->vz = key->z;
    return done;
}
