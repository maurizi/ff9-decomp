#include "fxmod/fx_1a8.h"

static void fx_1a8_op80ElemDraw0(s32 pt, s32 owner, SVECTOR *pos, SVECTOR *rot, s32 *scale, s32 alpha, s32 blend, s32 t, s32 n) {
    s32 r = (t << 12) / n;
    BtlfxExports *ex = BTLFX_EXPORTS(fx_1a8_env);
    VECTOR p;
    VECTOR s;
    s32 a;
    s32 c;

    s.vx = s.vy = s.vz = *scale >> 3;
    if (r >= 0xC00) {
        a = 0x1000 - (r - 0xC00) * 4;
    } else {
        a = 0x1000;
    }
    p.vx = pos->vx;
    p.vy = pos->vy;
    p.vz = pos->vz;
    ex->Hi_DrawEffModel(rot, &p, (s32)&s, fx_1a8_elemModels[0]);
    if (a < 0x1000) {
        ex->Hi_ModifyEffModelAbr(fx_1a8_elemModels[0], 1);
        c = (a >> 5) - 0x80;
        ex->Hi_ModifyEffModelRGB(fx_1a8_elemModels[0], c, c, c);
    }
}

static void fx_1a8_op80ElemDraw1(s32 pt, s32 owner, SVECTOR *pos, SVECTOR *rot, s32 *scale, s32 alpha, s32 blend, s32 t, s32 n) {
    s32 r = (t << 12) / n;
    BtlfxExports *ex = BTLFX_EXPORTS(fx_1a8_env);
    VECTOR p;
    VECTOR s;
    s32 a;
    s32 c;

    s.vx = s.vy = s.vz = *scale >> 3;
    if (r >= 0xC00) {
        a = 0x1000 - (r - 0xC00) * 4;
    } else {
        a = 0x1000;
    }
    p.vx = pos->vx;
    p.vy = pos->vy;
    p.vz = pos->vz;
    ex->Hi_DrawEffModel(rot, &p, (s32)&s, fx_1a8_elemModels[1]);
    if (a < 0x1000) {
        ex->Hi_ModifyEffModelAbr(fx_1a8_elemModels[1], 1);
        c = (a >> 5) - 0x80;
        ex->Hi_ModifyEffModelRGB(fx_1a8_elemModels[1], c, c, c);
    }
}

static void fxm_drawTexCylinder(SVECTOR *pos, SVECTOR *rot, VECTOR *scale, s32 r0, s32 r1, s32 w0, s32 w1, s32 bright, s32 tpage,
                   s32 clut, s32 u, s32 v, s32 ang, s32 dy, s32 abr, s32 dang) {
    Unk800F78A4 *env = fx_1a8_env;
    BtlfxExports *ex = BTLFX_EXPORTS(env);
    CVECTOR colFlat;
    CVECTOR colLit;
    CVECTOR colDark;
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
    s32 u0;
    s32 v0;
    s32 u1;
    s32 v1;
    s32 y0;
    s32 y1;
    s32 ratio0;
    s32 ratio1;
    s32 bulge0;
    s32 bulge1;
    s32 rad0;
    s32 rad1;
    s32 ang1;
    s32 ft;
    s32 nextJ;

    gte_SetRotMatrix(env->viewMatrix);
    gte_SetTransMatrix(env->viewMatrix);
    gte_ldv0(pos);
    gt4 = (POLY_GT4 *)env->prim;
    *(s32 *)&colDark = 0;
    ft4 = (POLY_FT4 *)(gt4 + 2);
    env->prim = (u8 *)(ft4 + 14);
    colFlat.r = colFlat.g = colFlat.b = bright;
    colLit.r = colLit.g = colLit.b = bright;
    if (abr != 0xFF) {
        colFlat.cd = 0x2E;
    } else {
        colFlat.cd = 0x2C;
    }
    colDark.cd = abr != 0xFF ? 0x3E : 0x3C;
    tpage |= (abr & 3) << 5;
    colLit.cd = colDark.cd;
    gte_rtv0tr_b();
    ex->math_rotMatrixYXZ(rot, &m);
    ex->ScaleMatrix(&m, scale);
    gte_stlvnl(m.t);
    ex->MulRotMatrix(&m);
    gte_SetRotMatrix(&m);
    gte_SetTransMatrix(&m);
    w0 >>= 1;
    w1 >>= 1;
    span = dy;
    u0 = u;
    u1 = u + 0x7F;
    v0 = v;
    v1 = v + 0x3F;
    seg = 0;
    for (rowY = 0, row = 0; row < 1; row++, rowY += dy) {
        for (j = 0; j < 16; j++, seg++, ang += dang << 8) {
            nextJ = (j + 1) << 8;
            y0 = rowY + (((j << 8) * dy) >> 12);
            y1 = rowY + ((nextJ * dy) >> 12);
            ratio0 = ((y0 + (span >> 1)) << 12) / span;
            ratio1 = ((y1 + (span >> 1)) << 12) / span;
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
            gte_nclip();
            if (seg == 0) {
                *(s32 *)&gt4->r0 = *(s32 *)&gt4->r1 = *(s32 *)&colDark;
                *(s32 *)&gt4->r2 = *(s32 *)&gt4->r3 = *(s32 *)&colLit;
                ft = 0;
            } else if (seg >= 15) {
                *(s32 *)&gt4->r2 = *(s32 *)&gt4->r3 = *(s32 *)&colDark;
                *(s32 *)&gt4->r0 = *(s32 *)&gt4->r1 = *(s32 *)&colLit;
                ft = 0;
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

s32 fx_1a8_op80(s32 mode, Fx1a8Work80 *work, Unk800F78A4 *env, Unk80100C78Entry *entry, s32 *args) {
    BtlfxExports *ex = BTLFX_EXPORTS(env);
    s32 alpha;
    s32 size;
    s32 frame;
    s32 t;

    if (mode == 0) {
        *args = 0x74;
    } else if (mode == 1) {
        work->state0 = 0;
        work->frame4 = 0;
        fx_1a8_env = env;
        ex->btlfx_initEffModels(fx_1a8_effModels, 0xC);
        work->model28 = ex->btlfx_registerEffectModel((Unk800E8F50 *)ex->btlfx_getFileData(entry->file, 0x19), 0);
        work->model4C[0] = ex->btlfx_registerEffectModel((Unk800E8F50 *)ex->btlfx_getFileData(entry->file, 0x10), 0);
        work->model4C[1] = ex->btlfx_registerEffectModel((Unk800E8F50 *)ex->btlfx_getFileData(entry->file, 7), 0);
        work->model4C[2] = ex->btlfx_registerEffectModel((Unk800E8F50 *)ex->btlfx_getFileData(entry->file, 8), 0);
        work->model4C[3] = ex->btlfx_registerEffectModel((Unk800E8F50 *)ex->btlfx_getFileData(entry->file, 9), 0);
        work->model5C = ex->btlfx_registerEffectModel((Unk800E8F50 *)ex->btlfx_getFileData(entry->file, 0x15), 2);
        work->model64 = ex->btlfx_registerEffectModel((Unk800E8F50 *)ex->btlfx_getFileData(entry->file, 0x11), 1);
        fx_1a8_elemModels[0] = ex->btlfx_registerEffectModel((Unk800E8F50 *)ex->btlfx_getFileData(entry->file, 0xA), 0);
        fx_1a8_elemModels[1] = ex->btlfx_registerEffectModel((Unk800E8F50 *)ex->btlfx_getFileData(entry->file, 6), 0);
        work->model60 = ex->btlfx_registerEffectModel((Unk800E8F50 *)ex->btlfx_getFileData(entry->file, 0x16), 3);
        work->model68 = ex->btlfx_registerEffectModel((Unk800E8F50 *)ex->btlfx_getFileData(entry->file, 0x14), 1);
        work->pos8.vx = fx_1a8_targetPos.vx;
        work->pos8.vy = fx_1a8_targetPos.vy - 0x2000;
        work->pos8.vz = fx_1a8_targetPos.vz - 0x2000;
        env->camera->points[3].vx = work->pos8.vx;
        env->camera->points[3].vy = work->pos8.vy;
        env->camera->points[3].vz = work->pos8.vz;
    } else {
        alpha = 0;
        size = 0;
        frame = entry->frame;
        switch (work->state0) {
        case 0:
            t = (frame << 12) / 10;
            if (frame == 0) {
                ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, 1), (s32)&work->pos8);
            }
            if (frame < 0x10) {
                ex->btlfx_drawTrail((u8 *)ex->btlfx_getFileData(entry->file, 2), (s32)&work->pos8, NULL, 0, 0x555,
                                    ex->math_cos((frame << 8) >> 2), 1, fx_1a8_trailPoints, 0);
            }
            if (frame == 0) {
                work->rot2C.vx = 0x600;
                work->rot2C.vy = 0;
                work->rot2C.vz = 0;
            }
            work->pos40.vx = work->pos8.vx;
            work->pos40.vy = work->pos8.vy;
            work->pos40.vz = work->pos8.vz;
            work->pos40.vy = work->pos40.vy - (ex->math_cos(t >> 2) - 0x1000);
            work->pos40.vz = work->pos40.vz - (ex->math_cos(t >> 2) - 0x1000);
            work->rot2C.vz += 0x80;
            work->scale34.vx = work->scale34.vy = work->scale34.vz = ((((0x1000 - ex->math_cos(t >> 2)) << 2) + 0x100) * 6) >> 2;
            ex->Hi_DrawEffModel(&work->rot2C, (VECTOR *)&work->pos40, (s32)&work->scale34, work->model28);
            work->scale34.vx = work->scale34.vy = work->scale34.vz = work->scale34.vx / 3;
            ex->Hi_DrawEffModel(&work->rot2C, (VECTOR *)&work->pos40, (s32)&work->scale34, work->model60);
            work->scale34.vx *= 2;
            work->scale34.vy = work->scale34.vz = work->scale34.vx;
            size = work->scale34.vx;
            alpha = 0x1000;
            work->pos20.vx = work->pos40.vx;
            work->pos20.vy = work->pos40.vy;
            work->pos20.vz = work->pos40.vz;
            if (frame >= 10) {
                work->state0 = 1;
                entry->frame = -1;
                ex->btlfx_drawBgOffscreen(0x240, 0x1C0, 0x40, 0x40, fx_1a8_bgOt, fx_1a8_targetPos.vx, fx_1a8_targetPos.vz, 0x800);
            }
            break;
        case 1: {
            SVECTOR v;

            size = work->scale34.vx;
            work->pos40.vx = fx_1a8_targetPos.vx;
            work->pos40.vy = fx_1a8_targetPos.vy;
            alpha = 0x1000;
            work->pos40.vz = fx_1a8_targetPos.vz;
            t = ((frame + 1) << 12) / 5;
            work->pos40.vy = work->pos40.vy + (t - 0x1000);
            work->pos40.vz = work->pos40.vz + (t - 0x1000);
            work->rot2C.vz += 0x80;
            work->pos20.vx = work->pos40.vx;
            work->pos20.vy = work->pos40.vy;
            work->pos20.vz = work->pos40.vz;
            if (frame == 0) {
                v.vx = work->pos40.vx;
                v.vy = work->pos40.vy;
                v.vz = work->pos40.vz;
                ex->btlfx_aimAngles((s16 *)&v, (s16 *)&fx_1a8_targetPos, (s16 *)&work->rot2C);
                work->scale34.vx = work->scale34.vy = work->scale34.vz = 0x5000;
                work->rot2C.vx += 0x800;
                env->camera->points[3].vx = fx_1a8_targetPos.vx;
                env->camera->points[3].vy = fx_1a8_targetPos.vy;
                env->camera->points[3].vz = fx_1a8_targetPos.vz;
                env->camera->points[3].vy -= 0x140;
            }
            ex->Hi_DrawEffModel(&work->rot2C, (VECTOR *)&work->pos40, (s32)&work->scale34, work->model28);
            work->scale34.vx = work->scale34.vy = work->scale34.vz = work->scale34.vx / 2;
            ex->Hi_DrawEffModel(&work->rot2C, (VECTOR *)&work->pos40, (s32)&work->scale34, work->model60);
            work->scale34.vx = work->scale34.vy = work->scale34.vz = work->scale34.vx * 2;
            if (frame >= 4) {
                Pool6CEntry *p;

                work->state0 = 2;
                entry->frame = -1;
                ex->btlfx_drawScreenTint((u32 *)&env->ot[1], 2, 0x46, 0x46, 0x46);
                BTLFX_COPY_SVEC(&work->pos10, &fx_1a8_targetPos);
                ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, 5), (s32)&work->pos10);
                p = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, 0xD), (s32)&work->pos10);
                if (p != NULL) {
                    p->otZ = 0x200;
                }
                p = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, 0xC), (s32)&work->pos10);
                if (p != NULL) {
                    p->otZ = 0x80;
                }
                p = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, 0x12), (s32)&work->pos10);
                if (p != NULL) {
                    p->otZ = 0x80;
                }
                p = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, 0x17), (s32)&work->pos10);
                if (p != NULL) {
                    p->elems[0].draw = (void (*)())fx_1a8_op80ElemDraw0;
                    p->elems[1].draw = (void (*)())fx_1a8_op80ElemDraw1;
                }
            }
            break;
        }
        case 2: {
            SVECTOR r1;
            VECTOR s1;
            SVECTOR r2;
            VECTOR s2;
            SVECTOR r3;
            VECTOR s3;
            SVECTOR r4;
            VECTOR s4;
            SVECTOR r5;
            s32 d;
            s32 i;
            s32 at;
            s32 q1;
            s32 a1;
            s32 q2;
            s32 c2;
            s32 a2;
            s32 q3;
            s32 a3;
            s32 y3;
            s32 p3;
            s32 sv;
            s32 a4;
            s32 x4;

            t = frame << 7;
            if (frame < 8) {
                env->camera->points[3].vx = fx_1a8_targetPos.vx;
                d = ((frame & 1) * 2 - 1) * (0x20 - frame * 4);
                env->camera->points[3].vy = fx_1a8_targetPos.vy;
                env->camera->points[3].vz = fx_1a8_targetPos.vz;
                env->camera->points[3].vy = env->camera->points[3].vy - (0x140 + d);
                env->camera->points[3].vx += d;
            }
            work->pos40.vx = fx_1a8_targetPos.vx;
            work->pos40.vy = fx_1a8_targetPos.vy;
            work->pos40.vz = fx_1a8_targetPos.vz;
            if (frame < 6) {
                work->pos40.vy = (((frame & 1) * 2 - 1) * ex->math_cos((frame << 10) / 6)) >> 6;
            }
            ex->Hi_DrawEffModel(&work->rot2C, (VECTOR *)&work->pos40, (s32)&work->scale34, work->model28);
            if (t > 0xC00) {
                s32 k;

                ex->Hi_ModifyEffModelAbr(work->model28, 1);
                k = ((0x1000 - (t - 0xC00) * 4) >> 5) - 0x80;
                ex->Hi_ModifyEffModelRGB(work->model28, k, k, k);
            }
            if (frame < 0x10) {
                alpha = 0x1000 - (frame << 8);
                work->pos20.vx = work->pos40.vx;
                work->pos20.vy = work->pos40.vy;
                size = work->scale34.vx + t;
                work->pos20.vz = work->pos40.vz;
            }
            if (frame < 6) {
                at = 0xFF - (frame * 0xFF) / 6;
                ex->btlfx_drawScreenTint((u32 *)&env->ot[1], 1, at, at >> 1, at >> 1);
            }
            if (frame < 0x10) {
                work->pos40.vx = fx_1a8_targetPos.vx;
                work->pos40.vy = fx_1a8_targetPos.vy;
                work->pos40.vz = fx_1a8_targetPos.vz;
                s1.vx = s1.vz = 0x1800;
                r1.vx = 0;
                r1.vy = 0;
                r1.vz = 0;
                q1 = (frame << 12) / 12;
                s1.vy = q1 + 0x2000;
                a1 = (ex->math_cos(q1 >> 2) >> 5) - 0x80;
                for (i = 0; i < 4; i++) {
                    ex->Hi_DrawEffModel(&r1, (VECTOR *)&work->pos40, (s32)&s1, work->model4C[i]);
                    ex->Hi_ModifyEffModelRGB(work->model4C[i], a1, a1, a1);
                }
                ex->btlfx_drawScrollMove(work->frame4 << 4, 0x1C0, 0x100, 0x38, 0x70, 0x1C0, 0x170);
            }
            if (frame < 0xC) {
                work->pos40.vx = fx_1a8_targetPos.vx;
                work->pos40.vy = fx_1a8_targetPos.vy;
                work->pos40.vz = fx_1a8_targetPos.vz;
                q2 = (frame << 12) / 12;
                s2.vx = s2.vz = q2 / 8 + 0x555;
                s2.vy = ex->math_sin(q2 >> 2);
                r2.vx = 0;
                r2.vy = 0;
                r2.vz = 0;
                c2 = ex->math_cos(q2 >> 2);
                ex->Hi_DrawEffModel(&r2, (VECTOR *)&work->pos40, (s32)&s2, work->model64);
                a2 = (c2 >> 5) - 0x80;
                ex->Hi_ModifyEffModelRGB(work->model64, a2, a2, a2);
                s2.vy >>= 1;
                s2.vx = s2.vz = (s2.vx * 4) / 3;
                ex->Hi_DrawEffModel(&r2, (VECTOR *)&work->pos40, (s32)&s2, work->model64);
                ex->Hi_ModifyEffModelRGB(work->model64, a2, a2, a2);
            }
            if (frame < 0x14) {
                work->pos40.vx = fx_1a8_targetPos.vx;
                work->pos40.vy = fx_1a8_targetPos.vy;
                work->pos40.vz = fx_1a8_targetPos.vz;
                q3 = (frame << 12) / 20;
                p3 = q3 >> 2;
                s3.vx = s3.vz = ex->math_sin(p3) / 2;
                a3 = ((0x1000 - q3) >> 5) - 0x80;
                s3.vy = 0x800 - p3;
                r3.vx = 0;
                r3.vy = frame * 0x18;
                r3.vz = 0;
                y3 = frame * 0x18;
                work->pos40.vy -= y3;
                for (i = 0; i < 4; i++) {
                    ex->Hi_DrawEffModel(&r3, (VECTOR *)&work->pos40, (s32)&s3, work->model5C);
                    ex->Hi_ModifyEffModelRGB(work->model5C, a3, a3, a3);
                    s3.vx = s3.vz = (s3.vx * 5) >> 2;
                    s3.vy = (s3.vy * 3) >> 2;
                    work->pos40.vy -= y3;
                    r3.vy = (r3.vy * 9) >> 3;
                }
            }
            work->pos40.vx = fx_1a8_targetPos.vx;
            work->pos40.vy = fx_1a8_targetPos.vy;
            work->pos40.vz = fx_1a8_targetPos.vz;
            s4.vx = s4.vz = ex->math_sin(t >> 2) / 2 + 0x1800;
            s4.vy = 0x1000;
            r4.vx = 0;
            r4.vy = frame * 0x18;
            r4.vz = 0;
            x4 = 0x1000 - t;
            ex->btlfx_drawScrollMove(10000 - (work->frame4 << 3), 0x240, 0x140, 0x20, 0x80, 0x260, 0x140);
            ex->Hi_DrawEffModel(&r4, (VECTOR *)&work->pos40, (s32)&s4, work->model68);
            a4 = (x4 >> 5) - 0x80;
            ex->Hi_ModifyEffModelRGB(work->model68, a4, a4, a4);
            if (frame < 0x10) {
                r5.vx = 0;
                r5.vy = 0;
                r5.vz = 0;
                r5.pad = 0;
                env->otZ = 0x200;
                ex->btlfx_drawRings((u8 *)ex->btlfx_getFileData(entry->file, 0xB), &work->pos10, &r5,
                                  ex->math_sin((frame << 8) >> 2), 0, 1, ex->math_cos((frame << 8) >> 2));
                env->otZ = 0;
            }
            if (frame == 0xC) {
                Pool6CEntry *p;

                p = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, 0x13), (s32)&work->pos10);
                if (p != NULL) {
                    p->otZ = 0x190;
                }
            }
            if (frame >= 0x20) {
                ex->btlfx_clearEffModels();
                return 1;
            }
            break;
        }
        }
        if (alpha != 0) {
            VECTOR sc;
            s32 c;

            sc.vx = sc.vy = sc.vz = size;
            c = alpha >> 5;
            fxm_drawTexCylinder(&work->pos20, &work->rot2C, &sc, 0x100, 0xA0, 0x200, 0x200, c, 0x19, 0x3D02, 0, 0,
                          work->frame4 << 8, 0xC00, 1, 1);
            fxm_drawTexCylinder(&work->pos20, &work->rot2C, &sc, 0x100, 0xA0, 0x100, 0x100, c, 0x19, 0x3D02, 0, 0,
                          (work->frame4 << 8) + 0x800, 0xC00, 1, 1);
            fxm_drawTexCylinder(&work->pos20, &work->rot2C, &sc, 0x100, 0xA0, 0x200, 0x200, c, 0x19, 0x3D02, 0, 0,
                          -work->frame4 << 8, 0xC00, 1, -1);
            fxm_drawTexCylinder(&work->pos20, &work->rot2C, &sc, 0x100, 0xA0, 0x100, 0x100, c, 0x19, 0x3D02, 0, 0,
                          (-work->frame4 << 8) - 0x800, 0xC00, 1, -1);
            ex->btlfx_drawScrollMove(10000 - (work->frame4 << 4), 0x240, 0x100, 0x20, 0x40, 0x260, 0x100);
        }
        work->frame4++;
    }
    return 0;
}

INCLUDE_RODATA("asm/us/fx_1a8/nonmatchings/fx_1a8", fx_1a8_headerPtr);
