#include "ovl_36.h"

INCLUDE_RODATA("asm/us/ovl_36/nonmatchings/ovl_36", D_801EDF04);

INCLUDE_RODATA("asm/us/ovl_36/nonmatchings/ovl_36", D_801EDF08);

INCLUDE_RODATA("asm/us/ovl_36/nonmatchings/ovl_36", D_801EDF10);

void func_801F08F4(void) {
    s32 i;
    s32 id;

    D_801F347C.unk0 = 1;
    D_801F347C.unk1 = 0;
    D_801F347C.unk2 = 0;
    D_801F347C.unk4 = 0;
    D_801F347C.unk3 = card_countKinds();
    D_801F347C.unk6 = 0xFF;
    text_setBank("-");
    func_801F2E60();
    func_801F2CD4();
    help_setCallback(D_801F347C.unk3 ? func_801F22DC : 0);
    setRECT(&D_801F347C.unk18D0, 8, 0x26, 0x9E, 0xB6);
    setRECT(&D_801F347C.unk18D8, 0xAA, 0x26, 0x8A, 0x4E);
    setRECT(&D_801F347C.unk18E0, 0xAA, 0x76, 0x8A, 0x66);
    for (i = 0; i < 100; i++) {
        D_801F347C.unk6C[i] = 0;
    }
    func_801F2FCC();
    while (cd_getState()) {
    }
    D_801F347C.unk18EC = file_find(1, 0x32);
    for (i = 0; i < 4; i++) {
        id = D_801F30D4[i];
        D_801F347C.unk1948[i].unk0 = pad_getRepeatDelay(0, id);
        D_801F347C.unk1948[i].unk4 = pad_getRepeatInterval(0, id);
        pad_setRepeatDelay(0, id, 0x10);
        pad_setRepeatInterval(0, id, 10);
    }
    func_801F1C88();
    id = D_801F347C.unk2 * 10 + D_801F347C.unk1;
    if (D_801F347C.unk8[id]) {
        func_801F2C5C(id);
    }
}

/* table and text after func_801F08F4's "-" (not referenced by it) */
INCLUDE_RODATA("asm/us/ovl_36/nonmatchings/ovl_36", D_801EDF1C);

void func_801F0AE8(void) {
    s32 cur;
    s32 next;
    s32 dir;

    cur = D_801F347C.unk2 * 10 + D_801F347C.unk1;
    if (g_padRepeat & BTN_CIRCLE) {
        snd_playSe(0x65);
        menuScreen_open(0);
        return;
    }
    if (g_padRepeat & BTN_CROSS) {
        func_801F0CC8();
        return;
    }
    if (g_padRepeat & (PAD_UP | PAD_RIGHT | PAD_DOWN | PAD_LEFT)) {
        if ((g_padRepeat & PAD_UP) && D_801F347C.unk1 != 0) {
            D_801F347C.unk1--;
        }
        if ((g_padRepeat & PAD_DOWN) && D_801F347C.unk1 < 9) {
            D_801F347C.unk1++;
        }
        if ((g_padRepeat & PAD_LEFT) && D_801F347C.unk2 != 0) {
            D_801F347C.unk2--;
        }
        if ((g_padRepeat & PAD_RIGHT) && D_801F347C.unk2 < 9) {
            D_801F347C.unk2++;
        }
        next = D_801F347C.unk2 * 10 + D_801F347C.unk1;
        if (next != cur) {
            snd_playSe(0x67);
            func_801F2C5C(next);
        }
    } else if (g_padRepeat & BTN_L1) {
        dir = 0;
        if (D_801F347C.unk8[cur] >= 2) {
            func_801F2FE0(cur, dir);
            snd_playSe(0x417);
        }
    } else if (g_padRepeat & BTN_R1) {
        dir = 1;
        if (D_801F347C.unk8[cur] >= 2) {
            func_801F2FE0(cur, dir);
            snd_playSe(0x417);
        }
    }
}

void func_801F0CC8(void) {
    if (D_801F347C.unk8[D_801F347C.unk2 * 10 + D_801F347C.unk1] != 0) {
        snd_playSe(0x67);
        D_801F347C.unk0 = 0;
        D_801F347C.unk5 = 1;
        D_801F347C.unk18E8 = win_openFree(&D_801F3100, 10, 2);
        win_setDrawContent(D_801F347C.unk18E8, func_801F2F34);
        win_setOnOpened(D_801F347C.unk18E8, func_801F2F14);
        win_setOnClosed(D_801F347C.unk18E8, func_801F2F24);
        win_setTitle(D_801F347C.unk18E8, D_800704C4);
        win_setShadow(D_801F347C.unk18E8, 1);
        return;
    }
    snd_playSe(0x66);
}

void func_801F0DB0(void) {
    u8 buf[8];
    RECT *r;
    s32 a;
    s32 b;
    s32 n;
    s32 i;
    s32 j;
    s32 x;
    s32 y;
    u8 *s;
    s32 w;
    s32 k = 8;
    s32 k2 = 0x80;

    r = &D_801F347C.unk18D0;
    a = card_countAll();
    b = card_countKinds();
    ui_drawLabel(r->x + 8, r->y - 3, 0x1E, D_8007041C, 0);
    win_drawBox(r, 0x1E, 1);
    win_drawShadow(r, 0x1E, 1);
    s = text_get(0x27);
    w = func_8002BA68(s);
    text_formatNumber(a, buf, 3, 1);
    text_drawBlock(r->x + 8, r->y + 0xA2, 0x14, s, 0);
    ui_drawDigits(r->x + (w + k), r->y + 0xA4, 0x14, buf, 0);
    s = text_get(0x28);
    w = func_8002BA68(s);
    text_formatNumber(b, buf, 3, 1);
    ui_drawDigits(r->x + 0x80, r->y + 0xA4, 0x14, buf, 0);
    text_drawBlock(r->x - (w - k2), r->y + 0xA2, 0x14, s, 0);
    for (n = 0, i = 0, x = 4; i < 10; i++, x += 15) {
        for (j = 0, y = 10; j < 10; j++, y += 15) {
            func_801F2860(r->x + x, r->y + y, 0x14, n);
            n++;
        }
    }
}

void func_801F0F9C(void) {
    u8 buf[8];
    RECT *r;
    s32 a;
    s32 b;
    s32 c;
    s32 rank;
    s32 w;

    r = &D_801F347C.unk18D8;
    if (card_getWins() <= 99999) {
        a = card_getWins();
    } else {
        a = 99999;
    }
    if (card_getLosses() <= 99999) {
        b = card_getLosses();
    } else {
        b = 99999;
    }
    if (card_getDraws() <= 99999) {
        c = card_getDraws();
    } else {
        c = 99999;
    }
    rank = D_801F347C.unk7;
    ui_drawLabel(r->x + 8, r->y - 3, 0x1E, D_80070460, 0);
    win_drawBox(r, 0x1E, 2);
    win_drawShadow(r, 0x1F, 2);
    ui_drawEtchedLine(r->x + 4, r->y + 0x24, 0x14, r->w - 9);
    text_formatNumber(D_801F347C.unk18F0[3], buf, 4, 1);
    text_drawBlock(r->x + 0xA, r->y + 5, 0x14, text_get(0x29), 0);
    text_drawBlock(r->x + 0xA, r->y + 0x14, 0x14, text_get(rank), 0);
    text_drawBlock(r->x + 0xA, r->y + 0x26, 0x14, text_get(0x2A), 0);
    text_drawBlock(r->x + 0xA, r->y + 0x30, 0x14, text_get(0x2B), 0);
    text_drawBlock(r->x + 0xA, r->y + 0x3A, 0x14, text_get(0x2C), 0);
    ui_drawSmallDigits(r->x + 0x5A, r->y + 9, 0x14, buf, 0);
    ui_drawSmallDigits(r->x + 0x78, r->y + 0xB, 0x14, D_801EDF04, 0);
    w = func_801F0F9C_width(text_get(0x2C));
    text_formatNumber(a, buf, 5, 1);
    w += 0xB;
    ui_drawSmallDigits(r->x + w, r->y + 0x2A, 0x14, buf, 0);
    text_formatNumber(b, buf, 5, 1);
    ui_drawSmallDigits(r->x + w, r->y + 0x34, 0x14, buf, 0);
    text_formatNumber(c, buf, 5, 1);
    ui_drawSmallDigits(r->x + w, r->y + 0x3E, 0x14, buf, 0);
}

void func_801F12B8(s32 id) {
    u8 buf[8];
    RECT *r;
    CardTurn *turn;
    CardCache *cache;
    u8 *name;
    u8 same;
    s32 i;
    s32 dx;
    s32 x;
    s32 y;
    s32 dy;
    s32 a;
    s32 b;
    s32 n;
    s32 cnt;

    r = &D_801F347C.unk18E0;
    n = D_801F347C.unk8[id];
    cnt = n;
    if (n >= 6) {
        n = 5;
    }
    turn = &D_801F347C.unk1941;
    cache = &D_801F347C.unk1900;
    if (cnt != 0) {
        same = id == cache->unk7;
        name = (u8 *)card_getName(id);
        if (cnt >= 2) {
            text_formatNumber(D_801F347C.unk6C[id] + 1, buf, 3, 1);
            text_formatNumber(cnt, buf + 4, -1, 1);
            buf[3] = '/';
            ui_drawSmallDigits(r->x + 0x2B, r->y + 0x43, 0x14, buf, 0);
            a = func_8002BE10(4);
            b = func_8002BE10(5);
            ui_drawSprite(r->x + 0x1E, r->y + 0x41, 0x14, a, 0);
            ui_drawSprite(r->x + 0x5E, r->y + 0x41, 0x14, b, 0);
        }
        i = 0;
        func_801F1D6C(r->x + 0x10, r->y + 0x50, 0x14, 0x37, 0x64808080);
        text_drawBlock(r->x + 0x20, r->y + 0x50, 0x14, name, 0);

        while (i < n) {
            dx = i * 4 + 0x2E;
            x = r->x + dx;
            dy = i + 9;
            y = r->y + dy;
            if (i != 0 && turn->unk0 != 0) {
                if (turn->unk1 == 0) {
                    x += turn->unk2 - 4;
                } else {
                    x -= turn->unk2;
                }
            }
            func_801F2730(x, y, 0x14, id, i, same);
            i++;

        }
    }
    win_drawBox(r, 0x1E, 2);
    win_drawShadow(r, 0x1F, 2);
}

void func_801F1508(void) {
    RECT *r = &D_801F347C.unk18D0;
    s32 a;
    s32 x, y;
    u32 rgbc;

    a = D_80077DC0 & 0x3F;
    if (a >= 0x20) {
        a = 0x3F - a;
    }
    x = r->x + (s16)(D_801F347C.unk2 * 15 + 4);
    y = r->y + (s16)(D_801F347C.unk1 * 15 + 10);
    setlen(g_primPtr, 5);
    rgbc = (a * 8) | 0x48000000;
    ((PolyF4Words *)g_primPtr)->rgbc = rgbc;
    ((PolyF4Words *)g_primPtr)->xy0 = x | (y << 16);
    ((PolyF4Words *)g_primPtr)->xy1 = (x + 13) | (y << 16);
    ((PolyF4Words *)g_primPtr)->xy2 = (x + 13) | ((y + 13) << 16);
    ((PolyF4Words *)g_primPtr)->xy3 = 0x55555555;
    addPrim(&g_ot[18], g_primPtr);
    g_primPtr += 0x18;
    setlen(g_primPtr, 5);
    ((PolyF4Words *)g_primPtr)->rgbc = rgbc;
    ((PolyF4Words *)g_primPtr)->xy0 = x | (y << 16);
    ((PolyF4Words *)g_primPtr)->xy1 = x | ((y + 13) << 16);
    ((PolyF4Words *)g_primPtr)->xy2 = (x + 13) | ((y + 13) << 16);
    ((PolyF4Words *)g_primPtr)->xy3 = 0x55555555;
    addPrim(&g_ot[18], g_primPtr);
    g_primPtr += 0x18;
}

void func_801F166C(void) {
    u8 blink = (D_80077DC0 & 3) != 0;

    if (!fade_isBusy()) {
        if (D_801F347C.unk0 == 1 || (D_801F347C.unk0 == 2 && blink)) {
            g_menuHelp.otz = 0x11;
            g_menuHelp.x = D_801F347C.unk18D0.x + D_801F347C.unk2 * 15 - 20;
            g_menuHelp.y = D_801F347C.unk18D0.y + D_801F347C.unk1 * 15 + 14;
            help_drawCursor();
        }
        if (D_801F347C.unk0 == 2) {
            Window *w = D_801F347C.unk18E8;
            g_menuHelp.x = w->unk0 + 0x16;
            g_menuHelp.y = w->unk2 + (s16)(D_801F347C.unk5 * 15 + 0x32);
            g_menuHelp.otz = 9;
            help_drawCursor();
        }
    }
}

void func_801F1778(void) {
    RECT a, b;
    CardCache *cc;
    CardSlot *s;
    s32 *ofs;
    s32 n;
    u32 k;

    cc = &D_801F347C.unk1900;
    ofs = D_801F0274;
    if (cc->unk2 >= 12) {
        cc->unkD[cc->unk0].unk0 = 0;
        cc->unk2++;
        cc->unk0 = (cc->unk0 + 1) % 12;
    }
    s = &cc->unkD[cc->unk1];
    {
        u8 id = D_801F347C.unk4;
        s->unk0 = 1;
        s->unk1 = id;
    }
    cc->unk2++;
    cc->unk1 = (cc->unk1 + 1) % 12;
    n = func_801F3034(D_801F347C.unk10D0, D_801F347C.unkD0);
    if (ofs[D_801F347C.unk4 + 1] != ofs[D_801F347C.unk4]) {
        func_801F3034((u8 *)D_801F0274 - 4 + ofs[D_801F347C.unk4], D_801F347C.unkD0 + n);
    }
    k = s->unk2;
    a.x = 0;
    a.w = 0x100;
    a.h = 1;
    b.w = 0x13;
    b.h = 0x2F;
    a.y = k + 0x1F4;
    b.x = (u8)(k % 3) * 0x13;
    b.y = (u8)(k / 3) * 0x2F + 0x100;
    LoadImage(&a, (u32 *)D_801F347C.unkD0);
    LoadImage(&b, (u32 *)D_801F347C.unk2D0);
    DrawSync(0);
    if (s->unk1 == cc->unk5) {
        func_801F2D88_inline(s);
    }
}

void func_801F1A48(void) {
    CardCache *c = &D_801F347C.unk1900;

    if (c->unkA != 0) {
        if (cd_getState() != 0) {
            return;
        }
        c->unkA = 0;
        func_801F1778();
        func_801F1A48_read(c);
    } else {
        func_801F1A48_read(c);
    }
}

void func_801F1AE8(s32 x, s32 y, s32 z, s32 dim) {
    SprtWords *p;
    u32 clut = (GetClut(0, 0x1F2) << 16) | 0xB000;
    s32 c;
    u32 rgbc;

    setlen(g_primPtr, 4);
    c = 0x80 - dim * 0x16;
    rgbc = c | (c << 8) | (c << 16) | 0x64000000;
    p = (SprtWords *)g_primPtr;
    p->rgbc = rgbc;
    p->wh = 0x2F0026;
    p->xy = (x + 2) | ((y + 2) << 16);
    p->uvclut = clut;
    g_primPtr = (u8 *)(p + 1);
    addPrim(&g_ot[z], p);
    SetDrawTPage(g_primPtr, 1, 1, 0xBF);
    addPrim(&g_ot[z], g_primPtr);
    g_primPtr += 8;
    func_801F1D6C(x, y, 0x14, 0, rgbc);
}

void func_801F1C88(void) {
    s32 i;
    u8 n;
    CardMenu *m;

    help_setCallback(card_countAll() ? func_801F22DC : NULL);
    for (i = 0; i < 100; i++) {
        n = card_countOf(i);
        D_801F347C.unk8[i] = n;
        if (D_801F347C.unk6C[i] >= n) {
            D_801F347C.unk6C[i] = n - 1;
        }
    }
    func_801F2138(D_801F347C.unk18F0);
    m = &D_801F347C;
    m->unk7 = 0x20;
    for (i = 1; i < 0x20; i++) {
        if (m->unk18F0[3] < D_801F3128[i]) {
            m->unk7 = i - 1;
            return;
        }
    }
}

void func_801F1D6C(s32 x, s32 y, s32 z, s32 idx, s32 rgbc) {
    SprtWords *p;
    Ovl36Sprite *t;

    setlen(g_primPtr, 4);
    p = (SprtWords *)g_primPtr;
    t = &D_801F3188[idx];
    p->rgbc = rgbc;
    p->xy = x | (y << 16);
    p->uvclut = t->unk0;
    p->wh = t->unk4 | (t->unk5 << 16);
    g_primPtr = (u8 *)(p + 1);
    addPrim(&g_ot[z], p);
    SetDrawTPage(g_primPtr, 1, 1, t->unk6 | 0x20);
    addPrim(&g_ot[z], g_primPtr);
    g_primPtr += 8;
}

void func_801F1EA0(u8 *buf) {
    RECT a = D_801EDF08;
    RECT b = D_801EDF10;
    s32 i;

    StoreImage(&D_801F3348, (u32 *)buf);
    buf += 0x200;
    StoreImage(&D_801F3350, (u32 *)buf);
    buf += 0x200;
    StoreImage(&D_801F3358, (u32 *)buf);
    buf += 0x200;
    DrawSync(0);
    for (i = 0; i < 12; i++) {
        a.y = i + 0x1F4;
        b.x = (i % 3) * 0x13;
        b.y = (i / 3) * 0x2F + 0x100;
        StoreImage(&a, (u32 *)buf);
        DrawSync(0);
        StoreImage(&b, (u32 *)(buf + 0x200));
        DrawSync(0);
        buf += 0x8FC;
    }
}

void func_801F1FE8(u8 *buf) {
    RECT a = D_801EDF08;
    RECT b = D_801EDF10;
    s32 i;

    LoadImage(&D_801F3360, (u32 *)buf);
    buf += 0x200;
    LoadImage(&D_801F3368, (u32 *)buf);
    buf += 0x200;
    DrawSync(0);
    LoadImage(&D_801F3370, (u32 *)buf);
    buf += 0x200;
    DrawSync(0);
    for (i = 0; i < 12; i++) {
        a.y = i + 0x1F4;
        b.x = (i % 3) * 0x13;
        b.y = (i / 3) * 0x2F + 0x100;
        LoadImage(&a, (u32 *)buf);
        DrawSync(0);
        LoadImage(&b, (u32 *)(buf + 0x200));
        DrawSync(0);
        buf += 0x8FC;
    }
}

void func_801F2138(s32 *out) {
    s32 kinds;
    s32 arrows;
    s32 types;
    s32 i;
    s32 j;
    CardInfo *e;
    Unk6 *c;

    e = card_getInfoTable();
    kinds = 0;
    arrows = 0;
    types = 0;
    for (i = 0; i < 100; i++) {
        if (D_801F347C.unk8[i] != 0) {
            kinds += e[i].unk4;
        }
    }
    memset(D_801F337C, 0, 0x100);
    for (i = 0; i < 100; i++) {
        for (j = 0; j < D_801F347C.unk8[i]; j++) {
            c = card_find(i, j);
            D_801F337C[c->unk1] = 1;
            arrows += D_801F3378[c->unk3];
        }
    }
    for (i = 0; i < 0x100; i++) {
        if (D_801F337C[i] != 0) {
            types += 5;
        }
    }
    out[0] = kinds;
    out[1] = arrows;
    out[2] = types;
    out[3] = kinds + arrows + types;
}

void func_801F22DC(void) {
    RECT r;
    u8 n;

    g_menuHelp.text = NULL;
    help_setBounds(NULL);
    if (D_801F347C.unk0 == 1) {
        n = D_801F347C.unk8[D_801F347C.unk2 * 10 + D_801F347C.unk1];
        if (n != 0) {
            g_menuHelp.text = text_get(n >= 2 ? 0x24 : 0x23);
        }
    } else if (D_801F347C.unk0 == 2) {
        if (D_801F347C.unk5 == 1) {
            r.x = 0;
            r.y = 0x8C;
            r.w = g_gameState->unkC->disp.w;
            r.h = g_gameState->unkC->disp.h - 0x8C;
            help_setBounds(&r);
        }
        g_menuHelp.text = text_get(D_801F347C.unk5 + 0x25);
    }
}

void func_801F23DC(void) {
    func_801F24A0();
    func_801F2508();
    func_801F2548();
}

void func_801F240C(void) {
    s32 i;
    s32 id;

    for (i = 0; i < 4; i++) {
        id = D_801F30E4[i];
        pad_setRepeatDelay(0, id, D_801F347C.unk1948[i].unk0);
        pad_setRepeatInterval(0, id, D_801F347C.unk1948[i].unk4);
    }
    help_setCallback(NULL);
    func_801F2EE8();
}

void func_801F24A0(void) {
    void (*f)();

    if (!fade_isBusy() && D_801F347C.unk1941.unk0 == 0) {
        f = D_801F30F4[D_801F347C.unk0];
        if (f != NULL) {
            f();
        }
    }
}

void func_801F2508(void) {
    func_801F0DB0();
    func_801F0F9C();
    func_801F2664();
    func_801F1508();
    func_801F166C();
}

void func_801F2548(void) {
    func_801F1A48();
    func_801F2B94();
    func_801F2C14();
}

void func_801F2578(void) {
    s32 cur;

    cur = D_801F347C.unk2 * 10 + D_801F347C.unk1;
    if (g_padRepeat & (BTN_CIRCLE | BTN_CROSS)) {
        if ((g_padRepeat & BTN_CROSS) && D_801F347C.unk5 == 0) {
            snd_playSe(0x67);
            card_remove(cur, D_801F347C.unk6C[cur]);
            func_801F1C88();
        } else {
            snd_playSe(0x65);
        }
        D_801F347C.unk0 = 0;
        win_close(D_801F347C.unk18E8);
        return;
    }
    if (g_padRepeat & (PAD_UP | PAD_DOWN)) {
        snd_playSe(0x67);
        D_801F347C.unk5 ^= 1;
    }
}

void func_801F2664(void) {
    RECT *r = &D_801F347C.unk18E0;
    CardCache *c = &D_801F347C.unk1900;
    s32 cur;

    cur = D_801F347C.unk2 * 10 + D_801F347C.unk1;
    ui_drawLabel(r->x + 8, r->y - 3, 0x1E, D_80070418, 0);
    if (c->unk7 != 0xFF) {
        if (c->unk7 != cur && c->unk9 != 0) {
            func_801F12B8(c->unk6);
        } else {
            do {
                func_801F12B8(cur);
            } while (0);
        }
    }
    win_drawBox(r, 0x1E, 2);
    win_drawShadow(r, 0x1F, 2);
}

void func_801F2730(s32 x, s32 y, s32 z, s32 id, s32 dim, u8 flag) {
    Unk6 *card;
    s32 c;

    if (flag) {
        if (dim == 0) {
            card = card_find(id, (s32)D_801F347C.unk6C[id] % (s32)D_801F347C.unk8[id]);
            func_801F2AB8(x, y, 0x14, card);
            func_801F2A04(x, y, 0x14, card);
        }
        func_801F1AE8(x, y, 0x14, dim);
        return;
    }
    c = 0x80 - dim * 0x16;
    func_801F1D6C(x, y, 0x14, 1, c | (c << 8) | (c << 16) | 0x64000000);
}

void func_801F2860(s32 x, s32 y, s32 z, s32 id) {
    s32 n;
    s32 icon;
    s32 q;
    s32 d;

    n = D_801F347C.unk8[id];
    if (n == 0) {
        icon = 0x1E;
    } else if (n < 2) {
        icon = card_getRarity(id) + 0x1F;
    } else {
        icon = card_getRarity(id) + 0x26;
        d = n % 10;
        q = n / 10;
        func_801F1D6C(x + 9, y + 7, 0x14, d + 0x2D, 0x64808080);
        if (n >= 10) {
            d = q % 10;
            func_801F1D6C(x + 5, y + 7, 0x14, d + 0x2D, 0x64808080);
        }
        if (n >= 100) {
            d = n / 100;
            func_801F1D6C(x + 1, y + 7, 0x14, d + 0x2D, 0x64808080);
        }
    }
    func_801F1D6C(x, y, 0x14, icon, 0x64808080);
}

void func_801F2A04(s32 x, s32 y, s32 otz, Unk6 *card) {
    u32 color;
    s32 i;

    color = func_801F30C8();
    for (i = 0; i < 8; i++) {
        if ((card->unk1 >> i) & 1) {
            func_801F1D6C(x + D_801F3108[i].unk2, y + D_801F3108[i].unk3, otz, D_801F3108[i].unk0, color);
        }
    }
}

void func_801F2AB8(s32 x, s32 y, s32 otz, Unk6 *card) {
    u8 s[4];
    u32 color;
    s32 i;
    s32 dx;

    color = func_801F30C8();
    s[0] = (card->unk2 >> 4) + 0xA;
    s[1] = card->unk3 + 0x1A;
    s[2] = (card->unk4 >> 4) + 0xA;
    s[3] = (card->unk5 >> 4) + 0xA;
    for (i = 0; i < 4; i++) {
        func_801F1D6C(x + 9 + i * 6, y + 0x26, otz, s[i], color);
    }
}

void func_801F2B94(void) {
    CardTurn *t;

    t = &D_801F347C.unk1941;
    if (t->unk0 != 0) {
        if (++t->unk2 >= 4) {
            if (t->unk1 == 1) {
                D_801F347C.unk6C[t->unk3] = D_801F347C.unk6C[t->unk3] >= D_801F347C.unk8[t->unk3] - 1 ? 0 : D_801F347C.unk6C[t->unk3] + 1;
            }
            t->unk2 = 0;
            t->unk0 = 0;
        }
    }
}

void func_801F2C14(void) {
    CardCache *c = &D_801F347C.unk1900;

    if (c->unk9 != 0) {
        if (++c->unk8 >= 4) {
            c->unk9 = 0;
            c->unk8 = 0;
        }
    }
}

void func_801F2C5C(s32 id) {
    CardCache *c;
    CardSlot *s;
    u8 prev;
    s32 owned;

    owned = D_801F347C.unk8[id];
    c = &D_801F347C.unk1900;
    prev = c->unk5;
    c->unk9 = 1;
    c->unk8 = 0;
    c->unk5 = id;
    c->unk6 = prev;
    if (owned != 0) {
        s = func_801F2D30(id);
        if (s == NULL) {
            c->unk4 = id;
            return;
        }
        func_801F2D88(s);
    }
}

void func_801F2CD4(void) {
    CardCache *c = &D_801F347C.unk1900;
    s32 i;

    c->unk1 = 0;
    c->unk0 = 0;
    c->unk2 = 0;
    c->unk6 = 0xFF;
    c->unk5 = 0xFF;
    c->unk3 = 0xFF;
    c->unk4 = 0xFF;
    c->unkA = 0;
    c->unk8 = 0;
    c->unk7 = 0xFF;
    for (i = 0; i < 12; i++) {
        c->unkD[i].unk0 = 0;
        c->unkD[i].unk1 = 0;
        c->unkD[i].unk2 = i;
    }
}

CardSlot *func_801F2D30(s32 id) {
    CardCache *c = &D_801F347C.unk1900;
    s32 i;

    for (i = 0; i < 12; i++) {
        if (c->unkD[i].unk0 != 0 && c->unkD[i].unk1 == id) {
            return &c->unkD[i];
        }
    }
    return NULL;
}

void func_801F2D88(CardSlot *s) {
    RECT a, b;
    u32 n = s->unk2;

    a.x = 0;
    a.w = 0x100;
    a.h = 1;
    b.w = 0x13;
    b.h = 0x2F;
    a.y = n + 0x1F4;
    b.x = (u8)(n % 3) * 0x13;
    b.y = (u8)(n / 3) * 0x2F + 0x100;
    MoveImage(&a, 0, 0x1F2);
    MoveImage(&b, 0x3C0, 0x1B0);
    DrawSync(0);
    D_801F347C.unk1900.unk7 = s->unk1;
}

void func_801F2E60(void) {
    u8 *a = D_801EE1B4;
    u8 *b = D_801EEFB4;

    func_801F1EA0((u8 *)0x801F6D30);
    LoadImage(&D_801F3168, (u32 *)a);
    LoadImage(&D_801F3170, (u32 *)(a + 0x200));
    LoadImage(&D_801F3178, (u32 *)b);
    LoadImage(&D_801F3180, (u32 *)(b + 0x200));
    DrawSync(0);
}

void func_801F2EE8(void) {
    func_801F1FE8((u8 *)0x801F6D30);
    DrawSync(0);
}

void func_801F2F14(Window *w) {
    D_801F347C.unk0 = 2;
}

void func_801F2F24(Window *w) {
    D_801F347C.unk0 = 1;
}

void func_801F2F34(Window *w) {
    text_drawBlock(w->unk0 + 0xE, w->unk2 + 0xA, w->unk18, text_get(0x20), 0);
    text_drawBlock(w->unk0 + 0x30, w->unk2 + 0x2E, w->unk18, text_get(0x21), 0);
    text_drawBlock(w->unk0 + 0x30, w->unk2 + 0x3D, w->unk18, text_get(0x22), 0);
}

void func_801F2FCC(void) {
    CardTurn *t = &D_801F347C.unk1941;

    t->unk0 = 0;
    t->unk2 = 0;
}

void func_801F2FE0(s32 card, s32 dir) {
    CardTurn *t;

    t = &D_801F347C.unk1941;
    t->unk0 = 1;
    t->unk2 = 0;
    t->unk1 = dir;
    t->unk3 = card;
    if (dir == 0) {
        if (D_801F347C.unk6C[t->unk3] != 0) {
            D_801F347C.unk6C[t->unk3]--;
        } else {
            D_801F347C.unk6C[t->unk3] = D_801F347C.unk8[t->unk3] - 1;
        }
    }
}

s32 func_801F3034(u8 *src, u8 *dst) {
    RleHeader *h = (RleHeader *)src;
    u8 *start = dst;
    u8 *end;
    u8 key;
    s32 n, i;

    key = ~h->unk1;
    src += 4;
    end = src + h->unk2;
    while (src < end) {
        if (*src <= key) {
            *dst++ = *src++;
        } else {
            n = *src++ - key;
            for (i = 0; i < n; i++) {
                *dst++ = *src;
            }
            src++;
        }
    }
    return dst - start;
}

u32 func_801F30C8(void) {
    return 0x64808080;
}

INCLUDE_RODATA("asm/us/ovl_36/nonmatchings/ovl_36", D_801EE1B4);

INCLUDE_RODATA("asm/us/ovl_36/nonmatchings/ovl_36", D_801EEFB4);

INCLUDE_RODATA("asm/us/ovl_36/nonmatchings/ovl_36", D_801F0274);
