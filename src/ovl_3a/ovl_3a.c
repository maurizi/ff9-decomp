#include "ovl_3a.h"

void menuName_init(void) {
    u32 id;
    s32 i;

    text_setBank(g_nameMenuText);
    help_setBounds(&s_menuNameHelpBounds);
    help_setCallback(menuName_updateHelp);
    s_menuName.focus = NAME_FOCUS_GRID;
    s_menuName.menuCursor = 0;
    s_menuName.charSet = 0;
    s_menuName.gridCursor = 0;
    s_menuName.startDefault = g_menuArg < NAME_ARG_CURRENT;
    s_menuName.nameId = menuName_getNameId(s_menuName.startDefault ? g_menuArg : g_menuArg - NAME_ARG_CURRENT);
    s_menuName.charIdx = menuName_getCharIdx(s_menuName.nameId);
    for (i = 0; i < (s32)ARRAY_COUNT(s_menuName.gridSize); i++) {
        s_menuName.gridSize[i] = s_menuNameGridSize[i];
    }
    chr_setDefaultName(s_menuName.defaultName, s_menuName.nameId);
    memset(s_menuName.name, TEXT_END, sizeof(s_menuName.name));
    if (s_menuName.startDefault) {
        chr_setDefaultName(s_menuName.name, s_menuName.nameId);
    } else {
        text_copy(s_menuName.name, g_gameState->gameData->characters[s_menuName.charIdx].name);
    }
    s_menuName.len = text_getLength(s_menuName.name);
    for (i = 0; i < (s32)ARRAY_COUNT(s_menuName.charSets); i++) {
        s_menuName.charSets[i] = text_get(i + NAME_TEXT_CHARSET);
    }
    setRECT(&s_menuName.windows[0], NAME_TITLE_WIN_X, NAME_TITLE_WIN_Y, NAME_TITLE_WIN_W, NAME_TITLE_WIN_H);
    setRECT(&s_menuName.windows[1], NAME_PORTRAIT_WIN_X, NAME_PORTRAIT_WIN_Y, NAME_PORTRAIT_WIN_W, NAME_PORTRAIT_WIN_H);
    setRECT(&s_menuName.windows[2], NAME_CMD_WIN_X, NAME_CMD_WIN_Y, NAME_CMD_WIN_W, NAME_CMD_WIN_H);
    setRECT(&s_menuName.windows[3], NAME_HELP_WIN_X, NAME_HELP_WIN_Y, NAME_HELP_WIN_W, NAME_HELP_WIN_H);
    setRECT(&s_menuName.windows[4], NAME_NAME_WIN_X, NAME_NAME_WIN_Y, NAME_NAME_WIN_W, NAME_NAME_WIN_H);
    menuName_loadPortrait(s_menuName.charIdx);
}

void menuName_input(void) {
    if (!fade_isBusy()) {
        if (g_padRepeat & PAD_START) {
            snd_playSe(SE_CURSOR);
            s_menuName.focus = NAME_FOCUS_MENU;
            s_menuName.menuCursor = NAME_MENU_CONFIRM;
            help_setBounds(NULL);
            return;
        }
        if (g_padRepeat & BTN_L1) {
            snd_playSe(SE_CURSOR);
            s_menuName.charSet = s_menuName.charSet == 0 ? 1 : s_menuName.charSet - 1;
            menuName_clampCursor();
            return;
        }
        if (g_padRepeat & BTN_R1) {
            snd_playSe(SE_CURSOR);
            s_menuName.charSet = s_menuName.charSet != 0 ? 0 : s_menuName.charSet + 1;
            menuName_clampCursor();
            return;
        }
        if (g_padRepeat & BTN_CROSS) {
            menuName_confirm();
            return;
        }
        if (s_menuName.focus == NAME_FOCUS_MENU) {
            menuName_inputCommands();
            return;
        }
        menuName_inputGrid();
    }
}

void menuName_inputGrid(void) {
    if ((g_padRepeat & PAD_UP) && s_menuName.gridCursor >= NAME_GRID_COLS) {
        snd_playSe(SE_CURSOR);
        s_menuName.gridCursor -= NAME_GRID_COLS;
        return;
    }
    if ((g_padRepeat & PAD_DOWN) && s_menuName.gridCursor < s_menuName.gridSize[s_menuName.charSet] - NAME_GRID_COLS) {
        snd_playSe(SE_CURSOR);
        s_menuName.gridCursor += NAME_GRID_COLS;
        return;
    }
    if ((g_padRepeat & PAD_RIGHT) && (u8)(s_menuName.gridCursor % NAME_GRID_COLS) != NAME_GRID_COLS - 1) {
        snd_playSe(SE_CURSOR);
        s_menuName.gridCursor++;
        return;
    }
    if (g_padRepeat & PAD_LEFT) {
        snd_playSe(SE_CURSOR);
        if ((u8)(s_menuName.gridCursor % NAME_GRID_COLS) != 0) {
            s_menuName.gridCursor--;
            return;
        }
        s_menuName.focus = NAME_FOCUS_MENU;
        help_setBounds(NULL);
        return;
    }
    if (g_padRepeat & BTN_CIRCLE) {
        snd_playSe(SE_CANCEL);
        if (s_menuName.len != 0) {
            s_menuName.len--;
            s_menuName.name[s_menuName.len] = TEXT_END;
        }
    }
}

void menuName_confirm(void) {
    if (s_menuName.focus == NAME_FOCUS_MENU) {
        switch (s_menuName.menuCursor) {
        case NAME_MENU_ALPHABET:
        case NAME_MENU_SYMBOL:
            s_menuName.charSet = s_menuName.menuCursor;
            menuName_clampCursor();
            snd_playSe(SE_CURSOR);
            break;
        case NAME_MENU_DELETE:
            snd_playSe(SE_CANCEL);
            if (s_menuName.len != 0) {
                s_menuName.len--;
                s_menuName.name[s_menuName.len] = TEXT_END;
            }
            break;
        case NAME_MENU_CONFIRM:
            if (s_menuName.len != 0) {
                snd_playSe(SE_CURSOR);
                memcpy(g_gameState->gameData->characters[s_menuName.charIdx].name, s_menuName.name, NAME_MAX_LEN);
                menu_close();
            } else {
                snd_playSe(SE_BUZZER);
            }
            break;
        case NAME_MENU_DEFAULT:
            snd_playSe(SE_CURSOR);
            s_menuName.len = text_getLength(s_menuName.defaultName);
            memset((s32)s_menuName.name, TEXT_END, NAME_MAX_LEN);
            memcpy(s_menuName.name, s_menuName.defaultName, s_menuName.len);
            break;
        }
    } else {
        snd_playSe(SE_CURSOR);
        s_menuName.name[s_menuName.len >= NAME_MAX_LEN - 1 ? s_menuName.len - 1 : s_menuName.len] =
            s_menuName.charSets[s_menuName.charSet][s_menuName.gridCursor];
        s_menuName.len = s_menuName.len >= NAME_MAX_LEN - 1 ? s_menuName.len : s_menuName.len + 1;
    }
}

void menuName_drawPortrait(void) {
    SprtWords *sprt;
    u32 clut = (GetClut(0, NAME_PORTRAIT_CLUT_Y) << 16) | (NAME_PORTRAIT_V << 8);

    setlen(g_primPtr, PRIM_LEN(SPRT));
    sprt = (SprtWords *)g_primPtr;
    sprt->rgbc = GPU_RGBC(GPU_SPRT, GPU_RGB_NEUTRAL);
    sprt->xy = (s_menuNamePortraitWin->x + NAME_PORTRAIT_INSET) | ((s_menuNamePortraitWin->y + NAME_PORTRAIT_INSET) << 16);
    sprt->uvclut = clut;
    sprt->wh = (NAME_PORTRAIT_H << 16) | NAME_PORTRAIT_W;
    addPrim(&g_ot[NAME_OTZ_PORTRAIT], sprt);
    g_primPtr = (u8 *)(sprt + 1);
    setlen(g_primPtr, 2);
    SetDrawTPage(g_primPtr, 0, 1, getTPage(1, 0, NAME_PORTRAIT_VRAM_X, NAME_PORTRAIT_VRAM_Y));
    addPrim(&g_ot[NAME_OTZ_PORTRAIT], g_primPtr);
    g_primPtr += 8;
    win_drawBox(s_menuNamePortraitWin, NAME_OTZ_BOX, WIN_STYLE_LINE);
    win_drawShadow(s_menuNamePortraitWin, NAME_OTZ_BOX, WIN_STYLE_LINE);
}

void menuName_drawCommands(void) {
    s32 i;

    ui_drawEtchedLine(s_menuNameCommandWin->x + 4, s_menuNameCommandWin->y + NAME_CMD_DIV_Y, NAME_OTZ_CMD_BOX, s_menuNameCommandWin->w - 9);
    win_drawBox(s_menuNameCommandWin, NAME_OTZ_CMD_BOX, WIN_STYLE_BEVEL);
    win_drawShadow(s_menuNameCommandWin, NAME_OTZ_CMD_BOX, WIN_STYLE_BEVEL);
    for (i = 0; i < 2; i++) {
        text_drawBlock(s_menuNameCommandWin->x + NAME_TEXT_X, s_menuNameCommandWin->y + NAME_CMD_SET_Y + i * NAME_CMD_ROW_H, NAME_OTZ_CMD_TEXT, text_get(i + NAME_TEXT_ALPHABET),
                      s_menuName.charSet == i ? TEXT_PAL_YELLOW : TEXT_PAL_WHITE);
    }
    for (i = 0; i < NAME_MENU_DEFAULT - NAME_MENU_DELETE + 1; i++) {
        text_drawBlock(s_menuNameCommandWin->x + NAME_TEXT_X, s_menuNameCommandWin->y + NAME_CMD_ACTION_Y + i * NAME_CMD_ROW_H, NAME_OTZ_CMD_TEXT, text_get(i + NAME_TEXT_DELETE), TEXT_PAL_WHITE);
    }
}

void menuName_drawNameWindow(void) {
    LINE_F2 *line;
    s32 cursor;
    s32 i;
    s32 j;
    s32 k;
    s32 y;

    k = 0;
    cursor = NAME_MAX_LEN - 2;
    if (s_menuName.len < NAME_MAX_LEN - 2) {
        cursor = s_menuName.len;
    }
    ui_drawLabel(s_menuNameNameWin->x + NAME_RULE_LABEL_X, s_menuNameNameWin->y + NAME_RULE_LABEL_Y, NAME_OTZ_TEXT, g_labelName, UI_LABEL_NORMAL);
    ui_drawEtchedLine(s_menuNameNameWin->x + 4, s_menuNameNameWin->y + NAME_DIV_Y, NAME_OTZ_BOX, s_menuNameNameWin->w - 8);
    win_drawBox(s_menuNameNameWin, NAME_OTZ_BOX, WIN_STYLE_LINE_RULE);
    win_drawShadow(s_menuNameNameWin, NAME_OTZ_BOX, WIN_STYLE_LINE_RULE);
    for (i = 0; i < NAME_MAX_LEN - 1; i++) {
        if (cursor != i || (g_menu.frame & 4)) {
            for (j = 0; j < 2; j++) {
                line = (LINE_F2 *)g_primPtr;
                SetLineF2(line);
                if (j == 0) {
                    setRGB0(line, 0xB0, 0xB0, 0xB0);
                } else {
                    setRGB0(line, 0x40, 0x40, 0x40);
                }
                y = j + NAME_SLOT_LINE_Y;
                line->x0 = s_menuNameNameWin->x + (i * NAME_SLOT_W + NAME_SLOT_X) + j;
                line->y0 = line->y1 = s_menuNameNameWin->y + y;
                line->x1 = line->x0 + NAME_SLOT_LINE_LEN;
                g_primPtr += sizeof(LINE_F2);
                addPrim(&g_ot[NAME_OTZ_TEXT], line);
            }
        }
    }
    for (i = 0; i < s_menuName.len; i++) {
        text_drawChar(s_menuNameNameWin->x + (NAME_SLOT_X + 1) + i * NAME_SLOT_W, s_menuNameNameWin->y + NAME_LETTER_Y, NAME_OTZ_TEXT, s_menuName.name[i], TEXT_PAL_WHITE);
    }
    for (i = 0; i < (u8)(s_menuName.gridSize[s_menuName.charSet] / NAME_GRID_COLS); i++) {
        for (j = 0; j < NAME_GRID_COLS; j++) {
            text_drawChar(s_menuNameNameWin->x + NAME_GRID_X + j * NAME_GRID_CELL_W, s_menuNameNameWin->y + NAME_GRID_Y + i * NAME_GRID_ROW_H, NAME_OTZ_TEXT,
                          s_menuName.charSets[s_menuName.charSet][k], TEXT_PAL_WHITE);
            k++;
        }
    }
    g_primPtr = text_buildFontTpage(&g_ot[NAME_OTZ_TEXT], g_primPtr);
}

void menuName_drawCursor(void) {
    RECT *gridRect;
    s32 y;
    s32 row;
    u32 idx;
    u32 col;
    u32 sub;

    if (s_menuName.focus == NAME_FOCUS_MENU) {
        HelpState *h;
        g_menuHelp.x = s_menuName.windows[2].x + NAME_CURSOR_X;
        h = &g_menuHelp;
        y = s_menuName.windows[2].y + s_menuName.menuCursor * NAME_CMD_ROW_H;
        h->y = s_menuName.menuCursor >= NAME_MENU_DELETE ? y + (NAME_CMD_ACTION_Y - 2 * NAME_CMD_ROW_H + NAME_CURSOR_DY) : y + (NAME_CMD_SET_Y + NAME_CURSOR_DY);
    } else {
        gridRect = &s_menuName.windows[4];
        if (s_menuName.charSet != 0) {
            row = s_menuName.gridCursor / NAME_GRID_COLS;
            idx = s_menuName.gridCursor % NAME_GRID_COLS;
            g_menuHelp.y = gridRect->y + row * NAME_GRID_ROW_H + (NAME_GRID_Y + NAME_CURSOR_DY);
            col = (u8)(idx / NAME_GROUP_COLS);
            sub = (u8)(idx % NAME_GROUP_COLS);
            g_menuHelp.x = gridRect->x + grp_x(col) + sub * NAME_GROUP_CELL_W + NAME_GRID_CURSOR_DX;
        } else {
            row = s_menuName.gridCursor / NAME_GRID_COLS;
            col = s_menuName.gridCursor % NAME_GRID_COLS;
            g_menuHelp.x = gridRect->x + col * NAME_GRID_CELL_W + (NAME_GRID_X + NAME_GRID_CURSOR_DX);
            g_menuHelp.y = gridRect->y + row * NAME_GRID_ROW_H + (NAME_GRID_Y + NAME_CURSOR_DY);
        }
    }
    g_menuHelp.otz = NAME_OTZ_CURSOR;
    help_drawCursor();
}

void menuName_update(void) {
    menuName_input();
    menuName_draw();
}

void menuName_exit(void) {
    help_setCallback(NULL);
    help_setBounds(NULL);
    menuName_restoreClut();
}

void menuName_draw(void) {
    menuName_drawTitle();
    menuName_drawPortrait();
    menuName_drawCommands();
    menuName_drawHelp();
    menuName_drawNameWindow();
    menuName_drawCursor();
}

void menuName_inputCommands(void) {
    if (g_padRepeat & PAD_UP) {
        snd_playSe(SE_CURSOR);
        s_menuName.menuCursor = s_menuName.menuCursor == 0 ? NAME_MENU_DEFAULT : s_menuName.menuCursor - 1;
        return;
    }
    if (g_padRepeat & PAD_DOWN) {
        snd_playSe(SE_CURSOR);
        s_menuName.menuCursor = s_menuName.menuCursor >= NAME_MENU_DEFAULT ? 0 : s_menuName.menuCursor + 1;
        return;
    }
    if (g_padRepeat & PAD_RIGHT) {
        snd_playSe(SE_CURSOR);
        s_menuName.focus = NAME_FOCUS_GRID;
        help_setBounds(&s_menuNameHelpBounds);
    }
}

void menuName_drawTitle(void) {
    win_drawBox(s_menuNameTitleWin, NAME_OTZ_BOX, WIN_STYLE_BEVEL);
    win_drawShadow(s_menuNameTitleWin, NAME_OTZ_BOX, WIN_STYLE_BEVEL);
    text_drawBlock(s_menuNameTitleWin->x + NAME_TEXT_X, s_menuNameTitleWin->y + NAME_TEXT_Y, NAME_OTZ_TEXT, text_get(NAME_TEXT_TITLE), TEXT_PAL_WHITE);
}

void menuName_drawHelp(void) {
    win_drawBox(s_menuNameHelpWin, NAME_OTZ_HELP_BOX, WIN_STYLE_LINE_RULE);
    win_drawShadow(s_menuNameHelpWin, NAME_OTZ_HELP_BOX, WIN_STYLE_LINE_RULE);
    text_drawBlock(s_menuNameHelpWin->x + NAME_PROMPT_X, s_menuNameHelpWin->y + NAME_TEXT_Y, NAME_OTZ_HELP_TEXT, text_get(NAME_TEXT_PROMPT), TEXT_PAL_WHITE);
    ui_drawLabel(s_menuNameHelpWin->x + NAME_RULE_LABEL_X, s_menuNameHelpWin->y + NAME_RULE_LABEL_Y, NAME_OTZ_HELP_TEXT, g_labelHelp, UI_LABEL_NORMAL);
}

void menuName_loadPortrait(s32 chr) {
    u8 *img;

    img = s_menuNamePortraits[chr_getPortrait(chr)];
    StoreImage(&s_menuNameClutRect, (u32 *)s_menuName.savedClut);
    DrawSync(0);
    LoadImage(&s_menuNameClutRect, (u32 *)img);
    DrawSync(0);
    LoadImage(&s_menuNamePortraitRect, (u32 *)(img + 0x100));
    DrawSync(0);
}

void menuName_restoreClut(void) {
    LoadImage(&s_menuNameClutRestoreRect, (u32 *)s_menuName.savedClut);
    DrawSync(0);
}

s32 menuName_getNameId(s32 i) {
    return s_menuNameIds[i];
}

s32 menuName_getCharIdx(s32 i) {
    return s_menuNameCharIdx[i];
}

void menuName_updateHelp(void) {
    g_menuHelp.text = text_get(s_menuName.focus == NAME_FOCUS_MENU ? s_menuName.menuCursor + NAME_TEXT_MENU_HELP : NAME_TEXT_GRID_HELP);
}

void menuName_clampCursor(void) {
    if (s_menuName.charSet == 1 && s_menuName.gridCursor >= s_menuName.gridSize[1]) {
        s_menuName.gridCursor = s_menuName.gridSize[1] + (u8)(s_menuName.gridCursor % NAME_GRID_COLS - NAME_GRID_COLS);
    }
}

INCLUDE_RODATA("asm/us/ovl_3a/nonmatchings/ovl_3a", g_nameMenuText);

INCLUDE_RODATA("asm/us/ovl_3a/nonmatchings/ovl_3a", s_menuNamePortraits);
