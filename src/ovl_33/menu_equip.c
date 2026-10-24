#include "ovl_33.h"

void menuEquip_init(void) {
    s32 i;
    u8 lv;
    Character *c;
    EquipMenu *d;

    s_menuEquip.slot = MENU_NO_SLOT;
    s_menuEquip.tab = 0;
    s_menuEquip.state = EQUIP_STATE_TABS;
    s_menuEquip.equipSlot = 0;
    s_menuEquip.listOpen = 0;
    text_setBank(g_equipMenuText);
    help_setCallback(help_nullHandler);
    help_setCursorOnTop(1);
    setRECT(&s_menuEquip.tabRect, 8, 8, 0xE4, 0x1A);
    setRECT(&s_menuEquip.infoRect, 8, 0x26, 0x8C, 0xB6);
    setRECT(&s_menuEquip.equipRect, 0x94, 0x26, 0xA0, 0x62);
    setRECT(&s_menuEquip.abilityRect, 0x94, 0x8A, 0xA0, 0x52);
    setRECT(&s_menuEquip.listRect, 0x94, 0x26, 0xA0, 0x62);
    for (i = 0; i < 4; i++) {
        lv = 0;
        c = g_gameState->unk1C->party[i];
        d = (EquipMenu *)((u8 *)&s_menuEquip + i); /* &s_menuEquip.initStones[i] would not match */
        if (c != NULL) {
            lv = c->stones;
        }
        d->initStones[0] = lv;
    }
    func_80063118(&s_menuEquip.listWin, 0, 1);
    menuEquip_setMember(g_menuScreen.unk3, 0);
    ui_initMemberSwitch(&s_menuEquip.header, g_menuScreen.unk3, menuEquip_onMemberSwitched);
}

void menuEquip_inputTabs(void) {
    u8 slot;
    s32 next;

    if (g_padRepeat & BTN_CIRCLE) {
        snd_playSe(SE_CANCEL);
        menuScreen_open(0);
    } else if (g_padRepeat & BTN_SQUARE) {
        snd_playSe(SE_CURSOR);
        menuScreen_switch(MENU_SCREEN_ABILITY);
    } else if (g_padRepeat & PAD_LEFT) {
        snd_playSe(SE_CURSOR);
        s_menuEquip.tab = s_menuEquip.tab != 0 ? s_menuEquip.tab - 1 : EQUIP_TAB_OFF;
    } else if (g_padRepeat & PAD_RIGHT) {
        snd_playSe(SE_CURSOR);
        s_menuEquip.tab = s_menuEquip.tab >= EQUIP_TAB_OFF ? 0 : s_menuEquip.tab + 1;
    } else if (g_padRepeat & BTN_R1) {
        slot = s_menuEquip.slot;
        next = party_getNextMember(slot);
        if (slot != next) {
            menuEquip_setMember(next, 2);
            snd_playSe(SE_PAGE);
        }
    } else if (g_padRepeat & BTN_L1) {
        slot = s_menuEquip.slot;
        next = party_getPrevMember(slot);
        if (slot != next) {
            menuEquip_setMember(next, 1);
            snd_playSe(SE_PAGE);
        }
    } else if (g_padRepeat & BTN_CROSS) {
        if (s_menuEquip.tab == EQUIP_TAB_OPTIMIZE) {
            menuEquip_optimize();
        } else {
            snd_playSe(SE_CURSOR);
            s_menuEquip.state = EQUIP_STATE_SLOT;
        }
    }
}

void menuEquip_inputSlot(void) {
    u8 slot;
    s32 next;

    if (g_padRepeat & BTN_CIRCLE) {
        snd_playSe(SE_CANCEL);
        s_menuEquip.state = EQUIP_STATE_TABS;
    } else if (g_padRepeat & PAD_UP) {
        snd_playSe(SE_CURSOR);
        s_menuEquip.equipSlot = s_menuEquip.equipSlot == 0 ? EQUIP_SLOT_COUNT - 1 : s_menuEquip.equipSlot - 1;
    } else if (g_padRepeat & PAD_DOWN) {
        snd_playSe(SE_CURSOR);
        s_menuEquip.equipSlot = s_menuEquip.equipSlot >= EQUIP_SLOT_COUNT - 1 ? 0 : s_menuEquip.equipSlot + 1;
    } else if (g_padRepeat & BTN_R1) {
        slot = s_menuEquip.slot;
        next = party_getNextMember(slot);
        if (slot != next) {
            snd_playSe(SE_PAGE);
            menuEquip_setMember(next, 2);
        }
    } else if (g_padRepeat & BTN_L1) {
        slot = s_menuEquip.slot;
        next = party_getPrevMember(slot);
        if (slot != next) {
            snd_playSe(SE_PAGE);
            menuEquip_setMember(next, 1);
        }
    } else if (g_padRepeat & BTN_CROSS) {
        if (s_menuEquip.tab == EQUIP_TAB_EQUIP) {
            snd_playSe(SE_CURSOR);
            menuEquip_buildItemList();
            menuEquip_openList();
        } else {
            menuEquip_removeSlot();
        }
    }
}

void menuEquip_drawInfo(void) {
    Unk80032E84In in;
    Unk80032E84Out cur;
    Unk80032E84Out out;
    RECT *r;
    Character *c;

    r = &s_menuEquip.infoRect;
    c = g_gameState->unk1C->party[s_menuEquip.slot];
    ui_drawEtchedLine(r->x + 3, r->y + 0x3B, 0x1E, r->w - 6);
    ui_drawLabel(r->x + 6, r->y - 3, 0x1E, D_80070460, 0);
    win_drawBox(r, 0x1E, 1);
    ui_drawMemberHeader(r->x + 8, r->y + 9, 0x1D, (Unk80024428 *)&s_menuEquip.header);
    cur.unk8[0] = c->speed;
    cur.unk8[1] = c->strength;
    cur.unk8[2] = c->magic;
    cur.unk8[3] = c->spirit;
    cur.unkC[0] = D_8006E0E8[c->equip[0]].unk5;
    cur.unkC[1] = c->defense;
    cur.unkC[2] = c->evade;
    cur.unkC[3] = c->magicDef;
    cur.unkC[4] = c->magicEva;
    if (s_menuEquip.state == EQUIP_STATE_LIST && s_menuEquip.list.rows != 0) {
        in.unk0 = *(Bytes8 *)&c->unk2C;
        in.unk10 = c->hp;
        in.unk12 = c->mp;
        in.unk14 = *(Bytes5 *)c->equip;
        in.unk8 = *(Words2 *)c->supportAbilities;
        in.unk14.unk0[s_menuEquip.equipSlot] = (s_menuEquip.items + (s_menuEquip.list.cursor + s_menuEquip.list.top))->id;
        chr_calcEquipStats(&in, &out);
        menuEquip_drawStatCompare(&cur, &out);
        return;
    } else if (*(u16 *)&s_menuEquip == EQUIP_TAB_STATE(EQUIP_TAB_OFF, EQUIP_STATE_SLOT) && s_menuEquip.equipSlot != 0) {
        in.unk0 = *(Bytes8 *)&c->unk2C;
        in.unk10 = c->hp;
        in.unk12 = c->mp;
        in.unk14 = *(Bytes5 *)c->equip;
        in.unk8 = *(Words2 *)c->supportAbilities;
        in.unk14.unk0[s_menuEquip.equipSlot] = ITEM_NONE;
        chr_calcEquipStats(&in, &out);
        menuEquip_drawStatCompare(&cur, &out);
        return;
    }
    menuEquip_drawStatCompare(&cur, NULL);
}

void menuEquip_drawEquipment(void) {
    Character *c;
    RECT *r;
    s32 i;
    s32 grey;
    u8 sel;
    s32 y8;
    s32 y12;
    s32 y10;
    Unk80069018 *it;

    r = &s_menuEquip.equipRect;
    c = g_gameState->unk1C->party[s_menuEquip.slot];
    if (s_menuEquip.state == EQUIP_STATE_LIST) {
        return;
    }
    ui_drawLabel(r->x + 6, r->y - 3, 0x1E, D_8007043C, 0);
    win_drawBox(r, 0x1E, 2);
    win_drawShadow(r, 0x1E, 2);
    for (i = 0, grey = 2, y8 = 8, y12 = 12, y10 = 10; i < 5; i++) {
        sel = 0;
        if (s_menuEquip.tab == grey) {
            sel = (i == 0);
        }
        ui_drawSprite(r->x + 10, r->y + y10, 0x1D, i + 0xF9, sel);
        ui_drawSmallDigits(r->x + 0x19, r->y + y12, 0x1D, D_801EE644, sel ? grey : 0);
        if (c->equip[i] != ITEM_NONE) {
            it = &D_80069018[c->equip[i]];
            ui_drawItemIcon(r->x + 0x21, r->y + y8, 0x1D, c->equip[i], !sel);
            text_drawBlock(r->x + 0x31, r->y + y8, 0x1D, D_8006A218 + it->unk0, sel ? grey : 0);
        }
        y8 += 17;
        y12 += 17;
        y10 += 17;
    }
}

void menuEquip_drawAbilities(void) {
    RECT *r;
    Character *c;
    u8 *name;
    s32 id;
    s32 ab;
    s32 i;
    s32 y1;
    s32 y2;
    s32 y3;
    s32 k;
    s32 icon;
    s32 ap;
    u8 ok;
    u8 flag;

    r = &s_menuEquip.abilityRect;
    c = g_gameState->unk1C->party[s_menuEquip.slot];
    ui_drawLabel(r->x + 6, r->y - 3, 0x13, D_80070400, 0);
    ui_drawEtchedLine(r->x + 3, r->y + 0x1A, 0x1E, r->w - 6);
    win_drawBox(r, 0x1E, 1);
    if (s_menuEquip.state < EQUIP_STATE_SLOT) {
        return;
    }
    ui_drawSprite(r->x + 0xA, r->y + 9, 0x1D, s_menuEquip.equipSlot + 0xF9, 0);
    ui_drawSmallDigits(r->x + 0x19, r->y + 0xB, 0x1D, D_801EE644, 0);
    if (s_menuEquip.state == EQUIP_STATE_SLOT) {
        id = c->equip[s_menuEquip.equipSlot];
    } else {
        id = s_menuEquip.items[s_menuEquip.list.top + s_menuEquip.list.cursor].id;
    }
    if (id == ITEM_NONE) {
        return;
    }
    ui_drawItemIcon(r->x + 0x21, r->y + 7, 0x1D, id, 1);
    text_drawBlock(r->x + 0x31, r->y + 7, 0x1D, D_8006A218 + D_80069018[id].unk0, 0);
    for (i = 0; i < 3; i++) {
        y3 = i * 15;
        y2 = i * 15 + 0x21;
        y1 = i * 15 + 0x22;
        ab = D_80069018[id].unkC[i];
        if (ab == 0) {
            continue;
        }
        if (ab < ABILITY_SUPPORT_FIRST) {
            name = s_menuEquipActionNames + s_menuEquipActionTable[ab].unkE;
        } else {
            name = s_menuEquipSupportNames + D_8006D128[ab - ABILITY_SUPPORT_FIRST].unk2;
        }
        if (func_80034A30(c)) {
            ok = chr_findAbilitySlot(c->unk34.b.id, ab) >= 0;
            ap = chr_getApNeeded(c->unk34.b.id, ab);
            if (ok) {
                if (ab < ABILITY_SUPPORT_FIRST) {
                    icon = 0xBC;
                } else if (chr_testSupportBit(c->supportAbilities, ab)) {
                    icon = 0xBD;
                } else {
                    icon = 0xF4;
                }
            } else {
                if (ab < ABILITY_SUPPORT_FIRST) {
                    icon = 0xBE;
                } else {
                    icon = 0xBF;
                }
            }
            if (ok) {
                if (ab < ABILITY_SUPPORT_FIRST) {
                    flag = !(s_menuEquipActionTable[ab].unkB & 2);
                } else {
                    flag = 1;
                }
                k = 0x26;
                ui_drawApGauge(r->x + 0x72, r->y + (y3 + k), 0x1D, chr_getAp(c->unk34.b.id, ab), ap, flag, 0);
            }
            text_drawBlock(r->x + 0x18, r->y + y2, 0x1D, name, ok ? 0 : 2);
            ui_drawSprite(r->x + 8, r->y + y1, 0x1D, icon, ok ^ 1);
        } else {
            text_drawBlock(r->x + 0x18, r->y + y2, 0x1D, name, 2);
            ui_drawSprite(r->x + 8, r->y + y1, 0x1D, ab < ABILITY_SUPPORT_FIRST ? 0xBE : 0xBF, 1);
        }
    }
}

void menuEquip_drawList(void) {
    RECT sub;
    MenuView view;
    WinDef *wd = &s_menuEquip.listWin;
    RECT *r = &wd->unk4;
    EquipMenu *m = &s_menuEquip;
    MenuList *l = &s_menuEquip.list;
    s32 w;
    s32 k = 0x97;
    s32 i;

    if (m->listOpen != 0) {
        w = ui_measureLabel((u8 *)D_800703FC.unk94);
        ui_drawLabel(wd->unk4.x + 6, r->y - 3, 0x14, D_800703FC.unk0[s_menuEquipSlotLabel[m->equipSlot]], 0);
        ui_drawLabel(wd->unk4.x - (w - k), r->y - 3, 0x14, D_800703FC.unk94, 0);
        win_drawListBox(r, 0x14, 1, l);
        menuList_getView(l, &view);
        win_setClip(NULL, 0x13, 1);
        for (i = 0; i < view.unk0; i++) {
            menuEquip_drawListRow(0, i, view.unk1 + i, &view);
        }
        if (l->scroll & 4) {
            menuList_getRowsRect(l, r, &sub);
            win_setClip(&sub, 0x13, 1);
            menuEquip_drawScrollRows();
        }
        if (wd->unkC == 0) {
            win_setClip(r, 0x13, 1);
        }
    }
}

void menuEquip_drawStatCompare(Unk80032E84Out *cur, Unk80032E84Out *new) {
    u8 buf[16];
    RECT *r;
    s32 i;
    s32 y;
    s32 col;
    u8 diff;
    u8 blink;

    r = &s_menuEquip.infoRect;
    blink = ((u32)D_80077DC0 >> 4) & 1;
    for (i = 0, y = r->y + 0x44; i < 4; y += 0xC, i++) {
        menuEquip_drawStatLabel(r->x + 8, y, 0x1D, s_menuEquipBaseStatText[i], 0);
        text_formatNumber(cur->unk8[i], buf, 2, 1);
        ui_drawSmallDigits(r->x + 0x4A, y, 0x1D, buf, 0);
        if (new != NULL) {
            diff = cur->unk8[i] != new->unk8[i];
            if (new->unk8[i] >= cur->unk8[i]) {
                if (diff) {
                    col = 7;
                } else {
                    col = 0;
                }
            } else {
                col = 3;
            }
            text_formatNumber(new->unk8[i], buf, 2, 1);
            ui_drawSmallDigits(r->x + 0x5C, y, 0x1D, s_menuEquipArrow, (diff && blink) ? 0 : 2);
            ui_drawSmallDigits(r->x + 0x6D, y, 0x1D, buf, col);
        }
    }
    for (i = 0, y = r->y + 0x78; i < 5; y += 0xC, i++) {
        menuEquip_drawStatLabel(r->x + 8, y, 0x1D, s_menuEquipBattleStatText[i], 0);
        text_formatNumber(cur->unkC[i], buf, 3, 1);
        ui_drawSmallDigits(r->x + 0x43, y, 0x1D, buf, 0);
        if (new != NULL) {
            diff = cur->unkC[i] != new->unkC[i];
            if (new->unkC[i] >= cur->unkC[i]) {
                if (diff) {
                    col = 7;
                } else {
                    col = 0;
                }
            } else {
                col = 3;
            }
            text_formatNumber(new->unkC[i], buf, 3, 1);
            ui_drawSmallDigits(r->x + 0x5C, y, 0x1D, s_menuEquipArrow, (diff && blink) ? 0 : 2);
            ui_drawSmallDigits(r->x + 0x66, y, 0x1D, buf, col);
        }
    }
}

void menuEquip_drawCursor(void) {
    EquipMenu *m = &s_menuEquip;
    u16 x;
    u16 k;
    HelpState *h;

    help_setCursorOnTop(m->state != EQUIP_STATE_TABS);
    if (fade_isBusy() || m->state == EQUIP_STATE_BUSY) {
        return;
    }
    if (m->state == EQUIP_STATE_TABS || (D_80077E54 & 3)) {
        h = &g_menuHelp;
        x = s_menuEquipTabCursorX[s_menuEquip.tab][0];
        h->y = m->tabRect.y + 8;
        h->otz = 0x1B;
        h->x = m->tabRect.x + x - 0x18;
        help_drawCursor();
    }
    if (s_menuEquip.state == EQUIP_STATE_SLOT) {
        g_menuHelp.otz = 0x12;
        g_menuHelp.x = s_menuEquip.equipRect.x - 0x10;
        k = s_menuEquip.equipSlot * 17 + 0xC;
        g_menuHelp.y = s_menuEquip.equipRect.y + k;
        help_drawCursor();
    } else if (s_menuEquip.state == EQUIP_STATE_LIST) {
        g_menuHelp.otz = 0x12;
        g_menuHelp.x = s_menuEquip.listWin.unk4.x - 0xD;
        k = s_menuEquip.list.cursor * 15 + 0xA;
        g_menuHelp.y = s_menuEquip.listWin.unk4.y + k;
        help_drawCursor();
    }
}

void menuEquip_optimize(void) {
    s32 best;
    s32 i;
    s32 j;
    s32 max;
    s32 kind;
    s32 cur;
    Character *c;
    UnkPair *p;
    Unk80069018 *it;

    best = 0;
    i = 0;
    c = g_gameState->unk1C->party[s_menuEquip.slot];
    kind = chr_getDefaultSet(c);
    for (; i < EQUIP_SLOT_COUNT; i++) {
        if (c->equip[i] != ITEM_NONE) {
            cur = D_80069018[c->equip[i]].unkA;
        } else {
            cur = -1;
        }
        for (j = 0, max = -1, p = g_gameState->unk1C->items.slots; j < ITEM_SLOT_COUNT; j++, p++) {
            if (p->unk1 != 0) {
                it = &D_80069018[p->unk0];
                if ((it->unkF & s_menuEquipOptSlotMask[i]) && (it->unk6 & s_menuEquipOptCharMask[kind])) {
                    if (it->unkA > max) {
                        max = it->unkA;
                        best = p->unk0;
                    }
                }
            }
        }
        if (cur < max) {
            if (c->equip[i] != ITEM_NONE) {
                item_add(c->equip[i], 1);
            }
            if (item_remove(best, 1) != 0) {
                c->equip[i] = best;
            }
        }
    }
    menuEquip_applyEquip(c);
    snd_playSe(SE_EQUIP);
}

void menuEquip_dropUnlearnedSupport(Character *c) {
    s32 i;
    s32 j;
    AbilityEntry *a;
    Unk80069018 *it;
    s32 id;

    if (func_80034A30(c)) {
        memset(s_menuEquipTaught, 0, SUPPORT_COUNT);
        for (i = 0; i < EQUIP_SLOT_COUNT; i++) {
            if (c->equip[i] != ITEM_NONE) {
                it = &D_80069018[c->equip[i]];
                for (j = 0; j < 3; j++) {
                    id = it->unkC[j];
                    if (id != 0) {
                        if (id >= ABILITY_SUPPORT_FIRST) {
                            s_menuEquipTaught[id - ABILITY_SUPPORT_FIRST] = 1;
                        }
                    }
                }
            }
        }
        a = D_8006CB28[c->unk34.b.row];
        for (i = 0; i < ABILITY_MAX; i++, a++) {
            if (a->unk0 >= ABILITY_SUPPORT_FIRST && chr_hasSupportAbility(c->unk34.b.id, a->unk0)) {
                if (s_menuEquipTaught[a->unk0 - ABILITY_SUPPORT_FIRST] == 0 && c->ap[i] < a->unk1) {
                    chr_setSupportAbility(c->unk34.b.id, a->unk0, 0);
                    if (c->maxStones - c->stones >= D_8006D128[a->unk0 - ABILITY_SUPPORT_FIRST].unk1) {
                        c->stones += D_8006D128[a->unk0 - ABILITY_SUPPORT_FIRST].unk1;
                    } else {
                        c->stones = c->maxStones;
                    }
                }
            }
        }
    }
}

void menuEquip_buildItemList(void) {
    ItemSlot tmp;
    Unk80069018 *it;
    s32 n;
    s32 i;
    s32 j;
    u16 mask;
    u8 slot;
    u8 a;
    u8 b;
    u8 ra;
    u8 rb;

    n = 0;
    mask = s_menuEquipCharMask[chr_getDefaultSet(g_gameState->unk1C->party[s_menuEquip.slot])];
    slot = s_menuEquipSlotMask[s_menuEquip.equipSlot];
    s_menuEquip.items[0].id = ITEM_NONE;
    for (i = 0; i < ITEM_SLOT_COUNT; i++) {

        if (g_gameState->unk1C->items.slots[i].count != 0) {
            it = &D_80069018[g_gameState->unk1C->items.slots[i].id];
            if ((it->unk6 & mask) && (it->unkF & slot)) {
                s_menuEquip.items[n] = g_gameState->unk1C->items.slots[i];
                n++;
            }
        }
    }
    for (i = 0; i < n - 1; i++) {
        for (j = i + 1; j < n; j++) {
            a = s_menuEquip.items[i].id;
            b = s_menuEquip.items[j].id;
            ra = D_80069018[a].unkA;
            rb = D_80069018[b].unkA;
            if ((ra == rb && b < a) || rb < ra) {
                tmp = s_menuEquip.items[i];
                s_menuEquip.items[i] = s_menuEquip.items[j];
                s_menuEquip.items[j] = tmp;
            }
        }
    }
    menuList_init(&s_menuEquip.list, n, 6, 1, 0x62);
}

void menuEquip_updateHelp(void) {
    Character *c;
    u8 *buf;
    s32 id;
    UnkPair *e;
    MenuList *l;

    c = g_gameState->unk1C->party[s_menuEquip.slot];
    g_menuHelp.text = NULL;
    if (s_menuEquip.state == EQUIP_STATE_TABS) {
        buf = s_menuEquipHelpBuf;
        g_menuHelp.text = buf;
        text_setStringArg(0, s_menuEquipHelpArg);
        text_copy(g_menuHelp.text, text_get(s_menuEquip.tab + EQUIP_TEXT_TAB_HELP));
        text_append(g_menuHelp.text, text_get(EQUIP_TEXT_TO_ABILITY));
    } else if (s_menuEquip.state == EQUIP_STATE_SLOT) {
        id = c->equip[s_menuEquip.equipSlot];
        if (id == ITEM_NONE) {
            g_menuHelp.text = text_get(EQUIP_TEXT_NOTHING);
        } else {
            g_menuHelp.text = s_menuEquipItemHelp + D_80069018[id].unk2;
        }
    } else if (s_menuEquip.state == EQUIP_STATE_LIST && (l = &s_menuEquip.list)->rows != 0) {
        e = s_menuEquip.items + (l->top + l->cursor);
        if (e->unk1 != 0) {
            g_menuHelp.text = s_menuEquipItemHelp + D_80069018[e->unk0].unk2;
        }
    }
}

void menuEquip_update(void) {
    menuEquip_input();
    menuEquip_draw();
    menuEquip_updateAnims();
}

void menuEquip_exit(void) {
    D_80077E53 = s_menuEquip.slot;
    win_closeAllNow();
    help_setCursorOnTop(1);
    help_setCallback(NULL);
}

void menuEquip_input(void) {
    void (*fn)(void);

    if (!fade_isBusy()) {
        fn = s_menuEquipInput[s_menuEquip.state];
        if (fn != NULL) {
            fn();
        }
    }
}

void menuEquip_draw(void) {
    menuEquip_drawTabs();
    menuEquip_drawInfo();
    menuEquip_drawEquipment();
    menuEquip_drawAbilities();
    menuEquip_drawList();
    menuEquip_drawCursor();
}

void menuEquip_updateAnims(void) {
    if (help_isOpen()) {
        menuEquip_updateHelp();
    }
    if (s_menuEquip.state == EQUIP_STATE_LIST && s_menuEquip.list.scroll != 0) {
        menuList_update(&s_menuEquip.list);
    }
    if (s_menuEquip.listWin.unkC != 0) {
        func_800631AC(&s_menuEquip.listWin);
    }
    if (s_menuEquip.header.unk0 != 0) {
        ui_updateMemberSwitch(&s_menuEquip.header);
    }
}

void menuEquip_inputList(void) {
    if (s_menuEquip.list.scroll == 0) {
        if (g_padRepeat & BTN_CIRCLE) {
            snd_playSe(SE_CANCEL);
            menuEquip_closeList();
            return;
        }
        if (g_padRepeat & BTN_CROSS) {
            menuEquip_equipSelected();
            return;
        }
        menuList_input(&s_menuEquip.list, g_padRepeat);
    }
}

void menuEquip_drawTabs(void) {
    RECT *r;
    s32 i;

    r = &s_menuEquip.tabRect;
    win_drawBox(r, 0x1E, 2);
    win_drawShadow(r, 0x1E, 2);
    for (i = 0; i < 3; i++) {
        text_drawBlock(r->x + s_menuEquipTabX[i], r->y + 6, 0x1D, text_get(i), 0);
    }
}

void menuEquip_drawScrollRows(void) {
    MenuView v;
    s32 i;

    menuList_getPageView(&s_menuEquip.list, &v);
    for (i = 0; i < v.unk0; i++) {
        menuEquip_drawListRow(0, i, v.unk1 + i, &v);
    }
}

void menuEquip_drawListRow(s32 col, s32 row, s32 i, MenuView *v) {
    WinDef *w;
    UnkPair *p;
    s32 x;
    s32 y;
    u8 buf[16];

    p = s_menuEquip.items + i;
    x = s_menuEquip.listWin.unk4.x;
    w = &s_menuEquip.listWin;
    y = w->unk4.y + row * 0xF + v->unk2;
    ui_drawItemIcon(x + 0xB, y, 0x13, p->unk0, 1);
    text_drawBlock(x + 0x1B, y, 0x13, D_8006A218 + D_80069018[p->unk0].unk0, 0);
    text_formatNumber(p->unk1, buf, 2, 1);
    ui_drawSmallDigits(x + 0x8A, y + 4, 0x13, buf, 0);
}

void menuEquip_openList(void) {
    WinDef *w = &s_menuEquip.listWin;

    w->unk4 = s_menuEquip.listRect;
    s_menuEquip.listOpen = 1;
    s_menuEquip.state = EQUIP_STATE_BUSY;
    func_80063118(w, &s_menuEquipListOpen, 1);
}

void menuEquip_closeList(void) {
    WinDef *w = &s_menuEquip.listWin;

    s_menuEquip.state = EQUIP_STATE_BUSY;
    func_80063118(w, &s_menuEquipListClose, 0);
}

void menuEquip_onListOpened(void) {
    s_menuEquip.state = EQUIP_STATE_LIST;
}

void menuEquip_onListClosed(void) {
    s_menuEquip.state = EQUIP_STATE_SLOT;
    s_menuEquip.listOpen = 0;
}

void menuEquip_equipSelected(void) {
    GameState *gs;
    EquipMenu *m;
    MenuList *l;
    Character *c;
    s32 old;
    s32 item;

    gs = g_gameState;
    m = &s_menuEquip;
    l = &m->list;
    c = gs->unk1C->party[m->slot];
    old = c->equip[m->equipSlot];
    item = m->items[l->top + l->cursor].id;
    if (l->rows == 0) {
        snd_playSe(SE_BUZZER);
        return;
    }
    if (old != ITEM_NONE) {
        item_add(old, 1);
    }
    if (item_remove(item, 1) != 0) {
        c->equip[m->equipSlot] = item;
        menuEquip_applyEquip(c);
        menuEquip_closeList();
    }
    snd_playSe(SE_EQUIP);
}

void menuEquip_removeSlot(void) {
    s32 se;
    Character *c;
    u8 ok;
    u8 old;

    ok = 0;
    if (s_menuEquip.equipSlot != 0) {
        c = g_gameState->unk1C->party[s_menuEquip.slot];
        old = c->equip[s_menuEquip.equipSlot];
        if (old != ITEM_NONE) {
            item_add(old, 1);
            c->equip[s_menuEquip.equipSlot] = ITEM_NONE;
            menuEquip_applyEquip(c);
            ok = 1;
        }
    }
    se = SE_EQUIP;
    if (!ok) {
        se = SE_BUZZER;
    }
    snd_playSe(se);
}

void menuEquip_onMemberSwitched(void) {
    s_menuEquip.state = s_menuEquip.savedState;
}

void menuEquip_setMember(s32 slot, s32 dir) {
    u8 s;

    ui_startMemberSwitch(&s_menuEquip.header, s_menuEquip.slot, slot, dir);
    if (dir != 0) {
        s = s_menuEquip.state;
        s_menuEquip.state = EQUIP_STATE_BUSY;
        s_menuEquip.savedState = s;
    }
    s_menuEquip.slot = slot;
}

void menuEquip_applyEquip(Character *c) {
    menuEquip_dropUnlearnedSupport(c);
    chr_updateStats(c);
    c->unk34.b.unk4 = chr_calcAppearance(c->unk34.b.id, (c->unkA >> 4) & 1, c->equip);
}

void menuEquip_drawStatLabel(s32 x, s32 y, s32 z, u8 id, s32 color) {
    text_drawBlock(x, y - 4, z, text_get(id), color);
}

INCLUDE_RODATA("asm/us/ovl_33/nonmatchings/menu_equip", D_801EE644);

INCLUDE_RODATA("asm/us/ovl_33/nonmatchings/menu_equip", g_equipMenuText);
