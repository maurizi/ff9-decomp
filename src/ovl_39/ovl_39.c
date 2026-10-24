#include "ovl_39.h"

void menu_save_init(void) {
    s32 i;

    g_menuSave.unk0 = g_menu.unkC;
    g_menuSave.unk14 = func_8002F8E4(7);
    g_menuSave.unk1 = 1;
    g_menuSave.unk2 = 2;
    g_menuSave.unk7 = 0;
    g_menuSave.unk8 = 1;
    g_menuSave.unkC = 0;
    g_menuSave.unk10 = 0;
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
    g_menuSave.unk20 = text_get(0x16);
    g_menuSave.unk4[0] = 1;
    g_menuSave.unk4[1] = 1;
    menu_save_cardInit();
    menu_save_openCardEvents();
    func_801F5A60();
    for (i = 0; i < 2; i++) {
        g_menuSave.unk18[i] = D_80073998[i];
        D_80073998[i] |= 0x300;
    }
}

void menu_save_drawHelp(void) {
    u8 *s = text_get(g_menu.unk2 + 3);
    s32 w = func_8002BA68((s32)s);

    text_drawBlock(g_menuSavePortNameRect.x + ((g_menuSavePortNameRect.w - w) >> 1), g_menuSavePortNameRect.y + 2, 5, s, (menu_save_isPortReady(g_menu.unk2, 1) == 0) * 2);
    if (g_menuSave.unk20 != NULL) {
        text_drawBlock(g_menuSaveHelpRect.x + 0xA, g_menuSaveHelpRect.y + 6, 5, g_menuSave.unk20, 0);
    }
    ui_drawLabel(g_menuSaveHelpRect.x + 6, g_menuSaveHelpRect.y - 3, 5, D_80070450, 0);
    func_8002DEE4(&g_menuSavePortNameRect, 5, 0);
    func_8002DEE4(&g_menuSaveHelpRect, 5, 1);
    func_8002705C(&g_menuSaveHelpRect, 5, 1);
}

void menu_save_portInput(void) {
    if (g_padRepeat & BTN_CIRCLE) {
        snd_playSe(0x65);
        if (g_menu.unk1 != 0) {
            menu_save_close();
            return;
        }
        func_800361CC(0);
        return;
    }
    if (g_padRepeat & PAD_UP) {
        snd_playSe(0x67);
        g_menu.unk2 = g_menu.unk2 == 0 ? 1 : g_menu.unk2 - 1;
    }
    if (g_padRepeat & PAD_DOWN) {
        snd_playSe(0x67);
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
        str = text_get(i + 3);
        ok = 0;
        s = g_menuSave.unk4[i];
        if (s == 0 || s == 3) {
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
    MenuList *l = &g_menuSave.unk24;

    menu_save_drawScrollArrows();
    ui_drawLabel(0xE, 0x23, 0x2C, D_80070444, 0);
    ui_drawRule(8, 0x26, 0x2C, 1, 0x12C, (g_gameState->unk24 >> 18) & 1);
    r.x = 8;
    r.y = 0;
    r.w = 0x12C;
    r.h = 0x3C;
    menu_save_listGetView(l, &v);
    win_setClip(NULL, 0x2D, 4);
    for (i = 0; i < v.unk0; i++) {
        r.y = i * 0x3C + (s16)(v.unk2 + 0x2A);
        menu_save_drawFile(&r, 0x2D, v.unk1 + i);
    }
    if (l->unk8 & 4) {
        a.x = 8;
        a.y = 0x2A;
        a.w = 0x12C;
        a.h = 0xB4;
        menu_save_listPageClip(l, &a, &b);
        win_setClip(&b, 0x2D, 4);
        menu_save_drawPageScroll();
    }
    r.y = 0x2A;
    r.h = 0xB4;
    win_setClip(&r, 0x2D, 4);
}

void menu_save_drawFile(RECT *r, s32 otz, s32 slot) {
    char name[8];
    void (*fn)(RECT *, s32, s32);
    s32 v;

    strcpy(name, "File00");
    name[4] = (slot + 1) / 10 + 0x30;
    name[5] = (slot + 1) % 10 + 0x30;
    ui_drawSmallDigits(r->x + 6, r->y + 3, otz, (u8 *)name, 0);
    fn = g_menuSaveFileDrawers[g_menuSave.unk34[slot].unk1A];
    if (fn != NULL) {
        fn(r, otz, slot);
    }
    if (g_menuSave.unk34[slot].unk1A == 4) {
        v = g_menuSave.unk34[slot].unk48;
    } else {
        v = g_gameState->unk24 >> 18;
        v &= 1;
    }
    func_8002CD4C(r, 0x2D, 0, v);
}

void menu_save_drawFileInfo(RECT *r, s32 z, s32 i) {
    u8 buf[16];
    RECT rr;
    SaveSlot *s;
    s32 j;
    s32 n;

    s = &g_menuSave.unk34[i];
    n = 0;
    for (j = n; j < 4; j++) {
        if ((s->unk1C >> j) & 1) {
            buf[n++] = j + 0x31;
        }
    }
    buf[n] = 0;
    ui_drawSmallDigits(r->x + 0x30, r->y + 3, z, "/DISC", 0);
    ui_drawSmallDigits(r->x + 0x56, r->y + 3, z, buf, 0);
    for (j = 0; j < 4; j++) {
        if (s->unk58[j].unk1 != 0xFF) {
            func_80028204(r->x + 6 + j * 0x21, r->y + 0xD, z, s->unk58[j].unk1, 0x2C, 0);
        }
    }
    text_formatNumber(s->unk1D, buf, 2, 1);
    text_drawBlock(r->x + 0x90, r->y + 6, z, s->unk1E.v, 0);
    ui_drawSprite(r->x + 0x90, r->y + 0x17, z, 0xB4, 0);
    ui_drawDigits(r->x + 0xA2, r->y + 0x15, z, buf, 0);
    ui_drawSprite(r->x + 0xD8, r->y + 9, z, 0xB1, 0);
    ui_drawSprite(r->x + 0xD8, r->y + 0x16, z, 0xB2, 0);
    ui_drawSmallDigits(r->x + 0xE4, r->y + 0xA, z, buf, func_8002AA04(s->unk50, buf, 0));
    text_formatNumber(s->unk54, buf, 7, 1);
    buf[7] = 0xA4;
    buf[8] = 0;
    ui_drawSmallDigits(r->x + 0xE4, r->y + 0x17, z, buf, 0);
    rr.x = r->x + 0x8E;
    rr.y = r->y + 0x25;
    rr.w = 0x9C;
    rr.h = 0x15;
    text_drawBlock(rr.x + 8, rr.y + 3, z, (u8 *)&s->unk28, 0);
    func_8002CD4C(&rr, z, 0, s->unk48);
}

void menu_save_drawCursor(void) {
    u8 blink = (g_menu.unk10 & 3) != 0;

    if (!func_8002F810() && g_menuSave.unk2 == 2) {
        if (g_menuSave.unk1 == 1) {
            D_80077DE8.unk0 = 0x74;
            D_80077DE8.unk4 = 0x32;
            D_80077DE8.unk2 = g_menu.unk2 * 0x12 + 0x6D;
            func_800355E4();
        } else if (g_menuSave.unk1 == 4 || (g_menuSave.unk1 == 5 && blink)) {
            D_80077DE8.unk0 = 0;
            D_80077DE8.unk4 = 0x2A;
            D_80077DE8.unk2 = g_menuSave.unk24.cursor * 0x3C + 0x42;
            func_800355E4();
        } else if (g_menuSave.unk1 == 3 && blink) {
            ui_drawHand(0, g_menuSave.unk24.cursor * 0x3C + 0x42, 0x2A);
        }
    }
}

void menu_save_fileListTick(void) {
    MenuList *l = &g_menuSave.unk24;
    s32 v;
    s32 dec;

    if (g_menuSave.unk4[D_80077DB2] != 0) {
        menu_save_showInfo(0x2D);
        return;
    }
    if (l->unk8 != 0) {
        if (!(l->unk8 & 4)) {
            dec = l->unk8 & 2;
            l->unk9 += 6;
            if (l->unk9 >= 0x3C) {
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
            if (l->unk9 >= 0xB4) {
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
    if (g_menuSave.unk2 == 2 && g_menuSave.unk8 != 0) {
        func_80035580(NULL);
        st = g_menuSave.unk1;
        if (st == 1) {
            func_80035580(&r);
            v = 5;
            if (g_menuSave.unk0 == st) {
                v = 7;
            }
            D_80077DE8.unk8 = text_get(v + D_80077DB2);
        } else if (st == 4) {
            menu_save_setFileHelp();
        } else if (st == 3) {
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

    l = &g_menuSave.unk24;
    idx = l->unk5 + l->cursor;
    st = g_menuSave.unk34[idx].unk1A;
    switch (st) {
    case 1:
    case 2:
    case 3:
        D_80077DF0 = text_get(st + 9);
        break;
    case 4:
        g_menuSaveHelpBuf[0] = 0xFF;
        n = 0;
        m = g_menuSave.unk34[idx].unk58;
        for (i = 0; i < 4; i++) {
            if (m[i].unk0 != 0) {
                n = i + 1;
            }
        }
        for (i = 0; i < n; i++) {
            if (m[i].unk0 != 0) {
                func_8002BE44(i, m[i].unk2);
                func_8002BE5C(i, m[i].unkC);
                text_append(g_menuSaveHelpBuf, text_get(i + 0xD));
                if (i < n - 1) {
                    text_append(g_menuSaveHelpBuf, text_get(0x11));
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
    return g_menuSave.unk7;
}

u8 menu_save_isPortReady(s32 slot, u8 any) {
    s32 st = g_menuSave.unk4[slot];
    s32 r;

    if (any) {
        r = 0;
        if (st == 0 || st == 3) {
            r = 1;
        }
        return r;
    }
    return st == 0;
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
    text_drawBlock(g_menuSaveTitleRect.x + 0xA, g_menuSaveTitleRect.y + 6, 4, (u8 *)text_getFrom(g_saveMenuText, g_menuSave.unk0), 0);
}

void menu_save_draw(void) {
    void (*fn)(void) = g_menuSaveDrawFns[g_menuSave.unk1];

    if (fn != NULL) {
        fn();
    }
    if (g_menuSave.unk8 != 0) {
        menu_save_drawCursor();
    }
}

void menu_save_input(void) {
    if (!func_8002F810()) {
        switch (g_menuSave.unk2) {
        case 1:
            menu_save_messageInput();
            break;
        case 2:
            if (g_menuSaveInputFns[g_menuSave.unk1] != NULL) {
                g_menuSaveInputFns[g_menuSave.unk1]();
            }
            break;
        }
    }
}

void menu_save_tick(void) {
    menu_save_cardPoll();
    g_menuSave.unk4[0] = menu_save_getCardStatus(0);
    g_menuSave.unk4[1] = menu_save_getCardStatus(0x10);
    if (g_menuSave.unk2 == 2) {
        if (g_menuSaveTickFns[g_menuSave.unk1] != NULL) {
            g_menuSaveTickFns[g_menuSave.unk1]();
        }
    }
    menu_save_tickCardAnim();
    menu_save_updateHelpVisibility();
}

void menu_save_fileListInput(void) {
    MenuList *l = &g_menuSave.unk24;

    if (l->unk8 == 0) {
        if (g_padRepeat & BTN_CIRCLE) {
            snd_playSe(0x65);
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

    menu_save_listGetPageView(&g_menuSave.unk24, &v);
    r.x = 8;
    r.w = 0x12C;
    r.y = 0;
    r.h = 0x3C;
    for (i = 0; i < v.unk0; i++) {
        r.y = i * 0x3C + (s16)(v.unk2 + 0x2A);
        menu_save_drawFile(&r, 0x2D, v.unk1 + i);
        func_8002DEE4(&r, 0x2D, 0);
    }
}

void menu_save_drawScrollArrows(void) {
    s32 blink = (((u32)D_80077DC0 >> 4) ^ 1) & 1;
    MenuList *l = &g_menuSave.unk24;

    ui_drawSprite(0x12E, 0x2C, 0x2A, 0xB8, l->unk5 != 0 ? blink : 1);
    ui_drawSprite(0x12E, 0xCF, 0x2A, 0xB9, l->unk5 < l->unk2 - l->unk3 ? blink : 1);
}

void menu_save_drawEmptyFile(RECT *r, s32 otz) {
    text_drawBlock(r->x + 0x10, r->y + 0x16, otz, text_get(0x13), 0);
}

void menu_save_drawOtherFile(RECT *r, s32 otz) {
    text_drawBlock(r->x + 0x10, r->y + 0x16, otz, text_get(0x14), 0);
}

void menu_save_drawCorruptFile(RECT *r, s32 otz) {
    text_drawBlock(r->x + 0x10, r->y + 0x16, otz, text_get(0x15), 0);
}

void menu_save_selectPort(void) {
    if (!menu_save_isPortReady_inline(D_80077DB2, 1)) {
        snd_playSe(0x66);
        menu_save_showInfo(0x2D);
        return;
    }
    snd_playSe(0x67);
    menu_save_startScan();
}

void menu_save_selectFile(void) {
    s32 i = g_menuSave.unk24.unk5 + g_menuSave.unk24.cursor;

    if (g_menuSave.unk0 == 1) {
        menu_save_selectLoadFile(i, i);
        return;
    }
    menu_save_selectSaveFile(i, i);
}

void menu_save_updateHelpVisibility(void) {
    u8 hide;

    hide = g_menuSave.unk7 != 0;
    if (g_menuSave.unk2 != 2) {
        hide = 1;
    }
    if (g_menuSave.unk1 != 1 && g_menuSave.unk1 != 4 && g_menuSave.unk1 != 3) {
        hide = 1;
    }
    func_8003563C(hide);
}

void menu_save_returnToPorts(void) {
    g_menuSave.unk1 = 1;
    g_menuSave.unk20 = text_get(0x16);
    func_8002F9D0();
}

void func_801F5A60(void) {
}

void menu_save_drawCardIcon(s32 x, s32 y, s32 z, s32 idx) {
    u32 clut;
    Ovl39Sprite *e;

    clut = GetClut(0, 0xE0);
    setlen(g_primPtr, 4);
    e = &g_menuSaveCardIconFrames[idx];
    ((SprtWords *)g_primPtr)->rgbc = 0x64808080;
    ((SprtWords *)g_primPtr)->xy = x | (y << 16);
    ((SprtWords *)g_primPtr)->uvclut = e->unk0 | (e->unk1 << 8) | (clut << 16);
    ((SprtWords *)g_primPtr)->wh = e->unk2 | (e->unk3 << 16);
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

    r.y = 0xE0;
    r.x = 0;
    r.w = 0x100;
    r.h = 1;
    StoreImage(&r, (u32 *)buf);
    DrawSync(0);
    LoadImage(&r, (u32 *)img);
    DrawSync(0);
    buf += 0x200;
    img += 0x200;
    r.x = 0x40;
    r.w = 0x58;
    r.y = 0x100;
    r.h = 3;
    for (i = 0; i < 11; i++) {
        if (i == 0) {
            StoreImage(&r, (u32 *)buf);
        } else {
            StoreImage(&r, (u32 *)src);
            src += 0x210;
        }
        DrawSync(0);
        LoadImage(&r, (u32 *)img);
        DrawSync(0);
        r.y += 3;
        img += 0x210;
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
    r.y = 0xE0;
    r.w = 0x100;
    r.h = 1;
    LoadImage(&r, (u32 *)buf);
    DrawSync(0);
    buf += 0x200;
    r.x = 0x40;
    r.w = 0x58;
    r.y = 0x100;
    r.h = 3;
    for (i = 0; i < 11; i++) {
        if (i == 0) {
            LoadImage(&r, (u32 *)buf);
        } else {
            LoadImage(&r, (u32 *)img);
            img += 0x210;
        }
        DrawSync(0);
        r.y += 3;
    }
}

void menu_save_tickCardAnim(void) {
    if (g_menuSaveCardAnim.unk0 != 0) {
        if (++g_menuSaveCardAnim.unk1 >= 8) {
            g_menuSaveCardAnim.unk1 = 0;
            if (++g_menuSaveCardAnim.unk2 >= 4) {
                g_menuSaveCardAnim.unk2 = 0;
            }
        }
    }
}

void menu_save_resetCardAnim(void) {
    g_menuSaveCardAnim.unk2 = 0;
    g_menuSaveCardAnim.unk1 = 0;
}

u8 menu_save_enableCardAnim(u8 v) {
    u8 old = g_menuSaveCardAnim.unk0;

    g_menuSaveCardAnim.unk0 = v;
    return old;
}

void menu_save_drawCardAnim(s32 x, s32 y, s32 otz) {
    menu_save_drawCardIcon(x, y, otz, g_menuSaveCardAnim.unk2);
}

void menu_save_openConfirm(s32 id, s32 yes, s32 no) {
    RECT r;
    u16 sz;
    s32 w, h;

    g_menuSave.unk1 = 3;
    g_menuSave.unk2 = 0;
    g_menuSaveConfirm.unk4 = (s32)text_get(id);
    sz = text_getInfo(id);
    w = (sz & 0xFFF) + 0x10;
    g_menuSaveConfirm.unk2 = sz;
    g_menuSaveConfirm.unk0 = 1;
    g_menuSaveConfirm.unk1 = 1;
    h = ((sz >> 12) + 2) * 15 + 0x18;
    g_menuSaveConfirm.unk10 = yes;
    g_menuSaveConfirm.unk12 = no;
    r.w = w;
    r.h = h;
    r.x = (g_gameState->unkC->disp.w - w) >> 1;
    r.y = (g_gameState->unkC->disp.h - h) >> 1;
    g_menuSaveConfirm.unk8 = func_8002DC54(&r, 0x19, 2);
    win_setOnOpened(g_menuSaveConfirm.unk8, menu_save_onConfirmOpened);
    win_setOnClosed(g_menuSaveConfirm.unk8, menu_save_onConfirmClosed);
    win_setDrawContent(g_menuSaveConfirm.unk8, menu_save_drawConfirm);
    win_setDrawOverlay(g_menuSaveConfirm.unk8, menu_save_drawConfirmCursor);
    func_8002E31C(g_menuSaveConfirm.unk8, 1);
    func_8002E304(g_menuSaveConfirm.unk8, D_8007048C);
}

void menu_save_drawConfirm(Window *w) {
    u32 a = text_getInfo(0x21);
    u32 b = text_getInfo(0x22) & 0xFFF;
    s32 d1 = 0x26;
    s32 d2 = 0x17;
    s16 m;

    if (b < (a & 0xFFF)) {
        b = a & 0xFFF;
    }
    text_drawBlock(w->unk0 + 8, w->unk2 + 8, w->unk18, (u8 *)g_menuSaveConfirm.unk4, 0);
    m = b;
    text_drawBlock(w->unk0 + ((s32)(w->unk4 - m) >> 1), (w->unk2 - d1) + w->unk6, w->unk18, text_get(0x21), 0);
    text_drawBlock(w->unk0 + ((s32)(w->unk4 - m) >> 1), (w->unk2 - d2) + w->unk6, w->unk18, text_get(0x22), 0);
}

void menu_save_drawConfirmCursor(Window *w) {
    u32 a = text_getInfo(0x21);
    u32 b = text_getInfo(0x22) & 0xFFF;
    s32 z;

    if (b < (a & 0xFFF)) {
        b = a & 0xFFF;
    }
    if (w->unk1C == 3) {
        D_80077DE8.unk0 = w->unk0 + ((s32)(w->unk4 - b) >> 1) - 0x1A;
        D_80077DE8.unk2 = w->unk2 + (s16)(w->unk6 + (s16)(g_menuSaveConfirm.unk1 * 15 - 0x26)) + 3;
        z = w->unk18;
        D_80077DE8.unk4 = z;
        ui_drawHand(D_80077DE8.unk0, D_80077DE8.unk2, z - 1);
    }
}

void menu_save_initConfirm(void) {
    g_menuSaveConfirm.unk0 = 0;
}

void menu_save_confirmInput(void) {
    if (g_menuSaveConfirm.unk8->unk1C == 3) {
        if (g_padRepeat & BTN_CROSS) {
            snd_playSe(0x67);
            win_close(g_menuSaveConfirm.unk8);
            g_menuSave.unk2 = 0;
            return;
        }
        if (g_padRepeat & BTN_CIRCLE) {
            snd_playSe(0x65);
            g_menuSaveConfirm.unk1 = 1;
            win_close(g_menuSaveConfirm.unk8);
            g_menuSave.unk2 = 0;
            return;
        }
        if (g_padRepeat & (PAD_UP | PAD_DOWN)) {
            snd_playSe(0x67);
            g_menuSaveConfirm.unk1 ^= 1;
        }
    }
}

void menu_save_confirmTick(void) {
    u8 st = g_menuSave.unk4[g_menu.unk2];

    if (st != 0 && st != 3) {
        win_setOnClosed(g_menuSaveConfirm.unk8, NULL);
        win_close(g_menuSaveConfirm.unk8);
        g_menuSaveConfirm.unk0 = 0;
        g_menuSaveConfirm.unk8 = NULL;
        menu_save_showInfo(0x2D);
    }
}

void menu_save_setConfirmCallback(void (*v)(Unk801FBEE0 *)) {
    g_menuSaveConfirm.unkC = v;
}

u8 menu_save_isConfirmActive(void) {
    return g_menuSaveConfirm.unk0;
}

u8 menu_save_setConfirmActive(u8 v) {
    u8 old = g_menuSaveConfirm.unk0;

    g_menuSaveConfirm.unk0 = v;
    return old;
}

u8 *menu_save_getConfirmHelp(void) {
    return text_get(g_menuSaveConfirm.unk1 == 0 ? g_menuSaveConfirm.unk10 : g_menuSaveConfirm.unk12);
}

void menu_save_onConfirmOpened(Window *w) {
    g_menuSave.unk2 = 2;
}

void menu_save_onConfirmClosed(Window *w) {
    g_menuSave.unk1 = 4;
    if (g_menuSaveConfirm.unk1 == 0) {
        g_menuSaveConfirm.unkC(&g_menuSaveConfirm);
    }
    g_menuSaveConfirm.unk0 = 0;
    g_menuSave.unk2 = 2;
}

void menu_save_startScan(void) {
    s32 sl;
    s32 i;

    g_menuSave.unk1 = 2;
    g_menuSave.unk6 = 0;
    g_menuSave.unk10 = 0;
    g_menuSave.unkC = 0;
    g_menuSaveScan.unk1 = 0;
    g_menuSaveScan.unk0 = 1;
    g_menuSaveScan.unk1C = func_8002DC54(&g_menuSaveScanRect, 0x1E, 2);
    g_menuSaveScan.unk18 = 0xFF;
    g_menuSaveScan.unk3 = 0;
    g_menuSaveScan.unk10 = 0;
    g_menuSaveScan.unk14 = 0;
    g_menuSaveScan.unkC = 0;
    g_menuSaveScan.unk8 = 0;
    g_menuSaveScan.unk2 = 0;
    for (i = 0; i < 4; i++) {
        g_menuSaveScan.unk4[i] = 0;
    }
    for (i = 0; i < 15; i++) {
        g_menuSave.unk34[i].unk1A = 1;
        g_menuSave.unk34[i].unk1B = i;
        g_menuSaveScan.unk2B4[i] = 0;
    }
    menu_save_resetCardAnim();
    menu_save_enableCardAnim(1);
    win_setDrawContent(g_menuSaveScan.unk1C, menu_save_drawScan);
    win_setOnOpened(g_menuSaveScan.unk1C, menu_save_onScanOpened);
    win_setOnClosed(g_menuSaveScan.unk1C, menu_save_onScanClosed);
    func_8002E304(g_menuSaveScan.unk1C, D_80070420);
    func_8002E31C(g_menuSaveScan.unk1C, 1);
    sl = g_menu.unk2;
    switch (g_menuSave.unk4[sl]) {
    case 3:
        g_menuSaveScan.unk1 = 1;
        if (menu_save_cardClearSync(g_menu.unk2 * 0x10) != 0) {
            menu_save_scanError(0x2E);
        }
        break;
    case 0:
        g_menuSaveScan.unk1 = 2;
        if (menu_save_cardLoad(g_menu.unk2 * 0x10, menu_save_onCardLoaded) == 0) {
            menu_save_scanError(0x2E);
        }
        break;
    case 2:
        menu_save_scanError(0x2D);
        break;
    case 1:
        menu_save_scanError(0x2E);
        break;
    }
}

void menu_save_onCardLoaded(void) {
    s32 r;

    r = menu_save_getCardResult(D_80077DB2 << 4);
    g_menuSaveScan.unk3++;
    g_menuSaveScan.unk4[r]++;
    if (g_menuSaveScan.unk3 == g_menuSaveScan.unk4[r]) {
        if (g_menuSaveScan.unk3 < g_menuSaveScanRetries[r]) {
            menu_save_retryScan(r);
            return;
        }
        switch (r) {
        case 0:
            menu_save_onCardFormatted();
            return;
        case 3:
            menu_save_onCardUnformatted();
            return;
        }
    }
    menu_save_scanError(0x2E);
}

u8 menu_save_readDir(void) {
    DirEnt *d;
    SaveName20 *p;
    s32 i;
    s32 j;
    s32 k;
    s32 n;

    g_menuSaveScan.unk8 = menu_save_countFiles();
    if (g_menuSaveScan.unk8 < 0) {
        menu_save_scanError(0x2E);
        return 0;
    }
    for (i = 0; i < g_menuSaveScan.unk8; i++) {
        d = &g_menuSaveScan.unk20[i];
        if (strncmp((char *)d, (char *)menu_save_getFilePrefix(), 0xC) == 0) {
            k = menu_save_parseFileNumber((Unk801F6D24 *)d);
            p = &g_menuSave.unk34[k].unk4.unk0;
            *p = d->unk0;
            g_menuSave.unk34[k].unk4.unk14[0] = 0;
            g_menuSave.unk34[k].unk1A = 4;
            g_menuSaveScan.unk10++;
        }
    }
    for (i = 0; i < g_menuSaveScan.unk8; i++) {
        d = &g_menuSaveScan.unk20[i];
        if (strncmp((char *)d, (char *)menu_save_getFilePrefix(), 0xC) != 0) {
            n = (g_menuSaveScan.unk20[i].unk18 + 0x1FFF) / 0x2000;
            for (j = 0; j < n; j++) {
                k = menu_save_findLastEmpty();
                p = &g_menuSave.unk34[k].unk4.unk0;
                *p = d->unk0;
                g_menuSave.unk34[k].unk4.unk14[0] = 0;
                g_menuSave.unk34[k].unk1A = 2;
                g_menuSaveScan.unkC++;
            }
        }
    }
    return menu_save_checkSpace();
}

/* Inlined copy of menu_save_readHeader */
static inline void menu_save_readHeader_inline(s32 slot) {
    s32 fd;

    g_menuSaveScan.unk2 = slot;
    fd = menu_save_openFile(D_80077DB2 << 4, (char *)g_menuSave.unk34[slot].unk4.unk0.v);
    g_menuSaveScan.unk18 = fd;
    if (fd < 0) {
        menu_save_scanError(0x2E);
        return;
    }
    menu_save_readAsync(fd, (s32)g_menuSaveScan.unk2D0, 0x180, menu_save_onHeaderRead);
}

void menu_save_onHeaderRead(void) {
    if (menu_save_closeFile(g_menuSaveScan.unk18) < 0) {
        menu_save_scanError(0x2E);
        return;
    }
    switch (menu_save_getCardResult(D_80077DB2 << 4)) {
    case 2:
        menu_save_scanError(0x2D);
        return;
    case 1:
    case 3:
        menu_save_scanError(0x2E);
        return;
    }
    menu_save_copyHeader(&g_menuSave.unk34[g_menuSaveScan.unk2], (SaveHeader *)&g_menuSaveScan.unk2D0[0x100]);
    if (++g_menuSaveScan.unk14 < g_menuSaveScan.unk10) {
        menu_save_readHeader_inline(menu_save_findUsed(g_menuSaveScan.unk14));
        return;
    }
    menu_save_finishScan();
}

void menu_save_enterFileList(void) {
    g_menuSave.unk2 = 2;
    g_menuSave.unk1 = 4;
    g_menuSave.unk20 = text_get(0x1A);
    func_8002DE10(g_menuSaveScan.unk1C);
    func_8002EDFC(&g_menuSave.unk24, 0xF, 3, 1, 0xB4);
    if (g_menuSave.unk34[g_menuSave.unk10].unk1A == 4) {
        g_menuSave.unk24.cursor = g_menuSave.unk34[g_menuSave.unk10].unk44;
        g_menuSave.unk24.unk5 = g_menuSave.unk10 - g_menuSave.unk24.cursor;
    }
    func_8002F9D0();
    func_8002F8A8(menu_save_onFileListFadedIn);
}

void menu_save_findNewest(void) {
    s32 i;

    g_menuSave.unkC = 0;
    g_menuSave.unk10 = 0;
    for (i = 0; i < 15; i++) {
        if (g_menuSave.unk34[i].unk1A == 4) {
            if (g_menuSave.unk34[i].unk0 != 0x30005) {
                g_menuSave.unk34[i].unk1A = 3;
            } else if (g_menuSave.unkC < g_menuSave.unk34[i].unk4C) {
                g_menuSave.unkC = g_menuSave.unk34[i].unk4C;
                g_menuSave.unk10 = i;
            }
        }
    }
}

void menu_save_initScan(void) {
    g_menuSaveScan.unk0 = 0;
}

void menu_save_scanTick(void) {
    void (*fn)(void);

    if (!menu_save_isPortReady(D_80077DB2, 1)) {
        menu_save_scanError(0x2D);
        return;
    }
    fn = g_menuSaveScanSteps[g_menuSaveScan.unk1];
    if (fn != NULL) {
        fn();
    }
}

u8 menu_save_isScanActive(void) {
    return g_menuSaveScan.unk0;
}

u8 menu_save_setScanActive(u8 v) {
    u8 old = g_menuSaveScan.unk0;

    g_menuSaveScan.unk0 = v;
    return old;
}

s32 menu_save_findLastEmpty(void) {
    s32 i;

    for (i = 0; i < 15; i++) {
        if (g_menuSave.unk34[14 - i].unk1A == 1) {
            return 14 - i;
        }
    }
    return -1;
}

s32 menu_save_findUsed(s32 n) {
    s32 found = 0;
    s32 i;

    for (i = 0; i < 15; i++) {
        if (g_menuSave.unk34[i].unk1A == 4) {
            found++;
            if (n < found) {
                return i;
            }
        }
    }
    return -1;
}

s32 menu_save_parseFileNumber(Unk801F6D24 *p) {
    return (p->unk12 - '0') * 10 + (p->unk13 - '0');
}

void menu_save_scanWaitClear(void) {
    if (menu_save_isCardBusy() == 0) {
        g_menuSaveScan.unk1++;
        if (menu_save_cardLoad(D_80077DB2 * 0x10, menu_save_onCardLoaded) == 0) {
            menu_save_scanError(0x2E);
        }
    }
}

void menu_save_onCardFormatted(void) {
    g_menuSave.unk6 = 1;
    if (menu_save_readDir()) {
        if (g_menuSaveScan.unk14 < g_menuSaveScan.unk10) {
            g_menuSaveScan.unk1 = 3;
            menu_save_readHeader(menu_save_findUsed(g_menuSaveScan.unk14));
            return;
        }
        menu_save_finishScan();
    }
}

void menu_save_onCardUnformatted(void) {
    g_menuSave.unk6 = 0;
    if (g_menuSave.unk0 == 1) {
        menu_save_scanInfo(0x30);
        return;
    }
    menu_save_finishScan();
}

void menu_save_retryScan(s32 mode) {
    if (mode == 3) {
        g_menuSaveScan.unk1 = 1;
        if (menu_save_cardClearSync(g_menu.unk2 * 0x10) == 0) {
            return;
        }
    } else if (menu_save_cardLoad(g_menu.unk2 * 0x10, menu_save_onCardLoaded) != 0) {
        return;
    }
    menu_save_scanError(0x2E);
}

void menu_save_drawScan(Window *w) {
    menu_save_drawCardAnim(w->unk0 + 0xE, w->unk2 + 0x12, w->unk18);
    text_drawBlock(w->unk0 + 0x4A, w->unk2 + 0xE, w->unk18, text_get(0x12), 0);
}

void menu_save_onScanOpened(Window *w) {
    g_menuSave.unk2 = 2;
    g_menuSave.unk20 = text_get(0x17);
}

void menu_save_onScanClosed(Window *w) {
}

s32 menu_save_checkSpace(void) {
    if (g_menuSave.unk0 != 1) {
        if (g_menuSaveScan.unkC >= 0xF) {
            menu_save_scanInfo(0x2F);
            return 0;
        }
    } else if (g_menuSaveScan.unk10 == 0) {
        menu_save_scanInfo(0x30);
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
    port = D_80077DB2 << 4;
    do {
        r = menu_save_readDirectory(port, (s32)&D_801FBF18);
        if (n != 0 && prev != r) {
            goto fail;
        }
        n++;
        prev = r;
    } while (n < 3);
    if (menu_save_cardInfoSync(port) == 0) {
        return r;
    }
fail:
    menu_save_scanError(0x2E);
    return -1;
}

void menu_save_finishScan(void) {
    g_menuSaveScan.unk1 = 4;
    g_menuSaveScan.unk0 = 0;
    menu_save_findNewest();
    if (!menu_save_isMessageActive()) {
        g_menuSave.unk2 = 0;
        func_8002F9F0();
        func_8002F8A8(menu_save_enterFileList);
        snd_playSe(0x414);
    }
}

void menu_save_onFileListFadedIn(void) {
    if (g_menuSave.unk2 != 1) {
        g_menuSave.unk2 = 2;
    }
}

void menu_save_readHeader(s32 slot) {
    s32 fd;

    g_menuSaveScan.unk2 = slot;
    fd = menu_save_openFile(D_80077DB2 << 4, (char *)g_menuSave.unk34[slot].unk4.unk0.v);
    g_menuSaveScan.unk18 = fd;
    if (fd < 0) {
        menu_save_scanError(0x2E);
        return;
    }
    menu_save_readAsync(fd, (s32)g_menuSaveScan.unk2D0, 0x180, menu_save_onHeaderRead);
}

void menu_save_copyHeader(SaveSlot *s, SaveHeader *h) {
    s32 i;

    s->unk0 = h->unk0;
    s->unk1E = h->unk6;
    s->unk28 = h->unk10;
    s->unk1E.v[7] = 0xFF;
    s->unk1C = h->unk4;
    s->unk1D = h->unk5;
    s->unk50 = h->unk2C;
    s->unk54 = h->unk30;
    s->unk48 = h->unk74;
    s->unk4C = h->unk78;
    s->unk44 = h->unk7C;
    for (i = 0; i < 4; i++) {
        s->unk58[i] = h->unk34[i];
    }
}

void menu_save_scanError(s32 a) {
    g_menuSaveScan.unk1 = 5;
    win_close(g_menuSaveScan.unk1C);
    g_menuSaveScan.unk1C = NULL;
    menu_save_showError(a);
}

void menu_save_scanInfo(s32 a) {
    g_menuSaveScan.unk1 = 5;
    win_close(g_menuSaveScan.unk1C);
    g_menuSaveScan.unk1C = NULL;
    menu_save_showInfo(a);
}

void menu_save_initSavedMsg(void) {
    g_menuSaveSavedMsg.unk0 = 0;
    g_menuSaveSavedMsg.unk8 = 0;
    g_menuSaveSavedMsg.unk4 = 0;
}

void menu_save_openSavedMsg(SaveMenu *m) {
    if (g_menuSaveSavedMsg.unk0 == 0) {
        g_menuSave.unk2 = 0;
        g_menuSaveSavedMsg.unk4 = 0;
        g_menuSaveSavedMsgRect.y = g_menuSave.unk24.cursor * 0x3C + 0x3F;
        g_menuSaveSavedMsg.unk8 = func_8002DC54(&g_menuSaveSavedMsgRect, 0x14, 2);
        win_setDrawContent(g_menuSaveSavedMsg.unk8, menu_save_drawSavedMsg);
        win_setOnOpened(g_menuSaveSavedMsg.unk8, menu_save_onSavedMsgOpened);
        win_setOnClosed(g_menuSaveSavedMsg.unk8, menu_save_onSavedMsgClosed);
        func_8002E31C(g_menuSaveSavedMsg.unk8, 1);
    }
}

void menu_save_savedMsgInput(void) {
    if (g_menuSaveSavedMsg.unk8->unk1C == 3 && (g_padRepeat & (BTN_CIRCLE | BTN_CROSS))) {
        snd_playSe(0x65);
        g_menuSave.unk2 = 0;
        win_close(g_menuSaveSavedMsg.unk8);
    }
}

void menu_save_savedMsgTick(void) {
    if (!menu_save_isPortReady(g_menu.unk2, 0)) {
        func_8002DE10(g_menuSaveSavedMsg.unk8);
        menu_save_showInfo(0x2D);
        return;
    }
    if (++g_menuSaveSavedMsg.unk4 >= 0x40) {
        g_menuSave.unk2 = 0;
        win_close(g_menuSaveSavedMsg.unk8);
    }
}

void menu_save_onSavedMsgOpened(Window *w) {
    g_menuSave.unk2 = 2;
    g_menuSave.unk1 = 6;
}

void menu_save_onSavedMsgClosed(Window *w) {
    g_menuSave.unk2 = 2;
    g_menuSave.unk1 = 4;
    g_menuSaveSavedMsg.unk8 = 0;
    g_menuSaveSavedMsg.unk4 = 0;
    g_menuSave.unk20 = text_get(0x1A);
}

void menu_save_drawSavedMsg(Window *w) {
    u8 *s = text_get(0x2C);

    text_drawBlock(w->unk0 + ((w->unk4 - func_8002BA68((s32)s)) >> 1), w->unk2 + 8, w->unk18, s, 0);
}

void menu_save_openMessage(s32 id, s32 title) {
    Unk801FC358 *m = &g_menuSaveMessage;
    RECT r;
    s32 i;
    u16 sz;
    s32 w, h;

    if (m->unk0 == 0) {
        snd_playSe(0x416);
        g_menuSave.unk2 = 0;
        for (i = 14; i >= 0; i--) {
            g_menuSave.unk34[i].unk1A = 0;
        }
        m->unk0 = 1;
        m->unk8 = text_get(id);
        sz = text_getInfo(id);
        w = (sz & 0xFFF) + 0x10;
        m->unk2 = sz;
        h = (sz >> 12) * 15 + 0x10;
        r.w = w;
        r.h = h;
        r.x = (g_gameState->unkC->disp.w - w) >> 1;
        r.y = (g_gameState->unkC->disp.h - h) >> 1;
        m->unk4 = func_8002DC54(&r, 0xF, 2);
        win_setDrawContent(m->unk4, menu_save_drawMessage);
        win_setOnOpened(m->unk4, menu_save_onMessageOpened);
        win_setOnClosed(m->unk4, menu_save_onMessageClosed);
        func_8002E31C(m->unk4, 1);
        func_8002E304(m->unk4, title);
    }
}

void menu_save_initMessage(void) {
    g_menuSaveMessage.unk0 = 0;
}

void menu_save_messageInput(void) {
    if (g_menuSaveMessage.unk4->unk1C == 3 && (g_padRepeat & (BTN_CIRCLE | BTN_CROSS))) {
        snd_playSe(0x65);
        g_menuSave.unk2 = 0;
        win_close(g_menuSaveMessage.unk4);
    }
}

void menu_save_showError(s32 a) {
    menu_save_openMessage(a, D_80070440);
}

void menu_save_showInfo(s32 a) {
    menu_save_openMessage(a, D_8007048C);
}

u8 menu_save_setMessageActive(u8 v) {
    u8 old = g_menuSaveMessage.unk0;

    g_menuSaveMessage.unk0 = v;
    return old;
}

u8 menu_save_isMessageActive(void) {
    return g_menuSaveMessage.unk0;
}

void menu_save_drawMessage(Window *w) {
    text_drawBlock(w->unk0 + 8, w->unk2 + 8, w->unk18, g_menuSaveMessage.unk8, 0);
}

void menu_save_onMessageOpened(Window *w) {
    g_menuSave.unk2 = 1;
}

void menu_save_onMessageClosed(Window *w) {
    g_menuSave.unk2 = 2;
    g_menuSaveMessage.unk4 = NULL;
    g_menuSaveMessage.unk0 = 0;
    if (g_menuSaveResetAfterMsg[g_menuSave.unk1] != 0) {
        g_menuSave.unk2 = 0;
        func_8002F9F0();
        func_8002F8A8(menu_save_resetToPorts);
        return;
    }
    g_menuSave.unk20 = text_get(0x16);
    g_menuSave.unk1 = 1;
}

void menu_save_resetToPorts(void) {
    g_menuSave.unk20 = text_get(0x16);
    g_menuSave.unk1 = 1;
    g_menuSave.unk2 = 2;
    func_8002F9D0();
    func_8002F8A8(menu_save_onPortsFadedIn);
}

void menu_save_onPortsFadedIn(void) {
    g_menuSave.unk2 = 2;
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
        return 0xFF;
    }
    m = func_80028954(c->unk34.b.id);
    if (m == 0xFF) {
        return 0xFF;
    }
    if (m == 2 && (s32)(c->unk34.b.unk4) >= 5) {
        return 3;
    }
    return g_menuSavePortraitIds[m];
}

u8 menu_save_startAccess(s32 mode) {
    g_menuSave.unk1 = 5;
    g_menuSave.unk2 = 0;
    g_menuSaveAccess.unk0 = 1;
    g_menuSaveAccess.unk2 = 0;
    g_menuSaveAccess.unk3 = mode;
    g_menuSaveAccess.unk24 = 0;
    g_menuSaveAccess.unk4 = 0;
    g_menuSaveAccess.unk5 = 0;
    g_menuSaveAccess.unk1 = 0;
    menu_save_resetCardAnim();
    menu_save_resetReadProgress();
    menu_save_resetWriteProgress();
    g_menuSaveAccess.unk1C = func_8002DC54(&g_menuSaveAccessRect, 0x1E, 2);
    win_setDrawContent(g_menuSaveAccess.unk1C, menu_save_drawAccess);
    win_setOnOpened(g_menuSaveAccess.unk1C, menu_save_onAccessOpened);
    win_setOnClosed(g_menuSaveAccess.unk1C, menu_save_onAccessClosed);
    func_8002E31C(g_menuSaveAccess.unk1C, 1);
    func_8002E304(g_menuSaveAccess.unk1C, D_800703FC.unk0[g_menuSaveAccessTitles[mode]]);
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
    Unk801FC388 *p;
    SaveMenu *m;
    s32 t;
    s32 se;

    switch (g_menuSaveAccess.unk2) {
    case 1:
        p = &g_menuSaveAccess;
        if (++p->unk24 >= 4) {
            m = &g_menuSave;
            p->unk24 = 0;
            if (m->unk6 != 0) {
                t = 0x1D;
                se = 4;
            } else {
                t = 0x19;
                se = 2;
                m->unk8 = 0;
            }
            p->unk2 = se;
            m->unk20 = text_get(t);
        }
        break;
    case 2:
        if (++g_menuSaveAccess.unk24 >= 4) {
            g_menuSaveAccess.unk24 = 0;
            if (menu_save_formatCard()) {
                g_menuSaveAccess.unk2 = 3;
            }
        }
        break;
    case 3:
        break;
    case 4:
        if (++g_menuSaveAccess.unk24 >= 4) {
            g_menuSaveAccess.unk24 = 0;
            if (menu_save_writeFile()) {
                g_menuSaveAccess.unk2 = 5;
            }
        }
        break;
    case 5:
        break;
    case 6:
        break;
    case 7:
        break;
    }
}

void menu_save_loadTick(void) {
    switch (g_menuSaveAccess.unk2) {
    case 1:
        if (++g_menuSaveAccess.unk24 >= 4) {
            g_menuSaveAccess.unk2 = 4;
            g_menuSaveAccess.unk24 = 0;
            g_menuSave.unk20 = text_get(0x1E);
        }
        break;
    case 4:
        if (++g_menuSaveAccess.unk24 >= 4) {
            g_menuSaveAccess.unk24 = 0;
            if (menu_save_readFile()) {
                g_menuSaveAccess.unk2 = 5;
            }
        }
        break;
    case 5:
        break;
    }
}

void menu_save_drawAccess(Window *w) {
    RECT *r;
    s32 st;
    s32 n;

    r = (RECT *)w;
    if (g_menuSave.unk6 != 0) {
        if (g_menuSave.unk0 != 1) {
            st = menu_save_getWriteProgress();
        } else {
            st = menu_save_getReadProgress();
        }
        g_menuSaveAccess.unk5 = st == g_menuSaveAccess.unk4 ? g_menuSaveAccess.unk5 + 1 : 0;
        n = g_menuSaveAccess.unk5 * 2;
        if (n >= 6) {
            n = 5;
        }
        menu_save_drawCardAnim(r->x + 0xE, r->y + 0x12, w->unk18);
        text_drawBlock(r->x + 0x4A, r->y + 0xE, w->unk18, text_get(0x12), 0);
        menu_save_drawGauge(r->x + 0x1A, r->y + 0x44, w->unk18, st * 5 + n, 0xC8);
        g_menuSaveAccess.unk4 = st;
        return;
    }
    text_drawBlock(r->x + 0x26, r->y + 0xC, w->unk18, text_get(0x2A), 0);
    text_drawBlock(r->x + 0x32, r->y + 0x20, w->unk18, text_get(0x12), 0);
}

u8 menu_save_writeFile(void) {
    SaveSlot *sl;
    s32 port;
    s32 fd;

    port = D_80077DB2 << 4;
    sl = &g_menuSave.unk34[g_menuSave.unk3];
    if ((u32)(sl->unk1A - 3) >= 2) {
        strcpy((char *)g_menuSaveAccess.unk6.unk0.v, menu_save_getFilePrefix());
        strcat(g_menuSaveAccess.unk6.unk0.v, "00000-");
        g_menuSaveAccess.unk6.unk0.v[0x12] = g_menuSave.unk3 / 10 + '0';
        g_menuSaveAccess.unk6.unk0.v[0x13] = g_menuSave.unk3 % 10 + '0';
        /* called before its definition without a prototype: int result */
        if (!((s32 (*)(s32, char *))menu_save_createFile)(port, (char *)g_menuSaveAccess.unk6.unk0.v)) {
            menu_save_abortAccess(0x33);
            return 0;
        }
    } else {
        strcpy((char *)g_menuSaveAccess.unk6.unk0.v, sl->unk4.unk0.v);
    }
    fd = menu_save_openFile(port, (char *)g_menuSaveAccess.unk6.unk0.v);
    g_menuSaveAccess.unk20 = fd;
    if (fd < 0) {
        menu_save_abortAccess(0x33);
        return 0;
    }
    menu_save_writeAsync(fd, 0x801FCB00, 0x1400, menu_save_onFileIo);
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
    g_menuSaveAccess.unk0 = 0;
}

void menu_save_accessTick(void) {
    if (!menu_save_isPortReady(g_menu.unk2, 0)) {
        menu_save_abortAccess(0x2D);
        return;
    }
    if (g_menuSave.unk2 != 1) {
        if (g_menuSaveAccess.unk3 == 0) {
            menu_save_saveTick();
        } else {
            menu_save_loadTick();
        }
    }
}

u8 menu_save_isAccessActive(void) {
    return g_menuSaveAccess.unk0;
}

u8 menu_save_setAccessActive(u8 v) {
    u8 old = g_menuSaveAccess.unk0;

    g_menuSaveAccess.unk0 = v;
    return old;
}

u8 *menu_save_getFilePrefix(void) {
    memcpy(g_menuSaveFilePrefix, g_menuSaveProductCode, 13);
    return g_menuSaveFilePrefix;
}

void menu_save_onAccessOpened(Window *w) {
    g_menuSave.unk2 = 2;
    g_menuSaveAccess.unk2 = 1;
}

void menu_save_onAccessClosed(Window *w) {
    g_menuSaveAccess.unk0 = 0;
    g_menuSave.unk2 = 2;
    if (g_menuSave.unk0 != 1) {
        menu_save_openSavedMsg(&g_menuSave);
        return;
    }
    g_menuSave.unk1 = 4;
}

u8 menu_save_readFile(void) {
    SaveSlot *sl;
    s32 port;
    s32 fd;

    port = D_80077DB2 << 4;
    sl = &g_menuSave.unk34[g_menuSave.unk3];
    strcpy((char *)g_menuSaveAccess.unk6.unk0.v, sl->unk4.unk0.v);
    fd = menu_save_openFile(port, (char *)g_menuSaveAccess.unk6.unk0.v);
    g_menuSaveAccess.unk20 = fd;
    if (fd < 0) {
        menu_save_abortAccess(0x34);
        return 0;
    }
    menu_save_readAsync(fd, 0x801FCB00, 0x1400, menu_save_onFileIo);
    return 1;
}

u8 menu_save_formatCard(void) {
    if (menu_save_cardFormat(g_menu.unk2 * 0x10) == 0) {
        menu_save_abortAccess(0x32);
        return 0;
    }
    if (menu_save_cardLoad(g_menu.unk2 * 0x10, menu_save_onFormatDone) != 0) {
        return 1;
    }
    menu_save_abortAccess(0x33);
    return 0;
}

void menu_save_onFileIo(void) {
    s32 n = menu_save_getCardResult(g_menu.unk2 * 0x10);

    if (menu_save_closeFile(g_menuSaveAccess.unk20) < 0) {
        menu_save_abortAccess(g_menuSave.unk0 == 1 ? 0x34 : 0x33);
        return;
    }
    if (n == 0) {
        menu_save_onFileIoDone();
        return;
    }
    menu_save_onFileIoFailed(n);
}

void menu_save_onFileIoDone(void) {
    u8 mode;

    win_close(g_menuSaveAccess.unk1C);
    g_menuSave.unk2 = 0;
    mode = g_menuSave.unk0;
    if (mode != 1) {
        menu_save_storeSavedFile(g_menuSave.unk3, (void *)0x801FCC00);
        snd_playSe(0x4ED);
        return;
    }
    if (menu_save_applyLoaded()) {
        g_menuSave.unk7 = mode;
        snd_playSe(0x4ED);
        if (g_menu.unk1 == 0) {
            func_800361CC(0);
            return;
        }
        menu_save_close();
    }
}

void menu_save_onFileIoFailed(s32 i) {
    if (i < 4) {
        if (i <= 0) {
            return;
        }
        menu_save_abortAccess(g_menuSave.unk0 != 1 ? 0x33 : 0x34);
    }
}

void menu_save_storeSavedFile(s32 slot, SaveHeader *h) {
    SaveSlot *s;
    SaveFileName *n;
    s32 i;

    s = &g_menuSave.unk34[slot];
    n = &g_menuSaveAccess.unk6;
    s->unk4 = *n;
    s->unk1A = 4;
    s->unk1C = h->unk4;
    s->unk1D = h->unk5;
    s->unk1E = h->unk6;
    s->unk28 = h->unk10;
    s->unk50 = h->unk2C;
    s->unk54 = h->unk30;
    s->unk48 = h->unk74;
    s->unk4C = h->unk78;
    s->unk44 = h->unk7C;
    for (i = 0; i < 4; i++) {
        s->unk58[i] = h->unk34[i];
    }
}

void menu_save_abortAccess(s32 msg) {
    g_menuSave.unk8 = 1;
    menu_save_closeFile(g_menuSaveAccess.unk20);
    win_setOnClosed(g_menuSaveAccess.unk1C, menu_save_onAccessAborted);
    win_close(g_menuSaveAccess.unk1C);
    menu_save_showError(msg);
}

void menu_save_onFormatDone(void) {
    s32 r = menu_save_getCardResult();

    g_menuSave.unk8 = 1;
    if (r == 0) {
        snd_playSe(0x414);
        g_menuSave.unk6 = 1;
        g_menuSaveAccess.unk2 = 4;
        g_menuSaveAccess.unk24 = 4;
        g_menuSave.unk20 = text_get(0x1D);
        return;
    }
    win_setOnClosed(g_menuSaveAccess.unk1C, menu_save_onAccessAborted);
    win_close(g_menuSaveAccess.unk1C);
    menu_save_showError(0x33);
}

void menu_save_onAccessAborted(Window *w) {
    g_menuSaveAccess.unk0 = 0;
    g_menuSave.unk2 = 2;
}

MenuView *menu_save_listGetView(MenuList *l, MenuView *v) {
    s32 d;
    u8 f;

    f = l->unk8;
    if (f & 4) {
        if (f & 2) {
            d = l->unkD - l->unk9;
            v->unk0 = (d + 0x3B) / 60;
            v->unk1 = l->unk5 + (l->unk3 - v->unk0);
            v->unk2 = (l->unk3 - v->unk0) * 60;
        } else {
            d = l->unkD - l->unk9;
            v->unk0 = (d + 0x3B) / 60;
            v->unk1 = l->unk5;
            v->unk2 = 0;
        }
    } else if (f & 2) {
        v->unk0 = l->unk3 + 1;
        v->unk1 = l->unk5 - 1;
        v->unk2 = l->unk9 - 60;
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
        v->unk0 = (l->unk9 + 0x3B) / 60;
        v->unk1 = l->unkC + (l->unk3 - v->unk0);
        v->unk2 = l->unk9 - v->unk0 * 60;
    } else {
        x = l->unk9;
        v->unk2 = l->unkD - x;
        v->unk0 = (x + 0x3B) / 60;
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

    if ((btn & 0x100000) && !(g_padHeld & BTN_R1) && l->unk5 != 0) {
        d = l->unkD;
        l->unk9 = d >> 2;
        l->unk8 = 7;
        l->unkE = (d - l->unk9 + 0xE) / 15;
        top = l->unk5 - l->unk3;
        if (top < 0) {
            top = 0;
        }
        l->unkC = top;
        snd_playSe(0x67);
    }
}

void menu_save_listPageDown(MenuList *l, s32 pad) {
    s32 h;
    s32 page;
    s32 top;
    s32 max;

    if ((pad & 0x200000) && l->unk5 < l->unk2 - l->unk3) {
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
        snd_playSe(0x67);
    }
}

void menu_save_buildHeader(u8 *buf) {
    SaveHeader *h;
    Unk8004AB64 *o;
    s32 t;
    s32 i;
    u8 n;

    h = (SaveHeader *)(buf + 0x100);
    t = game_getPlayTime();
    menu_save_buildCardHeader(buf, t);
    n = 0;
    o = func_8004AE00();
    if (o != NULL) {
        n = o->unk3B;
    }
    if (g_menuSave.unk0 == 2) {
        h->unk4 = func_800B5074();
    } else {
        h->unk4 = g_gameState->unkB;
    }
    h->unk4 = menu_save_getDiscBit();
    h->unk0 = 0x30005;
    h->unk5 = g_gameState->unk1C->characters[n].level;
    h->unk2C = t;
    h->unk30 = g_gameState->unk1C->gil;
    h->unk74 = (g_gameState->unk24 >> 18) & 1;
    h->unk78 = ++g_menuSave.unkC;
    h->unk7C = g_menuSave.unk24.cursor;
    h->unk6 = *(SaveName *)g_gameState->unk1C->characters[n].unk0;
    menu_save_getLocation((u8 *)&h->unk10);
    for (i = 0; i < 4; i++) {
        if (g_gameState->unk1C->party[i] != NULL) {
            h->unk34[i].unk0 = 1;
            h->unk34[i].unk1 = menu_save_getPortrait(g_gameState->unk1C->party[i]);
            h->unk34[i].unkC = g_gameState->unk1C->party[i]->level;
            *(Name10 *)h->unk34[i].unk2 = *(Name10 *)g_gameState->unk1C->party[i]->unk0;
        } else {
            h->unk34[i].unk0 = 0;
            h->unk34[i].unk1 = 0xFF;
            h->unk34[i].unkC = 0;
            memset(h->unk34[i].unk2, 0xFF, 10);
        }
    }
}

void menu_save_getLocation(u8 *dst) {
    u8 *name;
    u8 *s;

    if (g_menuSave.unk0 == 2) {
        name = text_get(func_800B5064() + 0x36);
    } else {
        name = g_gameState->unk1C->unk80C;
    }
    if (name == NULL) {
        s = text_get(0x20);
        memset(dst, 0xFF, 0x1C);
        memcpy(dst, s, func_8002BCC0(s));
    } else {
        memset(g_menuSaveLocationBuf, 0xFF, 0x26);
        memcpy(g_menuSaveLocationBuf, name, func_8002BCC0(name));
        g_menuSaveLocationBuf[0x1B] = 0xFF;
        memset(dst, 0xFF, 0x1C);
        memcpy(dst, g_menuSaveLocationBuf, func_8002BCC0(g_menuSaveLocationBuf));
    }
}

void menu_save_buildCardHeader(u8 *buf, s32 t) {
    u8 *title;
    IconTmpl *icon;
    s32 sec;
    s32 min;
    s32 hour;
    s32 n;

    *(u16 *)buf = 0x4353;
    buf[2] = 0x11;
    buf[3] = 1;
    title = buf + 4;
    memset(buf + 0x44, 0, 0x1C);
    sec = (u32)t / 60;
    min = sec / 60;
    hour = min / 60;
    min = min % 60;
    sec = sec % 60;
    if (hour >= 100) {
        hour %= 100;
    }
    memset(title, 0, 0x40);
    *(TitleTmpl *)title = g_menuSaveTitleTemplate;
    title[0x11] += (g_menuSave.unk3 + 1) / 10;
    n = g_menuSave.unk3 + 1;
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
    *(IconTmpl *)(buf + 0x60) = *icon;
}

void menu_save_selectSaveFile(s32 slot, s32 unused) {
    g_menuSave.unk3 = slot;
    if (g_menuSave.unk6 == 0) {
        snd_playSe(0x67);
        menu_save_setConfirmCallback(menu_save_doSave);
        menu_save_openConfirm(D_80077DB2 + 0x23, 0x25, 0x26);
        return;
    }
    switch (g_menuSave.unk34[g_menuSave.unk3].unk1A) {
    case 1:
    case 3:
        snd_playSe(0x67);
        menu_save_doSave();
        break;
    case 4:
        snd_playSe(0x67);
        menu_save_setConfirmCallback(menu_save_doSave);
        menu_save_openConfirm(0x27, 0x28, 0x29);
        break;
    case 2:
        snd_playSe(0x66);
        break;
    }
}

void menu_save_doSave(void) {
    menu_save_initCrcTable();
    menu_save_buildHeader(((SaveBuf *)0x801FCB00)->unk0);
    menu_save_packData(&((SaveBuf *)0x801FCB00)->unk180);
    ((SaveBuf *)0x801FCB00)->unk13FE = menu_save_crc16(((SaveBuf *)0x801FCB00)->unk0, 0x13FE);
    menu_save_startAccess(0);
}

void menu_save_packData(SaveData *d) {
    menu_save_packState(&d->unk0);
    menu_save_packEvents(&d->unk40);
    menu_save_packParty(&d->unk850);
    menu_save_packMisc(&d->unkD88);
    menu_save_packItems(&d->unkDA0);
    menu_save_packCards(d->unkFF8);
}

s32 menu_save_getDiscBit(void) {
    if ((u16)g_evtGlobalVars.h[0] < 0xF3C) {
        return 1;
    }
    if ((u16)g_evtGlobalVars.h[0] < 0x1B4E) {
        return 2;
    }
    if ((u16)g_evtGlobalVars.h[0] < 0x2AF3) {
        return 4;
    }
    return 8;
}

void menu_save_initCrcTable(void) {
    u16 i;
    u16 j;
    u16 c;

    for (i = 0; i < 256; i++) {
        c = i;
        for (j = 0; j < 8; j++) {
            if (c & 1) {
                c = (c >> 1) ^ 0x8408;
            } else {
                c >>= 1;
            }
        }
        g_menuSaveCrcTable[i] = c;
    }
}

u16 menu_save_crc16(u8 *p, s32 n) {
    Crc16 c = { 0xFFFF };

    while (--n >= 0) {
        c.w = g_menuSaveCrcTable[(c.w ^ *p++) & 0xFF] ^ (c.h >> 8);
    }
    return c.w;
}

void menu_save_selectLoadFile(s32 slot, s32 arg1) {
    SaveSlot *s;

    s = g_menuSave.unk34;
    g_menuSave.unk3 = slot;
    switch (s[slot].unk1A) {
    case 4:
        snd_playSe(0x67);
        menu_save_doLoad();
        break;
    case 1:
    case 2:
    case 3:
        snd_playSe(0x66);
        break;
    }
}

u8 menu_save_applyLoaded(void) {
    SaveHdr *h;
    u8 ret;

    ret = 1;
    menu_save_initCrcTable();
    if (D_801FDEFE == menu_save_crc16(((SaveBuf *)0x801FCB00)->unk0, 0x13FE)) {
        h = &((SaveBuf *)0x801FCB00)->unk100;
        g_gameState->unkB = h->unk4;
        game_setPlayTime(h->unk2C);
        menu_save_unpackState(&((SaveBuf *)0x801FCB00)->unk180.unk0);
        menu_save_unpackEvents(&((SaveBuf *)0x801FCB00)->unk180.unk40);
        menu_save_unpackParty(&((SaveBuf *)0x801FCB00)->unk180.unk850);
        menu_save_unpackMisc(&((SaveBuf *)0x801FCB00)->unk180.unkD88);
        menu_save_unpackItems(&((SaveBuf *)0x801FCB00)->unk180.unkDA0);
        menu_save_unpackCards(((SaveBuf *)0x801FCB00)->unk180.unkFF8);
    } else {
        menu_save_showError(0x35);
        ret = 0;
    }
    return ret;
}

void menu_save_doLoad(void) {
    if (!menu_save_startAccess(1)) {
        menu_save_showError(0x34);
    }
}

void menu_save_packParty(SaveParty *s) {
    GameData *gd;
    s32 i;

    gd = g_gameState->unk1C;
    for (i = 0; i < 9; i++) {
        s->unk0[i] = gd->characters[i];
    }
    for (i = 0; i < 4; i++) {
        if (gd->party[i] != NULL) {
            s->unk510[i] = gd->party[i]->unk34.b.id;
        } else {
            s->unk510[i] = 9;
        }
    }
    s->unk514 = gd->unk55C;
    s->unk516 = gd->firstUseFxSeen;
    s->unk518 = gd->gil;
    s->unk51C = gd->unk568;
    s->unk51E = gd->unk56A;
    s->unk520 = gd->unk56C;
}

void menu_save_unpackParty(SaveParty *s) {
    GameData *gd;
    s32 i;

    gd = g_gameState->unk1C;
    for (i = 0; i < 9; i++) {
        gd->characters[i] = s->unk0[i];
    }
    for (i = 0; i < 4; i++) {
        if (s->unk510[i] < 9) {
            gd->party[i] = &gd->characters[s->unk510[i]];
        } else {
            gd->party[i] = NULL;
        }
    }
    gd->unk55C = s->unk514;
    gd->firstUseFxSeen = s->unk516;
    gd->gil = s->unk518;
    gd->unk568 = s->unk51C;
    gd->unk56A = s->unk51E;
    gd->unk56C = s->unk520;
}

void menu_save_packState(Unk801F9BEC *h) {
    GameData *gd;
    s32 i;

    gd = g_gameState->unk1C;
    h->unk0 = g_gameState->mode;
    h->unk1 = g_gameState->unkA;
    for (i = 0x1D; i >= 0; i--) {
        h->unk2[i] = 0;
    }
    if (g_menuSave.unk0 == 2) {
        h->unk20 = func_800B5084();
        h->unk22 = func_800B5094();
    } else {
        h->unk20 = gd->fieldId;
        h->unk22 = gd->unk3A;
    }
    h->unk24 = gd->battleId;
    h->unk26 = (s8)gd->unk848;
    h->unk28 = gd->unk7DC;
    h->unk2A = gd->unk7DE;
    for (i = 0x13; i >= 0; i--) {
        h->unk2C[i] = 0;
    }
}

void menu_save_unpackState(Unk801F9BEC *p) {
    GameState *gs = g_gameState;
    GameData *gd;

    gs->mode = p->unk0;
    gd = gs->unk1C;
    g_gameState->unkA = p->unk1;
    gd->fieldId = p->unk20;
    gd->unk3A = p->unk22;
    gd->battleId = p->unk24;
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
    d->unk0 = g_evtGlobalVars;
    d->unk800 = D_8007AEC4;
}

void menu_save_unpackEvents(SaveEvents *s) {
    g_evtGlobalVars = s->unk0;
    D_8007AEC4 = s->unk800;
}

void menu_save_packItems(SaveItems *s) {
    s->unk0 = g_gameState->unk1C->items;
    s->unk200 = g_gameState->unk1C->unk774;
    s->unk240 = *(GameStateUnk24 *)&g_gameState->unk24;
}

void menu_save_unpackItems(SaveItems *s) {
    g_gameState->unk1C->items = s->unk0;
    g_gameState->unk1C->unk774 = s->unk200;
    *(GameStateUnk24 *)&g_gameState->unk24 = s->unk240;
    func_8002397C();
}

void menu_save_packMisc(Unk801FA078 *p) {
    p->unk0 = g_gameState->unk1C->unk844;
    p->unk4 = g_gameState->unk1C->unk880;
    p->unk14 = g_gameState->unk1C->unk892;
}

void menu_save_unpackMisc(Unk801FA078 *p) {
    g_gameState->unk1C->unk844 = p->unk0;
    g_gameState->unk1C->unk880 = p->unk4;
    g_gameState->unk1C->unk892 = p->unk14;
}

void menu_save_cardInit(void) {
    g_menuSaveCardOp = 0;
    g_menuSaveCardInfoChan = 0;
    g_menuSaveCardInfoBusy = 0;
    g_menuSaveInfoCallbacks = 0;
    D_801FB0A0 = 0;
}

INCLUDE_RODATA("asm/us/ovl_39/nonmatchings/ovl_39", g_menuSaveProductCode);

INCLUDE_RODATA("asm/us/ovl_39/nonmatchings/ovl_39", g_menuSaveTitleTemplate);

s32 menu_save_firstFile(s32 mode, s32 chan) {
    char path[16];

    if (mode == 0) {
        strcpy(path, "bu00:*");
    } else {
        strcpy(path, "bu10:*");
    }
    _card_wait(0);
    _card_wait(1);
    menu_save_cardIdle();
    return firstfile(path, chan);
}

s32 menu_save_nextFile(s32 chan) {
    _card_wait(0);
    _card_wait(1);
    menu_save_cardIdle();
    return nextfile(chan);
}

s32 menu_save_readDirectory(s32 mode, s32 chan) {
    s32 n = 0;

    if (menu_save_firstFile(mode, chan) == chan) {
        do {
            n++;
            chan += 0x28;
        } while (menu_save_nextFile(chan) == chan);
    }
    return n;
}

s32 menu_save_getCardStatus(s32 i) {
    if (i == 0) {
        return g_menuSaveCardStatus;
    }
    return D_801FB08C;
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
    return g_menuSaveCardOp != 0;
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

s32 menu_save_cardLoad(s32 a, void (*b)(void)) {
    s32 r = menu_save_cardLoadStart(a);

    g_menuSaveCardCallback = b;
    return r;
}

s32 menu_save_cardLoadStart(s32 chan) {
    g_menuSaveCardCallback = NULL;
    g_menuSaveCardChan = chan;
    _card_wait(0);
    _card_wait(1);
    menu_save_clearSwCard();
    g_menuSaveCardOp = 1;
    return _card_load(chan);
}

s32 menu_save_openFile(s32 port, char *name) {
    char path[32];

    _card_wait(0);
    _card_wait(1);
    g_menuSaveFileChan = port;
    if (port == 0) {
        strcpy(path, "bu00:");
    } else {
        strcpy(path, "bu10:");
    }
    strcat((u8 *)path, (u8 *)name);
    menu_save_cardIdle();
    return open(path, 0x8003);
}

s32 menu_save_closeFile(s32 chan) {
    _card_wait(0);
    _card_wait(1);
    menu_save_cardIdle();
    return close(chan);
}

s32 menu_save_writeAsync(s32 a, s32 b, s32 c, void (*d)(void)) {
    s32 r = menu_save_writeStart(a, b, c);

    g_menuSaveCardCallback = d;
    return r;
}

s32 menu_save_writeStart(s32 a, s32 b, s32 size) {
    g_menuSaveCardCallback = NULL;
    g_menuSaveFd = a;
    g_menuSaveSectorsDone = 0;
    g_menuSaveIoBuf = b;
    g_menuSaveSectorsLeft = (size + 0x7F) / 0x80 - 1;
    menu_save_writeSector();
    return 1;
}

s32 menu_save_writeSector(void) {
    _card_wait(0);
    _card_wait(1);
    menu_save_clearSwCard();
    if (write(g_menuSaveFd, g_menuSaveIoBuf, 0x80) == -1) {
        return 0;
    }
    g_menuSaveCardOp = 5;
    return 1;
}

s32 menu_save_readAsync(s32 a, s32 b, s32 c, void (*d)(void)) {
    s32 r = menu_save_readStart(a, b, c);

    g_menuSaveCardCallback = d;
    return r;
}

s32 menu_save_readStart(s32 arg0, s32 arg1, s32 size) {
    g_menuSaveCardCallback = NULL;
    g_menuSaveFd = arg0;
    g_menuSaveSectorsDone = 0;
    g_menuSaveIoBuf = arg1;
    g_menuSaveSectorsLeft = (size + 0x7F) / 128 - 1;
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
    if (read(g_menuSaveFd, g_menuSaveIoBuf, 0x80) == -1) {
        return 0;
    }
    g_menuSaveCardOp = 4;
    return 1;
}

u8 menu_save_createFile(s32 port, char *name) {
    char path[32];
    s32 fd;
    s32 ok;

    _card_wait(0);
    _card_wait(1);
    if (port == 0) {
        strcpy(path, "bu00:");
    } else {
        strcpy(path, "bu10:");
    }
    strcat((u8 *)path, (u8 *)name);
    fd = open(path, 0x10200);
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
    case 0:
        if (g_menuSaveCardInfoBusy == 0) {
            menu_save_clearSwCard();
            g_menuSaveCardChan = g_menuSaveCardInfoChan = (g_menuSaveCardInfoChan + 0x10) & 0x10;
            _card_wait(0);
            _card_wait(1);
            _card_info(g_menuSaveCardChan);
            g_menuSaveCardInfoBusy = 1;
        }
        r = menu_save_testSwCard();
        switch (r) {
        case 0:
            menu_save_cardPoll_set(g_menuSaveCardChan, 0);
            break;
        case 1:
        case 3:
            menu_save_cardPoll_set(g_menuSaveCardChan, r);
            break;
        case 2:
            menu_save_cardPoll_retry(g_menuSaveCardChan, r);
            break;
        }
        if (r != -1) {
            if (g_menuSaveCardChan == 0) {
                if (g_menuSaveInfoCallbacks) {
                    ((void (*)(void))g_menuSaveInfoCallbacks)();
                }
                g_menuSaveInfoCallbacks = 0;
            } else {
                if (D_801FB0A0) {
                    ((void (*)(void))D_801FB0A0)();
                }
                D_801FB0A0 = 0;
            }
        }
        break;
    case 2:
        menu_save_testHwCard();
        break;
    case 1:
        r = menu_save_testSwCard();
        g_menuSaveCardResult = r;
        if (r == -1) {
            break;
        }
        goto callback;
    case 4:
        r = menu_save_testSwCard();
        g_menuSaveCardResult = r;
        if (r == -1) {
            break;
        }
        if (r != 0) {
            goto callback;
        }
        g_menuSaveCardResult = 0;
        lseek(g_menuSaveFd, 0x80, 1);
        g_menuSaveSectorsDone++;
        if (g_menuSaveSectorsLeft != 0) {
            g_menuSaveSectorsLeft--;
            g_menuSaveIoBuf += 0x80;
            menu_save_readSector();
            break;
        }
        goto callback;
    case 5:
        r = menu_save_testSwCard();
        g_menuSaveCardResult = r;
        if (r == -1) {
            break;
        }
        if (r != 0) {
            goto callback;
        }
        g_menuSaveSectorsDone++;
        if (g_menuSaveSectorsLeft != 0) {
            g_menuSaveSectorsLeft--;
            g_menuSaveIoBuf += 0x80;
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
    g_menuSaveCardOp = 0;
    g_menuSaveCardInfoBusy = 0;
}

s32 menu_save_testSwCard(void) {
    s32 r = -1;

    if (TestEvent(g_menuSaveSwEvents) == 1) {
        r = 0;
    } else if (TestEvent(D_801FC5F8) == 1) {
        r = 1;
    } else if (TestEvent(D_801FC5FC) == 1) {
        r = 2;
    } else if (TestEvent(D_801FC600) == 1) {
        r = 3;
    }
    if (r != -1) {
        menu_save_cardIdle();
    }
    return r;
}

s32 menu_save_testHwCard(void) {
    s32 r = -1;

    if (TestEvent(g_menuSaveHwEvents) == 1) {
        r = 0;
    } else if (TestEvent(D_801FC60C) == 1) {
        r = 1;
    } else if (TestEvent(D_801FC610) == 1) {
        r = 2;
    } else if (TestEvent(D_801FC614) == 1) {
        r = 3;
    }
    if (r != -1) {
        menu_save_cardIdle();
    }
    return r;
}

void menu_save_openCardEvents(void) {
    EnterCriticalSection();
    g_menuSaveSwEvents = OpenEvent(0xF4000001, 4, 0x2000, NULL);
    D_801FC5F8 = OpenEvent(0xF4000001, 0x8000, 0x2000, NULL);
    D_801FC5FC = OpenEvent(0xF4000001, 0x100, 0x2000, NULL);
    D_801FC600 = OpenEvent(0xF4000001, 0x2000, 0x2000, NULL);
    g_menuSaveHwEvents = OpenEvent(0xF0000011, 4, 0x2000, NULL);
    D_801FC60C = OpenEvent(0xF0000011, 0x8000, 0x2000, NULL);
    D_801FC610 = OpenEvent(0xF0000011, 0x100, 0x2000, NULL);
    D_801FC614 = OpenEvent(0xF0000011, 0x2000, 0x2000, NULL);
    EnableEvent(g_menuSaveSwEvents);
    EnableEvent(D_801FC5F8);
    EnableEvent(D_801FC5FC);
    EnableEvent(D_801FC600);
    EnableEvent(g_menuSaveHwEvents);
    EnableEvent(D_801FC60C);
    EnableEvent(D_801FC610);
    EnableEvent(D_801FC614);
    ExitCriticalSection();
}

void menu_save_clearSwCard(void) {
    TestEvent(g_menuSaveSwEvents);
    TestEvent(D_801FC5F8);
    TestEvent(D_801FC5FC);
    TestEvent(D_801FC600);
}

void menu_save_clearHwCard(void) {
    TestEvent(g_menuSaveHwEvents);
    TestEvent(D_801FC60C);
    TestEvent(D_801FC610);
    TestEvent(D_801FC614);
}

void menu_save_closeCardEvents(void) {
    EnterCriticalSection();
    CloseEvent(g_menuSaveSwEvents);
    CloseEvent(D_801FC5F8);
    CloseEvent(D_801FC5FC);
    CloseEvent(D_801FC600);
    CloseEvent(g_menuSaveHwEvents);
    CloseEvent(D_801FC60C);
    CloseEvent(D_801FC610);
    CloseEvent(D_801FC614);
    ExitCriticalSection();
}
