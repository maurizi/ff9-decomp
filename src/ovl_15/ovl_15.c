#include "ovl_15.h"

INCLUDE_RODATA("asm/us/ovl_15/nonmatchings/ovl_15", D_800A7000);

void func_800A713C(void) {
    func_800ACB98();
    func_800ACE1C();
    D_800AE4B8 = 0;
    while (D_800AE4B8 < 5) {
        switch (D_800AE4B8) {
        case 0:
            func_800A7378(4, 0x12);
            D_800AE4B8 = 3;
            break;
        case 1:
            func_800AD7DC();
            D_800AE4B8 = 2;
            break;
        case 2:
            func_800A7378(4, 0x13);
            D_800AE4B8 = 3;
            break;
        case 3:
            if (func_800AB490() == 1) {
                D_800AE4B8 = 4;
            } else {
                D_800AE4B8 = 3;
            }
            break;
        case 4:
            func_800A9488();
            D_800AE4B8 = 3;
            break;
        }
    }
    func_800ACF00();
    func_800ACC08();
}

void func_800A723C(void) {
    GameData *ctx = D_8006794C->unk1C;
    ClearOTagR(ctx->unk10[ctx->unk8], 0x1000);
}

void func_800A7280(void) {
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

s32 func_800A7378(s32 arg0, s32 arg1) {
    u8 *buf;
    s32 timer;
    s32 i;
    s32 r;

    buf = D_80067944 + 0x106700;
    timer = -1;
    func_800A766C(arg0, arg1);
    do {
        func_800A7838();
        func_80019194();
        switch (D_800AE4C0) {
        case 0:
            if (D_8006794C->unkB != 0xFF && (D_8006794C->unkB & 8)) {
                D_800AE0E0 = 1;
                D_800AE4C0 = 1;
            } else {
                D_800AE4C0 = 3;
                D_800AE0E0 |= 0x80;
            }
            break;
        case 1:
            i = 1;
            if (D_8006794C->unk1C->unk8 != 0) {
                i = 2;
            }
            D_800AE4C8[0][i - 1] = 7;
            D_800AE4C8[0][i] = 0x10;
            D_8006794C->unk9 = 4;
            DrawSync(0);
            func_801999B4(1, &buf);
            func_8019A5B0(1);
            func_8019AC3C(func_800A7A84);
            func_80199B28(arg0, arg1);
            D_800AE4C0 = 2;
            break;
        case 2:
            if ((D_800AE0E0 & 0x10) && (func_80012AD4() & 2)) {
                r = func_80199F30(0);
                func_8019AD0C();
                if (r != 0) {
                    DrawOTag(D_8006794C->unk1C->unk10[D_8006794C->unk1C->unk8 ^ 1] + 0xFFF);
                } else if (timer == -1 && D_8006794C->unk1C->unk8 == 0) {
                    D_8006794C->unkC[D_8006794C->unk1C->unk8].disp.x = 0x1E0;
                }
            }
            break;
        case 3:
            if (D_800AE0E0 & 0x80) {
                D_8006794C->unk9 = 0;
                timer = 900;
                D_8006794C->unkC[0].disp.x = 0x140;
                D_800AE4C0 = 4;
                D_8006794C->unkC[1].disp.x = 0;
                D_80073998[0] = 0;
            }
            break;
        }
        if (!(D_800AE0E0 & 2)) {
            func_80032098();
        }
        func_800A787C();
    } while (D_800AE4C0 < 4);
    return 0;
}

s32 func_800A766C(s32 arg0, s32 arg1) {
    FileEntry *e;
    s32 h;
    s32 i;
    s32 j;

    while (func_800217C4(D_8006794C->unk1C->unk854) != 0) {
    }
    e = func_8001E218(0, 0x64);
    func_8002177C(D_8006794C->unk1C->unk854, e->unk4, (e->unkC - e->unk4) << 11, D_80067944 + 0xF1700, 0, 0x15000, 0);
    while (func_800217C4(D_8006794C->unk1C->unk854) != 0) {
    }
    func_80012D08();
    func_80012CF8();
    func_80012D18();
    if (D_8006794C->unkB != 0xFF && (D_8006794C->unkB & 8)) {
        h = func_80022AC4(D_8019DC5C[arg0].unk4[arg1].unk0);
        func_80022ED4(h);
        while (func_80022B18() != 0) {
        }
        func_8019C98C(h);
    }
    func_800320B8(1, 1, 0x78, 0xFFFFFF);
    D_800AE4C0 = 0;
    D_800AE0E0 = 0;
    for (i = 1; i >= 0; i--) {
        for (j = 3; j >= 0; j--) {
            D_800AE4C8[i][j] = 0;
        }
    }
    D_800AE0E4 = 0;
    D_80073998[0] = 0;
    return 0;
}

void func_800A7838(void) {
    GameData *ctx = D_8006794C->unk1C;
    ClearOTagR(ctx->unk10[ctx->unk8], 0x1000);
}

void func_800A787C(void) {
    GameData *ctx;
    u8 k;
    s32 i;
    s32 j;

    DrawSync(0);
    if (!(func_80012AD4() & 2)) {
        D_8006794C->unk9 = 0;
    }
    VSync(D_8006794C->unk9);
    ResetGraph(1);
    PutDispEnv(&D_8006794C->unkC[D_8006794C->unk1C->unk8]);
    if (D_800AE0E0 & 4) {
        PutDrawEnv(&D_8006794C->unk10[D_8006794C->unk1C->unk8]);
    } else {
        ctx = D_8006794C->unk1C;
        k = ctx->unk8;
        DrawOTagEnv(ctx->unk10[k] + 0xFFF, &D_8006794C->unk10[k]);
    }
    D_8006794C->unk1C->unk8 ^= 1;
    func_8002C0DC(D_8006794C->unk1C->unk8);
    D_800AE0E0 |= D_800AE4C8[0][0];
    D_800AE0E0 &= ~D_800AE4C8[1][0];
    for (i = 0; i < 2; i++) {
        for (j = 0; j < 3; j++) {
            D_800AE4C8[i][j] = D_800AE4C8[i][j + 1];
            D_800AE4C8[i][3] = 0;
        }
    }
    D_800AE0E4++;
}

void func_800A7A84(void) {
    func_8019A9D8();
    D_800AE0E0 |= 0x80;
    D_800AE4C0 = 3;
}

INCLUDE_ASM("asm/us/ovl_15/nonmatchings/ovl_15", func_800A7ABC);

s32 func_800A8528(s32 x, s32 y) {
    SPRT *p = &D_800AE5F8[D_8006794C->unk1C->unk8][D_800AE4D8];

    setlen(p, 4);
    p->code = 0x64;
    setRGB0(p, 0x80, 0x80, 0x80);
    p->x0 = x;
    p->u0 = 0;
    p->v0 = 0x80;
    p->w = 0x38;
    p->h = 0x48;
    p->y0 = y;
    p->clut = 0x3940;
    addPrim(&D_8006794C->unk1C->unk10[D_8006794C->unk1C->unk8][10], p);
    D_800AE4D8++;
    return 0;
}

INCLUDE_ASM("asm/us/ovl_15/nonmatchings/ovl_15", func_800A8630);

s32 func_800A8F00(s32 x, s32 y) {
    s32 i;

    for (i = 0; i < 3; i++) {
        func_800A8528(x - i * 2, y - i);
    }
    return 0;
}

s32 func_800A8F58(Ovl15Shoe *shoe) {
    s32 i;
    s32 j;

    for (i = 0; i < 6; i++) {
        for (j = 0; j < 52; j++) {
            shoe->unk4[i * 52 + j] = j;
        }
    }
    shoe->unk0 = 1;
    shoe->unk2 = 0;
    return 0;
}

s32 func_800A8FA4(Ovl15Shoe *shoe) {
    s32 pass;
    s32 n;
    s32 i;
    s32 j;
    u8 t;

    for (pass = 0; pass < 5; pass++) {
        for (i = 0, n = 312; n > 0; n--, i++) {
            if (D_8007B720 >= D_8007B92D) {
                func_8004BF20();
            }
            j = (*D_8007B720++ * 2) % n;
            t = shoe->unk4[i];
            shoe->unk4[i] = shoe->unk4[i + j];
            shoe->unk4[i + j] = t;
        }
    }
    shoe->unk0 = (shoe->unk0 & ~1) | 2;
    return 0;
}

s32 func_800A90B8(Ovl15Hand *hand, Ovl15Shoe *shoe) {
    s32 card = shoe->unk4[shoe->unk2++];
    s32 rank;

    hand->unkC[hand->unk2++] = card;
    rank = card / 4;
    if (rank < 9) {
        hand->unk4 += rank + 2;
        hand->unk6 += rank + 2;
    } else if (rank == 12) {
        hand->unk4 += 11;
        hand->unk6 += 1;
        hand->unk0 |= 1;
    } else {
        hand->unk4 += 10;
        hand->unk6 += 10;
    }
    if (hand->unk4 >= 22) {
        if (hand->unk0 & 1) {
            hand->unk4 -= 10;
            if (hand->unk6 >= hand->unk4) {
                hand->unk0 &= ~1;
            }
        } else {
            hand->unk0 |= 2;
        }
    }
    if (shoe->unk2 >= 249) {
        shoe->unk0 |= 4;
    }
    return 0;
}

s32 func_800A91F0(Ovl15Hand *hand, s16 x, s16 y) {
    s32 i;

    hand->unk0 = 0;
    hand->unk2 = 0;
    hand->unk4 = 0;
    hand->unk6 = 0;
    hand->unk8 = x;
    hand->unkA = y;
    for (i = 21; i >= 0; i--) {
        hand->unkC[i] = 0;
    }
    return 0;
}

s32 func_800A9228(Ovl15Hand *a, Ovl15Hand *b) {
    s32 r;
    u16 v;

    a->unk2 = 1;
    a->unk8 = 0x1E;
    a->unkA = 0x8C;
    func_800A91F0(b, 0x1E, 0x60);
    b->unk2 = 1;
    b->unkC[0] = a->unkC[1];
    r = (u32)a->unkC[1] >> 2;
    if (r < 9) {
        v = r + 2;
    } else if (r == 12) {
        a->unk4 = 11;
        a->unk6 = 1;
        a->unk0 |= 9;
        b->unk4 = 11;
        b->unk6 = 1;
        b->unk0 |= 9;
        goto end;
    } else {
        v = 10;
    }
    a->unk4 = v;
    a->unk6 = v;
    b->unk4 = v;
    b->unk6 = v;
end:
    return 0;
}

s32 func_800A92FC(Ovl15Hand *hand) {
    s32 ret = 0;

    if (hand->unk2 == 2) {
        ret = 2;
        if ((u32)hand->unkC[0] >> 2 == (u32)hand->unkC[1] >> 2) {
            ret = 3;
        }
    }
    return ret;
}

s32 func_800A9334(Ovl15Hand *hand, s32 dx, s32 dy, s32 count) {
    s32 first;
    s32 n;
    s32 i;
    s32 x;
    s32 y;
    s32 card;

    if (count == 0) {
        return 1;
    }
    n = count - 1;
    if (count < 0) {
        n = hand->unk2 - 1;
        if (hand->unk2 == 0) {
            return 1;
        }
    }
    first = 1;
    x = hand->unk8 + dx + n * 13;
    y = hand->unkA + dy;
    for (i = n; i >= 0; i--) {
        card = hand->unkC[i];
        if (first && (hand->unk0 & 0x10) && n == 1) {
            func_800A8528(x, y);
            first = 0;
        } else {
            func_800A8630(card / 4, card % 4, x, y);
        }
        x -= 13;
    }
    return 0;
}

s32 func_800A945C(Ovl15Hand *h) {
    if (h->unk4 == 0x15 && h->unk2 == 2) {
        return 1;
    }
    return 0;
}

INCLUDE_ASM("asm/us/ovl_15/nonmatchings/ovl_15", func_800A9488);

s32 func_800AAD24(void) {
    s32 p;

    p = (s32)func_8001C7FC(4, 0x44F4C, 0);
    func_8001D9CC(p, 0, 0xE4, 0x1C0, 0x180);
    func_8001D8A8(p);
    func_8001D9CC(p, 0, 0xE5, 0x1C0, 0x180);
    func_8001D8A8(p);
    p = (s32)func_8001C7FC(4, 0x44F4D, 0);
    func_8001D9CC(p, 0, 0xE6, 0x1EC, 0x100);
    func_8001D8A8(p);
    p = (s32)func_8001C7FC(4, 0x44F4E, 0);
    func_8001D9CC(p, 0, 0xE0, 0x1C0, 0x100);
    func_8001D8A8(p);
    p = (s32)func_8001C7FC(4, 0x44F50, 0);
    func_8001D9CC(p, 0, 0xE1, 0x1D6, 0x100);
    func_8001D8A8(p);
    p = (s32)func_8001C7FC(4, 0x44F4F, 0);
    func_8001D9CC(p, 0, 0xE2, 0x1C0, 0x140);
    func_8001D8A8(p);
    p = (s32)func_8001C7FC(4, 0x44F51, 0);
    func_8001D9CC(p, 0, 0xE9, 0x280, 0x100);
    func_8001D8A8(p);
    setRECT(&D_800AE4E0, 0x20, 0xE4, 0x10, 1);
    LoadImage(&D_800AE4E0, D_800AE0EC);
    DrawSync(0);
    setlen(&D_800AE4E8, 1);
    D_800AE4E8.code = 0xE1000697;
    if (!(D_8006794C->unk1C->unk0 & 0x20000000)) {
        func_800AC598(0, 0x7009C, 0, 0, 0);
        func_800AC598(0xC401, 0x7009C, 0x5A, 0, 0x7F);
    }
    func_800320B8(1, 1, 0, 0xFFFFFF);
    D_800B0DF8 = 0;
    D_80073998[0] = 0;
    return 0;
}

s32 func_800AAFA4(void) {
    return 0;
}

void func_800AAFAC(void) {
    GameData *ctx = D_8006794C->unk1C;
    ClearOTagR(ctx->unk10[ctx->unk8], 0x1000);
    D_800AE4D8 = 0;
    D_800AE4DC = 0;
}

void func_800AAFFC(void) {
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

void func_800AB100(void) {
    D_800B0F50.unk3 = 0;
}

void func_800AB10C(void) {
    func_80032574();
    func_8002C4B0(1);
    D_800B0F50.unk3 = 0;
    D_800B0F50.unk0 = 1;
    D_800B0F50.unk7 = 0;
    D_800B0F50.unk6 = 0;
    D_800B0F50.unk1 = 1;
    D_800B0F50.unk2 = 0;
    D_800B0F50.unk8 = 0;
    D_800B0F50.unkC = 0;
}

void func_800AB160(void) {
    func_8002C308(0);
    func_800AB1C8();
    func_800AB34C();
    func_800AB37C();
}

void func_800AB198(void) {
    func_8002C4B0(0);
}

u8 func_800AB1B8(void) {
    return D_800B0F50.unk3;
}

void func_800AB1C8(void) {
    if (D_800B0F50.unk1 != 0 && D_800B0F50.unk2 == 0) {
        if (D_80077528 & 0x10) {
            func_8002F488(0x67);
            D_800B0F50.unk3 = D_800B0F50.unk3 != 0 ? D_800B0F50.unk3 - 1 : 3;
            if (D_800B0F4C == 0 && D_800B0F50.unk3 == 3) {
                D_800B0F50.unk3--;
            }
            if (D_800B0F44 == 0 && D_800B0F50.unk3 == 2) {
                D_800B0F50.unk3--;
            }
        } else if (D_80077528 & 0x40) {
            func_8002F488(0x67);
            D_800B0F50.unk3 = D_800B0F50.unk3 < 3 ? D_800B0F50.unk3 + 1 : 0;
            if (D_800B0F44 == 0 && D_800B0F50.unk3 == 2) {
                D_800B0F50.unk3++;
            }
            if (D_800B0F4C == 0 && D_800B0F50.unk3 == 3) {
                D_800B0F50.unk3 = 0;
            }
        } else if (D_80077528 & 0x20000) {
            func_800AB39C();
        }
    }
}

void func_800AB34C(void) {
    func_800AB3C4();
    func_800AB3CC();
    func_800AB3D4();
}

void func_800AB37C(void) {
    func_800AB45C();
}

void func_800AB39C(void) {
    func_8002F488(0x67);
    func_800ACF58(4);
}

void func_800AB3C4(void) {
}

void func_800AB3CC(void) {
}

void func_800AB3D4(void) {
    if (D_800B0F50.unk1 != 0 && D_800B0F50.unk2 == 0) {
        D_80077DE8.unk0 = D_800AE3DC[D_800B0F50.unk3].x - 0x1A;
        D_80077DE8.unk2 = D_800AE3DC[D_800B0F50.unk3].y + 3;
        D_80077DE8.unk4 = 3;
        func_800355E4(&D_80077DE8, &D_800B0F50);
    }
}

void func_800AB45C(void) {
    D_800B0F50.unk8++;
    if (D_800B0F50.unk8 > 48) {
        D_800B0F50.unk8 = 0;
        D_800B0F50.unk2 = 0;
    }
}

INCLUDE_ASM("asm/us/ovl_15/nonmatchings/ovl_15", func_800AB490);

s32 func_800AC374(void) {
    s32 p = (s32)func_8001C7FC(4, D_800AE432 | 0x40000, 0);

    func_8001D9CC(p, 0, 0xE0, 0x280, 0);
    func_8001D8A8(p);
    func_800552B8(0);
    func_800AC598(0, 0x7009C, 0, 0, 0);
    func_800AC598(0xC401, 0x7009C, 0x5A, 0, 0x7F);
    func_800320B8(1, 1, 0, 0xFFFFFF);
    D_800B0FD0 = 0;
    D_800AE430 = 0;
    D_80073998[0] = 0;
    return 0;
}

s32 func_800AC448(void) {
    return 0;
}

void func_800AC450(void) {
    GameData *ctx = D_8006794C->unk1C;
    ClearOTagR(ctx->unk10[ctx->unk8], 0x1000);
}

void func_800AC494(void) {
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

INCLUDE_ASM("asm/us/ovl_15/nonmatchings/ovl_15", func_800AC598);

void func_800ACB98(void) {
    s32 i;

    for (i = 1; i >= 0; i--) {
        D_800739B0[i].unk1 = 0;
        D_800739B0[i].unk0 = 0;
    }
    D_8006794C->unk1C->unk83C = func_800AC598;
    func_800ACD70();
    func_800ACCB4();
    func_800239D0();
    SetDispMask(1);
}

void func_800ACC08(void) {
    s32 i;

    SetDispMask(0);
    if (D_8006794C->unk1C->unk0 & 0x20000000) {
        func_800AC598(0x8301, 0x7009C, 0x5A, 0, 0);
        for (i = 0x59; i >= 0; i--) {
            VSync(0);
        }
    }
    func_800AC598(0x100, 0x7009C, 0, 0, 0);
    func_8001C784(D_800AE440);
    D_800AE440 = NULL;
}

void func_800ACCB4(void) {
    s32 a;
    s32 b;

    func_800552B8(0);
    a = (s32)func_8001C7FC(4, 0x7009C, 0);
    memmove(D_8006794C->unk1C->unk814, (void *)a, (s32)func_8001C7FC(5, 0x7009C, 0));
    a = (s32)func_8001C7FC(4, 0x901F3, 0);
    b = (s32)func_8001C7FC(5, 0x901F3, 0);
    func_80055DF4();
    func_80055E14(a, b, 1);
}

void func_800ACD70(void) {
    u8 *buf;
    FileEntry *e;

    buf = D_80067944 + 0x10000;
    e = func_8001E218(1, 0x2F44);
    func_8002177C(D_8006794C->unk1C->unk854, e->unk4, (e->unkC - e->unk4) << 11, buf, 0, -1, 0);
    while (func_800217C4(D_8006794C->unk1C->unk854) != 0) {
    }
    D_800AE440 = buf;
    func_8001C750((s32)buf, 0);
}

void func_800ACE1C(void) {
    s32 i;
    Ovl15Mode *m;

    i = 0;
    m = D_800AE444;
    D_800B0FD8.unk0 = 0;
    D_800B0FD8.unk2 = 0;
    D_800B0FD8.unk3 = 0;
    for (; i < 5; i++, m++) {
        if (m->unk0 != NULL) {
            m->unk0();
        }
    }
    if (D_800AE444[D_800B0FD8.unk0].unk4 != NULL) {
        D_800AE444[D_800B0FD8.unk0].unk4();
    }
}

void func_800ACEB8(void) {
    void (*f)(void) = D_800AE444[D_800B0FD8.unk0].unk8;
    if (f != NULL) {
        f();
    }
}

void func_800ACF00(void) {
    void (*f)(void) = D_800AE444[D_800B0FD8.unk0].unkC;
    if (f != NULL) {
        f();
    }
}

u8 func_800ACF48(void) {
    return D_800B0FD8.unk0;
}

void func_800ACF58(s32 arg0) {
    if (arg0 != D_800B0FD8.unk0) {
        if (D_800AE444[D_800B0FD8.unk0].unkC != NULL) {
            D_800AE444[D_800B0FD8.unk0].unkC();
        }
        D_800B0FD8.unk0 = arg0;
        if (D_800AE444[D_800B0FD8.unk0].unk4 != NULL) {
            D_800AE444[D_800B0FD8.unk0].unk4();
        }
    }
}

s32 func_800ACFE4(void) {
    u32 i = 0;
    s32 pad = D_8006794C->unkB;

    do {
        func_8001DCB8();
    } while (D_8006794C->unkB == 0xFF && ++i < 10);
    func_80033D1C(1);
    if (func_800AD0C4(pad, D_8006794C->unkB)) {
        return func_800AD08C(pad);
    }
    return 0xFF;
}

s32 func_800AD08C(s32 arg0) {
    s32 i;
    s32 mask;

    for (i = 0; i < 4; i++) {
        mask = 1 << i;
        if (arg0 & mask) {
            return i;
        }
    }
    return 0;
}

u8 func_800AD0C4(s32 arg0, s32 arg1) {
    s32 a = arg0 & 0xF;
    s32 b = arg1 & 0xF;
    return (b & a) != a;
}

s32 func_800AD0DC(void) {

    if (func_8002F810() == 0) {
        return (u32)(D_800B0FD8.unk0 - 1) < 3;
    }
    return 0;
}

void func_800AD11C(s8 arg0) {
    D_800B0FD8.unk3 = arg0;
}

u8 func_800AD128(void) {
    return D_800B0FD8.unk3;
}

void func_800AD138(void) {
    D_800B0FE0.unk3 = 0;
}

void func_800AD144(void) {
    func_80032574();
    func_8002C4B0(1);
    D_800B0FE0.unk0 = 1;
    D_800B0FE0.unk7 = 0;
    D_800B0FE0.unk6 = 0;
    D_800B0FE0.unk1 = 1;
    D_800B0FE0.unk2 = 0;
    D_800B0FE0.unk8 = 0;
    D_800B0FE0.unkC = 0;
}

void func_800AD194(void) {
    func_8002C308(0);
    func_800AD1FC();
    func_800AD308();
    func_800AD338();
}

void func_800AD1CC(void) {
    func_8002C4B0(0);
}

u8 func_800AD1EC(void) {
    return D_800B0FE0.unk3;
}

void func_800AD1FC(void) {
    if (D_800B0FE0.unk1 != 0 && D_800B0FE0.unk2 == 0) {
        if (D_80077528 & 0x20) {
            D_800B0F48 += 10;
            if (D_800AE0E8 < D_800B0F48) {
                D_800B0F48 = D_800AE0E8 / 10 * 10;
            }
            if (D_800B0F48 > 1000) {
                D_800B0F48 = 1000;
            }
            func_8002F488(0x67);
        } else if (D_80077528 & 0x80) {
            D_800B0F48 -= 10;
            if (D_800B0F48 < 10) {
                D_800B0F48 = 10;
            }
            func_8002F488(0x67);
        } else if (D_80077528 & 0x20000) {
            func_800AD358();
        }
    }
}

void func_800AD308(void) {
    func_800AD380();
    func_800AD388();
    func_800AD390();
}

void func_800AD338(void) {
    func_800AD418();
}

void func_800AD358(void) {
    func_8002F488(0x67);
    func_800ACF58(4);
}

void func_800AD380(void) {
}

void func_800AD388(void) {
}

void func_800AD390(void) {
    if (D_800B0FE0.unk1 != 0 && D_800B0FE0.unk2 == 0) {
        D_80077DE8.unk0 = D_800AE494[D_800B0FE0.unk3].x - 0x1A;
        D_80077DE8.unk2 = D_800AE494[D_800B0FE0.unk3].y + 3;
        D_80077DE8.unk4 = 3;
        func_800355E4(&D_80077DE8, &D_800B0FE0);
    }
}

void func_800AD418(void) {
    D_800B0FE0.unk8++;
    if (D_800B0FE0.unk8 > 48) {
        D_800B0FE0.unk8 = 0;
        D_800B0FE0.unk2 = 0;
    }
}

void func_800AD44C(void) {
    D_800B0FF0.unk3 = 0;
}

void func_800AD458(void) {
    func_80032574();
    func_8002C4B0(1);
    D_800B0FF0.unk0 = 1;
    D_800B0FF0.unk7 = 0;
    D_800B0FF0.unk6 = 0;
    D_800B0FF0.unk1 = 0;
    D_800B0FF0.unk2 = 0;
    D_800B0FF0.unk8 = 0;
    D_800B0FF0.unkC = 0;
    func_800AD77C();
}

void func_800AD4AC(void) {
    func_8002C308(0);
    func_800AD514();
    func_800AD5EC();
    func_800AD624();
}

void func_800AD4E4(void) {
    func_8002C4B0(0);
}

u8 func_800AD504(void) {
    return D_800B0FF0.unk3;
}

void func_800AD514(void) {
    if (func_8002F810() == 0 && D_800B0FF0.unk1 != 0 && D_800B0FF0.unk2 == 0) {
        if (D_80077528 & 0x10) {
            func_8002F488(0x67);
            D_800B0FF0.unk3 = D_800B0FF0.unk3 != 0 ? D_800B0FF0.unk3 - 1 : 3;
        } else if (D_80077528 & 0x40) {
            func_8002F488(0x67);
            D_800B0FF0.unk3 = D_800B0FF0.unk3 < 3 ? D_800B0FF0.unk3 + 1 : 0;
        } else if (D_80077528 & 0x20000) {
            func_800AD644();
        }
    }
}

void func_800AD5EC(void) {
    func_800AD678();
    func_800AD680();
    func_800AD688();
    func_800AD690();
}

void func_800AD624(void) {
    func_800AD728();
}

void func_800AD644(void) {
    func_8002F488(0x67);
    func_8002FAAC(0x10);
    func_8002F8A8(func_800AD75C);
}

void func_800AD678(void) {
}

void func_800AD680(void) {
}

void func_800AD688(void) {
}

void func_800AD690(void) {
    if (func_8002F810() == 0 && D_800B0FF0.unk1 != 0 && D_800B0FF0.unk2 == 0) {
        D_80077DE8.unk0 = D_800AE498[D_800B0FF0.unk3].x - 0x1A;
        D_80077DE8.unk2 = D_800AE498[D_800B0FF0.unk3].y + 3;
        D_80077DE8.unk4 = 3;
        func_800355E4(&D_80077DE8, &D_800B0FF0);
    }
}

void func_800AD728(void) {
    D_800B0FF0.unk8++;
    if (D_800B0FF0.unk8 > 48) {
        D_800B0FF0.unk8 = 0;
        D_800B0FF0.unk2 = 0;
    }
}

void func_800AD75C(void) {
    func_800ACF58(4);
}

void func_800AD77C(void) {
    if (func_800AD128() == 0) {
        D_800B0FF0.unk1 = 1;
        D_800B0FF0.unk2 = 1;
        D_800B0FF0.unk8 = 0;
        return;
    }
    D_800B0FF0.unk1 = 1;
    D_800B0FF0.unk2 = 0;
    D_800B0FF0.unk8 = 0;
    func_8002FA50(0x20);
}

INCLUDE_ASM("asm/us/ovl_15/nonmatchings/ovl_15", func_800AD7DC);

s32 func_800ADC3C(void) {
    D_8006794C->unk9 = 0;
    func_800320B8(1, 1, 0, 0xFFFFFF);
    D_800B5000 = 0;
    D_800AE4A8 = 0;
    D_800AE4AC = 0;
    D_80073998[0] = 0;
    return 0;
}

void func_800ADC98(void) {
    GameData *ctx = D_8006794C->unk1C;
    ClearOTagR(ctx->unk10[ctx->unk8], 0x1000);
    D_800AE4B0 = D_800B1000[D_8006794C->unk1C->unk8];
}

void func_800ADD04(void) {
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
    D_800AE4A8 = 0;
    D_800AE4AC++;
}

u8 *func_800ADE14(u8 *arg0, s32 arg1) {
    return arg0 + (arg0[arg1 * 4 + 4] + 4);
}

INCLUDE_ASM("asm/us/ovl_15/nonmatchings/ovl_15", func_800ADE30);

INCLUDE_ASM("asm/us/ovl_15/nonmatchings/ovl_15", func_800ADF90);

void *func_800AE028(s32 x, s32 y, u32 *ot, u32 a, s32 *out, SprtWords *p) {
    u32 n;

    setlen(p, 4);
    p->rgbc = 0x64808080;
    p->xy = ((x + ((a >> 20) & 3)) & 0xFFFF) | (y << 16);
    p->uvclut = (a & 0x40FFFF) + 0x38270000;
    p->wh = ((a >> 16) & 0xF) | 0xD0000;
    addPrim(ot, p);
    p++;
    n = a >> 28;
    if (out != NULL) {
        *out = n + 1;
    }
    return p;
}

INCLUDE_RODATA("asm/us/ovl_15/nonmatchings/ovl_15", D_800A70EC);
