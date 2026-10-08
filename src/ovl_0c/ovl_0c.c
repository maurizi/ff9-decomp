#include "ovl_0c.h"

void func_800A8860(void) {
    func_800A8B14();
    do {
        func_800A88C4(D_8006794C->unk1C->unk7DC);
    } while (!(D_8006794C->unk0 & 0x1001));
    func_800A8CA4();
}

void func_800A88C4(s16 arg0) {
    func_800A924C();
    SetDispMask(1);
    do {
        func_800AC580();
        func_80019194();
        if ((D_80073990 & 0x04000000) && (D_800739A0[0] & 0xF8) && !(D_800739A0[0] & 0xFA0001)) {
            func_800ACEEC();
        }
        if (!(D_8006794C->unk1C->unk0 & 0x100)) {
            if (!(D_8006794C->unk1C->unk8E4.world->unk0 & 0x100)) {
                switch (func_800B6BC4()) {
                case 3:
                    D_8006794C->unk1C->unk8E4.world->unk8->unk1B = 2;
                    func_800AC7D4();
                    break;
                case 4:
                    D_8006794C->unk1C->unk8E4.world->unk8->unk1B = 1;
                    D_8006794C->unk0 |= 0x1000;
                    break;
                case 5:
                    D_8006794C->unk0 |= 0x2000;
                    break;
                }
            }
            if (!(D_8006794C->unk1C->unk8E4.world->unk0 & 0x200)) {
                func_80032098();
            }
            if (!(D_8006794C->unk1C->unk8E4.world->unk0 & 0x400)) {
                func_800ACAFC();
            }
            if (!(D_8006794C->unk1C->unk8E4.world->unk0 & 0x800)) {
                func_80023A4C();
            }
        }
        func_800AC5C8();
    } while (!(D_8006794C->unk0 & 0x3001));
    func_800A95A0();
}

void func_800A8B14(void) {
    WorldMap *wm;
    GameData *gd;

    func_800A8CF0();
    wm = D_8006794C->unk1C->unk8E4.world;
    func_800A8D28();
    D_8006794C->unk9 = 3;
    func_8001CD04(0xDA);
    func_8001CFB4();
    SetDispMask(1);
    D_8006794C->unk1C->unk838 = func_800AA15C;
    D_8006794C->unk1C->unk83C = func_800AB0C0;
    D_8006794C->unk1C->unk8BC = func_800AD10C;
    D_8006794C->unk1C->unk84C = 0;
    gd = D_8006794C->unk1C;
    gd->unk39 = 0;
    gd->unk38 = 0;
    wm->unk0 |= 1;
    func_800A8FF0();
    func_800A91A4();
    func_800A8D78();
    func_800A919C();
    func_800A8F18();
    func_800A91AC();
    func_800A8F8C();
    func_800A9220();
    func_800B69B4();
    func_800239D0();
    if (func_800C45D8() == 0) {
        D_8006794C->unk1C->unk0 |= 0x400000;
    }
    if (D_8006794C->unk1C->unk0 & 0x400000) {
        func_800AB0C0(0x8601, 0, 0x78, 0, 0);
    }
    wm->unk4 = (u8 *)D_80067948;
}

void func_800A8CA4(void) {
    s32 i;

    for (i = 1; i >= 0; i--) {
        D_800739B0[i].unk1 = 0;
        D_800739B0[i].unk0 = 0;
    }
    func_800B8068();
    func_800A91B4();
}

void func_800A8CF0(void) {
    D_80067948 = (s32)(D_80067944 + 0x32000);
    D_8006794C->unk1C->unk8E4.world = (WorldMap *)D_80067948;
    D_80067948 += 0x3000;
}

void func_800A8D28(void) {
    u32 i;
    WorldMap *wm = D_8006794C->unk1C->unk8E4.world;

    wm->unk0 = 0;
    wm->unk4 = 0;
    wm->unk10 = -1;
    wm->unkC = 0;
    wm->unk8 = 0;
    for (i = 0; i < 0x34; i++) {
        wm->unk14[i] = 0;
    }
}

INCLUDE_ASM("asm/us/ovl_0c/nonmatchings/ovl_0c", func_800A8D78);

s32 func_800A8F18(void) {
    s32 addr = (s32)func_8001C7FC(4, 0x70064, 0);

    func_800AD114(D_8006794C->unk1C->unk818, addr, (s32)func_8001C7FC(5, 0x70064, 0));
    DrawSync(0);
    return 0;
}

s32 func_800A8F8C(void) {
    s32 addr = (s32)func_8001C7FC(4, 0x42774, 0);

    func_8001D9CC(addr, 0x280, 0xDE, 0x330, 0xB0);
    func_8001D8A8(addr);
    DrawSync(0);
    return 0;
}

INCLUDE_ASM("asm/us/ovl_0c/nonmatchings/ovl_0c", func_800A8FF0);

s32 func_800A919C(void) {
    return 0;
}

s32 func_800A91A4(void) {
    return 0;
}

s32 func_800A91AC(void) {
    return 0;
}

s32 func_800A91B4(void) {
    void *p = D_8006794C->unk1C->unk8E4.world->unkC;

    if (p != NULL) {
        func_8001C784(p);
        D_8006794C->unk1C->unk8E4.world->unkC = 0;
    }
    return 0;
}

s32 func_800A9220(void) {
    return 0;
}

s32 func_800A9228(void) {
    s32 v = 0x2712;
    if (D_8007AEF0.h[0] >= 0x2B52) {
        v = 0x2716;
    }
    return v;
}

INCLUDE_ASM("asm/us/ovl_0c/nonmatchings/ovl_0c", func_800A924C);

INCLUDE_ASM("asm/us/ovl_0c/nonmatchings/ovl_0c", func_800A95A0);

INCLUDE_ASM("asm/us/ovl_0c/nonmatchings/ovl_0c", func_800A98B8);

INCLUDE_ASM("asm/us/ovl_0c/nonmatchings/ovl_0c", func_800A98F8);

INCLUDE_ASM("asm/us/ovl_0c/nonmatchings/ovl_0c", func_800A99C4);

INCLUDE_ASM("asm/us/ovl_0c/nonmatchings/ovl_0c", func_800A9A04);

INCLUDE_ASM("asm/us/ovl_0c/nonmatchings/ovl_0c", func_800A9A44);

INCLUDE_ASM("asm/us/ovl_0c/nonmatchings/ovl_0c", func_800A9B54);

INCLUDE_ASM("asm/us/ovl_0c/nonmatchings/ovl_0c", func_800A9C1C);

INCLUDE_ASM("asm/us/ovl_0c/nonmatchings/ovl_0c", func_800A9DE4);

INCLUDE_ASM("asm/us/ovl_0c/nonmatchings/ovl_0c", func_800A9E24);

INCLUDE_ASM("asm/us/ovl_0c/nonmatchings/ovl_0c", func_800A9EC4);

INCLUDE_ASM("asm/us/ovl_0c/nonmatchings/ovl_0c", func_800A9FC0);

INCLUDE_ASM("asm/us/ovl_0c/nonmatchings/ovl_0c", func_800AA060);

INCLUDE_ASM("asm/us/ovl_0c/nonmatchings/ovl_0c", func_800AA15C);

INCLUDE_ASM("asm/us/ovl_0c/nonmatchings/ovl_0c", func_800AA39C);

INCLUDE_ASM("asm/us/ovl_0c/nonmatchings/ovl_0c", func_800AA418);

void func_800AAD0C(s32 arg0, s32 arg1) {
    func_80062E3C(arg0, 0x280, 0xDE, 0x330, 0xB0, arg1, 1);
}

INCLUDE_ASM("asm/us/ovl_0c/nonmatchings/ovl_0c", func_800AAD48);

INCLUDE_RODATA("asm/us/ovl_0c/nonmatchings/ovl_0c", D_800A7000);

INCLUDE_ASM("asm/us/ovl_0c/nonmatchings/ovl_0c", func_800AB0C0);

INCLUDE_ASM("asm/us/ovl_0c/nonmatchings/ovl_0c", func_800AC020);

INCLUDE_ASM("asm/us/ovl_0c/nonmatchings/ovl_0c", func_800AC21C);

INCLUDE_ASM("asm/us/ovl_0c/nonmatchings/ovl_0c", func_800AC25C);

INCLUDE_ASM("asm/us/ovl_0c/nonmatchings/ovl_0c", func_800AC29C);

INCLUDE_ASM("asm/us/ovl_0c/nonmatchings/ovl_0c", func_800AC344);

INCLUDE_ASM("asm/us/ovl_0c/nonmatchings/ovl_0c", func_800AC3A8);

INCLUDE_ASM("asm/us/ovl_0c/nonmatchings/ovl_0c", func_800AC424);

void func_800AC580(void) {
    GameData *gd = D_8006794C->unk1C;

    ClearOTagR(gd->unk10[gd->unk8] - 8, 0x1010);
}

INCLUDE_ASM("asm/us/ovl_0c/nonmatchings/ovl_0c", func_800AC5C8);

INCLUDE_ASM("asm/us/ovl_0c/nonmatchings/ovl_0c", func_800AC7D4);

INCLUDE_ASM("asm/us/ovl_0c/nonmatchings/ovl_0c", func_800ACAFC);

INCLUDE_ASM("asm/us/ovl_0c/nonmatchings/ovl_0c", func_800ACE24);

INCLUDE_ASM("asm/us/ovl_0c/nonmatchings/ovl_0c", func_800ACEEC);

s32 func_800AD10C(void) {
    return 0;
}

INCLUDE_ASM("asm/us/ovl_0c/nonmatchings/ovl_0c", func_800AD114);

INCLUDE_ASM("asm/us/ovl_0c/nonmatchings/ovl_0c", func_800AD180);

INCLUDE_ASM("asm/us/ovl_0c/nonmatchings/ovl_0c", func_800AD204);

INCLUDE_ASM("asm/us/ovl_0c/nonmatchings/ovl_0c", func_800AD39C);

INCLUDE_ASM("asm/us/ovl_0c/nonmatchings/ovl_0c", func_800AD738);

INCLUDE_ASM("asm/us/ovl_0c/nonmatchings/ovl_0c", func_800AE280);

INCLUDE_ASM("asm/us/ovl_0c/nonmatchings/ovl_0c", func_800AE4C8);

INCLUDE_ASM("asm/us/ovl_0c/nonmatchings/ovl_0c", func_800AE670);

s32 *func_800AE744(void) {
    return D_800CE108;
}

s32 *func_800AE750(void) {
    return D_800CE198;
}

void func_800AE75C(void) {
    func_800AE280();
    func_800AD738();
}

INCLUDE_ASM("asm/us/ovl_0c/nonmatchings/ovl_0c", func_800AE784);

INCLUDE_ASM("asm/us/ovl_0c/nonmatchings/ovl_0c", func_800AE7F0);

INCLUDE_ASM("asm/us/ovl_0c/nonmatchings/ovl_0c", func_800AE828);

void func_800AE860(void) {
    D_800CE0EC = 1;
    D_800CE0ED = 1;
    D_800CE0EE = 1;
}

INCLUDE_ASM("asm/us/ovl_0c/nonmatchings/ovl_0c", func_800AE880);

s32 func_800AEA94(s32 arg0) {
    arg0 = (arg0 - 1000) << 12;
    arg0 /= 4500;
    if (arg0 > 0x1000) {
        arg0 = 0x1000;
    }
    if (arg0 < 0) {
        arg0 = 0;
    }
    return arg0 & 0xFFFF;
}

INCLUDE_ASM("asm/us/ovl_0c/nonmatchings/ovl_0c", func_800AEADC);

INCLUDE_ASM("asm/us/ovl_0c/nonmatchings/ovl_0c", func_800AEC74);

INCLUDE_ASM("asm/us/ovl_0c/nonmatchings/ovl_0c", func_800AEF10);

INCLUDE_ASM("asm/us/ovl_0c/nonmatchings/ovl_0c", func_800AF08C);

INCLUDE_ASM("asm/us/ovl_0c/nonmatchings/ovl_0c", func_800AF414);

INCLUDE_ASM("asm/us/ovl_0c/nonmatchings/ovl_0c", func_800AF66C);

INCLUDE_ASM("asm/us/ovl_0c/nonmatchings/ovl_0c", func_800AF8C0);

INCLUDE_ASM("asm/us/ovl_0c/nonmatchings/ovl_0c", func_800AFCE0);

INCLUDE_ASM("asm/us/ovl_0c/nonmatchings/ovl_0c", func_800B01F0);

INCLUDE_ASM("asm/us/ovl_0c/nonmatchings/ovl_0c", func_800B0378);

INCLUDE_ASM("asm/us/ovl_0c/nonmatchings/ovl_0c", func_800B0D6C);

INCLUDE_ASM("asm/us/ovl_0c/nonmatchings/ovl_0c", func_800B105C);

INCLUDE_ASM("asm/us/ovl_0c/nonmatchings/ovl_0c", func_800B153C);

INCLUDE_ASM("asm/us/ovl_0c/nonmatchings/ovl_0c", func_800B17B4);

INCLUDE_ASM("asm/us/ovl_0c/nonmatchings/ovl_0c", func_800B1CD0);

INCLUDE_ASM("asm/us/ovl_0c/nonmatchings/ovl_0c", func_800B20B8);

INCLUDE_ASM("asm/us/ovl_0c/nonmatchings/ovl_0c", func_800B2C24);

INCLUDE_ASM("asm/us/ovl_0c/nonmatchings/ovl_0c", func_800B2E54);

INCLUDE_ASM("asm/us/ovl_0c/nonmatchings/ovl_0c", func_800B30C0);

INCLUDE_ASM("asm/us/ovl_0c/nonmatchings/ovl_0c", func_800B31B8);

INCLUDE_ASM("asm/us/ovl_0c/nonmatchings/ovl_0c", func_800B326C);

INCLUDE_ASM("asm/us/ovl_0c/nonmatchings/ovl_0c", func_800B3340);

INCLUDE_ASM("asm/us/ovl_0c/nonmatchings/ovl_0c", func_800B3398);

INCLUDE_ASM("asm/us/ovl_0c/nonmatchings/ovl_0c", func_800B345C);

INCLUDE_ASM("asm/us/ovl_0c/nonmatchings/ovl_0c", func_800B3524);

void func_800B35F4(void) {
    D_800CE25C = 2;
    D_800CE3E4 = 10000;
    D_800CF480 = D_8007AEF0.b[0x67] - 1;
    D_800CE26C = D_800CE0E8->unk11;
}

INCLUDE_ASM("asm/us/ovl_0c/nonmatchings/ovl_0c", func_800B3638);

void func_800B36AC(void) {
}

INCLUDE_ASM("asm/us/ovl_0c/nonmatchings/ovl_0c", func_800B36B4);

INCLUDE_ASM("asm/us/ovl_0c/nonmatchings/ovl_0c", func_800B3760);

void func_800B383C(void) {
}

INCLUDE_ASM("asm/us/ovl_0c/nonmatchings/ovl_0c", func_800B3844);

INCLUDE_ASM("asm/us/ovl_0c/nonmatchings/ovl_0c", func_800B3998);

INCLUDE_ASM("asm/us/ovl_0c/nonmatchings/ovl_0c", func_800B3A80);

INCLUDE_ASM("asm/us/ovl_0c/nonmatchings/ovl_0c", func_800B3FEC);

INCLUDE_ASM("asm/us/ovl_0c/nonmatchings/ovl_0c", func_800B4270);

INCLUDE_ASM("asm/us/ovl_0c/nonmatchings/ovl_0c", func_800B4720);

INCLUDE_ASM("asm/us/ovl_0c/nonmatchings/ovl_0c", func_800B4BDC);

INCLUDE_ASM("asm/us/ovl_0c/nonmatchings/ovl_0c", func_800B4FD4);

INCLUDE_ASM("asm/us/ovl_0c/nonmatchings/ovl_0c", func_800B53FC);

INCLUDE_ASM("asm/us/ovl_0c/nonmatchings/ovl_0c", func_800B5880);

INCLUDE_ASM("asm/us/ovl_0c/nonmatchings/ovl_0c", func_800B5CD0);

INCLUDE_ASM("asm/us/ovl_0c/nonmatchings/ovl_0c", func_800B5FD0);

INCLUDE_ASM("asm/us/ovl_0c/nonmatchings/ovl_0c", func_800B6338);

INCLUDE_ASM("asm/us/ovl_0c/nonmatchings/ovl_0c", func_800B6598);

INCLUDE_ASM("asm/us/ovl_0c/nonmatchings/ovl_0c", func_800B6908);

void func_800B69AC(void) {
}

INCLUDE_ASM("asm/us/ovl_0c/nonmatchings/ovl_0c", func_800B69B4);

INCLUDE_RODATA("asm/us/ovl_0c/nonmatchings/ovl_0c", D_800A71D8);

INCLUDE_RODATA("asm/us/ovl_0c/nonmatchings/ovl_0c", D_800A71E0);

INCLUDE_ASM("asm/us/ovl_0c/nonmatchings/ovl_0c", func_800B6BC4);

INCLUDE_ASM("asm/us/ovl_0c/nonmatchings/ovl_0c", func_800B7098);

INCLUDE_ASM("asm/us/ovl_0c/nonmatchings/ovl_0c", func_800B72A4);

INCLUDE_RODATA("asm/us/ovl_0c/nonmatchings/ovl_0c", D_800A71F4);

INCLUDE_RODATA("asm/us/ovl_0c/nonmatchings/ovl_0c", D_800A720C);

INCLUDE_ASM("asm/us/ovl_0c/nonmatchings/ovl_0c", func_800B73F8);

INCLUDE_ASM("asm/us/ovl_0c/nonmatchings/ovl_0c", func_800B7A14);

INCLUDE_ASM("asm/us/ovl_0c/nonmatchings/ovl_0c", func_800B7DAC);

INCLUDE_ASM("asm/us/ovl_0c/nonmatchings/ovl_0c", func_800B7F7C);

void func_800B8024(void) {
    if (D_800CCE50 & 0x40) {
        func_80015C38(D_800A71F4);
    }
    func_800C43F0();
    func_800BC098();
}

INCLUDE_ASM("asm/us/ovl_0c/nonmatchings/ovl_0c", func_800B8068);

s32 func_800B8108(void) {
    return 0x801EDF00;
}

s32 func_800B8114(void) {
    return D_800CF408;
}

INCLUDE_RODATA("asm/us/ovl_0c/nonmatchings/ovl_0c", D_800A79D0);

INCLUDE_RODATA("asm/us/ovl_0c/nonmatchings/ovl_0c", D_800A7A0C);

INCLUDE_RODATA("asm/us/ovl_0c/nonmatchings/ovl_0c", D_800A7A40);

INCLUDE_ASM("asm/us/ovl_0c/nonmatchings/ovl_0c", func_800B8124);

void func_800B81A0(void) {
    func_800B30C0();
}

s32 func_800B81C0(s32 arg0, s32 arg1, s32 arg2) {
    return ((arg0 * (0x1000 - arg2)) >> 0xC) + ((arg1 * arg2) >> 0xC);
}

s32 func_800B81F0(s8 arg0, s8 arg1, s32 arg2) {
    return ((((arg0 << 0xA) * (0x1000 - arg2)) >> 0xC) + (((arg1 << 0xA) * arg2) >> 0xC)) >> 0xA;
}

INCLUDE_ASM("asm/us/ovl_0c/nonmatchings/ovl_0c", func_800B822C);

INCLUDE_ASM("asm/us/ovl_0c/nonmatchings/ovl_0c", func_800B828C);

INCLUDE_ASM("asm/us/ovl_0c/nonmatchings/ovl_0c", func_800B8388);

INCLUDE_ASM("asm/us/ovl_0c/nonmatchings/ovl_0c", func_800B83FC);

s32 func_800B8498(void) {
    if (D_800CF46C == 2) {
        return D_800739A0[0];
    }
    return 0;
}

s32 func_800B84C0(void) {
    if (D_800CF46C == 2) {
        return D_80073990;
    }
    return 0;
}

void func_800B84E8(void) {
    func_800BAD98();
    func_800B8724();
    func_800B8B10();
    func_800AEC74();
    func_800AE75C();
    func_800C3E28();
    func_800C464C();
    func_800AEF10();
    D_800CF46C = 1;
}

s32 func_800B8548(void) {
    return (u32)(D_800D3F98 - 2) < 2U;
}

void func_800B8560(void) {
}

s32 func_800B8568(void) {
    return 0x801EDF00;
}

INCLUDE_ASM("asm/us/ovl_0c/nonmatchings/ovl_0c", func_800B8574);

INCLUDE_ASM("asm/us/ovl_0c/nonmatchings/ovl_0c", func_800B8664);

void func_800B8724(void) {
}

INCLUDE_RODATA("asm/us/ovl_0c/nonmatchings/ovl_0c", D_800A7AE8);

INCLUDE_ASM("asm/us/ovl_0c/nonmatchings/ovl_0c", func_800B872C);

INCLUDE_RODATA("asm/us/ovl_0c/nonmatchings/ovl_0c", D_800A7BB8);

INCLUDE_ASM("asm/us/ovl_0c/nonmatchings/ovl_0c", func_800B88C0);

INCLUDE_ASM("asm/us/ovl_0c/nonmatchings/ovl_0c", func_800B89B0);

INCLUDE_ASM("asm/us/ovl_0c/nonmatchings/ovl_0c", func_800B8A80);

INCLUDE_ASM("asm/us/ovl_0c/nonmatchings/ovl_0c", func_800B8ABC);

INCLUDE_ASM("asm/us/ovl_0c/nonmatchings/ovl_0c", func_800B8B10);

INCLUDE_ASM("asm/us/ovl_0c/nonmatchings/ovl_0c", func_800B8DC0);

INCLUDE_ASM("asm/us/ovl_0c/nonmatchings/ovl_0c", func_800B905C);

INCLUDE_ASM("asm/us/ovl_0c/nonmatchings/ovl_0c", func_800B91A0);

INCLUDE_ASM("asm/us/ovl_0c/nonmatchings/ovl_0c", func_800B93B8);

INCLUDE_ASM("asm/us/ovl_0c/nonmatchings/ovl_0c", func_800B94F8);

void func_800B969C(void) {
    func_800B905C();
    func_800B91A0();
    func_800B93B8();
}

INCLUDE_ASM("asm/us/ovl_0c/nonmatchings/ovl_0c", func_800B96CC);

INCLUDE_ASM("asm/us/ovl_0c/nonmatchings/ovl_0c", func_800B9774);

INCLUDE_RODATA("asm/us/ovl_0c/nonmatchings/ovl_0c", D_800A7CCC);

INCLUDE_ASM("asm/us/ovl_0c/nonmatchings/ovl_0c", func_800B9808);

INCLUDE_ASM("asm/us/ovl_0c/nonmatchings/ovl_0c", func_800BA1E4);

void func_800BA29C(void) {
    func_800BA1E4();
    D_800D30C6 = 1;
    D_800D30C5 = 0;
    D_800D30C4 = 0;
}

void func_800BA2D4(void) {
}

INCLUDE_ASM("asm/us/ovl_0c/nonmatchings/ovl_0c", func_800BA2DC);

INCLUDE_ASM("asm/us/ovl_0c/nonmatchings/ovl_0c", func_800BA3F4);

INCLUDE_ASM("asm/us/ovl_0c/nonmatchings/ovl_0c", func_800BA458);

INCLUDE_ASM("asm/us/ovl_0c/nonmatchings/ovl_0c", func_800BA528);

INCLUDE_ASM("asm/us/ovl_0c/nonmatchings/ovl_0c", func_800BA58C);

INCLUDE_ASM("asm/us/ovl_0c/nonmatchings/ovl_0c", func_800BAD98);

INCLUDE_ASM("asm/us/ovl_0c/nonmatchings/ovl_0c", func_800BAEC0);

INCLUDE_ASM("asm/us/ovl_0c/nonmatchings/ovl_0c", func_800BB07C);

INCLUDE_RODATA("asm/us/ovl_0c/nonmatchings/ovl_0c", D_800A7D20);

INCLUDE_RODATA("asm/us/ovl_0c/nonmatchings/ovl_0c", D_800A7D38);

INCLUDE_ASM("asm/us/ovl_0c/nonmatchings/ovl_0c", func_800BB508);

INCLUDE_ASM("asm/us/ovl_0c/nonmatchings/ovl_0c", func_800BBBA8);

INCLUDE_ASM("asm/us/ovl_0c/nonmatchings/ovl_0c", func_800BBE24);

void func_800BC098(void) {
}

void func_800BC0A0(void) {
}

INCLUDE_ASM("asm/us/ovl_0c/nonmatchings/ovl_0c", func_800BC0A8);

INCLUDE_ASM("asm/us/ovl_0c/nonmatchings/ovl_0c", func_800BC130);

INCLUDE_ASM("asm/us/ovl_0c/nonmatchings/ovl_0c", func_800BC1FC);

INCLUDE_ASM("asm/us/ovl_0c/nonmatchings/ovl_0c", func_800BC2C8);

INCLUDE_ASM("asm/us/ovl_0c/nonmatchings/ovl_0c", func_800BC354);

INCLUDE_ASM("asm/us/ovl_0c/nonmatchings/ovl_0c", func_800BC3C4);

INCLUDE_ASM("asm/us/ovl_0c/nonmatchings/ovl_0c", func_800BC450);

INCLUDE_ASM("asm/us/ovl_0c/nonmatchings/ovl_0c", func_800BC490);

u8 func_800BC528(s32 arg0) {
    return D_800CCFB4[arg0];
}

INCLUDE_ASM("asm/us/ovl_0c/nonmatchings/ovl_0c", func_800BC540);

INCLUDE_ASM("asm/us/ovl_0c/nonmatchings/ovl_0c", func_800BC690);

INCLUDE_ASM("asm/us/ovl_0c/nonmatchings/ovl_0c", func_800BC84C);

INCLUDE_ASM("asm/us/ovl_0c/nonmatchings/ovl_0c", func_800BCA6C);

INCLUDE_ASM("asm/us/ovl_0c/nonmatchings/ovl_0c", func_800BCBDC);

INCLUDE_ASM("asm/us/ovl_0c/nonmatchings/ovl_0c", func_800BD150);

INCLUDE_ASM("asm/us/ovl_0c/nonmatchings/ovl_0c", func_800BD3F8);

void func_800BD57C(void) {
    func_800C1C98();
    func_800C2134();
    D_800D3338 = 0;
}

void func_800BD5A8(void) {
    func_800C2048();
    func_800C26E4();
}

INCLUDE_ASM("asm/us/ovl_0c/nonmatchings/ovl_0c", func_800BD5D0);

void func_800BD66C(void) {
}

INCLUDE_ASM("asm/us/ovl_0c/nonmatchings/ovl_0c", func_800BD674);

INCLUDE_ASM("asm/us/ovl_0c/nonmatchings/ovl_0c", func_800BD78C);

INCLUDE_ASM("asm/us/ovl_0c/nonmatchings/ovl_0c", func_800BDAB0);

INCLUDE_ASM("asm/us/ovl_0c/nonmatchings/ovl_0c", func_800BE138);

INCLUDE_ASM("asm/us/ovl_0c/nonmatchings/ovl_0c", func_800BE238);

INCLUDE_ASM("asm/us/ovl_0c/nonmatchings/ovl_0c", func_800BE4A8);

INCLUDE_ASM("asm/us/ovl_0c/nonmatchings/ovl_0c", func_800BE674);

INCLUDE_ASM("asm/us/ovl_0c/nonmatchings/ovl_0c", func_800BE6B4);

INCLUDE_ASM("asm/us/ovl_0c/nonmatchings/ovl_0c", func_800BE748);

INCLUDE_RODATA("asm/us/ovl_0c/nonmatchings/ovl_0c", D_800A7F48);

INCLUDE_RODATA("asm/us/ovl_0c/nonmatchings/ovl_0c", D_800A7F94);

INCLUDE_RODATA("asm/us/ovl_0c/nonmatchings/ovl_0c", D_800A7FB8);

INCLUDE_ASM("asm/us/ovl_0c/nonmatchings/ovl_0c", func_800BE894);

INCLUDE_ASM("asm/us/ovl_0c/nonmatchings/ovl_0c", func_800BE984);

INCLUDE_ASM("asm/us/ovl_0c/nonmatchings/ovl_0c", func_800BEECC);

INCLUDE_ASM("asm/us/ovl_0c/nonmatchings/ovl_0c", func_800BF544);

INCLUDE_ASM("asm/us/ovl_0c/nonmatchings/ovl_0c", func_800BF978);

INCLUDE_ASM("asm/us/ovl_0c/nonmatchings/ovl_0c", func_800C0888);

INCLUDE_ASM("asm/us/ovl_0c/nonmatchings/ovl_0c", func_800C0F34);

INCLUDE_ASM("asm/us/ovl_0c/nonmatchings/ovl_0c", func_800C1428);

INCLUDE_ASM("asm/us/ovl_0c/nonmatchings/ovl_0c", func_800C1604);

INCLUDE_ASM("asm/us/ovl_0c/nonmatchings/ovl_0c", func_800C16C0);

INCLUDE_ASM("asm/us/ovl_0c/nonmatchings/ovl_0c", func_800C1824);

INCLUDE_ASM("asm/us/ovl_0c/nonmatchings/ovl_0c", func_800C1B00);

INCLUDE_ASM("asm/us/ovl_0c/nonmatchings/ovl_0c", func_800C1C98);

INCLUDE_RODATA("asm/us/ovl_0c/nonmatchings/ovl_0c", D_800A7FF0);

INCLUDE_RODATA("asm/us/ovl_0c/nonmatchings/ovl_0c", D_800A7FF4);

INCLUDE_RODATA("asm/us/ovl_0c/nonmatchings/ovl_0c", D_800A7FF8);

INCLUDE_RODATA("asm/us/ovl_0c/nonmatchings/ovl_0c", D_800A7FFC);

INCLUDE_RODATA("asm/us/ovl_0c/nonmatchings/ovl_0c", D_800A8000);

INCLUDE_RODATA("asm/us/ovl_0c/nonmatchings/ovl_0c", D_800A8020);

INCLUDE_ASM("asm/us/ovl_0c/nonmatchings/ovl_0c", func_800C1DA4);

INCLUDE_ASM("asm/us/ovl_0c/nonmatchings/ovl_0c", func_800C1F3C);

void func_800C2040(void) {
}

void func_800C2048(void) {
}

INCLUDE_ASM("asm/us/ovl_0c/nonmatchings/ovl_0c", func_800C2050);

INCLUDE_ASM("asm/us/ovl_0c/nonmatchings/ovl_0c", func_800C20B0);

INCLUDE_ASM("asm/us/ovl_0c/nonmatchings/ovl_0c", func_800C2134);

INCLUDE_RODATA("asm/us/ovl_0c/nonmatchings/ovl_0c", D_800A8098);

INCLUDE_ASM("asm/us/ovl_0c/nonmatchings/ovl_0c", func_800C2230);

INCLUDE_ASM("asm/us/ovl_0c/nonmatchings/ovl_0c", func_800C23CC);

INCLUDE_ASM("asm/us/ovl_0c/nonmatchings/ovl_0c", func_800C24D8);

void func_800C26DC(void) {
}

void func_800C26E4(void) {
}

INCLUDE_ASM("asm/us/ovl_0c/nonmatchings/ovl_0c", func_800C26EC);

INCLUDE_ASM("asm/us/ovl_0c/nonmatchings/ovl_0c", func_800C2830);

INCLUDE_ASM("asm/us/ovl_0c/nonmatchings/ovl_0c", func_800C291C);

INCLUDE_ASM("asm/us/ovl_0c/nonmatchings/ovl_0c", func_800C2A80);

INCLUDE_ASM("asm/us/ovl_0c/nonmatchings/ovl_0c", func_800C3954);

void func_800C3DEC(void) {
    D_800D3F9C = 1;
    D_800D3F98 = -2;
    D_800D3F8C = -1;
    D_800D3F94 = 0;
    D_800D3F90 = 0x80;
}

INCLUDE_RODATA("asm/us/ovl_0c/nonmatchings/ovl_0c", D_800A8118);

INCLUDE_ASM("asm/us/ovl_0c/nonmatchings/ovl_0c", func_800C3E28);

INCLUDE_ASM("asm/us/ovl_0c/nonmatchings/ovl_0c", func_800C3E88);

INCLUDE_RODATA("asm/us/ovl_0c/nonmatchings/ovl_0c", D_800A8178);

INCLUDE_ASM("asm/us/ovl_0c/nonmatchings/ovl_0c", func_800C3EAC);

INCLUDE_ASM("asm/us/ovl_0c/nonmatchings/ovl_0c", func_800C3FE4);

INCLUDE_ASM("asm/us/ovl_0c/nonmatchings/ovl_0c", func_800C42B0);

INCLUDE_ASM("asm/us/ovl_0c/nonmatchings/ovl_0c", func_800C43F0);

s32 func_800C45D8(void) {
    u16 sc = D_8007AEF0.h[0];
    u8 v = D_8007AEF0.b[0xBE];

    if (sc - 0x2580U < 0x122) {
        return 1;
    }
    if (sc - 0x26A2U < 0x14) {
        return 1;
    }
    return v - 8U < 2;
}

void func_800C461C(void) {
    D_800D4818 = 0;
    D_800D4824 = -1;
    D_800D481C = -1;
    D_800D4828 = -1;
    D_800D4820 = 0;
}

void func_800C464C(void) {
}

void func_800C4654(void) {
}

INCLUDE_ASM("asm/us/ovl_0c/nonmatchings/ovl_0c", func_800C465C);

void func_800C46D8(u16 arg0) {
    func_800AB0C0(0x100, func_800C497C(arg0), 0, 0, 0);
}

void func_800C4710(void) {
}

INCLUDE_ASM("asm/us/ovl_0c/nonmatchings/ovl_0c", func_800C4718);

INCLUDE_ASM("asm/us/ovl_0c/nonmatchings/ovl_0c", func_800C47B0);

INCLUDE_ASM("asm/us/ovl_0c/nonmatchings/ovl_0c", func_800C4850);

INCLUDE_ASM("asm/us/ovl_0c/nonmatchings/ovl_0c", func_800C48F4);

INCLUDE_ASM("asm/us/ovl_0c/nonmatchings/ovl_0c", func_800C497C);

INCLUDE_ASM("asm/us/ovl_0c/nonmatchings/ovl_0c", func_800C49EC);

INCLUDE_ASM("asm/us/ovl_0c/nonmatchings/ovl_0c", func_800C4A5C);

INCLUDE_RODATA("asm/us/ovl_0c/nonmatchings/ovl_0c", D_800A849C);

INCLUDE_ASM("asm/us/ovl_0c/nonmatchings/ovl_0c", func_800C4DD8);

INCLUDE_ASM("asm/us/ovl_0c/nonmatchings/ovl_0c", func_800C53DC);

INCLUDE_ASM("asm/us/ovl_0c/nonmatchings/ovl_0c", func_800C5A48);

INCLUDE_RODATA("asm/us/ovl_0c/nonmatchings/ovl_0c", D_800A8588);

INCLUDE_RODATA("asm/us/ovl_0c/nonmatchings/ovl_0c", D_800A8598);

INCLUDE_ASM("asm/us/ovl_0c/nonmatchings/ovl_0c", func_800C69F4);

void func_800C6EF8(void) {
}

INCLUDE_ASM("asm/us/ovl_0c/nonmatchings/ovl_0c", func_800C6F00);

void func_800C7050(WmNode *arg0) {
    arg0->unk4->unk8 = arg0->unk8;
    arg0->unk8->unk4 = arg0->unk4;
    arg0->unk4 = &D_800D4848;
    arg0->unk8 = D_800D4848.unk8;
    D_800D4848.unk8->unk4 = arg0;
    D_800D4848.unk8 = arg0;
}

INCLUDE_ASM("asm/us/ovl_0c/nonmatchings/ovl_0c", func_800C709C);

INCLUDE_ASM("asm/us/ovl_0c/nonmatchings/ovl_0c", func_800C7914);

INCLUDE_ASM("asm/us/ovl_0c/nonmatchings/ovl_0c", func_800C7BA8);

INCLUDE_ASM("asm/us/ovl_0c/nonmatchings/ovl_0c", func_800C7CF4);

INCLUDE_ASM("asm/us/ovl_0c/nonmatchings/ovl_0c", func_800C8950);

INCLUDE_ASM("asm/us/ovl_0c/nonmatchings/ovl_0c", func_800C9484);

INCLUDE_ASM("asm/us/ovl_0c/nonmatchings/ovl_0c", func_800C95C8);

INCLUDE_ASM("asm/us/ovl_0c/nonmatchings/ovl_0c", func_800C96AC);

INCLUDE_ASM("asm/us/ovl_0c/nonmatchings/ovl_0c", func_800C9FCC);

INCLUDE_ASM("asm/us/ovl_0c/nonmatchings/ovl_0c", func_800CA1D8);

s32 func_800CAE00(u16 *arg0) {
    return *arg0 & 0x7FFF;
}

INCLUDE_ASM("asm/us/ovl_0c/nonmatchings/ovl_0c", func_800CAE0C);

INCLUDE_ASM("asm/us/ovl_0c/nonmatchings/ovl_0c", func_800CB09C);

INCLUDE_ASM("asm/us/ovl_0c/nonmatchings/ovl_0c", func_800CB1EC);

INCLUDE_ASM("asm/us/ovl_0c/nonmatchings/ovl_0c", func_800CB434);

INCLUDE_ASM("asm/us/ovl_0c/nonmatchings/ovl_0c", func_800CB594);

INCLUDE_ASM("asm/us/ovl_0c/nonmatchings/ovl_0c", func_800CB664);

INCLUDE_ASM("asm/us/ovl_0c/nonmatchings/ovl_0c", func_800CB73C);

INCLUDE_ASM("asm/us/ovl_0c/nonmatchings/ovl_0c", func_800CB8C8);

INCLUDE_ASM("asm/us/ovl_0c/nonmatchings/ovl_0c", func_800CB9D8);

INCLUDE_ASM("asm/us/ovl_0c/nonmatchings/ovl_0c", func_800CBB4C);

INCLUDE_ASM("asm/us/ovl_0c/nonmatchings/ovl_0c", func_800CBDF8);

INCLUDE_ASM("asm/us/ovl_0c/nonmatchings/ovl_0c", func_800CBF18);

INCLUDE_ASM("asm/us/ovl_0c/nonmatchings/ovl_0c", func_800CC050);

INCLUDE_ASM("asm/us/ovl_0c/nonmatchings/ovl_0c", func_800CC13C);

INCLUDE_ASM("asm/us/ovl_0c/nonmatchings/ovl_0c", func_800CC324);

INCLUDE_ASM("asm/us/ovl_0c/nonmatchings/ovl_0c", func_800CC3E0);

INCLUDE_ASM("asm/us/ovl_0c/nonmatchings/ovl_0c", func_800CC54C);

void func_800CC5CC(void) {
    D_80077E50.unk1 = 0;
}

INCLUDE_ASM("asm/us/ovl_0c/nonmatchings/ovl_0c", func_800CC5D8);

INCLUDE_ASM("asm/us/ovl_0c/nonmatchings/ovl_0c", func_800CC6E4);

void func_800CC7C0(void) {
    func_800CC82C();
    func_800CC890();
    func_800CC8C8();
}

void func_800CC7F0(void) {
    func_80035674(1);
    func_80035580(0);
    func_8003556C(0);
}

void func_800CC820(u8 arg0) {
    D_800D5438 = arg0;
}

INCLUDE_ASM("asm/us/ovl_0c/nonmatchings/ovl_0c", func_800CC82C);

void func_800CC890(void) {
    func_800CBF18();
    func_800CC050();
    func_800CC940();
    func_800CC13C();
}

INCLUDE_ASM("asm/us/ovl_0c/nonmatchings/ovl_0c", func_800CC8C8);

INCLUDE_ASM("asm/us/ovl_0c/nonmatchings/ovl_0c", func_800CC940);

INCLUDE_RODATA("asm/us/ovl_0c/nonmatchings/ovl_0c", D_800A8644);

INCLUDE_RODATA("asm/us/ovl_0c/nonmatchings/ovl_0c", D_800A8678);
