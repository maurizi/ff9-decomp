#include "ovl_33.h"

/* Inlined copy of func_801F0CC4 (support ability state). */
static inline s32 func_801F0CC4_inline(s32 id, Character *c) {
    Unk8006D128 *sup;
    s32 slot;
    s32 ap;
    s32 need;

    if (id < 0xC0) {
        return 0;
    }
    if (func_80034A04(c->supportAbilities, id)) {
        return 4;
    }
    if (D_801FBF84.unk68[id] == 0) {
        slot = func_80034430(c->unk34.b.id, id);
        if (slot < 0) {
            return 0;
        }
        if (func_80034A30(c)) {
            ap = c->unk58[slot];
            need = D_8006CB28[c->unk34.b.row][slot].unk1;
            if (ap == 0) {
                return 0;
            }
            if (ap < need) {
                return 0;
            }
        }
    }
    sup = &D_8006D128[id - 0xC0];
    if (sup->unk1 > c->stones) {
        return 2;
    }
    return 3;
}

void func_801EE7C0(void) {
    s32 i;

    D_801FBF84.unk2 = 0xFF;
    D_801FBF84.unk1 = 1;
    D_801FBF84.unk0 = 0;
    ui_initMemberSwitch(&D_801FBF84.unk770, g_menuScreen.unk3, func_801F0ED4);
    func_801EF900(g_menuScreen.unk3, 0);
    D_801FBF84.unk76C = 0;
    D_801FBF84.unk76B = 0;
    text_setBank(D_801EDF04);
    func_801F1148();
    help_setCallback(help_nullHandler);
    help_setCursorOnTop(1);
    setRECT(&D_801FBF84.unk28, 8, 8, 0xE4, 0x1A);
    setRECT(&D_801FBF84.unk30, 8, 0x26, 0xD0, 0x38);
    setRECT(&D_801FBF84.unk38, 0xD8, 0x26, 0x5C, 0x38);
    setRECT(&D_801FBF84.unk40, 0x8E, 0x40, 0x46, 0x1A);
    setRECT(&D_801FBF84.unk48, 8, 0x5E, 0x12C, 0x80);
    setRECT(&D_801FBF84.unk50, 0, 0x26, 0x90, 0xB6);
    D_801FBF84.unk768 = 0;
    func_80063118(&D_801FBF84.unk58, 0, 0);
    func_801F0EE4();
    D_801FBF84.unk76E = 0;
    for (i = 0; i < 4; i++) {
        if (g_gameState->unk1C->party[i] != NULL) {
            D_801FBF84.unk76E++;
        }
    }
}

void func_801EE978(void) {
    EquipMenu *m;

    if (help_isOpen()) {
        func_801F0024();
    }
    m = &D_801FBF84;
    if (m->unk1 == 2) {
        func_8002EB74(&D_801FBF84.unk8[D_801FBF84.unk0]);
    }
    if (m->unk58.unkC != 0) {
        func_800631AC(&m->unk58);
    }
    if (++m->unk76B >= 7) {
        m->unk76B = 0;
        if (++m->unk76C >= 7) {
            m->unk76C = 0;
        }
    }
    if (D_801FBF84.unk770.unk0 != 0) {
        ui_updateMemberSwitch(&D_801FBF84.unk770);
    }
}

void func_801EEA50(void) {
    u8 slot;
    s32 next;

    if (g_padRepeat & BTN_CIRCLE) {
        snd_playSe(0x65);
        menuScreen_open(0);
    } else if (g_padRepeat & BTN_SQUARE) {
        snd_playSe(0x67);
        menuScreen_switch(3);
    } else if (g_padRepeat & PAD_LEFT) {
        snd_playSe(0x67);
        D_801FBF84.unk0 = D_801FBF84.unk0 != 0 ? D_801FBF84.unk0 - 1 : 1;
    } else if (g_padRepeat & PAD_RIGHT) {
        snd_playSe(0x67);
        D_801FBF84.unk0 = D_801FBF84.unk0 == 0 ? D_801FBF84.unk0 + 1 : 0;
    } else if (g_padRepeat & BTN_R1) {
        slot = D_801FBF84.unk2;
        next = func_8003380C(slot);
        if (slot != next) {
            func_801EF900(next, 2);
            snd_playSe(0x417);
        }
    } else if (g_padRepeat & BTN_L1) {
        slot = D_801FBF84.unk2;
        next = func_800337D0(slot);
        if (slot != next) {
            func_801EF900(next, 1);
            snd_playSe(0x417);
        }
    } else if (g_padRepeat & BTN_CROSS) {
        if (D_801FBF84.unk4[D_801FBF84.unk0] != 0) {
            snd_playSe(0x67);
            D_801FBF84.unk1 = 2;
        } else {
            snd_playSe(0x66);
        }
    }
}

void func_801EEBE0(void) {
    s32 cur;
    s32 next;

    if (g_padRepeat & BTN_CIRCLE) {
        snd_playSe(0x65);
        func_801F0DDC();
        return;
    }
    if (g_padRepeat & PAD_UP) {
        if (!(D_801FBF84.unk3 & 0x80)) {
            cur = D_801FBF84.unk3;
            next = func_800337D0(cur);
            if (cur != next) {
                D_801FBF84.unk3 = next;
                snd_playSe(0x67);
            }
        }
    } else if (g_padRepeat & PAD_DOWN) {
        if (!(D_801FBF84.unk3 & 0x80)) {
            cur = D_801FBF84.unk3;
            next = func_8003380C(cur);
            if (cur != next) {
                D_801FBF84.unk3 = next;
                snd_playSe(0x67);
            }
        }
    } else if (g_padRepeat & (PAD_RIGHT | PAD_LEFT | BTN_L1 | BTN_R1)) {
        if (D_801FBF84.unk76F != 0) {
            snd_playSe(0x67);
            D_801FBF84.unk3 ^= 0x80;
        }
    } else if (g_padRepeat & BTN_CROSS) {
        func_801EED10();
    }
}

void func_801EED10(void) {
    u8 ok;
    MenuList *l;
    Character *c;
    Unk801F34EC *it;
    s32 i;
    s32 cost;

    ok = 0;
    c = g_gameState->unk1C->party[D_801FBF84.unk2];
    l = &D_801FBF84.unk8[D_801FBF84.unk0];
    it = &D_801F34EC[D_801FBF84.unk668[(l->unk5 + l->cursor) * 2 + l->unk7].unk0];
    if (!(D_801FBF84.unk3 >> 7)) {
        ok = item_useOnMember(c, g_gameState->unk1C->party[D_801FBF84.unk3], it, it->unk4, 0);
    } else {
        for (i = 0; i < 4; i++) {
            if (g_gameState->unk1C->party[i] != NULL) {
                ok |= item_useOnMember(c, g_gameState->unk1C->party[i], it, it->unk4, 1);
            }
        }
    }
    if (ok) {
        snd_playSe(0x6A);
        cost = func_801F1018(it);
        c->mp -= cost;
        if (c->mp < cost) {
            func_801F0DDC();
        }
    } else {
        snd_playSe(0x66);
    }
}

void func_801EEEA8(void) {
    RECT box;
    RECT *r;
    Character *c;

    r = &D_801FBF84.unk30;
    c = g_gameState->unk1C->party[D_801FBF84.unk2];
    ui_drawMemberHeader(r->x + 8, r->y + 5, 0x1D, (Unk80024428 *)&D_801FBF84.unk770);
    box.w = 0x5B;
    box.h = 0x12;
    box.x = r->x + 0x72;
    box.y = r->y + 4;
    ui_drawStatusIcons(box.x + 3, box.y + 4, 0x1D, c->status, 0);
    win_drawBox(&box, 0x1D, 0);
    if (D_801FBF84.unk0 == 1) {
        ui_drawStones(box.x + 0x19, box.y + 0x14, 0x1D, c->stones, c->maxStones);
    }
    ui_drawLabel(r->x + 6, r->y - 3, 0x1D, D_80070498, 0);
    win_drawBox(r, 0x1D, 1);
}

void func_801EEFDC(void) {
    RECT *r;
    MenuList *l;
    Character *c;
    s32 n;
    s32 id;
    s32 ok;
    u8 flag;
    s32 i;
    s32 x;

    r = &D_801FBF84.unk38;
    c = g_gameState->unk1C->party[D_801FBF84.unk2];
    l = &D_801FBF84.unk8[D_801FBF84.unk0];
    n = (l->unk5 + l->cursor) * 2 + l->unk7;
    ui_drawLabel(r->x + 6, r->y - 3, 0x1D, D_80070460, 0);
    win_drawBox(r, 0x1D, 2);
    text_drawBlock(r->x + 8, r->y + 8, 0x1C, text_get(0x10), 0);
    text_drawBlock(r->x + 8, r->y + 0x16, 0x1C, text_get(0x11), 0);
    if (D_801FBF84.unk1 == 1 || D_801FBF84.unk1 == 5 || D_801FBF84.unk1 == 0 || l->unk0 == 0) {
        return;
    }
    if (D_801FBF84.unk0 == 0) {
        id = D_801FBF84.unk668[n].unk0;
        ok = func_801EFB8C(id, c);
        flag = !(D_801F34EC[id].unkB & 2);
    } else {
        id = D_801FBF84.unk728[n];
        ok = func_801F0CC4(id, c);
        flag = 1;
    }
    if (ok == 0) {
        return;
    }
    if (func_80034A30(c)) {
        ui_drawApGauge(r->x + 0x2D, r->y + 0xC, 0x1C, func_80034584(c->unk34.b.id, id), func_800347D8(c->unk34.b.id, id), flag, 0);
    }
    if (D_801FBF84.unk68[id] != 0) {
        x = r->x;
        for (i = 0; i < 5; i++) {
            if ((D_801FBF84.unk68[id] >> i) & 1) {
                ui_drawItemIcon(x + 8, r->y + 0x24, 0x1C, D_801FBF84.unk168[id][i], 1);
                x += 0x10;
            }
        }
    }
}

void func_801EF270(void) {
    RECT *r;
    EquipMenu *m;
    MenuList *l;
    Character *c;
    s32 idx;

    r = &D_801FBF84.unk40;
    m = &D_801FBF84;
    l = &D_801FBF84.unk8[D_801FBF84.unk0];
    idx = (l->cursor + l->unk5) * 2 + l->unk7;
    c = g_gameState->unk1C->party[m->unk2];
    if (m->unk0 != 1) {
        if (m->unk1 != 1 && m->unk1 != 5 && m->unk1 != 0) {
            if (func_801EFB8C(m->unk668[idx].unk0, c) != 0) {
                text_drawBlock(r->x + 8, r->y + 6, 0x1B, D_801F722C + D_801F6FEC[m->unk668[idx].unk1].unk0, 0);
            }
        }
        ui_drawLabel(r->x + 6, r->y - 3, 0x1B, D_8007042C, 0);
        win_drawBox(r, 0x1B, 2);
    }
}

void func_801EF3A8(void) {
    RECT sub;
    MenuView view;
    s32 row;
    s32 col;
    s32 idx;
    MenuList *l;
    RECT *r = &D_801FBF84.unk48;

    l = &D_801FBF84.unk8[D_801FBF84.unk0];
    ui_drawLabel(r->x + 0xE, r->y - 3, 0x1C, D_800703FC.unk4, 0);
    ui_drawLabel(r->x + 0xA4, r->y - 3, 0x1C, D_800703FC.unk4, 0);
    ui_drawLabel(r->x + 0x7C, r->y - 3, 0x1C, D_800703FC.unk7C, 0);
    ui_drawLabel(r->x + 0x112, r->y - 3, 0x1C, D_800703FC.unk7C, 0);
    win_drawListBox(r, 0x1D, 1, l);
    if (l->unk0 != 0) {
        menuList_getView(l, &view);
        win_setClip(NULL, 0x1C, 1);
        idx = view.unk1 * 2;
        for (row = 0; row < view.unk0; row++) {
            for (col = 0; col < 2; col++) {
                func_801F0934(col, row, idx++, &view);
            }
        }
        if (l->unk8 & 4) {
            func_8002F40C(l, r, &sub);
            win_setClip(&sub, 0x1C, 1);
            func_801F0890();
        }
        win_setClip(r, 0x1C, 1);
    }
}

void func_801EF560(void) {
    RECT *r = &D_801FBF84.unk58.unk4;
    EquipMenu *m = &D_801FBF84;
    s32 i;

    if (m->unk768 != 0) {
        ui_drawLabel(r->x + 8, r->y - 3, 0x14, D_800704B8, 0);
        win_drawBox(r, 0x14, 2);
        for (i = 0; i < 4; i++) {
            if (g_gameState->unk1C->party[i] != NULL) {
                ui_drawMemberSummary(r->x + 8, r->y + 6 + i * 0x2B, 0x13, i, D_801FBF84.unk76D);
            }
        }
    }
}

void func_801EF644(void) {
    RECT *r;
    MenuList *l;
    HelpState *h3;
    s32 i;
    s32 y;
    u16 k;
    u8 blink;
    u8 done;
    s16 ry;

    done = 0;
    blink = (D_80077E54 & 3) != 0;
    help_setCursorOnTop(D_801FBF84.unk1 != 1);
    if (fade_isBusy()) {
        return;
    }
    if (D_801FBF84.unk1 == 1 || D_801FBF84.unk1 == 5 || blink) {
        HelpState *h1;
        r = &D_801FBF84.unk28;
        h1 = &g_menuHelp;
        k = D_801F33C8[D_801FBF84.unk0][0] - 0x18;
        g_menuHelp.x = r->x + k;
        ry = r->y + 8;
        h1->otz = 0x17;
        h1->y = ry;
        help_drawCursor();
    }
    if (D_801FBF84.unk1 == 2 || (D_801FBF84.unk1 == 3 && blink)) {
        HelpState *h2;
        l = &D_801FBF84.unk8[D_801FBF84.unk0];
        h2 = &g_menuHelp;
        k = l->unk7;
        k ^= (l->unk8 >> 3) & 1;
        k = k * 150 - 10;
        g_menuHelp.x = D_801FBF84.unk48.x + k;
        k = l->cursor * 15 + 10;
        h2->otz = 0x17;
        h2->y = D_801FBF84.unk48.y + k;
        help_drawCursor();
    }
    if (D_801FBF84.unk1 == 3) {
        r = &D_801FBF84.unk58.unk4;
        if (D_801FBF84.unk3 & 0x80) {
            g_menuHelp.otz = 0x12;
            if (blink) {
                for (i = 0; i < 4; i++) {
                    y = i * 0x2B + 0x12;
                    if (g_gameState->unk1C->party[i] != NULL) {
                        ui_drawHand(r->x - 10, r->y + y, g_menuHelp.otz);
                        if (!done) {
                            g_menuHelp.x = r->x - 10;
                            done = 1;
                            g_menuHelp.y = r->y + y;
                        }
                    }
                }
            }
        } else {
            g_menuHelp.x = D_801FBF84.unk58.unk4.x - 10;
            h3 = &g_menuHelp;
            k = D_801FBF84.unk3 * 0x2B + 0x12;
            ry = r->y;
            h3->otz = 0x12;
            h3->y = ry + k;
            help_drawCursor();
        }
    }
}

void func_801EF900(s32 slot, s32 arg1) {
    EquipMenu *m;
    EquipMenu *m2;
    UnkBlink *b;
    Character *c;
    AbilityEntry *a;
    Unk80069018 *it;
    s32 j;
    s32 nItems;
    s32 nAbils;
    s32 id;

    b = &D_801FBF84.unk770;
    c = g_gameState->unk1C->party[slot];
    m = &D_801FBF84;
    nItems = 0;
    nAbils = 0;
    ui_startMemberSwitch(b, m->unk2, slot, arg1);
    if (arg1 != 0) {
        m->unk1 = 5;
    }
    m->unk2 = slot;
    if (c->unk34.b.row < 16) {
        m2 = m;
        slot = 0;
        a = D_8006CB28[c->unk34.b.row];
        for (; slot < 0x30; slot++) {
            if (a->unk0 != 0) {
                if (a->unk0 < 0xC0) {
                    D_801FBF84.unk668[nItems].unk0 = a->unk0;
                    D_801FBF84.unk668[nItems].unk1 = func_801F0C10(a->unk0, c);
                    nItems++;
                } else {
                    *(nAbils + m2->unk728) = a->unk0;
                    nAbils++;
                }
            }
            a++;
        }
    }
    func_8002EDFC(&D_801FBF84.unk8[0], nItems, 8, 2, 0x80);
    func_8002EDFC(&D_801FBF84.unk8[1], nAbils, 8, 2, 0x80);
    D_801FBF84.unk769 = nItems;
    D_801FBF84.unk76A = nAbils;
    memset(D_801FBF84.unk68, 0, 0x100);
    for (slot = 0; slot < 5; slot++) {
        id = c->equip[slot];
        if (id != 0xFF) {
            it = &D_80069018[id];
            for (j = 0; j < 3; j++) {
                D_801FBF84.unk68[it->unkC[j]] |= 1 << slot;
                D_801FBF84.unk168[it->unkC[j]][slot] = id;
            }
        }
    }
    D_801FBF84.unk4[0] = c->hp != 0 && !(c->status & 9) && nItems != 0 && D_8007AFD3 == 0;
    D_801FBF84.unk4[1] = func_80034A30(c);
    func_801F1048();
    func_801F0410();
}

s32 func_801EFB8C(s32 id, Character *c) {
    Unk801F34EC *info;
    s32 slot;
    s32 ap;
    s32 need;

    info = &D_801F34EC[id];
    if (D_801FBF84.unk68[id] == 0) {
        slot = func_80034430(c->unk34.b.id, id);
        if (slot < 0) {
            return 0;
        }
        if (func_80034A30(c)) {
            ap = c->unk58[slot];
            need = D_8006CB28[c->unk34.b.row][slot].unk1;
            if (ap == 0) {
                return 0;
            }
            if (ap < need) {
                return 0;
            }
        }
    }
    if (c->status & 9) {
        return 2;
    }
    if (!(info->unkB & 1)) {
        return 2;
    }
    if (func_801F1018(info) > c->mp) {
        return 2;
    }
    return 3;
}

void func_801EFCB4(void) {
    MenuList *l = D_801FBF84.unk8 + D_801FBF84.unk0;
    MenuList *ls;
    Character *c = g_gameState->unk1C->party[D_801FBF84.unk2];
    s32 i;
    u8 id;

    if (l->unk2 != 0) {
        i = (l->unk5 + l->cursor) * 2 + l->unk7;
        if (i < D_801FBF84.unk769) {
            id = D_801FBF84.unk668[i].unk0;
            if (func_801EFB8C(id, c) == 3) {
                D_801FBF84.unk76D = D_801F34EC[id].unk0_5;
                D_801FBF84.unk76F = func_801F0E64(id);
                ls = D_801FBF84.unk8 + D_801FBF84.unk0;
                D_801FBF84.unk58.unk4 = D_801FBF84.unk50;
                if (ls->unk7 == 0) {
                    D_801FBF84.unk58.unk4.x = 0xA4;
                    func_80063118(&D_801FBF84.unk58, D_801F3428, 1);
                } else {
                    D_801FBF84.unk58.unk4.x = 8;
                    func_80063118(&D_801FBF84.unk58, D_801F3418, 1);
                }
                D_801FBF84.unk768 = 1;
                D_801FBF84.unk3 = party_getFirstMember();
                D_801FBF84.unk1 = 4;
                snd_playSe(0x67);
            } else {
                snd_playSe(0x66);
            }
        }
    }
}

void func_801EFE2C(void) {
    MenuList *l;
    Character *c;
    s32 idx;
    s32 id;
    u8 ap;
    Unk8006D128 *sup;
    s32 st;

    l = &D_801FBF84.unk8[D_801FBF84.unk0];
    c = g_gameState->unk1C->party[D_801FBF84.unk2];
    if (l->unk2 != 0 && (idx = (l->unk5 + l->cursor) * 2 + l->unk7) < D_801FBF84.unk76A) {
        id = D_801FBF84.unk728[idx];
        sup = &D_8006D128[id - 0xC0];
        st = func_801F0CC4_inline(id, c);
        if (st >= 3) {
            chr_setSupportAbility(c->unk34.b.id, id, st == 3);
            ap = c->stones;
            if (st == 3) {
                c->stones = ap - sup->unk1;
            } else {
                c->stones = ap + sup->unk1;
            }
            chr_updateStats(c);
            snd_playSe(0x6B);
            return;
        }
    }
    snd_playSe(0x66);
}

void func_801F0024(void) {
    Character *c;
    MenuList *l;
    RECT r;
    s32 i;
    s32 id;
    s32 t;
    u8 *buf;

    l = &D_801FBF84.unk8[D_801FBF84.unk0];
    c = g_gameState->unk1C->party[D_801FBF84.unk2];
    g_menuHelp.text = NULL;
    help_setBounds(NULL);
    if (D_801FBF84.unk1 == 1 || D_801FBF84.unk1 == 5) {
        buf = D_801FBD84;
        g_menuHelp.text = buf;
        func_8002BE44(0, D_801F3478);
        func_8002BD54(buf, text_get(D_801FBF84.unk0 + 2));
        if (D_801FBF84.unk0 == 0) {
            if (D_8007AFD3) {
                t = 6;
            } else if (!D_801FBF84.unk769) {
                t = 5;
            } else {
                t = 4;
            }
        } else {
            if (func_80034A30(c)) {
                goto end;
            }
            if (c->unkA & 0x10) {
                t = 8;
            } else {
                t = 7;
            }
        }
        text_append(buf, text_get(t));
    end:
        text_append(D_801FBD84, text_get(0xF));
    } else if (D_801FBF84.unk1 == 2) {
        if (l->unk0 == 0) {
            return;
        }
        r.x = 0;
        r.y = 0x5E;
        r.w = g_gameState->unkC->disp.w;
        r.h = g_gameState->unkC->disp.h - 0x5E;
        help_setBounds(&r);
        i = func_801F0FA4(l);
        if (D_801FBF84.unk0 == 0) {
            id = D_801FBF84.unk668[i].unk0;
            if (func_801EFB8C(id, c)) {
                g_menuHelp.text = D_801F4734 + D_801F6240[id].unk0;
            }
        } else {
            id = D_801FBF84.unk728[i];
            if (func_801F0CC4_inline(id, c)) {
                u8 **d = &D_80077DF0;
                *d = func_801F0024_suphelp(id);
            }
        }
    } else if (D_801FBF84.unk1 == 3) {
        if (func_801F0FA4(l) & 1) {
            r.x = 0xC;
            r.y = 0;
            r.w = g_gameState->unkC->disp.w;
            r.h = g_gameState->unkC->disp.h;
        } else {
            r.x = 0;
            r.y = 0;
            r.w = g_gameState->unkC->disp.w >> 1;
            r.h = g_gameState->unkC->disp.h;
        }
        help_setBounds(&r);
        D_80077DF0 = text_get(D_801FBF84.unk76F == 0 ? 9 : 0xA);
    }
}

void func_801F0410(void) {
    s32 i;
    s32 sel;
    s32 row;
    Character *c;
    MenuList *l;

    sel = 0;
    i = 0;
    c = g_gameState->unk1C->party[D_801FBF84.unk2];
    l = &D_801FBF84.unk8[1];
    for (; i < D_801FBF84.unk76A; i++) {
        if (func_801F0CC4_inline(D_801FBF84.unk728[i], c) >= 3) {
            sel = i;
            break;
        }
    }
    row = sel / 2;
    l->unk7 = sel % 2;
    if (row - 7 >= 0) {
        l->unk5 = row - 7;
    } else {
        l->unk5 = 0;
    }
    l->cursor = row - l->unk5;
}

void func_801F05FC(void) {
    func_801F0664();
    func_801F06BC();
    func_801EE978();
}

void func_801F062C(void) {
    D_80077E53 = D_801FBF84.unk2;
    help_setCursorOnTop(1);
    help_setCallback(NULL);
}

void func_801F0664(void) {
    void (*fn)(void);

    if (!fade_isBusy()) {
        fn = D_801F33A8[D_801FBF84.unk1];
        if (fn != NULL) {
            fn();
        }
    }
}

void func_801F06BC(void) {
    func_801F07DC();
    func_801EEEA8();
    func_801EEFDC();
    func_801EF270();
    func_801EF3A8();
    func_801EF560();
    func_801EF644();
}

void func_801F070C(void) {
    MenuList *l;

    l = &D_801FBF84.unk8[D_801FBF84.unk0];
    if (l->unk8 == 0) {
        if (g_padRepeat & BTN_CIRCLE) {
            snd_playSe(0x65);
            D_801FBF84.unk1 = 1;
            return;
        }
        if (g_padRepeat & BTN_CROSS) {
            if (D_801FBF84.unk0 == 0) {
                func_801EFCB4();
            } else {
                func_801EFE2C();
            }
            return;
        }
        menuList_input(l, g_padRepeat);
        func_8002F044(l, g_padRepeat);
        func_8002F0EC(l, g_padRepeat);
    }
}

void func_801F07DC(void) {
    RECT *r;
    s32 i;

    r = &D_801FBF84.unk28;
    win_drawBox(r, 0x1D, 2);
    for (i = 0; i < 2; i++) {
        text_drawBlock(r->x + D_801F33C0[i], r->y + 6, 0x1C, text_get(i), D_801FBF84.unk4[i] == 0 ? 2 : 0);
    }
}

void func_801F0890(void) {
    MenuView v;
    s32 row;
    s32 col;
    s32 idx;

    row = 0;
    func_8002F344(&D_801FBF84.unk8[D_801FBF84.unk0], &v);
    idx = v.unk1 * 2;
    for (; row < v.unk0; row++) {
        for (col = 0; col < 2; col++) {
            func_801F0934(col, row, idx++, &v);
        }
    }
}

void func_801F0934(s32 col, s32 row, s32 z, MenuView *v) {
    Character *c = g_gameState->unk1C->party[D_801FBF84.unk2];
    s32 x, y;
    s32 dx = 8;
    s32 dy = 0x5E;

    x = col * 0x96 + (D_801FBF84.unk48.x + dx);
    y = row * 0xF + (v->unk2 + dy);
    if (D_801FBF84.unk0 == 0) {
        func_801F09CC(x, y, z, c);
    } else {
        func_801F0AB8(x, y, z, c);
    }
}

void func_801F09CC(s32 x, s32 y, s32 idx, Character *c) {
    u8 buf[8];
    u8 id;
    Unk801F34EC *info;
    s32 st;
    s32 col;

    id = D_801FBF84.unk668[idx].unk0;
    info = &D_801F34EC[id];
    st = func_801EFB8C(id, c);
    if (st != 0) {
        col = D_801F740C[st];
        text_drawBlock(x + 8, y, 0x1C, D_801F40EC + info->unkE, col);
        if (info->unkA != 0) {
            text_formatNumber(func_801F1018(info), buf, 3, 1);
            ui_drawSmallDigits(x + 0x6C, y + 4, 0x1C, buf, col);
        }
    }
}

void func_801F0AB8(s32 x, s32 y, s32 i, Character *c) {
    u8 buf[0x10];
    Unk8006D128 *sup;
    s32 id;
    u32 st;
    s32 col;

    id = D_801FBF84.unk728[i];
    sup = &D_8006D128[id - 0xC0];
    st = func_801F0CC4(id, c);
    if (st != 0) {
        col = D_801F740C[st];
        if (st == 4) {
            func_801F0F38(x + 8, y, 0x1C, D_801FBF84.unk76C);
        }
        if (!((g_gameState->config >> 18) & 1)) {
            func_801F0F38(x + 8, y, 0x1C, st >= 3 ? 9 : 10);
        } else {
            func_801F0F38(x + 8, y, 0x1C, st >= 3 ? 7 : 8);
        }
        text_drawBlock(x + 0x18, y, 0x1C, D_801F6540 + sup->unk2, col);
        text_formatNumber(sup->unk1, buf, 3, 1);
        ui_drawSmallDigits(x + 0x6C, y + 4, 0x1C, buf, col);
    }
}

u8 func_801F0C10(s32 id, Character *c) {
    s32 i;
    s32 j;
    u8 k;
    Unk801F6FEC *e;

    for (i = 0; i < 2; i++) {
        k = D_801F33D0[c->unk34.b.row][i];
        e = &D_801F6FEC[k];
        if (e->unk6 == 1) {
            for (j = 0; j < e->unk7; j++) {
                if (id == D_801F734C[e->unk8 + j]) {
                    return k;
                }
            }
        }
    }
    return 0;
}

u32 func_801F0CC4(s32 id, Character *c) {
    Unk8006D128 *sup;
    s32 slot;
    s32 ap;
    s32 need;

    if (id < 0xC0) {
        return 0;
    }
    if (func_80034A04(c->supportAbilities, id)) {
        return 4;
    }
    if (D_801FBF84.unk68[id] == 0) {
        slot = func_80034430(c->unk34.b.id, id);
        if (slot < 0) {
            return 0;
        }
        if (func_80034A30(c)) {
            ap = c->unk58[slot];
            need = D_8006CB28[c->unk34.b.row][slot].unk1;
            if (ap == 0) {
                return 0;
            }
            if (ap < need) {
                return 0;
            }
        }
    }
    sup = &D_8006D128[id - 0xC0];
    if (sup->unk1 > c->stones) {
        return 2;
    }
    return 3;
}

void func_801F0DDC(void) {
    MenuList *l = &D_801FBF84.unk8[D_801FBF84.unk0];

    func_80063118(&D_801FBF84.unk58, l->unk7 == 0 ? D_801F3458 : D_801F3468, 0);
    D_801FBF84.unk1 = 4;
}

void func_801F0E3C(void) {
    D_801FBF84.unk1 = 3;
}

void func_801F0E4C(void) {
    D_801FBF84.unk1 = 2;
    D_801FBF84.unk768 = 0;
}

s32 func_801F0E64(s32 id) {
    s32 kind = D_801F34EC[id].unk0;
    s32 n = 0;
    s32 i;

    for (i = 0; i < 4; i++) {
        if (g_gameState->unk1C->party[i] != NULL) {
            n++;
        }
    }
    if (n < 2) {
        return 0;
    }
    return (u32)(kind - 3) < 3;
}

void func_801F0ED4(void) {
    D_801FBF84.unk1 = 1;
}

void func_801F0EE4(void) {
    u8 *p = D_801EE164;

    LoadImage(&D_801F3484, (u32 *)p);
    DrawSync(0);
    LoadImage(&D_801F348C, (u32 *)(p + 0x120));
    DrawSync(0);
}

void func_801F0F38(s32 x, s32 y, s32 otz, s32 i) {
    ui_drawSpriteDef(x, y, otz, func_801F0F90(i), 0);
}

Icon *func_801F0F90(s32 i) {
    return &D_801F3494[i];
}

s32 func_801F0FA4(MenuList *l) {
    s32 r, t;
    s32 step;

    if (l->unk8 & 8) {
        t = l->unk7 ^ 1;
        step = 1;
        r = l->cursor;
        if (l->unk8 & 2) {
            return (l->unk5 + (r - step)) * 2 + t;
        }
        return (l->unk5 + (r + step)) * 2 + t;
    }
    return (l->unk5 + l->cursor) * 2 + l->unk7;
}

s32 func_801F1018(Unk801F34EC *p) {
    s32 v;

    v = p->unkA;
    if ((p->unkB & 4) && D_8007AF02 != 0) {
        v *= 4;
    }
    return v;
}

void func_801F1048(void) {
    Character *c;
    MenuList *l;
    s32 i;
    s32 sel;
    s32 row;
    s32 top;

    sel = 0;
    i = 0;
    c = g_gameState->unk1C->party[D_801FBF84.unk2];
    l = &D_801FBF84.unk8[0];
    for (; i < D_801FBF84.unk769; i++) {
        if (func_801EFB8C(D_801FBF84.unk668[i].unk0, c) == 3) {
            sel = i;
            break;
        }
    }
    row = sel / 2;
    l->unk7 = sel % 2;
    top = row - 7;
    if (top >= 0) {
        l->unk5 = top;
    } else {
        l->unk5 = 0;
    }
    l->cursor = row - l->unk5;
}

void func_801F1148(void) {
}

INCLUDE_RODATA("asm/us/ovl_33/nonmatchings/tu_801EE7C0", D_801EDF04);

INCLUDE_RODATA("asm/us/ovl_33/nonmatchings/tu_801EE7C0", D_801EE164);
