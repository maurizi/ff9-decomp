#include "fxmod/fx_105.h"

static void fxm_drawRibbonTrail(SVECTOR *a, SVECTOR *b, u32 n, CVECTOR *ca, CVECTOR *cb, s32 p, Fx105Seg *buf, s32 abr) {
    Unk800F78A4 *env = fx_105_env;
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
            buf[i].pos0.pad = 0;
        }
        return;
    }
    s = &buf[(s32)(n - 1)];
    for (i = 0; i < n; i++) {
        BTLFX_COPY_SVEC(&s[1].pos0, &s[0].pos0);
        BTLFX_COPY_SVEC(&s[1].pos8, &s[0].pos8);
        s--;
    }
    s = buf;
    for (i = 0; i < n; i++, s++) {
        if (!(s[0].pos0.pad & s[1].pos0.pad)) {
            break;
        }
    }
    BTLFX_COPY_SVEC(&buf->pos0, a);
    buf->pos0.pad = 1;
    BTLFX_COPY_SVEC(&buf->pos8, b);
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
            gte_ldv0(&buf[1].pos8);
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

static void fxm_camKeyReset(void) {
    s32 i;
    s32 m;

    fx_105_camFrame = 0;
    m = -1;
    for (i = 15; i >= 0; i--) {
        fx_105_camKeys[i].timer4 = m;
    }
}

static void fxm_camKeyAdd(s32 start, u32 dst, u32 src, s16 *data) {
    s32 i;

    for (i = 0; i < 16; i++) {
        if (fx_105_camKeys[i].timer4 == -1) {
            fx_105_camKeys[i].timer4 = 0;
            fx_105_camKeys[i].keys10 = data + 1;
            fx_105_camKeys[i].frame0 = start;
            fx_105_camKeys[i].dstSlots8 = dst;
            fx_105_camKeys[i].srcSlotsC = src;
            return;
        }
    }
}

static void fxm_camKeyUpdate(void) {
    SVECTOR *ext = fx_105_camKeyPoints;
    s32 i;
    Unk800F78A4 *env = fx_105_env;
    s32 frame = fx_105_camFrame;
    s32 k;
    s32 d;
    s32 s;
    s32 x, y, z;
    s32 j;

    for (i = 0; i < 16; i++) {
        if (fx_105_camKeys[i].timer4 == 0 && fx_105_camKeys[i].frame0 == frame) {
            fx_105_camKeys[i].timer4 = *fx_105_camKeys[i].keys10;
            fx_105_camKeys[i].keys10++;
        }
        if (fx_105_camKeys[i].timer4 > 0) {
            fx_105_camKeys[i].timer4--;
            for (k = 0; k < 3; k++) {
                d = ((fx_105_camKeys[i].dstSlots8 >> (k * 4)) & 0xF) - 1;
                s = ((fx_105_camKeys[i].srcSlotsC >> (k * 4)) & 0xF) - 1;
                if (d >= 0) {
                    x = *fx_105_camKeys[i].keys10;
                    fx_105_camKeys[i].keys10++;
                    y = *fx_105_camKeys[i].keys10;
                    fx_105_camKeys[i].keys10++;
                    z = *fx_105_camKeys[i].keys10;
                    fx_105_camKeys[i].keys10++;
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
    fx_105_camFrame++;
}

void fx_105_drawScreenBand(s32 z, s32 tpage, s32 y0, s32 y1, CVECTOR *rgb) {
    POLY_G4 *p = (POLY_G4 *)fx_105_env->prim;
    BtlfxExports *ex = BTLFX_EXPORTS(fx_105_env);

    fx_105_env->prim = (u8 *)(p + 1);
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
        fx_105_env = env;
        ex->btlfx_initEffModels(fx_105_effModels, 0xF);
        for (i = 0xF; i >= 0; i--) {
            (&work->visible0)[i] = 0;
        }
        for (i = 0; i < 0x20; i++) {
            fx_105_stars[i].posX0 = ex->btlfx_randRange(0xC0, 0x1E0);
            fx_105_stars[i].posY4 = ex->btlfx_randRange(0, 0xDA);
            fx_105_stars[i].speed8 = (ex->btlfx_rand() & 7) + 4;
            fx_105_stars[i].flicker10 = 1;
        }
        fx_105_stars[0].posX0 = 0x10;
        fx_105_stars[0].posY4 = 0x10;
        fx_105_stars[1].posX0 = 0x20;
        fx_105_stars[1].posY4 = 0x18;
        fx_105_stars[2].posX0 = 8;
        fx_105_stars[2].posY4 = 0x28;
        fx_105_stars[3].posX0 = 0x14;
        fx_105_stars[3].posY4 = 0xB4;
        fx_105_stars[4].posX0 = 0x2D;
        fx_105_stars[4].posY4 = 0xD2;
        fx_105_stars[5].posX0 = 0x3C;
        fx_105_stars[5].posY4 = 0xC3;
        fx_105_stars[6].posX0 = 0x64;
        fx_105_stars[6].posY4 = 0xBE;
        fx_105_modelRot.vx = 0;
        fx_105_modelRot.vy = 0x800;
        fx_105_modelRot.vz = 0;
        fx_105_modelScale.vx = 0x1000;
        fx_105_modelScale.vy = 0x1000;
        fx_105_modelScale.vz = 0x1000;
        fx_105_modelPos.vx = 0;
        fx_105_modelPos.vy = 0;
        fx_105_modelPos.vz = 0;
        work->emit124 = NULL;
        work->emit40 = NULL;
        work->emit44 = NULL;
        work->emit48 = NULL;
        work->emit4C = NULL;
        work->emit50 = NULL;
        work->emit54 = NULL;
        work->emitA8 = NULL;
        work->emitAC = NULL;
        work->emitB0 = NULL;
        work->emitD4 = NULL;
        work->emitD8 = NULL;
        work->emitDC = NULL;
        work->emitE0 = NULL;
        work->pos104.vx = 0;
        work->pos104.vy = 0;
        work->pos104.vz = env->targets->centerZ;
        work->pos58.vx = 0;
        work->pos58.vy = -0x62C;
        work->pos58.vz = -0x186A;
        work->pos60.vx = 0;
        work->pos60.vy = -0x62C;
        work->pos60.vz = -0x186A;
        work->pos68.vx = 0;
        work->pos68.vy = -0x62C;
        work->pos68.vz = -0x186A;
        work->pos128.vx = -0x304;
        work->pos128.vy = -0x26FC;
        work->pos128.vz = -0x2A94;
        work->pos130.vx = -0x304;
        work->pos130.vy = -0x26FC;
        work->pos130.vz = -0x2A94;
        env->camera->pointRots[2].vx = -0x304;
        env->camera->pointRots[2].vy = -0x26FC;
        env->camera->pointRots[2].vz = -0x2A94;
        work->model138[0] = ex->btlfx_registerEffectModel((Unk800E8F50 *)ex->btlfx_getFileData(entry->file, 0xD), 0xFF);
        work->model138[1] = ex->btlfx_registerEffectModel((Unk800E8F50 *)ex->btlfx_getFileData(entry->file, 0x15), 0xFF);
        work->model138[2] = ex->btlfx_registerEffectModel((Unk800E8F50 *)ex->btlfx_getFileData(entry->file, 0x29), 0xFF);
        work->model138[3] = ex->btlfx_registerEffectModel((Unk800E8F50 *)ex->btlfx_getFileData(entry->file, 0x2A), 0xFF);
        work->model138[4] = ex->btlfx_registerEffectModel((Unk800E8F50 *)ex->btlfx_getFileData(entry->file, 0x2B), 0xFF);
        work->model138[5] = ex->btlfx_registerEffectModel((Unk800E8F50 *)ex->btlfx_getFileData(entry->file, 0x2C), 0xFF);
        work->model138[6] = ex->btlfx_registerEffectModel((Unk800E8F50 *)ex->btlfx_getFileData(entry->file, 0x2D), 0xFF);
        work->model138[7] = ex->btlfx_registerEffectModel((Unk800E8F50 *)ex->btlfx_getFileData(entry->file, 0x2E), 0xFF);
        work->scale170[0] = 0x2000;
        work->scale170[1] = 0x2000;
        work->pos158[0] = 0;
        work->pos158[1] = 0;
        work->pos158[2] = 0;
        work->rot168.vx = 0;
        work->rot168.vy = 0;
        work->rot168.vz = 0;
        work->scale170[2] = 0x1A80;
        work->model180 = ex->btlfx_registerEffectModel((Unk800E8F50 *)ex->btlfx_getFileData(entry->file, 0x80), 2);
        work->model184 = ex->btlfx_registerEffectModel((Unk800E8F50 *)ex->btlfx_getFileData(entry->file, 0x81), 2);
        work->model188 = ex->btlfx_registerEffectModel((Unk800E8F50 *)ex->btlfx_getFileData(entry->file, 0x82), 2);
        work->model18C = ex->btlfx_registerEffectModel((Unk800E8F50 *)ex->btlfx_getFileData(entry->file, 0x83), 2);
        work->scale1A8[0] = 0x200;
        work->scale1A8[1] = 0x200;
        work->scale1A8[2] = 0x200;
        work->posB4.vz = -0x800;
        work->posBC.vz = -0x800;
        work->posC4.vz = -0x800;
        work->color11C.b = 0x40;
        work->color11C.g = 0x40;
        work->color120.g = 0x1E;
        work->pos198[0] = 0;
        work->pos198[1] = 0;
        work->pos198[2] = 0;
        work->rot190.vx = 0;
        work->rot190.vy = 0;
        work->rot190.vz = 0;
        work->posB4.vx = 0;
        work->posB4.vy = 0;
        work->posBC.vx = 0;
        work->posBC.vy = 0;
        work->posC4.vx = 0;
        work->posC4.vy = 0;
        work->rotCC.vx = 0;
        work->rotCC.vy = 0x800;
        work->rotCC.vz = 0;
        work->color11C.r = 0;
        work->color120.r = 0;
        work->color120.b = 0x10;
        fxm_drawRibbonTrail(&work->pos10C, &work->pos114, 0x408, &work->color11C, &work->color120, 0x1000, fx_105_ribbonSegs, 1);
        fx_105_initWindStreaks();
        fxm_camKeyReset();
        fxm_camKeyAdd(0, 3, 0, (s16 *)ex->btlfx_getFileData(entry->file, 5));
        fxm_camKeyAdd(0, 1, 0, (s16 *)ex->btlfx_getFileData(entry->file, 2));
        fxm_camKeyAdd(0, 2, 0, (s16 *)ex->btlfx_getFileData(entry->file, 8));
        fxm_camKeyAdd(0, 9, 0, (s16 *)ex->btlfx_getFileData(entry->file, 7));
        fxm_camKeyAdd(0, 4, 0, (s16 *)ex->btlfx_getFileData(entry->file, 6));
        fxm_camKeyAdd(0xA7, 3, 0, (s16 *)ex->btlfx_getFileData(entry->file, 0xA));
        fxm_camKeyAdd(0xA7, 1, 0, (s16 *)ex->btlfx_getFileData(entry->file, 9));
        fxm_camKeyAdd(0xA7, 2, 0, (s16 *)ex->btlfx_getFileData(entry->file, 0x16));
        fxm_camKeyAdd(0xFB, 3, 0, (s16 *)ex->btlfx_getFileData(entry->file, 1));
        fxm_camKeyAdd(0xFB, 1, 0, (s16 *)ex->btlfx_getFileData(entry->file, 0x17));
        fxm_camKeyAdd(0xFB, 2, 0, (s16 *)ex->btlfx_getFileData(entry->file, 0x19));
        fxm_camKeyAdd(0x137, 3, 0, (s16 *)ex->btlfx_getFileData(entry->file, 0x1B));
        fxm_camKeyAdd(0x137, 1, 0, (s16 *)ex->btlfx_getFileData(entry->file, 0x1A));
        fxm_camKeyAdd(0x137, 2, 0, (s16 *)ex->btlfx_getFileData(entry->file, 0x1C));
        return 0;
    } else {
        frame = entry->frame;
        switch (frame) {
        case 0:
            work->emit124 = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, 0x11), (s32)&work->pos128);
            if (work->emit124 != NULL) {
                work->emit124->otZ = -0x400;
            }
            work->timer8 = 0x71;
            work->timerC = 0x93;
            work->timer30 = 0x78;
            break;
        case 0xF:
            ex->Hi_SetSummonMotion(SUMMON105(env)->summon54->motion180[0], SUMMON105(env)->summonModel60);
            work->visible0 = 1;
            break;
        case 0x1E:
            work->timer4 = 0x10;
            break;
        case 0x21:
            ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, 3), (s32)&work->pos58);
            ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, 0xB), (s32)&work->pos60);
            ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, 4), (s32)&work->pos68);
            work->emit40 = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, 0xE), (s32)&work->pos70);
            work->emit44 = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, 0xE), (s32)&work->pos78);
            break;
        case 0x41:
            work->flag10 = 1;
            break;
        case 0x51:
            work->emit48 = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, 0x1F), (s32)&work->pos80);
            work->emit4C = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, 0x1F), (s32)&work->pos90);
            work->emit50 = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, 0x1F), (s32)&work->pos88);
            work->emit54 = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, 0x1F), (s32)&work->pos98);
            work->timer24 = 0x30;
            break;
        case 0x55:
            ex->Hi_SetSummonMotion(SUMMON105(env)->summon54->motion180[1], SUMMON105(env)->summonModel60);
            break;
        case 0x7F:
            ex->Hi_SetSummonMotion(SUMMON105(env)->summon54->motion180[2], SUMMON105(env)->summonModel60);
            break;
        case 0x93:
            ex->btlfx_stopEmitter(work->emit48);
            ex->btlfx_stopEmitter(work->emit4C);
            ex->btlfx_stopEmitter(work->emit50);
            ex->btlfx_stopEmitter(work->emit54);
            work->flag10 = 0;
            work->timer24 = 0;
            work->timer30 = 0;
            work->visible0 = 0;
            ex->Hi_SetSummonMotion(SUMMON105(env)->summon54->motion180[3], SUMMON105(env)->summonModel60);
            break;
        case 0x9C ... 0xA6:
            ex->Hi_SetSummonMotFrame(SUMMON105(env)->summonModel60, 0);
            work->visible0 = 1;
            break;
        case 0xA7:
            ex->Hi_SetSummonMotFrame(SUMMON105(env)->summonModel60, 0);
            break;
        case 0xAB:
            work->posA0.vx = env->camera->points[2].vx;
            work->posA0.vy = env->camera->points[2].vy;
            work->posA0.vz = env->camera->points[2].vz;
            ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, 0x1D), (s32)&work->posA0);
            break;
        case 0xD8:
            work->timer14 = 0x21;
            break;
        case 0xE0:
            work->timer28 = 8;
            break;
        case 0xE7:
            ex->Hi_SetSummonMotion(SUMMON105(env)->summon54->motion180[4], SUMMON105(env)->summonModel60);
            break;
        case 0xEC:
            work->timer1C = 0x10;
            break;
        case 0xFC:
            ex->Hi_SetSummonMotion(SUMMON105(env)->summon54->motion180[5], SUMMON105(env)->summonModel60);
            for (i = 0; i < env->targets->targetCount; i++) {
                work->hold1B8[i] = ex->btlfx_holdMotion(i);
            }
            work->timer20 = 0x30;
            work->timer18 = 0x3C;
            work->emitA8 = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, 0x1E), (s32)&work->posB4);
            work->emitAC = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, 0x1E), (s32)&work->posBC);
            work->emitB0 = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, 0x1E), (s32)&work->posC4);
            if (work->emitA8 != NULL) {
                work->emitA8->otZ = 0x100;
            }
            if (work->emitAC != NULL) {
                work->emitAC->otZ = 0x100;
            }
            if (work->emitB0 != NULL) {
                work->emitB0->otZ = 0x100;
            }
            break;
        case 0x138:
            ex->Hi_SetSummonMotion(SUMMON105(env)->summon54->motion180[6], SUMMON105(env)->summonModel60);
            ex->btlfx_stopEmitter(work->emitA8);
            ex->btlfx_stopEmitter(work->emitAC);
            ex->btlfx_stopEmitter(work->emitB0);
            work->emitD4 = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, 0x28), (s32)&work->posE4);
            work->emitD8 = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, 0x28), (s32)&work->posEC);
            work->emitDC = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, 0x28), (s32)&work->posF4);
            work->emitE0 = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, 0x28), (s32)&work->posFC);
            if (work->emitD4 != NULL) {
                work->emitD4->otZ = 0x100;
            }
            if (work->emitD8 != NULL) {
                work->emitD8->otZ = 0x100;
            }
            break;
        case 0x146:
            for (i = 0; i < env->targets->targetCount; i++) {
                ex->btlfx_setMotionHeld(i, work->hold1B8[i]);
            }
            break;
        case 0x165:
            ex->btlfx_stopEmitter(work->emitD4);
            ex->btlfx_stopEmitter(work->emitD8);
            ex->btlfx_stopEmitter(work->emitDC);
            ex->btlfx_stopEmitter(work->emitE0);
            work->visible0 = 0;
            ex->btlfx_clearEffModels();
            work->timer2C = 0x1C;
            ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, 0x3B), (s32)&work->pos104);
            break;
        case 0x185:
            env->camera->points[0].vx = 0;
            env->camera->points[0].vy = -0x200;
            env->camera->points[0].vz = 0x200;
            break;
        case 0x1B0:
            return 1;
        }
        if (work->emit40 != NULL) {
            if (work->emit40->state == BTLFX_EMIT_DONE) {
                work->emit40 = NULL;
            }
            ex->Hi_GetSummonBonePos(SUMMON105(env)->summonModel60, 0x4C, (u16 *)&work->pos70);
        }
        if (work->emit44 != NULL) {
            if (work->emit44->state == BTLFX_EMIT_DONE) {
                work->emit44 = NULL;
            }
            ex->Hi_GetSummonBonePos(SUMMON105(env)->summonModel60, 0x60, (u16 *)&work->pos78);
        }
        if (work->emit48 != NULL) {
            if (work->emit48->state == BTLFX_EMIT_DONE) {
                work->emit48 = NULL;
            }
            ex->Hi_GetSummonBonePos(SUMMON105(env)->summonModel60, 0x4C, (u16 *)&work->pos80);
        }
        if (work->emit4C != NULL) {
            if (work->emit4C->state == BTLFX_EMIT_DONE) {
                work->emit4C = NULL;
            }
            ex->Hi_GetSummonBonePos(SUMMON105(env)->summonModel60, 0x60, (u16 *)&work->pos90);
        }
        if (work->emit50 != NULL) {
            if (work->emit50->state == BTLFX_EMIT_DONE) {
                work->emit50 = NULL;
            }
            ex->Hi_GetSummonBonePos(SUMMON105(env)->summonModel60, 0x3E, (u16 *)&work->pos88);
        }
        if (work->emit54 != NULL) {
            if (work->emit54->state == BTLFX_EMIT_DONE) {
                work->emit54 = NULL;
            }
            ex->Hi_GetSummonBonePos(SUMMON105(env)->summonModel60, 0x43, (u16 *)&work->pos98);
        }
        if (frame >= 0xFC) {
            if (work->emitA8 != NULL) {
                if (work->emitA8->state == BTLFX_EMIT_DONE) {
                    work->emitA8 = NULL;
                }
                if (work->emitA8 != NULL) {
                    work->emitA8->rot.vx = work->rotCC.vx;
                    work->emitA8->rot.vy = work->rotCC.vy;
                    work->emitA8->rot.vz = work->rotCC.vz;
                }
                ex->Hi_GetSummonBonePos(SUMMON105(env)->summonModel60, 0x60, (u16 *)&work->posB4);
            }
            if (work->emitAC != NULL) {
                if (work->emitAC->state == BTLFX_EMIT_DONE) {
                    work->emitAC = NULL;
                }
                if (work->emitAC != NULL) {
                    work->emitAC->rot.vx = work->rotCC.vx;
                    work->emitAC->rot.vy = work->rotCC.vy;
                    work->emitAC->rot.vz = work->rotCC.vz;
                }
                ex->Hi_GetSummonBonePos(SUMMON105(env)->summonModel60, 0x5C, (u16 *)&work->posBC);
            }
            if (work->emitB0 != NULL) {
                if (work->emitB0->state == BTLFX_EMIT_DONE) {
                    work->emitB0 = NULL;
                }
                if (work->emitB0 != NULL) {
                    work->emitB0->rot.vx = work->rotCC.vx;
                    work->emitB0->rot.vy = work->rotCC.vy;
                    work->emitB0->rot.vz = work->rotCC.vz;
                }
                ex->Hi_GetSummonBonePos(SUMMON105(env)->summonModel60, 0x43, (u16 *)&work->posC4);
            }
        }
        if (work->emitD4 != NULL) {
            if (work->emitD4->state == BTLFX_EMIT_DONE) {
                work->emitD4 = NULL;
            }
            ex->Hi_GetSummonBonePos(SUMMON105(env)->summonModel60, 0x4C, (u16 *)&work->posE4);
        }
        if (work->emitD8 != NULL) {
            if (work->emitD8->state == BTLFX_EMIT_DONE) {
                work->emitD8 = NULL;
            }
            ex->Hi_GetSummonBonePos(SUMMON105(env)->summonModel60, 0x60, (u16 *)&work->posEC);
        }
        if (work->emitDC != NULL) {
            if (work->emitDC->state == BTLFX_EMIT_DONE) {
                work->emitDC = NULL;
            }
            ex->Hi_GetSummonBonePos(SUMMON105(env)->summonModel60, 0x3E, (u16 *)&work->posF4);
        }
        if (work->emitE0 != NULL) {
            if (work->emitE0->state == BTLFX_EMIT_DONE) {
                work->emitE0 = NULL;
            }
            ex->Hi_GetSummonBonePos(SUMMON105(env)->summonModel60, 0x43, (u16 *)&work->posFC);
        }
        if (work->timer28 != 0) {
            work->timer28--;
            if (work->timer28 >= 7) {
                fx_105_drawCamRibbon((s16 *)ex->btlfx_getFileData(entry->file, 0x18), 0x80, 0x20, (8 - work->timer28) * 8);
            } else {
                a = ex->btlfx_lerpRatio(0, 0x80, work->timer28, 6);
                fx_105_drawCamRibbon((s16 *)ex->btlfx_getFileData(entry->file, 0x18), a, 0x20, 0x10);
            }
        }
        if (work->timer14 != 0) {
            work->timer14--;
            ex->btlfx_aimAngles((s16 *)&env->camera->points[0], (s16 *)&env->camera->points[1], (s16 *)&work->rot190);
            work->pos198[1] = env->camera->points[0].vy - ((ex->math_sin(work->rot190.vx) * 8) >> 12);
            ka = abs((ex->math_cos(work->rot190.vx) * 8) >> 12);
            work->pos198[0] = env->camera->points[0].vx + ((ex->math_sin(work->rot190.vy) * ka) >> 12);
            work->pos198[2] = env->camera->points[0].vz + ((ex->math_cos(work->rot190.vy) * ka) >> 12);
            work->scale1A8[0] = 0x333;
            work->scale1A8[1] = 0x333;
            work->scale1A8[2] = 0x333;
            if (work->timer14 >= 0x12) {
                ex->btlfx_drawScreenTint((u32 *)env->ot, 0xFF, 0, 0, 0);
            }
            if (work->timer14 == 0x11) {
                ex->Hi_DrawEffModel(&work->rot190, (VECTOR *)work->pos198, (s32)work->scale1A8, work->model180);
            }
            if (work->timer14 == 0x10) {
                ex->Hi_DrawEffModel(&work->rot190, (VECTOR *)work->pos198, (s32)work->scale1A8, work->model184);
            }
            m14 = work->timer14;
            if (m14 < 0x10) {
                ex->Hi_DrawMorphEffModel(&work->rot190, (VECTOR *)work->pos198, (s32)work->scale1A8, work->model188, work->model18C,
                                         m14 << 8);
            }
            if (work->timer14 < 8) {
                k8 = -((8 - work->timer14) << 5);
                ex->Hi_ModifyEffModelRGB(work->model188, k8, k8, k8);
            }
        }
        if (work->visible0 != 0) {
            fx_105_modelPos.vx = env->camera->points[2].vx;
            fx_105_modelPos.vy = env->camera->points[2].vy;
            fx_105_modelPos.vz = env->camera->points[2].vz;
            ex->Hi_DrawSummonModel(&fx_105_modelRot, &fx_105_modelPos, (s32)&fx_105_modelScale, SUMMON105(env)->summonModel60, 1);
        }
        if (work->timer4 != 0) {
            work->timer4--;
            ex->Hi_GetSummonBoneMatrix(SUMMON105(env)->summonModel60, 0x27, &bone1);
            ex->btlfx_matrixPointAhead((MATRIX *)&bone1, 0x80, (s32)&work->pos10C);
            ex->Hi_GetSummonBoneMatrix(SUMMON105(env)->summonModel60, 0x27, &bone2);
            ex->btlfx_matrixPointAhead((MATRIX *)&bone2, 0x384, (s32)&work->pos114);
            s4 = 0x1000;
            if (work->timer4 < 4) {
                s4 = work->timer4 << 10;
            }
            fxm_drawRibbonTrail(&work->pos10C, &work->pos114, 0x10, &work->color11C, &work->color120, s4, fx_105_ribbonSegs, 1);
        }
        if (work->flag10 != 0) {
            work->pos158[0] = env->camera->points[3].vx;
            work->pos158[1] = env->camera->points[3].vy;
            work->pos158[2] = env->camera->points[3].vz;
            for (i = 0; i < 8; i++) {
                if (fx_105_modelTimers[i].timer0 == 0) {
                    if (fx_105_modelTimers[i].timer4 != 0) {
                        fx_105_modelTimers[i].timer4--;
                        ex->Hi_DrawEffModel(&work->rot168, (VECTOR *)work->pos158, (s32)work->scale170, work->model138[i]);
                    }
                } else {
                    fx_105_modelTimers[i].timer0--;
                }
            }
        }
        if (work->timer8 != 0) {
            work->timer8--;
            if (work->emit124 != NULL) {
                if (work->emit124->state == BTLFX_EMIT_DONE) {
                    work->emit124 = NULL;
                }
                if (work->timer8 != 0) {
                    work->pos130.vx = env->camera->pointRots[2].vx;
                    work->pos130.vy = env->camera->pointRots[2].vy;
                    work->pos130.vz = env->camera->pointRots[2].vz;
                    ex->btlfx_moveEmitterTo((s32)work->emit124, (s32)&work->pos130);
                    if (work->timer8 >= 0x62 && work->emit124 != NULL) {
                        work->emit124->alpha = (0x71 - work->timer8) << 8;
                    }
                } else {
                    ex->btlfx_stopEmitter(work->emit124);
                }
            }
        }
        if (work->timerC != 0) {
            work->timerC--;
            a = 0x1000;
            tp = 0xFF;
            z = 0xFFF;
            if (work->timerC >= 0x84) {
                a = (0x93 - work->timerC) << 8;
            }
            if (work->timerC < 8) {
                a = work->timerC << 9;
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
            fx_105_drawScreenBand(z, tp, 0, 0xF0, c1);
        }
        if (work->timer30 != 0) {
            work->timer30--;
            for (i = 0; i < 32; i++) {
                a = 0xFFF;
                if (fx_105_stars[i].flicker10 == 0) {
                    if (!(ex->btlfx_rand() & 0x1F)) {
                        fx_105_stars[i].flicker10 = (ex->btlfx_rand() & 7) + 1;
                    }
                } else {
                    fx_105_stars[i].flicker10--;
                    a = (ex->btlfx_rand() & 0x3FF) + 0x800;
                }
                fx_105_stars[i].flicker10 = ex->btlfx_rand() & 0x1F;
                ex->btlfx_drawSpriteAtOt(0xFF8, (u8 *)ex->btlfx_getFileData(entry->file, 0x10), fx_105_stars[i].posX0, fx_105_stars[i].posY4, 0, 1, a);
                if (work->timer30 < 0x28) {
                    fx_105_stars[i].posX0 -= fx_105_stars[i].speed8;
                    if (fx_105_stars[i].posX0 < 0) {
                        fx_105_stars[i].posX0 = 0x1E0;
                        fx_105_stars[i].posY4 = ex->btlfx_randRange(0, 0xDA);
                        fx_105_stars[i].speed8 = (ex->btlfx_rand() & 7) + 4;
                    }
                }
                if (work->timer30 >= 0x29 && work->timer30 < 0x31) {
                    fx_105_stars[i].posY4 -= 10;
                }
            }
        }
        if (work->timer1C != 0) {
            work->timer1C--;
            if (work->timer1C >= 9) {
                v1C = ex->btlfx_lerpRatio(0xFF, 0, work->timer1C - 8, 8);
                c2[1].b = c2[3].b = v1C;
                c2[1].g = c2[3].g = v1C;
                c2[1].r = c2[3].r = v1C;
            } else {
                c2[1].b = c2[3].b = 0xFF;
                c2[1].g = c2[3].g = 0xFF;
                c2[1].r = c2[3].r = 0xFF;
            }
            w1C = ex->btlfx_lerpRatio(0xFF, 0, work->timer1C, 0x10);
            c2[0].b = c2[2].b = w1C;
            c2[0].g = c2[2].g = w1C;
            c2[0].r = c2[2].r = w1C;
            fx_105_drawScreenBand(0, 1, 0, 0xF0, c2);
        }
        if (work->timer20 != 0) {
            work->timer20--;
            if (work->timer20 >= 0x21) {
                v20 = ex->btlfx_lerpRatio(0, 0x7F, work->timer20 - 0x20, 0x10);
                c3[0].b = c3[2].b = v20;
                c3[0].g = c3[2].g = v20;
                c3[0].r = c3[2].r = v20;
            } else {
                c3[0].b = c3[2].b = 0;
                c3[0].g = c3[2].g = 0;
                c3[0].r = c3[2].r = 0;
            }
            w20 = ex->btlfx_lerpRatio(0, 0x7F, work->timer20, 0x30);
            c3[1].b = c3[3].b = w20;
            c3[1].g = c3[3].g = w20;
            c3[1].r = c3[3].r = w20;
            fx_105_drawScreenBand(0, 1, 0, 0xF0, c3);
        }
        if (work->timer18 != 0) {
            work->timer18--;
            c4[2].b = c4[3].b = 0xFF;
            c4[2].g = c4[3].g = 0xFF;
            c4[2].r = c4[3].r = 0xFF;
            c4[0].b = c4[1].b = 0;
            c4[0].g = c4[1].g = 0;
            c4[0].r = c4[1].r = 0;
            fx_105_drawScreenBand(0, 2, 0xA0, 0xF0, c4);
        }
        if (work->timer24 != 0) {
            work->timer24--;
            a = 0x1000;
            if (work->timer24 >= 0x31) {
                a = (0x30 - work->timer24) << 10;
            }
            if (work->timer24 < 4) {
                a = work->timer24 << 10;
            }
            fx_105_drawWindStreaks(a);
        }
        if (work->timer2C != 0) {
            work->timer2C--;
            w2C = ex->btlfx_lerpRatio(0x3F, 0, work->timer2C, 0x1C);
            a = 0x80;
            if (work->timer2C < 0x10) {
                a = work->timer2C * 8;
            }
            fx_105_drawWorldRibbon((s16 *)ex->btlfx_getFileData(entry->file, 0x20), a, a, w2C);
            fx_105_drawWorldRibbon((s16 *)ex->btlfx_getFileData(entry->file, 0x22), a, a, w2C);
            fx_105_drawWorldRibbon((s16 *)ex->btlfx_getFileData(entry->file, 0x23), a, a, w2C);
            fx_105_drawWorldRibbon((s16 *)ex->btlfx_getFileData(entry->file, 0x14), a, a, w2C);
            fx_105_drawWorldRibbon((s16 *)ex->btlfx_getFileData(entry->file, 0x25), a, a, w2C);
        }
        fxm_camKeyUpdate();
    }
    return 0;
}

void fx_105_initWindStreaks(void) {
    Unk800F78A4 *env = fx_105_env;
    BtlfxExports *ex = BTLFX_EXPORTS(env);
    Fx105Spark *p;
    s32 t;
    s32 i;

    for (i = 0; i < 0x20; i++) {
        t = ex->btlfx_randRange(-((i + 1) * 0x180), -(i * 0x180));
        p = &fx_105_windStreaks[i];
        p->vx = t;
        p->vz = ex->btlfx_randRange(-0x1000, 0x2000);
        p->vy = p->vx >> 4;
        p->vx += env->camera->points[2].vx;
        p->vz += env->camera->points[2].vz;
        p->vy += env->camera->points[2].vy;
    }
}

void fx_105_drawWindStreaks(s32 c) {
    Unk800F78A4 *env = fx_105_env;
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
    ex->math_rotMatrixYXZ(&rot, &m);
    gte_stlvnl(m.t);
    ex->MulRotMatrix(&m);
    gte_SetRotMatrix(&m);
    gte_SetTransMatrix(&m);
    for (i = 0; i < 32; i++) {
        w = i * 16 - 0x5E8;
        v[0].vx = v[2].vx = fx_105_windStreaks[i].vx - 0x400;
        v[1].vx = v[3].vx = fx_105_windStreaks[i].vx;
        v[0].vz = v[1].vz = fx_105_windStreaks[i].vz + w;
        v[2].vz = v[3].vz = fx_105_windStreaks[i].vz - w;
        v[0].vy = v[2].vy = fx_105_windStreaks[i].vy - 0x600;
        v[1].vy = v[3].vy = fx_105_windStreaks[i].vy;
        gte_ldv3c(v);
        gte_rtpt();
        if (abr == 0xFF) {
            p->code = GPU_POLY_FT4;
        } else {
            p->code = GPU_POLY_FT4 | GPU_ABE;
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
        fx_105_windStreaks[i].vz -= (fx_105_windStreaks[i].vx + 0x3000) >> 6;
        if (fx_105_windStreaks[i].vz < env->camera->points[2].vz - 0x1000) {
            fx_105_windStreaks[i].vz = 0x2000;
        }
    }
}

void fx_105_drawCamRibbon(s16 *data, s32 c, s32 h, s32 n) {
    Unk800F78A4 *env = fx_105_env;
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
        ex->math_rotMatrixYXZ(&rot, &m);
        ex->MulRotMatrix(&m);
        gte_SetRotMatrix(&m);
        gte_SetTransMatrix(&m);
        gte_ldv0(&a);
        gte_rtps();
        gte_stsxy(&fx_105_camRibbonPts[i].xy0);
        gte_ldv0(&b);
        gte_rtps();
        gte_stsxy(&fx_105_camRibbonPts[i].xy1);
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
            prim->code = GPU_POLY_FT4 | GPU_ABE;
            prim->x0 = fx_105_camRibbonPts[i].xy0.vx;
            prim->y0 = fx_105_camRibbonPts[i].xy0.vy;
            prim->x1 = fx_105_camRibbonPts[i].xy1.vx;
            prim->y1 = fx_105_camRibbonPts[i].xy1.vy;
            prim->x2 = fx_105_camRibbonPts[i + 1].xy0.vx;
            prim->y2 = fx_105_camRibbonPts[i + 1].xy0.vy;
            prim->x3 = fx_105_camRibbonPts[i + 1].xy1.vx;
            prim->y3 = fx_105_camRibbonPts[i + 1].xy1.vy;
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

void fx_105_drawWorldRibbon(s16 *data, s32 c, s32 c2, s32 n) {
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
    env = fx_105_env;
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
        ex->math_rotMatrixYXZ(&r, &m);
        ex->MulRotMatrix(&m);
        gte_SetRotMatrix(&m);
        gte_SetTransMatrix(&m);
        gte_ldv0(&a);
        gte_rtps();
        gte_stsxy(&fx_105_worldRibbonPts[i].xy0);
        gte_ldv0(&b);
        gte_rtps();
        gte_stsxy(&fx_105_worldRibbonPts[i].xy1);
        gte_stszotz(&fx_105_worldRibbonPts[i].z);
    }
    if (n != 0) {
        tp = 0xB8;
        prim = (POLY_FT4 *)env->prim;
        env->prim = (u8 *)(prim + 63);
        for (i = 0; i < n; prim++, i++) {
            cur = &fx_105_worldRibbonPts[i];
            prim->clut = 0x3DC0;
            prim->tpage = tp;
            prim->b0 = c;
            prim->g0 = c;
            prim->r0 = c;
            prim->code = GPU_POLY_FT4 | GPU_ABE;
            prim->x0 = cur->xy0.vx;
            prim->y0 = cur->xy0.vy;
            prim->x1 = cur->xy1.vx;
            prim->y1 = cur->xy1.vy;
            prim->x2 = fx_105_worldRibbonPts[i + 1].xy0.vx;
            prim->y2 = fx_105_worldRibbonPts[i + 1].xy0.vy;
            prim->x3 = fx_105_worldRibbonPts[i + 1].xy1.vx;
            prim->y3 = fx_105_worldRibbonPts[i + 1].xy1.vy;
            prim->v2 = 0xDF;
            prim->v0 = 0xDF;
            prim->v3 = 0xA0;
            prim->v1 = 0xA0;
            prim->u0 = prim->u1 = ex->btlfx_lerpRatio(0xBF, 0, i, n);
            prim->u2 = prim->u3 = ex->btlfx_lerpRatio(0xBF, 0, i + 1, n);
            if (cur->xy0.vx < fx_105_worldRibbonPts[i + 1].xy0.vx) {
                ex->btlfx_addPrim(0x09000000, 0, prim);
            } else {
                ex->btlfx_addPrim(0x09000000, (cur->z + fx_105_worldRibbonPts[i + 1].z) >> 1, prim);
            }
        }
    }
}

INCLUDE_RODATA("asm/us/fx_105/nonmatchings/fx_105", fx_105_headerPtr);
