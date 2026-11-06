#include "ovl_38.h"

void menuStatus_init(void) {
    s32 i;
    RECT *winRect;

    s_menuStatus.slot = STATUS_NO_SLOT;
    s_menuStatus.state = STATUS_STATE_MAIN;
    s_menuStatus.pagesOpen = 0;
    for (i = 0; i < (s32)ARRAY_COUNT(s_menuStatus.pageOpen); i++) {
        s_menuStatus.pageOnTop[i] = 0;
        s_menuStatus.pageOpen[i] = 0;
    }
    text_setBank(g_statusMenuText);
    ui_initMemberSwitch(&s_menuStatus.header, g_menuScreen.member, menuStatus_onMemberSwitched);
    menuStatus_setMember(g_menuScreen.member, 0);
    winRect = &s_menuStatus.win;
    winRect->x = STATUS_WIN_X;
    winRect->y = STATUS_WIN_Y;
    winRect->w = STATUS_WIN_W;
    winRect->h = STATUS_WIN_H;
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
    x = w->x + STATUS_ICONBOX_X;
    y = w->y + STATUS_ICONBOX_Y;
    setRECT(&iconBox, x, y, STATUS_ICONBOX_W, STATUS_ICONBOX_H);
    ui_drawStones(x + STATUS_STONES_DX, y + STATUS_STONES_DY, STATUS_OTZ_FRAME, chr->stones, chr->maxStones);
    ui_drawMemberHeader(w->x + STATUS_HEADER_X, w->y + STATUS_HEADER_Y, STATUS_OTZ_CONTENT, (MemberSwitch *)&s_menuStatus.header);
    ui_drawStatusIcons(iconBox.x + 4, iconBox.y + 4, STATUS_OTZ_CONTENT, chr->status, 0);
    win_drawBox(&iconBox, STATUS_OTZ_CONTENT, WIN_STYLE_LINE);
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
    cmdBox.w = STATUS_CMDBOX_W;
    cmdBox.h = STATUS_CMDBOX_H;
    cmdBox.x = w->x + STATUS_CMDBOX_X;
    cmdBox.y = w->y + STATUS_CMDBOX_Y;
    s_menuStatusCmds[1] = s_menuStatusCharCmds[chr->info.b.row][0];
    s_menuStatusCmds[2] = s_menuStatusCharCmds[chr->info.b.row][1];
    for (i = 0; i < (s32)ARRAY_COUNT(s_menuStatusCmds); i++) {
        tp = cmdTable.v;
        y = i * STATUS_CMD_ROW_H + STATUS_CMD_TEXT_Y;
        off = s_menuStatusCmds[i] * sizeof(StatusEntry);
        k = ((StatusEntry *)((u8 *)tp + off))->nameOffset;
        s_menuStatus.prim = text_buildBlock(cmdBox.x + STATUS_CMD_TEXT_X, cmdBox.y + y, g_ot + STATUS_OTZ_CMDS, pool.s + k, TEXT_PAL_WHITE, s_menuStatus.prim);
    }
    ui_drawLabel(cmdBox.x + WIN_TITLE_X, cmdBox.y + WIN_TITLE_Y, STATUS_OTZ_CMDS, g_labelCommand, UI_LABEL_NORMAL);
    win_drawBox(&cmdBox, STATUS_OTZ_CMDS, WIN_STYLE_BEVEL);
    win_drawShadow(&cmdBox, STATUS_OTZ_CMDS, WIN_STYLE_BEVEL);
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
    y = STATUS_BASE_STAT_Y;
    for (i = 0; i < (s32)ARRAY_COUNT(s_menuStatusBaseStatText); i++) {
        s_menuStatus.prim = text_buildBlock(winRect->x + STATUS_STAT_LABEL_X, winRect->y + (i * STATUS_STAT_ROW_H + y) + STATUS_STAT_LABEL_DY, g_ot + STATUS_OTZ_CONTENT, text_get(s_menuStatusBaseStatText[i]), TEXT_PAL_WHITE, s_menuStatus.prim);
    }
    y = STATUS_BATTLE_STAT_Y;
    for (i = 0; i < (s32)ARRAY_COUNT(s_menuStatusBattleStatText); i++) {
        s_menuStatus.prim = text_buildBlock(winRect->x + STATUS_STAT_LABEL_X, winRect->y + (i * STATUS_STAT_ROW_H + y) + STATUS_STAT_LABEL_DY, g_ot + STATUS_OTZ_CONTENT, text_get(s_menuStatusBattleStatText[i]), TEXT_PAL_WHITE, s_menuStatus.prim);
    }
    text_formatNumber(chr->speed, numText, 2, TEXT_NUM_ASCII);
    ui_drawSmallDigits(winRect->x + STATUS_BASE_STAT_X, winRect->y + STATUS_BASE_STAT_Y, STATUS_OTZ_CONTENT, numText, TEXT_PAL_WHITE);
    text_formatNumber(chr->strength, numText, 2, TEXT_NUM_ASCII);
    ui_drawSmallDigits(winRect->x + STATUS_BASE_STAT_X, winRect->y + STATUS_BASE_STAT_Y + STATUS_STAT_ROW_H, STATUS_OTZ_CONTENT, numText, TEXT_PAL_WHITE);
    text_formatNumber(chr->magic, numText, 2, TEXT_NUM_ASCII);
    ui_drawSmallDigits(winRect->x + STATUS_BASE_STAT_X, winRect->y + STATUS_BASE_STAT_Y + 2 * STATUS_STAT_ROW_H, STATUS_OTZ_CONTENT, numText, TEXT_PAL_WHITE);
    text_formatNumber(chr->spirit, numText, 2, TEXT_NUM_ASCII);
    ui_drawSmallDigits(winRect->x + STATUS_BASE_STAT_X, winRect->y + STATUS_BASE_STAT_Y + 3 * STATUS_STAT_ROW_H, STATUS_OTZ_CONTENT, numText, TEXT_PAL_WHITE);
    text_formatNumber(g_weaponTable[chr->equip[0]].attack, numText, 3, TEXT_NUM_ASCII);
    ui_drawSmallDigits(winRect->x + STATUS_BATTLE_STAT_X, winRect->y + STATUS_BATTLE_STAT_Y, STATUS_OTZ_CONTENT, numText, TEXT_PAL_WHITE);
    text_formatNumber(chr->defense, numText, 3, TEXT_NUM_ASCII);
    ui_drawSmallDigits(winRect->x + STATUS_BATTLE_STAT_X, winRect->y + STATUS_BATTLE_STAT_Y + STATUS_STAT_ROW_H, STATUS_OTZ_CONTENT, numText, TEXT_PAL_WHITE);
    text_formatNumber(chr->evade, numText, 3, TEXT_NUM_ASCII);
    ui_drawSmallDigits(winRect->x + STATUS_BATTLE_STAT_X, winRect->y + STATUS_BATTLE_STAT_Y + 2 * STATUS_STAT_ROW_H, STATUS_OTZ_CONTENT, numText, TEXT_PAL_WHITE);
    text_formatNumber(chr->magicDef, numText, 3, TEXT_NUM_ASCII);
    ui_drawSmallDigits(winRect->x + STATUS_BATTLE_STAT_X, winRect->y + STATUS_BATTLE_STAT_Y + 3 * STATUS_STAT_ROW_H, STATUS_OTZ_CONTENT, numText, TEXT_PAL_WHITE);
    text_formatNumber(chr->magicEva, numText, 3, TEXT_NUM_ASCII);
    ui_drawSmallDigits(winRect->x + STATUS_BATTLE_STAT_X, winRect->y + STATUS_BATTLE_STAT_Y + 4 * STATUS_STAT_ROW_H, STATUS_OTZ_CONTENT, numText, TEXT_PAL_WHITE);
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
        st->prim = text_buildBlock(winRect->x + STATUS_EXP_LABEL_X, winRect->y + STATUS_TRANCE_LABEL_Y, &g_ot[STATUS_OTZ_CONTENT], text_get(STATUS_TEXT_TRANCE), TEXT_PAL_WHITE, st->prim);
        ui_drawGauge(winRect->x + STATUS_TRANCE_GAUGE_X, winRect->y + STATUS_TRANCE_GAUGE_Y, STATUS_OTZ_CONTENT, chr->trance, 0xFF, 1, 1);
    }
    text_drawBlock(winRect->x + STATUS_EXP_LABEL_X, winRect->y + STATUS_EXP_LABEL_Y, STATUS_OTZ_CONTENT, text_get(STATUS_TEXT_EXP), TEXT_PAL_WHITE);
    text_drawBlock(winRect->x + STATUS_EXP_LABEL_X, winRect->y + STATUS_NEXT_LABEL_Y, STATUS_OTZ_CONTENT, text_get(STATUS_TEXT_NEXT), TEXT_PAL_WHITE);
    text_formatNumber(chr->exp, numText, 7, TEXT_NUM_ASCII);
    ui_drawSmallDigits(winRect->x + STATUS_EXP_X, winRect->y + STATUS_EXP_Y, STATUS_OTZ_CONTENT, numText, TEXT_PAL_WHITE);
    if (chr->level >= LEVEL_MAX) {
        next = chr->exp;
    } else {
        next = g_levelExp[chr->level];
    }
    text_formatNumber(next - chr->exp, numText, 6, TEXT_NUM_ASCII);
    ui_drawSmallDigits(winRect->x + STATUS_NEXT_X, winRect->y + STATUS_NEXT_Y, STATUS_OTZ_CONTENT, numText, TEXT_PAL_WHITE);
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
        s_menuStatus.prim = ui_buildSprite(winRect->x + STATUS_EQUIP_SLOT_X, winRect->y + STATUS_EQUIP_Y + i * STATUS_EQUIP_ROW_H + 1, &g_ot[STATUS_OTZ_CONTENT], i + UI_SPRITE_EQUIP_SLOTS, 0, spriteW, s_menuStatus.prim);
        s_menuStatus.prim = text_buildSmall(winRect->x + STATUS_EQUIP_COLON_X, winRect->y + STATUS_EQUIP_Y + i * STATUS_EQUIP_ROW_H + 3, &g_ot[STATUS_OTZ_CONTENT], s_menuStatusColon, TEXT_PAL_WHITE, s_menuStatus.prim);
        id = chr->equip[i];
        if (id != ITEM_NONE) {
            ui_drawItemIcon(winRect->x + STATUS_EQUIP_ICON_X, winRect->y + STATUS_EQUIP_Y + i * STATUS_EQUIP_ROW_H - 1, STATUS_OTZ_CONTENT, id, 1);
            s_menuStatus.prim = text_buildBlock(winRect->x + STATUS_EQUIP_NAME_X, winRect->y + STATUS_EQUIP_Y + i * STATUS_EQUIP_ROW_H - 1, &g_ot[STATUS_OTZ_CONTENT], g_itemNames + g_itemTable[id].name, TEXT_PAL_WHITE, s_menuStatus.prim);
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
                    menuStatus_drawAbilityRow(pageRect->x + STATUS_PAGE_ROW_X, pageRect->y + STATUS_PAGE_ROW_Y + j * STATUS_PAGE_ROW_H, STATUS_OTZ_PAGE0 - i, base + j, s_menuStatus.pageOnTop[i]);
                }
            }
            z = STATUS_OTZ_PAGE0 - i;
            ui_drawLabel(pageRect->x + WIN_TITLE_X, pageRect->y + WIN_TITLE_Y, z, g_winLabelStrings.abilityList, UI_LABEL_NORMAL);
            if (s_menuStatus.pageOnTop[i] != 0) {
                s_menuStatusPageLabel[1] = i + '1';
                ui_drawLabel(pageRect->x + STATUS_PAGE_NO_X, pageRect->y + WIN_TITLE_Y, z, g_winLabelStrings.labelNo, UI_LABEL_NORMAL);
                ui_drawLabel(pageRect->x + STATUS_PAGE_NUM_X, pageRect->y + WIN_TITLE_Y, z, (s32)s_menuStatusPageLabel, UI_LABEL_NORMAL);
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
        s_menuStatus.prim = text_buildBlock(x + STATUS_ABIL_NAME_DX, y, g_ot + z, name, TEXT_PAL_WHITE, s_menuStatus.prim);
    }
    if (ok) {
        s_menuStatus.prim = ui_buildApGauge(x + STATUS_ABIL_GAUGE_DX, y + STATUS_ABIL_GAUGE_DY, g_ot + z, cur, max, flag, 0, s_menuStatus.prim);
    } else {
        s_menuStatus.prim = ui_buildApGauge(x + STATUS_ABIL_GAUGE_DX, y + STATUS_ABIL_GAUGE_DY, g_ot + z, 1, 1, 0, 0, s_menuStatus.prim);
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

    for (i = 0; i < (s32)ARRAY_COUNT(s_menuStatus.pageOpen); i++) {
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

    ui_drawEtchedLine(winRect->x + STATUS_DIV_EQUIP_X, winRect->y + STATUS_DIV_EQUIP_Y, STATUS_OTZ_FRAME, winRect->w - 0x71);
    ui_drawEtchedLine(winRect->x + STATUS_DIV_COLUMN_X, winRect->y + STATUS_DIV_COLUMN_Y, STATUS_OTZ_FRAME, 0x3E - winRect->h);
    ui_drawEtchedLine(winRect->x + 4, winRect->y + STATUS_DIV_HEADER_Y, STATUS_OTZ_FRAME, winRect->w - 9);
    win_drawBox(winRect, STATUS_OTZ_FRAME, WIN_STYLE_BEVEL);
    win_drawShadow(winRect, STATUS_OTZ_FRAME, WIN_STYLE_BEVEL);
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
    for (i = 0; i < (s32)ARRAY_COUNT(s_menuStatus.pageOpen); i++) {
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

    g_menuHelp.x = STATUS_HELP_X;
    g_menuHelp.y = STATUS_HELP_Y;
    g_menuHelp.otz = STATUS_HELP_OTZ;
    g_menuHelp.text = text_get(STATUS_TEXT_HELP);
    for (i = 0; i < (s32)ARRAY_COUNT(s_menuStatusHelpArgs); i++) {
        text_setStringArg(i, s_menuStatusHelpArgs[i].str);
        text_setIconArg(i, s_menuStatusHelpArgs[i].val);
    }
    text_setStringArg(i, s_menuStatusHelpArg7);
}

INCLUDE_RODATA("asm/us/ovl_38/nonmatchings/ovl_38", g_statusMenuText);
