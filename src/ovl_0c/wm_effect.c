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
        D_800D4960[i].vx = 0;
        D_800D4960[i].vy = 0;
        D_800D4960[i].vz = 0;
        D_800D49F0[i].vx = 0;
        D_800D49F0[i].vy = 0;
        D_800D49F0[i].vz = 0;
    }
    setVector(&D_800D4960[0], 0x4464C, -0x1E68, -0x32681);
    setVector(&D_800D4960[1], 0x3805B, 0x1F4, -0x30847);
    setVector(&D_800D4960[6], 0x2FFFA, -0x2A82, -0x14052);
    D_800D4960[2] = D_800D4960[1];
    D_800D4960[3] = D_800D4960[1];
    D_800D4960[4] = D_800D4960[1];
    D_800D4960[5] = D_800D4960[1];
    D_800D4960[7] = D_800D4960[6];
    D_800D4960[8] = D_800D4960[6];
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
    WmEffect *o;
    VECTOR a;
    VECTOR b;
    SVECTOR pos;
    s32 tex;
    s32 k;

    o = g_worldEffectActive.next;
    SetRotMatrix(g_worldViewMatrix);
    SetTransMatrix(g_worldViewMatrix);
    while (o->next != NULL) {
        if ((u8)wm_worldToView((VECTOR *)&o->x, &a, &b, 0)) {
            pos.vx = a.vx;
            pos.vy = a.vy;
            pos.vz = a.vz;
            if (b.vz > 1000) {
                tex = g_worldOt[*g_worldBufIndex];
                switch ((u8)o->type) {
                case 2:
                    g_worldOtBias = 0;
                    g_worldPrimPtr = wm_drawSprite3D(g_worldPrimPtr, (WmSprHdr *)o->sprite, &pos, NULL, o->scale = o->frame * 400 + 6000,
                                               (s16)((s16)(o->frame / 4) % 26), (u8)o->abr, -1, tex, 0, 2);
                    break;
                case 3:
                    g_worldOtBias = 0;
                    g_worldPrimPtr = wm_drawSprite3D(g_worldPrimPtr, (WmSprHdr *)o->sprite, &pos, NULL, o->scale = o->frame * 400 + 6000,
                                               (s16)((s16)(o->frame / 2) % 23), (u8)o->abr, -1, tex, 0, 2);
                    break;
                case 8: {
                    SVECTOR r = g_worldEffectFlatRot;
                    g_worldOtBias = 20;
                    g_worldPrimPtr = wm_drawSprite3D(g_worldPrimPtr, (WmSprHdr *)o->sprite, &pos, &r, o->scale, o->frame, (u8)o->abr,
                                               -1, tex, 0, 0);
                    break;
                }
                case 15:
                case 22: {
                    SVECTOR r = g_worldEffectFlatRot;
                    g_worldOtBias = 20;
                    g_worldPrimPtr = wm_drawSprite3D(g_worldPrimPtr, (WmSprHdr *)o->sprite, &pos, &r, o->scale, o->frame, (u8)o->abr,
                                               g_worldEffectSpeed, tex, 0, 0);
                    break;
                }
                case 16: {
                    SVECTOR r = g_worldEffectFlatRot;
                    pos.vy -= 400;
                    o->scale = (8 - o->frame) * 4000;
                    r.vy = o->frame * 128;
                    g_worldOtBias = 0;
                    g_worldPrimPtr = wm_drawSprite3D(g_worldPrimPtr, (WmSprHdr *)o->sprite, &pos, &r, o->scale, 0, 1, -1, tex, 0, 0);
                    r.vy = o->frame * 200;
                    g_worldPrimPtr = wm_drawSprite3D(g_worldPrimPtr, (WmSprHdr *)o->sprite, &pos, &r, o->scale, 0, 2, -1, tex, 0, 0);
                    break;
                }
                case 14:
                case 18:
                case 19:
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
                        g_worldPrimPtr = wm_drawSprite3D(g_worldPrimPtr, (WmSprHdr *)o->sprite, &pos, NULL, o->scale, o->frame,
                                                   (u8)o->abr, k, tex, 0, 0);
                    }
                    break;
                case 25:
                case 26: {
                    SVECTOR r;
                    memset(&r, 0, 8);
                    r.pad = 1;
                    r.vy = ratan2(0x30031 - o->x, -0x13F6E - o->z);
                    g_worldOtBias = 0;
                    g_worldPrimPtr = wm_drawSprite3D(g_worldPrimPtr, (WmSprHdr *)o->sprite, &pos, &r, o->scale, o->frame, (u8)o->abr,
                                               -1, tex, 0, 0);
                    break;
                }
                default:
                    g_worldOtBias = 40;
                    g_worldPrimPtr = wm_drawSprite3D(g_worldPrimPtr, (WmSprHdr *)o->sprite, &pos, NULL, o->scale, o->frame, (u8)o->abr,
                                               -1, tex, 0, 0);
                    break;
                }
            }
        }
        o = o->next;
    }
}

void wm_updateEffects(void) {
    WmEffect *o;
    WmEffect *cur;
    s16 spd;
    u8 flag;

    flag = 0;
    o = g_worldEffectActive.next;
    spd = g_worldMoveModeInfo->speed;
    if (spd != 0) {
        g_worldEffectSpeedTarget = abs((g_worldPlayerSpeed << 12) / spd);
    } else {
        g_worldEffectSpeedTarget = 0;
    }
    g_worldEffectSpeed += (g_worldEffectSpeedTarget - g_worldEffectSpeed) / 32;
    while (o->next != NULL) {
        /* dead flag: combine folds the test (flag is only ever 0) and jump2 drops the
         * body after register allocation, which leaves s7 = 0 and its save behind */
        if (flag) {
            wm_freeEffect(o);
        }
        switch ((u8)o->type) {
        case 2:
            o->y -= 60 + wm_rand8() % 24;
            o->x -= 30 - wm_rand8() % 60;
            o->z -= 30 - wm_rand8() % 60;
            break;
        case 3:
            o->y -= 60 + wm_rand8() % 24;
            o->x -= 60 - wm_rand8() % 60;
            o->z -= 30 - wm_rand8() % 60;
            break;
        case 23:
            o->x = math_sin((o->frame << 6) + o->phase) / 2 + g_worldBlockFx4Pos.vx;
            o->y -= 80;
            o->z = math_cos((o->frame << 6) + o->phase) / 2 + g_worldBlockFx4Pos.vz;
            /* fallthrough */
        case 24:
            o->x -= 40 + wm_rand8() % 130;
            o->y -= 40 - wm_rand8() % 80;
            o->z += 300 + wm_rand8() % 300;
            break;
        }
        o->frame++;
        cur = o;
        o = o->next;
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
        case 16:
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

void wm_drawBlockFx(u8 arg0) {
    VECTOR v;
    MATRIX m;
    s32 i;
    s32 j;

    SetLightMatrix(&g_worldBlockFxLightMatrix);
    SetColorMatrix(&g_worldBlockFxColorMatrix);
    SetBackColor((u8)((u16)g_worldWeatherData.work[0].fxBackColor[0] >> 5), (u8)((u16)g_worldWeatherData.work[0].fxBackColor[1] >> 5),
                 (u8)((u16)g_worldWeatherData.work[0].fxBackColor[2] >> 5));
    switch (arg0) {
    case 4:
        if (g_worldFormFlags[5] == 0 && g_worldDataSet == WM_DATASET_DISC1) {
            VECTOR scale;

            func_80030AF4(&g_worldBlockFx4Rot, &m);
            if ((u8)wm_worldToView(&g_worldBlockFx4Pos, (VECTOR *)m.t, &v, 0)) {
                scale = g_worldBlockFx4Scale;
                ScaleMatrix(&m, &scale);
                CompMatrix(g_worldViewMatrix, &m, &m);
                SetRotMatrix(&m);
                SetTransMatrix(&m);
                wm_drawModel(WM_PACK0(0x3A), 0x50, 1);
                wm_drawModel(WM_PACK0(0x35), 0x3C, 0);
                for (j = 0; j < 3; j++) {
                    func_80030AF4(&g_worldBlockFx4PartRots[j], &m);
                    wm_worldToView(&g_worldBlockFx4Pos, (VECTOR *)m.t, &v, 0);
                    CompMatrix(g_worldViewMatrix, &m, &m);
                    SetRotMatrix(&m);
                    SetTransMatrix(&m);
                    wm_drawModel(WM_PACK0(j + 0x36), 0, 0);
                }
            }
        }
        break;
    case 7:
        func_80030AF4(&g_worldBlockFx7Rot, &m);
        wm_worldToView(&g_worldBlockFx7Pos, (VECTOR *)m.t, &v, 0);
        CompMatrix(g_worldViewMatrix, &m, &m);
        SetRotMatrix(&m);
        SetTransMatrix(&m);
        wm_drawModel(WM_PACK0(0x39), 0, 1);
        break;
    case 6:
        if (g_worldDataSet == WM_DATASET_DISC4) {
            SVECTOR pos;
            u32 *ot;
            WmSprHdr *spr;
            s32 n;

            ot = (u32 *)g_worldOt[*g_worldBufIndex];
            wm_worldToView(&g_worldBlockFx6Pos, (VECTOR *)m.t, &v, 0);
            g_worldOtBias = 0;
            pos.vx = m.t[0];
            pos.vy = m.t[1];
            pos.vz = m.t[2];
            spr = w_framePackGetPtr(g_worldDataPack, 0x25);
            n = wm_getSpriteFrameCount((u16 *)spr) - 1;
            if (n != 0) {
                g_worldPrimPtr = wm_drawSprite3D(g_worldPrimPtr, spr, &pos, NULL, g_worldBlockFx6SpriteSize, g_worldFrameCount / 2 % n, 1, -1,
                                           (s32)ot, 0, 0);
            }
            wm_worldToView(&g_worldBlockFx6Pos, (VECTOR *)m.t, &v, 0);
            CompMatrix(g_worldViewMatrix, &m, &m);
            func_80030AF4(&g_worldBlockFx6CoreRot, &m);
            SetRotMatrix(&m);
            SetTransMatrix(&m);
            wm_drawModel(WM_PACK0(0x3F), g_worldBlockFx6CoreOtOfs, 2);
            func_80030AF4(&g_worldBlockFx6RotA, &m);
            wm_worldToView(&g_worldBlockFx6Pos, (VECTOR *)m.t, &v, 0);
            CompMatrix(g_worldViewMatrix, &m, &m);
            SetRotMatrix(&m);
            SetTransMatrix(&m);
            wm_drawModel(WM_PACK0(0x3C), 0, 2);
            func_80030AF4(&g_worldBlockFx6RotB, &m);
            wm_worldToView(&g_worldBlockFx6Pos, (VECTOR *)m.t, &v, 0);
            CompMatrix(g_worldViewMatrix, &m, &m);
            SetRotMatrix(&m);
            SetTransMatrix(&m);
            wm_drawModel(WM_PACK0(0x3D), 0, 2);
            for (i = 0; i < 10; i++) {
                MATRIX m1;
                MATRIX m2;
                VECTOR d;
                VECTOR scale;
                VECTOR s;
                u8 c;

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
                    c = ~(g_worldBlockFx6FlashTimer[i] * 7);
                    SetBackColor(c >> 1, c >> 2, c);
                    m2.t[1] = -700;
                    func_80030AF4(&g_worldBlockFx6FlashRot[i], &m1);
                    gte_CompMatrix(&m1, &m2, &m);
                    memset(&s, 0, sizeof(VECTOR));
                    s.vx = 0x2FA0;
                    s.vy = g_worldBlockFx6FlashLen[i];
                    s.vz = 0x1830;
                    scale = s;
                    ScaleMatrix(&m, &scale);
                    wm_worldToView(&g_worldBlockFx6Pos, &d, &v, 0);
                    m.t[0] += d.vx;
                    m.t[1] += d.vy;
                    m.t[2] += d.vz;
                    gte_CompMatrix(g_worldViewMatrix, &m, &m);
                    SetRotMatrix(&m);
                    SetTransMatrix(&m);
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
                VECTOR d;
                VECTOR scale;
                s32 c;

                m1 = g_worldIdentityMatrix;
                m2 = g_worldIdentityMatrix;
                c = (math_sin(g_worldFrameCount * 16 + i * 400) >> 5) + 0x80;
                if (c >= 0x100) {
                    c = 0xFF;
                }
                SetBackColor((u8)c >> 1, (u8)c >> 2, (u8)c);
                if (c == 0) {
                    g_worldBlockFx6RingDist[i] = 0x400;
                } else {
                    g_worldBlockFx6RingDist[i] += 3;
                }
                m2.t[2] = -g_worldBlockFx6RingDist[i];
                func_80030EA4(&g_worldBlockFx6RingRot[i], &m1);
                gte_CompMatrix(&m1, &m2, &m);
                scale = g_worldBlockFx6RingScale;
                ScaleMatrix(&m, &scale);
                wm_worldToView(&g_worldBlockFx6Pos, &d, &v, 0);
                m.t[0] += d.vx;
                m.t[1] += d.vy;
                m.t[2] += d.vz;
                gte_CompMatrix(g_worldViewMatrix, &m, &m);
                SetRotMatrix(&m);
                SetTransMatrix(&m);
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

void wm_updateBlockFx(s32 arg0) {
    WmEmitter *e;
    u8 on;
    s32 i;
    s32 r;
    s32 x;
    s32 y;
    s32 z;
    s32 b;
    u16 t;

    on = 0;
    switch ((u8)arg0) {
    case 2:
        g_worldClutAnimFx2 = 1;
        on = 1;
        break;
    case 5:
        on = 1;
        break;
    case 10:
        if (g_worldFormFlags[10] != 0) {
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
            t = g_worldBlockFx6RingRot[i].vy - 10;
            g_worldBlockFx6RingRot[i].vy = t - i / 4;
        }
        for (i = 0; i < 10; i++) {
            g_worldBlockFx6FlashRot[i].vx += -6 + rand() % 13;
            g_worldBlockFx6FlashRot[i].vy += -6 + rand() % 13;
        }
        break;
    }
    if (on) {
        e = w_framePackGetPtr(g_worldDataPack, (u8)arg0 + 0x28);
        for (; e->effect != -1; e++) {
            if (rand() % 4096 < e->chance) {
                x = e->x - e->rangeX / 2;
                y = e->y - e->rangeY / 2;
                z = e->z - e->rangeZ / 2;
                if (e->rangeX != 0) {
                    if (g_randPtr >= g_randTableEnd) {
                        rand_refill();
                        if (g_randPtr >= g_randTableEnd) {
                            rand_refill();
                        }
                    }
                    b = *g_randPtr++;
                    x += ((b << 8) | b) % e->rangeX;
                }
                if (e->rangeY != 0) {
                    if (g_randPtr >= g_randTableEnd) {
                        rand_refill();
                        if (g_randPtr >= g_randTableEnd) {
                            rand_refill();
                        }
                    }
                    b = *g_randPtr++;
                    y += ((b << 8) | b) % e->rangeY;
                }
                if (e->rangeZ != 0) {
                    if (g_randPtr >= g_randTableEnd) {
                        rand_refill();
                        if (g_randPtr >= g_randTableEnd) {
                            rand_refill();
                        }
                    }
                    b = *g_randPtr++;
                    z += ((b << 8) | b) % e->rangeZ;
                }
                wm_spawnEffect(x, y, z, e->effect, e->scale);
            }
        }
    }
}

void wm_termEffects(void) {
}

void wm_spawnEffect(s32 arg0, s32 arg1, s32 arg2, s32 arg3, s32 arg4) {
    WmEffect *p;
    s32 v;
    u8 k;
    WmEffect *h;

    p = g_worldEffectFree.next;
    h = &g_worldEffectActive;
    switch (arg3) {
    case 2:
    case 3:
        k = 2;
        break;
    case 16:
        k = 0xFF;
        break;
    case 4:
    case 5:
    case 6:
    case 7:
    case 8:
    case 9:
    case 10:
    case 11:
    case 12:
    case 13:
    case 14:
    case 15:
    case 17:
    case 18:
    case 19:
    case 20:
    case 21:
    case 22:
    case 23:
    case 24:
    case 25:
    case 26:
    case 27:
    case 28:
    case 29:
        k = 1;
        break;
    default:
        k = 1;
        break;
    }
    if (p->next != 0) {
        v = (s32)w_framePackGetPtr(g_worldDataPack, arg3 + 7);
        p->prev->next = p->next;
        p->next->prev = p->prev;
        p->prev = h;
        p->next = h->next;
        h->next->prev = p;
        h->next = p;
        p->abr = k;
        p->frame = 0;
        p->x = arg0;
        p->y = arg1;
        p->z = arg2;
        p->sprite = v;
        p->scale = arg4;
        p->type = arg3;
        p->phase = 0;
        p->unk24 = 0;
        if (arg3 == 0x17) {
            p->phase = rand() & 0xFFF;
        }
    }
}

void wm_freeEffect(WmEffect *arg0) {
    arg0->prev->next = arg0->next;
    arg0->next->prev = arg0->prev;
    arg0->prev = &g_worldEffectFree;
    arg0->next = g_worldEffectFree.next;
    g_worldEffectFree.next->prev = arg0;
    g_worldEffectFree.next = arg0;
}

INCLUDE_RODATA("asm/us/ovl_0c/nonmatchings/wm_effect", D_800A8644);
