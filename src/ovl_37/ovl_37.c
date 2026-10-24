#include "ovl_37.h"

INCLUDE_RODATA("asm/us/ovl_37/nonmatchings/ovl_37", g_itemMenuText);

void menu_items_init(void) {
    g_menuItems.state = ITEM_STATE_LIST;
    g_menuItems.cmd = 0;
    g_menuItems.arrangeCursor = 0;
    g_menuItems.charWinOpen = 0;
    g_menuItems.swapFrom = 0;
    g_menuItems.popupTimer = 0;
    g_menuItems.keyView = 0;
    g_menuItems.keyViewPhase = 0;
    func_801F3038();
    menu_items_initKeyItems();
    text_setBank(g_itemMenuText);
    func_8003556C(func_800355DC);
    func_80035674(1);
    func_8002EDFC(&g_menuItems.itemList, ITEM_SLOT_COUNT - 1, 0xB, 2, 0xAD);
    func_8002EDFC(&g_menuItems.keyList, g_menuItems.keyCount, 0xB, 2, 0xAD);
    setRECT(&g_menuItems.cmdRect, 8, 8, 0xE4, 0x1A);
    setRECT(&g_menuItems.listRect, 8, 0x2A, 0x12C, 0xAD);
    func_80063118(&g_menuItems.charWin, 0, 0);
    menu_items_loadImages();
}

void menu_items_inputCmd(void) {
    if (g_padRepeat & BTN_CIRCLE) {
        snd_playSe(SE_CANCEL);
        func_800361CC(0);
        return;
    }
    if (g_padRepeat & BTN_CROSS) {
        snd_playSe(SE_CURSOR);
        if (g_menuItems.cmd != ITEM_CMD_ARRANGE) {
            g_menuItems.state = ITEM_STATE_LIST;
            return;
        }
        menu_items_openArrange();
        return;
    }
    if (g_padRepeat & PAD_LEFT) {
        snd_playSe(SE_CURSOR);
        g_menuItems.cmd = g_menuItems.cmd != 0 ? g_menuItems.cmd - 1 : ITEM_CMD_KEY;
        return;
    }
    if (g_padRepeat & PAD_RIGHT) {
        snd_playSe(SE_CURSOR);
        g_menuItems.cmd = g_menuItems.cmd >= ITEM_CMD_KEY ? 0 : g_menuItems.cmd + 1;
    }
}

void menu_items_inputTarget(void) {
    ItemList *it = &g_menuItemsLists[g_menuItems.cmd];
    MenuList *l = it->list;
    s32 i;
    u8 cur;
    s32 next;

    if (g_menuItems.charWin.unkC == 0) {
        if (g_padRepeat & BTN_CIRCLE) {
            snd_playSe(SE_CANCEL);
            menu_items_closeTarget();
            return;
        }
        if (g_padRepeat & BTN_CROSS) {
            i = (l->unk5 + l->cursor) * 2 + l->unk7;
            menu_items_useItem(g_gameState->unk1C->items.slots[i].id);
            if (g_gameState->unk1C->items.slots[i].count == 0) {
                menu_items_closeTarget();
            }
        } else if (g_padRepeat & PAD_UP) {
            cur = g_menuItems.slot;
            next = func_800337D0(cur);
            if (cur != next) {
                g_menuItems.slot = next;
                snd_playSe(SE_CURSOR);
            }
        } else if (g_padRepeat & PAD_DOWN) {
            cur = g_menuItems.slot;
            next = func_8003380C(cur);
            if (cur != next) {
                g_menuItems.slot = next;
                snd_playSe(SE_CURSOR);
            }
        }
    }
}

void menu_items_inputManual(void) {
    MenuList *l = g_menuItemsLists[1].list;
    ItemSlot tmp;
    s32 i;
    u8 j;

    if (l->unk8 == 0) {
        if (g_padRepeat & BTN_CIRCLE) {
            snd_playSe(SE_CANCEL);
            g_menuItems.state = ITEM_STATE_ARRANGE;
            return;
        }
        if (g_padRepeat & BTN_CROSS) {
            if (g_menuItems.swapPicked == 0) {
                snd_playSe(SE_CURSOR);
                j = l->unk7 + (l->cursor + l->unk5) * 2;
                g_menuItems.swapPicked = 1;
                g_menuItems.swapFrom = j;
                return;
            }
            snd_playSe(SE_CURSOR);
            i = (l->cursor + l->unk5) * 2 + l->unk7;
            tmp = g_gameState->unk1C->items.slots[g_menuItems.swapFrom];
            g_gameState->unk1C->items.slots[g_menuItems.swapFrom] = g_gameState->unk1C->items.slots[i];
            g_gameState->unk1C->items.slots[i] = tmp;
            g_menuItems.swapPicked = 0;
            return;
        }
        menuList_input(l, g_padRepeat);
        func_8002F044(l, g_padRepeat);
        func_8002F0EC(l, g_padRepeat);
    }
}

void menu_items_selectItem(s32 idx, MenuList *l) {
    UnkPair *p;
    Unk80069018 *it;
    Unk8006B1F4 *e;
    u8 ok;
    s32 id;

    p = &g_gameState->unk1C->items.slots[idx];
    ok = 0;
    if (p->unk1 != 0 && p->unk0 != ITEM_NONE && p->unk0 >= ITEM_FIRST_USABLE) {
        id = p->unk0;
        it = &D_80069018[id];
        e = &D_8006B1F4[id - ITEM_FIRST_USABLE];
        if (it->unkF & 1) {
            g_menuItems.targetKind = e->unk0;
            ok = 1;
            g_menuItems.targetMode = p->unk0 != 0xFB ? e->unk0_5 : 0;
            if (g_menuItems.targetMode == 0) {
                menu_items_useItem(p->unk0);
            } else {
                menu_items_openTarget();
            }
        }
    }
    if (!ok) {
        snd_playSe(SE_BUZZER);
    }
}

void menu_items_drawList(void) {
    MenuView view;
    ItemList *il;
    s32 w;
    s32 row;
    s32 col;
    s32 idx;

    il = &g_menuItemsLists[g_menuItems.cmd];
    ui_drawLabel(g_menuItemsListRect->x + 0xE, g_menuItemsListRect->y - 3, 0x1D, D_800703FC.unk68, 0);
    ui_drawLabel(g_menuItemsListRect->x + 0xA4, g_menuItemsListRect->y - 3, 0x1D, D_800703FC.unk68, 0);
    if (g_menuItems.cmd != ITEM_CMD_KEY) {
        w = func_8002AE7C((u8 *)D_800703FC.unk94);
        ui_drawLabel((g_menuItemsListRect->x + 0x8A) - w, g_menuItemsListRect->y - 3, 0x1D, D_800703FC.unk94, 0);
        ui_drawLabel((g_menuItemsListRect->x + 0x120) - w, g_menuItemsListRect->y - 3, 0x1D, D_800703FC.unk94, 0);
    }
    menuList_getView(il->list, &view);
    func_8002E040(g_menuItemsListRect, 0x1D, 1, il->list);
    func_8002705C(g_menuItemsListRect, 0x1D, 1);
    idx = view.unk1 * 2;
    win_setClip(NULL, 0x1C, 1);
    for (row = 0; row < view.unk0; row++) {
        for (col = 0; col < 2; col++) {
            il->drawRow(col, row, idx++, &view);
        }
    }
    if (il->list->unk8 & 4) {
        func_8002F40C(il->list, g_menuItemsListRect, &g_menuItemsScrollClip);
        win_setClip(&g_menuItemsScrollClip, 0x1C, 1);
        menu_items_drawListScroll();
    }
    win_setClip(g_menuItemsListRect, 0x1C, 1);
}

void menu_items_drawCursor(void) {
    MenuList *l;
    s32 off;
    u8 blink;
    s32 a;
    s32 k;
    u8 u;

    off = 0;
    l = g_menuItemsLists[g_menuItems.cmd].list;
    blink = (D_80077E54 & 3) != 0;
    func_80035674(g_menuItems.state != ITEM_STATE_CMD);
    if (func_8002F810() || g_menuItems.state == ITEM_STATE_BUSY) {
        return;
    }
    if (g_menuItems.state == ITEM_STATE_CMD || blink) {
        D_80077DE8.unk0 = g_menuItemsCmdCursorX[g_menuItems.cmd] - 0x10;
        D_80077DE8.unk2 = 0x10;
        D_80077DE8.unk4 = 0x19;
        func_800355E4();
    }
    if (g_menuItems.state == ITEM_STATE_ARRANGE || (g_menuItems.state == ITEM_STATE_MANUAL && blink)) {
        D_80077DE8.unk0 = 0x7E;
        D_80077DE8.unk4 = 0x12;
        D_80077DE8.unk2 = g_menuItems.arrangeCursor * 15 + 0xE;
        func_800355E4();
    }
    if (g_menuItems.state >= ITEM_STATE_LIST && g_menuItems.state != ITEM_STATE_ARRANGE && (g_menuItems.state == ITEM_STATE_LIST || blink)) {
        k = l->unk7 ^ ((l->unk8 >> 3) & 1);
        D_80077DE8.unk4 = 0x19;
        /* Both arms are identical (jump2 merges them after register allocation); the test keeps the
         * hi part of &g_menuItems and the constant 2 live, which the original's registers need. */
        if (g_menuItems.cmd == ITEM_CMD_KEY) {
            D_80077DE8.unk0 = k * 0x96;
            D_80077DE8.unk2 = l->cursor * 15 + 0x34;
            func_800355E4();
        } else {
            D_80077DE8.unk0 = k * 0x96;
            D_80077DE8.unk2 = l->cursor * 15 + 0x34;
            func_800355E4();
        }
    }
    if (g_menuItems.state == ITEM_STATE_TARGET) {
        D_80077DE8.unk4 = 0x12;
        D_80077DE8.unk0 = g_menuItems.charWin.unk4.x - 0xA;
        D_80077DE8.unk2 = g_menuItems.charWin.unk4.y + (s16)(g_menuItems.slot * 43 + 0x12);
        func_800355E4();
    }
    if (g_menuItems.state == ITEM_STATE_MANUAL) {
        if (g_menuItems.swapPicked == 1 && blink && !(l->unk8 & 4)) {
            a = l->unk5;
            if (g_menuItems.swapFrom >= a * 2 && g_menuItems.swapFrom < (a + 0xB) * 2) {
                if (l->unk8 != 0) {
                    off = l->unk9;
                    if (!(l->unk8 & 2)) {
                        off = -off;
                    }
                }
                u = g_menuItems.swapFrom;
                D_80077DE8.unk0 = (u & 1) * 0x96 + 2;
                D_80077DE8.unk2 = ((u >> 1) - l->unk5) * 15 + (s16)(off + 0x32);
                D_80077DE8.unk4 = 0x12;
                func_800355E4();
            }
        }
        k = l->unk7 ^ ((l->unk8 >> 3) & 1);
        D_80077DE8.unk0 = k * 0x96;
        D_80077DE8.unk2 = l->cursor * 15 + 0x34;
        D_80077DE8.unk4 = 0x12;
        func_800355E4();
    }
}

void menu_items_openTarget(void) {
    ItemMenu *m;
    RECT *r;
    GameState *gs;
    s32 k = (g_menuItemsLists[g_menuItems.cmd].list->unk7 ^ 1) & 1;

    g_menuItems.charWin.unk4.x = (u8)k == 0 ? 8 : 0xA4;
    r = &g_menuItems.charWin.unk4;
    r->y = 0x26;
    r->w = 0x90;
    r->h = 0xB6;
    func_80063118(&g_menuItems.charWin, g_menuItemsTargetOpen[k], 1);
    gs = g_gameState;
    m = &g_menuItems;
    m->slot = 0;
loop:
    if (gs->unk1C->party[m->slot] == NULL) {
        if (++m->slot < 4) {
            goto loop;
        }
    }
    m->state = ITEM_STATE_BUSY;
    m->charWinOpen = 1;
    snd_playSe(SE_CURSOR);
}

void menu_items_sort(void) {
    s32 n;
    s32 i;
    s32 j;

    n = 0;
    for (i = 0; i < ITEM_SLOT_COUNT; i++) {
        if (g_gameState->unk1C->items.slots[i].count != 0 && g_gameState->unk1C->items.slots[i].id != ITEM_NONE) {
            g_menuItemsSortBuf[n++] = g_gameState->unk1C->items.slots[i];
        }
    }
    for (i = 0; i < n - 1; i++) {
        for (j = i + 1; j < n; j++) {
            if (D_80069018[g_menuItemsSortBuf[i].id].unk10 > D_80069018[g_menuItemsSortBuf[j].id].unk10) {
                g_menuItemsSortTmp = g_menuItemsSortBuf[i];
                g_menuItemsSortBuf[i] = g_menuItemsSortBuf[j];
                g_menuItemsSortBuf[j] = g_menuItemsSortTmp;
            } else if (D_80069018[g_menuItemsSortBuf[i].id].unk10 == D_80069018[g_menuItemsSortBuf[j].id].unk10 &&
                       g_menuItemsSortBuf[j].id < g_menuItemsSortBuf[i].id) {
                g_menuItemsSortTmp = g_menuItemsSortBuf[i];
                g_menuItemsSortBuf[i] = g_menuItemsSortBuf[j];
                g_menuItemsSortBuf[j] = g_menuItemsSortTmp;
            }
        }
    }
    for (i = 0; i < n; i++) {
        g_gameState->unk1C->items.slots[i] = g_menuItemsSortBuf[i];
    }
    for (i = n; i < ITEM_SLOT_COUNT; i++) {
        g_gameState->unk1C->items.slots[i].count = 0;
    }
    g_menuItems.itemList.unk7 = 0;
    g_menuItems.itemList.cursor = 0;
    g_menuItems.itemList.unk5 = 0;
}

s32 menu_items_callChocobo(void) {
    ItemMenu *m = &g_menuItems;

    if (g_gameState->mode == 3 && g_evtGlobalVars.b[0xBF] != 0 && g_evtGlobalVars.b[0xB5] == 0) {
        if (func_800B8124()) {
            g_evtGlobalVars.b[0xB5] = 1;
            func_800B81A0();
            m->popup = func_8002DC54(&g_menuItemsKwehRect, 0xF, 2);
            win_setOnOpened(m->popup, menu_items_onKwehOpened);
            win_setDrawContent(m->popup, menu_items_drawKweh);
            func_8002E304(m->popup, (s32)D_801F3140);
            func_8002E31C(m->popup, 1);
            m->popupTimer = 1;
            m->state = ITEM_STATE_BUSY;
            func_8003563C(1);
            return 1;
        }
        return 0;
    }
    return 0;
}

void menu_items_setHelpText(void) {
    RECT r;
    MenuList *l;
    UnkPair *p;
    s32 i;
    s16 x;
    s16 y;
    s32 z;
    Unk80077DE8 *h;

    h = &D_80077DE8;
    x = h->unk0;
    y = h->unk2;
    z = h->unk4;
    h->unk8 = NULL;
    h->unk0 = x;
    h->unk2 = y;
    h->unk4 = z;
    func_80035580(NULL);
    if (g_menuItems.state == ITEM_STATE_CMD) {
        h->unk8 = text_get(g_menuItems.cmd + ITEM_TEXT_CMD_HELP);
    }
    if (g_menuItems.state == ITEM_STATE_LIST) {
        if (g_menuItems.cmd != 0) {
            i = menu_items_getCursorIndex(&g_menuItems.keyList);
            if (i < g_menuItems.keyCount) {
                h->unk8 = &g_menuItemsKeyHelp[D_8006B374[g_menuItems.keyItems[i]].unk2];
            }
            return;
        }
        l = &g_menuItems.itemList;
    } else if (g_menuItems.state == ITEM_STATE_TARGET) {
        if (menu_items_getCursorIndex(&g_menuItems.itemList) & 1) {
            r.x = 0xC;
            r.y = 0;
            r.w = g_gameState->unkC->disp.w;
            r.h = g_gameState->unkC->disp.h;
        } else {
            r.x = 0;
            r.y = 0;
            r.w = g_gameState->unkC->disp.w >> 1;
            r.h = g_gameState->unkC->disp.h;
        }
        func_80035580(&r);
        D_80077DE8.unk8 = text_get(ITEM_TEXT_SELECT_CHAR);
        return;
    } else if (g_menuItems.state == ITEM_STATE_ARRANGE) {
        func_80035580(&g_menuItemsArrangeHelpArea);
        h->unk8 = text_get(g_menuItems.arrangeCursor + ITEM_TEXT_ARRANGE_HELP);
        return;
    } else if (g_menuItems.state == ITEM_STATE_MANUAL) {
        l = g_menuItemsLists[1].list;
    } else {
        return;
    }
    p = &g_gameState->unk1C->items.slots[menu_items_getCursorIndex(l)];
    if (p->unk1 != 0) {
        h->unk8 = &g_menuItemsHelp[D_80069018[p->unk0].unk2];
    }
}

void menu_items_stepKeyView(void) {
    if (g_menuItems.keyView != 0 && g_menuItems.keyViewPhase != 0) {
        if (g_menuItems.keyViewPhase == KEY_VIEW_OPENING) {
            g_menuItems.keyViewX += g_menuItemsKeyViewSlide[g_menuItems.keyViewStep++];
            if (g_menuItems.keyViewStep >= 0x16) {
                g_menuItems.keyViewPhase = KEY_VIEW_SHOWN;
                g_menuItems.keyViewStep = 0;
                g_menuItems.state = ITEM_STATE_KEY_VIEW;
            }
        } else if (g_menuItems.keyViewPhase == KEY_VIEW_CLOSING) {
            g_menuItems.keyViewX += g_menuItemsKeyViewSlide[g_menuItems.keyViewStep++];
            if (g_menuItems.keyViewStep >= 8) {
                g_menuItems.keyViewStep = 0;
                g_menuItems.keyViewPhase = 0;
                g_menuItems.keyView = 0;
                g_menuItems.state = ITEM_STATE_LIST;
                func_80028128(g_menuItems.keyViewId);
                func_8003563C(0);
            }
        }
    }
}

void menu_items_drawKeyView(void) {
    ItemMenu *m = &g_menuItems;
    s32 x;
    u8 *name;

    if (m->keyView != 0) {
        x = m->keyViewX;
        name = D_8006ADAC + D_8006B374[m->keyViewId].unk0;
        text_drawBlock(x + ((0xB0 - func_8002BA68((s32)name)) >> 1), 0x50, 0x14, name, 8);
        text_drawBlock(x + 0xD, 0x66, 0x14, g_menuItemsKeyDesc + D_8006B374[m->keyViewId].unk6, 8);
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

void menu_items_update(void) {
    menu_items_input();
    menu_items_draw();
    menu_items_animate();
}

void menu_items_exit(void) {
    if (g_gameState->mode == 3) {
        func_8002F55C(0x44A);
    }
    func_8003563C(0);
    func_80035674(1);
    func_8003556C(NULL);
    func_801F2F30();
}

void menu_items_input(void) {
    void (*fn)(void);

    if (!func_8002F810()) {
        fn = g_menuItemsInputFns[g_menuItems.state];
        if (fn != NULL) {
            fn();
        }
    }
}

void menu_items_draw(void) {
    menu_items_drawCmds();
    menu_items_drawList();
    menu_items_drawTargets();
    menu_items_drawKeyView();
    menu_items_drawCursor();
}

void menu_items_animate(void) {
    menu_items_setHelpText();
    menu_items_stepKeyView();
    menu_items_stepKweh();
    func_800631AC(&g_menuItems.charWin);
    func_8002EB74(&g_menuItems.itemList);
    func_8002EB74(&g_menuItems.keyList);
}

void menu_items_inputList(void) {
    ItemList *it = &g_menuItemsLists[g_menuItems.cmd];
    MenuList *l = it->list;

    if (l->unk8 == 0) {
        if (g_padRepeat & BTN_CIRCLE) {
            snd_playSe(SE_CANCEL);
            g_menuItems.state = ITEM_STATE_CMD;
            return;
        }
        if (g_padRepeat & BTN_CROSS) {
            menu_items_select();
            return;
        }
        if (l->unk0 != 0) {
            menuList_input(l, g_padRepeat);
            if (l->unk8 == 0) {
                func_8002F044(l, g_padRepeat);
                if (l->unk8 == 0) {
                    func_8002F0EC(l, g_padRepeat);
                }
            }
        }
    }
}

void menu_items_inputArrange(void) {
    if (g_padRepeat & BTN_CIRCLE) {
        snd_playSe(SE_CANCEL);
        menu_items_closeArrange();
        return;
    }
    if (g_padRepeat & (PAD_UP | PAD_DOWN)) {
        snd_playSe(SE_CURSOR);
        g_menuItems.arrangeCursor ^= 1;
        return;
    }
    if (g_padRepeat & BTN_CROSS) {
        if (g_menuItems.arrangeCursor == 0) {
            snd_playSe(SE_CURSOR);
            menu_items_sort();
            return;
        }
        snd_playSe(SE_CURSOR);
        g_menuItems.swapPicked = 0;
        g_menuItems.state = ITEM_STATE_MANUAL;
    }
}

void menu_items_inputKeyView(void) {
    if (g_padRepeat & BTN_CIRCLE) {
        snd_playSe(SE_CANCEL);
        menu_items_closeKeyView();
    } else if (g_padRepeat & BTN_CROSS) {
        snd_playSe(SE_CURSOR);
        menu_items_closeKeyView();
    }
}

void menu_items_select(void) {
    ItemList *it = &g_menuItemsLists[g_menuItems.cmd];
    MenuList *l = it->list;
    void (*f)(s32, MenuList *) = it->select;
    s32 idx = (l->unk5 + l->cursor) * 2 + l->unk7;

    do {
        f(idx, l);
    } while (0);
}

void menu_items_selectKeyItem(s32 i) {
    if (i < g_menuItems.keyCount) {
        menu_items_openKeyView(g_menuItems.keyItems[i]);
        snd_playSe(SE_CURSOR);
    } else {
        snd_playSe(SE_BUZZER);
    }
}

void menu_items_useItem(s32 id) {
    Unk8006B1F4 *e;
    Character *c;
    s32 i;

    i = id - ITEM_FIRST_USABLE;
    e = &D_8006B1F4[i];
    c = g_gameState->unk1C->party[g_menuItems.slot];
    if (func_800654F8(c, c, e, e->unk4, 0)) {
        snd_playSe(SE_USE_ITEM);
        item_remove(id, 1);
    } else {
        snd_playSe(SE_BUZZER);
    }
}

void menu_items_drawCmds(void) {
    RECT *r;
    s32 i;

    r = &g_menuItems.cmdRect;
    for (i = 0; i < 3; i++) {
        text_drawBlock(r->x + g_menuItemsCmdX[i], r->y + 6, 0x1C, text_get(g_menuItemsCmdText[i]), 0);
    }
    func_8002DEE4(r, 0x1D, 2);
    func_8002705C(r, 0x1D, 2);
}

void menu_items_drawListScroll(void) {
    MenuView v;
    ItemList *it;
    s32 row;
    s32 col;
    s32 idx;

    it = &g_menuItemsLists[g_menuItems.cmd];
    row = 0;
    func_8002F344(it->list, &v);
    idx = v.unk1 * 2;
    for (; row < v.unk0; row++) {
        for (col = 0; col < 2; col++) {
            it->drawRow(col, row, idx++, &v);
        }
    }
}

void menu_items_drawTargets(void) {
    RECT *r = &g_menuItems.charWin.unk4;
    s32 i;

    if (g_menuItems.charWinOpen != 0) {
        for (i = 0; i < 4; i++) {
            if (g_gameState->unk1C->party[i] != NULL) {
                func_80026EA0(r->x + 8, r->y + 6 + i * 0x2B, 0x14, i, g_menuItems.targetMode);
            }
        }
        func_8002DEE4(r, 0x14, 2);
        func_8002705C(r, 0x14, 2);
    }
}

void menu_items_drawItemRow(s32 col, s32 row, s32 i, MenuView *v) {
    UnkPair *p = &g_gameState->unk1C->items.slots[i];
    Unk80069018 *it;
    u8 ok;
    s32 x, y, x2;
    s32 k = 0x2A;
    u8 buf[8];

    if (p->unk1 != 0 && p->unk0 != ITEM_NONE) {
        it = &D_80069018[p->unk0];
        ok = it->unkF & 1;
        x = col * 0x96;
        y = row * 0xF + (v->unk2 + k);
        text_formatNumber(p->unk1, buf, 2, 1);
        x2 = x + 0x18;
        text_drawBlock(x + 0x28, y, 0x1C, D_8006A218 + it->unk0, ok ? 0 : 2);
        ui_drawItemIcon(x2, y, 0x1C, p->unk0, ok);
        ui_drawSmallDigits(x2 + 0x6C, y + 4, 0x1C, buf, ok ? 0 : 2);
    }
}

void menu_items_drawKeyRow(s32 col, s32 row, s32 i, MenuView *v) {
    u8 blink = (u32)D_80077DC0 % 48 >= 0x10;
    u8 id;
    s32 x, y;
    s32 k = 0x2A;
    u8 *name;
    s32 w;
    Unk8006B374 *t;
    u8 got;

    if (i < g_menuItems.keyCount) {
        id = g_menuItems.keyItems[i];
        t = &D_8006B374[id];
        got = func_800281D0(id);
        x = col * 0x96 + 0x18;
        y = row * 0xF + (v->unk2 + k);
        name = D_8006ADAC + t->unk0;
        if (!got) {
            w = x + func_8002BA68((s32)name);
            ui_drawSprite(w + 5, y + 2, 0x1C, 0xB6, blink);
            ui_drawSprite(w - 1, y - 1, 0x1C, 0xB5, 0);
        }
        text_drawBlock(x, y, 0x1C, name, 0);
    }
}

void menu_items_onKwehOpened(Window *w) {
    if (g_gameState->mode == 3) {
        func_8002F514(0x44A);
    }
}

void menu_items_drawKweh(Window *w) {
    u8 *s;

    s = text_get(ITEM_TEXT_KWEH);
    text_drawBlock(w->unk0 + ((w->unk4 - func_8002BA68((s32)s)) >> 1), w->unk2 + 6, w->unk18, s, 0);
}

void menu_items_stepKweh(void) {
    u32 t;

    if (g_menuItems.state == ITEM_STATE_BUSY) {
        t = g_menuItems.popupTimer;
        if (t != 0) {
            g_menuItems.popupTimer = t + 1;
            if (t >= 0x60) {
                g_menuItems.popupTimer = 0;
                if (g_gameState->mode == 3) {
                    func_8002F55C(0x44A);
                }
                func_8003563C(0);
                func_80035674(1);
                func_8003556C(NULL);
                func_801F2F30();
                func_800362C8();
            }
        }
    }
}

void menu_items_closeTarget(void) {
    ItemList *it;
    ItemMenu *m;

    it = g_menuItemsLists;
    m = &g_menuItems;
    func_80063118(&m->charWin, g_menuItemsTargetClose[it[g_menuItems.cmd].list->unk7 & 1], 0);
    m->state = ITEM_STATE_BUSY;
}

void menu_items_onTargetOpened(void) {
    g_menuItems.state = ITEM_STATE_TARGET;
}

void menu_items_onTargetClosed(void) {
    g_menuItems.state = ITEM_STATE_LIST;
    g_menuItems.charWinOpen = 0;
}

void menu_items_initKeyItems(void) {
    s32 n;
    s32 i;

    n = 0;
    for (i = 0; i < KEY_ITEM_COUNT; i++) {
        if ((u8)func_800281A4(i)) {
            g_menuItems.keyItems[n] = i;
            n++;
        }
    }
    g_menuItems.keyCount = n;
}

void menu_items_openArrange(void) {
    MenuList *l;

    l = g_menuItemsLists[1].list;
    g_menuItems.popup = func_8002DC54(&g_menuItemsArrangeRect, 0x14, 2);
    win_setDrawContent(g_menuItems.popup, menu_items_drawArrange);
    win_setOnOpened(g_menuItems.popup, menu_items_onArrangeOpened);
    win_setOnClosed(g_menuItems.popup, menu_items_onArrangeClosed);
    func_8002E31C(g_menuItems.popup, 1);
    func_8002E304(g_menuItems.popup, D_80070408);
    g_menuItems.arrangeCursor = 0;
    g_menuItems.swapFrom = l->unk7 + (l->unk5 + l->cursor) * 2;
    g_menuItems.state = ITEM_STATE_BUSY;
}

void menu_items_closeArrange(void) {
    win_close(g_menuItems.popup);
}

void menu_items_drawArrange(Window *w) {
    Window *p = w;
    s32 i;

    for (i = 0; i < 2; i++) {
        text_drawBlock(p->unk0 + 10, p->unk2 + 6 + i * 15, w->unk18, text_get(i + ITEM_TEXT_AUTO), 0);
    }
}

void menu_items_onArrangeOpened(Window *w) {
    g_menuItems.state = ITEM_STATE_ARRANGE;
}

void menu_items_onArrangeClosed(Window *w) {
    g_menuItems.state = ITEM_STATE_CMD;
}

void menu_items_loadImages(void) {
    u8 *p = g_menuItemsKeyImage;

    LoadImage(&g_menuItemsKeyClutRect, (u32 *)p);
    DrawSync(0);
    LoadImage(&g_menuItemsKeyImageRect, (u32 *)(p + 0x40));
    DrawSync(0);
}

void func_801F2F30(void) {
}

void menu_items_openKeyView(s32 id) {
    g_menuItems.keyViewId = id;
    g_menuItems.keyView = 1;
    g_menuItems.keyViewPhase = KEY_VIEW_OPENING;
    g_menuItems.state = ITEM_STATE_BUSY;
    g_menuItems.keyViewStep = 0;
    g_menuItems.keyViewX = 0x48;
    g_menuItems.keyViewX -= func_800632B4(0x16, g_menuItemsKeyViewSlide);
    func_8003563C(1);
}

void menu_items_closeKeyView(void) {
    g_menuItems.keyViewPhase = KEY_VIEW_CLOSING;
    g_menuItems.state = ITEM_STATE_BUSY;
    g_menuItems.keyViewStep = 0;
    g_menuItems.keyViewX = 0x48;
}

s32 menu_items_getCursorIndex(MenuList *l) {
    s32 r, t;
    s32 step;

    if (l->unk8 & 8) {
        t = l->unk7 ^ 1;
        step = 1;
        r = l->cursor;
        if (l->unk8 & 2) {
            return (l->unk5 + (r - step)) * 2 + t;
        }
        return (l->unk5 + (r + step)) * 2 + t;
    }
    return (l->unk5 + l->cursor) * 2 + l->unk7;
}

void func_801F3038(void) {
}

INCLUDE_RODATA("asm/us/ovl_37/nonmatchings/ovl_37", g_menuItemsKeyImage);
