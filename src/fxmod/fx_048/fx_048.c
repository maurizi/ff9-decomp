#include "fxmod/fx_048.h"

void func_801E7704(SVECTOR *pos, SVECTOR *rot, s32 tp, s32 x, s32 y, s32 div, s32 idx, s32 size, s32 w, s32 h,
                   s32 scale, s32 bright, s32 abr, u16 *colors, s32 skip) {
    Unk800F78A4 *env = D_801E8F3C;
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

s32 fx_048_op80(s32 mode, Fx048Work *work, Unk800F78A4 *env, Unk80100C78Entry *entry, s32 *arg4) {
    BtlfxExports *ex = BTLFX_EXPORTS(env);
    SVECTOR rot;
    SVECTOR pos2;
    VECTOR pos;
    VECTOR sc;
    SVECTOR rot2;
    SVECTOR r3;
    VECTOR pos3;
    VECTOR sc2;
    SVECTOR p3;
    Pool6CEntry *p;
    s32 w;
    s32 frame;
    s32 bri;
    s32 scl;
    s32 t;
    s32 u;
    s32 x;
    s32 i;
    s32 cnt;
    s32 ang;
    s32 q;
    s32 r;
    s32 s;
    s32 sy;
    s32 g;

    if (mode == 0) {
        *arg4 = 0x38;
    } else if (mode == 1) {
        work->unk0.vx = env->targets->centerX;
        D_801E8F3C = env;
        work->unk0.vy = env->targets->centerY;
        work->unk0.vz = env->targets->centerZ;
        work->unk0.vy -= 0x200;
        BTLFX_COPY_SVEC(&work->unk8, &work->unk0);
        work->unk8.vy = 0;
        ex->btlfx_initEffModels(D_801E8F4C, 6);
        work->unk14[0] = ex->btlfx_registerEffectModel((Unk800E8F50 *)ex->btlfx_getFileData(entry->unk8, 0xC), 1);
        work->unk14[1] = ex->btlfx_registerEffectModel((Unk800E8F50 *)ex->btlfx_getFileData(entry->unk8, 9), 1);
        work->unk14[2] = ex->btlfx_registerEffectModel((Unk800E8F50 *)ex->btlfx_getFileData(entry->unk8, 0xB), 1);
        work->unk14[3] = ex->btlfx_registerEffectModel((Unk800E8F50 *)ex->btlfx_getFileData(entry->unk8, 2), 1);
        work->unk24 = ex->Hi_RegisterTexEffModel((BtlUnit9C8 *)ex->btlfx_getFileData(entry->unk8, 0xA), 0x37, 0x3D06, 0x80, 0x80);
        work->unk28 = ex->Hi_RegisterTexEffModel((BtlUnit9C8 *)ex->btlfx_getFileData(entry->unk8, 0xE), 0x37, 0x3D06, 0x80, 0x80);
        p = ex->func_800E3CEC(ex->btlfx_getFileData(entry->unk8, 0x11), (s32)&work->unk8);
        if (p != NULL) {
            p->unk14 = (s32)D_801E93FC;
            p->unk12 = 0x12;
            p->unkC = 0x24;
            p->unk22 = 0x20;
        }
        work->unk2C = NULL;
        work->unk30 = NULL;
        work->unk34 = 0;
        work->unk10 = 0;
    } else {
        bri = 0;
        scl = 0;
        frame = entry->unk0;
        switch (work->unk10) {
        case 0:
            t = (frame << 12) / 24;
            bri = t / 3;
            scl = 0x1800;
            rot.vz = 0;
            rot.vy = 0;
            rot.pad = 1;
            rot.vx = 0x400;
            env->unk28 = 0x200;
            ex->func_800D23D8((u8 *)ex->btlfx_getFileData(entry->unk8, 0x10), &work->unk8, NULL, 0x1000, 0, 1, t);
            env->unk28 = 0;
            if (frame >= 0x18) {
                work->unk10 = 1;
                entry->unk0 = -1;
                p = ex->func_800E3CEC(ex->btlfx_getFileData(entry->unk8, 5), (s32)work);
                if (p != NULL) {
                    p->unk22 = 0x100;
                }
            }
            break;
        case 1:
            t = frame << 9;
            ex->btlfx_drawScreenTint((u32 *)env->ot + 0x800, 1, (t * 70) >> 12, 0, 0);
            if (frame == 4) {
                p = ex->func_800E3CEC(ex->btlfx_getFileData(entry->unk8, 0x14), (s32)work);
                if (p != NULL) {
                    p->unk22 = 0x100;
                }
                p = ex->func_800E3CEC(ex->btlfx_getFileData(entry->unk8, 0x15), (s32)work);
                if (p != NULL) {
                    p->unk22 = 0x100;
                }
                p = ex->func_800E3CEC(ex->btlfx_getFileData(entry->unk8, 0x16), (s32)work);
                if (p != NULL) {
                    p->unk22 = 0x100;
                }
            }
            bri = ex->math_cos(t >> 2);
            env->unk28 = 0x200;
            scl = (t * 2) / 3 + 0x555;
            ex->func_800D23D8((u8 *)ex->btlfx_getFileData(entry->unk8, 0x10), &work->unk8, NULL, 0x1000, 0, 1,
                              ex->math_cos(t >> 2));
            env->unk28 = 0;
            if (frame >= 8) {
                work->unk10 = 2;
                entry->unk0 = -1;
            }
            break;
        case 2:
            t = (frame << 12) / 46;
            if (frame < 4) {
                x = ex->math_cos(frame << 8) * 0xFF;
                ex->btlfx_drawScreenTint((u32 *)env->ot + 1, 1, x >> 12, x >> 13, x >> 15);
            }
            if (frame < 0x20) {
                ex->btlfx_drawScreenTint((u32 *)env->ot + 0xFFF, 0xFF,
                                         ((ex->math_cos(frame << 5) / 2 + 0x800) * (ex->btlfx_rand() % 50 + 7)) >> 12, 0,
                                         0);
            }
            {
            s32 z;
            s32 f29;
            s32 lo;
            s32 hi;
            s32 ext;
            s32 c8;
            s32 cc;
            s32 d0;
            s32 d4;
            s32 d8;
            s32 dc;
            s32 b;
            s32 e4;

            z = frame << 4;
            rot.vx = 0x400;
            rot.vy = 0;
            rot.pad = 1;
            rot.vz = z;
            f29 = frame < 0x1D;
            if (f29) {
                g = (frame << 12) / 29;
                w = g >> 2;
                bri = ex->math_cos(w);
                pos2.vx = work->unk8.vx;
                pos2.vy = work->unk8.vy;
                scl = g + 0x1000;
                pos2.vz = work->unk8.vz;
                pos2.vy -= 0x400;
                ex->func_800D23D8((u8 *)ex->btlfx_getFileData(entry->unk8, 0x13), &pos2, &rot, ex->math_sin(w) * 2 + 0x1800,
                                  0, 1, ex->math_cos(w));
            }
            env->unk28 = 0x200;
            ex->func_800D23D8((u8 *)ex->btlfx_getFileData(entry->unk8, 0x10), &work->unk8, NULL,
                              ex->math_cos(t >> 2) / 2 * 3 + 0x2000, 0, 1, ex->math_cos(t >> 2));
            env->unk28 = 0;
            if (f29) {
                ex->btlfx_drawScrollMove(z, 0x1E0, 0x180, 0x20, 0x80, 0x1E0, 0x100);
                s = ((frame << 12) / 29) >> 2;
                sc.vx = sc.vz = ex->math_sin(s) / 4 + 0x80;
                sc.vy = ex->math_sin(s) * 3 / 2 + 0x1000;
                rot2.vx = 0;
                rot2.vy = frame * 24;
                rot2.vz = 0;
                pos.vx = work->unk8.vx;
                pos.vy = work->unk8.vy;
                pos.vz = work->unk8.vz;
                sc.vx = sc.vz = ex->math_sin(s) / 2 + 0x200;
                sc.vy = (ex->math_cos(s) * 3 / 2 + 0x1000) * 3 / 2;
                ex->Hi_DrawEffModel(&rot2, &pos, (s32)&sc, work->unk24);
                ex->Hi_ModifyEffModelRGB(work->unk24, (ex->math_cos(s) >> 5) - 0x80, (ex->math_cos(s) >> 5) - 0x80,
                                         (ex->math_cos(s) >> 5) - 0x80);
            }
            if (frame == 2) {
                p = ex->func_800E3CEC(ex->btlfx_getFileData(entry->unk8, 4), (s32)&work->unk8);
                if (p != NULL) {
                    p->unk22 = 0x100;
                }
            }
            if (frame == 0xE) {
                p = ex->func_800E3CEC(ex->btlfx_getFileData(entry->unk8, 0x17), (s32)work);
                if (p != NULL) {
                    p->unk22 = 0x100;
                }
            }
            if (frame == 3) {
                p = work->unk2C = ex->func_800E3CEC(ex->btlfx_getFileData(entry->unk8, 0x18), (s32)&work->unk8);
                if (p != NULL) {
                    p->unk22 = 0x100;
                }
                p = work->unk30 = ex->func_800E3CEC(ex->btlfx_getFileData(entry->unk8, 0x19), (s32)&work->unk8);
                if (p != NULL) {
                    p->unk22 = 0x100;
                }
            }
            r3.vx = 0x400;
            r3.vy = 0;
            r3.vz = 0;
            if (work->unk2C != NULL) {
                if (work->unk2C->unk30 == -1) {
                    work->unk2C = NULL;
                }
                if (work->unk2C != NULL) {
                    work->unk2C->unk5C.vx = r3.vx;
                    work->unk2C->unk5C.vy = r3.vy;
                    work->unk2C->unk5C.vz = r3.vz;
                }
            }
            if (work->unk30 != NULL) {
                if (work->unk30->unk30 == -1) {
                    work->unk30 = NULL;
                }
                if (work->unk30 != NULL) {
                    work->unk30->unk5C.vx = r3.vx;
                    work->unk30->unk5C.vy = r3.vy;
                    work->unk30->unk5C.vz = r3.vz;
                }
            }
            if (frame < 0x24) {
                u = (frame << 12) / 36;
                ang = -frame << 7;
                b = -frame * 0x19B;
                r3.vx = 0x800;
                ext = 0;
                r3.vy = frame * 24;
                r3.vz = 0;
                q = (u << 7) >> 12;
                d8 = q + 0x80;
                d4 = q + 0x100;
                if (frame < 0x10) {
                    lo = 0x4E2;
                    hi = (frame * 0x4E2) / 16;
                    e4 = 0xB9;
                } else {
                    s = ((frame - 0x10) << 10) / 20;
                    lo = ex->math_sin(s) / 4 + 0x4E2;
                    hi = ((ex->math_sin(s) * 3) >> 2) + 0x4E2;
                    ext = s / 3;
                    e4 = (s * 2) / 3 + 0xB9;
                }
                s = u >> 2;
                c8 = (u >> 5) + 0xE00;
                d0 = frame * 6 + 8;
                dc = ex->math_cos(s) / 2;
                if (frame < 6) {
                    cc = (ex->math_sin((frame << 10) / 6) * 0xB86) >> 12;
                } else {
                    cc = 0xA28 - s;
                }
                cnt = 0;
                for (i = 0; i < 0x12; i++) {
                    u = (i << 12) / 18;
                    pos3.vx = work->unk8.vx;
                    pos3.vy = work->unk8.vy;
                    pos3.vz = work->unk8.vz;
                    pos3.vy -= 0x200 + ((u * cc) >> 12);
                    ang += d8;
                    r = (d4 * u) >> 12;
                    pos3.vx += (ex->math_sin(ang) * r) >> 12;
                    pos3.vz += (ex->math_cos(ang) * r) >> 12;
                    sy = u >> 2;
                    b += 0x180;
                    s = ex->btlfx_lerp(lo, hi, 0x1000 - ex->math_cos(sy)) + ((ex->math_sin(b) * e4) >> 12);
                    sc2.vy = c8;
                    sc2.vx = sc2.vz = s;
                    if (u > 0x800) {
                        s = u - 0x800;
                        s >>= 1;
                        sc2.vx += ((0x1000 - ex->math_cos(s)) * ext) >> 12;
                        sc2.vz += ((0x1000 - ex->math_cos(s)) * ext) >> 12;
                    }
                    r3.vy += d0;
                    ex->Hi_DrawEffModel(&r3, &pos3, (s32)&sc2, work->unk14[i & 3]);
                    ex->Hi_ModifyEffModelRGB(work->unk14[i & 3], ((dc * ex->math_cos(sy)) >> 17) - 0x80, ((dc * (0x1000 - u)) >> 17) - 0x80, ((dc * (0x1000 - u)) >> 17) - 0x80);
                    if (!(ex->btlfx_rand() & 3) && cnt < 3) {
                        env->unk28 = 0x100;
                        p3.vx = pos3.vx;
                        p3.vy = pos3.vy;
                        p3.vz = pos3.vz;
                        p3.vx += ex->btlfx_randCentered(0x140);
                        p3.vy += ex->btlfx_randCentered(0x100);
                        p3.vz += ex->btlfx_randCentered(0x140);
                        ex->func_800D23D8((u8 *)ex->btlfx_getFileData(entry->unk8, 0x12), &p3, NULL, 0x7000, 0, 1, dc);
                        cnt++;
                        env->unk28 = 0;
                    }
                }
            }
            }
            if (frame >= 0x2E) {
                work->unk10 = 3;
                entry->unk0 = -1;
            }
            break;
        case 3:
            if (frame >= 0x10) {
                ex->btlfx_clearEffModels();
                return 1;
            }
            break;
        }
        if (bri != 0) {
            rot.vx = 0x400;
            rot.vz = frame << 8;
            rot.pad = 1;
            rot.vy = 0;
            func_801E7704(&work->unk8, &rot, 0x117, 0, 0x30, 2, work->unk34 & 7, 0x100, 0x10, 0x10, (scl * 2) / 3, bri, 1,
                          D_801E968C, 0);
        }
        work->unk34++;
    }
    return 0;
}

INCLUDE_RODATA("asm/us/fx_048/nonmatchings/fx_048", fx_048_headerPtr);
