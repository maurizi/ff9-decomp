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

INCLUDE_ASM("asm/us/ovl_36/nonmatchings/ovl_36", func_801F0DB0);

INCLUDE_ASM("asm/us/ovl_36/nonmatchings/ovl_36", func_801F0F9C);

INCLUDE_ASM("asm/us/ovl_36/nonmatchings/ovl_36", func_801F12B8);

INCLUDE_ASM("asm/us/ovl_36/nonmatchings/ovl_36", func_801F1508);

INCLUDE_ASM("asm/us/ovl_36/nonmatchings/ovl_36", func_801F166C);

INCLUDE_ASM("asm/us/ovl_36/nonmatchings/ovl_36", func_801F1778);

INCLUDE_ASM("asm/us/ovl_36/nonmatchings/ovl_36", func_801F1A48);

INCLUDE_ASM("asm/us/ovl_36/nonmatchings/ovl_36", func_801F1AE8);

INCLUDE_ASM("asm/us/ovl_36/nonmatchings/ovl_36", func_801F1C88);

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

INCLUDE_ASM("asm/us/ovl_36/nonmatchings/ovl_36", func_801F24A0);

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

INCLUDE_ASM("asm/us/ovl_36/nonmatchings/ovl_36", func_801F2860);

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

INCLUDE_ASM("asm/us/ovl_36/nonmatchings/ovl_36", func_801F2D88);

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

INCLUDE_ASM("asm/us/ovl_36/nonmatchings/ovl_36", func_801F2FE0);

INCLUDE_ASM("asm/us/ovl_36/nonmatchings/ovl_36", func_801F3034);

INCLUDE_ASM("asm/us/ovl_36/nonmatchings/ovl_36", func_801F30C8);

INCLUDE_RODATA("asm/us/ovl_36/nonmatchings/ovl_36", D_801EE1B4);

INCLUDE_RODATA("asm/us/ovl_36/nonmatchings/ovl_36", D_801EEFB4);

INCLUDE_RODATA("asm/us/ovl_36/nonmatchings/ovl_36", D_801F0274);
