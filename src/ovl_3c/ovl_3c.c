#include "ovl_3c.h"

void func_801F4000(void) {
    func_8002B524(D_801FBC14.x + 6, D_801FBC14.y - 3, D_801FC2D0.unk20, D_80070450, 0);
    if (D_801FC2D0.unk10 != NULL) {
        func_8002B5FC(D_801FBC14.x + 0xA, D_801FBC14.y + 6, D_801FC2D0.unk20, D_801FC2D0.unk10, 0);
    }
    func_8002DEE4(&D_801FBC14, D_801FC2D0.unk20, 1);
    func_8002705C(&D_801FBC14, D_801FC2D0.unk20, 1);
}

void func_801F40AC(void) {
    if (!func_8002F810()) {
        if (D_80077528 & 0x10000) {
            func_8002F488(0x65);
            func_801F4A14();
            return;
        }
        if (D_80077528 & 0x10) {
            func_8002F488(0x67);
            D_801FC2D0.unk1 = D_801FC2D0.unk1 == 0 ? D_801FC2D0.unk2 - 1 : D_801FC2D0.unk1 - 1;
            return;
        }
        if (D_80077528 & 0x40) {
            func_8002F488(0x67);
            D_801FC2D0.unk1 = D_801FC2D0.unk1 >= D_801FC2D0.unk2 - 1 ? 0 : D_801FC2D0.unk1 + 1;
            return;
        }
        if (D_80077528 & 0x20000) {
            func_8002F488(0x67);
            func_801F4960();
        }
    }
}

void func_801F4198(void) {
    s32 i;
    s32 y;

    D_801FBC1C.h = D_801FC2D0.unk2 * 0xF + 0xB;
    func_8002DEE4(&D_801FBC1C, 0x23, 2);
    func_8002705C(&D_801FBC1C, 0x23, 2);
    for (i = 0; i < D_801FC2D0.unk2; i++) {
        func_8002B5FC(D_801FBC1C.x + 0xA, D_801FBC1C.y + 6 + i * 0xF, 0x1E, func_80035774(D_801FBC24[D_801FC2D0.unk0] + i), 0);
    }
}

INCLUDE_ASM("asm/us/ovl_3c/nonmatchings/ovl_3c", func_801F42A8);

void func_801F43F8(void) {
    D_801FC2D0.unk20 = 5;
    func_8002F9F0();
    func_8002F8A8(func_801F4854);
}

void func_801F442C(s32 i) {
    D_801FC2D0.unk10 = (u8 *)func_800357C0(D_801F3C9C, i);
}

s32 func_801F4458(void) {
    return D_801FC2D0.unk8;
}

void func_801F4468(void) {
    s32 i;

    D_801FC2D0.unk14 = 0;
    for (i = 0; i < 9; i++) {
        if ((D_8006794C->unk1C->unk3C[i].unk34 >> 11) & 1) {
            D_801FC2D0.unk18[D_801FC2D0.unk14] = i;
            if (++D_801FC2D0.unk14 >= 8) {
                break;
            }
        }
    }
}

void func_801F44F4(s32 x, s32 y, s32 z, s32 icon, u8 arg4) {
    RectTbl7 rects = D_801EDF04;

    func_80027710(x, y, z, &rects.r[icon], arg4);
}

INCLUDE_ASM("asm/us/ovl_3c/nonmatchings/ovl_3c", func_801F45E4);

void func_801F4688(void) {
    func_801F4748();
    func_801F4000();
    if (D_801FC2D0.unk3 != 0) {
        D_801FBBA0[D_801FC2D0.unk0][D_801FC2D0.unk1][1]();
        return;
    }
    func_801F40AC();
    func_801F47C8();
    func_801F47F0();
}

void func_801F4720(void) {
    func_8003556C(NULL);
    func_8002886C();
}

void func_801F4748(void) {
    func_8002DEE4(&D_801FBC0C, 5, 2);
    func_8002705C(&D_801FBC0C, 0x28, 2);
    func_8002B5FC(D_801FBC0C.x + 0xA, D_801FBC0C.y + 6, 4, (u8 *)func_800357C0(D_801F3C9C, D_801FC2D0.unk0), 0);
}

void func_801F47C8(void) {
    func_801F4198();
    func_801F47F8();
}

void func_801F47F0(void) {
}

void func_801F47F8(void) {
    if (!func_8002F810()) {
        D_80077DE8.unk0 = 0x6E;
        D_80077DE8.unk4 = 0x14;
        D_80077DE8.unk2 = D_801FC2D0.unk1 * 0xF + 0x6A;
        func_800355E4();
    }
}

void func_801F4854(void) {
    D_801FC2D0.unk3 = 0;
    D_801FBBA0[D_801FC2D0.unk0][D_801FC2D0.unk1][2]();
    func_8002F9D0();
    func_8003556C(func_801F49C4);
    func_8003575C(D_801F3C9C);
    func_801F442C(D_801FC2D0.unk4 != 0 ? 0xE : 0xD);
}

s32 func_801F48F4(s32 kind) {
    s32 n;
    s32 i;

    n = 1;
    for (i = 0; i < 3; i++) {
        if (D_801FBBA0[kind][i][0] != NULL || D_801FBBA0[kind][i][1] != NULL || D_801FBBA0[kind][i][2] != NULL) {
            n++;
        }
    }
    return n;
}

void func_801F4960(void) {
    if (D_801FC2D0.unk2 - 1 == D_801FC2D0.unk1) {
        func_801F4A14();
        return;
    }
    D_801FC2D0.unk4++;
    func_8002F9F0();
    func_8002F8A8(func_801F4AC0);
}

void func_801F49C4(void) {
    func_80035580(&D_801FBC28);
    D_80077DE8.unk8 = func_80035774(D_801FBC30[D_801FC2D0.unk0] + D_801FC2D0.unk1);
}

void func_801F4A14(void) {
    func_8002F8E4(D_801FC2D0.unkC);
    func_800325E0();
}

void func_801F4A44(void) {
    u8 *p = D_801F36FC;

    LoadImage(&D_801FBC34, (u32 *)p);
    LoadImage(&D_801FBC3C, (u32 *)(p + 0x60));
    DrawSync(0);
}

s32 func_801F4A90(s32 id) {
    if (id < 0x20) {
        return func_801F59A8(id);
    }
    return 2;
}

void func_801F4AC0(void) {
    D_801FC2D0.unk3 = 1;
    D_801FBBA0[D_801FC2D0.unk0][D_801FC2D0.unk1][0]();
    func_8002F9D0();
    func_8002F8A8(func_801F4B38);
}

void func_801F4B38(void) {
    D_801FC2D0.unk20 = 0x24;
}

void func_801F4B48(void) {
    D_801FC2F8.unk0 = 1;
    func_801F5D98();
    func_80035580(NULL);
    func_8003556C(func_801F5EEC);
    setRECT(&D_801FC2F8.unk8, 0xBC, 0x26, 0x78, D_801FC2F8.unk4 ? 0x71 : 0xAD);
    setRECT(&D_801FC2F8.unk10, 8, 0x26, 0xB4, D_801FC2F8.unk4 ? 0x71 : 0xAD);
    setRECT(&D_801FC2F8.unk18, 8, 0x97, 0x12C, 0x45);
    setRECT(&D_801FC2F8.unk20, 0x62, 0x60, 0x7C, 0x45);
    func_8003575C(D_801F3E98);
    func_801F442C(0xF);
    func_8002EDFC(&D_801FC2F8.unk28, D_801FC2F8.unk1, D_801FC2F8.unk4 ? 7 : 0xB, 1, D_801FC2F8.unk10.h);
    func_801F4468();
}

void func_801F4C68(void) {
    s32 q;
    s32 t;

    if (D_80077528 & 0x10000) {
        func_8002F488(0x65);
        func_801F5D14();
        return;
    }
    if ((D_80077528 & 0x20) && D_801FC2F8.unk2 < D_801FC2F8.unk3) {
        func_8002F488(0x67);
        D_801FC2F8.unk2++;
        return;
    }
    if ((D_80077528 & 0x80) && D_801FC2F8.unk2 >= 2) {
        func_8002F488(0x67);
        D_801FC2F8.unk2--;
        return;
    }
    if ((D_80077528 & 0x10) && D_801FC2F8.unk2 < D_801FC2F8.unk3) {
        func_8002F488(0x67);
        q = D_801FC2F8.unk2 + 10;
        if (q > D_801FC2F8.unk3) {
            q = D_801FC2F8.unk3;
        }
        D_801FC2F8.unk2 = q;
        return;
    }
    if ((D_80077528 & 0x40) && D_801FC2F8.unk2 >= 2) {
        func_8002F488(0x67);
        t = D_801FC2F8.unk2 - 10;
        q = 1;
        if (t > 0) {
            q = t;
        }
        D_801FC2F8.unk2 = q;
        return;
    }
    if (D_80077528 & 0x20000) {
        func_8002F488(0x415);
        func_801F5E4C();
    }
}

void func_801F4E1C(void) {
    u8 buf[16];
    RECT *r;
    MenuList *l;
    u8 id;
    u8 equip;
    s32 i;

    r = &D_801FC2F8.unk8;
    l = &D_801FC2F8.unk28;
    id = D_801FC2F8.unk3C[l->unk5 + l->unk6];
    equip = (D_80069018[id].unkF & 0xF8) != 0;
    func_8002B524(r->x + 6, r->y - 3, 0x1E, D_80070460, 0);
    func_8002DEE4(r, 0x23, 1);
    for (i = 0; i < 3; i++) {
        func_8002B5FC(r->x + 8, r->y + D_801FC050[i] - 4, 0x1E, func_800357C0(D_801F3C9C, D_801FC064[i]),
                      (D_801FC05C[i] | equip) ? 0 : 2);
    }
    func_8002BAE4(D_8006794C->unk1C->unk560, buf, 7, 1);
    func_80063108(buf, D_801FC060);
    func_8002B56C(r->x + 0x39, r->y + D_801FC050[0], 0x1E, buf, 0);
    func_8002BAE4(func_80027F34(id), buf, 2, 1);
    func_8002B56C(r->x + 0x5C, r->y + D_801FC050[1], 0x1E, buf, 0);
    func_8002BAE4(func_80027F94(id), buf, 2, 1);
    func_8002B56C(r->x + 0x5C, r->y + D_801FC050[2], 0x1E, buf, equip ? 0 : 2);
}

void func_801F506C(void) {
    RECT sub;
    MenuView view;
    s32 w;
    s32 i;
    MenuList *list;
    RECT *r = &D_801FC2F8.unk10;

    w = func_8002AE7C((u8 *)D_800703FC.unkA4);
    func_8002B524(r->x + 0xE, r->y - 3, 0x23, D_800703FC.unk68, 0);
    func_8002B524((r->x + 0xA6) - w, r->y - 3, 0x23, D_800703FC.unkA4, 0);
    list = &D_801FC2F8.unk28;
    func_8002E040(r, 0x23, 1, list);
    func_8002ECA0(list, &view);
    func_8002D9B4(NULL, 0x1E, 1);
    for (i = 0; i < view.unk0; i++) {
        func_801F5BCC(0, i, view.unk1 + i, &view);
    }
    if (list->unk8 & 4) {
        func_8002F40C(list, r, &sub);
        func_8002D9B4(&sub, 0x1E, 1);
        func_801F5B64();
    }
    func_8002D9B4(r, 0x1E, 1);
}

INCLUDE_ASM("asm/us/ovl_3c/nonmatchings/ovl_3c", func_801F51B8);

void func_801F551C(Window *w) {
    u8 buf[16];
    ShopBuy *b;
    MenuList *l;
    Unk80069018 *it;
    u8 id;

    l = &D_801FC2F8.unk28;
    b = &D_801FC2F8;
    id = b->unk3C[l->unk5 + l->unk6];
    it = &D_80069018[id];
    func_80026C34(w->unk0 + 8, w->unk2 + 8, w->unk18, id, 1);
    func_8002B5FC(w->unk0 + 0x18, w->unk2 + 8, w->unk18, D_8006A218 + it->unk0, 0);
    func_800269F4(w->unk0 + 4, w->unk2 + 0x18, w->unk18, w->unk4 - 9);
    func_8002B5FC(w->unk0 + 0xC, w->unk2 + 0x1E, w->unk18, func_800357C0(D_801F3C9C, 0x19), 0);
    func_8002B5FC(w->unk0 + 0xC, w->unk2 + 0x2E, w->unk18, func_800357C0(D_801F3C9C, 0x1A), 0);
    func_8002BAE4(b->unk2, buf, 2, 1);
    func_8002B56C(w->unk0 + 0x5C, w->unk2 + 0x22, w->unk18, buf, 0);
    func_8002BAE4(b->unk2 * it->unk4, buf, 7, 1);
    func_80063108(buf, D_801FC098);
    func_8002B56C(w->unk0 + 0x39, w->unk2 + 0x32, w->unk18, buf, 0);
}

void func_801F56E4(void) {
    if (!func_8002F810()) {
        if (D_801FC2F8.unk0 == 1 || (D_80077DC0 & 3)) {
            D_80077DE8.unk4 = 0x14;
            D_80077DE8.unk0 = D_801FC2F8.unk10.x - 8;
            D_80077DE8.unk2 = D_801FC2F8.unk10.y + (s16)(D_801FC2F8.unk28.unk6 * 0xF + 0xB);
            func_800355E4();
        }
        if (D_801FC2F8.unk0 == 2) {
            D_80077DE8.unk0 = D_801FC2F8.unk38->unk0 - 0xE;
            D_80077DE8.unk2 = D_801FC2F8.unk38->unk2 + 0x22;
            D_80077DE8.unk4 = D_801FC2F8.unk38->unk18 - 1;
            func_800355E4();
        }
    }
}

INCLUDE_ASM("asm/us/ovl_3c/nonmatchings/ovl_3c", func_801F57C8);

void func_801F5940(void) {
    if (D_80077528 & 8) {
        func_8002C1E4();
    }
    func_801F5A1C();
    func_801F5A74();
    func_801F5AAC();
}

void func_801F5988(void) {
    func_8003556C(NULL);
}

s32 func_801F59A8(s32 shop) {
    u8 *list;
    s32 i;
    u8 id;

    list = D_801FBC44[shop];
    for (i = 0; i < 0x20; i++) {
        if (list[i] != 0xFF) {
            if (!(D_80069018[list[i]].unkF & 0xF0)) {
                return 0;
            }
        }
    }
    return 1;
}

void func_801F5A1C(void) {
    void (*fn)(void);

    if (!func_8002F810()) {
        fn = D_801FC044[D_801FC2F8.unk0];
        if (fn != NULL) {
            fn();
        }
    }
}

void func_801F5A74(void) {
    func_801F506C();
    func_801F4E1C();
    func_801F51B8();
    func_801F56E4();
}

void func_801F5AAC(void) {
    if (D_801FC2F8.unk28.unk8 != 0) {
        func_8002EB74(&D_801FC2F8.unk28);
    }
}

void func_801F5AE4(void) {
    MenuList *m = &D_801FC2F8.unk28;

    if (m->unk8 == 0) {
        if (D_80077528 & 0x10000) {
            func_8002F488(0x65);
            func_801F43F8();
            return;
        }
        if (D_80077528 & 0x20000) {
            func_801F57C8();
            return;
        }
        func_8002EE8C(m, D_80077528);
    }
}

void func_801F5B64(void) {
    MenuView v;
    s32 i;

    func_8002F344(&D_801FC2F8.unk28, &v);
    for (i = 0; i < v.unk0; i++) {
        func_801F5BCC(0, i, v.unk1 + i, &v);
    }
}

INCLUDE_ASM("asm/us/ovl_3c/nonmatchings/ovl_3c", func_801F5BCC);

void func_801F5D14(void) {
    D_801FC2F8.unk0 = 0;
    func_8002DD00(D_801FC2F8.unk38);
}

void func_801F5D40(Window *w) {
    D_801FC2F8.unk0 = 2;
    func_801F442C(0x12);
}

void func_801F5D6C(Window *w) {
    D_801FC2F8.unk0 = 1;
    func_801F442C(0xF);
}

void func_801F5D98(void) {
    s32 i;

    D_801FC2F8.unk3C = D_801FBC44[func_801F4458()];
    D_801FC2F8.unk4 = 0;
    D_801FC2F8.unk1 = 0;
    for (i = 0; i < 0x20; i++) {
        if (D_801FC2F8.unk3C[i] == 0xFF) {
            break;
        }
        if (D_80069018[D_801FC2F8.unk3C[i]].unkF & 0xF8) {
            D_801FC2F8.unk4 = 1;
        }
        D_801FC2F8.unk1++;
    }
}

void func_801F5E4C(void) {
    MenuList *l;
    ShopBuy *b;
    Unk80069018 *it;
    u8 id;
    s32 n;

    l = &D_801FC2F8.unk28;
    b = &D_801FC2F8;
    id = b->unk3C[l->unk5 + l->unk6];
    it = &D_80069018[id];
    n = func_80027D84(id, b->unk2);
    if (n != 0) {
        D_8006794C->unk1C->unk560 -= it->unk4 * n;
        func_801F5D14();
    }
}

INCLUDE_ASM("asm/us/ovl_3c/nonmatchings/ovl_3c", func_801F5EEC);

void func_801F5F88(void) {
    func_801F7250();
    D_801FC338.unk0 = 1;
    func_80035580(NULL);
    func_8003556C(func_801F76C8);
    setRECT(&D_801FC338.unk8, 0xBC, 0x26, 0x78, D_801FC338.unk4 ? 0x71 : 0xAD);
    setRECT(&D_801FC338.unk10, 8, 0x26, 0xB4, D_801FC338.unk4 ? 0x71 : 0xAD);
    setRECT(&D_801FC338.unk18, 8, 0x97, 0x12C, 0x45);
    setRECT(&D_801FC338.unk20, 0x62, 0x60, 0x7C, 0x45);
    func_8003575C(D_801F3F08);
    func_801F442C(0x11);
    func_8002EDFC(&D_801FC338.unk28, D_801FC338.unk1, D_801FC338.unk4 ? 7 : 0xB, 1, D_801FC338.unk10.h);
    func_801F4468();
}

void func_801F60A8(void) {
    s32 q;
    s32 t;

    if (D_80077528 & 0x10000) {
        func_8002F488(0x65);
        func_801F7590();
        return;
    }
    if ((D_80077528 & 0x20) && D_801FC338.unk2 < D_801FC338.unk3) {
        func_8002F488(0x67);
        D_801FC338.unk2++;
        return;
    }
    if ((D_80077528 & 0x80) && D_801FC338.unk2 >= 2) {
        func_8002F488(0x67);
        D_801FC338.unk2--;
        return;
    }
    if ((D_80077528 & 0x10) && D_801FC338.unk2 < D_801FC338.unk3) {
        func_8002F488(0x67);
        q = D_801FC338.unk2 + 10;
        if (q > D_801FC338.unk3) {
            q = D_801FC338.unk3;
        }
        D_801FC338.unk2 = q;
        return;
    }
    if ((D_80077528 & 0x40) && D_801FC338.unk2 >= 2) {
        func_8002F488(0x67);
        t = D_801FC338.unk2 - 10;
        q = 1;
        if (t > 0) {
            q = t;
        }
        D_801FC338.unk2 = q;
        return;
    }
    if (D_80077528 & 0x20000) {
        func_8002F488(0x415);
        func_801F7614();
    }
}

INCLUDE_ASM("asm/us/ovl_3c/nonmatchings/ovl_3c", func_801F625C);

void func_801F65C8(void) {
    RECT sub;
    MenuView view;
    s32 w;
    s32 i;
    MenuList *list;
    RECT *r = &D_801FC338.unk10;

    w = func_8002AE7C((u8 *)D_800703FC.unkA4);
    func_8002B524(r->x + 0xE, r->y - 3, 0x23, D_800703FC.unk68, 0);
    func_8002B524((r->x + 0xA6) - w, r->y - 3, 0x23, D_800703FC.unkA4, 0);
    list = &D_801FC338.unk28;
    func_8002E040(r, 0x23, 1, list);
    func_8002ECA0(list, &view);
    func_8002D9B4(NULL, 0x1E, 1);
    for (i = 0; i < view.unk0; i++) {
        func_801F6D34(0, i, view.unk1 + i, &view);
    }
    if (list->unk8 & 4) {
        func_8002F40C(list, r, &sub);
        func_8002D9B4(&sub, 0x1E, 1);
        func_801F7528();
    }
    func_8002D9B4(r, 0x1E, 1);
}

INCLUDE_ASM("asm/us/ovl_3c/nonmatchings/ovl_3c", func_801F6714);

void func_801F6A7C(Window *w) {
    u8 buf[16];
    ShopSynth *s;
    MenuList *l;
    SynthRecipe *r;
    Unk80069018 *it;
    u8 id;

    l = &D_801FC338.unk28;
    r = &D_801FC338.unk3C[l->unk5 + l->unk6];
    id = r->unk4;
    it = &D_80069018[id];
    func_80026C34(w->unk0 + 8, w->unk2 + 8, w->unk18, id, 1);
    func_8002B5FC(w->unk0 + 0x18, w->unk2 + 8, w->unk18, D_8006A218 + it->unk0, 0);
    func_800269F4(w->unk0 + 4, w->unk2 + 0x18, w->unk18, w->unk4 - 9);
    func_8002B5FC(w->unk0 + 0xC, w->unk2 + 0x1E, w->unk18, func_800357C0(D_801F3C9C, 0x19), 0);
    func_8002B5FC(w->unk0 + 0xC, w->unk2 + 0x2E, w->unk18, func_800357C0(D_801F3C9C, 0x1A), 0);
    s = &D_801FC338;
    func_8002BAE4(s->unk2, buf, 2, 1);
    func_8002B56C(w->unk0 + 0x5C, w->unk2 + 0x22, w->unk18, buf, 0);
    func_8002BAE4(s->unk2 * r->unk0, buf, 7, 1);
    func_80063108(buf, D_801FC0F8);
    func_8002B56C(w->unk0 + 0x39, w->unk2 + 0x32, w->unk18, buf, 0);
}

void func_801F6C50(void) {
    if (!func_8002F810()) {
        if (D_801FC338.unk0 == 1 || (D_80077DC0 & 3)) {
            D_80077DE8.unk4 = 0x14;
            D_80077DE8.unk0 = D_801FC338.unk10.x - 8;
            D_80077DE8.unk2 = D_801FC338.unk10.y + (s16)(D_801FC338.unk28.unk6 * 0xF + 0xB);
            func_800355E4();
        }
        if (D_801FC338.unk0 == 2) {
            D_80077DE8.unk0 = D_801FC338.unk38->unk0 - 0xE;
            D_80077DE8.unk2 = D_801FC338.unk38->unk2 + 0x22;
            D_80077DE8.unk4 = D_801FC338.unk38->unk18 - 1;
            func_800355E4();
        }
    }
}

INCLUDE_ASM("asm/us/ovl_3c/nonmatchings/ovl_3c", func_801F6D34);

INCLUDE_ASM("asm/us/ovl_3c/nonmatchings/ovl_3c", func_801F6F00);

void func_801F7250(void) {
    s32 mask;
    s32 i;
    s32 n;

    n = 0x40;
    mask = 1 << (func_801F4458() - 0x20);
    D_801FC338.unk1 = 0;
    for (i = 0; i < n; i++) {
        if ((D_801FC0FC[i].unk5 & mask) && D_801FC0FC[i].unk4 != 0xFF) {
            D_801FC338.unk3C[D_801FC338.unk1++] = D_801FC0FC[i];
            if (D_801FC338.unk1 >= 0x20) {
                break;
            }
        }
    }
    for (i = 0; i < D_801FC338.unk1; i++) {
        if (D_80069018[D_801FC338.unk3C[i].unk4].unkF & 0xF8) {
            D_801FC338.unk4 = 1;
        }
    }
}

void func_801F7390(void) {
    func_801F73E0();
    func_801F7438();
    func_801F7470();
}

void func_801F73C0(void) {
    func_8003556C(NULL);
}

void func_801F73E0(void) {
    void (*fn)(void);

    if (!func_8002F810()) {
        fn = D_801FC09C[D_801FC338.unk0];
        if (fn != NULL) {
            fn();
        }
    }
}

void func_801F7438(void) {
    func_801F65C8();
    func_801F625C();
    func_801F6714();
    func_801F6C50();
}

void func_801F7470(void) {
    if (D_801FC338.unk28.unk8 != 0) {
        func_8002EB74(&D_801FC338.unk28);
    }
}

void func_801F74A8(void) {
    MenuList *m = &D_801FC338.unk28;

    if (m->unk8 == 0) {
        if (D_80077528 & 0x10000) {
            func_8002F488(0x65);
            func_801F43F8();
            return;
        }
        if (D_80077528 & 0x20000) {
            func_801F6F00();
            return;
        }
        func_8002EE8C(m, D_80077528);
    }
}

void func_801F7528(void) {
    MenuView v;
    s32 i;

    func_8002F344(&D_801FC338.unk28, &v);
    for (i = 0; i < v.unk0; i++) {
        func_801F6D34(0, i, v.unk1 + i, &v);
    }
}

void func_801F7590(void) {
    D_801FC338.unk0 = 0;
    func_8002DD00(D_801FC338.unk38);
}

void func_801F75BC(void) {
    D_801FC338.unk0 = 2;
    func_801F442C(0x14);
}

void func_801F75E8(void) {
    D_801FC338.unk0 = 1;
    func_801F442C(0x11);
}

void func_801F7614(void) {
    MenuList *m = &D_801FC338.unk28;
    SynthRecipe *r;
    s32 n;

    r = &D_801FC338.unk3C[m->unk5 + m->unk6];
    n = func_80027D84(r->unk4, D_801FC338.unk2);
    if (n != 0) {
        func_80027E5C(r->unk2[0], n);
        func_80027E5C(r->unk2[1], n);
        D_8006794C->unk1C->unk560 -= r->unk0 * n;
        func_801F7590();
    }
}

INCLUDE_ASM("asm/us/ovl_3c/nonmatchings/ovl_3c", func_801F76C8);

void func_801F7770(void) {
    D_801FC438.unk0 = 1;
    func_8003575C(D_801F3F88);
    func_801F442C(0x10);
    func_80035580(NULL);
    func_8003556C(func_801F8728);
    func_8002EDFC(&D_801FC438.unk4, 0xFF, 0xB, 2, 0xAD);
    setRECT(&D_801FC438.unk14, 8, 0x2A, 0x12C, 0xAD);
    setRECT(&D_801FC438.unk1C, 0x62, 0x40, 0x7C, 0x45);
    setRECT(&D_801FC438.unk24, 0x62, 0x87, 0x7C, 0x3E);
}

void func_801F7848(void) {
    s32 q;
    s32 t;

    if (D_80077528 & 0x10000) {
        func_8002F488(0x65);
        func_801F8680();
        return;
    }
    if ((D_80077528 & 0x20) && D_801FC438.unk1 < D_801FC438.unk2) {
        func_8002F488(0x67);
        D_801FC438.unk1++;
        return;
    }
    if ((D_80077528 & 0x80) && D_801FC438.unk1 >= 2) {
        func_8002F488(0x67);
        D_801FC438.unk1--;
        return;
    }
    if ((D_80077528 & 0x10) && D_801FC438.unk1 < D_801FC438.unk2) {
        func_8002F488(0x67);
        q = D_801FC438.unk1 + 10;
        if (q > D_801FC438.unk2) {
            q = D_801FC438.unk2;
        }
        D_801FC438.unk1 = q;
        return;
    }
    if ((D_80077528 & 0x40) && D_801FC438.unk1 >= 2) {
        func_8002F488(0x67);
        t = D_801FC438.unk1 - 10;
        q = 1;
        if (t > 0) {
            q = t;
        }
        D_801FC438.unk1 = q;
        return;
    }
    if (D_80077528 & 0x20000) {
        func_8002F488(0x415);
        func_801F8224();
    }
}

void func_801F79FC(void) {
    RECT sub;
    MenuView view;
    s32 w;
    s32 row;
    s32 col;
    s32 idx;
    MenuList *list = &D_801FC438.unk4;

    func_8002B524(D_801FC288->x + 0xE, D_801FC288->y - 3, 0x23, D_800703FC.unk68, 0);
    func_8002B524(D_801FC288->x + 0xA4, D_801FC288->y - 3, 0x23, D_800703FC.unk68, 0);
    w = func_8002AE7C((u8 *)D_800703FC.unk94);
    func_8002B524((D_801FC288->x + 0x8A) - w, D_801FC288->y - 3, 0x23, D_800703FC.unk94, 0);
    func_8002B524((D_801FC288->x + 0x120) - w, D_801FC288->y - 3, 0x23, D_800703FC.unk94, 0);
    func_8002ECA0(list, &view);
    func_8002E040(D_801FC288, 0x23, 1, list);
    idx = view.unk1 * 2;
    func_8002D9B4(NULL, 0x1E, 1);
    for (row = 0; row < view.unk0; row++) {
        for (col = 0; col < 2; col++) {
            func_801F856C(col, row, idx++, &view);
        }
    }
    if (list->unk8 & 4) {
        func_8002F40C(list, D_801FC288, &sub);
        func_8002D9B4(&sub, 0x1E, 1);
        func_801F84D8();
    }
    func_8002D9B4(D_801FC288, 0x1E, 1);
}

void func_801F7BC4(Window *w) {
    u8 buf[16];
    ShopSell *m;
    MenuList *l;
    Unk80069018 *it;
    u8 id;
    u32 price;

    l = &D_801FC438.unk4;
    id = (D_8006794C->unk1C->unk574 + ((l->unk5 + l->unk6) * 2 + l->unk7))->unk0;
    it = &D_80069018[id];
    price = it->unk4 >> 1;
    func_80026C34(w->unk0 + 8, w->unk2 + 8, w->unk18, id, 1);
    func_8002B5FC(w->unk0 + 0x18, w->unk2 + 8, w->unk18, D_8006A218 + it->unk0, 0);
    func_800269F4(w->unk0 + 4, w->unk2 + 0x18, w->unk18, w->unk4 - 9);
    func_8002B5FC(w->unk0 + 0xC, w->unk2 + 0x1E, w->unk18, func_800357C0(D_801F3C9C, 0x19), 0);
    func_8002B5FC(w->unk0 + 0xC, w->unk2 + 0x2E, w->unk18, func_800357C0(D_801F3C9C, 0x1A), 0);
    m = &D_801FC438;
    func_8002BAE4(m->unk1, buf, 2, 1);
    func_8002B56C(w->unk0 + 0x5C, w->unk2 + 0x22, w->unk18, buf, 0);
    func_8002BAE4(m->unk1 * price, buf, 7, 1);
    func_80063108(buf, D_801FC28C);
    func_8002B56C(w->unk0 + 0x39, w->unk2 + 0x32, w->unk18, buf, 0);
}

INCLUDE_ASM("asm/us/ovl_3c/nonmatchings/ovl_3c", func_801F7DA8);

void func_801F7FE0(void) {
    MenuList *l;

    l = &D_801FC438.unk4;
    if (!func_8002F810()) {
        if (D_801FC438.unk0 == 1 || (D_80077DC0 & 3)) {
            D_80077DE8.unk4 = 0x14;
            D_80077DE8.unk0 = l->unk7 * 0x96;
            D_80077DE8.unk2 = l->unk6 * 0xF + 0x34;
            func_800355E4();
        }
        if (D_801FC438.unk0 == 2) {
            D_80077DE8.unk0 = D_801FC438.unk2C[0]->unk0 - 0xE;
            D_80077DE8.unk2 = D_801FC438.unk2C[0]->unk2 + 0x22;
            D_80077DE8.unk4 = D_801FC438.unk2C[0]->unk18 - 1;
            func_800355E4();
        }
    }
}

INCLUDE_ASM("asm/us/ovl_3c/nonmatchings/ovl_3c", func_801F80D0);

void func_801F8224(void) {
    MenuList *l;
    ShopSell *m;
    Unk80069018 *it;
    u8 id;
    u32 price;
    s32 n;
    s32 i;

    l = &D_801FC438.unk4;
    m = &D_801FC438;
    id = (D_8006794C->unk1C->unk574 + ((l->unk5 + l->unk6) * 2 + l->unk7))->unk0;
    it = &D_80069018[id];
    price = it->unk4 >> 1;
    n = func_80027E5C(id, D_801FC438.unk1);
    if (n != 0) {
        D_8006794C->unk1C->unk560 += price * n;
        if (D_8006794C->unk1C->unk560 > 9999999) {
            D_8006794C->unk1C->unk560 = 9999999;
        }
        D_801FC438.unk0 = 0;
        for (i = 0; i < 2; i++) {
            func_8002DD00(D_801FC438.unk2C[i]);
        }
    }
}

void func_801F832C(void) {
    func_801F837C();
    func_801F83D4();
    func_801F83FC();
}

void func_801F835C(void) {
    func_8003556C(NULL);
}

void func_801F837C(void) {
    void (*fn)(void);

    if (!func_8002F810()) {
        fn = D_801FC27C[D_801FC438.unk0];
        if (fn != NULL) {
            fn();
        }
    }
}

void func_801F83D4(void) {
    func_801F79FC();
    func_801F7FE0();
}

void func_801F83FC(void) {
    if (D_801FC438.unk4.unk8 != 0) {
        func_8002EB74(&D_801FC438.unk4);
    }
}

void func_801F8434(void) {
    MenuList *m = &D_801FC438.unk4;

    if (m->unk8 == 0) {
        if (D_80077528 & 0x10000) {
            func_8002F488(0x65);
            func_801F43F8();
            return;
        }
        if (D_80077528 & 0x20000) {
            func_801F80D0();
            return;
        }
        func_8002EE8C(m, D_80077528);
        func_8002F044(m, D_80077528);
        func_8002F0EC(m, D_80077528);
    }
}

void func_801F84D8(void) {
    MenuView v;
    s32 row;
    s32 col;
    s32 idx;

    func_8002F344(&D_801FC438.unk4, &v);
    idx = v.unk1 * 2;
    for (row = 0; row < v.unk0; row++) {
        for (col = 0; col < 2; col++) {
            func_801F856C(col, row, idx++, &v);
        }
    }
}

INCLUDE_ASM("asm/us/ovl_3c/nonmatchings/ovl_3c", func_801F856C);

void func_801F8680(void) {
    s32 i;

    D_801FC438.unk0 = 0;
    for (i = 0; i < 2; i++) {
        func_8002DD00(D_801FC438.unk2C[i]);
    }
}

void func_801F86D0(void) {
    D_801FC438.unk0 = 2;
    func_801F442C(0x13);
}

void func_801F86FC(void) {
    D_801FC438.unk0 = 1;
    func_801F442C(0x10);
}

void func_801F8728(void) {
    Unk80077DE8 *h;
    MenuList *l;
    s32 idx;
    UnkPair *p;
    u8 *text;

    h = &D_80077DE8;
    h->unk8 = NULL;
    if (D_801FC438.unk0 == 1) {
        l = &D_801FC438.unk4;
        idx = (l->unk5 + l->unk6) * 2 + l->unk7;
        p = &D_8006794C->unk1C->unk574[idx];
        if (p->unk1 == 0) {
            return;
        }
        text = D_801F87EC + D_80069018[p->unk0].unk2;
    } else if (D_801FC438.unk0 == 2) {
        text = func_80035774(3);
    } else {
        return;
    }
    h->unk8 = text;
}

INCLUDE_RODATA("asm/us/ovl_3c/nonmatchings/ovl_3c", D_801EDF00);

INCLUDE_RODATA("asm/us/ovl_3c/nonmatchings/ovl_3c", D_801EDF04);

INCLUDE_RODATA("asm/us/ovl_3c/nonmatchings/ovl_3c", D_801EDF3C);

INCLUDE_RODATA("asm/us/ovl_3c/nonmatchings/ovl_3c", D_801F36FC);

INCLUDE_RODATA("asm/us/ovl_3c/nonmatchings/ovl_3c", D_801F3C9C);

INCLUDE_RODATA("asm/us/ovl_3c/nonmatchings/ovl_3c", D_801F3E98);

INCLUDE_RODATA("asm/us/ovl_3c/nonmatchings/ovl_3c", D_801F3F08);

INCLUDE_RODATA("asm/us/ovl_3c/nonmatchings/ovl_3c", D_801F3F88);

INCLUDE_RODATA("asm/us/ovl_3c/nonmatchings/ovl_3c", D_801F3FF8);

INCLUDE_RODATA("asm/us/ovl_3c/nonmatchings/ovl_3c", D_801F3FFC);
