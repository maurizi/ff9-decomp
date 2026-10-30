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
            g_endingMenu.cursor = g_endingMenu.cursor != 0 ? g_endingMenu.cursor - 1 : 3;
        } else if (g_padRepeat & PAD_DOWN) {
            snd_playSe(SE_CURSOR);
            g_endingMenu.cursor = g_endingMenu.cursor < 3 ? g_endingMenu.cursor + 1 : 0;
        } else if (g_padRepeat & BTN_CROSS) {
            ending_menuDecide();
        }
    }
}

void ending_menuDraw(void) {
    ending_menuDrawStub1();
    ending_menuDrawStub2();
    ending_menuDrawStub3();
    ending_menuDrawCursor();
}

void ending_menuTick(void) {
    ending_menuStepFadeIn();
}

void ending_menuDecide(void) {
    snd_playSe(SE_CURSOR);
    fade_out(0x10);
    fade_setOnDone(ending_menuOnFadeOut);
}

void ending_menuDrawStub1(void) {
}

void ending_menuDrawStub2(void) {
}

void ending_menuDrawStub3(void) {
}

void ending_menuDrawCursor(void) {
    if (fade_isBusy() == 0 && g_endingMenu.shown != 0 && g_endingMenu.fadingIn == 0) {
        g_menuHelp.x = g_endingMenuCursorPos[g_endingMenu.cursor].x - 0x1A;
        g_menuHelp.y = g_endingMenuCursorPos[g_endingMenu.cursor].y + 3;
        g_menuHelp.otz = 3;
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
    fade_in(0x20);
}
