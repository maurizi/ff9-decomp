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
                g_worldFrmbufTransfers[n].pack = r;
                g_worldFrmbufTransfers[n].vramArea = p->vramArea;
                g_worldFrmbufTransfers[n].area = p->texId & 0x3F;
                n++;
                p->transferred = 1;
            }
        }
    }
    for (i = 0; i < n; i++) {
        id = g_worldFrmbufTransfers[i].area;
        k = g_worldFrmbufTransfers[i].vramArea - 1;
        if (g_worldMapDebugFlags & WM_DEBUG_TEXTURE) {
            printf(D_800A8118, (u8)(id + 0x60), g_worldFrmbufTransfers[i].pack, k);
        }
        px = &g_worldFrmbufAreaX[k];
        py = &g_worldFrmbufAreaY[k];
        cx = &g_worldFrmbufClutX[k];
        cy = &g_worldFrmbufClutY[k];
        wm_loadTimPack((s32 *)g_worldFrmbufTransfers[i].pack, *px, *py, *cx, *cy);
        g_worldTexAreas[id].frmTpage = getTPage(1, 1, *px, *py);
        g_worldTexAreas[id].frmClut = getClut(*cx, *cy);
    }
}

void wm_frmbufReset(void) {
}

void wm_frmbufUpdate(void) {
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
        wm_blendWeather(&g_worldWeatherData.work[1], &g_worldWeatherData.work[3], &g_worldWeatherData.work[4], g_worldWeatherBlend, g_worldWeatherBlend);
        wm_blendWeather(&g_worldWeatherData.work[2], &g_worldWeatherData.work[5], &g_worldWeatherData.work[6], g_worldWeatherBlend, g_worldWeatherBlend);
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
    wm_blendWeather(&g_worldWeatherData.work[0], &g_worldWeatherData.work[1], &g_worldWeatherData.work[2], t, t);
    wm_updateFogParams();
    wm_updateColorMatrices();
    func_8001CCB4((u8)g_worldWeatherData.work[0].fog[1].color[0], (u8)g_worldWeatherData.work[0].fog[1].color[1],
                  (u8)g_worldWeatherData.work[0].fog[1].color[2]);
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
            g_worldWeatherData.presets[3].fog[0].fogOffset = 0x7F58;
            g_worldWeatherData.presets[3].fog[0].fogOffset2 = 0x7F58;
            wm_setWeather(id, wm_getStoryFog() & 0xFF, 0x80);
            g_worldBackdropOn = 0;
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
    g_worldWeatherData.work[3] = g_worldWeatherData.work[1];
    g_worldWeatherData.work[5] = g_worldWeatherData.work[2];
    if (g_worldFog != 0) {
        switch (mode) {
        case 0:
            g_worldWeatherData.work[4] = g_worldWeatherData.presets[3];
            g_worldWeatherData.work[6] = g_worldWeatherData.presets[2];
            return;
        case 1:
            g_worldWeatherData.work[4] = g_worldWeatherData.presets[7];
            g_worldWeatherData.work[6] = g_worldWeatherData.presets[6];
            return;
        case 2:
            g_worldWeatherData.work[4] = g_worldWeatherData.presets[11];
            g_worldWeatherData.work[6] = g_worldWeatherData.presets[10];
            return;
        case 3:
            g_worldWeatherData.work[4] = g_worldWeatherData.presets[15];
            g_worldWeatherData.work[6] = g_worldWeatherData.presets[14];
            return;
        }
    } else {
        switch (mode) {
        case 0:
            g_worldWeatherData.work[4] = g_worldWeatherData.presets[1];
            g_worldWeatherData.work[6] = g_worldWeatherData.presets[0];
            return;
        case 1:
            g_worldWeatherData.work[4] = g_worldWeatherData.presets[5];
            g_worldWeatherData.work[6] = g_worldWeatherData.presets[4];
            return;
        case 2:
            g_worldWeatherData.work[4] = g_worldWeatherData.presets[9];
            g_worldWeatherData.work[6] = g_worldWeatherData.presets[8];
            return;
        case 3:
            g_worldWeatherData.work[4] = g_worldWeatherData.presets[13];
            g_worldWeatherData.work[6] = g_worldWeatherData.presets[12];
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
    arg0->lightColor[0].vx = wm_lerpShift(arg1->lightColor[0].vx, arg2->lightColor[0].vx, arg3, 0xD);
    arg0->lightColor[0].vy = wm_lerpShift(arg1->lightColor[0].vy, arg2->lightColor[0].vy, arg3, 0xD);
    arg0->lightColor[0].vz = wm_lerpShift(arg1->lightColor[0].vz, arg2->lightColor[0].vz, arg3, 0xD);
    d = &arg0->lightColor[1].vx;
    a = &arg1->lightColor[1].vx;
    b = &arg2->lightColor[1].vx;
    d[0] = wm_lerpShift(a[0], b[0], arg3, 0xD);
    d[1] = wm_lerpShift(a[1], b[1], arg3, 0xD);
    d[2] = wm_lerpShift(a[2], b[2], arg3, 0xD);
    d = &arg0->lightColor[2].vx;
    a = &arg1->lightColor[2].vx;
    b = &arg2->lightColor[2].vx;
    d[0] = wm_lerpShift(a[0], b[0], arg3, 0xD);
    d[1] = wm_lerpShift(a[1], b[1], arg3, 0xD);
    d[2] = wm_lerpShift(a[2], b[2], arg3, 0xD);
    d = &arg0->backdropLightColor.vx;
    a = &arg1->backdropLightColor.vx;
    b = &arg2->backdropLightColor.vx;
    d[0] = wm_lerpShift(a[0], b[0], arg3, 0xD);
    d[1] = wm_lerpShift(a[1], b[1], arg3, 0xD);
    d[2] = wm_lerpShift(a[2], b[2], arg3, 0xD);
    d = arg0->fxBackColor;
    a = arg1->fxBackColor;
    b = arg2->fxBackColor;
    d[0] = wm_lerpShift(a[0], b[0], arg3, 0xD);
    d[1] = wm_lerpShift(a[1], b[1], arg3, 0xD);
    d[2] = wm_lerpShift(a[2], b[2], arg3, 0xD);
    arg0->unk5C = wm_lerpShift((s16)arg1->unk5C, (s16)arg2->unk5C, arg3, 0xD);
    d = &arg0->backColor.v.vx;
    a = &arg1->backColor.v.vx;
    b = &arg2->backColor.v.vx;
    d[0] = wm_lerp(a[0] << 12, b[0] << 12, arg3) >> 12;
    d[1] = wm_lerp(a[1] << 12, b[1] << 12, arg3) >> 12;
    d[2] = wm_lerp(a[2] << 12, b[2] << 12, arg3) >> 12;
    d = &arg0->backdropBackColor.v.vx;
    a = &arg1->backdropBackColor.v.vx;
    b = &arg2->backdropBackColor.v.vx;
    d[0] = wm_lerp(a[0] << 12, b[0] << 12, arg3) >> 12;
    d[1] = wm_lerp(a[1] << 12, b[1] << 12, arg3) >> 12;
    d[2] = wm_lerp(a[2] << 12, b[2] << 12, arg3) >> 12;
    d = arg0->fog[0].color;
    a = arg1->fog[0].color;
    b = arg2->fog[0].color;
    d[0] = wm_lerp(a[0], b[0], arg3);
    d[1] = wm_lerp(a[1], b[1], arg3);
    d[2] = wm_lerp(a[2], b[2], arg3);
    d = arg0->fog[1].color;
    a = arg1->fog[1].color;
    b = arg2->fog[1].color;
    d[0] = wm_lerp(a[0], b[0], arg3);
    d[1] = wm_lerp(a[1], b[1], arg3);
    d[2] = wm_lerp(a[2], b[2], arg3);
    d = arg0->fog[2].color;
    a = arg1->fog[2].color;
    b = arg2->fog[2].color;
    d[0] = wm_lerp(a[0], b[0], arg3);
    d[1] = wm_lerp(a[1], b[1], arg3);
    d[2] = wm_lerp(a[2], b[2], arg3);
    arg0->fog[0].fogOffset = wm_lerp(arg1->fog[0].fogOffset, arg2->fog[0].fogOffset, arg3);
    arg0->fog[0].fogOffset2 = wm_lerp((u16)arg1->fog[0].fogOffset2, (u16)arg2->fog[0].fogOffset2, arg3);
    arg0->fog[1].fogOffset = wm_lerp(arg1->fog[1].fogOffset, arg2->fog[1].fogOffset, arg3);
    arg0->fog[1].fogOffset2 = wm_lerp((u16)arg1->fog[1].fogOffset2, (u16)arg2->fog[1].fogOffset2, arg3);
    arg0->fog[2].fogOffset = wm_lerp(arg1->fog[2].fogOffset, arg2->fog[2].fogOffset, arg3);
    arg0->fog[2].fogOffset2 = wm_lerp((u16)arg1->fog[2].fogOffset2, (u16)arg2->fog[2].fogOffset2, arg3);
    arg0->fog[0].fogOffset &= 0xFFFE;
    arg0->fog[0].fogOffset2 = (u16)arg0->fog[0].fogOffset2 & 0xFFFE;
    arg0->fog[1].fogOffset &= 0xFFFE;
    arg0->fog[1].fogOffset2 = (u16)arg0->fog[1].fogOffset2 & 0xFFFE;
    arg0->fog[2].fogOffset &= 0xFFFE;
    arg0->fog[2].fogOffset2 = (u16)arg0->fog[2].fogOffset2 & 0xFFFE;
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
