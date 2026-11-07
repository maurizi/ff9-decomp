#include "fxmod/fx_105.h"

static void fxm_drawRibbonTrail(SVECTOR *posA, SVECTOR *posB, u32 n, CVECTOR *colorA, CVECTOR *colorB, s32 fade, Fx105Seg *buf, s32 abr) {
    Unk800F78A4 *env = fx_105_env;
    CVECTOR colA;
    CVECTOR colB;
    s32 otz;
    BtlfxExports *ex = BTLFX_EXPORTS(env);
    POLY_G4 *prim;
    Fx105Seg *seg;
    u32 i;
    s32 fadeNear;
    s32 fadeFar;

    if (fade < 0) {
        fade = 0x1000;
    }
    if (n >= 1000) {
        n -= 999;
        for (i = 0; i < n; i++) {
            buf[i].pos0.pad = 0;
        }
        return;
    }
    seg = &buf[(s32)(n - 1)];
    for (i = 0; i < n; i++) {
        BTLFX_COPY_SVEC(&seg[1].pos0, &seg[0].pos0);
        BTLFX_COPY_SVEC(&seg[1].pos8, &seg[0].pos8);
        seg--;
    }
    seg = buf;
    for (i = 0; i < n; i++, seg++) {
        if (!(seg[0].pos0.pad & seg[1].pos0.pad)) {
            break;
        }
    }
    BTLFX_COPY_SVEC(&buf->pos0, posA);
    buf->pos0.pad = 1;
    BTLFX_COPY_SVEC(&buf->pos8, posB);
    n = i;
    prim = (POLY_G4 *)env->prim;
    env->prim = (s32)(prim + n);
    if (colorA == NULL) {
        colA.r = colA.g = colA.b = 0;
    } else {
        gte_DpqColorFar_nst(colorA, fade);
        gte_strgb(&colA);
    }
    if (colorB == NULL) {
        colB.r = colB.g = colB.b = 0;
    } else {
        gte_DpqColorFar_nst(colorB, fade);
        gte_strgb(&colB);
    }
    gte_SetRotMatrix(env->viewMatrix);
    gte_SetTransMatrix(env->viewMatrix);
    colA.cd = abr != 0xFF ? 0x3A : 0x38;
    colB.cd = abr != 0xFF ? 0x3A : 0x38;
    for (i = 0; i < n; i++, buf++, prim++) {
        gte_ldv3c(buf);
        fadeNear = 0x1000 - (i << 12) / n;
        gte_rtpt_b();
        fadeFar = 0x1000 - ((i + 1) << 12) / n;
        gte_stopz(&otz);
        if (otz != 0) {
            gte_stsxy3(&prim->x0, &prim->x1, &prim->x2);
            gte_avsz3();
            gte_stszotz(&otz);
            gte_ldv0(&buf[1].pos8);
            gte_rtps();
            ex->btlfx_addPrimAbr(0x8000000, otz, prim, abr);
            gte_stsxy(&prim->x3);
            gte_DpqColorFar_nst(&colA, fadeNear);
            gte_strgb(&prim->r0);
            gte_DpqColorFar_nst(&colB, fadeNear);
            gte_strgb(&prim->r1);
            gte_DpqColorFar_nst(&colA, fadeFar);
            gte_strgb(&prim->r2);
            gte_DpqColorFar_nst(&colB, fadeFar);
            gte_strgb(&prim->r3);
        }
    }
}

static void fxm_camKeyReset(void) {
    s32 i;
    s32 freeMark;

    fx_105_camFrame = 0;
    freeMark = FX_105_KEY_FREE;
    for (i = 15; i >= 0; i--) {
        fx_105_camKeys[i].timer4 = freeMark;
    }
}

static void fxm_camKeyAdd(s32 start, u32 dst, u32 src, s16 *data) {
    s32 i;

    for (i = 0; i < 16; i++) {
        if (fx_105_camKeys[i].timer4 == FX_105_KEY_FREE) {
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
    s32 dstSlot;
    s32 srcSlot;
    s32 x, y, z;
    s32 extIdx;

    for (i = 0; i < 16; i++) {
        if (fx_105_camKeys[i].timer4 == 0 && fx_105_camKeys[i].frame0 == frame) {
            fx_105_camKeys[i].timer4 = *fx_105_camKeys[i].keys10;
            fx_105_camKeys[i].keys10++;
        }
        if (fx_105_camKeys[i].timer4 > 0) {
            fx_105_camKeys[i].timer4--;
            for (k = 0; k < 3; k++) {
                dstSlot = BTLFX_CAM_SLOT(fx_105_camKeys[i].dstSlots8, k);
                srcSlot = BTLFX_CAM_SLOT(fx_105_camKeys[i].srcSlotsC, k);
                if (dstSlot >= 0) {
                    x = *fx_105_camKeys[i].keys10;
                    fx_105_camKeys[i].keys10++;
                    y = *fx_105_camKeys[i].keys10;
                    fx_105_camKeys[i].keys10++;
                    z = *fx_105_camKeys[i].keys10;
                    fx_105_camKeys[i].keys10++;
                    if (srcSlot >= 0) {
                        /* each branch adds its own source; jump2's cross-jumping merges the identical tails */
                        if (srcSlot >= 10) {
                            extIdx = srcSlot - 10;
                            x += ext[extIdx].vx;
                            y += ext[extIdx].vy;
                            z += ext[extIdx].vz;
                        } else {
                            x += ((SVECTOR *)env->camera)[srcSlot].vx;
                            y += ((SVECTOR *)env->camera)[srcSlot].vy;
                            z += ((SVECTOR *)env->camera)[srcSlot].vz;
                        }
                    }
                    if (dstSlot >= 10) {
                        extIdx = dstSlot - 10;
                        ext[extIdx].vx = x;
                        ext[extIdx].vy = y;
                        ext[extIdx].vz = z;
                    } else {
                        ((SVECTOR *)env->camera)[dstSlot].vx = x;
                        ((SVECTOR *)env->camera)[dstSlot].vy = y;
                        ((SVECTOR *)env->camera)[dstSlot].vz = z;
                    }
                }
            }
        }
    }
    fx_105_camFrame++;
}

void fx_105_drawScreenBand(s32 z, s32 tpage, s32 y0, s32 y1, CVECTOR *rgb) {
    POLY_G4 *poly = (POLY_G4 *)fx_105_env->prim;
    BtlfxExports *ex = BTLFX_EXPORTS(fx_105_env);

    fx_105_env->prim = (u8 *)(poly + 1);
    poly->code = (tpage != 0xFF) ? 0x3A : 0x38;
    poly->r0 = rgb[0].r;
    poly->g0 = rgb[0].g;
    poly->b0 = rgb[0].b;
    poly->r1 = rgb[1].r;
    poly->g1 = rgb[1].g;
    poly->b1 = rgb[1].b;
    poly->r2 = rgb[2].r;
    poly->g2 = rgb[2].g;
    poly->b2 = rgb[2].b;
    poly->r3 = rgb[3].r;
    poly->g3 = rgb[3].g;
    poly->b3 = rgb[3].b;
    poly->x2 = 0;
    poly->x0 = 0;
    poly->x3 = 0x140;
    poly->x1 = 0x140;
    poly->y1 = y0;
    poly->y0 = y0;
    poly->y3 = y1;
    poly->y2 = y1;
    ex->btlfx_addPrim(0x08000000, z * 4, poly);
    ex->btlfx_addPrim(0x01000000, z * 4, ex->btlfx_allocDrMode(0, tpage));
}

s32 fx_105_op80(s32 mode, Fx105Work80 *work, Unk800F78A4 *env, Unk80100C78Entry *entry, s32 *args) {
    BtlfxExports *ex = BTLFX_EXPORTS(env);
    BtlBone bone1;
    BtlBone bone2;
    CVECTOR nightBand[4];
    CVECTOR flashBand[4];
    CVECTOR dimBand[4];
    CVECTOR lightBand[4];
    s32 frame;
    s32 i;
    s32 bright;
    s32 bandZ;
    s32 tp;
    s32 flat;
    s32 ribbonFade;
    s32 nightBlue;
    s32 flashRight;
    s32 flashLeft;
    s32 dimLeft;
    s32 dimRight;
    s32 ribbonLen;
    s32 morphStep;
    s32 rgb;

    if (mode == 0) {
        *args = 0x1D8;
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
        work->model138[0] = ex->btlfx_registerEffectModel((Unk800E8F50 *)ex->btlfx_getFileData(entry->file, FX_ODIN_STONE_RIBBON), 0xFF);
        work->model138[1] = ex->btlfx_registerEffectModel((Unk800E8F50 *)ex->btlfx_getFileData(entry->file, FX_ODIN_STONE_RIBBON_2), 0xFF);
        work->model138[2] = ex->btlfx_registerEffectModel((Unk800E8F50 *)ex->btlfx_getFileData(entry->file, FX_ODIN_STONE_RIBBON_3), 0xFF);
        work->model138[3] = ex->btlfx_registerEffectModel((Unk800E8F50 *)ex->btlfx_getFileData(entry->file, FX_ODIN_STONE_RIBBON_4), 0xFF);
        work->model138[4] = ex->btlfx_registerEffectModel((Unk800E8F50 *)ex->btlfx_getFileData(entry->file, FX_ODIN_STONE_RIBBON_5), 0xFF);
        work->model138[5] = ex->btlfx_registerEffectModel((Unk800E8F50 *)ex->btlfx_getFileData(entry->file, FX_ODIN_STONE_RIBBON_6), 0xFF);
        work->model138[6] = ex->btlfx_registerEffectModel((Unk800E8F50 *)ex->btlfx_getFileData(entry->file, FX_ODIN_STONE_RIBBON_7), 0xFF);
        work->model138[7] = ex->btlfx_registerEffectModel((Unk800E8F50 *)ex->btlfx_getFileData(entry->file, FX_ODIN_STONE_RIBBON_8), 0xFF);
        work->scale170[0] = 0x2000;
        work->scale170[1] = 0x2000;
        work->pos158[0] = 0;
        work->pos158[1] = 0;
        work->pos158[2] = 0;
        work->rot168.vx = 0;
        work->rot168.vy = 0;
        work->rot168.vz = 0;
        work->scale170[2] = 0x1A80;
        work->model180 = ex->btlfx_registerEffectModel((Unk800E8F50 *)ex->btlfx_getFileData(entry->file, FX_ODIN_WHITE_FOLD_0), 2);
        work->model184 = ex->btlfx_registerEffectModel((Unk800E8F50 *)ex->btlfx_getFileData(entry->file, FX_ODIN_WHITE_FOLD_1), 2);
        work->model188 = ex->btlfx_registerEffectModel((Unk800E8F50 *)ex->btlfx_getFileData(entry->file, FX_ODIN_WHITE_FOLD_2), 2);
        work->model18C = ex->btlfx_registerEffectModel((Unk800E8F50 *)ex->btlfx_getFileData(entry->file, FX_ODIN_WHITE_FOLD_3), 2);
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
        fxm_camKeyAdd(0, 3, 0, (s16 *)ex->btlfx_getFileData(entry->file, FX_ODIN_SHOT0_CAMERA_KEYS_3));
        fxm_camKeyAdd(0, 1, 0, (s16 *)ex->btlfx_getFileData(entry->file, FX_ODIN_SHOT0_CAMERA_KEYS_1));
        fxm_camKeyAdd(0, 2, 0, (s16 *)ex->btlfx_getFileData(entry->file, FX_ODIN_SHOT0_CAMERA_KEYS_2));
        fxm_camKeyAdd(0, 9, 0, (s16 *)ex->btlfx_getFileData(entry->file, FX_ODIN_SHOT0_CAMERA_KEYS_9));
        fxm_camKeyAdd(0, 4, 0, (s16 *)ex->btlfx_getFileData(entry->file, FX_ODIN_SHOT0_CAMERA_KEYS_4));
        fxm_camKeyAdd(0xA7, 3, 0, (s16 *)ex->btlfx_getFileData(entry->file, FX_ODIN_SHOT1_CAMERA_KEYS_3));
        fxm_camKeyAdd(0xA7, 1, 0, (s16 *)ex->btlfx_getFileData(entry->file, FX_ODIN_SHOT1_CAMERA_KEYS_1));
        fxm_camKeyAdd(0xA7, 2, 0, (s16 *)ex->btlfx_getFileData(entry->file, FX_ODIN_SHOT1_CAMERA_KEYS_2));
        fxm_camKeyAdd(0xFB, 3, 0, (s16 *)ex->btlfx_getFileData(entry->file, FX_ODIN_SHOT2_CAMERA_KEYS_3));
        fxm_camKeyAdd(0xFB, 1, 0, (s16 *)ex->btlfx_getFileData(entry->file, FX_ODIN_SHOT2_CAMERA_KEYS_1));
        fxm_camKeyAdd(0xFB, 2, 0, (s16 *)ex->btlfx_getFileData(entry->file, FX_ODIN_SHOT2_CAMERA_KEYS_2));
        fxm_camKeyAdd(0x137, 3, 0, (s16 *)ex->btlfx_getFileData(entry->file, FX_ODIN_SHOT3_CAMERA_KEYS_3));
        fxm_camKeyAdd(0x137, 1, 0, (s16 *)ex->btlfx_getFileData(entry->file, FX_ODIN_SHOT3_CAMERA_KEYS_1));
        fxm_camKeyAdd(0x137, 2, 0, (s16 *)ex->btlfx_getFileData(entry->file, FX_ODIN_SHOT3_CAMERA_KEYS_2));
        return 0;
    } else {
        frame = entry->frame;
        switch (frame) {
        case 0:
            work->emit124 = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, FX_ODIN_MOON), (s32)&work->pos128);
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
            ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, FX_ODIN_SPECK_SPIRAL), (s32)&work->pos58);
            ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, FX_ODIN_PALE_CLOUD_2), (s32)&work->pos60);
            ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, FX_ODIN_PALE_CLOUD), (s32)&work->pos68);
            work->emit40 = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, FX_ODIN_WHITE_DEBRIS), (s32)&work->pos70);
            work->emit44 = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, FX_ODIN_WHITE_DEBRIS), (s32)&work->pos78);
            break;
        case 0x41:
            work->flag10 = 1;
            break;
        case 0x51:
            work->emit48 = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, FX_ODIN_DEBRIS_SPIRAL), (s32)&work->pos80);
            work->emit4C = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, FX_ODIN_DEBRIS_SPIRAL), (s32)&work->pos90);
            work->emit50 = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, FX_ODIN_DEBRIS_SPIRAL), (s32)&work->pos88);
            work->emit54 = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, FX_ODIN_DEBRIS_SPIRAL), (s32)&work->pos98);
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
            ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, FX_ODIN_DEBRIS_RINGS), (s32)&work->posA0);
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
            work->emitA8 = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, FX_ODIN_DEBRIS_CONE), (s32)&work->posB4);
            work->emitAC = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, FX_ODIN_DEBRIS_CONE), (s32)&work->posBC);
            work->emitB0 = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, FX_ODIN_DEBRIS_CONE), (s32)&work->posC4);
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
            work->emitD4 = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, FX_ODIN_DEBRIS_BURST), (s32)&work->posE4);
            work->emitD8 = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, FX_ODIN_DEBRIS_BURST), (s32)&work->posEC);
            work->emitDC = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, FX_ODIN_DEBRIS_BURST), (s32)&work->posF4);
            work->emitE0 = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, FX_ODIN_DEBRIS_BURST), (s32)&work->posFC);
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
            ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, FX_ODIN_SMOKE_SPIRAL), (s32)&work->pos104);
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
                fx_105_drawCamRibbon((s16 *)ex->btlfx_getFileData(entry->file, FX_ODIN_CAM_RIBBON_PATH), 0x80, 0x20, (8 - work->timer28) * 8);
            } else {
                bright = ex->btlfx_lerpRatio(0, 0x80, work->timer28, 6);
                fx_105_drawCamRibbon((s16 *)ex->btlfx_getFileData(entry->file, FX_ODIN_CAM_RIBBON_PATH), bright, 0x20, 0x10);
            }
        }
        if (work->timer14 != 0) {
            work->timer14--;
            ex->btlfx_aimAngles((s16 *)&env->camera->points[0], (s16 *)&env->camera->points[1], (s16 *)&work->rot190);
            work->pos198[1] = env->camera->points[0].vy - ((ex->math_sin(work->rot190.vx) * 8) >> 12);
            flat = abs((ex->math_cos(work->rot190.vx) * 8) >> 12);
            work->pos198[0] = env->camera->points[0].vx + ((ex->math_sin(work->rot190.vy) * flat) >> 12);
            work->pos198[2] = env->camera->points[0].vz + ((ex->math_cos(work->rot190.vy) * flat) >> 12);
            work->scale1A8[0] = 0x333;
            work->scale1A8[1] = 0x333;
            work->scale1A8[2] = 0x333;
            if (work->timer14 >= 0x12) {
                ex->btlfx_drawScreenTint((u32 *)env->ot, BTLFX_ABR_OPAQUE, 0, 0, 0);
            }
            if (work->timer14 == 0x11) {
                ex->Hi_DrawEffModel(&work->rot190, (VECTOR *)work->pos198, (s32)work->scale1A8, work->model180);
            }
            if (work->timer14 == 0x10) {
                ex->Hi_DrawEffModel(&work->rot190, (VECTOR *)work->pos198, (s32)work->scale1A8, work->model184);
            }
            morphStep = work->timer14;
            if (morphStep < 0x10) {
                ex->Hi_DrawMorphEffModel(&work->rot190, (VECTOR *)work->pos198, (s32)work->scale1A8, work->model188, work->model18C,
                                         morphStep << 8);
            }
            if (work->timer14 < 8) {
                rgb = -((8 - work->timer14) << 5);
                ex->Hi_ModifyEffModelRGB(work->model188, rgb, rgb, rgb);
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
            ribbonFade = 0x1000;
            if (work->timer4 < 4) {
                ribbonFade = work->timer4 << 10;
            }
            fxm_drawRibbonTrail(&work->pos10C, &work->pos114, 0x10, &work->color11C, &work->color120, ribbonFade, fx_105_ribbonSegs, 1);
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
            bright = 0x1000;
            tp = 0xFF;
            bandZ = 0xFFF;
            if (work->timerC >= 0x84) {
                bright = (0x93 - work->timerC) << 8;
            }
            if (work->timerC < 8) {
                bright = work->timerC << 9;
                tp = 1;
                bandZ = 0;
            }
            nightBlue = (bright * 0x30) >> 12;
            nightBand[2].b = nightBand[3].b = nightBlue;
            nightBand[0].b = nightBand[1].b = 0;
            nightBand[0].g = nightBand[1].g = 0;
            nightBand[0].r = nightBand[1].r = 0;
            nightBand[2].r = nightBand[3].r = 0;
            nightBand[2].g = nightBand[3].g = 0;
            fx_105_drawScreenBand(bandZ, tp, 0, 0xF0, nightBand);
        }
        if (work->timer30 != 0) {
            work->timer30--;
            for (i = 0; i < 32; i++) {
                bright = 0xFFF;
                if (fx_105_stars[i].flicker10 == 0) {
                    if (!(ex->btlfx_rand() & 0x1F)) {
                        fx_105_stars[i].flicker10 = (ex->btlfx_rand() & 7) + 1;
                    }
                } else {
                    fx_105_stars[i].flicker10--;
                    bright = (ex->btlfx_rand() & 0x3FF) + 0x800;
                }
                fx_105_stars[i].flicker10 = ex->btlfx_rand() & 0x1F;
                ex->btlfx_drawSpriteAtOt(0xFF8, (u8 *)ex->btlfx_getFileData(entry->file, FX_ODIN_STAR), fx_105_stars[i].posX0, fx_105_stars[i].posY4, 0, 1, bright);
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
                flashRight = ex->btlfx_lerpRatio(0xFF, 0, work->timer1C - 8, 8);
                flashBand[1].b = flashBand[3].b = flashRight;
                flashBand[1].g = flashBand[3].g = flashRight;
                flashBand[1].r = flashBand[3].r = flashRight;
            } else {
                flashBand[1].b = flashBand[3].b = 0xFF;
                flashBand[1].g = flashBand[3].g = 0xFF;
                flashBand[1].r = flashBand[3].r = 0xFF;
            }
            flashLeft = ex->btlfx_lerpRatio(0xFF, 0, work->timer1C, 0x10);
            flashBand[0].b = flashBand[2].b = flashLeft;
            flashBand[0].g = flashBand[2].g = flashLeft;
            flashBand[0].r = flashBand[2].r = flashLeft;
            fx_105_drawScreenBand(0, 1, 0, 0xF0, flashBand);
        }
        if (work->timer20 != 0) {
            work->timer20--;
            if (work->timer20 >= 0x21) {
                dimLeft = ex->btlfx_lerpRatio(0, 0x7F, work->timer20 - 0x20, 0x10);
                dimBand[0].b = dimBand[2].b = dimLeft;
                dimBand[0].g = dimBand[2].g = dimLeft;
                dimBand[0].r = dimBand[2].r = dimLeft;
            } else {
                dimBand[0].b = dimBand[2].b = 0;
                dimBand[0].g = dimBand[2].g = 0;
                dimBand[0].r = dimBand[2].r = 0;
            }
            dimRight = ex->btlfx_lerpRatio(0, 0x7F, work->timer20, 0x30);
            dimBand[1].b = dimBand[3].b = dimRight;
            dimBand[1].g = dimBand[3].g = dimRight;
            dimBand[1].r = dimBand[3].r = dimRight;
            fx_105_drawScreenBand(0, 1, 0, 0xF0, dimBand);
        }
        if (work->timer18 != 0) {
            work->timer18--;
            lightBand[2].b = lightBand[3].b = 0xFF;
            lightBand[2].g = lightBand[3].g = 0xFF;
            lightBand[2].r = lightBand[3].r = 0xFF;
            lightBand[0].b = lightBand[1].b = 0;
            lightBand[0].g = lightBand[1].g = 0;
            lightBand[0].r = lightBand[1].r = 0;
            fx_105_drawScreenBand(0, 2, 0xA0, 0xF0, lightBand);
        }
        if (work->timer24 != 0) {
            work->timer24--;
            bright = 0x1000;
            if (work->timer24 >= 0x31) {
                bright = (0x30 - work->timer24) << 10;
            }
            if (work->timer24 < 4) {
                bright = work->timer24 << 10;
            }
            fx_105_drawWindStreaks(bright);
        }
        if (work->timer2C != 0) {
            work->timer2C--;
            ribbonLen = ex->btlfx_lerpRatio(0x3F, 0, work->timer2C, 0x1C);
            bright = 0x80;
            if (work->timer2C < 0x10) {
                bright = work->timer2C * 8;
            }
            fx_105_drawWorldRibbon((s16 *)ex->btlfx_getFileData(entry->file, FX_ODIN_RIBBON_PATH), bright, bright, ribbonLen);
            fx_105_drawWorldRibbon((s16 *)ex->btlfx_getFileData(entry->file, FX_ODIN_RIBBON_PATH_2), bright, bright, ribbonLen);
            fx_105_drawWorldRibbon((s16 *)ex->btlfx_getFileData(entry->file, FX_ODIN_RIBBON_PATH_3), bright, bright, ribbonLen);
            fx_105_drawWorldRibbon((s16 *)ex->btlfx_getFileData(entry->file, FX_ODIN_RIBBON_PATH_4), bright, bright, ribbonLen);
            fx_105_drawWorldRibbon((s16 *)ex->btlfx_getFileData(entry->file, FX_ODIN_RIBBON_PATH_5), bright, bright, ribbonLen);
        }
        fxm_camKeyUpdate();
    }
    return 0;
}

void fx_105_initWindStreaks(void) {
    Unk800F78A4 *env = fx_105_env;
    BtlfxExports *ex = BTLFX_EXPORTS(env);
    Fx105Spark *streak;
    s32 offX;
    s32 i;

    for (i = 0; i < 0x20; i++) {
        offX = ex->btlfx_randRange(-((i + 1) * 0x180), -(i * 0x180));
        streak = &fx_105_windStreaks[i];
        streak->vx = offX;
        streak->vz = ex->btlfx_randRange(-0x1000, 0x2000);
        streak->vy = streak->vx >> 4;
        streak->vx += env->camera->points[2].vx;
        streak->vz += env->camera->points[2].vz;
        streak->vy += env->camera->points[2].vy;
    }
}

void fx_105_drawWindStreaks(s32 bright) {
    Unk800F78A4 *env = fx_105_env;
    BtlfxExports *ex = BTLFX_EXPORTS(env);
    MATRIX m;
    SVECTOR quad[4];
    SVECTOR rot;
    u8 uv[4][8];
    s32 otz;
    POLY_FT4 *poly;
    s32 i;
    s32 halfDepth;
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
    poly = (POLY_FT4 *)env->prim;
    tp = 0x3A;
    env->prim = (u8 *)(poly + 32);
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
        halfDepth = i * 16 - 0x5E8;
        quad[0].vx = quad[2].vx = fx_105_windStreaks[i].vx - 0x400;
        quad[1].vx = quad[3].vx = fx_105_windStreaks[i].vx;
        quad[0].vz = quad[1].vz = fx_105_windStreaks[i].vz + halfDepth;
        quad[2].vz = quad[3].vz = fx_105_windStreaks[i].vz - halfDepth;
        quad[0].vy = quad[2].vy = fx_105_windStreaks[i].vy - 0x600;
        quad[1].vy = quad[3].vy = fx_105_windStreaks[i].vy;
        gte_ldv3c(quad);
        gte_rtpt();
        if (abr == 0xFF) {
            poly->code = GPU_POLY_FT4;
        } else {
            poly->code = GPU_POLY_FT4 | GPU_ABE;
        }
        poly->r0 = poly->g0 = poly->b0 = (bright * 0x70) >> 12;
        gte_stsxy3(&poly->x0, &poly->x1, &poly->x2);
        gte_avsz3();
        *(u16 *)&poly->u0 = *(u16 *)uv[0];
        *(u16 *)&poly->u1 = *(u16 *)uv[1];
        *(u16 *)&poly->u2 = *(u16 *)uv[2];
        *(u16 *)&poly->u3 = *(u16 *)uv[3];
        gte_stszotz(&otz);
        gte_ldv0(&quad[3]);
        gte_rtps();
        poly->clut = clut;
        poly->tpage = tp;
        gte_stsxy(&poly->x3);
        ex->btlfx_addPrim(0x09000000, otz >> 1, poly);
        poly++;
        fx_105_windStreaks[i].vz -= (fx_105_windStreaks[i].vx + 0x3000) >> 6;
        if (fx_105_windStreaks[i].vz < env->camera->points[2].vz - 0x1000) {
            fx_105_windStreaks[i].vz = 0x2000;
        }
    }
}

void fx_105_drawCamRibbon(s16 *data, s32 bright, s32 halfWidth, s32 n) {
    Unk800F78A4 *env = fx_105_env;
    BtlfxExports *ex = BTLFX_EXPORTS(env);
    SVECTOR pos;
    SVECTOR rot;
    SVECTOR cen;
    SVECTOR ang;
    SVECTOR edgeA;
    SVECTOR edgeB;
    MATRIX m;
    POLY_FT4 *prim;
    s32 dist;
    s32 tp;
    s32 i;
    s32 (*fsin)(s32);
    s32 av;

    edgeA.vx = edgeA.vz = edgeB.vx = edgeB.vz = 0;
    edgeA.vy = -halfWidth;
    edgeB.vy = halfWidth;
    ex->btlfx_aimAngles(&env->camera->points[0].vx, &env->camera->points[1].vx, &ang.vx);
    i = 0;
    cen.vy = env->camera->points[0].vy - ((ex->math_sin(ang.vx) * 0x800) >> 12);
    data += 2;
    dist = ex->math_cos(ang.vx) * 0x800;
    dist >>= 12;
    /* The do-while only raises dist's reference count so global-alloc puts it ahead of data; the sin pointer and
     * argument are loaded before the abs() branch in the original. */
    do {
        av = ang.vy;
        fsin = ex->math_sin;
        if (dist < 0) {
            dist = -dist;
        }
    } while (0);
    cen.vx = env->camera->points[0].vx + ((fsin(av) * dist) >> 12);
    cen.vz = env->camera->points[0].vz + ((ex->math_cos(ang.vy) * dist) >> 12);
    for (; i < 0x11; i++) {
        pos.vx = -*data++ + cen.vx;
        pos.vy = *data++ + cen.vy;
        pos.vz = -*data++ + cen.vz;
        rot.vx = *data++ + ang.vx;
        rot.vy = *data++ + ang.vy;
        rot.vz = *data++ + ang.vz;
        gte_SetRotMatrix(env->viewMatrix);
        gte_SetTransMatrix(env->viewMatrix);
        gte_ldv0(&pos);
        gte_rtv0tr();
        gte_stlvnl(m.t);
        ex->math_rotMatrixYXZ(&rot, &m);
        ex->MulRotMatrix(&m);
        gte_SetRotMatrix(&m);
        gte_SetTransMatrix(&m);
        gte_ldv0(&edgeA);
        gte_rtps();
        gte_stsxy(&fx_105_camRibbonPts[i].xy0);
        gte_ldv0(&edgeB);
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
            prim->b0 = bright;
            prim->g0 = bright;
            prim->r0 = bright;
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

void fx_105_drawWorldRibbon(s16 *data, s32 bright, s32 unused, s32 n) {
    Unk800F78A4 *env;
    BtlfxExports *ex;
    SVECTOR pos;
    SVECTOR rot;
    SVECTOR edgeA;
    SVECTOR edgeB;
    MATRIX m;
    POLY_FT4 *prim;
    Fx105RibbonPt *cur;
    s16 *src;
    s32 i;
    s32 tp;

    src = data + 2;
    env = fx_105_env;
    ex = BTLFX_EXPORTS(env);
    edgeA.vy = -0xC0;
    edgeA.vx = edgeA.vz = edgeB.vx = edgeB.vz = 0;
    edgeB.vy = 0xC0;
    for (i = 0; i < 0x40; i++) {
        pos.vx = *src++;
        pos.vy = *src++;
        pos.vz = *src++ + env->targets->centerZ;
        rot.vx = *src++;
        rot.vy = *src++;
        rot.vz = *src++;
        gte_SetRotMatrix(env->viewMatrix);
        gte_SetTransMatrix(env->viewMatrix);
        gte_ldv0(&pos);
        gte_rtv0tr();
        gte_stlvnl(m.t);
        ex->math_rotMatrixYXZ(&rot, &m);
        ex->MulRotMatrix(&m);
        gte_SetRotMatrix(&m);
        gte_SetTransMatrix(&m);
        gte_ldv0(&edgeA);
        gte_rtps();
        gte_stsxy(&fx_105_worldRibbonPts[i].xy0);
        gte_ldv0(&edgeB);
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
            prim->b0 = bright;
            prim->g0 = bright;
            prim->r0 = bright;
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
