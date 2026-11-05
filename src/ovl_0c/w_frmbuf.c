#include "ovl_0c.h"

const char s_wmStrFrmbufReqOver[] = "w_frmbufRequest:reqover [%d/%d]\n";
const char s_wmStrFrmbufError[] = "==== ERROR ===== / %s";
const char s_wmStrFrmbufRequestError[] = "-- w_framebufRequest Error --\n";

s32 w_frmbufRequest(s16 texId) {
    WmFrmbufSlot *slot;
    u32 i;

    if (texId & WM_TEXID_MASK) {
        slot = g_worldFrmbufHead.next;
        i = 0;
        while (1) {
            if (slot->texId == texId) {
                slot->prev->next = slot->next;
                slot->next->prev = slot->prev;
                slot->next = g_worldFrmbufHead.next;
                slot->prev = &g_worldFrmbufHead;
                g_worldFrmbufHead.next->prev = slot;
                g_worldFrmbufHead.next = slot;
                break;
            }
            if (slot->texId == -1) {
                g_worldFrmbufReqCount++;
                if (g_worldFrmbufReqCount >= 3) {
                    sprintf(g_worldMapErrorMsg, s_wmStrFrmbufReqOver, g_worldFrmbufReqCount, 2);
                    printf(s_wmStrFrmbufError, g_worldMapErrorMsg);
                    g_worldMapError = 1;
                }
                slot->texId = texId;
                slot->prev->next = slot->next;
                slot->next->prev = slot->prev;
                slot->next = g_worldFrmbufHead.next;
                slot->prev = &g_worldFrmbufHead;
                g_worldFrmbufDirty++;
                g_worldFrmbufHead.next->prev = slot;
                g_worldFrmbufHead.next = slot;
                break;
            }
            slot = slot->next;
            i++;
            if (i >= 4) {
                break;
            }
        }
        if (i == 4) {
            printf(s_wmStrFrmbufRequestError);
            return 0;
        }
    }
    return 1;
}

void wm_frmbufCompact(void) {
    WmFrmbufSlot *src;
    WmFrmbufSlot *dst;
    u32 i;

    g_worldFrmbufReqCount = 0;
    if (g_worldFrmbufDirty != 0) {
        src = g_worldFrmbufHead.next;
        i = 0;
        g_worldFrmbufDirty = 0;
        dst = src;
        do {
            src = src->next;
            i++;
        } while (i < 2);
        for (;;) {
            while ((src != &g_worldFrmbufTail) & (src->vramArea == 0)) {
                if (((u8)src->vramArea == 0) & (src->texId != -1)) {
                    src->texId = -1;
                }
                src = src->next;
            }
            if (src == &g_worldFrmbufTail) {
                break;
            }
            while (dst->vramArea != 0) {
                dst = dst->next;
            }
            dst->vramArea = src->vramArea;
            src->vramArea = 0;
            src->transferred = 0;
            src->texId = -1;
            src = src->next;
        }
    }
}

void wm_frmbufTransfer(void) {
    WmFrmbufSlot *slot;
    u32 xferCount;
    u32 i;
    s32 texPack;
    s32 id;
    s32 k;
    s16 *px;
    s16 *py;
    s16 *cx;
    s16 *cy;

    xferCount = 0;
    slot = g_worldFrmbufHead.next;
    for (i = 0; i < 2; i++, slot = slot->next) {
        if (slot->texId != -1 && slot->vramArea != 0 && slot->transferred == 0) {
            texPack = wm_readtxFind(slot->texId, 1);
            if (texPack != 0) {
                g_worldFrmbufTransfers[xferCount].pack = texPack;
                g_worldFrmbufTransfers[xferCount].vramArea = slot->vramArea;
                g_worldFrmbufTransfers[xferCount].area = slot->texId & WM_TEXID_MASK;
                xferCount++;
                slot->transferred = 1;
            }
        }
    }
    for (i = 0; i < xferCount; i++) {
        id = g_worldFrmbufTransfers[i].area;
        k = g_worldFrmbufTransfers[i].vramArea - 1;
        if (g_worldMapDebugFlags & WM_DEBUG_TEXTURE) {
            printf(s_wmStrFrmbufTransfer, (u8)(id + 0x60), g_worldFrmbufTransfers[i].pack, k);
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
    game_setClearColor((u8)g_worldWeatherData.work[0].fog[1].color[0], (u8)g_worldWeatherData.work[0].fog[1].color[1],
                  (u8)g_worldWeatherData.work[0].fog[1].color[2]);
}

void wm_weatherAuto(void) {
    s32 id = wm_getWeatherByPos();

    if (id != g_worldWeatherAutoMode) {
        g_worldWeatherAutoMode = id;
        wm_setWeather(id, wm_getStoryFog() & 0xFF, 0x80);
    }
    if (g_evtGlobalVars.b[EVT_GB_WM_FILE] == WM_FILE_DISC1 && g_worldReadyFrameCount == 10) {
        switch (WM_ATTR_AREA(g_worldActorTypeInfo[g_worldPlayer->actorType].cellAttr)) {
        case WM_AREA_DAINES_HORSE_BASIN:
        case WM_AREA_VUBE_DESERT:
        case WM_AREA_EESISTERN_COAST:
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
    s32 weather;
    s32 dist;

    dx = (g_worldPlayer->pos.vx >> 8) - 0x508;
    dz = (g_worldPlayer->pos.vz >> 8) + 0x3AD;
    weather = SquareRoot0(dx * dx + dz * dz) < 0x56; /* WM_WEATHER_SUNSET or WM_WEATHER_CLEAR */
    dx = (g_worldPlayer->pos.vx >> 8) - 0x50B;
    dz = (g_worldPlayer->pos.vz >> 8) + 0x3A0;
    if (SquareRoot0(dx * dx + dz * dz) < 0x3B) {
        weather = WM_WEATHER_DARK;
    }
    dx = (g_worldPlayer->pos.vx >> 8) - 0x1F4;
    dz = (g_worldPlayer->pos.vz >> 8) + 0x316;
    if (SquareRoot0(dx * dx + dz * dz) < 0xB4) {
        weather = WM_WEATHER_SUNSET;
    }
    if (g_worldDataSet == WM_DATASET_DISC4) {
        dx = (g_worldPlayer->pos.vx >> 8) - 0x2FE;
        dz = (g_worldPlayer->pos.vz >> 8) + 0x140;
        dist = SquareRoot0(dx * dx + dz * dz);
        g_worldWeatherPointDist = dist;
        if (dist < 0x65) {
            weather = WM_WEATHER_DARK_PURPLE;
        }
    }
    return weather;
}

void wm_setWeather(s32 mode, s32 flag, s32 speed) {
    if (mode == WM_WEATHER_KEEP) {
        mode = g_worldWeatherMode;
    }
    if (flag == WM_FOG_KEEP) {
        flag = g_worldFog;
    }
    g_worldWeatherMode = mode;
    g_worldFog = flag;
    g_worldWeatherSpeed = speed;
    g_worldWeatherBlend = 0;
    g_worldWeatherData.work[3] = g_worldWeatherData.work[1];
    g_worldWeatherData.work[5] = g_worldWeatherData.work[2];
    if (g_worldFog != 0) {
        switch (mode) {
        case WM_WEATHER_CLEAR:
            g_worldWeatherData.work[4] = g_worldWeatherData.presets[3];
            g_worldWeatherData.work[6] = g_worldWeatherData.presets[2];
            return;
        case WM_WEATHER_SUNSET:
            g_worldWeatherData.work[4] = g_worldWeatherData.presets[7];
            g_worldWeatherData.work[6] = g_worldWeatherData.presets[6];
            return;
        case WM_WEATHER_DARK:
            g_worldWeatherData.work[4] = g_worldWeatherData.presets[11];
            g_worldWeatherData.work[6] = g_worldWeatherData.presets[10];
            return;
        case WM_WEATHER_DARK_PURPLE:
            g_worldWeatherData.work[4] = g_worldWeatherData.presets[15];
            g_worldWeatherData.work[6] = g_worldWeatherData.presets[14];
            return;
        }
    } else {
        switch (mode) {
        case WM_WEATHER_CLEAR:
            g_worldWeatherData.work[4] = g_worldWeatherData.presets[1];
            g_worldWeatherData.work[6] = g_worldWeatherData.presets[0];
            return;
        case WM_WEATHER_SUNSET:
            g_worldWeatherData.work[4] = g_worldWeatherData.presets[5];
            g_worldWeatherData.work[6] = g_worldWeatherData.presets[4];
            return;
        case WM_WEATHER_DARK:
            g_worldWeatherData.work[4] = g_worldWeatherData.presets[9];
            g_worldWeatherData.work[6] = g_worldWeatherData.presets[8];
            return;
        case WM_WEATHER_DARK_PURPLE:
            g_worldWeatherData.work[4] = g_worldWeatherData.presets[13];
            g_worldWeatherData.work[6] = g_worldWeatherData.presets[12];
            return;
        }
    }
    printf(s_wmStrIllegalWeatherMode, mode);
}

/* blend the sky parameter blocks from -> to by t (0..0x1000) into out */
void wm_blendWeather(WmSky *out, WmSky *from, WmSky *to, s32 t, s32 unusedT) {
    s16 *outVals;
    s16 *fromVals;
    s16 *toVals;

    if (t < 0) {
        t = 0;
    }
    if (t > 0x1000) {
        t = 0x1000;
    }
    if (unusedT < 0) {
        unusedT = 0;
    }
    if (unusedT > 0x1000) {
        unusedT = 0x1000;
    }
    out->lightColor[0].vx = wm_lerpShift(from->lightColor[0].vx, to->lightColor[0].vx, t, 0xD);
    out->lightColor[0].vy = wm_lerpShift(from->lightColor[0].vy, to->lightColor[0].vy, t, 0xD);
    out->lightColor[0].vz = wm_lerpShift(from->lightColor[0].vz, to->lightColor[0].vz, t, 0xD);
    outVals = &out->lightColor[1].vx;
    fromVals = &from->lightColor[1].vx;
    toVals = &to->lightColor[1].vx;
    outVals[0] = wm_lerpShift(fromVals[0], toVals[0], t, 0xD);
    outVals[1] = wm_lerpShift(fromVals[1], toVals[1], t, 0xD);
    outVals[2] = wm_lerpShift(fromVals[2], toVals[2], t, 0xD);
    outVals = &out->lightColor[2].vx;
    fromVals = &from->lightColor[2].vx;
    toVals = &to->lightColor[2].vx;
    outVals[0] = wm_lerpShift(fromVals[0], toVals[0], t, 0xD);
    outVals[1] = wm_lerpShift(fromVals[1], toVals[1], t, 0xD);
    outVals[2] = wm_lerpShift(fromVals[2], toVals[2], t, 0xD);
    outVals = &out->backdropLightColor.vx;
    fromVals = &from->backdropLightColor.vx;
    toVals = &to->backdropLightColor.vx;
    outVals[0] = wm_lerpShift(fromVals[0], toVals[0], t, 0xD);
    outVals[1] = wm_lerpShift(fromVals[1], toVals[1], t, 0xD);
    outVals[2] = wm_lerpShift(fromVals[2], toVals[2], t, 0xD);
    outVals = out->fxBackColor;
    fromVals = from->fxBackColor;
    toVals = to->fxBackColor;
    outVals[0] = wm_lerpShift(fromVals[0], toVals[0], t, 0xD);
    outVals[1] = wm_lerpShift(fromVals[1], toVals[1], t, 0xD);
    outVals[2] = wm_lerpShift(fromVals[2], toVals[2], t, 0xD);
    out->shadowShade = wm_lerpShift((s16)from->shadowShade, (s16)to->shadowShade, t, 0xD);
    outVals = &out->backColor.v.vx;
    fromVals = &from->backColor.v.vx;
    toVals = &to->backColor.v.vx;
    outVals[0] = wm_lerp(fromVals[0] << 12, toVals[0] << 12, t) >> 12;
    outVals[1] = wm_lerp(fromVals[1] << 12, toVals[1] << 12, t) >> 12;
    outVals[2] = wm_lerp(fromVals[2] << 12, toVals[2] << 12, t) >> 12;
    outVals = &out->backdropBackColor.v.vx;
    fromVals = &from->backdropBackColor.v.vx;
    toVals = &to->backdropBackColor.v.vx;
    outVals[0] = wm_lerp(fromVals[0] << 12, toVals[0] << 12, t) >> 12;
    outVals[1] = wm_lerp(fromVals[1] << 12, toVals[1] << 12, t) >> 12;
    outVals[2] = wm_lerp(fromVals[2] << 12, toVals[2] << 12, t) >> 12;
    outVals = out->fog[0].color;
    fromVals = from->fog[0].color;
    toVals = to->fog[0].color;
    outVals[0] = wm_lerp(fromVals[0], toVals[0], t);
    outVals[1] = wm_lerp(fromVals[1], toVals[1], t);
    outVals[2] = wm_lerp(fromVals[2], toVals[2], t);
    outVals = out->fog[1].color;
    fromVals = from->fog[1].color;
    toVals = to->fog[1].color;
    outVals[0] = wm_lerp(fromVals[0], toVals[0], t);
    outVals[1] = wm_lerp(fromVals[1], toVals[1], t);
    outVals[2] = wm_lerp(fromVals[2], toVals[2], t);
    outVals = out->fog[2].color;
    fromVals = from->fog[2].color;
    toVals = to->fog[2].color;
    outVals[0] = wm_lerp(fromVals[0], toVals[0], t);
    outVals[1] = wm_lerp(fromVals[1], toVals[1], t);
    outVals[2] = wm_lerp(fromVals[2], toVals[2], t);
    out->fog[0].fogOffset = wm_lerp(from->fog[0].fogOffset, to->fog[0].fogOffset, t);
    out->fog[0].fogOffset2 = wm_lerp((u16)from->fog[0].fogOffset2, (u16)to->fog[0].fogOffset2, t);
    out->fog[1].fogOffset = wm_lerp(from->fog[1].fogOffset, to->fog[1].fogOffset, t);
    out->fog[1].fogOffset2 = wm_lerp((u16)from->fog[1].fogOffset2, (u16)to->fog[1].fogOffset2, t);
    out->fog[2].fogOffset = wm_lerp(from->fog[2].fogOffset, to->fog[2].fogOffset, t);
    out->fog[2].fogOffset2 = wm_lerp((u16)from->fog[2].fogOffset2, (u16)to->fog[2].fogOffset2, t);
    out->fog[0].fogOffset &= 0xFFFE;
    out->fog[0].fogOffset2 = (u16)out->fog[0].fogOffset2 & 0xFFFE;
    out->fog[1].fogOffset &= 0xFFFE;
    out->fog[1].fogOffset2 = (u16)out->fog[1].fogOffset2 & 0xFFFE;
    out->fog[2].fogOffset &= 0xFFFE;
    out->fog[2].fogOffset2 = (u16)out->fog[2].fogOffset2 & 0xFFFE;
}

void wm_weatherInit(void) {
    g_worldWeatherAuto = 1;
    g_worldWeatherMode = -2;
    g_worldWeatherAutoMode = -1;
    g_worldWeatherBlend = 0;
    g_worldWeatherSpeed = 0x80;
}

INCLUDE_RODATA("asm/us/ovl_0c/nonmatchings/w_frmbuf", s_wmStrFrmbufTransfer);

void wm_weatherStart(void) {
    g_worldWeatherAutoMode = wm_getWeatherByPos();
    g_worldFog = wm_getStoryFog();
    printf("start weather mode : %d / fog : %d\n", g_worldWeatherAutoMode, g_worldFog);
    wm_setWeather(g_worldWeatherAutoMode, g_worldFog, 0x1000);
}

s32 wm_getStoryFog(void) {
    return (u32)(*g_worldStoryCounterPtr - WM_STORY_MIST_GONE) >= WM_STORY_MIST_BACK - WM_STORY_MIST_GONE;
}

INCLUDE_RODATA("asm/us/ovl_0c/nonmatchings/w_frmbuf", s_wmStrIllegalWeatherMode);
