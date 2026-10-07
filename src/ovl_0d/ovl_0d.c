#include "ovl_0d.h"

/* Sits right after the last function, at the end of .text in the original. */
s32 D_800B365C = 0;

void func_800A752C(void) {
    func_800A755C();
    func_800A7E10();
    func_800A75E0();
}

void func_800A755C(void) {
    D_800B365C = 0;
    D_800B3660 = 0;
    D_8006794C->unk1C->unk83C = func_800A78EC;
    func_80021C60();
    func_800A7764();
    func_800A7634();
    func_800A76F8();
    func_800A78EC(0, 0x42, 0, 0, 0);
    func_800A7818();
    func_800A7884();
}

void func_800A75E0(void) {
    func_800A78B8();
    func_800A78EC(0x100, 0x42, 0, 0, 0);
    D_8006794C->unkA = 9;
    D_8006794C->unk8 = 1;
}

s32 func_800A7634(void) {
    s32 i;
    s32 h;
    s32 a, b;
    for (i = func_8001C8B0(D_800B365C, 1, 0x90000, 0) - 1; i >= 0; i--) {
        h = func_8001C8B0(D_800B365C, 3, (i & 0xFFFF) | 0x90000, 0);
        a = (s32)func_8001C7FC(4, h, 0);
        b = (s32)func_8001C7FC(5, h, 0);
        func_80055DF4();
        func_80055E14(a, b, 1);
    }
    return 0;
}

s32 func_800A76F8(void) {
    s32 p = (s32)func_8001C7FC(4, 0x70042, 0);
    memmove(D_8006794C->unk1C->unk814, (void *)p, (s32)func_8001C7FC(5, 0x70042, 0));
    return 0;
}

s32 func_800A7764(void) {
    s32 dst = (s32)(D_80067944 + 0x57800);
    FileEntry *f = func_8001E218(1, 0x2715);
    func_8002177C(D_8006794C->unk1C->unk854, f->unk4, (f->unkC - f->unk4) << 11, (void *)dst, 0, -1, 0);
    while (func_800217C4(D_8006794C->unk1C->unk854) != 0) {
    }
    D_800B365C = dst;
    func_8001C750(dst, 0);
    return 0;
}

s32 func_800A7818(void) {
    s32 end = func_8001C8B0(D_800B365C, 0, 0x70000, 0);
    D_800B3660 = (s32)(D_80067944 + 0x141000);
    memmove((void *)D_800B3660, (void *)D_800B365C, end - D_800B365C);
    func_8001C750(D_800B3660, 0);
    return 0;
}

s32 func_800A7884(void) {
    func_8001C784((void *)D_800B365C);
    D_800B365C = 0;
    return 0;
}

s32 func_800A78B8(void) {
    func_8001C784((void *)D_800B3660);
    D_800B3660 = 0;
    return 0;
}

INCLUDE_RODATA("asm/us/ovl_0d/nonmatchings/ovl_0d", D_800A7000);

INCLUDE_ASM("asm/us/ovl_0d/nonmatchings/ovl_0d", func_800A78EC);

INCLUDE_ASM("asm/us/ovl_0d/nonmatchings/ovl_0d", func_800A7E10);

void func_800A8B80(void) {
    ClearOTagR(D_8006794C->unk1C->unk10[D_8006794C->unk1C->unk8], 0x1000);
}

INCLUDE_ASM("asm/us/ovl_0d/nonmatchings/ovl_0d", func_800A8BC4);

INCLUDE_ASM("asm/us/ovl_0d/nonmatchings/ovl_0d", func_800A8D74);

INCLUDE_ASM("asm/us/ovl_0d/nonmatchings/ovl_0d", func_800A9298);

void func_800A932C(void) {
}

void func_800A9334(void) {
    FileEntry *f = func_8001E218(1, 0x44C);
    if (f == NULL) {
        func_8002EB64(0x46, 0x3E6);
    }
    func_80022DF0(D_8006794C->unk20 + f->unk4, (f->unkC - f->unk4) << 11, (void *)0x80104A00);
}

INCLUDE_ASM("asm/us/ovl_0d/nonmatchings/ovl_0d", func_800A93A0);

void func_800A93C8(u8 *arg0, s32 arg1, s32 arg2) {
    s32 i;
    u8 *src = func_800A93A0(arg1);
    for (i = 0; i < arg2; i++) {
        *arg0++ = *src++;
    }
}

INCLUDE_ASM("asm/us/ovl_0d/nonmatchings/ovl_0d", func_800A9424);

INCLUDE_ASM("asm/us/ovl_0d/nonmatchings/ovl_0d", func_800A96B0);

INCLUDE_ASM("asm/us/ovl_0d/nonmatchings/ovl_0d", func_800A986C);

INCLUDE_ASM("asm/us/ovl_0d/nonmatchings/ovl_0d", func_800A993C);

void func_800A9B0C(void) {
    func_8002C308(0);
}

INCLUDE_ASM("asm/us/ovl_0d/nonmatchings/ovl_0d", func_800A9B2C);

INCLUDE_ASM("asm/us/ovl_0d/nonmatchings/ovl_0d", func_800A9F10);

INCLUDE_ASM("asm/us/ovl_0d/nonmatchings/ovl_0d", func_800AA1FC);

void func_800AA368(TmState *arg0) {
    if (D_800B3CD0 == 0 && (D_80077528 & 0x30000)) {
        func_8002DD00(arg0->unk4B8);
        arg0->unk0 = 4;
    }
}

void func_800AA3C0(Window *arg0) {
    func_8002B5FC(arg0->unk0 + 9, arg0->unk2 + 7, arg0->unk18, func_80035774(0x19), 0);
}

INCLUDE_ASM("asm/us/ovl_0d/nonmatchings/ovl_0d", func_800AA408);

void func_800AA504(TmState *arg0) {
    if (D_800B3CD0 == 0 && (D_80077528 & 0xA0000)) {
        func_800AE54C(arg0);
    }
}

INCLUDE_ASM("asm/us/ovl_0d/nonmatchings/ovl_0d", func_800AA548);

INCLUDE_ASM("asm/us/ovl_0d/nonmatchings/ovl_0d", func_800AA790);

INCLUDE_ASM("asm/us/ovl_0d/nonmatchings/ovl_0d", func_800AAEB4);

void func_800AB060(Window *arg0) {
    func_8002B5FC(arg0->unk0 + 9, arg0->unk2 + 7, arg0->unk18, func_80035774(6), 0);
}

void func_800AB0A8(Window *arg0) {
    func_8002B5FC(arg0->unk0 + 9, arg0->unk2 + 7, arg0->unk18, func_80035774(0x1A), 0);
}

INCLUDE_ASM("asm/us/ovl_0d/nonmatchings/ovl_0d", func_800AB0F0);

void func_800AB2E0(TmState *arg0) {
    if (D_800B3CD0 == 0 && (D_80077528 & 0xA0000)) {
        func_8002DD00(arg0->unk4B8);
        arg0->unk0 = 0x17;
    }
}

INCLUDE_ASM("asm/us/ovl_0d/nonmatchings/ovl_0d", func_800AB338);

void func_800AB52C(Window *arg0) {
    func_8002B5FC(arg0->unk0 + 0xA, arg0->unk2 + 6, arg0->unk18, func_80035774(0x18), 0);
}

INCLUDE_ASM("asm/us/ovl_0d/nonmatchings/ovl_0d", func_800AB574);

void func_800AB854(TmState *arg0) {
    if (D_80077528 & 0x30000) {
        func_8002DD00(arg0->unk4C4);
        func_800B35F8(arg0);
    }
}

void func_800AB89C(Window *arg0) {
    func_8002E2E4(arg0, (void (*)(Window *))func_800AB8C8);
    func_800AB8E8(1);
}

void func_800AB8C8(void) {
    func_800AB8E8(0);
}

void func_800AB8E8(s32 arg0) {
    D_800B3CD0 = arg0;
}

void func_800AB8F4(s16 arg0) {
    RECT r;
    r.x = 0;
    r.y = 0;
    r.w = arg0;
    r.h = 0xF0;
    func_80035580(&r);
}

INCLUDE_ASM("asm/us/ovl_0d/nonmatchings/ovl_0d", func_800AB928);

INCLUDE_ASM("asm/us/ovl_0d/nonmatchings/ovl_0d", func_800ABC74);

void func_800ABE74(Window *arg0) {
    func_8002B5FC(arg0->unk0 + 9, arg0->unk2 + 6, arg0->unk18, func_80035774(0x2D), 0);
}

s32 func_800ABEBC(s32 arg0, s32 arg1) {
    return arg1;
}

INCLUDE_ASM("asm/us/ovl_0d/nonmatchings/ovl_0d", func_800ABEC4);

INCLUDE_ASM("asm/us/ovl_0d/nonmatchings/ovl_0d", func_800AC060);

INCLUDE_ASM("asm/us/ovl_0d/nonmatchings/ovl_0d", func_800AC4C8);

void func_800AC5AC(TmState *arg0) {
    if (func_800B0EB8(arg0, arg0->unk434, arg0->unk42C, arg0->unk430, -1) != 0) {
        func_800B1110(arg0, arg0->unk434, arg0->unk42C, arg0->unk430, -1, 2);
    } else {
        func_800ABEC4(arg0);
    }
}

void func_800AC624(TmState *arg0) {
    s32 who = arg0->unk434 == 0;
    if (func_800B0EB8(arg0, who, arg0->unk424, arg0->unk428, -1) != 0) {
        func_800B1110(arg0, who, arg0->unk424, arg0->unk428, 8, 2);
    } else {
        arg0->unk0 = 8;
    }
}

void func_800AC6A4(TmState *arg0) {
    if (arg0->unk434 == 0) {
        arg0->unk434 = 1;
        arg0->unk2E8 = 0;
        arg0->unk418 = 0;
        arg0->unk5A4 = (func_80062B3C() & 0xF) + 0xF;
        arg0->unk0 = 0x1E;
    } else {
        arg0->unk564 = 0;
        arg0->unk434 = 0;
        arg0->unk0 = 0;
    }
}

INCLUDE_ASM("asm/us/ovl_0d/nonmatchings/ovl_0d", func_800AC708);

INCLUDE_ASM("asm/us/ovl_0d/nonmatchings/ovl_0d", func_800ACDA0);

void func_800ACF8C(TmState *arg0) {
    if (arg0->unk2E8 >= 0x15) {
        arg0->unk2E8 = 0;
        arg0->unk0 = 5;
        func_800B01E8(9);
    }
}

void func_800ACFC8(TmState *arg0) {
    s32 i;
    TmUnk350 *p = arg0->unk350;
    arg0->unk2E8 = 0;
    arg0->unk418 = 0;
    for (i = 0; i < 10; i++) {
        p[i].unk0 = 0;
        p[i].unk4 = 0;
        p[i].unk8 = 0;
    }
}

INCLUDE_ASM("asm/us/ovl_0d/nonmatchings/ovl_0d", func_800ACFFC);

INCLUDE_ASM("asm/us/ovl_0d/nonmatchings/ovl_0d", func_800AD400);

void func_800AD974(TmState *arg0) {
    s32 i;
    TmCard *c = arg0->unk10;
    for (i = 0; i < 10; i++) {
        c[i].unk20 = c[i].unkC << 8;
        c[i].unk24 = c[i].unk10 << 8;
        if (i < 5) {
            c[i].unk28 = 0x10400;
            c[i].unk2C = i * 0x2000 + 0x1900;
        } else {
            c[i].unk28 = 0x1000;
            c[i].unk2C = (i - 5) * 0x2000 + 0x1900;
        }
    }
    arg0->unk460 = 0x14;
    arg0->unk2E8 = 0;
    arg0->unk0 = 0xE;
}

INCLUDE_ASM("asm/us/ovl_0d/nonmatchings/ovl_0d", func_800AD9F8);

s32 func_800ADDCC(TmState *arg0) {
    s32 n = 0;
    s32 i;
    TmCard *c = arg0->unk10;
    for (i = 0; i < arg0->unk550; i++) {
        if (c[i + 5].unk14 == 0) {
            n++;
        }
    }
    return n;
}

s32 func_800ADE14(TmState *arg0, s32 arg1) {
    s32 i;
    s32 n;
    TmCard *c;
    if (arg1 == 0) {
        c = &arg0->unk10[0];
    } else {
        c = &arg0->unk10[5];
    }
    n = 0;
    for (i = 0; i < 5; i++) {
        if (c[i].unk14 == arg1) {
            n++;
        }
    }
    return n;
}

INCLUDE_ASM("asm/us/ovl_0d/nonmatchings/ovl_0d", func_800ADE5C);

void func_800ADFFC(Window *arg0) {
    func_8002B5FC(arg0->unk0 + 9, arg0->unk2 + 7, arg0->unk18, func_80035774(0x16), 0);
}

void func_800AE044(Window *arg0) {
    func_8002B5FC(0x82, 0x10, arg0->unk18, func_80035774(0x17), 0);
}

INCLUDE_ASM("asm/us/ovl_0d/nonmatchings/ovl_0d", func_800AE084);

INCLUDE_ASM("asm/us/ovl_0d/nonmatchings/ovl_0d", func_800AE398);

INCLUDE_ASM("asm/us/ovl_0d/nonmatchings/ovl_0d", func_800AE494);

void func_800AE54C(TmState *arg0) {
    TmCard *c = arg0->unk470;
    c->unk30 = 0x28;
    c->unk20 = c->unkC << 8;
    c->unk24 = c->unk10 << 8;
    arg0->unk2E8 = 0;
    arg0->unk0 = 0x11;
    func_8002DD00(arg0->unk458);
}

INCLUDE_ASM("asm/us/ovl_0d/nonmatchings/ovl_0d", func_800AE5A0);

INCLUDE_ASM("asm/us/ovl_0d/nonmatchings/ovl_0d", func_800AE6EC);

s32 func_800AE830(TmState *arg0, s32 arg1) {
    s32 n = 0;
    s32 i;
    TmCard *c = arg0->unk10;
    for (i = 0; i < 10; i++) {
        if (c[i].unk14 == arg1 && c[i].unk18 == 0) {
            n++;
        }
    }
    return n;
}

s32 func_800AE878(TmState *arg0, s32 arg1) {
    s32 n = 0;
    s32 i;
    TmCard *c = arg0->unk10;
    for (i = 0; i < 5; i++) {
        if (c[i].unk34 != 0 && c[i].unk14 == arg1 && c[i].unk18 == 0) {
            n++;
        }
    }
    return n;
}

s32 func_800AE8D0(TmState *arg0) {
    s32 n = 0;
    s32 i;
    TmCard *c = arg0->unk10 + 5;
    for (i = 0; i < 5; i++) {
        if (c[i].unk14 == 0) {
            n++;
        }
    }
    return n;
}

TmCard *func_800AE908(TmState *arg0, s32 arg1) {
    s32 i;
    TmCard *base;
    TmCard *c;
    i = 0;
    c = base = &arg0->unk10[5];
    for (; i < 5; i++, c++) {
        if (c->unk14 == 0) {
            if (arg1 == 0) {
                return c;
            }
            arg1--;
        }
    }
    return base;
}

s32 func_800AE94C(TmState *arg0, s32 arg1) {
    s32 n = 0;
    s32 i;
    TmCard *c = arg0->unk10;
    for (i = 0; i < 10; i++) {
        if (c[i].unk18 == 1 && c[i].unk14 == arg1) {
            n++;
        }
    }
    return n;
}

s32 func_800AE998(TmState *arg0, s32 arg1) {
    s32 n = 0;
    s32 i;
    TmCard *c = arg0->unk10;
    for (i = 0; i < 10; i++) {
        if (c[i].unk14 == arg1) {
            n++;
        }
    }
    return n;
}

TmCard *func_800AE9D0(TmState *arg0, s32 arg1, s32 arg2) {
    s32 n = 0;
    s32 i;
    TmCard *c = arg0->unk10;
    for (i = 0; i < 10; i++) {
        if (c[i].unk14 == arg1 && c[i].unk18 == 0) {
            if (arg2 == n) {
                return &c[i];
            }
            n++;
        }
    }
    return NULL;
}

INCLUDE_ASM("asm/us/ovl_0d/nonmatchings/ovl_0d", func_800AEA24);

INCLUDE_ASM("asm/us/ovl_0d/nonmatchings/ovl_0d", func_800AEBB4);

TmCard *func_800AEE08(TmState *arg0, s32 arg1, s32 arg2) {
    return func_800B19D4(arg0, arg1, arg2)->unk4;
}

INCLUDE_RODATA("asm/us/ovl_0d/nonmatchings/ovl_0d", D_800A72B8);

INCLUDE_ASM("asm/us/ovl_0d/nonmatchings/ovl_0d", func_800AEE2C);

void func_800AF100(TmState *arg0) {
    s32 i;
    TmCard *c = arg0->unk10;
    for (i = 0; i < 5; i++) {
        if (c[i].unk14 == 0) {
            func_80062420(c[i].unk0, c[i].unk1, c[i].unk2, c[i].unk3, c[i].unk4, c[i].unk5);
        }
    }
}

void func_800AF170(TmState *arg0) {
    s32 i;
    TmCard *c = arg0->unk10;
    for (i = 0; i < 5; i++) {
        if (arg0->unk45C == 0) {
            func_800AF388(arg0, &c[i]);
        }
        func_80062420(c[i].unk0, c[i].unk1, c[i].unk2, c[i].unk3, c[i].unk4, c[i].unk5);
    }
}

void func_800AF1F4(TmState *arg0, u8 arg1) {
    TmCard c;
    if (func_800625F8() < 0x64) {
        func_800AF3E8(arg0, &c, arg1, 1);
        func_80062420(arg1, c.unk1, c.unk2, c.unk3, c.unk4, c.unk5);
    }
}

s32 func_800AF268(TmState *arg0) {
    return arg0->unk474 * 10 + arg0->unk478;
}

void func_800AF288(TmState *arg0) {
    s32 i;
    for (i = 9; i >= 0; i--) {
        arg0->unk10[i].unk34 = 0;
    }
}

void func_800AF2A8(TmState *arg0) {
    s32 i;
    s32 one = 1;
    for (i = 9; i >= 0; i--) {
        arg0->unk10[i].unk34 = one;
    }
}

INCLUDE_ASM("asm/us/ovl_0d/nonmatchings/ovl_0d", func_800AF2CC);

INCLUDE_ASM("asm/us/ovl_0d/nonmatchings/ovl_0d", func_800AF388);

INCLUDE_ASM("asm/us/ovl_0d/nonmatchings/ovl_0d", func_800AF3E8);

INCLUDE_ASM("asm/us/ovl_0d/nonmatchings/ovl_0d", func_800AF760);

INCLUDE_ASM("asm/us/ovl_0d/nonmatchings/ovl_0d", func_800AF890);

INCLUDE_ASM("asm/us/ovl_0d/nonmatchings/ovl_0d", func_800AF95C);

INCLUDE_ASM("asm/us/ovl_0d/nonmatchings/ovl_0d", func_800AFB60);

INCLUDE_ASM("asm/us/ovl_0d/nonmatchings/ovl_0d", func_800AFBE4);

INCLUDE_ASM("asm/us/ovl_0d/nonmatchings/ovl_0d", func_800AFCE4);

void func_800B01E8(s32 arg0) {
    func_800A78EC(0xD000, D_800B3C8C[arg0], 0, 0x80, 0x7F);
}

void func_800B022C(s32 arg0) {
    func_800A78EC(0x5100, D_800B3C8C[arg0], 0, 0, 0);
}

INCLUDE_ASM("asm/us/ovl_0d/nonmatchings/ovl_0d", func_800B026C);

INCLUDE_ASM("asm/us/ovl_0d/nonmatchings/ovl_0d", func_800B030C);

INCLUDE_ASM("asm/us/ovl_0d/nonmatchings/ovl_0d", func_800B0618);

INCLUDE_ASM("asm/us/ovl_0d/nonmatchings/ovl_0d", func_800B07DC);

INCLUDE_ASM("asm/us/ovl_0d/nonmatchings/ovl_0d", func_800B08CC);

s32 func_800B0E58(TmState *arg0, s32 arg1, s32 arg2, s32 arg3) {
    TmCell *cell = func_800B19D4(arg0, arg2, arg3);
    if (cell->unk0 == 0) {
        TmCard *c = cell->unk4;
        if (c->unk14 != arg1) {
            c->unk14 = arg1;
        }
    }
    return 0;
}

INCLUDE_ASM("asm/us/ovl_0d/nonmatchings/ovl_0d", func_800B0EB8);

INCLUDE_ASM("asm/us/ovl_0d/nonmatchings/ovl_0d", func_800B1010);

INCLUDE_ASM("asm/us/ovl_0d/nonmatchings/ovl_0d", func_800B1110);

INCLUDE_ASM("asm/us/ovl_0d/nonmatchings/ovl_0d", func_800B12F0);

INCLUDE_ASM("asm/us/ovl_0d/nonmatchings/ovl_0d", func_800B13C0);

INCLUDE_ASM("asm/us/ovl_0d/nonmatchings/ovl_0d", func_800B14AC);

INCLUDE_ASM("asm/us/ovl_0d/nonmatchings/ovl_0d", func_800B16A4);

TmCell *func_800B19D4(TmState *arg0, s32 arg1, s32 arg2) {
    TmCell(*board)[4] = arg0->unk268;
    if (arg1 < 0) {
        return &D_800B3CE8;
    }
    if (arg1 >= 4) {
        return &D_800B3CE8;
    }
    if (arg2 < 0) {
        return &D_800B3CE8;
    }
    if (arg2 >= 4) {
        return &D_800B3CE8;
    }
    return &board[arg2][arg1];
}

void func_800B1A24(TmState *arg0) {
    s32 x, y;
    TmCell(*b)[4] = arg0->unk268;
    D_800B3CE8.unk0 = 3;
    for (y = 0; y < 4; y++) {
        for (x = 3; x >= 0; x--) {
            b[y][x].unk0 = 1;
        }
    }
    func_800B0618(arg0);
}

INCLUDE_RODATA("asm/us/ovl_0d/nonmatchings/ovl_0d", D_800A73B4);

INCLUDE_RODATA("asm/us/ovl_0d/nonmatchings/ovl_0d", D_800A73BC);

INCLUDE_RODATA("asm/us/ovl_0d/nonmatchings/ovl_0d", D_800A73C4);

INCLUDE_ASM("asm/us/ovl_0d/nonmatchings/ovl_0d", func_800B1A84);

void func_800B295C(TmState *arg0) {
    D_800B3CF4 = func_80062638();
    D_800B3CF0 = func_800625F8();
    arg0->unk0 = 0x12;
}

void func_800B299C(Window *arg0) {
    func_8002B5FC(0x8A, 0x10, arg0->unk18, func_80035774(0x13), 0);
}

void func_800B29DC(TmState *arg0) {
    arg0->unk474 = 0;
    arg0->unk478 = 0;
    arg0->unk47C = 0;
    arg0->unk4B4 = 0;
}

INCLUDE_ASM("asm/us/ovl_0d/nonmatchings/ovl_0d", func_800B29F0);

INCLUDE_ASM("asm/us/ovl_0d/nonmatchings/ovl_0d", func_800B2C5C);

INCLUDE_ASM("asm/us/ovl_0d/nonmatchings/ovl_0d", func_800B2F60);

INCLUDE_ASM("asm/us/ovl_0d/nonmatchings/ovl_0d", func_800B3198);

INCLUDE_ASM("asm/us/ovl_0d/nonmatchings/ovl_0d", func_800B33CC);

s32 func_800B3480(s32 arg0, s32 arg1) {
    return arg1;
}

INCLUDE_ASM("asm/us/ovl_0d/nonmatchings/ovl_0d", func_800B3488);

void func_800B355C(Window *arg0) {
    func_8002B5FC(arg0->unk0 + 9, arg0->unk2 + 6, arg0->unk18, func_80035774(2), 0);
}

void func_800B35A4(TmState *arg0) {
    func_8002F93C((arg0->unk2E8 * 0x88) & 0xFF8);
    if (arg0->unk2E8 >= 0x1F) {
        arg0->unk0 = 0xC;
    }
}

void func_800B35F8(TmState *arg0) {
    arg0->unk2E8 = 0;
    func_800320B8(2, 1, 0x1E, 0xFFFFFF);
    func_8002F93C(0);
    func_800A78EC(0xC401, 0x42, 0x1E, 0x7F, 0);
    arg0->unk0 = 0x1B;
}

INCLUDE_RODATA("asm/us/ovl_0d/nonmatchings/ovl_0d", D_800A74A8);

INCLUDE_RODATA("asm/us/ovl_0d/nonmatchings/ovl_0d", D_800A74AC);
