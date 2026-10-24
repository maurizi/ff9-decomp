#include "ff9.h"

void func_8002EB74(MenuList *p) {
    s32 d;
    if (p->unk8 == 0) {
        return;
    }
    if (p->unk8 & 4) {
        p->unk9 += p->unkE;
        if (p->unk9 >= p->unkD - 6) {
            s32 t = (p->unkC + p->cursor) * p->unk4;
            p->unk8 = 0;
            p->unk9 = 0;
            p->unk5 = p->unkC;
            if (t + p->unk7 >= p->unk0) {
                p->cursor--;
            }
        }
        return;
    }
    if (p->unk8 & 2) {
        d = -3;
    } else {
        d = 3;
    }
    p->unk9 += 3;
    p->unkB = (p->unkB + d) & 0x1F;
    if (p->unk9 < 15) {
        return;
    }
    if (p->unk8 & 8) {
        p->unk7 = (p->unk8 & 2) ? p->unk4 - 1 : 0;
    }
    {
        u8 c = p->unk5;
        s32 e;
        if (p->unk8 & 2) {
            e = c - 1;
        } else {
            e = c + 1;
        }
        p->unk5 = e;
    }
    p->unk8 = 0;
    p->unk9 = 0;
}

MenuView *menuList_getView(MenuList *m, MenuView *v) {
    s32 d;

    if (m->unk8 & 4) {
        if (m->unk8 & 2) {
            d = m->unkD - m->unk9;
            v->unk0 = (d + 8) / 15;
            v->unk1 = m->unk5 + (m->unk3 - v->unk0);
            v->unk2 = (m->unk3 - v->unk0) * 15 + 6;
        } else {
            d = m->unkD - m->unk9;
            v->unk0 = (d + 8) / 15;
            v->unk1 = m->unk5;
            v->unk2 = 6;
        }
    } else if (m->unk8 & 2) {
        v->unk0 = m->unk3 + 1;
        v->unk1 = m->unk5 - 1;
        v->unk2 = m->unk9 - 9;
    } else if (m->unk8 & 1) {
        v->unk1 = m->unk5;
        v->unk0 = m->unk3 + 1;
        v->unk2 = -m->unk9 + 6;
    } else {
        v->unk1 = m->unk5;
        v->unk0 = m->unk3;
        v->unk2 = 6;
    }
    return v;
}

void func_8002EDFC(MenuList *m, s32 count, s32 rows, s32 cols, s32 e) {
    m->unk0 = count;
    if (cols != 0) {
        m->unk2 = (count + cols - 1) / cols;
    } else {
        m->unk2 = 0;
    }
    m->unk4 = cols;
    if (m->unk2 < rows) {
        rows = m->unk2;
    }
    m->unk3 = rows;
    m->unk8 = 0;
    m->unkC = 0;
    m->unkB = 0;
    m->unkA = 0;
    m->unk9 = 0;
    m->unk7 = 0;
    m->cursor = 0;
    m->unk5 = 0;
    m->unkD = e;
}

void menuList_input(MenuList *m, s32 b) {
    if (m->unk8 == 0) {
        menuList_up(m, b);
        menuList_down(m, b);
        menuList_pageUp(m, b);
        menuList_pageDown(m, b);
    }
}

void menuList_up(MenuList *m, u32 pad) {
    if (pad & 0x10) {
        if (m->cursor == 0) {
            if (m->unk5 != 0) {
                snd_playSe(0x67);
                m->unk8 = 2;
            }
        } else {
            m->cursor--;
            snd_playSe(0x67);
        }
    }
}

void menuList_down(MenuList *m, u32 pad) {
    if (pad & 0x40) {
        if ((m->unk5 + m->cursor) * m->unk4 + m->unk7 + m->unk4 < m->unk0) {
            if (m->cursor >= m->unk3 - 1) {
                if (m->unk5 < m->unk2 - m->unk3) {
                    m->unk8 = 1;
                    snd_playSe(0x67);
                }
            } else {
                snd_playSe(0x67);
                m->cursor++;
            }
        } else if (m->cursor >= m->unk3 - 1 && m->unk5 < m->unk2 - m->unk3) {
            m->unk8 = 9;
            snd_playSe(0x67);
        }
    }
}

void func_8002F044(MenuList *arg0, s32 arg1) {
    arg1 &= 0x80;
    if (arg1 != 0) {
        if (arg0->unk7 == 0) {
            if (arg0->cursor != 0) {
                arg0->unk7 = arg0->unk4 - 1;
                arg0->cursor = arg0->cursor - 1;
                snd_playSe(0x67);
                return;
            }
            if (arg0->unk5 != 0) {
                snd_playSe(0x67);
                arg0->unk8 = 0xA;
            }
        } else {
            snd_playSe(0x67);
            arg0->unk7--;
        }
    }
}

void func_8002F0EC(MenuList *m, s32 pad) {
    if (pad & 0x20) {
        if ((m->unk5 + m->cursor) * m->unk4 + m->unk7 < m->unk0 - 1) {
            if (m->unk7 >= m->unk4 - 1) {
                if (m->cursor < m->unk3 - 1) {
                    snd_playSe(0x67);
                    m->unk7 = 0;
                    m->cursor++;
                } else if (m->unk5 < m->unk2 - m->unk3) {
                    snd_playSe(0x67);
                    m->unk8 = 9;
                }
            } else {
                snd_playSe(0x67);
                m->unk7++;
            }
        }
    }
}

void menuList_pageUp(MenuList *arg0, s32 arg1) {
    s32 t;
    s32 c;

    if (!(g_padPortHeld[0] & BTN_R1) && (arg1 & 0x100000) && arg0->unk5 != 0) {
        t = arg0->unkD - 6;
        arg0->unk9 = t >> 2;
        t -= arg0->unk9;
        arg0->unk8 = 7;
        c = arg0->unk5 - arg0->unk3;
        arg0->unkE = (t + 14) / 15;
        if (c < 0) {
            c = 0;
        }
        arg0->unkC = c;
        snd_playSe(0x417);
    }
}

void menuList_pageDown(MenuList *arg0, s32 arg1) {
    s32 t;
    s32 a;
    s32 c;
    s32 u3;

    if ((arg1 & 0x200000) && arg0->unk5 < arg0->unk2 - arg0->unk3) {
        t = arg0->unkD - 6;
        arg0->unk9 = t >> 2;
        t -= arg0->unk9;
        u3 = arg0->unk3;
        a = arg0->unk2 - u3;
        arg0->unk8 = 5;
        c = arg0->unk5 + u3;
        arg0->unkE = (t + 14) / 15;
        if (c < a) {
            a = c;
        }
        arg0->unkC = a;
        snd_playSe(0x417);
    }
}

MenuView *func_8002F344(MenuList *m, MenuView *v) {
    s32 q;
    if (m->unk8 & 2) {
        q = (m->unk9 + 12) / 15;
        v->unk0 = q;
        v->unk1 = m->unkC + (m->unk3 - v->unk0);
        v->unk2 = m->unk9 - v->unk0 * 15 + 7;
    } else {
        s32 n = m->unk9;
        v->unk2 = m->unkD - n;
        v->unk0 = (n + 14) / 15;
        v->unk1 = m->unkC;
    }
    return v;
}

void func_8002F40C(MenuList *m, RECT *src, RECT *dst) {
    dst->x = src->x;
    dst->w = src->w;
    if (m->unk8 & 2) {
        dst->y = m->unk9 + src->y + 5;
        dst->h = src->h - (m->unk9 + 4) - 1;
    } else {
        dst->y = src->y;
        dst->h = src->h - m->unk9 - 1;
    }
}

void snd_playSe(s32 a) {
    g_gameState->unk1C->unk83C(0xD000, a, 0x800000, 0x80, 0x7F);
}

void func_8002F4D0(s32 a) {
    g_gameState->unk1C->unk83C(0x5100, a, 0x800000, 0, 0);
}

void func_8002F514(s32 a) {
    g_gameState->unk1C->unk83C(0xD000, a, 0, 0x80, 0x7F);
}

void func_8002F55C(s32 a) {
    g_gameState->unk1C->unk83C(0x5100, a, 0, 0, 0);
}

s32 func_8002F5A0(void (*fn)(void)) {
    FadeState *p = &g_fade;
    s32 d = p->frames - p->frame;
    Unk4 c1;
    Unk4 c2;

    if (p->active == 0) {
        return 0;
    }
    c1 = p->startColor;
    c2 = *(Unk4 *)&p->endR;
    func_8002F93C(p->unkA ^ 0x800);
    fade_start(g_fade.type, p->subtract, 0, c2.unk0[0] | (c2.unk0[1] << 8) | (c2.unk0[2] << 16));
    fade_start(g_fade.type, p->subtract, p->frames, c1.unk0[0] | (c1.unk0[1] << 8) | (c1.unk0[2] << 16));
    D_8006BD28[g_fade.type](p, d);
    p->onDone = (s32)fn;
    p->frame += d;
    return 1;
}

void fade_update(void) {
    u8 *buf = D_80077878[0];
    FadeState *f = &g_fade;
    FadeFn fn;

    if (g_gameState->unk1C->unk8 != 0) {
        buf = D_80077878[1];
    }
    f->pkt = buf;
    if (f->active != 0) {
        fn = (FadeFn)D_8006BD04[f->type].unk4;
        if (fn != NULL) {
            fn(f);
        }
    }
    if (*(u32 *)&g_fade & 0xFFFF0000) {
        fn = (FadeFn)D_8006BD04[g_fade.type].unk8;
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
