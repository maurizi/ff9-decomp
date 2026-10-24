#include "ovl_14.h"

void func_800A833C(void) {
}

void opening_discEnter(void) {
    func_80032628(opening_discOnClose);
}

void func_800A8368(void) {
}

void func_800A8370(void) {
}

void opening_discOnClose(void) {
    scene_set(4);
}

void func_800A8398(void) {
}

void opening_startEnter(void) {
    g_startFrames = 0;
}

void opening_startUpdate(void) {
    if (++g_startFrames >= 2) {
        scene_set(5);
    }
}

void opening_startLeave(void) {
    if (opening_titleGetCursor() == 0) {
        opening_initNewGame();
        g_gameState->mode = 1;
        g_gameState->unk1C->fieldId = 0x46;
        g_gameState->unk1C->unk3A = 2;
    }
}

void func_800A8448(void) {
}

void opening_loadEnter(void) {
    g_loadMenuClosed = 0;
    func_80032628(opening_loadOnClose);
}

void opening_loadUpdate(void) {
    if (g_loadMenuClosed != 0) {
        opening_loadDone();
    }
}

void func_800A84A8(void) {
}

void opening_loadOnClose(void) {
    g_loadMenuClosed = 1;
    if (func_801F5330()) {
        func_80032634(0);
    }
}

void opening_loadDone(void) {
    s32 mode;
    s32 r;

    if (func_801F5330()) {
        opening_resetGameData();
        r = scene_checkDisc();
        if (r != 0xFF) {
            menu_open(8, ((r & 3) << 14) | 0x3A98);
            mode = 3;
        } else {
            func_80032634(0);
            mode = 4;
        }
    } else {
        scene_setFadeScreen(1);
        mode = 1;
    }
    scene_set(mode);
}
