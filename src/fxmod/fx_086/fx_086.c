#include "fxmod/fx_086.h"

void func_801E7704(SVECTOR *pos, SVECTOR *rot, VECTOR *scale, s32 r0, s32 r1, s32 w0, s32 w1, s32 c, s32 tpage,
                   s32 clut, s32 u, s32 v, s32 ang, s32 dy, s32 abr, s32 dang) {
    BtlfxExports *ex = BTLFX_EXPORTS(D_801E9D6C);
    Fx086Rgbc c0;
    Fx086Rgbc c1;
    Fx086Rgbc c2;
    MATRIX m;
    SVECTOR vv[4];
    s32 z;
    POLY_GT4 *gt4;
    POLY_FT4 *ft4;
    s32 k;
    s32 j;
    s32 row;
    s32 h;
    s32 d;
    s32 y0;
    s32 y1;
    s32 s0;
    s32 s1;
    s32 ra;
    s32 rb;
    s32 a;
    s32 ft;
    s32 t;
    s32 u0;
    s32 u1;
    s32 v0;
    s32 v1;
    s32 jn;
    s32 hd;
    s32 q;

    gte_SetRotMatrix(D_801E9D6C->unk14);
    gte_SetTransMatrix(D_801E9D6C->unk14);
    gte_ldv0(pos);
    gt4 = (POLY_GT4 *)D_801E9D6C->unk24;
    *(s32 *)&c2 = 0;
    ft4 = (POLY_FT4 *)(gt4 + 2);
    D_801E9D6C->unk24 = (u8 *)(ft4 + 14);
    c0.r = c0.g = c0.b = c;
    c1.r = c1.g = c1.b = c;
    c0.code = abr != 0xFF ? 0x2E : 0x2C;
    c2.code = abr != 0xFF ? 0x3E : 0x3C;
    tpage |= (abr & 3) << 5;
    c1.code = c2.code;
    gte_rtv0tr_b();
    ex->func_800316D8(rot, &m);
    ex->ScaleMatrix(&m, scale);
    gte_stlvnl(m.t);
    ex->MulRotMatrix(&m);
    gte_SetRotMatrix(&m);
    gte_SetTransMatrix(&m);
    k = 0;
    h = 0;
    w0 >>= 1;
    w1 >>= 1;
    d = dy; /* a copy: the original keeps it in its own stack slot */
    for (row = 0; row < 1; row++, h += dy) {
        for (j = 0; j < 16; j++, k++, ang += dang << 8) {
            jn = (j + 1) << 8;
            y0 = h + (((j << 8) * dy) >> 12);
            y1 = h + ((jn * dy) >> 12);
            /* one variable for d / 2 and the second quotient: the original recomputes d >> 1 every iteration */
            hd = d >> 1;
            q = ((y0 + hd) << 12) / d;
            hd = ((y1 + hd) << 12) / d;
            s0 = ex->math_sin(q >> 1);
            s1 = ex->math_sin(hd >> 1);
            ra = ex->btlfx_lerp(r0, r1, s0);
            rb = ex->btlfx_lerp(r0, r1, s1);
            vv[0].vx = vv[1].vx = (ex->math_cos(ang) * ra) >> 12;
            vv[0].vy = vv[1].vy = (ex->math_sin(ang) * ra) >> 12;
            vv[0].vz = y0 - ex->btlfx_lerp(w0, w1, s0);
            vv[1].vz = y0 + ex->btlfx_lerp(w0, w1, s1);
            a = ang + (dang << 8);
            vv[2].vx = vv[3].vx = (ex->math_cos(a) * rb) >> 12;
            vv[2].vy = vv[3].vy = (ex->math_sin(a) * rb) >> 12;
            vv[2].vz = y1 - ex->btlfx_lerp(w0, w1, s0);
            vv[3].vz = y1 + ex->btlfx_lerp(w0, w1, s1);
            gte_ldv3c(vv);
            gte_rtpt();
            t = k & 1;
            u0 = u + (t << 6);
            u1 = u0 + 0x40;
            if (t != 0) {
                u1 = u0 + 0x3F;
            }
            v0 = v;
            v1 = v0 + 0x3F;
            gte_nclip();
            if (k == 0) {
                ft = 0;
                *(s32 *)&gt4->r0 = *(s32 *)&gt4->r1 = *(s32 *)&c2;
                *(s32 *)&gt4->r2 = *(s32 *)&gt4->r3 = *(s32 *)&c1;
            } else if (k >= 15) {
                ft = 0;
                *(s32 *)&gt4->r2 = *(s32 *)&gt4->r3 = *(s32 *)&c2;
                *(s32 *)&gt4->r0 = *(s32 *)&gt4->r1 = *(s32 *)&c1;
            } else {
                ft = 1;
                *(s32 *)&ft4->r0 = *(s32 *)&c0;
            }
            gte_stopz(&z);
            if (z != 0) {
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
                gte_stszotz(&z);
                gte_ldv0(&vv[3]);
                gte_rtps();
                if (ft) {
                    ex->btlfx_addPrim(0x09000000, z, ft4);
                    gte_stsxy(&ft4->x3);
                    ft4++;
                } else {
                    ex->btlfx_addPrim(0x0C000000, z, gt4);
                    gte_stsxy(&gt4->x3);
                    gt4++;
                }
            }
        }
    }
}

void func_801E7E18(s32 n, s32 r, SVECTOR *pts) {
    SVECTOR *p;
    s32 *prev;
    BtlfxExports *ex = BTLFX_EXPORTS(D_801E9D6C);
    s32 i;
    s32 rad;
    s32 a;

    p = pts;
    prev = (s32 *)&p[n];
    for (i = 0; i < n; i++, prev++, p++) {
        *prev = 0x7F7F7F7F;
        rad = ex->btlfx_randRange(r >> 2, r >> 1);
        a = ex->btlfx_rand() & 0xFFF;
        p->vx = (ex->math_cos(a) * rad) >> 12;
        p->vy = (ex->math_sin(a) * rad) >> 12;
        p->vz = ex->btlfx_rand() % 1280 - 0x100;
    }
}

void func_801E7F6C(u32 *ot, s32 n, s32 h, s32 dz, void *src, SVECTOR *pt, CVECTOR *col, s32 p, s32 abr) {
    Unk800F78A4 *env = D_801E9D6C;
    CVECTOR c;
    MATRIX m;
    s32 hsave;
    u32 otz;
    BtlfxExports *ex = BTLFX_EXPORTS(env);
    LINE_G2 *prim;
    s32 *prev;
    SVECTOR *pts;
    s32 live;
    s32 i;

    if (p < 0) {
        p = 0x1000;
    }
    prim = (LINE_G2 *)env->unk24;
    env->unk24 = (u8 *)(prim + n);
    if (col == NULL) {
        c.r = c.g = c.b = 0;
    } else {
        gte_DpqColorFar_nst(col, p);
        gte_strgb(&c);
    }
    gte_SetRotMatrix(env->unk14);
    gte_SetTransMatrix(env->unk14);
    gte_ReadGeomScreen(&hsave);
    gte_SetGeomScreen(h);
    ex->func_800316D8(src, &m);
    m.t[1] = 0;
    m.t[0] = 0;
    m.t[2] = h;
    gte_SetRotMatrix(&m);
    gte_SetTransMatrix(&m);
    pts = pt;
    prev = (s32 *)&pts[n];
    for (i = 0; i < n; i++, prev++, pts++, prim++) {
        live = *prev != 0x7F7F7F7F;
        gte_ldv0(pts);
        gte_rtps();
        pts->vz -= dz;
        if (dz > 0 && pts->vz < -0x100) {
            pts->vz = 0x400;
            *prev = 0x7F7F7F7F;
        } else if (dz < 0 && pts->vz > 0x300) {
            pts->vz = -0x200;
            *prev = 0x7F7F7F7F;
        } else {
            gte_stsxy(&prim->x0);
            gte_stszotz(&otz);
            if (otz < 0x200 && live) {
                gte_DpqColorFar_nst(&c, ((0x200 - otz) << 12) >> 9);
                gte_strgb(&prim->r0);
                *(s32 *)&prim->x1 = *prev;
                *(s32 *)&prim->r1 = 0;
                prim->code = abr != 0xFF ? 0x52 : 0x50;
                ex->btlfx_linkPrimAbr(0x4000000, ot, prim, abr);
            }
            *prev = *(s32 *)&prim->x0;
        }
    }
    gte_SetGeomScreen(hsave);
}

s32 fx_086_op80(s32 mode, Fx086Work80 *work, Unk800F78A4 *env, Unk80100C78Entry *entry, s32 *arg4) {
    BtlfxExports *ex = BTLFX_EXPORTS(env);
    VECTOR pos;
    VECTOR scale;
    SVECTOR rot;
    Pool6CEntry *p;
    s32 frame;
    s32 c41b;
    s32 a41;
    s32 cc;
    s32 c20;
    s32 py;
    s32 px;
    s32 c25;
    s32 c41;

    if (mode == 0) {
        *arg4 = 0x48;
    } else if (mode == 1) {
        D_801E9D6C = env;
        work->unk0 = 1;
        work->unk4 = 0;
        func_801E7E18(0x180, 0x400, D_801EA3BC);
        work->unk8[0] = 0xA0;
        work->unk8[1] = 0x78;
        work->unk8[2] = 0;
        work->unk8[3] = 0x78;
        work->unk8[4] = 0xC8;
        work->unk8[5] = 0x78;
        ex->btlfx_initEffModels(D_801E9D7C, 8);
        work->unk24 = ex->Hi_RegisterTexEffModel((BtlUnit9C8 *)ex->btlfx_getFileData(entry->unk8, 0x11), 0x9A, 0x3E00, 0, 0);
        work->unk28 = ex->Hi_RegisterTexEffModel((BtlUnit9C8 *)ex->btlfx_getFileData(entry->unk8, 4), 0x139, 0xFFFF, 0, 0x80);
        work->unk2C = ex->Hi_RegisterTexEffModel((BtlUnit9C8 *)ex->btlfx_getFileData(entry->unk8, 0x1B), 0xB9, 0x3D40, 0, 0);
        work->unk30 = ex->Hi_RegisterTexEffModel((BtlUnit9C8 *)ex->btlfx_getFileData(entry->unk8, 0xC), 0x3C, 0x3D04, 0, 0x80);
        work->unk34 = ex->Hi_RegisterGouEffModel((BtlUnit9C8 *)ex->btlfx_getFileData(entry->unk8, 0x18));
        work->unk38 = ex->Hi_RegisterGouEffModel((BtlUnit9C8 *)ex->btlfx_getFileData(entry->unk8, 0x19));
        work->unk3C = ex->Hi_RegisterGouEffModel((BtlUnit9C8 *)ex->btlfx_getFileData(entry->unk8, 0x1A));
        work->unk40 = ex->Hi_RegisterTexEffModel((BtlUnit9C8 *)ex->btlfx_getFileData(entry->unk8, 0xF), 0x97, 0x3D80, 0, 0);
    } else {
        frame = entry->unk0;
        switch (work->unk0) {
        case 1: {
        {
            SVECTOR v;
            CVECTOR col;
            s32 t;
            s32 c;
            s32 a;
            s32 s;
            s32 k;
            s32 z;

            pos.vx = 0;
            t = (frame << 11) / 49;
            pos.vz = 0x800 - t;
            pos.vy = t - 0x800;
            s = (frame << 12) / 49;
            if (frame >= 8) {
                k = (frame - 8) * 2;
                pos.vx += ex->btlfx_randCentered((frame - 8) * 3);
                pos.vy += ex->btlfx_randCentered(k);
                pos.vz += ex->btlfx_randCentered(k);
            }
            rot.vx = 0x200;
            rot.vy = 0;
            rot.vz = -frame * 0x40;
            scale.vx = 0x1000;
            scale.vy = 0x1000;
            scale.vz = 0x1000;
            work->unk14.vx = pos.vx;
            work->unk14.vy = pos.vy;
            work->unk14.vz = pos.vz - 0x46;
            work->unk14.vy += 0x1D;
            env->unk2C->unk0[3].vx = pos.vx;
            env->unk2C->unk0[3].vy = pos.vy;
            env->unk2C->unk0[3].vz = pos.vz;
            env->unk2C->unk0[3].vz += frame * 900 / 49 - 0x100;
            env->unk2C->unk0[3].vy += 0x180 - frame * 900 / 49;
            ex->Hi_DrawEffModel(&rot, &pos, (s32)&scale, work->unk24);
            ex->func_800E6624(work->unk4 * 8, 0x1C0, 0x100, 0x40, 0x80, 0x200, 0x100);
            scale.vz = scale.vz * 3 / 2;
            ex->Hi_DrawEffModel(&rot, &pos, (s32)&scale, work->unk40);
            scale.vz = scale.vz * 2 / 3;
            ex->Hi_ModifyEffModelRGB(work->unk40, (t >> 5) - 0x80, (t >> 5) - 0x80, (t >> 5) - 0x80);
            if (frame >= 8) {
                c41 = ((frame - 8) << 12) / 41;
                rot.vx = 0x200;
                rot.vy = 0;
                rot.vz = 0;
                ex->Hi_DrawEffModel(&rot, &pos, (s32)&scale, work->unk28);
                ex->Hi_ModifyEffModelAbr(work->unk28, 1);
                ex->Hi_ModifyEffModelRGB(work->unk28, (c41 >> 5) - 0x80, (c41 >> 5) - 0x80, (c41 >> 5) - 0x80);
                ex->btlfx_drawBackdrop((BtlBgLayers *)ex->btlfx_getFileData(entry->unk8, 7), work->unk4);
            }
            z = -0x64;
            col.r = 0x20;
            col.b = 0xFF;
            col.g = 0x80;
            v.vx = 0x200;
            v.vy = -0x400;
            v.vz = 0;
            if (frame < 0x18) {
                z = -0x46;
            }
            func_801E7F6C((u32 *)(env->unk20 + 8), 0x180, 0x80, z, &v, D_801EA3BC, &col,
                          ex->math_cos((frame << 10) / 49) * 3 / 4 + 0x400, 1);
            if (frame == 2) {
                work->unk44 = p = ex->func_800E3CEC(ex->btlfx_getFileData(entry->unk8, 0xB), (s32)&work->unk14);
                if (p != NULL) {
                    p->unk22 = 0x5C;
                }
            }
            if (frame >= 25) {
                CVECTOR col2;
                SVECTOR rot2;
                VECTOR sc2;

                c25 = ((frame - 24) << 12) / 25;
                ex->btlfx_drawScreenTint((u32 *)(env->unk20 + 1), 1, (c25 << 6) >> 12, 0, 0);
                col2.r = col2.g = col2.b = ((ex->math_sin(c25 >> 2) << 5) >> 12) + 0x10;
                sc2.vx = 0x1080;
                sc2.vy = 0x1080;
                sc2.vz = 0x1000;
                rot2.vx = rot2.vy = rot2.vz = 0;
                ex->func_800E6764(1, 0xA0, 0x78, 0x20, 0x18, 0xA, 0xA, 5, 5, &rot2, &sc2, &col2, 1, NULL);
            }
            px = 0x1B8 - (frame & 7) * 60;
            py = 0x140 - (frame & 7) * 60;
            ex->func_800D1BC4(2, (u8 *)ex->btlfx_getFileData(entry->unk8, 0x10), px + 0x64, py + 0x78, 0, 1, s);
            px = 0x1B8 - (frame + 5) % 10 * 60;
            py = 0x140 - (frame + 5) % 10 * 60;
            ex->func_800D1BC4(2, (u8 *)ex->btlfx_getFileData(entry->unk8, 0xE), px - 0x40, py + 0x78, 0, 1, s);
            if (frame >= 49) {
                ex->func_800E3D54(work->unk44);
                BTLFX_COPY_SVEC(&work->unk1C, &env->unkC->unk38);
                env->unk2C->unk0[3].vx = work->unk1C.vx;
                env->unk2C->unk0[3].vy = work->unk1C.vy;
                env->unk2C->unk0[3].vz = work->unk1C.vz;
                env->unk2C->unk0[3].vx += 0x1800;
                env->unk2C->unk0[3].vx -= 0x1000;
                ex->func_800E63E4(1, 1);
                work->unk0 = 2;
                entry->unk0 = -1;
            }
        }
            break;
        }
        case 2: {
        {
            SVECTOR v0;
            SVECTOR v1;
            SVECTOR v2;
            VECTOR unused; /* never used: keeps the original stack layout */
            s32 t;
            s32 c;
            s32 s;
            s32 k;
            s32 i;

            t = ((frame << 12) / 14) >> 2;
            pos.vz = work->unk1C.vx + ex->math_cos(t) * 3 / 2;
            pos.vy = work->unk1C.vy - ex->math_cos(t) * 3 / 2;
            pos.vx = 0;
            rot.vx = 0x200;
            rot.vy = 0;
            rot.vz = -frame * 0x40;
            s = 0x3800 - ex->math_cos(t);
            scale.vx = s;
            scale.vy = s;
            scale.vz = s;
            work->unk14.vx = pos.vx;
            work->unk14.vy = pos.vy;
            work->unk14.vz = pos.vz;
            if (frame < 11) {
                v0.vx = pos.vx;
                v0.vy = pos.vy;
                v0.vz = pos.vz;
                BTLFX_COPY_SVEC(&v1, &work->unk1C);
                v1.vy -= 0x200;
                ex->btlfx_lerpVec((s16 *)&v0, (s16 *)&v1, (frame << 12) / 10, (s16 *)&v2);
                env->unk2C->unk0[3].vx = v2.vx;
                env->unk2C->unk0[3].vy = v2.vy;
                env->unk2C->unk0[3].vz = v2.vz;
            }
            ex->Hi_DrawEffModel(&rot, &pos, (s32)&scale, work->unk24);
            ex->Hi_DrawEffModel(&rot, &pos, (s32)&scale, work->unk28);
            if (frame >= 7) {
                CVECTOR col;
                SVECTOR rot2;
                VECTOR sc2;

                c = ((ex->math_sin(((frame - 6) << 12) / 6 >> 2) << 6) >> 12) + 0x20;
                col.r = c;
                col.g = c >> 1;
                col.b = c >> 2;
                sc2.vx = 0x1200;
                sc2.vy = 0x1200;
                sc2.vz = 0x1000;
                rot2.vx = rot2.vy = rot2.vz = 0;
                ex->func_800E6764(1, 0xA0, 0x70, 0x46, 0x38, 4, 4, 2, 2, &rot2, &sc2, &col, 1, NULL);
            }
            {
            VECTOR sc;

            sc.vx = sc.vy = sc.vz = s / 2;
            func_801E7704(&work->unk14, &rot, &sc, 0x4B0, 0x2BC, 0x300, 0x398, 0x80, 0x1C, 0x3D05, 0x80, 0x80,
                          -(work->unk4 * 0x180), 0x2000, 1, 1);
            func_801E7704(&work->unk14, &rot, &sc, 0x4B0, 0x2BC, 0x300, 0x400, 0x80, 0x1C, 0x3D05, 0x80, 0x80,
                          -(work->unk4 * 0x180) + 0x550, 0x2000, 1, 1);
            func_801E7704(&work->unk14, &rot, &sc, 0x4B0, 0x2BC, 0x300, 0x300, 0x80, 0x1C, 0x3D05, 0x80, 0x80,
                          -(work->unk4 * 0x180) + 0xAA0, 0x2000, 1, 1);
            func_801E7704(&work->unk14, &rot, &sc, 0x4B0, 0x2BC, 0x300, 0x398, 0x80, 0x1C, 0x3D05, 0x80, 0x80,
                          work->unk4 << 9, 0x2000, 1, -1);
            func_801E7704(&work->unk14, &rot, &sc, 0x4B0, 0x2BC, 0x300, 0x400, 0x80, 0x1C, 0x3D05, 0x80, 0x80,
                          (work->unk4 << 9) + 0x550, 0x2000, 1, -1);
            func_801E7704(&work->unk14, &rot, &sc, 0x4B0, 0x2BC, 0x300, 0x300, 0x80, 0x1C, 0x3D05, 0x80, 0x80,
                          (work->unk4 << 9) + 0xAA0, 0x2000, 1, -1);
            ex->btlfx_drawBackdrop((BtlBgLayers *)ex->btlfx_getFileData(entry->unk8, 7), work->unk4);
            if (frame >= 12) {
                SVECTOR d;
                SVECTOR out;

                work->unk0 = 3;
                entry->unk0 = -1;
                ex->btlfx_drawScreenTint((u32 *)(env->unk20 + 1), 1, 0xC8, 0xC8, 0x64);
                d.vx = 0x4E0;
                d.vy = 0x800;
                d.vz = 0x700;
                for (i = 0, k = 0; i < 64; i++, k += 0x30) {
                    ex->func_800CFD78((s16 *)&work->unk1C, (s16 *)&d, k, (s16 *)&out);
                    out.vx += ex->btlfx_randCentered(0x180);
                    out.vy += ex->btlfx_randCentered(0x180);
                    out.vz += ex->btlfx_randCentered(0x180);
                    BTLFX_COPY_SVEC(&D_801EB5BC[i], &out);
                    D_801EB5BC[i].pad = i >> 2;
                }
            }
            }
        }
            break;
        }
        case 3: {
        {
            SVECTOR v;
            CVECTOR col;
            SVECTOR rot2;
            VECTOR sc2;
            s32 sx;
            s32 cs;
            s32 cs2;
            s32 a6;
            s32 t6;
            s32 t;
            s32 c;
            s32 a;
            s32 s;
            s32 k;
            s32 i;

            if (frame == 0) {
                ex->btlfx_drawScreenTint((u32 *)(env->unk20 + 1), 1, 0x64, 0x20, 0x20);
            }
            if (frame < 21) {
                env->unk2C->unk0[3].vx = work->unk1C.vx;
                env->unk2C->unk0[3].vy = work->unk1C.vy - 0x200;
                env->unk2C->unk0[3].vz = work->unk1C.vz;
                c20 = ((0x1000 - (frame << 12) / 20) << 9) >> 12;
                env->unk2C->unk0[3].vy += ex->btlfx_randCentered(c20);
                env->unk2C->unk0[3].vz += ex->btlfx_randCentered(c20);
            }
            if (frame == 12) {
                p = ex->func_800E3CEC(ex->btlfx_getFileData(entry->unk8, 9), (s32)&work->unk1C);
                if (p != NULL) {
                    p->unk22 = 0x20;
                }
            }
            if (frame == 20) {
                p = ex->func_800E3CEC(ex->btlfx_getFileData(entry->unk8, 6), (s32)&work->unk1C);
                if (p != NULL) {
                    p->unk22 = 0x20;
                }
            }
            if (frame < 7) {
                t6 = (frame << 12) / 6;
                a6 = t6 >> 2;
                s = (ex->math_cos(a6) + 0x800) * 2;
                c = ex->math_cos(a6);
                scale.vx = s;
                scale.vy = s;
                scale.vz = s >> 1;
                pos.vx = work->unk1C.vx;
                pos.vy = work->unk1C.vy;
                pos.vz = work->unk1C.vz;
                rot.vx = -0x320;
                rot.vy = 0x6C0;
                rot.vz = 0x700;
                ex->Hi_DrawEffModel(&rot, &pos, (s32)&scale, work->unk34);
                ex->Hi_ModifyEffModelRGB(work->unk34, (c >> 5) - 0x80, (c >> 5) - 0x80, (c >> 5) - 0x80);
                env->unk28 = 0x80;
                ex->func_800D23D8((u8 *)ex->btlfx_getFileData(entry->unk8, 0x12), &work->unk1C, NULL,
                                  ((ex->math_sin(t6 >> 1) + 0x1000) * 3) >> 1, 0, 1, 0x1000);
                ex->func_800D23D8((u8 *)ex->btlfx_getFileData(entry->unk8, 0x12), &work->unk1C, NULL, 0x1C00, 0, 1,
                                  0x1000);
                env->unk28 = 0;
            }
            if (frame < 8) {
                col.r = col.g = col.b = 0x46;
                sc2.vx = 0x1080;
                sc2.vy = 0x1080;
                sc2.vz = 0x1000;
                rot2.vx = rot2.vy = rot2.vz = 0;
                ex->func_800E6764(1, 0xA0, 0x78, 0x20, 0x18, 0xA, 0xA, 5, 5, &rot2, &sc2, &col, 1, NULL);
            }
            if (frame < 13) {
                cc = ex->math_cos((frame << 10) / 12);
                for (i = 0; i < 20; i++) {
                    D_801EB5BC[i * 3].pad++;
                    BTLFX_COPY_SVEC(&v, &D_801EB5BC[i * 3]);
                    v.pad = 0;
                    ex->btlfx_drawSprites((u8 *)ex->btlfx_getFileData(entry->unk8, 0x13), &v, NULL, 0x3000,
                                          D_801EB5BC[i * 3].pad, 1, cc);
                }
            }
            if (frame >= 4) {
                k = frame - 4;
                if (k == 0) {
                    ex->btlfx_drawScreenTint((u32 *)(env->unk20 + 1), 2, 0xFF, 0xFF, 0xFF);
                } else if (k < 24) {
                    c = ex->math_cos((k << 10) / 24) * 0xFF / 4096;
                    ex->btlfx_drawScreenTint((u32 *)(env->unk20 + 1), 1, c, c, c >> 1);
                }
                t = k << 7;
                if (k < 32) {
                    a = t >> 2;
                    s = ((ex->math_sin(a) >> 1) + 0x1000) * 2 / 3;
                    c = ex->math_cos(a);
                    scale.vx = s;
                    scale.vy = s;
                    scale.vz = s;
                    pos.vx = work->unk1C.vx;
                    pos.vy = work->unk1C.vy;
                    pos.vz = work->unk1C.vz;
                    rot.vx = 0;
                    rot.vy = ex->math_sin(a) >> 1;
                    rot.vz = 0;
                    if (k < 3) {
                        c = 0x1C00;
                    }
                    ex->Hi_DrawEffModel(&rot, &pos, (s32)&scale, work->unk3C);
                    ex->Hi_ModifyEffModelRGB(work->unk3C, ((c * 2 - 0x1000) >> 5) - 0x80,
                                             ((c * 2 - 0x1000) >> 5) - 0x80, ((c * 2 - 0x1000) >> 5) - 0x80);
                    if (k != 0) {
                        sx = scale.vx;
                        scale.vy = ex->math_cos(a) + 0x800;
                        scale.vx = scale.vz = t * 3 / 4 + 0xC00;
                        pos.vy += -0x370 - k * 24;
                        ex->Hi_DrawEffModel(&rot, &pos, (s32)&scale, work->unk30);
                        ex->Hi_ModifyEffModelAbr(work->unk30, 2);
                        cs = (c >> 5) - 0x80;
                        ex->Hi_ModifyEffModelRGB(work->unk30, cs, cs, cs);
                        pos.vy += k * 24;
                        pos.vy += 0xB4 - k * 20;
                        scale.vy = ex->math_cos(a) + 0x1000;
                        scale.vx = scale.vz = t + 0x1000;
                        ex->Hi_DrawEffModel(&rot, &pos, (s32)&scale, work->unk30);
                        ex->Hi_ModifyEffModelAbr(work->unk30, 1);
                        ex->Hi_ModifyEffModelRGB(work->unk30, cs, cs, cs);
                        pos.vy += 0x2BC + k * 20;
                        cs2 = 0xA00; /* a variable: a literal 0xA00 gets folded into the s >> 2 term */
                        scale.vy = (s >> 2) + (((0x1000 - ex->math_cos(a)) >> 1) + cs2);
                        scale.vx = scale.vz = sx + (0x1000 - ex->math_cos(a)) / 3;
                        ex->Hi_DrawEffModel(&rot, &pos, (s32)&scale, work->unk38);
                        ex->Hi_ModifyEffModelRGB(work->unk38, cs, cs, cs);
                    }
                }
                if (k < 41) {
                    t = (k << 12) / 41;
                    a41 = t >> 2;
                    s = ((ex->math_sin(a41) >> 1) + 0x1000) * 2 / 3;
                    c41b = ex->math_cos(a41);
                    scale.vx = s;
                    scale.vy = s;
                    scale.vz = s;
                    pos.vx = work->unk1C.vx;
                    pos.vy = work->unk1C.vy;
                    pos.vz = work->unk1C.vz;
                    rot.vx = 0;
                    rot.vy = ex->math_cos(a41) >> 2;
                    rot.vz = 0;
                    ex->func_800E6624(work->unk4 * 8, 0x240, 0x100, 0x40, 0x80, 0x300, 0x100);
                    if (k < 3) {
                        c41b = 0x1C00;
                    }
                    ex->Hi_DrawEffModel(&rot, &pos, (s32)&scale, work->unk2C);
                    ex->Hi_ModifyEffModelRGB(work->unk2C, (c41b >> 5) - 0x80, (c41b >> 5) - 0x80, (c41b >> 5) - 0x80);
                    env->unk28 = 0x300;
                    ex->func_800D23D8((u8 *)ex->btlfx_getFileData(entry->unk8, 0x12), &work->unk1C, NULL,
                                      (ex->math_cos(a41) + 0x1000) * 2, 0, 1, c41b);
                    env->unk28 = 0;
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
        work->unk4++;
    }
    return 0;
}

INCLUDE_RODATA("asm/us/fx_086/nonmatchings/fx_086", fx_086_headerPtr);
