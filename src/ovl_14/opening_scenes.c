#include "ovl_14.h"

void opening_discInit(void) {
}

void opening_discEnter(void) {
    func_80032628(opening_discOnClose);
}

void opening_discUpdate(void) {
}

void opening_discLeave(void) {
}

void opening_discOnClose(void) {
    scene_set(SCENE_START);
}

void opening_startInit(void) {
}

void opening_startEnter(void) {
    g_startFrames = 0;
}

void opening_startUpdate(void) {
    if (++g_startFrames >= 2) {
        scene_set(SCENE_END);
    }
}

void opening_startLeave(void) {
    if (opening_titleGetCursor() == TITLE_NEW_GAME) {
        opening_initNewGame();
        g_gameState->mode = 1;
        g_gameState->unk1C->fieldId = 0x46;
        g_gameState->unk1C->unk3A = 2;
    }
}

void opening_loadInit(void) {
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

void opening_loadLeave(void) {
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
        if (r != DISC_NONE) {
            menu_open(MENU_DISC, ((r & 3) << 14) | 0x3A98);
            mode = SCENE_DISC;
        } else {
            func_80032634(0);
            mode = SCENE_START;
        }
    } else {
        scene_setFadeScreen(1);
        mode = SCENE_TITLE;
    }
    scene_set(mode);
}
