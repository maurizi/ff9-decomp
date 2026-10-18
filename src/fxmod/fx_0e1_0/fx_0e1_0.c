#include "fxmod/fx_0e1_0.h"

INCLUDE_ASM("asm/us/fx_0e1_0/nonmatchings/fx_0e1_0", func_801E7718);

void func_801E7CB4(SVECTOR *a, SVECTOR *b, s32 n, s32 tpage, s32 clut, s32 u, s32 v, s32 w, s32 h, s32 p,
                   Fx0e10Seg *buf, s32 abr, s32 phase) {
    Unk800F78A4 *env = D_801E9E38;
    BtlfxExports *ex = BTLFX_EXPORTS(env);
    CVECTOR c;
    s32 otz;
    POLY_GT4 *prim;
    Fx0e10Seg *s;
    s32 i;
    s32 t;
    s32 k;
    s32 j;

    if (p < 0) {
        p = 0x1000;
    }
    if (n > 1000) {
        n -= 999;
        for (i = 0; i < n; i++) {
            buf[i].unk0.pad = 0;
        }
        return;
    }
    s = &buf[(u32)(n - 1)]; /* (n - 1) * 16, not n * 16 - 16 */
    for (i = 0; i < n; i++) {
        BTLFX_COPY_SVEC(&s[1].unk0, &s[0].unk0);
        BTLFX_COPY_SVEC(&s[1].unk8, &s[0].unk8);
        s--;
    }
    s = buf;
    for (i = 0; i < n; i++, s++) {
        if (!(s[0].unk0.pad & s[1].unk0.pad)) {
            break;
        }
    }
    BTLFX_COPY_SVEC(&buf->unk0, a);
    buf->unk0.pad = 1;
    BTLFX_COPY_SVEC(&buf->unk8, b);
    n = i;
    prim = (POLY_GT4 *)env->unk24;
    env->unk24 = (u8 *)(prim + n);
    c.cd = abr == 0xFF ? 0x3C : 0x3E;
    w--;
    h--;
    tpage |= (abr & 3) << 5;
    gte_SetRotMatrix(env->unk14);
    gte_SetTransMatrix(env->unk14);
    /* the last uv store (u1) is the address giv loop.c bases the others on, as in the original's s0 = &prim->u1 */
    /* i = j in the increment: i + 1 lives across the call in its own register and is copied back */
    for (i = 0; i < n; i = j, buf++, prim++, phase++) {
        gte_ldv3c(buf);
        t = (i << 12) / n;
        c.r = c.g = c.b = p;
        gte_rtpt_b();
        t = ((0x1000 - t) * p) >> 12;
        c.r = c.g = c.b = t;
        *(s32 *)&prim->r0 = *(s32 *)&prim->r1 = *(s32 *)&c;
        c.r = c.g = c.b = t;
        *(s32 *)&prim->r2 = *(s32 *)&prim->r3 = *(s32 *)&c;
        j = i + 1;
        gte_stopz(&otz);
        if (otz != 0) {
            gte_stsxy3(&prim->x0, &prim->x1, &prim->x2);
            gte_avsz3();
            gte_stszotz(&otz);
            gte_ldv0(&buf[1].unk8);
            gte_rtps();
            ex->btlfx_addPrim(0x0C000000, otz, prim);
            gte_stsxy(&prim->x3);
            prim->clut = clut;
            prim->tpage = tpage;
            prim->u0 = prim->u2 = u;
            k = phase & 0x1F;
            prim->v0 = prim->v1 = v + ((k * h) >> 5);
            prim->v2 = prim->v3 = v + (((k + 1) * h) >> 5);
            prim->u1 = prim->u3 = u + w;
        }
    }
}

void func_801E80A8(s32 ot, s32 x, s32 y, s32 nx, s32 ny, s32 dx, s32 dy, s32 abr, s32 sy, s32 tpage, s32 clut, s32 u,
                   s32 v, s32 w, s32 h, s32 c, s32 amp, s32 ph, s32 dphx, s32 dphy) {
    Unk800F78A4 *env = D_801E9E38;
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
    ft4 = (POLY_FT4 *)env->unk24;
    gt4 = (POLY_GT4 *)(ft4 + nx * (ny - 1));
    ex = BTLFX_EXPORTS(env);
    env->unk24 = (u8 *)(gt4 + nx);
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

s32 func_801E8844(SVECTOR *out) {
    Unk800F78A4 *env = D_801E9E38;
    BtlfxExports *ex = BTLFX_EXPORTS(env);
    s32 n;

    if (D_801E9DBC != 0) {
        out->pad = 1;
        n = D_801E9DB8;
        ex->Hi_GetSummonBonePos(((Fx0e10EnvB *)env->unk4)->unk60, D_801E9D84[n++], (u16 *)out);
        if (n >= 5) {
            n = 0;
        }
        D_801E9DB8 = n;
    } else {
        out->vx = 10000;
        out->vy = 10000;
        out->vz = 10000;
        out->pad = 1;
    }
    return 0;
}

void func_801E88F0(s32 arg0, s32 arg1, s32 arg2, s32 arg3, s16 *v) {
    v[0] = (v[0] * 5) >> 2;
    v[1] = (v[1] * 5) >> 2;
}

INCLUDE_RODATA("asm/us/fx_0e1_0/nonmatchings/fx_0e1_0", fx_0e1_0_headerPtr);

s32 fx_0e1_0_op80(s32 mode, Fx0e10Work80 *work, Unk800F78A4 *env, Unk80100C78Entry *entry, s32 *arg4) {
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
    Pool6CEntry *p;
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
        *arg4 = 0xA4;
    } else if (mode == 1) {
        work->unk0 = 0;
        work->unk4 = 0;
        D_801E9E38 = env;
        ex->btlfx_initEffModels(D_801EA388, 5);
        work->unk50 = ex->func_800E8F50((Unk800E8F50 *)ex->btlfx_getFileData(entry->unk8, 0x82), 0);
        work->unk54 = ex->func_800E8F50((Unk800E8F50 *)ex->btlfx_getFileData(entry->unk8, 0x81), 0);
        work->unk58 = ex->func_800E8F50((Unk800E8F50 *)ex->btlfx_getFileData(entry->unk8, 0x80), 0);
        work->unk60 = ex->func_800E8F50((Unk800E8F50 *)ex->btlfx_getFileData(entry->unk8, 3), 0);
        work->unk5C = ex->func_800E8F50((Unk800E8F50 *)ex->btlfx_getFileData(entry->unk8, 0x83), 1);
        work->unk8.vx = D_801E9D7C.vx;
        work->unk8.vy = D_801E9D7C.vy;
        work->unk8.vz = D_801E9D7C.vz;
        work->unk6C.vx = D_801E9D7C.vx;
        work->unk6C.vy = D_801E9D7C.vy;
        work->unk6C.vz = D_801E9D7C.vz;
        work->unk28.vy = -0x400;
        work->unk10.vx = work->unk8.vx;
        work->unk1C.vx = 0xC00;
        work->unk1C.vy = 0xC00;
        work->unk1C.vz = 0xC00;
        work->unk10.vy = work->unk8.vy;
        work->unk10.vz = work->unk8.vz;
        work->unk10.vx -= 0x4BB;
        work->unk64.vx = 0;
        work->unk64.vy = 0;
        work->unk64.vz = 0;
        work->unk28.vx = 0;
        work->unk28.vz = 0;
        work->unk10.vy += 0x376;
        work->unk10.vz += 0x8C9;
        work->unk3C.vx = work->unk8.vx;
        work->unk3C.vy = work->unk8.vy;
        work->unk3C.vz = work->unk8.vz;
        env->unk2C->unk0[3].vx = work->unk3C.vx;
        env->unk2C->unk0[3].vy = work->unk3C.vy;
        env->unk2C->unk0[3].vz = work->unk3C.vz;
        work->unk84.vx = D_801E9D7C.vx;
        work->unk84.vy = D_801E9D7C.vy;
        work->unk84.vz = D_801E9D7C.vz;
        work->unk90.vx = 0x7000;
        work->unk90.vy = 0x7000;
        work->unk90.vz = 0x7000;
        work->unk9C.vx = 0;
        work->unk9C.vy = 0;
        work->unk9C.vz = 0;
        work->unk30 = 0;
        work->unk4C = mode;
    } else {
        glow = 0;
        k = 0;
        rib = 0;
        wave = 0;
        frame = entry->unk0;
        wave2 = 0;
        switch (work->unk0) {
        case 0:
            k = 0x1000;
            if (frame < 0x10) {
                ex->btlfx_drawScreenTint((u32 *)(env->unk20 + 1), 1, 0xFF - ((frame * 0xFF) >> 4),
                                         0xFF - ((frame * 0xFF) >> 4), 0xFF - ((frame * 0xFF) >> 4));
            }
            if (frame < 0x2E) {
                work->unk78.vx = work->unk78.vy = work->unk78.vz = k;
                r = ex->math_cos((frame << 10) / 46);
                work->unk6C.vx = D_801E9D7C.vx + ((ex->math_cos(work->unk4 << 8) * r) >> 17);
                work->unk6C.vy = D_801E9D7C.vx + ((ex->math_sin(work->unk4 << 8) * r) >> 17);
                /* i doubles as the morph blend: r must stay single-set (sched1 then births its copy from v0 after
                   the second math_cos pointer load, as in the original) */
                i = 0x2000;
            } else {
                s32 v = ex->math_cos(((frame - 0x2E) << 10) / 18);

                i = v * 2;
                work->unk78.vx = work->unk78.vy = work->unk78.vz = k - (((k - v) * 0xF00) >> 12);
            }
            if (i > 0x1000) {
                ex->Hi_DrawMorphEffModel(&work->unk64, (VECTOR *)&work->unk6C, (s32)&work->unk78, work->unk50, work->unk54,
                                         i - 0x1000);
            } else {
                ex->Hi_DrawMorphEffModel(&work->unk64, (VECTOR *)&work->unk6C, (s32)&work->unk78, work->unk54, work->unk58, i);
            }
            col.r = col.g = col.b = 0x84;
            rot.vz = -0x18;
            sc.vy = sc.vx = 0x10F5;
            sc.vz = 0x1000;
            rot.vx = 0;
            rot.vy = 0;
            ex->func_800E6764(3, 0xA0, 0x6E, 0xA0, 0x6E, 2, 2, 1, 1, &rot, &sc, &col, 0, 0);
            if (frame >= 0x40) {
                work->unk0 = 1;
                entry->unk0 = -1;
                p = ex->func_800E3CEC(ex->btlfx_getFileData(entry->unk8, 8), (s32)&work->unk8);
                if (p != NULL) {
                    p->unk22 = 0x20;
                }
                p = ex->func_800E3CEC(ex->btlfx_getFileData(entry->unk8, 9), (s32)&work->unk8);
                if (p != NULL) {
                    p->unk22 = 0x20;
                }
                p = ex->func_800E3CEC(ex->btlfx_getFileData(entry->unk8, 0xD), (s32)&work->unk8);
                if (p != NULL) {
                    p->unk22 = 0xC;
                }
                ex->btlfx_drawScreenTint((u32 *)(env->unk20 + 1), 1, 0xFF, 0x80, 0);
                work->unk44 = ex->func_800E3CEC(ex->btlfx_getFileData(entry->unk8, 1), (s32)&work->unk8);
                if (work->unk44 != NULL) {
                    work->unk44->unk22 = 0xC;
                }
                work->unk48 = ex->func_800E3CEC(ex->btlfx_getFileData(entry->unk8, 7), (s32)&work->unk8);
                if (work->unk48 != NULL) {
                    work->unk48->unk14 = (s32)D_801EA778;
                    work->unk48->unk12 = 0xC;
                    work->unk48->unkC = 0x8C;
                    if (work->unk48 != NULL) {
                        work->unk48->unk22 = 0xC;
                    }
                }
            }
            break;
        case 1:
            t = frame << 7;
            k = 0x1000;
            glow = 0x1000;
            ex->Hi_DrawEffModel(&work->unk64, (VECTOR *)&work->unk6C, (s32)&work->unk78, work->unk58);
            ex->Hi_ModifyEffModelAbr(work->unk58, 1);
            ex->Hi_ModifyEffModelRGB(work->unk58, ((0x1000 - t) >> 5) - 0x80, ((0x1000 - t) >> 5) - 0x80,
                                     ((0x1000 - t) >> 5) - 0x80);
            if (frame >= 0x20) {
                work->unk0 = 2;
                entry->unk0 = -1;
                ex->Hi_SetSummonMotion(((Fx0e10Summon *)env->unk4)->unk54->unk180,
                                       ((Fx0e10Summon *)env->unk4)->unk60);
                ex->Hi_SetSummonMotFrame(((Fx0e10Summon *)env->unk4)->unk60, 0);
                p = ex->func_800E3CEC(ex->btlfx_getFileData(entry->unk8, 0xC), (s32)&work->unk8);
                if (p != NULL) {
                    p->unk24->unk0 = -1;
                    *(s32 *)&p->unk24->unk3C = (s32)func_801E8844;
                    p->unk22 = 0x20;
                }
            }
            break;
        case 2:
            if (frame < 0x10) {
                work->unk10.vy -= 0x80;
            }
            k = 0x1000;
            if (frame == 0x50) {
                ex->Hi_SetSummonMotFrame(((Fx0e10Summon *)env->unk4)->unk60, 0x5A);
                D_801E9DBC = 1;
            }
            if ((u32)(frame - 0x36) >= 0x1A) {
                ex->Hi_DrawSummonModel(&work->unk28, (VECTOR *)&work->unk10, (s32)&work->unk1C,
                                       ((Fx0e10Summon *)env->unk4)->unk60, 1);
                if (frame < 0x20) {
                    ex->Hi_ModifySummonModelRGB(((Fx0e10Summon *)env->unk4)->unk60,
                                                (ex->math_sin((frame << 10) >> 5) >> 5) - 0x80,
                                                ((frame << 7) >> 5) - 0x80, ((frame << 7) >> 5) - 0x80);
                    ex->Hi_ModifySummonModelAbr(((Fx0e10Summon *)env->unk4)->unk60, 1);
                }
                rib = 0x1000;
            } else {
                D_801E9DBC = 0;
            }
            if (D_801E9DBC != 0) {
                s32 d = ((ex->math_sin((frame << 11) / 159) * 3) >> 2) + 0x400;

                env->unk28 = 0xF5;
                ex->Hi_GetSummonBonePos(((Fx0e10Summon *)env->unk4)->unk60, 0, (u16 *)&work->unk34);
                ex->func_800D23D8((u8 *)ex->btlfx_getFileData(entry->unk8, 5), &work->unk34, NULL, 0x1000, 0, 1,
                                  d);
                env->unk28 = 0;
            }
            if (frame == 0) {
                p = ex->func_800E3CEC(ex->btlfx_getFileData(entry->unk8, 0xD), (s32)&work->unk8);
                if (p != NULL) {
                    p->unk22 = 0xC;
                }
                work->unk48 = ex->func_800E3CEC(ex->btlfx_getFileData(entry->unk8, 7), (s32)&work->unk8);
                if (work->unk48 != NULL) {
                    work->unk48->unk14 = (s32)D_801EAE08;
                    work->unk48->unk12 = 0xC;
                    work->unk48->unkC = 0x8C;
                    if (work->unk48 != NULL) {
                        work->unk48->unk22 = 0xC;
                    }
                }
            }
            if ((u32)(frame - 0x7F) <= 0x10) {
                s32 d = (frame - 0x7F) << 12;
                s32 e = d >> 4;

                if (work->unk44 != NULL) {
                    work->unk44->unk20 = 0x1000 - e;
                    if (work->unk44 != NULL) {
                        work->unk44->unk1C = 0x1000 - (d >> 5);
                    }
                }
                if (work->unk48 != NULL) {
                    work->unk48->unk20 = 0x1000 - e;
                }
                if (frame - 0x7F == 0x10) {
                    ex->func_800E3D54(work->unk44);
                    ex->func_800E3D54(work->unk48);
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
                work->unk0 = 3;
                entry->unk0 = -1;
                p = ex->func_800E3CEC(ex->btlfx_getFileData(entry->unk8, 0xA), (s32)&work->unk8);
                if (p != NULL) {
                    p->unk22 = 0x40;
                }
                p = ex->func_800E3CEC(ex->btlfx_getFileData(entry->unk8, 6), (s32)&work->unk8);
                if (p != NULL) {
                    p->unk22 = 0x12C;
                }
            }
            break;
        case 3:
            wave = 0x1000;
            wave2 = 0x1000;
            ex->Hi_SetSummonMotFrame(((Fx0e10Summon *)env->unk4)->unk60, 0xA9);
            ex->Hi_DrawSummonModel(&work->unk28, (VECTOR *)&work->unk10, (s32)&work->unk1C, ((Fx0e10Summon *)env->unk4)->unk60,
                                   1);
            if (frame == 0xF) {
                p = ex->func_800E3CEC(ex->btlfx_getFileData(entry->unk8, 4), (s32)&work->unk8);
                if (p != NULL) {
                    p->unk22 = 0x80;
                }
            }
            if (frame >= 0x33) {
                work->unk0 = 4;
                entry->unk0 = -1;
            }
            break;
        case 4:
            t = (frame << 12) / 49;
            if (frame < 0x20) {
                wave = ex->math_cos(frame << 5);
            }
            wave2 = t + 0x1000;
            if (frame < 0x2C) {
                ex->Hi_SetSummonMotFrame(((Fx0e10Summon *)env->unk4)->unk60, 0xA9);
                ex->Hi_DrawSummonModel(&work->unk28, (VECTOR *)&work->unk10, (s32)&work->unk1C,
                                       ((Fx0e10Summon *)env->unk4)->unk60, 1);
            }
            if (frame >= 0x31) {
                ex->btlfx_clearEffModels();
                return 1;
            }
            break;
        }
        if (wave != 0) {
            func_801E80A8(0xFFE, -0x31, 0xDC, 0xC, 5, 0x28, 0x32, 1, 0x1000, 0x99, 0x3E00, 0x80, 0, 0x40, 0x80,
                          wave >> 6, 0x20, (work->unk4 << 5) + 0x400, 0x140, 0x200);
            ex->func_800E6624(10000 - work->unk4 * 8, 0x280, 0x100, 0x20, 0x80, 0x2A0, 0x100);
        }
        if (wave2 != 0) {
            func_801E80A8(2, -0x20, 0xDC, 6, 5, 0x42, 0x32, 1, 0x1000, 0x1B, 0x3D05, 0xC0, 0, 0x40, 0x80,
                          wave2 >> 7, 0x16, work->unk4 << 6, 0x200, 0x302);
            ex->func_800E6624(10000 - work->unk4 * 4, 0x2F0, 0x100, 0x10, 0x80, 0x2E0, 0x100);
        }
        if (k != 0) {
            ex->func_800E651C(D_801E9DC0, &col, work->unk4 & 0x1F);
            col.r = ((k * col.r) >> 11) / 3;
            col.g = ((k * col.g) >> 11) / 3;
            col.b = ((k * col.b) >> 11) / 3;
            ex->Hi_DrawEffModel(&work->unk9C, (VECTOR *)&work->unk84, (s32)&work->unk90, work->unk60);
            ex->Hi_ModifyEffModelRGB(work->unk60, col.r - 0x80, col.g - 0x80, col.b - 0x80);
            work->unk9C.vy += 0x10;
        }
        if (glow != 0) {
            pos.vx = work->unk8.vx;
            pos.vy = work->unk8.vy;
            pos.vz = work->unk8.vz;
            sc2.vx = sc2.vy = 0x1000;
            sc2.vz = 0x555;
            rot.vx = work->unk4 * 0xC;
            rot.vy = work->unk4 * 4;
            rot.vz = frame * 8;
            ex->Hi_DrawEffModel(&rot, &pos, (s32)&sc2, work->unk5C);
            ex->Hi_ModifyEffModelRGB(work->unk5C, ((glow * 3) >> 7) - 0x80, ((glow * 3) >> 7) - 0x80,
                                     ((glow * 3) >> 7) - 0x80);
            sc2.vx = sc2.vy = 0xAAA;
            sc2.vz = 0x400;
            rot.vx = -work->unk4 * 4;
            rot.vy = work->unk4 * 0x10;
            rot.vz = frame * 0xC;
            ex->Hi_DrawEffModel(&rot, &pos, (s32)&sc2, work->unk5C);
            ex->Hi_ModifyEffModelRGB(work->unk5C, ((glow * 7) >> 8) - 0x80, ((glow * 7) >> 8) - 0x80,
                                     ((glow * 7) >> 8) - 0x80);
            sc2.vx = sc2.vy = 0x800;
            sc2.vz = 0x333;
            rot.vx = work->unk4 * 4;
            rot.vy = work->unk4 * 0xC;
            rot.vz = frame * 0x10;
            ex->Hi_DrawEffModel(&rot, &pos, (s32)&sc2, work->unk5C);
            ex->Hi_ModifyEffModelRGB(work->unk5C, (glow >> 5) - 0x80, (glow >> 5) - 0x80, (glow >> 5) - 0x80);
            ex->func_800E6624(work->unk4 * 4, 0x2A0, 0x180, 0x10, 0x40, 0x2B0, 0x180);
        }
        if (rib != 0) {
            env->unk28 = -0x80;
            if (work->unk4C != 0) {
                work->unk4C = 0;
                for (i = 0; i < 4; i++) {
                    ex->Hi_GetSummonBonePos(((Fx0e10Summon *)env->unk4)->unk60, D_801E9D98[i].unk0, (u16 *)&sc);
                    ex->Hi_GetSummonBonePos(((Fx0e10Summon *)env->unk4)->unk60, D_801E9D98[i].unk4, (u16 *)&b);
                    func_801E7CB4((SVECTOR *)&sc, &b, 0x3FC, 0, 0, 0, 0, 0, 0, 0, D_801E9E48[i], 1, 0);
                }
            }
            for (i = 0; i < 4; i++) {
                ex->Hi_GetSummonBonePos(((Fx0e10Summon *)env->unk4)->unk60, D_801E9D98[i].unk0, (u16 *)&sc);
                ex->Hi_GetSummonBonePos(((Fx0e10Summon *)env->unk4)->unk60, D_801E9D98[i].unk4, (u16 *)&b);
                func_801E7CB4((SVECTOR *)&sc, &b, 0x14, 0x1B, 0x3D05, 0xC0, 0, 0x40, 0x80, ((rib >> 5) * 2) / 3, D_801E9E48[i], 1,
                              10000 - work->unk4);
            }
            env->unk28 = 0;
            ex->func_800E6624(10000 - work->unk4 * 4, 0x2F0, 0x100, 0x10, 0x80, 0x2E0, 0x100);
        }
        col2.r = col2.g = col2.b = 0x89;
        sc3.vy = sc3.vx = 0x1073;
        sc3.vz = 0x1000;
        rot2.vx = 0;
        rot2.vy = 0;
        rot2.vz = 0;
        ex->func_800E6764(3, 0xA0, 0x6E, 0xA0, 0x6E, 2, 2, 1, 1, &rot2, &sc3, &col2, 0, 0);
        work->unk4++;
    }
    return 0;
}
