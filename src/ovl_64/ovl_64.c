#include "ovl_64.h"

INCLUDE_ASM("asm/us/ovl_64/nonmatchings/ovl_64", func_801990A0);

INCLUDE_ASM("asm/us/ovl_64/nonmatchings/ovl_64", func_80199554);

INCLUDE_RODATA("asm/us/ovl_64/nonmatchings/ovl_64", D_80198700);

INCLUDE_ASM("asm/us/ovl_64/nonmatchings/ovl_64", func_801999B4);

INCLUDE_ASM("asm/us/ovl_64/nonmatchings/ovl_64", func_80199B28);

INCLUDE_ASM("asm/us/ovl_64/nonmatchings/ovl_64", func_80199F30);

INCLUDE_ASM("asm/us/ovl_64/nonmatchings/ovl_64", func_8019A5B0);

void func_8019A688(s32 h, s32 hires, s32 i) {
    s32 w = 0x140;

    if (hires != 0) {
        w = 0x1E0;
    }
    SetDefDispEnv(&D_8006794C->unkC[i], w, 0, w, h);
    if (hires != 0) {
        D_8006794C->unkC[i].isrgb24 = 1;
        D_8006794C->unkC[i].disp.w = D_8006794C->unkC->disp.w * 2 / 3;
    }
    D_8006794C->unkC[i].isinter = 0;
    D_8006794C->unkC[i].screen.x = 0;
    D_8006794C->unkC[i].screen.y = (0xF0 - h) >> 1;
    D_8006794C->unkC[i].screen.h = h;
}

INCLUDE_ASM("asm/us/ovl_64/nonmatchings/ovl_64", func_8019A7A8);

s32 func_8019A9D8(void) {
    if (D_80071E32 == 0) {
        return 0;
    }
    D_8019DC94.unk48 = 0;
    D_8019DC94.unk4C = 0;
    D_8019DC94.unk58 = 0;
    D_8019DC94.unk5C = 0;
    D_8019DC94.unk44 = 0;
    D_8019DC94.unk50 = 0;
    D_8019DC94.unk54 = 0;
    D_80071E32 = 0;
    return 1;
}

s32 func_8019AA20(DISPENV *env) {
    VSync(0);
    PutDispEnv(env);
    SetDispMask(1);
    return 1;
}

s32 func_8019AA5C(void) {
    u8 done;

    if (D_80071E32 < 2) {
        return 0;
    }
    done = 0;
    while (!done) {
        if (func_80199F30(1) == 0) {
            done = 1;
        }
    }
    return 1;
}

s32 func_8019AAB4(s32 arg0) {
    if (arg0 != 0) {
        D_8019DC94.unk4 = 1;
    } else {
        D_8019DC94.unk4 = 0;
        D_80071E38 = 0;
        func_8019CC3C(0);
    }
    return 1;
}

INCLUDE_ASM("asm/us/ovl_64/nonmatchings/ovl_64", func_8019AAF8);

s32 func_8019AB70(void) {
    if (D_80071E32 != 0) {
        return D_80071E34;
    }
    return -1;
}

void func_8019AB98(s32 arg0, s32 arg1, s32 arg2) {
    if (arg0 < 0) {
        D_8019DC94.unk38 = 3;
    } else {
        D_8019DC94.unk38 = arg0;
    }
    if (arg1 < 0) {
        D_8019DC94.unk3C = 4;
    } else {
        D_8019DC94.unk3C = arg1;
    }
    if (arg2 < 0) {
        D_8019DC94.unk40 = 0x7F;
    } else {
        D_8019DC94.unk40 = arg2;
    }
}

s32 func_8019ABF0(void) {
    s32 v;

    func_8019C99C(&v);
    return v;
}

void func_8019AC14(u32 arg0) {
    if (arg0 < 2) {
        func_8019C9B4(arg0);
    }
}

void func_8019AC3C(void (*arg0)(s8 *)) {
    D_8019DC94.unk60 = arg0;
}

void func_8019AC48(s32 arg0) {
    if (arg0 != 0) {
        D_8019DC94.unk44 = NULL;
    } else {
        D_8019DC94.unk44 = D_8019F288;
    }
}

s32 func_8019AC6C(s32 *arg0, u32 arg1) {
    if (arg1 >= 6) {
        return 0;
    }
    *arg0 = D_8019DC5C[arg1].unk0;
    return 1;
}

s32 func_8019AC9C(s32 arg0, u32 arg1, s32 arg2) {
    if (arg1 >= 6 || arg2 < 0 || arg2 >= D_8019DC5C[arg1].unk0) {
        return 0;
    }
    func_80065238(arg0, D_8019DC5C[arg1].unk4[arg2].unk0);
    return 1;
}

s32 func_8019AD0C(void) {
    s32 v;

    if (D_80071E32 < 2) {
        return -1;
    }
    func_8019CBF4(&v);
    return v;
}

s32 func_8019AD4C(Ovl64Header *arg0, u16 *arg1) {
    if (D_8019DC94.unk6 != 0) {
        return 0;
    }
    arg0->unk0 = D_8019F258.unk0;
    arg0->unk2 = D_8019F258.unk2;
    arg0->unk4 = D_8019F258.unk4;
    arg0->unk6 = D_8019F258.unk6;
    arg0->unk8 = D_8019F258.unk8;
    arg0->unkA = D_8019F258.unkA;
    arg0->unkC = D_8019F258.unkC;
    arg0->unkE = D_8019F258.unkE;
    arg0->unk10 = D_8019F258.unk10;
    arg0->unk14 = D_8019F258.unk14;
    arg0->unk18 = D_8019F258.unk18;
    arg0->unk1C = D_8019F258.unk1C;
    *arg1 = D_8019F278;
    return 1;
}

s32 func_8019AE0C(s32 *arg0, s32 *arg1) {
    if (D_8019DC94.unk6 != 0) {
        return 0;
    }
    *arg0 = D_8019DC88.unk0;
    *arg1 = D_8019DC88.unk4;
    return 1;
}

INCLUDE_ASM("asm/us/ovl_64/nonmatchings/ovl_64", func_8019AE44);

INCLUDE_ASM("asm/us/ovl_64/nonmatchings/ovl_64", func_8019AE98);

INCLUDE_ASM("asm/us/ovl_64/nonmatchings/ovl_64", func_8019AEC0);

INCLUDE_ASM("asm/us/ovl_64/nonmatchings/ovl_64", func_8019B00C);

INCLUDE_ASM("asm/us/ovl_64/nonmatchings/ovl_64", func_8019B188);

INCLUDE_ASM("asm/us/ovl_64/nonmatchings/ovl_64", func_8019B3D4);

INCLUDE_ASM("asm/us/ovl_64/nonmatchings/ovl_64", func_8019B7C4);

INCLUDE_ASM("asm/us/ovl_64/nonmatchings/ovl_64", func_8019BA70);

INCLUDE_ASM("asm/us/ovl_64/nonmatchings/ovl_64", func_8019BCE8);

INCLUDE_ASM("asm/us/ovl_64/nonmatchings/ovl_64", func_8019C1B0);

INCLUDE_ASM("asm/us/ovl_64/nonmatchings/ovl_64", func_8019C2F4);

INCLUDE_ASM("asm/us/ovl_64/nonmatchings/ovl_64", func_8019C434);

INCLUDE_ASM("asm/us/ovl_64/nonmatchings/ovl_64", func_8019C7E4);

INCLUDE_ASM("asm/us/ovl_64/nonmatchings/ovl_64", func_8019C864);

INCLUDE_ASM("asm/us/ovl_64/nonmatchings/ovl_64", func_8019C884);

INCLUDE_ASM("asm/us/ovl_64/nonmatchings/ovl_64", func_8019C954);

INCLUDE_ASM("asm/us/ovl_64/nonmatchings/ovl_64", func_8019C98C);

INCLUDE_ASM("asm/us/ovl_64/nonmatchings/ovl_64", func_8019C99C);

INCLUDE_ASM("asm/us/ovl_64/nonmatchings/ovl_64", func_8019C9B4);

INCLUDE_ASM("asm/us/ovl_64/nonmatchings/ovl_64", func_8019C9D4);

INCLUDE_ASM("asm/us/ovl_64/nonmatchings/ovl_64", func_8019CA1C);

INCLUDE_ASM("asm/us/ovl_64/nonmatchings/ovl_64", func_8019CA80);

INCLUDE_ASM("asm/us/ovl_64/nonmatchings/ovl_64", func_8019CAC0);

INCLUDE_ASM("asm/us/ovl_64/nonmatchings/ovl_64", func_8019CAEC);

INCLUDE_ASM("asm/us/ovl_64/nonmatchings/ovl_64", func_8019CBA0);

INCLUDE_ASM("asm/us/ovl_64/nonmatchings/ovl_64", func_8019CBF4);

INCLUDE_ASM("asm/us/ovl_64/nonmatchings/ovl_64", func_8019CC0C);

INCLUDE_ASM("asm/us/ovl_64/nonmatchings/ovl_64", func_8019CC3C);

INCLUDE_ASM("asm/us/ovl_64/nonmatchings/ovl_64", func_8019CC7C);

INCLUDE_ASM("asm/us/ovl_64/nonmatchings/ovl_64", func_8019CCBC);

INCLUDE_ASM("asm/us/ovl_64/nonmatchings/ovl_64", DecDCTReset);

INCLUDE_ASM("asm/us/ovl_64/nonmatchings/ovl_64", DecDCTGetEnv);

INCLUDE_ASM("asm/us/ovl_64/nonmatchings/ovl_64", DecDCTPutEnv);

INCLUDE_ASM("asm/us/ovl_64/nonmatchings/ovl_64", DecDCTin);

INCLUDE_ASM("asm/us/ovl_64/nonmatchings/ovl_64", DecDCTout);

INCLUDE_ASM("asm/us/ovl_64/nonmatchings/ovl_64", DecDCTinSync);

INCLUDE_ASM("asm/us/ovl_64/nonmatchings/ovl_64", DecDCToutSync);

INCLUDE_ASM("asm/us/ovl_64/nonmatchings/ovl_64", DecDCTinCallback);

INCLUDE_ASM("asm/us/ovl_64/nonmatchings/ovl_64", DecDCToutCallback);

INCLUDE_RODATA("asm/us/ovl_64/nonmatchings/ovl_64", D_8019903C);

INCLUDE_ASM("asm/us/ovl_64/nonmatchings/ovl_64", func_8019D020);

INCLUDE_ASM("asm/us/ovl_64/nonmatchings/ovl_64", func_8019D110);

INCLUDE_ASM("asm/us/ovl_64/nonmatchings/ovl_64", func_8019D1A0);

INCLUDE_ASM("asm/us/ovl_64/nonmatchings/ovl_64", func_8019D22C);

INCLUDE_ASM("asm/us/ovl_64/nonmatchings/ovl_64", func_8019D2C0);

INCLUDE_ASM("asm/us/ovl_64/nonmatchings/ovl_64", func_8019D354);

INCLUDE_ASM("asm/us/ovl_64/nonmatchings/ovl_64", func_8019D36C);

INCLUDE_ASM("asm/us/ovl_64/nonmatchings/ovl_64", func_8019D3F0);
