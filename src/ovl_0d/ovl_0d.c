#include "ovl_0d.h"

void func_800A752C(void) {
    func_800A755C();
    func_800A7E10();
    func_800A75E0();
}

INCLUDE_ASM("asm/us/ovl_0d/nonmatchings/ovl_0d", func_800A755C);

INCLUDE_ASM("asm/us/ovl_0d/nonmatchings/ovl_0d", func_800A75E0);

INCLUDE_ASM("asm/us/ovl_0d/nonmatchings/ovl_0d", func_800A7634);

INCLUDE_ASM("asm/us/ovl_0d/nonmatchings/ovl_0d", func_800A76F8);

INCLUDE_ASM("asm/us/ovl_0d/nonmatchings/ovl_0d", func_800A7764);

INCLUDE_ASM("asm/us/ovl_0d/nonmatchings/ovl_0d", func_800A7818);

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

INCLUDE_ASM("asm/us/ovl_0d/nonmatchings/ovl_0d", func_800A8B80);

INCLUDE_ASM("asm/us/ovl_0d/nonmatchings/ovl_0d", func_800A8BC4);

INCLUDE_ASM("asm/us/ovl_0d/nonmatchings/ovl_0d", func_800A8D74);

INCLUDE_ASM("asm/us/ovl_0d/nonmatchings/ovl_0d", func_800A9298);

void func_800A932C(void) {
}

INCLUDE_ASM("asm/us/ovl_0d/nonmatchings/ovl_0d", func_800A9334);

INCLUDE_ASM("asm/us/ovl_0d/nonmatchings/ovl_0d", func_800A93A0);

INCLUDE_ASM("asm/us/ovl_0d/nonmatchings/ovl_0d", func_800A93C8);

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

INCLUDE_ASM("asm/us/ovl_0d/nonmatchings/ovl_0d", func_800AA368);

void func_800AA3C0(WinCtx *arg0) {
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

void func_800AB060(WinCtx *arg0) {
    func_8002B5FC(arg0->unk0 + 9, arg0->unk2 + 7, arg0->unk18, func_80035774(6), 0);
}

void func_800AB0A8(WinCtx *arg0) {
    func_8002B5FC(arg0->unk0 + 9, arg0->unk2 + 7, arg0->unk18, func_80035774(0x1A), 0);
}

INCLUDE_ASM("asm/us/ovl_0d/nonmatchings/ovl_0d", func_800AB0F0);

INCLUDE_ASM("asm/us/ovl_0d/nonmatchings/ovl_0d", func_800AB2E0);

INCLUDE_ASM("asm/us/ovl_0d/nonmatchings/ovl_0d", func_800AB338);

void func_800AB52C(WinCtx *arg0) {
    func_8002B5FC(arg0->unk0 + 0xA, arg0->unk2 + 6, arg0->unk18, func_80035774(0x18), 0);
}

INCLUDE_ASM("asm/us/ovl_0d/nonmatchings/ovl_0d", func_800AB574);

INCLUDE_ASM("asm/us/ovl_0d/nonmatchings/ovl_0d", func_800AB854);

void func_800AB89C(s32 arg0) {
    func_8002E2E4(arg0, func_800AB8C8);
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

void func_800ABE74(WinCtx *arg0) {
    func_8002B5FC(arg0->unk0 + 9, arg0->unk2 + 6, arg0->unk18, func_80035774(0x2D), 0);
}

s32 func_800ABEBC(s32 arg0, s32 arg1) {
    return arg1;
}

INCLUDE_ASM("asm/us/ovl_0d/nonmatchings/ovl_0d", func_800ABEC4);

INCLUDE_ASM("asm/us/ovl_0d/nonmatchings/ovl_0d", func_800AC060);

INCLUDE_ASM("asm/us/ovl_0d/nonmatchings/ovl_0d", func_800AC4C8);

INCLUDE_ASM("asm/us/ovl_0d/nonmatchings/ovl_0d", func_800AC5AC);

INCLUDE_ASM("asm/us/ovl_0d/nonmatchings/ovl_0d", func_800AC624);

INCLUDE_ASM("asm/us/ovl_0d/nonmatchings/ovl_0d", func_800AC6A4);

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

INCLUDE_ASM("asm/us/ovl_0d/nonmatchings/ovl_0d", func_800AD974);

INCLUDE_ASM("asm/us/ovl_0d/nonmatchings/ovl_0d", func_800AD9F8);

INCLUDE_ASM("asm/us/ovl_0d/nonmatchings/ovl_0d", func_800ADDCC);

INCLUDE_ASM("asm/us/ovl_0d/nonmatchings/ovl_0d", func_800ADE14);

INCLUDE_ASM("asm/us/ovl_0d/nonmatchings/ovl_0d", func_800ADE5C);

void func_800ADFFC(WinCtx *arg0) {
    func_8002B5FC(arg0->unk0 + 9, arg0->unk2 + 7, arg0->unk18, func_80035774(0x16), 0);
}

void func_800AE044(WinCtx *arg0) {
    func_8002B5FC(0x82, 0x10, arg0->unk18, func_80035774(0x17), 0);
}

INCLUDE_ASM("asm/us/ovl_0d/nonmatchings/ovl_0d", func_800AE084);

INCLUDE_ASM("asm/us/ovl_0d/nonmatchings/ovl_0d", func_800AE398);

INCLUDE_ASM("asm/us/ovl_0d/nonmatchings/ovl_0d", func_800AE494);

INCLUDE_ASM("asm/us/ovl_0d/nonmatchings/ovl_0d", func_800AE54C);

INCLUDE_ASM("asm/us/ovl_0d/nonmatchings/ovl_0d", func_800AE5A0);

INCLUDE_ASM("asm/us/ovl_0d/nonmatchings/ovl_0d", func_800AE6EC);

INCLUDE_ASM("asm/us/ovl_0d/nonmatchings/ovl_0d", func_800AE830);

INCLUDE_ASM("asm/us/ovl_0d/nonmatchings/ovl_0d", func_800AE878);

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

INCLUDE_ASM("asm/us/ovl_0d/nonmatchings/ovl_0d", func_800AE908);

INCLUDE_ASM("asm/us/ovl_0d/nonmatchings/ovl_0d", func_800AE94C);

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

INCLUDE_ASM("asm/us/ovl_0d/nonmatchings/ovl_0d", func_800AE9D0);

INCLUDE_ASM("asm/us/ovl_0d/nonmatchings/ovl_0d", func_800AEA24);

INCLUDE_ASM("asm/us/ovl_0d/nonmatchings/ovl_0d", func_800AEBB4);

TmCard *func_800AEE08(TmState *arg0, s32 arg1, s32 arg2) {
    return func_800B19D4(arg0, arg1, arg2)->unk4;
}

INCLUDE_RODATA("asm/us/ovl_0d/nonmatchings/ovl_0d", D_800A72B8);

INCLUDE_ASM("asm/us/ovl_0d/nonmatchings/ovl_0d", func_800AEE2C);

INCLUDE_ASM("asm/us/ovl_0d/nonmatchings/ovl_0d", func_800AF100);

INCLUDE_ASM("asm/us/ovl_0d/nonmatchings/ovl_0d", func_800AF170);

INCLUDE_ASM("asm/us/ovl_0d/nonmatchings/ovl_0d", func_800AF1F4);

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

INCLUDE_ASM("asm/us/ovl_0d/nonmatchings/ovl_0d", func_800B0E58);

INCLUDE_ASM("asm/us/ovl_0d/nonmatchings/ovl_0d", func_800B0EB8);

INCLUDE_ASM("asm/us/ovl_0d/nonmatchings/ovl_0d", func_800B1010);

INCLUDE_ASM("asm/us/ovl_0d/nonmatchings/ovl_0d", func_800B1110);

INCLUDE_ASM("asm/us/ovl_0d/nonmatchings/ovl_0d", func_800B12F0);

INCLUDE_ASM("asm/us/ovl_0d/nonmatchings/ovl_0d", func_800B13C0);

INCLUDE_ASM("asm/us/ovl_0d/nonmatchings/ovl_0d", func_800B14AC);

INCLUDE_ASM("asm/us/ovl_0d/nonmatchings/ovl_0d", func_800B16A4);

INCLUDE_ASM("asm/us/ovl_0d/nonmatchings/ovl_0d", func_800B19D4);

INCLUDE_ASM("asm/us/ovl_0d/nonmatchings/ovl_0d", func_800B1A24);

INCLUDE_RODATA("asm/us/ovl_0d/nonmatchings/ovl_0d", D_800A73B4);

INCLUDE_RODATA("asm/us/ovl_0d/nonmatchings/ovl_0d", D_800A73BC);

INCLUDE_RODATA("asm/us/ovl_0d/nonmatchings/ovl_0d", D_800A73C4);

INCLUDE_ASM("asm/us/ovl_0d/nonmatchings/ovl_0d", func_800B1A84);

void func_800B295C(TmState *arg0) {
    D_800B3CF4 = func_80062638();
    D_800B3CF0 = func_800625F8();
    arg0->unk0 = 0x12;
}

void func_800B299C(WinCtx *arg0) {
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

void func_800B355C(WinCtx *arg0) {
    func_8002B5FC(arg0->unk0 + 9, arg0->unk2 + 6, arg0->unk18, func_80035774(2), 0);
}

INCLUDE_ASM("asm/us/ovl_0d/nonmatchings/ovl_0d", func_800B35A4);

INCLUDE_ASM("asm/us/ovl_0d/nonmatchings/ovl_0d", func_800B35F8);

INCLUDE_RODATA("asm/us/ovl_0d/nonmatchings/ovl_0d", D_800A74A8);

INCLUDE_RODATA("asm/us/ovl_0d/nonmatchings/ovl_0d", D_800A74AC);
