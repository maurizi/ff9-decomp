#include "fxmod/fx_0fb_0.h"

static void fxm_drawRibbonTrail(SVECTOR *posA, SVECTOR *posB, u32 n, CVECTOR *colorA, CVECTOR *colorB, s32 fade, Fx0fb0Seg *buf, s32 abr) {
    Unk800F78A4 *env = fx_0fb_0_env;
    CVECTOR colA;
    CVECTOR colB;
    s32 otz;
    BtlfxExports *ex = BTLFX_EXPORTS(env);
    POLY_G4 *prim;
    Fx0fb0Seg *seg;
    u32 i;
    s32 fadeNear;
    s32 fadeFar;

    if (fade < 0) {
        fade = 0x1000;
    }
    if (n >= 1000) {
        n -= 999;
        for (i = 0; i < n; i++) {
            buf[i].a0.pad = 0;
        }
        return;
    }
    seg = &buf[(s32)(n - 1)];
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
    prim = (POLY_G4 *)env->prim;
    env->prim = (s32)(prim + n);
    if (colorA == NULL) {
        colA.r = colA.g = colA.b = 0;
    } else {
        gte_DpqColorFar_nst(colorA, fade);
        gte_strgb(&colA);
    }
    if (colorB == NULL) {
        colB.r = colB.g = colB.b = 0;
    } else {
        gte_DpqColorFar_nst(colorB, fade);
        gte_strgb(&colB);
    }
    gte_SetRotMatrix(env->viewMatrix);
    gte_SetTransMatrix(env->viewMatrix);
    colA.cd = abr != 0xFF ? 0x3A : 0x38;
    colB.cd = abr != 0xFF ? 0x3A : 0x38;
    for (i = 0; i < n; i++, buf++, prim++) {
        gte_ldv3c(buf);
        fadeNear = 0x1000 - (i << 12) / n;
        gte_rtpt_b();
        fadeFar = 0x1000 - ((i + 1) << 12) / n;
        gte_stopz(&otz);
        if (otz != 0) {
            gte_stsxy3(&prim->x0, &prim->x1, &prim->x2);
            gte_avsz3();
            gte_stszotz(&otz);
            gte_ldv0(&buf[1].b8);
            gte_rtps();
            ex->btlfx_addPrimAbr(0x8000000, otz, prim, abr);
            gte_stsxy(&prim->x3);
            gte_DpqColorFar_nst(&colA, fadeNear);
            gte_strgb(&prim->r0);
            gte_DpqColorFar_nst(&colB, fadeNear);
            gte_strgb(&prim->r1);
            gte_DpqColorFar_nst(&colA, fadeFar);
            gte_strgb(&prim->r2);
            gte_DpqColorFar_nst(&colB, fadeFar);
            gte_strgb(&prim->r3);
        }
    }
}

void fx_0fb_0_drawWaveSheet(SVECTOR *pos, SVECTOR *rot, VECTOR *scale, s32 col, s32 tpage, s32 clut, s32 u, s32 v, s32 amp,
                   s32 ph, s32 step, s32 abr) {
    Unk800F78A4 *env = fx_0fb_0_env;
    BtlfxExports *ex = BTLFX_EXPORTS(env);
    CVECTOR midCol;
    CVECTOR edgeCol;
    CVECTOR rimCol;
    MATRIX m;
    SVECTOR vs[4];
    s32 otz;
    s32 xo;
    s32 rowY;
    s32 i;
    s32 j;
    s32 x;
    s32 topY;
    s32 botY;
    s32 texRow;
    s32 xb;
    s32 yb;
    s32 sinNext;
    s32 u0;
    s32 u1;
    s32 v0;
    s32 v1;
    s32 ft;
    POLY_GT4 *edgePoly;
    POLY_FT4 *poly;

    gte_SetRotMatrix(env->viewMatrix);
    gte_SetTransMatrix(env->viewMatrix);
    gte_ldv0(pos);
    edgePoly = (POLY_GT4 *)env->prim;
    *(s32 *)&rimCol = 0;
    poly = (POLY_FT4 *)(edgePoly + 64);
    env->prim = (u8 *)(poly + 448);
    midCol.r = midCol.g = midCol.b = col;
    edgeCol.r = edgeCol.g = edgeCol.b = col;
    if (abr != 0xFF) {
        midCol.cd = 0x2E;
    } else {
        midCol.cd = 0x2C;
    }
    rimCol.cd = abr != 0xFF ? 0x3E : 0x3C;
    tpage |= (abr & 3) << 5;
    edgeCol.cd = rimCol.cd;
    gte_rtv0tr_b();
    ex->math_rotMatrixYXZ(rot, &m);
    ex->ScaleMatrix(&m, scale);
    gte_stlvnl(m.t);
    ex->MulRotMatrix(&m);
    gte_SetRotMatrix(&m);
    gte_SetTransMatrix(&m);
    xb = -0x2EE0;
    yb = -0x2000;
    vs[0].vz = vs[1].vz = vs[2].vz = vs[3].vz = 0;
    for (rowY = 0, i = 0; i < 32; i++, rowY += 0x200) {
        ph += step;
        topY = yb + rowY;
        botY = topY + 0x200;
        for (xo = 0, j = 0, texRow = i & 3; j < 16; j++, xo += 0x5DC) {
            x = xb + xo;
            vs[0].vx = x + ((ex->math_sin(ph) * amp) >> 12);
            vs[1].vx = vs[0].vx + 0x5DC;
            sinNext = ex->math_sin(ph + step);
            vs[0].vy = vs[1].vy = topY;
            vs[2].vy = vs[3].vy = botY;
            x += (sinNext * amp) >> 12;
            vs[2].vx = x;
            vs[3].vx = x + 0x5DC;
            gte_ldv3c(vs);
            gte_rtpt();
            u0 = u + (((j & 1) * 0x70) >> 1);
            u1 = u0 + 0x38;
            if (j & 1) {
                u1 = u0 + 0x37;
            }
            v0 = v + ((texRow * 0x70) >> 2);
            v1 = v0 + 0x1C;
            if (texRow == 3) {
                v1 = v0 + 0x1B;
            }
            if (j == 0) {
                *(s32 *)&edgePoly->r0 = *(s32 *)&edgePoly->r2 = *(s32 *)&rimCol;
                *(s32 *)&edgePoly->r1 = *(s32 *)&edgePoly->r3 = *(s32 *)&edgeCol;
                ft = 0;
            } else if (j >= 15) {
                *(s32 *)&edgePoly->r1 = *(s32 *)&edgePoly->r3 = *(s32 *)&rimCol;
                *(s32 *)&edgePoly->r0 = *(s32 *)&edgePoly->r2 = *(s32 *)&edgeCol;
                ft = 0;
            } else {
                ft = 1;
                *(s32 *)&poly->r0 = *(s32 *)&midCol;
            }
            gte_stopz(&otz);
            if (otz != 0) {
                if (ft) {
                    poly->u0 = poly->u2 = u0;
                    poly->u1 = poly->u3 = u1;
                    poly->v0 = poly->v1 = v0;
                    poly->v2 = poly->v3 = v1;
                    poly->tpage = tpage;
                    poly->clut = clut;
                    gte_stsxy3(&poly->x0, &poly->x1, &poly->x2);
                } else {
                    edgePoly->u0 = edgePoly->u2 = u0;
                    edgePoly->u1 = edgePoly->u3 = u1;
                    edgePoly->v0 = edgePoly->v1 = v0;
                    edgePoly->v2 = edgePoly->v3 = v1;
                    edgePoly->tpage = tpage;
                    edgePoly->clut = clut;
                    gte_stsxy3(&edgePoly->x0, &edgePoly->x1, &edgePoly->x2);
                }
                gte_avsz3();
                gte_stszotz(&otz);
                gte_ldv0(&vs[3]);
                gte_rtps();
                if (ft) {
                    ex->btlfx_addPrim(0x09000000, otz, poly);
                    gte_stsxy(&poly->x3);
                    poly++;
                } else {
                    ex->btlfx_addPrim(0x0C000000, otz, edgePoly);
                    gte_stsxy(&edgePoly->x3);
                    edgePoly++;
                }
            }
        }
    }
}

void fx_0fb_0_drawWaveCurtain(SVECTOR *pos, s32 dy, s32 c, s32 tpage, s32 clut, s32 u, s32 v, s32 amp, s32 ph, s32 step,
                   s32 abr, s32 ph2) {
    Unk800F78A4 *env = fx_0fb_0_env;
    BtlfxExports *ex = BTLFX_EXPORTS(env);
    SVECTOR prev;
    SVECTOR next;
    SVECTOR vs[4];
    s32 otz;
    s32 x;
    s32 t;
    s32 code;
    POLY_GT4 *poly;
    s32 i;
    s32 topBright;
    s32 botBright;
    s32 u0;
    s32 u1;
    s32 v1;
    s32 nph;

    botBright = 0;
    gte_SetRotMatrix(env->viewMatrix);
    gte_SetTransMatrix(env->viewMatrix);
    poly = (POLY_GT4 *)env->prim;
    code = 0x3C;
    env->prim = (u8 *)(poly + 44);
    if (abr != 0xFF) {
        code = 0x3E;
    }
    tpage |= (abr & 3) << 5;
    x = -0x6098;
    topBright = c >> 3;
    for (i = 0; i < 44; i++) {
        t = ((i + 1) << 12) / 44;
        if (i == 0) {
            BTLFX_COPY_SVEC(&prev, pos);
            prev.vx += x;
            prev.vz += ex->math_sin(ph2) * 2;
            prev.vz += (ex->math_sin(ph) * amp) >> 12;
            prev.vx -= (ex->math_sin(ph) * amp) >> 13;
            botBright = c;
        }
        BTLFX_COPY_SVEC(&next, pos);
        next.vx += 0x464 + x;
        next.vz += ex->math_sin(t + ph2) * 2;
        nph = ph + step;
        next.vz += (ex->math_sin(nph) * amp) >> 12;
        next.vx -= (ex->math_sin(nph) * amp) >> 13;
        BTLFX_COPY_SVEC(&vs[0], &prev);
        BTLFX_COPY_SVEC(&vs[2], &prev);
        BTLFX_COPY_SVEC(&vs[1], &next);
        BTLFX_COPY_SVEC(&vs[3], &next);
        BTLFX_COPY_SVEC(&prev, &next);
        vs[2].vy += dy;
        vs[3].vy += dy;
        gte_ldv3c(vs);
        gte_rtpt();
        u0 = u + (((i & 7) * 0x70) >> 3);
        u1 = u0 + 0xE;
        if ((i & 7) == 7) {
            u1 = u0 + 0xD;
        }
        v1 = v + 0x6F;
        gte_nclip();
        poly->r1 = poly->g1 = poly->b1 = topBright;
        poly->r0 = poly->g0 = poly->b0 = topBright;
        poly->r2 = poly->g2 = poly->b2 = botBright;
        botBright = ex->math_cos(t >> 2) >> 5;
        poly->r3 = poly->g3 = poly->b3 = botBright;
        poly->code = code;
        gte_stopz(&otz);
        if (otz != 0) {
            poly->u0 = poly->u2 = u0;
            poly->u1 = poly->u3 = u1;
            poly->v0 = poly->v1 = v;
            poly->v2 = poly->v3 = v1;
            poly->tpage = tpage;
            poly->clut = clut;
            gte_stsxy3(&poly->x0, &poly->x1, &poly->x2);
            gte_avsz3();
            gte_stszotz(&otz);
            gte_ldv0(&vs[3]);
            gte_rtps();
            ex->btlfx_addPrim(0x0C000000, otz, poly);
            gte_stsxy(&poly->x3);
            poly++;
        }
        ph = nph;
        x += 0x464;
    }
}

static s32 fx_0fb_0_op80PoolTask(s32 mode, Fx0fb0Rock *r, Unk800F78A4 *env) {
    BtlfxExports *ex = BTLFX_EXPORTS(env);
    VECTOR pos;
    VECTOR sc;

    if (mode == 1) {
        r->state0 = 0;
        r->frame8 = 0;
        r->bob6 = ex->btlfx_randCentered(0x1800) - 0x3000;
        r->rot14.vx = 0;
        r->rot14.vy = 0;
        r->rot14.vz = 0;
        r->spinX1C = ex->btlfx_rand() % 64 + 0x40;
        r->spinY1E = 0x20 - ex->btlfx_rand() % 32;
    } else {
    if (r->kind4 < 0x19) {
        sc.vx = 0x1000;
        sc.vy = 0x1000;
        sc.vz = 0x1000;
    } else {
        sc.vx = 0x199;
        sc.vy = 0x199;
        sc.vz = 0x200;
    }
    pos.vx = r->posC.vx;
    pos.vy = r->posC.vy;
    pos.vz = r->posC.vz;
    r->rot14.vx += r->spinX1C;
    r->rot14.vy += r->spinY1E;
    r->posC.vx += ex->btlfx_randCentered(0x10);
    r->posC.vz += ex->btlfx_randCentered(0x10);
    switch (r->state0) {
    case 0:
        mode = (r->frame8 << 12) / 40; /* reuses the parameter */
        pos.vy += (ex->math_sin(mode >> 2) * r->bob6) >> 12;
        if (++r->frame8 >= 0x28) {
            r->state0 = 1;
            r->frame8 = 0;
            r->posC.vy = pos.vy;
            r->bob6 = ex->btlfx_rand();
        }
        break;
    case 1:
        pos.vy += ex->math_sin((r->frame8 << 7) + r->bob6) >> 3;
        r->frame8++;
        break;
    }
    if (fx_0fb_0_unitsRestored != 0 || (fx_0fb_0_rocksReleased != 0 && r->kind4 >= 0x19)) {
        return 1;
    }
    ex->Hi_DrawEffModel(&r->rot14, &pos, (s32)&sc, r->model20);
    }
    return 0;
}

INCLUDE_RODATA("asm/us/fx_0fb_0/nonmatchings/fx_0fb_0", fx_0fb_0_headerPtr);

/* Case 2's unk64 block: xs (s16) and ys as block locals computed right after the sin call (xs -= cc in place
 * makes post-reload cse turn the env->camera reload into the original's move), then empty do-while barriers around
 * `cc = (level * 3) / 2;` that split the rest into the original's two scheduling groups. Other levers are listed in
 * the history of docs/wip/fx_0fb_0_op80.c. */
s32 fx_0fb_0_op80(s32 mode, Fx0fb0Work80 *work, Unk800F78A4 *env, Unk80100C78Entry *entry, EffInit *init) {
    BtlfxExports *ex = BTLFX_EXPORTS(env);
    Fx0fb0Rock *rock;
    Pool6CEntry *emitter;
    s16 *unused;
    s16 *ptWidth;
    s16 *fadeWidth;
    SVECTOR *rp;
    s32 frame;
    s32 i;
    s32 rockModel;
    s32 bone;
    s32 node;
    s32 strip;
    s32 strand;
    s32 j;
    s32 k;
    s32 sparkScale;
    s32 swingAng;
    s32 shakeAng;
    s32 burstT;
    s32 curtainCol;
    s32 step;
    s32 unused2;
    s32 shakeCos;
    s32 wingAng;
    s32 unused3;
    s32 pullBack;
    s32 lift;
    s32 blueTint;
    s32 flashTint;
    s32 level;
    s32 fadeIn;
    s32 fadeOut;
    s32 domeRgb;
    s32 haloRgb;
    s32 wingGrow;
    s32 swing;
    s32 wingSpread;
    s32 bob;
    s32 unused4;
    s32 unused5;
    s32 shake;
    s32 big;
    s32 idx;
    s32 sheetPhase;
    s32 curtainBright;
    s32 domeBright;
    s32 gridBright;
    s32 spriteAlpha;
    s32 haloBright;
    s32 t;

    if (mode == 0) {
        init->workSize = 0x128;
        init->elemSize = 0x28;
        init->elemFn = (s32)fx_0fb_0_op80PoolTask;
        init->poolCount = 0x32;
    } else if (mode == 1) {
        work->state0 = 0;
        work->frame4 = 0;
        fx_0fb_0_env = env;
        ex->btlfx_initEffModels(SUMMON0FB(env)->effModels38, 0x1F);
        work->basePosC.vy = 0;
        work->basePosC.vx = 0;
        work->basePosC.vz = 0;
        work->rotF0.vy = -0x400;
        work->scaleE4.vx = 0x1800;
        work->scaleE4.vy = 0x1800;
        work->rotF0.vx = 0;
        work->rotF0.vz = 0;
        work->scaleE4.vz = 0x2000;
        work->posD8.vx = work->basePosC.vx;
        work->posD8.vy = work->basePosC.vy;
        work->posD8.vz = work->basePosC.vz;
        work->posD8.vy -= 0x5000;
        env->camera->points[3].vx = work->basePosC.vx;
        env->camera->points[3].vy = work->basePosC.vy;
        env->camera->points[3].vz = work->basePosC.vz;
        FX0FB_W(work->pos5C, 0) = FX0FB_W(work->basePosC, 0);
        FX0FB_W(work->pos5C, 1) = FX0FB_W(work->basePosC, 1);
    } else {
        sheetPhase = 0;
        curtainBright = 0;
        domeBright = 0;
        gridBright = 0;
        spriteAlpha = 0;
        haloBright = 0;
        frame = entry->frame;
        switch (work->state0) {
        case 0:
            sheetPhase = (frame << 12) / 30;
            work->pos5C.vy = work->basePosC.vy - ((sheetPhase << 9) >> 12);
            env->camera->points[3].vx = work->pos5C.vx;
            env->camera->points[3].vy = work->pos5C.vy;
            env->camera->points[3].vz = work->pos5C.vz;
            env->camera->points[2].vx = env->camera->eye.vx;
            env->camera->points[2].vy = env->camera->eye.vy;
            env->camera->points[2].vz = env->camera->eye.vz;
            work->camPos64.vx = env->camera->eye.vx;
            work->camPos64.vy = env->camera->eye.vy;
            work->camPos64.vz = env->camera->eye.vz;
            if (frame >= 0xE) {
                ex->btlfx_drawRippleGrid((s32)(env->ot + 2), 0xF0 - (((frame - 0xE) * 0xE600) >> 12), 8, 2, 0x78, 1,
                                  fx_0fb_0_rippleRgb, 0x20, work->frame4 << 7, 0x100, 0x400);
            }
            if (frame == 0x18) {
                work->pos24.vx = fx_0fb_0_origin.vx;
                work->pos24.vy = fx_0fb_0_origin.vy;
                work->pos24.vz = fx_0fb_0_origin.vz;
                work->pos24.vy += 0x640;
                emitter = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, 5), (s32)&work->pos24);
                if (emitter != NULL) {
                    emitter->otZ = 0x20;
                }
            }
            if (frame >= 0x1E) {
                work->state0 = 1;
                entry->frame = -1;
                for (rockModel = 0; rockModel < 27; rockModel++) {
                    work->model6C[rockModel] = ex->Hi_RegisterTexEffModel((BtlModelHeader *)ex->btlfx_getFileData(entry->file, fx_0fb_0_rockBlocks[rockModel]),
                                                                0x9A, 0x3E00, 0, 0);
                }
                work->rockCount8 = 0;
                work->model118 = ex->Hi_RegisterTexEffModel((BtlModelHeader *)ex->btlfx_getFileData(entry->file, 0x82), 0x96, 0x3D40, 0, 0);
                work->model11C = ex->Hi_RegisterTexEffModel((BtlModelHeader *)ex->btlfx_getFileData(entry->file, 0x87), 0x9A, 0x3E00, 0, 0);
                work->model120 = ex->Hi_RegisterTexEffModel((BtlModelHeader *)ex->btlfx_getFileData(entry->file, 0x84), 0x7C, 0x3D02, 0, 0);
                work->model124 = ex->Hi_RegisterTexEffModel((BtlModelHeader *)ex->btlfx_getFileData(entry->file, 0x94), 0xBD, 0x3E40, 0, 0);
                emitter = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, 0xB), (s32)&work->pos1C);
                if (emitter != NULL) {
                    emitter->otZ = 0x180;
                }
                emitter = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, 0xC), (s32)&work->pos14);
                if (emitter != NULL) {
                    emitter->otZ = 0x200;
                }
                emitter = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, 1), (s32)&work->pos1C);
                if (emitter != NULL) {
                    emitter->otZ = 0x100;
                }
                frame = 0;
            }
            break;
        case 1:
            t = (frame << 12) / 55;
            if (frame < 8) {
                sheetPhase = ex->math_cos((frame << 10) >> 3);
            }
            curtainBright = ex->math_sin(t >> 2);
            domeBright = 0x1000;
            gridBright = 0x78;
            if (frame < 0x19) {
                ex->btlfx_drawRippleGrid((s32)(env->ot + 2), ((((frame << 12) / 24) * 0x118) >> 12) + 0xA, 8, 2, 0x8C, 1,
                                  fx_0fb_0_rippleRgb, 0x20, work->frame4 << 7, 0x100, 0x400);
            }
            if (frame == 0) {
                ex->btlfx_drawScreenTint((u32 *)(env->ot + 1), 1, 0, 0x23, 0);
                ex->btlfx_drawScreenTint((u32 *)(env->ot + 1), 2, 0xFF, 0xFF, 0);
            } else if (frame < 0xC) {
                flashTint = 0xFF - ((frame - 1) * 0xFF) / 11;
                ex->btlfx_drawScreenTint((u32 *)(env->ot + 1), 1, flashTint >> 1, flashTint, flashTint);
            }
            work->pos5C.vy = work->basePosC.vy + (-((0x1000 - ex->math_cos(t >> 1)) * 0x3000) >> 13) - 0x180;
            env->camera->points[3].vx = work->pos5C.vx;
            env->camera->points[3].vy = work->pos5C.vy;
            env->camera->points[3].vz = work->pos5C.vz;
            work->camPos64.vy += frame * 10;
            work->camPos64.vz -= frame * 9;
            env->camera->points[2].vx = work->camPos64.vx;
            env->camera->points[2].vy = work->camPos64.vy;
            env->camera->points[2].vz = work->camPos64.vz;
            work->pos14.vy = work->basePosC.vy + (-(t * 0x1800) >> 12) - 0x100;
            work->pos1C.vy = work->basePosC.vy + (-(t << 13) >> 12) - 0x100;
            if (frame < 0x1E) {
                for (k = 0; frame < 0xD ? k < 2 : k < 1; k++) {
                    if (work->rockCount8 < 0x2F) {
                        rock = ex->btlfx_poolSpawn((s16 *)entry->pool);
                        if (rock != NULL) {
                            big = work->rockCount8 >= 25;
                            idx = fx_0fb_0_rockOrder[work->rockCount8 % 25];
                            rock->kind4 = idx + big * 25;
                            if (big) {
                                rock->model20 = work->model6C[(idx & 1) + 25];
                            } else {
                                rock->model20 = work->model6C[idx];
                            }
                            rock->posC.vx = fx_0fb_0_origin.vx;
                            rock->posC.vy = fx_0fb_0_origin.vy;
                            rock->posC.vz = fx_0fb_0_origin.vz;
                            rock->posC.vx -= ((fx_0fb_0_rockOffsets[idx * 2] << 2) * (big + 3)) >> 1;
                            if (!big) {
                                rock->posC.vy += 0x300;
                            } else {
                                rock->posC.vy = work->camPos64.vy + 0x300;
                            }
                            rock->posC.vz -= ((fx_0fb_0_rockOffsets[idx * 2 + 1] << 2) * (big + 3)) >> 1;
                            work->rockCount8++;
                        }
                    }
                }
            }
            if (frame >= 0x37) {
                work->state0 = 2;
                entry->frame = -1;
                fx_0fb_0_rocksReleased = 1;
                ex->Hi_SetSummonMotion(SUMMON0FB(env)->summon54->motion180[0], SUMMON0FB(env)->summonModel60);
                ex->Hi_SetSummonMotFrame(SUMMON0FB(env)->summonModel60, 0);
                work->rot44.vy = 0xA70;
                work->summonScale38.vx = 0x1000;
                work->summonScale38.vy = 0x1000;
                work->summonScale38.vz = 0x1000;
                work->summonPos2C.vx = work->basePosC.vx - 0x5780;
                work->summonPos2C.vz = work->basePosC.vz - 0x3880;
                work->summonPos2C.vy = work->basePosC.vy - 0x16DD;
                work->rot44.vx = 0;
                work->rot44.vz = 0;
                work->pos14.vx = work->basePosC.vx;
                work->pos14.vz = work->basePosC.vz;
                work->pos14.vy = work->basePosC.vy - 0x2800;
                emitter = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, 6), (s32)&work->pos14);
                if (emitter != NULL) {
                    emitter->otZ = 0x80;
                }
                work->scale104.vx = 0x2000;
                work->scale104.vy = 0x555;
                work->scale104.vz = 0x2000;
                work->rot110.vx = 0;
                work->rot110.vy = 0;
                work->rot110.vz = 0;
                work->posF8.vx = work->posD8.vx + 0x2000;
                work->posF8.vz = work->posD8.vz + 0x2000;
                work->posF8.vy = work->basePosC.vy - 0x1500;
            }
            break;
        case 2:
            curtainBright = 0x1000;
            domeBright = 0x1000;
            gridBright = 0x78;
            t = (frame << 12) / 40;
            haloBright = ex->math_sin(t >> 2);
            if (frame >= 0x20) {
                ex->Hi_DrawEffModel(&work->rot44, (VECTOR *)&work->summonPos2C, (s32)&work->summonScale38, work->model11C);
                spriteAlpha = ((frame - 0x20) << 12) >> 3;
            }
            swingAng = t >> 1;
            work->pos5C.vx = work->basePosC.vx;
            work->pos5C.vz = work->basePosC.vz;
            work->pos5C.vy = work->basePosC.vy - 0x2800;
            swing = (domeBright - ex->math_cos(swingAng)) >> 1;
            {
            s16 xs; s32 ys; s32 q4; s32 cc;
            level = ex->math_sin(swingAng);
            q4 = swing * 4;
            xs = work->basePosC.vx - (q4 + swing);
            ys = swing - 0x2800;
            do { } while (0);
            cc = (level * 3) / 2;
            do { } while (0);
            work->camPos64.vz = work->basePosC.vz - q4 + 0x800;
            cc += 0x800;
            xs -= cc;
            work->camPos64.vx = xs;
            work->camPos64.vy = work->basePosC.vy + ys;
            }
            env->camera->points[2].vx = work->camPos64.vx;
            env->camera->points[2].vy = work->camPos64.vy;
            env->camera->points[2].vz = work->camPos64.vz;
            if (frame >= 0x28) {
                work->state0 = 3;
                entry->frame = -1;
            }
            break;
        case 3: {
            SVECTOR pos;
            struct { s32 wingSpread; } z1 = { frame };

        {
            curtainBright = 0x1000;
            domeBright = 0x1000;
            haloBright = 0x1000;
            gridBright = 0x78;
            spriteAlpha = 0x1000;
            shake = bob = 0;
            if (frame < 4) {
                bob = 0x400 - (frame << 10) / 3;
            } else if (frame < 0xB) {
                shakeAng = ((frame - 3) << 10) >> 3;
                shakeCos = ex->math_cos(shakeAng);
                shake = (shakeCos >> 8) * ((frame & 1) * 2 - 1) + ((spriteAlpha - ex->math_sin(shakeAng)) >> 6);
            }
            if (frame >= 8) {
                pullBack = 0x1000 - ex->math_cos((((frame - 8) << 12) / 17) >> 1);
                pos.vx = work->camPos64.vx - (s16)((pullBack >> 6) - 0x120);
                pos.vz = work->camPos64.vz - (pullBack >> 5);
                pos.vy = work->camPos64.vy - (((pullBack >> 1) * 0x302) >> 12);
                env->camera->points[2].vx = pos.vx;
                env->camera->points[2].vy = pos.vy;
                env->camera->points[2].vz = pos.vz;
            } else {
                pos.vx = work->camPos64.vx + ((frame << 12) >> 7) + 0x20;
                pos.vz = work->camPos64.vz;
                pos.vy = work->camPos64.vy;
                env->camera->points[2].vx = pos.vx;
                env->camera->points[2].vy = pos.vy;
                env->camera->points[2].vz = pos.vz;
            }
            work->summonPos2C.vy = work->summonPos2C.vy + shake - bob;
            ex->Hi_DrawSummonModel(&work->rot44, (VECTOR *)&work->summonPos2C, (s32)&work->summonScale38, SUMMON0FB(env)->summonModel60, 1);
            work->summonPos2C.vy = work->summonPos2C.vy + (bob + 0xF0);
            ex->Hi_DrawEffModel(&work->rot44, (VECTOR *)&work->summonPos2C, (s32)&work->summonScale38, work->model11C);
            work->summonPos2C.vy = work->summonPos2C.vy - (shake + 0xF0);
            if (frame == 3) {
                ex->Hi_GetSummonBonePos(SUMMON0FB(env)->summonModel60, 0x5B, (u16 *)&work->pos1C);
                work->pos1C.vy += 0x10;
                emitter = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, 2), (s32)&work->pos1C);
                if (emitter != NULL) {
                    emitter->otZ = 0x40;
                }
            }
            if (frame >= 0x19) {
                work->state0 = 4;
                entry->frame = -1;
                ex->Hi_SetSummonMotion(SUMMON0FB(env)->summon54->motion180[1], SUMMON0FB(env)->summonModel60);
                ex->Hi_SetSummonMotFrame(SUMMON0FB(env)->summonModel60, 0);
                fx_0fb_0_unitsRestored = 1;
                ex->Hi_GetSummonBonePos(SUMMON0FB(env)->summonModel60, 0x24, (u16 *)&work->pos5C);
                env->camera->points[3].vx = work->pos5C.vx;
                env->camera->points[3].vy = work->pos5C.vy;
                env->camera->points[3].vz = work->pos5C.vz;
                work->posD8.vz -= 0x5C00;
                work->posD8.vx -= 0x5C00;
                ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, 4), (s32)&work->pos4C);
                ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, 4), (s32)&work->pos54);
            }
        }
        } break;
        case 4: {
            SVECTOR ptA;
            SVECTOR ptB;
            BtlBone m;
            BtlBone m2;
            struct { s32 wingSpread; } z0 = { frame };
            s32 glowScale;

        {
            t = (frame << 12) / 44;
            ex->Hi_DrawSummonModel(&work->rot44, (VECTOR *)&work->summonPos2C, (s32)&work->summonScale38, SUMMON0FB(env)->summonModel60, 1);
            ex->Hi_SetEffModelOffset(work->model11C, 0x80);
            ex->Hi_DrawEffModel(&work->rot44, (VECTOR *)&work->summonPos2C, (s32)&work->summonScale38, work->model11C);
            spriteAlpha = 0x1000;
            lift = 0x1000 - ex->math_cos(t >> 1);
            ptA.vx = work->pos5C.vx;
            ptA.vy = work->pos5C.vy;
            ptA.vz = work->pos5C.vz;
            ptA.vy += (lift << 6) >> 12;
            env->camera->points[3].vx = ptA.vx;
            domeBright = 0x1000;
            env->camera->points[3].vy = ptA.vy;
            gridBright = 0x84;
            env->camera->points[3].vz = ptA.vz;
            if (frame == 0) {
                for (strand = 0; strand < 4; strand++) {
                    ex->Hi_GetSummonBonePos(SUMMON0FB(env)->summonModel60, fx_0fb_0_ribbonBones[strand][0], (u16 *)&ptA);
                    ex->Hi_GetSummonBonePos(SUMMON0FB(env)->summonModel60, fx_0fb_0_ribbonBones[strand][1], (u16 *)&ptB);
                    fxm_drawRibbonTrail(&ptA, &ptB, 0x3F3, &fx_0fb_0_ribbonRgb[strand & 1], &fx_0fb_0_ribbonRgb[(strand & 1) + 1], 0, fx_0fb_0_ribbons[strand].seg, 1);
                }
                for (strip = 0; strip < 10; strip++) {
                    ex->Hi_GetSummonBoneMatrix(SUMMON0FB(env)->summonModel60, fx_0fb_0_trailBones[strip], &m);
                    ex->btlfx_matrixPointAhead((MATRIX *)&m, 0x46, (s32)&ptA);
                    ex->btlfx_drawTrail((u8 *)ex->btlfx_getFileData(entry->file, 9), (s32)&ptA, 0, -1, 0x1000, 0x1000, 1,
                                        (u8 *)&fx_0fb_0_strips[strip], 2);
                }
            }
            for (strand = 0; strand < 4; strand++) {
                ex->Hi_GetSummonBonePos(SUMMON0FB(env)->summonModel60, fx_0fb_0_ribbonBones[strand][0], (u16 *)&ptA);
                ex->Hi_GetSummonBonePos(SUMMON0FB(env)->summonModel60, fx_0fb_0_ribbonBones[strand][1], (u16 *)&ptB);
                fxm_drawRibbonTrail(&ptA, &ptB, 0xB, &fx_0fb_0_ribbonRgb[strand & 1], &fx_0fb_0_ribbonRgb[(strand & 1) + 1], 0x1000, fx_0fb_0_ribbons[strand].seg, 1);
            }
            if (frame >= 0x26) {
                glowScale = ((((frame - 0x26) << 12) / 6) * 2) + 0x1000;
            } else {
                glowScale = (t >> 1) + 0x300;
            }
            for (strand = 0; strand < 10; strand++) {
                Fx0fb0Trail16 *pp;

                ex->Hi_GetSummonBoneMatrix(SUMMON0FB(env)->summonModel60, fx_0fb_0_trailBones[strand], &m2);
                ex->btlfx_matrixPointAhead((MATRIX *)&m2, 0x46, (s32)&ptA);
                ex->btlfx_drawTrail((u8 *)ex->btlfx_getFileData(entry->file, 9), (s32)&ptA, 0, 0, 0x800, 0x1000, 1,
                                    (u8 *)(pp = &fx_0fb_0_strips[strand]), 2);
                env->otZ = 0x18;
                ex->btlfx_drawParticles((u8 *)ex->btlfx_getFileData(entry->file, 0x10), &ptA, NULL, glowScale >> 1, 0, 1,
                                        ex->math_sin(t >> 2), 0, 0);
                env->otZ = 0;
                j = 0;
                ptWidth = (s16 *)pp + 3;
                for (j = 0; j < 16; j++) {
                    *ptWidth -= 8 + (t >> 8);
                    ptWidth += 4;
                }
            }
            ex->Hi_GetSummonBonePos(SUMMON0FB(env)->summonModel60, fx_0fb_0_trailBones[0], (u16 *)&work->pos4C);
            ex->Hi_GetSummonBonePos(SUMMON0FB(env)->summonModel60, fx_0fb_0_trailBones[5], (u16 *)&work->pos54);
            if (frame >= 0x2C) {
                work->state0 = 5;
                entry->frame = -1;
            }
        }
        } break;
        case 5: {
            SVECTOR ptA;
            BtlBone m;
            BtlBone m2;

        {
            domeBright = 0x1000;
            spriteAlpha = 0x1000;
            gridBright = 0x78;
            t = (frame << 12) / 21;
            if (frame == 0x11) {
                ex->btlfx_drawScreenTint((u32 *)(env->ot + 1), 1, 0, 0, 0x40);
            } else if (frame >= 0x11) {
                blueTint = ((frame - 0x11) << 7) / 3;
                ex->btlfx_drawScreenTint((u32 *)(env->ot + 1), 1, blueTint >> 2, blueTint >> 1, blueTint);
            }
            if (frame >= 5) {
                ex->Hi_SetSummonMotFrame(SUMMON0FB(env)->summonModel60, 0x31);
            }
            ex->Hi_DrawSummonModel(&work->rot44, (VECTOR *)&work->summonPos2C, (s32)&work->summonScale38, SUMMON0FB(env)->summonModel60, 1);
            sparkScale = t * 3 + 0x1000;
            for (node = 0; node < 10; node++) {
                ex->Hi_GetSummonBoneMatrix(SUMMON0FB(env)->summonModel60, fx_0fb_0_trailBones[node], &m);
                ex->btlfx_matrixPointAhead((MATRIX *)&m, 0x46, (s32)&ptA);
                env->otZ = 0x18;
                ex->btlfx_drawParticles((u8 *)ex->btlfx_getFileData(entry->file, 0x10), &ptA, NULL, sparkScale, 0, 1,
                                        0x1000, 0, 0);
                env->otZ = 0;
                if (node == 0) {
                    FX0FB_W(work->pos14, 0) = FX0FB_W(ptA, 0);
                    FX0FB_W(work->pos14, 1) = FX0FB_W(ptA, 1);
                }
                if (node == 5) {
                    FX0FB_W(work->pos1C, 0) = FX0FB_W(ptA, 0);
                    FX0FB_W(work->pos1C, 1) = FX0FB_W(ptA, 1);
                }
            }
            for (node = 0; node < 10; node++) {
                Fx0fb0Trail16 *pp;

                ex->Hi_GetSummonBoneMatrix(SUMMON0FB(env)->summonModel60, fx_0fb_0_trailBones[node], &m2);
                ex->btlfx_matrixPointAhead((MATRIX *)&m2, 0x46, (s32)&ptA);
                ex->btlfx_drawTrail((u8 *)ex->btlfx_getFileData(entry->file, 9), (s32)&ptA, 0, 0, 0x800, 0x1000 - t, 1,
                                    (u8 *)(pp = &fx_0fb_0_strips[node]), 2);
                j = 0;
                fadeWidth = (s16 *)pp + 3;
                for (j = 0; j < 16; j++) {
                    *fadeWidth -= 0x18;
                    fadeWidth += 4;
                }
            }
            ex->btlfx_drawRings((u8 *)ex->btlfx_getFileData(entry->file, 0x14), &work->pos14, NULL, (t >> 1) + 0xAAA, 0, 1,
                              t >> 1);
            ex->btlfx_drawRings((u8 *)ex->btlfx_getFileData(entry->file, 0x14), &work->pos1C, NULL, (t >> 1) + 0xAAA, 0, 1,
                              t >> 1);
            if (frame == 4) {
                emitter = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, 0x15), (s32)&work->pos14);
                if (emitter != NULL) {
                    emitter->otZ = 0x80;
                }
                emitter = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, 0x15), (s32)&work->pos1C);
                if (emitter != NULL) {
                    emitter->otZ = 0x80;
                }
            }
            if (frame == 7) {
                emitter = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, 0xF), (s32)&work->pos14);
                if (emitter != NULL) {
                    emitter->otZ = 0x80;
                }
                emitter = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, 0xF), (s32)&work->pos1C);
                if (emitter != NULL) {
                    emitter->otZ = 0x80;
                }
            }
            if (frame >= 0x15) {
                work->state0 = 6;
                entry->frame = -1;
                ex->Hi_SetSummonMotion(SUMMON0FB(env)->summon54->motion180[2], SUMMON0FB(env)->summonModel60);
            }
        }
        } break;
        case 6: {
            VECTOR pos;
            VECTOR sc;
            SVECTOR wingRotA;
            SVECTOR wingRotB;
            BtlBone m;
            BtlBone m2;

        {
            gridBright = 0xF1;
            if (frame < 0xD) {
                ex->Hi_SetSummonMotFrame(SUMMON0FB(env)->summonModel60, frame);
            } else {
                ex->Hi_SetSummonMotFrame(SUMMON0FB(env)->summonModel60, 0xC);
            }
            ex->Hi_DrawSummonModel(&work->rot44, (VECTOR *)&work->summonPos2C, (s32)&work->summonScale38, SUMMON0FB(env)->summonModel60, 1);
            ex->Hi_DrawEffModel(&work->rot44, (VECTOR *)&work->summonPos2C, (s32)&work->summonScale38, work->model11C);
            env->otZ = 0x40;
            for (bone = 0; bone < 10; bone++) {
                ex->Hi_GetSummonBoneMatrix(SUMMON0FB(env)->summonModel60, fx_0fb_0_trailBones[bone], (BtlBone *)&pos);
                ex->btlfx_matrixPointAhead((MATRIX *)&pos, 0x46, (s32)&fx_0fb_0_bonePts[bone]);
            }
            for (bone = 0; bone < 2; bone++) {
                ex->btlfx_drawRings((u8 *)ex->btlfx_getFileData(entry->file, 0xA), &fx_0fb_0_bonePts1[bone * 5], NULL, 0x1000, 0, 1,
                                  0x1000);
            }
            env->otZ = 0;
            if (frame >= 4) {
                wingGrow = 0;
                if (frame < 5) {
                    burstT = frame << 12;
                    wingAng = burstT >> 2;
                    sc.vx = (0x1000 - ex->math_cos(burstT >> 4)) / 12;
                    sc.vy = (ex->math_cos(burstT >> 4) >> 1) + 0xC00;
                    sc.vz = wingAng / 8;
                    wingSpread = 0;
                    if (frame == 4) {
                        ex->btlfx_drawScreenTint((u32 *)(env->ot + 1), 2, 0x80, 0x80, 0x80);
                    }
                } else {
                    step = frame - 4;
                    if (frame < 8) {
                        level = 0x80 - ((frame << 7) >> 3);
                        if (frame == 0) {
                            level = 0xFF;
                        }
                        ex->btlfx_drawScreenTint((u32 *)(env->ot + 1), 1, level, level, level);
                    }
                    if (step < 0x20) {
                        wingSpread = (ex->math_sin((step << 10) >> 5) >> 5) + 0x2E;
                    } else {
                        wingSpread = 0xAE;
                    }
                    if (step < 8) {
                        wingAng = (step << 12) >> 5;
                        sc.vx = ex->math_cos(wingAng) + 0x155;
                        sc.vy = 0xC00 - wingAng;
                        sc.vz = ((step << 12) >> 6) + 0x200;
                    } else {
                        sc.vx = ((step - 8) << 6) + 0x155;
                        sc.vy = ((step - 8) << 5) + 0x800;
                        wingGrow = (step - 8) << 5;
                        sc.vz = ((step - 8) << 6) + 0x400;
                    }
                }
                ex->Hi_GetSummonBoneMatrix(SUMMON0FB(env)->summonModel60, 7, &m);
                ex->btlfx_matrixAngles((MATRIX *)&m, (s16 *)&wingRotA);
                wingRotA.vz = 0xA00;
                wingRotA.vx -= 0x2F8;
                wingRotA.vy += 0x300;
                ex->Hi_GetSummonBoneMatrix(SUMMON0FB(env)->summonModel60, 0x31, &m2);
                ex->btlfx_matrixAngles((MATRIX *)&m2, (s16 *)&wingRotB);
                wingRotB.vz = 0x600;
                wingRotB.vx -= 0x2F8;
                wingRotB.vy -= 0x300;
                for (bone = 9; bone >= 0; bone--) {
                    pos.vx = fx_0fb_0_bonePts[bone].vx;
                    pos.vy = fx_0fb_0_bonePts[bone].vy;
                    pos.vz = fx_0fb_0_bonePts[bone].vz;
                    if (bone < 5) {
                        wingRotB.vx += wingSpread;
                        sc.vx += wingGrow;
                        rp = &wingRotB;
                    } else {
                        wingRotA.vx += wingSpread;
                        sc.vx += wingGrow;
                        rp = &wingRotA;
                    }
                    ex->Hi_DrawEffModel(rp, &pos, (s32)&sc, work->model124);
                    if (bone == 4) {
                        sc.vx -= wingGrow * 5;
                    }
                }
            }
            if (frame == 0) {
                work->pos5C.vy += 0x20;
                env->camera->points[3].vx = work->pos5C.vx;
                env->camera->points[3].vy = work->pos5C.vy;
                env->camera->points[3].vz = work->pos5C.vz;
            }
            if (frame == 4) {
                for (bone = 0; bone < 2; bone++) {
                    emitter = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, 8), (s32)&fx_0fb_0_bonePts1[bone * 5]);
                    if (emitter != NULL) {
                        emitter->otZ = 0x100;
                    }
                }
            }
            if (frame < 2) {
                fadeIn = 0xFF - frame * 0xFF / 2;
                ex->btlfx_drawScreenTint((u32 *)(env->ot + 1), 1, fadeIn, fadeIn, fadeIn >> 1);
            }
            if (frame >= 0x1C) {
                fadeOut = (frame - 0x1B) * 0x33;
                ex->btlfx_drawScreenTint((u32 *)(env->ot + 1), 1, fadeOut, fadeOut, fadeOut >> 1);
            }
            if (frame >= 0x20) {
                for (node = 0; node < 27; node++) {
                    ex->Hi_FreeEffModel(work->model6C[node]);
                }
                return 1;
            }
        }
        } break;
        }
        {
        SVECTOR pos;

        if (curtainBright != 0) {
            pos.vx = fx_0fb_0_origin.vx;
            curtainCol = curtainBright >> 5;
            pos.vy = fx_0fb_0_origin.vy;
            pos.vz = fx_0fb_0_origin.vz;
            pos.vy -= 0x79C0;
            pos.vx += 0xA00;
            pos.vz -= 0x1400;
            fx_0fb_0_drawWaveCurtain(&pos, 0x4000, curtainCol, 0x118, 0xFFFF, 0x10, 0x90, 0x7D0, work->frame4 * 0x5C, 0xF0, 1,
                          work->frame4 * 0x37);
            pos.vy += 0x640;
            pos.vx -= 0x1400;
            pos.vz += 0x2800;
            fx_0fb_0_drawWaveCurtain(&pos, 0x4000, curtainCol, 0x118, 0xFFFF, 0x10, 0x90, 0x76C, work->frame4 * 0x5C + 0x400, 0xF0, 1,
                          work->frame4 * 0x37 - 0x200);
        }
        if (sheetPhase != 0) {
            SVECTOR rot;
            VECTOR sc;

            env->otZ = -0x300;
            rot.vx = 0x400;
            sc.vx = 0x1000;
            rot.vz = 0x400;
            sc.vy = 0x1000;
            sc.vz = 0x1000;
            rot.vy = 0;
            pos.vz = (0x1000 - sheetPhase) * 2;
            pos.vx = fx_0fb_0_origin.vx;
            pos.vy = fx_0fb_0_origin.vy;
            fx_0fb_0_drawWaveSheet(&pos, &rot, &sc, ex->math_sin(sheetPhase >> 2) >> 5, 0x118, 0xFFFF, 0x10, 0x90, 0x400, sheetPhase, 0x180, 1);
            env->otZ = 0;
        }
        if (curtainBright != 0 || sheetPhase != 0) {
            if (work->state0 == 0 || (work->state0 == 1 && frame < 0x10)) {
                ex->btlfx_drawBackdrop((BtlBgLayers *)ex->btlfx_getFileData(entry->file, 0xD), work->frame4);
            } else {
                ex->btlfx_drawBackdrop((BtlBgLayers *)ex->btlfx_getFileData(entry->file, 0xE), work->frame4);
            }
        }
        if (domeBright != 0) {
            work->rotF0.vz += 7;
            ex->Hi_DrawEffModel(&work->rotF0, (VECTOR *)&work->posD8, (s32)&work->scaleE4, work->model118);
            if (domeBright != 0x1000) {
                domeRgb = (domeBright >> 5) - 0x80;
                ex->Hi_ModifyEffModelRGB(work->model118, domeRgb, domeRgb, domeRgb);
            }
            if (haloBright != 0) {
                work->rot110.vy += 4;
                ex->Hi_DrawEffModel(&work->rot110, (VECTOR *)&work->posF8, (s32)&work->scale104, work->model120);
                if (haloBright != 0x1000) {
                    haloRgb = (haloBright >> 5) - 0x80;
                    ex->Hi_ModifyEffModelRGB(work->model120, haloRgb, haloRgb, haloRgb);
                }
            }
        }
        if (spriteAlpha != 0) {
            step = (work->frame4 * 16) % 240;
            ex->btlfx_drawSpriteAtOt(8, (u8 *)ex->btlfx_getFileData(entry->file, 3), 0xA0 - step, 0xA0, 0, 3, spriteAlpha);
            ex->btlfx_drawSpriteAtOt(8, (u8 *)ex->btlfx_getFileData(entry->file, 3), 0x280 - step, 0xA0, 0, 3, spriteAlpha);
        }
        if (gridBright != 0) {
            CVECTOR col;
            SVECTOR rot;
            VECTOR sc;

            col.r = col.g = col.b = gridBright;
            sc.vx = 0x1060;
            sc.vy = 0x1060;
            sc.vz = 0x1000;
            rot.vx = 0;
            rot.vy = 0;
            rot.vz = 0;
            ex->btlfx_drawScreenGrid(0x400, 0xA0, 0x70, 0x4D, 0x37, 4, 4, 2, 2, &rot, &sc, &col, 0, 0);
        }
        }
        work->frame4++;
    }
    return 0;
}

s32 fx_0fb_0_op81(s32 mode, Fx0fb0Work81 *work, Unk800F78A4 *env, Unk80100C78Entry *entry, s32 *args) {
    BtlfxExports *ex = BTLFX_EXPORTS(env);
    SVECTOR unitPos;
    s32 frame;
    s32 t;
    s32 unit;

    if (mode == 0) {
        *args = 0x10C;
    } else if (mode == 1) {
        fx_0fb_0_env = env;
        work->unitCount104 = env->targets->targetCount;
        work->state0 = 0;
        for (unit = 0; unit < work->unitCount104; unit++) {
            if (ex->btlfx_isEnemyUnit(unit) != 0) {
                ex->btlfx_getUnitHomePos(unit, (s16 *)&work->homePos4[unit]);
                work->lift108 = ex->btlfx_randCentered(0x400) - 0x3000;
                ex->btlfx_getUnitRot(unit, (s16 *)&work->rot44[unit]);
                BTLFX_COPY_SVEC(&work->curRotC4[unit], &work->rot44[unit]);
                work->spin84[unit].vx = ex->btlfx_randCentered(0x100);
                work->spin84[unit].vy = ex->btlfx_randCentered(0x180);
                work->spin84[unit].vz = ex->btlfx_randCentered(0x20);
            }
        }
    } else {
        frame = entry->frame;
        if (work->state0 == 0) {
            t = (frame << 12) / 46;
            for (unit = 0; unit < work->unitCount104; unit++) {
                if (ex->btlfx_isEnemyUnit(unit) != 0) {
                    BTLFX_COPY_SVEC(&unitPos, &work->homePos4[unit]);
                    unitPos.vy += (ex->math_sin(t >> 2) * work->lift108) >> 12;
                    ex->btlfx_setUnitPos(unit, (s16 *)&unitPos);
                    work->curRotC4[unit].vx += work->spin84[unit].vx;
                    work->curRotC4[unit].vy += work->spin84[unit].vy;
                    work->curRotC4[unit].vz += work->spin84[unit].vz;
                    ex->btlfx_setUnitRot(unit, (s16 *)&work->curRotC4[unit]);
                }
            }
            if (frame >= 0x2E) {
                work->state0 = 1;
                entry->frame = -1;
            }
        } else {
            for (unit = 0; unit < work->unitCount104; unit++) {
                if (ex->btlfx_isEnemyUnit(unit) != 0) {
                    work->curRotC4[unit].vx += work->spin84[unit].vx;
                    work->curRotC4[unit].vy += work->spin84[unit].vy;
                    work->curRotC4[unit].vz += work->spin84[unit].vz;
                    ex->btlfx_setUnitRot(unit, (s16 *)&work->curRotC4[unit]);
                }
            }
            if (fx_0fb_0_unitsRestored != 0) {
                for (unit = 0; unit < work->unitCount104; unit++) {
                    if (ex->btlfx_isEnemyUnit(unit) != 0) {
                        ex->btlfx_setUnitRot(unit, (s16 *)&work->rot44[unit]);
                        ex->btlfx_getUnitHomePos(unit, (s16 *)&unitPos);
                        ex->btlfx_setUnitPos(unit, (s16 *)&unitPos);
                    }
                }
                return 1;
            }
        }
    }
    return 0;
}

s32 fx_0fb_0_op87(s32 mode, void *work, Unk800F78A4 *env, Unk80100C78Entry *entry, s32 *args) {
    SVECTOR camPos;

    if (mode == 0) {
        *args = 4;
    } else if (mode == 1) {
        camPos.vx = env->targets->centerX;
        camPos.vy = env->targets->centerY;
        camPos.vz = env->targets->centerZ;
        camPos.vy -= 0x230;
        env->camera->points[2].vx = camPos.vx;
        env->camera->points[2].vy = camPos.vy;
        env->camera->points[2].vz = camPos.vz;
        fx_0fb_0_env = env;
        return 1;
    } else {
        return 0;
    }
    return 0;
}
