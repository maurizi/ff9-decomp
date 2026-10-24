#include "fxmod/fx_195.h"

void func_801E7704(Unk80100C78Entry *entry, Fx195Trail *trail, SVECTOR *from, SVECTOR *to, s32 arg4, s32 arg5, s32 arg6,
                   s32 arg7) {
    BtlfxExports *ex = BTLFX_EXPORTS(D_801EA278);
    SVECTOR pos;
    SVECTOR rot;
    SVECTOR *p;
    s32 i;
    s32 t;
    s32 h;

    ex->btlfx_aimAngles((s16 *)from, (s16 *)to, (s16 *)&rot);
    i = 0;
    p = trail->unk4;
    trail->unk0 = trail->unk2 = 0xC;
    trail->unk1 = 0;
    do {
        t = 0x1000 - (i << 12) / 12;
        t += ex->btlfx_randCentered(0x180);
        ex->btlfx_lerpVec((s16 *)from, (s16 *)to, t, (s16 *)&pos);
        h = ex->btlfx_randCentered(0x80);
        i++;
        h = (ex->math_sin(t >> 1) * h) >> 12;
        h += ex->btlfx_lerp(arg4, arg5, t);
        ex->func_800D044C(&pos, (s32)p, 0, (XY32 *)&rot, h, arg6);
        p->pad = 0x1A;
        arg6 += arg7 + ex->btlfx_randCentered(0x100);
        p++;
    } while (i < 13);
}

void func_801E78A4(Unk80100C78Entry *entry, Fx195Trail57 *trail, SVECTOR *from, SVECTOR *to, s32 arg4, s32 arg5,
                   s32 arg6, s32 arg7, s32 arg8, s32 arg9, s32 arg10) {
    BtlfxExports *ex = BTLFX_EXPORTS(D_801EA278);
    SVECTOR pos;
    SVECTOR rot;
    SVECTOR *p;
    s32 i;
    s32 t;
    s32 h;

    ex->btlfx_aimAngles((s16 *)from, (s16 *)to, (s16 *)&rot);
    i = 0;
    p = trail->unk4;
    trail->unk0 = trail->unk2 = 0x38;
    trail->unk1 = 0;
    do {
        t = 0x1000 - (i << 12) / 56;
        t += ex->btlfx_randCentered(0x100);
        ex->btlfx_lerpVec((s16 *)from, (s16 *)to, t, (s16 *)&pos);
        h = ex->btlfx_randCentered(0xA0);
        i++;
        h = (ex->math_sin(t >> 1) * h) >> 12;
        h += ex->btlfx_lerp(arg4, arg5, t);
        ex->func_800D044C(&pos, (s32)p, 0, (XY32 *)&rot, h, arg6);
        p->pad = arg10;
        arg6 += arg7 + ex->btlfx_randCentered(0x80);
        p++;
    } while (i < 57);
    ex->btlfx_drawTrail((u8 *)ex->btlfx_getFileData(entry->unk8, 0x18), 0, NULL, arg8, 0x1000, arg9, 1, (u8 *)trail, 0);
}

void func_801E7AA0(Unk80100C78Entry *entry, Fx195Trail *trail, SVECTOR *from, SVECTOR *to, s32 arg4, s32 arg5, s32 arg6,
                   s32 arg7, s32 arg8, s32 arg9, s32 arg10) {
    BtlfxExports *ex = BTLFX_EXPORTS(D_801EA278);
    SVECTOR pos;
    SVECTOR rot;
    SVECTOR *p;
    s32 i;
    s32 t;
    s32 h;

    ex->btlfx_aimAngles((s16 *)from, (s16 *)to, (s16 *)&rot);
    i = 0;
    p = trail->unk4;
    trail->unk0 = trail->unk2 = 0xC;
    trail->unk1 = 0;
    do {
        t = 0x1000 - (i << 12) / 12;
        t += ex->btlfx_randCentered(0x100);
        ex->btlfx_lerpVec((s16 *)from, (s16 *)to, t, (s16 *)&pos);
        h = ex->btlfx_randCentered(0xA0);
        i++;
        h = (ex->math_sin(t >> 1) * h) >> 12;
        h += ex->btlfx_lerp(arg4, arg5, t);
        ex->func_800D044C(&pos, (s32)p, 0, (XY32 *)&rot, h, arg6);
        p->pad = arg10;
        arg6 += arg7 + ex->btlfx_randCentered(0x80);
        p++;
    } while (i < 13);
    ex->btlfx_drawTrail((u8 *)ex->btlfx_getFileData(entry->unk8, 0x19), 0, NULL, arg8, 0x1000, arg9, 1, (u8 *)trail, 0);
}

void func_801E7C98(SVECTOR *pos, SVECTOR *rot, VECTOR *scale, s32 r0, s32 r1, s32 w0, s32 w1, s32 c, s32 tpage,
                   s32 clut, s32 u, s32 v, s32 ang, s32 dy, s32 abr, s32 dang) {
    BtlfxExports *ex = BTLFX_EXPORTS(D_801EA278);
    Fx195Rgbc c0;
    Fx195Rgbc c1;
    Fx195Rgbc c2;
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

    gte_SetRotMatrix(D_801EA278->viewMatrix);
    gte_SetTransMatrix(D_801EA278->viewMatrix);
    gte_ldv0(pos);
    gt4 = (POLY_GT4 *)D_801EA278->prim;
    *(s32 *)&c2 = 0;
    ft4 = (POLY_FT4 *)(gt4 + 2);
    D_801EA278->prim = (u8 *)(ft4 + 46);
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
    d = dy * 3;
    for (row = 0; row < 3; row++, h += dy) {
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
            t = k & 7;
            u0 = u + (t << 4);
            u1 = u0 + 0x10;
            if (t == 7) {
                u1 = u0 + 0xF;
            }
            v0 = v;
            v1 = v0 + 0x3F;
            gte_nclip();
            if (k == 0) {
                ft = 0;
                *(s32 *)&gt4->r0 = *(s32 *)&gt4->r1 = *(s32 *)&c2;
                *(s32 *)&gt4->r2 = *(s32 *)&gt4->r3 = *(s32 *)&c1;
            } else if (k >= 47) {
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

void func_801E83BC(VECTOR *pos, SVECTOR *rot) {
    Unk800F78A4 *env = D_801EA278;
    BtlfxExports *ex = BTLFX_EXPORTS(env);
    SVECTOR v;
    s32 tmp;
    s32 a;

    v.vx = pos->vx;
    v.vy = pos->vy;
    v.vz = pos->vz;
    ex->func_800D0674(&env->camera->eye, &v, rot, (s32)&tmp, (s32)&tmp, &a);
    rot->vz = -a + 0x400;
}

s32 fx_195_op80(s32 mode, Fx195Work80 *work, Unk800F78A4 *env, Unk80100C78Entry *entry, s32 *arg4) {
    BtlfxExports *ex = BTLFX_EXPORTS(env);
    SVECTOR rot;
    VECTOR sc;
    CVECTOR col;
    SVECTOR rot2;
    VECTOR sc2;
    SVECTOR v1;
    SVECTOR v2;
    SVECTOR r3;
    s32 frame;
    s32 t;
    s32 tint1;
    s32 tint2;
    Pool6CEntry *p;

    if (mode == 0) {
        *arg4 = 0x98;
    } else if (mode == 1) {
        work->unk0 = 0;
        work->unk4 = 0;
        D_801EA278 = env;
        ex->btlfx_initEffModels(D_801EA288, 7);
        work->unk80 = ex->btlfx_registerEffectModel((Unk800E8F50 *)ex->btlfx_getFileData(entry->unk8, 0x11), 1);
        work->unk84 = ex->btlfx_registerEffectModel((Unk800E8F50 *)ex->btlfx_getFileData(entry->unk8, 0x14), 1);
        work->unk5C = ex->btlfx_registerEffectModel((Unk800E8F50 *)ex->btlfx_getFileData(entry->unk8, 0x1C), 0);
        work->unk88 = ex->btlfx_registerEffectModel((Unk800E8F50 *)ex->btlfx_getFileData(entry->unk8, 0x16), 1);
        work->unk8C = ex->btlfx_registerEffectModel((Unk800E8F50 *)ex->btlfx_getFileData(entry->unk8, 0x15), 0);
        work->unk90 = ex->btlfx_registerEffectModel((Unk800E8F50 *)ex->btlfx_getFileData(entry->unk8, 0x1A), 0);
        work->unk94 = ex->btlfx_registerEffectModel((Unk800E8F50 *)ex->btlfx_getFileData(entry->unk8, 0x13), 1);
        BTLFX_COPY_SVEC(&work->unk8, &D_801EA228);
        BTLFX_COPY_SVEC(&work->unk20, &D_801EA228);
        work->unk20.vy -= 0x980;
        work->unk10.vx = D_801EA228.vx;
        work->unk10.vy = D_801EA228.vy;
        work->unk10.vz = D_801EA228.vz;
        work->unk78.vx = -0x400;
        work->unk78.vy = 0;
        work->unk78.vz = 0;
    } else {
        tint1 = 0;
        tint2 = 0;
        frame = entry->unk0;
        {
            /* Dead struct initializer: it only emits a CLOBBER of q, whose spill slot is the original's otherwise
             * unused word at 0x140 (it shifts the case 5 spill slots up by one). */
            struct {
                s32 x;
            } q = { frame };
        }
        switch (work->unk0) {
        case 0:
            env->camera->points[3].vx = work->unk20.vx;
            env->camera->points[3].vy = work->unk20.vy;
            env->camera->points[3].vz = work->unk20.vz;
            if (frame >= 2) {
                work->unk0 = 4;
                entry->unk0 = -1;
                work->unk44 = 0;
            }
            break;
        case 4: {
            s32 k;
            s32 kk;
            s32 v;
            s32 w1;
            s32 w1_200;
            s32 w2;
            s32 w2_202;
            s32 u;
            s32 u4;
            s32 c;
            s32 a;
            s32 i;
            s32 i_1;
            s32 ang;
            s32 r;

            t = (frame << 12) / 14;
            tint2 = ex->math_cos(t >> 2);
            if (frame == 0) {
                work->unk10.vx = D_801EA228.vx;
                work->unk10.vy = D_801EA228.vy;
                work->unk10.vz = D_801EA228.vz;
                work->unk60.vx = D_801EA228.vx;
                work->unk60.vy = D_801EA228.vy;
                work->unk60.vz = D_801EA228.vz;
            }
            if (frame < 4) {
                tint1 = 0x1000 - ((frame << 12) >> 2);
                work->unk6C.vx = work->unk6C.vy = 0x1200;
                work->unk6C.vz = 0x1555;
            } else {
                kk = frame - 4;
                if (frame == 4) {
                    work->unk6C.vx = work->unk6C.vy = 0x4000;
                } else if (kk < 11) {
                    kk = (kk << 12) / 10;
                    v = ((0x1000 - kk) * 3) >> 1;
                    work->unk6C.vx = work->unk6C.vy = v;
                }
            }
            if (frame < 5) {
                work->unk20.vy = work->unk10.vy;
                work->unk20.vx = work->unk10.vx;
                work->unk20.vz = work->unk10.vz;
                work->unk20.vy += -0x180 - ((0x1000 - ((frame << 12) >> 2)) >> 1);
                env->camera->points[3].vx = work->unk20.vx;
                env->camera->points[3].vy = work->unk20.vy;
                env->camera->points[3].vz = work->unk20.vz;
            }
            if (frame == 4) {
                ex->btlfx_drawScreenTint((u32 *)(env->ot + 2), 1, 0x20, 0x80, 0xFF);
            } else if ((u32)(frame - 5) < 7) {
                u = 0x1000 - ((frame - 5) << 12) / 7;
                r = u >> 5;
                work->unk20.vx = D_801EA228.vx + ex->btlfx_randCentered(r);
                work->unk20.vy = D_801EA228.vy + ex->btlfx_randCentered(r);
                work->unk20.vy = D_801EA228.vy - 0x200 + (u >> 6) * ((frame & 1) * 2 - 1);
                env->camera->points[3].vx = work->unk20.vx;
                env->camera->points[3].vy = work->unk20.vy;
                env->camera->points[3].vz = work->unk20.vz;
            }
            if (work->unk6C.vx != 0) {
                func_801E83BC((VECTOR *)&work->unk60, &work->unk78);
                ex->Hi_DrawEffModel(&work->unk78, (VECTOR *)&work->unk60, (s32)&work->unk6C, work->unk5C);
            }
            if (frame >= 4) {
                u4 = ((frame - 4) << 12) / 10;
                rot.vx = 0;
                rot.vz = 0;
                rot.vy = u4 >> 3;
                sc.vx = sc.vz = (u4 >> 2) + 0x555;
                sc.vy = (u4 >> 1) + 0x400;
                c = ex->math_cos(u4 >> 2);
                ex->Hi_DrawEffModel(&rot, (VECTOR *)&work->unk60, (s32)&sc, work->unk80);
                ex->Hi_ModifyEffModelRGB(work->unk80, (c >> 5) - 0x80, (c >> 5) - 0x80, (c >> 5) - 0x80);
                sc.vx = sc.vz = sc.vx * 2;
                sc.vy >>= 2;
                ex->Hi_DrawEffModel(&rot, (VECTOR *)&work->unk60, (s32)&sc, work->unk80);
                ex->Hi_ModifyEffModelRGB(work->unk80, (c >> 6) - 0x80, (c >> 6) - 0x80, (c >> 6) - 0x80);
            }
            if (frame == 0) {
                work->unk54 = ex->func_800E3CEC(ex->btlfx_getFileData(entry->unk8, 0xE), (s32)&work->unk10);
                work->unk58 = ex->func_800E3CEC(ex->btlfx_getFileData(entry->unk8, 0xC), (s32)&work->unk10);
                ex->func_800E3CEC(ex->btlfx_getFileData(entry->unk8, 0xD), (s32)&work->unk10);
            }
            col.r = col.g = col.b = 0x37;
            sc2.vx = 0x1400;
            sc2.vy = 0x1400;
            sc2.vz = 0x1000;
            rot2.vx = 0;
            rot2.vy = 0;
            rot2.vz = 0;
            ex->btlfx_drawScreenGrid(3, 0xA0, 0x70, 0x50, 0x38, 4, 4, 2, 2, &rot2, &sc2, &col, 1, NULL);
            BTLFX_COPY_SVEC(&v1, &work->unk10);
            ex->func_800CFD78((s16 *)&v1, (s16 *)&work->unk78, t - 0x2800, (s16 *)&v2);
            r = (t >> 2) + 0x400;
            for (i = 0, ang = work->unk4 << 8; i < 3; i++, ang += 0x554) {
                func_801E78A4(entry, &D_801EAB08[i], &v1, &v2, r, r, ang, 0xC0, 0, ex->btlfx_randCentered(0x400) + 0xC00,
                              0x84);
            }
            env->unk28 = 0x200;
            w1_200 = (ex->math_sin(t >> 2) << 1) + 0x1000;
            w2_202 = (ex->math_cos(t >> 2) >> 1) + 0x800;
            ex->func_800D23D8((u8 *)ex->btlfx_getFileData(entry->unk8, 9), &work->unk10, NULL, w1_200, 0, 1, w2_202);
            w2 = ex->math_sin(t >> 2) * 3 + 0x1000;
            w1 = ex->math_cos(t >> 2);
            ex->func_800D23D8((u8 *)ex->btlfx_getFileData(entry->unk8, 8), &work->unk10, NULL, w2, 0, 1, w1);
            if (frame >= 4 && (k = frame - 4) < 10) {
                r3.vx = 0x400;
                r3.vy = 0;
                r3.vz = 0;
                r3.pad = 1;
                ex->func_800D23D8((u8 *)ex->btlfx_getFileData(entry->unk8, 0xA), &work->unk10, &r3, 0x4000, k, 1, 0x1000);
            }
            env->unk28 = 0;
            if (frame >= 14) {
                work->unk0 = 5;
                entry->unk0 = -1;
                ex->func_800E3D54(work->unk54);
                ex->func_800E3D54(work->unk58);
                ex->btlfx_drawScreenTint((u32 *)(env->ot + 1), 1, 0, 0x80, 0xFF);
                p = ex->func_800E3CEC(ex->btlfx_getFileData(entry->unk8, 0xF), (s32)&work->unk10);
                if (p != NULL) {
                    p->unk22 = 0x300;
                }
                p = ex->func_800E3CEC(ex->btlfx_getFileData(entry->unk8, 0xB), (s32)&work->unk10);
                if (p != NULL) {
                    p->unk22 = 0x300;
                }
                p = ex->func_800E3CEC(ex->btlfx_getFileData(entry->unk8, 0x17), (s32)&work->unk10);
                if (p != NULL) {
                    p->unk22 = 0x400;
                }
                work->unk30 = ex->btlfx_playFileSound(0x22, entry->unk8, -1);
                work->unk32 = ex->btlfx_playFileSound(0x23, entry->unk8, -1);
                ex->btlfx_playFileSound(0x20, entry->unk8, -1);
                ex->btlfx_playFileSound(0x21, entry->unk8, -1);
                env->camera->points[2].vx = 0;
                env->camera->points[2].vy = -0xC8;
                env->camera->points[2].vz = 0x300;
            }
            break;
        }
        case 5: {
            VECTOR sc;
            SVECTOR rotA;
            VECTOR scA;
            VECTOR posA;
            SVECTOR rotC;
            VECTOR scC;
            VECTOR posC;
            SVECTOR rotF;
            VECTOR scF;
            VECTOR posF;
            SVECTOR v1;
            SVECTOR v2;
            SVECTOR rotG;
            s32 c;
            s32 c_321;
            s32 c_359;
            s32 c_407;
            s32 v;
            s32 b;
            s32 b2;
            s32 s;
            s32 s_421;
            s32 a;
            s32 a_331;
            s32 a_340;
            s32 a_405;
            s32 d;
            s32 e;
            s32 f;
            s32 g;
            s32 g_356;
            s32 i;
            s32 i_1;
            s32 k;
            s32 n;
            s32 ang;
            s32 r;
            s32 r_375;
            s32 p1;
            s32 p2;

            t = (frame << 12) / 77;
            if (frame == 0) {
                work->unk10.vx = D_801EA228.vx;
                work->unk10.vy = D_801EA228.vy;
                work->unk10.vz = D_801EA228.vz;
                work->unk60.vx = D_801EA228.vx;
                work->unk60.vy = D_801EA228.vy;
                work->unk60.vz = D_801EA228.vz;
                work->unk78.vx = -0x400;
                work->unk78.vy = 0;
                work->unk78.vz = 0;
                work->unk6C.vx = 0x4000;
                work->unk6C.vy = 0x4000;
                work->unk6C.vz = 0x4000;
            }
            work->unk20.vx = work->unk10.vx;
            work->unk20.vy = work->unk10.vy;
            work->unk20.vz = work->unk10.vz;
            work->unk20.vy = -0x2D4 - ((t * 0x76C) >> 12);
            env->camera->points[3].vx = work->unk20.vx;
            env->camera->points[3].vy = work->unk20.vy;
            env->camera->points[3].vz = work->unk20.vz;
            if (frame < 2) {
                ex->btlfx_drawScreenTint((u32 *)(env->ot + 2), 2, 0xFF - (frame << 7), 0xFF - (frame << 7), 0xFF);
            } else if (frame == 2) {
                ex->btlfx_drawScreenTint((u32 *)(env->ot + 2), 1, 0xFF, 0xFF, 0xFF);
            } else if (frame < 0x13) {
                c = 0x1000 - (((frame - 3) << 12) >> 4);
                c = (c * 0xA0) >> 12;
                ex->btlfx_drawScreenTint((u32 *)(env->ot + 2), 1, c, c, c);
            }
            if (frame == 0x14) {
                ex->btlfx_drawScreenTint((u32 *)(env->ot + 2), 1, 0xD7, 0x64, 0x64);
            }
            if (frame == 0x1D) {
                ex->btlfx_drawScreenTint((u32 *)(env->ot + 2), 1, 0x97, 0x4D, 0x4D);
            }
            if (frame == 0x20) {
                ex->btlfx_drawScreenTint((u32 *)(env->ot + 2), 1, 0x80, 0x40, 0x40);
            }
            if (frame < 0x49) {
                if (frame < 7) {
                    v = ex->math_sin((frame << 10) / 7);
                    b2 = 0x2000;
                } else {
                    v = (frame - 7) * 0x1124 / 65 * 7;
                    /* Scheduling barriers (LOOP notes): b2 is set after the multiply chain (so it gets v1), and s is
                     * computed before frame's reload below. */
                    do {
                    } while (0);
                    b2 = 0x7800;
                }
                s = b2 - v;
                do {
                } while (0);
                if ((u32)(frame - 5) < 7) {
                    c_321 = ex->math_sin(((frame - 5) << 11) >> 12) * 0xF5;
                    ex->btlfx_drawScreenTint((u32 *)(env->ot + 0x400), 1, 0, c_321 >> 14, c_321 >> 12);
                }
                work->unk6C.vx = work->unk6C.vy = s;
                func_801E83BC((VECTOR *)&work->unk60, &work->unk78);
                work->unk78.vz -= 0x400;
                ex->Hi_DrawEffModel(&work->unk78, (VECTOR *)&work->unk60, (s32)&work->unk6C, work->unk88);
                ex->Hi_DrawEffModel(&work->unk78, (VECTOR *)&work->unk60, (s32)&work->unk6C, work->unk8C);
            }
            if (frame >= 0x3B) {
                a_331 = ((frame - 0x3B) << 12) / 18;
                work->unk6C.vx = work->unk6C.vy = 0x1000;
                func_801E83BC((VECTOR *)&work->unk60, &work->unk78);
                work->unk78.vz += 0x400;
                ex->Hi_DrawEffModel(&work->unk78, (VECTOR *)&work->unk60, (s32)&work->unk6C, work->unk90);
                ex->Hi_ModifyEffModelRGB(work->unk90, (ex->math_cos(a_331 >> 2) >> 5) - 0x80,
                                         (ex->math_cos(a_331 >> 2) >> 5) - 0x80, (ex->math_cos(a_331 >> 2) >> 5) - 0x80);
            }
            a_340 = t >> 2;
            b = (t >> 1) + 0xE00;
            c = ex->math_cos(a_340);
            sc.vx = 0x1000;
            sc.vy = 0x1000;
            sc.vz = 0x1000;
            e = (c >> 2) + 0x100;
            r3.vx = 0x400;
            r3.vy = 0;
            r3.vz = 0;
            d = (((t >> 3) + 0x500) * 4) / 3;
            f = ex->math_cos(a_340) >> 5;
            func_801E7C98(&work->unk10, &r3, &sc, b, a_340 + 0xA00, e >> 1, e, f, 0x18, 0x3E81, 0, 0xC0, -frame << 7, d, 1,
                          1);
            func_801E7C98(&work->unk10, &r3, &sc, b, a_340 + 0xA00, e >> 1, e, f, 0x18, 0x3E81, 0, 0xC0, frame * 0xA0, d, 1,
                          -1);
            if (frame < 0x41) {
                g_356 = frame << 6;
                s_421 = g_356 >> 2;
                c_359 = ex->math_cos(s_421) - 0x5DC;
                rotA.vx = 0;
                rotA.vz = 0;
                rotA.vy = frame << 5;
                scA.vx = scA.vz = ((ex->math_sin(s_421) * 3) >> 2) + 0xA00;
                scA.vy = (g_356 + 0x300) * 2;
                posA.vx = work->unk10.vx;
                posA.vy = work->unk10.vy;
                posA.vz = work->unk10.vz;
                posA.vy -= (ex->math_sin(s_421) * 0xFA0) >> 12;
                ex->Hi_DrawEffModel(&rotA, &posA, (s32)&scA, work->unk84);
                ex->Hi_ModifyEffModelRGB(work->unk84, (c_359 >> 5) - 0x80, (c_359 >> 5) - 0x80, (c_359 >> 5) - 0x80);
            }
            rotC.vx = 0;
            rotC.vz = 0;
            rotC.vy = frame << 6;
            r_375 = 0x4000;
            posC.vx = work->unk10.vx;
            posC.vy = work->unk10.vy;
            posC.vz = work->unk10.vz;
            for (i_1 = 0, n = 7; i_1 < 6; n += 7, i_1++) {
                k = frame - n;
                if ((u32)k < 0x20) {
                    p2 = ex->math_cos((k << 12) >> 7);
                    scC.vx = scC.vz = r_375;
                    if (k < 4) {
                        scC.vy = ex->math_sin((k << 10) >> 2) * 6;
                    } else {
                        scC.vy = 0x6000 - ((k - 4) << 8);
                    }
                    scC.vy = (scC.vy * (8 - i_1)) >> 3;
                    if (i_1 < 2) {
                        scC.vy = (scC.vy * 5) >> 2;
                    }
                    ex->Hi_DrawEffModel(&rotC, &posC, (s32)&scC, work->unk94);
                    ex->Hi_ModifyEffModelRGB(work->unk94, (p2 >> 5) - 0x80, (p2 >> 5) - 0x80, (p2 >> 5) - 0x80);
                }
                r_375 = (r_375 * 9) >> 3;
                rotC.vy = (rotC.vy * 3) >> 2;
            }
            if (frame < 0x10) {
                g = frame << 12;
                rotF.vy = frame * 0x37;
                rotF.vx = 0;
                rotF.vz = 0;
                scF.vx = scF.vz = (g >> 5) + 0x1200;
                a_405 = g >> 6;
                scF.vy = ex->math_sin(a_405) + 0x1800;
                c_407 = ex->math_cos(a_405);
                posF.vx = work->unk10.vx;
                posF.vy = work->unk10.vy;
                posF.vz = work->unk10.vz;
                ex->Hi_DrawEffModel(&rotF, &posF, (s32)&scF, work->unk80);
                ex->Hi_ModifyEffModelRGB(work->unk80, (c_407 >> 5) - 0x80, (c_407 >> 5) - 0x80, (c_407 >> 5) - 0x80);
            }
            if (frame == 0x3C) {
                ex->btlfx_replaceSound(0x24, entry->unk8, -1, work->unk30);
                ex->btlfx_replaceSound(0x25, entry->unk8, -1, work->unk32);
            }
            BTLFX_COPY_SVEC(&v1, &work->unk10);
            BTLFX_COPY_SVEC(&v2, &v1);
            v2.vy -= 0x1800;
            s_421 = 0x1000 - ((t * 0xDAC) >> 12);
            ang = (work->unk4 << 9) + (ex->btlfx_rand() & 0x1FF);
            r = (ex->math_cos(t >> 2) * 0xF5) >> 12;
            for (i = 0; i < 2; i++, ang += 0x800) {
                func_801E78A4(entry, &D_801EAB08[i], &v1, &v2, (s_421 * 3) >> 2, s_421, ang, 0xC0, 0, 0x1000, r);
            }
            if (frame < 0x26) {
                s_421 = (s_421 * 3) >> 1;
                r = (ex->math_cos(t >> 1) * 0xF5) >> 12;
                func_801E78A4(entry, &D_801EAEA0, &v1, &v2, (s_421 * 3) >> 2, s_421, -ang, 0xC0, 0, 0x1000, r);
            }
            s = 0x1000 - t;
            env->unk28 = 0x300;
            ang = work->unk4 << 7;
            r = ((ex->math_cos(t >> 2) * 0xEB) >> 12) + 0x20;
            for (i = 0; i < 5; i++, ang += ex->btlfx_randCentered(0x20) + 0x333) {
                v1.vx = work->unk10.vx;
                v1.vy = work->unk10.vy;
                v1.vz = work->unk10.vz;
                v1.vx += (ex->math_sin(ang) * s) >> 12;
                v1.vz += (ex->math_cos(ang) * s) >> 12;
                v2.vx = work->unk10.vx;
                v2.vy = work->unk10.vy;
                v2.vz = work->unk10.vz;
                v2.vx += ex->math_sin(ang);
                v2.vz += ex->math_cos(ang);
                v2.vy -= 0xF00;
                func_801E7AA0(entry, &D_801EA808[i], &v2, &v1, 0x80, 0x200, ang, 0xFF, 0, ex->math_cos(t >> 2), r);
            }
            env->unk28 = 0;
            if (frame == 0x31) {
                BTLFX_COPY_SVEC(&work->unk18, &work->unk10);
                work->unk18.vy -= 0x400;
                p = ex->func_800E3CEC(ex->btlfx_getFileData(entry->unk8, 0x10), (s32)&work->unk18);
                if (p != NULL) {
                    p->unk22 = 0x180;
                }
            }
            env->unk28 = 0x300;
            rotG.vx = 0x400;
            rotG.vy = 0;
            rotG.vz = 0;
            rotG.pad = 1;
            a = t >> 2;
            p1 = ex->math_cos(a) + 0x1800;
            p2 = ((ex->math_cos(a) * 3) >> 2) + 0x400;
            ex->func_800D23D8((u8 *)ex->btlfx_getFileData(entry->unk8, 9), &work->unk10, NULL, p1, 0, 1, p2);
            p1 = 0x5000 - (t << 1);
            p2 = ex->math_cos(a);
            ex->func_800D23D8((u8 *)ex->btlfx_getFileData(entry->unk8, 2), &work->unk10, &rotG, p1, 0, 1, p2);
            env->unk28 = 0;
            ex->btlfx_drawBackdrop((BtlBgLayers *)ex->btlfx_getFileData(entry->unk8, 7), frame);
            ex->btlfx_drawScrollMove(work->unk4 * 5, 0x200, 0x100, 0x20, 0xC0, 0x220, 0x100);
            if (frame >= 0x4D) {
                ex->btlfx_clearEffModels();
                return 1;
            }
            break;
        }
        }
        if (tint1 != 0) {
            ex->btlfx_drawScreenTint((u32 *)(env->ot + 0xFFE), 0xFF, 0, 0, (tint1 * 0x4D) >> 12);
        }
        if (tint2 != 0) {
            s32 x = tint2 << 7;
            s32 y = x >> 12;

            if (work->unk4 % 6 == 0) {
                ex->btlfx_drawScreenTint((u32 *)(env->ot + 2), 1, 0, (y * 2) / 3, y);
            }
            if (((work->unk4 - 3) & 7) == 0) {
                ex->btlfx_drawScreenTint((u32 *)(env->ot + 2), 2, x >> 13, x >> 13, 0);
            }
        }
        work->unk4++;
    }
    return 0;
}

INCLUDE_RODATA("asm/us/fx_195/nonmatchings/fx_195", fx_195_headerPtr);
