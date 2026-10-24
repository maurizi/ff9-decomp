#include "ovl_14.h"

void func_800A8574(void) {
    D_800AB320.unk3 = 1;
    func_800A880C();
}

void func_800A859C(void) {
    s32 i;

    func_80032574();
    pad_setStickAsDpad(1);
    D_800AB320.unk8 = 0;
    D_800AB320.unk0 = 1;
    D_800AB320.unk7 = 0;
    D_800AB320.unk6 = 0;
    D_800AB320.unk1 = 0;
    D_800AB320.unk2 = 0;
    D_800AB320.unkC = 0;
    D_800AB320.unk10 = 0;
    for (i = 0; i < 2; i++) {
        D_800AB320.unk4[i] = 0;
    }
    func_800A8D2C();
    func_800A8FDC();
}

void func_800A8618(void) {
    func_800A8FDC();
}

void func_800A8638(void) {
    pad_updateRepeat(0);
    if (D_800AB320.unk8 == 0) {
        func_800A86D8();
        func_800A87AC();
        func_800A87E4();
    } else {
        func_800A9180();
    }
}

void func_800A8694(void) {
    pad_setStickAsDpad(0);
    func_800A8DB0();
    if (D_800AB320.unk3 == 1) {
        menu_open(4, 1);
    }
}

void func_800A86D8(void) {
    if (fade_isBusy() == 0) {
        if (D_800AB320.unk1 != 0 && D_800AB320.unk2 == 0) {
            if (g_padRepeat & PAD_UP) {
                snd_playSe(0x67);
                D_800AB320.unk3 = D_800AB320.unk3 != 0 ? D_800AB320.unk3 - 1 : 1;
            } else if (g_padRepeat & PAD_DOWN) {
                snd_playSe(0x67);
                D_800AB320.unk3 = D_800AB320.unk3 == 0 ? D_800AB320.unk3 + 1 : 0;
            } else if (g_padRepeat & (PAD_START | BTN_CROSS)) {
                func_800A8A0C();
            }
        }
    }
}

void func_800A87AC(void) {
    func_800A8A8C();
    func_800A8A94();
    func_800A8B7C();
    func_800A8C60();
}

void func_800A87E4(void) {
    func_800A8E10();
    func_800A8CF8();
}

void func_800A880C(void) {
    FileEntry *e;

    e = file_find(1, 0x3C);
    cd_readSync(g_gameState->unk20 + e->unk4, (e->unkC - e->unk4) << 11, (void *)0x801CDF00, e->unk4);
    func_800A8868((s32 *)0x801CDF00);
}

void func_800A8868(s32 *pack) {
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
        file_loadTim((s32)tim);
        file_getTimRects(tim, &clut, &img);
        u = img->x & 0x3F;
        mode = tim->unk4 & 3;
        sh = 2 - mode;
        D_800AB320.unk24[i].unk0.s.u = u << sh;
        D_800AB320.unk24[i].unk0.s.v = img->y;
        D_800AB320.unk24[i].unk4 = img->w << sh;
        D_800AB320.unk24[i].unk5 = img->h;
        tp = mode << 7;
        yb = (s16)((s16)(img->y & 0x100) >> 4);
        xb = (img->x & 0x3FF) >> 6;
        y2 = (s16)(img->y & 0x200) << 2;
        D_800AB320.unk24[i].unk6 = ((i == 10) ? (tp | (yb | 0x20)) : (tp | yb)) | xb | y2;
        if (mode < 2) {
            D_800AB320.unk24[i].unk0.s.clut = (clut->y << 6) | ((clut->x >> 4) & 0x3F);
        } else {
            D_800AB320.unk24[i].unk0.s.clut = 0;
        }
        DrawSync(0);
    }
}

void func_800A8A0C(void) {
    if (D_800AB320.unk3 == 1 && func_800A8EA4() == 0) {
        snd_playSe(0x66);
        return;
    }
    snd_playSe(D_800AB320.unk3 == 0 ? 0xC18 : 0x67);
    fade_out(0x10);
    fade_setOnDone(func_800A8FBC);
}

void func_800A8A8C(void) {
}

void func_800A8A94(void) {
    s32 i;

    for (i = 0; i < 10; i++) {
        ui_drawSpriteDef(i << 5, 0, 10, &D_800AB320.unk24[i], 0);
    }
    ui_drawSpriteDef(D_800AB298.x, D_800AB298.y, 5, &D_800AB320.unk24[10], 1);
    if (D_800AB320.unk1 != 0) {
        if (D_800AB320.unk2 == 0) {
            ui_drawSpriteDef(D_800AB29C.x, D_800AB29C.y, 5, &D_800AB320.unk24[13], 0);
        } else {
            func_800A903C(D_800AB29C.x, D_800AB29C.y, 5, &D_800AB320.unk24[13], 0);
        }
    }
}

void func_800A8B7C(void) {
    if (D_800AB320.unk1 != 0) {
        if (D_800AB320.unk2 == 0) {
            ui_drawSpriteDef(D_800AB2A0.x, D_800AB2A0.y, 5, &D_800AB320.unk24[11], 0);
            ui_drawSpriteDef(D_800AB2A4.x, D_800AB2A4.y, 5, &D_800AB320.unk24[12], func_800A8EA4() == 0);
        } else {
            func_800A903C(D_800AB2A0.x, D_800AB2A0.y, 5, &D_800AB320.unk24[11], 0);
            func_800A903C(D_800AB2A4.x, D_800AB2A4.y, 5, &D_800AB320.unk24[12], func_800A8EA4() == 0);
        }
    }
}

void func_800A8C60(void) {
    if (fade_isBusy() == 0 && D_800AB320.unk1 != 0 && D_800AB320.unk2 == 0) {
        g_menuHelp.x = D_800AB2A8[D_800AB320.unk3].x - 0x1A;
        g_menuHelp.y = D_800AB2A8[D_800AB320.unk3].y + 3;
        g_menuHelp.otz = 3;
        help_drawCursor(&g_menuHelp);
    }
}

void func_800A8CF8(void) {
    if (++D_800AB320.unkC > 0x30) {
        D_800AB320.unkC = 0;
        D_800AB320.unk2 = 0;
    }
}

void func_800A8D2C(void) {
    s32 i;
    s32 h;

    EnterCriticalSection();
    for (i = 0; i < 4; i++) {
        h = OpenEvent(0xF4000001, D_800AB2B4[i], 0x2000, 0);
        D_800AB320.unk14[i] = h;
        EnableEvent(h);
    }
    ExitCriticalSection();
}

void func_800A8DB0(void) {
    s32 i;

    EnterCriticalSection();
    for (i = 0; i < 4; i++) {
        DisableEvent(D_800AB320.unk14[i]);
        CloseEvent(D_800AB320.unk14[i]);
    }
    ExitCriticalSection();
}

void func_800A8E10(void) {
    s32 r;

    if (D_800AB320.unk7 == 0) {
        func_800A8ED8();
        D_800AB320.unk7 = 1;
    }
    r = func_800A8F44();
    if (r != -1) {
        D_800AB320.unk4[D_800AB320.unk6] = (r == 0 || r == 3) ? 1 : 0;
        D_800AB320.unk7 = 0;
        D_800AB320.unk6 ^= 1;
    }
}

u8 func_800A8EA4(void) {
    s32 i;
    u8 r = 0;

    for (i = 0; i < 2; i++) {
        r |= D_800AB320.unk4[i];
    }
    return r;
}

void func_800A8ED8(void) {
    s32 i;

    for (i = 0; i < 4; i++) {
        TestEvent(D_800AB320.unk14[i]);
    }
    _card_wait(0);
    _card_wait(1);
    _card_info(D_800AB320.unk6 << 4);
}

s32 func_800A8F44(void) {
    s32 i;
    s32 r = -1;

    for (i = 0; i < 4; i++) {
        if (TestEvent(D_800AB320.unk14[i]) == 1) {
            r = i;
            break;
        }
    }
    return r;
}

void func_800A8FBC(void) {
    D_800AB320.unk8 = 1;
}

s32 func_800A8FCC(void) {
    return D_800AB320.unk3;
}

void func_800A8FDC(void) {
    if (func_800A832C() == 0) {
        D_800AB320.unk1 = 1;
        D_800AB320.unk2 = 1;
        D_800AB320.unkC = 0;
    } else {
        D_800AB320.unk1 = 1;
        D_800AB320.unk2 = 0;
        D_800AB320.unkC = 0;
        fade_in(0x20);
    }
}

void func_800A903C(s32 x, s32 y, s32 z, Ovl14Text *t, s32 arg4) {
    SprtWords *p;
    s32 c = D_800AB320.unkC;

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

void func_800A9180(void) {
    s32 r;

    switch (D_800AB320.unk3) {
    case 0:
        g_gameState->unkB = 1;
        r = func_800A81B8();
        if (r != 0xFF) {
            menu_open(8, ((r & 3) << 14) | 0x3A98);
            func_800A812C(3);
        } else {
            func_800A812C(4);
        }
        break;
    case 1:
        func_800A812C(2);
        menu_open(4, 1);
        break;
    }
}
