#include "fxmod/fx_0fb_0.h"

void func_801E7720(SVECTOR *a, SVECTOR *b, u32 n, CVECTOR *ca, CVECTOR *cb, s32 p, Fx0fb0Seg *buf, s32 abr) {
    Unk800F78A4 *env = D_801EB3AC;
    CVECTOR c0;
    CVECTOR c1;
    s32 otz;
    BtlfxExports *ex = BTLFX_EXPORTS(env);
    POLY_G4 *prim;
    Fx0fb0Seg *s;
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

INCLUDE_ASM("asm/us/fx_0fb_0/nonmatchings/fx_0fb_0", func_801E7B90);

INCLUDE_ASM("asm/us/fx_0fb_0/nonmatchings/fx_0fb_0", func_801E809C);

s32 func_801E8540(s32 mode, Fx0fb0Rock *r, Unk800F78A4 *env) {
    BtlfxExports *ex = BTLFX_EXPORTS(env);
    VECTOR pos;
    VECTOR sc;

    if (mode == 1) {
        r->unk0 = 0;
        r->unk8 = 0;
        r->unk6 = ex->btlfx_randCentered(0x1800) - 0x3000;
        r->unk14.vx = 0;
        r->unk14.vy = 0;
        r->unk14.vz = 0;
        r->unk1C = ex->btlfx_rand() % 64 + 0x40;
        r->unk1E = 0x20 - ex->btlfx_rand() % 32;
    } else {
    if (r->unk4 < 0x19) {
        sc.vx = 0x1000;
        sc.vy = 0x1000;
        sc.vz = 0x1000;
    } else {
        sc.vx = 0x199;
        sc.vy = 0x199;
        sc.vz = 0x200;
    }
    pos.vx = r->unkC.vx;
    pos.vy = r->unkC.vy;
    pos.vz = r->unkC.vz;
    r->unk14.vx += r->unk1C;
    r->unk14.vy += r->unk1E;
    r->unkC.vx += ex->btlfx_randCentered(0x10);
    r->unkC.vz += ex->btlfx_randCentered(0x10);
    switch (r->unk0) {
    case 0:
        mode = (r->unk8 << 12) / 40; /* reuses the parameter */
        pos.vy += (ex->math_sin(mode >> 2) * r->unk6) >> 12;
        if (++r->unk8 >= 0x28) {
            r->unk0 = 1;
            r->unk8 = 0;
            r->unkC.vy = pos.vy;
            r->unk6 = ex->btlfx_rand();
        }
        break;
    case 1:
        pos.vy += ex->math_sin((r->unk8 << 7) + r->unk6) >> 3;
        r->unk8++;
        break;
    }
    if (D_801EB348 != 0 || (D_801EB344 != 0 && r->unk4 >= 0x19)) {
        return 1;
    }
    ex->Hi_DrawEffModel(&r->unk14, &pos, (s32)&sc, r->unk20);
    }
    return 0;
}

INCLUDE_RODATA("asm/us/fx_0fb_0/nonmatchings/fx_0fb_0", fx_0fb_0_headerPtr);

INCLUDE_ASM("asm/us/fx_0fb_0/nonmatchings/fx_0fb_0", fx_0fb_0_op80);

s32 fx_0fb_0_op81(s32 mode, Fx0fb0Work81 *work, Unk800F78A4 *env, Unk80100C78Entry *entry, s32 *arg4) {
    BtlfxExports *ex = BTLFX_EXPORTS(env);
    SVECTOR v;
    s32 frame;
    s32 t;
    s32 i;

    if (mode == 0) {
        *arg4 = 0x10C;
    } else if (mode == 1) {
        D_801EB3AC = env;
        work->unk104 = env->unkC->unk24;
        work->unk0 = 0;
        for (i = 0; i < work->unk104; i++) {
            if (ex->btlfx_isEnemyUnit(i) != 0) {
                ex->btlfx_getUnitHomePos(i, (s16 *)&work->unk4[i]);
                work->unk108 = ex->btlfx_randCentered(0x400) - 0x3000;
                ex->btlfx_getUnitRot(i, (s16 *)&work->unk44[i]);
                BTLFX_COPY_SVEC(&work->unkC4[i], &work->unk44[i]);
                work->unk84[i].vx = ex->btlfx_randCentered(0x100);
                work->unk84[i].vy = ex->btlfx_randCentered(0x180);
                work->unk84[i].vz = ex->btlfx_randCentered(0x20);
            }
        }
    } else {
        frame = entry->unk0;
        if (work->unk0 == 0) {
            t = (frame << 12) / 46;
            for (i = 0; i < work->unk104; i++) {
                if (ex->btlfx_isEnemyUnit(i) != 0) {
                    BTLFX_COPY_SVEC(&v, &work->unk4[i]);
                    v.vy += (ex->math_sin(t >> 2) * work->unk108) >> 12;
                    ex->btlfx_setUnitPos(i, (s16 *)&v);
                    work->unkC4[i].vx += work->unk84[i].vx;
                    work->unkC4[i].vy += work->unk84[i].vy;
                    work->unkC4[i].vz += work->unk84[i].vz;
                    ex->btlfx_setUnitRot(i, (s16 *)&work->unkC4[i]);
                }
            }
            if (frame >= 0x2E) {
                work->unk0 = 1;
                entry->unk0 = -1;
            }
        } else {
            for (i = 0; i < work->unk104; i++) {
                if (ex->btlfx_isEnemyUnit(i) != 0) {
                    work->unkC4[i].vx += work->unk84[i].vx;
                    work->unkC4[i].vy += work->unk84[i].vy;
                    work->unkC4[i].vz += work->unk84[i].vz;
                    ex->btlfx_setUnitRot(i, (s16 *)&work->unkC4[i]);
                }
            }
            if (D_801EB348 != 0) {
                for (i = 0; i < work->unk104; i++) {
                    if (ex->btlfx_isEnemyUnit(i) != 0) {
                        ex->btlfx_setUnitRot(i, (s16 *)&work->unk44[i]);
                        ex->btlfx_getUnitHomePos(i, (s16 *)&v);
                        ex->btlfx_setUnitPos(i, (s16 *)&v);
                    }
                }
                return 1;
            }
        }
    }
    return 0;
}

s32 fx_0fb_0_op87(s32 mode, void *work, Unk800F78A4 *env, Unk80100C78Entry *entry, s32 *arg4) {
    SVECTOR v;

    if (mode == 0) {
        *arg4 = 4;
    } else if (mode == 1) {
        v.vx = env->unkC->unk38;
        v.vy = env->unkC->unk3A;
        v.vz = env->unkC->unk3C;
        v.vy -= 0x230;
        env->unk2C->unk0[2].vx = v.vx;
        env->unk2C->unk0[2].vy = v.vy;
        env->unk2C->unk0[2].vz = v.vz;
        D_801EB3AC = env;
        return 1;
    } else {
        return 0;
    }
    return 0;
}
