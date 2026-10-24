#include "ff9.h"

void help_draw(void) {
    s32 otz;
    Unk80035214 r;

    otz = 1;
    if (g_help.drawHand == 0) {
        otz = g_help.otz;
    }
    do {
    } while (0); /* empty debug macro in the original; changes the register allocation */
    if (g_help.atCursor != 0) {
        if (g_help.drawHand != 0) {
            ui_drawHand(g_help.x, g_help.y, otz);
        }
        func_80035C2C(g_help.x + 5, g_help.y + 2, otz, &g_help.anim);
    }
    func_8003573C();
    if (g_help.textShown != 0) {
        help_layout(&r);
        ui_drawSprite(r.unk0.x + 8, r.unk0.y, otz, 0xB7, 0);
        text_drawBlock(r.unk0.x + 6, r.unk0.y + 8, otz, g_help.text, 8);
        win_drawHelpFrame(&r.unk0, otz, r.unk8, r.unkA);
    }
}

void help_layout(Unk80035214 *w) {
    RECT *r = &w->unk0;
    RECT *c = &g_help.bounds;
    HelpState *m = &g_help;
    TextSize sz;
    s32 x;
    s32 v;
    s32 d1;
    s32 d2;
    s32 t;

    sz = func_8002AF70(m->text);
    w->unk0.w = sz.w + 12;
    w->unk0.h = sz.h + 12;
    if (w->unk0.w >= c->w) {
        w->unk0.w = c->w;
    }
    if (w->unk0.h >= c->h) {
        w->unk0.h = c->h;
    }
    if (m->atCursor == 0) {
        w->unk8 = 4;
        w->unk0.x = m->x;
        w->unk0.y = m->y;
        return;
    }
    v = m->y;
    t = c->y + 16;
    d2 = v - t;
    d1 = c->y + c->h - 10;
    d1 -= v;
    if (d1 < d2) {
        w->unk8 = 2;
        w->unk0.y = m->y - (s16)(w->unk0.h + 16);
    } else {
        w->unk8 = 0;
        w->unk0.y = m->y + 10;
    }
    v = g_help.x;
    t = c->x + 19;
    d2 = v - t;
    d1 = c->x + c->w - 18;
    d1 -= v;
    if (d1 < d2) {
        w->unk8 |= 1;
        x = g_help.x - 19;
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
        w->unkA = 8;
    } else {
        w->unkA = x - r->x;
    }
}

void help_init(void) {
    g_help.isOpen = 0;
    g_help.handler = 0;
    g_help.text = 0;
    g_help.textShown = 0;
    g_help.opening = 0;
    g_help.disabled = 0;
    g_help.atCursor = 1;
    g_help.drawHand = 1;
    help_setBounds(NULL);
    func_80035B24(&g_help.anim, &D_80010E48);
}

void help_update(void) {
    HelpState *p;
    if (g_help.disabled == 0 && func_8002F810() == 0 && (g_padRepeat & 0x8000000)) {
        help_toggle();
    }
    p = &g_help;
    if (p->isOpen != 0 && func_8002F810() == 0 && p->handler != NULL) {
        p->handler();
        if (p->text != NULL && *p->text != 0xFF) {
            help_draw();
        }
    }
    func_80035B90(&g_help.anim);
}

void func_80035564(void) {
}

void help_setHandler(void (*a)(void)) {
    g_help.text = 0;
    g_help.handler = a;
}

void help_setBounds(RECT *r) {
    if (r == NULL) {
        g_help.bounds.x = 8;
        g_help.bounds.y = 8;
        g_help.bounds.w = 0x131;
        g_help.bounds.h = 0xD1;
    } else {
        g_help.bounds = *r;
    }
}

void help_nullHandler(void) {
}

void func_800355E4(HelpState *unused) {
    ui_drawSprite(g_help.x, g_help.y, g_help.otz, 1, 0);
}

u8 help_isOpen(void) {
    return g_help.isOpen;
}

u8 help_isDisabled(void) {
    return g_help.disabled;
}

void help_setDisabled(u8 a) {
    g_help.disabled = a;
}

u8 help_getAtCursor(void) {
    return g_help.atCursor;
}

void help_setAtCursor(u8 a) {
    g_help.atCursor = a;
}

u8 help_getDrawHand(void) {
    return g_help.drawHand;
}

void help_setDrawHand(u8 a) {
    g_help.drawHand = a;
}

void help_toggle(void) {
    if (g_help.opening == 0 && g_help.handler != NULL) {
        if (g_help.isOpen == 0) {
            snd_playSe(0x2AA);
            g_help.isOpen = 1;
            g_help.opening = 1;
            g_help.textShown = 0;
            func_80035B7C(&g_help.anim);
            func_80035B74(&g_help.anim, help_onOpened);
            func_80035B64(&g_help.anim, 1);
        } else {
            snd_playSe(0x65);
            g_help.isOpen = 0;
            g_help.textShown = 0;
            func_80035B64(&g_help.anim, 0);
        }
    }
}

void func_8003573C(void) {
}

void help_onOpened(void) {
    g_help.textShown = 1;
    g_help.opening = 0;
}

INCLUDE_RODATA("asm/us/main/nonmatchings/tu_80035108", D_80010E48);
