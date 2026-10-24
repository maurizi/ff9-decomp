#include "ovl_0a.h"

void func_800C8124(void) {
    u8 *s;
    u8 *p;
    s32 w;
    s32 i;

    p = g_gameState->unk1C->unk80C;
    if (p != NULL && *p != 0xFF) {
        s = p;
    } else {
        s = text_get(1);
    }
    w = func_8002B980(s, 0) + 0x10;
    if (w < 0x44) {
        w = 0x44;
    }
    D_800C9E20[5].w = w;
    D_800C9E20[5].x = 0x134 - w;
    for (i = 0; i < 6; i++) {
        D_800CB188.unk8[i].unk4 = D_800C9E20[i];
        func_80063118(&D_800CB188.unk8[i], NULL, 0);
    }
    if (D_80077E51 != 0) {
        func_80063118(&D_800CB188.unk8[0], &D_800C9E60[0], 1);
        func_80063118(&D_800CB188.unk8[1], &D_800C9E60[1], 1);
        func_80063118(&D_800CB188.unk8[2], &D_800C9E60[2], 1);
        func_80063118(&D_800CB188.unk8[3], &D_800C9E60[3], 1);
    }
}

void func_800C8284(void) {
    if (g_padRepeat & BTN_CIRCLE) {
        snd_playSe(0x65);
        menuScreen_close();
    } else if (g_padRepeat & BTN_CROSS) {
        func_800C923C();
    } else if (g_padRepeat & PAD_UP) {
        snd_playSe(0x67);
        D_800CB188.unk0 = D_800CB188.unk0 != 0 ? D_800CB188.unk0 - 1 : 6;
    } else if (g_padRepeat & PAD_DOWN) {
        snd_playSe(0x67);
        D_800CB188.unk0 = D_800CB188.unk0 < 6 ? D_800CB188.unk0 + 1 : 0;
    }
}

void func_800C8354(void) {
    Unk80077E50 *m;
    s32 old;
    s32 new;

    if (g_padRepeat & BTN_CIRCLE) {
        snd_playSe(0x65);
        D_800CB188.unk1 = 0;
        return;
    } else if (g_padRepeat & BTN_CROSS) {
        snd_playSe(0x67);
        menuScreen_open(D_800CB188.unk0 + 1);
        return;
    } else if (g_padRepeat & PAD_UP) {
        m = &g_menuScreen;
        old = m->unk3;
        new = func_800337D0(old);
    } else if (g_padRepeat & PAD_DOWN) {
        m = &g_menuScreen;
        old = m->unk3;
        new = func_8003380C(old);
    } else {
        return;
    }
    if (old != new) {
        snd_playSe(0x67);
        old = new;
        m->unk3 = old;
    }
}

void func_800C842C(void) {
    if (g_padRepeat & BTN_CIRCLE) {
        snd_playSe(0x65);
        if (D_800CB188.unk4 == 0) {
            D_800CB188.unk1 = 0;
        } else {
            D_800CB188.unk4 = 0;
        }
    } else if (g_padRepeat & PAD_UP) {
        snd_playSe(0x67);
        if (D_800CB188.unk4 == 0) {
            D_800CB188.unk2 = D_800CB188.unk2 != 0 ? D_800CB188.unk2 - 1 : 3;
        } else {
            D_800CB188.unk3 = D_800CB188.unk3 != 0 ? D_800CB188.unk3 - 1 : 3;
        }
    } else if (g_padRepeat & PAD_DOWN) {
        snd_playSe(0x67);
        if (D_800CB188.unk4 == 0) {
            D_800CB188.unk2 = D_800CB188.unk2 < 3 ? D_800CB188.unk2 + 1 : 0;
        } else {
            D_800CB188.unk3 = D_800CB188.unk3 < 3 ? D_800CB188.unk3 + 1 : 0;
        }
    } else if (g_padRepeat & BTN_CROSS) {
        if (D_800CB188.unk4 == 0) {
            snd_playSe(0x67);
            D_800CB188.unk4 = 1;
            D_800CB188.unk3 = D_800CB188.unk2;
        } else {
            snd_playSe(0x67);
            func_800C85B8(D_800CB188.unk2, D_800CB188.unk3);
            D_800CB188.unk4 = 0;
            D_800CB188.unk2 = D_800CB188.unk3;
        }
    }
}

void func_800C85B8(s32 arg0, s32 arg1) {
    Character *c;
    Character *t;

    if (arg0 == arg1) {
        c = g_gameState->unk1C->party[arg1];
        if (c != NULL) {
            c->unk34.b.unk9 ^= 1;
        }
    } else {
        t = g_gameState->unk1C->party[D_800CB188.unk2];
        g_gameState->unk1C->party[D_800CB188.unk2] = g_gameState->unk1C->party[D_800CB188.unk3];
        g_gameState->unk1C->party[D_800CB188.unk3] = t;
        func_800C86C8(arg0, arg1);
        if (g_gameState->unk1C->party[g_menuScreen.unk3] == NULL) {
            g_menuScreen.unk3 = party_getFirstMember();
        }
    }
}

void func_800C86C8(s32 arg0, s32 arg1) {
    RECT a0;
    RECT a1;
    RECT a2;
    RECT b0;
    RECT b1;
    RECT b2;

    ui_getPartyPortraitRects(arg0, &a0, &a1, &a2);
    ui_getPartyPortraitRects(arg1, &b0, &b1, &b2);
    StoreImage(&a0, g_portraitSavedVram);
    DrawSync(0);
    MoveImage(&a1, 0x3C0, 0x1B0);
    DrawSync(0);
    MoveImage(&a2, 0x3D0, 0x1B0);
    DrawSync(0);
    MoveImage(&b0, a0.x, a0.y);
    DrawSync(0);
    MoveImage(&b1, a1.x, a1.y);
    DrawSync(0);
    MoveImage(&b2, a2.x, a2.y);
    DrawSync(0);
    a1.x = 0x3C0;
    a1.y = 0x1B0;
    a2.x = 0x3D0;
    a2.y = 0x1B0;
    LoadImage(&b0, g_portraitSavedVram);
    DrawSync(0);
    MoveImage(&a1, b1.x, b1.y);
    DrawSync(0);
    MoveImage(&a2, b2.x, b2.y);
    DrawSync(0);
}

void func_800C883C(s32 arg0) {
    u8 buf[16];
    RECT *r;
    Character *c;
    s32 x;
    s32 color;
    s16 maxmp;

    c = g_gameState->unk1C->party[arg0];
    r = &D_800CB188.unk8[arg0].unk4;
    x = r->x + 8;
    if (!c->unk34.b.unk9) {
        x = r->x + 0x16;
    }
    ui_drawPortrait(x, r->y + 1, 0x14, arg0, 0x2D, c->hp == 0);
    text_drawBlock(r->x + 0x3C, r->y + 3, 0x14, c->unk0, 0);
    ui_drawStatusIcons(r->x + 0x86, r->y + 4, 0x14, c->status, 0);
    text_formatNumber(c->level, buf, 2, 1);
    ui_drawSprite(r->x + 0x3C, r->y + 0x12, 0x14, 0xB4, 0);
    ui_drawSmallDigits(r->x + 0x60, r->y + 0x12, 0x14, buf, 0);
    ui_drawSprite(r->x + 0x3C, r->y + 0x1B, 0x14, 0xAF, 0);
    text_formatNumber(c->hp, buf, 4, 1);
    buf[4] = 0x2F;
    if (c->hp != 0) {
        color = ((u16)(c->maxHp / 6) >= c->hp) * 4;
    } else {
        color = 3;
    }
    text_formatNumber(c->maxHp, &buf[5], -1, 1);
    ui_drawSmallDigits(r->x + 0x52, r->y + 0x1B, 0x14, buf, color);
    ui_drawSprite(r->x + 0x3C, r->y + 0x24, 0x14, 0xB0, 0);
    text_formatNumber(c->mp, buf, 3, 1);
    buf[3] = 0x2F;
    maxmp = c->maxMp;
    color = ((s16)(maxmp / 6) >= c->mp) * 4;
    text_formatNumber(maxmp, &buf[4], -1, 1);
    ui_drawSmallDigits(r->x + 0x59, r->y + 0x24, 0x14, buf, color);
    ui_drawStones(r->x + 0x9B, r->y + 0x14, 0x14, c->stones, c->maxStones);
}

void func_800C8AE8(RECT *r, s32 otz) {
    TileWords *p;

    setlen(g_primPtr, 3);
    p = (TileWords *)g_primPtr;
    p->rgbc = 0x62000000;
    p->xy = ((r->x + 8) & 0xFFFF) | (r->y << 16);
    p->wh = 0x30002E;
    g_primPtr = (u8 *)(p + 1);
    addPrim(&g_ot[otz], p);
    SetDrawTPage(g_primPtr, 1, 1, 0);
    addPrim(&g_ot[otz], g_primPtr);
    g_primPtr += 8;
}

void func_800C8C08(void) {
    u8 buf[16];
    s32 v;

    v = game_getPlayTime();
    ui_drawSprite(D_800C9EA0->x + 6, D_800C9EA0->y + 8, 0x13, 0xB1, 0);
    ui_drawSprite(D_800C9EA0->x + 6, D_800C9EA0->y + 0x15, 0x13, 0xB2, 0);
    ui_drawSmallDigits(D_800C9EA0->x + 0x13, D_800C9EA0->y + 9, 0x13, buf, func_8002AA04(v, buf, 1));
    text_formatNumber(g_gameState->unk1C->gil, buf, 7, 1);
    strcat(buf, D_800C9EA4);
    ui_drawSmallDigits(D_800C9EA0->x + 0x13, D_800C9EA0->y + 0x16, 0x13, buf, 0);
    ui_drawLabel(D_800C9EA0->x + 6, D_800C9EA0->y - 3, 0x13, D_800704C0, 0);
    win_drawBox(D_800C9EA0, 0x14, 2);
}

void func_800C8D40(void) {
    GameData *gd;

    ui_drawLabel(D_800C9EA8->x + 8, D_800C9EA8->y - 3, 0x13, D_8007046C, 0);
    win_drawBox(D_800C9EA8, 0x14, 2);
    win_drawShadow(D_800C9EA8, 0x14, 2);
    gd = g_gameState->unk1C;
    if (gd->unk80C != NULL && *gd->unk80C != 0xFF) {
        text_drawBlock(D_800C9EA8->x + 8, D_800C9EA8->y + 7, 0x13, gd->unk80C, 0);
    } else {
        text_drawBlock(D_800C9EA8->x + 8, D_800C9EA8->y + 7, 0x13, text_get(1), 0);
    }
}

void func_800C8E2C(void) {
    u8 blink;

    blink = (D_80077E54 & 3) != 0;
    help_setCursorOnTop(D_800CB188.unk1 - 1U >= 2);
    if (!fade_isBusy()) {
        if (D_800CB188.unk1 == 0 || blink) {
            g_menuHelp.x = 0xE0;
            g_menuHelp.otz = 1;
            g_menuHelp.y = D_800CB188.unk0 * 0xF + 0x11;
            help_drawCursor();
        }
        if (D_800CB188.unk1 == 1) {
            g_menuHelp.otz = 0xC;
            g_menuHelp.x = D_800CB188.unk8[0].unk4.x - 0xE;
            g_menuHelp.y = D_80077E53 * 0x2E + 0x20;
            help_drawCursor();
        }
        if (D_800CB188.unk1 == 2) {
            if (D_800CB188.unk4 == 1) {
                g_menuHelp.otz = 0xB;
                g_menuHelp.x = D_800CB188.unk8[0].unk4.x - 0xA;
                g_menuHelp.y = D_800CB188.unk3 * 0x2E + 0x24;
                help_drawCursor();
                if (blink) {
                    ui_drawHand(D_800CB188.unk8[0].unk4.x - 0xE, D_800CB188.unk2 * 0x2E + 0x20, 0xC);
                }
            } else if (D_800CB188.unk4 == 0) {
                g_menuHelp.otz = 0xC;
                g_menuHelp.x = D_800CB188.unk8[0].unk4.x - 0xE;
                g_menuHelp.y = D_800CB188.unk2 * 0x2E + 0x20;
                help_drawCursor();
            }
        }
    }
}

void func_800C9014(void) {
    g_menuHelp.text = 0;
    if (D_800CB188.unk1 == 0) {
        g_menuHelp.text = (u8 *)menuScreen_getCommandHelp(D_800CB188.unk0);
    } else if (D_800CB188.unk1 == 1) {
        if (D_800CB188.unk0 + 1 == 3) {
            func_800C92C8(D_800CB188.unk0);
        } else {
            s32 a = D_800CB188.unk0 + 2;
            if (D_800CB188.unk0 + 1 == 4) {
                func_800C90D0(a);
            } else {
                g_menuHelp.text = text_get(a);
            }
        }
    } else if (D_800CB188.unk1 == 2) {
        g_menuHelp.text = text_get(D_800CB188.unk4 + 0xB);
    }
}

void func_800C90D0(s32 arg0) {
    Character *c;
    u32 next;

    c = g_gameState->unk1C->party[D_80077E53];
    if (c != NULL) {
        if (c->level >= 99) {
            next = c->exp;
        } else {
            next = g_levelExp[c->level];
        }
        func_8002BE44(0, c->unk0);
        text_formatNumber(c->exp, D_800CB168, 7, 1);
        text_formatNumber(next - c->exp, D_800CB178, 7, 1);
        func_8002BE44(1, D_800C9EC0);
        func_8002BE44(2, D_800CB168);
        func_8002BE44(3, D_800C9EC4);
        func_8002BE44(4, D_800C9ECC);
        func_8002BE44(5, D_800CB178);
        func_8002BE44(6, D_800C9ED4);
        func_8002BE44(7, c->status != 0 ? D_800C9EE4 : D_800C9EDC);
        D_80077DF0 = text_get(5);
    }
}

void func_800C923C(void) {
    if (D_800CB188.unk0 + 1 != 5) {
        if (menuScreen_needsMember(D_800CB188.unk0 + 1) == 0) {
            menuScreen_open(D_800CB188.unk0 + 1);
        } else {
            D_800CB188.unk1 = 1;
        }
    } else {
        D_800CB188.unk1 = 2;
        D_800CB188.unk4 = 0;
    }
    snd_playSe(0x67);
}

void func_800C92BC(void) {
    g_menuScreen.unk1 = 0;
}

void func_800C92C8(u8 arg0) {
    s32 i;
    s32 j;
    Character *c;
    s32 id;

    i = 0;
    j = 5;
    c = g_gameState->unk1C->party[D_80077E53];
    do {
        func_8002BE44(i, D_800C9EAC[i]);
        id = c->equip[i];
        if (id != 0xFF) {
            func_8002BE44(j, D_8006A218 + D_80069018[id].unk0);
        } else {
            func_8002BE44(j, text_get(0xD));
        }
        j++;
        func_8002BE74(i, id);
        i++;
    } while (i < 5);
    g_menuHelp.text = text_get(4);
}

void func_800C93D4(void) {
    RECT r;

    if (g_menuScreen.unk1 != 0) {
        D_800CB188.unk0 = 0;
        D_800CB188.unk2 = 0;
    }
    D_800CB188.unk1 = 0;
    text_setBank(D_800A7860);
    func_800C8124();
    r.x = 8;
    r.y = 8;
    r.w = 0xE8;
    r.h = g_gameState->unkC->disp.h - 0xF;
    help_setBounds(&r);
    help_setCallback(func_800C9014);
    help_setCursorOnTop(1);
    if (g_gameState->unk1C->party[g_menuScreen.unk3] == NULL) {
        g_menuScreen.unk3 = party_getFirstMember();
    }
}

void func_800C94B0(void) {
    func_800C951C();
    func_800C9580();
    func_800C95B8();
}

void func_800C94E0(void) {
    help_setCursorOnTop(1);
    help_setBounds(NULL);
    help_setCallback(NULL);
}

void func_800C9510(u8 arg0) {
    D_800CB188.unk0 = arg0;
}

void func_800C951C(void) {
    if (g_menuScreen.unk1 == 0 && fade_isBusy() == 0) {
        if (D_800C9E14[D_800CB188.unk1] != NULL) {
            D_800C9E14[D_800CB188.unk1]();
        }
    }
}

void func_800C9580(void) {
    func_800C8C08();
    func_800C8D40();
    func_800C9630();
    func_800C8E2C();
}

void func_800C95B8(void) {
    s32 i;

    if (g_menuScreen.unk1 != 0) {
        for (i = 0; i < 6; i++) {
            if (D_800CB188.unk8[i].unkC != 0) {
                func_800631AC(&D_800CB188.unk8[i]);
            }
        }
    }
}

void func_800C9630(void) {
    RECT r;
    s16 y;
    s32 i;
    RECT *p;

    i = 0;
    do {
        p = &D_800CB188.unk8[i].unk4;
        r.x = p->x + 0x82;
        y = p->y;
        r.w = 0x72;
        r.h = 0x12;
        r.y = y;
        if (g_gameState->unk1C->party[i] != NULL) {
            func_800C883C(i);
        }
        func_800C8AE8(p, 0x14);
        win_drawBox(&r, 0x14, 0);
        win_drawBox(p, 0x14, 0);
        win_drawShadow(p, 0x15, 0);
        i++;
    } while (i < 4);
}

s32 func_800C9700(u16 *arg0) {
    return *arg0 & 0x7FFF;
}

u8 *func_800C970C(u8 *arg0, FieldSprHdr *h, SVECTOR *pos, SVECTOR *rot, s32 scale, s32 frame, s32 abr, s32 arg7,
                  u32 *arg8) {
    VECTOR s;
    SVECTOR zero;
    MATRIX m;
    FieldSprArgs c;
    u16 *p;
    u8 *b;
    s32 n;
    s32 f;
    u16 *o;

    if (rot == NULL) {
        rot = &zero;
        ((s32 *)&zero)[0] = 0;
        ((s32 *)&zero)[1] = 0;
    }
    n = h->unk0 & 0x7FFF;
    f = frame;
    if (f >= n) {
        f = frame % n;
    }
    o = &h->unk8[f];
    n = n * 2 + 8;
    b = (u8 *)h + n;
    p = (u16 *)b;
    f = *p++;
    c.unk0 = p;
    c.unk4 = (u8 *)(p + f + 1);
    c.unk10 = h->unk2 | ((abr & 3) << 5);
    c.unk14 = h->unk4;
    c.unkC = (h->unk6 - 1) * 2;
    c.unk8 = (h->unk7 - 1) * 2;
    n = *o;
    c.unk1C = ((u8 *)h)[n++];
    h = (FieldSprHdr *)((u8 *)h + n);
    gte_SetRotMatrix(&g_gameState->unk1C->unk18);
    gte_SetTransMatrix(&g_gameState->unk1C->unk18);
    gte_ldv0(pos);
    gte_rtv0tr();
    func_800316D8(rot, &m);
    if (scale >= 0) {
        s.vx = s.vy = s.vz = scale;
        ScaleMatrix(&m, &s);
    }
    gte_stlvnl(m.t);
    if (rot->pad != 0) {
        MulRotMatrix(&m);
    }
    gte_SetRotMatrix(&m);
    gte_SetTransMatrix(&m);
    c.unk20 = (abr == 0xFF) ? 0x2C : 0x2E;
    c.unk18 = arg7;
    return func_800C9AB8((u8 *)h, &c, arg0, arg8);
}

/* upload the images and palettes of a texture file to VRAM */
void func_800C9968(s32 *arg0) {
    RECT r;
    RECT *q;
    u16 *p;
    u8 *data;
    s32 n;
    s32 i;
    s32 sz;
    s32 m;
    s32 ofs;

    ofs = arg0[0];
    q = (RECT *)((u8 *)arg0 + ofs);
    ofs = arg0[1];
    i = 0;
    p = (u16 *)((u8 *)arg0 + ofs);
    ofs = *(s32 *)q;
    n = arg0[2];
    data = (u8 *)arg0 + ofs;
    for (; i < n; i++) {
        r.x = p[0];
        r.y = p[1];
        r.w = p[2];
        r.h = p[3];
        sz = r.w * r.h * 2;
        p += 4;
        LoadImage(&r, (u32 *)p);
        p = (u16 *)((u8 *)p + sz);
    }
    m = ((s32 *)q)[1];
    q++;
    for (i = 0; i < m; i++) {
        r.x = q->x;
        r.y = q->y;
        r.w = q->w;
        r.h = q->h;
        LoadImage(&r, (u32 *)data);
        q++;
        data += r.w * r.h * 2;
    }
}

INCLUDE_RODATA("asm/us/ovl_0a/nonmatchings/tu_800C8124", D_800A7860);
