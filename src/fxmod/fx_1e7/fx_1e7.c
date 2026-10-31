#include "fxmod/fx_1e7.h"

static s32 fx_1e7_op80ParticleMotion0(Pool30Entry *pt, s32 arg1, SVECTOR *pos, s32 arg3, s32 frame, s32 n) {
    return func_801E776C(pt, pos, &D_801E82E8[0], frame, n);
}

static s32 fx_1e7_op80ParticleMotion1(Pool30Entry *pt, s32 arg1, SVECTOR *pos, s32 arg3, s32 frame, s32 n) {
    return func_801E776C(pt, pos, &D_801E82E8[1], frame, n);
}

s32 func_801E776C(Pool30Entry *e, SVECTOR *pos, Fx1e7Path *path, s32 frame, s32 frames) {
    BtlfxExports *ex = BTLFX_EXPORTS(fx_1e7_env);
    MATRIX m;
    VECTOR o;
    SVECTOR v;

    fxm_posPathGet(path, frame * 4, &v);
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

s32 fx_1e7_op81SpawnPoint0(SVECTOR *out, s32 arg1, s32 i) {
    func_801E79BC(out, &D_801E8300[0], i);
    return 0;
}

s32 fx_1e7_op81SpawnPoint1(SVECTOR *out, s32 arg1, s32 i) {
    func_801E79BC(out, &D_801E8300[1], i);
    return 0;
}

s32 fx_1e7_op81SpawnPoint2(SVECTOR *out, s32 arg1, s32 i) {
    func_801E79BC(out, &D_801E8300[2], i);
    return 0;
}

s32 fx_1e7_op81SpawnPoint3(SVECTOR *out, s32 arg1, s32 i) {
    func_801E79BC(out, &D_801E8300[3], i);
    return 0;
}

s32 fx_1e7_op81SpawnPoint4(SVECTOR *out, s32 arg1, s32 i) {
    func_801E79BC(out, &D_801E8300[4], i);
    return 0;
}

void func_801E79BC(SVECTOR *out, Fx1e7Path *path, s32 t) {
    SVECTOR v;

    fxm_posPathGet(path, t, &v);
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
        fx_1e7_env = env;
        ex->btlfx_getUnitPos(0, (s16 *)&work->pos8);
        work->pos8.vy -= 0x12C;
        D_801E84D0.vx = work->pos8.vx;
        D_801E84D0.vy = work->pos8.vy;
        D_801E84D0.vz = work->pos8.vz;
        fxm_posPathInit(&D_801E82E8[0], (u16 *)ex->btlfx_getFileData(entry->file, 0x13));
        fxm_posPathInit(&D_801E82F0, (u16 *)ex->btlfx_getFileData(entry->file, 0x14));
        work->unk10 = 0x1000;
    } else {
        frame = entry->frame;
        if (frame != 0) {
            if (frame == 3) {
                work->emit0 = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, 0x11), (s32)&D_801E84E8);
                if (work->emit0 != NULL) {
                    work->emit0->trails = (s32)D_801E8330;
                    work->emit0->trailSlots = 1;
                    work->emit0->trailStride = 0xD0;
                }
                if (work->emit0 != NULL) {
                    ((EffMotion *)work->emit0->file)->update = fx_1e7_op80ParticleMotion0;
                }
                if (work->emit0 != NULL) {
                    work->emit0->otZ = ex->btlfx_addRadiusSixth(0, 0x80);
                }
            }
        } else {
            work->emit4 = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, 0x12), (s32)&D_801E84E8);
            if (work->emit4 != NULL) {
                work->emit4->trails = (s32)D_801E8400;
                work->emit4->trailSlots = 1;
                work->emit4->trailStride = 0xD0;
            }
            if (work->emit4 != NULL) {
                ((EffMotion *)work->emit4->file)->update = fx_1e7_op80ParticleMotion1;
            }
            if (work->emit4 != NULL) {
                work->emit4->otZ = ex->btlfx_addRadiusSixth(0, 0x80);
            }
        }
        if (frame == 0xC) {
            ex->btlfx_stopEmitter(work->emit0);
            ex->btlfx_stopEmitter(work->emit4);
        }
        ex->btlfx_aimAngles((s16 *)&env->camera->eye, (s16 *)&work->pos8, (s16 *)&D_801E84E0);
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
        fx_1e7_env = env;
        ex->btlfx_getUnitBodyPos(0, 0, (s16 *)&work->pos20);
        ex->btlfx_getUnitPos(0, (s16 *)&work->pos28);
        D_801E84D8.vx = work->pos28.vx;
        D_801E84D8.vy = work->pos28.vy;
        D_801E84D8.vz = work->pos28.vz;
        work->emit0 = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, 7), (s32)&work->pos28);
        work->emit18 = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, 8), (s32)&work->pos28);
        work->emit1C = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, 1), (s32)&work->pos28);
        if (work->emit0 != NULL) {
            work->emit0->otZ = ex->btlfx_addRadiusSixth(0, 0x20);
        }
        if (work->emit1C != NULL) {
            work->emit1C->otZ = ex->btlfx_addRadiusSixth(0, 0x20);
        }
        for (i = 0; i < 5; i++) {
            fxm_posPathInit(&D_801E8300[i], (u16 *)ex->btlfx_getFileData(entry->file, D_801E8278[i]));
            work->emit4[i] = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, D_801E826C[i]), (s32)&D_801E84F0[i]);
            if (work->emit4[i] != NULL) {
                work->emit4[i]->tail->shape = -1;
                ((EffTailSpawn *)work->emit4[i]->tail)->spawnFn = fx_1e7_op81SpawnPoints[i];
            }
        }
        work->unk30 = 0;
        work->unk34 = ex->btlfx_isUnitActive(0);
        if (work->unk34 != 0) {
            fxm_startMotion3IfActive(0);
        }
    } else {
        frame = entry->frame;
        D_801E82E4 = frame;
        if (work->unk34 != 0 && ex->btlfx_isUnitActive(0) != 0 && fxm_isMotionDone(0) != 0) {
            fxm_startUnitMotion(0, 4);
            work->unk34 = 0;
        }
        ex->btlfx_getUnitPos(0, (s16 *)&work->pos28);
        work->pos28.vz -= 0x64;
        ex->btlfx_setUnitPos(0, (s16 *)&work->pos28);
        return frame >= 0x1E;
    }
    return 0;
}

static void fxm_posPathInit(Fx1e7Path *path, u16 *data) {
    data++;
    path->unk2 = *data++;
    path->unk4 = (Fx1e7Point *)data;
}

static s32 fxm_posPathGet(Fx1e7Path *path, s32 t, SVECTOR *out) {
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

static void fxm_startUnitMotion(s32 unit, s32 motion) {
    BtlfxExports *ex = BTLFX_EXPORTS(fx_1e7_env);

    ex->btlfx_setUnitMotion(unit, ex->btlfx_getUnitMotion(unit, motion));
    ex->btlfx_setMotionFrame(unit, 0);
}

static s32 fxm_isMotionDone(s32 unit) {
    BtlfxExports *ex = BTLFX_EXPORTS(fx_1e7_env);

    s32 r;

    if (ex->btlfx_isUnitActive(unit)) {
        r = ex->btlfx_getMotionFrame(unit) == ex->btlfx_getMotionLength(unit) - 1;
    } else {
        r = 0;
    }
    return r;
}

static void fxm_startMotion2IfActive(s32 unit) {
    if (BTLFX_EXPORTS(fx_1e7_env)->btlfx_isUnitActive(unit)) {
        fxm_startUnitMotion(unit, 2);
    }
}

static void fxm_startMotion3IfActive(s32 unit) {
    if (BTLFX_EXPORTS(fx_1e7_env)->btlfx_isUnitActive(unit)) {
        fxm_startUnitMotion(unit, 3);
    }
}

static void fxm_setSignal(Unk800F78A4 *env, s32 i, s32 v) {
    *((u8 *)env->state + i + 0x10) = v;
}

INCLUDE_RODATA("asm/us/fx_1e7/nonmatchings/fx_1e7", fx_1e7_headerPtr);
