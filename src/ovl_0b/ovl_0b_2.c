#include "ovl_0b.h"

s32 func_800CFC0C(s32 arg0) {
    s32 v = arg0 & ~3;
    if (arg0 & 3) {
        v += 4;
    }
    return v;
}

s32 func_800CFC2C(s32 arg0, s32 arg1, s32 arg2, s32 arg3, s32 arg4, s32 *arg5) {
    if (arg0 >= arg1 && arg1 + arg2 >= arg0) {
        *arg5 = (arg0 - arg1) * arg4 / arg2;
        if (arg3 < 0) {
            *arg5 = arg4 - *arg5;
        }
        return 1;
    }
    return 0;
}

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

s32 func_800D72BC(s32 arg0, s16 *arg1, s32 arg2, s32 arg3, s16 *arg4, s32 arg5) {
    s16 sp10[4];
    s32 t;
    if (arg2 == 0) {
        switch (arg5) {
        case 0:
            func_800E73EC(arg0, arg4);
            break;
        case 1:
            func_800E750C(arg0, arg4);
            break;
        default:
            return 1;
        }
    }
    t = ((arg2 + 1) << 12) / arg3;
    if (t > 0x1000) {
        t = 0x1000;
    }
    func_800CFA00(arg4, arg1, t, sp10);
    func_800E76DC(0x10, sp10);
    return arg2 >= arg3 - 1;
}

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

s32 func_800D7660(s32 arg0, Unk800D8EA4 *arg1, s32 arg2, s32 *arg3, s32 *arg4) {
    if (arg0 == 0) {
        *arg4 = 8;
    } else if (arg0 != 1) {
        return func_800D8D94(arg1, arg2, arg3);
    } else {
        arg1->unk0 = arg4[0];
        arg1->unk1 = arg4[1];
        D_800FEC54 = 0;
    }
    return 0;
}

s32 func_800D76DC(s32 arg0, Unk800D8EA4 *arg1, s32 arg2, s32 *arg3, s32 *arg4) {
    if (arg0 == 0) {
        *arg4 = 8;
    } else if (arg0 != 1) {
        return func_800D8E10(arg1, arg2, arg3);
    } else {
        arg1->unk0 = arg4[0];
        arg1->unk1 = arg4[1];
        arg1->unk2 = D_80100D88.unk0;
        D_800FEC54 = 0;
    }
    return 0;
}

s32 func_800D7768(s32 arg0, Unk800D8EA4 *arg1, s32 arg2, s32 *arg3, s32 *arg4) {
    if (arg0 == 0) {
        *arg4 = 8;
    } else if (arg0 != 1) {
        return func_800D8EA4(arg1, arg2, arg3);
    } else {
        arg1->unk0 = arg4[0];
        arg1->unk1 = arg4[1];
        D_800FEC54 = 0;
    }
    return 0;
}

s32 func_800D77E4(s32 arg0, Unk800D8EA4 *arg1, s32 arg2, s32 *arg3, s32 *arg4) {
    if (arg0 == 0) {
        *arg4 = 8;
    } else if (arg0 != 1) {
        return func_800D8F1C(arg1, arg2, arg3);
    } else {
        arg1->unk0 = arg4[0];
        arg1->unk1 = arg4[1];
        arg1->unk2 = D_80100D88.unk0;
        D_800FEC54 = 0;
    }
    return 0;
}

s32 func_800D7870(s32 arg0, Unk800D8EA4 *arg1, s32 arg2, s32 *arg3, s32 *arg4) {
    if (arg0 == 0) {
        *arg4 = 8;
    } else if (arg0 != 1) {
        return func_800D8FAC(arg1, arg2, arg3);
    } else {
        arg1->unk0 = arg4[0];
        arg1->unk1 = arg4[1];
        D_800FEC54 = 0;
    }
    return 0;
}

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

s32 func_800D79BC(s32 arg0, Unk800D8EA4 *arg1, s32 arg2, s32 *arg3, s32 *arg4) {
    if (arg0 == 0) {
        *arg4 = 8;
    } else if (arg0 != 1) {
        return func_800D908C(arg1, arg2, arg3);
    } else {
        arg1->unk0 = arg4[0];
        arg1->unk1 = arg4[1];
        D_800FEC54 = 0;
    }
    return 0;
}

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

s32 func_800D7B2C(s32 arg0, u8 *arg1, s32 arg2, s32 arg3, s32 *arg4) {
    if (arg0 == 0) {
        *arg4 = 0x10;
    } else if (arg0 != 1) {
        return func_800D9260(arg1, arg2, arg3);
    } else {
        arg1[0] = arg4[0];
        arg1[1] = arg4[1];
        D_800FEC55 = 0;
    }
    return 0;
}

INCLUDE_ASM("asm/us/ovl_0b/nonmatchings/ovl_0b_2", func_800D7BA8);

INCLUDE_ASM("asm/us/ovl_0b/nonmatchings/ovl_0b_2", func_800D7C9C);

s32 func_800D7D64(s32 arg0, Unk800D955C *arg1, s32 arg2, s32 *arg3, s32 *arg4) {
    s32 v;
    if (arg0 == 0) {
        *arg4 = 0x18;
    } else if (arg0 == 1) {
        arg1->unk0 = 0x10;
        arg1->unk1 = arg4[0];
        arg1->unk2 = arg4[1];
        arg1->unk3 = 1;
        if ((v = arg1->unk2) == arg0 && func_800D8A64(arg1->unk1) != 0) {
            D_800FEC55 = v;
            return 1;
        }
        D_800FEC55 = 0;
    } else {
        return func_800D955C(arg1, arg2, arg3);
    }
    return 0;
}

INCLUDE_ASM("asm/us/ovl_0b/nonmatchings/ovl_0b_2", func_800D7E14);

INCLUDE_ASM("asm/us/ovl_0b/nonmatchings/ovl_0b_2", func_800D7EA0);

INCLUDE_ASM("asm/us/ovl_0b/nonmatchings/ovl_0b_2", func_800D7F80);

INCLUDE_ASM("asm/us/ovl_0b/nonmatchings/ovl_0b_2", func_800D805C);

INCLUDE_ASM("asm/us/ovl_0b/nonmatchings/ovl_0b_2", func_800D8124);

s32 func_800D81EC(s32 arg0, u8 *arg1, s32 arg2, s32 arg3, s32 *arg4) {
    if (arg0 == 0) {
        *arg4 = 4;
    } else if (arg0 != 1) {
        return func_800D9F5C(arg1, arg2, arg3);
    } else {
        arg1[0] = arg4[0];
        arg1[1] = arg4[1];
        arg1[2] = D_80100D88.unk0;
    }
    return 0;
}

INCLUDE_ASM("asm/us/ovl_0b/nonmatchings/ovl_0b_2", func_800D8270);

INCLUDE_ASM("asm/us/ovl_0b/nonmatchings/ovl_0b_2", func_800D8368);

s32 func_800D8414(s32 arg0, u8 *arg1, s32 arg2, s32 *arg3, s32 *arg4) {
    s32 v;
    u8 a, b;
    if (arg0 == 0) {
        *arg4 = 4;
    } else if (arg0 == 1) {
        arg1[0] = arg4[0];
        arg1[1] = arg4[1];
        arg1[2] = D_80100D88.unk0;
        v = func_800D8A94(arg1[0], arg1[1]);
        if (v == arg0) {
            D_800FEC57 = v;
            return 1;
        }
        D_800FEC57 = 0;
    } else {
        return func_800DA004(arg1, arg2, arg3);
    }
    return 0;
}

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

s32 func_800D8CD8(u8 *arg0, s32 arg1, s32 arg2) {
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

s32 func_800D8D94(Unk800D8EA4 *arg0, s32 arg1, s32 *arg2) {
    s32 v = *arg2;
    if (v == 0) {
        func_800D85AC(arg0->unk0, arg0->unk1);
        arg0->unk4 = func_800D87D0(arg0->unk0) - 1;
    }
    if (v >= arg0->unk4) {
        D_800FEC54 = 1;
        return 1;
    }
    return 0;
}

s32 func_800D8E10(Unk800D8EA4 *arg0, s32 arg1, s32 *arg2) {
    s32 v = *arg2;
    if (v == 0) {
        func_800D85AC(arg0->unk0, arg0->unk1);
        arg0->unk4 = func_800D87D0(arg0->unk0) - arg0->unk2 - 1;
    }
    if (v >= arg0->unk4) {
        D_800FEC54 = 1;
        func_800D89E8(arg0->unk0);
        return 1;
    }
    return 0;
}

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

s32 func_800D8F1C(Unk800D8EA4 *arg0, s32 arg1, s32 *arg2) {
    s32 v = *arg2;
    if (v == 0) {
        func_800D85AC(arg0->unk0, 0x17);
        arg0->unk4 = func_800D87D0(arg0->unk0) - arg0->unk2 - 1;
    }
    if (v >= arg0->unk4) {
        func_800D89E8(arg0->unk0);
        D_800FEC54 = 1;
        return 1;
    }
    return 0;
}

s32 func_800D8FAC(Unk800D8EA4 *arg0, s32 arg1, s32 *arg2) {
    s32 v = *arg2;
    if (func_800D8A64(arg0->unk1) == 0) {
        if (v == 0) {
            func_800D85AC(arg0->unk0, 0x19);
            arg0->unk4 = func_800D87D0(arg0->unk0) - 1;
        }
        if (v < arg0->unk4) {
            return 0;
        }
    }
    D_800FEC54 = 1;
    return 1;
}

s32 func_800D9038(u8 *arg0, s32 arg1, s32 arg2) {
    func_800D8658(*arg0, 1);
    return 1;
}

s32 func_800D905C(u8 *arg0, s32 arg1, Unk800F78A40 *arg2) {
    func_800D85F8(arg2, arg0[0], arg0[1]);
    return 1;
}

s32 func_800D908C(Unk800D8EA4 *arg0, s32 arg1, s32 *arg2) {
    s32 v = *arg2;
    if (v == 0) {
        func_800D85F8((Unk800F78A40 *)arg2, arg0->unk0, arg0->unk1);
        arg0->unk4 = func_800D87D0(arg0->unk0) - 1;
    }
    if (v >= arg0->unk4) {
        D_800FEC54 = 1;
        return 1;
    }
    return 0;
}

s32 func_800D9110(u8 *arg0, s32 arg1, s32 arg2) {
    func_800D8A20(*arg0, 0);
    return 1;
}

INCLUDE_ASM("asm/us/ovl_0b/nonmatchings/ovl_0b_2", func_800D9134);

INCLUDE_ASM("asm/us/ovl_0b/nonmatchings/ovl_0b_2", func_800D9260);

INCLUDE_ASM("asm/us/ovl_0b/nonmatchings/ovl_0b_2", func_800D9350);

INCLUDE_ASM("asm/us/ovl_0b/nonmatchings/ovl_0b_2", func_800D9488);

s32 func_800D955C(Unk800D955C *arg0, s32 arg1, s32 *arg2) {
    s32 v = *arg2;
    if (v == 0) {
        func_800E73EC(arg0->unk0, arg0->unk10);
        if (arg0->unk3 != 0) {
            func_800D85AC(arg0->unk0, 0x1E);
        }
        arg0->unk4 = func_800D87D0(arg0->unk0);
    }
    if (func_800D72BC(arg0->unk0, arg0->unk10, v, arg0->unk4, arg0->unk8, 1) != 0) {
        D_800FEC55 = 1;
        return 1;
    }
    return 0;
}

INCLUDE_ASM("asm/us/ovl_0b/nonmatchings/ovl_0b_2", func_800D960C);

INCLUDE_ASM("asm/us/ovl_0b/nonmatchings/ovl_0b_2", func_800D974C);

INCLUDE_ASM("asm/us/ovl_0b/nonmatchings/ovl_0b_2", func_800D9990);

INCLUDE_ASM("asm/us/ovl_0b/nonmatchings/ovl_0b_2", func_800D9AB0);

INCLUDE_ASM("asm/us/ovl_0b/nonmatchings/ovl_0b_2", func_800D9D58);

s32 func_800D9F5C(u8 *arg0, s32 arg1, s32 arg2) {
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

void func_800DC634(char *arg0) {
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

void func_800DCA68(BtlUnit9C *arg0, SVECTOR *arg1, VECTOR *arg2, s32 arg3) {
    SVECTOR r;
    VECTOR t;
    if (arg1 != NULL) {
        func_800316D8(arg1, &arg0->unk24);
    } else {
        r.vx = 0;
        r.vy = 0;
        r.vz = 0;
        func_800316D8(&r, &arg0->unk24);
    }
    if (arg2 != NULL) {
        TransMatrix(&arg0->unk24, arg2);
    } else {
        t.vx = 0;
        t.vy = 0;
        t.vz = 0;
        TransMatrix(&arg0->unk24, &t);
    }
    if (arg3 != 0) {
        func_800DCB20(arg0, arg3);
    } else {
        arg0->unk54 = 0x1000;
        arg0->unk56 = 0x1000;
        arg0->unk58 = 0x1000;
        func_800C7B84(arg0);
    }
}

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

BtlObjSlot *func_800DCFB0(s32 arg0, char *arg1) {
    BtlObjSlot *p = &D_80100F1C[arg0];
    if (p->unk1C == 0 || p->unk0 == NULL) {
        sprintf(D_80101470, arg1, arg0);
        func_800DC634(D_80101470);
    }
    return p;
}

Unk8010141C *func_800DD02C(s32 arg0, char *arg1) {
    Unk8010141C *p = &D_8010141C[arg0];
    if (p->unk4C == 0 || p->unk0 == NULL) {
        sprintf(D_80101470, arg1, arg0);
        func_800DC634(D_80101470);
    }
    return p;
}

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

void func_800E702C(void) {
    GameData *gd;
    D_800F78A4->unk28 = 0;
    gd = D_8006794C->unk1C;
    D_800F78A4->unk20 = (s32 *)gd->unk10[gd->unk8];
    D_800F78A4->unk18 = &D_8006794C->unkC[D_8006794C->unk1C->unk8];
    D_800F78A4->unk1C = &D_8006794C->unk10[D_8006794C->unk1C->unk8];
    D_800F78A4->unk14 = &D_8006794C->unk1C->unk18;
}

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

s32 func_800E72C8(s32 arg0) {
    s32 flag;
    BtlUnit *u;
    u32 f;
    if (arg0 & 0x100) {
        arg0 &= 0xFF;
        flag = 1;
    } else {
        flag = 0;
    }
    u = func_800E716C(arg0);
    f = u->unk68;
    if ((f & 1) || flag != 0) {
        return u->unk9C->unk8->unkA;
    }
    return D_8006794C->unk1C->unk8E4.battle->unk488[(f >> 1) & 0xF].unk0->unk24;
}

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

void func_800E73EC(s32 arg0, s16 *arg1) {
    BtlUnit *u = func_800E716C(arg0);
    if (func_800E7298(u) != 0) {
        arg1[0] = u->unk8C >> 16;
        arg1[1] = u->unk90 >> 16;
        arg1[2] = u->unk94 >> 16;
    } else {
        arg1[0] = u->unk98->unk0 >> 16;
        arg1[1] = u->unk98->unk4 >> 16;
        arg1[2] = u->unk98->unk8 >> 16;
    }
}

void func_800E748C(BtlUnit *arg0, s16 *arg1) {
    if (func_800E7298(arg0) != 0 || !(arg0->unk68 & 1)) {
        func_800E73B4(arg0, (u16 *)arg1);
        arg1[1] = 0;
    } else {
        arg1[0] = arg0->unk8C >> 16;
        arg1[1] = arg0->unk90 >> 16;
        arg1[2] = arg0->unk94 >> 16;
    }
}

void func_800E750C(s32 arg0, s16 *arg1) {
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

s32 func_800E774C(s32 arg0, s32 arg1, s16 *arg2) {
    BtlUnitList *l = D_800F78A4->unkC;
    BtlUnit *u = l->unk28[arg0];
    if (arg0 >= l->unk27) {
        return 1;
    }
    func_800E753C(u, 0, arg1);
    arg2[0] = u->unk86[0];
    arg2[1] = u->unk86[1];
    arg2[2] = u->unk86[2];
    return 0;
}

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

void func_800E7A44(s32 arg0, VECTOR *arg1) {
    BtlUnit *u = func_800E716C(arg0);
    BtlUnit9C *m = u->unk9C;
    if (!(m->unk0 & 2)) {
        arg1->vz = 0x1000;
        arg1->vy = 0x1000;
        arg1->vx = 0x1000;
    } else {
        arg1->vx = m->unk54;
        arg1->vy = u->unk9C->unk56;
        arg1->vz = u->unk9C->unk58;
    }
}

INCLUDE_ASM("asm/us/ovl_0b/nonmatchings/ovl_0b_2", func_800E7ACC);

void func_800E7BA0(s32 arg0, s32 arg1, s16 *arg2) {
    s16 sp10[4];
    s16 sp18[4];
    s16 sp20[4];
    if (func_800E716C(arg1) != func_800E716C(arg0)) {
        func_800E750C(arg0, sp10);
        func_800E750C(arg1, sp18);
        func_800CF808(sp18, sp10, sp20);
        func_800CFD78(sp18, sp20, func_800E72C8(arg1), arg2);
    } else {
        func_800E750C(arg0, arg2);
    }
}

INCLUDE_ASM("asm/us/ovl_0b/nonmatchings/ovl_0b_2", func_800E7C54);

s32 func_800E7CC4(s32 arg0, s32 arg1) {
    return (u16)(func_800E716C(arg0)->unk9C->unk8->unkA / 6) + arg1;
}

u32 func_800E7D18(s32 arg0) {
    return (u8)func_800E716C(arg0)->unk68 >> 5;
}

INCLUDE_ASM("asm/us/ovl_0b/nonmatchings/ovl_0b_2", func_800E7D3C);

INCLUDE_ASM("asm/us/ovl_0b/nonmatchings/ovl_0b_2", func_800E7EC0);

void func_800E7FC0(BtlUnit *arg0, s32 arg1) {
    if (func_800E7298(arg0) == 0) {
        if (arg1 < 8) {
            if (!((arg0->unk68 >> 22) & 1)) {
                arg0->unk68 |= 0x400000;
                func_800E7EC0(arg0, arg1);
            }
        } else if ((arg0->unk68 >> 22) & 1) {
            if (!(arg0->unk3C & 0x40000000)) {
                arg0->unk68 &= ~0x400000;
            }
        } else {
            func_800E7EC0(arg0, arg1);
        }
    }
}

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

s32 *func_800E871C(s32 arg0, s32 *arg1) {
    s32 id = (arg0 & 0xFFFF) | 0x20000;
    u32 size = (u32)func_8001C7FC(5, id, 0);
    s32 *p = func_8001C7FC(4, id, 0);
    if (*arg1 == 0x1234) {
        func_800E86E0(p, arg1 + 1, size);
    } else {
        func_800E86E0(arg1 + 1, p, size);
        *arg1 = 0x1234;
    }
    return p;
}

void func_800E87C4(s32 arg0) {
    func_800AB4FC();
    func_800E86E0(D_8006794C->unk1C->unk8E4.battle->unk11C0->unk44, D_80100DC0.unk54->unk50, arg0 << 10);
    D_80100DC0.unk54->unk50 = D_8006794C->unk1C->unk8E4.battle->unk11C0->unk44;
}

INCLUDE_ASM("asm/us/ovl_0b/nonmatchings/ovl_0b_2", func_800E8854);

void func_800E8980(s32 arg0) {
    D_800F78A4->unkC->unk44 = func_800BED10(D_800F78A4->unkC->unk25[arg0]);
}

void func_800E89D0(void) {
    BtlUnit *u = D_8006794C->unk1C->unk8E4.battle->unk6F8;
    while (u != NULL) {
        if (u->unk68 & 1) {
            func_800C0D24(u);
            func_800E8408(u);
            func_800E8460(u);
        }
        u = u->unk0;
    }
    if (D_800F78A4->unk0->unk2 == 0x1BF) {
        func_800BC78C(func_800E716C(0x10));
    }
}

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

void func_800E8DC8(s32 arg0) {
    s32 i;
    if (func_800E8D60() == 0) {
        if (arg0 == -1) {
            for (i = 0; i < D_800F78A4->unkC->unk24; i++) {
                func_800B2804(D_800F78A4->unkC->unk4[i]);
            }
        } else {
            func_800B2804(func_800E716C(arg0));
        }
    }
}

void func_800E8E84(s32 arg0) {
    s32 i;
    if (func_800E8D60() == 0) {
        if (arg0 == -1) {
            for (i = 0; i < D_800F78A4->unkC->unk24; i++) {
                func_800CC044(D_800F78A4->unkC->unk4[i]);
            }
        } else {
            func_800CC044(func_800E716C(arg0));
        }
    }
}

void func_800E8F40(s32 arg0) {
    D_8006794C->unk9 = arg0;
}

INCLUDE_ASM("asm/us/ovl_0b/nonmatchings/ovl_0b_2", func_800E8F50);

INCLUDE_ASM("asm/us/ovl_0b/nonmatchings/ovl_0b_2", func_800E9000);

INCLUDE_ASM("asm/us/ovl_0b/nonmatchings/ovl_0b_2", func_800E9124);

INCLUDE_ASM("asm/us/ovl_0b/nonmatchings/ovl_0b_2", func_800E9168);

INCLUDE_ASM("asm/us/ovl_0b/nonmatchings/ovl_0b_2", func_800E9290);

void func_800E94C4(void) {
    u8 v;
    if (D_800F8D74 != 0 && D_800F78A4->unkC->unk27 != 0) {
        if (func_800E716C(8)->unk68 & 1) {
            v = D_800F8D80;
        } else {
            v = D_800F8D81;
        }
        func_800C2F9C(func_800E454C(v));
    }
}

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

Unk80104E34 *func_800E9730(s32 arg0) {
    Unk80104E34 *p;
    s32 i;
    if (arg0 != 0) {
        p = D_80104E34;
        for (i = 0; i < 6; i++, p++) {
            if (p->unk0 == arg0) {
                return p;
            }
        }
    }
    return NULL;
}

INCLUDE_ASM("asm/us/ovl_0b/nonmatchings/ovl_0b_2", func_800E976C);

void func_800E9884(s32 arg0) {
    Unk80104E34 *p = func_800E9730(arg0);
    if (p != NULL) {
        func_8005546C(0, arg0);
        p->unk0 = 0;
    }
}

void func_800E98C8(s32 arg0, s32 arg1) {
    Unk80104E34 *p = func_800E9730(arg0);
    if (p != NULL) {
        p->unk4 = arg1;
    }
}

void func_800E98F8(s32 arg0) {
    Unk80104E34 *p = D_80104E34;
    s32 i;
    for (i = 0; i < 6; i++, p++) {
        if (p->unk0 != 0) {
            p->unk4 = arg0;
        }
    }
}

s32 func_800E9930(s32 arg0, s32 arg1, s32 arg2) {
    s32 v = func_800E9638(arg2);
    if (v != 0) {
        func_800553EC(func_800DA3D8(arg1, arg0), v, func_800E976C(arg2), 0x7F);
    }
    return v;
}

void func_800E99B0(s32 arg0, s32 arg1, s32 arg2, s32 arg3) {
    func_800E9884(arg3);
    func_800E9930(arg0, arg1, arg2);
}

s32 func_800E9A00(s32 arg0, s32 arg1) {
    s32 v = func_800E9638(arg1);
    if (v != 0) {
        func_800553EC(func_800E454C(arg0), v, func_800E976C(arg1), 0x7F);
    }
    return v;
}

INCLUDE_ASM("asm/us/ovl_0b/nonmatchings/ovl_0b_2", func_800E9A74);

INCLUDE_ASM("asm/us/ovl_0b/nonmatchings/ovl_0b_2", func_800E9BBC);

void func_800E9D30(void) {
    func_800EB05C();
    func_800F3C08();
    func_800F4884();
    D_80104E68.unk6 = 1;
    D_80104E68.unk15 = 0;
    VSyncCallback(func_800E9A74);
}

void func_800E9D84(void) {
    void *p = func_8001C7FC(4, 0x14, 0);
    if (p != NULL) {
        func_8002C158(p);
    }
}

void func_800E9DBC(void) {
    void *p = func_8001C7FC(4, 0x1E, 0);
    if (p != NULL) {
        func_80028B20(p);
        func_80033D1C(0);
    }
}

void func_800E9DFC(void) {
    D_80104E68.unk1C = 0;
}

void func_800E9E10(void) {
    func_8001D898(&D_80104E68, 0, 0x11C);
    func_800F2A0C();
    func_800F48DC();
    func_800F48E4();
    func_800F4630();
    func_800EBB34();
    func_800EC008();
    func_8003575C(D_800A83AC);
    VSyncCallback(NULL);
}

void func_800E9E80(void) {
    func_800E9F3C(0);
    VSyncCallback(NULL);
    func_800F4700();
    func_800F4D20();
    D_80104E68.unkE = 0;
}

INCLUDE_ASM("asm/us/ovl_0b/nonmatchings/ovl_0b_2", func_800E9EC4);

void func_800E9F3C(u8 arg0) {
    if (arg0) {
        D_80104E68.unk12 = 1;
    } else {
        D_80104E68.unk0 = 0;
    }
}

void func_800E9F68(u8 arg0) {
    if (D_80104E68.unk6 != 0) {
        if (D_80104E68.unk2 != arg0) {
            D_80104E68.unk2 = arg0;
            if (arg0 == 0) {
                func_800EB410();
                func_800EBD58();
            }
            func_800EB2FC();
        }
    }
}

BtlUnit *func_800E9FCC(void) {
    BtlUnit *u = NULL;
    if (D_80104F88->unk12B4 != 0) {
        u = D_80104F88->unk12A0;
    }
    return u;
}

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

u8 *func_800EA0B4(s32 arg0) {
    if (arg0 >= 0xC0) {
        return NULL;
    }
    return D_8006794C->unk1C->unk8E4.battle->unk16CC[arg0].unkE + D_800F909C;
}

void func_800EA0F8(s32 arg0) {
    func_800EC170(arg0);
    func_800EBEFC(arg0);
}

void func_800EA124(s32 arg0) {
    func_800EC170(arg0);
}

void func_800EA144(s32 arg0) {
    func_800EC14C(arg0);
}

void func_800EA164(s32 arg0) {
    func_800EBEFC(arg0);
    if (func_800EC194(arg0) != 0 && func_80027F34(arg0) == 0) {
        func_800EC170(arg0);
    }
}

void func_800EA1B0(s32 arg0) {
    func_800EC070(arg0);
}

void func_800EA1D0(s8 arg0) {
    if (D_80104E68.unk9 == 0) {
        D_80104E68.unk9 = 1;
        D_80104E68.unk14 = arg0;
    }
}

void func_800EA1F8(u32 *arg0, DRAWENV *arg1) {
    D_80104E68.unk10 = 1;
    DrawOTagEnv(arg0, arg1);
    D_80104E68.unk10 = 0;
}

void func_800EA230(u32 *arg0) {
    D_80104E68.unk10 = 1;
    DrawOTag(arg0);
    D_80104E68.unk10 = 0;
}

u8 func_800EA268(void) {
    return D_80104E68.unk15;
}

u8 func_800EA27C(void) {
    return D_80104E68.unkE;
}

void func_800EA290(void) {
    if (D_80104E68.unk6 == 0) {
        GameData *gd = D_8006794C->unk1C;
        if (!(gd->unk8E4.battle->unk0 & 0x100)) {
            PutDispEnv(&D_8006794C->unkC[gd->unk8]);
        }
    }
}

INCLUDE_ASM("asm/us/ovl_0b/nonmatchings/ovl_0b_2", func_800EA30C);

void func_800EA338(s8 arg0) {
    D_80104E68.unkB = arg0;
}

void func_800EA34C(void) {
    u8 v = D_8006794C->unk1C->unk8;
    D_80104E68.unkC = 2;
    D_80077148.unk0 = v;
}

INCLUDE_ASM("asm/us/ovl_0b/nonmatchings/ovl_0b_2", func_800EA378);

void func_800EA400(void) {
    if (D_80104E68.unk2 != 0) {
        func_800EBB78();
        func_800EB114();
    }
}

void func_800EA440(void) {
    if (D_80104E68.unk8 == 0) {
        func_800EBA20();
        func_800F3060();
        func_8002C9B0();
        func_800EA518();
    }
    func_800F0DF8();
}

void func_800EA498(void) {
    func_800EB1CC();
    func_800F3D0C();
    func_8002E180();
    func_800EBBB4();
}

void func_800EA4D0(void) {
    if (D_80104E68.unkF != 0) {
        func_800F386C();
        func_800ECDB8();
        func_800F2374();
    }
}

void func_800EA518(void) {
    func_800F3A8C();
    func_800ECF0C();
    func_800F2560();
}

void func_800EA548(void) {
}

void func_800EA550(void) {
    if (D_80104E68.unk12 != 0) {
        D_80104E68.unk12 = 0;
        D_80104E68.unk0 = 1;
        if (D_80104E68.unk6 != 0 && D_80104E68.unk2 != 1) {
            D_80104E68.unk2 = 1;
            func_800EB2FC();
        }
        func_800F2AAC();
        func_800EC048();
    }
}

void func_800EA5C4(void) {
    if (D_80104E68.unkF != 0 && D_80104E68.unk6 != 0) {
        GameData *gd = D_8006794C->unk1C;
        if (!(gd->unk8E4.battle->unk0 & 0x100)) {
            PutDispEnv(&D_8006794C->unkC[gd->unk8]);
        }
    }
}

INCLUDE_ASM("asm/us/ovl_0b/nonmatchings/ovl_0b_2", func_800EA650);

INCLUDE_ASM("asm/us/ovl_0b/nonmatchings/ovl_0b_2", func_800EA7B8);

INCLUDE_ASM("asm/us/ovl_0b/nonmatchings/ovl_0b_2", func_800EA908);

INCLUDE_ASM("asm/us/ovl_0b/nonmatchings/ovl_0b_2", func_800EAAFC);

INCLUDE_ASM("asm/us/ovl_0b/nonmatchings/ovl_0b_2", func_800EAC88);

INCLUDE_ASM("asm/us/ovl_0b/nonmatchings/ovl_0b_2", func_800EAD94);

INCLUDE_ASM("asm/us/ovl_0b/nonmatchings/ovl_0b_2", func_800EAE8C);

void func_800EB05C(void) {
    s32 i;
    Unk80104F88 *p = func_800F2A60(0x12D8);
    D_80104F88 = p;
    p->unk12B4 = 0;
    p->unk12A0 = NULL;
    D_80104F88->unk12C0 = 0;
    D_80104F88->unk12BC = D_8006794C->unk1C->unk8E4.battle->unk132C & 3;
    D_80104F88->unk12B6 = func_800EB9E0();
    func_800EB5AC();
    func_800EA650();
    func_800EB2E0();
    for (i = 0; i < 4; i++) {
        D_80104F88->unk12A4[i] = func_8002E43C(i);
    }
}

void func_800EB114(void) {
    u8 st;
    func_800EAE8C();
    st = D_80104F88->unk12B4;
    if (st == 4) {
        return;
    }
    if (st == 0) {
        BtlUnit *u = func_800EAD94();
        D_80104F88->unk12A0 = u;
        if (u != NULL) {
            func_800EC4EC();
        }
    } else if (!(func_800EB7B0(D_80104F88->unk12A0) & 0xFF)) {
        func_800EB410();
    } else {
        D_800F8D94[D_80104F88->unk12B4]();
    }
}

void func_800EB1CC(void) {
    void (*fn)(void);
    func_800EB81C();
    func_800EB924();
    func_800EAAFC();
    func_800EAC88();
    fn = D_800F8DA8[D_80104F88->unk12B4];
    if (fn != NULL) {
        fn();
    }
}

void func_800EB238(void) {
    if (D_80104F88->unk12C0 < 75) {
        D_80104F88->unk12B6 = func_800EB9E0();
    } else {
        D_80104F88->unk12B6 = 2;
    }
    D_80104F88->unk12C0++;
}

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

void func_800EB2FC(void) {
    Unk80104F88 *p;
    s32 n = D_80104F88->unk12C4;
    s32 i = D_80104F88->unk12C5;
    while (n != 0) {
        n--;
        D_80104F88->unk12C8[i]->unkCF = 0;
        i = (i + 1) & 3;
    }
    p = D_80104F88;
    p->unk12C6 = 0;
    p->unk12C5 = 0;
    p->unk12C4 = 0;
}

void func_800EB358(BtlUnit *arg0) {
    Unk80104F88 *p = D_80104F88;
    arg0->unkCF = 1;
    D_80104F88->unk12C8[(p->unk12C5 + p->unk12C4) & 3] = arg0;
    p->unk12C4++;
}

void func_800EB3A0(void) {
    BtlUnit *u = D_8006794C->unk1C->unk8E4.battle->unk6F8;
    func_800EB2E0();
    while (u != NULL) {
        if (u->unk68 & 1) {
            u->unkCF = 0;
        }
        u = u->unk0;
    }
}

void func_800EB410(void) {
    BtlUnit *u = D_80104F88->unk12A0;
    if (u != NULL) {
        u->unkCF = 0;
    }
    if (D_80104F88->unk12B4 != 0) {
        D_80104F88->unk12B4 = 4;
        func_800EB46C();
    }
    func_800F142C();
}

INCLUDE_ASM("asm/us/ovl_0b/nonmatchings/ovl_0b_2", func_800EB46C);

void func_800EB518(Unk800EB518 *arg0, s32 arg1, s32 arg2, s8 arg3) {
    if (arg1 == 0) {
        arg0->unk0 = arg2;
        return;
    }
    arg0->unk8[arg0->unk0] = arg2;
    arg0->unk4[arg0->unk0] = arg3;
}

void func_800EB54C(Unk800EB518 *arg0) {
    s32 i;
    arg0->unk0 = 0;
    for (i = 0; i < 4; i++) {
        arg0->unk8[i] = 0;
        arg0->unk4[i] = 0;
    }
}

void func_800EB578(void) {
}

u8 *func_800EB580(void) {
    return &D_80104F88->unk12C4;
}

BtlUnit *func_800EB590(s32 arg0) {
    return D_80104F88->unk12C8[arg0];
}

INCLUDE_ASM("asm/us/ovl_0b/nonmatchings/ovl_0b_2", func_800EB5AC);

void func_800EB600(Unk80104F88Entry *arg0) {
    func_800EB63C(arg0);
    func_800EB6FC(arg0);
    func_800EA7B8(arg0);
    func_800EA908(arg0);
}

INCLUDE_ASM("asm/us/ovl_0b/nonmatchings/ovl_0b_2", func_800EB63C);

void func_800EB6FC(Unk80104F88Entry *arg0) {
    Character *c = arg0->unk14;
    s32 i;
    s32 j;
    func_8001D898((s32)arg0->unk3DC, 0, 0xC0);
    for (i = 0; i < 5; i++) {
        u8 item = c->unk39[i];
        if (item != 0xFF) {
            Unk80069018 *e = &D_80069018[item];
            for (j = 0; j < 3; j++) {
                s32 a = e->unkC[j];
                if (a != 0 && a < 0xC0) {
                    arg0->unk3DC[a] = 1;
                }
            }
        }
    }
}

s32 func_800EB7B0(BtlUnit *arg0) {
    if (arg0 == NULL) {
        return 0;
    }
    if (arg0->unk28 != 0 && !(func_800C09A4(arg0, 0x42021D03) & 0xFF)) {
        return (D_800FDF3A >> ((u8)arg0->unk68 >> 5)) & 1;
    }
    return 0;
}

INCLUDE_ASM("asm/us/ovl_0b/nonmatchings/ovl_0b_2", func_800EB81C);

INCLUDE_ASM("asm/us/ovl_0b/nonmatchings/ovl_0b_2", func_800EB924);

s32 func_800EB9E0(void) {
    s32 v = D_80104F88->unk12BC;
    if (v != 0) {
        return (v != 1) * 2;
    }
    return 1;
}

void func_800EBA10(void) {
    D_80104F88->unk12B4 = 0;
}

INCLUDE_ASM("asm/us/ovl_0b/nonmatchings/ovl_0b_2", func_800EBA20);

void func_800EBB34(void) {
    D_80104F90.unk0 = 0;
    D_80104F90.unk1 = 0;
    D_80104F90.unk2 = 0;
    D_80104F90.unk3 = 0;
    D_80104F90.unk4 = 0;
    D_80104F90.unkC = 0;
    D_80104FA0 = func_800F2A60(0x100);
}

void func_800EBB78(void) {
    if (D_80077528 & 0x08000000) {
        func_800EBCC4(D_80104F90.unk0 == 0);
    }
}

void func_800EBBB4(void) {
    if (D_80104F90.unk0 != 0 && D_80104E75 != 0) {
        D_80104F90.unkC++;
        if (D_80104F90.unk1 != 0 && D_80104F90.unkC >= 7) {
            D_80104F90.unk1 = 0;
            func_800F3ECC(0);
        }
    }
}

void func_800EBC30(void) {
    if (func_800F3EA4()->unk5C == 0) {
        if (D_80104F90.unk0 != 0) {
            D_80104F90.unk3 = 1;
        }
    }
    if (D_80104F90.unk3 != 0 && D_80104F90.unk0 != 0) {
        D_80104F90.unk0 = 0;
        D_80104F90.unk3 = 0;
        func_800F3ECC(1);
    }
}

u8 func_800EBCB4(void) {
    return D_80104F90.unk0;
}

void func_800EBCC4(u8 arg0) {
    if (arg0) {
        if (D_80104F90.unk0 == 0) {
            func_8002F488(0x2AA);
            D_80104F90.unk0 = 1;
            D_80104F90.unk1 = 1;
            D_80104F90.unkC = 0;
        }
    } else if (D_80104F90.unk0 != 0 && D_80104F90.unk3 == 0) {
        func_8002F488(0x65);
        D_80104F90.unk3 = 1;
    }
}

void func_800EBD58(void) {
    if (D_80104F90.unk0 != 0 && D_80104F90.unk3 == 0) {
        D_80104F90.unk3 = 1;
    }
}

void func_800EBD88(void) {
    s32 (*fn)(void);
    D_80104F90.unk4 = 0;
    if (D_8006794C->unk1C->unk8E4.battle->unkD83 != 0) {
        D_80104F90.unk4 = (s32)func_80035774(4);
    } else {
        fn = D_800F8DC4[D_80104F88->unk12B4];
        if (fn != NULL) {
            D_80104F90.unk4 = fn();
        }
    }
}

INCLUDE_ASM("asm/us/ovl_0b/nonmatchings/ovl_0b_2", func_800EBE1C);

INCLUDE_ASM("asm/us/ovl_0b/nonmatchings/ovl_0b_2", func_800EBEFC);

void func_800EC008(void) {
    Unk80104FA8 *p = func_800F2A60(0x2E0);
    D_80104FA8 = p;
    func_8001D898((s32)p->unk1E0, 0, 0x100);
    func_800EC048();
}

void func_800EC048(void) {
    func_800EC1B0();
    func_800EC258();
}

INCLUDE_ASM("asm/us/ovl_0b/nonmatchings/ovl_0b_2", func_800EC070);

UnkPair *func_800EC114(s32 arg0) {
    return D_80104FA8->unk4[arg0];
}

UnkPair *func_800EC130(s32 arg0) {
    return D_80104FA8->unk80[arg0];
}

void func_800EC14C(s32 arg0) {
    D_80104FA8->unk1E0[arg0]++;
}

void func_800EC170(s32 arg0) {
    D_80104FA8->unk1E0[arg0]--;
}

s32 func_800EC194(s32 arg0) {
    return D_80104FA8->unk1E0[arg0];
}

void func_800EC1B0(void) {
    s32 n = 0;
    s32 i;
    UnkPair *p = D_8006794C->unk1C->unk574;
    Unk80104FA8 *f = D_80104FA8;
    for (i = 0; i < 0x100; i++, p++) {
        if (p->unk1 != 0 && p->unk0 != 0xFF && p->unk0 >= 0xE0) {
            f->unk4[n] = p;
            n++;
        }
    }
    for (i = n; i < 31; i++) {
        D_80104FA8->unk4[i] = NULL;
    }
    D_80104FA8->unk0 = n;
}

INCLUDE_ASM("asm/us/ovl_0b/nonmatchings/ovl_0b_2", func_800EC258);

void func_800EC334(s32 arg0) {
    s32 i;
    for (i = 0; i < 31; i++) {
        if (D_80104FA8->unk4[i] == NULL) {
            D_80104FA8->unk4[i] = func_80027EDC(arg0);
            break;
        }
    }
    D_80104FA8->unk0++;
}

void func_800EC3C0(s32 arg0) {
    s32 i;
    for (i = 0; i < 0x58; i++) {
        if (D_80104FA8->unk80[i] == NULL) {
            D_80104FA8->unk80[i] = func_80027EDC(arg0);
            break;
        }
    }
    D_80104FA8->unk2++;
}

s32 func_800EC44C(s32 arg0) {
    s32 i;
    Unk80104FA8 *p = D_80104FA8;
    for (i = 0; i < 31; i++) {
        if (p->unk4[i] != NULL && arg0 == p->unk4[i]->unk0) {
            return i;
        }
    }
    return 0xFF;
}

s32 func_800EC49C(s32 arg0) {
    s32 i;
    Unk80104FA8 *p = D_80104FA8;
    for (i = 0; i < 0x58; i++) {
        if (p->unk80[i] != NULL && arg0 == p->unk80[i]->unk0) {
            return i;
        }
    }
    return 0xFF;
}

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

void func_800EDAD4(void) {
    if (D_80077528 & 0x01000000) {
        func_800EDDF8();
    } else if (D_80077528 & 0x20000) {
        func_800ECA50();
    } else if (D_80077528 & 0x40) {
        func_800EDD1C();
    } else if (D_80077528 & 0x10) {
        func_800EDCA0();
    } else {
        func_800EDD9C();
    }
}

void func_800EDB68(void) {
    func_800ECB5C();
}

INCLUDE_ASM("asm/us/ovl_0b/nonmatchings/ovl_0b_2", func_800EDB88);

void func_800EDC3C(void) {
    s32 v = D_80104FC8->unkB4->unk1B;
    func_800EB2BC(1);
    if (v != D_80104E68.unk15) {
        func_8002C584(v);
    }
    D_80104E68.unk15 = v;
}

INCLUDE_ASM("asm/us/ovl_0b/nonmatchings/ovl_0b_2", func_800EDCA0);

INCLUDE_ASM("asm/us/ovl_0b/nonmatchings/ovl_0b_2", func_800EDD1C);

void func_800EDD9C(void) {
    if (D_80077524 & 0x80) {
        func_800EE154();
    } else if (D_80077524 & 0x20) {
        func_800EE24C();
    } else {
        func_800EE31C();
    }
}

void func_800EDDF8(void) {
    func_800EB518(&D_80104FB0.unk18->unkB4->unk49C, 0, D_80104FB0.unk0, 0);
    func_800EB2BC(4);
    func_8002E2FC(D_80104F88->unk12A4[3], func_800EE120);
    func_8002DD00(D_80104F88->unk12A4[3]);
}

INCLUDE_ASM("asm/us/ovl_0b/nonmatchings/ovl_0b_2", func_800EDE6C);

INCLUDE_ASM("asm/us/ovl_0b/nonmatchings/ovl_0b_2", func_800EDF34);

INCLUDE_ASM("asm/us/ovl_0b/nonmatchings/ovl_0b_2", func_800EDF90);

INCLUDE_ASM("asm/us/ovl_0b/nonmatchings/ovl_0b_2", func_800EDFEC);

void func_800EE048(void) {
    s32 v = D_800F8E04[D_80104FB0.unk11];
    D_80104FB0.unk18->unkCF = 0;
    D_80104FB0.unk18->unkB3 = 1;
    func_800B1250((s32)D_80104FB0.unk18->unk4, v, D_800FD56C[v].unk8, D_80104FB0.unk18->unkB0, 0);
    func_800EB2BC(4);
    func_8002E2FC(D_80104F88->unk12A4[3], func_800EE0FC);
    func_8002DD00(D_80104F88->unk12A4[3]);
}

void func_800EE0FC(Window *arg0) {
    func_800EB2BC(0);
    D_80104FC0.unk0 = 0;
}

void func_800EE120(Window *arg0) {
    func_800EB2BC(0);
    D_80104FB0.unk10 = 0;
    func_800EB358(D_80104FB0.unk18);
}

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

void func_800EF00C(void) {
    func_800EB2BC(2);
    D_80104F88->unk12A4[3]->unk1E = 0;
}

void func_800EF044(void) {
    func_800EB2BC(4);
    func_8002E2FC(D_80104F88->unk12A4[2], func_800EF7D4);
    func_8002DD00(D_80104F88->unk12A4[2]);
}

INCLUDE_ASM("asm/us/ovl_0b/nonmatchings/ovl_0b_2", func_800EF094);

INCLUDE_ASM("asm/us/ovl_0b/nonmatchings/ovl_0b_2", func_800EF0D8);

INCLUDE_ASM("asm/us/ovl_0b/nonmatchings/ovl_0b_2", func_800EF228);

INCLUDE_ASM("asm/us/ovl_0b/nonmatchings/ovl_0b_2", func_800EF390);

INCLUDE_ASM("asm/us/ovl_0b/nonmatchings/ovl_0b_2", func_800EF4D8);

s32 func_800EF598(BtlCmd8 *arg0) {
    s32 cost = arg0->unkA;
    if (arg0->unkB & 4) {
        if (D_8007AF02 != 0) {
            cost *= 4;
        }
    }
    if (D_80104FE8 != 0) {
        cost >>= 1;
    }
    return cost;
}

INCLUDE_ASM("asm/us/ovl_0b/nonmatchings/ovl_0b_2", func_800EF5E0);

u8 *func_800EF674(void) {
    Unk16 *w = &D_80104F88->unk12A4[2]->unk30;
    UnkPair *e = func_800EC114(w->unk6 + w->unk5);
    if (e != NULL) {
        if (e->unk1 - func_800EC194(e->unk0) >= 0) {
            if (e->unk1 != func_800EC194(e->unk0)) {
                return D_800FB4F0 + D_800FD36C[e->unk0];
            }
            return NULL;
        }
    }
    return NULL;
}

u8 *func_800EF724(void) {
    Unk16 *w = &D_80104F88->unk12A4[2]->unk30;
    UnkPair *e = func_800EC130(w->unk6 + w->unk5);
    if (e != NULL) {
        if (e->unk1 - func_800EC194(e->unk0) >= 0) {
            if (e->unk1 != func_800EC194(e->unk0)) {
                return D_800FB4F0 + D_800FD36C[e->unk0];
            }
            return NULL;
        }
    }
    return NULL;
}

void func_800EF7D4(Window *arg0) {
    func_800EB2BC(1);
    D_80104F88->unk12A4[3]->unk1E = 1;
}

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

void func_800F1150(void) {
    if (D_80104FF0.unk6 != 0 && D_800F8EE4[D_80104FF0.unk2] != 0) {
        func_800EFA10(0);
    }
}

void func_800F11A4(BtlUnit *arg0) {
    if ((arg0->unk68 >> 29) & 1) {
        arg0->unk68 &= ~0x20000000;
        if (D_80104FF0.unk6 == 0) {
            func_800EFA10(0);
        }
    }
}

INCLUDE_ASM("asm/us/ovl_0b/nonmatchings/ovl_0b_2", func_800F11FC);

void func_800F1300(void) {
    if (!(func_800F0118() & 0xFF)) {
        func_8002F488(0x65);
        func_800F07F0();
    } else if (func_800F028C() & 0xFF) {
        func_800F0904();
    }
}

u8 func_800F1360(void) {
    return D_80104FFC;
}

Window *func_800F1370(void) {
    return D_80104F88->unk12A4[0];
}

s32 func_800F1388(void) {
    s32 ret = 0;
    if (D_80104F88->unk12B4 == 3) {
        ret = D_80104FFF != 0;
    }
    return ret;
}

u8 func_800F13BC(void) {
    return D_80104FF0.unk0;
}

s32 func_800F13CC(void) {
    BtlUnit *u = D_8006794C->unk1C->unk8E4.battle->unk6F8;
    s32 n = 0;
    while (u != NULL) {
        if ((u->unk68 >> 29) & 1) {
            n++;
        }
        u = u->unk0;
    }
    return n;
}

void func_800F142C(void) {
    BtlUnit *u = D_8006794C->unk1C->unk8E4.battle->unk6F8;
    while (u != NULL) {
        u->unk68 &= ~0x20000000;
        u = u->unk0;
    }
}

INCLUDE_ASM("asm/us/ovl_0b/nonmatchings/ovl_0b_2", func_800F1484);

u8 *func_800F1548(void) {
    return D_80105020;
}

void func_800F1554(void) {
    s32 i;
    s32 m;
    if (D_80104FFC == 0) {
        m = func_800C2DF4();
    } else {
        m = 0;
    }
    for (i = 0; i < 4; i++) {
        if (D_80077528 & D_800F8EA4[m][i]) {
            D_800F8E94[i]();
        }
    }
}

void func_800F160C(void) {
    if (func_800EFB3C(0) & 0xFF) {
        func_8002F488(0x67);
    }
}

void func_800F1640(void) {
    if (func_800EFB3C(1) & 0xFF) {
        func_8002F488(0x67);
    }
}

void func_800F1674(void) {
    if (func_800EFD1C(0) & 0xFF) {
        func_8002F488(0x67);
    }
}

void func_800F16A8(void) {
    if (func_800EFD1C(1) & 0xFF) {
        func_8002F488(0x67);
    }
}

void func_800F16DC(u8 arg0, u8 arg1) {
    BtlUnit *u = D_8006794C->unk1C->unk8E4.battle->unk6F8;
    if (arg0 == 0xFF) {
        arg0 = D_80104FFA;
    }
    for (; u != NULL; u = u->unk0) {
        if (((u->unk68 >> 28) & 1) && (u->unk68 & 1) == arg0) {
            if (!(u->unk3C & 0x100) || D_80104FF0.unkB != 0) {
                u->unk68 |= 0x20000000;
                if (arg1 == 0) {
                    return;
                }
            }
        }
    }
}

INCLUDE_ASM("asm/us/ovl_0b/nonmatchings/ovl_0b_2", func_800F1790);

INCLUDE_ASM("asm/us/ovl_0b/nonmatchings/ovl_0b_2", func_800F18F0);

void func_800F1970(s8 arg0) {
    Window *w;
    func_800EB2BC(4);
    w = func_8002DCAC(D_800F8EF4, 5, 2, 0);
    D_80104F88->unk12A4[0] = w;
    func_8002CF20(w, 4);
    func_8002E2E4(D_80104F88->unk12A4[0], func_800F1A8C);
    func_8002E2EC(D_80104F88->unk12A4[0], func_800F0A54);
    func_8002E2F4(D_80104F88->unk12A4[0], func_800F0BE0);
    D_80104FF0.unk11 = arg0;
    D_80104FF0.unkC = 1;
}

void func_800F1A20(s32 arg0) {
    if (D_80104FF0.unkC != 0) {
        D_80104FF0.unk12 = arg0;
        func_800EB2BC(4);
        func_8002E2FC(D_80104F88->unk12A4[0], func_800F1AF8);
        func_8002DD00(D_80104F88->unk12A4[0]);
    }
}

void func_800F1A8C(Window *arg0) {
    if (D_80104FF0.unk0 == 2) {
        func_8002E32C(D_80104F88->unk12A4[2], 0);
    }
    func_8002E32C(D_80104F88->unk12A4[3], 0);
    func_800EB2BC(D_80104FF0.unk11);
}

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

void func_800F2890(s32 arg0) {
    func_8002E380(D_80104F88->unk12A4[1], 7);
    func_8002DD00(D_80104F88->unk12A4[1]);
    func_800F3DE0(arg0);
    D_80105058 = 0;
}

INCLUDE_ASM("asm/us/ovl_0b/nonmatchings/ovl_0b_2", func_800F28F0);

u8 func_800F29A4(void) {
    return D_80105058;
}

void func_800F29B4(s32 arg0) {
    if (func_800F3EA4()->unk0 != 0) {
        D_800F8F04[D_80105059](arg0);
    }
}

void func_800F2A0C(void) {
    D_80105060.unk4 = D_8006794C->unk1C->unk7E0;
    D_80105060.unk0 = D_80105060.unk4;
    D_80105060.unk10 = 0x8000;
    D_80105060.unk8 = D_80105060.unk10;
    D_80105060.unkC = 0;
}

void func_800F2A58(void) {
}

void *func_800F2A60(s32 arg0) {
    s32 size = (arg0 + 3) >> 2 << 2;
    s32 p = D_80105060.unk4;
    D_80105060.unk4 += size;
    D_80105060.unkC += size;
    D_80105060.unk10 -= size;
    return (void *)p;
}

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

void func_800F3D0C(void) {
    func_800F3F10();
    func_800F316C();
    func_800F3FDC();
}

INCLUDE_ASM("asm/us/ovl_0b/nonmatchings/ovl_0b_2", func_800F3D3C);

INCLUDE_ASM("asm/us/ovl_0b/nonmatchings/ovl_0b_2", func_800F3DE0);

void func_800F3E60(void) {
    func_800F2AAC();
    if (D_80105078->unk64 != 0) {
        func_800F42F0();
    }
}

Unk80105078 *func_800F3EA4(void) {
    return D_80105078;
}

u8 func_800F3EB4(void) {
    return D_80105078->unk0;
}

void func_800F3ECC(u8 arg0) {
    D_80105078->unk0 = arg0;
}

void func_800F3EDC(s32 arg0) {
    func_800F2C74();
    func_800F2DC0(arg0);
    func_800F2ED0(arg0);
}

INCLUDE_ASM("asm/us/ovl_0b/nonmatchings/ovl_0b_2", func_800F3F10);

void func_800F3FDC(void) {
    Unk80105078 *p = D_80105078;
    u32 v = D_80104E80 & 0x3F;
    p->unk54 = v;
    if (v >= 0x20) {
        p->unk54 = 0x3F - v;
    }
}

INCLUDE_ASM("asm/us/ovl_0b/nonmatchings/ovl_0b_2", func_800F4010);

INCLUDE_ASM("asm/us/ovl_0b/nonmatchings/ovl_0b_2", func_800F4060);

INCLUDE_ASM("asm/us/ovl_0b/nonmatchings/ovl_0b_2", func_800F40D0);

void func_800F412C(Unk800F412C *arg0, s32 arg1) {
    arg0->unkC = arg1;
    arg0->unk4 = 1;
    arg0->unk6 = 0;
}

void func_800F4140(void) {
    D_80105078->unk18 = 0;
    D_80105078->unk28 = 0;
    D_80105078->unk38 = 0;
    func_800EB2BC(3);
}

void func_800F4180(void) {
    D_80105078->unk48 = 0;
    func_800EB2BC(D_80105078->unk58);
}

INCLUDE_ASM("asm/us/ovl_0b/nonmatchings/ovl_0b_2", func_800F41B8);

INCLUDE_ASM("asm/us/ovl_0b/nonmatchings/ovl_0b_2", func_800F422C);

INCLUDE_ASM("asm/us/ovl_0b/nonmatchings/ovl_0b_2", func_800F42F0);

INCLUDE_ASM("asm/us/ovl_0b/nonmatchings/ovl_0b_2", func_800F4384);

INCLUDE_ASM("asm/us/ovl_0b/nonmatchings/ovl_0b_2", func_800F4630);

INCLUDE_ASM("asm/us/ovl_0b/nonmatchings/ovl_0b_2", func_800F4700);

void func_800F4778(void) {
    func_80019194();
    func_8002C308(D_80104E68.unk15);
    if ((D_80077528 & 8) && D_80104E68.unkB == 0) {
        D_80104E68.unkB = 1;
    }
}

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

void func_800F4D40(void) {
    D_801051E0 ^= 1;
    func_8002C084(D_801051E0);
    func_800F4D80(D_8007716C, 0x18);
}

void func_800F4D80(u32 *arg0, u32 arg1) {
    u32 i;
    ClearOTag(arg0, 1);
    for (i = 1; i < arg1; i++) {
        arg0[i] = (u32)&arg0[i - 1] & 0xFFFFFF;
    }
}

void func_800F4DF4(void) {
}

void func_800F4DFC(void) {
    RECT *r;
    s32 v;
    D_801051F0.unk90 = 0;
    D_801051F0.unk8C = 0;
    D_801051F0.unk80 = 0;
    D_801051F0.unk0 = 0xFF;
    v = D_800FDE3C[(D_8006794C->unk24 >> 11) & 7];
    r = &D_801051F0.unkA4;
    r->x = 0x60;
    r->y = 8;
    r->w = 0x80;
    r->h = 0x15;
    D_801051F0.unk84 = v;
}

void func_800F4E68(void) {
}

INCLUDE_ASM("asm/us/ovl_0b/nonmatchings/ovl_0b_2", func_800F4E70);

void func_800F4F04(s32 arg0, s32 arg1) {
    if (arg1 >= D_801051F0.unk90) {
        D_801051F0.unk90 = arg1;
        D_801051F0.unk8D = 0;
        func_800F4FE8(arg0);
    }
}

void func_800F4F44(s32 arg0, s32 arg1) {
    if (arg1 >= D_801051F0.unk90) {
        D_801051F0.unk90 = arg1;
        D_801051F0.unk8D = 1;
        func_800F4FE8(arg0);
    }
}

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
