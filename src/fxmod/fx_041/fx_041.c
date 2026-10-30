#include "fxmod/fx_041.h"

s32 func_801E7704(VECTOR *out, s32 arg1, SVECTOR *pos, s32 arg3, s32 t, s32 n) {
    SVECTOR v;

    func_801E7D08(&D_801E7F44, t, &v);
    v.vy = -300;
    D_801E7F54.vy = -300;
    D_801E7F54.vx = v.vx;
    D_801E7F54.vz = v.vz;
    v.vx += D_801E7F4C.vx;
    v.vz += D_801E7F4C.vz;
    pos->vx = v.vx;
    pos->vy = v.vy;
    pos->vz = v.vz;
    out->vx = pos->vx << 12;
    out->vy = pos->vy << 12;
    out->vz = pos->vz << 12;
    if (t >= n) {
        return -1;
    }
    return 0;
}

s32 fx_041_op80(s32 mode, Fx041Work80 *work, Unk800F78A4 *env, Unk80100C78Entry *entry, s32 *arg4) {
    BtlfxExports *ex = BTLFX_EXPORTS(env);
    s32 frame;

    if (mode == 0) {
        *arg4 = 0x30;
    } else if (mode == 1) {
        D_801E7F3C = env;
        ex->btlfx_getUnitBodyPos(0, 0, (s16 *)&work->unk18);
        ex->btlfx_getUnitPos(0, (s16 *)&work->unk20);
        D_801E7F4C.vx = work->unk20.vx;
        D_801E7F4C.vy = work->unk20.vy;
        D_801E7F4C.vz = work->unk20.vz;
        func_801E7CF0(&D_801E7F44, (u16 *)ex->btlfx_getFileData(entry->file, 8));
    } else {
        frame = entry->frame;
        switch (frame) {
        case 3:
            ex->btlfx_getUnitPos(0x10, (s16 *)&work->unk28);
            work->unk0 = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, 7), (s32)&work->unk28);
            work->unk4 = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, 2), (s32)&work->unk20);
            if (work->unk0 != NULL) {
                work->unk0->otZ = ex->btlfx_addRadiusSixth(0x10, 0x20);
            }
            if (work->unk4 != NULL) {
                work->unk4->otZ = ex->btlfx_addRadiusSixth(0x10, 0x20);
            }
            break;
        case 4:
            func_801E7E5C(0);
            work->unk8 = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, 0xF), (s32)&work->unk18);
            work->unkC = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, 0x10), (s32)&work->unk18);
            if (work->unk8 != NULL) {
                work->unk8->otZ = ex->btlfx_addRadiusSixth(0, 0x20);
            }
            if (work->unkC != NULL) {
                work->unkC->otZ = ex->btlfx_addRadiusSixth(0, 0x20);
            }
            break;
        case 0xF:
            ex->btlfx_getUnitPos(0x10, (s16 *)&work->unk28);
            work->unk10 = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, 3), (s32)&work->unk28);
            if (work->unk10 != NULL) {
                work->unk10->otZ = ex->btlfx_addRadiusSixth(0x10, 0x20);
            }
            break;
        case 0x12:
            work->unk14 = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, 4), (s32)&D_801E7F5C);
            if (work->unk14 != NULL) {
                work->unk14->unk14 = (s32)D_801E7F6C;
                work->unk14->unk12 = 1;
                work->unk14->unkC = 0xD0;
            }
            if (work->unk14 != NULL) {
                ((s32 *)work->unk14->file)[2] = (s32)func_801E7704;
            }
            break;
        }
        if (frame >= 4 && func_801E7DD8(0)) {
            func_801E7D6C(0, 0);
        }
        return frame >= 0x28;
    }
    return 0;
}

s32 fx_041_op81(s32 mode, Fx041Work81 *work, Unk800F78A4 *env, Unk80100C78Entry *entry, s32 *arg4) {
    BtlfxExports *ex = BTLFX_EXPORTS(env);

    if (mode == 0) {
        *arg4 = 0x1C;
    } else if (mode == 1) {
        D_801E7F3C = env;
        ex->btlfx_getUnitPos(0, (s16 *)&work->unkC);
        work->unk14.vx = work->unkC.vx;
        work->unk14.vy = work->unkC.vy;
        work->unk14.vz = work->unkC.vz;
        work->unk14.vy = D_801E7F54.vy;
        work->unk0 = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, 5), (s32)&work->unk14);
        work->unk4 = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, 6), (s32)&work->unk14);
        work->unk8 = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, 1), (s32)&work->unk14);
    } else {
        return entry->frame >= 0x28;
    }
    return 0;
}

void func_801E7CF0(Fx041Path *path, u16 *data) {
    data++;
    path->unk2 = *data++;
    path->unk4 = (Fx041Point *)data;
}

s32 func_801E7D08(Fx041Path *path, s32 t, SVECTOR *out) {
    s32 last = path->unk2 - 1;
    s32 done = 1;
    Fx041Point *p;

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

void func_801E7D6C(s32 unit, s32 motion) {
    BtlfxExports *ex = BTLFX_EXPORTS(D_801E7F3C);

    ex->btlfx_setUnitMotion(unit, ex->btlfx_getUnitMotion(unit, motion));
    ex->btlfx_setMotionFrame(unit, 0);
}

s32 func_801E7DD8(s32 unit) {
    BtlfxExports *ex = BTLFX_EXPORTS(D_801E7F3C);

    s32 r;

    if (ex->btlfx_isUnitActive(unit)) {
        r = ex->btlfx_getMotionFrame(unit) == ex->btlfx_getMotionLength(unit) - 1;
    } else {
        r = 0;
    }
    return r;
}

void func_801E7E5C(s32 unit) {
    if (BTLFX_EXPORTS(D_801E7F3C)->btlfx_isUnitActive(unit)) {
        func_801E7D6C(unit, 2);
    }
}

void func_801E7EA8(s32 unit) {
    if (BTLFX_EXPORTS(D_801E7F3C)->btlfx_isUnitActive(unit)) {
        func_801E7D6C(unit, 3);
    }
}

INCLUDE_RODATA("asm/us/fx_041/nonmatchings/fx_041", fx_041_headerPtr);
