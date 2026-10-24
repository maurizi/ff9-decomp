#include "fxmod/fx_17a.h"

void func_801E7718(SVECTOR *pos, SVECTOR *rot, VECTOR *scale, s32 r, s32 p, CVECTOR *rgb, s32 tpage, s32 clut, s32 u,
                   s32 v, s32 dang, s32 abr) {
    Unk800F78A4 *env = D_801E9F5C;
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

    gte_SetRotMatrix(env->viewMatrix);
    gte_SetTransMatrix(env->viewMatrix);
    gte_ldv0(pos);
    prim = (POLY_FT4 *)env->prim;
    env->prim = (u8 *)(prim + 320);
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

s32 func_801E7BC0(s32 mode, Fx17aPart *p, Unk800F78A4 *env, Unk80100C78Entry *entry, s32 frame) {
    BtlfxExports *ex = BTLFX_EXPORTS(env);

    if (mode == 1) {
        p->unk2 = 0;
        p->unk14.vx = ex->btlfx_rand();
        p->unk14.vy = ex->btlfx_rand();
        p->unk14.vz = 0;
        p->unk1C = ex->btlfx_rand() % 128 + 0x80;
        p->unk1E = 0xC0 - ex->btlfx_rand() % 32;
        p->unk24 = 0;
        p->unk20 = 0;
    } else if (p->unk0 == 4) {
        p->unkC.vx += -0x80 - p->unkA;
        ex->btlfx_drawParticles((u8 *)ex->btlfx_getFileData(entry->unk8, 2), &p->unkC, NULL, p->unk4 * 3 >> 1, 0, 1, -1, 0, 0);
        if (frame >= 0x11) {
            return 1;
        }
    }
    return 0;
}

INCLUDE_RODATA("asm/us/fx_17a/nonmatchings/fx_17a", fx_17a_headerPtr);

/* Matching notes: the variable split is what global-alloc needs. The spawn loop's e/d are fresh block locals (d is set
 * once, so sched1 launches its +0x180 next to the call; both are local-alloc'd), the spawn counter s, the trail ring
 * counter m and case 3's ph are their own variables, the case 3 model loop subtracts b * 0x30 (a loop giv, not a
 * counter variable), and the tint values reuse c1 (a global pseudo, so the 0xFF constant isn't tied to the result). */
s32 fx_17a_op80(s32 mode, Fx17aWork80 *work, Unk800F78A4 *env, Unk80100C78Entry *entry, EffInit *init) {
    BtlfxExports *ex = BTLFX_EXPORTS(env);
    Pool6CEntry *p;
    Fx17aPart *q;
    s32 frame;
    s32 t;
    s32 glow;
    s32 g;
    s32 c;
    s32 i;
    s32 j;
    s32 r;
    s32 a;
    s32 b;
    s32 g2;
    s32 k;
    s32 x2;
    s32 n;
    s32 f1;
    s32 f2;
    s32 f3;
    s32 f4;
    s32 f5;
    s32 f6;
    s32 c1;
    s32 ph;
    s32 m;
    s32 s;

    if (mode == 0) {
        init->unk0 = 0xB4;
        init->unk8 = 0x28;
        init->unk14 = (s32)func_801E7BC0;
        init->unk4 = 0x30;
    } else if (mode == 1) {
        work->unk0 = 2;
        work->unk4 = 0;
        D_801E9F5C = env;
        ex->btlfx_initEffModels(D_801E9F64, 6);
        work->unk80 = ex->Hi_RegisterTexEffModel((BtlUnit9C8 *)ex->btlfx_getFileData(entry->unk8, 0xA), 0xBA, 0x3FC0, 0, 0);
        work->unk84 = ex->Hi_RegisterTexEffModel((BtlUnit9C8 *)ex->btlfx_getFileData(entry->unk8, 7), 0xBA, 0x3EC0, 0, 0x80);
        work->unk88 = ex->Hi_RegisterTexEffModel((BtlUnit9C8 *)ex->btlfx_getFileData(entry->unk8, 8), 0x3A, 0x3E81, 0, 0x80);
        work->unk8C = ex->Hi_RegisterTexEffModel((BtlUnit9C8 *)ex->btlfx_getFileData(entry->unk8, 9), 0x138, 0xFFFF, 0, 0x80);
        BTLFX_COPY_SVEC(&work->unkC, &D_801E9AC4);
        work->unk4C.vx = 0;
        work->unk4C.vy = 0;
        work->unk4C.vz = 0;
        work->unk34.vx = work->unkC.vx;
        work->unk34.vy = work->unkC.vy;
        work->unk34.vz = work->unkC.vz;
        work->unk14.vx = work->unkC.vx;
        work->unk14.vy = work->unkC.vy;
        work->unk14.vz = work->unkC.vz - 0x2555;
        work->unk1C.vx = work->unkC.vx;
        work->unk1C.vy = work->unkC.vy;
        work->unk1C.vz = work->unkC.vz - 0x1C00;
    } else {
        glow = 0;
        g = 0;
        frame = entry->unk0;
        {
        switch (work->unk0) {
        case 2:
            env->camera->points[3].vx = work->unkC.vx;
            env->camera->points[3].vy = work->unkC.vy;
            env->camera->points[3].vz = work->unkC.vz;
            work->unk4C.vx += 0x20;
            work->unk4C.vy += 0x10;
            t = (frame << 12) / 18;
            work->unk40.vx = work->unk40.vy = work->unk40.vz = (0x1000 - t) * 2;
            glow = (t * 0xAA) >> 12;
            ex->Hi_DrawEffModel(&work->unk4C, (VECTOR *)&work->unk34, (s32)&work->unk40, work->unk80);
            ex->Hi_ModifyEffModelRGB(work->unk80, (ex->math_sin(t >> 2) >> 5) - 0x80, (ex->math_sin(t >> 2) >> 5) - 0x80,
                                     (ex->math_sin(t >> 2) >> 5) - 0x80);
            work->unk4C.vz += 0x400;
            ex->Hi_DrawEffModel(&work->unk4C, (VECTOR *)&work->unk34, (s32)&work->unk40, work->unk80);
            ex->Hi_ModifyEffModelRGB(work->unk80, (ex->math_sin(t >> 2) >> 5) - 0x80, (ex->math_sin(t >> 2) >> 5) - 0x80,
                                     (ex->math_sin(t >> 2) >> 5) - 0x80);
            work->unk4C.vz -= 0x400;
            env->unk28 = 0x100;
            f2 = ex->math_sin(t >> 2) * 2;
            ex->func_800D23D8((u8 *)ex->btlfx_getFileData(entry->unk8, 3), &work->unkC, NULL, f2, frame, 1, -1);
            env->unk28 = 0;
            if (frame >= 0x12) {
                work->unk0 = 3;
                entry->unk0 = -1;
                for (f3 = 0; f3 < 4; f3++) {
                    work->unk90[f3].vx = 0x800 - f3 * 0x180;
                    work->unk90[f3].vy = f3 * 0xC0;
                    work->unk90[f3].vz = f3 * 0x60;
                }
                p = ex->func_800E3CEC(ex->btlfx_getFileData(entry->unk8, 0x13), (s32)&work->unk1C);
                if (p != NULL) {
                    p->unk22 = 0x80;
                }
            }
            break;
        case 3: {
            VECTOR sc;

            glow = 0xAA;
            work->unk4C.vx += 0x20;
            work->unk4C.vy += 0x10;
            t = (frame << 12) / 24;
            g = (t >> 4) + 0x80;
            if (frame < 0xC) {
                b = 0x6AA;
            } else if (frame < 0x14) {
                b = 0x6AA - ((frame - 0xC) * 0x6AA) / 8;
            } else {
                b = 0;
            }
            if (b != 0) {
                work->unk40.vx = work->unk40.vy = work->unk40.vz = b;
                ex->Hi_DrawEffModel(&work->unk4C, (VECTOR *)&work->unk34, (s32)&work->unk40, work->unk80);
                work->unk4C.vz += 0x400;
                ex->Hi_DrawEffModel(&work->unk4C, (VECTOR *)&work->unk34, (s32)&work->unk40, work->unk80);
                work->unk4C.vz -= 0x400;
            }
            b = b * 5 + 0x1000;
            ex->func_800D23D8((u8 *)ex->btlfx_getFileData(entry->unk8, 3), &work->unkC, NULL, b, frame, 1, -1);
            if ((u32)(frame - 0xF) < 6) {
                b = 0x1000 - (((frame - 0xF) << 12) / 6);
                ex->func_800D23D8((u8 *)ex->btlfx_getFileData(entry->unk8, 1), &work->unk14, NULL, b, frame, 1, 0x1000);
            }
            if (frame >= 0xC) {
                ph = ((frame - 0xC) << 10) / 12;
                b = ex->math_sin(ph) * 2 + 0x2000;
                i = ex->math_sin(ph);
                ex->func_800D23D8((u8 *)ex->btlfx_getFileData(entry->unk8, 4), &work->unk14, NULL, b, 0, 1, i);
            }
            if (frame >= 0xB) {
                x2 = ((frame - 0xA) << 12) / 14;
                r = x2 >> 2;
                sc.vx = sc.vy = sc.vz = (x2 * 0xB) / 12;
                for (b = 0; b < 4; b++) {
                    work->unk90[b].vz = (s16)(work->unk90[b].vz - 0x18) - b * 0x30;
                    ex->Hi_DrawEffModel(&work->unk90[b], (VECTOR *)&work->unk34, (s32)&sc, work->unk84);
                    sc.vx = sc.vy = sc.vz = (sc.vx * 0x12) >> 4;
                    ex->Hi_ModifyEffModelRGB(work->unk84, ((ex->math_sin(r) * 2) / 3 >> 5) - 0x80,
                                             ((ex->math_sin(r) * 2) / 3 >> 5) - 0x80,
                                             ((ex->math_sin(r) * 2) / 3 >> 5) - 0x80);
                }
            }
            if (frame == 0xC) {
                p = ex->func_800E3CEC(ex->btlfx_getFileData(entry->unk8, 0xD), (s32)&work->unk14);
                if (p != NULL) {
                    p->unk22 = 0x80;
                }
            }
            i = (ex->math_sin(t >> 2) * 0x46) >> 12;
            ex->btlfx_drawScreenTint((u32 *)(env->ot + 1), 1, i >> 2, i >> 1, i);
            if (frame >= 0x15) {
                i = ((frame - 0x14) * 0xFF) / 4;
                ex->btlfx_drawScreenTint((u32 *)(env->ot + 1), 1, i, i, i);
            }
            if (frame >= 0x11) {
                n = frame - 0x14;
                i = ex->math_sin(n << 8);
                ex->func_800D3B6C((u8 *)ex->btlfx_getFileData(entry->unk8, 0xC), &work->unkC, n, i);
            }
            if (frame >= 0x18) {
                work->unk0 = 4;
                entry->unk0 = -1;
                BTLFX_COPY_SVEC(&work->unk24, &D_801E9AC4);
                work->unk24.vy += 0x400;
                env->camera->points[3].vx = work->unk24.vx;
                env->camera->points[3].vy = work->unk24.vy;
                env->camera->points[3].vz = work->unk24.vz;
                work->unk34.vx = D_801E9AC4.vx;
                work->unk34.vy = D_801E9AC4.vy;
                work->unk34.vz = D_801E9AC4.vz;
                BTLFX_COPY_SVEC(&work->unkC, &D_801E9AC4);
                BTLFX_COPY_SVEC(&work->unk14, &D_801E9AC4);
                p = work->unkB0 = ex->func_800E3CEC(ex->btlfx_getFileData(entry->unk8, 0xD), (s32)&work->unk14);
                if (p != NULL) {
                    p->unk22 = 0x80;
                }
            }
            break;
        }
        case 4:
            BTLFX_COPY_SVEC(&work->unk24, &D_801E9AC4);
            t = (frame << 12) / 10;
            work->unk24.vy += (0x1000 - t) >> 2;
            env->camera->points[3].vx = work->unk24.vx;
            glow = 0xF0;
            env->camera->points[3].vy = work->unk24.vy;
            g = 0x300;
            env->camera->points[3].vz = work->unk24.vz;
            work->unk4C.vx += 0x20;
            work->unk4C.vy += 0x10;
            if (frame < 0x10) {
                s32 v;

                c = (frame << 12) >> 4;
                a = ex->math_sin((frame << 12) >> 6) >> 1;
                work->unk40.vx = work->unk40.vy = work->unk40.vz = a;
                ex->Hi_DrawEffModel(&work->unk4C, (VECTOR *)&work->unk34, (s32)&work->unk40, work->unk80);
                v = ((0x1000 - c) >> 5) - 0x80;
                ex->Hi_ModifyEffModelRGB(work->unk80, v, v, v);
            }
            env->unk28 = 0x100;
            c = ex->math_cos(t >> 2);
            a = ex->math_sin(t >> 2) + 0x400;
            ex->func_800D23D8((u8 *)ex->btlfx_getFileData(entry->unk8, 3), &work->unkC, NULL, a, frame, 1, -1);
            ex->func_800D23D8((u8 *)ex->btlfx_getFileData(entry->unk8, 1), &work->unkC, NULL, a, frame, 1, c);
            env->unk28 = 0;
            if (frame < 6) {
                c1 = 0xFF - ((frame * 0xFF) / 6);
                ex->btlfx_drawScreenTint((u32 *)(env->ot + 1), 1, c1, c1, c1);
            }
            if (frame >= 7) {
                c1 = (frame - 7) * 0x55;
                ex->btlfx_drawScreenTint((u32 *)(env->ot + 1), 2, c1, c1, c1);
            }
            if (frame >= 0xA) {
                work->unk0 = 5;
                entry->unk0 = -1;
                ex->func_800E3D54(work->unkB0);
                work->unk6C.vy = 0x400;
                work->unk4C.vy = 0x400;
                work->unk6C.vx = 0;
                work->unk6C.vz = 0;
                work->unk60.vx = 0x1000;
                work->unk60.vy = 0x1000;
                work->unk60.vz = 0x1000;
                work->unk4C.vx = 0;
                work->unk4C.vz = 0;
                work->unk40.vx = 0x333;
                work->unk40.vy = 0x333;
                work->unk40.vz = 0x333;
                work->unk34.vx = work->unkC.vx;
                work->unk34.vy = work->unkC.vy;
                work->unk34.vz = work->unkC.vz;
                p = ex->func_800E3CEC(ex->btlfx_getFileData(entry->unk8, 0x13), (s32)&work->unk14);
                if (p != NULL) {
                    p->unk22 = 0x80;
                }
                p = ex->func_800E3CEC(ex->btlfx_getFileData(entry->unk8, 0xF), (s32)&work->unk14);
                if (p != NULL) {
                    p->unk22 = 0x80;
                }
                ex->btlfx_drawScreenTint((u32 *)(env->ot + 1), 2, 0xFF, 0xFF, 0xFF);
            }
            break;
        case 5: {
            SVECTOR dir;
            CVECTOR col;
            SVECTOR rot;
            VECTOR sc;
            SVECTOR ang;
            SVECTOR at;
            SVECTOR ang2;
            SVECTOR at2;
            CVECTOR col2;
            SVECTOR rot2;
            VECTOR sc2;
            s32 u;
            s32 w;
            s32 h;

            u = frame << 12;
            env->unk28 = 0x100;
            t = u / 51;
            f5 = t >> 2;
            b = ex->math_cos(f5);
            a = ex->math_sin(f5) + 0xC00;
            ex->func_800D23D8((u8 *)ex->btlfx_getFileData(entry->unk8, 3), &work->unkC, NULL, a, frame, 1, 0xB86);
            ex->func_800D23D8((u8 *)ex->btlfx_getFileData(entry->unk8, 3), &work->unkC, NULL, a / 2, frame, 1, -1);
            ex->func_800D23D8((u8 *)ex->btlfx_getFileData(entry->unk8, 6), &work->unkC, NULL, (a * 2) / 3, frame, 1, b);
            a = ex->math_sin(f5) * 2 + 0x3000;
            ex->func_800D23D8((u8 *)ex->btlfx_getFileData(entry->unk8, 4), &work->unkC, NULL, a, 0, 1, -1);
            env->unk28 = 0;
            if (frame < 0x20) {
                f4 = u >> 7;
                a = (ex->math_sin(f4) * 3) >> 1;
                i = ex->math_cos(f4);
                dir.vx = 0x200;
                dir.vy = 0x280;
                dir.vz = work->unk4 * 32;
                dir.pad = 1;
                ex->func_800D23D8((u8 *)ex->btlfx_getFileData(entry->unk8, 5), &work->unkC, &dir, a, 0, 1, i);
                dir.vy += 0x800;
                ex->func_800D23D8((u8 *)ex->btlfx_getFileData(entry->unk8, 5), &work->unkC, &dir, a, 0, 1, i);
            }
            if (frame == 0) {
                ex->btlfx_drawScreenTint((u32 *)(env->ot + 1), 0xFF, 0xA0, 0, 0xFF);
            } else if (frame == 1) {
                ex->btlfx_drawScreenTint((u32 *)(env->ot + 1), 2, 0xFF, 0xFF, 0);
            } else if (frame < 6) {
                i = 0xFF - ((frame * 0xFF) / 6);
                ex->btlfx_drawScreenTint((u32 *)(env->ot + 1), 1, i >> 1, i >> 1, i);
            }
            work->unk6C.vz += 0x18;
            ex->btlfx_sampleGradient(D_801E9ACC, &col, frame);
            f6 = t >> 2;
            func_801E7718(&work->unkC, &work->unk6C, (VECTOR *)&work->unk60, 0x1C00, 0x1800, &col, 0x17, 0x3E80, 0, 0,
                          ((ex->math_sin(f6) * 0x58) >> 12) + 0x40, 0xFF);
            ex->btlfx_drawScrollMove(work->unk4 * 7, 0x1C0, 0x100, 0x20, 0x80, 0x2A0, 0x100);
            if (frame < 0x19) {
                work->unk4C.vz += 0xC;
                c = 0x1000 - ((frame << 12) / 25);
                ex->Hi_DrawEffModel(&work->unk4C, (VECTOR *)&work->unk34, (s32)&work->unk40, work->unk88);
                ex->Hi_ModifyEffModelRGB(work->unk88, ((ex->math_sin(c >> 2) - 0x800) >> 5) - 0x80,
                                         ((ex->math_sin(c >> 2) - 0x800) >> 5) - 0x80,
                                         ((ex->math_sin(c >> 2) - 0x800) >> 5) - 0x80);
            }
            rot.vx = 0x980;
            rot.vy = 0x400;
            rot.vz = 0;
            sc.vx = sc.vy = sc.vz = ex->math_sin(f6) + 0x800;
            ex->Hi_DrawEffModel(&rot, (VECTOR *)&work->unk34, (s32)&sc, work->unk8C);
            ex->Hi_ModifyEffModelRGB(work->unk8C, (((ex->math_cos(f6) >> 1) + 0x800) >> 5) - 0x80,
                                     (((ex->math_cos(f6) >> 1) + 0x800) >> 5) - 0x80,
                                     (((ex->math_cos(f6) >> 1) + 0x800) >> 5) - 0x80);
            ex->btlfx_drawBackdrop((BtlBgLayers *)ex->btlfx_getFileData(entry->unk8, 0x12), work->unk4);
            ex->btlfx_drawScrollMove(work->unk4 * 10, 0x2A0, 0x180, 0x10, 0x40, 0x2B0, 0x180);
            g2 = (ex->math_cos(f6) >> 1) + 0x800;
            if (frame < 0x18) {
                c = ((ex->math_sin((frame << 10) / 24) * 0x76C) >> 12) + 0x200;
            } else {
                c = 0x96C;
            }
            ang.vx = (ex->math_sin(t >> 2) * 2) / 3;
            ang.vy = work->unk4 << 3;
            ang.vz = 0;
            for (i = 0, f4 = work->unk4 * 0xC; i < 0x80; i++, f4 += 0x20) {
                ex->func_800D044C(&work->unkC, (s32)&at, 0, (XY32 *)&ang, c, f4);
                ex->btlfx_drawParticles((u8 *)ex->btlfx_getFileData(entry->unk8, 0xE), &at, NULL, 0x2800, 0, 1, g2, 0,
                                        0);
                ang.vx += 0x40;
                ang.vy -= 0x80;
            }
            h = (ex->math_cos(t >> 2) >> 1) + 0x1000;
            if (frame < 0x18) {
                w = (ex->math_sin((frame << 10) / 24) >> 2) + 0x6A4;
            } else {
                w = 0xAA4;
            }
            for (m = 0; m < 2; m++) {
                if (frame == 0) {
                    ex->btlfx_drawTrail((u8 *)ex->btlfx_getFileData(entry->unk8, 0x11), (s32)&work->unkC,
                                        (s32 *)&work->unkC, -1, h, ex->math_cos(t >> 2), 1, (u8 *)&D_801E9B44[m], 1);
                }
                {
                    SVECTOR *pp = D_801E9B44[m].unk4;

                    for (k = 0; k < 0x40; k++) {
                        pp->vy -= 0x40;
                        pp++;
                    }
                }
            }
            ang2.vx = ex->math_sin(work->unk4 << 7) >> 4;
            ang2.vy = work->unk4 * 0x52 - 0x180;
            ang2.vz = 0;
            for (k = 0, j = work->unk4 * 0x5C; k < 2; k++, j += 0x800) {
                ex->func_800D044C(&work->unkC, (s32)&at2, 0, (XY32 *)&ang2, w, j);
                ex->btlfx_drawTrail((u8 *)ex->btlfx_getFileData(entry->unk8, 0x11), (s32)&at2, (s32 *)&work->unkC, 0, h,
                                    (ex->math_sin(t >> 1) >> 1) + 0x60E, 1, (u8 *)&D_801E9B44[k], 1);
            }
            if (frame < 0x18) {
                k = ex->math_cos((frame << 10) / 24);
                ex->func_800D3B6C((u8 *)ex->btlfx_getFileData(entry->unk8, 0xB), &work->unkC, frame, k);
            }
            if (frame >= 8) {
                for (s = 0; s < 3; s++) {
                    q = ex->btlfx_poolSpawn((s16 *)entry->unk10);
                    if (q != NULL) {
                        s32 d;
                        s32 e;
                        q->unk0 = 4;
                        BTLFX_COPY_SVEC(&q->unkC, &work->unkC);
                        e = ex->btlfx_rand();
                        d = (ex->btlfx_rand() & 0x1FF) + 0x180;
                        q->unkC.vx = (s16)(q->unkC.vx - 0x100) + ex->btlfx_randCentered(0x200);
                        q->unkC.vy += (ex->math_cos(e) * d) >> 12;
                        q->unkC.vz += (ex->math_sin(e) * d) >> 12;
                        q->unkA = ex->btlfx_rand() % 70;
                        q->unk4 = (ex->btlfx_rand() & 0xFFF) + 0x1000;
                    }
                }
            }
            if (frame == 0x2B) {
                p = ex->func_800E3CEC(ex->btlfx_getFileData(entry->unk8, 0xD), (s32)&work->unk14);
                if (p != NULL) {
                    p->unk22 = 0x80;
                }
            }
            if (frame >= 0x2C) {
                x2 = (frame - 0x2B) << 12;
                k = x2 >> 7;
                if (k >= 0x100) {
                    k = 0xFF;
                }
                rot2.vz = 0x20;
                sc2.vy = sc2.vx = (x2 >> 5) + 0x1000;
                sc2.vz = 0x1000;
                col2.r = col2.g = col2.b = k;
                rot2.vx = 0;
                rot2.vy = 0;
                ex->btlfx_drawScreenGrid(2, 0xA0, 0x70, 0x4D, 0x37, 4, 4, 2, 2, &rot2, &sc2, &col2, 1, 0);
            }
            if (frame >= 0x33) {
                work->unk0 = 6;
                entry->unk0 = -1;
                ex->btlfx_poolClear(entry->unk10);
                p = ex->func_800E3CEC(ex->btlfx_getFileData(entry->unk8, 0xF), (s32)&D_801E9AC4);
                if (p != NULL) {
                    p->unk22 = 0x80;
                }
            }
            break;
        }
        case 6:
            BTLFX_COPY_SVEC(&work->unk24, &D_801E9AC4);
            env->camera->points[3].vx = work->unk24.vx;
            env->camera->points[3].vy = work->unk24.vy;
            t = frame << 8;
            env->camera->points[3].vz = work->unk24.vz;
            env->unk28 = 0x180;
            f1 = ex->math_cos(t >> 2) + 0x400;
            c = ex->math_cos(t >> 2);
            ex->func_800D23D8((u8 *)ex->btlfx_getFileData(entry->unk8, 3), &D_801E9AC4, NULL, f1, 0, 1, c);
            env->unk28 = 0;
            c = (ex->math_cos(t >> 2) * 0xFF) >> 12;
            ex->btlfx_drawScreenTint((u32 *)(env->ot + 1), 1, c, c, c);
            if (frame >= 0x10) {
                ex->btlfx_clearEffModels();
                return 1;
            }
            break;
        }
        }
        if (glow != 0) {
            CVECTOR col;
            SVECTOR rot;
            VECTOR sc;

            col.r = col.g = col.b = glow;
            rot.vx = 0;
            rot.vy = 0;
            rot.vz = 0;
            sc.vy = sc.vx = g + 0x1020;
            sc.vz = 0x1000;
            ex->btlfx_drawScreenGrid(0x200, 0xA0, 0x70, 0x4D, 0x37, 4, 4, 2, 2, &rot, &sc, &col, 0, 0);
        }
        work->unk4++;
    }
    return 0;
}
