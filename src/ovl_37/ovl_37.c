#include "ovl_37.h"

INCLUDE_RODATA("asm/us/ovl_37/nonmatchings/ovl_37", D_801EDF00);

INCLUDE_ASM("asm/us/ovl_37/nonmatchings/ovl_37", func_801F0C18);

INCLUDE_ASM("asm/us/ovl_37/nonmatchings/ovl_37", func_801F0D18);

INCLUDE_ASM("asm/us/ovl_37/nonmatchings/ovl_37", func_801F0E10);

INCLUDE_ASM("asm/us/ovl_37/nonmatchings/ovl_37", func_801F0F50);

INCLUDE_ASM("asm/us/ovl_37/nonmatchings/ovl_37", func_801F10C4);

INCLUDE_ASM("asm/us/ovl_37/nonmatchings/ovl_37", func_801F11E4);

INCLUDE_ASM("asm/us/ovl_37/nonmatchings/ovl_37", func_801F13F8);

INCLUDE_ASM("asm/us/ovl_37/nonmatchings/ovl_37", func_801F1778);

INCLUDE_ASM("asm/us/ovl_37/nonmatchings/ovl_37", func_801F187C);

INCLUDE_ASM("asm/us/ovl_37/nonmatchings/ovl_37", func_801F1A7C);

INCLUDE_ASM("asm/us/ovl_37/nonmatchings/ovl_37", func_801F1B78);

INCLUDE_ASM("asm/us/ovl_37/nonmatchings/ovl_37", func_801F1D88);

INCLUDE_ASM("asm/us/ovl_37/nonmatchings/ovl_37", func_801F1E88);

void func_801F2160(void) {
    func_801F21E8();
    func_801F2240();
    func_801F2280();
}

void func_801F2190(void) {
    if (D_8006794C->unk8 == 3) {
        func_8002F55C(0x44A);
    }
    func_8003563C(0);
    func_80035674(1);
    func_8003556C(NULL);
    func_801F2F30();
}

void func_801F21E8(void) {
    void (*fn)(void);

    if (!func_8002F810()) {
        fn = D_801F307C[D_801FAB78.unk1];
        if (fn != NULL) {
            fn();
        }
    }
}

void func_801F2240(void) {
    func_801F262C();
    func_801F11E4();
    func_801F27A0();
    func_801F1E88();
    func_801F13F8();
}

void func_801F2280(void) {
    func_801F1B78();
    func_801F1D88();
    func_801F2BC0();
    func_800631AC(&D_801FAB78.unk3C);
    func_8002EB74(&D_801FAB78.unkC);
    func_8002EB74(&D_801FAB78.unk1C);
}

INCLUDE_ASM("asm/us/ovl_37/nonmatchings/ovl_37", func_801F22C8);

INCLUDE_ASM("asm/us/ovl_37/nonmatchings/ovl_37", func_801F23C4);

void func_801F2488(void) {
    if (D_80077528 & 0x10000) {
        func_8002F488(0x65);
        func_801F2FA0();
    } else if (D_80077528 & 0x20000) {
        func_8002F488(0x67);
        func_801F2FA0();
    }
}

INCLUDE_ASM("asm/us/ovl_37/nonmatchings/ovl_37", func_801F24DC);

void func_801F253C(s32 i) {
    if (i < D_801FAB78.unkB0) {
        func_801F2F38(D_801FAB78.unk60[i]);
        func_8002F488(0x67);
    } else {
        func_8002F488(0x66);
    }
}

INCLUDE_ASM("asm/us/ovl_37/nonmatchings/ovl_37", func_801F2590);

INCLUDE_ASM("asm/us/ovl_37/nonmatchings/ovl_37", func_801F262C);

INCLUDE_ASM("asm/us/ovl_37/nonmatchings/ovl_37", func_801F26E0);

INCLUDE_ASM("asm/us/ovl_37/nonmatchings/ovl_37", func_801F27A0);

INCLUDE_ASM("asm/us/ovl_37/nonmatchings/ovl_37", func_801F2870);

INCLUDE_ASM("asm/us/ovl_37/nonmatchings/ovl_37", func_801F29D0);

void func_801F2B20(void) {
    if (D_8006794C->unk8 == 3) {
        func_8002F514(0x44A);
    }
}

INCLUDE_ASM("asm/us/ovl_37/nonmatchings/ovl_37", func_801F2B58);

INCLUDE_ASM("asm/us/ovl_37/nonmatchings/ovl_37", func_801F2BC0);

INCLUDE_ASM("asm/us/ovl_37/nonmatchings/ovl_37", func_801F2C58);

void func_801F2CC4(void) {
    D_801FAB78.unk1 = 3;
}

void func_801F2CD4(void) {
    D_801FAB78.unk1 = 2;
    D_801FAB78.unk8 = 0;
}

INCLUDE_ASM("asm/us/ovl_37/nonmatchings/ovl_37", func_801F2CEC);

INCLUDE_ASM("asm/us/ovl_37/nonmatchings/ovl_37", func_801F2D58);

void func_801F2E18(void) {
    func_8002DD00(D_801FAB78.unk4C);
}

INCLUDE_ASM("asm/us/ovl_37/nonmatchings/ovl_37", func_801F2E40);

void func_801F2EBC(void) {
    D_801FAB78.unk1 = 4;
}

void func_801F2ECC(void) {
    D_801FAB78.unk1 = 1;
}

void func_801F2EDC(void) {
    u8 *p = D_801EDFD8;

    LoadImage(&D_801F3158, (u32 *)p);
    DrawSync(0);
    LoadImage(&D_801F3160, (u32 *)(p + 0x40));
    DrawSync(0);
}

void func_801F2F30(void) {
}

void func_801F2F38(s32 id) {
    D_801FAB78.unk56 = id;
    D_801FAB78.unk54 = 1;
    D_801FAB78.unk55 = 1;
    D_801FAB78.unk1 = 0;
    D_801FAB78.unk57 = 0;
    D_801FAB78.unk58 = 0x48;
    D_801FAB78.unk58 -= func_800632B4(0x16, D_801F3064);
    func_8003563C(1);
}

void func_801F2FA0(void) {
    D_801FAB78.unk55 = 3;
    D_801FAB78.unk1 = 0;
    D_801FAB78.unk57 = 0;
    D_801FAB78.unk58 = 0x48;
}

INCLUDE_ASM("asm/us/ovl_37/nonmatchings/ovl_37", func_801F2FC4);

void func_801F3038(void) {
}

INCLUDE_RODATA("asm/us/ovl_37/nonmatchings/ovl_37", D_801EDFD8);
