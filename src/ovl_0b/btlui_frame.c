#include "ovl_0b.h"

void btlui_submitOt(void) {
    RECT *tw;
    Unk80104E68 *p;

    setlen(&D_80105140[g_btluiBuf], 2);
    D_80105140[g_btluiBuf].code[0] = GPU_STP(1, 0);
    D_80105140[g_btluiBuf].code[1] = 0;
    setlen(&D_80105120[g_btluiBuf], 2);
    ((u32 *)&D_80105120[g_btluiBuf])[1] = get_mode(1, 1, 0);
    tw = &D_800FDE34;
    ((u32 *)&D_80105120[g_btluiBuf])[2] = get_tw(tw);
    SetDrawOffset(&D_801050E0[g_btluiBuf], g_btluiDispOrigin[g_drawBufs.cur ^ 1]);
    SetDrawArea(&D_80105100[g_btluiBuf], &g_gameState->dispEnv[g_drawBufs.cur ^ 1].disp);
    addPrim(&g_ot[0x17], &D_80105140[g_btluiBuf]);
    addPrim(&g_ot[0x17], &D_80105120[g_btluiBuf]);
    addPrim(&g_ot[0x17], &D_801050E0[g_btluiBuf]);
    addPrim(&g_ot[0x17], &D_80105100[g_btluiBuf]);
    addPrim(&g_ot[1], &g_btluiSavedEnv[g_btluiBuf]);
    p = (Unk80104E68 *)&g_btluiState;
    if (p->unk10 == 0) {
        s32 r = BreakDraw();
        if (r == -1) {
            p->unk24++;
        } else {
            while (IsIdleGPU(1) != 0) {
            }
            GetDrawEnv2(&g_btluiSavedEnv[g_btluiBuf]);
            ContinueDraw(&g_ot[0x17], r);
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

void btlui_clearOtLinked(u32 *arg0, u32 arg1) {
    u32 i;
    ClearOTag(arg0, 1);
    for (i = 1; i < arg1; i++) {
        arg0[i] = (u32)&arg0[i - 1] & 0xFFFFFF;
    }
}

void func_800F4DF4(void) {
}

void btl_initBanner(void) {
    RECT *r;
    s32 v;
    g_btlBanner.priority = 0;
    g_btlBanner.active = 0;
    g_btlBanner.timer = 0;
    g_btlBanner.text = 0xFF;
    v = D_800FDE3C[(g_gameState->config >> 11) & 7];
    r = &g_btlBanner.rect;
    r->x = 0x60;
    r->y = 8;
    r->w = 0x80;
    r->h = 0x15;
    g_btlBanner.duration = v;
}

void func_800F4E68(void) {
}

u8 *btl_drawBanner(u8 *arg0) {
    if (g_btlBanner.active != 0) {
        g_btlBanner.ot = (u32 *)btl_getBannerOt(g_gameState->gameData->unk8);
        arg0 = btl_buildBanner(arg0);
        if ((u32)++g_btlBanner.timer >= (u32)g_btlBanner.duration) {
            g_btlBanner.active = 0;
            g_btlBanner.priority = 0;
        }
    }
    return arg0;
}

void btl_setBanner(s32 arg0, s32 arg1) {
    if (arg1 >= g_btlBanner.priority) {
        g_btlBanner.priority = arg1;
        g_btlBanner.useRect = 0;
        btl_setBannerText(arg0);
    }
}

void btl_setBannerAt(s32 arg0, s32 arg1) {
    if (arg1 >= g_btlBanner.priority) {
        g_btlBanner.priority = arg1;
        g_btlBanner.useRect = 1;
        btl_setBannerText(arg0);
    }
}

void btl_setBannerRect(RECT *arg0) {
    g_btlBanner.rect = *arg0;
}

s32 *btl_getBannerOt(s32 arg0) {
    return &g_btlBannerOt[arg0];
}

s32 func_800F4FC8(void) {
    return D_80105274;
}

u8 func_800F4FD8(void) {
    return D_8010527C;
}

void btl_setBannerText(s32 arg0) {
    Unk801051F088 v;
    if (arg0 == 0) {
        g_btlBanner.text = 0xFF;
        g_btlBanner.active = 0;
        g_btlBanner.priority = 0;
    } else {
        text_copy(&g_btlBanner.text, (u8 *)arg0);
        g_btlBanner.active = 1;
        g_btlBanner.timer = 0;
        text_measure(&v.unk0, (s32)&g_btlBanner);
        g_btlBanner.size = v;
    }
}

/* draws the battle help text (g_btlBanner starts with the text buffer) centred in its window */
u8 *btl_buildBanner(u8 *arg0) {
    RECT *r;
    RECT *t;

    if (g_btlBanner.active != 0 && g_btlBanner.text != 0xFF) {
        if (g_btlBanner.useRect != 0) {
            r = &g_btlBanner.rect;
        } else {
            t = &D_800FDE58;
            t->h = g_btlBanner.size.unk2 + 6;
            r = t;
        }
        arg0 = btl_buildBannerFrame(text_buildBlock(r->x + ((r->w - g_btlBanner.size.unk0) >> 1), r->y + 3,
                                           g_btlBanner.ot, &g_btlBanner.text, 0, arg0),
                             r);
    }
    return arg0;
}

u8 *btl_buildBannerFrame(u8 *arg0, RECT *arg1) {
    RECT r;
    u8 *v = win_buildBorder((s32)arg1, g_btlBanner.ot, 1, 0, 1, (g_gameState->config >> 18) & 1, arg0);
    r.x = arg1->x + 2;
    r.y = arg1->y + 2;
    r.w = arg1->w - 3;
    r.h = arg1->h - 3;
    return win_buildFill((s32)&r, g_btlBanner.ot, 0, 0, (g_gameState->config >> 18) & 1, v);
}
