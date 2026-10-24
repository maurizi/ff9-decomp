#include "ovl_15.h"

void bj_wagerInit(void) {
    g_bjWagerMenu.cursor = 0;
}

void bj_wagerEnter(void) {
    func_80032574();
    func_8002C4B0(1);
    g_bjWagerMenu.unk0 = 1;
    g_bjWagerMenu.unk7 = 0;
    g_bjWagerMenu.unk6 = 0;
    g_bjWagerMenu.shown = 1;
    g_bjWagerMenu.fadingIn = 0;
    g_bjWagerMenu.fadeFrames = 0;
    g_bjWagerMenu.unkC = 0;
}

void bj_wagerUpdate(void) {
    func_8002C308(0);
    bj_wagerInput();
    bj_wagerDraw();
    bj_wagerTick();
}

void bj_wagerLeave(void) {
    func_8002C4B0(0);
}

u8 bj_wagerGetCursor(void) {
    return g_bjWagerMenu.cursor;
}

void bj_wagerInput(void) {
    if (g_bjWagerMenu.shown != 0 && g_bjWagerMenu.fadingIn == 0) {
        if (g_padRepeat & PAD_RIGHT) {
            g_bjWager += BJ_MIN_BET;
            if (g_bjBankroll < g_bjWager) {
                g_bjWager = g_bjBankroll / BJ_MIN_BET * BJ_MIN_BET;
            }
            if (g_bjWager > BJ_MAX_BET) {
                g_bjWager = BJ_MAX_BET;
            }
            snd_playSe(SE_CURSOR);
        } else if (g_padRepeat & PAD_LEFT) {
            g_bjWager -= BJ_MIN_BET;
            if (g_bjWager < BJ_MIN_BET) {
                g_bjWager = BJ_MIN_BET;
            }
            snd_playSe(SE_CURSOR);
        } else if (g_padRepeat & BTN_CROSS) {
            bj_wagerDecide();
        }
    }
}

void bj_wagerDraw(void) {
    func_800AD380();
    func_800AD388();
    bj_wagerDrawCursor();
}

void bj_wagerTick(void) {
    bj_wagerStepFadeIn();
}

void bj_wagerDecide(void) {
    snd_playSe(SE_CURSOR);
    scene_set(SCENE_CHOSEN);
}

void func_800AD380(void) {
}

void func_800AD388(void) {
}

void bj_wagerDrawCursor(void) {
    if (g_bjWagerMenu.shown != 0 && g_bjWagerMenu.fadingIn == 0) {
        D_80077DE8.unk0 = g_bjWagerCursorPos[g_bjWagerMenu.cursor].x - 0x1A;
        D_80077DE8.unk2 = g_bjWagerCursorPos[g_bjWagerMenu.cursor].y + 3;
        D_80077DE8.unk4 = 3;
        func_800355E4(&D_80077DE8, &g_bjWagerMenu);
    }
}

void bj_wagerStepFadeIn(void) {
    g_bjWagerMenu.fadeFrames++;
    if (g_bjWagerMenu.fadeFrames > SCENE_MENU_FADE_FRAMES) {
        g_bjWagerMenu.fadeFrames = 0;
        g_bjWagerMenu.fadingIn = 0;
    }
}
