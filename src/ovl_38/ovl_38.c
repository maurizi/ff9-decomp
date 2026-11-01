#include "ovl_38.h"

void menuStatus_init(void) {
    s32 i;
    RECT *winRect;

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
    winRect = &s_menuStatus.win;
    winRect->x = 8;
    winRect->y = 0x26;
    winRect->w = 0x12C;
    winRect->h = 0xB6;
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
    RECT iconBox;
    Character *chr;
    RECT *w;
    s16 x;
    s16 y;

    w = &s_menuStatus.win;
    chr = g_gameState->gameData->party[s_menuStatus.slot];
    x = w->x + 0x72;
    y = w->y + 4;
    setRECT(&iconBox, x, y, 0x5C, 0x12);
    ui_drawStones(x + 0x19, y + 0x14, 0x1E, chr->stones, chr->maxStones);
    ui_drawMemberHeader(w->x + 8, w->y + 6, 0x1D, (MemberSwitch *)&s_menuStatus.header);
    ui_drawStatusIcons(iconBox.x + 4, iconBox.y + 4, 0x1D, chr->status, 0);
    win_drawBox(&iconBox, 0x1D, WIN_STYLE_LINE);
}

void menuStatus_drawCommands(void) {
    RECT cmdBox;
    StatusTable cmdTable;
    StatusPool pool;
    RECT *w;
    Character *chr;
    s32 i;
    s32 y;
    s32 k;
    s32 off;
    StatusEntry *tp;

    w = &s_menuStatus.win;
    chr = g_gameState->gameData->party[s_menuStatus.slot];
    cmdTable = s_menuStatusCmdTable;
    pool = s_menuStatusCmdNames;
    cmdBox.w = 0x42;
    cmdBox.h = 0x48;
    cmdBox.x = w->x + 0xE4;
    cmdBox.y = w->y + 8;
    s_menuStatusCmds[1] = s_menuStatusCharCmds[chr->info.b.row][0];
    s_menuStatusCmds[2] = s_menuStatusCharCmds[chr->info.b.row][1];
    for (i = 0; i < 4; i++) {
        tp = cmdTable.v;
        y = i * 15 + 6;
        off = s_menuStatusCmds[i] * sizeof(StatusEntry);
        k = ((StatusEntry *)((u8 *)tp + off))->nameOffset;
        s_menuStatus.prim = text_buildBlock(cmdBox.x + 0xC, cmdBox.y + y, g_ot + 25, pool.s + k, TEXT_PAL_WHITE, s_menuStatus.prim);
    }
    ui_drawLabel(cmdBox.x + 6, cmdBox.y - 3, 0x19, g_labelCommand, UI_LABEL_NORMAL);
    win_drawBox(&cmdBox, 0x19, WIN_STYLE_BEVEL);
    win_drawShadow(&cmdBox, 0x19, WIN_STYLE_BEVEL);
}

void menuStatus_drawStats(void) {
    u8 numText[16];
    RECT *winRect;
    StatusMenu *menu;
    Character *chr;
    s32 y;
    s32 i;

    menu = &s_menuStatus;
    winRect = &menu->win;
    chr = g_gameState->gameData->party[menu->slot];
    y = 0x3D;
    for (i = 0; i < 4; i++) {
        s_menuStatus.prim = text_buildBlock(winRect->x + 0xA, winRect->y + (i * 13 + y) - 4, g_ot + 0x1D, text_get(s_menuStatusBaseStatText[i]), TEXT_PAL_WHITE, s_menuStatus.prim);
    }
    y = 0x72;
    for (i = 0; i < 5; i++) {
        s_menuStatus.prim = text_buildBlock(winRect->x + 0xA, winRect->y + (i * 13 + y) - 4, g_ot + 0x1D, text_get(s_menuStatusBattleStatText[i]), TEXT_PAL_WHITE, s_menuStatus.prim);
    }
    text_formatNumber(chr->speed, numText, 2, TEXT_NUM_ASCII);
    ui_drawSmallDigits(winRect->x + 0x58, winRect->y + 0x3D, 0x1D, numText, TEXT_PAL_WHITE);
    text_formatNumber(chr->strength, numText, 2, TEXT_NUM_ASCII);
    ui_drawSmallDigits(winRect->x + 0x58, winRect->y + 0x4A, 0x1D, numText, TEXT_PAL_WHITE);
    text_formatNumber(chr->magic, numText, 2, TEXT_NUM_ASCII);
    ui_drawSmallDigits(winRect->x + 0x58, winRect->y + 0x57, 0x1D, numText, TEXT_PAL_WHITE);
    text_formatNumber(chr->spirit, numText, 2, TEXT_NUM_ASCII);
    ui_drawSmallDigits(winRect->x + 0x58, winRect->y + 0x64, 0x1D, numText, TEXT_PAL_WHITE);
    text_formatNumber(g_weaponTable[chr->equip[0]].attack, numText, 3, TEXT_NUM_ASCII);
    ui_drawSmallDigits(winRect->x + 0x51, winRect->y + 0x72, 0x1D, numText, TEXT_PAL_WHITE);
    text_formatNumber(chr->defense, numText, 3, TEXT_NUM_ASCII);
    ui_drawSmallDigits(winRect->x + 0x51, winRect->y + 0x7F, 0x1D, numText, TEXT_PAL_WHITE);
    text_formatNumber(chr->evade, numText, 3, TEXT_NUM_ASCII);
    ui_drawSmallDigits(winRect->x + 0x51, winRect->y + 0x8C, 0x1D, numText, TEXT_PAL_WHITE);
    text_formatNumber(chr->magicDef, numText, 3, TEXT_NUM_ASCII);
    ui_drawSmallDigits(winRect->x + 0x51, winRect->y + 0x99, 0x1D, numText, TEXT_PAL_WHITE);
    text_formatNumber(chr->magicEva, numText, 3, TEXT_NUM_ASCII);
    ui_drawSmallDigits(winRect->x + 0x51, winRect->y + 0xA6, 0x1D, numText, TEXT_PAL_WHITE);
}

void menuStatus_drawExp(void) {
    u8 numText[16];
    RECT *winRect;
    StatusMenu *st;
    Character *chr;
    u32 next;
    s32 bit;
    u8 show;

    winRect = &s_menuStatus.win;
    st = &s_menuStatus;
    chr = g_gameState->gameData->party[st->slot];
    show = 0;
    if (g_evtGlobalVars.b[EVT_GB_TRANCE_ENABLED] != 0) {
        bit = chr->classFlags & CHR_CLASS_GUEST;
        show = bit == 0;
    }
    if (show) {
        st->prim = text_buildBlock(winRect->x + 0x74, winRect->y + 0x39, &g_ot[0x1D], text_get(STATUS_TEXT_TRANCE), TEXT_PAL_WHITE, st->prim);
        ui_drawGauge(winRect->x + 0xB4, winRect->y + 0x3E, 0x1D, chr->trance, 0xFF, 1, 1);
    }
    text_drawBlock(winRect->x + 0x74, winRect->y + 0x46, 0x1D, text_get(STATUS_TEXT_EXP), TEXT_PAL_WHITE);
    text_drawBlock(winRect->x + 0x74, winRect->y + 0x53, 0x1D, text_get(STATUS_TEXT_NEXT), TEXT_PAL_WHITE);
    text_formatNumber(chr->exp, numText, 7, TEXT_NUM_ASCII);
    ui_drawSmallDigits(winRect->x + 0xAC, winRect->y + 0x4A, 0x1D, numText, TEXT_PAL_WHITE);
    if (chr->level >= 99) {
        next = chr->exp;
    } else {
        next = g_levelExp[chr->level];
    }
    text_formatNumber(next - chr->exp, numText, 6, TEXT_NUM_ASCII);
    ui_drawSmallDigits(winRect->x + 0xB3, winRect->y + 0x57, 0x1D, numText, TEXT_PAL_WHITE);
}

INCLUDE_RODATA("asm/us/ovl_38/nonmatchings/ovl_38", s_menuStatusCmdTable);

INCLUDE_RODATA("asm/us/ovl_38/nonmatchings/ovl_38", s_menuStatusCmdNames);

INCLUDE_RODATA("asm/us/ovl_38/nonmatchings/ovl_38", s_menuStatusColon);

void menuStatus_drawEquipment(void) {
    RECT *winRect = &s_menuStatus.win;
    StatusMenu *st;
    Character *chr;
    s32 i;
    u8 id;
    u8 spriteW[8];

    st = &s_menuStatus;
    chr = g_gameState->gameData->party[st->slot];
    for (i = 0; i < EQUIP_SLOT_COUNT; i++) {
        s_menuStatus.prim = ui_buildSprite(winRect->x + 0x74, winRect->y + 0x68 + i * 15 + 1, &g_ot[0x1D], i + UI_SPRITE_EQUIP_SLOTS, 0, spriteW, s_menuStatus.prim);
        s_menuStatus.prim = text_buildSmall(winRect->x + 0x84, winRect->y + 0x68 + i * 15 + 3, &g_ot[0x1D], s_menuStatusColon, TEXT_PAL_WHITE, s_menuStatus.prim);
        id = chr->equip[i];
        if (id != ITEM_NONE) {
            ui_drawItemIcon(winRect->x + 0x8D, winRect->y + 0x68 + i * 15 - 1, 0x1D, id, 1);
            s_menuStatus.prim = text_buildBlock(winRect->x + 0x9D, winRect->y + 0x68 + i * 15 - 1, &g_ot[0x1D], g_itemNames + g_itemTable[id].name, TEXT_PAL_WHITE, s_menuStatus.prim);
        }
    }
}

void menuStatus_drawAbilityPages(void) {
    s32 i, j;
    s32 base = 0;
    RECT *pageRect;
    s32 z;

    for (i = 0; i < s_menuStatus.pageCount; i++) {
        if (s_menuStatus.pageOpen[i] != 0) {
            pageRect = &s_menuStatus.pageWins[i].rect;
            for (j = 0; j < STATUS_PAGE_ROWS; j++) {
                if (base + j < s_menuStatus.abilityCount) {
                    menuStatus_drawAbilityRow(pageRect->x + 0xA, pageRect->y + 8 + j * 15, 0x14 - i, base + j, s_menuStatus.pageOnTop[i]);
                }
            }
            z = 0x14 - i;
            ui_drawLabel(pageRect->x + 6, pageRect->y - 3, z, g_winLabelStrings.abilityList, UI_LABEL_NORMAL);
            if (s_menuStatus.pageOnTop[i] != 0) {
                s_menuStatusPageLabel[1] = i + 0x31;
                ui_drawLabel(pageRect->x + 0x90, pageRect->y - 3, z, g_winLabelStrings.labelNo, UI_LABEL_NORMAL);
                ui_drawLabel(pageRect->x + 0x9A, pageRect->y - 3, z, (s32)s_menuStatusPageLabel, UI_LABEL_NORMAL);
            }
            win_drawBox(pageRect, z, WIN_STYLE_BEVEL);
            win_drawShadow(pageRect, z, WIN_STYLE_BEVEL);
        }
        base += STATUS_PAGE_ROWS;
    }
}

void menuStatus_drawAbilityRow(s32 x, s32 y, s32 z, s32 i, u8 draw) {
    Character *chr;
    u8 *name;
    u8 ok;
    s32 id;
    s32 max;
    s32 cur;
    s32 icon;
    u8 flag;
    ActiveAbility *act;
    Unk8006D128 *sup;

    chr = g_gameState->gameData->party[s_menuStatus.slot];
    ok = chr_canLearnAbilities(chr);
    if (ok && chr->ap[i] == 0) {
        return;
    }
    id = g_abilityRows[chr->info.b.row][i].id;
    max = g_abilityRows[chr->info.b.row][i].ap;
    if (id == 0) {
        return;
    }
    cur = chr->ap[i];
    icon = UI_SPRITE_ABIL_ACTION;
    if (id < ABILITY_SUPPORT_FIRST) {
        act = &s_menuStatusActionAbilities[id];
        name = s_menuStatusActionNames + act->nameOffset;
        flag = !(act->flags & 2);
    } else {
        sup = &g_supportAbilities[id - ABILITY_SUPPORT_FIRST];
        name = s_menuStatusSupportNames + sup->name;
        if (chr_testSupportBit(chr->supportAbilities, id)) {
            icon = UI_SPRITE_ABIL_SUPPORT_ON;
        } else {
            icon = UI_SPRITE_ABIL_SUPPORT_OFF;
        }
        flag = 1;
    }
    if (draw) {
        ui_drawSprite(x, y, z, icon, 0);
        s_menuStatus.prim = text_buildBlock(x + 0x10, y, g_ot + z, name, TEXT_PAL_WHITE, s_menuStatus.prim);
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
    Character *chr;
    s32 i;

    chr = g_gameState->gameData->party[slot];
    ui_startMemberSwitch(&s_menuStatus.header, s_menuStatus.slot, slot, dir);
    if (dir != 0) {
        s_menuStatus.state = STATUS_STATE_WAIT;
    }
    s_menuStatus.slot = slot;
    s_menuStatus.pageCount = 0;
    s_menuStatus.abilityCount = 0;
    if ((s32)(chr->info.b.row) < 0x10) {
        for (i = 0; i < ABILITY_MAX; i++) {
            if (g_abilityRows[chr->info.b.row][i].id != 0) {
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
    u8 *primBase = s_menuStatus.prim - 0x4000;

    s_menuStatus.primFree = s_menuStatus.primBufs[g_gameState->gameData->drawBuf ^ 1] - primBase;
    s_menuStatus.prim = s_menuStatus.primBufs[g_gameState->gameData->drawBuf];
    menuStatus_drawFrame();
    menuStatus_drawHeader();
    menuStatus_drawCommands();
    menuStatus_drawStats();
    menuStatus_drawExp();
    menuStatus_drawEquipment();
    menuStatus_drawAbilityPages();
    menuStatus_pollFade();
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
    RECT *winRect = &s_menuStatus.win;

    ui_drawEtchedLine(winRect->x + 0x6D, winRect->y + 0x63, 0x1E, winRect->w - 0x71);
    ui_drawEtchedLine(winRect->x + 0x6B, winRect->y + 0x38, 0x1E, 0x3E - winRect->h);
    ui_drawEtchedLine(winRect->x + 4, winRect->y + 0x36, 0x1E, winRect->w - 9);
    win_drawBox(winRect, 0x1E, WIN_STYLE_BEVEL);
    win_drawShadow(winRect, 0x1E, WIN_STYLE_BEVEL);
}

void menuStatus_pollFade(void) {
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
