#include "ovl_32.h"

INCLUDE_RODATA("asm/us/ovl_32/nonmatchings/ovl_32", s_menuChocographIcons);

/* text bank for this menu (count + {offset, info} table, then strings) */
INCLUDE_RODATA("asm/us/ovl_32/nonmatchings/ovl_32", g_chocographText);

void menuChocograph_init(void) {
    ChocographMenu *st;

    if (g_gameState->unk1C->unk844 > CHOCOGRAPH_COUNT) {
        g_gameState->unk1C->unk844 = 0;
    }
    st = &s_menuChocograph;
    st->mode = CHOCO_MODE_TABS;
    s_menuChocograph.tab = CHOCO_TAB_SELECT;
    text_setBank(g_chocographText);
    help_setCallback(menuChocograph_updateHelp);
    help_setCursorOnTop(1);
    setRECT(&s_menuChocograph.titleRect, 0xE4, 8, 0x50, 0x1A);
    setRECT(&s_menuChocograph.tabRect, 8, 8, 0xD8, 0x1A);
    setRECT(&s_menuChocograph.selectedRect, 8, 0x26, 0xB7, 0x43);
    setRECT(&s_menuChocograph.listRect, 8, 0x6B, 0x86, 0x71);
    setRECT(&s_menuChocograph.countRect, 0xBF, 0x47, 0x75, 0x22);
    setRECT(&s_menuChocograph.abilityRect, 0xBF, 0x26, 0x75, 0x21);
    setRECT(&s_menuChocograph.infoRect, 0x8E, 0x6B, 0xA6, 0x71);
    menuList_init(&s_menuChocograph.list, CHOCOGRAPH_COUNT, CHOCO_LIST_ROWS, 1, 0x71);
    menuChocograph_readFlags();
    menuChocograph_loadImages();
    if (g_gameState->unk1C->unk844 != 0) {
        menuChocograph_scrollTo(g_gameState->unk1C->unk844 - 1);
    }
    s_menuChocograph.textPtr = s_menuChocograph.textBuf + (g_gameState->unk1C->unk8 << 12);
}

void menuChocograph_inputTabs(void) {
    u8 v;
    ChocographMenu *st;

    if (g_padRepeat & BTN_CIRCLE) {
        snd_playSe(SE_CANCEL);
        menu_close();
    } else if (g_padRepeat & BTN_CROSS) {
        if (s_menuChocograph.tab == CHOCO_TAB_SELECT) {
            s_menuChocograph.mode = CHOCO_MODE_LIST;
            snd_playSe(SE_CURSOR);
        } else if (g_gameState->unk1C->unk844 == 0) {
            snd_playSe(SE_BUZZER);
        } else {
            g_gameState->unk1C->unk844 = 0;
            snd_playSe(SE_CONFIRM);
        }
    } else {
        if (g_padRepeat & PAD_LEFT) {
            snd_playSe(SE_CURSOR);
            st = &s_menuChocograph;
            if (st->tab != 0) {
                v = st->tab - 1;
            } else {
                v = 1;
            }
        } else if (g_padRepeat & PAD_RIGHT) {
            snd_playSe(SE_CURSOR);
            st = &s_menuChocograph;
            if (st->tab == 0) {
                v = st->tab + 1;
            } else {
                v = 0;
            }
        } else {
            return;
        }
        st->tab = v;
    }
}

void menuChocograph_inputList(void) {
    ChocographMenu *st;
    MenuList *l = &s_menuChocograph.list;
    s32 idx;
    s32 n;

    if (l->scroll == 0) {
        if (g_padRepeat & BTN_CIRCLE) {
            snd_playSe(SE_CANCEL);
            s_menuChocograph.mode = CHOCO_MODE_TABS;
            return;
        }
        if (g_padRepeat & BTN_CROSS) {
            st = &s_menuChocograph;
            idx = l->top + l->cursor;
            if (st->owned & (1 << idx)) {
                n = st->ability;
                if (menuChocograph_getRequiredAbility(idx) < n) {
                    snd_playSe(SE_CONFIRM);
                    g_gameState->unk1C->unk844 = l->top + l->cursor + 1;
                    return;
                }
            }
            snd_playSe(SE_BUZZER);
            return;
        }
        menuList_input(l, g_padRepeat);
    }
}

void menuChocograph_drawSelected(void) {
    RECT *r = &s_menuChocograph.selectedRect;
    s32 sel;

    win_drawBox(r, 0x1D, 2);
    win_drawShadow(r, 0x1E, 2);
    ui_drawLabel(r->x + 8, r->y - 3, 0x1C, D_800704AC, 0);
    sel = g_gameState->unk1C->unk844;
    if (sel != 0) {
        menuChocograph_drawIcon(r->x + 0x10, r->y + 0x2C, 0x1C, (s_menuChocograph.found & (1 << (sel - 1))) ? CHOCO_ICON_CHOCOGRAPH + 1 : CHOCO_ICON_CHOCOGRAPH, 0);
        text_drawBlock(r->x + 0x21, r->y + 0x2E, 0x1C, text_get(g_gameState->unk1C->unk844 - 1 + CHOCO_TEXT_NAME), 0);
        menuChocograph_drawPicture(r->x + 0x4C, r->y + 0xB, 0x1C, g_gameState->unk1C->unk844 - 1, 0);
        menuChocograph_drawFrame(r->x + 0x48, r->y + 7, 0x1C, 0);
    }
}

void menuChocograph_drawList(void) {
    RECT sub;
    MenuView view;
    s32 i;
    RECT *r = &s_menuChocograph.listRect;
    MenuList *list;

    ui_drawLabel(r->x + 0x10, r->y - 3, 0x1C, D_80070428, 0);
    list = &s_menuChocograph.list;
    win_drawListBox(r, 0x1D, 1, list);
    win_drawShadow(r, 0x1D, 1);
    menuList_getView(list, &view);
    win_setClip(NULL, 0x1C, 1);
    for (i = 0; i < view.unk0; i++) {
        menuChocograph_drawRow(0, i, view.unk1 + i, &view);
    }
    if (list->scroll & 4) {
        menuList_getRowsRect(list, r, &sub);
        win_setClip(&sub, 0x1C, 1);
        menuChocograph_drawScrollRows();
    }
    win_setClip(r, 0x1C, 1);
}

void menuChocograph_drawCounts(void) {
    u8 buf[8];
    RECT *r = &s_menuChocograph.countRect;
    ChocographMenu *st;

    win_drawBox(r, 0x1D, 2);
    win_drawShadow(r, 0x1E, 2);
    text_drawBlock(r->x + 9, r->y + 3, 0x1C, text_get(CHOCO_TEXT_INVENTORY), 0);
    text_drawBlock(r->x + 9, r->y + 0xF, 0x1C, text_get(CHOCO_TEXT_FOUND), 0);
    st = &s_menuChocograph;
    text_formatNumber(st->ownedCount, buf, 2, 1);
    ui_drawSmallDigits(r->x + 0x5E, r->y + 7, 0x1C, buf, 0);
    text_formatNumber(st->foundCount, buf, 2, 1);
    ui_drawSmallDigits(r->x + 0x5E, r->y + 0x13, 0x1C, buf, 0);
}

void menuChocograph_drawHint(void) {
    RECT *r = &s_menuChocograph.infoRect;
    ChocographMenu *st = &s_menuChocograph;
    s32 id;
    s32 n;
    s32 i;

    id = st->list.top + st->list.cursor;
    ui_drawLabel(r->x + 8, r->y - 3, 0x1C, D_80070454, 0);
    win_drawBox(r, 0x1D, 1);
    win_drawShadow(r, 0x1E, 1);
    if (st->mode == CHOCO_MODE_LIST && (st->owned & (1 << id))) {
        menuChocograph_getRequiredAbility(id);
        menuChocograph_drawPicture(r->x + 0xC, r->y + 0xC, 0x1C, id, 0);
        menuChocograph_drawFrame(r->x + 8, r->y + 8, 0x1C, 0);
        st->textPtr = text_buildBlock(r->x + 0xC, r->y + 0x31, &g_ot[0x1C], text_get(id + CHOCO_TEXT_CLUE), 0, st->textPtr);
        text_drawBlock(r->x + 0x42, r->y + 6, 0x1C, text_get(CHOCO_TEXT_ABILITY), 0);
        n = menuChocograph_getRequiredAbility(id);
        for (i = 0; i <= n; i++) {
            menuChocograph_drawIcon(r->x + 0x42 + i * 0x13, r->y + 0x16, 0x1C, i < st->ability ? i : CHOCO_ICON_LOCKED, i >= st->ability);
        }
    }
}

void menuChocograph_drawCursor(void) {
    help_setCursorOnTop(s_menuChocograph.mode != CHOCO_MODE_TABS);
    if (!fade_isBusy()) {
        if (s_menuChocograph.mode == CHOCO_MODE_TABS || (D_80077DC0 & 3)) {
            g_menuHelp.y = 0x11;
            g_menuHelp.otz = 0x1B;
            g_menuHelp.x = s_menuChocograph.tab * 0x60 + 0x1C;
            help_drawCursor(&g_menuHelp);
        }
        if (s_menuChocograph.mode == CHOCO_MODE_LIST) {
            g_menuHelp.x = 0;
            g_menuHelp.otz = 0x1B;
            g_menuHelp.y = s_menuChocograph.list.cursor * 0xF + 0x74;
            help_drawCursor();
        }
    }
}

void menuChocograph_updateHelp(void) {
    s32 idx;

    g_menuHelp.text = NULL;
    help_setBounds(0);
    if (s_menuChocograph.mode == CHOCO_MODE_TABS) {
        g_menuHelp.text = text_get(s_menuChocograph.tab + CHOCO_TEXT_HELP_TAB);
    } else if (s_menuChocograph.mode == CHOCO_MODE_LIST) {
        idx = s_menuChocograph.list.top + s_menuChocograph.list.cursor;
        if (s_menuChocograph.owned & (1 << idx)) {
            text_copy(s_menuChocographHelpBuf, text_get(idx + CHOCO_TEXT_HINT));
            if (s_menuChocograph.ability <= menuChocograph_getRequiredAbility_inline(idx)) {
                text_append(s_menuChocographHelpBuf, text_get(CHOCO_TEXT_INADEQUATE));
            }
            g_menuHelp.text = s_menuChocographHelpBuf;
        }
    }
}

void menuChocograph_readFlags(void) {
    s32 i;
    u32 bit;

    s_menuChocograph.found = 0;
    s_menuChocograph.owned = 0;
    s_menuChocograph.foundCount = 0;
    s_menuChocograph.ownedCount = 0;
    s_menuChocograph.ability = g_evtGlobalVars.b[CHOCO_VAR_ABILITY];
    for (i = 0; i < 3; i++) {
        s_menuChocograph.owned |= g_evtGlobalVars.b[i + CHOCO_VAR_OWNED] << (i * 8);
        s_menuChocograph.found |= g_evtGlobalVars.b[i + CHOCO_VAR_FOUND] << (i * 8);
    }
    for (i = 0; i < CHOCOGRAPH_COUNT; i++) {
        bit = 1 << i;
        if (s_menuChocograph.owned & bit) {
            s_menuChocograph.ownedCount++;
        }
        if (s_menuChocograph.found & bit) {
            s_menuChocograph.foundCount++;
        }
    }
}

void menuChocograph_loadImages(void) {
    RECT r0;
    RECT r1;
    Ovl32Image *src = &s_menuChocographPictures[1];
    Ovl32Image *dst = &s_menuChocographPictures[0];
    s32 i;

    func_801F9660();
    LoadImage(&s_menuChocographIconClutRect, (u32 *)s_menuChocographIconImage);
    DrawSync(0);
    LoadImage(&s_menuChocographIconRect, (u32 *)(s_menuChocographIconImage + 0x1C0));
    DrawSync(0);
    LoadImage(&s_menuChocographFrameClutRect, (u32 *)s_menuChocographFrameImage);
    DrawSync(0);
    LoadImage(&s_menuChocographFrameRect, (u32 *)(s_menuChocographFrameImage + 0x40));
    DrawSync(0);
    for (i = 0; i < CHOCOGRAPH_COUNT; i++) {
        menuChocograph_getPictureRects(i, &r0, &r1);
        StoreImage(&r0, (u32 *)dst->clut);
        DrawSync(0);
        StoreImage(&r1, (u32 *)dst->pixels);
        DrawSync(0);
        LoadImage(&r0, (u32 *)src->clut);
        DrawSync(0);
        LoadImage(&r1, (u32 *)src->pixels);
        DrawSync(0);
        dst++;
        src++;
    }
}

void menuChocograph_restoreVram(void) {
    RECT r0;
    RECT r1;
    s32 i;
    Ovl32Image *img = s_menuChocographPictures;
    s32 sel;

    if (g_gameState->mode == 3) {
        sel = g_gameState->unk1C->unk844;
        if (sel != 0) {
            menuChocograph_getPictureRects(sel - 1, &r0, &r1);
            MoveImage(&r0, 0x280, 0xF6);
            DrawSync(0);
            MoveImage(&r1, 0x318, 0xB0);
            DrawSync(0);
        }
    }
    for (i = 0; i < CHOCOGRAPH_COUNT; i++, img++) {
        menuChocograph_getPictureRects(i, &r0, &r1);
        LoadImage(&r0, (u32 *)img->clut);
        DrawSync(0);
        LoadImage(&r1, (u32 *)img->pixels);
        DrawSync(0);
    }
}

void menuChocograph_drawPicture(s32 x, s32 y, s32 z, s32 id, u8 dim) {
    SprtWords *p;
    u16 cl;
    u16 clut = GetClut(0, id + 0x1E8);
    s32 u = (id % 3) * 0x2A;
    s32 v = (id / 3) * 0x1C;

    setlen(g_primPtr, 4);
    cl = clut;
    ((SprtWords *)g_primPtr)->rgbc = dim ? 0x66808080 : 0x64808080;
    p = (SprtWords *)g_primPtr;
    p->xy = x | (y << 16);
    p->uvclut = (u & 0xFF) | ((v & 0xFF) << 8) | (cl << 16);
    p->wh = 0x1C002A;
    g_primPtr = (u8 *)(p + 1);
    addPrim(&g_ot[z], p);
    SetDrawTPage(g_primPtr, 0, 1, 0x90);
    addPrim(&g_ot[z], g_primPtr);
    g_primPtr += 8;
}

void menuChocograph_drawFrame(s32 x, s32 y, s32 otz, u8 dim) {
    u16 clut;
    s32 i;

    clut = GetClut(0x3C0, 0x1FE);
    for (i = 0; i < 4; i++) {
        setlen(g_primPtr, 9);
        ((Ft4W *)g_primPtr)->rgbc = dim ? 0x2E808080 : 0x2C808080;
        if (i < 2) {
            ((Ft4W *)g_primPtr)->x0 = s_menuChocographFramePieces[i].x + x;
            ((Ft4W *)g_primPtr)->y0 = s_menuChocographFramePieces[i].y + y;
            ((Ft4W *)g_primPtr)->x1 = s_menuChocographFramePieces[i].x + x + s_menuChocographFramePieces[i].w;
            ((Ft4W *)g_primPtr)->y1 = s_menuChocographFramePieces[i].y + y;
            ((Ft4W *)g_primPtr)->x2 = s_menuChocographFramePieces[i].x + x;
            ((Ft4W *)g_primPtr)->y2 = s_menuChocographFramePieces[i].y + y + s_menuChocographFramePieces[i].h;
            ((Ft4W *)g_primPtr)->x3 = s_menuChocographFramePieces[i].x + x + s_menuChocographFramePieces[i].w;
            ((Ft4W *)g_primPtr)->y3 = s_menuChocographFramePieces[i].y + y + s_menuChocographFramePieces[i].h;
        } else {
            ((Ft4W *)g_primPtr)->x0 = s_menuChocographFramePieces[i].x + x;
            ((Ft4W *)g_primPtr)->y0 = s_menuChocographFramePieces[i].y + y + s_menuChocographFramePieces[i].w;
            ((Ft4W *)g_primPtr)->x1 = s_menuChocographFramePieces[i].x + x;
            ((Ft4W *)g_primPtr)->y1 = y + s_menuChocographFramePieces[i].y - (i != 2);
            ((Ft4W *)g_primPtr)->x2 = s_menuChocographFramePieces[i].x + x + s_menuChocographFramePieces[i].h;
            ((Ft4W *)g_primPtr)->y2 = s_menuChocographFramePieces[i].y + y + s_menuChocographFramePieces[i].w;
            ((Ft4W *)g_primPtr)->x3 = s_menuChocographFramePieces[i].x + x + s_menuChocographFramePieces[i].h;
            ((Ft4W *)g_primPtr)->y3 = y + s_menuChocographFramePieces[i].y - (i != 2);
        }
        ((Ft4W *)g_primPtr)->u0 = s_menuChocographFramePieces[i].u;
        ((Ft4W *)g_primPtr)->v0 = s_menuChocographFramePieces[i].v;
        ((Ft4W *)g_primPtr)->u1 = s_menuChocographFramePieces[i].u + s_menuChocographFramePieces[i].w;
        ((Ft4W *)g_primPtr)->v1 = s_menuChocographFramePieces[i].v;
        ((Ft4W *)g_primPtr)->u2 = s_menuChocographFramePieces[i].u;
        ((Ft4W *)g_primPtr)->v2 = s_menuChocographFramePieces[i].v + s_menuChocographFramePieces[i].h;
        ((Ft4W *)g_primPtr)->u3 = s_menuChocographFramePieces[i].u + s_menuChocographFramePieces[i].w;
        ((Ft4W *)g_primPtr)->v3 = s_menuChocographFramePieces[i].v + s_menuChocographFramePieces[i].h;
        ((Ft4W *)g_primPtr)->tpage = 0x1F;
        ((Ft4W *)g_primPtr)->clut = clut;
        addPrim(&g_ot[otz], (Ft4W *)g_primPtr);
        g_primPtr += sizeof(Ft4W);
    }
    x += 3;
    y += 3;
    for (i = 0; i < 2; i++) {
        s32 k = s_menuChocographFrameShadowPieces[i];
        setlen(g_primPtr, 9);
        ((Ft4W *)g_primPtr)->rgbc = 0x2E101010;
        if (k < 2) {
            ((Ft4W *)g_primPtr)->x0 = s_menuChocographFramePieces[k].x + x;
            ((Ft4W *)g_primPtr)->y0 = s_menuChocographFramePieces[k].y + y;
            ((Ft4W *)g_primPtr)->x1 = s_menuChocographFramePieces[k].x + x + s_menuChocographFramePieces[k].w;
            ((Ft4W *)g_primPtr)->y1 = s_menuChocographFramePieces[k].y + y;
            ((Ft4W *)g_primPtr)->x2 = s_menuChocographFramePieces[k].x + x;
            ((Ft4W *)g_primPtr)->y2 = s_menuChocographFramePieces[k].y + y + s_menuChocographFramePieces[k].h;
            ((Ft4W *)g_primPtr)->x3 = s_menuChocographFramePieces[k].x + x + s_menuChocographFramePieces[k].w;
            ((Ft4W *)g_primPtr)->y3 = s_menuChocographFramePieces[k].y + y + s_menuChocographFramePieces[k].h;
        } else {
            ((Ft4W *)g_primPtr)->x0 = s_menuChocographFramePieces[k].x + x;
            ((Ft4W *)g_primPtr)->y0 = s_menuChocographFramePieces[k].y + y + s_menuChocographFramePieces[k].w;
            ((Ft4W *)g_primPtr)->x1 = s_menuChocographFramePieces[k].x + x;
            ((Ft4W *)g_primPtr)->y1 = s_menuChocographFramePieces[k].y + y - 1;
            ((Ft4W *)g_primPtr)->x2 = s_menuChocographFramePieces[k].x + x + s_menuChocographFramePieces[k].h;
            ((Ft4W *)g_primPtr)->y2 = s_menuChocographFramePieces[k].y + y + s_menuChocographFramePieces[k].w;
            ((Ft4W *)g_primPtr)->x3 = s_menuChocographFramePieces[k].x + x + s_menuChocographFramePieces[k].h;
            ((Ft4W *)g_primPtr)->y3 = s_menuChocographFramePieces[k].y + y - 1;
        }
        ((Ft4W *)g_primPtr)->u0 = s_menuChocographFramePieces[k].u;
        ((Ft4W *)g_primPtr)->v0 = s_menuChocographFramePieces[k].v;
        ((Ft4W *)g_primPtr)->u1 = s_menuChocographFramePieces[k].u + s_menuChocographFramePieces[k].w;
        ((Ft4W *)g_primPtr)->v1 = s_menuChocographFramePieces[k].v;
        ((Ft4W *)g_primPtr)->u2 = s_menuChocographFramePieces[k].u;
        ((Ft4W *)g_primPtr)->v2 = s_menuChocographFramePieces[k].v + s_menuChocographFramePieces[k].h;
        ((Ft4W *)g_primPtr)->u3 = s_menuChocographFramePieces[k].u + s_menuChocographFramePieces[k].w;
        ((Ft4W *)g_primPtr)->v3 = s_menuChocographFramePieces[k].v + s_menuChocographFramePieces[k].h;
        ((Ft4W *)g_primPtr)->tpage = 0x1F;
        ((Ft4W *)g_primPtr)->clut = clut + 0x40;
        addPrim(&g_ot[otz], (Ft4W *)g_primPtr);
        g_primPtr += sizeof(Ft4W);
    }
}

void menuChocograph_update(void) {
    s_menuChocograph.textPtr = s_menuChocograph.textBuf + (g_gameState->unk1C->unk8 << 12);
    menuChocograph_input();
    menuChocograph_draw();
    menuChocograph_updateList();
}

void menuChocograph_exit(void) {
    help_setCursorOnTop(1);
    help_setCallback(0);
    menuChocograph_restoreVram();
}

void menuChocograph_input(void) {
    void (*f)(void);

    if (!fade_isBusy()) {
        f = s_menuChocographInputFuncs[s_menuChocograph.mode];
        if (f != NULL) {
            f();
        }
    }
}

void menuChocograph_draw(void) {
    menuChocograph_drawTitle();
    menuChocograph_drawTabs();
    menuChocograph_drawSelected();
    menuChocograph_drawList();
    menuChocograph_drawCounts();
    menuChocograph_drawAbilities();
    menuChocograph_drawHint();
    menuChocograph_drawCursor();
}

void menuChocograph_updateList(void) {
    if (s_menuChocograph.mode == CHOCO_MODE_LIST) {
        menuList_update(&s_menuChocograph.list);
    }
}

void menuChocograph_drawTitle(void) {
    RECT *r = &s_menuChocograph.titleRect;

    win_drawBox(r, 0x1D, 2);
    win_drawShadow(r, 0x1D, 2);
    text_drawBlock(r->x + 7, r->y + 6, 0x1C, text_get(CHOCO_TEXT_TITLE), 0);
}

void menuChocograph_drawTabs(void) {
    RECT *r = &s_menuChocograph.tabRect;
    s32 i;
    s32 x;
    u8 none = g_gameState->unk1C->unk844 == 0;

    win_drawBox(r, 0x1D, 2);
    win_drawShadow(r, 0x1D, 2);
    for (i = 0, x = 0x2C; i < 2; i++, x += 0x60) {
        text_drawBlock(r->x + x, r->y + 6, 0x1C, text_get(i + CHOCO_TEXT_SELECT), (s_menuChocographTabNeedsSel[i] != 0 && none) ? 2 : 0);
    }
}

void menuChocograph_drawAbilities(void) {
    s32 i;
    s32 x;
    RECT *r = &s_menuChocograph.abilityRect;

    ui_drawLabel(r->x + 8, r->y - 3, 0x1C, D_80070424, 0);
    win_drawBox(r, 0x1D, 2);
    win_drawShadow(r, 0x1E, 2);
    for (i = 0; i < s_menuChocograph.ability; i++) {
        menuChocograph_drawIcon(r->x + 0xB + i * 0x13, r->y + 5, 0x1C, i, 0);
    }
}

void menuChocograph_drawScrollRows(void) {
    MenuView view;
    s32 i;

    menuList_getPageView(&s_menuChocograph.list, &view);
    for (i = 0; i < view.unk0; i++) {
        menuChocograph_drawRow(0, i, view.unk1 + i, &view);
    }
}

void menuChocograph_drawRow(s32 col, s32 row, s32 id, MenuView *v) {
    u8 got;
    u8 near;
    s32 y;
    s32 icon;
    s32 k = 0x6B;
    s32 x;

    if (s_menuChocograph.owned & (1 << id)) {
        got = (s_menuChocograph.found & (1 << id)) != 0;
        near = menuChocograph_getRequiredAbility(id) < s_menuChocograph.ability;
        x = 0xE;
        y = row * 0xF + (v->unk2 + k);
        if (got) {
            icon = near ? CHOCO_ICON_CHOCOGRAPH + 1 : CHOCO_ICON_CHOCOGRAPH + 3;
        } else {
            icon = near ? CHOCO_ICON_CHOCOGRAPH : CHOCO_ICON_CHOCOGRAPH + 2;
        }
        menuChocograph_drawIcon(x, y, 0x1C, icon, near == 0);
        text_drawBlock(x + 0x11, y, 0x1C, text_get(id + CHOCO_TEXT_NAME), near ? 0 : 2);
    }
}

void menuChocograph_drawIcon(s32 x, s32 y, s32 z, s32 icon, u8 arg4) {
    IconTable rects = s_menuChocographIcons;

    ui_drawSpriteDef(x, y, z, &rects.r[icon], arg4);
}

s32 menuChocograph_getRequiredAbility(s32 id) {
    return s_menuChocographRequiredAbility[id];
}

void menuChocograph_scrollTo(s32 idx) {
    MenuList *l = &s_menuChocograph.list;

    if (idx > CHOCO_LAST_PAGE - 1) {
        l->top = CHOCO_LAST_PAGE;
        l->cursor = idx - CHOCO_LAST_PAGE;
    } else {
        l->cursor = idx % CHOCO_LIST_ROWS;
        l->top = idx - l->cursor;
    }
}

void func_801F9660(void) {
}

void menuChocograph_getPictureRects(s32 i, RECT *a, RECT *b) {
    a->x = 0;
    a->y = i + 0x1E8;
    a->w = 0x40;
    a->h = 1;
    b->x = (i % 3) * 0x15;
    b->y = (i / 3) * 0x1C + 0x100;
    b->w = 0x15;
    b->h = 0x1C;
}

INCLUDE_RODATA("asm/us/ovl_32/nonmatchings/ovl_32", s_menuChocographPictures);

INCLUDE_RODATA("asm/us/ovl_32/nonmatchings/ovl_32", D_801EF6FC);

INCLUDE_RODATA("asm/us/ovl_32/nonmatchings/ovl_32", s_menuChocographIconImage);

INCLUDE_RODATA("asm/us/ovl_32/nonmatchings/ovl_32", s_menuChocographFrameImage);
