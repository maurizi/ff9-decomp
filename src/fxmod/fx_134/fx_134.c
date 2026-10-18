#include "fxmod/fx_134.h"

void func_801E7748(s32 ot, s32 x, s32 y, s32 w, s32 h, s32 c, s32 tpage, s32 clut, s32 u, s32 v, s32 abr) {
    POLY_FT4 *p = (POLY_FT4 *)D_801EC8E4->unk24;
    BtlfxExports *ex = BTLFX_EXPORTS(D_801EC8E4);
    u8 col[4];
    s32 u1;
    s32 v1;
    s32 y1;

    D_801EC8E4->unk24 = (u8 *)(p + 2);
    col[0] = col[1] = col[2] = c;
    if (abr != 0xFF) {
        col[3] = 0x2E;
    } else {
        col[3] = 0x2C;
    }
    tpage |= (abr & 3) << 5;
    v1 = v + h - 1;
    p->u0 = p->u2 = u;
    p->u1 = p->u3 = u1 = u + w - 1;
    p->v0 = p->v1 = v;
    p->v2 = p->v3 = v1;
    p->x0 = p->x2 = x - w;
    p->x1 = p->x3 = x;
    p->y0 = p->y1 = y;
    p->clut = clut;
    p->tpage = tpage;
    *(u32 *)&p->r0 = *(u32 *)col;
    y1 = y + h * 7 / 6;
    p->y2 = p->y3 = y1;
    ex->btlfx_addPrim(0x09000000, ot, p);
    p++;
    p->x0 = p->x2 = x;
    p->u0 = p->u2 = u1;
    p->u1 = p->u3 = u;
    p->v0 = p->v1 = v;
    p->v2 = p->v3 = v1;
    p->x1 = p->x3 = x + w;
    p->y0 = p->y1 = y;
    p->y2 = p->y3 = y1;
    p->clut = clut;
    p->tpage = tpage;
    *(u32 *)&p->r0 = *(u32 *)col;
    ex->btlfx_addPrim(0x09000000, ot, p);
}

void func_801E7940(s32 ot, s32 x, s32 y, s32 w, s32 h, s32 c, s32 tpage, s32 clut, s32 u, s32 v, s32 abr, s32 flip) {
    s32 dx;
    s32 w2;
    POLY_FT4 *p;
    BtlfxExports *ex;
    u8 col[4];
    s32 n;
    s32 i;
    s32 u1;
    s32 v1;
    s32 y1;

    n = 2;
    ex = BTLFX_EXPORTS(D_801EC8E4);
    if (flip != 0) {
        n = 4;
    }
    p = (POLY_FT4 *)D_801EC8E4->unk24;
    D_801EC8E4->unk24 = (u8 *)(p + n);
    col[0] = col[1] = col[2] = c;
    if (abr != 0xFF) {
        col[3] = 0x2E;
    } else {
        col[3] = 0x2C;
    }
    tpage |= (abr & 3) << 5;
    dx = -0x140;
    if (flip != 0) {
        dx = 0xA0;
    }
    w2 = 0x140;
    if (flip != 0) {
        w2 = 0xA0;
    }
    for (i = 0; i < n; i++) {
        p->x0 = p->x2 = x;
        p->x1 = p->x3 = x + w2;
        p->y0 = p->y1 = y;
        p->y2 = p->y3 = y + h;
        if (flip != 0) {
            p->u0 = p->u2 = u;
            p->u1 = p->u3 = u + w - 1;
        } else {
            p->u0 = p->u2 = u + w - 1;
            p->u1 = p->u3 = u;
        }
        p->v0 = p->v1 = v;
        p->v2 = p->v3 = v + h - 1;
        p->clut = clut;
        p->tpage = tpage;
        *(u32 *)&p->r0 = *(u32 *)col;
        ex->btlfx_addPrim(0x09000000, ot, p);
        p++;
        x += dx;
    }
}

void func_801E7B38(SVECTOR *pos, SVECTOR *rot, s32 scale, s32 c, s32 tpage, s32 clut, s32 u, s32 v, s32 w, s32 h, s32 abr) {
    BtlfxExports *ex = BTLFX_EXPORTS(D_801EC8E4);
    MATRIX m;
    VECTOR sc;
    SVECTOR zero;
    SVECTOR vtx[4];
    u8 col[4];
    s32 otz;
    POLY_FT4 *p;
    s32 code;
    s32 a;
    s32 b;

    if (rot == NULL) {
        rot = &zero;
        ((s32 *)&zero)[1] = 0;
        ((s32 *)&zero)[0] = 0;
    }
    gte_SetRotMatrix(D_801EC8E4->unk14);
    gte_SetTransMatrix(D_801EC8E4->unk14);
    gte_ldv0(pos);
    p = (POLY_FT4 *)D_801EC8E4->unk24;
    D_801EC8E4->unk24 = (u8 *)(p + 1);
    col[0] = col[1] = col[2] = c >> 5;
    col[3] = (abr != 0xFF) ? 0x2E : 0x2C;
    tpage |= (abr & 3) << 5;
    gte_rtv0tr_b();
    ex->func_800316D8(rot, &m);
    sc.vx = sc.vy = sc.vz = scale;
    ex->ScaleMatrix(&m, &sc);
    gte_stlvnl(m.t);
    if (rot->pad != 0) {
        ex->MulRotMatrix(&m);
    }
    gte_SetRotMatrix(&m);
    gte_SetTransMatrix(&m);
    a = w << 6;
    b = -a;
    vtx[1].vx = vtx[3].vx = a;
    vtx[0].vx = vtx[2].vx = b;
    a = h << 6;
    b = -a;
    vtx[2].vy = vtx[3].vy = a;
    vtx[0].vz = vtx[1].vz = vtx[2].vz = vtx[3].vz = 0;
    vtx[0].vy = vtx[1].vy = b;
    gte_ldv3c(vtx);
    gte_rtpt();
    gte_nclip();
    p->tpage = tpage;
    p->clut = clut;
    gte_stopz(&otz);
    if (otz != 0) {
        gte_stsxy3(&p->x0, &p->x1, &p->x2);
        gte_avsz3();
        gte_stszotz(&otz);
        gte_ldv0(&vtx[3]);
        gte_rtps();
        p->u1 = p->u3 = u + w - 1;
        p->u0 = p->u2 = u;
        p->v0 = p->v1 = v;
        p->v2 = p->v3 = v + h - 1;
        *(u32 *)&p->r0 = *(u32 *)col;
        gte_stsxy(&p->x3);
        ex->btlfx_addPrim(0x09000000, otz, p);
    }
}

void func_801E7E48(SVECTOR *pos, SVECTOR *rot, s32 scale, s32 c, s32 tpage, s32 clut, s32 u, s32 v, s32 amp, s32 abr) {
    MATRIX m;
    VECTOR sc;
    SVECTOR vtx[4];
    SVECTOR center;
    s32 otz;
    s32 j;
    s32 i;
    s32 code;
    BtlfxExports *ex;
    POLY_GT4 *p;
    s32 d0;
    s32 d1;
    s32 d2;
    s32 d3;
    s32 r;
    s32 iv;
    s32 u0;
    s32 u1;
    s32 v0;
    s32 v1;

    ex = BTLFX_EXPORTS(D_801EC8E4);
    ((s32 *)&center)[0] = 0;
    center.vz = 0;
    gte_SetRotMatrix(D_801EC8E4->unk14);
    gte_SetTransMatrix(D_801EC8E4->unk14);
    gte_ldv0(pos);
    p = (POLY_GT4 *)D_801EC8E4->unk24;
    code = 0x3C;
    D_801EC8E4->unk24 = (u8 *)(p + 576);
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
    for (j = 0; j < 24; j++) {
        for (i = 0; i < 24; i++) {
            vtx[0].vx = vtx[2].vx = j * 0x180 - 0x1200;
            vtx[0].vz = vtx[1].vz = vtx[2].vz = vtx[3].vz = 0;
            vtx[0].vy = vtx[1].vy = i * 0x180 - 0x1200;
            vtx[2].vy = vtx[3].vy = i * 0x180 - 0x1080;
            vtx[1].vx = vtx[3].vx = j * 0x180 - 0x1080;
            d0 = ex->btlfx_distance(&vtx[0], &center);
            d1 = ex->btlfx_distance(&vtx[1], &center);
            d2 = ex->btlfx_distance(&vtx[2], &center);
            d3 = ex->btlfx_distance(&vtx[3], &center);
            if (d0 > 0xC00) {
                vtx[0].vz = 0;
            } else {
                vtx[0].vz = ((ex->math_cos((d0 << 11) / 0xC00) + 0x1000) * amp) >> 13;
            }
            if (d1 > 0xC00) {
                vtx[1].vz = 0;
            } else {
                vtx[1].vz = ((ex->math_cos((d1 << 11) / 0xC00) + 0x1000) * amp) >> 13;
            }
            if (d2 > 0xC00) {
                vtx[2].vz = 0;
            } else {
                vtx[2].vz = ((ex->math_cos((d2 << 11) / 0xC00) + 0x1000) * amp) >> 13;
            }
            if (d3 > 0xC00) {
                vtx[3].vz = 0;
            } else {
                vtx[3].vz = ((ex->math_cos((d3 << 11) / 0xC00) + 0x1000) * amp) >> 13;
            }
            gte_ldv3c(vtx);
            gte_rtpt();
            r = c - (vtx[0].vz >> 4);
            if (r < 0) {
                r = 0;
            }
            p->code = code;
            p->r0 = p->g0 = p->b0 = r;
            r = c - (vtx[1].vz >> 4);
            if (r < 0) {
                r = 0;
            }
            p->r1 = p->g1 = p->b1 = r;
            r = c - (vtx[2].vz >> 4);
            if (r < 0) {
                r = 0;
            }
            p->r2 = p->g2 = p->b2 = r;
            r = c - (vtx[3].vz >> 4);
            if (r < 0) {
                r = 0;
            }
            p->r3 = p->g3 = p->b3 = r;
            gte_nclip();
            p->tpage = tpage;
            p->clut = clut;
            gte_stopz(&otz);
            if (otz != 0) {
                gte_stsxy3(&p->x0, &p->x1, &p->x2);
                gte_avsz3();
                gte_stszotz(&otz);
                gte_ldv0(&vtx[3]);
                gte_rtps();
                u0 = u + ((j - 1) & 1) * 48;
                iv = (i - 1) & 1;
                v0 = v + iv * 48;
                u1 = u0 + 0x30;
                if ((j - 1) & 1) {
                    u1 = u0 + 0x2F;
                }
                v1 = v0 + 0x30;
                if (iv != 0) {
                    v1 = v0 + 0x2F;
                }
                p->u0 = p->u2 = u0;
                p->u1 = p->u3 = u1;
                p->v0 = p->v1 = v0;
                p->v2 = p->v3 = v1;
                ex->btlfx_addPrim(0x0C000000, 2, p);
                gte_stsxy(&p->x3);
                p++;
            }
        }
    }
}

void func_801E8460(Unk80100C78Entry *entry, Fx134Bolt *bolt, SVECTOR *from, SVECTOR *to, s32 amp0, s32 amp1, s32 ang,
                   s32 dang, s32 arg8, s32 arg9, s32 pad) {
    BtlfxExports *ex = BTLFX_EXPORTS(D_801EC8E4);
    SVECTOR p;
    SVECTOR rot;
    SVECTOR *pt;
    s32 i;
    s32 t;
    s32 a;

    ex->btlfx_aimAngles((s16 *)from, (s16 *)to, (s16 *)&rot);
    pt = bolt->unk4;
    bolt->unk2 = 0x18;
    bolt->unk0 = 0x18;
    bolt->unk1 = 0;
    for (i = 0; i < 25; i++) {
        t = 0x1000 - (i << 12) / 24;
        t += ex->btlfx_randCentered(0x100);
        ex->btlfx_lerpVec((s16 *)from, (s16 *)to, t, (s16 *)&p);
        a = ex->btlfx_randCentered(0xA0);
        a = (ex->math_sin(t >> 1) * a) >> 12;
        a += ex->btlfx_lerp(amp0, amp1, t);
        ex->func_800D044C(&p, (s32)pt, 0, (XY32 *)&rot, a, ang);
        pt->pad = pad;
        ang += dang + ex->btlfx_randCentered(0x80);
        pt++;
    }
    ex->btlfx_drawTrail((u8 *)ex->btlfx_getFileData(entry->unk8, 0x1E), 0, 0, arg8, 0x1000, arg9, 1, (u8 *)bolt, 0);
}

INCLUDE_RODATA("asm/us/fx_134/nonmatchings/fx_134", fx_134_headerPtr);

s32 func_801E8658(s32 mode, Fx134Orb *it, Unk800F78A4 *env, Unk80100C78Entry *entry, s32 frame, Unk800DA1B8 *pool,
                  Fx134Work *parent) {
    BtlfxExports *ex = BTLFX_EXPORTS(env);
    VECTOR pos;
    VECTOR scale;
    SVECTOR ang;
    SVECTOR d;
    SVECTOR old;
    SVECTOR t;
    SVECTOR pad;
    Pool6CEntry *e;
    s32 k;
    s32 a;
    s32 c;
    s32 i;
    s32 sh;

    if (mode == 1) {
        it->unk2 = 0;
        it->unk14.vx = ex->btlfx_rand();
        it->unk14.vy = ex->btlfx_rand();
        it->unk14.vz = 0;
        it->unk1C = ex->btlfx_rand() % 64;
        it->unk1E = ex->btlfx_rand() % 32;
        it->unk20 = NULL;
    } else {
        switch (it->unk0) {
        case 0:
        case 1:
            k = 0;
            switch (it->unk2) {
            case 0:
                it->unk14.vx += it->unk1C;
                it->unk14.vy += it->unk1E;
                old.vx = it->unkC.vx;
                old.vy = it->unkC.vy;
                old.vz = it->unkC.vz;
                it->unkC.vx += it->unk4.vx;
                it->unkC.vy += it->unk4.vy;
                it->unkC.vz += it->unk4.vz;
                pos.vx = it->unkC.vx;
                pos.vy = it->unkC.vy;
                pos.vz = it->unkC.vz;
                scale.vx = scale.vy = scale.vz = D_801EC868[it->unk4.pad];
                ex->Hi_SetEffModelOffset(parent->unk5C, 0x40);
                ex->Hi_DrawEffModel(&it->unk14, &pos, (s32)&scale, parent->unk5C);
                d.vx = parent->unk8.vx - it->unkC.vx;
                d.vy = parent->unk8.vy - it->unkC.vy;
                d.vz = parent->unk8.vz - it->unkC.vz;
                if (frame < 8) {
                    k = (frame << 12) >> 3;
                } else {
                    k = 0x1000;
                }
                if (d.vx * d.vx + d.vy * d.vy + d.vz * d.vz < D_801EF97C * D_801EF97C * D_801EF97C) {
                    it->unk2 = 1;
                    ((s16 *)it)[-1] = -1;
                    it->unk20 = e = ex->func_800E3CEC(ex->btlfx_getFileData(entry->unk8, 0xD), (s32)&it->unkC);
                    if (e != NULL) {
                        e->unk22 = 0x20;
                    }
                }
                break;
            case 1:
                scale.vx = scale.vy = scale.vz = D_801EC868[it->unk4.pad];
                pos.vx = it->unkC.vx;
                pos.vy = it->unkC.vy;
                pos.vz = it->unkC.vz;
                ex->Hi_SetEffModelOffset(parent->unk5C, 0x40);
                ex->Hi_DrawEffModel(&it->unk14, &pos, (s32)&scale, parent->unk5C);
                ex->btlfx_aimAngles((s16 *)&parent->unk8, (s16 *)&it->unkC, (s16 *)&ang);
                ang.pad = 1;
                if (it->unk20 != NULL) {
                    it->unk20->unk5C.vx = ang.vx;
                    it->unk20->unk5C.vy = ang.vy;
                    it->unk20->unk5C.vz = ang.vz;
                }
                if (frame < 4) {
                    k = 0x1000 - ((frame << 12) >> 2);
                }
                it->unk4.vx = it->unk4.vx * 2 / 3;
                it->unk4.vy = it->unk4.vy * 2 / 3;
                it->unk4.vz = it->unk4.vz * 2 / 3;
                env->unk28 = 0x1F4;
                if (frame < 8) {
                    ex->func_800D23D8((u8 *)ex->btlfx_getFileData(entry->unk8, 0xA), &it->unkC, NULL, 0x3000, frame, 1,
                                      ex->math_cos(frame << 7));
                }
                env->unk28 = 0x20;
                if (frame < 8) {
                    ex->func_800D23D8((u8 *)ex->btlfx_getFileData(entry->unk8, 9), &it->unkC, &ang, 0x2000, frame, 1,
                                      ex->math_cos(frame << 7));
                }
                env->unk28 = 0;
                if (D_801EF980 != 0) {
                    return 1;
                }
                break;
            }
            if (k != 0) {
                env->unk28 = 0x180;
                BTLFX_COPY_SVEC(&t, &it->unkC);
                t.vx -= it->unk4.vx;
                t.vy -= it->unk4.vy;
                t.vz -= it->unk4.vz;
                for (i = 0; i < 12; i++) {
                    t.vx -= it->unk4.vx;
                    t.vy -= it->unk4.vy;
                    t.vz -= it->unk4.vz;
                    func_801E7B38(&t, NULL, 0x600, k, 0x1A, 0x3D06, (i & 3) << 6, (i >> 2) << 6, 0x40, 0x40, 1);
                }
                env->unk28 = 0;
            }
            break;
        case 2:
            a = (frame << 12) / 24;
            scale.vx = scale.vy = (ex->math_sin(a >> 2) >> 1) + 0x800;
            scale.vz = it->unk4.pad + 0x200;
            it->unk14.vz -= 0x40;
            it->unkC.vy -= 0x4D;
            pos.vx = it->unkC.vx;
            pos.vy = it->unkC.vy;
            pos.vz = it->unkC.vz;
            ex->Hi_DrawEffModel(&it->unk14, &pos, (s32)&scale, parent->unk70);
            ex->Hi_ModifyEffModelRGB(parent->unk70, (ex->math_sin(a >> 1) >> 5) - 0x80, (ex->math_sin(a >> 1) >> 5) - 0x80,
                                     (ex->math_sin(a >> 1) >> 5) - 0x80);
            if (frame >= 24) {
                return 1;
            }
            break;
        case 3:
            it->unk14.vx += it->unk1C * 2;
            it->unk14.vy += it->unk1E * 2;
            it->unkC.vx += it->unk4.vx;
            it->unkC.vy += it->unk4.vy;
            it->unkC.vz += it->unk4.vz;
            pos.vx = it->unkC.vx;
            pos.vy = it->unkC.vy;
            pos.vz = it->unkC.vz;
            scale.vz = ((it->unk4.pad * ex->math_cos((frame << 8) >> 2)) >> 12) * 2;
            scale.vx = scale.vy = scale.vz / 3;
            ex->Hi_DrawEffModel(&it->unk14, &pos, (s32)&scale, parent->unk5C);
            ex->Hi_ModifyEffModelRGB(parent->unk5C, -0x4B, -0x4B, -0x4B);
            if (frame >= 16) {
                return 1;
            }
            break;
        case 4:
            it->unk14.vx += it->unk1C;
            it->unk14.vy += it->unk1E;
            it->unkC.vx += it->unk4.vx;
            it->unkC.vy += it->unk4.vy;
            it->unkC.vz += it->unk4.vz;
            it->unk4.vx = (it->unk4.vx * 31) >> 5;
            it->unk4.vy = (it->unk4.vy * 31) >> 5;
            it->unk4.vz = (it->unk4.vz * 31) >> 5;
            pos.vx = it->unkC.vx;
            pos.vy = it->unkC.vy;
            sh = frame << 6;
            /* Scheduling barrier: frame << 6 fills the pos.vy load delay and pos.vz's stays a nop. */
            do {
            } while (0);
            pos.vz = it->unkC.vz;
            scale.vz = ((it->unk4.pad * ex->math_cos(sh >> 2)) >> 12) * 2;
            scale.vx = scale.vy = scale.vz >> 1;
            ex->Hi_DrawEffModel(&it->unk14, &pos, (s32)&scale, parent->unk5C);
            ex->Hi_ModifyEffModelRGB(parent->unk5C, 0, -0x4B, -0x4B);
            if (frame >= 0x40) {
                return 1;
            }
            break;
        }
    }
    return 0;
}

/* padded: a zero word follows func_801E8658's jump table (op80's table is 8-aligned in the original object) */
__asm__(".section .rodata\n\t.word 0\n\t.section .text");

INCLUDE_ASM("asm/us/fx_134/nonmatchings/fx_134", fx_134_op80);
