#include "fxmod/fx_1e7.h"

s32 func_801E7704(Pool30Entry *pt, s32 arg1, SVECTOR *pos, s32 arg3, s32 frame, s32 n) {
    return func_801E776C(pt, pos, &D_801E82E8[0], frame, n);
}

s32 func_801E7738(Pool30Entry *pt, s32 arg1, SVECTOR *pos, s32 arg3, s32 frame, s32 n) {
    return func_801E776C(pt, pos, &D_801E82E8[1], frame, n);
}

s32 func_801E776C(Pool30Entry *e, SVECTOR *pos, Fx1e7Path *path, s32 frame, s32 frames) {
    BtlfxExports *ex = BTLFX_EXPORTS(D_801E82E0);
    MATRIX m;
    VECTOR o;
    SVECTOR v;

    func_801E806C(path, frame * 4, &v);
    if (path == D_801E82E8) {
        v.vx += 0x50;
    }
    ex->math_rotMatrixYXZ(&D_801E84E0, &m);
    gte_SetRotMatrix(&m);
    gte_ldtr0();
    gte_ldv0(&v);
    gte_rtv0tr();
    gte_stlvnl(&o);
    o.vx += D_801E84D0.vx;
    o.vy += D_801E84D0.vy;
    o.vz += D_801E84D0.vz;
    pos->vx = o.vx;
    pos->vy = o.vy;
    pos->vz = o.vz;
    e->pos.vx = pos->vx << 12;
    e->pos.vy = pos->vy << 12;
    e->pos.vz = pos->vz << 12;
    if (frame >= frames) {
        return -1;
    }
    return 0;
}

s32 func_801E7908(SVECTOR *out, s32 arg1, s32 i) {
    func_801E79BC(out, &D_801E8300[0], i);
    return 0;
}

s32 func_801E792C(SVECTOR *out, s32 arg1, s32 i) {
    func_801E79BC(out, &D_801E8300[1], i);
    return 0;
}

s32 func_801E7950(SVECTOR *out, s32 arg1, s32 i) {
    func_801E79BC(out, &D_801E8300[2], i);
    return 0;
}

s32 func_801E7974(SVECTOR *out, s32 arg1, s32 i) {
    func_801E79BC(out, &D_801E8300[3], i);
    return 0;
}

s32 func_801E7998(SVECTOR *out, s32 arg1, s32 i) {
    func_801E79BC(out, &D_801E8300[4], i);
    return 0;
}

void func_801E79BC(SVECTOR *out, Fx1e7Path *path, s32 t) {
    SVECTOR v;

    func_801E806C(path, t, &v);
    v.vx += D_801E84D8.vx;
    v.vy += D_801E84D8.vy;
    v.vz += D_801E84D8.vz;
    out->vx = v.vx;
    out->vy = v.vy;
    out->vz = v.vz;
}

s32 fx_1e7_op80(s32 mode, Fx1e7Work80 *work, Unk800F78A4 *env, Unk80100C78Entry *entry, s32 *arg4) {
    BtlfxExports *ex = BTLFX_EXPORTS(env);
    s32 frame;

    if (mode == 0) {
        *arg4 = 0x14;
    } else if (mode == 1) {
        D_801E82E0 = env;
        ex->btlfx_getUnitPos(0, (s16 *)&work->unk8);
        work->unk8.vy -= 0x12C;
        D_801E84D0.vx = work->unk8.vx;
        D_801E84D0.vy = work->unk8.vy;
        D_801E84D0.vz = work->unk8.vz;
        func_801E8054(&D_801E82E8[0], (u16 *)ex->btlfx_getFileData(entry->file, 0x13));
        func_801E8054(&D_801E82F0, (u16 *)ex->btlfx_getFileData(entry->file, 0x14));
        work->unk10 = 0x1000;
    } else {
        frame = entry->frame;
        if (frame != 0) {
            if (frame == 3) {
                work->unk0 = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, 0x11), (s32)&D_801E84E8);
                if (work->unk0 != NULL) {
                    work->unk0->unk14 = (s32)D_801E8330;
                    work->unk0->unk12 = 1;
                    work->unk0->unkC = 0xD0;
                }
                if (work->unk0 != NULL) {
                    ((EffMotion *)work->unk0->file)->unk8 = func_801E7704;
                }
                if (work->unk0 != NULL) {
                    work->unk0->unk22 = ex->func_800E7CC4(0, 0x80);
                }
            }
        } else {
            work->unk4 = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, 0x12), (s32)&D_801E84E8);
            if (work->unk4 != NULL) {
                work->unk4->unk14 = (s32)D_801E8400;
                work->unk4->unk12 = 1;
                work->unk4->unkC = 0xD0;
            }
            if (work->unk4 != NULL) {
                ((EffMotion *)work->unk4->file)->unk8 = func_801E7738;
            }
            if (work->unk4 != NULL) {
                work->unk4->unk22 = ex->func_800E7CC4(0, 0x80);
            }
        }
        if (frame == 0xC) {
            ex->btlfx_stopEmitter(work->unk0);
            ex->btlfx_stopEmitter(work->unk4);
        }
        ex->btlfx_aimAngles((s16 *)&env->camera->eye, (s16 *)&work->unk8, (s16 *)&D_801E84E0);
        return frame >= 0x3C;
    }
    return 0;
}

s32 fx_1e7_op81(s32 mode, Fx1e7Work81 *work, Unk800F78A4 *env, Unk80100C78Entry *entry, s32 *arg4) {
    BtlfxExports *ex = BTLFX_EXPORTS(env);
    s32 frame;
    s32 i;

    if (mode == 0) {
        *arg4 = 0x38;
    } else if (mode == 1) {
        D_801E82E0 = env;
        ex->btlfx_getUnitBodyPos(0, 0, (s16 *)&work->unk20);
        ex->btlfx_getUnitPos(0, (s16 *)&work->unk28);
        D_801E84D8.vx = work->unk28.vx;
        D_801E84D8.vy = work->unk28.vy;
        D_801E84D8.vz = work->unk28.vz;
        work->unk0 = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, 7), (s32)&work->unk28);
        work->unk18 = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, 8), (s32)&work->unk28);
        work->unk1C = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, 1), (s32)&work->unk28);
        if (work->unk0 != NULL) {
            work->unk0->unk22 = ex->func_800E7CC4(0, 0x20);
        }
        if (work->unk1C != NULL) {
            work->unk1C->unk22 = ex->func_800E7CC4(0, 0x20);
        }
        for (i = 0; i < 5; i++) {
            func_801E8054(&D_801E8300[i], (u16 *)ex->btlfx_getFileData(entry->file, D_801E8278[i]));
            work->unk4[i] = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, D_801E826C[i]), (s32)&D_801E84F0[i]);
            if (work->unk4[i] != NULL) {
                work->unk4[i]->tail->unk0 = -1;
                ((EffTailSpawn *)work->unk4[i]->tail)->unk3C = D_801E8284[i];
            }
        }
        work->unk30 = 0;
        work->unk34 = ex->btlfx_isUnitActive(0);
        if (work->unk34 != 0) {
            func_801E820C(0);
        }
    } else {
        frame = entry->frame;
        D_801E82E4 = frame;
        if (work->unk34 != 0 && ex->btlfx_isUnitActive(0) != 0 && func_801E813C(0) != 0) {
            func_801E80D0(0, 4);
            work->unk34 = 0;
        }
        ex->btlfx_getUnitPos(0, (s16 *)&work->unk28);
        work->unk28.vz -= 0x64;
        ex->btlfx_setUnitPos(0, (s16 *)&work->unk28);
        return frame >= 0x1E;
    }
    return 0;
}

void func_801E8054(Fx1e7Path *path, u16 *data) {
    data++;
    path->unk2 = *data++;
    path->unk4 = (Fx1e7Point *)data;
}

s32 func_801E806C(Fx1e7Path *path, s32 t, SVECTOR *out) {
    s32 last = path->unk2 - 1;
    s32 done = 1;
    Fx1e7Point *p;

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

void func_801E80D0(s32 unit, s32 motion) {
    BtlfxExports *ex = BTLFX_EXPORTS(D_801E82E0);

    ex->btlfx_setUnitMotion(unit, ex->btlfx_getUnitMotion(unit, motion));
    ex->btlfx_setMotionFrame(unit, 0);
}

s32 func_801E813C(s32 unit) {
    BtlfxExports *ex = BTLFX_EXPORTS(D_801E82E0);

    s32 r;

    if (ex->btlfx_isUnitActive(unit)) {
        r = ex->btlfx_getMotionFrame(unit) == ex->btlfx_getMotionLength(unit) - 1;
    } else {
        r = 0;
    }
    return r;
}

void func_801E81C0(s32 unit) {
    if (BTLFX_EXPORTS(D_801E82E0)->btlfx_isUnitActive(unit)) {
        func_801E80D0(unit, 2);
    }
}

void func_801E820C(s32 unit) {
    if (BTLFX_EXPORTS(D_801E82E0)->btlfx_isUnitActive(unit)) {
        func_801E80D0(unit, 3);
    }
}

void func_801E8258(Unk800F78A4 *env, s32 i, s32 v) {
    *((u8 *)env->state + i + 0x10) = v;
}

INCLUDE_RODATA("asm/us/fx_1e7/nonmatchings/fx_1e7", fx_1e7_headerPtr);
