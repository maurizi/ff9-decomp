#include "ff9.h"

void help_draw(void) {
    s32 otz;
    HelpLayout layout;

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
    help_onDraw();
    if (g_menuHelp.textShown != 0) {
        help_layout(&layout);
        ui_drawSprite(layout.rect.x + 8, layout.rect.y, otz, UI_SPRITE_HELP_TAG, 0);
        text_drawBlock(layout.rect.x + 6, layout.rect.y + 8, otz, g_menuHelp.text, TEXT_PAL_HELP);
        win_drawHelpFrame(&layout.rect, otz, layout.style, layout.tailX);
    }
}

void help_layout(HelpLayout *w) {
    RECT *box = &w->rect;
    RECT *bounds = &g_menuHelp.bounds;
    HelpState *help = &g_menuHelp;
    TextSize sz;
    s32 x;
    s32 cursorPos;
    s32 d1;
    s32 d2;
    s32 edge;

    sz = text_measure(help->text);
    w->rect.w = sz.w + 12;
    w->rect.h = sz.h + 12;
    if (w->rect.w >= bounds->w) {
        w->rect.w = bounds->w;
    }
    if (w->rect.h >= bounds->h) {
        w->rect.h = bounds->h;
    }
    if (help->follow == 0) {
        w->style = HELP_TAIL_NONE;
        w->rect.x = help->x;
        w->rect.y = help->y;
        return;
    }
    cursorPos = help->y;
    edge = bounds->y + 16;
    d2 = cursorPos - edge;
    d1 = bounds->y + bounds->h - 10;
    d1 -= cursorPos;
    if (d1 < d2) {
        w->style = HELP_TAIL_DOWN;
        w->rect.y = help->y - (s16)(w->rect.h + 16);
    } else {
        w->style = HELP_TAIL_UP;
        w->rect.y = help->y + 10;
    }
    cursorPos = g_menuHelp.x;
    edge = bounds->x + 19;
    d2 = cursorPos - edge;
    d1 = bounds->x + bounds->w - 18;
    d1 -= cursorPos;
    if (d1 < d2) {
        w->style |= HELP_TAIL_RIGHT;
        x = g_menuHelp.x - 19;
    } else {
        x = cursorPos + 18;
    }
    box->x = x - (box->w >> 1);
    if (box->x < bounds->x) {
        box->x = bounds->x;
    }
    if (box->x + box->w > bounds->x + bounds->w) {
        box->x = bounds->x + bounds->w - box->w;
    }
    if (x - box->x < 8) {
        w->tailX = 8;
    } else {
        w->tailX = x - box->x;
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
    HelpState *help;
    if (g_menuHelp.locked == 0 && fade_isBusy() == 0 && (g_padRepeat & BTN_SELECT)) {
        help_toggle();
    }
    help = &g_menuHelp;
    if (help->isOpen != 0 && fade_isBusy() == 0 && help->callback != NULL) {
        help->callback();
        if (help->text != NULL && *help->text != TEXT_END) {
            help_draw();
        }
    }
    sprAnim_update(&g_menuHelp.iconAnim);
}

void help_nop(void) {
}

void help_setCallback(void (*fn)(void)) {
    g_menuHelp.text = 0;
    g_menuHelp.callback = fn;
}

void help_setBounds(RECT *bounds) {
    if (bounds == NULL) {
        g_menuHelp.bounds.x = 8;
        g_menuHelp.bounds.y = 8;
        g_menuHelp.bounds.w = 0x131;
        g_menuHelp.bounds.h = 0xD1;
    } else {
        g_menuHelp.bounds = *bounds;
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

void help_setLocked(u8 locked) {
    g_menuHelp.locked = locked;
}

u8 help_getFollow(void) {
    return g_menuHelp.follow;
}

void help_setFollow(u8 follow) {
    g_menuHelp.follow = follow;
}

u8 help_getCursorOnTop(void) {
    return g_menuHelp.cursorOnTop;
}

void help_setCursorOnTop(u8 onTop) {
    g_menuHelp.cursorOnTop = onTop;
}

void help_toggle(void) {
    if (g_menuHelp.opening == 0 && g_menuHelp.callback != NULL) {
        if (g_menuHelp.isOpen == 0) {
            snd_playSe(SE_HELP_OPEN);
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

void help_onDraw(void) {
}

void help_onIconAnimDone(void) {
    g_menuHelp.textShown = 1;
    g_menuHelp.opening = 0;
}

INCLUDE_RODATA("asm/us/main/nonmatchings/help", g_helpIconAnimData);
