#include "ovl_3c.h"

void shop_drawMessage(void) {
    ui_drawLabel(s_shopMessageRect.x + WIN_TITLE_X, s_shopMessageRect.y + WIN_TITLE_Y, s_shop.messageOtz, g_labelHelp, UI_LABEL_NORMAL);
    if (s_shop.help != NULL) {
        text_drawBlock(s_shopMessageRect.x + 0xA, s_shopMessageRect.y + 6, s_shop.messageOtz, s_shop.help, TEXT_PAL_WHITE);
    }
    win_drawBox(&s_shopMessageRect, s_shop.messageOtz, WIN_STYLE_LINE_RULE);
    win_drawShadow(&s_shopMessageRect, s_shop.messageOtz, WIN_STYLE_LINE_RULE);
}

void shop_inputMenu(void) {
    if (!fade_isBusy()) {
        if (g_padRepeat & BTN_CIRCLE) {
            snd_playSe(SE_CANCEL);
            shop_close();
            return;
        }
        if (g_padRepeat & PAD_UP) {
            snd_playSe(SE_CURSOR);
            s_shop.cursor = s_shop.cursor == 0 ? s_shop.entryCount - 1 : s_shop.cursor - 1;
            return;
        }
        if (g_padRepeat & PAD_DOWN) {
            snd_playSe(SE_CURSOR);
            s_shop.cursor = s_shop.cursor >= s_shop.entryCount - 1 ? 0 : s_shop.cursor + 1;
            return;
        }
        if (g_padRepeat & BTN_CROSS) {
            snd_playSe(SE_CURSOR);
            shop_selectMenu();
        }
    }
}

void shop_drawMenu(void) {
    s32 i;
    s32 y;

    s_shopMenuRect.h = s_shop.entryCount * 0xF + 0xB;
    win_drawBox(&s_shopMenuRect, 0x23, WIN_STYLE_BEVEL);
    win_drawShadow(&s_shopMenuRect, 0x23, WIN_STYLE_BEVEL);
    for (i = 0; i < s_shop.entryCount; i++) {
        text_drawBlock(s_shopMenuRect.x + 0xA, s_shopMenuRect.y + 6 + i * 0xF, 0x1E, text_get(s_shopMenuText[s_shop.kind] + i), TEXT_PAL_WHITE);
    }
}

s32 shop_sumArmorStat(s32 stat, u8 *equip) {
    u32 result = 0;
    u8 sum[4];
    s32 i;
    s32 j;
    Unk8006D328 *bonus;

    memset(sum, result, sizeof(sum));
    for (i = EQUIP_SLOT_HEAD; i < EQUIP_SLOT_COUNT; i++) {
        if (equip[i] != ITEM_NONE) {
            if (equip[i] < ITEM_FIRST_USABLE) {
                j = equip[i] - ITEM_FIRST_ARMOR;
                bonus = &g_armorStatBonus[j];
                sum[0] += bonus->defense;
                sum[1] += bonus->evade;
                sum[2] += bonus->magicDef;
                sum[3] += bonus->magicEvade;
            }
        }
    }
    switch (stat) {
    case EQUIP_SLOT_HEAD:
        result = sum[2];
        break;
    case EQUIP_SLOT_ARM:
        result = sum[1];
        break;
    case EQUIP_SLOT_BODY:
        result = sum[0];
        break;
    case EQUIP_SLOT_ACCESSORY:
        result = 0;
        break;
    }
    return result;
}

void shop_leaveMode(void) {
    s_shop.messageOtz = SHOP_OTZ_MENU;
    fade_outDefault();
    fade_setOnDone(shop_onModeLeft);
}

void shop_setHelp(s32 i) {
    s_shop.help = (u8 *)text_getFrom(g_shopText, i);
}

s32 shop_getId(void) {
    return s_shop.id;
}

void shop_collectMembers(void) {
    s32 i;

    s_shop.memberCount = 0;
    for (i = 0; i < CHR_COUNT; i++) {
        if (g_gameState->gameData->characters[i].info.b.recruited) {
            s_shop.members[s_shop.memberCount] = i;
            if (++s_shop.memberCount >= 8) {
                break;
            }
        }
    }
}

void shop_drawStatIcon(s32 x, s32 y, s32 z, s32 icon, u8 flag) {
    RectTbl7 rects = s_shopIcons;

    ui_drawSpriteDef(x, y, z, &rects.r[icon], flag);
}

void shop_init(void) {
    s_shop.id = g_menuArg;
    s_shop.kind = shop_getKind(g_menuArg);
    s_shop.cursor = 0;
    s_shop.inMode = 0;
    s_shop.entryCount = shop_countEntries(s_shop.kind);
    s_shop.savedOtz = fade_setOtz(SHOP_FADE_OTZ);
    s_shop.messageOtz = SHOP_OTZ_MENU;
    s_shop.visits = 0;
    shop_setHelp(SHOP_TEXT_WELCOME);
    text_setBank(g_shopText);
    help_setCallback(shop_updateHelp);
    ui_loadPortraits(s_shopImage, SHOP_PORTRAIT_COUNT);
    shop_loadImages();
}

void shop_update(void) {
    shop_drawTitle();
    shop_drawMessage();
    if (s_shop.inMode != 0) {
        s_shopModes[s_shop.kind][s_shop.cursor][SHOP_MODE_UPDATE]();
        return;
    }
    shop_inputMenu();
    shop_drawMenuAll();
    shop_onUpdate();
}

void shop_exit(void) {
    help_setCallback(NULL);
    ui_restorePortraits();
}

void shop_drawTitle(void) {
    win_drawBox(&s_shopTitleRect, SHOP_OTZ_MENU, WIN_STYLE_BEVEL);
    win_drawShadow(&s_shopTitleRect, SHOP_OTZ_SHADOW, WIN_STYLE_BEVEL);
    text_drawBlock(s_shopTitleRect.x + SHOP_TITLE_TEXT_X, s_shopTitleRect.y + SHOP_TITLE_TEXT_Y, SHOP_OTZ_MENU - 1, (u8 *)text_getFrom(g_shopText, s_shop.kind + SHOP_TEXT_TITLE), TEXT_PAL_WHITE);
}

void shop_drawMenuAll(void) {
    shop_drawMenu();
    shop_drawMenuCursor();
}

void shop_onUpdate(void) {
}

void shop_drawMenuCursor(void) {
    if (!fade_isBusy()) {
        g_menuHelp.x = SHOP_MENU_CURSOR_X;
        g_menuHelp.otz = SHOP_OTZ_CURSOR;
        g_menuHelp.y = s_shop.cursor * SHOP_ROW_H + SHOP_MENU_CURSOR_Y;
        help_drawCursor();
    }
}

void shop_onModeLeft(void) {
    s_shop.inMode = 0;
    s_shopModes[s_shop.kind][s_shop.cursor][SHOP_MODE_EXIT]();
    fade_inDefault();
    help_setCallback(shop_updateHelp);
    text_setBank(g_shopText);
    shop_setHelp(s_shop.visits != 0 ? SHOP_TEXT_ANYTHING_ELSE : SHOP_TEXT_WELCOME);
}

s32 shop_countEntries(s32 kind) {
    s32 count;
    s32 i;

    count = 1;
    for (i = 0; i < SHOP_MODE_COUNT; i++) {
        if (s_shopModes[kind][i][0] != NULL || s_shopModes[kind][i][1] != NULL || s_shopModes[kind][i][2] != NULL) {
            count++;
        }
    }
    return count;
}

void shop_selectMenu(void) {
    if (s_shop.entryCount - 1 == s_shop.cursor) {
        shop_close();
        return;
    }
    s_shop.visits++;
    fade_outDefault();
    fade_setOnDone(shop_enterMode);
}

void shop_updateHelp(void) {
    help_setBounds(&s_shopHelpRect);
    g_menuHelp.text = text_get(s_shopMenuHelpText[s_shop.kind] + s_shop.cursor);
}

void shop_close(void) {
    fade_setOtz(s_shop.savedOtz);
    menu_close();
}

void shop_loadImages(void) {
    u8 *img = s_shopIconImage;

    LoadImage(&s_shopIconClutRect, (u32 *)img);
    LoadImage(&s_shopIconRect, (u32 *)(img + 0x60));
    DrawSync(0);
}

s32 shop_getKind(s32 id) {
    if (id < SHOP_ID_SYNTH) {
        return shop_isEquipmentShop(id);
    }
    return SHOP_KIND_SYNTH;
}

void shop_enterMode(void) {
    s_shop.inMode = 1;
    s_shopModes[s_shop.kind][s_shop.cursor][SHOP_MODE_INIT]();
    fade_inDefault();
    fade_setOnDone(shop_onModeEntered);
}

void shop_onModeEntered(void) {
    s_shop.messageOtz = SHOP_OTZ_MODE;
}

void shopBuy_init(void) {
    s_shopBuy.state = SHOP_ST_LIST;
    shopBuy_loadList();
    help_setBounds(NULL);
    help_setCallback(shopBuy_updateHelp);
    setRECT(&s_shopBuy.infoRect, SHOP_INFO_WIN_X, SHOP_INFO_WIN_Y, SHOP_INFO_WIN_W, s_shopBuy.hasEquipment ? SHOP_LIST_WIN_H_EQUIP : SHOP_LIST_WIN_H);
    setRECT(&s_shopBuy.listRect, SHOP_LIST_WIN_X, SHOP_LIST_WIN_Y, SHOP_LIST_WIN_W, s_shopBuy.hasEquipment ? SHOP_LIST_WIN_H_EQUIP : SHOP_LIST_WIN_H);
    setRECT(&s_shopBuy.partyRect, SHOP_PARTY_WIN_X, SHOP_PARTY_WIN_Y, SHOP_PARTY_WIN_W, SHOP_PARTY_WIN_H);
    setRECT(&s_shopBuy.quantityRect, SHOP_QTY_WIN_X, SHOP_QTY_WIN_Y, SHOP_QTY_WIN_W, SHOP_QTY_WIN_H);
    text_setBank(g_shopBuyText);
    shop_setHelp(SHOP_TEXT_BUY_WHAT);
    menuList_init(&s_shopBuy.list, s_shopBuy.count, s_shopBuy.hasEquipment ? SHOP_LIST_ROWS_EQUIP : SHOP_LIST_ROWS, 1, s_shopBuy.listRect.h);
    shop_collectMembers();
}

void shopBuy_inputQuantity(void) {
    s32 qty;
    s32 qtyDown;

    if (g_padRepeat & BTN_CIRCLE) {
        snd_playSe(SE_CANCEL);
        shopBuy_closeQuantity();
        return;
    }
    if ((g_padRepeat & PAD_RIGHT) && s_shopBuy.quantity < s_shopBuy.maxQuantity) {
        snd_playSe(SE_CURSOR);
        s_shopBuy.quantity++;
        return;
    }
    if ((g_padRepeat & PAD_LEFT) && s_shopBuy.quantity >= 2) {
        snd_playSe(SE_CURSOR);
        s_shopBuy.quantity--;
        return;
    }
    if ((g_padRepeat & PAD_UP) && s_shopBuy.quantity < s_shopBuy.maxQuantity) {
        snd_playSe(SE_CURSOR);
        qty = s_shopBuy.quantity + SHOP_QUANTITY_STEP;
        if (qty > s_shopBuy.maxQuantity) {
            qty = s_shopBuy.maxQuantity;
        }
        s_shopBuy.quantity = qty;
        return;
    }
    if ((g_padRepeat & PAD_DOWN) && s_shopBuy.quantity >= 2) {
        snd_playSe(SE_CURSOR);
        qtyDown = s_shopBuy.quantity - SHOP_QUANTITY_STEP;
        qty = 1;
        if (qtyDown > 0) {
            qty = qtyDown;
        }
        s_shopBuy.quantity = qty;
        return;
    }
    if (g_padRepeat & BTN_CROSS) {
        snd_playSe(SE_SHOP);
        shopBuy_confirm();
    }
}

void shopBuy_drawInfo(void) {
    u8 numText[16];
    RECT *rect;
    MenuList *list;
    u8 id;
    u8 equip;
    s32 i;

    rect = &s_shopBuy.infoRect;
    list = &s_shopBuy.list;
    id = s_shopBuy.items[list->top + list->cursor];
    equip = (g_itemTable[id].slots & ITEM_SLOT_EQUIP) != 0;
    ui_drawLabel(rect->x + WIN_TITLE_X, rect->y + WIN_TITLE_Y, SHOP_OTZ_TEXT, g_labelInfo, UI_LABEL_NORMAL);
    win_drawBox(rect, SHOP_OTZ_WIN, WIN_STYLE_LINE_RULE);
    for (i = 0; i < (s32)ARRAY_COUNT(s_shopBuyInfoRowY); i++) {
        text_drawBlock(rect->x + SHOP_INFO_TEXT_X, rect->y + s_shopBuyInfoRowY[i] - 4, SHOP_OTZ_TEXT, text_getFrom(g_shopText, s_shopBuyInfoRowText[i]),
                      (s_shopBuyInfoRowAlways[i] | equip) ? TEXT_PAL_WHITE : TEXT_PAL_GREY);
    }
    text_formatNumber(g_gameState->gameData->gil, numText, 7, TEXT_NUM_ASCII);
    strcat(numText, s_shopBuyInfoGil);
    ui_drawSmallDigits(rect->x + SHOP_GIL_X, rect->y + s_shopBuyInfoRowY[0], SHOP_OTZ_TEXT, numText, TEXT_PAL_WHITE);
    text_formatNumber(item_getCount(id), numText, 2, TEXT_NUM_ASCII);
    ui_drawSmallDigits(rect->x + SHOP_COUNT_X, rect->y + s_shopBuyInfoRowY[1], SHOP_OTZ_TEXT, numText, TEXT_PAL_WHITE);
    text_formatNumber(item_countEquipped(id), numText, 2, TEXT_NUM_ASCII);
    ui_drawSmallDigits(rect->x + SHOP_COUNT_X, rect->y + s_shopBuyInfoRowY[2], SHOP_OTZ_TEXT, numText, equip ? TEXT_PAL_WHITE : TEXT_PAL_GREY);
}

void shopBuy_drawList(void) {
    RECT sub;
    MenuView view;
    s32 w;
    s32 i;
    MenuList *list;
    RECT *rect = &s_shopBuy.listRect;

    w = ui_measureLabel((u8 *)g_winLabelStrings.shopPrice);
    ui_drawLabel(rect->x + SHOP_LIST_LABEL_X, rect->y + WIN_TITLE_Y, SHOP_OTZ_WIN, g_winLabelStrings.itemList, UI_LABEL_NORMAL);
    ui_drawLabel((rect->x + SHOP_PRICE_LABEL_R) - w, rect->y + WIN_TITLE_Y, SHOP_OTZ_WIN, g_winLabelStrings.shopPrice, UI_LABEL_NORMAL);
    list = &s_shopBuy.list;
    win_drawListBox(rect, SHOP_OTZ_WIN, 1, list);
    menuList_getView(list, &view);
    win_setClip(NULL, SHOP_OTZ_TEXT, WIN_STYLE_LINE_RULE);
    for (i = 0; i < view.rows; i++) {
        shopBuy_drawRow(0, i, view.first + i, &view);
    }
    if (list->scroll & MENULIST_SCROLL_PAGE) {
        menuList_getRowsRect(list, rect, &sub);
        win_setClip(&sub, SHOP_OTZ_TEXT, WIN_STYLE_LINE_RULE);
        shopBuy_drawScrollRows();
    }
    win_setClip(rect, SHOP_OTZ_TEXT, WIN_STYLE_LINE_RULE);
}

void shopBuy_drawParty(void) {
    u8 numText[8];
    Equip5 eq;
    RECT *rect;
    MenuList *list;
    Character *chr;
    u8 id;
    s32 kind;
    u8 armor;
    u8 ok;
    s32 cur;
    s32 new;
    s32 i;
    s32 x;
    s32 y;

    rect = &s_shopBuy.partyRect;
    list = &s_shopBuy.list;
    id = s_shopBuy.items[list->top + list->cursor];
    if (s_shopBuy.hasEquipment == 0) {
        return;
    }
    win_drawBox(rect, SHOP_OTZ_WIN, WIN_STYLE_LINE_RULE);
    kind = item_getEquipSlot(id);
    if (kind < 0) {
        return;
    }
    armor = kind > 0;
    ui_drawLabel(rect->x + WIN_TITLE_X, rect->y + WIN_TITLE_Y, SHOP_OTZ_PARTY_LABEL, ((u8 **)&g_winLabelStrings)[s_shopBuyPartyLabel[armor]], UI_LABEL_NORMAL);
    for (i = 0; i < s_shop.memberCount; i++) {
        x = SHOP_PARTY_X + i * SHOP_PARTY_COL_W;
        chr = &g_gameState->gameData->characters[(u8)s_shop.members[i]];
        ok = (g_itemTable[id].equipMask & s_shopBuyEquipMask[chr_getDefaultSet(chr)]) != 0;
        y = SHOP_PARTY_Y;
        if (ok) {
            if (!armor) {
                new = g_weaponTable[id].attack;
                cur = g_weaponTable[chr->equip[0]].attack;
            } else {
                eq = *(Equip5 *)chr->equip;
                eq.v[kind] = id;
                cur = shop_sumArmorStat(kind, chr->equip);
                new = shop_sumArmorStat(kind, eq.v);
            }
            if (kind == EQUIP_SLOT_ACCESSORY) {
                *(s32 *)numText = s_shopBuyNoStat;
            } else {
                text_formatNumber(new, numText, 3, TEXT_NUM_ASCII);
            }
            shop_drawStatIcon(x - 1, y + SHOP_STAT_Y, SHOP_OTZ_TEXT, s_shopBuyStatIcon[kind], 0);
            ui_drawSmallDigits(x + SHOP_STAT_DIGITS_X, y + (SHOP_STAT_Y + 1), SHOP_OTZ_TEXT, numText, TEXT_PAL_WHITE);
            if (cur < new) {
                shop_drawStatIcon(x - 1, y + SHOP_STAT_UP_Y, SHOP_OTZ_TEXT, SHOP_ICON_UP, 0);
            } else if (new < cur) {
                shop_drawStatIcon(x - 1, y + SHOP_STAT_DOWN_Y, SHOP_OTZ_TEXT, SHOP_ICON_DOWN, 0);
            }
        }
        ui_drawPortraitId(x, y, SHOP_OTZ_TEXT, chr_getPortrait((u8)s_shop.members[i]), SHOP_PORTRAIT_H, !ok);
    }
}

void shopBuy_drawQuantityWindow(Window *w) {
    u8 numText[16];
    ShopBuy *buy;
    MenuList *list;
    ItemInfo *it;
    u8 id;

    list = &s_shopBuy.list;
    buy = &s_shopBuy;
    id = buy->items[list->top + list->cursor];
    it = &g_itemTable[id];
    ui_drawItemIcon(w->x + SHOP_QTY_ICON_X, w->y + SHOP_QTY_ITEM_Y, w->otz, id, 1);
    text_drawBlock(w->x + SHOP_QTY_NAME_X, w->y + SHOP_QTY_ITEM_Y, w->otz, g_itemNames + it->name, TEXT_PAL_WHITE);
    ui_drawEtchedLine(w->x + 4, w->y + SHOP_QTY_DIV_Y, w->otz, w->w - 9);
    text_drawBlock(w->x + SHOP_QTY_TEXT_X, w->y + SHOP_QTY_QUANTITY_Y, w->otz, text_getFrom(g_shopText, SHOP_TEXT_QUANTITY), TEXT_PAL_WHITE);
    text_drawBlock(w->x + SHOP_QTY_TEXT_X, w->y + SHOP_QTY_PRICE_Y, w->otz, text_getFrom(g_shopText, SHOP_TEXT_PRICE), TEXT_PAL_WHITE);
    text_formatNumber(buy->quantity, numText, 2, TEXT_NUM_ASCII);
    ui_drawSmallDigits(w->x + SHOP_COUNT_X, w->y + (SHOP_QTY_QUANTITY_Y + 4), w->otz, numText, TEXT_PAL_WHITE);
    text_formatNumber(buy->quantity * it->price, numText, 7, TEXT_NUM_ASCII);
    strcat(numText, s_shopBuyQuantityGil);
    ui_drawSmallDigits(w->x + SHOP_GIL_X, w->y + (SHOP_QTY_PRICE_Y + 4), w->otz, numText, TEXT_PAL_WHITE);
}

void shopBuy_drawCursor(void) {
    if (!fade_isBusy()) {
        if (s_shopBuy.state == SHOP_ST_LIST || (g_menuFrame & 3)) {
            g_menuHelp.otz = SHOP_OTZ_CURSOR;
            g_menuHelp.x = s_shopBuy.listRect.x - SHOP_LIST_CURSOR_LEFT;
            g_menuHelp.y = s_shopBuy.listRect.y + (s16)(s_shopBuy.list.cursor * SHOP_ROW_H + SHOP_LIST_CURSOR_DY);
            help_drawCursor();
        }
        if (s_shopBuy.state == SHOP_ST_QUANTITY) {
            g_menuHelp.x = s_shopBuy.quantityWin->x - SHOP_QTY_CURSOR_LEFT;
            g_menuHelp.y = s_shopBuy.quantityWin->y + (SHOP_QTY_QUANTITY_Y + 4);
            g_menuHelp.otz = s_shopBuy.quantityWin->otz - 1;
            help_drawCursor();
        }
    }
}

void shopBuy_openQuantity(void) {
    MenuList *list;
    ShopBuy *buy;
    ShopBuy *m2;
    ItemInfo *it;
    s32 have;
    u32 maxQty;
    s32 affordable;
    u8 id;

    list = &s_shopBuy.list;
    buy = &s_shopBuy;
    id = buy->items[list->top + list->cursor];
    it = &g_itemTable[id];
    have = item_getCount(id);
    if (have < ITEM_MAX_COUNT && g_gameState->gameData->gil >= it->price) {
        buy->quantity = 1;
        affordable = it->price != 0 ? g_gameState->gameData->gil / it->price : ITEM_MAX_COUNT;
        maxQty = affordable;
        m2 = &s_shopBuy;
        if ((s32)maxQty > ITEM_MAX_COUNT - have) {
            maxQty = ITEM_MAX_COUNT - have;
        }
        m2->maxQuantity = maxQty;
        s_shopBuy.state = SHOP_ST_NONE;
        m2->quantityWin = win_openFree(&m2->quantityRect, SHOP_OTZ_QTY, WIN_STYLE_BEVEL);
        win_setDrawContent(s_shopBuy.quantityWin, shopBuy_drawQuantityWindow);
        win_setOnOpened(s_shopBuy.quantityWin, shopBuy_onQuantityOpened);
        win_setOnClosed(s_shopBuy.quantityWin, shopBuy_onQuantityClosed);
        win_setShadow(s_shopBuy.quantityWin, 1);
        win_setTitle(s_shopBuy.quantityWin, g_labelQuantity);
        snd_playSe(SE_CURSOR);
    } else {
        snd_playSe(SE_BUZZER);
    }
}

void shopBuy_update(void) {
    if (g_padRepeat & PAD_START) {
        pad_onStartPressed();
    }
    shopBuy_input();
    shopBuy_draw();
    shopBuy_updateList();
}

void shopBuy_exit(void) {
    help_setCallback(NULL);
}

s32 shop_isEquipmentShop(s32 shop) {
    u8 *list;
    s32 i;
    u8 id;

    list = s_shopItems[shop];
    for (i = 0; i < SHOP_LIST_SIZE; i++) {
        if (list[i] != ITEM_NONE) {
            if (!(g_itemTable[list[i]].slots & ITEM_SLOT_ARMOR)) {
                return 0;
            }
        }
    }
    return 1;
}

void shopBuy_input(void) {
    void (*fn)(void);

    if (!fade_isBusy()) {
        fn = s_shopBuyInputFuncs[s_shopBuy.state];
        if (fn != NULL) {
            fn();
        }
    }
}

void shopBuy_draw(void) {
    shopBuy_drawList();
    shopBuy_drawInfo();
    shopBuy_drawParty();
    shopBuy_drawCursor();
}

void shopBuy_updateList(void) {
    if (s_shopBuy.list.scroll != 0) {
        menuList_update(&s_shopBuy.list);
    }
}

void shopBuy_inputList(void) {
    MenuList *list = &s_shopBuy.list;

    if (list->scroll == 0) {
        if (g_padRepeat & BTN_CIRCLE) {
            snd_playSe(SE_CANCEL);
            shop_leaveMode();
            return;
        }
        if (g_padRepeat & BTN_CROSS) {
            shopBuy_openQuantity();
            return;
        }
        menuList_input(list, g_padRepeat);
    }
}

void shopBuy_drawScrollRows(void) {
    MenuView view;
    s32 i;

    menuList_getPageView(&s_shopBuy.list, &view);
    for (i = 0; i < view.rows; i++) {
        shopBuy_drawRow(0, i, view.first + i, &view);
    }
}

void shopBuy_drawRow(s32 unused, s32 row, s32 idx, MenuView *view) {
    u8 numText[16];
    ItemInfo *it;
    s32 y;
    s32 x;
    u8 id;
    u8 ok;

    ok = 0;
    id = s_shopBuy.items[idx];
    it = &g_itemTable[id];
    if (item_getCount(id) < ITEM_MAX_COUNT) {
        ok = g_gameState->gameData->gil >= it->price;
    }
    x = SHOP_ROW_ICON_X;
    y = SHOP_LIST_WIN_Y;
    y = row * SHOP_ROW_H + (view->y + y);
    text_formatNumber(it->price, numText, 5, TEXT_NUM_ASCII);
    ui_drawItemIcon(x, y, SHOP_OTZ_TEXT, id, ok);
    text_drawBlock(x + SHOP_ROW_NAME_DX, y, SHOP_OTZ_TEXT, g_itemNames + it->name, ok ? TEXT_PAL_WHITE : TEXT_PAL_GREY);
    ui_drawSmallDigits(x + SHOP_ROW_PRICE_DX, y + 4, SHOP_OTZ_TEXT, numText, ok ? TEXT_PAL_WHITE : TEXT_PAL_GREY);
}

void shopBuy_closeQuantity(void) {
    s_shopBuy.state = SHOP_ST_NONE;
    win_close(s_shopBuy.quantityWin);
}

void shopBuy_onQuantityOpened(Window *w) {
    s_shopBuy.state = SHOP_ST_QUANTITY;
    shop_setHelp(SHOP_TEXT_BUY_HOW_MANY);
}

void shopBuy_onQuantityClosed(Window *w) {
    s_shopBuy.state = SHOP_ST_LIST;
    shop_setHelp(SHOP_TEXT_BUY_WHAT);
}

void shopBuy_loadList(void) {
    s32 i;

    s_shopBuy.items = s_shopItems[shop_getId()];
    s_shopBuy.hasEquipment = 0;
    s_shopBuy.count = 0;
    for (i = 0; i < SHOP_LIST_SIZE; i++) {
        if (s_shopBuy.items[i] == ITEM_NONE) {
            break;
        }
        if (g_itemTable[s_shopBuy.items[i]].slots & ITEM_SLOT_EQUIP) {
            s_shopBuy.hasEquipment = 1;
        }
        s_shopBuy.count++;
    }
}

void shopBuy_confirm(void) {
    MenuList *list;
    ShopBuy *buy;
    ItemInfo *it;
    u8 id;
    s32 added;

    list = &s_shopBuy.list;
    buy = &s_shopBuy;
    id = buy->items[list->top + list->cursor];
    it = &g_itemTable[id];
    added = item_add(id, buy->quantity);
    if (added != 0) {
        g_gameState->gameData->gil -= it->price * added;
        shopBuy_closeQuantity();
    }
}

void shopBuy_updateHelp(void) {
    MenuList *list;
    u8 id;

    g_menuHelp.text = NULL;
    list = &s_shopBuy.list;
    if (s_shopBuy.state == SHOP_ST_LIST) {
        id = s_shopBuy.items[list->top + list->cursor];
        g_menuHelp.text = s_shopItemHelp + g_itemTable[id].help;
    } else if (s_shopBuy.state == SHOP_ST_QUANTITY) {
        g_menuHelp.text = text_get(SHOP_BUY_TEXT_QUANTITY_HELP);
    }
}

void shopSynth_init(void) {
    shopSynth_loadRecipes();
    s_shopSynth.state = SHOP_ST_LIST;
    help_setBounds(NULL);
    help_setCallback(shopSynth_updateHelp);
    setRECT(&s_shopSynth.infoRect, SHOP_INFO_WIN_X, SHOP_INFO_WIN_Y, SHOP_INFO_WIN_W, s_shopSynth.hasEquipment ? SHOP_LIST_WIN_H_EQUIP : SHOP_LIST_WIN_H);
    setRECT(&s_shopSynth.listRect, SHOP_LIST_WIN_X, SHOP_LIST_WIN_Y, SHOP_LIST_WIN_W, s_shopSynth.hasEquipment ? SHOP_LIST_WIN_H_EQUIP : SHOP_LIST_WIN_H);
    setRECT(&s_shopSynth.partyRect, SHOP_PARTY_WIN_X, SHOP_PARTY_WIN_Y, SHOP_PARTY_WIN_W, SHOP_PARTY_WIN_H);
    setRECT(&s_shopSynth.quantityRect, SHOP_QTY_WIN_X, SHOP_QTY_WIN_Y, SHOP_QTY_WIN_W, SHOP_QTY_WIN_H);
    text_setBank(g_shopSynthText);
    shop_setHelp(SHOP_TEXT_SYNTH_WHAT);
    menuList_init(&s_shopSynth.list, s_shopSynth.count, s_shopSynth.hasEquipment ? SHOP_LIST_ROWS_EQUIP : SHOP_LIST_ROWS, 1, s_shopSynth.listRect.h);
    shop_collectMembers();
}

void shopSynth_inputQuantity(void) {
    s32 qty;
    s32 qtyDown;

    if (g_padRepeat & BTN_CIRCLE) {
        snd_playSe(SE_CANCEL);
        shopSynth_closeQuantity();
        return;
    }
    if ((g_padRepeat & PAD_RIGHT) && s_shopSynth.quantity < s_shopSynth.maxQuantity) {
        snd_playSe(SE_CURSOR);
        s_shopSynth.quantity++;
        return;
    }
    if ((g_padRepeat & PAD_LEFT) && s_shopSynth.quantity >= 2) {
        snd_playSe(SE_CURSOR);
        s_shopSynth.quantity--;
        return;
    }
    if ((g_padRepeat & PAD_UP) && s_shopSynth.quantity < s_shopSynth.maxQuantity) {
        snd_playSe(SE_CURSOR);
        qty = s_shopSynth.quantity + SHOP_QUANTITY_STEP;
        if (qty > s_shopSynth.maxQuantity) {
            qty = s_shopSynth.maxQuantity;
        }
        s_shopSynth.quantity = qty;
        return;
    }
    if ((g_padRepeat & PAD_DOWN) && s_shopSynth.quantity >= 2) {
        snd_playSe(SE_CURSOR);
        qtyDown = s_shopSynth.quantity - SHOP_QUANTITY_STEP;
        qty = 1;
        if (qtyDown > 0) {
            qty = qtyDown;
        }
        s_shopSynth.quantity = qty;
        return;
    }
    if (g_padRepeat & BTN_CROSS) {
        snd_playSe(SE_SHOP);
        shopSynth_confirm();
    }
}

void shopSynth_drawInfo(void) {
    u8 numText[16];
    RECT *rect;
    MenuList *list;
    SynthRecipe *recipe;
    u8 equip;
    u8 ok;
    s32 i;
    s32 have;

    rect = &s_shopSynth.infoRect;
    list = &s_shopSynth.list;
    recipe = &s_shopSynth.recipes[list->top + list->cursor];
    equip = (g_itemTable[recipe->product].slots & ITEM_SLOT_EQUIP) != 0;
    ui_drawEtchedLine(rect->x + 2, rect->y + SHOP_SYNTH_DIV_Y, SHOP_OTZ_WIN, rect->w - 5);
    win_drawBox(rect, SHOP_OTZ_WIN, WIN_STYLE_LINE_RULE);
    ui_drawLabel(rect->x + WIN_TITLE_X, rect->y + WIN_TITLE_Y, SHOP_OTZ_TEXT, g_labelInfo, UI_LABEL_NORMAL);
    for (i = 0; i < (s32)ARRAY_COUNT(s_shopSynthInfoRowY); i++) {
        text_drawBlock(rect->x + SHOP_INFO_TEXT_X, rect->y + s_shopSynthInfoRowY[i] - 4, SHOP_OTZ_TEXT, text_getFrom(g_shopText, s_shopSynthInfoRowText[i]),
                      (s_shopSynthInfoRowAlways[i] | equip) ? TEXT_PAL_WHITE : TEXT_PAL_GREY);
    }
    text_formatNumber(g_gameState->gameData->gil, numText, 7, TEXT_NUM_ASCII);
    strcat(numText, s_shopSynthInfoGil);
    ui_drawSmallDigits(rect->x + SHOP_GIL_X, rect->y + s_shopSynthInfoRowY[0], SHOP_OTZ_TEXT, numText, TEXT_PAL_WHITE);
    text_formatNumber(item_getCount(recipe->product), numText, 2, TEXT_NUM_ASCII);
    ui_drawSmallDigits(rect->x + SHOP_COUNT_X, rect->y + s_shopSynthInfoRowY[1], SHOP_OTZ_TEXT, numText, TEXT_PAL_WHITE);
    text_formatNumber(item_countEquipped(recipe->product), numText, 2, TEXT_NUM_ASCII);
    ui_drawSmallDigits(rect->x + SHOP_COUNT_X, rect->y + s_shopSynthInfoRowY[2], SHOP_OTZ_TEXT, numText, equip ? TEXT_PAL_WHITE : TEXT_PAL_GREY);
    for (i = 0; i < 2; i++) {
        if (recipe->ingredients[i] != ITEM_NONE) {
            have = item_getCount(recipe->ingredients[i]);
            if (recipe->ingredients[0] == recipe->ingredients[1]) {
                ok = i < have;
            } else {
                ok = have != 0;
            }
            ui_drawItemIcon(rect->x + SHOP_INGREDIENT_ICON_X, rect->y + SHOP_INGREDIENT_Y + i * SHOP_INGREDIENT_ROW_H, SHOP_OTZ_TEXT, recipe->ingredients[i], ok);
            text_drawBlock(rect->x + (SHOP_INGREDIENT_ICON_X + 0x10), rect->y + (SHOP_INGREDIENT_Y + 1) + i * SHOP_INGREDIENT_ROW_H, SHOP_OTZ_TEXT, g_itemNames + g_itemTable[recipe->ingredients[i]].name,
                          ok ? TEXT_PAL_WHITE : TEXT_PAL_GREY);
        }
    }
}

void shopSynth_drawList(void) {
    RECT sub;
    MenuView view;
    s32 w;
    s32 i;
    MenuList *list;
    RECT *rect = &s_shopSynth.listRect;

    w = ui_measureLabel((u8 *)g_winLabelStrings.shopPrice);
    ui_drawLabel(rect->x + SHOP_LIST_LABEL_X, rect->y + WIN_TITLE_Y, SHOP_OTZ_WIN, g_winLabelStrings.itemList, UI_LABEL_NORMAL);
    ui_drawLabel((rect->x + SHOP_PRICE_LABEL_R) - w, rect->y + WIN_TITLE_Y, SHOP_OTZ_WIN, g_winLabelStrings.shopPrice, UI_LABEL_NORMAL);
    list = &s_shopSynth.list;
    win_drawListBox(rect, SHOP_OTZ_WIN, 1, list);
    menuList_getView(list, &view);
    win_setClip(NULL, SHOP_OTZ_TEXT, WIN_STYLE_LINE_RULE);
    for (i = 0; i < view.rows; i++) {
        shopSynth_drawRow(0, i, view.first + i, &view);
    }
    if (list->scroll & MENULIST_SCROLL_PAGE) {
        menuList_getRowsRect(list, rect, &sub);
        win_setClip(&sub, SHOP_OTZ_TEXT, WIN_STYLE_LINE_RULE);
        shopSynth_drawScrollRows();
    }
    win_setClip(rect, SHOP_OTZ_TEXT, WIN_STYLE_LINE_RULE);
}

void shopSynth_drawParty(void) {
    u8 numText[8];
    Equip5 eq;
    RECT *rect;
    MenuList *list;
    Character *chr;
    SynthRecipe *recipe;
    s32 kind;
    u8 armor;
    u8 ok;
    s32 cur;
    s32 new;
    s32 i;
    s32 x;
    s32 y;

    rect = &s_shopSynth.partyRect;
    list = &s_shopSynth.list;
    recipe = &s_shopSynth.recipes[list->top + list->cursor];
    if (s_shopSynth.hasEquipment == 0) {
        return;
    }
    win_drawBox(rect, SHOP_OTZ_WIN, WIN_STYLE_LINE_RULE);
    kind = item_getEquipSlot(recipe->product);
    if (kind < 0) {
        return;
    }
    armor = kind > 0;
    ui_drawLabel(rect->x + WIN_TITLE_X, rect->y + WIN_TITLE_Y, SHOP_OTZ_PARTY_LABEL, ((u8 **)&g_winLabelStrings)[s_shopSynthPartyLabel[armor]], UI_LABEL_NORMAL);
    for (i = 0; i < s_shop.memberCount; i++) {
        x = SHOP_PARTY_X + i * SHOP_PARTY_COL_W;
        chr = &g_gameState->gameData->characters[(u8)s_shop.members[i]];
        ok = (g_itemTable[recipe->product].equipMask & s_shopSynthEquipMask[chr_getDefaultSet(chr)]) != 0;
        y = SHOP_PARTY_Y;
        if (ok) {
            if (!armor) {
                new = g_weaponTable[recipe->product].attack;
                cur = g_weaponTable[chr->equip[0]].attack;
            } else {
                eq = *(Equip5 *)chr->equip;
                eq.v[kind] = recipe->product;
                cur = shop_sumArmorStat(kind, chr->equip);
                new = shop_sumArmorStat(kind, eq.v);
            }
            if (kind == EQUIP_SLOT_ACCESSORY) {
                *(s32 *)numText = s_shopSynthNoStat;
            } else {
                text_formatNumber(new, numText, 3, TEXT_NUM_ASCII);
            }
            shop_drawStatIcon(x - 1, y + SHOP_STAT_Y, SHOP_OTZ_TEXT, s_shopSynthStatIcon[kind], 0);
            ui_drawSmallDigits(x + SHOP_STAT_DIGITS_X, y + (SHOP_STAT_Y + 1), SHOP_OTZ_TEXT, numText, TEXT_PAL_WHITE);
            if (cur < new) {
                shop_drawStatIcon(x - 1, y + SHOP_STAT_UP_Y, SHOP_OTZ_TEXT, SHOP_ICON_UP, 0);
            } else if (new < cur) {
                shop_drawStatIcon(x - 1, y + SHOP_STAT_DOWN_Y, SHOP_OTZ_TEXT, SHOP_ICON_DOWN, 0);
            }
        }
        ui_drawPortraitId(x, y, SHOP_OTZ_TEXT, chr_getPortrait((u8)s_shop.members[i]), SHOP_PORTRAIT_H, !ok);
    }
}

void shopSynth_drawQuantityWindow(Window *w) {
    u8 numText[16];
    ShopSynth *synth;
    MenuList *list;
    SynthRecipe *recipe;
    ItemInfo *it;
    u8 id;

    list = &s_shopSynth.list;
    recipe = &s_shopSynth.recipes[list->top + list->cursor];
    id = recipe->product;
    it = &g_itemTable[id];
    ui_drawItemIcon(w->x + SHOP_QTY_ICON_X, w->y + SHOP_QTY_ITEM_Y, w->otz, id, 1);
    text_drawBlock(w->x + SHOP_QTY_NAME_X, w->y + SHOP_QTY_ITEM_Y, w->otz, g_itemNames + it->name, TEXT_PAL_WHITE);
    ui_drawEtchedLine(w->x + 4, w->y + SHOP_QTY_DIV_Y, w->otz, w->w - 9);
    text_drawBlock(w->x + SHOP_QTY_TEXT_X, w->y + SHOP_QTY_QUANTITY_Y, w->otz, text_getFrom(g_shopText, SHOP_TEXT_QUANTITY), TEXT_PAL_WHITE);
    text_drawBlock(w->x + SHOP_QTY_TEXT_X, w->y + SHOP_QTY_PRICE_Y, w->otz, text_getFrom(g_shopText, SHOP_TEXT_PRICE), TEXT_PAL_WHITE);
    synth = &s_shopSynth;
    text_formatNumber(synth->quantity, numText, 2, TEXT_NUM_ASCII);
    ui_drawSmallDigits(w->x + SHOP_COUNT_X, w->y + (SHOP_QTY_QUANTITY_Y + 4), w->otz, numText, TEXT_PAL_WHITE);
    text_formatNumber(synth->quantity * recipe->price, numText, 7, TEXT_NUM_ASCII);
    strcat(numText, s_shopSynthQuantityGil);
    ui_drawSmallDigits(w->x + SHOP_GIL_X, w->y + (SHOP_QTY_PRICE_Y + 4), w->otz, numText, TEXT_PAL_WHITE);
}

void shopSynth_drawCursor(void) {
    if (!fade_isBusy()) {
        if (s_shopSynth.state == SHOP_ST_LIST || (g_menuFrame & 3)) {
            g_menuHelp.otz = SHOP_OTZ_CURSOR;
            g_menuHelp.x = s_shopSynth.listRect.x - SHOP_LIST_CURSOR_LEFT;
            g_menuHelp.y = s_shopSynth.listRect.y + (s16)(s_shopSynth.list.cursor * SHOP_ROW_H + SHOP_LIST_CURSOR_DY);
            help_drawCursor();
        }
        if (s_shopSynth.state == SHOP_ST_QUANTITY) {
            g_menuHelp.x = s_shopSynth.quantityWin->x - SHOP_QTY_CURSOR_LEFT;
            g_menuHelp.y = s_shopSynth.quantityWin->y + (SHOP_QTY_QUANTITY_Y + 4);
            g_menuHelp.otz = s_shopSynth.quantityWin->otz - 1;
            help_drawCursor();
        }
    }
}

void shopSynth_drawRow(s32 unused, s32 row, s32 idx, MenuView *view) {
    u8 numText[16];
    SynthRecipe *recipe;
    ItemInfo *it;
    s32 i;
    s32 avail;
    s32 x;
    s32 y;
    u8 ok;
    u8 id;
    s32 k;

    recipe = &s_shopSynth.recipes[idx];
    ok = 0;
    id = recipe->product;
    it = &g_itemTable[id];
    k = 1;
    if (item_getCount(id) < ITEM_MAX_COUNT) {
        ok = g_gameState->gameData->gil >= recipe->price;
    }
    x = SHOP_ROW_ICON_X;
    y = SHOP_LIST_WIN_Y;
    y = row * SHOP_ROW_H + (view->y + y);
    i = 0;
loop:
    if (recipe->ingredients[i] != ITEM_NONE) {
        if (recipe->ingredients[0] == recipe->ingredients[1]) {
            avail = i < item_getCount(recipe->ingredients[i]);
        } else {
            avail = item_getCount(recipe->ingredients[i]);
        }
        if (!avail) {
            do {
                ok = 0;
            } while (0);
        }
    }
    i++;
    if (i < 2) {
        goto loop;
    }
    text_formatNumber(recipe->price, numText, 5, k);
    id = ok;
    ui_drawItemIcon(x, y, SHOP_OTZ_TEXT, recipe->product, id);
    text_drawBlock(x + SHOP_ROW_NAME_DX, y, SHOP_OTZ_TEXT, g_itemNames + it->name, id ? TEXT_PAL_WHITE : TEXT_PAL_GREY);
    ui_drawSmallDigits(x + SHOP_ROW_PRICE_DX, y + 4, SHOP_OTZ_TEXT, numText, id ? TEXT_PAL_WHITE : TEXT_PAL_GREY);
}

void shopSynth_openQuantity(void) {
    MenuList *list;
    SynthRecipe *recipe;
    ShopSynth *synth;
    ShopSynth *m2;
    ShopSynth *m3;
    s32 need;
    s32 have;
    u32 maxQty;

    list = &s_shopSynth.list;
    recipe = &s_shopSynth.recipes[list->top + list->cursor];
    need = 1;
    if (recipe->ingredients[0] == recipe->ingredients[1]) {
        need = 2;
    }
    have = item_getCount(recipe->product);
    if (have < ITEM_MAX_COUNT && g_gameState->gameData->gil >= recipe->price &&
        (recipe->ingredients[0] == ITEM_NONE || item_getCount(recipe->ingredients[0]) >= need) &&
        (recipe->ingredients[1] == ITEM_NONE || item_getCount(recipe->ingredients[1]) >= need)) {
        s_shopSynth.quantity = 1;
        maxQty = recipe->price != 0 ? g_gameState->gameData->gil / recipe->price : ITEM_MAX_COUNT;
        m2 = &s_shopSynth;
        if ((s32)maxQty > ITEM_MAX_COUNT - have) {
            maxQty = ITEM_MAX_COUNT - have;
        }
        m2->maxQuantity = maxQty;
        synth = m2;
        if (recipe->ingredients[0] != ITEM_NONE) {
            synth->maxQuantity = MIN(synth->maxQuantity, item_getCount(recipe->ingredients[0]) / need);
        }
        if (recipe->ingredients[1] != ITEM_NONE) {
            s_shopSynth.maxQuantity = MIN(s_shopSynth.maxQuantity, item_getCount(recipe->ingredients[1]) / need);
        }
        m3 = &s_shopSynth;
        s_shopSynth.state = SHOP_ST_NONE;
        m3->quantityWin = win_openFree(&m3->quantityRect, SHOP_OTZ_QTY, WIN_STYLE_BEVEL);
        win_setDrawContent(m3->quantityWin, shopSynth_drawQuantityWindow);
        win_setOnOpened(m3->quantityWin, shopSynth_onQuantityOpened);
        win_setOnClosed(m3->quantityWin, shopSynth_onQuantityClosed);
        win_setShadow(m3->quantityWin, 1);
        win_setTitle(m3->quantityWin, g_labelQuantity);
        snd_playSe(SE_CURSOR);
    } else {
        snd_playSe(SE_BUZZER);
    }
}

void shopSynth_loadRecipes(void) {
    s32 shopBit;
    s32 i;
    s32 recipeCount;

    recipeCount = SHOP_RECIPE_COUNT;
    shopBit = 1 << (shop_getId() - SHOP_ID_SYNTH);
    s_shopSynth.count = 0;
    for (i = 0; i < recipeCount; i++) {
        if ((s_shopRecipes[i].shopMask & shopBit) && s_shopRecipes[i].product != ITEM_NONE) {
            s_shopSynth.recipes[s_shopSynth.count++] = s_shopRecipes[i];
            if (s_shopSynth.count >= SHOP_LIST_SIZE) {
                break;
            }
        }
    }
    for (i = 0; i < s_shopSynth.count; i++) {
        if (g_itemTable[s_shopSynth.recipes[i].product].slots & ITEM_SLOT_EQUIP) {
            s_shopSynth.hasEquipment = 1;
        }
    }
}

void shopSynth_update(void) {
    shopSynth_input();
    shopSynth_draw();
    shopSynth_updateList();
}

void shopSynth_exit(void) {
    help_setCallback(NULL);
}

void shopSynth_input(void) {
    void (*fn)(void);

    if (!fade_isBusy()) {
        fn = s_shopSynthInputFuncs[s_shopSynth.state];
        if (fn != NULL) {
            fn();
        }
    }
}

void shopSynth_draw(void) {
    shopSynth_drawList();
    shopSynth_drawInfo();
    shopSynth_drawParty();
    shopSynth_drawCursor();
}

void shopSynth_updateList(void) {
    if (s_shopSynth.list.scroll != 0) {
        menuList_update(&s_shopSynth.list);
    }
}

void shopSynth_inputList(void) {
    MenuList *list = &s_shopSynth.list;

    if (list->scroll == 0) {
        if (g_padRepeat & BTN_CIRCLE) {
            snd_playSe(SE_CANCEL);
            shop_leaveMode();
            return;
        }
        if (g_padRepeat & BTN_CROSS) {
            shopSynth_openQuantity();
            return;
        }
        menuList_input(list, g_padRepeat);
    }
}

void shopSynth_drawScrollRows(void) {
    MenuView view;
    s32 i;

    menuList_getPageView(&s_shopSynth.list, &view);
    for (i = 0; i < view.rows; i++) {
        shopSynth_drawRow(0, i, view.first + i, &view);
    }
}

void shopSynth_closeQuantity(void) {
    s_shopSynth.state = SHOP_ST_NONE;
    win_close(s_shopSynth.quantityWin);
}

void shopSynth_onQuantityOpened(void) {
    s_shopSynth.state = SHOP_ST_QUANTITY;
    shop_setHelp(SHOP_TEXT_SYNTH_HOW_MANY);
}

void shopSynth_onQuantityClosed(void) {
    s_shopSynth.state = SHOP_ST_LIST;
    shop_setHelp(SHOP_TEXT_SYNTH_WHAT);
}

void shopSynth_confirm(void) {
    MenuList *list = &s_shopSynth.list;
    SynthRecipe *recipe;
    s32 made;

    recipe = &s_shopSynth.recipes[list->top + list->cursor];
    made = item_add(recipe->product, s_shopSynth.quantity);
    if (made != 0) {
        item_remove(recipe->ingredients[0], made);
        item_remove(recipe->ingredients[1], made);
        g_gameState->gameData->gil -= recipe->price * made;
        shopSynth_closeQuantity();
    }
}

void shopSynth_updateHelp(void) {
    MenuList *list;
    u8 id;

    g_menuHelp.text = NULL;
    list = &s_shopSynth.list;
    if (s_shopSynth.state == SHOP_ST_LIST) {
        id = s_shopSynth.recipes[list->top + list->cursor].product;
        g_menuHelp.text = s_shopItemHelp + g_itemTable[id].help;
    } else if (s_shopSynth.state == SHOP_ST_QUANTITY) {
        g_menuHelp.text = text_get(SHOP_SYNTH_TEXT_QUANTITY_HELP);
    }
}

void shopSell_init(void) {
    s_shopSell.state = SHOP_ST_LIST;
    text_setBank(g_shopSellText);
    shop_setHelp(SHOP_TEXT_SELL_WHAT);
    help_setBounds(NULL);
    help_setCallback(shopSell_updateHelp);
    menuList_init(&s_shopSell.list, 0xFF, SHOP_SELL_ROWS, SHOP_SELL_COLS, SHOP_SELL_WIN_H);
    setRECT(&s_shopSell.listRect, SHOP_SELL_WIN_X, SHOP_SELL_WIN_Y, SHOP_SELL_WIN_W, SHOP_SELL_WIN_H);
    setRECT(&s_shopSell.winRects[0], SHOP_QTY_WIN_X, SHOP_SELL_QTY_WIN_Y, SHOP_QTY_WIN_W, SHOP_QTY_WIN_H);
    setRECT(&s_shopSell.winRects[1], SHOP_QTY_WIN_X, SHOP_SELL_INFO_WIN_Y, SHOP_QTY_WIN_W, SHOP_SELL_INFO_WIN_H);
}

void shopSell_inputQuantity(void) {
    s32 qty;
    s32 qtyDown;

    if (g_padRepeat & BTN_CIRCLE) {
        snd_playSe(SE_CANCEL);
        shopSell_closeQuantity();
        return;
    }
    if ((g_padRepeat & PAD_RIGHT) && s_shopSell.quantity < s_shopSell.maxQuantity) {
        snd_playSe(SE_CURSOR);
        s_shopSell.quantity++;
        return;
    }
    if ((g_padRepeat & PAD_LEFT) && s_shopSell.quantity >= 2) {
        snd_playSe(SE_CURSOR);
        s_shopSell.quantity--;
        return;
    }
    if ((g_padRepeat & PAD_UP) && s_shopSell.quantity < s_shopSell.maxQuantity) {
        snd_playSe(SE_CURSOR);
        qty = s_shopSell.quantity + SHOP_QUANTITY_STEP;
        if (qty > s_shopSell.maxQuantity) {
            qty = s_shopSell.maxQuantity;
        }
        s_shopSell.quantity = qty;
        return;
    }
    if ((g_padRepeat & PAD_DOWN) && s_shopSell.quantity >= 2) {
        snd_playSe(SE_CURSOR);
        qtyDown = s_shopSell.quantity - SHOP_QUANTITY_STEP;
        qty = 1;
        if (qtyDown > 0) {
            qty = qtyDown;
        }
        s_shopSell.quantity = qty;
        return;
    }
    if (g_padRepeat & BTN_CROSS) {
        snd_playSe(SE_SHOP);
        shopSell_confirm();
    }
}

void shopSell_drawList(void) {
    RECT sub;
    MenuView view;
    s32 w;
    s32 row;
    s32 col;
    s32 idx;
    MenuList *list = &s_shopSell.list;

    ui_drawLabel(s_shopSellListRectPtr->x + SHOP_LIST_LABEL_X, s_shopSellListRectPtr->y + WIN_TITLE_Y, SHOP_OTZ_WIN, g_winLabelStrings.itemList, UI_LABEL_NORMAL);
    ui_drawLabel(s_shopSellListRectPtr->x + (SHOP_LIST_LABEL_X + SHOP_SELL_COL_W), s_shopSellListRectPtr->y + WIN_TITLE_Y, SHOP_OTZ_WIN, g_winLabelStrings.itemList, UI_LABEL_NORMAL);
    w = ui_measureLabel((u8 *)g_winLabelStrings.itemCount);
    ui_drawLabel((s_shopSellListRectPtr->x + SHOP_SELL_COUNT_LABEL_R) - w, s_shopSellListRectPtr->y + WIN_TITLE_Y, SHOP_OTZ_WIN, g_winLabelStrings.itemCount, UI_LABEL_NORMAL);
    ui_drawLabel((s_shopSellListRectPtr->x + (SHOP_SELL_COUNT_LABEL_R + SHOP_SELL_COL_W)) - w, s_shopSellListRectPtr->y + WIN_TITLE_Y, SHOP_OTZ_WIN, g_winLabelStrings.itemCount, UI_LABEL_NORMAL);
    menuList_getView(list, &view);
    win_drawListBox(s_shopSellListRectPtr, SHOP_OTZ_WIN, 1, list);
    idx = view.first * 2;
    win_setClip(NULL, SHOP_OTZ_TEXT, WIN_STYLE_LINE_RULE);
    for (row = 0; row < view.rows; row++) {
        for (col = 0; col < 2; col++) {
            shopSell_drawRow(col, row, idx++, &view);
        }
    }
    if (list->scroll & MENULIST_SCROLL_PAGE) {
        menuList_getRowsRect(list, s_shopSellListRectPtr, &sub);
        win_setClip(&sub, SHOP_OTZ_TEXT, WIN_STYLE_LINE_RULE);
        shopSell_drawScrollRows();
    }
    win_setClip(s_shopSellListRectPtr, SHOP_OTZ_TEXT, WIN_STYLE_LINE_RULE);
}

void shopSell_drawQuantityWindow(Window *w) {
    u8 numText[16];
    ShopSell *sell;
    MenuList *list;
    ItemInfo *it;
    u8 id;
    u32 price;

    list = &s_shopSell.list;
    id = (g_gameState->gameData->items.slots + ((list->top + list->cursor) * 2 + list->col))->id;
    it = &g_itemTable[id];
    price = it->price >> 1;
    ui_drawItemIcon(w->x + SHOP_QTY_ICON_X, w->y + SHOP_QTY_ITEM_Y, w->otz, id, 1);
    text_drawBlock(w->x + SHOP_QTY_NAME_X, w->y + SHOP_QTY_ITEM_Y, w->otz, g_itemNames + it->name, TEXT_PAL_WHITE);
    ui_drawEtchedLine(w->x + 4, w->y + SHOP_QTY_DIV_Y, w->otz, w->w - 9);
    text_drawBlock(w->x + SHOP_QTY_TEXT_X, w->y + SHOP_QTY_QUANTITY_Y, w->otz, text_getFrom(g_shopText, SHOP_TEXT_QUANTITY), TEXT_PAL_WHITE);
    text_drawBlock(w->x + SHOP_QTY_TEXT_X, w->y + SHOP_QTY_PRICE_Y, w->otz, text_getFrom(g_shopText, SHOP_TEXT_PRICE), TEXT_PAL_WHITE);
    sell = &s_shopSell;
    text_formatNumber(sell->quantity, numText, 2, TEXT_NUM_ASCII);
    ui_drawSmallDigits(w->x + SHOP_COUNT_X, w->y + (SHOP_QTY_QUANTITY_Y + 4), w->otz, numText, TEXT_PAL_WHITE);
    text_formatNumber(sell->quantity * price, numText, 7, TEXT_NUM_ASCII);
    strcat(numText, s_shopSellQuantityGil);
    ui_drawSmallDigits(w->x + SHOP_GIL_X, w->y + (SHOP_QTY_PRICE_Y + 4), w->otz, numText, TEXT_PAL_WHITE);
}

void shopSell_drawInfoWindow(Window *w) {
    u8 numText[16];
    RECT *rect;
    MenuList *list;
    ItemSlot *slot;
    u8 equip;
    s32 i;

    rect = (RECT *)w;
    list = &s_shopSell.list;
    slot = &g_gameState->gameData->items.slots[(list->top + list->cursor) * 2 + list->col];
    equip = (g_itemTable[slot->id].slots & ITEM_SLOT_EQUIP) != 0;
    for (i = 0; i < (s32)ARRAY_COUNT(s_shopSellInfoRowY); i++) {
        text_drawBlock(rect->x + SHOP_QTY_TEXT_X, rect->y + s_shopSellInfoRowY[i] - 4, w->otz, text_getFrom(g_shopText, s_shopSellInfoRowText[i]),
                      (s_shopSellInfoRowAlways[i] | equip) ? TEXT_PAL_WHITE : TEXT_PAL_GREY);
    }
    text_formatNumber(g_gameState->gameData->gil, numText, 7, TEXT_NUM_ASCII);
    strcat(numText, s_shopSellInfoGil);
    ui_drawSmallDigits(rect->x + SHOP_GIL_X, rect->y + s_shopSellInfoRowY[0], w->otz, numText, TEXT_PAL_WHITE);
    text_formatNumber(item_getCount(slot->id), numText, 2, TEXT_NUM_ASCII);
    ui_drawSmallDigits(rect->x + SHOP_COUNT_X, rect->y + s_shopSellInfoRowY[1], w->otz, numText, TEXT_PAL_WHITE);
    if (equip) {
        text_formatNumber(item_countEquipped(slot->id), numText, 2, TEXT_NUM_ASCII);
        ui_drawSmallDigits(rect->x + SHOP_COUNT_X, rect->y + s_shopSellInfoRowY[2], w->otz, numText, TEXT_PAL_WHITE);
    }
}

void shopSell_drawCursor(void) {
    MenuList *list;

    list = &s_shopSell.list;
    if (!fade_isBusy()) {
        if (s_shopSell.state == SHOP_ST_LIST || (g_menuFrame & 3)) {
            g_menuHelp.otz = SHOP_OTZ_CURSOR;
            g_menuHelp.x = list->col * SHOP_SELL_COL_W;
            g_menuHelp.y = list->cursor * SHOP_ROW_H + SHOP_SELL_CURSOR_Y;
            help_drawCursor();
        }
        if (s_shopSell.state == SHOP_ST_QUANTITY) {
            g_menuHelp.x = s_shopSell.wins[0]->x - SHOP_QTY_CURSOR_LEFT;
            g_menuHelp.y = s_shopSell.wins[0]->y + (SHOP_QTY_QUANTITY_Y + 4);
            g_menuHelp.otz = s_shopSell.wins[0]->otz - 1;
            help_drawCursor();
        }
    }
}

void shopSell_openQuantity(void) {
    MenuList *list = &s_shopSell.list;
    ItemSlot *slot;
    s32 i;

    slot = &g_gameState->gameData->items.slots[(list->top + list->cursor) * 2 + list->col];
    if (slot->count != 0) {
        s_shopSell.quantity = 1;
        s_shopSell.maxQuantity = slot->count;
        s_shopSell.state = SHOP_ST_NONE;
        for (i = 0; i < 2; i++) {
            s_shopSell.wins[i] = win_openFree(&s_shopSell.winRects[i], s_shopSellWindows[i].otz, s_shopSellWindows[i].style);
            win_setDrawContent(s_shopSell.wins[i], s_shopSellWindows[i].draw);
            win_setOnOpened(s_shopSell.wins[i], shopSell_onQuantityOpened);
            win_setOnClosed(s_shopSell.wins[i], shopSell_onQuantityClosed);
            win_setOpenAnim(s_shopSell.wins[i], WIN_ANIM_ZOOM);
            win_setShadow(s_shopSell.wins[i], 1);
            win_setTitle(s_shopSell.wins[i], g_winLabelStrings.labels[s_shopSellWindows[i].label]);
        }
        snd_playSe(SE_CURSOR);
    } else {
        snd_playSe(SE_BUZZER);
    }
}

void shopSell_confirm(void) {
    MenuList *list;
    ShopSell *sell;
    ItemInfo *it;
    u8 id;
    u32 price;
    s32 sold;
    s32 i;

    list = &s_shopSell.list;
    sell = &s_shopSell;
    id = (g_gameState->gameData->items.slots + ((list->top + list->cursor) * 2 + list->col))->id;
    it = &g_itemTable[id];
    price = it->price >> 1;
    sold = item_remove(id, s_shopSell.quantity);
    if (sold != 0) {
        g_gameState->gameData->gil += price * sold;
        if (g_gameState->gameData->gil > GIL_MAX) {
            g_gameState->gameData->gil = GIL_MAX;
        }
        s_shopSell.state = SHOP_ST_NONE;
        for (i = 0; i < 2; i++) {
            win_close(s_shopSell.wins[i]);
        }
    }
}

void shopSell_update(void) {
    shopSell_input();
    shopSell_draw();
    shopSell_updateList();
}

void shopSell_exit(void) {
    help_setCallback(NULL);
}

void shopSell_input(void) {
    void (*fn)(void);

    if (!fade_isBusy()) {
        fn = s_shopSellInputFuncs[s_shopSell.state];
        if (fn != NULL) {
            fn();
        }
    }
}

void shopSell_draw(void) {
    shopSell_drawList();
    shopSell_drawCursor();
}

void shopSell_updateList(void) {
    if (s_shopSell.list.scroll != 0) {
        menuList_update(&s_shopSell.list);
    }
}

void shopSell_inputList(void) {
    MenuList *list = &s_shopSell.list;

    if (list->scroll == 0) {
        if (g_padRepeat & BTN_CIRCLE) {
            snd_playSe(SE_CANCEL);
            shop_leaveMode();
            return;
        }
        if (g_padRepeat & BTN_CROSS) {
            shopSell_openQuantity();
            return;
        }
        menuList_input(list, g_padRepeat);
        menuList_left(list, g_padRepeat);
        menuList_right(list, g_padRepeat);
    }
}

void shopSell_drawScrollRows(void) {
    MenuView view;
    s32 row;
    s32 col;
    s32 idx;

    menuList_getPageView(&s_shopSell.list, &view);
    idx = view.first * 2;
    for (row = 0; row < view.rows; row++) {
        for (col = 0; col < 2; col++) {
            shopSell_drawRow(col, row, idx++, &view);
        }
    }
}

void shopSell_drawRow(s32 col, s32 row, s32 i, MenuView *view) {
    ItemSlot *slot = &g_gameState->gameData->items.slots[i];
    ItemInfo *it;
    s32 x, y;
    s32 k = SHOP_SELL_WIN_Y;
    u8 numText[8];

    if (slot->count != 0) {
        it = &g_itemTable[slot->id];
        x = col * SHOP_SELL_COL_W;
        y = view->y + (row * SHOP_ROW_H + k);
        text_formatNumber(slot->count, numText, 2, TEXT_NUM_ASCII);
        text_drawBlock(x + SHOP_SELL_NAME_X, y, SHOP_OTZ_TEXT, g_itemNames + it->name, TEXT_PAL_WHITE);
        ui_drawItemIcon(x + SHOP_SELL_ICON_X, y, SHOP_OTZ_TEXT, slot->id, 1);
        ui_drawSmallDigits(x + SHOP_SELL_COUNT_X, y + 4, SHOP_OTZ_TEXT, numText, TEXT_PAL_WHITE);
    }
}

void shopSell_closeQuantity(void) {
    s32 i;

    s_shopSell.state = SHOP_ST_NONE;
    for (i = 0; i < 2; i++) {
        win_close(s_shopSell.wins[i]);
    }
}

void shopSell_onQuantityOpened(Window *w) {
    s_shopSell.state = SHOP_ST_QUANTITY;
    shop_setHelp(SHOP_TEXT_SELL_HOW_MANY);
}

void shopSell_onQuantityClosed(Window *w) {
    s_shopSell.state = SHOP_ST_LIST;
    shop_setHelp(SHOP_TEXT_SELL_WHAT);
}

void shopSell_updateHelp(void) {
    HelpState *h;
    MenuList *list;
    s32 idx;
    ItemSlot *slot;
    u8 *text;

    h = &g_menuHelp;
    h->text = NULL;
    if (s_shopSell.state == SHOP_ST_LIST) {
        list = &s_shopSell.list;
        idx = (list->top + list->cursor) * 2 + list->col;
        slot = &g_gameState->gameData->items.slots[idx];
        if (slot->count == 0) {
            return;
        }
        text = s_shopItemHelp + g_itemTable[slot->id].help;
    } else if (s_shopSell.state == SHOP_ST_QUANTITY) {
        text = text_get(SHOP_SELL_TEXT_QUANTITY_HELP);
    } else {
        return;
    }
    h->text = text;
}

INCLUDE_RODATA("asm/us/ovl_3c/nonmatchings/ovl_3c", s_shopIcons);

INCLUDE_RODATA("asm/us/ovl_3c/nonmatchings/ovl_3c", s_shopImage);

INCLUDE_RODATA("asm/us/ovl_3c/nonmatchings/ovl_3c", s_shopIconImage);

INCLUDE_RODATA("asm/us/ovl_3c/nonmatchings/ovl_3c", g_shopText);

INCLUDE_RODATA("asm/us/ovl_3c/nonmatchings/ovl_3c", g_shopBuyText);

INCLUDE_RODATA("asm/us/ovl_3c/nonmatchings/ovl_3c", g_shopSynthText);

INCLUDE_RODATA("asm/us/ovl_3c/nonmatchings/ovl_3c", g_shopSellText);

INCLUDE_RODATA("asm/us/ovl_3c/nonmatchings/ovl_3c", s_shopBuyNoStat);

INCLUDE_RODATA("asm/us/ovl_3c/nonmatchings/ovl_3c", s_shopSynthNoStat);
