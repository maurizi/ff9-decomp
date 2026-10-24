#include "ovl_3a.h"

void menu_name_init(void) {
    u32 id;
    s32 i;

    text_setBank(g_nameMenuText);
    func_80035580(&g_menuNameHelpRect);
    func_8003556C(menu_name_setHelpText);
    g_menuName.focus = NAME_FOCUS_GRID;
    g_menuName.menuCursor = 0;
    g_menuName.charSet = 0;
    g_menuName.gridCursor = 0;
    g_menuName.startDefault = D_80077DBC < 12;
    g_menuName.nameId = menu_name_getNameId(g_menuName.startDefault ? D_80077DBC : D_80077DBC - 12);
    g_menuName.charIdx = menu_name_getCharIdx(g_menuName.nameId);
    for (i = 0; i < 2; i++) {
        g_menuName.gridSize[i] = g_menuNameGridSize[i];
    }
    chr_setDefaultName(g_menuName.defaultName, g_menuName.nameId);
    memset(g_menuName.name, 0xFF, 10);
    if (g_menuName.startDefault) {
        chr_setDefaultName(g_menuName.name, g_menuName.nameId);
    } else {
        func_8002BD54(g_menuName.name, g_gameState->unk1C->characters[g_menuName.charIdx].unk0);
    }
    g_menuName.len = func_8002BCC0(g_menuName.name);
    for (i = 0; i < 2; i++) {
        g_menuName.charSets[i] = text_get(i + NAME_TEXT_CHARSET);
    }
    setRECT(&g_menuName.windows[0], 0xF0, 8, 0x44, 0x1A);
    setRECT(&g_menuName.windows[1], 0x16, 0x1A, 0x24, 0x32);
    setRECT(&g_menuName.windows[2], 0xE, 0x52, 0x44, 0x69);
    setRECT(&g_menuName.windows[3], 0x1C, 0x26, 0x118, 0x18);
    setRECT(&g_menuName.windows[4], 0x1C, 0x3E, 0x118, 0xA0);
    menu_name_loadPortrait(g_menuName.charIdx);
}

void menu_name_input(void) {
    if (!func_8002F810()) {
        if (g_padRepeat & PAD_START) {
            snd_playSe(SE_CURSOR);
            g_menuName.focus = NAME_FOCUS_MENU;
            g_menuName.menuCursor = NAME_MENU_CONFIRM;
            func_80035580(NULL);
            return;
        }
        if (g_padRepeat & BTN_L1) {
            snd_playSe(SE_CURSOR);
            g_menuName.charSet = g_menuName.charSet == 0 ? 1 : g_menuName.charSet - 1;
            menu_name_clampCursor();
            return;
        }
        if (g_padRepeat & BTN_R1) {
            snd_playSe(SE_CURSOR);
            g_menuName.charSet = g_menuName.charSet != 0 ? 0 : g_menuName.charSet + 1;
            menu_name_clampCursor();
            return;
        }
        if (g_padRepeat & BTN_CROSS) {
            menu_name_select();
            return;
        }
        if (g_menuName.focus == NAME_FOCUS_MENU) {
            menu_name_moveInMenu();
            return;
        }
        menu_name_moveInGrid();
    }
}

void menu_name_moveInGrid(void) {
    if ((g_padRepeat & PAD_UP) && g_menuName.gridCursor >= NAME_GRID_COLS) {
        snd_playSe(SE_CURSOR);
        g_menuName.gridCursor -= NAME_GRID_COLS;
        return;
    }
    if ((g_padRepeat & PAD_DOWN) && g_menuName.gridCursor < g_menuName.gridSize[g_menuName.charSet] - NAME_GRID_COLS) {
        snd_playSe(SE_CURSOR);
        g_menuName.gridCursor += NAME_GRID_COLS;
        return;
    }
    if ((g_padRepeat & PAD_RIGHT) && (u8)(g_menuName.gridCursor % NAME_GRID_COLS) != NAME_GRID_COLS - 1) {
        snd_playSe(SE_CURSOR);
        g_menuName.gridCursor++;
        return;
    }
    if (g_padRepeat & PAD_LEFT) {
        snd_playSe(SE_CURSOR);
        if ((u8)(g_menuName.gridCursor % NAME_GRID_COLS) != 0) {
            g_menuName.gridCursor--;
            return;
        }
        g_menuName.focus = NAME_FOCUS_MENU;
        func_80035580(NULL);
        return;
    }
    if (g_padRepeat & BTN_CIRCLE) {
        snd_playSe(SE_CANCEL);
        if (g_menuName.len != 0) {
            g_menuName.len--;
            g_menuName.name[g_menuName.len] = 0xFF;
        }
    }
}

void menu_name_select(void) {
    if (g_menuName.focus == NAME_FOCUS_MENU) {
        switch (g_menuName.menuCursor) {
        case NAME_MENU_ALPHABET:
        case NAME_MENU_SYMBOL:
            g_menuName.charSet = g_menuName.menuCursor;
            menu_name_clampCursor();
            snd_playSe(SE_CURSOR);
            break;
        case NAME_MENU_DELETE:
            snd_playSe(SE_CANCEL);
            if (g_menuName.len != 0) {
                g_menuName.len--;
                g_menuName.name[g_menuName.len] = 0xFF;
            }
            break;
        case NAME_MENU_CONFIRM:
            if (g_menuName.len != 0) {
                snd_playSe(SE_CURSOR);
                memcpy(g_gameState->unk1C->characters[g_menuName.charIdx].unk0, g_menuName.name, NAME_MAX_LEN);
                func_800325E0();
            } else {
                snd_playSe(SE_BUZZER);
            }
            break;
        case NAME_MENU_DEFAULT:
            snd_playSe(SE_CURSOR);
            g_menuName.len = func_8002BCC0(g_menuName.defaultName);
            memset((s32)g_menuName.name, 0xFF, NAME_MAX_LEN);
            memcpy(g_menuName.name, g_menuName.defaultName, g_menuName.len);
            break;
        }
    } else {
        snd_playSe(SE_CURSOR);
        g_menuName.name[g_menuName.len >= NAME_MAX_LEN - 1 ? g_menuName.len - 1 : g_menuName.len] =
            g_menuName.charSets[g_menuName.charSet][g_menuName.gridCursor];
        g_menuName.len = g_menuName.len >= NAME_MAX_LEN - 1 ? g_menuName.len : g_menuName.len + 1;
    }
}

void menu_name_drawPortrait(void) {
    SprtWords *p;
    u32 clut = (GetClut(0, 0x100) << 16) | 0xB000;

    setlen(g_primPtr, 4);
    p = (SprtWords *)g_primPtr;
    p->rgbc = 0x64808080;
    p->xy = (g_menuNamePortraitWin->x + 2) | ((g_menuNamePortraitWin->y + 2) << 16);
    p->uvclut = clut;
    p->wh = 0x2E0020;
    addPrim(&g_ot[4], p);
    g_primPtr = (u8 *)(p + 1);
    setlen(g_primPtr, 2);
    SetDrawTPage(g_primPtr, 0, 1, 0x9F);
    addPrim(&g_ot[4], g_primPtr);
    g_primPtr += 8;
    func_8002DEE4(g_menuNamePortraitWin, 6, 0);
    func_8002705C(g_menuNamePortraitWin, 6, 0);
}

void menu_name_drawMenu(void) {
    s32 i;

    func_800269F4(g_menuNameMenuWin->x + 4, g_menuNameMenuWin->y + 0x2A, 5, g_menuNameMenuWin->w - 9);
    func_8002DEE4(g_menuNameMenuWin, 5, 2);
    func_8002705C(g_menuNameMenuWin, 5, 2);
    for (i = 0; i < 2; i++) {
        text_drawBlock(g_menuNameMenuWin->x + 0xA, g_menuNameMenuWin->y + 9 + i * 0x10, 4, text_get(i + NAME_TEXT_ALPHABET),
                      g_menuName.charSet == i ? 4 : 0);
    }
    for (i = 0; i < 3; i++) {
        text_drawBlock(g_menuNameMenuWin->x + 0xA, g_menuNameMenuWin->y + 0x31 + i * 0x10, 4, text_get(i + NAME_TEXT_DELETE), 0);
    }
}

void menu_name_drawEntry(void) {
    LINE_F2 *p;
    s32 cursor;
    s32 i;
    s32 j;
    s32 k;
    s32 y;

    k = 0;
    cursor = 6;
    if (g_menuName.len < 6) {
        cursor = g_menuName.len;
    }
    ui_drawLabel(g_menuNameEntryWin->x + 0x23, g_menuNameEntryWin->y - 4, 5, D_8007047C, 0);
    func_800269F4(g_menuNameEntryWin->x + 4, g_menuNameEntryWin->y + 0x1A, 6, g_menuNameEntryWin->w - 8);
    func_8002DEE4(g_menuNameEntryWin, 6, 1);
    func_8002705C(g_menuNameEntryWin, 6, 1);
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
                p->x0 = g_menuNameEntryWin->x + (i * 16 + 0x3E) + j;
                p->y0 = p->y1 = g_menuNameEntryWin->y + y;
                p->x1 = p->x0 + 0xB;
                g_primPtr += sizeof(LINE_F2);
                addPrim(&g_ot[5], p);
            }
        }
    }
    for (i = 0; i < g_menuName.len; i++) {
        func_80028F3C(g_menuNameEntryWin->x + 0x3F + i * 16, g_menuNameEntryWin->y + 7, 5, g_menuName.name[i], 0);
    }
    for (i = 0; i < (u8)(g_menuName.gridSize[g_menuName.charSet] / NAME_GRID_COLS); i++) {
        for (j = 0; j < NAME_GRID_COLS; j++) {
            func_80028F3C(g_menuNameEntryWin->x + 0x3E + j * 14, g_menuNameEntryWin->y + 0x22 + i * 21, 5,
                          g_menuName.charSets[g_menuName.charSet][k], 0);
            k++;
        }
    }
    g_primPtr = func_8002BA90(&g_ot[5], g_primPtr);
}

void menu_name_drawCursor(void) {
    RECT *r;
    s32 y;
    s32 row;
    u32 idx;
    u32 col;
    u32 sub;

    if (g_menuName.focus == NAME_FOCUS_MENU) {
        Unk80077DE8 *h;
        D_80077DE8.unk0 = g_menuName.windows[2].x - 0xE;
        h = &D_80077DE8;
        y = g_menuName.windows[2].y + g_menuName.menuCursor * 16;
        h->unk2 = g_menuName.menuCursor >= 2 ? y + 0x15 : y + 0xD;
    } else {
        r = &g_menuName.windows[4];
        if (g_menuName.charSet != 0) {
            row = g_menuName.gridCursor / NAME_GRID_COLS;
            idx = g_menuName.gridCursor % NAME_GRID_COLS;
            D_80077DE8.unk2 = r->y + row * 21 + 0x26;
            col = (u8)(idx / 5);
            sub = (u8)(idx % 5);
            D_80077DE8.unk0 = r->x + grp_x(col) + sub * 13 - 0x17;
        } else {
            row = g_menuName.gridCursor / NAME_GRID_COLS;
            col = g_menuName.gridCursor % NAME_GRID_COLS;
            D_80077DE8.unk0 = r->x + col * 14 + 0x27;
            D_80077DE8.unk2 = r->y + row * 21 + 0x26;
        }
    }
    D_80077DEC = 3;
    func_800355E4();
}

void menu_name_update(void) {
    menu_name_input();
    menu_name_draw();
}

void menu_name_exit(void) {
    func_8003556C(NULL);
    func_80035580(NULL);
    menu_name_restoreClut();
}

void menu_name_draw(void) {
    menu_name_drawTitle();
    menu_name_drawPortrait();
    menu_name_drawMenu();
    menu_name_drawHelp();
    menu_name_drawEntry();
    menu_name_drawCursor();
}

void menu_name_moveInMenu(void) {
    if (g_padRepeat & PAD_UP) {
        snd_playSe(SE_CURSOR);
        g_menuName.menuCursor = g_menuName.menuCursor == 0 ? NAME_MENU_DEFAULT : g_menuName.menuCursor - 1;
        return;
    }
    if (g_padRepeat & PAD_DOWN) {
        snd_playSe(SE_CURSOR);
        g_menuName.menuCursor = g_menuName.menuCursor >= NAME_MENU_DEFAULT ? 0 : g_menuName.menuCursor + 1;
        return;
    }
    if (g_padRepeat & PAD_RIGHT) {
        snd_playSe(SE_CURSOR);
        g_menuName.focus = NAME_FOCUS_GRID;
        func_80035580(&g_menuNameHelpRect);
    }
}

void menu_name_drawTitle(void) {
    func_8002DEE4(g_menuNameTitleWin, 6, 2);
    func_8002705C(g_menuNameTitleWin, 6, 2);
    text_drawBlock(g_menuNameTitleWin->x + 0xA, g_menuNameTitleWin->y + 6, 5, text_get(NAME_TEXT_TITLE), 0);
}

void menu_name_drawHelp(void) {
    func_8002DEE4(g_menuNameHelpWin, 7, 1);
    func_8002705C(g_menuNameHelpWin, 7, 1);
    text_drawBlock(g_menuNameHelpWin->x + 0x3E, g_menuNameHelpWin->y + 6, 6, text_get(NAME_TEXT_PROMPT), 0);
    ui_drawLabel(g_menuNameHelpWin->x + 0x23, g_menuNameHelpWin->y - 4, 6, D_80070450, 0);
}

void menu_name_loadPortrait(s32 chr) {
    u8 *img;

    img = g_menuNamePortraits[func_80028954(chr)];
    StoreImage(&g_menuNameClutRect, (u32 *)g_menuName.savedClut);
    DrawSync(0);
    LoadImage(&g_menuNameClutRect, (u32 *)img);
    DrawSync(0);
    LoadImage(&g_menuNamePortraitRect, (u32 *)(img + 0x100));
    DrawSync(0);
}

void menu_name_restoreClut(void) {
    LoadImage(&g_menuNameClutRestoreRect, (u32 *)g_menuName.savedClut);
    DrawSync(0);
}

s32 menu_name_getNameId(s32 i) {
    return g_menuNameIds[i];
}

s32 menu_name_getCharIdx(s32 i) {
    return g_menuNameCharIdx[i];
}

void menu_name_setHelpText(void) {
    D_80077DE8.unk8 = text_get(g_menuName.focus == NAME_FOCUS_MENU ? g_menuName.menuCursor + NAME_TEXT_MENU_HELP : NAME_TEXT_GRID_HELP);
}

void menu_name_clampCursor(void) {
    if (g_menuName.charSet == 1 && g_menuName.gridCursor >= g_menuName.gridSize[1]) {
        g_menuName.gridCursor = g_menuName.gridSize[1] + (u8)(g_menuName.gridCursor % NAME_GRID_COLS - NAME_GRID_COLS);
    }
}

INCLUDE_RODATA("asm/us/ovl_3a/nonmatchings/ovl_3a", g_nameMenuText);

INCLUDE_RODATA("asm/us/ovl_3a/nonmatchings/ovl_3a", g_menuNamePortraits);
