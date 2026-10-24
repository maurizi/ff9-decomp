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
    prim = (POLY_G4 *)env->prim;
    env->prim = (s32)(prim + n);
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
    gte_SetRotMatrix(env->viewMatrix);
    gte_SetTransMatrix(env->viewMatrix);
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

void func_801E7BF8(void) {
    SVECTOR *ext = D_801EAC68;
    s32 i;
    Unk800F78A4 *env = D_801EAB18;
    s32 frame = D_801EAB1C;
    s32 k;
    s32 d;
    s32 s;
    s32 x, y, z;
    s32 j;

    for (i = 0; i < 16; i++) {
        if (D_801EAB28[i].unk4 == 0 && D_801EAB28[i].unk0 == frame) {
            D_801EAB28[i].unk4 = *D_801EAB28[i].unk10;
            D_801EAB28[i].unk10++;
        }
        if (D_801EAB28[i].unk4 > 0) {
            D_801EAB28[i].unk4--;
            for (k = 0; k < 3; k++) {
                d = ((D_801EAB28[i].unk8 >> (k * 4)) & 0xF) - 1;
                s = ((D_801EAB28[i].unkC >> (k * 4)) & 0xF) - 1;
                if (d >= 0) {
                    x = *D_801EAB28[i].unk10;
                    D_801EAB28[i].unk10++;
                    y = *D_801EAB28[i].unk10;
                    D_801EAB28[i].unk10++;
                    z = *D_801EAB28[i].unk10;
                    D_801EAB28[i].unk10++;
                    if (s >= 0) {
                        /* each branch adds its own source; jump2's cross-jumping merges the identical tails */
                        if (s >= 10) {
                            j = s - 10;
                            x += ext[j].vx;
                            y += ext[j].vy;
                            z += ext[j].vz;
                        } else {
                            x += ((SVECTOR *)env->camera)[s].vx;
                            y += ((SVECTOR *)env->camera)[s].vy;
                            z += ((SVECTOR *)env->camera)[s].vz;
                        }
                    }
                    if (d >= 10) {
                        j = d - 10;
                        ext[j].vx = x;
                        ext[j].vy = y;
                        ext[j].vz = z;
                    } else {
                        ((SVECTOR *)env->camera)[d].vx = x;
                        ((SVECTOR *)env->camera)[d].vy = y;
                        ((SVECTOR *)env->camera)[d].vz = z;
                    }
                }
            }
        }
    }
    D_801EAB1C++;
}

void func_801E7D9C(s32 z, s32 tpage, s32 y0, s32 y1, CVECTOR *rgb) {
    POLY_G4 *p = (POLY_G4 *)D_801EAB18->prim;
    BtlfxExports *ex = BTLFX_EXPORTS(D_801EAB18);

    D_801EAB18->prim = (u8 *)(p + 1);
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

s32 fx_105_op80(s32 mode, Fx105Work80 *work, Unk800F78A4 *env, Unk80100C78Entry *entry, s32 *arg4) {
    BtlfxExports *ex = BTLFX_EXPORTS(env);
    BtlBone bone1;
    BtlBone bone2;
    CVECTOR c1[4];
    CVECTOR c2[4];
    CVECTOR c3[4];
    CVECTOR c4[4];
    s32 frame;
    s32 i;
    s32 a;
    s32 z;
    s32 tp;
    s32 ka;
    s32 s4;
    s32 vC;
    s32 v1C;
    s32 w1C;
    s32 v20;
    s32 w20;
    s32 w2C;
    s32 m14;
    s32 k8;

    if (mode == 0) {
        *arg4 = 0x1D8;
    } else if (mode == 1) {
        D_801EAB18 = env;
        ex->btlfx_initEffModels(D_801EADD8, 0xF);
        for (i = 0xF; i >= 0; i--) {
            (&work->unk0)[i] = 0;
        }
        for (i = 0; i < 0x20; i++) {
            D_801EB998[i].unk0 = ex->btlfx_randRange(0xC0, 0x1E0);
            D_801EB998[i].unk4 = ex->btlfx_randRange(0, 0xDA);
            D_801EB998[i].unk8 = (ex->btlfx_rand() & 7) + 4;
            D_801EB998[i].unk10 = 1;
        }
        D_801EB998[0].unk0 = 0x10;
        D_801EB998[0].unk4 = 0x10;
        D_801EB998[1].unk0 = 0x20;
        D_801EB998[1].unk4 = 0x18;
        D_801EB998[2].unk0 = 8;
        D_801EB998[2].unk4 = 0x28;
        D_801EB998[3].unk0 = 0x14;
        D_801EB998[3].unk4 = 0xB4;
        D_801EB998[4].unk0 = 0x2D;
        D_801EB998[4].unk4 = 0xD2;
        D_801EB998[5].unk0 = 0x3C;
        D_801EB998[5].unk4 = 0xC3;
        D_801EB998[6].unk0 = 0x64;
        D_801EB998[6].unk4 = 0xBE;
        D_801EADA8.vx = 0;
        D_801EADA8.vy = 0x800;
        D_801EADA8.vz = 0;
        D_801EADC8.vx = 0x1000;
        D_801EADC8.vy = 0x1000;
        D_801EADC8.vz = 0x1000;
        D_801EADB8.vx = 0;
        D_801EADB8.vy = 0;
        D_801EADB8.vz = 0;
        work->unk124 = NULL;
        work->unk40 = NULL;
        work->unk44 = NULL;
        work->unk48 = NULL;
        work->unk4C = NULL;
        work->unk50 = NULL;
        work->unk54 = NULL;
        work->unkA8 = NULL;
        work->unkAC = NULL;
        work->unkB0 = NULL;
        work->unkD4 = NULL;
        work->unkD8 = NULL;
        work->unkDC = NULL;
        work->unkE0 = NULL;
        work->unk104.vx = 0;
        work->unk104.vy = 0;
        work->unk104.vz = env->targets->centerZ;
        work->unk58.vx = 0;
        work->unk58.vy = -0x62C;
        work->unk58.vz = -0x186A;
        work->unk60.vx = 0;
        work->unk60.vy = -0x62C;
        work->unk60.vz = -0x186A;
        work->unk68.vx = 0;
        work->unk68.vy = -0x62C;
        work->unk68.vz = -0x186A;
        work->unk128.vx = -0x304;
        work->unk128.vy = -0x26FC;
        work->unk128.vz = -0x2A94;
        work->unk130.vx = -0x304;
        work->unk130.vy = -0x26FC;
        work->unk130.vz = -0x2A94;
        env->camera->unk30[2].vx = -0x304;
        env->camera->unk30[2].vy = -0x26FC;
        env->camera->unk30[2].vz = -0x2A94;
        work->unk138[0] = ex->btlfx_registerEffectModel((Unk800E8F50 *)ex->btlfx_getFileData(entry->unk8, 0xD), 0xFF);
        work->unk138[1] = ex->btlfx_registerEffectModel((Unk800E8F50 *)ex->btlfx_getFileData(entry->unk8, 0x15), 0xFF);
        work->unk138[2] = ex->btlfx_registerEffectModel((Unk800E8F50 *)ex->btlfx_getFileData(entry->unk8, 0x29), 0xFF);
        work->unk138[3] = ex->btlfx_registerEffectModel((Unk800E8F50 *)ex->btlfx_getFileData(entry->unk8, 0x2A), 0xFF);
        work->unk138[4] = ex->btlfx_registerEffectModel((Unk800E8F50 *)ex->btlfx_getFileData(entry->unk8, 0x2B), 0xFF);
        work->unk138[5] = ex->btlfx_registerEffectModel((Unk800E8F50 *)ex->btlfx_getFileData(entry->unk8, 0x2C), 0xFF);
        work->unk138[6] = ex->btlfx_registerEffectModel((Unk800E8F50 *)ex->btlfx_getFileData(entry->unk8, 0x2D), 0xFF);
        work->unk138[7] = ex->btlfx_registerEffectModel((Unk800E8F50 *)ex->btlfx_getFileData(entry->unk8, 0x2E), 0xFF);
        work->unk170[0] = 0x2000;
        work->unk170[1] = 0x2000;
        work->unk158[0] = 0;
        work->unk158[1] = 0;
        work->unk158[2] = 0;
        work->unk168.vx = 0;
        work->unk168.vy = 0;
        work->unk168.vz = 0;
        work->unk170[2] = 0x1A80;
        work->unk180 = ex->btlfx_registerEffectModel((Unk800E8F50 *)ex->btlfx_getFileData(entry->unk8, 0x80), 2);
        work->unk184 = ex->btlfx_registerEffectModel((Unk800E8F50 *)ex->btlfx_getFileData(entry->unk8, 0x81), 2);
        work->unk188 = ex->btlfx_registerEffectModel((Unk800E8F50 *)ex->btlfx_getFileData(entry->unk8, 0x82), 2);
        work->unk18C = ex->btlfx_registerEffectModel((Unk800E8F50 *)ex->btlfx_getFileData(entry->unk8, 0x83), 2);
        work->unk1A8[0] = 0x200;
        work->unk1A8[1] = 0x200;
        work->unk1A8[2] = 0x200;
        work->unkB4.vz = -0x800;
        work->unkBC.vz = -0x800;
        work->unkC4.vz = -0x800;
        work->unk11C.b = 0x40;
        work->unk11C.g = 0x40;
        work->unk120.g = 0x1E;
        work->unk198[0] = 0;
        work->unk198[1] = 0;
        work->unk198[2] = 0;
        work->unk190.vx = 0;
        work->unk190.vy = 0;
        work->unk190.vz = 0;
        work->unkB4.vx = 0;
        work->unkB4.vy = 0;
        work->unkBC.vx = 0;
        work->unkBC.vy = 0;
        work->unkC4.vx = 0;
        work->unkC4.vy = 0;
        work->unkCC.vx = 0;
        work->unkCC.vy = 0x800;
        work->unkCC.vz = 0;
        work->unk11C.r = 0;
        work->unk120.r = 0;
        work->unk120.b = 0x10;
        func_801E7704(&work->unk10C, &work->unk114, 0x408, &work->unk11C, &work->unk120, 0x1000, D_801EAC98, 1);
        func_801E9E1C();
        func_801E7B74();
        func_801E7BA8(0, 3, 0, (s16 *)ex->btlfx_getFileData(entry->unk8, 5));
        func_801E7BA8(0, 1, 0, (s16 *)ex->btlfx_getFileData(entry->unk8, 2));
        func_801E7BA8(0, 2, 0, (s16 *)ex->btlfx_getFileData(entry->unk8, 8));
        func_801E7BA8(0, 9, 0, (s16 *)ex->btlfx_getFileData(entry->unk8, 7));
        func_801E7BA8(0, 4, 0, (s16 *)ex->btlfx_getFileData(entry->unk8, 6));
        func_801E7BA8(0xA7, 3, 0, (s16 *)ex->btlfx_getFileData(entry->unk8, 0xA));
        func_801E7BA8(0xA7, 1, 0, (s16 *)ex->btlfx_getFileData(entry->unk8, 9));
        func_801E7BA8(0xA7, 2, 0, (s16 *)ex->btlfx_getFileData(entry->unk8, 0x16));
        func_801E7BA8(0xFB, 3, 0, (s16 *)ex->btlfx_getFileData(entry->unk8, 1));
        func_801E7BA8(0xFB, 1, 0, (s16 *)ex->btlfx_getFileData(entry->unk8, 0x17));
        func_801E7BA8(0xFB, 2, 0, (s16 *)ex->btlfx_getFileData(entry->unk8, 0x19));
        func_801E7BA8(0x137, 3, 0, (s16 *)ex->btlfx_getFileData(entry->unk8, 0x1B));
        func_801E7BA8(0x137, 1, 0, (s16 *)ex->btlfx_getFileData(entry->unk8, 0x1A));
        func_801E7BA8(0x137, 2, 0, (s16 *)ex->btlfx_getFileData(entry->unk8, 0x1C));
        return 0;
    } else {
        frame = entry->unk0;
        switch (frame) {
        case 0:
            work->unk124 = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->unk8, 0x11), (s32)&work->unk128);
            if (work->unk124 != NULL) {
                work->unk124->unk22 = -0x400;
            }
            work->unk8 = 0x71;
            work->unkC = 0x93;
            work->unk30 = 0x78;
            break;
        case 0xF:
            ex->Hi_SetSummonMotion(SUMMON105(env)->unk54->unk180[0], SUMMON105(env)->unk60);
            work->unk0 = 1;
            break;
        case 0x1E:
            work->unk4 = 0x10;
            break;
        case 0x21:
            ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->unk8, 3), (s32)&work->unk58);
            ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->unk8, 0xB), (s32)&work->unk60);
            ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->unk8, 4), (s32)&work->unk68);
            work->unk40 = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->unk8, 0xE), (s32)&work->unk70);
            work->unk44 = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->unk8, 0xE), (s32)&work->unk78);
            break;
        case 0x41:
            work->unk10 = 1;
            break;
        case 0x51:
            work->unk48 = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->unk8, 0x1F), (s32)&work->unk80);
            work->unk4C = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->unk8, 0x1F), (s32)&work->unk90);
            work->unk50 = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->unk8, 0x1F), (s32)&work->unk88);
            work->unk54 = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->unk8, 0x1F), (s32)&work->unk98);
            work->unk24 = 0x30;
            break;
        case 0x55:
            ex->Hi_SetSummonMotion(SUMMON105(env)->unk54->unk180[1], SUMMON105(env)->unk60);
            break;
        case 0x7F:
            ex->Hi_SetSummonMotion(SUMMON105(env)->unk54->unk180[2], SUMMON105(env)->unk60);
            break;
        case 0x93:
            ex->btlfx_stopEmitter(work->unk48);
            ex->btlfx_stopEmitter(work->unk4C);
            ex->btlfx_stopEmitter(work->unk50);
            ex->btlfx_stopEmitter(work->unk54);
            work->unk10 = 0;
            work->unk24 = 0;
            work->unk30 = 0;
            work->unk0 = 0;
            ex->Hi_SetSummonMotion(SUMMON105(env)->unk54->unk180[3], SUMMON105(env)->unk60);
            break;
        case 0x9C ... 0xA6:
            ex->Hi_SetSummonMotFrame(SUMMON105(env)->unk60, 0);
            work->unk0 = 1;
            break;
        case 0xA7:
            ex->Hi_SetSummonMotFrame(SUMMON105(env)->unk60, 0);
            break;
        case 0xAB:
            work->unkA0.vx = env->camera->points[2].vx;
            work->unkA0.vy = env->camera->points[2].vy;
            work->unkA0.vz = env->camera->points[2].vz;
            ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->unk8, 0x1D), (s32)&work->unkA0);
            break;
        case 0xD8:
            work->unk14 = 0x21;
            break;
        case 0xE0:
            work->unk28 = 8;
            break;
        case 0xE7:
            ex->Hi_SetSummonMotion(SUMMON105(env)->unk54->unk180[4], SUMMON105(env)->unk60);
            break;
        case 0xEC:
            work->unk1C = 0x10;
            break;
        case 0xFC:
            ex->Hi_SetSummonMotion(SUMMON105(env)->unk54->unk180[5], SUMMON105(env)->unk60);
            for (i = 0; i < env->targets->targetCount; i++) {
                work->unk1B8[i] = ex->btlfx_holdMotion(i);
            }
            work->unk20 = 0x30;
            work->unk18 = 0x3C;
            work->unkA8 = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->unk8, 0x1E), (s32)&work->unkB4);
            work->unkAC = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->unk8, 0x1E), (s32)&work->unkBC);
            work->unkB0 = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->unk8, 0x1E), (s32)&work->unkC4);
            if (work->unkA8 != NULL) {
                work->unkA8->unk22 = 0x100;
            }
            if (work->unkAC != NULL) {
                work->unkAC->unk22 = 0x100;
            }
            if (work->unkB0 != NULL) {
                work->unkB0->unk22 = 0x100;
            }
            break;
        case 0x138:
            ex->Hi_SetSummonMotion(SUMMON105(env)->unk54->unk180[6], SUMMON105(env)->unk60);
            ex->btlfx_stopEmitter(work->unkA8);
            ex->btlfx_stopEmitter(work->unkAC);
            ex->btlfx_stopEmitter(work->unkB0);
            work->unkD4 = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->unk8, 0x28), (s32)&work->unkE4);
            work->unkD8 = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->unk8, 0x28), (s32)&work->unkEC);
            work->unkDC = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->unk8, 0x28), (s32)&work->unkF4);
            work->unkE0 = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->unk8, 0x28), (s32)&work->unkFC);
            if (work->unkD4 != NULL) {
                work->unkD4->unk22 = 0x100;
            }
            if (work->unkD8 != NULL) {
                work->unkD8->unk22 = 0x100;
            }
            break;
        case 0x146:
            for (i = 0; i < env->targets->targetCount; i++) {
                ex->btlfx_setMotionHeld(i, work->unk1B8[i]);
            }
            break;
        case 0x165:
            ex->btlfx_stopEmitter(work->unkD4);
            ex->btlfx_stopEmitter(work->unkD8);
            ex->btlfx_stopEmitter(work->unkDC);
            ex->btlfx_stopEmitter(work->unkE0);
            work->unk0 = 0;
            ex->btlfx_clearEffModels();
            work->unk2C = 0x1C;
            ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->unk8, 0x3B), (s32)&work->unk104);
            break;
        case 0x185:
            env->camera->points[0].vx = 0;
            env->camera->points[0].vy = -0x200;
            env->camera->points[0].vz = 0x200;
            break;
        case 0x1B0:
            return 1;
        }
        if (work->unk40 != NULL) {
            if (work->unk40->unk30 == -1) {
                work->unk40 = NULL;
            }
            ex->Hi_GetSummonBonePos(SUMMON105(env)->unk60, 0x4C, (u16 *)&work->unk70);
        }
        if (work->unk44 != NULL) {
            if (work->unk44->unk30 == -1) {
                work->unk44 = NULL;
            }
            ex->Hi_GetSummonBonePos(SUMMON105(env)->unk60, 0x60, (u16 *)&work->unk78);
        }
        if (work->unk48 != NULL) {
            if (work->unk48->unk30 == -1) {
                work->unk48 = NULL;
            }
            ex->Hi_GetSummonBonePos(SUMMON105(env)->unk60, 0x4C, (u16 *)&work->unk80);
        }
        if (work->unk4C != NULL) {
            if (work->unk4C->unk30 == -1) {
                work->unk4C = NULL;
            }
            ex->Hi_GetSummonBonePos(SUMMON105(env)->unk60, 0x60, (u16 *)&work->unk90);
        }
        if (work->unk50 != NULL) {
            if (work->unk50->unk30 == -1) {
                work->unk50 = NULL;
            }
            ex->Hi_GetSummonBonePos(SUMMON105(env)->unk60, 0x3E, (u16 *)&work->unk88);
        }
        if (work->unk54 != NULL) {
            if (work->unk54->unk30 == -1) {
                work->unk54 = NULL;
            }
            ex->Hi_GetSummonBonePos(SUMMON105(env)->unk60, 0x43, (u16 *)&work->unk98);
        }
        if (frame >= 0xFC) {
            if (work->unkA8 != NULL) {
                if (work->unkA8->unk30 == -1) {
                    work->unkA8 = NULL;
                }
                if (work->unkA8 != NULL) {
                    work->unkA8->unk5C.vx = work->unkCC.vx;
                    work->unkA8->unk5C.vy = work->unkCC.vy;
                    work->unkA8->unk5C.vz = work->unkCC.vz;
                }
                ex->Hi_GetSummonBonePos(SUMMON105(env)->unk60, 0x60, (u16 *)&work->unkB4);
            }
            if (work->unkAC != NULL) {
                if (work->unkAC->unk30 == -1) {
                    work->unkAC = NULL;
                }
                if (work->unkAC != NULL) {
                    work->unkAC->unk5C.vx = work->unkCC.vx;
                    work->unkAC->unk5C.vy = work->unkCC.vy;
                    work->unkAC->unk5C.vz = work->unkCC.vz;
                }
                ex->Hi_GetSummonBonePos(SUMMON105(env)->unk60, 0x5C, (u16 *)&work->unkBC);
            }
            if (work->unkB0 != NULL) {
                if (work->unkB0->unk30 == -1) {
                    work->unkB0 = NULL;
                }
                if (work->unkB0 != NULL) {
                    work->unkB0->unk5C.vx = work->unkCC.vx;
                    work->unkB0->unk5C.vy = work->unkCC.vy;
                    work->unkB0->unk5C.vz = work->unkCC.vz;
                }
                ex->Hi_GetSummonBonePos(SUMMON105(env)->unk60, 0x43, (u16 *)&work->unkC4);
            }
        }
        if (work->unkD4 != NULL) {
            if (work->unkD4->unk30 == -1) {
                work->unkD4 = NULL;
            }
            ex->Hi_GetSummonBonePos(SUMMON105(env)->unk60, 0x4C, (u16 *)&work->unkE4);
        }
        if (work->unkD8 != NULL) {
            if (work->unkD8->unk30 == -1) {
                work->unkD8 = NULL;
            }
            ex->Hi_GetSummonBonePos(SUMMON105(env)->unk60, 0x60, (u16 *)&work->unkEC);
        }
        if (work->unkDC != NULL) {
            if (work->unkDC->unk30 == -1) {
                work->unkDC = NULL;
            }
            ex->Hi_GetSummonBonePos(SUMMON105(env)->unk60, 0x3E, (u16 *)&work->unkF4);
        }
        if (work->unkE0 != NULL) {
            if (work->unkE0->unk30 == -1) {
                work->unkE0 = NULL;
            }
            ex->Hi_GetSummonBonePos(SUMMON105(env)->unk60, 0x43, (u16 *)&work->unkFC);
        }
        if (work->unk28 != 0) {
            work->unk28--;
            if (work->unk28 >= 7) {
                func_801EA298((s16 *)ex->btlfx_getFileData(entry->unk8, 0x18), 0x80, 0x20, (8 - work->unk28) * 8);
            } else {
                a = ex->btlfx_lerpRatio(0, 0x80, work->unk28, 6);
                func_801EA298((s16 *)ex->btlfx_getFileData(entry->unk8, 0x18), a, 0x20, 0x10);
            }
        }
        if (work->unk14 != 0) {
            work->unk14--;
            ex->btlfx_aimAngles((s16 *)&env->camera->points[0], (s16 *)&env->camera->points[1], (s16 *)&work->unk190);
            work->unk198[1] = env->camera->points[0].vy - ((ex->math_sin(work->unk190.vx) * 8) >> 12);
            ka = abs((ex->math_cos(work->unk190.vx) * 8) >> 12);
            work->unk198[0] = env->camera->points[0].vx + ((ex->math_sin(work->unk190.vy) * ka) >> 12);
            work->unk198[2] = env->camera->points[0].vz + ((ex->math_cos(work->unk190.vy) * ka) >> 12);
            work->unk1A8[0] = 0x333;
            work->unk1A8[1] = 0x333;
            work->unk1A8[2] = 0x333;
            if (work->unk14 >= 0x12) {
                ex->btlfx_drawScreenTint((u32 *)env->ot, 0xFF, 0, 0, 0);
            }
            if (work->unk14 == 0x11) {
                ex->Hi_DrawEffModel(&work->unk190, (VECTOR *)work->unk198, (s32)work->unk1A8, work->unk180);
            }
            if (work->unk14 == 0x10) {
                ex->Hi_DrawEffModel(&work->unk190, (VECTOR *)work->unk198, (s32)work->unk1A8, work->unk184);
            }
            m14 = work->unk14;
            if (m14 < 0x10) {
                ex->Hi_DrawMorphEffModel(&work->unk190, (VECTOR *)work->unk198, (s32)work->unk1A8, work->unk188, work->unk18C,
                                         m14 << 8);
            }
            if (work->unk14 < 8) {
                k8 = -((8 - work->unk14) << 5);
                ex->Hi_ModifyEffModelRGB(work->unk188, k8, k8, k8);
            }
        }
        if (work->unk0 != 0) {
            D_801EADB8.vx = env->camera->points[2].vx;
            D_801EADB8.vy = env->camera->points[2].vy;
            D_801EADB8.vz = env->camera->points[2].vz;
            ex->Hi_DrawSummonModel(&D_801EADA8, &D_801EADB8, (s32)&D_801EADC8, SUMMON105(env)->unk60, 1);
        }
        if (work->unk4 != 0) {
            work->unk4--;
            ex->Hi_GetSummonBoneMatrix(SUMMON105(env)->unk60, 0x27, &bone1);
            ex->btlfx_matrixPointAhead((MATRIX *)&bone1, 0x80, (s32)&work->unk10C);
            ex->Hi_GetSummonBoneMatrix(SUMMON105(env)->unk60, 0x27, &bone2);
            ex->btlfx_matrixPointAhead((MATRIX *)&bone2, 0x384, (s32)&work->unk114);
            s4 = 0x1000;
            if (work->unk4 < 4) {
                s4 = work->unk4 << 10;
            }
            func_801E7704(&work->unk10C, &work->unk114, 0x10, &work->unk11C, &work->unk120, s4, D_801EAC98, 1);
        }
        if (work->unk10 != 0) {
            work->unk158[0] = env->camera->points[3].vx;
            work->unk158[1] = env->camera->points[3].vy;
            work->unk158[2] = env->camera->points[3].vz;
            for (i = 0; i < 8; i++) {
                if (D_801EAA90[i].unk0 == 0) {
                    if (D_801EAA90[i].unk4 != 0) {
                        D_801EAA90[i].unk4--;
                        ex->Hi_DrawEffModel(&work->unk168, (VECTOR *)work->unk158, (s32)work->unk170, work->unk138[i]);
                    }
                } else {
                    D_801EAA90[i].unk0--;
                }
            }
        }
        if (work->unk8 != 0) {
            work->unk8--;
            if (work->unk124 != NULL) {
                if (work->unk124->unk30 == -1) {
                    work->unk124 = NULL;
                }
                if (work->unk8 != 0) {
                    work->unk130.vx = env->camera->unk30[2].vx;
                    work->unk130.vy = env->camera->unk30[2].vy;
                    work->unk130.vz = env->camera->unk30[2].vz;
                    ex->btlfx_moveEmitterTo((s32)work->unk124, (s32)&work->unk130);
                    if (work->unk8 >= 0x62 && work->unk124 != NULL) {
                        work->unk124->unk20 = (0x71 - work->unk8) << 8;
                    }
                } else {
                    ex->btlfx_stopEmitter(work->unk124);
                }
            }
        }
        if (work->unkC != 0) {
            work->unkC--;
            a = 0x1000;
            tp = 0xFF;
            z = 0xFFF;
            if (work->unkC >= 0x84) {
                a = (0x93 - work->unkC) << 8;
            }
            if (work->unkC < 8) {
                a = work->unkC << 9;
                tp = 1;
                z = 0;
            }
            vC = (a * 0x30) >> 12;
            c1[2].b = c1[3].b = vC;
            c1[0].b = c1[1].b = 0;
            c1[0].g = c1[1].g = 0;
            c1[0].r = c1[1].r = 0;
            c1[2].r = c1[3].r = 0;
            c1[2].g = c1[3].g = 0;
            func_801E7D9C(z, tp, 0, 0xF0, c1);
        }
        if (work->unk30 != 0) {
            work->unk30--;
            for (i = 0; i < 32; i++) {
                a = 0xFFF;
                if (D_801EB998[i].unk10 == 0) {
                    if (!(ex->btlfx_rand() & 0x1F)) {
                        D_801EB998[i].unk10 = (ex->btlfx_rand() & 7) + 1;
                    }
                } else {
                    D_801EB998[i].unk10--;
                    a = (ex->btlfx_rand() & 0x3FF) + 0x800;
                }
                D_801EB998[i].unk10 = ex->btlfx_rand() & 0x1F;
                ex->btlfx_drawSpriteAtOt(0xFF8, (u8 *)ex->btlfx_getFileData(entry->unk8, 0x10), D_801EB998[i].unk0, D_801EB998[i].unk4, 0, 1, a);
                if (work->unk30 < 0x28) {
                    D_801EB998[i].unk0 -= D_801EB998[i].unk8;
                    if (D_801EB998[i].unk0 < 0) {
                        D_801EB998[i].unk0 = 0x1E0;
                        D_801EB998[i].unk4 = ex->btlfx_randRange(0, 0xDA);
                        D_801EB998[i].unk8 = (ex->btlfx_rand() & 7) + 4;
                    }
                }
                if (work->unk30 >= 0x29 && work->unk30 < 0x31) {
                    D_801EB998[i].unk4 -= 10;
                }
            }
        }
        if (work->unk1C != 0) {
            work->unk1C--;
            if (work->unk1C >= 9) {
                v1C = ex->btlfx_lerpRatio(0xFF, 0, work->unk1C - 8, 8);
                c2[1].b = c2[3].b = v1C;
                c2[1].g = c2[3].g = v1C;
                c2[1].r = c2[3].r = v1C;
            } else {
                c2[1].b = c2[3].b = 0xFF;
                c2[1].g = c2[3].g = 0xFF;
                c2[1].r = c2[3].r = 0xFF;
            }
            w1C = ex->btlfx_lerpRatio(0xFF, 0, work->unk1C, 0x10);
            c2[0].b = c2[2].b = w1C;
            c2[0].g = c2[2].g = w1C;
            c2[0].r = c2[2].r = w1C;
            func_801E7D9C(0, 1, 0, 0xF0, c2);
        }
        if (work->unk20 != 0) {
            work->unk20--;
            if (work->unk20 >= 0x21) {
                v20 = ex->btlfx_lerpRatio(0, 0x7F, work->unk20 - 0x20, 0x10);
                c3[0].b = c3[2].b = v20;
                c3[0].g = c3[2].g = v20;
                c3[0].r = c3[2].r = v20;
            } else {
                c3[0].b = c3[2].b = 0;
                c3[0].g = c3[2].g = 0;
                c3[0].r = c3[2].r = 0;
            }
            w20 = ex->btlfx_lerpRatio(0, 0x7F, work->unk20, 0x30);
            c3[1].b = c3[3].b = w20;
            c3[1].g = c3[3].g = w20;
            c3[1].r = c3[3].r = w20;
            func_801E7D9C(0, 1, 0, 0xF0, c3);
        }
        if (work->unk18 != 0) {
            work->unk18--;
            c4[2].b = c4[3].b = 0xFF;
            c4[2].g = c4[3].g = 0xFF;
            c4[2].r = c4[3].r = 0xFF;
            c4[0].b = c4[1].b = 0;
            c4[0].g = c4[1].g = 0;
            c4[0].r = c4[1].r = 0;
            func_801E7D9C(0, 2, 0xA0, 0xF0, c4);
        }
        if (work->unk24 != 0) {
            work->unk24--;
            a = 0x1000;
            if (work->unk24 >= 0x31) {
                a = (0x30 - work->unk24) << 10;
            }
            if (work->unk24 < 4) {
                a = work->unk24 << 10;
            }
            func_801E9F28(a);
        }
        if (work->unk2C != 0) {
            work->unk2C--;
            w2C = ex->btlfx_lerpRatio(0x3F, 0, work->unk2C, 0x1C);
            a = 0x80;
            if (work->unk2C < 0x10) {
                a = work->unk2C * 8;
            }
            func_801EA6D4((s16 *)ex->btlfx_getFileData(entry->unk8, 0x20), a, a, w2C);
            func_801EA6D4((s16 *)ex->btlfx_getFileData(entry->unk8, 0x22), a, a, w2C);
            func_801EA6D4((s16 *)ex->btlfx_getFileData(entry->unk8, 0x23), a, a, w2C);
            func_801EA6D4((s16 *)ex->btlfx_getFileData(entry->unk8, 0x14), a, a, w2C);
            func_801EA6D4((s16 *)ex->btlfx_getFileData(entry->unk8, 0x25), a, a, w2C);
        }
        func_801E7BF8();
    }
    return 0;
}

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
        p->vx += env->camera->points[2].vx;
        p->vz += env->camera->points[2].vz;
        p->vy += env->camera->points[2].vy;
    }
}

void func_801E9F28(s32 c) {
    Unk800F78A4 *env = D_801EAB18;
    BtlfxExports *ex = BTLFX_EXPORTS(env);
    MATRIX m;
    SVECTOR v[4];
    SVECTOR rot;
    u8 uv[4][8];
    s32 otz;
    POLY_FT4 *p;
    s32 i;
    s32 w;
    s32 abr;
    s32 tp;
    s32 clut;

    abr = 1;
    clut = 0x3D00;
    uv[1][0] = uv[3][0] = 0x4F;
    uv[0][1] = uv[1][1] = 0xA0;
    uv[0][0] = uv[2][0] = 0;
    uv[2][1] = uv[3][1] = 0xEF;
    gte_SetRotMatrix(env->viewMatrix);
    gte_SetTransMatrix(env->viewMatrix);
    gte_ldv0(&env->camera->points[2]);
    p = (POLY_FT4 *)env->prim;
    tp = 0x3A;
    env->prim = (u8 *)(p + 32);
    gte_rtv0tr_b();
    rot.vx = 0;
    rot.vy = 0;
    rot.vz = 0;
    ex->func_800316D8(&rot, &m);
    gte_stlvnl(m.t);
    ex->MulRotMatrix(&m);
    gte_SetRotMatrix(&m);
    gte_SetTransMatrix(&m);
    for (i = 0; i < 32; i++) {
        w = i * 16 - 0x5E8;
        v[0].vx = v[2].vx = D_801EC168[i].vx - 0x400;
        v[1].vx = v[3].vx = D_801EC168[i].vx;
        v[0].vz = v[1].vz = D_801EC168[i].vz + w;
        v[2].vz = v[3].vz = D_801EC168[i].vz - w;
        v[0].vy = v[2].vy = D_801EC168[i].vy - 0x600;
        v[1].vy = v[3].vy = D_801EC168[i].vy;
        gte_ldv3c(v);
        gte_rtpt();
        if (abr == 0xFF) {
            p->code = 0x2C;
        } else {
            p->code = 0x2E;
        }
        p->r0 = p->g0 = p->b0 = (c * 0x70) >> 12;
        gte_stsxy3(&p->x0, &p->x1, &p->x2);
        gte_avsz3();
        *(u16 *)&p->u0 = *(u16 *)uv[0];
        *(u16 *)&p->u1 = *(u16 *)uv[1];
        *(u16 *)&p->u2 = *(u16 *)uv[2];
        *(u16 *)&p->u3 = *(u16 *)uv[3];
        gte_stszotz(&otz);
        gte_ldv0(&v[3]);
        gte_rtps();
        p->clut = clut;
        p->tpage = tp;
        gte_stsxy(&p->x3);
        ex->btlfx_addPrim(0x09000000, otz >> 1, p);
        p++;
        D_801EC168[i].vz -= (D_801EC168[i].vx + 0x3000) >> 6;
        if (D_801EC168[i].vz < env->camera->points[2].vz - 0x1000) {
            D_801EC168[i].vz = 0x2000;
        }
    }
}

void func_801EA298(s16 *data, s32 c, s32 h, s32 n) {
    Unk800F78A4 *env = D_801EAB18;
    BtlfxExports *ex = BTLFX_EXPORTS(env);
    SVECTOR v;
    SVECTOR rot;
    SVECTOR cen;
    SVECTOR ang;
    SVECTOR a;
    SVECTOR b;
    MATRIX m;
    POLY_FT4 *prim;
    s32 r;
    s32 tp;
    s32 i;
    s32 (*fsin)(s32);
    s32 av;

    a.vx = a.vz = b.vx = b.vz = 0;
    a.vy = -h;
    b.vy = h;
    ex->btlfx_aimAngles(&env->camera->points[0].vx, &env->camera->points[1].vx, &ang.vx);
    i = 0;
    cen.vy = env->camera->points[0].vy - ((ex->math_sin(ang.vx) * 0x800) >> 12);
    data += 2;
    r = ex->math_cos(ang.vx) * 0x800;
    r >>= 12;
    /* The do-while only raises r's reference count so global-alloc puts it ahead of data; the sin pointer and
     * argument are loaded before the abs() branch in the original. */
    do {
        av = ang.vy;
        fsin = ex->math_sin;
        if (r < 0) {
            r = -r;
        }
    } while (0);
    cen.vx = env->camera->points[0].vx + ((fsin(av) * r) >> 12);
    cen.vz = env->camera->points[0].vz + ((ex->math_cos(ang.vy) * r) >> 12);
    for (; i < 0x11; i++) {
        v.vx = -*data++ + cen.vx;
        v.vy = *data++ + cen.vy;
        v.vz = -*data++ + cen.vz;
        rot.vx = *data++ + ang.vx;
        rot.vy = *data++ + ang.vy;
        rot.vz = *data++ + ang.vz;
        gte_SetRotMatrix(env->viewMatrix);
        gte_SetTransMatrix(env->viewMatrix);
        gte_ldv0(&v);
        gte_rtv0tr();
        gte_stlvnl(m.t);
        ex->func_800316D8(&rot, &m);
        ex->MulRotMatrix(&m);
        gte_SetRotMatrix(&m);
        gte_SetTransMatrix(&m);
        gte_ldv0(&a);
        gte_rtps();
        gte_stsxy(&D_801EC2E8[i].xy0);
        gte_ldv0(&b);
        gte_rtps();
        gte_stsxy(&D_801EC2E8[i].xy1);
    }
    if (n != 0) {
        prim = (POLY_FT4 *)env->prim;
        env->prim = (u8 *)(prim + 16);
        for (i = 0; i < n; prim++) {
            tp = 0xB8;
            prim->clut = 0x3D80;
            prim->tpage = tp;
            prim->b0 = c;
            prim->g0 = c;
            prim->r0 = c;
            prim->code = 0x2E;
            prim->x0 = D_801EC2E8[i].xy0.vx;
            prim->y0 = D_801EC2E8[i].xy0.vy;
            prim->x1 = D_801EC2E8[i].xy1.vx;
            prim->y1 = D_801EC2E8[i].xy1.vy;
            prim->x2 = D_801EC2E8[i + 1].xy0.vx;
            prim->y2 = D_801EC2E8[i + 1].xy0.vy;
            prim->x3 = D_801EC2E8[i + 1].xy1.vx;
            prim->y3 = D_801EC2E8[i + 1].xy1.vy;
            prim->u2 = 0xFF;
            prim->u0 = 0xFF;
            prim->u3 = 0xC0;
            prim->u1 = 0xC0;
            prim->v0 = prim->v1 = ex->btlfx_lerpRatio(0, 0xFF, i, n);
            i++;
            prim->v2 = prim->v3 = ex->btlfx_lerpRatio(0, 0xFF, i, n);
            ex->btlfx_addPrim(0x09000000, 0, prim);
        }
    }
}

void func_801EA6D4(s16 *data, s32 c, s32 c2, s32 n) {
    Unk800F78A4 *env;
    BtlfxExports *ex;
    SVECTOR v;
    SVECTOR r;
    SVECTOR a;
    SVECTOR b;
    MATRIX m;
    POLY_FT4 *prim;
    Fx105RibbonPt *cur;
    s16 *p;
    s32 i;
    s32 tp;

    p = data + 2;
    env = D_801EAB18;
    ex = BTLFX_EXPORTS(env);
    a.vy = -0xC0;
    a.vx = a.vz = b.vx = b.vz = 0;
    b.vy = 0xC0;
    for (i = 0; i < 0x40; i++) {
        v.vx = *p++;
        v.vy = *p++;
        v.vz = *p++ + env->targets->centerZ;
        r.vx = *p++;
        r.vy = *p++;
        r.vz = *p++;
        gte_SetRotMatrix(env->viewMatrix);
        gte_SetTransMatrix(env->viewMatrix);
        gte_ldv0(&v);
        gte_rtv0tr();
        gte_stlvnl(m.t);
        ex->func_800316D8(&r, &m);
        ex->MulRotMatrix(&m);
        gte_SetRotMatrix(&m);
        gte_SetTransMatrix(&m);
        gte_ldv0(&a);
        gte_rtps();
        gte_stsxy(&D_801EC378[i].xy0);
        gte_ldv0(&b);
        gte_rtps();
        gte_stsxy(&D_801EC378[i].xy1);
        gte_stszotz(&D_801EC378[i].z);
    }
    if (n != 0) {
        tp = 0xB8;
        prim = (POLY_FT4 *)env->prim;
        env->prim = (u8 *)(prim + 63);
        for (i = 0; i < n; prim++, i++) {
            cur = &D_801EC378[i];
            prim->clut = 0x3DC0;
            prim->tpage = tp;
            prim->b0 = c;
            prim->g0 = c;
            prim->r0 = c;
            prim->code = 0x2E;
            prim->x0 = cur->xy0.vx;
            prim->y0 = cur->xy0.vy;
            prim->x1 = cur->xy1.vx;
            prim->y1 = cur->xy1.vy;
            prim->x2 = D_801EC378[i + 1].xy0.vx;
            prim->y2 = D_801EC378[i + 1].xy0.vy;
            prim->x3 = D_801EC378[i + 1].xy1.vx;
            prim->y3 = D_801EC378[i + 1].xy1.vy;
            prim->v2 = 0xDF;
            prim->v0 = 0xDF;
            prim->v3 = 0xA0;
            prim->v1 = 0xA0;
            prim->u0 = prim->u1 = ex->btlfx_lerpRatio(0xBF, 0, i, n);
            prim->u2 = prim->u3 = ex->btlfx_lerpRatio(0xBF, 0, i + 1, n);
            if (cur->xy0.vx < D_801EC378[i + 1].xy0.vx) {
                ex->btlfx_addPrim(0x09000000, 0, prim);
            } else {
                ex->btlfx_addPrim(0x09000000, (cur->z + D_801EC378[i + 1].z) >> 1, prim);
            }
        }
    }
}

INCLUDE_RODATA("asm/us/fx_105/nonmatchings/fx_105", fx_105_headerPtr);
