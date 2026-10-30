#include "fxmod/fx_1ac.h"

s32 func_801E77D0(s32 curve, s32 n, s32 t, s32 from, s32 to) {
    BtlfxExports *ex = BTLFX_EXPORTS(D_801E8300);
    s32 k = 0x20000000 / (n * n);
    s32 r = 0;

    switch (curve) {
    case 0:
        r = 0x1000 - (((0x10000000 / n) * t) >> 16);
        break;
    case 1:
        r = (k * (n - t) * (n - t)) >> 17;
        break;
    case 2:
        r = 0x1000 - ((k * t * t) >> 17);
        break;
    }
    return ex->btlfx_lerp(from, to, r);
}

INCLUDE_RODATA("asm/us/fx_1ac/nonmatchings/fx_1ac", fx_1ac_headerPtr);

s32 fx_1ac_op80(s32 mode, Fx1acWork *work, Unk800F78A4 *env, Unk80100C78Entry *entry, s32 *arg4) {
    BtlfxExports *ex = BTLFX_EXPORTS(env);
    SVECTOR v;
    SVECTOR a;
    Pool6CEntry *p;
    s32 frame;
    s32 i;
    s32 t;

    if (mode == 0) {
        *arg4 = 0x60;
    } else if (mode == 1) {
        D_801E8300 = env;
        for (i = 7; i >= 0; i--) {
            work->unk0[i] = 0;
        }
        ex->btlfx_getUnitRot(0, (s16 *)&work->unk38);
        ex->btlfx_getUnitRot(0, (s16 *)&work->unk44);
        work->unk4C[0] = NULL;
        work->unk4C[1] = NULL;
        work->unk4C[2] = NULL;
        work->unk4C[3] = NULL;
        work->unk4C[4] = NULL;
    } else {
        frame = entry->frame;
        if (work->unk4C[0] != NULL && work->unk4C[0]->state == -1) {
            work->unk4C[0] = NULL;
        }
        if (work->unk4C[1] != NULL && work->unk4C[1]->state == -1) {
            work->unk4C[1] = NULL;
        }
        if (work->unk4C[2] != NULL && work->unk4C[2]->state == -1) {
            work->unk4C[2] = NULL;
        }
        if (work->unk4C[3] != NULL && work->unk4C[3]->state == -1) {
            work->unk4C[3] = NULL;
        }
        if (work->unk4C[4] != NULL && work->unk4C[4]->state == -1) {
            work->unk4C[4] = NULL;
        }
        ex->btlfx_getUnitBonePos(0x10, 4, (u16 *)&work->unk30);
        switch (frame) {
        case 0:
            ex->btlfx_getUnitPos(0x10, (s16 *)&work->unk20);
            ex->btlfx_getUnitPos(0, (s16 *)&work->unk28);
            D_801E8304 = p = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, 5), (s32)&work->unk28);
            if (p != NULL) {
                p->unk14 = (s32)D_801E8310;
                p->unk12 = 4;
                p->unkC = 0x9C;
                ((EffMotion *)p->file)->unk8 = func_801E81C4;
            }
            work->unk0[0] = 0xE;
            break;
        case 14:
            ex->btlfx_fadeUnitById(0, 0);
            ex->btlfx_getUnitHomePos(0, (s16 *)&v);
            ex->btlfx_setUnitPos(0, (s16 *)&v);
            break;
        case 20:
            ex->btlfx_getUnitBonePos(0x10, 4, (u16 *)&D_801E8580);
            work->unk4C[0] = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, 1), (s32)&D_801E8580);
            work->unk0[3] = 0x10;
            break;
        case 26:
            ex->btlfx_getUnitBonePos(0x10, 4, (u16 *)&D_801E8588);
            work->unk4C[1] = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, 1), (s32)&D_801E8588);
            work->unk0[4] = 0x10;
            break;
        case 32:
            ex->btlfx_getUnitBonePos(0x10, 4, (u16 *)&D_801E8590);
            work->unk4C[2] = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, 1), (s32)&D_801E8590);
            work->unk0[5] = 0x10;
            break;
        case 46:
            ex->btlfx_getUnitBonePos(0x10, 4, (u16 *)&D_801E8598);
            work->unk4C[3] = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, 2), (s32)&D_801E8598);
            work->unk0[6] = 0x10;
            break;
        case 50:
            work->unk0[2] = 2;
            ex->btlfx_fadeUnitById(0, 0x80);
            ex->btlfx_setUnitRot(0, (s16 *)&work->unk44);
            ex->btlfx_getUnitHomePos(0, (s16 *)&D_801E85A0);
            work->unk4C[4] = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, 3), (s32)&D_801E85A0);
            work->unk0[7] = 0x10;
            break;
        }
        if (work->unk0[0] != 0) {
            work->unk0[0]--;
            t = 0xE - work->unk0[0];
            D_801E85A8.vx = ex->btlfx_lerpRatio(work->unk28.vx, work->unk30.vx, t, 0xE);
            D_801E85A8.vz = ex->btlfx_lerpRatio(work->unk28.vz, work->unk30.vz, t, 0xE);
            D_801E85A8.vy = work->unk28.vy - (t * 0x258 - (t * 0x25) * t);
            ex->btlfx_setUnitPos(0, (s16 *)&D_801E85A8);
            work->unk38.vx -= 0xAA;
            ex->btlfx_setUnitRot(0, (s16 *)&work->unk38);
        }
        if (work->unk0[3] != 0) {
            MATRIX m;

            work->unk0[3]--;
            ex->btlfx_getUnitBoneMatrix(0x10, 4, (BtlBone *)&m);
            ex->btlfx_matrixAngles(&m, (s16 *)&a);
            if (work->unk4C[0] != NULL) {
                work->unk4C[0]->rot.vx = a.vx;
                work->unk4C[0]->rot.vy = a.vy;
                work->unk4C[0]->rot.vz = a.vz;
            }
            ex->btlfx_getUnitBonePos(0x10, 4, (u16 *)&v);
            ex->btlfx_moveEmitterTo((s32)work->unk4C[0], (s32)&v);
        }
        if (work->unk0[4] != 0) {
            MATRIX m;

            work->unk0[4]--;
            ex->btlfx_getUnitBoneMatrix(0x10, 4, (BtlBone *)&m);
            ex->btlfx_matrixAngles(&m, (s16 *)&a);
            if (work->unk4C[1] != NULL) {
                work->unk4C[1]->rot.vx = a.vx;
                work->unk4C[1]->rot.vy = a.vy;
                work->unk4C[1]->rot.vz = a.vz;
            }
            ex->btlfx_getUnitBonePos(0x10, 4, (u16 *)&v);
            ex->btlfx_moveEmitterTo((s32)work->unk4C[1], (s32)&v);
        }
        if (work->unk0[5] != 0) {
            MATRIX m;

            work->unk0[5]--;
            ex->btlfx_getUnitBoneMatrix(0x10, 4, (BtlBone *)&m);
            ex->btlfx_matrixAngles(&m, (s16 *)&a);
            if (work->unk4C[2] != NULL) {
                work->unk4C[2]->rot.vx = a.vx;
                work->unk4C[2]->rot.vy = a.vy;
                work->unk4C[2]->rot.vz = a.vz;
            }
            ex->btlfx_getUnitBonePos(0x10, 4, (u16 *)&v);
            ex->btlfx_moveEmitterTo((s32)work->unk4C[2], (s32)&v);
        }
        if (work->unk0[6] != 0) {
            MATRIX m;

            work->unk0[6]--;
            ex->btlfx_getUnitBoneMatrix(0x10, 4, (BtlBone *)&m);
            ex->btlfx_matrixAngles(&m, (s16 *)&a);
            if (work->unk4C[3] != NULL) {
                work->unk4C[3]->rot.vx = a.vx;
                work->unk4C[3]->rot.vy = a.vy;
                work->unk4C[3]->rot.vz = a.vz;
            }
            ex->btlfx_getUnitBonePos(0x10, 4, (u16 *)&v);
            ex->btlfx_moveEmitterTo((s32)work->unk4C[3], (s32)&v);
        }
        if (work->unk0[7] != 0) {
            work->unk0[7]--;
            a.vx = 0xC00;
            a.vz = 0;
            a.vy = 0;
            if (work->unk4C[4] != NULL) {
                work->unk4C[4]->rot.vx = a.vx;
                work->unk4C[4]->rot.vy = a.vy;
                work->unk4C[4]->rot.vz = a.vz;
            }
        }
        if (work->unk0[2] != 0) {
            work->unk0[2]--;
            if (work->unk0[2] != 0) {
                ex->btlfx_lerpVec((s16 *)&work->unk28, (s16 *)&work->unk20, (work->unk0[2] << 23) >> 12, (s16 *)&v);
            } else {
                ex->btlfx_getUnitHomePos(0, (s16 *)&v);
            }
            ex->btlfx_setUnitPos(0, (s16 *)&v);
        }
        return frame >= 0x46;
    }
    return 0;
}

s32 func_801E81C4(Pool30Entry *e, Pool6CEntry *owner, SVECTOR *pos, s32 arg3, s32 frame, s32 frames) {
    SVECTOR v;

    switch (e->unk1F - 1) {
    case 0: {
        SVECTOR *src = &D_801E85A8;
        s16 x = src->vx;
        s16 y = src->vy;
        s16 z = src->vz;

        /* both cases copy through their own locals; jump2's cross-jumping merges the identical tails */
        v.vx = x;
        v.vy = y;
        v.vz = z;
        break;
    }
    default: {
        SVECTOR *src = &D_801E85B0;
        s16 x = src->vx;
        s16 y = src->vy;
        s16 z = src->vz;

        v.vx = x;
        v.vy = y;
        v.vz = z;
        break;
    }
    }
    D_801E85B0.vx = e->pos.vx >> 12;
    D_801E85B0.vy = e->pos.vy >> 12;
    D_801E85B0.vz = e->pos.vz >> 12;
    pos->vx = v.vx;
    pos->vy = v.vy;
    pos->vz = v.vz;
    e->pos.vx = v.vx << 12;
    e->pos.vy = v.vy << 12;
    e->pos.vz = v.vz << 12;
    if (frame >= frames) {
        return -1;
    }
    return 0;
}
