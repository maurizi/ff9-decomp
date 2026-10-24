#include "ovl_3c.h"

void shop_drawMessage(void) {
    ui_drawLabel(s_shopMessageRect.x + 6, s_shopMessageRect.y - 3, s_shop.unk20, D_80070450, 0);
    if (s_shop.unk10 != NULL) {
        text_drawBlock(s_shopMessageRect.x + 0xA, s_shopMessageRect.y + 6, s_shop.unk20, s_shop.unk10, 0);
    }
    func_8002DEE4(&s_shopMessageRect, s_shop.unk20, 1);
    func_8002705C(&s_shopMessageRect, s_shop.unk20, 1);
}

void shop_inputMenu(void) {
    if (!func_8002F810()) {
        if (g_padRepeat & BTN_CIRCLE) {
            snd_playSe(0x65);
            shop_close();
            return;
        }
        if (g_padRepeat & PAD_UP) {
            snd_playSe(0x67);
            s_shop.unk1 = s_shop.unk1 == 0 ? s_shop.unk2 - 1 : s_shop.unk1 - 1;
            return;
        }
        if (g_padRepeat & PAD_DOWN) {
            snd_playSe(0x67);
            s_shop.unk1 = s_shop.unk1 >= s_shop.unk2 - 1 ? 0 : s_shop.unk1 + 1;
            return;
        }
        if (g_padRepeat & BTN_CROSS) {
            snd_playSe(0x67);
            shop_selectMenu();
        }
    }
}

void shop_drawMenu(void) {
    s32 i;
    s32 y;

    s_shopMenuRect.h = s_shop.unk2 * 0xF + 0xB;
    func_8002DEE4(&s_shopMenuRect, 0x23, 2);
    func_8002705C(&s_shopMenuRect, 0x23, 2);
    for (i = 0; i < s_shop.unk2; i++) {
        text_drawBlock(s_shopMenuRect.x + 0xA, s_shopMenuRect.y + 6 + i * 0xF, 0x1E, text_get(s_shopMenuText[s_shop.unk0] + i), 0);
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
        if (equip[i] != 0xFF) {
            if (equip[i] < 0xE0) {
                j = equip[i] - 0x58;
                b = &D_8006D328[j];
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
    s_shop.unk20 = 5;
    func_8002F9F0();
    func_8002F8A8(shop_onModeLeft);
}

void shop_setHelp(s32 i) {
    s_shop.unk10 = (u8 *)text_getFrom(g_shopText, i);
}

s32 shop_getId(void) {
    return s_shop.unk8;
}

void shop_collectMembers(void) {
    s32 i;

    s_shop.unk14 = 0;
    for (i = 0; i < 9; i++) {
        if (g_gameState->unk1C->characters[i].unk34.b.unkB) {
            s_shop.unk18[s_shop.unk14] = i;
            if (++s_shop.unk14 >= 8) {
                break;
            }
        }
    }
}

void shop_drawStatIcon(s32 x, s32 y, s32 z, s32 icon, u8 arg4) {
    RectTbl7 rects = s_shopIcons;

    func_80027710(x, y, z, &rects.r[icon], arg4);
}

void shop_init(void) {
    s_shop.unk8 = D_80077DBC;
    s_shop.unk0 = shop_getKind(D_80077DBC);
    s_shop.unk1 = 0;
    s_shop.unk3 = 0;
    s_shop.unk2 = shop_countEntries(s_shop.unk0);
    s_shop.unkC = func_8002F8E4(7);
    s_shop.unk20 = 5;
    s_shop.unk4 = 0;
    shop_setHelp(0xD);
    text_setBank(g_shopText);
    func_8003556C(shop_updateHelp);
    func_80028720(s_shopImage, 0xD);
    shop_loadImages();
}

void shop_update(void) {
    shop_drawTitle();
    shop_drawMessage();
    if (s_shop.unk3 != 0) {
        s_shopModes[s_shop.unk0][s_shop.unk1][1]();
        return;
    }
    shop_inputMenu();
    shop_drawMenuAll();
    func_801F47F0();
}

void shop_exit(void) {
    func_8003556C(NULL);
    func_8002886C();
}

void shop_drawTitle(void) {
    func_8002DEE4(&s_shopTitleRect, 5, 2);
    func_8002705C(&s_shopTitleRect, 0x28, 2);
    text_drawBlock(s_shopTitleRect.x + 0xA, s_shopTitleRect.y + 6, 4, (u8 *)text_getFrom(g_shopText, s_shop.unk0), 0);
}

void shop_drawMenuAll(void) {
    shop_drawMenu();
    shop_drawMenuCursor();
}

void func_801F47F0(void) {
}

void shop_drawMenuCursor(void) {
    if (!func_8002F810()) {
        D_80077DE8.unk0 = 0x6E;
        D_80077DE8.unk4 = 0x14;
        D_80077DE8.unk2 = s_shop.unk1 * 0xF + 0x6A;
        func_800355E4();
    }
}

void shop_onModeLeft(void) {
    s_shop.unk3 = 0;
    s_shopModes[s_shop.unk0][s_shop.unk1][2]();
    func_8002F9D0();
    func_8003556C(shop_updateHelp);
    text_setBank(g_shopText);
    shop_setHelp(s_shop.unk4 != 0 ? 0xE : 0xD);
}

s32 shop_countEntries(s32 kind) {
    s32 n;
    s32 i;

    n = 1;
    for (i = 0; i < 3; i++) {
        if (s_shopModes[kind][i][0] != NULL || s_shopModes[kind][i][1] != NULL || s_shopModes[kind][i][2] != NULL) {
            n++;
        }
    }
    return n;
}

void shop_selectMenu(void) {
    if (s_shop.unk2 - 1 == s_shop.unk1) {
        shop_close();
        return;
    }
    s_shop.unk4++;
    func_8002F9F0();
    func_8002F8A8(shop_enterMode);
}

void shop_updateHelp(void) {
    func_80035580(&s_shopHelpRect);
    D_80077DE8.unk8 = text_get(s_shopMenuHelpText[s_shop.unk0] + s_shop.unk1);
}

void shop_close(void) {
    func_8002F8E4(s_shop.unkC);
    func_800325E0();
}

void shop_loadImages(void) {
    u8 *p = s_shopIconImage;

    LoadImage(&s_shopIconClutRect, (u32 *)p);
    LoadImage(&s_shopIconRect, (u32 *)(p + 0x60));
    DrawSync(0);
}

s32 shop_getKind(s32 id) {
    if (id < 0x20) {
        return shop_isEquipmentShop(id);
    }
    return 2;
}

void shop_enterMode(void) {
    s_shop.unk3 = 1;
    s_shopModes[s_shop.unk0][s_shop.unk1][0]();
    func_8002F9D0();
    func_8002F8A8(shop_onModeEntered);
}

void shop_onModeEntered(void) {
    s_shop.unk20 = 0x24;
}

void shopBuy_init(void) {
    s_shopBuy.unk0 = 1;
    shopBuy_loadList();
    func_80035580(NULL);
    func_8003556C(shopBuy_updateHelp);
    setRECT(&s_shopBuy.unk8, 0xBC, 0x26, 0x78, s_shopBuy.unk4 ? 0x71 : 0xAD);
    setRECT(&s_shopBuy.unk10, 8, 0x26, 0xB4, s_shopBuy.unk4 ? 0x71 : 0xAD);
    setRECT(&s_shopBuy.unk18, 8, 0x97, 0x12C, 0x45);
    setRECT(&s_shopBuy.unk20, 0x62, 0x60, 0x7C, 0x45);
    text_setBank(g_shopBuyText);
    shop_setHelp(0xF);
    func_8002EDFC(&s_shopBuy.unk28, s_shopBuy.unk1, s_shopBuy.unk4 ? 7 : 0xB, 1, s_shopBuy.unk10.h);
    shop_collectMembers();
}

void shopBuy_inputQuantity(void) {
    s32 q;
    s32 t;

    if (g_padRepeat & BTN_CIRCLE) {
        snd_playSe(0x65);
        shopBuy_closeQuantity();
        return;
    }
    if ((g_padRepeat & PAD_RIGHT) && s_shopBuy.unk2 < s_shopBuy.unk3) {
        snd_playSe(0x67);
        s_shopBuy.unk2++;
        return;
    }
    if ((g_padRepeat & PAD_LEFT) && s_shopBuy.unk2 >= 2) {
        snd_playSe(0x67);
        s_shopBuy.unk2--;
        return;
    }
    if ((g_padRepeat & PAD_UP) && s_shopBuy.unk2 < s_shopBuy.unk3) {
        snd_playSe(0x67);
        q = s_shopBuy.unk2 + 10;
        if (q > s_shopBuy.unk3) {
            q = s_shopBuy.unk3;
        }
        s_shopBuy.unk2 = q;
        return;
    }
    if ((g_padRepeat & PAD_DOWN) && s_shopBuy.unk2 >= 2) {
        snd_playSe(0x67);
        t = s_shopBuy.unk2 - 10;
        q = 1;
        if (t > 0) {
            q = t;
        }
        s_shopBuy.unk2 = q;
        return;
    }
    if (g_padRepeat & BTN_CROSS) {
        snd_playSe(0x415);
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

    r = &s_shopBuy.unk8;
    l = &s_shopBuy.unk28;
    id = s_shopBuy.unk3C[l->unk5 + l->cursor];
    equip = (D_80069018[id].unkF & 0xF8) != 0;
    ui_drawLabel(r->x + 6, r->y - 3, 0x1E, D_80070460, 0);
    func_8002DEE4(r, 0x23, 1);
    for (i = 0; i < 3; i++) {
        text_drawBlock(r->x + 8, r->y + s_shopBuyInfoRowY[i] - 4, 0x1E, text_getFrom(g_shopText, s_shopBuyInfoRowText[i]),
                      (s_shopBuyInfoRowAlways[i] | equip) ? 0 : 2);
    }
    text_formatNumber(g_gameState->unk1C->gil, buf, 7, 1);
    strcat(buf, s_shopBuyInfoGil);
    ui_drawSmallDigits(r->x + 0x39, r->y + s_shopBuyInfoRowY[0], 0x1E, buf, 0);
    text_formatNumber(item_getCount(id), buf, 2, 1);
    ui_drawSmallDigits(r->x + 0x5C, r->y + s_shopBuyInfoRowY[1], 0x1E, buf, 0);
    text_formatNumber(func_80027F94(id), buf, 2, 1);
    ui_drawSmallDigits(r->x + 0x5C, r->y + s_shopBuyInfoRowY[2], 0x1E, buf, equip ? 0 : 2);
}

void shopBuy_drawList(void) {
    RECT sub;
    MenuView view;
    s32 w;
    s32 i;
    MenuList *list;
    RECT *r = &s_shopBuy.unk10;

    w = func_8002AE7C((u8 *)D_800703FC.unkA4);
    ui_drawLabel(r->x + 0xE, r->y - 3, 0x23, D_800703FC.unk68, 0);
    ui_drawLabel((r->x + 0xA6) - w, r->y - 3, 0x23, D_800703FC.unkA4, 0);
    list = &s_shopBuy.unk28;
    func_8002E040(r, 0x23, 1, list);
    menuList_getView(list, &view);
    win_setClip(NULL, 0x1E, 1);
    for (i = 0; i < view.unk0; i++) {
        shopBuy_drawRow(0, i, view.unk1 + i, &view);
    }
    if (list->unk8 & 4) {
        func_8002F40C(list, r, &sub);
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

    r = &s_shopBuy.unk18;
    l = &s_shopBuy.unk28;
    id = s_shopBuy.unk3C[l->unk5 + l->cursor];
    if (s_shopBuy.unk4 == 0) {
        return;
    }
    func_8002DEE4(r, 0x23, 1);
    kind = func_80028014(id);
    if (kind < 0) {
        return;
    }
    armor = kind > 0;
    ui_drawLabel(r->x + 6, r->y - 3, 0x1D, ((u8 **)&D_800703FC)[s_shopBuyPartyLabel[armor]], 0);
    for (i = 0; i < s_shop.unk14; i++) {
        x = 0xF + i * 0x24;
        c = &g_gameState->unk1C->characters[(u8)s_shop.unk18[i]];
        ok = (D_80069018[id].unk6 & s_shopBuyEquipMask[chr_getDefaultSet(c)]) != 0;
        y = 0x9E;
        if (ok) {
            if (!armor) {
                new = D_8006E0E8[id].unk5;
                cur = D_8006E0E8[c->equip[0]].unk5;
            } else {
                eq = *(Equip5 *)c->equip;
                eq.v[kind] = id;
                cur = shop_sumArmorStat(kind, c->equip);
                new = shop_sumArmorStat(kind, eq.v);
            }
            if (kind == 4) {
                *(s32 *)buf = s_shopBuyNoStat;
            } else {
                text_formatNumber(new, buf, 3, 1);
            }
            shop_drawStatIcon(x - 1, y + 0x2E, 0x1E, s_shopBuyStatIcon[kind], 0);
            ui_drawSmallDigits(x + 0xC, y + 0x2F, 0x1E, buf, 0);
            if (cur < new) {
                shop_drawStatIcon(x - 1, y + 0x27, 0x1E, 5, 0);
            } else if (new < cur) {
                shop_drawStatIcon(x - 1, y + 0x28, 0x1E, 6, 0);
            }
        }
        func_80028204(x, y, 0x1E, func_80028954((u8)s_shop.unk18[i]), 0x2D, !ok);
    }
}

void shopBuy_drawQuantityWindow(Window *w) {
    u8 buf[16];
    ShopBuy *b;
    MenuList *l;
    Unk80069018 *it;
    u8 id;

    l = &s_shopBuy.unk28;
    b = &s_shopBuy;
    id = b->unk3C[l->unk5 + l->cursor];
    it = &D_80069018[id];
    ui_drawItemIcon(w->unk0 + 8, w->unk2 + 8, w->unk18, id, 1);
    text_drawBlock(w->unk0 + 0x18, w->unk2 + 8, w->unk18, D_8006A218 + it->unk0, 0);
    func_800269F4(w->unk0 + 4, w->unk2 + 0x18, w->unk18, w->unk4 - 9);
    text_drawBlock(w->unk0 + 0xC, w->unk2 + 0x1E, w->unk18, text_getFrom(g_shopText, 0x19), 0);
    text_drawBlock(w->unk0 + 0xC, w->unk2 + 0x2E, w->unk18, text_getFrom(g_shopText, 0x1A), 0);
    text_formatNumber(b->unk2, buf, 2, 1);
    ui_drawSmallDigits(w->unk0 + 0x5C, w->unk2 + 0x22, w->unk18, buf, 0);
    text_formatNumber(b->unk2 * it->unk4, buf, 7, 1);
    strcat(buf, s_shopBuyQuantityGil);
    ui_drawSmallDigits(w->unk0 + 0x39, w->unk2 + 0x32, w->unk18, buf, 0);
}

void shopBuy_drawCursor(void) {
    if (!func_8002F810()) {
        if (s_shopBuy.unk0 == 1 || (D_80077DC0 & 3)) {
            D_80077DE8.unk4 = 0x14;
            D_80077DE8.unk0 = s_shopBuy.unk10.x - 8;
            D_80077DE8.unk2 = s_shopBuy.unk10.y + (s16)(s_shopBuy.unk28.cursor * 0xF + 0xB);
            func_800355E4();
        }
        if (s_shopBuy.unk0 == 2) {
            D_80077DE8.unk0 = s_shopBuy.unk38->unk0 - 0xE;
            D_80077DE8.unk2 = s_shopBuy.unk38->unk2 + 0x22;
            D_80077DE8.unk4 = s_shopBuy.unk38->unk18 - 1;
            func_800355E4();
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

    l = &s_shopBuy.unk28;
    m = &s_shopBuy;
    id = m->unk3C[l->unk5 + l->cursor];
    it = &D_80069018[id];
    have = item_getCount(id);
    if (have < 99 && g_gameState->unk1C->gil >= it->unk4) {
        m->unk2 = 1;
        q = it->unk4 != 0 ? g_gameState->unk1C->gil / it->unk4 : 99;
        n = q;
        m2 = &s_shopBuy;
        if ((s32)n > 99 - have) {
            n = 99 - have;
        }
        m2->unk3 = n;
        s_shopBuy.unk0 = 0;
        m2->unk38 = func_8002DC54(&m2->unk20, 0xA, 2);
        win_setDrawContent(s_shopBuy.unk38, shopBuy_drawQuantityWindow);
        win_setOnOpened(s_shopBuy.unk38, shopBuy_onQuantityOpened);
        win_setOnClosed(s_shopBuy.unk38, shopBuy_onQuantityClosed);
        func_8002E31C(s_shopBuy.unk38, 1);
        func_8002E304(s_shopBuy.unk38, D_800704A4);
        snd_playSe(0x67);
    } else {
        snd_playSe(0x66);
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
    func_8003556C(NULL);
}

s32 shop_isEquipmentShop(s32 shop) {
    u8 *list;
    s32 i;
    u8 id;

    list = s_shopItems[shop];
    for (i = 0; i < 0x20; i++) {
        if (list[i] != 0xFF) {
            if (!(D_80069018[list[i]].unkF & 0xF0)) {
                return 0;
            }
        }
    }
    return 1;
}

void shopBuy_input(void) {
    void (*fn)(void);

    if (!func_8002F810()) {
        fn = s_shopBuyInputFuncs[s_shopBuy.unk0];
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
    if (s_shopBuy.unk28.unk8 != 0) {
        func_8002EB74(&s_shopBuy.unk28);
    }
}

void shopBuy_inputList(void) {
    MenuList *m = &s_shopBuy.unk28;

    if (m->unk8 == 0) {
        if (g_padRepeat & BTN_CIRCLE) {
            snd_playSe(0x65);
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

    func_8002F344(&s_shopBuy.unk28, &v);
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
    id = s_shopBuy.unk3C[idx];
    it = &D_80069018[id];
    if (item_getCount(id) < 99) {
        ok = g_gameState->unk1C->gil >= it->unk4;
    }
    x = 0x18;
    y = 0x26;
    y = row * 15 + (v->unk2 + y);
    text_formatNumber(it->unk4, buf, 5, 1);
    ui_drawItemIcon(x, y, 0x1E, id, ok);
    text_drawBlock(x + 0x10, y, 0x1E, D_8006A218 + it->unk0, ok ? 0 : 2);
    ui_drawSmallDigits(x + 0x72, y + 4, 0x1E, buf, ok ? 0 : 2);
}

void shopBuy_closeQuantity(void) {
    s_shopBuy.unk0 = 0;
    win_close(s_shopBuy.unk38);
}

void shopBuy_onQuantityOpened(Window *w) {
    s_shopBuy.unk0 = 2;
    shop_setHelp(0x12);
}

void shopBuy_onQuantityClosed(Window *w) {
    s_shopBuy.unk0 = 1;
    shop_setHelp(0xF);
}

void shopBuy_loadList(void) {
    s32 i;

    s_shopBuy.unk3C = s_shopItems[shop_getId()];
    s_shopBuy.unk4 = 0;
    s_shopBuy.unk1 = 0;
    for (i = 0; i < 0x20; i++) {
        if (s_shopBuy.unk3C[i] == 0xFF) {
            break;
        }
        if (D_80069018[s_shopBuy.unk3C[i]].unkF & 0xF8) {
            s_shopBuy.unk4 = 1;
        }
        s_shopBuy.unk1++;
    }
}

void shopBuy_confirm(void) {
    MenuList *l;
    ShopBuy *b;
    Unk80069018 *it;
    u8 id;
    s32 n;

    l = &s_shopBuy.unk28;
    b = &s_shopBuy;
    id = b->unk3C[l->unk5 + l->cursor];
    it = &D_80069018[id];
    n = func_80027D84(id, b->unk2);
    if (n != 0) {
        g_gameState->unk1C->gil -= it->unk4 * n;
        shopBuy_closeQuantity();
    }
}

void shopBuy_updateHelp(void) {
    MenuList *l;
    u8 id;

    D_80077DE8.unk8 = NULL;
    l = &s_shopBuy.unk28;
    if (s_shopBuy.unk0 == 1) {
        id = s_shopBuy.unk3C[l->unk5 + l->cursor];
        D_80077DE8.unk8 = s_shopItemHelp + D_80069018[id].unk2;
    } else if (s_shopBuy.unk0 == 2) {
        D_80077DE8.unk8 = text_get(3);
    }
}

void shopSynth_init(void) {
    shopSynth_loadRecipes();
    s_shopSynth.unk0 = 1;
    func_80035580(NULL);
    func_8003556C(shopSynth_updateHelp);
    setRECT(&s_shopSynth.unk8, 0xBC, 0x26, 0x78, s_shopSynth.unk4 ? 0x71 : 0xAD);
    setRECT(&s_shopSynth.unk10, 8, 0x26, 0xB4, s_shopSynth.unk4 ? 0x71 : 0xAD);
    setRECT(&s_shopSynth.unk18, 8, 0x97, 0x12C, 0x45);
    setRECT(&s_shopSynth.unk20, 0x62, 0x60, 0x7C, 0x45);
    text_setBank(g_shopSynthText);
    shop_setHelp(0x11);
    func_8002EDFC(&s_shopSynth.unk28, s_shopSynth.unk1, s_shopSynth.unk4 ? 7 : 0xB, 1, s_shopSynth.unk10.h);
    shop_collectMembers();
}

void shopSynth_inputQuantity(void) {
    s32 q;
    s32 t;

    if (g_padRepeat & BTN_CIRCLE) {
        snd_playSe(0x65);
        shopSynth_closeQuantity();
        return;
    }
    if ((g_padRepeat & PAD_RIGHT) && s_shopSynth.unk2 < s_shopSynth.unk3) {
        snd_playSe(0x67);
        s_shopSynth.unk2++;
        return;
    }
    if ((g_padRepeat & PAD_LEFT) && s_shopSynth.unk2 >= 2) {
        snd_playSe(0x67);
        s_shopSynth.unk2--;
        return;
    }
    if ((g_padRepeat & PAD_UP) && s_shopSynth.unk2 < s_shopSynth.unk3) {
        snd_playSe(0x67);
        q = s_shopSynth.unk2 + 10;
        if (q > s_shopSynth.unk3) {
            q = s_shopSynth.unk3;
        }
        s_shopSynth.unk2 = q;
        return;
    }
    if ((g_padRepeat & PAD_DOWN) && s_shopSynth.unk2 >= 2) {
        snd_playSe(0x67);
        t = s_shopSynth.unk2 - 10;
        q = 1;
        if (t > 0) {
            q = t;
        }
        s_shopSynth.unk2 = q;
        return;
    }
    if (g_padRepeat & BTN_CROSS) {
        snd_playSe(0x415);
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

    r = &s_shopSynth.unk8;
    l = &s_shopSynth.unk28;
    s = &s_shopSynth.unk3C[l->unk5 + l->cursor];
    equip = (D_80069018[s->unk4].unkF & 0xF8) != 0;
    func_800269F4(r->x + 2, r->y + 0x36, 0x23, r->w - 5);
    func_8002DEE4(r, 0x23, 1);
    ui_drawLabel(r->x + 6, r->y - 3, 0x1E, D_80070460, 0);
    for (i = 0; i < 4; i++) {
        text_drawBlock(r->x + 8, r->y + s_shopSynthInfoRowY[i] - 4, 0x1E, text_getFrom(g_shopText, s_shopSynthInfoRowText[i]),
                      (s_shopSynthInfoRowAlways[i] | equip) ? 0 : 2);
    }
    text_formatNumber(g_gameState->unk1C->gil, buf, 7, 1);
    strcat(buf, s_shopSynthInfoGil);
    ui_drawSmallDigits(r->x + 0x39, r->y + s_shopSynthInfoRowY[0], 0x1E, buf, 0);
    text_formatNumber(item_getCount(s->unk4), buf, 2, 1);
    ui_drawSmallDigits(r->x + 0x5C, r->y + s_shopSynthInfoRowY[1], 0x1E, buf, 0);
    text_formatNumber(func_80027F94(s->unk4), buf, 2, 1);
    ui_drawSmallDigits(r->x + 0x5C, r->y + s_shopSynthInfoRowY[2], 0x1E, buf, equip ? 0 : 2);
    for (i = 0; i < 2; i++) {
        if (s->unk2[i] != 0xFF) {
            have = item_getCount(s->unk2[i]);
            if (s->unk2[0] == s->unk2[1]) {
                ok = i < have;
            } else {
                ok = have != 0;
            }
            ui_drawItemIcon(r->x + 0xC, r->y + 0x4C + i * 17, 0x1E, s->unk2[i], ok);
            text_drawBlock(r->x + 0x1C, r->y + 0x4D + i * 17, 0x1E, D_8006A218 + D_80069018[s->unk2[i]].unk0,
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
    RECT *r = &s_shopSynth.unk10;

    w = func_8002AE7C((u8 *)D_800703FC.unkA4);
    ui_drawLabel(r->x + 0xE, r->y - 3, 0x23, D_800703FC.unk68, 0);
    ui_drawLabel((r->x + 0xA6) - w, r->y - 3, 0x23, D_800703FC.unkA4, 0);
    list = &s_shopSynth.unk28;
    func_8002E040(r, 0x23, 1, list);
    menuList_getView(list, &view);
    win_setClip(NULL, 0x1E, 1);
    for (i = 0; i < view.unk0; i++) {
        shopSynth_drawRow(0, i, view.unk1 + i, &view);
    }
    if (list->unk8 & 4) {
        func_8002F40C(list, r, &sub);
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

    r = &s_shopSynth.unk18;
    l = &s_shopSynth.unk28;
    s = &s_shopSynth.unk3C[l->unk5 + l->cursor];
    if (s_shopSynth.unk4 == 0) {
        return;
    }
    func_8002DEE4(r, 0x23, 1);
    kind = func_80028014(s->unk4);
    if (kind < 0) {
        return;
    }
    armor = kind > 0;
    ui_drawLabel(r->x + 6, r->y - 3, 0x1D, ((u8 **)&D_800703FC)[s_shopSynthPartyLabel[armor]], 0);
    for (i = 0; i < s_shop.unk14; i++) {
        x = 0xF + i * 0x24;
        c = &g_gameState->unk1C->characters[(u8)s_shop.unk18[i]];
        ok = (D_80069018[s->unk4].unk6 & s_shopSynthEquipMask[chr_getDefaultSet(c)]) != 0;
        y = 0x9E;
        if (ok) {
            if (!armor) {
                new = D_8006E0E8[s->unk4].unk5;
                cur = D_8006E0E8[c->equip[0]].unk5;
            } else {
                eq = *(Equip5 *)c->equip;
                eq.v[kind] = s->unk4;
                cur = shop_sumArmorStat(kind, c->equip);
                new = shop_sumArmorStat(kind, eq.v);
            }
            if (kind == 4) {
                *(s32 *)buf = s_shopSynthNoStat;
            } else {
                text_formatNumber(new, buf, 3, 1);
            }
            shop_drawStatIcon(x - 1, y + 0x2E, 0x1E, s_shopSynthStatIcon[kind], 0);
            ui_drawSmallDigits(x + 0xC, y + 0x2F, 0x1E, buf, 0);
            if (cur < new) {
                shop_drawStatIcon(x - 1, y + 0x27, 0x1E, 5, 0);
            } else if (new < cur) {
                shop_drawStatIcon(x - 1, y + 0x28, 0x1E, 6, 0);
            }
        }
        func_80028204(x, y, 0x1E, func_80028954((u8)s_shop.unk18[i]), 0x2D, !ok);
    }
}

void shopSynth_drawQuantityWindow(Window *w) {
    u8 buf[16];
    ShopSynth *s;
    MenuList *l;
    SynthRecipe *r;
    Unk80069018 *it;
    u8 id;

    l = &s_shopSynth.unk28;
    r = &s_shopSynth.unk3C[l->unk5 + l->cursor];
    id = r->unk4;
    it = &D_80069018[id];
    ui_drawItemIcon(w->unk0 + 8, w->unk2 + 8, w->unk18, id, 1);
    text_drawBlock(w->unk0 + 0x18, w->unk2 + 8, w->unk18, D_8006A218 + it->unk0, 0);
    func_800269F4(w->unk0 + 4, w->unk2 + 0x18, w->unk18, w->unk4 - 9);
    text_drawBlock(w->unk0 + 0xC, w->unk2 + 0x1E, w->unk18, text_getFrom(g_shopText, 0x19), 0);
    text_drawBlock(w->unk0 + 0xC, w->unk2 + 0x2E, w->unk18, text_getFrom(g_shopText, 0x1A), 0);
    s = &s_shopSynth;
    text_formatNumber(s->unk2, buf, 2, 1);
    ui_drawSmallDigits(w->unk0 + 0x5C, w->unk2 + 0x22, w->unk18, buf, 0);
    text_formatNumber(s->unk2 * r->unk0, buf, 7, 1);
    strcat(buf, s_shopSynthQuantityGil);
    ui_drawSmallDigits(w->unk0 + 0x39, w->unk2 + 0x32, w->unk18, buf, 0);
}

void shopSynth_drawCursor(void) {
    if (!func_8002F810()) {
        if (s_shopSynth.unk0 == 1 || (D_80077DC0 & 3)) {
            D_80077DE8.unk4 = 0x14;
            D_80077DE8.unk0 = s_shopSynth.unk10.x - 8;
            D_80077DE8.unk2 = s_shopSynth.unk10.y + (s16)(s_shopSynth.unk28.cursor * 0xF + 0xB);
            func_800355E4();
        }
        if (s_shopSynth.unk0 == 2) {
            D_80077DE8.unk0 = s_shopSynth.unk38->unk0 - 0xE;
            D_80077DE8.unk2 = s_shopSynth.unk38->unk2 + 0x22;
            D_80077DE8.unk4 = s_shopSynth.unk38->unk18 - 1;
            func_800355E4();
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

    r = &s_shopSynth.unk3C[idx];
    ok = 0;
    id = r->unk4;
    it = &D_80069018[id];
    k = 1;
    if (item_getCount(id) < 99) {
        ok = g_gameState->unk1C->gil >= r->unk0;
    }
    x = 0x18;
    y = 0x26;
    y = row * 15 + (v->unk2 + y);
    i = 0;
loop:
    if (r->unk2[i] != 0xFF) {
        if (r->unk2[0] == r->unk2[1]) {
            t = i < item_getCount(r->unk2[i]);
        } else {
            t = item_getCount(r->unk2[i]);
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
    text_formatNumber(r->unk0, buf, 5, k);
    id = ok;
    ui_drawItemIcon(x, y, 0x1E, r->unk4, id);
    text_drawBlock(x + 0x10, y, 0x1E, D_8006A218 + it->unk0, id ? 0 : 2);
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

    l = &s_shopSynth.unk28;
    r = &s_shopSynth.unk3C[l->unk5 + l->cursor];
    need = 1;
    if (r->unk2[0] == r->unk2[1]) {
        need = 2;
    }
    have = item_getCount(r->unk4);
    if (have < 99 && g_gameState->unk1C->gil >= r->unk0 &&
        (r->unk2[0] == 0xFF || item_getCount(r->unk2[0]) >= need) &&
        (r->unk2[1] == 0xFF || item_getCount(r->unk2[1]) >= need)) {
        s_shopSynth.unk2 = 1;
        n = r->unk0 != 0 ? g_gameState->unk1C->gil / r->unk0 : 99;
        m2 = &s_shopSynth;
        if ((s32)n > 99 - have) {
            n = 99 - have;
        }
        m2->unk3 = n;
        m = m2;
        if (r->unk2[0] != 0xFF) {
            m->unk3 = MIN(m->unk3, item_getCount(r->unk2[0]) / need);
        }
        if (r->unk2[1] != 0xFF) {
            s_shopSynth.unk3 = MIN(s_shopSynth.unk3, item_getCount(r->unk2[1]) / need);
        }
        m3 = &s_shopSynth;
        s_shopSynth.unk0 = 0;
        m3->unk38 = func_8002DC54(&m3->unk20, 0xA, 2);
        win_setDrawContent(m3->unk38, shopSynth_drawQuantityWindow);
        win_setOnOpened(m3->unk38, shopSynth_onQuantityOpened);
        win_setOnClosed(m3->unk38, shopSynth_onQuantityClosed);
        func_8002E31C(m3->unk38, 1);
        func_8002E304(m3->unk38, D_800704A4);
        snd_playSe(0x67);
    } else {
        snd_playSe(0x66);
    }
}

void shopSynth_loadRecipes(void) {
    s32 mask;
    s32 i;
    s32 n;

    n = 0x40;
    mask = 1 << (shop_getId() - 0x20);
    s_shopSynth.unk1 = 0;
    for (i = 0; i < n; i++) {
        if ((s_shopRecipes[i].unk5 & mask) && s_shopRecipes[i].unk4 != 0xFF) {
            s_shopSynth.unk3C[s_shopSynth.unk1++] = s_shopRecipes[i];
            if (s_shopSynth.unk1 >= 0x20) {
                break;
            }
        }
    }
    for (i = 0; i < s_shopSynth.unk1; i++) {
        if (D_80069018[s_shopSynth.unk3C[i].unk4].unkF & 0xF8) {
            s_shopSynth.unk4 = 1;
        }
    }
}

void shopSynth_update(void) {
    shopSynth_input();
    shopSynth_draw();
    shopSynth_updateList();
}

void shopSynth_exit(void) {
    func_8003556C(NULL);
}

void shopSynth_input(void) {
    void (*fn)(void);

    if (!func_8002F810()) {
        fn = s_shopSynthInputFuncs[s_shopSynth.unk0];
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
    if (s_shopSynth.unk28.unk8 != 0) {
        func_8002EB74(&s_shopSynth.unk28);
    }
}

void shopSynth_inputList(void) {
    MenuList *m = &s_shopSynth.unk28;

    if (m->unk8 == 0) {
        if (g_padRepeat & BTN_CIRCLE) {
            snd_playSe(0x65);
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

    func_8002F344(&s_shopSynth.unk28, &v);
    for (i = 0; i < v.unk0; i++) {
        shopSynth_drawRow(0, i, v.unk1 + i, &v);
    }
}

void shopSynth_closeQuantity(void) {
    s_shopSynth.unk0 = 0;
    win_close(s_shopSynth.unk38);
}

void shopSynth_onQuantityOpened(void) {
    s_shopSynth.unk0 = 2;
    shop_setHelp(0x14);
}

void shopSynth_onQuantityClosed(void) {
    s_shopSynth.unk0 = 1;
    shop_setHelp(0x11);
}

void shopSynth_confirm(void) {
    MenuList *m = &s_shopSynth.unk28;
    SynthRecipe *r;
    s32 n;

    r = &s_shopSynth.unk3C[m->unk5 + m->cursor];
    n = func_80027D84(r->unk4, s_shopSynth.unk2);
    if (n != 0) {
        item_remove(r->unk2[0], n);
        item_remove(r->unk2[1], n);
        g_gameState->unk1C->gil -= r->unk0 * n;
        shopSynth_closeQuantity();
    }
}

void shopSynth_updateHelp(void) {
    MenuList *l;
    u8 id;

    D_80077DE8.unk8 = NULL;
    l = &s_shopSynth.unk28;
    if (s_shopSynth.unk0 == 1) {
        id = s_shopSynth.unk3C[l->unk5 + l->cursor].unk4;
        D_80077DE8.unk8 = s_shopItemHelp + D_80069018[id].unk2;
    } else if (s_shopSynth.unk0 == 2) {
        D_80077DE8.unk8 = text_get(4);
    }
}

void shopSell_init(void) {
    s_shopSell.unk0 = 1;
    text_setBank(g_shopSellText);
    shop_setHelp(0x10);
    func_80035580(NULL);
    func_8003556C(shopSell_updateHelp);
    func_8002EDFC(&s_shopSell.unk4, 0xFF, 0xB, 2, 0xAD);
    setRECT(&s_shopSell.unk14, 8, 0x2A, 0x12C, 0xAD);
    setRECT(&s_shopSell.unk1C[0], 0x62, 0x40, 0x7C, 0x45);
    setRECT(&s_shopSell.unk1C[1], 0x62, 0x87, 0x7C, 0x3E);
}

void shopSell_inputQuantity(void) {
    s32 q;
    s32 t;

    if (g_padRepeat & BTN_CIRCLE) {
        snd_playSe(0x65);
        shopSell_closeQuantity();
        return;
    }
    if ((g_padRepeat & PAD_RIGHT) && s_shopSell.unk1 < s_shopSell.unk2) {
        snd_playSe(0x67);
        s_shopSell.unk1++;
        return;
    }
    if ((g_padRepeat & PAD_LEFT) && s_shopSell.unk1 >= 2) {
        snd_playSe(0x67);
        s_shopSell.unk1--;
        return;
    }
    if ((g_padRepeat & PAD_UP) && s_shopSell.unk1 < s_shopSell.unk2) {
        snd_playSe(0x67);
        q = s_shopSell.unk1 + 10;
        if (q > s_shopSell.unk2) {
            q = s_shopSell.unk2;
        }
        s_shopSell.unk1 = q;
        return;
    }
    if ((g_padRepeat & PAD_DOWN) && s_shopSell.unk1 >= 2) {
        snd_playSe(0x67);
        t = s_shopSell.unk1 - 10;
        q = 1;
        if (t > 0) {
            q = t;
        }
        s_shopSell.unk1 = q;
        return;
    }
    if (g_padRepeat & BTN_CROSS) {
        snd_playSe(0x415);
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
    MenuList *list = &s_shopSell.unk4;

    ui_drawLabel(s_shopSellListRectPtr->x + 0xE, s_shopSellListRectPtr->y - 3, 0x23, D_800703FC.unk68, 0);
    ui_drawLabel(s_shopSellListRectPtr->x + 0xA4, s_shopSellListRectPtr->y - 3, 0x23, D_800703FC.unk68, 0);
    w = func_8002AE7C((u8 *)D_800703FC.unk94);
    ui_drawLabel((s_shopSellListRectPtr->x + 0x8A) - w, s_shopSellListRectPtr->y - 3, 0x23, D_800703FC.unk94, 0);
    ui_drawLabel((s_shopSellListRectPtr->x + 0x120) - w, s_shopSellListRectPtr->y - 3, 0x23, D_800703FC.unk94, 0);
    menuList_getView(list, &view);
    func_8002E040(s_shopSellListRectPtr, 0x23, 1, list);
    idx = view.unk1 * 2;
    win_setClip(NULL, 0x1E, 1);
    for (row = 0; row < view.unk0; row++) {
        for (col = 0; col < 2; col++) {
            shopSell_drawRow(col, row, idx++, &view);
        }
    }
    if (list->unk8 & 4) {
        func_8002F40C(list, s_shopSellListRectPtr, &sub);
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

    l = &s_shopSell.unk4;
    id = (g_gameState->unk1C->items.slots + ((l->unk5 + l->cursor) * 2 + l->unk7))->id;
    it = &D_80069018[id];
    price = it->unk4 >> 1;
    ui_drawItemIcon(w->unk0 + 8, w->unk2 + 8, w->unk18, id, 1);
    text_drawBlock(w->unk0 + 0x18, w->unk2 + 8, w->unk18, D_8006A218 + it->unk0, 0);
    func_800269F4(w->unk0 + 4, w->unk2 + 0x18, w->unk18, w->unk4 - 9);
    text_drawBlock(w->unk0 + 0xC, w->unk2 + 0x1E, w->unk18, text_getFrom(g_shopText, 0x19), 0);
    text_drawBlock(w->unk0 + 0xC, w->unk2 + 0x2E, w->unk18, text_getFrom(g_shopText, 0x1A), 0);
    m = &s_shopSell;
    text_formatNumber(m->unk1, buf, 2, 1);
    ui_drawSmallDigits(w->unk0 + 0x5C, w->unk2 + 0x22, w->unk18, buf, 0);
    text_formatNumber(m->unk1 * price, buf, 7, 1);
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
    l = &s_shopSell.unk4;
    e = &g_gameState->unk1C->items.slots[(l->unk5 + l->cursor) * 2 + l->unk7];
    equip = (D_80069018[e->unk0].unkF & 0xF8) != 0;
    for (i = 0; i < 3; i++) {
        text_drawBlock(r->x + 0xC, r->y + s_shopSellInfoRowY[i] - 4, w->unk18, text_getFrom(g_shopText, s_shopSellInfoRowText[i]),
                      (s_shopSellInfoRowAlways[i] | equip) ? 0 : 2);
    }
    text_formatNumber(g_gameState->unk1C->gil, buf, 7, 1);
    strcat(buf, s_shopSellInfoGil);
    ui_drawSmallDigits(r->x + 0x39, r->y + s_shopSellInfoRowY[0], w->unk18, buf, 0);
    text_formatNumber(item_getCount(e->unk0), buf, 2, 1);
    ui_drawSmallDigits(r->x + 0x5C, r->y + s_shopSellInfoRowY[1], w->unk18, buf, 0);
    if (equip) {
        text_formatNumber(func_80027F94(e->unk0), buf, 2, 1);
        ui_drawSmallDigits(r->x + 0x5C, r->y + s_shopSellInfoRowY[2], w->unk18, buf, 0);
    }
}

void shopSell_drawCursor(void) {
    MenuList *l;

    l = &s_shopSell.unk4;
    if (!func_8002F810()) {
        if (s_shopSell.unk0 == 1 || (D_80077DC0 & 3)) {
            D_80077DE8.unk4 = 0x14;
            D_80077DE8.unk0 = l->unk7 * 0x96;
            D_80077DE8.unk2 = l->cursor * 0xF + 0x34;
            func_800355E4();
        }
        if (s_shopSell.unk0 == 2) {
            D_80077DE8.unk0 = s_shopSell.unk2C[0]->unk0 - 0xE;
            D_80077DE8.unk2 = s_shopSell.unk2C[0]->unk2 + 0x22;
            D_80077DE8.unk4 = s_shopSell.unk2C[0]->unk18 - 1;
            func_800355E4();
        }
    }
}

void shopSell_openQuantity(void) {
    MenuList *l = &s_shopSell.unk4;
    UnkPair *p;
    s32 i;

    p = &g_gameState->unk1C->items.slots[(l->unk5 + l->cursor) * 2 + l->unk7];
    if (p->unk1 != 0) {
        s_shopSell.unk1 = 1;
        s_shopSell.unk2 = p->unk1;
        s_shopSell.unk0 = 0;
        for (i = 0; i < 2; i++) {
            s_shopSell.unk2C[i] = func_8002DC54(&s_shopSell.unk1C[i], s_shopSellWindows[i].unk8, s_shopSellWindows[i].unk0);
            win_setDrawContent(s_shopSell.unk2C[i], s_shopSellWindows[i].unk4);
            win_setOnOpened(s_shopSell.unk2C[i], shopSell_onQuantityOpened);
            win_setOnClosed(s_shopSell.unk2C[i], shopSell_onQuantityClosed);
            win_setOpenAnim(s_shopSell.unk2C[i], 0);
            func_8002E31C(s_shopSell.unk2C[i], 1);
            func_8002E304(s_shopSell.unk2C[i], D_800703FC.unk0[s_shopSellWindows[i].unkC]);
        }
        snd_playSe(0x67);
    } else {
        snd_playSe(0x66);
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

    l = &s_shopSell.unk4;
    m = &s_shopSell;
    id = (g_gameState->unk1C->items.slots + ((l->unk5 + l->cursor) * 2 + l->unk7))->id;
    it = &D_80069018[id];
    price = it->unk4 >> 1;
    n = item_remove(id, s_shopSell.unk1);
    if (n != 0) {
        g_gameState->unk1C->gil += price * n;
        if (g_gameState->unk1C->gil > 9999999) {
            g_gameState->unk1C->gil = 9999999;
        }
        s_shopSell.unk0 = 0;
        for (i = 0; i < 2; i++) {
            win_close(s_shopSell.unk2C[i]);
        }
    }
}

void shopSell_update(void) {
    shopSell_input();
    shopSell_draw();
    shopSell_updateList();
}

void shopSell_exit(void) {
    func_8003556C(NULL);
}

void shopSell_input(void) {
    void (*fn)(void);

    if (!func_8002F810()) {
        fn = s_shopSellInputFuncs[s_shopSell.unk0];
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
    if (s_shopSell.unk4.unk8 != 0) {
        func_8002EB74(&s_shopSell.unk4);
    }
}

void shopSell_inputList(void) {
    MenuList *m = &s_shopSell.unk4;

    if (m->unk8 == 0) {
        if (g_padRepeat & BTN_CIRCLE) {
            snd_playSe(0x65);
            shop_leaveMode();
            return;
        }
        if (g_padRepeat & BTN_CROSS) {
            shopSell_openQuantity();
            return;
        }
        menuList_input(m, g_padRepeat);
        func_8002F044(m, g_padRepeat);
        func_8002F0EC(m, g_padRepeat);
    }
}

void shopSell_drawScrollRows(void) {
    MenuView v;
    s32 row;
    s32 col;
    s32 idx;

    func_8002F344(&s_shopSell.unk4, &v);
    idx = v.unk1 * 2;
    for (row = 0; row < v.unk0; row++) {
        for (col = 0; col < 2; col++) {
            shopSell_drawRow(col, row, idx++, &v);
        }
    }
}

void shopSell_drawRow(s32 col, s32 row, s32 i, MenuView *v) {
    UnkPair *p = &g_gameState->unk1C->items.slots[i];
    Unk80069018 *it;
    s32 x, y;
    s32 k = 0x2A;
    u8 buf[8];

    if (p->unk1 != 0) {
        it = &D_80069018[p->unk0];
        x = col * 0x96;
        y = v->unk2 + (row * 0xF + k);
        text_formatNumber(p->unk1, buf, 2, 1);
        text_drawBlock(x + 0x28, y, 0x1E, D_8006A218 + it->unk0, 0);
        ui_drawItemIcon(x + 0x18, y, 0x1E, p->unk0, 1);
        ui_drawSmallDigits(x + 0x84, y + 4, 0x1E, buf, 0);
    }
}

void shopSell_closeQuantity(void) {
    s32 i;

    s_shopSell.unk0 = 0;
    for (i = 0; i < 2; i++) {
        win_close(s_shopSell.unk2C[i]);
    }
}

void shopSell_onQuantityOpened(Window *w) {
    s_shopSell.unk0 = 2;
    shop_setHelp(0x13);
}

void shopSell_onQuantityClosed(Window *w) {
    s_shopSell.unk0 = 1;
    shop_setHelp(0x10);
}

void shopSell_updateHelp(void) {
    Unk80077DE8 *h;
    MenuList *l;
    s32 idx;
    UnkPair *p;
    u8 *text;

    h = &D_80077DE8;
    h->unk8 = NULL;
    if (s_shopSell.unk0 == 1) {
        l = &s_shopSell.unk4;
        idx = (l->unk5 + l->cursor) * 2 + l->unk7;
        p = &g_gameState->unk1C->items.slots[idx];
        if (p->unk1 == 0) {
            return;
        }
        text = s_shopItemHelp + D_80069018[p->unk0].unk2;
    } else if (s_shopSell.unk0 == 2) {
        text = text_get(3);
    } else {
        return;
    }
    h->unk8 = text;
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
