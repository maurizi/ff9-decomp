#include "ovl_38.h"

void menuStatus_init(void) {
    s32 i;
    RECT *r;

    s_menuStatus.slot = STATUS_NO_SLOT;
    s_menuStatus.state = STATUS_STATE_MAIN;
    s_menuStatus.pagesOpen = 0;
    for (i = 0; i < 6; i++) {
        s_menuStatus.pageOnTop[i] = 0;
        s_menuStatus.pageOpen[i] = 0;
    }
    text_setBank(g_statusMenuText);
    ui_initMemberSwitch(&s_menuStatus.header, g_menuScreen.member, menuStatus_onMemberSwitched);
    menuStatus_setMember(g_menuScreen.member, 0);
    r = &s_menuStatus.win;
    r->x = 8;
    r->y = 0x26;
    r->w = 0x12C;
    r->h = 0xB6;
    s_menuStatus.savedHelp = help_getFollow();
    help_setFollow(0);
    help_setCallback(menuStatus_updateHelp);
    s_menuStatus.primBufs[0] = PSX_RAM(u8 *, 0x801F5F00);
    s_menuStatus.primBufs[1] = PSX_RAM(u8 *, 0x801F9F00);
    s_menuStatus.prim = s_menuStatus.primBufs[g_gameState->gameData->drawBuf];
}

void menuStatus_inputMain(void) {
    u8 cur;
    s32 next;

    if (g_padRepeat & BTN_CIRCLE) {
        snd_playSe(SE_CANCEL);
        menuScreen_open(0);
        return;
    }
    if (g_padRepeat & BTN_CROSS) {
        menuStatus_openAbilityPage();
        return;
    }
    if (g_padRepeat & BTN_R1) {
        cur = s_menuStatus.slot;
        next = party_getNextMember(cur);
        if (cur != next) {
            menuStatus_setMember(next, 2);
            snd_playSe(SE_PAGE);
        }
    } else if (g_padRepeat & BTN_L1) {
        cur = s_menuStatus.slot;
        next = party_getPrevMember(cur);
        if (cur != next) {
            snd_playSe(SE_PAGE);
            menuStatus_setMember(next, 1);
        }
    }
}

void menuStatus_drawHeader(void) {
    RECT r;
    Character *c;
    RECT *w;
    s16 x;
    s16 y;

    w = &s_menuStatus.win;
    c = g_gameState->gameData->party[s_menuStatus.slot];
    x = w->x + 0x72;
    y = w->y + 4;
    setRECT(&r, x, y, 0x5C, 0x12);
    ui_drawStones(x + 0x19, y + 0x14, 0x1E, c->stones, c->maxStones);
    ui_drawMemberHeader(w->x + 8, w->y + 6, 0x1D, (MemberSwitch *)&s_menuStatus.header);
    ui_drawStatusIcons(r.x + 4, r.y + 4, 0x1D, c->status, 0);
    win_drawBox(&r, 0x1D, 0);
}

void menuStatus_drawCommands(void) {
    RECT r;
    StatusTable t;
    StatusPool pool;
    RECT *w;
    Character *c;
    s32 i;
    s32 y;
    s32 k;
    s32 off;
    StatusEntry *tp;

    w = &s_menuStatus.win;
    c = g_gameState->gameData->party[s_menuStatus.slot];
    t = s_menuStatusCmdTable;
    pool = s_menuStatusCmdNames;
    r.w = 0x42;
    r.h = 0x48;
    r.x = w->x + 0xE4;
    r.y = w->y + 8;
    s_menuStatusCmds[1] = s_menuStatusCharCmds[c->info.b.row][0];
    s_menuStatusCmds[2] = s_menuStatusCharCmds[c->info.b.row][1];
    for (i = 0; i < 4; i++) {
        tp = t.v;
        y = i * 15 + 6;
        off = s_menuStatusCmds[i] * sizeof(StatusEntry);
        k = ((StatusEntry *)((u8 *)tp + off))->unk0;
        s_menuStatus.prim = text_buildBlock(r.x + 0xC, r.y + y, g_ot + 25, pool.s + k, 0, s_menuStatus.prim);
    }
    ui_drawLabel(r.x + 6, r.y - 3, 0x19, D_8007042C, 0);
    win_drawBox(&r, 0x19, 2);
    win_drawShadow(&r, 0x19, 2);
}

void menuStatus_drawStats(void) {
    u8 buf[16];
    RECT *r;
    StatusMenu *m;
    Character *c;
    s32 y;
    s32 i;

    m = &s_menuStatus;
    r = &m->win;
    c = g_gameState->gameData->party[m->slot];
    y = 0x3D;
    for (i = 0; i < 4; i++) {
        s_menuStatus.prim = text_buildBlock(r->x + 0xA, r->y + (i * 13 + y) - 4, g_ot + 0x1D, text_get(s_menuStatusBaseStatText[i]), 0, s_menuStatus.prim);
    }
    y = 0x72;
    for (i = 0; i < 5; i++) {
        s_menuStatus.prim = text_buildBlock(r->x + 0xA, r->y + (i * 13 + y) - 4, g_ot + 0x1D, text_get(s_menuStatusBattleStatText[i]), 0, s_menuStatus.prim);
    }
    text_formatNumber(c->speed, buf, 2, 1);
    ui_drawSmallDigits(r->x + 0x58, r->y + 0x3D, 0x1D, buf, 0);
    text_formatNumber(c->strength, buf, 2, 1);
    ui_drawSmallDigits(r->x + 0x58, r->y + 0x4A, 0x1D, buf, 0);
    text_formatNumber(c->magic, buf, 2, 1);
    ui_drawSmallDigits(r->x + 0x58, r->y + 0x57, 0x1D, buf, 0);
    text_formatNumber(c->spirit, buf, 2, 1);
    ui_drawSmallDigits(r->x + 0x58, r->y + 0x64, 0x1D, buf, 0);
    text_formatNumber(g_weaponTable[c->equip[0]].unk5, buf, 3, 1);
    ui_drawSmallDigits(r->x + 0x51, r->y + 0x72, 0x1D, buf, 0);
    text_formatNumber(c->defense, buf, 3, 1);
    ui_drawSmallDigits(r->x + 0x51, r->y + 0x7F, 0x1D, buf, 0);
    text_formatNumber(c->evade, buf, 3, 1);
    ui_drawSmallDigits(r->x + 0x51, r->y + 0x8C, 0x1D, buf, 0);
    text_formatNumber(c->magicDef, buf, 3, 1);
    ui_drawSmallDigits(r->x + 0x51, r->y + 0x99, 0x1D, buf, 0);
    text_formatNumber(c->magicEva, buf, 3, 1);
    ui_drawSmallDigits(r->x + 0x51, r->y + 0xA6, 0x1D, buf, 0);
}

void menuStatus_drawExp(void) {
    u8 buf[16];
    RECT *r;
    StatusMenu *st;
    Character *c;
    u32 next;
    s32 bit;
    u8 show;

    r = &s_menuStatus.win;
    st = &s_menuStatus;
    c = g_gameState->gameData->party[st->slot];
    show = 0;
    if (D_8007AF00 != 0) {
        bit = c->classFlags & 0x10;
        show = bit == 0;
    }
    if (show) {
        st->prim = text_buildBlock(r->x + 0x74, r->y + 0x39, &g_ot[0x1D], text_get(STATUS_TEXT_TRANCE), 0, st->prim);
        ui_drawGauge(r->x + 0xB4, r->y + 0x3E, 0x1D, c->trance, 0xFF, 1, 1);
    }
    text_drawBlock(r->x + 0x74, r->y + 0x46, 0x1D, text_get(STATUS_TEXT_EXP), 0);
    text_drawBlock(r->x + 0x74, r->y + 0x53, 0x1D, text_get(STATUS_TEXT_NEXT), 0);
    text_formatNumber(c->exp, buf, 7, 1);
    ui_drawSmallDigits(r->x + 0xAC, r->y + 0x4A, 0x1D, buf, 0);
    if (c->level >= 99) {
        next = c->exp;
    } else {
        next = g_levelExp[c->level];
    }
    text_formatNumber(next - c->exp, buf, 6, 1);
    ui_drawSmallDigits(r->x + 0xB3, r->y + 0x57, 0x1D, buf, 0);
}

INCLUDE_RODATA("asm/us/ovl_38/nonmatchings/ovl_38", s_menuStatusCmdTable);

INCLUDE_RODATA("asm/us/ovl_38/nonmatchings/ovl_38", s_menuStatusCmdNames);

INCLUDE_RODATA("asm/us/ovl_38/nonmatchings/ovl_38", D_801EE264);

void menuStatus_drawEquipment(void) {
    RECT *r = &s_menuStatus.win;
    StatusMenu *st;
    Character *c;
    s32 i;
    u8 id;
    u8 buf[8];

    st = &s_menuStatus;
    c = g_gameState->gameData->party[st->slot];
    for (i = 0; i < 5; i++) {
        s_menuStatus.prim = ui_buildSprite(r->x + 0x74, r->y + 0x68 + i * 15 + 1, &g_ot[0x1D], i + 0xF9, 0, buf, s_menuStatus.prim);
        s_menuStatus.prim = text_buildSmall(r->x + 0x84, r->y + 0x68 + i * 15 + 3, &g_ot[0x1D], D_801EE264, 0, s_menuStatus.prim);
        id = c->equip[i];
        if (id != ITEM_NONE) {
            ui_drawItemIcon(r->x + 0x8D, r->y + 0x68 + i * 15 - 1, 0x1D, id, 1);
            s_menuStatus.prim = text_buildBlock(r->x + 0x9D, r->y + 0x68 + i * 15 - 1, &g_ot[0x1D], g_itemNames + g_itemTable[id].name, 0, s_menuStatus.prim);
        }
    }
}

void menuStatus_drawAbilityPages(void) {
    s32 i, j;
    s32 base = 0;
    RECT *r;
    s32 z;

    for (i = 0; i < s_menuStatus.pageCount; i++) {
        if (s_menuStatus.pageOpen[i] != 0) {
            r = &s_menuStatus.pageWins[i].rect;
            for (j = 0; j < STATUS_PAGE_ROWS; j++) {
                if (base + j < s_menuStatus.abilityCount) {
                    menuStatus_drawAbilityRow(r->x + 0xA, r->y + 8 + j * 15, 0x14 - i, base + j, s_menuStatus.pageOnTop[i]);
                }
            }
            z = 0x14 - i;
            ui_drawLabel(r->x + 6, r->y - 3, z, g_winLabelStrings.unk4, 0);
            if (s_menuStatus.pageOnTop[i] != 0) {
                s_menuStatusPageLabel[1] = i + 0x31;
                ui_drawLabel(r->x + 0x90, r->y - 3, z, g_winLabelStrings.unk88, 0);
                ui_drawLabel(r->x + 0x9A, r->y - 3, z, (s32)s_menuStatusPageLabel, 0);
            }
            win_drawBox(r, z, 2);
            win_drawShadow(r, z, 2);
        }
        base += STATUS_PAGE_ROWS;
    }
}

void menuStatus_drawAbilityRow(s32 x, s32 y, s32 z, s32 i, u8 draw) {
    Character *c;
    u8 *name;
    u8 ok;
    s32 id;
    s32 max;
    s32 cur;
    s32 icon;
    u8 flag;
    ActiveAbility *a;
    Unk8006D128 *sup;

    c = g_gameState->gameData->party[s_menuStatus.slot];
    ok = func_80034A30(c);
    if (ok && c->ap[i] == 0) {
        return;
    }
    id = g_abilityRows[c->info.b.row][i].id;
    max = g_abilityRows[c->info.b.row][i].ap;
    if (id == 0) {
        return;
    }
    cur = c->ap[i];
    icon = 0xBC;
    if (id < STATUS_SUPPORT_FIRST) {
        a = &s_menuStatusActionAbilities[id];
        name = s_menuStatusActionNames + a->unkE;
        flag = !(a->unkB & 2);
    } else {
        sup = &g_supportAbilities[id - STATUS_SUPPORT_FIRST];
        name = s_menuStatusSupportNames + sup->unk2;
        if (chr_testSupportBit(c->supportAbilities, id)) {
            icon = 0xBD;
        } else {
            icon = 0xF4;
        }
        flag = 1;
    }
    if (draw) {
        ui_drawSprite(x, y, z, icon, 0);
        s_menuStatus.prim = text_buildBlock(x + 0x10, y, g_ot + z, name, 0, s_menuStatus.prim);
    }
    if (ok) {
        s_menuStatus.prim = ui_buildApGauge(x + 0x70, y + 3, g_ot + z, cur, max, flag, 0, s_menuStatus.prim);
    } else {
        s_menuStatus.prim = ui_buildApGauge(x + 0x70, y + 3, g_ot + z, 1, 1, 0, 0, s_menuStatus.prim);
    }
}

void menuStatus_openAbilityPage(void) {
    WinDef *w;

    if (s_menuStatus.pagesOpen >= s_menuStatus.pageCount) {
        menuStatus_closeAbilityPages();
        return;
    }
    s_menuStatus.state = s_menuStatus.pagesOpen != 0 ? STATUS_STATE_WAIT : STATUS_STATE_BUSY;
    snd_playSe(SE_PAGE);
    w = &s_menuStatus.pageWins[s_menuStatus.pagesOpen];
    s_menuStatus.pageOpen[s_menuStatus.pagesOpen] = 1;
    s_menuStatus.pageOnTop[s_menuStatus.pagesOpen] = 1;
    w->rect = s_menuStatusPageRects[s_menuStatus.pagesOpen];
    tween_start(w, &s_menuStatusPageOpenTween, 1);
}

void menuStatus_setMember(s32 slot, s32 dir) {
    Character *c;
    s32 i;

    c = g_gameState->gameData->party[slot];
    ui_startMemberSwitch(&s_menuStatus.header, s_menuStatus.slot, slot, dir);
    if (dir != 0) {
        s_menuStatus.state = STATUS_STATE_WAIT;
    }
    s_menuStatus.slot = slot;
    s_menuStatus.pageCount = 0;
    s_menuStatus.abilityCount = 0;
    if ((s32)(c->info.b.row) < 0x10) {
        for (i = 0; i < STATUS_ABILITY_MAX; i++) {
            if (g_abilityRows[c->info.b.row][i].id != 0) {
                s_menuStatus.abilityCount++;
            }
        }
        s_menuStatus.pageCount = (s_menuStatus.abilityCount + STATUS_PAGE_ROWS - 1) / STATUS_PAGE_ROWS;
    }
}

void menuStatus_update(void) {
    menuStatus_input();
    menuStatus_draw();
    menuStatus_updateAnims();
}

void menuStatus_exit(void) {
    g_menuScreenMember = s_menuStatus.slot;
    help_setFollow(s_menuStatus.savedHelp);
    help_setCallback(NULL);
}

void menuStatus_input(void) {
    void (*fn)(void);

    if (!fade_isBusy()) {
        fn = s_menuStatusInput[s_menuStatus.state];
        if (fn != NULL) {
            fn();
        }
    }
}

void menuStatus_draw(void) {
    u8 *p = s_menuStatus.prim - 0x4000;

    s_menuStatus.primFree = s_menuStatus.primBufs[g_gameState->gameData->drawBuf ^ 1] - p;
    s_menuStatus.prim = s_menuStatus.primBufs[g_gameState->gameData->drawBuf];
    menuStatus_drawFrame();
    menuStatus_drawHeader();
    menuStatus_drawCommands();
    menuStatus_drawStats();
    menuStatus_drawExp();
    menuStatus_drawEquipment();
    menuStatus_drawAbilityPages();
    func_801EF9B4();
}

void menuStatus_updateAnims(void) {
    s32 i;

    for (i = 0; i < 6; i++) {
        if (s_menuStatus.pageOpen[i] != 0 && s_menuStatus.pageWins[i].anim != 0) {
            tween_step(&s_menuStatus.pageWins[i]);
        }
    }
    ui_updateMemberSwitch(&s_menuStatus.header);
}

void menuStatus_inputAbilities(void) {
    if (g_padRepeat & BTN_CIRCLE) {
        snd_playSe(SE_CANCEL);
        menuStatus_closeAbilityPages();
        return;
    }
    if (g_padRepeat & BTN_CROSS) {
        snd_playSe(SE_CURSOR);
        menuStatus_openAbilityPage();
    }
}

void menuStatus_drawFrame(void) {
    RECT *r = &s_menuStatus.win;

    ui_drawEtchedLine(r->x + 0x6D, r->y + 0x63, 0x1E, r->w - 0x71);
    ui_drawEtchedLine(r->x + 0x6B, r->y + 0x38, 0x1E, 0x3E - r->h);
    ui_drawEtchedLine(r->x + 4, r->y + 0x36, 0x1E, r->w - 9);
    win_drawBox(r, 0x1E, 2);
    win_drawShadow(r, 0x1E, 2);
}

void func_801EF9B4(void) {
    fade_isBusy();
}

void menuStatus_closeAbilityPages(void) {
    s32 i;

    if (s_menuStatus.pagesOpen != 0) {
        s_menuStatus.state = STATUS_STATE_BUSY;
        for (i = 0; i < s_menuStatus.pagesOpen; i++) {
            if (s_menuStatus.pageOpen[i] != 0) {
                s_menuStatusPageCloseTweens[i].delay = s_menuStatus.pagesOpen - 1 - i;
                tween_start(&s_menuStatus.pageWins[i], &s_menuStatusPageCloseTweens[i], 0);
            }
        }
        snd_playSe(SE_PAGE);
    }
}

void menuStatus_onPageOpened(void) {
    s_menuStatus.state = STATUS_STATE_PAGES;
    if (s_menuStatus.pagesOpen != 0) {
        s_menuStatus.pageOnTop[s_menuStatus.pagesOpen - 1] = 0;
    }
    s_menuStatus.pagesOpen++;
}

void menuStatus_onPagesClosed(void) {
    s32 i;

    s_menuStatus.state = STATUS_STATE_MAIN;
    for (i = 0; i < 6; i++) {
        s_menuStatus.pageOnTop[i] = 0;
        s_menuStatus.pageOpen[i] = 0;
    }
    s_menuStatus.pagesOpen = 0;
}

void menuStatus_onMemberSwitched(void) {
    s_menuStatus.state = STATUS_STATE_MAIN;
}

void menuStatus_updateHelp(void) {
    s32 i;

    g_menuHelp.x = 0xD8;
    g_menuHelp.y = 0x52;
    g_menuHelp.otz = 0xA;
    g_menuHelp.text = text_get(STATUS_TEXT_HELP);
    for (i = 0; i < 7; i++) {
        text_setStringArg(i, s_menuStatusHelpArgs[i].str);
        text_setIconArg(i, s_menuStatusHelpArgs[i].val);
    }
    text_setStringArg(i, s_menuStatusHelpArg7);
}

INCLUDE_RODATA("asm/us/ovl_38/nonmatchings/ovl_38", g_statusMenuText);
