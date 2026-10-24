#include "ff9.h"

void win_drawAll(void) {
    Window *w;
    s32 i;

    w = g_windows;
    for (i = 0; i < 10; w++, i++) {
        if (w->unk1C != 0 && win_isVisible(w)) {
            if (w->unk24 != 0) {
                win_drawTitle(w);
            }
            if (w->unk44 != NULL) {
                w->unk44(w);
            }
            if (w->unk1C == 3 && w->unk30.unk3 < w->unk30.unk2) {
                win_drawScrollArrows(&w->unk8, w->unk18, &w->unk30);
            }
            win_setClip(NULL, w->unk18, w->unk1A);
            if (w->unk40 != NULL && w->unk1E != 0) {
                w->unk40(w);
            }
            win_setClip(&w->unk8, w->unk18, w->unk1A);
            if (w->unk1A != 4) {
                win_drawFrame(w);
            }
            if (w->unk1F != 0) {
                win_drawShadow(&w->unk8, w->unk18, w->unk1A);
            }
        }
    }
}

void win_drawFrame(Window *w) {
    s32 abr;
    RECT t;

    abr = g_gameState->unk24 >> 18;
    abr &= 1;

    if (w->unk1A == 2) {
        win_drawBorder(&w->unk8, w->unk18, w->unk1E, w->unk1D, abr);
        if (w->unk1E == 0) {
            return;
        }
        t.x = w->unk8.x + 4;
        t.y = w->unk8.y + 4;
        t.w = w->unk8.w - 8;
        t.h = w->unk8.h - 8;
    } else if (w->unk1A == 3) {
        if (w->unk29 != 4) {
            win_drawTail(w);
        }
        win_drawBalloonBorder(&w->unk8, w->unk18, w->unk1E, w->unk1D, abr);
        if (w->unk1E == 0) {
            return;
        }
        t.x = w->unk8.x + 2;
        t.y = w->unk8.y + 2;
        t.w = w->unk8.w - 4;
        t.h = w->unk8.h - 4;
    } else {
        if (w->unk1A == 1) {
            t.x = w->unk8.x;
            t.y = w->unk8.y + 4;
            t.w = w->unk8.w;
            t.h = w->unk8.h - 4;
            ui_drawRule(w->unk8.x, w->unk8.y, w->unk18, 0, w->unk8.w, abr);
            win_drawLineBorder(&t, w->unk18, 1, abr);
        } else {
            t = w->unk8;
            win_drawLineBorder(&t, w->unk18, 0, abr);
        }
        if (w->unk1E == 0) {
            return;
        }
        if (w->unk30.unk8 & 4) {
            win_drawSplitFill(&t, w->unk18, &w->unk30, abr);
            return;
        }
    }
    win_drawFill(&t, w->unk18, w->unk30.unkA, w->unk30.unkB, abr);
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

    w->unk28 = mode;
    if (mode == 0) {
        w->unk14 = ((w->unk4 + w->unk10 - 0x11) / w->unk10) & ~1;
        w->unk16 = ((w->unk6 + w->unk10 - 0x11) / w->unk10) & ~1;
        w->unk8.w = w->unk4 - w->unk14 * w->unk10;
        w->unk8.h = w->unk6 - w->unk16 * w->unk10;
        w->unk8.x = w->unk0 + w->unk10 * (w->unk14 >> 1);
        w->unk8.y = w->unk2 + w->unk10 * (w->unk16 >> 1);
    } else if (mode == 4) {
        w->unk14 = ((w->unk4 + w->unk10 - 0x11) / w->unk10) & ~1;
        w->unk16 = 0;
        w->unk8.h = w->unk6;
        w->unk8.y = w->unk2;
        w->unk8.w = w->unk4 - w->unk14 * w->unk10;
        w->unk8.x = w->unk0 + w->unk10 * (w->unk14 >> 1);
    } else if (mode == 1) {
        w->unk14 = 0;
        w->unk30.unkB = 0;
        w->unk30.unkA = 0;
        h = w->unk6 >> 2;
        w->unk8.x = w->unk0;
        w->unk8.y = w->unk2;
        w->unk8.w = w->unk4;
        w->unk8.h = h;
        w->unk16 = (w->unk6 - h + (w->unk10 - 1)) / w->unk10;
    } else if (mode == 3) {
        w->unk8 = *(RECT *)w;
        w->unk1C = mode;
    }
}

void win_setCloseAnim(Window *w, s32 mode) {
    w->unk28 = mode;
    if (mode == 0 || mode == 4) {
        w->unk14 = (w->unk4 + w->unk12 - 0x11) / w->unk12;
        w->unk16 = (w->unk6 + w->unk12 - 0x11) / w->unk12;
    } else if (mode == 1) {
        w->unk14 = 0;
        w->unk16 = (w->unk8.h + w->unk12 - 1) / w->unk12;
    } else if (mode == 2) {
        w->unk2C = (w->unk2A + w->unk12 - 9) / w->unk12;
        w->unk2E = (w->unk4 - (w->unk2A + 0x18) + w->unk12 - 1) / w->unk12;
        w->unk16 = (w->unk6 + w->unk12 - 0x11) / w->unk12;
    } else if (mode == 3) {
        w->unk1C = 0;
    }
}

void win_setTail(Window *w, s32 mode, s32 off) {
    RECT *r = (RECT *)w;

    if (w->unk1A != 4) {
        w->unk28 = 2;
        if (mode == 1 || mode == 3) {
            off -= 0x10;
        }
        w->unk29 = mode;
        w->unk2A = off;
        w->unk2C = (off + w->unk10 - 9) / w->unk10;
        w->unk2E = (r->w - (off + 0x18) + w->unk10 - 1) / w->unk10;
        w->unk16 = (w->unk10 + r->h - 0x11) / w->unk10;
        w->unk8.w = r->w - (w->unk2C + w->unk2E) * w->unk10;
        w->unk8.h = r->h - w->unk16 * w->unk10;
        w->unk8.x = r->x + w->unk2C * w->unk10;
        if ((u32)(mode - 2) < 2) {
            w->unk8.y = r->y + r->h - (s16)(w->unk8.h + 1);
        } else {
            w->unk8.y = r->y;
        }
    }
}

void win_stepOpen(Window *w) {
    if (w->unk28 == 0 || w->unk28 == 4) {
        w->unk8.w += w->unk14;
        w->unk8.h += w->unk16;
        w->unk8.x -= w->unk14 >> 1;
        w->unk8.y -= w->unk16 >> 1;
        if (w->unk8.w > w->unk4) {
            w->unk8.w = w->unk4;
        }
        if (w->unk8.h > w->unk6) {
            w->unk8.h = w->unk6;
        }
        if (w->unk8.x < w->unk0) {
            w->unk8.x = w->unk0;
        }
        if (w->unk8.y < w->unk2) {
            w->unk8.y = w->unk2;
        }
        w->unk30.unkA = (w->unk8.x - w->unk0) & 0x1F;
        w->unk30.unkB = (w->unk8.y - w->unk2) & 0x1F;
    } else if (w->unk28 == 1) {
        w->unk8.w += w->unk14;
        w->unk8.h += w->unk16;
        if (w->unk8.w > w->unk4) {
            w->unk8.w = w->unk4;
        }
        if (w->unk8.h > w->unk6) {
            w->unk8.h = w->unk6;
        }
        w->unk30.unkB = (w->unk6 - w->unk8.h) & 0x1F;
    } else if (w->unk28 == 2) {
        s16 a = w->unk2C + w->unk2E;
        w->unk8.w += a;
        w->unk8.h += w->unk16;
        w->unk8.x -= w->unk2C;
        if (w->unk29 >= 2) {
            w->unk8.y -= w->unk16;
        }
        if (w->unk8.w > w->unk4) {
            w->unk8.w = w->unk4;
        }
        if (w->unk8.h > w->unk6) {
            w->unk8.h = w->unk6;
        }
        if (w->unk8.x < w->unk0) {
            w->unk8.x = w->unk0;
        }
        if (w->unk8.y < w->unk2) {
            w->unk8.y = w->unk2;
        }
        w->unk30.unkA = (w->unk8.x - w->unk0) & 0x1F;
        w->unk30.unkB = (w->unk8.y - w->unk2) & 0x1F;
    }
    if (*(s32 *)&w->unk8.w == *(s32 *)&w->unk4) {
        w->unk1C = 3;
        if (w->unk48 != NULL) {
            w->unk48(w);
        }
    }
}

void win_stepClose(Window *w) {
    if (w->unk28 == 0 || w->unk28 == 4) {
        w->unk8.w -= w->unk14;
        w->unk8.h -= w->unk16;
        w->unk8.x += w->unk14 >> 1;
        w->unk8.y += w->unk16 >> 1;
        w->unk30.unkA = (w->unk8.x - w->unk0) & 0x1F;
        w->unk30.unkB = (w->unk8.y - w->unk2) & 0x1F;
    } else if (w->unk28 == 1) {
        w->unk8.h -= w->unk16;
        w->unk8.w -= w->unk14;
        w->unk30.unkB = (w->unk6 - w->unk8.h) & 0x1F;
    } else if (w->unk28 == 2) {
        w->unk8.w -= w->unk2C + w->unk2E;
        w->unk8.h -= w->unk16;
        w->unk8.x += w->unk2C;
        if (w->unk29 >= 2) {
            w->unk8.y += w->unk16;
        }
        w->unk30.unkA = (w->unk8.x - w->unk0) & 0x1F;
        w->unk30.unkB = (w->unk8.y - w->unk2) & 0x1F;
    }
    if (w->unk8.w < 0) {
        w->unk8.w = 0;
    }
    if (w->unk8.h < 0) {
        w->unk8.h = 0;
    }
    if (w->unk8.w == 0 || w->unk8.h == 0) {
        w->unk1C = 0;
        if (w->unk4C != NULL) {
            w->unk4C(w);
        }
    }
}

void win_setClip(RECT *r, s32 otz, s32 style) {
    RECT t;
    Unk80010CE0 offs;
    DR_AREA *p;
    RECT *clip;
    s8 *o;

    clip = &g_gameState->unkC[g_drawBufs.cur ^ 1].disp;
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
        if (w->unk1C == 0) {
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
    if (w != NULL && w->unk1C != 0 && w->unk1C != 2) {
        w->unk1C = 2;
        if (g_drawBufs.unk1 != 2) {
            win_setCloseAnim(w, w->unk28);
        }
    }
}

void win_closeAll(void) {
    s32 i;
    Window *w = g_windows;
    for (i = 0; i < 10; w++, i++) {
        if (w->unk1C != 0 && w->unk1C != 2) {
            win_close_inline(w);
        }
    }
}

void win_closeNow(Window *w) {
    if (w != NULL && w->unk1C != 0) {
        if (w->unk4C != NULL) {
            w->unk4C(w);
        }
        w->unk1C = 0;
    }
}

void win_closeAllNow(void) {
    s32 i;
    Window *w = g_windows;
    for (i = 0; i < 10; w++, i++) {
        if (w->unk1C != 0 && w != NULL) {
            if (w->unk4C != NULL) {
                w->unk4C(w);
            }
            w->unk1C = 0;
        }
    }
}

void win_drawBox(RECT *a, s32 b, s32 c) {
    win_drawBoxEx(a, b, c, (g_gameState->unk24 >> 18) & 1);
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
    win_drawHelpBorder(r, otz, 1, 0, (g_gameState->unk24 >> 18) & 1);
    t.x = r->x + 2;
    t.y = r->y + 2;
    t.w = r->w - 4;
    t.h = r->h - 4;
    win_drawHelpFill(&t, otz);
}

void win_drawListBox(RECT *r, s32 otz, s32 mode, MenuList *m) {
    RECT t;
    s32 abr;

    abr = g_gameState->unk24 >> 18;
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
    if (m->unk8 & 4) {
        win_drawSplitFill(&t, otz, m, abr);
    } else {
        win_drawFill(&t, otz, 0, m->unkB, abr);
    }
}

void win_updateAll(void) {
    s32 i;
    Window *w = g_windows;
    for (i = 0; i < 10; w++, i++) {
        u8 st = w->unk1C;
        if (st == 3) {
            if (w->unk30.unk3 < w->unk30.unk2 && w->unk30.unk8 != 0) {
                func_8002EB74(&w->unk30);
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
        if (p->unk1C == 2) {
            p->unk1C = 0;
            if (p->unk4C != NULL) {
                p->unk4C(p);
            }
        }
    }
}

void win_setOnOpened(Window *arg0, void (*arg1)(Window *)) {
    arg0->unk48 = arg1;
}

void win_setDrawContent(Window *arg0, void (*arg1)(Window *)) {
    arg0->unk40 = arg1;
}

void win_setDrawOverlay(Window *arg0, void (*arg1)(Window *)) {
    arg0->unk44 = arg1;
}

void win_setOnClosed(Window *arg0, void (*arg1)(Window *)) {
    arg0->unk4C = arg1;
}

void win_setTitle(Window *arg0, s32 arg1) {
    arg0->unk20 = 6;
    arg0->unk24 = arg1;
    arg0->unk22 = -3;
}

void win_setShadow(Window *arg0, u8 arg1) {
    arg0->unk1F = arg1;
}

void win_setTranslucent(Window *arg0, u8 arg1) {
    arg0->unk1D = arg1;
}

void win_setVisible(Window *arg0, u8 arg1) {
    arg0->unk23 = arg1;
}

void win_setOpenFrames(Window *arg0, s16 arg1) {
    arg0->unk10 = arg1;
    if (arg0->unk1A != 3) {
        win_setOpenAnim(arg0, arg0->unk28);
    } else {
        win_setTail(arg0, arg0->unk29, arg0->unk2A);
    }
}

void win_setCloseFrames(Window *arg0, s16 arg1) {
    arg0->unk10 = arg1;
    win_setCloseAnim(arg0, arg0->unk28);
}

void win_setList(Window *arg0, MenuList *arg1) {
    arg0->unk30 = *arg1;
}

void win_setTitleAt(Window *arg0, s32 arg1, s16 arg2, s8 arg3) {
    arg0->unk24 = arg1;
    arg0->unk20 = arg2;
    arg0->unk22 = arg3;
}

void win_drawFillPal(RECT *a, s32 b) {
    win_drawFill(a, b, 0, 0, (g_gameState->unk24 >> 18) & 1);
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
        g_windows[i].unk1C = 0;
        g_windows[i].unk1B = i;
    }
}

u8 win_isVisible(Window *w) {
    Unk80010CD0 min = D_80010CD0;
    u8 r = 0;

    if (w->unk23 == 0) {
        return r;
    }
    if (w->unk1A == 2 || w->unk1A == 3) {
        if (w->unk8.w >= min.unk0[w->unk1A] && w->unk8.h >= 16) {
            r = 1;
        }
    } else if ((u16)w->unk8.w | (u16)w->unk8.h) {
        r = 1;
    }
    return r;
}

void win_drawTitle(Window *w) {
    RECT r;
    if (w->unk24 == 0) {
        return;
    }
    if (w->unk1C != 3) {
        win_setClip(NULL, w->unk18, 4);
    }
    ui_drawLabel(w->unk0 + w->unk20, w->unk8.y + w->unk22, w->unk18, w->unk24, w->unk1E ? 0 : 2);
    if (w->unk1C != 3) {
        RECT *s = &w->unk8;
        r.x = s->x;
        r.y = s->y - 3;
        r.w = s->w;
        r.h = 8;
        win_setClip(&r, w->unk18, 4);
    }
}

void win_drawTail(Window *w) {
    RECT *r = &w->unk8;
    s32 y;
    if (w->unk29 >= 2) {
        y = r->y + r->h - 2;
    } else {
        y = r->y - 11;
    }
    win_drawMsgTailEdge(w->unk0 + w->unk2A, y, w->unk18, w->unk29, w->unk1D);
    win_drawTailFill(w->unk0 + w->unk2A, y, w->unk18, w->unk29, (w->unk30.unkA + w->unk2A) & 0x1F, w->unk30.unkB);
}

void win_drawScrollArrows(RECT *r, s32 otz, MenuList *m) {
    s32 blink = ((u32)g_winLastVsync >> 4) & 1;
    u8 top = m->unk5;
    u8 more = top < m->unk2 - m->unk3;
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
    if (m->unk8 & 2) {
        t.y = r->y;
        t.h = m->unk9 + 1;
        win_drawFill(&t, otz, 0, (m->unkB + (r->h - m->unk9)) & 0x1F, abr);
        t.y = r->y + m->unk9 - 1;
        t.h = r->h - m->unk9;
        win_drawFill(&t, otz, 0, (m->unkB + (m->unk9 - 1)) & 0x1F, abr);
    } else {
        t.y = r->y;
        t.h = r->h - m->unk9;
        win_drawFill(&t, otz, 0, m->unkB, abr);
        t.y = r->y + t.h;
        t.h = m->unk9;
        win_drawFill(&t, otz, 0, m->unkB, abr);
    }
}

void win_drawDivider(RECT *r, s32 otz, MenuList *m, s32 style) {
    LineF2Words *p;
    u32 c;
    s16 y;

    c = D_80068F20[style][(m->unk8 & 2) ? 1 : 0];
    p = (LineF2Words *)g_primPtr;
    setlen(p, 3);
    ((LineF2Words *)g_primPtr)->rgbc = c;
    p->x0 = r->x + 2;
    p->x1 = r->x + r->w - 3;
    if (m->unk8 & 2) {
        y = r->y + m->unk9 + 4;
    } else {
        y = r->y + r->h - (m->unk9 + 1) - 2;
    }
    p->y1 = y;
    p->y0 = y;
    addPrim(&g_ot[otz], p);
    g_primPtr += sizeof(LineF2Words);
}

Window *win_init(RECT *r, s32 otz, s32 style, Window *w) {
    w->unk1A = style;
    *(RECT *)w = *r;
    w->unk1C = 1;
    w->unk10 = 7;
    w->unk12 = 5;
    w->unk23 = 1;
    w->unk1D = 0;
    w->unk18 = otz;
    w->unk4C = 0;
    w->unk44 = 0;
    w->unk40 = 0;
    w->unk48 = 0;
    win_setOpenAnim(w, D_8006BCFC[style]);
    w->unk1F = w->unk1A == 3;
    func_8002EDFC(&w->unk30, 0, 0, 0, r->h);
    w->unk29 = 4;
    w->unk1E = 1;
    w->unk24 = 0;
    w->unk2A = 0;
    return w;
}

INCLUDE_RODATA("asm/us/main/nonmatchings/window", D_80010CD0);

INCLUDE_RODATA("asm/us/main/nonmatchings/window", D_80010CE0);
