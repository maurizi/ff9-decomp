#include "ff9.h"

void win_drawAll(void) {
    Window *w;
    s32 i;

    w = g_windows;
    for (i = 0; i < 10; w++, i++) {
        if (w->state != WIN_STATE_CLOSED && win_isVisible(w)) {
            if (w->title != 0) {
                win_drawTitle(w);
            }
            if (w->drawOverlay != NULL) {
                w->drawOverlay(w);
            }
            if (w->state == WIN_STATE_OPEN && w->list.visibleRows < w->list.rows) {
                win_drawScrollArrows(&w->cur, w->otz, &w->list);
            }
            win_setClip(NULL, w->otz, w->style);
            if (w->drawContent != NULL && w->active != 0) {
                w->drawContent(w);
            }
            win_setClip(&w->cur, w->otz, w->style);
            if (w->style != WIN_STYLE_FRAMELESS) {
                win_drawFrame(w);
            }
            if (w->shadow != 0) {
                win_drawShadow(&w->cur, w->otz, w->style);
            }
        }
    }
}

void win_drawFrame(Window *w) {
    s32 abr;
    RECT inner;

    abr = g_gameState->config >> CFG_BIT_WINDOW_COLOR;
    abr &= 1;

    if (w->style == WIN_STYLE_BEVEL) {
        win_drawBorder(&w->cur, w->otz, w->active, w->translucent, abr);
        if (w->active == 0) {
            return;
        }
        inner.x = w->cur.x + 4;
        inner.y = w->cur.y + 4;
        inner.w = w->cur.w - 8;
        inner.h = w->cur.h - 8;
    } else if (w->style == WIN_STYLE_BALLOON) {
        if (w->tailMode != 4) {
            win_drawTail(w);
        }
        win_drawBalloonBorder(&w->cur, w->otz, w->active, w->translucent, abr);
        if (w->active == 0) {
            return;
        }
        inner.x = w->cur.x + 2;
        inner.y = w->cur.y + 2;
        inner.w = w->cur.w - 4;
        inner.h = w->cur.h - 4;
    } else {
        if (w->style == WIN_STYLE_LINE_RULE) {
            inner.x = w->cur.x;
            inner.y = w->cur.y + 4;
            inner.w = w->cur.w;
            inner.h = w->cur.h - 4;
            ui_drawRule(w->cur.x, w->cur.y, w->otz, 0, w->cur.w, abr);
            win_drawLineBorder(&inner, w->otz, 1, abr);
        } else {
            inner = w->cur;
            win_drawLineBorder(&inner, w->otz, 0, abr);
        }
        if (w->active == 0) {
            return;
        }
        if (w->list.scroll & MENULIST_SCROLL_PAGE) {
            win_drawSplitFill(&inner, w->otz, &w->list, abr);
            return;
        }
    }
    win_drawFill(&inner, w->otz, w->list.fillU, w->list.fillV, abr);
}

void win_drawBoxEx(RECT *rect, s32 otz, s32 style, s32 abr) {
    RECT inner;

    switch (style) {
    case 2:
        win_drawBorder(rect, otz, 1, 0, abr);
        inner.x = rect->x + 4;
        inner.y = rect->y + 4;
        inner.w = rect->w - 8;
        inner.h = rect->h - 8;
        win_drawFill(&inner, otz, 0, 0, abr);
        break;
    case 1:
        inner.x = rect->x;
        inner.y = rect->y + 4;
        inner.w = rect->w;
        inner.h = rect->h - 4;
        ui_drawRule(rect->x, rect->y, otz, 0, rect->w - 2, abr);
        win_drawLineBorder(&inner, otz, 1, abr);
        win_drawFill(&inner, otz, 0, 0, abr);
        break;
    case 0:
        win_drawLineBorder(rect, otz, 0, abr);
        win_drawFill(rect, otz, 0, 0, abr);
        break;
    case 3:
        win_drawBalloonBorder(rect, otz, 1, 0, abr);
        inner.x = rect->x + 2;
        inner.y = rect->y + 2;
        inner.w = rect->w - 3;
        inner.h = rect->h - 3;
        win_drawFill(&inner, otz, 0, 0, abr);
        break;
    }
}

void win_setOpenAnim(Window *w, s32 mode) {
    s16 h;

    w->animMode = mode;
    if (mode == WIN_ANIM_ZOOM) {
        w->stepW = ((w->w + w->openFrames - 0x11) / w->openFrames) & ~1;
        w->stepH = ((w->h + w->openFrames - 0x11) / w->openFrames) & ~1;
        w->cur.w = w->w - w->stepW * w->openFrames;
        w->cur.h = w->h - w->stepH * w->openFrames;
        w->cur.x = w->x + w->openFrames * (w->stepW >> 1);
        w->cur.y = w->y + w->openFrames * (w->stepH >> 1);
    } else if (mode == WIN_ANIM_ZOOM_X) {
        w->stepW = ((w->w + w->openFrames - 0x11) / w->openFrames) & ~1;
        w->stepH = 0;
        w->cur.h = w->h;
        w->cur.y = w->y;
        w->cur.w = w->w - w->stepW * w->openFrames;
        w->cur.x = w->x + w->openFrames * (w->stepW >> 1);
    } else if (mode == WIN_ANIM_UNROLL) {
        w->stepW = 0;
        w->list.fillV = 0;
        w->list.fillU = 0;
        h = w->h >> 2;
        w->cur.x = w->x;
        w->cur.y = w->y;
        w->cur.w = w->w;
        w->cur.h = h;
        w->stepH = (w->h - h + (w->openFrames - 1)) / w->openFrames;
    } else if (mode == WIN_ANIM_NONE) {
        w->cur = *(RECT *)w;
        w->state = mode;
    }
}

void win_setCloseAnim(Window *w, s32 mode) {
    w->animMode = mode;
    if (mode == WIN_ANIM_ZOOM || mode == WIN_ANIM_ZOOM_X) {
        w->stepW = (w->w + w->closeFrames - 0x11) / w->closeFrames;
        w->stepH = (w->h + w->closeFrames - 0x11) / w->closeFrames;
    } else if (mode == WIN_ANIM_UNROLL) {
        w->stepW = 0;
        w->stepH = (w->cur.h + w->closeFrames - 1) / w->closeFrames;
    } else if (mode == WIN_ANIM_TAIL) {
        w->stepLeft = (w->tailX + w->closeFrames - 9) / w->closeFrames;
        w->stepRight = (w->w - (w->tailX + 0x18) + w->closeFrames - 1) / w->closeFrames;
        w->stepH = (w->h + w->closeFrames - 0x11) / w->closeFrames;
    } else if (mode == WIN_ANIM_NONE) {
        w->state = WIN_STATE_CLOSED;
    }
}

void win_setTail(Window *w, s32 mode, s32 off) {
    RECT *rect = (RECT *)w;

    if (w->style != WIN_STYLE_FRAMELESS) {
        w->animMode = WIN_ANIM_TAIL;
        if (mode == 1 || mode == 3) {
            off -= 0x10;
        }
        w->tailMode = mode;
        w->tailX = off;
        w->stepLeft = (off + w->openFrames - 9) / w->openFrames;
        w->stepRight = (rect->w - (off + 0x18) + w->openFrames - 1) / w->openFrames;
        w->stepH = (w->openFrames + rect->h - 0x11) / w->openFrames;
        w->cur.w = rect->w - (w->stepLeft + w->stepRight) * w->openFrames;
        w->cur.h = rect->h - w->stepH * w->openFrames;
        w->cur.x = rect->x + w->stepLeft * w->openFrames;
        if ((u32)(mode - 2) < 2) {
            w->cur.y = rect->y + rect->h - (s16)(w->cur.h + 1);
        } else {
            w->cur.y = rect->y;
        }
    }
}

void win_stepOpen(Window *w) {
    if (w->animMode == WIN_ANIM_ZOOM || w->animMode == WIN_ANIM_ZOOM_X) {
        w->cur.w += w->stepW;
        w->cur.h += w->stepH;
        w->cur.x -= w->stepW >> 1;
        w->cur.y -= w->stepH >> 1;
        if (w->cur.w > w->w) {
            w->cur.w = w->w;
        }
        if (w->cur.h > w->h) {
            w->cur.h = w->h;
        }
        if (w->cur.x < w->x) {
            w->cur.x = w->x;
        }
        if (w->cur.y < w->y) {
            w->cur.y = w->y;
        }
        w->list.fillU = (w->cur.x - w->x) & 0x1F;
        w->list.fillV = (w->cur.y - w->y) & 0x1F;
    } else if (w->animMode == WIN_ANIM_UNROLL) {
        w->cur.w += w->stepW;
        w->cur.h += w->stepH;
        if (w->cur.w > w->w) {
            w->cur.w = w->w;
        }
        if (w->cur.h > w->h) {
            w->cur.h = w->h;
        }
        w->list.fillV = (w->h - w->cur.h) & 0x1F;
    } else if (w->animMode == WIN_ANIM_TAIL) {
        s16 growW = w->stepLeft + w->stepRight;
        w->cur.w += growW;
        w->cur.h += w->stepH;
        w->cur.x -= w->stepLeft;
        if (w->tailMode >= 2) {
            w->cur.y -= w->stepH;
        }
        if (w->cur.w > w->w) {
            w->cur.w = w->w;
        }
        if (w->cur.h > w->h) {
            w->cur.h = w->h;
        }
        if (w->cur.x < w->x) {
            w->cur.x = w->x;
        }
        if (w->cur.y < w->y) {
            w->cur.y = w->y;
        }
        w->list.fillU = (w->cur.x - w->x) & 0x1F;
        w->list.fillV = (w->cur.y - w->y) & 0x1F;
    }
    if (*(s32 *)&w->cur.w == *(s32 *)&w->w) {
        w->state = WIN_STATE_OPEN;
        if (w->onOpened != NULL) {
            w->onOpened(w);
        }
    }
}

void win_stepClose(Window *w) {
    if (w->animMode == WIN_ANIM_ZOOM || w->animMode == WIN_ANIM_ZOOM_X) {
        w->cur.w -= w->stepW;
        w->cur.h -= w->stepH;
        w->cur.x += w->stepW >> 1;
        w->cur.y += w->stepH >> 1;
        w->list.fillU = (w->cur.x - w->x) & 0x1F;
        w->list.fillV = (w->cur.y - w->y) & 0x1F;
    } else if (w->animMode == WIN_ANIM_UNROLL) {
        w->cur.h -= w->stepH;
        w->cur.w -= w->stepW;
        w->list.fillV = (w->h - w->cur.h) & 0x1F;
    } else if (w->animMode == WIN_ANIM_TAIL) {
        w->cur.w -= w->stepLeft + w->stepRight;
        w->cur.h -= w->stepH;
        w->cur.x += w->stepLeft;
        if (w->tailMode >= 2) {
            w->cur.y += w->stepH;
        }
        w->list.fillU = (w->cur.x - w->x) & 0x1F;
        w->list.fillV = (w->cur.y - w->y) & 0x1F;
    }
    if (w->cur.w < 0) {
        w->cur.w = 0;
    }
    if (w->cur.h < 0) {
        w->cur.h = 0;
    }
    if (w->cur.w == 0 || w->cur.h == 0) {
        w->state = WIN_STATE_CLOSED;
        if (w->onClosed != NULL) {
            w->onClosed(w);
        }
    }
}

void win_setClip(RECT *rect, s32 otz, s32 style) {
    RECT area;
    Unk80010CE0 offs;
    DR_AREA *drArea;
    RECT *clip;
    s8 *inset;

    clip = &g_gameState->dispEnv[g_drawBufs.cur ^ 1].disp;
    offs = g_winClipInsets;
    drArea = (DR_AREA *)g_primPtr;
    inset = offs.insets[style];
    if (rect != NULL) {
        area.x = g_drawBufs.dispPos[g_drawBufs.cur ^ 1].vx + (rect->x + inset[0]);
        area.y = g_drawBufs.dispPos[g_drawBufs.cur ^ 1].vy + (rect->y + inset[1]);
        area.w = rect->w + inset[2];
        area.h = rect->h + inset[3];
        if (area.x < clip->x) {
            area.w -= clip->x - area.x;
            area.x = clip->x;
        }
        if (area.y < clip->y) {
            area.h -= clip->y - area.y;
            area.y = clip->y;
        }
        if (area.x + area.w > clip->x + clip->w) {
            area.w = clip->x + clip->w - area.x;
        }
        if (area.y + area.h > clip->y + clip->h) {
            area.h = clip->y + clip->h - area.y;
        }
    } else {
        area = *clip;
    }
    SetDrawArea(drArea, &area);
    addPrim(&g_ot[otz], drArea);
    g_primPtr += sizeof(DR_AREA);
}

Window *win_openFree(RECT *rect, s32 otz, s32 style) {
    s32 i;
    Window *w = g_windows;
    Window *win;
    for (i = 0; i < 10; i++, w++) {
        if (w->state == WIN_STATE_CLOSED) {
            break;
        }
    }
    if (i < 10) {
        win = win_init(rect, otz, style, w);
    } else {
        win = NULL;
    }
    return win;
}

Window *win_open(RECT *rect, s32 otz, s32 style, u32 slot) {
    Window *win;
    if (slot < 10) {
        win = win_init(rect, otz, style, &g_windows[slot]);
    } else {
        win = NULL;
    }
    return win;
}

void win_close(Window *w) {
    win_close_inline(w);
}

void win_closeAll(void) {
    s32 i;
    Window *w = g_windows;
    for (i = 0; i < 10; w++, i++) {
        if (w->state != WIN_STATE_CLOSED && w->state != WIN_STATE_CLOSING) {
            win_close_inline(w);
        }
    }
}

void win_closeNow(Window *w) {
    if (w != NULL && w->state != WIN_STATE_CLOSED) {
        if (w->onClosed != NULL) {
            w->onClosed(w);
        }
        w->state = WIN_STATE_CLOSED;
    }
}

void win_closeAllNow(void) {
    s32 i;
    Window *w = g_windows;
    for (i = 0; i < 10; w++, i++) {
        if (w->state != WIN_STATE_CLOSED && w != NULL) {
            if (w->onClosed != NULL) {
                w->onClosed(w);
            }
            w->state = WIN_STATE_CLOSED;
        }
    }
}

void win_drawBox(RECT *rect, s32 otz, s32 style) {
    win_drawBoxEx(rect, otz, style, CONFIG_WINDOW_COLOR());
}

void win_drawHelpFrame(RECT *rect, s32 otz, s32 style, s32 xoff) {
    RECT inner;
    s32 y;
    if (style != HELP_TAIL_NONE) {
        if (style >= HELP_TAIL_DOWN) {
            y = rect->y + rect->h - 2;
        } else {
            y = rect->y - 11;
        }
        win_drawHelpTailEdge(rect->x + xoff, y, otz, style, 0);
        win_drawHelpTailFill(rect->x + xoff, y, otz, style);
    }
    win_drawHelpBorder(rect, otz, 1, 0, CONFIG_WINDOW_COLOR());
    inner.x = rect->x + 2;
    inner.y = rect->y + 2;
    inner.w = rect->w - 4;
    inner.h = rect->h - 4;
    win_drawHelpFill(&inner, otz);
}

void win_drawListBox(RECT *rect, s32 otz, s32 mode, MenuList *list) {
    RECT inner;
    s32 abr;

    abr = g_gameState->config >> CFG_BIT_WINDOW_COLOR;
    abr &= 1;

    if (mode == 1) {
        inner.x = rect->x;
        inner.y = rect->y + 4;
        inner.w = rect->w;
        inner.h = rect->h - 4;
        win_drawScrollArrows(rect, otz, list);
        ui_drawRule(rect->x, rect->y, otz, 0, rect->w - 2, abr);
        win_drawLineBorder(&inner, otz, 1, abr);
    } else {
        inner = *rect;
        win_drawLineBorder(&inner, otz, 0, abr);
    }
    if (list->scroll & MENULIST_SCROLL_PAGE) {
        win_drawSplitFill(&inner, otz, list, abr);
    } else {
        win_drawFill(&inner, otz, 0, list->fillV, abr);
    }
}

void win_updateAll(void) {
    s32 i;
    Window *w = g_windows;
    for (i = 0; i < 10; w++, i++) {
        u8 st = w->state;
        if (st == WIN_STATE_OPEN) {
            if (w->list.visibleRows < w->list.rows && w->list.scroll != 0) {
                menuList_update(&w->list);
            }
        } else if (st == WIN_STATE_OPENING) {
            win_stepOpen(w);
        } else if (st == WIN_STATE_CLOSING && g_drawBufs.uiMode != 2) {
            win_stepClose(w);
        }
    }
}

void win_reapClosed(void) {
    s32 i;
    Window *win = g_windows;
    for (i = 0; i < 10; win++, i++) {
        if (win->state == WIN_STATE_CLOSING) {
            win->state = WIN_STATE_CLOSED;
            if (win->onClosed != NULL) {
                win->onClosed(win);
            }
        }
    }
}

void win_setOnOpened(Window *win, void (*cb)(Window *)) {
    win->onOpened = cb;
}

void win_setDrawContent(Window *win, void (*cb)(Window *)) {
    win->drawContent = cb;
}

void win_setDrawOverlay(Window *win, void (*cb)(Window *)) {
    win->drawOverlay = cb;
}

void win_setOnClosed(Window *win, void (*cb)(Window *)) {
    win->onClosed = cb;
}

void win_setTitle(Window *win, s32 title) {
    win->titleX = WIN_TITLE_X;
    win->title = title;
    win->titleY = WIN_TITLE_Y;
}

void win_setShadow(Window *win, u8 shadow) {
    win->shadow = shadow;
}

void win_setTranslucent(Window *win, u8 translucent) {
    win->translucent = translucent;
}

void win_setVisible(Window *win, u8 visible) {
    win->visible = visible;
}

void win_setOpenFrames(Window *win, s16 frames) {
    win->openFrames = frames;
    if (win->style != WIN_STYLE_BALLOON) {
        win_setOpenAnim(win, win->animMode);
    } else {
        win_setTail(win, win->tailMode, win->tailX);
    }
}

void win_setCloseFrames(Window *win, s16 frames) {
    win->openFrames = frames;
    win_setCloseAnim(win, win->animMode);
}

void win_setList(Window *win, MenuList *list) {
    win->list = *list;
}

void win_setTitleAt(Window *win, s32 title, s16 x, s8 y) {
    win->title = title;
    win->titleX = x;
    win->titleY = y;
}

void win_drawFillPal(RECT *rect, s32 otz) {
    win_drawFill(rect, otz, 0, 0, CONFIG_WINDOW_COLOR());
}

Window *win_get(s32 i) {
    if (i >= 10) {
        SystemError(0x59, 0x53);
    }
    return &g_windows[i];
}

void win_initAll(void) {
    s32 i;
    for (i = 0; i < 10; i++) {
        g_windows[i].state = WIN_STATE_CLOSED;
        g_windows[i].slot = i;
    }
}

u8 win_isVisible(Window *w) {
    Unk80010CD0 min = g_winMinWidths;
    u8 ret = 0;

    if (w->visible == 0) {
        return ret;
    }
    if (w->style == WIN_STYLE_BEVEL || w->style == WIN_STYLE_BALLOON) {
        if (w->cur.w >= min.w[w->style] && w->cur.h >= 16) {
            ret = 1;
        }
    } else if ((u16)w->cur.w | (u16)w->cur.h) {
        ret = 1;
    }
    return ret;
}

void win_drawTitle(Window *w) {
    RECT clipRect;
    if (w->title == 0) {
        return;
    }
    if (w->state != WIN_STATE_OPEN) {
        win_setClip(NULL, w->otz, WIN_STYLE_FRAMELESS);
    }
    ui_drawLabel(w->x + w->titleX, w->cur.y + w->titleY, w->otz, w->title, w->active ? UI_LABEL_NORMAL : UI_LABEL_DIM);
    if (w->state != WIN_STATE_OPEN) {
        RECT *frame = &w->cur;
        clipRect.x = frame->x;
        clipRect.y = frame->y - 3;
        clipRect.w = frame->w;
        clipRect.h = 8;
        win_setClip(&clipRect, w->otz, WIN_STYLE_FRAMELESS);
    }
}

void win_drawTail(Window *w) {
    RECT *rect = &w->cur;
    s32 y;
    if (w->tailMode >= 2) {
        y = rect->y + rect->h - 2;
    } else {
        y = rect->y - 11;
    }
    win_drawMsgTailEdge(w->x + w->tailX, y, w->otz, w->tailMode, w->translucent);
    win_drawTailFill(w->x + w->tailX, y, w->otz, w->tailMode, (w->list.fillU + w->tailX) & 0x1F, w->list.fillV);
}

void win_drawScrollArrows(RECT *rect, s32 otz, MenuList *list) {
    s32 blink = ((u32)g_winLastVsync >> 4) & 1;
    u8 top = list->top;
    u8 more = top < list->rows - list->visibleRows;
    s32 z = otz;
    s32 x;
    s32 y;

    /* The two `&& (x = 0)` tests store only to x, which is dead (reassigned before use): flow deletes the
     * stores and jump2 deletes the then-empty branches, so they emit no code. They exist for the register
     * allocator: the first splits the block between the x and y arguments (as the original's schedule
     * needs) and keeps top live into a second block, the second adds references to more and blink. */
    ui_drawSprite(rect->x + rect->w - 6, (top != (u32)list && (x = 0), rect->y + 5), z, UI_SPRITE_SCROLL_UP,
                  top != 0 ? (more != blink && (x = 0), blink) : 1);
    y = rect->y + rect->h - 14;
    x = rect->x + rect->w - 6;
    ui_drawSprite(x, y, z, UI_SPRITE_SCROLL_DOWN, more ? blink : 1);
}

void win_drawSplitFill(RECT *rect, s32 otz, MenuList *list, s32 abr) {
    RECT part;

    win_drawDivider(rect, otz, list, abr);
    part.x = rect->x;
    part.w = rect->w;
    if (list->scroll & MENULIST_SCROLL_UP) {
        part.y = rect->y;
        part.h = list->scrollPos + 1;
        win_drawFill(&part, otz, 0, (list->fillV + (rect->h - list->scrollPos)) & 0x1F, abr);
        part.y = rect->y + list->scrollPos - 1;
        part.h = rect->h - list->scrollPos;
        win_drawFill(&part, otz, 0, (list->fillV + (list->scrollPos - 1)) & 0x1F, abr);
    } else {
        part.y = rect->y;
        part.h = rect->h - list->scrollPos;
        win_drawFill(&part, otz, 0, list->fillV, abr);
        part.y = rect->y + part.h;
        part.h = list->scrollPos;
        win_drawFill(&part, otz, 0, list->fillV, abr);
    }
}

void win_drawDivider(RECT *rect, s32 otz, MenuList *list, s32 style) {
    LineF2Words *line;
    u32 lineColor;
    s16 y;

    lineColor = g_winLineColors[style][(list->scroll & MENULIST_SCROLL_UP) ? 1 : 0];
    line = (LineF2Words *)g_primPtr;
    setlen(line, PRIM_LEN(LINE_F2));
    ((LineF2Words *)g_primPtr)->rgbc = lineColor;
    line->x0 = rect->x + 2;
    line->x1 = rect->x + rect->w - 3;
    if (list->scroll & MENULIST_SCROLL_UP) {
        y = rect->y + list->scrollPos + 4;
    } else {
        y = rect->y + rect->h - (list->scrollPos + 1) - 2;
    }
    line->y1 = y;
    line->y0 = y;
    addPrim(&g_ot[otz], line);
    g_primPtr += sizeof(LineF2Words);
}

Window *win_init(RECT *rect, s32 otz, s32 style, Window *w) {
    w->style = style;
    *(RECT *)w = *rect;
    w->state = WIN_STATE_OPENING;
    w->openFrames = 7;
    w->closeFrames = 5;
    w->visible = 1;
    w->translucent = 0;
    w->otz = otz;
    w->onClosed = 0;
    w->drawOverlay = 0;
    w->drawContent = 0;
    w->onOpened = 0;
    win_setOpenAnim(w, g_winStyleOpenAnim[style]);
    w->shadow = w->style == WIN_STYLE_BALLOON;
    menuList_init(&w->list, 0, 0, 0, rect->h);
    w->tailMode = 4;
    w->active = 1;
    w->title = 0;
    w->tailX = 0;
    return w;
}

INCLUDE_RODATA("asm/us/main/nonmatchings/window", g_winMinWidths);

INCLUDE_RODATA("asm/us/main/nonmatchings/window", g_winClipInsets);
