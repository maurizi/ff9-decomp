#include "ovl_14.h"

void opening_titleInit(void) {
    g_titleMenu.unk3 = 1;
    opening_titleLoadImages();
}

void opening_titleEnter(void) {
    s32 i;

    func_80032574();
    func_8002C4B0(1);
    g_titleMenu.unk8 = 0;
    g_titleMenu.unk0 = 1;
    g_titleMenu.unk7 = 0;
    g_titleMenu.unk6 = 0;
    g_titleMenu.unk1 = 0;
    g_titleMenu.unk2 = 0;
    g_titleMenu.unkC = 0;
    g_titleMenu.unk10 = 0;
    for (i = 0; i < 2; i++) {
        g_titleMenu.unk4[i] = 0;
    }
    opening_titleOpenCardEvents();
    opening_titleStartFadeIn();
}

void opening_titleRefresh(void) {
    opening_titleStartFadeIn();
}

void opening_titleUpdate(void) {
    func_8002C308(0);
    if (g_titleMenu.unk8 == 0) {
        opening_titleInput();
        opening_titleDraw();
        opening_titleTick();
    } else {
        opening_titleConfirm();
    }
}

void opening_titleLeave(void) {
    func_8002C4B0(0);
    opening_titleCloseCardEvents();
    if (g_titleMenu.unk3 == 1) {
        menu_open(4, 1);
    }
}

void opening_titleInput(void) {
    if (func_8002F810() == 0) {
        if (g_titleMenu.unk1 != 0 && g_titleMenu.unk2 == 0) {
            if (g_padRepeat & PAD_UP) {
                snd_playSe(0x67);
                g_titleMenu.unk3 = g_titleMenu.unk3 != 0 ? g_titleMenu.unk3 - 1 : 1;
            } else if (g_padRepeat & PAD_DOWN) {
                snd_playSe(0x67);
                g_titleMenu.unk3 = g_titleMenu.unk3 == 0 ? g_titleMenu.unk3 + 1 : 0;
            } else if (g_padRepeat & (PAD_START | BTN_CROSS)) {
                opening_titleDecide();
            }
        }
    }
}

void opening_titleDraw(void) {
    func_800A8A8C();
    opening_titleDrawBackground();
    opening_titleDrawChoices();
    opening_titleDrawCursor();
}

void opening_titleTick(void) {
    opening_titlePollCard();
    opening_titleStepFadeIn();
}

void opening_titleLoadImages(void) {
    FileEntry *e;

    e = file_find(1, 0x3C);
    cd_readSync(g_gameState->unk20 + e->unk4, (e->unkC - e->unk4) << 11, (void *)0x801CDF00, e->unk4);
    opening_titleLoadTims((s32 *)0x801CDF00);
}

void opening_titleLoadTims(s32 *pack) {
    RECT *clut;
    RECT *img;
    TmTimLike *tim;
    s32 i;
    s32 n;
    u32 mode;
    s32 sh;
    s32 u;
    s32 tp;
    s32 yb;
    s32 xb;
    s32 y2;
    s32 *p;

    n = pack[0];
    p = pack + 1;
    for (i = 0; i < n; i++) {
        tim = (TmTimLike *)((u8 *)pack + p[i]);
        func_8001D8A8((s32)tim);
        func_8001D910(tim, &clut, &img);
        u = img->x & 0x3F;
        mode = tim->unk4 & 3;
        sh = 2 - mode;
        g_titleMenu.unk24[i].unk0.s.u = u << sh;
        g_titleMenu.unk24[i].unk0.s.v = img->y;
        g_titleMenu.unk24[i].unk4 = img->w << sh;
        g_titleMenu.unk24[i].unk5 = img->h;
        tp = mode << 7;
        yb = (s16)((s16)(img->y & 0x100) >> 4);
        xb = (img->x & 0x3FF) >> 6;
        y2 = (s16)(img->y & 0x200) << 2;
        g_titleMenu.unk24[i].unk6 = ((i == 10) ? (tp | (yb | 0x20)) : (tp | yb)) | xb | y2;
        if (mode < 2) {
            g_titleMenu.unk24[i].unk0.s.clut = (clut->y << 6) | ((clut->x >> 4) & 0x3F);
        } else {
            g_titleMenu.unk24[i].unk0.s.clut = 0;
        }
        DrawSync(0);
    }
}

void opening_titleDecide(void) {
    if (g_titleMenu.unk3 == 1 && opening_titleHasCard() == 0) {
        snd_playSe(0x66);
        return;
    }
    snd_playSe(g_titleMenu.unk3 == 0 ? 0xC18 : 0x67);
    func_8002FAAC(0x10);
    func_8002F8A8(opening_titleOnFadeOut);
}

void func_800A8A8C(void) {
}

void opening_titleDrawBackground(void) {
    s32 i;

    for (i = 0; i < 10; i++) {
        func_80027710(i << 5, 0, 10, &g_titleMenu.unk24[i], 0);
    }
    func_80027710(g_titleImage10Pos.x, g_titleImage10Pos.y, 5, &g_titleMenu.unk24[10], 1);
    if (g_titleMenu.unk1 != 0) {
        if (g_titleMenu.unk2 == 0) {
            func_80027710(g_titleImage13Pos.x, g_titleImage13Pos.y, 5, &g_titleMenu.unk24[13], 0);
        } else {
            opening_titleDrawFading(g_titleImage13Pos.x, g_titleImage13Pos.y, 5, &g_titleMenu.unk24[13], 0);
        }
    }
}

void opening_titleDrawChoices(void) {
    if (g_titleMenu.unk1 != 0) {
        if (g_titleMenu.unk2 == 0) {
            func_80027710(g_titleNewGamePos.x, g_titleNewGamePos.y, 5, &g_titleMenu.unk24[11], 0);
            func_80027710(g_titleContinuePos.x, g_titleContinuePos.y, 5, &g_titleMenu.unk24[12], opening_titleHasCard() == 0);
        } else {
            opening_titleDrawFading(g_titleNewGamePos.x, g_titleNewGamePos.y, 5, &g_titleMenu.unk24[11], 0);
            opening_titleDrawFading(g_titleContinuePos.x, g_titleContinuePos.y, 5, &g_titleMenu.unk24[12], opening_titleHasCard() == 0);
        }
    }
}

void opening_titleDrawCursor(void) {
    if (func_8002F810() == 0 && g_titleMenu.unk1 != 0 && g_titleMenu.unk2 == 0) {
        D_80077DE8.unk0 = g_titleCursorPos[g_titleMenu.unk3].x - 0x1A;
        D_80077DE8.unk2 = g_titleCursorPos[g_titleMenu.unk3].y + 3;
        D_80077DE8.unk4 = 3;
        func_800355E4(&D_80077DE8);
    }
}

void opening_titleStepFadeIn(void) {
    if (++g_titleMenu.unkC > 0x30) {
        g_titleMenu.unkC = 0;
        g_titleMenu.unk2 = 0;
    }
}

void opening_titleOpenCardEvents(void) {
    s32 i;
    s32 h;

    EnterCriticalSection();
    for (i = 0; i < 4; i++) {
        h = OpenEvent(0xF4000001, g_titleCardEvents[i], 0x2000, 0);
        g_titleMenu.unk14[i] = h;
        EnableEvent(h);
    }
    ExitCriticalSection();
}

void opening_titleCloseCardEvents(void) {
    s32 i;

    EnterCriticalSection();
    for (i = 0; i < 4; i++) {
        DisableEvent(g_titleMenu.unk14[i]);
        CloseEvent(g_titleMenu.unk14[i]);
    }
    ExitCriticalSection();
}

void opening_titlePollCard(void) {
    s32 r;

    if (g_titleMenu.unk7 == 0) {
        opening_titleStartCardInfo();
        g_titleMenu.unk7 = 1;
    }
    r = opening_titleTestCardEvents();
    if (r != -1) {
        g_titleMenu.unk4[g_titleMenu.unk6] = (r == 0 || r == 3) ? 1 : 0;
        g_titleMenu.unk7 = 0;
        g_titleMenu.unk6 ^= 1;
    }
}

u8 opening_titleHasCard(void) {
    s32 i;
    u8 r = 0;

    for (i = 0; i < 2; i++) {
        r |= g_titleMenu.unk4[i];
    }
    return r;
}

void opening_titleStartCardInfo(void) {
    s32 i;

    for (i = 0; i < 4; i++) {
        TestEvent(g_titleMenu.unk14[i]);
    }
    _card_wait(0);
    _card_wait(1);
    _card_info(g_titleMenu.unk6 << 4);
}

s32 opening_titleTestCardEvents(void) {
    s32 i;
    s32 r = -1;

    for (i = 0; i < 4; i++) {
        if (TestEvent(g_titleMenu.unk14[i]) == 1) {
            r = i;
            break;
        }
    }
    return r;
}

void opening_titleOnFadeOut(void) {
    g_titleMenu.unk8 = 1;
}

s32 opening_titleGetCursor(void) {
    return g_titleMenu.unk3;
}

void opening_titleStartFadeIn(void) {
    if (scene_getFadeScreen() == 0) {
        g_titleMenu.unk1 = 1;
        g_titleMenu.unk2 = 1;
        g_titleMenu.unkC = 0;
    } else {
        g_titleMenu.unk1 = 1;
        g_titleMenu.unk2 = 0;
        g_titleMenu.unkC = 0;
        func_8002FA50(0x20);
    }
}

void opening_titleDrawFading(s32 x, s32 y, s32 z, Ovl14Text *t, s32 arg4) {
    SprtWords *p;
    s32 c = g_titleMenu.unkC;

    setlen(g_primPtr, 4);
    p = (SprtWords *)g_primPtr;
    p->rgbc = (c << 1) | (c << 9) | (c << 17) | 0x66000000;
    p->xy = x | (y << 16);
    p->uvclut = t->unk0.w;
    p->wh = t->unk4 | (t->unk5 << 16);
    g_primPtr = (u8 *)(p + 1);
    addPrim(&g_ot[z], p);
    SetDrawTPage(g_primPtr, 1, 1, t->unk6 | 0x20);
    addPrim(&g_ot[z], g_primPtr);
    g_primPtr += sizeof(SprtWords); /* sic: the DR_TPAGE is only 8 bytes */
}

void opening_titleConfirm(void) {
    s32 r;

    switch (g_titleMenu.unk3) {
    case 0:
        g_gameState->unkB = 1;
        r = scene_checkDisc();
        if (r != 0xFF) {
            menu_open(8, ((r & 3) << 14) | 0x3A98);
            scene_set(3);
        } else {
            scene_set(4);
        }
        break;
    case 1:
        scene_set(2);
        menu_open(4, 1);
        break;
    }
}
