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
    setRECT(&s_menuAbility.tabRect, 8, 8, 0xE4, 0x1A);
    setRECT(&s_menuAbility.playerRect, 8, 0x26, 0xD0, 0x38);
    setRECT(&s_menuAbility.infoRect, 0xD8, 0x26, 0x5C, 0x38);
    setRECT(&s_menuAbility.commandRect, 0x8E, 0x40, 0x46, 0x1A);
    setRECT(&s_menuAbility.listRect, 8, 0x5E, 0x12C, 0x80);
    setRECT(&s_menuAbility.targetRect, 0, 0x26, 0x90, 0xB6);
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
    if (++menu->animTick >= 7) {
        menu->animTick = 0;
        if (++menu->animFrame >= 7) {
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
    ui_drawMemberHeader(rect->x + 8, rect->y + 5, 0x1D, (MemberSwitch *)&s_menuAbility.header);
    box.w = 0x5B;
    box.h = 0x12;
    box.x = rect->x + 0x72;
    box.y = rect->y + 4;
    ui_drawStatusIcons(box.x + 3, box.y + 4, 0x1D, chr->status, 0);
    win_drawBox(&box, 0x1D, WIN_STYLE_LINE);
    if (s_menuAbility.tab == ABILITY_TAB_SUPPORT) {
        ui_drawStones(box.x + 0x19, box.y + 0x14, 0x1D, chr->stones, chr->maxStones);
    }
    ui_drawLabel(rect->x + 6, rect->y - 3, 0x1D, g_labelPlayer, UI_LABEL_NORMAL);
    win_drawBox(rect, 0x1D, WIN_STYLE_LINE_RULE);
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
    ui_drawLabel(rect->x + 6, rect->y - 3, 0x1D, g_labelInfo, UI_LABEL_NORMAL);
    win_drawBox(rect, 0x1D, WIN_STYLE_BEVEL);
    text_drawBlock(rect->x + 8, rect->y + 8, 0x1C, text_get(ABILITY_TEXT_AP), TEXT_PAL_WHITE);
    text_drawBlock(rect->x + 8, rect->y + 0x16, 0x1C, text_get(ABILITY_TEXT_EQUIP), TEXT_PAL_WHITE);
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
        ui_drawApGauge(rect->x + 0x2D, rect->y + 0xC, 0x1C, chr_getAp(chr->info.b.id, id), chr_getApNeeded(chr->info.b.id, id), flag, 0);
    }
    if (s_menuAbility.equipSlots[id] != 0) {
        x = rect->x;
        for (i = 0; i < 5; i++) {
            if ((s_menuAbility.equipSlots[id] >> i) & 1) {
                ui_drawItemIcon(x + 8, rect->y + 0x24, 0x1C, s_menuAbility.equipItems[id][i], 1);
                x += 0x10;
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
                text_drawBlock(rect->x + 8, rect->y + 6, 0x1B, s_menuAbilityCmdNames + s_menuAbilityCmds[menu->actions[idx].cmd].name, TEXT_PAL_WHITE);
            }
        }
        ui_drawLabel(rect->x + 6, rect->y - 3, 0x1B, g_labelCommand, UI_LABEL_NORMAL);
        win_drawBox(rect, 0x1B, WIN_STYLE_BEVEL);
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
    ui_drawLabel(rect->x + 0xE, rect->y - 3, 0x1C, g_winLabelStrings.abilityList, UI_LABEL_NORMAL);
    ui_drawLabel(rect->x + 0xA4, rect->y - 3, 0x1C, g_winLabelStrings.abilityList, UI_LABEL_NORMAL);
    ui_drawLabel(rect->x + 0x7C, rect->y - 3, 0x1C, g_winLabelStrings.abilityCost, UI_LABEL_NORMAL);
    ui_drawLabel(rect->x + 0x112, rect->y - 3, 0x1C, g_winLabelStrings.abilityCost, UI_LABEL_NORMAL);
    win_drawListBox(rect, 0x1D, 1, list);
    if (list->count != 0) {
        menuList_getView(list, &view);
        win_setClip(NULL, 0x1C, WIN_STYLE_LINE_RULE);
        idx = view.first * 2;
        for (row = 0; row < view.rows; row++) {
            for (col = 0; col < 2; col++) {
                menuAbility_drawRow(col, row, idx++, &view);
            }
        }
        if (list->scroll & MENULIST_SCROLL_PAGE) {
            menuList_getRowsRect(list, rect, &sub);
            win_setClip(&sub, 0x1C, WIN_STYLE_LINE_RULE);
            menuAbility_drawScrollRows();
        }
        win_setClip(rect, 0x1C, WIN_STYLE_LINE_RULE);
    }
}

void menuAbility_drawTargetWindow(void) {
    RECT *rect = &s_menuAbility.targetWin.rect;
    AbilityMenu *menu = &s_menuAbility;
    s32 i;

    if (menu->targetWinOpen != 0) {
        ui_drawLabel(rect->x + 8, rect->y - 3, 0x14, g_labelTarget, UI_LABEL_NORMAL);
        win_drawBox(rect, 0x14, WIN_STYLE_BEVEL);
        for (i = 0; i < PARTY_SIZE; i++) {
            if (g_gameState->gameData->party[i] != NULL) {
                ui_drawMemberSummary(rect->x + 8, rect->y + 6 + i * 0x2B, 0x13, i, s_menuAbility.targetMode);
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
        k = s_menuAbilityTabCursorX[s_menuAbility.tab][0] - 0x18;
        g_menuHelp.x = rect->x + k;
        ry = rect->y + 8;
        h1->otz = 0x17;
        h1->y = ry;
        help_drawCursor();
    }
    if (s_menuAbility.state == ABILITY_STATE_LIST || (s_menuAbility.state == ABILITY_STATE_TARGET && blink)) {
        HelpState *h2;
        list = &s_menuAbility.lists[s_menuAbility.tab];
        h2 = &g_menuHelp;
        k = list->col;
        k ^= (list->scroll >> 3) & 1;
        k = k * 150 - 10;
        g_menuHelp.x = s_menuAbility.listRect.x + k;
        k = list->cursor * 15 + 10;
        h2->otz = 0x17;
        h2->y = s_menuAbility.listRect.y + k;
        help_drawCursor();
    }
    if (s_menuAbility.state == ABILITY_STATE_TARGET) {
        rect = &s_menuAbility.targetWin.rect;
        if (s_menuAbility.target & ABILITY_TARGET_ALL) {
            g_menuHelp.otz = 0x12;
            if (blink) {
                for (i = 0; i < PARTY_SIZE; i++) {
                    y = i * 0x2B + 0x12;
                    if (g_gameState->gameData->party[i] != NULL) {
                        ui_drawHand(rect->x - 10, rect->y + y, g_menuHelp.otz);
                        if (!done) {
                            g_menuHelp.x = rect->x - 10;
                            done = 1;
                            g_menuHelp.y = rect->y + y;
                        }
                    }
                }
            }
        } else {
            g_menuHelp.x = s_menuAbility.targetWin.rect.x - 10;
            h3 = &g_menuHelp;
            k = s_menuAbility.target * 0x2B + 0x12;
            ry = rect->y;
            h3->otz = 0x12;
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
    menuList_init(&s_menuAbility.lists[0], nItems, 8, 2, 0x80);
    menuList_init(&s_menuAbility.lists[1], nAbils, 8, 2, 0x80);
    s_menuAbility.actionCount = nItems;
    s_menuAbility.supportCount = nAbils;
    memset(s_menuAbility.equipSlots, 0, 0x100);
    for (slot = 0; slot < EQUIP_SLOT_COUNT; slot++) {
        id = chr->equip[slot];
        if (id != ITEM_NONE) {
            it = &g_itemTable[id];
            for (j = 0; j < 3; j++) {
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
                    s_menuAbility.targetWin.rect.x = 0xA4;
                    tween_start(&s_menuAbility.targetWin, &s_menuAbilityTargetOpenL, 1);
                } else {
                    s_menuAbility.targetWin.rect.x = 8;
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
        bounds.y = 0x5E;
        bounds.w = g_gameState->dispEnv->disp.w;
        bounds.h = g_gameState->dispEnv->disp.h - 0x5E;
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
        if (menuAbility_getSupportState_inline(s_menuAbility.supports[i], chr) >= 3) {
            sel = i;
            break;
        }
    }
    row = sel / 2;
    list->col = sel % 2;
    if (row - 7 >= 0) {
        list->top = row - 7;
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
    win_drawBox(rect, 0x1D, WIN_STYLE_BEVEL);
    for (i = 0; i < 2; i++) {
        text_drawBlock(rect->x + s_menuAbilityTabX[i], rect->y + 6, 0x1C, text_get(i), s_menuAbility.tabEnabled[i] == 0 ? TEXT_PAL_GREY : TEXT_PAL_WHITE);
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
    s32 dx = 8;
    s32 dy = 0x5E;

    x = col * 0x96 + (s_menuAbility.listRect.x + dx);
    y = row * 0xF + (view->y + dy);
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
        text_drawBlock(x + 8, y, 0x1C, s_menuAbilityActionNames + info->nameOffset, col);
        if (info->mpCost != 0) {
            text_formatNumber(menuAbility_getMpCost(info), numText, 3, TEXT_NUM_ASCII);
            ui_drawSmallDigits(x + 0x6C, y + 4, 0x1C, numText, col);
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
            menuAbility_drawIcon(x + 8, y, 0x1C, s_menuAbility.animFrame);
        }
        if (!(CONFIG_WINDOW_COLOR())) {
            menuAbility_drawIcon(x + 8, y, 0x1C, st >= ABILITY_USABLE ? 9 : 10);
        } else {
            menuAbility_drawIcon(x + 8, y, 0x1C, st >= ABILITY_USABLE ? 7 : 8);
        }
        text_drawBlock(x + 0x18, y, 0x1C, s_menuAbilitySupportNames + sup->name, col);
        text_formatNumber(sup->stones, numText, 3, TEXT_NUM_ASCII);
        ui_drawSmallDigits(x + 0x6C, y + 4, 0x1C, numText, col);
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
    top = row - 7;
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
