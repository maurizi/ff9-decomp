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
    if (func_80034A04(c->unk88, id)) {
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
    func_80028CF8(&D_801FBF84.unk770, D_80077E50.unk3, func_801F0ED4);
    func_801EF900(D_80077E50.unk3, 0);
    D_801FBF84.unk76C = 0;
    D_801FBF84.unk76B = 0;
    text_setBank(D_801EDF04);
    func_801F1148();
    func_8003556C(D_800355DC);
    func_80035674(1);
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

    if (func_8003561C()) {
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
        func_80028D2C(&D_801FBF84.unk770);
    }
}

void func_801EEA50(void) {
    u8 slot;
    s32 next;

    if (g_padRepeat & BTN_CIRCLE) {
        snd_playSe(0x65);
        func_800361CC(0);
    } else if (g_padRepeat & BTN_SQUARE) {
        snd_playSe(0x67);
        func_80036270(3);
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
        ok = func_800654F8(c, g_gameState->unk1C->party[D_801FBF84.unk3], it, it->unk4, 0);
    } else {
        for (i = 0; i < 4; i++) {
            if (g_gameState->unk1C->party[i] != NULL) {
                ok |= func_800654F8(c, g_gameState->unk1C->party[i], it, it->unk4, 1);
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
    func_80024428(r->x + 8, r->y + 5, 0x1D, (Unk80024428 *)&D_801FBF84.unk770);
    box.w = 0x5B;
    box.h = 0x12;
    box.x = r->x + 0x72;
    box.y = r->y + 4;
    func_80026FC4(box.x + 3, box.y + 4, 0x1D, c->unk38, 0);
    func_8002DEE4(&box, 0x1D, 0);
    if (D_801FBF84.unk0 == 1) {
        func_800273C0(box.x + 0x19, box.y + 0x14, 0x1D, c->stones, c->maxStones);
    }
    func_8002B524(r->x + 6, r->y - 3, 0x1D, D_80070498, 0);
    func_8002DEE4(r, 0x1D, 1);
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
    func_8002B524(r->x + 6, r->y - 3, 0x1D, D_80070460, 0);
    func_8002DEE4(r, 0x1D, 2);
    func_8002B5FC(r->x + 8, r->y + 8, 0x1C, text_get(0x10), 0);
    func_8002B5FC(r->x + 8, r->y + 0x16, 0x1C, text_get(0x11), 0);
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
        func_80027110(r->x + 0x2D, r->y + 0xC, 0x1C, func_80034584(c->unk34.b.id, id), func_800347D8(c->unk34.b.id, id), flag, 0);
    }
    if (D_801FBF84.unk68[id] != 0) {
        x = r->x;
        for (i = 0; i < 5; i++) {
            if ((D_801FBF84.unk68[id] >> i) & 1) {
                func_80026C34(x + 8, r->y + 0x24, 0x1C, D_801FBF84.unk168[id][i], 1);
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
                func_8002B5FC(r->x + 8, r->y + 6, 0x1B, D_801F722C + D_801F6FEC[m->unk668[idx].unk1].unk0, 0);
            }
        }
        func_8002B524(r->x + 6, r->y - 3, 0x1B, D_8007042C, 0);
        func_8002DEE4(r, 0x1B, 2);
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
    func_8002B524(r->x + 0xE, r->y - 3, 0x1C, D_800703FC.unk4, 0);
    func_8002B524(r->x + 0xA4, r->y - 3, 0x1C, D_800703FC.unk4, 0);
    func_8002B524(r->x + 0x7C, r->y - 3, 0x1C, D_800703FC.unk7C, 0);
    func_8002B524(r->x + 0x112, r->y - 3, 0x1C, D_800703FC.unk7C, 0);
    func_8002E040(r, 0x1D, 1, l);
    if (l->unk0 != 0) {
        func_8002ECA0(l, &view);
        func_8002D9B4(NULL, 0x1C, 1);
        idx = view.unk1 * 2;
        for (row = 0; row < view.unk0; row++) {
            for (col = 0; col < 2; col++) {
                func_801F0934(col, row, idx++, &view);
            }
        }
        if (l->unk8 & 4) {
            func_8002F40C(l, r, &sub);
            func_8002D9B4(&sub, 0x1C, 1);
            func_801F0890();
        }
        func_8002D9B4(r, 0x1C, 1);
    }
}

void func_801EF560(void) {
    RECT *r = &D_801FBF84.unk58.unk4;
    EquipMenu *m = &D_801FBF84;
    s32 i;

    if (m->unk768 != 0) {
        func_8002B524(r->x + 8, r->y - 3, 0x14, D_800704B8, 0);
        func_8002DEE4(r, 0x14, 2);
        for (i = 0; i < 4; i++) {
            if (g_gameState->unk1C->party[i] != NULL) {
                func_80026EA0(r->x + 8, r->y + 6 + i * 0x2B, 0x13, i, D_801FBF84.unk76D);
            }
        }
    }
}

void func_801EF644(void) {
    RECT *r;
    MenuList *l;
    Unk80077DE8 *h3;
    s32 i;
    s32 y;
    u16 k;
    u8 blink;
    u8 done;
    s16 ry;

    done = 0;
    blink = (D_80077E54 & 3) != 0;
    func_80035674(D_801FBF84.unk1 != 1);
    if (func_8002F810()) {
        return;
    }
    if (D_801FBF84.unk1 == 1 || D_801FBF84.unk1 == 5 || blink) {
        Unk80077DE8 *h1;
        r = &D_801FBF84.unk28;
        h1 = &D_80077DE8;
        k = D_801F33C8[D_801FBF84.unk0][0] - 0x18;
        D_80077DE8.unk0 = r->x + k;
        ry = r->y + 8;
        h1->unk4 = 0x17;
        h1->unk2 = ry;
        func_800355E4();
    }
    if (D_801FBF84.unk1 == 2 || (D_801FBF84.unk1 == 3 && blink)) {
        Unk80077DE8 *h2;
        l = &D_801FBF84.unk8[D_801FBF84.unk0];
        h2 = &D_80077DE8;
        k = l->unk7;
        k ^= (l->unk8 >> 3) & 1;
        k = k * 150 - 10;
        D_80077DE8.unk0 = D_801FBF84.unk48.x + k;
        k = l->cursor * 15 + 10;
        h2->unk4 = 0x17;
        h2->unk2 = D_801FBF84.unk48.y + k;
        func_800355E4();
    }
    if (D_801FBF84.unk1 == 3) {
        r = &D_801FBF84.unk58.unk4;
        if (D_801FBF84.unk3 & 0x80) {
            D_80077DE8.unk4 = 0x12;
            if (blink) {
                for (i = 0; i < 4; i++) {
                    y = i * 0x2B + 0x12;
                    if (g_gameState->unk1C->party[i] != NULL) {
                        func_800270EC(r->x - 10, r->y + y, D_80077DE8.unk4);
                        if (!done) {
                            D_80077DE8.unk0 = r->x - 10;
                            done = 1;
                            D_80077DE8.unk2 = r->y + y;
                        }
                    }
                }
            }
        } else {
            D_80077DE8.unk0 = D_801FBF84.unk58.unk4.x - 10;
            h3 = &D_80077DE8;
            k = D_801FBF84.unk3 * 0x2B + 0x12;
            ry = r->y;
            h3->unk4 = 0x12;
            h3->unk2 = ry + k;
            func_800355E4();
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
    func_80028D08(b, m->unk2, slot, arg1);
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
    D_801FBF84.unk4[0] = c->hp != 0 && !(c->unk38 & 9) && nItems != 0 && D_8007AFD3 == 0;
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
    if (c->unk38 & 9) {
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
            func_8003493C(c->unk34.b.id, id, st == 3);
            ap = c->stones;
            if (st == 3) {
                c->stones = ap - sup->unk1;
            } else {
                c->stones = ap + sup->unk1;
            }
            func_8003384C(c);
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
    D_80077DE8.unk8 = NULL;
    func_80035580(NULL);
    if (D_801FBF84.unk1 == 1 || D_801FBF84.unk1 == 5) {
        buf = D_801FBD84;
        D_80077DE8.unk8 = buf;
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
        func_8002AD38(buf, text_get(t));
    end:
        func_8002AD38(D_801FBD84, text_get(0xF));
    } else if (D_801FBF84.unk1 == 2) {
        if (l->unk0 == 0) {
            return;
        }
        r.x = 0;
        r.y = 0x5E;
        r.w = g_gameState->unkC->disp.w;
        r.h = g_gameState->unkC->disp.h - 0x5E;
        func_80035580(&r);
        i = func_801F0FA4(l);
        if (D_801FBF84.unk0 == 0) {
            id = D_801FBF84.unk668[i].unk0;
            if (func_801EFB8C(id, c)) {
                D_80077DE8.unk8 = D_801F4734 + D_801F6240[id].unk0;
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
        func_80035580(&r);
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
    func_80035674(1);
    func_8003556C(NULL);
}

void func_801F0664(void) {
    void (*fn)(void);

    if (!func_8002F810()) {
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
        func_8002EE8C(l, g_padRepeat);
        func_8002F044(l, g_padRepeat);
        func_8002F0EC(l, g_padRepeat);
    }
}

void func_801F07DC(void) {
    RECT *r;
    s32 i;

    r = &D_801FBF84.unk28;
    func_8002DEE4(r, 0x1D, 2);
    for (i = 0; i < 2; i++) {
        func_8002B5FC(r->x + D_801F33C0[i], r->y + 6, 0x1C, text_get(i), D_801FBF84.unk4[i] == 0 ? 2 : 0);
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
        func_8002B5FC(x + 8, y, 0x1C, D_801F40EC + info->unkE, col);
        if (info->unkA != 0) {
            text_formatNumber(func_801F1018(info), buf, 3, 1);
            func_8002B56C(x + 0x6C, y + 4, 0x1C, buf, col);
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
        if (!((g_gameState->unk24 >> 18) & 1)) {
            func_801F0F38(x + 8, y, 0x1C, st >= 3 ? 9 : 10);
        } else {
            func_801F0F38(x + 8, y, 0x1C, st >= 3 ? 7 : 8);
        }
        func_8002B5FC(x + 0x18, y, 0x1C, D_801F6540 + sup->unk2, col);
        text_formatNumber(sup->unk1, buf, 3, 1);
        func_8002B56C(x + 0x6C, y + 4, 0x1C, buf, col);
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
    if (func_80034A04(c->unk88, id)) {
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
    func_80027710(x, y, otz, func_801F0F90(i), 0);
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
    func_8003556C(D_800355DC);
    func_80035674(1);
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
    func_801F3278(D_80077E50.unk3, 0);
    func_80028CF8(&D_801FC840.unk24C, D_80077E50.unk3, func_801F3264);
}

void func_801F12C8(void) {
    u8 slot;
    s32 next;

    if (g_padRepeat & BTN_CIRCLE) {
        snd_playSe(0x65);
        func_800361CC(0);
    } else if (g_padRepeat & BTN_SQUARE) {
        snd_playSe(0x67);
        func_80036270(2);
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
    func_800269F4(r->x + 3, r->y + 0x3B, 0x1E, r->w - 6);
    func_8002B524(r->x + 6, r->y - 3, 0x1E, D_80070460, 0);
    func_8002DEE4(r, 0x1E, 1);
    func_80024428(r->x + 8, r->y + 9, 0x1D, (Unk80024428 *)&D_801FC840.unk24C);
    cur.unk8[0] = c->speed;
    cur.unk8[1] = c->strength;
    cur.unk8[2] = c->magic;
    cur.unk8[3] = c->spirit;
    cur.unkC[0] = D_8006E0E8[c->equip[0]].unk5;
    cur.unkC[1] = c->defense;
    cur.unkC[2] = c->evade;
    cur.unkC[3] = c->magicDef;
    cur.unkC[4] = c->magicEva;
    if (D_801FC840.unk1 == 3 && D_801FC840.unk4.unk2 != 0) {
        in.unk0 = *(Bytes8 *)&c->unk2C;
        in.unk10 = c->hp;
        in.unk12 = c->mp;
        in.unk14 = *(Bytes5 *)c->equip;
        in.unk8 = *(Words2 *)c->unk88;
        in.unk14.unk0[D_801FC840.unk2] = (D_801FC840.unk3C + (D_801FC840.unk4.cursor + D_801FC840.unk4.unk5))->id;
        func_80032E84(&in, &out);
        func_801F20A0(&cur, &out);
        return;
    } else if (*(u16 *)&D_801FC840 == 0x202 && D_801FC840.unk2 != 0) {
        in.unk0 = *(Bytes8 *)&c->unk2C;
        in.unk10 = c->hp;
        in.unk12 = c->mp;
        in.unk14 = *(Bytes5 *)c->equip;
        in.unk8 = *(Words2 *)c->unk88;
        in.unk14.unk0[D_801FC840.unk2] = 0xFF;
        func_80032E84(&in, &out);
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
    func_8002B524(r->x + 6, r->y - 3, 0x1E, D_8007043C, 0);
    func_8002DEE4(r, 0x1E, 2);
    func_8002705C(r, 0x1E, 2);
    for (i = 0, grey = 2, y8 = 8, y12 = 12, y10 = 10; i < 5; i++) {
        sel = 0;
        if (D_801FC840.unk0 == grey) {
            sel = (i == 0);
        }
        func_800275DC(r->x + 10, r->y + y10, 0x1D, i + 0xF9, sel);
        func_8002B56C(r->x + 0x19, r->y + y12, 0x1D, D_801EE644, sel ? grey : 0);
        if (c->equip[i] != 0xFF) {
            it = &D_80069018[c->equip[i]];
            func_80026C34(r->x + 0x21, r->y + y8, 0x1D, c->equip[i], !sel);
            func_8002B5FC(r->x + 0x31, r->y + y8, 0x1D, D_8006A218 + it->unk0, sel ? grey : 0);
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
    func_8002B524(r->x + 6, r->y - 3, 0x13, D_80070400, 0);
    func_800269F4(r->x + 3, r->y + 0x1A, 0x1E, r->w - 6);
    func_8002DEE4(r, 0x1E, 1);
    if (D_801FC840.unk1 < 2) {
        return;
    }
    func_800275DC(r->x + 0xA, r->y + 9, 0x1D, D_801FC840.unk2 + 0xF9, 0);
    func_8002B56C(r->x + 0x19, r->y + 0xB, 0x1D, D_801EE644, 0);
    if (D_801FC840.unk1 == 2) {
        id = c->equip[D_801FC840.unk2];
    } else {
        id = D_801FC840.unk3C[D_801FC840.unk4.unk5 + D_801FC840.unk4.cursor].id;
    }
    if (id == 0xFF) {
        return;
    }
    func_80026C34(r->x + 0x21, r->y + 7, 0x1D, id, 1);
    func_8002B5FC(r->x + 0x31, r->y + 7, 0x1D, D_8006A218 + D_80069018[id].unk0, 0);
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
                } else if (func_80034A04(c->unk88, ab)) {
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
                func_80027110(r->x + 0x72, r->y + (y3 + k), 0x1D, func_80034584(c->unk34.b.id, ab), ap, flag, 0);
            }
            func_8002B5FC(r->x + 0x18, r->y + y2, 0x1D, name, ok ? 0 : 2);
            func_800275DC(r->x + 8, r->y + y1, 0x1D, icon, ok ^ 1);
        } else {
            func_8002B5FC(r->x + 0x18, r->y + y2, 0x1D, name, 2);
            func_800275DC(r->x + 8, r->y + y1, 0x1D, ab < 0xC0 ? 0xBE : 0xBF, 1);
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
        func_8002B524(wd->unk4.x + 6, r->y - 3, 0x14, D_800703FC.unk0[D_801F8918[m->unk2]], 0);
        func_8002B524(wd->unk4.x - (w - k), r->y - 3, 0x14, D_800703FC.unk94, 0);
        func_8002E040(r, 0x14, 1, l);
        func_8002ECA0(l, &view);
        func_8002D9B4(NULL, 0x13, 1);
        for (i = 0; i < view.unk0; i++) {
            func_801F2F3C(0, i, view.unk1 + i, &view);
        }
        if (l->unk8 & 4) {
            func_8002F40C(l, r, &sub);
            func_8002D9B4(&sub, 0x13, 1);
            func_801F2ED4();
        }
        if (wd->unkC == 0) {
            func_8002D9B4(r, 0x13, 1);
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
        func_8002B56C(r->x + 0x4A, y, 0x1D, buf, 0);
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
            func_8002B56C(r->x + 0x5C, y, 0x1D, D_801F892C, (diff && blink) ? 0 : 2);
            func_8002B56C(r->x + 0x6D, y, 0x1D, buf, col);
        }
    }
    for (i = 0, y = r->y + 0x78; i < 5; y += 0xC, i++) {
        func_801F3344(r->x + 8, y, 0x1D, D_801F8934[i], 0);
        text_formatNumber(cur->unkC[i], buf, 3, 1);
        func_8002B56C(r->x + 0x43, y, 0x1D, buf, 0);
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
            func_8002B56C(r->x + 0x5C, y, 0x1D, D_801F892C, (diff && blink) ? 0 : 2);
            func_8002B56C(r->x + 0x66, y, 0x1D, buf, col);
        }
    }
}

void func_801F239C(void) {
    AbilityMenu *m = &D_801FC840;
    u16 x;
    u16 k;
    Unk80077DE8 *h;

    func_80035674(m->unk1 != 1);
    if (func_8002F810() || m->unk1 == 0) {
        return;
    }
    if (m->unk1 == 1 || (D_80077E54 & 3)) {
        h = &D_80077DE8;
        x = D_801F893C[D_801FC840.unk0][0];
        h->unk2 = m->unk14.y + 8;
        h->unk4 = 0x1B;
        h->unk0 = m->unk14.x + x - 0x18;
        func_800355E4();
    }
    if (D_801FC840.unk1 == 2) {
        D_80077DE8.unk4 = 0x12;
        D_80077DE8.unk0 = D_801FC840.unk24.x - 0x10;
        k = D_801FC840.unk2 * 17 + 0xC;
        D_80077DE8.unk2 = D_801FC840.unk24.y + k;
        func_800355E4();
    } else if (D_801FC840.unk1 == 3) {
        D_80077DE8.unk4 = 0x12;
        D_80077DE8.unk0 = D_801FC840.unk23C.unk4.x - 0xD;
        k = D_801FC840.unk4.cursor * 15 + 0xA;
        D_80077DE8.unk2 = D_801FC840.unk23C.unk4.y + k;
        func_800355E4();
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
                func_80027D84(c->equip[i], 1);
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
                    func_8003493C(c->unk34.b.id, a->unk0, 0);
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
    func_8002EDFC(&D_801FC840.unk4, n, 6, 1, 0x62);
}

void func_801F2AB4(void) {
    Character *c;
    u8 *buf;
    s32 id;
    UnkPair *e;
    MenuList *l;

    c = g_gameState->unk1C->party[D_801FC840.unk3];
    D_80077DE8.unk8 = NULL;
    if (D_801FC840.unk1 == 1) {
        buf = D_801FC740;
        D_80077DE8.unk8 = buf;
        func_8002BE44(0, D_801F8988);
        func_8002BD54(D_80077DE8.unk8, text_get(D_801FC840.unk0 + 3));
        func_8002AD38(D_80077DE8.unk8, text_get(7));
    } else if (D_801FC840.unk1 == 2) {
        id = c->equip[D_801FC840.unk2];
        if (id == 0xFF) {
            D_80077DE8.unk8 = text_get(6);
        } else {
            D_80077DE8.unk8 = D_801F8990 + D_80069018[id].unk2;
        }
    } else if (D_801FC840.unk1 == 3 && (l = &D_801FC840.unk4)->unk2 != 0) {
        e = D_801FC840.unk3C + (l->unk5 + l->cursor);
        if (e->unk1 != 0) {
            D_80077DE8.unk8 = D_801F8990 + D_80069018[e->unk0].unk2;
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
    func_8002DE60();
    func_80035674(1);
    func_8003556C(NULL);
}

void func_801F2C78(void) {
    void (*fn)(void);

    if (!func_8002F810()) {
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
    if (func_8003561C()) {
        func_801F2AB4();
    }
    if (D_801FC840.unk1 == 3 && D_801FC840.unk4.unk8 != 0) {
        func_8002EB74(&D_801FC840.unk4);
    }
    if (D_801FC840.unk23C.unkC != 0) {
        func_800631AC(&D_801FC840.unk23C);
    }
    if (D_801FC840.unk24C.unk0 != 0) {
        func_80028D2C(&D_801FC840.unk24C);
    }
}

void func_801F2DB4(void) {
    if (D_801FC840.unk4.unk8 == 0) {
        if (g_padRepeat & BTN_CIRCLE) {
            snd_playSe(0x65);
            func_801F307C();
            return;
        }
        if (g_padRepeat & BTN_CROSS) {
            func_801F30D8();
            return;
        }
        func_8002EE8C(&D_801FC840.unk4, g_padRepeat);
    }
}

void func_801F2E34(void) {
    RECT *r;
    s32 i;

    r = &D_801FC840.unk14;
    func_8002DEE4(r, 0x1E, 2);
    func_8002705C(r, 0x1E, 2);
    for (i = 0; i < 3; i++) {
        func_8002B5FC(r->x + D_801F7430[i], r->y + 6, 0x1D, text_get(i), 0);
    }
}

void func_801F2ED4(void) {
    MenuView v;
    s32 i;

    func_8002F344(&D_801FC840.unk4, &v);
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
    func_80026C34(x + 0xB, y, 0x13, p->unk0, 1);
    func_8002B5FC(x + 0x1B, y, 0x13, D_8006A218 + D_80069018[p->unk0].unk0, 0);
    text_formatNumber(p->unk1, buf, 2, 1);
    func_8002B56C(x + 0x8A, y + 4, 0x13, buf, 0);
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
    item = m->unk3C[l->unk5 + l->cursor].id;
    if (l->unk2 == 0) {
        snd_playSe(0x66);
        return;
    }
    if (old != 0xFF) {
        func_80027D84(old, 1);
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
            func_80027D84(old, 1);
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

    func_80028D08(&D_801FC840.unk24C, D_801FC840.unk3, slot, dir);
    if (dir != 0) {
        s = D_801FC840.unk1;
        D_801FC840.unk1 = 0;
        D_801FC840.unk261 = s;
    }
    D_801FC840.unk3 = slot;
}

void func_801F32E0(Character *c) {
    func_801F26C4(c);
    func_8003384C(c);
    c->unk34.b.unk4 = func_80033280(c->unk34.b.id, (c->unkA >> 4) & 1, c->equip);
}

void func_801F3344(s32 x, s32 y, s32 z, u8 id, s32 color) {
    func_8002B5FC(x, y - 4, z, text_get(id), color);
}

INCLUDE_RODATA("asm/us/ovl_33/nonmatchings/ovl_33", D_801EDF00);

INCLUDE_RODATA("asm/us/ovl_33/nonmatchings/ovl_33", D_801EDF04);

INCLUDE_RODATA("asm/us/ovl_33/nonmatchings/ovl_33", D_801EE164);

INCLUDE_RODATA("asm/us/ovl_33/nonmatchings/ovl_33", D_801EE644);

INCLUDE_RODATA("asm/us/ovl_33/nonmatchings/ovl_33", D_801EE648);
