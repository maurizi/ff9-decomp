#include "ovl_0b.h"

s32 func_800CFC0C(s32 arg0) {
    s32 v = arg0 & ~3;
    if (arg0 & 3) {
        v += 4;
    }
    return v;
}

INCLUDE_ASM("asm/us/ovl_0b/nonmatchings/ovl_0b_2", func_800CFC2C);

INCLUDE_ASM("asm/us/ovl_0b/nonmatchings/ovl_0b_2", func_800CFCA8);

INCLUDE_ASM("asm/us/ovl_0b/nonmatchings/ovl_0b_2", func_800CFD78);

INCLUDE_ASM("asm/us/ovl_0b/nonmatchings/ovl_0b_2", func_800CFE88);

INCLUDE_ASM("asm/us/ovl_0b/nonmatchings/ovl_0b_2", func_800CFF40);

INCLUDE_ASM("asm/us/ovl_0b/nonmatchings/ovl_0b_2", func_800D001C);

INCLUDE_ASM("asm/us/ovl_0b/nonmatchings/ovl_0b_2", func_800D006C);

INCLUDE_ASM("asm/us/ovl_0b/nonmatchings/ovl_0b_2", func_800D0130);

INCLUDE_ASM("asm/us/ovl_0b/nonmatchings/ovl_0b_2", func_800D01D4);

INCLUDE_ASM("asm/us/ovl_0b/nonmatchings/ovl_0b_2", func_800D044C);

INCLUDE_ASM("asm/us/ovl_0b/nonmatchings/ovl_0b_2", func_800D0580);

INCLUDE_ASM("asm/us/ovl_0b/nonmatchings/ovl_0b_2", func_800D0674);

INCLUDE_ASM("asm/us/ovl_0b/nonmatchings/ovl_0b_2", func_800D07B4);

INCLUDE_ASM("asm/us/ovl_0b/nonmatchings/ovl_0b_2", func_800D0858);

void func_800D08C4(s32 arg0, s32 arg1) {
    u32 i = (arg1 - D_800F78A4->unk28) >> 2;
    if (i < 0x1000) {
        func_800D07B4(arg0, &D_800F78A4->unk20[i]);
    }
}

INCLUDE_ASM("asm/us/ovl_0b/nonmatchings/ovl_0b_2", func_800D090C);

INCLUDE_ASM("asm/us/ovl_0b/nonmatchings/ovl_0b_2", func_800D0940);

INCLUDE_ASM("asm/us/ovl_0b/nonmatchings/ovl_0b_2", func_800D0A0C);

INCLUDE_ASM("asm/us/ovl_0b/nonmatchings/ovl_0b_2", func_800D0B18);

INCLUDE_ASM("asm/us/ovl_0b/nonmatchings/ovl_0b_2", func_800D0E14);

INCLUDE_ASM("asm/us/ovl_0b/nonmatchings/ovl_0b_2", func_800D0FFC);

INCLUDE_ASM("asm/us/ovl_0b/nonmatchings/ovl_0b_2", func_800D12C4);

INCLUDE_ASM("asm/us/ovl_0b/nonmatchings/ovl_0b_2", func_800D1494);

INCLUDE_ASM("asm/us/ovl_0b/nonmatchings/ovl_0b_2", func_800D1680);

INCLUDE_ASM("asm/us/ovl_0b/nonmatchings/ovl_0b_2", func_800D198C);

INCLUDE_ASM("asm/us/ovl_0b/nonmatchings/ovl_0b_2", func_800D1BC4);

INCLUDE_ASM("asm/us/ovl_0b/nonmatchings/ovl_0b_2", func_800D1D74);

INCLUDE_ASM("asm/us/ovl_0b/nonmatchings/ovl_0b_2", func_800D1FFC);

INCLUDE_ASM("asm/us/ovl_0b/nonmatchings/ovl_0b_2", func_800D23D8);

INCLUDE_ASM("asm/us/ovl_0b/nonmatchings/ovl_0b_2", func_800D2938);

INCLUDE_ASM("asm/us/ovl_0b/nonmatchings/ovl_0b_2", func_800D2C78);

INCLUDE_ASM("asm/us/ovl_0b/nonmatchings/ovl_0b_2", func_800D2DBC);

INCLUDE_ASM("asm/us/ovl_0b/nonmatchings/ovl_0b_2", func_800D3030);

INCLUDE_ASM("asm/us/ovl_0b/nonmatchings/ovl_0b_2", func_800D3A6C);

INCLUDE_ASM("asm/us/ovl_0b/nonmatchings/ovl_0b_2", func_800D3B6C);

INCLUDE_ASM("asm/us/ovl_0b/nonmatchings/ovl_0b_2", func_800D3ED4);

INCLUDE_ASM("asm/us/ovl_0b/nonmatchings/ovl_0b_2", func_800D40CC);

INCLUDE_ASM("asm/us/ovl_0b/nonmatchings/ovl_0b_2", func_800D4228);

INCLUDE_ASM("asm/us/ovl_0b/nonmatchings/ovl_0b_2", func_800D437C);

INCLUDE_ASM("asm/us/ovl_0b/nonmatchings/ovl_0b_2", func_800D4478);

INCLUDE_ASM("asm/us/ovl_0b/nonmatchings/ovl_0b_2", func_800D45FC);

INCLUDE_ASM("asm/us/ovl_0b/nonmatchings/ovl_0b_2", func_800D4768);

INCLUDE_ASM("asm/us/ovl_0b/nonmatchings/ovl_0b_2", func_800D4A08);

INCLUDE_ASM("asm/us/ovl_0b/nonmatchings/ovl_0b_2", func_800D4A30);

INCLUDE_ASM("asm/us/ovl_0b/nonmatchings/ovl_0b_2", func_800D4D6C);

INCLUDE_ASM("asm/us/ovl_0b/nonmatchings/ovl_0b_2", func_800D50A0);

INCLUDE_ASM("asm/us/ovl_0b/nonmatchings/ovl_0b_2", func_800D5568);

INCLUDE_ASM("asm/us/ovl_0b/nonmatchings/ovl_0b_2", func_800D57D0);

INCLUDE_ASM("asm/us/ovl_0b/nonmatchings/ovl_0b_2", func_800D597C);

INCLUDE_ASM("asm/us/ovl_0b/nonmatchings/ovl_0b_2", func_800D59D0);

INCLUDE_ASM("asm/us/ovl_0b/nonmatchings/ovl_0b_2", func_800D5C30);

void func_800D616C(void) {
    D_800FEC40.unk0 = -1;
    D_800FEC40.unk22 = 0;
    D_800FEC40.unk24 = 0;
}

INCLUDE_ASM("asm/us/ovl_0b/nonmatchings/ovl_0b_2", func_800D6188);

INCLUDE_ASM("asm/us/ovl_0b/nonmatchings/ovl_0b_2", func_800D62A0);

void func_800D6340(void) {
    D_800FEC40.unk20 = 1;
    D_800FEC40.unk18 -= 3;
}

INCLUDE_ASM("asm/us/ovl_0b/nonmatchings/ovl_0b_2", func_800D6360);

INCLUDE_ASM("asm/us/ovl_0b/nonmatchings/ovl_0b_2", func_800D63B0);

void func_800D64C8(s32 arg0) {
    D_800FEC38 = 1;
    D_800FEC3C = D_800FEC40.unk18;
    D_800FEC40.unk18 = func_800D6360();
}

INCLUDE_ASM("asm/us/ovl_0b/nonmatchings/ovl_0b_2", func_800D650C);

void func_800D6D3C(void) {
    s32 i;
    Unk80100C78Entry *p;
    D_80100C78.unk0 = 0;
    func_800E3B34(&D_800FF470, 0x1800);
    p = D_80100C78.unk4;
    for (i = 10; i >= 0; i--) {
        p->unk4 = 0;
        p++;
    }
}

INCLUDE_ASM("asm/us/ovl_0b/nonmatchings/ovl_0b_2", func_800D6D8C);

INCLUDE_ASM("asm/us/ovl_0b/nonmatchings/ovl_0b_2", func_800D6E70);

INCLUDE_ASM("asm/us/ovl_0b/nonmatchings/ovl_0b_2", func_800D7094);

INCLUDE_ASM("asm/us/ovl_0b/nonmatchings/ovl_0b_2", func_800D71C8);

INCLUDE_ASM("asm/us/ovl_0b/nonmatchings/ovl_0b_2", func_800D72BC);

INCLUDE_ASM("asm/us/ovl_0b/nonmatchings/ovl_0b_2", func_800D73AC);

s32 func_800D73E4(s32 arg0, s32 arg1, s32 arg2, s32 arg3, u8 *arg4) {
    if (arg0 == 1) {
        D_80100D88.unk2 = arg4[0];
        D_80100D88.unk3 = arg4[4];
    }
    return 1;
}

INCLUDE_ASM("asm/us/ovl_0b/nonmatchings/ovl_0b_2", func_800D7414);

INCLUDE_ASM("asm/us/ovl_0b/nonmatchings/ovl_0b_2", func_800D74B8);

s32 func_800D7578(s32 arg0, u8 *arg1, s32 arg2, s32 arg3, s32 *arg4) {
    if (arg0 == 0) {
        *arg4 = 4;
    } else if (arg0 != 1) {
        return func_800D8D08(arg1, arg2, arg3);
    } else {
        arg1[0] = arg4[0];
        arg1[1] = arg4[1];
    }
    return 0;
}

s32 func_800D75EC(s32 arg0, u8 *arg1, s32 arg2, s32 arg3, s32 *arg4) {
    if (arg0 == 0) {
        *arg4 = 4;
    } else if (arg0 != 1) {
        return func_800D8D68(arg1, arg2, arg3);
    } else {
        arg1[0] = arg4[0];
        arg1[1] = arg4[1];
    }
    return 0;
}

INCLUDE_ASM("asm/us/ovl_0b/nonmatchings/ovl_0b_2", func_800D7660);

INCLUDE_ASM("asm/us/ovl_0b/nonmatchings/ovl_0b_2", func_800D76DC);

INCLUDE_ASM("asm/us/ovl_0b/nonmatchings/ovl_0b_2", func_800D7768);

INCLUDE_ASM("asm/us/ovl_0b/nonmatchings/ovl_0b_2", func_800D77E4);

INCLUDE_ASM("asm/us/ovl_0b/nonmatchings/ovl_0b_2", func_800D7870);

s32 func_800D78EC(s32 arg0, u8 *arg1, s32 arg2, s32 arg3, s32 *arg4) {
    if (arg0 == 0) {
        *arg4 = 8;
    } else if (arg0 != 1) {
        return func_800D9038(arg1, arg2, arg3);
    } else {
        *arg1 = *arg4;
    }
    return 0;
}

s32 func_800D7948(s32 arg0, u8 *arg1, s32 arg2, Unk800F78A40 *arg3, s32 *arg4) {
    if (arg0 == 0) {
        *arg4 = 4;
    } else if (arg0 != 1) {
        return func_800D905C(arg1, arg2, arg3);
    } else {
        arg1[0] = arg4[0];
        arg1[1] = arg4[1];
    }
    return 0;
}

INCLUDE_ASM("asm/us/ovl_0b/nonmatchings/ovl_0b_2", func_800D79BC);

s32 func_800D7A38(s32 arg0, u8 *arg1, s32 arg2, s32 arg3, s32 *arg4) {
    if (arg0 == 0) {
        *arg4 = 4;
    } else if (arg0 != 1) {
        return func_800D9110(arg1, arg2, arg3);
    } else {
        *arg1 = *arg4;
    }
    return 0;
}

INCLUDE_ASM("asm/us/ovl_0b/nonmatchings/ovl_0b_2", func_800D7A94);

INCLUDE_ASM("asm/us/ovl_0b/nonmatchings/ovl_0b_2", func_800D7B2C);

INCLUDE_ASM("asm/us/ovl_0b/nonmatchings/ovl_0b_2", func_800D7BA8);

INCLUDE_ASM("asm/us/ovl_0b/nonmatchings/ovl_0b_2", func_800D7C9C);

INCLUDE_ASM("asm/us/ovl_0b/nonmatchings/ovl_0b_2", func_800D7D64);

INCLUDE_ASM("asm/us/ovl_0b/nonmatchings/ovl_0b_2", func_800D7E14);

INCLUDE_ASM("asm/us/ovl_0b/nonmatchings/ovl_0b_2", func_800D7EA0);

INCLUDE_ASM("asm/us/ovl_0b/nonmatchings/ovl_0b_2", func_800D7F80);

INCLUDE_ASM("asm/us/ovl_0b/nonmatchings/ovl_0b_2", func_800D805C);

INCLUDE_ASM("asm/us/ovl_0b/nonmatchings/ovl_0b_2", func_800D8124);

INCLUDE_ASM("asm/us/ovl_0b/nonmatchings/ovl_0b_2", func_800D81EC);

INCLUDE_ASM("asm/us/ovl_0b/nonmatchings/ovl_0b_2", func_800D8270);

INCLUDE_ASM("asm/us/ovl_0b/nonmatchings/ovl_0b_2", func_800D8368);

INCLUDE_ASM("asm/us/ovl_0b/nonmatchings/ovl_0b_2", func_800D8414);

s32 func_800D84C4(s32 arg0, u8 *arg1, s32 arg2, s32 arg3, s32 *arg4) {
    if (arg0 == 0) {
        *arg4 = 4;
    } else if (arg0 != 1) {
        return func_800DA030(arg1, arg2, arg3);
    } else {
        *arg1 = *arg4;
    }
    return 0;
}

INCLUDE_ASM("asm/us/ovl_0b/nonmatchings/ovl_0b_2", func_800D8520);

void func_800D85AC(s32 arg0, s32 arg1) {
    if (func_800D8A64(arg0) != 0) {
        func_800D86E0(arg0, func_800D8740(arg0, arg1));
    }
}

void func_800D85F8(Unk800F78A40 *arg0, s32 arg1, s32 arg2) {
    if (func_800D8A64(arg1) != 0) {
        func_800D86E0(arg1, func_800DA3D8(arg0->unk8, arg2));
    }
}

INCLUDE_ASM("asm/us/ovl_0b/nonmatchings/ovl_0b_2", func_800D8658);

void func_800D86E0(s32 arg0, s32 arg1) {
    if (func_800D8A64(arg0) != 0) {
        BtlUnit *u = func_800E716C(arg0);
        BtlUnit9C *m = u->unk9C;
        if (m != NULL) {
            m->unkC = (BtlUnit9CC *)arg1;
            u->unkA0->unk3A = 0;
        }
    }
}

s32 func_800D8740(s32 arg0, s32 arg1) {
    return func_800E716C(arg0)->unkA4[arg1];
}

void func_800D8774(s32 arg0, s32 arg1) {
    BtlUnit *u = func_800E716C(arg0);
    if (arg1 >= u->unk9C->unkC->unk2) {
        arg1 = u->unk9C->unkC->unk2;
    }
    u->unkA0->unk3A = arg1;
}

s32 func_800D87D0(s32 arg0) {
    return func_800E716C(arg0)->unk9C->unkC->unk2;
}

u8 func_800D87FC(s32 arg0) {
    return func_800E716C(arg0)->unkA0->unk3A;
}

INCLUDE_ASM("asm/us/ovl_0b/nonmatchings/ovl_0b_2", func_800D8820);

void func_800D8914(s32 arg0) {
    BtlUnit *u = func_800E716C(arg0);
    if (func_800D8A64(arg0) != 0) {
        func_800AD90C(0xD000, func_800AE47C((u8)u->unk68 >> 5, 0), 0, func_800E976C(arg0), 0x7F);
    }
}

void func_800D8988(s32 arg0, s32 arg1, s32 arg2) {
    BtlUnit *a = func_800E716C(arg0);
    BtlUnit *b = func_800E716C(arg1);
    if (func_800D8A64(arg0) != 0) {
        func_800C2200(a, b);
    }
}

u32 func_800D89E8(s32 arg0) {
    BtlUnit *u = func_800E716C(arg0);
    u32 f = u->unk68;
    u->unk68 = f | 0x200000;
    return (f >> 21) & 1;
}

void func_800D8A20(s32 arg0, s32 arg1) {
    BtlUnit *u = func_800E716C(arg0);
    u->unk68 = (u->unk68 & ~0x200000) | ((arg1 & 1) << 21);
}

s32 func_800D8A64(s32 arg0) {
    if (arg0 == 0xFF) {
        arg0 = 0;
    }
    return func_800E716C(arg0)->unk68 & 1;
}

s32 func_800D8A94(s32 arg0, s32 arg1) {
    return func_800D8A64(arg0) == func_800D8A64(arg1);
}

s32 func_800D8ACC(s32 arg0, s32 arg1) {
    BtlUnit *a;
    if (arg0 == 0xFF || arg1 == 0xFF) {
        return 0;
    }
    a = func_800E716C(arg0);
    return a == func_800E716C(arg1);
}

s32 func_800D8B20(s32 arg0) {
    s32 idx = (func_800E716C(arg0)->unk68 >> 1) & 0xF;
    return (D_8006794C->unk1C->unk3C[idx].unk34 >> 4) & 0x1F;
}

s32 func_800D8B70(s32 arg0) {
    s32 idx = (func_800E716C(arg0)->unk68 >> 1) & 0xF;
    return D_8006794C->unk1C->unk3C[idx].unk39[0];
}

void func_800D8BB8(s32 arg0, s32 arg1) {
    BtlUnit *u = func_800E716C(arg0);
    if (arg1 != 0) {
        u->unk68 &= ~0x400000;
    } else {
        u->unk68 |= 0x400000;
    }
}

void func_800D8C08(s32 arg0, s32 arg1) {
    BtlUnit *u = func_800E716C(arg0);
    if (arg1 != 0) {
        func_800BD53C(u);
    } else {
        func_800BD4DC(u);
    }
}

s32 func_800D8C4C(s32 arg0) {
    return func_800D8C70(arg0, 0x02001103) == 0;
}

u32 func_800D8C70(s32 arg0, u32 arg1) {
    return func_800E716C(arg0)->unk3C & arg1;
}

s32 func_800D8C9C(void) {
    return 0;
}

s32 func_800D8CA4(void) {
    return -1;
}

void func_800D8CAC(void) {
}

s32 func_800D8CB4(u8 *arg0) {
    func_800D8914(*arg0);
    return 1;
}

s32 func_800D8CD8(u8 *arg0) {
    func_800D8988(arg0[0], arg0[1], arg0[2]);
    return 1;
}

s32 func_800D8D08(u8 *arg0, s32 arg1, s32 arg2) {
    BtlUnit *u = func_800E716C(arg0[0]);
    switch (arg0[1]) {
    case 0:
        func_800C22CC(u, 0x456, 0x7F);
        break;
    case 1:
        func_800C22CC(u, 0xAC0, 0x7F);
        break;
    }
    return 1;
}

s32 func_800D8D68(u8 *arg0, s32 arg1, s32 arg2) {
    func_800D85AC(arg0[0], arg0[1]);
    return 1;
}

INCLUDE_ASM("asm/us/ovl_0b/nonmatchings/ovl_0b_2", func_800D8D94);

INCLUDE_ASM("asm/us/ovl_0b/nonmatchings/ovl_0b_2", func_800D8E10);

s32 func_800D8EA4(Unk800D8EA4 *arg0, s32 arg1, s32 *arg2) {
    s32 v = *arg2;
    if (v == 0) {
        func_800D85AC(arg0->unk0, 0x17);
        arg0->unk4 = func_800D87D0(arg0->unk0) - 1;
    }
    if (v >= arg0->unk4) {
        D_800FEC54 = 1;
        return 1;
    }
    return 0;
}

INCLUDE_ASM("asm/us/ovl_0b/nonmatchings/ovl_0b_2", func_800D8F1C);

INCLUDE_ASM("asm/us/ovl_0b/nonmatchings/ovl_0b_2", func_800D8FAC);

s32 func_800D9038(u8 *arg0, s32 arg1, s32 arg2) {
    func_800D8658(*arg0, 1);
    return 1;
}

s32 func_800D905C(u8 *arg0, s32 arg1, Unk800F78A40 *arg2) {
    func_800D85F8(arg2, arg0[0], arg0[1]);
    return 1;
}

INCLUDE_ASM("asm/us/ovl_0b/nonmatchings/ovl_0b_2", func_800D908C);

s32 func_800D9110(u8 *arg0, s32 arg1, s32 arg2) {
    func_800D8A20(*arg0, 0);
    return 1;
}

INCLUDE_ASM("asm/us/ovl_0b/nonmatchings/ovl_0b_2", func_800D9134);

INCLUDE_ASM("asm/us/ovl_0b/nonmatchings/ovl_0b_2", func_800D9260);

INCLUDE_ASM("asm/us/ovl_0b/nonmatchings/ovl_0b_2", func_800D9350);

INCLUDE_ASM("asm/us/ovl_0b/nonmatchings/ovl_0b_2", func_800D9488);

INCLUDE_ASM("asm/us/ovl_0b/nonmatchings/ovl_0b_2", func_800D955C);

INCLUDE_ASM("asm/us/ovl_0b/nonmatchings/ovl_0b_2", func_800D960C);

INCLUDE_ASM("asm/us/ovl_0b/nonmatchings/ovl_0b_2", func_800D974C);

INCLUDE_ASM("asm/us/ovl_0b/nonmatchings/ovl_0b_2", func_800D9990);

INCLUDE_ASM("asm/us/ovl_0b/nonmatchings/ovl_0b_2", func_800D9AB0);

INCLUDE_ASM("asm/us/ovl_0b/nonmatchings/ovl_0b_2", func_800D9D58);

s32 func_800D9F5C(u8 *arg0) {
    func_800D8820(arg0[0], arg0[2]);
    return 1;
}

s32 func_800D9F88(u8 *arg0, s32 arg1, s32 *arg2) {
    s32 v = *arg2;
    if (v == func_800DA06C(arg0[0], arg0[2]) - 1) {
        D_800FEC57 = 1;
        return 1;
    }
    return 0;
}

s32 func_800D9FD8(u8 *arg0, s32 arg1, s32 *arg2) {
    if (*arg2 == arg0[2] - 1) {
        D_800FEC57 = 1;
        return 1;
    }
    return 0;
}

s32 func_800DA004(u8 *arg0, s32 arg1, s32 *arg2) {
    if (*arg2 == arg0[2] - 1) {
        D_800FEC57 = 1;
        return 1;
    }
    return 0;
}

s32 func_800DA030(u8 *arg0, s32 arg1, s32 arg2) {
    s32 v = func_800DA0B8(*arg0);
    D_80100D8C = v;
    func_800D64C8(v);
    return 1;
}

s32 func_800DA064(void) {
    return 1;
}

s32 func_800DA06C(s32 arg0, s32 arg1) {
    s32 i = func_800D8B20(arg0);
    return D_800F73A0[arg1][i];
}

s32 func_800DA0B8(s32 arg0) {
    s32 v = func_800D8B70(arg0);
    if (v >= 0x55) {
        v = 0;
    }
    return D_800F7460[v];
}

s32 func_800DA0FC(void) {
    return 0;
}

s32 func_800DA104(s32 arg0, s32 arg1) {
    return (func_800CFC0C(arg0) + 4) * arg1 + 0xC;
}

INCLUDE_ASM("asm/us/ovl_0b/nonmatchings/ovl_0b_2", func_800DA138);

void func_800DA1B8(Unk800DA1B8 *arg0) {
    s32 i;
    s32 *p = arg0->unkC;
    for (i = 0; i < arg0->unk2; i++) {
        *p = 0;
        p = (s32 *)((u8 *)p + arg0->unk0);
    }
}

INCLUDE_ASM("asm/us/ovl_0b/nonmatchings/ovl_0b_2", func_800DA1F4);

INCLUDE_ASM("asm/us/ovl_0b/nonmatchings/ovl_0b_2", func_800DA2AC);

s32 func_800DA3A4(s32 arg0) {
    s32 i;
    for (i = 0; i < 2; i++) {
        if (arg0 == D_80100DC0.unk2C[i]) {
            break;
        }
    }
    return i;
}

INCLUDE_ASM("asm/us/ovl_0b/nonmatchings/ovl_0b_2", func_800DA3D8);

s32 func_800DA480(s32 arg0, s32 arg1) {
    s32 v = func_800DA3D8(arg0, arg1);
    return func_800DA3D8(arg0, arg1 + 1) - v;
}

INCLUDE_ASM("asm/us/ovl_0b/nonmatchings/ovl_0b_2", func_800DA4C4);

void func_800DA4E8(void) {
    D_80100DCC = 0;
}

INCLUDE_ASM("asm/us/ovl_0b/nonmatchings/ovl_0b_2", func_800DA4F4);

s32 func_800DA5C4(void) {
    if (D_80100DC0.unkC != 0) {
        return -1;
    }
    D_80100DC0.unkC = 3;
    D_80100DC0.unkE = 0;
    D_80100DC0.unk50 = 0;
    D_80100DC0.unk68 = 0;
    D_80100DC0.unk64 = 0;
    D_80100DC0.unk65 = 0;
    D_80100DC0.unk67 = 0;
    D_80100DC0.unk1A++;
    D_80100DC0.unk8 = (D_80100DC0.unk8 + 1) & 1;
    D_80100DC0.unk2C[D_80100DC0.unk8] = -1;
    return 0;
}

INCLUDE_ASM("asm/us/ovl_0b/nonmatchings/ovl_0b_2", func_800DA634);

s32 func_800DA6B4(s32 arg0, s32 arg1) {
    if (D_80100DC0.unk66 != 0) {
        if (func_80055DCC() == 0) {
            D_80100DC0.unk66 = 0;
            func_80056388(0x80, arg1);
            return 0;
        }
    }
    return 1;
}

void func_800DA71C(RECT *arg0) {
    if (arg0->x >= 0x300 && arg0->x + arg0->w < 0x381) {
        if (arg0->y >= 0x100 && arg0->y + arg0->h < 0x201) {
            D_800FEC40.unk1C |= 1;
        }
    }
}

INCLUDE_ASM("asm/us/ovl_0b/nonmatchings/ovl_0b_2", func_800DA790);

void func_800DA898(void) {
    if (D_800F750C == 2) {
        func_80012D08();
        if (DrawSync(1) == 0) {
            D_800F750C = 1;
            func_80012CF8();
        }
        func_80012D18();
    }
}

s32 func_800DA8EC(void) {
    s32 ret = 0;
    switch (D_800F750C) {
    case 0:
        D_800F750C = 2;
        break;
    case 1:
        D_800F750C = 0;
        ret = 1;
        break;
    case 2:
        break;
    }
    return ret;
}

INCLUDE_ASM("asm/us/ovl_0b/nonmatchings/ovl_0b_2", func_800DA92C);

INCLUDE_ASM("asm/us/ovl_0b/nonmatchings/ovl_0b_2", func_800DB424);

INCLUDE_ASM("asm/us/ovl_0b/nonmatchings/ovl_0b_2", func_800DB4EC);

INCLUDE_ASM("asm/us/ovl_0b/nonmatchings/ovl_0b_2", func_800DB5B4);

INCLUDE_ASM("asm/us/ovl_0b/nonmatchings/ovl_0b_2", func_800DB6B8);

INCLUDE_ASM("asm/us/ovl_0b/nonmatchings/ovl_0b_2", func_800DB7F8);

s32 func_800DB8D8(void) {
    return 0;
}

INCLUDE_ASM("asm/us/ovl_0b/nonmatchings/ovl_0b_2", func_800DB8E0);

INCLUDE_ASM("asm/us/ovl_0b/nonmatchings/ovl_0b_2", func_800DBA80);

INCLUDE_ASM("asm/us/ovl_0b/nonmatchings/ovl_0b_2", func_800DBBFC);

INCLUDE_ASM("asm/us/ovl_0b/nonmatchings/ovl_0b_2", func_800DBD10);

INCLUDE_ASM("asm/us/ovl_0b/nonmatchings/ovl_0b_2", func_800DBE98);

INCLUDE_ASM("asm/us/ovl_0b/nonmatchings/ovl_0b_2", func_800DC048);

INCLUDE_ASM("asm/us/ovl_0b/nonmatchings/ovl_0b_2", func_800DC208);

INCLUDE_ASM("asm/us/ovl_0b/nonmatchings/ovl_0b_2", func_800DC3F8);

void func_800DC434(s32 arg0, s32 arg1) {
    Unk8010141C *p = func_800DD02C(arg0, D_800A7F34);
    if (p->unk0->unkC->unk2 < arg1) {
        p->unk50 = 0;
    } else {
        p->unk50 = arg1;
    }
}

BtlObjSlot *func_800DC494(void) {
    BtlObjSlot *p = D_80100F1C;
    s32 i;
    for (i = 0; i < 32; i++, p++) {
        if (p->unk1C == 0) {
            return p;
        }
    }
    return NULL;
}

Unk8010141C *func_800DC4CC(void) {
    Unk8010141C *p = D_8010141C;
    s32 i;
    for (i = 0; i < 1; i++, p++) {
        if (p->unk4C == 0) {
            return p;
        }
    }
    return NULL;
}

void func_800DC500(void) {
}

void func_800DC508(void) {
}

INCLUDE_ASM("asm/us/ovl_0b/nonmatchings/ovl_0b_2", func_800DC510);

void func_800DC634(s32 arg0) {
    char buf[0x100];
    sprintf(buf, D_800A7F60, arg0);
    func_800DC510(buf);
}

INCLUDE_ASM("asm/us/ovl_0b/nonmatchings/ovl_0b_2", func_800DC668);

void func_800DC6E8(s32 arg0) {
    func_800DCFB0(arg0, D_800A7F78)->unk1C = 0;
}

INCLUDE_ASM("asm/us/ovl_0b/nonmatchings/ovl_0b_2", func_800DC710);

INCLUDE_ASM("asm/us/ovl_0b/nonmatchings/ovl_0b_2", func_800DC748);

INCLUDE_ASM("asm/us/ovl_0b/nonmatchings/ovl_0b_2", func_800DC7E0);

void func_800DC818(s32 arg0, s32 arg1, u16 *arg2) {
    func_800DC9D0(func_800E716C(arg0)->unk9C, arg1, arg2);
}

void func_800DC858(s32 arg0, s32 arg1, u16 *arg2) {
    func_800DC9D0(func_800E716C(arg0)->unk9C, D_800F78A4->unkC->unk25[arg1], arg2);
}

void func_800DC8AC(s32 arg0, s32 arg1, BtlBone *arg2) {
    func_800DCA18(func_800E716C(arg0)->unk9C, arg1, arg2);
}

void func_800DC8EC(s32 arg0, s32 arg1, BtlBone *arg2) {
    func_800DCA18(func_800E716C(arg0)->unk9C, D_800F78A4->unkC->unk25[arg1], arg2);
}

INCLUDE_RODATA("asm/us/ovl_0b/nonmatchings/ovl_0b_2", D_800A7F34);

INCLUDE_RODATA("asm/us/ovl_0b/nonmatchings/ovl_0b_2", D_800A7F60);

INCLUDE_RODATA("asm/us/ovl_0b/nonmatchings/ovl_0b_2", D_800A7F78);

INCLUDE_ASM("asm/us/ovl_0b/nonmatchings/ovl_0b_2", func_800DC940);

INCLUDE_ASM("asm/us/ovl_0b/nonmatchings/ovl_0b_2", func_800DC988);

void func_800DC9D0(BtlUnit9C *arg0, s32 arg1, u16 *arg2) {
    arg2[0] = arg0->unk20[arg1].unk14;
    arg2[1] = arg0->unk20[arg1].unk18;
    arg2[2] = arg0->unk20[arg1].unk1C;
}

void func_800DCA18(BtlUnit9C *arg0, s32 arg1, BtlBone *arg2) {
    *arg2 = arg0->unk20[arg1];
}

INCLUDE_ASM("asm/us/ovl_0b/nonmatchings/ovl_0b_2", func_800DCA68);

INCLUDE_ASM("asm/us/ovl_0b/nonmatchings/ovl_0b_2", func_800DCB20);

INCLUDE_ASM("asm/us/ovl_0b/nonmatchings/ovl_0b_2", func_800DCC58);

INCLUDE_ASM("asm/us/ovl_0b/nonmatchings/ovl_0b_2", func_800DCCA0);

INCLUDE_ASM("asm/us/ovl_0b/nonmatchings/ovl_0b_2", func_800DCCE4);

INCLUDE_ASM("asm/us/ovl_0b/nonmatchings/ovl_0b_2", func_800DCDA4);

INCLUDE_ASM("asm/us/ovl_0b/nonmatchings/ovl_0b_2", func_800DCDF8);

INCLUDE_ASM("asm/us/ovl_0b/nonmatchings/ovl_0b_2", func_800DCE3C);

INCLUDE_ASM("asm/us/ovl_0b/nonmatchings/ovl_0b_2", func_800DCEA0);

INCLUDE_ASM("asm/us/ovl_0b/nonmatchings/ovl_0b_2", func_800DCED0);

INCLUDE_ASM("asm/us/ovl_0b/nonmatchings/ovl_0b_2", func_800DCF08);

INCLUDE_ASM("asm/us/ovl_0b/nonmatchings/ovl_0b_2", func_800DCF4C);

INCLUDE_ASM("asm/us/ovl_0b/nonmatchings/ovl_0b_2", func_800DCFB0);

INCLUDE_ASM("asm/us/ovl_0b/nonmatchings/ovl_0b_2", func_800DD02C);

void func_800DD0B0(s32 arg0) {
    func_800DD10C(func_800E716C(arg0)->unk9C);
}

void func_800DD0DC(s32 arg0) {
    func_800DD10C(func_800DCFB0(arg0, D_800A81A8)->unk0);
}

u8 func_800DD10C(BtlUnit9C *arg0) {
    return arg0->unk8->unk3;
}

INCLUDE_ASM("asm/us/ovl_0b/nonmatchings/ovl_0b_2", func_800DD120);

INCLUDE_ASM("asm/us/ovl_0b/nonmatchings/ovl_0b_2", func_800DD27C);

INCLUDE_ASM("asm/us/ovl_0b/nonmatchings/ovl_0b_2", func_800DDF10);

INCLUDE_ASM("asm/us/ovl_0b/nonmatchings/ovl_0b_2", func_800DE11C);

INCLUDE_ASM("asm/us/ovl_0b/nonmatchings/ovl_0b_2", func_800DF15C);

INCLUDE_RODATA("asm/us/ovl_0b/nonmatchings/ovl_0b_2", D_800A81A8);

INCLUDE_ASM("asm/us/ovl_0b/nonmatchings/ovl_0b_2", func_800DF508);

INCLUDE_ASM("asm/us/ovl_0b/nonmatchings/ovl_0b_2", func_800DF550);

INCLUDE_ASM("asm/us/ovl_0b/nonmatchings/ovl_0b_2", func_800DF588);

INCLUDE_ASM("asm/us/ovl_0b/nonmatchings/ovl_0b_2", func_800DF670);

INCLUDE_ASM("asm/us/ovl_0b/nonmatchings/ovl_0b_2", func_800DF738);

INCLUDE_ASM("asm/us/ovl_0b/nonmatchings/ovl_0b_2", func_800DF7A0);

void func_800DF920(void) {
}

s32 func_800DF928(void) {
    return 0;
}

s32 func_800DF930(void) {
    return 0;
}

void func_800DF938(void) {
}

INCLUDE_ASM("asm/us/ovl_0b/nonmatchings/ovl_0b_2", func_800DF940);

INCLUDE_ASM("asm/us/ovl_0b/nonmatchings/ovl_0b_2", func_800DFE4C);

INCLUDE_ASM("asm/us/ovl_0b/nonmatchings/ovl_0b_2", func_800DFE54);

INCLUDE_ASM("asm/us/ovl_0b/nonmatchings/ovl_0b_2", func_800DFF5C);

INCLUDE_ASM("asm/us/ovl_0b/nonmatchings/ovl_0b_2", func_800DFF64);

INCLUDE_ASM("asm/us/ovl_0b/nonmatchings/ovl_0b_2", func_800E0014);

INCLUDE_ASM("asm/us/ovl_0b/nonmatchings/ovl_0b_2", func_800E00DC);

INCLUDE_ASM("asm/us/ovl_0b/nonmatchings/ovl_0b_2", func_800E0134);

INCLUDE_RODATA("asm/us/ovl_0b/nonmatchings/ovl_0b_2", D_800A825C);

INCLUDE_ASM("asm/us/ovl_0b/nonmatchings/ovl_0b_2", func_800E0168);

INCLUDE_ASM("asm/us/ovl_0b/nonmatchings/ovl_0b_2", func_800E14D8);

INCLUDE_ASM("asm/us/ovl_0b/nonmatchings/ovl_0b_2", func_800E1534);

INCLUDE_ASM("asm/us/ovl_0b/nonmatchings/ovl_0b_2", func_800E1708);

INCLUDE_ASM("asm/us/ovl_0b/nonmatchings/ovl_0b_2", func_800E1810);

INCLUDE_ASM("asm/us/ovl_0b/nonmatchings/ovl_0b_2", func_800E1874);

INCLUDE_ASM("asm/us/ovl_0b/nonmatchings/ovl_0b_2", func_800E1F90);

INCLUDE_ASM("asm/us/ovl_0b/nonmatchings/ovl_0b_2", func_800E2000);

INCLUDE_ASM("asm/us/ovl_0b/nonmatchings/ovl_0b_2", func_800E2F84);

void func_800E3B34(Heap *arg0, s32 arg1) {
    s32 size = func_800CFC0C(arg1) - 0xC;
    arg0->unk0 = arg0->unkC;
    arg0->unk4 = size;
    arg0->unk8 = size;
}

INCLUDE_ASM("asm/us/ovl_0b/nonmatchings/ovl_0b_2", func_800E3B70);

s32 func_800E3BF8(Heap *arg0, s32 arg1) {
    s32 size = func_800CFC0C(arg1);
    s32 ret = 1;
    if (arg0->unk4 < size) {
        arg0->unk0 = arg0->unkC;
        arg0->unk4 = arg0->unk8;
    } else {
        ret = 0;
    }
    return ret;
}

void func_800E3C50(void) {
}

void func_800E3C58(void) {
    func_800E00DC(0x3000, D_801014B8, &D_801044B8);
    func_800E14D8(0x920, &D_801044C8);
}

void func_800E3C98(void) {
    func_800E2000(&D_801044B8, &D_801044C8);
    func_800E0168(&D_801044B8);
    func_800E1874(&D_801044B8, &D_801044C8);
}

void func_800E3CEC(s32 arg0, s32 arg1) {
    func_800E1534(&D_801044B8, &D_801044C8, arg0, arg1);
}

void func_800E3D20(s32 arg0, s32 arg1) {
    if (arg0 != 0) {
        func_800E1708(&D_801044B8, arg0, arg1);
    }
}

void func_800E3D54(Pool6CEntry *arg0) {
    if (arg0 != NULL) {
        func_800E1810(&D_801044B8, arg0);
    }
}

void func_800E3D80(void) {
    func_800E4098();
    func_800E3DA8();
}

void func_800E3DA8(void) {
    func_800E72B8();
    func_800D616C();
    func_800D6D3C();
    func_800DA4E8();
    func_800E3C58();
    func_800E954C();
    func_800DC710();
    func_800DC7E0();
    func_80054F80();
}

INCLUDE_ASM("asm/us/ovl_0b/nonmatchings/ovl_0b_2", func_800E3E08);

INCLUDE_ASM("asm/us/ovl_0b/nonmatchings/ovl_0b_2", func_800E3EEC);

INCLUDE_ASM("asm/us/ovl_0b/nonmatchings/ovl_0b_2", func_800E4098);

INCLUDE_ASM("asm/us/ovl_0b/nonmatchings/ovl_0b_2", func_800E4134);

INCLUDE_ASM("asm/us/ovl_0b/nonmatchings/ovl_0b_2", func_800E4218);

void func_800E436C(s32 arg0, s32 arg1, s32 arg2, s32 arg3) {
    func_800D87D0(arg0);
    func_800E78B4(arg0, arg1, arg2, 2, 0, arg3, 3);
}

INCLUDE_ASM("asm/us/ovl_0b/nonmatchings/ovl_0b_2", func_800E43D8);

INCLUDE_ASM("asm/us/ovl_0b/nonmatchings/ovl_0b_2", func_800E4530);

s32 func_800E454C(s32 arg0) {
    return (s32)D_80104DF8 + D_80104DF8[arg0];
}

s32 func_800E4568(s32 arg0, s32 arg1, s32 arg2, s32 arg3, u8 *arg4) {
    switch (arg0) {
    case 1:
        D_80104DF0.unk0 = arg4[0];
        D_80104DF0.unk1 = arg4[4];
        break;
    }
    return 1;
}

s32 func_800E45A0(s32 arg0, s32 arg1, s32 arg2, s32 arg3, u8 *arg4) {
    if (arg0 == 1) {
        D_80104DF0.unk2 = arg4[0];
        D_80104DF0.unk3 = arg4[4];
    }
    return 1;
}

s32 func_800E45D0(s32 arg0, s32 arg1, s32 arg2, s32 arg3, s32 *arg4) {
    switch (arg0) {
    case 1:
        func_800E8F40(60 / *arg4);
        break;
    }
    return 1;
}

INCLUDE_ASM("asm/us/ovl_0b/nonmatchings/ovl_0b_2", func_800E4634);

void func_800E4984(s32 arg0) {
    s32 a = 0xFF;
    s32 b = arg0 * 16;
    if (D_800F78A4->unkC->unk24 == 1) {
        a = 0;
    }
    func_800D6E70(7, -1, a, b);
}

void func_800E49D0(s32 arg0, s32 arg1) {
    s32 i;
    for (i = 0; i < 3; i++) {
        func_800E9A00(D_800F8CF4[arg0][i], arg1);
    }
}

INCLUDE_ASM("asm/us/ovl_0b/nonmatchings/ovl_0b_2", func_800E4A3C);

void func_800E4C04(s32 arg0, s32 arg1, s32 arg2) {
    if (func_800E788C(arg0) == 0) {
        func_800E8B28(arg0, arg1, arg2);
    }
}

void func_800E4C54(void) {
    if (D_800F78A4->unkC->unk27 == 0) {
        D_800FEC65 = 1;
    }
}

INCLUDE_ASM("asm/us/ovl_0b/nonmatchings/ovl_0b_2", func_800E4C88);

INCLUDE_ASM("asm/us/ovl_0b/nonmatchings/ovl_0b_2", func_800E5538);

s32 func_800E56DC(s32 arg0, s32 arg1, s32 arg2, s32 arg3, u8 *arg4) {
    switch (arg0) {
    case 1:
        D_80104DF4.unk0 = arg4[0];
        D_80104DF4.unk1 = arg4[4];
        break;
    }
    return 1;
}

INCLUDE_ASM("asm/us/ovl_0b/nonmatchings/ovl_0b_2", func_800E5714);

INCLUDE_ASM("asm/us/ovl_0b/nonmatchings/ovl_0b_2", func_800E57B0);

void func_800E5D48(s8 arg0, s8 arg1, s8 arg2, s32 arg3, s32 arg4, s32 arg5) {
    D_80104DF4.unk0 = arg0;
    D_80104DF4.unk1 = arg1;
    D_80104DF0.unk1 = arg2;
    D_80104DF0.unk0 = arg5 | 0x80;
    func_800D6E70(1, D_800F78A4->unk0->unk8, arg3, arg4);
}

INCLUDE_ASM("asm/us/ovl_0b/nonmatchings/ovl_0b_2", func_800E5DA8);

INCLUDE_ASM("asm/us/ovl_0b/nonmatchings/ovl_0b_2", func_800E5EAC);

INCLUDE_ASM("asm/us/ovl_0b/nonmatchings/ovl_0b_2", func_800E5F24);

void func_800E60A4(s32 arg0, s32 arg1) {
    s32 v;
    if (arg0 < 0) {
        v = 0x80;
    } else {
        v = D_800F8D0C[arg0];
    }
    func_800D6E70(0xA, -1, v, arg1);
}

INCLUDE_ASM("asm/us/ovl_0b/nonmatchings/ovl_0b_2", func_800E60EC);

void func_800E6244(s32 arg0, s32 arg1) {
    func_800D6E70(4, -1, arg0 & 0xFF, arg1 & 0xFF);
}

INCLUDE_ASM("asm/us/ovl_0b/nonmatchings/ovl_0b_2", func_800E6270);

s32 func_800E639C(s32 arg0, s32 arg1, s32 arg2, Unk800F78A40 *arg3, s32 *arg4) {
    if (arg0 == 1) {
        s32 v = -1;
        if (arg4[1] != 0xFF) {
            v = arg4[1];
        }
        func_800E9930(arg4[0], arg3->unk8, v);
    }
    return 1;
}

void func_800E63E4(s32 arg0, s32 arg1) {
    func_800D6E70(0xE, -1, arg0, arg1);
}

INCLUDE_ASM("asm/us/ovl_0b/nonmatchings/ovl_0b_2", func_800E6410);

INCLUDE_ASM("asm/us/ovl_0b/nonmatchings/ovl_0b_2", func_800E651C);

INCLUDE_ASM("asm/us/ovl_0b/nonmatchings/ovl_0b_2", func_800E6624);

INCLUDE_ASM("asm/us/ovl_0b/nonmatchings/ovl_0b_2", func_800E6764);

INCLUDE_ASM("asm/us/ovl_0b/nonmatchings/ovl_0b_2", func_800E6C58);

void func_800E6D58(void) {
    *D_800F78A4->unk14 = D_80104E10;
}

INCLUDE_ASM("asm/us/ovl_0b/nonmatchings/ovl_0b_2", func_800E6DB8);

void func_800E6DF4(void) {
    func_800639D4(D_80104E00);
}

INCLUDE_ASM("asm/us/ovl_0b/nonmatchings/ovl_0b_2", func_800E6E1C);

void func_800E701C(BtlUnitList *arg0) {
    D_800F78A4->unkC = arg0;
}

INCLUDE_ASM("asm/us/ovl_0b/nonmatchings/ovl_0b_2", func_800E702C);

INCLUDE_ASM("asm/us/ovl_0b/nonmatchings/ovl_0b_2", func_800E70D0);

u8 *func_800E7154(void) {
    return D_800F78A4->unk24;
}

INCLUDE_ASM("asm/us/ovl_0b/nonmatchings/ovl_0b_2", func_800E716C);

s32 func_800E7298(BtlUnit *arg0) {
    return (arg0->unk68 & 0x800001) == 0x800000;
}

void func_800E72B8(void) {
    D_800F78A4->unkC = NULL;
}

INCLUDE_ASM("asm/us/ovl_0b/nonmatchings/ovl_0b_2", func_800E72C8);

u16 func_800E7370(BtlUnit *arg0) {
    return arg0->unk9C->unk8->unk8;
}

void func_800E738C(s32 arg0) {
    func_800E7370(func_800E716C(arg0));
}

void func_800E73B4(BtlUnit *arg0, u16 *arg1) {
    BtlBone *b = &arg0->unk9C->unk20[arg0->unk7C];
    arg1[0] = b->unk14;
    arg1[1] = b->unk18;
    arg1[2] = b->unk1C;
}

INCLUDE_ASM("asm/us/ovl_0b/nonmatchings/ovl_0b_2", func_800E73EC);

INCLUDE_ASM("asm/us/ovl_0b/nonmatchings/ovl_0b_2", func_800E748C);

void func_800E750C(s32 arg0, s32 arg1) {
    func_800E748C(func_800E716C(arg0), arg1);
}

INCLUDE_ASM("asm/us/ovl_0b/nonmatchings/ovl_0b_2", func_800E753C);

void func_800E7650(s32 arg0, s16 *arg1) {
    if (func_800C09A4(func_800E716C(arg0), 0x200000) & 0xFF) {
        arg1[1] -= 0x80;
    }
}

void func_800E769C(s32 arg0, s32 arg1, s32 arg2) {
    func_800E753C(func_800E716C(arg0), arg1, arg2);
}

void func_800E76DC(s32 arg0, s16 *arg1) {
    BtlUnit *u = func_800E716C(arg0);
    if (func_800E7298(u) == 0) {
        u->unk8C = arg1[0] << 16;
        u->unk90 = arg1[1] << 16;
        u->unk94 = arg1[2] << 16;
    }
}

INCLUDE_ASM("asm/us/ovl_0b/nonmatchings/ovl_0b_2", func_800E774C);

void func_800E77D8(s32 arg0, s16 *arg1) {
    BtlUnit *u = func_800E716C(arg0);
    s16 *p = u->unk86;
    arg1[0] = p[0];
    arg1[1] = p[1] + 0x800;
    arg1[2] = p[2];
}

void func_800E7824(s32 arg0, s16 *arg1) {
    BtlUnit *u = func_800E716C(arg0);
    if (func_800E7298(u) == 0) {
        s16 *p = u->unk86;
        p[0] = arg1[0];
        p[1] = arg1[1] - 0x800;
        p[2] = arg1[2];
    }
}

s32 func_800E788C(s32 arg0) {
    return (func_800E716C(arg0)->unk68 & 1) ^ 1;
}

INCLUDE_ASM("asm/us/ovl_0b/nonmatchings/ovl_0b_2", func_800E78B4);

INCLUDE_ASM("asm/us/ovl_0b/nonmatchings/ovl_0b_2", func_800E7A44);

INCLUDE_ASM("asm/us/ovl_0b/nonmatchings/ovl_0b_2", func_800E7ACC);

INCLUDE_ASM("asm/us/ovl_0b/nonmatchings/ovl_0b_2", func_800E7BA0);

INCLUDE_ASM("asm/us/ovl_0b/nonmatchings/ovl_0b_2", func_800E7C54);

s32 func_800E7CC4(s32 arg0, s32 arg1) {
    return (u16)(func_800E716C(arg0)->unk9C->unk8->unkA / 6) + arg1;
}

u32 func_800E7D18(s32 arg0) {
    return (u8)func_800E716C(arg0)->unk68 >> 5;
}

INCLUDE_ASM("asm/us/ovl_0b/nonmatchings/ovl_0b_2", func_800E7D3C);

INCLUDE_ASM("asm/us/ovl_0b/nonmatchings/ovl_0b_2", func_800E7EC0);

INCLUDE_ASM("asm/us/ovl_0b/nonmatchings/ovl_0b_2", func_800E7FC0);

void func_800E806C(s32 arg0, s32 arg1) {
    BtlUnit *u = func_800E716C(arg0);
    if (arg1 < 0) {
        if (arg1 == -1) {
            func_800BD414(u, 0xFFFF);
        } else {
            func_800BD464(u, 0xFFFF);
        }
    } else {
        func_800E7FC0(u, arg1);
    }
}

void func_800E80CC(s32 arg0, s32 arg1) {
    BtlUnit *u = func_800E716C(arg0);
    if (func_800E7298(u) == 0) {
        BtlUnit9C *m = u->unk9C;
        s16 v = arg1 - 0x80;
        if ((*(u32 *)m & 0x30) == 0x20) {
            func_800C86C8(m, v, v, v);
        }
    }
}

INCLUDE_ASM("asm/us/ovl_0b/nonmatchings/ovl_0b_2", func_800E813C);

void func_800E8274(s32 arg0) {
    if (func_800E788C(arg0) != 0) {
        func_800C0B6C(func_800E716C(arg0), -0x1381);
    } else {
        func_800C09EC(func_800E716C(arg0));
    }
}

INCLUDE_ASM("asm/us/ovl_0b/nonmatchings/ovl_0b_2", func_800E82CC);

void func_800E83EC(void) {
    D_8007AEF0.b[0xC7] |= 0x10;
}

void func_800E8408(BtlUnit *arg0) {
    if (arg0->unk3C & 0x10000000) {
        arg0->unk9C->unk0 |= 2;
        arg0->unk9C->unk54 = 0x800;
        arg0->unk9C->unk56 = 0x800;
        arg0->unk9C->unk58 = 0x800;
    }
}

void func_800E8460(BtlUnit *arg0) {
    if (arg0->unk3C & 0x04000000) {
        func_800BD414(arg0, arg0->unkAE);
    }
}

s32 func_800E8498(s32 arg0) {
    VECTOR v;
    func_800E7A44(arg0, &v);
    return func_800E716C(arg0)->unk9C->unk8->unkA * v.vx >> 12;
}

s32 func_800E84F0(s32 arg0) {
    s32 ret = 0;
    if (D_8006794C->unk1C->unk8E4.battle->unk1088->unkE == 0x33) {
        ret = func_800B3E7C(func_800E716C(arg0)->unk4) != 0;
    }
    return ret;
}

INCLUDE_ASM("asm/us/ovl_0b/nonmatchings/ovl_0b_2", func_800E8554);

INCLUDE_ASM("asm/us/ovl_0b/nonmatchings/ovl_0b_2", func_800E865C);

INCLUDE_ASM("asm/us/ovl_0b/nonmatchings/ovl_0b_2", func_800E86E0);

INCLUDE_ASM("asm/us/ovl_0b/nonmatchings/ovl_0b_2", func_800E871C);

INCLUDE_ASM("asm/us/ovl_0b/nonmatchings/ovl_0b_2", func_800E87C4);

INCLUDE_ASM("asm/us/ovl_0b/nonmatchings/ovl_0b_2", func_800E8854);

void func_800E8980(s32 arg0) {
    D_800F78A4->unkC->unk44 = func_800BED10(D_800F78A4->unkC->unk25[arg0]);
}

INCLUDE_ASM("asm/us/ovl_0b/nonmatchings/ovl_0b_2", func_800E89D0);

void func_800E8A80(s32 arg0) {
    BtlUnit *u = func_800E716C(arg0);
    if (u->unk68 & 1) {
        func_800C0D24(u);
        func_800E8408(u);
        func_800E8460(u);
    }
}

s32 func_800E8AD0(void) {
    BtlUnit *u = D_8006794C->unk1C->unk8E4.battle->unk6F8;
    s32 n = 0;
    while (u != NULL) {
        n += (u->unk68 & 0x400001) == 1;
        u = u->unk0;
    }
    return n;
}

INCLUDE_ASM("asm/us/ovl_0b/nonmatchings/ovl_0b_2", func_800E8B28);

void func_800E8C84(s32 arg0) {
    if (arg0 != 0) {
        D_8006794C->unk1C->unk8E4.battle->unk11B8 |= 2;
    } else {
        D_8006794C->unk1C->unk8E4.battle->unk11B8 &= ~2;
    }
}

s32 func_800E8CE4(void) {
    return (D_8006794C->unk1C->unk8E4.battle->unk11B8 >> 1) & 1;
}

void func_800E8D14(void) {
    if (func_800E788C(0x10) == 0) {
        func_800CDEF0(D_8006794C->unk1C->unk8E4.battle->unk1088);
    }
}

INCLUDE_ASM("asm/us/ovl_0b/nonmatchings/ovl_0b_2", func_800E8D60);

INCLUDE_ASM("asm/us/ovl_0b/nonmatchings/ovl_0b_2", func_800E8DC8);

INCLUDE_ASM("asm/us/ovl_0b/nonmatchings/ovl_0b_2", func_800E8E84);

void func_800E8F40(s32 arg0) {
    D_8006794C->unk9 = arg0;
}

INCLUDE_ASM("asm/us/ovl_0b/nonmatchings/ovl_0b_2", func_800E8F50);

INCLUDE_ASM("asm/us/ovl_0b/nonmatchings/ovl_0b_2", func_800E9000);

INCLUDE_ASM("asm/us/ovl_0b/nonmatchings/ovl_0b_2", func_800E9124);

INCLUDE_ASM("asm/us/ovl_0b/nonmatchings/ovl_0b_2", func_800E9168);

INCLUDE_ASM("asm/us/ovl_0b/nonmatchings/ovl_0b_2", func_800E9290);

INCLUDE_ASM("asm/us/ovl_0b/nonmatchings/ovl_0b_2", func_800E94C4);

void func_800E954C(void) {
    s32 i;
    D_80104E30.unk0 = 0;
    for (i = 5; i >= 0; i--) {
        D_80104E30.unk4[i].unk0 = 0;
    }
}

INCLUDE_ASM("asm/us/ovl_0b/nonmatchings/ovl_0b_2", func_800E9578);

INCLUDE_ASM("asm/us/ovl_0b/nonmatchings/ovl_0b_2", func_800E9638);

void func_800E970C(void) {
    func_8005546C(0, 0x3F);
}

INCLUDE_ASM("asm/us/ovl_0b/nonmatchings/ovl_0b_2", func_800E9730);

INCLUDE_ASM("asm/us/ovl_0b/nonmatchings/ovl_0b_2", func_800E976C);

INCLUDE_ASM("asm/us/ovl_0b/nonmatchings/ovl_0b_2", func_800E9884);

INCLUDE_ASM("asm/us/ovl_0b/nonmatchings/ovl_0b_2", func_800E98C8);

INCLUDE_ASM("asm/us/ovl_0b/nonmatchings/ovl_0b_2", func_800E98F8);

INCLUDE_ASM("asm/us/ovl_0b/nonmatchings/ovl_0b_2", func_800E9930);

INCLUDE_ASM("asm/us/ovl_0b/nonmatchings/ovl_0b_2", func_800E99B0);

INCLUDE_ASM("asm/us/ovl_0b/nonmatchings/ovl_0b_2", func_800E9A00);

INCLUDE_ASM("asm/us/ovl_0b/nonmatchings/ovl_0b_2", func_800E9A74);

INCLUDE_ASM("asm/us/ovl_0b/nonmatchings/ovl_0b_2", func_800E9BBC);

INCLUDE_ASM("asm/us/ovl_0b/nonmatchings/ovl_0b_2", func_800E9D30);

INCLUDE_ASM("asm/us/ovl_0b/nonmatchings/ovl_0b_2", func_800E9D84);

INCLUDE_ASM("asm/us/ovl_0b/nonmatchings/ovl_0b_2", func_800E9DBC);

void func_800E9DFC(void) {
    D_80104E68.unk1C = 0;
}

INCLUDE_ASM("asm/us/ovl_0b/nonmatchings/ovl_0b_2", func_800E9E10);

INCLUDE_ASM("asm/us/ovl_0b/nonmatchings/ovl_0b_2", func_800E9E80);

INCLUDE_ASM("asm/us/ovl_0b/nonmatchings/ovl_0b_2", func_800E9EC4);

INCLUDE_ASM("asm/us/ovl_0b/nonmatchings/ovl_0b_2", func_800E9F3C);

INCLUDE_ASM("asm/us/ovl_0b/nonmatchings/ovl_0b_2", func_800E9F68);

INCLUDE_ASM("asm/us/ovl_0b/nonmatchings/ovl_0b_2", func_800E9FCC);

s32 func_800E9FF4(void) {
    return D_80104E68.unk0 != 0;
}

s32 func_800EA004(void) {
    return D_80104F88->unk12B4 == 3;
}

u8 func_800EA024(void) {
    return D_80104E68.unk4;
}

INCLUDE_ASM("asm/us/ovl_0b/nonmatchings/ovl_0b_2", func_800EA038);

s32 func_800EA094(void) {
    return func_800F1360();
}

INCLUDE_ASM("asm/us/ovl_0b/nonmatchings/ovl_0b_2", func_800EA0B4);

INCLUDE_ASM("asm/us/ovl_0b/nonmatchings/ovl_0b_2", func_800EA0F8);

void func_800EA124(s32 arg0) {
    func_800EC170(arg0);
}

void func_800EA144(s32 arg0) {
    func_800EC14C(arg0);
}

INCLUDE_ASM("asm/us/ovl_0b/nonmatchings/ovl_0b_2", func_800EA164);

void func_800EA1B0(void) {
    func_800EC070();
}

INCLUDE_ASM("asm/us/ovl_0b/nonmatchings/ovl_0b_2", func_800EA1D0);

INCLUDE_ASM("asm/us/ovl_0b/nonmatchings/ovl_0b_2", func_800EA1F8);

INCLUDE_ASM("asm/us/ovl_0b/nonmatchings/ovl_0b_2", func_800EA230);

u8 func_800EA268(void) {
    return D_80104E68.unk15;
}

u8 func_800EA27C(void) {
    return D_80104E68.unkE;
}

INCLUDE_ASM("asm/us/ovl_0b/nonmatchings/ovl_0b_2", func_800EA290);

INCLUDE_ASM("asm/us/ovl_0b/nonmatchings/ovl_0b_2", func_800EA30C);

void func_800EA338(s8 arg0) {
    D_80104E68.unkB = arg0;
}

INCLUDE_ASM("asm/us/ovl_0b/nonmatchings/ovl_0b_2", func_800EA34C);

INCLUDE_ASM("asm/us/ovl_0b/nonmatchings/ovl_0b_2", func_800EA378);

INCLUDE_ASM("asm/us/ovl_0b/nonmatchings/ovl_0b_2", func_800EA400);

INCLUDE_ASM("asm/us/ovl_0b/nonmatchings/ovl_0b_2", func_800EA440);

INCLUDE_ASM("asm/us/ovl_0b/nonmatchings/ovl_0b_2", func_800EA498);

INCLUDE_ASM("asm/us/ovl_0b/nonmatchings/ovl_0b_2", func_800EA4D0);

INCLUDE_ASM("asm/us/ovl_0b/nonmatchings/ovl_0b_2", func_800EA518);

void func_800EA548(void) {
}

INCLUDE_ASM("asm/us/ovl_0b/nonmatchings/ovl_0b_2", func_800EA550);

INCLUDE_ASM("asm/us/ovl_0b/nonmatchings/ovl_0b_2", func_800EA5C4);

INCLUDE_ASM("asm/us/ovl_0b/nonmatchings/ovl_0b_2", func_800EA650);

INCLUDE_ASM("asm/us/ovl_0b/nonmatchings/ovl_0b_2", func_800EA7B8);

INCLUDE_ASM("asm/us/ovl_0b/nonmatchings/ovl_0b_2", func_800EA908);

INCLUDE_ASM("asm/us/ovl_0b/nonmatchings/ovl_0b_2", func_800EAAFC);

INCLUDE_ASM("asm/us/ovl_0b/nonmatchings/ovl_0b_2", func_800EAC88);

INCLUDE_ASM("asm/us/ovl_0b/nonmatchings/ovl_0b_2", func_800EAD94);

INCLUDE_ASM("asm/us/ovl_0b/nonmatchings/ovl_0b_2", func_800EAE8C);

INCLUDE_ASM("asm/us/ovl_0b/nonmatchings/ovl_0b_2", func_800EB05C);

INCLUDE_ASM("asm/us/ovl_0b/nonmatchings/ovl_0b_2", func_800EB114);

INCLUDE_ASM("asm/us/ovl_0b/nonmatchings/ovl_0b_2", func_800EB1CC);

INCLUDE_ASM("asm/us/ovl_0b/nonmatchings/ovl_0b_2", func_800EB238);

u8 func_800EB2A4(void) {
    return D_80104F88->unk12B4;
}

s32 func_800EB2BC(s32 arg0) {
    s32 old = D_80104F88->unk12B4;
    if (arg0 != 5) {
        D_80104F88->unk12B4 = arg0;
    }
    return old;
}

void func_800EB2E0(void) {
    Unk80104F88 *p = D_80104F88;
    p->unk12C6 = 0;
    p->unk12C5 = 0;
    p->unk12C4 = 0;
}

INCLUDE_ASM("asm/us/ovl_0b/nonmatchings/ovl_0b_2", func_800EB2FC);

INCLUDE_ASM("asm/us/ovl_0b/nonmatchings/ovl_0b_2", func_800EB358);

INCLUDE_ASM("asm/us/ovl_0b/nonmatchings/ovl_0b_2", func_800EB3A0);

INCLUDE_ASM("asm/us/ovl_0b/nonmatchings/ovl_0b_2", func_800EB410);

INCLUDE_ASM("asm/us/ovl_0b/nonmatchings/ovl_0b_2", func_800EB46C);

INCLUDE_ASM("asm/us/ovl_0b/nonmatchings/ovl_0b_2", func_800EB518);

INCLUDE_ASM("asm/us/ovl_0b/nonmatchings/ovl_0b_2", func_800EB54C);

void func_800EB578(void) {
}

u8 *func_800EB580(void) {
    return &D_80104F88->unk12C4;
}

s32 func_800EB590(s32 arg0) {
    return D_80104F88->unk12C8[arg0];
}

INCLUDE_ASM("asm/us/ovl_0b/nonmatchings/ovl_0b_2", func_800EB5AC);

INCLUDE_ASM("asm/us/ovl_0b/nonmatchings/ovl_0b_2", func_800EB600);

INCLUDE_ASM("asm/us/ovl_0b/nonmatchings/ovl_0b_2", func_800EB63C);

INCLUDE_ASM("asm/us/ovl_0b/nonmatchings/ovl_0b_2", func_800EB6FC);

INCLUDE_ASM("asm/us/ovl_0b/nonmatchings/ovl_0b_2", func_800EB7B0);

INCLUDE_ASM("asm/us/ovl_0b/nonmatchings/ovl_0b_2", func_800EB81C);

INCLUDE_ASM("asm/us/ovl_0b/nonmatchings/ovl_0b_2", func_800EB924);

INCLUDE_ASM("asm/us/ovl_0b/nonmatchings/ovl_0b_2", func_800EB9E0);

void func_800EBA10(void) {
    D_80104F88->unk12B4 = 0;
}

INCLUDE_ASM("asm/us/ovl_0b/nonmatchings/ovl_0b_2", func_800EBA20);

INCLUDE_ASM("asm/us/ovl_0b/nonmatchings/ovl_0b_2", func_800EBB34);

INCLUDE_ASM("asm/us/ovl_0b/nonmatchings/ovl_0b_2", func_800EBB78);

INCLUDE_ASM("asm/us/ovl_0b/nonmatchings/ovl_0b_2", func_800EBBB4);

INCLUDE_ASM("asm/us/ovl_0b/nonmatchings/ovl_0b_2", func_800EBC30);

u8 func_800EBCB4(void) {
    return D_80104F90;
}

INCLUDE_ASM("asm/us/ovl_0b/nonmatchings/ovl_0b_2", func_800EBCC4);

INCLUDE_ASM("asm/us/ovl_0b/nonmatchings/ovl_0b_2", func_800EBD58);

INCLUDE_ASM("asm/us/ovl_0b/nonmatchings/ovl_0b_2", func_800EBD88);

INCLUDE_ASM("asm/us/ovl_0b/nonmatchings/ovl_0b_2", func_800EBE1C);

INCLUDE_ASM("asm/us/ovl_0b/nonmatchings/ovl_0b_2", func_800EBEFC);

INCLUDE_ASM("asm/us/ovl_0b/nonmatchings/ovl_0b_2", func_800EC008);

INCLUDE_ASM("asm/us/ovl_0b/nonmatchings/ovl_0b_2", func_800EC048);

INCLUDE_ASM("asm/us/ovl_0b/nonmatchings/ovl_0b_2", func_800EC070);

s32 func_800EC114(s32 arg0) {
    return D_80104FA8->unk4[arg0];
}

s32 func_800EC130(s32 arg0) {
    return D_80104FA8->unk80[arg0];
}

void func_800EC14C(s32 arg0) {
    D_80104FA8->unk1E0[arg0]++;
}

void func_800EC170(s32 arg0) {
    D_80104FA8->unk1E0[arg0]--;
}

u8 func_800EC194(s32 arg0) {
    return D_80104FA8->unk1E0[arg0];
}

INCLUDE_ASM("asm/us/ovl_0b/nonmatchings/ovl_0b_2", func_800EC1B0);

INCLUDE_ASM("asm/us/ovl_0b/nonmatchings/ovl_0b_2", func_800EC258);

INCLUDE_ASM("asm/us/ovl_0b/nonmatchings/ovl_0b_2", func_800EC334);

INCLUDE_ASM("asm/us/ovl_0b/nonmatchings/ovl_0b_2", func_800EC3C0);

INCLUDE_ASM("asm/us/ovl_0b/nonmatchings/ovl_0b_2", func_800EC44C);

INCLUDE_ASM("asm/us/ovl_0b/nonmatchings/ovl_0b_2", func_800EC49C);

INCLUDE_ASM("asm/us/ovl_0b/nonmatchings/ovl_0b_2", func_800EC4EC);

INCLUDE_ASM("asm/us/ovl_0b/nonmatchings/ovl_0b_2", func_800EC628);

INCLUDE_RODATA("asm/us/ovl_0b/nonmatchings/ovl_0b_2", D_800A83AC);

INCLUDE_ASM("asm/us/ovl_0b/nonmatchings/ovl_0b_2", func_800EC730);

INCLUDE_ASM("asm/us/ovl_0b/nonmatchings/ovl_0b_2", func_800EC928);

INCLUDE_ASM("asm/us/ovl_0b/nonmatchings/ovl_0b_2", func_800ECA50);

INCLUDE_ASM("asm/us/ovl_0b/nonmatchings/ovl_0b_2", func_800ECB5C);

INCLUDE_ASM("asm/us/ovl_0b/nonmatchings/ovl_0b_2", func_800ECCA8);

INCLUDE_ASM("asm/us/ovl_0b/nonmatchings/ovl_0b_2", func_800ECDB8);

INCLUDE_ASM("asm/us/ovl_0b/nonmatchings/ovl_0b_2", func_800ECF0C);

INCLUDE_ASM("asm/us/ovl_0b/nonmatchings/ovl_0b_2", func_800ED02C);

INCLUDE_ASM("asm/us/ovl_0b/nonmatchings/ovl_0b_2", func_800ED3F8);

INCLUDE_ASM("asm/us/ovl_0b/nonmatchings/ovl_0b_2", func_800ED7C0);

INCLUDE_ASM("asm/us/ovl_0b/nonmatchings/ovl_0b_2", func_800EDAD4);

void func_800EDB68(void) {
    func_800ECB5C();
}

INCLUDE_ASM("asm/us/ovl_0b/nonmatchings/ovl_0b_2", func_800EDB88);

INCLUDE_ASM("asm/us/ovl_0b/nonmatchings/ovl_0b_2", func_800EDC3C);

INCLUDE_ASM("asm/us/ovl_0b/nonmatchings/ovl_0b_2", func_800EDCA0);

INCLUDE_ASM("asm/us/ovl_0b/nonmatchings/ovl_0b_2", func_800EDD1C);

INCLUDE_ASM("asm/us/ovl_0b/nonmatchings/ovl_0b_2", func_800EDD9C);

INCLUDE_ASM("asm/us/ovl_0b/nonmatchings/ovl_0b_2", func_800EDDF8);

INCLUDE_ASM("asm/us/ovl_0b/nonmatchings/ovl_0b_2", func_800EDE6C);

INCLUDE_ASM("asm/us/ovl_0b/nonmatchings/ovl_0b_2", func_800EDF34);

INCLUDE_ASM("asm/us/ovl_0b/nonmatchings/ovl_0b_2", func_800EDF90);

INCLUDE_ASM("asm/us/ovl_0b/nonmatchings/ovl_0b_2", func_800EDFEC);

INCLUDE_ASM("asm/us/ovl_0b/nonmatchings/ovl_0b_2", func_800EE048);

void func_800EE0FC(void) {
    func_800EB2BC(0);
    D_80104FC0 = 0;
}

INCLUDE_ASM("asm/us/ovl_0b/nonmatchings/ovl_0b_2", func_800EE120);

INCLUDE_ASM("asm/us/ovl_0b/nonmatchings/ovl_0b_2", func_800EE154);

INCLUDE_ASM("asm/us/ovl_0b/nonmatchings/ovl_0b_2", func_800EE24C);

INCLUDE_ASM("asm/us/ovl_0b/nonmatchings/ovl_0b_2", func_800EE31C);

INCLUDE_ASM("asm/us/ovl_0b/nonmatchings/ovl_0b_2", func_800EE37C);

INCLUDE_ASM("asm/us/ovl_0b/nonmatchings/ovl_0b_2", func_800EE54C);

INCLUDE_ASM("asm/us/ovl_0b/nonmatchings/ovl_0b_2", func_800EE680);

INCLUDE_ASM("asm/us/ovl_0b/nonmatchings/ovl_0b_2", func_800EE7F8);

INCLUDE_ASM("asm/us/ovl_0b/nonmatchings/ovl_0b_2", func_800EE938);

INCLUDE_ASM("asm/us/ovl_0b/nonmatchings/ovl_0b_2", func_800EEA98);

INCLUDE_ASM("asm/us/ovl_0b/nonmatchings/ovl_0b_2", func_800EEBB8);

INCLUDE_ASM("asm/us/ovl_0b/nonmatchings/ovl_0b_2", func_800EED30);

void func_800EEDD4(void) {
    D_80104FD0.unk10++;
}

INCLUDE_ASM("asm/us/ovl_0b/nonmatchings/ovl_0b_2", func_800EEDF0);

INCLUDE_ASM("asm/us/ovl_0b/nonmatchings/ovl_0b_2", func_800EEE3C);

INCLUDE_ASM("asm/us/ovl_0b/nonmatchings/ovl_0b_2", func_800EEED4);

INCLUDE_ASM("asm/us/ovl_0b/nonmatchings/ovl_0b_2", func_800EEFA8);

INCLUDE_ASM("asm/us/ovl_0b/nonmatchings/ovl_0b_2", func_800EF00C);

INCLUDE_ASM("asm/us/ovl_0b/nonmatchings/ovl_0b_2", func_800EF044);

INCLUDE_ASM("asm/us/ovl_0b/nonmatchings/ovl_0b_2", func_800EF094);

INCLUDE_ASM("asm/us/ovl_0b/nonmatchings/ovl_0b_2", func_800EF0D8);

INCLUDE_ASM("asm/us/ovl_0b/nonmatchings/ovl_0b_2", func_800EF228);

INCLUDE_ASM("asm/us/ovl_0b/nonmatchings/ovl_0b_2", func_800EF390);

INCLUDE_ASM("asm/us/ovl_0b/nonmatchings/ovl_0b_2", func_800EF4D8);

INCLUDE_ASM("asm/us/ovl_0b/nonmatchings/ovl_0b_2", func_800EF598);

INCLUDE_ASM("asm/us/ovl_0b/nonmatchings/ovl_0b_2", func_800EF5E0);

INCLUDE_ASM("asm/us/ovl_0b/nonmatchings/ovl_0b_2", func_800EF674);

INCLUDE_ASM("asm/us/ovl_0b/nonmatchings/ovl_0b_2", func_800EF724);

INCLUDE_ASM("asm/us/ovl_0b/nonmatchings/ovl_0b_2", func_800EF7D4);

INCLUDE_ASM("asm/us/ovl_0b/nonmatchings/ovl_0b_2", func_800EF80C);

INCLUDE_ASM("asm/us/ovl_0b/nonmatchings/ovl_0b_2", func_800EFA10);

INCLUDE_ASM("asm/us/ovl_0b/nonmatchings/ovl_0b_2", func_800EFB3C);

INCLUDE_ASM("asm/us/ovl_0b/nonmatchings/ovl_0b_2", func_800EFD1C);

INCLUDE_ASM("asm/us/ovl_0b/nonmatchings/ovl_0b_2", func_800EFF14);

INCLUDE_ASM("asm/us/ovl_0b/nonmatchings/ovl_0b_2", func_800F0118);

INCLUDE_ASM("asm/us/ovl_0b/nonmatchings/ovl_0b_2", func_800F028C);

INCLUDE_ASM("asm/us/ovl_0b/nonmatchings/ovl_0b_2", func_800F0444);

INCLUDE_ASM("asm/us/ovl_0b/nonmatchings/ovl_0b_2", func_800F0620);

INCLUDE_ASM("asm/us/ovl_0b/nonmatchings/ovl_0b_2", func_800F07F0);

INCLUDE_ASM("asm/us/ovl_0b/nonmatchings/ovl_0b_2", func_800F0904);

INCLUDE_ASM("asm/us/ovl_0b/nonmatchings/ovl_0b_2", func_800F0A54);

INCLUDE_ASM("asm/us/ovl_0b/nonmatchings/ovl_0b_2", func_800F0BE0);

INCLUDE_ASM("asm/us/ovl_0b/nonmatchings/ovl_0b_2", func_800F0DF8);

INCLUDE_ASM("asm/us/ovl_0b/nonmatchings/ovl_0b_2", func_800F0F60);

INCLUDE_ASM("asm/us/ovl_0b/nonmatchings/ovl_0b_2", func_800F1150);

INCLUDE_ASM("asm/us/ovl_0b/nonmatchings/ovl_0b_2", func_800F11A4);

INCLUDE_ASM("asm/us/ovl_0b/nonmatchings/ovl_0b_2", func_800F11FC);

INCLUDE_ASM("asm/us/ovl_0b/nonmatchings/ovl_0b_2", func_800F1300);

u8 func_800F1360(void) {
    return D_80104FFC;
}

INCLUDE_ASM("asm/us/ovl_0b/nonmatchings/ovl_0b_2", func_800F1370);

INCLUDE_ASM("asm/us/ovl_0b/nonmatchings/ovl_0b_2", func_800F1388);

u8 func_800F13BC(void) {
    return D_80104FF0;
}

INCLUDE_ASM("asm/us/ovl_0b/nonmatchings/ovl_0b_2", func_800F13CC);

INCLUDE_ASM("asm/us/ovl_0b/nonmatchings/ovl_0b_2", func_800F142C);

INCLUDE_ASM("asm/us/ovl_0b/nonmatchings/ovl_0b_2", func_800F1484);

u8 *func_800F1548(void) {
    return D_80105020;
}

INCLUDE_ASM("asm/us/ovl_0b/nonmatchings/ovl_0b_2", func_800F1554);

INCLUDE_ASM("asm/us/ovl_0b/nonmatchings/ovl_0b_2", func_800F160C);

INCLUDE_ASM("asm/us/ovl_0b/nonmatchings/ovl_0b_2", func_800F1640);

INCLUDE_ASM("asm/us/ovl_0b/nonmatchings/ovl_0b_2", func_800F1674);

INCLUDE_ASM("asm/us/ovl_0b/nonmatchings/ovl_0b_2", func_800F16A8);

INCLUDE_ASM("asm/us/ovl_0b/nonmatchings/ovl_0b_2", func_800F16DC);

INCLUDE_ASM("asm/us/ovl_0b/nonmatchings/ovl_0b_2", func_800F1790);

INCLUDE_ASM("asm/us/ovl_0b/nonmatchings/ovl_0b_2", func_800F18F0);

INCLUDE_ASM("asm/us/ovl_0b/nonmatchings/ovl_0b_2", func_800F1970);

INCLUDE_ASM("asm/us/ovl_0b/nonmatchings/ovl_0b_2", func_800F1A20);

INCLUDE_ASM("asm/us/ovl_0b/nonmatchings/ovl_0b_2", func_800F1A8C);

INCLUDE_ASM("asm/us/ovl_0b/nonmatchings/ovl_0b_2", func_800F1AF8);

INCLUDE_ASM("asm/us/ovl_0b/nonmatchings/ovl_0b_2", func_800F1B78);

INCLUDE_ASM("asm/us/ovl_0b/nonmatchings/ovl_0b_2", func_800F1BFC);

INCLUDE_ASM("asm/us/ovl_0b/nonmatchings/ovl_0b_2", func_800F1E84);

INCLUDE_ASM("asm/us/ovl_0b/nonmatchings/ovl_0b_2", func_800F1FBC);

INCLUDE_ASM("asm/us/ovl_0b/nonmatchings/ovl_0b_2", func_800F2118);

INCLUDE_ASM("asm/us/ovl_0b/nonmatchings/ovl_0b_2", func_800F224C);

INCLUDE_ASM("asm/us/ovl_0b/nonmatchings/ovl_0b_2", func_800F2374);

INCLUDE_ASM("asm/us/ovl_0b/nonmatchings/ovl_0b_2", func_800F2560);

INCLUDE_ASM("asm/us/ovl_0b/nonmatchings/ovl_0b_2", func_800F2684);

INCLUDE_ASM("asm/us/ovl_0b/nonmatchings/ovl_0b_2", func_800F27CC);

INCLUDE_ASM("asm/us/ovl_0b/nonmatchings/ovl_0b_2", func_800F2890);

INCLUDE_ASM("asm/us/ovl_0b/nonmatchings/ovl_0b_2", func_800F28F0);

u8 func_800F29A4(void) {
    return D_80105058;
}

INCLUDE_ASM("asm/us/ovl_0b/nonmatchings/ovl_0b_2", func_800F29B4);

INCLUDE_ASM("asm/us/ovl_0b/nonmatchings/ovl_0b_2", func_800F2A0C);

void func_800F2A58(void) {
}

INCLUDE_ASM("asm/us/ovl_0b/nonmatchings/ovl_0b_2", func_800F2A60);

INCLUDE_ASM("asm/us/ovl_0b/nonmatchings/ovl_0b_2", func_800F2AAC);

INCLUDE_ASM("asm/us/ovl_0b/nonmatchings/ovl_0b_2", func_800F2C74);

INCLUDE_ASM("asm/us/ovl_0b/nonmatchings/ovl_0b_2", func_800F2DC0);

INCLUDE_ASM("asm/us/ovl_0b/nonmatchings/ovl_0b_2", func_800F2ED0);

INCLUDE_ASM("asm/us/ovl_0b/nonmatchings/ovl_0b_2", func_800F3060);

INCLUDE_ASM("asm/us/ovl_0b/nonmatchings/ovl_0b_2", func_800F316C);

INCLUDE_ASM("asm/us/ovl_0b/nonmatchings/ovl_0b_2", func_800F3390);

INCLUDE_ASM("asm/us/ovl_0b/nonmatchings/ovl_0b_2", func_800F34C0);

INCLUDE_ASM("asm/us/ovl_0b/nonmatchings/ovl_0b_2", func_800F35C8);

INCLUDE_ASM("asm/us/ovl_0b/nonmatchings/ovl_0b_2", func_800F36F8);

INCLUDE_ASM("asm/us/ovl_0b/nonmatchings/ovl_0b_2", func_800F386C);

INCLUDE_ASM("asm/us/ovl_0b/nonmatchings/ovl_0b_2", func_800F3A8C);

INCLUDE_ASM("asm/us/ovl_0b/nonmatchings/ovl_0b_2", func_800F3C08);

INCLUDE_ASM("asm/us/ovl_0b/nonmatchings/ovl_0b_2", func_800F3D0C);

INCLUDE_ASM("asm/us/ovl_0b/nonmatchings/ovl_0b_2", func_800F3D3C);

INCLUDE_ASM("asm/us/ovl_0b/nonmatchings/ovl_0b_2", func_800F3DE0);

INCLUDE_ASM("asm/us/ovl_0b/nonmatchings/ovl_0b_2", func_800F3E60);

u8 *func_800F3EA4(void) {
    return D_80105078;
}

u8 func_800F3EB4(void) {
    return *D_80105078;
}

void func_800F3ECC(u8 arg0) {
    *D_80105078 = arg0;
}

INCLUDE_ASM("asm/us/ovl_0b/nonmatchings/ovl_0b_2", func_800F3EDC);

INCLUDE_ASM("asm/us/ovl_0b/nonmatchings/ovl_0b_2", func_800F3F10);

INCLUDE_ASM("asm/us/ovl_0b/nonmatchings/ovl_0b_2", func_800F3FDC);

INCLUDE_ASM("asm/us/ovl_0b/nonmatchings/ovl_0b_2", func_800F4010);

INCLUDE_ASM("asm/us/ovl_0b/nonmatchings/ovl_0b_2", func_800F4060);

INCLUDE_ASM("asm/us/ovl_0b/nonmatchings/ovl_0b_2", func_800F40D0);

INCLUDE_ASM("asm/us/ovl_0b/nonmatchings/ovl_0b_2", func_800F412C);

INCLUDE_ASM("asm/us/ovl_0b/nonmatchings/ovl_0b_2", func_800F4140);

INCLUDE_ASM("asm/us/ovl_0b/nonmatchings/ovl_0b_2", func_800F4180);

INCLUDE_ASM("asm/us/ovl_0b/nonmatchings/ovl_0b_2", func_800F41B8);

INCLUDE_ASM("asm/us/ovl_0b/nonmatchings/ovl_0b_2", func_800F422C);

INCLUDE_ASM("asm/us/ovl_0b/nonmatchings/ovl_0b_2", func_800F42F0);

INCLUDE_ASM("asm/us/ovl_0b/nonmatchings/ovl_0b_2", func_800F4384);

INCLUDE_ASM("asm/us/ovl_0b/nonmatchings/ovl_0b_2", func_800F4630);

INCLUDE_ASM("asm/us/ovl_0b/nonmatchings/ovl_0b_2", func_800F4700);

INCLUDE_ASM("asm/us/ovl_0b/nonmatchings/ovl_0b_2", func_800F4778);

INCLUDE_ASM("asm/us/ovl_0b/nonmatchings/ovl_0b_2", func_800F47D8);

INCLUDE_ASM("asm/us/ovl_0b/nonmatchings/ovl_0b_2", func_800F4884);

void func_800F48DC(void) {
}

INCLUDE_ASM("asm/us/ovl_0b/nonmatchings/ovl_0b_2", func_800F48E4);

INCLUDE_ASM("asm/us/ovl_0b/nonmatchings/ovl_0b_2", func_800F4A00);

void func_800F4D20(void) {
    DRAWENV *env = D_8006794C->unk10;
    env[1].dfe = 0;
    env[0].dfe = 0;
}

INCLUDE_ASM("asm/us/ovl_0b/nonmatchings/ovl_0b_2", func_800F4D40);

INCLUDE_ASM("asm/us/ovl_0b/nonmatchings/ovl_0b_2", func_800F4D80);

void func_800F4DF4(void) {
}

INCLUDE_ASM("asm/us/ovl_0b/nonmatchings/ovl_0b_2", func_800F4DFC);

void func_800F4E68(void) {
}

INCLUDE_ASM("asm/us/ovl_0b/nonmatchings/ovl_0b_2", func_800F4E70);

INCLUDE_ASM("asm/us/ovl_0b/nonmatchings/ovl_0b_2", func_800F4F04);

INCLUDE_ASM("asm/us/ovl_0b/nonmatchings/ovl_0b_2", func_800F4F44);

INCLUDE_ASM("asm/us/ovl_0b/nonmatchings/ovl_0b_2", func_800F4F84);

s32 *func_800F4FB4(s32 arg0) {
    return &D_8010528C[arg0];
}

s32 func_800F4FC8(void) {
    return D_80105274;
}

u8 func_800F4FD8(void) {
    return D_8010527C;
}

INCLUDE_ASM("asm/us/ovl_0b/nonmatchings/ovl_0b_2", func_800F4FE8);

INCLUDE_ASM("asm/us/ovl_0b/nonmatchings/ovl_0b_2", func_800F5064);

INCLUDE_ASM("asm/us/ovl_0b/nonmatchings/ovl_0b_2", func_800F511C);

INCLUDE_RODATA("asm/us/ovl_0b/nonmatchings/ovl_0b_2", D_800A8584);

INCLUDE_RODATA("asm/us/ovl_0b/nonmatchings/ovl_0b_2", D_800A858C);

INCLUDE_RODATA("asm/us/ovl_0b/nonmatchings/ovl_0b_2", D_800A8594);
