#include "ovl_14.h"

INCLUDE_RODATA("asm/us/ovl_14/nonmatchings/ovl_14", D_800A7000);

INCLUDE_ASM("asm/us/ovl_14/nonmatchings/ovl_14", func_800A7088);

void func_800A727C(void) {
    Ovl14Ctx *ctx = G14->unk1C;
    ClearOTagR(ctx->unk10[ctx->unk8], 0x1000);
}

void func_800A72C0(void) {
    Ovl14Ctx *ctx;
    u8 i;

    DrawSync(0);
    VSync(G14->unk9);
    ResetGraph(1);
    PutDispEnv(&G14->unkC[G14->unk1C->unk8]);
    ctx = G14->unk1C;
    i = ctx->unk8;
    DrawOTagEnv(ctx->unk10[i] + 0xFFF, &G14->unk10[i]);
    G14->unk1C->unk8 ^= 1;
    func_8002C0DC(G14->unk1C->unk8);
}

void func_800A73B8(void) {
    Ovl14Ctx *ctx = G14->unk1C;
    func_8001CCB4(0, 0, 0);
    G14->unk8 = 1;
    G14->unk9 = 2;
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
    Ovl14Ctx *ctx = G14->unk1C;

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
    if (G14->unk1C->unk0 & 0x100000) {
        func_800551F0();
        func_800A7768();
        G14->unk1C->unk0 &= ~0x100000;
    }
    func_8002306C();
    G14->unk1C->unk83C = func_800A799C;
    G14->unk1C->unk84C = 0;
    func_800A78C0();
    func_800A780C();
    func_800A796C();
    func_800239D0();
    SetDispMask(1);
}

void func_800A7768(void) {
    s32 i;
    Ovl14Ctx *ctx = G14->unk1C;

    for (i = 1; i >= 0; i--) {
        ctx->unk81C[i] = 0xFFFF;
        ctx->unk820[i] = 0xFFFF;
        ctx->unk824[i] = 0;
        ctx->unk826[i] = 0x7F;
    }
    ctx->unk0 &= 0xBFFFFF;
}

void func_800A77BC(void) {
    Ovl14Ctx *ctx;

    SetDispMask(0);
    ctx = G14->unk1C;
    if (ctx->unk0 & 0x20000000) {
        ctx->unk0 |= 0x400000;
    }
}

void func_800A780C(void) {
    s32 a;
    s32 b;

    a = func_8001C7FC(4, 0x7003A, 0);
    memmove(G14->unk1C->unk814, (void *)a, func_8001C7FC(5, 0x7003A, 0));
    a = func_8001C7FC(4, 0x90122, 0);
    b = func_8001C7FC(5, 0x90122, 0);
    func_80055DF4();
    func_80055E14(a, b, 1);
}

void func_800A78C0(void) {
    u8 *buf;
    Ovl14FileEntry *e;

    buf = D_80067944 + 0x10000;
    e = func_8001E218(1, 0x2717);
    func_8002177C(G14->unk1C->unk854, e->unk4, (e->unkC - e->unk4) << 11, buf, 0, -1, 0);
    while (func_800217C4(G14->unk1C->unk854) != 0) {
    }
    D_800AB230 = buf;
    func_8001C750(buf, 0);
}

void func_800A796C(void) {
    func_8001C784(D_800AB230);
    D_800AB230 = NULL;
}

INCLUDE_ASM("asm/us/ovl_14/nonmatchings/ovl_14", func_800A799C);

void func_800A7F70(s32 arg0, s32 arg1) {
    Ovl14Ctx *ctx = G14->unk1C;

    ctx->unk826[0] = arg0;
    if (arg0 == 0) {
        ctx->unk842 = arg1;
        ctx->unk8C4 = VSync(-1);
    }
}

s32 func_800A7FB0(void) {
    Ovl14Ctx *ctx = G14->unk1C;
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

u8 func_800A811C(void) {
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

INCLUDE_ASM("asm/us/ovl_14/nonmatchings/ovl_14", func_800A81B8);

INCLUDE_ASM("asm/us/ovl_14/nonmatchings/ovl_14", func_800A8260);

INCLUDE_ASM("asm/us/ovl_14/nonmatchings/ovl_14", func_800A8298);

INCLUDE_ASM("asm/us/ovl_14/nonmatchings/ovl_14", func_800A82B0);

INCLUDE_ASM("asm/us/ovl_14/nonmatchings/ovl_14", func_800A82F4);

INCLUDE_ASM("asm/us/ovl_14/nonmatchings/ovl_14", func_800A832C);

void func_800A833C(void) {
}

INCLUDE_ASM("asm/us/ovl_14/nonmatchings/ovl_14", func_800A8344);

void func_800A8368(void) {
}

void func_800A8370(void) {
}

INCLUDE_ASM("asm/us/ovl_14/nonmatchings/ovl_14", func_800A8378);

void func_800A8398(void) {
}

INCLUDE_ASM("asm/us/ovl_14/nonmatchings/ovl_14", func_800A83A0);

INCLUDE_ASM("asm/us/ovl_14/nonmatchings/ovl_14", func_800A83AC);

INCLUDE_ASM("asm/us/ovl_14/nonmatchings/ovl_14", func_800A83E8);

void func_800A8448(void) {
}

INCLUDE_ASM("asm/us/ovl_14/nonmatchings/ovl_14", func_800A8450);

INCLUDE_ASM("asm/us/ovl_14/nonmatchings/ovl_14", func_800A847C);

void func_800A84A8(void) {
}

INCLUDE_ASM("asm/us/ovl_14/nonmatchings/ovl_14", func_800A84B0);

INCLUDE_ASM("asm/us/ovl_14/nonmatchings/ovl_14", func_800A84EC);

INCLUDE_ASM("asm/us/ovl_14/nonmatchings/ovl_14", func_800A8574);

INCLUDE_ASM("asm/us/ovl_14/nonmatchings/ovl_14", func_800A859C);

INCLUDE_ASM("asm/us/ovl_14/nonmatchings/ovl_14", func_800A8618);

INCLUDE_ASM("asm/us/ovl_14/nonmatchings/ovl_14", func_800A8638);

INCLUDE_ASM("asm/us/ovl_14/nonmatchings/ovl_14", func_800A8694);

INCLUDE_ASM("asm/us/ovl_14/nonmatchings/ovl_14", func_800A86D8);

INCLUDE_ASM("asm/us/ovl_14/nonmatchings/ovl_14", func_800A87AC);

INCLUDE_ASM("asm/us/ovl_14/nonmatchings/ovl_14", func_800A87E4);

INCLUDE_ASM("asm/us/ovl_14/nonmatchings/ovl_14", func_800A880C);

INCLUDE_ASM("asm/us/ovl_14/nonmatchings/ovl_14", func_800A8868);

INCLUDE_ASM("asm/us/ovl_14/nonmatchings/ovl_14", func_800A8A0C);

void func_800A8A8C(void) {
}

INCLUDE_ASM("asm/us/ovl_14/nonmatchings/ovl_14", func_800A8A94);

INCLUDE_ASM("asm/us/ovl_14/nonmatchings/ovl_14", func_800A8B7C);

INCLUDE_ASM("asm/us/ovl_14/nonmatchings/ovl_14", func_800A8C60);

INCLUDE_ASM("asm/us/ovl_14/nonmatchings/ovl_14", func_800A8CF8);

INCLUDE_ASM("asm/us/ovl_14/nonmatchings/ovl_14", func_800A8D2C);

INCLUDE_ASM("asm/us/ovl_14/nonmatchings/ovl_14", func_800A8DB0);

INCLUDE_ASM("asm/us/ovl_14/nonmatchings/ovl_14", func_800A8E10);

INCLUDE_ASM("asm/us/ovl_14/nonmatchings/ovl_14", func_800A8EA4);

INCLUDE_ASM("asm/us/ovl_14/nonmatchings/ovl_14", func_800A8ED8);

INCLUDE_ASM("asm/us/ovl_14/nonmatchings/ovl_14", func_800A8F44);

INCLUDE_ASM("asm/us/ovl_14/nonmatchings/ovl_14", func_800A8FBC);

INCLUDE_ASM("asm/us/ovl_14/nonmatchings/ovl_14", func_800A8FCC);

INCLUDE_ASM("asm/us/ovl_14/nonmatchings/ovl_14", func_800A8FDC);

INCLUDE_ASM("asm/us/ovl_14/nonmatchings/ovl_14", func_800A903C);

INCLUDE_ASM("asm/us/ovl_14/nonmatchings/ovl_14", func_800A9180);

INCLUDE_ASM("asm/us/ovl_14/nonmatchings/ovl_14", func_800A9220);

INCLUDE_ASM("asm/us/ovl_14/nonmatchings/ovl_14", func_800A9968);

INCLUDE_ASM("asm/us/ovl_14/nonmatchings/ovl_14", func_800A9B14);

INCLUDE_ASM("asm/us/ovl_14/nonmatchings/ovl_14", func_800A9B58);

INCLUDE_ASM("asm/us/ovl_14/nonmatchings/ovl_14", func_800A9DD4);

INCLUDE_ASM("asm/us/ovl_14/nonmatchings/ovl_14", func_800A9E14);

INCLUDE_ASM("asm/us/ovl_14/nonmatchings/ovl_14", func_800AA848);

INCLUDE_ASM("asm/us/ovl_14/nonmatchings/ovl_14", func_800AA960);

INCLUDE_ASM("asm/us/ovl_14/nonmatchings/ovl_14", func_800AA994);

INCLUDE_ASM("asm/us/ovl_14/nonmatchings/ovl_14", func_800AA9D8);

INCLUDE_ASM("asm/us/ovl_14/nonmatchings/ovl_14", func_800AAADC);

INCLUDE_ASM("asm/us/ovl_14/nonmatchings/ovl_14", func_800AAF40);

INCLUDE_ASM("asm/us/ovl_14/nonmatchings/ovl_14", func_800AB06C);

INCLUDE_ASM("asm/us/ovl_14/nonmatchings/ovl_14", func_800AB09C);

INCLUDE_ASM("asm/us/ovl_14/nonmatchings/ovl_14", func_800AB0E0);

INCLUDE_ASM("asm/us/ovl_14/nonmatchings/ovl_14", func_800AB1E4);

INCLUDE_ASM("asm/us/ovl_14/nonmatchings/ovl_14", func_800AB200);
