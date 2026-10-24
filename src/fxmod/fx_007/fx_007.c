#include "fxmod/fx_007.h"

void func_801E7704(SVECTOR *pos, SVECTOR *rot, s32 tp, s32 x, s32 y, s32 div, s32 idx, s32 size, s32 w, s32 h,
                   s32 scale, s32 bright, s32 abr, u16 *colors, s32 skip) {
    Unk800F78A4 *env = D_801E8EE8;
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

s32 fx_007_op80(s32 mode, Fx007Work80 *work, Unk800F78A4 *env, Unk80100C78Entry *entry, s32 *arg4) {
    BtlfxExports *ex = BTLFX_EXPORTS(env);
    SVECTOR rot;
    s32 alpha;
    s32 size;
    s32 frame;
    s32 t;

    if (mode == 0) {
        *arg4 = 0x38;
    } else if (mode == 1) {
        Pool6CEntry *p;

        D_801E8EE8 = env;
        ex->btlfx_getUnitBodyPos(0, 0, (s16 *)&work->unk0);
        ex->btlfx_getUnitPos(0, (s16 *)&work->unk8);
        ex->btlfx_initEffModels(D_801E8EF8, 6);
        work->unk14[0] = ex->btlfx_registerEffectModel((Unk800E8F50 *)ex->btlfx_getFileData(entry->unk8, 0x16), 1);
        work->unk14[1] = ex->btlfx_registerEffectModel((Unk800E8F50 *)ex->btlfx_getFileData(entry->unk8, 0x17), 1);
        work->unk14[2] = ex->btlfx_registerEffectModel((Unk800E8F50 *)ex->btlfx_getFileData(entry->unk8, 0x18), 1);
        work->unk14[3] = ex->btlfx_registerEffectModel((Unk800E8F50 *)ex->btlfx_getFileData(entry->unk8, 0x19), 1);
        work->unk24 = ex->Hi_RegisterTexEffModel((BtlUnit9C8 *)ex->btlfx_getFileData(entry->unk8, 8), 0x37, 0x3D06, 0x80, 0x80);
        work->unk28 = ex->Hi_RegisterTexEffModel((BtlUnit9C8 *)ex->btlfx_getFileData(entry->unk8, 9), 0x37, 0x3D06, 0x80, 0x80);
        p = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->unk8, 0xC), (s32)&work->unk8);
        if (p != NULL) {
            p->unk14 = (s32)D_801E93A8;
            p->unk12 = 0x12;
            p->unkC = 0x24;
            p->unk22 = 0x20;
        }
        work->unk2C = NULL;
        work->unk30 = NULL;
        work->unk34 = 0;
        work->unk10 = 0;
    } else {
        alpha = 0;
        size = 0;
        frame = entry->unk0;
        switch (work->unk10) {
        case 0:
            t = (frame << 12) / 24;
            alpha = t / 3;
            size = 0xAAA;
            rot.vz = 0;
            rot.vy = 0;
            rot.pad = 1;
            rot.vx = 0x400;
            env->unk28 = 0x200;
            ex->btlfx_drawRings((u8 *)ex->btlfx_getFileData(entry->unk8, 0xB), &work->unk8, NULL, 0x1000, 0, 1, t);
            env->unk28 = 0;
            if (frame >= 0x18) {
                Pool6CEntry *p;

                work->unk10 = 1;
                entry->unk0 = -1;
                p = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->unk8, 0xA), (s32)work);
                if (p != NULL) {
                    p->unk22 = 0x100;
                }
            }
            break;
        case 1:
            t = frame << 9;
            ex->btlfx_drawScreenTint((u32 *)env->ot + 0x800, 1, (t * 0x46) >> 12, 0, 0);
            if (frame == 4) {
                Pool6CEntry *p;

                p = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->unk8, 0x12), (s32)work);
                if (p != NULL) {
                    p->unk22 = 0x100;
                }
                p = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->unk8, 0x11), (s32)work);
                if (p != NULL) {
                    p->unk22 = 0x100;
                }
                p = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->unk8, 0x10), (s32)work);
                if (p != NULL) {
                    p->unk22 = 0x100;
                }
            }
            alpha = ex->math_cos(t >> 2);
            env->unk28 = 0x200;
            size = t / 3 + 0x555;
            ex->btlfx_drawRings((u8 *)ex->btlfx_getFileData(entry->unk8, 0xB), &work->unk8, NULL, 0x1000, 0, 1,
                              ex->math_cos(t >> 2));
            env->unk28 = 0;
            if (frame >= 8) {
                work->unk10 = 2;
                entry->unk0 = -1;
            }
            break;
        case 2:
            t = (frame << 12) / 46;
            {
                SVECTOR p;
                VECTOR pos;
                VECTOR sc;
                SVECTOR r;
                SVECTOR q;
                VECTOR p2;
                VECTOR sc2;
                SVECTOR sp;
                s32 y;
                s32 flag;
                s32 a;
                s32 n;
                s32 i;
                s32 m;

                if (frame < 4) {
                    n = ex->math_cos(frame << 8) * 0xFF;
                    ex->btlfx_drawScreenTint((u32 *)env->ot + 1, 1, n >> 12, n >> 13, n >> 15);
                }
                if (frame < 0x20) {
                    a = ex->math_cos(frame << 5);
                    ex->btlfx_drawScreenTint((u32 *)env->ot + 0xFFF, 0xFF,
                                             ((a / 2 + 0x800) * (ex->btlfx_rand() % 50 + 7)) >> 12, 0, 0);
                }
                y = frame * 16;
                rot.vx = 0x400;
                rot.vy = 0;
                rot.pad = 1;
                rot.vz = y;
                flag = frame < 0x1D;
                if (flag) {
                    s32 u;
                    s32 v;
                    u8 *data;

                    u = (frame << 12) / 29;
                    v = u >> 2;
                    alpha = ex->math_cos(v);
                    p.vx = work->unk8.vx;
                    size = u / 2 + 0x800;
                    p.vy = work->unk8.vy;
                    p.vz = work->unk8.vz;
                    p.vy -= 0x400;
                    data = (u8 *)ex->btlfx_getFileData(entry->unk8, 0xF);
                    ex->btlfx_drawRings(data, &p, &rot, ex->math_sin(v) * 3 / 2 + 0x1000, 0, 1, ex->math_cos(v));
                }
                env->unk28 = 0x200;
                {
                    u8 *data = (u8 *)ex->btlfx_getFileData(entry->unk8, 0xB);
                    s32 c = ex->math_cos(t >> 2) / 2 * 2 + 0x2000;

                    ex->btlfx_drawRings(data, &work->unk8, NULL, c, 0, 1, ex->math_cos(t >> 2));
                }
                env->unk28 = 0;
                if (flag) {
                    s32 v;

                    ex->btlfx_drawScrollMove(y, 0x1E0, 0x180, 0x20, 0x80, 0x1E0, 0x100);
                    v = ((frame << 12) / 29) >> 2;
                    sc.vx = sc.vz = ex->math_sin(v) / 4 + 0x80;
                    sc.vy = ex->math_sin(v) * 3 / 2 + 0x1000;
                    r.vx = 0;
                    r.vy = frame * 0x18;
                    r.vz = 0;
                    pos.vx = work->unk8.vx;
                    pos.vy = work->unk8.vy;
                    pos.vz = work->unk8.vz;
                    sc.vx = sc.vz = ex->math_sin(v) / 3 + 0x180;
                    sc.vy = ex->math_cos(v) * 3 / 2 + 0x1000;
                    ex->Hi_DrawEffModel(&r, &pos, (s32)&sc, work->unk24);
                    ex->Hi_ModifyEffModelRGB(work->unk24, (ex->math_cos(v) >> 5) - 0x80, (ex->math_cos(v) >> 5) - 0x80,
                                             (ex->math_cos(v) >> 5) - 0x80);
                }
                if (frame == 2) {
                    Pool6CEntry *pp = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->unk8, 0xE), (s32)&work->unk8);

                    if (pp != NULL) {
                        pp->unk22 = 0x100;
                    }
                }
                if (frame == 0xE) {
                    Pool6CEntry *pp = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->unk8, 0x13), (s32)work);

                    if (pp != NULL) {
                        pp->unk22 = 0x100;
                    }
                }
                if (frame == 3) {
                    work->unk2C = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->unk8, 0x15), (s32)&work->unk8);
                    if (work->unk2C != NULL) {
                        work->unk2C->unk22 = 0x100;
                    }
                    work->unk30 = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->unk8, 0x14), (s32)&work->unk8);
                    if (work->unk30 != NULL) {
                        work->unk30->unk22 = 0x100;
                    }
                }
                q.vx = 0x400;
                q.vy = 0;
                q.vz = 0;
                if (work->unk2C != NULL) {
                    if (work->unk2C->unk30 == -1) {
                        work->unk2C = NULL;
                    }
                    if (work->unk2C != NULL) {
                        work->unk2C->unk5C.vx = q.vx;
                        work->unk2C->unk5C.vy = q.vy;
                        work->unk2C->unk5C.vz = q.vz;
                    }
                }
                if (work->unk30 != NULL) {
                    if (work->unk30->unk30 == -1) {
                        work->unk30 = NULL;
                    }
                    if (work->unk30 != NULL) {
                        work->unk30->unk5C.vx = q.vx;
                        work->unk30->unk5C.vy = q.vy;
                        work->unk30->unk5C.vz = q.vz;
                    }
                }
                if (frame < 0x24) {
                    s32 r0;
                    s32 r1;
                    s32 r2;
                    s32 h;
                    s32 d;
                    s32 dy;
                    s32 w0;
                    s32 w1;
                    s32 br;
                    s32 ang2;
                    s32 sw;
                    s32 ang;
                    s32 s0;
                    s32 s1;
                    s32 s2;
                    s32 k;

                    s1 = (frame << 12) / 36;
                    ang = -frame * 0x80;
                    q.vx = 0x800;
                    ang2 = -frame * 0x19B;
                    r2 = 0;
                    q.vy = frame * 0x18;
                    q.vz = 0;
                    k = (s1 << 7) >> 12;
                    w1 = k + 0x80;
                    w0 = k + 0x100;
                    if (frame < 0x10) {
                        r0 = 0x2BC;
                        sw = 0xAA;
                        r1 = (frame * 0x2BC) / 12;
                    } else {
                        s0 = ((frame - 0x10) << 10) / 20;
                        r0 = ex->math_sin(s0) / 4 + 0x2BC;
                        r1 = ((ex->math_sin(s0) * 3) >> 2) + 0x2BC;
                        sw = s0 / 2 + 0xAA;
                        r2 = s0 / 3;
                    }
                    s0 = s1 >> 2;
                    h = (s1 >> 5) + 0x9C0;
                    dy = frame * 6 + 8;
                    br = ex->math_cos(s0) / 2;
                    if (frame < 6) {
                        d = (ex->math_sin((frame << 10) / 6) * 0x960) >> 12;
                    } else {
                        d = 0x960 - s0;
                    }
                    for (m = 0, i = 0; i < 18; i++) {
                        s1 = (i << 12) / 18;
                        p2.vx = work->unk8.vx;
                        p2.vy = work->unk8.vy;
                        p2.vz = work->unk8.vz;
                        p2.vy -= 0x200;
                        p2.vy -= (s1 * d) >> 12;
                        ang += w1;
                        s0 = (w0 * s1) >> 12;
                        p2.vx += (ex->math_sin(ang) * s0) >> 12;
                        s2 = s1 >> 2;
                        ang2 += 0x180;
                        p2.vz += (ex->math_cos(ang) * s0) >> 12;
                        s0 = ex->btlfx_lerp(r0, r1, 0x1000 - ex->math_cos(s2)) + ((ex->math_sin(ang2) * sw) >> 12);
                        sc2.vy = h;
                        sc2.vx = sc2.vz = s0;
                        if (s1 > 0x800) {
                            s0 = s1 - 0x800;
                            s0 >>= 1;
                            sc2.vx += ((0x1000 - ex->math_cos(s0)) * r2) >> 12;
                            sc2.vz += ((0x1000 - ex->math_cos(s0)) * r2) >> 12;
                        }
                        q.vy += dy;
                        ex->Hi_DrawEffModel(&q, &p2, (s32)&sc2, work->unk14[i & 3]);
                        ex->Hi_ModifyEffModelRGB(work->unk14[i & 3], ((br * ex->math_cos(s2)) >> 17) - 0x80,
                                                 ((br * (0x1000 - s1)) >> 17) - 0x80, ((br * (0x1000 - s1)) >> 17) - 0x80);
                        if (!(ex->btlfx_rand() & 3) && m < 3) {
                            env->unk28 = 0x100;
                            sp.vx = p2.vx;
                            sp.vy = p2.vy;
                            sp.vz = p2.vz;
                            sp.vx += ex->btlfx_randCentered(0x140);
                            sp.vy += ex->btlfx_randCentered(0x100);
                            sp.vz += ex->btlfx_randCentered(0x140);
                            ex->btlfx_drawRings((u8 *)ex->btlfx_getFileData(entry->unk8, 0xD), &sp, NULL, 0x7000, 0, 1, br);
                            m++;
                            env->unk28 = 0;
                        }
                    }
                }
                if (frame >= 0x2E) {
                    work->unk10 = 3;
                    entry->unk0 = -1;
                }
            }
            break;
        case 3:
            if (frame >= 0x10) {
                ex->btlfx_clearEffModels();
                return 1;
            }
            break;
        }
        if (alpha != 0) {
            rot.vx = 0x400;
            rot.vz = frame << 8;
            rot.pad = 1;
            rot.vy = 0;
            func_801E7704(&work->unk8, &rot, 0x117, 0, 0x30, 2, work->unk34 & 7, 0x100, 0x10, 0x10, size, alpha, 1,
                          D_801E9638, 0);
        }
        work->unk34++;
    }
    return 0;
}

INCLUDE_RODATA("asm/us/fx_007/nonmatchings/fx_007", fx_007_headerPtr);
