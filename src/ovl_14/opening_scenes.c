#include "ovl_14.h"

void opening_discInit(void) {
}

void opening_discEnter(void) {
    menu_setOnClose(opening_discOnClose);
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
        g_gameState->mode = GS_MODE_FIELD;
        g_gameState->gameData->fieldId = 0x46;
        g_gameState->gameData->fieldAreaId = 2;
    }
}

void opening_loadInit(void) {
}

void opening_loadEnter(void) {
    g_loadMenuClosed = 0;
    menu_setOnClose(opening_loadOnClose);
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
    if (menuSave_isLoaded()) {
        menu_setFadeInOnClose(0);
    }
}

void opening_loadDone(void) {
    s32 mode;
    s32 disc;

    if (menuSave_isLoaded()) {
        opening_resetGameData();
        disc = scene_checkDisc();
        if (disc != DISC_NONE) {
            menu_open(MENU_DISC, MENU_DISC_ARG(disc, DISC_NO_LOCATION));
            mode = SCENE_DISC;
        } else {
            menu_setFadeInOnClose(0);
            mode = SCENE_START;
        }
    } else {
        scene_setFadeScreen(1);
        mode = SCENE_TITLE;
    }
    scene_set(mode);
}
