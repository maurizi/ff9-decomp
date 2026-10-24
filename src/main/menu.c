#include "ff9.h"

void game_initFade(void) {
    fade_init();
}

void game_updateFade(void) {
    fade_update();
}

void game_startFade(s32 a, s32 b, s32 c, s32 d) {
    fade_start(a, b, c, d);
}

void menu_init(void) {
    menu_reset();
    func_8003264C();
}

void menu_update(void) {
    menu_dispatch();
}

void menu_open(s32 a, s32 b) {
    menu_doOpen(a, b);
}

void menu_doOpen(s32 a, s32 b) {
    GameState *st = g_gameState;

    if (!(st->unk1C->unk0 & 1) && g_menu.open == 0 && g_menu.state == 0 && (u32)a < 9) {
        g_menu.open = 1;
        g_menu.type = a;
        g_menu.arg = b;
        g_menu.unk1C = 0;
        g_menu.reading = 0;
        g_menu.fadingOut = 0;
        g_menu.drawBackground = 1;
        g_menu.fadeInOnClose = 1;
        g_menu.fadeOutOnClose = 1;
        g_menu.onClose = 0;
        g_menu.state = 1;
        if (st->mode == 1) {
            st->unk1C->unk8E4.ovl65->unk0 |= 0x1000;
        }
        g_gameState->unk1C->unk0 |= 2;
        help_init();
        menu_loadOverlay();
        menu_startFadeOut();
    }
}

void menu_onFadedOut(void) {
    GameState *g;
    s32 i;

    g_menu.fadingOut = 0;
    g = g_gameState;
    g->unk1C->unk0 |= 0x1C;
    g_menu.savedVsyncMode = g->vsyncMode;
    g->vsyncMode = 0;
    g_menu.savedR = g_gameState->unk10[0].r0;
    g_menu.savedG = g_gameState->unk10[0].g0;
    g_menu.savedB = g_gameState->unk10[0].b0;
    g_gameState->unk10[0].r0 = 0;
    g_gameState->unk10[0].g0 = 0;
    g_gameState->unk10[0].b0 = 0;
    g_gameState->unk10[1].r0 = 0;
    g_gameState->unk10[1].g0 = 0;
    g_gameState->unk10[1].b0 = 0;
    win_closeAllNow();
    g_menu.savedPadMask = g_padPortMask[0];
    g_padPortMask[0] = BTN_START;
    pad_setStickAsDpad(1);
    for (i = 0; i < 2; i++) {
        pad_resetRepeats(i);
    }
}

void menu_finish(void) {
    void (*fn)(u8);
    u8 idx;

    if (g_gameState->mode == 1) {
        g_gameState->unk1C->unk8E4.ovl65->unk0 &= ~0x1000;
    }
    g_gameState->unk1C->unk0 &= ~2;
    g_gameState->unk1C->unk0 &= ~0x1C;
    win_closeAllNow();
    g_gameState->vsyncMode = g_menu.savedVsyncMode;
    g_gameState->unk10[0].r0 = g_menu.savedR;
    g_gameState->unk10[0].g0 = g_menu.savedG;
    g_gameState->unk10[0].b0 = g_menu.savedB;
    g_gameState->unk10[1].r0 = g_menu.savedR;
    g_gameState->unk10[1].g0 = g_menu.savedG;
    g_gameState->unk10[1].b0 = g_menu.savedB;
    g_menu.state = 0;
    g_menu.open = 0;
    fn = (void (*)(u8))g_menuTypes[g_menu.type].unk8;
    idx = g_menu.type;
    if (fn != NULL) {
        fn(idx);
    }
    pad_setStickAsDpad(0);
    g_padPortMask[0] = g_menu.savedPadMask;
    if (g_menu.onClose != 0) {
        ((void (*)(void))g_menu.onClose)();
    }
    if (g_menu.fadeInOnClose != 0) {
        fade_inDefault();
    }
}

void menu_initNewGame(void) {
    config_init();
    game_initFade();
    game_initTimer();
    menu_reset();
}

void menu_initForMode(void) {
    config_init();
    game_initFade();
    help_init();
}

void menu_reset(void) {
    g_menu.state = 0;
    g_menu.open = 0;
    g_menu.unk1C = 0;
    g_menu.onClose = 0;
    g_menu.unk4 = 0;
    g_menu.unk3 = 0;
    help_init();
}

void menu_close(void) {
    if (g_menuFadeOutOnClose != 0) {
        fade_outDefault();
        fade_setOnDone(menu_finish);
    } else {
        menu_finish();
    }
}

void menu_setOnClose(void (*a)(void)) {
    g_menuOnClose = (s32)a;
}

void menu_setFadeInOnClose(u8 a) {
    g_menuFadeInOnClose = a;
}

void menu_setFadeOutOnClose(u8 a) {
    g_menuFadeOutOnClose = a;
}

void func_8003264C(void) {
    D_80077DB2 = 0;
}

void menu_dispatch(void) {
    void (*fn)(void);
    if (g_menu.open != 0) {
        fn = g_menuStateFuncs[g_menu.state];
        if (fn != NULL) {
            fn();
        }
    }
}

void menu_loadOverlay(void) {
    if (g_menuTypeOvl[g_menu.type] != 0xFF) {
        menu_readOverlay();
    } else {
        menu_readPortraits();
    }
}

void menu_readOverlay(void) {
    FileEntry *f;
    if (cd_getState() == 0) {
        g_menu.state = 2;
        g_menu.reading = 1;
        f = file_find(0, g_menuTypeOvl[g_menu.type]);
        cd_readAsync(f->unk4 + g_gameState->unk20, (f->unkC - f->unk4) << 11, (void *)0x801EDF00, menu_onOverlayRead);

    }
}

void menu_readPortraits(void) {
    s32 busy = cd_getState();
    FileEntry *f;

    if (g_menu.type == 0) {
        if (party_hasChanged() == 0) {
            g_menu.state = 2;
        } else if (busy == 0 || g_gameState->mode != 3) {
            g_menu.reading = 1;
            f = file_find(1, 30);
            if (cd_readAsync(g_gameState->unk20 + f->unk4, (f->unkC - f->unk4) << 11, (void *)0x801EDF00,
                              menu_onPortraitsRead)) {
                g_menu.state = 2;
            }
        }
    }
}

void menu_updateLoad(void) {
    if (g_gameState->mode != 3 || cd_getState() == 0) {
        menu_loadOverlay();
    }
}

void menu_updateWait(void) {
    if ((D_80077DB4 & 0xFFFF0000) == 0) {
        menu_start();
    }
}

void menu_updateRun(void) {
    void (*fn)(u8);
    u8 idx;

    pad_updateRepeat(0);
    if (g_menu.drawBackground != 0) {
        ui_drawBackground();
    }
    fn = g_menuTypes[g_menu.type].unk4;
    idx = g_menu.type;
    if (fn != NULL) {
        fn(idx);
    }
    help_update();
    win_resetClock();
    g_menu.frame++;
}

void menu_startFadeOut(void) {
    g_menu.fadingOut = 1;
    if (fade_isActive() == 0) {
        if (fade_isBusy() == 0) {
            fade_outDefault();
            fade_setOnDone(menu_onFadedOut);
        } else {
            menu_onFadedOut();
        }
    } else {
        fade_reverse(menu_onFadedOut);
    }
}

void menu_onOverlayRead(u8 *a) {
    if (*a == 0) {
        g_menuReading = 0;
    }
}

void menu_onPortraitsRead(u8 *a) {
    if (*a == 0) {
        g_menuReading = 0;
    }
}

s32 func_80032A34(void) {
    return D_80077DCC;
}

void menu_start(void) {
    void (*fn)(u8);

    g_menu.frame = 0;
    DrawSync(0);
    EnterCriticalSection();
    FlushCache();
    ExitCriticalSection();
    g_menu.state = 3;
    if (g_menu.type == 0 && party_hasChanged()) {
        ui_loadPartyPortraits((u8 *)0x801EDF00);
        party_setChanged(0);
    }
    fade_in(0x10);
    fn = g_menuTypes[g_menu.type].unk0;
    if (fn != NULL) {
        fn(g_menu.type);
    }
}
