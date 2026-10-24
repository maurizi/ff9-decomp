#include "ovl_15.h"

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
        pad_update();
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
            g_gameState->vsyncMode = 4;
            DrawSync(0);
            MBG_init(1, &buf);
            mbg_setRgb24(1);
            mbg_setEndCallback(func_800A7A84);
            mbg_play(arg0, arg1);
            D_800AE4C0 = 2;
            break;
        case 2:
            if ((D_800AE0E0 & 0x10) && (func_80012AD4() & 2)) {
                r = mbg_update(0);
                mbg_getLastFrame();
                if (r != 0) {
                    DrawOTag(g_gameState->unk1C->unk10[g_gameState->unk1C->unk8 ^ 1] + 0xFFF);
                } else if (timer == -1 && g_gameState->unk1C->unk8 == 0) {
                    g_gameState->unkC[g_gameState->unk1C->unk8].disp.x = 0x1E0;
                }
            }
            break;
        case 3:
            if (D_800AE0E0 & 0x80) {
                g_gameState->vsyncMode = 0;
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
    e = file_find(0, 0x64);
    func_8002177C(g_gameState->unk1C->unk854, e->unk4, (e->unkC - e->unk4) << 11, D_80067944 + 0xF1700, 0, 0x15000, 0);
    while (func_800217C4(g_gameState->unk1C->unk854) != 0) {
    }
    EnterCriticalSection();
    FlushCache();
    ExitCriticalSection();
    if (g_gameState->unkB != 0xFF && (g_gameState->unkB & 8)) {
        h = cd_findFile(g_mbgMovieLists[arg0].unk4[arg1].unk0);
        func_80022ED4(h);
        while (cd_getState() != 0) {
        }
        mbg_streamSetBaseSector(h);
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
        g_gameState->vsyncMode = 0;
    }
    VSync(g_gameState->vsyncMode);
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
    mbg_shutdown();
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
INCLUDE_RODATA("asm/us/ovl_15/nonmatchings/tu_800A7378", D_800A7018);

INCLUDE_RODATA("asm/us/ovl_15/nonmatchings/tu_800A7378", D_800A701C);

INCLUDE_RODATA("asm/us/ovl_15/nonmatchings/tu_800A7378", D_800A7028);

INCLUDE_RODATA("asm/us/ovl_15/nonmatchings/tu_800A7378", D_800A702C);

INCLUDE_RODATA("asm/us/ovl_15/nonmatchings/tu_800A7378", D_800A7034);

INCLUDE_RODATA("asm/us/ovl_15/nonmatchings/tu_800A7378", D_800A703C);

INCLUDE_RODATA("asm/us/ovl_15/nonmatchings/tu_800A7378", D_800A7040);

INCLUDE_RODATA("asm/us/ovl_15/nonmatchings/tu_800A7378", D_800A7048);

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
        pad_update();
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
                ui_drawSmallDigits(10, dealer.unkA + 0x20, 0x10, buf, 0);
            }
            if (nsplit == 2) {
                func_800A9334(&split, ofsX[2], ofsY[2], -1);
                if (split.unk2 != 0) {
                    sprintf(buf, D_800A7018, split.unk4);
                    ui_drawSmallDigits(10, split.unkA + 0x20, 0x10, buf, 0);
                    if (split.unk4 != 21 && (split.unk0 & 1)) {
                        sprintf(buf, D_800A7018, split.unk6);
                        ui_drawSmallDigits(10, split.unkA + 0x2A, 0x10, buf, 5);
                    }
                }
            }
            func_800A9334(&player, ofsX[1], ofsY[1], -1);
            if (player.unk2 != 0) {
                sprintf(buf, D_800A7018, player.unk4);
                ui_drawSmallDigits(10, player.unkA + 0x20, 0x10, buf, 0);
                if (player.unk4 != 21 && (player.unk0 & 1)) {
                    sprintf(buf, D_800A7018, player.unk6);
                    ui_drawSmallDigits(10, player.unkA + 0x2A, 0x10, buf, 5);
                }
            }
            if (nsplit == 1) {
                func_800A9334(&split, ofsX[2], ofsY[2], -1);
                if (split.unk2 != 0) {
                    sprintf(buf, D_800A7018, split.unk4);
                    ui_drawSmallDigits(10, split.unkA + 0x20, 0x10, buf, 0);
                    if (split.unk4 != 21 && (split.unk0 & 1)) {
                        sprintf(buf, D_800A7018, split.unk6);
                        ui_drawSmallDigits(10, split.unkA + 0x2A, 0x10, buf, 5);
                    }
                }
            }
        }
        func_800A8F00(0xF0, 0xA);
        addPrim(g_gameState->unk1C->unk10[g_gameState->unk1C->unk8] + 10, &D_800AE4E8);
        sprintf(buf, D_800A701C);
        ui_drawSmallDigits(0x10C - (func_8002B9B8(buf) >> 1), 0x58, 0x10, buf, 4);
        sprintf(buf, D_800A7028, D_800AE0E8);
        ui_drawSmallDigits(0x10C - (func_8002B9B8(buf) >> 1), 0x64, 0x10, buf, 5);
        sprintf(buf, D_800A702C);
        ui_drawSmallDigits(0x10C - (func_8002B9B8(buf) >> 1), 0x70, 0x10, buf, 4);
        sprintf(buf, D_800A7028, D_800B0F48);
        ui_drawSmallDigits(0x10C - (func_8002B9B8(buf) >> 1), 0x7C, 0x10, buf, 0);
        sprintf(buf, D_800A7034);
        ui_drawSmallDigits(0x10C - (func_8002B9B8(buf) >> 1), 0x90, 0x10, buf, 0);
        sprintf(buf, D_800A703C);
        ui_drawSmallDigits(0x10C - (func_8002B9B8(buf) >> 1), 0xA0, 0x10, buf, 0);
        sprintf(buf, D_800A7040);
        ui_drawSmallDigits(0x10C - (func_8002B9B8(buf) >> 1), 0xB0, 0x10, buf, D_800B0F44 ? 0 : 2);
        sprintf(buf, D_800A7048);
        ui_drawSmallDigits(0x10C - (func_8002B9B8(buf) >> 1), 0xC0, 0x10, buf, D_800B0F4C ? 0 : 2);
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

    p = (s32)db_lookupRegistered(4, 0x44F4C, 0);
    func_8001D9CC(p, 0, 0xE4, 0x1C0, 0x180);
    func_8001D8A8(p);
    func_8001D9CC(p, 0, 0xE5, 0x1C0, 0x180);
    func_8001D8A8(p);
    p = (s32)db_lookupRegistered(4, 0x44F4D, 0);
    func_8001D9CC(p, 0, 0xE6, 0x1EC, 0x100);
    func_8001D8A8(p);
    p = (s32)db_lookupRegistered(4, 0x44F4E, 0);
    func_8001D9CC(p, 0, 0xE0, 0x1C0, 0x100);
    func_8001D8A8(p);
    p = (s32)db_lookupRegistered(4, 0x44F50, 0);
    func_8001D9CC(p, 0, 0xE1, 0x1D6, 0x100);
    func_8001D8A8(p);
    p = (s32)db_lookupRegistered(4, 0x44F4F, 0);
    func_8001D9CC(p, 0, 0xE2, 0x1C0, 0x140);
    func_8001D8A8(p);
    p = (s32)db_lookupRegistered(4, 0x44F51, 0);
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
    g_gameState->vsyncMode = 0;
    VSync(g_gameState->vsyncMode);
    ResetGraph(1);
    PutDispEnv(&g_gameState->unkC[g_gameState->unk1C->unk8]);
    ctx = g_gameState->unk1C;
    i = ctx->unk8;
    DrawOTagEnv(ctx->unk10[i] + 0xFFF, &g_gameState->unk10[i]);
    g_gameState->unk1C->unk8 ^= 1;
    func_8002C0DC(g_gameState->unk1C->unk8);
}
