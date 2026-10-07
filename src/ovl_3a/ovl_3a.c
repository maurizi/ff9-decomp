#include "ovl_3a.h"

INCLUDE_ASM("asm/us/ovl_3a/nonmatchings/ovl_3a", func_801F3874);

void func_801F3A98(void) {
    if (!func_8002F810()) {
        if (D_80077528 & 8) {
            func_8002F488(0x67);
            D_801F4BBC.unk0 = 0;
            D_801F4BBC.unk1 = 3;
            func_80035580(NULL);
            return;
        }
        if (D_80077528 & 0x100000) {
            func_8002F488(0x67);
            D_801F4BBC.unk2 = D_801F4BBC.unk2 == 0 ? 1 : D_801F4BBC.unk2 - 1;
            func_801F4AE0();
            return;
        }
        if (D_80077528 & 0x200000) {
            func_8002F488(0x67);
            D_801F4BBC.unk2 = D_801F4BBC.unk2 != 0 ? 0 : D_801F4BBC.unk2 + 1;
            func_801F4AE0();
            return;
        }
        if (D_80077528 & 0x20000) {
            func_801F3D9C();
            return;
        }
        if (D_801F4BBC.unk0 == 0) {
            func_801F47E8();
            return;
        }
        func_801F3BBC();
    }
}

void func_801F3BBC(void) {
    if ((D_80077528 & 0x10) && D_801F4BBC.unk3 >= 15) {
        func_8002F488(0x67);
        D_801F4BBC.unk3 -= 15;
        return;
    }
    if ((D_80077528 & 0x40) && D_801F4BBC.unk3 < D_801F4BBC.unk5[D_801F4BBC.unk2] - 15) {
        func_8002F488(0x67);
        D_801F4BBC.unk3 += 15;
        return;
    }
    if ((D_80077528 & 0x20) && (u8)(D_801F4BBC.unk3 % 15) != 14) {
        func_8002F488(0x67);
        D_801F4BBC.unk3++;
        return;
    }
    if (D_80077528 & 0x80) {
        func_8002F488(0x67);
        if ((u8)(D_801F4BBC.unk3 % 15) != 0) {
            D_801F4BBC.unk3--;
            return;
        }
        D_801F4BBC.unk0 = 0;
        func_80035580(NULL);
        return;
    }
    if (D_80077528 & 0x10000) {
        func_8002F488(0x65);
        if (D_801F4BBC.unk4 != 0) {
            D_801F4BBC.unk4--;
            D_801F4BBC.unk7[D_801F4BBC.unk4] = 0xFF;
        }
    }
}

INCLUDE_RODATA("asm/us/ovl_3a/nonmatchings/ovl_3a", D_801EDF00);

INCLUDE_ASM("asm/us/ovl_3a/nonmatchings/ovl_3a", func_801F3D9C);

INCLUDE_ASM("asm/us/ovl_3a/nonmatchings/ovl_3a", func_801F3FAC);

INCLUDE_ASM("asm/us/ovl_3a/nonmatchings/ovl_3a", func_801F4110);

INCLUDE_ASM("asm/us/ovl_3a/nonmatchings/ovl_3a", func_801F4238);

INCLUDE_ASM("asm/us/ovl_3a/nonmatchings/ovl_3a", func_801F459C);

void func_801F4748(void) {
    func_801F3A98();
    func_801F47A0();
}

void func_801F4770(void) {
    func_8003556C(NULL);
    func_80035580(NULL);
    func_801F4A34();
}

void func_801F47A0(void) {
    func_801F48A0();
    func_801F3FAC();
    func_801F4110();
    func_801F490C();
    func_801F4238();
    func_801F459C();
}

void func_801F47E8(void) {
    if (D_80077528 & 0x10) {
        func_8002F488(0x67);
        D_801F4BBC.unk1 = D_801F4BBC.unk1 == 0 ? 4 : D_801F4BBC.unk1 - 1;
        return;
    }
    if (D_80077528 & 0x40) {
        func_8002F488(0x67);
        D_801F4BBC.unk1 = D_801F4BBC.unk1 >= 4 ? 0 : D_801F4BBC.unk1 + 1;
        return;
    }
    if (D_80077528 & 0x20) {
        func_8002F488(0x67);
        D_801F4BBC.unk0 = 1;
        func_80035580(&func_801F4B48);
    }
}

void func_801F48A0(void) {
    func_8002DEE4(D_801F4B54, 6, 2);
    func_8002705C(D_801F4B54, 6, 2);
    func_8002B5FC(D_801F4B54->x + 0xA, D_801F4B54->y + 6, 5, func_80035774(0), 0);
}

void func_801F490C(void) {
    func_8002DEE4(D_801F4B60, 7, 1);
    func_8002705C(D_801F4B60, 7, 1);
    func_8002B5FC(D_801F4B60->x + 0x3E, D_801F4B60->y + 6, 6, func_80035774(1), 0);
    func_8002B524(D_801F4B60->x + 0x23, D_801F4B60->y - 4, 6, D_80070450, 0);
}

void func_801F49A0(s32 chr) {
    u8 *img;

    img = D_801EE0B4[func_80028954(chr)];
    StoreImage(&D_801F4B68, (u32 *)D_801F4BBC.unk28);
    DrawSync(0);
    LoadImage(&D_801F4B68, (u32 *)img);
    DrawSync(0);
    LoadImage(&D_801F4B70, (u32 *)(img + 0x100));
    DrawSync(0);
}

void func_801F4A34(void) {
    LoadImage(&D_801F4B78, (u32 *)D_801F4BBC.unk28);
    DrawSync(0);
}

s32 func_801F4A68(s32 i) {
    return D_801F4B80[i];
}

s32 func_801F4A84(s32 i) {
    return D_801F4BB0[i];
}

void func_801F4A9C(void) {
    D_80077DE8.unk8 = func_80035774(D_801F4BBC.unk0 == 0 ? D_801F4BBC.unk1 + 7 : 0xC);
}

void func_801F4AE0(void) {
    if (D_801F4BBC.unk2 == 1 && D_801F4BBC.unk3 >= D_801F4BBC.unk5[1]) {
        D_801F4BBC.unk3 = D_801F4BBC.unk5[1] + (u8)(D_801F4BBC.unk3 % 15 - 15);
    }
}

INCLUDE_ASM("asm/us/ovl_3a/nonmatchings/ovl_3a", func_801F4B48);

INCLUDE_RODATA("asm/us/ovl_3a/nonmatchings/ovl_3a", D_801EDF18);

INCLUDE_RODATA("asm/us/ovl_3a/nonmatchings/ovl_3a", D_801EE0B4);
