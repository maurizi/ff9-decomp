#include "ovl_39.h"

void menuSave_init(void) {
    s32 i;

    s_menuSave.mode = g_menu.unkC;
    s_menuSave.savedFadeOtz = func_8002F8E4(7);
    s_menuSave.state = SAVE_ST_SLOT;
    s_menuSave.phase = SAVE_PH_READY;
    s_menuSave.loaded = 0;
    s_menuSave.showCursor = 1;
    s_menuSave.newestCounter = 0;
    s_menuSave.newestFile = 0;
    g_menu.unk2 &= 1;
    menuSave_initCheck();
    menuSave_initMessage();
    menuSave_initSavedMsg();
    menuSave_initConfirm();
    menuSave_initAccess();
    menuSave_loadImage();
    menuSave_saveVram();
    func_8003556C(menuSave_updateHelp);
    text_setBank(g_saveMenuText);
    s_menuSave.help = text_get(SAVE_TEXT_SELECT_SLOT);
    s_menuSave.cardStatus[0] = CARD_RES_ERROR;
    s_menuSave.cardStatus[1] = CARD_RES_ERROR;
    menuSave_cardInit();
    menuSave_openCardEvents();
    func_801F5A60();
    for (i = 0; i < 2; i++) {
        s_menuSave.unk18[i] = D_80073998[i];
        D_80073998[i] |= 0x300;
    }
}

void menuSave_drawHelp(void) {
    u8 *s = text_get(g_menu.unk2 + SAVE_TEXT_SLOT1);
    s32 w = func_8002BA68((s32)s);

    text_drawBlock(s_menuSaveSlotNameRect.x + ((s_menuSaveSlotNameRect.w - w) >> 1), s_menuSaveSlotNameRect.y + 2, 5, s, (menuSave_isSlotReady(g_menu.unk2, 1) == 0) * 2);
    if (s_menuSave.help != NULL) {
        text_drawBlock(s_menuSaveHelpRect.x + 0xA, s_menuSaveHelpRect.y + 6, 5, s_menuSave.help, 0);
    }
    ui_drawLabel(s_menuSaveHelpRect.x + 6, s_menuSaveHelpRect.y - 3, 5, D_80070450, 0);
    func_8002DEE4(&s_menuSaveSlotNameRect, 5, 0);
    func_8002DEE4(&s_menuSaveHelpRect, 5, 1);
    func_8002705C(&s_menuSaveHelpRect, 5, 1);
}

void menuSave_inputSlot(void) {
    if (g_padRepeat & BTN_CIRCLE) {
        snd_playSe(SE_CANCEL);
        if (g_menu.unk1 != 0) {
            menuSave_close();
            return;
        }
        func_800361CC(0);
        return;
    }
    if (g_padRepeat & PAD_UP) {
        snd_playSe(SE_CURSOR);
        g_menu.unk2 = g_menu.unk2 == 0 ? 1 : g_menu.unk2 - 1;
    }
    if (g_padRepeat & PAD_DOWN) {
        snd_playSe(SE_CURSOR);
        g_menu.unk2 = g_menu.unk2 != 0 ? 0 : g_menu.unk2 + 1;
    }
    if (g_padRepeat & BTN_CROSS) {
        menuSave_selectSlot();
    }
}

void menuSave_drawSlotWindow(void) {
    s32 i;
    s32 ok;
    u8 *str;
    u8 s;

    func_8002DEE4(&s_menuSaveSlotWindowRect, 0x34, 2);
    func_8002705C(&s_menuSaveSlotWindowRect, 0x34, 2);
    for (i = 0; i < 2; i++) {
        str = text_get(i + SAVE_TEXT_SLOT1);
        ok = 0;
        s = s_menuSave.cardStatus[i];
        if (s == CARD_RES_DONE || s == CARD_RES_NEW) {
            ok = 1;
        }
        text_drawBlock(s_menuSaveSlotWindowRect.x + 10, s_menuSaveSlotWindowRect.y + 10 + i * 18, 0x33, str, (ok ^ 1) * 2);
    }
}

void menuSave_drawFileList(void) {
    MenuView v;
    RECT r;
    RECT a, b;
    s32 i;
    MenuList *l = &s_menuSave.list;

    menuSave_drawScrollArrows();
    ui_drawLabel(0xE, 0x23, 0x2C, D_80070444, 0);
    ui_drawRule(8, 0x26, 0x2C, 1, 0x12C, (g_gameState->unk24 >> CFG_BIT_WINDOW_COLOR) & 1);
    r.x = SAVE_LIST_X;
    r.y = 0;
    r.w = SAVE_LIST_W;
    r.h = SAVE_ROW_H;
    menuSave_getListView(l, &v);
    win_setClip(NULL, 0x2D, 4);
    for (i = 0; i < v.unk0; i++) {
        r.y = i * SAVE_ROW_H + (s16)(v.unk2 + SAVE_LIST_Y);
        menuSave_drawFile(&r, 0x2D, v.unk1 + i);
    }
    if (l->unk8 & 4) {
        a.x = SAVE_LIST_X;
        a.y = SAVE_LIST_Y;
        a.w = SAVE_LIST_W;
        a.h = SAVE_LIST_H;
        menuSave_getScrollClip(l, &a, &b);
        win_setClip(&b, 0x2D, 4);
        menuSave_drawScrollRows();
    }
    r.y = SAVE_LIST_Y;
    r.h = SAVE_LIST_H;
    win_setClip(&r, 0x2D, 4);
}

void menuSave_drawFile(RECT *r, s32 otz, s32 file) {
    char name[8];
    void (*fn)(RECT *, s32, s32);
    s32 v;

    strcpy(name, "File00");
    name[4] = (file + 1) / 10 + '0';
    name[5] = (file + 1) % 10 + '0';
    ui_drawSmallDigits(r->x + 6, r->y + 3, otz, (u8 *)name, 0);
    fn = s_menuSaveFileDrawers[s_menuSave.files[file].state];
    if (fn != NULL) {
        fn(r, otz, file);
    }
    if (s_menuSave.files[file].state == SAVE_FILE_USED) {
        v = s_menuSave.files[file].windowColor;
    } else {
        v = g_gameState->unk24 >> CFG_BIT_WINDOW_COLOR;
        v &= 1;
    }
    func_8002CD4C(r, 0x2D, 0, v);
}

void menuSave_drawFileInfo(RECT *r, s32 z, s32 i) {
    u8 buf[16];
    RECT rr;
    SaveFile *s;
    s32 j;
    s32 n;

    s = &s_menuSave.files[i];
    n = 0;
    for (j = n; j < 4; j++) {
        if ((s->discs >> j) & 1) {
            buf[n++] = j + '1';
        }
    }
    buf[n] = 0;
    ui_drawSmallDigits(r->x + 0x30, r->y + 3, z, "/DISC", 0);
    ui_drawSmallDigits(r->x + 0x56, r->y + 3, z, buf, 0);
    for (j = 0; j < 4; j++) {
        if (s->party[j].portrait != PORTRAIT_NONE) {
            func_80028204(r->x + 6 + j * 0x21, r->y + 0xD, z, s->party[j].portrait, 0x2C, 0);
        }
    }
    text_formatNumber(s->level, buf, 2, 1);
    text_drawBlock(r->x + 0x90, r->y + 6, z, s->name.v, 0);
    ui_drawSprite(r->x + 0x90, r->y + 0x17, z, 0xB4, 0);
    ui_drawDigits(r->x + 0xA2, r->y + 0x15, z, buf, 0);
    ui_drawSprite(r->x + 0xD8, r->y + 9, z, 0xB1, 0);
    ui_drawSprite(r->x + 0xD8, r->y + 0x16, z, 0xB2, 0);
    ui_drawSmallDigits(r->x + 0xE4, r->y + 0xA, z, buf, func_8002AA04(s->playTime, buf, 0));
    text_formatNumber(s->gil, buf, 7, 1);
    buf[7] = 0xA4;
    buf[8] = 0;
    ui_drawSmallDigits(r->x + 0xE4, r->y + 0x17, z, buf, 0);
    rr.x = r->x + 0x8E;
    rr.y = r->y + 0x25;
    rr.w = 0x9C;
    rr.h = 0x15;
    text_drawBlock(rr.x + 8, rr.y + 3, z, (u8 *)&s->location, 0);
    func_8002CD4C(&rr, z, 0, s->windowColor);
}

void menuSave_drawCursor(void) {
    u8 blink = (g_menu.unk10 & 3) != 0;

    if (!func_8002F810() && s_menuSave.phase == SAVE_PH_READY) {
        if (s_menuSave.state == SAVE_ST_SLOT) {
            D_80077DE8.unk0 = 0x74;
            D_80077DE8.unk4 = 0x32;
            D_80077DE8.unk2 = g_menu.unk2 * 0x12 + 0x6D;
            func_800355E4();
        } else if (s_menuSave.state == SAVE_ST_LIST || (s_menuSave.state == SAVE_ST_ACCESS && blink)) {
            D_80077DE8.unk0 = 0;
            D_80077DE8.unk4 = 0x2A;
            D_80077DE8.unk2 = s_menuSave.list.cursor * SAVE_ROW_H + 0x42;
            func_800355E4();
        } else if (s_menuSave.state == SAVE_ST_CONFIRM && blink) {
            ui_drawHand(0, s_menuSave.list.cursor * SAVE_ROW_H + 0x42, 0x2A);
        }
    }
}

void menuSave_updateFileList(void) {
    MenuList *l = &s_menuSave.list;
    s32 v;
    s32 dec;

    if (s_menuSave.cardStatus[g_menu.unk2] != CARD_RES_DONE) {
        menuSave_showNotice(SAVE_TEXT_NO_CARD);
        return;
    }
    if (l->unk8 != 0) {
        if (!(l->unk8 & 4)) {
            dec = l->unk8 & 2;
            l->unk9 += 6;
            if (l->unk9 >= SAVE_ROW_H) {
                v = l->unk5;
                if (dec) {
                    v--;
                } else {
                    v++;
                }
                l->unk5 = v;
                l->unk8 = 0;
                l->unk9 = 0;
            }
        } else {
            l->unk9 += 9;
            if (l->unk9 >= SAVE_LIST_H) {

                l->unk8 = 0;
                l->unk9 = 0;
                l->unk5 = l->unkC;
            }
        }
    }
}

void menuSave_updateHelp(void) {
    RECT r = s_menuSaveSlotHelpRect;
    u8 st;
    s32 v;

    D_80077DE8.unk8 = NULL;
    if (s_menuSave.phase == SAVE_PH_READY && s_menuSave.showCursor != 0) {
        func_80035580(NULL);
        st = s_menuSave.state;
        if (st == SAVE_ST_SLOT) {
            func_80035580(&r);
            v = SAVE_TEXT_SAVES_TO;
            /* st is SAVE_ST_SLOT == SAVE_MODE_LOAD here */
            if (s_menuSave.mode == st) {
                v = SAVE_TEXT_LOADS_FROM;
            }
            D_80077DE8.unk8 = text_get(v + g_menu.unk2);
        } else if (st == SAVE_ST_LIST) {
            menuSave_setFileHelp();
        } else if (st == SAVE_ST_CONFIRM) {
            D_80077DE8.unk8 = menuSave_getConfirmHelp();
        }
    }
}

void menuSave_setFileHelp(void) {
    MenuList *l;
    SaveMember *m;
    s32 idx;
    s32 n;
    s32 i;
    s32 st;

    l = &s_menuSave.list;
    idx = l->unk5 + l->cursor;
    st = s_menuSave.files[idx].state;
    switch (st) {
    case SAVE_FILE_EMPTY:
    case SAVE_FILE_OTHER:
    case SAVE_FILE_CORRUPT:
        D_80077DF0 = text_get(st + SAVE_TEXT_FILE_HELP);
        break;
    case SAVE_FILE_USED:
        s_menuSaveHelpBuf[0] = 0xFF;
        n = 0;
        m = s_menuSave.files[idx].party;
        for (i = 0; i < 4; i++) {
            if (m[i].present != 0) {
                n = i + 1;
            }
        }
        for (i = 0; i < n; i++) {
            if (m[i].present != 0) {
                func_8002BE44(i, m[i].name);
                func_8002BE5C(i, m[i].level);
                text_append(s_menuSaveHelpBuf, text_get(i + SAVE_TEXT_MEMBER1));
                if (i < n - 1) {
                    text_append(s_menuSaveHelpBuf, text_get(SAVE_TEXT_NEWLINE));
                }
            }
        }
        D_80077DF0 = s_menuSaveHelpBuf;
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

    func_8003563C(0);
    func_8003556C(NULL);
    menuSave_closeCardEvents();
    menuSave_restoreVram();
    menuSave_freeImage();
    for (i = 0; i < 2; i++) {
        D_80073998[i] = s_menuSave.unk18[i];
    }
}

u8 menuSave_isLoaded(void) {
    return s_menuSave.loaded;
}

u8 menuSave_isSlotReady(s32 port, u8 allowNew) {
    s32 st = s_menuSave.cardStatus[port];
    s32 r;

    if (allowNew) {
        r = 0;
        if (st == CARD_RES_DONE || st == CARD_RES_NEW) {
            r = 1;
        }
        return r;
    }
    return st == CARD_RES_DONE;
}

void menuSave_backToSlots(void) {
    func_8002F9F0();
    func_8002F8A8(menuSave_returnToSlots);
}

void menuSave_close(void) {
    func_8002F8E4(s_menuSave.savedFadeOtz);
    func_800325E0();
}

void menuSave_drawTitle(void) {
    func_8002DEE4(&s_menuSaveTitleRect, 5, 2);
    func_8002705C(&s_menuSaveTitleRect, 5, 2);
    text_drawBlock(s_menuSaveTitleRect.x + 0xA, s_menuSaveTitleRect.y + 6, 4, (u8 *)text_getFrom(g_saveMenuText, s_menuSave.mode), 0);
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
    if (!func_8002F810()) {
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
    MenuList *l = &s_menuSave.list;

    if (l->unk8 == 0) {
        if (g_padRepeat & BTN_CIRCLE) {
            snd_playSe(SE_CANCEL);
            menuSave_backToSlots();
            return;
        }
        if (g_padRepeat & BTN_CROSS) {
            menuSave_selectFile();
            return;
        }
        menuList_up(l, g_padRepeat);
        menuList_down(l, g_padRepeat);
        menuSave_pageUp(l, g_padRepeat);
        menuSave_pageDown(l, g_padRepeat);
    }
}

void menuSave_drawScrollRows(void) {
    MenuView v;
    RECT r;
    s32 i;

    menuSave_getScrollView(&s_menuSave.list, &v);
    r.x = SAVE_LIST_X;
    r.w = SAVE_LIST_W;
    r.y = 0;
    r.h = SAVE_ROW_H;
    for (i = 0; i < v.unk0; i++) {
        r.y = i * SAVE_ROW_H + (s16)(v.unk2 + SAVE_LIST_Y);
        menuSave_drawFile(&r, 0x2D, v.unk1 + i);
        func_8002DEE4(&r, 0x2D, 0);
    }
}

void menuSave_drawScrollArrows(void) {
    s32 blink = (((u32)g_menu.unk10 >> 4) ^ 1) & 1;
    MenuList *l = &s_menuSave.list;

    ui_drawSprite(0x12E, 0x2C, 0x2A, 0xB8, l->unk5 != 0 ? blink : 1);
    ui_drawSprite(0x12E, 0xCF, 0x2A, 0xB9, l->unk5 < l->unk2 - l->unk3 ? blink : 1);
}

void menuSave_drawEmptyFile(RECT *r, s32 otz) {
    text_drawBlock(r->x + 0x10, r->y + 0x16, otz, text_get(SAVE_TEXT_EMPTY_BLOCK), 0);
}

void menuSave_drawOtherFile(RECT *r, s32 otz) {
    text_drawBlock(r->x + 0x10, r->y + 0x16, otz, text_get(SAVE_TEXT_OTHER_GAME), 0);
}

void menuSave_drawCorruptFile(RECT *r, s32 otz) {
    text_drawBlock(r->x + 0x10, r->y + 0x16, otz, text_get(SAVE_TEXT_DATA_CORRUPTED), 0);
}

void menuSave_selectSlot(void) {
    if (!menuSave_isSlotReady_inline(g_menu.unk2, 1)) {
        snd_playSe(SE_BUZZER);
        menuSave_showNotice(SAVE_TEXT_NO_CARD);
        return;
    }
    snd_playSe(SE_CURSOR);
    menuSave_openCheck();
}

void menuSave_selectFile(void) {
    s32 i = s_menuSave.list.unk5 + s_menuSave.list.cursor;

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
    func_8003563C(hide);
}

void menuSave_returnToSlots(void) {
    s_menuSave.state = SAVE_ST_SLOT;
    s_menuSave.help = text_get(SAVE_TEXT_SELECT_SLOT);
    func_8002F9D0();
}

void func_801F5A60(void) {
}

void menuSave_drawBookIcon(s32 x, s32 y, s32 z, s32 idx) {
    u32 clut;
    Ovl39Sprite *e;

    clut = GetClut(0, BOOK_CLUT_Y);

    setlen(g_primPtr, 4);
    e = &s_menuSaveBookFrames[idx];
    ((SprtWords *)g_primPtr)->rgbc = 0x64808080;
    ((SprtWords *)g_primPtr)->xy = x | (y << 16);
    ((SprtWords *)g_primPtr)->uvclut = e->u | (e->v << 8) | (clut << 16);
    ((SprtWords *)g_primPtr)->wh = e->w | (e->h << 16);
    addPrim(&g_ot[z], g_primPtr);
    g_primPtr += sizeof(SprtWords);
    setlen(g_primPtr, 2);
    SetDrawTPage(g_primPtr, 0, 1, 0x91);
    addPrim(&g_ot[z], g_primPtr);
    g_primPtr += sizeof(DR_TPAGE);
}

void menuSave_saveVram(void) {
    RECT r;
    u8 *img = s_menuSaveBookImage;
    u8 *buf = s_menuSaveVramBackup;
    u8 *src = img;
    s32 i;

    r.y = BOOK_CLUT_Y;
    r.x = 0;
    r.w = 0x100;
    r.h = 1;
    StoreImage(&r, (u32 *)buf);
    DrawSync(0);
    LoadImage(&r, (u32 *)img);
    DrawSync(0);
    buf += BOOK_CLUT_SIZE;
    img += BOOK_CLUT_SIZE;
    r.x = BOOK_X;
    r.w = BOOK_W;
    r.y = BOOK_Y;
    r.h = BOOK_STRIP_H;
    /* swaps VRAM and image strip by strip: each strip's old VRAM goes over the strip
       uploaded before it, the first one into s_menuSaveVramBackup */
    for (i = 0; i < BOOK_STRIPS; i++) {
        if (i == 0) {
            StoreImage(&r, (u32 *)buf);
        } else {
            StoreImage(&r, (u32 *)src);
            src += BOOK_STRIP_SIZE;
        }
        DrawSync(0);
        LoadImage(&r, (u32 *)img);
        DrawSync(0);
        r.y += BOOK_STRIP_H;
        img += BOOK_STRIP_SIZE;
    }
}

void menuSave_restoreVram(void) {
    RECT r;
    u8 *img;
    u8 *buf;
    s32 i;

    img = s_menuSaveBookImage;
    buf = s_menuSaveVramBackup;
    r.x = 0;
    r.y = BOOK_CLUT_Y;
    r.w = 0x100;
    r.h = 1;
    LoadImage(&r, (u32 *)buf);
    DrawSync(0);
    buf += BOOK_CLUT_SIZE;
    r.x = BOOK_X;
    r.w = BOOK_W;
    r.y = BOOK_Y;
    r.h = BOOK_STRIP_H;
    for (i = 0; i < BOOK_STRIPS; i++) {
        if (i == 0) {
            LoadImage(&r, (u32 *)buf);
        } else {
            LoadImage(&r, (u32 *)img);
            img += BOOK_STRIP_SIZE;
        }
        DrawSync(0);
        r.y += BOOK_STRIP_H;
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

u8 menuSave_enableBookAnim(u8 v) {
    u8 old = s_menuSaveBookAnim.enabled;

    s_menuSaveBookAnim.enabled = v;
    return old;
}

void menuSave_drawBook(s32 x, s32 y, s32 otz) {
    menuSave_drawBookIcon(x, y, otz, s_menuSaveBookAnim.frame);
}

void menuSave_openConfirm(s32 id, s32 yes, s32 no) {
    RECT r;
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
    r.w = w;
    r.h = h;
    r.x = (g_gameState->unkC->disp.w - w) >> 1;
    r.y = (g_gameState->unkC->disp.h - h) >> 1;
    s_menuSaveConfirm.win = func_8002DC54(&r, 0x19, 2);
    win_setOnOpened(s_menuSaveConfirm.win, menuSave_onConfirmOpened);
    win_setOnClosed(s_menuSaveConfirm.win, menuSave_onConfirmClosed);
    win_setDrawContent(s_menuSaveConfirm.win, menuSave_drawConfirm);
    win_setDrawOverlay(s_menuSaveConfirm.win, menuSave_drawConfirmCursor);
    func_8002E31C(s_menuSaveConfirm.win, 1);
    func_8002E304(s_menuSaveConfirm.win, D_8007048C);
}

void menuSave_drawConfirm(Window *w) {
    u32 a = text_getInfo(SAVE_TEXT_YES);
    u32 b = text_getInfo(SAVE_TEXT_NO) & 0xFFF;
    s32 d1 = 0x26;
    s32 d2 = 0x17;
    s16 m;

    if (b < (a & 0xFFF)) {
        b = a & 0xFFF;
    }
    text_drawBlock(w->unk0 + 8, w->unk2 + 8, w->unk18, (u8 *)s_menuSaveConfirm.text, 0);
    m = b;
    text_drawBlock(w->unk0 + ((s32)(w->unk4 - m) >> 1), (w->unk2 - d1) + w->unk6, w->unk18, text_get(SAVE_TEXT_YES), 0);
    text_drawBlock(w->unk0 + ((s32)(w->unk4 - m) >> 1), (w->unk2 - d2) + w->unk6, w->unk18, text_get(SAVE_TEXT_NO), 0);
}

void menuSave_drawConfirmCursor(Window *w) {
    u32 a = text_getInfo(SAVE_TEXT_YES);
    u32 b = text_getInfo(SAVE_TEXT_NO) & 0xFFF;
    s32 z;

    if (b < (a & 0xFFF)) {
        b = a & 0xFFF;
    }
    if (w->unk1C == WIN_OPEN) {
        D_80077DE8.unk0 = w->unk0 + ((s32)(w->unk4 - b) >> 1) - 0x1A;
        D_80077DE8.unk2 = w->unk2 + (s16)(w->unk6 + (s16)(s_menuSaveConfirm.choice * 15 - 0x26)) + 3;
        z = w->unk18;
        D_80077DE8.unk4 = z;
        ui_drawHand(D_80077DE8.unk0, D_80077DE8.unk2, z - 1);
    }
}

void menuSave_initConfirm(void) {
    s_menuSaveConfirm.active = 0;
}

void menuSave_inputConfirm(void) {
    if (s_menuSaveConfirm.win->unk1C == WIN_OPEN) {
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
    u8 st = s_menuSave.cardStatus[g_menu.unk2];

    if (st != CARD_RES_DONE && st != CARD_RES_NEW) {
        win_setOnClosed(s_menuSaveConfirm.win, NULL);
        win_close(s_menuSaveConfirm.win);
        s_menuSaveConfirm.active = 0;
        s_menuSaveConfirm.win = NULL;
        menuSave_showNotice(SAVE_TEXT_NO_CARD);
    }
}

void menuSave_setConfirmCallback(void (*v)(SaveConfirm *)) {
    s_menuSaveConfirm.onYes = v;
}

u8 menuSave_isConfirmActive(void) {
    return s_menuSaveConfirm.active;
}

u8 menuSave_setConfirmActive(u8 v) {
    u8 old = s_menuSaveConfirm.active;

    s_menuSaveConfirm.active = v;
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
    s_menuSaveCheck.win = func_8002DC54(&s_menuSaveCheckRect, 0x1E, 2);
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
    func_8002E304(s_menuSaveCheck.win, D_80070420);
    func_8002E31C(s_menuSaveCheck.win, 1);
    sl = g_menu.unk2;
    switch (s_menuSave.cardStatus[sl]) {
    case CARD_RES_NEW:
        s_menuSaveCheck.step = CHECK_STEP_CLEAR;
        if (menuSave_cardClearSync(CARD_CHAN(g_menu.unk2)) != CARD_RES_DONE) {
            menuSave_checkError(SAVE_TEXT_CHECK_FAILED);
        }
        break;
    case CARD_RES_DONE:
        s_menuSaveCheck.step = CHECK_STEP_LOAD;
        if (menuSave_cardLoad(CARD_CHAN(g_menu.unk2), menuSave_onCardLoaded) == 0) {
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
    s32 r;

    r = menuSave_getCardResult(CARD_CHAN(g_menu.unk2));

    s_menuSaveCheck.tries++;
    s_menuSaveCheck.triesPerResult[r]++;
    if (s_menuSaveCheck.tries == s_menuSaveCheck.triesPerResult[r]) {
        if (s_menuSaveCheck.tries < s_menuSaveCheckRetries[r]) {
            menuSave_retryCheck(r);
            return;
        }
        switch (r) {
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
    DirEnt *d;
    SaveName20 *p;
    s32 i;
    s32 j;
    s32 k;
    s32 n;

    s_menuSaveCheck.dirCount = menuSave_countFiles();
    if (s_menuSaveCheck.dirCount < 0) {
        menuSave_checkError(SAVE_TEXT_CHECK_FAILED);
        return 0;
    }
    for (i = 0; i < s_menuSaveCheck.dirCount; i++) {
        d = &s_menuSaveCheck.dir[i];
        if (strncmp((char *)d, (char *)menuSave_getFilePrefix(), SAVE_PREFIX_LEN) == 0) {
            k = menuSave_parseFileNumber(d->name.v);
            p = &s_menuSave.files[k].fileName.name;
            *p = d->name;
            s_menuSave.files[k].fileName.term[0] = 0;
            s_menuSave.files[k].state = SAVE_FILE_USED;
            s_menuSaveCheck.ff9Files++;
        }
    }
    for (i = 0; i < s_menuSaveCheck.dirCount; i++) {
        d = &s_menuSaveCheck.dir[i];
        if (strncmp((char *)d, (char *)menuSave_getFilePrefix(), SAVE_PREFIX_LEN) != 0) {
            n = (s_menuSaveCheck.dir[i].size + CARD_BLOCK - 1) / CARD_BLOCK;
            for (j = 0; j < n; j++) {
                k = menuSave_findLastEmpty();
                p = &s_menuSave.files[k].fileName.name;
                *p = d->name;
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
    fd = menuSave_openFile(CARD_CHAN(g_menu.unk2), (char *)s_menuSave.files[file].fileName.name.v);
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
    switch (menuSave_getCardResult(CARD_CHAN(g_menu.unk2))) {
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
    func_8002DE10(s_menuSaveCheck.win);
    func_8002EDFC(&s_menuSave.list, SAVE_FILES, SAVE_LIST_ROWS, 1, SAVE_LIST_H);
    if (s_menuSave.files[s_menuSave.newestFile].state == SAVE_FILE_USED) {
        s_menuSave.list.cursor = s_menuSave.files[s_menuSave.newestFile].cursor;
        s_menuSave.list.unk5 = s_menuSave.newestFile - s_menuSave.list.cursor;
    }
    func_8002F9D0();
    func_8002F8A8(menuSave_onFileListFadedIn);
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

    if (!menuSave_isSlotReady(g_menu.unk2, 1)) {
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

u8 menuSave_setCheckActive(u8 v) {
    u8 old = s_menuSaveCheck.active;

    s_menuSaveCheck.active = v;
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

s32 menuSave_findUsed(s32 n) {
    s32 found = 0;
    s32 i;

    for (i = 0; i < SAVE_FILES; i++) {
        if (s_menuSave.files[i].state == SAVE_FILE_USED) {
            found++;
            if (n < found) {
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
        if (menuSave_cardLoad(CARD_CHAN(g_menu.unk2), menuSave_onCardLoaded) == 0) {
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
        if (menuSave_cardClearSync(CARD_CHAN(g_menu.unk2)) == CARD_RES_DONE) {
            return;
        }
    } else if (menuSave_cardLoad(CARD_CHAN(g_menu.unk2), menuSave_onCardLoaded) != 0) {
        return;
    }
    menuSave_checkError(SAVE_TEXT_CHECK_FAILED);
}

void menuSave_drawCheck(Window *w) {
    menuSave_drawBook(w->unk0 + 0xE, w->unk2 + 0x12, w->unk18);
    text_drawBlock(w->unk0 + 0x4A, w->unk2 + 0xE, w->unk18, text_get(SAVE_TEXT_DO_NOT_REMOVE), 0);
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
    s32 n;
    s32 prev;
    s32 r;

    prev = 0;
    n = 0;
    port = CARD_CHAN(g_menu.unk2);
    do {
        r = menuSave_readDirectory(port, (s32)s_menuSaveCheck.dir);
        if (n != 0 && prev != r) {
            goto fail;
        }
        n++;
        prev = r;
    } while (n < 3);
    if (menuSave_cardInfoSync(port) == CARD_RES_DONE) {
        return r;
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
        func_8002F9F0();
        func_8002F8A8(menuSave_enterFileList);
        snd_playSe(SE_SAVE_LIST);
    }
}

void menuSave_onFileListFadedIn(void) {
    if (s_menuSave.phase != SAVE_PH_MESSAGE) {
        s_menuSave.phase = SAVE_PH_READY;
    }
}

void menuSave_readHeader(s32 file) {
    s32 fd;

    s_menuSaveCheck.file = file;
    fd = menuSave_openFile(CARD_CHAN(g_menu.unk2), (char *)s_menuSave.files[file].fileName.name.v);
    s_menuSaveCheck.fd = fd;
    if (fd < 0) {
        menuSave_checkError(SAVE_TEXT_CHECK_FAILED);
        return;
    }
    menuSave_readAsync(fd, (s32)s_menuSaveCheck.buf, CHECK_HEADER_SIZE, menuSave_onHeaderRead);
}

void menuSave_copyHeader(SaveFile *s, SaveHeader *h) {
    s32 i;

    s->version = h->version;
    s->name = h->name;
    s->location = h->location;
    s->name.v[7] = 0xFF;
    s->discs = h->discs;
    s->level = h->level;
    s->playTime = h->playTime;
    s->gil = h->gil;
    s->windowColor = h->windowColor;
    s->counter = h->counter;
    s->cursor = h->cursor;
    for (i = 0; i < 4; i++) {
        s->party[i] = h->party[i];
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

void menuSave_openSavedMsg(SaveMenu *m) {
    if (s_menuSaveSavedMsg.active == 0) {
        s_menuSave.phase = SAVE_PH_WAIT;
        s_menuSaveSavedMsg.timer = 0;
        s_menuSaveSavedMsgRect.y = s_menuSave.list.cursor * SAVE_ROW_H + 0x3F;
        s_menuSaveSavedMsg.win = func_8002DC54(&s_menuSaveSavedMsgRect, 0x14, 2);
        win_setDrawContent(s_menuSaveSavedMsg.win, menuSave_drawSavedMsg);
        win_setOnOpened(s_menuSaveSavedMsg.win, menuSave_onSavedMsgOpened);
        win_setOnClosed(s_menuSaveSavedMsg.win, menuSave_onSavedMsgClosed);
        func_8002E31C(s_menuSaveSavedMsg.win, 1);
    }
}

void menuSave_inputSavedMsg(void) {
    if (s_menuSaveSavedMsg.win->unk1C == WIN_OPEN && (g_padRepeat & (BTN_CIRCLE | BTN_CROSS))) {
        snd_playSe(SE_CANCEL);
        s_menuSave.phase = SAVE_PH_WAIT;
        win_close(s_menuSaveSavedMsg.win);
    }
}

void menuSave_updateSavedMsg(void) {
    if (!menuSave_isSlotReady(g_menu.unk2, 0)) {
        func_8002DE10(s_menuSaveSavedMsg.win);
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
    u8 *s = text_get(SAVE_TEXT_DATA_SAVED);

    text_drawBlock(w->unk0 + ((w->unk4 - func_8002BA68((s32)s)) >> 1), w->unk2 + 8, w->unk18, s, 0);
}

void menuSave_openMessage(s32 id, s32 title) {
    SaveMessage *m = &s_menuSaveMessage;
    RECT r;
    s32 i;
    u16 sz;
    s32 w, h;

    if (m->active == 0) {
        snd_playSe(SE_SAVE_MESSAGE);
        s_menuSave.phase = SAVE_PH_WAIT;
        for (i = SAVE_FILES - 1; i >= 0; i--) {
            s_menuSave.files[i].state = SAVE_FILE_NONE;
        }
        m->active = 1;
        m->text = text_get(id);
        sz = text_getInfo(id);
        w = (sz & 0xFFF) + 0x10;
        m->textSize = sz;
        h = (sz >> 12) * 15 + 0x10;
        r.w = w;
        r.h = h;
        r.x = (g_gameState->unkC->disp.w - w) >> 1;
        r.y = (g_gameState->unkC->disp.h - h) >> 1;
        m->win = func_8002DC54(&r, 0xF, 2);
        win_setDrawContent(m->win, menuSave_drawMessage);
        win_setOnOpened(m->win, menuSave_onMessageOpened);
        win_setOnClosed(m->win, menuSave_onMessageClosed);
        func_8002E31C(m->win, 1);
        func_8002E304(m->win, title);
    }
}

void menuSave_initMessage(void) {
    s_menuSaveMessage.active = 0;
}

void menuSave_inputMessage(void) {
    if (s_menuSaveMessage.win->unk1C == WIN_OPEN && (g_padRepeat & (BTN_CIRCLE | BTN_CROSS))) {
        snd_playSe(SE_CANCEL);
        s_menuSave.phase = SAVE_PH_WAIT;
        win_close(s_menuSaveMessage.win);
    }
}

void menuSave_showError(s32 id) {
    menuSave_openMessage(id, D_80070440);
}

void menuSave_showNotice(s32 id) {
    menuSave_openMessage(id, D_8007048C);
}

u8 menuSave_setMessageActive(u8 v) {
    u8 old = s_menuSaveMessage.active;

    s_menuSaveMessage.active = v;
    return old;
}

u8 menuSave_isMessageActive(void) {
    return s_menuSaveMessage.active;
}

void menuSave_drawMessage(Window *w) {
    text_drawBlock(w->unk0 + 8, w->unk2 + 8, w->unk18, s_menuSaveMessage.text, 0);
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
        func_8002F9F0();
        func_8002F8A8(menuSave_resetToSlots);
        return;
    }
    s_menuSave.help = text_get(SAVE_TEXT_SELECT_SLOT);
    s_menuSave.state = SAVE_ST_SLOT;
}

void menuSave_resetToSlots(void) {
    s_menuSave.help = text_get(SAVE_TEXT_SELECT_SLOT);
    s_menuSave.state = SAVE_ST_SLOT;
    s_menuSave.phase = SAVE_PH_READY;
    func_8002F9D0();
    func_8002F8A8(menuSave_onSlotsFadedIn);
}

void menuSave_onSlotsFadedIn(void) {
    s_menuSave.phase = SAVE_PH_READY;
}

void menuSave_loadImage(void) {
    func_80028720(s_menuSaveImage, 10);
}

void menuSave_freeImage(void) {
    func_8002886C();
}

u8 menuSave_getPortrait(Character *c) {
    s32 m;

    if (c == NULL) {
        return PORTRAIT_NONE;
    }
    m = func_80028954(c->unk34.b.id);
    if (m == 0xFF) {
        return PORTRAIT_NONE;
    }
    if (m == 2 && (s32)(c->unk34.b.unk4) >= 5) {
        return 3;
    }
    return s_menuSavePortraitIds[m];
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
    s_menuSaveAccess.win = func_8002DC54(&s_menuSaveAccessRect, 0x1E, 2);
    win_setDrawContent(s_menuSaveAccess.win, menuSave_drawAccess);
    win_setOnOpened(s_menuSaveAccess.win, menuSave_onAccessOpened);
    win_setOnClosed(s_menuSaveAccess.win, menuSave_onAccessClosed);
    func_8002E31C(s_menuSaveAccess.win, 1);
    func_8002E304(s_menuSaveAccess.win, D_800703FC.unk0[s_menuSaveAccessTitles[mode]]);
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
    SaveAccess *p;
    SaveMenu *m;
    s32 t;
    s32 stage;

    switch (s_menuSaveAccess.stage) {
    case ACCESS_STAGE_OPEN:
        p = &s_menuSaveAccess;
        if (++p->delay >= ACCESS_TICKS) {
            m = &s_menuSave;
            p->delay = 0;
            if (m->formatted != 0) {
                t = SAVE_TEXT_SAVING;
                stage = ACCESS_STAGE_IO;
            } else {
                t = SAVE_TEXT_FORMATTING;
                stage = ACCESS_STAGE_FORMAT;
                m->showCursor = 0;
            }
            p->stage = stage;
            m->help = text_get(t);
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
    RECT *r;
    s32 st;
    s32 n;

    r = (RECT *)w;
    if (s_menuSave.formatted != 0) {
        if (s_menuSave.mode != SAVE_MODE_LOAD) {
            st = menuSave_getWriteProgress();
        } else {
            st = menuSave_getReadProgress();
        }
        s_menuSaveAccess.progressTicks = st == s_menuSaveAccess.lastProgress ? s_menuSaveAccess.progressTicks + 1 : 0;
        n = s_menuSaveAccess.progressTicks * 2;
        if (n >= 6) {
            n = 5;
        }
        menuSave_drawBook(r->x + 0xE, r->y + 0x12, w->unk18);
        text_drawBlock(r->x + 0x4A, r->y + 0xE, w->unk18, text_get(SAVE_TEXT_DO_NOT_REMOVE), 0);
        menuSave_drawGauge(r->x + 0x1A, r->y + 0x44, w->unk18, st * ACCESS_GAUGE_STEP + n, ACCESS_GAUGE_W);
        s_menuSaveAccess.lastProgress = st;
        return;
    }
    text_drawBlock(r->x + 0x26, r->y + 0xC, w->unk18, text_get(SAVE_TEXT_FORMATTING_CARD), 0);
    text_drawBlock(r->x + 0x32, r->y + 0x20, w->unk18, text_get(SAVE_TEXT_DO_NOT_REMOVE), 0);
}

u8 menuSave_writeFile(void) {
    SaveFile *sl;
    s32 port;
    s32 fd;

    port = CARD_CHAN(g_menu.unk2);
    sl = &s_menuSave.files[s_menuSave.file];
    /* not SAVE_FILE_CORRUPT or SAVE_FILE_USED: create a new file */
    if ((u32)(sl->state - SAVE_FILE_CORRUPT) >= 2) {
        strcpy((char *)s_menuSaveAccess.fileName.name.v, menuSave_getFilePrefix());
        strcat(s_menuSaveAccess.fileName.name.v, "00000-");
        s_menuSaveAccess.fileName.name.v[SAVE_NAME_NUMBER] = s_menuSave.file / 10 + '0';
        s_menuSaveAccess.fileName.name.v[SAVE_NAME_NUMBER + 1] = s_menuSave.file % 10 + '0';
        /* called before its definition without a prototype: int result */
        if (!((s32 (*)(s32, char *))menuSave_createFile)(port, (char *)s_menuSaveAccess.fileName.name.v)) {
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
            l->rgbc = s_menuSaveGaugeColors[i];
            l->xy0 = (x + 4) | yy;
            l->xy1 = (pos + x + 3) | yy;
            addPrim(&g_ot[otz], l);
            g_primPtr = (u8 *)(l + 1);
        }
    }
    func_80027710(x, y, otz, ic, 0);
    func_80027710(x + width - ec->unk4, y, otz, ec, 0);
    SetTexWindow(g_primPtr, &s_menuSaveGaugeTexWindow);
    addPrim(&g_ot[otz], g_primPtr);
    g_primPtr += 0xC;
    setlen(g_primPtr, 4);
    p = (SprtWords *)g_primPtr;
    p->rgbc = 0x64808080;
    p->xy = ((x + ic->unk4) & 0xFFFF) | (y << 16);
    p->uvclut = mid->unk2 << 16;
    p->wh = (width - (ic->unk4 + ec->unk4)) | (mid->unk5 << 16);
    addPrim(&g_ot[otz], p);
    g_primPtr = (u8 *)(p + 1);
    r.x = mid->unk0;
    r.y = mid->unk1;
    r.w = mid->unk4;
    r.h = mid->unk5;
    SetTexWindow(g_primPtr, &r);
    addPrim(&g_ot[otz], g_primPtr);
    g_primPtr += 0xC;
    SetDrawTPage(g_primPtr, 1, 1, 0x1F);
    addPrim(&g_ot[otz], g_primPtr);
    g_primPtr += 8;
}

void menuSave_initAccess(void) {
    s_menuSaveAccess.active = 0;
}

void menuSave_updateAccess(void) {
    if (!menuSave_isSlotReady(g_menu.unk2, 0)) {
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

u8 menuSave_setAccessActive(u8 v) {
    u8 old = s_menuSaveAccess.active;

    s_menuSaveAccess.active = v;
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

    port = CARD_CHAN(g_menu.unk2);
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
    if (menuSave_cardFormat(CARD_CHAN(g_menu.unk2)) == 0) {
        menuSave_abortAccess(SAVE_TEXT_FORMAT_FAILED);
        return 0;
    }
    if (menuSave_cardLoad(CARD_CHAN(g_menu.unk2), menuSave_onFormatDone) != 0) {
        return 1;
    }
    menuSave_abortAccess(SAVE_TEXT_SAVE_FAILED);
    return 0;
}

void menuSave_onFileIo(void) {
    s32 n = menuSave_getCardResult(CARD_CHAN(g_menu.unk2));

    if (menuSave_closeFile(s_menuSaveAccess.fd) < 0) {
        menuSave_abortAccess(s_menuSave.mode == SAVE_MODE_LOAD ? SAVE_TEXT_LOAD_FAILED : SAVE_TEXT_SAVE_FAILED);
        return;
    }
    if (n == CARD_RES_DONE) {
        menuSave_onFileIoDone();
        return;
    }
    menuSave_onFileIoFailed(n);
}

void menuSave_onFileIoDone(void) {
    u8 mode;

    win_close(s_menuSaveAccess.win);
    s_menuSave.phase = SAVE_PH_WAIT;
    mode = s_menuSave.mode;
    if (mode != SAVE_MODE_LOAD) {
        menuSave_storeSavedFile(s_menuSave.file, &SAVE_BUF->hdr);
        snd_playSe(SE_SAVE_DONE);
        return;
    }
    if (menuSave_applyLoaded()) {
        s_menuSave.loaded = mode;
        snd_playSe(SE_SAVE_DONE);
        if (g_menu.unk1 == 0) {
            func_800361CC(0);
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
    SaveFile *s;
    SaveFileName *n;
    s32 i;

    s = &s_menuSave.files[file];
    n = &s_menuSaveAccess.fileName;
    s->fileName = *n;
    s->state = SAVE_FILE_USED;

    s->discs = h->discs;
    s->level = h->level;
    s->name = h->name;
    s->location = h->location;
    s->playTime = h->playTime;
    s->gil = h->gil;
    s->windowColor = h->windowColor;
    s->counter = h->counter;
    s->cursor = h->cursor;
    for (i = 0; i < 4; i++) {
        s->party[i] = h->party[i];
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
    s32 r = menuSave_getCardResult();

    s_menuSave.showCursor = 1;
    if (r == CARD_RES_DONE) {
        snd_playSe(SE_SAVE_LIST);
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

MenuView *menuSave_getListView(MenuList *l, MenuView *v) {
    s32 d;
    u8 f;

    f = l->unk8;
    if (f & 4) {
        if (f & 2) {
            d = l->unkD - l->unk9;
            v->unk0 = (d + SAVE_ROW_H - 1) / SAVE_ROW_H;
            v->unk1 = l->unk5 + (l->unk3 - v->unk0);
            v->unk2 = (l->unk3 - v->unk0) * SAVE_ROW_H;
        } else {
            d = l->unkD - l->unk9;
            v->unk0 = (d + SAVE_ROW_H - 1) / SAVE_ROW_H;
            v->unk1 = l->unk5;
            v->unk2 = 0;
        }
    } else if (f & 2) {
        v->unk0 = l->unk3 + 1;
        v->unk1 = l->unk5 - 1;
        v->unk2 = l->unk9 - SAVE_ROW_H;
    } else if (f & 1) {
        v->unk1 = l->unk5;
        v->unk0 = l->unk3 + 1;
        v->unk2 = -l->unk9;
    } else {
        v->unk1 = l->unk5;
        v->unk0 = l->unk3;
        v->unk2 = 0;
    }
    return v;
}

MenuView *menuSave_getScrollView(MenuList *l, MenuView *v) {
    s32 x;

    if (l->unk8 & 2) {
        v->unk0 = (l->unk9 + SAVE_ROW_H - 1) / SAVE_ROW_H;
        v->unk1 = l->unkC + (l->unk3 - v->unk0);
        v->unk2 = l->unk9 - v->unk0 * SAVE_ROW_H;
    } else {
        x = l->unk9;
        v->unk2 = l->unkD - x;
        v->unk0 = (x + SAVE_ROW_H - 1) / SAVE_ROW_H;
        v->unk1 = l->unkC;
    }
    return v;
}

void menuSave_getScrollClip(MenuList *m, RECT *src, RECT *dst) {
    dst->x = src->x;
    dst->w = src->w;
    if (m->unk8 & 2) {
        dst->y = src->y + m->unk9;
    } else {
        dst->y = src->y;
    }
    dst->h = src->h - m->unk9;
}

void menuSave_pageUp(MenuList *l, s32 btn) {
    s32 top;
    s32 d;

    if ((btn & BTN_L1) && !(g_padHeld & BTN_R1) && l->unk5 != 0) {
        d = l->unkD;
        l->unk9 = d >> 2;
        l->unk8 = 7;
        l->unkE = (d - l->unk9 + 0xE) / 15;
        top = l->unk5 - l->unk3;
        if (top < 0) {
            top = 0;
        }
        l->unkC = top;
        snd_playSe(SE_CURSOR);
    }
}

void menuSave_pageDown(MenuList *l, s32 pad) {
    s32 h;
    s32 page;
    s32 top;
    s32 max;

    if ((pad & BTN_R1) && l->unk5 < l->unk2 - l->unk3) {
        h = l->unkD;
        l->unk9 = h >> 2;
        page = l->unk3;
        max = l->unk2 - page;
        l->unk8 = 5;
        top = l->unk5 + page;
        l->unkE = (h - l->unk9 + 0xE) / 15;
        if (top < max) {
            max = top;
        }
        l->unkC = max;
        snd_playSe(SE_CURSOR);
    }
}

void menuSave_buildHeader(SaveBuf *buf) {
    SaveHeader *h;
    Unk8004AB64 *o;
    s32 t;
    s32 i;
    u8 n;

    h = &buf->hdr;
    t = game_getPlayTime();
    menuSave_buildCardHeader(&buf->card, t);
    n = 0;
    o = func_8004AE00();
    if (o != NULL) {
        n = o->unk3B;
    }
    if (s_menuSave.mode == SAVE_MODE_NEXT_DISC) {
        h->discs = func_800B5074();
    } else {
        h->discs = g_gameState->unkB;
    }
    h->discs = menuSave_getDiscBit();
    h->version = SAVE_VERSION;
    h->level = g_gameState->unk1C->characters[n].level;
    h->playTime = t;
    h->gil = g_gameState->unk1C->gil;
    h->windowColor = (g_gameState->unk24 >> CFG_BIT_WINDOW_COLOR) & 1;
    h->counter = ++s_menuSave.newestCounter;
    h->cursor = s_menuSave.list.cursor;
    h->name = *(SaveName *)g_gameState->unk1C->characters[n].unk0;
    menuSave_getLocation((u8 *)&h->location);
    for (i = 0; i < 4; i++) {
        if (g_gameState->unk1C->party[i] != NULL) {
            h->party[i].present = 1;
            h->party[i].portrait = menuSave_getPortrait(g_gameState->unk1C->party[i]);
            h->party[i].level = g_gameState->unk1C->party[i]->level;
            *(Name10 *)h->party[i].name = *(Name10 *)g_gameState->unk1C->party[i]->unk0;
        } else {
            h->party[i].present = 0;
            h->party[i].portrait = PORTRAIT_NONE;
            h->party[i].level = 0;
            memset(h->party[i].name, 0xFF, 10);
        }
    }
}

void menuSave_getLocation(u8 *dst) {
    u8 *name;
    u8 *s;

    if (s_menuSave.mode == SAVE_MODE_NEXT_DISC) {
        name = text_get(func_800B5064() + SAVE_TEXT_DISC_START);
    } else {
        name = g_gameState->unk1C->unk80C;
    }
    if (name == NULL) {
        s = text_get(SAVE_TEXT_NO_LOCATION);
        memset(dst, 0xFF, sizeof(SaveLocation));
        memcpy(dst, s, func_8002BCC0(s));
    } else {
        memset(s_menuSaveLocationBuf, 0xFF, sizeof(s_menuSaveLocationBuf));
        memcpy(s_menuSaveLocationBuf, name, func_8002BCC0(name));
        s_menuSaveLocationBuf[sizeof(SaveLocation) - 1] = 0xFF; /* cut to fit */
        memset(dst, 0xFF, sizeof(SaveLocation));
        memcpy(dst, s_menuSaveLocationBuf, func_8002BCC0(s_menuSaveLocationBuf));
    }
}

void menuSave_buildCardHeader(CardFileHeader *buf, s32 t) {
    u8 *title;
    IconTmpl *icon;
    s32 sec;
    s32 min;
    s32 hour;
    s32 n;

    buf->magic = CARD_MAGIC;
    buf->iconFlag = CARD_ICON_1FRAME;
    buf->blocks = 1;
    title = buf->title;
    memset(buf->pad44, 0, sizeof(buf->pad44));
    sec = (u32)t / 60;
    min = sec / 60;
    hour = min / 60;
    min = min % 60;
    sec = sec % 60;
    if (hour >= 100) {
        hour %= 100;
    }
    memset(title, 0, sizeof(buf->title));
    /* "ＦＦ９／ＦＩＬＥ００／００：００／" in 2-byte SJIS: bump the low bytes of the digits,
       file number at 0x11/0x13, hours at 0x17/0x19, minutes at 0x1D/0x1F; 0x20 cuts the last "／" */
    *(TitleTmpl *)title = s_menuSaveTitleTemplate;
    title[0x11] += (s_menuSave.file + 1) / 10;
    n = s_menuSave.file + 1;
    title[0x17] += hour / 10;
    title[0x20] = 0;
    title[0x19] += hour % 10;
    title[0x1D] += min / 10;
    title[0x1F] += min % 10;
    title[0x13] += n % 10;
    if (sec == 0) {
        if (min == 0) {
            icon = &s_menuSaveIconHour;
        } else {
            icon = &s_menuSaveIconMinute;
        }
    } else if (t & 1) {
        icon = &s_menuSaveIconOdd;
    } else {
        icon = &s_menuSaveIconEven;
    }
    buf->icon = *icon;
}

void menuSave_saveFile(s32 file, s32 unused) {
    s_menuSave.file = file;
    if (s_menuSave.formatted == 0) {
        snd_playSe(SE_CURSOR);
        menuSave_setConfirmCallback(menuSave_startSave);
        menuSave_openConfirm(g_menu.unk2 + SAVE_TEXT_UNFORMATTED, SAVE_TEXT_FORMAT_YES, SAVE_TEXT_FORMAT_NO);
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

void menuSave_packData(SaveData *d) {
    menuSave_packState(&d->state);
    menuSave_packEvents(&d->events);
    menuSave_packParty(&d->party);
    menuSave_packMisc(&d->misc);
    menuSave_packItems(&d->items);
    menuSave_packCards(d->cards);
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
    u16 c;

    for (i = 0; i < 256; i++) {
        c = i;
        for (j = 0; j < 8; j++) {
            if (c & 1) {
                c = (c >> 1) ^ CRC16_POLY;
            } else {
                c >>= 1;
            }
        }
        s_menuSaveCrcTable[i] = c;
    }
}

u16 menuSave_crc16(u8 *p, s32 n) {
    Crc16 c = { CRC16_INIT };

    while (--n >= 0) {
        c.w = s_menuSaveCrcTable[(c.w ^ *p++) & 0xFF] ^ (c.h >> 8);
    }
    return c.w;
}

void menuSave_loadFile(s32 file, s32 arg1) {
    SaveFile *s;

    s = s_menuSave.files;
    s_menuSave.file = file;
    switch (s[file].state) {
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
    u8 ret;

    ret = 1;
    menuSave_initCrcTable();
    if (SAVE_BUF->crc == menuSave_crc16((u8 *)SAVE_BUF, SAVE_CRC_LEN)) {
        h = &SAVE_BUF->hdr;
        g_gameState->unkB = h->discs;
        game_setPlayTime(h->playTime);
        menuSave_unpackState(&SAVE_BUF->data.state);
        menuSave_unpackEvents(&SAVE_BUF->data.events);
        menuSave_unpackParty(&SAVE_BUF->data.party);
        menuSave_unpackMisc(&SAVE_BUF->data.misc);
        menuSave_unpackItems(&SAVE_BUF->data.items);
        menuSave_unpackCards(SAVE_BUF->data.cards);
    } else {
        menuSave_showError(SAVE_TEXT_CORRUPTED);
        ret = 0;
    }
    return ret;
}

void menuSave_startLoad(void) {
    if (!menuSave_startAccess(ACCESS_LOAD)) {
        menuSave_showError(SAVE_TEXT_LOAD_FAILED);
    }
}


void menuSave_packParty(SaveParty *s) {
    GameData *gd;
    s32 i;

    gd = g_gameState->unk1C;
    for (i = 0; i < SAVE_CHARACTERS; i++) {
        s->characters[i] = gd->characters[i];
    }
    for (i = 0; i < 4; i++) {
        if (gd->party[i] != NULL) {
            s->party[i] = gd->party[i]->unk34.b.id;
        } else {
            s->party[i] = SAVE_CHARACTERS;
        }
    }
    s->unk514 = gd->unk55C;
    s->firstUseFxSeen = gd->firstUseFxSeen;
    s->gil = gd->gil;
    s->unk51C = gd->unk568;
    s->unk51E = gd->unk56A;
    s->unk520 = gd->unk56C;
}

void menuSave_unpackParty(SaveParty *s) {
    GameData *gd;
    s32 i;

    gd = g_gameState->unk1C;
    for (i = 0; i < SAVE_CHARACTERS; i++) {
        gd->characters[i] = s->characters[i];
    }
    for (i = 0; i < 4; i++) {
        if (s->party[i] < SAVE_CHARACTERS) {
            gd->party[i] = &gd->characters[s->party[i]];
        } else {
            gd->party[i] = NULL;
        }
    }
    gd->unk55C = s->unk514;
    gd->firstUseFxSeen = s->firstUseFxSeen;
    gd->gil = s->gil;
    gd->unk568 = s->unk51C;
    gd->unk56A = s->unk51E;
    gd->unk56C = s->unk520;
}

void menuSave_packState(SaveState *h) {
    GameData *gd;
    s32 i;

    gd = g_gameState->unk1C;
    h->mode = g_gameState->mode;
    h->unk1 = g_gameState->unkA;
    for (i = sizeof(h->reserved2) - 1; i >= 0; i--) {
        h->reserved2[i] = 0;
    }
    if (s_menuSave.mode == SAVE_MODE_NEXT_DISC) {
        h->fieldId = func_800B5084();
        h->unk22 = func_800B5094();
    } else {
        h->fieldId = gd->fieldId;
        h->unk22 = gd->unk3A;
    }
    h->battleId = gd->battleId;
    h->unk26 = (s8)gd->unk848;
    h->unk28 = gd->unk7DC;
    h->unk2A = gd->unk7DE;
    for (i = sizeof(h->reserved2C) - 1; i >= 0; i--) {
        h->reserved2C[i] = 0;
    }
}

void menuSave_unpackState(SaveState *p) {
    GameState *gs = g_gameState;
    GameData *gd;

    gs->mode = p->mode;
    gd = gs->unk1C;
    g_gameState->unkA = p->unk1;
    gd->fieldId = p->fieldId;
    gd->unk3A = p->unk22;
    gd->battleId = p->battleId;
    gd->unk848 = p->unk26;
    gd->unk7DC = p->unk28;
    gd->unk7DE = p->unk2A;
}

void menuSave_copyCards(CardData *d, CardData *s) {
    s32 i;

    d->unk0 = s->unk0;
    d->unk2 = s->unk2;
    d->unk4 = s->unk4;
    for (i = 0; i < 0x69; i++) {
        d->unk6[i].unk0 = s->unk6[i].unk0;
        d->unk6[i].unk1 = s->unk6[i].unk1;
        d->unk6[i].unk2 = s->unk6[i].unk2;
        d->unk6[i].unk3 = s->unk6[i].unk3;
        d->unk6[i].unk4 = s->unk6[i].unk4;
        d->unk6[i].unk5 = s->unk6[i].unk5;
    }
    d->unk27C = s->unk27C;
    d->unk27E = s->unk27E;
}

void menuSave_packCards(void *p) {
    menuSave_copyCards(p, func_80062824());
}

void menuSave_unpackCards(void *p) {
    menuSave_copyCards(func_80062824(), p);
}

void menuSave_packEvents(SaveEvents *d) {
    d->vars = g_evtGlobalVars;
    d->unk800 = D_8007AEC4;
}

void menuSave_unpackEvents(SaveEvents *s) {
    g_evtGlobalVars = s->vars;
    D_8007AEC4 = s->unk800;
}

void menuSave_packItems(SaveItems *s) {
    s->items = g_gameState->unk1C->items;
    s->flags = g_gameState->unk1C->unk774;
    s->unk240 = *(GameStateUnk24 *)&g_gameState->unk24;
}

void menuSave_unpackItems(SaveItems *s) {
    g_gameState->unk1C->items = s->items;
    g_gameState->unk1C->unk774 = s->flags;
    *(GameStateUnk24 *)&g_gameState->unk24 = s->unk240;
    func_8002397C();
}

void menuSave_packMisc(SaveMisc *p) {
    p->unk0 = g_gameState->unk1C->unk844;
    p->unk4 = g_gameState->unk1C->unk880;
    p->unk14 = g_gameState->unk1C->unk892;
}

void menuSave_unpackMisc(SaveMisc *p) {
    g_gameState->unk1C->unk844 = p->unk0;
    g_gameState->unk1C->unk880 = p->unk4;
    g_gameState->unk1C->unk892 = p->unk14;
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
    s32 n = 0;

    if (menuSave_firstFile(chan, dir) == dir) {
        do {
            n++;
            dir += sizeof(DirEnt);
        } while (menuSave_nextFile(dir) == dir);
    }
    return n;
}

s32 menuSave_getCardStatus(s32 chan) {
    if (chan == 0) {

        return s_menuSaveCardStatus[0];
    }
    return s_menuSaveCardStatus[1];
}

s32 menuSave_cardInfoSync(s32 chan) {
    s32 r;

    menuSave_clearSwCard();
    _card_wait(0);
    _card_wait(1);
    _card_info(chan);
    while ((r = menuSave_testSwCard()) == -1) {
    }
    return r;
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
    s32 r;

    _card_wait(0);
    _card_wait(1);
    menuSave_clearHwCard();
    _card_clear(chan);
    do {
        r = menuSave_testHwCard();
    } while (r == -1);
    menuSave_cardIdle();
    return r;
}

s32 menuSave_cardLoad(s32 chan, void (*cb)(void)) {
    s32 r = menuSave_cardLoadStart(chan);

    s_menuSaveCardCallback = cb;
    return r;
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
    s32 r = menuSave_writeStart(fd, buf, size);

    s_menuSaveCardCallback = cb;
    return r;
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
    s32 r = menuSave_readStart(fd, buf, size);

    s_menuSaveCardCallback = cb;
    return r;
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

u8 menuSave_createFile(s32 chan, char *name) {
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
    s32 r;

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
        r = menuSave_testSwCard();
        switch (r) {
        case CARD_RES_DONE:
            menuSave_cardPoll_set(s_menuSaveCardChan, CARD_RES_DONE);
            break;
        case CARD_RES_ERROR:
        case CARD_RES_NEW:
            menuSave_cardPoll_set(s_menuSaveCardChan, r);
            break;
        case CARD_RES_TIMEOUT:
            menuSave_cardPoll_retry(s_menuSaveCardChan, r);
            break;
        }
        if (r != -1) {
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
        r = menuSave_testSwCard();
        s_menuSaveCardResult = r;
        if (r == -1) {
            break;
        }
        goto callback;
    case CARD_OP_READ:
        r = menuSave_testSwCard();
        s_menuSaveCardResult = r;
        if (r == -1) {
            break;
        }
        if (r != CARD_RES_DONE) {
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
        r = menuSave_testSwCard();
        s_menuSaveCardResult = r;
        if (r == -1) {
            break;
        }
        if (r != CARD_RES_DONE) {
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
    s32 r = -1;

    if (TestEvent(s_menuSaveSwEventIoe) == 1) {
        r = CARD_RES_DONE;
    } else if (TestEvent(s_menuSaveSwEventError) == 1) {
        r = CARD_RES_ERROR;
    } else if (TestEvent(s_menuSaveSwEventTimeout) == 1) {
        r = CARD_RES_TIMEOUT;
    } else if (TestEvent(s_menuSaveSwEventNew) == 1) {
        r = CARD_RES_NEW;
    }
    if (r != -1) {
        menuSave_cardIdle();
    }
    return r;
}

s32 menuSave_testHwCard(void) {
    s32 r = -1;

    if (TestEvent(s_menuSaveHwEventIoe) == 1) {
        r = CARD_RES_DONE;
    } else if (TestEvent(s_menuSaveHwEventError) == 1) {
        r = CARD_RES_ERROR;
    } else if (TestEvent(s_menuSaveHwEventTimeout) == 1) {
        r = CARD_RES_TIMEOUT;
    } else if (TestEvent(s_menuSaveHwEventNew) == 1) {
        r = CARD_RES_NEW;
    }
    if (r != -1) {
        menuSave_cardIdle();
    }
    return r;
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
