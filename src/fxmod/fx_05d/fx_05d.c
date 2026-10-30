#include "fxmod/fx_05d.h"

INCLUDE_RODATA("asm/us/fx_05d/nonmatchings/fx_05d", fx_05d_headerPtr);

s32 fx_05d_op80(s32 mode, Fx05dWork *work, Unk800F78A4 *env, Unk80100C78Entry *entry, s32 *arg4) {
    BtlfxExports *ex = BTLFX_EXPORTS(env);
    s32 frame;
    s32 i;
    s32 a;
    s32 b;
    s32 n;
    s32 w;
    SVECTOR p;

    if (mode == 0) {
        *arg4 = 0x68;
    } else if (mode == 1) {
        D_801E8098 = env;
        ex->btlfx_initEffModels(D_801E80A8, 6);
        ex->btlfx_getUnitPos(0, (s16 *)&work->unk10);
        work->unk18.vx = 0;
        work->unk18.vy = 0;
        work->unk18.vz = 0;
        work->unk20.vx = 0;
        work->unk20.vy = 0;
        work->unk20.vz = 0;
        work->unk30.vx = 0x1000;
        work->unk30.vy = 0x1000;
        work->unk30.vz = 0x1000;
        for (i = 0; i < 6; i++) {
            work->unk5C[i] = ex->Hi_RegisterTexEffModel((BtlUnit9C8 *)ex->btlfx_getFileData(entry->file, D_801E8048[i]),
                                                        0x97, 0x3D40, 0, 0);
        }
        func_801E7F20(&D_801E8DA0, (u16 *)ex->btlfx_getFileData(entry->file, 1));
        func_801E7F20(&D_801E8DA8, (u16 *)ex->btlfx_getFileData(entry->file, 0x10));
        func_801E7FD0(&D_801E8DB0, (u16 *)ex->btlfx_getFileData(entry->file, 2));
        func_801E7FD0(&D_801E8DB8, (u16 *)ex->btlfx_getFileData(entry->file, 0xF));
        work->unk58 = -1;
    } else {
        frame = entry->frame;
        switch (frame) {
        case 0:
            work->unk4 = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, 0x14), (s32)&D_801E8DC0);
            work->unk0 = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, 0x12), (s32)&work->unk10);
            break;
        case 0x23:
            func_801E7F34(&D_801E8DA0, 0x23, NULL, &work->unk4C);
            work->unk4C.vx += work->unk10.vx;
            work->unk4C.vy += work->unk10.vy;
            work->unk4C.vz += work->unk10.vz;
            work->unk8 = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, 0x11), (s32)&work->unk4C);
            work->unkC = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, 0x13), (s32)&work->unk4C);
            if (work->unk8 != NULL) {
                work->unk8->unk14 = (s32)D_801E8558;
                work->unk8->unk12 = 0x35;
                work->unk8->unkC = 0x28;
            }
            if (work->unk8 != NULL) {
                work->unk8->unk22 = ex->func_800E7CC4(0, 0x80);
            }
            if (work->unkC != NULL) {
                work->unkC->unk22 = ex->func_800E7CC4(0, 0x80);
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
                w = 0x1000;
            } else if (n < work->unk54) {
                w = 0;
            } else {
                w = 0x1000 - (work->unk54 << 12) / n;
            }
            func_801E7F34(&D_801E8DA0, frame, &work->unk18, &p);
            work->unk20.vx = p.vx;
            work->unk20.vy = p.vy;
            work->unk20.vz = p.vz;
            work->unk20.vx += work->unk10.vx;
            work->unk20.vy += work->unk10.vy;
            work->unk20.vz += work->unk10.vz;
            env->camera->points[0].vx = work->unk20.vx;
            env->camera->points[0].vy = work->unk20.vy;
            env->camera->points[0].vz = work->unk20.vz;
            func_801E7FE4(&D_801E8DB0, frame, &work->unk30);
            work->unk30.vx = work->unk30.vx * 2 / 3;
            work->unk30.vy = work->unk30.vy * 2 / 3;
            work->unk30.vz = work->unk30.vz * 2 / 3;
            ex->Hi_DrawMorphEffModel(&work->unk18, &work->unk20, (s32)&work->unk30, work->unk5C[a], work->unk5C[b], w);
            ex->Hi_ModifyEffModelAbr(work->unk5C[a], 1);
            func_801E7F34(&D_801E8DA8, frame, &work->unk18, &p);
            work->unk20.vx = p.vx;
            work->unk20.vy = p.vy;
            work->unk20.vz = p.vz;
            work->unk20.vx += work->unk10.vx;
            work->unk20.vy += work->unk10.vy;
            work->unk20.vz += work->unk10.vz;
            func_801E7FE4(&D_801E8DB8, frame, &work->unk30);
            work->unk18.vx -= 0x400;
            work->unk30.vx = work->unk30.vx * 2 / 3;
            work->unk30.vy = work->unk30.vy * 2 / 3;
            work->unk30.vz = work->unk30.vz * 2 / 3;
            ex->Hi_DrawMorphEffModel(&work->unk18, &work->unk20, (s32)&work->unk30, work->unk5C[a], work->unk5C[b], w);
            ex->Hi_ModifyEffModelAbr(work->unk5C[a], 1);
            work->unk44.vx = work->unk20.vx;
            work->unk44.vy = work->unk20.vy;
            work->unk44.vz = work->unk20.vz;
            if (frame >= 0) {
                if (work->unk4 != NULL && work->unk4->state == -1) {
                    work->unk4 = NULL;
                }
                ex->btlfx_moveEmitterTo((s32)work->unk4, (s32)&work->unk44);
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

void func_801E7F20(Fx05dTrack12 *track, u16 *data) {
    track->unk0 = *data++;
    track->unk4 = (Fx05dKey12 *)data;
}

s32 func_801E7F34(Fx05dTrack12 *track, s32 t, SVECTOR *rot, SVECTOR *pos) {
    s32 last = track->unk0 - 1;
    s32 done = 1;
    Fx05dKey12 *k;

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

void func_801E7FD0(Fx05dTrack6 *track, u16 *data) {
    track->unk0 = *data++;
    track->unk4 = (Fx05dKey6 *)data;
}

s32 func_801E7FE4(Fx05dTrack6 *track, s32 t, VECTOR *out) {
    s32 last = track->unk0 - 1;
    s32 done = 1;
    Fx05dKey6 *k;

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
