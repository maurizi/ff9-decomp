#include "ovl_37.h"

INCLUDE_RODATA("asm/us/ovl_37/nonmatchings/ovl_37", D_801EDF00);

INCLUDE_RODATA("asm/us/ovl_37/nonmatchings/ovl_37", D_801EDF04);

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
    func_8003575C(D_801EDF04);
    func_8003556C(D_800355DC);
    func_80035674(1);
    func_8002EDFC(&D_801FAB78.unkC, 0xFF, 0xB, 2, 0xAD);
    func_8002EDFC(&D_801FAB78.unk1C, D_801FAB78.unkB0, 0xB, 2, 0xAD);
    setRECT(&D_801FAB78.unk2C, 8, 8, 0xE4, 0x1A);
    setRECT(&D_801FAB78.unk34, 8, 0x2A, 0x12C, 0xAD);
    func_80063118(&D_801FAB78.unk3C, 0, 0);
    func_801F2EDC();
}

void func_801F0D18(void) {
    if (D_80077528 & 0x10000) {
        func_8002F488(0x65);
        func_800361CC(0);
        return;
    }
    if (D_80077528 & 0x20000) {
        func_8002F488(0x67);
        if (D_801FAB78.unk0 != 1) {
            D_801FAB78.unk1 = 2;
            return;
        }
        func_801F2D58();
        return;
    }
    if (D_80077528 & 0x80) {
        func_8002F488(0x67);
        D_801FAB78.unk0 = D_801FAB78.unk0 != 0 ? D_801FAB78.unk0 - 1 : 2;
        return;
    }
    if (D_80077528 & 0x20) {
        func_8002F488(0x67);
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
        if (D_80077528 & 0x10000) {
            func_8002F488(0x65);
            func_801F2C58();
            return;
        }
        if (D_80077528 & 0x20000) {
            i = (l->unk5 + l->unk6) * 2 + l->unk7;
            func_801F2590(D_8006794C->unk1C->unk574[i].unk0);
            if (D_8006794C->unk1C->unk574[i].unk1 == 0) {
                func_801F2C58();
            }
        } else if (D_80077528 & 0x10) {
            cur = D_801FAB78.unk2;
            next = func_800337D0(cur);
            if (cur != next) {
                D_801FAB78.unk2 = next;
                func_8002F488(0x67);
            }
        } else if (D_80077528 & 0x40) {
            cur = D_801FAB78.unk2;
            next = func_8003380C(cur);
            if (cur != next) {
                D_801FAB78.unk2 = next;
                func_8002F488(0x67);
            }
        }
    }
}

void func_801F0F50(void) {
    MenuList *l = D_801F3040[1].unk8;
    UnkPair tmp;
    s32 i;
    u8 j;

    if (l->unk8 == 0) {
        if (D_80077528 & 0x10000) {
            func_8002F488(0x65);
            D_801FAB78.unk1 = 4;
            return;
        }
        if (D_80077528 & 0x20000) {
            if (D_801FAB78.unk7 == 0) {
                func_8002F488(0x67);
                j = l->unk7 + (l->unk6 + l->unk5) * 2;
                D_801FAB78.unk7 = 1;
                D_801FAB78.unk6 = j;
                return;
            }
            func_8002F488(0x67);
            i = (l->unk6 + l->unk5) * 2 + l->unk7;
            tmp = D_8006794C->unk1C->unk574[D_801FAB78.unk6];
            D_8006794C->unk1C->unk574[D_801FAB78.unk6] = D_8006794C->unk1C->unk574[i];
            D_8006794C->unk1C->unk574[i] = tmp;
            D_801FAB78.unk7 = 0;
            return;
        }
        func_8002EE8C(l, D_80077528);
        func_8002F044(l, D_80077528);
        func_8002F0EC(l, D_80077528);
    }
}

void func_801F10C4(s32 idx, MenuList *l) {
    UnkPair *p;
    Unk80069018 *it;
    Unk8006B1F4 *e;
    u8 ok;
    s32 id;

    p = &D_8006794C->unk1C->unk574[idx];
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
        func_8002F488(0x66);
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
    func_8002B524(D_801F30A8->x + 0xE, D_801F30A8->y - 3, 0x1D, D_800703FC.unk68, 0);
    func_8002B524(D_801F30A8->x + 0xA4, D_801F30A8->y - 3, 0x1D, D_800703FC.unk68, 0);
    if (D_801FAB78.unk0 != 2) {
        w = func_8002AE7C((u8 *)D_800703FC.unk94);
        func_8002B524((D_801F30A8->x + 0x8A) - w, D_801F30A8->y - 3, 0x1D, D_800703FC.unk94, 0);
        func_8002B524((D_801F30A8->x + 0x120) - w, D_801F30A8->y - 3, 0x1D, D_800703FC.unk94, 0);
    }
    func_8002ECA0(il->unk8, &view);
    func_8002E040(D_801F30A8, 0x1D, 1, il->unk8);
    func_8002705C(D_801F30A8, 0x1D, 1);
    idx = view.unk1 * 2;
    func_8002D9B4(NULL, 0x1C, 1);
    for (row = 0; row < view.unk0; row++) {
        for (col = 0; col < 2; col++) {
            il->unk4(col, row, idx++, &view);
        }
    }
    if (il->unk8->unk8 & 4) {
        func_8002F40C(il->unk8, D_801F30A8, &D_801FA958);
        func_8002D9B4(&D_801FA958, 0x1C, 1);
        func_801F26E0();
    }
    func_8002D9B4(D_801F30A8, 0x1C, 1);
}

INCLUDE_ASM("asm/us/ovl_37/nonmatchings/ovl_37", func_801F13F8);

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
    gs = D_8006794C;
    m = &D_801FAB78;
    m->unk2 = 0;
loop:
    if (gs->unk1C->unk54C[m->unk2] == NULL) {
        if (++m->unk2 < 4) {
            goto loop;
        }
    }
    m->unk1 = 0;
    m->unk8 = 1;
    func_8002F488(0x67);
}

INCLUDE_ASM("asm/us/ovl_37/nonmatchings/ovl_37", func_801F187C);

s32 func_801F1A7C(void) {
    ItemMenu *m = &D_801FAB78;

    if (D_8006794C->unk8 == 3 && D_8007AEF0.b[0xBF] != 0 && D_8007AEF0.b[0xB5] == 0) {
        if (func_800B8124()) {
            D_8007AEF0.b[0xB5] = 1;
            func_800B81A0();
            m->unk4C = func_8002DC54(&D_801F3148, 0xF, 2);
            func_8002E2E4(m->unk4C, func_801F2B20);
            func_8002E2EC(m->unk4C, func_801F2B58);
            func_8002E304(m->unk4C, (s32)D_801F3140);
            func_8002E31C(m->unk4C, 1);
            m->unk50 = 1;
            m->unk1 = 0;
            func_8003563C(1);
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
    Unk80077DE8 *h;

    h = &D_80077DE8;
    x = h->unk0;
    y = h->unk2;
    z = h->unk4;
    h->unk8 = NULL;
    h->unk0 = x;
    h->unk2 = y;
    h->unk4 = z;
    func_80035580(NULL);
    if (D_801FAB78.unk1 == 1) {
        h->unk8 = func_80035774(D_801FAB78.unk0 + 5);
    }
    if (D_801FAB78.unk1 == 2) {
        if (D_801FAB78.unk0 != 0) {
            i = func_801F2FC4(&D_801FAB78.unk1C);
            if (i < D_801FAB78.unkB0) {
                h->unk8 = &D_801F651C[D_8006B374[D_801FAB78.unk60[i]].unk2];
            }
            return;
        }
        l = &D_801FAB78.unkC;
    } else if (D_801FAB78.unk1 == 3) {
        if (func_801F2FC4(&D_801FAB78.unkC) & 1) {
            r.x = 0xC;
            r.y = 0;
            r.w = D_8006794C->unkC->disp.w;
            r.h = D_8006794C->unkC->disp.h;
        } else {
            r.x = 0;
            r.y = 0;
            r.w = D_8006794C->unkC->disp.w >> 1;
            r.h = D_8006794C->unkC->disp.h;
        }
        func_80035580(&r);
        D_80077DE8.unk8 = func_80035774(8);
        return;
    } else if (D_801FAB78.unk1 == 4) {
        func_80035580(&D_801F3150);
        h->unk8 = func_80035774(D_801FAB78.unk5 + 9);
        return;
    } else if (D_801FAB78.unk1 == 5) {
        l = D_801F3040[1].unk8;
    } else {
        return;
    }
    p = &D_8006794C->unk1C->unk574[func_801F2FC4(l)];
    if (p->unk1 != 0) {
        h->unk8 = &D_801F3168[D_80069018[p->unk0].unk2];
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
                func_80028128(D_801FAB78.unk56);
                func_8003563C(0);
            }
        }
    }
}

INCLUDE_ASM("asm/us/ovl_37/nonmatchings/ovl_37", func_801F1E88);

void func_801F2160(void) {
    func_801F21E8();
    func_801F2240();
    func_801F2280();
}

void func_801F2190(void) {
    if (D_8006794C->unk8 == 3) {
        func_8002F55C(0x44A);
    }
    func_8003563C(0);
    func_80035674(1);
    func_8003556C(NULL);
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
        if (D_80077528 & 0x10000) {
            func_8002F488(0x65);
            D_801FAB78.unk1 = 1;
            return;
        }
        if (D_80077528 & 0x20000) {
            func_801F24DC();
            return;
        }
        if (l->unk0 != 0) {
            func_8002EE8C(l, D_80077528);
            if (l->unk8 == 0) {
                func_8002F044(l, D_80077528);
                if (l->unk8 == 0) {
                    func_8002F0EC(l, D_80077528);
                }
            }
        }
    }
}

void func_801F23C4(void) {
    if (D_80077528 & 0x10000) {
        func_8002F488(0x65);
        func_801F2E18();
        return;
    }
    if (D_80077528 & 0x50) {
        func_8002F488(0x67);
        D_801FAB78.unk5 ^= 1;
        return;
    }
    if (D_80077528 & 0x20000) {
        if (D_801FAB78.unk5 == 0) {
            func_8002F488(0x67);
            func_801F187C();
            return;
        }
        func_8002F488(0x67);
        D_801FAB78.unk7 = 0;
        D_801FAB78.unk1 = 5;
    }
}

void func_801F2488(void) {
    if (D_80077528 & 0x10000) {
        func_8002F488(0x65);
        func_801F2FA0();
    } else if (D_80077528 & 0x20000) {
        func_8002F488(0x67);
        func_801F2FA0();
    }
}

INCLUDE_ASM("asm/us/ovl_37/nonmatchings/ovl_37", func_801F24DC);

void func_801F253C(s32 i) {
    if (i < D_801FAB78.unkB0) {
        func_801F2F38(D_801FAB78.unk60[i]);
        func_8002F488(0x67);
    } else {
        func_8002F488(0x66);
    }
}

void func_801F2590(s32 id) {
    Unk8006B1F4 *e;
    Character *c;
    s32 i;

    i = id - 0xE0;
    e = &D_8006B1F4[i];
    c = D_8006794C->unk1C->unk54C[D_801FAB78.unk2];
    if (func_800654F8(c, c, e, e->unk4, 0)) {
        func_8002F488(0x6A);
        func_80027E5C(id, 1);
    } else {
        func_8002F488(0x66);
    }
}

void func_801F262C(void) {
    RECT *r;
    s32 i;

    r = &D_801FAB78.unk2C;
    for (i = 0; i < 3; i++) {
        func_8002B5FC(r->x + D_801F309C[i], r->y + 6, 0x1C, func_80035774(D_801F3098[i]), 0);
    }
    func_8002DEE4(r, 0x1D, 2);
    func_8002705C(r, 0x1D, 2);
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
            if (D_8006794C->unk1C->unk54C[i] != NULL) {
                func_80026EA0(r->x + 8, r->y + 6 + i * 0x2B, 0x14, i, D_801FAB78.unk4);
            }
        }
        func_8002DEE4(r, 0x14, 2);
        func_8002705C(r, 0x14, 2);
    }
}

void func_801F2870(s32 col, s32 row, s32 i, MenuView *v) {
    UnkPair *p = &D_8006794C->unk1C->unk574[i];
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
        func_8002BAE4(p->unk1, buf, 2, 1);
        x2 = x + 0x18;
        func_8002B5FC(x + 0x28, y, 0x1C, D_8006A218 + it->unk0, ok ? 0 : 2);
        func_80026C34(x2, y, 0x1C, p->unk0, ok);
        func_8002B56C(x2 + 0x6C, y + 4, 0x1C, buf, ok ? 0 : 2);
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
        got = func_800281D0(id);
        x = col * 0x96 + 0x18;
        y = row * 0xF + (v->unk2 + k);
        name = D_8006ADAC + t->unk0;
        if (!got) {
            w = x + func_8002BA68((s32)name);
            func_800275DC(w + 5, y + 2, 0x1C, 0xB6, blink);
            func_800275DC(w - 1, y - 1, 0x1C, 0xB5, 0);
        }
        func_8002B5FC(x, y, 0x1C, name, 0);
    }
}

void func_801F2B20(Window *w) {
    if (D_8006794C->unk8 == 3) {
        func_8002F514(0x44A);
    }
}

void func_801F2B58(Window *w) {
    u8 *s;

    s = func_80035774(0xB);
    func_8002B5FC(w->unk0 + ((w->unk4 - func_8002BA68((s32)s)) >> 1), w->unk2 + 6, w->unk18, s, 0);
}

void func_801F2BC0(void) {
    u32 t;

    if (D_801FAB78.unk1 == 0) {
        t = D_801FAB78.unk50;
        if (t != 0) {
            D_801FAB78.unk50 = t + 1;
            if (t >= 0x60) {
                D_801FAB78.unk50 = 0;
                if (D_8006794C->unk8 == 3) {
                    func_8002F55C(0x44A);
                }
                func_8003563C(0);
                func_80035674(1);
                func_8003556C(NULL);
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
        if ((u8)func_800281A4(i)) {
            D_801FAB78.unk60[n] = i;
            n++;
        }
    }
    D_801FAB78.unkB0 = n;
}

void func_801F2D58(void) {
    MenuList *l;

    l = D_801F3040[1].unk8;
    D_801FAB78.unk4C = func_8002DC54(&D_801F30B8, 0x14, 2);
    func_8002E2EC(D_801FAB78.unk4C, func_801F2E40);
    func_8002E2E4(D_801FAB78.unk4C, func_801F2EBC);
    func_8002E2FC(D_801FAB78.unk4C, func_801F2ECC);
    func_8002E31C(D_801FAB78.unk4C, 1);
    func_8002E304(D_801FAB78.unk4C, D_80070408);
    D_801FAB78.unk5 = 0;
    D_801FAB78.unk6 = l->unk7 + (l->unk5 + l->unk6) * 2;
    D_801FAB78.unk1 = 0;
}

void func_801F2E18(void) {
    func_8002DD00(D_801FAB78.unk4C);
}

void func_801F2E40(Window *w) {
    Window *p = w;
    s32 i;

    for (i = 0; i < 2; i++) {
        func_8002B5FC(p->unk0 + 10, p->unk2 + 6 + i * 15, w->unk18, func_80035774(i + 3), 0);
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
    func_8003563C(1);
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
        r = l->unk6;
        if (l->unk8 & 2) {
            return (l->unk5 + (r - step)) * 2 + t;
        }
        return (l->unk5 + (r + step)) * 2 + t;
    }
    return (l->unk5 + l->unk6) * 2 + l->unk7;
}

void func_801F3038(void) {
}

INCLUDE_RODATA("asm/us/ovl_37/nonmatchings/ovl_37", D_801EDFD8);
