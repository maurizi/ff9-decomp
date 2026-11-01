#include "ovl_0b.h"

void btlui_submitOt(void) {
    RECT *tw;
    Unk80104E68 *uiState;

    setlen(&g_btluiDrStp[g_btluiBuf], PRIM_LEN(DR_STP));
    g_btluiDrStp[g_btluiBuf].code[0] = GPU_STP(1, 0);
    g_btluiDrStp[g_btluiBuf].code[1] = 0;
    setlen(&g_btluiDrMode[g_btluiBuf], PRIM_LEN(DR_MODE));
    ((u32 *)&g_btluiDrMode[g_btluiBuf])[1] = get_mode(1, 1, 0);
    tw = &g_btluiTexWindowRect;
    ((u32 *)&g_btluiDrMode[g_btluiBuf])[2] = get_tw(tw);
    SetDrawOffset(&g_btluiDrOffset[g_btluiBuf], g_btluiDispOrigin[g_drawBufs.cur ^ 1]);
    SetDrawArea(&g_btluiDrArea[g_btluiBuf], &g_gameState->dispEnv[g_drawBufs.cur ^ 1].disp);
    addPrim(&g_ot[0x17], &g_btluiDrStp[g_btluiBuf]);
    addPrim(&g_ot[0x17], &g_btluiDrMode[g_btluiBuf]);
    addPrim(&g_ot[0x17], &g_btluiDrOffset[g_btluiBuf]);
    addPrim(&g_ot[0x17], &g_btluiDrArea[g_btluiBuf]);
    addPrim(&g_ot[1], &g_btluiSavedEnv[g_btluiBuf]);
    uiState = (Unk80104E68 *)&g_btluiState;
    if (uiState->drawing == 0) {
        s32 breakAddr = BreakDraw();
        if (breakAddr == -1) {
            uiState->breakMisses++;
        } else {
            while (IsIdleGPU(1) != 0) {
            }
            GetDrawEnv2(&g_btluiSavedEnv[g_btluiBuf]);
            ContinueDraw(&g_ot[0x17], breakAddr);
        }
    }
}

void btlui_clearDfe(void) {
    DRAWENV *env = g_gameState->drawEnv;
    env[1].dfe = 0;
    env[0].dfe = 0;
}

void btlui_beginFrame(void) {
    g_btluiBuf ^= 1;
    gfx_selectBuffer(g_btluiBuf);
    btlui_clearOtLinked(g_ot, 0x18);
}

void btlui_clearOtLinked(u32 *ot, u32 count) {
    u32 i;
    ClearOTag(ot, 1);
    for (i = 1; i < count; i++) {
        ot[i] = (u32)&ot[i - 1] & 0xFFFFFF;
    }
}

void btlui_frameNop(void) {
}

void btl_initBanner(void) {
    RECT *bannerRect;
    s32 duration;
    g_btlBanner.priority = 0;
    g_btlBanner.active = 0;
    g_btlBanner.timer = 0;
    g_btlBanner.text = 0xFF;
    duration = g_btlBannerDurations[(g_gameState->config >> CFG_SHIFT_BTL_MSG_SPEED) & 7];
    bannerRect = &g_btlBanner.rect;
    bannerRect->x = 0x60;
    bannerRect->y = 8;
    bannerRect->w = 0x80;
    bannerRect->h = 0x15;
    g_btlBanner.duration = duration;
}

void btl_termBanner(void) {
}

u8 *btl_drawBanner(u8 *pkt) {
    if (g_btlBanner.active != 0) {
        g_btlBanner.ot = (u32 *)btl_getBannerOt(g_gameState->gameData->drawBuf);
        pkt = btl_buildBanner(pkt);
        if ((u32)++g_btlBanner.timer >= (u32)g_btlBanner.duration) {
            g_btlBanner.active = 0;
            g_btlBanner.priority = 0;
        }
    }
    return pkt;
}

void btl_setBanner(s32 text, s32 priority) {
    if (priority >= g_btlBanner.priority) {
        g_btlBanner.priority = priority;
        g_btlBanner.useRect = 0;
        btl_setBannerText(text);
    }
}

void btl_setBannerAt(s32 text, s32 priority) {
    if (priority >= g_btlBanner.priority) {
        g_btlBanner.priority = priority;
        g_btlBanner.useRect = 1;
        btl_setBannerText(text);
    }
}

void btl_setBannerRect(RECT *rect) {
    g_btlBanner.rect = *rect;
}

s32 *btl_getBannerOt(s32 drawBuf) {
    return &g_btlBannerOt[drawBuf];
}

s32 btl_getBannerDuration(void) {
    return g_btlBanner.duration;
}

u8 btl_isBannerActive(void) {
    return g_btlBanner.active;
}

void btl_setBannerText(s32 text) {
    if (text == 0) {
        g_btlBanner.text = 0xFF;
        g_btlBanner.active = 0;
        g_btlBanner.priority = 0;
    } else {
        text_copy(&g_btlBanner.text, (u8 *)text);
        g_btlBanner.active = 1;
        g_btlBanner.timer = 0;
        g_btlBanner.size = text_measure((u8 *)&g_btlBanner);
    }
}

/* draws the battle help text (g_btlBanner starts with the text buffer) centred in its window */
u8 *btl_buildBanner(u8 *pkt) {
    RECT *bannerRect;
    RECT *defRect;

    if (g_btlBanner.active != 0 && g_btlBanner.text != 0xFF) {
        if (g_btlBanner.useRect != 0) {
            bannerRect = &g_btlBanner.rect;
        } else {
            defRect = &g_btlBannerDefaultRect;
            defRect->h = g_btlBanner.size.h + 6;
            bannerRect = defRect;
        }
        pkt = btl_buildBannerFrame(text_buildBlock(bannerRect->x + ((bannerRect->w - g_btlBanner.size.w) >> 1), bannerRect->y + 3,
                                           g_btlBanner.ot, &g_btlBanner.text, TEXT_PAL_WHITE, pkt),
                             bannerRect);
    }
    return pkt;
}

u8 *btl_buildBannerFrame(u8 *pkt, RECT *rect) {
    RECT fillRect;
    u8 *next = win_buildBorder((s32)rect, g_btlBanner.ot, 1, 0, 1, CONFIG_WINDOW_COLOR(), pkt);
    fillRect.x = rect->x + 2;
    fillRect.y = rect->y + 2;
    fillRect.w = rect->w - 3;
    fillRect.h = rect->h - 3;
    return win_buildFill((s32)&fillRect, g_btlBanner.ot, 0, 0, CONFIG_WINDOW_COLOR(), next);
}
