#include "fxmod/fx_07a.h"

s32 func_801E77A8(s32 curve, s32 n, s32 t, SVECTOR *from, SVECTOR *to, SVECTOR *out) {
    BtlfxExports *ex = BTLFX_EXPORTS(D_801E94F4);
    s32 k = 0x20000000 / (n * n);
    s32 r = 0;

    switch (curve) {
    case 0:
        r = ((0x10000000 / n) * t) >> 16;
        break;
    case 1:
        r = 0x1000 - ((k * (n - t) * (n - t)) >> 17);
        break;
    case 2:
        r = (k * t * t) >> 17;
        break;
    }
    return ex->btlfx_lerpVec((s16 *)from, (s16 *)to, 0x1000 - r, (s16 *)out);
}

s32 func_801E78E4(s32 curve, s32 n, s32 t, s32 from, s32 to) {
    BtlfxExports *ex = BTLFX_EXPORTS(D_801E94F4);
    s32 k = 0x20000000 / (n * n);
    s32 r = 0;

    switch (curve) {
    case 0:
        r = 0x1000 - (((0x10000000 / n) * t) >> 16);
        break;
    case 1:
        r = (k * (n - t) * (n - t)) >> 17;
        break;
    case 2:
        r = 0x1000 - ((k * t * t) >> 17);
        break;
    }
    return ex->btlfx_lerp(from, to, r);
}

INCLUDE_RODATA("asm/us/fx_07a/nonmatchings/fx_07a", fx_07a_headerPtr);

s32 fx_07a_op80(s32 mode, Fx07aWork *work, Unk800F78A4 *env, Unk80100C78Entry *entry, s32 *arg4) {
    BtlfxExports *ex = BTLFX_EXPORTS(env);
    SVECTOR pos;
    SVECTOR out;
    VECTOR cur;
    VECTOR step;
    Fx07aStrip *tr;
    Fx07aStrip *q;
    Fx07aPt *pt;
    s32 frame;
    s32 i;
    s32 j;
    s32 n = 11;
    s32 w;
    s32 r;
    s32 a;
    s32 b;
    Pool6CEntry *p;

    if (mode == 0) {
        *arg4 = 0x108;
    } else if (mode == 1) {
        D_801E94F4 = env;
        ex->btlfx_initEffModels(D_801E9934, 3);
        for (i = 15; i >= 0; i--) {
            work->unk0[i] = 0;
        }
        for (i = 0; i < 5; i++) {
            D_801E9BA4[i].unk10C = -i;
            ex->btlfx_drawTrail((u8 *)ex->btlfx_getFileData(entry->unk8, 0xC), 0, NULL, -1, 0x1000, 0x1000, 1,
                                (u8 *)&D_801E9BA4[i], 0);
        }
        work->unk94.vx = D_801E9B94.vx = env->unkC->unk38;
        work->unk94.vy = D_801E9B94.vz = env->unkC->unk3C;
        work->unk94.vz = D_801E9B94.vy = -0x500;
        work->unk40[0].unk0 = ex->Hi_RegisterTexEffModel((BtlUnit9C8 *)ex->btlfx_getFileData(entry->unk8, 0xB), 0x137,
                                                         0xFFFF, 0, 0);
        work->unk40[0].unk4.vx = 0x800;
        work->unk40[0].unk4.vy = 0;
        work->unk40[0].unk4.vz = 0;
        work->unk40[0].unkC.vx = 0x5C;
        work->unk40[0].unkC.vy = 0x5C;
        work->unk40[0].unkC.vz = 0x5C;
        work->unk40[1].unk0 = ex->Hi_RegisterGouEffModel((BtlUnit9C8 *)ex->btlfx_getFileData(entry->unk8, 0xD));
        work->unk40[1].unk4.vx = 0;
        work->unk40[1].unk4.vy = 0;
        work->unk40[1].unk4.vz = 0;
        work->unk40[1].unkC.vx = 0x50;
        work->unk40[1].unkC.vy = 0x50;
        work->unk40[1].unkC.vz = 0x50;
        work->unk40[2].unk0 = ex->Hi_RegisterGouEffModel((BtlUnit9C8 *)ex->btlfx_getFileData(entry->unk8, 0xE));
        work->unk40[2].unk4.vx = 0;
        work->unk40[2].unk4.vy = 0;
        work->unk40[2].unk4.vz = 0;
        work->unk40[2].unkC.vx = 0x66;
        work->unk40[2].unkC.vy = 0x66;
        work->unk40[2].unkC.vz = 0x66;
        for (i = 0; i < env->unkC->unk24; i++) {
            ex->btlfx_getUnitHomePos(i, (s16 *)&work->unkA4[i]);
            work->unkE4[i] = ex->btlfx_rand() & 7;
        }
        D_801E94F8 = 0;
        D_801E9500 = 0;
        D_801E94FC = 0;
        D_801E9504 = 0;
        D_801E9514[4] = 0;
        D_801E9514[3] = 0;
        D_801E9514[2] = 0;
        D_801E9514[1] = 0;
        D_801E9514[0] = 0;
    } else {
        frame = entry->unk0;
        ex->func_800DC818(0x10, 0x17, (u16 *)&D_801E9B8C);
        switch (frame) {
        case 0:
            ex->func_800E6E1C(0x1C0, 0x17E, 0x70, 0x70, D_801E9534, D_801E9B94.vx, D_801E9B94.vz, 0x1208);
            break;
        case 1:
            work->unk0[9] = 1;
            work->unk0[1] = 1;
            work->unk0[0] = 0x24;
            work->unk0[2] = 8;
            work->unk0[4] = 0x10;
            break;
        case 8:
            work->unk0[10] = 1;
            work->unk0[11] = 8;
            work->unk0[12] = 0x20;
            work->unk0[6] = 0x1C;
            break;
        case 12:
            ex->func_800E3CEC(ex->btlfx_getFileData(entry->unk8, 6), (s32)&D_801E9B94);
            break;
        case 24:
            work->unk0[13] = 0x10;
            work->unk0[5] = 0x10;
            break;
        case 36:
            work->unk0[7] = 4;
            p = ex->func_800E3CEC(ex->btlfx_getFileData(entry->unk8, 9), (s32)&D_801E9B94);
            if (p != NULL) {
                p->unk22 = ex->func_800E7CC4(0, 0x80);
            }
            break;
        case 40:
            work->unk0[10] = 0;
            ex->btlfx_drawScreenTint((u32 *)env->unk20, 0xFF, 0xFF, 0xFF, 0xFF);
            work->unk0[1] = 0;
            break;
        }
        pos.vx = D_801E9B94.vx;
        pos.vy = 0;
        pos.vz = D_801E9B94.vz;
        if (work->unk0[11] != 0) {
            work->unk0[11]--;
            D_801E9508 = func_801E78E4(0, 8, work->unk0[11], 0, 0x4A00);
            D_801E94F8 = func_801E78E4(0, 8, work->unk0[11], 0, 0x5A00);
            D_801E950C = func_801E78E4(0, 8, work->unk0[11], 0, 0xB20);
            D_801E9500 = func_801E78E4(0, 8, work->unk0[11], 0, 0x340);
        }
        if (work->unk0[12] != 0) {
            work->unk0[12]--;
            D_801E9514[1] = func_801E78E4(1, 0x20, work->unk0[12], 0, 0x1000);
            D_801E9514[2] = func_801E78E4(1, 0x20, work->unk0[12], 0, 0xEAB);
            D_801E9514[3] = func_801E78E4(1, 0x20, work->unk0[12], 0, 0xD56);
            D_801E9514[4] = func_801E78E4(1, 0x20, work->unk0[12], 0, 0xC00);
            D_801E94FC = func_801E78E4(1, 0x20, work->unk0[12], 0, 0x340);
        }
        if (work->unk0[13] != 0) {
            work->unk0[13]--;
            D_801E9508 = func_801E78E4(0, 0x10, work->unk0[12], 0x4A00, 0);
            D_801E950C = func_801E78E4(0, 0x10, work->unk0[13], 0xB20, 0);
            D_801E94F8 = func_801E78E4(0, 0x10, work->unk0[12], 0x5A00, 0);
        }
        if (work->unk0[10] != 0) {
            func_801E8A88(&pos, 0xFFF, 0x117, -1, 0xFF);
        }
        if (work->unk0[2] != 0) {
            work->unk0[2]--;
            func_801E77A8(0, 8, work->unk0[2], &D_801E9B8C, &D_801E9B94, &out);
            work->unk94.vx = out.vx;
            work->unk94.vy = out.vy;
            work->unk94.vz = out.vz;
        }
        if (work->unk0[4] != 0) {
            work->unk0[4]--;
            work->unk40[0].unkC.vx = work->unk40[0].unkC.vy = work->unk40[0].unkC.vz =
                func_801E78E4(0, 0x10, work->unk0[4], 0x5C, 0x2A2);
            work->unk40[1].unkC.vx = work->unk40[1].unkC.vy = work->unk40[1].unkC.vz =
                func_801E78E4(0, 0x10, work->unk0[4], 0x50, 0x243);
            work->unk40[2].unkC.vx = work->unk40[2].unkC.vy = work->unk40[2].unkC.vz =
                func_801E78E4(0, 0x10, work->unk0[4], 0x66, 0x299);
        }
        if (work->unk0[5] != 0) {
            work->unk0[5]--;
            work->unk40[0].unkC.vx = work->unk40[0].unkC.vy = work->unk40[0].unkC.vz =
                func_801E78E4(0, 0x10, work->unk0[5], 0x2A2, 0);
            work->unk40[1].unkC.vx = work->unk40[1].unkC.vy = work->unk40[1].unkC.vz =
                func_801E78E4(0, 0x10, work->unk0[5], 0x243, 0);
            work->unk40[2].unkC.vx = work->unk40[2].unkC.vy = work->unk40[2].unkC.vz =
                func_801E78E4(0, 0x10, work->unk0[5], 0x299, 0);
        }
        if (work->unk0[6] != 0) {
            work->unk0[6]--;
            for (i = 0; i < env->unkC->unk24; i++) {
                if (0x1C - work->unk0[6] >= work->unkE4[i]) {
                    pos.vx = work->unkA4[i].vx;
                    pos.vz = work->unkA4[i].vz;
                    pos.vy = func_801E78E4(2, 0x1C - work->unkE4[i], work->unk0[6], work->unkA4[i].vy,
                                           work->unkA4[i].vy - 0x200);
                    ex->btlfx_setUnitPos(i, (s16 *)&pos);
                }
            }
        }
        if (work->unk0[7] != 0) {
            work->unk0[7]--;
            for (i = 0; i < env->unkC->unk24; i++) {
                if (work->unk0[7] != 0) {
                    pos.vx = work->unkA4[i].vx;
                    pos.vz = work->unkA4[i].vz;
                    pos.vy = func_801E78E4(2, 4, work->unk0[7], work->unkA4[i].vy - 0x200, work->unkA4[i].vy);
                } else {
                    ex->btlfx_getUnitHomePos(i, (s16 *)&pos);
                }
                ex->btlfx_setUnitPos(i, (s16 *)&pos);
            }
        }
        if (work->unk0[1] != 0) {
            ex->btlfx_aimAngles((s16 *)&D_801E9B94, (s16 *)&env->unk2C->unk54, (s16 *)&work->unk40[1].unk4);
            work->unk40[0].unk4.vx = work->unk40[1].unk4.vx + 0x800;
            work->unk40[0].unk4.vy = work->unk40[1].unk4.vy;
            work->unk40[2].unk4.vy = work->unk40[1].unk4.vy;
            work->unk40[2].unk4.vx = work->unk40[1].unk4.vx + 0x400;
            work->unk40[1].unk4.vx -= 0x400;
            ex->Hi_DrawEffModel(&work->unk40[1].unk4, &work->unk94, (s32)&work->unk40[1].unkC, work->unk40[1].unk0);
            ex->Hi_DrawEffModel(&work->unk40[2].unk4, &work->unk94, (s32)&work->unk40[2].unkC, work->unk40[2].unk0);
            ex->Hi_DrawEffModel(&work->unk40[0].unk4, &work->unk94, (s32)&work->unk40[0].unkC, work->unk40[0].unk0);
            ex->Hi_ModifyEffModelAbr(work->unk40[0].unk0, 1);
        }
        if (work->unk0[0] != 0) {
            work->unk0[0]--;
            for (i = 0; i < 5; i++) {
                tr = &D_801E9BA4[i];
                if (tr->unk10C > 0) {
                    tr->unk10C--;
                    tr->unk0 = tr->unk2 = n - 1;
                    tr->unk1 = 0;
                    cur.vx = D_801E9B8C.vx << 12;
                    cur.vy = D_801E9B8C.vy << 12;
                    cur.vz = D_801E9B8C.vz << 12;
                    step.vx = ((work->unk94.vx + tr->unk5C - D_801E9B8C.vx) << 12) / n;
                    step.vy = ((work->unk94.vy + tr->unk60 - D_801E9B8C.vy) << 12) / n;
                    step.vz = ((work->unk94.vz + tr->unk64 - D_801E9B8C.vz) << 12) / n;
                    q = tr;
                    for (j = 0, pt = tr->unk4; j < n; pt++, j++) {
                        pt->vx = cur.vx >> 12;
                        pt->vz = cur.vz >> 12;
                        pt->vy = tr->unk6C[j] + (cur.vy >> 12);
                        pt->unk6 = (u16)tr->unk10C * 4;
                        cur.vx += step.vx;
                        cur.vy += step.vy;
                        cur.vz += step.vz;
                        tr->unk6C[j] += tr->unkBC[j];
                    }
                    if (tr->unk10C < 4) {
                        ex->btlfx_drawTrail((u8 *)ex->btlfx_getFileData(entry->unk8, 0xC), 0, NULL, frame, 0x1000,
                                            tr->unk10C << 10, 1, (u8 *)q, 0);
                    } else {
                        ex->btlfx_drawTrail((u8 *)ex->btlfx_getFileData(entry->unk8, 0xC), 0, NULL, frame, 0x1000,
                                            0x1000, 1, (u8 *)q, 0);
                    }
                } else if (tr->unk10C < 0) {
                    tr->unk10C++;
                } else if (work->unk0[0] >= 5) {
                    tr->unk10C = ex->btlfx_randRange(4, 8);
                    j = 1;
                    a = ex->btlfx_rand() & 0xFFF;
                    b = ex->btlfx_rand() & 0xFFF;
                    r = (ex->math_cos(a) * (work->unk40[0].unkC.vx + 0x80)) >> 12;
                    tr->unk60 = r;
                    tr->unk5C = (ex->math_sin(b) * abs(r)) >> 12;
                    tr->unk64 = (ex->math_cos(b) * abs(r)) >> 12;
                    tr->unkBC[0] = 0;
                    tr->unk6C[0] = 0;
                    for (; j < n - 1; j++) {
                        w = j * 8 + 0x10;
                        tr->unk6C[j] = ex->btlfx_randRange(-w * 2, w * 2);
                        tr->unkBC[j] = ex->btlfx_randRange(-w, w);
                    }
                    tr->unkBC[j] = 0;
                    tr->unk6C[j] = 0;
                }
            }
        }
        if (work->unk0[9] != 0) {
            ex->btlfx_drawBackdrop((BtlBgLayers *)ex->btlfx_getFileData(entry->unk8, 8), work->unk0[9]);
            work->unk0[9]++;
        }
        return frame >= 0x46;
    }
    return 0;
}

void func_801E8A88(SVECTOR *pos, s32 ot, s32 tpage, s32 clut, s32 abr) {
    Unk800F78A4 *env = D_801E94F4;
    BtlfxExports *ex = BTLFX_EXPORTS(env);
    u8 uv[4][8];
    SVECTOR p[4];
    SVECTOR rot;
    MATRIX m;
    s32 dr;
    s32 dk;
    POLY_FT4 *ft4;
    POLY_G4 *g4;
    s32 a;
    s32 j;
    s32 r;
    s32 k;
    s32 b;

    p[3].vy = 0;
    p[2].vy = 0;
    p[1].vy = 0;
    p[0].vy = 0;
    rot.vz = 0;
    rot.vy = 0;
    rot.vx = 0;
    gte_SetRotMatrix(env->unk14);
    gte_SetTransMatrix(env->unk14);
    gte_ldv0(pos);
    ft4 = (POLY_FT4 *)env->unk24;
    tpage |= (abr & 3) << 5;
    g4 = (POLY_G4 *)(ft4 + 64);
    env->unk24 = (u8 *)(g4 + 32);
    gte_rtv0tr_b();
    ex->math_rotMatrixYXZ(&rot, &m);
    gte_stlvnl(m.t);
    ex->MulRotMatrix(&m);
    gte_SetRotMatrix(&m);
    gte_SetTransMatrix(&m);
    a = 0;
    dr = D_801E94F8 >> 2;
    dk = (D_801E9500 - D_801E94FC) >> 2;
    for (; a < 0x1000; a += 0x100) {
        r = 0;
        k = 0;
        for (j = 0; j < 4; j++) {
            b = a + D_801E9514[j];
            uv[0][0] = ((ex->math_cos(a) * (D_801E94FC + k)) >> 16) + 0x38;
            uv[0][1] = ((ex->math_sin(a) * (D_801E94FC + k)) >> 16) - 0x48;
            uv[1][0] = ((ex->math_cos(a + 0x100) * (D_801E94FC + k)) >> 16) + 0x38;
            uv[1][1] = ((ex->math_sin(a + 0x100) * (D_801E94FC + k)) >> 16) - 0x48;
            p[0].vx = (ex->math_cos(b) * r) >> 16;
            p[0].vz = -((ex->math_sin(b) * r) >> 16) - 0x20;
            b += 0x100;
            p[1].vx = (ex->math_cos(b) * r) >> 16;
            p[1].vz = -((ex->math_sin(b) * r) >> 16) - 0x20;
            b = a + D_801E9514[j + 1];
            k += dk;
            uv[2][0] = ((ex->math_cos(a) * (D_801E94FC + k)) >> 16) + 0x38;
            uv[2][1] = ((ex->math_sin(a) * (D_801E94FC + k)) >> 16) - 0x48;
            uv[3][0] = ((ex->math_cos(a + 0x100) * (D_801E94FC + k)) >> 16) + 0x38;
            r += dr;
            uv[3][1] = ((ex->math_sin(a + 0x100) * (D_801E94FC + k)) >> 16) - 0x48;
            p[2].vx = (ex->math_cos(b) * r) >> 16;
            p[2].vz = -((ex->math_sin(b) * r) >> 16) - 0x20;
            b += 0x100;
            p[3].vx = (ex->math_cos(b) * r) >> 16;
            p[3].vz = -((ex->math_sin(b) * r) >> 16) - 0x20;
            gte_ldv3c(p);
            gte_rtpt();
            ft4->b0 = 0x40;
            ft4->g0 = 0x40;
            ft4->r0 = 0x40;
            ft4->code = abr == 0xFF ? 0x2C : 0x2E;
            *(u16 *)&ft4->u0 = *(u16 *)uv[0];
            *(u16 *)&ft4->u1 = *(u16 *)uv[1];
            *(u16 *)&ft4->u2 = *(u16 *)uv[2];
            *(u16 *)&ft4->u3 = *(u16 *)uv[3];
            gte_stsxy3(&ft4->x0, &ft4->x1, &ft4->x2);
            gte_ldv0(&p[3]);
            gte_rtps();
            ft4->clut = clut;
            ft4->tpage = tpage;
            gte_stsxy(&ft4->x3);
            ex->btlfx_addPrim(0x09000000, ot * 4, ft4);
            ft4++;
        }
        for (j = 0; j < 2; j++) {
            if (j != 0) {
                p[2].vx = (ex->math_cos(a) * D_801E9508) >> 16;
                p[2].vz = -((ex->math_sin(a) * D_801E9508) >> 16) - 0x20;
                p[3].vx = (ex->math_cos(a + 0x100) * D_801E9508) >> 16;
                p[3].vz = -((ex->math_sin(a + 0x100) * D_801E9508) >> 16) - 0x20;
                p[0].vx = (ex->math_cos(a) * D_801E94F8) >> 16;
                p[0].vz = -((ex->math_sin(a) * D_801E94F8) >> 16) - 0x20;
                p[1].vx = (ex->math_cos(a + 0x100) * D_801E94F8) >> 16;
                p[1].vz = -((ex->math_sin(a + 0x100) * D_801E94F8) >> 16) - 0x20;
                g4->b1 = 0x80;
                g4->g1 = 0x80;
                g4->r1 = 0x80;
                g4->b0 = 0x80;
                g4->g0 = 0x80;
                g4->r0 = 0x80;
                g4->b3 = 0;
                g4->g3 = 0;
                g4->r3 = 0;
                g4->b2 = 0;
                g4->g2 = 0;
                g4->r2 = 0;
            } else {
                p[2].vx = (ex->math_cos(a) * D_801E94F8) >> 16;
                p[2].vz = -((ex->math_sin(a) * D_801E94F8) >> 16) - 0x20;
                p[3].vx = (ex->math_cos(a + 0x100) * D_801E94F8) >> 16;
                p[3].vz = -((ex->math_sin(a + 0x100) * D_801E94F8) >> 16) - 0x20;
                p[0].vx = (ex->math_cos(a) * D_801E950C) >> 12;
                p[0].vz = -((ex->math_sin(a) * D_801E950C) >> 12) - 0x20;
                p[1].vx = (ex->math_cos(a + 0x100) * D_801E950C) >> 12;
                p[1].vz = -((ex->math_sin(a + 0x100) * D_801E950C) >> 12) - 0x20;
                g4->b1 = 0;
                g4->g1 = 0;
                g4->r1 = 0;
                g4->b0 = 0;
                g4->g0 = 0;
                g4->r0 = 0;
                g4->b3 = 0xFF;
                g4->g3 = 0xFF;
                g4->r3 = 0xFF;
                g4->b2 = 0xFF;
                g4->g2 = 0xFF;
                g4->r2 = 0xFF;
            }
            gte_ldv3c(p);
            gte_rtpt();
            g4->code = 0x3A;
            gte_stsxy3(&g4->x0, &g4->x1, &g4->x2);
            gte_ldv0(&p[3]);
            gte_rtps();
            gte_stsxy(&g4->x3);
            ex->btlfx_addPrim(0x08000000, ot * 4 - 4, g4);
            g4++;
        }
    }
    ex->btlfx_addPrim(0x01000000, ot * 4 - 4, ex->btlfx_allocDrMode(0, 2));
}
