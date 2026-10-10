#include "ovl_36.h"

INCLUDE_RODATA("asm/us/ovl_36/nonmatchings/ovl_36", D_801EDF00);

INCLUDE_RODATA("asm/us/ovl_36/nonmatchings/ovl_36", D_801EDF04);

INCLUDE_RODATA("asm/us/ovl_36/nonmatchings/ovl_36", D_801EDF08);

INCLUDE_RODATA("asm/us/ovl_36/nonmatchings/ovl_36", D_801EDF10);

INCLUDE_ASM("asm/us/ovl_36/nonmatchings/ovl_36", func_801F08F4);

void func_801F0AE8(void) {
    s32 cur;
    s32 next;
    s32 dir;

    cur = D_801F347C.unk2 * 10 + D_801F347C.unk1;
    if (D_80077528 & 0x10000) {
        func_8002F488(0x65);
        func_800361CC(0);
        return;
    }
    if (D_80077528 & 0x20000) {
        func_801F0CC8();
        return;
    }
    if (D_80077528 & 0xF0) {
        if ((D_80077528 & 0x10) && D_801F347C.unk1 != 0) {
            D_801F347C.unk1--;
        }
        if ((D_80077528 & 0x40) && D_801F347C.unk1 < 9) {
            D_801F347C.unk1++;
        }
        if ((D_80077528 & 0x80) && D_801F347C.unk2 != 0) {
            D_801F347C.unk2--;
        }
        if ((D_80077528 & 0x20) && D_801F347C.unk2 < 9) {
            D_801F347C.unk2++;
        }
        next = D_801F347C.unk2 * 10 + D_801F347C.unk1;
        if (next != cur) {
            func_8002F488(0x67);
            func_801F2C5C(next);
        }
    } else if (D_80077528 & 0x100000) {
        dir = 0;
        if (D_801F347C.unk8[cur] >= 2) {
            func_801F2FE0(cur, dir);
            func_8002F488(0x417);
        }
    } else if (D_80077528 & 0x200000) {
        dir = 1;
        if (D_801F347C.unk8[cur] >= 2) {
            func_801F2FE0(cur, dir);
            func_8002F488(0x417);
        }
    }
}

void func_801F0CC8(void) {
    if (D_801F347C.unk8[D_801F347C.unk2 * 10 + D_801F347C.unk1] != 0) {
        func_8002F488(0x67);
        D_801F347C.unk0 = 0;
        D_801F347C.unk5 = 1;
        D_801F347C.unk18E8 = func_8002DC54(&D_801F3100, 10, 2);
        func_8002E2EC(D_801F347C.unk18E8, func_801F2F34);
        func_8002E2E4(D_801F347C.unk18E8, func_801F2F14);
        func_8002E2FC(D_801F347C.unk18E8, func_801F2F24);
        func_8002E304(D_801F347C.unk18E8, D_800704C4);
        func_8002E31C(D_801F347C.unk18E8, 1);
        return;
    }
    func_8002F488(0x66);
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
    a = func_800625F8();
    b = func_80062638();
    func_8002B524(r->x + 8, r->y - 3, 0x1E, D_8007041C, 0);
    func_8002DEE4(r, 0x1E, 1);
    func_8002705C(r, 0x1E, 1);
    s = func_80035774(0x27);
    w = func_8002BA68(s);
    func_8002BAE4(a, buf, 3, 1);
    func_8002B5FC(r->x + 8, r->y + 0xA2, 0x14, s, 0);
    func_8002B5B4(r->x + (w + k), r->y + 0xA4, 0x14, buf, 0);
    s = func_80035774(0x28);
    w = func_8002BA68(s);
    func_8002BAE4(b, buf, 3, 1);
    func_8002B5B4(r->x + 0x80, r->y + 0xA4, 0x14, buf, 0);
    func_8002B5FC(r->x - (w - k2), r->y + 0xA2, 0x14, s, 0);
    for (n = 0, i = 0, x = 4; i < 10; i++, x += 15) {
        for (j = 0, y = 10; j < 10; j++, y += 15) {
            func_801F2860(r->x + x, r->y + y, 0x14, n);
            n++;
        }
    }
}

INCLUDE_ASM("asm/us/ovl_36/nonmatchings/ovl_36", func_801F0F9C);

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
        name = (u8 *)func_8006276C(id);
        if (cnt >= 2) {
            func_8002BAE4(D_801F347C.unk6C[id] + 1, buf, 3, 1);
            func_8002BAE4(cnt, buf + 4, -1, 1);
            buf[3] = '/';
            func_8002B56C(r->x + 0x2B, r->y + 0x43, 0x14, buf, 0);
            a = func_8002BE10(4);
            b = func_8002BE10(5);
            func_800275DC(r->x + 0x1E, r->y + 0x41, 0x14, a, 0);
            func_800275DC(r->x + 0x5E, r->y + 0x41, 0x14, b, 0);
        }
        i = 0;
        func_801F1D6C(r->x + 0x10, r->y + 0x50, 0x14, 0x37, 0x64808080);
        func_8002B5FC(r->x + 0x20, r->y + 0x50, 0x14, name, 0);

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
    func_8002DEE4(r, 0x1E, 2);
    func_8002705C(r, 0x1F, 2);
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
    setlen(D_80077170, 5);
    rgbc = (a * 8) | 0x48000000;
    ((PolyF4Words *)D_80077170)->rgbc = rgbc;
    ((PolyF4Words *)D_80077170)->xy0 = x | (y << 16);
    ((PolyF4Words *)D_80077170)->xy1 = (x + 13) | (y << 16);
    ((PolyF4Words *)D_80077170)->xy2 = (x + 13) | ((y + 13) << 16);
    ((PolyF4Words *)D_80077170)->xy3 = 0x55555555;
    addPrim(&D_8007716C[18], D_80077170);
    D_80077170 += 0x18;
    setlen(D_80077170, 5);
    ((PolyF4Words *)D_80077170)->rgbc = rgbc;
    ((PolyF4Words *)D_80077170)->xy0 = x | (y << 16);
    ((PolyF4Words *)D_80077170)->xy1 = x | ((y + 13) << 16);
    ((PolyF4Words *)D_80077170)->xy2 = (x + 13) | ((y + 13) << 16);
    ((PolyF4Words *)D_80077170)->xy3 = 0x55555555;
    addPrim(&D_8007716C[18], D_80077170);
    D_80077170 += 0x18;
}

void func_801F166C(void) {
    u8 blink = (D_80077DC0 & 3) != 0;

    if (!func_8002F810()) {
        if (D_801F347C.unk0 == 1 || (D_801F347C.unk0 == 2 && blink)) {
            D_80077DE8.unk4 = 0x11;
            D_80077DE8.unk0 = D_801F347C.unk18D0.x + D_801F347C.unk2 * 15 - 20;
            D_80077DE8.unk2 = D_801F347C.unk18D0.y + D_801F347C.unk1 * 15 + 14;
            func_800355E4();
        }
        if (D_801F347C.unk0 == 2) {
            Window *w = D_801F347C.unk18E8;
            D_80077DE8.unk0 = w->unk0 + 0x16;
            D_80077DE8.unk2 = w->unk2 + (s16)(D_801F347C.unk5 * 15 + 0x32);
            D_80077DE8.unk4 = 9;
            func_800355E4();
        }
    }
}

INCLUDE_ASM("asm/us/ovl_36/nonmatchings/ovl_36", func_801F1778);

INCLUDE_ASM("asm/us/ovl_36/nonmatchings/ovl_36", func_801F1A48);

void func_801F1AE8(s32 x, s32 y, s32 z, s32 dim) {
    SprtWords *p;
    u32 clut = (GetClut(0, 0x1F2) << 16) | 0xB000;
    s32 c;
    u32 rgbc;

    setlen(D_80077170, 4);
    c = 0x80 - dim * 0x16;
    rgbc = c | (c << 8) | (c << 16) | 0x64000000;
    p = (SprtWords *)D_80077170;
    p->rgbc = rgbc;
    p->wh = 0x2F0026;
    p->xy = (x + 2) | ((y + 2) << 16);
    p->uvclut = clut;
    D_80077170 = (u8 *)(p + 1);
    addPrim(&D_8007716C[z], p);
    SetDrawTPage(D_80077170, 1, 1, 0xBF);
    addPrim(&D_8007716C[z], D_80077170);
    D_80077170 += 8;
    func_801F1D6C(x, y, 0x14, 0, rgbc);
}

void func_801F1C88(void) {
    s32 i;
    u8 n;
    CardMenu *m;

    func_8003556C(func_800625F8() ? func_801F22DC : NULL);
    for (i = 0; i < 100; i++) {
        n = func_80062560(i);
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

    setlen(D_80077170, 4);
    p = (SprtWords *)D_80077170;
    t = &D_801F3188[idx];
    p->rgbc = rgbc;
    p->xy = x | (y << 16);
    p->uvclut = t->unk0;
    p->wh = t->unk4 | (t->unk5 << 16);
    D_80077170 = (u8 *)(p + 1);
    addPrim(&D_8007716C[z], p);
    SetDrawTPage(D_80077170, 1, 1, t->unk6 | 0x20);
    addPrim(&D_8007716C[z], D_80077170);
    D_80077170 += 8;
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

    e = func_80062B30();
    kinds = 0;
    arrows = 0;
    types = 0;
    for (i = 0; i < 100; i++) {
        if (D_801F347C.unk8[i] != 0) {
            kinds += e[i].unk4;
        }
    }
    func_8001D898(D_801F337C, 0, 0x100);
    for (i = 0; i < 100; i++) {
        for (j = 0; j < D_801F347C.unk8[i]; j++) {
            c = func_800625A0(i, j);
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

    D_80077DE8.unk8 = NULL;
    func_80035580(NULL);
    if (D_801F347C.unk0 == 1) {
        n = D_801F347C.unk8[D_801F347C.unk2 * 10 + D_801F347C.unk1];
        if (n != 0) {
            D_80077DE8.unk8 = func_80035774(n >= 2 ? 0x24 : 0x23);
        }
    } else if (D_801F347C.unk0 == 2) {
        if (D_801F347C.unk5 == 1) {
            r.x = 0;
            r.y = 0x8C;
            r.w = D_8006794C->unkC->disp.w;
            r.h = D_8006794C->unkC->disp.h - 0x8C;
            func_80035580(&r);
        }
        D_80077DE8.unk8 = func_80035774(D_801F347C.unk5 + 0x25);
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
        func_8002C870(0, id, D_801F347C.unk1948[i].unk0);
        func_8002C8AC(0, id, D_801F347C.unk1948[i].unk4);
    }
    func_8003556C(NULL);
    func_801F2EE8();
}

void func_801F24A0(void) {
    void (*f)();

    if (!func_8002F810() && D_801F347C.unk1941.unk0 == 0) {
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
    if (D_80077528 & 0x30000) {
        if ((D_80077528 & 0x20000) && D_801F347C.unk5 == 0) {
            func_8002F488(0x67);
            func_800626D0(cur, D_801F347C.unk6C[cur]);
            func_801F1C88();
        } else {
            func_8002F488(0x65);
        }
        D_801F347C.unk0 = 0;
        func_8002DD00(D_801F347C.unk18E8);
        return;
    }
    if (D_80077528 & 0x50) {
        func_8002F488(0x67);
        D_801F347C.unk5 ^= 1;
    }
}

INCLUDE_ASM("asm/us/ovl_36/nonmatchings/ovl_36", func_801F2664);

void func_801F2730(s32 x, s32 y, s32 z, s32 id, s32 dim, u8 flag) {
    Unk6 *card;
    s32 c;

    if (flag) {
        if (dim == 0) {
            card = func_800625A0(id, (s32)D_801F347C.unk6C[id] % (s32)D_801F347C.unk8[id]);
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
        icon = func_800627BC(id) + 0x1F;
    } else {
        icon = func_800627BC(id) + 0x26;
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
    func_8002B5FC(w->unk0 + 0xE, w->unk2 + 0xA, w->unk18, func_80035774(0x20), 0);
    func_8002B5FC(w->unk0 + 0x30, w->unk2 + 0x2E, w->unk18, func_80035774(0x21), 0);
    func_8002B5FC(w->unk0 + 0x30, w->unk2 + 0x3D, w->unk18, func_80035774(0x22), 0);
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
