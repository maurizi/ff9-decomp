#include "fxmod/fx_0fb_1.h"

void func_801EC738(SVECTOR *pos, SVECTOR *rot, VECTOR *scale, s32 r, s32 p, CVECTOR *rgb, s32 tpage, s32 clut, s32 u,
                   s32 v, s32 dang, s32 abr) {
    Unk800F78A4 *env = D_801F0D2C;
    BtlfxExports *ex = BTLFX_EXPORTS(env);
    CVECTOR col;
    MATRIX m;
    SVECTOR q[4];
    s32 z;
    POLY_FT4 *prim;
    s32 ang;
    s32 a;
    s32 z0;
    s32 z1;
    s32 row;
    s32 rv;
    s32 j;
    s32 ju;
    s32 u0;
    s32 v0;
    s32 u1;
    s32 v1;

    gte_SetRotMatrix(env->unk14);
    gte_SetTransMatrix(env->unk14);
    gte_ldv0(pos);
    prim = (POLY_FT4 *)env->unk24;
    env->unk24 = (u8 *)(prim + 320);
    col.cd = abr == 0xFF ? 0x2C : 0x2E;
    col.r = (rgb->r * p) >> 12;
    col.g = (rgb->g * p) >> 12;
    col.b = (rgb->b * p) >> 12;
    tpage |= (abr & 3) << 5;
    gte_rtv0tr_b();
    ex->func_800316D8(rot, &m);
    ex->ScaleMatrix(&m, scale);
    gte_stlvnl(m.t);
    ex->MulRotMatrix(&m);
    gte_SetRotMatrix(&m);
    gte_SetTransMatrix(&m);
    ang = 0;
    z1 = 0x866;
    z0 = 0;
    for (row = 0; row < 20; row++) {
        ang += dang;
        rv = row & 0xF;
        for (j = 0; j < 16; j++) {
            q[0].vx = (ex->math_cos(ang) * r) >> 12;
            q[0].vy = (ex->math_sin(ang) * r) >> 12;
            a = ang + 0x100;
            q[1].vx = (ex->math_cos(a) * r) >> 12;
            q[1].vy = (ex->math_sin(a) * r) >> 12;
            q[0].vz = q[1].vz = z0;
            ang += dang;
            q[2].vx = (ex->math_cos(ang) * r) >> 12;
            q[2].vy = (ex->math_sin(ang) * r) >> 12;
            a = ang + 0x100;
            q[3].vx = (ex->math_cos(a) * r) >> 12;
            q[3].vy = (ex->math_sin(a) * r) >> 12;
            q[2].vz = q[3].vz = z1;
            ang -= dang;
            gte_ldv3c(q);
            gte_rtpt();
            ju = j & 3;
            /* two statements each: a twice-set u0/v0 is not loop-invariant, so v + rv * 8 stays in the loop */
            u0 = u;
            u0 += ju * 32;
            v0 = v;
            v0 += rv * 8;
            u1 = u0 + 0x20;
            if (ju == 3) {
                u1 = u0 + 0x1F;
            }
            v1 = v0 + 8;
            if (rv == 0xF) {
                v1 = v0 + 7;
            }
            gte_nclip();
            *(s32 *)&prim->r0 = *(s32 *)&col;
            gte_stopz(&z);
            if (z >= 0) {
                prim->u0 = prim->u2 = u0;
                prim->u1 = prim->u3 = u1;
                prim->v0 = prim->v1 = v0;
                prim->v2 = prim->v3 = v1;
                prim->tpage = tpage;
                prim->clut = clut;
                gte_stsxy3(&prim->x0, &prim->x1, &prim->x2);
                gte_avsz3();
                gte_stszotz(&z);
                z = 0xFFF;
                gte_ldv0(&q[3]);
                gte_rtps();
                ex->btlfx_addPrim(0x09000000, z, prim);
                gte_stsxy(&prim->x3);
                prim++;
            }
            ang += 0x100;
        }
        z1 += 0x866;
        z0 += 0x866;
    }
}

INCLUDE_RODATA("asm/us/fx_0fb_1/nonmatchings/fx_0fb_1", fx_0fb_1_headerPtr);

INCLUDE_ASM("asm/us/fx_0fb_1/nonmatchings/fx_0fb_1", func_801ECBE0);

INCLUDE_ASM("asm/us/fx_0fb_1/nonmatchings/fx_0fb_1", fx_0fb_1_op80);

s32 fx_0fb_1_op81(s32 mode, Fx0fb1Work81 *work, Unk800F78A4 *env, Unk80100C78Entry *entry, s32 *arg4) {
    BtlfxExports *ex = BTLFX_EXPORTS(env);
    Fx0fb1Vec3 sc;
    s32 i;
    s32 j;
    s32 k;
    s32 a;
    s32 r;

    if (mode == 0) {
        *arg4 = 0x1A8;
    } else if (mode == 1) {
        D_801F0D2C = env;
        work->unk1A0 = env->unkC->unk24;
        for (i = 0; i < work->unk1A0; i++) {
            if (ex->btlfx_isEnemyUnit(i) != 0) {
                ex->btlfx_getUnitHomePos(i, (s16 *)&work->unk60[i]);
                if (work->unk1A0 >= 2) {
                    work->unk1A4 = ex->btlfx_randCentered(0x200);
                    a = (i << 12) / work->unk1A0 + 0x200;
                    r = ex->btlfx_randCentered(0x100) + 0x200;
                    setVector(&work->unkA0[i], (ex->math_cos(a) * r) >> 12, work->unk1A4 - 0x1000,
                              (ex->math_sin(a) * r) >> 12);
                } else {
                    work->unk1A4 = 0;
                    setVector(&work->unkA0[i], 0, -0x1000, 0);
                }
                ex->btlfx_setUnitPos(i, (s16 *)&work->unkA0[i]);
                ex->btlfx_getUnitRot(i, (s16 *)&work->unkE0[i]);
                BTLFX_COPY_SVEC(&work->unk160[i], &work->unkE0[i]);
                work->unk120[i].vx = ex->btlfx_randCentered(0x80);
                work->unk120[i].vy = ex->btlfx_randCentered(0x80);
                work->unk120[i].vz = ex->btlfx_randCentered(0x20);
                work->unk160[i].vx = ex->btlfx_rand();
                work->unk160[i].vy = ex->btlfx_rand();
                ex->btlfx_getUnitScale(i, (VECTOR *)&work->unk0[i]);
                sc.vx = work->unk0[i].vx / 2;
                sc.vy = work->unk0[i].vy / 2;
                sc.vz = work->unk0[i].vz / 2;
                ex->func_800E7ACC(i, (s32 *)&sc);
            }
        }
    } else {
        for (j = 0; j < work->unk1A0; j++) {
            if (ex->btlfx_isEnemyUnit(j) != 0) {
                work->unk160[j].vx += work->unk120[j].vx;
                work->unk160[j].vy += work->unk120[j].vy;
                work->unk160[j].vz += work->unk120[j].vz;
                ex->btlfx_setUnitRot(j, (s16 *)&work->unk160[j]);
                if (D_801F0D34 != 0x1000) {
                    ex->func_800E80CC(j, D_801F0D34 >> 5);
                }
            }
        }
        if (D_801F0D30 != 0) {
            for (j = 0; j < work->unk1A0; j++) {
                if (ex->btlfx_isEnemyUnit(j) != 0) {
                    ex->btlfx_setUnitRot(j, (s16 *)&work->unkE0[j]);
                    ex->btlfx_getUnitHomePos(j, (s16 *)&sc);
                    ex->btlfx_setUnitPos(j, (s16 *)&sc);
                    ex->func_800E7ACC(j, (s32 *)&work->unk0[j]);
                }
            }
            return 1;
        }
    }
    return 0;
}

s32 fx_0fb_1_op82(s32 mode, Fx0fb1Work82 *work, Unk800F78A4 *env, Unk80100C78Entry *entry, s32 *arg4) {
    BtlfxExports *ex = BTLFX_EXPORTS(env);
    SVECTOR v;
    SVECTOR rot;
    s32 frame;
    s32 n;
    s32 k;
    u8 *f;
    BtlUnitList *units;

    if (mode == 0) {
        *arg4 = 0x30;
    } else if (mode == 1) {
        units = env->unkC;
        n = *arg4;
        D_801F0D2C = env;
        work->unk24 = units->unk24;
        work->unk0 = 0;
        if (n >= work->unk24) {
            return 1;
        }
        work->unk2C = n;
        ex->btlfx_getUnitHomePos(n, (s16 *)&work->unk4);
        ex->btlfx_getUnitRot(work->unk2C, (s16 *)&work->unkC);
        BTLFX_COPY_SVEC(&work->unk1C, &work->unkC);
        work->unk14 = 0x2000 / (((ex->btlfx_rand() & 1) + 1) * 16);
        work->unk16 = 0x2000 / (((ex->btlfx_rand() & 1) + 1) * 16);
        work->unk18 = 0x2000 / (((ex->btlfx_rand() & 1) + 1) * 16);
        work->unk28 = -0x1000 - (ex->btlfx_rand() & 0x3FF);
    } else {
        frame = entry->unk0;
        if (work->unk0 == 0) {
            k = frame << 8;
            BTLFX_COPY_SVEC(&v, &work->unk4);
            v.vy += (work->unk28 * ex->math_cos(k >> 2)) >> 12;
            ex->btlfx_setUnitPos(work->unk2C, (s16 *)&v);
            work->unk1C.vx += work->unk14;
            work->unk1C.vy += work->unk16;
            work->unk1C.vz += work->unk18;
            ex->btlfx_setUnitRot(work->unk2C, (s16 *)&work->unk1C);
            if (frame >= 0x10) {
                ex->btlfx_setUnitRot(work->unk2C, (s16 *)&work->unkC);
                ex->btlfx_getUnitHomePos(work->unk2C, (s16 *)&v);
                ex->btlfx_setUnitPos(work->unk2C, (s16 *)&v);
                work->unk0 = 1;
                entry->unk0 = -1;
            }
        } else {
            k = (frame << 12) / 7;
            rot.vx = 0x400;
            rot.vz = 0;
            rot.vy = 0;
            rot.pad = 1;
            f = (u8 *)ex->btlfx_getFileData(entry->unk8, 0x12);
            ex->func_800D23D8(f, &work->unk4, &rot, 0x1000, frame, 1, ex->math_cos(k >> 2));
            if (frame >= 7) {
                return 1;
            }
        }
    }
    return 0;
}
