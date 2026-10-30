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
    func_801F3038();
    menuItem_collectKeyItems();
    text_setBank(g_itemMenuText);
    help_setCallback(help_nullHandler);
    help_setCursorOnTop(1);
    menuList_init(&s_menuItem.itemList, ITEM_SLOT_COUNT - 1, 0xB, 2, 0xAD);
    menuList_init(&s_menuItem.keyList, s_menuItem.keyCount, 0xB, 2, 0xAD);
    setRECT(&s_menuItem.cmdRect, 8, 8, 0xE4, 0x1A);
    setRECT(&s_menuItem.listRect, 8, 0x2A, 0x12C, 0xAD);
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
    MenuList *l = it->list;
    s32 i;
    u8 cur;
    s32 next;

    if (s_menuItem.charWin.unkC == 0) {
        if (g_padRepeat & BTN_CIRCLE) {
            snd_playSe(SE_CANCEL);
            menuItem_closeTarget();
            return;
        }
        if (g_padRepeat & BTN_CROSS) {
            i = (l->top + l->cursor) * 2 + l->col;
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
    MenuList *l = s_menuItemTabs[1].list;
    ItemSlot tmp;
    s32 i;
    u8 j;

    if (l->scroll == 0) {
        if (g_padRepeat & BTN_CIRCLE) {
            snd_playSe(SE_CANCEL);
            s_menuItem.state = ITEM_STATE_ARRANGE;
            return;
        }
        if (g_padRepeat & BTN_CROSS) {
            if (s_menuItem.swapPicked == 0) {
                snd_playSe(SE_CURSOR);
                j = l->col + (l->cursor + l->top) * 2;
                s_menuItem.swapPicked = 1;
                s_menuItem.swapFrom = j;
                return;
            }
            snd_playSe(SE_CURSOR);
            i = (l->cursor + l->top) * 2 + l->col;
            tmp = g_gameState->gameData->items.slots[s_menuItem.swapFrom];
            g_gameState->gameData->items.slots[s_menuItem.swapFrom] = g_gameState->gameData->items.slots[i];
            g_gameState->gameData->items.slots[i] = tmp;
            s_menuItem.swapPicked = 0;
            return;
        }
        menuList_input(l, g_padRepeat);
        menuList_left(l, g_padRepeat);
        menuList_right(l, g_padRepeat);
    }
}

void menuItem_selectItem(s32 idx, MenuList *l) {
    UnkPair *p;
    Unk80069018 *it;
    Unk8006B1F4 *e;
    u8 ok;
    s32 id;

    p = &g_gameState->gameData->items.slots[idx];
    ok = 0;
    if (p->unk1 != 0 && p->unk0 != ITEM_NONE && p->unk0 >= ITEM_FIRST_USABLE) {
        id = p->unk0;
        it = &g_itemTable[id];
        e = &D_8006B1F4[id - ITEM_FIRST_USABLE];
        if (it->unkF & 1) {
            s_menuItem.targetKind = e->unk0;
            ok = 1;
            s_menuItem.targetMode = p->unk0 != 0xFB ? e->unk0_5 : 0;
            if (s_menuItem.targetMode == 0) {
                menuItem_useOnTarget(p->unk0);
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
    ui_drawLabel(s_menuItemListRect->x + 0xE, s_menuItemListRect->y - 3, 0x1D, g_winLabelStrings.unk68, 0);
    ui_drawLabel(s_menuItemListRect->x + 0xA4, s_menuItemListRect->y - 3, 0x1D, g_winLabelStrings.unk68, 0);
    if (s_menuItem.tab != ITEM_TAB_KEY) {
        w = ui_measureLabel((u8 *)g_winLabelStrings.unk94);
        ui_drawLabel((s_menuItemListRect->x + 0x8A) - w, s_menuItemListRect->y - 3, 0x1D, g_winLabelStrings.unk94, 0);
        ui_drawLabel((s_menuItemListRect->x + 0x120) - w, s_menuItemListRect->y - 3, 0x1D, g_winLabelStrings.unk94, 0);
    }
    menuList_getView(il->list, &view);
    win_drawListBox(s_menuItemListRect, 0x1D, 1, il->list);
    win_drawShadow(s_menuItemListRect, 0x1D, 1);
    idx = view.unk1 * 2;
    win_setClip(NULL, 0x1C, 1);
    for (row = 0; row < view.unk0; row++) {
        for (col = 0; col < 2; col++) {
            il->drawRow(col, row, idx++, &view);
        }
    }
    if (il->list->scroll & 4) {
        menuList_getRowsRect(il->list, s_menuItemListRect, &s_menuItemScrollClip);
        win_setClip(&s_menuItemScrollClip, 0x1C, 1);
        menuItem_drawScrollRows();
    }
    win_setClip(s_menuItemListRect, 0x1C, 1);
}

void menuItem_drawCursor(void) {
    MenuList *l;
    s32 off;
    u8 blink;
    s32 a;
    s32 k;
    u8 u;

    off = 0;
    l = s_menuItemTabs[s_menuItem.tab].list;
    blink = (g_menuScreenFrame & 3) != 0;
    help_setCursorOnTop(s_menuItem.state != ITEM_STATE_TABS);
    if (fade_isBusy() || s_menuItem.state == ITEM_STATE_BUSY) {
        return;
    }
    if (s_menuItem.state == ITEM_STATE_TABS || blink) {
        g_menuHelp.x = s_menuItemTabCursorX[s_menuItem.tab] - 0x10;
        g_menuHelp.y = 0x10;
        g_menuHelp.otz = 0x19;
        help_drawCursor();
    }
    if (s_menuItem.state == ITEM_STATE_ARRANGE || (s_menuItem.state == ITEM_STATE_MANUAL && blink)) {
        g_menuHelp.x = 0x7E;
        g_menuHelp.otz = 0x12;
        g_menuHelp.y = s_menuItem.arrangeCursor * 15 + 0xE;
        help_drawCursor();
    }
    if (s_menuItem.state >= ITEM_STATE_LIST && s_menuItem.state != ITEM_STATE_ARRANGE && (s_menuItem.state == ITEM_STATE_LIST || blink)) {
        k = l->col ^ ((l->scroll >> 3) & 1);
        g_menuHelp.otz = 0x19;
        /* Both arms are identical (jump2 merges them after register allocation); the test keeps the
         * hi part of &s_menuItem and the constant 2 live, which the original's registers need. */
        if (s_menuItem.tab == ITEM_TAB_KEY) {
            g_menuHelp.x = k * 0x96;
            g_menuHelp.y = l->cursor * 15 + 0x34;
            help_drawCursor();
        } else {
            g_menuHelp.x = k * 0x96;
            g_menuHelp.y = l->cursor * 15 + 0x34;
            help_drawCursor();
        }
    }
    if (s_menuItem.state == ITEM_STATE_TARGET) {
        g_menuHelp.otz = 0x12;
        g_menuHelp.x = s_menuItem.charWin.unk4.x - 0xA;
        g_menuHelp.y = s_menuItem.charWin.unk4.y + (s16)(s_menuItem.slot * 43 + 0x12);
        help_drawCursor();
    }
    if (s_menuItem.state == ITEM_STATE_MANUAL) {
        if (s_menuItem.swapPicked == 1 && blink && !(l->scroll & 4)) {
            a = l->top;
            if (s_menuItem.swapFrom >= a * 2 && s_menuItem.swapFrom < (a + 0xB) * 2) {
                if (l->scroll != 0) {
                    off = l->scrollPos;
                    if (!(l->scroll & 2)) {
                        off = -off;
                    }
                }
                u = s_menuItem.swapFrom;
                g_menuHelp.x = (u & 1) * 0x96 + 2;
                g_menuHelp.y = ((u >> 1) - l->top) * 15 + (s16)(off + 0x32);
                g_menuHelp.otz = 0x12;
                help_drawCursor();
            }
        }
        k = l->col ^ ((l->scroll >> 3) & 1);
        g_menuHelp.x = k * 0x96;
        g_menuHelp.y = l->cursor * 15 + 0x34;
        g_menuHelp.otz = 0x12;
        help_drawCursor();
    }
}

void menuItem_openTarget(void) {
    ItemMenu *m;
    RECT *r;
    GameState *gs;
    s32 k = (s_menuItemTabs[s_menuItem.tab].list->col ^ 1) & 1;

    s_menuItem.charWin.unk4.x = (u8)k == 0 ? 8 : 0xA4;
    r = &s_menuItem.charWin.unk4;
    r->y = 0x26;
    r->w = 0x90;
    r->h = 0xB6;
    tween_start(&s_menuItem.charWin, s_menuItemTargetOpen[k], 1);
    gs = g_gameState;
    m = &s_menuItem;
    m->slot = 0;
loop:
    if (gs->gameData->party[m->slot] == NULL) {
        if (++m->slot < 4) {
            goto loop;
        }
    }
    m->state = ITEM_STATE_BUSY;
    m->charWinOpen = 1;
    snd_playSe(SE_CURSOR);
}

void menuItem_sortAuto(void) {
    s32 n;
    s32 i;
    s32 j;

    n = 0;
    for (i = 0; i < ITEM_SLOT_COUNT; i++) {
        if (g_gameState->gameData->items.slots[i].count != 0 && g_gameState->gameData->items.slots[i].id != ITEM_NONE) {
            s_menuItemSortBuf[n++] = g_gameState->gameData->items.slots[i];
        }
    }
    for (i = 0; i < n - 1; i++) {
        for (j = i + 1; j < n; j++) {
            if (g_itemTable[s_menuItemSortBuf[i].id].unk10 > g_itemTable[s_menuItemSortBuf[j].id].unk10) {
                s_menuItemSortTmp = s_menuItemSortBuf[i];
                s_menuItemSortBuf[i] = s_menuItemSortBuf[j];
                s_menuItemSortBuf[j] = s_menuItemSortTmp;
            } else if (g_itemTable[s_menuItemSortBuf[i].id].unk10 == g_itemTable[s_menuItemSortBuf[j].id].unk10 &&
                       s_menuItemSortBuf[j].id < s_menuItemSortBuf[i].id) {
                s_menuItemSortTmp = s_menuItemSortBuf[i];
                s_menuItemSortBuf[i] = s_menuItemSortBuf[j];
                s_menuItemSortBuf[j] = s_menuItemSortTmp;
            }
        }
    }
    for (i = 0; i < n; i++) {
        g_gameState->gameData->items.slots[i] = s_menuItemSortBuf[i];
    }
    for (i = n; i < ITEM_SLOT_COUNT; i++) {
        g_gameState->gameData->items.slots[i].count = 0;
    }
    s_menuItem.itemList.col = 0;
    s_menuItem.itemList.cursor = 0;
    s_menuItem.itemList.top = 0;
}

s32 menuItem_callChocobo(void) {
    ItemMenu *m = &s_menuItem;

    if (g_gameState->mode == 3 && g_evtGlobalVars.b[0xBF] != 0 && g_evtGlobalVars.b[0xB5] == 0) {
        if (wm_canCallChocobo()) {
            g_evtGlobalVars.b[0xB5] = 1;
            wm_callChocobo();
            m->popup = win_openFree(&s_menuItemKwehRect, 0xF, 2);
            win_setOnOpened(m->popup, menuItem_onKwehOpened);
            win_setDrawContent(m->popup, menuItem_drawKweh);
            win_setTitle(m->popup, (s32)D_801F3140);
            win_setShadow(m->popup, 1);
            m->popupTimer = 1;
            m->state = ITEM_STATE_BUSY;
            help_setLocked(1);
            return 1;
        }
        return 0;
    }
    return 0;
}

void menuItem_updateHelp(void) {
    RECT r;
    MenuList *l;
    UnkPair *p;
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
                h->text = &s_menuItemKeyHelp[g_keyItemTable[s_menuItem.keyItems[i]].unk2];
            }
            return;
        }
        l = &s_menuItem.itemList;
    } else if (s_menuItem.state == ITEM_STATE_TARGET) {
        if (menuItem_getSlot(&s_menuItem.itemList) & 1) {
            r.x = 0xC;
            r.y = 0;
            r.w = g_gameState->dispEnv->disp.w;
            r.h = g_gameState->dispEnv->disp.h;
        } else {
            r.x = 0;
            r.y = 0;
            r.w = g_gameState->dispEnv->disp.w >> 1;
            r.h = g_gameState->dispEnv->disp.h;
        }
        help_setBounds(&r);
        g_menuHelp.text = text_get(ITEM_TEXT_SELECT_CHAR);
        return;
    } else if (s_menuItem.state == ITEM_STATE_ARRANGE) {
        help_setBounds(&s_menuItemArrangeHelpBounds);
        h->text = text_get(s_menuItem.arrangeCursor + ITEM_TEXT_ARRANGE_HELP);
        return;
    } else if (s_menuItem.state == ITEM_STATE_MANUAL) {
        l = s_menuItemTabs[1].list;
    } else {
        return;
    }
    p = &g_gameState->gameData->items.slots[menuItem_getSlot(l)];
    if (p->unk1 != 0) {
        h->text = &s_menuItemHelp[g_itemTable[p->unk0].unk2];
    }
}

void menuItem_updateKeyItemAnim(void) {
    if (s_menuItem.keyView != 0 && s_menuItem.keyViewPhase != 0) {
        if (s_menuItem.keyViewPhase == KEY_VIEW_OPENING) {
            s_menuItem.keyViewX += s_menuItemKeyItemSlide[s_menuItem.keyViewStep++];
            if (s_menuItem.keyViewStep >= 0x16) {
                s_menuItem.keyViewPhase = KEY_VIEW_SHOWN;
                s_menuItem.keyViewStep = 0;
                s_menuItem.state = ITEM_STATE_KEY_VIEW;
            }
        } else if (s_menuItem.keyViewPhase == KEY_VIEW_CLOSING) {
            s_menuItem.keyViewX += s_menuItemKeyItemSlide[s_menuItem.keyViewStep++];
            if (s_menuItem.keyViewStep >= 8) {
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
    ItemMenu *m = &s_menuItem;
    s32 x;
    u8 *name;

    if (m->keyView != 0) {
        x = m->keyViewX;
        name = g_keyItemNames + g_keyItemTable[m->keyViewId].unk0;
        text_drawBlock(x + ((0xB0 - text_measureWidth((s32)name)) >> 1), 0x50, 0x14, name, 8);
        text_drawBlock(x + 0xD, 0x66, 0x14, s_menuItemKeyDesc + g_keyItemTable[m->keyViewId].unk6, 8);
        setlen(g_primPtr, 4);
        ((SprtWords *)g_primPtr)->rgbc = 0x64808080;
        ((SprtWords *)g_primPtr)->xy = (x & 0xFFFF) | 0x410000;
        ((SprtWords *)g_primPtr)->uvclut = (GetClut(0x3C0, 0x1B0) << 16) | 0x8000;
        ((SprtWords *)g_primPtr)->wh = 0x8000B0;
        addPrim(&g_ot[0x14], g_primPtr);
        g_primPtr += sizeof(SprtWords);
        setlen(g_primPtr, 4);
        ((SprtWords *)g_primPtr)->rgbc = 0x66000000;
        ((SprtWords *)g_primPtr)->xy = ((x + 3) & 0xFFFF) | 0xBE0000;
        ((SprtWords *)g_primPtr)->uvclut = (GetClut(0x3C0, 0x1B1) << 16) | 0xFA00;
        ((SprtWords *)g_primPtr)->wh = 0x600B0;
        addPrim(&g_ot[0x14], g_primPtr);
        g_primPtr += sizeof(SprtWords);
        setlen(g_primPtr, 4);
        ((SprtWords *)g_primPtr)->rgbc = 0x66000000;
        ((SprtWords *)g_primPtr)->xy = ((x + 0xAD) & 0xFFFF) | 0x440000;
        ((SprtWords *)g_primPtr)->uvclut = (GetClut(0x3C0, 0x1B1) << 16) | 0x80AA;
        ((SprtWords *)g_primPtr)->wh = 0x7A0006;
        addPrim(&g_ot[0x14], g_primPtr);
        g_primPtr += sizeof(SprtWords);
        SetDrawTPage(g_primPtr, 1, 1, 0x1E);
        addPrim(&g_ot[0x14], g_primPtr);
        g_primPtr += sizeof(DR_TPAGE);
    }
}

void menuItem_update(void) {
    menuItem_input();
    menuItem_draw();
    menuItem_updateAnims();
}

void menuItem_exit(void) {
    if (g_gameState->mode == 3) {
        snd_stopSe(0x44A);
    }
    help_setLocked(0);
    help_setCursorOnTop(1);
    help_setCallback(NULL);
    func_801F2F30();
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
    MenuList *l = it->list;

    if (l->scroll == 0) {
        if (g_padRepeat & BTN_CIRCLE) {
            snd_playSe(SE_CANCEL);
            s_menuItem.state = ITEM_STATE_TABS;
            return;
        }
        if (g_padRepeat & BTN_CROSS) {
            menuItem_selectRow();
            return;
        }
        if (l->count != 0) {
            menuList_input(l, g_padRepeat);
            if (l->scroll == 0) {
                menuList_left(l, g_padRepeat);
                if (l->scroll == 0) {
                    menuList_right(l, g_padRepeat);
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
    MenuList *l = it->list;
    void (*f)(s32, MenuList *) = it->select;
    s32 idx = (l->top + l->cursor) * 2 + l->col;

    do {
        f(idx, l);
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
    Unk8006B1F4 *e;
    Character *c;
    s32 i;

    i = id - ITEM_FIRST_USABLE;
    e = &D_8006B1F4[i];
    c = g_gameState->gameData->party[s_menuItem.slot];
    if (item_useOnMember(c, c, e, e->unk4, 0)) {
        snd_playSe(SE_USE_ITEM);
        item_remove(id, 1);
    } else {
        snd_playSe(SE_BUZZER);
    }
}

void menuItem_drawTabs(void) {
    RECT *r;
    s32 i;

    r = &s_menuItem.cmdRect;
    for (i = 0; i < 3; i++) {
        text_drawBlock(r->x + s_menuItemTabX[i], r->y + 6, 0x1C, text_get(s_menuItemTabText[i]), 0);
    }
    win_drawBox(r, 0x1D, 2);
    win_drawShadow(r, 0x1D, 2);
}

void menuItem_drawScrollRows(void) {
    MenuView v;
    ItemList *it;
    s32 row;
    s32 col;
    s32 idx;

    it = &s_menuItemTabs[s_menuItem.tab];
    row = 0;
    menuList_getPageView(it->list, &v);
    idx = v.unk1 * 2;
    for (; row < v.unk0; row++) {
        for (col = 0; col < 2; col++) {
            it->drawRow(col, row, idx++, &v);
        }
    }
}

void menuItem_drawTargetWindow(void) {
    RECT *r = &s_menuItem.charWin.unk4;
    s32 i;

    if (s_menuItem.charWinOpen != 0) {
        for (i = 0; i < 4; i++) {
            if (g_gameState->gameData->party[i] != NULL) {
                ui_drawMemberSummary(r->x + 8, r->y + 6 + i * 0x2B, 0x14, i, s_menuItem.targetMode);
            }
        }
        win_drawBox(r, 0x14, 2);
        win_drawShadow(r, 0x14, 2);
    }
}

void menuItem_drawRow(s32 col, s32 row, s32 i, MenuView *v) {
    UnkPair *p = &g_gameState->gameData->items.slots[i];
    Unk80069018 *it;
    u8 ok;
    s32 x, y, x2;
    s32 k = 0x2A;
    u8 buf[8];

    if (p->unk1 != 0 && p->unk0 != ITEM_NONE) {
        it = &g_itemTable[p->unk0];
        ok = it->unkF & 1;
        x = col * 0x96;
        y = row * 0xF + (v->unk2 + k);
        text_formatNumber(p->unk1, buf, 2, 1);
        x2 = x + 0x18;
        text_drawBlock(x + 0x28, y, 0x1C, g_itemNames + it->unk0, ok ? 0 : 2);
        ui_drawItemIcon(x2, y, 0x1C, p->unk0, ok);
        ui_drawSmallDigits(x2 + 0x6C, y + 4, 0x1C, buf, ok ? 0 : 2);
    }
}

void menuItem_drawKeyRow(s32 col, s32 row, s32 i, MenuView *v) {
    u8 blink = (u32)g_menuFrame % 48 >= 0x10;
    u8 id;
    s32 x, y;
    s32 k = 0x2A;
    u8 *name;
    s32 w;
    Unk8006B374 *t;
    u8 got;

    if (i < s_menuItem.keyCount) {
        id = s_menuItem.keyItems[i];
        t = &g_keyItemTable[id];
        got = keyItem_isSeen(id);
        x = col * 0x96 + 0x18;
        y = row * 0xF + (v->unk2 + k);
        name = g_keyItemNames + t->unk0;
        if (!got) {
            w = x + text_measureWidth((s32)name);
            ui_drawSprite(w + 5, y + 2, 0x1C, 0xB6, blink);
            ui_drawSprite(w - 1, y - 1, 0x1C, 0xB5, 0);
        }
        text_drawBlock(x, y, 0x1C, name, 0);
    }
}

void menuItem_onKwehOpened(Window *w) {
    if (g_gameState->mode == 3) {
        snd_startSe(0x44A);
    }
}

void menuItem_drawKweh(Window *w) {
    u8 *s;

    s = text_get(ITEM_TEXT_KWEH);
    text_drawBlock(w->x + ((w->w - text_measureWidth((s32)s)) >> 1), w->y + 6, w->otz, s, 0);
}

void menuItem_stepKweh(void) {
    u32 t;

    if (s_menuItem.state == ITEM_STATE_BUSY) {
        t = s_menuItem.popupTimer;
        if (t != 0) {
            s_menuItem.popupTimer = t + 1;
            if (t >= 0x60) {
                s_menuItem.popupTimer = 0;
                if (g_gameState->mode == 3) {
                    snd_stopSe(0x44A);
                }
                help_setLocked(0);
                help_setCursorOnTop(1);
                help_setCallback(NULL);
                func_801F2F30();
                menuScreen_close();
            }
        }
    }
}

void menuItem_closeTarget(void) {
    ItemList *it;
    ItemMenu *m;

    it = s_menuItemTabs;
    m = &s_menuItem;
    tween_start(&m->charWin, s_menuItemTargetClose[it[s_menuItem.tab].list->col & 1], 0);
    m->state = ITEM_STATE_BUSY;
}

void menuItem_onTargetOpened(void) {
    s_menuItem.state = ITEM_STATE_TARGET;
}

void menuItem_onTargetClosed(void) {
    s_menuItem.state = ITEM_STATE_LIST;
    s_menuItem.charWinOpen = 0;
}

void menuItem_collectKeyItems(void) {
    s32 n;
    s32 i;

    n = 0;
    for (i = 0; i < KEY_ITEM_COUNT; i++) {
        if ((u8)keyItem_has(i)) {
            s_menuItem.keyItems[n] = i;
            n++;
        }
    }
    s_menuItem.keyCount = n;
}

void menuItem_openArrange(void) {
    MenuList *l;

    l = s_menuItemTabs[1].list;
    s_menuItem.popup = win_openFree(&s_menuItemArrangeRect, 0x14, 2);
    win_setDrawContent(s_menuItem.popup, menuItem_drawArrangeWindow);
    win_setOnOpened(s_menuItem.popup, menuItem_onArrangeOpened);
    win_setOnClosed(s_menuItem.popup, menuItem_onArrangeClosed);
    win_setShadow(s_menuItem.popup, 1);
    win_setTitle(s_menuItem.popup, D_80070408);
    s_menuItem.arrangeCursor = 0;
    s_menuItem.swapFrom = l->col + (l->top + l->cursor) * 2;
    s_menuItem.state = ITEM_STATE_BUSY;
}

void menuItem_closeArrange(void) {
    win_close(s_menuItem.popup);
}

void menuItem_drawArrangeWindow(Window *w) {
    Window *p = w;
    s32 i;

    for (i = 0; i < 2; i++) {
        text_drawBlock(p->x + 10, p->y + 6 + i * 15, w->otz, text_get(i + ITEM_TEXT_AUTO), 0);
    }
}

void menuItem_onArrangeOpened(Window *w) {
    s_menuItem.state = ITEM_STATE_ARRANGE;
}

void menuItem_onArrangeClosed(Window *w) {
    s_menuItem.state = ITEM_STATE_TABS;
}

void menuItem_loadImages(void) {
    u8 *p = s_menuItemKeyImage;

    LoadImage(&s_menuItemKeyClutRect, (u32 *)p);
    DrawSync(0);
    LoadImage(&s_menuItemKeyImageRect, (u32 *)(p + 0x40));
    DrawSync(0);
}

void func_801F2F30(void) {
}

void menuItem_showKeyItem(s32 id) {
    s_menuItem.keyViewId = id;
    s_menuItem.keyView = 1;
    s_menuItem.keyViewPhase = KEY_VIEW_OPENING;
    s_menuItem.state = ITEM_STATE_BUSY;
    s_menuItem.keyViewStep = 0;
    s_menuItem.keyViewX = 0x48;
    s_menuItem.keyViewX -= tween_sumDeltas(0x16, s_menuItemKeyItemSlide);
    help_setLocked(1);
}

void menuItem_hideKeyItem(void) {
    s_menuItem.keyViewPhase = KEY_VIEW_CLOSING;
    s_menuItem.state = ITEM_STATE_BUSY;
    s_menuItem.keyViewStep = 0;
    s_menuItem.keyViewX = 0x48;
}

s32 menuItem_getSlot(MenuList *l) {
    s32 r, t;
    s32 step;

    if (l->scroll & 8) {
        t = l->col ^ 1;
        step = 1;
        r = l->cursor;
        if (l->scroll & 2) {
            return (l->top + (r - step)) * 2 + t;
        }
        return (l->top + (r + step)) * 2 + t;
    }
    return (l->top + l->cursor) * 2 + l->col;
}

void func_801F3038(void) {
}

INCLUDE_RODATA("asm/us/ovl_37/nonmatchings/ovl_37", s_menuItemKeyImage);
