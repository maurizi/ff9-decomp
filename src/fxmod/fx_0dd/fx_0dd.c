#include "fxmod/fx_0dd.h"

INCLUDE_RODATA("asm/us/fx_0dd/nonmatchings/fx_0dd", fx_0dd_headerPtr);

s32 fx_0dd_op80(s32 mode, Fx0ddWork80 *work, Unk800F78A4 *env, Unk80100C78Entry *entry, s32 *args) {
    BtlfxExports *ex = BTLFX_EXPORTS(env);
    SVECTOR keyPos;
    s32 t;
    s32 frame;
    s32 morphFrom;
    s32 morphTo;
    s32 morphLen;
    s32 i;

    if (mode == 0) {
        *args = 0x68;
    } else if (mode == 1) {
        fx_0dd_env = env;
        ex->btlfx_initEffModels(fx_0dd_effModels, 6);
        ex->btlfx_getUnitPos(0, &work->pos10.vx);
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
            work->model5C[i] = ex->Hi_RegisterTexEffModel((BtlModelHeader *)ex->btlfx_getFileData(entry->file, fx_0dd_morphBlocks[i]), 0x97, 0x3D40, 0, 0);
        }
        fxm_rotPosTrackInit(&fx_0dd_rotPosKeys, (u16 *)ex->btlfx_getFileData(entry->file, 1));
        fxm_rotPosTrackInit(&fx_0dd_rotPosKeys2, (u16 *)ex->btlfx_getFileData(entry->file, 0x10));
        fxm_vecTrackInit(&fx_0dd_scaleKeys, (u16 *)ex->btlfx_getFileData(entry->file, 2));
        fxm_vecTrackInit(&fx_0dd_scaleKeys2, (u16 *)ex->btlfx_getFileData(entry->file, 0xF));
        work->unk58 = -1;
    } else {
        frame = entry->frame;
        switch (frame) {
        case 0:
            work->emit4 = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, 0x14), (s32)&fx_0dd_emitPos);
            work->emit0 = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, 0x12), (s32)&work->pos10);
            break;
        case 0x23:
            fxm_rotPosTrackGet(&fx_0dd_rotPosKeys, 0x23, NULL, &work->pos4C);
            work->pos4C.vx += work->pos10.vx;
            work->pos4C.vy += work->pos10.vy;
            work->pos4C.vz += work->pos10.vz;
            work->emit8 = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, 0x11), (s32)&work->pos4C);
            work->emitC = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, 0x13), (s32)&work->pos4C);
            if (work->emit8 != NULL) {
                work->emit8->trails = (s32)fx_0dd_trails;
                work->emit8->trailSlots = 0x35;
                work->emit8->trailStride = 0x28;
                if (work->emit8 != NULL) {
                    work->emit8->otZ = ex->btlfx_addRadiusSixth(0, 0x80);
                }
            }
            if (work->emitC != NULL) {
                work->emitC->otZ = ex->btlfx_addRadiusSixth(0, 0x80);
            }
            break;
        }
        if ((u32)frame < 0x26) {
            switch (frame) {
            case 0:
            case 2:
            case 4:
            case 6:
            case 8:
                work->morphTime54 = 0;
                break;
            case 30:
                break;
            }
            if (frame < 2) {
                morphFrom = 0;
                morphTo = 1;
                morphLen = 2;
            } else if (frame < 4) {
                morphFrom = 1;
                morphTo = 2;
                morphLen = 2;
            } else if (frame < 6) {
                morphFrom = 2;
                morphTo = 3;
                morphLen = 2;
            } else if (frame < 8) {
                morphFrom = 3;
                morphTo = 4;
                morphLen = 2;
            } else if (frame < 0x1E) {
                morphFrom = 4;
                morphTo = 5;
                morphLen = 0xC;
            } else {
                morphFrom = 5;
                morphTo = 5;
                morphLen = 0;
            }
            if (morphFrom == morphTo) {
                t = 0x1000;
            } else if (work->morphTime54 > morphLen) {
                t = 0;
            } else {
                t = 0x1000 - (work->morphTime54 << 12) / morphLen;
            }
            fxm_rotPosTrackGet(&fx_0dd_rotPosKeys, frame, &work->rot18, &keyPos);
            work->pos20.vx = keyPos.vx;
            work->pos20.vy = keyPos.vy;
            work->pos20.vz = keyPos.vz;
            work->pos20.vx += work->pos10.vx;
            work->pos20.vy += work->pos10.vy;
            work->pos20.vz += work->pos10.vz;
            fxm_vecTrackGet(&fx_0dd_scaleKeys, frame, &work->scale30);
            work->scale30.vx = work->scale30.vx * 2 / 3;
            work->scale30.vy = work->scale30.vy * 2 / 3;
            work->scale30.vz = work->scale30.vz * 2 / 3;
            ex->Hi_DrawMorphEffModel(&work->rot18, &work->pos20, (s32)&work->scale30, work->model5C[morphFrom], work->model5C[morphTo], t);
            ex->Hi_ModifyEffModelAbr(work->model5C[morphFrom], 1);
            fxm_rotPosTrackGet(&fx_0dd_rotPosKeys2, frame, &work->rot18, &keyPos);
            work->pos20.vx = keyPos.vx;
            work->pos20.vy = keyPos.vy;
            work->pos20.vz = keyPos.vz;
            work->pos20.vx += work->pos10.vx;
            work->pos20.vy += work->pos10.vy;
            work->pos20.vz += work->pos10.vz;
            fxm_vecTrackGet(&fx_0dd_scaleKeys2, frame, &work->scale30);
            work->rot18.vx -= 0x400;
            work->scale30.vx = work->scale30.vx * 2 / 3;
            work->scale30.vy = work->scale30.vy * 2 / 3;
            work->scale30.vz = work->scale30.vz * 2 / 3;
            ex->Hi_DrawMorphEffModel(&work->rot18, &work->pos20, (s32)&work->scale30, work->model5C[morphFrom], work->model5C[morphTo], t);
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
            work->morphTime54++;
        }
        if (frame >= 0x3C) {
            ex->btlfx_clearEffModels();
            return 1;
        }
    }
    return 0;
}

s32 fx_0dd_op84(s32 mode, Fx0ddWork84 *work, Unk800F78A4 *env, Unk80100C78Entry *entry, s32 *args) {
    BtlfxExports *ex = BTLFX_EXPORTS(env);
    SVECTOR rot;
    s32 frame;
    s32 alpha;
    s32 scale0;
    s32 scale1;
    s32 flicker;
    s32 ang;
    s32 t;
    u16 ry;

    if (mode == 0) {
        *args = 0x10;
    } else {
        if (mode == 1) {
            fx_0dd_env = env;
            work->state0 = 0;
            work->frame4 = 0;
        } else {
            ry = scale1 = scale0 = alpha = 0;
            frame = entry->frame;
            ex->btlfx_getWeaponPos(0x10, 1, (s16 *)&work->pos8);
            switch (work->state0) {
            case 0:
                ry = 0x200;
                alpha = (frame << 12) / 3;
                scale0 = ex->math_sin(alpha >> 2) >> 1;
                scale1 = ((0x1000 - alpha) >> 1) + 0x800;
                if (frame >= 3) {
                    work->state0 = 1;
                    entry->frame = -1;
                }
                break;
            case 1:
                flicker = (frame & 1) << 8;
                scale1 = flicker + 0x1000;
                t = (frame << 12) / 9;
                alpha = (flicker + 0x800) + (t >> 1);
                ry = (t >> 3) + 0x200;
                scale0 = scale1;
                if (frame >= 9) {
                    work->state0 = 2;
                    entry->frame = -1;
                }
                break;
            case 2:
                t = (frame << 12) / 9;
                ang = t >> 2;
                ry = 0x400 - ang;
                alpha = ex->math_cos(ang);
                scale0 = ex->math_sin(ang) + 0x1000;
                scale1 = ex->math_sin(ang) * 3 / 2 + 0x1000;
                if (frame >= 9) {
                    return 1;
                }
                break;
            }
            rot.vx = 0;
            rot.vy = 0;
            rot.vz = ry;
            rot.pad = 0;
            env->otZ = 0x80;
            ex->btlfx_drawRings((u8 *)ex->btlfx_getFileData(entry->file, 0x15), &work->pos8, NULL, scale0 / 3, 0, 1, alpha / 2);
            ex->btlfx_drawRings((u8 *)ex->btlfx_getFileData(entry->file, 0x16), &work->pos8, &rot, scale1 / 2, 0, 1, alpha * 2 / 3);
            env->otZ = 0;
            work->frame4++;
        }
    }
    return 0;
}

static void fxm_rotPosTrackInit(Fx0ddTrack12 *track, u16 *data) {
    track->count0 = *data++;
    track->keys4 = (Fx0ddKey12 *)data;
}

static s32 fxm_rotPosTrackGet(Fx0ddTrack12 *track, s32 t, SVECTOR *rot, SVECTOR *pos) {
    s32 last = track->count0 - 1;
    s32 done = 1;
    Fx0ddKey12 *k;

    if (last < t) {
        t = last;
    } else {
        done = 0;
    }
    k = &track->keys4[t];
    if (rot != NULL) {
        rot->vx = k->rotX6;
        rot->vy = k->rotY8;
        rot->vz = k->rotZA;
    }
    if (pos != NULL) {
        pos->vx = -k->posX0;
        pos->vy = k->posY2;
        pos->vz = -k->posZ4;
    }
    return done;
}

static void fxm_vecTrackInit(Fx0ddTrack6 *track, u16 *data) {
    track->count0 = *data++;
    track->keys4 = (Fx0ddKey6 *)data;
}

static s32 fxm_vecTrackGet(Fx0ddTrack6 *track, s32 t, VECTOR *out) {
    s32 last = track->count0 - 1;
    s32 done = 1;
    Fx0ddKey6 *k;

    if (last < t) {
        t = last;
    } else {
        done = 0;
    }
    k = &track->keys4[t];
    out->vx = k->x;
    out->vy = k->y;
    out->vz = k->z;
    return done;
}
