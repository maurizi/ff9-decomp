#include "ovl_15.h"

void ending_menuInit(void) {
    g_endingMenu.cursor = 0;
}

void ending_menuEnter(void) {
    func_80032574();
    func_8002C4B0(1);
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
    func_8002C308(0);
    ending_menuInput();
    ending_menuDraw();
    ending_menuTick();
}

void ending_menuLeave(void) {
    func_8002C4B0(0);
}

u8 ending_menuGetCursor(void) {
    return g_endingMenu.cursor;
}

void ending_menuInput(void) {
    if (func_8002F810() == 0 && g_endingMenu.shown != 0 && g_endingMenu.fadingIn == 0) {
        if (g_padRepeat & PAD_UP) {
            snd_playSe(0x67);
            g_endingMenu.cursor = g_endingMenu.cursor != 0 ? g_endingMenu.cursor - 1 : 3;
        } else if (g_padRepeat & PAD_DOWN) {
            snd_playSe(0x67);
            g_endingMenu.cursor = g_endingMenu.cursor < 3 ? g_endingMenu.cursor + 1 : 0;
        } else if (g_padRepeat & BTN_CROSS) {
            ending_menuDecide();
        }
    }
}

void ending_menuDraw(void) {
    func_800AD678();
    func_800AD680();
    func_800AD688();
    ending_menuDrawCursor();
}

void ending_menuTick(void) {
    ending_menuStepFadeIn();
}

void ending_menuDecide(void) {
    snd_playSe(0x67);
    func_8002FAAC(0x10);
    func_8002F8A8(ending_menuOnFadeOut);
}

void func_800AD678(void) {
}

void func_800AD680(void) {
}

void func_800AD688(void) {
}

void ending_menuDrawCursor(void) {
    if (func_8002F810() == 0 && g_endingMenu.shown != 0 && g_endingMenu.fadingIn == 0) {
        D_80077DE8.unk0 = g_endingMenuCursorPos[g_endingMenu.cursor].x - 0x1A;
        D_80077DE8.unk2 = g_endingMenuCursorPos[g_endingMenu.cursor].y + 3;
        D_80077DE8.unk4 = 3;
        func_800355E4(&D_80077DE8, &g_endingMenu);
    }
}

void ending_menuStepFadeIn(void) {
    g_endingMenu.fadeFrames++;
    if (g_endingMenu.fadeFrames > 48) {
        g_endingMenu.fadeFrames = 0;
        g_endingMenu.fadingIn = 0;
    }
}

void ending_menuOnFadeOut(void) {
    scene_set(4);
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
    func_8002FA50(0x20);
}
