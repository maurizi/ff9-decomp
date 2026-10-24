#include "fxmod/fx_097.h"

void func_801E7720(SVECTOR *pos, SVECTOR *rot, s32 tp, s32 x, s32 y, s32 div, s32 idx, s32 size, s32 w, s32 h,
                   s32 scale, s32 bright, s32 abr, u16 *colors, s32 skip) {
    Unk800F78A4 *env = D_801E9DB4;
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
        ex->func_800316D8(rot, &m);
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

void func_801E7CBC(SVECTOR *pos, SVECTOR *rot, s32 scale, s32 c, s32 tpage, s32 clut, s32 u, s32 v, s32 h, s32 abr) {
    Unk800F78A4 *env = D_801E9DB4;
    BtlfxExports *ex = BTLFX_EXPORTS(env);
    MATRIX m;
    VECTOR sc;
    SVECTOR q[4];
    SVECTOR o;
    s32 otz;
    s32 row;
    s32 code;
    s32 rx;
    s32 y;
    POLY_GT4 *prim;
    s32 col;
    s32 d0;
    s32 d1;
    s32 d2;
    s32 d3;
    s32 k;
    s32 tu;
    s32 tu2;
    s32 tv;
    s32 tv2;

    *(s32 *)&o.vx = 0;
    o.vz = 0;
    gte_SetRotMatrix(env->viewMatrix);
    gte_SetTransMatrix(env->viewMatrix);
    gte_ldv0(pos);
    prim = (POLY_GT4 *)env->prim;
    code = 0x3C;
    env->prim = (u8 *)(prim + 14 * 14);
    if (abr != 0xFF) {
        code = 0x3E;
    }
    tpage |= (abr & 3) << 5;
    gte_rtv0tr_b();
    ex->func_800316D8(rot, &m);
    sc.vx = sc.vy = sc.vz = scale;
    ex->ScaleMatrix(&m, &sc);
    gte_stlvnl(m.t);
    ex->MulRotMatrix(&m);
    gte_SetRotMatrix(&m);
    gte_SetTransMatrix(&m);
    for (row = 0; row < 14; row++) {
        y = (row << 6) - 0x180;
        rx = row << 6;
        for (col = 0; col < 14; col++) {
            q[0].vz = q[1].vz = q[2].vz = q[3].vz = 0;
            q[0].vx = q[2].vx = rx - 0x1C0;
            q[1].vx = q[3].vx = y;
            q[0].vy = q[1].vy = (col << 6) - 0x1C0;
            q[2].vy = q[3].vy = (col << 6) - 0x180;
            d0 = ex->btlfx_distance(&q[0], &o);
            d1 = ex->btlfx_distance(&q[1], &o);
            d2 = ex->btlfx_distance(&q[2], &o);
            d3 = ex->btlfx_distance(&q[3], &o);
            if (d0 > 0x100) {
                q[0].vz = 0;
            } else {
                q[0].vz = -(((ex->math_cos(d0 * 8) + 0x1000) * h) >> 13);
            }
            if (d1 > 0x100) {
                q[1].vz = 0;
            } else {
                q[1].vz = -(((ex->math_cos(d1 * 8) + 0x1000) * h) >> 13);
            }
            if (d2 > 0x100) {
                q[2].vz = 0;
            } else {
                q[2].vz = -(((ex->math_cos(d2 * 8) + 0x1000) * h) >> 13);
            }
            if (d3 > 0x100) {
                q[3].vz = 0;
            } else {
                q[3].vz = -(((ex->math_cos(d3 * 8) + 0x1000) * h) >> 13);
            }
            gte_ldv3c(q);
            gte_rtpt();
            k = c - (-q[0].vz >> 3);
            if (k >= 0x100) {
                k = 0xFF;
            }
            if (d0 > 0x1A0) {
                k = 0;
            }
            prim->code = code;
            prim->r0 = prim->g0 = prim->b0 = k;
            k = c - (-q[1].vz >> 3);
            if (k >= 0x100) {
                k = 0xFF;
            }
            if (d1 > 0x1A0) {
                k = 0;
            }
            prim->r1 = prim->g1 = prim->b1 = k;
            k = c - (-q[2].vz >> 3);
            if (k >= 0x100) {
                k = 0xFF;
            }
            if (d2 > 0x1A0) {
                k = 0;
            }
            prim->r2 = prim->g2 = prim->b2 = k;
            k = c - (-q[3].vz >> 3);
            if (k >= 0x100) {
                k = 0xFF;
            }
            if (d3 > 0x1A0) {
                k = 0;
            }
            prim->r3 = prim->g3 = prim->b3 = k;
            gte_nclip();
            prim->tpage = tpage;
            prim->clut = clut;
            gte_stopz(&otz);
            if (otz != 0) {
                gte_stsxy3(&prim->x0, &prim->x1, &prim->x2);
                gte_avsz3();
                gte_stszotz(&otz);
                gte_ldv0(&q[3]);
                gte_rtps();
                tu = u + ((row & 3) << 5);
                tu2 = tu + 0x20;
                tv = v + ((col & 3) << 5);
                if ((row & 3) == 3) {
                    tu2 = tu + 0x1F;
                }
                tv2 = tv + 0x20;
                if ((col & 3) == 3) {
                    tv2 = tv + 0x1F;
                }
                prim->u0 = prim->u2 = tu;
                prim->u1 = prim->u3 = tu2;
                prim->v0 = prim->v1 = tv;
                prim->v2 = prim->v3 = tv2;
                ex->btlfx_addPrim(0xC000000, otz, prim);
                gte_stsxy(&prim->x3);
                prim++;
            }
        }
    }
}

void func_801E8284(s16 *a, s16 *b, s32 i, s16 *out) {
    BTLFX_EXPORTS(D_801E9DB4)->btlfx_pointAhead(a, b, -((D_801E9D50[i] * 0x455) >> 9), out);
}

s32 func_801E82F4(s32 mode, Fx097Item *it, Unk800F78A4 *env, Unk80100C78Entry *entry, s32 frame, s32 a5,
                  Fx097Work *work) {
    BtlfxExports *ex = BTLFX_EXPORTS(env);
    SVECTOR *p;
    Pool6CEntry *e;
    Fx097Item *q;
    s32 t;
    s32 c;
    s32 k;
    s32 n;
    s32 d;

    if (mode == 1) {
        it->unk0 = 0;
        it->unk8 = 0;
    } else if (it->unk2 == 0) {
        SVECTOR rot;
        VECTOR scale;
        SVECTOR v38;
        SVECTOR v40;
        SVECTOR v48;
        SVECTOR v50;
        SVECTOR v58;

        p = &D_801E9CE8[it->unk4];
        switch (it->unk0) {
        case 0:
            if (it->unk8 == 0) {
                e = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->unk8, 0x25), (s32)&it->unk18);
                it->unk34 = e;
                if (e != NULL) {
                    e->unk22 = 0x80;
                }
            }
            scale.vx = 0;
            BTLFX_COPY_SVEC(&rot, p);
            rot.vx -= 0x800;
            if (it->unk34 != NULL) {
                it->unk34->unk5C.vx = rot.vx;
                it->unk34->unk5C.vy = rot.vy;
                it->unk34->unk5C.vz = rot.vz;
            }
            if (++it->unk8 > 0) {
                it->unk0 = 1;
                it->unk8 = 0;
            }
            break;
        case 1:
            {
                s32 a = it->unk8 << 10;

                scale.vx = scale.vy = (a * 0x8AA) >> 12;
                scale.vz = (ex->math_sin(a >> 2) * 0x8AA) >> 12;
            }
            BTLFX_COPY_SVEC(&rot, p);
            rot.vx -= 0x800;
            if (it->unk34 != NULL) {
                it->unk34->unk5C.vx = rot.vx;
                it->unk34->unk5C.vy = rot.vy;
                it->unk34->unk5C.vz = rot.vz;
            }
            if (++it->unk8 >= 4) {
                it->unk0 = 2;
                it->unk8 = 0;
            }
            break;
        case 2:
            scale.vx = scale.vy = scale.vz = 0x8AA;
            BTLFX_COPY_SVEC(&rot, p);
            rot.vx -= 0x800;
            if (it->unk34 != NULL) {
                it->unk34->unk5C.vx = rot.vx;
                it->unk34->unk5C.vy = rot.vy;
                it->unk34->unk5C.vz = rot.vz;
            }
            if (frame < 12) {
                rot.pad = 1;
                v38.vx = it->unkC.vx;
                v38.vy = it->unkC.vy;
                v38.vz = it->unkC.vz;
                {
                    s32 a = (frame << 10) / 12;
                    s32 b;

                    ex->btlfx_pointAhead((s16 *)&v38, (s16 *)&rot, 0x2F6, (s16 *)&v40);
                    d = ex->btlfx_getFileData(entry->unk8, 0x24);
                    b = (ex->math_sin(a) * 0x1154) >> 12;
                    ex->btlfx_drawRings((u8 *)d, &v40, &rot, b, 0, 1, ex->math_cos(a) * 2 / 3);
                }
            }
            n = 6;
            if (work->unk78 != 0) {
                it->unk8 = 0;
                if (it->unk4 < 3) {
                    n = 10;
                }
                BTLFX_COPY_SVEC(&v50, &it->unk18);
                for (k = 0; k < 4; k++, n++) {
                    q = it;
                    if (k != 0) {
                        q = ex->btlfx_poolSpawn((s16 *)entry->unk10);
                    }
                    if (q != NULL) {
                        q->unk2 = 1;
                        q->unk0 = n;
                        q->unk4 = k;
                        q->unk8 = 0;
                        q->unk2C = work->unk20[n];
                        func_801E8284((s16 *)&v50, (s16 *)p, n, (s16 *)&v48);
                        BTLFX_COPY_SVEC(&q->unk20, p);
                        q->unkC.vx = v48.vx << 4;
                        q->unkC.vy = v48.vy << 4;
                        q->unkC.vz = v48.vz << 4;
                        q->unk28 = ex->btlfx_randCentered(0x84);
                        q->unk2A = ex->btlfx_randCentered(0x84);
                        d = 0x80; /* through a variable, so the 0x80 isn't folded into k * 0x29 */
                        t = -(((ex->btlfx_rand() & 0xF) + d - k * 0x29) * 16);
                        BTLFX_COPY_SVEC(&v58, p);
                        v58.vx += 0x80;
                        ex->btlfx_rotateOffset(&v58, t, (s32)&q->unk18);
                        q->unk18.vx += ex->btlfx_randCentered(0x200);
                        q->unk18.vy += ex->btlfx_randCentered(0x200);
                        q->unk18.vz += ex->btlfx_randCentered(0x200);
                    }
                }
            }
            break;
        }
        if (scale.vx != 0) {
            scale.vx *= 2;
            scale.vy *= 2;
            scale.vz = scale.vz * 5 / 2;
            ex->Hi_SetEffModelOffset(it->unk2C, 2);
            ex->Hi_DrawEffModel(p, (VECTOR *)&it->unkC, (s32)&scale, it->unk2C);
            ex->Hi_DrawEffModel(p, (VECTOR *)&it->unkC, (s32)&scale, it->unk30);
            return 0;
        }
    } else {
        VECTOR pos;
        VECTOR scale;

        t = ((it->unk8 << 12) / 22) >> 2;
        c = ex->math_cos(t);
        it->unkC.vx += it->unk18.vx;
        it->unkC.vy += it->unk18.vy;
        it->unkC.vz += it->unk18.vz;
        it->unk18.vy += 0x40;
        it->unk20.vx += it->unk28;
        it->unk20.vy += it->unk2A;
        c = (c * 3) >> 1;
        if (it->unk0 < 6) {
            scale.vx = scale.vy = (ex->math_cos(t) * 0x19FE) >> 12;
            scale.vz = (ex->math_cos(t) * 0x1154) >> 12;
        } else {
            scale.vx = scale.vy = (ex->math_cos(t) * 0x1154) >> 12;
            scale.vz = (ex->math_cos(t) * 0x15A9) >> 12;
        }
        pos.vx = it->unkC.vx >> 4;
        pos.vy = it->unkC.vy >> 4;
        pos.vz = it->unkC.vz >> 4;
        ex->Hi_DrawEffModel(&it->unk20, &pos, (s32)&scale, it->unk2C);
        ex->Hi_ModifyEffModelAbr(it->unk2C, 1);
        ex->Hi_ModifyEffModelRGB(it->unk2C, (c >> 5) - 0x80, (c >> 5) - 0x80, (c >> 5) - 0x80);
        if (++it->unk8 >= 22) {
            return 1;
        }
    }
    return 0;
}

INCLUDE_RODATA("asm/us/fx_097/nonmatchings/fx_097", fx_097_headerPtr);

s32 fx_097_op80(s32 mode, Fx097Work80 *work, Unk800F78A4 *env, Unk80100C78Entry *entry, EffInit *init) {
    BtlfxExports *ex = BTLFX_EXPORTS(env);
    VECTOR pos;
    VECTOR scale;
    SVECTOR rot;
    SVECTOR v;
    s32 k;
    Fx097Item *it;
    Fx097Item *it2;
    Pool6CEntry *p;
    Pool6CEntry *q0;
    Pool6CEntry *q1;
    Pool6CEntry *q2;
    SVECTOR *a;
    s32 frame;
    s32 i;
    s32 j;
    SVECTOR *pr;
    s32 t;
    s32 u;
    s32 c;
    s32 c_7;
    s32 c_5;
    s32 c_3;
    s32 d;

    if (mode == 0) {
        init->unk0 = sizeof(Fx097Work80);
        init->unk8 = sizeof(Fx097Item);
        init->unk14 = (s32)func_801E82F4;
        init->unk4 = 0x20;
    } else if (mode == 1) {
        D_801E9DB4 = env;
        ex->btlfx_initEffModels(D_801E9DC4, 0x19);
        c = 0x9A;
        i = 0;
        work->unk8 = (s16)ex->Hi_RegisterTexEffModel((BtlUnit9C8 *)ex->btlfx_getFileData(entry->unk8, 0x14), c, 0x3DC0, 0x80, 0);
        work->unkC = (s16)ex->Hi_RegisterTexEffModel((BtlUnit9C8 *)ex->btlfx_getFileData(entry->unk8, 0x12), c, 0x3DC0, 0x80, 0);
        work->unk10 = (s16)ex->Hi_RegisterTexEffModel((BtlUnit9C8 *)ex->btlfx_getFileData(entry->unk8, 0x13), c, 0x3DC0, 0x80, 0);
        work->unk14 = (s16)ex->Hi_RegisterTexEffModel((BtlUnit9C8 *)ex->btlfx_getFileData(entry->unk8, 0x11), 0xBA, 0x3D40, 0, 0x80);
        work->unk18 = (s16)ex->Hi_RegisterTexEffModel((BtlUnit9C8 *)ex->btlfx_getFileData(entry->unk8, 0xF), 0xBA, 0x3D40, 0, 0x80);
        work->unk1C = (s16)ex->Hi_RegisterTexEffModel((BtlUnit9C8 *)ex->btlfx_getFileData(entry->unk8, 0x10), 0xBA, 0x3D40, 0, 0x80);
        for (; i < 14; i++) {
            work->unk20[i] =
                (s16)ex->Hi_RegisterTexEffModel((BtlUnit9C8 *)ex->btlfx_getFileData(entry->unk8, D_801E9D18[i]), c, 0x3DC0, 0x80, 0);
        }
        work->unk0 = 0;
        work->unk4 = 0;
        work->unk7A = 0;
        work->unk78 = 0;
        work->unk7C = 0;
        work->unk80 = 0;
        ex->btlfx_getUnitPos(0, (s16 *)&work->unk58);
        BTLFX_COPY_SVEC(&work->unk60, &work->unk58);
        BTLFX_COPY_SVEC(&work->unk70, &work->unk58);
        work->unk60.vy -= 0x22A;
        work->unk70.vy -= 0x80;
    } else {
        frame = entry->unk0;
        pos.vx = work->unk58.vx;
        pos.vy = work->unk58.vy;
        pos.vz = work->unk58.vz;
        scale.vx = scale.vy = 0x1DFE;
        scale.vz = 0x1354;
        k = 0;
        switch (work->unk0) {
        case 0:
            rot.vx = 0x400;
            rot.vz = 0x200;
            rot.vy = 0;
            t = (frame << 12) / 6;
            func_801E7CBC(&work->unk58, &rot, 0x26A8, t >> 5, 0x9A, 0x3D40, 0, 0x80, 0, 1);
            ex->btlfx_drawScrollMove(work->unk4 * 3, 0x280, 0x180, 0x40, 0x80, 0x280, 0x100);
            if (frame >= 6) {
                work->unk0 = 1;
                entry->unk0 = -1;
            }
            break;
        case 1:
            t = (frame << 12) / 10;
            u = t >> 2;
            rot.vx = 0x400;
            rot.vy = 0;
            rot.vz = 0x200;
            func_801E7CBC(&work->unk58, &rot, 0x26A8, 0x80, 0x9A, 0x3D40, 0, 0x80, -((ex->math_sin(u) * 0x2BC) >> 12), 1);
            ex->btlfx_drawScrollMove(work->unk4 * 3, 0x280, 0x180, 0x40, 0x80, 0x280, 0x100);
            k = t;
            rot.pad = 1;
            d = ex->btlfx_getFileData(entry->unk8, 0x24);
            ex->btlfx_drawRings((u8 *)d, &work->unk58, &rot, ((((ex->math_sin(u) * 0x19FE) >> 12) + 0x1154) * 3) >> 2, 0, 1,
                              ex->math_cos(u));
            if (frame >= 10) {
                work->unk0 = 2;
                entry->unk0 = -1;
                BTLFX_COPY_SVEC(&work->unk68, &work->unk58);
                work->unk68.vy -= 0x5C6;
                q0 = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->unk8, 0x26), (s32)&work->unk58);
                if (q0 != NULL) {
                    q0->unk22 = 0x80;
                }
                q1 = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->unk8, 0x27), (s32)&work->unk68);
                if (q1 != NULL) {
                    q1->unk22 = 0x80;
                }
                q2 = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->unk8, 0x29), (s32)&work->unk58);
                if (q2 != NULL) {
                    q2->unk22 = 0x80;
                }
                ex->btlfx_drawScreenTint((u32 *)(env->ot + 1), 2, 0xFF, 0xFF, 0);
                ex->btlfx_drawScreenTint((u32 *)(env->ot + 1), 1, 0, 0, 0x80);
            }
            break;
        case 2:
            k = 0x1000;
            t = frame << 10;
            t = 0x2000 - t;
            rot.vx = -0x400;
            rot.vy = 0;
            rot.vz = -0x400;
            ex->Hi_SetEffModelOffset(work->unk8, 2);
            ex->Hi_DrawEffModel(&rot, &pos, (s32)&scale, work->unk8);
            ex->Hi_ModifyEffModelRGB(work->unk8, (t >> 5) - 0x80, (t >> 5) - 0x80, (t >> 5) - 0x80);
            ex->Hi_DrawEffModel(&rot, &pos, (s32)&scale, work->unk14);
            if (frame < 6) {
                t = 0xFF - (frame * 0xFF) / 6;
                ex->btlfx_drawScreenTint((u32 *)(env->ot + 1), 1, t >> 2, t * 2 / 3, t);
                if (frame == 0) {
                    ex->btlfx_drawScreenTint((u32 *)(env->ot + 1), 1, 0xFF, 0xFF, 0xFF);
                }
            }
            if (frame >= 4) {
                work->unk0 = 3;
                entry->unk0 = -1;
            }
            break;
        case 3:
            k = 0x1000;
            if (frame & 1) {
                it = ex->btlfx_poolSpawn((s16 *)entry->unk10);
                if (it != NULL) {
                    it->unk2 = 0;
                    it->unk4 = work->unk7A;
                    a = &D_801E9CE8[work->unk7A];
                    if (work->unk7A >= 3) {
                        it->unk2C = work->unkC;
                        it->unk30 = work->unk18;
                    } else {
                        it->unk2C = work->unk10;
                        it->unk30 = work->unk1C;
                    }
                    it->unkC.vx = work->unk58.vx;
                    it->unkC.vy = work->unk58.vy;
                    it->unkC.vz = work->unk58.vz;
                    t = ex->math_sin(a->vy + 0x800);
                    c_3 = 0xC8;
                    it->unkC.vx += (t * c_3) >> 12;
                    it->unkC.vz += (ex->math_cos(a->vy + 0x800) * c_3) >> 12;
                    it->unk18.vx = it->unkC.vx;
                    it->unk18.vy = it->unkC.vy;
                    it->unk18.vz = it->unkC.vz;
                    work->unk7A++;
                }
            }
            rot.vx = -0x400;
            rot.vy = 0;
            rot.vz = -0x400;
            ex->Hi_SetEffModelOffset(work->unk8, 2);
            ex->Hi_DrawEffModel(&rot, &pos, (s32)&scale, work->unk8);
            ex->Hi_DrawEffModel(&rot, &pos, (s32)&scale, work->unk14);
            if (work->unk7A >= 6) {
                work->unk0 = 4;
                entry->unk0 = -1;
                p = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->unk8, 0x1C), (s32)&work->unk58);
                work->unk84 = p;
                if (p != NULL) {
                    p->unk22 = 0x5C;
                }
            }
            break;
        case 4:
            k = 0x1000;
            rot.vx = -0x400;
            rot.vy = 0;
            rot.vz = -0x400;
            ex->Hi_SetEffModelOffset(work->unk8, 2);
            ex->Hi_DrawEffModel(&rot, &pos, (s32)&scale, work->unk8);
            ex->Hi_DrawEffModel(&rot, &pos, (s32)&scale, work->unk14);
            if (frame >= 8) {
                work->unk0 = 5;
                entry->unk0 = -1;
            }
            break;
        case 5:
            pr = &rot; /* one pseudo (s2) for case 5's calls and the spawn loop's */
            k = 0x1000;
            rot.vx = -0x400;
            rot.vy = 0;
            rot.vz = -0x400;
            ex->Hi_SetEffModelOffset(work->unk8, 2);
            ex->Hi_DrawEffModel(pr, &pos, (s32)&scale, work->unk8);
            ex->Hi_DrawEffModel(pr, &pos, (s32)&scale, work->unk14);
            rot.pad = 1;
            if (frame >= 10) {
                work->unk0 = 6;
                work->unk78 = 1;
                entry->unk0 = -1;
                ex->btlfx_drawScreenTint((u32 *)(env->ot + 1), 2, 0xFF, 0xFF, 0);
                ex->btlfx_stopEmitter(work->unk84);
                p = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->unk8, 0x20), (s32)&work->unk58);
                if (p != NULL) {
                    p->unk22 = 0x60;
                }
                p = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->unk8, 0x1F), (s32)&work->unk58);
                if (p != NULL) {
                    p->unk22 = 0x60;
                }
                ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->unk8, 0x1D), (s32)&work->unk60);
                p = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->unk8, 0x21), (s32)&work->unk60);
                if (p != NULL) {
                    p->unk22 = 0x80;
                }
                p = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->unk8, 0x22), (s32)&work->unk60);
                if (p != NULL) {
                    p->unk22 = 0x80;
                }
                p = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->unk8, 0x28), (s32)&work->unk60);
                if (p != NULL) {
                    p->unk22 = 0x80;
                }
                j = 0;
                rot.vx = -0x400;
                rot.vy = 0;
                rot.vz = -0x400;
            loop:
                it2 = ex->btlfx_poolSpawn((s16 *)entry->unk10);
                if (it2 != NULL) {
                    it2->unk2 = 1;
                    it2->unk0 = j;
                    it2->unk4 = j;
                    it2->unk8 = 0;
                    it2->unk2C = work->unk20[j];
                    func_801E8284((s16 *)&work->unk58, (s16 *)pr, j, (s16 *)&v);
                    BTLFX_COPY_SVEC(&it2->unk20, &rot);
                    it2->unkC.vx = v.vx << 4;
                    it2->unkC.vy = v.vy << 4;
                    it2->unkC.vz = v.vz << 4;
                    it2->unk28 = ex->btlfx_randCentered(0x5C);
                    it2->unk2A = ex->btlfx_randCentered(0x80);
                    c = 0x5C;
                    ex->btlfx_rotateOffset(pr, -(((ex->btlfx_rand() & 0xF) + c - (j << 3)) << 4), (s32)&it2->unk18);
                    it2->unk18.vx += ex->btlfx_randCentered(0x3E0);
                    it2->unk18.vz += ex->btlfx_randCentered(0x3E0);
                }
                j++;
                if (j < 6) {
                    goto loop;
                }
            }
            break;
        case 6:
            rot.vx = -0x400;
            rot.vz = -0x400;
            rot.vy = 0;
            if (frame < 18) {
                k = ex->math_cos((frame << 10) / 18);
            }
            if (frame == 5) {
                p = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->unk8, 0x1E), (s32)&work->unk60);
                if (p != NULL) {
                    p->unk22 = 0x80;
                }
            }
            if (frame < 8) {
                c_5 = 0xFF - ((frame * 0xFF) >> 3);
                ex->btlfx_drawScreenTint((u32 *)(env->ot + 1), 1, c_5 >> 2, c_5 * 2 / 3, c_5);
                if (frame == 0) {
                    ex->btlfx_drawScreenTint((u32 *)(env->ot + 1), 1, 0xFF, 0xFF, 0xFF);
                }
            }
            if (frame >= 0x18) {
                ex->btlfx_clearEffModels();
                return 1;
            }
            break;
        }
        if (work->unk0 - 2 < 4) {
            if (++work->unk7C < 11) {
                c_7 = work->unk80 + 11;
                work->unk80 = c_7 - (work->unk7C * 10) / 11;
            } else {
                work->unk80++;
            }
            ex->btlfx_drawScrollMove(work->unk80, 0x280, 0x180, 0x40, 0x80, 0x280, 0x100);
        }
        if (k != 0) {
            v.vx = 0x400;
            v.vy = 0;
            v.vz = work->unk4 * 0x29;
            v.pad = 1;
            ex->btlfx_drawRings((u8 *)ex->btlfx_getFileData(entry->unk8, 0x23), &work->unk58, &v, (work->unk4 << 6) + 0x15A9, 0,
                              1, k);
            v.vz = work->unk4 * 0x18 + 0x400;
            ex->btlfx_drawRings((u8 *)ex->btlfx_getFileData(entry->unk8, 0x23), &work->unk58, &v, (work->unk4 << 6) + 0x15A9, 0,
                              1, k);
            env->unk28 = 0x127;
            ex->btlfx_drawRings((u8 *)ex->btlfx_getFileData(entry->unk8, 0x1B), &work->unk58, NULL, (work->unk4 << 5) + 0x15A9, 0,
                              1, k);
            env->unk28 = 0;
        }
        work->unk4++;
    }
    return 0;
}
