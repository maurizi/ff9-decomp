#include "ovl_39.h"

void menuSave_init(void) {
    s32 i;

    s_menuSave.mode = g_menu.arg;
    s_menuSave.savedFadeOtz = fade_setOtz(7);
    s_menuSave.state = SAVE_ST_SLOT;
    s_menuSave.phase = SAVE_PH_READY;
    s_menuSave.loaded = 0;
    s_menuSave.showCursor = 1;
    s_menuSave.newestCounter = 0;
    s_menuSave.newestFile = 0;
    g_menu.cardSlot &= 1;
    menuSave_initCheck();
    menuSave_initMessage();
    menuSave_initSavedMsg();
    menuSave_initConfirm();
    menuSave_initAccess();
    menuSave_loadImage();
    menuSave_saveVram();
    help_setCallback(menuSave_updateHelp);
    text_setBank(g_saveMenuText);
    s_menuSave.help = text_get(SAVE_TEXT_SELECT_SLOT);
    s_menuSave.cardStatus[0] = CARD_RES_ERROR;
    s_menuSave.cardStatus[1] = CARD_RES_ERROR;
    menuSave_cardInit();
    menuSave_openCardEvents();
    menuSave_onInit();
    for (i = 0; i < 2; i++) {
        s_menuSave.savedPortMask[i] = g_padPortMask[i];
        g_padPortMask[i] |= PAD_L2 | PAD_R2;
    }
}

void menuSave_drawHelp(void) {
    u8 *slotName = text_get(g_menu.cardSlot + SAVE_TEXT_SLOT1);
    s32 w = text_measureWidth((s32)slotName);

    text_drawBlock(s_menuSaveSlotNameRect.x + ((s_menuSaveSlotNameRect.w - w) >> 1), s_menuSaveSlotNameRect.y + 2, 5, slotName, (menuSave_isSlotReady(g_menu.cardSlot, 1) == 0) * TEXT_PAL_GREY);
    if (s_menuSave.help != NULL) {
        text_drawBlock(s_menuSaveHelpRect.x + 0xA, s_menuSaveHelpRect.y + 6, 5, s_menuSave.help, TEXT_PAL_WHITE);
    }
    ui_drawLabel(s_menuSaveHelpRect.x + 6, s_menuSaveHelpRect.y - 3, 5, g_labelHelp, UI_LABEL_NORMAL);
    win_drawBox(&s_menuSaveSlotNameRect, 5, WIN_STYLE_LINE);
    win_drawBox(&s_menuSaveHelpRect, 5, WIN_STYLE_LINE_RULE);
    win_drawShadow(&s_menuSaveHelpRect, 5, WIN_STYLE_LINE_RULE);
}

void menuSave_inputSlot(void) {
    if (g_padRepeat & BTN_CIRCLE) {
        snd_playSe(SE_CANCEL);
        if (g_menu.type != 0) {
            menuSave_close();
            return;
        }
        menuScreen_open(0);
        return;
    }
    if (g_padRepeat & PAD_UP) {
        snd_playSe(SE_CURSOR);
        g_menu.cardSlot = g_menu.cardSlot == 0 ? 1 : g_menu.cardSlot - 1;
    }
    if (g_padRepeat & PAD_DOWN) {
        snd_playSe(SE_CURSOR);
        g_menu.cardSlot = g_menu.cardSlot != 0 ? 0 : g_menu.cardSlot + 1;
    }
    if (g_padRepeat & BTN_CROSS) {
        menuSave_selectSlot();
    }
}

void menuSave_drawSlotWindow(void) {
    s32 i;
    s32 ok;
    u8 *str;
    u8 status;

    win_drawBox(&s_menuSaveSlotWindowRect, 0x34, WIN_STYLE_BEVEL);
    win_drawShadow(&s_menuSaveSlotWindowRect, 0x34, WIN_STYLE_BEVEL);
    for (i = 0; i < 2; i++) {
        str = text_get(i + SAVE_TEXT_SLOT1);
        ok = 0;
        status = s_menuSave.cardStatus[i];
        if (status == CARD_RES_DONE || status == CARD_RES_NEW) {
            ok = 1;
        }
        text_drawBlock(s_menuSaveSlotWindowRect.x + 10, s_menuSaveSlotWindowRect.y + 10 + i * 18, 0x33, str, (ok ^ 1) * TEXT_PAL_GREY);
    }
}

void menuSave_drawFileList(void) {
    MenuView view;
    RECT rowRect;
    RECT listRect, clipRect;
    s32 i;
    MenuList *list = &s_menuSave.list;

    menuSave_drawScrollArrows();
    ui_drawLabel(0xE, 0x23, 0x2C, g_labelFile, UI_LABEL_NORMAL);
    ui_drawRule(8, 0x26, 0x2C, 1, 0x12C, (g_gameState->config >> CFG_BIT_WINDOW_COLOR) & 1);
    rowRect.x = SAVE_LIST_X;
    rowRect.y = 0;
    rowRect.w = SAVE_LIST_W;
    rowRect.h = SAVE_ROW_H;
    menuSave_getListView(list, &view);
    win_setClip(NULL, 0x2D, WIN_STYLE_FRAMELESS);
    for (i = 0; i < view.rows; i++) {
        rowRect.y = i * SAVE_ROW_H + (s16)(view.y + SAVE_LIST_Y);
        menuSave_drawFile(&rowRect, 0x2D, view.first + i);
    }
    if (list->scroll & MENULIST_SCROLL_PAGE) {
        listRect.x = SAVE_LIST_X;
        listRect.y = SAVE_LIST_Y;
        listRect.w = SAVE_LIST_W;
        listRect.h = SAVE_LIST_H;
        menuSave_getScrollClip(list, &listRect, &clipRect);
        win_setClip(&clipRect, 0x2D, WIN_STYLE_FRAMELESS);
        menuSave_drawScrollRows();
    }
    rowRect.y = SAVE_LIST_Y;
    rowRect.h = SAVE_LIST_H;
    win_setClip(&rowRect, 0x2D, WIN_STYLE_FRAMELESS);
}

void menuSave_drawFile(RECT *rect, s32 otz, s32 file) {
    char name[8];
    void (*fn)(RECT *, s32, s32);
    s32 winColor;

    strcpy(name, "File00");
    name[4] = (file + 1) / 10 + '0';
    name[5] = (file + 1) % 10 + '0';
    ui_drawSmallDigits(rect->x + 6, rect->y + 3, otz, (u8 *)name, TEXT_PAL_WHITE);
    fn = s_menuSaveFileDrawers[s_menuSave.files[file].state];
    if (fn != NULL) {
        fn(rect, otz, file);
    }
    if (s_menuSave.files[file].state == SAVE_FILE_USED) {
        winColor = s_menuSave.files[file].windowColor;
    } else {
        winColor = g_gameState->config >> CFG_BIT_WINDOW_COLOR;
        winColor &= 1;
    }
    win_drawBoxEx(rect, 0x2D, WIN_STYLE_LINE, winColor);
}

void menuSave_drawFileInfo(RECT *rect, s32 z, s32 i) {
    u8 numText[16];
    RECT rr;
    SaveFile *entry;
    s32 j;
    s32 len;

    entry = &s_menuSave.files[i];
    len = 0;
    for (j = len; j < 4; j++) {
        if ((entry->discs >> j) & 1) {
            numText[len++] = j + '1';
        }
    }
    numText[len] = 0;
    ui_drawSmallDigits(rect->x + 0x30, rect->y + 3, z, "/DISC", TEXT_PAL_WHITE);
    ui_drawSmallDigits(rect->x + 0x56, rect->y + 3, z, numText, TEXT_PAL_WHITE);
    for (j = 0; j < PARTY_SIZE; j++) {
        if (entry->party[j].portrait != PORTRAIT_NONE) {
            ui_drawPortraitId(rect->x + 6 + j * 0x21, rect->y + 0xD, z, entry->party[j].portrait, 0x2C, 0);
        }
    }
    text_formatNumber(entry->level, numText, 2, TEXT_NUM_ASCII);
    text_drawBlock(rect->x + 0x90, rect->y + 6, z, entry->name.v, TEXT_PAL_WHITE);
    ui_drawSprite(rect->x + 0x90, rect->y + 0x17, z, 0xB4, 0);
    ui_drawDigits(rect->x + 0xA2, rect->y + 0x15, z, numText, DIGITS_WHITE);
    ui_drawSprite(rect->x + 0xD8, rect->y + 9, z, 0xB1, 0);
    ui_drawSprite(rect->x + 0xD8, rect->y + 0x16, z, 0xB2, 0);
    ui_drawSmallDigits(rect->x + 0xE4, rect->y + 0xA, z, numText, text_formatTime(entry->playTime, numText, 0));
    text_formatNumber(entry->gil, numText, 7, TEXT_NUM_ASCII);
    numText[7] = 0xA4;
    numText[8] = 0;
    ui_drawSmallDigits(rect->x + 0xE4, rect->y + 0x17, z, numText, TEXT_PAL_WHITE);
    rr.x = rect->x + 0x8E;
    rr.y = rect->y + 0x25;
    rr.w = 0x9C;
    rr.h = 0x15;
    text_drawBlock(rr.x + 8, rr.y + 3, z, (u8 *)&entry->location, TEXT_PAL_WHITE);
    win_drawBoxEx(&rr, z, WIN_STYLE_LINE, entry->windowColor);
}

void menuSave_drawCursor(void) {
    u8 blink = (g_menu.frame & 3) != 0;

    if (!fade_isBusy() && s_menuSave.phase == SAVE_PH_READY) {
        if (s_menuSave.state == SAVE_ST_SLOT) {
            g_menuHelp.x = 0x74;
            g_menuHelp.otz = 0x32;
            g_menuHelp.y = g_menu.cardSlot * 0x12 + 0x6D;
            help_drawCursor();
        } else if (s_menuSave.state == SAVE_ST_LIST || (s_menuSave.state == SAVE_ST_ACCESS && blink)) {
            g_menuHelp.x = 0;
            g_menuHelp.otz = 0x2A;
            g_menuHelp.y = s_menuSave.list.cursor * SAVE_ROW_H + 0x42;
            help_drawCursor();
        } else if (s_menuSave.state == SAVE_ST_CONFIRM && blink) {
            ui_drawHand(0, s_menuSave.list.cursor * SAVE_ROW_H + 0x42, 0x2A);
        }
    }
}

void menuSave_updateFileList(void) {
    MenuList *list = &s_menuSave.list;
    s32 newTop;
    s32 dec;

    if (s_menuSave.cardStatus[g_menu.cardSlot] != CARD_RES_DONE) {
        menuSave_showNotice(SAVE_TEXT_NO_CARD);
        return;
    }
    if (list->scroll != 0) {
        if (!(list->scroll & MENULIST_SCROLL_PAGE)) {
            dec = list->scroll & MENULIST_SCROLL_UP;
            list->scrollPos += 6;
            if (list->scrollPos >= SAVE_ROW_H) {
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
            if (list->scrollPos >= SAVE_LIST_H) {

                list->scroll = 0;
                list->scrollPos = 0;
                list->top = list->pageTop;
            }
        }
    }
}

void menuSave_updateHelp(void) {
    RECT helpRect = s_menuSaveSlotHelpRect;
    u8 st;
    s32 textId;

    g_menuHelp.text = NULL;
    if (s_menuSave.phase == SAVE_PH_READY && s_menuSave.showCursor != 0) {
        help_setBounds(NULL);
        st = s_menuSave.state;
        if (st == SAVE_ST_SLOT) {
            help_setBounds(&helpRect);
            textId = SAVE_TEXT_SAVES_TO;
            /* st is SAVE_ST_SLOT == SAVE_MODE_LOAD here */
            if (s_menuSave.mode == st) {
                textId = SAVE_TEXT_LOADS_FROM;
            }
            g_menuHelp.text = text_get(textId + g_menu.cardSlot);
        } else if (st == SAVE_ST_LIST) {
            menuSave_setFileHelp();
        } else if (st == SAVE_ST_CONFIRM) {
            g_menuHelp.text = menuSave_getConfirmHelp();
        }
    }
}

void menuSave_setFileHelp(void) {
    MenuList *list;
    SaveMember *members;
    s32 idx;
    s32 memberCount;
    s32 i;
    s32 st;

    list = &s_menuSave.list;
    idx = list->top + list->cursor;
    st = s_menuSave.files[idx].state;
    switch (st) {
    case SAVE_FILE_EMPTY:
    case SAVE_FILE_OTHER:
    case SAVE_FILE_CORRUPT:
        g_menuHelpText = text_get(st + SAVE_TEXT_FILE_HELP);
        break;
    case SAVE_FILE_USED:
        s_menuSaveHelpBuf[0] = 0xFF;
        memberCount = 0;
        members = s_menuSave.files[idx].party;
        for (i = 0; i < 4; i++) {
            if (members[i].present != 0) {
                memberCount = i + 1;
            }
        }
        for (i = 0; i < memberCount; i++) {
            if (members[i].present != 0) {
                text_setStringArg(i, members[i].name);
                text_setNumberArg(i, members[i].level);
                text_append(s_menuSaveHelpBuf, text_get(i + SAVE_TEXT_MEMBER1));
                if (i < memberCount - 1) {
                    text_append(s_menuSaveHelpBuf, text_get(SAVE_TEXT_NEWLINE));
                }
            }
        }
        g_menuHelpText = s_menuSaveHelpBuf;
        break;
    }
}

void menuSave_update(void) {
    menuSave_drawTitle();
    menuSave_drawHelp();
    menuSave_input();
    menuSave_drawState();
    menuSave_updateState();
}

void menuSave_exit(void) {
    s32 i;

    help_setLocked(0);
    help_setCallback(NULL);
    menuSave_closeCardEvents();
    menuSave_restoreVram();
    menuSave_freeImage();
    for (i = 0; i < 2; i++) {
        g_padPortMask[i] = s_menuSave.savedPortMask[i];
    }
}

u8 menuSave_isLoaded(void) {
    return s_menuSave.loaded;
}

u8 menuSave_isSlotReady(s32 port, u8 allowNew) {
    return menuSave_isSlotReady_inline(port, allowNew);
}

void menuSave_backToSlots(void) {
    fade_outDefault();
    fade_setOnDone(menuSave_returnToSlots);
}

void menuSave_close(void) {
    fade_setOtz(s_menuSave.savedFadeOtz);
    menu_close();
}

void menuSave_drawTitle(void) {
    win_drawBox(&s_menuSaveTitleRect, 5, WIN_STYLE_BEVEL);
    win_drawShadow(&s_menuSaveTitleRect, 5, WIN_STYLE_BEVEL);
    text_drawBlock(s_menuSaveTitleRect.x + 0xA, s_menuSaveTitleRect.y + 6, 4, (u8 *)text_getFrom(g_saveMenuText, s_menuSave.mode), TEXT_PAL_WHITE);
}

void menuSave_drawState(void) {
    void (*fn)(void) = s_menuSaveDrawFns[s_menuSave.state];

    if (fn != NULL) {
        fn();
    }
    if (s_menuSave.showCursor != 0) {
        menuSave_drawCursor();
    }
}

void menuSave_input(void) {
    if (!fade_isBusy()) {
        switch (s_menuSave.phase) {
        case SAVE_PH_MESSAGE:
            menuSave_inputMessage();
            break;
        case SAVE_PH_READY:
            if (s_menuSaveInputFns[s_menuSave.state] != NULL) {
                s_menuSaveInputFns[s_menuSave.state]();
            }
            break;
        }
    }
}

void menuSave_updateState(void) {
    menuSave_cardPoll();
    s_menuSave.cardStatus[0] = menuSave_getCardStatus(CARD_CHAN(0));
    s_menuSave.cardStatus[1] = menuSave_getCardStatus(CARD_CHAN(1));
    if (s_menuSave.phase == SAVE_PH_READY) {
        if (s_menuSaveUpdateFns[s_menuSave.state] != NULL) {
            s_menuSaveUpdateFns[s_menuSave.state]();
        }
    }
    menuSave_updateBookAnim();
    menuSave_updateHelpVisibility();
}

void menuSave_inputFileList(void) {
    MenuList *list = &s_menuSave.list;

    if (list->scroll == 0) {
        if (g_padRepeat & BTN_CIRCLE) {
            snd_playSe(SE_CANCEL);
            menuSave_backToSlots();
            return;
        }
        if (g_padRepeat & BTN_CROSS) {
            menuSave_selectFile();
            return;
        }
        menuList_up(list, g_padRepeat);
        menuList_down(list, g_padRepeat);
        menuSave_pageUp(list, g_padRepeat);
        menuSave_pageDown(list, g_padRepeat);
    }
}

void menuSave_drawScrollRows(void) {
    MenuView view;
    RECT rowRect;
    s32 i;

    menuSave_getScrollView(&s_menuSave.list, &view);
    rowRect.x = SAVE_LIST_X;
    rowRect.w = SAVE_LIST_W;
    rowRect.y = 0;
    rowRect.h = SAVE_ROW_H;
    for (i = 0; i < view.rows; i++) {
        rowRect.y = i * SAVE_ROW_H + (s16)(view.y + SAVE_LIST_Y);
        menuSave_drawFile(&rowRect, 0x2D, view.first + i);
        win_drawBox(&rowRect, 0x2D, WIN_STYLE_LINE);
    }
}

void menuSave_drawScrollArrows(void) {
    s32 blink = (((u32)g_menu.frame >> 4) ^ 1) & 1;
    MenuList *list = &s_menuSave.list;

    ui_drawSprite(0x12E, 0x2C, 0x2A, UI_SPRITE_SCROLL_UP, list->top != 0 ? blink : 1);
    ui_drawSprite(0x12E, 0xCF, 0x2A, UI_SPRITE_SCROLL_DOWN, list->top < list->rows - list->visibleRows ? blink : 1);
}

void menuSave_drawEmptyFile(RECT *rect, s32 otz) {
    text_drawBlock(rect->x + 0x10, rect->y + 0x16, otz, text_get(SAVE_TEXT_EMPTY_BLOCK), TEXT_PAL_WHITE);
}

void menuSave_drawOtherFile(RECT *rect, s32 otz) {
    text_drawBlock(rect->x + 0x10, rect->y + 0x16, otz, text_get(SAVE_TEXT_OTHER_GAME), TEXT_PAL_WHITE);
}

void menuSave_drawCorruptFile(RECT *rect, s32 otz) {
    text_drawBlock(rect->x + 0x10, rect->y + 0x16, otz, text_get(SAVE_TEXT_DATA_CORRUPTED), TEXT_PAL_WHITE);
}

void menuSave_selectSlot(void) {
    if (!menuSave_isSlotReady_inline(g_menu.cardSlot, 1)) {
        snd_playSe(SE_BUZZER);
        menuSave_showNotice(SAVE_TEXT_NO_CARD);
        return;
    }
    snd_playSe(SE_CURSOR);
    menuSave_openCheck();
}

void menuSave_selectFile(void) {
    s32 i = s_menuSave.list.top + s_menuSave.list.cursor;

    if (s_menuSave.mode == SAVE_MODE_LOAD) {
        menuSave_loadFile(i, i);
        return;
    }
    menuSave_saveFile(i, i);
}

void menuSave_updateHelpVisibility(void) {
    u8 hide;

    hide = s_menuSave.loaded != 0;
    if (s_menuSave.phase != SAVE_PH_READY) {
        hide = 1;
    }
    if (s_menuSave.state != SAVE_ST_SLOT && s_menuSave.state != SAVE_ST_LIST && s_menuSave.state != SAVE_ST_CONFIRM) {
        hide = 1;
    }
    help_setLocked(hide);
}

void menuSave_returnToSlots(void) {
    s_menuSave.state = SAVE_ST_SLOT;
    s_menuSave.help = text_get(SAVE_TEXT_SELECT_SLOT);
    fade_inDefault();
}

void menuSave_onInit(void) {
}

void menuSave_drawBookIcon(s32 x, s32 y, s32 z, s32 idx) {
    u32 clut;
    Ovl39Sprite *frame;

    clut = GetClut(0, BOOK_CLUT_Y);

    setlen(g_primPtr, PRIM_LEN(SPRT));
    frame = &s_menuSaveBookFrames[idx];
    ((SprtWords *)g_primPtr)->rgbc = GPU_RGBC(GPU_SPRT, GPU_RGB_NEUTRAL);
    ((SprtWords *)g_primPtr)->xy = x | (y << 16);
    ((SprtWords *)g_primPtr)->uvclut = frame->u | (frame->v << 8) | (clut << 16);
    ((SprtWords *)g_primPtr)->wh = frame->w | (frame->h << 16);
    addPrim(&g_ot[z], g_primPtr);
    g_primPtr += sizeof(SprtWords);
    setlen(g_primPtr, 2);
    SetDrawTPage(g_primPtr, 0, 1, getTPage(1, 0, 0x40, 0x100));
    addPrim(&g_ot[z], g_primPtr);
    g_primPtr += sizeof(DR_TPAGE);
}

void menuSave_saveVram(void) {
    RECT rect;
    u8 *img = s_menuSaveBookImage;
    u8 *backup = s_menuSaveVramBackup;
    u8 *src = img;
    s32 i;

    rect.y = BOOK_CLUT_Y;
    rect.x = 0;
    rect.w = 0x100;
    rect.h = 1;
    StoreImage(&rect, (u32 *)backup);
    DrawSync(0);
    LoadImage(&rect, (u32 *)img);
    DrawSync(0);
    backup += BOOK_CLUT_SIZE;
    img += BOOK_CLUT_SIZE;
    rect.x = BOOK_X;
    rect.w = BOOK_W;
    rect.y = BOOK_Y;
    rect.h = BOOK_STRIP_H;
    /* swaps VRAM and image strip by strip: each strip's old VRAM goes over the strip
       uploaded before it, the first one into s_menuSaveVramBackup */
    for (i = 0; i < BOOK_STRIPS; i++) {
        if (i == 0) {
            StoreImage(&rect, (u32 *)backup);
        } else {
            StoreImage(&rect, (u32 *)src);
            src += BOOK_STRIP_SIZE;
        }
        DrawSync(0);
        LoadImage(&rect, (u32 *)img);
        DrawSync(0);
        rect.y += BOOK_STRIP_H;
        img += BOOK_STRIP_SIZE;
    }
}

void menuSave_restoreVram(void) {
    RECT rect;
    u8 *img;
    u8 *backup;
    s32 i;

    img = s_menuSaveBookImage;
    backup = s_menuSaveVramBackup;
    rect.x = 0;
    rect.y = BOOK_CLUT_Y;
    rect.w = 0x100;
    rect.h = 1;
    LoadImage(&rect, (u32 *)backup);
    DrawSync(0);
    backup += BOOK_CLUT_SIZE;
    rect.x = BOOK_X;
    rect.w = BOOK_W;
    rect.y = BOOK_Y;
    rect.h = BOOK_STRIP_H;
    for (i = 0; i < BOOK_STRIPS; i++) {
        if (i == 0) {
            LoadImage(&rect, (u32 *)backup);
        } else {
            LoadImage(&rect, (u32 *)img);
            img += BOOK_STRIP_SIZE;
        }
        DrawSync(0);
        rect.y += BOOK_STRIP_H;
    }
}

void menuSave_updateBookAnim(void) {
    if (s_menuSaveBookAnim.enabled != 0) {
        if (++s_menuSaveBookAnim.tick >= BOOK_ANIM_TICKS) {
            s_menuSaveBookAnim.tick = 0;
            if (++s_menuSaveBookAnim.frame >= BOOK_ANIM_FRAMES) {
                s_menuSaveBookAnim.frame = 0;
            }
        }
    }
}

void menuSave_resetBookAnim(void) {
    s_menuSaveBookAnim.frame = 0;
    s_menuSaveBookAnim.tick = 0;
}

u8 menuSave_enableBookAnim(u8 enable) {
    u8 old = s_menuSaveBookAnim.enabled;

    s_menuSaveBookAnim.enabled = enable;
    return old;
}

void menuSave_drawBook(s32 x, s32 y, s32 otz) {
    menuSave_drawBookIcon(x, y, otz, s_menuSaveBookAnim.frame);
}

void menuSave_openConfirm(s32 id, s32 yes, s32 no) {
    RECT rect;
    u16 sz;
    s32 w, h;

    s_menuSave.state = SAVE_ST_CONFIRM;
    s_menuSave.phase = SAVE_PH_WAIT;
    s_menuSaveConfirm.text = (s32)text_get(id);
    sz = text_getInfo(id);
    w = (sz & 0xFFF) + 0x10;
    s_menuSaveConfirm.textSize = sz;
    s_menuSaveConfirm.active = 1;
    s_menuSaveConfirm.choice = CONFIRM_NO;
    h = ((sz >> 12) + 2) * 15 + 0x18;
    s_menuSaveConfirm.yesHelp = yes;
    s_menuSaveConfirm.noHelp = no;
    rect.w = w;
    rect.h = h;
    rect.x = (g_gameState->dispEnv->disp.w - w) >> 1;
    rect.y = (g_gameState->dispEnv->disp.h - h) >> 1;
    s_menuSaveConfirm.win = win_openFree(&rect, 0x19, WIN_STYLE_BEVEL);
    win_setOnOpened(s_menuSaveConfirm.win, menuSave_onConfirmOpened);
    win_setOnClosed(s_menuSaveConfirm.win, menuSave_onConfirmClosed);
    win_setDrawContent(s_menuSaveConfirm.win, menuSave_drawConfirm);
    win_setDrawOverlay(s_menuSaveConfirm.win, menuSave_drawConfirmCursor);
    win_setShadow(s_menuSaveConfirm.win, 1);
    win_setTitle(s_menuSaveConfirm.win, g_labelNotice);
}

void menuSave_drawConfirm(Window *w) {
    u32 yesInfo = text_getInfo(SAVE_TEXT_YES);
    u32 maxW = text_getInfo(SAVE_TEXT_NO) & 0xFFF;
    s32 d1 = 0x26;
    s32 d2 = 0x17;
    s16 textW;

    if (maxW < (yesInfo & 0xFFF)) {
        maxW = yesInfo & 0xFFF;
    }
    text_drawBlock(w->x + 8, w->y + 8, w->otz, (u8 *)s_menuSaveConfirm.text, TEXT_PAL_WHITE);
    textW = maxW;
    text_drawBlock(w->x + ((s32)(w->w - textW) >> 1), (w->y - d1) + w->h, w->otz, text_get(SAVE_TEXT_YES), TEXT_PAL_WHITE);
    text_drawBlock(w->x + ((s32)(w->w - textW) >> 1), (w->y - d2) + w->h, w->otz, text_get(SAVE_TEXT_NO), TEXT_PAL_WHITE);
}

void menuSave_drawConfirmCursor(Window *w) {
    u32 yesInfo = text_getInfo(SAVE_TEXT_YES);
    u32 maxW = text_getInfo(SAVE_TEXT_NO) & 0xFFF;
    s32 z;

    if (maxW < (yesInfo & 0xFFF)) {
        maxW = yesInfo & 0xFFF;
    }
    if (w->state == WIN_STATE_OPEN) {
        g_menuHelp.x = w->x + ((s32)(w->w - maxW) >> 1) - 0x1A;
        g_menuHelp.y = w->y + (s16)(w->h + (s16)(s_menuSaveConfirm.choice * 15 - 0x26)) + 3;
        z = w->otz;
        g_menuHelp.otz = z;
        ui_drawHand(g_menuHelp.x, g_menuHelp.y, z - 1);
    }
}

void menuSave_initConfirm(void) {
    s_menuSaveConfirm.active = 0;
}

void menuSave_inputConfirm(void) {
    if (s_menuSaveConfirm.win->state == WIN_STATE_OPEN) {
        if (g_padRepeat & BTN_CROSS) {
            snd_playSe(SE_CURSOR);
            win_close(s_menuSaveConfirm.win);
            s_menuSave.phase = SAVE_PH_WAIT;
            return;
        }
        if (g_padRepeat & BTN_CIRCLE) {
            snd_playSe(SE_CANCEL);
            s_menuSaveConfirm.choice = CONFIRM_NO;
            win_close(s_menuSaveConfirm.win);
            s_menuSave.phase = SAVE_PH_WAIT;
            return;
        }
        if (g_padRepeat & (PAD_UP | PAD_DOWN)) {
            snd_playSe(SE_CURSOR);
            s_menuSaveConfirm.choice ^= 1;
        }
    }
}

void menuSave_updateConfirm(void) {
    u8 st = s_menuSave.cardStatus[g_menu.cardSlot];

    if (st != CARD_RES_DONE && st != CARD_RES_NEW) {
        win_setOnClosed(s_menuSaveConfirm.win, NULL);
        win_close(s_menuSaveConfirm.win);
        s_menuSaveConfirm.active = 0;
        s_menuSaveConfirm.win = NULL;
        menuSave_showNotice(SAVE_TEXT_NO_CARD);
    }
}

void menuSave_setConfirmCallback(void (*callback)(SaveConfirm *)) {
    s_menuSaveConfirm.onYes = callback;
}

u8 menuSave_isConfirmActive(void) {
    return s_menuSaveConfirm.active;
}

u8 menuSave_setConfirmActive(u8 enable) {
    u8 old = s_menuSaveConfirm.active;

    s_menuSaveConfirm.active = enable;
    return old;
}

u8 *menuSave_getConfirmHelp(void) {
    return text_get(s_menuSaveConfirm.choice == CONFIRM_YES ? s_menuSaveConfirm.yesHelp : s_menuSaveConfirm.noHelp);
}

void menuSave_onConfirmOpened(Window *w) {
    s_menuSave.phase = SAVE_PH_READY;
}

void menuSave_onConfirmClosed(Window *w) {
    s_menuSave.state = SAVE_ST_LIST;
    if (s_menuSaveConfirm.choice == CONFIRM_YES) {
        s_menuSaveConfirm.onYes(&s_menuSaveConfirm);
    }
    s_menuSaveConfirm.active = 0;
    s_menuSave.phase = SAVE_PH_READY;
}

void menuSave_openCheck(void) {
    s32 sl;
    s32 i;

    s_menuSave.state = SAVE_ST_CHECK;
    s_menuSave.formatted = 0;
    s_menuSave.newestFile = 0;
    s_menuSave.newestCounter = 0;
    s_menuSaveCheck.step = CHECK_STEP_NONE;
    s_menuSaveCheck.active = 1;
    s_menuSaveCheck.win = win_openFree(&s_menuSaveCheckRect, 0x1E, WIN_STYLE_BEVEL);
    s_menuSaveCheck.fd = 0xFF;
    s_menuSaveCheck.tries = 0;
    s_menuSaveCheck.ff9Files = 0;
    s_menuSaveCheck.headersRead = 0;
    s_menuSaveCheck.otherBlocks = 0;
    s_menuSaveCheck.dirCount = 0;
    s_menuSaveCheck.file = 0;
    for (i = 0; i < 4; i++) {
        s_menuSaveCheck.triesPerResult[i] = 0;
    }
    for (i = 0; i < SAVE_FILES; i++) {
        s_menuSave.files[i].state = SAVE_FILE_EMPTY;
        s_menuSave.files[i].index = i;
        s_menuSaveCheck.unk2B4[i] = 0;
    }
    menuSave_resetBookAnim();
    menuSave_enableBookAnim(1);
    win_setDrawContent(s_menuSaveCheck.win, menuSave_drawCheck);
    win_setOnOpened(s_menuSaveCheck.win, menuSave_onCheckOpened);
    win_setOnClosed(s_menuSaveCheck.win, menuSave_onCheckClosed);
    win_setTitle(s_menuSaveCheck.win, g_labelCheck);
    win_setShadow(s_menuSaveCheck.win, 1);
    sl = g_menu.cardSlot;
    switch (s_menuSave.cardStatus[sl]) {
    case CARD_RES_NEW:
        s_menuSaveCheck.step = CHECK_STEP_CLEAR;
        if (menuSave_cardClearSync(CARD_CHAN(g_menu.cardSlot)) != CARD_RES_DONE) {
            menuSave_checkError(SAVE_TEXT_CHECK_FAILED);
        }
        break;
    case CARD_RES_DONE:
        s_menuSaveCheck.step = CHECK_STEP_LOAD;
        if (menuSave_cardLoad(CARD_CHAN(g_menu.cardSlot), menuSave_onCardLoaded) == 0) {
            menuSave_checkError(SAVE_TEXT_CHECK_FAILED);
        }
        break;
    case CARD_RES_TIMEOUT:
        menuSave_checkError(SAVE_TEXT_NO_CARD);
        break;
    case CARD_RES_ERROR:
        menuSave_checkError(SAVE_TEXT_CHECK_FAILED);
        break;
    }
}

void menuSave_onCardLoaded(void) {
    s32 result;

    result = menuSave_getCardResult(CARD_CHAN(g_menu.cardSlot));

    s_menuSaveCheck.tries++;
    s_menuSaveCheck.triesPerResult[result]++;
    if (s_menuSaveCheck.tries == s_menuSaveCheck.triesPerResult[result]) {
        if (s_menuSaveCheck.tries < s_menuSaveCheckRetries[result]) {
            menuSave_retryCheck(result);
            return;
        }
        switch (result) {
        case CARD_RES_DONE:
            menuSave_onCardFormatted();
            return;
        case CARD_RES_NEW:
            menuSave_onCardUnformatted();
            return;
        }
    }
    menuSave_checkError(SAVE_TEXT_CHECK_FAILED);
}

u8 menuSave_readDir(void) {
    DirEnt *ent;
    SaveName20 *dstName;
    s32 i;
    s32 j;
    s32 k;
    s32 blockCount;

    s_menuSaveCheck.dirCount = menuSave_countFiles();
    if (s_menuSaveCheck.dirCount < 0) {
        menuSave_checkError(SAVE_TEXT_CHECK_FAILED);
        return 0;
    }
    for (i = 0; i < s_menuSaveCheck.dirCount; i++) {
        ent = &s_menuSaveCheck.dir[i];
        if (strncmp((char *)ent, (char *)menuSave_getFilePrefix(), SAVE_PREFIX_LEN) == 0) {
            k = menuSave_parseFileNumber(ent->name.v);
            dstName = &s_menuSave.files[k].fileName.name;
            *dstName = ent->name;
            s_menuSave.files[k].fileName.term[0] = 0;
            s_menuSave.files[k].state = SAVE_FILE_USED;
            s_menuSaveCheck.ff9Files++;
        }
    }
    for (i = 0; i < s_menuSaveCheck.dirCount; i++) {
        ent = &s_menuSaveCheck.dir[i];
        if (strncmp((char *)ent, (char *)menuSave_getFilePrefix(), SAVE_PREFIX_LEN) != 0) {
            blockCount = (s_menuSaveCheck.dir[i].size + CARD_BLOCK - 1) / CARD_BLOCK;
            for (j = 0; j < blockCount; j++) {
                k = menuSave_findLastEmpty();
                dstName = &s_menuSave.files[k].fileName.name;
                *dstName = ent->name;
                s_menuSave.files[k].fileName.term[0] = 0;
                s_menuSave.files[k].state = SAVE_FILE_OTHER;
                s_menuSaveCheck.otherBlocks++;
            }
        }
    }
    return menuSave_checkSpace();
}

/* Inlined copy of menuSave_readHeader */
static inline void menuSave_readHeader_inline(s32 file) {
    s32 fd;

    s_menuSaveCheck.file = file;
    fd = menuSave_openFile(CARD_CHAN(g_menu.cardSlot), (char *)s_menuSave.files[file].fileName.name.v);
    s_menuSaveCheck.fd = fd;
    if (fd < 0) {
        menuSave_checkError(SAVE_TEXT_CHECK_FAILED);
        return;
    }
    menuSave_readAsync(fd, (s32)s_menuSaveCheck.buf, CHECK_HEADER_SIZE, menuSave_onHeaderRead);
}

void menuSave_onHeaderRead(void) {
    if (menuSave_closeFile(s_menuSaveCheck.fd) < 0) {
        menuSave_checkError(SAVE_TEXT_CHECK_FAILED);
        return;
    }
    switch (menuSave_getCardResult(CARD_CHAN(g_menu.cardSlot))) {
    case CARD_RES_TIMEOUT:
        menuSave_checkError(SAVE_TEXT_NO_CARD);
        return;
    case CARD_RES_ERROR:
    case CARD_RES_NEW:
        menuSave_checkError(SAVE_TEXT_CHECK_FAILED);
        return;
    }
    menuSave_copyHeader(&s_menuSave.files[s_menuSaveCheck.file], (SaveHeader *)&s_menuSaveCheck.buf[sizeof(CardFileHeader)]);
    if (++s_menuSaveCheck.headersRead < s_menuSaveCheck.ff9Files) {
        menuSave_readHeader_inline(menuSave_findUsed(s_menuSaveCheck.headersRead));
        return;
    }
    menuSave_finishCheck();
}

void menuSave_enterFileList(void) {
    s_menuSave.phase = SAVE_PH_READY;
    s_menuSave.state = SAVE_ST_LIST;
    s_menuSave.help = text_get(SAVE_TEXT_SELECT_BLOCK);
    win_closeNow(s_menuSaveCheck.win);
    menuList_init(&s_menuSave.list, SAVE_FILES, SAVE_LIST_ROWS, 1, SAVE_LIST_H);
    if (s_menuSave.files[s_menuSave.newestFile].state == SAVE_FILE_USED) {
        s_menuSave.list.cursor = s_menuSave.files[s_menuSave.newestFile].cursor;
        s_menuSave.list.top = s_menuSave.newestFile - s_menuSave.list.cursor;
    }
    fade_inDefault();
    fade_setOnDone(menuSave_onFileListFadedIn);
}

void menuSave_findNewest(void) {
    s32 i;

    s_menuSave.newestCounter = 0;
    s_menuSave.newestFile = 0;
    for (i = 0; i < SAVE_FILES; i++) {
        if (s_menuSave.files[i].state == SAVE_FILE_USED) {
            if (s_menuSave.files[i].version != SAVE_VERSION) {
                s_menuSave.files[i].state = SAVE_FILE_CORRUPT;
            } else if (s_menuSave.newestCounter < s_menuSave.files[i].counter) {
                s_menuSave.newestCounter = s_menuSave.files[i].counter;
                s_menuSave.newestFile = i;
            }
        }
    }
}

void menuSave_initCheck(void) {
    s_menuSaveCheck.active = 0;
}

void menuSave_updateCheck(void) {
    void (*fn)(void);

    if (!menuSave_isSlotReady(g_menu.cardSlot, 1)) {
        menuSave_checkError(SAVE_TEXT_NO_CARD);
        return;
    }
    fn = s_menuSaveCheckSteps[s_menuSaveCheck.step];
    if (fn != NULL) {
        fn();
    }
}

u8 menuSave_isCheckActive(void) {
    return s_menuSaveCheck.active;
}

u8 menuSave_setCheckActive(u8 enable) {
    u8 old = s_menuSaveCheck.active;

    s_menuSaveCheck.active = enable;
    return old;
}

s32 menuSave_findLastEmpty(void) {
    s32 i;

    for (i = 0; i < SAVE_FILES; i++) {
        if (s_menuSave.files[SAVE_FILES - 1 - i].state == SAVE_FILE_EMPTY) {
            return SAVE_FILES - 1 - i;
        }
    }
    return -1;
}

s32 menuSave_findUsed(s32 nth) {
    s32 found = 0;
    s32 i;

    for (i = 0; i < SAVE_FILES; i++) {
        if (s_menuSave.files[i].state == SAVE_FILE_USED) {
            found++;
            if (nth < found) {
                return i;
            }
        }
    }
    return -1;
}

s32 menuSave_parseFileNumber(u8 *name) {
    return (name[SAVE_NAME_NUMBER] - '0') * 10 + (name[SAVE_NAME_NUMBER + 1] - '0');
}

void menuSave_checkWaitClear(void) {
    if (menuSave_isCardBusy() == 0) {
        s_menuSaveCheck.step++;
        if (menuSave_cardLoad(CARD_CHAN(g_menu.cardSlot), menuSave_onCardLoaded) == 0) {
            menuSave_checkError(SAVE_TEXT_CHECK_FAILED);
        }
    }
}

void menuSave_onCardFormatted(void) {
    s_menuSave.formatted = 1;
    if (menuSave_readDir()) {
        if (s_menuSaveCheck.headersRead < s_menuSaveCheck.ff9Files) {
            s_menuSaveCheck.step = CHECK_STEP_HEADERS;
            menuSave_readHeader(menuSave_findUsed(s_menuSaveCheck.headersRead));
            return;
        }
        menuSave_finishCheck();
    }
}

void menuSave_onCardUnformatted(void) {
    s_menuSave.formatted = 0;
    if (s_menuSave.mode == SAVE_MODE_LOAD) {
        menuSave_checkNotice(SAVE_TEXT_NO_FF9_DATA);
        return;
    }
    menuSave_finishCheck();
}

void menuSave_retryCheck(s32 result) {
    if (result == CARD_RES_NEW) {
        s_menuSaveCheck.step = CHECK_STEP_CLEAR;
        if (menuSave_cardClearSync(CARD_CHAN(g_menu.cardSlot)) == CARD_RES_DONE) {
            return;
        }
    } else if (menuSave_cardLoad(CARD_CHAN(g_menu.cardSlot), menuSave_onCardLoaded) != 0) {
        return;
    }
    menuSave_checkError(SAVE_TEXT_CHECK_FAILED);
}

void menuSave_drawCheck(Window *w) {
    menuSave_drawBook(w->x + 0xE, w->y + 0x12, w->otz);
    text_drawBlock(w->x + 0x4A, w->y + 0xE, w->otz, text_get(SAVE_TEXT_DO_NOT_REMOVE), TEXT_PAL_WHITE);
}

void menuSave_onCheckOpened(Window *w) {
    s_menuSave.phase = SAVE_PH_READY;
    s_menuSave.help = text_get(SAVE_TEXT_CHECKING);
}

void menuSave_onCheckClosed(Window *w) {
}

s32 menuSave_checkSpace(void) {
    if (s_menuSave.mode != SAVE_MODE_LOAD) {
        if (s_menuSaveCheck.otherBlocks >= SAVE_FILES) {
            menuSave_checkNotice(SAVE_TEXT_CARD_FULL);
            return 0;
        }
    } else if (s_menuSaveCheck.ff9Files == 0) {
        menuSave_checkNotice(SAVE_TEXT_NO_FF9_DATA);
        return 0;
    }
    return 1;
}

s32 menuSave_countFiles(void) {
    s32 port;
    s32 pass;
    s32 prev;
    s32 count;

    prev = 0;
    pass = 0;
    port = CARD_CHAN(g_menu.cardSlot);
    do {
        count = menuSave_readDirectory(port, (s32)s_menuSaveCheck.dir);
        if (pass != 0 && prev != count) {
            goto fail;
        }
        pass++;
        prev = count;
    } while (pass < 3);
    if (menuSave_cardInfoSync(port) == CARD_RES_DONE) {
        return count;
    }
fail:
    menuSave_checkError(SAVE_TEXT_CHECK_FAILED);
    return -1;
}

void menuSave_finishCheck(void) {
    s_menuSaveCheck.step = CHECK_STEP_DONE;
    s_menuSaveCheck.active = 0;
    menuSave_findNewest();
    if (!menuSave_isMessageActive()) {
        s_menuSave.phase = SAVE_PH_WAIT;
        fade_outDefault();
        fade_setOnDone(menuSave_enterFileList);
        snd_playSe(SE_WINDOW_OPEN);
    }
}

void menuSave_onFileListFadedIn(void) {
    if (s_menuSave.phase != SAVE_PH_MESSAGE) {
        s_menuSave.phase = SAVE_PH_READY;
    }
}

void menuSave_readHeader(s32 file) {
    menuSave_readHeader_inline(file);
}

void menuSave_copyHeader(SaveFile *entry, SaveHeader *h) {
    s32 i;

    entry->version = h->version;
    entry->name = h->name;
    entry->location = h->location;
    entry->name.v[7] = 0xFF;
    entry->discs = h->discs;
    entry->level = h->level;
    entry->playTime = h->playTime;
    entry->gil = h->gil;
    entry->windowColor = h->windowColor;
    entry->counter = h->counter;
    entry->cursor = h->cursor;
    for (i = 0; i < PARTY_SIZE; i++) {
        entry->party[i] = h->party[i];
    }
}

void menuSave_checkError(s32 id) {
    s_menuSaveCheck.step = CHECK_STEP_FAILED;
    win_close(s_menuSaveCheck.win);
    s_menuSaveCheck.win = NULL;
    menuSave_showError(id);
}

void menuSave_checkNotice(s32 id) {
    s_menuSaveCheck.step = CHECK_STEP_FAILED;
    win_close(s_menuSaveCheck.win);
    s_menuSaveCheck.win = NULL;
    menuSave_showNotice(id);
}

void menuSave_initSavedMsg(void) {
    s_menuSaveSavedMsg.active = 0;
    s_menuSaveSavedMsg.win = 0;
    s_menuSaveSavedMsg.timer = 0;
}

void menuSave_openSavedMsg(SaveMenu *menu) {
    if (s_menuSaveSavedMsg.active == 0) {
        s_menuSave.phase = SAVE_PH_WAIT;
        s_menuSaveSavedMsg.timer = 0;
        s_menuSaveSavedMsgRect.y = s_menuSave.list.cursor * SAVE_ROW_H + 0x3F;
        s_menuSaveSavedMsg.win = win_openFree(&s_menuSaveSavedMsgRect, 0x14, WIN_STYLE_BEVEL);
        win_setDrawContent(s_menuSaveSavedMsg.win, menuSave_drawSavedMsg);
        win_setOnOpened(s_menuSaveSavedMsg.win, menuSave_onSavedMsgOpened);
        win_setOnClosed(s_menuSaveSavedMsg.win, menuSave_onSavedMsgClosed);
        win_setShadow(s_menuSaveSavedMsg.win, 1);
    }
}

void menuSave_inputSavedMsg(void) {
    if (s_menuSaveSavedMsg.win->state == WIN_STATE_OPEN && (g_padRepeat & (BTN_CIRCLE | BTN_CROSS))) {
        snd_playSe(SE_CANCEL);
        s_menuSave.phase = SAVE_PH_WAIT;
        win_close(s_menuSaveSavedMsg.win);
    }
}

void menuSave_updateSavedMsg(void) {
    if (!menuSave_isSlotReady(g_menu.cardSlot, 0)) {
        win_closeNow(s_menuSaveSavedMsg.win);
        menuSave_showNotice(SAVE_TEXT_NO_CARD);
        return;
    }
    if (++s_menuSaveSavedMsg.timer >= SAVED_MSG_TICKS) {
        s_menuSave.phase = SAVE_PH_WAIT;
        win_close(s_menuSaveSavedMsg.win);
    }
}

void menuSave_onSavedMsgOpened(Window *w) {
    s_menuSave.phase = SAVE_PH_READY;
    s_menuSave.state = SAVE_ST_SAVED;
}

void menuSave_onSavedMsgClosed(Window *w) {
    s_menuSave.phase = SAVE_PH_READY;
    s_menuSave.state = SAVE_ST_LIST;
    s_menuSaveSavedMsg.win = 0;
    s_menuSaveSavedMsg.timer = 0;
    s_menuSave.help = text_get(SAVE_TEXT_SELECT_BLOCK);
}

void menuSave_drawSavedMsg(Window *w) {
    u8 *msg = text_get(SAVE_TEXT_DATA_SAVED);

    text_drawBlock(w->x + ((w->w - text_measureWidth((s32)msg)) >> 1), w->y + 8, w->otz, msg, TEXT_PAL_WHITE);
}

void menuSave_openMessage(s32 id, s32 title) {
    SaveMessage *msg = &s_menuSaveMessage;
    RECT rect;
    s32 i;
    u16 sz;
    s32 w, h;

    if (msg->active == 0) {
        snd_playSe(SE_MESSAGE);
        s_menuSave.phase = SAVE_PH_WAIT;
        for (i = SAVE_FILES - 1; i >= 0; i--) {
            s_menuSave.files[i].state = SAVE_FILE_NONE;
        }
        msg->active = 1;
        msg->text = text_get(id);
        sz = text_getInfo(id);
        w = (sz & 0xFFF) + 0x10;
        msg->textSize = sz;
        h = (sz >> 12) * 15 + 0x10;
        rect.w = w;
        rect.h = h;
        rect.x = (g_gameState->dispEnv->disp.w - w) >> 1;
        rect.y = (g_gameState->dispEnv->disp.h - h) >> 1;
        msg->win = win_openFree(&rect, 0xF, WIN_STYLE_BEVEL);
        win_setDrawContent(msg->win, menuSave_drawMessage);
        win_setOnOpened(msg->win, menuSave_onMessageOpened);
        win_setOnClosed(msg->win, menuSave_onMessageClosed);
        win_setShadow(msg->win, 1);
        win_setTitle(msg->win, title);
    }
}

void menuSave_initMessage(void) {
    s_menuSaveMessage.active = 0;
}

void menuSave_inputMessage(void) {
    if (s_menuSaveMessage.win->state == WIN_STATE_OPEN && (g_padRepeat & (BTN_CIRCLE | BTN_CROSS))) {
        snd_playSe(SE_CANCEL);
        s_menuSave.phase = SAVE_PH_WAIT;
        win_close(s_menuSaveMessage.win);
    }
}

void menuSave_showError(s32 id) {
    menuSave_openMessage(id, g_labelError);
}

void menuSave_showNotice(s32 id) {
    menuSave_openMessage(id, g_labelNotice);
}

u8 menuSave_setMessageActive(u8 enable) {
    u8 old = s_menuSaveMessage.active;

    s_menuSaveMessage.active = enable;
    return old;
}

u8 menuSave_isMessageActive(void) {
    return s_menuSaveMessage.active;
}

void menuSave_drawMessage(Window *w) {
    text_drawBlock(w->x + 8, w->y + 8, w->otz, s_menuSaveMessage.text, TEXT_PAL_WHITE);
}

void menuSave_onMessageOpened(Window *w) {
    s_menuSave.phase = SAVE_PH_MESSAGE;
}

void menuSave_onMessageClosed(Window *w) {
    s_menuSave.phase = SAVE_PH_READY;
    s_menuSaveMessage.win = NULL;
    s_menuSaveMessage.active = 0;
    if (s_menuSaveResetAfterMsg[s_menuSave.state] != 0) {
        s_menuSave.phase = SAVE_PH_WAIT;
        fade_outDefault();
        fade_setOnDone(menuSave_resetToSlots);
        return;
    }
    s_menuSave.help = text_get(SAVE_TEXT_SELECT_SLOT);
    s_menuSave.state = SAVE_ST_SLOT;
}

void menuSave_resetToSlots(void) {
    s_menuSave.help = text_get(SAVE_TEXT_SELECT_SLOT);
    s_menuSave.state = SAVE_ST_SLOT;
    s_menuSave.phase = SAVE_PH_READY;
    fade_inDefault();
    fade_setOnDone(menuSave_onSlotsFadedIn);
}

void menuSave_onSlotsFadedIn(void) {
    s_menuSave.phase = SAVE_PH_READY;
}

void menuSave_loadImage(void) {
    ui_loadPortraits(s_menuSaveImage, 10);
}

void menuSave_freeImage(void) {
    ui_restorePortraits();
}

u8 menuSave_getPortrait(Character *chr) {
    s32 portrait;

    if (chr == NULL) {
        return PORTRAIT_NONE;
    }
    portrait = chr_getPortrait(chr->info.b.id);
    if (portrait == 0xFF) {
        return PORTRAIT_NONE;
    }
    if (portrait == 2 && (s32)(chr->info.b.kind) >= 5) {
        return 3;
    }
    return s_menuSavePortraitIds[portrait];
}

u8 menuSave_startAccess(s32 mode) {
    s_menuSave.state = SAVE_ST_ACCESS;
    s_menuSave.phase = SAVE_PH_WAIT;
    s_menuSaveAccess.active = 1;
    s_menuSaveAccess.stage = ACCESS_STAGE_NONE;

    s_menuSaveAccess.mode = mode;
    s_menuSaveAccess.delay = 0;
    s_menuSaveAccess.lastProgress = 0;
    s_menuSaveAccess.progressTicks = 0;
    s_menuSaveAccess.unk1 = 0;
    menuSave_resetBookAnim();
    menuSave_resetReadProgress();
    menuSave_resetWriteProgress();
    s_menuSaveAccess.win = win_openFree(&s_menuSaveAccessRect, 0x1E, WIN_STYLE_BEVEL);
    win_setDrawContent(s_menuSaveAccess.win, menuSave_drawAccess);
    win_setOnOpened(s_menuSaveAccess.win, menuSave_onAccessOpened);
    win_setOnClosed(s_menuSaveAccess.win, menuSave_onAccessClosed);
    win_setShadow(s_menuSaveAccess.win, 1);
    win_setTitle(s_menuSaveAccess.win, g_winLabelStrings.labels[s_menuSaveAccessTitles[mode]]);
    return 1;
}

INCLUDE_RODATA("asm/us/ovl_39/nonmatchings/ovl_39", s_menuSaveSlotHelpRect);

INCLUDE_RODATA("asm/us/ovl_39/nonmatchings/ovl_39", g_saveMenuText);

INCLUDE_RODATA("asm/us/ovl_39/nonmatchings/ovl_39", s_menuSaveImage);

INCLUDE_RODATA("asm/us/ovl_39/nonmatchings/ovl_39", s_menuSaveBookImage);

INCLUDE_RODATA("asm/us/ovl_39/nonmatchings/ovl_39", s_menuSaveIconEven);

INCLUDE_RODATA("asm/us/ovl_39/nonmatchings/ovl_39", s_menuSaveIconOdd);

INCLUDE_RODATA("asm/us/ovl_39/nonmatchings/ovl_39", s_menuSaveIconMinute);

INCLUDE_RODATA("asm/us/ovl_39/nonmatchings/ovl_39", s_menuSaveIconHour);

void menuSave_updateSave(void) {
    SaveAccess *access;
    SaveMenu *menu;
    s32 helpId;
    s32 stage;

    switch (s_menuSaveAccess.stage) {
    case ACCESS_STAGE_OPEN:
        access = &s_menuSaveAccess;
        if (++access->delay >= ACCESS_TICKS) {
            menu = &s_menuSave;
            access->delay = 0;
            if (menu->formatted != 0) {
                helpId = SAVE_TEXT_SAVING;
                stage = ACCESS_STAGE_IO;
            } else {
                helpId = SAVE_TEXT_FORMATTING;
                stage = ACCESS_STAGE_FORMAT;
                menu->showCursor = 0;
            }
            access->stage = stage;
            menu->help = text_get(helpId);
        }
        break;
    case ACCESS_STAGE_FORMAT:
        if (++s_menuSaveAccess.delay >= ACCESS_TICKS) {
            s_menuSaveAccess.delay = 0;
            if (menuSave_formatCard()) {
                s_menuSaveAccess.stage = ACCESS_STAGE_FORMATTING;
            }
        }
        break;
    case ACCESS_STAGE_FORMATTING:
        break;
    case ACCESS_STAGE_IO:
        if (++s_menuSaveAccess.delay >= ACCESS_TICKS) {
            s_menuSaveAccess.delay = 0;
            if (menuSave_writeFile()) {
                s_menuSaveAccess.stage = ACCESS_STAGE_WAIT;
            }
        }
        break;
    case ACCESS_STAGE_WAIT:
        break;
    case 6:
        break;
    case 7:
        break;
    }
}

void menuSave_updateLoad(void) {
    switch (s_menuSaveAccess.stage) {
    case ACCESS_STAGE_OPEN:
        if (++s_menuSaveAccess.delay >= ACCESS_TICKS) {
            s_menuSaveAccess.stage = ACCESS_STAGE_IO;
            s_menuSaveAccess.delay = 0;
            s_menuSave.help = text_get(SAVE_TEXT_LOADING);
        }
        break;
    case ACCESS_STAGE_IO:
        if (++s_menuSaveAccess.delay >= ACCESS_TICKS) {
            s_menuSaveAccess.delay = 0;
            if (menuSave_readFile()) {
                s_menuSaveAccess.stage = ACCESS_STAGE_WAIT;
            }
        }
        break;
    case ACCESS_STAGE_WAIT:
        break;
    }
}

void menuSave_drawAccess(Window *w) {
    RECT *rect;
    s32 st;
    s32 tickOffset;

    rect = (RECT *)w;
    if (s_menuSave.formatted != 0) {
        if (s_menuSave.mode != SAVE_MODE_LOAD) {
            st = menuSave_getWriteProgress();
        } else {
            st = menuSave_getReadProgress();
        }
        s_menuSaveAccess.progressTicks = st == s_menuSaveAccess.lastProgress ? s_menuSaveAccess.progressTicks + 1 : 0;
        tickOffset = s_menuSaveAccess.progressTicks * 2;
        if (tickOffset >= 6) {
            tickOffset = 5;
        }
        menuSave_drawBook(rect->x + 0xE, rect->y + 0x12, w->otz);
        text_drawBlock(rect->x + 0x4A, rect->y + 0xE, w->otz, text_get(SAVE_TEXT_DO_NOT_REMOVE), TEXT_PAL_WHITE);
        menuSave_drawGauge(rect->x + 0x1A, rect->y + 0x44, w->otz, st * ACCESS_GAUGE_STEP + tickOffset, ACCESS_GAUGE_W);
        s_menuSaveAccess.lastProgress = st;
        return;
    }
    text_drawBlock(rect->x + 0x26, rect->y + 0xC, w->otz, text_get(SAVE_TEXT_FORMATTING_CARD), TEXT_PAL_WHITE);
    text_drawBlock(rect->x + 0x32, rect->y + 0x20, w->otz, text_get(SAVE_TEXT_DO_NOT_REMOVE), TEXT_PAL_WHITE);
}

u8 menuSave_writeFile(void) {
    SaveFile *sl;
    s32 port;
    s32 fd;

    port = CARD_CHAN(g_menu.cardSlot);
    sl = &s_menuSave.files[s_menuSave.file];
    /* not SAVE_FILE_CORRUPT or SAVE_FILE_USED: create a new file */
    if ((u32)(sl->state - SAVE_FILE_CORRUPT) >= 2) {
        strcpy((char *)s_menuSaveAccess.fileName.name.v, menuSave_getFilePrefix());
        strcat(s_menuSaveAccess.fileName.name.v, "00000-");
        s_menuSaveAccess.fileName.name.v[SAVE_NAME_NUMBER] = s_menuSave.file / 10 + '0';
        s_menuSaveAccess.fileName.name.v[SAVE_NAME_NUMBER + 1] = s_menuSave.file % 10 + '0';
        if (!menuSave_createFile(port, (char *)s_menuSaveAccess.fileName.name.v)) {
            menuSave_abortAccess(SAVE_TEXT_SAVE_FAILED);
            return 0;
        }
    } else {
        strcpy((char *)s_menuSaveAccess.fileName.name.v, sl->fileName.name.v);
    }
    fd = menuSave_openFile(port, (char *)s_menuSaveAccess.fileName.name.v);
    s_menuSaveAccess.fd = fd;
    if (fd < 0) {
        menuSave_abortAccess(SAVE_TEXT_SAVE_FAILED);
        return 0;
    }
    menuSave_writeAsync(fd, (s32)SAVE_BUF, SAVE_BUF_SIZE, menuSave_onFileIo);
    menuSave_enableBookAnim(1);
    return 1;
}

void menuSave_drawGauge(s32 x, s32 y, s32 otz, s32 pos, s32 w) {
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
            line->rgbc = s_menuSaveGaugeColors[i];
            line->xy0 = (x + 4) | yy;
            line->xy1 = (pos + x + 3) | yy;
            addPrim(&g_ot[otz], line);
            g_primPtr = (u8 *)(line + 1);
        }
    }
    ui_drawSpriteDef(x, y, otz, ic, 0);
    ui_drawSpriteDef(x + width - ec->w, y, otz, ec, 0);
    SetTexWindow(g_primPtr, &s_menuSaveGaugeTexWindow);
    addPrim(&g_ot[otz], g_primPtr);
    g_primPtr += 0xC;
    setlen(g_primPtr, PRIM_LEN(SPRT));
    sprt = (SprtWords *)g_primPtr;
    sprt->rgbc = GPU_RGBC(GPU_SPRT, GPU_RGB_NEUTRAL);
    sprt->xy = ((x + ic->w) & 0xFFFF) | (y << 16);
    sprt->uvclut = mid->clut << 16;
    sprt->wh = (width - (ic->w + ec->w)) | (mid->h << 16);
    addPrim(&g_ot[otz], sprt);
    g_primPtr = (u8 *)(sprt + 1);
    texWin.x = mid->u;
    texWin.y = mid->v;
    texWin.w = mid->w;
    texWin.h = mid->h;
    SetTexWindow(g_primPtr, &texWin);
    addPrim(&g_ot[otz], g_primPtr);
    g_primPtr += 0xC;
    SetDrawTPage(g_primPtr, 1, 1, getTPage(0, 0, VRAM_UI_SHEET_X, VRAM_UI_SHEET_Y));
    addPrim(&g_ot[otz], g_primPtr);
    g_primPtr += 8;
}

void menuSave_initAccess(void) {
    s_menuSaveAccess.active = 0;
}

void menuSave_updateAccess(void) {
    if (!menuSave_isSlotReady(g_menu.cardSlot, 0)) {
        menuSave_abortAccess(SAVE_TEXT_NO_CARD);
        return;
    }
    if (s_menuSave.phase != SAVE_PH_MESSAGE) {
        if (s_menuSaveAccess.mode == ACCESS_SAVE) {
            menuSave_updateSave();
        } else {
            menuSave_updateLoad();
        }
    }
}

u8 menuSave_isAccessActive(void) {
    return s_menuSaveAccess.active;
}

u8 menuSave_setAccessActive(u8 enable) {
    u8 old = s_menuSaveAccess.active;

    s_menuSaveAccess.active = enable;
    return old;
}

u8 *menuSave_getFilePrefix(void) {
    memcpy(s_menuSaveFilePrefix, s_menuSaveProductCode, SAVE_PREFIX_LEN + 1);
    return s_menuSaveFilePrefix;
}

void menuSave_onAccessOpened(Window *w) {
    s_menuSave.phase = SAVE_PH_READY;
    s_menuSaveAccess.stage = ACCESS_STAGE_OPEN;
}

void menuSave_onAccessClosed(Window *w) {
    s_menuSaveAccess.active = 0;
    s_menuSave.phase = SAVE_PH_READY;
    if (s_menuSave.mode != SAVE_MODE_LOAD) {
        menuSave_openSavedMsg(&s_menuSave);
        return;
    }
    s_menuSave.state = SAVE_ST_LIST;
}

u8 menuSave_readFile(void) {
    SaveFile *sl;
    s32 port;
    s32 fd;

    port = CARD_CHAN(g_menu.cardSlot);
    sl = &s_menuSave.files[s_menuSave.file];
    strcpy((char *)s_menuSaveAccess.fileName.name.v, sl->fileName.name.v);
    fd = menuSave_openFile(port, (char *)s_menuSaveAccess.fileName.name.v);
    s_menuSaveAccess.fd = fd;
    if (fd < 0) {
        menuSave_abortAccess(SAVE_TEXT_LOAD_FAILED);
        return 0;
    }
    menuSave_readAsync(fd, (s32)SAVE_BUF, SAVE_BUF_SIZE, menuSave_onFileIo);
    return 1;
}

u8 menuSave_formatCard(void) {
    if (menuSave_cardFormat(CARD_CHAN(g_menu.cardSlot)) == 0) {
        menuSave_abortAccess(SAVE_TEXT_FORMAT_FAILED);
        return 0;
    }
    if (menuSave_cardLoad(CARD_CHAN(g_menu.cardSlot), menuSave_onFormatDone) != 0) {
        return 1;
    }
    menuSave_abortAccess(SAVE_TEXT_SAVE_FAILED);
    return 0;
}

void menuSave_onFileIo(void) {
    s32 result = menuSave_getCardResult(CARD_CHAN(g_menu.cardSlot));

    if (menuSave_closeFile(s_menuSaveAccess.fd) < 0) {
        menuSave_abortAccess(s_menuSave.mode == SAVE_MODE_LOAD ? SAVE_TEXT_LOAD_FAILED : SAVE_TEXT_SAVE_FAILED);
        return;
    }
    if (result == CARD_RES_DONE) {
        menuSave_onFileIoDone();
        return;
    }
    menuSave_onFileIoFailed(result);
}

void menuSave_onFileIoDone(void) {
    u8 mode;

    win_close(s_menuSaveAccess.win);
    s_menuSave.phase = SAVE_PH_WAIT;
    mode = s_menuSave.mode;
    if (mode != SAVE_MODE_LOAD) {
        menuSave_storeSavedFile(s_menuSave.file, &SAVE_BUF->hdr);
        snd_playSe(SE_COMPLETE);
        return;
    }
    if (menuSave_applyLoaded()) {
        s_menuSave.loaded = mode;
        snd_playSe(SE_COMPLETE);
        if (g_menu.type == 0) {
            menuScreen_open(0);
            return;
        }
        menuSave_close();
    }
}

void menuSave_onFileIoFailed(s32 result) {
    if (result <= CARD_RES_NEW) {
        if (result <= CARD_RES_DONE) {
            return;
        }
        menuSave_abortAccess(s_menuSave.mode != SAVE_MODE_LOAD ? SAVE_TEXT_SAVE_FAILED : SAVE_TEXT_LOAD_FAILED);
    }
}

void menuSave_storeSavedFile(s32 file, SaveHeader *h) {
    SaveFile *entry;
    SaveFileName *newName;
    s32 i;

    entry = &s_menuSave.files[file];
    newName = &s_menuSaveAccess.fileName;
    entry->fileName = *newName;
    entry->state = SAVE_FILE_USED;

    entry->discs = h->discs;
    entry->level = h->level;
    entry->name = h->name;
    entry->location = h->location;
    entry->playTime = h->playTime;
    entry->gil = h->gil;
    entry->windowColor = h->windowColor;
    entry->counter = h->counter;
    entry->cursor = h->cursor;
    for (i = 0; i < PARTY_SIZE; i++) {
        entry->party[i] = h->party[i];
    }
}

void menuSave_abortAccess(s32 msg) {
    s_menuSave.showCursor = 1;
    menuSave_closeFile(s_menuSaveAccess.fd);
    win_setOnClosed(s_menuSaveAccess.win, menuSave_onAccessAborted);
    win_close(s_menuSaveAccess.win);
    menuSave_showError(msg);
}

void menuSave_onFormatDone(void) {
    s32 result = menuSave_getCardResult();

    s_menuSave.showCursor = 1;
    if (result == CARD_RES_DONE) {
        snd_playSe(SE_WINDOW_OPEN);
        s_menuSave.formatted = 1;
        s_menuSaveAccess.stage = ACCESS_STAGE_IO;
        s_menuSaveAccess.delay = ACCESS_TICKS; /* write on the next tick */
        s_menuSave.help = text_get(SAVE_TEXT_SAVING);
        return;
    }
    win_setOnClosed(s_menuSaveAccess.win, menuSave_onAccessAborted);
    win_close(s_menuSaveAccess.win);
    menuSave_showError(SAVE_TEXT_SAVE_FAILED);
}

void menuSave_onAccessAborted(Window *w) {
    s_menuSaveAccess.active = 0;
    s_menuSave.phase = SAVE_PH_READY;
}

MenuView *menuSave_getListView(MenuList *list, MenuView *view) {
    s32 remaining;
    u8 scrollFlags;

    scrollFlags = list->scroll;
    if (scrollFlags & MENULIST_SCROLL_PAGE) {
        if (scrollFlags & MENULIST_SCROLL_UP) {
            remaining = list->height - list->scrollPos;
            view->rows = (remaining + SAVE_ROW_H - 1) / SAVE_ROW_H;
            view->first = list->top + (list->visibleRows - view->rows);
            view->y = (list->visibleRows - view->rows) * SAVE_ROW_H;
        } else {
            remaining = list->height - list->scrollPos;
            view->rows = (remaining + SAVE_ROW_H - 1) / SAVE_ROW_H;
            view->first = list->top;
            view->y = 0;
        }
    } else if (scrollFlags & MENULIST_SCROLL_UP) {
        view->rows = list->visibleRows + 1;
        view->first = list->top - 1;
        view->y = list->scrollPos - SAVE_ROW_H;
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

MenuView *menuSave_getScrollView(MenuList *list, MenuView *view) {
    s32 x;

    if (list->scroll & MENULIST_SCROLL_UP) {
        view->rows = (list->scrollPos + SAVE_ROW_H - 1) / SAVE_ROW_H;
        view->first = list->pageTop + (list->visibleRows - view->rows);
        view->y = list->scrollPos - view->rows * SAVE_ROW_H;
    } else {
        x = list->scrollPos;
        view->y = list->height - x;
        view->rows = (x + SAVE_ROW_H - 1) / SAVE_ROW_H;
        view->first = list->pageTop;
    }
    return view;
}

void menuSave_getScrollClip(MenuList *list, RECT *src, RECT *dst) {
    dst->x = src->x;
    dst->w = src->w;
    if (list->scroll & MENULIST_SCROLL_UP) {
        dst->y = src->y + list->scrollPos;
    } else {
        dst->y = src->y;
    }
    dst->h = src->h - list->scrollPos;
}

void menuSave_pageUp(MenuList *list, s32 btn) {
    s32 top;
    s32 listH;

    if ((btn & BTN_L1) && !(g_padHeld & BTN_R1) && list->top != 0) {
        listH = list->height;
        list->scrollPos = listH >> 2;
        list->scroll = MENULIST_SCROLL_DOWN | MENULIST_SCROLL_UP | MENULIST_SCROLL_PAGE;
        list->pageStep = (listH - list->scrollPos + 0xE) / 15;
        top = list->top - list->visibleRows;
        if (top < 0) {
            top = 0;
        }
        list->pageTop = top;
        snd_playSe(SE_CURSOR);
    }
}

void menuSave_pageDown(MenuList *list, s32 pad) {
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

void menuSave_buildHeader(SaveBuf *save) {
    SaveHeader *h;
    EvtObject *player;
    s32 elapsed;
    s32 i;
    u8 chrIdx;

    h = &save->hdr;
    elapsed = game_getPlayTime();
    menuSave_buildCardHeader(&save->card, elapsed);
    chrIdx = 0;
    player = evt_getPlayer();
    if (player != NULL) {
        chrIdx = player->modelSlot;
    }
    if (s_menuSave.mode == SAVE_MODE_NEXT_DISC) {
        h->discs = fld_getDiscChangeMask();
    } else {
        h->discs = g_gameState->disc;
    }
    h->discs = menuSave_getDiscBit();
    h->version = SAVE_VERSION;
    h->level = g_gameState->gameData->characters[chrIdx].level;
    h->playTime = elapsed;
    h->gil = g_gameState->gameData->gil;
    h->windowColor = (g_gameState->config >> CFG_BIT_WINDOW_COLOR) & 1;
    h->counter = ++s_menuSave.newestCounter;
    h->cursor = s_menuSave.list.cursor;
    h->name = *(SaveName *)g_gameState->gameData->characters[chrIdx].name;
    menuSave_getLocation((u8 *)&h->location);
    for (i = 0; i < PARTY_SIZE; i++) {
        if (g_gameState->gameData->party[i] != NULL) {
            h->party[i].present = 1;
            h->party[i].portrait = menuSave_getPortrait(g_gameState->gameData->party[i]);
            h->party[i].level = g_gameState->gameData->party[i]->level;
            *(Name10 *)h->party[i].name = *(Name10 *)g_gameState->gameData->party[i]->name;
        } else {
            h->party[i].present = 0;
            h->party[i].portrait = PORTRAIT_NONE;
            h->party[i].level = 0;
            memset(h->party[i].name, TEXT_END, 10);
        }
    }
}

void menuSave_getLocation(u8 *dst) {
    u8 *name;
    u8 *fallback;

    if (s_menuSave.mode == SAVE_MODE_NEXT_DISC) {
        name = text_get(fld_getDiscChangeDisc() + SAVE_TEXT_DISC_START);
    } else {
        name = g_gameState->gameData->locationName;
    }
    if (name == NULL) {
        fallback = text_get(SAVE_TEXT_NO_LOCATION);
        memset(dst, 0xFF, sizeof(SaveLocation));
        memcpy(dst, fallback, text_getLength(fallback));
    } else {
        memset(s_menuSaveLocationBuf, 0xFF, sizeof(s_menuSaveLocationBuf));
        memcpy(s_menuSaveLocationBuf, name, text_getLength(name));
        s_menuSaveLocationBuf[sizeof(SaveLocation) - 1] = 0xFF; /* cut to fit */
        memset(dst, 0xFF, sizeof(SaveLocation));
        memcpy(dst, s_menuSaveLocationBuf, text_getLength(s_menuSaveLocationBuf));
    }
}

void menuSave_buildCardHeader(CardFileHeader *hdr, s32 elapsed) {
    u8 *title;
    IconTmpl *icon;
    s32 sec;
    s32 min;
    s32 hour;
    s32 fileNo;

    hdr->magic = CARD_MAGIC;
    hdr->iconFlag = CARD_ICON_1FRAME;
    hdr->blocks = 1;
    title = hdr->title;
    memset(hdr->pad44, 0, sizeof(hdr->pad44));
    sec = (u32)elapsed / 60;
    min = sec / 60;
    hour = min / 60;
    min = min % 60;
    sec = sec % 60;
    if (hour >= 100) {
        hour %= 100;
    }
    memset(title, 0, sizeof(hdr->title));
    /* "ＦＦ９／ＦＩＬＥ００／００：００／" in 2-byte SJIS: bump the low bytes of the digits,
       file number at 0x11/0x13, hours at 0x17/0x19, minutes at 0x1D/0x1F; 0x20 cuts the last "／" */
    *(TitleTmpl *)title = s_menuSaveTitleTemplate;
    title[0x11] += (s_menuSave.file + 1) / 10;
    fileNo = s_menuSave.file + 1;
    title[0x17] += hour / 10;
    title[0x20] = 0;
    title[0x19] += hour % 10;
    title[0x1D] += min / 10;
    title[0x1F] += min % 10;
    title[0x13] += fileNo % 10;
    if (sec == 0) {
        if (min == 0) {
            icon = &s_menuSaveIconHour;
        } else {
            icon = &s_menuSaveIconMinute;
        }
    } else if (elapsed & 1) {
        icon = &s_menuSaveIconOdd;
    } else {
        icon = &s_menuSaveIconEven;
    }
    hdr->icon = *icon;
}

void menuSave_saveFile(s32 file, s32 unused) {
    s_menuSave.file = file;
    if (s_menuSave.formatted == 0) {
        snd_playSe(SE_CURSOR);
        menuSave_setConfirmCallback(menuSave_startSave);
        menuSave_openConfirm(g_menu.cardSlot + SAVE_TEXT_UNFORMATTED, SAVE_TEXT_FORMAT_YES, SAVE_TEXT_FORMAT_NO);
        return;
    }
    switch (s_menuSave.files[s_menuSave.file].state) {
    case SAVE_FILE_EMPTY:
    case SAVE_FILE_CORRUPT:
        snd_playSe(SE_CURSOR);
        menuSave_startSave();
        break;
    case SAVE_FILE_USED:
        snd_playSe(SE_CURSOR);
        menuSave_setConfirmCallback(menuSave_startSave);
        menuSave_openConfirm(SAVE_TEXT_OVERWRITE, SAVE_TEXT_OVERWRITE_YES, SAVE_TEXT_OVERWRITE_NO);
        break;
    case SAVE_FILE_OTHER:
        snd_playSe(SE_BUZZER);
        break;
    }
}

void menuSave_startSave(void) {
    menuSave_initCrcTable();
    menuSave_buildHeader(SAVE_BUF);
    menuSave_packData(&SAVE_BUF->data);
    SAVE_BUF->crc = menuSave_crc16((u8 *)SAVE_BUF, SAVE_CRC_LEN);
    menuSave_startAccess(ACCESS_SAVE);
}

void menuSave_packData(SaveData *saveData) {
    menuSave_packState(&saveData->state);
    menuSave_packEvents(&saveData->events);
    menuSave_packParty(&saveData->party);
    menuSave_packMisc(&saveData->misc);
    menuSave_packItems(&saveData->items);
    menuSave_packCards(saveData->cards);
}

/* Story counter thresholds: g_evtGlobalVars.h[0] below 0xF3C is disc 1, and so on. */
s32 menuSave_getDiscBit(void) {
    if ((u16)g_evtGlobalVars.h[0] < 0xF3C) {
        return 1 << 0;
    }
    if ((u16)g_evtGlobalVars.h[0] < 0x1B4E) {
        return 1 << 1;
    }
    if ((u16)g_evtGlobalVars.h[0] < 0x2AF3) {
        return 1 << 2;
    }
    return 1 << 3;
}

void menuSave_initCrcTable(void) {
    u16 i;
    u16 j;
    u16 crc;

    for (i = 0; i < 256; i++) {
        crc = i;
        for (j = 0; j < 8; j++) {
            if (crc & 1) {
                crc = (crc >> 1) ^ CRC16_POLY;
            } else {
                crc >>= 1;
            }
        }
        s_menuSaveCrcTable[i] = crc;
    }
}

u16 menuSave_crc16(u8 *p, s32 len) {
    Crc16 crc = { CRC16_INIT };

    while (--len >= 0) {
        crc.w = s_menuSaveCrcTable[(crc.w ^ *p++) & 0xFF] ^ (crc.h >> 8);
    }
    return crc.w;
}

void menuSave_loadFile(s32 file, s32 unused) {
    SaveFile *files;

    files = s_menuSave.files;
    s_menuSave.file = file;
    switch (files[file].state) {
    case SAVE_FILE_USED:
        snd_playSe(SE_CURSOR);
        menuSave_startLoad();
        break;
    case SAVE_FILE_EMPTY:
    case SAVE_FILE_OTHER:
    case SAVE_FILE_CORRUPT:
        snd_playSe(SE_BUZZER);
        break;
    }
}

u8 menuSave_applyLoaded(void) {
    SaveHeader *h;
    u8 ok;

    ok = 1;
    menuSave_initCrcTable();
    if (SAVE_BUF->crc == menuSave_crc16((u8 *)SAVE_BUF, SAVE_CRC_LEN)) {
        h = &SAVE_BUF->hdr;
        g_gameState->disc = h->discs;
        game_setPlayTime(h->playTime);
        menuSave_unpackState(&SAVE_BUF->data.state);
        menuSave_unpackEvents(&SAVE_BUF->data.events);
        menuSave_unpackParty(&SAVE_BUF->data.party);
        menuSave_unpackMisc(&SAVE_BUF->data.misc);
        menuSave_unpackItems(&SAVE_BUF->data.items);
        menuSave_unpackCards(SAVE_BUF->data.cards);
    } else {
        menuSave_showError(SAVE_TEXT_CORRUPTED);
        ok = 0;
    }
    return ok;
}

void menuSave_startLoad(void) {
    if (!menuSave_startAccess(ACCESS_LOAD)) {
        menuSave_showError(SAVE_TEXT_LOAD_FAILED);
    }
}


void menuSave_packParty(SaveParty *saveParty) {
    GameData *gd;
    s32 i;

    gd = g_gameState->gameData;
    for (i = 0; i < SAVE_CHARACTERS; i++) {
        saveParty->characters[i] = gd->characters[i];
    }
    for (i = 0; i < PARTY_SIZE; i++) {
        if (gd->party[i] != NULL) {
            saveParty->party[i] = gd->party[i]->info.b.id;
        } else {
            saveParty->party[i] = SAVE_CHARACTERS;
        }
    }
    saveParty->escapeCount = gd->escapeCount;
    saveParty->firstUseFxSeen = gd->firstUseFxSeen;
    saveParty->gil = gd->gil;
    saveParty->frogCount = gd->frogCount;
    saveParty->stealCount = gd->stealCount;
    saveParty->dragonKills = gd->dragonKills;
}

void menuSave_unpackParty(SaveParty *saveParty) {
    GameData *gd;
    s32 i;

    gd = g_gameState->gameData;
    for (i = 0; i < SAVE_CHARACTERS; i++) {
        gd->characters[i] = saveParty->characters[i];
    }
    for (i = 0; i < PARTY_SIZE; i++) {
        if (saveParty->party[i] < SAVE_CHARACTERS) {
            gd->party[i] = &gd->characters[saveParty->party[i]];
        } else {
            gd->party[i] = NULL;
        }
    }
    gd->escapeCount = saveParty->escapeCount;
    gd->firstUseFxSeen = saveParty->firstUseFxSeen;
    gd->gil = saveParty->gil;
    gd->frogCount = saveParty->frogCount;
    gd->stealCount = saveParty->stealCount;
    gd->dragonKills = saveParty->dragonKills;
}

void menuSave_packState(SaveState *h) {
    GameData *gd;
    s32 i;

    gd = g_gameState->gameData;
    h->mode = g_gameState->mode;
    h->prevMode = g_gameState->prevMode;
    for (i = sizeof(h->reserved2) - 1; i >= 0; i--) {
        h->reserved2[i] = 0;
    }
    if (s_menuSave.mode == SAVE_MODE_NEXT_DISC) {
        h->fieldId = fld_getDiscChangeFieldId();
        h->areaId = fld_getDiscChangeAreaId();
    } else {
        h->fieldId = gd->fieldId;
        h->areaId = gd->fieldAreaId;
    }
    h->battleId = gd->battleId;
    h->enemyGroup = (s8)gd->enemyGroup;
    h->worldId = gd->worldId;
    h->worldAreaId = gd->worldAreaId;
    for (i = sizeof(h->reserved2C) - 1; i >= 0; i--) {
        h->reserved2C[i] = 0;
    }
}

void menuSave_unpackState(SaveState *saveState) {
    GameState *gs = g_gameState;
    GameData *gd;

    gs->mode = saveState->mode;
    gd = gs->gameData;
    g_gameState->prevMode = saveState->prevMode;
    gd->fieldId = saveState->fieldId;
    gd->fieldAreaId = saveState->areaId;
    gd->battleId = saveState->battleId;
    gd->enemyGroup = saveState->enemyGroup;
    gd->worldId = saveState->worldId;
    gd->worldAreaId = saveState->worldAreaId;
}

void menuSave_copyCards(CardData *dst, CardData *src) {
    s32 i;

    dst->wins = src->wins;
    dst->losses = src->losses;
    dst->draws = src->draws;
    for (i = 0; i < 0x69; i++) {
        dst->cards[i].id = src->cards[i].id;
        dst->cards[i].arrows = src->cards[i].arrows;
        dst->cards[i].power = src->cards[i].power;
        dst->cards[i].type = src->cards[i].type;
        dst->cards[i].physDef = src->cards[i].physDef;
        dst->cards[i].magicDef = src->cards[i].magicDef;
    }
    dst->unk27C = src->unk27C;
    dst->unk27E = src->unk27E;
}

void menuSave_packCards(void *dst) {
    menuSave_copyCards(dst, card_getInventory());
}

void menuSave_unpackCards(void *src) {
    menuSave_copyCards(card_getInventory(), src);
}

void menuSave_packEvents(SaveEvents *saveEvents) {
    saveEvents->vars = g_evtGlobalVars;
    saveEvents->evtStepCount = g_evtStepCount;
}

void menuSave_unpackEvents(SaveEvents *saveEvents) {
    g_evtGlobalVars = saveEvents->vars;
    g_evtStepCount = saveEvents->evtStepCount;
}

void menuSave_packItems(SaveItems *saveItems) {
    saveItems->items = g_gameState->gameData->items;
    saveItems->flags = g_gameState->gameData->keyItems;
    saveItems->config = *(GameConfig *)&g_gameState->config;
}

void menuSave_unpackItems(SaveItems *saveItems) {
    g_gameState->gameData->items = saveItems->items;
    g_gameState->gameData->keyItems = saveItems->flags;
    *(GameConfig *)&g_gameState->config = saveItems->config;
    config_apply();
}

void menuSave_packMisc(SaveMisc *saveMisc) {
    saveMisc->chocograph = g_gameState->gameData->chocograph;
    saveMisc->worldCamera = g_gameState->gameData->worldCamera;
    saveMisc->inWorldFrame = g_gameState->gameData->inWorldFrame;
}

void menuSave_unpackMisc(SaveMisc *saveMisc) {
    g_gameState->gameData->chocograph = saveMisc->chocograph;
    g_gameState->gameData->worldCamera = saveMisc->worldCamera;
    g_gameState->gameData->inWorldFrame = saveMisc->inWorldFrame;
}

void menuSave_cardInit(void) {
    s_menuSaveCardOp = CARD_OP_INFO;
    s_menuSaveCardInfoChan = 0;
    s_menuSaveCardInfoBusy = 0;
    s_menuSaveInfoCallback1 = 0;
    s_menuSaveInfoCallback2 = 0;

}

INCLUDE_RODATA("asm/us/ovl_39/nonmatchings/ovl_39", s_menuSaveProductCode);

INCLUDE_RODATA("asm/us/ovl_39/nonmatchings/ovl_39", s_menuSaveTitleTemplate);

/* dir: a struct DIRENTRY * passed as an int, as libapi's firstfile/nextfile take it */
s32 menuSave_firstFile(s32 chan, s32 dir) {
    char path[16];

    if (chan == 0) {
        strcpy(path, "bu00:*");
    } else {
        strcpy(path, "bu10:*");
    }
    _card_wait(0);
    _card_wait(1);
    menuSave_cardIdle();
    return firstfile(path, dir);
}

s32 menuSave_nextFile(s32 dir) {
    _card_wait(0);
    _card_wait(1);
    menuSave_cardIdle();
    return nextfile(dir);
}

s32 menuSave_readDirectory(s32 chan, s32 dir) {
    s32 count = 0;

    if (menuSave_firstFile(chan, dir) == dir) {
        do {
            count++;
            dir += sizeof(DirEnt);
        } while (menuSave_nextFile(dir) == dir);
    }
    return count;
}

s32 menuSave_getCardStatus(s32 chan) {
    if (chan == 0) {

        return s_menuSaveCardStatus[0];
    }
    return s_menuSaveCardStatus[1];
}

s32 menuSave_cardInfoSync(s32 chan) {
    s32 result;

    menuSave_clearSwCard();
    _card_wait(0);
    _card_wait(1);
    _card_info(chan);
    while ((result = menuSave_testSwCard()) == -1) {
    }
    return result;
}

s32 menuSave_getCardOp(void) {
    return s_menuSaveCardOp;
}

void menuSave_resetReadProgress(void) {
    s_menuSaveSectorsDone = 0;
}

void menuSave_resetWriteProgress(void) {
    s_menuSaveSectorsDone = 0;
}

s32 menuSave_getReadProgress(void) {
    return s_menuSaveSectorsDone;
}

s32 menuSave_getWriteProgress(void) {
    return s_menuSaveSectorsDone;
}

s32 menuSave_isCardBusy(void) {
    return s_menuSaveCardOp != CARD_OP_INFO;
}

s32 menuSave_cardClearSync(s32 chan) {
    s32 result;

    _card_wait(0);
    _card_wait(1);
    menuSave_clearHwCard();
    _card_clear(chan);
    do {
        result = menuSave_testHwCard();
    } while (result == -1);
    menuSave_cardIdle();
    return result;
}

s32 menuSave_cardLoad(s32 chan, void (*cb)(void)) {
    s32 result = menuSave_cardLoadStart(chan);

    s_menuSaveCardCallback = cb;
    return result;
}

s32 menuSave_cardLoadStart(s32 chan) {
    s_menuSaveCardCallback = NULL;
    s_menuSaveCardChan = chan;
    _card_wait(0);
    _card_wait(1);
    menuSave_clearSwCard();
    s_menuSaveCardOp = CARD_OP_LOAD;
    return _card_load(chan);
}

s32 menuSave_openFile(s32 chan, char *name) {
    char path[32];

    _card_wait(0);
    _card_wait(1);
    s_menuSaveFileChan = chan;
    if (chan == 0) {
        strcpy(path, "bu00:");
    } else {
        strcpy(path, "bu10:");
    }
    strcat((u8 *)path, (u8 *)name);
    menuSave_cardIdle();
    return open(path, FASYNC | FWRITE | FREAD);
}

s32 menuSave_closeFile(s32 fd) {
    _card_wait(0);
    _card_wait(1);
    menuSave_cardIdle();
    return close(fd);
}

s32 menuSave_writeAsync(s32 fd, s32 buf, s32 size, void (*cb)(void)) {
    s32 result = menuSave_writeStart(fd, buf, size);

    s_menuSaveCardCallback = cb;
    return result;
}

s32 menuSave_writeStart(s32 fd, s32 buf, s32 size) {
    s_menuSaveCardCallback = NULL;
    s_menuSaveFd = fd;
    s_menuSaveSectorsDone = 0;
    s_menuSaveIoBuf = buf;
    s_menuSaveSectorsLeft = (size + CARD_SECTOR - 1) / CARD_SECTOR - 1;
    menuSave_writeSector();
    return 1;
}

s32 menuSave_writeSector(void) {
    _card_wait(0);
    _card_wait(1);
    menuSave_clearSwCard();
    if (write(s_menuSaveFd, s_menuSaveIoBuf, CARD_SECTOR) == -1) {
        return 0;
    }
    s_menuSaveCardOp = CARD_OP_WRITE;
    return 1;
}

s32 menuSave_readAsync(s32 fd, s32 buf, s32 size, void (*cb)(void)) {
    s32 result = menuSave_readStart(fd, buf, size);

    s_menuSaveCardCallback = cb;
    return result;
}

s32 menuSave_readStart(s32 fd, s32 buf, s32 size) {
    s_menuSaveCardCallback = NULL;
    s_menuSaveFd = fd;
    s_menuSaveSectorsDone = 0;
    s_menuSaveIoBuf = buf;
    s_menuSaveSectorsLeft = (size + CARD_SECTOR - 1) / CARD_SECTOR - 1;
    _card_wait(0);
    _card_wait(1);
    menuSave_clearSwCard();
    menuSave_readSector();
    return 1;
}

s32 menuSave_readSector(void) {
    _card_wait(0);
    _card_wait(1);
    menuSave_clearSwCard();
    if (read(s_menuSaveFd, s_menuSaveIoBuf, CARD_SECTOR) == -1) {
        return 0;
    }
    s_menuSaveCardOp = CARD_OP_READ;
    return 1;
}

s32 menuSave_createFile(s32 chan, char *name) {
    char path[32];
    s32 fd;
    s32 ok;

    _card_wait(0);
    _card_wait(1);
    if (chan == 0) {
        strcpy(path, "bu00:");
    } else {
        strcpy(path, "bu10:");
    }
    strcat((u8 *)path, (u8 *)name);
    fd = open(path, (1 << 16) | FCREAT); /* 1 block */
    ok = fd != -1;
    close(fd);
    return ok;
}

s32 menuSave_cardFormat(s32 chan) {
    _card_wait(0);
    _card_wait(1);
    menuSave_cardIdle();
    return _card_format(chan);
}

void menuSave_cardPoll(void) {
    s32 result;

    switch (s_menuSaveCardOp) {
    case CARD_OP_INFO:
        if (s_menuSaveCardInfoBusy == 0) {
            menuSave_clearSwCard();
            /* alternate between the two slots */
            s_menuSaveCardChan = s_menuSaveCardInfoChan = (s_menuSaveCardInfoChan + CARD_CHAN(1)) & CARD_CHAN(1);
            _card_wait(0);
            _card_wait(1);
            _card_info(s_menuSaveCardChan);
            s_menuSaveCardInfoBusy = 1;
        }
        result = menuSave_testSwCard();
        switch (result) {
        case CARD_RES_DONE:
            menuSave_cardPoll_set(s_menuSaveCardChan, CARD_RES_DONE);
            break;
        case CARD_RES_ERROR:
        case CARD_RES_NEW:
            menuSave_cardPoll_set(s_menuSaveCardChan, result);
            break;
        case CARD_RES_TIMEOUT:
            menuSave_cardPoll_retry(s_menuSaveCardChan, result);
            break;
        }
        if (result != -1) {
            if (s_menuSaveCardChan == 0) {
                if (s_menuSaveInfoCallback1) {
                    ((void (*)(void))s_menuSaveInfoCallback1)();
                }
                s_menuSaveInfoCallback1 = 0;
            } else {
                if (s_menuSaveInfoCallback2) {
                    ((void (*)(void))s_menuSaveInfoCallback2)();
                }
                s_menuSaveInfoCallback2 = 0;
            }
        }
        break;
    case CARD_OP_CLEAR:
        menuSave_testHwCard();
        break;
    case CARD_OP_LOAD:
        result = menuSave_testSwCard();
        s_menuSaveCardResult = result;
        if (result == -1) {
            break;
        }
        goto callback;
    case CARD_OP_READ:
        result = menuSave_testSwCard();
        s_menuSaveCardResult = result;
        if (result == -1) {
            break;
        }
        if (result != CARD_RES_DONE) {
            goto callback;
        }
        s_menuSaveCardResult = CARD_RES_DONE;
        lseek(s_menuSaveFd, CARD_SECTOR, SEEK_CUR);
        s_menuSaveSectorsDone++;
        if (s_menuSaveSectorsLeft != 0) {
            s_menuSaveSectorsLeft--;
            s_menuSaveIoBuf += CARD_SECTOR;
            menuSave_readSector();
            break;
        }
        goto callback;
    case CARD_OP_WRITE:
        result = menuSave_testSwCard();
        s_menuSaveCardResult = result;
        if (result == -1) {
            break;
        }
        if (result != CARD_RES_DONE) {
            goto callback;
        }
        s_menuSaveSectorsDone++;
        if (s_menuSaveSectorsLeft != 0) {
            s_menuSaveSectorsLeft--;
            s_menuSaveIoBuf += CARD_SECTOR;
            menuSave_writeSector();
            break;
        }
    callback:
        if (s_menuSaveCardCallback != NULL) {
            s_menuSaveCardCallback();
        }
        break;
    }
}

s32 menuSave_getCardResult(s32 chan) {
    return s_menuSaveCardResult;
}

void menuSave_cardIdle(void) {
    s_menuSaveCardOp = CARD_OP_INFO;

    s_menuSaveCardInfoBusy = 0;
}

s32 menuSave_testSwCard(void) {
    s32 result = -1;

    if (TestEvent(s_menuSaveSwEventIoe) == 1) {
        result = CARD_RES_DONE;
    } else if (TestEvent(s_menuSaveSwEventError) == 1) {
        result = CARD_RES_ERROR;
    } else if (TestEvent(s_menuSaveSwEventTimeout) == 1) {
        result = CARD_RES_TIMEOUT;
    } else if (TestEvent(s_menuSaveSwEventNew) == 1) {
        result = CARD_RES_NEW;
    }
    if (result != -1) {
        menuSave_cardIdle();
    }
    return result;
}

s32 menuSave_testHwCard(void) {
    s32 result = -1;

    if (TestEvent(s_menuSaveHwEventIoe) == 1) {
        result = CARD_RES_DONE;
    } else if (TestEvent(s_menuSaveHwEventError) == 1) {
        result = CARD_RES_ERROR;
    } else if (TestEvent(s_menuSaveHwEventTimeout) == 1) {
        result = CARD_RES_TIMEOUT;
    } else if (TestEvent(s_menuSaveHwEventNew) == 1) {
        result = CARD_RES_NEW;
    }
    if (result != -1) {
        menuSave_cardIdle();
    }
    return result;
}

void menuSave_openCardEvents(void) {
    EnterCriticalSection();
    s_menuSaveSwEventIoe = OpenEvent(SwCARD, EvSpIOE, EvMdNOINTR, NULL);
    s_menuSaveSwEventError = OpenEvent(SwCARD, EvSpERROR, EvMdNOINTR, NULL);
    s_menuSaveSwEventTimeout = OpenEvent(SwCARD, EvSpTIMOUT, EvMdNOINTR, NULL);
    s_menuSaveSwEventNew = OpenEvent(SwCARD, EvSpNEW, EvMdNOINTR, NULL);
    s_menuSaveHwEventIoe = OpenEvent(HwCARD, EvSpIOE, EvMdNOINTR, NULL);
    s_menuSaveHwEventError = OpenEvent(HwCARD, EvSpERROR, EvMdNOINTR, NULL);
    s_menuSaveHwEventTimeout = OpenEvent(HwCARD, EvSpTIMOUT, EvMdNOINTR, NULL);
    s_menuSaveHwEventNew = OpenEvent(HwCARD, EvSpNEW, EvMdNOINTR, NULL);
    EnableEvent(s_menuSaveSwEventIoe);
    EnableEvent(s_menuSaveSwEventError);
    EnableEvent(s_menuSaveSwEventTimeout);
    EnableEvent(s_menuSaveSwEventNew);
    EnableEvent(s_menuSaveHwEventIoe);
    EnableEvent(s_menuSaveHwEventError);
    EnableEvent(s_menuSaveHwEventTimeout);
    EnableEvent(s_menuSaveHwEventNew);
    ExitCriticalSection();
}

void menuSave_clearSwCard(void) {
    TestEvent(s_menuSaveSwEventIoe);
    TestEvent(s_menuSaveSwEventError);
    TestEvent(s_menuSaveSwEventTimeout);
    TestEvent(s_menuSaveSwEventNew);
}

void menuSave_clearHwCard(void) {
    TestEvent(s_menuSaveHwEventIoe);
    TestEvent(s_menuSaveHwEventError);
    TestEvent(s_menuSaveHwEventTimeout);
    TestEvent(s_menuSaveHwEventNew);
}

void menuSave_closeCardEvents(void) {
    EnterCriticalSection();
    CloseEvent(s_menuSaveSwEventIoe);
    CloseEvent(s_menuSaveSwEventError);
    CloseEvent(s_menuSaveSwEventTimeout);
    CloseEvent(s_menuSaveSwEventNew);
    CloseEvent(s_menuSaveHwEventIoe);
    CloseEvent(s_menuSaveHwEventError);
    CloseEvent(s_menuSaveHwEventTimeout);
    CloseEvent(s_menuSaveHwEventNew);
    ExitCriticalSection();
}
