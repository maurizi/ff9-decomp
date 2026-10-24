#include "ovl_39.h"

void menu_save_init(void) {
    s32 i;

    g_menuSave.mode = g_menu.unkC;
    g_menuSave.unk14 = func_8002F8E4(7);
    g_menuSave.state = SAVE_ST_PORT;
    g_menuSave.phase = SAVE_PH_READY;
    g_menuSave.loaded = 0;
    g_menuSave.showCursor = 1;
    g_menuSave.newestCounter = 0;
    g_menuSave.newestFile = 0;
    g_menu.unk2 &= 1;
    menu_save_initScan();
    menu_save_initMessage();
    menu_save_initSavedMsg();
    menu_save_initConfirm();
    menu_save_initAccess();
    menu_save_loadImage();
    menu_save_loadCardIconImage();
    func_8003556C(menu_save_setHelp);
    text_setBank(g_saveMenuText);
    g_menuSave.help = text_get(SAVE_TEXT_SELECT_SLOT);
    g_menuSave.cardStatus[0] = CARD_RES_ERROR;
    g_menuSave.cardStatus[1] = CARD_RES_ERROR;
    menu_save_cardInit();
    menu_save_openCardEvents();
    func_801F5A60();
    for (i = 0; i < 2; i++) {
        g_menuSave.unk18[i] = D_80073998[i];
        D_80073998[i] |= 0x300;
    }
}

void menu_save_drawHelp(void) {
    u8 *s = text_get(g_menu.unk2 + SAVE_TEXT_SLOT1);
    s32 w = func_8002BA68((s32)s);

    text_drawBlock(g_menuSavePortNameRect.x + ((g_menuSavePortNameRect.w - w) >> 1), g_menuSavePortNameRect.y + 2, 5, s, (menu_save_isPortReady(g_menu.unk2, 1) == 0) * 2);
    if (g_menuSave.help != NULL) {
        text_drawBlock(g_menuSaveHelpRect.x + 0xA, g_menuSaveHelpRect.y + 6, 5, g_menuSave.help, 0);
    }
    ui_drawLabel(g_menuSaveHelpRect.x + 6, g_menuSaveHelpRect.y - 3, 5, D_80070450, 0);
    func_8002DEE4(&g_menuSavePortNameRect, 5, 0);
    func_8002DEE4(&g_menuSaveHelpRect, 5, 1);
    func_8002705C(&g_menuSaveHelpRect, 5, 1);
}

void menu_save_portInput(void) {
    if (g_padRepeat & BTN_CIRCLE) {
        snd_playSe(SE_CANCEL);
        if (g_menu.unk1 != 0) {
            menu_save_close();
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
        menu_save_selectPort();
    }
}

void menu_save_drawPorts(void) {
    s32 i;
    s32 ok;
    u8 *str;
    u8 s;

    func_8002DEE4(&g_menuSavePortListRect, 0x34, 2);
    func_8002705C(&g_menuSavePortListRect, 0x34, 2);
    for (i = 0; i < 2; i++) {
        str = text_get(i + SAVE_TEXT_SLOT1);
        ok = 0;
        s = g_menuSave.cardStatus[i];
        if (s == CARD_RES_DONE || s == CARD_RES_NEW) {
            ok = 1;
        }
        text_drawBlock(g_menuSavePortListRect.x + 10, g_menuSavePortListRect.y + 10 + i * 18, 0x33, str, (ok ^ 1) * 2);
    }
}

void menu_save_drawFileList(void) {
    MenuView v;
    RECT r;
    RECT a, b;
    s32 i;
    MenuList *l = &g_menuSave.list;

    menu_save_drawScrollArrows();
    ui_drawLabel(0xE, 0x23, 0x2C, D_80070444, 0);
    ui_drawRule(8, 0x26, 0x2C, 1, 0x12C, (g_gameState->unk24 >> 18) & 1);
    r.x = SAVE_LIST_X;
    r.y = 0;
    r.w = SAVE_LIST_W;
    r.h = SAVE_ROW_H;
    menu_save_listGetView(l, &v);
    win_setClip(NULL, 0x2D, 4);
    for (i = 0; i < v.unk0; i++) {
        r.y = i * SAVE_ROW_H + (s16)(v.unk2 + SAVE_LIST_Y);
        menu_save_drawFile(&r, 0x2D, v.unk1 + i);
    }
    if (l->unk8 & 4) {
        a.x = SAVE_LIST_X;
        a.y = SAVE_LIST_Y;
        a.w = SAVE_LIST_W;
        a.h = SAVE_LIST_H;
        menu_save_listPageClip(l, &a, &b);
        win_setClip(&b, 0x2D, 4);
        menu_save_drawPageScroll();
    }
    r.y = SAVE_LIST_Y;
    r.h = SAVE_LIST_H;
    win_setClip(&r, 0x2D, 4);
}

void menu_save_drawFile(RECT *r, s32 otz, s32 file) {
    char name[8];
    void (*fn)(RECT *, s32, s32);
    s32 v;

    strcpy(name, "File00");
    name[4] = (file + 1) / 10 + '0';
    name[5] = (file + 1) % 10 + '0';
    ui_drawSmallDigits(r->x + 6, r->y + 3, otz, (u8 *)name, 0);
    fn = g_menuSaveFileDrawers[g_menuSave.files[file].state];
    if (fn != NULL) {
        fn(r, otz, file);
    }
    if (g_menuSave.files[file].state == SAVE_FILE_USED) {
        v = g_menuSave.files[file].unk48;
    } else {
        v = g_gameState->unk24 >> 18;
        v &= 1;
    }
    func_8002CD4C(r, 0x2D, 0, v);
}

void menu_save_drawFileInfo(RECT *r, s32 z, s32 i) {
    u8 buf[16];
    RECT rr;
    SaveFile *s;
    s32 j;
    s32 n;

    s = &g_menuSave.files[i];
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
    func_8002CD4C(&rr, z, 0, s->unk48);
}

void menu_save_drawCursor(void) {
    u8 blink = (g_menu.unk10 & 3) != 0;

    if (!func_8002F810() && g_menuSave.phase == SAVE_PH_READY) {
        if (g_menuSave.state == SAVE_ST_PORT) {
            D_80077DE8.unk0 = 0x74;
            D_80077DE8.unk4 = 0x32;
            D_80077DE8.unk2 = g_menu.unk2 * 0x12 + 0x6D;
            func_800355E4();
        } else if (g_menuSave.state == SAVE_ST_LIST || (g_menuSave.state == SAVE_ST_ACCESS && blink)) {
            D_80077DE8.unk0 = 0;
            D_80077DE8.unk4 = 0x2A;
            D_80077DE8.unk2 = g_menuSave.list.cursor * SAVE_ROW_H + 0x42;
            func_800355E4();
        } else if (g_menuSave.state == SAVE_ST_CONFIRM && blink) {
            ui_drawHand(0, g_menuSave.list.cursor * SAVE_ROW_H + 0x42, 0x2A);
        }
    }
}

void menu_save_fileListTick(void) {
    MenuList *l = &g_menuSave.list;
    s32 v;
    s32 dec;

    if (g_menuSave.cardStatus[g_menu.unk2] != CARD_RES_DONE) {
        menu_save_showInfo(SAVE_TEXT_NO_CARD);
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

void menu_save_setHelp(void) {
    RECT r = g_menuSavePortHelpRect;
    u8 st;
    s32 v;

    D_80077DE8.unk8 = NULL;
    if (g_menuSave.phase == SAVE_PH_READY && g_menuSave.showCursor != 0) {
        func_80035580(NULL);
        st = g_menuSave.state;
        if (st == SAVE_ST_PORT) {
            func_80035580(&r);
            v = SAVE_TEXT_SAVES_TO;
            /* st is SAVE_ST_PORT == SAVE_MODE_LOAD here */
            if (g_menuSave.mode == st) {
                v = SAVE_TEXT_LOADS_FROM;
            }
            D_80077DE8.unk8 = text_get(v + g_menu.unk2);
        } else if (st == SAVE_ST_LIST) {
            menu_save_setFileHelp();
        } else if (st == SAVE_ST_CONFIRM) {
            D_80077DE8.unk8 = menu_save_getConfirmHelp();
        }
    }
}

void menu_save_setFileHelp(void) {
    MenuList *l;
    SaveMember *m;
    s32 idx;
    s32 n;
    s32 i;
    s32 st;

    l = &g_menuSave.list;
    idx = l->unk5 + l->cursor;
    st = g_menuSave.files[idx].state;
    switch (st) {
    case SAVE_FILE_EMPTY:
    case SAVE_FILE_OTHER:
    case SAVE_FILE_CORRUPT:
        D_80077DF0 = text_get(st + SAVE_TEXT_FILE_HELP);
        break;
    case SAVE_FILE_USED:
        g_menuSaveHelpBuf[0] = 0xFF;
        n = 0;
        m = g_menuSave.files[idx].party;
        for (i = 0; i < 4; i++) {
            if (m[i].present != 0) {
                n = i + 1;
            }
        }
        for (i = 0; i < n; i++) {
            if (m[i].present != 0) {
                func_8002BE44(i, m[i].name);
                func_8002BE5C(i, m[i].level);
                text_append(g_menuSaveHelpBuf, text_get(i + SAVE_TEXT_MEMBER1));
                if (i < n - 1) {
                    text_append(g_menuSaveHelpBuf, text_get(SAVE_TEXT_NEWLINE));
                }
            }
        }
        D_80077DF0 = g_menuSaveHelpBuf;
        break;
    }
}

void menu_save_update(void) {
    menu_save_drawTitle();
    menu_save_drawHelp();
    menu_save_input();
    menu_save_draw();
    menu_save_tick();
}

void menu_save_exit(void) {
    s32 i;

    func_8003563C(0);
    func_8003556C(NULL);
    menu_save_closeCardEvents();
    menu_save_restoreVram();
    menu_save_freeImage();
    for (i = 0; i < 2; i++) {
        D_80073998[i] = g_menuSave.unk18[i];
    }
}

u8 menu_save_isLoaded(void) {
    return g_menuSave.loaded;
}

u8 menu_save_isPortReady(s32 port, u8 allowNew) {
    s32 st = g_menuSave.cardStatus[port];
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

void menu_save_backToPorts(void) {
    func_8002F9F0();
    func_8002F8A8(menu_save_returnToPorts);
}

void menu_save_close(void) {
    func_8002F8E4(g_menuSave.unk14);
    func_800325E0();
}

void menu_save_drawTitle(void) {
    func_8002DEE4(&g_menuSaveTitleRect, 5, 2);
    func_8002705C(&g_menuSaveTitleRect, 5, 2);
    text_drawBlock(g_menuSaveTitleRect.x + 0xA, g_menuSaveTitleRect.y + 6, 4, (u8 *)text_getFrom(g_saveMenuText, g_menuSave.mode), 0);
}

void menu_save_draw(void) {
    void (*fn)(void) = g_menuSaveDrawFns[g_menuSave.state];

    if (fn != NULL) {
        fn();
    }
    if (g_menuSave.showCursor != 0) {
        menu_save_drawCursor();
    }
}

void menu_save_input(void) {
    if (!func_8002F810()) {
        switch (g_menuSave.phase) {
        case SAVE_PH_MESSAGE:
            menu_save_messageInput();
            break;
        case SAVE_PH_READY:
            if (g_menuSaveInputFns[g_menuSave.state] != NULL) {
                g_menuSaveInputFns[g_menuSave.state]();
            }
            break;
        }
    }
}

void menu_save_tick(void) {
    menu_save_cardPoll();
    g_menuSave.cardStatus[0] = menu_save_getCardStatus(CARD_CHAN(0));
    g_menuSave.cardStatus[1] = menu_save_getCardStatus(CARD_CHAN(1));
    if (g_menuSave.phase == SAVE_PH_READY) {
        if (g_menuSaveTickFns[g_menuSave.state] != NULL) {
            g_menuSaveTickFns[g_menuSave.state]();
        }
    }
    menu_save_tickCardAnim();
    menu_save_updateHelpVisibility();
}

void menu_save_fileListInput(void) {
    MenuList *l = &g_menuSave.list;

    if (l->unk8 == 0) {
        if (g_padRepeat & BTN_CIRCLE) {
            snd_playSe(SE_CANCEL);
            menu_save_backToPorts();
            return;
        }
        if (g_padRepeat & BTN_CROSS) {
            menu_save_selectFile();
            return;
        }
        menuList_up(l, g_padRepeat);
        menuList_down(l, g_padRepeat);
        menu_save_listPageUp(l, g_padRepeat);
        menu_save_listPageDown(l, g_padRepeat);
    }
}

void menu_save_drawPageScroll(void) {
    MenuView v;
    RECT r;
    s32 i;

    menu_save_listGetPageView(&g_menuSave.list, &v);
    r.x = SAVE_LIST_X;
    r.w = SAVE_LIST_W;
    r.y = 0;
    r.h = SAVE_ROW_H;
    for (i = 0; i < v.unk0; i++) {
        r.y = i * SAVE_ROW_H + (s16)(v.unk2 + SAVE_LIST_Y);
        menu_save_drawFile(&r, 0x2D, v.unk1 + i);
        func_8002DEE4(&r, 0x2D, 0);
    }
}

void menu_save_drawScrollArrows(void) {
    s32 blink = (((u32)g_menu.unk10 >> 4) ^ 1) & 1;
    MenuList *l = &g_menuSave.list;

    ui_drawSprite(0x12E, 0x2C, 0x2A, 0xB8, l->unk5 != 0 ? blink : 1);
    ui_drawSprite(0x12E, 0xCF, 0x2A, 0xB9, l->unk5 < l->unk2 - l->unk3 ? blink : 1);
}

void menu_save_drawEmptyFile(RECT *r, s32 otz) {
    text_drawBlock(r->x + 0x10, r->y + 0x16, otz, text_get(SAVE_TEXT_EMPTY_BLOCK), 0);
}

void menu_save_drawOtherFile(RECT *r, s32 otz) {
    text_drawBlock(r->x + 0x10, r->y + 0x16, otz, text_get(SAVE_TEXT_OTHER_GAME), 0);
}

void menu_save_drawCorruptFile(RECT *r, s32 otz) {
    text_drawBlock(r->x + 0x10, r->y + 0x16, otz, text_get(SAVE_TEXT_DATA_CORRUPTED), 0);
}

void menu_save_selectPort(void) {
    if (!menu_save_isPortReady_inline(g_menu.unk2, 1)) {
        snd_playSe(SE_BUZZER);
        menu_save_showInfo(SAVE_TEXT_NO_CARD);
        return;
    }
    snd_playSe(SE_CURSOR);
    menu_save_startScan();
}

void menu_save_selectFile(void) {
    s32 i = g_menuSave.list.unk5 + g_menuSave.list.cursor;

    if (g_menuSave.mode == SAVE_MODE_LOAD) {
        menu_save_selectLoadFile(i, i);
        return;
    }
    menu_save_selectSaveFile(i, i);
}

void menu_save_updateHelpVisibility(void) {
    u8 hide;

    hide = g_menuSave.loaded != 0;
    if (g_menuSave.phase != SAVE_PH_READY) {
        hide = 1;
    }
    if (g_menuSave.state != SAVE_ST_PORT && g_menuSave.state != SAVE_ST_LIST && g_menuSave.state != SAVE_ST_CONFIRM) {
        hide = 1;
    }
    func_8003563C(hide);
}

void menu_save_returnToPorts(void) {
    g_menuSave.state = SAVE_ST_PORT;
    g_menuSave.help = text_get(SAVE_TEXT_SELECT_SLOT);
    func_8002F9D0();
}

void func_801F5A60(void) {
}

void menu_save_drawCardIcon(s32 x, s32 y, s32 z, s32 idx) {
    u32 clut;
    Ovl39Sprite *e;

    clut = GetClut(0, CARD_ICON_CLUT_Y);

    setlen(g_primPtr, 4);
    e = &g_menuSaveCardIconFrames[idx];
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

void menu_save_loadCardIconImage(void) {
    RECT r;
    u8 *img = g_menuSaveCardIconImage;
    u8 *buf = g_menuSaveVramBackup;
    u8 *src = img;
    s32 i;

    r.y = CARD_ICON_CLUT_Y;
    r.x = 0;
    r.w = 0x100;
    r.h = 1;
    StoreImage(&r, (u32 *)buf);
    DrawSync(0);
    LoadImage(&r, (u32 *)img);
    DrawSync(0);
    buf += CARD_ICON_CLUT_SIZE;
    img += CARD_ICON_CLUT_SIZE;
    r.x = CARD_ICON_X;
    r.w = CARD_ICON_W;
    r.y = CARD_ICON_Y;
    r.h = CARD_ICON_STRIP_H;
    /* swaps VRAM and image strip by strip: each strip's old VRAM goes over the strip
       uploaded before it, the first one into g_menuSaveVramBackup */
    for (i = 0; i < CARD_ICON_STRIPS; i++) {
        if (i == 0) {
            StoreImage(&r, (u32 *)buf);
        } else {
            StoreImage(&r, (u32 *)src);
            src += CARD_ICON_STRIP_SIZE;
        }
        DrawSync(0);
        LoadImage(&r, (u32 *)img);
        DrawSync(0);
        r.y += CARD_ICON_STRIP_H;
        img += CARD_ICON_STRIP_SIZE;
    }
}

void menu_save_restoreVram(void) {
    RECT r;
    u8 *img;
    u8 *buf;
    s32 i;

    img = g_menuSaveCardIconImage;
    buf = g_menuSaveVramBackup;
    r.x = 0;
    r.y = CARD_ICON_CLUT_Y;
    r.w = 0x100;
    r.h = 1;
    LoadImage(&r, (u32 *)buf);
    DrawSync(0);
    buf += CARD_ICON_CLUT_SIZE;
    r.x = CARD_ICON_X;
    r.w = CARD_ICON_W;
    r.y = CARD_ICON_Y;
    r.h = CARD_ICON_STRIP_H;
    for (i = 0; i < CARD_ICON_STRIPS; i++) {
        if (i == 0) {
            LoadImage(&r, (u32 *)buf);
        } else {
            LoadImage(&r, (u32 *)img);
            img += CARD_ICON_STRIP_SIZE;
        }
        DrawSync(0);
        r.y += CARD_ICON_STRIP_H;
    }
}

void menu_save_tickCardAnim(void) {
    if (g_menuSaveCardAnim.enabled != 0) {
        if (++g_menuSaveCardAnim.tick >= CARD_ANIM_TICKS) {
            g_menuSaveCardAnim.tick = 0;
            if (++g_menuSaveCardAnim.frame >= CARD_ANIM_FRAMES) {
                g_menuSaveCardAnim.frame = 0;
            }
        }
    }
}

void menu_save_resetCardAnim(void) {
    g_menuSaveCardAnim.frame = 0;
    g_menuSaveCardAnim.tick = 0;
}

u8 menu_save_enableCardAnim(u8 v) {
    u8 old = g_menuSaveCardAnim.enabled;

    g_menuSaveCardAnim.enabled = v;
    return old;
}

void menu_save_drawCardAnim(s32 x, s32 y, s32 otz) {
    menu_save_drawCardIcon(x, y, otz, g_menuSaveCardAnim.frame);
}

void menu_save_openConfirm(s32 id, s32 yes, s32 no) {
    RECT r;
    u16 sz;
    s32 w, h;

    g_menuSave.state = SAVE_ST_CONFIRM;
    g_menuSave.phase = SAVE_PH_WAIT;
    g_menuSaveConfirm.text = (s32)text_get(id);
    sz = text_getInfo(id);
    w = (sz & 0xFFF) + 0x10;
    g_menuSaveConfirm.textSize = sz;
    g_menuSaveConfirm.active = 1;
    g_menuSaveConfirm.choice = CONFIRM_NO;
    h = ((sz >> 12) + 2) * 15 + 0x18;
    g_menuSaveConfirm.yesHelp = yes;
    g_menuSaveConfirm.noHelp = no;
    r.w = w;
    r.h = h;
    r.x = (g_gameState->unkC->disp.w - w) >> 1;
    r.y = (g_gameState->unkC->disp.h - h) >> 1;
    g_menuSaveConfirm.win = func_8002DC54(&r, 0x19, 2);
    win_setOnOpened(g_menuSaveConfirm.win, menu_save_onConfirmOpened);
    win_setOnClosed(g_menuSaveConfirm.win, menu_save_onConfirmClosed);
    win_setDrawContent(g_menuSaveConfirm.win, menu_save_drawConfirm);
    win_setDrawOverlay(g_menuSaveConfirm.win, menu_save_drawConfirmCursor);
    func_8002E31C(g_menuSaveConfirm.win, 1);
    func_8002E304(g_menuSaveConfirm.win, D_8007048C);
}

void menu_save_drawConfirm(Window *w) {
    u32 a = text_getInfo(SAVE_TEXT_YES);
    u32 b = text_getInfo(SAVE_TEXT_NO) & 0xFFF;
    s32 d1 = 0x26;
    s32 d2 = 0x17;
    s16 m;

    if (b < (a & 0xFFF)) {
        b = a & 0xFFF;
    }
    text_drawBlock(w->unk0 + 8, w->unk2 + 8, w->unk18, (u8 *)g_menuSaveConfirm.text, 0);
    m = b;
    text_drawBlock(w->unk0 + ((s32)(w->unk4 - m) >> 1), (w->unk2 - d1) + w->unk6, w->unk18, text_get(SAVE_TEXT_YES), 0);
    text_drawBlock(w->unk0 + ((s32)(w->unk4 - m) >> 1), (w->unk2 - d2) + w->unk6, w->unk18, text_get(SAVE_TEXT_NO), 0);
}

void menu_save_drawConfirmCursor(Window *w) {
    u32 a = text_getInfo(SAVE_TEXT_YES);
    u32 b = text_getInfo(SAVE_TEXT_NO) & 0xFFF;
    s32 z;

    if (b < (a & 0xFFF)) {
        b = a & 0xFFF;
    }
    if (w->unk1C == WIN_OPEN) {
        D_80077DE8.unk0 = w->unk0 + ((s32)(w->unk4 - b) >> 1) - 0x1A;
        D_80077DE8.unk2 = w->unk2 + (s16)(w->unk6 + (s16)(g_menuSaveConfirm.choice * 15 - 0x26)) + 3;
        z = w->unk18;
        D_80077DE8.unk4 = z;
        ui_drawHand(D_80077DE8.unk0, D_80077DE8.unk2, z - 1);
    }
}

void menu_save_initConfirm(void) {
    g_menuSaveConfirm.active = 0;
}

void menu_save_confirmInput(void) {
    if (g_menuSaveConfirm.win->unk1C == WIN_OPEN) {
        if (g_padRepeat & BTN_CROSS) {
            snd_playSe(SE_CURSOR);
            win_close(g_menuSaveConfirm.win);
            g_menuSave.phase = SAVE_PH_WAIT;
            return;
        }
        if (g_padRepeat & BTN_CIRCLE) {
            snd_playSe(SE_CANCEL);
            g_menuSaveConfirm.choice = CONFIRM_NO;
            win_close(g_menuSaveConfirm.win);
            g_menuSave.phase = SAVE_PH_WAIT;
            return;
        }
        if (g_padRepeat & (PAD_UP | PAD_DOWN)) {
            snd_playSe(SE_CURSOR);
            g_menuSaveConfirm.choice ^= 1;
        }
    }
}

void menu_save_confirmTick(void) {
    u8 st = g_menuSave.cardStatus[g_menu.unk2];

    if (st != CARD_RES_DONE && st != CARD_RES_NEW) {
        win_setOnClosed(g_menuSaveConfirm.win, NULL);
        win_close(g_menuSaveConfirm.win);
        g_menuSaveConfirm.active = 0;
        g_menuSaveConfirm.win = NULL;
        menu_save_showInfo(SAVE_TEXT_NO_CARD);
    }
}

void menu_save_setConfirmCallback(void (*v)(SaveConfirm *)) {
    g_menuSaveConfirm.onYes = v;
}

u8 menu_save_isConfirmActive(void) {
    return g_menuSaveConfirm.active;
}

u8 menu_save_setConfirmActive(u8 v) {
    u8 old = g_menuSaveConfirm.active;

    g_menuSaveConfirm.active = v;
    return old;
}

u8 *menu_save_getConfirmHelp(void) {
    return text_get(g_menuSaveConfirm.choice == CONFIRM_YES ? g_menuSaveConfirm.yesHelp : g_menuSaveConfirm.noHelp);
}

void menu_save_onConfirmOpened(Window *w) {
    g_menuSave.phase = SAVE_PH_READY;
}

void menu_save_onConfirmClosed(Window *w) {
    g_menuSave.state = SAVE_ST_LIST;
    if (g_menuSaveConfirm.choice == CONFIRM_YES) {
        g_menuSaveConfirm.onYes(&g_menuSaveConfirm);
    }
    g_menuSaveConfirm.active = 0;
    g_menuSave.phase = SAVE_PH_READY;
}

void menu_save_startScan(void) {
    s32 sl;
    s32 i;

    g_menuSave.state = SAVE_ST_SCAN;
    g_menuSave.formatted = 0;
    g_menuSave.newestFile = 0;
    g_menuSave.newestCounter = 0;
    g_menuSaveScan.step = SCAN_STEP_NONE;
    g_menuSaveScan.active = 1;
    g_menuSaveScan.win = func_8002DC54(&g_menuSaveScanRect, 0x1E, 2);
    g_menuSaveScan.fd = 0xFF;
    g_menuSaveScan.tries = 0;
    g_menuSaveScan.ff9Files = 0;
    g_menuSaveScan.headersRead = 0;
    g_menuSaveScan.otherBlocks = 0;
    g_menuSaveScan.dirCount = 0;
    g_menuSaveScan.file = 0;
    for (i = 0; i < 4; i++) {
        g_menuSaveScan.triesPerResult[i] = 0;
    }
    for (i = 0; i < SAVE_FILES; i++) {
        g_menuSave.files[i].state = SAVE_FILE_EMPTY;
        g_menuSave.files[i].index = i;
        g_menuSaveScan.unk2B4[i] = 0;
    }
    menu_save_resetCardAnim();
    menu_save_enableCardAnim(1);
    win_setDrawContent(g_menuSaveScan.win, menu_save_drawScan);
    win_setOnOpened(g_menuSaveScan.win, menu_save_onScanOpened);
    win_setOnClosed(g_menuSaveScan.win, menu_save_onScanClosed);
    func_8002E304(g_menuSaveScan.win, D_80070420);
    func_8002E31C(g_menuSaveScan.win, 1);
    sl = g_menu.unk2;
    switch (g_menuSave.cardStatus[sl]) {
    case CARD_RES_NEW:
        g_menuSaveScan.step = SCAN_STEP_CLEAR;
        if (menu_save_cardClearSync(CARD_CHAN(g_menu.unk2)) != CARD_RES_DONE) {
            menu_save_scanError(SAVE_TEXT_CHECK_FAILED);
        }
        break;
    case CARD_RES_DONE:
        g_menuSaveScan.step = SCAN_STEP_LOAD;
        if (menu_save_cardLoad(CARD_CHAN(g_menu.unk2), menu_save_onCardLoaded) == 0) {
            menu_save_scanError(SAVE_TEXT_CHECK_FAILED);
        }
        break;
    case CARD_RES_TIMEOUT:
        menu_save_scanError(SAVE_TEXT_NO_CARD);
        break;
    case CARD_RES_ERROR:
        menu_save_scanError(SAVE_TEXT_CHECK_FAILED);
        break;
    }
}

void menu_save_onCardLoaded(void) {
    s32 r;

    r = menu_save_getCardResult(CARD_CHAN(g_menu.unk2));

    g_menuSaveScan.tries++;
    g_menuSaveScan.triesPerResult[r]++;
    if (g_menuSaveScan.tries == g_menuSaveScan.triesPerResult[r]) {
        if (g_menuSaveScan.tries < g_menuSaveScanRetries[r]) {
            menu_save_retryScan(r);
            return;
        }
        switch (r) {
        case CARD_RES_DONE:
            menu_save_onCardFormatted();
            return;
        case CARD_RES_NEW:
            menu_save_onCardUnformatted();
            return;
        }
    }
    menu_save_scanError(SAVE_TEXT_CHECK_FAILED);
}

u8 menu_save_readDir(void) {
    DirEnt *d;
    SaveName20 *p;
    s32 i;
    s32 j;
    s32 k;
    s32 n;

    g_menuSaveScan.dirCount = menu_save_countFiles();
    if (g_menuSaveScan.dirCount < 0) {
        menu_save_scanError(SAVE_TEXT_CHECK_FAILED);
        return 0;
    }
    for (i = 0; i < g_menuSaveScan.dirCount; i++) {
        d = &g_menuSaveScan.dir[i];
        if (strncmp((char *)d, (char *)menu_save_getFilePrefix(), SAVE_PREFIX_LEN) == 0) {
            k = menu_save_parseFileNumber(d->name.v);
            p = &g_menuSave.files[k].fileName.name;
            *p = d->name;
            g_menuSave.files[k].fileName.term[0] = 0;
            g_menuSave.files[k].state = SAVE_FILE_USED;
            g_menuSaveScan.ff9Files++;
        }
    }
    for (i = 0; i < g_menuSaveScan.dirCount; i++) {
        d = &g_menuSaveScan.dir[i];
        if (strncmp((char *)d, (char *)menu_save_getFilePrefix(), SAVE_PREFIX_LEN) != 0) {
            n = (g_menuSaveScan.dir[i].size + CARD_BLOCK - 1) / CARD_BLOCK;
            for (j = 0; j < n; j++) {
                k = menu_save_findLastEmpty();
                p = &g_menuSave.files[k].fileName.name;
                *p = d->name;
                g_menuSave.files[k].fileName.term[0] = 0;
                g_menuSave.files[k].state = SAVE_FILE_OTHER;
                g_menuSaveScan.otherBlocks++;
            }
        }
    }
    return menu_save_checkSpace();
}

/* Inlined copy of menu_save_readHeader */
static inline void menu_save_readHeader_inline(s32 file) {
    s32 fd;

    g_menuSaveScan.file = file;
    fd = menu_save_openFile(CARD_CHAN(g_menu.unk2), (char *)g_menuSave.files[file].fileName.name.v);
    g_menuSaveScan.fd = fd;
    if (fd < 0) {
        menu_save_scanError(SAVE_TEXT_CHECK_FAILED);
        return;
    }
    menu_save_readAsync(fd, (s32)g_menuSaveScan.buf, SCAN_HEADER_SIZE, menu_save_onHeaderRead);
}

void menu_save_onHeaderRead(void) {
    if (menu_save_closeFile(g_menuSaveScan.fd) < 0) {
        menu_save_scanError(SAVE_TEXT_CHECK_FAILED);
        return;
    }
    switch (menu_save_getCardResult(CARD_CHAN(g_menu.unk2))) {
    case CARD_RES_TIMEOUT:
        menu_save_scanError(SAVE_TEXT_NO_CARD);
        return;
    case CARD_RES_ERROR:
    case CARD_RES_NEW:
        menu_save_scanError(SAVE_TEXT_CHECK_FAILED);
        return;
    }
    menu_save_copyHeader(&g_menuSave.files[g_menuSaveScan.file], (SaveHeader *)&g_menuSaveScan.buf[sizeof(CardFileHeader)]);
    if (++g_menuSaveScan.headersRead < g_menuSaveScan.ff9Files) {
        menu_save_readHeader_inline(menu_save_findUsed(g_menuSaveScan.headersRead));
        return;
    }
    menu_save_finishScan();
}

void menu_save_enterFileList(void) {
    g_menuSave.phase = SAVE_PH_READY;
    g_menuSave.state = SAVE_ST_LIST;
    g_menuSave.help = text_get(SAVE_TEXT_SELECT_BLOCK);
    func_8002DE10(g_menuSaveScan.win);
    func_8002EDFC(&g_menuSave.list, SAVE_FILES, SAVE_LIST_ROWS, 1, SAVE_LIST_H);
    if (g_menuSave.files[g_menuSave.newestFile].state == SAVE_FILE_USED) {
        g_menuSave.list.cursor = g_menuSave.files[g_menuSave.newestFile].cursor;
        g_menuSave.list.unk5 = g_menuSave.newestFile - g_menuSave.list.cursor;
    }
    func_8002F9D0();
    func_8002F8A8(menu_save_onFileListFadedIn);
}

void menu_save_findNewest(void) {
    s32 i;

    g_menuSave.newestCounter = 0;
    g_menuSave.newestFile = 0;
    for (i = 0; i < SAVE_FILES; i++) {
        if (g_menuSave.files[i].state == SAVE_FILE_USED) {
            if (g_menuSave.files[i].version != SAVE_VERSION) {
                g_menuSave.files[i].state = SAVE_FILE_CORRUPT;
            } else if (g_menuSave.newestCounter < g_menuSave.files[i].counter) {
                g_menuSave.newestCounter = g_menuSave.files[i].counter;
                g_menuSave.newestFile = i;
            }
        }
    }
}

void menu_save_initScan(void) {
    g_menuSaveScan.active = 0;
}

void menu_save_scanTick(void) {
    void (*fn)(void);

    if (!menu_save_isPortReady(g_menu.unk2, 1)) {
        menu_save_scanError(SAVE_TEXT_NO_CARD);
        return;
    }
    fn = g_menuSaveScanSteps[g_menuSaveScan.step];
    if (fn != NULL) {
        fn();
    }
}

u8 menu_save_isScanActive(void) {
    return g_menuSaveScan.active;
}

u8 menu_save_setScanActive(u8 v) {
    u8 old = g_menuSaveScan.active;

    g_menuSaveScan.active = v;
    return old;
}

s32 menu_save_findLastEmpty(void) {
    s32 i;

    for (i = 0; i < SAVE_FILES; i++) {
        if (g_menuSave.files[SAVE_FILES - 1 - i].state == SAVE_FILE_EMPTY) {
            return SAVE_FILES - 1 - i;
        }
    }
    return -1;
}

s32 menu_save_findUsed(s32 n) {
    s32 found = 0;
    s32 i;

    for (i = 0; i < SAVE_FILES; i++) {
        if (g_menuSave.files[i].state == SAVE_FILE_USED) {
            found++;
            if (n < found) {
                return i;
            }
        }
    }
    return -1;
}

s32 menu_save_parseFileNumber(u8 *name) {
    return (name[SAVE_NAME_NUMBER] - '0') * 10 + (name[SAVE_NAME_NUMBER + 1] - '0');
}

void menu_save_scanWaitClear(void) {
    if (menu_save_isCardBusy() == 0) {
        g_menuSaveScan.step++;
        if (menu_save_cardLoad(CARD_CHAN(g_menu.unk2), menu_save_onCardLoaded) == 0) {
            menu_save_scanError(SAVE_TEXT_CHECK_FAILED);
        }
    }
}

void menu_save_onCardFormatted(void) {
    g_menuSave.formatted = 1;
    if (menu_save_readDir()) {
        if (g_menuSaveScan.headersRead < g_menuSaveScan.ff9Files) {
            g_menuSaveScan.step = SCAN_STEP_HEADERS;
            menu_save_readHeader(menu_save_findUsed(g_menuSaveScan.headersRead));
            return;
        }
        menu_save_finishScan();
    }
}

void menu_save_onCardUnformatted(void) {
    g_menuSave.formatted = 0;
    if (g_menuSave.mode == SAVE_MODE_LOAD) {
        menu_save_scanInfo(SAVE_TEXT_NO_FF9_DATA);
        return;
    }
    menu_save_finishScan();
}

void menu_save_retryScan(s32 result) {
    if (result == CARD_RES_NEW) {
        g_menuSaveScan.step = SCAN_STEP_CLEAR;
        if (menu_save_cardClearSync(CARD_CHAN(g_menu.unk2)) == CARD_RES_DONE) {
            return;
        }
    } else if (menu_save_cardLoad(CARD_CHAN(g_menu.unk2), menu_save_onCardLoaded) != 0) {
        return;
    }
    menu_save_scanError(SAVE_TEXT_CHECK_FAILED);
}

void menu_save_drawScan(Window *w) {
    menu_save_drawCardAnim(w->unk0 + 0xE, w->unk2 + 0x12, w->unk18);
    text_drawBlock(w->unk0 + 0x4A, w->unk2 + 0xE, w->unk18, text_get(SAVE_TEXT_DO_NOT_REMOVE), 0);
}

void menu_save_onScanOpened(Window *w) {
    g_menuSave.phase = SAVE_PH_READY;
    g_menuSave.help = text_get(SAVE_TEXT_CHECKING);
}

void menu_save_onScanClosed(Window *w) {
}

s32 menu_save_checkSpace(void) {
    if (g_menuSave.mode != SAVE_MODE_LOAD) {
        if (g_menuSaveScan.otherBlocks >= SAVE_FILES) {
            menu_save_scanInfo(SAVE_TEXT_CARD_FULL);
            return 0;
        }
    } else if (g_menuSaveScan.ff9Files == 0) {
        menu_save_scanInfo(SAVE_TEXT_NO_FF9_DATA);
        return 0;
    }
    return 1;
}

s32 menu_save_countFiles(void) {
    s32 port;
    s32 n;
    s32 prev;
    s32 r;

    prev = 0;
    n = 0;
    port = CARD_CHAN(g_menu.unk2);
    do {
        r = menu_save_readDirectory(port, (s32)g_menuSaveScan.dir);
        if (n != 0 && prev != r) {
            goto fail;
        }
        n++;
        prev = r;
    } while (n < 3);
    if (menu_save_cardInfoSync(port) == CARD_RES_DONE) {
        return r;
    }
fail:
    menu_save_scanError(SAVE_TEXT_CHECK_FAILED);
    return -1;
}

void menu_save_finishScan(void) {
    g_menuSaveScan.step = SCAN_STEP_DONE;
    g_menuSaveScan.active = 0;
    menu_save_findNewest();
    if (!menu_save_isMessageActive()) {
        g_menuSave.phase = SAVE_PH_WAIT;
        func_8002F9F0();
        func_8002F8A8(menu_save_enterFileList);
        snd_playSe(SE_SAVE_LIST);
    }
}

void menu_save_onFileListFadedIn(void) {
    if (g_menuSave.phase != SAVE_PH_MESSAGE) {
        g_menuSave.phase = SAVE_PH_READY;
    }
}

void menu_save_readHeader(s32 file) {
    s32 fd;

    g_menuSaveScan.file = file;
    fd = menu_save_openFile(CARD_CHAN(g_menu.unk2), (char *)g_menuSave.files[file].fileName.name.v);
    g_menuSaveScan.fd = fd;
    if (fd < 0) {
        menu_save_scanError(SAVE_TEXT_CHECK_FAILED);
        return;
    }
    menu_save_readAsync(fd, (s32)g_menuSaveScan.buf, SCAN_HEADER_SIZE, menu_save_onHeaderRead);
}

void menu_save_copyHeader(SaveFile *s, SaveHeader *h) {
    s32 i;

    s->version = h->version;
    s->name = h->name;
    s->location = h->location;
    s->name.v[7] = 0xFF;
    s->discs = h->discs;
    s->level = h->level;
    s->playTime = h->playTime;
    s->gil = h->gil;
    s->unk48 = h->unk74;
    s->counter = h->counter;
    s->cursor = h->cursor;
    for (i = 0; i < 4; i++) {
        s->party[i] = h->party[i];
    }
}

void menu_save_scanError(s32 id) {
    g_menuSaveScan.step = SCAN_STEP_FAILED;
    win_close(g_menuSaveScan.win);
    g_menuSaveScan.win = NULL;
    menu_save_showError(id);
}

void menu_save_scanInfo(s32 id) {
    g_menuSaveScan.step = SCAN_STEP_FAILED;
    win_close(g_menuSaveScan.win);
    g_menuSaveScan.win = NULL;
    menu_save_showInfo(id);
}

void menu_save_initSavedMsg(void) {
    g_menuSaveSavedMsg.active = 0;
    g_menuSaveSavedMsg.win = 0;
    g_menuSaveSavedMsg.timer = 0;
}

void menu_save_openSavedMsg(SaveMenu *m) {
    if (g_menuSaveSavedMsg.active == 0) {
        g_menuSave.phase = SAVE_PH_WAIT;
        g_menuSaveSavedMsg.timer = 0;
        g_menuSaveSavedMsgRect.y = g_menuSave.list.cursor * SAVE_ROW_H + 0x3F;
        g_menuSaveSavedMsg.win = func_8002DC54(&g_menuSaveSavedMsgRect, 0x14, 2);
        win_setDrawContent(g_menuSaveSavedMsg.win, menu_save_drawSavedMsg);
        win_setOnOpened(g_menuSaveSavedMsg.win, menu_save_onSavedMsgOpened);
        win_setOnClosed(g_menuSaveSavedMsg.win, menu_save_onSavedMsgClosed);
        func_8002E31C(g_menuSaveSavedMsg.win, 1);
    }
}

void menu_save_savedMsgInput(void) {
    if (g_menuSaveSavedMsg.win->unk1C == WIN_OPEN && (g_padRepeat & (BTN_CIRCLE | BTN_CROSS))) {
        snd_playSe(SE_CANCEL);
        g_menuSave.phase = SAVE_PH_WAIT;
        win_close(g_menuSaveSavedMsg.win);
    }
}

void menu_save_savedMsgTick(void) {
    if (!menu_save_isPortReady(g_menu.unk2, 0)) {
        func_8002DE10(g_menuSaveSavedMsg.win);
        menu_save_showInfo(SAVE_TEXT_NO_CARD);
        return;
    }
    if (++g_menuSaveSavedMsg.timer >= SAVED_MSG_TICKS) {
        g_menuSave.phase = SAVE_PH_WAIT;
        win_close(g_menuSaveSavedMsg.win);
    }
}

void menu_save_onSavedMsgOpened(Window *w) {
    g_menuSave.phase = SAVE_PH_READY;
    g_menuSave.state = SAVE_ST_SAVED;
}

void menu_save_onSavedMsgClosed(Window *w) {
    g_menuSave.phase = SAVE_PH_READY;
    g_menuSave.state = SAVE_ST_LIST;
    g_menuSaveSavedMsg.win = 0;
    g_menuSaveSavedMsg.timer = 0;
    g_menuSave.help = text_get(SAVE_TEXT_SELECT_BLOCK);
}

void menu_save_drawSavedMsg(Window *w) {
    u8 *s = text_get(SAVE_TEXT_DATA_SAVED);

    text_drawBlock(w->unk0 + ((w->unk4 - func_8002BA68((s32)s)) >> 1), w->unk2 + 8, w->unk18, s, 0);
}

void menu_save_openMessage(s32 id, s32 title) {
    SaveMessage *m = &g_menuSaveMessage;
    RECT r;
    s32 i;
    u16 sz;
    s32 w, h;

    if (m->active == 0) {
        snd_playSe(SE_SAVE_MESSAGE);
        g_menuSave.phase = SAVE_PH_WAIT;
        for (i = SAVE_FILES - 1; i >= 0; i--) {
            g_menuSave.files[i].state = SAVE_FILE_NONE;
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
        win_setDrawContent(m->win, menu_save_drawMessage);
        win_setOnOpened(m->win, menu_save_onMessageOpened);
        win_setOnClosed(m->win, menu_save_onMessageClosed);
        func_8002E31C(m->win, 1);
        func_8002E304(m->win, title);
    }
}

void menu_save_initMessage(void) {
    g_menuSaveMessage.active = 0;
}

void menu_save_messageInput(void) {
    if (g_menuSaveMessage.win->unk1C == WIN_OPEN && (g_padRepeat & (BTN_CIRCLE | BTN_CROSS))) {
        snd_playSe(SE_CANCEL);
        g_menuSave.phase = SAVE_PH_WAIT;
        win_close(g_menuSaveMessage.win);
    }
}

void menu_save_showError(s32 id) {
    menu_save_openMessage(id, D_80070440);
}

void menu_save_showInfo(s32 id) {
    menu_save_openMessage(id, D_8007048C);
}

u8 menu_save_setMessageActive(u8 v) {
    u8 old = g_menuSaveMessage.active;

    g_menuSaveMessage.active = v;
    return old;
}

u8 menu_save_isMessageActive(void) {
    return g_menuSaveMessage.active;
}

void menu_save_drawMessage(Window *w) {
    text_drawBlock(w->unk0 + 8, w->unk2 + 8, w->unk18, g_menuSaveMessage.text, 0);
}

void menu_save_onMessageOpened(Window *w) {
    g_menuSave.phase = SAVE_PH_MESSAGE;
}

void menu_save_onMessageClosed(Window *w) {
    g_menuSave.phase = SAVE_PH_READY;
    g_menuSaveMessage.win = NULL;
    g_menuSaveMessage.active = 0;
    if (g_menuSaveResetAfterMsg[g_menuSave.state] != 0) {
        g_menuSave.phase = SAVE_PH_WAIT;
        func_8002F9F0();
        func_8002F8A8(menu_save_resetToPorts);
        return;
    }
    g_menuSave.help = text_get(SAVE_TEXT_SELECT_SLOT);
    g_menuSave.state = SAVE_ST_PORT;
}

void menu_save_resetToPorts(void) {
    g_menuSave.help = text_get(SAVE_TEXT_SELECT_SLOT);
    g_menuSave.state = SAVE_ST_PORT;
    g_menuSave.phase = SAVE_PH_READY;
    func_8002F9D0();
    func_8002F8A8(menu_save_onPortsFadedIn);
}

void menu_save_onPortsFadedIn(void) {
    g_menuSave.phase = SAVE_PH_READY;
}

void menu_save_loadImage(void) {
    func_80028720(g_menuSaveImage, 10);
}

void menu_save_freeImage(void) {
    func_8002886C();
}

u8 menu_save_getPortrait(Character *c) {
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
    return g_menuSavePortraitIds[m];
}

u8 menu_save_startAccess(s32 mode) {
    g_menuSave.state = SAVE_ST_ACCESS;
    g_menuSave.phase = SAVE_PH_WAIT;
    g_menuSaveAccess.active = 1;
    g_menuSaveAccess.stage = ACCESS_STAGE_NONE;

    g_menuSaveAccess.mode = mode;
    g_menuSaveAccess.delay = 0;
    g_menuSaveAccess.lastProgress = 0;
    g_menuSaveAccess.progressTicks = 0;
    g_menuSaveAccess.unk1 = 0;
    menu_save_resetCardAnim();
    menu_save_resetReadProgress();
    menu_save_resetWriteProgress();
    g_menuSaveAccess.win = func_8002DC54(&g_menuSaveAccessRect, 0x1E, 2);
    win_setDrawContent(g_menuSaveAccess.win, menu_save_drawAccess);
    win_setOnOpened(g_menuSaveAccess.win, menu_save_onAccessOpened);
    win_setOnClosed(g_menuSaveAccess.win, menu_save_onAccessClosed);
    func_8002E31C(g_menuSaveAccess.win, 1);
    func_8002E304(g_menuSaveAccess.win, D_800703FC.unk0[g_menuSaveAccessTitles[mode]]);
    return 1;
}

INCLUDE_RODATA("asm/us/ovl_39/nonmatchings/ovl_39", g_menuSavePortHelpRect);

INCLUDE_RODATA("asm/us/ovl_39/nonmatchings/ovl_39", g_saveMenuText);

INCLUDE_RODATA("asm/us/ovl_39/nonmatchings/ovl_39", g_menuSaveImage);

INCLUDE_RODATA("asm/us/ovl_39/nonmatchings/ovl_39", g_menuSaveCardIconImage);

INCLUDE_RODATA("asm/us/ovl_39/nonmatchings/ovl_39", g_menuSaveIconEven);

INCLUDE_RODATA("asm/us/ovl_39/nonmatchings/ovl_39", g_menuSaveIconOdd);

INCLUDE_RODATA("asm/us/ovl_39/nonmatchings/ovl_39", g_menuSaveIconMinute);

INCLUDE_RODATA("asm/us/ovl_39/nonmatchings/ovl_39", g_menuSaveIconHour);

void menu_save_saveTick(void) {
    SaveAccess *p;
    SaveMenu *m;
    s32 t;
    s32 stage;

    switch (g_menuSaveAccess.stage) {
    case ACCESS_STAGE_OPEN:
        p = &g_menuSaveAccess;
        if (++p->delay >= ACCESS_TICKS) {
            m = &g_menuSave;
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
        if (++g_menuSaveAccess.delay >= ACCESS_TICKS) {
            g_menuSaveAccess.delay = 0;
            if (menu_save_formatCard()) {
                g_menuSaveAccess.stage = ACCESS_STAGE_FORMATTING;
            }
        }
        break;
    case ACCESS_STAGE_FORMATTING:
        break;
    case ACCESS_STAGE_IO:
        if (++g_menuSaveAccess.delay >= ACCESS_TICKS) {
            g_menuSaveAccess.delay = 0;
            if (menu_save_writeFile()) {
                g_menuSaveAccess.stage = ACCESS_STAGE_WAIT;
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

void menu_save_loadTick(void) {
    switch (g_menuSaveAccess.stage) {
    case ACCESS_STAGE_OPEN:
        if (++g_menuSaveAccess.delay >= ACCESS_TICKS) {
            g_menuSaveAccess.stage = ACCESS_STAGE_IO;
            g_menuSaveAccess.delay = 0;
            g_menuSave.help = text_get(SAVE_TEXT_LOADING);
        }
        break;
    case ACCESS_STAGE_IO:
        if (++g_menuSaveAccess.delay >= ACCESS_TICKS) {
            g_menuSaveAccess.delay = 0;
            if (menu_save_readFile()) {
                g_menuSaveAccess.stage = ACCESS_STAGE_WAIT;
            }
        }
        break;
    case ACCESS_STAGE_WAIT:
        break;
    }
}

void menu_save_drawAccess(Window *w) {
    RECT *r;
    s32 st;
    s32 n;

    r = (RECT *)w;
    if (g_menuSave.formatted != 0) {
        if (g_menuSave.mode != SAVE_MODE_LOAD) {
            st = menu_save_getWriteProgress();
        } else {
            st = menu_save_getReadProgress();
        }
        g_menuSaveAccess.progressTicks = st == g_menuSaveAccess.lastProgress ? g_menuSaveAccess.progressTicks + 1 : 0;
        n = g_menuSaveAccess.progressTicks * 2;
        if (n >= 6) {
            n = 5;
        }
        menu_save_drawCardAnim(r->x + 0xE, r->y + 0x12, w->unk18);
        text_drawBlock(r->x + 0x4A, r->y + 0xE, w->unk18, text_get(SAVE_TEXT_DO_NOT_REMOVE), 0);
        menu_save_drawGauge(r->x + 0x1A, r->y + 0x44, w->unk18, st * ACCESS_GAUGE_STEP + n, ACCESS_GAUGE_W);
        g_menuSaveAccess.lastProgress = st;
        return;
    }
    text_drawBlock(r->x + 0x26, r->y + 0xC, w->unk18, text_get(SAVE_TEXT_FORMATTING_CARD), 0);
    text_drawBlock(r->x + 0x32, r->y + 0x20, w->unk18, text_get(SAVE_TEXT_DO_NOT_REMOVE), 0);
}

u8 menu_save_writeFile(void) {
    SaveFile *sl;
    s32 port;
    s32 fd;

    port = CARD_CHAN(g_menu.unk2);
    sl = &g_menuSave.files[g_menuSave.file];
    /* not SAVE_FILE_CORRUPT or SAVE_FILE_USED: create a new file */
    if ((u32)(sl->state - SAVE_FILE_CORRUPT) >= 2) {
        strcpy((char *)g_menuSaveAccess.fileName.name.v, menu_save_getFilePrefix());
        strcat(g_menuSaveAccess.fileName.name.v, "00000-");
        g_menuSaveAccess.fileName.name.v[SAVE_NAME_NUMBER] = g_menuSave.file / 10 + '0';
        g_menuSaveAccess.fileName.name.v[SAVE_NAME_NUMBER + 1] = g_menuSave.file % 10 + '0';
        /* called before its definition without a prototype: int result */
        if (!((s32 (*)(s32, char *))menu_save_createFile)(port, (char *)g_menuSaveAccess.fileName.name.v)) {
            menu_save_abortAccess(SAVE_TEXT_SAVE_FAILED);
            return 0;
        }
    } else {
        strcpy((char *)g_menuSaveAccess.fileName.name.v, sl->fileName.name.v);
    }
    fd = menu_save_openFile(port, (char *)g_menuSaveAccess.fileName.name.v);
    g_menuSaveAccess.fd = fd;
    if (fd < 0) {
        menu_save_abortAccess(SAVE_TEXT_SAVE_FAILED);
        return 0;
    }
    menu_save_writeAsync(fd, (s32)SAVE_BUF, SAVE_BUF_SIZE, menu_save_onFileIo);
    menu_save_enableCardAnim(1);
    return 1;
}

void menu_save_drawGauge(s32 x, s32 y, s32 otz, s32 pos, s32 w) {
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
            l->rgbc = g_menuSaveGaugeColors[i];
            l->xy0 = (x + 4) | yy;
            l->xy1 = (pos + x + 3) | yy;
            addPrim(&g_ot[otz], l);
            g_primPtr = (u8 *)(l + 1);
        }
    }
    func_80027710(x, y, otz, ic, 0);
    func_80027710(x + width - ec->unk4, y, otz, ec, 0);
    SetTexWindow(g_primPtr, &g_menuSaveGaugeTexWindow);
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

void menu_save_initAccess(void) {
    g_menuSaveAccess.active = 0;
}

void menu_save_accessTick(void) {
    if (!menu_save_isPortReady(g_menu.unk2, 0)) {
        menu_save_abortAccess(SAVE_TEXT_NO_CARD);
        return;
    }
    if (g_menuSave.phase != SAVE_PH_MESSAGE) {
        if (g_menuSaveAccess.mode == ACCESS_SAVE) {
            menu_save_saveTick();
        } else {
            menu_save_loadTick();
        }
    }
}

u8 menu_save_isAccessActive(void) {
    return g_menuSaveAccess.active;
}

u8 menu_save_setAccessActive(u8 v) {
    u8 old = g_menuSaveAccess.active;

    g_menuSaveAccess.active = v;
    return old;
}

u8 *menu_save_getFilePrefix(void) {
    memcpy(g_menuSaveFilePrefix, g_menuSaveProductCode, SAVE_PREFIX_LEN + 1);
    return g_menuSaveFilePrefix;
}

void menu_save_onAccessOpened(Window *w) {
    g_menuSave.phase = SAVE_PH_READY;
    g_menuSaveAccess.stage = ACCESS_STAGE_OPEN;
}

void menu_save_onAccessClosed(Window *w) {
    g_menuSaveAccess.active = 0;
    g_menuSave.phase = SAVE_PH_READY;
    if (g_menuSave.mode != SAVE_MODE_LOAD) {
        menu_save_openSavedMsg(&g_menuSave);
        return;
    }
    g_menuSave.state = SAVE_ST_LIST;
}

u8 menu_save_readFile(void) {
    SaveFile *sl;
    s32 port;
    s32 fd;

    port = CARD_CHAN(g_menu.unk2);
    sl = &g_menuSave.files[g_menuSave.file];
    strcpy((char *)g_menuSaveAccess.fileName.name.v, sl->fileName.name.v);
    fd = menu_save_openFile(port, (char *)g_menuSaveAccess.fileName.name.v);
    g_menuSaveAccess.fd = fd;
    if (fd < 0) {
        menu_save_abortAccess(SAVE_TEXT_LOAD_FAILED);
        return 0;
    }
    menu_save_readAsync(fd, (s32)SAVE_BUF, SAVE_BUF_SIZE, menu_save_onFileIo);
    return 1;
}

u8 menu_save_formatCard(void) {
    if (menu_save_cardFormat(CARD_CHAN(g_menu.unk2)) == 0) {
        menu_save_abortAccess(SAVE_TEXT_FORMAT_FAILED);
        return 0;
    }
    if (menu_save_cardLoad(CARD_CHAN(g_menu.unk2), menu_save_onFormatDone) != 0) {
        return 1;
    }
    menu_save_abortAccess(SAVE_TEXT_SAVE_FAILED);
    return 0;
}

void menu_save_onFileIo(void) {
    s32 n = menu_save_getCardResult(CARD_CHAN(g_menu.unk2));

    if (menu_save_closeFile(g_menuSaveAccess.fd) < 0) {
        menu_save_abortAccess(g_menuSave.mode == SAVE_MODE_LOAD ? SAVE_TEXT_LOAD_FAILED : SAVE_TEXT_SAVE_FAILED);
        return;
    }
    if (n == CARD_RES_DONE) {
        menu_save_onFileIoDone();
        return;
    }
    menu_save_onFileIoFailed(n);
}

void menu_save_onFileIoDone(void) {
    u8 mode;

    win_close(g_menuSaveAccess.win);
    g_menuSave.phase = SAVE_PH_WAIT;
    mode = g_menuSave.mode;
    if (mode != SAVE_MODE_LOAD) {
        menu_save_storeSavedFile(g_menuSave.file, &SAVE_BUF->hdr);
        snd_playSe(SE_SAVE_DONE);
        return;
    }
    if (menu_save_applyLoaded()) {
        g_menuSave.loaded = mode;
        snd_playSe(SE_SAVE_DONE);
        if (g_menu.unk1 == 0) {
            func_800361CC(0);
            return;
        }
        menu_save_close();
    }
}

void menu_save_onFileIoFailed(s32 result) {
    if (result <= CARD_RES_NEW) {
        if (result <= CARD_RES_DONE) {
            return;
        }
        menu_save_abortAccess(g_menuSave.mode != SAVE_MODE_LOAD ? SAVE_TEXT_SAVE_FAILED : SAVE_TEXT_LOAD_FAILED);
    }
}

void menu_save_storeSavedFile(s32 file, SaveHeader *h) {
    SaveFile *s;
    SaveFileName *n;
    s32 i;

    s = &g_menuSave.files[file];
    n = &g_menuSaveAccess.fileName;
    s->fileName = *n;
    s->state = SAVE_FILE_USED;

    s->discs = h->discs;
    s->level = h->level;
    s->name = h->name;
    s->location = h->location;
    s->playTime = h->playTime;
    s->gil = h->gil;
    s->unk48 = h->unk74;
    s->counter = h->counter;
    s->cursor = h->cursor;
    for (i = 0; i < 4; i++) {
        s->party[i] = h->party[i];
    }
}

void menu_save_abortAccess(s32 msg) {
    g_menuSave.showCursor = 1;
    menu_save_closeFile(g_menuSaveAccess.fd);
    win_setOnClosed(g_menuSaveAccess.win, menu_save_onAccessAborted);
    win_close(g_menuSaveAccess.win);
    menu_save_showError(msg);
}

void menu_save_onFormatDone(void) {
    s32 r = menu_save_getCardResult();

    g_menuSave.showCursor = 1;
    if (r == CARD_RES_DONE) {
        snd_playSe(SE_SAVE_LIST);
        g_menuSave.formatted = 1;
        g_menuSaveAccess.stage = ACCESS_STAGE_IO;
        g_menuSaveAccess.delay = ACCESS_TICKS; /* write on the next tick */
        g_menuSave.help = text_get(SAVE_TEXT_SAVING);
        return;
    }
    win_setOnClosed(g_menuSaveAccess.win, menu_save_onAccessAborted);
    win_close(g_menuSaveAccess.win);
    menu_save_showError(SAVE_TEXT_SAVE_FAILED);
}

void menu_save_onAccessAborted(Window *w) {
    g_menuSaveAccess.active = 0;
    g_menuSave.phase = SAVE_PH_READY;
}

MenuView *menu_save_listGetView(MenuList *l, MenuView *v) {
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

MenuView *menu_save_listGetPageView(MenuList *l, MenuView *v) {
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

void menu_save_listPageClip(MenuList *m, RECT *src, RECT *dst) {
    dst->x = src->x;
    dst->w = src->w;
    if (m->unk8 & 2) {
        dst->y = src->y + m->unk9;
    } else {
        dst->y = src->y;
    }
    dst->h = src->h - m->unk9;
}

void menu_save_listPageUp(MenuList *l, s32 btn) {
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

void menu_save_listPageDown(MenuList *l, s32 pad) {
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

void menu_save_buildHeader(SaveBuf *buf) {
    SaveHeader *h;
    Unk8004AB64 *o;
    s32 t;
    s32 i;
    u8 n;

    h = &buf->hdr;
    t = game_getPlayTime();
    menu_save_buildCardHeader(&buf->card, t);
    n = 0;
    o = func_8004AE00();
    if (o != NULL) {
        n = o->unk3B;
    }
    if (g_menuSave.mode == SAVE_MODE_NEXT_DISC) {
        h->discs = func_800B5074();
    } else {
        h->discs = g_gameState->unkB;
    }
    h->discs = menu_save_getDiscBit();
    h->version = SAVE_VERSION;
    h->level = g_gameState->unk1C->characters[n].level;
    h->playTime = t;
    h->gil = g_gameState->unk1C->gil;
    h->unk74 = (g_gameState->unk24 >> 18) & 1;
    h->counter = ++g_menuSave.newestCounter;
    h->cursor = g_menuSave.list.cursor;
    h->name = *(SaveName *)g_gameState->unk1C->characters[n].unk0;
    menu_save_getLocation((u8 *)&h->location);
    for (i = 0; i < 4; i++) {
        if (g_gameState->unk1C->party[i] != NULL) {
            h->party[i].present = 1;
            h->party[i].portrait = menu_save_getPortrait(g_gameState->unk1C->party[i]);
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

void menu_save_getLocation(u8 *dst) {
    u8 *name;
    u8 *s;

    if (g_menuSave.mode == SAVE_MODE_NEXT_DISC) {
        name = text_get(func_800B5064() + SAVE_TEXT_DISC_START);
    } else {
        name = g_gameState->unk1C->unk80C;
    }
    if (name == NULL) {
        s = text_get(SAVE_TEXT_NO_LOCATION);
        memset(dst, 0xFF, sizeof(SaveLocation));
        memcpy(dst, s, func_8002BCC0(s));
    } else {
        memset(g_menuSaveLocationBuf, 0xFF, sizeof(g_menuSaveLocationBuf));
        memcpy(g_menuSaveLocationBuf, name, func_8002BCC0(name));
        g_menuSaveLocationBuf[sizeof(SaveLocation) - 1] = 0xFF; /* cut to fit */
        memset(dst, 0xFF, sizeof(SaveLocation));
        memcpy(dst, g_menuSaveLocationBuf, func_8002BCC0(g_menuSaveLocationBuf));
    }
}

void menu_save_buildCardHeader(CardFileHeader *buf, s32 t) {
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
    *(TitleTmpl *)title = g_menuSaveTitleTemplate;
    title[0x11] += (g_menuSave.file + 1) / 10;
    n = g_menuSave.file + 1;
    title[0x17] += hour / 10;
    title[0x20] = 0;
    title[0x19] += hour % 10;
    title[0x1D] += min / 10;
    title[0x1F] += min % 10;
    title[0x13] += n % 10;
    if (sec == 0) {
        if (min == 0) {
            icon = &g_menuSaveIconHour;
        } else {
            icon = &g_menuSaveIconMinute;
        }
    } else if (t & 1) {
        icon = &g_menuSaveIconOdd;
    } else {
        icon = &g_menuSaveIconEven;
    }
    buf->icon = *icon;
}

void menu_save_selectSaveFile(s32 file, s32 unused) {
    g_menuSave.file = file;
    if (g_menuSave.formatted == 0) {
        snd_playSe(SE_CURSOR);
        menu_save_setConfirmCallback(menu_save_doSave);
        menu_save_openConfirm(g_menu.unk2 + SAVE_TEXT_UNFORMATTED, SAVE_TEXT_FORMAT_YES, SAVE_TEXT_FORMAT_NO);
        return;
    }
    switch (g_menuSave.files[g_menuSave.file].state) {
    case SAVE_FILE_EMPTY:
    case SAVE_FILE_CORRUPT:
        snd_playSe(SE_CURSOR);
        menu_save_doSave();
        break;
    case SAVE_FILE_USED:
        snd_playSe(SE_CURSOR);
        menu_save_setConfirmCallback(menu_save_doSave);
        menu_save_openConfirm(SAVE_TEXT_OVERWRITE, SAVE_TEXT_OVERWRITE_YES, SAVE_TEXT_OVERWRITE_NO);
        break;
    case SAVE_FILE_OTHER:
        snd_playSe(SE_BUZZER);
        break;
    }
}

void menu_save_doSave(void) {
    menu_save_initCrcTable();
    menu_save_buildHeader(SAVE_BUF);
    menu_save_packData(&SAVE_BUF->data);
    SAVE_BUF->crc = menu_save_crc16((u8 *)SAVE_BUF, SAVE_CRC_LEN);
    menu_save_startAccess(ACCESS_SAVE);
}

void menu_save_packData(SaveData *d) {
    menu_save_packState(&d->state);
    menu_save_packEvents(&d->events);
    menu_save_packParty(&d->party);
    menu_save_packMisc(&d->misc);
    menu_save_packItems(&d->items);
    menu_save_packCards(d->cards);
}

/* Story counter thresholds: g_evtGlobalVars.h[0] below 0xF3C is disc 1, and so on. */
s32 menu_save_getDiscBit(void) {
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

void menu_save_initCrcTable(void) {
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
        g_menuSaveCrcTable[i] = c;
    }
}

u16 menu_save_crc16(u8 *p, s32 n) {
    Crc16 c = { CRC16_INIT };

    while (--n >= 0) {
        c.w = g_menuSaveCrcTable[(c.w ^ *p++) & 0xFF] ^ (c.h >> 8);
    }
    return c.w;
}

void menu_save_selectLoadFile(s32 file, s32 arg1) {
    SaveFile *s;

    s = g_menuSave.files;
    g_menuSave.file = file;
    switch (s[file].state) {
    case SAVE_FILE_USED:
        snd_playSe(SE_CURSOR);
        menu_save_doLoad();
        break;
    case SAVE_FILE_EMPTY:
    case SAVE_FILE_OTHER:
    case SAVE_FILE_CORRUPT:
        snd_playSe(SE_BUZZER);
        break;
    }
}

u8 menu_save_applyLoaded(void) {
    SaveHeader *h;
    u8 ret;

    ret = 1;
    menu_save_initCrcTable();
    if (SAVE_BUF->crc == menu_save_crc16((u8 *)SAVE_BUF, SAVE_CRC_LEN)) {
        h = &SAVE_BUF->hdr;
        g_gameState->unkB = h->discs;
        game_setPlayTime(h->playTime);
        menu_save_unpackState(&SAVE_BUF->data.state);
        menu_save_unpackEvents(&SAVE_BUF->data.events);
        menu_save_unpackParty(&SAVE_BUF->data.party);
        menu_save_unpackMisc(&SAVE_BUF->data.misc);
        menu_save_unpackItems(&SAVE_BUF->data.items);
        menu_save_unpackCards(SAVE_BUF->data.cards);
    } else {
        menu_save_showError(SAVE_TEXT_CORRUPTED);
        ret = 0;
    }
    return ret;
}

void menu_save_doLoad(void) {
    if (!menu_save_startAccess(ACCESS_LOAD)) {
        menu_save_showError(SAVE_TEXT_LOAD_FAILED);
    }
}


void menu_save_packParty(SaveParty *s) {
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

void menu_save_unpackParty(SaveParty *s) {
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

void menu_save_packState(SaveState *h) {
    GameData *gd;
    s32 i;

    gd = g_gameState->unk1C;
    h->mode = g_gameState->mode;
    h->unk1 = g_gameState->unkA;
    for (i = sizeof(h->reserved2) - 1; i >= 0; i--) {
        h->reserved2[i] = 0;
    }
    if (g_menuSave.mode == SAVE_MODE_NEXT_DISC) {
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

void menu_save_unpackState(SaveState *p) {
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

void menu_save_copyCards(CardData *d, CardData *s) {
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

void menu_save_packCards(void *p) {
    menu_save_copyCards(p, func_80062824());
}

void menu_save_unpackCards(void *p) {
    menu_save_copyCards(func_80062824(), p);
}

void menu_save_packEvents(SaveEvents *d) {
    d->vars = g_evtGlobalVars;
    d->unk800 = D_8007AEC4;
}

void menu_save_unpackEvents(SaveEvents *s) {
    g_evtGlobalVars = s->vars;
    D_8007AEC4 = s->unk800;
}

void menu_save_packItems(SaveItems *s) {
    s->items = g_gameState->unk1C->items;
    s->flags = g_gameState->unk1C->unk774;
    s->unk240 = *(GameStateUnk24 *)&g_gameState->unk24;
}

void menu_save_unpackItems(SaveItems *s) {
    g_gameState->unk1C->items = s->items;
    g_gameState->unk1C->unk774 = s->flags;
    *(GameStateUnk24 *)&g_gameState->unk24 = s->unk240;
    func_8002397C();
}

void menu_save_packMisc(SaveMisc *p) {
    p->unk0 = g_gameState->unk1C->unk844;
    p->unk4 = g_gameState->unk1C->unk880;
    p->unk14 = g_gameState->unk1C->unk892;
}

void menu_save_unpackMisc(SaveMisc *p) {
    g_gameState->unk1C->unk844 = p->unk0;
    g_gameState->unk1C->unk880 = p->unk4;
    g_gameState->unk1C->unk892 = p->unk14;
}

void menu_save_cardInit(void) {
    g_menuSaveCardOp = CARD_OP_INFO;
    g_menuSaveCardInfoChan = 0;
    g_menuSaveCardInfoBusy = 0;
    g_menuSaveInfoCallback1 = 0;
    g_menuSaveInfoCallback2 = 0;

}

INCLUDE_RODATA("asm/us/ovl_39/nonmatchings/ovl_39", g_menuSaveProductCode);

INCLUDE_RODATA("asm/us/ovl_39/nonmatchings/ovl_39", g_menuSaveTitleTemplate);

/* dir: a struct DIRENTRY * passed as an int, as libapi's firstfile/nextfile take it */
s32 menu_save_firstFile(s32 chan, s32 dir) {
    char path[16];

    if (chan == 0) {
        strcpy(path, "bu00:*");
    } else {
        strcpy(path, "bu10:*");
    }
    _card_wait(0);
    _card_wait(1);
    menu_save_cardIdle();
    return firstfile(path, dir);
}

s32 menu_save_nextFile(s32 dir) {
    _card_wait(0);
    _card_wait(1);
    menu_save_cardIdle();
    return nextfile(dir);
}

s32 menu_save_readDirectory(s32 chan, s32 dir) {
    s32 n = 0;

    if (menu_save_firstFile(chan, dir) == dir) {
        do {
            n++;
            dir += sizeof(DirEnt);
        } while (menu_save_nextFile(dir) == dir);
    }
    return n;
}

s32 menu_save_getCardStatus(s32 chan) {
    if (chan == 0) {

        return g_menuSaveCardStatus[0];
    }
    return g_menuSaveCardStatus[1];
}

s32 menu_save_cardInfoSync(s32 chan) {
    s32 r;

    menu_save_clearSwCard();
    _card_wait(0);
    _card_wait(1);
    _card_info(chan);
    while ((r = menu_save_testSwCard()) == -1) {
    }
    return r;
}

s32 menu_save_getCardOp(void) {
    return g_menuSaveCardOp;
}

void menu_save_resetReadProgress(void) {
    g_menuSaveSectorsDone = 0;
}

void menu_save_resetWriteProgress(void) {
    g_menuSaveSectorsDone = 0;
}

s32 menu_save_getReadProgress(void) {
    return g_menuSaveSectorsDone;
}

s32 menu_save_getWriteProgress(void) {
    return g_menuSaveSectorsDone;
}

s32 menu_save_isCardBusy(void) {
    return g_menuSaveCardOp != CARD_OP_INFO;
}

s32 menu_save_cardClearSync(s32 chan) {
    s32 r;

    _card_wait(0);
    _card_wait(1);
    menu_save_clearHwCard();
    _card_clear(chan);
    do {
        r = menu_save_testHwCard();
    } while (r == -1);
    menu_save_cardIdle();
    return r;
}

s32 menu_save_cardLoad(s32 chan, void (*cb)(void)) {
    s32 r = menu_save_cardLoadStart(chan);

    g_menuSaveCardCallback = cb;
    return r;
}

s32 menu_save_cardLoadStart(s32 chan) {
    g_menuSaveCardCallback = NULL;
    g_menuSaveCardChan = chan;
    _card_wait(0);
    _card_wait(1);
    menu_save_clearSwCard();
    g_menuSaveCardOp = CARD_OP_LOAD;
    return _card_load(chan);
}

s32 menu_save_openFile(s32 chan, char *name) {
    char path[32];

    _card_wait(0);
    _card_wait(1);
    g_menuSaveFileChan = chan;
    if (chan == 0) {
        strcpy(path, "bu00:");
    } else {
        strcpy(path, "bu10:");
    }
    strcat((u8 *)path, (u8 *)name);
    menu_save_cardIdle();
    return open(path, FASYNC | FWRITE | FREAD);
}

s32 menu_save_closeFile(s32 fd) {
    _card_wait(0);
    _card_wait(1);
    menu_save_cardIdle();
    return close(fd);
}

s32 menu_save_writeAsync(s32 fd, s32 buf, s32 size, void (*cb)(void)) {
    s32 r = menu_save_writeStart(fd, buf, size);

    g_menuSaveCardCallback = cb;
    return r;
}

s32 menu_save_writeStart(s32 fd, s32 buf, s32 size) {
    g_menuSaveCardCallback = NULL;
    g_menuSaveFd = fd;
    g_menuSaveSectorsDone = 0;
    g_menuSaveIoBuf = buf;
    g_menuSaveSectorsLeft = (size + CARD_SECTOR - 1) / CARD_SECTOR - 1;
    menu_save_writeSector();
    return 1;
}

s32 menu_save_writeSector(void) {
    _card_wait(0);
    _card_wait(1);
    menu_save_clearSwCard();
    if (write(g_menuSaveFd, g_menuSaveIoBuf, CARD_SECTOR) == -1) {
        return 0;
    }
    g_menuSaveCardOp = CARD_OP_WRITE;
    return 1;
}

s32 menu_save_readAsync(s32 fd, s32 buf, s32 size, void (*cb)(void)) {
    s32 r = menu_save_readStart(fd, buf, size);

    g_menuSaveCardCallback = cb;
    return r;
}

s32 menu_save_readStart(s32 fd, s32 buf, s32 size) {
    g_menuSaveCardCallback = NULL;
    g_menuSaveFd = fd;
    g_menuSaveSectorsDone = 0;
    g_menuSaveIoBuf = buf;
    g_menuSaveSectorsLeft = (size + CARD_SECTOR - 1) / CARD_SECTOR - 1;
    _card_wait(0);
    _card_wait(1);
    menu_save_clearSwCard();
    menu_save_readSector();
    return 1;
}

s32 menu_save_readSector(void) {
    _card_wait(0);
    _card_wait(1);
    menu_save_clearSwCard();
    if (read(g_menuSaveFd, g_menuSaveIoBuf, CARD_SECTOR) == -1) {
        return 0;
    }
    g_menuSaveCardOp = CARD_OP_READ;
    return 1;
}

u8 menu_save_createFile(s32 chan, char *name) {
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

s32 menu_save_cardFormat(s32 chan) {
    _card_wait(0);
    _card_wait(1);
    menu_save_cardIdle();
    return _card_format(chan);
}

void menu_save_cardPoll(void) {
    s32 r;

    switch (g_menuSaveCardOp) {
    case CARD_OP_INFO:
        if (g_menuSaveCardInfoBusy == 0) {
            menu_save_clearSwCard();
            /* alternate between the two slots */
            g_menuSaveCardChan = g_menuSaveCardInfoChan = (g_menuSaveCardInfoChan + CARD_CHAN(1)) & CARD_CHAN(1);
            _card_wait(0);
            _card_wait(1);
            _card_info(g_menuSaveCardChan);
            g_menuSaveCardInfoBusy = 1;
        }
        r = menu_save_testSwCard();
        switch (r) {
        case CARD_RES_DONE:
            menu_save_cardPoll_set(g_menuSaveCardChan, CARD_RES_DONE);
            break;
        case CARD_RES_ERROR:
        case CARD_RES_NEW:
            menu_save_cardPoll_set(g_menuSaveCardChan, r);
            break;
        case CARD_RES_TIMEOUT:
            menu_save_cardPoll_retry(g_menuSaveCardChan, r);
            break;
        }
        if (r != -1) {
            if (g_menuSaveCardChan == 0) {
                if (g_menuSaveInfoCallback1) {
                    ((void (*)(void))g_menuSaveInfoCallback1)();
                }
                g_menuSaveInfoCallback1 = 0;
            } else {
                if (g_menuSaveInfoCallback2) {
                    ((void (*)(void))g_menuSaveInfoCallback2)();
                }
                g_menuSaveInfoCallback2 = 0;
            }
        }
        break;
    case CARD_OP_CLEAR:
        menu_save_testHwCard();
        break;
    case CARD_OP_LOAD:
        r = menu_save_testSwCard();
        g_menuSaveCardResult = r;
        if (r == -1) {
            break;
        }
        goto callback;
    case CARD_OP_READ:
        r = menu_save_testSwCard();
        g_menuSaveCardResult = r;
        if (r == -1) {
            break;
        }
        if (r != CARD_RES_DONE) {
            goto callback;
        }
        g_menuSaveCardResult = CARD_RES_DONE;
        lseek(g_menuSaveFd, CARD_SECTOR, SEEK_CUR);
        g_menuSaveSectorsDone++;
        if (g_menuSaveSectorsLeft != 0) {
            g_menuSaveSectorsLeft--;
            g_menuSaveIoBuf += CARD_SECTOR;
            menu_save_readSector();
            break;
        }
        goto callback;
    case CARD_OP_WRITE:
        r = menu_save_testSwCard();
        g_menuSaveCardResult = r;
        if (r == -1) {
            break;
        }
        if (r != CARD_RES_DONE) {
            goto callback;
        }
        g_menuSaveSectorsDone++;
        if (g_menuSaveSectorsLeft != 0) {
            g_menuSaveSectorsLeft--;
            g_menuSaveIoBuf += CARD_SECTOR;
            menu_save_writeSector();
            break;
        }
    callback:
        if (g_menuSaveCardCallback != NULL) {
            g_menuSaveCardCallback();
        }
        break;
    }
}

s32 menu_save_getCardResult(s32 chan) {
    return g_menuSaveCardResult;
}

void menu_save_cardIdle(void) {
    g_menuSaveCardOp = CARD_OP_INFO;

    g_menuSaveCardInfoBusy = 0;
}

s32 menu_save_testSwCard(void) {
    s32 r = -1;

    if (TestEvent(g_menuSaveSwEventIoe) == 1) {
        r = CARD_RES_DONE;
    } else if (TestEvent(g_menuSaveSwEventError) == 1) {
        r = CARD_RES_ERROR;
    } else if (TestEvent(g_menuSaveSwEventTimeout) == 1) {
        r = CARD_RES_TIMEOUT;
    } else if (TestEvent(g_menuSaveSwEventNew) == 1) {
        r = CARD_RES_NEW;
    }
    if (r != -1) {
        menu_save_cardIdle();
    }
    return r;
}

s32 menu_save_testHwCard(void) {
    s32 r = -1;

    if (TestEvent(g_menuSaveHwEventIoe) == 1) {
        r = CARD_RES_DONE;
    } else if (TestEvent(g_menuSaveHwEventError) == 1) {
        r = CARD_RES_ERROR;
    } else if (TestEvent(g_menuSaveHwEventTimeout) == 1) {
        r = CARD_RES_TIMEOUT;
    } else if (TestEvent(g_menuSaveHwEventNew) == 1) {
        r = CARD_RES_NEW;
    }
    if (r != -1) {
        menu_save_cardIdle();
    }
    return r;
}

void menu_save_openCardEvents(void) {
    EnterCriticalSection();
    g_menuSaveSwEventIoe = OpenEvent(SwCARD, EvSpIOE, EvMdNOINTR, NULL);
    g_menuSaveSwEventError = OpenEvent(SwCARD, EvSpERROR, EvMdNOINTR, NULL);
    g_menuSaveSwEventTimeout = OpenEvent(SwCARD, EvSpTIMOUT, EvMdNOINTR, NULL);
    g_menuSaveSwEventNew = OpenEvent(SwCARD, EvSpNEW, EvMdNOINTR, NULL);
    g_menuSaveHwEventIoe = OpenEvent(HwCARD, EvSpIOE, EvMdNOINTR, NULL);
    g_menuSaveHwEventError = OpenEvent(HwCARD, EvSpERROR, EvMdNOINTR, NULL);
    g_menuSaveHwEventTimeout = OpenEvent(HwCARD, EvSpTIMOUT, EvMdNOINTR, NULL);
    g_menuSaveHwEventNew = OpenEvent(HwCARD, EvSpNEW, EvMdNOINTR, NULL);
    EnableEvent(g_menuSaveSwEventIoe);
    EnableEvent(g_menuSaveSwEventError);
    EnableEvent(g_menuSaveSwEventTimeout);
    EnableEvent(g_menuSaveSwEventNew);
    EnableEvent(g_menuSaveHwEventIoe);
    EnableEvent(g_menuSaveHwEventError);
    EnableEvent(g_menuSaveHwEventTimeout);
    EnableEvent(g_menuSaveHwEventNew);
    ExitCriticalSection();
}

void menu_save_clearSwCard(void) {
    TestEvent(g_menuSaveSwEventIoe);
    TestEvent(g_menuSaveSwEventError);
    TestEvent(g_menuSaveSwEventTimeout);
    TestEvent(g_menuSaveSwEventNew);
}

void menu_save_clearHwCard(void) {
    TestEvent(g_menuSaveHwEventIoe);
    TestEvent(g_menuSaveHwEventError);
    TestEvent(g_menuSaveHwEventTimeout);
    TestEvent(g_menuSaveHwEventNew);
}

void menu_save_closeCardEvents(void) {
    EnterCriticalSection();
    CloseEvent(g_menuSaveSwEventIoe);
    CloseEvent(g_menuSaveSwEventError);
    CloseEvent(g_menuSaveSwEventTimeout);
    CloseEvent(g_menuSaveSwEventNew);
    CloseEvent(g_menuSaveHwEventIoe);
    CloseEvent(g_menuSaveHwEventError);
    CloseEvent(g_menuSaveHwEventTimeout);
    CloseEvent(g_menuSaveHwEventNew);
    ExitCriticalSection();
}
