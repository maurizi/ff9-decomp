#include "ovl_37.h"

INCLUDE_RODATA("asm/us/ovl_37/nonmatchings/ovl_37", g_itemMenuText);

void func_801F0C18(void) {
    D_801FAB78.unk1 = 2;
    D_801FAB78.unk0 = 0;
    D_801FAB78.unk5 = 0;
    D_801FAB78.unk8 = 0;
    D_801FAB78.unk6 = 0;
    D_801FAB78.unk50 = 0;
    D_801FAB78.unk54 = 0;
    D_801FAB78.unk55 = 0;
    func_801F3038();
    func_801F2CEC();
    text_setBank(g_itemMenuText);
    help_setCallback(help_nullHandler);
    help_setCursorOnTop(1);
    func_8002EDFC(&D_801FAB78.unkC, 0xFF, 0xB, 2, 0xAD);
    func_8002EDFC(&D_801FAB78.unk1C, D_801FAB78.unkB0, 0xB, 2, 0xAD);
    setRECT(&D_801FAB78.unk2C, 8, 8, 0xE4, 0x1A);
    setRECT(&D_801FAB78.unk34, 8, 0x2A, 0x12C, 0xAD);
    func_80063118(&D_801FAB78.unk3C, 0, 0);
    func_801F2EDC();
}

void func_801F0D18(void) {
    if (g_padRepeat & BTN_CIRCLE) {
        snd_playSe(0x65);
        func_800361CC(0);
        return;
    }
    if (g_padRepeat & BTN_CROSS) {
        snd_playSe(0x67);
        if (D_801FAB78.unk0 != 1) {
            D_801FAB78.unk1 = 2;
            return;
        }
        func_801F2D58();
        return;
    }
    if (g_padRepeat & PAD_LEFT) {
        snd_playSe(0x67);
        D_801FAB78.unk0 = D_801FAB78.unk0 != 0 ? D_801FAB78.unk0 - 1 : 2;
        return;
    }
    if (g_padRepeat & PAD_RIGHT) {
        snd_playSe(0x67);
        D_801FAB78.unk0 = D_801FAB78.unk0 >= 2 ? 0 : D_801FAB78.unk0 + 1;
    }
}

void func_801F0E10(void) {
    ItemList *it = &D_801F3040[D_801FAB78.unk0];
    MenuList *l = it->unk8;
    s32 i;
    u8 cur;
    s32 next;

    if (D_801FAB78.unk3C.unkC == 0) {
        if (g_padRepeat & BTN_CIRCLE) {
            snd_playSe(0x65);
            func_801F2C58();
            return;
        }
        if (g_padRepeat & BTN_CROSS) {
            i = (l->unk5 + l->cursor) * 2 + l->unk7;
            func_801F2590(g_gameState->unk1C->items.slots[i].id);
            if (g_gameState->unk1C->items.slots[i].count == 0) {
                func_801F2C58();
            }
        } else if (g_padRepeat & PAD_UP) {
            cur = D_801FAB78.unk2;
            next = func_800337D0(cur);
            if (cur != next) {
                D_801FAB78.unk2 = next;
                snd_playSe(0x67);
            }
        } else if (g_padRepeat & PAD_DOWN) {
            cur = D_801FAB78.unk2;
            next = func_8003380C(cur);
            if (cur != next) {
                D_801FAB78.unk2 = next;
                snd_playSe(0x67);
            }
        }
    }
}

void func_801F0F50(void) {
    MenuList *l = D_801F3040[1].unk8;
    ItemSlot tmp;
    s32 i;
    u8 j;

    if (l->unk8 == 0) {
        if (g_padRepeat & BTN_CIRCLE) {
            snd_playSe(0x65);
            D_801FAB78.unk1 = 4;
            return;
        }
        if (g_padRepeat & BTN_CROSS) {
            if (D_801FAB78.unk7 == 0) {
                snd_playSe(0x67);
                j = l->unk7 + (l->cursor + l->unk5) * 2;
                D_801FAB78.unk7 = 1;
                D_801FAB78.unk6 = j;
                return;
            }
            snd_playSe(0x67);
            i = (l->cursor + l->unk5) * 2 + l->unk7;
            tmp = g_gameState->unk1C->items.slots[D_801FAB78.unk6];
            g_gameState->unk1C->items.slots[D_801FAB78.unk6] = g_gameState->unk1C->items.slots[i];
            g_gameState->unk1C->items.slots[i] = tmp;
            D_801FAB78.unk7 = 0;
            return;
        }
        menuList_input(l, g_padRepeat);
        func_8002F044(l, g_padRepeat);
        func_8002F0EC(l, g_padRepeat);
    }
}

void func_801F10C4(s32 idx, MenuList *l) {
    UnkPair *p;
    Unk80069018 *it;
    Unk8006B1F4 *e;
    u8 ok;
    s32 id;

    p = &g_gameState->unk1C->items.slots[idx];
    ok = 0;
    if (p->unk1 != 0 && p->unk0 != 0xFF && p->unk0 >= 0xE0) {
        id = p->unk0;
        it = &D_80069018[id];
        e = &D_8006B1F4[id - 0xE0];
        if (it->unkF & 1) {
            D_801FAB78.unk3 = e->unk0;
            ok = 1;
            D_801FAB78.unk4 = p->unk0 != 0xFB ? e->unk0_5 : 0;
            if (D_801FAB78.unk4 == 0) {
                func_801F2590(p->unk0);
            } else {
                func_801F1778();
            }
        }
    }
    if (!ok) {
        snd_playSe(0x66);
    }
}

void func_801F11E4(void) {
    MenuView view;
    ItemList *il;
    s32 w;
    s32 row;
    s32 col;
    s32 idx;

    il = &D_801F3040[D_801FAB78.unk0];
    ui_drawLabel(D_801F30A8->x + 0xE, D_801F30A8->y - 3, 0x1D, D_800703FC.unk68, 0);
    ui_drawLabel(D_801F30A8->x + 0xA4, D_801F30A8->y - 3, 0x1D, D_800703FC.unk68, 0);
    if (D_801FAB78.unk0 != 2) {
        w = func_8002AE7C((u8 *)D_800703FC.unk94);
        ui_drawLabel((D_801F30A8->x + 0x8A) - w, D_801F30A8->y - 3, 0x1D, D_800703FC.unk94, 0);
        ui_drawLabel((D_801F30A8->x + 0x120) - w, D_801F30A8->y - 3, 0x1D, D_800703FC.unk94, 0);
    }
    menuList_getView(il->unk8, &view);
    win_drawListBox(D_801F30A8, 0x1D, 1, il->unk8);
    win_drawShadow(D_801F30A8, 0x1D, 1);
    idx = view.unk1 * 2;
    win_setClip(NULL, 0x1C, 1);
    for (row = 0; row < view.unk0; row++) {
        for (col = 0; col < 2; col++) {
            il->unk4(col, row, idx++, &view);
        }
    }
    if (il->unk8->unk8 & 4) {
        func_8002F40C(il->unk8, D_801F30A8, &D_801FA958);
        win_setClip(&D_801FA958, 0x1C, 1);
        func_801F26E0();
    }
    win_setClip(D_801F30A8, 0x1C, 1);
}

void func_801F13F8(void) {
    MenuList *l;
    s32 off;
    u8 blink;
    s32 a;
    s32 k;
    u8 u;

    off = 0;
    l = D_801F3040[D_801FAB78.unk0].unk8;
    blink = (D_80077E54 & 3) != 0;
    help_setCursorOnTop(D_801FAB78.unk1 != 1);
    if (func_8002F810() || D_801FAB78.unk1 == 0) {
        return;
    }
    if (D_801FAB78.unk1 == 1 || blink) {
        g_menuHelp.x = D_801F30AC[D_801FAB78.unk0] - 0x10;
        g_menuHelp.y = 0x10;
        g_menuHelp.otz = 0x19;
        func_800355E4();
    }
    if (D_801FAB78.unk1 == 4 || (D_801FAB78.unk1 == 5 && blink)) {
        g_menuHelp.x = 0x7E;
        g_menuHelp.otz = 0x12;
        g_menuHelp.y = D_801FAB78.unk5 * 15 + 0xE;
        func_800355E4();
    }
    if (D_801FAB78.unk1 >= 2 && D_801FAB78.unk1 != 4 && (D_801FAB78.unk1 == 2 || blink)) {
        k = l->unk7 ^ ((l->unk8 >> 3) & 1);
        g_menuHelp.otz = 0x19;
        /* Both arms are identical (jump2 merges them after register allocation); the test keeps the
         * hi part of &D_801FAB78 and the constant 2 live, which the original's registers need. */
        if (D_801FAB78.unk0 == 2) {
            g_menuHelp.x = k * 0x96;
            g_menuHelp.y = l->cursor * 15 + 0x34;
            func_800355E4();
        } else {
            g_menuHelp.x = k * 0x96;
            g_menuHelp.y = l->cursor * 15 + 0x34;
            func_800355E4();
        }
    }
    if (D_801FAB78.unk1 == 3) {
        g_menuHelp.otz = 0x12;
        g_menuHelp.x = D_801FAB78.unk3C.unk4.x - 0xA;
        g_menuHelp.y = D_801FAB78.unk3C.unk4.y + (s16)(D_801FAB78.unk2 * 43 + 0x12);
        func_800355E4();
    }
    if (D_801FAB78.unk1 == 5) {
        if (D_801FAB78.unk7 == 1 && blink && !(l->unk8 & 4)) {
            a = l->unk5;
            if (D_801FAB78.unk6 >= a * 2 && D_801FAB78.unk6 < (a + 0xB) * 2) {
                if (l->unk8 != 0) {
                    off = l->unk9;
                    if (!(l->unk8 & 2)) {
                        off = -off;
                    }
                }
                u = D_801FAB78.unk6;
                g_menuHelp.x = (u & 1) * 0x96 + 2;
                g_menuHelp.y = ((u >> 1) - l->unk5) * 15 + (s16)(off + 0x32);
                g_menuHelp.otz = 0x12;
                func_800355E4();
            }
        }
        k = l->unk7 ^ ((l->unk8 >> 3) & 1);
        g_menuHelp.x = k * 0x96;
        g_menuHelp.y = l->cursor * 15 + 0x34;
        g_menuHelp.otz = 0x12;
        func_800355E4();
    }
}

void func_801F1778(void) {
    ItemMenu *m;
    RECT *r;
    GameState *gs;
    s32 k = (D_801F3040[D_801FAB78.unk0].unk8->unk7 ^ 1) & 1;

    D_801FAB78.unk3C.unk4.x = (u8)k == 0 ? 8 : 0xA4;
    r = &D_801FAB78.unk3C.unk4;
    r->y = 0x26;
    r->w = 0x90;
    r->h = 0xB6;
    func_80063118(&D_801FAB78.unk3C, D_801F30E0[k], 1);
    gs = g_gameState;
    m = &D_801FAB78;
    m->unk2 = 0;
loop:
    if (gs->unk1C->party[m->unk2] == NULL) {
        if (++m->unk2 < 4) {
            goto loop;
        }
    }
    m->unk1 = 0;
    m->unk8 = 1;
    snd_playSe(0x67);
}

void func_801F187C(void) {
    s32 n;
    s32 i;
    s32 j;

    n = 0;
    for (i = 0; i < 0x100; i++) {
        if (g_gameState->unk1C->items.slots[i].count != 0 && g_gameState->unk1C->items.slots[i].id != 0xFF) {
            D_801FA968[n++] = g_gameState->unk1C->items.slots[i];
        }
    }
    for (i = 0; i < n - 1; i++) {
        for (j = i + 1; j < n; j++) {
            if (D_80069018[D_801FA968[i].id].unk10 > D_80069018[D_801FA968[j].id].unk10) {
                D_801FAB68 = D_801FA968[i];
                D_801FA968[i] = D_801FA968[j];
                D_801FA968[j] = D_801FAB68;
            } else if (D_80069018[D_801FA968[i].id].unk10 == D_80069018[D_801FA968[j].id].unk10 &&
                       D_801FA968[j].id < D_801FA968[i].id) {
                D_801FAB68 = D_801FA968[i];
                D_801FA968[i] = D_801FA968[j];
                D_801FA968[j] = D_801FAB68;
            }
        }
    }
    for (i = 0; i < n; i++) {
        g_gameState->unk1C->items.slots[i] = D_801FA968[i];
    }
    for (i = n; i < 0x100; i++) {
        g_gameState->unk1C->items.slots[i].count = 0;
    }
    D_801FAB78.unkC.unk7 = 0;
    D_801FAB78.unkC.cursor = 0;
    D_801FAB78.unkC.unk5 = 0;
}

s32 func_801F1A7C(void) {
    ItemMenu *m = &D_801FAB78;

    if (g_gameState->mode == 3 && g_evtGlobalVars.b[0xBF] != 0 && g_evtGlobalVars.b[0xB5] == 0) {
        if (func_800B8124()) {
            g_evtGlobalVars.b[0xB5] = 1;
            func_800B81A0();
            m->unk4C = win_openFree(&D_801F3148, 0xF, 2);
            win_setOnOpened(m->unk4C, func_801F2B20);
            win_setDrawContent(m->unk4C, func_801F2B58);
            win_setTitle(m->unk4C, (s32)D_801F3140);
            win_setShadow(m->unk4C, 1);
            m->unk50 = 1;
            m->unk1 = 0;
            help_setLocked(1);
            return 1;
        }
        return 0;
    }
    return 0;
}

void func_801F1B78(void) {
    RECT r;
    MenuList *l;
    UnkPair *p;
    s32 i;
    s16 x;
    s16 y;
    s32 z;
    HelpState *h;

    h = &g_menuHelp;
    x = h->x;
    y = h->y;
    z = h->otz;
    h->text = NULL;
    h->x = x;
    h->y = y;
    h->otz = z;
    help_setBounds(NULL);
    if (D_801FAB78.unk1 == 1) {
        h->text = text_get(D_801FAB78.unk0 + 5);
    }
    if (D_801FAB78.unk1 == 2) {
        if (D_801FAB78.unk0 != 0) {
            i = func_801F2FC4(&D_801FAB78.unk1C);
            if (i < D_801FAB78.unkB0) {
                h->text = &D_801F651C[D_8006B374[D_801FAB78.unk60[i]].unk2];
            }
            return;
        }
        l = &D_801FAB78.unkC;
    } else if (D_801FAB78.unk1 == 3) {
        if (func_801F2FC4(&D_801FAB78.unkC) & 1) {
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
        g_menuHelp.text = text_get(8);
        return;
    } else if (D_801FAB78.unk1 == 4) {
        help_setBounds(&D_801F3150);
        h->text = text_get(D_801FAB78.unk5 + 9);
        return;
    } else if (D_801FAB78.unk1 == 5) {
        l = D_801F3040[1].unk8;
    } else {
        return;
    }
    p = &g_gameState->unk1C->items.slots[func_801F2FC4(l)];
    if (p->unk1 != 0) {
        h->text = &D_801F3168[D_80069018[p->unk0].unk2];
    }
}

void func_801F1D88(void) {
    if (D_801FAB78.unk54 != 0 && D_801FAB78.unk55 != 0) {
        if (D_801FAB78.unk55 == 1) {
            D_801FAB78.unk58 += D_801F3064[D_801FAB78.unk57++];
            if (D_801FAB78.unk57 >= 0x16) {
                D_801FAB78.unk55 = 2;
                D_801FAB78.unk57 = 0;
                D_801FAB78.unk1 = 6;
            }
        } else if (D_801FAB78.unk55 == 3) {
            D_801FAB78.unk58 += D_801F3064[D_801FAB78.unk57++];
            if (D_801FAB78.unk57 >= 8) {
                D_801FAB78.unk57 = 0;
                D_801FAB78.unk55 = 0;
                D_801FAB78.unk54 = 0;
                D_801FAB78.unk1 = 2;
                keyItem_setSeen(D_801FAB78.unk56);
                help_setLocked(0);
            }
        }
    }
}

void func_801F1E88(void) {
    ItemMenu *m = &D_801FAB78;
    s32 x;
    u8 *name;

    if (m->unk54 != 0) {
        x = m->unk58;
        name = D_8006ADAC + D_8006B374[m->unk56].unk0;
        text_drawBlock(x + ((0xB0 - func_8002BA68((s32)name)) >> 1), 0x50, 0x14, name, 8);
        text_drawBlock(x + 0xD, 0x66, 0x14, D_801F79F8 + D_8006B374[m->unk56].unk6, 8);
        setlen(g_primPtr, 4);
        ((SprtWords *)g_primPtr)->rgbc = 0x64808080;
        ((SprtWords *)g_primPtr)->xy = (x & 0xFFFF) | 0x410000;
        ((SprtWords *)g_primPtr)->uvclut = (GetClut(0x3C0, 0x1B0) << 16) | 0x8000;
        ((SprtWords *)g_primPtr)->wh = 0x8000B0;
        addPrim(&g_ot[0x14], g_primPtr);
        g_primPtr += sizeof(SprtWords);
        setlen(g_primPtr, 4);
        ((SprtWords *)g_primPtr)->rgbc = 0x66000000;
        ((SprtWords *)g_primPtr)->xy = ((x + 3) & 0xFFFF) | 0xBE0000;
        ((SprtWords *)g_primPtr)->uvclut = (GetClut(0x3C0, 0x1B1) << 16) | 0xFA00;
        ((SprtWords *)g_primPtr)->wh = 0x600B0;
        addPrim(&g_ot[0x14], g_primPtr);
        g_primPtr += sizeof(SprtWords);
        setlen(g_primPtr, 4);
        ((SprtWords *)g_primPtr)->rgbc = 0x66000000;
        ((SprtWords *)g_primPtr)->xy = ((x + 0xAD) & 0xFFFF) | 0x440000;
        ((SprtWords *)g_primPtr)->uvclut = (GetClut(0x3C0, 0x1B1) << 16) | 0x80AA;
        ((SprtWords *)g_primPtr)->wh = 0x7A0006;
        addPrim(&g_ot[0x14], g_primPtr);
        g_primPtr += sizeof(SprtWords);
        SetDrawTPage(g_primPtr, 1, 1, 0x1E);
        addPrim(&g_ot[0x14], g_primPtr);
        g_primPtr += sizeof(DR_TPAGE);
    }
}

void func_801F2160(void) {
    func_801F21E8();
    func_801F2240();
    func_801F2280();
}

void func_801F2190(void) {
    if (g_gameState->mode == 3) {
        func_8002F55C(0x44A);
    }
    help_setLocked(0);
    help_setCursorOnTop(1);
    help_setCallback(NULL);
    func_801F2F30();
}

void func_801F21E8(void) {
    void (*fn)(void);

    if (!func_8002F810()) {
        fn = D_801F307C[D_801FAB78.unk1];
        if (fn != NULL) {
            fn();
        }
    }
}

void func_801F2240(void) {
    func_801F262C();
    func_801F11E4();
    func_801F27A0();
    func_801F1E88();
    func_801F13F8();
}

void func_801F2280(void) {
    func_801F1B78();
    func_801F1D88();
    func_801F2BC0();
    func_800631AC(&D_801FAB78.unk3C);
    func_8002EB74(&D_801FAB78.unkC);
    func_8002EB74(&D_801FAB78.unk1C);
}

void func_801F22C8(void) {
    ItemList *it = &D_801F3040[D_801FAB78.unk0];
    MenuList *l = it->unk8;

    if (l->unk8 == 0) {
        if (g_padRepeat & BTN_CIRCLE) {
            snd_playSe(0x65);
            D_801FAB78.unk1 = 1;
            return;
        }
        if (g_padRepeat & BTN_CROSS) {
            func_801F24DC();
            return;
        }
        if (l->unk0 != 0) {
            menuList_input(l, g_padRepeat);
            if (l->unk8 == 0) {
                func_8002F044(l, g_padRepeat);
                if (l->unk8 == 0) {
                    func_8002F0EC(l, g_padRepeat);
                }
            }
        }
    }
}

void func_801F23C4(void) {
    if (g_padRepeat & BTN_CIRCLE) {
        snd_playSe(0x65);
        func_801F2E18();
        return;
    }
    if (g_padRepeat & (PAD_UP | PAD_DOWN)) {
        snd_playSe(0x67);
        D_801FAB78.unk5 ^= 1;
        return;
    }
    if (g_padRepeat & BTN_CROSS) {
        if (D_801FAB78.unk5 == 0) {
            snd_playSe(0x67);
            func_801F187C();
            return;
        }
        snd_playSe(0x67);
        D_801FAB78.unk7 = 0;
        D_801FAB78.unk1 = 5;
    }
}

void func_801F2488(void) {
    if (g_padRepeat & BTN_CIRCLE) {
        snd_playSe(0x65);
        func_801F2FA0();
    } else if (g_padRepeat & BTN_CROSS) {
        snd_playSe(0x67);
        func_801F2FA0();
    }
}

void func_801F24DC(void) {
    ItemList *it = &D_801F3040[D_801FAB78.unk0];
    MenuList *l = it->unk8;
    void (*f)(s32, MenuList *) = it->unk0;
    s32 idx = (l->unk5 + l->cursor) * 2 + l->unk7;

    do {
        f(idx, l);
    } while (0);
}

void func_801F253C(s32 i) {
    if (i < D_801FAB78.unkB0) {
        func_801F2F38(D_801FAB78.unk60[i]);
        snd_playSe(0x67);
    } else {
        snd_playSe(0x66);
    }
}

void func_801F2590(s32 id) {
    Unk8006B1F4 *e;
    Character *c;
    s32 i;

    i = id - 0xE0;
    e = &D_8006B1F4[i];
    c = g_gameState->unk1C->party[D_801FAB78.unk2];
    if (func_800654F8(c, c, e, e->unk4, 0)) {
        snd_playSe(0x6A);
        item_remove(id, 1);
    } else {
        snd_playSe(0x66);
    }
}

void func_801F262C(void) {
    RECT *r;
    s32 i;

    r = &D_801FAB78.unk2C;
    for (i = 0; i < 3; i++) {
        text_drawBlock(r->x + D_801F309C[i], r->y + 6, 0x1C, text_get(D_801F3098[i]), 0);
    }
    win_drawBox(r, 0x1D, 2);
    win_drawShadow(r, 0x1D, 2);
}

void func_801F26E0(void) {
    MenuView v;
    ItemList *it;
    s32 row;
    s32 col;
    s32 idx;

    it = &D_801F3040[D_801FAB78.unk0];
    row = 0;
    func_8002F344(it->unk8, &v);
    idx = v.unk1 * 2;
    for (; row < v.unk0; row++) {
        for (col = 0; col < 2; col++) {
            it->unk4(col, row, idx++, &v);
        }
    }
}

void func_801F27A0(void) {
    RECT *r = &D_801FAB78.unk3C.unk4;
    s32 i;

    if (D_801FAB78.unk8 != 0) {
        for (i = 0; i < 4; i++) {
            if (g_gameState->unk1C->party[i] != NULL) {
                ui_drawMemberSummary(r->x + 8, r->y + 6 + i * 0x2B, 0x14, i, D_801FAB78.unk4);
            }
        }
        win_drawBox(r, 0x14, 2);
        win_drawShadow(r, 0x14, 2);
    }
}

void func_801F2870(s32 col, s32 row, s32 i, MenuView *v) {
    UnkPair *p = &g_gameState->unk1C->items.slots[i];
    Unk80069018 *it;
    u8 ok;
    s32 x, y, x2;
    s32 k = 0x2A;
    u8 buf[8];

    if (p->unk1 != 0 && p->unk0 != 0xFF) {
        it = &D_80069018[p->unk0];
        ok = it->unkF & 1;
        x = col * 0x96;
        y = row * 0xF + (v->unk2 + k);
        text_formatNumber(p->unk1, buf, 2, 1);
        x2 = x + 0x18;
        text_drawBlock(x + 0x28, y, 0x1C, D_8006A218 + it->unk0, ok ? 0 : 2);
        ui_drawItemIcon(x2, y, 0x1C, p->unk0, ok);
        ui_drawSmallDigits(x2 + 0x6C, y + 4, 0x1C, buf, ok ? 0 : 2);
    }
}

void func_801F29D0(s32 col, s32 row, s32 i, MenuView *v) {
    u8 blink = (u32)D_80077DC0 % 48 >= 0x10;
    u8 id;
    s32 x, y;
    s32 k = 0x2A;
    u8 *name;
    s32 w;
    Unk8006B374 *t;
    u8 got;

    if (i < D_801FAB78.unkB0) {
        id = D_801FAB78.unk60[i];
        t = &D_8006B374[id];
        got = keyItem_isSeen(id);
        x = col * 0x96 + 0x18;
        y = row * 0xF + (v->unk2 + k);
        name = D_8006ADAC + t->unk0;
        if (!got) {
            w = x + func_8002BA68((s32)name);
            ui_drawSprite(w + 5, y + 2, 0x1C, 0xB6, blink);
            ui_drawSprite(w - 1, y - 1, 0x1C, 0xB5, 0);
        }
        text_drawBlock(x, y, 0x1C, name, 0);
    }
}

void func_801F2B20(Window *w) {
    if (g_gameState->mode == 3) {
        func_8002F514(0x44A);
    }
}

void func_801F2B58(Window *w) {
    u8 *s;

    s = text_get(0xB);
    text_drawBlock(w->unk0 + ((w->unk4 - func_8002BA68((s32)s)) >> 1), w->unk2 + 6, w->unk18, s, 0);
}

void func_801F2BC0(void) {
    u32 t;

    if (D_801FAB78.unk1 == 0) {
        t = D_801FAB78.unk50;
        if (t != 0) {
            D_801FAB78.unk50 = t + 1;
            if (t >= 0x60) {
                D_801FAB78.unk50 = 0;
                if (g_gameState->mode == 3) {
                    func_8002F55C(0x44A);
                }
                help_setLocked(0);
                help_setCursorOnTop(1);
                help_setCallback(NULL);
                func_801F2F30();
                func_800362C8();
            }
        }
    }
}

void func_801F2C58(void) {
    ItemList *it;
    ItemMenu *m;

    it = D_801F3040;
    m = &D_801FAB78;
    func_80063118(&m->unk3C, D_801F3120[it[D_801FAB78.unk0].unk8->unk7 & 1], 0);
    m->unk1 = 0;
}

void func_801F2CC4(void) {
    D_801FAB78.unk1 = 3;
}

void func_801F2CD4(void) {
    D_801FAB78.unk1 = 2;
    D_801FAB78.unk8 = 0;
}

void func_801F2CEC(void) {
    s32 n;
    s32 i;

    n = 0;
    for (i = 0; i < 0x50; i++) {
        if ((u8)keyItem_has(i)) {
            D_801FAB78.unk60[n] = i;
            n++;
        }
    }
    D_801FAB78.unkB0 = n;
}

void func_801F2D58(void) {
    MenuList *l;

    l = D_801F3040[1].unk8;
    D_801FAB78.unk4C = win_openFree(&D_801F30B8, 0x14, 2);
    win_setDrawContent(D_801FAB78.unk4C, func_801F2E40);
    win_setOnOpened(D_801FAB78.unk4C, func_801F2EBC);
    win_setOnClosed(D_801FAB78.unk4C, func_801F2ECC);
    win_setShadow(D_801FAB78.unk4C, 1);
    win_setTitle(D_801FAB78.unk4C, D_80070408);
    D_801FAB78.unk5 = 0;
    D_801FAB78.unk6 = l->unk7 + (l->unk5 + l->cursor) * 2;
    D_801FAB78.unk1 = 0;
}

void func_801F2E18(void) {
    win_close(D_801FAB78.unk4C);
}

void func_801F2E40(Window *w) {
    Window *p = w;
    s32 i;

    for (i = 0; i < 2; i++) {
        text_drawBlock(p->unk0 + 10, p->unk2 + 6 + i * 15, w->unk18, text_get(i + 3), 0);
    }
}

void func_801F2EBC(Window *w) {
    D_801FAB78.unk1 = 4;
}

void func_801F2ECC(Window *w) {
    D_801FAB78.unk1 = 1;
}

void func_801F2EDC(void) {
    u8 *p = D_801EDFD8;

    LoadImage(&D_801F3158, (u32 *)p);
    DrawSync(0);
    LoadImage(&D_801F3160, (u32 *)(p + 0x40));
    DrawSync(0);
}

void func_801F2F30(void) {
}

void func_801F2F38(s32 id) {
    D_801FAB78.unk56 = id;
    D_801FAB78.unk54 = 1;
    D_801FAB78.unk55 = 1;
    D_801FAB78.unk1 = 0;
    D_801FAB78.unk57 = 0;
    D_801FAB78.unk58 = 0x48;
    D_801FAB78.unk58 -= func_800632B4(0x16, D_801F3064);
    help_setLocked(1);
}

void func_801F2FA0(void) {
    D_801FAB78.unk55 = 3;
    D_801FAB78.unk1 = 0;
    D_801FAB78.unk57 = 0;
    D_801FAB78.unk58 = 0x48;
}

s32 func_801F2FC4(MenuList *l) {
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

void func_801F3038(void) {
}

INCLUDE_RODATA("asm/us/ovl_37/nonmatchings/ovl_37", D_801EDFD8);
