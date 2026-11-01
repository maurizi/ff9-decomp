#include "ovl_0c.h"

void wm_initEffects(void) {
    s32 i;

    for (i = 0; i < 10; i++) {
        g_worldBlockFx6RingRot[i].vx = i << 7;
        g_worldBlockFx6RingRot[i].vy = rand() % 4096;
        g_worldBlockFx6RingRot[i].vz = 0;
        g_worldBlockFx6RingDist[i] = i * 0x14 + 0x200;
        g_worldBlockFx6FlashLen[i] = 0x800;
        g_worldBlockFx6FlashTimer[i] = 0;
        g_worldBlockFx6FlashRot[i].vx = rand() % 4096;
        g_worldBlockFx6FlashRot[i].vy = rand() % 4096;
        g_worldBlockFx6FlashRot[i].vz = 0;
    }
    for (i = 0; i < 9; i++) {
        g_worldEffectPos[i].vx = 0;
        g_worldEffectPos[i].vy = 0;
        g_worldEffectPos[i].vz = 0;
        g_worldEffectRot[i].vx = 0;
        g_worldEffectRot[i].vy = 0;
        g_worldEffectRot[i].vz = 0;
    }
    setVector(&g_worldEffectPos[0], 0x4464C, -0x1E68, -0x32681);
    setVector(&g_worldEffectPos[1], 0x3805B, 0x1F4, -0x30847);
    setVector(&g_worldEffectPos[6], 0x2FFFA, -0x2A82, -0x14052);
    g_worldEffectPos[2] = g_worldEffectPos[1];
    g_worldEffectPos[3] = g_worldEffectPos[1];
    g_worldEffectPos[4] = g_worldEffectPos[1];
    g_worldEffectPos[5] = g_worldEffectPos[1];
    g_worldEffectPos[7] = g_worldEffectPos[6];
    g_worldEffectPos[8] = g_worldEffectPos[6];
    g_worldEffectFree.prev = NULL;
    g_worldEffectFree.next = &g_worldEffectNodes[0];
    g_worldEffectNodes[0].prev = &g_worldEffectFree;
    g_worldEffectNodes[0].next = &g_worldEffectNodes[1];
    for (i = 1; i < 59; i++) {
        g_worldEffectNodes[i].prev = &g_worldEffectNodes[i - 1];
        g_worldEffectNodes[i].next = &g_worldEffectNodes[i + 1];
    }
    g_worldEffectNodes[i].prev = &g_worldEffectNodes[i - 1];
    g_worldEffectNodes[i].next = &g_worldEffectFreeTail;
    g_worldEffectFreeTail.prev = &g_worldEffectNodes[i];
    g_worldEffectFreeTail.next = NULL;
    g_worldEffectActive.prev = NULL;
    g_worldEffectActive.next = &g_worldEffectActiveTail;
    g_worldEffectActiveTail.prev = &g_worldEffectActive;
    g_worldEffectActiveTail.next = NULL;
    g_worldBlockFx4Rot.vy = 0;
    g_worldBlockFx6RotA.vy = 0;
    g_worldBlockFx6RotB.vy = 0;
    g_worldBlockFx4PartRots[0].vy = 0;
    g_worldBlockFx4PartRots[1].vy = 0;
    g_worldBlockFx4PartRots[2].vy = 0;
    g_worldEffectSpeed = 0;
    g_worldSprayLevel = 0;
}

INCLUDE_RODATA("asm/us/ovl_0c/nonmatchings/wm_effect", g_worldEffectFlatRot);

void wm_drawEffects(void) {
    WmEffect *fx;
    VECTOR rel;
    VECTOR view;
    SVECTOR pos;
    s32 tex;
    s32 k;

    fx = g_worldEffectActive.next;
    SetRotMatrix(g_worldViewMatrix);
    SetTransMatrix(g_worldViewMatrix);
    while (fx->next != NULL) {
        if ((u8)wm_worldToView((VECTOR *)&fx->x, &rel, &view, 0)) {
            pos.vx = rel.vx;
            pos.vy = rel.vy;
            pos.vz = rel.vz;
            if (view.vz > 1000) {
                tex = g_worldOt[*g_worldBufIndex];
                switch ((u8)fx->type) {
                case 2:
                    g_worldOtBias = 0;
                    g_worldPrimPtr = wm_drawSprite3D(g_worldPrimPtr, (WmSprHdr *)fx->sprite, &pos, NULL, fx->scale = fx->frame * 400 + 6000,
                                               (s16)((s16)(fx->frame / 4) % 26), (u8)fx->abr, -1, tex, 0, 2);
                    break;
                case 3:
                    g_worldOtBias = 0;
                    g_worldPrimPtr = wm_drawSprite3D(g_worldPrimPtr, (WmSprHdr *)fx->sprite, &pos, NULL, fx->scale = fx->frame * 400 + 6000,
                                               (s16)((s16)(fx->frame / 2) % 23), (u8)fx->abr, -1, tex, 0, 2);
                    break;
                case 8: {
                    SVECTOR rot = g_worldEffectFlatRot;
                    g_worldOtBias = 20;
                    g_worldPrimPtr = wm_drawSprite3D(g_worldPrimPtr, (WmSprHdr *)fx->sprite, &pos, &rot, fx->scale, fx->frame, (u8)fx->abr,
                                               -1, tex, 0, 0);
                    break;
                }
                case WM_FX_BOAT_WAKE:
                case WM_FX_WATER_RIPPLE: {
                    SVECTOR rot = g_worldEffectFlatRot;
                    g_worldOtBias = 20;
                    g_worldPrimPtr = wm_drawSprite3D(g_worldPrimPtr, (WmSprHdr *)fx->sprite, &pos, &rot, fx->scale, fx->frame, (u8)fx->abr,
                                               g_worldEffectSpeed, tex, 0, 0);
                    break;
                }
                case WM_FX_FOREST_LEAVES: {
                    SVECTOR rot = g_worldEffectFlatRot;
                    pos.vy -= 400;
                    fx->scale = (8 - fx->frame) * 4000;
                    rot.vy = fx->frame * 128;
                    g_worldOtBias = 0;
                    g_worldPrimPtr = wm_drawSprite3D(g_worldPrimPtr, (WmSprHdr *)fx->sprite, &pos, &rot, fx->scale, 0, 1, -1, tex, 0, 0);
                    rot.vy = fx->frame * 200;
                    g_worldPrimPtr = wm_drawSprite3D(g_worldPrimPtr, (WmSprHdr *)fx->sprite, &pos, &rot, fx->scale, 0, 2, -1, tex, 0, 0);
                    break;
                }
                case WM_FX_CHOCOBO_SAND:
                case WM_FX_AIRSHIP_SPRAY:
                case WM_FX_AIRSHIP_SAND:
                    if (g_worldSprayTarget >= 0x1000) {
                        g_worldSprayTarget = 0xFFF;
                    }
                    if (g_worldSprayTarget < 0) {
                        g_worldSprayTarget = 0;
                    }
                    g_worldSprayLevel += (g_worldSprayTarget - g_worldSprayLevel) / 32;
                    g_worldOtBias = 50;
                    k = g_worldEffectSpeed * g_worldSprayLevel >> 12;
                    if (k >= 25) {
                        g_worldPrimPtr = wm_drawSprite3D(g_worldPrimPtr, (WmSprHdr *)fx->sprite, &pos, NULL, fx->scale, fx->frame,
                                                   (u8)fx->abr, k, tex, 0, 0);
                    }
                    break;
                case 25:
                case 26: {
                    SVECTOR rot;
                    memset(&rot, 0, 8);
                    rot.pad = 1;
                    rot.vy = ratan2(0x30031 - fx->x, -0x13F6E - fx->z);
                    g_worldOtBias = 0;
                    g_worldPrimPtr = wm_drawSprite3D(g_worldPrimPtr, (WmSprHdr *)fx->sprite, &pos, &rot, fx->scale, fx->frame, (u8)fx->abr,
                                               -1, tex, 0, 0);
                    break;
                }
                default:
                    g_worldOtBias = 40;
                    g_worldPrimPtr = wm_drawSprite3D(g_worldPrimPtr, (WmSprHdr *)fx->sprite, &pos, NULL, fx->scale, fx->frame, (u8)fx->abr,
                                               -1, tex, 0, 0);
                    break;
                }
            }
        }
        fx = fx->next;
    }
}

void wm_updateEffects(void) {
    WmEffect *fx;
    WmEffect *cur;
    s16 spd;
    u8 flag;

    flag = 0;
    fx = g_worldEffectActive.next;
    spd = g_worldMoveModeInfo->speed;
    if (spd != 0) {
        g_worldEffectSpeedTarget = abs((g_worldPlayerSpeed << 12) / spd);
    } else {
        g_worldEffectSpeedTarget = 0;
    }
    g_worldEffectSpeed += (g_worldEffectSpeedTarget - g_worldEffectSpeed) / 32;
    while (fx->next != NULL) {
        /* dead flag: combine folds the test (flag is only ever 0) and jump2 drops the
         * body after register allocation, which leaves s7 = 0 and its save behind */
        if (flag) {
            wm_freeEffect(fx);
        }
        switch ((u8)fx->type) {
        case 2:
            fx->y -= 60 + wm_rand8() % 24;
            fx->x -= 30 - wm_rand8() % 60;
            fx->z -= 30 - wm_rand8() % 60;
            break;
        case 3:
            fx->y -= 60 + wm_rand8() % 24;
            fx->x -= 60 - wm_rand8() % 60;
            fx->z -= 30 - wm_rand8() % 60;
            break;
        case 23:
            fx->x = math_sin((fx->frame << 6) + fx->phase) / 2 + g_worldBlockFx4Pos.vx;
            fx->y -= 80;
            fx->z = math_cos((fx->frame << 6) + fx->phase) / 2 + g_worldBlockFx4Pos.vz;
            /* fallthrough */
        case 24:
            fx->x -= 40 + wm_rand8() % 130;
            fx->y -= 40 - wm_rand8() % 80;
            fx->z += 300 + wm_rand8() % 300;
            break;
        }
        fx->frame++;
        cur = fx;
        fx = fx->next;
        switch ((u8)cur->type) {
        case 2:
            if (cur->frame >= 77) {
                wm_freeEffect(cur);
            }
            break;
        case 3:
            if (cur->frame >= 46) {
                wm_freeEffect(cur);
            }
            break;
        case WM_FX_FOREST_LEAVES:
        case 17:
            if (cur->frame >= 9) {
                wm_freeEffect(cur);
            }
            break;
        case 32:
            if (abs(cur->x - g_worldPlayer->pos.vx) > 32000 || abs(cur->z - g_worldPlayer->pos.vz) > 32000) {
                wm_freeEffect(cur);
            }
            break;
        default:
            if (cur->frame == wm_getSpriteFrameCount((u16 *)cur->sprite)) {
                wm_freeEffect(cur);
            }
            break;
        }
    }
}

void wm_drawBlockFx(u8 fxId) {
    VECTOR view;
    MATRIX mtx;
    s32 i;
    s32 j;

    SetLightMatrix(&g_worldBlockFxLightMatrix);
    SetColorMatrix(&g_worldBlockFxColorMatrix);
    SetBackColor((u8)((u16)g_worldWeatherData.work[0].fxBackColor[0] >> 5), (u8)((u16)g_worldWeatherData.work[0].fxBackColor[1] >> 5),
                 (u8)((u16)g_worldWeatherData.work[0].fxBackColor[2] >> 5));
    switch (fxId) {
    case 4:
        if (g_worldFormFlags[WM_FORM_CLAY] == 0 && g_worldDataSet == WM_DATASET_DISC1) {
            VECTOR scale;

            math_rotMatrixXYZ(&g_worldBlockFx4Rot, &mtx);
            if ((u8)wm_worldToView(&g_worldBlockFx4Pos, (VECTOR *)mtx.t, &view, 0)) {
                scale = g_worldBlockFx4Scale;
                ScaleMatrix(&mtx, &scale);
                CompMatrix(g_worldViewMatrix, &mtx, &mtx);
                SetRotMatrix(&mtx);
                SetTransMatrix(&mtx);
                wm_drawModel(WM_PACK0(0x3A), 0x50, 1);
                wm_drawModel(WM_PACK0(0x35), 0x3C, 0);
                for (j = 0; j < 3; j++) {
                    math_rotMatrixXYZ(&g_worldBlockFx4PartRots[j], &mtx);
                    wm_worldToView(&g_worldBlockFx4Pos, (VECTOR *)mtx.t, &view, 0);
                    CompMatrix(g_worldViewMatrix, &mtx, &mtx);
                    SetRotMatrix(&mtx);
                    SetTransMatrix(&mtx);
                    wm_drawModel(WM_PACK0(j + 0x36), 0, 0);
                }
            }
        }
        break;
    case 7:
        math_rotMatrixXYZ(&g_worldBlockFx7Rot, &mtx);
        wm_worldToView(&g_worldBlockFx7Pos, (VECTOR *)mtx.t, &view, 0);
        CompMatrix(g_worldViewMatrix, &mtx, &mtx);
        SetRotMatrix(&mtx);
        SetTransMatrix(&mtx);
        wm_drawModel(WM_PACK0(0x39), 0, 1);
        break;
    case 6:
        if (g_worldDataSet == WM_DATASET_DISC4) {
            SVECTOR pos;
            u32 *ot;
            WmSprHdr *spr;
            s32 frames;

            ot = (u32 *)g_worldOt[*g_worldBufIndex];
            wm_worldToView(&g_worldBlockFx6Pos, (VECTOR *)mtx.t, &view, 0);
            g_worldOtBias = 0;
            pos.vx = mtx.t[0];
            pos.vy = mtx.t[1];
            pos.vz = mtx.t[2];
            spr = w_framePackGetPtr(g_worldDataPack, 0x25);
            frames = wm_getSpriteFrameCount((u16 *)spr) - 1;
            if (frames != 0) {
                g_worldPrimPtr = wm_drawSprite3D(g_worldPrimPtr, spr, &pos, NULL, g_worldBlockFx6SpriteSize, g_worldFrameCount / 2 % frames, 1, -1,
                                           (s32)ot, 0, 0);
            }
            wm_worldToView(&g_worldBlockFx6Pos, (VECTOR *)mtx.t, &view, 0);
            CompMatrix(g_worldViewMatrix, &mtx, &mtx);
            math_rotMatrixXYZ(&g_worldBlockFx6CoreRot, &mtx);
            SetRotMatrix(&mtx);
            SetTransMatrix(&mtx);
            wm_drawModel(WM_PACK0(0x3F), g_worldBlockFx6CoreOtOfs, 2);
            math_rotMatrixXYZ(&g_worldBlockFx6RotA, &mtx);
            wm_worldToView(&g_worldBlockFx6Pos, (VECTOR *)mtx.t, &view, 0);
            CompMatrix(g_worldViewMatrix, &mtx, &mtx);
            SetRotMatrix(&mtx);
            SetTransMatrix(&mtx);
            wm_drawModel(WM_PACK0(0x3C), 0, 2);
            math_rotMatrixXYZ(&g_worldBlockFx6RotB, &mtx);
            wm_worldToView(&g_worldBlockFx6Pos, (VECTOR *)mtx.t, &view, 0);
            CompMatrix(g_worldViewMatrix, &mtx, &mtx);
            SetRotMatrix(&mtx);
            SetTransMatrix(&mtx);
            wm_drawModel(WM_PACK0(0x3D), 0, 2);
            for (i = 0; i < 10; i++) {
                MATRIX m1;
                MATRIX m2;
                VECTOR rel;
                VECTOR scale;
                VECTOR scaleVec;
                u8 bright;

                m1 = g_worldIdentityMatrix;
                m2 = g_worldIdentityMatrix;
                if (g_worldBlockFx6FlashTimer[i] == 0) {
                    if (rand() % 5 == 0) {
                        g_worldBlockFx6FlashRot[i].vx = rand() % 4096;
                        g_worldBlockFx6FlashRot[i].vy = rand() % 4096;
                        g_worldBlockFx6FlashRot[i].vz = rand() % 4096;
                        g_worldBlockFx6FlashLen[i] = rand() % 1024 + 0x400;
                        g_worldBlockFx6FlashTimer[i] = 1;
                    }
                } else {
                    g_worldBlockFx6FlashLen[i] += rand() % 86 - 10;
                    if (++g_worldBlockFx6FlashTimer[i] == 33) {
                        g_worldBlockFx6FlashTimer[i] = 0;
                    }
                }
                if (g_worldBlockFx6FlashTimer[i] != 0) {
                    bright = ~(g_worldBlockFx6FlashTimer[i] * 7);
                    SetBackColor(bright >> 1, bright >> 2, bright);
                    m2.t[1] = -700;
                    math_rotMatrixXYZ(&g_worldBlockFx6FlashRot[i], &m1);
                    gte_CompMatrix(&m1, &m2, &mtx);
                    memset(&scaleVec, 0, sizeof(VECTOR));
                    scaleVec.vx = 0x2FA0;
                    scaleVec.vy = g_worldBlockFx6FlashLen[i];
                    scaleVec.vz = 0x1830;
                    scale = scaleVec;
                    ScaleMatrix(&mtx, &scale);
                    wm_worldToView(&g_worldBlockFx6Pos, &rel, &view, 0);
                    mtx.t[0] += rel.vx;
                    mtx.t[1] += rel.vy;
                    mtx.t[2] += rel.vz;
                    gte_CompMatrix(g_worldViewMatrix, &mtx, &mtx);
                    SetRotMatrix(&mtx);
                    SetTransMatrix(&mtx);
                    switch (rand() % 3) {
                    case 0:
                        wm_drawModel(WM_PACK0(0x40), 0, 2);
                        break;
                    case 1:
                        wm_drawModel(WM_PACK0(0x41), 0, 2);
                        break;
                    }
                }
            }
            for (i = 0; i < 10; i++) {
                MATRIX m1;
                MATRIX m2;
                VECTOR rel;
                VECTOR scale;
                s32 bright;

                m1 = g_worldIdentityMatrix;
                m2 = g_worldIdentityMatrix;
                bright = (math_sin(g_worldFrameCount * 16 + i * 400) >> 5) + 0x80;
                if (bright >= 0x100) {
                    bright = 0xFF;
                }
                SetBackColor((u8)bright >> 1, (u8)bright >> 2, (u8)bright);
                if (bright == 0) {
                    g_worldBlockFx6RingDist[i] = 0x400;
                } else {
                    g_worldBlockFx6RingDist[i] += 3;
                }
                m2.t[2] = -g_worldBlockFx6RingDist[i];
                math_rotMatrixZXY(&g_worldBlockFx6RingRot[i], &m1);
                gte_CompMatrix(&m1, &m2, &mtx);
                scale = g_worldBlockFx6RingScale;
                ScaleMatrix(&mtx, &scale);
                wm_worldToView(&g_worldBlockFx6Pos, &rel, &view, 0);
                mtx.t[0] += rel.vx;
                mtx.t[1] += rel.vy;
                mtx.t[2] += rel.vz;
                gte_CompMatrix(g_worldViewMatrix, &mtx, &mtx);
                SetRotMatrix(&mtx);
                SetTransMatrix(&mtx);
                wm_drawModel(WM_PACK0(0x3E), 0, 2);
            }
        }
        break;
    }
    SetLightMatrix(&g_worldMapLightMatrix);
    SetColorMatrix(&g_worldMapColorMatrix);
    SetBackColor(g_worldWeatherData.work[0].backColor.c[0], g_worldWeatherData.work[0].backColor.c[2], g_worldWeatherData.work[0].backColor.c[4]);
}

INCLUDE_RODATA("asm/us/ovl_0c/nonmatchings/wm_effect", g_worldBlockFx4Scale);

INCLUDE_RODATA("asm/us/ovl_0c/nonmatchings/wm_effect", g_worldBlockFx6RingScale);

void wm_updateBlockFx(s32 fxId) {
    WmEmitter *emitter;
    u8 on;
    s32 i;
    s32 unused;
    s32 x;
    s32 y;
    s32 z;
    s32 rnd;
    u16 rotY;

    on = 0;
    switch ((u8)fxId) {
    case 2:
        g_worldClutAnimFx2 = 1;
        on = 1;
        break;
    case WM_BLOCKFX_WATERFALLS:
        on = 1;
        break;
    case WM_BLOCKFX_WATER_SHRINE:
        if (g_worldFormFlags[WM_FORM_HOKORA_MIZU] != 0) {
            on = 1;
        }
        break;
    case 3:
        if ((u16)(*g_worldStoryCounterPtr - 0x24EA) < 0x1B9) {
            on = 1;
        }
        break;
    case 7:
        if (g_worldBlockFx7Spin != 0) {
            g_worldBlockFx7Rot.vy += 0x40;
        }
        break;
    case 4:
        on = 1;
        g_worldBlockFx4Rot.vy -= 0x100;
        g_worldBlockFx4PartRots[0].vy += 0x100;
        g_worldBlockFx4PartRots[1].vy -= 0x40;
        g_worldBlockFx4PartRots[2].vy -= 0x180;
        break;
    case 6:
        g_worldBlockFx6RotA.vz -= 8;
        g_worldBlockFx6RotA.vy -= 0x10;
        g_worldBlockFx6RotB.vz -= 0x10;
        g_worldBlockFx6RotB.vy -= 8;
        for (i = 0; i < 10; i++) {
            rotY = g_worldBlockFx6RingRot[i].vy - 10;
            g_worldBlockFx6RingRot[i].vy = rotY - i / 4;
        }
        for (i = 0; i < 10; i++) {
            g_worldBlockFx6FlashRot[i].vx += -6 + rand() % 13;
            g_worldBlockFx6FlashRot[i].vy += -6 + rand() % 13;
        }
        break;
    }
    if (on) {
        emitter = w_framePackGetPtr(g_worldDataPack, (u8)fxId + 0x28);
        for (; emitter->effect != -1; emitter++) {
            if (rand() % 4096 < emitter->chance) {
                x = emitter->x - emitter->rangeX / 2;
                y = emitter->y - emitter->rangeY / 2;
                z = emitter->z - emitter->rangeZ / 2;
                if (emitter->rangeX != 0) {
                    if (g_randPtr >= g_randTableEnd) {
                        rand_refill();
                        if (g_randPtr >= g_randTableEnd) {
                            rand_refill();
                        }
                    }
                    rnd = *g_randPtr++;
                    x += ((rnd << 8) | rnd) % emitter->rangeX;
                }
                if (emitter->rangeY != 0) {
                    if (g_randPtr >= g_randTableEnd) {
                        rand_refill();
                        if (g_randPtr >= g_randTableEnd) {
                            rand_refill();
                        }
                    }
                    rnd = *g_randPtr++;
                    y += ((rnd << 8) | rnd) % emitter->rangeY;
                }
                if (emitter->rangeZ != 0) {
                    if (g_randPtr >= g_randTableEnd) {
                        rand_refill();
                        if (g_randPtr >= g_randTableEnd) {
                            rand_refill();
                        }
                    }
                    rnd = *g_randPtr++;
                    z += ((rnd << 8) | rnd) % emitter->rangeZ;
                }
                wm_spawnEffect(x, y, z, emitter->effect, emitter->scale);
            }
        }
    }
}

void wm_termEffects(void) {
}

void wm_spawnEffect(s32 x, s32 y, s32 z, s32 type, s32 scale) {
    WmEffect *fx;
    s32 sprite;
    u8 k;
    WmEffect *h;

    fx = g_worldEffectFree.next;
    h = &g_worldEffectActive;
    switch (type) {
    case 2:
    case 3:
        k = 2;
        break;
    case WM_FX_FOREST_LEAVES:
        k = 0xFF;
        break;
    case 4:
    case 5:
    case WM_FX_CHOCOBO_SPRAY:
    case WM_FX_BEACH_SPLASH:
    case 8:
    case 9:
    case WM_FX_BOAT_SIDE_WAKE:
    case 11:
    case 12:
    case 13:
    case WM_FX_CHOCOBO_SAND:
    case WM_FX_BOAT_WAKE:
    case 17:
    case WM_FX_AIRSHIP_SPRAY:
    case WM_FX_AIRSHIP_SAND:
    case WM_FX_WATERFALL_MIST:
    case 21:
    case WM_FX_WATER_RIPPLE:
    case 23:
    case 24:
    case 25:
    case 26:
    case WM_FX_SAND_DUST:
    case 28:
    case 29:
        k = 1;
        break;
    default:
        k = 1;
        break;
    }
    if (fx->next != 0) {
        sprite = (s32)w_framePackGetPtr(g_worldDataPack, type + 7);
        fx->prev->next = fx->next;
        fx->next->prev = fx->prev;
        fx->prev = h;
        fx->next = h->next;
        h->next->prev = fx;
        h->next = fx;
        fx->abr = k;
        fx->frame = 0;
        fx->x = x;
        fx->y = y;
        fx->z = z;
        fx->sprite = sprite;
        fx->scale = scale;
        fx->type = type;
        fx->phase = 0;
        fx->unk24 = 0;
        if (type == 0x17) {
            fx->phase = rand() & 0xFFF;
        }
    }
}

void wm_freeEffect(WmEffect *fx) {
    fx->prev->next = fx->next;
    fx->next->prev = fx->prev;
    fx->prev = &g_worldEffectFree;
    fx->next = g_worldEffectFree.next;
    g_worldEffectFree.next->prev = fx;
    g_worldEffectFree.next = fx;
}

INCLUDE_RODATA("asm/us/ovl_0c/nonmatchings/wm_effect", s_menuMainEquipLabelStrs);
