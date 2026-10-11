#include "ovl_14.h"

INCLUDE_RODATA("asm/us/ovl_14/nonmatchings/ovl_14", D_800A7000);

void func_800A7088(void) {
    s32 n;

    n = 0;
    func_800A767C();
    func_800A7FF0();
    D_800AB300 = D_80067950 == 0 ? 6 : 0;
    while (D_800AB300 < 7) {
        switch (D_800AB300) {
        case 0:
            if (func_800A9220() != 0) {
                D_800AB300 = 7;
            } else {
                D_800AB300 = 1;
            }
            break;
        case 1:
            if (func_800A9E14(0) != 0) {
                D_800AB300 = 7;
            } else if (g_gameState->unkB != 0xFF && (g_gameState->unkB & 1)) {
                n = 180;
                D_800AB300 = 2;
            } else {
                D_800AB300 = 4;
            }
            break;
        case 2:
            VSync(0);
            if (--n == 0) {
                D_800AB300 = 3;
            }
            break;
        case 3:
            if (func_800A9220() == 0) {
                D_800AB300 = 4;
            } else {
                D_800AB300 = 7;
            }
            break;
        case 4:
            if (func_800A9E14(1) != 0) {
                D_800AB300 = 7;
            } else if (g_gameState->unkB != 0xFF && (g_gameState->unkB & 1)) {
                n = 180;
                D_800AB300 = 5;
            } else {
                D_800AB300 = 1;
            }
            break;
        case 5:
            VSync(0);
            if (--n == 0) {
                D_800AB300 = 0;
            }
            break;
        case 6:
            func_800AAADC();
            D_800AB300 = 0;
            break;
        }
    }
    g_gameState->unk0 |= 0x100000;
    func_8001DE50(g_gameState);
    func_800A741C();
    func_800A80D4();
    func_800A77BC();
}

void func_800A727C(void) {
    GameData *ctx = g_gameState->unk1C;
    ClearOTagR(ctx->unk10[ctx->unk8], 0x1000);
}

void func_800A72C0(void) {
    GameData *ctx;
    u8 i;

    DrawSync(0);
    VSync(g_gameState->unk9);
    ResetGraph(1);
    PutDispEnv(&g_gameState->unkC[g_gameState->unk1C->unk8]);
    ctx = g_gameState->unk1C;
    i = ctx->unk8;
    DrawOTagEnv(ctx->unk10[i] + 0xFFF, &g_gameState->unk10[i]);
    g_gameState->unk1C->unk8 ^= 1;
    func_8002C0DC(g_gameState->unk1C->unk8);
}

void func_800A73B8(void) {
    GameData *ctx = g_gameState->unk1C;
    func_8001CCB4(0, 0, 0);
    g_gameState->unk8 = 1;
    g_gameState->unk9 = 2;
    ctx->unkC = 0;
    ctx->unk3A = 1;
}

/* looks up the current area/field ids in the two directories at D_80067944 + 0x14C700 and stores their sector ranges */
void func_800A741C(void) {
    GameState *gs = g_gameState;
    s32 mode = gs->unk8;
    GameData *ctx = gs->unk1C;
    GameStateUnk14 *f;
    s32 base;
    u8 *area;
    u8 *start;
    Ovl14Dir *e;
    s32 key;
    s32 id;

    if (mode == 1 || mode == 3) {
        id = -1;
        f = gs->unk14;
        base = f->unk8;
        area = D_80067944 + ((base << 11) + 0x14C700);
        if (mode == 1) {
            key = (s16)ctx->unkC;
            start = area + ((f->unk48 - base) << 11);
            e = (Ovl14Dir *)start + f->unk44 - 1;
        } else {
            key = ctx->unk7DC;
            start = area + ((f->unk38 - base) << 11);
            e = (Ovl14Dir *)start + f->unk34 - 1;
        }
        for (;;) {
            if (e->unk0 == key) {
                id = e->unk2;
                ctx->unk7FC[1] = e->unk4;
                ctx->unk804[1] = e[1].unk4 - e->unk4;
                break;
            }
            e--;
            if ((u8 *)e < start) {
                break;
            }
        }
        start = area + ((gs->unk14->unk28 - base) << 11);
        e = (Ovl14Dir *)start + gs->unk14->unk24 - 1;
        for (;;) {
            if (e->unk0 == id) {
                ctx->unk7FC[0] = e->unk4;
                ctx->unk804[0] = e[1].unk4 - e->unk4;
                break;
            }
            e--;
            if ((u8 *)e < start) {
                break;
            }
        }
        if (mode == 1) {
            ctx->unk3A = id;
        } else {
            ctx->unk7DE = id;
        }
    }
}

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
    GameData *ctx = g_gameState->unk1C;

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
    if (g_gameState->unk1C->unk0 & 0x100000) {
        func_800551F0();
        func_800A7768();
        g_gameState->unk1C->unk0 &= ~0x100000;
    }
    func_8002306C();
    g_gameState->unk1C->unk83C = (void (*)(s32, s32, s32, s32, s32))func_800A799C;
    g_gameState->unk1C->unk84C = 0;
    func_800A78C0();
    func_800A780C();
    func_800A796C();
    func_800239D0();
    SetDispMask(1);
}

void func_800A7768(void) {
    s32 i;
    GameData *ctx = g_gameState->unk1C;

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
    ctx = g_gameState->unk1C;
    if (ctx->unk0 & 0x20000000) {
        ctx->unk0 |= 0x400000;
    }
}

void func_800A780C(void) {
    s32 a;
    s32 b;

    a = (s32)func_8001C7FC(4, 0x7003A, 0);
    memmove(g_gameState->unk1C->unk814, (void *)a, (s32)func_8001C7FC(5, 0x7003A, 0));
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
    func_8002177C(g_gameState->unk1C->unk854, e->unk4, (e->unkC - e->unk4) << 11, buf, 0, -1, 0);
    while (func_800217C4(g_gameState->unk1C->unk854) != 0) {
    }
    D_800AB230 = buf;
    func_8001C750((s32)buf, 0);
}

void func_800A796C(void) {
    func_8001C784(D_800AB230);
    D_800AB230 = NULL;
}

s32 func_800A799C(s32 arg0, s32 arg1, s32 arg2, s32 arg3, s32 arg4) {
    GameData *g;
    s32 kind;
    s32 h;
    s32 id;

    kind = (arg0 >> 8) & 0xF;
    switch ((arg0 >> 12) & 3) {
    case 0:
        id = (arg1 & 0xFFFF) | 0x70000;
        switch (kind) {
        case 0:
            if (g_gameState->unk1C->unk851 != 0) {
                return -1;
            }
            if (D_800AB234 == -1) {
                D_800AB234 = kind;
            }
            if (g_gameState->unk1C->unk81C[0] != arg1) {
                if (g_gameState->unk1C->unk0 & 0x20000000) {
                    func_800552B8(g_gameState->unk1C->unk824[0]);
                }
                g_gameState->unk1C->unk0 &= 0xCFFFFFFF;
                g_gameState->unk1C->unk820[0] = 0x122;
                g_gameState->unk1C->unk81C[0] = arg1;
                g_gameState->unk1C->unk826[0] = 0x7F;
            }
            g_gameState->unk1C->unk824[0] = func_80055290(g_gameState->unk1C->unk814);
            g = g_gameState->unk1C;
            if (!(g->unk0 & 0x20000000)) {
                func_80055A34(g->unk824[0], 0xA, 0, g->unk826[0]);
            }
            D_800AB234 = -1;
            g_gameState->unk1C->unk0 |= 0x30000000;
            return 0;
        case 1:
            if (g_gameState->unk1C->unk81C[0] == (u16)id) {
                func_800552B8(g_gameState->unk1C->unk824[0]);
                g_gameState->unk1C->unk826[0] = 0x7F;
                g_gameState->unk1C->unk0 &= 0xDFFFFFFF;
            }
            return 0;
        case 2:
            if ((arg0 & 0x3F) != 1) {
                break;
            }
            func_800559C8(g_gameState->unk1C->unk824[0], arg2);
            g_gameState->unk1C->unk826[0] = arg2;
            return 0;
        case 3:
            if ((arg0 & 0x3F) != 1) {
                break;
            }
            func_800559FC(g_gameState->unk1C->unk824[0], arg2, arg3);
            func_800A7F70(arg3, arg2);
            return 0;
        case 4:
            if ((arg0 & 0x3F) != 1) {
                break;
            }
            func_80055A34(g_gameState->unk1C->unk824[0], arg2, arg3, arg4);
            func_800A7F70(arg4, arg2);
            return 0;
        case 6:
            if ((arg0 & 0x3F) != 1) {
                break;
            }
            func_800559FC(0, arg2, arg3);
            func_800A7F70(arg3, arg2);
            return 0;
        case 11:
            if ((arg0 & 0x3F) != 1) {
                break;
            }
            func_80055A34(0, arg2, arg3, arg4);
            func_800A7F70(arg4, arg2);
            return 0;
        }
        return -1;
    case 1:
        id = (arg1 & 0xFFFF) | 0x80000;
        h = 0;
        if (kind != 5 && kind != 6) {
            h = (s32)func_8001C7FC(4, id, 0);
        }
        switch (kind) {
        case 0:
            func_800553EC(h, arg2, arg3, arg4);
            return 0;
        case 1:
            func_8005546C(h, arg2);
            return 0;
        case 2:
            switch (arg0 & 0x3F) {
            case 1:
                func_80055764(h, arg2, arg3);
                return 0;
            case 5:
                func_80055850(h, arg2, arg3);
                return 0;
            case 3:
                func_8005593C(h, arg2, arg3);
                return 0;
            }
            return -1;
        case 3:
            switch (arg0 & 0x3F) {
            case 1:
                func_800557A8(h, arg2, arg3, arg4);
                return 0;
            case 5:
                func_80055894(h, arg2, arg3, arg4);
                return 0;
            case 3:
                func_80055980(h, arg2, arg3, arg4);
                return 0;
            }
            return -1;
        case 5:
            switch (arg0 & 0x3F) {
            case 1:
                func_80055704(arg2);
                return 0;
            case 5:
                func_800557F0(arg2);
                return 0;
            case 3:
                func_800558DC(arg2);
                return 0;
            }
            return -1;
        case 6:
            switch (arg0 & 0x3F) {
            case 1:
                func_80055730(arg2, arg3);
                return -1;
            case 5:
                func_8005581C(arg2, arg3);
                return -1;
            case 3:
                func_80055908(arg2, arg3);
                return -1;
            }
            return -1;
        }
        return -1;
    }
    return -1;
}

void func_800A7F70(s32 arg0, s32 arg1) {
    GameData *ctx = g_gameState->unk1C;

    ctx->unk826[0] = arg0;
    if (arg0 == 0) {
        ctx->unk842 = arg1;
        ctx->unk8C4 = VSync(-1);
    }
}

s32 func_800A7FB0(void) {
    GameData *ctx = g_gameState->unk1C;
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
    prev = g_gameState->unkB;
    do {
        func_8001DCB8();
    } while (g_gameState->unkB == 0xFF && ++i < 10);
    func_80033D1C(1);
    if (func_800A8298(prev, g_gameState->unkB)) {
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
        g_gameState->unk8 = 1;
        g_gameState->unk1C->unkC = 0x46;
        g_gameState->unk1C->unk3A = 2;
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

void func_800A859C(void) {
    s32 i;

    func_80032574();
    func_8002C4B0(1);
    D_800AB320.unk8 = 0;
    D_800AB320.unk0 = 1;
    D_800AB320.unk7 = 0;
    D_800AB320.unk6 = 0;
    D_800AB320.unk1 = 0;
    D_800AB320.unk2 = 0;
    D_800AB320.unkC = 0;
    D_800AB320.unk10 = 0;
    for (i = 0; i < 2; i++) {
        D_800AB320.unk4[i] = 0;
    }
    func_800A8D2C();
    func_800A8FDC();
}

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
            if (g_padRepeat & 0x10) {
                snd_playSe(0x67);
                D_800AB320.unk3 = D_800AB320.unk3 != 0 ? D_800AB320.unk3 - 1 : 1;
            } else if (g_padRepeat & 0x40) {
                snd_playSe(0x67);
                D_800AB320.unk3 = D_800AB320.unk3 == 0 ? D_800AB320.unk3 + 1 : 0;
            } else if (g_padRepeat & 0x20008) {
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
    func_80022DF0(g_gameState->unk20 + e->unk4, (e->unkC - e->unk4) << 11, (void *)0x801CDF00, e->unk4);
    func_800A8868((s32 *)0x801CDF00);
}

void func_800A8868(s32 *pack) {
    RECT *clut;
    RECT *img;
    TmTimLike *tim;
    s32 i;
    s32 n;
    u32 mode;
    s32 sh;
    s32 u;
    s32 tp;
    s32 yb;
    s32 xb;
    s32 y2;
    s32 *p;

    n = pack[0];
    p = pack + 1;
    for (i = 0; i < n; i++) {
        tim = (TmTimLike *)((u8 *)pack + p[i]);
        func_8001D8A8((s32)tim);
        func_8001D910(tim, &clut, &img);
        u = img->x & 0x3F;
        mode = tim->unk4 & 3;
        sh = 2 - mode;
        D_800AB320.unk24[i].unk0.s.u = u << sh;
        D_800AB320.unk24[i].unk0.s.v = img->y;
        D_800AB320.unk24[i].unk4 = img->w << sh;
        D_800AB320.unk24[i].unk5 = img->h;
        tp = mode << 7;
        yb = (s16)((s16)(img->y & 0x100) >> 4);
        xb = (img->x & 0x3FF) >> 6;
        y2 = (s16)(img->y & 0x200) << 2;
        D_800AB320.unk24[i].unk6 = ((i == 10) ? (tp | (yb | 0x20)) : (tp | yb)) | xb | y2;
        if (mode < 2) {
            D_800AB320.unk24[i].unk0.s.clut = (clut->y << 6) | ((clut->x >> 4) & 0x3F);
        } else {
            D_800AB320.unk24[i].unk0.s.clut = 0;
        }
        DrawSync(0);
    }
}

void func_800A8A0C(void) {
    if (D_800AB320.unk3 == 1 && func_800A8EA4() == 0) {
        snd_playSe(0x66);
        return;
    }
    snd_playSe(D_800AB320.unk3 == 0 ? 0xC18 : 0x67);
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
        h = OpenEvent(0xF4000001, D_800AB2B4[i], 0x2000, 0);
        D_800AB320.unk14[i] = h;
        EnableEvent(h);
    }
    func_80012D18();
}

void func_800A8DB0(void) {
    s32 i;

    func_80012D08();
    for (i = 0; i < 4; i++) {
        DisableEvent(D_800AB320.unk14[i]);
        CloseEvent(D_800AB320.unk14[i]);
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

    setlen(g_primPtr, 4);
    p = (SprtWords *)g_primPtr;
    p->rgbc = (c << 1) | (c << 9) | (c << 17) | 0x66000000;
    p->xy = x | (y << 16);
    p->uvclut = t->unk0.w;
    p->wh = t->unk4 | (t->unk5 << 16);
    g_primPtr = (u8 *)(p + 1);
    addPrim(&g_ot[z], p);
    SetDrawTPage(g_primPtr, 1, 1, t->unk6 | 0x20);
    addPrim(&g_ot[z], g_primPtr);
    g_primPtr += sizeof(SprtWords); /* sic: the DR_TPAGE is only 8 bytes */
}

void func_800A9180(void) {
    s32 r;

    switch (D_800AB320.unk3) {
    case 0:
        g_gameState->unkB = 1;
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

s32 func_800A9220(void) {
    u8 *buf;
    s32 timer;
    s32 i;
    s32 r;
    s32 end;

    buf = D_80067944 + 0x106700;
    timer = -1;
    func_800A9968();
    do {
        func_800A9B14();
        func_80019194();
        switch (D_800AB3C8) {
        case 0:
            if (g_gameState->unkB != 0xFF && (g_gameState->unkB & 1)) {
                D_800AB2CC = 0;
                D_800AB2C4 = 1;
                D_800AB3C8 = 1;
                func_8002F884();
            } else {
                if (!(g_gameState->unk1C->unk0 & 0x20000000)) {
                    func_800A799C(0, 0x7003A, 0, 0, 0);
                    func_800A799C(0xC401, 0x7003A, 0x5A, 0, 0x7F);
                }
                D_800AB2CC = 1;
                D_800AB3C8 = 3;
                D_800AB2C4 |= 0x80;
                func_8002FAAC(1);
            }
            break;
        case 1:
            i = 1;
            if (g_gameState->unk1C->unk8 != 0) {
                i = 2;
            }
            D_800AB3B8[0][i - 1] = 7;
            D_800AB3B8[0][i] = 0x10;
            g_gameState->unk9 = 4;
            DrawSync(0);
            func_801999B4(1, &buf);
            func_8019A5B0(1);
            func_8019AC3C(func_800A9DD4);
            D_800AB2CE = D_80071E34;
            func_80199B28(1, 0);
            D_800AB3C8 = 2;
            break;
        case 2:
            if ((D_800AB2C4 & 0x10) && (func_80012AD4() & 2)) {
                r = func_80199F30(0);
                end = func_8019AD0C();
                if (r != 0) {
                    DrawOTag(g_gameState->unk1C->unk10[g_gameState->unk1C->unk8 ^ 1] + 0xFFF);
                } else if (timer == -1 && g_gameState->unk1C->unk8 == 0 && !(D_800AB2C4 & 0x20)) {
                    g_gameState->unkC[g_gameState->unk1C->unk8].disp.x = 0x1E0;
                }
                if ((D_80071E34 >= end || D_800AB2CE >= D_80071E34 || timer == 0) && !(D_800AB2C4 & 0x40)) {
                    if (timer == 0) {
                        func_8019AAF8();
                        func_80023018();
                        if (!(g_gameState->unk1C->unk0 & 0x20000000)) {
                            func_800A799C(0, 0x7003A, 0, 0, 0);
                            func_800A799C(0xC401, 0x7003A, 0x5A, 0, 0x7F);
                        }
                    }
                    if (D_800AB2CE >= D_80071E34) {
                        D_800AB2C4 &= ~7;
                        if (g_gameState->unk1C->unk8 == 0) {
                            g_gameState->unkC[g_gameState->unk1C->unk8].disp.x = 0x1E0;
                        }
                        printf("Fr: %d\n", D_800AB2CE);
                    } else {
                        D_800AB3B8[1][0] = 7;
                    }
                    D_800AB2C4 |= 0x40;
                }
                if (!(D_800AB2C4 & 0x100) && D_80071E34 > 0) {
                    func_800A799C(0, 0x7003A, 0, 0, 0);
                    D_800AB2C4 |= 0x100;
                }
                if (!(D_800AB2C4 & 0x20) && (g_padPortPressed[0] & 0x4008) && D_80071E34 < end) {
                    D_800AB2CC = 1;
                    func_8019A5B0(0);
                    D_800AB3B8[1][g_gameState->unk1C->unk8 != 0 ? 1 : 2] = 2;
                    timer = 6;
                    func_800320B8(1, 1, 0, 0);
                    func_800320B8(1, 1, timer, 0xFFFFFF);
                    D_80073998[0] = -1;
                    D_800AB2C4 |= 0x20;
                }
                D_800AB2CE = D_80071E34;
                if (!(D_800AB2C4 & 2) && timer != -1) {
                    timer--;
                }
            }
            break;
        case 3:
            if ((D_800AB2C4 & 0x80) && func_80022B18() == 0) {
                g_gameState->unk9 = 0;
                g_gameState->unkC[0].disp.x = 0x140;
                timer = 0x4EC;
                g_gameState->unkC[1].disp.x = 0;
                D_80073998[0] = 0;
                func_800A82F4(D_800AB2CC);
                func_800A812C(1);
                D_800AB3C8 = 4;
            }
            break;
        case 4:
            if (!(D_800AB2C4 & 0x1000) && (u8)func_800A82B0() && timer < 0x5B) {
                func_800320B8(1, 1, 0, 0);
                func_800320B8(1, 1, 0x5A, 0xFFFFFF);
                D_800AB2C4 |= 0x1000;
            }
            if (timer == 0 && D_800AB3C8 != 6) {
                D_800AB3C8 = 5;
            }
            if ((D_800AB2C4 & 0x1000) || (u8)func_800A82B0()) {
                timer--;
            }
            break;
        }
        if (!(D_800AB2C4 & 1)) {
            func_800A808C();
            if (func_800A811C() == 5) {
                D_800AB3C8 = 6;
            }
            func_80032100();
            func_80023A0C();
        }
        if (!(D_800AB2C4 & 2)) {
            func_80032098();
        }
        func_800A9B58();
    } while ((u32)D_800AB3C8 < 5);
    return D_800AB3C8 == 6;
}

s32 func_800A9968(void) {
    FileEntry *f;
    s32 h;
    s32 i, j;

    while (func_800217C4(g_gameState->unk1C->unk854) != 0) {
    }
    f = func_8001E218(0, 0x64);
    func_8002177C(g_gameState->unk1C->unk854, f->unk4, (f->unkC - f->unk4) << 11, D_80067944 + 0xF1700, 0, 0x15000, NULL);
    while (func_800217C4(g_gameState->unk1C->unk854) != 0) {
    }
    func_80012D08();
    FlushCache();
    func_80012D18();
    if (g_gameState->unkB != 0xFF && (g_gameState->unkB & 1)) {
        h = func_80022AC4(*D_8019DC68);
        func_80022ED4(h);
        while (func_80022B18() != 0) {
        }
        func_8019C98C(h);
    }
    func_800320B8(1, 1, 0, 0xFFFFFF);
    D_800AB3C8 = 0;
    D_800AB2C4 = 0;
    for (i = 1; i >= 0; i--) {
        for (j = 3; j >= 0; j--) {
            D_800AB3B8[i][j] = 0;
        }
    }
    D_800AB2C8 = 0;
    func_800A82F4(0);
    func_800A812C(1);
    D_80073998[0] = 0;
    return 0;
}

void func_800A9B14(void) {
    GameData *ctx = g_gameState->unk1C;
    ClearOTagR(ctx->unk10[ctx->unk8], 0x1000);
}

void func_800A9B58(void) {
    s32 i, j;
    GameData *g;

    DrawSync(0);
    if (func_80012AD4() & 2) {
        if (D_80071E34 != 0 && D_80071E34 == D_80071E34 / 50 * 50) {
            g_gameState->unk9 = 3;
        } else {
            g_gameState->unk9 = 4;
        }
    } else {
        g_gameState->unk9 = 0;
    }
    VSync(g_gameState->unk9);
    ResetGraph(1);
    PutDispEnv(&g_gameState->unkC[g_gameState->unk1C->unk8]);
    if (D_800AB2C4 & 4) {
        PutDrawEnv(&g_gameState->unk10[g_gameState->unk1C->unk8]);
    } else {
        g = g_gameState->unk1C;
        DrawOTagEnv(g->unk10[g->unk8] + 0xFFF, &g_gameState->unk10[g->unk8]);
    }
    g_gameState->unk1C->unk8 ^= 1;
    func_8002C0DC(g_gameState->unk1C->unk8);
    D_800AB2C4 |= D_800AB3B8[0][0];
    D_800AB2C4 &= ~D_800AB3B8[1][0];
    for (i = 0; i < 2; i++) {
        for (j = 0; j < 3; j++) {
            D_800AB3B8[i][j] = D_800AB3B8[i][j + 1];
        }
        D_800AB3B8[i][3] = 0;
    }
    D_800AB2C8++;
}

void func_800A9DD4(void) {
    func_8019A9D8();
    func_80022B28();
    D_800AB2C4 |= 0x80;
    D_800AB3C8 = 3;
}

s32 func_800A9E14(s32 arg0) {
    s32 timer;
    s32 i;
    s32 tim;
    s32 pad;
    s32 id;
    s32 k;
    s32 btn;

    i = 0;
    timer = -1;
    func_800AA848(arg0);
    do {
        func_800AA994();
        func_80019194();
        switch (D_800AB440) {
        case 0:
            i = 0;
            timer = -1;
            D_800AB440 = 1;
            D_800AB2EC = 1;
            break;
        case 1:
            if (i < 4) {
                k = arg0 * 4 + i;
                id = D_800AB2DC[k] | 0x40000;
                if (timer == -1) {
                    tim = (s32)func_8001C7FC(4, id, 0);
                    func_8001D9CC(tim, 0, 0xE0, 0x280, 0);
                    func_8001D8A8(tim);
                    timer = 0x3A2;
                    func_800320B8(1, 1, 0xF0, 0);
                } else if (timer != 0) {
                    pad = g_padPortPressed[0];
                    setlen(&D_800AB3D0[g_gameState->unk1C->unk8][0], 4);
                    D_800AB3D0[g_gameState->unk1C->unk8][0].code = 0x64;
                    D_800AB3D0[g_gameState->unk1C->unk8][0].r0 = 0x80;
                    D_800AB3D0[g_gameState->unk1C->unk8][0].g0 = 0x80;
                    D_800AB3D0[g_gameState->unk1C->unk8][0].b0 = 0x80;
                    D_800AB3D0[g_gameState->unk1C->unk8][0].x0 = 0;
                    D_800AB3D0[g_gameState->unk1C->unk8][0].y0 = 0;
                    D_800AB3D0[g_gameState->unk1C->unk8][0].u0 = 0;
                    D_800AB3D0[g_gameState->unk1C->unk8][0].v0 = 0;
                    D_800AB3D0[g_gameState->unk1C->unk8][0].w = 0xC0;
                    D_800AB3D0[g_gameState->unk1C->unk8][0].h = 0xE0;
                    D_800AB3D0[g_gameState->unk1C->unk8][0].clut = 0x3800;
                    addPrim(&g_gameState->unk1C->unk10[g_gameState->unk1C->unk8][8], &D_800AB3D0[g_gameState->unk1C->unk8][0]);
                    setDrawTPage(&D_800AB420[g_gameState->unk1C->unk8][0], 1, 1, 0x10A);
                    addPrim(&g_gameState->unk1C->unk10[g_gameState->unk1C->unk8][8], &D_800AB420[g_gameState->unk1C->unk8][0]);
                    setlen((D_800AB3D0[g_gameState->unk1C->unk8] + 1), 4);
                    (D_800AB3D0[g_gameState->unk1C->unk8] + 1)->code = 0x64;
                    (D_800AB3D0[g_gameState->unk1C->unk8] + 1)->r0 = 0x80;
                    (D_800AB3D0[g_gameState->unk1C->unk8] + 1)->g0 = 0x80;
                    (D_800AB3D0[g_gameState->unk1C->unk8] + 1)->b0 = 0x80;
                    (D_800AB3D0[g_gameState->unk1C->unk8] + 1)->x0 = 0xC0;
                    (D_800AB3D0[g_gameState->unk1C->unk8] + 1)->y0 = 0;
                    (D_800AB3D0[g_gameState->unk1C->unk8] + 1)->u0 = 0;
                    (D_800AB3D0[g_gameState->unk1C->unk8] + 1)->v0 = 0;
                    (D_800AB3D0[g_gameState->unk1C->unk8] + 1)->w = 0x80;
                    (D_800AB3D0[g_gameState->unk1C->unk8] + 1)->h = 0xE0;
                    (D_800AB3D0[g_gameState->unk1C->unk8] + 1)->clut = 0x3800;
                    addPrim(&g_gameState->unk1C->unk10[g_gameState->unk1C->unk8][8], (D_800AB3D0[g_gameState->unk1C->unk8] + 1));
                    setDrawTPage(D_800AB420[g_gameState->unk1C->unk8] + 1, 1, 1, 0x10D);
                    addPrim(&g_gameState->unk1C->unk10[g_gameState->unk1C->unk8][8], (D_800AB420[g_gameState->unk1C->unk8] + 1));
                    pad &= 0x4008;
                    btn = pad;
                    if (!(D_800AB2D0.unk0 & 1) || btn) {
                        if (btn) {
                            if (timer >= 0x1F) {
                                timer = 0x1E;
                            }
                            D_800AB2EC = 1;
                            D_80073998[0] = -1;
                            D_800AB2D0.unk0 |= 0x100;
                        }
                        if (!(D_800AB2D0.unk0 & 1) && timer < 0xF1) {
                            func_800320B8(1, 1, timer, 0xFFFFFF);
                            D_800AB2D0.unk0 |= 1;
                        }
                    }
                    timer--;
                } else {
                    timer = -1;
                    D_800AB2D0.unk0 &= ~1;
                    if (D_800AB2D0.unk0 & 0x100) {
                        i = 4;
                    } else {
                        i++;
                    }
                }
            } else {
                timer = 0x4EC;
                D_80073998[0] = 0;
                func_800A82F4(D_800AB2EC);
                func_800A812C(1);
                D_800AB440 = 2;
            }
            break;
        case 2:
            func_800A808C();
            if (func_800A811C() == 5) {
                D_800AB440 = 4;
            }
            func_80032100();
            func_80023A0C();
            if (!(D_800AB2D0.unk0 & 0x10) && (u8)func_800A82B0() && timer < 0x5B) {
                func_800320B8(1, 1, 0x5A, 0xFFFFFF);
                if (g_gameState->unkB != 0xFF && (g_gameState->unkB & 1)) {
                    func_800A799C(0x8301, 0x7003A, 0x5A, 0, 0);
                }
                D_800AB2D0.unk0 |= 0x10;
            }
            if (timer == 0) {
                if (D_800AB440 != 4) {
                    D_800AB440 = 3;
                }
                if (g_gameState->unkB != 0xFF && (g_gameState->unkB & 1)) {
                    func_800A799C(0x100, 0x7003A, 0, 0, 0);
                }
            }
            if ((D_800AB2D0.unk0 & 0x10) || (u8)func_800A82B0()) {
                timer--;
            }
            break;
        }
        func_80032098();
        func_800AA9D8();
    } while ((u32)D_800AB440 < 3);
    func_800AA960(arg0);
    return D_800AB440 == 4;
}

s32 func_800AA848(s32 idx) {
    u8 *buf;
    FileEntry *e;

    while (func_800217C4(g_gameState->unk1C->unk854) != 0) {
    }
    e = func_8001E218(1, D_800AB2D8[idx]);
    func_8002177C(g_gameState->unk1C->unk854, e->unk4, (e->unkC - e->unk4) << 11, D_80067944 + 0x10000, 0, -1, 0);
    while (func_800217C4(g_gameState->unk1C->unk854) != 0) {
    }
    buf = D_80067944 + 0x10000;
    D_800AB2D4 = buf;
    func_8001C750((s32)buf, 0);
    func_800320B8(1, 1, 0, 0xFFFFFF);
    D_800AB440 = 0;
    D_800AB2D0.unk0 = 0;
    D_80073998[0] = 0;
    return 0;
}

s32 func_800AA960(s32 arg0) {
    func_8001C784(D_800AB2D4);
    D_800AB2D4 = NULL;
    return 0;
}

void func_800AA994(void) {
    GameData *ctx = g_gameState->unk1C;
    ClearOTagR(ctx->unk10[ctx->unk8], 0x1000);
}

void func_800AA9D8(void) {
    GameData *ctx;
    u8 i;

    DrawSync(0);
    g_gameState->unk9 = 0;
    VSync(g_gameState->unk9);
    ResetGraph(1);
    PutDispEnv(&g_gameState->unkC[g_gameState->unk1C->unk8]);
    ctx = g_gameState->unk1C;
    i = ctx->unk8;
    DrawOTagEnv(ctx->unk10[i] + 0xFFF, &g_gameState->unk10[i]);
    g_gameState->unk1C->unk8 ^= 1;
    func_8002C0DC(g_gameState->unk1C->unk8);
}

void func_800AAADC(void) {
    RECT *r;
    s32 t;

    t = 0xD2;
    func_800AAF40();
    func_8001D910((void *)D_800AB2F4, NULL, &r);
    r->w *= 4;
    do {
        func_800AB09C();
        func_80019194();
        if (t == 0xD2) {
            func_800320B8(1, 1, 0x3C, 0);
        } else if (t == 0x3C) {
            func_800320B8(1, 1, 0x3C, 0xFFFFFF);
        }
        setlen(&D_800AB448[g_gameState->unk1C->unk8], 4);
        D_800AB448[g_gameState->unk1C->unk8].code = 0x64;
        D_800AB448[g_gameState->unk1C->unk8].r0 = 0x80;
        D_800AB448[g_gameState->unk1C->unk8].g0 = 0x80;
        D_800AB448[g_gameState->unk1C->unk8].b0 = 0x80;
        D_800AB448[g_gameState->unk1C->unk8].x0 = (0x140 - r->w) / 2;
        D_800AB448[g_gameState->unk1C->unk8].y0 = (0xE0 - r->h) / 2;
        D_800AB448[g_gameState->unk1C->unk8].u0 = 0;
        D_800AB448[g_gameState->unk1C->unk8].v0 = 0;
        D_800AB448[g_gameState->unk1C->unk8].w = r->w;
        D_800AB448[g_gameState->unk1C->unk8].h = r->h;
        D_800AB448[g_gameState->unk1C->unk8].clut = 0x3800;
        addPrim(&g_gameState->unk1C->unk10[g_gameState->unk1C->unk8][8], &D_800AB448[g_gameState->unk1C->unk8]);
        setDrawTPage(&D_800AB470[g_gameState->unk1C->unk8], 1, 1, 0xA);
        addPrim(&g_gameState->unk1C->unk10[g_gameState->unk1C->unk8][8], &D_800AB470[g_gameState->unk1C->unk8]);
        t--;
        func_80032098();
        func_800AB0E0();
    } while (t > 0);
    func_800AB06C();
}

void func_800AAF40(void) {
    u8 *buf;
    FileEntry *e;

    while (func_800217C4(g_gameState->unk1C->unk854) != 0) {
    }
    e = func_8001E218(1, 0x2718);
    func_8002177C(g_gameState->unk1C->unk854, e->unk4, (e->unkC - e->unk4) << 11, D_80067944 + 0x10000, 0, -1, 0);
    while (func_800217C4(g_gameState->unk1C->unk854) != 0) {
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
    GameData *ctx = g_gameState->unk1C;
    ClearOTagR(ctx->unk10[ctx->unk8], 0x1000);
}

void func_800AB0E0(void) {
    GameData *ctx;
    u8 i;

    DrawSync(0);
    g_gameState->unk9 = 0;
    VSync(g_gameState->unk9);
    ResetGraph(1);
    PutDispEnv(&g_gameState->unkC[g_gameState->unk1C->unk8]);
    ctx = g_gameState->unk1C;
    i = ctx->unk8;
    DrawOTagEnv(ctx->unk10[i] + 0xFFF, &g_gameState->unk10[i]);
    g_gameState->unk1C->unk8 ^= 1;
    func_8002C0DC(g_gameState->unk1C->unk8);
}

void func_800AB1E4(void) {
    g_gameState->unk1C->unk890 = 1;
}

void func_800AB200(void) {
    GameData *ctx = g_gameState->unk1C;
    if (ctx->unk890 == 0) {
        ctx->unk890 = 2;
    }
}
