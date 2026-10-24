#include "fxmod/fx_17d_0.h"

void func_801E7704(u32 *ot, s32 abr, s32 h, s32 r, s32 g, s32 b) {
    POLY_G4 *p;
    BtlfxExports *ex;
    s32 i;
    s32 x;

    h >>= 1;
    p = (POLY_G4 *)D_801E9470->prim;
    ex = BTLFX_EXPORTS(D_801E9470);
    D_801E9470->prim = (u8 *)(p + 4);
    for (i = 0; i < 4; i++, p++) {
        setlen(p, 8);
        setcode(p, 0x3A);
        if (!(i & 1)) {
            p->r0 = p->r1 = 0;
            p->g0 = p->g1 = 0;
            p->b0 = p->b1 = b;
            p->r2 = p->r3 = r;
            p->g2 = p->g3 = g;
            p->b2 = p->b3 = b;
            p->y0 = p->y1 = 0x70 - h;
            p->y2 = p->y3 = 0x70;
        } else {
            p->r2 = p->r3 = 0;
            p->g2 = p->g3 = 0;
            p->b2 = p->b3 = b;
            p->r0 = p->r1 = r;
            p->g0 = p->g1 = g;
            p->b0 = p->b1 = b;
            p->y0 = p->y1 = 0x70;
            p->y2 = p->y3 = 0x70 + h;
        }
        x = (i >> 1) * 160;
        p->x0 = p->x2 = x;
        p->x1 = p->x3 = x + 160;
        ex->btlfx_linkPrimAbr(0x08000000, ot, p, abr);
    }
}

void func_801E7894(s32 n, s32 r, SVECTOR *pts) {
    SVECTOR *q;
    u32 *col = (u32 *)(pts + n);
    BtlfxExports *ex = BTLFX_EXPORTS(D_801E9470);
    s32 i;
    s32 rad;
    s32 a;

    for (i = 0, q = pts; i < n; i++, col++, q++, pts++) {
        *col = 0x7F7F7F7F;
        rad = ex->btlfx_randRange(r >> 2, r >> 1);
        a = ex->btlfx_rand() & 0xFFF;
        pts->vx = (ex->math_cos(a) * rad) >> 12;
        q->vy = (ex->math_sin(a) * rad) >> 12;
        q->vz = ex->btlfx_rand() % 640 - 0x80;
    }
}

void func_801E79E8(u32 *ot, s32 n, s32 h, s32 dz, SVECTOR *rot, SVECTOR *pts, CVECTOR *rgb, s32 p, s32 abr) {
    Unk800F78A4 *env = D_801E9470;
    BtlfxExports *ex = BTLFX_EXPORTS(env);
    LINE_G2 *prim;
    CVECTOR c;
    MATRIX m;
    s32 oldh;
    u32 z;
    u32 *last;
    SVECTOR *q;
    SVECTOR *v;
    LINE_G2 *pp;
    s32 flag;
    s32 i;

    if (p < 0) {
        p = 0x1000;
    }
    prim = (LINE_G2 *)env->prim;
    env->prim = (u8 *)(prim + n);
    if (rgb == NULL) {
        c.b = 0;
        c.g = 0;
        c.r = 0;
    } else {
        gte_DpqColorFar_nst(rgb, p);
        gte_strgb(&c);
    }
    gte_SetRotMatrix(env->viewMatrix);
    gte_SetTransMatrix(env->viewMatrix);
    gte_ReadGeomScreen(&oldh);
    gte_SetGeomScreen(h);
    ex->math_rotMatrixYXZ(rot, &m);
    m.t[0] = m.t[1] = 0;
    m.t[2] = h;
    gte_SetRotMatrix(&m);
    gte_SetTransMatrix(&m);
    v = pts;
    last = (u32 *)(v + n);
    for (i = 0, q = v, pp = prim; i < n; i++, last++, q++, v++, pp++, prim++) {
        flag = *last != 0x7F7F7F7F;
        gte_ldv0(v);
        gte_rtps();
        q->vz -= dz;
        if (dz > 0 && q->vz < -0x80) {
            q->vz = 0x200;
            *last = 0x7F7F7F7F;
        } else if (dz < 0 && q->vz > 0x180) {
            q->vz = -0x100;
            *last = 0x7F7F7F7F;
        } else {
            gte_stsxy(&pp->x0);
            gte_stszotz(&z);
            if (z < 0x80 && flag) {
                gte_DpqColorFar_nst(&c, ((0x80 - z) << 12) >> 7);
                gte_strgb(&prim->r0);
                *(s32 *)&pp->x1 = *last;
                *(s32 *)&pp->r1 = 0;
                pp->code = abr == 0xFF ? 0x50 : 0x52;
                ex->btlfx_linkPrimAbr(0x04000000, ot, prim, abr);
            }
            *last = *(s32 *)&pp->x0;
        }
    }
    gte_SetGeomScreen(oldh);
}

void func_801E7D18(Unk80100C78Entry *entry, SVECTOR *origin, s32 frame, s32 models, s16 *morphs, s32 scale) {
    BtlfxExports *ex = BTLFX_EXPORTS(D_801E9470);
    s16 *morph = morphs; /* a copy: the original loads scale before morphs */
    VECTOR pos;
    VECTOR sc;
    SVECTOR rot;
    s16 *f;
    s32 n;
    s32 i;
    s32 j;
    s32 k;

    for (i = 0; i < 21; i++) {
        j = i * 4 + 2;
        f = (s16 *)ex->btlfx_getFileData(entry->file, ((s16 *)D_801E9370)[j]);
        n = frame;
        if (n >= f[1]) {
            n = f[1] - 1;
        }
        if (i < 16 || i >= 19 || frame < 0x28) {
            if (i != 16) {
                f = f + 2 + n * 6;
                sc.vx = sc.vy = sc.vz = scale;
            } else {
                f = f + 2 + n * 9;
                sc.vx = (f[6] * scale) >> 12;
                sc.vy = (f[7] * scale) >> 12;
                sc.vz = (f[8] * scale) >> 12;
            }
            pos.vx = ((f[0] * scale) >> 12) + origin->vx;
            pos.vy = ((f[1] * scale) >> 12) + origin->vy;
            pos.vz = ((f[2] * scale) >> 12) + origin->vz;
            rot.vx = f[3];
            rot.vy = f[4];
            rot.vz = -f[5];
            k = i << 1;
            if (*morph < 0) {
                ex->Hi_DrawEffModel(&rot, &pos, (s32)&sc, *(s16 *)(k + models));
            } else {
                j = i * 4 + 3;
                f = (s16 *)ex->btlfx_getFileData(entry->file, ((s16 *)D_801E9370)[j]);
                n = frame;
                if (n >= f[1]) {
                    n = f[1] - 1;
                }
                ex->Hi_DrawMorphEffModel(&rot, &pos, (s32)&sc, *(s16 *)(k + models), *morph, *(f = f + 2 + n * 3));
            }
        }
        morph++;
    }
}

s32 func_801E7FE0(s32 mode, Fx17d0Part *p, Unk800F78A4 *env, Unk80100C78Entry *entry, s32 frame) {
    BtlfxExports *ex = BTLFX_EXPORTS(env);

    if (mode == 1) {
        p->unk0 = 0;
    } else {
        p->unk8.vz += p->unk2 + 0x186;
        env->unk28 = -0x80;
        if (frame == 0) {
            ex->btlfx_drawTrail((u8 *)ex->btlfx_getFileData(entry->file, 0x22), (s32)&p->unk8, 0, -1, 0x800, 0x1000, 1,
                              D_801EBC90[p->unk4], 0);
        }
        ex->btlfx_drawTrail((u8 *)ex->btlfx_getFileData(entry->file, 0x22), (s32)&p->unk8, 0, 0, 0x800, 0x1000, 1,
                          D_801EBC90[p->unk4], 0);
        env->unk28 = 0;
        if (frame >= 0x10) {
            return 1;
        }
    }
    return 0;
}

s32 fx_17d_0_op80(s32 mode, Fx17d0Work80 *work, Unk800F78A4 *env, Unk80100C78Entry *entry, EffInit *arg4) {
    BtlfxExports *ex = BTLFX_EXPORTS(env);
    s32 sx;
    s32 code;
    s32 abr;
    s32 alpha;
    s32 frame;
    s32 c;
    s32 t;
    s32 i;
    s32 i0;
    s32 i3;
    s32 o;

    if (mode == 0) {
        arg4->unk0 = 0xBC;
        arg4->unk8 = 0x10;
        arg4->unk14 = (s32)func_801E7FE0;
        arg4->unk4 = 0x20;
    } else if (mode == 1) {
        work->unk0 = 0xA;
        work->unk4 = 0;
        D_801E9470 = env;
        ex->btlfx_initEffModels(D_801EA380, 0x20);
        work->unkBA = ex->btlfx_registerEffectModel((Unk800E8F50 *)ex->btlfx_getFileData(entry->file, 0x1C), 0);
        BTLFX_COPY_SVEC(&work->unk8, &D_801E9368);
        work->unk18.vy = -0x1000;
        work->unk18.vx = 0;
        work->unk18.vz = 0;
        work->unk30.vx = 0;
        work->unk30.vy = 0;
        work->unk30.vz = 0;
        D_801EBC80.vx = 0x1000;
        D_801EBC80.vy = 0x1000;
        D_801EBC80.vz = 0x1000;
        BTLFX_COPY_SVEC(&work->unk48, &work->unk8);
        work->unk48.vy -= 0x180;
        env->camera->points[3].vx = work->unk48.vx;
        env->camera->points[3].vy = work->unk48.vy;
        env->camera->points[3].vz = work->unk48.vz;
        func_801E7894(0x140, 0x200, D_801E9480);
        work->unk58 = 0;
    } else {
        sx = 0;
        code = 0;
        abr = 0;
        alpha = 0;
        frame = entry->frame;
        c = 0;
        switch (work->unk0) {
            SVECTOR rot;

        case 0xA: {
            CVECTOR col;
            s32 h;

            t = frame << 7;
            col.r = 0x80;
            col.g = 0xC8;
            col.b = 0xFF;
            h = t >> 1;
            rot.vx = ((0x1000 - ex->math_cos(h)) >> 1) / 3;
            rot.vy = ex->math_sin(t >> 2) >> 2;
            rot.vz = work->unk4 * 4;
            work->unk38.vx = rot.vx >> 1;
            work->unk38.vy = rot.vy >> 1;
            work->unk38.vz = rot.vz >> 1;
            func_801E79E8((u32 *)env->ot + 8, 0x140, 0x80, ((ex->math_sin(h) * 0x18) >> 12) - 1, &rot, D_801E9480,
                          &col, t, 1);
            if (frame >= 0x19) {
                code = 1;
                abr = 1;
                c = (frame - 0x18) * 12;
                sx = 0;
            }
            if (frame >= 0x20) {
                work->unk0 = 0;
                entry->frame = -1;
            }
            break;
        }
        case 0: {
            CVECTOR col;
            s32 q;
            s32 h;
            s32 a;
            s32 b;

            t = (frame << 12) / 80;
            q = t >> 2;
            alpha = ex->math_sin(q);
            col.r = 0x80;
            col.g = 0xC8;
            col.b = 0xFF;
            h = t >> 1;
            rot.vx = (0x1000 - ((0x1000 - ex->math_cos(h)) >> 1)) / 3;
            sx = t >> 7;
            rot.vy = (0x1000 - ex->math_sin(q)) >> 2;
            rot.vz = work->unk4 * 4;
            work->unk38.vx = rot.vx >> 1;
            work->unk38.vy = rot.vy >> 1;
            work->unk38.vz = rot.vz >> 1;
            func_801E79E8((u32 *)env->ot + 8, 0x140, 0x80, ~((t * 20) >> 12), &rot, D_801E9480, &col, 0x1000, 1);
            code = 1;
            abr = 1;
            a = ((0x1000 - ex->math_sin(q)) >> 5) + 0xA0;
            b = 0x6E - ((0x1000 - ((0x1000 - ex->math_cos(h)) >> 1)) >> 3);
            ex->btlfx_drawSpriteAtOt(0xC00, (u8 *)ex->btlfx_getFileData(entry->file, 0x1D), a, b, 0, 0xFF, t);
            c = 0x60 - sx;
            if (frame >= 0x50) {
                work->unk0 = code;
                entry->frame = -1;
                for (i0 = 0; i0 < 21; i0++) {
                    s32 k;
                    s32 v;

                    work->unk5C[i0] = ex->btlfx_registerEffectModel(
                        (Unk800E8F50 *)ex->btlfx_getFileData(entry->file, ((s16 *)D_801E9370)[i0 * 4]), 0);
                    o = i0 * 8 + 2; /* byte offset: a separate 8i + 2 giv added to the table base */
                    k = *(s16 *)((u8 *)D_801E9370 + o);
                    v = -1;
                    if (k >= 0) {
                        v = ex->btlfx_registerEffectModel((Unk800E8F50 *)ex->btlfx_getFileData(entry->file, k), 0);
                    }
                    work->unk86[i0] = v;
                }
            }
            break;
        }
        case 1: {
            SVECTOR rot;
            CVECTOR col;
            s32 q;
            s32 b;
            s32 d;
            s32 e;

            t = (frame << 12) / 88;
            t = 0x1000 - ex->math_cos(t >> 2);
            if (frame == 0) {
                Pool6CEntry *p;

                BTLFX_COPY_SVEC(&work->unk40, &work->unk8);
                work->unk40.vy -= 0x140;
                work->unk40.vz += 0x1000;
                p = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, 0x23), (s32)&work->unk40);
                if (p != NULL) {
                    p->unk22 = -0x100;
                }
            }
            q = t >> 2;
            alpha = 0x1000;
            work->unk38.vx -= 1;
            work->unk38.vy += 1;
            /* mode (dead here) holds the intermediate: as a global pseudo with an a0 preference it gets a0 */
            mode = 0x1000 - ex->math_cos((0x1000 - ex->math_cos(q)) >> 2);
            b = mode >> 2;
            work->unk10.vx = 0;
            work->unk10.vy = 0x200 - ((ex->math_cos(b) * 0x300) >> 12);
            work->unk10.vz = ex->math_cos(b);
            col.r = 0x80;
            col.g = 0xC8;
            col.b = 0xFF;
            rot.vx = 0;
            rot.vy = 0;
            rot.vz = work->unk4 * 4;
            d = (t >> 8) + 0x14;
            func_801E79E8((u32 *)env->ot + 0x400, 0x140, 0x80, -d, &rot, D_801E9480, &col,
                          (ex->math_cos(q) >> 1) + 0x800, 1);
            b = 0x1000 - ex->math_cos(q);
            b = 0x1000 - ex->math_cos(b >> 2);
            if (t > 0xC00) {
                b = b - (ex->math_cos(t - 0xC00) - 0x1000);
            }
            func_801E7D18(entry, &work->unk10, frame % 10, (s32)work->unk5C, work->unk86, b);
            if (frame < 0x58) {
                e = (frame >> 2) + 0x6E;
                ex->btlfx_drawSpriteAtOt(0xC00, (u8 *)ex->btlfx_getFileData(entry->file, 0x1D), 0xA0, e, 0, 0xFF, alpha);
            }
            code = 1;
            abr = 1;
            sx = (t >> 4) + 0x20;
            c = 0x40;
            if (frame >= 0x58) {
                u16 r0[2][2];
                u16 r1[2][2];

                work->unk0 = 2;
                entry->frame = -1;
                work->unk10.vy = 0;
                BTLFX_COPY_SVEC(&work->unk8, &work->unk18);
                for (i = 0; i < 2; i++) {
                    r0[i][0] = ((Fx17d0Summon *)env->loader)->unk54->unk18[D_801E9424[i]];
                    r0[i][1] = ((Fx17d0Summon *)env->loader)->unk54->unk24[D_801E9424[i]];
                    r1[i][0] = 0;
                    r1[i][1] = ((Fx17d0Summon *)env->loader)->unk54->unk30[D_801E9424[i]];
                }
                for (i = 0; i < 5; i++) {
                    work->unkB0[i] = ((s32 (*)(void *, void *, void *))ex->Hi_RegisterTexListModel)(
                        (void *)ex->btlfx_getFileData(entry->file, D_801E9418[i]), r0, r1);
                }
            }
            break;
        }
        case 2: {
            VECTOR pos;
            SVECTOR rot;
            s16 *f;
            s32 n;

            alpha = 0x1000;
            work->unk38.vx += 0xC;
            work->unk38.vy += 6;
            func_801E7D18(entry, &work->unk10, frame, (s32)work->unk5C, work->unk86, 0x1000);
            if (frame >= 0x79) {
                s32 a;
                s32 b;
                s32 m;

                f = (s16 *)ex->btlfx_getFileData(entry->file, 0x1E);
                n = frame;
                if (n >= f[1]) {
                    n = f[1] - 1;
                }
                f = f + 2 + n * 6;
                pos.vx = f[0];
                pos.vy = f[1];
                pos.vz = f[2];
                rot.vx = f[3];
                rot.vy = f[4];
                rot.vz = -f[5];
                f = (s16 *)ex->btlfx_getFileData(entry->file, 0x1B);
                if (frame >= f[1]) {
                    f = (s16 *)ex->btlfx_getFileData(entry->file, 0x20);
                    n = frame - (f[1] - 1);
                    if (n >= f[1]) {
                        n = f[1] - 1;
                    }
                    n = 0x1000 - *(f += 2 + n * 3);
                    ex->Hi_DrawMorphEffModel(&rot, &pos, (s32)&D_801EBC80, work->unkB0[1], work->unkB0[2], n);
                } else {
                    n = *(f += 2 + frame * 3);
                    ex->Hi_DrawMorphEffModel(&rot, &pos, (s32)&D_801EBC80, work->unkB0[0], work->unkB0[1], n);
                }
            }
            if (frame >= 0x8E) {
                f = (s16 *)ex->btlfx_getFileData(entry->file, 0x1F);
                n = frame;
                if (n >= f[1]) {
                    n = f[1] - 1;
                }
                f = f + 2 + n * 6;
                pos.vx = f[0];
                pos.vy = f[1];
                pos.vz = f[2];
                rot.vx = f[3];
                rot.vy = f[4];
                rot.vz = -f[5];
                f = (s16 *)ex->btlfx_getFileData(entry->file, 0x21);
                n = frame;
                if (n >= f[1]) {
                    n = f[1] - 1;
                }
                n = *(f += 2 + n * 3);
                ex->Hi_DrawMorphEffModel(&rot, &pos, (s32)&D_801EBC80, work->unkB0[3], work->unkB0[4], n);
            }
            if (frame < 0x9D) {
                for (i3 = 0; i3 < 2; i3++) {
                    Fx17d0Part *p = ex->btlfx_poolSpawn((s16 *)entry->pool);

                    if (p != NULL) {
                        s32 a;
                        s32 r;

                        BTLFX_COPY_SVEC(&p->unk8, &work->unk18);
                        a = ex->btlfx_rand();
                        r = (ex->btlfx_rand() & 0x7F) + 0x400;
                        p->unk8.vz -= 0x5DC;
                        p->unk8.vx += (ex->math_cos(a) * r) >> 12;
                        p->unk8.vy = p->unk8.vy + (((ex->math_sin(a) * r) >> 12) - 0x19A);
                        p->unk2 = ex->btlfx_rand() & 0x7F;
                        p->unk4 = work->unk58 & 0x1F;
                        work->unk58++;
                    }
                }
            }
            if (frame >= 0xA1) {
                func_801E7704((u32 *)env->ot + 1, 1,
                              ((0x1000 - ex->math_cos((((frame - 0xA1) << 12) / 7) >> 2)) * 0xE0) >> 12, 0xF0, 0xF0,
                              0xF0);
            }
            if (frame >= 0xA3) {
                s32 k = ((frame - 0xA3) * 0xFF) / 6;

                ex->btlfx_drawScreenTint((u32 *)env->ot + 1, 1, k, k, k);
            }
            if (frame >= 0xA9) {
                ex->btlfx_poolClear((Unk800DA1B8 *)entry->pool);
                ex->btlfx_clearEffModels();
                return 1;
            }
            break;
        }
        }
        if (c != 0) {
            CVECTOR col;
            SVECTOR rot;
            VECTOR sc;

            if (abr >= 0) {
                col.r = col.g = col.b = c;
            } else {
                abr = 0xFF;
                col.b = c;
                col.g = 0;
                col.r = 0;
            }
            rot.vx = 0;
            rot.vy = 0;
            rot.vz = 0;
            sc.vy = sc.vx = sx + 0x1000;
            sc.vz = 0x1000;
            ex->btlfx_drawScreenGrid(code, 0xA0, 0x70, 0x50, 0x38, 4, 4, 2, 2, &rot, &sc, &col, abr, 0);
        }
        if (alpha != 0) {
            VECTOR pos;
            VECTOR sc;
            s32 s;
            s32 k;

            pos.vx = work->unk18.vx;
            pos.vy = work->unk18.vy;
            pos.vz = work->unk18.vz;
            s = 0x4000;
            if (work->unk0 == 2) {
                s = 0xAAA;
            }
            sc.vx = sc.vy = sc.vz = s;
            ex->Hi_DrawEffModel(&work->unk38, &pos, (s32)&sc, work->unkBA);
            k = (alpha >> 5) - 0x80;
            ex->Hi_ModifyEffModelRGB(work->unkBA, k, k, k);
        }
        work->unk4++;
    }
    return 0;
}

s32 fx_17d_0_op82(s32 mode, void *work, Unk800F78A4 *env, Unk80100C78Entry *entry, s32 *arg4) {
    BtlfxExports *ex = BTLFX_EXPORTS(env);
    SVECTOR v;
    s16 *f;
    s32 n;
    s32 frame;

    if (mode == 0) {
        *arg4 = 4;
    } else if (mode == 1) {
        D_801E9470 = env;
    } else {
        frame = entry->frame;
        f = (s16 *)ex->btlfx_getFileData(entry->file, 0x1A);
        n = frame;
        if (n >= f[1]) {
            n = f[1] - 1;
        }
        f += n * 3 + 2;
        v.vx = f[0];
        v.vy = f[1];
        v.vz = f[2];
        env->camera->points[0].vx = v.vx;
        env->camera->points[0].vy = v.vy;
        env->camera->points[0].vz = v.vz;
        f = (s16 *)ex->btlfx_getFileData(entry->file, 0x25);
        n = frame;
        if (n >= f[1]) {
            n = f[1] - 1;
        }
        f += n * 3 + 2;
        env->camera->unk30[0].vz = f[2];
        f = (s16 *)ex->btlfx_getFileData(entry->file, 0x19);
        n = frame;
        if (n >= f[1]) {
            n = f[1] - 1;
        }
        f += n * 3 + 2;
        v.vx = f[0];
        v.vy = f[1];
        v.vz = f[2];
        env->camera->points[1].vx = v.vx;
        env->camera->points[1].vy = v.vy;
        env->camera->points[1].vz = v.vz;
        if (frame >= 0xA9) {
            return 1;
        }
    }
    return 0;
}

INCLUDE_RODATA("asm/us/fx_17d_0/nonmatchings/fx_17d_0", fx_17d_0_headerPtr);
