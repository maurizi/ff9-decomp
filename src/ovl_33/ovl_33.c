#include "ovl_33.h"

INCLUDE_ASM("asm/us/ovl_33/nonmatchings/ovl_33", func_801EE7C0);

INCLUDE_ASM("asm/us/ovl_33/nonmatchings/ovl_33", func_801EE978);

INCLUDE_ASM("asm/us/ovl_33/nonmatchings/ovl_33", func_801EEA50);

INCLUDE_ASM("asm/us/ovl_33/nonmatchings/ovl_33", func_801EEBE0);

INCLUDE_ASM("asm/us/ovl_33/nonmatchings/ovl_33", func_801EED10);

INCLUDE_ASM("asm/us/ovl_33/nonmatchings/ovl_33", func_801EEEA8);

INCLUDE_ASM("asm/us/ovl_33/nonmatchings/ovl_33", func_801EEFDC);

INCLUDE_ASM("asm/us/ovl_33/nonmatchings/ovl_33", func_801EF270);

INCLUDE_ASM("asm/us/ovl_33/nonmatchings/ovl_33", func_801EF3A8);

INCLUDE_ASM("asm/us/ovl_33/nonmatchings/ovl_33", func_801EF560);

INCLUDE_ASM("asm/us/ovl_33/nonmatchings/ovl_33", func_801EF644);

INCLUDE_ASM("asm/us/ovl_33/nonmatchings/ovl_33", func_801EF900);

INCLUDE_ASM("asm/us/ovl_33/nonmatchings/ovl_33", func_801EFB8C);

INCLUDE_ASM("asm/us/ovl_33/nonmatchings/ovl_33", func_801EFCB4);

INCLUDE_ASM("asm/us/ovl_33/nonmatchings/ovl_33", func_801EFE2C);

INCLUDE_ASM("asm/us/ovl_33/nonmatchings/ovl_33", func_801F0024);

INCLUDE_ASM("asm/us/ovl_33/nonmatchings/ovl_33", func_801F0410);

void func_801F05FC(void) {
    func_801F0664();
    func_801F06BC();
    func_801EE978();
}

void func_801F062C(void) {
    D_80077E53 = D_801FBF84.unk2;
    func_80035674(1);
    func_8003556C(NULL);
}

void func_801F0664(void) {
    void (*fn)(void);

    if (!func_8002F810()) {
        fn = D_801F33A8[D_801FBF84.unk1];
        if (fn != NULL) {
            fn();
        }
    }
}

void func_801F06BC(void) {
    func_801F07DC();
    func_801EEEA8();
    func_801EEFDC();
    func_801EF270();
    func_801EF3A8();
    func_801EF560();
    func_801EF644();
}

INCLUDE_ASM("asm/us/ovl_33/nonmatchings/ovl_33", func_801F070C);

INCLUDE_ASM("asm/us/ovl_33/nonmatchings/ovl_33", func_801F07DC);

INCLUDE_ASM("asm/us/ovl_33/nonmatchings/ovl_33", func_801F0890);

INCLUDE_ASM("asm/us/ovl_33/nonmatchings/ovl_33", func_801F0934);

INCLUDE_ASM("asm/us/ovl_33/nonmatchings/ovl_33", func_801F09CC);

INCLUDE_ASM("asm/us/ovl_33/nonmatchings/ovl_33", func_801F0AB8);

INCLUDE_ASM("asm/us/ovl_33/nonmatchings/ovl_33", func_801F0C10);

INCLUDE_ASM("asm/us/ovl_33/nonmatchings/ovl_33", func_801F0CC4);

INCLUDE_ASM("asm/us/ovl_33/nonmatchings/ovl_33", func_801F0DDC);

void func_801F0E3C(void) {
    D_801FBF84.unk1 = 3;
}

void func_801F0E4C(void) {
    D_801FBF84.unk1 = 2;
    D_801FBF84.unk768 = 0;
}

INCLUDE_ASM("asm/us/ovl_33/nonmatchings/ovl_33", func_801F0E64);

void func_801F0ED4(void) {
    D_801FBF84.unk1 = 1;
}

void func_801F0EE4(void) {
    u8 *p = D_801EE164;

    LoadImage(&D_801F3484, (u32 *)p);
    DrawSync(0);
    LoadImage(&D_801F348C, (u32 *)(p + 0x120));
    DrawSync(0);
}

void func_801F0F38(s32 x, s32 y, s32 otz, s32 i) {
    func_80027710(x, y, otz, func_801F0F90(i), 0);
}

Icon *func_801F0F90(s32 i) {
    return &D_801F3494[i];
}

INCLUDE_ASM("asm/us/ovl_33/nonmatchings/ovl_33", func_801F0FA4);

INCLUDE_ASM("asm/us/ovl_33/nonmatchings/ovl_33", func_801F1018);

INCLUDE_ASM("asm/us/ovl_33/nonmatchings/ovl_33", func_801F1048);

void func_801F1148(void) {
}

INCLUDE_ASM("asm/us/ovl_33/nonmatchings/ovl_33", func_801F1150);

INCLUDE_ASM("asm/us/ovl_33/nonmatchings/ovl_33", func_801F12C8);

INCLUDE_ASM("asm/us/ovl_33/nonmatchings/ovl_33", func_801F1454);

INCLUDE_ASM("asm/us/ovl_33/nonmatchings/ovl_33", func_801F15E0);

INCLUDE_ASM("asm/us/ovl_33/nonmatchings/ovl_33", func_801F189C);

INCLUDE_ASM("asm/us/ovl_33/nonmatchings/ovl_33", func_801F1A94);

INCLUDE_ASM("asm/us/ovl_33/nonmatchings/ovl_33", func_801F1F08);

INCLUDE_ASM("asm/us/ovl_33/nonmatchings/ovl_33", func_801F20A0);

INCLUDE_ASM("asm/us/ovl_33/nonmatchings/ovl_33", func_801F239C);

INCLUDE_ASM("asm/us/ovl_33/nonmatchings/ovl_33", func_801F24EC);

INCLUDE_ASM("asm/us/ovl_33/nonmatchings/ovl_33", func_801F26C4);

INCLUDE_ASM("asm/us/ovl_33/nonmatchings/ovl_33", func_801F2894);

INCLUDE_ASM("asm/us/ovl_33/nonmatchings/ovl_33", func_801F2AB4);

void func_801F2C0C(void) {
    func_801F2C78();
    func_801F2CD0();
    func_801F2D18();
}

void func_801F2C3C(void) {
    D_80077E53 = D_801FC840.unk3;
    func_8002DE60();
    func_80035674(1);
    func_8003556C(NULL);
}

void func_801F2C78(void) {
    void (*fn)(void);

    if (!func_8002F810()) {
        fn = D_801F7420[D_801FC840.unk1];
        if (fn != NULL) {
            fn();
        }
    }
}

void func_801F2CD0(void) {
    func_801F2E34();
    func_801F15E0();
    func_801F189C();
    func_801F1A94();
    func_801F1F08();
    func_801F239C();
}

INCLUDE_ASM("asm/us/ovl_33/nonmatchings/ovl_33", func_801F2D18);

INCLUDE_ASM("asm/us/ovl_33/nonmatchings/ovl_33", func_801F2DB4);

INCLUDE_ASM("asm/us/ovl_33/nonmatchings/ovl_33", func_801F2E34);

void func_801F2ED4(void) {
    MenuView v;
    s32 i;

    func_8002F344(&D_801FC840.unk4, &v);
    for (i = 0; i < v.unk0; i++) {
        func_801F2F3C(0, i, v.unk1 + i, &v);
    }
}

INCLUDE_ASM("asm/us/ovl_33/nonmatchings/ovl_33", func_801F2F3C);

void func_801F301C(void) {
    WinDef *w = &D_801FC840.unk23C;

    w->unk4 = D_801FC840.unk34;
    D_801FC840.unk260 = 1;
    D_801FC840.unk1 = 0;
    func_80063118(w, D_801FBD54, 1);
}

void func_801F307C(void) {
    WinDef *w = &D_801FC840.unk23C;

    D_801FC840.unk1 = 0;
    func_80063118(w, D_801FBD74, 0);
}

void func_801F30B0(void) {
    D_801FC840.unk1 = 3;
}

void func_801F30C0(void) {
    D_801FC840.unk1 = 2;
    D_801FC840.unk260 = 0;
}

INCLUDE_ASM("asm/us/ovl_33/nonmatchings/ovl_33", func_801F30D8);

INCLUDE_ASM("asm/us/ovl_33/nonmatchings/ovl_33", func_801F31B0);

void func_801F3264(void) {
    D_801FC840.unk1 = D_801FC840.unk261;
}

void func_801F3278(s32 slot, s32 dir) {
    u8 s;

    func_80028D08(&D_801FC840.unk24C, D_801FC840.unk3, slot, dir);
    if (dir != 0) {
        s = D_801FC840.unk1;
        D_801FC840.unk1 = 0;
        D_801FC840.unk261 = s;
    }
    D_801FC840.unk3 = slot;
}

void func_801F32E0(Character *c) {
    func_801F26C4();
    func_8003384C(c);
    c->unk34 = (c->unk34 & ~0x1F0) | ((func_80033280(c->unk34 & 0xF, (c->unkA >> 4) & 1, c->unk39) & 0x1F) << 4);
}

INCLUDE_ASM("asm/us/ovl_33/nonmatchings/ovl_33", func_801F3344);

INCLUDE_ASM("asm/us/ovl_33/nonmatchings/ovl_33", func_801F33AC);

INCLUDE_RODATA("asm/us/ovl_33/nonmatchings/ovl_33", D_801EDF00);

INCLUDE_RODATA("asm/us/ovl_33/nonmatchings/ovl_33", D_801EDF04);

INCLUDE_RODATA("asm/us/ovl_33/nonmatchings/ovl_33", D_801EE164);

INCLUDE_RODATA("asm/us/ovl_33/nonmatchings/ovl_33", D_801EE644);

INCLUDE_RODATA("asm/us/ovl_33/nonmatchings/ovl_33", D_801EE648);
