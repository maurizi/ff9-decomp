#include "ovl_36.h"

INCLUDE_RODATA("asm/us/ovl_36/nonmatchings/ovl_36", s_menuCardPointsUnit);

INCLUDE_RODATA("asm/us/ovl_36/nonmatchings/ovl_36", s_menuCardSlotClutRect);

INCLUDE_RODATA("asm/us/ovl_36/nonmatchings/ovl_36", s_menuCardSlotRect);

/* text bank for this menu (count + {offset, info} table, then strings) */
INCLUDE_RODATA("asm/us/ovl_36/nonmatchings/ovl_36", g_cardMenuText);

void menuCard_init(void) {
    s32 i;
    s32 id;

    s_menuCard.state = CARD_ST_GRID;
    s_menuCard.row = 0;
    s_menuCard.col = 0;
    s_menuCard.loadingId = 0;
    s_menuCard.kinds = card_countKinds();
    s_menuCard.unk6 = CARD_NONE;
    text_setBank(g_cardMenuText);
    menuCard_loadImages();
    menuCard_initImageCache();
    help_setCallback(s_menuCard.kinds ? menuCard_updateHelp : 0);
    setRECT(&s_menuCard.gridRect, 8, 0x26, 0x9E, 0xB6);
    setRECT(&s_menuCard.infoRect, 0xAA, 0x26, 0x8A, 0x4E);
    setRECT(&s_menuCard.cardInfoRect, 0xAA, 0x76, 0x8A, 0x66);
    for (i = 0; i < CARD_COUNT; i++) {
        s_menuCard.shown[i] = 0;
    }
    menuCard_initFlip();
    while (cd_getState()) {
    }
    s_menuCard.imageFile = file_find(DIR_COMMON, FILE_CARD_IMAGES);
    for (i = 0; i < 4; i++) {
        id = s_menuCardRepeatIds[i];
        s_menuCard.saved[i].repeatDelay = pad_getRepeatDelay(0, id);
        s_menuCard.saved[i].repeatInterval = pad_getRepeatInterval(0, id);
        pad_setRepeatDelay(0, id, 0x10);
        pad_setRepeatInterval(0, id, 10);
    }
    menuCard_refresh();
    id = s_menuCard.col * CARD_GRID_ROWS + s_menuCard.row;
    if (s_menuCard.count[id]) {
        menuCard_selectCard(id);
    }
}

void menuCard_inputGrid(void) {
    s32 cur;
    s32 next;
    s32 dir;

    cur = s_menuCard.col * CARD_GRID_ROWS + s_menuCard.row;
    if (g_padRepeat & BTN_CIRCLE) {
        snd_playSe(SE_CANCEL);
        menuScreen_open(0);
        return;
    }
    if (g_padRepeat & BTN_CROSS) {
        menuCard_openDiscard();
        return;
    }
    if (g_padRepeat & (PAD_UP | PAD_RIGHT | PAD_DOWN | PAD_LEFT)) {
        if ((g_padRepeat & PAD_UP) && s_menuCard.row != 0) {
            s_menuCard.row--;
        }
        if ((g_padRepeat & PAD_DOWN) && s_menuCard.row < CARD_GRID_ROWS - 1) {
            s_menuCard.row++;
        }
        if ((g_padRepeat & PAD_LEFT) && s_menuCard.col != 0) {
            s_menuCard.col--;
        }
        if ((g_padRepeat & PAD_RIGHT) && s_menuCard.col < CARD_GRID_COLS - 1) {
            s_menuCard.col++;
        }
        next = s_menuCard.col * CARD_GRID_ROWS + s_menuCard.row;
        if (next != cur) {
            snd_playSe(SE_CURSOR);
            menuCard_selectCard(next);
        }
    } else if (g_padRepeat & BTN_L1) {
        dir = CARD_FLIP_PREV;
        if (s_menuCard.count[cur] >= 2) {
            menuCard_flipCopy(cur, dir);
            snd_playSe(SE_PAGE);
        }
    } else if (g_padRepeat & BTN_R1) {
        dir = CARD_FLIP_NEXT;
        if (s_menuCard.count[cur] >= 2) {
            menuCard_flipCopy(cur, dir);
            snd_playSe(SE_PAGE);
        }
    }
}

void menuCard_openDiscard(void) {
    if (s_menuCard.count[s_menuCard.col * CARD_GRID_ROWS + s_menuCard.row] != 0) {
        snd_playSe(SE_CURSOR);
        s_menuCard.state = CARD_ST_NONE;
        s_menuCard.discardCursor = CARD_DISCARD_NO;
        s_menuCard.discardWin = win_openFree(&s_menuCardDiscardRect, 10, WIN_STYLE_BEVEL);
        win_setDrawContent(s_menuCard.discardWin, menuCard_drawDiscardWindow);
        win_setOnOpened(s_menuCard.discardWin, menuCard_onDiscardOpened);
        win_setOnClosed(s_menuCard.discardWin, menuCard_onDiscardClosed);
        win_setTitle(s_menuCard.discardWin, g_labelTrash);
        win_setShadow(s_menuCard.discardWin, 1);
        return;
    }
    snd_playSe(SE_BUZZER);
}

void menuCard_drawGrid(void) {
    u8 numText[8];
    RECT *rect;
    s32 total;
    s32 kinds;
    s32 cardId;
    s32 i;
    s32 j;
    s32 x;
    s32 y;
    u8 *label;
    s32 w;
    s32 k = 8;
    s32 k2 = 0x80;

    rect = &s_menuCard.gridRect;
    total = card_countAll();
    kinds = card_countKinds();
    ui_drawLabel(rect->x + 8, rect->y - 3, 0x1E, g_labelCard, UI_LABEL_NORMAL);
    win_drawBox(rect, 0x1E, WIN_STYLE_LINE_RULE);
    win_drawShadow(rect, 0x1E, WIN_STYLE_LINE_RULE);
    label = text_get(CARD_TEXT_STOCK);
    w = text_measureWidth(label);
    text_formatNumber(total, numText, 3, TEXT_NUM_ASCII);
    text_drawBlock(rect->x + 8, rect->y + 0xA2, 0x14, label, TEXT_PAL_WHITE);
    ui_drawDigits(rect->x + (w + k), rect->y + 0xA4, 0x14, numText, DIGITS_WHITE);
    label = text_get(CARD_TEXT_TYPE);
    w = text_measureWidth(label);
    text_formatNumber(kinds, numText, 3, TEXT_NUM_ASCII);
    ui_drawDigits(rect->x + 0x80, rect->y + 0xA4, 0x14, numText, DIGITS_WHITE);
    text_drawBlock(rect->x - (w - k2), rect->y + 0xA2, 0x14, label, TEXT_PAL_WHITE);
    for (cardId = 0, i = 0, x = 4; i < CARD_GRID_COLS; i++, x += 15) {
        for (j = 0, y = 10; j < CARD_GRID_ROWS; j++, y += 15) {
            menuCard_drawCell(rect->x + x, rect->y + y, 0x14, cardId);
            cardId++;
        }
    }
}

void menuCard_drawInfo(void) {
    u8 numText[8];
    RECT *rect;
    s32 winCount;
    s32 lossCount;
    s32 drawCount;
    s32 rank;
    s32 w;

    rect = &s_menuCard.infoRect;
    if (card_getWins() <= 99999) {
        winCount = card_getWins();
    } else {
        winCount = 99999;
    }
    if (card_getLosses() <= 99999) {
        lossCount = card_getLosses();
    } else {
        lossCount = 99999;
    }
    if (card_getDraws() <= 99999) {
        drawCount = card_getDraws();
    } else {
        drawCount = 99999;
    }
    rank = s_menuCard.rank;
    ui_drawLabel(rect->x + 8, rect->y - 3, 0x1E, g_labelInfo, UI_LABEL_NORMAL);
    win_drawBox(rect, 0x1E, WIN_STYLE_BEVEL);
    win_drawShadow(rect, 0x1F, WIN_STYLE_BEVEL);
    ui_drawEtchedLine(rect->x + 4, rect->y + 0x24, 0x14, rect->w - 9);
    text_formatNumber(s_menuCard.points[CARD_PTS_TOTAL], numText, 4, TEXT_NUM_ASCII);
    text_drawBlock(rect->x + 0xA, rect->y + 5, 0x14, text_get(CARD_TEXT_COLLECTORS), TEXT_PAL_WHITE);
    text_drawBlock(rect->x + 0xA, rect->y + 0x14, 0x14, text_get(rank), TEXT_PAL_WHITE);
    text_drawBlock(rect->x + 0xA, rect->y + 0x26, 0x14, text_get(CARD_TEXT_WIN), TEXT_PAL_WHITE);
    text_drawBlock(rect->x + 0xA, rect->y + 0x30, 0x14, text_get(CARD_TEXT_LOSE), TEXT_PAL_WHITE);
    text_drawBlock(rect->x + 0xA, rect->y + 0x3A, 0x14, text_get(CARD_TEXT_DRAW), TEXT_PAL_WHITE);
    ui_drawSmallDigits(rect->x + 0x5A, rect->y + 9, 0x14, numText, TEXT_PAL_WHITE);
    ui_drawSmallDigits(rect->x + 0x78, rect->y + 0xB, 0x14, s_menuCardPointsUnit, TEXT_PAL_WHITE);
    w = menuCard_drawInfo_width(text_get(CARD_TEXT_DRAW));
    text_formatNumber(winCount, numText, 5, TEXT_NUM_ASCII);
    w += 0xB;
    ui_drawSmallDigits(rect->x + w, rect->y + 0x2A, 0x14, numText, TEXT_PAL_WHITE);
    text_formatNumber(lossCount, numText, 5, TEXT_NUM_ASCII);
    ui_drawSmallDigits(rect->x + w, rect->y + 0x34, 0x14, numText, TEXT_PAL_WHITE);
    text_formatNumber(drawCount, numText, 5, TEXT_NUM_ASCII);
    ui_drawSmallDigits(rect->x + w, rect->y + 0x3E, 0x14, numText, TEXT_PAL_WHITE);
}

void menuCard_drawCardStack(s32 id) {
    u8 numText[8];
    RECT *rect;
    CardFlip *turn;
    CardCache *cache;
    u8 *name;
    u8 same;
    s32 i;
    s32 dx;
    s32 x;
    s32 y;
    s32 dy;
    s32 prevBtn;
    s32 nextBtn;
    s32 stackDepth;
    s32 cnt;

    rect = &s_menuCard.cardInfoRect;
    stackDepth = s_menuCard.count[id];
    cnt = stackDepth;
    if (stackDepth >= 6) {
        stackDepth = 5;
    }
    turn = &s_menuCard.flip;
    cache = &s_menuCard.cache;
    if (cnt != 0) {
        same = id == cache->shown;
        name = (u8 *)card_getName(id);
        if (cnt >= 2) {
            text_formatNumber(s_menuCard.shown[id] + 1, numText, 3, TEXT_NUM_ASCII);
            text_formatNumber(cnt, numText + 4, TEXT_NUM_NO_PAD, TEXT_NUM_ASCII);
            numText[3] = '/';
            ui_drawSmallDigits(rect->x + 0x2B, rect->y + 0x43, 0x14, numText, TEXT_PAL_WHITE);
            prevBtn = text_getButtonSprite(4);
            nextBtn = text_getButtonSprite(5);
            ui_drawSprite(rect->x + 0x1E, rect->y + 0x41, 0x14, prevBtn, 0);
            ui_drawSprite(rect->x + 0x5E, rect->y + 0x41, 0x14, nextBtn, 0);
        }
        i = 0;
        menuCard_drawSprite(rect->x + 0x10, rect->y + 0x50, 0x14, CARD_SPR_NAME_ICON, GPU_RGBC(GPU_SPRT, GPU_RGB_NEUTRAL));
        text_drawBlock(rect->x + 0x20, rect->y + 0x50, 0x14, name, TEXT_PAL_WHITE);

        while (i < stackDepth) {
            dx = i * 4 + 0x2E;
            x = rect->x + dx;
            dy = i + 9;
            y = rect->y + dy;
            if (i != 0 && turn->active != 0) {
                if (turn->dir == 0) {
                    x += turn->ticks - 4;
                } else {
                    x -= turn->ticks;
                }
            }
            menuCard_drawCard(x, y, 0x14, id, i, same);
            i++;

        }
    }
    win_drawBox(rect, 0x1E, WIN_STYLE_BEVEL);
    win_drawShadow(rect, 0x1F, WIN_STYLE_BEVEL);
}

void menuCard_drawCellHighlight(void) {
    RECT *rect = &s_menuCard.gridRect;
    s32 pulse;
    s32 x, y;
    u32 rgbc;

    pulse = g_menuFrame & 0x3F;
    if (pulse >= 0x20) {
        pulse = 0x3F - pulse;
    }
    x = rect->x + (s16)(s_menuCard.col * 15 + 4);
    y = rect->y + (s16)(s_menuCard.row * 15 + 10);
    setlen(g_primPtr, 5);
    rgbc = (pulse * 8) | 0x48000000;
    ((PolyF4Words *)g_primPtr)->rgbc = rgbc;
    ((PolyF4Words *)g_primPtr)->xy0 = x | (y << 16);
    ((PolyF4Words *)g_primPtr)->xy1 = (x + 13) | (y << 16);
    ((PolyF4Words *)g_primPtr)->xy2 = (x + 13) | ((y + 13) << 16);
    ((PolyF4Words *)g_primPtr)->xy3 = 0x55555555;
    addPrim(&g_ot[18], g_primPtr);
    g_primPtr += 0x18;
    setlen(g_primPtr, 5);
    ((PolyF4Words *)g_primPtr)->rgbc = rgbc;
    ((PolyF4Words *)g_primPtr)->xy0 = x | (y << 16);
    ((PolyF4Words *)g_primPtr)->xy1 = x | ((y + 13) << 16);
    ((PolyF4Words *)g_primPtr)->xy2 = (x + 13) | ((y + 13) << 16);
    ((PolyF4Words *)g_primPtr)->xy3 = 0x55555555;
    addPrim(&g_ot[18], g_primPtr);
    g_primPtr += 0x18;
}

void menuCard_drawCursor(void) {
    u8 blink = (g_menuFrame & 3) != 0;

    if (!fade_isBusy()) {
        if (s_menuCard.state == CARD_ST_GRID || (s_menuCard.state == CARD_ST_DISCARD && blink)) {
            g_menuHelp.otz = 0x11;
            g_menuHelp.x = s_menuCard.gridRect.x + s_menuCard.col * 15 - 20;
            g_menuHelp.y = s_menuCard.gridRect.y + s_menuCard.row * 15 + 14;
            help_drawCursor();
        }
        if (s_menuCard.state == CARD_ST_DISCARD) {
            Window *w = s_menuCard.discardWin;
            g_menuHelp.x = w->x + 0x16;
            g_menuHelp.y = w->y + (s16)(s_menuCard.discardCursor * 15 + 0x32);
            g_menuHelp.otz = 9;
            help_drawCursor();
        }
    }
}

void menuCard_loadCardImage(void) {
    RECT clutRect, pixRect;
    CardCache *cc;
    CardSlot *slot;
    s32 *ofs;
    s32 clutLen;
    u32 k;

    cc = &s_menuCard.cache;
    ofs = s_menuCardImageOffsets;
    if (cc->used >= CARD_CACHE_SLOTS) {
        cc->slots[cc->oldest].used = 0;
        cc->used++;
        cc->oldest = (cc->oldest + 1) % CARD_CACHE_SLOTS;
    }
    slot = &cc->slots[cc->next];
    {
        u8 id = s_menuCard.loadingId;
        slot->used = 1;
        slot->id = id;
    }
    cc->used++;
    cc->next = (cc->next + 1) % CARD_CACHE_SLOTS;
    clutLen = menuCard_decodeRle(s_menuCard.sectorBuf, s_menuCard.clutBuf);
    if (ofs[s_menuCard.loadingId + 1] != ofs[s_menuCard.loadingId]) {
        menuCard_decodeRle((u8 *)s_menuCardImageOffsets - 4 + ofs[s_menuCard.loadingId], s_menuCard.clutBuf + clutLen);
    }
    k = slot->vram;
    clutRect.x = 0;
    clutRect.w = 0x100;
    clutRect.h = 1;
    pixRect.w = 0x13;
    pixRect.h = 0x2F;
    clutRect.y = k + 0x1F4;
    pixRect.x = (u8)(k % 3) * 0x13;
    pixRect.y = (u8)(k / 3) * 0x2F + 0x100;
    LoadImage(&clutRect, (u32 *)s_menuCard.clutBuf);
    LoadImage(&pixRect, (u32 *)s_menuCard.pixelBuf);
    DrawSync(0);
    if (slot->id == cc->cur) {
        menuCard_showCachedImage_inline(slot);
    }
}

void menuCard_updateImageLoad(void) {
    CardCache *imgCache = &s_menuCard.cache;

    if (imgCache->reading != 0) {
        if (cd_getState() != 0) {
            return;
        }
        imgCache->reading = 0;
        menuCard_loadCardImage();
        menuCard_updateImageLoad_read(imgCache);
    } else {
        menuCard_updateImageLoad_read(imgCache);
    }
}

void menuCard_drawFace(s32 x, s32 y, s32 z, s32 dim) {
    SprtWords *sprt;
    u32 clut = (GetClut(0, 0x1F2) << 16) | 0xB000;
    s32 shade;
    u32 rgbc;

    setlen(g_primPtr, 4);
    shade = 0x80 - dim * 0x16;
    rgbc = shade | (shade << 8) | (shade << 16) | GPU_CODE(GPU_SPRT);
    sprt = (SprtWords *)g_primPtr;
    sprt->rgbc = rgbc;
    sprt->wh = 0x2F0026;
    sprt->xy = (x + 2) | ((y + 2) << 16);
    sprt->uvclut = clut;
    g_primPtr = (u8 *)(sprt + 1);
    addPrim(&g_ot[z], sprt);
    SetDrawTPage(g_primPtr, 1, 1, 0xBF);
    addPrim(&g_ot[z], g_primPtr);
    g_primPtr += 8;
    menuCard_drawSprite(x, y, 0x14, CARD_SPR_FRAME, rgbc);
}

void menuCard_refresh(void) {
    s32 i;
    u8 owned;
    CardMenu *menu;

    help_setCallback(card_countAll() ? menuCard_updateHelp : NULL);
    for (i = 0; i < CARD_COUNT; i++) {
        owned = card_countOf(i);
        s_menuCard.count[i] = owned;
        if (s_menuCard.shown[i] >= owned) {
            s_menuCard.shown[i] = owned - 1;
        }
    }
    menuCard_calcCollectorPoints(s_menuCard.points);
    menu = &s_menuCard;
    menu->rank = CARD_RANK_COUNT;
    for (i = 1; i < CARD_RANK_COUNT; i++) {
        if (menu->points[CARD_PTS_TOTAL] < s_menuCardRankPoints[i]) {
            menu->rank = i - 1;
            return;
        }
    }
}

void menuCard_drawSprite(s32 x, s32 y, s32 z, s32 idx, s32 rgbc) {
    SprtWords *sprt;
    Ovl36Sprite *sprDef;

    setlen(g_primPtr, 4);
    sprt = (SprtWords *)g_primPtr;
    sprDef = &s_menuCardSprites[idx];
    sprt->rgbc = rgbc;
    sprt->xy = x | (y << 16);
    sprt->uvclut = sprDef->uvclut;
    sprt->wh = sprDef->w | (sprDef->h << 16);
    g_primPtr = (u8 *)(sprt + 1);
    addPrim(&g_ot[z], sprt);
    SetDrawTPage(g_primPtr, 1, 1, sprDef->tpage | 0x20);
    addPrim(&g_ot[z], g_primPtr);
    g_primPtr += 8;
}

void menuCard_saveVram(u8 *backup) {
    RECT clutRect = s_menuCardSlotClutRect;
    RECT pixRect = s_menuCardSlotRect;
    s32 i;

    StoreImage(&s_menuCardSaveClutRect0, (u32 *)backup);
    backup += 0x200;
    StoreImage(&s_menuCardSaveClutRect1, (u32 *)backup);
    backup += 0x200;
    StoreImage(&s_menuCardSaveRect, (u32 *)backup);
    backup += 0x200;
    DrawSync(0);
    for (i = 0; i < CARD_CACHE_SLOTS; i++) {
        clutRect.y = i + 0x1F4;
        pixRect.x = (i % 3) * 0x13;
        pixRect.y = (i / 3) * 0x2F + 0x100;
        StoreImage(&clutRect, (u32 *)backup);
        DrawSync(0);
        StoreImage(&pixRect, (u32 *)(backup + 0x200));
        DrawSync(0);
        backup += 0x8FC;
    }
}

void menuCard_restoreVram(u8 *backup) {
    RECT clutRect = s_menuCardSlotClutRect;
    RECT pixRect = s_menuCardSlotRect;
    s32 i;

    LoadImage(&s_menuCardRestoreClutRect0, (u32 *)backup);
    backup += 0x200;
    LoadImage(&s_menuCardRestoreClutRect1, (u32 *)backup);
    backup += 0x200;
    DrawSync(0);
    LoadImage(&s_menuCardRestoreRect, (u32 *)backup);
    backup += 0x200;
    DrawSync(0);
    for (i = 0; i < CARD_CACHE_SLOTS; i++) {
        clutRect.y = i + 0x1F4;
        pixRect.x = (i % 3) * 0x13;
        pixRect.y = (i / 3) * 0x2F + 0x100;
        LoadImage(&clutRect, (u32 *)backup);
        DrawSync(0);
        LoadImage(&pixRect, (u32 *)(backup + 0x200));
        DrawSync(0);
        backup += 0x8FC;
    }
}

void menuCard_calcCollectorPoints(s32 *out) {
    s32 kinds;
    s32 typePts;
    s32 arrowPts;
    s32 i;
    s32 j;
    CardInfo *infoTbl;
    CardEntry *card;

    infoTbl = card_getInfoTable();
    kinds = 0;
    typePts = 0;
    arrowPts = 0;
    for (i = 0; i < CARD_COUNT; i++) {
        if (s_menuCard.count[i] != 0) {
            kinds += infoTbl[i].points;
        }
    }
    memset(s_menuCardArrowsSeen, 0, 0x100);
    for (i = 0; i < CARD_COUNT; i++) {
        for (j = 0; j < s_menuCard.count[i]; j++) {
            card = card_find(i, j);
            s_menuCardArrowsSeen[card->arrows] = 1;
            typePts += s_menuCardTypePoints[card->type];
        }
    }
    for (i = 0; i < 0x100; i++) {
        if (s_menuCardArrowsSeen[i] != 0) {
            arrowPts += 5;
        }
    }
    out[CARD_PTS_KINDS] = kinds;
    out[CARD_PTS_TYPES] = typePts;
    out[CARD_PTS_ARROWS] = arrowPts;
    out[CARD_PTS_TOTAL] = kinds + typePts + arrowPts;
}

void menuCard_updateHelp(void) {
    RECT bounds;
    u8 owned;

    g_menuHelp.text = NULL;
    help_setBounds(NULL);
    if (s_menuCard.state == CARD_ST_GRID) {
        owned = s_menuCard.count[s_menuCard.col * CARD_GRID_ROWS + s_menuCard.row];
        if (owned != 0) {
            g_menuHelp.text = text_get(owned >= 2 ? CARD_TEXT_HELP_STACK : CARD_TEXT_HELP_CARD);
        }
    } else if (s_menuCard.state == CARD_ST_DISCARD) {
        if (s_menuCard.discardCursor == CARD_DISCARD_NO) {
            bounds.x = 0;
            bounds.y = 0x8C;
            bounds.w = g_gameState->dispEnv->disp.w;
            bounds.h = g_gameState->dispEnv->disp.h - 0x8C;
            help_setBounds(&bounds);
        }
        g_menuHelp.text = text_get(s_menuCard.discardCursor + CARD_TEXT_HELP_DISCARD);
    }
}

void menuCard_update(void) {
    menuCard_input();
    menuCard_draw();
    menuCard_updateAnims();
}

void menuCard_exit(void) {
    s32 i;
    s32 id;

    for (i = 0; i < 4; i++) {
        id = s_menuCardRestoreRepeatIds[i];
        pad_setRepeatDelay(0, id, s_menuCard.saved[i].repeatDelay);
        pad_setRepeatInterval(0, id, s_menuCard.saved[i].repeatInterval);
    }
    help_setCallback(NULL);
    menuCard_unloadImages();
}

void menuCard_input(void) {
    void (*handler)();

    if (!fade_isBusy() && s_menuCard.flip.active == 0) {
        handler = s_menuCardInputFuncs[s_menuCard.state];
        if (handler != NULL) {
            handler();
        }
    }
}

void menuCard_draw(void) {
    menuCard_drawGrid();
    menuCard_drawInfo();
    menuCard_drawCardInfo();
    menuCard_drawCellHighlight();
    menuCard_drawCursor();
}

void menuCard_updateAnims(void) {
    menuCard_updateImageLoad();
    menuCard_updateFlip();
    menuCard_updateFade();
}

void menuCard_inputDiscard(void) {
    s32 cur;

    cur = s_menuCard.col * CARD_GRID_ROWS + s_menuCard.row;
    if (g_padRepeat & (BTN_CIRCLE | BTN_CROSS)) {
        if ((g_padRepeat & BTN_CROSS) && s_menuCard.discardCursor == CARD_DISCARD_YES) {
            snd_playSe(SE_CURSOR);
            card_remove(cur, s_menuCard.shown[cur]);
            menuCard_refresh();
        } else {
            snd_playSe(SE_CANCEL);
        }
        s_menuCard.state = CARD_ST_NONE;
        win_close(s_menuCard.discardWin);
        return;
    }
    if (g_padRepeat & (PAD_UP | PAD_DOWN)) {
        snd_playSe(SE_CURSOR);
        s_menuCard.discardCursor ^= 1;
    }
}

void menuCard_drawCardInfo(void) {
    RECT *rect = &s_menuCard.cardInfoRect;
    CardCache *imgCache = &s_menuCard.cache;
    s32 cur;

    cur = s_menuCard.col * CARD_GRID_ROWS + s_menuCard.row;
    ui_drawLabel(rect->x + 8, rect->y - 3, 0x1E, g_labelCardInfo, UI_LABEL_NORMAL);
    if (imgCache->shown != CARD_NONE) {
        if (imgCache->shown != cur && imgCache->fading != 0) {
            menuCard_drawCardStack(imgCache->prev);
        } else {
            do {
                menuCard_drawCardStack(cur);
            } while (0);
        }
    }
    win_drawBox(rect, 0x1E, WIN_STYLE_BEVEL);
    win_drawShadow(rect, 0x1F, WIN_STYLE_BEVEL);
}

void menuCard_drawCard(s32 x, s32 y, s32 z, s32 id, s32 dim, u8 flag) {
    CardEntry *card;
    s32 shade;

    if (flag) {
        if (dim == 0) {
            card = card_find(id, (s32)s_menuCard.shown[id] % (s32)s_menuCard.count[id]);
            menuCard_drawStats(x, y, 0x14, card);
            menuCard_drawArrows(x, y, 0x14, card);
        }
        menuCard_drawFace(x, y, 0x14, dim);
        return;
    }
    shade = 0x80 - dim * 0x16;
    menuCard_drawSprite(x, y, 0x14, CARD_SPR_BACK, shade | (shade << 8) | (shade << 16) | GPU_CODE(GPU_SPRT));
}

void menuCard_drawCell(s32 x, s32 y, s32 z, s32 id) {
    s32 owned;
    s32 icon;
    s32 tens;
    s32 digit;

    owned = s_menuCard.count[id];
    if (owned == 0) {
        icon = CARD_SPR_EMPTY_CELL;
    } else if (owned < 2) {
        icon = card_getRarity(id) + CARD_SPR_CELL;
    } else {
        icon = card_getRarity(id) + CARD_SPR_CELL_STACK;
        digit = owned % 10;
        tens = owned / 10;
        menuCard_drawSprite(x + 9, y + 7, 0x14, digit + CARD_SPR_DIGIT, GPU_RGBC(GPU_SPRT, GPU_RGB_NEUTRAL));
        if (owned >= 10) {
            digit = tens % 10;
            menuCard_drawSprite(x + 5, y + 7, 0x14, digit + CARD_SPR_DIGIT, GPU_RGBC(GPU_SPRT, GPU_RGB_NEUTRAL));
        }
        if (owned >= 100) {
            digit = owned / 100;
            menuCard_drawSprite(x + 1, y + 7, 0x14, digit + CARD_SPR_DIGIT, GPU_RGBC(GPU_SPRT, GPU_RGB_NEUTRAL));
        }
    }
    menuCard_drawSprite(x, y, 0x14, icon, GPU_RGBC(GPU_SPRT, GPU_RGB_NEUTRAL));
}

void menuCard_drawArrows(s32 x, s32 y, s32 otz, CardEntry *card) {
    u32 color;
    s32 i;

    color = menuCard_getStatsColor();
    for (i = 0; i < 8; i++) {
        if ((card->arrows >> i) & 1) {
            menuCard_drawSprite(x + s_menuCardArrows[i].x, y + s_menuCardArrows[i].y, otz, s_menuCardArrows[i].sprite, color);
        }
    }
}

void menuCard_drawStats(s32 x, s32 y, s32 otz, CardEntry *card) {
    u8 statSpr[4];
    u32 color;
    s32 i;
    s32 dx;

    color = menuCard_getStatsColor();
    statSpr[0] = (card->power >> 4) + CARD_SPR_HEX;
    statSpr[1] = card->type + CARD_SPR_TYPE;
    statSpr[2] = (card->physDef >> 4) + CARD_SPR_HEX;
    statSpr[3] = (card->magicDef >> 4) + CARD_SPR_HEX;
    for (i = 0; i < 4; i++) {
        menuCard_drawSprite(x + 9 + i * 6, y + 0x26, otz, statSpr[i], color);
    }
}

void menuCard_updateFlip(void) {
    CardFlip *turn;

    turn = &s_menuCard.flip;
    if (turn->active != 0) {
        if (++turn->ticks >= CARD_FLIP_FRAMES) {
            if (turn->dir == CARD_FLIP_NEXT) {
                s_menuCard.shown[turn->id] = s_menuCard.shown[turn->id] >= s_menuCard.count[turn->id] - 1 ? 0 : s_menuCard.shown[turn->id] + 1;
            }
            turn->ticks = 0;
            turn->active = 0;
        }
    }
}

void menuCard_updateFade(void) {
    CardCache *imgCache = &s_menuCard.cache;

    if (imgCache->fading != 0) {
        if (++imgCache->fadeTicks >= 4) {
            imgCache->fading = 0;
            imgCache->fadeTicks = 0;
        }
    }
}

void menuCard_selectCard(s32 id) {
    CardCache *imgCache;
    CardSlot *slot;
    u8 prev;
    s32 owned;

    owned = s_menuCard.count[id];
    imgCache = &s_menuCard.cache;
    prev = imgCache->cur;
    imgCache->fading = 1;
    imgCache->fadeTicks = 0;
    imgCache->cur = id;
    imgCache->prev = prev;
    if (owned != 0) {
        slot = menuCard_findCachedImage(id);
        if (slot == NULL) {
            imgCache->pending = id;
            return;
        }
        menuCard_showCachedImage(slot);
    }
}

void menuCard_initImageCache(void) {
    CardCache *imgCache = &s_menuCard.cache;
    s32 i;

    imgCache->next = 0;
    imgCache->oldest = 0;
    imgCache->used = 0;
    imgCache->prev = CARD_NONE;
    imgCache->cur = CARD_NONE;
    imgCache->loading = CARD_NONE;
    imgCache->pending = CARD_NONE;
    imgCache->reading = 0;
    imgCache->fadeTicks = 0;
    imgCache->shown = CARD_NONE;
    for (i = 0; i < CARD_CACHE_SLOTS; i++) {
        imgCache->slots[i].used = 0;
        imgCache->slots[i].id = 0;
        imgCache->slots[i].vram = i;
    }
}

CardSlot *menuCard_findCachedImage(s32 id) {
    CardCache *imgCache = &s_menuCard.cache;
    s32 i;

    for (i = 0; i < CARD_CACHE_SLOTS; i++) {
        if (imgCache->slots[i].used != 0 && imgCache->slots[i].id == id) {
            return &imgCache->slots[i];
        }
    }
    return NULL;
}

void menuCard_showCachedImage(CardSlot *slot) {
    menuCard_showCachedImage_inline(slot);
}

void menuCard_loadImages(void) {
    u8 *image1 = s_menuCardImage1;
    u8 *image2 = s_menuCardImage2;

    menuCard_saveVram(CARD_VRAM_SAVE_BUF);
    LoadImage(&s_menuCardImage1ClutRect, (u32 *)image1);
    LoadImage(&s_menuCardImage1Rect, (u32 *)(image1 + 0x200));
    LoadImage(&s_menuCardImage2ClutRect, (u32 *)image2);
    LoadImage(&s_menuCardImage2Rect, (u32 *)(image2 + 0x200));
    DrawSync(0);
}

void menuCard_unloadImages(void) {
    menuCard_restoreVram(CARD_VRAM_SAVE_BUF);
    DrawSync(0);
}

void menuCard_onDiscardOpened(Window *w) {
    s_menuCard.state = CARD_ST_DISCARD;
}

void menuCard_onDiscardClosed(Window *w) {
    s_menuCard.state = CARD_ST_GRID;
}

void menuCard_drawDiscardWindow(Window *w) {
    text_drawBlock(w->x + 0xE, w->y + 0xA, w->otz, text_get(CARD_TEXT_DISCARD), TEXT_PAL_WHITE);
    text_drawBlock(w->x + 0x30, w->y + 0x2E, w->otz, text_get(CARD_TEXT_DISCARD + 1), TEXT_PAL_WHITE);
    text_drawBlock(w->x + 0x30, w->y + 0x3D, w->otz, text_get(CARD_TEXT_DISCARD + 2), TEXT_PAL_WHITE);
}

void menuCard_initFlip(void) {
    CardFlip *turn = &s_menuCard.flip;

    turn->active = 0;
    turn->ticks = 0;
}

void menuCard_flipCopy(s32 card, s32 dir) {
    CardFlip *turn;

    turn = &s_menuCard.flip;
    turn->active = 1;
    turn->ticks = 0;
    turn->dir = dir;
    turn->id = card;
    if (dir == CARD_FLIP_PREV) {
        if (s_menuCard.shown[turn->id] != 0) {
            s_menuCard.shown[turn->id]--;
        } else {
            s_menuCard.shown[turn->id] = s_menuCard.count[turn->id] - 1;
        }
    }
}

s32 menuCard_decodeRle(u8 *src, u8 *dst) {
    RleHeader *h = (RleHeader *)src;
    u8 *start = dst;
    u8 *end;
    u8 key;
    s32 runLen, i;

    key = ~h->notKey;
    src += 4;
    end = src + h->size;
    while (src < end) {
        if (*src <= key) {
            *dst++ = *src++;
        } else {
            runLen = *src++ - key;
            for (i = 0; i < runLen; i++) {
                *dst++ = *src;
            }
            src++;
        }
    }
    return dst - start;
}

u32 menuCard_getStatsColor(void) {
    return GPU_RGBC(GPU_SPRT, GPU_RGB_NEUTRAL);
}

INCLUDE_RODATA("asm/us/ovl_36/nonmatchings/ovl_36", s_menuCardImage1);

INCLUDE_RODATA("asm/us/ovl_36/nonmatchings/ovl_36", s_menuCardImage2);

INCLUDE_RODATA("asm/us/ovl_36/nonmatchings/ovl_36", s_menuCardImageOffsets);
