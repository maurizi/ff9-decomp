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

INCLUDE_ASM("asm/us/ovl_3c/nonmatchings/ovl_3c", func_801F44F4);

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

INCLUDE_ASM("asm/us/ovl_3c/nonmatchings/ovl_3c", func_801F4C68);

INCLUDE_ASM("asm/us/ovl_3c/nonmatchings/ovl_3c", func_801F4E1C);

INCLUDE_ASM("asm/us/ovl_3c/nonmatchings/ovl_3c", func_801F506C);

INCLUDE_ASM("asm/us/ovl_3c/nonmatchings/ovl_3c", func_801F51B8);

INCLUDE_ASM("asm/us/ovl_3c/nonmatchings/ovl_3c", func_801F551C);

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

void func_801F5D40(void) {
    D_801FC2F8.unk0 = 2;
    func_801F442C(0x12);
}

void func_801F5D6C(void) {
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

INCLUDE_ASM("asm/us/ovl_3c/nonmatchings/ovl_3c", func_801F60A8);

INCLUDE_ASM("asm/us/ovl_3c/nonmatchings/ovl_3c", func_801F625C);

INCLUDE_ASM("asm/us/ovl_3c/nonmatchings/ovl_3c", func_801F65C8);

INCLUDE_ASM("asm/us/ovl_3c/nonmatchings/ovl_3c", func_801F6714);

INCLUDE_ASM("asm/us/ovl_3c/nonmatchings/ovl_3c", func_801F6A7C);

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

INCLUDE_ASM("asm/us/ovl_3c/nonmatchings/ovl_3c", func_801F7250);

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
        func_80027E5C(r->unk2, n);
        func_80027E5C(r->unk3, n);
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

INCLUDE_ASM("asm/us/ovl_3c/nonmatchings/ovl_3c", func_801F7848);

INCLUDE_ASM("asm/us/ovl_3c/nonmatchings/ovl_3c", func_801F79FC);

INCLUDE_ASM("asm/us/ovl_3c/nonmatchings/ovl_3c", func_801F7BC4);

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
        text = func_801F87EC + D_80069018[p->unk0].unk2;
    } else if (D_801FC438.unk0 == 2) {
        text = func_80035774(3);
    } else {
        return;
    }
    h->unk8 = text;
}

INCLUDE_ASM("asm/us/ovl_3c/nonmatchings/ovl_3c", func_801F87EC);

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
