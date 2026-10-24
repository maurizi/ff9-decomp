#include "ovl_0b.h"

void btlui_submitOt(void) {
    RECT *tw;
    Unk80104E68 *p;

    setlen(&D_80105140[D_801051E0], 2);
    D_80105140[D_801051E0].code[0] = 0xE6000001;
    D_80105140[D_801051E0].code[1] = 0;
    setlen(&D_80105120[D_801051E0], 2);
    ((u32 *)&D_80105120[D_801051E0])[1] = 0xE1000600;
    tw = &D_800FDE34;
    ((u32 *)&D_80105120[D_801051E0])[2] = get_tw(tw);
    SetDrawOffset(&D_801050E0[D_801051E0], g_btluiDispOrigin[g_drawBufs.unk0 ^ 1]);
    SetDrawArea(&D_80105100[D_801051E0], &g_gameState->unkC[g_drawBufs.unk0 ^ 1].disp);
    addPrim(&g_ot[0x17], &D_80105140[D_801051E0]);
    addPrim(&g_ot[0x17], &D_80105120[D_801051E0]);
    addPrim(&g_ot[0x17], &D_801050E0[D_801051E0]);
    addPrim(&g_ot[0x17], &D_80105100[D_801051E0]);
    addPrim(&g_ot[1], &D_80105160[D_801051E0]);
    p = (Unk80104E68 *)&g_btluiState;
    if (p->unk10 == 0) {
        s32 r = BreakDraw();
        if (r == -1) {
            p->unk24++;
        } else {
            while (IsIdleGPU(1) != 0) {
            }
            GetDrawEnv2(&D_80105160[D_801051E0]);
            ContinueDraw(&g_ot[0x17], r);
        }
    }
}

void func_800F4D20(void) {
    DRAWENV *env = g_gameState->unk10;
    env[1].dfe = 0;
    env[0].dfe = 0;
}

void btlui_beginFrame(void) {
    D_801051E0 ^= 1;
    gfx_selectBuffer(D_801051E0);
    func_800F4D80(g_ot, 0x18);
}

void func_800F4D80(u32 *arg0, u32 arg1) {
    u32 i;
    ClearOTag(arg0, 1);
    for (i = 1; i < arg1; i++) {
        arg0[i] = (u32)&arg0[i - 1] & 0xFFFFFF;
    }
}

void func_800F4DF4(void) {
}

void func_800F4DFC(void) {
    RECT *r;
    s32 v;
    g_btlBanner.unk90 = 0;
    g_btlBanner.unk8C = 0;
    g_btlBanner.unk80 = 0;
    g_btlBanner.unk0 = 0xFF;
    v = D_800FDE3C[(g_gameState->unk24 >> 11) & 7];
    r = &g_btlBanner.unkA4;
    r->x = 0x60;
    r->y = 8;
    r->w = 0x80;
    r->h = 0x15;
    g_btlBanner.unk84 = v;
}

void func_800F4E68(void) {
}

u8 *btl_drawBanner(u8 *arg0) {
    if (g_btlBanner.unk8C != 0) {
        g_btlBanner.unk98 = (u32 *)func_800F4FB4(g_gameState->unk1C->unk8);
        arg0 = btl_buildBanner(arg0);
        if ((u32)++g_btlBanner.unk80 >= (u32)g_btlBanner.unk84) {
            g_btlBanner.unk8C = 0;
            g_btlBanner.unk90 = 0;
        }
    }
    return arg0;
}

void btl_setBanner(s32 arg0, s32 arg1) {
    if (arg1 >= g_btlBanner.unk90) {
        g_btlBanner.unk90 = arg1;
        g_btlBanner.unk8D = 0;
        func_800F4FE8(arg0);
    }
}

void func_800F4F44(s32 arg0, s32 arg1) {
    if (arg1 >= g_btlBanner.unk90) {
        g_btlBanner.unk90 = arg1;
        g_btlBanner.unk8D = 1;
        func_800F4FE8(arg0);
    }
}

void func_800F4F84(RECT *arg0) {
    g_btlBanner.unkA4 = *arg0;
}

s32 *func_800F4FB4(s32 arg0) {
    return &D_8010528C[arg0];
}

s32 func_800F4FC8(void) {
    return D_80105274;
}

u8 func_800F4FD8(void) {
    return D_8010527C;
}

void func_800F4FE8(s32 arg0) {
    Unk801051F088 v;
    if (arg0 == 0) {
        g_btlBanner.unk0 = 0xFF;
        g_btlBanner.unk8C = 0;
        g_btlBanner.unk90 = 0;
    } else {
        func_8002BD54(&g_btlBanner.unk0, (u8 *)arg0);
        g_btlBanner.unk8C = 1;
        g_btlBanner.unk80 = 0;
        func_8002AF70(&v.unk0, (s32)&g_btlBanner);
        g_btlBanner.unk88 = v;
    }
}

/* draws the battle help text (g_btlBanner starts with the text buffer) centred in its window */
u8 *btl_buildBanner(u8 *arg0) {
    RECT *r;
    RECT *t;

    if (g_btlBanner.unk8C != 0 && g_btlBanner.unk0 != 0xFF) {
        if (g_btlBanner.unk8D != 0) {
            r = &g_btlBanner.unkA4;
        } else {
            t = &D_800FDE58;
            t->h = g_btlBanner.unk88.unk2 + 6;
            r = t;
        }
        arg0 = btl_buildBannerFrame(func_8002920C(r->x + ((r->w - g_btlBanner.unk88.unk0) >> 1), r->y + 3,
                                           g_btlBanner.unk98, &g_btlBanner.unk0, 0, arg0),
                             r);
    }
    return arg0;
}

u8 *btl_buildBannerFrame(u8 *arg0, RECT *arg1) {
    RECT r;
    u8 *v = win_buildBorder((s32)arg1, g_btlBanner.unk98, 1, 0, 1, (g_gameState->unk24 >> 18) & 1, arg0);
    r.x = arg1->x + 2;
    r.y = arg1->y + 2;
    r.w = arg1->w - 3;
    r.h = arg1->h - 3;
    return win_buildFill((s32)&r, g_btlBanner.unk98, 0, 0, (g_gameState->unk24 >> 18) & 1, v);
}
