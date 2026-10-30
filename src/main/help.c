#include "ff9.h"

void help_draw(void) {
    s32 otz;
    HelpLayout r;

    otz = 1;
    if (g_menuHelp.cursorOnTop == 0) {
        otz = g_menuHelp.otz;
    }
    do {
    } while (0); /* empty debug macro in the original; changes the register allocation */
    if (g_menuHelp.follow != 0) {
        if (g_menuHelp.cursorOnTop != 0) {
            ui_drawHand(g_menuHelp.x, g_menuHelp.y, otz);
        }
        sprAnim_draw(g_menuHelp.x + 5, g_menuHelp.y + 2, otz, &g_menuHelp.iconAnim);
    }
    func_8003573C();
    if (g_menuHelp.textShown != 0) {
        help_layout(&r);
        ui_drawSprite(r.rect.x + 8, r.rect.y, otz, 0xB7, 0);
        text_drawBlock(r.rect.x + 6, r.rect.y + 8, otz, g_menuHelp.text, 8);
        win_drawHelpFrame(&r.rect, otz, r.style, r.tailX);
    }
}

void help_layout(HelpLayout *w) {
    RECT *r = &w->rect;
    RECT *c = &g_menuHelp.bounds;
    HelpState *m = &g_menuHelp;
    TextSize sz;
    s32 x;
    s32 v;
    s32 d1;
    s32 d2;
    s32 t;

    sz = text_measure(m->text);
    w->rect.w = sz.w + 12;
    w->rect.h = sz.h + 12;
    if (w->rect.w >= c->w) {
        w->rect.w = c->w;
    }
    if (w->rect.h >= c->h) {
        w->rect.h = c->h;
    }
    if (m->follow == 0) {
        w->style = 4;
        w->rect.x = m->x;
        w->rect.y = m->y;
        return;
    }
    v = m->y;
    t = c->y + 16;
    d2 = v - t;
    d1 = c->y + c->h - 10;
    d1 -= v;
    if (d1 < d2) {
        w->style = 2;
        w->rect.y = m->y - (s16)(w->rect.h + 16);
    } else {
        w->style = 0;
        w->rect.y = m->y + 10;
    }
    v = g_menuHelp.x;
    t = c->x + 19;
    d2 = v - t;
    d1 = c->x + c->w - 18;
    d1 -= v;
    if (d1 < d2) {
        w->style |= 1;
        x = g_menuHelp.x - 19;
    } else {
        x = v + 18;
    }
    r->x = x - (r->w >> 1);
    if (r->x < c->x) {
        r->x = c->x;
    }
    if (r->x + r->w > c->x + c->w) {
        r->x = c->x + c->w - r->w;
    }
    if (x - r->x < 8) {
        w->tailX = 8;
    } else {
        w->tailX = x - r->x;
    }
}

void help_init(void) {
    g_menuHelp.isOpen = 0;
    g_menuHelp.callback = 0;
    g_menuHelp.text = 0;
    g_menuHelp.textShown = 0;
    g_menuHelp.opening = 0;
    g_menuHelp.locked = 0;
    g_menuHelp.follow = 1;
    g_menuHelp.cursorOnTop = 1;
    help_setBounds(NULL);
    sprAnim_init(&g_menuHelp.iconAnim, &g_helpIconAnimData);
}

void help_update(void) {
    HelpState *p;
    if (g_menuHelp.locked == 0 && fade_isBusy() == 0 && (g_padRepeat & BTN_SELECT)) {
        help_toggle();
    }
    p = &g_menuHelp;
    if (p->isOpen != 0 && fade_isBusy() == 0 && p->callback != NULL) {
        p->callback();
        if (p->text != NULL && *p->text != 0xFF) {
            help_draw();
        }
    }
    sprAnim_update(&g_menuHelp.iconAnim);
}

void func_80035564(void) {
}

void help_setCallback(void (*a)(void)) {
    g_menuHelp.text = 0;
    g_menuHelp.callback = a;
}

void help_setBounds(RECT *r) {
    if (r == NULL) {
        g_menuHelp.bounds.x = 8;
        g_menuHelp.bounds.y = 8;
        g_menuHelp.bounds.w = 0x131;
        g_menuHelp.bounds.h = 0xD1;
    } else {
        g_menuHelp.bounds = *r;
    }
}

void help_nullHandler(void) {
}

void help_drawCursor(HelpState *unused) {
    ui_drawSprite(g_menuHelp.x, g_menuHelp.y, g_menuHelp.otz, 1, 0);
}

u8 help_isOpen(void) {
    return g_menuHelp.isOpen;
}

u8 help_isLocked(void) {
    return g_menuHelp.locked;
}

void help_setLocked(u8 a) {
    g_menuHelp.locked = a;
}

u8 help_getFollow(void) {
    return g_menuHelp.follow;
}

void help_setFollow(u8 a) {
    g_menuHelp.follow = a;
}

u8 help_getCursorOnTop(void) {
    return g_menuHelp.cursorOnTop;
}

void help_setCursorOnTop(u8 a) {
    g_menuHelp.cursorOnTop = a;
}

void help_toggle(void) {
    if (g_menuHelp.opening == 0 && g_menuHelp.callback != NULL) {
        if (g_menuHelp.isOpen == 0) {
            snd_playSe(0x2AA);
            g_menuHelp.isOpen = 1;
            g_menuHelp.opening = 1;
            g_menuHelp.textShown = 0;
            sprAnim_reset(&g_menuHelp.iconAnim);
            sprAnim_setOnDone(&g_menuHelp.iconAnim, help_onIconAnimDone);
            sprAnim_setPlaying(&g_menuHelp.iconAnim, 1);
        } else {
            snd_playSe(SE_CANCEL);
            g_menuHelp.isOpen = 0;
            g_menuHelp.textShown = 0;
            sprAnim_setPlaying(&g_menuHelp.iconAnim, 0);
        }
    }
}

void func_8003573C(void) {
}

void help_onIconAnimDone(void) {
    g_menuHelp.textShown = 1;
    g_menuHelp.opening = 0;
}

INCLUDE_RODATA("asm/us/main/nonmatchings/help", g_helpIconAnimData);
