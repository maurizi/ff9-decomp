#include "ovl_65.h"

INCLUDE_ASM("asm/us/ovl_65/nonmatchings/ovl_65", func_801EF28C);

INCLUDE_ASM("asm/us/ovl_65/nonmatchings/ovl_65", func_801EF45C);

INCLUDE_ASM("asm/us/ovl_65/nonmatchings/ovl_65", func_801EF5C8);

s32 func_801EF6D8(void) {
    func_800E9DBC();
    return 0;
}

INCLUDE_ASM("asm/us/ovl_65/nonmatchings/ovl_65", func_801EF6F8);

s32 func_801EF814(void) {
    Unk1354 *p = GDATA->unk8E4->unk1354;
    if (p->unk4 != 0) {
        func_8001C784((void *)p->unk4);
        GDATA->unk8E4->unk1354->unk4 = 0;
    }
    return 0;
}

INCLUDE_ASM("asm/us/ovl_65/nonmatchings/ovl_65", func_801EF890);

void func_801EFB98(void) {
    RECT *r = &D_801F7A78.unkAC[0].unk4;
    s32 x, y;
    func_8002B524(r->x + 6, r->y - 3, 0xB, D_80070460, 0);
    func_8002DEE4(r, 0xB, 1);
    func_8002705C(r, 0xC, 1);
    x = r->x;
    y = r->y;
    D_801F7A78.unk374 = func_8002920C(x + 0x13, y + 6, D_8007716C + 10,
                                      func_80035774(D_801F3BB8[D_801F7A78.unkA]), 0, D_801F7A78.unk374);
}

INCLUDE_ASM("asm/us/ovl_65/nonmatchings/ovl_65", func_801EFC68);

void func_801EFF50(void) {
    RECT *r = &D_801F7A78.unkAC[6].unk4;
    u8 buf[16];
    s32 x, y;

    if (D_801F7A78.unk14C[6] != 0) {
        func_8002DEE4(r, 0xB, 2);
        func_8002705C(r, 0xC, 2);
        func_8002BAE4(D_801F7A78.unk24.unk0, buf, 6, 1);
        x = r->x;
        y = r->y;
        D_801F7A78.unk374 = func_8002920C(x + 0xC, y + 5, D_8007716C + 10,
                                          func_80035774(0), 0, D_801F7A78.unk374);
        D_801F7A78.unk374 = func_80029D60(r->x + 0x5A, r->y + 9, D_8007716C + 10,
                                          (s32)buf, 0, D_801F7A78.unk374);
    }
}

void func_801F0044(void) {
    RECT *r = &D_801F7A78.unkAC[7].unk4;
    u8 buf[16];
    s32 x, y;

    if (D_801F7A78.unk14C[7] != 0) {
        func_8002DEE4(r, 0xB, 2);
        func_8002705C(r, 0xC, 2);
        func_8002BAE4(D_801F7A78.unk24.unk4, buf, 3, 1);
        x = r->x;
        y = r->y;
        D_801F7A78.unk374 = func_8002920C(x + 0xC, y + 5, D_8007716C + 10,
                                          func_80035774(1), 0, D_801F7A78.unk374);
        D_801F7A78.unk374 = func_80029D60(r->x + 0x6F, r->y + 9, D_8007716C + 10,
                                          (s32)buf, 0, D_801F7A78.unk374);
    }
}

void func_801F0138(void) {
    RECT *r = &D_801F7A78.unkAC[8].unk4;
    u8 buf[16];
    s32 x, y;
    u32 gil = D_801F7A78.unk24.unk7C.unk0 - D_801F7A78.unk24.unk7C.unk8;

    if (gil > 999999) {
        gil = 999999;
    }
    if (D_801F7A78.unk14C[8] != 0) {
        func_8002DEE4(r, 0xE, 2);
        func_8002705C(r, 0xF, 2);
        func_8002BAE4(gil, buf, 6, 1);
        func_80063108(buf, D_801F3BC0);
        x = r->x;
        y = r->y;
        D_801F7A78.unk374 = func_8002920C(x + 0xC, y + 5, D_8007716C + 13,
                                          func_80035774(0xD), 0, D_801F7A78.unk374);
        D_801F7A78.unk374 = func_80029D60(r->x + 0x54, r->y + 9, D_8007716C + 13,
                                          (s32)buf, 0, D_801F7A78.unk374);
    }
}

void func_801F0264(void) {
    RECT *r = &D_801F7A78.unkAC[9].unk4;
    u8 buf[16];
    s32 x, y;
    u32 gil = GDATA->unk560;

    if (gil > 9999999) {
        gil = 9999999;
    }
    if (D_801F7A78.unk14C[9] != 0) {
        func_8002DEE4(r, 0xE, 2);
        func_8002705C(r, 0xF, 2);
        func_8002BAE4(gil, buf, 7, 1);
        func_80063108(buf, D_801F3BC4);
        x = r->x;
        y = r->y;
        D_801F7A78.unk374 = func_8002920C(x + 0xC, y + 5, D_8007716C + 13,
                                          func_80035774(0xE), 0, D_801F7A78.unk374);
        D_801F7A78.unk374 = func_80029D60(r->x + 0x4D, r->y + 9, D_8007716C + 13,
                                          (s32)buf, 0, D_801F7A78.unk374);
    }
}

INCLUDE_ASM("asm/us/ovl_65/nonmatchings/ovl_65", func_801F0394);

INCLUDE_ASM("asm/us/ovl_65/nonmatchings/ovl_65", func_801F04BC);

INCLUDE_ASM("asm/us/ovl_65/nonmatchings/ovl_65", func_801F0594);

void func_801F076C(void) {
    s32 se;

    if (D_801F7A78.unk3 == 0) {
        func_801F2C7C();
        func_801F325C();
        if (!func_8002F810() && (D_80077528 & 0x20000)) {
            func_801F1688();
        }
    } else if (++D_801F7A78.unkC >= 24) {
        se = D_801F3BA0[D_801F7A78.unk8];
        if (se != 0) {
            func_8002F488(se);
        }
        D_801F7A78.unk8++;
        func_8002F8E4(D_801F7A78.unk370);
        if (D_80077DB5 != 0) {
            func_800325E0();
        } else {
            func_8002FAAC(0x10);
            func_8002F8A8(func_801F17D4);
        }
    }
}

void func_801F0878(void) {
    Rewards *r = &D_801F7A78.unk24;
    s32 i;

    for (i = 0; i < 4; i++) {
        if (D_801F7A78.unk14[i] != 0) {
            func_801F2560(i, r->unk1C[i].unk0 - r->unk1C[i].unk8);
            func_801F269C(i);
        }
    }
    func_8002F55C(0x69);
    func_8002F488(0x67);
    for (i = 0; i < 7; i++) {
        func_80063118(&D_801F7A78.unkAC[2 + i], &D_801F3CDC[i], 0);
    }
    D_801F7A78.unkA = 1;
    D_801F7A78.unk14C[9] = 1;
    D_801F7A78.unk14C[8] = 1;
    D_801F7A78.unk370 = func_8002F8E4(0xC);
    func_8002FAAC(4);
    func_8002F8A8(func_801F16F4);
}

void func_801F0978(void) {
    s32 i;
    s32 se;

    D_801F7A78.unk14C[7] = 0;
    D_801F7A78.unk14C[6] = 0;
    D_801F7A78.unk14C[5] = 0;
    D_801F7A78.unk14C[4] = 0;
    D_801F7A78.unk14C[3] = 0;
    D_801F7A78.unk14C[2] = 0;
    func_801F1318();
    for (i = 0; i < 4; i++) {
        WinSlot_clear(&D_801F7A78.unk1D8[i]);
        WinSlot_clear(&D_801F7A78.unk158[i]);
    }
    se = D_801F3BA0[D_801F7A78.unk8];
    if (se != 0) {
        func_8002F488(se);
    }
    D_801F7A78.unk8++;
}

INCLUDE_ASM("asm/us/ovl_65/nonmatchings/ovl_65", func_801F0A44);

INCLUDE_ASM("asm/us/ovl_65/nonmatchings/ovl_65", func_801F0C0C);

void func_801F0D98(s32 arg0, s32 arg1, s32 arg2, s32 arg3, s32 arg4) {
    D_801F7A78.unk374 = func_80029D60(arg0, arg1, D_8007716C + arg2, arg3, arg4, D_801F7A78.unk374);
}

void func_801F0DE4(s32 arg0, s32 arg1, s32 arg2, s32 arg3, s32 arg4) {
    D_801F7A78.unk374 = func_8002920C(arg0, arg1, D_8007716C + arg2, arg3, arg4, D_801F7A78.unk374);
}

void func_801F0E30(void) {
    func_800239D0();
    func_8002C064(0);
    func_8003556C(0);
    GSTATE->unk9 = 2;
    D_801F7A78.unkA = 0;
    D_801F7A78.unk8 = 0;
    D_801F7A78.unk36C = 0;
    D_801F7A78.unk10 = 0;
    D_801F7A78.unk0 = 1;
    D_801F7A78.unkC = 0;
    D_801F7A78.unk3 = 0;
    D_801F7A78.unk4 = 0;
    switch (func_801F1004()) {
    case 0:
        func_801F1060();
        break;
    case 1:
        func_801F107C();
        break;
    case 2:
        func_801F114C();
        break;
    }
}

s32 func_801F0F04(void) {
    if (D_801F7A78.unk1 != 0) {
        if (D_801F7A78.unk2 != 0) {
            goto count;
        }
        D_801F7A78.unk374 = D_801F7A78.unk378[D_80077148.unk0];
        if (D_80077DB5 == 0) {
            func_8002C308(0);
        }
        func_801F1160();
        func_801F11B8();
    }
    if (D_801F7A78.unk2 != 0) {
    count:
        if (++D_801F7A78.unk10 >= 2) {
            D_801F7A78.unk4 = 1;
        }
    }
    return D_801F7A78.unk4 == 0;
}

void func_801F0FC4(void) {
    func_8003556C(0);
    func_801F2E24();
    D_801F7A78.unk2 = 1;
}

void func_801F0FF4(void) {
    D_801F7A78.unk2 = 1;
}

s32 func_801F1004(void) {
    u8 mode = GDATA->unk56E;
    s32 ret = 0;

    if (mode == 4) {
        ret = D_800FDF4D != 0;
    } else if (mode != 7) {
        ret = 1;
        if (D_800FDF30.unkB != 0) {
            ret = 2;
        }
    }
    return ret;
}

void func_801F1060(void) {
    D_801F7A78.unk1 = 0;
    D_801F7A78.unk2 = 1;
    D_801F7A78.unk4 = 0;
}

void func_801F107C(void) {
    s32 i;

    D_801F7A78.unk1 = 1;
    D_801F7A78.unk2 = 0;
    func_801F0C0C();
    func_801F1300(&D_801F7A78.unk348);
    for (i = 0; i < 4; i++) {
        func_801F1300(&D_801F7A78.unk158[i]);
        func_801F1300(&D_801F7A78.unk1D8[i]);
    }
    func_8003575C(D_801EDF04);
    func_801F12E0();
    func_801F2490();
    func_801F1FEC();
    func_801F3250();
    func_801F2D74();
    func_801F2BBC();
    func_801F04BC();
    GSTATE->unk9 = 0;
    func_8002F9D0();
}

void func_801F114C(void) {
    D_801F7A78.unk1 = 0;
    D_801F7A78.unk2 = 0;
}

void func_801F1160(void) {
    func_80024BD8();
    func_801EFB98();
    func_801F1228();
    func_801EFC68();
    func_801EFF50();
    func_801F0044();
    func_801F0138();
    func_801F0264();
}

void func_801F11B8(void) {
    void (*fn)(void) = D_801F3B88[D_801F7A78.unk8];

    if (fn != NULL) {
        fn();
    }
    func_801F13B4();
    func_801F0594();
    D_801F7A78.unk36C++;
}

void func_801F1228(void) {
    s32 x, y;
    RECT *r = &D_801F7A78.unkAC[1].unk4;
    func_8002B524(r->x + 6, r->y - 3, 0xB, D_80070450, 0);
    func_8002DEE4(r, 0xB, 1);
    func_8002705C(r, 0xC, 1);
    x = r->x;
    y = r->y;
    D_801F7A78.unk374 = func_8002920C(x + 0x13, y + 6, D_8007716C + 10,
                                      func_80035774(7), 0, D_801F7A78.unk374);
}

void func_801F12E0(void) {
    D_801F7A78.unk378[0] = D_801F5278[0];
    D_801F7A78.unk378[1] = D_801F5278[1];
}

void func_801F1300(WinSlot *arg0) {
    arg0->unk4 = 0;
    arg0->unk6 = 0;
    arg0->unk1C = 0;
    arg0->unk8 = 0;
    arg0->unk0 = 0;
}

void func_801F1318(void) {
    s32 i;

    for (i = 0; i < 4; i++) {
        if (D_801F7A78.unk14[i] != 0 && D_801F7A78.unk158[i].unk0 != NULL) {
            func_8002DD00(D_801F7A78.unk158[i].unk0);
        }
        if (D_801F7A78.unk18[i] != 0 && D_801F7A78.unk1D8[i].unk0 != NULL) {
            func_8002DD00(D_801F7A78.unk1D8[i].unk0);
        }
    }
}

void func_801F13B4(void) {
    s32 i;

    for (i = 0; i < 10; i++) {
        if (D_801F7A78.unk14C[i] != 0 && D_801F7A78.unkAC[i].unkC != 0) {
            func_800631AC(&D_801F7A78.unkAC[i]);
        }
    }
}

void func_801F143C(void) {
    s32 se;

    if (!func_8002F810() && (D_80077528 & 0x20000)) {
        se = D_801F3BA0[D_801F7A78.unk8];
        if (se != 0) {
            func_8002F488(se);
        }
        D_801F7A78.unk8++;
        func_801F1560();
    }
}

void func_801F14C0(void) {
    func_801F2538();
    func_801F2080();
    if (!func_8002F810() && (D_80077528 & 0x20000)) {
        func_801F0878();
    }
}

void func_801F1518(void) {
    if (!func_8002F810() && (D_80077528 & 0x20000)) {
        func_801F160C();
    }
}

void func_801F1560(void) {
    Rewards *r = &D_801F7A78.unk24;
    s32 i;
    u8 any = 0;

    for (i = 0; i < 4; i++) {
        if (D_801F7A78.unk18[i] != 0) {
            func_801F1A04(i, r->unk4C[i].unk0);
        }
        if (D_801F7A78.unk14[i] != 0) {
            any = 1;
        }
    }
    if (any) {
        func_8002F514(0x69);
    }
}

void func_801F160C(void) {
    s32 se;

    if (func_801F3264()) {
        se = D_801F3BA0[D_801F7A78.unk8];
        if (se != 0) {
            func_8002F488(se);
        }
        D_801F7A78.unk8++;
        if (D_800FDF30.unk0 != 0) {
            func_8002F514(0x6D);
        }
    }
}

void func_801F1688(void) {
    Rewards *r = &D_801F7A78.unk24;

    if (r->unk7C.unk0 != r->unk7C.unk8) {
        func_801F2D08(r->unk7C.unk0 - r->unk7C.unk8);
        r->unk7C.unk8 = r->unk7C.unk0;
        func_8002F55C(0x6D);
        D_801F7A78.unkC = 0;
    } else {
        D_801F7A78.unkC = 24;
    }
    D_801F7A78.unk3 = 1;
}

void func_801F16F4(void) {
    func_801F332C();
    func_8002FA50(0xC);
}

u8 func_801F171C(PartyMember *arg0, s32 arg1) {
    if (!func_801F_alive(arg0)) {
        return 0;
    }
    return (D_800FDF30.unkA >> arg1) & 1;
}

void func_801F1774(void) {
    s32 se = D_801F3BA0[D_801F7A78.unk8];

    if (se != 0) {
        func_8002F488(se);
    }
    D_801F7A78.unk8++;
}

void func_801F17D4(void) {
    D_801F7A78.unk2 = 1;
    func_8002DE60();
}

void func_801F17FC(s32 arg0, s32 arg1, s32 arg2, u8 *arg3, s32 arg4) {
    D_801F7A78.unk374 = func_8002B644(arg0, arg1, D_8007716C + arg2, arg3, arg4, D_801F7A78.unk374);
}

INCLUDE_ASM("asm/us/ovl_65/nonmatchings/ovl_65", func_801F1848);

INCLUDE_ASM("asm/us/ovl_65/nonmatchings/ovl_65", func_801F1A04);

INCLUDE_ASM("asm/us/ovl_65/nonmatchings/ovl_65", func_801F1BA4);

INCLUDE_ASM("asm/us/ovl_65/nonmatchings/ovl_65", func_801F1D94);

INCLUDE_ASM("asm/us/ovl_65/nonmatchings/ovl_65", func_801F1FEC);

void func_801F2080(void) {
    func_801F213C();
    func_801F1848();
}

INCLUDE_ASM("asm/us/ovl_65/nonmatchings/ovl_65", func_801F20A8);

INCLUDE_ASM("asm/us/ovl_65/nonmatchings/ovl_65", func_801F213C);

INCLUDE_ASM("asm/us/ovl_65/nonmatchings/ovl_65", func_801F21DC);

void func_801F2248(Window *arg0) {
    D_801F7A78.unk1D8[arg0->unk50].unk0 = 0;
}

INCLUDE_ASM("asm/us/ovl_65/nonmatchings/ovl_65", func_801F2264);

INCLUDE_ASM("asm/us/ovl_65/nonmatchings/ovl_65", func_801F2490);

void func_801F2538(void) {
    func_801F2834();
    func_801F28F0();
}

INCLUDE_ASM("asm/us/ovl_65/nonmatchings/ovl_65", func_801F2560);

INCLUDE_ASM("asm/us/ovl_65/nonmatchings/ovl_65", func_801F269C);

INCLUDE_ASM("asm/us/ovl_65/nonmatchings/ovl_65", func_801F26F8);

s32 func_801F27F0(PartyMember *arg0, s32 arg1) {
    if (arg0->unkB >= 99) {
        return 0;
    }
    return arg0->unkC + arg1 >= D_8006DF5C[arg0->unkB];
}

INCLUDE_ASM("asm/us/ovl_65/nonmatchings/ovl_65", func_801F2834);

INCLUDE_ASM("asm/us/ovl_65/nonmatchings/ovl_65", func_801F28F0);

INCLUDE_ASM("asm/us/ovl_65/nonmatchings/ovl_65", func_801F29BC);

void func_801F2A4C(Popup *arg0) {
    arg0->unk0 = 0;
    arg0->unk14 = 0;
    arg0->unk16 = 0;
    arg0->unk4 = 0;
    arg0->unk1 = 0;
    arg0->unk2 = 1;
}

void func_801F2A6C(Popup *arg0) {
    arg0->unk4 = 0;
    arg0->unk16 = 0;
    arg0->unk1 = 1;
    arg0->unk0 = 1;
    arg0->unk14--;
    func_8002F488(0x2AB);
}

void func_801F2AAC(Popup *arg0) {
    arg0->unk0 = 0;
    arg0->unk4 = 0;
    arg0->unk16 = 0;
    arg0->unk15++;
}

INCLUDE_ASM("asm/us/ovl_65/nonmatchings/ovl_65", func_801F2AC8);

INCLUDE_ASM("asm/us/ovl_65/nonmatchings/ovl_65", func_801F2BBC);

INCLUDE_ASM("asm/us/ovl_65/nonmatchings/ovl_65", func_801F2C7C);

INCLUDE_ASM("asm/us/ovl_65/nonmatchings/ovl_65", func_801F2D08);

INCLUDE_ASM("asm/us/ovl_65/nonmatchings/ovl_65", func_801F2D74);

void func_801F2E24(void) {
    LoadImage(&D_801F525C, D_801F7DF8);
    DrawSync(0);
}

INCLUDE_ASM("asm/us/ovl_65/nonmatchings/ovl_65", func_801F2E58);

INCLUDE_ASM("asm/us/ovl_65/nonmatchings/ovl_65", func_801F2FE8);

void func_801F3250(void) {
    D_801F8278 = 0;
}

void func_801F325C(void) {
}

INCLUDE_ASM("asm/us/ovl_65/nonmatchings/ovl_65", func_801F3264);

INCLUDE_ASM("asm/us/ovl_65/nonmatchings/ovl_65", func_801F332C);

INCLUDE_ASM("asm/us/ovl_65/nonmatchings/ovl_65", func_801F33A0);

INCLUDE_ASM("asm/us/ovl_65/nonmatchings/ovl_65", func_801F3434);

void func_801F347C(void) {
    D_801F7A78.unk348.unk0 = NULL;
}

INCLUDE_RODATA("asm/us/ovl_65/nonmatchings/ovl_65", D_801EDF00);

INCLUDE_RODATA("asm/us/ovl_65/nonmatchings/ovl_65", D_801EDF04);

INCLUDE_RODATA("asm/us/ovl_65/nonmatchings/ovl_65", D_801EE03C);

INCLUDE_RODATA("asm/us/ovl_65/nonmatchings/ovl_65", D_801EE19C);

INCLUDE_RODATA("asm/us/ovl_65/nonmatchings/ovl_65", D_801EF260);

INCLUDE_ASM("asm/us/ovl_65/nonmatchings/ovl_65", func_801F3488);

INCLUDE_ASM("asm/us/ovl_65/nonmatchings/ovl_65", func_801F3818);

INCLUDE_ASM("asm/us/ovl_65/nonmatchings/ovl_65", func_801F3A20);

INCLUDE_ASM("asm/us/ovl_65/nonmatchings/ovl_65", func_801F3A7C);

INCLUDE_ASM("asm/us/ovl_65/nonmatchings/ovl_65", func_801F3B8C);

INCLUDE_RODATA("asm/us/ovl_65/nonmatchings/ovl_65", D_801EF27C);
