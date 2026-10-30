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
    s_menuCard.imageFile = file_find(1, CARD_IMAGE_FILE);
    for (i = 0; i < 4; i++) {
        id = D_801F30D4[i];
        s_menuCard.saved[i].unk0 = pad_getRepeatDelay(0, id);
        s_menuCard.saved[i].unk4 = pad_getRepeatInterval(0, id);
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
        s_menuCard.discardWin = win_openFree(&s_menuCardDiscardRect, 10, 2);
        win_setDrawContent(s_menuCard.discardWin, menuCard_drawDiscardWindow);
        win_setOnOpened(s_menuCard.discardWin, menuCard_onDiscardOpened);
        win_setOnClosed(s_menuCard.discardWin, menuCard_onDiscardClosed);
        win_setTitle(s_menuCard.discardWin, D_800704C4);
        win_setShadow(s_menuCard.discardWin, 1);
        return;
    }
    snd_playSe(SE_BUZZER);
}

void menuCard_drawGrid(void) {
    u8 buf[8];
    RECT *r;
    s32 a;
    s32 b;
    s32 n;
    s32 i;
    s32 j;
    s32 x;
    s32 y;
    u8 *s;
    s32 w;
    s32 k = 8;
    s32 k2 = 0x80;

    r = &s_menuCard.gridRect;
    a = card_countAll();
    b = card_countKinds();
    ui_drawLabel(r->x + 8, r->y - 3, 0x1E, D_8007041C, 0);
    win_drawBox(r, 0x1E, 1);
    win_drawShadow(r, 0x1E, 1);
    s = text_get(CARD_TEXT_STOCK);
    w = text_measureWidth(s);
    text_formatNumber(a, buf, 3, 1);
    text_drawBlock(r->x + 8, r->y + 0xA2, 0x14, s, 0);
    ui_drawDigits(r->x + (w + k), r->y + 0xA4, 0x14, buf, 0);
    s = text_get(CARD_TEXT_TYPE);
    w = text_measureWidth(s);
    text_formatNumber(b, buf, 3, 1);
    ui_drawDigits(r->x + 0x80, r->y + 0xA4, 0x14, buf, 0);
    text_drawBlock(r->x - (w - k2), r->y + 0xA2, 0x14, s, 0);
    for (n = 0, i = 0, x = 4; i < CARD_GRID_COLS; i++, x += 15) {
        for (j = 0, y = 10; j < CARD_GRID_ROWS; j++, y += 15) {
            menuCard_drawCell(r->x + x, r->y + y, 0x14, n);
            n++;
        }
    }
}

void menuCard_drawInfo(void) {
    u8 buf[8];
    RECT *r;
    s32 a;
    s32 b;
    s32 c;
    s32 rank;
    s32 w;

    r = &s_menuCard.infoRect;
    if (card_getWins() <= 99999) {
        a = card_getWins();
    } else {
        a = 99999;
    }
    if (card_getLosses() <= 99999) {
        b = card_getLosses();
    } else {
        b = 99999;
    }
    if (card_getDraws() <= 99999) {
        c = card_getDraws();
    } else {
        c = 99999;
    }
    rank = s_menuCard.rank;
    ui_drawLabel(r->x + 8, r->y - 3, 0x1E, D_80070460, 0);
    win_drawBox(r, 0x1E, 2);
    win_drawShadow(r, 0x1F, 2);
    ui_drawEtchedLine(r->x + 4, r->y + 0x24, 0x14, r->w - 9);
    text_formatNumber(s_menuCard.points[CARD_PTS_TOTAL], buf, 4, 1);
    text_drawBlock(r->x + 0xA, r->y + 5, 0x14, text_get(CARD_TEXT_COLLECTORS), 0);
    text_drawBlock(r->x + 0xA, r->y + 0x14, 0x14, text_get(rank), 0);
    text_drawBlock(r->x + 0xA, r->y + 0x26, 0x14, text_get(CARD_TEXT_WIN), 0);
    text_drawBlock(r->x + 0xA, r->y + 0x30, 0x14, text_get(CARD_TEXT_LOSE), 0);
    text_drawBlock(r->x + 0xA, r->y + 0x3A, 0x14, text_get(CARD_TEXT_DRAW), 0);
    ui_drawSmallDigits(r->x + 0x5A, r->y + 9, 0x14, buf, 0);
    ui_drawSmallDigits(r->x + 0x78, r->y + 0xB, 0x14, s_menuCardPointsUnit, 0);
    w = menuCard_drawInfo_width(text_get(CARD_TEXT_DRAW));
    text_formatNumber(a, buf, 5, 1);
    w += 0xB;
    ui_drawSmallDigits(r->x + w, r->y + 0x2A, 0x14, buf, 0);
    text_formatNumber(b, buf, 5, 1);
    ui_drawSmallDigits(r->x + w, r->y + 0x34, 0x14, buf, 0);
    text_formatNumber(c, buf, 5, 1);
    ui_drawSmallDigits(r->x + w, r->y + 0x3E, 0x14, buf, 0);
}

void menuCard_drawCardStack(s32 id) {
    u8 buf[8];
    RECT *r;
    CardFlip *turn;
    CardCache *cache;
    u8 *name;
    u8 same;
    s32 i;
    s32 dx;
    s32 x;
    s32 y;
    s32 dy;
    s32 a;
    s32 b;
    s32 n;
    s32 cnt;

    r = &s_menuCard.cardInfoRect;
    n = s_menuCard.count[id];
    cnt = n;
    if (n >= 6) {
        n = 5;
    }
    turn = &s_menuCard.flip;
    cache = &s_menuCard.cache;
    if (cnt != 0) {
        same = id == cache->shown;
        name = (u8 *)card_getName(id);
        if (cnt >= 2) {
            text_formatNumber(s_menuCard.shown[id] + 1, buf, 3, 1);
            text_formatNumber(cnt, buf + 4, -1, 1);
            buf[3] = '/';
            ui_drawSmallDigits(r->x + 0x2B, r->y + 0x43, 0x14, buf, 0);
            a = text_getButtonSprite(4);
            b = text_getButtonSprite(5);
            ui_drawSprite(r->x + 0x1E, r->y + 0x41, 0x14, a, 0);
            ui_drawSprite(r->x + 0x5E, r->y + 0x41, 0x14, b, 0);
        }
        i = 0;
        menuCard_drawSprite(r->x + 0x10, r->y + 0x50, 0x14, CARD_SPR_NAME_ICON, 0x64808080);
        text_drawBlock(r->x + 0x20, r->y + 0x50, 0x14, name, 0);

        while (i < n) {
            dx = i * 4 + 0x2E;
            x = r->x + dx;
            dy = i + 9;
            y = r->y + dy;
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
    win_drawBox(r, 0x1E, 2);
    win_drawShadow(r, 0x1F, 2);
}

void menuCard_drawCellHighlight(void) {
    RECT *r = &s_menuCard.gridRect;
    s32 a;
    s32 x, y;
    u32 rgbc;

    a = g_menuFrame & 0x3F;
    if (a >= 0x20) {
        a = 0x3F - a;
    }
    x = r->x + (s16)(s_menuCard.col * 15 + 4);
    y = r->y + (s16)(s_menuCard.row * 15 + 10);
    setlen(g_primPtr, 5);
    rgbc = (a * 8) | 0x48000000;
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
    RECT a, b;
    CardCache *cc;
    CardSlot *s;
    s32 *ofs;
    s32 n;
    u32 k;

    cc = &s_menuCard.cache;
    ofs = s_menuCardImageOffsets;
    if (cc->used >= CARD_CACHE_SLOTS) {
        cc->slots[cc->oldest].used = 0;
        cc->used++;
        cc->oldest = (cc->oldest + 1) % CARD_CACHE_SLOTS;
    }
    s = &cc->slots[cc->next];
    {
        u8 id = s_menuCard.loadingId;
        s->used = 1;
        s->id = id;
    }
    cc->used++;
    cc->next = (cc->next + 1) % CARD_CACHE_SLOTS;
    n = menuCard_decodeRle(s_menuCard.sectorBuf, s_menuCard.clutBuf);
    if (ofs[s_menuCard.loadingId + 1] != ofs[s_menuCard.loadingId]) {
        menuCard_decodeRle((u8 *)s_menuCardImageOffsets - 4 + ofs[s_menuCard.loadingId], s_menuCard.clutBuf + n);
    }
    k = s->vram;
    a.x = 0;
    a.w = 0x100;
    a.h = 1;
    b.w = 0x13;
    b.h = 0x2F;
    a.y = k + 0x1F4;
    b.x = (u8)(k % 3) * 0x13;
    b.y = (u8)(k / 3) * 0x2F + 0x100;
    LoadImage(&a, (u32 *)s_menuCard.clutBuf);
    LoadImage(&b, (u32 *)s_menuCard.pixelBuf);
    DrawSync(0);
    if (s->id == cc->cur) {
        menuCard_showCachedImage_inline(s);
    }
}

void menuCard_updateImageLoad(void) {
    CardCache *c = &s_menuCard.cache;

    if (c->reading != 0) {
        if (cd_getState() != 0) {
            return;
        }
        c->reading = 0;
        menuCard_loadCardImage();
        menuCard_updateImageLoad_read(c);
    } else {
        menuCard_updateImageLoad_read(c);
    }
}

void menuCard_drawFace(s32 x, s32 y, s32 z, s32 dim) {
    SprtWords *p;
    u32 clut = (GetClut(0, 0x1F2) << 16) | 0xB000;
    s32 c;
    u32 rgbc;

    setlen(g_primPtr, 4);
    c = 0x80 - dim * 0x16;
    rgbc = c | (c << 8) | (c << 16) | 0x64000000;
    p = (SprtWords *)g_primPtr;
    p->rgbc = rgbc;
    p->wh = 0x2F0026;
    p->xy = (x + 2) | ((y + 2) << 16);
    p->uvclut = clut;
    g_primPtr = (u8 *)(p + 1);
    addPrim(&g_ot[z], p);
    SetDrawTPage(g_primPtr, 1, 1, 0xBF);
    addPrim(&g_ot[z], g_primPtr);
    g_primPtr += 8;
    menuCard_drawSprite(x, y, 0x14, CARD_SPR_FRAME, rgbc);
}

void menuCard_refresh(void) {
    s32 i;
    u8 n;
    CardMenu *m;

    help_setCallback(card_countAll() ? menuCard_updateHelp : NULL);
    for (i = 0; i < CARD_COUNT; i++) {
        n = card_countOf(i);
        s_menuCard.count[i] = n;
        if (s_menuCard.shown[i] >= n) {
            s_menuCard.shown[i] = n - 1;
        }
    }
    menuCard_calcCollectorPoints(s_menuCard.points);
    m = &s_menuCard;
    m->rank = CARD_RANK_COUNT;
    for (i = 1; i < CARD_RANK_COUNT; i++) {
        if (m->points[CARD_PTS_TOTAL] < s_menuCardRankPoints[i]) {
            m->rank = i - 1;
            return;
        }
    }
}

void menuCard_drawSprite(s32 x, s32 y, s32 z, s32 idx, s32 rgbc) {
    SprtWords *p;
    Ovl36Sprite *t;

    setlen(g_primPtr, 4);
    p = (SprtWords *)g_primPtr;
    t = &s_menuCardSprites[idx];
    p->rgbc = rgbc;
    p->xy = x | (y << 16);
    p->uvclut = t->uvclut;
    p->wh = t->w | (t->h << 16);
    g_primPtr = (u8 *)(p + 1);
    addPrim(&g_ot[z], p);
    SetDrawTPage(g_primPtr, 1, 1, t->tpage | 0x20);
    addPrim(&g_ot[z], g_primPtr);
    g_primPtr += 8;
}

void menuCard_saveVram(u8 *buf) {
    RECT a = s_menuCardSlotClutRect;
    RECT b = s_menuCardSlotRect;
    s32 i;

    StoreImage(&s_menuCardSaveClutRect0, (u32 *)buf);
    buf += 0x200;
    StoreImage(&s_menuCardSaveClutRect1, (u32 *)buf);
    buf += 0x200;
    StoreImage(&s_menuCardSaveRect, (u32 *)buf);
    buf += 0x200;
    DrawSync(0);
    for (i = 0; i < CARD_CACHE_SLOTS; i++) {
        a.y = i + 0x1F4;
        b.x = (i % 3) * 0x13;
        b.y = (i / 3) * 0x2F + 0x100;
        StoreImage(&a, (u32 *)buf);
        DrawSync(0);
        StoreImage(&b, (u32 *)(buf + 0x200));
        DrawSync(0);
        buf += 0x8FC;
    }
}

void menuCard_restoreVram(u8 *buf) {
    RECT a = s_menuCardSlotClutRect;
    RECT b = s_menuCardSlotRect;
    s32 i;

    LoadImage(&s_menuCardRestoreClutRect0, (u32 *)buf);
    buf += 0x200;
    LoadImage(&s_menuCardRestoreClutRect1, (u32 *)buf);
    buf += 0x200;
    DrawSync(0);
    LoadImage(&s_menuCardRestoreRect, (u32 *)buf);
    buf += 0x200;
    DrawSync(0);
    for (i = 0; i < CARD_CACHE_SLOTS; i++) {
        a.y = i + 0x1F4;
        b.x = (i % 3) * 0x13;
        b.y = (i / 3) * 0x2F + 0x100;
        LoadImage(&a, (u32 *)buf);
        DrawSync(0);
        LoadImage(&b, (u32 *)(buf + 0x200));
        DrawSync(0);
        buf += 0x8FC;
    }
}

void menuCard_calcCollectorPoints(s32 *out) {
    s32 kinds;
    s32 typePts;
    s32 arrowPts;
    s32 i;
    s32 j;
    CardInfo *e;
    Unk6 *c;

    e = card_getInfoTable();
    kinds = 0;
    typePts = 0;
    arrowPts = 0;
    for (i = 0; i < CARD_COUNT; i++) {
        if (s_menuCard.count[i] != 0) {
            kinds += e[i].unk4;
        }
    }
    memset(s_menuCardArrowsSeen, 0, 0x100);
    for (i = 0; i < CARD_COUNT; i++) {
        for (j = 0; j < s_menuCard.count[i]; j++) {
            c = card_find(i, j);
            s_menuCardArrowsSeen[c->unk1] = 1;
            typePts += s_menuCardTypePoints[c->unk3];
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
    RECT r;
    u8 n;

    g_menuHelp.text = NULL;
    help_setBounds(NULL);
    if (s_menuCard.state == CARD_ST_GRID) {
        n = s_menuCard.count[s_menuCard.col * CARD_GRID_ROWS + s_menuCard.row];
        if (n != 0) {
            g_menuHelp.text = text_get(n >= 2 ? CARD_TEXT_HELP_STACK : CARD_TEXT_HELP_CARD);
        }
    } else if (s_menuCard.state == CARD_ST_DISCARD) {
        if (s_menuCard.discardCursor == CARD_DISCARD_NO) {
            r.x = 0;
            r.y = 0x8C;
            r.w = g_gameState->dispEnv->disp.w;
            r.h = g_gameState->dispEnv->disp.h - 0x8C;
            help_setBounds(&r);
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
        id = D_801F30E4[i];
        pad_setRepeatDelay(0, id, s_menuCard.saved[i].unk0);
        pad_setRepeatInterval(0, id, s_menuCard.saved[i].unk4);
    }
    help_setCallback(NULL);
    menuCard_unloadImages();
}

void menuCard_input(void) {
    void (*f)();

    if (!fade_isBusy() && s_menuCard.flip.active == 0) {
        f = s_menuCardInputFuncs[s_menuCard.state];
        if (f != NULL) {
            f();
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
    RECT *r = &s_menuCard.cardInfoRect;
    CardCache *c = &s_menuCard.cache;
    s32 cur;

    cur = s_menuCard.col * CARD_GRID_ROWS + s_menuCard.row;
    ui_drawLabel(r->x + 8, r->y - 3, 0x1E, D_80070418, 0);
    if (c->shown != CARD_NONE) {
        if (c->shown != cur && c->fading != 0) {
            menuCard_drawCardStack(c->prev);
        } else {
            do {
                menuCard_drawCardStack(cur);
            } while (0);
        }
    }
    win_drawBox(r, 0x1E, 2);
    win_drawShadow(r, 0x1F, 2);
}

void menuCard_drawCard(s32 x, s32 y, s32 z, s32 id, s32 dim, u8 flag) {
    Unk6 *card;
    s32 c;

    if (flag) {
        if (dim == 0) {
            card = card_find(id, (s32)s_menuCard.shown[id] % (s32)s_menuCard.count[id]);
            menuCard_drawStats(x, y, 0x14, card);
            menuCard_drawArrows(x, y, 0x14, card);
        }
        menuCard_drawFace(x, y, 0x14, dim);
        return;
    }
    c = 0x80 - dim * 0x16;
    menuCard_drawSprite(x, y, 0x14, CARD_SPR_BACK, c | (c << 8) | (c << 16) | 0x64000000);
}

void menuCard_drawCell(s32 x, s32 y, s32 z, s32 id) {
    s32 n;
    s32 icon;
    s32 q;
    s32 d;

    n = s_menuCard.count[id];
    if (n == 0) {
        icon = CARD_SPR_EMPTY_CELL;
    } else if (n < 2) {
        icon = card_getRarity(id) + CARD_SPR_CELL;
    } else {
        icon = card_getRarity(id) + CARD_SPR_CELL_STACK;
        d = n % 10;
        q = n / 10;
        menuCard_drawSprite(x + 9, y + 7, 0x14, d + CARD_SPR_DIGIT, 0x64808080);
        if (n >= 10) {
            d = q % 10;
            menuCard_drawSprite(x + 5, y + 7, 0x14, d + CARD_SPR_DIGIT, 0x64808080);
        }
        if (n >= 100) {
            d = n / 100;
            menuCard_drawSprite(x + 1, y + 7, 0x14, d + CARD_SPR_DIGIT, 0x64808080);
        }
    }
    menuCard_drawSprite(x, y, 0x14, icon, 0x64808080);
}

void menuCard_drawArrows(s32 x, s32 y, s32 otz, Unk6 *card) {
    u32 color;
    s32 i;

    color = menuCard_getStatsColor();
    for (i = 0; i < 8; i++) {
        if ((card->unk1 >> i) & 1) {
            menuCard_drawSprite(x + s_menuCardArrows[i].x, y + s_menuCardArrows[i].y, otz, s_menuCardArrows[i].sprite, color);
        }
    }
}

void menuCard_drawStats(s32 x, s32 y, s32 otz, Unk6 *card) {
    u8 s[4];
    u32 color;
    s32 i;
    s32 dx;

    color = menuCard_getStatsColor();
    s[0] = (card->unk2 >> 4) + CARD_SPR_HEX;
    s[1] = card->unk3 + CARD_SPR_TYPE;
    s[2] = (card->unk4 >> 4) + CARD_SPR_HEX;
    s[3] = (card->unk5 >> 4) + CARD_SPR_HEX;
    for (i = 0; i < 4; i++) {
        menuCard_drawSprite(x + 9 + i * 6, y + 0x26, otz, s[i], color);
    }
}

void menuCard_updateFlip(void) {
    CardFlip *t;

    t = &s_menuCard.flip;
    if (t->active != 0) {
        if (++t->ticks >= CARD_FLIP_FRAMES) {
            if (t->dir == CARD_FLIP_NEXT) {
                s_menuCard.shown[t->id] = s_menuCard.shown[t->id] >= s_menuCard.count[t->id] - 1 ? 0 : s_menuCard.shown[t->id] + 1;
            }
            t->ticks = 0;
            t->active = 0;
        }
    }
}

void menuCard_updateFade(void) {
    CardCache *c = &s_menuCard.cache;

    if (c->fading != 0) {
        if (++c->fadeTicks >= 4) {
            c->fading = 0;
            c->fadeTicks = 0;
        }
    }
}

void menuCard_selectCard(s32 id) {
    CardCache *c;
    CardSlot *s;
    u8 prev;
    s32 owned;

    owned = s_menuCard.count[id];
    c = &s_menuCard.cache;
    prev = c->cur;
    c->fading = 1;
    c->fadeTicks = 0;
    c->cur = id;
    c->prev = prev;
    if (owned != 0) {
        s = menuCard_findCachedImage(id);
        if (s == NULL) {
            c->pending = id;
            return;
        }
        menuCard_showCachedImage(s);
    }
}

void menuCard_initImageCache(void) {
    CardCache *c = &s_menuCard.cache;
    s32 i;

    c->next = 0;
    c->oldest = 0;
    c->used = 0;
    c->prev = CARD_NONE;
    c->cur = CARD_NONE;
    c->loading = CARD_NONE;
    c->pending = CARD_NONE;
    c->reading = 0;
    c->fadeTicks = 0;
    c->shown = CARD_NONE;
    for (i = 0; i < CARD_CACHE_SLOTS; i++) {
        c->slots[i].used = 0;
        c->slots[i].id = 0;
        c->slots[i].vram = i;
    }
}

CardSlot *menuCard_findCachedImage(s32 id) {
    CardCache *c = &s_menuCard.cache;
    s32 i;

    for (i = 0; i < CARD_CACHE_SLOTS; i++) {
        if (c->slots[i].used != 0 && c->slots[i].id == id) {
            return &c->slots[i];
        }
    }
    return NULL;
}

void menuCard_showCachedImage(CardSlot *s) {
    RECT a, b;
    u32 n = s->vram;

    a.x = 0;
    a.w = 0x100;
    a.h = 1;
    b.w = 0x13;
    b.h = 0x2F;
    a.y = n + 0x1F4;
    b.x = (u8)(n % 3) * 0x13;
    b.y = (u8)(n / 3) * 0x2F + 0x100;
    MoveImage(&a, 0, 0x1F2);
    MoveImage(&b, 0x3C0, 0x1B0);
    DrawSync(0);
    s_menuCard.cache.shown = s->id;
}

void menuCard_loadImages(void) {
    u8 *a = s_menuCardImage1;
    u8 *b = s_menuCardImage2;

    menuCard_saveVram(CARD_VRAM_SAVE_BUF);
    LoadImage(&s_menuCardImage1ClutRect, (u32 *)a);
    LoadImage(&s_menuCardImage1Rect, (u32 *)(a + 0x200));
    LoadImage(&s_menuCardImage2ClutRect, (u32 *)b);
    LoadImage(&s_menuCardImage2Rect, (u32 *)(b + 0x200));
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
    text_drawBlock(w->x + 0xE, w->y + 0xA, w->otz, text_get(CARD_TEXT_DISCARD), 0);
    text_drawBlock(w->x + 0x30, w->y + 0x2E, w->otz, text_get(CARD_TEXT_DISCARD + 1), 0);
    text_drawBlock(w->x + 0x30, w->y + 0x3D, w->otz, text_get(CARD_TEXT_DISCARD + 2), 0);
}

void menuCard_initFlip(void) {
    CardFlip *t = &s_menuCard.flip;

    t->active = 0;
    t->ticks = 0;
}

void menuCard_flipCopy(s32 card, s32 dir) {
    CardFlip *t;

    t = &s_menuCard.flip;
    t->active = 1;
    t->ticks = 0;
    t->dir = dir;
    t->id = card;
    if (dir == CARD_FLIP_PREV) {
        if (s_menuCard.shown[t->id] != 0) {
            s_menuCard.shown[t->id]--;
        } else {
            s_menuCard.shown[t->id] = s_menuCard.count[t->id] - 1;
        }
    }
}

s32 menuCard_decodeRle(u8 *src, u8 *dst) {
    RleHeader *h = (RleHeader *)src;
    u8 *start = dst;
    u8 *end;
    u8 key;
    s32 n, i;

    key = ~h->notKey;
    src += 4;
    end = src + h->size;
    while (src < end) {
        if (*src <= key) {
            *dst++ = *src++;
        } else {
            n = *src++ - key;
            for (i = 0; i < n; i++) {
                *dst++ = *src;
            }
            src++;
        }
    }
    return dst - start;
}

u32 menuCard_getStatsColor(void) {
    return 0x64808080;
}

INCLUDE_RODATA("asm/us/ovl_36/nonmatchings/ovl_36", s_menuCardImage1);

INCLUDE_RODATA("asm/us/ovl_36/nonmatchings/ovl_36", s_menuCardImage2);

INCLUDE_RODATA("asm/us/ovl_36/nonmatchings/ovl_36", s_menuCardImageOffsets);
