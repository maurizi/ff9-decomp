#include "ovl_34.h"

void menuConfig_init(void) {
    ConfigMenu *m;
    s32 i;

    text_setBank(g_configMenuText);
    menuList_init(&s_menuConfig.list, CONFIG_ROW_COUNT, 9, 1, 0xB4);
    m = &s_menuConfig;
    m->subCursor = 0;
    m->editing = 0;
    m->rumbleTest = 0;
    m->sliderDir = 0;
    m->sliderTicks = 0;
    m->rumbleTicks = 0;
    m->primBufs[0] = (u8 *)0x801F9F00;
    m->primBufs[1] = (u8 *)0x801FBF00;
    m->prim = m->primBufs[g_gameState->unk1C->unk8 ^ 1];
    m->charCount = 0;
    for (i = 0; i < 9; i++) {
        if (g_gameState->unk1C->characters[i].unk34.b.unkB) {
            s_menuConfig.charPortBit[s_menuConfig.charCount] = chr_getClass(g_gameState->unk1C->characters[i].unk34.b.row);
            s_menuConfig.charIdx[s_menuConfig.charCount] = i;
            s_menuConfig.charCount++;
        }
    }
    help_setCallback(help_nullHandler);
}

void menuConfig_drawRows(void) {
    ScrollPos sp;
    RECT r;
    RECT a, b;
    s32 i;
    MenuList *l = &s_menuConfig.list;

    win_setClip(NULL, 0x17, 4);
    menuConfig_getListView(l, &sp);
    i = 0;
    r.x = 8;
    r.w = 0x12C;
    r.y = 0;
    r.h = 0x14;
    for (; i < sp.rows; i++) {
        r.y = i * 0x14 + (s16)(sp.y + 0x2A);
        menuConfig_drawRow(&r, 0x17, sp.first + i);
        win_drawBox(&r, 0x17, 0);
    }
    if (l->scroll & 4) {
        a.x = 8;
        a.y = 0x2A;
        a.w = 0x12C;
        a.h = 0xA8C;
        menuConfig_getScrollClip(l, &a, &b);
        win_setClip(&b, 0x17, 4);
        menuConfig_drawScrollRows();
    }
    r.y = 0x2A;
    r.h = 0xB4;
    win_setClip(&r, 0x17, 4);
}

void menuConfig_inputList(void) {
    ConfigMenu *m = &s_menuConfig;
    s32 dir;

    if (m->sliderDir == 0 && m->list.scroll == 0) {
        if (g_padRepeat & BTN_CIRCLE) {
            snd_playSe(SE_CANCEL);
            menuScreen_open(0);
            return;
        }
        if (g_padRepeat & BTN_CROSS) {
            menuConfig_openSubWindow();
            return;
        }
        menuList_down(&m->list, g_padRepeat);
        menuList_up(&m->list, g_padRepeat);
        menuConfig_pageDown(&m->list, g_padRepeat);
        menuConfig_pageUp(&m->list, g_padRepeat);
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
                func_80063118(&s_menuConfig.wins[i], &s_menuConfigPadMapClose, 0);
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
                func_80063118(&s_menuConfig.wins[i], &s_menuConfigBattleControlClose, 0);
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
    RECT *r = &s_menuConfig.wins[0].unk4;
    RECT *r2;
    s32 i = 0;

    ui_drawLabel(r->x + 0xB, r->y - 3, 0x11, g_winLabelStrings.unk84, 0);
    ui_drawLabel(r->x + 0x2D, r->y - 3, 0x11, g_winLabelStrings.unk8C, 0);
    r2 = &s_menuConfig.wins[1].unk4;
    text_drawBlock(r2->x + 0x2A, r2->y + 8, 0x11, text_get(CONFIG_TEXT_PAD_EXIT), 0);
    for (; i < CONFIG_PAD_SLOTS; i++) {
        ui_drawSprite(r->x + 0x10, r->y + 0xB + i * 0x11, 0x11, s_menuConfigPadActions[(g_gameState->padMap >> (i * 4)) & 7].icon, 0);
        menuConfig_drawSmallText(r->x + 0x27, r->y + 0xE + i * 0x11, 0x11, D_801F14F8, 0);
        ui_drawSprite(r->x + 0x3A, r->y + 0xB + i * 0x11, 0x11, s_menuConfigPadActions[i].icon, 0);
        menuConfig_drawText(r->x + 0x54, r->y + 0xC + i * 0x11, 0x11, s_menuConfigPadActions[i].text, 0);
    }
}

void menuConfig_drawBattleControl(void) {
    RECT *r = &s_menuConfig.wins[0].unk4;
    RECT *r2;
    s32 i;
    u8 on;

    r2 = &s_menuConfig.wins[1].unk4;
    text_drawBlock(r2->x + 0x2A, r2->y + 8, 0x11, text_get(CONFIG_TEXT_PORT_EXIT), 0);
    for (i = 0; i < s_menuConfig.charCount; i++) {
        on = ((s32)(g_gameState->config >> CFG_SHIFT_CHAR_PORT) >> s_menuConfig.charPortBit[i]) & 1;
        text_drawBlock(r->x + 0xE, r->y + 0xA + i * 0x11, 0x11, g_gameState->unk1C->characters[s_menuConfig.charIdx[i]].unk0, 0);
        menuConfig_drawIcon(r->x + 0x60, r->y + 0xA + i * 0x11, 0x11, 0xF8, on);
        menuConfig_drawIcon(r->x + 0x92, r->y + 0xA + i * 0x11, 0x11, 0x100, on == 0);
    }
}

void menuConfig_drawCursor(void) {
    s32 i;

    i = s_menuConfig.list.top + s_menuConfig.list.cursor;
    if (!fade_isBusy()) {
        g_menuHelp.x = 0;
        g_menuHelp.otz = 0x15;
        g_menuHelp.y = s_menuConfig.list.cursor * 0x14 + 0x31;
        if (s_menuConfig.editing == 0 || (D_80077E54 & 3)) {
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

void menuConfig_setOption(u8 v) {
    u8 changed = 0;

    switch (s_menuConfig.list.top + s_menuConfig.list.cursor) {
    case CONFIG_ROW_SOUND:
        if (CFG.soundMono != v) {
            changed = 1;
        }
        CFG.soundMono = v;
        break;
    case CONFIG_ROW_CONTROLLER:
        if (CFG.padCustom != v) {
            changed = 1;
        }
        CFG.padCustom = v;
        break;
    case CONFIG_ROW_CURSOR:
        if (CFG.cursorMemory != v) {
            changed = 1;
        }
        CFG.cursorMemory = v;
        break;
    case CONFIG_ROW_ATB:
        if (CFG.atbWait != v) {
            changed = 1;
        }
        CFG.atbWait = v;
        break;
    case CONFIG_ROW_CAMERA:
        if (CFG.battleCameraFixed != v) {
            changed = 1;
        }
        CFG.battleCameraFixed = v;
        break;
    case CONFIG_ROW_BTL_CONTROL:
        if (CFG.battleControlCustom != v) {
            changed = 1;
        }
        CFG.battleControlCustom = v;
        break;
    case CONFIG_ROW_MOVEMENT:
        if (CFG.movementRun != v) {
            changed = 1;
        }
        CFG.movementRun = v;
        break;
    case CONFIG_ROW_HERE_ICON:
        if (CFG.hereIconOff != v) {
            changed = 1;
        }
        CFG.hereIconOff = v;
        break;
    case CONFIG_ROW_WINDOW_COLOR:
        if (CFG.windowClassic != v) {
            changed = 1;
        }
        CFG.windowClassic = v;
        break;
    case CONFIG_ROW_TARGET_WINDOW:
        if (CFG.targetWindowAlways != v) {
            changed = 1;
        }
        CFG.targetWindowAlways = v;
        break;
    case CONFIG_ROW_BTL_SPEED:
        if (v != 0 ? (s32)CFG.battleSpeed < CFG_BTL_SPEED_MAX : CFG.battleSpeed != 0) {
            s_menuConfig.sliderDir = v != 0 ? CONFIG_SLIDE_RIGHT : CONFIG_SLIDE_LEFT;
            changed = 1;
        }
        break;
    case CONFIG_ROW_MSG_SPEED:
        if (v != 0 ? (s32)CFG.messageSpeed < CFG_MSG_SPEED_MAX : CFG.messageSpeed != 0) {
            s_menuConfig.sliderDir = v != 0 ? CONFIG_SLIDE_RIGHT : CONFIG_SLIDE_LEFT;
            changed = 1;
        }
        break;
    case CONFIG_ROW_VIBRATION:
        if (!menuConfig_hasVibration()) {
            snd_playSe(SE_BUZZER);
            return;
        }
        if (CFG.vibration != v) {
            changed = 1;
            CFG.vibration = v;
            if (v == 0) {
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
            s_menuConfig.wins[j].unk4 = s_menuConfigSubRects[j];
            func_80063118(&s_menuConfig.wins[j], &s_menuConfigSubOpen, 1);
        }
        snd_playSe(SE_CURSOR);
    }
}

void menuConfig_stepSlider(void) {
    s32 i;
    s32 d;

    if (s_menuConfig.sliderDir != 0 && ++s_menuConfig.sliderTicks >= 5) {
        i = s_menuConfig.list.top + s_menuConfig.list.cursor;
        d = 0xFF;
        if (s_menuConfig.sliderDir == CONFIG_SLIDE_RIGHT) {
            d = 1;
        }
        s_menuConfig.sliderTicks = 0;
        s_menuConfig.sliderDir = 0;
        switch (i) {
        case CONFIG_ROW_BTL_SPEED:
            g_gameState->config = (g_gameState->config & ~(7 << CFG_SHIFT_BTL_SPEED)) | (((d + ((g_gameState->config >> CFG_SHIFT_BTL_SPEED) & 7)) & 7) << CFG_SHIFT_BTL_SPEED);
            break;
        case CONFIG_ROW_MSG_SPEED:
            g_gameState->config = (g_gameState->config & ~(7 << CFG_SHIFT_MSG_SPEED)) | (((d + ((g_gameState->config >> CFG_SHIFT_MSG_SPEED) & 7)) & 7) << CFG_SHIFT_MSG_SPEED);
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
    SprtWords *p;
    LineWords *l;
    RECT r;
    s32 i;

    ic = D_80068988;
    mid = &ic[1];
    ec = &ic[2];
    width = w + 8;
    if (pos != 0) {
        for (i = 0; i < 3; i++) {
            s32 k = i + 2;
            s32 yy = (y + k) << 16;
            setlen(g_primPtr, 3);
            l = (LineWords *)g_primPtr;
            l->rgbc = s_menuConfigSliderColors[i];
            l->xy0 = (x + 4) | yy;
            l->xy1 = (pos + x + 3) | yy;
            addPrim(&g_ot[otz], l);
            g_primPtr = (u8 *)(l + 1);
        }
    }
    ui_drawSpriteDef(x, y, otz, ic, 0);
    ui_drawSpriteDef(x + width - ec->w, y, otz, ec, 0);
    SetTexWindow(g_primPtr, &s_menuConfigSliderTexWindow);
    addPrim(&g_ot[otz], g_primPtr);
    g_primPtr += 0xC;
    setlen(g_primPtr, 4);
    p = (SprtWords *)g_primPtr;
    p->rgbc = 0x64808080;
    p->xy = ((x + ic->w) & 0xFFFF) | (y << 16);
    p->uvclut = mid->uv << 16;
    p->wh = (width - (ic->w + ec->w)) | (mid->h << 16);
    addPrim(&g_ot[otz], p);
    g_primPtr = (u8 *)(p + 1);
    r.x = mid->twX;
    r.y = mid->twY;
    r.w = mid->w;
    r.h = mid->h;
    SetTexWindow(g_primPtr, &r);
    addPrim(&g_ot[otz], g_primPtr);
    g_primPtr += 0xC;
    SetDrawTPage(g_primPtr, 1, 1, 0x1F);
    addPrim(&g_ot[otz], g_primPtr);
    g_primPtr += 8;
}

ScrollPos *menuConfig_getListView(MenuList *l, ScrollPos *p) {
    s32 q;
    s32 f;
    s32 d;

    f = l->scroll;
    if (f & 4) {
        if (f & 2) {
            d = l->height - l->scrollPos;
            q = (d + 19) / 20;
            p->rows = q;
            p->first = l->top + (l->visibleRows - q);
            p->y = (l->visibleRows - p->rows) * 20;
        } else {
            d = l->height - l->scrollPos;
            p->rows = (d + 19) / 20;
            p->first = l->top;
            p->y = 0;
        }
    } else if (f & 2) {
        p->rows = l->visibleRows + 1;
        p->first = l->top - 1;
        p->y = l->scrollPos - 20;
    } else if (f & 1) {
        p->first = l->top;
        p->rows = l->visibleRows + 1;
        p->y = -l->scrollPos;
    } else {
        p->first = l->top;
        p->rows = l->visibleRows;
        p->y = 0;
    }
    return p;
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

    s_menuConfig.primFree = s_menuConfig.primBufs[g_gameState->unk1C->unk8 ^ 1] - (s_menuConfig.prim - k);
    s_menuConfig.prim = s_menuConfig.primBufs[g_gameState->unk1C->unk8];
    ui_drawLabel(0x10, 0x23, 0x16, D_80070430, 0);
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

void menuConfig_pageUp(MenuList *l, u32 pad) {
    s32 h;
    s32 top;

    if ((pad & BTN_L1) && !(g_padHeld & BTN_R1) && l->top != 0) {
        h = l->height;
        l->scrollPos = h >> 2;
        l->scroll = 7;
        top = l->top - l->visibleRows;
        l->pageStep = (h - l->scrollPos + 0xE) / 15;
        if (top < 0) {
            top = 0;
        }
        l->pageTop = top;
        snd_playSe(SE_CURSOR);
    }
}

void menuConfig_pageDown(MenuList *l, u32 pad) {
    s32 h;
    s32 page;
    s32 top;
    s32 max;

    if ((pad & BTN_R1) && l->top < l->rows - l->visibleRows) {
        h = l->height;
        l->scrollPos = h >> 2;
        page = l->visibleRows;
        max = l->rows - page;
        l->scroll = 5;
        top = l->top + page;
        l->pageStep = (h - l->scrollPos + 0xE) / 15;
        if (top < max) {
            max = top;
        }
        l->pageTop = max;
        snd_playSe(SE_CURSOR);
    }
}

void menuConfig_getScrollClip(MenuList *m, RECT *src, RECT *dst) {
    dst->x = src->x;
    dst->w = src->w;
    if (m->scroll & 2) {
        dst->y = src->y + m->scrollPos;
    } else {
        dst->y = src->y;
    }
    dst->h = src->h - m->scrollPos;
}

void menuConfig_drawScrollRows(void) {
    ScrollPos sp;
    RECT r;
    s32 i;
    s32 y;

    menuConfig_getScrollView(&s_menuConfig.list, &sp);
    i = 0;
    r.x = 8;
    r.w = 0x12C;
    r.y = 0;
    r.h = 0x14;
    for (; i < sp.rows; i++) {
        r.y = i * 0x14 + (s16)(sp.y + 0x2A);
        menuConfig_drawRow(&r, 0x17, sp.first + i);
        win_drawBox(&r, 0x17, 0);
    }
}

void menuConfig_drawRow(RECT *r, s32 otz, s32 i) {
    ui_drawEtchedLine(r->x + 0x70, r->y + 1, otz, 4 - r->h);
    s_menuConfigRowDraw[i](r, otz);
}

void menuConfig_drawSoundRow(RECT *r, s32 otz) {
    menuConfig_drawText(r->x + 0xF, r->y + 6, otz, s_menuConfigRowText[CONFIG_ROW_SOUND].title, 0);
    menuConfig_drawChoice(r, otz, (g_gameState->config >> CFG_BIT_SOUND) & 1, CONFIG_ROW_SOUND);
}

void menuConfig_drawControllerRow(RECT *r, s32 otz) {
    menuConfig_drawText(r->x + 0xF, r->y + 6, otz, s_menuConfigRowText[CONFIG_ROW_CONTROLLER].title, 0);
    menuConfig_drawChoice(r, otz, (g_gameState->config >> CFG_BIT_CONTROLLER) & 1, CONFIG_ROW_CONTROLLER);
}

void menuConfig_drawCursorRow(RECT *r, s32 otz) {
    menuConfig_drawText(r->x + 0xF, r->y + 6, otz, s_menuConfigRowText[CONFIG_ROW_CURSOR].title, 0);
    menuConfig_drawChoice(r, otz, (g_gameState->config >> CFG_BIT_CURSOR) & 1, CONFIG_ROW_CURSOR);
}

void menuConfig_drawAtbRow(RECT *r, s32 otz) {
    menuConfig_drawText(r->x + 0xF, r->y + 6, otz, s_menuConfigRowText[CONFIG_ROW_ATB].title, 0);
    menuConfig_drawChoice(r, otz, (g_gameState->config >> CFG_BIT_ATB) & 1, CONFIG_ROW_ATB);
}

void menuConfig_drawCameraRow(RECT *r, s32 otz) {
    menuConfig_drawText(r->x + 0xF, r->y + 6, otz, s_menuConfigRowText[CONFIG_ROW_CAMERA].title, 0);
    menuConfig_drawChoice(r, otz, (g_gameState->config >> CFG_BIT_CAMERA) & 1, CONFIG_ROW_CAMERA);
}

void menuConfig_drawBattleControlRow(RECT *r, s32 otz) {
    menuConfig_drawText(r->x + 0xF, r->y + 6, otz, s_menuConfigRowText[CONFIG_ROW_BTL_CONTROL].title, 0);
    menuConfig_drawChoice(r, otz, (g_gameState->config >> CFG_BIT_BTL_CONTROL) & 1, CONFIG_ROW_BTL_CONTROL);
}

void menuConfig_drawMovementRow(RECT *r, s32 otz) {
    menuConfig_drawText(r->x + 0xF, r->y + 6, otz, s_menuConfigRowText[CONFIG_ROW_MOVEMENT].title, 0);
    menuConfig_drawChoice(r, otz, (g_gameState->config >> CFG_BIT_MOVEMENT) & 1, CONFIG_ROW_MOVEMENT);
}

void menuConfig_drawBattleSpeedRow(RECT *r, s32 otz) {
    menuConfig_drawText(r->x + 0xF, r->y + 6, otz, s_menuConfigRowText[CONFIG_ROW_BTL_SPEED].title, 0);
    menuConfig_drawSliderRow(r, otz, (g_gameState->config >> CFG_SHIFT_BTL_SPEED) & 7, CFG_BTL_SPEED_MAX, CONFIG_ROW_BTL_SPEED);
}

void menuConfig_drawMessageSpeedRow(RECT *r, s32 otz) {
    menuConfig_drawText(r->x + 0xF, r->y + 6, otz, s_menuConfigRowText[CONFIG_ROW_MSG_SPEED].title, 0);
    menuConfig_drawSliderRow(r, otz, (g_gameState->config >> CFG_SHIFT_MSG_SPEED) & 7, CFG_MSG_SPEED_MAX, CONFIG_ROW_MSG_SPEED);
}

void menuConfig_drawHereIconRow(RECT *r, s32 otz) {
    menuConfig_drawText(r->x + 0xF, r->y + 6, otz, s_menuConfigRowText[CONFIG_ROW_HERE_ICON].title, 0);
    menuConfig_drawChoice(r, otz, (g_gameState->config >> CFG_BIT_HERE_ICON) & 1, CONFIG_ROW_HERE_ICON);
}

void menuConfig_drawWindowColorRow(RECT *r, s32 otz) {
    menuConfig_drawText(r->x + 0xF, r->y + 6, otz, s_menuConfigRowText[CONFIG_ROW_WINDOW_COLOR].title, 0);
    menuConfig_drawChoice(r, otz, (g_gameState->config >> CFG_BIT_WINDOW_COLOR) & 1, CONFIG_ROW_WINDOW_COLOR);
}

void menuConfig_drawTargetWindowRow(RECT *r, s32 otz) {
    menuConfig_drawText(r->x + 0xF, r->y + 6, otz, s_menuConfigRowText[CONFIG_ROW_TARGET_WINDOW].title, 0);
    menuConfig_drawChoice(r, otz, (g_gameState->config >> CFG_BIT_TARGET_WINDOW) & 1, CONFIG_ROW_TARGET_WINDOW);
}

void menuConfig_drawVibrationRow(RECT *r, s32 otz) {
    s32 c;
    u32 sel;

    if (menuConfig_hasVibration()) {
        c = 0;
        sel = (u8)g_gameState->config;
        sel >>= CFG_BIT_VIBRATION;
    } else {
        sel = c = 2;
    }
    menuConfig_drawText(r->x + 0xF, r->y + 6, otz, s_menuConfigRowText[CONFIG_ROW_VIBRATION].title, c);
    menuConfig_drawChoice(r, otz, sel, CONFIG_ROW_VIBRATION);
}

void menuConfig_drawSubWindows(void) {
    s32 i;

    i = s_menuConfig.list.top + s_menuConfig.list.cursor;
    if (s_menuConfig.editing != 0) {
        win_drawBox(&s_menuConfig.wins[0].unk4, 0x12, 2);
        win_drawBox(&s_menuConfig.wins[1].unk4, 0x12, 2);
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
    MenuList *l;
    s32 blink;

    l = &s_menuConfig.list;
    blink = (((u32)D_80077DC0 >> 4) ^ 1) & 1;
    ui_drawSprite(0x12E, 0x2C, 0x14, 0xB8, l->top != 0 ? blink : 1);
    ui_drawSprite(0x12E, 0xCF, 0x14, 0xB9, l->top < l->rows - l->visibleRows ? blink : 1);
}

void menuConfig_drawChoice(RECT *r, s32 otz, s32 sel, s32 item) {
    s32 i;

    for (i = 0; i < 2; i++) {
        menuConfig_drawText(r->x + 0x90 + i * 0x5A, r->y + 6, otz, s_menuConfigRowText[item].options[i], sel != i ? 2 : 0);
    }
}

void menuConfig_drawSliderRow(RECT *r, s32 otz, s32 v, s32 n, s32 item) {
    s32 step = 0x12 / n;
    s32 x;
    s32 y;
    s32 pos;
    s32 cur;
    s32 w;

    pos = v * step * 5;
    x = r->x;
    y = r->y + 6;
    cur = s_menuConfig.list.top + s_menuConfig.list.cursor;
    w = text_measureWidth(text_get(s_menuConfigSlowText));
    if (item == cur && s_menuConfig.sliderDir != 0) {
        if (s_menuConfig.sliderDir == CONFIG_SLIDE_RIGHT) {
            pos += s_menuConfig.sliderTicks * step;
        } else {
            pos += -s_menuConfig.sliderTicks * step;
        }
    }
    menuConfig_drawText(x - w + 0x9C, y, otz, (u8)s_menuConfigSlowText, 0);
    menuConfig_drawText(x + 0x103, y, otz, (u8)s_menuConfigFastText, 0);
    menuConfig_drawSlider(x + 0x9E, y, otz, pos, 0x5A);
}

void menuConfig_drawSmallText(s32 x, s32 y, s32 otz, u8 *s, s32 arg4) {
    s_menuConfig.prim = text_buildSmall(x, y, &g_ot[otz], s, arg4, s_menuConfig.prim);
}

void menuConfig_drawIcon(s32 x, s32 y, s32 otz, s32 n, u8 arg4) {
    s32 buf[2];

    s_menuConfig.prim = ui_buildSprite(x, y, &g_ot[otz], n, arg4, buf, s_menuConfig.prim);
}

void menuConfig_drawText(s32 x, s32 y, s32 otz, s32 text, s32 color) {
    s_menuConfig.prim = text_buildBlock(x, y - 4, &g_ot[otz], text_get(text & 0xFF), color, s_menuConfig.prim);
}

void menuConfig_stepWindows(void) {
    if (s_menuConfig.wins[0].unkC != 0) {
        func_800631AC(&s_menuConfig.wins[0]);
    }
    if (s_menuConfig.wins[1].unkC != 0) {
        func_800631AC(&s_menuConfig.wins[1]);
    }
}

void menuConfig_stepScroll(void) {
    MenuList *l = &s_menuConfig.list;
    s32 v;
    s32 dec;

    if (l->scroll != 0) {
        if (!(l->scroll & 4)) {
            dec = l->scroll & 2;
            l->scrollPos += 4;
            if (l->scrollPos >= 0x14) {
                v = l->top;
                if (dec) {
                    v--;
                } else {
                    v++;
                }
                l->top = v;
                l->scroll = 0;
                l->scrollPos = 0;
            }
        } else {
            l->scrollPos += 9;
            if (l->scrollPos >= 0xB4) {
                l->scroll = 0;
                l->scrollPos = 0;
                l->top = l->pageTop;
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

void menuConfig_setPadButton(s32 slot, s32 v) {
    s32 i;
    s32 j;
    u32 w;
    u32 old;

    for (i = 0; i < CONFIG_PAD_SLOTS; i++) {
        if (v == ((g_gameState->padMap >> (i * 4)) & 7)) {
            j = i;
            goto found;
        }
    }
    j = 0;
found:
    w = g_gameState->padMap;
    old = (w >> (slot * 4)) & 7;
    g_gameState->padMap = (((w & ~(7 << (slot * 4))) | (v << (slot * 4))) & ~(7 << (j * 4))) | (old << (j * 4));
}

void menuConfig_closeSubWindow(void) {
    s_menuConfig.editing = 0;
}

u8 menuConfig_isWindowMoving(void) {
    return s_menuConfig.wins[0].unkC != 0;
}

u8 menuConfig_hasVibration(void) {
    s32 i;

    for (i = 0; i < 2; i++) {
        if (((g_padBuf[i].unk0 >> 12) & 0xF) == 7) {
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
            g_padActuator[i].unk1 = 0xFF;
            g_padActuator[i].unk0 = 1;
        }
        return;
    }
    for (i = 0; i < 2; i++) {
        g_padActuator[i].unk0 = 0;
        g_padActuator[i].unk1 = 0;
    }
}

ScrollPos *menuConfig_getScrollView(MenuList *l, ScrollPos *p) {
    s32 v;

    if (l->scroll & 2) {
        p->rows = (l->scrollPos + 0x13) / 20;
        p->first = l->pageTop + (l->visibleRows - p->rows);
        p->y = l->scrollPos - p->rows * 0x14;
    } else {
        v = l->scrollPos;
        p->y = l->height - v;
        p->rows = (v + 0x13) / 20;
        p->first = l->pageTop;
    }
    return p;
}

/* Defined last: its .rodata follows menuConfig_setOption's jump table. */
/* menu text block */
INCLUDE_RODATA("asm/us/ovl_34/nonmatchings/ovl_34", g_configMenuText);
