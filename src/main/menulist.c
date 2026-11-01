#include "ff9.h"

void menuList_update(MenuList *list) {
    s32 fillStep;
    if (list->scroll == 0) {
        return;
    }
    if (list->scroll & MENULIST_SCROLL_PAGE) {
        list->scrollPos += list->pageStep;
        if (list->scrollPos >= list->height - 6) {
            s32 rowItem = (list->pageTop + list->cursor) * list->cols;
            list->scroll = 0;
            list->scrollPos = 0;
            list->top = list->pageTop;
            if (rowItem + list->col >= list->count) {
                list->cursor--;
            }
        }
        return;
    }
    if (list->scroll & MENULIST_SCROLL_UP) {
        fillStep = -3;
    } else {
        fillStep = 3;
    }
    list->scrollPos += 3;
    list->fillV = (list->fillV + fillStep) & 0x1F;
    if (list->scrollPos < 15) {
        return;
    }
    if (list->scroll & MENULIST_SCROLL_WRAP_COL) {
        list->col = (list->scroll & MENULIST_SCROLL_UP) ? list->cols - 1 : 0;
    }
    {
        u8 oldTop = list->top;
        s32 newTop;
        if (list->scroll & MENULIST_SCROLL_UP) {
            newTop = oldTop - 1;
        } else {
            newTop = oldTop + 1;
        }
        list->top = newTop;
    }
    list->scroll = 0;
    list->scrollPos = 0;
}

MenuView *menuList_getView(MenuList *list, MenuView *view) {
    s32 remain;

    if (list->scroll & MENULIST_SCROLL_PAGE) {
        if (list->scroll & MENULIST_SCROLL_UP) {
            remain = list->height - list->scrollPos;
            view->rows = (remain + 8) / 15;
            view->first = list->top + (list->visibleRows - view->rows);
            view->y = (list->visibleRows - view->rows) * 15 + 6;
        } else {
            remain = list->height - list->scrollPos;
            view->rows = (remain + 8) / 15;
            view->first = list->top;
            view->y = 6;
        }
    } else if (list->scroll & MENULIST_SCROLL_UP) {
        view->rows = list->visibleRows + 1;
        view->first = list->top - 1;
        view->y = list->scrollPos - 9;
    } else if (list->scroll & MENULIST_SCROLL_DOWN) {
        view->first = list->top;
        view->rows = list->visibleRows + 1;
        view->y = -list->scrollPos + 6;
    } else {
        view->first = list->top;
        view->rows = list->visibleRows;
        view->y = 6;
    }
    return view;
}

void menuList_init(MenuList *list, s32 count, s32 rows, s32 cols, s32 height) {
    list->count = count;
    if (cols != 0) {
        list->rows = (count + cols - 1) / cols;
    } else {
        list->rows = 0;
    }
    list->cols = cols;
    if (list->rows < rows) {
        rows = list->rows;
    }
    list->visibleRows = rows;
    list->scroll = 0;
    list->pageTop = 0;
    list->fillV = 0;
    list->fillU = 0;
    list->scrollPos = 0;
    list->col = 0;
    list->cursor = 0;
    list->top = 0;
    list->height = height;
}

void menuList_input(MenuList *list, s32 pad) {
    if (list->scroll == 0) {
        menuList_up(list, pad);
        menuList_down(list, pad);
        menuList_pageUp(list, pad);
        menuList_pageDown(list, pad);
    }
}

void menuList_up(MenuList *list, u32 pad) {
    if (pad & PAD_UP) {
        if (list->cursor == 0) {
            if (list->top != 0) {
                snd_playSe(SE_CURSOR);
                list->scroll = MENULIST_SCROLL_UP;
            }
        } else {
            list->cursor--;
            snd_playSe(SE_CURSOR);
        }
    }
}

void menuList_down(MenuList *list, u32 pad) {
    if (pad & PAD_DOWN) {
        if ((list->top + list->cursor) * list->cols + list->col + list->cols < list->count) {
            if (list->cursor >= list->visibleRows - 1) {
                if (list->top < list->rows - list->visibleRows) {
                    list->scroll = MENULIST_SCROLL_DOWN;
                    snd_playSe(SE_CURSOR);
                }
            } else {
                snd_playSe(SE_CURSOR);
                list->cursor++;
            }
        } else if (list->cursor >= list->visibleRows - 1 && list->top < list->rows - list->visibleRows) {
            list->scroll = MENULIST_SCROLL_DOWN | MENULIST_SCROLL_WRAP_COL;
            snd_playSe(SE_CURSOR);
        }
    }
}

void menuList_left(MenuList *list, s32 pressed) {
    pressed &= PAD_LEFT;
    if (pressed != 0) {
        if (list->col == 0) {
            if (list->cursor != 0) {
                list->col = list->cols - 1;
                list->cursor = list->cursor - 1;
                snd_playSe(SE_CURSOR);
                return;
            }
            if (list->top != 0) {
                snd_playSe(SE_CURSOR);
                list->scroll = MENULIST_SCROLL_UP | MENULIST_SCROLL_WRAP_COL;
            }
        } else {
            snd_playSe(SE_CURSOR);
            list->col--;
        }
    }
}

void menuList_right(MenuList *list, s32 pad) {
    if (pad & PAD_RIGHT) {
        if ((list->top + list->cursor) * list->cols + list->col < list->count - 1) {
            if (list->col >= list->cols - 1) {
                if (list->cursor < list->visibleRows - 1) {
                    snd_playSe(SE_CURSOR);
                    list->col = 0;
                    list->cursor++;
                } else if (list->top < list->rows - list->visibleRows) {
                    snd_playSe(SE_CURSOR);
                    list->scroll = MENULIST_SCROLL_DOWN | MENULIST_SCROLL_WRAP_COL;
                }
            } else {
                snd_playSe(SE_CURSOR);
                list->col++;
            }
        }
    }
}

void menuList_pageUp(MenuList *list, s32 pressed) {
    s32 dist;
    s32 newTop;

    if (!(g_padPortHeld[0] & BTN_R1) && (pressed & BTN_L1) && list->top != 0) {
        dist = list->height - 6;
        list->scrollPos = dist >> 2;
        dist -= list->scrollPos;
        list->scroll = MENULIST_SCROLL_DOWN | MENULIST_SCROLL_UP | MENULIST_SCROLL_PAGE;
        newTop = list->top - list->visibleRows;
        list->pageStep = (dist + 14) / 15;
        if (newTop < 0) {
            newTop = 0;
        }
        list->pageTop = newTop;
        snd_playSe(SE_PAGE);
    }
}

void menuList_pageDown(MenuList *list, s32 pressed) {
    s32 dist;
    s32 maxTop;
    s32 newTop;
    s32 u3;

    if ((pressed & BTN_R1) && list->top < list->rows - list->visibleRows) {
        dist = list->height - 6;
        list->scrollPos = dist >> 2;
        dist -= list->scrollPos;
        u3 = list->visibleRows;
        maxTop = list->rows - u3;
        list->scroll = MENULIST_SCROLL_DOWN | MENULIST_SCROLL_PAGE;
        newTop = list->top + u3;
        list->pageStep = (dist + 14) / 15;
        if (newTop < maxTop) {
            maxTop = newTop;
        }
        list->pageTop = maxTop;
        snd_playSe(SE_PAGE);
    }
}

MenuView *menuList_getPageView(MenuList *list, MenuView *view) {
    s32 rowCount;
    if (list->scroll & MENULIST_SCROLL_UP) {
        rowCount = (list->scrollPos + 12) / 15;
        view->rows = rowCount;
        view->first = list->pageTop + (list->visibleRows - view->rows);
        view->y = list->scrollPos - view->rows * 15 + 7;
    } else {
        s32 scrolled = list->scrollPos;
        view->y = list->height - scrolled;
        view->rows = (scrolled + 14) / 15;
        view->first = list->pageTop;
    }
    return view;
}

void menuList_getRowsRect(MenuList *list, RECT *src, RECT *dst) {
    dst->x = src->x;
    dst->w = src->w;
    if (list->scroll & MENULIST_SCROLL_UP) {
        dst->y = list->scrollPos + src->y + 5;
        dst->h = src->h - (list->scrollPos + 4) - 1;
    } else {
        dst->y = src->y;
        dst->h = src->h - list->scrollPos - 1;
    }
}

void snd_playSe(s32 se) {
    g_gameState->gameData->soundHook(SND_SE_PLAY, se, SND_SE_GROUP_MENU, SND_PAN_CENTER, SND_VOL_MAX);
}

void snd_stopPlayedSe(s32 se) {
    g_gameState->gameData->soundHook(SND_SE_STOP, se, SND_SE_GROUP_MENU, 0, 0);
}

void snd_startSe(s32 se) {
    g_gameState->gameData->soundHook(SND_SE_PLAY, se, 0, SND_PAN_CENTER, SND_VOL_MAX);
}

void snd_stopSe(s32 se) {
    g_gameState->gameData->soundHook(SND_SE_STOP, se, 0, 0, 0);
}

s32 fade_reverse(void (*fn)(void)) {
    FadeState *fade = &g_fade;
    s32 remain = fade->frames - fade->frame;
    Unk4 c1;
    Unk4 c2;

    if (fade->active == 0) {
        return 0;
    }
    c1 = fade->startColor;
    c2 = *(Unk4 *)&fade->endR;
    fade_setAngle(fade->angle ^ 0x800);
    fade_start(g_fade.type, fade->subtract, 0, c2.b[0] | (c2.b[1] << 8) | (c2.b[2] << 16));
    fade_start(g_fade.type, fade->subtract, fade->frames, c1.b[0] | (c1.b[1] << 8) | (c1.b[2] << 16));
    g_fadeSeekFuncs[g_fade.type](fade, remain);
    fade->onDone = (s32)fn;
    fade->frame += remain;
    return 1;
}

void fade_update(void) {
    u8 *pkt = g_fadePktBufs[0];
    FadeState *fade = &g_fade;
    FadeFn fn;

    if (g_gameState->gameData->drawBuf != 0) {
        pkt = g_fadePktBufs[1];
    }
    fade->pkt = pkt;
    if (fade->active != 0) {
        fn = (FadeFn)g_fadeTypes[fade->type].update;
        if (fn != NULL) {
            fn(fade);
        }
    }
    if (*(u32 *)&g_fade & 0xFFFF0000) {
        fn = (FadeFn)g_fadeTypes[g_fade.type].draw;
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
