#include "fxmod/fx_0dd.h"

INCLUDE_RODATA("asm/us/fx_0dd/nonmatchings/fx_0dd", fx_0dd_headerPtr);

s32 fx_0dd_op80(s32 mode, Fx0ddWork80 *work, Unk800F78A4 *env, Unk80100C78Entry *entry, s32 *arg4) {
    BtlfxExports *ex = BTLFX_EXPORTS(env);
    SVECTOR v;
    s32 t;
    s32 frame;
    s32 a;
    s32 b;
    s32 n;
    s32 i;

    if (mode == 0) {
        *arg4 = 0x68;
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
            work->model5C[i] = ex->Hi_RegisterTexEffModel((BtlUnit9C8 *)ex->btlfx_getFileData(entry->file, D_801E8360[i]), 0x97, 0x3D40, 0, 0);
        }
        fxm_rotPosTrackInit(&D_801E90B8, (u16 *)ex->btlfx_getFileData(entry->file, 1));
        fxm_rotPosTrackInit(&D_801E90C0, (u16 *)ex->btlfx_getFileData(entry->file, 0x10));
        fxm_vecTrackInit(&D_801E90C8, (u16 *)ex->btlfx_getFileData(entry->file, 2));
        fxm_vecTrackInit(&D_801E90D0, (u16 *)ex->btlfx_getFileData(entry->file, 0xF));
        work->unk58 = -1;
    } else {
        frame = entry->frame;
        switch (frame) {
        case 0:
            work->emit4 = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, 0x14), (s32)&D_801E90D8);
            work->emit0 = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, 0x12), (s32)&work->pos10);
            break;
        case 0x23:
            fxm_rotPosTrackGet(&D_801E90B8, 0x23, NULL, &work->pos4C);
            work->pos4C.vx += work->pos10.vx;
            work->pos4C.vy += work->pos10.vy;
            work->pos4C.vz += work->pos10.vz;
            work->emit8 = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, 0x11), (s32)&work->pos4C);
            work->emitC = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, 0x13), (s32)&work->pos4C);
            if (work->emit8 != NULL) {
                work->emit8->unk14 = (s32)D_801E8870;
                work->emit8->unk12 = 0x35;
                work->emit8->unkC = 0x28;
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
                work->unk54 = 0;
                break;
            case 30:
                break;
            }
            if (frame < 2) {
                a = 0;
                b = 1;
                n = 2;
            } else if (frame < 4) {
                a = 1;
                b = 2;
                n = 2;
            } else if (frame < 6) {
                a = 2;
                b = 3;
                n = 2;
            } else if (frame < 8) {
                a = 3;
                b = 4;
                n = 2;
            } else if (frame < 0x1E) {
                a = 4;
                b = 5;
                n = 0xC;
            } else {
                a = 5;
                b = 5;
                n = 0;
            }
            if (a == b) {
                t = 0x1000;
            } else if (work->unk54 > n) {
                t = 0;
            } else {
                t = 0x1000 - (work->unk54 << 12) / n;
            }
            fxm_rotPosTrackGet(&D_801E90B8, frame, &work->rot18, &v);
            work->pos20.vx = v.vx;
            work->pos20.vy = v.vy;
            work->pos20.vz = v.vz;
            work->pos20.vx += work->pos10.vx;
            work->pos20.vy += work->pos10.vy;
            work->pos20.vz += work->pos10.vz;
            fxm_vecTrackGet(&D_801E90C8, frame, &work->scale30);
            work->scale30.vx = work->scale30.vx * 2 / 3;
            work->scale30.vy = work->scale30.vy * 2 / 3;
            work->scale30.vz = work->scale30.vz * 2 / 3;
            ex->Hi_DrawMorphEffModel(&work->rot18, &work->pos20, (s32)&work->scale30, work->model5C[a], work->model5C[b], t);
            ex->Hi_ModifyEffModelAbr(work->model5C[a], 1);
            fxm_rotPosTrackGet(&D_801E90C0, frame, &work->rot18, &v);
            work->pos20.vx = v.vx;
            work->pos20.vy = v.vy;
            work->pos20.vz = v.vz;
            work->pos20.vx += work->pos10.vx;
            work->pos20.vy += work->pos10.vy;
            work->pos20.vz += work->pos10.vz;
            fxm_vecTrackGet(&D_801E90D0, frame, &work->scale30);
            work->rot18.vx -= 0x400;
            work->scale30.vx = work->scale30.vx * 2 / 3;
            work->scale30.vy = work->scale30.vy * 2 / 3;
            work->scale30.vz = work->scale30.vz * 2 / 3;
            ex->Hi_DrawMorphEffModel(&work->rot18, &work->pos20, (s32)&work->scale30, work->model5C[a], work->model5C[b], t);
            ex->Hi_ModifyEffModelAbr(work->model5C[a], 1);
            work->unk44.vx = work->pos20.vx;
            work->unk44.vy = work->pos20.vy;
            work->unk44.vz = work->pos20.vz;
            if (frame >= 0) {
                if (work->emit4 != NULL && work->emit4->state == BTLFX_EMIT_DONE) {
                    work->emit4 = NULL;
                }
                ex->btlfx_moveEmitterTo((s32)work->emit4, (s32)&work->unk44);
            }
            work->unk54++;
        }
        if (frame >= 0x3C) {
            ex->btlfx_clearEffModels();
            return 1;
        }
    }
    return 0;
}

s32 fx_0dd_op84(s32 mode, Fx0ddWork84 *work, Unk800F78A4 *env, Unk80100C78Entry *entry, s32 *arg4) {
    BtlfxExports *ex = BTLFX_EXPORTS(env);
    SVECTOR rot;
    s32 frame;
    s32 a;
    s32 b;
    s32 c;
    s32 k;
    s32 t;
    s32 d;
    u16 r;

    if (mode == 0) {
        *arg4 = 0x10;
    } else {
        if (mode == 1) {
            fx_0dd_env = env;
            work->unk0 = 0;
            work->unk4 = 0;
        } else {
            r = c = b = a = 0;
            frame = entry->frame;
            ex->btlfx_getWeaponPos(0x10, 1, (s16 *)&work->unk8);
            switch (work->unk0) {
            case 0:
                r = 0x200;
                a = (frame << 12) / 3;
                b = ex->math_sin(a >> 2) >> 1;
                c = ((0x1000 - a) >> 1) + 0x800;
                if (frame >= 3) {
                    work->unk0 = 1;
                    entry->frame = -1;
                }
                break;
            case 1:
                k = (frame & 1) << 8;
                c = k + 0x1000;
                d = (frame << 12) / 9;
                a = (k + 0x800) + (d >> 1);
                r = (d >> 3) + 0x200;
                b = c;
                if (frame >= 9) {
                    work->unk0 = 2;
                    entry->frame = -1;
                }
                break;
            case 2:
                d = (frame << 12) / 9;
                t = d >> 2;
                r = 0x400 - t;
                a = ex->math_cos(t);
                b = ex->math_sin(t) + 0x1000;
                c = ex->math_sin(t) * 3 / 2 + 0x1000;
                if (frame >= 9) {
                    return 1;
                }
                break;
            }
            rot.vx = 0;
            rot.vy = 0;
            rot.vz = r;
            rot.pad = 0;
            env->otZ = 0x80;
            ex->btlfx_drawRings((u8 *)ex->btlfx_getFileData(entry->file, 0x15), &work->unk8, NULL, b / 3, 0, 1, a / 2);
            ex->btlfx_drawRings((u8 *)ex->btlfx_getFileData(entry->file, 0x16), &work->unk8, &rot, c / 2, 0, 1, a * 2 / 3);
            env->otZ = 0;
            work->unk4++;
        }
    }
    return 0;
}

static void fxm_rotPosTrackInit(Fx0ddTrack12 *track, u16 *data) {
    track->unk0 = *data++;
    track->unk4 = (Fx0ddKey12 *)data;
}

static s32 fxm_rotPosTrackGet(Fx0ddTrack12 *track, s32 t, SVECTOR *rot, SVECTOR *pos) {
    s32 last = track->unk0 - 1;
    s32 done = 1;
    Fx0ddKey12 *k;

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

static void fxm_vecTrackInit(Fx0ddTrack6 *track, u16 *data) {
    track->unk0 = *data++;
    track->unk4 = (Fx0ddKey6 *)data;
}

static s32 fxm_vecTrackGet(Fx0ddTrack6 *track, s32 t, VECTOR *out) {
    s32 last = track->unk0 - 1;
    s32 done = 1;
    Fx0ddKey6 *k;

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
