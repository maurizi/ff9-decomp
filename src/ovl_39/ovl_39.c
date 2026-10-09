#include "ovl_39.h"

void func_801F4414(void) {
    s32 i;

    D_801FB1A8.unk0 = D_80077DB0.unkC;
    D_801FB1A8.unk14 = func_8002F8E4(7);
    D_801FB1A8.unk1 = 1;
    D_801FB1A8.unk2 = 2;
    D_801FB1A8.unk7 = 0;
    D_801FB1A8.unk8 = 1;
    D_801FB1A8.unkC = 0;
    D_801FB1A8.unk10 = 0;
    D_80077DB0.unk2 &= 1;
    func_801F6BD4();
    func_801F770C();
    func_801F734C();
    func_801F61B0();
    func_801F830C();
    func_801F7928();
    func_801F5BE0();
    func_8003556C(func_801F500C);
    func_8003575C(D_801EDF1C);
    D_801FB1A8.unk20 = func_80035774(0x16);
    D_801FB1A8.unk4[0] = 1;
    D_801FB1A8.unk4[1] = 1;
    func_801FA0C8();
    func_801FAC68();
    func_801F5A60();
    for (i = 0; i < 2; i++) {
        D_801FB1A8.unk18[i] = D_80073998[i];
        D_80073998[i] |= 0x300;
    }
}

INCLUDE_ASM("asm/us/ovl_39/nonmatchings/ovl_39", func_801F4538);

void func_801F4664(void) {
    if (D_80077528 & 0x10000) {
        func_8002F488(0x65);
        if (D_80077DB0.unk1 != 0) {
            func_801F53B0();
            return;
        }
        func_800361CC(0);
        return;
    }
    if (D_80077528 & 0x10) {
        func_8002F488(0x67);
        D_80077DB0.unk2 = D_80077DB0.unk2 == 0 ? 1 : D_80077DB0.unk2 - 1;
    }
    if (D_80077528 & 0x40) {
        func_8002F488(0x67);
        D_80077DB0.unk2 = D_80077DB0.unk2 != 0 ? 0 : D_80077DB0.unk2 + 1;
    }
    if (D_80077528 & 0x20000) {
        func_801F58E0();
    }
}

void func_801F4760(void) {
    s32 i;
    s32 ok;
    u8 *str;
    u8 s;

    func_8002DEE4(&D_801FAFE0, 0x34, 2);
    func_8002705C(&D_801FAFE0, 0x34, 2);
    for (i = 0; i < 2; i++) {
        str = func_80035774(i + 3);
        ok = 0;
        s = D_801FB1A8.unk4[i];
        if (s == 0 || s == 3) {
            ok = 1;
        }
        func_8002B5FC(D_801FAFE0.x + 10, D_801FAFE0.y + 10 + i * 18, 0x33, str, (ok ^ 1) * 2);
    }
}

INCLUDE_ASM("asm/us/ovl_39/nonmatchings/ovl_39", func_801F4840);

INCLUDE_RODATA("asm/us/ovl_39/nonmatchings/ovl_39", D_801EDF00);

INCLUDE_ASM("asm/us/ovl_39/nonmatchings/ovl_39", func_801F49C4);

INCLUDE_ASM("asm/us/ovl_39/nonmatchings/ovl_39", func_801F4B18);

INCLUDE_ASM("asm/us/ovl_39/nonmatchings/ovl_39", func_801F4E00);

void func_801F4F30(void) {
    MenuList *l = &D_801FB1A8.unk24;
    s32 v;
    s32 dec;

    if (D_801FB1A8.unk4[D_80077DB2] != 0) {
        func_801F77A8(0x2D);
        return;
    }
    if (l->unk8 != 0) {
        if (!(l->unk8 & 4)) {
            dec = l->unk8 & 2;
            l->unk9 += 6;
            if (l->unk9 >= 0x3C) {
                v = l->unk5;
                if (dec) {
                    v--;
                } else {
                    v++;
                }
                l->unk5 = v;
                l->unk8 = 0;
                l->unk9 = 0;
            }
        } else {
            l->unk9 += 9;
            if (l->unk9 >= 0xB4) {
                l->unk8 = 0;
                l->unk9 = 0;
                l->unk5 = l->unkC;
            }
        }
    }
}

void func_801F500C(void) {
    RECT r = D_801EDF14;
    u8 st;
    s32 v;

    D_80077DE8.unk8 = NULL;
    if (D_801FB1A8.unk2 == 2 && D_801FB1A8.unk8 != 0) {
        func_80035580(NULL);
        st = D_801FB1A8.unk1;
        if (st == 1) {
            func_80035580(&r);
            v = 5;
            if (D_801FB1A8.unk0 == st) {
                v = 7;
            }
            D_80077DE8.unk8 = func_80035774(v + D_80077DB2);
        } else if (st == 4) {
            func_801F5114();
        } else if (st == 3) {
            D_80077DE8.unk8 = func_801F6320();
        }
    }
}

INCLUDE_ASM("asm/us/ovl_39/nonmatchings/ovl_39", func_801F5114);

void func_801F5280(void) {
    func_801F53E0();
    func_801F4538();
    func_801F54C8();
    func_801F5460();
    func_801F5550();
}

void func_801F52C0(void) {
    s32 i;

    func_8003563C(0);
    func_8003556C(NULL);
    func_801FAECC();
    func_801F5D00();
    func_801F7950();
    for (i = 0; i < 2; i++) {
        D_80073998[i] = D_801FB1A8.unk18[i];
    }
}

u8 func_801F5330(void) {
    return D_801FB1A8.unk7;
}

u8 func_801F5340(s32 slot, u8 any) {
    s32 st = D_801FB1A8.unk4[slot];
    s32 r;

    if (any) {
        r = 0;
        if (st == 0 || st == 3) {
            r = 1;
        }
        return r;
    }
    return st == 0;
}

void func_801F5384(void) {
    func_8002F9F0();
    func_8002F8A8(func_801F5A24);
}

void func_801F53B0(void) {
    func_8002F8E4(D_801FB1A8.unk14);
    func_800325E0();
}

void func_801F53E0(void) {
    func_8002DEE4(&D_801FAF74, 5, 2);
    func_8002705C(&D_801FAF74, 5, 2);
    func_8002B5FC(D_801FAF74.x + 0xA, D_801FAF74.y + 6, 4, (u8 *)func_800357C0(D_801EDF1C, D_801FB1A8.unk0), 0);
}

void func_801F5460(void) {
    void (*fn)(void) = D_801FAFA8[D_801FB1A8.unk1];

    if (fn != NULL) {
        fn();
    }
    if (D_801FB1A8.unk8 != 0) {
        func_801F4E00();
    }
}

void func_801F54C8(void) {
    if (!func_8002F810()) {
        switch (D_801FB1A8.unk2) {
        case 1:
            func_801F7718();
            break;
        case 2:
            if (D_801FAF8C[D_801FB1A8.unk1] != NULL) {
                D_801FAF8C[D_801FB1A8.unk1]();
            }
            break;
        }
    }
}

void func_801F5550(void) {
    func_801FA794();
    D_801FB1A8.unk4[0] = func_801FA21C(0);
    D_801FB1A8.unk4[1] = func_801FA21C(0x10);
    if (D_801FB1A8.unk2 == 2) {
        if (D_801FAFC4[D_801FB1A8.unk1] != NULL) {
            D_801FAFC4[D_801FB1A8.unk1]();
        }
    }
    func_801F5DE0();
    func_801F59B8();
}

void func_801F55D8(void) {
    MenuList *l = &D_801FB1A8.unk24;

    if (l->unk8 == 0) {
        if (D_80077528 & 0x10000) {
            func_8002F488(0x65);
            func_801F5384();
            return;
        }
        if (D_80077528 & 0x20000) {
            func_801F5960();
            return;
        }
        func_8002EEF0(l, D_80077528);
        func_8002EF58(l, D_80077528);
        func_801F8C34(l, D_80077528);
        func_801F8CE8(l, D_80077528);
    }
}

void func_801F5688(void) {
    MenuView v;
    RECT r;
    s32 i;

    func_801F8B10(&D_801FB1A8.unk24, &v);
    r.x = 8;
    r.w = 0x12C;
    r.y = 0;
    r.h = 0x3C;
    for (i = 0; i < v.unk0; i++) {
        r.y = i * 0x3C + (s16)(v.unk2 + 0x2A);
        func_801F49C4(&r, 0x2D, v.unk1 + i);
        func_8002DEE4(&r, 0x2D, 0);
    }
}

void func_801F573C(void) {
    s32 blink = (((u32)D_80077DC0 >> 4) ^ 1) & 1;
    MenuList *l = &D_801FB1A8.unk24;

    func_800275DC(0x12E, 0x2C, 0x2A, 0xB8, l->unk5 != 0 ? blink : 1);
    func_800275DC(0x12E, 0xCF, 0x2A, 0xB9, l->unk5 < l->unk2 - l->unk3 ? blink : 1);
}

void func_801F57E4(RECT *r, s32 otz) {
    func_8002B5FC(r->x + 0x10, r->y + 0x16, otz, func_80035774(0x13), 0);
}

void func_801F5838(RECT *r, s32 otz) {
    func_8002B5FC(r->x + 0x10, r->y + 0x16, otz, func_80035774(0x14), 0);
}

void func_801F588C(RECT *r, s32 otz) {
    func_8002B5FC(r->x + 0x10, r->y + 0x16, otz, func_80035774(0x15), 0);
}

INCLUDE_ASM("asm/us/ovl_39/nonmatchings/ovl_39", func_801F58E0);

void func_801F5960(void) {
    s32 i = D_801FB1A8.unk24.unk5 + D_801FB1A8.unk24.unk6;

    if (D_801FB1A8.unk0 == 1) {
        func_801F9788(i, i);
        return;
    }
    func_801F94D0(i, i);
}

void func_801F59B8(void) {
    u8 hide;

    hide = D_801FB1A8.unk7 != 0;
    if (D_801FB1A8.unk2 != 2) {
        hide = 1;
    }
    if (D_801FB1A8.unk1 != 1 && D_801FB1A8.unk1 != 4 && D_801FB1A8.unk1 != 3) {
        hide = 1;
    }
    func_8003563C(hide);
}

void func_801F5A24(void) {
    D_801FB1A8.unk1 = 1;
    D_801FB1A8.unk20 = func_80035774(0x16);
    func_8002F9D0();
}

void func_801F5A60(void) {
}

INCLUDE_ASM("asm/us/ovl_39/nonmatchings/ovl_39", func_801F5A68);

void func_801F5BE0(void) {
    RECT r;
    u8 *img = D_801F2854;
    u8 *buf = D_801FBACC;
    u8 *src = img;
    s32 i;

    r.y = 0xE0;
    r.x = 0;
    r.w = 0x100;
    r.h = 1;
    StoreImage(&r, (u32 *)buf);
    DrawSync(0);
    LoadImage(&r, (u32 *)img);
    DrawSync(0);
    buf += 0x200;
    img += 0x200;
    r.x = 0x40;
    r.w = 0x58;
    r.y = 0x100;
    r.h = 3;
    for (i = 0; i < 11; i++) {
        if (i == 0) {
            StoreImage(&r, (u32 *)buf);
        } else {
            StoreImage(&r, (u32 *)src);
            src += 0x210;
        }
        DrawSync(0);
        LoadImage(&r, (u32 *)img);
        DrawSync(0);
        r.y += 3;
        img += 0x210;
    }
}

void func_801F5D00(void) {
    RECT r;
    u8 *img;
    u8 *buf;
    s32 i;

    img = D_801F2854;
    buf = D_801FBACC;
    r.x = 0;
    r.y = 0xE0;
    r.w = 0x100;
    r.h = 1;
    LoadImage(&r, (u32 *)buf);
    DrawSync(0);
    buf += 0x200;
    r.x = 0x40;
    r.w = 0x58;
    r.y = 0x100;
    r.h = 3;
    for (i = 0; i < 11; i++) {
        if (i == 0) {
            LoadImage(&r, (u32 *)buf);
        } else {
            LoadImage(&r, (u32 *)img);
            img += 0x210;
        }
        DrawSync(0);
        r.y += 3;
    }
}

void func_801F5DE0(void) {
    if (D_801FBAC8.unk0 != 0) {
        if (++D_801FBAC8.unk1 >= 8) {
            D_801FBAC8.unk1 = 0;
            if (++D_801FBAC8.unk2 >= 4) {
                D_801FBAC8.unk2 = 0;
            }
        }
    }
}

void func_801F5E40(void) {
    D_801FBAC8.unk2 = 0;
    D_801FBAC8.unk1 = 0;
}

u8 func_801F5E54(u8 v) {
    u8 old = D_801FBAC8.unk0;

    D_801FBAC8.unk0 = v;
    return old;
}

void func_801F5E68(s32 x, s32 y, s32 otz) {
    func_801F5A68(x, y, otz, D_801FBAC8.unk2);
}

INCLUDE_ASM("asm/us/ovl_39/nonmatchings/ovl_39", func_801F5E90);

void func_801F5FE4(Window *w) {
    u32 a = func_8003579C(0x21);
    u32 b = func_8003579C(0x22) & 0xFFF;
    s32 d1 = 0x26;
    s32 d2 = 0x17;
    s16 m;

    if (b < (a & 0xFFF)) {
        b = a & 0xFFF;
    }
    func_8002B5FC(w->unk0 + 8, w->unk2 + 8, w->unk18, (u8 *)D_801FBEE0.unk4, 0);
    m = b;
    func_8002B5FC(w->unk0 + ((s32)(w->unk4 - m) >> 1), (w->unk2 - d1) + w->unk6, w->unk18, func_80035774(0x21), 0);
    func_8002B5FC(w->unk0 + ((s32)(w->unk4 - m) >> 1), (w->unk2 - d2) + w->unk6, w->unk18, func_80035774(0x22), 0);
}

void func_801F60DC(Window *w) {
    u32 a = func_8003579C(0x21);
    u32 b = func_8003579C(0x22) & 0xFFF;
    s32 z;

    if (b < (a & 0xFFF)) {
        b = a & 0xFFF;
    }
    if (w->unk1C == 3) {
        D_80077DE8.unk0 = w->unk0 + ((s32)(w->unk4 - b) >> 1) - 0x1A;
        D_80077DE8.unk2 = w->unk2 + (s16)(w->unk6 + (s16)(D_801FBEE0.unk1 * 15 - 0x26)) + 3;
        z = w->unk18;
        D_80077DE8.unk4 = z;
        func_800270EC(D_80077DE8.unk0, D_80077DE8.unk2, z - 1);
    }
}

void func_801F61B0(void) {
    D_801FBEE0.unk0 = 0;
}

void func_801F61BC(void) {
    if (D_801FBEE0.unk8->unk1C == 3) {
        if (D_80077528 & 0x20000) {
            func_8002F488(0x67);
            func_8002DD00(D_801FBEE0.unk8);
            D_801FB1A8.unk2 = 0;
            return;
        }
        if (D_80077528 & 0x10000) {
            func_8002F488(0x65);
            D_801FBEE0.unk1 = 1;
            func_8002DD00(D_801FBEE0.unk8);
            D_801FB1A8.unk2 = 0;
            return;
        }
        if (D_80077528 & 0x50) {
            func_8002F488(0x67);
            D_801FBEE0.unk1 ^= 1;
        }
    }
}

void func_801F6274(void) {
    u8 st = D_801FB1A8.unk4[D_80077DB0.unk2];

    if (st != 0 && st != 3) {
        func_8002E2FC(D_801FBEE0.unk8, NULL);
        func_8002DD00(D_801FBEE0.unk8);
        D_801FBEE0.unk0 = 0;
        D_801FBEE0.unk8 = NULL;
        func_801F77A8(0x2D);
    }
}

void func_801F62F0(void (*v)(Unk801FBEE0 *)) {
    D_801FBEE0.unkC = v;
}

u8 func_801F62FC(void) {
    return D_801FBEE0.unk0;
}

u8 func_801F630C(u8 v) {
    u8 old = D_801FBEE0.unk0;

    D_801FBEE0.unk0 = v;
    return old;
}

u8 *func_801F6320(void) {
    return func_80035774(D_801FBEE0.unk1 == 0 ? D_801FBEE0.unk10 : D_801FBEE0.unk12);
}

void func_801F6368(void) {
    D_801FB1A8.unk2 = 2;
}

void func_801F6378(void) {
    D_801FB1A8.unk1 = 4;
    if (D_801FBEE0.unk1 == 0) {
        D_801FBEE0.unkC(&D_801FBEE0);
    }
    D_801FBEE0.unk0 = 0;
    D_801FB1A8.unk2 = 2;
}

INCLUDE_ASM("asm/us/ovl_39/nonmatchings/ovl_39", func_801F63D8);

void func_801F65C4(void) {
    s32 r;

    r = func_801FAAE4(D_80077DB2 << 4);
    D_801FBEF8.unk3++;
    D_801FBEF8.unk4[r]++;
    if (D_801FBEF8.unk3 == D_801FBEF8.unk4[r]) {
        if (D_801FBEF8.unk3 < D_801FB02C[r]) {
            func_801F6E68(r);
            return;
        }
        switch (r) {
        case 0:
            func_801F6DAC();
            return;
        case 3:
            func_801F6E1C();
            return;
        }
    }
    func_801F72B4(0x2E);
}

INCLUDE_ASM("asm/us/ovl_39/nonmatchings/ovl_39", func_801F6694);

INCLUDE_ASM("asm/us/ovl_39/nonmatchings/ovl_39", func_801F6938);

void func_801F6A84(void) {
    D_801FB1A8.unk2 = 2;
    D_801FB1A8.unk1 = 4;
    D_801FB1A8.unk20 = func_80035774(0x1A);
    func_8002DE10(D_801FBEF8.unk1C);
    func_8002EDFC(&D_801FB1A8.unk24, 0xF, 3, 1, 0xB4);
    if (D_801FB1A8.unk34[D_801FB1A8.unk10].unk1A == 4) {
        D_801FB1A8.unk24.unk6 = D_801FB1A8.unk34[D_801FB1A8.unk10].unk44;
        D_801FB1A8.unk24.unk5 = D_801FB1A8.unk10 - D_801FB1A8.unk24.unk6;
    }
    func_8002F9D0();
    func_8002F8A8(func_801F70E4);
}

void func_801F6B48(void) {
    s32 i;

    D_801FB1A8.unkC = 0;
    D_801FB1A8.unk10 = 0;
    for (i = 0; i < 15; i++) {
        if (D_801FB1A8.unk34[i].unk1A == 4) {
            if (D_801FB1A8.unk34[i].unk0 != 0x30005) {
                D_801FB1A8.unk34[i].unk1A = 3;
            } else if (D_801FB1A8.unkC < D_801FB1A8.unk34[i].unk4C) {
                D_801FB1A8.unkC = D_801FB1A8.unk34[i].unk4C;
                D_801FB1A8.unk10 = i;
            }
        }
    }
}

void func_801F6BD4(void) {
    D_801FBEF8.unk0 = 0;
}

void func_801F6BE0(void) {
    void (*fn)(void);

    if (!func_801F5340(D_80077DB2, 1)) {
        func_801F72B4(0x2D);
        return;
    }
    fn = D_801FB014[D_801FBEF8.unk1];
    if (fn != NULL) {
        fn();
    }
}

u8 func_801F6C50(void) {
    return D_801FBEF8.unk0;
}

u8 func_801F6C60(u8 v) {
    u8 old = D_801FBEF8.unk0;

    D_801FBEF8.unk0 = v;
    return old;
}

s32 func_801F6C74(void) {
    s32 i;

    for (i = 0; i < 15; i++) {
        if (D_801FB1A8.unk34[14 - i].unk1A == 1) {
            return 14 - i;
        }
    }
    return -1;
}

s32 func_801F6CD0(s32 n) {
    s32 found = 0;
    s32 i;

    for (i = 0; i < 15; i++) {
        if (D_801FB1A8.unk34[i].unk1A == 4) {
            found++;
            if (n < found) {
                return i;
            }
        }
    }
    return -1;
}

s32 func_801F6D24(Unk801F6D24 *p) {
    return (p->unk12 - '0') * 10 + (p->unk13 - '0');
}

void func_801F6D4C(void) {
    if (func_801FA2D8() == 0) {
        D_801FBEF8.unk1++;
        if (func_801FA350(D_80077DB2 * 0x10, func_801F65C4) == 0) {
            func_801F72B4(0x2E);
        }
    }
}

void func_801F6DAC(void) {
    D_801FB1A8.unk6 = 1;
    if (func_801F6694()) {
        if (D_801FBEF8.unk14 < D_801FBEF8.unk10) {
            D_801FBEF8.unk1 = 3;
            func_801F7108(func_801F6CD0(D_801FBEF8.unk14));
            return;
        }
        func_801F7084();
    }
}

void func_801F6E1C(void) {
    D_801FB1A8.unk6 = 0;
    if (D_801FB1A8.unk0 == 1) {
        func_801F7300(0x30);
        return;
    }
    func_801F7084();
}

void func_801F6E68(s32 mode) {
    if (mode == 3) {
        D_801FBEF8.unk1 = 1;
        if (func_801FA2E8(D_80077DB0.unk2 * 0x10) == 0) {
            return;
        }
    } else if (func_801FA350(D_80077DB0.unk2 * 0x10, func_801F65C4) != 0) {
        return;
    }
    func_801F72B4(0x2E);
}

void func_801F6EDC(Window *w) {
    func_801F5E68(w->unk0 + 0xE, w->unk2 + 0x12, w->unk18);
    func_8002B5FC(w->unk0 + 0x4A, w->unk2 + 0xE, w->unk18, func_80035774(0x12), 0);
}

void func_801F6F3C(void) {
    D_801FB1A8.unk2 = 2;
    D_801FB1A8.unk20 = func_80035774(0x17);
}

void func_801F6F74(void) {
}

s32 func_801F6F7C(void) {
    if (D_801FB1A8.unk0 != 1) {
        if (D_801FBEF8.unkC >= 0xF) {
            func_801F7300(0x2F);
            return 0;
        }
    } else if (D_801FBEF8.unk10 == 0) {
        func_801F7300(0x30);
        return 0;
    }
    return 1;
}

s32 func_801F6FE8(void) {
    s32 port;
    s32 n;
    s32 prev;
    s32 r;

    prev = 0;
    n = 0;
    port = D_80077DB2 << 4;
    do {
        r = func_801FA1C4(port, (s32)&D_801FBF18);
        if (n != 0 && prev != r) {
            goto fail;
        }
        n++;
        prev = r;
    } while (n < 3);
    if (func_801FA240(port) == 0) {
        return r;
    }
fail:
    func_801F72B4(0x2E);
    return -1;
}

void func_801F7084(void) {
    D_801FBEF8.unk1 = 4;
    D_801FBEF8.unk0 = 0;
    func_801F6B48();
    if (!func_801F77E4()) {
        D_801FB1A8.unk2 = 0;
        func_8002F9F0();
        func_8002F8A8(func_801F6A84);
        func_8002F488(0x414);
    }
}

void func_801F70E4(void) {
    if (D_801FB1A8.unk2 != 1) {
        D_801FB1A8.unk2 = 2;
    }
}

void func_801F7108(s32 slot) {
    s32 fd;

    D_801FBEF8.unk2 = slot;
    fd = func_801FA3D8(D_80077DB2 << 4, (char *)D_801FB1A8.unk34[slot].unk4);
    D_801FBEF8.unk18 = fd;
    if (fd < 0) {
        func_801F72B4(0x2E);
        return;
    }
    func_801FA5A4(fd, (s32)D_801FBEF8.unk2D0, 0x180, func_801F6938);
}

INCLUDE_ASM("asm/us/ovl_39/nonmatchings/ovl_39", func_801F7190);

void func_801F72B4(s32 a) {
    D_801FBEF8.unk1 = 5;
    func_8002DD00(D_801FBEF8.unk1C);
    D_801FBEF8.unk1C = NULL;
    func_801F7780(a);
}

void func_801F7300(s32 a) {
    D_801FBEF8.unk1 = 5;
    func_8002DD00(D_801FBEF8.unk1C);
    D_801FBEF8.unk1C = NULL;
    func_801F77A8(a);
}

void func_801F734C(void) {
    D_801FC348.unk0 = 0;
    D_801FC348.unk8 = 0;
    D_801FC348.unk4 = 0;
}

void func_801F7364(SaveMenu *m) {
    if (D_801FC348.unk0 == 0) {
        D_801FB1A8.unk2 = 0;
        D_801FC348.unk4 = 0;
        D_801FB030.y = D_801FB1A8.unk24.unk6 * 0x3C + 0x3F;
        D_801FC348.unk8 = func_8002DC54(&D_801FB030, 0x14, 2);
        func_8002E2EC(D_801FC348.unk8, func_801F7560);
        func_8002E2E4(D_801FC348.unk8, func_801F74F4);
        func_8002E2FC(D_801FC348.unk8, func_801F7510);
        func_8002E31C(D_801FC348.unk8, 1);
    }
}

void func_801F740C(void) {
    if (D_801FC348.unk8->unk1C == 3 && (D_80077528 & 0x30000)) {
        func_8002F488(0x65);
        D_801FB1A8.unk2 = 0;
        func_8002DD00(D_801FC348.unk8);
    }
}

void func_801F7474(void) {
    if (!func_801F5340(D_80077DB0.unk2, 0)) {
        func_8002DE10(D_801FC348.unk8);
        func_801F77A8(0x2D);
        return;
    }
    if (++D_801FC348.unk4 >= 0x40) {
        D_801FB1A8.unk2 = 0;
        func_8002DD00(D_801FC348.unk8);
    }
}

void func_801F74F4(Window *w) {
    D_801FB1A8.unk2 = 2;
    D_801FB1A8.unk1 = 6;
}

void func_801F7510(Window *w) {
    D_801FB1A8.unk2 = 2;
    D_801FB1A8.unk1 = 4;
    D_801FC348.unk8 = 0;
    D_801FC348.unk4 = 0;
    D_801FB1A8.unk20 = func_80035774(0x1A);
}

void func_801F7560(Window *w) {
    u8 *s = func_80035774(0x2C);

    func_8002B5FC(w->unk0 + ((w->unk4 - func_8002BA68((s32)s)) >> 1), w->unk2 + 8, w->unk18, s, 0);
}

INCLUDE_ASM("asm/us/ovl_39/nonmatchings/ovl_39", func_801F75C8);

void func_801F770C(void) {
    D_801FC358.unk0 = 0;
}

void func_801F7718(void) {
    if (D_801FC358.unk4->unk1C == 3 && (D_80077528 & 0x30000)) {
        func_8002F488(0x65);
        D_801FB1A8.unk2 = 0;
        func_8002DD00(D_801FC358.unk4);
    }
}

void func_801F7780(s32 a) {
    func_801F75C8(a, D_80070440);
}

void func_801F77A8(s32 a) {
    func_801F75C8(a, D_8007048C);
}

u8 func_801F77D0(u8 v) {
    u8 old = D_801FC358.unk0;

    D_801FC358.unk0 = v;
    return old;
}

u8 func_801F77E4(void) {
    return D_801FC358.unk0;
}

void func_801F77F4(Window *w) {
    func_8002B5FC(w->unk0 + 8, w->unk2 + 8, w->unk18, D_801FC360, 0);
}

void func_801F7830(void) {
    D_801FB1A8.unk2 = 1;
}

void func_801F7840(void) {
    D_801FB1A8.unk2 = 2;
    D_801FC358.unk4 = NULL;
    D_801FC358.unk0 = 0;
    if (D_801FB038[D_801FB1A8.unk1] != 0) {
        D_801FB1A8.unk2 = 0;
        func_8002F9F0();
        func_8002F8A8(func_801F78CC);
        return;
    }
    D_801FB1A8.unk20 = func_80035774(0x16);
    D_801FB1A8.unk1 = 1;
}

void func_801F78CC(void) {
    D_801FB1A8.unk20 = func_80035774(0x16);
    D_801FB1A8.unk1 = 1;
    D_801FB1A8.unk2 = 2;
    func_8002F9D0();
    func_8002F8A8(func_801F7918);
}

void func_801F7918(void) {
    D_801FB1A8.unk2 = 2;
}

void func_801F7928(void) {
    func_80028720(D_801EE4D4, 10);
}

void func_801F7950(void) {
    func_8002886C();
}

u8 func_801F7970(Character *c) {
    s32 m;

    if (c == NULL) {
        return 0xFF;
    }
    m = func_80028954(c->unk34 & 0xF);
    if (m == 0xFF) {
        return 0xFF;
    }
    if (m == 2 && (s32)((c->unk34 >> 4) & 0x1F) >= 5) {
        return 3;
    }
    return D_801FB040[m];
}

u8 func_801F79F4(s32 mode) {
    D_801FB1A8.unk1 = 5;
    D_801FB1A8.unk2 = 0;
    D_801FC388.unk0 = 1;
    D_801FC388.unk2 = 0;
    D_801FC388.unk3 = mode;
    D_801FC388.unk24 = 0;
    D_801FC388.unk4 = 0;
    D_801FC388.unk5 = 0;
    D_801FC388.unk1 = 0;
    func_801F5E40();
    func_801FA2A0();
    func_801FA2AC();
    D_801FC388.unk1C = func_8002DC54(&D_801FB058, 0x1E, 2);
    func_8002E2EC(D_801FC388.unk1C, func_801F7CD0);
    func_8002E2E4(D_801FC388.unk1C, func_801F8408);
    func_8002E2FC(D_801FC388.unk1C, func_801F8424);
    func_8002E31C(D_801FC388.unk1C, 1);
    func_8002E304(D_801FC388.unk1C, D_800703FC.unk0[D_801FB050[mode]]);
    return 1;
}

INCLUDE_RODATA("asm/us/ovl_39/nonmatchings/ovl_39", D_801EDF14);

INCLUDE_RODATA("asm/us/ovl_39/nonmatchings/ovl_39", D_801EDF1C);

INCLUDE_RODATA("asm/us/ovl_39/nonmatchings/ovl_39", D_801EE4D4);

INCLUDE_RODATA("asm/us/ovl_39/nonmatchings/ovl_39", D_801F2854);

INCLUDE_RODATA("asm/us/ovl_39/nonmatchings/ovl_39", D_801F4104);

INCLUDE_RODATA("asm/us/ovl_39/nonmatchings/ovl_39", D_801F41A4);

INCLUDE_RODATA("asm/us/ovl_39/nonmatchings/ovl_39", D_801F4244);

INCLUDE_RODATA("asm/us/ovl_39/nonmatchings/ovl_39", D_801F42E4);

INCLUDE_ASM("asm/us/ovl_39/nonmatchings/ovl_39", func_801F7AF4);

void func_801F7C20(void) {
    switch (D_801FC388.unk2) {
    case 1:
        if (++D_801FC388.unk24 >= 4) {
            D_801FC388.unk2 = 4;
            D_801FC388.unk24 = 0;
            D_801FB1A8.unk20 = func_80035774(0x1E);
        }
        break;
    case 4:
        if (++D_801FC388.unk24 >= 4) {
            D_801FC388.unk24 = 0;
            if (func_801F8478()) {
                D_801FC388.unk2 = 5;
            }
        }
        break;
    case 5:
        break;
    }
}

INCLUDE_ASM("asm/us/ovl_39/nonmatchings/ovl_39", func_801F7CD0);

INCLUDE_ASM("asm/us/ovl_39/nonmatchings/ovl_39", func_801F7E68);

INCLUDE_ASM("asm/us/ovl_39/nonmatchings/ovl_39", func_801F7FD4);

void func_801F830C(void) {
    D_801FC388.unk0 = 0;
}

void func_801F8318(void) {
    if (!func_801F5340(D_80077DB0.unk2, 0)) {
        func_801F889C(0x2D);
        return;
    }
    if (D_801FB1A8.unk2 != 1) {
        if (D_801FC388.unk3 == 0) {
            func_801F7AF4();
        } else {
            func_801F7C20();
        }
    }
}

u8 func_801F8394(void) {
    return D_801FC388.unk0;
}

u8 func_801F83A4(u8 v) {
    u8 old = D_801FC388.unk0;

    D_801FC388.unk0 = v;
    return old;
}

u8 *func_801F83B8(void) {
    memcpy(D_801FC368, D_801F43A8, 13);
    return D_801FC368;
}

void func_801F8408(Window *w) {
    D_801FB1A8.unk2 = 2;
    D_801FC388.unk2 = 1;
}

void func_801F8424(Window *w) {
    D_801FC388.unk0 = 0;
    D_801FB1A8.unk2 = 2;
    if (D_801FB1A8.unk0 != 1) {
        func_801F7364(&D_801FB1A8);
        return;
    }
    D_801FB1A8.unk1 = 4;
}

u8 func_801F8478(void) {
    SaveSlot *sl;
    s32 port;
    s32 fd;

    port = D_80077DB2 << 4;
    sl = &D_801FB1A8.unk34[D_801FB1A8.unk3];
    func_80065238(D_801FC388.unk6, sl->unk4);
    fd = func_801FA3D8(port, D_801FC388.unk6);
    D_801FC388.unk20 = fd;
    if (fd < 0) {
        func_801F889C(0x34);
        return 0;
    }
    func_801FA5A4(fd, 0x801FCB00, 0x1400, func_801F8588);
    return 1;
}

s32 func_801F8524(void) {
    if (func_801FA754(D_80077DB0.unk2 * 0x10) == 0) {
        func_801F889C(0x32);
        return 0;
    }
    if (func_801FA350(D_80077DB0.unk2 * 0x10, func_801F8904) != 0) {
        return 1;
    }
    func_801F889C(0x33);
    return 0;
}

void func_801F8588(void) {
    s32 n = func_801FAAE4(D_80077DB0.unk2 * 0x10);

    if (func_801FA478(D_801FC388.unk20) < 0) {
        func_801F889C(D_801FB1A8.unk0 == 1 ? 0x34 : 0x33);
        return;
    }
    if (n == 0) {
        func_801F8610();
        return;
    }
    func_801F86C0(n);
}

void func_801F8610(void) {
    u8 mode;

    func_8002DD00(D_801FC388.unk1C);
    D_801FB1A8.unk2 = 0;
    mode = D_801FB1A8.unk0;
    if (mode != 1) {
        func_801F8704(D_801FB1A8.unk3, (void *)0x801FCC00);
        func_8002F488(0x4ED);
        return;
    }
    if (func_801F9804()) {
        D_801FB1A8.unk7 = mode;
        func_8002F488(0x4ED);
        if (D_80077DB0.unk1 == 0) {
            func_800361CC(0);
            return;
        }
        func_801F53B0();
    }
}

void func_801F86C0(s32 i) {
    if (i < 4) {
        if (i <= 0) {
            return;
        }
        func_801F889C(D_801FB1A8.unk0 != 1 ? 0x33 : 0x34);
    }
}

INCLUDE_ASM("asm/us/ovl_39/nonmatchings/ovl_39", func_801F8704);

void func_801F889C(s32 msg) {
    D_801FB1A8.unk8 = 1;
    func_801FA478(D_801FC388.unk20);
    func_8002E2FC(D_801FC388.unk1C, func_801F89A0);
    func_8002DD00(D_801FC388.unk1C);
    func_801F7780(msg);
}

void func_801F8904(void) {
    s32 r = func_801FAAE4();

    D_801FB1A8.unk8 = 1;
    if (r == 0) {
        func_8002F488(0x414);
        D_801FB1A8.unk6 = 1;
        D_801FC388.unk2 = 4;
        D_801FC388.unk24 = 4;
        D_801FB1A8.unk20 = func_80035774(0x1D);
        return;
    }
    func_8002E2FC(D_801FC388.unk1C, func_801F89A0);
    func_8002DD00(D_801FC388.unk1C);
    func_801F7780(0x33);
}

void func_801F89A0(Window *w) {
    D_801FC388.unk0 = 0;
    D_801FB1A8.unk2 = 2;
}

MenuView *func_801F89B8(MenuList *l, MenuView *v) {
    s32 d;
    u8 f;

    f = l->unk8;
    if (f & 4) {
        if (f & 2) {
            d = l->unkD - l->unk9;
            v->unk0 = (d + 0x3B) / 60;
            v->unk1 = l->unk5 + (l->unk3 - v->unk0);
            v->unk2 = (l->unk3 - v->unk0) * 60;
        } else {
            d = l->unkD - l->unk9;
            v->unk0 = (d + 0x3B) / 60;
            v->unk1 = l->unk5;
            v->unk2 = 0;
        }
    } else if (f & 2) {
        v->unk0 = l->unk3 + 1;
        v->unk1 = l->unk5 - 1;
        v->unk2 = l->unk9 - 60;
    } else if (f & 1) {
        v->unk1 = l->unk5;
        v->unk0 = l->unk3 + 1;
        v->unk2 = -l->unk9;
    } else {
        v->unk1 = l->unk5;
        v->unk0 = l->unk3;
        v->unk2 = 0;
    }
    return v;
}

MenuView *func_801F8B10(MenuList *l, MenuView *v) {
    s32 x;

    if (l->unk8 & 2) {
        v->unk0 = (l->unk9 + 0x3B) / 60;
        v->unk1 = l->unkC + (l->unk3 - v->unk0);
        v->unk2 = l->unk9 - v->unk0 * 60;
    } else {
        x = l->unk9;
        v->unk2 = l->unkD - x;
        v->unk0 = (x + 0x3B) / 60;
        v->unk1 = l->unkC;
    }
    return v;
}

void func_801F8BD4(MenuList *m, RECT *src, RECT *dst) {
    dst->x = src->x;
    dst->w = src->w;
    if (m->unk8 & 2) {
        dst->y = src->y + m->unk9;
    } else {
        dst->y = src->y;
    }
    dst->h = src->h - m->unk9;
}

void func_801F8C34(MenuList *l, s32 btn) {
    s32 top;
    s32 d;

    if ((btn & 0x100000) && !(D_80077524 & 0x200000) && l->unk5 != 0) {
        d = l->unkD;
        l->unk9 = d >> 2;
        l->unk8 = 7;
        l->unkE = (d - l->unk9 + 0xE) / 15;
        top = l->unk5 - l->unk3;
        if (top < 0) {
            top = 0;
        }
        l->unkC = top;
        func_8002F488(0x67);
    }
}

void func_801F8CE8(MenuList *l, s32 pad) {
    s32 h;
    s32 page;
    s32 top;
    s32 max;

    if ((pad & 0x200000) && l->unk5 < l->unk2 - l->unk3) {
        h = l->unkD;
        l->unk9 = h >> 2;
        page = l->unk3;
        max = l->unk2 - page;
        l->unk8 = 5;
        top = l->unk5 + page;
        l->unkE = (h - l->unk9 + 0xE) / 15;
        if (top < max) {
            max = top;
        }
        l->unkC = max;
        func_8002F488(0x67);
    }
}

INCLUDE_ASM("asm/us/ovl_39/nonmatchings/ovl_39", func_801F8DA4);

INCLUDE_ASM("asm/us/ovl_39/nonmatchings/ovl_39", func_801F9024);

INCLUDE_ASM("asm/us/ovl_39/nonmatchings/ovl_39", func_801F9120);

void func_801F94D0(s32 slot, s32 unused) {
    D_801FB1A8.unk3 = slot;
    if (D_801FB1A8.unk6 == 0) {
        func_8002F488(0x67);
        func_801F62F0(func_801F95E4);
        func_801F5E90(D_80077DB2 + 0x23, 0x25, 0x26);
        return;
    }
    switch (D_801FB1A8.unk34[D_801FB1A8.unk3].unk1A) {
    case 1:
    case 3:
        func_8002F488(0x67);
        func_801F95E4();
        break;
    case 4:
        func_8002F488(0x67);
        func_801F62F0(func_801F95E4);
        func_801F5E90(0x27, 0x28, 0x29);
        break;
    case 2:
        func_8002F488(0x66);
        break;
    }
}

void func_801F95E4(void) {
    func_801F96CC();
    func_801F8DA4(((SaveBuf *)0x801FCB00)->unk0);
    func_801F963C(&((SaveBuf *)0x801FCB00)->unk180);
    ((SaveBuf *)0x801FCB00)->unk13FE = func_801F973C(((SaveBuf *)0x801FCB00)->unk0, 0x13FE);
    func_801F79F4(0);
}

void func_801F963C(SaveData *d) {
    func_801F9B00(&d->unk0);
    func_801F9D48(&d->unk40);
    func_801F9904(&d->unk850);
    func_801FA01C(&d->unkD88);
    func_801F9EA8(d->unkDA0);
    func_801F9CE8(d->unkFF8);
}

s32 func_801F9688(void) {
    if ((u16)D_8007AEF0.h[0] < 0xF3C) {
        return 1;
    }
    if ((u16)D_8007AEF0.h[0] < 0x1B4E) {
        return 2;
    }
    if ((u16)D_8007AEF0.h[0] < 0x2AF3) {
        return 4;
    }
    return 8;
}

void func_801F96CC(void) {
    u16 i;
    u16 j;
    u16 c;

    for (i = 0; i < 256; i++) {
        c = i;
        for (j = 0; j < 8; j++) {
            if (c & 1) {
                c = (c >> 1) ^ 0x8408;
            } else {
                c >>= 1;
            }
        }
        D_801FC3E0[i] = c;
    }
}

INCLUDE_ASM("asm/us/ovl_39/nonmatchings/ovl_39", func_801F973C);

void func_801F9788(s32 slot, s32 arg1) {
    SaveSlot *s;

    s = D_801FB1A8.unk34;
    D_801FB1A8.unk3 = slot;
    switch (s[slot].unk1A) {
    case 4:
        func_8002F488(0x67);
        func_801F98D0();
        break;
    case 1:
    case 2:
    case 3:
        func_8002F488(0x66);
        break;
    }
}

u8 func_801F9804(void) {
    SaveHdr *h;
    u8 ret;

    ret = 1;
    func_801F96CC();
    if (D_801FDEFE == func_801F973C(((SaveBuf *)0x801FCB00)->unk0, 0x13FE)) {
        h = &((SaveBuf *)0x801FCB00)->unk100;
        D_8006794C->unkB = h->unk4;
        func_80024000(h->unk2C);
        func_801F9BEC(&((SaveBuf *)0x801FCB00)->unk180.unk0);
        func_801F9DF8(&((SaveBuf *)0x801FCB00)->unk180.unk40);
        func_801F99FC(&((SaveBuf *)0x801FCB00)->unk180.unk850);
        func_801FA078(&((SaveBuf *)0x801FCB00)->unk180.unkD88);
        func_801F9F5C(((SaveBuf *)0x801FCB00)->unk180.unkDA0);
        func_801F9D18(((SaveBuf *)0x801FCB00)->unk180.unkFF8);
    } else {
        func_801F7780(0x35);
        ret = 0;
    }
    return ret;
}

void func_801F98D0(void) {
    if (!func_801F79F4(1)) {
        func_801F7780(0x34);
    }
}

INCLUDE_ASM("asm/us/ovl_39/nonmatchings/ovl_39", func_801F9904);

void func_801F99FC(SaveParty *s) {
    GameData *gd;
    s32 i;

    gd = D_8006794C->unk1C;
    for (i = 0; i < 9; i++) {
        gd->unk3C[i] = s->unk0[i];
    }
    for (i = 0; i < 4; i++) {
        if (s->unk510[i] < 9) {
            gd->unk54C[i] = &gd->unk3C[s->unk510[i]];
        } else {
            gd->unk54C[i] = NULL;
        }
    }
    gd->unk55C = s->unk514;
    gd->unk55E = s->unk516;
    gd->unk560 = s->unk518;
    gd->unk568 = s->unk51C;
    gd->unk56A = s->unk51E;
    gd->unk56C = s->unk520;
}

void func_801F9B00(Unk801F9BEC *h) {
    GameData *gd;
    s32 i;

    gd = D_8006794C->unk1C;
    h->unk0 = D_8006794C->unk8;
    h->unk1 = D_8006794C->unkA;
    for (i = 0x1D; i >= 0; i--) {
        h->unk2[i] = 0;
    }
    if (D_801FB1A8.unk0 == 2) {
        h->unk20 = func_800B5084();
        h->unk22 = func_800B5094();
    } else {
        h->unk20 = gd->unkC;
        h->unk22 = gd->unk3A;
    }
    h->unk24 = gd->unkE;
    h->unk26 = (s8)gd->unk848;
    h->unk28 = gd->unk7DC;
    h->unk2A = gd->unk7DE;
    for (i = 0x13; i >= 0; i--) {
        h->unk2C[i] = 0;
    }
}

void func_801F9BEC(Unk801F9BEC *p) {
    GameState *gs = D_8006794C;
    GameData *gd;

    gs->unk8 = p->unk0;
    gd = gs->unk1C;
    D_8006794C->unkA = p->unk1;
    gd->unkC = p->unk20;
    gd->unk3A = p->unk22;
    gd->unkE = p->unk24;
    gd->unk848 = p->unk26;
    gd->unk7DC = p->unk28;
    gd->unk7DE = p->unk2A;
}

void func_801F9C58(CardData *d, CardData *s) {
    s32 i;

    d->unk0 = s->unk0;
    d->unk2 = s->unk2;
    d->unk4 = s->unk4;
    for (i = 0; i < 0x69; i++) {
        d->unk6[i].unk0 = s->unk6[i].unk0;
        d->unk6[i].unk1 = s->unk6[i].unk1;
        d->unk6[i].unk2 = s->unk6[i].unk2;
        d->unk6[i].unk3 = s->unk6[i].unk3;
        d->unk6[i].unk4 = s->unk6[i].unk4;
        d->unk6[i].unk5 = s->unk6[i].unk5;
    }
    d->unk27C = s->unk27C;
    d->unk27E = s->unk27E;
}

void func_801F9CE8(void *p) {
    func_801F9C58(p, func_80062824());
}

void func_801F9D18(void *p) {
    func_801F9C58(func_80062824(), p);
}

void func_801F9D48(SaveEvents *d) {
    d->unk0 = D_8007AEF0;
    d->unk800 = D_8007AEC4;
}

void func_801F9DF8(SaveEvents *s) {
    D_8007AEF0 = s->unk0;
    D_8007AEC4 = s->unk800;
}

INCLUDE_ASM("asm/us/ovl_39/nonmatchings/ovl_39", func_801F9EA8);

INCLUDE_ASM("asm/us/ovl_39/nonmatchings/ovl_39", func_801F9F5C);

void func_801FA01C(Unk801FA078 *p) {
    p->unk0 = D_8006794C->unk1C->unk844;
    p->unk4 = D_8006794C->unk1C->unk880;
    p->unk14 = D_8006794C->unk1C->unk892;
}

void func_801FA078(Unk801FA078 *p) {
    D_8006794C->unk1C->unk844 = p->unk0;
    D_8006794C->unk1C->unk880 = p->unk4;
    D_8006794C->unk1C->unk892 = p->unk14;
}

void func_801FA0C8(void) {
    D_801FB074 = 0;
    D_801FB07C = 0;
    D_801FB078 = 0;
    D_801FB09C = 0;
    D_801FB0A0 = 0;
}

INCLUDE_RODATA("asm/us/ovl_39/nonmatchings/ovl_39", D_801F43A8);

INCLUDE_RODATA("asm/us/ovl_39/nonmatchings/ovl_39", D_801F43B8);

s32 func_801FA0F4(s32 mode, s32 chan) {
    char path[16];

    if (mode == 0) {
        strcpy(path, "bu00:*");
    } else {
        strcpy(path, "bu10:*");
    }
    func_80065228(0);
    func_80065228(1);
    func_801FAAF4();
    return firstfile(path, chan);
}

s32 func_801FA184(s32 chan) {
    func_80065228(0);
    func_80065228(1);
    func_801FAAF4();
    return func_80065EB0(chan);
}

s32 func_801FA1C4(s32 mode, s32 chan) {
    s32 n = 0;

    if (func_801FA0F4(mode, chan) == chan) {
        do {
            n++;
            chan += 0x28;
        } while (func_801FA184(chan) == chan);
    }
    return n;
}

s32 func_801FA21C(s32 i) {
    if (i == 0) {
        return D_801FB088;
    }
    return D_801FB08C;
}

s32 func_801FA240(s32 chan) {
    func_801FAE1C();
    func_80065228(0);
    func_80065228(1);
    func_80065218(chan);
    while (func_801FAB08() == -1) {
    }
}

s32 func_801FA290(void) {
    return D_801FB074;
}

void func_801FA2A0(void) {
    D_801FB084 = 0;
}

void func_801FA2AC(void) {
    D_801FB084 = 0;
}

s32 func_801FA2B8(void) {
    return D_801FB084;
}

s32 func_801FA2C8(void) {
    return D_801FB084;
}

s32 func_801FA2D8(void) {
    return D_801FB074 != 0;
}

s32 func_801FA2E8(s32 chan) {
    s32 r;

    func_80065228(0);
    func_80065228(1);
    func_801FAE74();
    _card_clear(chan);
    do {
        r = func_801FABB8();
    } while (r == -1);
    func_801FAAF4();
    return r;
}

s32 func_801FA350(s32 a, void (*b)(void)) {
    s32 r = func_801FA37C(a);

    D_801FB098 = b;
    return r;
}

s32 func_801FA37C(s32 chan) {
    D_801FB098 = NULL;
    D_801FC5EC = chan;
    func_80065228(0);
    func_80065228(1);
    func_801FAE1C();
    D_801FB074 = 1;
    return func_80066170(chan);
}

s32 func_801FA3D8(s32 port, char *name) {
    char path[32];

    func_80065228(0);
    func_80065228(1);
    D_801FB080 = port;
    if (port == 0) {
        strcpy(path, "bu00:");
    } else {
        strcpy(path, "bu10:");
    }
    func_80063108((u8 *)path, (u8 *)name);
    func_801FAAF4();
    return func_80065E60(path, 0x8003);
}

s32 func_801FA478(s32 chan) {
    func_80065228(0);
    func_80065228(1);
    func_801FAAF4();
    return func_80065EA0(chan);
}

s32 func_801FA4B8(s32 a, s32 b, s32 c, void (*d)(void)) {
    s32 r = func_801FA4E4(a, b, c);

    D_801FB098 = d;
    return r;
}

s32 func_801FA4E4(s32 a, s32 b, s32 size) {
    D_801FB098 = NULL;
    D_801FC5E4 = a;
    D_801FB084 = 0;
    D_801FC5E8 = b;
    D_801FC5E0 = (size + 0x7F) / 0x80 - 1;
    func_801FA53C();
    return 1;
}

s32 func_801FA53C(void) {
    func_80065228(0);
    func_80065228(1);
    func_801FAE1C();
    if (func_80065E90(D_801FC5E4, D_801FC5E8, 0x80) == -1) {
        return 0;
    }
    D_801FB074 = 5;
    return 1;
}

s32 func_801FA5A4(s32 a, s32 b, s32 c, void (*d)(void)) {
    s32 r = func_801FA5D0(a, b, c);

    D_801FB098 = d;
    return r;
}

s32 func_801FA5D0(s32 arg0, s32 arg1, s32 size) {
    D_801FB098 = NULL;
    D_801FC5E4 = arg0;
    D_801FB084 = 0;
    D_801FC5E8 = arg1;
    D_801FC5E0 = (size + 0x7F) / 128 - 1;
    func_80065228(0);
    func_80065228(1);
    func_801FAE1C();
    func_801FA644();
    return 1;
}

s32 func_801FA644(void) {
    func_80065228(0);
    func_80065228(1);
    func_801FAE1C();
    if (func_80065E80(D_801FC5E4, D_801FC5E8, 0x80) == -1) {
        return 0;
    }
    D_801FB074 = 4;
    return 1;
}

u8 func_801FA6AC(s32 port, char *name) {
    char path[32];
    s32 fd;
    s32 ok;

    func_80065228(0);
    func_80065228(1);
    if (port == 0) {
        strcpy(path, "bu00:");
    } else {
        strcpy(path, "bu10:");
    }
    func_80063108((u8 *)path, (u8 *)name);
    fd = func_80065E60(path, 0x10200);
    ok = fd != -1;
    func_80065EA0(fd);
    return ok;
}

s32 func_801FA754(s32 chan) {
    func_80065228(0);
    func_80065228(1);
    func_801FAAF4();
    return _card_format(chan);
}

INCLUDE_ASM("asm/us/ovl_39/nonmatchings/ovl_39", func_801FA794);

s32 func_801FAAE4(s32 chan) {
    return D_801FC604;
}

void func_801FAAF4(void) {
    D_801FB074 = 0;
    D_801FB078 = 0;
}

s32 func_801FAB08(void) {
    s32 r = -1;

    if (func_80065208(D_801FC5F4) == 1) {
        r = 0;
    } else if (func_80065208(D_801FC5F8) == 1) {
        r = 1;
    } else if (func_80065208(D_801FC5FC) == 1) {
        r = 2;
    } else if (func_80065208(D_801FC600) == 1) {
        r = 3;
    }
    if (r != -1) {
        func_801FAAF4();
    }
    return r;
}

s32 func_801FABB8(void) {
    s32 r = -1;

    if (func_80065208(D_801FC608) == 1) {
        r = 0;
    } else if (func_80065208(D_801FC60C) == 1) {
        r = 1;
    } else if (func_80065208(D_801FC610) == 1) {
        r = 2;
    } else if (func_80065208(D_801FC614) == 1) {
        r = 3;
    }
    if (r != -1) {
        func_801FAAF4();
    }
    return r;
}

INCLUDE_ASM("asm/us/ovl_39/nonmatchings/ovl_39", func_801FAC68);

void func_801FAE1C(void) {
    func_80065208(D_801FC5F4);
    func_80065208(D_801FC5F8);
    func_80065208(D_801FC5FC);
    func_80065208(D_801FC600);
}

void func_801FAE74(void) {
    func_80065208(D_801FC608);
    func_80065208(D_801FC60C);
    func_80065208(D_801FC610);
    func_80065208(D_801FC614);
}

void func_801FAECC(void) {
    func_80012D08();
    func_80056CAC(D_801FC5F4);
    func_80056CAC(D_801FC5F8);
    func_80056CAC(D_801FC5FC);
    func_80056CAC(D_801FC600);
    func_80056CAC(D_801FC608);
    func_80056CAC(D_801FC60C);
    func_80056CAC(D_801FC610);
    func_80056CAC(D_801FC614);
    func_80012D18();
}
