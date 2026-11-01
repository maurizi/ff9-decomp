#include "fxmod/fx_0e1_0.h"

static void fxm_drawTexelQuads(SVECTOR *pos, SVECTOR *rot, s32 tp, s32 x, s32 y, s32 div, s32 idx, s32 size, s32 w, s32 h,
                   s32 scale, s32 bright, s32 abr, u16 *colors, s32 skip) {
    Unk800F78A4 *env = fx_0e1_0_env;
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
    left = (tp & 0xF) * 64 + x + (idx % div) * w;
    py = (tp & 0x10) * 16 + y + (idx / div) * h;
    rect.x = left;
    rect.y = py;
    rect.w = w;
    rect.h = h;
    ex->StoreImage(&rect, (u32 *)colors);
}

static void fxm_drawTexRibbonTrail(SVECTOR *posA, SVECTOR *posB, s32 n, s32 tpage, s32 clut, s32 u, s32 v, s32 w, s32 h, s32 bright,
                   Fx0e10Seg *buf, s32 abr, s32 phase) {
    Unk800F78A4 *env = fx_0e1_0_env;
    BtlfxExports *ex = BTLFX_EXPORTS(env);
    CVECTOR col;
    s32 otz;
    POLY_GT4 *prim;
    Fx0e10Seg *seg;
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
            buf[i].a0.pad = 0;
        }
        return;
    }
    seg = &buf[(u32)(n - 1)]; /* (n - 1) * 16, not n * 16 - 16 */
    for (i = 0; i < n; i++) {
        BTLFX_COPY_SVEC(&seg[1].a0, &seg[0].a0);
        BTLFX_COPY_SVEC(&seg[1].b8, &seg[0].b8);
        seg--;
    }
    seg = buf;
    for (i = 0; i < n; i++, seg++) {
        if (!(seg[0].a0.pad & seg[1].a0.pad)) {
            break;
        }
    }
    BTLFX_COPY_SVEC(&buf->a0, posA);
    buf->a0.pad = 1;
    BTLFX_COPY_SVEC(&buf->b8, posB);
    n = i;
    prim = (POLY_GT4 *)env->prim;
    env->prim = (u8 *)(prim + n);
    col.cd = abr == 0xFF ? 0x3C : 0x3E;
    w--;
    h--;
    tpage |= (abr & 3) << 5;
    gte_SetRotMatrix(env->viewMatrix);
    gte_SetTransMatrix(env->viewMatrix);
    /* the last uv store (u1) is the address giv loop.c bases the others on, as in the original's s0 = &prim->u1 */
    /* i = next in the increment: i + 1 lives across the call in its own register and is copied back */
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
            gte_ldv0(&buf[1].b8);
            gte_rtps();
            ex->btlfx_addPrim(0x0C000000, otz, prim);
            gte_stsxy(&prim->x3);
            prim->clut = clut;
            prim->tpage = tpage;
            prim->u0 = prim->u2 = u;
            tileV = phase & 0x1F;
            prim->v0 = prim->v1 = v + ((tileV * h) >> 5);
            prim->v2 = prim->v3 = v + (((tileV + 1) * h) >> 5);
            prim->u1 = prim->u3 = u + w;
        }
    }
}

void fx_0e1_0_drawWaveQuadGrid(s32 ot, s32 x, s32 y, s32 nx, s32 ny, s32 dx, s32 dy, s32 abr, s32 sy, s32 tpage, s32 clut, s32 u,
                   s32 v, s32 w, s32 h, s32 c, s32 amp, s32 ph, s32 dphx, s32 dphy) {
    Unk800F78A4 *env = fx_0e1_0_env;
    BtlfxExports *ex;
    POLY_FT4 *ft4;
    POLY_GT4 *gt4;
    s32 i;
    s32 j;
    s32 px;
    s32 yo;
    s32 u0;
    s32 v0;
    CVECTOR col;
    s32 u1;
    s32 v1;
    s32 k;
    s32 p;

    yo = 0;
    j = 0;
    tpage |= (abr & 3) << 5;
    ft4 = (POLY_FT4 *)env->prim;
    gt4 = (POLY_GT4 *)(ft4 + nx * (ny - 1));
    ex = BTLFX_EXPORTS(env);
    env->prim = (u8 *)(gt4 + nx);
    col.r = col.g = col.b = c;
    for (; j < ny; j++, yo -= dy) {
        px = x;
        p = ph + j * dphy;
        for (i = 0; i < nx; i++, p += dphx, px += dx) {
            k = i & 3;
            u0 = u + ((k * w) >> 2);
            u1 = u0 + (w >> 2);
            if (k == 3) {
                u1--;
            }
            k = j & 3;
            v0 = v + ((k * h) >> 2);
            v1 = v0 + (h >> 2);
            if (k == 3) {
                v1--;
            }
            if (j < ny - 1) {
                ft4->x0 = px + ((ex->math_sin(p) * amp) >> 12);
                ft4->x2 = px + ((ex->math_sin(p + dphy) * amp) >> 12);
                ft4->x1 = (px + dx) + ((ex->math_sin(p + dphx) * amp) >> 12);
                ft4->x3 = (px + dx) + ((ex->math_sin(p + dphy + dphx) * amp) >> 12);
                if (j == 0) {
                    ft4->y1 = y;
                    ft4->y0 = y;
                } else {
                    ft4->y0 = y + (((yo + ((ex->math_sin(p) * amp) >> 12)) * sy) >> 12);
                    ft4->y1 = y + (((yo + ((ex->math_sin(p + dphx) * amp) >> 12)) * sy) >> 12);
                }
                p += dphy;
                ft4->y2 = y + ((((yo - dy) + ((ex->math_sin(p) * amp) >> 12)) * sy) >> 12);
                ft4->y3 = y + ((((yo - dy) + ((ex->math_sin(p + dphx) * amp) >> 12)) * sy) >> 12);
                p -= dphy;
                ft4->tpage = tpage;
                ft4->clut = clut;
                ft4->u0 = ft4->u2 = u0;
                ft4->u1 = ft4->u3 = u1;
                ft4->v0 = ft4->v1 = v0;
                ft4->v2 = ft4->v3 = v1;
                col.cd = abr == 0xFF ? 0x2C : 0x2E;
                *(s32 *)&ft4->r0 = *(s32 *)&col;
                ex->btlfx_addPrim(0x09000000, ot, ft4);
                ft4++;
            } else {
                gt4->x0 = px + ((ex->math_sin(p) * amp) >> 12);
                gt4->x2 = px + ((ex->math_sin(p + dphy) * amp) >> 12);
                gt4->x1 = (px + dx) + ((ex->math_sin(p + dphx) * amp) >> 12);
                gt4->x3 = (px + dx) + ((ex->math_sin(p + dphy + dphx) * amp) >> 12);
                if (j == 0) {
                    gt4->y1 = y;
                    gt4->y0 = y;
                } else {
                    gt4->y0 = y + (((yo + ((ex->math_sin(p) * amp) >> 12)) * sy) >> 12);
                    gt4->y1 = y + (((yo + ((ex->math_sin(p + dphx) * amp) >> 12)) * sy) >> 12);
                }
                p += dphy;
                gt4->y2 = y + ((((yo - dy) + ((ex->math_sin(p) * amp) >> 12)) * sy) >> 12);
                gt4->y3 = y + ((((yo - dy) + ((ex->math_sin(p + dphx) * amp) >> 12)) * sy) >> 12);
                p -= dphy;
                gt4->tpage = tpage;
                gt4->clut = clut;
                gt4->u0 = gt4->u2 = u0;
                gt4->u1 = gt4->u3 = u1;
                gt4->v0 = gt4->v1 = v0;
                gt4->v2 = gt4->v3 = v1;
                col.cd = abr == 0xFF ? 0x3C : 0x3E;
                *(s32 *)&gt4->r0 = *(s32 *)&gt4->r1 = *(s32 *)&col;
                *(s32 *)&gt4->r2 = *(s32 *)&gt4->r3 = 0;
                ex->btlfx_addPrim(0x0C000000, ot, gt4);
                gt4++;
            }
        }
    }
}

static s32 fx_0e1_0_op80SpawnPoint(SVECTOR *out) {
    Unk800F78A4 *env = fx_0e1_0_env;
    BtlfxExports *ex = BTLFX_EXPORTS(env);
    s32 n;

    if (fx_0e1_0_boneSpawnOn != 0) {
        out->pad = 1;
        n = fx_0e1_0_spawnIndex;
        ex->Hi_GetSummonBonePos(((Fx0e10EnvB *)env->loader)->summonModel60, fx_0e1_0_spawnBones[n++], (u16 *)out);
        if (n >= 5) {
            n = 0;
        }
        fx_0e1_0_spawnIndex = n;
    } else {
        out->vx = 10000;
        out->vy = 10000;
        out->vz = 10000;
        out->pad = 1;
    }
    return 0;
}

void fx_0e1_0_scaleXY(s32 unused0, s32 unused1, s32 unused2, s32 unused3, s16 *v) {
    v[0] = (v[0] * 5) >> 2;
    v[1] = (v[1] * 5) >> 2;
}

INCLUDE_RODATA("asm/us/fx_0e1_0/nonmatchings/fx_0e1_0", fx_0e1_0_headerPtr);

s32 fx_0e1_0_op80(s32 mode, Fx0e10Work80 *work, Unk800F78A4 *env, Unk80100C78Entry *entry, s32 *args) {
    BtlfxExports *ex = BTLFX_EXPORTS(env);
    CVECTOR col;
    SVECTOR rot;
    VECTOR sc;
    VECTOR pos;
    VECTOR sc2;
    SVECTOR b;
    CVECTOR col2;
    SVECTOR rot2;
    VECTOR sc3;
    Pool6CEntry *emitter;
    s32 frame;
    s32 k;
    s32 r;
    s32 i;
    s32 glow;
    s32 rib;
    s32 wave;
    s32 c;
    s32 wave2;
    s32 t;

    if (mode == 0) {
        *args = 0xA4;
    } else if (mode == 1) {
        work->state0 = 0;
        work->frame4 = 0;
        fx_0e1_0_env = env;
        ex->btlfx_initEffModels(fx_0e1_0_effModels, 5);
        work->model50 = ex->btlfx_registerEffectModel((Unk800E8F50 *)ex->btlfx_getFileData(entry->file, 0x82), 0);
        work->model54 = ex->btlfx_registerEffectModel((Unk800E8F50 *)ex->btlfx_getFileData(entry->file, 0x81), 0);
        work->model58 = ex->btlfx_registerEffectModel((Unk800E8F50 *)ex->btlfx_getFileData(entry->file, 0x80), 0);
        work->model60 = ex->btlfx_registerEffectModel((Unk800E8F50 *)ex->btlfx_getFileData(entry->file, 3), 0);
        work->model5C = ex->btlfx_registerEffectModel((Unk800E8F50 *)ex->btlfx_getFileData(entry->file, 0x83), 1);
        work->pos8.vx = fx_0e1_0_origin.vx;
        work->pos8.vy = fx_0e1_0_origin.vy;
        work->pos8.vz = fx_0e1_0_origin.vz;
        work->morphPos6C.vx = fx_0e1_0_origin.vx;
        work->morphPos6C.vy = fx_0e1_0_origin.vy;
        work->morphPos6C.vz = fx_0e1_0_origin.vz;
        work->rot28.vy = -0x400;
        work->summonPos10.vx = work->pos8.vx;
        work->summonScale1C.vx = 0xC00;
        work->summonScale1C.vy = 0xC00;
        work->summonScale1C.vz = 0xC00;
        work->summonPos10.vy = work->pos8.vy;
        work->summonPos10.vz = work->pos8.vz;
        work->summonPos10.vx -= 0x4BB;
        work->rot64.vx = 0;
        work->rot64.vy = 0;
        work->rot64.vz = 0;
        work->rot28.vx = 0;
        work->rot28.vz = 0;
        work->summonPos10.vy += 0x376;
        work->summonPos10.vz += 0x8C9;
        work->camPos3C.vx = work->pos8.vx;
        work->camPos3C.vy = work->pos8.vy;
        work->camPos3C.vz = work->pos8.vz;
        env->camera->points[3].vx = work->camPos3C.vx;
        env->camera->points[3].vy = work->camPos3C.vy;
        env->camera->points[3].vz = work->camPos3C.vz;
        work->pos84.vx = fx_0e1_0_origin.vx;
        work->pos84.vy = fx_0e1_0_origin.vy;
        work->pos84.vz = fx_0e1_0_origin.vz;
        work->scale90.vx = 0x7000;
        work->scale90.vy = 0x7000;
        work->scale90.vz = 0x7000;
        work->rot9C.vx = 0;
        work->rot9C.vy = 0;
        work->rot9C.vz = 0;
        work->unk30 = 0;
        work->ribbonReset4C = mode;
    } else {
        glow = 0;
        k = 0;
        rib = 0;
        wave = 0;
        frame = entry->frame;
        wave2 = 0;
        switch (work->state0) {
        case 0:
            k = 0x1000;
            if (frame < 0x10) {
                ex->btlfx_drawScreenTint((u32 *)(env->ot + 1), 1, 0xFF - ((frame * 0xFF) >> 4),
                                         0xFF - ((frame * 0xFF) >> 4), 0xFF - ((frame * 0xFF) >> 4));
            }
            if (frame < 0x2E) {
                work->morphScale78.vx = work->morphScale78.vy = work->morphScale78.vz = k;
                r = ex->math_cos((frame << 10) / 46);
                work->morphPos6C.vx = fx_0e1_0_origin.vx + ((ex->math_cos(work->frame4 << 8) * r) >> 17);
                work->morphPos6C.vy = fx_0e1_0_origin.vx + ((ex->math_sin(work->frame4 << 8) * r) >> 17);
                /* i doubles as the morph blend: r must stay single-set (sched1 then births its copy from v0 after
                   the second math_cos pointer load, as in the original) */
                i = 0x2000;
            } else {
                s32 v = ex->math_cos(((frame - 0x2E) << 10) / 18);

                i = v * 2;
                work->morphScale78.vx = work->morphScale78.vy = work->morphScale78.vz = k - (((k - v) * 0xF00) >> 12);
            }
            if (i > 0x1000) {
                ex->Hi_DrawMorphEffModel(&work->rot64, (VECTOR *)&work->morphPos6C, (s32)&work->morphScale78, work->model50, work->model54,
                                         i - 0x1000);
            } else {
                ex->Hi_DrawMorphEffModel(&work->rot64, (VECTOR *)&work->morphPos6C, (s32)&work->morphScale78, work->model54, work->model58, i);
            }
            col.r = col.g = col.b = 0x84;
            rot.vz = -0x18;
            sc.vy = sc.vx = 0x10F5;
            sc.vz = 0x1000;
            rot.vx = 0;
            rot.vy = 0;
            ex->btlfx_drawScreenGrid(3, 0xA0, 0x6E, 0xA0, 0x6E, 2, 2, 1, 1, &rot, &sc, &col, 0, 0);
            if (frame >= 0x40) {
                work->state0 = 1;
                entry->frame = -1;
                emitter = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, 8), (s32)&work->pos8);
                if (emitter != NULL) {
                    emitter->otZ = 0x20;
                }
                emitter = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, 9), (s32)&work->pos8);
                if (emitter != NULL) {
                    emitter->otZ = 0x20;
                }
                emitter = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, 0xD), (s32)&work->pos8);
                if (emitter != NULL) {
                    emitter->otZ = 0xC;
                }
                ex->btlfx_drawScreenTint((u32 *)(env->ot + 1), 1, 0xFF, 0x80, 0);
                work->emit44 = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, 1), (s32)&work->pos8);
                if (work->emit44 != NULL) {
                    work->emit44->otZ = 0xC;
                }
                work->emit48 = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, 7), (s32)&work->pos8);
                if (work->emit48 != NULL) {
                    work->emit48->trails = (s32)fx_0e1_0_op80Trails0;
                    work->emit48->trailSlots = 0xC;
                    work->emit48->trailStride = 0x8C;
                    if (work->emit48 != NULL) {
                        work->emit48->otZ = 0xC;
                    }
                }
            }
            break;
        case 1:
            t = frame << 7;
            k = 0x1000;
            glow = 0x1000;
            ex->Hi_DrawEffModel(&work->rot64, (VECTOR *)&work->morphPos6C, (s32)&work->morphScale78, work->model58);
            ex->Hi_ModifyEffModelAbr(work->model58, 1);
            ex->Hi_ModifyEffModelRGB(work->model58, ((0x1000 - t) >> 5) - 0x80, ((0x1000 - t) >> 5) - 0x80,
                                     ((0x1000 - t) >> 5) - 0x80);
            if (frame >= 0x20) {
                work->state0 = 2;
                entry->frame = -1;
                ex->Hi_SetSummonMotion(((Fx0e10Summon *)env->loader)->summon54->motion180,
                                       ((Fx0e10Summon *)env->loader)->summonModel60);
                ex->Hi_SetSummonMotFrame(((Fx0e10Summon *)env->loader)->summonModel60, 0);
                emitter = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, 0xC), (s32)&work->pos8);
                if (emitter != NULL) {
                    emitter->tail->shape = -1;
                    *(s32 *)&emitter->tail->s.module.spawnFn = (s32)fx_0e1_0_op80SpawnPoint;
                    emitter->otZ = 0x20;
                }
            }
            break;
        case 2:
            if (frame < 0x10) {
                work->summonPos10.vy -= 0x80;
            }
            k = 0x1000;
            if (frame == 0x50) {
                ex->Hi_SetSummonMotFrame(((Fx0e10Summon *)env->loader)->summonModel60, 0x5A);
                fx_0e1_0_boneSpawnOn = 1;
            }
            if ((u32)(frame - 0x36) >= 0x1A) {
                ex->Hi_DrawSummonModel(&work->rot28, (VECTOR *)&work->summonPos10, (s32)&work->summonScale1C,
                                       ((Fx0e10Summon *)env->loader)->summonModel60, 1);
                if (frame < 0x20) {
                    ex->Hi_ModifySummonModelRGB(((Fx0e10Summon *)env->loader)->summonModel60,
                                                (ex->math_sin((frame << 10) >> 5) >> 5) - 0x80,
                                                ((frame << 7) >> 5) - 0x80, ((frame << 7) >> 5) - 0x80);
                    ex->Hi_ModifySummonModelAbr(((Fx0e10Summon *)env->loader)->summonModel60, 1);
                }
                rib = 0x1000;
            } else {
                fx_0e1_0_boneSpawnOn = 0;
            }
            if (fx_0e1_0_boneSpawnOn != 0) {
                s32 d = ((ex->math_sin((frame << 11) / 159) * 3) >> 2) + 0x400;

                env->otZ = 0xF5;
                ex->Hi_GetSummonBonePos(((Fx0e10Summon *)env->loader)->summonModel60, 0, (u16 *)&work->pos34);
                ex->btlfx_drawRings((u8 *)ex->btlfx_getFileData(entry->file, 5), &work->pos34, NULL, 0x1000, 0, 1,
                                  d);
                env->otZ = 0;
            }
            if (frame == 0) {
                emitter = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, 0xD), (s32)&work->pos8);
                if (emitter != NULL) {
                    emitter->otZ = 0xC;
                }
                work->emit48 = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, 7), (s32)&work->pos8);
                if (work->emit48 != NULL) {
                    work->emit48->trails = (s32)fx_0e1_0_op80Trails1;
                    work->emit48->trailSlots = 0xC;
                    work->emit48->trailStride = 0x8C;
                    if (work->emit48 != NULL) {
                        work->emit48->otZ = 0xC;
                    }
                }
            }
            if ((u32)(frame - 0x7F) <= 0x10) {
                s32 d = (frame - 0x7F) << 12;
                s32 e = d >> 4;

                if (work->emit44 != NULL) {
                    work->emit44->alpha = 0x1000 - e;
                    if (work->emit44 != NULL) {
                        work->emit44->scale = 0x1000 - (d >> 5);
                    }
                }
                if (work->emit48 != NULL) {
                    work->emit48->alpha = 0x1000 - e;
                }
                if (frame - 0x7F == 0x10) {
                    ex->btlfx_stopEmitter(work->emit44);
                    ex->btlfx_stopEmitter(work->emit48);
                }
            }
            if (frame < 0x87) {
                glow = 0x1000;
            } else {
                s32 d = frame - 0x87;

                d = (d << 12) / 24;

                k = 0x1000 - d;
                wave2 = d;
                wave = wave2;
                glow = k;
            }
            if (frame >= 0x9F) {
                work->state0 = 3;
                entry->frame = -1;
                emitter = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, 0xA), (s32)&work->pos8);
                if (emitter != NULL) {
                    emitter->otZ = 0x40;
                }
                emitter = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, 6), (s32)&work->pos8);
                if (emitter != NULL) {
                    emitter->otZ = 0x12C;
                }
            }
            break;
        case 3:
            wave = 0x1000;
            wave2 = 0x1000;
            ex->Hi_SetSummonMotFrame(((Fx0e10Summon *)env->loader)->summonModel60, 0xA9);
            ex->Hi_DrawSummonModel(&work->rot28, (VECTOR *)&work->summonPos10, (s32)&work->summonScale1C, ((Fx0e10Summon *)env->loader)->summonModel60,
                                   1);
            if (frame == 0xF) {
                emitter = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, 4), (s32)&work->pos8);
                if (emitter != NULL) {
                    emitter->otZ = 0x80;
                }
            }
            if (frame >= 0x33) {
                work->state0 = 4;
                entry->frame = -1;
            }
            break;
        case 4:
            t = (frame << 12) / 49;
            if (frame < 0x20) {
                wave = ex->math_cos(frame << 5);
            }
            wave2 = t + 0x1000;
            if (frame < 0x2C) {
                ex->Hi_SetSummonMotFrame(((Fx0e10Summon *)env->loader)->summonModel60, 0xA9);
                ex->Hi_DrawSummonModel(&work->rot28, (VECTOR *)&work->summonPos10, (s32)&work->summonScale1C,
                                       ((Fx0e10Summon *)env->loader)->summonModel60, 1);
            }
            if (frame >= 0x31) {
                ex->btlfx_clearEffModels();
                return 1;
            }
            break;
        }
        if (wave != 0) {
            fx_0e1_0_drawWaveQuadGrid(0xFFE, -0x31, 0xDC, 0xC, 5, 0x28, 0x32, 1, 0x1000, 0x99, 0x3E00, 0x80, 0, 0x40, 0x80,
                          wave >> 6, 0x20, (work->frame4 << 5) + 0x400, 0x140, 0x200);
            ex->btlfx_drawScrollMove(10000 - work->frame4 * 8, 0x280, 0x100, 0x20, 0x80, 0x2A0, 0x100);
        }
        if (wave2 != 0) {
            fx_0e1_0_drawWaveQuadGrid(2, -0x20, 0xDC, 6, 5, 0x42, 0x32, 1, 0x1000, 0x1B, 0x3D05, 0xC0, 0, 0x40, 0x80,
                          wave2 >> 7, 0x16, work->frame4 << 6, 0x200, 0x302);
            ex->btlfx_drawScrollMove(10000 - work->frame4 * 4, 0x2F0, 0x100, 0x10, 0x80, 0x2E0, 0x100);
        }
        if (k != 0) {
            ex->btlfx_sampleGradient(fx_0e1_0_gradient, &col, work->frame4 & 0x1F);
            col.r = ((k * col.r) >> 11) / 3;
            col.g = ((k * col.g) >> 11) / 3;
            col.b = ((k * col.b) >> 11) / 3;
            ex->Hi_DrawEffModel(&work->rot9C, (VECTOR *)&work->pos84, (s32)&work->scale90, work->model60);
            ex->Hi_ModifyEffModelRGB(work->model60, col.r - 0x80, col.g - 0x80, col.b - 0x80);
            work->rot9C.vy += 0x10;
        }
        if (glow != 0) {
            pos.vx = work->pos8.vx;
            pos.vy = work->pos8.vy;
            pos.vz = work->pos8.vz;
            sc2.vx = sc2.vy = 0x1000;
            sc2.vz = 0x555;
            rot.vx = work->frame4 * 0xC;
            rot.vy = work->frame4 * 4;
            rot.vz = frame * 8;
            ex->Hi_DrawEffModel(&rot, &pos, (s32)&sc2, work->model5C);
            ex->Hi_ModifyEffModelRGB(work->model5C, ((glow * 3) >> 7) - 0x80, ((glow * 3) >> 7) - 0x80,
                                     ((glow * 3) >> 7) - 0x80);
            sc2.vx = sc2.vy = 0xAAA;
            sc2.vz = 0x400;
            rot.vx = -work->frame4 * 4;
            rot.vy = work->frame4 * 0x10;
            rot.vz = frame * 0xC;
            ex->Hi_DrawEffModel(&rot, &pos, (s32)&sc2, work->model5C);
            ex->Hi_ModifyEffModelRGB(work->model5C, ((glow * 7) >> 8) - 0x80, ((glow * 7) >> 8) - 0x80,
                                     ((glow * 7) >> 8) - 0x80);
            sc2.vx = sc2.vy = 0x800;
            sc2.vz = 0x333;
            rot.vx = work->frame4 * 4;
            rot.vy = work->frame4 * 0xC;
            rot.vz = frame * 0x10;
            ex->Hi_DrawEffModel(&rot, &pos, (s32)&sc2, work->model5C);
            ex->Hi_ModifyEffModelRGB(work->model5C, (glow >> 5) - 0x80, (glow >> 5) - 0x80, (glow >> 5) - 0x80);
            ex->btlfx_drawScrollMove(work->frame4 * 4, 0x2A0, 0x180, 0x10, 0x40, 0x2B0, 0x180);
        }
        if (rib != 0) {
            env->otZ = -0x80;
            if (work->ribbonReset4C != 0) {
                work->ribbonReset4C = 0;
                for (i = 0; i < 4; i++) {
                    ex->Hi_GetSummonBonePos(((Fx0e10Summon *)env->loader)->summonModel60, fx_0e1_0_ribbonBones[i].boneA0, (u16 *)&sc);
                    ex->Hi_GetSummonBonePos(((Fx0e10Summon *)env->loader)->summonModel60, fx_0e1_0_ribbonBones[i].boneB4, (u16 *)&b);
                    fxm_drawTexRibbonTrail((SVECTOR *)&sc, &b, 0x3FC, 0, 0, 0, 0, 0, 0, 0, fx_0e1_0_ribbons[i], 1, 0);
                }
            }
            for (i = 0; i < 4; i++) {
                ex->Hi_GetSummonBonePos(((Fx0e10Summon *)env->loader)->summonModel60, fx_0e1_0_ribbonBones[i].boneA0, (u16 *)&sc);
                ex->Hi_GetSummonBonePos(((Fx0e10Summon *)env->loader)->summonModel60, fx_0e1_0_ribbonBones[i].boneB4, (u16 *)&b);
                fxm_drawTexRibbonTrail((SVECTOR *)&sc, &b, 0x14, 0x1B, 0x3D05, 0xC0, 0, 0x40, 0x80, ((rib >> 5) * 2) / 3, fx_0e1_0_ribbons[i], 1,
                              10000 - work->frame4);
            }
            env->otZ = 0;
            ex->btlfx_drawScrollMove(10000 - work->frame4 * 4, 0x2F0, 0x100, 0x10, 0x80, 0x2E0, 0x100);
        }
        col2.r = col2.g = col2.b = 0x89;
        sc3.vy = sc3.vx = 0x1073;
        sc3.vz = 0x1000;
        rot2.vx = 0;
        rot2.vy = 0;
        rot2.vz = 0;
        ex->btlfx_drawScreenGrid(3, 0xA0, 0x6E, 0xA0, 0x6E, 2, 2, 1, 1, &rot2, &sc3, &col2, 0, 0);
        work->frame4++;
    }
    return 0;
}
