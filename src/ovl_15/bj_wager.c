#include "ovl_15.h"

void bj_wagerInit(void) {
    g_bjWagerMenu.cursor = 0;
}

void bj_wagerEnter(void) {
    menu_initForMode();
    pad_setStickAsDpad(1);
    g_bjWagerMenu.unk0 = 1;
    g_bjWagerMenu.unk7 = 0;
    g_bjWagerMenu.unk6 = 0;
    g_bjWagerMenu.shown = 1;
    g_bjWagerMenu.fadingIn = 0;
    g_bjWagerMenu.fadeFrames = 0;
    g_bjWagerMenu.unkC = 0;
}

void bj_wagerUpdate(void) {
    pad_updateRepeat(0);
    bj_wagerInput();
    bj_wagerDraw();
    bj_wagerTick();
}

void bj_wagerLeave(void) {
    pad_setStickAsDpad(0);
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
    bj_wagerDrawStub1();
    bj_wagerDrawStub2();
    bj_wagerDrawCursor();
}

void bj_wagerTick(void) {
    bj_wagerStepFadeIn();
}

void bj_wagerDecide(void) {
    snd_playSe(SE_CURSOR);
    scene_set(SCENE_CHOSEN);
}

void bj_wagerDrawStub1(void) {
}

void bj_wagerDrawStub2(void) {
}

void bj_wagerDrawCursor(void) {
    if (g_bjWagerMenu.shown != 0 && g_bjWagerMenu.fadingIn == 0) {
        g_menuHelp.x = g_bjWagerCursorPos[g_bjWagerMenu.cursor].x - 0x1A;
        g_menuHelp.y = g_bjWagerCursorPos[g_bjWagerMenu.cursor].y + 3;
        g_menuHelp.otz = 3;
        help_drawCursor(&g_menuHelp, &g_bjWagerMenu);
    }
}

void bj_wagerStepFadeIn(void) {
    g_bjWagerMenu.fadeFrames++;
    if (g_bjWagerMenu.fadeFrames > SCENE_MENU_FADE_FRAMES) {
        g_bjWagerMenu.fadeFrames = 0;
        g_bjWagerMenu.fadingIn = 0;
    }
}
