#include "fxmod/fx_0d3.h"

static void fxm_drawTexelQuads(SVECTOR *pos, SVECTOR *rot, s32 tp, s32 x, s32 y, s32 div, s32 idx, s32 size, s32 w, s32 h,
                   s32 scale, s32 bright, s32 abr, u16 *colors, s32 skip) {
    Unk800F78A4 *env = fx_0d3_env;
    RECT rect;
    VECTOR sv;
    SVECTOR zero;
    SVECTOR v[4];
    CVECTOR c1;
    CVECTOR c3;
    CVECTOR c0;
    CVECTOR c2;
    MATRIX m;
    s32 z;
    s32 flag;
    DR_TPAGE *tpage;
    s32 code;
    BtlfxExports *ex;
    POLY_G4 *prim;
    u16 *row;
    u16 *p;
    s32 i;
    s32 j;
    s32 px;
    s32 py;
    s32 hh;
    s32 x0;
    s32 col;

    *(s32 *)&zero.vx = 0;
    *(s32 *)&zero.vz = 0;
    ex = BTLFX_EXPORTS(env);
    if (skip == 0) {
        /* The nesting only weights w's references for global-alloc (w gets s4 ahead of py and prim). */
        do { do { do { w++; } while (0); } while (0); } while (0);
        hh = h + 1;
        prim = (POLY_G4 *)env->prim;
        env->prim = (u8 *)(prim + w * hh);
        tpage = ex->btlfx_allocDrMode(0, abr);
        gte_SetRotMatrix(env->viewMatrix);
        gte_SetTransMatrix(env->viewMatrix);
        gte_ldv0(pos);
        if (rot == NULL) {
            rot = &zero;
        }
        gte_rtv0tr_b();
        ex->math_rotMatrixYXZ(rot, &m);
        if (scale >= 0) {
            sv.vx = sv.vy = sv.vz = scale;
            ex->ScaleMatrix(&m, &sv);
        }
        gte_stlvnl(m.t);
        gte_rtps();
        code = 0x38;
        if (abr != 0xFF) {
            code = 0x3A;
        }
        gte_stszotz(&z);
        if (rot->pad != 0) {
            ex->MulRotMatrix(&m);
        }
        gte_SetRotMatrix(&m);
        gte_SetTransMatrix(&m);
        x0 = -(w * size) >> 1;
        h = hh;
        py = -(h * size) >> 1;
        row = colors - (u32)(w - 1);
        bright <<= 2;
        gte_ldVZ012_0();
        for (j = 0; j < h; j++, py += size, row += (u32)(w - 1)) {
            px = x0;
            p = row;
            *(s32 *)&c0 = 0;
            *(s32 *)&c2 = 0;
            for (i = 0; i < w; i++, prim++, px += size, p++) {
                v[0].vx = v[2].vx = px;
                v[0].vy = v[1].vy = py;
                v[1].vx = v[3].vx = px + size;
                v[2].vy = v[3].vy = py + size;
                if (i >= w - 1) {
                    *(s32 *)&c1 = 0;
                    *(s32 *)&c3 = 0;
                } else {
                    if (j == 0) {
                        *(s32 *)&c1 = 0;
                    } else {
                        col = p[0];
                        c1.r = ((col & 0x1F) * (bright << 2)) >> 12;
                        c1.g = (((col >> 5) & 0x1F) * (bright << 2)) >> 12;
                        c1.b = (((col >> 10) & 0x1F) * (bright << 2)) >> 12;
                    }
                    if (j >= h - 1) {
                        *(s32 *)&c3 = 0;
                    } else {
                        col = p[w - 1];
                        c3.r = ((col & 0x1F) * (bright << 2)) >> 12;
                        c3.g = (((col >> 5) & 0x1F) * (bright << 2)) >> 12;
                        c3.b = (((col >> 10) & 0x1F) * (bright << 2)) >> 12;
                    }
                }
                gte_ldv3xy(&v[0], &v[1], &v[2]);
                c0.cd = code;
                *(s32 *)&prim->r0 = *(s32 *)&c0;
                gte_rtpt_b();
                *(s32 *)&prim->r1 = *(s32 *)&c1;
                *(s32 *)&prim->r2 = *(s32 *)&c2;
                gte_stsxy3(&prim->x0, &prim->x1, &prim->x2);
                gte_stopz(&flag);
                gte_ldv0xy(&v[3]);
                *(s32 *)&prim->r3 = *(s32 *)&c3;
                gte_rtps_b();
                *(s32 *)&c2 = *(s32 *)&c3;
                *(s32 *)&c0 = *(s32 *)&c1;
                gte_stsxy(&prim->x3);
                if (flag != 0) {
                    ex->btlfx_addPrim(0x8000000, z, prim);
                }
            }
        }
        ex->btlfx_addPrim(0x1000000, z, tpage);
    }
    w--;
    h--;
    x0 = (tp & 0xF) * 64 + x + (idx % div) * w;
    py = (tp & 0x10) * 16 + y + (idx / div) * h;
    rect.x = x0;
    rect.y = py;
    rect.w = w;
    rect.h = h;
    ex->StoreImage(&rect, (u32 *)colors);
}

static void fxm_drawRadialScreenGrid(u32 ot, s32 x, s32 y, s32 n, s32 m, s32 dr, s32 from, s32 to, RECT *src,
                   void (*fn)(s32, s32, s32, s32, DVECTOR *)) {
    DVECTOR pts[2][18];
    DVECTOR a;
    DVECTOR b;
    DVECTOR c;
    DVECTOR d;
    DVECTOR e;
    DVECTOR f;
    DVECTOR g;
    DVECTOR h;
    CVECTOR c0;
    CVECTOR c1;
    Unk800F78A4 *env = fx_0d3_env;
    u32 *move = NULL;
    s32 step;
    s32 cx;
    s32 cy;
    s32 tp0;
    s32 tp1;
    s32 flag = 0;
    BtlfxExports *ex;
    POLY_GT4 *prim;
    s32 t;
    s32 ang;
    s32 k;
    DVECTOR *p;
    s32 r;
    s32 i;
    s32 j;
    s32 sx;
    s32 sy;

    prim = (POLY_GT4 *)env->prim;
    env->prim = (u8 *)(prim + n * m);
    t = 0xFF;
    ex = BTLFX_EXPORTS(env);
    if (src == NULL) {
        t = 1;
    }
    c0.cd = t != 0xFF ? 0x3E : 0x3C;
    if (src != NULL) {
        tp0 = getTPage(2, 0, src->x, src->y);
        tp1 = getTPage(2, 0, src->x + 0x80, src->y);
        sx = src->x;
        sy = src->y;
        src->x = env->drawEnv->clip.x + (x - (src->w >> 1));
        src->y = env->drawEnv->clip.y + (y - (src->h >> 1));
        move = (u32 *)env->prim;
        env->prim = (u8 *)move + 0x18;
        ex->SetDrawMove(move, src, sx, sy);
        cx = (src->w >> 1) + (sx & 0x3F);
        cy = (src->h >> 1) + (sy & 0xFF);
    } else {
        cx = x;
        cy = y;
        tp0 = getTPage(2, 1, env->dispEnv->disp.x, env->dispEnv->disp.y);
        tp1 = getTPage(2, 1, env->dispEnv->disp.x + 0x80, env->dispEnv->disp.y);
    }
    p = pts[1];
    tp0 <<= 16;
    tp1 <<= 16;
    for (i = 0; i < 17; i++) {
        *(s32 *)&pts[0][i] = 0;
        p[i].vx = cx;
        k = i * 4 + 2;
        *(s16 *)((u8 *)p + k) = cy;
    }
    step = 0x1000 / n;
    r = 0;
    for (j = 1; j <= m; j++) {
        t = ex->btlfx_lerpRatio(from, to, j - 1, m);
        i = 0;
        ang = 0;
        c0.r = c0.g = c0.b = t;
        t = ex->btlfx_lerpRatio(from, to, j, m);
        c1.r = c1.g = c1.b = t;
        r += dr;
        for (; i <= n; i++, ang += step) {
            *(s32 *)&a = *(s32 *)&pts[0][i];
            *(s32 *)&e = *(s32 *)&pts[1][i];
            if (i < n) {
                b.vx = (ex->math_cos(ang) * r) >> 12;
                b.vy = (ex->math_sin(ang) * r) >> 12;
                f.vx = b.vx + cx;
                f.vy = b.vy + cy;
            } else {
                *(s32 *)&b = *(s32 *)&pts[0][0];
                *(s32 *)&f = *(s32 *)&pts[1][0];
            }
            if (f.vx < 0) {
                f.vx = 0;
            }
            if (f.vx >= 0x140) {
                f.vx = 0x13F;
            }
            if (f.vy < 0) {
                f.vy = 0;
            }
            if (f.vy >= 0xF0) {
                f.vy = 0xEF;
            }
            *(s32 *)&pts[1][i] = *(s32 *)&f;
            if (i != 0) {
                if (e.vx < 0x100 && f.vx < 0x100 && g.vx < 0x100 && h.vx < 0x100) {
                    t = tp0;
                    flag = 0;
                } else {
                    t = tp1;
                    flag = 1;
                    e.vx -= 0x80;
                    f.vx -= 0x80;
                    g.vx -= 0x80;
                    h.vx -= 0x80;
                }
            }
            if (i < n && fn != NULL) {
                fn(r, j, ang, i, &b);
            }
            *(s32 *)&pts[0][i] = *(s32 *)&b;
            a.vx += x;
            a.vy += y;
            b.vx += x;
            b.vy += y;
            if (i != 0) {
                *(s32 *)&prim->u0 = e.vx | (e.vy << 8);
                *(s32 *)&prim->u1 = f.vx | (f.vy << 8) | t;
                *(s32 *)&prim->u2 = g.vx | (g.vy << 8);
                *(s32 *)&prim->u3 = h.vx | (h.vy << 8);
                *(s32 *)&prim->r2 = *(s32 *)&c0;
                *(s32 *)&prim->r0 = *(s32 *)&c0;
                *(s32 *)&prim->r3 = *(s32 *)&c1;
                *(s32 *)&prim->r1 = *(s32 *)&c1;
                *(s32 *)&prim->x0 = *(s32 *)&a;
                *(s32 *)&prim->x1 = *(s32 *)&b;
                *(s32 *)&prim->x2 = *(s32 *)&c;
                *(s32 *)&prim->x3 = *(s32 *)&d;
                if (ot < 0x1000) {
                    addPrimLenIn(&env->ot[ot], prim, 0xC000000);
                }
                prim++;
            }
            if (flag) {
                e.vx += 0x80;
                f.vx += 0x80;
                g.vx += 0x80;
                h.vx += 0x80;
            }
            *(s32 *)&c = *(s32 *)&a;
            *(s32 *)&d = *(s32 *)&b;
            *(s32 *)&g = *(s32 *)&e;
            *(s32 *)&h = *(s32 *)&f;
        }
    }
    if (src != NULL && ot < 0x1000) {
        addPrimLenIn(&env->ot[ot], move, 0x5000000);
    }
}

static void fxm_keyGetVec(Fx0d3Track *track, s32 t, SVECTOR *out) {
    if (t >= track->count2) {
        t = track->count2 - 1;
    }
    track = (Fx0d3Track *)&track->keys4[t * 3];
    out->vx = ((s16 *)track)[0];
    out->vy = ((s16 *)track)[1];
    out->vz = ((s16 *)track)[2];
}

static void fxm_keyGetVec2(Fx0d3Track *track, s32 t, SVECTOR *a, SVECTOR *b) {
    s16 *k;

    if (t >= track->count2) {
        t = track->count2 - 1;
    }
    k = &track->keys4[t * 6];
    if (a != NULL) {
        a->vx = k[0];
        a->vy = k[1];
        a->vz = k[2];
    }
    if (b != NULL) {
        b->vx = k[3];
        b->vy = k[4];
        b->vz = k[5];
    }
}

static void fxm_keyGetVec3(Fx0d3Track *track, s32 t, SVECTOR *a, SVECTOR *b, SVECTOR *c) {
    s16 *k;

    if (t >= track->count2) {
        t = track->count2 - 1;
    }
    k = &track->keys4[t * 9];
    if (a != NULL) {
        a->vx = k[0];
        a->vy = k[1];
        a->vz = k[2];
    }
    if (b != NULL) {
        b->vx = k[3];
        b->vy = k[4];
        b->vz = k[5];
    }
    if (c != NULL) {
        c->vx = k[6];
        c->vy = k[7];
        c->vz = k[8];
    }
}

static s32 fxm_keyGetValue(Fx0d3Track *track, s32 t) {
    if (t >= track->count2) {
        t = track->count2 - 1;
    }
    return (track->keys4 + t)[0];
}

static void fxm_drawTexRibbonTrail(SVECTOR *a, SVECTOR *b, s32 n, s32 tpage, s32 clut, s32 u, s32 v, s32 w, s32 h, s32 p,
                   Fx0d3Seg *buf, s32 abr, s32 phase) {
    Unk800F78A4 *env = fx_0d3_env;
    BtlfxExports *ex = BTLFX_EXPORTS(env);
    CVECTOR c;
    s32 otz;
    POLY_GT4 *prim;
    Fx0d3Seg *s;
    s32 i;
    s32 t;
    s32 k;
    s32 j;

    if (p < 0) {
        p = 0x1000;
    }
    if (n > 1000) {
        n -= 999;
        for (i = 0; i < n; i++) {
            buf[i].edgeA0.pad = 0;
        }
        return;
    }
    s = &buf[(u32)(n - 1)];
    for (i = 0; i < n; i++) {
        BTLFX_COPY_SVEC(&s[1].edgeA0, &s[0].edgeA0);
        BTLFX_COPY_SVEC(&s[1].edgeB8, &s[0].edgeB8);
        s--;
    }
    s = buf;
    for (i = 0; i < n; i++, s++) {
        if (!(s[0].edgeA0.pad & s[1].edgeA0.pad)) {
            break;
        }
    }
    BTLFX_COPY_SVEC(&buf->edgeA0, a);
    buf->edgeA0.pad = 1;
    BTLFX_COPY_SVEC(&buf->edgeB8, b);
    n = i;
    prim = (POLY_GT4 *)env->prim;
    env->prim = (u8 *)(prim + n);
    c.cd = abr == 0xFF ? 0x3C : 0x3E;
    w--;
    h--;
    tpage |= (abr & 3) << 5;
    gte_SetRotMatrix(env->viewMatrix);
    gte_SetTransMatrix(env->viewMatrix);
    /* the twin of fx_0e1_0 func_801E7CB4: i = j in the increment, and the last uv store (u1) is the giv base */
    for (i = 0; i < n; i = j, buf++, prim++, phase++) {
        gte_ldv3c(buf);
        t = (i << 12) / n;
        c.r = c.g = c.b = p;
        gte_rtpt_b();
        t = ((0x1000 - t) * p) >> 12;
        c.r = c.g = c.b = t;
        *(s32 *)&prim->r0 = *(s32 *)&prim->r1 = *(s32 *)&c;
        c.r = c.g = c.b = t;
        *(s32 *)&prim->r2 = *(s32 *)&prim->r3 = *(s32 *)&c;
        j = i + 1;
        gte_stopz(&otz);
        if (otz != 0) {
            gte_stsxy3(&prim->x0, &prim->x1, &prim->x2);
            gte_avsz3();
            gte_stszotz(&otz);
            gte_ldv0(&buf[1].edgeB8);
            gte_rtps();
            ex->btlfx_addPrim(0x0C000000, otz, prim);
            gte_stsxy(&prim->x3);
            prim->clut = clut;
            prim->tpage = tpage;
            k = phase & 1;
            prim->v2 = prim->v3 = v + ((h << k) >> 1);
            prim->u0 = prim->u2 = u;
            prim->v0 = prim->v1 = v + ((k * h) >> 1);
            prim->u1 = prim->u3 = u + w;
        }
    }
}

static void fx_0d3_op80GridVertex(s32 a0, s32 frame, s32 a2, s32 a3, s16 *v) {
    frame += 7;
    v[0] = (v[0] * frame) >> 3;
    v[1] = (v[1] * frame) >> 3;
}

s32 fx_0d3_op80(s32 mode, Fx0d3Work80 *work, Unk800F78A4 *env, Unk80100C78Entry *entry, s32 *arg4) {
    BtlfxExports *ex = BTLFX_EXPORTS(env);
    SVECTOR rot;
    VECTOR pos;
    VECTOR scale;
    Pool6CEntry *p;
    s32 frame;
    s32 t;
    s32 k;
    s32 c;
    s32 m;

    if (mode == 0) {
        *arg4 = sizeof(Fx0d3Work80);
    } else if (mode == 1) {
        work->state0 = 0;
        work->frame4 = 0;
        fx_0d3_env = env;
        ex->btlfx_initEffModels(fx_0d3_effModels, 7);
        fx_0d3_sharedModel = work->model60 = ex->btlfx_registerEffectModel((Unk800E8F50 *)ex->btlfx_getFileData(entry->file, 0x16), 1);
        m = ex->btlfx_registerEffectModel((Unk800E8F50 *)ex->btlfx_getFileData(entry->file, 0x26), 1);
        work->rot28.vy = 0x800;
        work->summonScale1C.vx = 0x1000;
        work->summonScale1C.vy = 0x1000;
        work->summonScale1C.vz = 0x1000;
        work->model64 = m;
        work->rot28.vx = 0;
        work->rot28.vz = 0;
        work->summonPos10.vx = fx_0d3_summonPos.vx;
        work->summonPos10.vy = fx_0d3_summonPos.vy;
        work->summonPos10.vz = fx_0d3_summonPos.vz;
        work->summonPos10.vy -= 0x800;
        work->pos8.vx = work->summonPos10.vx;
        work->pos8.vy = work->summonPos10.vy;
        work->pos8.vz = work->summonPos10.vz;
        env->camera->points[3].vx = work->summonPos10.vx;
        env->camera->points[3].vy = work->summonPos10.vy;
        env->camera->points[3].vz = work->summonPos10.vz;
        fx_0d3_sharedModel = m;
        work->emit30 = NULL;
        work->emit34 = NULL;
        work->scroll68 = 0;
    } else {
        frame = entry->frame;
        if (work->state0 == 0) {
            t = (frame << 12) / 75;
            if (frame == 0) {
                p = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, 1), (s32)&work->pos8);
                if (p != NULL) {
                    p->otZ = 0x100;
                }
            }
            if (frame == 0x28) {
                p = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, 2), (s32)&work->pos8);
                if (p != NULL) {
                    p->trails = (s32)fx_0d3_trails;
                    p->trailSlots = 0xC0;
                    p->trailStride = 0x1C;
                    p->otZ = 0x100;
                }
            }
            if (frame == 0x32) {
                p = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, 6), (s32)&work->pos8);
                work->emit30 = p;
                if (p != NULL) {
                    p->otZ = 0x100;
                }
            }
            if (frame == 0x1E) {
                p = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, 4), (s32)&work->pos8);
                work->emit34 = p;
                if (p != NULL) {
                    p->otZ = 0x100;
                }
            }
            if (frame == 0x3C) {
                ex->Hi_SetSummonMotion(((Fx0d3Summon *)env->loader)->unk54->motion180, ((Fx0d3Summon *)env->loader)->model60);
                ex->Hi_SetSummonMotFrame(((Fx0d3Summon *)env->loader)->model60, 0);
            } else if (frame >= 0x3C) {
                k = frame - 0x3C;
                if (k < 8) {
                    ex->Hi_SetSummonMotFrame(((Fx0d3Summon *)env->loader)->model60, 0);
                }
                work->summonPos10.vy += 0xC00;
                ex->Hi_DrawSummonModel(&work->rot28, (VECTOR *)&work->summonPos10, (s32)&work->summonScale1C,
                                       ((Fx0d3Summon *)env->loader)->model60, 0);
                work->summonPos10.vy -= 0xC00;
                if (k < 8) {
                    ex->Hi_ModifySummonModelRGB(((Fx0d3Summon *)env->loader)->model60, ((k * 0xA0) >> 3) - 0xA0,
                                                ((k * 0xA0) >> 3) - 0xA0, ((k * 0xA0) >> 3) - 0xA0);
                    ex->Hi_ModifySummonModelAbr(((Fx0d3Summon *)env->loader)->model60, 1);
                }
            }
            pos.vx = work->pos8.vx;
            pos.vy = work->pos8.vy;
            pos.vz = work->pos8.vz;
            rot.vy = 0x800;
            rot.vx = 0;
            rot.vz = frame * 8;
            scale.vz = scale.vx = scale.vy = 0x800;
            ex->Hi_DrawEffModel(&rot, &pos, (s32)&scale, work->model60);
            c = (t >> 5) - 0x80;
            ex->Hi_ModifyEffModelRGB(work->model60, c, c, c);
            rot.vz = -frame * 4;
            rot.vx = 0;
            rot.vy = 0x800;
            scale.vz = scale.vx = scale.vy = 0x738;
            ex->Hi_DrawEffModel(&rot, &pos, (s32)&scale, work->model64);
            ex->Hi_ModifyEffModelRGB(work->model64, c, c, c);
            work->scroll68 += 2;
            work->scroll68 += t >> 9;
            ex->btlfx_drawScrollMove(work->scroll68, 0x240, 0x100, 0x1C, 0x70, 0x25C, 0x100);
            ex->btlfx_drawScrollMove(work->scroll68 * 2 / 3, 0x240, 0x170, 0x1C, 0x70, 0x25C, 0x170);
            fxm_drawRadialScreenGrid(2, 0xA0, 0x78, 0x10, 4, 0x29, ((ex->math_sin(t >> 2) << 5) >> 12) + 0x40, 0, 0,
                          fx_0d3_op80GridVertex);
            if (frame >= 0x4B) {
                ex->btlfx_initParticleSystem();
                return 1;
            }
        }
        work->frame4++;
    }
    return 0;
}

static s32 fx_0d3_op81SpawnPoint(SVECTOR *v) {
    s32 i = fx_0d3_spawnBoneNext;
    Unk800F78A4 *env = fx_0d3_env;
    BtlfxExports *ex = BTLFX_EXPORTS(env);

    v->pad = 1;
    ex->Hi_GetSummonBonePos(((s32 *)env->loader)[0x60 / 4], fx_0d3_spawnBones[i], (u16 *)v);
    i++;
    if (i >= 5) {
        i = 0;
    }
    fx_0d3_spawnBoneNext = i;
    return 0;
}

static void fxm_waveVertexX(s16 *v) {
    s32 t = (BTLFX_EXPORTS(fx_0d3_env)->math_sin(fx_0d3_wavePhase + (v[1] << 8)) * fx_0d3_waveAmp) >> 12;
    s32 x = v[0];

    v[0] = x + t;
}

INCLUDE_RODATA("asm/us/fx_0d3/nonmatchings/fx_0d3", fx_0d3_headerPtr);

s32 fx_0d3_op81(s32 mode, Fx0d3Work81 *work, Unk800F78A4 *env, Unk80100C78Entry *entry, s32 *arg4) {
    BtlfxExports *ex = BTLFX_EXPORTS(env);
    SVECTOR rot;
    VECTOR pos;
    VECTOR scale;
    union {
        struct {
            CVECTOR col;
            u8 pad4[4];
            SVECTOR rot;
            VECTOR sc;
        } a;
        struct {
            CVECTOR col;
            u8 pad4[4];
            SVECTOR rot;
            u8 pad[16];
            VECTOR sc;
        } b;
        struct {
            SVECTOR b0;
            SVECTOR b1;
            CVECTOR col;
            u8 pad4[4];
            SVECTOR rot;
            u8 pad[16];
            VECTOR sc;
        } c;
    } u;
    Fx0d3Summon81 *sm;
    Pool6CEntry *p;
    Pool6CEntry *p2;
    s32 sv;
    s32 frame;
    s32 t;
    s32 frame_119;
    s32 a;
    s32 a_174;
    s32 a_281;
    s32 a_309;
    s32 a_457;
    s32 c;
    s32 c_315;
    s32 c_332;
    s32 c_498;
    s32 c_516;
    s32 k;
    s32 k_181;
    s32 k_286;
    s32 k_303;
    s32 i;
    s32 s;
    s32 s_282;
    s32 s_310;
    s32 r;
    u8 *f;

    if (mode == 0) {
        *arg4 = 0x64;
    } else if (mode == 1) {
        work->state0 = 0;
        work->frame4 = 0;
        fx_0d3_env = env;
        work->model54 = ex->btlfx_registerEffectModel((Unk800E8F50 *)ex->btlfx_getFileData(entry->file, 0xD), 1);
        work->model58 = ex->btlfx_registerEffectModel((Unk800E8F50 *)ex->btlfx_getFileData(entry->file, 0xC), 1);
        work->model5C = ex->btlfx_registerEffectModel((Unk800E8F50 *)ex->btlfx_getFileData(entry->file, 0xE), 1);
        work->model60 = fx_0d3_sharedModel;
        work->pos8.vx = fx_0d3_centerPos.vx;
        work->pos8.vy = fx_0d3_centerPos.vy;
        work->pos8.vz = fx_0d3_centerPos.vz;
        work->rot28.vy = 0x800;
        work->rot28.vx = 0;
        work->rot28.vz = 0;
        work->summonScale1C.vx = 0xF00;
        work->summonScale1C.vy = 0xF00;
        work->summonScale1C.vz = 0xF00;
        work->summonPos10.vx = fx_0d3_summonPos.vx;
        work->summonPos10.vy = fx_0d3_summonPos.vy;
        work->summonPos10.vz = fx_0d3_summonPos.vz;
        work->summonPos10.vy -= 0x800;
        work->pos8.vy = work->summonPos10.vy;
        work->pos8.vx = work->summonPos10.vx;
        work->pos8.vz = work->summonPos10.vz;
        work->pos8.vy += 0x136;
        sm = (Fx0d3Summon81 *)env->loader;
        ex->Hi_SetSummonMotion(sm->unk54->motion180[0], sm->model60);
        ex->Hi_SetSummonMotFrame(((Fx0d3Summon81 *)env->loader)->model60, 0xA);
        return 0;
    } else {
    frame_119 = entry->frame;
    switch (work->state0) {
    case 0:
        t = (frame_119 << 12) / 66;
        if (frame_119 == 18) {
            ex->Hi_SetSummonMotion(((Fx0d3Summon81 *)env->loader)->unk54->motion180[1], ((Fx0d3Summon81 *)env->loader)->model60);
        }
        work->summonPos10.vy += 0xC00;
        ex->Hi_DrawSummonModel(&work->rot28, (VECTOR *)&work->summonPos10, (s32)&work->summonScale1C,
                               ((Fx0d3Summon81 *)env->loader)->model60, 1);
        work->summonPos10.vy -= 0xC00;
        if (frame_119 == 0) {
            p = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, 3), (s32)&work->pos8);
            if (p != NULL) {
                p->otZ = 0x100;
            }
            p = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, 5), (s32)&work->pos8);
            if (p != NULL) {
                p->otZ = -0x80;
            }
            p = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, 6), (s32)&work->pos8);
            if (p != NULL) {
                p->otZ = -0x80;
            }
        }
        if (frame_119 == 1) {
            p = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, 0xB), (s32)&work->pos8);
            if (p != NULL) {
                p->trails = (s32)fx_0d3_trails2;
                p->trailSlots = 0x4A;
                p->trailStride = 0x1C;
                p->otZ = 0x100;
            }
        }
        if (frame_119 == 7) {
            p = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, 0x17), (s32)&work->pos8);
            if (p != NULL) {
                p->otZ = 0x80;
            }
            p = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, 0xF), (s32)&work->pos8);
            if (p != NULL) {
                p->otZ = 0x200;
            }
        }
        if (frame_119 < 0x23) {
            pos.vx = work->pos8.vx;
            pos.vy = work->pos8.vy;
            pos.vz = work->pos8.vz - 0x200;
            rot.vx = 0;
            rot.vy = 0x800;
            rot.vz = frame_119 * 8;
            scale.vx = scale.vy = 0x555;
            scale.vz = 0x780;
            a_174 = (frame_119 << 12) / 35;
            ex->Hi_DrawEffModel(&rot, &pos, (s32)&scale, work->model60);
            a_174 >>= 2;
            ex->Hi_ModifyEffModelRGB(work->model60, (ex->math_cos(a_174) * 3 / 2 >> 5) - 0x80,
                                     (ex->math_cos(a_174) * 3 / 2 >> 5) - 0x80, (ex->math_cos(a_174) * 3 / 2 >> 5) - 0x80);
            ex->btlfx_drawScrollMove(frame_119 * 16, 0x240, 0x100, 0x1C, 0x70, 0x25C, 0x100);
            ex->btlfx_drawScrollMove(frame_119 * 8, 0x240, 0x170, 0x1C, 0x70, 0x25C, 0x170);
        }
        if (frame_119 >= 0x18) {
            k_181 = frame_119 - 0x18;
            a = (k_181 << 12) / 42;
            env->otZ = 0x80;
            f = (u8 *)ex->btlfx_getFileData(entry->file, 7);
            ex->btlfx_drawSpriteLine(f, &work->pos8, k_181, ex->math_sin(a >> 2));
            env->otZ = 0;
        }
        ex->btlfx_drawBackdrop((BtlBgLayers *)ex->btlfx_getFileData(entry->file, 0x15), frame_119);
        work->camAt40.vy = work->summonPos10.vy;
        work->camAt40.vx = work->summonPos10.vx;
        work->camAt40.vz = work->summonPos10.vz;
        work->camAt40.vy += 0x300;
        work->camAt40.vy -= (0x1000 - ex->math_sin(t >> 2)) >> 2;
        env->camera->points[3].vx = work->camAt40.vx;
        env->camera->points[3].vy = work->camAt40.vy;
        env->camera->points[3].vz = work->camAt40.vz;
        if (frame_119 >= 0x42) {
            work->state0 = 1;
            entry->frame = -1;
            work->camAt40.vx = fx_0d3_centerPos.vx;
            work->camAt40.vy = fx_0d3_centerPos.vy;
            work->camAt40.vz = fx_0d3_centerPos.vz;
            env->camera->points[3].vx = work->camAt40.vx;
            env->camera->points[3].vy = work->camAt40.vy;
            env->camera->points[3].vz = work->camAt40.vz;
        }
        break;
    case 1:
        if (frame_119 == 0) {
            ex->btlfx_initParticleSystem();
            BTLFX_COPY_SVEC(&work->pos30, &fx_0d3_centerPos);
            work->pos30.vy -= 0x1000;
            work->pos30.vz -= 0x200;
            p = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, 0x10), (s32)&work->pos30);
            if (p != NULL) {
                p->trails = (s32)fx_0d3_trails3;
                p->trailSlots = 0x40;
                p->trailStride = 0x1C;
                p->otZ = 0x80;
            }
            BTLFX_COPY_SVEC(&work->pos38, &fx_0d3_centerPos);
            work->pos38.vy -= 0x1000;
            work->pos38.vz += 0xFF;
            work->emit4C = p2 = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, 0x13), (s32)&work->pos38);
            if (p2 != NULL) {
                p2->otZ = 0x80;
            }
            work->emit50 = p2 = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, 0x14), (s32)&work->pos38);
            if (p2 != NULL) {
                p2->otZ = 0x80;
            }
        }
        if (work->emit4C != NULL && work->emit4C->state == BTLFX_EMIT_DONE) {
            work->emit4C = NULL;
        }
        if (work->emit50 != NULL && work->emit50->state == BTLFX_EMIT_DONE) {
            work->emit50 = NULL;
        }
        if (frame_119 >= 0x25) {
            work->state0 = 2;
            entry->frame = -1;
            ex->btlfx_stopEmitter(work->emit4C);
            ex->btlfx_stopEmitter(work->emit50);
            work->camAt40.vx = fx_0d3_centerPos.vx;
            work->camAt40.vy = fx_0d3_centerPos.vy;
            work->camAt40.vz = fx_0d3_centerPos.vz;
            work->camAt40.vy -= 0x200;
            env->camera->points[3].vx = work->camAt40.vx;
            env->camera->points[3].vy = work->camAt40.vy;
            env->camera->points[3].vz = work->camAt40.vz;
        }
        break;
    case 2:
        if (frame_119 == 0) {
            p = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, 0x11), (s32)&fx_0d3_centerPos);
            if (p != NULL) {
                p->otZ = 0x100;
            }
            work->pos30.vx = fx_0d3_centerPos.vx;
            work->pos30.vy = fx_0d3_centerPos.vy;
            work->pos30.vz = fx_0d3_centerPos.vz;
            work->pos30.vy -= 0x200;
            p = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, 0x18), (s32)&work->pos30);
            if (p != NULL) {
                p->otZ = 0x100;
            }
        }
        if (frame_119 == 7) {
            ex->btlfx_initParticleSystem();
            p = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, 0x12), (s32)&fx_0d3_centerPos);
            if (p != NULL) {
                p->otZ = 0x100;
            }
        }
        if (frame_119 >= 7) {
            rot.vy = frame_119 * 4;
            rot.vx = 0;
            rot.vz = 0;
            pos.vx = fx_0d3_centerPos.vx;
            pos.vy = fx_0d3_centerPos.vy;
            pos.vz = fx_0d3_centerPos.vz;
            a_281 = ((frame_119 - 7) << 12) / 25;
            s_282 = a_281 >> 4;
            scale.vy = 0x555;
            scale.vx = scale.vz = s_282 + 0x555;
            ex->Hi_DrawEffModel(&rot, &pos, (s32)&scale, work->model54);
            k_286 = frame_119 * 7;
            rot.vy = k_286;
            scale.vx = scale.vz = s_282 + 0x400;
            scale.vy = 0x800;
            ex->Hi_DrawEffModel(&rot, &pos, (s32)&scale, work->model54);
            ex->Hi_ModifyEffModelRGB(work->model54, -0x20, -0x20, -0x20);
            rot.vy = k_286;
            scale.vx = scale.vz = 0x400;
            scale.vy = (ex->math_sin(a_281 >> 2) / 2 + 0x800) >> 2;
            ex->Hi_DrawEffModel(&rot, &pos, (s32)&scale, work->model58);
            ex->btlfx_drawScrollMove(0x3E8 - frame_119 * 9, 0x2C0, 0x100, 0x40, 0x80, 0x2C0, 0x180);
            ex->btlfx_drawScrollMove(0x3E8 - k_286, 0x280, 0x100, 0x20, 0x80, 0x2A0, 0x100);
            ex->math_cos(a_281);
            ex->math_cos(a_281 << 1);
            ex->btlfx_drawRippleGrid((s32)(env->ot + 2), 0x42, 0x10, 3, 0x3C, 1, fx_0d3_rippleColors, 0x18, frame_119 * 0x190, 0x186,
                              0x100);
        }
        k_303 = frame_119 - 7;
        if ((u32)k_303 < 20) {
            rot.vx = 0x400;
            pos.vx = fx_0d3_centerPos.vx;
            pos.vy = fx_0d3_centerPos.vy;
            pos.vz = fx_0d3_centerPos.vz;
            a_309 = ((k_303 << 12) / 20) >> 2;
            s_310 = 0xC00 - (k_303 << 12) / 20;
            rot.vy = ex->math_sin(a_309) / 3;
            rot.vz = 0;
            scale.vx = scale.vy = (ex->math_sin(a_309) >> 6) + 0x249;
            scale.vz = 0x555;
            c_315 = ex->math_sin(a_309) / 15 + 2;
            sv = (s_310 >> 5) - 0x80;
            for (i = 0; i < 7; i++) {
                ex->Hi_DrawEffModel(&rot, &pos, (s32)&scale, work->model5C);
                ex->Hi_ModifyEffModelRGB(work->model5C, sv, sv, sv);
                pos.vy -= c_315;
                rot.vy = ((rot.vy * 5) >> 2) + (i << 10);
                scale.vx = scale.vy = (scale.vx * 9) >> 3;
            }
        }
        if (frame_119 == 5) {
            ex->btlfx_drawScreenTint((u32 *)(env->ot + 1), 1, 0, 0, 0x64);
        }
        if (frame_119 == 6) {
            ex->btlfx_drawScreenTint((u32 *)(env->ot + 1), 2, 0, 0x80, 0xFF);
        }
        if ((u32)(frame_119 - 7) < 8) {
            c_332 = 0xF0 - (((frame_119 - 7) * 0xF0) >> 3);
            ex->btlfx_drawScreenTint((u32 *)(env->ot + 1), 1, c_332, c_332, c_332 >> 1);
        }
        if (frame_119 >= 0x20) {
            work->state0 = 3;
            entry->frame = -1;
            work->camAt40.vx = work->summonPos10.vx;
            work->camAt40.vy = work->summonPos10.vy;
            work->camAt40.vz = work->summonPos10.vz;
            env->camera->points[3].vx = work->camAt40.vx;
            env->camera->points[3].vy = work->camAt40.vy;
            env->camera->points[3].vz = work->camAt40.vz;
            work->rot28.vx = 0x80;
        }
        break;
    case 3:
        work->summonPos10.vy += 0xC00;
        t = (frame_119 << 12) / 23;
        ex->Hi_DrawSummonModel(&work->rot28, (VECTOR *)&work->summonPos10, (s32)&work->summonScale1C,
                               ((Fx0d3Summon81 *)env->loader)->model60, 1);
        work->summonPos10.vy -= 0xC00;
        rot.vx = 8;
        rot.vy = frame_119 * 12;
        rot.vz = 0;
        scale.vx = scale.vz = 0x555;
        pos.vx = fx_0d3_summonPos.vx;
        pos.vy = fx_0d3_summonPos.vy;
        pos.vz = fx_0d3_summonPos.vz;
        pos.vy += 0x992;
        pos.vz += 0x9F6;
        scale.vy = ((t / 2 + 0x800) * 2) / 3;
        ex->Hi_DrawEffModel(&rot, &pos, (s32)&scale, work->model58);
        rot.vx = -8;
        rot.vy = -frame_119 * 4;
        rot.vz = 0;
        scale.vx = scale.vz = 0x400;
        scale.vy = (ex->math_sin(t >> 2) / 2 + 0x800) / 2;
        ex->Hi_DrawEffModel(&rot, &pos, (s32)&scale, work->model58);
        ex->math_cos(t);
        ex->math_cos(t << 1);
        ex->btlfx_drawRippleGrid((s32)(env->ot + 2), 0x42, 0x10, 2, 0x5A, 1, fx_0d3_rippleColors2, 0x18, frame_119 * 0x140, 0x186, 0x100);
        ex->btlfx_drawScrollMove(0x3E8 - frame_119 * 7, 0x2C0, 0x100, 0x40, 0x80, 0x2C0, 0x180);
        fx_0d3_waveAmp = 4;
        fx_0d3_wavePhase = t >> 1;
        u.a.col.r = u.a.col.g = u.a.col.b = (ex->math_sin(t << 1) >> 7) - 0x73;
        u.a.sc.vx = u.a.sc.vy = u.a.sc.vz = 0x1020;
        u.a.rot.vx = 0;
        u.a.rot.vy = 0;
        u.a.rot.vz = 0;
        ex->btlfx_drawScreenGrid(1, 0xA0, 0x70, 0x50, 0x1C, 4, 8, 2, 4, &u.a.rot, &u.a.sc, &u.a.col, 0,
                          (void (*)(SVECTOR *))fxm_waveVertexX);
        if (frame_119 >= 0x17) {
            work->state0 = 4;
            entry->frame = -1;
            work->rot28.vx = 0;
            work->camAt40.vx = work->summonPos10.vx;
            work->camAt40.vy = work->summonPos10.vy;
            work->camAt40.vz = work->summonPos10.vz + 0x64;
            env->camera->points[3].vx = work->camAt40.vx;
            env->camera->points[3].vy = work->camAt40.vy;
            env->camera->points[3].vz = work->camAt40.vz;
            ex->Hi_SetSummonMotion(((Fx0d3Summon81 *)env->loader)->unk54->motion180[2], ((Fx0d3Summon81 *)env->loader)->model60);
        }
        break;
    case 4:
        work->summonPos10.vy += 0xC00;
        t = (frame_119 << 12) / 14;
        ex->Hi_DrawSummonModel(&work->rot28, (VECTOR *)&work->summonPos10, (s32)&work->summonScale1C,
                               ((Fx0d3Summon81 *)env->loader)->model60, 1);
        work->summonPos10.vy -= 0xC00;
        if (frame_119 == 0) {
            work->pos30.vx = work->camAt40.vx;
            work->pos30.vy = work->camAt40.vy;
            work->pos30.vz = work->camAt40.vz;
            p = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, 0x1A), (s32)&work->pos30);
            if (p != NULL) {
                p->otZ = 0x10;
            }
            p = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, 0x1B), (s32)&work->pos30);
            if (p != NULL) {
                p->otZ = 0x10;
            }
        }
        fx_0d3_waveAmp = 4;
        fx_0d3_wavePhase = t >> 1;
        u.b.col.r = u.b.col.g = u.b.col.b = (ex->math_sin(t << 1) >> 7) - 0x73;
        u.b.sc.vx = u.b.sc.vy = u.b.sc.vz = 0x1020;
        u.b.rot.vx = 0;
        u.b.rot.vy = 0;
        u.b.rot.vz = 0;
        ex->btlfx_drawScreenGrid(3, 0xA0, 0x70, 0x50, 0x1C, 4, 8, 2, 4, &u.b.rot, &u.b.sc, &u.b.col, 0,
                          (void (*)(SVECTOR *))fxm_waveVertexX);
        ex->math_cos(t);
        ex->math_cos(t << 1);
        ex->btlfx_drawRippleGrid((s32)(env->ot + 2), 0x42, 0x10, 2, 0x5A, 1, fx_0d3_rippleColors3, 0x1D, frame_119 << 9, 0x186, 0x100);
        if (frame_119 >= 0xE) {
            work->state0 = 5;
            entry->frame = -1;
            ex->Hi_SetSummonMotion(((Fx0d3Summon81 *)env->loader)->unk54->motion180[3], ((Fx0d3Summon81 *)env->loader)->model60);
            work->summonPos10.vx = 0;
            work->summonPos10.vy = 0;
            work->summonPos10.vz = 0;
            work->rot28.vx = 0;
            work->rot28.vy = 0x800;
            work->rot28.vz = 0;
        }
        break;
    case 5:
        t = (frame_119 << 12) / 60;
        ex->Hi_DrawSummonModel(&work->rot28, (VECTOR *)&work->summonPos10, (s32)&work->summonScale1C,
                               ((Fx0d3Summon81 *)env->loader)->model60, 0);
        if (frame_119 == 0) {
            ex->btlfx_initParticleSystem();
            p = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, 0x19), (s32)&work->pos8);
            if (p != NULL) {
                p->tail->shape = -1;
                *(s32 *)&p->tail->s.module.spawnFn = (s32)fx_0d3_op81SpawnPoint;
                p->otZ = 0x20;
            }
        }
        rot.vx = 0;
        rot.vy = frame_119 * 8;
        rot.vz = 0;
        a_457 = t >> 2;
        scale.vx = scale.vz = a_457 + 0x800;
        pos.vx = fx_0d3_centerPos.vx;
        pos.vy = fx_0d3_centerPos.vy;
        pos.vz = fx_0d3_centerPos.vz;
        scale.vy = (((t >> 3) + 0x800) * 2) / 3;
        ex->Hi_DrawEffModel(&rot, &pos, (s32)&scale, work->model54);
        ex->Hi_ModifyEffModelRGB(work->model54, (ex->math_cos(a_457) >> 5) - 0x80, (ex->math_cos(a_457) >> 5) - 0x80,
                                 (ex->math_cos(a_457) >> 5) - 0x80);
        env->otZ = 0x302;
        rot.vx = 0x400;
        rot.vy = 0;
        rot.vz = 0;
        rot.pad = 1;
        fxm_drawTexelQuads(&fx_0d3_centerPos, &rot, 0x11C, 0, 0, 4, frame_119 & 0x1F, 0x190, 0xC, 0xC, a_457 + 0x1000, ex->math_cos(a_457), 1,
                      fx_0d3_texels, 0);
        env->otZ = 0;
        ex->btlfx_drawScrollMove(0x3E8 - frame_119 * 3, 0x280, 0x100, 0x20, 0x80, 0x2A0, 0x100);
        env->otZ = -0x80;
        if (frame_119 == 0) {
            for (i = 0; i < 6; i++) {
                ex->Hi_GetSummonBonePos(((Fx0d3Summon81 *)env->loader)->model60, fx_0d3_ribbonBones[i].bone0, (u16 *)&u.c.b0);
                ex->Hi_GetSummonBonePos(((Fx0d3Summon81 *)env->loader)->model60, fx_0d3_ribbonBones[i].bone4, (u16 *)&u.c.b1);
                fxm_drawTexRibbonTrail(&u.c.b0, &u.c.b1, 0x3F3, 0, 0, 0, 0, 0, 0, 0, fx_0d3_ribbonSegs[i], 1, 0);
            }
        }
        for (i = 0; i < 6; i++) {
            ex->Hi_GetSummonBonePos(((Fx0d3Summon81 *)env->loader)->model60, fx_0d3_ribbonBones[i].bone0, (u16 *)&u.c.b0);
            ex->Hi_GetSummonBonePos(((Fx0d3Summon81 *)env->loader)->model60, fx_0d3_ribbonBones[i].bone4, (u16 *)&u.c.b1);
            fxm_drawTexRibbonTrail(&u.c.b0, &u.c.b1, 0xB, 0x1C, 0x3D09, 0, 0x80, 0x40, 0x80, 0x80, fx_0d3_ribbonSegs[i], 1,
                          10000 - work->frame4);
        }
        env->otZ = 0;
        ex->btlfx_drawScrollMove(10000 - work->frame4 * 4, 0x300, 0x180, 0x10, 0x80, 0x310, 0x180);
        u.c.rot.vx = 0;
        u.c.rot.vy = 0;
        u.c.rot.vz = 0;
        u.c.col.r = u.c.col.g = u.c.col.b = ((t * 0x4D) >> 12) + 0xF;
        u.c.sc.vx = u.c.sc.vy = u.c.sc.vz = 0x1080;
        ex->btlfx_drawScreenGrid(1, 0xA0, 0x70, 0x50, 0x1C, 4, 8, 2, 4, &u.c.rot, &u.c.sc, &u.c.col, 1, NULL);
        if (frame_119 >= 0x30) {
            c_498 = (frame_119 - 0x30) * 0xFF / 12;
            ex->btlfx_drawScreenTint((u32 *)(env->ot + 1), 1, c_498, c_498, c_498);
        }
        if (frame_119 >= 0x3C) {
            work->state0 = 6;
            entry->frame = -1;
            work->camAt40.vx = fx_0d3_summonPos.vx;
            work->camAt40.vy = fx_0d3_summonPos.vy;
            work->camAt40.vz = fx_0d3_summonPos.vz;
            work->camAt40.vy -= 0x400;
            env->camera->points[3].vx = work->camAt40.vx;
            env->camera->points[3].vy = work->camAt40.vy;
            env->camera->points[3].vz = work->camAt40.vz;
        }
        break;
    case 6:
        t = (frame_119 << 12) / 87;
        if (frame_119 < 0x18) {
            c_516 = 0xFF - frame_119 * 0xFF / 24;
            ex->btlfx_drawScreenTint((u32 *)(env->ot + 1), 1, c_516, c_516, c_516);
        }
        work->camAt40.vx = fx_0d3_summonPos.vx;
        work->camAt40.vy = fx_0d3_summonPos.vy;
        work->camAt40.vz = fx_0d3_summonPos.vz;
        work->camAt40.vy -= (0x1000 - ex->math_sin(t >> 2)) >> 2;
        env->camera->points[3].vx = work->camAt40.vx;
        env->camera->points[3].vy = work->camAt40.vy;
        env->camera->points[3].vz = work->camAt40.vz;
        if (frame_119 == 0) {
            work->pos30.vx = fx_0d3_summonPos.vx;
            work->pos30.vy = fx_0d3_summonPos.vy;
            work->pos30.vz = fx_0d3_summonPos.vz - 0x100;
            work->pos30.vy -= 0x400;
            p = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, 0x1C), (s32)&work->pos30);
            if (p != NULL) {
                p->otZ = 0x10;
            }
        }
        if (frame_119 == 0x20) {
            work->pos38.vx = fx_0d3_summonPos.vx;
            work->pos38.vy = fx_0d3_summonPos.vy;
            work->pos38.vz = fx_0d3_summonPos.vz;
            work->pos30.vz -= 0x100;
            p = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, 0x1D), (s32)&work->pos38);
            if (p != NULL) {
                p->otZ = 0x10;
            }
        }
        if (frame_119 >= 0x57) {
            ex->btlfx_clearEffModels();
            return 1;
        }
        break;
    }
    work->frame4++;
    }
    return 0;
}

s32 fx_0d3_op85(s32 mode, void *work, Unk800F78A4 *env, Unk80100C78Entry *entry, s32 *arg4) {
    if (mode == 0) {
        *arg4 = 0;
    } else if (mode == 1) {
        fx_0d3_env = env;
        env->camera->paused = 0;
    } else {
        return 1;
    }
    return 0;
}

s32 fx_0d3_op87(s32 mode, SVECTOR *work, Unk800F78A4 *env, Unk80100C78Entry *entry, s32 *arg4) {
    BtlfxExports *ex = BTLFX_EXPORTS(env);
    s32 frame;

    if (mode == 0) {
        *arg4 = 0x10;
    } else if (mode == 1) {
        fx_0d3_env = env;
    } else {
        frame = entry->frame;
        fxm_keyGetVec((u8 *)ex->btlfx_getFileData(entry->file, fx_0d3_camKeyBlocks[1]), frame, &work[0]);
        env->camera->points[1].vx = work[0].vx;
        env->camera->points[1].vy = work[0].vy;
        env->camera->points[1].vz = work[0].vz;
        fxm_keyGetVec((u8 *)ex->btlfx_getFileData(entry->file, fx_0d3_camKeyBlocks[0]), frame, &work[1]);
        env->camera->points[0].vx = work[1].vx;
        env->camera->points[0].vy = work[1].vy;
        env->camera->points[0].vz = work[1].vz;
        if (frame >= fx_0d3_camKeyBlocks[2]) {
            return 1;
        }
    }
    return 0;
}
