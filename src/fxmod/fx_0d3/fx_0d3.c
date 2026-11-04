#include "fxmod/fx_0d3.h"

static void fxm_drawTexelQuads(SVECTOR *pos, SVECTOR *rot, s32 tp, s32 x, s32 y, s32 div, s32 idx, s32 size, s32 w, s32 h,
                   s32 scale, s32 bright, s32 abr, u16 *colors, s32 skip) {
    Unk800F78A4 *env = fx_0d3_env;
    RECT rect;
    VECTOR sv;
    SVECTOR zero;
    SVECTOR quad[4];
    CVECTOR colTR;
    CVECTOR colBR;
    CVECTOR colTL;
    CVECTOR colBL;
    MATRIX m;
    s32 otz;
    s32 nclip;
    DR_TPAGE *tpage;
    s32 code;
    BtlfxExports *ex;
    POLY_G4 *prim;
    u16 *row;
    u16 *src;
    s32 i;
    s32 j;
    s32 px;
    s32 py;
    s32 rows;
    s32 left;
    s32 texel;

    *(s32 *)&zero.vx = 0;
    *(s32 *)&zero.vz = 0;
    ex = BTLFX_EXPORTS(env);
    if (skip == 0) {
        /* The nesting only weights w's references for global-alloc (w gets s4 ahead of py and prim). */
        do { do { do { w++; } while (0); } while (0); } while (0);
        rows = h + 1;
        prim = (POLY_G4 *)env->prim;
        env->prim = (u8 *)(prim + w * rows);
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
        gte_stszotz(&otz);
        if (rot->pad != 0) {
            ex->MulRotMatrix(&m);
        }
        gte_SetRotMatrix(&m);
        gte_SetTransMatrix(&m);
        left = -(w * size) >> 1;
        h = rows;
        py = -(h * size) >> 1;
        row = colors - (u32)(w - 1);
        bright <<= 2;
        gte_ldVZ012_0();
        for (j = 0; j < h; j++, py += size, row += (u32)(w - 1)) {
            px = left;
            src = row;
            *(s32 *)&colTL = 0;
            *(s32 *)&colBL = 0;
            for (i = 0; i < w; i++, prim++, px += size, src++) {
                quad[0].vx = quad[2].vx = px;
                quad[0].vy = quad[1].vy = py;
                quad[1].vx = quad[3].vx = px + size;
                quad[2].vy = quad[3].vy = py + size;
                if (i >= w - 1) {
                    *(s32 *)&colTR = 0;
                    *(s32 *)&colBR = 0;
                } else {
                    if (j == 0) {
                        *(s32 *)&colTR = 0;
                    } else {
                        texel = src[0];
                        colTR.r = ((texel & 0x1F) * (bright << 2)) >> 12;
                        colTR.g = (((texel >> 5) & 0x1F) * (bright << 2)) >> 12;
                        colTR.b = (((texel >> 10) & 0x1F) * (bright << 2)) >> 12;
                    }
                    if (j >= h - 1) {
                        *(s32 *)&colBR = 0;
                    } else {
                        texel = src[w - 1];
                        colBR.r = ((texel & 0x1F) * (bright << 2)) >> 12;
                        colBR.g = (((texel >> 5) & 0x1F) * (bright << 2)) >> 12;
                        colBR.b = (((texel >> 10) & 0x1F) * (bright << 2)) >> 12;
                    }
                }
                gte_ldv3xy(&quad[0], &quad[1], &quad[2]);
                colTL.cd = code;
                *(s32 *)&prim->r0 = *(s32 *)&colTL;
                gte_rtpt_b();
                *(s32 *)&prim->r1 = *(s32 *)&colTR;
                *(s32 *)&prim->r2 = *(s32 *)&colBL;
                gte_stsxy3(&prim->x0, &prim->x1, &prim->x2);
                gte_stopz(&nclip);
                gte_ldv0xy(&quad[3]);
                *(s32 *)&prim->r3 = *(s32 *)&colBR;
                gte_rtps_b();
                *(s32 *)&colBL = *(s32 *)&colBR;
                *(s32 *)&colTL = *(s32 *)&colTR;
                gte_stsxy(&prim->x3);
                if (nclip != 0) {
                    ex->btlfx_addPrim(0x8000000, otz, prim);
                }
            }
        }
        ex->btlfx_addPrim(0x1000000, otz, tpage);
    }
    w--;
    h--;
    left = (tp & TPAGE_X_MASK) * 64 + x + (idx % div) * w;
    py = (tp & TPAGE_Y_BIT) * 16 + y + (idx / div) * h;
    rect.x = left;
    rect.y = py;
    rect.w = w;
    rect.h = h;
    ex->StoreImage(&rect, (u32 *)colors);
}

static void fxm_drawRadialScreenGrid(u32 ot, s32 x, s32 y, s32 segs, s32 rings, s32 dr, s32 from, s32 to, RECT *src,
                   void (*fn)(s32, s32, s32, s32, DVECTOR *)) {
    DVECTOR pts[2][18];
    DVECTOR inPos;
    DVECTOR outPos;
    DVECTOR inPrev;
    DVECTOR outPrev;
    DVECTOR inUv;
    DVECTOR outUv;
    DVECTOR inUvPrev;
    DVECTOR outUvPrev;
    CVECTOR colIn;
    CVECTOR colOut;
    Unk800F78A4 *env = fx_0d3_env;
    u32 *move = NULL;
    s32 step;
    s32 cx;
    s32 cy;
    s32 tpLo;
    s32 tpHi;
    s32 hiPage = 0;
    BtlfxExports *ex;
    POLY_GT4 *prim;
    s32 level;
    s32 ang;
    s32 vyOff;
    DVECTOR *uvRing;
    s32 radius;
    s32 i;
    s32 j;
    s32 sx;
    s32 sy;

    prim = (POLY_GT4 *)env->prim;
    env->prim = (u8 *)(prim + segs * rings);
    level = 0xFF;
    ex = BTLFX_EXPORTS(env);
    if (src == NULL) {
        level = 1;
    }
    colIn.cd = level != 0xFF ? 0x3E : 0x3C;
    if (src != NULL) {
        tpLo = getTPage(2, 0, src->x, src->y);
        tpHi = getTPage(2, 0, src->x + 0x80, src->y);
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
        tpLo = getTPage(2, 1, env->dispEnv->disp.x, env->dispEnv->disp.y);
        tpHi = getTPage(2, 1, env->dispEnv->disp.x + 0x80, env->dispEnv->disp.y);
    }
    uvRing = pts[1];
    tpLo <<= 16;
    tpHi <<= 16;
    for (i = 0; i < 17; i++) {
        *(s32 *)&pts[0][i] = 0;
        uvRing[i].vx = cx;
        vyOff = i * 4 + 2;
        *(s16 *)((u8 *)uvRing + vyOff) = cy;
    }
    step = 0x1000 / segs;
    radius = 0;
    for (j = 1; j <= rings; j++) {
        level = ex->btlfx_lerpRatio(from, to, j - 1, rings);
        i = 0;
        ang = 0;
        colIn.r = colIn.g = colIn.b = level;
        level = ex->btlfx_lerpRatio(from, to, j, rings);
        colOut.r = colOut.g = colOut.b = level;
        radius += dr;
        for (; i <= segs; i++, ang += step) {
            *(s32 *)&inPos = *(s32 *)&pts[0][i];
            *(s32 *)&inUv = *(s32 *)&pts[1][i];
            if (i < segs) {
                outPos.vx = (ex->math_cos(ang) * radius) >> 12;
                outPos.vy = (ex->math_sin(ang) * radius) >> 12;
                outUv.vx = outPos.vx + cx;
                outUv.vy = outPos.vy + cy;
            } else {
                *(s32 *)&outPos = *(s32 *)&pts[0][0];
                *(s32 *)&outUv = *(s32 *)&pts[1][0];
            }
            if (outUv.vx < 0) {
                outUv.vx = 0;
            }
            if (outUv.vx >= 0x140) {
                outUv.vx = 0x13F;
            }
            if (outUv.vy < 0) {
                outUv.vy = 0;
            }
            if (outUv.vy >= 0xF0) {
                outUv.vy = 0xEF;
            }
            *(s32 *)&pts[1][i] = *(s32 *)&outUv;
            if (i != 0) {
                if (inUv.vx < 0x100 && outUv.vx < 0x100 && inUvPrev.vx < 0x100 && outUvPrev.vx < 0x100) {
                    level = tpLo;
                    hiPage = 0;
                } else {
                    level = tpHi;
                    hiPage = 1;
                    inUv.vx -= 0x80;
                    outUv.vx -= 0x80;
                    inUvPrev.vx -= 0x80;
                    outUvPrev.vx -= 0x80;
                }
            }
            if (i < segs && fn != NULL) {
                fn(radius, j, ang, i, &outPos);
            }
            *(s32 *)&pts[0][i] = *(s32 *)&outPos;
            inPos.vx += x;
            inPos.vy += y;
            outPos.vx += x;
            outPos.vy += y;
            if (i != 0) {
                *(s32 *)&prim->u0 = inUv.vx | (inUv.vy << 8);
                *(s32 *)&prim->u1 = outUv.vx | (outUv.vy << 8) | level;
                *(s32 *)&prim->u2 = inUvPrev.vx | (inUvPrev.vy << 8);
                *(s32 *)&prim->u3 = outUvPrev.vx | (outUvPrev.vy << 8);
                *(s32 *)&prim->r2 = *(s32 *)&colIn;
                *(s32 *)&prim->r0 = *(s32 *)&colIn;
                *(s32 *)&prim->r3 = *(s32 *)&colOut;
                *(s32 *)&prim->r1 = *(s32 *)&colOut;
                *(s32 *)&prim->x0 = *(s32 *)&inPos;
                *(s32 *)&prim->x1 = *(s32 *)&outPos;
                *(s32 *)&prim->x2 = *(s32 *)&inPrev;
                *(s32 *)&prim->x3 = *(s32 *)&outPrev;
                if (ot < 0x1000) {
                    addPrimLenIn(&env->ot[ot], prim, 0xC000000);
                }
                prim++;
            }
            if (hiPage) {
                inUv.vx += 0x80;
                outUv.vx += 0x80;
                inUvPrev.vx += 0x80;
                outUvPrev.vx += 0x80;
            }
            *(s32 *)&inPrev = *(s32 *)&inPos;
            *(s32 *)&outPrev = *(s32 *)&outPos;
            *(s32 *)&inUvPrev = *(s32 *)&inUv;
            *(s32 *)&outUvPrev = *(s32 *)&outUv;
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

static void fxm_keyGetVec2(Fx0d3Track *track, s32 t, SVECTOR *out0, SVECTOR *out1) {
    s16 *key;

    if (t >= track->count2) {
        t = track->count2 - 1;
    }
    key = &track->keys4[t * 6];
    if (out0 != NULL) {
        out0->vx = key[0];
        out0->vy = key[1];
        out0->vz = key[2];
    }
    if (out1 != NULL) {
        out1->vx = key[3];
        out1->vy = key[4];
        out1->vz = key[5];
    }
}

static void fxm_keyGetVec3(Fx0d3Track *track, s32 t, SVECTOR *out0, SVECTOR *out1, SVECTOR *out2) {
    s16 *key;

    if (t >= track->count2) {
        t = track->count2 - 1;
    }
    key = &track->keys4[t * 9];
    if (out0 != NULL) {
        out0->vx = key[0];
        out0->vy = key[1];
        out0->vz = key[2];
    }
    if (out1 != NULL) {
        out1->vx = key[3];
        out1->vy = key[4];
        out1->vz = key[5];
    }
    if (out2 != NULL) {
        out2->vx = key[6];
        out2->vy = key[7];
        out2->vz = key[8];
    }
}

static s32 fxm_keyGetValue(Fx0d3Track *track, s32 t) {
    if (t >= track->count2) {
        t = track->count2 - 1;
    }
    return (track->keys4 + t)[0];
}

static void fxm_drawTexRibbonTrail(SVECTOR *posA, SVECTOR *posB, s32 n, s32 tpage, s32 clut, s32 u, s32 v, s32 w, s32 h, s32 bright,
                   Fx0d3Seg *buf, s32 abr, s32 phase) {
    Unk800F78A4 *env = fx_0d3_env;
    BtlfxExports *ex = BTLFX_EXPORTS(env);
    CVECTOR col;
    s32 otz;
    POLY_GT4 *prim;
    Fx0d3Seg *seg;
    s32 i;
    s32 t;
    s32 tileV;
    s32 next;

    if (bright < 0) {
        bright = 0x1000;
    }
    if (n > 1000) {
        n -= 999;
        for (i = 0; i < n; i++) {
            buf[i].edgeA0.pad = 0;
        }
        return;
    }
    seg = &buf[(u32)(n - 1)];
    for (i = 0; i < n; i++) {
        BTLFX_COPY_SVEC(&seg[1].edgeA0, &seg[0].edgeA0);
        BTLFX_COPY_SVEC(&seg[1].edgeB8, &seg[0].edgeB8);
        seg--;
    }
    seg = buf;
    for (i = 0; i < n; i++, seg++) {
        if (!(seg[0].edgeA0.pad & seg[1].edgeA0.pad)) {
            break;
        }
    }
    BTLFX_COPY_SVEC(&buf->edgeA0, posA);
    buf->edgeA0.pad = 1;
    BTLFX_COPY_SVEC(&buf->edgeB8, posB);
    n = i;
    prim = (POLY_GT4 *)env->prim;
    env->prim = (u8 *)(prim + n);
    col.cd = abr == 0xFF ? 0x3C : 0x3E;
    w--;
    h--;
    tpage |= TPAGE_ABR(abr);
    gte_SetRotMatrix(env->viewMatrix);
    gte_SetTransMatrix(env->viewMatrix);
    /* the twin of fx_0e1_0 fxm_drawTexRibbonTrail: i = next in the increment, and the last uv store (u1) is the giv base */
    for (i = 0; i < n; i = next, buf++, prim++, phase++) {
        gte_ldv3c(buf);
        t = (i << 12) / n;
        col.r = col.g = col.b = bright;
        gte_rtpt_b();
        t = ((0x1000 - t) * bright) >> 12;
        col.r = col.g = col.b = t;
        *(s32 *)&prim->r0 = *(s32 *)&prim->r1 = *(s32 *)&col;
        col.r = col.g = col.b = t;
        *(s32 *)&prim->r2 = *(s32 *)&prim->r3 = *(s32 *)&col;
        next = i + 1;
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
            tileV = phase & 1;
            prim->v2 = prim->v3 = v + ((h << tileV) >> 1);
            prim->u0 = prim->u2 = u;
            prim->v0 = prim->v1 = v + ((tileV * h) >> 1);
            prim->u1 = prim->u3 = u + w;
        }
    }
}

static void fx_0d3_op80GridVertex(s32 a0, s32 frame, s32 a2, s32 a3, s16 *v) {
    frame += 7;
    v[0] = (v[0] * frame) >> 3;
    v[1] = (v[1] * frame) >> 3;
}

s32 fx_0d3_op80(s32 mode, Fx0d3Work80 *work, Unk800F78A4 *env, Unk80100C78Entry *entry, s32 *args) {
    BtlfxExports *ex = BTLFX_EXPORTS(env);
    SVECTOR rot;
    VECTOR pos;
    VECTOR scale;
    Pool6CEntry *emitter;
    s32 frame;
    s32 t;
    s32 summonFrame;
    s32 rgb;
    s32 model;

    if (mode == 0) {
        *args = sizeof(Fx0d3Work80);
    } else if (mode == 1) {
        work->state0 = 0;
        work->frame4 = 0;
        fx_0d3_env = env;
        ex->btlfx_initEffModels(fx_0d3_effModels, 7);
        fx_0d3_sharedModel = work->model60 = ex->btlfx_registerEffectModel((Unk800E8F50 *)ex->btlfx_getFileData(entry->file, FX_PHOENIX_FLAME_SWIRL), 1);
        model = ex->btlfx_registerEffectModel((Unk800E8F50 *)ex->btlfx_getFileData(entry->file, FX_PHOENIX_FLAME_SWIRL_2), 1);
        work->rot28.vy = 0x800;
        work->summonScale1C.vx = 0x1000;
        work->summonScale1C.vy = 0x1000;
        work->summonScale1C.vz = 0x1000;
        work->model64 = model;
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
        fx_0d3_sharedModel = model;
        work->emit30 = NULL;
        work->emit34 = NULL;
        work->scroll68 = 0;
    } else {
        frame = entry->frame;
        if (work->state0 == 0) {
            t = (frame << 12) / 75;
            if (frame == 0) {
                emitter = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, FX_PHOENIX_RED_RINGS), (s32)&work->pos8);
                if (emitter != NULL) {
                    emitter->otZ = 0x100;
                }
            }
            if (frame == 0x28) {
                emitter = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, FX_PHOENIX_FIRE_TRAILS), (s32)&work->pos8);
                if (emitter != NULL) {
                    emitter->trails = (s32)fx_0d3_trails;
                    emitter->trailSlots = 0xC0;
                    emitter->trailStride = 0x1C;
                    emitter->otZ = 0x100;
                }
            }
            if (frame == 0x32) {
                emitter = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, FX_PHOENIX_FEATHERS), (s32)&work->pos8);
                work->emit30 = emitter;
                if (emitter != NULL) {
                    emitter->otZ = 0x100;
                }
            }
            if (frame == 0x1E) {
                emitter = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, FX_PHOENIX_PEACH_GLOW), (s32)&work->pos8);
                work->emit34 = emitter;
                if (emitter != NULL) {
                    emitter->otZ = 0x100;
                }
            }
            if (frame == 0x3C) {
                ex->Hi_SetSummonMotion(((Fx0d3Summon *)env->loader)->summon54->motion180, ((Fx0d3Summon *)env->loader)->model60);
                ex->Hi_SetSummonMotFrame(((Fx0d3Summon *)env->loader)->model60, 0);
            } else if (frame >= 0x3C) {
                summonFrame = frame - 0x3C;
                if (summonFrame < 8) {
                    ex->Hi_SetSummonMotFrame(((Fx0d3Summon *)env->loader)->model60, 0);
                }
                work->summonPos10.vy += 0xC00;
                ex->Hi_DrawSummonModel(&work->rot28, (VECTOR *)&work->summonPos10, (s32)&work->summonScale1C,
                                       ((Fx0d3Summon *)env->loader)->model60, 0);
                work->summonPos10.vy -= 0xC00;
                if (summonFrame < 8) {
                    ex->Hi_ModifySummonModelRGB(((Fx0d3Summon *)env->loader)->model60, ((summonFrame * 0xA0) >> 3) - 0xA0,
                                                ((summonFrame * 0xA0) >> 3) - 0xA0, ((summonFrame * 0xA0) >> 3) - 0xA0);
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
            rgb = (t >> 5) - 0x80;
            ex->Hi_ModifyEffModelRGB(work->model60, rgb, rgb, rgb);
            rot.vz = -frame * 4;
            rot.vx = 0;
            rot.vy = 0x800;
            scale.vz = scale.vx = scale.vy = 0x738;
            ex->Hi_DrawEffModel(&rot, &pos, (s32)&scale, work->model64);
            ex->Hi_ModifyEffModelRGB(work->model64, rgb, rgb, rgb);
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
    s32 boneIdx = fx_0d3_spawnBoneNext;
    Unk800F78A4 *env = fx_0d3_env;
    BtlfxExports *ex = BTLFX_EXPORTS(env);

    v->pad = 1;
    ex->Hi_GetSummonBonePos(((s32 *)env->loader)[0x60 / 4], fx_0d3_spawnBones[boneIdx], (u16 *)v);
    boneIdx++;
    if (boneIdx >= 5) {
        boneIdx = 0;
    }
    fx_0d3_spawnBoneNext = boneIdx;
    return 0;
}

static void fxm_waveVertexX(s16 *vert) {
    s32 wave = (BTLFX_EXPORTS(fx_0d3_env)->math_sin(fx_0d3_wavePhase + (vert[1] << 8)) * fx_0d3_waveAmp) >> 12;
    s32 x = vert[0];

    vert[0] = x + wave;
}

INCLUDE_RODATA("asm/us/fx_0d3/nonmatchings/fx_0d3", fx_0d3_headerPtr);

s32 fx_0d3_op81(s32 mode, Fx0d3Work81 *work, Unk800F78A4 *env, Unk80100C78Entry *entry, s32 *args) {
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
    Pool6CEntry *emitter;
    Pool6CEntry *trackedEmitter;
    s32 rgb;
    s32 frame;
    s32 t;
    s32 frame_119;
    s32 lineT;
    s32 fadeAng;
    s32 growT;
    s32 waveAng;
    s32 pulse;
    s32 c;
    s32 stepY;
    s32 tint;
    s32 fadeIn;
    s32 fadeOut;
    s32 k;
    s32 lineFrame;
    s32 spin;
    s32 waveFrame;
    s32 i;
    s32 s;
    s32 grow;
    s32 bright;
    s32 r;
    u8 *data;

    if (mode == 0) {
        *args = 0x64;
    } else if (mode == 1) {
        work->state0 = 0;
        work->frame4 = 0;
        fx_0d3_env = env;
        work->model54 = ex->btlfx_registerEffectModel((Unk800E8F50 *)ex->btlfx_getFileData(entry->file, FX_PHOENIX_FIRE_DISC), 1);
        work->model58 = ex->btlfx_registerEffectModel((Unk800E8F50 *)ex->btlfx_getFileData(entry->file, FX_PHOENIX_FIRE_DOME), 1);
        work->model5C = ex->btlfx_registerEffectModel((Unk800E8F50 *)ex->btlfx_getFileData(entry->file, FX_PHOENIX_SMOKE_ARC), 1);
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
        ex->Hi_SetSummonMotion(sm->summon54->motion180[0], sm->model60);
        ex->Hi_SetSummonMotFrame(((Fx0d3Summon81 *)env->loader)->model60, 0xA);
        return 0;
    } else {
    frame_119 = entry->frame;
    switch (work->state0) {
    case 0:
        t = (frame_119 << 12) / 66;
        if (frame_119 == 18) {
            ex->Hi_SetSummonMotion(((Fx0d3Summon81 *)env->loader)->summon54->motion180[1], ((Fx0d3Summon81 *)env->loader)->model60);
        }
        work->summonPos10.vy += 0xC00;
        ex->Hi_DrawSummonModel(&work->rot28, (VECTOR *)&work->summonPos10, (s32)&work->summonScale1C,
                               ((Fx0d3Summon81 *)env->loader)->model60, 1);
        work->summonPos10.vy -= 0xC00;
        if (frame_119 == 0) {
            emitter = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, FX_PHOENIX_GLOW_RINGS), (s32)&work->pos8);
            if (emitter != NULL) {
                emitter->otZ = 0x100;
            }
            emitter = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, FX_PHOENIX_RAINBOW_FAN), (s32)&work->pos8);
            if (emitter != NULL) {
                emitter->otZ = -0x80;
            }
            emitter = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, FX_PHOENIX_FEATHERS), (s32)&work->pos8);
            if (emitter != NULL) {
                emitter->otZ = -0x80;
            }
        }
        if (frame_119 == 1) {
            emitter = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, FX_PHOENIX_FIRE_TRAILS_2), (s32)&work->pos8);
            if (emitter != NULL) {
                emitter->trails = (s32)fx_0d3_trails2;
                emitter->trailSlots = 0x4A;
                emitter->trailStride = 0x1C;
                emitter->otZ = 0x100;
            }
        }
        if (frame_119 == 7) {
            emitter = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, FX_PHOENIX_FLAME_FEATHERS), (s32)&work->pos8);
            if (emitter != NULL) {
                emitter->otZ = 0x80;
            }
            emitter = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, FX_PHOENIX_EMBERS), (s32)&work->pos8);
            if (emitter != NULL) {
                emitter->otZ = 0x200;
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
            fadeAng = (frame_119 << 12) / 35;
            ex->Hi_DrawEffModel(&rot, &pos, (s32)&scale, work->model60);
            fadeAng >>= 2;
            ex->Hi_ModifyEffModelRGB(work->model60, (ex->math_cos(fadeAng) * 3 / 2 >> 5) - 0x80,
                                     (ex->math_cos(fadeAng) * 3 / 2 >> 5) - 0x80, (ex->math_cos(fadeAng) * 3 / 2 >> 5) - 0x80);
            ex->btlfx_drawScrollMove(frame_119 * 16, 0x240, 0x100, 0x1C, 0x70, 0x25C, 0x100);
            ex->btlfx_drawScrollMove(frame_119 * 8, 0x240, 0x170, 0x1C, 0x70, 0x25C, 0x170);
        }
        if (frame_119 >= 0x18) {
            lineFrame = frame_119 - 0x18;
            lineT = (lineFrame << 12) / 42;
            env->otZ = 0x80;
            data = (u8 *)ex->btlfx_getFileData(entry->file, FX_PHOENIX_LENS_FLARE);
            ex->btlfx_drawSpriteLine(data, &work->pos8, lineFrame, ex->math_sin(lineT >> 2));
            env->otZ = 0;
        }
        ex->btlfx_drawLayerTexture((BtlBgLayers *)ex->btlfx_getFileData(entry->file, FX_PHOENIX_BACKDROP), frame_119);
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
            emitter = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, FX_PHOENIX_FIRE_FEATHERS), (s32)&work->pos30);
            if (emitter != NULL) {
                emitter->trails = (s32)fx_0d3_trails3;
                emitter->trailSlots = 0x40;
                emitter->trailStride = 0x1C;
                emitter->otZ = 0x80;
            }
            BTLFX_COPY_SVEC(&work->pos38, &fx_0d3_centerPos);
            work->pos38.vy -= 0x1000;
            work->pos38.vz += 0xFF;
            work->emit4C = trackedEmitter = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, FX_PHOENIX_FEATHERS_2), (s32)&work->pos38);
            if (trackedEmitter != NULL) {
                trackedEmitter->otZ = 0x80;
            }
            work->emit50 = trackedEmitter = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, FX_PHOENIX_RAINBOW_FAN_2), (s32)&work->pos38);
            if (trackedEmitter != NULL) {
                trackedEmitter->otZ = 0x80;
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
            emitter = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, FX_PHOENIX_PINK_RINGS), (s32)&fx_0d3_centerPos);
            if (emitter != NULL) {
                emitter->otZ = 0x100;
            }
            work->pos30.vx = fx_0d3_centerPos.vx;
            work->pos30.vy = fx_0d3_centerPos.vy;
            work->pos30.vz = fx_0d3_centerPos.vz;
            work->pos30.vy -= 0x200;
            emitter = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, FX_PHOENIX_FLAME_FEATHERS_2), (s32)&work->pos30);
            if (emitter != NULL) {
                emitter->otZ = 0x100;
            }
        }
        if (frame_119 == 7) {
            ex->btlfx_initParticleSystem();
            emitter = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, FX_PHOENIX_GOLD_FEATHERS), (s32)&fx_0d3_centerPos);
            if (emitter != NULL) {
                emitter->otZ = 0x100;
            }
        }
        if (frame_119 >= 7) {
            rot.vy = frame_119 * 4;
            rot.vx = 0;
            rot.vz = 0;
            pos.vx = fx_0d3_centerPos.vx;
            pos.vy = fx_0d3_centerPos.vy;
            pos.vz = fx_0d3_centerPos.vz;
            growT = ((frame_119 - 7) << 12) / 25;
            grow = growT >> 4;
            scale.vy = 0x555;
            scale.vx = scale.vz = grow + 0x555;
            ex->Hi_DrawEffModel(&rot, &pos, (s32)&scale, work->model54);
            spin = frame_119 * 7;
            rot.vy = spin;
            scale.vx = scale.vz = grow + 0x400;
            scale.vy = 0x800;
            ex->Hi_DrawEffModel(&rot, &pos, (s32)&scale, work->model54);
            ex->Hi_ModifyEffModelRGB(work->model54, -0x20, -0x20, -0x20);
            rot.vy = spin;
            scale.vx = scale.vz = 0x400;
            scale.vy = (ex->math_sin(growT >> 2) / 2 + 0x800) >> 2;
            ex->Hi_DrawEffModel(&rot, &pos, (s32)&scale, work->model58);
            ex->btlfx_drawScrollMove(0x3E8 - frame_119 * 9, 0x2C0, 0x100, 0x40, 0x80, 0x2C0, 0x180);
            ex->btlfx_drawScrollMove(0x3E8 - spin, 0x280, 0x100, 0x20, 0x80, 0x2A0, 0x100);
            ex->math_cos(growT);
            ex->math_cos(growT << 1);
            ex->btlfx_drawRippleGrid((s32)(env->ot + 2), 0x42, 0x10, 3, 0x3C, 1, fx_0d3_rippleColors, 0x18, frame_119 * 0x190, 0x186,
                              0x100);
        }
        waveFrame = frame_119 - 7;
        if ((u32)waveFrame < 20) {
            rot.vx = 0x400;
            pos.vx = fx_0d3_centerPos.vx;
            pos.vy = fx_0d3_centerPos.vy;
            pos.vz = fx_0d3_centerPos.vz;
            waveAng = ((waveFrame << 12) / 20) >> 2;
            bright = 0xC00 - (waveFrame << 12) / 20;
            rot.vy = ex->math_sin(waveAng) / 3;
            rot.vz = 0;
            scale.vx = scale.vy = (ex->math_sin(waveAng) >> 6) + 0x249;
            scale.vz = 0x555;
            stepY = ex->math_sin(waveAng) / 15 + 2;
            rgb = (bright >> 5) - 0x80;
            for (i = 0; i < 7; i++) {
                ex->Hi_DrawEffModel(&rot, &pos, (s32)&scale, work->model5C);
                ex->Hi_ModifyEffModelRGB(work->model5C, rgb, rgb, rgb);
                pos.vy -= stepY;
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
            tint = 0xF0 - (((frame_119 - 7) * 0xF0) >> 3);
            ex->btlfx_drawScreenTint((u32 *)(env->ot + 1), 1, tint, tint, tint >> 1);
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
            ex->Hi_SetSummonMotion(((Fx0d3Summon81 *)env->loader)->summon54->motion180[2], ((Fx0d3Summon81 *)env->loader)->model60);
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
            emitter = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, FX_PHOENIX_EMBERS_2), (s32)&work->pos30);
            if (emitter != NULL) {
                emitter->otZ = 0x10;
            }
            emitter = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, FX_PHOENIX_COPPER_RINGS), (s32)&work->pos30);
            if (emitter != NULL) {
                emitter->otZ = 0x10;
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
            ex->Hi_SetSummonMotion(((Fx0d3Summon81 *)env->loader)->summon54->motion180[3], ((Fx0d3Summon81 *)env->loader)->model60);
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
            emitter = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, FX_PHOENIX_GOLD_FEATHERS_2), (s32)&work->pos8);
            if (emitter != NULL) {
                emitter->tail->shape = -1;
                *(s32 *)&emitter->tail->s.module.spawnFn = (s32)fx_0d3_op81SpawnPoint;
                emitter->otZ = 0x20;
            }
        }
        rot.vx = 0;
        rot.vy = frame_119 * 8;
        rot.vz = 0;
        pulse = t >> 2;
        scale.vx = scale.vz = pulse + 0x800;
        pos.vx = fx_0d3_centerPos.vx;
        pos.vy = fx_0d3_centerPos.vy;
        pos.vz = fx_0d3_centerPos.vz;
        scale.vy = (((t >> 3) + 0x800) * 2) / 3;
        ex->Hi_DrawEffModel(&rot, &pos, (s32)&scale, work->model54);
        ex->Hi_ModifyEffModelRGB(work->model54, (ex->math_cos(pulse) >> 5) - 0x80, (ex->math_cos(pulse) >> 5) - 0x80,
                                 (ex->math_cos(pulse) >> 5) - 0x80);
        env->otZ = 0x302;
        rot.vx = 0x400;
        rot.vy = 0;
        rot.vz = 0;
        rot.pad = 1;
        fxm_drawTexelQuads(&fx_0d3_centerPos, &rot, 0x11C, 0, 0, 4, frame_119 & 0x1F, 0x190, 0xC, 0xC, pulse + 0x1000, ex->math_cos(pulse), 1,
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
            fadeIn = (frame_119 - 0x30) * 0xFF / 12;
            ex->btlfx_drawScreenTint((u32 *)(env->ot + 1), 1, fadeIn, fadeIn, fadeIn);
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
            fadeOut = 0xFF - frame_119 * 0xFF / 24;
            ex->btlfx_drawScreenTint((u32 *)(env->ot + 1), 1, fadeOut, fadeOut, fadeOut);
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
            emitter = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, FX_PHOENIX_EMBERS_3), (s32)&work->pos30);
            if (emitter != NULL) {
                emitter->otZ = 0x10;
            }
        }
        if (frame_119 == 0x20) {
            work->pos38.vx = fx_0d3_summonPos.vx;
            work->pos38.vy = fx_0d3_summonPos.vy;
            work->pos38.vz = fx_0d3_summonPos.vz;
            work->pos30.vz -= 0x100;
            emitter = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, FX_PHOENIX_FLAME_FEATHERS_3), (s32)&work->pos38);
            if (emitter != NULL) {
                emitter->otZ = 0x10;
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

s32 fx_0d3_op85(s32 mode, void *work, Unk800F78A4 *env, Unk80100C78Entry *entry, s32 *args) {
    if (mode == 0) {
        *args = 0;
    } else if (mode == 1) {
        fx_0d3_env = env;
        env->camera->paused = 0;
    } else {
        return 1;
    }
    return 0;
}

s32 fx_0d3_op87(s32 mode, SVECTOR *work, Unk800F78A4 *env, Unk80100C78Entry *entry, s32 *args) {
    BtlfxExports *ex = BTLFX_EXPORTS(env);
    s32 frame;

    if (mode == 0) {
        *args = 0x10;
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
