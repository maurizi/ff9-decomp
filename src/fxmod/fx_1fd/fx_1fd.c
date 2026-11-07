#include "fxmod/fx_1fd.h"

void fxm_projectEdgesBlend(void *trackA, void *trackB, s32 unused, s32 unusedLength, SVECTOR *scale, s32 width, s32 t, s32 rotY, s32 twist) {
    Unk800F78A4 *env = fx_1fd_env;
    Fx1fdEdge *edge = fx_1fd_ribbonEdges;
    BtlfxExports *ex = BTLFX_EXPORTS(env);
    MATRIX m;
    VECTOR world;
    SVECTOR ends[2];
    SVECTOR rot;
    SVECTOR posA;
    SVECTOR posB;
    SVECTOR pos;
    s32 i;
    s16 roll;

    ends[0].vx = 0;
    ends[0].vy = -width / 2;
    ends[0].vz = 0;
    ends[1].vx = 0;
    ends[1].vy = width / 2;
    ends[1].vz = 0;
    for (i = 0; i < FX_FENRIR_EDGES; i++, edge++) {
        fxm_posPathGet(trackA, i, &posA);
        fxm_posPathGet(trackB, i, &posB);
        ex->btlfx_lerpVec((s16 *)&posA, (s16 *)&posB, t, (s16 *)&pos);
        pos.vx = pos.vx * scale->vx / 4096;
        pos.vy = pos.vy * scale->vy / 4096;
        pos.vz = pos.vz * scale->vz / 4096;
        rot.vx = 0;
        rot.vz = 0;
        rot.vy = rotY;
        ex->math_rotMatrixYXZ(&rot, &m);
        gte_SetRotMatrix(&m);
        gte_ldtr0();
        gte_ldv0(&pos);
        gte_rtv0tr();
        gte_stlvnl(&world);
        pos.vx = world.vx;
        pos.vy = world.vy;
        pos.vz = world.vz + 0x4B0;
        gte_SetRotMatrix(env->viewMatrix);
        gte_SetTransMatrix(env->viewMatrix);
        gte_ldv0(&pos);
        gte_rtv0tr();
        gte_stlvnl(m.t);
        rot.vx = 0;
        rot.vy = 0;
        rot.vz = 0;
        if (twist != 0) {
            if (i < 0x10) {
                rot.vz = 0x400 - (i << 6);
            } else if (i >= 0x1D) {
                rot.vz = ex->math_sin(((i - 0x1C) << 7) / 4) / 4 + 0x80;
                roll = rot.vz;
                if (roll > 0x400) {
                    roll = 0x400;
                }
                rot.vz = roll;
                if (pos.vz > 0) {
                    rot.vz = -roll;
                }
            }
        } else {
            rot.vz = 0x400;
        }
        ex->math_rotMatrixYXZ(&rot, &m);
        ex->MulRotMatrix(&m);
        gte_SetRotMatrix(&m);
        gte_SetTransMatrix(&m);
        gte_ldv0(&ends[0]);
        gte_rtps();
        gte_stsxy(&edge->sxy0);
        gte_ldv0(&ends[1]);
        gte_rtps();
        gte_stsxy(&edge->sxy4);
        gte_stszotz(&edge->otz8);
    }
}

void fxm_projectEdges(void *track, s32 unused, s32 unusedLength, SVECTOR *scale, s32 width, s32 rotY) {
    Unk800F78A4 *env = fx_1fd_env;
    Fx1fdEdge *edge = fx_1fd_ribbonEdges;
    BtlfxExports *ex = BTLFX_EXPORTS(env);
    MATRIX m;
    VECTOR world;
    SVECTOR ends[2];
    SVECTOR rot;
    SVECTOR pos;
    s32 i;

    ends[0].vx = 0;
    ends[0].vy = -width / 2;
    ends[0].vz = 0;
    ends[1].vx = 0;
    ends[1].vy = width / 2;
    ends[1].vz = 0;
    for (i = 0; i < FX_FENRIR_EDGES; i++, edge++) {
        fxm_posPathGet(track, i, &pos);
        pos.vx = pos.vx * scale->vx / 4096;
        pos.vy = pos.vy * scale->vy / 4096;
        pos.vz = pos.vz * scale->vz / 4096;
        rot.vx = 0;
        rot.vy = rotY;
        rot.vz = 0;
        ex->math_rotMatrixYXZ(&rot, &m);
        gte_SetRotMatrix(&m);
        gte_ldtr0();
        gte_ldv0(&pos);
        gte_rtv0tr();
        gte_stlvnl(&world);
        pos.vx = world.vx;
        pos.vy = world.vy;
        pos.vz = world.vz + 0x4B0;
        gte_SetRotMatrix(env->viewMatrix);
        gte_SetTransMatrix(env->viewMatrix);
        gte_ldv0(&pos);
        gte_rtv0tr();
        gte_stlvnl(m.t);
        rot.vx = 0;
        rot.vy = 0;
        rot.vz = 0x400;
        ex->math_rotMatrixYXZ(&rot, &m);
        ex->MulRotMatrix(&m);
        gte_SetRotMatrix(&m);
        gte_SetTransMatrix(&m);
        gte_ldv0(&ends[0]);
        gte_rtps();
        gte_stsxy(&edge->sxy0);
        gte_ldv0(&ends[1]);
        gte_rtps();
        gte_stsxy(&edge->sxy4);
        gte_stszotz(&edge->otz8);
    }
}

void fxm_drawEdgeQuads(s32 n, s32 bright, s32 tpage, s32 clut, u16 v0, u16 v1) {
    Fx1fdEdge *edge = fx_1fd_ribbonEdges;
    POLY_GT4 *poly;
    BtlfxExports *ex;
    u16 tp;
    u16 cl;
    s32 i;
    s32 tile;
    s32 ua;
    s32 ub;
    s32 ca;
    s32 cb;
    s32 unused;

    tp = tpage | TPAGE_ABR_ADD;
    poly = (POLY_GT4 *)fx_1fd_env->prim;
    fx_1fd_env->prim = (u8 *)(poly + n);
    cl = clut;
    ex = BTLFX_EXPORTS(fx_1fd_env);
    for (i = 0; i < n - 1; i++, poly++, edge++) {
        poly->tpage = tp;
        poly->clut = cl;
        poly->code = GPU_POLY_GT4 | GPU_ABE;
        poly->x0 = edge[0].sxy0.vx;
        poly->y0 = edge[0].sxy0.vy;
        poly->x1 = edge[0].sxy4.vx;
        poly->y1 = edge[0].sxy4.vy;
        poly->x2 = edge[1].sxy0.vx;
        poly->y2 = edge[1].sxy0.vy;
        poly->x3 = edge[1].sxy4.vx;
        poly->y3 = edge[1].sxy4.vy;
        poly->v0 = poly->v2 = v0;
        poly->v1 = poly->v3 = v1;
        tile = i % 8;
        ua = tile << 5;
        ub = (tile + 1) << 5;
        if (ua == 0x100) ua = 0xFF;
        if (ub == 0x100) ub = 0xFF;
        poly->u2 = poly->u3 = ub;
        poly->u0 = poly->u1 = ua;
        if (i < 8) {
            ca = i * 16;
            cb = (i + 1) * 16;
            if (bright < ca) {
                ca = bright;
            }
            if (bright < cb) {
                cb = bright;
            }
            setRGB0(poly, ca, ca, ca);
            setRGB1(poly, ca, ca, ca);
            setRGB2(poly, cb, cb, cb);
            setRGB3(poly, cb, cb, cb);
        } else if (i >= 0x35) {
            cb = 0x80 - (i - 0x34) * 16;
            ca = 0x80 - (i - 0x33) * 16;
            if (bright < cb) {
                cb = bright;
            }
            if (bright < ca) {
                ca = bright;
            }
            setRGB0(poly, cb, cb, cb);
            setRGB1(poly, cb, cb, cb);
            setRGB2(poly, ca, ca, ca);
            setRGB3(poly, ca, ca, ca);
        } else {
            setRGB0(poly, bright, bright, bright);
            setRGB1(poly, bright, bright, bright);
            setRGB2(poly, bright, bright, bright);
            setRGB3(poly, bright, bright, bright);
        }
        if (edge[0].sxy0.vx < edge[1].sxy0.vx) {
            ex->btlfx_addPrim(PRIM_TAG(POLY_GT4), 0, poly);
        } else {
            ex->btlfx_addPrim(PRIM_TAG(POLY_GT4), (edge[0].otz8 + edge[1].otz8) >> 1, poly);
        }
    }
}

s32 fx_1fd_op80(s32 mode, Fx1fdWork80 *work, Unk800F78A4 *env, Unk80100C78Entry *entry, s32 *args) {
    BtlfxExports *ex = BTLFX_EXPORTS(env);
    s32 frame;
    s32 pathFrame;
    s32 i;
    Pool6CEntry **emitter;
    s32 j;

    if (mode == 0) {
        *args = sizeof(Fx1fdWork80);
    } else if (mode == 1) {
        fx_1fd_env = env;
        ex->btlfx_initEffModels(fx_1fd_effModels, FX_FENRIR_MODEL_COUNT);
        for (i = 0; i < FX_FENRIR_HEADS; i++) {
            fxm_posPathInit(&fx_1fd_posPath[i], fxm_offsetTableEntry((u8 *)ex->btlfx_getFileData(entry->file, FX_FENRIR_EMIT_PATHS), i));
        }
        fxm_camChanReset();
        fxm_camChanAdd(0, 1, 0, fxm_offsetTableEntry((u8 *)ex->btlfx_getFileData(entry->file, FX_FENRIR_CAM_TRACK_1), 0));
        fxm_camChanAdd(0, 2, 0, fxm_offsetTableEntry((u8 *)ex->btlfx_getFileData(entry->file, FX_FENRIR_CAM_TRACK_2), 0));
    } else {
        frame = entry->frame;
        if (frame == 0) {
            emitter = work->emit0;
            for (j = 0; j < FX_FENRIR_HEADS; j++) {
                *emitter = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, fx_1fd_emitFileIds[j]), (s32)&fx_1fd_emitPos[j]);
                if (*emitter != NULL) {
                    (*emitter)->trails = (s32)fx_1fd_op80Trails[j];
                    (*emitter)->trailSlots = FX_FENRIR_HEAD_TRAIL_SLOTS;
                    (*emitter)->trailStride = FX_FENRIR_HEAD_TRAIL_STRIDE;
                }
                emitter++;
            }
        }
        if (frame >= 0) {
            pathFrame = frame;
            for (i = 0; i < FX_FENRIR_HEADS; i++) {
                fxm_posPathGet(&fx_1fd_posPath[i], pathFrame, &work->pos14[i]);
                if (work->emit0[i] != NULL && work->emit0[i]->state == BTLFX_EMIT_DONE) {
                    work->emit0[i] = NULL;
                }
                ex->btlfx_moveEmitterTo((s32)work->emit0[i], (s32)&work->pos14[i]);
            }
        }
        fxm_camChanUpdate(0);
        return frame >= FX_FENRIR_HEADS_FRAMES;
    }
    return 0;
}

s32 fx_1fd_op81(s32 mode, void *work, Unk800F78A4 *env, Unk80100C78Entry *entry, s32 *args) {
    if (mode == 0) {
        *args = 0;
    } else if (mode == 1) {
        fx_1fd_env = env;
    } else {
        return entry->frame > 0;
    }
    return 0;
}

s32 fx_1fd_op82(s32 mode, Fx1fdWork82 *work, Unk800F78A4 *env, Unk80100C78Entry *entry, s32 *args) {
    BtlfxExports *ex = BTLFX_EXPORTS(env);
    s32 frame;
    s32 i;
    s32 j;
    s32 cycle1;
    s32 fromPath0;
    s32 toPath0;
    s32 cycle2;
    s32 frameCopy;
    s32 elapsed;
    s32 fromPath1;
    s32 toPath1;
    s32 fromPath2;
    s32 toPath2;
    s32 cycle2Cmp;
    s16 count0;
    s16 count;

    if (mode == 0) {
        *args = sizeof(Fx1fdWork82);
    } else if (mode == 1) {
        work->unk50.vx = env->targets->centerX;
        fx_1fd_env = env;
        work->unk50.vy = env->targets->centerY;
        work->unk50.vz = env->targets->centerZ;
        work->unk50.vy -= 0x3E8;
        fx_1fd_ribbon0.width1E = 0x200;
        fx_1fd_ribbon0.unk18 = 0;
        fx_1fd_ribbon0.count1A = 0;
        fx_1fd_ribbon0.bright1C = 0x80;
        fx_1fd_ribbon0.blend20 = 0;
        fx_1fd_ribbon0.angle22 = 0;
        fx_1fd_ribbon1.width1E = 0x180;
        fx_1fd_ribbon1.unk18 = 0;
        fx_1fd_ribbon1.count1A = 0;
        fx_1fd_ribbon1.bright1C = 0x80;
        fx_1fd_ribbon1.blend20 = 0;
        fx_1fd_ribbon1.angle22 = 0;
        fx_1fd_ribbon3.unk10 = 0;
        fx_1fd_ribbon3.count12 = 0;
        fx_1fd_ribbon3.bright14 = 0x80;
        fx_1fd_ribbon3.width16 = 0x400;
        fx_1fd_ribbon3.angle18 = 0;
        fx_1fd_ribbon2.count1A = 0x20;
        fx_1fd_ribbon2.unk18 = 0;
        fx_1fd_ribbon2.bright1C = 0x80;
        fx_1fd_ribbon2.width1E = 0x400;
        fx_1fd_ribbon2.blend20 = 0;
        fx_1fd_ribbon2.angle22 = 0;
        setVector(&fx_1fd_ribbon0.scale10, 0x1000, 0x1000, 0x1000);
        setVector(&fx_1fd_ribbon1.scale10, 0x1000, 0x1000, 0x1000);
        setVector(&fx_1fd_ribbon3.scale8, 0x1000, 0x1000, 0x1000);
        setVector(&fx_1fd_ribbon2.scale10, 0x1000, 0x1000, 0x1000);
        setVector(&work->rotC, 0, 0, 0);
        setVector(&work->rot14, 0, 0, 0);
        setVector(&work->pos1C, 0, 0, 0x4B0);
        setVector(&work->scale2C, 0x1CCC, 0x1000, 0x1CCC);
        work->model68 = ex->btlfx_registerEffectModel((Unk800E8F50 *)ex->btlfx_getFileData(entry->file, FX_FENRIR_WIND_RING), mode);
        work->model6A = ex->btlfx_registerEffectModel((Unk800E8F50 *)ex->btlfx_getFileData(entry->file, FX_FENRIR_WIND_RING_2), mode);
        work->timer58 = -1;
        work->timer5C = -1;
        work->timer60 = mode;
        work->frame64 = 0;
        work->speed66 = 0xC;
    } else {
        frame = entry->frame;
        switch (frame) {
        case 0:
            work->pos40.vx = 0;
            work->pos40.vy = 0;
            work->pos40.vz = 0x4B0;
            work->emit0 = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, FX_FENRIR_GREEN_RINGS_2), (s32)&work->pos40);
            break;
        case 1:
            work->emit8 = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, FX_FENRIR_CYAN_GLINTS), (s32)&work->pos40);
            break;
        case 6:
            work->pos48.vx = work->pos40.vx;
            work->pos48.vy = work->pos40.vy;
            work->pos48.vz = work->pos40.vz;
            work->pos48.vy -= 0xC80;
            work->emit4 = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, FX_FENRIR_GREEN_RINGS), (s32)&work->pos48);
            break;
        case 0x1C:
            work->timer58 = 0x20;
            work->timer5C = 0x20;
            work->timer60 = 0x20;
            break;
        }
        if (frame >= 0 && frame < FX_FENRIR_RIBBON_FRAMES) {
            fx_1fd_ribbon0.count1A += 6;
            count0 = fx_1fd_ribbon0.count1A;
            if (count0 > FX_FENRIR_RIBBON_FRAMES) {
                count0 = FX_FENRIR_RIBBON_FRAMES;
            }
            fx_1fd_ribbon0.count1A = count0;
            fx_1fd_ribbon0.blend20 = (frame % 60 % 10) * 0x199;
            if (frame % 60 < 10) {
                fromPath0 = 0;
                toPath0 = 6;
            } else if (frame % 60 < 20) {
                fromPath0 = 6;
                toPath0 = 12;
            } else if (frame % 60 < 30) {
                fromPath0 = 12;
                toPath0 = 18;
            } else if (frame % 60 < 40) {
                fromPath0 = 18;
                toPath0 = 24;
            } else if (frame % 60 < 50) {
                fromPath0 = 24;
                toPath0 = 30;
            } else if (frame % 60 < 60) {
                fromPath0 = 30;
                toPath0 = 0;
            } else {
                fromPath0 = 0;
                toPath0 = 0;
            }
            if (fx_1fd_ribbon0.count1A >= FX_FENRIR_RIBBON_FRAMES) {
                fx_1fd_ribbon0.scale10.vy = 0x1000;
                fx_1fd_ribbon0.scale10.vx += 0x80;
                fx_1fd_ribbon0.scale10.vz += 0x80;
            } else {
                fx_1fd_ribbon0.scale10.vx = fx_1fd_ribbon0.scale10.vy = fx_1fd_ribbon0.scale10.vz =
                    (s16)(fx_1fd_ribbon0.count1A / 10) * 0x199 + 0x800;
            }
            if (work->timer58 >= 0) {
                fx_1fd_ribbon0.bright1C = fxm_lerpRatio(0x80, 0, 0x20 - work->timer58, 0x20);
                work->timer58--;
            }
            for (i = 0; i < 6; i++) {
                fxm_posPathInit(&fx_1fd_ribbon0.path0, (u16 *)fxm_offsetTableEntry((u8 *)ex->btlfx_getFileData(entry->file, FX_FENRIR_RIBBON_PATHS_A), fromPath0 + i));
                fxm_posPathInit(&fx_1fd_ribbon0.path8, (u16 *)fxm_offsetTableEntry((u8 *)ex->btlfx_getFileData(entry->file, FX_FENRIR_RIBBON_PATHS_A), toPath0 + i));
                fxm_projectEdgesBlend(&fx_1fd_ribbon0.path0, &fx_1fd_ribbon0.path8, fx_1fd_ribbon0.unk18, fx_1fd_ribbon0.count1A, &fx_1fd_ribbon0.scale10,
                              fx_1fd_ribbon0.width1E, fx_1fd_ribbon0.blend20, fx_1fd_ribbon0.angle22, 1);
                fxm_drawEdgeQuads(fx_1fd_ribbon0.count1A, fx_1fd_ribbon0.bright1C, 0x97, 0x3D80, 0x80, 0xC0);
            }
        }
        if (frame >= 0 && frame < FX_FENRIR_RIBBON_FRAMES) {
            frameCopy = frame;
            fx_1fd_ribbon1.count1A += 6;
            count = fx_1fd_ribbon1.count1A;
            if (count > FX_FENRIR_RIBBON_FRAMES) {
                count = FX_FENRIR_RIBBON_FRAMES;
            }
            fx_1fd_ribbon1.count1A = count;
            if (fx_1fd_ribbon1.count1A >= FX_FENRIR_RIBBON_FRAMES) {
                fx_1fd_ribbon1.scale10.vy = 0x1000;
                fx_1fd_ribbon1.scale10.vx += 0x80;
                fx_1fd_ribbon1.scale10.vz += 0x80;
            } else {
                fx_1fd_ribbon1.scale10.vx = fx_1fd_ribbon1.scale10.vy = fx_1fd_ribbon1.scale10.vz =
                    (s16)(fx_1fd_ribbon1.count1A / 10) * 0x199 + 0x800;
            }
            if (work->timer5C >= 0) {
                fx_1fd_ribbon1.bright1C = fxm_lerpRatio(0x80, 0, 0x20 - work->timer5C, 0x20);
                work->timer5C--;
            }
            cycle1 = frameCopy;
            cycle1 %= 30;
            fx_1fd_ribbon1.blend20 = (cycle1 % 10) * 0x199;
            if (cycle1 < 10) {
                fromPath1 = 0;
                toPath1 = 3;
            } else if (cycle1 < 20) {
                fromPath1 = 3;
                toPath1 = 6;
            } else if (cycle1 < 30) {
                fromPath1 = 6;
                toPath1 = 0;
            } else {
                fromPath1 = 0;
                toPath1 = 0;
            }
            for (i = 0; i < 3; i++) {
                fxm_posPathInit(&fx_1fd_ribbon1.path0, (u16 *)fxm_offsetTableEntry((u8 *)ex->btlfx_getFileData(entry->file, FX_FENRIR_RIBBON_PATHS_B), fromPath1 + i));
                fxm_posPathInit(&fx_1fd_ribbon1.path8, (u16 *)fxm_offsetTableEntry((u8 *)ex->btlfx_getFileData(entry->file, FX_FENRIR_RIBBON_PATHS_B), toPath1 + i));
                fxm_projectEdgesBlend(&fx_1fd_ribbon1.path0, &fx_1fd_ribbon1.path8, fx_1fd_ribbon1.unk18, fx_1fd_ribbon1.count1A, &fx_1fd_ribbon1.scale10,
                              fx_1fd_ribbon1.width1E, fx_1fd_ribbon1.blend20, fx_1fd_ribbon1.angle22, 1);
                fxm_drawEdgeQuads(fx_1fd_ribbon1.count1A, fx_1fd_ribbon1.bright1C, 0x99, 0x3C80, 0x80, 0xBF);
            }
        }
        if (frame >= 0 && frame < 16) {
            fx_1fd_ribbon3.count12 += 2;
            count = fx_1fd_ribbon3.count12;
            if (count > 0x20) {
                count = 0x20;
            }
            fx_1fd_ribbon3.count12 = count;
            for (i = 0; i < 5; i++) {
                if (i & 1) {
                    fxm_posPathInit(&fx_1fd_ribbon3.path0, (u16 *)fxm_offsetTableEntry((u8 *)ex->btlfx_getFileData(entry->file, FX_FENRIR_RIBBON_PATHS_C), 0));
                } else {
                    fxm_posPathInit(&fx_1fd_ribbon3.path0, (u16 *)fxm_offsetTableEntry((u8 *)ex->btlfx_getFileData(entry->file, FX_FENRIR_RIBBON_PATHS_D), 0));
                }
                fx_1fd_ribbon3.angle18 = i * 0x333;
                fxm_projectEdges(&fx_1fd_ribbon3.path0, fx_1fd_ribbon3.unk10, fx_1fd_ribbon3.count12, &fx_1fd_ribbon3.scale8, fx_1fd_ribbon3.width16,
                              fx_1fd_ribbon3.angle18);
                fxm_drawEdgeQuads(fx_1fd_ribbon3.count12, fx_1fd_ribbon3.bright14, 0x97, 0x3D80, 0x80, 0xC0);
            }
        }
        if (frame >= 16 && frame < 60) {
            elapsed = frame - 16;
            /* separate fresh variables: the compares use cycle2Cmp, the % 8 uses cycle2 */
            cycle2 = elapsed % 24;
            cycle2Cmp = cycle2;
            fx_1fd_ribbon2.blend20 = (cycle2 % 8) << 9;
            if (cycle2Cmp < 8) {
                fromPath2 = 0;
                toPath2 = 1;
            } else if (cycle2Cmp < 16) {
                fromPath2 = 1;
                toPath2 = 2;
            } else if (cycle2Cmp < 24) {
                fromPath2 = 2;
                toPath2 = 0;
            } else {
                fromPath2 = 0;
                toPath2 = 0;
            }
            if (elapsed >= 0) {
                fx_1fd_ribbon2.scale10.vx += 0x80;
                fx_1fd_ribbon2.scale10.vz += 0x80;
            }
            if (work->timer58 >= 0) {
                fx_1fd_ribbon2.bright1C = fxm_lerpRatio(0x80, 0, 0x20 - work->timer58, 0x20);
                work->timer58--;
            }
            for (j = 0; j < 5; j++) {
                if (j & 1) {
                    fxm_posPathInit(&fx_1fd_ribbon2.path0, (u16 *)fxm_offsetTableEntry((u8 *)ex->btlfx_getFileData(entry->file, FX_FENRIR_RIBBON_PATHS_C), fromPath2));
                    fxm_posPathInit(&fx_1fd_ribbon2.path8, (u16 *)fxm_offsetTableEntry((u8 *)ex->btlfx_getFileData(entry->file, FX_FENRIR_RIBBON_PATHS_C), toPath2));
                } else {
                    fxm_posPathInit(&fx_1fd_ribbon2.path0, (u16 *)fxm_offsetTableEntry((u8 *)ex->btlfx_getFileData(entry->file, FX_FENRIR_RIBBON_PATHS_D), fromPath2));
                    fxm_posPathInit(&fx_1fd_ribbon2.path8, (u16 *)fxm_offsetTableEntry((u8 *)ex->btlfx_getFileData(entry->file, FX_FENRIR_RIBBON_PATHS_D), toPath2));
                }
                fx_1fd_ribbon2.angle22 = j * 0x333;
                fxm_projectEdgesBlend(&fx_1fd_ribbon2.path0, &fx_1fd_ribbon2.path8, fx_1fd_ribbon2.unk18, fx_1fd_ribbon2.count1A, &fx_1fd_ribbon2.scale10,
                              fx_1fd_ribbon2.width1E, fx_1fd_ribbon2.blend20, fx_1fd_ribbon2.angle22, 0);
                fxm_drawEdgeQuads(fx_1fd_ribbon2.count1A, fx_1fd_ribbon2.bright1C, 0x97, 0x3D80, 0x80, 0xC0);
            }
        }
        if (frame >= 8 && frame < 60) {
            ex->Hi_DrawEffModel(&work->rotC, &work->pos1C, (s32)&work->scale2C, work->model68);
            ex->Hi_DrawEffModel(&work->rot14, &work->pos1C, (s32)&work->scale2C, work->model6A);
            if (work->timer60 >= 0) {
                work->tint3C = fxm_lerpRatio(0, -0x80, 0x20 - work->timer60, 0x20);
                work->timer60--;
                ex->Hi_ModifyEffModelRGB(work->model68, work->tint3C, work->tint3C, work->tint3C);
                ex->Hi_ModifyEffModelRGB(work->model6A, work->tint3C, work->tint3C, work->tint3C);
            }
        }
        fxm_drawScrollMoveX(work->speed66 * FX_FENRIR_RIBBONS_FRAMES - work->frame64 * work->speed66, FX_FENRIR_WIND_VRAM_X, FX_FENRIR_WIND_VRAM_Y, FX_FENRIR_WIND_VRAM_W, FX_FENRIR_WIND_VRAM_H,
                            FX_FENRIR_WIND_VRAM_X, FX_FENRIR_WIND_VRAM_Y + FX_FENRIR_WIND_VRAM_H);
        fxm_drawScrollMoveX(work->speed66 * FX_FENRIR_RIBBONS_FRAMES - work->frame64 * work->speed66, FX_FENRIR_WIND_2_VRAM_X, FX_FENRIR_WIND_VRAM_Y, FX_FENRIR_WIND_VRAM_W, FX_FENRIR_WIND_VRAM_H,
                            FX_FENRIR_WIND_2_VRAM_X, 0x100);
        work->frame64++;
        if (frame >= FX_FENRIR_RIBBONS_FRAMES) {
            ex->btlfx_clearEffModels();
            return 1;
        }
    }
    return 0;
}

s32 fx_1fd_op83(s32 mode, SVECTOR *work, Unk800F78A4 *env, Unk80100C78Entry *entry, s32 *args) {
    BtlfxExports *ex = BTLFX_EXPORTS(env);
    SVECTOR scratch;
    SVECTOR scratch2;
    Fx1fdOrbit *orbit;
    s32 i;
    s32 j;
    Fx1fdOrbit *unitOrbit;
    s32 frame;

    if (mode == 0) {
        *args = sizeof(SVECTOR);
    } else if (mode == 1) {
        orbit = fx_1fd_unitOrbits;
        work->vx = 0;
        work->vy = 0;
        work->vz = 0x4B0;
        fx_1fd_env = env;
        for (i = 0; i < env->targets->targetCount; i++, orbit++) {
            ex->btlfx_getUnitPos(i, (s16 *)&orbit->pos0);
            ex->btlfx_getUnitRot(i, (s16 *)&orbit->rot8);
            ex->btlfx_aimAngles((s16 *)work, (s16 *)&orbit->pos0, (s16 *)&scratch);
            orbit->angle12 = scratch.vy;
            scratch2.vx = work->vx - orbit->pos0.vx;
            scratch2.vy = work->vy - orbit->pos0.vy;
            scratch2.vz = work->vz - orbit->pos0.vz;
            orbit->radius10 = ex->btlfx_sqrt(scratch2.vx * scratch2.vx + scratch2.vz * scratch2.vz, 0x14);
        }
    } else {
        frame = entry->frame;
        if (frame >= 0 && frame <= FX_FENRIR_LIFT_FRAMES) {
            for (j = 0, unitOrbit = fx_1fd_unitOrbits; j < env->targets->targetCount; j++, unitOrbit++) {
                scratch.vx = work->vx + unitOrbit->radius10 * ex->math_sin(unitOrbit->angle12) / 4096;
                scratch.vy = unitOrbit->pos0.vy - frame * 120;
                scratch.vz = work->vz + unitOrbit->radius10 * ex->math_cos(unitOrbit->angle12) / 4096;
                ex->btlfx_setUnitPos(j, (s16 *)&scratch);
                ex->btlfx_aimAngles((s16 *)work, (s16 *)&scratch, (s16 *)&scratch2);
                scratch2.vx = 0;
                scratch2.vz = 0;
                ex->btlfx_setUnitRot(j, (s16 *)&scratch2);
                unitOrbit->angle12 += 0x100;
            }
        }
        return frame >= FX_FENRIR_ORBIT_FRAMES;
    }
    return 0;
}

s32 fx_1fd_op84(s32 mode, Fx1fdWork84 *work, Unk800F78A4 *env, Unk80100C78Entry *entry, s32 *args) {
    BtlfxExports *ex = BTLFX_EXPORTS(env);
    SVECTOR *homePos;
    SVECTOR *emitPos;
    s32 frame;
    s32 i;
    s32 j;
    Fx1fdOrbit *orbit;

    if (mode == 0) {
        *args = sizeof(Fx1fdWork84);
    } else if (mode == 1) {
        orbit = fx_1fd_unitOrbits;
        fx_1fd_env = env;
        for (j = 0; j < env->targets->targetCount; j++, orbit++) {
            work->doneC0[j] = 0;
            ex->btlfx_getUnitHomePos(j, (s16 *)&work->pos40[j]);
            work->pos40[j].vy = -0xBB8;
            ex->btlfx_setUnitPos(j, (s16 *)&work->pos40[j]);
            ex->btlfx_setUnitRot(j, (s16 *)&orbit->rot8);
        }
    } else {
        frame = entry->frame;
        for (i = 0; i < env->targets->targetCount; i++) {
            if (work->doneC0[i] == 0 && fxm_moveUnitHome(i, frame, 4, &work->pos40[i]) != 0) {
                work->doneC0[i] = 1;
                ex->btlfx_getUnitHomePos(i, (s16 *)&work->pos40[i]);
                ex->btlfx_setUnitPos(i, (s16 *)&work->pos40[i]);
                homePos = (SVECTOR *)((u8 *)work + i * 8 + 0x40);
                emitPos = (SVECTOR *)((u8 *)work + i * 8 + 0x80);
                emitPos->vx = homePos->vx;
                emitPos->vy = homePos->vy;
                emitPos->vz = homePos->vz;
                work->emit0[i] = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, FX_FENRIR_GREEN_WIND_TRAIL), (s32)&work->emitPos80[i]);
                work->emit20[i] = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, FX_FENRIR_SAND_RINGS), (s32)&work->emitPos80[i]);
                if (work->emit0[i] != NULL) {
                    work->emit0[i]->trails = (s32)fx_1fd_op84Trails[i];
                    work->emit0[i]->trailSlots = FX_FENRIR_GREEN_WIND_TRAIL_TRAIL_SLOTS;
                    work->emit0[i]->trailStride = FX_FENRIR_GREEN_WIND_TRAIL_TRAIL_STRIDE;
                }
                if (work->emit20[i] != NULL) {
                    work->emit20[i]->otZ = ex->btlfx_addRadiusSixth(i, 0x80);
                }
            }
        }
        return frame >= FX_FENRIR_LAND_FRAMES;
    }
    return 0;
}

static u16 *fxm_offsetTableEntry(u8 *base, u16 idx) {
    return (u16 *)((s32)base + ((s32 *)base)[(u16)idx + 1]);
}

static void fxm_lerpSvec(SVECTOR *from, SVECTOR *to, s32 t, SVECTOR *out) {
    BTLFX_EXPORTS(fx_1fd_env)->LoadAverageShort12(from, to, 0x1000 - t, t, out);
}

static s32 fxm_lerpRatio(s32 from, s32 to, s32 t, s32 n) {
    return (from * (n - t) + to * t) / n;
}

static void fxm_posPathInit(Fx1fdPath *path, u16 *data) {
    data++;
    path->count2 = *data++;
    path->keys4 = (Fx1fdPoint *)data;
}

static s32 fxm_posPathGet(Fx1fdPath *path, s32 t, SVECTOR *out) {
    s32 last = path->count2 - 1;
    s32 done = 1;
    Fx1fdPoint *key;

    if (last < t) {
        t = last;
    } else {
        done = 0;
    }
    key = &path->keys4[t];
    out->vx = key->x;
    out->vy = key->y;
    out->vz = key->z;
    return done;
}

static void fxm_rotPosPathInit(Fx1fdPath *path, u16 *data) {
    data++;
    path->count2 = *data++;
    path->keys4 = (Fx1fdPoint *)data;
}

static s32 fxm_rotPosPathGet(Fx1fdPath_0b4 *path, s32 t, SVECTOR *rot, SVECTOR *pos) {
    Fx1fdKey *key;
    s32 done = 1;

    if (t > path->count2 - 1) {
        t = path->count2 - 1;
    } else {
        done = 0;
    }
    key = &path->keys4[t];
    if (rot != NULL) {
        rot->vx = key->rot6[0];
        rot->vy = key->rot6[1];
        rot->vz = key->rot6[2];
    }
    if (pos != NULL) {
        pos->vx = key->pos0[0];
        pos->vy = key->pos0[1];
        pos->vz = key->pos0[2];
    }
    return done;
}

static void fxm_tintScreenLerp(s32 from, s32 to, s32 t, s32 n, s32 abr, s32 ot) {
    s32 bright = (from * (n - t) + to * t) / n;

    BTLFX_EXPORTS(fx_1fd_env)->btlfx_drawScreenTint((u32 *)(fx_1fd_env->ot + ot), abr, bright, bright, bright);
}

static s32 fxm_moveUnitHome(s32 unit, s32 t, s32 n, SVECTOR *start) {
    BtlfxExports *ex = BTLFX_EXPORTS(fx_1fd_env);
    SVECTOR home;
    SVECTOR out;

    ex->btlfx_getUnitHomePos(unit, (s16 *)&home);
    fxm_lerpSvec(start, &home, (t << 12) / n, &out);
    ex->btlfx_setUnitPos(unit, (s16 *)&out);
    return t >= n;
}

static void fxm_camChanReset(void) {
    s32 i;
    s32 freeMark = FX_1FD_KEY_FREE;

    for (i = FX_FENRIR_CAM_KEY_SLOTS - 1; i >= 0; i--) {
        fx_1fd_camKeys[i].timer6 = freeMark;
    }
    for (i = FX_FENRIR_CAM_CHANNELS - 1; i >= 0; i--) {
        fx_1fd_camChanFrames[i] = 0;
    }
}

static void fxm_camChanAdd(s16 start, u16 dst, s16 ch, s16 *data) {
    s32 i;
    s32 count;

    data++;
    count = *data;
    for (i = 0; i < FX_FENRIR_CAM_KEY_SLOTS; i++) {
        if (fx_1fd_camKeys[i].timer6 == FX_1FD_KEY_FREE) {
            fx_1fd_camKeys[i].timer6 = 0;
            fx_1fd_camKeys[i].keys0 = data;
            fx_1fd_camKeys[i].startFrame4 = start;
            fx_1fd_camKeys[i].slotsA = dst;
            fx_1fd_camKeys[i].indexC = 0;
            fx_1fd_camKeys[i].countE = count;
            fx_1fd_camKeys[i].channel8 = ch;
            return;
        }
    }
}

static void fxm_camChanUpdate(s32 ch) {
    Unk800F78A4 *env = fx_1fd_env;
    s32 i;
    s32 k;
    s32 dstSlot;

    for (i = 0; i < FX_FENRIR_CAM_KEY_SLOTS; i++) {
        if (fx_1fd_camKeys[i].channel8 == ch) {
            if (fx_1fd_camKeys[i].timer6 == 0 && fx_1fd_camKeys[i].startFrame4 == fx_1fd_camChanFrames[ch]) {
                fx_1fd_camKeys[i].timer6 = *fx_1fd_camKeys[i].keys0;
                fx_1fd_camKeys[i].keys0++;
            }
            if (fx_1fd_camKeys[i].timer6 > 0) {
                fx_1fd_camKeys[i].timer6--;
                for (k = 0; k < 3; k++) {
                    dstSlot = BTLFX_CAM_SLOT(fx_1fd_camKeys[i].slotsA, k);
                    if (dstSlot >= 0) {
                        ((SVECTOR *)env->camera)[dstSlot].vx = fx_1fd_camKeys[i].keys0[0];
                        ((SVECTOR *)env->camera)[dstSlot].vy = fx_1fd_camKeys[i].keys0[1];
                        ((SVECTOR *)env->camera)[dstSlot].vz = fx_1fd_camKeys[i].keys0[2];
                        if (fx_1fd_camKeys[i].indexC < fx_1fd_camKeys[i].countE) {
                            fx_1fd_camKeys[i].keys0 += 3;
                            fx_1fd_camKeys[i].indexC++;
                        }
                    }
                }
            }
        }
    }
    fx_1fd_camChanFrames[ch]++;
}

static void fxm_drawScrollMoveX(s32 x, s32 dx, s32 dy, s32 w, s32 h, s32 sx, s32 sy) {
    s32 wrapW = x % w;
    Unk800F78A4 *env = fx_1fd_env;
    u32 *move = (u32 *)env->prim;
    BtlfxExports *ex = BTLFX_EXPORTS(env);
    s32 restW;
    RECT rectRest;
    RECT rectWrap;

    env->prim = (u8 *)(move + 12);
    restW = w - wrapW;
    if (wrapW != 0) {
        rectWrap.x = sx;
        rectWrap.y = sy;
        rectWrap.w = wrapW;
        rectWrap.h = h;
        ex->SetDrawMove(move, &rectWrap, dx + restW, dy);
        addPrim(env->ot + OT_LAST, move);
    }
    move += 6;
    if (restW != 0) {
        rectRest.x = sx + wrapW;
        rectRest.y = sy;
        rectRest.w = restW;
        rectRest.h = h;
        ex->SetDrawMove(move, &rectRest, dx, dy);
        addPrim(env->ot + OT_LAST, move);
    }
}

INCLUDE_RODATA("asm/us/fx_1fd/nonmatchings/fx_1fd", fx_1fd_headerPtr);
