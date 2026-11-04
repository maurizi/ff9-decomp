#include "ovl_0c.h"

void menuMain_initPanels(void) {
    u8 *name;
    u8 *locName;
    s32 w;
    s32 i;

    locName = g_gameState->gameData->locationName;
    if (locName != NULL && *locName != 0xFF) {
        name = locName;
    } else {
        name = text_get(MENU_MAIN_TEXT_NO_LOCATION);
    }
    w = text_measureStyle(name, 0) + 0x10;
    if (w < 0x44) {
        w = 0x44;
    }
    s_menuMainPanelRects[5].w = w;
    s_menuMainPanelRects[5].x = 0x134 - w;
    for (i = 0; i < 6; i++) {
        s_menuMain.panels[i].rect = s_menuMainPanelRects[i];
        tween_start(&s_menuMain.panels[i], NULL, 0);
    }
    if (g_menuScreen.slideIn != 0) {
        tween_start(&s_menuMain.panels[0], &s_menuMainSlideIn[0], 1);
        tween_start(&s_menuMain.panels[1], &s_menuMainSlideIn[1], 1);
        tween_start(&s_menuMain.panels[2], &s_menuMainSlideIn[2], 1);
        tween_start(&s_menuMain.panels[3], &s_menuMainSlideIn[3], 1);
    }
}

void menuMain_inputCommand(void) {
    if (g_padRepeat & BTN_CIRCLE) {
        snd_playSe(SE_CANCEL);
        menuScreen_close();
    } else if (g_padRepeat & BTN_CROSS) {
        menuMain_selectCommand();
    } else if (g_padRepeat & PAD_UP) {
        snd_playSe(SE_CURSOR);
        s_menuMain.cursor = s_menuMain.cursor != 0 ? s_menuMain.cursor - 1 : 6;
    } else if (g_padRepeat & PAD_DOWN) {
        snd_playSe(SE_CURSOR);
        s_menuMain.cursor = s_menuMain.cursor < 6 ? s_menuMain.cursor + 1 : 0;
    }
}

void menuMain_inputMember(void) {
    MenuScreen *screen;
    s32 old;
    s32 new;

    if (g_padRepeat & BTN_CIRCLE) {
        snd_playSe(SE_CANCEL);
        s_menuMain.state = MENUMAIN_COMMAND;
        return;
    } else if (g_padRepeat & BTN_CROSS) {
        snd_playSe(SE_CURSOR);
        menuScreen_open(s_menuMain.cursor + 1);
        return;
    } else if (g_padRepeat & PAD_UP) {
        screen = &g_menuScreen;
        old = screen->member;
        new = party_getPrevMember(old);
    } else if (g_padRepeat & PAD_DOWN) {
        screen = &g_menuScreen;
        old = screen->member;
        new = party_getNextMember(old);
    } else {
        return;
    }
    if (old != new) {
        snd_playSe(SE_CURSOR);
        old = new;
        screen->member = old;
    }
}

void menuMain_inputOrder(void) {
    if (g_padRepeat & BTN_CIRCLE) {
        snd_playSe(SE_CANCEL);
        if (s_menuMain.orderPicked == 0) {
            s_menuMain.state = MENUMAIN_COMMAND;
        } else {
            s_menuMain.orderPicked = 0;
        }
    } else if (g_padRepeat & PAD_UP) {
        snd_playSe(SE_CURSOR);
        if (s_menuMain.orderPicked == 0) {
            s_menuMain.orderCursor = s_menuMain.orderCursor != 0 ? s_menuMain.orderCursor - 1 : 3;
        } else {
            s_menuMain.orderTarget = s_menuMain.orderTarget != 0 ? s_menuMain.orderTarget - 1 : 3;
        }
    } else if (g_padRepeat & PAD_DOWN) {
        snd_playSe(SE_CURSOR);
        if (s_menuMain.orderPicked == 0) {
            s_menuMain.orderCursor = s_menuMain.orderCursor < 3 ? s_menuMain.orderCursor + 1 : 0;
        } else {
            s_menuMain.orderTarget = s_menuMain.orderTarget < 3 ? s_menuMain.orderTarget + 1 : 0;
        }
    } else if (g_padRepeat & BTN_CROSS) {
        if (s_menuMain.orderPicked == 0) {
            snd_playSe(SE_CURSOR);
            s_menuMain.orderPicked = 1;
            s_menuMain.orderTarget = s_menuMain.orderCursor;
        } else {
            snd_playSe(SE_CURSOR);
            menuMain_reorder(s_menuMain.orderCursor, s_menuMain.orderTarget);
            s_menuMain.orderPicked = 0;
            s_menuMain.orderCursor = s_menuMain.orderTarget;
        }
    }
}

void menuMain_reorder(s32 from, s32 to) {
    Character *chr;
    Character *swapped;

    if (from == to) {
        chr = g_gameState->gameData->party[to];
        if (chr != NULL) {
            chr->info.b.battleRow ^= 1;
        }
    } else {
        swapped = g_gameState->gameData->party[s_menuMain.orderCursor];
        g_gameState->gameData->party[s_menuMain.orderCursor] = g_gameState->gameData->party[s_menuMain.orderTarget];
        g_gameState->gameData->party[s_menuMain.orderTarget] = swapped;
        menuMain_swapPortraits(from, to);
        if (g_gameState->gameData->party[g_menuScreen.member] == NULL) {
            g_menuScreen.member = party_getFirstMember();
        }
    }
}

void menuMain_swapPortraits(s32 slotA, s32 slotB) {
    RECT a0;
    RECT a1;
    RECT a2;
    RECT b0;
    RECT b1;
    RECT b2;

    ui_getPartyPortraitRects(slotA, &a0, &a1, &a2);
    ui_getPartyPortraitRects(slotB, &b0, &b1, &b2);
    StoreImage(&a0, g_portraitSavedVram);
    DrawSync(0);
    MoveImage(&a1, 0x3C0, 0x1B0);
    DrawSync(0);
    MoveImage(&a2, 0x3D0, 0x1B0);
    DrawSync(0);
    MoveImage(&b0, a0.x, a0.y);
    DrawSync(0);
    MoveImage(&b1, a1.x, a1.y);
    DrawSync(0);
    MoveImage(&b2, a2.x, a2.y);
    DrawSync(0);
    a1.x = 0x3C0;
    a1.y = 0x1B0;
    a2.x = 0x3D0;
    a2.y = 0x1B0;
    LoadImage(&b0, g_portraitSavedVram);
    DrawSync(0);
    MoveImage(&a1, b1.x, b1.y);
    DrawSync(0);
    MoveImage(&a2, b2.x, b2.y);
    DrawSync(0);
}

void menuMain_drawMember(s32 slot) {
    u8 numText[16];
    RECT *panelRect;
    Character *chr;
    s32 x;
    s32 color;
    s16 maxmp;

    chr = g_gameState->gameData->party[slot];
    panelRect = &s_menuMain.panels[slot].rect;
    x = panelRect->x + 8;
    if (!chr->info.b.battleRow) {
        x = panelRect->x + 0x16;
    }
    ui_drawPortrait(x, panelRect->y + 1, 0x14, slot, 0x2D, chr->hp == 0);
    text_drawBlock(panelRect->x + 0x3C, panelRect->y + 3, 0x14, chr->name, TEXT_PAL_WHITE);
    ui_drawStatusIcons(panelRect->x + 0x86, panelRect->y + 4, 0x14, chr->status, 0);
    text_formatNumber(chr->level, numText, 2, TEXT_NUM_ASCII);
    ui_drawSprite(panelRect->x + 0x3C, panelRect->y + 0x12, 0x14, UI_SPRITE_LV, 0);
    ui_drawSmallDigits(panelRect->x + 0x60, panelRect->y + 0x12, 0x14, numText, TEXT_PAL_WHITE);
    ui_drawSprite(panelRect->x + 0x3C, panelRect->y + 0x1B, 0x14, UI_SPRITE_HP, 0);
    text_formatNumber(chr->hp, numText, 4, TEXT_NUM_ASCII);
    numText[4] = '/';
    if (chr->hp != 0) {
        color = ((u16)(chr->maxHp / 6) >= chr->hp) * TEXT_PAL_YELLOW;
    } else {
        color = TEXT_PAL_RED;
    }
    text_formatNumber(chr->maxHp, &numText[5], TEXT_NUM_NO_PAD, TEXT_NUM_ASCII);
    ui_drawSmallDigits(panelRect->x + 0x52, panelRect->y + 0x1B, 0x14, numText, color);
    ui_drawSprite(panelRect->x + 0x3C, panelRect->y + 0x24, 0x14, UI_SPRITE_MP, 0);
    text_formatNumber(chr->mp, numText, 3, TEXT_NUM_ASCII);
    numText[3] = '/';
    maxmp = chr->maxMp;
    color = ((s16)(maxmp / 6) >= chr->mp) * TEXT_PAL_YELLOW;
    text_formatNumber(maxmp, &numText[4], TEXT_NUM_NO_PAD, TEXT_NUM_ASCII);
    ui_drawSmallDigits(panelRect->x + 0x59, panelRect->y + 0x24, 0x14, numText, color);
    ui_drawStones(panelRect->x + 0x9B, panelRect->y + 0x14, 0x14, chr->stones, chr->maxStones);
}

void menuMain_drawPortraitShade(RECT *rect, s32 otz) {
    TileWords *tile;

    setlen(g_primPtr, PRIM_LEN(TILE));
    tile = (TileWords *)g_primPtr;
    tile->rgbc = GPU_CODE(GPU_TILE | GPU_ABE);
    tile->xy = ((rect->x + 8) & 0xFFFF) | (rect->y << 16);
    tile->wh = 0x30002E;
    g_primPtr = (u8 *)(tile + 1);
    addPrim(&g_ot[otz], tile);
    SetDrawTPage(g_primPtr, 1, 1, 0);
    addPrim(&g_ot[otz], g_primPtr);
    g_primPtr += 8;
}

void menuMain_drawTimeGil(void) {
    u8 numText[16];
    s32 playTime;

    playTime = game_getPlayTime();
    ui_drawSprite(s_menuMainTimeGilRect->x + 6, s_menuMainTimeGilRect->y + 8, 0x13, UI_SPRITE_TIME, 0);
    ui_drawSprite(s_menuMainTimeGilRect->x + 6, s_menuMainTimeGilRect->y + 0x15, 0x13, UI_SPRITE_GIL, 0);
    ui_drawSmallDigits(s_menuMainTimeGilRect->x + 0x13, s_menuMainTimeGilRect->y + 9, 0x13, numText, text_formatTime(playTime, numText, 1));
    text_formatNumber(g_gameState->gameData->gil, numText, 7, TEXT_NUM_ASCII);
    strcat(numText, s_menuMainGilSuffix);
    ui_drawSmallDigits(s_menuMainTimeGilRect->x + 0x13, s_menuMainTimeGilRect->y + 0x16, 0x13, numText, TEXT_PAL_WHITE);
    ui_drawLabel(s_menuMainTimeGilRect->x + 6, s_menuMainTimeGilRect->y - 3, 0x13, g_labelTimeAndGil, UI_LABEL_NORMAL);
    win_drawBox(s_menuMainTimeGilRect, 0x14, WIN_STYLE_BEVEL);
}

void menuMain_drawLocation(void) {
    GameData *gd;

    ui_drawLabel(s_menuMainLocationRect->x + 8, s_menuMainLocationRect->y - 3, 0x13, g_labelLocation, UI_LABEL_NORMAL);
    win_drawBox(s_menuMainLocationRect, 0x14, WIN_STYLE_BEVEL);
    win_drawShadow(s_menuMainLocationRect, 0x14, WIN_STYLE_BEVEL);
    gd = g_gameState->gameData;
    if (gd->locationName != NULL && *gd->locationName != TEXT_END) {
        text_drawBlock(s_menuMainLocationRect->x + 8, s_menuMainLocationRect->y + 7, 0x13, gd->locationName, TEXT_PAL_WHITE);
    } else {
        text_drawBlock(s_menuMainLocationRect->x + 8, s_menuMainLocationRect->y + 7, 0x13, text_get(MENU_MAIN_TEXT_NO_LOCATION), TEXT_PAL_WHITE);
    }
}

void menuMain_drawCursor(void) {
    u8 blink;

    blink = (g_menuScreenFrame & 3) != 0;
    help_setCursorOnTop(s_menuMain.state - 1U >= 2);
    if (!fade_isBusy()) {
        if (s_menuMain.state == MENUMAIN_COMMAND || blink) {
            g_menuHelp.x = 0xE0;
            g_menuHelp.otz = 1;
            g_menuHelp.y = s_menuMain.cursor * 0xF + 0x11;
            help_drawCursor();
        }
        if (s_menuMain.state == MENUMAIN_MEMBER) {
            g_menuHelp.otz = 0xC;
            g_menuHelp.x = s_menuMain.panels[0].rect.x - 0xE;
            g_menuHelp.y = g_menuScreenMember * 0x2E + 0x20;
            help_drawCursor();
        }
        if (s_menuMain.state == MENUMAIN_ORDER) {
            if (s_menuMain.orderPicked == 1) {
                g_menuHelp.otz = 0xB;
                g_menuHelp.x = s_menuMain.panels[0].rect.x - 0xA;
                g_menuHelp.y = s_menuMain.orderTarget * 0x2E + 0x24;
                help_drawCursor();
                if (blink) {
                    ui_drawHand(s_menuMain.panels[0].rect.x - 0xE, s_menuMain.orderCursor * 0x2E + 0x20, 0xC);
                }
            } else if (s_menuMain.orderPicked == 0) {
                g_menuHelp.otz = 0xC;
                g_menuHelp.x = s_menuMain.panels[0].rect.x - 0xE;
                g_menuHelp.y = s_menuMain.orderCursor * 0x2E + 0x20;
                help_drawCursor();
            }
        }
    }
}

void menuMain_updateHelp(void) {
    g_menuHelp.text = 0;
    if (s_menuMain.state == MENUMAIN_COMMAND) {
        g_menuHelp.text = (u8 *)menuScreen_getCommandHelp(s_menuMain.cursor);
    } else if (s_menuMain.state == MENUMAIN_MEMBER) {
        if (s_menuMain.cursor + 1 == MENU_SCREEN_EQUIP) {
            menuMain_setEquipHelp(s_menuMain.cursor);
        } else {
            s32 textId = s_menuMain.cursor + 2;
            if (s_menuMain.cursor + 1 == MENU_SCREEN_STATUS) {
                menuMain_setExpHelp(textId);
            } else {
                g_menuHelp.text = text_get(textId);
            }
        }
    } else if (s_menuMain.state == MENUMAIN_ORDER) {
        g_menuHelp.text = text_get(s_menuMain.orderPicked + 0xB);
    }
}

void menuMain_setExpHelp(s32 unused) {
    Character *chr;
    u32 next;

    chr = g_gameState->gameData->party[g_menuScreenMember];
    if (chr != NULL) {
        if (chr->level >= LEVEL_MAX) {
            next = chr->exp;
        } else {
            next = g_levelExp[chr->level];
        }
        text_setStringArg(0, chr->name);
        text_formatNumber(chr->exp, s_menuMainExpText, 7, TEXT_NUM_ASCII);
        text_formatNumber(next - chr->exp, s_menuMainNextText, 7, TEXT_NUM_ASCII);
        text_setStringArg(1, s_menuMainExpLabel);
        text_setStringArg(2, s_menuMainExpText);
        text_setStringArg(3, s_menuMainNextLabel);
        text_setStringArg(4, s_menuMainUntilLabel);
        text_setStringArg(5, s_menuMainNextText);
        text_setStringArg(6, s_menuMainStatusLabel);
        text_setStringArg(7, chr->status != 0 ? s_menuMainStatusAbnormal : s_menuMainStatusNormal);
        g_menuHelpText = text_get(MENU_MAIN_TEXT_STATUS_HELP);
    }
}

void menuMain_selectCommand(void) {
    if (s_menuMain.cursor + 1 != MENU_SCREEN_ORDER) {
        if (menuScreen_needsMember(s_menuMain.cursor + 1) == 0) {
            menuScreen_open(s_menuMain.cursor + 1);
        } else {
            s_menuMain.state = MENUMAIN_MEMBER;
        }
    } else {
        s_menuMain.state = MENUMAIN_ORDER;
        s_menuMain.orderPicked = 0;
    }
    snd_playSe(SE_CURSOR);
}

void menuMain_onSlideInDone(void) {
    g_menuScreen.slideIn = 0;
}

void menuMain_setEquipHelp(u8 unused) {
    s32 i;
    s32 j;
    Character *chr;
    s32 id;

    i = 0;
    j = 5;
    chr = g_gameState->gameData->party[g_menuScreenMember];
    do {
        text_setStringArg(i, s_menuMainEquipLabels[i]);
        id = chr->equip[i];
        if (id != ITEM_NONE) {
            text_setStringArg(j, g_itemNames + g_itemTable[id].name);
        } else {
            text_setStringArg(j, text_get(MENU_MAIN_TEXT_EMPTY));
        }
        j++;
        text_setIconArg(i, id);
        i++;
    } while (i < EQUIP_SLOT_COUNT);
    g_menuHelp.text = text_get(MENU_MAIN_TEXT_EQUIP_HELP);
}

void menuMain_init(void) {
    RECT rect;

    if (g_menuScreen.slideIn != 0) {
        s_menuMain.cursor = 0;
        s_menuMain.orderCursor = 0;
    }
    s_menuMain.state = MENUMAIN_COMMAND;
    text_setBank(s_menuMainText);
    menuMain_initPanels();
    rect.x = 8;
    rect.y = 8;
    rect.w = 0xE8;
    rect.h = g_gameState->dispEnv->disp.h - 0xF;
    help_setBounds(&rect);
    help_setCallback(menuMain_updateHelp);
    help_setCursorOnTop(1);
    if (g_gameState->gameData->party[g_menuScreen.member] == NULL) {
        g_menuScreen.member = party_getFirstMember();
    }
}

void menuMain_update(void) {
    menuMain_input();
    menuMain_draw();
    menuMain_updatePanels();
}

void menuMain_exit(void) {
    help_setCursorOnTop(1);
    help_setBounds(0);
    help_setCallback(0);
}

void menuMain_setCursor(u8 cursor) {
    s_menuMain.cursor = cursor;
}

void menuMain_input(void) {
    if (g_menuScreen.slideIn == 0 && fade_isBusy() == 0) {
        if (s_menuMainInput[s_menuMain.state] != NULL) {
            s_menuMainInput[s_menuMain.state]();
        }
    }
}

void menuMain_draw(void) {
    menuMain_drawTimeGil();
    menuMain_drawLocation();
    menuMain_drawParty();
    menuMain_drawCursor();
}

void menuMain_updatePanels(void) {
    s32 i;

    if (g_menuScreen.slideIn != 0) {
        for (i = 0; i < 6; i++) {
            if (s_menuMain.panels[i].anim != 0) {
                tween_step(&s_menuMain.panels[i]);
            }
        }
    }
}

void menuMain_drawParty(void) {
    RECT boxRect;
    s16 y;
    s32 i;
    RECT *panelRect;

    i = 0;
    do {
        panelRect = &s_menuMain.panels[i].rect;
        boxRect.x = panelRect->x + 0x82;
        y = panelRect->y;
        boxRect.w = 0x72;
        boxRect.h = 0x12;
        boxRect.y = y;
        if (g_gameState->gameData->party[i] != NULL) {
            menuMain_drawMember(i);
        }
        menuMain_drawPortraitShade(panelRect, 0x14);
        win_drawBox(&boxRect, 0x14, WIN_STYLE_LINE);
        win_drawBox(panelRect, 0x14, WIN_STYLE_LINE);
        win_drawShadow(panelRect, 0x15, WIN_STYLE_LINE);
        i++;
    } while (i < 4);
}

INCLUDE_RODATA("asm/us/ovl_0c/nonmatchings/menu_main", s_menuMainText);
