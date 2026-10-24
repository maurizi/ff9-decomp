#include "ovl_0c.h"

const char D_800A80BC[] = "w_frmbufRequest:reqover [%d/%d]\n";
const char D_800A80E0[] = "==== ERROR ===== / %s";
const char D_800A80F8[] = "-- w_framebufRequest Error --\n";

s32 w_frmbufRequest(s16 arg0) {
    WmFrmbufSlot *p;
    u32 i;

    if (arg0 & 0x3F) {
        p = g_worldFrmbufHead.unk4;
        i = 0;
        while (1) {
            if (p->texId == arg0) {
                p->unk0->unk4 = p->unk4;
                p->unk4->unk0 = p->unk0;
                p->unk4 = g_worldFrmbufHead.unk4;
                p->unk0 = &g_worldFrmbufHead;
                g_worldFrmbufHead.unk4->unk0 = p;
                g_worldFrmbufHead.unk4 = p;
                break;
            }
            if (p->texId == -1) {
                g_worldFrmbufReqCount++;
                if (g_worldFrmbufReqCount >= 3) {
                    sprintf(g_worldMapErrorMsg, D_800A80BC, g_worldFrmbufReqCount, 2);
                    printf(D_800A80E0, g_worldMapErrorMsg);
                    g_worldMapError = 1;
                }
                p->texId = arg0;
                p->unk0->unk4 = p->unk4;
                p->unk4->unk0 = p->unk0;
                p->unk4 = g_worldFrmbufHead.unk4;
                p->unk0 = &g_worldFrmbufHead;
                g_worldFrmbufDirty++;
                g_worldFrmbufHead.unk4->unk0 = p;
                g_worldFrmbufHead.unk4 = p;
                break;
            }
            p = p->unk4;
            i++;
            if (i >= 4) {
                break;
            }
        }
        if (i == 4) {
            printf(D_800A80F8);
            return 0;
        }
    }
    return 1;
}

void wm_frmbufCompact(void) {
    WmFrmbufSlot *p;
    WmFrmbufSlot *q;
    u32 i;

    g_worldFrmbufReqCount = 0;
    if (g_worldFrmbufDirty != 0) {
        p = g_worldFrmbufHead.unk4;
        i = 0;
        g_worldFrmbufDirty = 0;
        q = p;
        do {
            p = p->unk4;
            i++;
        } while (i < 2);
        for (;;) {
            while ((p != &g_worldFrmbufTail) & (p->vramArea == 0)) {
                if (((u8)p->vramArea == 0) & (p->texId != -1)) {
                    p->texId = -1;
                }
                p = p->unk4;
            }
            if (p == &g_worldFrmbufTail) {
                break;
            }
            while (q->vramArea != 0) {
                q = q->unk4;
            }
            q->vramArea = p->vramArea;
            p->vramArea = 0;
            p->transferred = 0;
            p->texId = -1;
            p = p->unk4;
        }
    }
}

void wm_frmbufTransfer(void) {
    WmFrmbufSlot *p;
    u32 n;
    u32 i;
    s32 r;
    s32 id;
    s32 k;
    s16 *px;
    s16 *py;
    s16 *cx;
    s16 *cy;

    n = 0;
    p = g_worldFrmbufHead.unk4;
    for (i = 0; i < 2; i++, p = p->unk4) {
        if (p->texId != -1 && p->vramArea != 0 && p->transferred == 0) {
            r = wm_readtxFind(p->texId, 1);
            if (r != 0) {
                D_800D3F78[n].unk0 = r;
                D_800D3F78[n].unk5 = p->vramArea;
                D_800D3F78[n].unk4 = p->texId & 0x3F;
                n++;
                p->transferred = 1;
            }
        }
    }
    for (i = 0; i < n; i++) {
        id = D_800D3F78[i].unk4;
        k = D_800D3F78[i].unk5 - 1;
        if (g_worldMapDebugFlags & WM_DEBUG_TEXTURE) {
            printf(D_800A8118, (u8)(id + 0x60), D_800D3F78[i].unk0, k);
        }
        px = &D_800CDF14[k];
        py = &D_800CDF18[k];
        cx = &D_800CDF1C[k];
        cy = &D_800CDF20[k];
        wm_loadTimPack((s32 *)D_800D3F78[i].unk0, *px, *py, *cx, *cy);
        D_800D2FF8[id].unk8 = getTPage(1, 1, *px, *py);
        D_800D2FF8[id].unkA = getClut(*cx, *cy);
    }
}

void func_800C26DC(void) {
}

void func_800C26E4(void) {
}

void wm_weatherUpdate(void) {
    s32 t;

    if (g_worldWeatherAuto != 0) {
        wm_weatherAuto();
    }
    if (g_worldWeatherBlend >= 0) {
        g_worldWeatherBlend += g_worldWeatherSpeed;
        if (g_worldWeatherBlend > 0x1000) {
            g_worldWeatherBlend = 0x1000;
        }
        wm_blendWeather(&g_worldWeatherData.unk5E0[1], &g_worldWeatherData.unk5E0[3], &g_worldWeatherData.unk5E0[4], g_worldWeatherBlend, g_worldWeatherBlend);
        wm_blendWeather(&g_worldWeatherData.unk5E0[2], &g_worldWeatherData.unk5E0[5], &g_worldWeatherData.unk5E0[6], g_worldWeatherBlend, g_worldWeatherBlend);
        if (g_worldWeatherBlend == 0x1000) {
            g_worldWeatherBlend = -1;
        }
    }
    t = -g_worldCameraTarget[1];
    t -= 0x898;
    t <<= 12;
    t /= 4500;
    if (t > 0x1000) {
        t = 0x1000;
    }
    if (t < 0) {
        t = 0;
    }
    wm_blendWeather(&g_worldWeatherData.unk5E0[0], &g_worldWeatherData.unk5E0[1], &g_worldWeatherData.unk5E0[2], t, t);
    func_800B9774();
    wm_updateColorMatrices();
    func_8001CCB4((u8)g_worldWeatherData.unk5E0[0].unk30[1].unk4[0], (u8)g_worldWeatherData.unk5E0[0].unk30[1].unk4[1],
                  (u8)g_worldWeatherData.unk5E0[0].unk30[1].unk4[2]);
}

void wm_weatherAuto(void) {
    s32 id = wm_getWeatherByPos();

    if (id != g_worldWeatherAutoMode) {
        g_worldWeatherAutoMode = id;
        wm_setWeather(id, wm_getStoryFog() & 0xFF, 0x80);
    }
    if (g_evtGlobalVars.b[0x66] == 0 && g_worldReadyFrameCount == 10) {
        switch (WM_ATTR_AREA(g_worldActorTypeInfo[g_worldPlayer->unk11].cellAttr)) {
        case 9:
        case 12:
        case 13:
            g_worldWeatherData.unk0[3].unk30[0].unk0 = 0x7F58;
            g_worldWeatherData.unk0[3].unk30[0].unk2 = 0x7F58;
            wm_setWeather(id, wm_getStoryFog() & 0xFF, 0x80);
            D_800CF46D = 0;
            break;
        }
    }
}

s32 wm_getWeatherByPos(void) {
    s32 dx;
    s32 dz;
    s32 r;
    s32 d;

    dx = (g_worldPlayer->pos.vx >> 8) - 0x508;
    dz = (g_worldPlayer->pos.vz >> 8) + 0x3AD;
    r = SquareRoot0(dx * dx + dz * dz) < 0x56;
    dx = (g_worldPlayer->pos.vx >> 8) - 0x50B;
    dz = (g_worldPlayer->pos.vz >> 8) + 0x3A0;
    if (SquareRoot0(dx * dx + dz * dz) < 0x3B) {
        r = 2;
    }
    dx = (g_worldPlayer->pos.vx >> 8) - 0x1F4;
    dz = (g_worldPlayer->pos.vz >> 8) + 0x316;
    if (SquareRoot0(dx * dx + dz * dz) < 0xB4) {
        r = 1;
    }
    if (g_worldDataSet == WM_DATASET_DISC4) {
        dx = (g_worldPlayer->pos.vx >> 8) - 0x2FE;
        dz = (g_worldPlayer->pos.vz >> 8) + 0x140;
        d = SquareRoot0(dx * dx + dz * dz);
        g_worldWeatherPointDist = d;
        if (d < 0x65) {
            r = 3;
        }
    }
    return r;
}

void wm_setWeather(s32 mode, s32 flag, s32 arg2) {
    if (mode == 5) {
        mode = g_worldWeatherMode;
    }
    if (flag == 2) {
        flag = g_worldFog;
    }
    g_worldWeatherMode = mode;
    g_worldFog = flag;
    g_worldWeatherSpeed = arg2;
    g_worldWeatherBlend = 0;
    g_worldWeatherData.unk5E0[3] = g_worldWeatherData.unk5E0[1];
    g_worldWeatherData.unk5E0[5] = g_worldWeatherData.unk5E0[2];
    if (g_worldFog != 0) {
        switch (mode) {
        case 0:
            g_worldWeatherData.unk5E0[4] = g_worldWeatherData.unk0[3];
            g_worldWeatherData.unk5E0[6] = g_worldWeatherData.unk0[2];
            return;
        case 1:
            g_worldWeatherData.unk5E0[4] = g_worldWeatherData.unk0[7];
            g_worldWeatherData.unk5E0[6] = g_worldWeatherData.unk0[6];
            return;
        case 2:
            g_worldWeatherData.unk5E0[4] = g_worldWeatherData.unk0[11];
            g_worldWeatherData.unk5E0[6] = g_worldWeatherData.unk0[10];
            return;
        case 3:
            g_worldWeatherData.unk5E0[4] = g_worldWeatherData.unk0[15];
            g_worldWeatherData.unk5E0[6] = g_worldWeatherData.unk0[14];
            return;
        }
    } else {
        switch (mode) {
        case 0:
            g_worldWeatherData.unk5E0[4] = g_worldWeatherData.unk0[1];
            g_worldWeatherData.unk5E0[6] = g_worldWeatherData.unk0[0];
            return;
        case 1:
            g_worldWeatherData.unk5E0[4] = g_worldWeatherData.unk0[5];
            g_worldWeatherData.unk5E0[6] = g_worldWeatherData.unk0[4];
            return;
        case 2:
            g_worldWeatherData.unk5E0[4] = g_worldWeatherData.unk0[9];
            g_worldWeatherData.unk5E0[6] = g_worldWeatherData.unk0[8];
            return;
        case 3:
            g_worldWeatherData.unk5E0[4] = g_worldWeatherData.unk0[13];
            g_worldWeatherData.unk5E0[6] = g_worldWeatherData.unk0[12];
            return;
        }
    }
    printf(D_800A8178, mode);
}

/* blend the sky parameter blocks arg1 -> arg2 by arg3 (0..0x1000) into arg0 */
void wm_blendWeather(WmSky *arg0, WmSky *arg1, WmSky *arg2, s32 arg3, s32 arg4) {
    s16 *d;
    s16 *a;
    s16 *b;

    if (arg3 < 0) {
        arg3 = 0;
    }
    if (arg3 > 0x1000) {
        arg3 = 0x1000;
    }
    if (arg4 < 0) {
        arg4 = 0;
    }
    if (arg4 > 0x1000) {
        arg4 = 0x1000;
    }
    arg0->unk0[0].vx = wm_lerpShift(arg1->unk0[0].vx, arg2->unk0[0].vx, arg3, 0xD);
    arg0->unk0[0].vy = wm_lerpShift(arg1->unk0[0].vy, arg2->unk0[0].vy, arg3, 0xD);
    arg0->unk0[0].vz = wm_lerpShift(arg1->unk0[0].vz, arg2->unk0[0].vz, arg3, 0xD);
    d = &arg0->unk0[1].vx;
    a = &arg1->unk0[1].vx;
    b = &arg2->unk0[1].vx;
    d[0] = wm_lerpShift(a[0], b[0], arg3, 0xD);
    d[1] = wm_lerpShift(a[1], b[1], arg3, 0xD);
    d[2] = wm_lerpShift(a[2], b[2], arg3, 0xD);
    d = &arg0->unk0[2].vx;
    a = &arg1->unk0[2].vx;
    b = &arg2->unk0[2].vx;
    d[0] = wm_lerpShift(a[0], b[0], arg3, 0xD);
    d[1] = wm_lerpShift(a[1], b[1], arg3, 0xD);
    d[2] = wm_lerpShift(a[2], b[2], arg3, 0xD);
    d = &arg0->unk18.vx;
    a = &arg1->unk18.vx;
    b = &arg2->unk18.vx;
    d[0] = wm_lerpShift(a[0], b[0], arg3, 0xD);
    d[1] = wm_lerpShift(a[1], b[1], arg3, 0xD);
    d[2] = wm_lerpShift(a[2], b[2], arg3, 0xD);
    d = arg0->unk54;
    a = arg1->unk54;
    b = arg2->unk54;
    d[0] = wm_lerpShift(a[0], b[0], arg3, 0xD);
    d[1] = wm_lerpShift(a[1], b[1], arg3, 0xD);
    d[2] = wm_lerpShift(a[2], b[2], arg3, 0xD);
    arg0->unk5C = wm_lerpShift((s16)arg1->unk5C, (s16)arg2->unk5C, arg3, 0xD);
    d = &arg0->unk20.v.vx;
    a = &arg1->unk20.v.vx;
    b = &arg2->unk20.v.vx;
    d[0] = wm_lerp(a[0] << 12, b[0] << 12, arg3) >> 12;
    d[1] = wm_lerp(a[1] << 12, b[1] << 12, arg3) >> 12;
    d[2] = wm_lerp(a[2] << 12, b[2] << 12, arg3) >> 12;
    d = &arg0->unk28.v.vx;
    a = &arg1->unk28.v.vx;
    b = &arg2->unk28.v.vx;
    d[0] = wm_lerp(a[0] << 12, b[0] << 12, arg3) >> 12;
    d[1] = wm_lerp(a[1] << 12, b[1] << 12, arg3) >> 12;
    d[2] = wm_lerp(a[2] << 12, b[2] << 12, arg3) >> 12;
    d = arg0->unk30[0].unk4;
    a = arg1->unk30[0].unk4;
    b = arg2->unk30[0].unk4;
    d[0] = wm_lerp(a[0], b[0], arg3);
    d[1] = wm_lerp(a[1], b[1], arg3);
    d[2] = wm_lerp(a[2], b[2], arg3);
    d = arg0->unk30[1].unk4;
    a = arg1->unk30[1].unk4;
    b = arg2->unk30[1].unk4;
    d[0] = wm_lerp(a[0], b[0], arg3);
    d[1] = wm_lerp(a[1], b[1], arg3);
    d[2] = wm_lerp(a[2], b[2], arg3);
    d = arg0->unk30[2].unk4;
    a = arg1->unk30[2].unk4;
    b = arg2->unk30[2].unk4;
    d[0] = wm_lerp(a[0], b[0], arg3);
    d[1] = wm_lerp(a[1], b[1], arg3);
    d[2] = wm_lerp(a[2], b[2], arg3);
    arg0->unk30[0].unk0 = wm_lerp(arg1->unk30[0].unk0, arg2->unk30[0].unk0, arg3);
    arg0->unk30[0].unk2 = wm_lerp((u16)arg1->unk30[0].unk2, (u16)arg2->unk30[0].unk2, arg3);
    arg0->unk30[1].unk0 = wm_lerp(arg1->unk30[1].unk0, arg2->unk30[1].unk0, arg3);
    arg0->unk30[1].unk2 = wm_lerp((u16)arg1->unk30[1].unk2, (u16)arg2->unk30[1].unk2, arg3);
    arg0->unk30[2].unk0 = wm_lerp(arg1->unk30[2].unk0, arg2->unk30[2].unk0, arg3);
    arg0->unk30[2].unk2 = wm_lerp((u16)arg1->unk30[2].unk2, (u16)arg2->unk30[2].unk2, arg3);
    arg0->unk30[0].unk0 &= 0xFFFE;
    arg0->unk30[0].unk2 = (u16)arg0->unk30[0].unk2 & 0xFFFE;
    arg0->unk30[1].unk0 &= 0xFFFE;
    arg0->unk30[1].unk2 = (u16)arg0->unk30[1].unk2 & 0xFFFE;
    arg0->unk30[2].unk0 &= 0xFFFE;
    arg0->unk30[2].unk2 = (u16)arg0->unk30[2].unk2 & 0xFFFE;
}

void wm_weatherInit(void) {
    g_worldWeatherAuto = 1;
    g_worldWeatherMode = -2;
    g_worldWeatherAutoMode = -1;
    g_worldWeatherBlend = 0;
    g_worldWeatherSpeed = 0x80;
}

INCLUDE_RODATA("asm/us/ovl_0c/nonmatchings/w_frmbuf", D_800A8118);

void wm_weatherStart(void) {
    g_worldWeatherAutoMode = wm_getWeatherByPos();
    g_worldFog = wm_getStoryFog();
    printf("start weather mode : %d / fog : %d\n", g_worldWeatherAutoMode, g_worldFog);
    wm_setWeather(g_worldWeatherAutoMode, g_worldFog, 0x1000);
}

s32 wm_getStoryFog(void) {
    return (u32)(*g_worldStoryCounterPtr - 0x1766) >= 0x13ED;
}

INCLUDE_RODATA("asm/us/ovl_0c/nonmatchings/w_frmbuf", D_800A8178);
