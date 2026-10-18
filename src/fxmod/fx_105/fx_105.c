#include "fxmod/fx_105.h"

void func_801E7704(SVECTOR *a, SVECTOR *b, u32 n, CVECTOR *ca, CVECTOR *cb, s32 p, Fx105Seg *buf, s32 abr) {
    Unk800F78A4 *env = D_801EAB18;
    CVECTOR c0;
    CVECTOR c1;
    s32 otz;
    BtlfxExports *ex = BTLFX_EXPORTS(env);
    POLY_G4 *prim;
    Fx105Seg *s;
    u32 i;
    s32 p0;
    s32 p1;

    if (p < 0) {
        p = 0x1000;
    }
    if (n >= 1000) {
        n -= 999;
        for (i = 0; i < n; i++) {
            buf[i].unk0.pad = 0;
        }
        return;
    }
    s = &buf[(s32)(n - 1)];
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
    prim = (POLY_G4 *)env->unk24;
    env->unk24 = (s32)(prim + n);
    if (ca == NULL) {
        c0.r = c0.g = c0.b = 0;
    } else {
        gte_DpqColorFar_nst(ca, p);
        gte_strgb(&c0);
    }
    if (cb == NULL) {
        c1.r = c1.g = c1.b = 0;
    } else {
        gte_DpqColorFar_nst(cb, p);
        gte_strgb(&c1);
    }
    gte_SetRotMatrix(env->unk14);
    gte_SetTransMatrix(env->unk14);
    c0.cd = abr != 0xFF ? 0x3A : 0x38;
    c1.cd = abr != 0xFF ? 0x3A : 0x38;
    for (i = 0; i < n; i++, buf++, prim++) {
        gte_ldv3c(buf);
        p0 = 0x1000 - (i << 12) / n;
        gte_rtpt_b();
        p1 = 0x1000 - ((i + 1) << 12) / n;
        gte_stopz(&otz);
        if (otz != 0) {
            gte_stsxy3(&prim->x0, &prim->x1, &prim->x2);
            gte_avsz3();
            gte_stszotz(&otz);
            gte_ldv0(&buf[1].unk8);
            gte_rtps();
            ex->btlfx_addPrimAbr(0x8000000, otz, prim, abr);
            gte_stsxy(&prim->x3);
            gte_DpqColorFar_nst(&c0, p0);
            gte_strgb(&prim->r0);
            gte_DpqColorFar_nst(&c1, p0);
            gte_strgb(&prim->r1);
            gte_DpqColorFar_nst(&c0, p1);
            gte_strgb(&prim->r2);
            gte_DpqColorFar_nst(&c1, p1);
            gte_strgb(&prim->r3);
        }
    }
}

void func_801E7B74(void) {
    s32 i;
    s32 m;

    D_801EAB1C = 0;
    m = -1;
    for (i = 15; i >= 0; i--) {
        D_801EAB28[i].unk4 = m;
    }
}

void func_801E7BA8(s32 start, u32 dst, u32 src, s16 *data) {
    s32 i;

    for (i = 0; i < 16; i++) {
        if (D_801EAB28[i].unk4 == -1) {
            D_801EAB28[i].unk4 = 0;
            D_801EAB28[i].unk10 = data + 1;
            D_801EAB28[i].unk0 = start;
            D_801EAB28[i].unk8 = dst;
            D_801EAB28[i].unkC = src;
            return;
        }
    }
}

INCLUDE_ASM("asm/us/fx_105/nonmatchings/fx_105", func_801E7BF8);

void func_801E7D9C(s32 z, s32 tpage, s32 y0, s32 y1, CVECTOR *rgb) {
    POLY_G4 *p = (POLY_G4 *)D_801EAB18->unk24;
    BtlfxExports *ex = BTLFX_EXPORTS(D_801EAB18);

    D_801EAB18->unk24 = (u8 *)(p + 1);
    p->code = (tpage != 0xFF) ? 0x3A : 0x38;
    p->r0 = rgb[0].r;
    p->g0 = rgb[0].g;
    p->b0 = rgb[0].b;
    p->r1 = rgb[1].r;
    p->g1 = rgb[1].g;
    p->b1 = rgb[1].b;
    p->r2 = rgb[2].r;
    p->g2 = rgb[2].g;
    p->b2 = rgb[2].b;
    p->r3 = rgb[3].r;
    p->g3 = rgb[3].g;
    p->b3 = rgb[3].b;
    p->x2 = 0;
    p->x0 = 0;
    p->x3 = 0x140;
    p->x1 = 0x140;
    p->y1 = y0;
    p->y0 = y0;
    p->y3 = y1;
    p->y2 = y1;
    ex->btlfx_addPrim(0x08000000, z * 4, p);
    ex->btlfx_addPrim(0x01000000, z * 4, ex->btlfx_allocDrMode(0, tpage));
}

INCLUDE_ASM("asm/us/fx_105/nonmatchings/fx_105", fx_105_op80);

void func_801E9E1C(void) {
    Unk800F78A4 *env = D_801EAB18;
    BtlfxExports *ex = BTLFX_EXPORTS(env);
    Fx105Spark *p;
    s32 t;
    s32 i;

    for (i = 0; i < 0x20; i++) {
        t = ex->btlfx_randRange(-((i + 1) * 0x180), -(i * 0x180));
        p = &D_801EC168[i];
        p->vx = t;
        p->vz = ex->btlfx_randRange(-0x1000, 0x2000);
        p->vy = p->vx >> 4;
        p->vx += env->unk2C->unk0[2].vx;
        p->vz += env->unk2C->unk0[2].vz;
        p->vy += env->unk2C->unk0[2].vy;
    }
}

INCLUDE_ASM("asm/us/fx_105/nonmatchings/fx_105", func_801E9F28);

INCLUDE_ASM("asm/us/fx_105/nonmatchings/fx_105", func_801EA298);

INCLUDE_ASM("asm/us/fx_105/nonmatchings/fx_105", func_801EA6D4);

INCLUDE_RODATA("asm/us/fx_105/nonmatchings/fx_105", fx_105_headerPtr);
