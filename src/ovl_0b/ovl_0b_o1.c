#include "ovl_0b.h"

INCLUDE_ASM("asm/us/ovl_0b/nonmatchings/ovl_0b_o1", func_800C2598);

INCLUDE_ASM("asm/us/ovl_0b/nonmatchings/ovl_0b_o1", func_800C28F4);

u8 func_800C2DF4(void) {
    return D_800FE470;
}

void func_800C2E04(void) {
    D_800F78A4->unk2C = &D_800FE3B8;
    func_8001A878(&D_800FE3B8, 0xD0);
    D_800FE3B8.unk60 = 0x200;
    D_800FE3B8.unk5E = 0x200;
}

void func_800C2E4C(void) {
    func_8001A878(&D_800FE488, 0x64);
    func_80063864(D_800F78A4->unkC, &D_800FE368, 0x4C);
    D_800FE3B8.unk5C = 0;
    D_800FE3B8.unk53 = func_800CF690() & 1;
    if (D_800F78A4->unk0->unk1C & 4) {
        func_800C57A4(D_800FE368.unk44, D_800FE488.unk50);
    }
    func_800C57A4(D_800FE368.unk0, D_800FE488.unk46);
    func_800C57A4(D_800FE368.unk4, D_800FE488.unk5A);
}

void func_800C2F1C(s32 arg0) {
    D_800FE3B8.unkB9 = 0;
    D_800FE3B8.unk52 = 0;
    D_800FE3B8.unk5C = 0;
    func_8001A878(&D_800FE488, 0x64);
    func_8001A878(&D_800FE368, 0x4C);
    func_800C316C(arg0);
    if (D_800FE3B8.unkCC != 0) {
        func_800C57A4(D_800FE3B8.unkCC, D_800FE488.unk46);
    }
}

void func_800C2F9C(s32 arg0) {
    D_800FE3B8.unkB9 = 1;
    D_800FE3B8.unk52 = 0;
    D_800FE488.unk8 = 0;
    if (D_800FE3B8.unk5C & 2) {
        D_800FE3B8.unk5C = 0;
    }
    func_800C36D0();
    func_800C316C(arg0);
}

void func_800C2FFC(s32 arg0) {
    func_8001A878(&D_800FE488, 0x64);
    D_800FE3B8.unk53 = func_800CF690() & 1;
    func_800C57A4(D_8006794C->unk1C->unk8E4.battle->unk11C4.unk148, D_800FE488.unk46);
    func_800C57A4((BtlUnit *)D_8006794C->unk1C->unk8E4.battle->unk11C4.unk14C, D_800FE488.unk5A);
    func_800C2F9C(arg0);
    D_800FE3B8.unkB9 = 2;
    D_800FE3B8.unk5C = 0;
}

void func_800C30B8(void) {
    BtlUnit *u = D_800F78A4->unkC->unk0;
    if (u->unk68 & 1) {
        D_800FE484 = u;
    }
}

void func_800C30F4(void) {
    if (D_800FE488.unk45 == 0) {
        D_800FE3B8.unkA8 = D_800FE488.unk18;
        func_800C316C(func_800CA2B0(1));
        D_800FE3B8.unk5E = D_800FE3B8.unk60;
    }
    D_800FE3B8.unk5C = 1;
    D_800FE488.unk45 = 0;
}

INCLUDE_ASM("asm/us/ovl_0b/nonmatchings/ovl_0b_o1", func_800C316C);

INCLUDE_ASM("asm/us/ovl_0b/nonmatchings/ovl_0b_o1", func_800C36D0);

void func_800C3770(void) {
    if (D_800FE3B8.unk5D == 0 && func_800C37BC() != 0) {
        func_800C37BC();
    }
    D_800FE3B8.unk5D = 0;
}

INCLUDE_ASM("asm/us/ovl_0b/nonmatchings/ovl_0b_o1", func_800C37BC);

INCLUDE_ASM("asm/us/ovl_0b/nonmatchings/ovl_0b_o1", func_800C3F78);

void func_800C412C(Unk800C412C *arg0, u8 *arg1) {
    if (arg0->unk1B == 0) {
        arg0->unk1C = arg0->unk1D = arg1[0];
        arg0->unk1A = arg1[2];
        arg0->unk19 = arg1[3];
    }
}

INCLUDE_ASM("asm/us/ovl_0b/nonmatchings/ovl_0b_o1", func_800C416C);

void func_800C493C(s16 *arg0) {
    s32 t;
    if (D_800FE488.unk24 != 0) {
        D_800FE488.unk24--;
        t = func_800CF9DC(D_800FE488.unk2C, D_800FE488.unk28, D_800FE488.unk24, D_800FE488.unk20) * 2;
        if (t != 0) {
            if (!(D_800FE488.unk1C & 1)) {
                arg0[1] += func_800CF708(0, t) * 4 - t;
            }
            if (!(D_800FE488.unk1C & 2)) {
                arg0[0] += func_800CF708(0, t) * 4 - t;
                arg0[2] += func_800CF708(0, t) * 4 - t;
            }
        }
    }
}

void func_800C4A3C(s16 *arg0, s16 *arg1) {
    s32 dx = arg0[0] - arg1[0];
    s32 dz = arg0[2] - arg1[2];
    s32 d = func_800CF6B8(dx * dx + dz * dz, 0xF);
    u32 v = 0;
    if (D_800FE488.unk10 != 0) {
        v = ((func_8004BB80(D_800FE488.unk14 << 6) * d) >> 13) * (D_800FE488.unk10 & 0xF0) >> 11;
        D_800FE488.unk14 += D_800FE488.unk10 & 0xF;
    } else if (D_800FE3B8.unkAC != 0) {
        v = ((func_8004BB80(D_800FE3B8.unkB0 << 6) * d) >> 13) * (D_800FE3B8.unkAC & 0xF0) >> 11;
        D_800FE3B8.unkB0 += D_800FE3B8.unkAC & 0xF;
    }
    arg0[1] += v;
    arg1[1] += v;
}

INCLUDE_ASM("asm/us/ovl_0b/nonmatchings/ovl_0b_o1", func_800C4B9C);

INCLUDE_ASM("asm/us/ovl_0b/nonmatchings/ovl_0b_o1", func_800C4DC4);

INCLUDE_ASM("asm/us/ovl_0b/nonmatchings/ovl_0b_o1", func_800C4E84);

INCLUDE_ASM("asm/us/ovl_0b/nonmatchings/ovl_0b_o1", func_800C52B4);

INCLUDE_ASM("asm/us/ovl_0b/nonmatchings/ovl_0b_o1", func_800C5530);

void func_800C57A4(BtlUnit *arg0, s16 *arg1) {
    s32 i;
    if (arg0->unk68 & 1) {
        s32 h = arg0->unk9C->unk8->unk8;
        s16 y = arg0->unk8C[1] >> 16;
        arg1[0] = y - (h >> 2);
        arg1[1] = y - (h >> 1);
        arg1[2] = y - (h - (h >> 2));
    } else {
        for (i = 0; i < 3; i++) {
            arg1[i] = arg0->unk9C->unk20[D_8006794C->unk1C->unk8E4.battle->unk488[(arg0->unk68 >> 1) & 0xF].unk0->unk29[i]].unk18;
        }
    }
    arg1[3] = arg0->unk8C[0] >> 16;
    arg1[4] = arg0->unk8C[2] >> 16;
}

s32 func_800C5890(s32 arg0) {
    s32 ret = 0;
    if (arg0 == 5 || arg0 == 9) {
        ret = D_800FE36C;
    }
    if ((u32)(arg0 - 0xD) < 2) {
        ret = D_8006794C->unk1C->unk8E4.battle->unk11C4.unk14C;
    }
    return ret;
}

INCLUDE_ASM("asm/us/ovl_0b/nonmatchings/ovl_0b_o1", func_800C58E4);

INCLUDE_ASM("asm/us/ovl_0b/nonmatchings/ovl_0b_o1", func_800C6528);

INCLUDE_ASM("asm/us/ovl_0b/nonmatchings/ovl_0b_o1", func_800C67BC);

INCLUDE_ASM("asm/us/ovl_0b/nonmatchings/ovl_0b_o1", func_800C7730);

INCLUDE_ASM("asm/us/ovl_0b/nonmatchings/ovl_0b_o1", func_800C7B84);

INCLUDE_ASM("asm/us/ovl_0b/nonmatchings/ovl_0b_o1", func_800C7CC8);

INCLUDE_ASM("asm/us/ovl_0b/nonmatchings/ovl_0b_o1", func_800C7DAC);

INCLUDE_ASM("asm/us/ovl_0b/nonmatchings/ovl_0b_o1", func_800C86C8);

INCLUDE_ASM("asm/us/ovl_0b/nonmatchings/ovl_0b_o1", func_800C8A3C);
