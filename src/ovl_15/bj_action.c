#include "ovl_15.h"

void bj_actionInit(void) {
    g_bjAction.cursor = 0;
}

void bj_actionEnter(void) {
    menu_initForMode();
    pad_setStickAsDpad(1);
    g_bjAction.cursor = 0;
    g_bjAction.unk0 = 1;
    g_bjAction.unk7 = 0;
    g_bjAction.unk6 = 0;
    g_bjAction.shown = 1;
    g_bjAction.fadingIn = 0;
    g_bjAction.fadeFrames = 0;
    g_bjAction.unkC = 0;
}

void bj_actionUpdate(void) {
    pad_updateRepeat(0);
    bj_actionInput();
    bj_actionDraw();
    bj_actionTick();
}

void bj_actionLeave(void) {
    pad_setStickAsDpad(0);
}

s32 bj_actionGetCursor(void) {
    return g_bjAction.cursor;
}

void bj_actionInput(void) {
    if (g_bjAction.shown != 0 && g_bjAction.fadingIn == 0) {
        if (g_padRepeat & PAD_UP) {
            snd_playSe(SE_CURSOR);
            g_bjAction.cursor = g_bjAction.cursor != 0 ? g_bjAction.cursor - 1 : BJ_SPLIT;
            if (g_bjCanSplit == 0 && g_bjAction.cursor == BJ_SPLIT) {
                g_bjAction.cursor--;
            }
            if (g_bjCanDouble == 0 && g_bjAction.cursor == BJ_DOUBLE) {
                g_bjAction.cursor--;
            }
        } else if (g_padRepeat & PAD_DOWN) {
            snd_playSe(SE_CURSOR);
            g_bjAction.cursor = g_bjAction.cursor < BJ_SPLIT ? g_bjAction.cursor + 1 : BJ_STAND;
            if (g_bjCanDouble == 0 && g_bjAction.cursor == BJ_DOUBLE) {
                g_bjAction.cursor++;
            }
            if (g_bjCanSplit == 0 && g_bjAction.cursor == BJ_SPLIT) {
                g_bjAction.cursor = 0;
            }
        } else if (g_padRepeat & BTN_CROSS) {
            bj_actionDecide();
        }
    }
}

void bj_actionDraw(void) {
    bj_actionDrawStub1();
    bj_actionDrawStub2();
    bj_actionDrawCursor();
}

void bj_actionTick(void) {
    bj_actionStepFadeIn();
}

void bj_actionDecide(void) {
    snd_playSe(SE_CURSOR);
    scene_set(SCENE_CHOSEN);
}

void bj_actionDrawStub1(void) {
}

void bj_actionDrawStub2(void) {
}

void bj_actionDrawCursor(void) {
    if (g_bjAction.shown != 0 && g_bjAction.fadingIn == 0) {
        g_menuHelp.x = g_bjActionCursorPos[g_bjAction.cursor].x - 0x1A;
        g_menuHelp.y = g_bjActionCursorPos[g_bjAction.cursor].y + 3;
        g_menuHelp.otz = 3;
        help_drawCursor(&g_menuHelp, &g_bjAction);
    }
}

void bj_actionStepFadeIn(void) {
    g_bjAction.fadeFrames++;
    if (g_bjAction.fadeFrames > SCENE_MENU_FADE_FRAMES) {
        g_bjAction.fadeFrames = 0;
        g_bjAction.fadingIn = 0;
    }
}
