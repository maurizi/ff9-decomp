#include "fxmod/fx_0ec.h"

static s32 fx_0ec_op80PoolTask(s32 mode, SVECTOR *pos, Unk800F78A4 *env, Unk80100C78Entry *entry, s32 frame) {
    BtlfxExports *ex = BTLFX_EXPORTS(env);
    SVECTOR rot;
    s32 t;
    u8 *d;

    if (mode != 1) {
        t = (frame << 12) / 6;
        rot.vx = 0x400;
        rot.vy = 0;
        rot.vz = 0;
        rot.pad = 1;
        d = (u8 *)ex->btlfx_getFileData(entry->file, 6);
        ex->btlfx_drawParticles(d, pos, &rot, (ex->math_sin(t >> 2) >> 1) + 0x400, 0, 1, 0x1000 - t, 0, 0);
        if (frame >= 6) {
            return 1;
        }
    }
    return 0;
}

void fx_0ec_spawnImpact(SVECTOR *pos) {
    Unk800F78A4 *env = fx_0ec_env;
    BtlfxExports *ex = BTLFX_EXPORTS(env);
    SVECTOR *p = ex->btlfx_poolSpawn((s16 *)fx_0ec_task->pool10);
    MATRIX m;
    VECTOR v;

    if (p != NULL) {
        gte_SetRotMatrix(env->viewMatrix);
        gte_SetTransMatrix(env->viewMatrix);
        ex->math_rotMatrixYXZ(&fx_0ec_modelRot, &m);
        ex->ScaleMatrix(&m, &fx_0ec_modelScale);
        m.t[0] = fx_0ec_modelPos.vx;
        m.t[1] = fx_0ec_modelPos.vy;
        m.t[2] = fx_0ec_modelPos.vz;
        gte_SetRotMatrix(&m);
        gte_SetTransMatrix(&m);
        gte_ldv0(pos);
        gte_rtv0tr();
        gte_stlvnl(&v);
        v.vy = 0;
        p->vx = v.vx;
        p->vy = v.vy;
        p->vz = v.vz;
    }
}

static void fxm_shatterModel(s32 model, SVECTOR *center) {
    Fx0ecTri *t = fx_0ec_shatterTris;
    Fx0ecTri *u = fx_0ec_shatterTris;
    BtlfxExports *ex = BTLFX_EXPORTS(fx_0ec_env);
    Fx0ecShard *p;
    SVECTOR d;
    s32 i;
    s32 k;
    s32 r;

    ex->Hi_SplitMdlVertex(model, -1, (s32)t);
    p = fx_0ec_shatterShards;
    for (i = 0; i < 0x70; p++) {
        p->age18 = 0;
        p->pos0.vx = ((t->unk0[0].vx + u->unk0[1].vx + u->unk0[2].vx) / 3) << 12;
        p->pos0.vy = ((u->unk0[0].vy + u->unk0[1].vy + u->unk0[2].vy) / 3) << 12;
        p->pos0.vz = ((u->unk0[0].vz + u->unk0[1].vz + u->unk0[2].vz) / 3) << 12;
        p->unk1A = 0;
        i++;
        p->velC.pad = 0x1000;
        p->spinX14 = ex->btlfx_randCentered(0x100);
        p->spinY16 = ex->btlfx_randCentered(0x200);
        d.vx = (p->pos0.vx >> 12) - center->vx;
        d.vy = (p->pos0.vy >> 12) - center->vy;
        d.vz = (p->pos0.vz >> 12) - center->vz;
        p->velC.vx = d.vx >> 4;
        t++;
        p->velC.vy = d.vy >> 4;
        r = ex->btlfx_rand();
        u++;
        /* Dead store: the branch splits sched1's block here and is gone by final, which keeps the p++ increment
         * after the rand() call (sched2 then leaves it there and u++ fills the call's delay slot). */
        if (p == NULL) {
            k = 0;
        }
        k = (r & 0x1F) + 8;
        p->velC.vz = (d.vz >> 4) + k;
    }
}

void fxm_shatterModelUpdate(void) {
    Fx0ecTri *t = fx_0ec_shatterTris;
    Fx0ecShard *s = fx_0ec_shatterShards;
    BtlfxExports *ex;
    SVECTOR out;
    SVECTOR rot;
    SVECTOR c;
    SVECTOR loc[3];
    s32 bias[2];
    VECTOR v;
    VECTOR sc;
    MATRIX m;
    s32 i;
    s32 j;

    bias[0] = 0;
    bias[1] = 0;
    ex = BTLFX_EXPORTS(fx_0ec_env);
    sc.vx = sc.vy = sc.vz = 0xF80;
    for (i = 0; i < 0x70; i++, s++, t++) {
        if (s->age18 >= 0) {
            if (s->age18 < s->unk1A) {
                s->age18++;
            } else {
                c.vx = s->pos0.vx >> 12;
                c.vy = s->pos0.vy >> 12;
                c.vz = s->pos0.vz >> 12;
                for (j = 0; j < 3; j++) {
                    loc[j].vx = t->unk0[j].vx - c.vx;
                    loc[j].vy = t->unk0[j].vy - c.vy;
                    loc[j].vz = t->unk0[j].vz - c.vz;
                }
                if (s->age18 >= 8) {
                    ex->btlfx_stepModuleParticle((Pool6CEntry *)s, -10, (SVECTOR *)bias, &out);
                    if (s->velC.vy < 0 && s->pos0.vy < 0) {
                        s->velC.vy = -(s->velC.vy >> 1);
                        if (s->velC.vy < 0x3C) {
                            fx_0ec_spawnImpact(&c);
                            goto kill;
                        }
                    }
                }
                if (s->age18 == ex->btlfx_rand() % 3 + 4 || s->age18 >= 8) {
                    rot.vx = s->spinX14;
                    rot.vy = s->spinY16;
                    rot.vz = 0;
                } else {
                    rot.vx = 0;
                    rot.vy = 0;
                    rot.vz = 0;
                }
                ex->math_rotMatrixYXZ(&rot, &m);
                if (s->age18 >= 8) {
                    ex->ScaleMatrix(&m, &sc);
                }
                m.t[0] = s->pos0.vx >> 12;
                m.t[1] = s->pos0.vy >> 12;
                m.t[2] = s->pos0.vz >> 12;
                ex->SetRotMatrix(&m);
                ex->SetTransMatrix(&m);
                for (j = 0; j < 3; j++) {
                    gte_ldv0(&loc[j]);
                    gte_rtv0tr();
                    gte_stlvnl(&v);
                    t->unk0[j].vx = v.vx;
                    t->unk0[j].vy = v.vy;
                    t->unk0[j].vz = v.vz;
                }
                if (++s->age18 >= 0x47) {
                kill:
                    s->age18 = -1;
                    t->unk0[2].vz = 0;
                    *(s32 *)&t->unk0[2] = 0;
                    t->unk0[1].vz = 0;
                    *(s32 *)&t->unk0[1] = 0;
                    t->unk0[0].vz = 0;
                    /* the nested do-while(0)s weight t's refs by loop depth, which puts t ahead of ex in
                     * global-alloc (t in s3, ex in s4) */
                    do {
                        do {
                            *(s32 *)&t->unk0[0] = 0;
                        } while (0);
                    } while (0);
                }
            }
        }
    }
}

s32 fxm_splitModelInit(u8 *buf) {
    Fx0ecShard *p = fx_0ec_splitShards;
    s32 i;

    fx_0ec_split.modelCount0 = 0;
    fx_0ec_split.triCount2 = 0;
    fx_0ec_split.next10 = buf;
    fx_0ec_split.trisC = buf;
    for (i = 0; i < 300; i++) {
        p->age18 = -1;
        p++;
    }
}

static s32 fxm_splitModelAdd(s32 model) {
    BtlfxExports *ex = BTLFX_EXPORTS(fx_0ec_env);
    Fx0ecSplit *sp = &fx_0ec_split;
    Fx0ecVertex *v = (Fx0ecVertex *)sp->next10;
    s32 n;
    s32 i;

    n = (u32)ex->Hi_GetSplitMdlVertex(model, -1) / sizeof(Fx0ecVertex);
    ex->Hi_SplitMdlVertex(model, -1, (s32)v);
    {
        s32 k = fx_0ec_split.modelCount0;
        sp->modelTris14[k] = (u8 *)v;
        sp->triCounts4[k] = n;
    }
    sp->next10 = (u8 *)(v + n);
    sp->triCount2 += n;
    fx_0ec_split.modelCount0++;
    for (i = 0; i < n; i++) {
        v->unk6 = 0;
        v++;
    }
    return sp->triCount2;
}

Fx0ecShard *fxm_splitFreeShard(void) {
    Fx0ecShard *p = fx_0ec_splitShards;
    s32 i;

    for (i = 0; i < 300; i++) {
        if (p->age18 < 0) {
            return p;
        }
        p++;
    }
    return NULL;
}

s32 fxm_splitRelease(s32 y, SVECTOR *center) {
    Fx0ecTri *t;
    Fx0ecTri *u;
    BtlfxExports *ex;
    Fx0ecShard *p;
    SVECTOR d;
    s32 i;
    s32 k;
    s32 n;
    Fx0ecSplit *sp;

    i = k = 0;
    sp = &fx_0ec_split;
    t = (Fx0ecTri *)sp->trisC;
    u = t;
    ex = BTLFX_EXPORTS(fx_0ec_env);
    for (n = k; i < sp->triCount2; i++, u++, t++, k++) {
        if (y < (u->unk0[0].vy + u->unk0[1].vy + u->unk0[2].vy) / 3 && u->unk0[0].pad == 0) {
            p = fxm_splitFreeShard();
            if (p != NULL) {
                u->unk0[0].pad = 1;
                p->pos0.vx = ((t->unk0[0].vx + u->unk0[1].vx + u->unk0[2].vx) / 3) << 12;
                p->pos0.vy = ((u->unk0[0].vy + u->unk0[1].vy + u->unk0[2].vy) / 3) << 12;
                p->pos0.vz = ((u->unk0[0].vz + u->unk0[1].vz + u->unk0[2].vz) / 3) << 12;
                p->age18 = 0;
                p->unk1A = k;
                p->velC.pad = 0x1000;
                p->spinX14 = ex->btlfx_randCentered(0x100);
                p->spinY16 = ex->btlfx_randCentered(0x200);
                d.vx = (p->pos0.vx >> 12) - center->vx;
                d.vy = (p->pos0.vy >> 12) - center->vy;
                d.vz = (p->pos0.vz >> 12) - center->vz;
                n++;
                p->velC.vx = (d.vx >> 3) + ex->btlfx_randCentered(0x10);
                p->velC.vy = ex->btlfx_randCentered(0x10);
                p->velC.vz = ex->btlfx_randCentered(0x20) + 0x1E;
            }
        }
    }
    return n;
}

void fxm_splitUpdate(void) {
    Fx0ecSplit *sp = &fx_0ec_split;
    Fx0ecShard *s = fx_0ec_splitShards;
    BtlfxExports *ex;
    SVECTOR out;
    SVECTOR rot;
    SVECTOR c;
    SVECTOR loc[3];
    s32 bias[2];
    VECTOR v;
    VECTOR sc;
    MATRIX m;
    Fx0ecTri *t;
    s32 i;
    s32 j;

    bias[0] = 0;
    bias[1] = 0;
    ex = BTLFX_EXPORTS(fx_0ec_env);
    sc.vx = sc.vy = sc.vz = 0xEC0;
    for (i = 0; i < 300; i++, s++) {
        if (s->age18 >= 0) {
            t = (Fx0ecTri *)sp->trisC + s->unk1A;
            c.vx = s->pos0.vx >> 12;
            c.vy = s->pos0.vy >> 12;
            c.vz = s->pos0.vz >> 12;
            for (j = 0; j < 3; j++) {
                loc[j].vx = t->unk0[j].vx - c.vx;
                loc[j].vy = t->unk0[j].vy - c.vy;
                loc[j].vz = t->unk0[j].vz - c.vz;
            }
            ex->btlfx_stepModuleParticle((Pool6CEntry *)s, -10, (SVECTOR *)bias, &out);
            if (s->velC.vy < 0 && s->pos0.vy < 0) {
                fx_0ec_spawnImpact(&c);
                goto kill;
            }
            rot.vx = s->spinX14;
            rot.vy = s->spinY16;
            rot.vz = 0;
            ex->math_rotMatrixYXZ(&rot, &m);
            ex->ScaleMatrix(&m, &sc);
            m.t[0] = s->pos0.vx >> 12;
            m.t[1] = s->pos0.vy >> 12;
            m.t[2] = s->pos0.vz >> 12;
            ex->SetRotMatrix(&m);
            ex->SetTransMatrix(&m);
            for (j = 0; j < 3; j++) {
                gte_ldv0(&loc[j]);
                gte_rtv0tr();
                gte_stlvnl(&v);
                t->unk0[j].vx = v.vx;
                t->unk0[j].vy = v.vy;
                t->unk0[j].vz = v.vz;
            }
            if (++s->age18 >= 0x20) {
            kill:
                s->age18 = -1;
                t->unk0[2].vz = 0;
                *(s32 *)&t->unk0[2] = 0;
                t->unk0[1].vz = 0;
                *(s32 *)&t->unk0[1] = 0;
                t->unk0[0].vz = 0;
                *(s32 *)&t->unk0[0] = 0;
            }
        }
    }
}

s32 fx_0ec_op80(s32 mode, Fx0ecWork80 *work, Unk800F78A4 *env, Unk80100C78Entry *entry, EffInit *init) {
    BtlfxExports *ex = BTLFX_EXPORTS(env);
    SVECTOR rot;
    SVECTOR sv;
    VECTOR pos;
    VECTOR sc;
    MATRIX m;
    VECTOR out;
    SVECTOR c[2]; /* [1] unused: the original's frame has 8 more bytes of locals */
    Pool6CEntry *p;
    s32 frame;
    s32 t;
    s32 h;
    s32 r;
    s32 i;
    s32 j0; /* model index per case: block-local, so local-alloc gives it s0 */
    s32 j1;
    s32 j2;
    s32 x;
    s32 x2;
    s32 tp0;
    s32 tp1;
    s32 k;

    if (mode == 0) {
        init->workSize = 0x64;
        init->elemSize = 8;
        init->elemFn = (s32)fx_0ec_op80PoolTask;
        init->poolCount = 0x80;
    } else if (mode == 1) {
        fx_0ec_env = env;
        ex->btlfx_getUnitPos(0, (s16 *)&work->pos8);
        r = ex->btlfx_getUnitRadius(0) + 0xF0;
        ex->btlfx_getUnitRot(0, (s16 *)&work->rot10);
        work->rot10.vy &= 0xFFF;
        if (work->rot10.vy - 0x400U > 0x800) {
            work->rot10.vy = 0;
        } else {
            work->rot10.vy = 0x800;
        }
        ex->btlfx_pointAhead((s16 *)&work->pos8, (s16 *)&work->rot10, r, (s16 *)&work->pos18);
        if (ex->btlfx_isEnemyUnit(0) != 0 && work->pos18.vz < -0x200) {
            work->pos18.vz = -0x200;
        }
        ex->btlfx_rotateOffset(&work->rot10, -0xD2, (s32)&rot);
        sv.vx = work->pos18.vx + rot.vx;
        sv.vy = work->pos18.vy + rot.vy - 0x140;
        sv.vz = work->pos18.vz + rot.vz;
        env->camera->points[1].vx = sv.vx;
        env->camera->points[1].vy = sv.vy;
        env->camera->points[1].vz = sv.vz;
        ex->btlfx_rotateOffset(&work->rot10, -ex->btlfx_getUnitRadius(0), (s32)&rot);
        sv.vx = work->pos8.vx + rot.vx;
        sv.vy = work->pos8.vy + rot.vy - 0x140;
        sv.vz = work->pos8.vz + rot.vz;
        env->camera->points[2].vx = sv.vx;
        env->camera->points[2].vy = sv.vy;
        env->camera->points[2].vz = sv.vz;
        ex->btlfx_getUnitBodyPos(0, 0, (s16 *)&work->pos8);
        ex->btlfx_initEffModels(fx_0ec_effModels, 9);
        work->model38 = (s16)ex->Hi_RegisterTexEffModel((BtlUnit9C8 *)ex->btlfx_getFileData(entry->file, 1), 0x97, 0x3D40,
                                                      0x80, 0x80);
        work->model3C = (s16)ex->Hi_RegisterTexEffModel((BtlUnit9C8 *)ex->btlfx_getFileData(entry->file, 3), 0x97, 0x3D40,
                                                      0x80, 0x80);
        work->model40[0] = (s16)ex->Hi_RegisterTexEffModel((BtlUnit9C8 *)ex->btlfx_getFileData(entry->file, 4), 0xB7,
                                                         0x3DC0, 0, 0x80);
        work->model4C = (s16)ex->Hi_RegisterTexEffModel((BtlUnit9C8 *)ex->btlfx_getFileData(entry->file, 0xF), 0x97, 0x3DC0,
                                                      0, 0x80);
        work->model50 = (s16)ex->Hi_RegisterTexEffModel((BtlUnit9C8 *)ex->btlfx_getFileData(entry->file, 7), 0x39, 0x3D01,
                                                      0, 0);
        work->model54 = (s16)ex->Hi_RegisterTexEffModel((BtlUnit9C8 *)ex->btlfx_getFileData(entry->file, 0x10), 0x119,
                                                      0xFFFF, 0, 0x90);
        work->model58 = (s16)ex->Hi_RegisterGouEffModel((BtlUnit9C8 *)ex->btlfx_getFileData(entry->file, 0x13));
        x = env->dispEnv->disp.x;
        if (x < 0x100) {
            x2 = x + 0x140;
        } else {
            x2 = x - 0x140;
        }
        tp0 = getTPage(2, 0, x, env->dispEnv->disp.y);
        tp1 = getTPage(2, 0, x2, env->dispEnv->disp.y);
        work->model40[1] = (s16)ex->Hi_RegisterTexEffModel((BtlUnit9C8 *)ex->btlfx_getFileData(entry->file, 0xD), tp0 | 0x20,
                                                         0xFFFF, 0x40, 0);
        work->model40[2] = (s16)ex->Hi_RegisterTexEffModel((BtlUnit9C8 *)ex->btlfx_getFileData(entry->file, 0xD), tp1 | 0x20,
                                                         0xFFFF, 0x40, 0);
        fx_0ec_task = (Fx0ecWork *)entry; /* the shard callbacks spawn into entry->pool */
        k = (ex->btlfx_getUnitRadius(0) << 12) / 390;
        if (k < 0x1400) {
            k = 0x1400;
        }
        if (k > 0x1755) {
            k = 0x1755;
        }
        work->scale5C = k;
        p = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, 8), (s32)&work->pos18);
        if (p != NULL) {
            p->otZ = -0x40;
        }
        p = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, 0xC), (s32)&work->pos20);
        if (p != NULL) {
            p->otZ = 0x80;
        }
        work->state0 = 0;
        work->frame4 = 0;
    } else {
    frame = entry->frame;
    switch (work->state0) {
    case 0:
        sc.vx = sc.vy = sc.vz = work->scale5C;
        rot.vx = work->rot10.vx;
        rot.vy = work->rot10.vy;
        rot.vz = work->rot10.vz;
        t = (frame << 12) / 22;
        rot.vy += (0x1000 - t) >> 1;
        pos.vx = work->pos18.vx;
        pos.vy = work->pos18.vy;
        pos.vz = work->pos18.vz;
        h = (ex->math_cos(t >> 2) * 0x514) >> 12;
        pos.vy += h;
        work->pos20.vx = work->pos18.vx;
        work->pos20.vy = work->pos18.vy;
        work->pos20.vz = work->pos18.vz;
        work->pos20.vy -= (t * 0x140) >> 12;
        p = (Pool6CEntry *)ex->btlfx_getFileData(entry->file, 9);
        sv.vx = pos.vx;
        sv.vy = pos.vy;
        sv.vz = pos.vz;
        sv.vy -= (((SVECTOR *)p)->pad * sc.vy) >> 12;
        env->camera->points[0].vx = sv.vx;
        env->camera->points[0].vy = sv.vy;
        env->camera->points[0].vz = sv.vz;
        ex->Hi_SetEffModelSlice(work->model38, h);
        ex->Hi_DrawSliceEffModel(&rot, &pos, (s32)&sc, work->model38);
        ex->Hi_SetEffModelSlice(work->model3C, h);
        ex->Hi_DrawSliceEffModel(&rot, &pos, (s32)&sc, work->model3C);
        j0 = (work->frame4 & 1) + 1;
        ex->Hi_SetEffModelSlice(work->model40[j0], h);
        ex->Hi_DrawSliceEffModel(&rot, &pos, (s32)&sc, work->model40[j0]);
        ex->Hi_ModifyEffModelRGB(work->model40[j0], ((t / 2 + 0x800) >> 5) - 0x80, ((t / 2 + 0x800) >> 5) - 0x80,
                                 ((t / 2 + 0x800) >> 5) - 0x80);
        ex->Hi_SetEffModelSlice(work->model54, h);
        ex->Hi_DrawSliceEffModel(&rot, &pos, (s32)&sc, work->model54);
        ex->Hi_ModifyEffModelRGB(work->model54, -0x60, -0x60, -0x60);
        if (frame >= 22) {
            work->state0 = 1;
            entry->frame = -1;
        }
        break;
    case 1:
        sc.vx = sc.vy = sc.vz = work->scale5C;
        rot.vx = work->rot10.vx;
        rot.vy = work->rot10.vy;
        rot.vz = work->rot10.vz;
        pos.vx = work->pos18.vx;
        pos.vy = work->pos18.vy;
        pos.vz = work->pos18.vz;
        t = (frame << 12) / 20;
        ex->Hi_DrawEffModel(&rot, &pos, (s32)&sc, work->model38);
        ex->Hi_DrawEffModel(&rot, &pos, (s32)&sc, work->model3C);
        ex->Hi_SetEffModelOffset(work->model58, 0x28);
        ex->Hi_DrawEffModel(&rot, &pos, (s32)&sc, work->model58);
        ex->Hi_SetEffModelOffset(work->model54, 0x14);
        ex->Hi_DrawEffModel(&rot, &pos, (s32)&sc, work->model54);
        ex->Hi_ModifyEffModelRGB(work->model54, ((t * 3 / 4 + 0x400) >> 5) - 0x80, ((t * 3 / 4 + 0x400) >> 5) - 0x80,
                                 ((t * 3 / 4 + 0x400) >> 5) - 0x80);
        j1 = (work->frame4 & 1) + 1;
        ex->Hi_DrawEffModel(&rot, &pos, (s32)&sc, work->model40[j1]);
        ex->Hi_ModifyEffModelRGB(work->model40[j1], ((0x1000 - t / 2) >> 5) - 0x80, ((0x1000 - t / 2) >> 5) - 0x80,
                                 ((0x1000 - t / 2) >> 5) - 0x80);
        if (frame >= 20) {
            work->state0 = 2;
            entry->frame = -1;
        }
        break;
    case 2:
        sc.vx = sc.vy = sc.vz = work->scale5C;
        rot.vx = work->rot10.vx;
        rot.vy = work->rot10.vy;
        rot.vz = work->rot10.vz;
        pos.vx = work->pos18.vx;
        pos.vy = work->pos18.vy;
        pos.vz = work->pos18.vz;
        t = (frame << 12) / 30;
        ex->Hi_DrawEffModel(&rot, &pos, (s32)&sc, work->model38);
        ex->Hi_DrawEffModel(&rot, &pos, (s32)&sc, work->model3C);
        ex->Hi_DrawEffModel(&rot, &pos, (s32)&sc, work->model40[0]);
        if (frame < 8) {
            ex->Hi_ModifyEffModelAbr(work->model40[0], 1);
            ex->Hi_ModifyEffModelRGB(work->model40[0], ((frame << 9) >> 5) - 0x80, ((frame << 9) >> 5) - 0x80,
                                     ((frame << 9) >> 5) - 0x80);
        }
        ex->Hi_SetEffModelOffset(work->model58, 0x31);
        ex->Hi_DrawEffModel(&rot, &pos, (s32)&sc, work->model58);
        ex->Hi_SetEffModelOffset(work->model54, 0x27);
        ex->Hi_DrawEffModel(&rot, &pos, (s32)&sc, work->model54);
        {
            s32 q = t >> 2;

            ex->Hi_ModifyEffModelRGB(work->model54, ((0x1000 - ex->math_sin(q)) >> 5) - 0x80,
                                     ((0x1000 - ex->math_sin(q)) >> 5) - 0x80, ((0x1000 - ex->math_sin(q)) >> 5) - 0x80);
        }
        if (frame < 16) {
            j2 = (work->frame4 & 1) + 1;
            ex->Hi_DrawEffModel(&rot, &pos, (s32)&sc, work->model40[j2]);
            ex->Hi_ModifyEffModelRGB(work->model40[j2], ((0x1000 - (frame << 8)) >> 6) - 0x80,
                                     ((0x1000 - (frame << 8)) >> 6) - 0x80, ((0x1000 - (frame << 8)) >> 6) - 0x80);
        }
        if (frame == 20) {
            for (i = 0; i < 2; i++) {
                /* two sets keep cse2 from folding data + 4 into the first read (the original reads all three
                 * through v1); the register is pinned because local-alloc otherwise ties v to the call's v0 */
                register SVECTOR *v REG_ASM("$3");

                v = (SVECTOR *)ex->btlfx_getFileData(entry->file, fx_0ec_pointBlocks[i]);
                v = (SVECTOR *)((u8 *)v + 4);
                sv.vx = v->vx;
                sv.vy = v->vy;
                sv.vz = v->vz;
                gte_SetRotMatrix(env->viewMatrix);
                gte_SetTransMatrix(env->viewMatrix);
                ex->math_rotMatrixYXZ(&rot, &m);
                ex->ScaleMatrix(&m, &sc);
                m.t[0] = pos.vx;
                m.t[1] = pos.vy;
                m.t[2] = pos.vz;
                gte_SetRotMatrix(&m);
                gte_SetTransMatrix(&m);
                gte_ldv0(&sv);
                gte_rtv0tr();
                gte_stlvnl(&out);
                setVector(&work->emitPos28[i], out.vx, out.vy, out.vz);
                p = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, 0xB), (s32)&work->emitPos28[i]);
                if (p != NULL) {
                    p->otZ = 0x40;
                }
            }
        }
        if (frame >= 30) {
            work->state0 = 3;
            entry->frame = -1;
            ex->btlfx_drawScreenTint((u32 *)env->ot + 2, 2, 100, 100, 0);
            fxm_splitModelInit(fx_0ec_splitBuf);
            fxm_splitModelAdd(work->model38);
            fxm_splitModelAdd(work->model3C);
            c[0].vx = 0;
            c[0].vy = 0;
            c[0].vz = 0;
            fxm_shatterModel(work->model4C, c);
        }
        break;
    case 3:
        if (frame == 8) {
            ex->btlfx_drawScreenTint((u32 *)env->ot + 2, 1, 0xC8, 0, 0);
        }
        if (frame >= 8) {
            if (frame == 8) {
                work->pos20.vx = work->pos18.vx;
                work->pos20.vy = work->pos18.vy;
                work->pos20.vz = work->pos18.vz;
                work->pos20.vy -= (work->scale5C << 10) >> 12;
                work->emit60 = p = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, 0xE), (s32)&work->pos18);
                if (p == NULL) {
                    goto skip; /* the original's NULL branch skips the unk60 check (unk60 is NULL then) */
                }
                p->target = (s32)&work->pos20;
            }
            if (work->emit60 != NULL && work->emit60->state == BTLFX_EMIT_DONE) {
                work->emit60 = NULL;
            }
        skip:
            sv.vx = work->rot10.vx;
            sv.vy = work->rot10.vy;
            sv.vz = work->rot10.vz;
            sv.vy += 0x800;
            if (work->emit60 != NULL) {
                work->emit60->rot.vx = sv.vx;
                work->emit60->rot.vy = sv.vy;
                work->emit60->rot.vz = sv.vz;
            }
        }
        sv.vx = 0;
        sv.vy = 0;
        sv.vz = 0;
        sc.vx = sc.vy = sc.vz = work->scale5C;
        rot.vx = work->rot10.vx;
        rot.vy = work->rot10.vy;
        rot.vz = work->rot10.vz;
        pos.vx = work->pos18.vx;
        pos.vy = work->pos18.vy;
        pos.vz = work->pos18.vz;
        fx_0ec_modelScale.vx = sc.vx;
        fx_0ec_modelScale.vy = sc.vy;
        fx_0ec_modelScale.vz = sc.vz;
        fx_0ec_modelRot.vx = rot.vx;
        fx_0ec_modelRot.vy = rot.vy;
        fx_0ec_modelRot.vz = rot.vz;
        fx_0ec_modelRot.vy += 0x800;
        fx_0ec_modelPos.vx = pos.vx;
        fx_0ec_modelPos.vy = pos.vy;
        fx_0ec_modelPos.vz = pos.vz;
        ex->Hi_DrawEffModel(&rot, &pos, (s32)&sc, work->model38);
        ex->Hi_ModifyEffModelRGB(work->model38, 0, 0, 0);
        ex->Hi_DrawEffModel(&rot, &pos, (s32)&sc, work->model3C);
        ex->Hi_ModifyEffModelRGB(work->model3C, 0, 0, 0);
        ex->Hi_DrawEffModel(&rot, &pos, (s32)&sc, work->model4C);
        {
            s32 a;

            if (frame < 8) {
                a = (frame << 8) + 0x800;
            } else {
                a = 0x1000;
            }
            ex->Hi_ModifyEffModelRGB(work->model4C, (a >> 5) - 0x80, (a >> 5) - 0x80, (a >> 5) - 0x80);
        }
        if (frame < 47) {
            fxm_splitRelease(1000 - (((0x1000 - ex->math_cos((frame << 10) / 47)) * 1000) >> 12), &sv);
        }
        fxm_splitUpdate();
        fxm_shatterModelUpdate();
        if (frame >= 71) {
            work->state0 = 3;
            entry->frame = -1;
            ex->btlfx_clearEffModels();
            return 1;
        }
        break;
    }
    ex->btlfx_drawBackdrop((BtlBgLayers *)ex->btlfx_getFileData(entry->file, 0x11), work->frame4);
    work->frame4++;
    }
    return 0;
}

INCLUDE_RODATA("asm/us/fx_0ec/nonmatchings/fx_0ec", fx_0ec_headerPtr);
