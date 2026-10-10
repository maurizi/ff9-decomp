#include "ovl_0d.h"

/* Sits right after the last function, at the end of .text in the original. */
s32 D_800B365C = 0;

void func_800A752C(void) {
    func_800A755C();
    func_800A7E10();
    func_800A75E0();
}

void func_800A755C(void) {
    D_800B365C = 0;
    D_800B3660 = 0;
    D_8006794C->unk1C->unk83C = (void (*)(s32, s32, s32, s32, s32))func_800A78EC;
    func_80021C60();
    func_800A7764();
    func_800A7634();
    func_800A76F8();
    func_800A78EC(0, 0x42, 0, 0, 0);
    func_800A7818();
    func_800A7884();
}

void func_800A75E0(void) {
    func_800A78B8();
    func_800A78EC(0x100, 0x42, 0, 0, 0);
    D_8006794C->unkA = 9;
    D_8006794C->unk8 = 1;
}

s32 func_800A7634(void) {
    s32 i;
    s32 h;
    s32 a, b;
    for (i = func_8001C8B0(D_800B365C, 1, 0x90000, 0) - 1; i >= 0; i--) {
        h = func_8001C8B0(D_800B365C, 3, (i & 0xFFFF) | 0x90000, 0);
        a = (s32)func_8001C7FC(4, h, 0);
        b = (s32)func_8001C7FC(5, h, 0);
        func_80055DF4();
        func_80055E14(a, b, 1);
    }
    return 0;
}

s32 func_800A76F8(void) {
    s32 p = (s32)func_8001C7FC(4, 0x70042, 0);
    memmove(D_8006794C->unk1C->unk814, (void *)p, (s32)func_8001C7FC(5, 0x70042, 0));
    return 0;
}

s32 func_800A7764(void) {
    s32 dst = (s32)(D_80067944 + 0x57800);
    FileEntry *f = func_8001E218(1, 0x2715);
    func_8002177C(D_8006794C->unk1C->unk854, f->unk4, (f->unkC - f->unk4) << 11, (void *)dst, 0, -1, 0);
    while (func_800217C4(D_8006794C->unk1C->unk854) != 0) {
    }
    D_800B365C = dst;
    func_8001C750(dst, 0);
    return 0;
}

s32 func_800A7818(void) {
    s32 end = func_8001C8B0(D_800B365C, 0, 0x70000, 0);
    D_800B3660 = (s32)(D_80067944 + 0x141000);
    memmove((void *)D_800B3660, (void *)D_800B365C, end - D_800B365C);
    func_8001C750(D_800B3660, 0);
    return 0;
}

s32 func_800A7884(void) {
    func_8001C784((void *)D_800B365C);
    D_800B365C = 0;
    return 0;
}

s32 func_800A78B8(void) {
    func_8001C784((void *)D_800B3660);
    D_800B3660 = 0;
    return 0;
}

INCLUDE_RODATA("asm/us/ovl_0d/nonmatchings/ovl_0d", D_800A7000);

s32 func_800A78EC(s32 arg0, s32 arg1, s32 arg2, s32 arg3, s32 arg4) {
    GameData *g;
    s32 kind;
    s32 h;
    s32 id;

    kind = (arg0 >> 8) & 0xF;
    switch ((arg0 >> 12) & 3) {
    case 0:
        switch (kind) {
        case 0:
            if (D_8006794C->unk1C->unk851 != 0) {
                return -1;
            }
            if (D_800B3664 == -1) {
                D_800B3664 = kind;
            }
            if (D_8006794C->unk1C->unk81C[0] != arg1) {
                if (D_8006794C->unk1C->unk0 & 0x20000000) {
                    func_800552B8(D_8006794C->unk1C->unk824[0]);
                }
                D_8006794C->unk1C->unk0 &= 0xCFFFFFFF;
                D_8006794C->unk1C->unk820[0] = 0xE2;
                D_8006794C->unk1C->unk81C[0] = arg1;
                D_8006794C->unk1C->unk826[0] = 0x7F;
            }
            D_8006794C->unk1C->unk824[0] = func_80055290(D_8006794C->unk1C->unk814);
            g = D_8006794C->unk1C;
            if (!(g->unk0 & 0x20000000)) {
                func_80055A34(g->unk824[0], 0xA, 0, g->unk826[0]);
            }
            D_800B3664 = -1;
            D_8006794C->unk1C->unk0 |= 0x30000000;
            return 0;
        case 1:
            func_800552B8(D_8006794C->unk1C->unk824[0]);
            D_8006794C->unk1C->unk826[0] = 0x7F;
            D_8006794C->unk1C->unk0 &= 0xDFFFFFFF;
            return 0;
        case 2:
            if ((arg0 & 0x3F) != 1) {
                break;
            }
            func_800559C8(D_8006794C->unk1C->unk824[0], arg2);
            D_8006794C->unk1C->unk826[0] = arg2;
            return 0;
        case 3:
            if ((arg0 & 0x3F) != 1) {
                break;
            }
            func_800559FC(D_8006794C->unk1C->unk824[0], arg2, arg3);
            D_8006794C->unk1C->unk826[0] = arg3;
            return 0;
        case 4:
            if ((arg0 & 0x3F) != 1) {
                break;
            }
            func_80055A34(D_8006794C->unk1C->unk824[0], arg2, arg3, arg4);
            D_8006794C->unk1C->unk826[0] = arg4;
            return 0;
        case 6:
            if ((arg0 & 0x3F) != 1) {
                break;
            }
            func_800559FC(0, arg2, arg3);
            D_8006794C->unk1C->unk826[0] = arg3;
            return 0;
        case 11:
            if ((arg0 & 0x3F) != 1) {
                break;
            }
            func_80055A34(0, arg2, arg3, arg4);
            D_8006794C->unk1C->unk826[0] = arg4;
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
            }
            return -1;
        }
        return -1;
    }
    return -1;
}

void func_800A7E10(void) {
    TmState s;
    GameData *ctx;
    void *p;

    s.unk5C8.x = 0x32;
    s.unk5C8.y = 0x1E;
    s.unk5C8.w = 0xC8;
    s.unk5C8.h = 0x64;
    func_800A8D74(&s);
    SetDispMask(1);
    SetDispMask(1);
    while (s.unk0 != 0xC) {
        func_800A8B80();
        func_80019194();
        func_800A9B0C(&s);
        ctx = D_8006794C->unk1C;
        p = (void *)(ctx->unk8 * 0x4B000 + 0x80104A00);
        if (s.unk464 != 0) {
            p = func_800A993C(ctx->unk10[ctx->unk8], p, &s);
        }
        if (s.unk46C != 0) {
            p = func_800B030C(D_8006794C->unk1C->unk10[D_8006794C->unk1C->unk8], p, &s);
        }
        p = func_800A9424(D_8006794C->unk1C->unk10[D_8006794C->unk1C->unk8], p, &s);
        if (s.unk468 != 0 || s.unk0 == 0x15) {
            p = func_800A96B0(D_8006794C->unk1C->unk10[D_8006794C->unk1C->unk8], p, &s);
        }
        p = func_800AD400(D_8006794C->unk1C->unk10[D_8006794C->unk1C->unk8], p, &s);
        p = func_800B2F60(D_8006794C->unk1C->unk10[D_8006794C->unk1C->unk8], p, &s);
        p = func_800B3198(D_8006794C->unk1C->unk10[D_8006794C->unk1C->unk8], p, &s);
        switch (s.unk0) {
        case 2:
            D_80077DE8.unk8 = 0;
            func_800A932C(&s, "ENEMY CARD SELECT\n");
            func_800AFBE4(&s);
            func_800B08CC(&s);
            func_800ACFFC(&s);
            break;
        case 0:
            func_80035674(0);
            func_800AFBE4(&s);
            if (s.unk438 == 0) {
                func_800A932C(&s, "PLAYER CARD SELECT\n");
                func_800A9B2C(D_8006794C->unk1C->unk10[D_8006794C->unk1C->unk8], p, &s);
            } else {
                func_800A932C(&s, "PLAYER AUTO SELECT\n");
                func_800AA408(&s);
            }
            func_800ACFFC(&s);
            break;
        case 1:
            func_800A932C(&s, "PLAYER CARD SET\n");
            func_80035674(0);
            func_800A9F10(D_8006794C->unk1C->unk10[D_8006794C->unk1C->unk8], p, &s);
            func_800ACFFC(&s);
            break;
        case 3:
            D_80077DE8.unk8 = 0;
            func_800AFCE4(D_8006794C->unk1C->unk10[D_8006794C->unk1C->unk8], p, &s);
            func_800A932C(&s, "RESULT\n");
            func_800AA1FC(&s);
            break;
        case 4:
            D_80077DE8.unk8 = 0;
            func_800A932C(&s, "INIT\n");
            func_800A9298(&s);
            break;
        case 5:
            D_80077DE8.unk8 = 0;
            func_800A932C(&s, "FLASH\n");
            func_800ACFFC(&s);
            func_800AC708(D_8006794C->unk1C->unk10[D_8006794C->unk1C->unk8], p, &s);
            break;
        case 6:
            func_800A932C(&s, "BATTLE SELECT\n");
            func_80035674(0);
            p = func_800AB574(D_8006794C->unk1C->unk10[D_8006794C->unk1C->unk8], p, &s);
            func_800ACFFC(&s);
            func_800ABEBC(D_8006794C->unk1C->unk10[D_8006794C->unk1C->unk8], p, &s);
            break;
        case 7:
            D_80077DE8.unk8 = 0;
            func_800A932C(&s, "BATTLE\n");
            func_800ACFFC(&s);
            func_800AC060(D_8006794C->unk1C->unk10[D_8006794C->unk1C->unk8], p, &s);
            break;
        case 8:
            D_80077DE8.unk8 = 0;
            func_800A932C(&s, "TURN\n");
            func_800ACFFC(&s);
            func_800AC6A4(&s);
            break;
        case 9:
            D_80077DE8.unk8 = 0;
            func_800A932C(&s, "BATTLE WIN\n");
            func_800ACFFC(&s);
            func_800AC5AC(&s);
            break;
        case 10:
            D_80077DE8.unk8 = 0;
            func_800A932C(&s, "BATTLE LOSE\n");
            func_800ACFFC(&s);
            func_800AC624(&s);
            break;
        case 11:
            func_800A932C(&s, "TRY AGAIN\n");
            func_80035674(1);
            func_800AA548(&s);
            func_800B3480(D_8006794C->unk1C->unk10[D_8006794C->unk1C->unk8], p, &s);
            break;
        case 13:
            D_80077DE8.unk8 = 0;
            func_800A932C(&s, "CARD_CHANGE\n");
            func_800AE084(D_8006794C->unk1C->unk10[D_8006794C->unk1C->unk8], p, &s);
            break;
        case 14:
            D_80077DE8.unk8 = 0;
            func_800A932C(&s, "CARD_MOVE\n");
            func_800AD9F8(D_8006794C->unk1C->unk10[D_8006794C->unk1C->unk8], p, &s);
            break;
        case 15:
            D_80077DE8.unk8 = 0;
            func_800A932C(&s, "FLASH2\n");
            func_800ACDA0(D_8006794C->unk1C->unk10[D_8006794C->unk1C->unk8], p, &s);
            break;
        case 16:
            D_80077DE8.unk8 = 0;
            func_800A932C(&s, "TRADE\n");
            func_800AA504(&s);
            func_800AE398(D_8006794C->unk1C->unk10[D_8006794C->unk1C->unk8], p, &s);
            break;
        case 17:
            D_80077DE8.unk8 = 0;
            func_800A932C(&s, "CHANGE2\n");
            func_80035674(1);
            func_800AE5A0(D_8006794C->unk1C->unk10[D_8006794C->unk1C->unk8], p, &s);
            break;
        case 18:
            func_800A932C(&s, "CHOICE\n");
            func_80035674(1);
            func_800AA790(&s);
            func_800B1A84(D_8006794C->unk1C->unk10[D_8006794C->unk1C->unk8], p, &s);
            func_800AEE2C(&s);
            func_800ACFFC(&s);
            break;
        case 35:
            func_800A932C(&s, "GAME QUIT\n");
            func_800ABC74(&s);
            func_800B1A84(D_8006794C->unk1C->unk10[D_8006794C->unk1C->unk8], p, &s);
            func_800AEE2C(&s);
            func_800ACFFC(&s);
            break;
        case 19:
            func_800A932C(&s, "CHOICE2\n");
            func_800B1A84(D_8006794C->unk1C->unk10[D_8006794C->unk1C->unk8], p, &s);
            func_800AEE2C(&s);
            func_800AAEB4(&s);
            func_800ACFFC(&s);
            break;
        case 20:
            func_800A932C(&s, "STAGE SELECT\n");
            func_800AB0F0(&s);
            break;
        case 21:
            D_80077DE8.unk8 = 0;
            func_800A932C(&s, "FIELD FADE\n");
            func_800ACFFC(&s);
            break;
        case 22:
            D_80077DE8.unk8 = 0;
            func_800A932C(&s, "AWAY INFO\n");
            func_800B1A84(D_8006794C->unk1C->unk10[D_8006794C->unk1C->unk8], p, &s);
            func_800AB2E0(&s);
            break;
        case 23:
            func_800A932C(&s, "CARD AWAY\n");
            func_80035674(1);
            func_800AA790(&s);
            func_800AEE2C(&s);
            func_800B1A84(D_8006794C->unk1C->unk10[D_8006794C->unk1C->unk8], p, &s);
            break;
        case 24:
            func_800A932C(&s, "CARD AWAY2\n");
            func_80035674(1);
            func_800AEE2C(&s);
            func_800B1A84(D_8006794C->unk1C->unk10[D_8006794C->unk1C->unk8], p, &s);
            func_800AB338(&s);
            break;
        case 25:
            D_80077DE8.unk8 = 0;
            func_800A932C(&s, "CARD LESS5\n");
            func_800AB854(&s);
            break;
        case 26:
            D_80077DE8.unk8 = 0;
            func_800A932C(&s, "EXTENSION\n");
            func_800AA368(&s);
            break;
        case 27:
            D_80077DE8.unk8 = 0;
            func_800A932C(&s, "END FADE\n");
            if (s.unk5B8 != 0) {
                func_800B1A84(D_8006794C->unk1C->unk10[D_8006794C->unk1C->unk8], p, &s);
                func_800AEE2C(&s);
            }
            func_800B35A4(&s);
            break;
        case 28:
            D_80077DE8.unk8 = 0;
            func_800A932C(&s, "COIN\n");
            func_800B2C5C(D_8006794C->unk1C->unk10[D_8006794C->unk1C->unk8], p, &s);
            func_800ACFFC(&s);
            break;
        case 29:
            D_80077DE8.unk8 = 0;
            func_800A932C(&s, "FLASH ARROW\n");
            func_800ACF8C(&s);
            break;
        case 30:
            D_80077DE8.unk8 = 0;
            func_800A932C(&s, "ENEMY THINK\n");
            func_800B07DC(D_8006794C->unk1C->unk10[D_8006794C->unk1C->unk8], p, &s);
            func_800AF890(&s);
            break;
        case 33:
            D_80077DE8.unk8 = 0;
            func_800A932C(&s, "ENEMY APEAR\n");
            func_800AF760(&s);
            func_800B026C(&s);
            break;
        case 34:
            D_80077DE8.unk8 = 0;
            func_800A932C(&s, "GET SELECT\n");
            func_80035674(0);
            func_800AB928(D_8006794C->unk1C->unk10[D_8006794C->unk1C->unk8], p, &s);
            func_800ACFFC(&s);
            break;
        default:
            D_80077DE8.unk8 = 0;
            func_800A932C(&s, "NO TASK\n");
            break;
        }
        if (s.unk434 == 0) {
            func_800A932C(&s, "TURN PLAYER\n");
        }
        if (s.unk434 == 1) {
            func_800A932C(&s, "TURN ENEMY\n");
        }
        func_80023A0C();
        func_80032098();
        if (s.unk0 == 0x12 || s.unk0 == 0x23 || s.unk0 == 0x13 || s.unk0 == 0 || s.unk0 == 1 ||
            s.unk0 == 6 || s.unk0 == 0xB || s.unk0 == 0x22 || s.unk0 == 0x17 || s.unk0 == 0x18) {
            func_8003563C(0);
        } else {
            func_8003563C(1);
        }
        func_80035490();
        s.unk2E8++;
        s.unk480++;
        func_80062B3C();
        func_800A8BC4(&s);
    }
    func_8002C740(0, 0, s.unk558[0]);
    func_8002C740(0, 1, s.unk558[1]);
    func_8002C740(0, 2, s.unk558[2]);
    func_8002C740(0, 3, s.unk558[3]);
    func_8002C740(0, 8, s.unk558[4]);
    func_8002C740(0, 0xA, s.unk558[6]);
    func_8002C740(0, 9, s.unk558[5]);
    func_8002C740(0, 0xB, s.unk558[7]);
    func_8002C740(0, 4, s.unk558[8]);
    func_8002C740(0, 5, s.unk558[9]);
    func_8002C740(0, 6, s.unk558[10]);
    func_8002C740(0, 7, s.unk558[11]);
    DrawSync(0);
    VSync(D_8006794C->unk9);
    SetDispMask(0);
    func_80035674(1);
    func_8003556C(0);
    func_8003563C(0);
}

void func_800A8B80(void) {
    ClearOTagR(D_8006794C->unk1C->unk10[D_8006794C->unk1C->unk8], 0x1000);
}

void func_800A8BC4(TmState *s) {
    GameData *ctx;

    if ((D_80077524 & 0xF09) == 0xF09) {
        D_8006794C->unk0 |= 1;
        s->unk0 = 0xC;
    }
    if (&D_8006794C->unk10[D_8006794C->unk1C->unk8].dfe != NULL) {
        DrawSync(0);
        VSync(D_8006794C->unk9);
    } else {
        DrawSync(0);
        VSync(D_8006794C->unk9);
        ResetGraph(1);
    }
    PutDispEnv(&D_8006794C->unkC[D_8006794C->unk1C->unk8]);
    PutDrawEnv(&D_8006794C->unk10[D_8006794C->unk1C->unk8]);
    ctx = D_8006794C->unk1C;
    DrawOTag(ctx->unk10[ctx->unk8] + 0xFFF);
    D_8006794C->unk1C->unk8 ^= 1;
    func_8002C0DC(D_8006794C->unk1C->unk8);
}

void func_800A8D74(TmState *s) {
    s32 i;
    u8 *p;
    s32 x;
    s32 y;
    s32 v;

    func_800A9334();
    p = func_800A93A0(0);
    for (i = 0; i < 100; i++) {
        x = i / 30;
        y = i % 30;
        v = y / 6;
        func_800A986C((TmTim *)(p + i * 0xA80), x * 128 + (y % 6) * 21, v * 51 + 0x100, 0x180, i + 0x19C);
    }
    func_800A986C((TmTim *)func_800A93A0(0xB), 0x300, 0, 0, 0xEE);
    func_800A986C((TmTim *)func_800A93A0(4), 0x280, 0, 0, 0xE0);
    func_800A986C((TmTim *)func_800A93A0(5), 0x340, 0, 0, 0xE7);
    func_800A986C((TmTim *)func_800A93A0(7), 0x300, 0x100, 0, 0xEA);
    func_800A986C((TmTim *)func_800A93A0(8), 0x300, 0x180, 0, 0xEB);
    func_800A986C((TmTim *)func_800A93A0(9), 0x200, 0x100, 0, 0xEC);
    func_800A986C((TmTim *)func_800A93A0(0xA), 0x340, 0x1E0, 0, 0xED);
    func_800A986C((TmTim *)func_800A93A0(6), 0x280, 0x100, 0, 0xE9);
    func_800A93C8((u8 *)0x80104000, 3, 0x200);
    func_800A93C8((u8 *)0x80103800, 1, 0x1F4);
    func_800A93C8((u8 *)0x80103C00, 2, 0x400);
    D_8007B708 = 1;
    D_8006794C->unk9 = 0;
    func_800239D0();
    func_8003575C(D_800B3668);
    func_80035430();
    func_8003556C(D_800355DC);
    D_80077DE8.unk8 = 0;
    D_80077DE8.unk0 = 0x50;
    D_80077DE8.unk2 = 0x50;
    D_80077DE8.unk4 = 4;
    func_8002FA50(0xC);
    s->unk43C = 0;
    s->unk440 = 0;
    s->unk444 = 0;
    s->unk438 = 0;
    s->unk54C = 0;
    s->unk5B8 = 0;
    s->unk450 = (u16)D_8006794C->unk1C->unk8B2;
    s->unk448 = ((TmLevel *)0x80104000)[s->unk450].unk0;
    s->unk44C = ((TmLevel *)0x80104000)[s->unk450].unk1;
    s->unk454 = 1;
    func_800A9298(s);
    s->unk558[0] = func_8002C700(0, 0);
    s->unk558[1] = func_8002C700(0, 1);
    s->unk558[2] = func_8002C700(0, 2);
    s->unk558[3] = func_8002C700(0, 3);
    s->unk558[4] = func_8002C700(0, 8);
    s->unk558[6] = func_8002C700(0, 0xA);
    s->unk558[5] = func_8002C700(0, 9);
    s->unk558[7] = func_8002C700(0, 0xB);
    s->unk558[8] = func_8002C700(0, 4);
    s->unk558[9] = func_8002C700(0, 5);
    s->unk558[10] = func_8002C700(0, 6);
    s->unk558[11] = func_8002C700(0, 7);
    func_8002C740(0, 0, 1);
    func_8002C740(0, 1, 1);
    func_8002C740(0, 2, 1);
    func_8002C740(0, 3, 1);
    func_8002C740(0, 0x12, 1);
    func_8002C740(0, 0x13, 1);
    func_8002C740(0, 0xE, 0);
    func_8002C740(0, 0xF, 0);
    s->unk4C8[0] = 1;
    s->unk4C8[1] = 4;
    s->unk4C8[2] = 0x10;
    s->unk4C8[3] = 0x1F;
    s->unk4C8[4] = 0x1B;
    s->unk4C8[5] = 0xA;
    s->unk4C8[6] = 6;
    s->unk4C8[7] = 3;
    s->unk4C8[8] = 2;
    s->unk4C8[0] = 1;
    s->unk4C8[1] = 8;
    s->unk4C8[2] = 0x19;
    s->unk4C8[3] = 0x1F;
    s->unk4C8[4] = 0x12;
    s->unk4C8[5] = 9;
    s->unk4C8[6] = 5;
    s->unk4C8[7] = 2;
    s->unk4C8[8] = 1;
    s->unk4EC = 0;
    if (func_800625F8() < 5) {
        for (i = 0; i < 8; i++) {
            func_800AF1F4(s, func_80062B3C() % 100);
        }
    }
}

void func_800A9298(TmState *arg0) {
    arg0->unk4 = 0;
    arg0->unk8 = 0;
    arg0->unkC = 0;
    arg0->unk2EC = 0x80;
    arg0->unk554 = 0;
    arg0->unk568 = 0;
    arg0->unk57C = 0;
    arg0->unk458 = NULL;
    arg0->unk484 = 0;
    arg0->unk4B8 = NULL;
    arg0->unk4C4 = NULL;
    arg0->unk4F0 = 0;
    arg0->unk468 = 0;
    arg0->unk46C = 0;
    arg0->unk464 = 0;
    func_800AB8E8(0);
    func_800B29DC(arg0);
    func_800AEA24(arg0);
    func_800B1A24(arg0);
    arg0->unk4BC = 0;
    arg0->unk4C0 = 1;
    func_800B295C(arg0);
}

void func_800A932C(TmState *s, char *msg) {
}

void func_800A9334(void) {
    FileEntry *f = func_8001E218(1, 0x44C);
    if (f == NULL) {
        func_8002EB64(0x46, 0x3E6);
    }
    func_80022DF0(D_8006794C->unk20 + f->unk4, (f->unkC - f->unk4) << 11, (void *)0x80104A00);
}

INCLUDE_ASM("asm/us/ovl_0d/nonmatchings/ovl_0d", func_800A93A0);

void func_800A93C8(u8 *arg0, s32 arg1, s32 arg2) {
    s32 i;
    u8 *src = func_800A93A0(arg1);
    for (i = 0; i < arg2; i++) {
        *arg0++ = *src++;
    }
}

void *func_800A9424(u32 *ot, SPRT_16 *p, TmState *s) {
    u8 c;
    s32 x;
    s32 y;

    c = s->unk2EC;
    for (x = 0; x < 16; x++) {
        for (y = 0; y < 15; y++) {
            if (s->unk468 == 0 || x < 3) {
                if (s->unk0 == 0x15 && x >= 3) {
                    if (s->unk2E8 * 4 < 0x80) {
                        c = 0x80 - s->unk2E8 * 4;
                    } else {
                        c = 0;
                    }
                }
                setlen(p, 3);
                p->code = 0x7C;
                p->x0 = x * 16;
                p->y0 = y * 16;
                p->v0 = y * 16;
                p->r0 = c;
                p->g0 = c;
                p->b0 = c;
                p->u0 = x * 16;
                p->clut = 0x3800;
                AddPrim(ot + 0xFFE, p);
                p++;
            }
        }
    }
    setlen((DR_TPAGE *)p, 1);
    ((DR_TPAGE *)p)->code[0] = (GetTPage(1, 0, 0x280, 0) & 0x9FF) | 0xE1000000;
    AddPrim(ot + 0xFFE, p);
    do {
        p = (SPRT_16 *)((DR_TPAGE *)p + 1);
    } while (0);
    for (x = 0; x < 4; x++) {
        for (y = 0; y < 15; y++) {
            if (s->unk468 == 0 || x > 0) {
                if (s->unk0 == 0x15 && x == 0) {
                    if (s->unk2E8 * 4 < 0x80) {
                        c = 0x80 - s->unk2E8 * 4;
                    } else {
                        c = 0;
                    }
                }
                setlen(p, 3);
                p->code = 0x7C;
                p->x0 = x * 16 + 0x100;
                p->y0 = y * 16;
                p->v0 = y * 16;
                p->r0 = c;
                p->g0 = c;
                p->b0 = c;
                p->u0 = x * 16;
                p->clut = 0x3800;
                AddPrim(ot + 0xFFE, p);
                p++;
            }
        }
    }
    setlen((DR_TPAGE *)p, 1);
    ((DR_TPAGE *)p)->code[0] = (GetTPage(1, 0, 0x300, 0) & 0x9FF) | 0xE1000000;
    AddPrim(ot + 0xFFE, p);
    return (DR_TPAGE *)p + 1;
}

void *func_800A96B0(u32 *ot, SPRT_16 *p, TmState *s) {
    u8 c;
    s32 x;
    s32 y;
    DR_TPAGE *tp;

    c = s->unk2EC;
    if (s->unk0 == 0x15) {
        c = s->unk2E8 * 4;
    }
    for (x = 0; x < 14; x++) {
        for (y = 0; y < 15; y++) {
            setlen(p, 3);
            p->code = 0x7C;
            if (s->unk0 == 0x15) {
                p->code = 0x7E;
            }
            p->x0 = x * 16 + 0x30;
            p->y0 = y * 16;
            p->v0 = y * 16;
            p->r0 = c;
            p->g0 = c;
            p->b0 = c;
            p->u0 = x * 16;
            p->clut = 0x39C0;
            AddPrim(ot + 0xFFA, p);
            p++;
        }
    }
    tp = (DR_TPAGE *)p;
    setlen(tp, 1);
    tp->code[0] = (GetTPage(1, 1, 0x340, 0) & 0x9FF) | 0xE1000000;
    AddPrim(ot + 0xFFA, tp);
    tp++;
    if (s->unk0 == 0x15 && s->unk2E8 >= 0x20) {
        s->unk468 = 1;
        func_800AEBB4(s);
        s->unk464 = 1;
        for (y = 4; y >= 0; y--) {
            s->unk590[y] = 0;
        }
        s->unk46C = 1;
        s->unk5AC = 0;
        s->unk5A8 = 0;
        s->unk2E8 = 0;
        s->unk0 = 0x21;
    }
    return tp;
}

void func_800A986C(TmTim *tim, s32 x, s32 y, s32 cx, s32 cy) {
    TmTimImage *img;
    s32 tp;

    img = (TmTimImage *)((u8 *)tim + (tim->unk8 + 8));
    switch ((s32)(tim->unk4 & 3)) {
    case 0:
        tp = 0;
        LoadTPage(img->data, tp, 0, x, y, img->w << 2, img->h);
        DrawSync(0);
        LoadClut(tim->clut, cx, cy);
        DrawSync(0);
        break;
    case 1:
        tp = 1;
        LoadTPage(img->data, tp, 0, x, y, img->w << tp, img->h);
        DrawSync(0);
        LoadClut(tim->clut, cx, cy);
        DrawSync(0);
        break;
    case 2:
        break;
    default:
        return;
    }
}

void *func_800A993C(u32 *ot, SPRT *p, TmState *s) {
    s32 i;
    s32 n;
    s16 x;
    s16 y;
    u8 u;
    u8 v;
    s16 w;
    u8 c;

    c = s->unk2EC;
    for (i = 0; i < 2; i++) {
        if (i == 0) {
            n = func_800AE94C(s, 0);
            x = 0x28;
            y = 0xB9;
            v = 0;
        } else {
            n = func_800AE94C(s, 1);
            x = 0x1A;
            y = 0x93;
            v = 0x20;
        }
        u = (n - 1) * 16;
        if (n == 0) {
            u = 0x90;
        }
        if (n == 10) {
            u = 0xA0;
            x -= 8;
            w = 0x20;
        } else {
            w = 0x10;
        }
        setlen(p, 4);
        p->code = 0x64;
        p->x0 = x;
        p->y0 = y;
        p->r0 = c;
        p->g0 = c;
        p->b0 = c;
        p->u0 = u;
        p->v0 = v;
        p->clut = 0x3A80;
        p->w = w;
        p->h = 0x20;
        AddPrim(ot + 0xFF0, p);
        p++;
    }
    setlen(p, 4);
    p->code = 0x64;
    p->x0 = 0x10;
    p->y0 = 0xAA;
    p->r0 = c;
    p->g0 = c;
    p->b0 = c;
    p->u0 = 0xC0;
    p->v0 = 0;
    p->clut = 0x3A80;
    p->w = 0x30;
    p->h = 0x20;
    AddPrim(ot + 0xFF0, p);
    p++;
    setlen((DR_TPAGE *)p, 1);
    ((DR_TPAGE *)p)->code[0] = (GetTPage(1, 0, 0x300, 0x100) & 0x9FF) | 0xE1000000;
    AddPrim(ot + 0xFF0, p);
    return (DR_TPAGE *)p + 1;
}

void func_800A9B0C(TmState *s) {
    func_8002C308(0);
}

INCLUDE_ASM("asm/us/ovl_0d/nonmatchings/ovl_0d", func_800A9B2C);

void *func_800A9F10(u32 *ot, void *p, TmState *s) {
    TmCell *cell;
    TmCard *c;
    s32 pad;
    s32 cx;
    s32 cy;
    s32 ox;
    s32 oy;
    s32 x;
    s32 y;
    s32 one;

    if (func_800AE830(s, 0) == 0) {
        return p;
    }
    pad = D_80077528;
    func_800AE9D0(s, 0, s->unk4);
    cell = func_800B19D4(s, s->unk8, s->unkC);
    cx = s->unk8 * 43 + 0x48;
    cy = s->unkC * 52 + 8;
    p = func_800B16A4(ot, p, s, cx - 0xF, cy + 6, 0);
    c = func_800AE9D0(s, 0, s->unk4);
    if (func_800AE878(s, 0) != 5) {
        x = c->unkC - 0xF;
        y = c->unk10 + 6;
    } else {
        x = c->unkC - 0x12;
        y = c->unk10 + 3;
    }
    p = func_800B16A4(ot, p, s, x, y, 1);
    D_80077DE8.unk0 = cx - 0xD;
    D_80077DE8.unk2 = cy + 7;
    if (cell->unk0 == 1) {
        D_80077DE8.unk8 = func_80035774(0x22);
    } else {
        D_80077DE8.unk8 = func_80035774(0x30);
    }
    func_80035580(NULL);
    if (pad & 0x10000) {
        s->unk564 = 0;
        s->unk0 = 0;
        s->unk480 = s->unk488;
        func_800B01E8(3);
        return p;
    }
    if (pad & 0x20000) {
        if (cell->unk0 == 1) {
            c = func_800AE9D0(s, 0, s->unk4);
            c->unkC = cx;
            c->unk10 = cy;
            c->unk18 = 1;
            c->unk1C = 0;
            cell->unk0 = 0;
            cell->unk4 = c;
            s->unk4 = 0;
            s->unk424 = s->unk8;
            s->unk428 = s->unkC;
            func_800ABEC4(s);
            func_800B01E8(5);
            return p;
        }
        func_800B01E8(2);
    }
    ox = s->unk8;
    oy = s->unkC;
    if ((pad & 0x20) && s->unk8 != 3) {
        s->unk8++;
    }
    if ((pad & 0x80) && s->unk8 != 0) {
        s->unk8--;
    }
    if ((pad & 0x10) && s->unkC != 0) {
        s->unkC--;
    }
    if ((pad & 0x40) && s->unkC != 3) {
        s->unkC++;
    }
    if (ox != s->unk8 || oy != s->unkC) {
        func_800B01E8(1);
    }
    return p;
}

void func_800AA1FC(TmState *s) {
    RECT r;

    if ((D_80077528 & 0x30000) && s->unk2E8 >= 0x22) {
        s->unk468 = 0;
        s->unk464 = 0;
        s->unk46C = 0;
        switch (s->unk45C) {
        case 0:
            func_800AD974(s);
            break;
        case 1:
            func_800AD974(s);
            break;
        case 2:
            func_800AF170(s);
            func_800AF288(s);
            if ((u32)(s->unk450 - 0x7C) < 4) {
                r.x = 0x59;
                r.w = 0x8F;
                r.w = func_8002BA68(func_80035774(0x19)) + 0x12;
                r.x = 0xA0 - r.w / 2;
                r.y = 0x59;
                r.h = 0x2B;
                s->unk4B8 = func_8002DC54(&r, 5, 2);
                func_8002E2EC(s->unk4B8, func_800AA3C0);
                func_8002E31C(s->unk4B8, 1);
                func_800AB89C(s->unk4B8);
                s->unk0 = 0x1A;
            } else {
                func_800B3488(s);
            }
            break;
        }
    }
}

void func_800AA368(TmState *arg0) {
    if (D_800B3CD0 == 0 && (D_80077528 & 0x30000)) {
        func_8002DD00(arg0->unk4B8);
        arg0->unk0 = 4;
    }
}

void func_800AA3C0(Window *arg0) {
    func_8002B5FC(arg0->unk0 + 9, arg0->unk2 + 7, arg0->unk18, func_80035774(0x19), 0);
}

void func_800AA408(TmState *arg0) {
    s32 x;
    s32 y;
    s32 px;
    s32 py;
    TmCell *cell;
    TmCard *card;

    if (func_800AE830(arg0, 0) != 0) {
        do {
            x = func_80062B3C() & 3;
            y = func_80062B3C() & 3;
            cell = func_800B19D4(arg0, x, y);
        } while (cell->unk0 != 1);
        px = x * 0x2B + 0x48;
        py = y * 0x34 + 8;
        card = func_800AE9D0(arg0, 0, 0);
        card->unkC = px;
        card->unk10 = py;
        card->unk1C = 0;
        card->unk18 = 1;
        cell->unk0 = 0;
        cell->unk4 = card;
        arg0->unk424 = x;
        arg0->unk428 = y;
        func_800ABEC4(arg0);
    }
}

void func_800AA504(TmState *arg0) {
    if (D_800B3CD0 == 0 && (D_80077528 & 0xA0000)) {
        func_800AE54C(arg0);
    }
}

void func_800AA548(TmState *s) {
    RECT r;
    s32 pad;

    if (D_800B3CD0 == 0) {
        pad = D_80077528;
        if (pad & 0x20000) {
            func_8002DD00(s->unk458);
            if (s->unk4 == 0) {
                if (func_800625F8() < 5) {
                    r.w = func_8002BA68(func_80035774(0x1A)) + 0x12;
                    r.x = 0xA0 - r.w / 2;
                    r.y = 0x5B;
                    r.h = 0x2B;
                    s->unk4C4 = func_8002DC54(&r, 6, 2);
                    func_8002E2EC(s->unk4C4, func_800AB0A8);
                    func_8002E31C(s->unk4C4, 1);
                    func_800AB89C(s->unk4C4);
                    s->unk0 = 0x19;
                    func_800B01E8(2);
                } else {
                    s->unk0 = 4;
                }
            } else {
                func_800B35F8(s);
            }
            func_800B01E8(1);
        }
        if ((pad & 0x10000) && s->unk4 != 1) {
            func_800B01E8(3);
            s->unk4 = 1;
        }
        if (pad & 0x10) {
            if (s->unk4 != 0) {
                func_800B01E8(1);
                s->unk4 = 0;
            } else {
                func_800B01E8(1);
                s->unk4 = 1;
            }
        }
        if (pad & 0x40) {
            if (s->unk4 != 1) {
                func_800B01E8(1);
                s->unk4 = 1;
            } else {
                func_800B01E8(1);
                s->unk4 = 0;
            }
        }
        if (s->unk4 == 0) {
            D_80077DE8.unk0 = 0x7D;
            D_80077DE8.unk2 = 0x6B;
            D_80077DE8.unk8 = func_80035774(0x29);
            func_800270EC(0x7D, 0x6B, 3);
        } else {
            D_80077DE8.unk0 = 0x7D;
            D_80077DE8.unk2 = 0x7B;
            D_80077DE8.unk8 = func_80035774(0x28);
            func_800270EC(0x7D, 0x7B, 3);
        }
        func_800AB8F4(0x89);
    }
}

void func_800AA790(TmState *s) {
    RECT r;
    u32 pad;
    s32 n;
    s32 x;
    s32 y;
    TmCard *c;
    Unk6 *d;

    func_800270EC(s->unk474 * 15 + 3, s->unk478 * 15 + 0x30, 3);
    if (s->unk0 == 0x12 || s->unk0 == 0x17) {
        D_80077DE8.unk0 = s->unk474 * 15 + 3;
        D_80077DE8.unk2 = s->unk478 * 15 + 0x30;
        if (s->unk0 == 0x12) {
            D_80077DE8.unk8 = func_80035774((u32)(s->unk450 - 0x7C) >= 4 ? 0x24 : 0x32);
        } else {
            D_80077DE8.unk8 = func_80035774(0x27);
        }
        r.x = 0xD;
        r.w = 0x9E;
        r.y = 0;
        r.h = 0xDE;
        func_80035580(&r);
    }
    pad = D_80077528;
    n = func_80062560(func_800AF268(s));
    if (s->unk568 != 0) {
        pad = 0;
        if (--s->unk568 == 0 && s->unk418 == 0) {
            if (n == ++s->unk4B4) {
                s->unk4B4 = 0;
            }
        }
    }
    if (n >= 2) {
        if (pad & 0x200000) {
            func_800B01E8(5);
            pad = 0;
            s->unk418 = 0;
            s->unk568 = 4;
        } else if (pad & 0x100000) {
            pad = 0;
            s->unk418 = 1;
            s->unk568 = 4;
            s->unk4B4--;
            func_800B01E8(5);
            if (s->unk4B4 == -1) {
                s->unk4B4 = n - 1;
            }
        }
    }
    x = s->unk474;
    y = s->unk478;
    if ((pad & 0x20) && x != 9) {
        s->unk474 = x + 1;
        s->unk4B4 = 0;
    }
    if ((pad & 0x80) && s->unk474 != 0) {
        s->unk474--;
        s->unk4B4 = 0;
    }
    if ((pad & 0x40) && s->unk478 != 9) {
        s->unk478++;
        s->unk4B4 = 0;
    }
    if ((pad & 0x10) && s->unk478 != 0) {
        s->unk478--;
        s->unk4B4 = 0;
    }
    if (x != s->unk474 || y != s->unk478) {
        func_800B01E8(1);
        return;
    }
    if (s->unk0 == 0x12) {
        if (((pad & 0x10000) && s->unk47C == 0) || (pad & 0x80000)) {
            if (s->unk450 != 0x7C) if (s->unk450 != 0x7D) if (s->unk450 != 0x7E) if (s->unk450 != 0x7F) {
                r.w = func_8002BA68(func_80035774(0x2D)) + 0x12;
                r.x = 0xA0 - r.w / 2;
                r.y = 0x54;
                r.h = 0x38;
                s->unk5B4 = 1;
                s->unk484 = func_8002DC54(&r, 5, 2);
                func_8002E2EC(s->unk484, func_800ABE74);
                func_8002E31C(s->unk484, 1);
                func_800AB89C(s->unk484);
                s->unk0 = 0x23;
                return;
            }
        }
        if (pad & 0x10000) {
            if (s->unk47C != 0) {
                s->unk47C--;
                c = &s->unk10[s->unk47C];
                func_80062420(c->unk0, c->unk1, c->unk2, c->unk3, c->unk4, c->unk5);
                func_800B01E8(3);
            }
        } else if (pad & 0x20000) {
            if (s->unk438 != 0) {
                func_80062724();
                s->unk474 = func_80062B3C() % 10;
                s->unk478 = func_80062B3C() % 10;
                s->unk4B4 = 0;
                if (func_80062560(func_800AF268(s)) == 0) {
                    func_800AF1F4(s, func_800AF268(s));
                    func_800AF1F4(s, func_800AF268(s));
                    func_800AF1F4(s, func_800AF268(s));
                    func_800AF1F4(s, func_800AF268(s));
                    func_800AF1F4(s, func_800AF268(s));
                    func_800AF1F4(s, func_800AF268(s));
                    func_800AF1F4(s, func_800AF268(s));
                }
            }
            d = func_800625A0(func_800AF268(s), s->unk4B4);
            if (d != (Unk6 *)-1) {
                c = &s->unk10[s->unk47C];
                c->unk0 = d->unk0;
                c->unk2 = d->unk2;
                c->unk3 = d->unk3;
                c->unk4 = d->unk4;
                c->unk5 = d->unk5;
                c->unk1 = d->unk1;
                func_800626D0(func_800AF268(s), s->unk4B4);
                s->unk4B4 = 0;
                func_800B01E8(5);
                if (++s->unk47C == 5) {
                    D_80077DF0 = 0;
                    r.w = func_8002BA68(func_80035774(6)) + 0x12;
                    r.x = 0xA0 - r.w / 2;
                    r.y = 0x53;
                    r.h = 0x3A;
                    s->unk458 = func_8002DC54(&r, 6, 2);
                    func_8002E2EC(s->unk458, func_800AB060);
                    func_8002E31C(s->unk458, 1);
                    func_800AB89C(s->unk458);
                    s->unk4 = 0;
                    s->unk0 = 0x13;
                }
            }
        }
    } else if ((pad & 0x20000) && func_80062560(func_800AF268(s)) != 0) {
        D_80077DF0 = 0;
        r.w = func_8002BA68(func_80035774(0x18)) + 0x12;
        r.x = 0xB9;
        r.h = 0x38;
        r.y = 0x35;
        s->unk4 = 1;
        s->unk4B8 = func_8002DC54(&r, 5, 2);
        func_8002E2EC(s->unk4B8, func_800AB52C);
        func_8002E31C(s->unk4B8, 1);
        func_800AB89C(s->unk4B8);
        func_800B01E8(1);
        s->unk0 = 0x18;
    }
}

void func_800AAEB4(TmState *s) {
    s32 pad;
    TmCard *c;

    if (D_800B3CD0 == 0) {
        pad = D_80077528;
        if (pad & 0x20000) {
            if (s->unk4 == 0) {
                s->unk2E8 = 0;
                s->unk0 = 0x15;
            } else {
                s->unk0 = 0x12;
                s->unk47C = 4;
                c = &s->unk10[4];
                func_80062420(c->unk0, c->unk1, c->unk2, c->unk3, c->unk4, c->unk5);
            }
            func_8002DD00(s->unk458);
            func_800B01E8(1);
        }
        if ((pad & 0x10000) && s->unk4 != 1) {
            s->unk4 = 1;
            func_800B01E8(3);
        }
        if (pad & 0x10) {
            if (s->unk4 != 0) {
                s->unk4 = 0;
            } else {
                s->unk4 = 1;
            }
            func_800B01E8(1);
        }
        if (pad & 0x40) {
            if (s->unk4 != 1) {
                s->unk4 = 1;
            } else {
                s->unk4 = 0;
            }
            func_800B01E8(1);
        }
        if (s->unk4 == 0) {
            D_80077DE8.unk0 = 0x7B;
            D_80077DE8.unk2 = 0x6C;
            D_80077DE8.unk8 = func_80035774(0x25);
            func_800270EC(0x7B, 0x6C, 3);
        } else {
            D_80077DE8.unk0 = 0x7B;
            D_80077DE8.unk2 = 0x7B;
            D_80077DE8.unk8 = func_80035774(0x26);
            func_800270EC(0x7B, 0x7B, 3);
        }
        func_800AB8F4(0x8D);
    }
}

void func_800AB060(Window *arg0) {
    func_8002B5FC(arg0->unk0 + 9, arg0->unk2 + 7, arg0->unk18, func_80035774(6), 0);
}

void func_800AB0A8(Window *arg0) {
    func_8002B5FC(arg0->unk0 + 9, arg0->unk2 + 7, arg0->unk18, func_80035774(0x1A), 0);
}

void func_800AB0F0(TmState *s) {
    s32 sum;
    s32 pad;

    sum = s->unk4C8[0] + s->unk4C8[1] + s->unk4C8[2] + s->unk4C8[3] + s->unk4C8[4] + s->unk4C8[5] +
          s->unk4C8[6] + s->unk4C8[7] + s->unk4C8[8];
    pad = D_80077528;
    if (!(D_80077524 & 0x10000)) {
        if (pad & 0x40) {
            s->unk450--;
        }
        if (pad & 0x20) {
            s->unk450 += 10;
        }
        if (pad & 0x10) {
            s->unk450++;
        }
        if (pad & 0x80) {
            s->unk450 -= 10;
        }
        if ((pad & 0x20000) && sum == 100) {
            func_800B295C(s);
        }
    } else {
        if ((pad & 0x40) && s->unk4EC != 8) {
            s->unk4EC++;
        }
        if ((pad & 0x10) && s->unk4EC != 0) {
            s->unk4EC--;
        }
        if ((pad & 0x20) && sum != 100) {
            s->unk4C8[s->unk4EC]++;
        }
        if ((pad & 0x80) && s->unk4C8[s->unk4EC] != 0) {
            s->unk4C8[s->unk4EC]--;
        }
    }
    s->unk448 = ((TmLevel *)0x80104000)[s->unk450].unk0;
    s->unk44C = ((TmLevel *)0x80104000)[s->unk450].unk1;
}

void func_800AB2E0(TmState *arg0) {
    if (D_800B3CD0 == 0 && (D_80077528 & 0xA0000)) {
        func_8002DD00(arg0->unk4B8);
        arg0->unk0 = 0x17;
    }
}

void func_800AB338(TmState *s) {
    s32 pad;

    if (D_800B3CD0 == 0) {
        pad = D_80077528;
        if (pad & 0x20000) {
            if (s->unk4 == 0) {
                func_800626D0(func_800AF268(s), s->unk4B4);
                if (func_800625F8() > 100) {
                    s->unk4B4 = 0;
                    s->unk0 = 0x17;
                } else {
                    func_800AF288(s);
                    func_800B3488(s);
                }
            } else {
                s->unk0 = 0x17;
            }
            func_8002DD00(s->unk4B8);
            func_800B01E8(1);
        }
        if ((pad & 0x10000) && s->unk4 != 1) {
            s->unk4 = 1;
            func_800B01E8(3);
        }
        if (pad & 0x10) {
            if (s->unk4 != 0) {
                s->unk4 = 0;
            } else {
                s->unk4 = 1;
            }
            func_800B01E8(1);
        }
        if (pad & 0x40) {
            if (s->unk4 != 1) {
                s->unk4 = 1;
            } else {
                s->unk4 = 0;
            }
            func_800B01E8(1);
        }
        if (s->unk4 == 0) {
            D_80077DE8.unk0 = 0xBC;
            D_80077DE8.unk2 = 0x4D;
            D_80077DE8.unk8 = func_80035774(0x2A);
            func_800270EC(0xBC, 0x4D, 3);
        } else {
            D_80077DE8.unk0 = 0xBC;
            D_80077DE8.unk2 = 0x5C;
            D_80077DE8.unk8 = func_80035774(0x2B);
            func_800270EC(0xBC, 0x5C, 3);
        }
        func_800AB8F4(0xCE);
    }
    if (s->unk2E8 & 3) {
        func_800270EC(s->unk474 * 15 + 3, s->unk478 * 15 + 0x30, 6);
    }
}

void func_800AB52C(Window *arg0) {
    func_8002B5FC(arg0->unk0 + 0xA, arg0->unk2 + 6, arg0->unk18, func_80035774(0x18), 0);
}

void *func_800AB574(u32 *ot, void *p, TmState *s) {
    s32 pad;
    s32 ox;
    s32 oy;
    s32 i;
    s32 x;
    s32 y;
    TmCell *cell;
    TmCard *c;

    pad = D_80077528;
    ox = s->unk8;
    oy = s->unkC;
    if ((pad & 0x20) && ox != 3 && ox != s->unk424 + 1) {
        s->unk8 = ox + 1;
    }
    if ((pad & 0x80) && s->unk8 != 0 && s->unk8 != s->unk424 - 1) {
        s->unk8--;
    }
    if ((pad & 0x10) && s->unkC != 0 && s->unkC != s->unk428 - 1) {
        s->unkC--;
    }
    if ((pad & 0x40) && s->unkC != 3 && s->unkC != s->unk428 + 1) {
        s->unkC++;
    }
    if (ox != s->unk8 || oy != s->unkC) {
        func_800B01E8(1);
    }
    if (pad & 0x20000) {
        cell = func_800B19D4(s, s->unk8, s->unkC);
        if (cell->unk0 == 0) {
            c = cell->unk4;
            for (i = 0; i < s->unk4AC; i++) {
                if (c == s->unk48C[i]) {
                    s->unk2E8 = 0;
                    s->unk0 = 7;
                    s->unk42C = s->unk8;
                    s->unk430 = s->unkC;
                }
            }
        }
        if (s->unk0 != 7) {
            func_800B01E8(2);
        }
    }
    x = s->unk8 * 43;
    y = s->unkC * 52;
    p = func_800B16A4(ot, p, s, x + 0x39, y + 0xE, 0);
    D_80077DE8.unk0 = x + 0x3B;
    D_80077DE8.unk2 = y + 0xF;
    D_80077DE8.unk8 = func_80035774(0x31);
    cell = func_800B19D4(s, s->unk8, s->unkC);
    if (cell->unk0 == 0) {
        c = cell->unk4;
        for (i = 0; i < s->unk4AC; i++) {
            if (c == s->unk48C[i]) {
                D_80077DE8.unk8 = func_80035774(0x23);
            }
        }
    }
    func_80035580(NULL);
    return p;
}

void func_800AB854(TmState *arg0) {
    if (D_80077528 & 0x30000) {
        func_8002DD00(arg0->unk4C4);
        func_800B35F8(arg0);
    }
}

void func_800AB89C(Window *arg0) {
    func_8002E2E4(arg0, (void (*)(Window *))func_800AB8C8);
    func_800AB8E8(1);
}

void func_800AB8C8(void) {
    func_800AB8E8(0);
}

void func_800AB8E8(s32 arg0) {
    D_800B3CD0 = arg0;
}

void func_800AB8F4(s16 arg0) {
    RECT r;
    r.x = 0;
    r.y = 0;
    r.w = arg0;
    r.h = 0xF0;
    func_80035580(&r);
}

void *func_800AB928(u32 *ot, void *p, TmState *s) {
    RECT r;
    RECT r2;
    u8 buf[64];
    u8 num[16];
    s32 pad;
    TmCard *sel;
    TmCard *c;
    TmUnk350 *e;
    s32 x;
    s32 y;
    s32 i;
    s32 j;
    s32 n;
    TmCard *cards;
    s32 k;

    pad = D_80077528;
    if ((pad & 0x10) && s->unk5B0 != 0) {
        s->unk5B0--;
        func_800B01E8(1);
    }
    if ((pad & 0x40) && s->unk5B0 != func_800AE8D0(s) - 1) {
        s->unk5B0++;
        func_800B01E8(1);
    }
    sel = func_800AE908(s, s->unk5B0);
    x = sel->unkC;
    y = sel->unk10;
    p = func_800B16A4(ot, p, s, x + 0x22, y + 5, 2);
    D_80077DE8.unk0 = x + 0x25;
    D_80077DE8.unk2 = y + 6;
    D_80077DE8.unk8 = func_80035774(0x2C);
    r.x = x + 0x2A;
    r.w = 0x12C;
    r.y = 0;
    r.h = 0xF0;
    func_80035580(&r);
    if (pad & 0x20000) {
        if (s->unk54C != 0 && sel == &s->unk10[s->unk550 + 5]) {
            s->unk54C = 0;
        }
        func_800ACFC8(s);
        func_800ACFC8(s);
        k = 0;
        e = s->unk350;
        cards = s->unk10;
        for (i = 0; i < 10; i++) {
            c = &cards[i];
            if (i < 5) {
                if (c->unk14 == 1) {
                    e[k].unk10 = c;
                    e[k].unk0 = 1;
                    e[k].unkC = 0;
                    k++;
                }
            } else if (c->unk14 == 0 && sel != c) {
                e[k].unk10 = c;
                e[k].unk0 = 1;
                e[k].unkC = 1;
                k++;
            }
        }
        s->unk504 = 1;
        s->unk0 = 0xF;
    }
    n = 0;
    cards = s->unk10;
    for (i = 0; i < 10; i++) {
        if (i >= 5 && func_800AE908(s, s->unk5B0) == &cards[i]) {
            n = func_80062560(cards[i].unk0);
            for (j = 0; j < 5; j++) {
                if (cards[i].unk0 == cards[j].unk0) {
                    n++;
                }
            }
        }
    }
    func_8002BAE4(n, num, -1, 0);
    func_8002BD54(buf, func_80035774(0x33));
    func_8002AD38(buf, num);
    func_8002AD38(buf, func_80035774(0x34));
    r2.w = func_8002BA68((s32)buf) + 0x10;
    r2.x = 0x52;
    r2.y = sel->unk10 - 9;
    r2.h = 0x19;
    func_8002B5FC(0x5A, r2.y + 6, 3, buf, 0);
    func_8002DEE4(&r2, 3, 2);
    return p;
}

void func_800ABC74(TmState *s) {
    s32 pad;
    s32 i;
    TmCard *c;

    if (D_800B3CD0 == 0) {
        pad = D_80077528;
        if (pad & 0x10) {
            if (s->unk5B4 != 0) {
                s->unk5B4 = 0;
            } else {
                s->unk5B4 = 1;
            }
            func_800B01E8(1);
        }
        if (pad & 0x40) {
            if (s->unk5B4 != 1) {
                s->unk5B4 = 1;
            } else {
                s->unk5B4 = 0;
            }
            func_800B01E8(1);
        }
        if (pad & 0x10000) {
            if (s->unk5B4 != 1) {
                s->unk5B4 = 1;
                func_800B01E8(3);
            }
        } else if (pad & 0x20000) {
            if (s->unk5B4 == 0) {
                for (i = 0; i < s->unk47C; i++) {
                    c = &s->unk10[i];
                    func_80062420(c->unk0, c->unk1, c->unk2, c->unk3,
                                  c->unk4, c->unk5);
                }
                s->unk47C = 0;
                func_80062750(1);
                s->unk5B8 = 1;
                func_800B35F8(s);
            } else {
                s->unk0 = 0x12;
            }
            func_8002DD00(s->unk484);
            func_800B01E8(1);
        }
        if (s->unk5B4 == 0) {
            D_80077DE8.unk0 = 0x7B;
            D_80077DE8.unk2 = 0x6C;
            D_80077DE8.unk8 = func_80035774(0x2E);
            func_800270EC(0x7B, 0x6C, 3);
        } else {
            D_80077DE8.unk0 = 0x7B;
            D_80077DE8.unk2 = 0x7B;
            D_80077DE8.unk8 = func_80035774(0x2F);
            func_800270EC(0x7B, 0x7B, 3);
        }
        func_800AB8F4(0x8D);
    } else {
        D_80077DE8.unk8 = NULL;
    }
}

void func_800ABE74(Window *arg0) {
    func_8002B5FC(arg0->unk0 + 9, arg0->unk2 + 6, arg0->unk18, func_80035774(0x2D), 0);
}

void *func_800ABEBC(u32 *ot, void *p, TmState *s) {
    return p;
}

void func_800ABEC4(TmState *s) {
    TmPos pos;
    s32 n;
    s32 dir;
    s32 y;
    u8 arrows;
    s32 owner;
    TmCard *c;
    s32 v;

    n = 0;
    s->unk57C = 0;
    c = func_800AEE08(s, s->unk424, s->unk428);
    owner = c->unk14;
    arrows = c->unk1;
    for (dir = 0; dir < 8; dir++) {
        if (arrows & 1) {
            pos.unk0 = s->unk424;
            pos.unk4 = s->unk428;
            if (func_800B12F0(s, &pos, dir, owner) != NULL) {
                s->unk42C = pos.unk0;
                y = pos.unk4;
                s->unk2E8 = 0;
                s->unk430 = y;
                s->unk4B0 = func_800B19D4(s, s->unk424, s->unk428)->unk4;
                s->unk48C[n] = func_800B19D4(s, pos.unk0, pos.unk4)->unk4;
                n++;
            }
        }
        arrows >>= 1;
    }
    if (n == 1) {
        s->unk0 = 7;
    } else if (n >= 2) {
        if (s->unk434 == 0 && s->unk438 == 0) {
            s->unk2E8 = 0;
            s->unk4AC = n;
            s->unk0 = 6;
        } else {
            s->unk0 = 7;
        }
    } else if (func_800B0EB8(s, owner, s->unk424, s->unk428, (TmCard *)-1) != 0) {
        func_800B1110(s, owner, s->unk424, s->unk428, 8, 1);
    } else {
        s->unk0 = 8;
    }
}

INCLUDE_ASM("asm/us/ovl_0d/nonmatchings/ovl_0d", func_800AC060);

void func_800AC4C8(s32 unused, TmPos *out, TmCard *a, TmCard *b) {
    s32 atk;
    s32 def;
    s32 aatk;
    s32 apdef;
    s32 amdef;
    s32 batk;
    s32 bpdef;
    s32 bmdef;

    atk = 0;
    aatk = a->unk2;
    apdef = a->unk4;
    amdef = a->unk5;
    batk = b->unk2;
    bpdef = b->unk4;
    bmdef = b->unk5;
    def = 0;
    switch (a->unk3) {
    case 0:
        atk = aatk;
        def = bpdef;
        break;
    case 1:
        atk = aatk;
        def = bmdef;
        break;
    case 2:
        atk = aatk;
        def = bpdef;
        if (bmdef < def) {
            def = bmdef;
        }
        break;
    case 3:
        atk = aatk;
        if (atk < apdef) {
            atk = apdef;
        }
        if (atk < amdef) {
            atk = amdef;
        }
        def = batk;
        if (bpdef < def) {
            def = bpdef;
        }
        if (bmdef < def) {
            def = bmdef;
        }
        break;
    }
    if (atk == 0) {
        atk = 1;
    }
    if (def == 0) {
        def = 1;
    }
    out->unk0 = atk;
    out->unk4 = def;
}

void func_800AC5AC(TmState *arg0) {
    if (func_800B0EB8(arg0, arg0->unk434, arg0->unk42C, arg0->unk430, (TmCard *)-1) != 0) {
        func_800B1110(arg0, arg0->unk434, arg0->unk42C, arg0->unk430, -1, 2);
    } else {
        func_800ABEC4(arg0);
    }
}

void func_800AC624(TmState *arg0) {
    s32 who = arg0->unk434 == 0;
    if (func_800B0EB8(arg0, who, arg0->unk424, arg0->unk428, (TmCard *)-1) != 0) {
        func_800B1110(arg0, who, arg0->unk424, arg0->unk428, 8, 2);
    } else {
        arg0->unk0 = 8;
    }
}

void func_800AC6A4(TmState *arg0) {
    if (arg0->unk434 == 0) {
        arg0->unk434 = 1;
        arg0->unk2E8 = 0;
        arg0->unk418 = 0;
        arg0->unk5A4 = (func_80062B3C() & 0xF) + 0xF;
        arg0->unk0 = 0x1E;
    } else {
        arg0->unk564 = 0;
        arg0->unk434 = 0;
        arg0->unk0 = 0;
    }
}

INCLUDE_ASM("asm/us/ovl_0d/nonmatchings/ovl_0d", func_800AC708);

void *func_800ACDA0(u32 *ot, TILE *p, TmState *s) {
    s32 t;
    u8 c;
    s32 i;
    TmUnk350 *e;
    TmCard *card;

    e = s->unk350;
    if (s->unk418 == 0) {
        c = t = s->unk2E8 * 30;
        if (t >= 0x100) {
            c = 0xFF;
            s->unk2E8 = 0;
            s->unk418 = 1;
        }
    } else {
        c = t = 0xFF - s->unk2E8 * 12;
        if (t < 0) {
            c = 0;
            if (s->unk45C == 0) {
                func_800AF170(s);
            } else {
                func_800AF100(s);
            }
            func_800ADE5C(s);
        }
    }
    for (i = 0; i < 10; i++) {
        if (e[i].unk0 == 1) {
            card = e[i].unk10;
            if (s->unk418 == 1) {
                card->unk14 = e[i].unkC;
            }
            setlen(p, 3);
            p->code = 0x62;
            p->x0 = card->unkC;
            p->y0 = card->unk10;
            p->w = 0x2A;
            p->h = 0x33;
            p->r0 = c;
            p->g0 = c;
            p->b0 = c;
            AddPrim(ot + card->unk30 - 1, p);
            p++;
            SetDrawTPage(p, 0, 0, GetTPage(0, 1, 0, 0));
            AddPrim(ot + card->unk30 - 1, p);
            p = (TILE *)((DR_TPAGE *)p + 1);
        }
    }
    return p;
}

void func_800ACF8C(TmState *arg0) {
    if (arg0->unk2E8 >= 0x15) {
        arg0->unk2E8 = 0;
        arg0->unk0 = 5;
        func_800B01E8(9);
    }
}

void func_800ACFC8(TmState *arg0) {
    s32 i;
    TmUnk350 *p = arg0->unk350;
    arg0->unk2E8 = 0;
    arg0->unk418 = 0;
    for (i = 0; i < 10; i++) {
        p[i].unk0 = 0;
        p[i].unk4 = 0;
        p[i].unk8 = 0;
    }
}

INCLUDE_ASM("asm/us/ovl_0d/nonmatchings/ovl_0d", func_800ACFFC);

INCLUDE_ASM("asm/us/ovl_0d/nonmatchings/ovl_0d", func_800AD400);

void func_800AD974(TmState *arg0) {
    s32 i;
    TmCard *c = arg0->unk10;
    for (i = 0; i < 10; i++) {
        c[i].unk20 = c[i].unkC << 8;
        c[i].unk24 = c[i].unk10 << 8;
        if (i < 5) {
            c[i].unk28 = 0x10400;
            c[i].unk2C = i * 0x2000 + 0x1900;
        } else {
            c[i].unk28 = 0x1000;
            c[i].unk2C = (i - 5) * 0x2000 + 0x1900;
        }
    }
    arg0->unk460 = 0x14;
    arg0->unk2E8 = 0;
    arg0->unk0 = 0xE;
}

void *func_800AD9F8(u32 *ot, void *p, TmState *s) {
    TmCard *c;
    TmCard *cards;
    TmUnk350 *e;
    s32 i;
    s32 j;
    s32 k;
    s32 skip;
    s32 flag;
    s32 n;
    s32 a;
    s32 b;
    s32 st;

    if (s->unk2E8 < s->unk460) {
        cards = s->unk10;
        for (i = 0; i < 10; i++) {
            c = &cards[i];
            a = (c->unk28 - c->unk20) / s->unk460;
            b = (c->unk2C - c->unk24) / s->unk460;
            c->unkC = (c->unk20 + a * s->unk2E8) >> 8;
            c->unk10 = (c->unk24 + b * s->unk2E8) >> 8;
        }
    } else {
        cards = s->unk10;
        for (i = 0; i < 10; i++) {
            if (i < 5) {
                cards[i].unkC = 0x104;
                cards[i].unk10 = i * 32 + 0x19;
            } else {
                cards[i].unkC = 0x10;
                cards[i].unk10 = i * 32 - 0x87;
            }
        }
    }
    if (s->unk460 + 10 < s->unk2E8) {
        if (s->unk45C == 0) {
            if (func_800AE998(s, 0) != 10 && func_800AE8D0(s) != 1) {
                s->unk5B0 = 0;
                st = 0x22;
                goto done;
            }
        }
        if (s->unk45C == 0) {
            if (s->unk54C != 0 && (&s->unk10[5])[s->unk550].unk14 == 0) {
                s->unk54C = 0;
            }
        }
        flag = 0;
        if (s->unk45C == 0) {
            n = func_800AE998(s, 0);
            skip = -1;
            if (n == 10) {
                s->unk54C = 0;
            }
        } else {
            n = func_800ADE14(s, 0);
            skip = (func_80062B3C() & 0xFF) % (5 - n);
        }
        cards = s->unk10;
        e = s->unk350;
        func_800ACFC8(s);
        k = 0;
        j = 0;
        for (i = 0; i < 10; i++) {
            c = &cards[i];
            if (i < 5) {
                if (c->unk14 == 1) {
                    if (k != skip && func_800AE998(s, 1) != 10) {
                        e[j].unk10 = c;
                        e[j].unk0 = 1;
                        e[j].unkC = 0;
                        j++;
                        flag = 1;
                    }
                    if (s->unk45C == 1) {
                        k++;
                    }
                }
            } else if (c->unk14 == 0) {
                st = s->unk45C;
                if (st == 1) {
                    e[j].unk10 = c;
                    e[j].unk0 = st;
                    e[j].unkC = st;
                    j++;
                    flag = 1;
                }
            }
        }
        if (flag == 0) {
            if (s->unk45C == 0) {
                func_800AF170(s);
            } else {
                func_800AF100(s);
            }
            func_800ADE5C(s);
            return p;
        }
        s->unk504 = 1;
        st = 0xF;
    done:
        s->unk0 = st;
    }
    return p;
}

s32 func_800ADDCC(TmState *arg0) {
    s32 n = 0;
    s32 i;
    TmCard *c = arg0->unk10;
    for (i = 0; i < arg0->unk550; i++) {
        if (c[i + 5].unk14 == 0) {
            n++;
        }
    }
    return n;
}

s32 func_800ADE14(TmState *arg0, s32 arg1) {
    s32 i;
    s32 n;
    TmCard *c;
    if (arg1 == 0) {
        c = &arg0->unk10[0];
    } else {
        c = &arg0->unk10[5];
    }
    n = 0;
    for (i = 0; i < 5; i++) {
        if (c[i].unk14 == arg1) {
            n++;
        }
    }
    return n;
}

void func_800ADE5C(TmState *s) {
    RECT r;
    s32 owner;
    s32 x;
    s32 i;
    s32 found;
    s32 j;
    TmCard *c;

    owner = 1;
    if (s->unk45C == 0) {
        owner = 0;
        j = 5;
        x = 0x10;
    } else {
        j = 0;
        x = 0x104;
    }
    found = 0;
    c = s->unk10;
    for (i = 0; i < 5; i++) {
        if ((c + j)[i].unk14 == owner && (c + j)[i].unkC == x) {
            found = 1;
            s->unk470 = &(c + j)[i];
            s->unk460 = 0xA;
            s->unk2E8 = 0;
            (c + j)[i].unk20 = (c + j)[i].unkC << 8;
            (c + j)[i].unk24 = (c + j)[i].unk10 << 8;
            break;
        }
    }
    if (found == 0) {
        if (func_800625F8() > 100) {
            r.w = func_8002BA68(func_80035774(0x16)) + 0x12;
            r.x = 0xA0 - r.w / 2;
            r.y = 0x5B;
            r.h = 0x2A;
            s->unk4B8 = func_8002DC54(&r, 5, 2);
            func_8002E2EC(s->unk4B8, func_800ADFFC);
            func_8002E31C(s->unk4B8, 1);
            func_800AB89C(s->unk4B8);
            func_800AF288(s);
            func_800B29DC(s);
            s->unk0 = 0x16;
            func_800B01E8(2);
        } else {
            func_800B3488(s);
        }
    } else {
        s->unk0 = 0xD;
        func_800B01E8(5);
    }
}

void func_800ADFFC(Window *arg0) {
    func_8002B5FC(arg0->unk0 + 9, arg0->unk2 + 7, arg0->unk18, func_80035774(0x16), 0);
}

void func_800AE044(Window *arg0) {
    func_8002B5FC(0x82, 0x10, arg0->unk18, func_80035774(0x17), 0);
}

void *func_800AE084(u32 *ot, void *p, TmState *s) {
    RECT r;
    u8 buf[64];
    TmCard *c;
    TmCard *found;
    TmCard *cards;
    s32 i;
    s32 dx;
    s32 dy;
    s32 n;
    s32 w;
    u8 id;

    c = s->unk470;
    if (s->unk2E8 < s->unk460) {
        dx = (0x8B00 - c->unk20) / s->unk460;
        dy = (0x5500 - c->unk24) / s->unk460;
        c->unkC = (c->unk20 + dx * s->unk2E8) >> 8;
        c->unk10 = (c->unk24 + dy * s->unk2E8) >> 8;
        func_800AE6EC(s, 0);
        return p;
    }
    if (s->unk45C == 0) {
        D_800B3CDC = func_80062420(c->unk0, c->unk1, c->unk2, c->unk3, c->unk4, c->unk5);
    } else {
        s->unk510 = c->unk0;
        s->unk512 = c->unk2;
        s->unk511 = c->unk1;
        s->unk513 = c->unk3;
        s->unk514 = c->unk4;
        s->unk515 = c->unk5;
        s->unk54C = 1;
        D_800B3CDC = 0;
        if (func_80062560(c->unk0) == 0) {
            found = NULL;
            cards = s->unk10;
            for (i = 0; i < 5; i++) {
                if (cards[i].unk0 == c->unk0) {
                    found = &cards[i];
                }
            }
            if (found == c) {
                D_800B3CDC = 1;
            }
        }
    }
    n = s->unk45C;
    c->unkC = 0x8B;
    c->unk10 = 0x55;
    D_800B3CE0 = n;
    D_800B3CD8 = id = c->unk0;
    func_8002BD54(buf, (u8 *)func_8006276C(id));
    if (D_800B3CE0 == 0) {
        func_8002AD38(buf, func_80035774(3));
    } else {
        func_8002AD38(buf, func_80035774(4));
    }
    func_8002AD38(buf, func_80035774(7));
    w = func_8002BA68(buf) + 0x10;
    r.x = 0xA0 - w / 2;
    r.y = 0x96;
    r.w = w;
    r.h = 0x1A;
    s->unk458 = func_8002DC54(&r, 5, 2);
    func_8002E2EC(s->unk458, func_800AE494);
    func_8002E31C(s->unk458, 1);
    func_800AB89C(s->unk458);
    func_800B01E8(s->unk45C == 0 ? 4 : 0xC);
    s->unk0 = 0x10;
    return p;
}

void *func_800AE398(u32 *ot, SPRT *p, TmState *s) {
    u8 b;
    DR_TPAGE *tp;

    b = s->unk2EC;
    if (D_800B3CDC != 0) {
        setlen(p, 4);
        p->code = 0x64;
        p->x0 = 0xA3;
        p->y0 = 0x45;
        if (s->unk45C == 0) {
            p->u0 = 0x20;
            p->v0 = 0x90;
        } else {
            p->u0 = 0xC0;
            p->v0 = 0x80;
        }
        p->w = 0x38;
        p->h = 0x18;
        p->clut = 0x3AC0;
        p->r0 = b;
        p->g0 = b;
        p->b0 = b;
        AddPrim(ot + 30, p);
        p++;
        tp = (DR_TPAGE *)p;
        setlen(tp, 1);
        tp->code[0] = (GetTPage(1, 0, 0x300, 0x100) & 0x9FF) | 0xE1000000;
        AddPrim(ot + 30, tp);
        p = (SPRT *)(tp + 1);
    }
    return p;
}

void func_800AE494(Window *w) {
    u8 buf[64];
    Window *v;

    v = w;
    if (D_800B3CE0 == 0) {
        func_8002BD54(buf, func_80035774(3));
    } else {
        func_8002BD54(buf, func_80035774(4));
    }
    func_8002AD38(buf, (u8 *)func_8006276C(D_800B3CD8));
    func_8002AD38(buf, func_80035774(7));
    func_8002B5FC(0x9F - func_8002BA68((s32)buf) / 2, v->unk2 + 6, w->unk18, buf, 0);
}

void func_800AE54C(TmState *arg0) {
    TmCard *c = arg0->unk470;
    c->unk30 = 0x28;
    c->unk20 = c->unkC << 8;
    c->unk24 = c->unk10 << 8;
    arg0->unk2E8 = 0;
    arg0->unk0 = 0x11;
    func_8002DD00(arg0->unk458);
}

void *func_800AE5A0(u32 *ot, void *p, TmState *s) {
    TmCard *c;
    s32 dx;
    s32 dy;

    c = s->unk470;
    if (s->unk2E8 < s->unk460) {
        if (s->unk45C == 0) {
            dx = (0x10400 - c->unk20) / s->unk460;
        } else {
            dx = (0x1000 - c->unk20) / s->unk460;
        }
        dy = (0x9900 - c->unk24) / s->unk460;
        c->unkC = (c->unk20 + dx * s->unk2E8) >> 8;
        c->unk10 = (c->unk24 + dy * s->unk2E8) >> 8;
    } else {
        c->unk10 = 0x19;
        if (s->unk45C == 0) {
            c->unkC = 0x104;
        } else {
            c->unkC = 0x10;
        }
        func_800AE6EC(s, 1);
        func_800ADE5C(s);
    }
    return p;
}

void func_800AE6EC(TmState *s, s32 flag) {
    s32 side;
    s32 n;
    s32 i;
    s32 j;
    s32 x;
    s32 y;
    s32 step;
    TmCard *c;
    TmCard *first;
    TmCard *second;

    for (side = 0; side < 2; side++) {
        if (side == 0) {
            first = &s->unk10[0];
            second = &s->unk10[5];
            x = 0x104;
        } else {
            first = &s->unk10[5];
            second = &s->unk10[0];
            x = 0x10;
        }
        n = 0;
        c = s->unk10;
        for (i = 0; i < 10; i++) {
            if (c[i].unkC == x) {
                n++;
            }
        }
        step = 0x20;
        if (n >= 5) {
            step = 0x80 / (n - 1);
        }
        j = 0;
        for (i = 0; i < 5; i++) {
            if (first[i].unkC == x) {
                if (flag != 0) {
                    first[i].unk30 = 0x3C - j;
                }
                first[i].unk10 = j * step + 0x19;
                j++;
            }
        }
        for (i = 0; i < 5; i++) {
            if (second[i].unkC == x) {
                if (flag != 0) {
                    second[i].unk30 = 0x3C - j;
                }
                second[i].unk10 = j * step + 0x19;
                j++;
            }
        }
    }
}

s32 func_800AE830(TmState *arg0, s32 arg1) {
    s32 n = 0;
    s32 i;
    TmCard *c = arg0->unk10;
    for (i = 0; i < 10; i++) {
        if (c[i].unk14 == arg1 && c[i].unk18 == 0) {
            n++;
        }
    }
    return n;
}

s32 func_800AE878(TmState *arg0, s32 arg1) {
    s32 n = 0;
    s32 i;
    TmCard *c = arg0->unk10;
    for (i = 0; i < 5; i++) {
        if (c[i].unk34 != 0 && c[i].unk14 == arg1 && c[i].unk18 == 0) {
            n++;
        }
    }
    return n;
}

s32 func_800AE8D0(TmState *arg0) {
    s32 n = 0;
    s32 i;
    TmCard *c = arg0->unk10 + 5;
    for (i = 0; i < 5; i++) {
        if (c[i].unk14 == 0) {
            n++;
        }
    }
    return n;
}

TmCard *func_800AE908(TmState *arg0, s32 arg1) {
    s32 i;
    TmCard *base;
    TmCard *c;
    i = 0;
    c = base = &arg0->unk10[5];
    for (; i < 5; i++, c++) {
        if (c->unk14 == 0) {
            if (arg1 == 0) {
                return c;
            }
            arg1--;
        }
    }
    return base;
}

s32 func_800AE94C(TmState *arg0, s32 arg1) {
    s32 n = 0;
    s32 i;
    TmCard *c = arg0->unk10;
    for (i = 0; i < 10; i++) {
        if (c[i].unk18 == 1 && c[i].unk14 == arg1) {
            n++;
        }
    }
    return n;
}

s32 func_800AE998(TmState *arg0, s32 arg1) {
    s32 n = 0;
    s32 i;
    TmCard *c = arg0->unk10;
    for (i = 0; i < 10; i++) {
        if (c[i].unk14 == arg1) {
            n++;
        }
    }
    return n;
}

TmCard *func_800AE9D0(TmState *arg0, s32 arg1, s32 arg2) {
    s32 n = 0;
    s32 i;
    TmCard *c = arg0->unk10;
    for (i = 0; i < 10; i++) {
        if (c[i].unk14 == arg1 && c[i].unk18 == 0) {
            if (arg2 == n) {
                return &c[i];
            }
            n++;
        }
    }
    return NULL;
}

void func_800AEA24(TmState *s) {
    TmCard *c;
    TmCard *base;
    TmCardData *tbl;
    s32 v;
    s32 i;
    s32 r;
    u8 id;

    tbl = (TmCardData *)0x80103800;
    base = s->unk10;

    for (i = 0; i < 10; i++) {
        c = &base[i];
        c->unkC = 0xDC;
        c->unk10 = i * 30;
        c->unk14 = 0;
        c->unk18 = 0;
        c->unk8 = 0;
        c->unk30 = 0x28;
        c->unk0 = 0;
        c->unk1 = func_80062B3C();
        c->unk2 = func_80062B3C();
        r = func_80062B3C() & 3;
        switch (r) {
        case 0:
            c->unk3 = 0;
            break;
        case 1:
            c->unk3 = r;
            break;
        case 2:
            c->unk3 = 3;
            break;
        case 3:
            c->unk3 = 2;
            break;
        }
        c->unk4 = func_80062B3C();
        c->unk5 = func_80062B3C();
        id = i;
        c->unk0 = id;
        c->unk1 = func_80062B3C();
        c->unk2 = tbl[id].unk0;
        c->unk3 = tbl[id].unk1;
        c->unk4 = tbl[id].unk2;
        v = tbl[id].unk3;
        c->unk34 = 0;
        c->unk5 = v;
    }
    for (i = 5; i < 10; i++) {
        base[i].unk34 = 0;
    }
}

void func_800AEBB4(TmState *s) {
    s32 i;
    s32 r;
    s32 k;
    u8 id;
    TmCard *c;
    TmCard *base;
    TmDeckOdds *odds;

    odds = (TmDeckOdds *)0x80103C00;
    s->unk550 = func_80062B3C() % 5;
    base = &s->unk10[5];
    for (i = 0; i < 5; i++) {
        c = &base[i];
        c->unkC = 0x10;
        c->unk10 = 0x12C;
        c->unk14 = 1;
        c->unk18 = 0;
        c->unk8 = 1;
        c->unk1C = 0;
        c->unk30 = 0x28;
        r = func_80062B3C() & 0xFF;
        if (r < 20) {
            k = 0;
        } else if (r < 40) {
            k = 1;
        } else if (r < 60) {
            k = 2;
        } else if (r < 80) {
            k = 3;
        } else if (r < 100) {
            k = 4;
        } else if (r < 120) {
            k = 5;
        } else if (r < 140) {
            k = 6;
        } else if (r < 160) {
            k = 7;
        } else if (r < 180) {
            k = 8;
        } else if (r < 200) {
            k = 9;
        } else if (r < 220) {
            k = 10;
        } else if (r < 240) {
            k = 11;
        } else if (r < 252) {
            k = 12;
        } else if (r < 254) {
            k = 13;
        } else if (r < 255) {
            k = 14;
        } else {
            k = 15;
        }
        id = odds[s->unk448].v[k];
        if (s->unk438 != 0) {
            id = func_80062B3C() % 100;
        }
        if (i == s->unk550 && s->unk54C != 0) {
            c->unk0 = s->unk510;
            c->unk2 = s->unk512;
            c->unk1 = s->unk511;
            c->unk3 = s->unk513;
            c->unk4 = s->unk514;
            c->unk5 = s->unk515;
        } else {
            func_800AF3E8(s, c, id, 1);
        }
        c->unk34 = 1;
    }
}

TmCard *func_800AEE08(TmState *arg0, s32 arg1, s32 arg2) {
    return func_800B19D4(arg0, arg1, arg2)->unk4;
}

INCLUDE_RODATA("asm/us/ovl_0d/nonmatchings/ovl_0d", D_800A72B8);

INCLUDE_ASM("asm/us/ovl_0d/nonmatchings/ovl_0d", func_800AEE2C);

void func_800AF100(TmState *arg0) {
    s32 i;
    TmCard *c = arg0->unk10;
    for (i = 0; i < 5; i++) {
        if (c[i].unk14 == 0) {
            func_80062420(c[i].unk0, c[i].unk1, c[i].unk2, c[i].unk3, c[i].unk4, c[i].unk5);
        }
    }
}

void func_800AF170(TmState *arg0) {
    s32 i;
    TmCard *c = arg0->unk10;
    for (i = 0; i < 5; i++) {
        if (arg0->unk45C == 0) {
            func_800AF388(arg0, &c[i]);
        }
        func_80062420(c[i].unk0, c[i].unk1, c[i].unk2, c[i].unk3, c[i].unk4, c[i].unk5);
    }
}

void func_800AF1F4(TmState *arg0, u8 arg1) {
    TmCard c;
    if (func_800625F8() < 0x64) {
        func_800AF3E8(arg0, &c, arg1, 1);
        func_80062420(arg1, c.unk1, c.unk2, c.unk3, c.unk4, c.unk5);
    }
}

s32 func_800AF268(TmState *arg0) {
    return arg0->unk474 * 10 + arg0->unk478;
}

void func_800AF288(TmState *arg0) {
    s32 i;
    for (i = 9; i >= 0; i--) {
        arg0->unk10[i].unk34 = 0;
    }
}

void func_800AF2A8(TmState *arg0) {
    s32 i;
    s32 one = 1;
    for (i = 9; i >= 0; i--) {
        arg0->unk10[i].unk34 = one;
    }
}

void func_800AF2CC(TmState *s, TmCard *c, s32 stat) {
    TmCard base;

    func_800AF3E8(s, &base, c->unk0, 0);
    if (stat == 0 && c->unk2 != base.unk2) {
        c->unk2++;
    }
    if (stat == 1 && c->unk4 != base.unk4) {
        c->unk4++;
    }
    if (stat == 2 && c->unk5 != base.unk5) {
        c->unk5++;
    }
    func_80015C38("AP=%x : PD=%x : MD=%x \n", c->unk2, c->unk4, c->unk5);
}

void func_800AF388(TmState *s, TmCard *c) {
    func_800AF2CC(s, c, func_80062B3C() % 3);
}

void func_800AF3E8(TmState *s, TmCard *c, s32 arg2, s32 random) {
    TmDirMask dirs;
    TmCardData *tbl;
    u8 v;
    s32 r;
    s32 n;
    s32 id;
    s32 i;
    s32 done;
    u8 *bp;
    s32 p0, p1, p2, p3, p4, p5, p6, p7;

    dirs = D_800A73B4;
    tbl = (TmCardData *)0x80103800;
    c->unk0 = arg2;
    id = arg2;
    if (random == 0) {
        c->unk2 = tbl[(u8)id].unk0;
        c->unk4 = tbl[(u8)id].unk2;
        c->unk5 = tbl[(u8)id].unk3;
    } else {
        v = tbl[(u8)id].unk0 >> 1;
        if (v != 0) {
            c->unk2 = v + func_80062B3C() % v;
        } else {
            c->unk2 = 0;
        }
        v = tbl[(u8)id].unk2 >> 1;
        if (v != 0) {
            c->unk4 = v + func_80062B3C() % v;
        } else {
            c->unk4 = 0;
        }
        v = tbl[(u8)id].unk3 >> 1;
        if (v != 0) {
            c->unk5 = v + func_80062B3C() % v;
        } else {
            c->unk5 = 0;
        }
    }
    r = func_80062B3C() % 100;
    p0 = s->unk4C8[0];
    p1 = s->unk4C8[1];
    p2 = s->unk4C8[2];
    p3 = s->unk4C8[3];
    p4 = s->unk4C8[4];
    p5 = s->unk4C8[5];
    p6 = s->unk4C8[6];
    p7 = s->unk4C8[7];
    if (r < p0) {
        n = 0;
    } else if (r < p0 + p1) {
        n = 1;
    } else if (r < p0 + p1 + p2) {
        n = 2;
    } else if (r < p0 + p1 + p2 + p3) {
        n = 3;
    } else if (r < p0 + p1 + p2 + p3 + p4) {
        n = 4;
    } else if (r < p0 + p1 + p2 + p3 + p4 + p5) {
        n = 5;
    } else if (r < p0 + p1 + p2 + p3 + p4 + p5 + p6) {
        n = 6;
    } else if (r < p0 + p1 + p2 + p3 + p4 + p5 + p6 + p7) {
        n = 7;
    } else {
        n = 8;
    }
    c->unk1 = 0;
    for (i = 0; i < n; i++) {
        done = 0;
        while (done == 0) {
            bp = &dirs.v[func_80062B3C() & 7];
            if (!(c->unk1 & *bp)) {
                c->unk1 |= *bp;
                done = 1;
            }
        }
    }
    c->unk3 = tbl[(u8)id].unk1;
    if (!(func_80062B3C() & 0x7F)) {
        c->unk3 = 2;
    }
}

void func_800AF760(TmState *s) {
    s32 i;
    s32 t;
    s32 d;
    TmCard *c;

    c = &s->unk10[5];
    for (i = 0; i < 5; i++) {
        d = s->unk2E8 - 0xF;
        t = i * 6 - d;
        if (t <= 0) {
            t = 0;
            if (s->unk590[i] == 0) {
                s->unk590[i] = 1;
                func_800B01E8(5);
            }
        }
        c[i].unk30 = 0x3C - i;
        c[i].unk10 = t * t + i * 0xE + 0x1A;
    }
    if (s->unk2E8 >= 0x29 && s->unk5AC != 0) {
        s->unk2E8 = 0;
        s->unk418 = 0;
        if (func_80062B3C() & 1) {
            s->unk434 = 0;
        } else {
            s->unk434 = 1;
        }
        s->unk0 = 0x1C;
        func_800B01E8(6);
    }
}

void func_800AF890(TmState *s) {
    s32 n;
    s32 i;
    TmCard *c;

    n = 0;
    c = &s->unk10[5];
    for (i = 0; i < 5; i++) {
        if (c[i].unk18 == 0) {
            c[i].unkC = 0x10;
            if (s->unk418 == 1) {
                if (n == s->unk2E8 / 4) {
                    c[i].unkC = 0x1A;
                }
            } else if (n == 0) {
                c[i].unkC = 0x1A;
            }
            n++;
        }
    }
    if (s->unk2E8 != 0 && s->unk418 == 1 && !(s->unk2E8 & 3)) {
        func_800B01E8(5);
    }
}

void *func_800AF95C(u32 *ot, SPRT *p, TmState *s, TmCard *card, u8 c) {
    s32 x;
    s32 y;

    x = card->unkC;
    y = card->unk10;
    if (card->unk1C != 0) {
        x -= 5;
        y -= 13;
    } else {
        y -= 2;
    }
    p = func_800AFB60(ot, p, (s32)s, x + 9, y + 0x28, card->unk2 >> 4, c);
    switch (card->unk3) {
    case 0:
        p = func_800AFB60(ot, p, (s32)s, x + 15, y + 0x28, 0x10, c);
        break;
    case 1:
        p = func_800AFB60(ot, p, (s32)s, x + 15, y + 0x28, 0x11, c);
        break;
    case 2:
        p = func_800AFB60(ot, p, (s32)s, x + 15, y + 0x28, 0x12, c);
        break;
    case 3:
        p = func_800AFB60(ot, p, (s32)s, x + 15, y + 0x28, 0xA, c);
        break;
    }
    p = func_800AFB60(ot, p, (s32)s, x + 0x15, y + 0x28, card->unk4 >> 4, c);
    p = func_800AFB60(ot, p, (s32)s, x + 0x1B, y + 0x28, card->unk5 >> 4, c);
    setlen((DR_TPAGE *)p, 1);
    ((DR_TPAGE *)p)->code[0] = (GetTPage(1, 0, 0x300, 0x100) & 0x9FF) | 0xE1000000;
    AddPrim(ot, p);
    return (DR_TPAGE *)p + 1;
}

SPRT *func_800AFB60(void *ot, SPRT *p, s32 unused, s32 x, s32 y, s32 n, s32 c) {
    setlen(p, 4);
    p->code = 0x64;
    p->v0 = 0xC0;
    p->clut = 0x3AC0;
    p->x0 = x;
    p->w = 8;
    p->h = 8;
    setRGB0(p, c, c, c);
    p->y0 = y;
    p->u0 = n * 8 + 0x50;
    AddPrim(ot, p);
    return p + 1;
}

void func_800AFBE4(TmState *s) {
    s32 a;
    s32 b;

    if (func_800AE830(s, 0) == 0 && func_800AE830(s, 1) == 0) {
        a = func_800AE94C(s, 0);
        b = func_800AE94C(s, 1);
        if (a > b) {
            func_80062750(0);
            func_800624A8();
            s->unk45C = 0;
            s->unk43C++;
        }
        if (a < b) {
            func_80062750(1);
            func_800624E8();
            s->unk45C = 1;
            s->unk440++;
        }
        if (a == b) {
            func_80062750(2);
            func_8006252C();
            s->unk45C = 2;
            s->unk444++;
        }
        s->unk2E8 = 0;
        s->unk0 = 3;
    }
}

void *func_800AFCE4(u32 *ot, SPRT *p, TmState *s) {
    u8 c;
    s32 snd;

    c = s->unk2E8 * 8;
    if (s->unk2E8 * 8 >= 0x100) {
        c = 0xFF;
    }
    if ((u8)(c - 0xF5) < 8) {
        switch (s->unk45C) {
        case 0:
            if (func_800AE94C(s, 0) != 10) {
                snd = 0xD;
            } else {
                snd = 8;
            }
            break;
        case 1:
            snd = 0xE;
            break;
        default:
            snd = 0xF;
            break;
        }
        func_800B01E8(snd);
    }
    switch (s->unk45C) {
    case 0:
        if (func_800AE94C(s, 0) == 10) {
            setlen(p, 4);
            p->code = 0x66;
            p->x0 = 0x4A;
            p->y0 = 0x58;
            p->w = 0xA8;
            p->h = 0x28;
            p->r0 = c;
            p->g0 = c;
            p->b0 = c;
            p->u0 = 0;
            p->v0 = 0x50;
            p->clut = 0x3A40;
            AddPrim(ot + 1, p);
            p++;
            setlen((DR_TPAGE *)p, 1);
            ((DR_TPAGE *)p)->code[0] = (GetTPage(1, 1, 0x280, 0x100) & 0x9FF) | 0xE1000400;
            AddPrim(ot + 1, p);
            p = (SPRT *)((DR_TPAGE *)p + 1);
            setlen(p, 4);
            p->code = 0x66;
            p->x0 = 0x4A;
            p->y0 = 0x58;
            p->w = 0xB0;
            p->h = 0x28;
            p->r0 = c;
            p->g0 = c;
            p->b0 = c;
            p->u0 = 0;
            p->v0 = 0x78;
            p->clut = 0x3A40;
            AddPrim(ot + 1, p);
            p++;
            setlen((DR_TPAGE *)p, 1);
            ((DR_TPAGE *)p)->code[0] = (GetTPage(1, 2, 0x280, 0x100) & 0x9FF) | 0xE1000400;
            AddPrim(ot + 1, p);
            p = (SPRT *)((DR_TPAGE *)p + 1);
        } else {
            setlen(p, 4);
            p->code = 0x66;
            p->x0 = 0x6C;
            p->y0 = 0x58;
            p->w = 0x68;
            p->h = 0x28;
            p->u0 = 0;
            p->v0 = 0;
            p->r0 = c;
            p->g0 = c;
            p->b0 = c;
            p->clut = 0x3A40;
            AddPrim(ot + 1, p);
            p++;
            setlen((DR_TPAGE *)p, 1);
            ((DR_TPAGE *)p)->code[0] = (GetTPage(1, 1, 0x280, 0x100) & 0x9FF) | 0xE1000400;
            AddPrim(ot + 1, p);
            p = (SPRT *)((DR_TPAGE *)p + 1);
            setlen(p, 4);
            p->code = 0x66;
            p->x0 = 0x6C;
            p->y0 = 0x58;
            p->w = 0x70;
            p->h = 0x28;
            p->r0 = c;
            p->g0 = c;
            p->b0 = c;
            p->u0 = 0x68;
            p->v0 = 0;
            p->clut = 0x3A40;
            AddPrim(ot + 1, p);
            p++;
            setlen((DR_TPAGE *)p, 1);
            ((DR_TPAGE *)p)->code[0] = (GetTPage(1, 2, 0x280, 0x100) & 0x9FF) | 0xE1000400;
            AddPrim(ot + 1, p);
            p = (SPRT *)((DR_TPAGE *)p + 1);
        }
        break;
    case 1:
        setlen(p, 4);
        p->code = 0x66;
        p->x0 = 0x6A;
        p->y0 = 0x58;
        p->w = 0x68;
        p->h = 0x28;
        p->r0 = c;
        p->g0 = c;
        p->b0 = c;
        p->u0 = 0;
        p->v0 = 0x28;
        p->clut = 0x3A40;
        AddPrim(ot + 1, p);
        p++;
        setlen((DR_TPAGE *)p, 1);
        ((DR_TPAGE *)p)->code[0] = (GetTPage(1, 1, 0x280, 0x100) & 0x9FF) | 0xE1000400;
        AddPrim(ot + 1, p);
        p = (SPRT *)((DR_TPAGE *)p + 1);
        setlen(p, 4);
        p->code = 0x66;
        p->x0 = 0x6A;
        p->y0 = 0x58;
        p->w = 0x70;
        p->h = 0x28;
        p->r0 = c;
        p->g0 = c;
        p->b0 = c;
        p->u0 = 0x68;
        p->v0 = 0x28;
        p->clut = 0x3A40;
        AddPrim(ot + 1, p);
        p++;
        setlen((DR_TPAGE *)p, 1);
        ((DR_TPAGE *)p)->code[0] = (GetTPage(1, 2, 0x280, 0x100) & 0x9FF) | 0xE1000400;
        AddPrim(ot + 1, p);
        p = (SPRT *)((DR_TPAGE *)p + 1);
        break;
    default:
        setlen(p, 4);
        p->code = 0x66;
        p->x0 = 0x5C;
        p->y0 = 0x58;
        p->w = 0x88;
        p->h = 0x28;
        p->r0 = c;
        p->g0 = c;
        p->b0 = c;
        p->u0 = 0;
        p->v0 = 0xA0;
        p->clut = 0x3A40;
        AddPrim(ot + 1, p);
        p++;
        setlen((DR_TPAGE *)p, 1);
        ((DR_TPAGE *)p)->code[0] = (GetTPage(1, 1, 0x280, 0x100) & 0x9FF) | 0xE1000400;
        AddPrim(ot + 1, p);
        p = (SPRT *)((DR_TPAGE *)p + 1);
        setlen(p, 4);
        p->code = 0x66;
        p->x0 = 0x5C;
        p->y0 = 0x58;
        p->w = 0x90;
        p->h = 0x28;
        p->r0 = c;
        p->g0 = c;
        p->b0 = c;
        p->u0 = 0;
        p->v0 = 0xC8;
        p->clut = 0x3A40;
        AddPrim(ot + 1, p);
        p++;
        setlen((DR_TPAGE *)p, 1);
        ((DR_TPAGE *)p)->code[0] = (GetTPage(1, 2, 0x280, 0x100) & 0x9FF) | 0xE1000400;
        AddPrim(ot + 1, p);
        p = (SPRT *)((DR_TPAGE *)p + 1);
        break;
    }
    return p;
}

void func_800B01E8(s32 arg0) {
    func_800A78EC(0xD000, D_800B3C8C[arg0], 0, 0x80, 0x7F);
}

void func_800B022C(s32 arg0) {
    func_800A78EC(0x5100, D_800B3C8C[arg0], 0, 0, 0);
}

void func_800B026C(TmState *arg0) {
    if (arg0->unk5AC == 0) {
        if (arg0->unk570 != 0) {
            arg0->unk2F0[arg0->unk56C].unk0 = 0;
            if (arg0->unk5A8 >= 9) {
                func_800B01E8(7);
                arg0->unk5A8 = 0;
                if (++arg0->unk56C == arg0->unk570) {
                    arg0->unk5AC = 1;
                }
            }
        } else {
            arg0->unk5AC = 1;
        }
    }
    arg0->unk5A8++;
}

INCLUDE_ASM("asm/us/ovl_0d/nonmatchings/ovl_0d", func_800B030C);

void func_800B0618(TmState *s) {
    s32 i;
    u32 r;
    s32 n;
    s32 x;
    s32 y;
    TmCell *cell;
    TmUnk2F0 *base;

    base = s->unk2F0;
    for (i = 0; i < 6; i++) {
        base[i].unk0 = 1;
        base[i].unk4 = 0;
    }
    r = func_80062B3C() % 100;
    if (r < 3) {
        n = 0;
    } else if (r >= 3 && r < 7) {
        n = 1;
    } else if (r >= 7 && r < 12) {
        n = 2;
    } else if (r >= 12 && r < 17) {
        n = 3;
    } else if (r >= 17 && r < 27) {
        n = 4;
    } else if (r >= 27 && r < 97) {
        n = 5;
    } else {
        n = 6;
    }
    s->unk570 = n;
    s->unk56C = 0;
    for (i = 0; i < n; i++) {
        do {
            x = func_80062B3C() & 3;
            y = func_80062B3C() & 3;
            cell = func_800B19D4(s, x, y);
        } while (cell->unk0 != 1);
        cell->unk0 = 2;
        base[i].unk4 = x * 43 + 0x48;
        base[i].unk0 = 1;
        base[i].unk8 = y * 52 + 8;
        base[i].unkC = func_80062B3C() & 1;
    }
}

void *func_800B07DC(u32 *ot, void *p, TmState *s) {
    if (func_800AE830(s, 1) == 0) {
        s->unk0 = 2;
    } else if (s->unk418 == 0) {
        if (s->unk2E8 > s->unk5A4) {
            s->unk2E8 = 0;
            s->unk418 = 1;
            s->unk5A4 = (func_80062B3C() % (func_800AE830(s, 1) + 1)) * 4 - 2;
        }
    } else if (s->unk2E8 > s->unk5A4) {
        s->unk0 = 2;
    }
    return p;
}

void func_800B08CC(TmState *s) {
    s32 score[80];
    TmPos res;
    s32 bc;
    s32 by;
    s32 bx;
    s32 wgt;
    s32 mul;
    s32 best;
    s32 cnt;
    s32 k;
    s32 c;
    s32 x;
    s32 y;
    s32 v;
    s32 f;
    s32 r;
    TmCard *card;
    TmCell *cell;
    TmCard *sel;

    if (func_800AE830(s, 1) == 0) {
        return;
    }
    if (func_800AE998(s, 1) == func_800AE998(s, 0)) {
        wgt = 0x40;
        mul = 1;
    } else if (func_800AE998(s, 1) >= func_800AE998(s, 0)) {
        wgt = 0x43;
        mul = 1;
    } else {
        wgt = 1;
        mul = 5;
    }
    if (s->unk44C == 0 && func_80062B3C() % 100 >= 0x1D) {
        wgt = 0;
        mul = 0;
    }
    if (s->unk44C == 1 && func_80062B3C() % 100 >= 0x3F) {
        wgt = 0;
        mul = 0;
    }
    if (s->unk44C == 2 && func_80062B3C() % 100 >= 0x5D) {
        wgt = 0;
        mul = 0;
    }
    best = -0x7FFF;
    cnt = 0;
    k = 0;
    for (c = 0; c < func_800AE830(s, 1); c++) {
        for (x = 0; x < 4; x++) {
            for (y = 0; y < 4; y++) {
                score[k] = 0;
                if (func_800B19D4(s, x, y)->unk0 != 1) {
                    score[k] = -0x7FFF;
                } else {
                    card = func_800AE9D0(s, 1, c);
                    v = func_800B0EB8(s, 1, x, y, card);
                    if (v > 0) {
                        score[k] += v * wgt;
                    }
                    if (v < 0) {
                        func_800AC4C8((s32)s, &res, card, func_800B1010(s, x, y, card));
                        r = res.unk0 * 100 / (res.unk0 + res.unk4);
                        if (s->unk44C == 3) {
                            mul = 1;
                            if (r < 0x3C) {
                                r = r - 100;
                            } else {
                                r = 0x91 - r;
                            }
                        }
                        score[k] += r * mul;
                    }
                }
                if (score[k] == best) {
                    cnt++;
                }
                if (score[k] > best) {
                    best = score[k];
                    cnt = 1;
                }
                k++;
            }
        }
    }
    bx = 0;
    best = -0x7FFF;
    k = 0;
    by = 0;
    bc = 0;
    for (c = 0; c < func_800AE830(s, 1); c++) {
        for (x = 0; x < 4; x++) {
            for (y = 0; y < 4; y++) {
                f = best < score[k];
                if (best == score[k] && func_80062B3C() % 1000 < 1000 / cnt) {
                    f++;
                }
                if (f) {
                    bx = x;
                    bc = c;
                    by = y;
                    best = score[k];
                }
                k++;
            }
        }
    }
    sel = func_800AE9D0(s, 1, bc);
    cell = func_800B19D4(s, bx, by);
    cell->unk0 = 0;
    cell->unk4 = sel;
    sel->unkC = bx * 43 + 0x48;
    sel->unk10 = by * 52 + 8;
    sel->unk8 = 0;
    sel->unk18 = 1;
    s->unk424 = bx;
    s->unk428 = by;
    func_800B01E8(5);
    func_800ABEC4(s);
}

s32 func_800B0E58(TmState *arg0, s32 arg1, s32 arg2, s32 arg3) {
    TmCell *cell = func_800B19D4(arg0, arg2, arg3);
    if (cell->unk0 == 0) {
        TmCard *c = cell->unk4;
        if (c->unk14 != arg1) {
            c->unk14 = arg1;
        }
    }
    return 0;
}

s32 func_800B0EB8(TmState *s, s32 owner, s32 x, s32 y, TmCard *c) {
    TmPos p;
    TmDirMask m = D_800A73BC;
    TmCell *cell;
    TmCard *t;
    s32 i;
    s32 win;
    s32 lose;
    u8 bits;

    win = 0;
    lose = 0;
    if (c == (TmCard *)-1) {
        c = func_800AEE08(s, x, y);
    }
    bits = c->unk1;
    for (i = 0; i < 8; i++, bits >>= 1) {
        if (bits & 1) {
            p.unk0 = x;
            p.unk4 = y;
            func_800B13C0(&p, i);
            cell = func_800B19D4(s, p.unk0, p.unk4);
            if (cell->unk0 == 0) {
                t = cell->unk4;
                if (t->unk14 != owner) {
                    if (t->unk1 & m.v[i]) {
                        win++;
                    } else {
                        lose++;
                    }
                }
            }
        }
    }
    if (win != 0) {
        return -win;
    }
    if (lose != 0) {
        return lose;
    }
    return 0;
}

TmCard *func_800B1010(TmState *s, s32 x, s32 y, TmCard *c) {
    TmPos p;
    TmDirMask m = D_800A73BC;
    TmCell *cell;
    TmCard *t;
    s32 i;
    u8 bits;

    bits = c->unk1;
    for (i = 0; i < 8; i++, bits >>= 1) {
        if (bits & 1) {
            p.unk0 = x;
            p.unk4 = y;
            func_800B13C0(&p, i);
            cell = func_800B19D4(s, p.unk0, p.unk4);
            if (cell->unk0 == 0) {
                t = cell->unk4;
                if (t->unk14 != 1 && (t->unk1 & m.v[i])) {
                    return t;
                }
            }
        }
    }
    return (TmCard *)-1;
}

INCLUDE_ASM("asm/us/ovl_0d/nonmatchings/ovl_0d", func_800B1110);

TmCard *func_800B12F0(TmState *s, TmPos *p, s32 dir, s32 owner) {
    TmDirMask m = D_800A73BC;
    TmCell *cell;
    TmCard *c;

    func_800B13C0(p, dir);
    cell = func_800B19D4(s, p->unk0, p->unk4);
    if (cell->unk0 == 0) {
        c = cell->unk4;
        if (c->unk14 != owner && (c->unk1 & m.v[dir])) {
            return c;
        }
    }
    return NULL;
}

void func_800B13C0(TmPos *p, s32 dir) {
    if (dir == 0) {
        p->unk4--;
    } else if (dir == 1) {
        p->unk0++;
        p->unk4--;
    } else if (dir == 2) {
        p->unk0++;
    } else if (dir == 3) {
        p->unk0++;
        p->unk4++;
    } else if (dir == 4) {
        p->unk4++;
    } else if (dir == 5) {
        p->unk0--;
        p->unk4++;
    } else if (dir == 6) {
        p->unk0--;
    } else if (dir == 7) {
        p->unk0--;
        p->unk4--;
    }
}

void *func_800B14AC(u32 *ot, SPRT_16 *p, TmState *s, s32 x, s32 y, u8 mask, s32 flip, u8 c, s32 blink) {
    TmArrowTbl tbl;
    s32 i;
    u8 bits;
    u8 col;

    tbl = D_800A73C4;
    bits = s->unk50C;
    for (i = 0; i < 8; i++) {
        if (mask & 1) {
            setlen(p, 3);
            p->code = 0x74;
            if (flip == 0) {
                p->x0 = tbl.v[i].unk0 + x;
                p->y0 = tbl.v[i].unk4 + y;
            } else {
                p->x0 = tbl.v[i].unk8 + x;
                p->y0 = tbl.v[i].unkC + y;
            }
            col = c;
            if (blink != 0 && (bits & 1)) {
                col = 0;
                if (!(s->unk2E8 & 2)) {
                    col = 0xFF;
                }
            }
            p->r0 = col;
            p->g0 = col;
            p->b0 = col;
            p->u0 = tbl.v[i].unk10;
            p->v0 = tbl.v[i].unk14;
            p->clut = 0x3A80;
            AddPrim(ot, p);
            p++;
            setlen((DR_TPAGE *)p, 1);
            ((DR_TPAGE *)p)->code[0] = (GetTPage(1, 0, 0x300, 0x100) & 0x9FF) | 0xE1000000;
            AddPrim(ot, p);
            p = (SPRT_16 *)((DR_TPAGE *)p + 1);
        }
        mask >>= 1;
        bits >>= 1;
    }
    return p;
}

INCLUDE_ASM("asm/us/ovl_0d/nonmatchings/ovl_0d", func_800B16A4);

TmCell *func_800B19D4(TmState *arg0, s32 arg1, s32 arg2) {
    TmCell(*board)[4] = arg0->unk268;
    if (arg1 < 0) {
        return &D_800B3CE8;
    }
    if (arg1 >= 4) {
        return &D_800B3CE8;
    }
    if (arg2 < 0) {
        return &D_800B3CE8;
    }
    if (arg2 >= 4) {
        return &D_800B3CE8;
    }
    return &board[arg2][arg1];
}

void func_800B1A24(TmState *arg0) {
    s32 x, y;
    TmCell(*b)[4] = arg0->unk268;
    D_800B3CE8.unk0 = 3;
    for (y = 0; y < 4; y++) {
        for (x = 3; x >= 0; x--) {
            b[y][x].unk0 = 1;
        }
    }
    func_800B0618(arg0);
}

INCLUDE_RODATA("asm/us/ovl_0d/nonmatchings/ovl_0d", D_800A73B4);

INCLUDE_RODATA("asm/us/ovl_0d/nonmatchings/ovl_0d", D_800A73BC);

INCLUDE_RODATA("asm/us/ovl_0d/nonmatchings/ovl_0d", D_800A73C4);

void *func_800B1A84(u32 *ot, SPRT *p, TmState *s) {
    u8 buf[0x20];
    s32 i;
    s32 n;
    s32 m;
    s32 k;
    s32 c;

    SetSprt(p);
    if ((u32)(s->unk0 - 0x12) < 2 || s->unk0 == 0x23 || s->unk0 == 0x1B) {
        SetSprt(p);
        p->w = 0x30;
        p->h = 0x18;
        p->u0 = 0xB8;
        p->v0 = 0x78;
        p->x0 = 0x58;
        p->y0 = 0xA;
        p->r0 = 0x80;
        p->g0 = 0x80;
        p->b0 = 0x80;
        p->clut = 0x3A40;
        AddPrim(ot + 0xFFA, p);
        p++;
        SetSprt(p);
        p->w = 0x61;
        p->h = 0x20;
        p->u0 = 0x94;
        p->v0 = 0xD2;
        p->x0 = 0x8C;
        p->y0 = 0xC;
        p->r0 = 0x80;
        p->g0 = 0x80;
        p->b0 = 0x80;
        p->clut = 0x3A40;
        AddPrim(ot + 0xFFA, p);
    } else {
        SetSprt(p);
        p->w = 0x4E;
        p->h = 0x15;
        p->u0 = 0x98;
        p->v0 = 0xBC;
        p->x0 = 0x79;
        p->y0 = 0xE;
        p->r0 = 0x80;
        p->g0 = 0x80;
        p->b0 = 0x80;
        p->clut = 0x3A40;
        AddPrim(ot + 0xFFA, p);
        p++;
        SetSprt(p);
        p->w = 0x10;
        p->h = 0x10;
        p->u0 = 0xF0;
        p->v0 = 0xB8;
        p->x0 = 0xCA;
        p->y0 = 0x12;
        if (!(s->unk2E8 & 0x20)) {
            p->r0 = 0x80;
            p->g0 = 0x80;
            p->b0 = 0x80;
        } else {
            p->r0 = 0x40;
            p->g0 = 0x40;
            p->b0 = 0x40;
        }
        p->clut = 0x3A40;
        AddPrim(ot + 0xFFA, p);
        p++;
        SetSprt(p);
        p->w = 0x10;
        p->h = 0x10;
        p->u0 = 0xF0;
        p->v0 = 0xB8;
        p->x0 = 0x66;
        p->y0 = 0x12;
        if (!(s->unk2E8 & 0x20)) {
            p->r0 = 0x80;
            p->g0 = 0x80;
            p->b0 = 0x80;
        } else {
            p->r0 = 0x40;
            p->g0 = 0x40;
            p->b0 = 0x40;
        }
        p->clut = 0x3A40;
        AddPrim(ot + 0xFFA, p);
    }
    p++;
    setlen((DR_TPAGE *)p, 1);
    ((DR_TPAGE *)p)->code[0] = (GetTPage(1, 0, 0x280, 0x100) & 0x9FF) | 0xE1000000;
    AddPrim(ot + 0xFFA, p);
    p = (SPRT *)((DR_TPAGE *)p + 1);

    for (i = 0; i < 100; i++) {
        m = n = func_80062560(i);
        if (n != 1) {
            c = m % 10;
            if (c != 0 || n >= 10) {
                SetSprt(p);
                p->r0 = 0x80;
                p->g0 = 0x80;
                p->b0 = 0x80;
                p->clut = 0x3B40;
                p->w = 4;
                p->h = 5;
                p->u0 = (c % 5) * 4;
                p->v0 = (c / 5) * 8 - 0x10;
                p->x0 = (i / 10) * 15 + 0x1B;
                p->y0 = (i % 10) * 15 + 0x35;
                AddPrim(ot + 11, p);
                p++;
            }
            m /= 10;
            c = m / 10;
            if (m != c * 10 || n >= 100) {
                c = m;
                c %= 10;
                SetSprt(p);
                p->w = 4;
                p->h = 5;
                p->r0 = 0x80;
                p->g0 = 0x80;
                p->b0 = 0x80;
                p->clut = 0x3B40;
                p->u0 = (c % 5) * 4;
                p->v0 = (c / 5) * 8 - 0x10;
                p->x0 = (i / 10) * 15 + 0x17;
                p->y0 = (i % 10) * 15 + 0x35;
                AddPrim(ot + 11, p);
                p++;
                if (n >= 100) {
                    SetSprt(p);
                    p->w = 4;
                    p->h = 5;
                    p->u0 = 4;
                    p->v0 = 0xF0;
                    p->x0 = (i / 10) * 15 + 0x13;
                    p->y0 = (i % 10) * 15 + 0x35;
                    p->r0 = 0x80;
                    p->g0 = 0x80;
                    p->b0 = 0x80;
                    p->clut = 0x3B40;
                    AddPrim(ot + 11, p);
                    p++;
                }
            }
        }
        SetSprt16((SPRT_16 *)p);
        ((SPRT_16 *)p)->x0 = (i / 10) * 15 + 0x12;
        ((SPRT_16 *)p)->y0 = (i % 10) * 15 + 0x2D;
        if (n == 0) {
            ((SPRT_16 *)p)->u0 = 0;
            ((SPRT_16 *)p)->v0 = 0xE0;
        } else {
            k = func_800627BC(i);
            if (n >= 2) {
                k += 7;
            }
            ((SPRT_16 *)p)->u0 = k * 16 + 0x10;
            ((SPRT_16 *)p)->v0 = 0xE0;
        }
        do {
            ((SPRT_16 *)p)->r0 = 0x80;
            ((SPRT_16 *)p)->g0 = 0x80;
            ((SPRT_16 *)p)->b0 = 0x80;
            ((SPRT_16 *)p)->clut = 0x3B40;
            AddPrim(ot + 11, p);
        } while (0);
        p = (SPRT *)((SPRT_16 *)p + 1);
    }

    setlen((DR_TPAGE *)p, 1);
    ((DR_TPAGE *)p)->code[0] = (GetTPage(0, 0, 0x340, 0x1E0) & 0x9FF) | 0xE1000000;
    AddPrim(ot + 11, p);
    p = (SPRT *)((DR_TPAGE *)p + 1);
    SetSprt(p);
    p->w = 0xA8;
    p->h = 8;
    p->u0 = 0x58;
    p->v0 = 0xF0;
    p->x0 = 9;
    p->y0 = 0x29;
    p->r0 = 0x80;
    p->g0 = 0x80;
    p->b0 = 0x80;
    p->clut = 0x3B80;
    AddPrim(ot + 11, p);
    p++;
    SetSprt(p);
    p->w = 0xA8;
    p->h = 8;
    p->u0 = 0x58;
    p->v0 = 0xF8;
    p->x0 = 9;
    p->y0 = 0xBE;
    p->r0 = 0x80;
    p->g0 = 0x80;
    p->b0 = 0x80;
    p->clut = 0x3B80;
    AddPrim(ot + 11, p);
    p++;
    SetSprt(p);
    p->w = 8;
    p->h = 0x98;
    p->u0 = 0xF0;
    p->v0 = 0x58;
    p->x0 = 0xD;
    p->y0 = 0x2B;
    p->r0 = 0x80;
    p->g0 = 0x80;
    p->b0 = 0x80;
    p->clut = 0x3B80;
    AddPrim(ot + 11, p);
    p++;
    SetSprt(p);
    p->w = 8;
    p->h = 0x98;
    p->u0 = 0xF8;
    p->v0 = 0x58;
    p->x0 = 0xA5;
    p->y0 = 0x2B;
    p->r0 = 0x80;
    p->g0 = 0x80;
    p->b0 = 0x80;
    p->clut = 0x3B80;
    AddPrim(ot + 11, p);
    p++;
    setlen((DR_TPAGE *)p, 1);
    ((DR_TPAGE *)p)->code[0] = (GetTPage(0, 0, 0x300, 0) & 0x9FF) | 0xE1000000;
    AddPrim(ot + 11, p);
    p = (SPRT *)((DR_TPAGE *)p + 1);
    setlen((TILE *)p, 3);
    ((TILE *)p)->code = 0x60;
    ((TILE *)p)->x0 = 0x12;
    ((TILE *)p)->y0 = 0x2D;
    ((TILE *)p)->w = 0x95;
    ((TILE *)p)->h = 0x95;
    ((TILE *)p)->r0 = 0x20;
    ((TILE *)p)->g0 = 0x18;
    ((TILE *)p)->b0 = 0x18;
    AddPrim(ot + 11, p);
    p = (SPRT *)((TILE *)p + 1);
    setlen((DR_TPAGE *)p, 1);
    ((DR_TPAGE *)p)->code[0] = (GetTPage(0, 2, 0, 0) & 0x9FF) | 0xE1000000;
    AddPrim(ot + 11, p);
    p = (SPRT *)((DR_TPAGE *)p + 1);

    func_8002B56C(0x19, 0xCB, 0xFFA, "Stock:", 7);
    func_8002BAE4(D_800B3CF0, buf, 3, 3);
    func_8002B5B4(0x43, 0xC9, 0xFFA, buf, 0);
    func_8002B56C(0x65, 0xCB, 0xFFA, "Type:", 7);
    func_8002BAE4(D_800B3CF4, buf, 3, 3);
    func_8002B5B4(0x89, 0xC9, 0xFFA, buf, 0);

    if ((u32)(s->unk0 - 0x12) < 2 || s->unk0 == 0x23 || s->unk0 == 0x1B) {

        func_8002B56C(0xBC, 0x42, 0xFFA, "WIN", 7);
        func_8002BAE4(func_80062498(), buf, 4, 1);
        func_8002B56C(0xDC, 0x42, 0xFFA, buf, 0);
        func_8002B56C(0xBC, 0x4C, 0xFFA, "LOSE", 7);
        func_8002BAE4(func_800624D8(), buf, 4, 1);
        func_8002B56C(0xDC, 0x4C, 0xFFA, buf, 0);
        func_8002B56C(0xBC, 0x56, 0xFFA, "DRAW", 7);
        func_8002BAE4(func_8006251C(), buf, 4, 1);
        func_8002B56C(0xDC, 0x56, 0xFFA, buf, 0);

        setlen(p, 3);
        ((SPRT_8 *)p)->code = 0x74;
        ((SPRT_8 *)p)->x0 = 0xB5;
        ((SPRT_8 *)p)->y0 = 0x3D;
        ((SPRT_8 *)p)->r0 = 0x80;
        ((SPRT_8 *)p)->g0 = 0x80;
        ((SPRT_8 *)p)->b0 = 0x80;
        ((SPRT_8 *)p)->u0 = 0x38;
        ((SPRT_8 *)p)->v0 = 0xF0;
        ((SPRT_8 *)p)->clut = 0x3B80;
        AddPrim(ot + 0xFFA, p);
        p = (SPRT *)((SPRT_8 *)p + 1);
        setlen(p, 3);
        ((SPRT_8 *)p)->code = 0x74;
        ((SPRT_8 *)p)->x0 = 0xF6;
        ((SPRT_8 *)p)->y0 = 0x3D;
        ((SPRT_8 *)p)->r0 = 0x80;
        ((SPRT_8 *)p)->g0 = 0x80;
        ((SPRT_8 *)p)->b0 = 0x80;
        ((SPRT_8 *)p)->u0 = 0x48;
        ((SPRT_8 *)p)->v0 = 0xF0;
        ((SPRT_8 *)p)->clut = 0x3B80;
        AddPrim(ot + 0xFFA, p);
        p = (SPRT *)((SPRT_8 *)p + 1);
        setlen(p, 3);
        ((SPRT_8 *)p)->code = 0x74;
        ((SPRT_8 *)p)->x0 = 0xB5;
        ((SPRT_8 *)p)->y0 = 0x5A;
        ((SPRT_8 *)p)->r0 = 0x80;
        ((SPRT_8 *)p)->g0 = 0x80;
        ((SPRT_8 *)p)->b0 = 0x80;
        ((SPRT_8 *)p)->u0 = 0x38;
        ((SPRT_8 *)p)->v0 = 0xF8;
        ((SPRT_8 *)p)->clut = 0x3B80;
        AddPrim(ot + 0xFFA, p);
        p = (SPRT *)((SPRT_8 *)p + 1);
        setlen(p, 3);
        ((SPRT_8 *)p)->code = 0x74;
        ((SPRT_8 *)p)->x0 = 0xF6;
        ((SPRT_8 *)p)->y0 = 0x5A;
        ((SPRT_8 *)p)->r0 = 0x80;
        ((SPRT_8 *)p)->g0 = 0x80;
        ((SPRT_8 *)p)->b0 = 0x80;
        ((SPRT_8 *)p)->u0 = 0x48;
        ((SPRT_8 *)p)->v0 = 0xF8;
        ((SPRT_8 *)p)->clut = 0x3B80;
        AddPrim(ot + 0xFFA, p);
        p = (SPRT *)((SPRT_8 *)p + 1);
        for (i = 0; i < 8; i++) {
            setlen(p, 3);
            ((SPRT_8 *)p)->code = 0x74;
            ((SPRT_8 *)p)->y0 = 0x3D;
            ((SPRT_8 *)p)->x0 = i * 8 + 0xB9;
            ((SPRT_8 *)p)->r0 = 0x80;
            ((SPRT_8 *)p)->g0 = 0x80;
            ((SPRT_8 *)p)->b0 = 0x80;
            ((SPRT_8 *)p)->u0 = 0x40;
            ((SPRT_8 *)p)->v0 = 0xF0;
            ((SPRT_8 *)p)->clut = 0x3B80;
            AddPrim(ot + 0xFFA, p);
            p = (SPRT *)((SPRT_8 *)p + 1);
            setlen(p, 3);
            ((SPRT_8 *)p)->code = 0x74;
            ((SPRT_8 *)p)->y0 = 0x5A;
            ((SPRT_8 *)p)->x0 = i * 8 + 0xB9;
            ((SPRT_8 *)p)->r0 = 0x80;
            ((SPRT_8 *)p)->g0 = 0x80;
            ((SPRT_8 *)p)->b0 = 0x80;
            ((SPRT_8 *)p)->u0 = 0x40;
            ((SPRT_8 *)p)->v0 = 0xF8;
            ((SPRT_8 *)p)->clut = 0x3B80;
            AddPrim(ot + 0xFFA, p);
            p = (SPRT *)((SPRT_8 *)p + 1);
        }
        for (i = 0; i < 3; i++) {
            setlen(p, 3);
            ((SPRT_8 *)p)->code = 0x74;
            ((SPRT_8 *)p)->x0 = 0xB5;
            ((SPRT_8 *)p)->y0 = i * 8 + 0x43;
            ((SPRT_8 *)p)->r0 = 0x80;
            ((SPRT_8 *)p)->g0 = 0x80;
            ((SPRT_8 *)p)->b0 = 0x80;
            ((SPRT_8 *)p)->u0 = 0x50;
            ((SPRT_8 *)p)->v0 = 0xF8;
            ((SPRT_8 *)p)->clut = 0x3B80;
            AddPrim(ot + 0xFFA, p);
            p = (SPRT *)((SPRT_8 *)p + 1);
            setlen(p, 3);
            ((SPRT_8 *)p)->code = 0x74;
            ((SPRT_8 *)p)->x0 = 0xF6;
            ((SPRT_8 *)p)->y0 = i * 8 + 0x43;
            ((SPRT_8 *)p)->r0 = 0x80;
            ((SPRT_8 *)p)->g0 = 0x80;
            ((SPRT_8 *)p)->b0 = 0x80;
            ((SPRT_8 *)p)->u0 = 0x50;
            ((SPRT_8 *)p)->v0 = 0xF0;
            ((SPRT_8 *)p)->clut = 0x3B80;
            AddPrim(ot + 0xFFA, p);
            p = (SPRT *)((SPRT_8 *)p + 1);
        }
        setlen((TILE *)p, 3);
        ((TILE *)p)->code = 0x60;
        ((TILE *)p)->x0 = 0xB8;
        ((TILE *)p)->y0 = 0x41;
        ((TILE *)p)->w = 0x46;
        ((TILE *)p)->h = 0x1E;
        ((TILE *)p)->r0 = 0x20;
        ((TILE *)p)->g0 = 0x18;
        ((TILE *)p)->b0 = 0x18;
        AddPrim(ot + 0xFFA, p);
        p = (SPRT *)((TILE *)p + 1);
        setlen((DR_TPAGE *)p, 1);
        ((DR_TPAGE *)p)->code[0] = (GetTPage(0, 0, 0x300, 0) & 0x9FF) | 0xE1000000;
        AddPrim(ot + 0xFFA, p);
        p = (SPRT *)((DR_TPAGE *)p + 1);
    }
    if (s->unk47C != 5 && s->unk0 != 0x16 && func_80062560(func_800AF268(s)) >= 2) {
        func_8002BAE4(s->unk4B4 + 1, buf, 3, 1);
        func_8002B56C(0xBF, 0xBA, 0xFFA, buf, 0);
        func_8002B56C(0xD4, 0xBA, 0xFFA, D_800A74A8, 0);
        func_8002BAE4(func_80062560(func_800AF268(s)), buf, -1, 1);
        func_8002B56C(0xDB, 0xBA, 0xFFA, buf, 0);
        func_8002B5FC(0xB5, 0xB6, 0xFFA, func_80035774(0x1E), 0);
        func_8002B5FC(0xF1, 0xB6, 0xFFA, func_80035774(0x1C), 0);
    }
    if (s->unk47C == 0) {
        D_800B3CF4 = func_80062638();
        D_800B3CF0 = func_800625F8();
    }
    return p;
}

void func_800B295C(TmState *arg0) {
    D_800B3CF4 = func_80062638();
    D_800B3CF0 = func_800625F8();
    arg0->unk0 = 0x12;
}

void func_800B299C(Window *arg0) {
    func_8002B5FC(0x8A, 0x10, arg0->unk18, func_80035774(0x13), 0);
}

void func_800B29DC(TmState *arg0) {
    arg0->unk474 = 0;
    arg0->unk478 = 0;
    arg0->unk47C = 0;
    arg0->unk4B4 = 0;
}

void *func_800B29F0(u32 *ot, void *prim, TmState *s, TmCard *card, u8 c, s32 flip) {
    u8 id;
    s32 page;
    s32 n;
    s32 u;
    s32 v;

    id = card->unk0;
    page = (u8)(id / 30);
    n = (u8)(id % 30);
    u = (n % 6) * 42;
    v = (n / 6) * 51;
    if (flip == 0) {
        setlen((SPRT *)prim, 4);
        ((SPRT *)prim)->code = 0x64;
        ((SPRT *)prim)->x0 = card->unkC;
        ((SPRT *)prim)->y0 = card->unk10;
        ((SPRT *)prim)->w = 0x2A;
        ((SPRT *)prim)->h = 0x33;
        ((SPRT *)prim)->r0 = c;
        ((SPRT *)prim)->g0 = c;
        ((SPRT *)prim)->b0 = c;
        ((SPRT *)prim)->u0 = u;
        ((SPRT *)prim)->v0 = v;
        ((SPRT *)prim)->clut = getClut(0x180, id + 0x19C);
        AddPrim(ot, prim);
        prim = (SPRT *)prim + 1;
        setlen((DR_TPAGE *)prim, 1);
        ((DR_TPAGE *)prim)->code[0] = (GetTPage(1, 0, page << 7, 0x100) & 0x9FF) | 0xE1000400;
        AddPrim(ot, prim);
        prim = (DR_TPAGE *)prim + 1;
    } else {
        SetPolyFT4(prim);
        ((POLY_FT4 *)prim)->x0 = card->unkC;
        ((POLY_FT4 *)prim)->y0 = card->unk10;
        ((POLY_FT4 *)prim)->x1 = card->unkC + 0x22;
        ((POLY_FT4 *)prim)->y1 = card->unk10;
        ((POLY_FT4 *)prim)->x2 = card->unkC;
        ((POLY_FT4 *)prim)->y2 = card->unk10 + 0x29;
        ((POLY_FT4 *)prim)->x3 = card->unkC + 0x22;
        ((POLY_FT4 *)prim)->y3 = card->unk10 + 0x29;
        ((POLY_FT4 *)prim)->u0 = u;
        ((POLY_FT4 *)prim)->v0 = v;
        ((POLY_FT4 *)prim)->u1 = u + 0x2A;
        ((POLY_FT4 *)prim)->v1 = v;
        ((POLY_FT4 *)prim)->u2 = u;
        ((POLY_FT4 *)prim)->v2 = v + 0x33;
        ((POLY_FT4 *)prim)->u3 = u + 0x2A;
        ((POLY_FT4 *)prim)->v3 = v + 0x33;
        ((POLY_FT4 *)prim)->r0 = c;
        ((POLY_FT4 *)prim)->g0 = c;
        ((POLY_FT4 *)prim)->b0 = c;
        ((POLY_FT4 *)prim)->clut = getClut(0x180, id + 0x19C);
        ((POLY_FT4 *)prim)->tpage = ((page * 2) & 0xE) | 0x90;
        AddPrim(ot, prim);
        prim = (POLY_FT4 *)prim + 1;
    }
    return prim;
}

void *func_800B2C5C(u32 *ot, SPRT *p, TmState *s) {
    TmCoinFrames f = D_800A74AC;
    s32 dx;
    s32 dy;
    s32 flip;
    s32 t;
    s32 frame;
    s32 n;
    u8 c;
    s32 k;
    TmCoinFrame *fr;

    dx = 0;
    dy = 0;
    t = s->unk2E8;
    flip = 0;
    if (s->unk418 == 0) {
        frame = t & 7;
        if (t >= 0x29 && ((s->unk434 == 0 && frame == 7) || (s->unk434 == 1 && frame == 3))) {
            s->unk418 = 1;
            s->unk2E8 = 0;
            func_800B022C(6);
        }
    } else {
        frame = 3;
        if (s->unk434 == 0) {
            frame = 7;
        }
        if (t >= 0x1F) {
            n = t - 0x1E;
            flip = 1;
            dy = -(n * 82) / 12;
            dx = (n * 124) / 12;
            if (s->unk434 == 1) {
                dy = -(n * 66) / 12;
                dx = (n * -119) / 12;
            }
        }
        if (t >= 0x2A) {
            if (s->unk434 == 0) {
                s->unk564 = 0;
                s->unk0 = 0;
            } else {
                s->unk2E8 = 0;
                s->unk418 = 0;
                s->unk5A4 = (func_80062B3C() & 0xF) + 0xF;
                s->unk0 = 0x1E;
            }
        }
    }
    SetSprt(p);
    if (flip != 0) {
        p->code |= 2;
    }
    fr = &f.v[frame];
    p->w = fr->unkC;
    p->h = 0x28;
    p->u0 = fr->unk4;
    p->v0 = fr->unk8;
    k = dx + 0x9D;
    p->x0 = fr->unk0 + k;
    p->y0 = dy + 0x5C;
    if (flip == 0) {
        c = 0x80;
        p->r0 = c;
        p->g0 = c;
        p->b0 = c;
    } else {
        c = (0x2A - t) * 10;
        p->r0 = c;
        p->g0 = c;
        p->b0 = c;
    }
    p->clut = 0x3AC0;
    AddPrim(ot + 10, p);
    p++;
    setlen((DR_TPAGE *)p, 1);
    ((DR_TPAGE *)p)->code[0] = (GetTPage(1, 1, 0x300, 0x100) & 0x9FF) | 0xE1000000;
    AddPrim(ot + 10, p);
    return (DR_TPAGE *)p + 1;
}

void *func_800B2F60(u32 *ot, SPRT *p, TmState *s) {
    u8 c;

    if (s->unk4F0 != 0) {
        c = 0x80;
        if (s->unk4FC >= 0x19) {
            c = s->unk4F0 * 4 - 0x80;
        }
        SetSprt(p);
        p->w = 0x38;
        p->h = 0x10;
        p->u0 = 0xA0;
        p->v0 = 0xF0;
        p->x0 = s->unk4F4 + 0x10;
        p->y0 = s->unk4F8;
        p->r0 = c;
        p->g0 = c;
        p->b0 = c;
        p->clut = 0x3A40;
        AddPrim(ot + 10, p);
        p++;
        SetSprt(p);
        p->w = 0x38;
        p->h = 0x10;
        p->u0 = 0xA0;
        p->v0 = 0xF0;
        p->x0 = s->unk4F4 + 0x12;
        p->y0 = s->unk4F8 + 2;
        p->r0 = 0;
        p->g0 = 0;
        p->b0 = 0;
        p->clut = 0x3A40;
        AddPrim(ot + 10, p);
        p++;
        SetSprt(p);
        p->w = 0x10;
        p->h = 0x10;
        p->u0 = s->unk4FC * 16;
        p->v0 = 0xF0;
        p->x0 = s->unk4F4;
        p->y0 = s->unk4F8;
        p->r0 = c;
        p->g0 = c;
        p->b0 = c;
        p->clut = 0x3A40;
        AddPrim(ot + 10, p);
        p++;
        SetSprt(p);
        p->w = 0x38;
        p->h = 0x10;
        p->u0 = s->unk4FC * 16;
        p->v0 = 0xF0;
        p->x0 = s->unk4F4 + 2;
        p->y0 = s->unk4F8 + 2;
        p->r0 = 0;
        p->g0 = 0;
        p->b0 = 0;
        p->clut = 0x3A40;
        AddPrim(ot + 10, p);
        p++;
        setlen((DR_TPAGE *)p, 1);
        ((DR_TPAGE *)p)->code[0] = (GetTPage(1, 0, 0x280, 0x100) & 0x9FF) | 0xE1000000;
        AddPrim(ot + 10, p);
        p = (SPRT *)((DR_TPAGE *)p + 1);
        s->unk4F0--;
    }
    return p;
}

INCLUDE_ASM("asm/us/ovl_0d/nonmatchings/ovl_0d", func_800B3198);

void *func_800B33CC(u32 *ot, SPRT *p, s32 x, s32 y, s32 n) {
    s32 u;

    SetSprt(p);
    p->w = 10;
    p->h = 16;
    u = n * 10 + 0x18;
    p->v0 = 0xF0;
    p->x0 = x;
    p->y0 = y;
    p->r0 = 0x80;
    p->g0 = 0x80;
    p->b0 = 0x80;
    p->u0 = u;
    p->clut = 0x3B40;
    AddPrim(ot + 11, p);
    return p + 1;
}

void *func_800B3480(u32 *ot, void *p, TmState *s) {
    return p;
}

void func_800B3488(TmState *arg0) {
    RECT r;

    if ((u32)(arg0->unk450 - 0x7C) >= 4) {
        r.w = func_8002BA68((s32)func_80035774(2)) + 0x12;
        r.x = 0xA0 - r.w / 2;
        r.y = 0x54;
        r.h = 0x39;
        arg0->unk458 = func_8002DC54(&r, 5, 2);
        func_8002E2EC(arg0->unk458, func_800B355C);
        func_8002E31C(arg0->unk458, 1);
        func_800AB89C(arg0->unk458);
        arg0->unk4 = 0;
        arg0->unk0 = 0xB;
        return;
    }
    func_800B35F8(arg0);
}

void func_800B355C(Window *arg0) {
    func_8002B5FC(arg0->unk0 + 9, arg0->unk2 + 6, arg0->unk18, func_80035774(2), 0);
}

void func_800B35A4(TmState *arg0) {
    func_8002F93C((arg0->unk2E8 * 0x88) & 0xFF8);
    if (arg0->unk2E8 >= 0x1F) {
        arg0->unk0 = 0xC;
    }
}

void func_800B35F8(TmState *arg0) {
    arg0->unk2E8 = 0;
    func_800320B8(2, 1, 0x1E, 0xFFFFFF);
    func_8002F93C(0);
    func_800A78EC(0xC401, 0x42, 0x1E, 0x7F, 0);
    arg0->unk0 = 0x1B;
}

INCLUDE_RODATA("asm/us/ovl_0d/nonmatchings/ovl_0d", D_800A74A8);

INCLUDE_RODATA("asm/us/ovl_0d/nonmatchings/ovl_0d", D_800A74AC);
