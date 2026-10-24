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
    if (func_80034A04(c->unk88, id)) {
        return ABILITY_EQUIPPED;
    }
    if (s_menuAbility.equipSlots[id] == 0) {
        slot = func_80034430(c->unk34.b.id, id);
        if (slot < 0) {
            return 0;
        }
        if (func_80034A30(c)) {
            ap = c->unk58[slot];
            need = D_8006CB28[c->unk34.b.row][slot].unk1;
            if (ap == 0) {
                return 0;
            }
            if (ap < need) {
                return 0;
            }
        }
    }
    sup = &D_8006D128[id - ABILITY_SUPPORT_FIRST];
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
    func_80028CF8(&s_menuAbility.header, D_80077E50.unk3, menuAbility_onMemberSwitched);
    menuAbility_setMember(D_80077E50.unk3, 0);
    s_menuAbility.animFrame = 0;
    s_menuAbility.animTick = 0;
    text_setBank(g_abilityMenuText);
    func_801F1148();
    func_8003556C(func_800355DC);
    func_80035674(1);
    setRECT(&s_menuAbility.tabRect, 8, 8, 0xE4, 0x1A);
    setRECT(&s_menuAbility.playerRect, 8, 0x26, 0xD0, 0x38);
    setRECT(&s_menuAbility.infoRect, 0xD8, 0x26, 0x5C, 0x38);
    setRECT(&s_menuAbility.commandRect, 0x8E, 0x40, 0x46, 0x1A);
    setRECT(&s_menuAbility.listRect, 8, 0x5E, 0x12C, 0x80);
    setRECT(&s_menuAbility.targetRect, 0, 0x26, 0x90, 0xB6);
    s_menuAbility.targetWinOpen = 0;
    func_80063118(&s_menuAbility.targetWin, 0, 0);
    menuAbility_loadImages();
    s_menuAbility.partySize = 0;
    for (i = 0; i < 4; i++) {
        if (g_gameState->unk1C->party[i] != NULL) {
            s_menuAbility.partySize++;
        }
    }
}

void menuAbility_updateAnims(void) {
    AbilityMenu *m;

    if (func_8003561C()) {
        menuAbility_updateHelp();
    }
    m = &s_menuAbility;
    if (m->state == ABILITY_STATE_LIST) {
        func_8002EB74(&s_menuAbility.lists[s_menuAbility.tab]);
    }
    if (m->targetWin.unkC != 0) {
        func_800631AC(&m->targetWin);
    }
    if (++m->animTick >= 7) {
        m->animTick = 0;
        if (++m->animFrame >= 7) {
            m->animFrame = 0;
        }
    }
    if (s_menuAbility.header.unk0 != 0) {
        func_80028D2C(&s_menuAbility.header);
    }
}

void menuAbility_inputTabs(void) {
    u8 slot;
    s32 next;

    if (g_padRepeat & BTN_CIRCLE) {
        snd_playSe(SE_CANCEL);
        func_800361CC(0);
    } else if (g_padRepeat & BTN_SQUARE) {
        snd_playSe(SE_CURSOR);
        func_80036270(MENU_SCREEN_EQUIP);
    } else if (g_padRepeat & PAD_LEFT) {
        snd_playSe(SE_CURSOR);
        s_menuAbility.tab = s_menuAbility.tab != 0 ? s_menuAbility.tab - 1 : ABILITY_TAB_SUPPORT;
    } else if (g_padRepeat & PAD_RIGHT) {
        snd_playSe(SE_CURSOR);
        s_menuAbility.tab = s_menuAbility.tab == 0 ? s_menuAbility.tab + 1 : 0;
    } else if (g_padRepeat & BTN_R1) {
        slot = s_menuAbility.slot;
        next = func_8003380C(slot);
        if (slot != next) {
            menuAbility_setMember(next, 2);
            snd_playSe(SE_PAGE);
        }
    } else if (g_padRepeat & BTN_L1) {
        slot = s_menuAbility.slot;
        next = func_800337D0(slot);
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
            next = func_800337D0(cur);
            if (cur != next) {
                s_menuAbility.target = next;
                snd_playSe(SE_CURSOR);
            }
        }
    } else if (g_padRepeat & PAD_DOWN) {
        if (!(s_menuAbility.target & ABILITY_TARGET_ALL)) {
            cur = s_menuAbility.target;
            next = func_8003380C(cur);
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
    c = g_gameState->unk1C->party[s_menuAbility.slot];
    l = &s_menuAbility.lists[s_menuAbility.tab];
    it = &s_menuAbilityActionTable[s_menuAbility.actions[(l->unk5 + l->cursor) * 2 + l->unk7].id];
    if (!(s_menuAbility.target >> 7)) {
        ok = func_800654F8(c, g_gameState->unk1C->party[s_menuAbility.target], it, it->unk4, 0);
    } else {
        for (i = 0; i < 4; i++) {
            if (g_gameState->unk1C->party[i] != NULL) {
                ok |= func_800654F8(c, g_gameState->unk1C->party[i], it, it->unk4, 1);
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
    c = g_gameState->unk1C->party[s_menuAbility.slot];
    func_80024428(r->x + 8, r->y + 5, 0x1D, (Unk80024428 *)&s_menuAbility.header);
    box.w = 0x5B;
    box.h = 0x12;
    box.x = r->x + 0x72;
    box.y = r->y + 4;
    func_80026FC4(box.x + 3, box.y + 4, 0x1D, c->unk38, 0);
    func_8002DEE4(&box, 0x1D, 0);
    if (s_menuAbility.tab == ABILITY_TAB_SUPPORT) {
        func_800273C0(box.x + 0x19, box.y + 0x14, 0x1D, c->stones, c->maxStones);
    }
    ui_drawLabel(r->x + 6, r->y - 3, 0x1D, D_80070498, 0);
    func_8002DEE4(r, 0x1D, 1);
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
    c = g_gameState->unk1C->party[s_menuAbility.slot];
    l = &s_menuAbility.lists[s_menuAbility.tab];
    n = (l->unk5 + l->cursor) * 2 + l->unk7;
    ui_drawLabel(r->x + 6, r->y - 3, 0x1D, D_80070460, 0);
    func_8002DEE4(r, 0x1D, 2);
    text_drawBlock(r->x + 8, r->y + 8, 0x1C, text_get(ABILITY_TEXT_AP), 0);
    text_drawBlock(r->x + 8, r->y + 0x16, 0x1C, text_get(ABILITY_TEXT_EQUIP), 0);
    if (s_menuAbility.state == ABILITY_STATE_TABS || s_menuAbility.state == ABILITY_STATE_SWITCH || s_menuAbility.state == ABILITY_STATE_BUSY || l->unk0 == 0) {
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
        func_80027110(r->x + 0x2D, r->y + 0xC, 0x1C, func_80034584(c->unk34.b.id, id), func_800347D8(c->unk34.b.id, id), flag, 0);
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
    idx = (l->cursor + l->unk5) * 2 + l->unk7;
    c = g_gameState->unk1C->party[m->slot];
    if (m->tab != ABILITY_TAB_SUPPORT) {
        if (m->state != ABILITY_STATE_TABS && m->state != ABILITY_STATE_SWITCH && m->state != ABILITY_STATE_BUSY) {
            if (menuAbility_getActionState(m->actions[idx].id, c) != 0) {
                text_drawBlock(r->x + 8, r->y + 6, 0x1B, s_menuAbilityCmdNames + s_menuAbilityCmds[m->actions[idx].cmd].name, 0);
            }
        }
        ui_drawLabel(r->x + 6, r->y - 3, 0x1B, D_8007042C, 0);
        func_8002DEE4(r, 0x1B, 2);
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
    ui_drawLabel(r->x + 0xE, r->y - 3, 0x1C, D_800703FC.unk4, 0);
    ui_drawLabel(r->x + 0xA4, r->y - 3, 0x1C, D_800703FC.unk4, 0);
    ui_drawLabel(r->x + 0x7C, r->y - 3, 0x1C, D_800703FC.unk7C, 0);
    ui_drawLabel(r->x + 0x112, r->y - 3, 0x1C, D_800703FC.unk7C, 0);
    func_8002E040(r, 0x1D, 1, l);
    if (l->unk0 != 0) {
        menuList_getView(l, &view);
        win_setClip(NULL, 0x1C, 1);
        idx = view.unk1 * 2;
        for (row = 0; row < view.unk0; row++) {
            for (col = 0; col < 2; col++) {
                menuAbility_drawRow(col, row, idx++, &view);
            }
        }
        if (l->unk8 & 4) {
            func_8002F40C(l, r, &sub);
            win_setClip(&sub, 0x1C, 1);
            menuAbility_drawScrollRows();
        }
        win_setClip(r, 0x1C, 1);
    }
}

void menuAbility_drawTargetWindow(void) {
    RECT *r = &s_menuAbility.targetWin.unk4;
    AbilityMenu *m = &s_menuAbility;
    s32 i;

    if (m->targetWinOpen != 0) {
        ui_drawLabel(r->x + 8, r->y - 3, 0x14, D_800704B8, 0);
        func_8002DEE4(r, 0x14, 2);
        for (i = 0; i < 4; i++) {
            if (g_gameState->unk1C->party[i] != NULL) {
                func_80026EA0(r->x + 8, r->y + 6 + i * 0x2B, 0x13, i, s_menuAbility.targetMode);
            }
        }
    }
}

void menuAbility_drawCursor(void) {
    RECT *r;
    MenuList *l;
    Unk80077DE8 *h3;
    s32 i;
    s32 y;
    u16 k;
    u8 blink;
    u8 done;
    s16 ry;

    done = 0;
    blink = (D_80077E54 & 3) != 0;
    func_80035674(s_menuAbility.state != ABILITY_STATE_TABS);
    if (func_8002F810()) {
        return;
    }
    if (s_menuAbility.state == ABILITY_STATE_TABS || s_menuAbility.state == ABILITY_STATE_SWITCH || blink) {
        Unk80077DE8 *h1;
        r = &s_menuAbility.tabRect;
        h1 = &D_80077DE8;
        k = s_menuAbilityTabCursorX[s_menuAbility.tab][0] - 0x18;
        D_80077DE8.unk0 = r->x + k;
        ry = r->y + 8;
        h1->unk4 = 0x17;
        h1->unk2 = ry;
        func_800355E4();
    }
    if (s_menuAbility.state == ABILITY_STATE_LIST || (s_menuAbility.state == ABILITY_STATE_TARGET && blink)) {
        Unk80077DE8 *h2;
        l = &s_menuAbility.lists[s_menuAbility.tab];
        h2 = &D_80077DE8;
        k = l->unk7;
        k ^= (l->unk8 >> 3) & 1;
        k = k * 150 - 10;
        D_80077DE8.unk0 = s_menuAbility.listRect.x + k;
        k = l->cursor * 15 + 10;
        h2->unk4 = 0x17;
        h2->unk2 = s_menuAbility.listRect.y + k;
        func_800355E4();
    }
    if (s_menuAbility.state == ABILITY_STATE_TARGET) {
        r = &s_menuAbility.targetWin.unk4;
        if (s_menuAbility.target & ABILITY_TARGET_ALL) {
            D_80077DE8.unk4 = 0x12;
            if (blink) {
                for (i = 0; i < 4; i++) {
                    y = i * 0x2B + 0x12;
                    if (g_gameState->unk1C->party[i] != NULL) {
                        ui_drawHand(r->x - 10, r->y + y, D_80077DE8.unk4);
                        if (!done) {
                            D_80077DE8.unk0 = r->x - 10;
                            done = 1;
                            D_80077DE8.unk2 = r->y + y;
                        }
                    }
                }
            }
        } else {
            D_80077DE8.unk0 = s_menuAbility.targetWin.unk4.x - 10;
            h3 = &D_80077DE8;
            k = s_menuAbility.target * 0x2B + 0x12;
            ry = r->y;
            h3->unk4 = 0x12;
            h3->unk2 = ry + k;
            func_800355E4();
        }
    }
}

void menuAbility_setMember(s32 slot, s32 arg1) {
    AbilityMenu *m;
    AbilityMenu *m2;
    UnkBlink *b;
    Character *c;
    AbilityEntry *a;
    Unk80069018 *it;
    s32 j;
    s32 nItems;
    s32 nAbils;
    s32 id;

    b = &s_menuAbility.header;
    c = g_gameState->unk1C->party[slot];
    m = &s_menuAbility;
    nItems = 0;
    nAbils = 0;
    func_80028D08(b, m->slot, slot, arg1);
    if (arg1 != 0) {
        m->state = ABILITY_STATE_SWITCH;
    }
    m->slot = slot;
    if (c->unk34.b.row < 16) {
        m2 = m;
        slot = 0;
        a = D_8006CB28[c->unk34.b.row];
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
    func_8002EDFC(&s_menuAbility.lists[0], nItems, 8, 2, 0x80);
    func_8002EDFC(&s_menuAbility.lists[1], nAbils, 8, 2, 0x80);
    s_menuAbility.actionCount = nItems;
    s_menuAbility.supportCount = nAbils;
    memset(s_menuAbility.equipSlots, 0, 0x100);
    for (slot = 0; slot < 5; slot++) {
        id = c->equip[slot];
        if (id != 0xFF) {
            it = &D_80069018[id];
            for (j = 0; j < 3; j++) {
                s_menuAbility.equipSlots[it->unkC[j]] |= 1 << slot;
                s_menuAbility.equipItems[it->unkC[j]][slot] = id;
            }
        }
    }
    s_menuAbility.tabEnabled[0] = c->hp != 0 && !(c->unk38 & 9) && nItems != 0 && D_8007AFD3 == 0;
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
        slot = func_80034430(c->unk34.b.id, id);
        if (slot < 0) {
            return 0;
        }
        if (func_80034A30(c)) {
            ap = c->unk58[slot];
            need = D_8006CB28[c->unk34.b.row][slot].unk1;
            if (ap == 0) {
                return 0;
            }
            if (ap < need) {
                return 0;
            }
        }
    }
    if (c->unk38 & 9) {
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
    Character *c = g_gameState->unk1C->party[s_menuAbility.slot];
    s32 i;
    u8 id;

    if (l->unk2 != 0) {
        i = (l->unk5 + l->cursor) * 2 + l->unk7;
        if (i < s_menuAbility.actionCount) {
            id = s_menuAbility.actions[i].id;
            if (menuAbility_getActionState(id, c) == ABILITY_USABLE) {
                s_menuAbility.targetMode = s_menuAbilityActionTable[id].unk0_5;
                s_menuAbility.canTargetAll = menuAbility_canTargetAll(id);
                ls = s_menuAbility.lists + s_menuAbility.tab;
                s_menuAbility.targetWin.unk4 = s_menuAbility.targetRect;
                if (ls->unk7 == 0) {
                    s_menuAbility.targetWin.unk4.x = 0xA4;
                    func_80063118(&s_menuAbility.targetWin, &s_menuAbilityTargetOpenL, 1);
                } else {
                    s_menuAbility.targetWin.unk4.x = 8;
                    func_80063118(&s_menuAbility.targetWin, &s_menuAbilityTargetOpenR, 1);
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
    c = g_gameState->unk1C->party[s_menuAbility.slot];
    if (l->unk2 != 0 && (idx = (l->unk5 + l->cursor) * 2 + l->unk7) < s_menuAbility.supportCount) {
        id = s_menuAbility.supports[idx];
        sup = &D_8006D128[id - ABILITY_SUPPORT_FIRST];
        st = menuAbility_getSupportState_inline(id, c);
        if (st >= ABILITY_USABLE) {
            func_8003493C(c->unk34.b.id, id, st == ABILITY_USABLE);
            ap = c->stones;
            if (st == ABILITY_USABLE) {
                c->stones = ap - sup->unk1;
            } else {
                c->stones = ap + sup->unk1;
            }
            func_8003384C(c);
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
    c = g_gameState->unk1C->party[s_menuAbility.slot];
    D_80077DE8.unk8 = NULL;
    func_80035580(NULL);
    if (s_menuAbility.state == ABILITY_STATE_TABS || s_menuAbility.state == ABILITY_STATE_SWITCH) {
        buf = s_menuAbilityHelpBuf;
        D_80077DE8.unk8 = buf;
        func_8002BE44(0, s_menuAbilityHelpArg);
        func_8002BD54(buf, text_get(s_menuAbility.tab + ABILITY_TEXT_TAB_HELP));
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
            if (c->unkA & 0x10) {
                t = ABILITY_TEXT_CANNOT_EQUIP;
            } else {
                t = ABILITY_TEXT_CANNOT_EQUIP_NOW;
            }
        }
        text_append(buf, text_get(t));
    end:
        text_append(s_menuAbilityHelpBuf, text_get(ABILITY_TEXT_TO_EQUIP));
    } else if (s_menuAbility.state == ABILITY_STATE_LIST) {
        if (l->unk0 == 0) {
            return;
        }
        r.x = 0;
        r.y = 0x5E;
        r.w = g_gameState->unkC->disp.w;
        r.h = g_gameState->unkC->disp.h - 0x5E;
        func_80035580(&r);
        i = menuAbility_getSlot(l);
        if (s_menuAbility.tab == ABILITY_TAB_ACTION) {
            id = s_menuAbility.actions[i].id;
            if (menuAbility_getActionState(id, c)) {
                D_80077DE8.unk8 = s_menuAbilityActionHelp + s_menuAbilityActionHelpOfs[id].unk0;
            }
        } else {
            id = s_menuAbility.supports[i];
            if (menuAbility_getSupportState_inline(id, c)) {
                u8 **d = &D_80077DF0;
                *d = menuAbility_updateHelp_suphelp(id);
            }
        }
    } else if (s_menuAbility.state == ABILITY_STATE_TARGET) {
        if (menuAbility_getSlot(l) & 1) {
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
        D_80077DF0 = text_get(s_menuAbility.canTargetAll == 0 ? ABILITY_TEXT_SELECT_CHAR : ABILITY_TEXT_SELECT_CHAR_ALL);
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
    c = g_gameState->unk1C->party[s_menuAbility.slot];
    l = &s_menuAbility.lists[1];
    for (; i < s_menuAbility.supportCount; i++) {
        if (menuAbility_getSupportState_inline(s_menuAbility.supports[i], c) >= 3) {
            sel = i;
            break;
        }
    }
    row = sel / 2;
    l->unk7 = sel % 2;
    if (row - 7 >= 0) {
        l->unk5 = row - 7;
    } else {
        l->unk5 = 0;
    }
    l->cursor = row - l->unk5;
}

void menuAbility_update(void) {
    menuAbility_input();
    menuAbility_draw();
    menuAbility_updateAnims();
}

void menuAbility_exit(void) {
    D_80077E53 = s_menuAbility.slot;
    func_80035674(1);
    func_8003556C(NULL);
}

void menuAbility_input(void) {
    void (*fn)(void);

    if (!func_8002F810()) {
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
    if (l->unk8 == 0) {
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
        func_8002F044(l, g_padRepeat);
        func_8002F0EC(l, g_padRepeat);
    }
}

void menuAbility_drawTabs(void) {
    RECT *r;
    s32 i;

    r = &s_menuAbility.tabRect;
    func_8002DEE4(r, 0x1D, 2);
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
    func_8002F344(&s_menuAbility.lists[s_menuAbility.tab], &v);
    idx = v.unk1 * 2;
    for (; row < v.unk0; row++) {
        for (col = 0; col < 2; col++) {
            menuAbility_drawRow(col, row, idx++, &v);
        }
    }
}

void menuAbility_drawRow(s32 col, s32 row, s32 z, MenuView *v) {
    Character *c = g_gameState->unk1C->party[s_menuAbility.slot];
    s32 x, y;
    s32 dx = 8;
    s32 dy = 0x5E;

    x = col * 0x96 + (s_menuAbility.listRect.x + dx);
    y = row * 0xF + (v->unk2 + dy);
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
    sup = &D_8006D128[id - ABILITY_SUPPORT_FIRST];
    st = menuAbility_getSupportState(id, c);
    if (st != 0) {
        col = s_menuAbilityStateColor[st];
        if (st == ABILITY_EQUIPPED) {
            menuAbility_drawIcon(x + 8, y, 0x1C, s_menuAbility.animFrame);
        }
        if (!((g_gameState->unk24 >> 18) & 1)) {
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
        k = s_menuAbilityCharCmds[c->unk34.b.row][i];
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
    if (func_80034A04(c->unk88, id)) {
        return ABILITY_EQUIPPED;
    }
    if (s_menuAbility.equipSlots[id] == 0) {
        slot = func_80034430(c->unk34.b.id, id);
        if (slot < 0) {
            return 0;
        }
        if (func_80034A30(c)) {
            ap = c->unk58[slot];
            need = D_8006CB28[c->unk34.b.row][slot].unk1;
            if (ap == 0) {
                return 0;
            }
            if (ap < need) {
                return 0;
            }
        }
    }
    sup = &D_8006D128[id - ABILITY_SUPPORT_FIRST];
    if (sup->unk1 > c->stones) {
        return ABILITY_GREYED;
    }
    return ABILITY_USABLE;
}

void menuAbility_closeTarget(void) {
    MenuList *l = &s_menuAbility.lists[s_menuAbility.tab];

    func_80063118(&s_menuAbility.targetWin, l->unk7 == 0 ? &s_menuAbilityTargetCloseR : &s_menuAbilityTargetCloseL, 0);
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
        if (g_gameState->unk1C->party[i] != NULL) {
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
    func_80027710(x, y, otz, menuAbility_getIcon(i), 0);
}

Icon *menuAbility_getIcon(s32 i) {
    return &s_menuAbilityIcons[i];
}

s32 menuAbility_getSlot(MenuList *l) {
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
    c = g_gameState->unk1C->party[s_menuAbility.slot];
    l = &s_menuAbility.lists[0];
    for (; i < s_menuAbility.actionCount; i++) {
        if (menuAbility_getActionState(s_menuAbility.actions[i].id, c) == ABILITY_USABLE) {
            sel = i;
            break;
        }
    }
    row = sel / 2;
    l->unk7 = sel % 2;
    top = row - 7;
    if (top >= 0) {
        l->unk5 = top;
    } else {
        l->unk5 = 0;
    }
    l->cursor = row - l->unk5;
}

void func_801F1148(void) {
}

INCLUDE_RODATA("asm/us/ovl_33/nonmatchings/menu_ability", g_abilityMenuText);

INCLUDE_RODATA("asm/us/ovl_33/nonmatchings/menu_ability", s_menuAbilityImage);
