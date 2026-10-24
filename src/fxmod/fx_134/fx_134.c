#include "fxmod/fx_134.h"

void func_801E7748(s32 ot, s32 x, s32 y, s32 w, s32 h, s32 c, s32 tpage, s32 clut, s32 u, s32 v, s32 abr) {
    POLY_FT4 *p = (POLY_FT4 *)D_801EC8E4->prim;
    BtlfxExports *ex = BTLFX_EXPORTS(D_801EC8E4);
    u8 col[4];
    s32 u1;
    s32 v1;
    s32 y1;

    D_801EC8E4->prim = (u8 *)(p + 2);
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
    p = (POLY_FT4 *)D_801EC8E4->prim;
    D_801EC8E4->prim = (u8 *)(p + n);
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
    gte_SetRotMatrix(D_801EC8E4->viewMatrix);
    gte_SetTransMatrix(D_801EC8E4->viewMatrix);
    gte_ldv0(pos);
    p = (POLY_FT4 *)D_801EC8E4->prim;
    D_801EC8E4->prim = (u8 *)(p + 1);
    col[0] = col[1] = col[2] = c >> 5;
    col[3] = (abr != 0xFF) ? 0x2E : 0x2C;
    tpage |= (abr & 3) << 5;
    gte_rtv0tr_b();
    ex->math_rotMatrixYXZ(rot, &m);
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
    gte_SetRotMatrix(D_801EC8E4->viewMatrix);
    gte_SetTransMatrix(D_801EC8E4->viewMatrix);
    gte_ldv0(pos);
    p = (POLY_GT4 *)D_801EC8E4->prim;
    code = 0x3C;
    D_801EC8E4->prim = (u8 *)(p + 576);
    if (abr != 0xFF) {
        code = 0x3E;
    }
    tpage |= (abr & 3) << 5;
    gte_rtv0tr_b();
    ex->math_rotMatrixYXZ(rot, &m);
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
    ex->btlfx_drawTrail((u8 *)ex->btlfx_getFileData(entry->file, 0x1E), 0, 0, arg8, 0x1000, arg9, 1, (u8 *)bolt, 0);
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
                    it->unk20 = e = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, 0xD), (s32)&it->unkC);
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
                    ex->btlfx_drawRings((u8 *)ex->btlfx_getFileData(entry->file, 0xA), &it->unkC, NULL, 0x3000, frame, 1,
                                      ex->math_cos(frame << 7));
                }
                env->unk28 = 0x20;
                if (frame < 8) {
                    ex->btlfx_drawRings((u8 *)ex->btlfx_getFileData(entry->file, 9), &it->unkC, &ang, 0x2000, frame, 1,
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

/* Matching notes (same family as fx_0e0 op80): scalar temporaries are separate variables per case (_cN) and per
 * re-assignment (_pN/_wN, or a short name), since a variable reused across cases or phases spans blocks and changes
 * the allocation. Case 3's first angle a3x = t >> 2 (t = (frame << 12) / 24) is block-local: with it the case-3
 * block has eight call-crossing locals, its 0x1000 constant lands in s7 and is kicked out by work (work's
 * REG_LIVE_LENGTH is doubled for its REG_EQUIV, so ex is allocated first and gets fp, work then takes s7). The
 * outer switch sits in its own block so case 3/4's stack locals are freed before the tail's (which reuse 0x38).
 * Field copies are often written as three copies then the +=/-= adjustments (flow keeps both stores), the
 * statement orders were searched (build/s/permhc.py, bperm.py), and cases 6, 7, 8 and 10 follow fx_0e0's matched
 * forms. */
s32 fx_134_op80(s32 mode, Fx134Work *work, Unk800F78A4 *env, Unk80100C78Entry *entry, EffInit *init) {
    s32 n6;
    s32 r6;
    s32 d6;
    s32 a0b;
    s32 a_c4_w1;
    s32 k_c4_w3;
    s32 a1b;
    s32 a_c3_p2;
    s32 k_c3_p2;
    s32 i_c3_p2;
    s32 a_c4_p2;
    s32 a_c5_p2;
    s32 a_c5_p3;
    s32 k_c5_p2;
    s32 i_c5_p2;
    Fx134Orb *o_c1;
    Fx134Orb *o_c4;
    Fx134Orb *o_c8;
    s32 a3x;
    s32 d1;
    s32 a_c0;
    s32 b_c0;
    s32 k_c0;
    s32 a_c1;
    s32 k_c1;
    s32 i_c1;
    s32 n_c1;
    s32 a_c2;
    s32 b_c2;
    s32 i_c2;
    s32 a_c3;
    s32 k_c3;
    s32 i_c3;
    s32 n_c3;
    s32 a_c4;
    s32 b_c4;
    s32 k_c4;
    s32 i_c4;
    s32 n_c4;
    s32 a_c5;
    s32 k_c5;
    s32 i_c5;
    s32 n_c5;
    s32 b_c6;
    s32 k_c6;
    s32 i_c6;
    s32 n_c6;
    s32 a1_c6;
    s32 a_c7;
    s32 k_c7;
    s32 i_c8;
    s32 k_c9;
    BtlfxExports *ex = BTLFX_EXPORTS(env);
    Pool6CEntry *p;
    s32 frame;
    s32 t;
    s32 c;
    s32 s;
    s32 xo;
    s32 yo;

    if (mode == 0) {
        init->unk0 = 0x84;
        init->unk8 = 0x24;
        init->unk14 = (s32)func_801E8658;
        init->unk4 = 0x31;
    } else if (mode == 1) {
        work->unk0 = 0;
        work->unk4 = 0;
        D_801EC8E4 = env;
        ex->btlfx_initEffModels(D_801ECB64, 8);
        work->unk5C = ex->Hi_RegisterTexEffModel((BtlUnit9C8 *)ex->btlfx_getFileData(entry->file, 8), 0x98, 0x3D80, 0, 0);
        work->unk60 = ex->Hi_RegisterTexEffModel((BtlUnit9C8 *)ex->btlfx_getFileData(entry->file, 0xB), 0x118, 0xFFFF, 0x40,
                                                 0);
        work->unk64 = ex->Hi_RegisterTexEffModel((BtlUnit9C8 *)ex->btlfx_getFileData(entry->file, 0x14), 0x118, 0xFFFF,
                                                 0x40, 0);
        work->unk68 = ex->Hi_RegisterTexEffModel((BtlUnit9C8 *)ex->btlfx_getFileData(entry->file, 0x11), 0x37, 0x3D07, 0,
                                                 0);
        work->unk6C = ex->Hi_RegisterGouEffModel((BtlUnit9C8 *)ex->btlfx_getFileData(entry->file, 0x13));
        work->unk70 = ex->Hi_RegisterTexEffModel((BtlUnit9C8 *)ex->btlfx_getFileData(entry->file, 0x12), 0x37, 0x3D07, 0,
                                                 0);
        work->unk74 = ex->Hi_RegisterTexEffModel((BtlUnit9C8 *)ex->btlfx_getFileData(entry->file, 0x19), 0xBC, 0x3E00, 0x80,
                                                 0);
        work->unk8.vx = D_801EC860.vx;
        work->unk8.vy = D_801EC860.vy - 0x3000;
        work->unk8.vz = D_801EC860.vz;
        work->unk7C = p = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, 1), (s32)&work->unk8);
        if (p != NULL) {
            p->unk14 = (s32)D_801ED1A4;
            work->unk7C->unk12 = 0x40;
            work->unk7C->unkC = 0xCC;
        }
    } else {
        c = 0;
        s = 0;
        xo = 0;
        yo = 0;
        frame = entry->frame;
        {
        switch (work->unk0) {
        case 0:
            switch (frame) {
            default: {
                SVECTOR v;

                t = (frame << 12) / 40;
                if (frame < 0x10) {
                    BTLFX_COPY_SVEC(&work->unk18, &D_801EC860);
                    a_c0 = frame << 6;
                    work->unk18.vy -= ex->math_sin(a_c0) * 3;
                    env->camera->points[3].vx = work->unk18.vx;
                    env->camera->points[3].vy = work->unk18.vy;
                    env->camera->points[3].vz = work->unk18.vz;
                    b_c0 = (ex->math_sin(a_c0) >> 3) - 0x200;
                } else {
                    b_c0 = 0;
                }
                v.vx = ex->btlfx_randCentered(0x44C) + 0x400;
                v.vy = 0;
                v.vz = 0;
                if (work->unk7C != NULL) {
                    work->unk7C->unk5C.vx = v.vx;
                    work->unk7C->unk5C.vy = v.vy;
                    work->unk7C->unk5C.vz = v.vz;
                }
                if (frame < 8) {
                    k_c0 = 0;
                } else if (frame < 0xC) {
                    k_c0 = (frame - 8) << 10;
                } else {
                    k_c0 = 0x1000;
                }
                if (k_c0 != 0) {
                    ex->btlfx_drawSpriteAtOt(0xFF8, (u8 *)ex->btlfx_getFileData(entry->file, 3), a0b = (t >> 7) + 0xA0, b_c0, 0, 1, k_c0);
                    k_c0 += ex->btlfx_randCentered(0x18);
                    if (k_c0 < 0) {
                        k_c0 = 0;
                    }
                    ex->btlfx_drawSpriteAtOt(0xFF8, (u8 *)ex->btlfx_getFileData(entry->file, 4), a0b, b_c0, 0, 1, k_c0);
                    ex->btlfx_drawSpriteAtOt(0xFF8, (u8 *)ex->btlfx_getFileData(entry->file, 0x1F), a0b, b_c0 + 0x60, 0, 1, k_c0);
                }
                if (frame == 0x18) {
                    work->unk80 = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, 5), (s32)&work->unk8);
                }
                if (frame >= 8) {
                    ex->btlfx_drawRings((u8 *)ex->btlfx_getFileData(entry->file, 6), &work->unk8, NULL, 0x1800, frame, 2, -1);
                }
                if (frame >= 0x28) {
                    work->unk0 = 1;
                    entry->frame = -1;
                    ex->btlfx_stopEmitter(work->unk7C);
                    ex->btlfx_stopEmitter(work->unk80);
                    BTLFX_COPY_SVEC(&work->unk8, &D_801EC860);
                    work->unk30.vx = 0x1800;
                    work->unk30.vy = 0x1800;
                    work->unk30.vz = 0x1800;
                    work->unk3C.vx = 0x3C0;
                    work->unk8.vy -= 0x3000;
                    work->unk3C.vy = 0x100;
                    work->unk3C.vz = 0x180;
                    work->unk24.vx = work->unk8.vx;
                    work->unk24.vy = work->unk8.vy;
                    work->unk24.vz = work->unk8.vz;
                    ex->btlfx_startScreenFade(1, 1);
                }
                break;
            }
            }
            break;
        case 1:
            switch (frame) {
            default: {
                SVECTOR ang;

                t = frame << 6;
                BTLFX_COPY_SVEC(&work->unk18, &work->unk8);
                d1 = 0x1000 - t;
                work->unk18.vy += 0x400 + (d1 >> 1);
                s = d1 >> 4;
                env->camera->points[3].vx = work->unk18.vx;
                env->camera->points[3].vy = work->unk18.vy;
                env->camera->points[3].vz = work->unk18.vz;
                ex->Hi_DrawEffModel(&work->unk3C, (VECTOR *)&work->unk24, (s32)&work->unk30, work->unk60);
                a_c1 = t >> 2;
                ex->Hi_ModifyEffModelRGB(work->unk60, (((ex->math_sin(a_c1) >> 1) + 0x800) >> 5) - 0x80,
                                         (((ex->math_sin(a_c1) >> 1) + 0x800) >> 5) - 0x80,
                                         (((ex->math_sin(a_c1) >> 1) + 0x800) >> 5) - 0x80);
                ex->btlfx_drawBackdrop((BtlBgLayers *)ex->btlfx_getFileData(entry->file, 0xC), work->unk4);
                ex->btlfx_drawSpriteAtOt(0xFFE, (u8 *)ex->btlfx_getFileData(entry->file, 3), (t >> 6) + 0xA0, (t >> 9) + 0x78,
                                  0, 1, -1);
                func_801E7748(0xFFD, (t >> 5) + 0x78, (t >> 8) + 0xA9, 0x100, 0x40, 0x80, 0x98, 0x3DC0, 0, 0xC0, 0xFF);
                ex->btlfx_drawRippleGrid((s32)(env->ot + 8), (t >> 8) + 0x78, 1, 2, 0x78, 1, D_801EC878, 0, 0, 0, 0);
                c = 0x93;
                yo = 0;
                xo = 0;
                ex->btlfx_drawSpriteAtOt(0xFFE, (u8 *)ex->btlfx_getFileData(entry->file, 0x18), (frame << 6) / 40 + 0xBE, 0x73, 0,
                                  0xFF, -1);
                ex->btlfx_drawSpriteAtOt(0xFFE, (u8 *)ex->btlfx_getFileData(entry->file, 0x20), (t >> 6) + 0x20, 0x40, 0, 0xFF, -1);
                n_c1 = (frame << 7) + 0x2000;
                o_c1 = ex->btlfx_poolSpawn((s16 *)entry->pool);
                if (o_c1 != NULL) {
                    i_c1 = frame & 3;
                    k_c1 = frame * 0x140 + (i_c1 << 5) - 0x20;
                    k_c1 = k_c1 % 3072 + 0x600;
                    a1b = D_801EC880[i_c1] + 0x300;
                    o_c1->unkC.vx = work->unk8.vx + ((ex->math_cos(k_c1) * n_c1) >> 12);
                    o_c1->unkC.vz = work->unk8.vz + ((ex->math_sin(k_c1) * n_c1) >> 12);
                    o_c1->unkC.vy = work->unk8.vy + ((((frame + 4) % 6) << 11) - 0x1400);
                    ex->btlfx_aimAngles((s16 *)&o_c1->unkC, (s16 *)&work->unk8, (s16 *)&ang);
                    ex->btlfx_rotateOffset(&ang, a1b, (s32)&o_c1->unk4);
                    o_c1->unk0 = frame & 1;
                    o_c1->unk4.pad = i_c1;
                }
                D_801EF97C = (t >> 8) + 0x116;
                if (frame == 0x3E) {
                    ex->btlfx_drawScreenTint((u32 *)(env->ot + 2), 1, 0x46, 0x46, 0x46);
                }
                if (frame == 0x3F) {
                    ex->btlfx_drawScreenTint((u32 *)(env->ot + 2), 1, 0x80, 0x80, 0x80);
                }
                if (frame >= 0x40) {
                    work->unk0 = 2;
                    entry->frame = -1;
                    D_801EF980 = 1;
                    ex->btlfx_initParticleSystem();
                    work->unk3C.vx = 0x400;
                    work->unk3C.vy = 0;
                    work->unk3C.vz = 0;
                    ex->btlfx_startScreenFade(1, 1);
                }
                break;
            }
            }
            break;
        case 2:
            switch (frame) {
            default: {
                SVECTOR rot;
                Fx134Vec3 pos;
                Fx134Vec3 scale;

                t = (frame << 12) / 6;
                if (frame == 0) {
                    ex->btlfx_poolClear(entry->pool);
                }
                c = 0x97;
                s = 0x10;
                yo = 0;
                xo = 0;
                if (frame == 0) {
                    ex->btlfx_drawScreenTint((u32 *)(env->ot + 2), 1, 0xC0, 0xC0, 0xC0);
                }
                if (frame == 1) {
                    ex->btlfx_drawScreenTint((u32 *)(env->ot + 2), 1, 0x60, 0x60, 0x60);
                }
                BTLFX_COPY_SVEC(&work->unk18, &work->unk8);

                env->camera->points[3].vx = work->unk18.vx;
                env->camera->points[3].vy = work->unk18.vy;
                env->camera->points[3].vz = work->unk18.vz;
                work->unk3C.vy += 4;
                ex->Hi_DrawEffModel(&work->unk3C, (VECTOR *)&work->unk24, (s32)&work->unk30, work->unk64);
                ex->btlfx_drawBackdrop((BtlBgLayers *)ex->btlfx_getFileData(entry->file, 0xC), work->unk4);
                ex->btlfx_drawSpriteAtOt(0xFFE, (u8 *)ex->btlfx_getFileData(entry->file, 3), 0x120, 0x58, 0, 1, -1);
                ex->btlfx_drawSpriteAtOt(0xFF8, (u8 *)ex->btlfx_getFileData(entry->file, 4), 0x78, 0xA0, 0, 1, -1);
                ex->btlfx_drawSpriteAtOt(0xFF8, (u8 *)ex->btlfx_getFileData(entry->file, 0x1F), 0x78, 0x46, 0, 1, -1);
                a_c2 = (ex->math_sin(t >> 2) >> 3) + 0x1300;
                scale.vz = 0x1000;
                b_c2 = (((t >> 1) + 0x800) >> 5) - 0x80;
                pos.vx = work->unk24.vx;
                pos.vy = work->unk24.vy;
                pos.vz = work->unk24.vz;
                pos.vy += 0xC00;
                rot.vx = -0x400;
                rot.vy = 0;
                rot.vz = frame << 5;
                for (i_c2 = 0; i_c2 < 4; i_c2++) {
                    scale.vx = scale.vy = (a_c2 * ((ex->math_sin((i_c2 << 11) / 3) >> 1) + 0x800)) >> 12;
                    ex->Hi_DrawEffModel(&rot, (VECTOR *)&pos, (s32)&scale, work->unk70);
                    ex->Hi_ModifyEffModelRGB(work->unk70, b_c2, b_c2, b_c2);
                    rot.vz += 0x400;
                    pos.vy -= 0x800;
                }
                if (frame >= 6) {
                    work->unk0 = 3;
                    entry->frame = -1;
                    work->unk50.vx = 0x400;
                    work->unk44.vx = 0x3000;
                    work->unk44.vy = 0x3000;
                    work->unk50.vy = 0;
                    work->unk50.vz = 0;
                    work->unk44.vz = 0x1800;
                }
                break;
            }
            }
            break;
        case 3: {
            Fx134Vec3 pos;
            Fx134Vec3 scale;
            SVECTOR rot;
            Fx134Vec3 pos2;
            Fx134Vec3 scale2;
            SVECTOR rot2;

            t = (frame << 12) / 24;
            a3x = t >> 2;
            k_c3 = ex->math_cos(a3x);
            BTLFX_COPY_SVEC(&work->unk18, &work->unk8);

            env->camera->points[3].vx = work->unk18.vx;
            env->camera->points[3].vy = work->unk18.vy;
            c = 0x97;
            env->camera->points[3].vz = work->unk18.vz;
            s = (k_c3 >> 4) + 0x20;
            work->unk3C.vy += 4;
            n_c3 = t >> 7;
            work->unk3C.vz += n_c3;
            yo = 0;
            xo = 0;
            work->unk24.vy += (((0x1000 - ex->math_cos(a3x)) << 9) >> 12) + (ex->math_cos(a3x) >> 6) * ((frame & 1) * 2 - 1);
            ex->Hi_DrawEffModel(&work->unk3C, (VECTOR *)&work->unk24, (s32)&work->unk30, work->unk64);
            ex->Hi_ModifyEffModelRGB(work->unk64, -0x2B, -0x2B, -0x2B);
            ex->btlfx_drawBackdrop((BtlBgLayers *)ex->btlfx_getFileData(entry->file, 0x30), work->unk4);
            work->unk44.vx = work->unk44.vy = t + 0x2000;
            work->unk44.vz = 0x1800;
            work->unk50.vz += ex->math_sin(a3x) >> 7;
            ex->Hi_DrawEffModel(&work->unk50, (VECTOR *)&work->unk24, (s32)&work->unk44, work->unk68);
            ex->Hi_ModifyEffModelRGB(work->unk68, (ex->math_sin(a3x) >> 6) - 0x80, (ex->math_sin(a3x) >> 6) - 0x80,
                                     (ex->math_sin(a3x) >> 6) - 0x80);
            work->unk50.vz += 0x200;
            ex->Hi_DrawEffModel(&work->unk50, (VECTOR *)&work->unk24, (s32)&work->unk44, work->unk68);
            ex->Hi_ModifyEffModelRGB(work->unk68, (ex->math_sin(a3x) >> 5) - 0x80, (ex->math_sin(a3x) >> 5) - 0x80,
                                     (ex->math_sin(a3x) >> 5) - 0x80);
            work->unk50.vz -= 0x200;
            ex->Hi_DrawEffModel(&work->unk50, (VECTOR *)&work->unk24, (s32)&work->unk44, work->unk6C);
            ex->Hi_ModifyEffModelRGB(work->unk6C, n_c3 - 0x80, n_c3 - 0x80, n_c3 - 0x80);
            scale.vx = scale.vy = ex->math_sin(a3x) * 2 + 0x400;
            scale.vz = 0x1000;
            pos.vx = work->unk24.vx;
            pos.vy = work->unk24.vy;
            a_c3 = t >> 1;
            pos.vz = work->unk24.vz;
            rot.vx = 0x400;
            rot.vy = 0;
            rot.vz = frame << 7;
            for (i_c3 = 0; i_c3 < 4; i_c3++) {
                ex->Hi_DrawEffModel(&rot, (VECTOR *)&pos, (s32)&scale, work->unk70);
                ex->Hi_ModifyEffModelRGB(work->unk70, (ex->math_sin(a_c3) >> 5) - 0x80, (ex->math_sin(a_c3) >> 5) - 0x80,
                                         (ex->math_sin(a_c3) >> 5) - 0x80);
                scale.vx = scale.vy = scale.vx * 2 / 3;
                rot.vz += 0x200;
                pos.vy += 0x40;
            }
            i_c3_p2 = 0;
            k_c3_p2 = ex->math_sin(t >> 2);
            a_c3_p2 = t >> 2;
            scale2.vz = (t >> 1) + 0x1000;
            pos2.vx = work->unk24.vx;
            pos2.vy = work->unk24.vy;
            pos2.vz = work->unk24.vz;
            pos2.vy += 0xC00;
            rot2.vx = -0x400;
            rot2.vy = 0;
            rot2.vz = 0xC0 - (frame << 6);
            for (; i_c3_p2 < 4; i_c3_p2++) {
                scale2.vx = scale2.vy = ((k_c3_p2 * 3 / 2 + 0x1500) * ((ex->math_sin((i_c3_p2 << 10) / 3) >> 1) + 0x800)) >> 12;
                ex->Hi_DrawEffModel(&rot2, (VECTOR *)&pos2, (s32)&scale2, work->unk70);
                ex->Hi_ModifyEffModelRGB(work->unk70, (ex->math_cos(a_c3_p2) >> 5) - 0x80, (ex->math_cos(a_c3_p2) >> 5) - 0x80,
                                         (ex->math_cos(a_c3_p2) >> 5) - 0x80);
                rot2.vz += 0x400;
                pos2.vy -= 0x800;
            }
            ex->btlfx_drawSpriteAtOt(0xFFE, (u8 *)ex->btlfx_getFileData(entry->file, 3), 0x120, 0x58, 0, 1, -1);
            ex->btlfx_drawSpriteAtOt(0xFF8, (u8 *)ex->btlfx_getFileData(entry->file, 4), 0x78, 0xA0, 0, 1, -1);
            ex->btlfx_drawSpriteAtOt(0xFF8, (u8 *)ex->btlfx_getFileData(entry->file, 0x1F), 0x78, 0x46, 0, 1, -1);
            if (frame >= 0x18) {
                work->unk0 = 4;
                entry->frame = -1;
                work->unk24.vx = work->unk8.vx;
                work->unk24.vz = work->unk8.vz;
                work->unk24.vy = work->unk8.vy;
                work->unk30.vx = 0x36D;
                work->unk30.vy = 0x36D;
                work->unk30.vz = 0x36D;
                work->unk24.vy -= 0x800;
                work->unk10.vx = work->unk24.vx;
                work->unk10.vy = work->unk24.vy;
                work->unk10.vz = work->unk24.vz;
                work->unk7C = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, 0x15), (s32)&work->unk10);
                work->unk80 = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, 0x16), (s32)&work->unk10);
            }
            break;
        }
        case 4: {
            SVECTOR rot;
            Fx134Vec3 scale;
            Fx134Vec3 pos;
            SVECTOR pos3;
            SVECTOR rot3;

            t = (frame << 12) / 40;
            if (frame > 0x10) {
                a_c4 = ((frame - 0x10) << 10) / 24;
                c = (ex->math_sin(a_c4) >> 6) + 0x8D;
                s = a_c4 + 0x10;
            } else {
                c = 0x8D;
                s = 0x10;
            }
            BTLFX_COPY_SVEC(&work->unk18, &work->unk8);

            env->camera->points[3].vx = work->unk18.vx;
            env->camera->points[3].vy = work->unk18.vy;
            env->camera->points[3].vz = work->unk18.vz;
            yo = 0;
            work->unk3C.vz += 0x20;
            work->unk24.vy += 0x60 + ex->btlfx_randCentered(8);
            work->unk10.vx = work->unk24.vx;
            work->unk10.vy = work->unk24.vy;
            work->unk10.vz = work->unk24.vz;
            work->unk10.vy += 0x578;
            ex->Hi_DrawEffModel(&work->unk3C, (VECTOR *)&work->unk24, (s32)&work->unk30, work->unk64);
            ex->Hi_ModifyEffModelRGB(work->unk64, 0, 0, 0);
            ex->btlfx_drawBackdrop((BtlBgLayers *)ex->btlfx_getFileData(entry->file, 0x30), work->unk4);
            work->unk44.vx = work->unk44.vy = 0x492;
            work->unk44.vz = 0xAAA;
            work->unk24.vy -= 0x578;
            work->unk50.vz += 0x18;
            ex->Hi_DrawEffModel(&work->unk50, (VECTOR *)&work->unk24, (s32)&work->unk44, work->unk68);
            ex->Hi_ModifyEffModelRGB(work->unk68, -0x40, -0x40, -0x40);
            work->unk50.vz += 0x200;
            ex->Hi_DrawEffModel(&work->unk50, (VECTOR *)&work->unk24, (s32)&work->unk44, work->unk68);
            ex->Hi_ModifyEffModelRGB(work->unk68, -0x40, -0x40, -0x40);
            work->unk44.vx = work->unk44.vy = 0x924;
            work->unk44.vz = 0x1000;
            work->unk50.vz -= 0x200;
            work->unk24.vy -= 0x60E;
            ex->Hi_DrawEffModel(&work->unk50, (VECTOR *)&work->unk24, (s32)&work->unk44, work->unk6C);
            a_c4_w1 = t >> 2;
            ex->Hi_ModifyEffModelRGB(work->unk6C, (((ex->math_sin(a_c4_w1) >> 1) + 0x800) >> 5) - 0x80,
                                     (((ex->math_sin(a_c4_w1) >> 1) + 0x800) >> 5) - 0x80,
                                     (((ex->math_sin(a_c4_w1) >> 1) + 0x800) >> 5) - 0x80);
            xo = 0;
            work->unk24.vy += 0xB86;
            if (frame > 0x1C) {
                b_c4 = frame - 0x1C;
                k_c4 = (b_c4 << 12) / 12;
                pos.vx = work->unk24.vx;
                pos.vy = work->unk24.vy;
                pos.vz = work->unk24.vz;
                pos.vy += 0x6A4;
                rot.vx = 0;
                rot.vy = frame << 6;
                rot.vz = 0;
                scale.vx = scale.vz = ((ex->math_sin(k_c4 >> 2) * 7) >> 3) + 0x578;
                scale.vy = ex->math_sin(k_c4 >> 2) * 2 / 3 + 0x400;
                ex->Hi_DrawEffModel(&rot, (VECTOR *)&pos, (s32)&scale, work->unk74);
                ex->Hi_ModifyEffModelRGB(work->unk74, (ex->math_sin(k_c4 >> 1) >> 5) - 0x80, (ex->math_sin(k_c4 >> 1) >> 5) - 0x80,
                                         (ex->math_sin(k_c4 >> 1) >> 5) - 0x80);
            }
            env->unk28 = 0x100;
            k_c4_w3 = ex->math_sin(a_c4_w1);
            pos3.vx = work->unk24.vx;
            pos3.vy = work->unk24.vy;
            pos3.vz = work->unk24.vz;
            rot3.pad = 0;
            rot3.vy = 0;
            rot3.vx = 0;
            rot3.vz = frame << 5;
            for (i_c4 = 0; i_c4 < 0x10; i_c4++) {
                pos3.vy -= 0x19F;
                ex->btlfx_rand();
                func_801E7B38(&pos3, &rot3, 0x600, k_c4_w3, 0x1A, 0x3D02, (i_c4 & 3) << 6, (i_c4 >> 2) << 6, 0x40, 0x40, 1);
            }
            env->unk28 = 0;
            n_c4 = frame - (frame / 5) * 5;
            if (n_c4 == 2) {
                o_c4 = ex->btlfx_poolSpawn((s16 *)entry->pool);
                if (o_c4 != NULL) {
                    o_c4->unkC.vx = work->unk24.vx;
                    o_c4->unkC.vz = work->unk24.vz;
                    o_c4->unkC.vy = work->unk24.vy + 0x400;
                    o_c4->unk14.vx = -0x400;
                    o_c4->unk14.vy = 0;
                    o_c4->unk14.vz = frame * 0x154;
                    o_c4->unk0 = n_c4;
                    o_c4->unk4.pad = t;
                }
            }
            a_c4_p2 = t >> 6;
            ex->btlfx_drawSpriteAtOt(0xFFE, (u8 *)ex->btlfx_getFileData(entry->file, 3), 0xA0 - a_c4_p2, 0x58, 0, 1, -1);
            ex->btlfx_drawRippleGrid((s32)(env->ot + 0x3E8), 0x28, 1, 3, 0x64, 1, D_801EC888, 0, 0, 0, 0);
            ex->btlfx_drawSpriteAtOt(0xFFE, (u8 *)ex->btlfx_getFileData(entry->file, 0x18), 0xDC - t / 55, 0x78, 0, 1, -1);
            ex->btlfx_drawSpriteAtOt(0xFFE, (u8 *)ex->btlfx_getFileData(entry->file, 0x20), 0x28 - a_c4_p2, 0x28, 0, 1, -1);
            work->unk58 += 4 + ((t * 10) >> 12);
            func_801E7940(0, work->unk58 % 320, 0xA0 - (t >> 8), 0x80, 0x80, 0x80, 0x9C, 0x3E00, 0x80, 0, 1, 0);
            func_801E7940(0xFFF, -((work->unk58 >> 1) % 160), (t >> 9) + 0x8C, 0x80, 0x80, 0x80, 0x9C,
                          0x3E00, 0x80, 0, 0, 1);
            if (frame >= 0x28) {
                work->unk0 = 5;
                entry->frame = -1;
                ex->btlfx_poolClear(entry->pool);
                ex->btlfx_initParticleSystem();
                work->unk24.vy = work->unk8.vy;
                work->unk30.vx = 0x400;
                work->unk30.vy = 0x400;
                work->unk30.vz = 0x400;
                work->unk24.vx = work->unk8.vx;
                work->unk24.vz = work->unk8.vz;
                work->unk24.vy -= 0x280;
                ex->btlfx_startScreenFade(0, 1);
            }
            break;
        }
        case 5:
            switch (frame) {
            default: {
                Fx134Vec3 pos;
                Fx134Vec3 scale;
                SVECTOR rot;
                SVECTOR from;
                SVECTOR to;
                SVECTOR rot2;
                Fx134Bolt *bolt;

                t = (frame << 12) / 23;
                c = (t >> 7) + 0x8D;
                s = (t >> 4) + 0x20;
                ex->btlfx_drawScrollMove(frame * 0x14, 0x1C0, 0x180, 0x18, 0x60, 0x320, 0x100);
                BTLFX_COPY_SVEC(&work->unk18, &work->unk8);

                env->camera->points[3].vx = work->unk18.vx;
                env->camera->points[3].vy = work->unk18.vy;
                env->camera->points[3].vz = work->unk18.vz;
                work->unk3C.vz += 0x18;
                work->unk24.vy += 0x20;
                ex->Hi_DrawEffModel(&work->unk3C, (VECTOR *)&work->unk24, (s32)&work->unk30, work->unk64);
                k_c5 = (t >> 5) - 0x80;
                ex->Hi_ModifyEffModelRGB(work->unk64, k_c5, k_c5, k_c5);
                ex->btlfx_drawBackdrop((BtlBgLayers *)ex->btlfx_getFileData(entry->file, 0x31), work->unk4);
                work->unk44.vx = work->unk44.vy = 0x800;
                work->unk44.vz = 0xAAA;
                work->unk24.vy -= 0x578;
                work->unk50.vz += 0x18;
                ex->Hi_DrawEffModel(&work->unk50, (VECTOR *)&work->unk24, (s32)&work->unk44, work->unk68);
                ex->Hi_ModifyEffModelRGB(work->unk68, k_c5, k_c5, k_c5);
                work->unk50.vz -= 0x155;
                ex->Hi_DrawEffModel(&work->unk50, (VECTOR *)&work->unk24, (s32)&work->unk44, work->unk68);
                ex->Hi_ModifyEffModelRGB(work->unk68, k_c5, k_c5, k_c5);
                work->unk50.vz -= 0x155;
                ex->Hi_DrawEffModel(&work->unk50, (VECTOR *)&work->unk24, (s32)&work->unk44, work->unk68);
                ex->Hi_ModifyEffModelRGB(work->unk68, k_c5, k_c5, k_c5);
                work->unk44.vx = work->unk44.vy = 0x6DB;
                work->unk44.vz = 0x1000;
                work->unk50.vz += 0x2AA;
                work->unk24.vy -= 0x578;
                ex->Hi_DrawEffModel(&work->unk50, (VECTOR *)&work->unk24, (s32)&work->unk44, work->unk6C);
                a_c5 = t >> 2;
                ex->Hi_ModifyEffModelRGB(work->unk6C, (ex->math_sin(a_c5) >> 5) - 0x80, (ex->math_sin(a_c5) >> 5) - 0x80,
                                         (ex->math_sin(a_c5) >> 5) - 0x80);
                work->unk24.vy += 0xAF0;
                yo = 0;
                xo = 0;
                scale.vx = scale.vy = ex->math_sin(a_c5) * 2 / 3 + 0x100;
                scale.vz = 0x555;
                pos.vx = work->unk24.vx;
                a_c5_p2 = t >> 1;
                pos.vy = work->unk24.vy;
                pos.vz = work->unk24.vz;
                pos.vy += 0x80;
                rot.vx = 0x400;
                rot.vy = 0;
                rot.vz = frame << 7;
                for (i_c5 = 0; i_c5 < 3; i_c5++) {
                    ex->Hi_DrawEffModel(&rot, (VECTOR *)&pos, (s32)&scale, work->unk70);
                    ex->Hi_ModifyEffModelRGB(work->unk70, (ex->math_sin(a_c5_p2) >> 5) - 0x80, (ex->math_sin(a_c5_p2) >> 5) - 0x80,
                                             (ex->math_sin(a_c5_p2) >> 5) - 0x80);
                    scale.vx = scale.vy = (scale.vx * 3) >> 2;
                    rot.vz += 0x300;
                    pos.vy += 0x20;
                }
                from.vx = work->unk24.vx;
                from.vy = work->unk24.vy;
                from.vz = work->unk24.vz;
                from.vy += 0x400;
                k_c5_p2 = ex->btlfx_rand();
                for (i_c5_p2 = 0, bolt = D_801EC8F4; i_c5_p2 < 3; i_c5_p2++, bolt++) {
                    n_c5 = ex->btlfx_randCentered(0x400) + 0xC00;
                    BTLFX_COPY_SVEC(&to, &work->unk8);
                    to.vx += ex->math_cos(k_c5_p2) >> 1;
                    to.vz += ex->math_sin(k_c5_p2) >> 1;
                    func_801E8460(entry, bolt, &from, &to, 0x100, 0x200, k_c5_p2, 0xC0, 0, n_c5, (ex->btlfx_rand() & 0xF) + 0xF);
                    k_c5_p2 += 0x554 + (ex->btlfx_rand() & 0xFF);
                }
                rot2.vx = -0x400;
                rot2.vy = 0;
                a_c5_p3 = t >> 2;
                rot2.vz = ex->math_sin(a_c5_p3) >> 3;
                func_801E7E48(&work->unk8, &rot2, (ex->math_sin(a_c5_p3) >> 3) + 0x1000, 0x40, 0x17, 0x3D00, 0, 0x80, a_c5_p3, 1);
                if (frame >= 0x17) {
                    work->unk0 = 6;
                    entry->frame = -1;
                    D_801EC860.vz += 0x400;
                    BTLFX_COPY_SVEC(&work->unk8, &D_801EC860);
                    D_801EC860.vz -= 0x400;
                    work->unk30.vx = 0x2800;
                    work->unk30.vy = 0x2800;
                    work->unk24.vx = work->unk8.vx;
                    work->unk24.vy = work->unk8.vy;
                    work->unk24.vz = work->unk8.vz;
                    work->unk24.vy -= 0x3800;
                    work->unk24.vz -= 0x1C00;
                    work->unk30.vz = 0x1800;
                    work->unk10.vx = work->unk24.vx;
                    work->unk10.vy = work->unk24.vy;
                    work->unk10.vz = work->unk24.vz;
                    ex->btlfx_aimAngles((s16 *)&work->unk8, (s16 *)&work->unk10, (s16 *)&work->unk3C);
                    BTLFX_COPY_SVEC(&work->unk50, &work->unk3C);
                    ex->btlfx_startScreenFade(1, 1);
                    work->unk3C.vz += 0x320;
                }
                break;
            }
            }
            break;
        case 6:
            switch (frame) {
            default: {
                Fx134Vec3 pos;
                Fx134Vec3 scale;
                SVECTOR rot;
                SVECTOR from;
                SVECTOR to;
                Fx134Bolt *bolt;

                t = (frame << 12) / 56;
                c = ((t << 5) >> 12) + 0x8D;
                ex->btlfx_drawScreenTint((u32 *)(env->ot + 2), 1, (t * 0x58) >> 12, 0, 0);
                a1_c6 = t >> 2;
                BTLFX_COPY_SVEC(&work->unk18, &work->unk8);
                work->unk18.vy -= 0x400 + (ex->math_sin(a1_c6) >> 1);
                s = 0x80;
                yo = 0;
                xo = 0;
                env->camera->points[3].vx = work->unk18.vx;
                env->camera->points[3].vy = work->unk18.vy;
                env->camera->points[3].vz = work->unk18.vz;
                work->unk3C.vz += 8;
                work->unk24.vy += 0x9B;
                work->unk24.vz += 0x4D;
                ex->Hi_DrawEffModel(&work->unk3C, (VECTOR *)&work->unk24, (s32)&work->unk30, work->unk64);
                ex->btlfx_drawBackdrop((BtlBgLayers *)ex->btlfx_getFileData(entry->file, 0x31), work->unk4);
                work->unk44.vx = work->unk44.vy = 0x3800;
                work->unk44.vz = 0x2000;
                work->unk24.vy -= 0x578;
                work->unk24.vz += 0x2BC;
                work->unk50.vz += 0x14;
                ex->Hi_DrawEffModel(&work->unk50, (VECTOR *)&work->unk24, (s32)&work->unk44, work->unk68);
                ex->Hi_ModifyEffModelRGB(work->unk68, (((ex->math_cos(a1_c6) >> 1) + 0x800) >> 5) - 0x80,
                                         (((ex->math_cos(a1_c6) >> 1) + 0x800) >> 5) - 0x80,
                                         (((ex->math_cos(a1_c6) >> 1) + 0x800) >> 5) - 0x80);
                work->unk50.vz -= 0x200;
                ex->Hi_DrawEffModel(&work->unk50, (VECTOR *)&work->unk24, (s32)&work->unk44, work->unk68);
                ex->Hi_ModifyEffModelRGB(work->unk68, (((ex->math_cos(a1_c6) >> 1) + 0x800) >> 5) - 0x80,
                                         (((ex->math_cos(a1_c6) >> 1) + 0x800) >> 5) - 0x80,
                                         (((ex->math_cos(a1_c6) >> 1) + 0x800) >> 5) - 0x80);
                work->unk44.vx = work->unk44.vy = 0x2000;
                work->unk50.vz += 0x200;
                work->unk24.vy += 0x578;
                work->unk24.vz -= 0x2BC;
                work->unk44.vz = 0x3000;
                work->unk24.vy -= 0x1388;
                work->unk24.vz -= 0x9C4;
                ex->Hi_DrawEffModel(&work->unk50, (VECTOR *)&work->unk24, (s32)&work->unk44, work->unk6C);
                ex->Hi_ModifyEffModelRGB(work->unk6C, -0x2B, -0x2B, -0x2B);
                work->unk24.vy += 0x1388;
                work->unk24.vz += 0x9C4;
                if (frame > 0x28) {
                    n6 = 0;
                    b_c6 = (frame - 0x28) << 8;
                    scale.vx = scale.vy = (ex->math_sin(b_c6 >> 2) * 5) / 2 + 0x400;
                    k_c6 = b_c6 >> 2;
                    scale.vz = b_c6 + 0x1800;
                    rot.vx = -0x400;
                    rot.vy = 0;
                    pos.vx = D_801EC860.vx;
                    rot.vz = frame * 0x8D;
                    pos.vy = D_801EC860.vy;
                    pos.vz = D_801EC860.vz;
                    pos.vy -= 0x100;
                    do {
                        ex->Hi_DrawEffModel(&rot, (VECTOR *)&pos, (s32)&scale, work->unk70);
                        ex->Hi_ModifyEffModelRGB(work->unk70, (ex->math_sin(k_c6) >> 5) - 0x80, (ex->math_sin(k_c6) >> 5) - 0x80,
                                                 (ex->math_sin(k_c6) >> 5) - 0x80);
                        n6++;
                        scale.vx = scale.vy = (scale.vx * 3) >> 2;
                        rot.vz += 0x400;
                        pos.vy += 0x20;
                    } while (n6 <= 0);
                    scale.vx = scale.vz = (ex->math_sin(b_c6 >> 2) * 5) / 2 + 0x1000;
                    scale.vy = ((b_c6 * 0x300) >> 12) + 0x800;
                    rot.vx = 0;
                    rot.vy = frame * 0x8D;
                    rot.vz = 0;
                    pos.vx = D_801EC860.vx;
                    pos.vy = D_801EC860.vy;
                    pos.vz = D_801EC860.vz;
                    ex->Hi_DrawEffModel(&rot, (VECTOR *)&pos, (s32)&scale, work->unk74);
                    n_c6 = b_c6 >> 1;
                    ex->Hi_ModifyEffModelRGB(work->unk74, (ex->math_sin(n_c6) >> 5) - 0x80, (ex->math_sin(n_c6) >> 5) - 0x80,
                                             (ex->math_sin(n_c6) >> 5) - 0x80);
                }
                from.vx = work->unk24.vx;
                from.vy = work->unk24.vy;
                from.vz = work->unk24.vz;
                from.vy += 0x1838;
                from.vz += 0xC1C;
                for (i_c6 = 0; i_c6 < 2; i_c6++) {
                    r6 = ex->btlfx_rand();
                    d6 = ex->btlfx_randCentered(0x400) + 0xC00;
                    ((s32 *)&to)[0] = work->unk24.vx;
                    ((s32 *)&to)[1] = work->unk24.vy;
                    ex->func_800D044C(&from, (s32)&to, 0x1000, (XY32 *)&work->unk3C, 0x3000, r6);
                    func_801E8460(entry, &D_801EC8F4[i_c6], &from, &to, 0x100, 0x200, r6, 0xC0, 0, d6, (ex->btlfx_rand() & 0x1F) + 0x58);
                }
                if (frame >= 0x38) {
                    work->unk0 = 7;
                    entry->frame = -1;
                    work->unk8.vx = work->unk24.vx;
                    work->unk8.vy = work->unk24.vy;
                    work->unk8.vz = work->unk24.vz;
                    work->unk10.vx = work->unk24.vx;
                    work->unk10.vy = work->unk24.vy;
                    work->unk10.vz = work->unk24.vz;
                }
                break;
            }
            }
            break;
        case 7:
            t = frame << 8;
            c = 0xAD;
            yo = 0;
            xo = 0;
            a_c7 = t >> 2;
            s = a_c7 + 0x80;
            D_801EC894[1].r = (ex->math_sin(a_c7) * 0xFF) >> 12;
            D_801EC894[1].g = (ex->math_sin(a_c7) * 0xFF) >> 12;
            D_801EC894[1].b = (u32)ex->math_sin(a_c7) >> 6;
            ex->btlfx_drawScreenTint((u32 *)(env->ot + 2), 1, 0x58, 0, 0);
            ex->btlfx_drawRippleGrid((s32)(env->ot + 0xFF0), 0x28, 2, 2, 0xC8, 0xFF, D_801EC894, 0, 0, 0, 0);
            if (frame > 8) {
                k_c7 = (frame - 8) * 0xFF / 8;
                ex->btlfx_drawScreenTint((u32 *)(env->ot + 2), 1, k_c7, k_c7, k_c7);
            }
            work->unk24.vy = work->unk8.vy + (t >> 6) + ((((frame & 1) * 2 - 1) * ex->math_cos(t >> 2)) >> 6);
            ex->Hi_DrawEffModel(&work->unk3C, (VECTOR *)&work->unk24, (s32)&work->unk30, work->unk64);
            ex->Hi_ModifyEffModelRGB(work->unk64, ((((ex->math_cos(t >> 2) * 3) >> 2) + 0x400) >> 5) - 0x80,
                                     ((((ex->math_cos(t >> 2) * 3) >> 2) + 0x400) >> 5) - 0x80,
                                     ((((ex->math_cos(t >> 2) * 3) >> 2) + 0x400) >> 5) - 0x80);
            ex->btlfx_drawBackdrop((BtlBgLayers *)ex->btlfx_getFileData(entry->file, 0x31), work->unk4);
            work->unk44.vx = work->unk44.vy = 0x3800;
            work->unk44.vz = 0x2000;
            work->unk24.vy -= 0x578;
            work->unk24.vz += 0x2BC;
            work->unk50.vz += 8;
            ex->Hi_DrawEffModel(&work->unk50, (VECTOR *)&work->unk24, (s32)&work->unk44, work->unk68);
            ex->Hi_ModifyEffModelRGB(work->unk68, (ex->math_cos(t >> 2) >> 6) - 0x80, (ex->math_cos(t >> 2) >> 6) - 0x80,
                                     (ex->math_cos(t >> 2) >> 6) - 0x80);
            work->unk50.vz -= 0x200;
            ex->Hi_DrawEffModel(&work->unk50, (VECTOR *)&work->unk24, (s32)&work->unk44, work->unk68);
            ex->Hi_ModifyEffModelRGB(work->unk68, (ex->math_cos(t >> 2) >> 6) - 0x80, (ex->math_cos(t >> 2) >> 6) - 0x80,
                                     (ex->math_cos(t >> 2) >> 6) - 0x80);
            work->unk44.vx = work->unk44.vy = 0x2000;
            work->unk50.vz += 0x200;
            work->unk24.vy += 0x578;
            work->unk24.vz -= 0x2BC;
            work->unk44.vz = 0x3000;
            work->unk24.vy -= 0x1388;
            work->unk24.vz -= 0x9C4;
            ex->Hi_DrawEffModel(&work->unk50, (VECTOR *)&work->unk24, (s32)&work->unk44, work->unk6C);
            ex->Hi_ModifyEffModelRGB(work->unk6C, (ex->math_cos(t >> 2) >> 5) - 0x80, (ex->math_cos(t >> 2) >> 5) - 0x80,
                                     (ex->math_cos(t >> 2) >> 5) - 0x80);
            work->unk24.vy += 0x1388;
            work->unk24.vz += 0x9C4;
            env->unk28 = 0x898;
            ex->btlfx_drawRings((u8 *)ex->btlfx_getFileData(entry->file, 0x1A), &D_801EC860, NULL, t * 3 + 0x1000, frame, 1,
                              ex->math_sin(t >> 2));
            ex->btlfx_drawRings((u8 *)ex->btlfx_getFileData(entry->file, 0x1B), &D_801EC860, NULL, t * 2 + 0x4000, frame, 1,
                              ex->math_sin(t >> 2));
            env->unk28 = 0;
            if (frame >= 0x10) {
                work->unk0 = 8;
                entry->frame = -1;
                work->unk10.vx = work->unk18.vx;
                work->unk10.vy = work->unk18.vy;
                work->unk10.vz = work->unk18.vz;
                work->unk10.vx -= 0x2800;
                work->unk10.vy += 0xB86;
                ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, 0x21), (s32)&work->unk10);
            }
            break;
        case 8:
            t = frame << 7;
            c = 0xBC;
            s = 0x200;
            yo = 0;
            xo = 0;
            ex->btlfx_drawScreenTint((u32 *)(env->ot + 0xFF8), 0xFF, 0xFF, 0xFF, 0xC8);
            ex->btlfx_drawScreenTint((u32 *)(env->ot + 1), 1, 0x40, 0, 0);
            if (frame >= 0x1A) {
                ex->btlfx_drawScreenTint((u32 *)(env->ot + 1), 1, ((frame - 0x1A) << 7) / 6, ((frame - 0x1A) << 7) / 6,
                                         ((frame - 0x1A) << 7) / 6);
            }
            if (frame >= 0xA) {
                for (i_c8 = 0; i_c8 < 3; i_c8++) {
                    o_c8 = ex->btlfx_poolSpawn((s16 *)entry->pool);
                    if (o_c8 != NULL) {
                        o_c8->unkC.vx = work->unk18.vx - ((t >> 4) + 0x1800) + ex->btlfx_randCentered(0x200);
                        o_c8->unkC.vy = 0x6A4 - ((0x1000 - t) >> 1);
                        o_c8->unkC.vz = work->unk18.vz + ex->btlfx_randCentered(0x1C00);
                        o_c8->unk4.vx = ex->btlfx_randCentered(0x100);
                        o_c8->unk4.vy = -0x140 - (ex->btlfx_rand() & 0xFF);
                        o_c8->unk4.vz = ex->btlfx_randCentered(0x100);
                        o_c8->unk14.vx = ex->btlfx_rand();
                        o_c8->unk14.vy = ex->btlfx_rand();
                        o_c8->unk14.vz = ex->btlfx_rand();
                        o_c8->unk0 = 3;
                        o_c8->unk4.pad = t / 5 + (s16)((ex->btlfx_rand() & 0x3FF) + 0x80);
                    }
                }
            }
            if (frame == 1) {
                ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, 0x22), (s32)&work->unk10);
            }
            if (frame == 4) {
                ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, 0x22), (s32)&work->unk10);
            }
            if (frame >= 0x20) {
                work->unk0 = 0xA;
                entry->frame = -1;
                ex->btlfx_drawScreenTint((u32 *)(env->ot + 2), 1, 0xFF, 0xFF, 0xFF);
                ex->btlfx_poolClear(entry->pool);
                ex->btlfx_initParticleSystem();
                work->unk18.vx = 0;
                work->unk18.vy = -0x200;
                work->unk18.vz = 0;
                env->camera->points[3].vx = work->unk18.vx;
                env->camera->points[3].vy = work->unk18.vy;
                env->camera->points[3].vz = work->unk18.vz;
            }
            break;
        case 10:
            switch (frame) {
            default: {
                CVECTOR col;
                SVECTOR rot;
                Fx134Vec3 scale;

                t = frame << 7;
                k_c9 = 0xF5 - ((t * 0xF5) >> 12);
                ex->btlfx_drawScreenTint((u32 *)(env->ot + 1), 1, k_c9, k_c9, k_c9);
                rot.vx = 0;
                rot.vy = 0;
                k_c9 = 0xA0 - ((frame << 14) >> 12);
                rot.vz = 0x20;
                scale.vx = 0x1100;
                scale.vy = 0x1100;
                scale.vz = 0x1000;
                col.r = col.g = col.b = k_c9;
                ex->btlfx_drawScreenGrid(1, 0xA0, 0x70, 0x50, 0x38, 4, 4, 2, 2, &rot, (VECTOR *)&scale, &col, 1, NULL);
                if (frame >= 0x20) {
                    ex->btlfx_clearEffModels();
                    return 1;
                }
                break;
            }
            }
            break;
        }
        }
        if (c != 0) {
            CVECTOR col;
            SVECTOR rot;
            Fx134Vec3 scale;

            col.r = col.g = col.b = c;
            rot.vx = 0;
            rot.vy = 0;
            rot.vz = 0;
            scale.vy = scale.vx = s + 0x1000;
            scale.vz = 0x1000;
            ex->btlfx_drawScreenGrid(1, xo + 0xA0, yo + 0x78, 0x50, 0x3C, 4, 4, 2, 2, &rot, (VECTOR *)&scale, &col, 0, NULL);
        }
        if ((u32)(work->unk0 - 3) < 5) {
            ex->btlfx_drawScrollMove((0x1F4 - work->unk4) * 8, 0x1E0, 0x100, 0x20, 0x80, 0x300, 0x100);
        }
        work->unk4++;
    }
    return 0;
}
