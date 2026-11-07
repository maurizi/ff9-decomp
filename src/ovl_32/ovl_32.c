#include "ovl_32.h"

INCLUDE_RODATA("asm/us/ovl_32/nonmatchings/ovl_32", s_menuChocographIcons);

/* text bank for this menu (count + {offset, info} table, then strings) */
INCLUDE_RODATA("asm/us/ovl_32/nonmatchings/ovl_32", g_chocographText);

void menuChocograph_init(void) {
    ChocographMenu *st;

    if (g_gameState->gameData->chocograph > CHOCOGRAPH_COUNT) {
        g_gameState->gameData->chocograph = 0;
    }
    st = &s_menuChocograph;
    st->mode = CHOCO_MODE_TABS;
    s_menuChocograph.tab = CHOCO_TAB_SELECT;
    text_setBank(g_chocographText);
    help_setCallback(menuChocograph_updateHelp);
    help_setCursorOnTop(1);
    setRECT(&s_menuChocograph.titleRect, CHOCO_TITLE_WIN_X, CHOCO_TITLE_WIN_Y, CHOCO_TITLE_WIN_W, CHOCO_TITLE_WIN_H);
    setRECT(&s_menuChocograph.tabRect, CHOCO_TABS_WIN_X, CHOCO_TABS_WIN_Y, CHOCO_TABS_WIN_W, CHOCO_TABS_WIN_H);
    setRECT(&s_menuChocograph.selectedRect, CHOCO_SELECTED_WIN_X, CHOCO_SELECTED_WIN_Y, CHOCO_SELECTED_WIN_W, CHOCO_SELECTED_WIN_H);
    setRECT(&s_menuChocograph.listRect, CHOCO_LIST_WIN_X, CHOCO_LIST_WIN_Y, CHOCO_LIST_WIN_W, CHOCO_LIST_WIN_H);
    setRECT(&s_menuChocograph.countRect, CHOCO_COUNT_WIN_X, CHOCO_COUNT_WIN_Y, CHOCO_COUNT_WIN_W, CHOCO_COUNT_WIN_H);
    setRECT(&s_menuChocograph.abilityRect, CHOCO_ABIL_WIN_X, CHOCO_ABIL_WIN_Y, CHOCO_ABIL_WIN_W, CHOCO_ABIL_WIN_H);
    setRECT(&s_menuChocograph.infoRect, CHOCO_HINT_WIN_X, CHOCO_HINT_WIN_Y, CHOCO_HINT_WIN_W, CHOCO_HINT_WIN_H);
    menuList_init(&s_menuChocograph.list, CHOCOGRAPH_COUNT, CHOCO_LIST_ROWS, 1, CHOCO_LIST_WIN_H);
    menuChocograph_readFlags();
    menuChocograph_loadImages();
    if (g_gameState->gameData->chocograph != 0) {
        menuChocograph_scrollTo(g_gameState->gameData->chocograph - 1);
    }
    s_menuChocograph.textPtr = s_menuChocograph.textBuf + (g_gameState->gameData->drawBuf << 12);
}

void menuChocograph_inputTabs(void) {
    u8 tab;
    ChocographMenu *st;

    if (g_padRepeat & BTN_CIRCLE) {
        snd_playSe(SE_CANCEL);
        menu_close();
    } else if (g_padRepeat & BTN_CROSS) {
        if (s_menuChocograph.tab == CHOCO_TAB_SELECT) {
            s_menuChocograph.mode = CHOCO_MODE_LIST;
            snd_playSe(SE_CURSOR);
        } else if (g_gameState->gameData->chocograph == 0) {
            snd_playSe(SE_BUZZER);
        } else {
            g_gameState->gameData->chocograph = 0;
            snd_playSe(SE_SET);
        }
    } else {
        if (g_padRepeat & PAD_LEFT) {
            snd_playSe(SE_CURSOR);
            st = &s_menuChocograph;
            if (st->tab != 0) {
                tab = st->tab - 1;
            } else {
                tab = 1;
            }
        } else if (g_padRepeat & PAD_RIGHT) {
            snd_playSe(SE_CURSOR);
            st = &s_menuChocograph;
            if (st->tab == 0) {
                tab = st->tab + 1;
            } else {
                tab = 0;
            }
        } else {
            return;
        }
        st->tab = tab;
    }
}

void menuChocograph_inputList(void) {
    ChocographMenu *st;
    MenuList *list = &s_menuChocograph.list;
    s32 idx;
    s32 abilityLv;

    if (list->scroll == 0) {
        if (g_padRepeat & BTN_CIRCLE) {
            snd_playSe(SE_CANCEL);
            s_menuChocograph.mode = CHOCO_MODE_TABS;
            return;
        }
        if (g_padRepeat & BTN_CROSS) {
            st = &s_menuChocograph;
            idx = list->top + list->cursor;
            if (st->owned & (1 << idx)) {
                abilityLv = st->ability;
                if (menuChocograph_getRequiredAbility(idx) < abilityLv) {
                    snd_playSe(SE_SET);
                    g_gameState->gameData->chocograph = list->top + list->cursor + 1;
                    return;
                }
            }
            snd_playSe(SE_BUZZER);
            return;
        }
        menuList_input(list, g_padRepeat);
    }
}

void menuChocograph_drawSelected(void) {
    RECT *rect = &s_menuChocograph.selectedRect;
    s32 sel;

    win_drawBox(rect, CHOCO_OTZ_WIN, WIN_STYLE_BEVEL);
    win_drawShadow(rect, CHOCO_OTZ_SHADOW, WIN_STYLE_BEVEL);
    ui_drawLabel(rect->x + CHOCO_LABEL_X, rect->y + WIN_TITLE_Y, CHOCO_OTZ_TEXT, g_labelSelected, UI_LABEL_NORMAL);
    sel = g_gameState->gameData->chocograph;
    if (sel != 0) {
        menuChocograph_drawIcon(rect->x + CHOCO_SEL_ICON_X, rect->y + CHOCO_SEL_ICON_Y, CHOCO_OTZ_TEXT, (s_menuChocograph.found & (1 << (sel - 1))) ? CHOCO_ICON_CHOCOGRAPH + 1 : CHOCO_ICON_CHOCOGRAPH, 0);
        text_drawBlock(rect->x + CHOCO_SEL_NAME_X, rect->y + CHOCO_SEL_NAME_Y, CHOCO_OTZ_TEXT, text_get(g_gameState->gameData->chocograph - 1 + CHOCO_TEXT_NAME), TEXT_PAL_WHITE);
        menuChocograph_drawPicture(rect->x + (CHOCO_SEL_FRAME_X + CHOCO_FRAME_INSET), rect->y + (CHOCO_SEL_FRAME_Y + CHOCO_FRAME_INSET), CHOCO_OTZ_TEXT, g_gameState->gameData->chocograph - 1, 0);
        menuChocograph_drawFrame(rect->x + CHOCO_SEL_FRAME_X, rect->y + CHOCO_SEL_FRAME_Y, CHOCO_OTZ_TEXT, 0);
    }
}

void menuChocograph_drawList(void) {
    RECT sub;
    MenuView view;
    s32 i;
    RECT *rect = &s_menuChocograph.listRect;
    MenuList *list;

    ui_drawLabel(rect->x + CHOCO_LIST_LABEL_X, rect->y + WIN_TITLE_Y, CHOCO_OTZ_TEXT, g_labelChocograph, UI_LABEL_NORMAL);
    list = &s_menuChocograph.list;
    win_drawListBox(rect, CHOCO_OTZ_WIN, 1, list);
    win_drawShadow(rect, CHOCO_OTZ_WIN, WIN_STYLE_LINE_RULE);
    menuList_getView(list, &view);
    win_setClip(NULL, CHOCO_OTZ_TEXT, WIN_STYLE_LINE_RULE);
    for (i = 0; i < view.rows; i++) {
        menuChocograph_drawRow(0, i, view.first + i, &view);
    }
    if (list->scroll & MENULIST_SCROLL_PAGE) {
        menuList_getRowsRect(list, rect, &sub);
        win_setClip(&sub, CHOCO_OTZ_TEXT, WIN_STYLE_LINE_RULE);
        menuChocograph_drawScrollRows();
    }
    win_setClip(rect, CHOCO_OTZ_TEXT, WIN_STYLE_LINE_RULE);
}

void menuChocograph_drawCounts(void) {
    u8 numText[8];
    RECT *rect = &s_menuChocograph.countRect;
    ChocographMenu *st;

    win_drawBox(rect, CHOCO_OTZ_WIN, WIN_STYLE_BEVEL);
    win_drawShadow(rect, CHOCO_OTZ_SHADOW, WIN_STYLE_BEVEL);
    text_drawBlock(rect->x + CHOCO_COUNT_TEXT_X, rect->y + CHOCO_COUNT_INV_Y, CHOCO_OTZ_TEXT, text_get(CHOCO_TEXT_INVENTORY), TEXT_PAL_WHITE);
    text_drawBlock(rect->x + CHOCO_COUNT_TEXT_X, rect->y + CHOCO_COUNT_FOUND_Y, CHOCO_OTZ_TEXT, text_get(CHOCO_TEXT_FOUND), TEXT_PAL_WHITE);
    st = &s_menuChocograph;
    text_formatNumber(st->ownedCount, numText, 2, TEXT_NUM_ASCII);
    ui_drawSmallDigits(rect->x + CHOCO_COUNT_DIGITS_X, rect->y + (CHOCO_COUNT_INV_Y + 4), CHOCO_OTZ_TEXT, numText, TEXT_PAL_WHITE);
    text_formatNumber(st->foundCount, numText, 2, TEXT_NUM_ASCII);
    ui_drawSmallDigits(rect->x + CHOCO_COUNT_DIGITS_X, rect->y + (CHOCO_COUNT_FOUND_Y + 4), CHOCO_OTZ_TEXT, numText, TEXT_PAL_WHITE);
}

void menuChocograph_drawHint(void) {
    RECT *rect = &s_menuChocograph.infoRect;
    ChocographMenu *st = &s_menuChocograph;
    s32 id;
    s32 reqAbility;
    s32 i;

    id = st->list.top + st->list.cursor;
    ui_drawLabel(rect->x + CHOCO_LABEL_X, rect->y + WIN_TITLE_Y, CHOCO_OTZ_TEXT, g_labelHint, UI_LABEL_NORMAL);
    win_drawBox(rect, CHOCO_OTZ_WIN, WIN_STYLE_LINE_RULE);
    win_drawShadow(rect, CHOCO_OTZ_SHADOW, WIN_STYLE_LINE_RULE);
    if (st->mode == CHOCO_MODE_LIST && (st->owned & (1 << id))) {
        menuChocograph_getRequiredAbility(id);
        menuChocograph_drawPicture(rect->x + (CHOCO_HINT_FRAME_X + CHOCO_FRAME_INSET), rect->y + (CHOCO_HINT_FRAME_Y + CHOCO_FRAME_INSET), CHOCO_OTZ_TEXT, id, 0);
        menuChocograph_drawFrame(rect->x + CHOCO_HINT_FRAME_X, rect->y + CHOCO_HINT_FRAME_Y, CHOCO_OTZ_TEXT, 0);
        st->textPtr = text_buildBlock(rect->x + CHOCO_CLUE_X, rect->y + CHOCO_CLUE_Y, &g_ot[CHOCO_OTZ_TEXT], text_get(id + CHOCO_TEXT_CLUE), TEXT_PAL_WHITE, st->textPtr);
        text_drawBlock(rect->x + CHOCO_REQ_X, rect->y + CHOCO_REQ_TEXT_Y, CHOCO_OTZ_TEXT, text_get(CHOCO_TEXT_ABILITY), TEXT_PAL_WHITE);
        reqAbility = menuChocograph_getRequiredAbility(id);
        for (i = 0; i <= reqAbility; i++) {
            menuChocograph_drawIcon(rect->x + CHOCO_REQ_X + i * CHOCO_ABIL_ICON_W, rect->y + CHOCO_REQ_ICON_Y, CHOCO_OTZ_TEXT, i < st->ability ? i : CHOCO_ICON_LOCKED, i >= st->ability);
        }
    }
}

void menuChocograph_drawCursor(void) {
    help_setCursorOnTop(s_menuChocograph.mode != CHOCO_MODE_TABS);
    if (!fade_isBusy()) {
        if (s_menuChocograph.mode == CHOCO_MODE_TABS || (g_menuFrame & 3)) {
            g_menuHelp.y = CHOCO_TAB_CURSOR_Y;
            g_menuHelp.otz = CHOCO_OTZ_CURSOR;
            g_menuHelp.x = s_menuChocograph.tab * CHOCO_TAB_W + CHOCO_TAB_CURSOR_X;
            help_drawCursor(&g_menuHelp);
        }
        if (s_menuChocograph.mode == CHOCO_MODE_LIST) {
            g_menuHelp.x = 0;
            g_menuHelp.otz = CHOCO_OTZ_CURSOR;
            g_menuHelp.y = s_menuChocograph.list.cursor * CHOCO_ROW_H + CHOCO_LIST_CURSOR_Y;
            help_drawCursor();
        }
    }
}

void menuChocograph_updateHelp(void) {
    s32 idx;

    g_menuHelp.text = NULL;
    help_setBounds(0);
    if (s_menuChocograph.mode == CHOCO_MODE_TABS) {
        g_menuHelp.text = text_get(s_menuChocograph.tab + CHOCO_TEXT_HELP_TAB);
    } else if (s_menuChocograph.mode == CHOCO_MODE_LIST) {
        idx = s_menuChocograph.list.top + s_menuChocograph.list.cursor;
        if (s_menuChocograph.owned & (1 << idx)) {
            text_copy(s_menuChocographHelpBuf, text_get(idx + CHOCO_TEXT_HINT));
            if (s_menuChocograph.ability <= menuChocograph_getRequiredAbility_inline(idx)) {
                text_append(s_menuChocographHelpBuf, text_get(CHOCO_TEXT_INADEQUATE));
            }
            g_menuHelp.text = s_menuChocographHelpBuf;
        }
    }
}

void menuChocograph_readFlags(void) {
    s32 i;
    u32 bit;

    s_menuChocograph.found = 0;
    s_menuChocograph.owned = 0;
    s_menuChocograph.foundCount = 0;
    s_menuChocograph.ownedCount = 0;
    s_menuChocograph.ability = g_evtGlobalVars.b[CHOCO_VAR_ABILITY];
    for (i = 0; i < 3; i++) {
        s_menuChocograph.owned |= g_evtGlobalVars.b[i + CHOCO_VAR_OWNED] << (i * 8);
        s_menuChocograph.found |= g_evtGlobalVars.b[i + CHOCO_VAR_FOUND] << (i * 8);
    }
    for (i = 0; i < CHOCOGRAPH_COUNT; i++) {
        bit = 1 << i;
        if (s_menuChocograph.owned & bit) {
            s_menuChocograph.ownedCount++;
        }
        if (s_menuChocograph.found & bit) {
            s_menuChocograph.foundCount++;
        }
    }
}

void menuChocograph_loadImages(void) {
    RECT r0;
    RECT r1;
    Ovl32Image *src = &s_menuChocographPictures[1];
    Ovl32Image *dst = &s_menuChocographPictures[0];
    s32 i;

    menuChocograph_onLoadImages();
    LoadImage(&s_menuChocographIconClutRect, (u32 *)s_menuChocographIconImage);
    DrawSync(0);
    LoadImage(&s_menuChocographIconRect, (u32 *)(s_menuChocographIconImage + 0x1C0));
    DrawSync(0);
    LoadImage(&s_menuChocographFrameClutRect, (u32 *)s_menuChocographFrameImage);
    DrawSync(0);
    LoadImage(&s_menuChocographFrameRect, (u32 *)(s_menuChocographFrameImage + 0x40));
    DrawSync(0);
    for (i = 0; i < CHOCOGRAPH_COUNT; i++) {
        menuChocograph_getPictureRects(i, &r0, &r1);
        StoreImage(&r0, (u32 *)dst->clut);
        DrawSync(0);
        StoreImage(&r1, (u32 *)dst->pixels);
        DrawSync(0);
        LoadImage(&r0, (u32 *)src->clut);
        DrawSync(0);
        LoadImage(&r1, (u32 *)src->pixels);
        DrawSync(0);
        dst++;
        src++;
    }
}

void menuChocograph_restoreVram(void) {
    RECT r0;
    RECT r1;
    s32 i;
    Ovl32Image *img = s_menuChocographPictures;
    s32 sel;

    if (g_gameState->mode == GS_MODE_WORLD) {
        sel = g_gameState->gameData->chocograph;
        if (sel != 0) {
            menuChocograph_getPictureRects(sel - 1, &r0, &r1);
            MoveImage(&r0, WM_CHOCOGRAPH_CLUT_X, WM_CHOCOGRAPH_CLUT_Y);
            DrawSync(0);
            MoveImage(&r1, WM_CHOCOGRAPH_PICTURE_X, WM_CHOCOGRAPH_PICTURE_Y);
            DrawSync(0);
        }
    }
    for (i = 0; i < CHOCOGRAPH_COUNT; i++, img++) {
        menuChocograph_getPictureRects(i, &r0, &r1);
        LoadImage(&r0, (u32 *)img->clut);
        DrawSync(0);
        LoadImage(&r1, (u32 *)img->pixels);
        DrawSync(0);
    }
}

void menuChocograph_drawPicture(s32 x, s32 y, s32 z, s32 id, u8 dim) {
    SprtWords *sprt;
    u16 cl;
    u16 clut = GetClut(0, id + CHOCO_PICTURE_CLUT_Y);
    s32 u = (id % CHOCO_PICTURE_COLS) * CHOCO_PICTURE_W;
    s32 v = (id / CHOCO_PICTURE_COLS) * CHOCO_PICTURE_H;

    setlen(g_primPtr, PRIM_LEN(SPRT));
    cl = clut;
    ((SprtWords *)g_primPtr)->rgbc = dim ? GPU_RGBC(GPU_SPRT | GPU_ABE, GPU_RGB_NEUTRAL) : GPU_RGBC(GPU_SPRT, GPU_RGB_NEUTRAL);
    sprt = (SprtWords *)g_primPtr;
    sprt->xy = x | (y << 16);
    sprt->uvclut = (u & 0xFF) | ((v & 0xFF) << 8) | (cl << 16);
    sprt->wh = (CHOCO_PICTURE_H << 16) | CHOCO_PICTURE_W;
    g_primPtr = (u8 *)(sprt + 1);
    addPrim(&g_ot[z], sprt);
    SetDrawTPage(g_primPtr, 0, 1, getTPage(1, 0, 0, CHOCO_PICTURE_VRAM_Y));
    addPrim(&g_ot[z], g_primPtr);
    g_primPtr += 8;
}

void menuChocograph_drawFrame(s32 x, s32 y, s32 otz, u8 dim) {
    u16 clut;
    s32 i;

    clut = GetClut(CHOCO_FRAME_CLUT_X, CHOCO_FRAME_CLUT_Y);
    for (i = 0; i < (s32)ARRAY_COUNT(s_menuChocographFramePieces); i++) {
        setlen(g_primPtr, PRIM_LEN(POLY_FT4));
        ((Ft4W *)g_primPtr)->rgbc = dim ? GPU_RGBC(GPU_POLY_FT4 | GPU_ABE, GPU_RGB_NEUTRAL) : GPU_RGBC(GPU_POLY_FT4, GPU_RGB_NEUTRAL);
        if (i < 2) {
            ((Ft4W *)g_primPtr)->x0 = s_menuChocographFramePieces[i].x + x;
            ((Ft4W *)g_primPtr)->y0 = s_menuChocographFramePieces[i].y + y;
            ((Ft4W *)g_primPtr)->x1 = s_menuChocographFramePieces[i].x + x + s_menuChocographFramePieces[i].w;
            ((Ft4W *)g_primPtr)->y1 = s_menuChocographFramePieces[i].y + y;
            ((Ft4W *)g_primPtr)->x2 = s_menuChocographFramePieces[i].x + x;
            ((Ft4W *)g_primPtr)->y2 = s_menuChocographFramePieces[i].y + y + s_menuChocographFramePieces[i].h;
            ((Ft4W *)g_primPtr)->x3 = s_menuChocographFramePieces[i].x + x + s_menuChocographFramePieces[i].w;
            ((Ft4W *)g_primPtr)->y3 = s_menuChocographFramePieces[i].y + y + s_menuChocographFramePieces[i].h;
        } else {
            ((Ft4W *)g_primPtr)->x0 = s_menuChocographFramePieces[i].x + x;
            ((Ft4W *)g_primPtr)->y0 = s_menuChocographFramePieces[i].y + y + s_menuChocographFramePieces[i].w;
            ((Ft4W *)g_primPtr)->x1 = s_menuChocographFramePieces[i].x + x;
            ((Ft4W *)g_primPtr)->y1 = y + s_menuChocographFramePieces[i].y - (i != 2);
            ((Ft4W *)g_primPtr)->x2 = s_menuChocographFramePieces[i].x + x + s_menuChocographFramePieces[i].h;
            ((Ft4W *)g_primPtr)->y2 = s_menuChocographFramePieces[i].y + y + s_menuChocographFramePieces[i].w;
            ((Ft4W *)g_primPtr)->x3 = s_menuChocographFramePieces[i].x + x + s_menuChocographFramePieces[i].h;
            ((Ft4W *)g_primPtr)->y3 = y + s_menuChocographFramePieces[i].y - (i != 2);
        }
        ((Ft4W *)g_primPtr)->u0 = s_menuChocographFramePieces[i].u;
        ((Ft4W *)g_primPtr)->v0 = s_menuChocographFramePieces[i].v;
        ((Ft4W *)g_primPtr)->u1 = s_menuChocographFramePieces[i].u + s_menuChocographFramePieces[i].w;
        ((Ft4W *)g_primPtr)->v1 = s_menuChocographFramePieces[i].v;
        ((Ft4W *)g_primPtr)->u2 = s_menuChocographFramePieces[i].u;
        ((Ft4W *)g_primPtr)->v2 = s_menuChocographFramePieces[i].v + s_menuChocographFramePieces[i].h;
        ((Ft4W *)g_primPtr)->u3 = s_menuChocographFramePieces[i].u + s_menuChocographFramePieces[i].w;
        ((Ft4W *)g_primPtr)->v3 = s_menuChocographFramePieces[i].v + s_menuChocographFramePieces[i].h;
        ((Ft4W *)g_primPtr)->tpage = getTPage(0, 0, VRAM_UI_SHEET_X, VRAM_UI_SHEET_Y);
        ((Ft4W *)g_primPtr)->clut = clut;
        addPrim(&g_ot[otz], (Ft4W *)g_primPtr);
        g_primPtr += sizeof(Ft4W);
    }
    x += CHOCO_FRAME_SHADOW_D;
    y += CHOCO_FRAME_SHADOW_D;
    for (i = 0; i < 2; i++) {
        s32 k = s_menuChocographFrameShadowPieces[i];
        setlen(g_primPtr, PRIM_LEN(POLY_FT4));
        ((Ft4W *)g_primPtr)->rgbc = GPU_RGBC(GPU_POLY_FT4 | GPU_ABE, 0x101010);
        if (k < 2) {
            ((Ft4W *)g_primPtr)->x0 = s_menuChocographFramePieces[k].x + x;
            ((Ft4W *)g_primPtr)->y0 = s_menuChocographFramePieces[k].y + y;
            ((Ft4W *)g_primPtr)->x1 = s_menuChocographFramePieces[k].x + x + s_menuChocographFramePieces[k].w;
            ((Ft4W *)g_primPtr)->y1 = s_menuChocographFramePieces[k].y + y;
            ((Ft4W *)g_primPtr)->x2 = s_menuChocographFramePieces[k].x + x;
            ((Ft4W *)g_primPtr)->y2 = s_menuChocographFramePieces[k].y + y + s_menuChocographFramePieces[k].h;
            ((Ft4W *)g_primPtr)->x3 = s_menuChocographFramePieces[k].x + x + s_menuChocographFramePieces[k].w;
            ((Ft4W *)g_primPtr)->y3 = s_menuChocographFramePieces[k].y + y + s_menuChocographFramePieces[k].h;
        } else {
            ((Ft4W *)g_primPtr)->x0 = s_menuChocographFramePieces[k].x + x;
            ((Ft4W *)g_primPtr)->y0 = s_menuChocographFramePieces[k].y + y + s_menuChocographFramePieces[k].w;
            ((Ft4W *)g_primPtr)->x1 = s_menuChocographFramePieces[k].x + x;
            ((Ft4W *)g_primPtr)->y1 = s_menuChocographFramePieces[k].y + y - 1;
            ((Ft4W *)g_primPtr)->x2 = s_menuChocographFramePieces[k].x + x + s_menuChocographFramePieces[k].h;
            ((Ft4W *)g_primPtr)->y2 = s_menuChocographFramePieces[k].y + y + s_menuChocographFramePieces[k].w;
            ((Ft4W *)g_primPtr)->x3 = s_menuChocographFramePieces[k].x + x + s_menuChocographFramePieces[k].h;
            ((Ft4W *)g_primPtr)->y3 = s_menuChocographFramePieces[k].y + y - 1;
        }
        ((Ft4W *)g_primPtr)->u0 = s_menuChocographFramePieces[k].u;
        ((Ft4W *)g_primPtr)->v0 = s_menuChocographFramePieces[k].v;
        ((Ft4W *)g_primPtr)->u1 = s_menuChocographFramePieces[k].u + s_menuChocographFramePieces[k].w;
        ((Ft4W *)g_primPtr)->v1 = s_menuChocographFramePieces[k].v;
        ((Ft4W *)g_primPtr)->u2 = s_menuChocographFramePieces[k].u;
        ((Ft4W *)g_primPtr)->v2 = s_menuChocographFramePieces[k].v + s_menuChocographFramePieces[k].h;
        ((Ft4W *)g_primPtr)->u3 = s_menuChocographFramePieces[k].u + s_menuChocographFramePieces[k].w;
        ((Ft4W *)g_primPtr)->v3 = s_menuChocographFramePieces[k].v + s_menuChocographFramePieces[k].h;
        ((Ft4W *)g_primPtr)->tpage = getTPage(0, 0, VRAM_UI_SHEET_X, VRAM_UI_SHEET_Y);
        ((Ft4W *)g_primPtr)->clut = clut + 0x40;
        addPrim(&g_ot[otz], (Ft4W *)g_primPtr);
        g_primPtr += sizeof(Ft4W);
    }
}

void menuChocograph_update(void) {
    s_menuChocograph.textPtr = s_menuChocograph.textBuf + (g_gameState->gameData->drawBuf << 12);
    menuChocograph_input();
    menuChocograph_draw();
    menuChocograph_updateList();
}

void menuChocograph_exit(void) {
    help_setCursorOnTop(1);
    help_setCallback(0);
    menuChocograph_restoreVram();
}

void menuChocograph_input(void) {
    void (*fn)(void);

    if (!fade_isBusy()) {
        fn = s_menuChocographInputFuncs[s_menuChocograph.mode];
        if (fn != NULL) {
            fn();
        }
    }
}

void menuChocograph_draw(void) {
    menuChocograph_drawTitle();
    menuChocograph_drawTabs();
    menuChocograph_drawSelected();
    menuChocograph_drawList();
    menuChocograph_drawCounts();
    menuChocograph_drawAbilities();
    menuChocograph_drawHint();
    menuChocograph_drawCursor();
}

void menuChocograph_updateList(void) {
    if (s_menuChocograph.mode == CHOCO_MODE_LIST) {
        menuList_update(&s_menuChocograph.list);
    }
}

void menuChocograph_drawTitle(void) {
    RECT *rect = &s_menuChocograph.titleRect;

    win_drawBox(rect, CHOCO_OTZ_WIN, WIN_STYLE_BEVEL);
    win_drawShadow(rect, CHOCO_OTZ_WIN, WIN_STYLE_BEVEL);
    text_drawBlock(rect->x + CHOCO_TITLE_TEXT_X, rect->y + CHOCO_TEXT_Y, CHOCO_OTZ_TEXT, text_get(CHOCO_TEXT_TITLE), TEXT_PAL_WHITE);
}

void menuChocograph_drawTabs(void) {
    RECT *rect = &s_menuChocograph.tabRect;
    s32 i;
    s32 x;
    u8 none = g_gameState->gameData->chocograph == 0;

    win_drawBox(rect, CHOCO_OTZ_WIN, WIN_STYLE_BEVEL);
    win_drawShadow(rect, CHOCO_OTZ_WIN, WIN_STYLE_BEVEL);
    for (i = 0, x = CHOCO_TAB_TEXT_X; i < 2; i++, x += CHOCO_TAB_W) {
        text_drawBlock(rect->x + x, rect->y + CHOCO_TEXT_Y, CHOCO_OTZ_TEXT, text_get(i + CHOCO_TEXT_SELECT), (s_menuChocographTabNeedsSel[i] != 0 && none) ? TEXT_PAL_GREY : TEXT_PAL_WHITE);
    }
}

void menuChocograph_drawAbilities(void) {
    s32 i;
    s32 x;
    RECT *rect = &s_menuChocograph.abilityRect;

    ui_drawLabel(rect->x + CHOCO_LABEL_X, rect->y + WIN_TITLE_Y, CHOCO_OTZ_TEXT, g_labelChocoboAbility, UI_LABEL_NORMAL);
    win_drawBox(rect, CHOCO_OTZ_WIN, WIN_STYLE_BEVEL);
    win_drawShadow(rect, CHOCO_OTZ_SHADOW, WIN_STYLE_BEVEL);
    for (i = 0; i < s_menuChocograph.ability; i++) {
        menuChocograph_drawIcon(rect->x + CHOCO_ABIL_ICON_X + i * CHOCO_ABIL_ICON_W, rect->y + CHOCO_ABIL_ICON_Y, CHOCO_OTZ_TEXT, i, 0);
    }
}

void menuChocograph_drawScrollRows(void) {
    MenuView view;
    s32 i;

    menuList_getPageView(&s_menuChocograph.list, &view);
    for (i = 0; i < view.rows; i++) {
        menuChocograph_drawRow(0, i, view.first + i, &view);
    }
}

void menuChocograph_drawRow(s32 col, s32 row, s32 id, MenuView *view) {
    u8 got;
    u8 near;
    s32 y;
    s32 icon;
    s32 k = CHOCO_LIST_WIN_Y;
    s32 x;

    if (s_menuChocograph.owned & (1 << id)) {
        got = (s_menuChocograph.found & (1 << id)) != 0;
        near = menuChocograph_getRequiredAbility(id) < s_menuChocograph.ability;
        x = CHOCO_ROW_ICON_X;
        y = row * CHOCO_ROW_H + (view->y + k);
        if (got) {
            icon = near ? CHOCO_ICON_CHOCOGRAPH + 1 : CHOCO_ICON_CHOCOGRAPH + 3;
        } else {
            icon = near ? CHOCO_ICON_CHOCOGRAPH : CHOCO_ICON_CHOCOGRAPH + 2;
        }
        menuChocograph_drawIcon(x, y, CHOCO_OTZ_TEXT, icon, near == 0);
        text_drawBlock(x + CHOCO_ROW_NAME_DX, y, CHOCO_OTZ_TEXT, text_get(id + CHOCO_TEXT_NAME), near ? TEXT_PAL_WHITE : TEXT_PAL_GREY);
    }
}

void menuChocograph_drawIcon(s32 x, s32 y, s32 z, s32 icon, u8 flag) {
    IconTable rects = s_menuChocographIcons;

    ui_drawSpriteDef(x, y, z, &rects.r[icon], flag);
}

s32 menuChocograph_getRequiredAbility(s32 id) {
    return menuChocograph_getRequiredAbility_inline(id);
}

void menuChocograph_scrollTo(s32 idx) {
    MenuList *list = &s_menuChocograph.list;

    if (idx > CHOCO_LAST_PAGE - 1) {
        list->top = CHOCO_LAST_PAGE;
        list->cursor = idx - CHOCO_LAST_PAGE;
    } else {
        list->cursor = idx % CHOCO_LIST_ROWS;
        list->top = idx - list->cursor;
    }
}

void menuChocograph_onLoadImages(void) {
}

void menuChocograph_getPictureRects(s32 i, RECT *clut, RECT *img) {
    clut->x = 0;
    clut->y = i + CHOCO_PICTURE_CLUT_Y;
    clut->w = 0x40;
    clut->h = 1;
    img->x = (i % CHOCO_PICTURE_COLS) * (CHOCO_PICTURE_W / 2);
    img->y = (i / CHOCO_PICTURE_COLS) * CHOCO_PICTURE_H + CHOCO_PICTURE_VRAM_Y;
    img->w = CHOCO_PICTURE_W / 2;
    img->h = CHOCO_PICTURE_H;
}

INCLUDE_RODATA("asm/us/ovl_32/nonmatchings/ovl_32", s_menuChocographPictures);

INCLUDE_RODATA("asm/us/ovl_32/nonmatchings/ovl_32", s_menuChocographPictureImages);

INCLUDE_RODATA("asm/us/ovl_32/nonmatchings/ovl_32", s_menuChocographIconImage);

INCLUDE_RODATA("asm/us/ovl_32/nonmatchings/ovl_32", s_menuChocographFrameImage);
