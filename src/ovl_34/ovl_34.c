#include "ovl_34.h"

void menuConfig_init(void) {
    ConfigMenu *menu;
    s32 i;

    text_setBank(g_configMenuText);
    menuList_init(&s_menuConfig.list, CONFIG_ROW_COUNT, 9, 1, 0xB4);
    menu = &s_menuConfig;
    menu->subCursor = 0;
    menu->editing = 0;
    menu->rumbleTest = 0;
    menu->sliderDir = 0;
    menu->sliderTicks = 0;
    menu->rumbleTicks = 0;
    menu->primBufs[0] = PSX_RAM(u8 *, 0x801F9F00);
    menu->primBufs[1] = PSX_RAM(u8 *, 0x801FBF00);
    menu->prim = menu->primBufs[g_gameState->gameData->drawBuf ^ 1];
    menu->charCount = 0;
    for (i = 0; i < CHR_COUNT; i++) {
        if (g_gameState->gameData->characters[i].info.b.recruited) {
            s_menuConfig.charPortBit[s_menuConfig.charCount] = chr_getClass(g_gameState->gameData->characters[i].info.b.row);
            s_menuConfig.charIdx[s_menuConfig.charCount] = i;
            s_menuConfig.charCount++;
        }
    }
    help_setCallback(help_nullHandler);
}

void menuConfig_drawRows(void) {
    ScrollPos sp;
    RECT rowRect;
    RECT pageRect, clip;
    s32 i;
    MenuList *list = &s_menuConfig.list;

    win_setClip(NULL, 0x17, WIN_STYLE_FRAMELESS);
    menuConfig_getListView(list, &sp);
    i = 0;
    rowRect.x = 8;
    rowRect.w = 0x12C;
    rowRect.y = 0;
    rowRect.h = 0x14;
    for (; i < sp.rows; i++) {
        rowRect.y = i * 0x14 + (s16)(sp.y + 0x2A);
        menuConfig_drawRow(&rowRect, 0x17, sp.first + i);
        win_drawBox(&rowRect, 0x17, WIN_STYLE_LINE);
    }
    if (list->scroll & MENULIST_SCROLL_PAGE) {
        pageRect.x = 8;
        pageRect.y = 0x2A;
        pageRect.w = 0x12C;
        pageRect.h = 0xA8C;
        menuConfig_getScrollClip(list, &pageRect, &clip);
        win_setClip(&clip, 0x17, WIN_STYLE_FRAMELESS);
        menuConfig_drawScrollRows();
    }
    rowRect.y = 0x2A;
    rowRect.h = 0xB4;
    win_setClip(&rowRect, 0x17, WIN_STYLE_FRAMELESS);
}

void menuConfig_inputList(void) {
    ConfigMenu *menu = &s_menuConfig;
    s32 dir;

    if (menu->sliderDir == 0 && menu->list.scroll == 0) {
        if (g_padRepeat & BTN_CIRCLE) {
            snd_playSe(SE_CANCEL);
            menuScreen_open(0);
            return;
        }
        if (g_padRepeat & BTN_CROSS) {
            menuConfig_openSubWindow();
            return;
        }
        menuList_down(&menu->list, g_padRepeat);
        menuList_up(&menu->list, g_padRepeat);
        menuConfig_pageDown(&menu->list, g_padRepeat);
        menuConfig_pageUp(&menu->list, g_padRepeat);
        dir = 0;
        if ((g_padRepeat & PAD_LEFT) || (dir = 1, g_padRepeat & PAD_RIGHT)) {
            menuConfig_setOption(dir);
        }
    }
}

void menuConfig_inputPadMap(void) {
    s32 i;

    if (!menuConfig_isWindowMoving()) {
        if (g_padRepeat & PAD_START) {
            snd_playSe(SE_CANCEL);
            for (i = 0; i < 2; i++) {
                tween_start(&s_menuConfig.wins[i], &s_menuConfigPadMapClose, 0);
            }
            config_apply();
            return;
        }
        if (g_padRepeat & PAD_UP) {
            snd_playSe(SE_CURSOR);
            s_menuConfig.subCursor = s_menuConfig.subCursor != 0 ? s_menuConfig.subCursor - 1 : CONFIG_PAD_SLOTS - 1;
            return;
        }
        if (g_padRepeat & PAD_DOWN) {
            snd_playSe(SE_CURSOR);
            s_menuConfig.subCursor = s_menuConfig.subCursor >= CONFIG_PAD_SLOTS - 1 ? 0 : s_menuConfig.subCursor + 1;
            return;
        }
        for (i = 0; i < CONFIG_PAD_SLOTS; i++) {
            if (g_padPressed & s_menuConfigPadBits[i]) {
                snd_playSe(SE_CURSOR);
                menuConfig_setPadButton(s_menuConfig.subCursor, i);
                return;
            }
        }
    }
}

void menuConfig_inputBattleControl(void) {
    s32 i;

    if (!menuConfig_isWindowMoving()) {
        if (g_padRepeat & BTN_CIRCLE) {
            snd_playSe(SE_CANCEL);
            for (i = 0; i < 2; i++) {
                tween_start(&s_menuConfig.wins[i], &s_menuConfigBattleControlClose, 0);
            }
            return;
        }
        if ((g_padRepeat & PAD_LEFT) && (((s32)(g_gameState->config >> CFG_SHIFT_CHAR_PORT) >> s_menuConfig.charPortBit[s_menuConfig.subCursor]) & 1)) {
            snd_playSe(SE_CURSOR);
            g_gameState->config = (g_gameState->config & ((1 << CFG_SHIFT_CHAR_PORT) - 1)) | (((g_gameState->config >> CFG_SHIFT_CHAR_PORT) & ~(1 << s_menuConfig.charPortBit[s_menuConfig.subCursor])) << CFG_SHIFT_CHAR_PORT);
            return;
        }
        if ((g_padRepeat & PAD_RIGHT) && !(((s32)(g_gameState->config >> CFG_SHIFT_CHAR_PORT) >> s_menuConfig.charPortBit[s_menuConfig.subCursor]) & 1)) {
            snd_playSe(SE_CURSOR);
            g_gameState->config = (g_gameState->config & ((1 << CFG_SHIFT_CHAR_PORT) - 1)) | (((g_gameState->config >> CFG_SHIFT_CHAR_PORT) | (1 << s_menuConfig.charPortBit[s_menuConfig.subCursor])) << CFG_SHIFT_CHAR_PORT);
            return;
        }
        if (g_padRepeat & PAD_UP) {
            snd_playSe(SE_CURSOR);
            s_menuConfig.subCursor = s_menuConfig.subCursor != 0 ? s_menuConfig.subCursor - 1 : s_menuConfig.charCount - 1;
        } else if (g_padRepeat & PAD_DOWN) {
            snd_playSe(SE_CURSOR);
            s_menuConfig.subCursor = s_menuConfig.subCursor >= s_menuConfig.charCount - 1 ? 0 : s_menuConfig.subCursor + 1;
        }
    }
}

void menuConfig_drawPadMap(void) {
    RECT *rect = &s_menuConfig.wins[0].rect;
    RECT *r2;
    s32 i = 0;

    ui_drawLabel(rect->x + 0xB, rect->y - 3, 0x11, g_winLabelStrings.labelNew, UI_LABEL_NORMAL);
    ui_drawLabel(rect->x + 0x2D, rect->y - 3, 0x11, g_winLabelStrings.labelNormal, UI_LABEL_NORMAL);
    r2 = &s_menuConfig.wins[1].rect;
    text_drawBlock(r2->x + 0x2A, r2->y + 8, 0x11, text_get(CONFIG_TEXT_PAD_EXIT), TEXT_PAL_WHITE);
    for (; i < CONFIG_PAD_SLOTS; i++) {
        ui_drawSprite(rect->x + 0x10, rect->y + 0xB + i * 0x11, 0x11, s_menuConfigPadActions[(g_gameState->padMap >> CFG_PADMAP_SHIFT(i)) & CFG_PADMAP_ACTION_MASK].icon, 0);
        menuConfig_drawSmallText(rect->x + 0x27, rect->y + 0xE + i * 0x11, 0x11, s_menuConfigPadArrow, TEXT_PAL_WHITE);
        ui_drawSprite(rect->x + 0x3A, rect->y + 0xB + i * 0x11, 0x11, s_menuConfigPadActions[i].icon, 0);
        menuConfig_drawText(rect->x + 0x54, rect->y + 0xC + i * 0x11, 0x11, s_menuConfigPadActions[i].text, TEXT_PAL_WHITE);
    }
}

void menuConfig_drawBattleControl(void) {
    RECT *rect = &s_menuConfig.wins[0].rect;
    RECT *r2;
    s32 i;
    u8 on;

    r2 = &s_menuConfig.wins[1].rect;
    text_drawBlock(r2->x + 0x2A, r2->y + 8, 0x11, text_get(CONFIG_TEXT_PORT_EXIT), TEXT_PAL_WHITE);
    for (i = 0; i < s_menuConfig.charCount; i++) {
        on = ((s32)(g_gameState->config >> CFG_SHIFT_CHAR_PORT) >> s_menuConfig.charPortBit[i]) & 1;
        text_drawBlock(rect->x + 0xE, rect->y + 0xA + i * 0x11, 0x11, g_gameState->gameData->characters[s_menuConfig.charIdx[i]].name, TEXT_PAL_WHITE);
        menuConfig_drawIcon(rect->x + 0x60, rect->y + 0xA + i * 0x11, 0x11, 0xF8, on);
        menuConfig_drawIcon(rect->x + 0x92, rect->y + 0xA + i * 0x11, 0x11, 0x100, on == 0);
    }
}

void menuConfig_drawCursor(void) {
    s32 i;

    i = s_menuConfig.list.top + s_menuConfig.list.cursor;
    if (!fade_isBusy()) {
        g_menuHelp.x = 0;
        g_menuHelp.otz = 0x15;
        g_menuHelp.y = s_menuConfig.list.cursor * 0x14 + 0x31;
        if (s_menuConfig.editing == 0 || (g_menuScreenFrame & 3)) {
            help_drawCursor();
        }
        if (s_menuConfig.editing != 0 && !menuConfig_isWindowMoving()) {
            if (i == CONFIG_ROW_CONTROLLER) {
                g_menuHelp.x = 0x66;
                g_menuHelp.y = s_menuConfig.subCursor * 0x11 + 0x33;
            } else if (i == CONFIG_ROW_BTL_CONTROL) {
                g_menuHelp.x = 0x65;
                g_menuHelp.y = s_menuConfig.subCursor * 0x11 + 0x33;
            }
            g_menuHelp.otz = 0x10;
            help_drawCursor();
        }
    }
}

void menuConfig_setOption(u8 value) {
    u8 changed = 0;

    switch (s_menuConfig.list.top + s_menuConfig.list.cursor) {
    case CONFIG_ROW_SOUND:
        if (CFG.soundMono != value) {
            changed = 1;
        }
        CFG.soundMono = value;
        break;
    case CONFIG_ROW_CONTROLLER:
        if (CFG.padCustom != value) {
            changed = 1;
        }
        CFG.padCustom = value;
        break;
    case CONFIG_ROW_CURSOR:
        if (CFG.cursorMemory != value) {
            changed = 1;
        }
        CFG.cursorMemory = value;
        break;
    case CONFIG_ROW_ATB:
        if (CFG.atbWait != value) {
            changed = 1;
        }
        CFG.atbWait = value;
        break;
    case CONFIG_ROW_CAMERA:
        if (CFG.battleCameraFixed != value) {
            changed = 1;
        }
        CFG.battleCameraFixed = value;
        break;
    case CONFIG_ROW_BTL_CONTROL:
        if (CFG.battleControlCustom != value) {
            changed = 1;
        }
        CFG.battleControlCustom = value;
        break;
    case CONFIG_ROW_MOVEMENT:
        if (CFG.movementRun != value) {
            changed = 1;
        }
        CFG.movementRun = value;
        break;
    case CONFIG_ROW_HERE_ICON:
        if (CFG.hereIconOff != value) {
            changed = 1;
        }
        CFG.hereIconOff = value;
        break;
    case CONFIG_ROW_WINDOW_COLOR:
        if (CFG.windowClassic != value) {
            changed = 1;
        }
        CFG.windowClassic = value;
        break;
    case CONFIG_ROW_TARGET_WINDOW:
        if (CFG.targetWindowAlways != value) {
            changed = 1;
        }
        CFG.targetWindowAlways = value;
        break;
    case CONFIG_ROW_BTL_SPEED:
        if (value != 0 ? (s32)CFG.battleSpeed < CFG_BTL_SPEED_MAX : CFG.battleSpeed != 0) {
            s_menuConfig.sliderDir = value != 0 ? CONFIG_SLIDE_RIGHT : CONFIG_SLIDE_LEFT;
            changed = 1;
        }
        break;
    case CONFIG_ROW_MSG_SPEED:
        if (value != 0 ? (s32)CFG.messageSpeed < CFG_MSG_SPEED_MAX : CFG.messageSpeed != 0) {
            s_menuConfig.sliderDir = value != 0 ? CONFIG_SLIDE_RIGHT : CONFIG_SLIDE_LEFT;
            changed = 1;
        }
        break;
    case CONFIG_ROW_VIBRATION:
        if (!menuConfig_hasVibration()) {
            snd_playSe(SE_BUZZER);
            return;
        }
        if (CFG.vibration != value) {
            changed = 1;
            CFG.vibration = value;
            if (value == 0) {
                s_menuConfig.rumbleTest = changed;
                s_menuConfig.rumbleTicks = 0;
                menuConfig_setRumble(1);
            }
        }
        break;
    }
    if (changed) {
        snd_playSe(SE_CURSOR);
        config_apply();
    }
}

void menuConfig_openSubWindow(void) {
    s32 on;
    s32 i;
    s32 j;

    i = s_menuConfig.list.top + s_menuConfig.list.cursor;
    on = 1;
    if ((i == on && ((g_gameState->config >> i) & on) == on) ||
        (i == CONFIG_ROW_BTL_CONTROL && ((g_gameState->config >> CFG_BIT_BTL_CONTROL) & 1) == on)) {
        s_menuConfig.subCursor = 0;
        s_menuConfig.editing = 1;
        for (j = 0; j < 2; j++) {
            s_menuConfig.wins[j].rect = s_menuConfigSubRects[j];
            tween_start(&s_menuConfig.wins[j], &s_menuConfigSubOpen, 1);
        }
        snd_playSe(SE_CURSOR);
    }
}

void menuConfig_stepSlider(void) {
    s32 i;
    s32 step;

    if (s_menuConfig.sliderDir != 0 && ++s_menuConfig.sliderTicks >= 5) {
        i = s_menuConfig.list.top + s_menuConfig.list.cursor;
        step = 0xFF;
        if (s_menuConfig.sliderDir == CONFIG_SLIDE_RIGHT) {
            step = 1;
        }
        s_menuConfig.sliderTicks = 0;
        s_menuConfig.sliderDir = 0;
        switch (i) {
        case CONFIG_ROW_BTL_SPEED:
            g_gameState->config = (g_gameState->config & ~(CFG_SPEED_MASK << CFG_SHIFT_BTL_SPEED)) | (((step + ((g_gameState->config >> CFG_SHIFT_BTL_SPEED) & CFG_SPEED_MASK)) & CFG_SPEED_MASK) << CFG_SHIFT_BTL_SPEED);
            break;
        case CONFIG_ROW_MSG_SPEED:
            g_gameState->config = (g_gameState->config & ~(CFG_SPEED_MASK << CFG_SHIFT_MSG_SPEED)) | (((step + ((g_gameState->config >> CFG_SHIFT_MSG_SPEED) & CFG_SPEED_MASK)) & CFG_SPEED_MASK) << CFG_SHIFT_MSG_SPEED);
            break;
        }
    }
}

void menuConfig_updateHelp(void) {
    s32 i;
    s32 j;

    g_menuHelp.text = NULL;
    i = s_menuConfig.list.top + s_menuConfig.list.cursor;
    if (s_menuConfig.editing == 0) {
        g_menuHelp.text = text_get(i + CONFIG_TEXT_ROW_HELP);
        for (j = 0; j < 2; j++) {
            text_setStringArg(j, text_get(s_menuConfigRowText[i].options[j]));
        }
        return;
    }
    if (!menuConfig_isWindowMoving()) {
        if (i == CONFIG_ROW_CONTROLLER) {
            g_menuHelp.text = text_get(s_menuConfig.subCursor + CONFIG_TEXT_PAD_HELP);
        } else if (i == CONFIG_ROW_BTL_CONTROL) {
            g_menuHelp.text = text_get(CONFIG_TEXT_PORT_HELP);
        }
    }
}

void menuConfig_drawSlider(s32 x, s32 y, s32 otz, s32 pos, s32 w) {
    GaugeIcon *ic;
    GaugeIcon *mid;
    GaugeIcon *ec;
    s32 width;
    SprtWords *sprt;
    LineWords *line;
    RECT texWin;
    s32 i;

    ic = g_sliderGaugeSprites;
    mid = &ic[1];
    ec = &ic[2];
    width = w + 8;
    if (pos != 0) {
        for (i = 0; i < 3; i++) {
            s32 k = i + 2;
            s32 yy = (y + k) << 16;
            setlen(g_primPtr, PRIM_LEN(LINE_F2));
            line = (LineWords *)g_primPtr;
            line->rgbc = s_menuConfigSliderColors[i];
            line->xy0 = (x + 4) | yy;
            line->xy1 = (pos + x + 3) | yy;
            addPrim(&g_ot[otz], line);
            g_primPtr = (u8 *)(line + 1);
        }
    }
    ui_drawSpriteDef(x, y, otz, ic, 0);
    ui_drawSpriteDef(x + width - ec->w, y, otz, ec, 0);
    SetTexWindow(g_primPtr, &s_menuConfigSliderTexWindow);
    addPrim(&g_ot[otz], g_primPtr);
    g_primPtr += 0xC;
    setlen(g_primPtr, PRIM_LEN(SPRT));
    sprt = (SprtWords *)g_primPtr;
    sprt->rgbc = GPU_RGBC(GPU_SPRT, GPU_RGB_NEUTRAL);
    sprt->xy = ((x + ic->w) & 0xFFFF) | (y << 16);
    sprt->uvclut = mid->uv << 16;
    sprt->wh = (width - (ic->w + ec->w)) | (mid->h << 16);
    addPrim(&g_ot[otz], sprt);
    g_primPtr = (u8 *)(sprt + 1);
    texWin.x = mid->twX;
    texWin.y = mid->twY;
    texWin.w = mid->w;
    texWin.h = mid->h;
    SetTexWindow(g_primPtr, &texWin);
    addPrim(&g_ot[otz], g_primPtr);
    g_primPtr += 0xC;
    SetDrawTPage(g_primPtr, 1, 1, getTPage(0, 0, VRAM_UI_SHEET_X, VRAM_UI_SHEET_Y));
    addPrim(&g_ot[otz], g_primPtr);
    g_primPtr += 8;
}

ScrollPos *menuConfig_getListView(MenuList *list, ScrollPos *view) {
    s32 rows;
    s32 scrollFlags;
    s32 remain;

    scrollFlags = list->scroll;
    if (scrollFlags & MENULIST_SCROLL_PAGE) {
        if (scrollFlags & MENULIST_SCROLL_UP) {
            remain = list->height - list->scrollPos;
            rows = (remain + 19) / 20;
            view->rows = rows;
            view->first = list->top + (list->visibleRows - rows);
            view->y = (list->visibleRows - view->rows) * 20;
        } else {
            remain = list->height - list->scrollPos;
            view->rows = (remain + 19) / 20;
            view->first = list->top;
            view->y = 0;
        }
    } else if (scrollFlags & MENULIST_SCROLL_UP) {
        view->rows = list->visibleRows + 1;
        view->first = list->top - 1;
        view->y = list->scrollPos - 20;
    } else if (scrollFlags & MENULIST_SCROLL_DOWN) {
        view->first = list->top;
        view->rows = list->visibleRows + 1;
        view->y = -list->scrollPos;
    } else {
        view->first = list->top;
        view->rows = list->visibleRows;
        view->y = 0;
    }
    return view;
}

void menuConfig_update(void) {
    menuConfig_input();
    menuConfig_draw();
    menuConfig_updateAnims();
}

void menuConfig_exit(void) {
    help_setCallback(NULL);
}

void menuConfig_input(void) {
    s32 i;
    void (*fn)(void);

    i = s_menuConfig.list.top + s_menuConfig.list.cursor;
    if (!fade_isBusy()) {
        if (s_menuConfig.editing == 0) {
            menuConfig_inputList();
            return;
        }
        fn = s_menuConfigSubInput[i];
        if (fn != NULL) {
            fn();
        }
    }
}

void menuConfig_draw(void) {
    s32 k = 0x2000;

    s_menuConfig.primFree = s_menuConfig.primBufs[g_gameState->gameData->drawBuf ^ 1] - (s_menuConfig.prim - k);
    s_menuConfig.prim = s_menuConfig.primBufs[g_gameState->gameData->drawBuf];
    ui_drawLabel(0x10, 0x23, 0x16, g_labelConfig, UI_LABEL_NORMAL);
    ui_drawRule(8, 0x26, 0x16, 0, 0x12C, (g_gameState->config >> CFG_BIT_WINDOW_COLOR) & 1);
    menuConfig_drawRows();
    menuConfig_drawSubWindows();
    menuConfig_drawScrollArrows();
    menuConfig_drawCursor();
}

void menuConfig_updateAnims(void) {
    if (help_isOpen()) {
        menuConfig_updateHelp();
    }
    if (s_menuConfig.editing == 0) {
        menuConfig_stepSlider();
    } else {
        menuConfig_stepWindows();
    }
    menuConfig_stepScroll();
    menuConfig_stepRumble();
}

void menuConfig_pageUp(MenuList *list, u32 pad) {
    s32 h;
    s32 top;

    if ((pad & BTN_L1) && !(g_padHeld & BTN_R1) && list->top != 0) {
        h = list->height;
        list->scrollPos = h >> 2;
        list->scroll = MENULIST_SCROLL_DOWN | MENULIST_SCROLL_UP | MENULIST_SCROLL_PAGE;
        top = list->top - list->visibleRows;
        list->pageStep = (h - list->scrollPos + 0xE) / 15;
        if (top < 0) {
            top = 0;
        }
        list->pageTop = top;
        snd_playSe(SE_CURSOR);
    }
}

void menuConfig_pageDown(MenuList *list, u32 pad) {
    s32 h;
    s32 page;
    s32 top;
    s32 max;

    if ((pad & BTN_R1) && list->top < list->rows - list->visibleRows) {
        h = list->height;
        list->scrollPos = h >> 2;
        page = list->visibleRows;
        max = list->rows - page;
        list->scroll = MENULIST_SCROLL_DOWN | MENULIST_SCROLL_PAGE;
        top = list->top + page;
        list->pageStep = (h - list->scrollPos + 0xE) / 15;
        if (top < max) {
            max = top;
        }
        list->pageTop = max;
        snd_playSe(SE_CURSOR);
    }
}

void menuConfig_getScrollClip(MenuList *list, RECT *src, RECT *dst) {
    dst->x = src->x;
    dst->w = src->w;
    if (list->scroll & MENULIST_SCROLL_UP) {
        dst->y = src->y + list->scrollPos;
    } else {
        dst->y = src->y;
    }
    dst->h = src->h - list->scrollPos;
}

void menuConfig_drawScrollRows(void) {
    ScrollPos sp;
    RECT rowRect;
    s32 i;
    s32 y;

    menuConfig_getScrollView(&s_menuConfig.list, &sp);
    i = 0;
    rowRect.x = 8;
    rowRect.w = 0x12C;
    rowRect.y = 0;
    rowRect.h = 0x14;
    for (; i < sp.rows; i++) {
        rowRect.y = i * 0x14 + (s16)(sp.y + 0x2A);
        menuConfig_drawRow(&rowRect, 0x17, sp.first + i);
        win_drawBox(&rowRect, 0x17, WIN_STYLE_LINE);
    }
}

void menuConfig_drawRow(RECT *rect, s32 otz, s32 i) {
    ui_drawEtchedLine(rect->x + 0x70, rect->y + 1, otz, 4 - rect->h);
    s_menuConfigRowDraw[i](rect, otz);
}

void menuConfig_drawSoundRow(RECT *rect, s32 otz) {
    menuConfig_drawText(rect->x + 0xF, rect->y + 6, otz, s_menuConfigRowText[CONFIG_ROW_SOUND].title, TEXT_PAL_WHITE);
    menuConfig_drawChoice(rect, otz, (g_gameState->config >> CFG_BIT_SOUND) & 1, CONFIG_ROW_SOUND);
}

void menuConfig_drawControllerRow(RECT *rect, s32 otz) {
    menuConfig_drawText(rect->x + 0xF, rect->y + 6, otz, s_menuConfigRowText[CONFIG_ROW_CONTROLLER].title, TEXT_PAL_WHITE);
    menuConfig_drawChoice(rect, otz, (g_gameState->config >> CFG_BIT_CONTROLLER) & 1, CONFIG_ROW_CONTROLLER);
}

void menuConfig_drawCursorRow(RECT *rect, s32 otz) {
    menuConfig_drawText(rect->x + 0xF, rect->y + 6, otz, s_menuConfigRowText[CONFIG_ROW_CURSOR].title, TEXT_PAL_WHITE);
    menuConfig_drawChoice(rect, otz, (g_gameState->config >> CFG_BIT_CURSOR) & 1, CONFIG_ROW_CURSOR);
}

void menuConfig_drawAtbRow(RECT *rect, s32 otz) {
    menuConfig_drawText(rect->x + 0xF, rect->y + 6, otz, s_menuConfigRowText[CONFIG_ROW_ATB].title, TEXT_PAL_WHITE);
    menuConfig_drawChoice(rect, otz, (g_gameState->config >> CFG_BIT_ATB) & 1, CONFIG_ROW_ATB);
}

void menuConfig_drawCameraRow(RECT *rect, s32 otz) {
    menuConfig_drawText(rect->x + 0xF, rect->y + 6, otz, s_menuConfigRowText[CONFIG_ROW_CAMERA].title, TEXT_PAL_WHITE);
    menuConfig_drawChoice(rect, otz, (g_gameState->config >> CFG_BIT_CAMERA) & 1, CONFIG_ROW_CAMERA);
}

void menuConfig_drawBattleControlRow(RECT *rect, s32 otz) {
    menuConfig_drawText(rect->x + 0xF, rect->y + 6, otz, s_menuConfigRowText[CONFIG_ROW_BTL_CONTROL].title, TEXT_PAL_WHITE);
    menuConfig_drawChoice(rect, otz, (g_gameState->config >> CFG_BIT_BTL_CONTROL) & 1, CONFIG_ROW_BTL_CONTROL);
}

void menuConfig_drawMovementRow(RECT *rect, s32 otz) {
    menuConfig_drawText(rect->x + 0xF, rect->y + 6, otz, s_menuConfigRowText[CONFIG_ROW_MOVEMENT].title, TEXT_PAL_WHITE);
    menuConfig_drawChoice(rect, otz, (g_gameState->config >> CFG_BIT_MOVEMENT) & 1, CONFIG_ROW_MOVEMENT);
}

void menuConfig_drawBattleSpeedRow(RECT *rect, s32 otz) {
    menuConfig_drawText(rect->x + 0xF, rect->y + 6, otz, s_menuConfigRowText[CONFIG_ROW_BTL_SPEED].title, TEXT_PAL_WHITE);
    menuConfig_drawSliderRow(rect, otz, (g_gameState->config >> CFG_SHIFT_BTL_SPEED) & CFG_SPEED_MASK, CFG_BTL_SPEED_MAX, CONFIG_ROW_BTL_SPEED);
}

void menuConfig_drawMessageSpeedRow(RECT *rect, s32 otz) {
    menuConfig_drawText(rect->x + 0xF, rect->y + 6, otz, s_menuConfigRowText[CONFIG_ROW_MSG_SPEED].title, TEXT_PAL_WHITE);
    menuConfig_drawSliderRow(rect, otz, (g_gameState->config >> CFG_SHIFT_MSG_SPEED) & CFG_SPEED_MASK, CFG_MSG_SPEED_MAX, CONFIG_ROW_MSG_SPEED);
}

void menuConfig_drawHereIconRow(RECT *rect, s32 otz) {
    menuConfig_drawText(rect->x + 0xF, rect->y + 6, otz, s_menuConfigRowText[CONFIG_ROW_HERE_ICON].title, TEXT_PAL_WHITE);
    menuConfig_drawChoice(rect, otz, (g_gameState->config >> CFG_BIT_HERE_ICON) & 1, CONFIG_ROW_HERE_ICON);
}

void menuConfig_drawWindowColorRow(RECT *rect, s32 otz) {
    menuConfig_drawText(rect->x + 0xF, rect->y + 6, otz, s_menuConfigRowText[CONFIG_ROW_WINDOW_COLOR].title, TEXT_PAL_WHITE);
    menuConfig_drawChoice(rect, otz, (g_gameState->config >> CFG_BIT_WINDOW_COLOR) & 1, CONFIG_ROW_WINDOW_COLOR);
}

void menuConfig_drawTargetWindowRow(RECT *rect, s32 otz) {
    menuConfig_drawText(rect->x + 0xF, rect->y + 6, otz, s_menuConfigRowText[CONFIG_ROW_TARGET_WINDOW].title, TEXT_PAL_WHITE);
    menuConfig_drawChoice(rect, otz, (g_gameState->config >> CFG_BIT_TARGET_WINDOW) & 1, CONFIG_ROW_TARGET_WINDOW);
}

void menuConfig_drawVibrationRow(RECT *rect, s32 otz) {
    s32 pal;
    u32 sel;

    if (menuConfig_hasVibration()) {
        pal = 0;
        sel = (u8)g_gameState->config;
        sel >>= CFG_BIT_VIBRATION;
    } else {
        sel = pal = 2;
    }
    menuConfig_drawText(rect->x + 0xF, rect->y + 6, otz, s_menuConfigRowText[CONFIG_ROW_VIBRATION].title, pal);
    menuConfig_drawChoice(rect, otz, sel, CONFIG_ROW_VIBRATION);
}

void menuConfig_drawSubWindows(void) {
    s32 i;

    i = s_menuConfig.list.top + s_menuConfig.list.cursor;
    if (s_menuConfig.editing != 0) {
        win_drawBox(&s_menuConfig.wins[0].rect, 0x12, WIN_STYLE_BEVEL);
        win_drawBox(&s_menuConfig.wins[1].rect, 0x12, WIN_STYLE_BEVEL);
        if (i == CONFIG_ROW_CONTROLLER) {
            menuConfig_drawPadMap();
            return;
        }
        if (i == CONFIG_ROW_BTL_CONTROL) {
            menuConfig_drawBattleControl();
        }
    }
}

void menuConfig_drawScrollArrows(void) {
    MenuList *list;
    s32 blink;

    list = &s_menuConfig.list;
    blink = (((u32)g_menuFrame >> 4) ^ 1) & 1;
    ui_drawSprite(0x12E, 0x2C, 0x14, UI_SPRITE_SCROLL_UP, list->top != 0 ? blink : 1);
    ui_drawSprite(0x12E, 0xCF, 0x14, UI_SPRITE_SCROLL_DOWN, list->top < list->rows - list->visibleRows ? blink : 1);
}

void menuConfig_drawChoice(RECT *rect, s32 otz, s32 sel, s32 item) {
    s32 i;

    for (i = 0; i < 2; i++) {
        menuConfig_drawText(rect->x + 0x90 + i * 0x5A, rect->y + 6, otz, s_menuConfigRowText[item].options[i], sel != i ? TEXT_PAL_GREY : TEXT_PAL_WHITE);
    }
}

void menuConfig_drawSliderRow(RECT *rect, s32 otz, s32 value, s32 maxValue, s32 item) {
    s32 step = 0x12 / maxValue;
    s32 x;
    s32 y;
    s32 pos;
    s32 cur;
    s32 w;

    pos = value * step * 5;
    x = rect->x;
    y = rect->y + 6;
    cur = s_menuConfig.list.top + s_menuConfig.list.cursor;
    w = text_measureWidth(text_get(s_menuConfigSlowText));
    if (item == cur && s_menuConfig.sliderDir != 0) {
        if (s_menuConfig.sliderDir == CONFIG_SLIDE_RIGHT) {
            pos += s_menuConfig.sliderTicks * step;
        } else {
            pos += -s_menuConfig.sliderTicks * step;
        }
    }
    menuConfig_drawText(x - w + 0x9C, y, otz, (u8)s_menuConfigSlowText, TEXT_PAL_WHITE);
    menuConfig_drawText(x + 0x103, y, otz, (u8)s_menuConfigFastText, TEXT_PAL_WHITE);
    menuConfig_drawSlider(x + 0x9E, y, otz, pos, 0x5A);
}

void menuConfig_drawSmallText(s32 x, s32 y, s32 otz, u8 *str, s32 pal) {
    s_menuConfig.prim = text_buildSmall(x, y, &g_ot[otz], str, pal, s_menuConfig.prim);
}

void menuConfig_drawIcon(s32 x, s32 y, s32 otz, s32 spriteId, u8 flag) {
    s32 spriteW[2];

    s_menuConfig.prim = ui_buildSprite(x, y, &g_ot[otz], spriteId, flag, spriteW, s_menuConfig.prim);
}

void menuConfig_drawText(s32 x, s32 y, s32 otz, s32 text, s32 color) {
    s_menuConfig.prim = text_buildBlock(x, y - 4, &g_ot[otz], text_get(text & 0xFF), color, s_menuConfig.prim);
}

void menuConfig_stepWindows(void) {
    if (s_menuConfig.wins[0].anim != 0) {
        tween_step(&s_menuConfig.wins[0]);
    }
    if (s_menuConfig.wins[1].anim != 0) {
        tween_step(&s_menuConfig.wins[1]);
    }
}

void menuConfig_stepScroll(void) {
    MenuList *list = &s_menuConfig.list;
    s32 newTop;
    s32 dec;

    if (list->scroll != 0) {
        if (!(list->scroll & MENULIST_SCROLL_PAGE)) {
            dec = list->scroll & MENULIST_SCROLL_UP;
            list->scrollPos += 4;
            if (list->scrollPos >= 0x14) {
                newTop = list->top;
                if (dec) {
                    newTop--;
                } else {
                    newTop++;
                }
                list->top = newTop;
                list->scroll = 0;
                list->scrollPos = 0;
            }
        } else {
            list->scrollPos += 9;
            if (list->scrollPos >= 0xB4) {
                list->scroll = 0;
                list->scrollPos = 0;
                list->top = list->pageTop;
            }
        }
    }
}

void menuConfig_stepRumble(void) {
    if (s_menuConfig.rumbleTest != 0) {
        if (++s_menuConfig.rumbleTicks >= 9) {
            s_menuConfig.rumbleTest = 0;
            s_menuConfig.rumbleTicks = 0;
            menuConfig_setRumble(0);
        }
    }
}

void menuConfig_setPadButton(s32 slot, s32 action) {
    s32 i;
    s32 j;
    u32 w;
    u32 old;

    for (i = 0; i < CONFIG_PAD_SLOTS; i++) {
        if (action == ((g_gameState->padMap >> CFG_PADMAP_SHIFT(i)) & CFG_PADMAP_ACTION_MASK)) {
            j = i;
            goto found;
        }
    }
    j = 0;
found:
    w = g_gameState->padMap;
    old = (w >> CFG_PADMAP_SHIFT(slot)) & CFG_PADMAP_ACTION_MASK;
    g_gameState->padMap = (((w & ~(CFG_PADMAP_ACTION_MASK << CFG_PADMAP_SHIFT(slot))) | (action << CFG_PADMAP_SHIFT(slot)))
                           & ~(CFG_PADMAP_ACTION_MASK << CFG_PADMAP_SHIFT(j))) | (old << CFG_PADMAP_SHIFT(j));
}

void menuConfig_closeSubWindow(void) {
    s_menuConfig.editing = 0;
}

u8 menuConfig_isWindowMoving(void) {
    return s_menuConfig.wins[0].anim != 0;
}

u8 menuConfig_hasVibration(void) {
    s32 i;

    for (i = 0; i < 2; i++) {
        if (((g_padBuf[i].hdr >> 12) & 0xF) == PAD_ID_ANALOG) {
            return 1;
        }
        if (g_padPortState[i] == 6) {
            return 1;
        }
    }
    return 0;
}

void menuConfig_setRumble(u8 on) {
    s32 i;

    if (on) {
        for (i = 0; i < 2; i++) {
            g_padActuator[i].largeMotor = 0xFF;
            g_padActuator[i].smallMotor = 1;
        }
        return;
    }
    for (i = 0; i < 2; i++) {
        g_padActuator[i].smallMotor = 0;
        g_padActuator[i].largeMotor = 0;
    }
}

ScrollPos *menuConfig_getScrollView(MenuList *list, ScrollPos *view) {
    s32 ofs;

    if (list->scroll & MENULIST_SCROLL_UP) {
        view->rows = (list->scrollPos + 0x13) / 20;
        view->first = list->pageTop + (list->visibleRows - view->rows);
        view->y = list->scrollPos - view->rows * 0x14;
    } else {
        ofs = list->scrollPos;
        view->y = list->height - ofs;
        view->rows = (ofs + 0x13) / 20;
        view->first = list->pageTop;
    }
    return view;
}

/* Defined last: its .rodata follows menuConfig_setOption's jump table. */
/* menu text block */
INCLUDE_RODATA("asm/us/ovl_34/nonmatchings/ovl_34", g_configMenuText);
