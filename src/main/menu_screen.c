#include "ff9.h"

void menuScreen_init(void) {
    MenuScreenDef *t;

    printf("path=%s\n", "f:/code/dms/loc_us/ff9db.h");
    g_menuScreen.screen = 0;
    g_menuScreen.active = 1;
    g_menuScreen.slideIn = 1;
    g_menuScreen.frame = 0;
    g_menuScreen.savedOtz = fade_getOtz();
    g_menuScreen.member = party_getFirstMember();
    g_menuScreenSwitch.cmdRect = g_menuCmdRectInit;
    g_menuScreenSwitch.titleScroll = 0;
    g_menuScreenSwitch.busy = 0;
    g_menuScreenSwitch.loadedOverlay = 0xFF;
    g_menuScreenSwitch.titleResizing = 0;
    g_menuScreenSwitch.overlayLoading = 0;
    g_menuScreenSwitch.switching = 0;
    g_menuScreenSwitch.fadingOut = 0;
    g_menuScreenSwitch.screens = g_gameState->mode != 3 ? g_menuScreensField : g_menuScreensWorld;
    t = g_menuScreenSwitch.screens;
    t[g_menuScreenCur].init();
}

void menuScreen_drawCommands(void) {
    u8 all = 0;
    RECT *r = &g_menuScreenSwitch.cmdRect;
    s32 sel;
    s32 i;
    s32 y;

    if (g_menuScreenCur == 0 || (g_menuScreenSwitch.busy != 0 && (*(u32 *)&g_menuScreenSwitch & 0xFFFF00))) {
        all = 1;
    }
    win_setClip(NULL, 2, 2);
    if (g_menuScreenSwitch.target != 0) {
        sel = g_menuScreenSwitch.target - 1;
    } else {
        sel = g_menuScreenCur - 1;
    }
    for (i = 0; i < 7; i++) {
        y = i * 15 + 6;
        if (all || i == sel) {
            text_drawBlock(r->x + 10, r->y + y + g_menuScreenSwitch.titleScroll, 2,
                          (u8 *)text_getFrom(g_menuScreenStrings, i), 0);
        }
    }
    win_setClip(r, 2, 2);
    win_drawBox(r, 2, 2);
    win_drawShadow(r, 2, 2);
}

void menuScreen_updateOpen(void) {
    if (g_menuScreenSwitch.busy != 0) {
        if (g_menuScreenSwitch.overlayRetry != 0) {
            menuScreen_requestOverlay();
        }
        if (g_menuScreenSwitch.titleResizing != 0) {
            menuScreen_stepTitleResize();
        }
        if (g_menuScreenSwitch.titleScrolling != 0) {
            menuScreen_stepTitleScroll();
        }
        if (g_menuScreenSwitch.switching == 0 && g_menuScreenSwitch.overlayRetry == 0 && *(u16 *)&g_menuScreenSwitch.overlayLoading == 0 &&
            !(*(u32 *)&g_menuScreenSwitch & 0xFFFF00)) {
            g_menuScreenSwitch.busy = 0;
            g_menuScreen.frame = 0;
            g_menuScreen.screen = g_menuScreenSwitch.target;
            fade_inDefault();
            DrawSync(0);
            EnterCriticalSection();
            FlushCache();
            ExitCriticalSection();
            g_menuScreenSwitch.screens[g_menuScreen.screen].init();
        }
    }
}

void menuScreen_stepTitleScroll(void) {
    g_menuScreenSwitch.titleScroll += g_menuScreenSwitch.titleScrollStep;
    if (g_menuScreenSwitch.titleScrollStep >= 0) {
        if (g_menuScreenSwitch.titleScroll >= g_menuScreenSwitch.titleScrollEnd) {
            g_menuScreenSwitch.titleScroll = g_menuScreenSwitch.titleScrollEnd;
            g_menuScreenSwitch.titleScrolling = 0;
            g_menuScreenSwitch.titleScrollFrame = 0;
        }
    } else if (g_menuScreenSwitch.titleScrollEnd >= g_menuScreenSwitch.titleScroll) {
        g_menuScreenSwitch.titleScroll = g_menuScreenSwitch.titleScrollEnd;
        g_menuScreenSwitch.titleScrolling = 0;
        g_menuScreenSwitch.titleScrollFrame = 0;
    }
    g_menuScreenSwitch.titleScrollFrame++;
}

void menuScreen_update(void) {
    void (*fn)(void);
    MenuScreenDef *t;
    menuScreen_drawCommands();
    menuScreen_updateOpen();
    t = g_menuScreenSwitch.screens;
    fn = t[g_menuScreen.screen].update;
    if (fn != NULL) {
        fn();
    }
    g_menuScreen.frame++;
}

void menuScreen_exit(void) {
    help_setCallback(0);
    g_menuScreen.active = 0;
}

void menuScreen_open(s32 arg0) {
    s32 prev = g_menuScreenCur;

    if (g_menuScreenSwitch.screens[arg0].update != 0) {
        g_menuScreenSwitch.target = arg0;
        g_menuScreenSwitch.busy = 1;
        g_menuScreenSwitch.titleResizing = 1;
        g_menuScreenSwitch.fadingOut = 1;
        g_menuScreenSwitch.overlayRetry = 0;
        g_menuScreenSwitch.overlayLoading = 0;
        g_menuScreenSwitch.switching = 0;
        menuScreen_startTitleAnim();
        menuScreen_requestOverlay();
        g_menuScreenSwitch.onFaded = g_menuScreenSwitch.screens[prev].exit;
        fade_outDefault();
        fade_setOtz(3);
        fade_setOnDone(menuScreen_onOpenFaded);
    }
}

void menuScreen_switch(u8 a) {
    g_menuScreenSwitch.busy = 1;
    g_menuScreenSwitch.switching = 1;
    g_menuScreenSwitch.target = a;
    menuScreen_startTitleAnim();
    fade_out(0xC);
    fade_setOnDone(menuScreen_onSwitchFaded);
}

u8 menuScreen_getCurrent(void) {
    return g_menuScreenCur;
}

void menuScreen_close(void) {
    help_setCallback(NULL);
    fade_setOtz(g_menuScreen.savedOtz);
    menu_close();
}

u8 menuScreen_needsMember(s32 i) {
    return g_menuScreenSwitch.screens[i].needsMember;
}

s32 menuScreen_getCommandHelp(s32 a) {
    return text_getFrom(g_menuScreenStrings, a + 9);
}

void menuScreen_stepTitleResize(void) {
    if (g_menuScreenSwitch.target == 0) {
        g_menuScreenSwitch.cmdRect.h += 8;
        if (g_menuScreenSwitch.cmdRect.h >= 0x74) {
            g_menuScreenSwitch.cmdRect.h = 0x74;
            g_menuScreenSwitch.titleResizing = 0;
        }
    } else {
        g_menuScreenSwitch.cmdRect.h -= 8;
        if (g_menuScreenSwitch.cmdRect.h < 0x1B) {
            g_menuScreenSwitch.cmdRect.h = 0x1A;
            g_menuScreenSwitch.titleResizing = 0;
        }
    }
}

void menuScreen_startTitleAnim(void) {
    s32 v;
    s32 d;
    s32 q;
    s32 t = g_menuScreenSwitch.target;
    if (t != 0) {
        v = (1 - t) * 15;
    } else {
        v = 0;
    }
    d = v - g_menuScreenSwitch.titleScroll;
    if (d >= 0) {
        q = (d + 11) / 12;
    } else {
        q = (d - 11) / 12;
    }
    g_menuScreenSwitch.titleScrollStep = q;
    g_menuScreenSwitch.titleScrollEnd = v;
    g_menuScreenSwitch.titleScrollFrame = 0;
    g_menuScreenSwitch.titleScrolling = 1;
}

void menuScreen_requestOverlay(void) {
    if (g_menuScreenSwitch.target != 0) {
        menuScreen_loadOverlay();
    }
}

void menuScreen_loadOverlay(void) {
    s32 id = g_menuScreenOvl[g_menuScreenSwitch.target];
    FileEntry *f;

    if (id != 0xFF && id != g_menuScreenSwitch.loadedOverlay) {
        g_menuScreenSwitch.overlayRetry = 1;
        g_menuScreenSwitch.loadedOverlay = id;
        f = file_find(0, id);
        if (cd_readAsync(g_gameState->imgSector + f->sector, (f->unkC - f->sector) << 11, PSX_RAM(void *, 0x801EDF00), menuScreen_onOverlayRead)) {
            g_menuScreenSwitch.overlayLoading = 1;
            g_menuScreenSwitch.overlayRetry = 0;
        }
    }
}

void menuScreen_onOpenFaded(void) {
    g_menuScreenSwitch.fadingOut = 0;
    if (g_menuScreenSwitch.onFaded != NULL) {
        g_menuScreenSwitch.onFaded();
    }
}

void menuScreen_onSwitchFaded(void) {
    g_menuScreenSwitch.screens[g_menuScreen.screen].exit();
    g_menuScreenSwitch.busy = 0;
    g_menuScreen.screen = g_menuScreenSwitch.target;
    g_menuScreenSwitch.screens[g_menuScreen.screen].init();
    if (g_gameState->mode != 3) {
        menuMainField_setCursor(g_menuScreen.screen - 1);
    } else {
        menuMainWorld_setCursor(g_menuScreen.screen - 1);
    }
    fade_in(12);
    fade_setOnDone(NULL);
}

void menuScreen_onOverlayRead(u8 *a) {
    if (*a == 0) {
        g_menuScreenSwitch.overlayLoading = 0;
    }
}

INCLUDE_RODATA("asm/us/main/nonmatchings/menu_screen", g_menuScreenStrings);
