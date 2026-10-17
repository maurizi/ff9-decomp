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
    GameData *ctx = g_gameState->unk1C;
    ClearOTagR(ctx->unk10[ctx->unk8], 0x1000);
}

void func_800A7280(void) {
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
            if (g_gameState->unkB != 0xFF && (g_gameState->unkB & 8)) {
                D_800AE0E0 = 1;
                D_800AE4C0 = 1;
            } else {
                D_800AE4C0 = 3;
                D_800AE0E0 |= 0x80;
            }
            break;
        case 1:
            i = 1;
            if (g_gameState->unk1C->unk8 != 0) {
                i = 2;
            }
            D_800AE4C8[0][i - 1] = 7;
            D_800AE4C8[0][i] = 0x10;
            g_gameState->unk9 = 4;
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
                    DrawOTag(g_gameState->unk1C->unk10[g_gameState->unk1C->unk8 ^ 1] + 0xFFF);
                } else if (timer == -1 && g_gameState->unk1C->unk8 == 0) {
                    g_gameState->unkC[g_gameState->unk1C->unk8].disp.x = 0x1E0;
                }
            }
            break;
        case 3:
            if (D_800AE0E0 & 0x80) {
                g_gameState->unk9 = 0;
                timer = 900;
                g_gameState->unkC[0].disp.x = 0x140;
                D_800AE4C0 = 4;
                g_gameState->unkC[1].disp.x = 0;
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

    while (func_800217C4(g_gameState->unk1C->unk854) != 0) {
    }
    e = func_8001E218(0, 0x64);
    func_8002177C(g_gameState->unk1C->unk854, e->unk4, (e->unkC - e->unk4) << 11, D_80067944 + 0xF1700, 0, 0x15000, 0);
    while (func_800217C4(g_gameState->unk1C->unk854) != 0) {
    }
    EnterCriticalSection();
    FlushCache();
    ExitCriticalSection();
    if (g_gameState->unkB != 0xFF && (g_gameState->unkB & 8)) {
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
    GameData *ctx = g_gameState->unk1C;
    ClearOTagR(ctx->unk10[ctx->unk8], 0x1000);
}

void func_800A787C(void) {
    GameData *ctx;
    u8 k;
    s32 i;
    s32 j;

    DrawSync(0);
    if (!(func_80012AD4() & 2)) {
        g_gameState->unk9 = 0;
    }
    VSync(g_gameState->unk9);
    ResetGraph(1);
    PutDispEnv(&g_gameState->unkC[g_gameState->unk1C->unk8]);
    if (D_800AE0E0 & 4) {
        PutDrawEnv(&g_gameState->unk10[g_gameState->unk1C->unk8]);
    } else {
        ctx = g_gameState->unk1C;
        k = ctx->unk8;
        DrawOTagEnv(ctx->unk10[k] + 0xFFF, &g_gameState->unk10[k]);
    }
    g_gameState->unk1C->unk8 ^= 1;
    func_8002C0DC(g_gameState->unk1C->unk8);
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

s32 func_800A7ABC(s32 kind, s32 c, s32 x, s32 y) {
    SPRT *p;
    DR_TPAGE *t;
    s32 x1;
    s32 x2;

    switch (kind) {
    case 0:
        p = &D_800AE5F8[g_gameState->unk1C->unk8][D_800AE4D8];
        setlen(p, 4);
        p->code = 0x66;
        p->x0 = x;
        p->y0 = y;
        p->u0 = 0;
        p->v0 = 0;
        setRGB0(p, c, c, c);
        p->w = 0x68;
        p->h = 0x28;
        p->clut = 0x3A40;
        addPrim(&g_gameState->unk1C->unk10[g_gameState->unk1C->unk8][10], p);
        D_800AE4D8++;
        t = &D_800AE4F8[g_gameState->unk1C->unk8][D_800AE4DC];
        setlen(t, 1);
        t->code[0] = (GetTPage(1, 1, 0x280, 0x100) & 0x9FF) | 0xE1000400;
        addPrim(&g_gameState->unk1C->unk10[g_gameState->unk1C->unk8][10], t);
        D_800AE4DC++;
        p = &D_800AE5F8[g_gameState->unk1C->unk8][D_800AE4D8];
        setlen(p, 4);
        p->code = 0x66;
        p->x0 = x;
        p->y0 = y;
        setRGB0(p, c, c, c);
        p->u0 = 0x68;
        p->v0 = 0;
        p->w = 0x70;
        p->h = 0x28;
        p->clut = 0x3A40;
        addPrim(&g_gameState->unk1C->unk10[g_gameState->unk1C->unk8][10], p);
        D_800AE4D8++;
        t = &D_800AE4F8[g_gameState->unk1C->unk8][D_800AE4DC];
        setlen(t, 1);
        t->code[0] = (GetTPage(1, 2, 0x280, 0x100) & 0x9FF) | 0xE1000400;
        addPrim(&g_gameState->unk1C->unk10[g_gameState->unk1C->unk8][10], t);
        D_800AE4DC++;
        break;
    case 1:
        x1 = x - 2;
        p = &D_800AE5F8[g_gameState->unk1C->unk8][D_800AE4D8];
        setlen(p, 4);
        p->code = 0x66;
        p->x0 = x1;
        p->y0 = y;
        setRGB0(p, c, c, c);
        p->u0 = 0;
        p->w = 0x68;
        p->h = 0x28;
        p->v0 = 0x28;
        p->clut = 0x3A40;
        addPrim(&g_gameState->unk1C->unk10[g_gameState->unk1C->unk8][10], p);
        D_800AE4D8++;
        t = &D_800AE4F8[g_gameState->unk1C->unk8][D_800AE4DC];
        setlen(t, 1);
        t->code[0] = (GetTPage(1, 1, 0x280, 0x100) & 0x9FF) | 0xE1000400;
        addPrim(&g_gameState->unk1C->unk10[g_gameState->unk1C->unk8][10], t);
        D_800AE4DC++;
        p = &D_800AE5F8[g_gameState->unk1C->unk8][D_800AE4D8];
        setlen(p, 4);
        p->code = 0x66;
        p->x0 = x1;
        p->y0 = y;
        p->w = 0x70;
        setRGB0(p, c, c, c);
        p->u0 = 0x68;
        p->v0 = 0x28;
        p->h = 0x28;
        p->clut = 0x3A40;
        addPrim(&g_gameState->unk1C->unk10[g_gameState->unk1C->unk8][10], p);
        D_800AE4D8++;
        t = &D_800AE4F8[g_gameState->unk1C->unk8][D_800AE4DC];
        setlen(t, 1);
        t->code[0] = (GetTPage(1, 2, 0x280, 0x100) & 0x9FF) | 0xE1000400;
        addPrim(&g_gameState->unk1C->unk10[g_gameState->unk1C->unk8][10], t);
        D_800AE4DC++;
        break;
    case 2:
        x2 = x - 0x10;
        p = &D_800AE5F8[g_gameState->unk1C->unk8][D_800AE4D8];
        setlen(p, 4);
        p->code = 0x66;
        p->x0 = x2;
        p->y0 = y;
        p->w = 0x88;
        setRGB0(p, c, c, c);
        p->u0 = 0;
        p->v0 = 0xA0;
        p->h = 0x28;
        p->clut = 0x3A40;
        addPrim(&g_gameState->unk1C->unk10[g_gameState->unk1C->unk8][10], p);
        D_800AE4D8++;
        t = &D_800AE4F8[g_gameState->unk1C->unk8][D_800AE4DC];
        setlen(t, 1);
        t->code[0] = (GetTPage(1, 1, 0x280, 0x100) & 0x9FF) | 0xE1000400;
        addPrim(&g_gameState->unk1C->unk10[g_gameState->unk1C->unk8][10], t);
        D_800AE4DC++;
        p = &D_800AE5F8[g_gameState->unk1C->unk8][D_800AE4D8];
        setlen(p, 4);
        p->code = 0x66;
        p->x0 = x2;
        p->y0 = y;
        setRGB0(p, c, c, c);
        p->v0 = 0xC8;
        p->u0 = 0;
        p->w = 0x90;
        p->h = 0x28;
        p->clut = 0x3A40;
        addPrim(&g_gameState->unk1C->unk10[g_gameState->unk1C->unk8][10], p);
        D_800AE4D8++;
        t = &D_800AE4F8[g_gameState->unk1C->unk8][D_800AE4DC];
        setlen(t, 1);
        t->code[0] = (GetTPage(1, 2, 0x280, 0x100) & 0x9FF) | 0xE1000400;
        addPrim(&g_gameState->unk1C->unk10[g_gameState->unk1C->unk8][10], t);
        D_800AE4DC++;
        break;
    }
    return 0;
}

s32 func_800A8528(s32 x, s32 y) {
    SPRT *p = &D_800AE5F8[g_gameState->unk1C->unk8][D_800AE4D8];

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
    addPrim(&g_gameState->unk1C->unk10[g_gameState->unk1C->unk8][10], p);
    D_800AE4D8++;
    return 0;
}

s32 func_800A8630(s32 rank, s32 suit, s32 x, s32 y) {
    SPRT *p;
    s32 i;
    s32 f;
    s32 px;
    s32 py;

    p = &D_800AE5F8[g_gameState->unk1C->unk8][D_800AE4D8];
    setlen(p, 4);
    p->code = 0x64;
    setRGB0(p, 0x80, 0x80, 0x80);
    p->x0 = x + 2;
    p->y0 = y + 1;
    p->u0 = (suit & 1) ? 0x58 : 0x6C;
    p->v0 = rank * 14;
    p->w = 9;
    p->h = 0xE;
    p->clut = 0x3980;
    addPrim(&g_gameState->unk1C->unk10[g_gameState->unk1C->unk8][10], p);
    D_800AE4D8++;

    p = &D_800AE5F8[g_gameState->unk1C->unk8][D_800AE4D8];
    setlen(p, 4);
    p->code = 0x64;
    setRGB0(p, 0x80, 0x80, 0x80);
    p->x0 = x + 0x2C;
    p->y0 = y + 0x38;
    p->u0 = (suit & 1) ? 0x62 : 0x76;
    p->v0 = rank * 14;
    p->w = 9;
    p->h = 0xE;
    p->clut = 0x3980;
    addPrim(&g_gameState->unk1C->unk10[g_gameState->unk1C->unk8][10], p);
    D_800AE4D8++;

    p = &D_800AE5F8[g_gameState->unk1C->unk8][D_800AE4D8];
    p->x0 = x + 1;
    p->y0 = y + 0xF;
    setlen(p, 4);
    p->code = 0x64;
    setRGB0(p, 0x80, 0x80, 0x80);
    p->u0 = suit * 12;
    p->v0 = 0xD5;
    p->w = 0xB;
    p->h = 0xB;
    p->clut = 0x3900;
    addPrim(&g_gameState->unk1C->unk10[g_gameState->unk1C->unk8][10], p);
    D_800AE4D8++;

    p = &D_800AE5F8[g_gameState->unk1C->unk8][D_800AE4D8];
    p->x0 = x + 0x2B;
    setlen(p, 4);
    p->code = 0x64;
    setRGB0(p, 0x80, 0x80, 0x80);
    p->u0 = suit * 12;
    p->v0 = 0xCA;
    p->y0 = y + 0x2D;
    p->w = 0xB;
    p->h = 0xB;
    p->clut = 0x3900;
    addPrim(&g_gameState->unk1C->unk10[g_gameState->unk1C->unk8][10], p);
    D_800AE4D8++;

    if (rank >= 9) {
        f = rank - 9;
        if (rank == 12) {
            p = &D_800AE5F8[g_gameState->unk1C->unk8][D_800AE4D8];
            setlen(p, 4);
            p->code = 0x64;
            setRGB0(p, 0x80, 0x80, 0x80);
            p->u0 = suit * 12;
            p->v0 = 0xD5;
            p->x0 = x + 0x16;
            p->y0 = y + 0x1E;
            p->w = 0xB;
            p->h = 0xB;
            p->clut = 0x3900;
            addPrim(&g_gameState->unk1C->unk10[g_gameState->unk1C->unk8][10], p);
            D_800AE4D8++;
        } else {
            p = &D_800AE5F8[g_gameState->unk1C->unk8][D_800AE4D8];
            p->x0 = x + 7;
            p->y0 = y + 0xA;
            setlen(p, 4);
            p->code = 0x64;
            setRGB0(p, 0x80, 0x80, 0x80);
            if (f & 1) {
                p->u0 = 0x2C;
            } else {
                p->u0 = 0;
            }
            if (f >> 1) {
                p->v0 = 0x40;
            } else {
                p->v0 = 0;
            }
            p->w = 0x2A;
            p->h = 0x33;
            p->clut = (f + 0xE0) << 6;
            addPrim(&g_gameState->unk1C->unk10[g_gameState->unk1C->unk8][10], p);
            D_800AE4D8++;
        }
    } else {
        for (i = 0; i <= rank + 1; i++) {
            px = D_800AE10C[rank][i].x;
            py = D_800AE10C[rank][i].y;
            p = &D_800AE5F8[g_gameState->unk1C->unk8][D_800AE4D8];
            setlen(p, 4);
            p->code = 0x64;
            setRGB0(p, 0x80, 0x80, 0x80);
            p->u0 = suit * 12;
            p->x0 = x + px;
            p->y0 = y + py;
            if (py >= 0x1F) {
                p->v0 = 0xCA;
            } else {
                p->v0 = 0xD5;
            }
            p->w = 0xB;
            p->h = 0xB;
            p->clut = 0x3900;
            addPrim(&g_gameState->unk1C->unk10[g_gameState->unk1C->unk8][10], p);
            D_800AE4D8++;
        }
    }

    p = &D_800AE5F8[g_gameState->unk1C->unk8][D_800AE4D8];
    setlen(p, 4);
    p->code = 0x64;
    setRGB0(p, 0x80, 0x80, 0x80);
    p->u0 = 0;
    p->v0 = 0x80;
    p->w = 0x38;
    p->h = 0x48;
    p->x0 = x;
    p->y0 = y;
    p->clut = 0x3900;
    addPrim(&g_gameState->unk1C->unk10[g_gameState->unk1C->unk8][10], p);
    D_800AE4D8++;
    return 0;
}

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
            if (g_randPtr >= g_randTableEnd) {
                rand_refill();
            }
            j = (*g_randPtr++ * 2) % n;
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

/* func_800A9488's format strings and Blackjack UI labels */
INCLUDE_RODATA("asm/us/ovl_15/nonmatchings/ovl_15", D_800A7018);

INCLUDE_RODATA("asm/us/ovl_15/nonmatchings/ovl_15", D_800A701C);

INCLUDE_RODATA("asm/us/ovl_15/nonmatchings/ovl_15", D_800A7028);

INCLUDE_RODATA("asm/us/ovl_15/nonmatchings/ovl_15", D_800A702C);

INCLUDE_RODATA("asm/us/ovl_15/nonmatchings/ovl_15", D_800A7034);

INCLUDE_RODATA("asm/us/ovl_15/nonmatchings/ovl_15", D_800A703C);

INCLUDE_RODATA("asm/us/ovl_15/nonmatchings/ovl_15", D_800A7040);

INCLUDE_RODATA("asm/us/ovl_15/nonmatchings/ovl_15", D_800A7048);

s32 func_800A9488(void) {
    char buf[16];
    s32 ofsX[3];
    s32 ofsY[3];
    Ovl15Hand dealer;
    Ovl15Hand player;
    Ovl15Hand split;
    s32 t;
    Ovl15Hand *cur;
    s32 nsplit;
    s32 cnt;
    s32 i;
    s32 res;
    s32 x;
    s32 y;
    s32 dx;
    s32 dy;
    Ovl15Hand *p1;
    s32 r;
    s32 a;
    s32 c;

    t = -1;
    cnt = 0;
    func_800AAD24();
    D_800B0F48 = 10;
    func_800A91F0(&player, 0x1E, 0x78);
    func_800A91F0(&dealer, 0x1E, 0xA);
    res = 0;
    i = 0;
    cur = &player;
    nsplit = 0;
    x = 0xF0;
    y = 0xA;
    dx = 0x10000;
    dy = 0x10000;
    p1 = cur;
    ofsX[0] = ofsX[1] = ofsX[2] = 0;
    ofsY[0] = ofsY[1] = ofsY[2] = 0;
    do {
        func_800AAFAC();
        func_80019194();
        switch (D_800B0DF8) {
        case 0:
            t = -1;
            D_800B0DF8 = 1;
            break;
        case 1:
            if (t == -1) {
                t = 0x4E;
                func_800A8F58(&D_800B0E08);
                func_800A8FA4(&D_800B0E08);
                func_800320B8(1, 1, 0x3C, 0);
                i = 0;
            } else if (t != 0) {
                c = D_800B0E08.unk4[i];
                func_800A8630(c >> 2, c & 3, 0xF0, 0xA);
                i += 4;
                t--;
            } else {
                t = -1;
                D_800B0DF8 = 2;
                func_800ACF58(2);
            }
            break;
        case 2:
            func_800A91F0(&player, 0x1E, 0x78);
            func_800A91F0(&dealer, 0x1E, 0xA);
            func_800ACEB8();
            if (func_800ACF48() == 4) {
                t = -1;
                D_800B0DF8 = 3;
            }
            break;
        case 3:
            if (t == -1) {
                dealer.unk0 |= 0x10;
                func_800A90B8(p1, &D_800B0E08);
                func_800A90B8(&dealer, &D_800B0E08);
                func_800A90B8(p1, &D_800B0E08);
                func_800A90B8(&dealer, &D_800B0E08);
                cur = p1;
                i = 0;
                x = 0xF00000;
                y = 0xA0000;
                t = 0x78;
                dx = (((player.unk8 - 0xF0) << 8) / 30) << 8;
                dy = (((player.unkA - 0xA) << 8) / 30) << 8;
            } else if (t > 0) {
                func_800A8528(x >> 16, y >> 16);
                func_800A9334(&dealer, 0, 0, i >= 2);
                if (i > 0) {
                    a = 2;
                    if (i < 3) {
                        a = 1;
                    }
                } else {
                    a = 0;
                }
                func_800A9334(p1, 0, 0, a);
                x += dx;
                y += dy;
                if ((t - 1) % 30 == 0 && ++i < 4) {
                    x = 0xF00000;
                    y = 0xA0000;
                    switch (i) {
                    case 1:
                        dx = (((dealer.unk8 - 0xF0) << 8) / 30) << 8;
                        dy = (((dealer.unkA - 0xA) << 8) / 30) << 8;
                        break;
                    case 2:
                        dx = (((player.unk8 - 0xE3) << 8) / 30) << 8;
                        dy = (((player.unkA - 0xA) << 8) / 30) << 8;
                        break;
                    case 3:
                        dx = (((dealer.unk8 - 0xE3) << 8) / 30) << 8;
                        dy = (((dealer.unkA - 0xA) << 8) / 30) << 8;
                        break;
                    }
                }
                t--;
            } else if (func_800A945C(p1)) {
                if (func_800A945C(&dealer)) {
                    t = -1;
                    D_800B0DF8 = 10;
                } else {
                    t = -1;
                    D_800B0DF8 = 8;
                }
            } else if (func_800A945C(&dealer)) {
                t = -1;
                if (func_800A945C(p1)) {
                    D_800B0DF8 = 10;
                } else {
                    D_800B0DF8 = 8;
                }
            } else {
                t = -1;
                D_800B0DF8 = 4;
                func_800ACF58(3);
            }
            break;
        case 4:
            r = func_800A92FC(cur);
            D_800B0F44 = 0;
            D_800B0F4C = 0;
            if ((r & 2) && D_800AE0E8 >= D_800B0F48 * 2) {
                D_800B0F44 = 1;
            }
            if ((r & 1) && nsplit == 0 && D_800AE0E8 >= D_800B0F48 * 2) {
                D_800B0F4C = 1;
            }
            if (t == -1) {
                func_800ACEB8();
                if (func_800ACF48() == 4) {
                    if (func_800AB1B8() == 0) {
                        if (nsplit == 1) {
                            cur = &split;
                            D_800B0DF8 = 4;
                            func_800ACF58(3);
                            x = 0xF00000;
                            y = 0xA0000;
                            t = 30;
                            dx = (((cur->unk8 + cur->unk2 * 13 - 0xF0) << 8) / 30) << 8;
                            dy = (((cur->unkA - 0xA) << 8) / 30) << 8;
                            nsplit = 2;
                        } else {
                            D_800B0DF8 = 5;
                            dealer.unk0 &= ~0x10;
                        }
                    } else if (func_800AB1B8() == 1) {
                        x = 0xF00000;
                        y = 0xA0000;
                        t = 30;
                        dx = (((cur->unk8 + cur->unk2 * 13 - 0xF0) << 8) / 30) << 8;
                        dy = (((cur->unkA - 0xA) << 8) / 30) << 8;
                    } else if (func_800AB1B8() == 2) {
                        x = 0xF00000;
                        y = 0xA0000;
                        t = 30;
                        cur->unk0 |= 4;
                        dx = (((cur->unk8 + cur->unk2 * 13 - 0xF0) << 8) / 30) << 8;
                        dy = (((cur->unkA - 0xA) << 8) / 30) << 8;
                    } else if (func_800AB1B8() == 3) {
                        if (func_800A92FC(cur) & 1) {
                            func_800A9228(&player, &split);
                            x = 0xF00000;
                            y = 0xA0000;
                            t = 30;
                            dx = (((cur->unk8 + cur->unk2 * 13 - 0xF0) << 8) / 30) << 8;
                            dy = (((cur->unkA - 0xA) << 8) / 30) << 8;
                            nsplit = 1;
                        }
                    }
                }
            } else if (t > 0) {
                func_800A8528(x >> 16, y >> 16);
                x += dx;
                y += dy;
                t--;
            } else {
                func_800A90B8(cur, &D_800B0E08);
                t = -1;
                if (cur->unk4 >= 22) {
                    D_800B0DF8 = 7;
                } else if (cur->unk4 == 21) {
                    if (nsplit == 1) {
                        cur = &split;
                        D_800B0DF8 = 4;
                        func_800ACF58(3);
                        x = 0xF00000;
                        y = 0xA0000;
                        t = 30;
                        dx = (((cur->unk8 + cur->unk2 * 13 - 0xF0) << 8) / 30) << 8;
                        dy = (((cur->unkA - 0xA) << 8) / 30) << 8;
                        nsplit = 2;
                    } else {
                        D_800B0DF8 = 5;
                    }
                } else if (cur->unk0 & 8) {
                    if (nsplit == 1) {
                        cur = &split;
                        D_800B0DF8 = 4;
                        func_800ACF58(3);
                        x = 0xF00000;
                        y = 0xA0000;
                        t = 30;
                        dx = (((cur->unk8 + cur->unk2 * 13 - 0xF0) << 8) / 30) << 8;
                        dy = (((cur->unkA - 0xA) << 8) / 30) << 8;
                        nsplit = 2;
                    } else {
                        D_800B0DF8 = 5;
                    }
                } else if (cur->unk0 & 4) {
                    if (nsplit == 1) {
                        cur = &split;
                        D_800B0DF8 = 4;
                        func_800ACF58(3);
                        x = 0xF00000;
                        y = 0xA0000;
                        t = 30;
                        dx = (((cur->unk8 + cur->unk2 * 13 - 0xF0) << 8) / 30) << 8;
                        dy = (((cur->unkA - 0xA) << 8) / 30) << 8;
                        nsplit = 2;
                    } else {
                        D_800B0DF8 = 5;
                    }
                } else {
                    D_800B0DF8 = 4;
                    func_800ACF58(3);
                }
            }
            break;
        case 5:
            if (t == -1) {
                if (dealer.unk4 < 17) {
                    x = 0xF00000;
                    y = 0xA0000;
                    t = 30;
                    dy = 0;
                    dx = (((dealer.unk2 * 13 - 0xD2) << 8) / 30) << 8;
                } else if (dealer.unk0 & 2) {
                    D_800B0DF8 = 7;
                } else if (nsplit > 0) {
                    nsplit = 1;
                    D_800B0DF8 = 6;
                } else if (player.unk4 == dealer.unk4) {
                    D_800B0DF8 = 10;
                } else if (dealer.unk4 < player.unk4) {
                    D_800B0DF8 = 9;
                } else {
                    D_800B0DF8 = 11;
                }
            } else if (t > 0) {
                func_800A8528(x >> 16, y >> 16);
                x += dx;
                y += dy;
                t--;
            } else {
                t = -1;
                D_800B0DF8 = 5;
                func_800A90B8(&dealer, &D_800B0E08);
            }
            break;
        case 7:
            if (cur->unk4 >= 22) {
                i = 1;
                if (nsplit == 2) {
                    i = 2;
                }
            } else {
                i = 0;
            }
            if (t == -1) {
                t = 10;
                ofsX[i] = RND3();
                ofsY[i] = RND3();
            } else if (t > 0) {
                if (ofsX[i] < 0) {
                    ofsX[i] = RND3();
                } else {
                    ofsX[i] = -RND3();
                }
                if (ofsY[i] < 0) {
                    ofsY[i] = RND3();
                } else {
                    ofsY[i] = -RND3();
                }
                t--;
            } else {
                t = -1;
                ofsX[0] = ofsX[1] = ofsX[2] = 0;
                ofsY[0] = ofsY[1] = ofsY[2] = 0;
                if (nsplit == 1) {
                    cur = &split;
                    D_800B0DF8 = 4;
                    func_800ACF58(3);
                    x = 0xF00000;
                    y = 0xA0000;
                    t = 30;
                    dx = (((cur->unk8 + cur->unk2 * 13 - 0xF0) << 8) / 30) << 8;
                    dy = (((cur->unkA - 0xA) << 8) / 30) << 8;
                    nsplit = 2;
                } else if (nsplit == 2) {
                    nsplit = 1;
                    D_800B0DF8 = 6;
                } else if (cur->unk4 >= 22) {
                    D_800B0DF8 = 11;
                } else {
                    D_800B0DF8 = 9;
                }
            }
            break;
        case 6:
            if (t == -1) {
                if (nsplit == 1) {
                    if (player.unk4 >= 22) {
                        res = 1;
                        snd_playSe(0x6B);
                        D_800AE0E8 -= (player.unk0 & 4) ? D_800B0F48 * 2 : D_800B0F48;
                    } else if (dealer.unk4 >= 22 || dealer.unk4 < player.unk4) {
                        res = 0;
                        snd_playSe(0x6C);
                        D_800AE0E8 += (player.unk0 & 4) ? D_800B0F48 * 2 : D_800B0F48;
                    } else {
                        res = 2;
                        if (player.unk4 != dealer.unk4) {
                            res = 1;
                            snd_playSe(0x6B);
                            D_800AE0E8 -= (player.unk0 & 4) ? D_800B0F48 * 2 : D_800B0F48;
                        }
                    }
                } else {
                    if (split.unk4 >= 22) {
                        res = 1;
                        snd_playSe(0x6B);
                        D_800AE0E8 -= (split.unk0 & 4) ? D_800B0F48 * 2 : D_800B0F48;
                    } else if (dealer.unk4 >= 22 || dealer.unk4 < split.unk4) {
                        res = 0;
                        snd_playSe(0x6C);
                        D_800AE0E8 += (split.unk0 & 4) ? D_800B0F48 * 2 : D_800B0F48;
                    } else {
                        res = 2;
                        if (split.unk4 != dealer.unk4) {
                            res = 1;
                            snd_playSe(0x6B);
                            D_800AE0E8 -= (split.unk0 & 4) ? D_800B0F48 * 2 : D_800B0F48;
                        }
                    }
                }
                t = 0x94;
                dealer.unk0 &= ~0x10;
            } else if (t > 0) {
                s32 a;

                if (t < 0x10) {
                    a = t * 16;
                } else if (t < 0x85) {
                    a = 0xFF;
                } else {
                    a = (0x94 - t) * 16;
                }
                func_800A7ABC(res, a, 0x6C, nsplit == 1 ? player.unkA + 12 : split.unkA + 12);
                t--;
            } else {
                t = -1;
                if (nsplit == 1) {
                    nsplit = 2;
                } else {
                    D_800B0DF8 = 12;
                }
            }
            break;
        case 8:
            if (t == -1) {
                if (player.unk4 > dealer.unk4) {
                    snd_playSe(0x6C);
                    D_800AE0E8 += D_800B0F48 + (D_800B0F48 >> 1);
                } else {
                    snd_playSe(0x6B);
                    D_800AE0E8 -= D_800B0F48;
                }
                t = 0x94;
                dealer.unk0 &= ~0x10;
            } else if (t > 0) {
                s32 a;

                if (t < 0x10) {
                    a = t * 16;
                } else if (t < 0x85) {
                    a = 0xFF;
                } else {
                    a = (0x94 - t) * 16;
                }
                if (player.unk4 > dealer.unk4) {
                    func_800A7ABC(0, a, 0x6C, player.unkA + 12);
                } else {
                    func_800A7ABC(1, a, 0x6C, player.unkA + 12);
                }
                t--;
            } else {
                t = -1;
                D_800B0DF8 = 12;
            }
            break;
        case 9:
            if (t == -1) {
                snd_playSe(0x6C);
                {
                    s32 *pb = &D_800AE0E8;
                    *pb = D_800AE0E8 + ((player.unk0 & 4) ? D_800B0F48 * 2 : D_800B0F48);
                }
                t = 0x94;
                dealer.unk0 &= ~0x10;
            } else if (t > 0) {
                s32 a;

                if (t < 0x10) {
                    a = t * 16;
                } else if (t < 0x85) {
                    a = 0xFF;
                } else {
                    a = (0x94 - t) * 16;
                }
                func_800A7ABC(0, a, 0x6C, player.unkA + 12);
                t--;
            } else {
                t = -1;
                D_800B0DF8 = 12;
            }
            break;
        case 10:
            if (t == -1) {
                t = 0x94;
                dealer.unk0 &= ~0x10;
            } else if (t > 0) {
                s32 a;

                if (t < 0x10) {
                    a = t * 16;
                } else if (t < 0x85) {
                    a = 0xFF;
                } else {
                    a = (0x94 - t) * 16;
                }
                func_800A7ABC(2, a, 0x6C, player.unkA + 12);
                t--;
            } else {
                t = -1;
                D_800B0DF8 = 12;
            }
            break;
        case 11:
            if (t == -1) {
                snd_playSe(0x6B);
                D_800AE0E8 -= (player.unk0 & 4) ? D_800B0F48 * 2 : D_800B0F48;
                t = 0x94;
                dealer.unk0 &= ~0x10;
            } else if (t > 0) {
                s32 a;

                if (t < 0x10) {
                    a = t * 16;
                } else if (t < 0x85) {
                    a = 0xFF;
                } else {
                    a = (0x94 - t) * 16;
                }
                func_800A7ABC(1, a, 0x6C, player.unkA + 12);
                t--;
            } else {
                t = -1;
                D_800B0DF8 = 12;
            }
            break;
        case 12:
            if (D_800B0E08.unk0 & 4) {
                t = -1;
                D_800B0DF8 = 13;
            } else {
                nsplit = 0;
                if (D_800B0F48 > D_800AE0E8) {
                    D_800B0F48 = D_800AE0E8 / 10 * 10;
                }
                t = -1;
                D_800B0DF8 = 2;
                func_800ACF58(2);
            }
            break;
        case 13:
            if (t == -1) {
                func_800320B8(1, 1, 0x3C, 0xFFFFFF);
                t = 0x3C;
                if (D_800AE0E8 < 10) {
                    D_800AE0E8 = 1000;
                }
            } else if (t > 0) {
                t--;
            } else {
                t = -1;
                D_800B0DF8 = 14;
            }
            break;
        }
        if (D_800B0DF8 >= 4) {
            func_800A9334(&dealer, ofsX[0], ofsY[0], -1);
            if (dealer.unk2 != 0 && D_800B0DF8 >= 5) {
                sprintf(buf, D_800A7018, dealer.unk4);
                func_8002B56C(10, dealer.unkA + 0x20, 0x10, buf, 0);
            }
            if (nsplit == 2) {
                func_800A9334(&split, ofsX[2], ofsY[2], -1);
                if (split.unk2 != 0) {
                    sprintf(buf, D_800A7018, split.unk4);
                    func_8002B56C(10, split.unkA + 0x20, 0x10, buf, 0);
                    if (split.unk4 != 21 && (split.unk0 & 1)) {
                        sprintf(buf, D_800A7018, split.unk6);
                        func_8002B56C(10, split.unkA + 0x2A, 0x10, buf, 5);
                    }
                }
            }
            func_800A9334(&player, ofsX[1], ofsY[1], -1);
            if (player.unk2 != 0) {
                sprintf(buf, D_800A7018, player.unk4);
                func_8002B56C(10, player.unkA + 0x20, 0x10, buf, 0);
                if (player.unk4 != 21 && (player.unk0 & 1)) {
                    sprintf(buf, D_800A7018, player.unk6);
                    func_8002B56C(10, player.unkA + 0x2A, 0x10, buf, 5);
                }
            }
            if (nsplit == 1) {
                func_800A9334(&split, ofsX[2], ofsY[2], -1);
                if (split.unk2 != 0) {
                    sprintf(buf, D_800A7018, split.unk4);
                    func_8002B56C(10, split.unkA + 0x20, 0x10, buf, 0);
                    if (split.unk4 != 21 && (split.unk0 & 1)) {
                        sprintf(buf, D_800A7018, split.unk6);
                        func_8002B56C(10, split.unkA + 0x2A, 0x10, buf, 5);
                    }
                }
            }
        }
        func_800A8F00(0xF0, 0xA);
        addPrim(g_gameState->unk1C->unk10[g_gameState->unk1C->unk8] + 10, &D_800AE4E8);
        sprintf(buf, D_800A701C);
        func_8002B56C(0x10C - (func_8002B9B8(buf) >> 1), 0x58, 0x10, buf, 4);
        sprintf(buf, D_800A7028, D_800AE0E8);
        func_8002B56C(0x10C - (func_8002B9B8(buf) >> 1), 0x64, 0x10, buf, 5);
        sprintf(buf, D_800A702C);
        func_8002B56C(0x10C - (func_8002B9B8(buf) >> 1), 0x70, 0x10, buf, 4);
        sprintf(buf, D_800A7028, D_800B0F48);
        func_8002B56C(0x10C - (func_8002B9B8(buf) >> 1), 0x7C, 0x10, buf, 0);
        sprintf(buf, D_800A7034);
        func_8002B56C(0x10C - (func_8002B9B8(buf) >> 1), 0x90, 0x10, buf, 0);
        sprintf(buf, D_800A703C);
        func_8002B56C(0x10C - (func_8002B9B8(buf) >> 1), 0xA0, 0x10, buf, 0);
        sprintf(buf, D_800A7040);
        func_8002B56C(0x10C - (func_8002B9B8(buf) >> 1), 0xB0, 0x10, buf, D_800B0F44 ? 0 : 2);
        sprintf(buf, D_800A7048);
        func_8002B56C(0x10C - (func_8002B9B8(buf) >> 1), 0xC0, 0x10, buf, D_800B0F4C ? 0 : 2);
        D_800739B0[0].unk0 = 0;
        if (cnt > 0) {
            cnt--;
        }
        if (D_800B0DF8 < 14) {
            if (D_800AE0E8 < 10) {
                t = -1;
                D_800B0DF8 = 13;
            }
            if (g_padPortPressed[0] & 8) {
                t = -1;
                D_800B0DF8 = 13;
            }
        }
        func_80032098();
        func_800AAFFC();
    } while (D_800B0DF8 < 14);
    func_800AAFA4();
    return 0;
}

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
    if (!(g_gameState->unk1C->unk0 & 0x20000000)) {
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
    GameData *ctx = g_gameState->unk1C;
    ClearOTagR(ctx->unk10[ctx->unk8], 0x1000);
    D_800AE4D8 = 0;
    D_800AE4DC = 0;
}

void func_800AAFFC(void) {
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
        if (g_padRepeat & PAD_UP) {
            snd_playSe(0x67);
            D_800B0F50.unk3 = D_800B0F50.unk3 != 0 ? D_800B0F50.unk3 - 1 : 3;
            if (D_800B0F4C == 0 && D_800B0F50.unk3 == 3) {
                D_800B0F50.unk3--;
            }
            if (D_800B0F44 == 0 && D_800B0F50.unk3 == 2) {
                D_800B0F50.unk3--;
            }
        } else if (g_padRepeat & PAD_DOWN) {
            snd_playSe(0x67);
            D_800B0F50.unk3 = D_800B0F50.unk3 < 3 ? D_800B0F50.unk3 + 1 : 0;
            if (D_800B0F44 == 0 && D_800B0F50.unk3 == 2) {
                D_800B0F50.unk3++;
            }
            if (D_800B0F4C == 0 && D_800B0F50.unk3 == 3) {
                D_800B0F50.unk3 = 0;
            }
        } else if (g_padRepeat & BTN_CROSS) {
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
    snd_playSe(0x67);
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

s32 func_800AB490(void) {
    s32 timer;
    s32 wait;
    s32 n;

    timer = -1;
    wait = 0;
    func_800AC374();
    D_800AE3EC = 0;
    do {
        func_800AC450();
        func_80019194();
        switch (D_800B0FD0) {
        case 0:
            timer = -1;
            D_800B0FD0 = 1;
            break;
        case 1:
            if (timer == -1) {
                func_800320B8(1, 1, 0, 0xFFFFFF);
                timer = 0x21C;
                func_800320B8(1, 1, 0xB4, 0);
            } else if (timer != 0) {
                setlen(&D_800B0F60[g_gameState->unk1C->unk8][0], 4);
                D_800B0F60[g_gameState->unk1C->unk8][0].code = 0x64;
                D_800B0F60[g_gameState->unk1C->unk8][0].r0 = 0x80;
                D_800B0F60[g_gameState->unk1C->unk8][0].g0 = 0x80;
                D_800B0F60[g_gameState->unk1C->unk8][0].b0 = 0x80;
                D_800B0F60[g_gameState->unk1C->unk8][0].x0 = 0;
                D_800B0F60[g_gameState->unk1C->unk8][0].y0 = 0;
                D_800B0F60[g_gameState->unk1C->unk8][0].u0 = 0;
                D_800B0F60[g_gameState->unk1C->unk8][0].v0 = 0;
                D_800B0F60[g_gameState->unk1C->unk8][0].w = 0xC0;
                D_800B0F60[g_gameState->unk1C->unk8][0].h = 0xE0;
                D_800B0F60[g_gameState->unk1C->unk8][0].clut = 0x3800;
                addPrim(&g_gameState->unk1C->unk10[g_gameState->unk1C->unk8][8], &D_800B0F60[g_gameState->unk1C->unk8][0]);
                setDrawTPage(&D_800B0FB0[g_gameState->unk1C->unk8][0], 1, 1, 0x10A);
                addPrim(&g_gameState->unk1C->unk10[g_gameState->unk1C->unk8][8], &D_800B0FB0[g_gameState->unk1C->unk8][0]);
                setlen((D_800B0F60[g_gameState->unk1C->unk8] + 1), 4);
                (D_800B0F60[g_gameState->unk1C->unk8] + 1)->code = 0x64;
                (D_800B0F60[g_gameState->unk1C->unk8] + 1)->r0 = 0x80;
                (D_800B0F60[g_gameState->unk1C->unk8] + 1)->g0 = 0x80;
                (D_800B0F60[g_gameState->unk1C->unk8] + 1)->b0 = 0x80;
                (D_800B0F60[g_gameState->unk1C->unk8] + 1)->x0 = 0xC0;
                (D_800B0F60[g_gameState->unk1C->unk8] + 1)->y0 = 0;
                (D_800B0F60[g_gameState->unk1C->unk8] + 1)->u0 = 0;
                (D_800B0F60[g_gameState->unk1C->unk8] + 1)->v0 = 0;
                (D_800B0F60[g_gameState->unk1C->unk8] + 1)->w = 0x80;
                (D_800B0F60[g_gameState->unk1C->unk8] + 1)->h = 0xE0;
                (D_800B0F60[g_gameState->unk1C->unk8] + 1)->clut = 0x3800;
                addPrim(&g_gameState->unk1C->unk10[g_gameState->unk1C->unk8][8], (D_800B0F60[g_gameState->unk1C->unk8] + 1));
                setDrawTPage(D_800B0FB0[g_gameState->unk1C->unk8] + 1, 1, 1, 0x10D);
                addPrim(&g_gameState->unk1C->unk10[g_gameState->unk1C->unk8][8], (D_800B0FB0[g_gameState->unk1C->unk8] + 1));
            }
            break;
        case 2:
            setlen(&D_800B0F60[g_gameState->unk1C->unk8][0], 4);
            D_800B0F60[g_gameState->unk1C->unk8][0].code = 0x64;
            D_800B0F60[g_gameState->unk1C->unk8][0].r0 = 0x80;
            D_800B0F60[g_gameState->unk1C->unk8][0].g0 = 0x80;
            D_800B0F60[g_gameState->unk1C->unk8][0].b0 = 0x80;
            D_800B0F60[g_gameState->unk1C->unk8][0].x0 = 0;
            D_800B0F60[g_gameState->unk1C->unk8][0].y0 = 0;
            D_800B0F60[g_gameState->unk1C->unk8][0].u0 = 0;
            D_800B0F60[g_gameState->unk1C->unk8][0].v0 = 0;
            D_800B0F60[g_gameState->unk1C->unk8][0].w = 0xC0;
            D_800B0F60[g_gameState->unk1C->unk8][0].h = 0xE0;
            D_800B0F60[g_gameState->unk1C->unk8][0].clut = 0x3800;
            addPrim(&g_gameState->unk1C->unk10[g_gameState->unk1C->unk8][8], &D_800B0F60[g_gameState->unk1C->unk8][0]);
            setDrawTPage(&D_800B0FB0[g_gameState->unk1C->unk8][0], 1, 1, 0x10A);
            addPrim(&g_gameState->unk1C->unk10[g_gameState->unk1C->unk8][8], &D_800B0FB0[g_gameState->unk1C->unk8][0]);
            setlen((D_800B0F60[g_gameState->unk1C->unk8] + 1), 4);
            (D_800B0F60[g_gameState->unk1C->unk8] + 1)->code = 0x64;
            (D_800B0F60[g_gameState->unk1C->unk8] + 1)->r0 = 0x80;
            (D_800B0F60[g_gameState->unk1C->unk8] + 1)->g0 = 0x80;
            (D_800B0F60[g_gameState->unk1C->unk8] + 1)->b0 = 0x80;
            (D_800B0F60[g_gameState->unk1C->unk8] + 1)->x0 = 0xC0;
            (D_800B0F60[g_gameState->unk1C->unk8] + 1)->y0 = 0;
            (D_800B0F60[g_gameState->unk1C->unk8] + 1)->u0 = 0;
            (D_800B0F60[g_gameState->unk1C->unk8] + 1)->v0 = 0;
            (D_800B0F60[g_gameState->unk1C->unk8] + 1)->w = 0x80;
            (D_800B0F60[g_gameState->unk1C->unk8] + 1)->h = 0xE0;
            (D_800B0F60[g_gameState->unk1C->unk8] + 1)->clut = 0x3800;
            addPrim(&g_gameState->unk1C->unk10[g_gameState->unk1C->unk8][8], (D_800B0F60[g_gameState->unk1C->unk8] + 1));
            setDrawTPage(D_800B0FB0[g_gameState->unk1C->unk8] + 1, 1, 1, 0x10D);
            addPrim(&g_gameState->unk1C->unk10[g_gameState->unk1C->unk8][8], (D_800B0FB0[g_gameState->unk1C->unk8] + 1));
            if (timer == -1) {
                timer = 0x5A;
                func_800320B8(1, 1, timer, 0xFFFFFF);
            } else if (--timer == 0) {
                D_800B0FD0 = 4;
            }
            break;
        }
        if (D_800AE3EC == 0x10) {
            if (g_padPortHeld[0] & 8) {
                timer = -1;
                D_800B0FD0 = 2;
                snd_playSe(0xC18);
                D_80073998[0] = -1;
            }
        } else if (g_padPortPressed[0] & D_800AE3F0[D_800AE3EC]) {
            if (++D_800AE3EC == 0x10) {
                snd_playSe(0x67);
            }
        } else if (g_padPortPressed[0] != 0) {
            wait = 5;
            D_800AE3EC = 0;
        }
        D_800739B0[0].unk0 = 0;
        if (wait > 0) {
            wait--;
        }
        func_80032098();
        func_800AC494();
    } while ((u32)D_800B0FD0 < 3);
    func_800AC448();
    return D_800B0FD0 != 3;
}

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
    GameData *ctx = g_gameState->unk1C;
    ClearOTagR(ctx->unk10[ctx->unk8], 0x1000);
}

void func_800AC494(void) {
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

s32 func_800AC598(s32 arg0, s32 arg1, s32 arg2, s32 arg3, s32 arg4) {
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
            if (D_800AE43C == -1) {
                D_800AE43C = kind;
            }
            if (g_gameState->unk1C->unk81C[0] != arg1) {
                if (g_gameState->unk1C->unk0 & 0x20000000) {
                    func_800552B8(g_gameState->unk1C->unk824[0]);
                }
                g_gameState->unk1C->unk0 &= 0xCFFFFFFF;
                g_gameState->unk1C->unk820[0] = 0x1F3;
                g_gameState->unk1C->unk81C[0] = arg1;
                g_gameState->unk1C->unk826[0] = 0x7F;
            }
            g_gameState->unk1C->unk824[0] = func_80055290(g_gameState->unk1C->unk814);
            g = g_gameState->unk1C;
            if (!(g->unk0 & 0x20000000)) {
                func_80055A34(g->unk824[0], 0xA, 0, g->unk826[0]);
            }
            D_800AE43C = -1;
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
            g_gameState->unk1C->unk826[0] = arg3;
            return 0;
        case 4:
            if ((arg0 & 0x3F) != 1) {
                break;
            }
            func_80055A34(g_gameState->unk1C->unk824[0], arg2, arg3, arg4);
            g_gameState->unk1C->unk826[0] = arg4;
            return 0;
        case 6:
            if ((arg0 & 0x3F) != 1) {
                break;
            }
            func_800559FC(0, arg2, arg3);
            g_gameState->unk1C->unk826[0] = arg3;
            return 0;
        case 11:
            if ((arg0 & 0x3F) != 1) {
                break;
            }
            func_80055A34(0, arg2, arg3, arg4);
            g_gameState->unk1C->unk826[0] = arg4;
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

void func_800ACB98(void) {
    s32 i;

    for (i = 1; i >= 0; i--) {
        D_800739B0[i].unk1 = 0;
        D_800739B0[i].unk0 = 0;
    }
    g_gameState->unk1C->unk83C = (void (*)(s32, s32, s32, s32, s32))func_800AC598;
    func_800ACD70();
    func_800ACCB4();
    func_800239D0();
    SetDispMask(1);
}

void func_800ACC08(void) {
    s32 i;

    SetDispMask(0);
    if (g_gameState->unk1C->unk0 & 0x20000000) {
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
    memmove(g_gameState->unk1C->unk814, (void *)a, (s32)func_8001C7FC(5, 0x7009C, 0));
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
    func_8002177C(g_gameState->unk1C->unk854, e->unk4, (e->unkC - e->unk4) << 11, buf, 0, -1, 0);
    while (func_800217C4(g_gameState->unk1C->unk854) != 0) {
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
    s32 pad = g_gameState->unkB;

    do {
        func_8001DCB8();
    } while (g_gameState->unkB == 0xFF && ++i < 10);
    func_80033D1C(1);
    if (func_800AD0C4(pad, g_gameState->unkB)) {
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
        if (g_padRepeat & PAD_RIGHT) {
            D_800B0F48 += 10;
            if (D_800AE0E8 < D_800B0F48) {
                D_800B0F48 = D_800AE0E8 / 10 * 10;
            }
            if (D_800B0F48 > 1000) {
                D_800B0F48 = 1000;
            }
            snd_playSe(0x67);
        } else if (g_padRepeat & PAD_LEFT) {
            D_800B0F48 -= 10;
            if (D_800B0F48 < 10) {
                D_800B0F48 = 10;
            }
            snd_playSe(0x67);
        } else if (g_padRepeat & BTN_CROSS) {
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
    snd_playSe(0x67);
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
        if (g_padRepeat & PAD_UP) {
            snd_playSe(0x67);
            D_800B0FF0.unk3 = D_800B0FF0.unk3 != 0 ? D_800B0FF0.unk3 - 1 : 3;
        } else if (g_padRepeat & PAD_DOWN) {
            snd_playSe(0x67);
            D_800B0FF0.unk3 = D_800B0FF0.unk3 < 3 ? D_800B0FF0.unk3 + 1 : 0;
        } else if (g_padRepeat & BTN_CROSS) {
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
    snd_playSe(0x67);
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

s32 func_800AD7DC(void) {
    s32 t;
    s32 page;
    u32 *ot;
    u8 *msg;
    s32 x;

    page = 1;
    t = -1;
    func_800ADC3C();
    do {
        func_800ADC98();
        func_80019194();
        switch (D_800B5000) {
        case 0:
            page = 1;
            t = -1;
            D_800B5000 = 1;
            break;
        case 1:
            if (t == -1) {
                t = 0xA0;
                func_800320B8(1, 1, 0x14, 0);
            } else if (t != 0) {
                ot = g_gameState->unk1C->unk10[g_gameState->unk1C->unk8] + 8;
                msg = func_800ADE14(D_800A70EC, 0);
                x = 0xA0 - (func_800ADF90(msg) >> 1);
                D_800AE4B0 = func_800ADE30(x, 0x64, ot, msg, D_800AE4B0);
                if (t == 0x14) {
                    func_800320B8(1, 1, 0x14, 0xFFFFFF);
                }
                t--;
            } else {
                t = -1;
                D_800B5000 = 5;
            }
            break;
        case 2:
            if (t == -1) {
                t = 0xDC;
                func_800320B8(1, 1, 0x14, 0);
            } else if (t != 0) {
                ot = g_gameState->unk1C->unk10[g_gameState->unk1C->unk8] + 8;
                msg = func_800ADE14(D_800A70EC, 1);
                x = 0xA0 - (func_800ADF90(msg) >> 1);
                D_800AE4B0 = func_800ADE30(x, 0x5F, ot, msg, D_800AE4B0);
                if (t < 0x8C) {
                    msg = func_800ADE14(D_800A70EC, 2);
                    x = 0xA0 - (func_800ADF90(msg) >> 1);
                    D_800AE4B0 = func_800ADE30(x, 0x7D, ot, msg, D_800AE4B0);
                }
                if (t == 0x14) {
                    func_800320B8(1, 1, 0x14, 0xFFFFFF);
                }
                t--;
            } else {
                t = -1;
                D_800B5000 = 5;
            }
            break;
        case 3:
            if (t == -1) {
                t = 0xA0;
                func_800320B8(1, 1, 0x14, 0);
            } else if (t != 0) {
                ot = g_gameState->unk1C->unk10[g_gameState->unk1C->unk8] + 8;
                msg = func_800ADE14(D_800A70EC, 3);
                x = 0xA0 - (func_800ADF90(msg) >> 1);
                D_800AE4B0 = func_800ADE30(x, 0x64, ot, msg, D_800AE4B0);
                if (t == 0x14) {
                    func_800320B8(1, 1, 0x14, 0xFFFFFF);
                }
                t--;
            } else {
                t = -1;
                D_800B5000 = 5;
            }
            break;
        case 4:
            if (t == -1) {
                t = 0x118;
                func_800320B8(1, 1, 0x14, 0);
            } else if (t != 0) {
                ot = g_gameState->unk1C->unk10[g_gameState->unk1C->unk8] + 8;
                msg = func_800ADE14(D_800A70EC, 4);
                x = 0xA0 - (func_800ADF90(msg) >> 1);
                D_800AE4B0 = func_800ADE30(x, 0x5F, ot, msg, D_800AE4B0);
                if (t < 0x8C) {
                    msg = func_800ADE14(D_800A70EC, 5);
                    x = 0xA0 - (func_800ADF90(msg) >> 1);
                    D_800AE4B0 = func_800ADE30(x, 0x7D, ot, msg, D_800AE4B0);
                }
                if (t == 0x14) {
                    func_800320B8(1, 1, 0x14, 0xFFFFFF);
                }
                t--;
            } else {
                t = -1;
                D_800B5000 = 6;
            }
            break;
        case 5:
            if (t == -1) {
                t = 0x3C;
            } else if (t != 0) {
                t--;
            } else {
                t = -1;
                D_800B5000 = page + 1;
                page++;
            }
            break;
        }
        func_80032098();
        func_800ADD04();
    } while (D_800B5000 < 6);
    return 0;
}

s32 func_800ADC3C(void) {
    g_gameState->unk9 = 0;
    func_800320B8(1, 1, 0, 0xFFFFFF);
    D_800B5000 = 0;
    D_800AE4A8 = 0;
    D_800AE4AC = 0;
    D_80073998[0] = 0;
    return 0;
}

void func_800ADC98(void) {
    GameData *ctx = g_gameState->unk1C;
    ClearOTagR(ctx->unk10[ctx->unk8], 0x1000);
    D_800AE4B0 = D_800B1000[g_gameState->unk1C->unk8];
}

void func_800ADD04(void) {
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
    D_800AE4A8 = 0;
    D_800AE4AC++;
}

u8 *func_800ADE14(u8 *arg0, s32 arg1) {
    return arg0 + (arg0[arg1 * 4 + 4] + 4);
}

void *func_800ADE30(s32 x, s32 y, u32 *ot, u8 *s, void *p) {
    Unk7D4 *f = g_gameState->unk1C->unk7D4;
    s32 w;
    s32 c;
    s32 hi = f->unk4 >> 8;

    c = *s;
    while (c != 0xFF) {
        if (c >= hi) {
            c = (c << 8) + s[1] - f->unk4 + f->unk6;
            s++;
            p = func_800AE028(x, y, ot, *(f->unkC + c), &w, p);
            x += w;
        } else {
            p = func_800AE028(x, y, ot, *(f->unkC + c), &w, p);
            x += w;
        }
        c = *++s;
    }
    SetDrawTPage(p, 1, 1, 0x1E);
    addPrim(ot, p);
    return (DR_TPAGE *)p + 1;
}

s32 func_800ADF90(u8 *s) {
    Unk7D4 *f;
    s32 n;
    s32 c;
    u8 hi;
    u32 *g;

    n = 0;
    f = g_gameState->unk1C->unk7D4;
    hi = f->unk4 >> 8;
    while ((c = *s) != 0xFF) {
        if (c >= hi) {
            c = (c << 8) + s[1] - f->unk4 + f->unk6;
            s++;
            n += (f->unkC[c] >> 28) + 1;
        } else {
            g = &f->unkC[c];
            n += (*g >> 28) + 1;
        }
        s++;
    }
    return n;
}

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
