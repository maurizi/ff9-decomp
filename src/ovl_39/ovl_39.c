#include "ovl_39.h"

INCLUDE_ASM("asm/us/ovl_39/nonmatchings/ovl_39", func_801F4414);

INCLUDE_ASM("asm/us/ovl_39/nonmatchings/ovl_39", func_801F4538);

INCLUDE_ASM("asm/us/ovl_39/nonmatchings/ovl_39", func_801F4664);

INCLUDE_ASM("asm/us/ovl_39/nonmatchings/ovl_39", func_801F4760);

INCLUDE_ASM("asm/us/ovl_39/nonmatchings/ovl_39", func_801F4840);

INCLUDE_RODATA("asm/us/ovl_39/nonmatchings/ovl_39", D_801EDF00);

INCLUDE_ASM("asm/us/ovl_39/nonmatchings/ovl_39", func_801F49C4);

INCLUDE_ASM("asm/us/ovl_39/nonmatchings/ovl_39", func_801F4B18);

INCLUDE_ASM("asm/us/ovl_39/nonmatchings/ovl_39", func_801F4E00);

INCLUDE_ASM("asm/us/ovl_39/nonmatchings/ovl_39", func_801F4F30);

INCLUDE_ASM("asm/us/ovl_39/nonmatchings/ovl_39", func_801F500C);

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

INCLUDE_ASM("asm/us/ovl_39/nonmatchings/ovl_39", func_801F53E0);

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

INCLUDE_ASM("asm/us/ovl_39/nonmatchings/ovl_39", func_801F55D8);

INCLUDE_ASM("asm/us/ovl_39/nonmatchings/ovl_39", func_801F5688);

INCLUDE_ASM("asm/us/ovl_39/nonmatchings/ovl_39", func_801F573C);

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
    s32 i = D_801FB1A8.unk29 + D_801FB1A8.unk2A;

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

INCLUDE_ASM("asm/us/ovl_39/nonmatchings/ovl_39", func_801F5BE0);

INCLUDE_ASM("asm/us/ovl_39/nonmatchings/ovl_39", func_801F5D00);

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

INCLUDE_ASM("asm/us/ovl_39/nonmatchings/ovl_39", func_801F5FE4);

INCLUDE_ASM("asm/us/ovl_39/nonmatchings/ovl_39", func_801F60DC);

void func_801F61B0(void) {
    D_801FBEE0.unk0 = 0;
}

INCLUDE_ASM("asm/us/ovl_39/nonmatchings/ovl_39", func_801F61BC);

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

INCLUDE_ASM("asm/us/ovl_39/nonmatchings/ovl_39", func_801F65C4);

INCLUDE_ASM("asm/us/ovl_39/nonmatchings/ovl_39", func_801F6694);

INCLUDE_ASM("asm/us/ovl_39/nonmatchings/ovl_39", func_801F6938);

INCLUDE_ASM("asm/us/ovl_39/nonmatchings/ovl_39", func_801F6A84);

INCLUDE_ASM("asm/us/ovl_39/nonmatchings/ovl_39", func_801F6B48);

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

INCLUDE_ASM("asm/us/ovl_39/nonmatchings/ovl_39", func_801F6C74);

s32 func_801F6CD0(s32 n) {
    s32 found = 0;
    s32 i;

    for (i = 0; i < 15; i++) {
        if (D_801FB1A8.unk4E[i].unk0 == 4) {
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

INCLUDE_ASM("asm/us/ovl_39/nonmatchings/ovl_39", func_801F6FE8);

INCLUDE_ASM("asm/us/ovl_39/nonmatchings/ovl_39", func_801F7084);

void func_801F70E4(void) {
    if (D_801FB1A8.unk2 != 1) {
        D_801FB1A8.unk2 = 2;
    }
}

INCLUDE_ASM("asm/us/ovl_39/nonmatchings/ovl_39", func_801F7108);

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

INCLUDE_ASM("asm/us/ovl_39/nonmatchings/ovl_39", func_801F7364);

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

void func_801F74F4(void) {
    D_801FB1A8.unk2 = 2;
    D_801FB1A8.unk1 = 6;
}

void func_801F7510(void) {
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

INCLUDE_ASM("asm/us/ovl_39/nonmatchings/ovl_39", func_801F7840);

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

INCLUDE_ASM("asm/us/ovl_39/nonmatchings/ovl_39", func_801F7970);

INCLUDE_ASM("asm/us/ovl_39/nonmatchings/ovl_39", func_801F79F4);

INCLUDE_RODATA("asm/us/ovl_39/nonmatchings/ovl_39", D_801EDF14);

INCLUDE_RODATA("asm/us/ovl_39/nonmatchings/ovl_39", D_801EDF1C);

INCLUDE_RODATA("asm/us/ovl_39/nonmatchings/ovl_39", D_801EE4D4);

INCLUDE_RODATA("asm/us/ovl_39/nonmatchings/ovl_39", D_801F2854);

INCLUDE_RODATA("asm/us/ovl_39/nonmatchings/ovl_39", D_801F4104);

INCLUDE_RODATA("asm/us/ovl_39/nonmatchings/ovl_39", D_801F41A4);

INCLUDE_RODATA("asm/us/ovl_39/nonmatchings/ovl_39", D_801F4244);

INCLUDE_RODATA("asm/us/ovl_39/nonmatchings/ovl_39", D_801F42E4);

INCLUDE_ASM("asm/us/ovl_39/nonmatchings/ovl_39", func_801F7AF4);

INCLUDE_ASM("asm/us/ovl_39/nonmatchings/ovl_39", func_801F7C20);

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

void func_801F8408(void) {
    D_801FB1A8.unk2 = 2;
    D_801FC388.unk2 = 1;
}

void func_801F8424(void) {
    D_801FC388.unk0 = 0;
    D_801FB1A8.unk2 = 2;
    if (D_801FB1A8.unk0 != 1) {
        func_801F7364(&D_801FB1A8);
        return;
    }
    D_801FB1A8.unk1 = 4;
}

INCLUDE_ASM("asm/us/ovl_39/nonmatchings/ovl_39", func_801F8478);

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

INCLUDE_ASM("asm/us/ovl_39/nonmatchings/ovl_39", func_801F8588);

INCLUDE_ASM("asm/us/ovl_39/nonmatchings/ovl_39", func_801F8610);

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

INCLUDE_ASM("asm/us/ovl_39/nonmatchings/ovl_39", func_801F8904);

void func_801F89A0(Window *w) {
    D_801FC388.unk0 = 0;
    D_801FB1A8.unk2 = 2;
}

INCLUDE_ASM("asm/us/ovl_39/nonmatchings/ovl_39", func_801F89B8);

INCLUDE_ASM("asm/us/ovl_39/nonmatchings/ovl_39", func_801F8B10);

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

INCLUDE_ASM("asm/us/ovl_39/nonmatchings/ovl_39", func_801F8C34);

INCLUDE_ASM("asm/us/ovl_39/nonmatchings/ovl_39", func_801F8CE8);

INCLUDE_ASM("asm/us/ovl_39/nonmatchings/ovl_39", func_801F8DA4);

INCLUDE_ASM("asm/us/ovl_39/nonmatchings/ovl_39", func_801F9024);

INCLUDE_ASM("asm/us/ovl_39/nonmatchings/ovl_39", func_801F9120);

INCLUDE_ASM("asm/us/ovl_39/nonmatchings/ovl_39", func_801F94D0);

INCLUDE_ASM("asm/us/ovl_39/nonmatchings/ovl_39", func_801F95E4);

INCLUDE_ASM("asm/us/ovl_39/nonmatchings/ovl_39", func_801F963C);

INCLUDE_ASM("asm/us/ovl_39/nonmatchings/ovl_39", func_801F9688);

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

INCLUDE_ASM("asm/us/ovl_39/nonmatchings/ovl_39", func_801F9788);

INCLUDE_ASM("asm/us/ovl_39/nonmatchings/ovl_39", func_801F9804);

void func_801F98D0(void) {
    if (!func_801F79F4(1)) {
        func_801F7780(0x34);
    }
}

INCLUDE_ASM("asm/us/ovl_39/nonmatchings/ovl_39", func_801F9904);

INCLUDE_ASM("asm/us/ovl_39/nonmatchings/ovl_39", func_801F99FC);

INCLUDE_ASM("asm/us/ovl_39/nonmatchings/ovl_39", func_801F9B00);

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

INCLUDE_ASM("asm/us/ovl_39/nonmatchings/ovl_39", func_801F9C58);

void func_801F9CE8(void *p) {
    func_801F9C58(p, func_80062824());
}

void func_801F9D18(void *p) {
    func_801F9C58(func_80062824(), p);
}

INCLUDE_ASM("asm/us/ovl_39/nonmatchings/ovl_39", func_801F9D48);

INCLUDE_ASM("asm/us/ovl_39/nonmatchings/ovl_39", func_801F9DF8);

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

INCLUDE_ASM("asm/us/ovl_39/nonmatchings/ovl_39", func_801FA0F4);

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

void func_801FA240(s32 chan) {
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

INCLUDE_ASM("asm/us/ovl_39/nonmatchings/ovl_39", func_801FA3D8);

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

INCLUDE_ASM("asm/us/ovl_39/nonmatchings/ovl_39", func_801FA6AC);

s32 func_801FA754(s32 chan) {
    func_80065228(0);
    func_80065228(1);
    func_801FAAF4();
    return _card_format(chan);
}

INCLUDE_RODATA("asm/us/ovl_39/nonmatchings/ovl_39", D_801F43EC);

INCLUDE_RODATA("asm/us/ovl_39/nonmatchings/ovl_39", D_801F43F4);

INCLUDE_ASM("asm/us/ovl_39/nonmatchings/ovl_39", func_801FA794);

s32 func_801FAAE4(void) {
    return D_801FC604;
}

void func_801FAAF4(void) {
    D_801FB074 = 0;
    D_801FB078 = 0;
}

INCLUDE_ASM("asm/us/ovl_39/nonmatchings/ovl_39", func_801FAB08);

INCLUDE_ASM("asm/us/ovl_39/nonmatchings/ovl_39", func_801FABB8);

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

INCLUDE_ASM("asm/us/ovl_39/nonmatchings/ovl_39", func_801FAECC);

INCLUDE_ASM("asm/us/ovl_39/nonmatchings/ovl_39", func_801FAF74);
