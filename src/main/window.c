#include "ff9.h"

void win_drawAll(void) {
    Window *w;
    s32 i;

    w = g_windows;
    for (i = 0; i < 10; w++, i++) {
        if (w->state != 0 && win_isVisible(w)) {
            if (w->title != 0) {
                win_drawTitle(w);
            }
            if (w->drawOverlay != NULL) {
                w->drawOverlay(w);
            }
            if (w->state == 3 && w->list.visibleRows < w->list.rows) {
                win_drawScrollArrows(&w->cur, w->otz, &w->list);
            }
            win_setClip(NULL, w->otz, w->style);
            if (w->drawContent != NULL && w->active != 0) {
                w->drawContent(w);
            }
            win_setClip(&w->cur, w->otz, w->style);
            if (w->style != 4) {
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
    RECT t;

    abr = g_gameState->config >> 18;
    abr &= 1;

    if (w->style == 2) {
        win_drawBorder(&w->cur, w->otz, w->active, w->translucent, abr);
        if (w->active == 0) {
            return;
        }
        t.x = w->cur.x + 4;
        t.y = w->cur.y + 4;
        t.w = w->cur.w - 8;
        t.h = w->cur.h - 8;
    } else if (w->style == 3) {
        if (w->tailMode != 4) {
            win_drawTail(w);
        }
        win_drawBalloonBorder(&w->cur, w->otz, w->active, w->translucent, abr);
        if (w->active == 0) {
            return;
        }
        t.x = w->cur.x + 2;
        t.y = w->cur.y + 2;
        t.w = w->cur.w - 4;
        t.h = w->cur.h - 4;
    } else {
        if (w->style == 1) {
            t.x = w->cur.x;
            t.y = w->cur.y + 4;
            t.w = w->cur.w;
            t.h = w->cur.h - 4;
            ui_drawRule(w->cur.x, w->cur.y, w->otz, 0, w->cur.w, abr);
            win_drawLineBorder(&t, w->otz, 1, abr);
        } else {
            t = w->cur;
            win_drawLineBorder(&t, w->otz, 0, abr);
        }
        if (w->active == 0) {
            return;
        }
        if (w->list.scroll & 4) {
            win_drawSplitFill(&t, w->otz, &w->list, abr);
            return;
        }
    }
    win_drawFill(&t, w->otz, w->list.unkA, w->list.fillV, abr);
}

void win_drawBoxEx(RECT *r, s32 otz, s32 style, s32 abr) {
    RECT t;

    switch (style) {
    case 2:
        win_drawBorder(r, otz, 1, 0, abr);
        t.x = r->x + 4;
        t.y = r->y + 4;
        t.w = r->w - 8;
        t.h = r->h - 8;
        win_drawFill(&t, otz, 0, 0, abr);
        break;
    case 1:
        t.x = r->x;
        t.y = r->y + 4;
        t.w = r->w;
        t.h = r->h - 4;
        ui_drawRule(r->x, r->y, otz, 0, r->w - 2, abr);
        win_drawLineBorder(&t, otz, 1, abr);
        win_drawFill(&t, otz, 0, 0, abr);
        break;
    case 0:
        win_drawLineBorder(r, otz, 0, abr);
        win_drawFill(r, otz, 0, 0, abr);
        break;
    case 3:
        win_drawBalloonBorder(r, otz, 1, 0, abr);
        t.x = r->x + 2;
        t.y = r->y + 2;
        t.w = r->w - 3;
        t.h = r->h - 3;
        win_drawFill(&t, otz, 0, 0, abr);
        break;
    }
}

void win_setOpenAnim(Window *w, s32 mode) {
    s16 h;

    w->animMode = mode;
    if (mode == 0) {
        w->stepW = ((w->w + w->openFrames - 0x11) / w->openFrames) & ~1;
        w->stepH = ((w->h + w->openFrames - 0x11) / w->openFrames) & ~1;
        w->cur.w = w->w - w->stepW * w->openFrames;
        w->cur.h = w->h - w->stepH * w->openFrames;
        w->cur.x = w->x + w->openFrames * (w->stepW >> 1);
        w->cur.y = w->y + w->openFrames * (w->stepH >> 1);
    } else if (mode == 4) {
        w->stepW = ((w->w + w->openFrames - 0x11) / w->openFrames) & ~1;
        w->stepH = 0;
        w->cur.h = w->h;
        w->cur.y = w->y;
        w->cur.w = w->w - w->stepW * w->openFrames;
        w->cur.x = w->x + w->openFrames * (w->stepW >> 1);
    } else if (mode == 1) {
        w->stepW = 0;
        w->list.fillV = 0;
        w->list.unkA = 0;
        h = w->h >> 2;
        w->cur.x = w->x;
        w->cur.y = w->y;
        w->cur.w = w->w;
        w->cur.h = h;
        w->stepH = (w->h - h + (w->openFrames - 1)) / w->openFrames;
    } else if (mode == 3) {
        w->cur = *(RECT *)w;
        w->state = mode;
    }
}

void win_setCloseAnim(Window *w, s32 mode) {
    w->animMode = mode;
    if (mode == 0 || mode == 4) {
        w->stepW = (w->w + w->closeFrames - 0x11) / w->closeFrames;
        w->stepH = (w->h + w->closeFrames - 0x11) / w->closeFrames;
    } else if (mode == 1) {
        w->stepW = 0;
        w->stepH = (w->cur.h + w->closeFrames - 1) / w->closeFrames;
    } else if (mode == 2) {
        w->stepLeft = (w->tailX + w->closeFrames - 9) / w->closeFrames;
        w->stepRight = (w->w - (w->tailX + 0x18) + w->closeFrames - 1) / w->closeFrames;
        w->stepH = (w->h + w->closeFrames - 0x11) / w->closeFrames;
    } else if (mode == 3) {
        w->state = 0;
    }
}

void win_setTail(Window *w, s32 mode, s32 off) {
    RECT *r = (RECT *)w;

    if (w->style != 4) {
        w->animMode = 2;
        if (mode == 1 || mode == 3) {
            off -= 0x10;
        }
        w->tailMode = mode;
        w->tailX = off;
        w->stepLeft = (off + w->openFrames - 9) / w->openFrames;
        w->stepRight = (r->w - (off + 0x18) + w->openFrames - 1) / w->openFrames;
        w->stepH = (w->openFrames + r->h - 0x11) / w->openFrames;
        w->cur.w = r->w - (w->stepLeft + w->stepRight) * w->openFrames;
        w->cur.h = r->h - w->stepH * w->openFrames;
        w->cur.x = r->x + w->stepLeft * w->openFrames;
        if ((u32)(mode - 2) < 2) {
            w->cur.y = r->y + r->h - (s16)(w->cur.h + 1);
        } else {
            w->cur.y = r->y;
        }
    }
}

void win_stepOpen(Window *w) {
    if (w->animMode == 0 || w->animMode == 4) {
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
        w->list.unkA = (w->cur.x - w->x) & 0x1F;
        w->list.fillV = (w->cur.y - w->y) & 0x1F;
    } else if (w->animMode == 1) {
        w->cur.w += w->stepW;
        w->cur.h += w->stepH;
        if (w->cur.w > w->w) {
            w->cur.w = w->w;
        }
        if (w->cur.h > w->h) {
            w->cur.h = w->h;
        }
        w->list.fillV = (w->h - w->cur.h) & 0x1F;
    } else if (w->animMode == 2) {
        s16 a = w->stepLeft + w->stepRight;
        w->cur.w += a;
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
        w->list.unkA = (w->cur.x - w->x) & 0x1F;
        w->list.fillV = (w->cur.y - w->y) & 0x1F;
    }
    if (*(s32 *)&w->cur.w == *(s32 *)&w->w) {
        w->state = 3;
        if (w->onOpened != NULL) {
            w->onOpened(w);
        }
    }
}

void win_stepClose(Window *w) {
    if (w->animMode == 0 || w->animMode == 4) {
        w->cur.w -= w->stepW;
        w->cur.h -= w->stepH;
        w->cur.x += w->stepW >> 1;
        w->cur.y += w->stepH >> 1;
        w->list.unkA = (w->cur.x - w->x) & 0x1F;
        w->list.fillV = (w->cur.y - w->y) & 0x1F;
    } else if (w->animMode == 1) {
        w->cur.h -= w->stepH;
        w->cur.w -= w->stepW;
        w->list.fillV = (w->h - w->cur.h) & 0x1F;
    } else if (w->animMode == 2) {
        w->cur.w -= w->stepLeft + w->stepRight;
        w->cur.h -= w->stepH;
        w->cur.x += w->stepLeft;
        if (w->tailMode >= 2) {
            w->cur.y += w->stepH;
        }
        w->list.unkA = (w->cur.x - w->x) & 0x1F;
        w->list.fillV = (w->cur.y - w->y) & 0x1F;
    }
    if (w->cur.w < 0) {
        w->cur.w = 0;
    }
    if (w->cur.h < 0) {
        w->cur.h = 0;
    }
    if (w->cur.w == 0 || w->cur.h == 0) {
        w->state = 0;
        if (w->onClosed != NULL) {
            w->onClosed(w);
        }
    }
}

void win_setClip(RECT *r, s32 otz, s32 style) {
    RECT t;
    Unk80010CE0 offs;
    DR_AREA *p;
    RECT *clip;
    s8 *o;

    clip = &g_gameState->dispEnv[g_drawBufs.cur ^ 1].disp;
    offs = D_80010CE0;
    p = (DR_AREA *)g_primPtr;
    o = offs.unk0[style];
    if (r != NULL) {
        t.x = g_drawBufs.dispPos[g_drawBufs.cur ^ 1].vx + (r->x + o[0]);
        t.y = g_drawBufs.dispPos[g_drawBufs.cur ^ 1].vy + (r->y + o[1]);
        t.w = r->w + o[2];
        t.h = r->h + o[3];
        if (t.x < clip->x) {
            t.w -= clip->x - t.x;
            t.x = clip->x;
        }
        if (t.y < clip->y) {
            t.h -= clip->y - t.y;
            t.y = clip->y;
        }
        if (t.x + t.w > clip->x + clip->w) {
            t.w = clip->x + clip->w - t.x;
        }
        if (t.y + t.h > clip->y + clip->h) {
            t.h = clip->y + clip->h - t.y;
        }
    } else {
        t = *clip;
    }
    SetDrawArea(p, &t);
    addPrim(&g_ot[otz], p);
    g_primPtr += sizeof(DR_AREA);
}

Window *win_openFree(RECT *r, s32 a, s32 b) {
    s32 i;
    Window *w = g_windows;
    Window *res;
    for (i = 0; i < 10; i++, w++) {
        if (w->state == 0) {
            break;
        }
    }
    if (i < 10) {
        res = win_init(r, a, b, w);
    } else {
        res = NULL;
    }
    return res;
}

Window *win_open(RECT *r, s32 a, s32 b, u32 i) {
    Window *res;
    if (i < 10) {
        res = win_init(r, a, b, &g_windows[i]);
    } else {
        res = NULL;
    }
    return res;
}

void win_close(Window *w) {
    if (w != NULL && w->state != 0 && w->state != 2) {
        w->state = 2;
        if (g_drawBufs.unk1 != 2) {
            win_setCloseAnim(w, w->animMode);
        }
    }
}

void win_closeAll(void) {
    s32 i;
    Window *w = g_windows;
    for (i = 0; i < 10; w++, i++) {
        if (w->state != 0 && w->state != 2) {
            win_close_inline(w);
        }
    }
}

void win_closeNow(Window *w) {
    if (w != NULL && w->state != 0) {
        if (w->onClosed != NULL) {
            w->onClosed(w);
        }
        w->state = 0;
    }
}

void win_closeAllNow(void) {
    s32 i;
    Window *w = g_windows;
    for (i = 0; i < 10; w++, i++) {
        if (w->state != 0 && w != NULL) {
            if (w->onClosed != NULL) {
                w->onClosed(w);
            }
            w->state = 0;
        }
    }
}

void win_drawBox(RECT *a, s32 b, s32 c) {
    win_drawBoxEx(a, b, c, CONFIG_WINDOW_COLOR());
}

void win_drawHelpFrame(RECT *r, s32 otz, s32 style, s32 xoff) {
    RECT t;
    s32 y;
    if (style != 4) {
        if (style >= 2) {
            y = r->y + r->h - 2;
        } else {
            y = r->y - 11;
        }
        win_drawHelpTailEdge(r->x + xoff, y, otz, style, 0);
        win_drawHelpTailFill(r->x + xoff, y, otz, style);
    }
    win_drawHelpBorder(r, otz, 1, 0, CONFIG_WINDOW_COLOR());
    t.x = r->x + 2;
    t.y = r->y + 2;
    t.w = r->w - 4;
    t.h = r->h - 4;
    win_drawHelpFill(&t, otz);
}

void win_drawListBox(RECT *r, s32 otz, s32 mode, MenuList *m) {
    RECT t;
    s32 abr;

    abr = g_gameState->config >> 18;
    abr &= 1;

    if (mode == 1) {
        t.x = r->x;
        t.y = r->y + 4;
        t.w = r->w;
        t.h = r->h - 4;
        win_drawScrollArrows(r, otz, m);
        ui_drawRule(r->x, r->y, otz, 0, r->w - 2, abr);
        win_drawLineBorder(&t, otz, 1, abr);
    } else {
        t = *r;
        win_drawLineBorder(&t, otz, 0, abr);
    }
    if (m->scroll & 4) {
        win_drawSplitFill(&t, otz, m, abr);
    } else {
        win_drawFill(&t, otz, 0, m->fillV, abr);
    }
}

void win_updateAll(void) {
    s32 i;
    Window *w = g_windows;
    for (i = 0; i < 10; w++, i++) {
        u8 st = w->state;
        if (st == 3) {
            if (w->list.visibleRows < w->list.rows && w->list.scroll != 0) {
                menuList_update(&w->list);
            }
        } else if (st == 1) {
            win_stepOpen(w);
        } else if (st == 2 && g_drawBufs.unk1 != 2) {
            win_stepClose(w);
        }
    }
}

void win_reapClosed(void) {
    s32 i;
    Window *p = g_windows;
    for (i = 0; i < 10; p++, i++) {
        if (p->state == 2) {
            p->state = 0;
            if (p->onClosed != NULL) {
                p->onClosed(p);
            }
        }
    }
}

void win_setOnOpened(Window *arg0, void (*arg1)(Window *)) {
    arg0->onOpened = arg1;
}

void win_setDrawContent(Window *arg0, void (*arg1)(Window *)) {
    arg0->drawContent = arg1;
}

void win_setDrawOverlay(Window *arg0, void (*arg1)(Window *)) {
    arg0->drawOverlay = arg1;
}

void win_setOnClosed(Window *arg0, void (*arg1)(Window *)) {
    arg0->onClosed = arg1;
}

void win_setTitle(Window *arg0, s32 arg1) {
    arg0->titleX = 6;
    arg0->title = arg1;
    arg0->titleY = -3;
}

void win_setShadow(Window *arg0, u8 arg1) {
    arg0->shadow = arg1;
}

void win_setTranslucent(Window *arg0, u8 arg1) {
    arg0->translucent = arg1;
}

void win_setVisible(Window *arg0, u8 arg1) {
    arg0->visible = arg1;
}

void win_setOpenFrames(Window *arg0, s16 arg1) {
    arg0->openFrames = arg1;
    if (arg0->style != 3) {
        win_setOpenAnim(arg0, arg0->animMode);
    } else {
        win_setTail(arg0, arg0->tailMode, arg0->tailX);
    }
}

void win_setCloseFrames(Window *arg0, s16 arg1) {
    arg0->openFrames = arg1;
    win_setCloseAnim(arg0, arg0->animMode);
}

void win_setList(Window *arg0, MenuList *arg1) {
    arg0->list = *arg1;
}

void win_setTitleAt(Window *arg0, s32 arg1, s16 arg2, s8 arg3) {
    arg0->title = arg1;
    arg0->titleX = arg2;
    arg0->titleY = arg3;
}

void win_drawFillPal(RECT *a, s32 b) {
    win_drawFill(a, b, 0, 0, CONFIG_WINDOW_COLOR());
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
        g_windows[i].state = 0;
        g_windows[i].slot = i;
    }
}

u8 win_isVisible(Window *w) {
    Unk80010CD0 min = D_80010CD0;
    u8 r = 0;

    if (w->visible == 0) {
        return r;
    }
    if (w->style == 2 || w->style == 3) {
        if (w->cur.w >= min.unk0[w->style] && w->cur.h >= 16) {
            r = 1;
        }
    } else if ((u16)w->cur.w | (u16)w->cur.h) {
        r = 1;
    }
    return r;
}

void win_drawTitle(Window *w) {
    RECT r;
    if (w->title == 0) {
        return;
    }
    if (w->state != 3) {
        win_setClip(NULL, w->otz, 4);
    }
    ui_drawLabel(w->x + w->titleX, w->cur.y + w->titleY, w->otz, w->title, w->active ? 0 : 2);
    if (w->state != 3) {
        RECT *s = &w->cur;
        r.x = s->x;
        r.y = s->y - 3;
        r.w = s->w;
        r.h = 8;
        win_setClip(&r, w->otz, 4);
    }
}

void win_drawTail(Window *w) {
    RECT *r = &w->cur;
    s32 y;
    if (w->tailMode >= 2) {
        y = r->y + r->h - 2;
    } else {
        y = r->y - 11;
    }
    win_drawMsgTailEdge(w->x + w->tailX, y, w->otz, w->tailMode, w->translucent);
    win_drawTailFill(w->x + w->tailX, y, w->otz, w->tailMode, (w->list.unkA + w->tailX) & 0x1F, w->list.fillV);
}

void win_drawScrollArrows(RECT *r, s32 otz, MenuList *m) {
    s32 blink = ((u32)g_winLastVsync >> 4) & 1;
    u8 top = m->top;
    u8 more = top < m->rows - m->visibleRows;
    s32 z = otz;
    s32 x;
    s32 y;

    /* The two `&& (x = 0)` tests store only to x, which is dead (reassigned before use): flow deletes the
     * stores and jump2 deletes the then-empty branches, so they emit no code. They exist for the register
     * allocator: the first splits the block between the x and y arguments (as the original's schedule
     * needs) and keeps top live into a second block, the second adds references to more and blink. */
    ui_drawSprite(r->x + r->w - 6, (top != (u32)m && (x = 0), r->y + 5), z, 0xB8,
                  top != 0 ? (more != blink && (x = 0), blink) : 1);
    y = r->y + r->h - 14;
    x = r->x + r->w - 6;
    ui_drawSprite(x, y, z, 0xB9, more ? blink : 1);
}

void win_drawSplitFill(RECT *r, s32 otz, MenuList *m, s32 abr) {
    RECT t;

    win_drawDivider(r, otz, m, abr);
    t.x = r->x;
    t.w = r->w;
    if (m->scroll & 2) {
        t.y = r->y;
        t.h = m->scrollPos + 1;
        win_drawFill(&t, otz, 0, (m->fillV + (r->h - m->scrollPos)) & 0x1F, abr);
        t.y = r->y + m->scrollPos - 1;
        t.h = r->h - m->scrollPos;
        win_drawFill(&t, otz, 0, (m->fillV + (m->scrollPos - 1)) & 0x1F, abr);
    } else {
        t.y = r->y;
        t.h = r->h - m->scrollPos;
        win_drawFill(&t, otz, 0, m->fillV, abr);
        t.y = r->y + t.h;
        t.h = m->scrollPos;
        win_drawFill(&t, otz, 0, m->fillV, abr);
    }
}

void win_drawDivider(RECT *r, s32 otz, MenuList *m, s32 style) {
    LineF2Words *p;
    u32 c;
    s16 y;

    c = g_winLineColors[style][(m->scroll & 2) ? 1 : 0];
    p = (LineF2Words *)g_primPtr;
    setlen(p, 3);
    ((LineF2Words *)g_primPtr)->rgbc = c;
    p->x0 = r->x + 2;
    p->x1 = r->x + r->w - 3;
    if (m->scroll & 2) {
        y = r->y + m->scrollPos + 4;
    } else {
        y = r->y + r->h - (m->scrollPos + 1) - 2;
    }
    p->y1 = y;
    p->y0 = y;
    addPrim(&g_ot[otz], p);
    g_primPtr += sizeof(LineF2Words);
}

Window *win_init(RECT *r, s32 otz, s32 style, Window *w) {
    w->style = style;
    *(RECT *)w = *r;
    w->state = 1;
    w->openFrames = 7;
    w->closeFrames = 5;
    w->visible = 1;
    w->translucent = 0;
    w->otz = otz;
    w->onClosed = 0;
    w->drawOverlay = 0;
    w->drawContent = 0;
    w->onOpened = 0;
    win_setOpenAnim(w, D_8006BCFC[style]);
    w->shadow = w->style == 3;
    menuList_init(&w->list, 0, 0, 0, r->h);
    w->tailMode = 4;
    w->active = 1;
    w->title = 0;
    w->tailX = 0;
    return w;
}

INCLUDE_RODATA("asm/us/main/nonmatchings/window", D_80010CD0);

INCLUDE_RODATA("asm/us/main/nonmatchings/window", D_80010CE0);
