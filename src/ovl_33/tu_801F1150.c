#include "ovl_33.h"

void func_801F1150(void) {
    s32 i;
    u8 lv;
    Character *c;
    AbilityMenu *d;

    D_801FC840.unk3 = 0xFF;
    D_801FC840.unk0 = 0;
    D_801FC840.unk1 = 1;
    D_801FC840.unk2 = 0;
    D_801FC840.unk260 = 0;
    text_setBank(D_801EE648);
    help_setCallback(help_nullHandler);
    help_setCursorOnTop(1);
    setRECT(&D_801FC840.unk14, 8, 8, 0xE4, 0x1A);
    setRECT(&D_801FC840.unk1C, 8, 0x26, 0x8C, 0xB6);
    setRECT(&D_801FC840.unk24, 0x94, 0x26, 0xA0, 0x62);
    setRECT(&D_801FC840.unk2C, 0x94, 0x8A, 0xA0, 0x52);
    setRECT(&D_801FC840.unk34, 0x94, 0x26, 0xA0, 0x62);
    for (i = 0; i < 4; i++) {
        lv = 0;
        c = g_gameState->unk1C->party[i];
        d = (AbilityMenu *)((u8 *)&D_801FC840 + i);
        if (c != NULL) {
            lv = c->stones;
        }
        d->unk25C[0] = lv;
    }
    func_80063118(&D_801FC840.unk23C, 0, 1);
    func_801F3278(g_menuScreen.unk3, 0);
    ui_initMemberSwitch(&D_801FC840.unk24C, g_menuScreen.unk3, func_801F3264);
}

void func_801F12C8(void) {
    u8 slot;
    s32 next;

    if (g_padRepeat & BTN_CIRCLE) {
        snd_playSe(0x65);
        menuScreen_open(0);
    } else if (g_padRepeat & BTN_SQUARE) {
        snd_playSe(0x67);
        menuScreen_switch(2);
    } else if (g_padRepeat & PAD_LEFT) {
        snd_playSe(0x67);
        D_801FC840.unk0 = D_801FC840.unk0 != 0 ? D_801FC840.unk0 - 1 : 2;
    } else if (g_padRepeat & PAD_RIGHT) {
        snd_playSe(0x67);
        D_801FC840.unk0 = D_801FC840.unk0 >= 2 ? 0 : D_801FC840.unk0 + 1;
    } else if (g_padRepeat & BTN_R1) {
        slot = D_801FC840.unk3;
        next = func_8003380C(slot);
        if (slot != next) {
            func_801F3278(next, 2);
            snd_playSe(0x417);
        }
    } else if (g_padRepeat & BTN_L1) {
        slot = D_801FC840.unk3;
        next = func_800337D0(slot);
        if (slot != next) {
            func_801F3278(next, 1);
            snd_playSe(0x417);
        }
    } else if (g_padRepeat & BTN_CROSS) {
        if (D_801FC840.unk0 == 1) {
            func_801F24EC();
        } else {
            snd_playSe(0x67);
            D_801FC840.unk1 = 2;
        }
    }
}

void func_801F1454(void) {
    u8 slot;
    s32 next;

    if (g_padRepeat & BTN_CIRCLE) {
        snd_playSe(0x65);
        D_801FC840.unk1 = 1;
    } else if (g_padRepeat & PAD_UP) {
        snd_playSe(0x67);
        D_801FC840.unk2 = D_801FC840.unk2 == 0 ? 4 : D_801FC840.unk2 - 1;
    } else if (g_padRepeat & PAD_DOWN) {
        snd_playSe(0x67);
        D_801FC840.unk2 = D_801FC840.unk2 >= 4 ? 0 : D_801FC840.unk2 + 1;
    } else if (g_padRepeat & BTN_R1) {
        slot = D_801FC840.unk3;
        next = func_8003380C(slot);
        if (slot != next) {
            snd_playSe(0x417);
            func_801F3278(next, 2);
        }
    } else if (g_padRepeat & BTN_L1) {
        slot = D_801FC840.unk3;
        next = func_800337D0(slot);
        if (slot != next) {
            snd_playSe(0x417);
            func_801F3278(next, 1);
        }
    } else if (g_padRepeat & BTN_CROSS) {
        if (D_801FC840.unk0 == 0) {
            snd_playSe(0x67);
            func_801F2894();
            func_801F301C();
        } else {
            func_801F31B0();
        }
    }
}

void func_801F15E0(void) {
    Unk80032E84In in;
    Unk80032E84Out cur;
    Unk80032E84Out out;
    RECT *r;
    Character *c;

    r = &D_801FC840.unk1C;
    c = g_gameState->unk1C->party[D_801FC840.unk3];
    ui_drawEtchedLine(r->x + 3, r->y + 0x3B, 0x1E, r->w - 6);
    ui_drawLabel(r->x + 6, r->y - 3, 0x1E, D_80070460, 0);
    win_drawBox(r, 0x1E, 1);
    ui_drawMemberHeader(r->x + 8, r->y + 9, 0x1D, (Unk80024428 *)&D_801FC840.unk24C);
    cur.unk8[0] = c->speed;
    cur.unk8[1] = c->strength;
    cur.unk8[2] = c->magic;
    cur.unk8[3] = c->spirit;
    cur.unkC[0] = D_8006E0E8[c->equip[0]].unk5;
    cur.unkC[1] = c->defense;
    cur.unkC[2] = c->evade;
    cur.unkC[3] = c->magicDef;
    cur.unkC[4] = c->magicEva;
    if (D_801FC840.unk1 == 3 && D_801FC840.unk4.rows != 0) {
        in.unk0 = *(Bytes8 *)&c->unk2C;
        in.unk10 = c->hp;
        in.unk12 = c->mp;
        in.unk14 = *(Bytes5 *)c->equip;
        in.unk8 = *(Words2 *)c->supportAbilities;
        in.unk14.unk0[D_801FC840.unk2] = (D_801FC840.unk3C + (D_801FC840.unk4.cursor + D_801FC840.unk4.top))->id;
        chr_calcEquipStats(&in, &out);
        func_801F20A0(&cur, &out);
        return;
    } else if (*(u16 *)&D_801FC840 == 0x202 && D_801FC840.unk2 != 0) {
        in.unk0 = *(Bytes8 *)&c->unk2C;
        in.unk10 = c->hp;
        in.unk12 = c->mp;
        in.unk14 = *(Bytes5 *)c->equip;
        in.unk8 = *(Words2 *)c->supportAbilities;
        in.unk14.unk0[D_801FC840.unk2] = 0xFF;
        chr_calcEquipStats(&in, &out);
        func_801F20A0(&cur, &out);
        return;
    }
    func_801F20A0(&cur, NULL);
}

void func_801F189C(void) {
    Character *c;
    RECT *r;
    s32 i;
    s32 grey;
    u8 sel;
    s32 y8;
    s32 y12;
    s32 y10;
    Unk80069018 *it;

    r = &D_801FC840.unk24;
    c = g_gameState->unk1C->party[D_801FC840.unk3];
    if (D_801FC840.unk1 == 3) {
        return;
    }
    ui_drawLabel(r->x + 6, r->y - 3, 0x1E, D_8007043C, 0);
    win_drawBox(r, 0x1E, 2);
    win_drawShadow(r, 0x1E, 2);
    for (i = 0, grey = 2, y8 = 8, y12 = 12, y10 = 10; i < 5; i++) {
        sel = 0;
        if (D_801FC840.unk0 == grey) {
            sel = (i == 0);
        }
        ui_drawSprite(r->x + 10, r->y + y10, 0x1D, i + 0xF9, sel);
        ui_drawSmallDigits(r->x + 0x19, r->y + y12, 0x1D, D_801EE644, sel ? grey : 0);
        if (c->equip[i] != 0xFF) {
            it = &D_80069018[c->equip[i]];
            ui_drawItemIcon(r->x + 0x21, r->y + y8, 0x1D, c->equip[i], !sel);
            text_drawBlock(r->x + 0x31, r->y + y8, 0x1D, D_8006A218 + it->unk0, sel ? grey : 0);
        }
        y8 += 17;
        y12 += 17;
        y10 += 17;
    }
}

void func_801F1A94(void) {
    RECT *r;
    Character *c;
    u8 *name;
    s32 id;
    s32 ab;
    s32 i;
    s32 y1;
    s32 y2;
    s32 y3;
    s32 k;
    s32 icon;
    s32 ap;
    u8 ok;
    u8 flag;

    r = &D_801FC840.unk2C;
    c = g_gameState->unk1C->party[D_801FC840.unk3];
    ui_drawLabel(r->x + 6, r->y - 3, 0x13, D_80070400, 0);
    ui_drawEtchedLine(r->x + 3, r->y + 0x1A, 0x1E, r->w - 6);
    win_drawBox(r, 0x1E, 1);
    if (D_801FC840.unk1 < 2) {
        return;
    }
    ui_drawSprite(r->x + 0xA, r->y + 9, 0x1D, D_801FC840.unk2 + 0xF9, 0);
    ui_drawSmallDigits(r->x + 0x19, r->y + 0xB, 0x1D, D_801EE644, 0);
    if (D_801FC840.unk1 == 2) {
        id = c->equip[D_801FC840.unk2];
    } else {
        id = D_801FC840.unk3C[D_801FC840.unk4.top + D_801FC840.unk4.cursor].id;
    }
    if (id == 0xFF) {
        return;
    }
    ui_drawItemIcon(r->x + 0x21, r->y + 7, 0x1D, id, 1);
    text_drawBlock(r->x + 0x31, r->y + 7, 0x1D, D_8006A218 + D_80069018[id].unk0, 0);
    for (i = 0; i < 3; i++) {
        y3 = i * 15;
        y2 = i * 15 + 0x21;
        y1 = i * 15 + 0x22;
        ab = D_80069018[id].unkC[i];
        if (ab == 0) {
            continue;
        }
        if (ab < 0xC0) {
            name = D_801F803C + D_801F743C[ab].unkE;
        } else {
            name = D_801F8684 + D_8006D128[ab - 0xC0].unk2;
        }
        if (func_80034A30(c)) {
            ok = func_80034430(c->unk34.b.id, ab) >= 0;
            ap = func_800347D8(c->unk34.b.id, ab);
            if (ok) {
                if (ab < 0xC0) {
                    icon = 0xBC;
                } else if (func_80034A04(c->supportAbilities, ab)) {
                    icon = 0xBD;
                } else {
                    icon = 0xF4;
                }
            } else {
                if (ab < 0xC0) {
                    icon = 0xBE;
                } else {
                    icon = 0xBF;
                }
            }
            if (ok) {
                if (ab < 0xC0) {
                    flag = !(D_801F743C[ab].unkB & 2);
                } else {
                    flag = 1;
                }
                k = 0x26;
                ui_drawApGauge(r->x + 0x72, r->y + (y3 + k), 0x1D, func_80034584(c->unk34.b.id, ab), ap, flag, 0);
            }
            text_drawBlock(r->x + 0x18, r->y + y2, 0x1D, name, ok ? 0 : 2);
            ui_drawSprite(r->x + 8, r->y + y1, 0x1D, icon, ok ^ 1);
        } else {
            text_drawBlock(r->x + 0x18, r->y + y2, 0x1D, name, 2);
            ui_drawSprite(r->x + 8, r->y + y1, 0x1D, ab < 0xC0 ? 0xBE : 0xBF, 1);
        }
    }
}

void func_801F1F08(void) {
    RECT sub;
    MenuView view;
    WinDef *wd = &D_801FC840.unk23C;
    RECT *r = &wd->unk4;
    AbilityMenu *m = &D_801FC840;
    MenuList *l = &D_801FC840.unk4;
    s32 w;
    s32 k = 0x97;
    s32 i;

    if (m->unk260 != 0) {
        w = func_8002AE7C((u8 *)D_800703FC.unk94);
        ui_drawLabel(wd->unk4.x + 6, r->y - 3, 0x14, D_800703FC.unk0[D_801F8918[m->unk2]], 0);
        ui_drawLabel(wd->unk4.x - (w - k), r->y - 3, 0x14, D_800703FC.unk94, 0);
        win_drawListBox(r, 0x14, 1, l);
        menuList_getView(l, &view);
        win_setClip(NULL, 0x13, 1);
        for (i = 0; i < view.unk0; i++) {
            func_801F2F3C(0, i, view.unk1 + i, &view);
        }
        if (l->scroll & 4) {
            menuList_getRowsRect(l, r, &sub);
            win_setClip(&sub, 0x13, 1);
            func_801F2ED4();
        }
        if (wd->unkC == 0) {
            win_setClip(r, 0x13, 1);
        }
    }
}

void func_801F20A0(Unk80032E84Out *cur, Unk80032E84Out *new) {
    u8 buf[16];
    RECT *r;
    s32 i;
    s32 y;
    s32 col;
    u8 diff;
    u8 blink;

    r = &D_801FC840.unk1C;
    blink = ((u32)D_80077DC0 >> 4) & 1;
    for (i = 0, y = r->y + 0x44; i < 4; y += 0xC, i++) {
        func_801F3344(r->x + 8, y, 0x1D, D_801F8930[i], 0);
        text_formatNumber(cur->unk8[i], buf, 2, 1);
        ui_drawSmallDigits(r->x + 0x4A, y, 0x1D, buf, 0);
        if (new != NULL) {
            diff = cur->unk8[i] != new->unk8[i];
            if (new->unk8[i] >= cur->unk8[i]) {
                if (diff) {
                    col = 7;
                } else {
                    col = 0;
                }
            } else {
                col = 3;
            }
            text_formatNumber(new->unk8[i], buf, 2, 1);
            ui_drawSmallDigits(r->x + 0x5C, y, 0x1D, D_801F892C, (diff && blink) ? 0 : 2);
            ui_drawSmallDigits(r->x + 0x6D, y, 0x1D, buf, col);
        }
    }
    for (i = 0, y = r->y + 0x78; i < 5; y += 0xC, i++) {
        func_801F3344(r->x + 8, y, 0x1D, D_801F8934[i], 0);
        text_formatNumber(cur->unkC[i], buf, 3, 1);
        ui_drawSmallDigits(r->x + 0x43, y, 0x1D, buf, 0);
        if (new != NULL) {
            diff = cur->unkC[i] != new->unkC[i];
            if (new->unkC[i] >= cur->unkC[i]) {
                if (diff) {
                    col = 7;
                } else {
                    col = 0;
                }
            } else {
                col = 3;
            }
            text_formatNumber(new->unkC[i], buf, 3, 1);
            ui_drawSmallDigits(r->x + 0x5C, y, 0x1D, D_801F892C, (diff && blink) ? 0 : 2);
            ui_drawSmallDigits(r->x + 0x66, y, 0x1D, buf, col);
        }
    }
}

void func_801F239C(void) {
    AbilityMenu *m = &D_801FC840;
    u16 x;
    u16 k;
    HelpState *h;

    help_setCursorOnTop(m->unk1 != 1);
    if (fade_isBusy() || m->unk1 == 0) {
        return;
    }
    if (m->unk1 == 1 || (D_80077E54 & 3)) {
        h = &g_menuHelp;
        x = D_801F893C[D_801FC840.unk0][0];
        h->y = m->unk14.y + 8;
        h->otz = 0x1B;
        h->x = m->unk14.x + x - 0x18;
        help_drawCursor();
    }
    if (D_801FC840.unk1 == 2) {
        g_menuHelp.otz = 0x12;
        g_menuHelp.x = D_801FC840.unk24.x - 0x10;
        k = D_801FC840.unk2 * 17 + 0xC;
        g_menuHelp.y = D_801FC840.unk24.y + k;
        help_drawCursor();
    } else if (D_801FC840.unk1 == 3) {
        g_menuHelp.otz = 0x12;
        g_menuHelp.x = D_801FC840.unk23C.unk4.x - 0xD;
        k = D_801FC840.unk4.cursor * 15 + 0xA;
        g_menuHelp.y = D_801FC840.unk23C.unk4.y + k;
        help_drawCursor();
    }
}

void func_801F24EC(void) {
    s32 best;
    s32 i;
    s32 j;
    s32 max;
    s32 kind;
    s32 cur;
    Character *c;
    UnkPair *p;
    Unk80069018 *it;

    best = 0;
    i = 0;
    c = g_gameState->unk1C->party[D_801FC840.unk3];
    kind = chr_getDefaultSet(c);
    for (; i < 5; i++) {
        if (c->equip[i] != 0xFF) {
            cur = D_80069018[c->equip[i]].unkA;
        } else {
            cur = -1;
        }
        for (j = 0, max = -1, p = g_gameState->unk1C->items.slots; j < 0x100; j++, p++) {
            if (p->unk1 != 0) {
                it = &D_80069018[p->unk0];
                if ((it->unkF & D_801F8948[i]) && (it->unk6 & D_801F8950[kind])) {
                    if (it->unkA > max) {
                        max = it->unkA;
                        best = p->unk0;
                    }
                }
            }
        }
        if (cur < max) {
            if (c->equip[i] != 0xFF) {
                item_add(c->equip[i], 1);
            }
            if (item_remove(best, 1) != 0) {
                c->equip[i] = best;
            }
        }
    }
    func_801F32E0(c);
    snd_playSe(0x6B);
}

void func_801F26C4(Character *c) {
    s32 i;
    s32 j;
    AbilityEntry *a;
    Unk80069018 *it;
    s32 id;

    if (func_80034A30(c)) {
        memset(D_801FC700, 0, 0x40);
        for (i = 0; i < 5; i++) {
            if (c->equip[i] != 0xFF) {
                it = &D_80069018[c->equip[i]];
                for (j = 0; j < 3; j++) {
                    id = it->unkC[j];
                    if (id != 0) {
                        if (id >= 0xC0) {
                            D_801FC700[id - 0xC0] = 1;
                        }
                    }
                }
            }
        }
        a = D_8006CB28[c->unk34.b.row];
        for (i = 0; i < 0x30; i++, a++) {
            if (a->unk0 >= 0xC0 && func_800349BC(c->unk34.b.id, a->unk0)) {
                if (D_801FC700[a->unk0 - 0xC0] == 0 && c->unk58[i] < a->unk1) {
                    chr_setSupportAbility(c->unk34.b.id, a->unk0, 0);
                    if (c->maxStones - c->stones >= D_8006D128[a->unk0 - 0xC0].unk1) {
                        c->stones += D_8006D128[a->unk0 - 0xC0].unk1;
                    } else {
                        c->stones = c->maxStones;
                    }
                }
            }
        }
    }
}

void func_801F2894(void) {
    ItemSlot tmp;
    Unk80069018 *it;
    s32 n;
    s32 i;
    s32 j;
    u16 mask;
    u8 slot;
    u8 a;
    u8 b;
    u8 ra;
    u8 rb;

    n = 0;
    mask = D_801F8968[chr_getDefaultSet(g_gameState->unk1C->party[D_801FC840.unk3])];
    slot = D_801F8980[D_801FC840.unk2];
    D_801FC840.unk3C[0].id = 0xFF;
    for (i = 0; i < 0x100; i++) {

        if (g_gameState->unk1C->items.slots[i].count != 0) {
            it = &D_80069018[g_gameState->unk1C->items.slots[i].id];
            if ((it->unk6 & mask) && (it->unkF & slot)) {
                D_801FC840.unk3C[n] = g_gameState->unk1C->items.slots[i];
                n++;
            }
        }
    }
    for (i = 0; i < n - 1; i++) {
        for (j = i + 1; j < n; j++) {
            a = D_801FC840.unk3C[i].id;
            b = D_801FC840.unk3C[j].id;
            ra = D_80069018[a].unkA;
            rb = D_80069018[b].unkA;
            if ((ra == rb && b < a) || rb < ra) {
                tmp = D_801FC840.unk3C[i];
                D_801FC840.unk3C[i] = D_801FC840.unk3C[j];
                D_801FC840.unk3C[j] = tmp;
            }
        }
    }
    menuList_init(&D_801FC840.unk4, n, 6, 1, 0x62);
}

void func_801F2AB4(void) {
    Character *c;
    u8 *buf;
    s32 id;
    UnkPair *e;
    MenuList *l;

    c = g_gameState->unk1C->party[D_801FC840.unk3];
    g_menuHelp.text = NULL;
    if (D_801FC840.unk1 == 1) {
        buf = D_801FC740;
        g_menuHelp.text = buf;
        func_8002BE44(0, D_801F8988);
        func_8002BD54(g_menuHelp.text, text_get(D_801FC840.unk0 + 3));
        text_append(g_menuHelp.text, text_get(7));
    } else if (D_801FC840.unk1 == 2) {
        id = c->equip[D_801FC840.unk2];
        if (id == 0xFF) {
            g_menuHelp.text = text_get(6);
        } else {
            g_menuHelp.text = D_801F8990 + D_80069018[id].unk2;
        }
    } else if (D_801FC840.unk1 == 3 && (l = &D_801FC840.unk4)->rows != 0) {
        e = D_801FC840.unk3C + (l->top + l->cursor);
        if (e->unk1 != 0) {
            g_menuHelp.text = D_801F8990 + D_80069018[e->unk0].unk2;
        }
    }
}

void func_801F2C0C(void) {
    func_801F2C78();
    func_801F2CD0();
    func_801F2D18();
}

void func_801F2C3C(void) {
    D_80077E53 = D_801FC840.unk3;
    win_closeAllNow();
    help_setCursorOnTop(1);
    help_setCallback(NULL);
}

void func_801F2C78(void) {
    void (*fn)(void);

    if (!fade_isBusy()) {
        fn = D_801F7420[D_801FC840.unk1];
        if (fn != NULL) {
            fn();
        }
    }
}

void func_801F2CD0(void) {
    func_801F2E34();
    func_801F15E0();
    func_801F189C();
    func_801F1A94();
    func_801F1F08();
    func_801F239C();
}

void func_801F2D18(void) {
    if (help_isOpen()) {
        func_801F2AB4();
    }
    if (D_801FC840.unk1 == 3 && D_801FC840.unk4.scroll != 0) {
        menuList_update(&D_801FC840.unk4);
    }
    if (D_801FC840.unk23C.unkC != 0) {
        func_800631AC(&D_801FC840.unk23C);
    }
    if (D_801FC840.unk24C.unk0 != 0) {
        ui_updateMemberSwitch(&D_801FC840.unk24C);
    }
}

void func_801F2DB4(void) {
    if (D_801FC840.unk4.scroll == 0) {
        if (g_padRepeat & BTN_CIRCLE) {
            snd_playSe(0x65);
            func_801F307C();
            return;
        }
        if (g_padRepeat & BTN_CROSS) {
            func_801F30D8();
            return;
        }
        menuList_input(&D_801FC840.unk4, g_padRepeat);
    }
}

void func_801F2E34(void) {
    RECT *r;
    s32 i;

    r = &D_801FC840.unk14;
    win_drawBox(r, 0x1E, 2);
    win_drawShadow(r, 0x1E, 2);
    for (i = 0; i < 3; i++) {
        text_drawBlock(r->x + D_801F7430[i], r->y + 6, 0x1D, text_get(i), 0);
    }
}

void func_801F2ED4(void) {
    MenuView v;
    s32 i;

    menuList_getPageView(&D_801FC840.unk4, &v);
    for (i = 0; i < v.unk0; i++) {
        func_801F2F3C(0, i, v.unk1 + i, &v);
    }
}

void func_801F2F3C(s32 col, s32 row, s32 i, MenuView *v) {
    WinDef *w;
    UnkPair *p;
    s32 x;
    s32 y;
    u8 buf[16];

    p = D_801FC840.unk3C + i;
    x = D_801FC840.unk23C.unk4.x;
    w = &D_801FC840.unk23C;
    y = w->unk4.y + row * 0xF + v->unk2;
    ui_drawItemIcon(x + 0xB, y, 0x13, p->unk0, 1);
    text_drawBlock(x + 0x1B, y, 0x13, D_8006A218 + D_80069018[p->unk0].unk0, 0);
    text_formatNumber(p->unk1, buf, 2, 1);
    ui_drawSmallDigits(x + 0x8A, y + 4, 0x13, buf, 0);
}

void func_801F301C(void) {
    WinDef *w = &D_801FC840.unk23C;

    w->unk4 = D_801FC840.unk34;
    D_801FC840.unk260 = 1;
    D_801FC840.unk1 = 0;
    func_80063118(w, D_801FBD54, 1);
}

void func_801F307C(void) {
    WinDef *w = &D_801FC840.unk23C;

    D_801FC840.unk1 = 0;
    func_80063118(w, D_801FBD74, 0);
}

void func_801F30B0(void) {
    D_801FC840.unk1 = 3;
}

void func_801F30C0(void) {
    D_801FC840.unk1 = 2;
    D_801FC840.unk260 = 0;
}

void func_801F30D8(void) {
    GameState *gs;
    AbilityMenu *m;
    MenuList *l;
    Character *c;
    s32 old;
    s32 item;

    gs = g_gameState;
    m = &D_801FC840;
    l = &m->unk4;
    c = gs->unk1C->party[m->unk3];
    old = c->equip[m->unk2];
    item = m->unk3C[l->top + l->cursor].id;
    if (l->rows == 0) {
        snd_playSe(0x66);
        return;
    }
    if (old != 0xFF) {
        item_add(old, 1);
    }
    if (item_remove(item, 1) != 0) {
        c->equip[m->unk2] = item;
        func_801F32E0(c);
        func_801F307C();
    }
    snd_playSe(0x6B);
}

void func_801F31B0(void) {
    s32 se;
    Character *c;
    u8 ok;
    u8 old;

    ok = 0;
    if (D_801FC840.unk2 != 0) {
        c = g_gameState->unk1C->party[D_801FC840.unk3];
        old = c->equip[D_801FC840.unk2];
        if (old != 0xFF) {
            item_add(old, 1);
            c->equip[D_801FC840.unk2] = 0xFF;
            func_801F32E0(c);
            ok = 1;
        }
    }
    se = 0x6B;
    if (!ok) {
        se = 0x66;
    }
    snd_playSe(se);
}

void func_801F3264(void) {
    D_801FC840.unk1 = D_801FC840.unk261;
}

void func_801F3278(s32 slot, s32 dir) {
    u8 s;

    ui_startMemberSwitch(&D_801FC840.unk24C, D_801FC840.unk3, slot, dir);
    if (dir != 0) {
        s = D_801FC840.unk1;
        D_801FC840.unk1 = 0;
        D_801FC840.unk261 = s;
    }
    D_801FC840.unk3 = slot;
}

void func_801F32E0(Character *c) {
    func_801F26C4(c);
    chr_updateStats(c);
    c->unk34.b.unk4 = func_80033280(c->unk34.b.id, (c->unkA >> 4) & 1, c->equip);
}

void func_801F3344(s32 x, s32 y, s32 z, u8 id, s32 color) {
    text_drawBlock(x, y - 4, z, text_get(id), color);
}

INCLUDE_RODATA("asm/us/ovl_33/nonmatchings/tu_801F1150", D_801EE644);

INCLUDE_RODATA("asm/us/ovl_33/nonmatchings/tu_801F1150", D_801EE648);
