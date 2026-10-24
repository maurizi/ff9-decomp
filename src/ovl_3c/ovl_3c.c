#include "ovl_3c.h"

void shop_drawMessage(void) {
    ui_drawLabel(s_shopMessageRect.x + 6, s_shopMessageRect.y - 3, s_shop.messageOtz, D_80070450, 0);
    if (s_shop.help != NULL) {
        text_drawBlock(s_shopMessageRect.x + 0xA, s_shopMessageRect.y + 6, s_shop.messageOtz, s_shop.help, 0);
    }
    win_drawBox(&s_shopMessageRect, s_shop.messageOtz, 1);
    win_drawShadow(&s_shopMessageRect, s_shop.messageOtz, 1);
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
    win_drawBox(&s_shopMenuRect, 0x23, 2);
    win_drawShadow(&s_shopMenuRect, 0x23, 2);
    for (i = 0; i < s_shop.entryCount; i++) {
        text_drawBlock(s_shopMenuRect.x + 0xA, s_shopMenuRect.y + 6 + i * 0xF, 0x1E, text_get(s_shopMenuText[s_shop.kind] + i), 0);
    }
}

s32 shop_sumArmorStat(s32 stat, u8 *equip) {
    u32 r = 0;
    u8 sum[4];
    s32 i;
    s32 j;
    Unk8006D328 *b;

    memset(sum, r, 4);
    for (i = 1; i < 5; i++) {
        if (equip[i] != ITEM_NONE) {
            if (equip[i] < 0xE0) {
                j = equip[i] - 0x58;
                b = &g_armorStatBonus[j];
                sum[0] += b->unk0;
                sum[1] += b->unk1;
                sum[2] += b->unk2;
                sum[3] += b->unk3;
            }
        }
    }
    switch (stat) {
    case 1:
        r = sum[2];
        break;
    case 2:
        r = sum[1];
        break;
    case 3:
        r = sum[0];
        break;
    case 4:
        r = 0;
        break;
    }
    return r;
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
    for (i = 0; i < 9; i++) {
        if (g_gameState->gameData->characters[i].unk34.b.unkB) {
            s_shop.members[s_shop.memberCount] = i;
            if (++s_shop.memberCount >= 8) {
                break;
            }
        }
    }
}

void shop_drawStatIcon(s32 x, s32 y, s32 z, s32 icon, u8 arg4) {
    RectTbl7 rects = s_shopIcons;

    ui_drawSpriteDef(x, y, z, &rects.r[icon], arg4);
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
    ui_loadPortraits(s_shopImage, 0xD);
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
    func_801F47F0();
}

void shop_exit(void) {
    help_setCallback(NULL);
    ui_restorePortraits();
}

void shop_drawTitle(void) {
    win_drawBox(&s_shopTitleRect, 5, 2);
    win_drawShadow(&s_shopTitleRect, 0x28, 2);
    text_drawBlock(s_shopTitleRect.x + 0xA, s_shopTitleRect.y + 6, 4, (u8 *)text_getFrom(g_shopText, s_shop.kind + SHOP_TEXT_TITLE), 0);
}

void shop_drawMenuAll(void) {
    shop_drawMenu();
    shop_drawMenuCursor();
}

void func_801F47F0(void) {
}

void shop_drawMenuCursor(void) {
    if (!fade_isBusy()) {
        g_menuHelp.x = 0x6E;
        g_menuHelp.otz = 0x14;
        g_menuHelp.y = s_shop.cursor * 0xF + 0x6A;
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
    s32 n;
    s32 i;

    n = 1;
    for (i = 0; i < SHOP_MODE_COUNT; i++) {
        if (s_shopModes[kind][i][0] != NULL || s_shopModes[kind][i][1] != NULL || s_shopModes[kind][i][2] != NULL) {
            n++;
        }
    }
    return n;
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
    u8 *p = s_shopIconImage;

    LoadImage(&s_shopIconClutRect, (u32 *)p);
    LoadImage(&s_shopIconRect, (u32 *)(p + 0x60));
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
    setRECT(&s_shopBuy.infoRect, 0xBC, 0x26, 0x78, s_shopBuy.hasEquipment ? 0x71 : 0xAD);
    setRECT(&s_shopBuy.listRect, 8, 0x26, 0xB4, s_shopBuy.hasEquipment ? 0x71 : 0xAD);
    setRECT(&s_shopBuy.partyRect, 8, 0x97, 0x12C, 0x45);
    setRECT(&s_shopBuy.quantityRect, 0x62, 0x60, 0x7C, 0x45);
    text_setBank(g_shopBuyText);
    shop_setHelp(SHOP_TEXT_BUY_WHAT);
    menuList_init(&s_shopBuy.list, s_shopBuy.count, s_shopBuy.hasEquipment ? 7 : 0xB, 1, s_shopBuy.listRect.h);
    shop_collectMembers();
}

void shopBuy_inputQuantity(void) {
    s32 q;
    s32 t;

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
        q = s_shopBuy.quantity + SHOP_QUANTITY_STEP;
        if (q > s_shopBuy.maxQuantity) {
            q = s_shopBuy.maxQuantity;
        }
        s_shopBuy.quantity = q;
        return;
    }
    if ((g_padRepeat & PAD_DOWN) && s_shopBuy.quantity >= 2) {
        snd_playSe(SE_CURSOR);
        t = s_shopBuy.quantity - SHOP_QUANTITY_STEP;
        q = 1;
        if (t > 0) {
            q = t;
        }
        s_shopBuy.quantity = q;
        return;
    }
    if (g_padRepeat & BTN_CROSS) {
        snd_playSe(SE_SHOP);
        shopBuy_confirm();
    }
}

void shopBuy_drawInfo(void) {
    u8 buf[16];
    RECT *r;
    MenuList *l;
    u8 id;
    u8 equip;
    s32 i;

    r = &s_shopBuy.infoRect;
    l = &s_shopBuy.list;
    id = s_shopBuy.items[l->top + l->cursor];
    equip = (g_itemTable[id].unkF & 0xF8) != 0;
    ui_drawLabel(r->x + 6, r->y - 3, 0x1E, D_80070460, 0);
    win_drawBox(r, 0x23, 1);
    for (i = 0; i < 3; i++) {
        text_drawBlock(r->x + 8, r->y + s_shopBuyInfoRowY[i] - 4, 0x1E, text_getFrom(g_shopText, s_shopBuyInfoRowText[i]),
                      (s_shopBuyInfoRowAlways[i] | equip) ? 0 : 2);
    }
    text_formatNumber(g_gameState->gameData->gil, buf, 7, 1);
    strcat(buf, s_shopBuyInfoGil);
    ui_drawSmallDigits(r->x + 0x39, r->y + s_shopBuyInfoRowY[0], 0x1E, buf, 0);
    text_formatNumber(item_getCount(id), buf, 2, 1);
    ui_drawSmallDigits(r->x + 0x5C, r->y + s_shopBuyInfoRowY[1], 0x1E, buf, 0);
    text_formatNumber(item_countEquipped(id), buf, 2, 1);
    ui_drawSmallDigits(r->x + 0x5C, r->y + s_shopBuyInfoRowY[2], 0x1E, buf, equip ? 0 : 2);
}

void shopBuy_drawList(void) {
    RECT sub;
    MenuView view;
    s32 w;
    s32 i;
    MenuList *list;
    RECT *r = &s_shopBuy.listRect;

    w = ui_measureLabel((u8 *)g_winLabelStrings.unkA4);
    ui_drawLabel(r->x + 0xE, r->y - 3, 0x23, g_winLabelStrings.unk68, 0);
    ui_drawLabel((r->x + 0xA6) - w, r->y - 3, 0x23, g_winLabelStrings.unkA4, 0);
    list = &s_shopBuy.list;
    win_drawListBox(r, 0x23, 1, list);
    menuList_getView(list, &view);
    win_setClip(NULL, 0x1E, 1);
    for (i = 0; i < view.unk0; i++) {
        shopBuy_drawRow(0, i, view.unk1 + i, &view);
    }
    if (list->scroll & 4) {
        menuList_getRowsRect(list, r, &sub);
        win_setClip(&sub, 0x1E, 1);
        shopBuy_drawScrollRows();
    }
    win_setClip(r, 0x1E, 1);
}

void shopBuy_drawParty(void) {
    u8 buf[8];
    Equip5 eq;
    RECT *r;
    MenuList *l;
    Character *c;
    u8 id;
    s32 kind;
    u8 armor;
    u8 ok;
    s32 cur;
    s32 new;
    s32 i;
    s32 x;
    s32 y;

    r = &s_shopBuy.partyRect;
    l = &s_shopBuy.list;
    id = s_shopBuy.items[l->top + l->cursor];
    if (s_shopBuy.hasEquipment == 0) {
        return;
    }
    win_drawBox(r, 0x23, 1);
    kind = item_getEquipSlot(id);
    if (kind < 0) {
        return;
    }
    armor = kind > 0;
    ui_drawLabel(r->x + 6, r->y - 3, 0x1D, ((u8 **)&g_winLabelStrings)[s_shopBuyPartyLabel[armor]], 0);
    for (i = 0; i < s_shop.memberCount; i++) {
        x = 0xF + i * 0x24;
        c = &g_gameState->gameData->characters[(u8)s_shop.members[i]];
        ok = (g_itemTable[id].unk6 & s_shopBuyEquipMask[chr_getDefaultSet(c)]) != 0;
        y = 0x9E;
        if (ok) {
            if (!armor) {
                new = g_weaponTable[id].unk5;
                cur = g_weaponTable[c->equip[0]].unk5;
            } else {
                eq = *(Equip5 *)c->equip;
                eq.v[kind] = id;
                cur = shop_sumArmorStat(kind, c->equip);
                new = shop_sumArmorStat(kind, eq.v);
            }
            if (kind == EQUIP_SLOT_ACCESSORY) {
                *(s32 *)buf = s_shopBuyNoStat;
            } else {
                text_formatNumber(new, buf, 3, 1);
            }
            shop_drawStatIcon(x - 1, y + 0x2E, 0x1E, s_shopBuyStatIcon[kind], 0);
            ui_drawSmallDigits(x + 0xC, y + 0x2F, 0x1E, buf, 0);
            if (cur < new) {
                shop_drawStatIcon(x - 1, y + 0x27, 0x1E, SHOP_ICON_UP, 0);
            } else if (new < cur) {
                shop_drawStatIcon(x - 1, y + 0x28, 0x1E, SHOP_ICON_DOWN, 0);
            }
        }
        ui_drawPortraitId(x, y, 0x1E, chr_getPortrait((u8)s_shop.members[i]), 0x2D, !ok);
    }
}

void shopBuy_drawQuantityWindow(Window *w) {
    u8 buf[16];
    ShopBuy *b;
    MenuList *l;
    Unk80069018 *it;
    u8 id;

    l = &s_shopBuy.list;
    b = &s_shopBuy;
    id = b->items[l->top + l->cursor];
    it = &g_itemTable[id];
    ui_drawItemIcon(w->unk0 + 8, w->unk2 + 8, w->unk18, id, 1);
    text_drawBlock(w->unk0 + 0x18, w->unk2 + 8, w->unk18, g_itemNames + it->unk0, 0);
    ui_drawEtchedLine(w->unk0 + 4, w->unk2 + 0x18, w->unk18, w->unk4 - 9);
    text_drawBlock(w->unk0 + 0xC, w->unk2 + 0x1E, w->unk18, text_getFrom(g_shopText, SHOP_TEXT_QUANTITY), 0);
    text_drawBlock(w->unk0 + 0xC, w->unk2 + 0x2E, w->unk18, text_getFrom(g_shopText, SHOP_TEXT_PRICE), 0);
    text_formatNumber(b->quantity, buf, 2, 1);
    ui_drawSmallDigits(w->unk0 + 0x5C, w->unk2 + 0x22, w->unk18, buf, 0);
    text_formatNumber(b->quantity * it->unk4, buf, 7, 1);
    strcat(buf, s_shopBuyQuantityGil);
    ui_drawSmallDigits(w->unk0 + 0x39, w->unk2 + 0x32, w->unk18, buf, 0);
}

void shopBuy_drawCursor(void) {
    if (!fade_isBusy()) {
        if (s_shopBuy.state == SHOP_ST_LIST || (g_menuFrame & 3)) {
            g_menuHelp.otz = 0x14;
            g_menuHelp.x = s_shopBuy.listRect.x - 8;
            g_menuHelp.y = s_shopBuy.listRect.y + (s16)(s_shopBuy.list.cursor * 0xF + 0xB);
            help_drawCursor();
        }
        if (s_shopBuy.state == SHOP_ST_QUANTITY) {
            g_menuHelp.x = s_shopBuy.quantityWin->unk0 - 0xE;
            g_menuHelp.y = s_shopBuy.quantityWin->unk2 + 0x22;
            g_menuHelp.otz = s_shopBuy.quantityWin->unk18 - 1;
            help_drawCursor();
        }
    }
}

void shopBuy_openQuantity(void) {
    MenuList *l;
    ShopBuy *m;
    ShopBuy *m2;
    Unk80069018 *it;
    s32 have;
    u32 n;
    s32 q;
    u8 id;

    l = &s_shopBuy.list;
    m = &s_shopBuy;
    id = m->items[l->top + l->cursor];
    it = &g_itemTable[id];
    have = item_getCount(id);
    if (have < ITEM_MAX_COUNT && g_gameState->gameData->gil >= it->unk4) {
        m->quantity = 1;
        q = it->unk4 != 0 ? g_gameState->gameData->gil / it->unk4 : ITEM_MAX_COUNT;
        n = q;
        m2 = &s_shopBuy;
        if ((s32)n > ITEM_MAX_COUNT - have) {
            n = ITEM_MAX_COUNT - have;
        }
        m2->maxQuantity = n;
        s_shopBuy.state = SHOP_ST_NONE;
        m2->quantityWin = win_openFree(&m2->quantityRect, 0xA, 2);
        win_setDrawContent(s_shopBuy.quantityWin, shopBuy_drawQuantityWindow);
        win_setOnOpened(s_shopBuy.quantityWin, shopBuy_onQuantityOpened);
        win_setOnClosed(s_shopBuy.quantityWin, shopBuy_onQuantityClosed);
        win_setShadow(s_shopBuy.quantityWin, 1);
        win_setTitle(s_shopBuy.quantityWin, D_800704A4);
        snd_playSe(SE_CURSOR);
    } else {
        snd_playSe(SE_BUZZER);
    }
}

void shopBuy_update(void) {
    if (g_padRepeat & 8) {
        func_8002C1E4();
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
            if (!(g_itemTable[list[i]].unkF & 0xF0)) {
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
    MenuList *m = &s_shopBuy.list;

    if (m->scroll == 0) {
        if (g_padRepeat & BTN_CIRCLE) {
            snd_playSe(SE_CANCEL);
            shop_leaveMode();
            return;
        }
        if (g_padRepeat & BTN_CROSS) {
            shopBuy_openQuantity();
            return;
        }
        menuList_input(m, g_padRepeat);
    }
}

void shopBuy_drawScrollRows(void) {
    MenuView v;
    s32 i;

    menuList_getPageView(&s_shopBuy.list, &v);
    for (i = 0; i < v.unk0; i++) {
        shopBuy_drawRow(0, i, v.unk1 + i, &v);
    }
}

void shopBuy_drawRow(s32 unused, s32 row, s32 idx, MenuView *v) {
    u8 buf[16];
    Unk80069018 *it;
    s32 y;
    s32 x;
    u8 id;
    u8 ok;

    ok = 0;
    id = s_shopBuy.items[idx];
    it = &g_itemTable[id];
    if (item_getCount(id) < ITEM_MAX_COUNT) {
        ok = g_gameState->gameData->gil >= it->unk4;
    }
    x = 0x18;
    y = 0x26;
    y = row * 15 + (v->unk2 + y);
    text_formatNumber(it->unk4, buf, 5, 1);
    ui_drawItemIcon(x, y, 0x1E, id, ok);
    text_drawBlock(x + 0x10, y, 0x1E, g_itemNames + it->unk0, ok ? 0 : 2);
    ui_drawSmallDigits(x + 0x72, y + 4, 0x1E, buf, ok ? 0 : 2);
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
        if (g_itemTable[s_shopBuy.items[i]].unkF & 0xF8) {
            s_shopBuy.hasEquipment = 1;
        }
        s_shopBuy.count++;
    }
}

void shopBuy_confirm(void) {
    MenuList *l;
    ShopBuy *b;
    Unk80069018 *it;
    u8 id;
    s32 n;

    l = &s_shopBuy.list;
    b = &s_shopBuy;
    id = b->items[l->top + l->cursor];
    it = &g_itemTable[id];
    n = item_add(id, b->quantity);
    if (n != 0) {
        g_gameState->gameData->gil -= it->unk4 * n;
        shopBuy_closeQuantity();
    }
}

void shopBuy_updateHelp(void) {
    MenuList *l;
    u8 id;

    g_menuHelp.text = NULL;
    l = &s_shopBuy.list;
    if (s_shopBuy.state == SHOP_ST_LIST) {
        id = s_shopBuy.items[l->top + l->cursor];
        g_menuHelp.text = s_shopItemHelp + g_itemTable[id].unk2;
    } else if (s_shopBuy.state == SHOP_ST_QUANTITY) {
        g_menuHelp.text = text_get(SHOP_BUY_TEXT_QUANTITY_HELP);
    }
}

void shopSynth_init(void) {
    shopSynth_loadRecipes();
    s_shopSynth.state = SHOP_ST_LIST;
    help_setBounds(NULL);
    help_setCallback(shopSynth_updateHelp);
    setRECT(&s_shopSynth.infoRect, 0xBC, 0x26, 0x78, s_shopSynth.hasEquipment ? 0x71 : 0xAD);
    setRECT(&s_shopSynth.listRect, 8, 0x26, 0xB4, s_shopSynth.hasEquipment ? 0x71 : 0xAD);
    setRECT(&s_shopSynth.partyRect, 8, 0x97, 0x12C, 0x45);
    setRECT(&s_shopSynth.quantityRect, 0x62, 0x60, 0x7C, 0x45);
    text_setBank(g_shopSynthText);
    shop_setHelp(SHOP_TEXT_SYNTH_WHAT);
    menuList_init(&s_shopSynth.list, s_shopSynth.count, s_shopSynth.hasEquipment ? 7 : 0xB, 1, s_shopSynth.listRect.h);
    shop_collectMembers();
}

void shopSynth_inputQuantity(void) {
    s32 q;
    s32 t;

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
        q = s_shopSynth.quantity + SHOP_QUANTITY_STEP;
        if (q > s_shopSynth.maxQuantity) {
            q = s_shopSynth.maxQuantity;
        }
        s_shopSynth.quantity = q;
        return;
    }
    if ((g_padRepeat & PAD_DOWN) && s_shopSynth.quantity >= 2) {
        snd_playSe(SE_CURSOR);
        t = s_shopSynth.quantity - SHOP_QUANTITY_STEP;
        q = 1;
        if (t > 0) {
            q = t;
        }
        s_shopSynth.quantity = q;
        return;
    }
    if (g_padRepeat & BTN_CROSS) {
        snd_playSe(SE_SHOP);
        shopSynth_confirm();
    }
}

void shopSynth_drawInfo(void) {
    u8 buf[16];
    RECT *r;
    MenuList *l;
    SynthRecipe *s;
    u8 equip;
    u8 ok;
    s32 i;
    s32 have;

    r = &s_shopSynth.infoRect;
    l = &s_shopSynth.list;
    s = &s_shopSynth.recipes[l->top + l->cursor];
    equip = (g_itemTable[s->product].unkF & 0xF8) != 0;
    ui_drawEtchedLine(r->x + 2, r->y + 0x36, 0x23, r->w - 5);
    win_drawBox(r, 0x23, 1);
    ui_drawLabel(r->x + 6, r->y - 3, 0x1E, D_80070460, 0);
    for (i = 0; i < 4; i++) {
        text_drawBlock(r->x + 8, r->y + s_shopSynthInfoRowY[i] - 4, 0x1E, text_getFrom(g_shopText, s_shopSynthInfoRowText[i]),
                      (s_shopSynthInfoRowAlways[i] | equip) ? 0 : 2);
    }
    text_formatNumber(g_gameState->gameData->gil, buf, 7, 1);
    strcat(buf, s_shopSynthInfoGil);
    ui_drawSmallDigits(r->x + 0x39, r->y + s_shopSynthInfoRowY[0], 0x1E, buf, 0);
    text_formatNumber(item_getCount(s->product), buf, 2, 1);
    ui_drawSmallDigits(r->x + 0x5C, r->y + s_shopSynthInfoRowY[1], 0x1E, buf, 0);
    text_formatNumber(item_countEquipped(s->product), buf, 2, 1);
    ui_drawSmallDigits(r->x + 0x5C, r->y + s_shopSynthInfoRowY[2], 0x1E, buf, equip ? 0 : 2);
    for (i = 0; i < 2; i++) {
        if (s->ingredients[i] != ITEM_NONE) {
            have = item_getCount(s->ingredients[i]);
            if (s->ingredients[0] == s->ingredients[1]) {
                ok = i < have;
            } else {
                ok = have != 0;
            }
            ui_drawItemIcon(r->x + 0xC, r->y + 0x4C + i * 17, 0x1E, s->ingredients[i], ok);
            text_drawBlock(r->x + 0x1C, r->y + 0x4D + i * 17, 0x1E, g_itemNames + g_itemTable[s->ingredients[i]].unk0,
                          ok ? 0 : 2);
        }
    }
}

void shopSynth_drawList(void) {
    RECT sub;
    MenuView view;
    s32 w;
    s32 i;
    MenuList *list;
    RECT *r = &s_shopSynth.listRect;

    w = ui_measureLabel((u8 *)g_winLabelStrings.unkA4);
    ui_drawLabel(r->x + 0xE, r->y - 3, 0x23, g_winLabelStrings.unk68, 0);
    ui_drawLabel((r->x + 0xA6) - w, r->y - 3, 0x23, g_winLabelStrings.unkA4, 0);
    list = &s_shopSynth.list;
    win_drawListBox(r, 0x23, 1, list);
    menuList_getView(list, &view);
    win_setClip(NULL, 0x1E, 1);
    for (i = 0; i < view.unk0; i++) {
        shopSynth_drawRow(0, i, view.unk1 + i, &view);
    }
    if (list->scroll & 4) {
        menuList_getRowsRect(list, r, &sub);
        win_setClip(&sub, 0x1E, 1);
        shopSynth_drawScrollRows();
    }
    win_setClip(r, 0x1E, 1);
}

void shopSynth_drawParty(void) {
    u8 buf[8];
    Equip5 eq;
    RECT *r;
    MenuList *l;
    Character *c;
    SynthRecipe *s;
    s32 kind;
    u8 armor;
    u8 ok;
    s32 cur;
    s32 new;
    s32 i;
    s32 x;
    s32 y;

    r = &s_shopSynth.partyRect;
    l = &s_shopSynth.list;
    s = &s_shopSynth.recipes[l->top + l->cursor];
    if (s_shopSynth.hasEquipment == 0) {
        return;
    }
    win_drawBox(r, 0x23, 1);
    kind = item_getEquipSlot(s->product);
    if (kind < 0) {
        return;
    }
    armor = kind > 0;
    ui_drawLabel(r->x + 6, r->y - 3, 0x1D, ((u8 **)&g_winLabelStrings)[s_shopSynthPartyLabel[armor]], 0);
    for (i = 0; i < s_shop.memberCount; i++) {
        x = 0xF + i * 0x24;
        c = &g_gameState->gameData->characters[(u8)s_shop.members[i]];
        ok = (g_itemTable[s->product].unk6 & s_shopSynthEquipMask[chr_getDefaultSet(c)]) != 0;
        y = 0x9E;
        if (ok) {
            if (!armor) {
                new = g_weaponTable[s->product].unk5;
                cur = g_weaponTable[c->equip[0]].unk5;
            } else {
                eq = *(Equip5 *)c->equip;
                eq.v[kind] = s->product;
                cur = shop_sumArmorStat(kind, c->equip);
                new = shop_sumArmorStat(kind, eq.v);
            }
            if (kind == EQUIP_SLOT_ACCESSORY) {
                *(s32 *)buf = s_shopSynthNoStat;
            } else {
                text_formatNumber(new, buf, 3, 1);
            }
            shop_drawStatIcon(x - 1, y + 0x2E, 0x1E, s_shopSynthStatIcon[kind], 0);
            ui_drawSmallDigits(x + 0xC, y + 0x2F, 0x1E, buf, 0);
            if (cur < new) {
                shop_drawStatIcon(x - 1, y + 0x27, 0x1E, SHOP_ICON_UP, 0);
            } else if (new < cur) {
                shop_drawStatIcon(x - 1, y + 0x28, 0x1E, SHOP_ICON_DOWN, 0);
            }
        }
        ui_drawPortraitId(x, y, 0x1E, chr_getPortrait((u8)s_shop.members[i]), 0x2D, !ok);
    }
}

void shopSynth_drawQuantityWindow(Window *w) {
    u8 buf[16];
    ShopSynth *s;
    MenuList *l;
    SynthRecipe *r;
    Unk80069018 *it;
    u8 id;

    l = &s_shopSynth.list;
    r = &s_shopSynth.recipes[l->top + l->cursor];
    id = r->product;
    it = &g_itemTable[id];
    ui_drawItemIcon(w->unk0 + 8, w->unk2 + 8, w->unk18, id, 1);
    text_drawBlock(w->unk0 + 0x18, w->unk2 + 8, w->unk18, g_itemNames + it->unk0, 0);
    ui_drawEtchedLine(w->unk0 + 4, w->unk2 + 0x18, w->unk18, w->unk4 - 9);
    text_drawBlock(w->unk0 + 0xC, w->unk2 + 0x1E, w->unk18, text_getFrom(g_shopText, SHOP_TEXT_QUANTITY), 0);
    text_drawBlock(w->unk0 + 0xC, w->unk2 + 0x2E, w->unk18, text_getFrom(g_shopText, SHOP_TEXT_PRICE), 0);
    s = &s_shopSynth;
    text_formatNumber(s->quantity, buf, 2, 1);
    ui_drawSmallDigits(w->unk0 + 0x5C, w->unk2 + 0x22, w->unk18, buf, 0);
    text_formatNumber(s->quantity * r->price, buf, 7, 1);
    strcat(buf, s_shopSynthQuantityGil);
    ui_drawSmallDigits(w->unk0 + 0x39, w->unk2 + 0x32, w->unk18, buf, 0);
}

void shopSynth_drawCursor(void) {
    if (!fade_isBusy()) {
        if (s_shopSynth.state == SHOP_ST_LIST || (g_menuFrame & 3)) {
            g_menuHelp.otz = 0x14;
            g_menuHelp.x = s_shopSynth.listRect.x - 8;
            g_menuHelp.y = s_shopSynth.listRect.y + (s16)(s_shopSynth.list.cursor * 0xF + 0xB);
            help_drawCursor();
        }
        if (s_shopSynth.state == SHOP_ST_QUANTITY) {
            g_menuHelp.x = s_shopSynth.quantityWin->unk0 - 0xE;
            g_menuHelp.y = s_shopSynth.quantityWin->unk2 + 0x22;
            g_menuHelp.otz = s_shopSynth.quantityWin->unk18 - 1;
            help_drawCursor();
        }
    }
}

void shopSynth_drawRow(s32 unused, s32 row, s32 idx, MenuView *v) {
    u8 buf[16];
    SynthRecipe *r;
    Unk80069018 *it;
    s32 i;
    s32 t;
    s32 x;
    s32 y;
    u8 ok;
    u8 id;
    s32 k;

    r = &s_shopSynth.recipes[idx];
    ok = 0;
    id = r->product;
    it = &g_itemTable[id];
    k = 1;
    if (item_getCount(id) < ITEM_MAX_COUNT) {
        ok = g_gameState->gameData->gil >= r->price;
    }
    x = 0x18;
    y = 0x26;
    y = row * 15 + (v->unk2 + y);
    i = 0;
loop:
    if (r->ingredients[i] != ITEM_NONE) {
        if (r->ingredients[0] == r->ingredients[1]) {
            t = i < item_getCount(r->ingredients[i]);
        } else {
            t = item_getCount(r->ingredients[i]);
        }
        if (!t) {
            do {
                ok = 0;
            } while (0);
        }
    }
    i++;
    if (i < 2) {
        goto loop;
    }
    text_formatNumber(r->price, buf, 5, k);
    id = ok;
    ui_drawItemIcon(x, y, 0x1E, r->product, id);
    text_drawBlock(x + 0x10, y, 0x1E, g_itemNames + it->unk0, id ? 0 : 2);
    ui_drawSmallDigits(x + 0x72, y + 4, 0x1E, buf, id ? 0 : 2);
}

void shopSynth_openQuantity(void) {
    MenuList *l;
    SynthRecipe *r;
    ShopSynth *m;
    ShopSynth *m2;
    ShopSynth *m3;
    s32 need;
    s32 have;
    u32 n;

    l = &s_shopSynth.list;
    r = &s_shopSynth.recipes[l->top + l->cursor];
    need = 1;
    if (r->ingredients[0] == r->ingredients[1]) {
        need = 2;
    }
    have = item_getCount(r->product);
    if (have < ITEM_MAX_COUNT && g_gameState->gameData->gil >= r->price &&
        (r->ingredients[0] == ITEM_NONE || item_getCount(r->ingredients[0]) >= need) &&
        (r->ingredients[1] == ITEM_NONE || item_getCount(r->ingredients[1]) >= need)) {
        s_shopSynth.quantity = 1;
        n = r->price != 0 ? g_gameState->gameData->gil / r->price : ITEM_MAX_COUNT;
        m2 = &s_shopSynth;
        if ((s32)n > ITEM_MAX_COUNT - have) {
            n = ITEM_MAX_COUNT - have;
        }
        m2->maxQuantity = n;
        m = m2;
        if (r->ingredients[0] != ITEM_NONE) {
            m->maxQuantity = MIN(m->maxQuantity, item_getCount(r->ingredients[0]) / need);
        }
        if (r->ingredients[1] != ITEM_NONE) {
            s_shopSynth.maxQuantity = MIN(s_shopSynth.maxQuantity, item_getCount(r->ingredients[1]) / need);
        }
        m3 = &s_shopSynth;
        s_shopSynth.state = SHOP_ST_NONE;
        m3->quantityWin = win_openFree(&m3->quantityRect, 0xA, 2);
        win_setDrawContent(m3->quantityWin, shopSynth_drawQuantityWindow);
        win_setOnOpened(m3->quantityWin, shopSynth_onQuantityOpened);
        win_setOnClosed(m3->quantityWin, shopSynth_onQuantityClosed);
        win_setShadow(m3->quantityWin, 1);
        win_setTitle(m3->quantityWin, D_800704A4);
        snd_playSe(SE_CURSOR);
    } else {
        snd_playSe(SE_BUZZER);
    }
}

void shopSynth_loadRecipes(void) {
    s32 mask;
    s32 i;
    s32 n;

    n = SHOP_RECIPE_COUNT;
    mask = 1 << (shop_getId() - SHOP_ID_SYNTH);
    s_shopSynth.count = 0;
    for (i = 0; i < n; i++) {
        if ((s_shopRecipes[i].shopMask & mask) && s_shopRecipes[i].product != ITEM_NONE) {
            s_shopSynth.recipes[s_shopSynth.count++] = s_shopRecipes[i];
            if (s_shopSynth.count >= SHOP_LIST_SIZE) {
                break;
            }
        }
    }
    for (i = 0; i < s_shopSynth.count; i++) {
        if (g_itemTable[s_shopSynth.recipes[i].product].unkF & 0xF8) {
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
    MenuList *m = &s_shopSynth.list;

    if (m->scroll == 0) {
        if (g_padRepeat & BTN_CIRCLE) {
            snd_playSe(SE_CANCEL);
            shop_leaveMode();
            return;
        }
        if (g_padRepeat & BTN_CROSS) {
            shopSynth_openQuantity();
            return;
        }
        menuList_input(m, g_padRepeat);
    }
}

void shopSynth_drawScrollRows(void) {
    MenuView v;
    s32 i;

    menuList_getPageView(&s_shopSynth.list, &v);
    for (i = 0; i < v.unk0; i++) {
        shopSynth_drawRow(0, i, v.unk1 + i, &v);
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
    MenuList *m = &s_shopSynth.list;
    SynthRecipe *r;
    s32 n;

    r = &s_shopSynth.recipes[m->top + m->cursor];
    n = item_add(r->product, s_shopSynth.quantity);
    if (n != 0) {
        item_remove(r->ingredients[0], n);
        item_remove(r->ingredients[1], n);
        g_gameState->gameData->gil -= r->price * n;
        shopSynth_closeQuantity();
    }
}

void shopSynth_updateHelp(void) {
    MenuList *l;
    u8 id;

    g_menuHelp.text = NULL;
    l = &s_shopSynth.list;
    if (s_shopSynth.state == SHOP_ST_LIST) {
        id = s_shopSynth.recipes[l->top + l->cursor].product;
        g_menuHelp.text = s_shopItemHelp + g_itemTable[id].unk2;
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
    menuList_init(&s_shopSell.list, 0xFF, 0xB, 2, 0xAD);
    setRECT(&s_shopSell.listRect, 8, 0x2A, 0x12C, 0xAD);
    setRECT(&s_shopSell.winRects[0], 0x62, 0x40, 0x7C, 0x45);
    setRECT(&s_shopSell.winRects[1], 0x62, 0x87, 0x7C, 0x3E);
}

void shopSell_inputQuantity(void) {
    s32 q;
    s32 t;

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
        q = s_shopSell.quantity + SHOP_QUANTITY_STEP;
        if (q > s_shopSell.maxQuantity) {
            q = s_shopSell.maxQuantity;
        }
        s_shopSell.quantity = q;
        return;
    }
    if ((g_padRepeat & PAD_DOWN) && s_shopSell.quantity >= 2) {
        snd_playSe(SE_CURSOR);
        t = s_shopSell.quantity - SHOP_QUANTITY_STEP;
        q = 1;
        if (t > 0) {
            q = t;
        }
        s_shopSell.quantity = q;
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

    ui_drawLabel(s_shopSellListRectPtr->x + 0xE, s_shopSellListRectPtr->y - 3, 0x23, g_winLabelStrings.unk68, 0);
    ui_drawLabel(s_shopSellListRectPtr->x + 0xA4, s_shopSellListRectPtr->y - 3, 0x23, g_winLabelStrings.unk68, 0);
    w = ui_measureLabel((u8 *)g_winLabelStrings.unk94);
    ui_drawLabel((s_shopSellListRectPtr->x + 0x8A) - w, s_shopSellListRectPtr->y - 3, 0x23, g_winLabelStrings.unk94, 0);
    ui_drawLabel((s_shopSellListRectPtr->x + 0x120) - w, s_shopSellListRectPtr->y - 3, 0x23, g_winLabelStrings.unk94, 0);
    menuList_getView(list, &view);
    win_drawListBox(s_shopSellListRectPtr, 0x23, 1, list);
    idx = view.unk1 * 2;
    win_setClip(NULL, 0x1E, 1);
    for (row = 0; row < view.unk0; row++) {
        for (col = 0; col < 2; col++) {
            shopSell_drawRow(col, row, idx++, &view);
        }
    }
    if (list->scroll & 4) {
        menuList_getRowsRect(list, s_shopSellListRectPtr, &sub);
        win_setClip(&sub, 0x1E, 1);
        shopSell_drawScrollRows();
    }
    win_setClip(s_shopSellListRectPtr, 0x1E, 1);
}

void shopSell_drawQuantityWindow(Window *w) {
    u8 buf[16];
    ShopSell *m;
    MenuList *l;
    Unk80069018 *it;
    u8 id;
    u32 price;

    l = &s_shopSell.list;
    id = (g_gameState->gameData->items.slots + ((l->top + l->cursor) * 2 + l->col))->id;
    it = &g_itemTable[id];
    price = it->unk4 >> 1;
    ui_drawItemIcon(w->unk0 + 8, w->unk2 + 8, w->unk18, id, 1);
    text_drawBlock(w->unk0 + 0x18, w->unk2 + 8, w->unk18, g_itemNames + it->unk0, 0);
    ui_drawEtchedLine(w->unk0 + 4, w->unk2 + 0x18, w->unk18, w->unk4 - 9);
    text_drawBlock(w->unk0 + 0xC, w->unk2 + 0x1E, w->unk18, text_getFrom(g_shopText, SHOP_TEXT_QUANTITY), 0);
    text_drawBlock(w->unk0 + 0xC, w->unk2 + 0x2E, w->unk18, text_getFrom(g_shopText, SHOP_TEXT_PRICE), 0);
    m = &s_shopSell;
    text_formatNumber(m->quantity, buf, 2, 1);
    ui_drawSmallDigits(w->unk0 + 0x5C, w->unk2 + 0x22, w->unk18, buf, 0);
    text_formatNumber(m->quantity * price, buf, 7, 1);
    strcat(buf, s_shopSellQuantityGil);
    ui_drawSmallDigits(w->unk0 + 0x39, w->unk2 + 0x32, w->unk18, buf, 0);
}

void shopSell_drawInfoWindow(Window *w) {
    u8 buf[16];
    RECT *r;
    MenuList *l;
    UnkPair *e;
    u8 equip;
    s32 i;

    r = (RECT *)w;
    l = &s_shopSell.list;
    e = &g_gameState->gameData->items.slots[(l->top + l->cursor) * 2 + l->col];
    equip = (g_itemTable[e->unk0].unkF & 0xF8) != 0;
    for (i = 0; i < 3; i++) {
        text_drawBlock(r->x + 0xC, r->y + s_shopSellInfoRowY[i] - 4, w->unk18, text_getFrom(g_shopText, s_shopSellInfoRowText[i]),
                      (s_shopSellInfoRowAlways[i] | equip) ? 0 : 2);
    }
    text_formatNumber(g_gameState->gameData->gil, buf, 7, 1);
    strcat(buf, s_shopSellInfoGil);
    ui_drawSmallDigits(r->x + 0x39, r->y + s_shopSellInfoRowY[0], w->unk18, buf, 0);
    text_formatNumber(item_getCount(e->unk0), buf, 2, 1);
    ui_drawSmallDigits(r->x + 0x5C, r->y + s_shopSellInfoRowY[1], w->unk18, buf, 0);
    if (equip) {
        text_formatNumber(item_countEquipped(e->unk0), buf, 2, 1);
        ui_drawSmallDigits(r->x + 0x5C, r->y + s_shopSellInfoRowY[2], w->unk18, buf, 0);
    }
}

void shopSell_drawCursor(void) {
    MenuList *l;

    l = &s_shopSell.list;
    if (!fade_isBusy()) {
        if (s_shopSell.state == SHOP_ST_LIST || (g_menuFrame & 3)) {
            g_menuHelp.otz = 0x14;
            g_menuHelp.x = l->col * 0x96;
            g_menuHelp.y = l->cursor * 0xF + 0x34;
            help_drawCursor();
        }
        if (s_shopSell.state == SHOP_ST_QUANTITY) {
            g_menuHelp.x = s_shopSell.wins[0]->unk0 - 0xE;
            g_menuHelp.y = s_shopSell.wins[0]->unk2 + 0x22;
            g_menuHelp.otz = s_shopSell.wins[0]->unk18 - 1;
            help_drawCursor();
        }
    }
}

void shopSell_openQuantity(void) {
    MenuList *l = &s_shopSell.list;
    UnkPair *p;
    s32 i;

    p = &g_gameState->gameData->items.slots[(l->top + l->cursor) * 2 + l->col];
    if (p->unk1 != 0) {
        s_shopSell.quantity = 1;
        s_shopSell.maxQuantity = p->unk1;
        s_shopSell.state = SHOP_ST_NONE;
        for (i = 0; i < 2; i++) {
            s_shopSell.wins[i] = win_openFree(&s_shopSell.winRects[i], s_shopSellWindows[i].otz, s_shopSellWindows[i].style);
            win_setDrawContent(s_shopSell.wins[i], s_shopSellWindows[i].draw);
            win_setOnOpened(s_shopSell.wins[i], shopSell_onQuantityOpened);
            win_setOnClosed(s_shopSell.wins[i], shopSell_onQuantityClosed);
            win_setOpenAnim(s_shopSell.wins[i], 0);
            win_setShadow(s_shopSell.wins[i], 1);
            win_setTitle(s_shopSell.wins[i], g_winLabelStrings.unk0[s_shopSellWindows[i].label]);
        }
        snd_playSe(SE_CURSOR);
    } else {
        snd_playSe(SE_BUZZER);
    }
}

void shopSell_confirm(void) {
    MenuList *l;
    ShopSell *m;
    Unk80069018 *it;
    u8 id;
    u32 price;
    s32 n;
    s32 i;

    l = &s_shopSell.list;
    m = &s_shopSell;
    id = (g_gameState->gameData->items.slots + ((l->top + l->cursor) * 2 + l->col))->id;
    it = &g_itemTable[id];
    price = it->unk4 >> 1;
    n = item_remove(id, s_shopSell.quantity);
    if (n != 0) {
        g_gameState->gameData->gil += price * n;
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
    MenuList *m = &s_shopSell.list;

    if (m->scroll == 0) {
        if (g_padRepeat & BTN_CIRCLE) {
            snd_playSe(SE_CANCEL);
            shop_leaveMode();
            return;
        }
        if (g_padRepeat & BTN_CROSS) {
            shopSell_openQuantity();
            return;
        }
        menuList_input(m, g_padRepeat);
        menuList_left(m, g_padRepeat);
        menuList_right(m, g_padRepeat);
    }
}

void shopSell_drawScrollRows(void) {
    MenuView v;
    s32 row;
    s32 col;
    s32 idx;

    menuList_getPageView(&s_shopSell.list, &v);
    idx = v.unk1 * 2;
    for (row = 0; row < v.unk0; row++) {
        for (col = 0; col < 2; col++) {
            shopSell_drawRow(col, row, idx++, &v);
        }
    }
}

void shopSell_drawRow(s32 col, s32 row, s32 i, MenuView *v) {
    UnkPair *p = &g_gameState->gameData->items.slots[i];
    Unk80069018 *it;
    s32 x, y;
    s32 k = 0x2A;
    u8 buf[8];

    if (p->unk1 != 0) {
        it = &g_itemTable[p->unk0];
        x = col * 0x96;
        y = v->unk2 + (row * 0xF + k);
        text_formatNumber(p->unk1, buf, 2, 1);
        text_drawBlock(x + 0x28, y, 0x1E, g_itemNames + it->unk0, 0);
        ui_drawItemIcon(x + 0x18, y, 0x1E, p->unk0, 1);
        ui_drawSmallDigits(x + 0x84, y + 4, 0x1E, buf, 0);
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
    MenuList *l;
    s32 idx;
    UnkPair *p;
    u8 *text;

    h = &g_menuHelp;
    h->text = NULL;
    if (s_shopSell.state == SHOP_ST_LIST) {
        l = &s_shopSell.list;
        idx = (l->top + l->cursor) * 2 + l->col;
        p = &g_gameState->gameData->items.slots[idx];
        if (p->unk1 == 0) {
            return;
        }
        text = s_shopItemHelp + g_itemTable[p->unk0].unk2;
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
