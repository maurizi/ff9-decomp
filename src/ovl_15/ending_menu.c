#include "ovl_15.h"

void ending_menuInit(void) {
    g_endingMenu.cursor = 0;
}

void ending_menuEnter(void) {
    menu_initForMode();
    pad_setStickAsDpad(1);
    g_endingMenu.unk0 = 1;
    g_endingMenu.unk7 = 0;
    g_endingMenu.unk6 = 0;
    g_endingMenu.shown = 0;
    g_endingMenu.fadingIn = 0;
    g_endingMenu.fadeFrames = 0;
    g_endingMenu.unkC = 0;
    ending_menuStartFadeIn();
}

void ending_menuUpdate(void) {
    pad_updateRepeat(0);
    ending_menuInput();
    ending_menuDraw();
    ending_menuTick();
}

void ending_menuLeave(void) {
    pad_setStickAsDpad(0);
}

u8 ending_menuGetCursor(void) {
    return g_endingMenu.cursor;
}

void ending_menuInput(void) {
    if (fade_isBusy() == 0 && g_endingMenu.shown != 0 && g_endingMenu.fadingIn == 0) {
        if (g_padRepeat & PAD_UP) {
            snd_playSe(SE_CURSOR);
            g_endingMenu.cursor = g_endingMenu.cursor != 0 ? g_endingMenu.cursor - 1 : ENDING_MENU_LAST;
        } else if (g_padRepeat & PAD_DOWN) {
            snd_playSe(SE_CURSOR);
            g_endingMenu.cursor = g_endingMenu.cursor < ENDING_MENU_LAST ? g_endingMenu.cursor + 1 : 0;
        } else if (g_padRepeat & BTN_CROSS) {
            ending_menuDecide();
        }
    }
}

void ending_menuDraw(void) {
    ending_menuOnDraw();
    ending_menuDrawBackground();
    ending_menuDrawChoices();
    ending_menuDrawCursor();
}

void ending_menuTick(void) {
    ending_menuStepFadeIn();
}

void ending_menuDecide(void) {
    snd_playSe(SE_CURSOR);
    fade_out(ENDING_MENU_FADE_OUT_FRAMES);
    fade_setOnDone(ending_menuOnFadeOut);
}

void ending_menuOnDraw(void) {
}

void ending_menuDrawBackground(void) {
}

void ending_menuDrawChoices(void) {
}

void ending_menuDrawCursor(void) {
    if (fade_isBusy() == 0 && g_endingMenu.shown != 0 && g_endingMenu.fadingIn == 0) {
        g_menuHelp.x = g_endingMenuCursorPos[g_endingMenu.cursor].x - ENDING_MENU_HAND_DX;
        g_menuHelp.y = g_endingMenuCursorPos[g_endingMenu.cursor].y + ENDING_MENU_HAND_DY;
        g_menuHelp.otz = ENDING_MENU_HAND_OTZ;
        help_drawCursor(&g_menuHelp, &g_endingMenu);
    }
}

void ending_menuStepFadeIn(void) {
    g_endingMenu.fadeFrames++;
    if (g_endingMenu.fadeFrames > SCENE_MENU_FADE_FRAMES) {
        g_endingMenu.fadeFrames = 0;
        g_endingMenu.fadingIn = 0;
    }
}

void ending_menuOnFadeOut(void) {
    scene_set(SCENE_CHOSEN);
}

void ending_menuStartFadeIn(void) {
    if (scene_getFadeScreen() == 0) {
        g_endingMenu.shown = 1;
        g_endingMenu.fadingIn = 1;
        g_endingMenu.fadeFrames = 0;
        return;
    }
    g_endingMenu.shown = 1;
    g_endingMenu.fadingIn = 0;
    g_endingMenu.fadeFrames = 0;
    fade_in(ENDING_MENU_FADE_IN_FRAMES);
}
