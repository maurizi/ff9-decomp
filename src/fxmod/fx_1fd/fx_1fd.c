#include "fxmod/fx_1fd.h"

void fxm_projectEdgesBlend(void *trackA, void *trackB, s32 unused, s32 unusedLength, SVECTOR *scale, s32 w, s32 t, s32 rotY, s32 twist) {
    Unk800F78A4 *env = fx_1fd_env;
    Fx1fdEdge *e = fx_1fd_ribbonEdges;
    BtlfxExports *ex = BTLFX_EXPORTS(env);
    MATRIX m;
    VECTOR v;
    SVECTOR ends[2];
    SVECTOR rot;
    SVECTOR a;
    SVECTOR b;
    SVECTOR p;
    s32 i;
    s16 z;

    ends[0].vx = 0;
    ends[0].vy = -w / 2;
    ends[0].vz = 0;
    ends[1].vx = 0;
    ends[1].vy = w / 2;
    ends[1].vz = 0;
    for (i = 0; i < 60; i++, e++) {
        fxm_posPathGet(trackA, i, &a);
        fxm_posPathGet(trackB, i, &b);
        ex->btlfx_lerpVec((s16 *)&a, (s16 *)&b, t, (s16 *)&p);
        p.vx = p.vx * scale->vx / 4096;
        p.vy = p.vy * scale->vy / 4096;
        p.vz = p.vz * scale->vz / 4096;
        rot.vx = 0;
        rot.vz = 0;
        rot.vy = rotY;
        ex->math_rotMatrixYXZ(&rot, &m);
        gte_SetRotMatrix(&m);
        gte_ldtr0();
        gte_ldv0(&p);
        gte_rtv0tr();
        gte_stlvnl(&v);
        p.vx = v.vx;
        p.vy = v.vy;
        p.vz = v.vz + 0x4B0;
        gte_SetRotMatrix(env->viewMatrix);
        gte_SetTransMatrix(env->viewMatrix);
        gte_ldv0(&p);
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
                z = rot.vz;
                if (z > 0x400) {
                    z = 0x400;
                }
                rot.vz = z;
                if (p.vz > 0) {
                    rot.vz = -z;
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
        gte_stsxy(&e->sxy0);
        gte_ldv0(&ends[1]);
        gte_rtps();
        gte_stsxy(&e->sxy4);
        gte_stszotz(&e->otz8);
    }
}

void fxm_projectEdges(void *track, s32 unused, s32 unusedLength, SVECTOR *scale, s32 w, s32 rotY) {
    Unk800F78A4 *env = fx_1fd_env;
    Fx1fdEdge *e = fx_1fd_ribbonEdges;
    BtlfxExports *ex = BTLFX_EXPORTS(env);
    MATRIX m;
    VECTOR v;
    SVECTOR ends[2];
    SVECTOR rot;
    SVECTOR p;
    s32 i;

    ends[0].vx = 0;
    ends[0].vy = -w / 2;
    ends[0].vz = 0;
    ends[1].vx = 0;
    ends[1].vy = w / 2;
    ends[1].vz = 0;
    for (i = 0; i < 60; i++, e++) {
        fxm_posPathGet(track, i, &p);
        p.vx = p.vx * scale->vx / 4096;
        p.vy = p.vy * scale->vy / 4096;
        p.vz = p.vz * scale->vz / 4096;
        rot.vx = 0;
        rot.vy = rotY;
        rot.vz = 0;
        ex->math_rotMatrixYXZ(&rot, &m);
        gte_SetRotMatrix(&m);
        gte_ldtr0();
        gte_ldv0(&p);
        gte_rtv0tr();
        gte_stlvnl(&v);
        p.vx = v.vx;
        p.vy = v.vy;
        p.vz = v.vz + 0x4B0;
        gte_SetRotMatrix(env->viewMatrix);
        gte_SetTransMatrix(env->viewMatrix);
        gte_ldv0(&p);
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
        gte_stsxy(&e->sxy0);
        gte_ldv0(&ends[1]);
        gte_rtps();
        gte_stsxy(&e->sxy4);
        gte_stszotz(&e->otz8);
    }
}

void fxm_drawEdgeQuads(s32 n, s32 c, s32 tpage, s32 clut, u16 v0, u16 v1) {
    Fx1fdEdge *e = fx_1fd_ribbonEdges;
    POLY_GT4 *poly;
    BtlfxExports *ex;
    u16 tp;
    u16 cl;
    s32 i;
    s32 k;
    s32 ua;
    s32 ub;
    s32 ca;
    s32 cb;
    s32 z;

    tp = tpage | 0x20;
    poly = (POLY_GT4 *)fx_1fd_env->prim;
    fx_1fd_env->prim = (u8 *)(poly + n);
    cl = clut;
    ex = BTLFX_EXPORTS(fx_1fd_env);
    for (i = 0; i < n - 1; i++, poly++, e++) {
        poly->tpage = tp;
        poly->clut = cl;
        poly->code = GPU_POLY_GT4 | GPU_ABE;
        poly->x0 = e[0].sxy0.vx;
        poly->y0 = e[0].sxy0.vy;
        poly->x1 = e[0].sxy4.vx;
        poly->y1 = e[0].sxy4.vy;
        poly->x2 = e[1].sxy0.vx;
        poly->y2 = e[1].sxy0.vy;
        poly->x3 = e[1].sxy4.vx;
        poly->y3 = e[1].sxy4.vy;
        poly->v0 = poly->v2 = v0;
        poly->v1 = poly->v3 = v1;
        k = i % 8;
        ua = k << 5;
        ub = (k + 1) << 5;
        if (ua == 0x100) ua = 0xFF;
        if (ub == 0x100) ub = 0xFF;
        poly->u2 = poly->u3 = ub;
        poly->u0 = poly->u1 = ua;
        if (i < 8) {
            ca = i * 16;
            cb = (i + 1) * 16;
            if (c < ca) {
                ca = c;
            }
            if (c < cb) {
                cb = c;
            }
            setRGB0(poly, ca, ca, ca);
            setRGB1(poly, ca, ca, ca);
            setRGB2(poly, cb, cb, cb);
            setRGB3(poly, cb, cb, cb);
        } else if (i >= 0x35) {
            cb = 0x80 - (i - 0x34) * 16;
            ca = 0x80 - (i - 0x33) * 16;
            if (c < cb) {
                cb = c;
            }
            if (c < ca) {
                ca = c;
            }
            setRGB0(poly, cb, cb, cb);
            setRGB1(poly, cb, cb, cb);
            setRGB2(poly, ca, ca, ca);
            setRGB3(poly, ca, ca, ca);
        } else {
            setRGB0(poly, c, c, c);
            setRGB1(poly, c, c, c);
            setRGB2(poly, c, c, c);
            setRGB3(poly, c, c, c);
        }
        if (e[0].sxy0.vx < e[1].sxy0.vx) {
            ex->btlfx_addPrim(0x0C000000, 0, poly);
        } else {
            ex->btlfx_addPrim(0x0C000000, (e[0].otz8 + e[1].otz8) >> 1, poly);
        }
    }
}

s32 fx_1fd_op80(s32 mode, Fx1fdWork80 *work, Unk800F78A4 *env, Unk80100C78Entry *entry, s32 *args) {
    BtlfxExports *ex = BTLFX_EXPORTS(env);
    s32 frame;
    s32 t;
    s32 i;
    Pool6CEntry **emitter;
    s32 j;

    if (mode == 0) {
        *args = 0x3C;
    } else if (mode == 1) {
        fx_1fd_env = env;
        ex->btlfx_initEffModels(fx_1fd_effModels, 0x20);
        for (i = 0; i < 5; i++) {
            fxm_posPathInit(&fx_1fd_posPath[i], fxm_offsetTableEntry((u8 *)ex->btlfx_getFileData(entry->file, 0x10), i));
        }
        fxm_camChanReset();
        fxm_camChanAdd(0, 1, 0, fxm_offsetTableEntry((u8 *)ex->btlfx_getFileData(entry->file, 0xE), 0));
        fxm_camChanAdd(0, 2, 0, fxm_offsetTableEntry((u8 *)ex->btlfx_getFileData(entry->file, 0xF), 0));
    } else {
        frame = entry->frame;
        if (frame == 0) {
            emitter = work->emit0;
            for (j = 0; j < 5; j++) {
                *emitter = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, fx_1fd_emitFileIds[j]), (s32)&fx_1fd_emitPos[j]);
                if (*emitter != NULL) {
                    (*emitter)->trails = (s32)fx_1fd_op80Trails[j];
                    (*emitter)->trailSlots = 6;
                    (*emitter)->trailStride = 0x50;
                }
                emitter++;
            }
        }
        if (frame >= 0) {
            t = frame;
            for (i = 0; i < 5; i++) {
                fxm_posPathGet(&fx_1fd_posPath[i], t, &work->pos14[i]);
                if (work->emit0[i] != NULL && work->emit0[i]->state == BTLFX_EMIT_DONE) {
                    work->emit0[i] = NULL;
                }
                ex->btlfx_moveEmitterTo((s32)work->emit0[i], (s32)&work->pos14[i]);
            }
        }
        fxm_camChanUpdate(0);
        return frame >= 0x78;
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
    s32 k;
    s32 a;
    s32 b;
    s32 c;
    s32 t;
    s32 x;
    s32 a2;
    s32 b2;
    s32 a3;
    s32 b3;
    s32 r;
    s16 n;
    s16 m;

    if (mode == 0) {
        *args = 0x6C;
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
        work->model68 = ex->btlfx_registerEffectModel((Unk800E8F50 *)ex->btlfx_getFileData(entry->file, 5), mode);
        work->model6A = ex->btlfx_registerEffectModel((Unk800E8F50 *)ex->btlfx_getFileData(entry->file, 6), mode);
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
            work->emit0 = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, 8), (s32)&work->pos40);
            break;
        case 1:
            work->emit8 = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, 0x16), (s32)&work->pos40);
            break;
        case 6:
            work->pos48.vx = work->pos40.vx;
            work->pos48.vy = work->pos40.vy;
            work->pos48.vz = work->pos40.vz;
            work->pos48.vy -= 0xC80;
            work->emit4 = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, 0x11), (s32)&work->pos48);
            break;
        case 0x1C:
            work->timer58 = 0x20;
            work->timer5C = 0x20;
            work->timer60 = 0x20;
            break;
        }
        if (frame >= 0 && frame < 60) {
            fx_1fd_ribbon0.count1A += 6;
            n = fx_1fd_ribbon0.count1A;
            if (n > 60) {
                n = 60;
            }
            fx_1fd_ribbon0.count1A = n;
            fx_1fd_ribbon0.blend20 = (frame % 60 % 10) * 0x199;
            if (frame % 60 < 10) {
                a = 0;
                b = 6;
            } else if (frame % 60 < 20) {
                a = 6;
                b = 12;
            } else if (frame % 60 < 30) {
                a = 12;
                b = 18;
            } else if (frame % 60 < 40) {
                a = 18;
                b = 24;
            } else if (frame % 60 < 50) {
                a = 24;
                b = 30;
            } else if (frame % 60 < 60) {
                a = 30;
                b = 0;
            } else {
                a = 0;
                b = 0;
            }
            if (fx_1fd_ribbon0.count1A >= 60) {
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
                fxm_posPathInit(&fx_1fd_ribbon0.path0, (u16 *)fxm_offsetTableEntry((u8 *)ex->btlfx_getFileData(entry->file, 3), a + i));
                fxm_posPathInit(&fx_1fd_ribbon0.path8, (u16 *)fxm_offsetTableEntry((u8 *)ex->btlfx_getFileData(entry->file, 3), b + i));
                fxm_projectEdgesBlend(&fx_1fd_ribbon0.path0, &fx_1fd_ribbon0.path8, fx_1fd_ribbon0.unk18, fx_1fd_ribbon0.count1A, &fx_1fd_ribbon0.scale10,
                              fx_1fd_ribbon0.width1E, fx_1fd_ribbon0.blend20, fx_1fd_ribbon0.angle22, 1);
                fxm_drawEdgeQuads(fx_1fd_ribbon0.count1A, fx_1fd_ribbon0.bright1C, 0x97, 0x3D80, 0x80, 0xC0);
            }
        }
        if (frame >= 0 && frame < 60) {
            t = frame;
            fx_1fd_ribbon1.count1A += 6;
            m = fx_1fd_ribbon1.count1A;
            if (m > 60) {
                m = 60;
            }
            fx_1fd_ribbon1.count1A = m;
            if (fx_1fd_ribbon1.count1A >= 60) {
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
            k = t;
            k %= 30;
            fx_1fd_ribbon1.blend20 = (k % 10) * 0x199;
            if (k < 10) {
                a2 = 0;
                b2 = 3;
            } else if (k < 20) {
                a2 = 3;
                b2 = 6;
            } else if (k < 30) {
                a2 = 6;
                b2 = 0;
            } else {
                a2 = 0;
                b2 = 0;
            }
            for (i = 0; i < 3; i++) {
                fxm_posPathInit(&fx_1fd_ribbon1.path0, (u16 *)fxm_offsetTableEntry((u8 *)ex->btlfx_getFileData(entry->file, 4), a2 + i));
                fxm_posPathInit(&fx_1fd_ribbon1.path8, (u16 *)fxm_offsetTableEntry((u8 *)ex->btlfx_getFileData(entry->file, 4), b2 + i));
                fxm_projectEdgesBlend(&fx_1fd_ribbon1.path0, &fx_1fd_ribbon1.path8, fx_1fd_ribbon1.unk18, fx_1fd_ribbon1.count1A, &fx_1fd_ribbon1.scale10,
                              fx_1fd_ribbon1.width1E, fx_1fd_ribbon1.blend20, fx_1fd_ribbon1.angle22, 1);
                fxm_drawEdgeQuads(fx_1fd_ribbon1.count1A, fx_1fd_ribbon1.bright1C, 0x99, 0x3C80, 0x80, 0xBF);
            }
        }
        if (frame >= 0 && frame < 16) {
            fx_1fd_ribbon3.count12 += 2;
            m = fx_1fd_ribbon3.count12;
            if (m > 0x20) {
                m = 0x20;
            }
            fx_1fd_ribbon3.count12 = m;
            for (i = 0; i < 5; i++) {
                if (i & 1) {
                    fxm_posPathInit(&fx_1fd_ribbon3.path0, (u16 *)fxm_offsetTableEntry((u8 *)ex->btlfx_getFileData(entry->file, 0x12), 0));
                } else {
                    fxm_posPathInit(&fx_1fd_ribbon3.path0, (u16 *)fxm_offsetTableEntry((u8 *)ex->btlfx_getFileData(entry->file, 0x15), 0));
                }
                fx_1fd_ribbon3.angle18 = i * 0x333;
                fxm_projectEdges(&fx_1fd_ribbon3.path0, fx_1fd_ribbon3.unk10, fx_1fd_ribbon3.count12, &fx_1fd_ribbon3.scale8, fx_1fd_ribbon3.width16,
                              fx_1fd_ribbon3.angle18);
                fxm_drawEdgeQuads(fx_1fd_ribbon3.count12, fx_1fd_ribbon3.bright14, 0x97, 0x3D80, 0x80, 0xC0);
            }
        }
        if (frame >= 16 && frame < 60) {
            x = frame - 16;
            /* separate fresh variables: the compares use r, the % 8 uses c */
            c = x % 24;
            r = c;
            fx_1fd_ribbon2.blend20 = (c % 8) << 9;
            if (r < 8) {
                a3 = 0;
                b3 = 1;
            } else if (r < 16) {
                a3 = 1;
                b3 = 2;
            } else if (r < 24) {
                a3 = 2;
                b3 = 0;
            } else {
                a3 = 0;
                b3 = 0;
            }
            if (x >= 0) {
                fx_1fd_ribbon2.scale10.vx += 0x80;
                fx_1fd_ribbon2.scale10.vz += 0x80;
            }
            if (work->timer58 >= 0) {
                fx_1fd_ribbon2.bright1C = fxm_lerpRatio(0x80, 0, 0x20 - work->timer58, 0x20);
                work->timer58--;
            }
            for (j = 0; j < 5; j++) {
                if (j & 1) {
                    fxm_posPathInit(&fx_1fd_ribbon2.path0, (u16 *)fxm_offsetTableEntry((u8 *)ex->btlfx_getFileData(entry->file, 0x12), a3));
                    fxm_posPathInit(&fx_1fd_ribbon2.path8, (u16 *)fxm_offsetTableEntry((u8 *)ex->btlfx_getFileData(entry->file, 0x12), b3));
                } else {
                    fxm_posPathInit(&fx_1fd_ribbon2.path0, (u16 *)fxm_offsetTableEntry((u8 *)ex->btlfx_getFileData(entry->file, 0x15), a3));
                    fxm_posPathInit(&fx_1fd_ribbon2.path8, (u16 *)fxm_offsetTableEntry((u8 *)ex->btlfx_getFileData(entry->file, 0x15), b3));
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
        fxm_drawScrollMoveX(work->speed66 * 0x78 - work->frame64 * work->speed66, 0x1C0, 0x180, 0x80, 0x40, 0x1C0, 0x1C0);
        fxm_drawScrollMoveX(work->speed66 * 0x78 - work->frame64 * work->speed66, 0x240, 0x180, 0x80, 0x40, 0x240, 0x100);
        work->frame64++;
        if (frame >= 0x78) {
            ex->btlfx_clearEffModels();
            return 1;
        }
    }
    return 0;
}

s32 fx_1fd_op83(s32 mode, SVECTOR *work, Unk800F78A4 *env, Unk80100C78Entry *entry, s32 *args) {
    BtlfxExports *ex = BTLFX_EXPORTS(env);
    SVECTOR a;
    SVECTOR d;
    Fx1fdOrbit *o;
    s32 i;
    s32 j;
    Fx1fdOrbit *q;
    s32 frame;

    if (mode == 0) {
        *args = 8;
    } else if (mode == 1) {
        o = fx_1fd_unitOrbits;
        work->vx = 0;
        work->vy = 0;
        work->vz = 0x4B0;
        fx_1fd_env = env;
        for (i = 0; i < env->targets->targetCount; i++, o++) {
            ex->btlfx_getUnitPos(i, (s16 *)&o->pos0);
            ex->btlfx_getUnitRot(i, (s16 *)&o->rot8);
            ex->btlfx_aimAngles((s16 *)work, (s16 *)&o->pos0, (s16 *)&a);
            o->angle12 = a.vy;
            d.vx = work->vx - o->pos0.vx;
            d.vy = work->vy - o->pos0.vy;
            d.vz = work->vz - o->pos0.vz;
            o->radius10 = ex->btlfx_sqrt(d.vx * d.vx + d.vz * d.vz, 0x14);
        }
    } else {
        frame = entry->frame;
        if (frame >= 0 && frame <= 0x40) {
            for (j = 0, q = fx_1fd_unitOrbits; j < env->targets->targetCount; j++, q++) {
                a.vx = work->vx + q->radius10 * ex->math_sin(q->angle12) / 4096;
                a.vy = q->pos0.vy - frame * 120;
                a.vz = work->vz + q->radius10 * ex->math_cos(q->angle12) / 4096;
                ex->btlfx_setUnitPos(j, (s16 *)&a);
                ex->btlfx_aimAngles((s16 *)work, (s16 *)&a, (s16 *)&d);
                d.vx = 0;
                d.vz = 0;
                ex->btlfx_setUnitRot(j, (s16 *)&d);
                q->angle12 += 0x100;
            }
        }
        return frame >= 0x46;
    }
    return 0;
}

s32 fx_1fd_op84(s32 mode, Fx1fdWork84 *work, Unk800F78A4 *env, Unk80100C78Entry *entry, s32 *args) {
    BtlfxExports *ex = BTLFX_EXPORTS(env);
    SVECTOR *a;
    SVECTOR *b;
    s32 frame;
    s32 i;
    s32 j;
    Fx1fdOrbit *q;

    if (mode == 0) {
        *args = 0xD0;
    } else if (mode == 1) {
        q = fx_1fd_unitOrbits;
        fx_1fd_env = env;
        for (j = 0; j < env->targets->targetCount; j++, q++) {
            work->doneC0[j] = 0;
            ex->btlfx_getUnitHomePos(j, (s16 *)&work->pos40[j]);
            work->pos40[j].vy = -0xBB8;
            ex->btlfx_setUnitPos(j, (s16 *)&work->pos40[j]);
            ex->btlfx_setUnitRot(j, (s16 *)&q->rot8);
        }
    } else {
        frame = entry->frame;
        for (i = 0; i < env->targets->targetCount; i++) {
            if (work->doneC0[i] == 0 && fxm_moveUnitHome(i, frame, 4, &work->pos40[i]) != 0) {
                work->doneC0[i] = 1;
                ex->btlfx_getUnitHomePos(i, (s16 *)&work->pos40[i]);
                ex->btlfx_setUnitPos(i, (s16 *)&work->pos40[i]);
                a = (SVECTOR *)((u8 *)work + i * 8 + 0x40);
                b = (SVECTOR *)((u8 *)work + i * 8 + 0x80);
                b->vx = a->vx;
                b->vy = a->vy;
                b->vz = a->vz;
                work->emit0[i] = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, 0x13), (s32)&work->emitPos80[i]);
                work->emit20[i] = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, 0x14), (s32)&work->emitPos80[i]);
                if (work->emit0[i] != NULL) {
                    work->emit0[i]->trails = (s32)fx_1fd_op84Trails[i];
                    work->emit0[i]->trailSlots = 0xC;
                    work->emit0[i]->trailStride = 0x30;
                }
                if (work->emit20[i] != NULL) {
                    work->emit20[i]->otZ = ex->btlfx_addRadiusSixth(i, 0x80);
                }
            }
        }
        return frame >= 0x3C;
    }
    return 0;
}

static u16 *fxm_offsetTableEntry(u8 *base, u16 i) {
    return (u16 *)((s32)base + ((s32 *)base)[(u16)i + 1]);
}

static void fxm_lerpSvec(SVECTOR *a, SVECTOR *b, s32 t, SVECTOR *out) {
    BTLFX_EXPORTS(fx_1fd_env)->LoadAverageShort12(a, b, 0x1000 - t, t, out);
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
    Fx1fdPoint *p;

    if (last < t) {
        t = last;
    } else {
        done = 0;
    }
    p = &path->keys4[t];
    out->vx = p->x;
    out->vy = p->y;
    out->vz = p->z;
    return done;
}

static void fxm_rotPosPathInit(Fx1fdPath *path, u16 *data) {
    data++;
    path->count2 = *data++;
    path->keys4 = (Fx1fdPoint *)data;
}

static s32 fxm_rotPosPathGet(Fx1fdPath_0b4 *path, s32 i, SVECTOR *rot, SVECTOR *pos) {
    Fx1fdKey *k;
    s32 r = 1;

    if (i > path->count2 - 1) {
        i = path->count2 - 1;
    } else {
        r = 0;
    }
    k = &path->keys4[i];
    if (rot != NULL) {
        rot->vx = k->rot6[0];
        rot->vy = k->rot6[1];
        rot->vz = k->rot6[2];
    }
    if (pos != NULL) {
        pos->vx = k->pos0[0];
        pos->vy = k->pos0[1];
        pos->vz = k->pos0[2];
    }
    return r;
}

static void fxm_tintScreenLerp(s32 from, s32 to, s32 t, s32 n, s32 abr, s32 ot) {
    s32 c = (from * (n - t) + to * t) / n;

    BTLFX_EXPORTS(fx_1fd_env)->btlfx_drawScreenTint((u32 *)(fx_1fd_env->ot + ot), abr, c, c, c);
}

static s32 fxm_moveUnitHome(s32 a0, s32 t, s32 n, SVECTOR *b) {
    BtlfxExports *ex = BTLFX_EXPORTS(fx_1fd_env);
    SVECTOR a;
    SVECTOR out;

    ex->btlfx_getUnitHomePos(a0, (s16 *)&a);
    fxm_lerpSvec(b, &a, (t << 12) / n, &out);
    ex->btlfx_setUnitPos(a0, (s16 *)&out);
    return t >= n;
}

static void fxm_camChanReset(void) {
    s32 i;
    s32 m = FX_1FD_KEY_FREE;

    for (i = 63; i >= 0; i--) {
        fx_1fd_camKeys[i].timer6 = m;
    }
    for (i = 3; i >= 0; i--) {
        fx_1fd_camChanFrames[i] = 0;
    }
}

static void fxm_camChanAdd(s16 start, u16 dst, s16 ch, s16 *data) {
    s32 i;
    s32 c;

    data++;
    c = *data;
    for (i = 0; i < 64; i++) {
        if (fx_1fd_camKeys[i].timer6 == FX_1FD_KEY_FREE) {
            fx_1fd_camKeys[i].timer6 = 0;
            fx_1fd_camKeys[i].keys0 = data;
            fx_1fd_camKeys[i].startFrame4 = start;
            fx_1fd_camKeys[i].slotsA = dst;
            fx_1fd_camKeys[i].indexC = 0;
            fx_1fd_camKeys[i].countE = c;
            fx_1fd_camKeys[i].channel8 = ch;
            return;
        }
    }
}

static void fxm_camChanUpdate(s32 ch) {
    Unk800F78A4 *env = fx_1fd_env;
    s32 i;
    s32 k;
    s32 d;

    for (i = 0; i < 64; i++) {
        if (fx_1fd_camKeys[i].channel8 == ch) {
            if (fx_1fd_camKeys[i].timer6 == 0 && fx_1fd_camKeys[i].startFrame4 == fx_1fd_camChanFrames[ch]) {
                fx_1fd_camKeys[i].timer6 = *fx_1fd_camKeys[i].keys0;
                fx_1fd_camKeys[i].keys0++;
            }
            if (fx_1fd_camKeys[i].timer6 > 0) {
                fx_1fd_camKeys[i].timer6--;
                for (k = 0; k < 3; k++) {
                    d = ((fx_1fd_camKeys[i].slotsA >> (k * 4)) & 0xF) - 1;
                    if (d >= 0) {
                        ((SVECTOR *)env->camera)[d].vx = fx_1fd_camKeys[i].keys0[0];
                        ((SVECTOR *)env->camera)[d].vy = fx_1fd_camKeys[i].keys0[1];
                        ((SVECTOR *)env->camera)[d].vz = fx_1fd_camKeys[i].keys0[2];
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
    s32 m = x % w;
    Unk800F78A4 *env = fx_1fd_env;
    u32 *p = (u32 *)env->prim;
    BtlfxExports *ex = BTLFX_EXPORTS(env);
    s32 n;
    RECT r2;
    RECT r;

    env->prim = (u8 *)(p + 12);
    n = w - m;
    if (m != 0) {
        r.x = sx;
        r.y = sy;
        r.w = m;
        r.h = h;
        ex->SetDrawMove(p, &r, dx + n, dy);
        addPrim(env->ot + OT_LAST, p);
    }
    p += 6;
    if (n != 0) {
        r2.x = sx + m;
        r2.y = sy;
        r2.w = n;
        r2.h = h;
        ex->SetDrawMove(p, &r2, dx, dy);
        addPrim(env->ot + OT_LAST, p);
    }
}

INCLUDE_RODATA("asm/us/fx_1fd/nonmatchings/fx_1fd", fx_1fd_headerPtr);
