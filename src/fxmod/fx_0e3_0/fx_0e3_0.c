#include "fxmod/fx_0e3_0.h"

void func_801E7730(SVECTOR *a, SVECTOR *b, s32 n, s32 tpage, s32 clut, s32 u, s32 v, s32 w, s32 h, s32 p,
                   Fx0e30Seg *buf, s32 abr, s32 phase) {
    Unk800F78A4 *env = D_801EA868;
    BtlfxExports *ex = BTLFX_EXPORTS(env);
    CVECTOR c;
    s32 otz;
    POLY_GT4 *prim;
    Fx0e30Seg *s;
    s32 i;
    s32 t;
    s32 k;

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
    for (i = 0; i < n; i++, buf++, prim++, phase++) {
        gte_ldv3c(buf);
        t = (i << 12) / n;
        gte_rtpt_b();
        if (i != 0) {
            c.r = c.g = c.b = ((0x1000 - t) * p) >> 12;
        } else {
            c.r = c.g = c.b = 0;
        }
        *(s32 *)&prim->r0 = *(s32 *)&prim->r1 = *(s32 *)&c;
        c.r = c.g = c.b = ((0x1000 - t) * p) >> 12;
        *(s32 *)&prim->r2 = *(s32 *)&prim->r3 = *(s32 *)&c;
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

void func_801E7B40(Unk80100C78Entry *entry, Fx0e30Bolt *bolt, SVECTOR *from, SVECTOR *to, s32 amp0, s32 amp1, s32 ang,
                   s32 dang, s32 arg8, s32 arg9) {
    BtlfxExports *ex = BTLFX_EXPORTS(D_801EA868);
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
        t += ex->btlfx_randCentered(D_801EB370);
        ex->btlfx_lerpVec((s16 *)from, (s16 *)to, t, (s16 *)&p);
        a = ex->btlfx_randCentered(D_801EB374);
        a = (ex->math_sin(t >> 1) * a) >> 12;
        a += ex->btlfx_lerp(amp0, amp1, t);
        ex->func_800D044C(&p, (s32)pt, 0, (XY32 *)&rot, a, ang);
        pt->pad = 0x19;
        pt++;
        ang += dang + ex->btlfx_randCentered(D_801EB378);
    }
    ex->btlfx_drawTrail((u8 *)ex->btlfx_getFileData(entry->unk8, 8), 0, 0, arg8, 0x1000, arg9, 1, (u8 *)bolt, 0);
}

void func_801E7D40(Unk80100C78Entry *entry, s32 arg1, Fx0e30Bolt *bolt, SVECTOR *from, SVECTOR *to, s32 amp, s32 ang,
                   s32 dang, s32 arg8, s32 arg9) {
    BtlfxExports *ex = BTLFX_EXPORTS(D_801EA868);
    SVECTOR p;
    SVECTOR rot;
    SVECTOR *pt;
    s32 i;
    s32 t;
    s32 a;

    ex->btlfx_aimAngles((s16 *)from, (s16 *)to, (s16 *)&rot);
    pt = bolt->unk4;
    bolt->unk2 = 0x30;
    bolt->unk0 = 0x30;
    bolt->unk1 = 0;
    for (i = 0; i < 0x31; i++) {
        t = 0x1000 - (i << 12) / 48;
        t += ex->btlfx_randCentered(0x97);
        ex->btlfx_lerpVec((s16 *)from, (s16 *)to, t, (s16 *)&p);
        a = ex->btlfx_randCentered(0x73);
        a = (ex->math_sin(t >> 1) * a) >> 12;
        a += (ex->math_sin(t >> 1) * amp) >> 12;
        ex->func_800D044C(&p, (s32)pt, 0, (XY32 *)&rot, a, ang);
        pt->pad = 0x33;
        ang += dang + ex->btlfx_randCentered(0x100);
        pt++;
    }
    ex->btlfx_drawTrail((u8 *)ex->btlfx_getFileData(entry->unk8, 2), 0, (s32 *)arg1, arg8, 0x1000, arg9, 1, (u8 *)bolt, 1);
}

s32 func_801E7F48(s32 mode, Fx0e30Puff *p, Unk800F78A4 *env, Unk80100C78Entry *entry) {
    BtlfxExports *ex = BTLFX_EXPORTS(env);
    SVECTOR rot;
    SVECTOR pos;
    s32 n;
    s32 t;
    s32 a;
    s32 size;

    if (mode == 1) {
        p->unk8 = 0;
        p->unkE = (ex->btlfx_rand() & 0x1F) + 0x100;
        p->unkC = (ex->btlfx_rand() & 0x1FF) + 0x1000;
    } else {
        n = p->unk8 + p->unkE;
        t = n >> 2;
        p->unk8 = n;
        a = 0x1000 - ex->math_cos(t);
        size = (p->unkC * ((n >> 1) + 0x800)) >> 12;
        rot.pad = 0;
        rot.vz = 0;
        rot.vy = 0;
        rot.vx = 0;
        pos.vx = p->unk0.vx;
        pos.vy = p->unk0.vy - (0x1000 - ex->math_cos(t)) * 3;
        pos.vz = p->unk0.vz;
        ex->btlfx_drawParticles((u8 *)ex->btlfx_getFileData(entry->unk8, 3), &pos, &rot, size * 2, 0, 1, a, 0, 0);
        if (p->unk8 > 0x1000) {
            return 1;
        }
    }
    return 0;
}

INCLUDE_RODATA("asm/us/fx_0e3_0/nonmatchings/fx_0e3_0", fx_0e3_0_headerPtr);

INCLUDE_ASM("asm/us/fx_0e3_0/nonmatchings/fx_0e3_0", fx_0e3_0_op80);
