#include "ovl_14.h"

INCLUDE_RODATA("asm/us/ovl_14/nonmatchings/ovl_14", D_800A7000);

INCLUDE_ASM("asm/us/ovl_14/nonmatchings/ovl_14", func_800A7088);

void func_800A727C(void) {
    GameData *ctx = D_8006794C->unk1C;
    ClearOTagR(ctx->unk10[ctx->unk8], 0x1000);
}

void func_800A72C0(void) {
    GameData *ctx;
    u8 i;

    DrawSync(0);
    VSync(D_8006794C->unk9);
    ResetGraph(1);
    PutDispEnv(&D_8006794C->unkC[D_8006794C->unk1C->unk8]);
    ctx = D_8006794C->unk1C;
    i = ctx->unk8;
    DrawOTagEnv(ctx->unk10[i] + 0xFFF, &D_8006794C->unk10[i]);
    D_8006794C->unk1C->unk8 ^= 1;
    func_8002C0DC(D_8006794C->unk1C->unk8);
}

void func_800A73B8(void) {
    GameData *ctx = D_8006794C->unk1C;
    func_8001CCB4(0, 0, 0);
    D_8006794C->unk8 = 1;
    D_8006794C->unk9 = 2;
    ctx->unkC = 0;
    ctx->unk3A = 1;
}

INCLUDE_ASM("asm/us/ovl_14/nonmatchings/ovl_14", func_800A741C);

void func_800A7578(void) {
    func_800651D8();
    func_80050A84();
    func_8005392C();
    func_8003253C();
    func_800AB1E4();
    func_80062344();
    func_800A75C8();
}

void func_800A75C8(void) {
    func_800A7600();
    func_8004BBF0();
    func_800AB200();
    func_8006237C();
}

void func_800A7600(void) {
    s32 i;
    GameData *ctx = D_8006794C->unk1C;

    ctx->unk9 = 0;
    ctx->unk39 = 0;
    ctx->unk38 = 0;
    ctx->unk7E8 = 0xFFFF;
    ctx->unk7EA = 0xFFFF;
    ctx->unk840 = 0xFFFF;
    ctx->unk850 = 0;
    ctx->unk8AC = -1;
    ctx->unk8B0 = 0xFFFF;
    for (i = 1; i >= 0; i--) {
        ctx->unk8B4[i] = 0xFFFF;
    }
    for (i = 1; i >= 0; i--) {
        ctx->unk7EC[i] = -1;
        ctx->unk7F4[i] = -1;
    }
}

void func_800A767C(void) {
    s32 i;

    for (i = 1; i >= 0; i--) {
        D_800739B0[i].unk1 = 0;
        D_800739B0[i].unk0 = 0;
    }
    func_80055D40();
    func_80055D60();
    if (D_8006794C->unk1C->unk0 & 0x100000) {
        func_800551F0();
        func_800A7768();
        D_8006794C->unk1C->unk0 &= ~0x100000;
    }
    func_8002306C();
    D_8006794C->unk1C->unk83C = (s32 (*)(s32, s32, s32, s32, s32))func_800A799C;
    D_8006794C->unk1C->unk84C = 0;
    func_800A78C0();
    func_800A780C();
    func_800A796C();
    func_800239D0();
    SetDispMask(1);
}

void func_800A7768(void) {
    s32 i;
    GameData *ctx = D_8006794C->unk1C;

    for (i = 1; i >= 0; i--) {
        ctx->unk81C[i] = 0xFFFF;
        ctx->unk820[i] = 0xFFFF;
        ctx->unk824[i] = 0;
        ctx->unk826[i] = 0x7F;
    }
    ctx->unk0 &= 0xBFFFFF;
}

void func_800A77BC(void) {
    GameData *ctx;

    SetDispMask(0);
    ctx = D_8006794C->unk1C;
    if (ctx->unk0 & 0x20000000) {
        ctx->unk0 |= 0x400000;
    }
}

void func_800A780C(void) {
    s32 a;
    s32 b;

    a = (s32)func_8001C7FC(4, 0x7003A, 0);
    memmove(D_8006794C->unk1C->unk814, (void *)a, (s32)func_8001C7FC(5, 0x7003A, 0));
    a = (s32)func_8001C7FC(4, 0x90122, 0);
    b = (s32)func_8001C7FC(5, 0x90122, 0);
    func_80055DF4();
    func_80055E14(a, b, 1);
}

void func_800A78C0(void) {
    u8 *buf;
    FileEntry *e;

    buf = D_80067944 + 0x10000;
    e = func_8001E218(1, 0x2717);
    func_8002177C(D_8006794C->unk1C->unk854, e->unk4, (e->unkC - e->unk4) << 11, buf, 0, -1, 0);
    while (func_800217C4(D_8006794C->unk1C->unk854) != 0) {
    }
    D_800AB230 = buf;
    func_8001C750((s32)buf, 0);
}

void func_800A796C(void) {
    func_8001C784(D_800AB230);
    D_800AB230 = NULL;
}

INCLUDE_ASM("asm/us/ovl_14/nonmatchings/ovl_14", func_800A799C);

void func_800A7F70(s32 arg0, s32 arg1) {
    GameData *ctx = D_8006794C->unk1C;

    ctx->unk826[0] = arg0;
    if (arg0 == 0) {
        ctx->unk842 = arg1;
        ctx->unk8C4 = VSync(-1);
    }
}

s32 func_800A7FB0(void) {
    GameData *ctx = D_8006794C->unk1C;
    return (u32)(VSync(-1) - ctx->unk8C4) < ctx->unk842;
}

void func_800A7FF0(void) {
    s32 i;
    Ovl14Mode *m;

    i = 0;
    m = D_800AB238;
    D_800AB308.unk0 = 0;
    D_800AB308.unk2 = 0;
    D_800AB308.unk3 = 0;
    for (; i < 6; i++, m++) {
        if (m->unk0 != NULL) {
            m->unk0();
        }
    }
    if (D_800AB238[D_800AB308.unk0].unk4 != NULL) {
        D_800AB238[D_800AB308.unk0].unk4();
    }
}

void func_800A808C(void) {
    if (D_800AB238[D_800AB308.unk0].unk8 != NULL) {
        D_800AB238[D_800AB308.unk0].unk8();
    }
}

void func_800A80D4(void) {
    if (D_800AB238[D_800AB308.unk0].unkC != NULL) {
        D_800AB238[D_800AB308.unk0].unkC();
    }
}

s32 func_800A811C(void) {
    return D_800AB308.unk0;
}

void func_800A812C(s32 arg0) {
    if (arg0 != D_800AB308.unk0) {
        if (D_800AB238[D_800AB308.unk0].unkC != NULL) {
            D_800AB238[D_800AB308.unk0].unkC();
        }
        D_800AB308.unk0 = arg0;
        if (D_800AB238[D_800AB308.unk0].unk4 != NULL) {
            D_800AB238[D_800AB308.unk0].unk4();
        }
    }
}

s32 func_800A81B8(void) {
    u32 i;
    s32 prev;

    i = 0;
    prev = D_8006794C->unkB;
    do {
        func_8001DCB8();
    } while (D_8006794C->unkB == 0xFF && ++i < 10);
    func_80033D1C(1);
    if (func_800A8298(prev, D_8006794C->unkB)) {
        return func_800A8260(prev);
    }
    return 0xFF;
}

s32 func_800A8260(s32 arg0) {
    s32 i;
    s32 m;

    for (i = 0; i < 4; i++) {
        m = 1 << i;
        if (arg0 & m) {
            return i;
        }
    }
    return 0;
}

u8 func_800A8298(s32 arg0, s32 arg1) {
    s32 a = arg0 & 0xF;
    s32 b = arg1 & 0xF;
    return (b & a) != a;
}

s32 func_800A82B0(void) {
    if (func_8002F810() != 0) {
        return 0;
    }
    return D_800AB308.unk0 == 1;
}

void func_800A82F4(s32 arg0) {
    D_800AB308.unk3 = arg0;
    if (func_800A811C() == 1) {
        func_800A8618();
    }
}

u8 func_800A832C(void) {
    return D_800AB308.unk3;
}

void func_800A833C(void) {
}

void func_800A8344(void) {
    func_80032628(func_800A8378);
}

void func_800A8368(void) {
}

void func_800A8370(void) {
}

void func_800A8378(void) {
    func_800A812C(4);
}

void func_800A8398(void) {
}

void func_800A83A0(void) {
    D_800AB310 = 0;
}

void func_800A83AC(void) {
    if (++D_800AB310 >= 2) {
        func_800A812C(5);
    }
}

void func_800A83E8(void) {
    if (func_800A8FCC() == 0) {
        func_800A7578();
        D_8006794C->unk8 = 1;
        D_8006794C->unk1C->unkC = 0x46;
        D_8006794C->unk1C->unk3A = 2;
    }
}

void func_800A8448(void) {
}

void func_800A8450(void) {
    D_800AB318 = 0;
    func_80032628(func_800A84B0);
}

void func_800A847C(void) {
    if (D_800AB318 != 0) {
        func_800A84EC();
    }
}

void func_800A84A8(void) {
}

void func_800A84B0(void) {
    D_800AB318 = 1;
    if (func_801F5330()) {
        func_80032634(0);
    }
}

void func_800A84EC(void) {
    s32 mode;
    s32 r;

    if (func_801F5330()) {
        func_800A75C8();
        r = func_800A81B8();
        if (r != 0xFF) {
            func_80032120(8, ((r & 3) << 14) | 0x3A98);
            mode = 3;
        } else {
            func_80032634(0);
            mode = 4;
        }
    } else {
        func_800A82F4(1);
        mode = 1;
    }
    func_800A812C(mode);
}

void func_800A8574(void) {
    D_800AB320.unk3 = 1;
    func_800A880C();
}

INCLUDE_ASM("asm/us/ovl_14/nonmatchings/ovl_14", func_800A859C);

void func_800A8618(void) {
    func_800A8FDC();
}

void func_800A8638(void) {
    func_8002C308(0);
    if (D_800AB320.unk8 == 0) {
        func_800A86D8();
        func_800A87AC();
        func_800A87E4();
    } else {
        func_800A9180();
    }
}

void func_800A8694(void) {
    func_8002C4B0(0);
    func_800A8DB0();
    if (D_800AB320.unk3 == 1) {
        func_80032120(4, 1);
    }
}

void func_800A86D8(void) {
    if (func_8002F810() == 0) {
        if (D_800AB320.unk1 != 0 && D_800AB320.unk2 == 0) {
            if (D_80077528 & 0x10) {
                func_8002F488(0x67);
                D_800AB320.unk3 = D_800AB320.unk3 != 0 ? D_800AB320.unk3 - 1 : 1;
            } else if (D_80077528 & 0x40) {
                func_8002F488(0x67);
                D_800AB320.unk3 = D_800AB320.unk3 == 0 ? D_800AB320.unk3 + 1 : 0;
            } else if (D_80077528 & 0x20008) {
                func_800A8A0C();
            }
        }
    }
}

void func_800A87AC(void) {
    func_800A8A8C();
    func_800A8A94();
    func_800A8B7C();
    func_800A8C60();
}

void func_800A87E4(void) {
    func_800A8E10();
    func_800A8CF8();
}

void func_800A880C(void) {
    FileEntry *e;

    e = func_8001E218(1, 0x3C);
    func_80022DF0(D_8006794C->unk20 + e->unk4, (e->unkC - e->unk4) << 11, (void *)0x801CDF00, e->unk4);
    func_800A8868((void *)0x801CDF00);
}

INCLUDE_ASM("asm/us/ovl_14/nonmatchings/ovl_14", func_800A8868);

void func_800A8A0C(void) {
    if (D_800AB320.unk3 == 1 && func_800A8EA4() == 0) {
        func_8002F488(0x66);
        return;
    }
    func_8002F488(D_800AB320.unk3 == 0 ? 0xC18 : 0x67);
    func_8002FAAC(0x10);
    func_8002F8A8(func_800A8FBC);
}

void func_800A8A8C(void) {
}

void func_800A8A94(void) {
    s32 i;

    for (i = 0; i < 10; i++) {
        func_80027710(i << 5, 0, 10, &D_800AB320.unk24[i], 0);
    }
    func_80027710(D_800AB298.x, D_800AB298.y, 5, &D_800AB320.unk24[10], 1);
    if (D_800AB320.unk1 != 0) {
        if (D_800AB320.unk2 == 0) {
            func_80027710(D_800AB29C.x, D_800AB29C.y, 5, &D_800AB320.unk24[13], 0);
        } else {
            func_800A903C(D_800AB29C.x, D_800AB29C.y, 5, &D_800AB320.unk24[13], 0);
        }
    }
}

void func_800A8B7C(void) {
    if (D_800AB320.unk1 != 0) {
        if (D_800AB320.unk2 == 0) {
            func_80027710(D_800AB2A0.x, D_800AB2A0.y, 5, &D_800AB320.unk24[11], 0);
            func_80027710(D_800AB2A4.x, D_800AB2A4.y, 5, &D_800AB320.unk24[12], func_800A8EA4() == 0);
        } else {
            func_800A903C(D_800AB2A0.x, D_800AB2A0.y, 5, &D_800AB320.unk24[11], 0);
            func_800A903C(D_800AB2A4.x, D_800AB2A4.y, 5, &D_800AB320.unk24[12], func_800A8EA4() == 0);
        }
    }
}

void func_800A8C60(void) {
    if (func_8002F810() == 0 && D_800AB320.unk1 != 0 && D_800AB320.unk2 == 0) {
        D_80077DE8.unk0 = D_800AB2A8[D_800AB320.unk3].x - 0x1A;
        D_80077DE8.unk2 = D_800AB2A8[D_800AB320.unk3].y + 3;
        D_80077DE8.unk4 = 3;
        func_800355E4(&D_80077DE8);
    }
}

void func_800A8CF8(void) {
    if (++D_800AB320.unkC > 0x30) {
        D_800AB320.unkC = 0;
        D_800AB320.unk2 = 0;
    }
}

void func_800A8D2C(void) {
    s32 i;
    s32 h;

    func_80012D08();
    for (i = 0; i < 4; i++) {
        h = func_80016EC0(0xF4000001, D_800AB2B4[i], 0x2000, 0);
        D_800AB320.unk14[i] = h;
        func_80016ED0(h);
    }
    func_80012D18();
}

void func_800A8DB0(void) {
    s32 i;

    func_80012D08();
    for (i = 0; i < 4; i++) {
        func_80056CBC(D_800AB320.unk14[i]);
        func_80056CAC(D_800AB320.unk14[i]);
    }
    func_80012D18();
}

void func_800A8E10(void) {
    s32 r;

    if (D_800AB320.unk7 == 0) {
        func_800A8ED8();
        D_800AB320.unk7 = 1;
    }
    r = func_800A8F44();
    if (r != -1) {
        D_800AB320.unk4[D_800AB320.unk6] = (r == 0 || r == 3) ? 1 : 0;
        D_800AB320.unk7 = 0;
        D_800AB320.unk6 ^= 1;
    }
}

u8 func_800A8EA4(void) {
    s32 i;
    u8 r = 0;

    for (i = 0; i < 2; i++) {
        r |= D_800AB320.unk4[i];
    }
    return r;
}

void func_800A8ED8(void) {
    s32 i;

    for (i = 0; i < 4; i++) {
        func_80065208(D_800AB320.unk14[i]);
    }
    func_80065228(0);
    func_80065228(1);
    func_80065218(D_800AB320.unk6 << 4);
}

s32 func_800A8F44(void) {
    s32 i;
    s32 r = -1;

    for (i = 0; i < 4; i++) {
        if (func_80065208(D_800AB320.unk14[i]) == 1) {
            r = i;
            break;
        }
    }
    return r;
}

void func_800A8FBC(void) {
    D_800AB320.unk8 = 1;
}

s32 func_800A8FCC(void) {
    return D_800AB320.unk3;
}

void func_800A8FDC(void) {
    if (func_800A832C() == 0) {
        D_800AB320.unk1 = 1;
        D_800AB320.unk2 = 1;
        D_800AB320.unkC = 0;
    } else {
        D_800AB320.unk1 = 1;
        D_800AB320.unk2 = 0;
        D_800AB320.unkC = 0;
        func_8002FA50(0x20);
    }
}

void func_800A903C(s32 x, s32 y, s32 z, Ovl14Text *t, s32 arg4) {
    SprtWords *p;
    s32 c = D_800AB320.unkC;

    setlen(D_80077170, 4);
    p = (SprtWords *)D_80077170;
    p->rgbc = (c << 1) | (c << 9) | (c << 17) | 0x66000000;
    p->xy = x | (y << 16);
    p->uvclut = t->unk0;
    p->wh = t->unk4 | (t->unk5 << 16);
    D_80077170 = (u8 *)(p + 1);
    addPrim(&D_8007716C[z], p);
    SetDrawTPage(D_80077170, 1, 1, t->unk6 | 0x20);
    addPrim(&D_8007716C[z], D_80077170);
    D_80077170 += sizeof(SprtWords); /* sic: the DR_TPAGE is only 8 bytes */
}

void func_800A9180(void) {
    s32 r;

    switch (D_800AB320.unk3) {
    case 0:
        D_8006794C->unkB = 1;
        r = func_800A81B8();
        if (r != 0xFF) {
            func_80032120(8, ((r & 3) << 14) | 0x3A98);
            func_800A812C(3);
        } else {
            func_800A812C(4);
        }
        break;
    case 1:
        func_800A812C(2);
        func_80032120(4, 1);
        break;
    }
}

INCLUDE_ASM("asm/us/ovl_14/nonmatchings/ovl_14", func_800A9220);

INCLUDE_ASM("asm/us/ovl_14/nonmatchings/ovl_14", func_800A9968);

void func_800A9B14(void) {
    GameData *ctx = D_8006794C->unk1C;
    ClearOTagR(ctx->unk10[ctx->unk8], 0x1000);
}

INCLUDE_ASM("asm/us/ovl_14/nonmatchings/ovl_14", func_800A9B58);

void func_800A9DD4(void) {
    func_8019A9D8();
    func_80022B28();
    D_800AB2C4 |= 0x80;
    D_800AB3C8 = 3;
}

INCLUDE_ASM("asm/us/ovl_14/nonmatchings/ovl_14", func_800A9E14);

s32 func_800AA848(s32 idx) {
    u8 *buf;
    FileEntry *e;

    while (func_800217C4(D_8006794C->unk1C->unk854) != 0) {
    }
    e = func_8001E218(1, D_800AB2D8[idx]);
    func_8002177C(D_8006794C->unk1C->unk854, e->unk4, (e->unkC - e->unk4) << 11, D_80067944 + 0x10000, 0, -1, 0);
    while (func_800217C4(D_8006794C->unk1C->unk854) != 0) {
    }
    buf = D_80067944 + 0x10000;
    D_800AB2D4 = buf;
    func_8001C750((s32)buf, 0);
    func_800320B8(1, 1, 0, 0xFFFFFF);
    D_800AB440 = 0;
    D_800AB2D0 = 0;
    D_80073998[0] = 0;
    return 0;
}

s32 func_800AA960(void) {
    func_8001C784(D_800AB2D4);
    D_800AB2D4 = NULL;
    return 0;
}

void func_800AA994(void) {
    GameData *ctx = D_8006794C->unk1C;
    ClearOTagR(ctx->unk10[ctx->unk8], 0x1000);
}

void func_800AA9D8(void) {
    GameData *ctx;
    u8 i;

    DrawSync(0);
    D_8006794C->unk9 = 0;
    VSync(D_8006794C->unk9);
    ResetGraph(1);
    PutDispEnv(&D_8006794C->unkC[D_8006794C->unk1C->unk8]);
    ctx = D_8006794C->unk1C;
    i = ctx->unk8;
    DrawOTagEnv(ctx->unk10[i] + 0xFFF, &D_8006794C->unk10[i]);
    D_8006794C->unk1C->unk8 ^= 1;
    func_8002C0DC(D_8006794C->unk1C->unk8);
}

INCLUDE_ASM("asm/us/ovl_14/nonmatchings/ovl_14", func_800AAADC);

void func_800AAF40(void) {
    u8 *buf;
    FileEntry *e;

    while (func_800217C4(D_8006794C->unk1C->unk854) != 0) {
    }
    e = func_8001E218(1, 0x2718);
    func_8002177C(D_8006794C->unk1C->unk854, e->unk4, (e->unkC - e->unk4) << 11, D_80067944 + 0x10000, 0, -1, 0);
    while (func_800217C4(D_8006794C->unk1C->unk854) != 0) {
    }
    buf = D_80067944 + 0x10000;
    D_800AB2F0 = buf;
    func_8001C750((s32)buf, 0);
    D_800AB2F4 = (s32)func_8001C7FC(4, 0x44FB0, 0);
    func_8001D9CC(D_800AB2F4, 0, 0xE0, 0x280, 0);
    func_8001D8A8(D_800AB2F4);
    func_800320B8(1, 1, 0, 0xFFFFFF);
    D_80073998[0] = 0;
}

void func_800AB06C(void) {
    func_8001C784(D_800AB2F0);
    D_800AB2F0 = NULL;
}

void func_800AB09C(void) {
    GameData *ctx = D_8006794C->unk1C;
    ClearOTagR(ctx->unk10[ctx->unk8], 0x1000);
}

void func_800AB0E0(void) {
    GameData *ctx;
    u8 i;

    DrawSync(0);
    D_8006794C->unk9 = 0;
    VSync(D_8006794C->unk9);
    ResetGraph(1);
    PutDispEnv(&D_8006794C->unkC[D_8006794C->unk1C->unk8]);
    ctx = D_8006794C->unk1C;
    i = ctx->unk8;
    DrawOTagEnv(ctx->unk10[i] + 0xFFF, &D_8006794C->unk10[i]);
    D_8006794C->unk1C->unk8 ^= 1;
    func_8002C0DC(D_8006794C->unk1C->unk8);
}

void func_800AB1E4(void) {
    D_8006794C->unk1C->unk890 = 1;
}

void func_800AB200(void) {
    GameData *ctx = D_8006794C->unk1C;
    if (ctx->unk890 == 0) {
        ctx->unk890 = 2;
    }
}
