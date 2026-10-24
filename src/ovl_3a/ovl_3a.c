#include "ovl_3a.h"

void menuName_init(void) {
    u32 id;
    s32 i;

    text_setBank(g_nameMenuText);
    func_80035580(&s_menuNameHelpBounds);
    func_8003556C(menuName_updateHelp);
    s_menuName.focus = NAME_FOCUS_GRID;
    s_menuName.menuCursor = 0;
    s_menuName.charSet = 0;
    s_menuName.gridCursor = 0;
    s_menuName.startDefault = D_80077DBC < 12;
    s_menuName.nameId = menuName_getNameId(s_menuName.startDefault ? D_80077DBC : D_80077DBC - 12);
    s_menuName.charIdx = menuName_getCharIdx(s_menuName.nameId);
    for (i = 0; i < 2; i++) {
        s_menuName.gridSize[i] = s_menuNameGridSize[i];
    }
    chr_setDefaultName(s_menuName.defaultName, s_menuName.nameId);
    memset(s_menuName.name, 0xFF, 10);
    if (s_menuName.startDefault) {
        chr_setDefaultName(s_menuName.name, s_menuName.nameId);
    } else {
        func_8002BD54(s_menuName.name, g_gameState->unk1C->characters[s_menuName.charIdx].unk0);
    }
    s_menuName.len = func_8002BCC0(s_menuName.name);
    for (i = 0; i < 2; i++) {
        s_menuName.charSets[i] = text_get(i + NAME_TEXT_CHARSET);
    }
    setRECT(&s_menuName.windows[0], 0xF0, 8, 0x44, 0x1A);
    setRECT(&s_menuName.windows[1], 0x16, 0x1A, 0x24, 0x32);
    setRECT(&s_menuName.windows[2], 0xE, 0x52, 0x44, 0x69);
    setRECT(&s_menuName.windows[3], 0x1C, 0x26, 0x118, 0x18);
    setRECT(&s_menuName.windows[4], 0x1C, 0x3E, 0x118, 0xA0);
    menuName_loadPortrait(s_menuName.charIdx);
}

void menuName_input(void) {
    if (!func_8002F810()) {
        if (g_padRepeat & PAD_START) {
            snd_playSe(SE_CURSOR);
            s_menuName.focus = NAME_FOCUS_MENU;
            s_menuName.menuCursor = NAME_MENU_CONFIRM;
            func_80035580(NULL);
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
        func_80035580(NULL);
        return;
    }
    if (g_padRepeat & BTN_CIRCLE) {
        snd_playSe(SE_CANCEL);
        if (s_menuName.len != 0) {
            s_menuName.len--;
            s_menuName.name[s_menuName.len] = 0xFF;
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
                s_menuName.name[s_menuName.len] = 0xFF;
            }
            break;
        case NAME_MENU_CONFIRM:
            if (s_menuName.len != 0) {
                snd_playSe(SE_CURSOR);
                memcpy(g_gameState->unk1C->characters[s_menuName.charIdx].unk0, s_menuName.name, NAME_MAX_LEN);
                func_800325E0();
            } else {
                snd_playSe(SE_BUZZER);
            }
            break;
        case NAME_MENU_DEFAULT:
            snd_playSe(SE_CURSOR);
            s_menuName.len = func_8002BCC0(s_menuName.defaultName);
            memset((s32)s_menuName.name, 0xFF, NAME_MAX_LEN);
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
    SprtWords *p;
    u32 clut = (GetClut(0, 0x100) << 16) | 0xB000;

    setlen(g_primPtr, 4);
    p = (SprtWords *)g_primPtr;
    p->rgbc = 0x64808080;
    p->xy = (s_menuNamePortraitWin->x + 2) | ((s_menuNamePortraitWin->y + 2) << 16);
    p->uvclut = clut;
    p->wh = 0x2E0020;
    addPrim(&g_ot[4], p);
    g_primPtr = (u8 *)(p + 1);
    setlen(g_primPtr, 2);
    SetDrawTPage(g_primPtr, 0, 1, 0x9F);
    addPrim(&g_ot[4], g_primPtr);
    g_primPtr += 8;
    func_8002DEE4(s_menuNamePortraitWin, 6, 0);
    func_8002705C(s_menuNamePortraitWin, 6, 0);
}

void menuName_drawCommands(void) {
    s32 i;

    func_800269F4(s_menuNameCommandWin->x + 4, s_menuNameCommandWin->y + 0x2A, 5, s_menuNameCommandWin->w - 9);
    func_8002DEE4(s_menuNameCommandWin, 5, 2);
    func_8002705C(s_menuNameCommandWin, 5, 2);
    for (i = 0; i < 2; i++) {
        text_drawBlock(s_menuNameCommandWin->x + 0xA, s_menuNameCommandWin->y + 9 + i * 0x10, 4, text_get(i + NAME_TEXT_ALPHABET),
                      s_menuName.charSet == i ? 4 : 0);
    }
    for (i = 0; i < 3; i++) {
        text_drawBlock(s_menuNameCommandWin->x + 0xA, s_menuNameCommandWin->y + 0x31 + i * 0x10, 4, text_get(i + NAME_TEXT_DELETE), 0);
    }
}

void menuName_drawNameWindow(void) {
    LINE_F2 *p;
    s32 cursor;
    s32 i;
    s32 j;
    s32 k;
    s32 y;

    k = 0;
    cursor = 6;
    if (s_menuName.len < 6) {
        cursor = s_menuName.len;
    }
    ui_drawLabel(s_menuNameNameWin->x + 0x23, s_menuNameNameWin->y - 4, 5, D_8007047C, 0);
    func_800269F4(s_menuNameNameWin->x + 4, s_menuNameNameWin->y + 0x1A, 6, s_menuNameNameWin->w - 8);
    func_8002DEE4(s_menuNameNameWin, 6, 1);
    func_8002705C(s_menuNameNameWin, 6, 1);
    for (i = 0; i < 7; i++) {
        if (cursor != i || (g_menu.unk10 & 4)) {
            for (j = 0; j < 2; j++) {
                p = (LINE_F2 *)g_primPtr;
                SetLineF2(p);
                if (j == 0) {
                    setRGB0(p, 0xB0, 0xB0, 0xB0);
                } else {
                    setRGB0(p, 0x40, 0x40, 0x40);
                }
                y = j + 0x15;
                p->x0 = s_menuNameNameWin->x + (i * 16 + 0x3E) + j;
                p->y0 = p->y1 = s_menuNameNameWin->y + y;
                p->x1 = p->x0 + 0xB;
                g_primPtr += sizeof(LINE_F2);
                addPrim(&g_ot[5], p);
            }
        }
    }
    for (i = 0; i < s_menuName.len; i++) {
        func_80028F3C(s_menuNameNameWin->x + 0x3F + i * 16, s_menuNameNameWin->y + 7, 5, s_menuName.name[i], 0);
    }
    for (i = 0; i < (u8)(s_menuName.gridSize[s_menuName.charSet] / NAME_GRID_COLS); i++) {
        for (j = 0; j < NAME_GRID_COLS; j++) {
            func_80028F3C(s_menuNameNameWin->x + 0x3E + j * 14, s_menuNameNameWin->y + 0x22 + i * 21, 5,
                          s_menuName.charSets[s_menuName.charSet][k], 0);
            k++;
        }
    }
    g_primPtr = func_8002BA90(&g_ot[5], g_primPtr);
}

void menuName_drawCursor(void) {
    RECT *r;
    s32 y;
    s32 row;
    u32 idx;
    u32 col;
    u32 sub;

    if (s_menuName.focus == NAME_FOCUS_MENU) {
        Unk80077DE8 *h;
        D_80077DE8.unk0 = s_menuName.windows[2].x - 0xE;
        h = &D_80077DE8;
        y = s_menuName.windows[2].y + s_menuName.menuCursor * 16;
        h->unk2 = s_menuName.menuCursor >= 2 ? y + 0x15 : y + 0xD;
    } else {
        r = &s_menuName.windows[4];
        if (s_menuName.charSet != 0) {
            row = s_menuName.gridCursor / NAME_GRID_COLS;
            idx = s_menuName.gridCursor % NAME_GRID_COLS;
            D_80077DE8.unk2 = r->y + row * 21 + 0x26;
            col = (u8)(idx / 5);
            sub = (u8)(idx % 5);
            D_80077DE8.unk0 = r->x + grp_x(col) + sub * 13 - 0x17;
        } else {
            row = s_menuName.gridCursor / NAME_GRID_COLS;
            col = s_menuName.gridCursor % NAME_GRID_COLS;
            D_80077DE8.unk0 = r->x + col * 14 + 0x27;
            D_80077DE8.unk2 = r->y + row * 21 + 0x26;
        }
    }
    D_80077DEC = 3;
    func_800355E4();
}

void menuName_update(void) {
    menuName_input();
    menuName_draw();
}

void menuName_exit(void) {
    func_8003556C(NULL);
    func_80035580(NULL);
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
        func_80035580(&s_menuNameHelpBounds);
    }
}

void menuName_drawTitle(void) {
    func_8002DEE4(s_menuNameTitleWin, 6, 2);
    func_8002705C(s_menuNameTitleWin, 6, 2);
    text_drawBlock(s_menuNameTitleWin->x + 0xA, s_menuNameTitleWin->y + 6, 5, text_get(NAME_TEXT_TITLE), 0);
}

void menuName_drawHelp(void) {
    func_8002DEE4(s_menuNameHelpWin, 7, 1);
    func_8002705C(s_menuNameHelpWin, 7, 1);
    text_drawBlock(s_menuNameHelpWin->x + 0x3E, s_menuNameHelpWin->y + 6, 6, text_get(NAME_TEXT_PROMPT), 0);
    ui_drawLabel(s_menuNameHelpWin->x + 0x23, s_menuNameHelpWin->y - 4, 6, D_80070450, 0);
}

void menuName_loadPortrait(s32 chr) {
    u8 *img;

    img = s_menuNamePortraits[func_80028954(chr)];
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
    D_80077DE8.unk8 = text_get(s_menuName.focus == NAME_FOCUS_MENU ? s_menuName.menuCursor + NAME_TEXT_MENU_HELP : NAME_TEXT_GRID_HELP);
}

void menuName_clampCursor(void) {
    if (s_menuName.charSet == 1 && s_menuName.gridCursor >= s_menuName.gridSize[1]) {
        s_menuName.gridCursor = s_menuName.gridSize[1] + (u8)(s_menuName.gridCursor % NAME_GRID_COLS - NAME_GRID_COLS);
    }
}

INCLUDE_RODATA("asm/us/ovl_3a/nonmatchings/ovl_3a", g_nameMenuText);

INCLUDE_RODATA("asm/us/ovl_3a/nonmatchings/ovl_3a", s_menuNamePortraits);
