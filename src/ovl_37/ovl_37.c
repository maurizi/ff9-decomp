#include "ovl_37.h"

INCLUDE_RODATA("asm/us/ovl_37/nonmatchings/ovl_37", g_itemMenuText);

void menuItem_init(void) {
    s_menuItem.state = ITEM_STATE_LIST;
    s_menuItem.tab = 0;
    s_menuItem.arrangeCursor = 0;
    s_menuItem.charWinOpen = 0;
    s_menuItem.swapFrom = 0;
    s_menuItem.popupTimer = 0;
    s_menuItem.keyView = 0;
    s_menuItem.keyViewPhase = 0;
    menuItem_onInit();
    menuItem_collectKeyItems();
    text_setBank(g_itemMenuText);
    help_setCallback(help_nullHandler);
    help_setCursorOnTop(1);
    menuList_init(&s_menuItem.itemList, ITEM_SLOT_COUNT - 1, ITEM_LIST_ROWS, ITEM_LIST_COLS, ITEM_LIST_WIN_H);
    menuList_init(&s_menuItem.keyList, s_menuItem.keyCount, ITEM_LIST_ROWS, ITEM_LIST_COLS, ITEM_LIST_WIN_H);
    setRECT(&s_menuItem.cmdRect, ITEM_TABS_WIN_X, ITEM_TABS_WIN_Y, ITEM_TABS_WIN_W, ITEM_TABS_WIN_H);
    setRECT(&s_menuItem.listRect, ITEM_LIST_WIN_X, ITEM_LIST_WIN_Y, ITEM_LIST_WIN_W, ITEM_LIST_WIN_H);
    tween_start(&s_menuItem.charWin, 0, 0);
    menuItem_loadImages();
}

void menuItem_inputTabs(void) {
    if (g_padRepeat & BTN_CIRCLE) {
        snd_playSe(SE_CANCEL);
        menuScreen_open(0);
        return;
    }
    if (g_padRepeat & BTN_CROSS) {
        snd_playSe(SE_CURSOR);
        if (s_menuItem.tab != ITEM_TAB_ARRANGE) {
            s_menuItem.state = ITEM_STATE_LIST;
            return;
        }
        menuItem_openArrange();
        return;
    }
    if (g_padRepeat & PAD_LEFT) {
        snd_playSe(SE_CURSOR);
        s_menuItem.tab = s_menuItem.tab != 0 ? s_menuItem.tab - 1 : ITEM_TAB_KEY;
        return;
    }
    if (g_padRepeat & PAD_RIGHT) {
        snd_playSe(SE_CURSOR);
        s_menuItem.tab = s_menuItem.tab >= ITEM_TAB_KEY ? 0 : s_menuItem.tab + 1;
    }
}

void menuItem_inputTarget(void) {
    ItemList *it = &s_menuItemTabs[s_menuItem.tab];
    MenuList *list = it->list;
    s32 i;
    u8 cur;
    s32 next;

    if (s_menuItem.charWin.anim == 0) {
        if (g_padRepeat & BTN_CIRCLE) {
            snd_playSe(SE_CANCEL);
            menuItem_closeTarget();
            return;
        }
        if (g_padRepeat & BTN_CROSS) {
            i = (list->top + list->cursor) * 2 + list->col;
            menuItem_useOnTarget(g_gameState->gameData->items.slots[i].id);
            if (g_gameState->gameData->items.slots[i].count == 0) {
                menuItem_closeTarget();
            }
        } else if (g_padRepeat & PAD_UP) {
            cur = s_menuItem.slot;
            next = party_getPrevMember(cur);
            if (cur != next) {
                s_menuItem.slot = next;
                snd_playSe(SE_CURSOR);
            }
        } else if (g_padRepeat & PAD_DOWN) {
            cur = s_menuItem.slot;
            next = party_getNextMember(cur);
            if (cur != next) {
                s_menuItem.slot = next;
                snd_playSe(SE_CURSOR);
            }
        }
    }
}

void menuItem_inputManualArrange(void) {
    MenuList *list = s_menuItemTabs[1].list;
    ItemSlot swap;
    s32 i;
    u8 j;

    if (list->scroll == 0) {
        if (g_padRepeat & BTN_CIRCLE) {
            snd_playSe(SE_CANCEL);
            s_menuItem.state = ITEM_STATE_ARRANGE;
            return;
        }
        if (g_padRepeat & BTN_CROSS) {
            if (s_menuItem.swapPicked == 0) {
                snd_playSe(SE_CURSOR);
                j = list->col + (list->cursor + list->top) * 2;
                s_menuItem.swapPicked = 1;
                s_menuItem.swapFrom = j;
                return;
            }
            snd_playSe(SE_CURSOR);
            i = (list->cursor + list->top) * 2 + list->col;
            swap = g_gameState->gameData->items.slots[s_menuItem.swapFrom];
            g_gameState->gameData->items.slots[s_menuItem.swapFrom] = g_gameState->gameData->items.slots[i];
            g_gameState->gameData->items.slots[i] = swap;
            s_menuItem.swapPicked = 0;
            return;
        }
        menuList_input(list, g_padRepeat);
        menuList_left(list, g_padRepeat);
        menuList_right(list, g_padRepeat);
    }
}

void menuItem_selectItem(s32 idx, MenuList *list) {
    ItemSlot *slot;
    ItemInfo *it;
    Unk8006B1F4 *usable;
    u8 ok;
    s32 id;

    slot = &g_gameState->gameData->items.slots[idx];
    ok = 0;
    if (slot->count != 0 && slot->id != ITEM_NONE && slot->id >= ITEM_FIRST_USABLE) {
        id = slot->id;
        it = &g_itemTable[id];
        usable = &g_usableItemTable[id - ITEM_FIRST_USABLE];
        if (it->slots & ITEM_SLOT_USABLE) {
            s_menuItem.targetKind = usable->targetKind;
            ok = 1;
            s_menuItem.targetMode = slot->id != ITEM_GYSAHL_GREENS ? usable->targetMode : 0;
            if (s_menuItem.targetMode == 0) {
                menuItem_useOnTarget(slot->id);
            } else {
                menuItem_openTarget();
            }
        }
    }
    if (!ok) {
        snd_playSe(SE_BUZZER);
    }
}

void menuItem_drawList(void) {
    MenuView view;
    ItemList *il;
    s32 w;
    s32 row;
    s32 col;
    s32 idx;

    il = &s_menuItemTabs[s_menuItem.tab];
    ui_drawLabel(s_menuItemListRect->x + ITEM_LIST_LABEL_X, s_menuItemListRect->y + WIN_TITLE_Y, ITEM_OTZ_WIN, g_winLabelStrings.itemList, UI_LABEL_NORMAL);
    ui_drawLabel(s_menuItemListRect->x + (ITEM_LIST_LABEL_X + ITEM_COL_W), s_menuItemListRect->y + WIN_TITLE_Y, ITEM_OTZ_WIN, g_winLabelStrings.itemList, UI_LABEL_NORMAL);
    if (s_menuItem.tab != ITEM_TAB_KEY) {
        w = ui_measureLabel((u8 *)g_winLabelStrings.itemCount);
        ui_drawLabel((s_menuItemListRect->x + ITEM_COUNT_LABEL_R) - w, s_menuItemListRect->y + WIN_TITLE_Y, ITEM_OTZ_WIN, g_winLabelStrings.itemCount, UI_LABEL_NORMAL);
        ui_drawLabel((s_menuItemListRect->x + (ITEM_COUNT_LABEL_R + ITEM_COL_W)) - w, s_menuItemListRect->y + WIN_TITLE_Y, ITEM_OTZ_WIN, g_winLabelStrings.itemCount, UI_LABEL_NORMAL);
    }
    menuList_getView(il->list, &view);
    win_drawListBox(s_menuItemListRect, ITEM_OTZ_WIN, 1, il->list);
    win_drawShadow(s_menuItemListRect, ITEM_OTZ_WIN, WIN_STYLE_LINE_RULE);
    idx = view.first * 2;
    win_setClip(NULL, ITEM_OTZ_TEXT, WIN_STYLE_LINE_RULE);
    for (row = 0; row < view.rows; row++) {
        for (col = 0; col < ITEM_LIST_COLS; col++) {
            il->drawRow(col, row, idx++, &view);
        }
    }
    if (il->list->scroll & MENULIST_SCROLL_PAGE) {
        menuList_getRowsRect(il->list, s_menuItemListRect, &s_menuItemScrollClip);
        win_setClip(&s_menuItemScrollClip, ITEM_OTZ_TEXT, WIN_STYLE_LINE_RULE);
        menuItem_drawScrollRows();
    }
    win_setClip(s_menuItemListRect, ITEM_OTZ_TEXT, WIN_STYLE_LINE_RULE);
}

void menuItem_drawCursor(void) {
    MenuList *list;
    s32 off;
    u8 blink;
    s32 firstRow;
    s32 k;
    u8 swapIdx;

    off = 0;
    list = s_menuItemTabs[s_menuItem.tab].list;
    blink = (g_menuScreenFrame & 3) != 0;
    help_setCursorOnTop(s_menuItem.state != ITEM_STATE_TABS);
    if (fade_isBusy() || s_menuItem.state == ITEM_STATE_BUSY) {
        return;
    }
    if (s_menuItem.state == ITEM_STATE_TABS || blink) {
        g_menuHelp.x = s_menuItemTabCursorX[s_menuItem.tab] + ITEM_TAB_CURSOR_DX;
        g_menuHelp.y = ITEM_TAB_CURSOR_Y;
        g_menuHelp.otz = ITEM_OTZ_CURSOR;
        help_drawCursor();
    }
    if (s_menuItem.state == ITEM_STATE_ARRANGE || (s_menuItem.state == ITEM_STATE_MANUAL && blink)) {
        g_menuHelp.x = ITEM_ARRANGE_CURSOR_X;
        g_menuHelp.otz = ITEM_OTZ_POPUP_CURSOR;
        g_menuHelp.y = s_menuItem.arrangeCursor * ITEM_ROW_H + ITEM_ARRANGE_CURSOR_Y;
        help_drawCursor();
    }
    if (s_menuItem.state >= ITEM_STATE_LIST && s_menuItem.state != ITEM_STATE_ARRANGE && (s_menuItem.state == ITEM_STATE_LIST || blink)) {
        k = list->col ^ ((list->scroll >> 3) & 1);
        g_menuHelp.otz = ITEM_OTZ_CURSOR;
        /* Both arms are identical (jump2 merges them after register allocation); the test keeps the
         * hi part of &s_menuItem and the constant 2 live, which the original's registers need. */
        if (s_menuItem.tab == ITEM_TAB_KEY) {
            g_menuHelp.x = k * ITEM_COL_W;
            g_menuHelp.y = list->cursor * ITEM_ROW_H + ITEM_LIST_CURSOR_Y;
            help_drawCursor();
        } else {
            g_menuHelp.x = k * ITEM_COL_W;
            g_menuHelp.y = list->cursor * ITEM_ROW_H + ITEM_LIST_CURSOR_Y;
            help_drawCursor();
        }
    }
    if (s_menuItem.state == ITEM_STATE_TARGET) {
        g_menuHelp.otz = ITEM_OTZ_POPUP_CURSOR;
        g_menuHelp.x = s_menuItem.charWin.rect.x + ITEM_TARGET_CURSOR_X;
        g_menuHelp.y = s_menuItem.charWin.rect.y + (s16)(s_menuItem.slot * ITEM_TARGET_ROW_H + ITEM_TARGET_CURSOR_Y);
        help_drawCursor();
    }
    if (s_menuItem.state == ITEM_STATE_MANUAL) {
        if (s_menuItem.swapPicked == 1 && blink && !(list->scroll & MENULIST_SCROLL_PAGE)) {
            firstRow = list->top;
            if (s_menuItem.swapFrom >= firstRow * ITEM_LIST_COLS && s_menuItem.swapFrom < (firstRow + ITEM_LIST_ROWS) * ITEM_LIST_COLS) {
                if (list->scroll != 0) {
                    off = list->scrollPos;
                    if (!(list->scroll & MENULIST_SCROLL_UP)) {
                        off = -off;
                    }
                }
                swapIdx = s_menuItem.swapFrom;
                g_menuHelp.x = (swapIdx & 1) * ITEM_COL_W + ITEM_SWAP_CURSOR_X;
                g_menuHelp.y = ((swapIdx >> 1) - list->top) * ITEM_ROW_H + (s16)(off + ITEM_SWAP_CURSOR_Y);
                g_menuHelp.otz = ITEM_OTZ_POPUP_CURSOR;
                help_drawCursor();
            }
        }
        k = list->col ^ ((list->scroll >> 3) & 1);
        g_menuHelp.x = k * ITEM_COL_W;
        g_menuHelp.y = list->cursor * ITEM_ROW_H + ITEM_LIST_CURSOR_Y;
        g_menuHelp.otz = ITEM_OTZ_POPUP_CURSOR;
        help_drawCursor();
    }
}

void menuItem_openTarget(void) {
    ItemMenu *menu;
    RECT *rect;
    GameState *gs;
    s32 k = (s_menuItemTabs[s_menuItem.tab].list->col ^ 1) & 1;

    s_menuItem.charWin.rect.x = (u8)k == 0 ? ITEM_TARGET_WIN_X_LEFT : ITEM_TARGET_WIN_X_RIGHT;
    rect = &s_menuItem.charWin.rect;
    rect->y = ITEM_TARGET_WIN_Y;
    rect->w = ITEM_TARGET_WIN_W;
    rect->h = ITEM_TARGET_WIN_H;
    tween_start(&s_menuItem.charWin, s_menuItemTargetOpen[k], 1);
    gs = g_gameState;
    menu = &s_menuItem;
    menu->slot = 0;
loop:
    if (gs->gameData->party[menu->slot] == NULL) {
        if (++menu->slot < PARTY_SIZE) {
            goto loop;
        }
    }
    menu->state = ITEM_STATE_BUSY;
    menu->charWinOpen = 1;
    snd_playSe(SE_CURSOR);
}

void menuItem_sortAuto(void) {
    s32 count;
    s32 i;
    s32 j;

    count = 0;
    for (i = 0; i < ITEM_SLOT_COUNT; i++) {
        if (g_gameState->gameData->items.slots[i].count != 0 && g_gameState->gameData->items.slots[i].id != ITEM_NONE) {
            s_menuItemSortBuf[count++] = g_gameState->gameData->items.slots[i];
        }
    }
    for (i = 0; i < count - 1; i++) {
        for (j = i + 1; j < count; j++) {
            if (g_itemTable[s_menuItemSortBuf[i].id].sortKey > g_itemTable[s_menuItemSortBuf[j].id].sortKey) {
                s_menuItemSortTmp = s_menuItemSortBuf[i];
                s_menuItemSortBuf[i] = s_menuItemSortBuf[j];
                s_menuItemSortBuf[j] = s_menuItemSortTmp;
            } else if (g_itemTable[s_menuItemSortBuf[i].id].sortKey == g_itemTable[s_menuItemSortBuf[j].id].sortKey &&
                       s_menuItemSortBuf[j].id < s_menuItemSortBuf[i].id) {
                s_menuItemSortTmp = s_menuItemSortBuf[i];
                s_menuItemSortBuf[i] = s_menuItemSortBuf[j];
                s_menuItemSortBuf[j] = s_menuItemSortTmp;
            }
        }
    }
    for (i = 0; i < count; i++) {
        g_gameState->gameData->items.slots[i] = s_menuItemSortBuf[i];
    }
    for (i = count; i < ITEM_SLOT_COUNT; i++) {
        g_gameState->gameData->items.slots[i].count = 0;
    }
    s_menuItem.itemList.col = 0;
    s_menuItem.itemList.cursor = 0;
    s_menuItem.itemList.top = 0;
}

s32 menuItem_callChocobo(void) {
    ItemMenu *menu = &s_menuItem;

    if (g_gameState->mode == GS_MODE_WORLD && g_evtGlobalVars.b[EVT_GB_CHOCOBO_LEVEL] != 0 && g_evtGlobalVars.b[0xB5] == 0) {
        if (wm_canCallChocobo()) {
            g_evtGlobalVars.b[0xB5] = 1;
            wm_callChocobo();
            menu->popup = win_openFree(&s_menuItemKwehRect, ITEM_OTZ_KWEH, WIN_STYLE_BEVEL);
            win_setOnOpened(menu->popup, menuItem_onKwehOpened);
            win_setDrawContent(menu->popup, menuItem_drawKweh);
            win_setTitle(menu->popup, (s32)s_menuItemChocoboTitle);
            win_setShadow(menu->popup, 1);
            menu->popupTimer = 1;
            menu->state = ITEM_STATE_BUSY;
            help_setLocked(1);
            return 1;
        }
        return 0;
    }
    return 0;
}

void menuItem_updateHelp(void) {
    RECT bounds;
    MenuList *list;
    ItemSlot *slot;
    s32 i;
    s16 x;
    s16 y;
    s32 z;
    HelpState *h;

    h = &g_menuHelp;
    x = h->x;
    y = h->y;
    z = h->otz;
    h->text = NULL;
    h->x = x;
    h->y = y;
    h->otz = z;
    help_setBounds(NULL);
    if (s_menuItem.state == ITEM_STATE_TABS) {
        h->text = text_get(s_menuItem.tab + ITEM_TEXT_TAB_HELP);
    }
    if (s_menuItem.state == ITEM_STATE_LIST) {
        if (s_menuItem.tab != 0) {
            i = menuItem_getSlot(&s_menuItem.keyList);
            if (i < s_menuItem.keyCount) {
                h->text = &s_menuItemKeyHelp[g_keyItemTable[s_menuItem.keyItems[i]].help];
            }
            return;
        }
        list = &s_menuItem.itemList;
    } else if (s_menuItem.state == ITEM_STATE_TARGET) {
        if (menuItem_getSlot(&s_menuItem.itemList) & 1) {
            bounds.x = 0xC;
            bounds.y = 0;
            bounds.w = g_gameState->dispEnv->disp.w;
            bounds.h = g_gameState->dispEnv->disp.h;
        } else {
            bounds.x = 0;
            bounds.y = 0;
            bounds.w = g_gameState->dispEnv->disp.w >> 1;
            bounds.h = g_gameState->dispEnv->disp.h;
        }
        help_setBounds(&bounds);
        g_menuHelp.text = text_get(ITEM_TEXT_SELECT_CHAR);
        return;
    } else if (s_menuItem.state == ITEM_STATE_ARRANGE) {
        help_setBounds(&s_menuItemArrangeHelpBounds);
        h->text = text_get(s_menuItem.arrangeCursor + ITEM_TEXT_ARRANGE_HELP);
        return;
    } else if (s_menuItem.state == ITEM_STATE_MANUAL) {
        list = s_menuItemTabs[1].list;
    } else {
        return;
    }
    slot = &g_gameState->gameData->items.slots[menuItem_getSlot(list)];
    if (slot->count != 0) {
        h->text = &s_menuItemHelp[g_itemTable[slot->id].help];
    }
}

void menuItem_updateKeyItemAnim(void) {
    if (s_menuItem.keyView != 0 && s_menuItem.keyViewPhase != 0) {
        if (s_menuItem.keyViewPhase == KEY_VIEW_OPENING) {
            s_menuItem.keyViewX += s_menuItemKeyItemSlide[s_menuItem.keyViewStep++];
            if (s_menuItem.keyViewStep >= KEY_VIEW_OPEN_STEPS) {
                s_menuItem.keyViewPhase = KEY_VIEW_SHOWN;
                s_menuItem.keyViewStep = 0;
                s_menuItem.state = ITEM_STATE_KEY_VIEW;
            }
        } else if (s_menuItem.keyViewPhase == KEY_VIEW_CLOSING) {
            s_menuItem.keyViewX += s_menuItemKeyItemSlide[s_menuItem.keyViewStep++];
            if (s_menuItem.keyViewStep >= KEY_VIEW_CLOSE_STEPS) {
                s_menuItem.keyViewStep = 0;
                s_menuItem.keyViewPhase = 0;
                s_menuItem.keyView = 0;
                s_menuItem.state = ITEM_STATE_LIST;
                keyItem_setSeen(s_menuItem.keyViewId);
                help_setLocked(0);
            }
        }
    }
}

void menuItem_drawKeyItem(void) {
    ItemMenu *menu = &s_menuItem;
    s32 x;
    u8 *name;

    if (menu->keyView != 0) {
        x = menu->keyViewX;
        name = g_keyItemNames + g_keyItemTable[menu->keyViewId].name;
        text_drawBlock(x + ((KEY_VIEW_W - text_measureWidth((s32)name)) >> 1), KEY_VIEW_NAME_Y, ITEM_OTZ_POPUP, name, TEXT_PAL_HELP);
        text_drawBlock(x + KEY_VIEW_DESC_X, KEY_VIEW_DESC_Y, ITEM_OTZ_POPUP, s_menuItemKeyDesc + g_keyItemTable[menu->keyViewId].desc, TEXT_PAL_HELP);
        setlen(g_primPtr, PRIM_LEN(SPRT));
        ((SprtWords *)g_primPtr)->rgbc = GPU_RGBC(GPU_SPRT, GPU_RGB_NEUTRAL);
        ((SprtWords *)g_primPtr)->xy = (x & 0xFFFF) | (KEY_VIEW_Y << 16);
        ((SprtWords *)g_primPtr)->uvclut = (GetClut(KEY_VIEW_CLUT_X, KEY_VIEW_CLUT_Y) << 16) | (KEY_VIEW_V << 8);
        ((SprtWords *)g_primPtr)->wh = (KEY_VIEW_H << 16) | KEY_VIEW_W;
        addPrim(&g_ot[ITEM_OTZ_POPUP], g_primPtr);
        g_primPtr += sizeof(SprtWords);
        setlen(g_primPtr, PRIM_LEN(SPRT));
        ((SprtWords *)g_primPtr)->rgbc = GPU_CODE(GPU_SPRT | GPU_ABE);
        ((SprtWords *)g_primPtr)->xy = ((x + KEY_VIEW_SHADOW_D) & 0xFFFF) | ((KEY_VIEW_Y + KEY_VIEW_H - KEY_VIEW_SHADOW_D) << 16);
        ((SprtWords *)g_primPtr)->uvclut = (GetClut(KEY_VIEW_CLUT_X, KEY_VIEW_CLUT_Y + 1) << 16) | 0xFA00;
        ((SprtWords *)g_primPtr)->wh = (KEY_VIEW_SHADOW_W << 16) | KEY_VIEW_W;
        addPrim(&g_ot[ITEM_OTZ_POPUP], g_primPtr);
        g_primPtr += sizeof(SprtWords);
        setlen(g_primPtr, PRIM_LEN(SPRT));
        ((SprtWords *)g_primPtr)->rgbc = GPU_CODE(GPU_SPRT | GPU_ABE);
        ((SprtWords *)g_primPtr)->xy = ((x + (KEY_VIEW_W - KEY_VIEW_SHADOW_D)) & 0xFFFF) | ((KEY_VIEW_Y + KEY_VIEW_SHADOW_D) << 16);
        ((SprtWords *)g_primPtr)->uvclut = (GetClut(KEY_VIEW_CLUT_X, KEY_VIEW_CLUT_Y + 1) << 16) | 0x80AA;
        ((SprtWords *)g_primPtr)->wh = ((KEY_VIEW_H - KEY_VIEW_SHADOW_W) << 16) | KEY_VIEW_SHADOW_W;
        addPrim(&g_ot[ITEM_OTZ_POPUP], g_primPtr);
        g_primPtr += sizeof(SprtWords);
        SetDrawTPage(g_primPtr, 1, 1, getTPage(0, 0, VRAM_FONT_X, VRAM_FONT_Y));
        addPrim(&g_ot[ITEM_OTZ_POPUP], g_primPtr);
        g_primPtr += sizeof(DR_TPAGE);
    }
}

void menuItem_update(void) {
    menuItem_input();
    menuItem_draw();
    menuItem_updateAnims();
}

void menuItem_exit(void) {
    if (g_gameState->mode == GS_MODE_WORLD) {
        snd_stopSe(SE_CHOCOBO_CALL);
    }
    help_setLocked(0);
    help_setCursorOnTop(1);
    help_setCallback(NULL);
    menuItem_restoreVram();
}

void menuItem_input(void) {
    void (*fn)(void);

    if (!fade_isBusy()) {
        fn = s_menuItemInput[s_menuItem.state];
        if (fn != NULL) {
            fn();
        }
    }
}

void menuItem_draw(void) {
    menuItem_drawTabs();
    menuItem_drawList();
    menuItem_drawTargetWindow();
    menuItem_drawKeyItem();
    menuItem_drawCursor();
}

void menuItem_updateAnims(void) {
    menuItem_updateHelp();
    menuItem_updateKeyItemAnim();
    menuItem_stepKweh();
    tween_step(&s_menuItem.charWin);
    menuList_update(&s_menuItem.itemList);
    menuList_update(&s_menuItem.keyList);
}

void menuItem_inputList(void) {
    ItemList *it = &s_menuItemTabs[s_menuItem.tab];
    MenuList *list = it->list;

    if (list->scroll == 0) {
        if (g_padRepeat & BTN_CIRCLE) {
            snd_playSe(SE_CANCEL);
            s_menuItem.state = ITEM_STATE_TABS;
            return;
        }
        if (g_padRepeat & BTN_CROSS) {
            menuItem_selectRow();
            return;
        }
        if (list->count != 0) {
            menuList_input(list, g_padRepeat);
            if (list->scroll == 0) {
                menuList_left(list, g_padRepeat);
                if (list->scroll == 0) {
                    menuList_right(list, g_padRepeat);
                }
            }
        }
    }
}

void menuItem_inputArrange(void) {
    if (g_padRepeat & BTN_CIRCLE) {
        snd_playSe(SE_CANCEL);
        menuItem_closeArrange();
        return;
    }
    if (g_padRepeat & (PAD_UP | PAD_DOWN)) {
        snd_playSe(SE_CURSOR);
        s_menuItem.arrangeCursor ^= 1;
        return;
    }
    if (g_padRepeat & BTN_CROSS) {
        if (s_menuItem.arrangeCursor == 0) {
            snd_playSe(SE_CURSOR);
            menuItem_sortAuto();
            return;
        }
        snd_playSe(SE_CURSOR);
        s_menuItem.swapPicked = 0;
        s_menuItem.state = ITEM_STATE_MANUAL;
    }
}

void menuItem_inputKeyItem(void) {
    if (g_padRepeat & BTN_CIRCLE) {
        snd_playSe(SE_CANCEL);
        menuItem_hideKeyItem();
    } else if (g_padRepeat & BTN_CROSS) {
        snd_playSe(SE_CURSOR);
        menuItem_hideKeyItem();
    }
}

void menuItem_selectRow(void) {
    ItemList *it = &s_menuItemTabs[s_menuItem.tab];
    MenuList *list = it->list;
    void (*selectFn)(s32, MenuList *) = it->select;
    s32 idx = (list->top + list->cursor) * 2 + list->col;

    do {
        selectFn(idx, list);
    } while (0);
}

void menuItem_selectKeyItem(s32 i) {
    if (i < s_menuItem.keyCount) {
        menuItem_showKeyItem(s_menuItem.keyItems[i]);
        snd_playSe(SE_CURSOR);
    } else {
        snd_playSe(SE_BUZZER);
    }
}

void menuItem_useOnTarget(s32 id) {
    Unk8006B1F4 *usable;
    Character *chr;
    s32 i;

    i = id - ITEM_FIRST_USABLE;
    usable = &g_usableItemTable[i];
    chr = g_gameState->gameData->party[s_menuItem.slot];
    if (item_useOnMember(chr, chr, usable, usable->effect, 0)) {
        snd_playSe(SE_USE_ITEM);
        item_remove(id, 1);
    } else {
        snd_playSe(SE_BUZZER);
    }
}

void menuItem_drawTabs(void) {
    RECT *rect;
    s32 i;

    rect = &s_menuItem.cmdRect;
    for (i = 0; i < (s32)ARRAY_COUNT(s_menuItemTabX); i++) {
        text_drawBlock(rect->x + s_menuItemTabX[i], rect->y + ITEM_TAB_TEXT_Y, ITEM_OTZ_TEXT, text_get(s_menuItemTabText[i]), TEXT_PAL_WHITE);
    }
    win_drawBox(rect, ITEM_OTZ_WIN, WIN_STYLE_BEVEL);
    win_drawShadow(rect, ITEM_OTZ_WIN, WIN_STYLE_BEVEL);
}

void menuItem_drawScrollRows(void) {
    MenuView view;
    ItemList *it;
    s32 row;
    s32 col;
    s32 idx;

    it = &s_menuItemTabs[s_menuItem.tab];
    row = 0;
    menuList_getPageView(it->list, &view);
    idx = view.first * 2;
    for (; row < view.rows; row++) {
        for (col = 0; col < ITEM_LIST_COLS; col++) {
            it->drawRow(col, row, idx++, &view);
        }
    }
}

void menuItem_drawTargetWindow(void) {
    RECT *rect = &s_menuItem.charWin.rect;
    s32 i;

    if (s_menuItem.charWinOpen != 0) {
        for (i = 0; i < PARTY_SIZE; i++) {
            if (g_gameState->gameData->party[i] != NULL) {
                ui_drawMemberSummary(rect->x + ITEM_TARGET_ROW_X, rect->y + ITEM_TARGET_ROW_Y + i * ITEM_TARGET_ROW_H, ITEM_OTZ_POPUP, i, s_menuItem.targetMode);
            }
        }
        win_drawBox(rect, ITEM_OTZ_POPUP, WIN_STYLE_BEVEL);
        win_drawShadow(rect, ITEM_OTZ_POPUP, WIN_STYLE_BEVEL);
    }
}

void menuItem_drawRow(s32 col, s32 row, s32 i, MenuView *view) {
    ItemSlot *slot = &g_gameState->gameData->items.slots[i];
    ItemInfo *it;
    u8 ok;
    s32 x, y, x2;
    s32 k = ITEM_LIST_WIN_Y;
    u8 numText[8];

    if (slot->count != 0 && slot->id != ITEM_NONE) {
        it = &g_itemTable[slot->id];
        ok = it->slots & ITEM_SLOT_USABLE;
        x = col * ITEM_COL_W;
        y = row * ITEM_ROW_H + (view->y + k);
        text_formatNumber(slot->count, numText, 2, TEXT_NUM_ASCII);
        x2 = x + ITEM_ICON_X;
        text_drawBlock(x + ITEM_NAME_X, y, ITEM_OTZ_TEXT, g_itemNames + it->name, ok ? TEXT_PAL_WHITE : TEXT_PAL_GREY);
        ui_drawItemIcon(x2, y, ITEM_OTZ_TEXT, slot->id, ok);
        ui_drawSmallDigits(x2 + ITEM_COUNT_DX, y + ITEM_COUNT_DY, ITEM_OTZ_TEXT, numText, ok ? TEXT_PAL_WHITE : TEXT_PAL_GREY);
    }
}

void menuItem_drawKeyRow(s32 col, s32 row, s32 i, MenuView *view) {
    u8 blink = (u32)g_menuFrame % ITEM_NEW_BLINK_PERIOD >= ITEM_NEW_BLINK_OFF;
    u8 id;
    s32 x, y;
    s32 k = ITEM_LIST_WIN_Y;
    u8 *name;
    s32 w;
    KeyItemInfo *info;
    u8 got;

    if (i < s_menuItem.keyCount) {
        id = s_menuItem.keyItems[i];
        info = &g_keyItemTable[id];
        got = keyItem_isSeen(id);
        x = col * ITEM_COL_W + ITEM_ICON_X;
        y = row * ITEM_ROW_H + (view->y + k);
        name = g_keyItemNames + info->name;
        if (!got) {
            w = x + text_measureWidth((s32)name);
            ui_drawSprite(w + 5, y + 2, ITEM_OTZ_TEXT, UI_SPRITE_NEW_TEXT, blink);
            ui_drawSprite(w - 1, y - 1, ITEM_OTZ_TEXT, UI_SPRITE_NEW_MARK, 0);
        }
        text_drawBlock(x, y, ITEM_OTZ_TEXT, name, TEXT_PAL_WHITE);
    }
}

void menuItem_onKwehOpened(Window *w) {
    if (g_gameState->mode == GS_MODE_WORLD) {
        snd_startSe(SE_CHOCOBO_CALL);
    }
}

void menuItem_drawKweh(Window *w) {
    u8 *msg;

    msg = text_get(ITEM_TEXT_KWEH);
    text_drawBlock(w->x + ((w->w - text_measureWidth((s32)msg)) >> 1), w->y + 6, w->otz, msg, TEXT_PAL_WHITE);
}

void menuItem_stepKweh(void) {
    u32 timer;

    if (s_menuItem.state == ITEM_STATE_BUSY) {
        timer = s_menuItem.popupTimer;
        if (timer != 0) {
            s_menuItem.popupTimer = timer + 1;
            if (timer >= ITEM_KWEH_FRAMES) {
                s_menuItem.popupTimer = 0;
                if (g_gameState->mode == GS_MODE_WORLD) {
                    snd_stopSe(SE_CHOCOBO_CALL);
                }
                help_setLocked(0);
                help_setCursorOnTop(1);
                help_setCallback(NULL);
                menuItem_restoreVram();
                menuScreen_close();
            }
        }
    }
}

void menuItem_closeTarget(void) {
    ItemList *it;
    ItemMenu *menu;

    it = s_menuItemTabs;
    menu = &s_menuItem;
    tween_start(&menu->charWin, s_menuItemTargetClose[it[s_menuItem.tab].list->col & 1], 0);
    menu->state = ITEM_STATE_BUSY;
}

void menuItem_onTargetOpened(void) {
    s_menuItem.state = ITEM_STATE_TARGET;
}

void menuItem_onTargetClosed(void) {
    s_menuItem.state = ITEM_STATE_LIST;
    s_menuItem.charWinOpen = 0;
}

void menuItem_collectKeyItems(void) {
    s32 count;
    s32 i;

    count = 0;
    for (i = 0; i < KEY_ITEM_COUNT; i++) {
        if ((u8)keyItem_has(i)) {
            s_menuItem.keyItems[count] = i;
            count++;
        }
    }
    s_menuItem.keyCount = count;
}

void menuItem_openArrange(void) {
    MenuList *list;

    list = s_menuItemTabs[1].list;
    s_menuItem.popup = win_openFree(&s_menuItemArrangeRect, ITEM_OTZ_POPUP, WIN_STYLE_BEVEL);
    win_setDrawContent(s_menuItem.popup, menuItem_drawArrangeWindow);
    win_setOnOpened(s_menuItem.popup, menuItem_onArrangeOpened);
    win_setOnClosed(s_menuItem.popup, menuItem_onArrangeClosed);
    win_setShadow(s_menuItem.popup, 1);
    win_setTitle(s_menuItem.popup, g_labelArrange);
    s_menuItem.arrangeCursor = 0;
    s_menuItem.swapFrom = list->col + (list->top + list->cursor) * 2;
    s_menuItem.state = ITEM_STATE_BUSY;
}

void menuItem_closeArrange(void) {
    win_close(s_menuItem.popup);
}

void menuItem_drawArrangeWindow(Window *w) {
    Window *win = w;
    s32 i;

    for (i = 0; i < 2; i++) {
        text_drawBlock(win->x + ITEM_ARRANGE_TEXT_X, win->y + ITEM_ARRANGE_TEXT_Y + i * ITEM_ROW_H, w->otz, text_get(i + ITEM_TEXT_AUTO), TEXT_PAL_WHITE);
    }
}

void menuItem_onArrangeOpened(Window *w) {
    s_menuItem.state = ITEM_STATE_ARRANGE;
}

void menuItem_onArrangeClosed(Window *w) {
    s_menuItem.state = ITEM_STATE_TABS;
}

void menuItem_loadImages(void) {
    u8 *image = s_menuItemKeyImage;

    LoadImage(&s_menuItemKeyClutRect, (u32 *)image);
    DrawSync(0);
    LoadImage(&s_menuItemKeyImageRect, (u32 *)(image + 0x40));
    DrawSync(0);
}

void menuItem_restoreVram(void) {
}

void menuItem_showKeyItem(s32 id) {
    s_menuItem.keyViewId = id;
    s_menuItem.keyView = 1;
    s_menuItem.keyViewPhase = KEY_VIEW_OPENING;
    s_menuItem.state = ITEM_STATE_BUSY;
    s_menuItem.keyViewStep = 0;
    s_menuItem.keyViewX = KEY_VIEW_X;
    s_menuItem.keyViewX -= tween_sumDeltas(KEY_VIEW_OPEN_STEPS, s_menuItemKeyItemSlide);
    help_setLocked(1);
}

void menuItem_hideKeyItem(void) {
    s_menuItem.keyViewPhase = KEY_VIEW_CLOSING;
    s_menuItem.state = ITEM_STATE_BUSY;
    s_menuItem.keyViewStep = 0;
    s_menuItem.keyViewX = KEY_VIEW_X;
}

s32 menuItem_getSlot(MenuList *list) {
    s32 curRow, wrapCol;
    s32 step;

    if (list->scroll & MENULIST_SCROLL_WRAP_COL) {
        wrapCol = list->col ^ 1;
        step = 1;
        curRow = list->cursor;
        if (list->scroll & MENULIST_SCROLL_UP) {
            return (list->top + (curRow - step)) * 2 + wrapCol;
        }
        return (list->top + (curRow + step)) * 2 + wrapCol;
    }
    return (list->top + list->cursor) * 2 + list->col;
}

void menuItem_onInit(void) {
}

INCLUDE_RODATA("asm/us/ovl_37/nonmatchings/ovl_37", s_menuItemKeyImage);
