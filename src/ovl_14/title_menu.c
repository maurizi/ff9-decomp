#include "ovl_14.h"

void opening_titleInit(void) {
    g_titleMenu.cursor = TITLE_CONTINUE;
    opening_titleLoadImages();
}

void opening_titleEnter(void) {
    s32 i;

    menu_initForMode();
    pad_setStickAsDpad(1);
    g_titleMenu.decided = 0;
    g_titleMenu.unk0 = 1;
    g_titleMenu.cardPolling = 0;
    g_titleMenu.cardPort = 0;
    g_titleMenu.shown = 0;
    g_titleMenu.fadingIn = 0;
    g_titleMenu.fadeFrames = 0;
    g_titleMenu.unk10 = 0;
    for (i = 0; i < 2; i++) {
        g_titleMenu.cardIn[i] = 0;
    }
    opening_titleOpenCardEvents();
    opening_titleStartFadeIn();
}

void opening_titleRefresh(void) {
    opening_titleStartFadeIn();
}

void opening_titleUpdate(void) {
    pad_updateRepeat(0);
    if (g_titleMenu.decided == 0) {
        opening_titleInput();
        opening_titleDraw();
        opening_titleTick();
    } else {
        opening_titleConfirm();
    }
}

void opening_titleLeave(void) {
    pad_setStickAsDpad(0);
    opening_titleCloseCardEvents();
    if (g_titleMenu.cursor == TITLE_CONTINUE) {
        menu_open(MENU_SAVE, 1);
    }
}

void opening_titleInput(void) {
    if (fade_isBusy() == 0) {
        if (g_titleMenu.shown != 0 && g_titleMenu.fadingIn == 0) {
            if (g_padRepeat & PAD_UP) {
                snd_playSe(SE_CURSOR);
                g_titleMenu.cursor = g_titleMenu.cursor != 0 ? g_titleMenu.cursor - 1 : TITLE_CONTINUE;
            } else if (g_padRepeat & PAD_DOWN) {
                snd_playSe(SE_CURSOR);
                g_titleMenu.cursor = g_titleMenu.cursor == 0 ? g_titleMenu.cursor + 1 : 0;
            } else if (g_padRepeat & (PAD_START | BTN_CROSS)) {
                opening_titleDecide();
            }
        }
    }
}

void opening_titleDraw(void) {
    func_800A8A8C();
    opening_titleDrawBackground();
    opening_titleDrawChoices();
    opening_titleDrawCursor();
}

void opening_titleTick(void) {
    opening_titlePollCard();
    opening_titleStepFadeIn();
}

void opening_titleLoadImages(void) {
    FileEntry *e;

    e = file_find(1, 0x3C);
    cd_readSync(g_gameState->imgSector + e->sector, (e->unkC - e->sector) << 11, TITLE_PACK_BUF, e->sector);
    opening_titleLoadTims((s32 *)TITLE_PACK_BUF);
}

void opening_titleLoadTims(s32 *pack) {
    RECT *clut;
    RECT *img;
    TmTimLike *tim;
    s32 i;
    s32 n;
    u32 mode;
    s32 sh;
    s32 u;
    s32 tp;
    s32 yb;
    s32 xb;
    s32 y2;
    s32 *p;

    n = pack[0];
    p = pack + 1;
    for (i = 0; i < n; i++) {
        tim = (TmTimLike *)((u8 *)pack + p[i]);
        file_loadTim((s32)tim);
        file_getTimRects(tim, &clut, &img);
        u = img->x & 0x3F;
        mode = tim->flags & 3;
        sh = 2 - mode;
        g_titleMenu.images[i].uvClut.s.u = u << sh;
        g_titleMenu.images[i].uvClut.s.v = img->y;
        g_titleMenu.images[i].w = img->w << sh;
        g_titleMenu.images[i].h = img->h;
        tp = mode << 7;
        yb = (s16)((s16)(img->y & 0x100) >> 4);
        xb = (img->x & 0x3FF) >> 6;
        y2 = (s16)(img->y & 0x200) << 2;
        g_titleMenu.images[i].tpage = ((i == TITLE_IMG_10) ? (tp | (yb | 0x20)) : (tp | yb)) | xb | y2;
        if (mode < 2) {
            g_titleMenu.images[i].uvClut.s.clut = (clut->y << 6) | ((clut->x >> 4) & 0x3F);
        } else {
            g_titleMenu.images[i].uvClut.s.clut = 0;
        }
        DrawSync(0);
    }
}

void opening_titleDecide(void) {
    if (g_titleMenu.cursor == TITLE_CONTINUE && opening_titleHasCard() == 0) {
        snd_playSe(SE_BUZZER);
        return;
    }
    snd_playSe(g_titleMenu.cursor == TITLE_NEW_GAME ? SE_GAME_START : SE_CURSOR);
    fade_out(0x10);
    fade_setOnDone(opening_titleOnFadeOut);
}

void func_800A8A8C(void) {
}

void opening_titleDrawBackground(void) {
    s32 i;

    for (i = 0; i < TITLE_BG_STRIPS; i++) {
        ui_drawSpriteDef(i << 5, 0, 10, &g_titleMenu.images[i], 0);
    }
    ui_drawSpriteDef(g_titleImage10Pos.x, g_titleImage10Pos.y, 5, &g_titleMenu.images[TITLE_IMG_10], 1);
    if (g_titleMenu.shown != 0) {
        if (g_titleMenu.fadingIn == 0) {
            ui_drawSpriteDef(g_titleImage13Pos.x, g_titleImage13Pos.y, 5, &g_titleMenu.images[TITLE_IMG_13], 0);
        } else {
            opening_titleDrawFading(g_titleImage13Pos.x, g_titleImage13Pos.y, 5, &g_titleMenu.images[TITLE_IMG_13], 0);
        }
    }
}

void opening_titleDrawChoices(void) {
    if (g_titleMenu.shown != 0) {
        if (g_titleMenu.fadingIn == 0) {
            ui_drawSpriteDef(g_titleNewGamePos.x, g_titleNewGamePos.y, 5, &g_titleMenu.images[TITLE_IMG_NEW_GAME], 0);
            ui_drawSpriteDef(g_titleContinuePos.x, g_titleContinuePos.y, 5, &g_titleMenu.images[TITLE_IMG_CONTINUE], opening_titleHasCard() == 0);
        } else {
            opening_titleDrawFading(g_titleNewGamePos.x, g_titleNewGamePos.y, 5, &g_titleMenu.images[TITLE_IMG_NEW_GAME], 0);
            opening_titleDrawFading(g_titleContinuePos.x, g_titleContinuePos.y, 5, &g_titleMenu.images[TITLE_IMG_CONTINUE], opening_titleHasCard() == 0);
        }
    }
}

void opening_titleDrawCursor(void) {
    if (fade_isBusy() == 0 && g_titleMenu.shown != 0 && g_titleMenu.fadingIn == 0) {
        g_menuHelp.x = g_titleCursorPos[g_titleMenu.cursor].x - 0x1A;
        g_menuHelp.y = g_titleCursorPos[g_titleMenu.cursor].y + 3;
        g_menuHelp.otz = 3;
        help_drawCursor(&g_menuHelp);
    }
}

void opening_titleStepFadeIn(void) {
    if (++g_titleMenu.fadeFrames > TITLE_FADE_FRAMES) {
        g_titleMenu.fadeFrames = 0;
        g_titleMenu.fadingIn = 0;
    }
}

void opening_titleOpenCardEvents(void) {
    s32 i;
    s32 h;

    EnterCriticalSection();
    for (i = 0; i < 4; i++) {
        h = OpenEvent(0xF4000001, g_titleCardEvents[i], 0x2000, 0);
        g_titleMenu.cardEvents[i] = h;
        EnableEvent(h);
    }
    ExitCriticalSection();
}

void opening_titleCloseCardEvents(void) {
    s32 i;

    EnterCriticalSection();
    for (i = 0; i < 4; i++) {
        DisableEvent(g_titleMenu.cardEvents[i]);
        CloseEvent(g_titleMenu.cardEvents[i]);
    }
    ExitCriticalSection();
}

void opening_titlePollCard(void) {
    s32 r;

    if (g_titleMenu.cardPolling == 0) {
        opening_titleStartCardInfo();
        g_titleMenu.cardPolling = 1;
    }
    r = opening_titleTestCardEvents();
    if (r != -1) {
        g_titleMenu.cardIn[g_titleMenu.cardPort] = (r == 0 || r == 3) ? 1 : 0;
        g_titleMenu.cardPolling = 0;
        g_titleMenu.cardPort ^= 1;
    }
}

u8 opening_titleHasCard(void) {
    s32 i;
    u8 r = 0;

    for (i = 0; i < 2; i++) {
        r |= g_titleMenu.cardIn[i];
    }
    return r;
}

void opening_titleStartCardInfo(void) {
    s32 i;

    for (i = 0; i < 4; i++) {
        TestEvent(g_titleMenu.cardEvents[i]);
    }
    _card_wait(0);
    _card_wait(1);
    _card_info(g_titleMenu.cardPort << 4);
}

s32 opening_titleTestCardEvents(void) {
    s32 i;
    s32 r = -1;

    for (i = 0; i < 4; i++) {
        if (TestEvent(g_titleMenu.cardEvents[i]) == 1) {
            r = i;
            break;
        }
    }
    return r;
}

void opening_titleOnFadeOut(void) {
    g_titleMenu.decided = 1;
}

s32 opening_titleGetCursor(void) {
    return g_titleMenu.cursor;
}

void opening_titleStartFadeIn(void) {
    if (scene_getFadeScreen() == 0) {
        g_titleMenu.shown = 1;
        g_titleMenu.fadingIn = 1;
        g_titleMenu.fadeFrames = 0;
    } else {
        g_titleMenu.shown = 1;
        g_titleMenu.fadingIn = 0;
        g_titleMenu.fadeFrames = 0;
        fade_in(0x20);
    }
}

void opening_titleDrawFading(s32 x, s32 y, s32 z, TitleImage *t, s32 arg4) {
    SprtWords *p;
    s32 c = g_titleMenu.fadeFrames;

    setlen(g_primPtr, 4);
    p = (SprtWords *)g_primPtr;
    p->rgbc = (c << 1) | (c << 9) | (c << 17) | 0x66000000;
    p->xy = x | (y << 16);
    p->uvclut = t->uvClut.w;
    p->wh = t->w | (t->h << 16);
    g_primPtr = (u8 *)(p + 1);
    addPrim(&g_ot[z], p);
    SetDrawTPage(g_primPtr, 1, 1, t->tpage | 0x20);
    addPrim(&g_ot[z], g_primPtr);
    g_primPtr += sizeof(SprtWords); /* sic: the DR_TPAGE is only 8 bytes */
}

void opening_titleConfirm(void) {
    s32 r;

    switch (g_titleMenu.cursor) {
    case TITLE_NEW_GAME:
        g_gameState->disc = DISC_1;
        r = scene_checkDisc();
        if (r != DISC_NONE) {
            menu_open(MENU_DISC, ((r & 3) << 14) | 0x3A98);
            scene_set(SCENE_DISC);
        } else {
            scene_set(SCENE_START);
        }
        break;
    case TITLE_CONTINUE:
        scene_set(SCENE_LOAD);
        menu_open(MENU_SAVE, 1);
        break;
    }
}
