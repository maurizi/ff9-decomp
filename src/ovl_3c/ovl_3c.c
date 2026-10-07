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

INCLUDE_ASM("asm/us/ovl_3c/nonmatchings/ovl_3c", func_801F4198);

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

INCLUDE_ASM("asm/us/ovl_3c/nonmatchings/ovl_3c", func_801F4468);

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

INCLUDE_ASM("asm/us/ovl_3c/nonmatchings/ovl_3c", func_801F4B48);

INCLUDE_ASM("asm/us/ovl_3c/nonmatchings/ovl_3c", func_801F4C68);

INCLUDE_ASM("asm/us/ovl_3c/nonmatchings/ovl_3c", func_801F4E1C);

INCLUDE_ASM("asm/us/ovl_3c/nonmatchings/ovl_3c", func_801F506C);

INCLUDE_ASM("asm/us/ovl_3c/nonmatchings/ovl_3c", func_801F51B8);

INCLUDE_ASM("asm/us/ovl_3c/nonmatchings/ovl_3c", func_801F551C);

INCLUDE_ASM("asm/us/ovl_3c/nonmatchings/ovl_3c", func_801F56E4);

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

INCLUDE_ASM("asm/us/ovl_3c/nonmatchings/ovl_3c", func_801F59A8);

INCLUDE_ASM("asm/us/ovl_3c/nonmatchings/ovl_3c", func_801F5A1C);

INCLUDE_ASM("asm/us/ovl_3c/nonmatchings/ovl_3c", func_801F5A74);

INCLUDE_ASM("asm/us/ovl_3c/nonmatchings/ovl_3c", func_801F5AAC);

INCLUDE_ASM("asm/us/ovl_3c/nonmatchings/ovl_3c", func_801F5AE4);

INCLUDE_ASM("asm/us/ovl_3c/nonmatchings/ovl_3c", func_801F5B64);

INCLUDE_ASM("asm/us/ovl_3c/nonmatchings/ovl_3c", func_801F5BCC);

INCLUDE_ASM("asm/us/ovl_3c/nonmatchings/ovl_3c", func_801F5D14);

INCLUDE_ASM("asm/us/ovl_3c/nonmatchings/ovl_3c", func_801F5D40);

INCLUDE_ASM("asm/us/ovl_3c/nonmatchings/ovl_3c", func_801F5D6C);

INCLUDE_ASM("asm/us/ovl_3c/nonmatchings/ovl_3c", func_801F5D98);

INCLUDE_ASM("asm/us/ovl_3c/nonmatchings/ovl_3c", func_801F5E4C);

INCLUDE_ASM("asm/us/ovl_3c/nonmatchings/ovl_3c", func_801F5EEC);

INCLUDE_ASM("asm/us/ovl_3c/nonmatchings/ovl_3c", func_801F5F88);

INCLUDE_ASM("asm/us/ovl_3c/nonmatchings/ovl_3c", func_801F60A8);

INCLUDE_ASM("asm/us/ovl_3c/nonmatchings/ovl_3c", func_801F625C);

INCLUDE_ASM("asm/us/ovl_3c/nonmatchings/ovl_3c", func_801F65C8);

INCLUDE_ASM("asm/us/ovl_3c/nonmatchings/ovl_3c", func_801F6714);

INCLUDE_ASM("asm/us/ovl_3c/nonmatchings/ovl_3c", func_801F6A7C);

INCLUDE_ASM("asm/us/ovl_3c/nonmatchings/ovl_3c", func_801F6C50);

INCLUDE_ASM("asm/us/ovl_3c/nonmatchings/ovl_3c", func_801F6D34);

INCLUDE_ASM("asm/us/ovl_3c/nonmatchings/ovl_3c", func_801F6F00);

INCLUDE_ASM("asm/us/ovl_3c/nonmatchings/ovl_3c", func_801F7250);

INCLUDE_ASM("asm/us/ovl_3c/nonmatchings/ovl_3c", func_801F7390);

INCLUDE_ASM("asm/us/ovl_3c/nonmatchings/ovl_3c", func_801F73C0);

INCLUDE_ASM("asm/us/ovl_3c/nonmatchings/ovl_3c", func_801F73E0);

INCLUDE_ASM("asm/us/ovl_3c/nonmatchings/ovl_3c", func_801F7438);

INCLUDE_ASM("asm/us/ovl_3c/nonmatchings/ovl_3c", func_801F7470);

INCLUDE_ASM("asm/us/ovl_3c/nonmatchings/ovl_3c", func_801F74A8);

INCLUDE_ASM("asm/us/ovl_3c/nonmatchings/ovl_3c", func_801F7528);

INCLUDE_ASM("asm/us/ovl_3c/nonmatchings/ovl_3c", func_801F7590);

INCLUDE_ASM("asm/us/ovl_3c/nonmatchings/ovl_3c", func_801F75BC);

INCLUDE_ASM("asm/us/ovl_3c/nonmatchings/ovl_3c", func_801F75E8);

INCLUDE_ASM("asm/us/ovl_3c/nonmatchings/ovl_3c", func_801F7614);

INCLUDE_ASM("asm/us/ovl_3c/nonmatchings/ovl_3c", func_801F76C8);

INCLUDE_ASM("asm/us/ovl_3c/nonmatchings/ovl_3c", func_801F7770);

INCLUDE_ASM("asm/us/ovl_3c/nonmatchings/ovl_3c", func_801F7848);

INCLUDE_ASM("asm/us/ovl_3c/nonmatchings/ovl_3c", func_801F79FC);

INCLUDE_ASM("asm/us/ovl_3c/nonmatchings/ovl_3c", func_801F7BC4);

INCLUDE_ASM("asm/us/ovl_3c/nonmatchings/ovl_3c", func_801F7DA8);

INCLUDE_ASM("asm/us/ovl_3c/nonmatchings/ovl_3c", func_801F7FE0);

INCLUDE_ASM("asm/us/ovl_3c/nonmatchings/ovl_3c", func_801F80D0);

INCLUDE_ASM("asm/us/ovl_3c/nonmatchings/ovl_3c", func_801F8224);

INCLUDE_ASM("asm/us/ovl_3c/nonmatchings/ovl_3c", func_801F832C);

INCLUDE_ASM("asm/us/ovl_3c/nonmatchings/ovl_3c", func_801F835C);

INCLUDE_ASM("asm/us/ovl_3c/nonmatchings/ovl_3c", func_801F837C);

INCLUDE_ASM("asm/us/ovl_3c/nonmatchings/ovl_3c", func_801F83D4);

INCLUDE_ASM("asm/us/ovl_3c/nonmatchings/ovl_3c", func_801F83FC);

INCLUDE_ASM("asm/us/ovl_3c/nonmatchings/ovl_3c", func_801F8434);

INCLUDE_ASM("asm/us/ovl_3c/nonmatchings/ovl_3c", func_801F84D8);

INCLUDE_ASM("asm/us/ovl_3c/nonmatchings/ovl_3c", func_801F856C);

INCLUDE_ASM("asm/us/ovl_3c/nonmatchings/ovl_3c", func_801F8680);

INCLUDE_ASM("asm/us/ovl_3c/nonmatchings/ovl_3c", func_801F86D0);

INCLUDE_ASM("asm/us/ovl_3c/nonmatchings/ovl_3c", func_801F86FC);

INCLUDE_ASM("asm/us/ovl_3c/nonmatchings/ovl_3c", func_801F8728);

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
