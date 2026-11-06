#include "ovl_33.h"

/* Inlined copy of menuAbility_getSupportState (support ability state). */
static inline s32 menuAbility_getSupportState_inline(s32 id, Character *chr) {
    Unk8006D128 *sup;
    s32 slot;
    s32 ap;
    s32 need;

    if (id < ABILITY_SUPPORT_FIRST) {
        return 0;
    }
    if (chr_testSupportBit(chr->supportAbilities, id)) {
        return ABILITY_EQUIPPED;
    }
    if (s_menuAbility.equipSlots[id] == 0) {
        slot = chr_findAbilitySlot(chr->info.b.id, id);
        if (slot < 0) {
            return 0;
        }
        if (chr_canLearnAbilities(chr)) {
            ap = chr->ap[slot];
            need = g_abilityRows[chr->info.b.row][slot].ap;
            if (ap == 0) {
                return 0;
            }
            if (ap < need) {
                return 0;
            }
        }
    }
    sup = &g_supportAbilities[id - ABILITY_SUPPORT_FIRST];
    if (sup->stones > chr->stones) {
        return ABILITY_GREYED;
    }
    return ABILITY_USABLE;
}

void menuAbility_init(void) {
    s32 i;

    s_menuAbility.slot = MENU_NO_SLOT;
    s_menuAbility.state = ABILITY_STATE_TABS;
    s_menuAbility.tab = 0;
    ui_initMemberSwitch(&s_menuAbility.header, g_menuScreen.member, menuAbility_onMemberSwitched);
    menuAbility_setMember(g_menuScreen.member, 0);
    s_menuAbility.animFrame = 0;
    s_menuAbility.animTick = 0;
    text_setBank(g_abilityMenuText);
    menuAbility_onInit();
    help_setCallback(help_nullHandler);
    help_setCursorOnTop(1);
    setRECT(&s_menuAbility.tabRect, ABIL_TABS_WIN_X, ABIL_TABS_WIN_Y, ABIL_TABS_WIN_W, ABIL_TABS_WIN_H);
    setRECT(&s_menuAbility.playerRect, ABIL_PLAYER_WIN_X, ABIL_PLAYER_WIN_Y, ABIL_PLAYER_WIN_W, ABIL_PLAYER_WIN_H);
    setRECT(&s_menuAbility.infoRect, ABIL_INFO_WIN_X, ABIL_INFO_WIN_Y, ABIL_INFO_WIN_W, ABIL_INFO_WIN_H);
    setRECT(&s_menuAbility.commandRect, ABIL_CMD_WIN_X, ABIL_CMD_WIN_Y, ABIL_CMD_WIN_W, ABIL_CMD_WIN_H);
    setRECT(&s_menuAbility.listRect, ABIL_LIST_WIN_X, ABIL_LIST_WIN_Y, ABIL_LIST_WIN_W, ABIL_LIST_WIN_H);
    setRECT(&s_menuAbility.targetRect, 0, ABIL_TARGET_WIN_Y, ABIL_TARGET_WIN_W, ABIL_TARGET_WIN_H);
    s_menuAbility.targetWinOpen = 0;
    tween_start(&s_menuAbility.targetWin, 0, 0);
    menuAbility_loadImages();
    s_menuAbility.partySize = 0;
    for (i = 0; i < PARTY_SIZE; i++) {
        if (g_gameState->gameData->party[i] != NULL) {
            s_menuAbility.partySize++;
        }
    }
}

void menuAbility_updateAnims(void) {
    AbilityMenu *menu;

    if (help_isOpen()) {
        menuAbility_updateHelp();
    }
    menu = &s_menuAbility;
    if (menu->state == ABILITY_STATE_LIST) {
        menuList_update(&s_menuAbility.lists[s_menuAbility.tab]);
    }
    if (menu->targetWin.anim != 0) {
        tween_step(&menu->targetWin);
    }
    if (++menu->animTick >= ABIL_ICON_ANIM_TICKS) {
        menu->animTick = 0;
        if (++menu->animFrame >= ABIL_ICON_ANIM_FRAMES) {
            menu->animFrame = 0;
        }
    }
    if (s_menuAbility.header.active != 0) {
        ui_updateMemberSwitch(&s_menuAbility.header);
    }
}

void menuAbility_inputTabs(void) {
    u8 slot;
    s32 next;

    if (g_padRepeat & BTN_CIRCLE) {
        snd_playSe(SE_CANCEL);
        menuScreen_open(0);
    } else if (g_padRepeat & BTN_SQUARE) {
        snd_playSe(SE_CURSOR);
        menuScreen_switch(MENU_SCREEN_EQUIP);
    } else if (g_padRepeat & PAD_LEFT) {
        snd_playSe(SE_CURSOR);
        s_menuAbility.tab = s_menuAbility.tab != 0 ? s_menuAbility.tab - 1 : ABILITY_TAB_SUPPORT;
    } else if (g_padRepeat & PAD_RIGHT) {
        snd_playSe(SE_CURSOR);
        s_menuAbility.tab = s_menuAbility.tab == 0 ? s_menuAbility.tab + 1 : 0;
    } else if (g_padRepeat & BTN_R1) {
        slot = s_menuAbility.slot;
        next = party_getNextMember(slot);
        if (slot != next) {
            menuAbility_setMember(next, 2);
            snd_playSe(SE_PAGE);
        }
    } else if (g_padRepeat & BTN_L1) {
        slot = s_menuAbility.slot;
        next = party_getPrevMember(slot);
        if (slot != next) {
            menuAbility_setMember(next, 1);
            snd_playSe(SE_PAGE);
        }
    } else if (g_padRepeat & BTN_CROSS) {
        if (s_menuAbility.tabEnabled[s_menuAbility.tab] != 0) {
            snd_playSe(SE_CURSOR);
            s_menuAbility.state = ABILITY_STATE_LIST;
        } else {
            snd_playSe(SE_BUZZER);
        }
    }
}

void menuAbility_inputTarget(void) {
    s32 cur;
    s32 next;

    if (g_padRepeat & BTN_CIRCLE) {
        snd_playSe(SE_CANCEL);
        menuAbility_closeTarget();
        return;
    }
    if (g_padRepeat & PAD_UP) {
        if (!(s_menuAbility.target & ABILITY_TARGET_ALL)) {
            cur = s_menuAbility.target;
            next = party_getPrevMember(cur);
            if (cur != next) {
                s_menuAbility.target = next;
                snd_playSe(SE_CURSOR);
            }
        }
    } else if (g_padRepeat & PAD_DOWN) {
        if (!(s_menuAbility.target & ABILITY_TARGET_ALL)) {
            cur = s_menuAbility.target;
            next = party_getNextMember(cur);
            if (cur != next) {
                s_menuAbility.target = next;
                snd_playSe(SE_CURSOR);
            }
        }
    } else if (g_padRepeat & (PAD_RIGHT | PAD_LEFT | BTN_L1 | BTN_R1)) {
        if (s_menuAbility.canTargetAll != 0) {
            snd_playSe(SE_CURSOR);
            s_menuAbility.target ^= ABILITY_TARGET_ALL;
        }
    } else if (g_padRepeat & BTN_CROSS) {
        menuAbility_useOnTarget();
    }
}

void menuAbility_useOnTarget(void) {
    u8 ok;
    MenuList *list;
    Character *chr;
    Unk801F34EC *it;
    s32 i;
    s32 cost;

    ok = 0;
    chr = g_gameState->gameData->party[s_menuAbility.slot];
    list = &s_menuAbility.lists[s_menuAbility.tab];
    it = &s_menuAbilityActionTable[s_menuAbility.actions[(list->top + list->cursor) * 2 + list->col].id];
    if (!(s_menuAbility.target >> 7)) {
        ok = item_useOnMember(chr, g_gameState->gameData->party[s_menuAbility.target], it, it->effect, 0);
    } else {
        for (i = 0; i < PARTY_SIZE; i++) {
            if (g_gameState->gameData->party[i] != NULL) {
                ok |= item_useOnMember(chr, g_gameState->gameData->party[i], it, it->effect, 1);
            }
        }
    }
    if (ok) {
        snd_playSe(SE_USE_ITEM);
        cost = menuAbility_getMpCost(it);
        chr->mp -= cost;
        if (chr->mp < cost) {
            menuAbility_closeTarget();
        }
    } else {
        snd_playSe(SE_BUZZER);
    }
}

void menuAbility_drawPlayer(void) {
    RECT box;
    RECT *rect;
    Character *chr;

    rect = &s_menuAbility.playerRect;
    chr = g_gameState->gameData->party[s_menuAbility.slot];
    ui_drawMemberHeader(rect->x + ABIL_HEADER_X, rect->y + ABIL_HEADER_Y, ABIL_OTZ_WIN, (MemberSwitch *)&s_menuAbility.header);
    box.w = ABIL_ICONBOX_W;
    box.h = ABIL_ICONBOX_H;
    box.x = rect->x + ABIL_ICONBOX_X;
    box.y = rect->y + ABIL_ICONBOX_Y;
    ui_drawStatusIcons(box.x + 3, box.y + 4, ABIL_OTZ_WIN, chr->status, 0);
    win_drawBox(&box, ABIL_OTZ_WIN, WIN_STYLE_LINE);
    if (s_menuAbility.tab == ABILITY_TAB_SUPPORT) {
        ui_drawStones(box.x + ABIL_STONES_DX, box.y + ABIL_STONES_DY, ABIL_OTZ_WIN, chr->stones, chr->maxStones);
    }
    ui_drawLabel(rect->x + WIN_TITLE_X, rect->y + WIN_TITLE_Y, ABIL_OTZ_WIN, g_labelPlayer, UI_LABEL_NORMAL);
    win_drawBox(rect, ABIL_OTZ_WIN, WIN_STYLE_LINE_RULE);
}

void menuAbility_drawInfo(void) {
    RECT *rect;
    MenuList *list;
    Character *chr;
    s32 idx;
    s32 id;
    s32 ok;
    u8 flag;
    s32 i;
    s32 x;

    rect = &s_menuAbility.infoRect;
    chr = g_gameState->gameData->party[s_menuAbility.slot];
    list = &s_menuAbility.lists[s_menuAbility.tab];
    idx = (list->top + list->cursor) * 2 + list->col;
    ui_drawLabel(rect->x + WIN_TITLE_X, rect->y + WIN_TITLE_Y, ABIL_OTZ_WIN, g_labelInfo, UI_LABEL_NORMAL);
    win_drawBox(rect, ABIL_OTZ_WIN, WIN_STYLE_BEVEL);
    text_drawBlock(rect->x + ABIL_INFO_TEXT_X, rect->y + ABIL_INFO_AP_Y, ABIL_OTZ_TEXT, text_get(ABILITY_TEXT_AP), TEXT_PAL_WHITE);
    text_drawBlock(rect->x + ABIL_INFO_TEXT_X, rect->y + ABIL_INFO_EQUIP_Y, ABIL_OTZ_TEXT, text_get(ABILITY_TEXT_EQUIP), TEXT_PAL_WHITE);
    if (s_menuAbility.state == ABILITY_STATE_TABS || s_menuAbility.state == ABILITY_STATE_SWITCH || s_menuAbility.state == ABILITY_STATE_BUSY || list->count == 0) {
        return;
    }
    if (s_menuAbility.tab == ABILITY_TAB_ACTION) {
        id = s_menuAbility.actions[idx].id;
        ok = menuAbility_getActionState(id, chr);
        flag = !(s_menuAbilityActionTable[id].flags & 2);
    } else {
        id = s_menuAbility.supports[idx];
        ok = menuAbility_getSupportState(id, chr);
        flag = 1;
    }
    if (ok == 0) {
        return;
    }
    if (chr_canLearnAbilities(chr)) {
        ui_drawApGauge(rect->x + ABIL_INFO_GAUGE_X, rect->y + ABIL_INFO_GAUGE_Y, ABIL_OTZ_TEXT, chr_getAp(chr->info.b.id, id), chr_getApNeeded(chr->info.b.id, id), flag, 0);
    }
    if (s_menuAbility.equipSlots[id] != 0) {
        x = rect->x;
        for (i = 0; i < EQUIP_SLOT_COUNT; i++) {
            if ((s_menuAbility.equipSlots[id] >> i) & 1) {
                ui_drawItemIcon(x + ABIL_INFO_TEXT_X, rect->y + ABIL_INFO_ITEMS_Y, ABIL_OTZ_TEXT, s_menuAbility.equipItems[id][i], 1);
                x += ABIL_INFO_ITEM_W;
            }
        }
    }
}

void menuAbility_drawCommand(void) {
    RECT *rect;
    AbilityMenu *menu;
    MenuList *list;
    Character *chr;
    s32 idx;

    rect = &s_menuAbility.commandRect;
    menu = &s_menuAbility;
    list = &s_menuAbility.lists[s_menuAbility.tab];
    idx = (list->cursor + list->top) * 2 + list->col;
    chr = g_gameState->gameData->party[menu->slot];
    if (menu->tab != ABILITY_TAB_SUPPORT) {
        if (menu->state != ABILITY_STATE_TABS && menu->state != ABILITY_STATE_SWITCH && menu->state != ABILITY_STATE_BUSY) {
            if (menuAbility_getActionState(menu->actions[idx].id, chr) != 0) {
                text_drawBlock(rect->x + ABIL_CMD_TEXT_X, rect->y + ABIL_CMD_TEXT_Y, ABIL_OTZ_CMD, s_menuAbilityCmdNames + s_menuAbilityCmds[menu->actions[idx].cmd].name, TEXT_PAL_WHITE);
            }
        }
        ui_drawLabel(rect->x + WIN_TITLE_X, rect->y + WIN_TITLE_Y, ABIL_OTZ_CMD, g_labelCommand, UI_LABEL_NORMAL);
        win_drawBox(rect, ABIL_OTZ_CMD, WIN_STYLE_BEVEL);
    }
}

void menuAbility_drawList(void) {
    RECT sub;
    MenuView view;
    s32 row;
    s32 col;
    s32 idx;
    MenuList *list;
    RECT *rect = &s_menuAbility.listRect;

    list = &s_menuAbility.lists[s_menuAbility.tab];
    ui_drawLabel(rect->x + ABIL_LIST_LABEL_X, rect->y + WIN_TITLE_Y, ABIL_OTZ_TEXT, g_winLabelStrings.abilityList, UI_LABEL_NORMAL);
    ui_drawLabel(rect->x + (ABIL_LIST_LABEL_X + ABIL_COL_W), rect->y + WIN_TITLE_Y, ABIL_OTZ_TEXT, g_winLabelStrings.abilityList, UI_LABEL_NORMAL);
    ui_drawLabel(rect->x + ABIL_COST_LABEL_X, rect->y + WIN_TITLE_Y, ABIL_OTZ_TEXT, g_winLabelStrings.abilityCost, UI_LABEL_NORMAL);
    ui_drawLabel(rect->x + (ABIL_COST_LABEL_X + ABIL_COL_W), rect->y + WIN_TITLE_Y, ABIL_OTZ_TEXT, g_winLabelStrings.abilityCost, UI_LABEL_NORMAL);
    win_drawListBox(rect, ABIL_OTZ_WIN, 1, list);
    if (list->count != 0) {
        menuList_getView(list, &view);
        win_setClip(NULL, ABIL_OTZ_TEXT, WIN_STYLE_LINE_RULE);
        idx = view.first * 2;
        for (row = 0; row < view.rows; row++) {
            for (col = 0; col < 2; col++) {
                menuAbility_drawRow(col, row, idx++, &view);
            }
        }
        if (list->scroll & MENULIST_SCROLL_PAGE) {
            menuList_getRowsRect(list, rect, &sub);
            win_setClip(&sub, ABIL_OTZ_TEXT, WIN_STYLE_LINE_RULE);
            menuAbility_drawScrollRows();
        }
        win_setClip(rect, ABIL_OTZ_TEXT, WIN_STYLE_LINE_RULE);
    }
}

void menuAbility_drawTargetWindow(void) {
    RECT *rect = &s_menuAbility.targetWin.rect;
    AbilityMenu *menu = &s_menuAbility;
    s32 i;

    if (menu->targetWinOpen != 0) {
        ui_drawLabel(rect->x + ABIL_TARGET_LABEL_X, rect->y + WIN_TITLE_Y, ABIL_OTZ_TARGET, g_labelTarget, UI_LABEL_NORMAL);
        win_drawBox(rect, ABIL_OTZ_TARGET, WIN_STYLE_BEVEL);
        for (i = 0; i < PARTY_SIZE; i++) {
            if (g_gameState->gameData->party[i] != NULL) {
                ui_drawMemberSummary(rect->x + ABIL_TARGET_ROW_X, rect->y + ABIL_TARGET_ROW_Y + i * ABIL_TARGET_ROW_H, ABIL_OTZ_TARGET_TEXT, i, s_menuAbility.targetMode);
            }
        }
    }
}

void menuAbility_drawCursor(void) {
    RECT *rect;
    MenuList *list;
    HelpState *h3;
    s32 i;
    s32 y;
    u16 k;
    u8 blink;
    u8 done;
    s16 ry;

    done = 0;
    blink = (g_menuScreenFrame & 3) != 0;
    help_setCursorOnTop(s_menuAbility.state != ABILITY_STATE_TABS);
    if (fade_isBusy()) {
        return;
    }
    if (s_menuAbility.state == ABILITY_STATE_TABS || s_menuAbility.state == ABILITY_STATE_SWITCH || blink) {
        HelpState *h1;
        rect = &s_menuAbility.tabRect;
        h1 = &g_menuHelp;
        k = s_menuAbilityTabCursorX[s_menuAbility.tab][0] - ABIL_TAB_CURSOR_LEFT;
        g_menuHelp.x = rect->x + k;
        ry = rect->y + ABIL_TAB_CURSOR_DY;
        h1->otz = ABIL_OTZ_CURSOR;
        h1->y = ry;
        help_drawCursor();
    }
    if (s_menuAbility.state == ABILITY_STATE_LIST || (s_menuAbility.state == ABILITY_STATE_TARGET && blink)) {
        HelpState *h2;
        list = &s_menuAbility.lists[s_menuAbility.tab];
        h2 = &g_menuHelp;
        k = list->col;
        k ^= (list->scroll >> 3) & 1;
        k = k * ABIL_COL_W - ABIL_LIST_CURSOR_LEFT;
        g_menuHelp.x = s_menuAbility.listRect.x + k;
        k = list->cursor * ABIL_ROW_H + ABIL_LIST_CURSOR_DY;
        h2->otz = ABIL_OTZ_CURSOR;
        h2->y = s_menuAbility.listRect.y + k;
        help_drawCursor();
    }
    if (s_menuAbility.state == ABILITY_STATE_TARGET) {
        rect = &s_menuAbility.targetWin.rect;
        if (s_menuAbility.target & ABILITY_TARGET_ALL) {
            g_menuHelp.otz = ABIL_OTZ_TARGET_CURSOR;
            if (blink) {
                for (i = 0; i < PARTY_SIZE; i++) {
                    y = i * ABIL_TARGET_ROW_H + ABIL_TARGET_CURSOR_Y;
                    if (g_gameState->gameData->party[i] != NULL) {
                        ui_drawHand(rect->x + ABIL_TARGET_CURSOR_X, rect->y + y, g_menuHelp.otz);
                        if (!done) {
                            g_menuHelp.x = rect->x + ABIL_TARGET_CURSOR_X;
                            done = 1;
                            g_menuHelp.y = rect->y + y;
                        }
                    }
                }
            }
        } else {
            g_menuHelp.x = s_menuAbility.targetWin.rect.x + ABIL_TARGET_CURSOR_X;
            h3 = &g_menuHelp;
            k = s_menuAbility.target * ABIL_TARGET_ROW_H + ABIL_TARGET_CURSOR_Y;
            ry = rect->y;
            h3->otz = ABIL_OTZ_TARGET_CURSOR;
            h3->y = ry + k;
            help_drawCursor();
        }
    }
}

void menuAbility_setMember(s32 slot, s32 dir) {
    AbilityMenu *menu;
    AbilityMenu *m2;
    MemberSwitch *memberSw;
    Character *chr;
    AbilityEntry *abil;
    ItemInfo *it;
    s32 j;
    s32 nItems;
    s32 nAbils;
    s32 id;

    memberSw = &s_menuAbility.header;
    chr = g_gameState->gameData->party[slot];
    menu = &s_menuAbility;
    nItems = 0;
    nAbils = 0;
    ui_startMemberSwitch(memberSw, menu->slot, slot, dir);
    if (dir != 0) {
        menu->state = ABILITY_STATE_SWITCH;
    }
    menu->slot = slot;
    if (chr->info.b.row < CHR_ROW_EXTRA) {
        m2 = menu;
        slot = 0;
        abil = g_abilityRows[chr->info.b.row];
        for (; slot < ABILITY_MAX; slot++) {
            if (abil->id != 0) {
                if (abil->id < ABILITY_SUPPORT_FIRST) {
                    s_menuAbility.actions[nItems].id = abil->id;
                    s_menuAbility.actions[nItems].cmd = menuAbility_findCommand(abil->id, chr);
                    nItems++;
                } else {
                    *(nAbils + m2->supports) = abil->id;
                    nAbils++;
                }
            }
            abil++;
        }
    }
    menuList_init(&s_menuAbility.lists[0], nItems, ABIL_LIST_ROWS, ABIL_LIST_COLS, ABIL_LIST_WIN_H);
    menuList_init(&s_menuAbility.lists[1], nAbils, ABIL_LIST_ROWS, ABIL_LIST_COLS, ABIL_LIST_WIN_H);
    s_menuAbility.actionCount = nItems;
    s_menuAbility.supportCount = nAbils;
    memset(s_menuAbility.equipSlots, 0, sizeof(s_menuAbility.equipSlots));
    for (slot = 0; slot < EQUIP_SLOT_COUNT; slot++) {
        id = chr->equip[slot];
        if (id != ITEM_NONE) {
            it = &g_itemTable[id];
            for (j = 0; j < (s32)ARRAY_COUNT(it->abilities); j++) {
                s_menuAbility.equipSlots[it->abilities[j]] |= 1 << slot;
                s_menuAbility.equipItems[it->abilities[j]][slot] = id;
            }
        }
    }
    s_menuAbility.tabEnabled[0] = chr->hp != 0 && !(chr->status & (CHR_STATUS_PETRIFY | CHR_STATUS_SILENCE)) && nItems != 0 && g_evtGlobalVars.b[EVT_GB_NO_ACTION_ABILITIES] == 0;
    s_menuAbility.tabEnabled[1] = chr_canLearnAbilities(chr);
    menuAbility_initActionCursor();
    menuAbility_initSupportCursor();
}

s32 menuAbility_getActionState(s32 id, Character *chr) {
    Unk801F34EC *info;
    s32 slot;
    s32 ap;
    s32 need;

    info = &s_menuAbilityActionTable[id];
    if (s_menuAbility.equipSlots[id] == 0) {
        slot = chr_findAbilitySlot(chr->info.b.id, id);
        if (slot < 0) {
            return 0;
        }
        if (chr_canLearnAbilities(chr)) {
            ap = chr->ap[slot];
            need = g_abilityRows[chr->info.b.row][slot].ap;
            if (ap == 0) {
                return 0;
            }
            if (ap < need) {
                return 0;
            }
        }
    }
    if (chr->status & (CHR_STATUS_PETRIFY | CHR_STATUS_SILENCE)) {
        return ABILITY_GREYED;
    }
    if (!(info->flags & ABILITY_ACT_MENU)) {
        return ABILITY_GREYED;
    }
    if (menuAbility_getMpCost(info) > chr->mp) {
        return ABILITY_GREYED;
    }
    return ABILITY_USABLE;
}

void menuAbility_selectAction(void) {
    MenuList *list = s_menuAbility.lists + s_menuAbility.tab;
    MenuList *ls;
    Character *chr = g_gameState->gameData->party[s_menuAbility.slot];
    s32 i;
    u8 id;

    if (list->rows != 0) {
        i = (list->top + list->cursor) * 2 + list->col;
        if (i < s_menuAbility.actionCount) {
            id = s_menuAbility.actions[i].id;
            if (menuAbility_getActionState(id, chr) == ABILITY_USABLE) {
                s_menuAbility.targetMode = s_menuAbilityActionTable[id].targetMode;
                s_menuAbility.canTargetAll = menuAbility_canTargetAll(id);
                ls = s_menuAbility.lists + s_menuAbility.tab;
                s_menuAbility.targetWin.rect = s_menuAbility.targetRect;
                if (ls->col == 0) {
                    s_menuAbility.targetWin.rect.x = ABIL_TARGET_WIN_X_RIGHT;
                    tween_start(&s_menuAbility.targetWin, &s_menuAbilityTargetOpenL, 1);
                } else {
                    s_menuAbility.targetWin.rect.x = ABIL_TARGET_WIN_X_LEFT;
                    tween_start(&s_menuAbility.targetWin, &s_menuAbilityTargetOpenR, 1);
                }
                s_menuAbility.targetWinOpen = 1;
                s_menuAbility.target = party_getFirstMember();
                s_menuAbility.state = ABILITY_STATE_WAIT;
                snd_playSe(SE_CURSOR);
            } else {
                snd_playSe(SE_BUZZER);
            }
        }
    }
}

void menuAbility_toggleSupport(void) {
    MenuList *list;
    Character *chr;
    s32 idx;
    s32 id;
    u8 ap;
    Unk8006D128 *sup;
    s32 st;

    list = &s_menuAbility.lists[s_menuAbility.tab];
    chr = g_gameState->gameData->party[s_menuAbility.slot];
    if (list->rows != 0 && (idx = (list->top + list->cursor) * 2 + list->col) < s_menuAbility.supportCount) {
        id = s_menuAbility.supports[idx];
        sup = &g_supportAbilities[id - ABILITY_SUPPORT_FIRST];
        st = menuAbility_getSupportState_inline(id, chr);
        if (st >= ABILITY_USABLE) {
            chr_setSupportAbility(chr->info.b.id, id, st == ABILITY_USABLE);
            ap = chr->stones;
            if (st == ABILITY_USABLE) {
                chr->stones = ap - sup->stones;
            } else {
                chr->stones = ap + sup->stones;
            }
            chr_updateStats(chr);
            snd_playSe(SE_SET);
            return;
        }
    }
    snd_playSe(SE_BUZZER);
}

void menuAbility_updateHelp(void) {
    Character *chr;
    MenuList *list;
    RECT bounds;
    s32 i;
    s32 id;
    s32 reason;
    u8 *help;

    list = &s_menuAbility.lists[s_menuAbility.tab];
    chr = g_gameState->gameData->party[s_menuAbility.slot];
    g_menuHelp.text = NULL;
    help_setBounds(NULL);
    if (s_menuAbility.state == ABILITY_STATE_TABS || s_menuAbility.state == ABILITY_STATE_SWITCH) {
        help = s_menuAbilityHelpBuf;
        g_menuHelp.text = help;
        text_setStringArg(0, s_menuAbilityHelpArg);
        text_copy(help, text_get(s_menuAbility.tab + ABILITY_TEXT_TAB_HELP));
        if (s_menuAbility.tab == ABILITY_TAB_ACTION) {
            if (g_evtGlobalVars.b[EVT_GB_NO_ACTION_ABILITIES]) {
                reason = ABILITY_TEXT_NOT_HERE;
            } else if (!s_menuAbility.actionCount) {
                reason = ABILITY_TEXT_CANNOT_USE;
            } else {
                reason = ABILITY_TEXT_STATUS_BLOCKS;
            }
        } else {
            if (chr_canLearnAbilities(chr)) {
                goto end;
            }
            if (chr->classFlags & CHR_CLASS_GUEST) {
                reason = ABILITY_TEXT_CANNOT_EQUIP;
            } else {
                reason = ABILITY_TEXT_CANNOT_EQUIP_NOW;
            }
        }
        text_append(help, text_get(reason));
    end:
        text_append(s_menuAbilityHelpBuf, text_get(ABILITY_TEXT_TO_EQUIP));
    } else if (s_menuAbility.state == ABILITY_STATE_LIST) {
        if (list->count == 0) {
            return;
        }
        bounds.x = 0;
        bounds.y = ABIL_LIST_WIN_Y;
        bounds.w = g_gameState->dispEnv->disp.w;
        bounds.h = g_gameState->dispEnv->disp.h - ABIL_LIST_WIN_Y;
        help_setBounds(&bounds);
        i = menuAbility_getSlot(list);
        if (s_menuAbility.tab == ABILITY_TAB_ACTION) {
            id = s_menuAbility.actions[i].id;
            if (menuAbility_getActionState(id, chr)) {
                g_menuHelp.text = s_menuAbilityActionHelp + s_menuAbilityActionHelpOfs[id].helpOffset;
            }
        } else {
            id = s_menuAbility.supports[i];
            if (menuAbility_getSupportState_inline(id, chr)) {
                u8 **helpTextPtr = &g_menuHelpText;
                *helpTextPtr = menuAbility_updateHelp_suphelp(id);
            }
        }
    } else if (s_menuAbility.state == ABILITY_STATE_TARGET) {
        if (menuAbility_getSlot(list) & 1) {
            bounds.x = MENU_HELP_MARGIN_X;
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
        g_menuHelpText = text_get(s_menuAbility.canTargetAll == 0 ? ABILITY_TEXT_SELECT_CHAR : ABILITY_TEXT_SELECT_CHAR_ALL);
    }
}

void menuAbility_initSupportCursor(void) {
    s32 i;
    s32 sel;
    s32 row;
    Character *chr;
    MenuList *list;

    sel = 0;
    i = 0;
    chr = g_gameState->gameData->party[s_menuAbility.slot];
    list = &s_menuAbility.lists[1];
    for (; i < s_menuAbility.supportCount; i++) {
        if (menuAbility_getSupportState_inline(s_menuAbility.supports[i], chr) >= ABILITY_USABLE) {
            sel = i;
            break;
        }
    }
    row = sel / 2;
    list->col = sel % 2;
    if (row - (ABIL_LIST_ROWS - 1) >= 0) {
        list->top = row - (ABIL_LIST_ROWS - 1);
    } else {
        list->top = 0;
    }
    list->cursor = row - list->top;
}

void menuAbility_update(void) {
    menuAbility_input();
    menuAbility_draw();
    menuAbility_updateAnims();
}

void menuAbility_exit(void) {
    g_menuScreenMember = s_menuAbility.slot;
    help_setCursorOnTop(1);
    help_setCallback(NULL);
}

void menuAbility_input(void) {
    void (*fn)(void);

    if (!fade_isBusy()) {
        fn = s_menuAbilityInput[s_menuAbility.state];
        if (fn != NULL) {
            fn();
        }
    }
}

void menuAbility_draw(void) {
    menuAbility_drawTabs();
    menuAbility_drawPlayer();
    menuAbility_drawInfo();
    menuAbility_drawCommand();
    menuAbility_drawList();
    menuAbility_drawTargetWindow();
    menuAbility_drawCursor();
}

void menuAbility_inputList(void) {
    MenuList *list;

    list = &s_menuAbility.lists[s_menuAbility.tab];
    if (list->scroll == 0) {
        if (g_padRepeat & BTN_CIRCLE) {
            snd_playSe(SE_CANCEL);
            s_menuAbility.state = ABILITY_STATE_TABS;
            return;
        }
        if (g_padRepeat & BTN_CROSS) {
            if (s_menuAbility.tab == ABILITY_TAB_ACTION) {
                menuAbility_selectAction();
            } else {
                menuAbility_toggleSupport();
            }
            return;
        }
        menuList_input(list, g_padRepeat);
        menuList_left(list, g_padRepeat);
        menuList_right(list, g_padRepeat);
    }
}

void menuAbility_drawTabs(void) {
    RECT *rect;
    s32 i;

    rect = &s_menuAbility.tabRect;
    win_drawBox(rect, ABIL_OTZ_WIN, WIN_STYLE_BEVEL);
    for (i = 0; i < 2; i++) {
        text_drawBlock(rect->x + s_menuAbilityTabX[i], rect->y + ABIL_TAB_TEXT_Y, ABIL_OTZ_TEXT, text_get(i), s_menuAbility.tabEnabled[i] == 0 ? TEXT_PAL_GREY : TEXT_PAL_WHITE);
    }
}

void menuAbility_drawScrollRows(void) {
    MenuView view;
    s32 row;
    s32 col;
    s32 idx;

    row = 0;
    menuList_getPageView(&s_menuAbility.lists[s_menuAbility.tab], &view);
    idx = view.first * 2;
    for (; row < view.rows; row++) {
        for (col = 0; col < 2; col++) {
            menuAbility_drawRow(col, row, idx++, &view);
        }
    }
}

void menuAbility_drawRow(s32 col, s32 row, s32 z, MenuView *view) {
    Character *chr = g_gameState->gameData->party[s_menuAbility.slot];
    s32 x, y;
    s32 dx = ABIL_ROW_X;
    s32 dy = ABIL_LIST_WIN_Y;

    x = col * ABIL_COL_W + (s_menuAbility.listRect.x + dx);
    y = row * ABIL_ROW_H + (view->y + dy);
    if (s_menuAbility.tab == ABILITY_TAB_ACTION) {
        menuAbility_drawActionRow(x, y, z, chr);
    } else {
        menuAbility_drawSupportRow(x, y, z, chr);
    }
}

void menuAbility_drawActionRow(s32 x, s32 y, s32 idx, Character *chr) {
    u8 numText[8];
    u8 id;
    Unk801F34EC *info;
    s32 st;
    s32 col;

    id = s_menuAbility.actions[idx].id;
    info = &s_menuAbilityActionTable[id];
    st = menuAbility_getActionState(id, chr);
    if (st != 0) {
        col = s_menuAbilityStateColor[st];
        text_drawBlock(x + ABIL_ROW_ACTION_NAME_X, y, ABIL_OTZ_TEXT, s_menuAbilityActionNames + info->nameOffset, col);
        if (info->mpCost != 0) {
            text_formatNumber(menuAbility_getMpCost(info), numText, 3, TEXT_NUM_ASCII);
            ui_drawSmallDigits(x + ABIL_ROW_COST_X, y + ABIL_ROW_COST_DY, ABIL_OTZ_TEXT, numText, col);
        }
    }
}

void menuAbility_drawSupportRow(s32 x, s32 y, s32 i, Character *chr) {
    u8 numText[0x10];
    Unk8006D128 *sup;
    s32 id;
    u32 st;
    s32 col;

    id = s_menuAbility.supports[i];
    sup = &g_supportAbilities[id - ABILITY_SUPPORT_FIRST];
    st = menuAbility_getSupportState(id, chr);
    if (st != 0) {
        col = s_menuAbilityStateColor[st];
        if (st == ABILITY_EQUIPPED) {
            menuAbility_drawIcon(x + ABIL_ROW_ICON_X, y, ABIL_OTZ_TEXT, s_menuAbility.animFrame);
        }
        if (!(CONFIG_WINDOW_COLOR())) {
            menuAbility_drawIcon(x + ABIL_ROW_ICON_X, y, ABIL_OTZ_TEXT, st >= ABILITY_USABLE ? ABIL_ICON_SUPPORT : ABIL_ICON_SUPPORT_NA);
        } else {
            menuAbility_drawIcon(x + ABIL_ROW_ICON_X, y, ABIL_OTZ_TEXT, st >= ABILITY_USABLE ? ABIL_ICON_SUPPORT_CLASSIC : ABIL_ICON_SUPPORT_CLASSIC_NA);
        }
        text_drawBlock(x + ABIL_ROW_SUPPORT_NAME_X, y, ABIL_OTZ_TEXT, s_menuAbilitySupportNames + sup->name, col);
        text_formatNumber(sup->stones, numText, 3, TEXT_NUM_ASCII);
        ui_drawSmallDigits(x + ABIL_ROW_COST_X, y + ABIL_ROW_COST_DY, ABIL_OTZ_TEXT, numText, col);
    }
}

u8 menuAbility_findCommand(s32 id, Character *chr) {
    s32 i;
    s32 j;
    u8 k;
    MenuCmd *cmd;

    for (i = 0; i < 2; i++) {
        k = s_menuAbilityCharCmds[chr->info.b.row][i];
        cmd = &s_menuAbilityCmds[k];
        if (cmd->kind == MENU_CMD_ABILITIES) {
            for (j = 0; j < cmd->count; j++) {
                if (id == s_menuAbilityCmdAbilities[cmd->first + j]) {
                    return k;
                }
            }
        }
    }
    return 0;
}

/* Same logic as menuAbility_getSupportState_inline (above). Written out because a call of the inline compiles
   differently. */
u32 menuAbility_getSupportState(s32 id, Character *chr) {
    Unk8006D128 *sup;
    s32 slot;
    s32 ap;
    s32 need;

    if (id < ABILITY_SUPPORT_FIRST) {
        return 0;
    }
    if (chr_testSupportBit(chr->supportAbilities, id)) {
        return ABILITY_EQUIPPED;
    }
    if (s_menuAbility.equipSlots[id] == 0) {
        slot = chr_findAbilitySlot(chr->info.b.id, id);
        if (slot < 0) {
            return 0;
        }
        if (chr_canLearnAbilities(chr)) {
            ap = chr->ap[slot];
            need = g_abilityRows[chr->info.b.row][slot].ap;
            if (ap == 0) {
                return 0;
            }
            if (ap < need) {
                return 0;
            }
        }
    }
    sup = &g_supportAbilities[id - ABILITY_SUPPORT_FIRST];
    if (sup->stones > chr->stones) {
        return ABILITY_GREYED;
    }
    return ABILITY_USABLE;
}

void menuAbility_closeTarget(void) {
    MenuList *list = &s_menuAbility.lists[s_menuAbility.tab];

    tween_start(&s_menuAbility.targetWin, list->col == 0 ? &s_menuAbilityTargetCloseR : &s_menuAbilityTargetCloseL, 0);
    s_menuAbility.state = ABILITY_STATE_WAIT;
}

void menuAbility_onTargetOpened(void) {
    s_menuAbility.state = ABILITY_STATE_TARGET;
}

void menuAbility_onTargetClosed(void) {
    s_menuAbility.state = ABILITY_STATE_LIST;
    s_menuAbility.targetWinOpen = 0;
}

s32 menuAbility_canTargetAll(s32 id) {
    s32 kind = s_menuAbilityActionTable[id].targetKind;
    s32 memberCount = 0;
    s32 i;

    for (i = 0; i < PARTY_SIZE; i++) {
        if (g_gameState->gameData->party[i] != NULL) {
            memberCount++;
        }
    }
    if (memberCount < 2) {
        return 0;
    }
    return (u32)(kind - 3) < 3;
}

void menuAbility_onMemberSwitched(void) {
    s_menuAbility.state = ABILITY_STATE_TABS;
}

void menuAbility_loadImages(void) {
    u8 *img = s_menuAbilityImage;

    LoadImage(&s_menuAbilityClutRect, (u32 *)img);
    DrawSync(0);
    LoadImage(&s_menuAbilityImageRect, (u32 *)(img + 0x120));
    DrawSync(0);
}

void menuAbility_drawIcon(s32 x, s32 y, s32 otz, s32 i) {
    ui_drawSpriteDef(x, y, otz, menuAbility_getIcon(i), 0);
}

Icon *menuAbility_getIcon(s32 i) {
    return &s_menuAbilityIcons[i];
}

s32 menuAbility_getSlot(MenuList *list) {
    s32 cursorRow, otherCol;
    s32 step;

    if (list->scroll & MENULIST_SCROLL_WRAP_COL) {
        otherCol = list->col ^ 1;
        step = 1;
        cursorRow = list->cursor;
        if (list->scroll & MENULIST_SCROLL_UP) {
            return (list->top + (cursorRow - step)) * 2 + otherCol;
        }
        return (list->top + (cursorRow + step)) * 2 + otherCol;
    }
    return (list->top + list->cursor) * 2 + list->col;
}

s32 menuAbility_getMpCost(Unk801F34EC *info) {
    s32 cost;

    cost = info->mpCost;
    if ((info->flags & ABILITY_ACT_MP_X4) && g_evtGlobalVars.b[EVT_GB_SUMMON_MP_X4] != 0) {
        cost *= 4;
    }
    return cost;
}

void menuAbility_initActionCursor(void) {
    Character *chr;
    MenuList *list;
    s32 i;
    s32 sel;
    s32 row;
    s32 top;

    sel = 0;
    i = 0;
    chr = g_gameState->gameData->party[s_menuAbility.slot];
    list = &s_menuAbility.lists[0];
    for (; i < s_menuAbility.actionCount; i++) {
        if (menuAbility_getActionState(s_menuAbility.actions[i].id, chr) == ABILITY_USABLE) {
            sel = i;
            break;
        }
    }
    row = sel / 2;
    list->col = sel % 2;
    top = row - (ABIL_LIST_ROWS - 1);
    if (top >= 0) {
        list->top = top;
    } else {
        list->top = 0;
    }
    list->cursor = row - list->top;
}

void menuAbility_onInit(void) {
}

INCLUDE_RODATA("asm/us/ovl_33/nonmatchings/menu_ability", g_abilityMenuText);

INCLUDE_RODATA("asm/us/ovl_33/nonmatchings/menu_ability", s_menuAbilityImage);
