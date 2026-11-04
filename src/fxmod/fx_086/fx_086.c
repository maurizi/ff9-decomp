#include "fxmod/fx_086.h"

static void fxm_drawTexCylinder(SVECTOR *pos, SVECTOR *rot, VECTOR *scale, s32 r0, s32 r1, s32 w0, s32 w1, s32 bright, s32 tpage,
                   s32 clut, s32 u, s32 v, s32 ang, s32 dy, s32 abr, s32 dang) {
    BtlfxExports *ex = BTLFX_EXPORTS(fx_086_env);
    Fx086Rgbc colFlat;
    Fx086Rgbc colLit;
    Fx086Rgbc colDark;
    MATRIX m;
    SVECTOR quad[4];
    s32 otz;
    POLY_GT4 *gt4;
    POLY_FT4 *ft4;
    s32 seg;
    s32 j;
    s32 row;
    s32 rowY;
    s32 span;
    s32 y0;
    s32 y1;
    s32 bulge0;
    s32 bulge1;
    s32 rad0;
    s32 rad1;
    s32 ang1;
    s32 ft;
    s32 tile;
    s32 u0;
    s32 u1;
    s32 v0;
    s32 v1;
    s32 nextJ;
    s32 ratio1;
    s32 ratio0;

    gte_SetRotMatrix(fx_086_env->viewMatrix);
    gte_SetTransMatrix(fx_086_env->viewMatrix);
    gte_ldv0(pos);
    gt4 = (POLY_GT4 *)fx_086_env->prim;
    *(s32 *)&colDark = 0;
    ft4 = (POLY_FT4 *)(gt4 + 2);
    fx_086_env->prim = (u8 *)(ft4 + 14);
    colFlat.r = colFlat.g = colFlat.b = bright;
    colLit.r = colLit.g = colLit.b = bright;
    colFlat.code = abr != 0xFF ? 0x2E : 0x2C;
    colDark.code = abr != 0xFF ? 0x3E : 0x3C;
    tpage |= TPAGE_ABR(abr);
    colLit.code = colDark.code;
    gte_rtv0tr_b();
    ex->math_rotMatrixYXZ(rot, &m);
    ex->ScaleMatrix(&m, scale);
    gte_stlvnl(m.t);
    ex->MulRotMatrix(&m);
    gte_SetRotMatrix(&m);
    gte_SetTransMatrix(&m);
    seg = 0;
    rowY = 0;
    w0 >>= 1;
    w1 >>= 1;
    span = dy; /* a copy: the original keeps it in its own stack slot */
    for (row = 0; row < 1; row++, rowY += dy) {
        for (j = 0; j < 16; j++, seg++, ang += dang << 8) {
            nextJ = (j + 1) << 8;
            y0 = rowY + (((j << 8) * dy) >> 12);
            y1 = rowY + ((nextJ * dy) >> 12);
            /* one variable for span / 2 and the second ratio: the original recomputes span >> 1 every iteration */
            ratio1 = span >> 1;
            ratio0 = ((y0 + ratio1) << 12) / span;
            ratio1 = ((y1 + ratio1) << 12) / span;
            bulge0 = ex->math_sin(ratio0 >> 1);
            bulge1 = ex->math_sin(ratio1 >> 1);
            rad0 = ex->btlfx_lerp(r0, r1, bulge0);
            rad1 = ex->btlfx_lerp(r0, r1, bulge1);
            quad[0].vx = quad[1].vx = (ex->math_cos(ang) * rad0) >> 12;
            quad[0].vy = quad[1].vy = (ex->math_sin(ang) * rad0) >> 12;
            quad[0].vz = y0 - ex->btlfx_lerp(w0, w1, bulge0);
            quad[1].vz = y0 + ex->btlfx_lerp(w0, w1, bulge1);
            ang1 = ang + (dang << 8);
            quad[2].vx = quad[3].vx = (ex->math_cos(ang1) * rad1) >> 12;
            quad[2].vy = quad[3].vy = (ex->math_sin(ang1) * rad1) >> 12;
            quad[2].vz = y1 - ex->btlfx_lerp(w0, w1, bulge0);
            quad[3].vz = y1 + ex->btlfx_lerp(w0, w1, bulge1);
            gte_ldv3c(quad);
            gte_rtpt();
            tile = seg & 1;
            u0 = u + (tile << 6);
            u1 = u0 + 0x40;
            if (tile != 0) {
                u1 = u0 + 0x3F;
            }
            v0 = v;
            v1 = v0 + 0x3F;
            gte_nclip();
            if (seg == 0) {
                ft = 0;
                *(s32 *)&gt4->r0 = *(s32 *)&gt4->r1 = *(s32 *)&colDark;
                *(s32 *)&gt4->r2 = *(s32 *)&gt4->r3 = *(s32 *)&colLit;
            } else if (seg >= 15) {
                ft = 0;
                *(s32 *)&gt4->r2 = *(s32 *)&gt4->r3 = *(s32 *)&colDark;
                *(s32 *)&gt4->r0 = *(s32 *)&gt4->r1 = *(s32 *)&colLit;
            } else {
                ft = 1;
                *(s32 *)&ft4->r0 = *(s32 *)&colFlat;
            }
            gte_stopz(&otz);
            if (otz != 0) {
                if (ft) {
                    ft4->u0 = ft4->u1 = u0;
                    ft4->u2 = ft4->u3 = u1;
                    ft4->v0 = ft4->v2 = v0;
                    ft4->v1 = ft4->v3 = v1;
                    ft4->tpage = tpage;
                    ft4->clut = clut;
                    gte_stsxy3(&ft4->x0, &ft4->x1, &ft4->x2);
                } else {
                    gt4->u0 = gt4->u1 = u0;
                    gt4->u2 = gt4->u3 = u1;
                    gt4->v0 = gt4->v2 = v0;
                    gt4->v1 = gt4->v3 = v1;
                    gt4->tpage = tpage;
                    gt4->clut = clut;
                    gte_stsxy3(&gt4->x0, &gt4->x1, &gt4->x2);
                }
                gte_avsz3();
                gte_stszotz(&otz);
                gte_ldv0(&quad[3]);
                gte_rtps();
                if (ft) {
                    ex->btlfx_addPrim(0x09000000, otz, ft4);
                    gte_stsxy(&ft4->x3);
                    ft4++;
                } else {
                    ex->btlfx_addPrim(0x0C000000, otz, gt4);
                    gte_stsxy(&gt4->x3);
                    gt4++;
                }
            }
        }
    }
}

static void fxm_randDiscPoints(s32 n, s32 radius, SVECTOR *pts) {
    SVECTOR *pt;
    s32 *last;
    BtlfxExports *ex = BTLFX_EXPORTS(fx_086_env);
    s32 i;
    s32 rad;
    s32 ang;

    pt = pts;
    last = (s32 *)&pt[n];
    for (i = 0; i < n; i++, last++, pt++) {
        *last = 0x7F7F7F7F;
        rad = ex->btlfx_randRange(radius >> 2, radius >> 1);
        ang = ex->btlfx_rand() & 0xFFF;
        pt->vx = (ex->math_cos(ang) * rad) >> 12;
        pt->vy = (ex->math_sin(ang) * rad) >> 12;
        pt->vz = ex->btlfx_rand() % 1280 - 0x100;
    }
}

static void fxm_drawStreakField(u32 *ot, s32 n, s32 projH, s32 dz, void *rot, SVECTOR *pts, CVECTOR *rgb, s32 fade, s32 abr) {
    Unk800F78A4 *env = fx_086_env;
    CVECTOR col;
    MATRIX m;
    s32 oldH;
    u32 otz;
    BtlfxExports *ex = BTLFX_EXPORTS(env);
    LINE_G2 *prim;
    s32 *last;
    SVECTOR *cur;
    s32 live;
    s32 i;

    if (fade < 0) {
        fade = 0x1000;
    }
    prim = (LINE_G2 *)env->prim;
    env->prim = (u8 *)(prim + n);
    if (rgb == NULL) {
        col.r = col.g = col.b = 0;
    } else {
        gte_DpqColorFar_nst(rgb, fade);
        gte_strgb(&col);
    }
    gte_SetRotMatrix(env->viewMatrix);
    gte_SetTransMatrix(env->viewMatrix);
    gte_ReadGeomScreen(&oldH);
    gte_SetGeomScreen(projH);
    ex->math_rotMatrixYXZ(rot, &m);
    m.t[1] = 0;
    m.t[0] = 0;
    m.t[2] = projH;
    gte_SetRotMatrix(&m);
    gte_SetTransMatrix(&m);
    cur = pts;
    last = (s32 *)&cur[n];
    for (i = 0; i < n; i++, last++, cur++, prim++) {
        live = *last != 0x7F7F7F7F;
        gte_ldv0(cur);
        gte_rtps();
        cur->vz -= dz;
        if (dz > 0 && cur->vz < -0x100) {
            cur->vz = 0x400;
            *last = 0x7F7F7F7F;
        } else if (dz < 0 && cur->vz > 0x300) {
            cur->vz = -0x200;
            *last = 0x7F7F7F7F;
        } else {
            gte_stsxy(&prim->x0);
            gte_stszotz(&otz);
            if (otz < 0x200 && live) {
                gte_DpqColorFar_nst(&col, ((0x200 - otz) << 12) >> 9);
                gte_strgb(&prim->r0);
                *(s32 *)&prim->x1 = *last;
                *(s32 *)&prim->r1 = 0;
                prim->code = abr != 0xFF ? 0x52 : 0x50;
                ex->btlfx_linkPrimAbr(0x4000000, ot, prim, abr);
            }
            *last = *(s32 *)&prim->x0;
        }
    }
    gte_SetGeomScreen(oldH);
}

s32 fx_086_op80(s32 mode, Fx086Work80 *work, Unk800F78A4 *env, Unk80100C78Entry *entry, s32 *args) {
    BtlfxExports *ex = BTLFX_EXPORTS(env);
    VECTOR pos;
    VECTOR scale;
    SVECTOR rot;
    Pool6CEntry *emitter;
    s32 frame;
    s32 glow;
    s32 pulseAng;
    s32 spriteAlpha;
    s32 shake;
    s32 py;
    s32 px;
    s32 tintT;
    s32 glowT;

    if (mode == 0) {
        *args = 0x48;
    } else if (mode == 1) {
        fx_086_env = env;
        work->state0 = 1;
        work->frame4 = 0;
        fxm_randDiscPoints(0x180, 0x400, fx_086_streakPts);
        work->unk8[0] = 0xA0;
        work->unk8[1] = 0x78;
        work->unk8[2] = 0;
        work->unk8[3] = 0x78;
        work->unk8[4] = 0xC8;
        work->unk8[5] = 0x78;
        ex->btlfx_initEffModels(fx_086_effModels, 8);
        work->model24 = ex->Hi_RegisterTexEffModel((BtlModelHeader *)ex->btlfx_getFileData(entry->file, FX_METEOR_ROCK), 0x9A, 0x3E00, 0, 0);
        work->model28 = ex->Hi_RegisterTexEffModel((BtlModelHeader *)ex->btlfx_getFileData(entry->file, FX_METEOR_STREAK_SHELL), 0x139, 0xFFFF, 0, 0x80);
        work->model2C = ex->Hi_RegisterTexEffModel((BtlModelHeader *)ex->btlfx_getFileData(entry->file, FX_METEOR_FIRE_DOME), 0xB9, 0x3D40, 0, 0);
        work->model30 = ex->Hi_RegisterTexEffModel((BtlModelHeader *)ex->btlfx_getFileData(entry->file, FX_METEOR_RING_BAND), 0x3C, 0x3D04, 0, 0x80);
        work->model34 = ex->Hi_RegisterGouEffModel((BtlModelHeader *)ex->btlfx_getFileData(entry->file, FX_METEOR_STREAK));
        work->model38 = ex->Hi_RegisterGouEffModel((BtlModelHeader *)ex->btlfx_getFileData(entry->file, FX_METEOR_BOWL));
        work->model3C = ex->Hi_RegisterGouEffModel((BtlModelHeader *)ex->btlfx_getFileData(entry->file, FX_METEOR_RING_DISC));
        work->model40 = ex->Hi_RegisterTexEffModel((BtlModelHeader *)ex->btlfx_getFileData(entry->file, FX_METEOR_SKY_CYLINDER), 0x97, 0x3D80, 0, 0);
    } else {
        frame = entry->frame;
        switch (work->state0) {
        case 1: {
        {
            SVECTOR streakDir;
            CVECTOR col;
            s32 travel;
            s32 unused0;
            s32 unused1;
            s32 fadeIn;
            s32 jitter;
            s32 otz;

            pos.vx = 0;
            travel = (frame << 11) / 49;
            pos.vz = 0x800 - travel;
            pos.vy = travel - 0x800;
            fadeIn = (frame << 12) / 49;
            if (frame >= 8) {
                jitter = (frame - 8) * 2;
                pos.vx += ex->btlfx_randCentered((frame - 8) * 3);
                pos.vy += ex->btlfx_randCentered(jitter);
                pos.vz += ex->btlfx_randCentered(jitter);
            }
            rot.vx = 0x200;
            rot.vy = 0;
            rot.vz = -frame * 0x40;
            scale.vx = 0x1000;
            scale.vy = 0x1000;
            scale.vz = 0x1000;
            work->pos14.vx = pos.vx;
            work->pos14.vy = pos.vy;
            work->pos14.vz = pos.vz - 0x46;
            work->pos14.vy += 0x1D;
            env->camera->points[3].vx = pos.vx;
            env->camera->points[3].vy = pos.vy;
            env->camera->points[3].vz = pos.vz;
            env->camera->points[3].vz += frame * 900 / 49 - 0x100;
            env->camera->points[3].vy += 0x180 - frame * 900 / 49;
            ex->Hi_DrawEffModel(&rot, &pos, (s32)&scale, work->model24);
            ex->btlfx_drawScrollMove(work->frame4 * 8, 0x1C0, 0x100, 0x40, 0x80, 0x200, 0x100);
            scale.vz = scale.vz * 3 / 2;
            ex->Hi_DrawEffModel(&rot, &pos, (s32)&scale, work->model40);
            scale.vz = scale.vz * 2 / 3;
            ex->Hi_ModifyEffModelRGB(work->model40, (travel >> 5) - 0x80, (travel >> 5) - 0x80, (travel >> 5) - 0x80);
            if (frame >= 8) {
                glowT = ((frame - 8) << 12) / 41;
                rot.vx = 0x200;
                rot.vy = 0;
                rot.vz = 0;
                ex->Hi_DrawEffModel(&rot, &pos, (s32)&scale, work->model28);
                ex->Hi_ModifyEffModelAbr(work->model28, 1);
                ex->Hi_ModifyEffModelRGB(work->model28, (glowT >> 5) - 0x80, (glowT >> 5) - 0x80, (glowT >> 5) - 0x80);
                ex->btlfx_drawBackdrop((BtlBgLayers *)ex->btlfx_getFileData(entry->file, FX_METEOR_BACKDROP), work->frame4);
            }
            otz = -0x64;
            col.r = 0x20;
            col.b = 0xFF;
            col.g = 0x80;
            streakDir.vx = 0x200;
            streakDir.vy = -0x400;
            streakDir.vz = 0;
            if (frame < 0x18) {
                otz = -0x46;
            }
            fxm_drawStreakField((u32 *)(env->ot + 8), 0x180, 0x80, otz, &streakDir, fx_086_streakPts, &col,
                          ex->math_cos((frame << 10) / 49) * 3 / 4 + 0x400, 1);
            if (frame == 2) {
                work->emit44 = emitter = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, FX_METEOR_CONE_SPRITES), (s32)&work->pos14);
                if (emitter != NULL) {
                    emitter->otZ = 0x5C;
                }
            }
            if (frame >= 25) {
                CVECTOR col2;
                SVECTOR rot2;
                VECTOR sc2;

                tintT = ((frame - 24) << 12) / 25;
                ex->btlfx_drawScreenTint((u32 *)(env->ot + 1), 1, (tintT << 6) >> 12, 0, 0);
                col2.r = col2.g = col2.b = ((ex->math_sin(tintT >> 2) << 5) >> 12) + 0x10;
                sc2.vx = 0x1080;
                sc2.vy = 0x1080;
                sc2.vz = 0x1000;
                rot2.vx = rot2.vy = rot2.vz = 0;
                ex->btlfx_drawScreenGrid(1, 0xA0, 0x78, 0x20, 0x18, 0xA, 0xA, 5, 5, &rot2, &sc2, &col2, 1, NULL);
            }
            px = 0x1B8 - (frame & 7) * 60;
            py = 0x140 - (frame & 7) * 60;
            ex->btlfx_drawSpriteAtOt(2, (u8 *)ex->btlfx_getFileData(entry->file, FX_METEOR_DARK_CLOUDS), px + 0x64, py + 0x78, 0, 1, fadeIn);
            px = 0x1B8 - (frame + 5) % 10 * 60;
            py = 0x140 - (frame + 5) % 10 * 60;
            ex->btlfx_drawSpriteAtOt(2, (u8 *)ex->btlfx_getFileData(entry->file, FX_METEOR_CLOUDS), px - 0x40, py + 0x78, 0, 1, fadeIn);
            if (frame >= 49) {
                ex->btlfx_stopEmitter(work->emit44);
                BTLFX_COPY_SVEC(&work->pos1C, &env->targets->centerX);
                env->camera->points[3].vx = work->pos1C.vx;
                env->camera->points[3].vy = work->pos1C.vy;
                env->camera->points[3].vz = work->pos1C.vz;
                env->camera->points[3].vx += 0x1800;
                env->camera->points[3].vx -= 0x1000;
                ex->btlfx_startScreenFade(1, 1);
                work->state0 = 2;
                entry->frame = -1;
            }
        }
            break;
        }
        case 2: {
        {
            SVECTOR camFrom;
            SVECTOR camTo;
            SVECTOR camPos;
            VECTOR unused; /* never used: keeps the original stack layout */
            s32 ang;
            s32 bright;
            s32 size;
            s32 dist;
            s32 i;

            ang = ((frame << 12) / 14) >> 2;
            pos.vz = work->pos1C.vx + ex->math_cos(ang) * 3 / 2;
            pos.vy = work->pos1C.vy - ex->math_cos(ang) * 3 / 2;
            pos.vx = 0;
            rot.vx = 0x200;
            rot.vy = 0;
            rot.vz = -frame * 0x40;
            size = 0x3800 - ex->math_cos(ang);
            scale.vx = size;
            scale.vy = size;
            scale.vz = size;
            work->pos14.vx = pos.vx;
            work->pos14.vy = pos.vy;
            work->pos14.vz = pos.vz;
            if (frame < 11) {
                camFrom.vx = pos.vx;
                camFrom.vy = pos.vy;
                camFrom.vz = pos.vz;
                BTLFX_COPY_SVEC(&camTo, &work->pos1C);
                camTo.vy -= 0x200;
                ex->btlfx_lerpVec((s16 *)&camFrom, (s16 *)&camTo, (frame << 12) / 10, (s16 *)&camPos);
                env->camera->points[3].vx = camPos.vx;
                env->camera->points[3].vy = camPos.vy;
                env->camera->points[3].vz = camPos.vz;
            }
            ex->Hi_DrawEffModel(&rot, &pos, (s32)&scale, work->model24);
            ex->Hi_DrawEffModel(&rot, &pos, (s32)&scale, work->model28);
            if (frame >= 7) {
                CVECTOR col;
                SVECTOR rot2;
                VECTOR sc2;

                bright = ((ex->math_sin(((frame - 6) << 12) / 6 >> 2) << 6) >> 12) + 0x20;
                col.r = bright;
                col.g = bright >> 1;
                col.b = bright >> 2;
                sc2.vx = 0x1200;
                sc2.vy = 0x1200;
                sc2.vz = 0x1000;
                rot2.vx = rot2.vy = rot2.vz = 0;
                ex->btlfx_drawScreenGrid(1, 0xA0, 0x70, 0x46, 0x38, 4, 4, 2, 2, &rot2, &sc2, &col, 1, NULL);
            }
            {
            VECTOR sc;

            sc.vx = sc.vy = sc.vz = size / 2;
            fxm_drawTexCylinder(&work->pos14, &rot, &sc, 0x4B0, 0x2BC, 0x300, 0x398, 0x80, 0x1C, 0x3D05, 0x80, 0x80,
                          -(work->frame4 * 0x180), 0x2000, 1, 1);
            fxm_drawTexCylinder(&work->pos14, &rot, &sc, 0x4B0, 0x2BC, 0x300, 0x400, 0x80, 0x1C, 0x3D05, 0x80, 0x80,
                          -(work->frame4 * 0x180) + 0x550, 0x2000, 1, 1);
            fxm_drawTexCylinder(&work->pos14, &rot, &sc, 0x4B0, 0x2BC, 0x300, 0x300, 0x80, 0x1C, 0x3D05, 0x80, 0x80,
                          -(work->frame4 * 0x180) + 0xAA0, 0x2000, 1, 1);
            fxm_drawTexCylinder(&work->pos14, &rot, &sc, 0x4B0, 0x2BC, 0x300, 0x398, 0x80, 0x1C, 0x3D05, 0x80, 0x80,
                          work->frame4 << 9, 0x2000, 1, -1);
            fxm_drawTexCylinder(&work->pos14, &rot, &sc, 0x4B0, 0x2BC, 0x300, 0x400, 0x80, 0x1C, 0x3D05, 0x80, 0x80,
                          (work->frame4 << 9) + 0x550, 0x2000, 1, -1);
            fxm_drawTexCylinder(&work->pos14, &rot, &sc, 0x4B0, 0x2BC, 0x300, 0x300, 0x80, 0x1C, 0x3D05, 0x80, 0x80,
                          (work->frame4 << 9) + 0xAA0, 0x2000, 1, -1);
            ex->btlfx_drawBackdrop((BtlBgLayers *)ex->btlfx_getFileData(entry->file, FX_METEOR_BACKDROP), work->frame4);
            if (frame >= 12) {
                SVECTOR dir;
                SVECTOR out;

                work->state0 = 3;
                entry->frame = -1;
                ex->btlfx_drawScreenTint((u32 *)(env->ot + 1), 1, 0xC8, 0xC8, 0x64);
                dir.vx = 0x4E0;
                dir.vy = 0x800;
                dir.vz = 0x700;
                for (i = 0, dist = 0; i < 64; i++, dist += 0x30) {
                    ex->btlfx_pointAhead((s16 *)&work->pos1C, (s16 *)&dir, dist, (s16 *)&out);
                    out.vx += ex->btlfx_randCentered(0x180);
                    out.vy += ex->btlfx_randCentered(0x180);
                    out.vz += ex->btlfx_randCentered(0x180);
                    BTLFX_COPY_SVEC(&fx_086_spritePos[i], &out);
                    fx_086_spritePos[i].pad = i >> 2;
                }
            }
            }
        }
            break;
        }
        case 3: {
        {
            SVECTOR spritePos;
            CVECTOR col;
            SVECTOR rot2;
            VECTOR sc2;
            s32 sx;
            s32 cs;
            s32 cs2;
            s32 burstAng;
            s32 burstT;
            s32 t;
            s32 bright;
            s32 ang;
            s32 size;
            s32 elapsed;
            s32 i;

            if (frame == 0) {
                ex->btlfx_drawScreenTint((u32 *)(env->ot + 1), 1, 0x64, 0x20, 0x20);
            }
            if (frame < 21) {
                env->camera->points[3].vx = work->pos1C.vx;
                env->camera->points[3].vy = work->pos1C.vy - 0x200;
                env->camera->points[3].vz = work->pos1C.vz;
                shake = ((0x1000 - (frame << 12) / 20) << 9) >> 12;
                env->camera->points[3].vy += ex->btlfx_randCentered(shake);
                env->camera->points[3].vz += ex->btlfx_randCentered(shake);
            }
            if (frame == 12) {
                emitter = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, FX_METEOR_EXPLOSION), (s32)&work->pos1C);
                if (emitter != NULL) {
                    emitter->otZ = 0x20;
                }
            }
            if (frame == 20) {
                emitter = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, FX_METEOR_SPRITES), (s32)&work->pos1C);
                if (emitter != NULL) {
                    emitter->otZ = 0x20;
                }
            }
            if (frame < 7) {
                burstT = (frame << 12) / 6;
                burstAng = burstT >> 2;
                size = (ex->math_cos(burstAng) + 0x800) * 2;
                bright = ex->math_cos(burstAng);
                scale.vx = size;
                scale.vy = size;
                scale.vz = size >> 1;
                pos.vx = work->pos1C.vx;
                pos.vy = work->pos1C.vy;
                pos.vz = work->pos1C.vz;
                rot.vx = -0x320;
                rot.vy = 0x6C0;
                rot.vz = 0x700;
                ex->Hi_DrawEffModel(&rot, &pos, (s32)&scale, work->model34);
                ex->Hi_ModifyEffModelRGB(work->model34, (bright >> 5) - 0x80, (bright >> 5) - 0x80, (bright >> 5) - 0x80);
                env->otZ = 0x80;
                ex->btlfx_drawRings((u8 *)ex->btlfx_getFileData(entry->file, FX_METEOR_ORANGE_RING), &work->pos1C, NULL,
                                  ((ex->math_sin(burstT >> 1) + 0x1000) * 3) >> 1, 0, 1, 0x1000);
                ex->btlfx_drawRings((u8 *)ex->btlfx_getFileData(entry->file, FX_METEOR_ORANGE_RING), &work->pos1C, NULL, 0x1C00, 0, 1,
                                  0x1000);
                env->otZ = 0;
            }
            if (frame < 8) {
                col.r = col.g = col.b = 0x46;
                sc2.vx = 0x1080;
                sc2.vy = 0x1080;
                sc2.vz = 0x1000;
                rot2.vx = rot2.vy = rot2.vz = 0;
                ex->btlfx_drawScreenGrid(1, 0xA0, 0x78, 0x20, 0x18, 0xA, 0xA, 5, 5, &rot2, &sc2, &col, 1, NULL);
            }
            if (frame < 13) {
                spriteAlpha = ex->math_cos((frame << 10) / 12);
                for (i = 0; i < 20; i++) {
                    fx_086_spritePos[i * 3].pad++;
                    BTLFX_COPY_SVEC(&spritePos, &fx_086_spritePos[i * 3]);
                    spritePos.pad = 0;
                    ex->btlfx_drawSprites((u8 *)ex->btlfx_getFileData(entry->file, FX_METEOR_DEBRIS_SPRITES), &spritePos, NULL, 0x3000,
                                          fx_086_spritePos[i * 3].pad, 1, spriteAlpha);
                }
            }
            if (frame >= 4) {
                elapsed = frame - 4;
                if (elapsed == 0) {
                    ex->btlfx_drawScreenTint((u32 *)(env->ot + 1), 2, 0xFF, 0xFF, 0xFF);
                } else if (elapsed < 24) {
                    bright = ex->math_cos((elapsed << 10) / 24) * 0xFF / 4096;
                    ex->btlfx_drawScreenTint((u32 *)(env->ot + 1), 1, bright, bright, bright >> 1);
                }
                t = elapsed << 7;
                if (elapsed < 32) {
                    ang = t >> 2;
                    size = ((ex->math_sin(ang) >> 1) + 0x1000) * 2 / 3;
                    bright = ex->math_cos(ang);
                    scale.vx = size;
                    scale.vy = size;
                    scale.vz = size;
                    pos.vx = work->pos1C.vx;
                    pos.vy = work->pos1C.vy;
                    pos.vz = work->pos1C.vz;
                    rot.vx = 0;
                    rot.vy = ex->math_sin(ang) >> 1;
                    rot.vz = 0;
                    if (elapsed < 3) {
                        bright = 0x1C00;
                    }
                    ex->Hi_DrawEffModel(&rot, &pos, (s32)&scale, work->model3C);
                    ex->Hi_ModifyEffModelRGB(work->model3C, ((bright * 2 - 0x1000) >> 5) - 0x80,
                                             ((bright * 2 - 0x1000) >> 5) - 0x80, ((bright * 2 - 0x1000) >> 5) - 0x80);
                    if (elapsed != 0) {
                        sx = scale.vx;
                        scale.vy = ex->math_cos(ang) + 0x800;
                        scale.vx = scale.vz = t * 3 / 4 + 0xC00;
                        pos.vy += -0x370 - elapsed * 24;
                        ex->Hi_DrawEffModel(&rot, &pos, (s32)&scale, work->model30);
                        ex->Hi_ModifyEffModelAbr(work->model30, 2);
                        cs = (bright >> 5) - 0x80;
                        ex->Hi_ModifyEffModelRGB(work->model30, cs, cs, cs);
                        pos.vy += elapsed * 24;
                        pos.vy += 0xB4 - elapsed * 20;
                        scale.vy = ex->math_cos(ang) + 0x1000;
                        scale.vx = scale.vz = t + 0x1000;
                        ex->Hi_DrawEffModel(&rot, &pos, (s32)&scale, work->model30);
                        ex->Hi_ModifyEffModelAbr(work->model30, 1);
                        ex->Hi_ModifyEffModelRGB(work->model30, cs, cs, cs);
                        pos.vy += 0x2BC + elapsed * 20;
                        cs2 = 0xA00; /* a variable: a literal 0xA00 gets folded into the size >> 2 term */
                        scale.vy = (size >> 2) + (((0x1000 - ex->math_cos(ang)) >> 1) + cs2);
                        scale.vx = scale.vz = sx + (0x1000 - ex->math_cos(ang)) / 3;
                        ex->Hi_DrawEffModel(&rot, &pos, (s32)&scale, work->model38);
                        ex->Hi_ModifyEffModelRGB(work->model38, cs, cs, cs);
                    }
                }
                if (elapsed < 41) {
                    t = (elapsed << 12) / 41;
                    pulseAng = t >> 2;
                    size = ((ex->math_sin(pulseAng) >> 1) + 0x1000) * 2 / 3;
                    glow = ex->math_cos(pulseAng);
                    scale.vx = size;
                    scale.vy = size;
                    scale.vz = size;
                    pos.vx = work->pos1C.vx;
                    pos.vy = work->pos1C.vy;
                    pos.vz = work->pos1C.vz;
                    rot.vx = 0;
                    rot.vy = ex->math_cos(pulseAng) >> 2;
                    rot.vz = 0;
                    ex->btlfx_drawScrollMove(work->frame4 * 8, 0x240, 0x100, 0x40, 0x80, 0x300, 0x100);
                    if (elapsed < 3) {
                        glow = 0x1C00;
                    }
                    ex->Hi_DrawEffModel(&rot, &pos, (s32)&scale, work->model2C);
                    ex->Hi_ModifyEffModelRGB(work->model2C, (glow >> 5) - 0x80, (glow >> 5) - 0x80, (glow >> 5) - 0x80);
                    env->otZ = 0x300;
                    ex->btlfx_drawRings((u8 *)ex->btlfx_getFileData(entry->file, FX_METEOR_ORANGE_RING), &work->pos1C, NULL,
                                      (ex->math_cos(pulseAng) + 0x1000) * 2, 0, 1, glow);
                    env->otZ = 0;
                }
            }
            if (frame >= 50) {
                ex->btlfx_clearEffModels();
                return 1;
            }
        }
            break;
        }
        }
        work->frame4++;
    }
    return 0;
}

INCLUDE_RODATA("asm/us/fx_086/nonmatchings/fx_086", fx_086_headerPtr);
