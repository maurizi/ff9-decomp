#include "ovl_33.h"

/* Inlined copy of menuAbility_getSupportState (support ability state). */
static inline s32 menuAbility_getSupportState_inline(s32 id, Character *c) {
    Unk8006D128 *sup;
    s32 slot;
    s32 ap;
    s32 need;

    if (id < ABILITY_SUPPORT_FIRST) {
        return 0;
    }
    if (chr_testSupportBit(c->supportAbilities, id)) {
        return ABILITY_EQUIPPED;
    }
    if (s_menuAbility.equipSlots[id] == 0) {
        slot = chr_findAbilitySlot(c->info.b.id, id);
        if (slot < 0) {
            return 0;
        }
        if (func_80034A30(c)) {
            ap = c->ap[slot];
            need = g_abilityRows[c->info.b.row][slot].unk1;
            if (ap == 0) {
                return 0;
            }
            if (ap < need) {
                return 0;
            }
        }
    }
    sup = &g_supportAbilities[id - ABILITY_SUPPORT_FIRST];
    if (sup->unk1 > c->stones) {
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
    func_801F1148();
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
    for (i = 0; i < 4; i++) {
        if (g_gameState->gameData->party[i] != NULL) {
            s_menuAbility.partySize++;
        }
    }
}

void menuAbility_updateAnims(void) {
    AbilityMenu *m;

    if (help_isOpen()) {
        menuAbility_updateHelp();
    }
    m = &s_menuAbility;
    if (m->state == ABILITY_STATE_LIST) {
        menuList_update(&s_menuAbility.lists[s_menuAbility.tab]);
    }
    if (m->targetWin.anim != 0) {
        tween_step(&m->targetWin);
    }
    if (++m->animTick >= 7) {
        m->animTick = 0;
        if (++m->animFrame >= 7) {
            m->animFrame = 0;
        }
    }
    if (s_menuAbility.header.unk0 != 0) {
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
    MenuList *l;
    Character *c;
    Unk801F34EC *it;
    s32 i;
    s32 cost;

    ok = 0;
    c = g_gameState->gameData->party[s_menuAbility.slot];
    l = &s_menuAbility.lists[s_menuAbility.tab];
    it = &s_menuAbilityActionTable[s_menuAbility.actions[(l->top + l->cursor) * 2 + l->col].id];
    if (!(s_menuAbility.target >> 7)) {
        ok = item_useOnMember(c, g_gameState->gameData->party[s_menuAbility.target], it, it->unk4, 0);
    } else {
        for (i = 0; i < 4; i++) {
            if (g_gameState->gameData->party[i] != NULL) {
                ok |= item_useOnMember(c, g_gameState->gameData->party[i], it, it->unk4, 1);
            }
        }
    }
    if (ok) {
        snd_playSe(SE_USE_ITEM);
        cost = menuAbility_getMpCost(it);
        c->mp -= cost;
        if (c->mp < cost) {
            menuAbility_closeTarget();
        }
    } else {
        snd_playSe(SE_BUZZER);
    }
}

void menuAbility_drawPlayer(void) {
    RECT box;
    RECT *r;
    Character *c;

    r = &s_menuAbility.playerRect;
    c = g_gameState->gameData->party[s_menuAbility.slot];
    ui_drawMemberHeader(r->x + 8, r->y + 5, 0x1D, (Unk80024428 *)&s_menuAbility.header);
    box.w = 0x5B;
    box.h = 0x12;
    box.x = r->x + 0x72;
    box.y = r->y + 4;
    ui_drawStatusIcons(box.x + 3, box.y + 4, 0x1D, c->status, 0);
    win_drawBox(&box, 0x1D, 0);
    if (s_menuAbility.tab == ABILITY_TAB_SUPPORT) {
        ui_drawStones(box.x + 0x19, box.y + 0x14, 0x1D, c->stones, c->maxStones);
    }
    ui_drawLabel(r->x + 6, r->y - 3, 0x1D, D_80070498, 0);
    win_drawBox(r, 0x1D, 1);
}

void menuAbility_drawInfo(void) {
    RECT *r;
    MenuList *l;
    Character *c;
    s32 n;
    s32 id;
    s32 ok;
    u8 flag;
    s32 i;
    s32 x;

    r = &s_menuAbility.infoRect;
    c = g_gameState->gameData->party[s_menuAbility.slot];
    l = &s_menuAbility.lists[s_menuAbility.tab];
    n = (l->top + l->cursor) * 2 + l->col;
    ui_drawLabel(r->x + 6, r->y - 3, 0x1D, D_80070460, 0);
    win_drawBox(r, 0x1D, 2);
    text_drawBlock(r->x + 8, r->y + 8, 0x1C, text_get(ABILITY_TEXT_AP), 0);
    text_drawBlock(r->x + 8, r->y + 0x16, 0x1C, text_get(ABILITY_TEXT_EQUIP), 0);
    if (s_menuAbility.state == ABILITY_STATE_TABS || s_menuAbility.state == ABILITY_STATE_SWITCH || s_menuAbility.state == ABILITY_STATE_BUSY || l->count == 0) {
        return;
    }
    if (s_menuAbility.tab == ABILITY_TAB_ACTION) {
        id = s_menuAbility.actions[n].id;
        ok = menuAbility_getActionState(id, c);
        flag = !(s_menuAbilityActionTable[id].unkB & 2);
    } else {
        id = s_menuAbility.supports[n];
        ok = menuAbility_getSupportState(id, c);
        flag = 1;
    }
    if (ok == 0) {
        return;
    }
    if (func_80034A30(c)) {
        ui_drawApGauge(r->x + 0x2D, r->y + 0xC, 0x1C, chr_getAp(c->info.b.id, id), chr_getApNeeded(c->info.b.id, id), flag, 0);
    }
    if (s_menuAbility.equipSlots[id] != 0) {
        x = r->x;
        for (i = 0; i < 5; i++) {
            if ((s_menuAbility.equipSlots[id] >> i) & 1) {
                ui_drawItemIcon(x + 8, r->y + 0x24, 0x1C, s_menuAbility.equipItems[id][i], 1);
                x += 0x10;
            }
        }
    }
}

void menuAbility_drawCommand(void) {
    RECT *r;
    AbilityMenu *m;
    MenuList *l;
    Character *c;
    s32 idx;

    r = &s_menuAbility.commandRect;
    m = &s_menuAbility;
    l = &s_menuAbility.lists[s_menuAbility.tab];
    idx = (l->cursor + l->top) * 2 + l->col;
    c = g_gameState->gameData->party[m->slot];
    if (m->tab != ABILITY_TAB_SUPPORT) {
        if (m->state != ABILITY_STATE_TABS && m->state != ABILITY_STATE_SWITCH && m->state != ABILITY_STATE_BUSY) {
            if (menuAbility_getActionState(m->actions[idx].id, c) != 0) {
                text_drawBlock(r->x + 8, r->y + 6, 0x1B, s_menuAbilityCmdNames + s_menuAbilityCmds[m->actions[idx].cmd].name, 0);
            }
        }
        ui_drawLabel(r->x + 6, r->y - 3, 0x1B, D_8007042C, 0);
        win_drawBox(r, 0x1B, 2);
    }
}

void menuAbility_drawList(void) {
    RECT sub;
    MenuView view;
    s32 row;
    s32 col;
    s32 idx;
    MenuList *l;
    RECT *r = &s_menuAbility.listRect;

    l = &s_menuAbility.lists[s_menuAbility.tab];
    ui_drawLabel(r->x + 0xE, r->y - 3, 0x1C, g_winLabelStrings.unk4, 0);
    ui_drawLabel(r->x + 0xA4, r->y - 3, 0x1C, g_winLabelStrings.unk4, 0);
    ui_drawLabel(r->x + 0x7C, r->y - 3, 0x1C, g_winLabelStrings.unk7C, 0);
    ui_drawLabel(r->x + 0x112, r->y - 3, 0x1C, g_winLabelStrings.unk7C, 0);
    win_drawListBox(r, 0x1D, 1, l);
    if (l->count != 0) {
        menuList_getView(l, &view);
        win_setClip(NULL, 0x1C, 1);
        idx = view.first * 2;
        for (row = 0; row < view.rows; row++) {
            for (col = 0; col < 2; col++) {
                menuAbility_drawRow(col, row, idx++, &view);
            }
        }
        if (l->scroll & 4) {
            menuList_getRowsRect(l, r, &sub);
            win_setClip(&sub, 0x1C, 1);
            menuAbility_drawScrollRows();
        }
        win_setClip(r, 0x1C, 1);
    }
}

void menuAbility_drawTargetWindow(void) {
    RECT *r = &s_menuAbility.targetWin.rect;
    AbilityMenu *m = &s_menuAbility;
    s32 i;

    if (m->targetWinOpen != 0) {
        ui_drawLabel(r->x + 8, r->y - 3, 0x14, D_800704B8, 0);
        win_drawBox(r, 0x14, 2);
        for (i = 0; i < 4; i++) {
            if (g_gameState->gameData->party[i] != NULL) {
                ui_drawMemberSummary(r->x + 8, r->y + 6 + i * 0x2B, 0x13, i, s_menuAbility.targetMode);
            }
        }
    }
}

void menuAbility_drawCursor(void) {
    RECT *r;
    MenuList *l;
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
        r = &s_menuAbility.tabRect;
        h1 = &g_menuHelp;
        k = s_menuAbilityTabCursorX[s_menuAbility.tab][0] - 0x18;
        g_menuHelp.x = r->x + k;
        ry = r->y + 8;
        h1->otz = 0x17;
        h1->y = ry;
        help_drawCursor();
    }
    if (s_menuAbility.state == ABILITY_STATE_LIST || (s_menuAbility.state == ABILITY_STATE_TARGET && blink)) {
        HelpState *h2;
        l = &s_menuAbility.lists[s_menuAbility.tab];
        h2 = &g_menuHelp;
        k = l->col;
        k ^= (l->scroll >> 3) & 1;
        k = k * 150 - 10;
        g_menuHelp.x = s_menuAbility.listRect.x + k;
        k = l->cursor * 15 + 10;
        h2->otz = 0x17;
        h2->y = s_menuAbility.listRect.y + k;
        help_drawCursor();
    }
    if (s_menuAbility.state == ABILITY_STATE_TARGET) {
        r = &s_menuAbility.targetWin.rect;
        if (s_menuAbility.target & ABILITY_TARGET_ALL) {
            g_menuHelp.otz = 0x12;
            if (blink) {
                for (i = 0; i < 4; i++) {
                    y = i * 0x2B + 0x12;
                    if (g_gameState->gameData->party[i] != NULL) {
                        ui_drawHand(r->x - 10, r->y + y, g_menuHelp.otz);
                        if (!done) {
                            g_menuHelp.x = r->x - 10;
                            done = 1;
                            g_menuHelp.y = r->y + y;
                        }
                    }
                }
            }
        } else {
            g_menuHelp.x = s_menuAbility.targetWin.rect.x - 10;
            h3 = &g_menuHelp;
            k = s_menuAbility.target * 0x2B + 0x12;
            ry = r->y;
            h3->otz = 0x12;
            h3->y = ry + k;
            help_drawCursor();
        }
    }
}

void menuAbility_setMember(s32 slot, s32 arg1) {
    AbilityMenu *m;
    AbilityMenu *m2;
    UnkBlink *b;
    Character *c;
    AbilityEntry *a;
    ItemInfo *it;
    s32 j;
    s32 nItems;
    s32 nAbils;
    s32 id;

    b = &s_menuAbility.header;
    c = g_gameState->gameData->party[slot];
    m = &s_menuAbility;
    nItems = 0;
    nAbils = 0;
    ui_startMemberSwitch(b, m->slot, slot, arg1);
    if (arg1 != 0) {
        m->state = ABILITY_STATE_SWITCH;
    }
    m->slot = slot;
    if (c->info.b.row < 16) {
        m2 = m;
        slot = 0;
        a = g_abilityRows[c->info.b.row];
        for (; slot < ABILITY_MAX; slot++) {
            if (a->unk0 != 0) {
                if (a->unk0 < ABILITY_SUPPORT_FIRST) {
                    s_menuAbility.actions[nItems].id = a->unk0;
                    s_menuAbility.actions[nItems].cmd = menuAbility_findCommand(a->unk0, c);
                    nItems++;
                } else {
                    *(nAbils + m2->supports) = a->unk0;
                    nAbils++;
                }
            }
            a++;
        }
    }
    menuList_init(&s_menuAbility.lists[0], nItems, 8, 2, 0x80);
    menuList_init(&s_menuAbility.lists[1], nAbils, 8, 2, 0x80);
    s_menuAbility.actionCount = nItems;
    s_menuAbility.supportCount = nAbils;
    memset(s_menuAbility.equipSlots, 0, 0x100);
    for (slot = 0; slot < 5; slot++) {
        id = c->equip[slot];
        if (id != 0xFF) {
            it = &g_itemTable[id];
            for (j = 0; j < 3; j++) {
                s_menuAbility.equipSlots[it->abilities[j]] |= 1 << slot;
                s_menuAbility.equipItems[it->abilities[j]][slot] = id;
            }
        }
    }
    s_menuAbility.tabEnabled[0] = c->hp != 0 && !(c->status & 9) && nItems != 0 && D_8007AFD3 == 0;
    s_menuAbility.tabEnabled[1] = func_80034A30(c);
    menuAbility_initActionCursor();
    menuAbility_initSupportCursor();
}

s32 menuAbility_getActionState(s32 id, Character *c) {
    Unk801F34EC *info;
    s32 slot;
    s32 ap;
    s32 need;

    info = &s_menuAbilityActionTable[id];
    if (s_menuAbility.equipSlots[id] == 0) {
        slot = chr_findAbilitySlot(c->info.b.id, id);
        if (slot < 0) {
            return 0;
        }
        if (func_80034A30(c)) {
            ap = c->ap[slot];
            need = g_abilityRows[c->info.b.row][slot].unk1;
            if (ap == 0) {
                return 0;
            }
            if (ap < need) {
                return 0;
            }
        }
    }
    if (c->status & 9) {
        return ABILITY_GREYED;
    }
    if (!(info->unkB & 1)) {
        return ABILITY_GREYED;
    }
    if (menuAbility_getMpCost(info) > c->mp) {
        return ABILITY_GREYED;
    }
    return ABILITY_USABLE;
}

void menuAbility_selectAction(void) {
    MenuList *l = s_menuAbility.lists + s_menuAbility.tab;
    MenuList *ls;
    Character *c = g_gameState->gameData->party[s_menuAbility.slot];
    s32 i;
    u8 id;

    if (l->rows != 0) {
        i = (l->top + l->cursor) * 2 + l->col;
        if (i < s_menuAbility.actionCount) {
            id = s_menuAbility.actions[i].id;
            if (menuAbility_getActionState(id, c) == ABILITY_USABLE) {
                s_menuAbility.targetMode = s_menuAbilityActionTable[id].unk0_5;
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
    MenuList *l;
    Character *c;
    s32 idx;
    s32 id;
    u8 ap;
    Unk8006D128 *sup;
    s32 st;

    l = &s_menuAbility.lists[s_menuAbility.tab];
    c = g_gameState->gameData->party[s_menuAbility.slot];
    if (l->rows != 0 && (idx = (l->top + l->cursor) * 2 + l->col) < s_menuAbility.supportCount) {
        id = s_menuAbility.supports[idx];
        sup = &g_supportAbilities[id - ABILITY_SUPPORT_FIRST];
        st = menuAbility_getSupportState_inline(id, c);
        if (st >= ABILITY_USABLE) {
            chr_setSupportAbility(c->info.b.id, id, st == ABILITY_USABLE);
            ap = c->stones;
            if (st == ABILITY_USABLE) {
                c->stones = ap - sup->unk1;
            } else {
                c->stones = ap + sup->unk1;
            }
            chr_updateStats(c);
            snd_playSe(SE_EQUIP);
            return;
        }
    }
    snd_playSe(SE_BUZZER);
}

void menuAbility_updateHelp(void) {
    Character *c;
    MenuList *l;
    RECT r;
    s32 i;
    s32 id;
    s32 t;
    u8 *buf;

    l = &s_menuAbility.lists[s_menuAbility.tab];
    c = g_gameState->gameData->party[s_menuAbility.slot];
    g_menuHelp.text = NULL;
    help_setBounds(NULL);
    if (s_menuAbility.state == ABILITY_STATE_TABS || s_menuAbility.state == ABILITY_STATE_SWITCH) {
        buf = s_menuAbilityHelpBuf;
        g_menuHelp.text = buf;
        text_setStringArg(0, s_menuAbilityHelpArg);
        text_copy(buf, text_get(s_menuAbility.tab + ABILITY_TEXT_TAB_HELP));
        if (s_menuAbility.tab == ABILITY_TAB_ACTION) {
            if (D_8007AFD3) {
                t = ABILITY_TEXT_NOT_HERE;
            } else if (!s_menuAbility.actionCount) {
                t = ABILITY_TEXT_CANNOT_USE;
            } else {
                t = ABILITY_TEXT_STATUS_BLOCKS;
            }
        } else {
            if (func_80034A30(c)) {
                goto end;
            }
            if (c->classFlags & 0x10) {
                t = ABILITY_TEXT_CANNOT_EQUIP;
            } else {
                t = ABILITY_TEXT_CANNOT_EQUIP_NOW;
            }
        }
        text_append(buf, text_get(t));
    end:
        text_append(s_menuAbilityHelpBuf, text_get(ABILITY_TEXT_TO_EQUIP));
    } else if (s_menuAbility.state == ABILITY_STATE_LIST) {
        if (l->count == 0) {
            return;
        }
        r.x = 0;
        r.y = 0x5E;
        r.w = g_gameState->dispEnv->disp.w;
        r.h = g_gameState->dispEnv->disp.h - 0x5E;
        help_setBounds(&r);
        i = menuAbility_getSlot(l);
        if (s_menuAbility.tab == ABILITY_TAB_ACTION) {
            id = s_menuAbility.actions[i].id;
            if (menuAbility_getActionState(id, c)) {
                g_menuHelp.text = s_menuAbilityActionHelp + s_menuAbilityActionHelpOfs[id].unk0;
            }
        } else {
            id = s_menuAbility.supports[i];
            if (menuAbility_getSupportState_inline(id, c)) {
                u8 **d = &g_menuHelpText;
                *d = menuAbility_updateHelp_suphelp(id);
            }
        }
    } else if (s_menuAbility.state == ABILITY_STATE_TARGET) {
        if (menuAbility_getSlot(l) & 1) {
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
        g_menuHelpText = text_get(s_menuAbility.canTargetAll == 0 ? ABILITY_TEXT_SELECT_CHAR : ABILITY_TEXT_SELECT_CHAR_ALL);
    }
}

void menuAbility_initSupportCursor(void) {
    s32 i;
    s32 sel;
    s32 row;
    Character *c;
    MenuList *l;

    sel = 0;
    i = 0;
    c = g_gameState->gameData->party[s_menuAbility.slot];
    l = &s_menuAbility.lists[1];
    for (; i < s_menuAbility.supportCount; i++) {
        if (menuAbility_getSupportState_inline(s_menuAbility.supports[i], c) >= 3) {
            sel = i;
            break;
        }
    }
    row = sel / 2;
    l->col = sel % 2;
    if (row - 7 >= 0) {
        l->top = row - 7;
    } else {
        l->top = 0;
    }
    l->cursor = row - l->top;
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
    MenuList *l;

    l = &s_menuAbility.lists[s_menuAbility.tab];
    if (l->scroll == 0) {
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
        menuList_input(l, g_padRepeat);
        menuList_left(l, g_padRepeat);
        menuList_right(l, g_padRepeat);
    }
}

void menuAbility_drawTabs(void) {
    RECT *r;
    s32 i;

    r = &s_menuAbility.tabRect;
    win_drawBox(r, 0x1D, 2);
    for (i = 0; i < 2; i++) {
        text_drawBlock(r->x + s_menuAbilityTabX[i], r->y + 6, 0x1C, text_get(i), s_menuAbility.tabEnabled[i] == 0 ? 2 : 0);
    }
}

void menuAbility_drawScrollRows(void) {
    MenuView v;
    s32 row;
    s32 col;
    s32 idx;

    row = 0;
    menuList_getPageView(&s_menuAbility.lists[s_menuAbility.tab], &v);
    idx = v.first * 2;
    for (; row < v.rows; row++) {
        for (col = 0; col < 2; col++) {
            menuAbility_drawRow(col, row, idx++, &v);
        }
    }
}

void menuAbility_drawRow(s32 col, s32 row, s32 z, MenuView *v) {
    Character *c = g_gameState->gameData->party[s_menuAbility.slot];
    s32 x, y;
    s32 dx = 8;
    s32 dy = 0x5E;

    x = col * 0x96 + (s_menuAbility.listRect.x + dx);
    y = row * 0xF + (v->y + dy);
    if (s_menuAbility.tab == ABILITY_TAB_ACTION) {
        menuAbility_drawActionRow(x, y, z, c);
    } else {
        menuAbility_drawSupportRow(x, y, z, c);
    }
}

void menuAbility_drawActionRow(s32 x, s32 y, s32 idx, Character *c) {
    u8 buf[8];
    u8 id;
    Unk801F34EC *info;
    s32 st;
    s32 col;

    id = s_menuAbility.actions[idx].id;
    info = &s_menuAbilityActionTable[id];
    st = menuAbility_getActionState(id, c);
    if (st != 0) {
        col = s_menuAbilityStateColor[st];
        text_drawBlock(x + 8, y, 0x1C, s_menuAbilityActionNames + info->unkE, col);
        if (info->unkA != 0) {
            text_formatNumber(menuAbility_getMpCost(info), buf, 3, 1);
            ui_drawSmallDigits(x + 0x6C, y + 4, 0x1C, buf, col);
        }
    }
}

void menuAbility_drawSupportRow(s32 x, s32 y, s32 i, Character *c) {
    u8 buf[0x10];
    Unk8006D128 *sup;
    s32 id;
    u32 st;
    s32 col;

    id = s_menuAbility.supports[i];
    sup = &g_supportAbilities[id - ABILITY_SUPPORT_FIRST];
    st = menuAbility_getSupportState(id, c);
    if (st != 0) {
        col = s_menuAbilityStateColor[st];
        if (st == ABILITY_EQUIPPED) {
            menuAbility_drawIcon(x + 8, y, 0x1C, s_menuAbility.animFrame);
        }
        if (!((g_gameState->config >> 18) & 1)) {
            menuAbility_drawIcon(x + 8, y, 0x1C, st >= ABILITY_USABLE ? 9 : 10);
        } else {
            menuAbility_drawIcon(x + 8, y, 0x1C, st >= ABILITY_USABLE ? 7 : 8);
        }
        text_drawBlock(x + 0x18, y, 0x1C, s_menuAbilitySupportNames + sup->unk2, col);
        text_formatNumber(sup->unk1, buf, 3, 1);
        ui_drawSmallDigits(x + 0x6C, y + 4, 0x1C, buf, col);
    }
}

u8 menuAbility_findCommand(s32 id, Character *c) {
    s32 i;
    s32 j;
    u8 k;
    MenuCmd *e;

    for (i = 0; i < 2; i++) {
        k = s_menuAbilityCharCmds[c->info.b.row][i];
        e = &s_menuAbilityCmds[k];
        if (e->kind == MENU_CMD_ABILITIES) {
            for (j = 0; j < e->count; j++) {
                if (id == s_menuAbilityCmdAbilities[e->first + j]) {
                    return k;
                }
            }
        }
    }
    return 0;
}

u32 menuAbility_getSupportState(s32 id, Character *c) {
    Unk8006D128 *sup;
    s32 slot;
    s32 ap;
    s32 need;

    if (id < ABILITY_SUPPORT_FIRST) {
        return 0;
    }
    if (chr_testSupportBit(c->supportAbilities, id)) {
        return ABILITY_EQUIPPED;
    }
    if (s_menuAbility.equipSlots[id] == 0) {
        slot = chr_findAbilitySlot(c->info.b.id, id);
        if (slot < 0) {
            return 0;
        }
        if (func_80034A30(c)) {
            ap = c->ap[slot];
            need = g_abilityRows[c->info.b.row][slot].unk1;
            if (ap == 0) {
                return 0;
            }
            if (ap < need) {
                return 0;
            }
        }
    }
    sup = &g_supportAbilities[id - ABILITY_SUPPORT_FIRST];
    if (sup->unk1 > c->stones) {
        return ABILITY_GREYED;
    }
    return ABILITY_USABLE;
}

void menuAbility_closeTarget(void) {
    MenuList *l = &s_menuAbility.lists[s_menuAbility.tab];

    tween_start(&s_menuAbility.targetWin, l->col == 0 ? &s_menuAbilityTargetCloseR : &s_menuAbilityTargetCloseL, 0);
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
    s32 kind = s_menuAbilityActionTable[id].unk0;
    s32 n = 0;
    s32 i;

    for (i = 0; i < 4; i++) {
        if (g_gameState->gameData->party[i] != NULL) {
            n++;
        }
    }
    if (n < 2) {
        return 0;
    }
    return (u32)(kind - 3) < 3;
}

void menuAbility_onMemberSwitched(void) {
    s_menuAbility.state = ABILITY_STATE_TABS;
}

void menuAbility_loadImages(void) {
    u8 *p = s_menuAbilityImage;

    LoadImage(&s_menuAbilityClutRect, (u32 *)p);
    DrawSync(0);
    LoadImage(&s_menuAbilityImageRect, (u32 *)(p + 0x120));
    DrawSync(0);
}

void menuAbility_drawIcon(s32 x, s32 y, s32 otz, s32 i) {
    ui_drawSpriteDef(x, y, otz, menuAbility_getIcon(i), 0);
}

Icon *menuAbility_getIcon(s32 i) {
    return &s_menuAbilityIcons[i];
}

s32 menuAbility_getSlot(MenuList *l) {
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

s32 menuAbility_getMpCost(Unk801F34EC *p) {
    s32 v;

    v = p->unkA;
    if ((p->unkB & 4) && D_8007AF02 != 0) {
        v *= 4;
    }
    return v;
}

void menuAbility_initActionCursor(void) {
    Character *c;
    MenuList *l;
    s32 i;
    s32 sel;
    s32 row;
    s32 top;

    sel = 0;
    i = 0;
    c = g_gameState->gameData->party[s_menuAbility.slot];
    l = &s_menuAbility.lists[0];
    for (; i < s_menuAbility.actionCount; i++) {
        if (menuAbility_getActionState(s_menuAbility.actions[i].id, c) == ABILITY_USABLE) {
            sel = i;
            break;
        }
    }
    row = sel / 2;
    l->col = sel % 2;
    top = row - 7;
    if (top >= 0) {
        l->top = top;
    } else {
        l->top = 0;
    }
    l->cursor = row - l->top;
}

void func_801F1148(void) {
}

INCLUDE_RODATA("asm/us/ovl_33/nonmatchings/menu_ability", g_abilityMenuText);

INCLUDE_RODATA("asm/us/ovl_33/nonmatchings/menu_ability", s_menuAbilityImage);
