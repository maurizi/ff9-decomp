#include "fxmod/fx_05e.h"

/* ribbon trail: push a new pair of edge points and draw the quads between steps, fading out */
void func_801E7704(SVECTOR *a, SVECTOR *b, u32 n, CVECTOR *ca, CVECTOR *cb, s32 p, Fx05eSeg *buf, s32 abr) {
    Unk800F78A4 *env = D_801E8E74;
    CVECTOR c0;
    CVECTOR c1;
    s32 otz;
    BtlfxExports *ex = BTLFX_EXPORTS(env);
    POLY_G4 *prim;
    Fx05eSeg *s;
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

/* transform a point by a bone of the summon model, then restore the camera matrix */
void func_801E7B74(s32 model, s32 bone, SVECTOR *in, SVECTOR *out) {
    Unk800F78A4 *env = D_801E8E74;
    MATRIX m;

    BTLFX_EXPORTS(env)->Hi_GetSummonBoneMatrix(model, bone, (BtlBone *)&m);
    gte_SetRotMatrix(&m);
    gte_SetTransMatrix(&m);
    gte_ldv0(in);
    gte_rtv0tr();
    gte_stlvnl(m.t);
    out->vx = m.t[0];
    out->vy = m.t[1];
    out->vz = m.t[2];
    gte_SetRotMatrix(env->unk14);
    gte_SetTransMatrix(env->unk14);
}

INCLUDE_ASM("asm/us/fx_05e/nonmatchings/fx_05e", fx_05e_op80);

INCLUDE_RODATA("asm/us/fx_05e/nonmatchings/fx_05e", fx_05e_headerPtr);
