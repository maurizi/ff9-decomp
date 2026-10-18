#include "fxmod/fx_0e3_1.h"

void func_801EC71C(Unk80100C78Entry *entry, Fx0e31Bolt *bolt, SVECTOR *from, SVECTOR *to, s32 amp0, s32 amp1, s32 ang,
                   s32 dang, s32 arg8, s32 arg9) {
    BtlfxExports *ex = BTLFX_EXPORTS(D_801F0A04);
    SVECTOR p;
    SVECTOR rot;
    SVECTOR *pt;
    s32 i;
    s32 t;
    s32 a;

    ex->btlfx_aimAngles((s16 *)from, (s16 *)to, (s16 *)&rot);
    pt = bolt->unk4;
    bolt->unk2 = 0xC;
    bolt->unk0 = 0xC;
    bolt->unk1 = 0;
    for (i = 0; i < 13; i++) {
        t = 0x1000 - (i << 12) / 12;
        t += ex->btlfx_randCentered(0x180);
        ex->btlfx_lerpVec((s16 *)from, (s16 *)to, t, (s16 *)&p);
        a = ex->btlfx_randCentered(0x80);
        a = (ex->math_sin(t >> 1) * a) >> 12;
        a += ex->btlfx_lerp(amp0, amp1, t);
        ex->func_800D044C(&p, (s32)pt, 0, (XY32 *)&rot, a, ang);
        pt->pad = 0x1C;
        ang += dang + ex->btlfx_randCentered(0x100);
        pt++;
    }
    ex->btlfx_drawTrail((u8 *)ex->btlfx_getFileData(entry->unk8, 0x15), 0, 0, arg8, 0x1000, arg9, 1, (u8 *)bolt, 0);
}

void func_801EC910(Unk80100C78Entry *entry, Fx0e31Bolt *bolt, SVECTOR *from, SVECTOR *to, s32 amp, s32 ang, s32 dang,
                   s32 arg7, s32 arg8) {
    BtlfxExports *ex = BTLFX_EXPORTS(D_801F0A04);
    SVECTOR p;
    SVECTOR rot;
    SVECTOR *pt;
    s32 i;
    s32 t;
    s32 a;

    ex->btlfx_aimAngles((s16 *)from, (s16 *)to, (s16 *)&rot);
    pt = bolt->unk4;
    bolt->unk2 = 4;
    bolt->unk0 = 4;
    bolt->unk1 = 0;
    for (i = 0; i < 5; i++) {
        t = 0x1000 - (i << 10);
        t += ex->btlfx_randCentered(0x80);
        ex->btlfx_lerpVec((s16 *)from, (s16 *)to, t, (s16 *)&p);
        a = ex->btlfx_randCentered(0x20);
        a = (ex->math_sin(t >> 1) * a) >> 12;
        a += (ex->math_sin(t >> 1) * amp) >> 12;
        ex->func_800D044C(&p, (s32)pt, 0, (XY32 *)&rot, a, ang);
        pt->pad = 0x1A;
        ang += dang + ex->btlfx_randCentered(0xC0);
        pt++;
    }
    ex->btlfx_drawTrail((u8 *)ex->btlfx_getFileData(entry->unk8, 0x11), 0, 0, arg7, 0x1000, arg8, 1, (u8 *)bolt, 0);
}

void func_801ECAF8(Unk80100C78Entry *entry, Fx0e31Bolt *bolt, SVECTOR *from, SVECTOR *to, s32 amp0, s32 amp1, s32 ang,
                   s32 dang, s32 arg8, s32 arg9, s32 col) {
    BtlfxExports *ex = BTLFX_EXPORTS(D_801F0A04);
    SVECTOR p;
    SVECTOR rot;
    SVECTOR *pt;
    s32 i;
    s32 t;
    s32 a;

    ex->btlfx_aimAngles((s16 *)from, (s16 *)to, (s16 *)&rot);
    pt = bolt->unk4;
    bolt->unk2 = 56;
    bolt->unk0 = 56;
    bolt->unk1 = 0;
    for (i = 0; i < 56 + 1; i++) {
        t = 0x1000 - (i << 12) / 56;
        t += ex->btlfx_randCentered(0x100);
        ex->btlfx_lerpVec((s16 *)from, (s16 *)to, t, (s16 *)&p);
        a = ex->btlfx_randCentered(0xA0);
        a = (ex->math_sin(t >> 1) * a) >> 12;
        a += ex->btlfx_lerp(amp0, amp1, t);
        ex->func_800D044C(&p, (s32)pt, 0, (XY32 *)&rot, a, ang);
        pt->pad = col;
        ang += dang + ex->btlfx_randCentered(0x80);
        pt++;
    }
    ex->btlfx_drawTrail((u8 *)ex->btlfx_getFileData(entry->unk8, 0xA), 0, 0, arg8, 0x1000, arg9, 1, (u8 *)bolt, 0);
}

void func_801ECCF4(Unk80100C78Entry *entry, Fx0e31Bolt *src, Fx0e31Bolt *dst, s32 arg3) {
    BtlfxExports *ex = BTLFX_EXPORTS(D_801F0A04);
    SVECTOR p;
    SVECTOR *a = src->unk4;

    a += ex->btlfx_rand() % 56;

    ex->btlfx_lerpVec((s16 *)a, (s16 *)(a + 1), 0x4000, (s16 *)&p);
    func_801EC910(entry, dst, a, &p, 0x80, ex->btlfx_rand(), 0x37, 0, arg3);
}

void func_801ECDEC(Unk80100C78Entry *entry, Fx0e31Bolt *bolt, SVECTOR *from, SVECTOR *to, s32 amp0, s32 amp1, s32 ang,
                   s32 dang, s32 arg8, s32 arg9, s32 col) {
    BtlfxExports *ex = BTLFX_EXPORTS(D_801F0A04);
    SVECTOR p;
    SVECTOR rot;
    SVECTOR *pt;
    s32 i;
    s32 t;
    s32 a;

    ex->btlfx_aimAngles((s16 *)from, (s16 *)to, (s16 *)&rot);
    pt = bolt->unk4;
    bolt->unk2 = 12;
    bolt->unk0 = 12;
    bolt->unk1 = 0;
    for (i = 0; i < 12 + 1; i++) {
        t = 0x1000 - (i << 12) / 12;
        t += ex->btlfx_randCentered(0x100);
        ex->btlfx_lerpVec((s16 *)from, (s16 *)to, t, (s16 *)&p);
        a = ex->btlfx_randCentered(0xA0);
        a = (ex->math_sin(t >> 1) * a) >> 12;
        a += ex->btlfx_lerp(amp0, amp1, t);
        ex->func_800D044C(&p, (s32)pt, 0, (XY32 *)&rot, a, ang);
        pt->pad = col;
        ang += dang + ex->btlfx_randCentered(0x80);
        pt++;
    }
    ex->btlfx_drawTrail((u8 *)ex->btlfx_getFileData(entry->unk8, 0x23), 0, 0, arg8, 0x1000, arg9, 1, (u8 *)bolt, 0);
}

void func_801ECFE4(SVECTOR *pos, SVECTOR *rot, VECTOR *scale, s32 r0, s32 r1, s32 w0, s32 w1, s32 c, s32 tpage,
                   s32 clut, s32 u, s32 v, s32 ang, s32 dy, s32 abr, s32 dang) {
    BtlfxExports *ex = BTLFX_EXPORTS(D_801F0A04);
    Fx0e31Rgbc c0;
    Fx0e31Rgbc c1;
    Fx0e31Rgbc c2;
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

    gte_SetRotMatrix(D_801F0A04->unk14);
    gte_SetTransMatrix(D_801F0A04->unk14);
    gte_ldv0(pos);
    gt4 = (POLY_GT4 *)D_801F0A04->unk24;
    *(s32 *)&c2 = 0;
    ft4 = (POLY_FT4 *)(gt4 + 2);
    D_801F0A04->unk24 = (u8 *)(ft4 + 46);
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

void func_801ED708(VECTOR *pos, SVECTOR *rot) {
    Unk800F78A4 *env = D_801F0A04;
    BtlfxExports *ex = BTLFX_EXPORTS(env);
    SVECTOR v;
    s32 tmp;
    s32 a;

    v.vx = pos->vx;
    v.vy = pos->vy;
    v.vz = pos->vz;
    ex->func_800D0674(&env->unk2C->unk54, &v, rot, (s32)&tmp, (s32)&tmp, &a);
    rot->vz = -a + 0x400;
}

INCLUDE_RODATA("asm/us/fx_0e3_1/nonmatchings/fx_0e3_1", fx_0e3_1_headerPtr);

INCLUDE_ASM("asm/us/fx_0e3_1/nonmatchings/fx_0e3_1", fx_0e3_1_op80);
