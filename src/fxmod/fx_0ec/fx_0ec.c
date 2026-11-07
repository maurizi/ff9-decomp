#include "fxmod/fx_0ec.h"

static s32 fx_0ec_op80PoolTask(s32 mode, SVECTOR *pos, Unk800F78A4 *env, Unk80100C78Entry *entry, s32 frame) {
    BtlfxExports *ex = BTLFX_EXPORTS(env);
    SVECTOR rot;
    s32 t;
    u8 *data;

    if (mode != 1) {
        t = (frame << 12) / FX_DOOM_HALO_FRAMES;
        rot.vx = 0x400;
        rot.vy = 0;
        rot.vz = 0;
        rot.pad = 1;
        data = (u8 *)ex->btlfx_getFileData(entry->file, FX_DOOM_VIOLET_HALO);
        ex->btlfx_drawParticles(data, pos, &rot, (ex->math_sin(t >> 2) >> 1) + 0x400, 0, BTLFX_ABR_ADD, 0x1000 - t, 0, 0);
        if (frame >= FX_DOOM_HALO_FRAMES) {
            return 1;
        }
    }
    return 0;
}

void fx_0ec_spawnImpact(SVECTOR *pos) {
    Unk800F78A4 *env = fx_0ec_env;
    BtlfxExports *ex = BTLFX_EXPORTS(env);
    SVECTOR *spot = ex->btlfx_poolSpawn((s16 *)fx_0ec_task->pool10);
    MATRIX m;
    VECTOR world;

    if (spot != NULL) {
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
        gte_stlvnl(&world);
        world.vy = 0;
        spot->vx = world.vx;
        spot->vy = world.vy;
        spot->vz = world.vz;
    }
}

static void fxm_shatterModel(s32 model, SVECTOR *center) {
    Fx0ecTri *tri = fx_0ec_shatterTris;
    Fx0ecTri *triCopy = fx_0ec_shatterTris;
    BtlfxExports *ex = BTLFX_EXPORTS(fx_0ec_env);
    Fx0ecShard *shard;
    SVECTOR dir;
    s32 i;
    s32 lift;
    s32 rnd;

    ex->Hi_SplitMdlVertex(model, -1, (s32)tri);
    shard = fx_0ec_shatterShards;
    for (i = 0; i < FX_DOOM_SHATTER_TRIS; shard++) {
        shard->age18 = 0;
        shard->pos0.vx = ((tri->verts0[0].vx + triCopy->verts0[1].vx + triCopy->verts0[2].vx) / 3) << 12;
        shard->pos0.vy = ((triCopy->verts0[0].vy + triCopy->verts0[1].vy + triCopy->verts0[2].vy) / 3) << 12;
        shard->pos0.vz = ((triCopy->verts0[0].vz + triCopy->verts0[1].vz + triCopy->verts0[2].vz) / 3) << 12;
        shard->delayOrTri1A = 0;
        i++;
        shard->velC.pad = 0x1000;
        shard->spinX14 = ex->btlfx_randCentered(0x100);
        shard->spinY16 = ex->btlfx_randCentered(0x200);
        dir.vx = (shard->pos0.vx >> 12) - center->vx;
        dir.vy = (shard->pos0.vy >> 12) - center->vy;
        dir.vz = (shard->pos0.vz >> 12) - center->vz;
        shard->velC.vx = dir.vx >> 4;
        tri++;
        shard->velC.vy = dir.vy >> 4;
        rnd = ex->btlfx_rand();
        triCopy++;
        /* Dead store: the branch splits sched1's block here and is gone by final, which keeps the shard++ increment
         * after the rand() call (sched2 then leaves it there and triCopy++ fills the call's delay slot). */
        if (shard == NULL) {
            lift = 0;
        }
        lift = (rnd & 0x1F) + 8;
        shard->velC.vz = (dir.vz >> 4) + lift;
    }
}

void fxm_shatterModelUpdate(void) {
    Fx0ecTri *tri = fx_0ec_shatterTris;
    Fx0ecShard *shard = fx_0ec_shatterShards;
    BtlfxExports *ex;
    SVECTOR out;
    SVECTOR rot;
    SVECTOR center;
    SVECTOR loc[3];
    s32 bias[2];
    VECTOR world;
    VECTOR sc;
    MATRIX m;
    s32 i;
    s32 j;

    bias[0] = 0;
    bias[1] = 0;
    ex = BTLFX_EXPORTS(fx_0ec_env);
    sc.vx = sc.vy = sc.vz = 0xF80;
    for (i = 0; i < FX_DOOM_SHATTER_TRIS; i++, shard++, tri++) {
        if (shard->age18 >= 0) {
            if (shard->age18 < shard->delayOrTri1A) {
                shard->age18++;
            } else {
                center.vx = shard->pos0.vx >> 12;
                center.vy = shard->pos0.vy >> 12;
                center.vz = shard->pos0.vz >> 12;
                for (j = 0; j < 3; j++) {
                    loc[j].vx = tri->verts0[j].vx - center.vx;
                    loc[j].vy = tri->verts0[j].vy - center.vy;
                    loc[j].vz = tri->verts0[j].vz - center.vz;
                }
                if (shard->age18 >= FX_DOOM_SHATTER_FLY_AGE) {
                    ex->btlfx_stepModuleParticle((Pool6CEntry *)shard, -10, (SVECTOR *)bias, &out);
                    if (shard->velC.vy < 0 && shard->pos0.vy < 0) {
                        shard->velC.vy = -(shard->velC.vy >> 1);
                        if (shard->velC.vy < 0x3C) {
                            fx_0ec_spawnImpact(&center);
                            goto kill;
                        }
                    }
                }
                if (shard->age18 == ex->btlfx_rand() % 3 + 4 || shard->age18 >= FX_DOOM_SHATTER_FLY_AGE) {
                    rot.vx = shard->spinX14;
                    rot.vy = shard->spinY16;
                    rot.vz = 0;
                } else {
                    rot.vx = 0;
                    rot.vy = 0;
                    rot.vz = 0;
                }
                ex->math_rotMatrixYXZ(&rot, &m);
                if (shard->age18 >= FX_DOOM_SHATTER_FLY_AGE) {
                    ex->ScaleMatrix(&m, &sc);
                }
                m.t[0] = shard->pos0.vx >> 12;
                m.t[1] = shard->pos0.vy >> 12;
                m.t[2] = shard->pos0.vz >> 12;
                ex->SetRotMatrix(&m);
                ex->SetTransMatrix(&m);
                for (j = 0; j < 3; j++) {
                    gte_ldv0(&loc[j]);
                    gte_rtv0tr();
                    gte_stlvnl(&world);
                    tri->verts0[j].vx = world.vx;
                    tri->verts0[j].vy = world.vy;
                    tri->verts0[j].vz = world.vz;
                }
                if (++shard->age18 >= FX_DOOM_SHATTER_LIFE) {
                kill:
                    shard->age18 = -1;
                    tri->verts0[2].vz = 0;
                    *(s32 *)&tri->verts0[2] = 0;
                    tri->verts0[1].vz = 0;
                    *(s32 *)&tri->verts0[1] = 0;
                    tri->verts0[0].vz = 0;
                    /* the nested do-while(0)s weight tri's refs by loop depth, which puts tri ahead of ex in
                     * global-alloc (tri in s3, ex in s4) */
                    do {
                        do {
                            *(s32 *)&tri->verts0[0] = 0;
                        } while (0);
                    } while (0);
                }
            }
        }
    }
}

s32 fxm_splitModelInit(u8 *buf) {
    Fx0ecShard *shard = fx_0ec_splitShards;
    s32 i;

    fx_0ec_split.modelCount0 = 0;
    fx_0ec_split.triCount2 = 0;
    fx_0ec_split.next10 = buf;
    fx_0ec_split.trisC = buf;
    for (i = 0; i < FX_DOOM_SPLIT_SHARDS; i++) {
        shard->age18 = -1;
        shard++;
    }
}

static s32 fxm_splitModelAdd(s32 model) {
    BtlfxExports *ex = BTLFX_EXPORTS(fx_0ec_env);
    Fx0ecSplit *split = &fx_0ec_split;
    Fx0ecVertex *vert = (Fx0ecVertex *)split->next10;
    s32 n;
    s32 i;

    n = (u32)ex->Hi_GetSplitMdlVertex(model, -1) / sizeof(Fx0ecVertex);
    ex->Hi_SplitMdlVertex(model, -1, (s32)vert);
    {
        s32 slot = fx_0ec_split.modelCount0;
        split->modelTris14[slot] = (u8 *)vert;
        split->triCounts4[slot] = n;
    }
    split->next10 = (u8 *)(vert + n);
    split->triCount2 += n;
    fx_0ec_split.modelCount0++;
    for (i = 0; i < n; i++) {
        vert->split6 = 0;
        vert++;
    }
    return split->triCount2;
}

Fx0ecShard *fxm_splitFreeShard(void) {
    Fx0ecShard *shard = fx_0ec_splitShards;
    s32 i;

    for (i = 0; i < FX_DOOM_SPLIT_SHARDS; i++) {
        if (shard->age18 < 0) {
            return shard;
        }
        shard++;
    }
    return NULL;
}

s32 fxm_splitRelease(s32 lineY, SVECTOR *center) {
    Fx0ecTri *tri;
    Fx0ecTri *triCopy;
    BtlfxExports *ex;
    Fx0ecShard *shard;
    SVECTOR dir;
    s32 i;
    s32 triIdx;
    s32 n;
    Fx0ecSplit *split;

    i = triIdx = 0;
    split = &fx_0ec_split;
    tri = (Fx0ecTri *)split->trisC;
    triCopy = tri;
    ex = BTLFX_EXPORTS(fx_0ec_env);
    for (n = triIdx; i < split->triCount2; i++, triCopy++, tri++, triIdx++) {
        if (lineY < (triCopy->verts0[0].vy + triCopy->verts0[1].vy + triCopy->verts0[2].vy) / 3 && triCopy->verts0[0].pad == 0) {
            shard = fxm_splitFreeShard();
            if (shard != NULL) {
                triCopy->verts0[0].pad = 1;
                shard->pos0.vx = ((tri->verts0[0].vx + triCopy->verts0[1].vx + triCopy->verts0[2].vx) / 3) << 12;
                shard->pos0.vy = ((triCopy->verts0[0].vy + triCopy->verts0[1].vy + triCopy->verts0[2].vy) / 3) << 12;
                shard->pos0.vz = ((triCopy->verts0[0].vz + triCopy->verts0[1].vz + triCopy->verts0[2].vz) / 3) << 12;
                shard->age18 = 0;
                shard->delayOrTri1A = triIdx;
                shard->velC.pad = 0x1000;
                shard->spinX14 = ex->btlfx_randCentered(0x100);
                shard->spinY16 = ex->btlfx_randCentered(0x200);
                dir.vx = (shard->pos0.vx >> 12) - center->vx;
                dir.vy = (shard->pos0.vy >> 12) - center->vy;
                dir.vz = (shard->pos0.vz >> 12) - center->vz;
                n++;
                shard->velC.vx = (dir.vx >> 3) + ex->btlfx_randCentered(0x10);
                shard->velC.vy = ex->btlfx_randCentered(0x10);
                shard->velC.vz = ex->btlfx_randCentered(0x20) + 0x1E;
            }
        }
    }
    return n;
}

void fxm_splitUpdate(void) {
    Fx0ecSplit *split = &fx_0ec_split;
    Fx0ecShard *shard = fx_0ec_splitShards;
    BtlfxExports *ex;
    SVECTOR out;
    SVECTOR rot;
    SVECTOR center;
    SVECTOR loc[3];
    s32 bias[2];
    VECTOR world;
    VECTOR sc;
    MATRIX m;
    Fx0ecTri *tri;
    s32 i;
    s32 j;

    bias[0] = 0;
    bias[1] = 0;
    ex = BTLFX_EXPORTS(fx_0ec_env);
    sc.vx = sc.vy = sc.vz = 0xEC0;
    for (i = 0; i < FX_DOOM_SPLIT_SHARDS; i++, shard++) {
        if (shard->age18 >= 0) {
            tri = (Fx0ecTri *)split->trisC + shard->delayOrTri1A;
            center.vx = shard->pos0.vx >> 12;
            center.vy = shard->pos0.vy >> 12;
            center.vz = shard->pos0.vz >> 12;
            for (j = 0; j < 3; j++) {
                loc[j].vx = tri->verts0[j].vx - center.vx;
                loc[j].vy = tri->verts0[j].vy - center.vy;
                loc[j].vz = tri->verts0[j].vz - center.vz;
            }
            ex->btlfx_stepModuleParticle((Pool6CEntry *)shard, -10, (SVECTOR *)bias, &out);
            if (shard->velC.vy < 0 && shard->pos0.vy < 0) {
                fx_0ec_spawnImpact(&center);
                goto kill;
            }
            rot.vx = shard->spinX14;
            rot.vy = shard->spinY16;
            rot.vz = 0;
            ex->math_rotMatrixYXZ(&rot, &m);
            ex->ScaleMatrix(&m, &sc);
            m.t[0] = shard->pos0.vx >> 12;
            m.t[1] = shard->pos0.vy >> 12;
            m.t[2] = shard->pos0.vz >> 12;
            ex->SetRotMatrix(&m);
            ex->SetTransMatrix(&m);
            for (j = 0; j < 3; j++) {
                gte_ldv0(&loc[j]);
                gte_rtv0tr();
                gte_stlvnl(&world);
                tri->verts0[j].vx = world.vx;
                tri->verts0[j].vy = world.vy;
                tri->verts0[j].vz = world.vz;
            }
            if (++shard->age18 >= FX_DOOM_SPLIT_LIFE) {
            kill:
                shard->age18 = -1;
                tri->verts0[2].vz = 0;
                *(s32 *)&tri->verts0[2] = 0;
                tri->verts0[1].vz = 0;
                *(s32 *)&tri->verts0[1] = 0;
                tri->verts0[0].vz = 0;
                *(s32 *)&tri->verts0[0] = 0;
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
    SVECTOR center[2]; /* [1] unused: the original's frame has 8 more bytes of locals */
    Pool6CEntry *emitter;
    s32 frame;
    s32 t;
    s32 rise;
    s32 dist;
    s32 i;
    s32 flicker0; /* model index per case: block-local, so local-alloc gives it s0 */
    s32 flicker1;
    s32 flicker2;
    s32 dispX;
    s32 otherX;
    s32 tp0;
    s32 tp1;
    s32 unitScale;

    if (mode == 0) {
        init->workSize = sizeof(Fx0ecWork80);
        init->elemSize = sizeof(SVECTOR);
        init->elemFn = (s32)fx_0ec_op80PoolTask;
        init->poolCount = FX_DOOM_HALO_SLOTS;
    } else if (mode == 1) {
        fx_0ec_env = env;
        ex->btlfx_getUnitPos(0, (s16 *)&work->pos8);
        dist = ex->btlfx_getUnitRadius(0) + 0xF0;
        ex->btlfx_getUnitRot(0, (s16 *)&work->rot10);
        work->rot10.vy &= 0xFFF;
        if (work->rot10.vy - 0x400U > 0x800) {
            work->rot10.vy = 0;
        } else {
            work->rot10.vy = 0x800;
        }
        ex->btlfx_pointAhead((s16 *)&work->pos8, (s16 *)&work->rot10, dist, (s16 *)&work->pos18);
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
        ex->btlfx_initEffModels(fx_0ec_effModels, FX_DOOM_MODEL_COUNT);
        work->model38 = (s16)ex->Hi_RegisterTexEffModel((BtlModelHeader *)ex->btlfx_getFileData(entry->file, FX_DOOM_GOLD_FRAME), FX_DOOM_GOLD_FRAME_TPAGE, FX_DOOM_GOLD_FRAME_CLUT,
                                                      FX_DOOM_GOLD_FRAME_U, FX_DOOM_GOLD_FRAME_V);
        work->model3C = (s16)ex->Hi_RegisterTexEffModel((BtlModelHeader *)ex->btlfx_getFileData(entry->file, FX_DOOM_GOLD_PANEL), FX_DOOM_GOLD_PANEL_TPAGE, FX_DOOM_GOLD_PANEL_CLUT,
                                                      FX_DOOM_GOLD_PANEL_U, FX_DOOM_GOLD_PANEL_V);
        work->model40[0] = (s16)ex->Hi_RegisterTexEffModel((BtlModelHeader *)ex->btlfx_getFileData(entry->file, FX_DOOM_REAPER_PICTURE), FX_DOOM_REAPER_PICTURE_TPAGE,
                                                         FX_DOOM_REAPER_PICTURE_CLUT, FX_DOOM_REAPER_PICTURE_U, FX_DOOM_REAPER_PICTURE_V);
        work->model4C = (s16)ex->Hi_RegisterTexEffModel((BtlModelHeader *)ex->btlfx_getFileData(entry->file, FX_DOOM_REAPER_PICTURE_2), FX_DOOM_REAPER_PICTURE_2_TPAGE, FX_DOOM_REAPER_PICTURE_2_CLUT,
                                                      FX_DOOM_REAPER_PICTURE_2_U, FX_DOOM_REAPER_PICTURE_2_V);
        work->model50 = (s16)ex->Hi_RegisterTexEffModel((BtlModelHeader *)ex->btlfx_getFileData(entry->file, FX_DOOM_GOLD_LATTICE), FX_DOOM_GOLD_LATTICE_TPAGE, FX_DOOM_GOLD_LATTICE_CLUT,
                                                      FX_DOOM_GOLD_LATTICE_U, FX_DOOM_GOLD_LATTICE_V);
        work->model54 = (s16)ex->Hi_RegisterTexEffModel((BtlModelHeader *)ex->btlfx_getFileData(entry->file, FX_DOOM_SWIRL_SHELL), FX_DOOM_SWIRL_SHELL_TPAGE,
                                                      FX_DOOM_SWIRL_SHELL_CLUT, FX_DOOM_SWIRL_SHELL_U, FX_DOOM_SWIRL_SHELL_V);
        work->model58 = (s16)ex->Hi_RegisterGouEffModel((BtlModelHeader *)ex->btlfx_getFileData(entry->file, FX_DOOM_GOURAUD_SHELL));
        dispX = env->dispEnv->disp.x;
        if (dispX < 0x100) {
            otherX = dispX + SCREEN_W;
        } else {
            otherX = dispX - SCREEN_W;
        }
        tp0 = getTPage(2, 0, dispX, env->dispEnv->disp.y);
        tp1 = getTPage(2, 0, otherX, env->dispEnv->disp.y);
        work->model40[1] = (s16)ex->Hi_RegisterTexEffModel((BtlModelHeader *)ex->btlfx_getFileData(entry->file, FX_DOOM_GLOW_SHELL), tp0 | TPAGE_ABR_ADD,
                                                         FX_DOOM_GLOW_SHELL_CLUT, FX_DOOM_GLOW_SHELL_U, FX_DOOM_GLOW_SHELL_V);
        work->model40[2] = (s16)ex->Hi_RegisterTexEffModel((BtlModelHeader *)ex->btlfx_getFileData(entry->file, FX_DOOM_GLOW_SHELL), tp1 | TPAGE_ABR_ADD,
                                                         FX_DOOM_GLOW_SHELL_CLUT, FX_DOOM_GLOW_SHELL_U, FX_DOOM_GLOW_SHELL_V);
        fx_0ec_task = (Fx0ecWork *)entry; /* the shard callbacks spawn into entry->pool */
        unitScale = (ex->btlfx_getUnitRadius(0) << 12) / 390;
        if (unitScale < 0x1400) {
            unitScale = 0x1400;
        }
        if (unitScale > 0x1755) {
            unitScale = 0x1755;
        }
        work->scale5C = unitScale;
        emitter = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, FX_DOOM_PINK_RINGS), (s32)&work->pos18);
        if (emitter != NULL) {
            emitter->otZ = -0x40;
        }
        emitter = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, FX_DOOM_PINK_SMOKE), (s32)&work->pos20);
        if (emitter != NULL) {
            emitter->otZ = 0x80;
        }
        work->state0 = FX_DOOM_STATE_RISE;
        work->frame4 = 0;
    } else {
    frame = entry->frame;
    switch (work->state0) {
    case FX_DOOM_STATE_RISE:
        sc.vx = sc.vy = sc.vz = work->scale5C;
        rot.vx = work->rot10.vx;
        rot.vy = work->rot10.vy;
        rot.vz = work->rot10.vz;
        t = (frame << 12) / FX_DOOM_RISE_FRAMES;
        rot.vy += (0x1000 - t) >> 1;
        pos.vx = work->pos18.vx;
        pos.vy = work->pos18.vy;
        pos.vz = work->pos18.vz;
        rise = (ex->math_cos(t >> 2) * 0x514) >> 12;
        pos.vy += rise;
        work->pos20.vx = work->pos18.vx;
        work->pos20.vy = work->pos18.vy;
        work->pos20.vz = work->pos18.vz;
        work->pos20.vy -= (t * 0x140) >> 12;
        emitter = (Pool6CEntry *)ex->btlfx_getFileData(entry->file, FX_DOOM_CAMERA_POINT);
        sv.vx = pos.vx;
        sv.vy = pos.vy;
        sv.vz = pos.vz;
        sv.vy -= (((SVECTOR *)emitter)->pad * sc.vy) >> 12;
        env->camera->points[0].vx = sv.vx;
        env->camera->points[0].vy = sv.vy;
        env->camera->points[0].vz = sv.vz;
        ex->Hi_SetEffModelSlice(work->model38, rise);
        ex->Hi_DrawSliceEffModel(&rot, &pos, (s32)&sc, work->model38);
        ex->Hi_SetEffModelSlice(work->model3C, rise);
        ex->Hi_DrawSliceEffModel(&rot, &pos, (s32)&sc, work->model3C);
        flicker0 = (work->frame4 & 1) + 1;
        ex->Hi_SetEffModelSlice(work->model40[flicker0], rise);
        ex->Hi_DrawSliceEffModel(&rot, &pos, (s32)&sc, work->model40[flicker0]);
        ex->Hi_ModifyEffModelRGB(work->model40[flicker0], ((t / 2 + 0x800) >> 5) - 0x80, ((t / 2 + 0x800) >> 5) - 0x80,
                                 ((t / 2 + 0x800) >> 5) - 0x80);
        ex->Hi_SetEffModelSlice(work->model54, rise);
        ex->Hi_DrawSliceEffModel(&rot, &pos, (s32)&sc, work->model54);
        ex->Hi_ModifyEffModelRGB(work->model54, -0x60, -0x60, -0x60);
        if (frame >= FX_DOOM_RISE_FRAMES) {
            work->state0 = FX_DOOM_STATE_GLOW;
            entry->frame = -1;
        }
        break;
    case FX_DOOM_STATE_GLOW:
        sc.vx = sc.vy = sc.vz = work->scale5C;
        rot.vx = work->rot10.vx;
        rot.vy = work->rot10.vy;
        rot.vz = work->rot10.vz;
        pos.vx = work->pos18.vx;
        pos.vy = work->pos18.vy;
        pos.vz = work->pos18.vz;
        t = (frame << 12) / FX_DOOM_GLOW_FRAMES;
        ex->Hi_DrawEffModel(&rot, &pos, (s32)&sc, work->model38);
        ex->Hi_DrawEffModel(&rot, &pos, (s32)&sc, work->model3C);
        ex->Hi_SetEffModelOffset(work->model58, 0x28);
        ex->Hi_DrawEffModel(&rot, &pos, (s32)&sc, work->model58);
        ex->Hi_SetEffModelOffset(work->model54, 0x14);
        ex->Hi_DrawEffModel(&rot, &pos, (s32)&sc, work->model54);
        ex->Hi_ModifyEffModelRGB(work->model54, ((t * 3 / 4 + 0x400) >> 5) - 0x80, ((t * 3 / 4 + 0x400) >> 5) - 0x80,
                                 ((t * 3 / 4 + 0x400) >> 5) - 0x80);
        flicker1 = (work->frame4 & 1) + 1;
        ex->Hi_DrawEffModel(&rot, &pos, (s32)&sc, work->model40[flicker1]);
        ex->Hi_ModifyEffModelRGB(work->model40[flicker1], ((0x1000 - t / 2) >> 5) - 0x80, ((0x1000 - t / 2) >> 5) - 0x80,
                                 ((0x1000 - t / 2) >> 5) - 0x80);
        if (frame >= FX_DOOM_GLOW_FRAMES) {
            work->state0 = FX_DOOM_STATE_PICTURE;
            entry->frame = -1;
        }
        break;
    case FX_DOOM_STATE_PICTURE:
        sc.vx = sc.vy = sc.vz = work->scale5C;
        rot.vx = work->rot10.vx;
        rot.vy = work->rot10.vy;
        rot.vz = work->rot10.vz;
        pos.vx = work->pos18.vx;
        pos.vy = work->pos18.vy;
        pos.vz = work->pos18.vz;
        t = (frame << 12) / FX_DOOM_PICTURE_FRAMES;
        ex->Hi_DrawEffModel(&rot, &pos, (s32)&sc, work->model38);
        ex->Hi_DrawEffModel(&rot, &pos, (s32)&sc, work->model3C);
        ex->Hi_DrawEffModel(&rot, &pos, (s32)&sc, work->model40[0]);
        if (frame < FX_DOOM_PICTURE_FADE_FRAMES) {
            ex->Hi_ModifyEffModelAbr(work->model40[0], BTLFX_ABR_ADD);
            ex->Hi_ModifyEffModelRGB(work->model40[0], ((frame << 9) >> 5) - 0x80, ((frame << 9) >> 5) - 0x80,
                                     ((frame << 9) >> 5) - 0x80);
        }
        ex->Hi_SetEffModelOffset(work->model58, 0x31);
        ex->Hi_DrawEffModel(&rot, &pos, (s32)&sc, work->model58);
        ex->Hi_SetEffModelOffset(work->model54, 0x27);
        ex->Hi_DrawEffModel(&rot, &pos, (s32)&sc, work->model54);
        {
            s32 ang = t >> 2;

            ex->Hi_ModifyEffModelRGB(work->model54, ((0x1000 - ex->math_sin(ang)) >> 5) - 0x80,
                                     ((0x1000 - ex->math_sin(ang)) >> 5) - 0x80, ((0x1000 - ex->math_sin(ang)) >> 5) - 0x80);
        }
        if (frame < FX_DOOM_SHELL_FADE_FRAMES) {
            flicker2 = (work->frame4 & 1) + 1;
            ex->Hi_DrawEffModel(&rot, &pos, (s32)&sc, work->model40[flicker2]);
            ex->Hi_ModifyEffModelRGB(work->model40[flicker2], ((0x1000 - (frame << 8)) >> 6) - 0x80,
                                     ((0x1000 - (frame << 8)) >> 6) - 0x80, ((0x1000 - (frame << 8)) >> 6) - 0x80);
        }
        if (frame == FX_DOOM_RED_RING_FRAME) {
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
                emitter = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, FX_DOOM_RED_RING), (s32)&work->emitPos28[i]);
                if (emitter != NULL) {
                    emitter->otZ = 0x40;
                }
            }
        }
        if (frame >= FX_DOOM_PICTURE_FRAMES) {
            work->state0 = FX_DOOM_STATE_SHATTER;
            entry->frame = -1;
            ex->btlfx_drawScreenTint((u32 *)env->ot + 2, BTLFX_ABR_SUB, FX_DOOM_DARKEN_RGB);
            fxm_splitModelInit(fx_0ec_splitBuf);
            fxm_splitModelAdd(work->model38);
            fxm_splitModelAdd(work->model3C);
            center[0].vx = 0;
            center[0].vy = 0;
            center[0].vz = 0;
            fxm_shatterModel(work->model4C, center);
        }
        break;
    case FX_DOOM_STATE_SHATTER:
        if (frame == FX_DOOM_FLASH_FRAME) {
            ex->btlfx_drawScreenTint((u32 *)env->ot + 2, BTLFX_ABR_ADD, FX_DOOM_RED_FLASH_RGB);
        }
        if (frame >= FX_DOOM_FLASH_FRAME) {
            if (frame == FX_DOOM_FLASH_FRAME) {
                work->pos20.vx = work->pos18.vx;
                work->pos20.vy = work->pos18.vy;
                work->pos20.vz = work->pos18.vz;
                work->pos20.vy -= (work->scale5C << 10) >> 12;
                work->emit60 = emitter = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, FX_DOOM_CYAN_SPARKLES), (s32)&work->pos18);
                if (emitter == NULL) {
                    goto skip; /* the original's NULL branch skips the unk60 check (unk60 is NULL then) */
                }
                emitter->target = (s32)&work->pos20;
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
            s32 bright;

            if (frame < FX_DOOM_FLASH_FRAME) {
                bright = (frame << 8) + 0x800;
            } else {
                bright = 0x1000;
            }
            ex->Hi_ModifyEffModelRGB(work->model4C, (bright >> 5) - 0x80, (bright >> 5) - 0x80, (bright >> 5) - 0x80);
        }
        if (frame < FX_DOOM_RELEASE_FRAMES) {
            fxm_splitRelease(1000 - (((0x1000 - ex->math_cos((frame << 10) / FX_DOOM_RELEASE_FRAMES)) * 1000) >> 12), &sv);
        }
        fxm_splitUpdate();
        fxm_shatterModelUpdate();
        if (frame >= FX_DOOM_SHATTER_FRAMES) {
            work->state0 = FX_DOOM_STATE_SHATTER;
            entry->frame = -1;
            ex->btlfx_clearEffModels();
            return 1;
        }
        break;
    }
    ex->btlfx_drawLayerTexture((BtlBgLayers *)ex->btlfx_getFileData(entry->file, FX_DOOM_BACKDROP), work->frame4);
    work->frame4++;
    }
    return 0;
}

INCLUDE_RODATA("asm/us/fx_0ec/nonmatchings/fx_0ec", fx_0ec_headerPtr);
