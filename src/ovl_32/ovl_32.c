#include "ovl_32.h"

INCLUDE_RODATA("asm/us/ovl_32/nonmatchings/ovl_32", g_menuChocoIcons);

/* text bank for this menu (count + {offset, info} table, then strings) */
INCLUDE_RODATA("asm/us/ovl_32/nonmatchings/ovl_32", g_chocographText);

void menu_choco_init(void) {
    ChocographMenu *st;

    if (g_gameState->unk1C->unk844 > CHOCOGRAPH_COUNT) {
        g_gameState->unk1C->unk844 = 0;
    }
    st = &g_menuChoco;
    st->mode = CHOCO_MODE_COMMAND;
    g_menuChoco.cmdCursor = CHOCO_CMD_SELECT;
    text_setBank(g_chocographText);
    func_8003556C(menu_choco_setHelpText);
    func_80035674(1);
    setRECT(&g_menuChoco.titleRect, 0xE4, 8, 0x50, 0x1A);
    setRECT(&g_menuChoco.cmdRect, 8, 8, 0xD8, 0x1A);
    setRECT(&g_menuChoco.selectedRect, 8, 0x26, 0xB7, 0x43);
    setRECT(&g_menuChoco.listRect, 8, 0x6B, 0x86, 0x71);
    setRECT(&g_menuChoco.countRect, 0xBF, 0x47, 0x75, 0x22);
    setRECT(&g_menuChoco.abilityRect, 0xBF, 0x26, 0x75, 0x21);
    setRECT(&g_menuChoco.infoRect, 0x8E, 0x6B, 0xA6, 0x71);
    func_8002EDFC(&g_menuChoco.list, CHOCOGRAPH_COUNT, CHOCO_LIST_ROWS, 1, 0x71);
    menu_choco_loadState();
    menu_choco_loadImages();
    if (g_gameState->unk1C->unk844 != 0) {
        menu_choco_scrollTo(g_gameState->unk1C->unk844 - 1);
    }
    g_menuChoco.textPtr = g_menuChoco.textBuf + (g_gameState->unk1C->unk8 << 12);
}

void menu_choco_inputCommand(void) {
    u8 v;
    ChocographMenu *st;

    if (g_padRepeat & BTN_CIRCLE) {
        snd_playSe(SE_CANCEL);
        func_800325E0();
    } else if (g_padRepeat & BTN_CROSS) {
        if (g_menuChoco.cmdCursor == CHOCO_CMD_SELECT) {
            g_menuChoco.mode = CHOCO_MODE_LIST;
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
            st = &g_menuChoco;
            if (st->cmdCursor != 0) {
                v = st->cmdCursor - 1;
            } else {
                v = 1;
            }
        } else if (g_padRepeat & PAD_RIGHT) {
            snd_playSe(SE_CURSOR);
            st = &g_menuChoco;
            if (st->cmdCursor == 0) {
                v = st->cmdCursor + 1;
            } else {
                v = 0;
            }
        } else {
            return;
        }
        st->cmdCursor = v;
    }
}

void menu_choco_inputList(void) {
    ChocographMenu *st;
    MenuList *l = &g_menuChoco.list;
    s32 idx;
    s32 n;

    if (l->unk8 == 0) {
        if (g_padRepeat & BTN_CIRCLE) {
            snd_playSe(SE_CANCEL);
            g_menuChoco.mode = CHOCO_MODE_COMMAND;
            return;
        }
        if (g_padRepeat & BTN_CROSS) {
            st = &g_menuChoco;
            idx = l->unk5 + l->cursor;
            if (st->owned & (1 << idx)) {
                n = st->ability;
                if (menu_choco_getRequiredAbility(idx) < n) {
                    snd_playSe(SE_CONFIRM);
                    g_gameState->unk1C->unk844 = l->unk5 + l->cursor + 1;
                    return;
                }
            }
            snd_playSe(SE_BUZZER);
            return;
        }
        menuList_input(l, g_padRepeat);
    }
}

void menu_choco_drawSelected(void) {
    RECT *r = &g_menuChoco.selectedRect;
    s32 sel;

    func_8002DEE4(r, 0x1D, 2);
    func_8002705C(r, 0x1E, 2);
    ui_drawLabel(r->x + 8, r->y - 3, 0x1C, D_800704AC, 0);
    sel = g_gameState->unk1C->unk844;
    if (sel != 0) {
        menu_choco_drawIcon(r->x + 0x10, r->y + 0x2C, 0x1C, (g_menuChoco.found & (1 << (sel - 1))) ? CHOCO_ICON_CHOCOGRAPH + 1 : CHOCO_ICON_CHOCOGRAPH, 0);
        text_drawBlock(r->x + 0x21, r->y + 0x2E, 0x1C, text_get(g_gameState->unk1C->unk844 - 1 + CHOCO_TEXT_NAME), 0);
        menu_choco_drawPicture(r->x + 0x4C, r->y + 0xB, 0x1C, g_gameState->unk1C->unk844 - 1, 0);
        menu_choco_drawFrame(r->x + 0x48, r->y + 7, 0x1C, 0);
    }
}

void menu_choco_drawList(void) {
    RECT sub;
    MenuView view;
    s32 i;
    RECT *r = &g_menuChoco.listRect;
    MenuList *list;

    ui_drawLabel(r->x + 0x10, r->y - 3, 0x1C, D_80070428, 0);
    list = &g_menuChoco.list;
    func_8002E040(r, 0x1D, 1, list);
    func_8002705C(r, 0x1D, 1);
    menuList_getView(list, &view);
    win_setClip(NULL, 0x1C, 1);
    for (i = 0; i < view.unk0; i++) {
        menu_choco_drawListItem(0, i, view.unk1 + i, &view);
    }
    if (list->unk8 & 4) {
        func_8002F40C(list, r, &sub);
        win_setClip(&sub, 0x1C, 1);
        menu_choco_drawListScroll();
    }
    win_setClip(r, 0x1C, 1);
}

void menu_choco_drawCounts(void) {
    u8 buf[8];
    RECT *r = &g_menuChoco.countRect;
    ChocographMenu *st;

    func_8002DEE4(r, 0x1D, 2);
    func_8002705C(r, 0x1E, 2);
    text_drawBlock(r->x + 9, r->y + 3, 0x1C, text_get(CHOCO_TEXT_INVENTORY), 0);
    text_drawBlock(r->x + 9, r->y + 0xF, 0x1C, text_get(CHOCO_TEXT_FOUND), 0);
    st = &g_menuChoco;
    text_formatNumber(st->ownedCount, buf, 2, 1);
    ui_drawSmallDigits(r->x + 0x5E, r->y + 7, 0x1C, buf, 0);
    text_formatNumber(st->foundCount, buf, 2, 1);
    ui_drawSmallDigits(r->x + 0x5E, r->y + 0x13, 0x1C, buf, 0);
}

void menu_choco_drawInfo(void) {
    RECT *r = &g_menuChoco.infoRect;
    ChocographMenu *st = &g_menuChoco;
    s32 id;
    s32 n;
    s32 i;

    id = st->list.unk5 + st->list.cursor;
    ui_drawLabel(r->x + 8, r->y - 3, 0x1C, D_80070454, 0);
    func_8002DEE4(r, 0x1D, 1);
    func_8002705C(r, 0x1E, 1);
    if (st->mode == CHOCO_MODE_LIST && (st->owned & (1 << id))) {
        menu_choco_getRequiredAbility(id);
        menu_choco_drawPicture(r->x + 0xC, r->y + 0xC, 0x1C, id, 0);
        menu_choco_drawFrame(r->x + 8, r->y + 8, 0x1C, 0);
        st->textPtr = func_8002920C(r->x + 0xC, r->y + 0x31, &g_ot[0x1C], text_get(id + CHOCO_TEXT_CLUE), 0, st->textPtr);
        text_drawBlock(r->x + 0x42, r->y + 6, 0x1C, text_get(CHOCO_TEXT_ABILITY), 0);
        n = menu_choco_getRequiredAbility(id);
        for (i = 0; i <= n; i++) {
            menu_choco_drawIcon(r->x + 0x42 + i * 0x13, r->y + 0x16, 0x1C, i < st->ability ? i : CHOCO_ICON_LOCKED, i >= st->ability);
        }
    }
}

void menu_choco_drawCursor(void) {
    func_80035674(g_menuChoco.mode != CHOCO_MODE_COMMAND);
    if (!func_8002F810()) {
        if (g_menuChoco.mode == CHOCO_MODE_COMMAND || (D_80077DC0 & 3)) {
            D_80077DE8.unk2 = 0x11;
            D_80077DE8.unk4 = 0x1B;
            D_80077DE8.unk0 = g_menuChoco.cmdCursor * 0x60 + 0x1C;
            func_800355E4(&D_80077DE8);
        }
        if (g_menuChoco.mode == CHOCO_MODE_LIST) {
            D_80077DE8.unk0 = 0;
            D_80077DE8.unk4 = 0x1B;
            D_80077DE8.unk2 = g_menuChoco.list.cursor * 0xF + 0x74;
            func_800355E4();
        }
    }
}

void menu_choco_setHelpText(void) {
    s32 idx;

    D_80077DE8.unk8 = NULL;
    func_80035580(0);
    if (g_menuChoco.mode == CHOCO_MODE_COMMAND) {
        D_80077DE8.unk8 = text_get(g_menuChoco.cmdCursor + CHOCO_TEXT_HELP_CMD);
    } else if (g_menuChoco.mode == CHOCO_MODE_LIST) {
        idx = g_menuChoco.list.unk5 + g_menuChoco.list.cursor;
        if (g_menuChoco.owned & (1 << idx)) {
            func_8002BD54(g_menuChocoHelpBuf, text_get(idx + CHOCO_TEXT_HINT));
            if (g_menuChoco.ability <= menu_choco_getRequiredAbility_inline(idx)) {
                text_append(g_menuChocoHelpBuf, text_get(CHOCO_TEXT_INADEQUATE));
            }
            D_80077DE8.unk8 = g_menuChocoHelpBuf;
        }
    }
}

void menu_choco_loadState(void) {
    s32 i;
    u32 bit;

    g_menuChoco.found = 0;
    g_menuChoco.owned = 0;
    g_menuChoco.foundCount = 0;
    g_menuChoco.ownedCount = 0;
    g_menuChoco.ability = g_evtGlobalVars.b[CHOCO_VAR_ABILITY];
    for (i = 0; i < 3; i++) {
        g_menuChoco.owned |= g_evtGlobalVars.b[i + CHOCO_VAR_OWNED] << (i * 8);
        g_menuChoco.found |= g_evtGlobalVars.b[i + CHOCO_VAR_FOUND] << (i * 8);
    }
    for (i = 0; i < CHOCOGRAPH_COUNT; i++) {
        bit = 1 << i;
        if (g_menuChoco.owned & bit) {
            g_menuChoco.ownedCount++;
        }
        if (g_menuChoco.found & bit) {
            g_menuChoco.foundCount++;
        }
    }
}

void menu_choco_loadImages(void) {
    RECT r0;
    RECT r1;
    Ovl32Image *src = &g_menuChocoPictures[1];
    Ovl32Image *dst = &g_menuChocoPictures[0];
    s32 i;

    func_801F9660();
    LoadImage(&g_menuChocoIconClutRect, (u32 *)g_menuChocoIconImage);
    DrawSync(0);
    LoadImage(&g_menuChocoIconRect, (u32 *)(g_menuChocoIconImage + 0x1C0));
    DrawSync(0);
    LoadImage(&g_menuChocoFrameClutRect, (u32 *)g_menuChocoFrameImage);
    DrawSync(0);
    LoadImage(&g_menuChocoFrameRect, (u32 *)(g_menuChocoFrameImage + 0x40));
    DrawSync(0);
    for (i = 0; i < CHOCOGRAPH_COUNT; i++) {
        menu_choco_getPictureRects(i, &r0, &r1);
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

void menu_choco_restoreVram(void) {
    RECT r0;
    RECT r1;
    s32 i;
    Ovl32Image *img = g_menuChocoPictures;
    s32 sel;

    if (g_gameState->mode == 3) {
        sel = g_gameState->unk1C->unk844;
        if (sel != 0) {
            menu_choco_getPictureRects(sel - 1, &r0, &r1);
            MoveImage(&r0, 0x280, 0xF6);
            DrawSync(0);
            MoveImage(&r1, 0x318, 0xB0);
            DrawSync(0);
        }
    }
    for (i = 0; i < CHOCOGRAPH_COUNT; i++, img++) {
        menu_choco_getPictureRects(i, &r0, &r1);
        LoadImage(&r0, (u32 *)img->clut);
        DrawSync(0);
        LoadImage(&r1, (u32 *)img->pixels);
        DrawSync(0);
    }
}

void menu_choco_drawPicture(s32 x, s32 y, s32 z, s32 id, u8 dim) {
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

void menu_choco_drawFrame(s32 x, s32 y, s32 otz, u8 dim) {
    u16 clut;
    s32 i;

    clut = GetClut(0x3C0, 0x1FE);
    for (i = 0; i < 4; i++) {
        setlen(g_primPtr, 9);
        ((Ft4W *)g_primPtr)->rgbc = dim ? 0x2E808080 : 0x2C808080;
        if (i < 2) {
            ((Ft4W *)g_primPtr)->x0 = g_menuChocoFramePieces[i].x + x;
            ((Ft4W *)g_primPtr)->y0 = g_menuChocoFramePieces[i].y + y;
            ((Ft4W *)g_primPtr)->x1 = g_menuChocoFramePieces[i].x + x + g_menuChocoFramePieces[i].w;
            ((Ft4W *)g_primPtr)->y1 = g_menuChocoFramePieces[i].y + y;
            ((Ft4W *)g_primPtr)->x2 = g_menuChocoFramePieces[i].x + x;
            ((Ft4W *)g_primPtr)->y2 = g_menuChocoFramePieces[i].y + y + g_menuChocoFramePieces[i].h;
            ((Ft4W *)g_primPtr)->x3 = g_menuChocoFramePieces[i].x + x + g_menuChocoFramePieces[i].w;
            ((Ft4W *)g_primPtr)->y3 = g_menuChocoFramePieces[i].y + y + g_menuChocoFramePieces[i].h;
        } else {
            ((Ft4W *)g_primPtr)->x0 = g_menuChocoFramePieces[i].x + x;
            ((Ft4W *)g_primPtr)->y0 = g_menuChocoFramePieces[i].y + y + g_menuChocoFramePieces[i].w;
            ((Ft4W *)g_primPtr)->x1 = g_menuChocoFramePieces[i].x + x;
            ((Ft4W *)g_primPtr)->y1 = y + g_menuChocoFramePieces[i].y - (i != 2);
            ((Ft4W *)g_primPtr)->x2 = g_menuChocoFramePieces[i].x + x + g_menuChocoFramePieces[i].h;
            ((Ft4W *)g_primPtr)->y2 = g_menuChocoFramePieces[i].y + y + g_menuChocoFramePieces[i].w;
            ((Ft4W *)g_primPtr)->x3 = g_menuChocoFramePieces[i].x + x + g_menuChocoFramePieces[i].h;
            ((Ft4W *)g_primPtr)->y3 = y + g_menuChocoFramePieces[i].y - (i != 2);
        }
        ((Ft4W *)g_primPtr)->u0 = g_menuChocoFramePieces[i].u;
        ((Ft4W *)g_primPtr)->v0 = g_menuChocoFramePieces[i].v;
        ((Ft4W *)g_primPtr)->u1 = g_menuChocoFramePieces[i].u + g_menuChocoFramePieces[i].w;
        ((Ft4W *)g_primPtr)->v1 = g_menuChocoFramePieces[i].v;
        ((Ft4W *)g_primPtr)->u2 = g_menuChocoFramePieces[i].u;
        ((Ft4W *)g_primPtr)->v2 = g_menuChocoFramePieces[i].v + g_menuChocoFramePieces[i].h;
        ((Ft4W *)g_primPtr)->u3 = g_menuChocoFramePieces[i].u + g_menuChocoFramePieces[i].w;
        ((Ft4W *)g_primPtr)->v3 = g_menuChocoFramePieces[i].v + g_menuChocoFramePieces[i].h;
        ((Ft4W *)g_primPtr)->tpage = 0x1F;
        ((Ft4W *)g_primPtr)->clut = clut;
        addPrim(&g_ot[otz], (Ft4W *)g_primPtr);
        g_primPtr += sizeof(Ft4W);
    }
    x += 3;
    y += 3;
    for (i = 0; i < 2; i++) {
        s32 k = g_menuChocoFrameShadowPieces[i];
        setlen(g_primPtr, 9);
        ((Ft4W *)g_primPtr)->rgbc = 0x2E101010;
        if (k < 2) {
            ((Ft4W *)g_primPtr)->x0 = g_menuChocoFramePieces[k].x + x;
            ((Ft4W *)g_primPtr)->y0 = g_menuChocoFramePieces[k].y + y;
            ((Ft4W *)g_primPtr)->x1 = g_menuChocoFramePieces[k].x + x + g_menuChocoFramePieces[k].w;
            ((Ft4W *)g_primPtr)->y1 = g_menuChocoFramePieces[k].y + y;
            ((Ft4W *)g_primPtr)->x2 = g_menuChocoFramePieces[k].x + x;
            ((Ft4W *)g_primPtr)->y2 = g_menuChocoFramePieces[k].y + y + g_menuChocoFramePieces[k].h;
            ((Ft4W *)g_primPtr)->x3 = g_menuChocoFramePieces[k].x + x + g_menuChocoFramePieces[k].w;
            ((Ft4W *)g_primPtr)->y3 = g_menuChocoFramePieces[k].y + y + g_menuChocoFramePieces[k].h;
        } else {
            ((Ft4W *)g_primPtr)->x0 = g_menuChocoFramePieces[k].x + x;
            ((Ft4W *)g_primPtr)->y0 = g_menuChocoFramePieces[k].y + y + g_menuChocoFramePieces[k].w;
            ((Ft4W *)g_primPtr)->x1 = g_menuChocoFramePieces[k].x + x;
            ((Ft4W *)g_primPtr)->y1 = g_menuChocoFramePieces[k].y + y - 1;
            ((Ft4W *)g_primPtr)->x2 = g_menuChocoFramePieces[k].x + x + g_menuChocoFramePieces[k].h;
            ((Ft4W *)g_primPtr)->y2 = g_menuChocoFramePieces[k].y + y + g_menuChocoFramePieces[k].w;
            ((Ft4W *)g_primPtr)->x3 = g_menuChocoFramePieces[k].x + x + g_menuChocoFramePieces[k].h;
            ((Ft4W *)g_primPtr)->y3 = g_menuChocoFramePieces[k].y + y - 1;
        }
        ((Ft4W *)g_primPtr)->u0 = g_menuChocoFramePieces[k].u;
        ((Ft4W *)g_primPtr)->v0 = g_menuChocoFramePieces[k].v;
        ((Ft4W *)g_primPtr)->u1 = g_menuChocoFramePieces[k].u + g_menuChocoFramePieces[k].w;
        ((Ft4W *)g_primPtr)->v1 = g_menuChocoFramePieces[k].v;
        ((Ft4W *)g_primPtr)->u2 = g_menuChocoFramePieces[k].u;
        ((Ft4W *)g_primPtr)->v2 = g_menuChocoFramePieces[k].v + g_menuChocoFramePieces[k].h;
        ((Ft4W *)g_primPtr)->u3 = g_menuChocoFramePieces[k].u + g_menuChocoFramePieces[k].w;
        ((Ft4W *)g_primPtr)->v3 = g_menuChocoFramePieces[k].v + g_menuChocoFramePieces[k].h;
        ((Ft4W *)g_primPtr)->tpage = 0x1F;
        ((Ft4W *)g_primPtr)->clut = clut + 0x40;
        addPrim(&g_ot[otz], (Ft4W *)g_primPtr);
        g_primPtr += sizeof(Ft4W);
    }
}

void menu_choco_update(void) {
    g_menuChoco.textPtr = g_menuChoco.textBuf + (g_gameState->unk1C->unk8 << 12);
    menu_choco_input();
    menu_choco_draw();
    menu_choco_updateList();
}

void menu_choco_exit(void) {
    func_80035674(1);
    func_8003556C(0);
    menu_choco_restoreVram();
}

void menu_choco_input(void) {
    void (*f)(void);

    if (!func_8002F810()) {
        f = g_menuChocoInputFuncs[g_menuChoco.mode];
        if (f != NULL) {
            f();
        }
    }
}

void menu_choco_draw(void) {
    menu_choco_drawTitle();
    menu_choco_drawCommands();
    menu_choco_drawSelected();
    menu_choco_drawList();
    menu_choco_drawCounts();
    menu_choco_drawAbility();
    menu_choco_drawInfo();
    menu_choco_drawCursor();
}

void menu_choco_updateList(void) {
    if (g_menuChoco.mode == CHOCO_MODE_LIST) {
        func_8002EB74(&g_menuChoco.list);
    }
}

void menu_choco_drawTitle(void) {
    RECT *r = &g_menuChoco.titleRect;

    func_8002DEE4(r, 0x1D, 2);
    func_8002705C(r, 0x1D, 2);
    text_drawBlock(r->x + 7, r->y + 6, 0x1C, text_get(CHOCO_TEXT_TITLE), 0);
}

void menu_choco_drawCommands(void) {
    RECT *r = &g_menuChoco.cmdRect;
    s32 i;
    s32 x;
    u8 none = g_gameState->unk1C->unk844 == 0;

    func_8002DEE4(r, 0x1D, 2);
    func_8002705C(r, 0x1D, 2);
    for (i = 0, x = 0x2C; i < 2; i++, x += 0x60) {
        text_drawBlock(r->x + x, r->y + 6, 0x1C, text_get(i + CHOCO_TEXT_SELECT), (g_menuChocoCmdNeedsSel[i] != 0 && none) ? 2 : 0);
    }
}

void menu_choco_drawAbility(void) {
    s32 i;
    s32 x;
    RECT *r = &g_menuChoco.abilityRect;

    ui_drawLabel(r->x + 8, r->y - 3, 0x1C, D_80070424, 0);
    func_8002DEE4(r, 0x1D, 2);
    func_8002705C(r, 0x1E, 2);
    for (i = 0; i < g_menuChoco.ability; i++) {
        menu_choco_drawIcon(r->x + 0xB + i * 0x13, r->y + 5, 0x1C, i, 0);
    }
}

void menu_choco_drawListScroll(void) {
    MenuView view;
    s32 i;

    func_8002F344(&g_menuChoco.list, &view);
    for (i = 0; i < view.unk0; i++) {
        menu_choco_drawListItem(0, i, view.unk1 + i, &view);
    }
}

void menu_choco_drawListItem(s32 col, s32 row, s32 id, MenuView *v) {
    u8 got;
    u8 near;
    s32 y;
    s32 icon;
    s32 k = 0x6B;
    s32 x;

    if (g_menuChoco.owned & (1 << id)) {
        got = (g_menuChoco.found & (1 << id)) != 0;
        near = menu_choco_getRequiredAbility(id) < g_menuChoco.ability;
        x = 0xE;
        y = row * 0xF + (v->unk2 + k);
        if (got) {
            icon = near ? CHOCO_ICON_CHOCOGRAPH + 1 : CHOCO_ICON_CHOCOGRAPH + 3;
        } else {
            icon = near ? CHOCO_ICON_CHOCOGRAPH : CHOCO_ICON_CHOCOGRAPH + 2;
        }
        menu_choco_drawIcon(x, y, 0x1C, icon, near == 0);
        text_drawBlock(x + 0x11, y, 0x1C, text_get(id + CHOCO_TEXT_NAME), near ? 0 : 2);
    }
}

void menu_choco_drawIcon(s32 x, s32 y, s32 z, s32 icon, u8 arg4) {
    IconTable rects = g_menuChocoIcons;

    func_80027710(x, y, z, &rects.r[icon], arg4);
}

s32 menu_choco_getRequiredAbility(s32 id) {
    return g_menuChocoRequiredAbility[id];
}

void menu_choco_scrollTo(s32 idx) {
    MenuList *l = &g_menuChoco.list;

    if (idx > CHOCO_LAST_PAGE - 1) {
        l->unk5 = CHOCO_LAST_PAGE;
        l->cursor = idx - CHOCO_LAST_PAGE;
    } else {
        l->cursor = idx % CHOCO_LIST_ROWS;
        l->unk5 = idx - l->cursor;
    }
}

void func_801F9660(void) {
}

void menu_choco_getPictureRects(s32 i, RECT *a, RECT *b) {
    a->x = 0;
    a->y = i + 0x1E8;
    a->w = 0x40;
    a->h = 1;
    b->x = (i % 3) * 0x15;
    b->y = (i / 3) * 0x1C + 0x100;
    b->w = 0x15;
    b->h = 0x1C;
}

INCLUDE_RODATA("asm/us/ovl_32/nonmatchings/ovl_32", g_menuChocoPictures);

INCLUDE_RODATA("asm/us/ovl_32/nonmatchings/ovl_32", D_801EF6FC);

INCLUDE_RODATA("asm/us/ovl_32/nonmatchings/ovl_32", g_menuChocoIconImage);

INCLUDE_RODATA("asm/us/ovl_32/nonmatchings/ovl_32", g_menuChocoFrameImage);
