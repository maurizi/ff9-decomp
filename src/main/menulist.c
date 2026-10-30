#include "ff9.h"

void menuList_update(MenuList *p) {
    s32 d;
    if (p->scroll == 0) {
        return;
    }
    if (p->scroll & 4) {
        p->scrollPos += p->pageStep;
        if (p->scrollPos >= p->height - 6) {
            s32 t = (p->pageTop + p->cursor) * p->cols;
            p->scroll = 0;
            p->scrollPos = 0;
            p->top = p->pageTop;
            if (t + p->col >= p->count) {
                p->cursor--;
            }
        }
        return;
    }
    if (p->scroll & 2) {
        d = -3;
    } else {
        d = 3;
    }
    p->scrollPos += 3;
    p->fillV = (p->fillV + d) & 0x1F;
    if (p->scrollPos < 15) {
        return;
    }
    if (p->scroll & 8) {
        p->col = (p->scroll & 2) ? p->cols - 1 : 0;
    }
    {
        u8 c = p->top;
        s32 e;
        if (p->scroll & 2) {
            e = c - 1;
        } else {
            e = c + 1;
        }
        p->top = e;
    }
    p->scroll = 0;
    p->scrollPos = 0;
}

MenuView *menuList_getView(MenuList *m, MenuView *v) {
    s32 d;

    if (m->scroll & 4) {
        if (m->scroll & 2) {
            d = m->height - m->scrollPos;
            v->unk0 = (d + 8) / 15;
            v->unk1 = m->top + (m->visibleRows - v->unk0);
            v->unk2 = (m->visibleRows - v->unk0) * 15 + 6;
        } else {
            d = m->height - m->scrollPos;
            v->unk0 = (d + 8) / 15;
            v->unk1 = m->top;
            v->unk2 = 6;
        }
    } else if (m->scroll & 2) {
        v->unk0 = m->visibleRows + 1;
        v->unk1 = m->top - 1;
        v->unk2 = m->scrollPos - 9;
    } else if (m->scroll & 1) {
        v->unk1 = m->top;
        v->unk0 = m->visibleRows + 1;
        v->unk2 = -m->scrollPos + 6;
    } else {
        v->unk1 = m->top;
        v->unk0 = m->visibleRows;
        v->unk2 = 6;
    }
    return v;
}

void menuList_init(MenuList *m, s32 count, s32 rows, s32 cols, s32 e) {
    m->count = count;
    if (cols != 0) {
        m->rows = (count + cols - 1) / cols;
    } else {
        m->rows = 0;
    }
    m->cols = cols;
    if (m->rows < rows) {
        rows = m->rows;
    }
    m->visibleRows = rows;
    m->scroll = 0;
    m->pageTop = 0;
    m->fillV = 0;
    m->unkA = 0;
    m->scrollPos = 0;
    m->col = 0;
    m->cursor = 0;
    m->top = 0;
    m->height = e;
}

void menuList_input(MenuList *m, s32 b) {
    if (m->scroll == 0) {
        menuList_up(m, b);
        menuList_down(m, b);
        menuList_pageUp(m, b);
        menuList_pageDown(m, b);
    }
}

void menuList_up(MenuList *m, u32 pad) {
    if (pad & PAD_UP) {
        if (m->cursor == 0) {
            if (m->top != 0) {
                snd_playSe(SE_CURSOR);
                m->scroll = 2;
            }
        } else {
            m->cursor--;
            snd_playSe(SE_CURSOR);
        }
    }
}

void menuList_down(MenuList *m, u32 pad) {
    if (pad & PAD_DOWN) {
        if ((m->top + m->cursor) * m->cols + m->col + m->cols < m->count) {
            if (m->cursor >= m->visibleRows - 1) {
                if (m->top < m->rows - m->visibleRows) {
                    m->scroll = 1;
                    snd_playSe(SE_CURSOR);
                }
            } else {
                snd_playSe(SE_CURSOR);
                m->cursor++;
            }
        } else if (m->cursor >= m->visibleRows - 1 && m->top < m->rows - m->visibleRows) {
            m->scroll = 9;
            snd_playSe(SE_CURSOR);
        }
    }
}

void menuList_left(MenuList *arg0, s32 arg1) {
    arg1 &= PAD_LEFT;
    if (arg1 != 0) {
        if (arg0->col == 0) {
            if (arg0->cursor != 0) {
                arg0->col = arg0->cols - 1;
                arg0->cursor = arg0->cursor - 1;
                snd_playSe(SE_CURSOR);
                return;
            }
            if (arg0->top != 0) {
                snd_playSe(SE_CURSOR);
                arg0->scroll = 0xA;
            }
        } else {
            snd_playSe(SE_CURSOR);
            arg0->col--;
        }
    }
}

void menuList_right(MenuList *m, s32 pad) {
    if (pad & PAD_RIGHT) {
        if ((m->top + m->cursor) * m->cols + m->col < m->count - 1) {
            if (m->col >= m->cols - 1) {
                if (m->cursor < m->visibleRows - 1) {
                    snd_playSe(SE_CURSOR);
                    m->col = 0;
                    m->cursor++;
                } else if (m->top < m->rows - m->visibleRows) {
                    snd_playSe(SE_CURSOR);
                    m->scroll = 9;
                }
            } else {
                snd_playSe(SE_CURSOR);
                m->col++;
            }
        }
    }
}

void menuList_pageUp(MenuList *arg0, s32 arg1) {
    s32 t;
    s32 c;

    if (!(g_padPortHeld[0] & BTN_R1) && (arg1 & BTN_L1) && arg0->top != 0) {
        t = arg0->height - 6;
        arg0->scrollPos = t >> 2;
        t -= arg0->scrollPos;
        arg0->scroll = 7;
        c = arg0->top - arg0->visibleRows;
        arg0->pageStep = (t + 14) / 15;
        if (c < 0) {
            c = 0;
        }
        arg0->pageTop = c;
        snd_playSe(SE_PAGE);
    }
}

void menuList_pageDown(MenuList *arg0, s32 arg1) {
    s32 t;
    s32 a;
    s32 c;
    s32 u3;

    if ((arg1 & BTN_R1) && arg0->top < arg0->rows - arg0->visibleRows) {
        t = arg0->height - 6;
        arg0->scrollPos = t >> 2;
        t -= arg0->scrollPos;
        u3 = arg0->visibleRows;
        a = arg0->rows - u3;
        arg0->scroll = 5;
        c = arg0->top + u3;
        arg0->pageStep = (t + 14) / 15;
        if (c < a) {
            a = c;
        }
        arg0->pageTop = a;
        snd_playSe(SE_PAGE);
    }
}

MenuView *menuList_getPageView(MenuList *m, MenuView *v) {
    s32 q;
    if (m->scroll & 2) {
        q = (m->scrollPos + 12) / 15;
        v->unk0 = q;
        v->unk1 = m->pageTop + (m->visibleRows - v->unk0);
        v->unk2 = m->scrollPos - v->unk0 * 15 + 7;
    } else {
        s32 n = m->scrollPos;
        v->unk2 = m->height - n;
        v->unk0 = (n + 14) / 15;
        v->unk1 = m->pageTop;
    }
    return v;
}

void menuList_getRowsRect(MenuList *m, RECT *src, RECT *dst) {
    dst->x = src->x;
    dst->w = src->w;
    if (m->scroll & 2) {
        dst->y = m->scrollPos + src->y + 5;
        dst->h = src->h - (m->scrollPos + 4) - 1;
    } else {
        dst->y = src->y;
        dst->h = src->h - m->scrollPos - 1;
    }
}

void snd_playSe(s32 a) {
    g_gameState->gameData->soundHook(SND_SE_PLAY, a, 0x800000, 0x80, 0x7F);
}

void func_8002F4D0(s32 a) {
    g_gameState->gameData->soundHook(SND_SE_STOP, a, 0x800000, 0, 0);
}

void snd_startSe(s32 a) {
    g_gameState->gameData->soundHook(SND_SE_PLAY, a, 0, 0x80, 0x7F);
}

void snd_stopSe(s32 a) {
    g_gameState->gameData->soundHook(SND_SE_STOP, a, 0, 0, 0);
}

s32 fade_reverse(void (*fn)(void)) {
    FadeState *p = &g_fade;
    s32 d = p->frames - p->frame;
    Unk4 c1;
    Unk4 c2;

    if (p->active == 0) {
        return 0;
    }
    c1 = p->startColor;
    c2 = *(Unk4 *)&p->endR;
    fade_setAngle(p->angle ^ 0x800);
    fade_start(g_fade.type, p->subtract, 0, c2.unk0[0] | (c2.unk0[1] << 8) | (c2.unk0[2] << 16));
    fade_start(g_fade.type, p->subtract, p->frames, c1.unk0[0] | (c1.unk0[1] << 8) | (c1.unk0[2] << 16));
    g_fadeSeekFuncs[g_fade.type](p, d);
    p->onDone = (s32)fn;
    p->frame += d;
    return 1;
}

void fade_update(void) {
    u8 *buf = D_80077878[0];
    FadeState *f = &g_fade;
    FadeFn fn;

    if (g_gameState->gameData->unk8 != 0) {
        buf = D_80077878[1];
    }
    f->pkt = buf;
    if (f->active != 0) {
        fn = (FadeFn)g_fadeTypes[f->type].unk4;
        if (fn != NULL) {
            fn(f);
        }
    }
    if (*(u32 *)&g_fade & 0xFFFF0000) {
        fn = (FadeFn)g_fadeTypes[g_fade.type].unk8;
        if (fn != NULL) {
            fn(&g_fade);
        }
    }
    if (g_fade.active != 0) {
        if (++g_fade.frame >= (u32)g_fade.frames) {
            g_fade.active = 0;
            fn = (FadeFn)g_fade.onDone;
            if (fn != NULL) {
                fn(&g_fade);
            }
        }
    }
}
