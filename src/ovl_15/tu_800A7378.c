#include "ovl_15.h"

s32 ending_playMovie(s32 arg0, s32 arg1) {
    u8 *buf;
    s32 timer;
    s32 i;
    s32 r;

    buf = D_80067944 + 0x106700;
    timer = -1;
    ending_movieInit(arg0, arg1);
    do {
        ending_movieClearOt();
        pad_update();
        switch (g_movieState) {
        case 0:
            if (g_gameState->unkB != 0xFF && (g_gameState->unkB & 8)) {
                g_movieFlags = 1;
                g_movieState = 1;
            } else {
                g_movieState = 3;
                g_movieFlags |= 0x80;
            }
            break;
        case 1:
            i = 1;
            if (g_gameState->unk1C->unk8 != 0) {
                i = 2;
            }
            g_movieFlagQueue[0][i - 1] = 7;
            g_movieFlagQueue[0][i] = 0x10;
            g_gameState->vsyncMode = 4;
            DrawSync(0);
            MBG_init(1, &buf);
            mbg_setRgb24(1);
            mbg_setEndCallback(ending_movieOnEnd);
            mbg_play(arg0, arg1);
            g_movieState = 2;
            break;
        case 2:
            if ((g_movieFlags & 0x10) && (func_80012AD4() & 2)) {
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
            if (g_movieFlags & 0x80) {
                g_gameState->vsyncMode = 0;
                timer = 900;
                g_gameState->unkC[0].disp.x = 0x140;
                g_movieState = 4;
                g_gameState->unkC[1].disp.x = 0;
                D_80073998[0] = 0;
            }
            break;
        }
        if (!(g_movieFlags & 2)) {
            func_80032098();
        }
        ending_movieSwap();
    } while (g_movieState < 4);
    return 0;
}

s32 ending_movieInit(s32 arg0, s32 arg1) {
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
    g_movieState = 0;
    g_movieFlags = 0;
    for (i = 1; i >= 0; i--) {
        for (j = 3; j >= 0; j--) {
            g_movieFlagQueue[i][j] = 0;
        }
    }
    g_movieFrame = 0;
    D_80073998[0] = 0;
    return 0;
}

void ending_movieClearOt(void) {
    GameData *ctx = g_gameState->unk1C;
    ClearOTagR(ctx->unk10[ctx->unk8], 0x1000);
}

void ending_movieSwap(void) {
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
    if (g_movieFlags & 4) {
        PutDrawEnv(&g_gameState->unk10[g_gameState->unk1C->unk8]);
    } else {
        ctx = g_gameState->unk1C;
        k = ctx->unk8;
        DrawOTagEnv(ctx->unk10[k] + 0xFFF, &g_gameState->unk10[k]);
    }
    g_gameState->unk1C->unk8 ^= 1;
    func_8002C0DC(g_gameState->unk1C->unk8);
    g_movieFlags |= g_movieFlagQueue[0][0];
    g_movieFlags &= ~g_movieFlagQueue[1][0];
    for (i = 0; i < 2; i++) {
        for (j = 0; j < 3; j++) {
            g_movieFlagQueue[i][j] = g_movieFlagQueue[i][j + 1];
            g_movieFlagQueue[i][3] = 0;
        }
    }
    g_movieFrame++;
}

void ending_movieOnEnd(void) {
    mbg_shutdown();
    g_movieFlags |= 0x80;
    g_movieState = 3;
}

s32 bj_drawResult(s32 kind, s32 c, s32 x, s32 y) {
    SPRT *p;
    DR_TPAGE *t;
    s32 x1;
    s32 x2;

    switch (kind) {
    case 0:
        p = &g_bjSprites[g_gameState->unk1C->unk8][g_bjSprtCount];
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
        g_bjSprtCount++;
        t = &g_bjTpages[g_gameState->unk1C->unk8][g_bjTpageCount];
        setlen(t, 1);
        t->code[0] = (GetTPage(1, 1, 0x280, 0x100) & 0x9FF) | 0xE1000400;
        addPrim(&g_gameState->unk1C->unk10[g_gameState->unk1C->unk8][10], t);
        g_bjTpageCount++;
        p = &g_bjSprites[g_gameState->unk1C->unk8][g_bjSprtCount];
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
        g_bjSprtCount++;
        t = &g_bjTpages[g_gameState->unk1C->unk8][g_bjTpageCount];
        setlen(t, 1);
        t->code[0] = (GetTPage(1, 2, 0x280, 0x100) & 0x9FF) | 0xE1000400;
        addPrim(&g_gameState->unk1C->unk10[g_gameState->unk1C->unk8][10], t);
        g_bjTpageCount++;
        break;
    case 1:
        x1 = x - 2;
        p = &g_bjSprites[g_gameState->unk1C->unk8][g_bjSprtCount];
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
        g_bjSprtCount++;
        t = &g_bjTpages[g_gameState->unk1C->unk8][g_bjTpageCount];
        setlen(t, 1);
        t->code[0] = (GetTPage(1, 1, 0x280, 0x100) & 0x9FF) | 0xE1000400;
        addPrim(&g_gameState->unk1C->unk10[g_gameState->unk1C->unk8][10], t);
        g_bjTpageCount++;
        p = &g_bjSprites[g_gameState->unk1C->unk8][g_bjSprtCount];
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
        g_bjSprtCount++;
        t = &g_bjTpages[g_gameState->unk1C->unk8][g_bjTpageCount];
        setlen(t, 1);
        t->code[0] = (GetTPage(1, 2, 0x280, 0x100) & 0x9FF) | 0xE1000400;
        addPrim(&g_gameState->unk1C->unk10[g_gameState->unk1C->unk8][10], t);
        g_bjTpageCount++;
        break;
    case 2:
        x2 = x - 0x10;
        p = &g_bjSprites[g_gameState->unk1C->unk8][g_bjSprtCount];
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
        g_bjSprtCount++;
        t = &g_bjTpages[g_gameState->unk1C->unk8][g_bjTpageCount];
        setlen(t, 1);
        t->code[0] = (GetTPage(1, 1, 0x280, 0x100) & 0x9FF) | 0xE1000400;
        addPrim(&g_gameState->unk1C->unk10[g_gameState->unk1C->unk8][10], t);
        g_bjTpageCount++;
        p = &g_bjSprites[g_gameState->unk1C->unk8][g_bjSprtCount];
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
        g_bjSprtCount++;
        t = &g_bjTpages[g_gameState->unk1C->unk8][g_bjTpageCount];
        setlen(t, 1);
        t->code[0] = (GetTPage(1, 2, 0x280, 0x100) & 0x9FF) | 0xE1000400;
        addPrim(&g_gameState->unk1C->unk10[g_gameState->unk1C->unk8][10], t);
        g_bjTpageCount++;
        break;
    }
    return 0;
}

s32 bj_drawCardBack(s32 x, s32 y) {
    SPRT *p = &g_bjSprites[g_gameState->unk1C->unk8][g_bjSprtCount];

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
    g_bjSprtCount++;
    return 0;
}

s32 bj_drawCard(s32 rank, s32 suit, s32 x, s32 y) {
    SPRT *p;
    s32 i;
    s32 f;
    s32 px;
    s32 py;

    p = &g_bjSprites[g_gameState->unk1C->unk8][g_bjSprtCount];
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
    g_bjSprtCount++;

    p = &g_bjSprites[g_gameState->unk1C->unk8][g_bjSprtCount];
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
    g_bjSprtCount++;

    p = &g_bjSprites[g_gameState->unk1C->unk8][g_bjSprtCount];
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
    g_bjSprtCount++;

    p = &g_bjSprites[g_gameState->unk1C->unk8][g_bjSprtCount];
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
    g_bjSprtCount++;

    if (rank >= 9) {
        f = rank - 9;
        if (rank == 12) {
            p = &g_bjSprites[g_gameState->unk1C->unk8][g_bjSprtCount];
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
            g_bjSprtCount++;
        } else {
            p = &g_bjSprites[g_gameState->unk1C->unk8][g_bjSprtCount];
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
            g_bjSprtCount++;
        }
    } else {
        for (i = 0; i <= rank + 1; i++) {
            px = g_bjPips[rank][i].x;
            py = g_bjPips[rank][i].y;
            p = &g_bjSprites[g_gameState->unk1C->unk8][g_bjSprtCount];
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
            g_bjSprtCount++;
        }
    }

    p = &g_bjSprites[g_gameState->unk1C->unk8][g_bjSprtCount];
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
    g_bjSprtCount++;
    return 0;
}

s32 bj_drawShoe(s32 x, s32 y) {
    s32 i;

    for (i = 0; i < 3; i++) {
        bj_drawCardBack(x - i * 2, y - i);
    }
    return 0;
}

s32 bj_shoeInit(Ovl15Shoe *shoe) {
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

s32 bj_shoeShuffle(Ovl15Shoe *shoe) {
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

s32 bj_dealCard(Ovl15Hand *hand, Ovl15Shoe *shoe) {
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

s32 bj_handInit(Ovl15Hand *hand, s16 x, s16 y) {
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

s32 bj_splitHand(Ovl15Hand *a, Ovl15Hand *b) {
    s32 r;
    u16 v;

    a->unk2 = 1;
    a->unk8 = 0x1E;
    a->unkA = 0x8C;
    bj_handInit(b, 0x1E, 0x60);
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

s32 bj_handOptions(Ovl15Hand *hand) {
    s32 ret = 0;

    if (hand->unk2 == 2) {
        ret = 2;
        if ((u32)hand->unkC[0] >> 2 == (u32)hand->unkC[1] >> 2) {
            ret = 3;
        }
    }
    return ret;
}

s32 bj_drawHand(Ovl15Hand *hand, s32 dx, s32 dy, s32 count) {
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
            bj_drawCardBack(x, y);
            first = 0;
        } else {
            bj_drawCard(card / 4, card % 4, x, y);
        }
        x -= 13;
    }
    return 0;
}

s32 bj_isBlackjack(Ovl15Hand *h) {
    if (h->unk4 == 0x15 && h->unk2 == 2) {
        return 1;
    }
    return 0;
}

/* bj_main's format strings and Blackjack UI labels */
INCLUDE_RODATA("asm/us/ovl_15/nonmatchings/tu_800A7378", g_bjFmt2d);

INCLUDE_RODATA("asm/us/ovl_15/nonmatchings/tu_800A7378", g_bjStrBankroll);

INCLUDE_RODATA("asm/us/ovl_15/nonmatchings/tu_800A7378", g_bjFmtLong);

INCLUDE_RODATA("asm/us/ovl_15/nonmatchings/tu_800A7378", g_bjStrWager);

INCLUDE_RODATA("asm/us/ovl_15/nonmatchings/tu_800A7378", g_bjStrStand);

INCLUDE_RODATA("asm/us/ovl_15/nonmatchings/tu_800A7378", g_bjStrHit);

INCLUDE_RODATA("asm/us/ovl_15/nonmatchings/tu_800A7378", g_bjStrDouble);

INCLUDE_RODATA("asm/us/ovl_15/nonmatchings/tu_800A7378", g_bjStrSplit);

s32 bj_main(void) {
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
    bj_init();
    g_bjWager = 10;
    bj_handInit(&player, 0x1E, 0x78);
    bj_handInit(&dealer, 0x1E, 0xA);
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
        bj_clearOt();
        pad_update();
        switch (g_bjState) {
        case 0:
            t = -1;
            g_bjState = 1;
            break;
        case 1:
            if (t == -1) {
                t = 0x4E;
                bj_shoeInit(&g_bjShoe);
                bj_shoeShuffle(&g_bjShoe);
                func_800320B8(1, 1, 0x3C, 0);
                i = 0;
            } else if (t != 0) {
                c = g_bjShoe.unk4[i];
                bj_drawCard(c >> 2, c & 3, 0xF0, 0xA);
                i += 4;
                t--;
            } else {
                t = -1;
                g_bjState = 2;
                scene_set(2);
            }
            break;
        case 2:
            bj_handInit(&player, 0x1E, 0x78);
            bj_handInit(&dealer, 0x1E, 0xA);
            scene_update();
            if (scene_get() == 4) {
                t = -1;
                g_bjState = 3;
            }
            break;
        case 3:
            if (t == -1) {
                dealer.unk0 |= 0x10;
                bj_dealCard(p1, &g_bjShoe);
                bj_dealCard(&dealer, &g_bjShoe);
                bj_dealCard(p1, &g_bjShoe);
                bj_dealCard(&dealer, &g_bjShoe);
                cur = p1;
                i = 0;
                x = 0xF00000;
                y = 0xA0000;
                t = 0x78;
                dx = (((player.unk8 - 0xF0) << 8) / 30) << 8;
                dy = (((player.unkA - 0xA) << 8) / 30) << 8;
            } else if (t > 0) {
                bj_drawCardBack(x >> 16, y >> 16);
                bj_drawHand(&dealer, 0, 0, i >= 2);
                if (i > 0) {
                    a = 2;
                    if (i < 3) {
                        a = 1;
                    }
                } else {
                    a = 0;
                }
                bj_drawHand(p1, 0, 0, a);
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
            } else if (bj_isBlackjack(p1)) {
                if (bj_isBlackjack(&dealer)) {
                    t = -1;
                    g_bjState = 10;
                } else {
                    t = -1;
                    g_bjState = 8;
                }
            } else if (bj_isBlackjack(&dealer)) {
                t = -1;
                if (bj_isBlackjack(p1)) {
                    g_bjState = 10;
                } else {
                    g_bjState = 8;
                }
            } else {
                t = -1;
                g_bjState = 4;
                scene_set(3);
            }
            break;
        case 4:
            r = bj_handOptions(cur);
            g_bjCanDouble = 0;
            g_bjCanSplit = 0;
            if ((r & 2) && g_bjBankroll >= g_bjWager * 2) {
                g_bjCanDouble = 1;
            }
            if ((r & 1) && nsplit == 0 && g_bjBankroll >= g_bjWager * 2) {
                g_bjCanSplit = 1;
            }
            if (t == -1) {
                scene_update();
                if (scene_get() == 4) {
                    if (bj_actionGetCursor() == 0) {
                        if (nsplit == 1) {
                            cur = &split;
                            g_bjState = 4;
                            scene_set(3);
                            x = 0xF00000;
                            y = 0xA0000;
                            t = 30;
                            dx = (((cur->unk8 + cur->unk2 * 13 - 0xF0) << 8) / 30) << 8;
                            dy = (((cur->unkA - 0xA) << 8) / 30) << 8;
                            nsplit = 2;
                        } else {
                            g_bjState = 5;
                            dealer.unk0 &= ~0x10;
                        }
                    } else if (bj_actionGetCursor() == 1) {
                        x = 0xF00000;
                        y = 0xA0000;
                        t = 30;
                        dx = (((cur->unk8 + cur->unk2 * 13 - 0xF0) << 8) / 30) << 8;
                        dy = (((cur->unkA - 0xA) << 8) / 30) << 8;
                    } else if (bj_actionGetCursor() == 2) {
                        x = 0xF00000;
                        y = 0xA0000;
                        t = 30;
                        cur->unk0 |= 4;
                        dx = (((cur->unk8 + cur->unk2 * 13 - 0xF0) << 8) / 30) << 8;
                        dy = (((cur->unkA - 0xA) << 8) / 30) << 8;
                    } else if (bj_actionGetCursor() == 3) {
                        if (bj_handOptions(cur) & 1) {
                            bj_splitHand(&player, &split);
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
                bj_drawCardBack(x >> 16, y >> 16);
                x += dx;
                y += dy;
                t--;
            } else {
                bj_dealCard(cur, &g_bjShoe);
                t = -1;
                if (cur->unk4 >= 22) {
                    g_bjState = 7;
                } else if (cur->unk4 == 21) {
                    if (nsplit == 1) {
                        cur = &split;
                        g_bjState = 4;
                        scene_set(3);
                        x = 0xF00000;
                        y = 0xA0000;
                        t = 30;
                        dx = (((cur->unk8 + cur->unk2 * 13 - 0xF0) << 8) / 30) << 8;
                        dy = (((cur->unkA - 0xA) << 8) / 30) << 8;
                        nsplit = 2;
                    } else {
                        g_bjState = 5;
                    }
                } else if (cur->unk0 & 8) {
                    if (nsplit == 1) {
                        cur = &split;
                        g_bjState = 4;
                        scene_set(3);
                        x = 0xF00000;
                        y = 0xA0000;
                        t = 30;
                        dx = (((cur->unk8 + cur->unk2 * 13 - 0xF0) << 8) / 30) << 8;
                        dy = (((cur->unkA - 0xA) << 8) / 30) << 8;
                        nsplit = 2;
                    } else {
                        g_bjState = 5;
                    }
                } else if (cur->unk0 & 4) {
                    if (nsplit == 1) {
                        cur = &split;
                        g_bjState = 4;
                        scene_set(3);
                        x = 0xF00000;
                        y = 0xA0000;
                        t = 30;
                        dx = (((cur->unk8 + cur->unk2 * 13 - 0xF0) << 8) / 30) << 8;
                        dy = (((cur->unkA - 0xA) << 8) / 30) << 8;
                        nsplit = 2;
                    } else {
                        g_bjState = 5;
                    }
                } else {
                    g_bjState = 4;
                    scene_set(3);
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
                    g_bjState = 7;
                } else if (nsplit > 0) {
                    nsplit = 1;
                    g_bjState = 6;
                } else if (player.unk4 == dealer.unk4) {
                    g_bjState = 10;
                } else if (dealer.unk4 < player.unk4) {
                    g_bjState = 9;
                } else {
                    g_bjState = 11;
                }
            } else if (t > 0) {
                bj_drawCardBack(x >> 16, y >> 16);
                x += dx;
                y += dy;
                t--;
            } else {
                t = -1;
                g_bjState = 5;
                bj_dealCard(&dealer, &g_bjShoe);
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
                    g_bjState = 4;
                    scene_set(3);
                    x = 0xF00000;
                    y = 0xA0000;
                    t = 30;
                    dx = (((cur->unk8 + cur->unk2 * 13 - 0xF0) << 8) / 30) << 8;
                    dy = (((cur->unkA - 0xA) << 8) / 30) << 8;
                    nsplit = 2;
                } else if (nsplit == 2) {
                    nsplit = 1;
                    g_bjState = 6;
                } else if (cur->unk4 >= 22) {
                    g_bjState = 11;
                } else {
                    g_bjState = 9;
                }
            }
            break;
        case 6:
            if (t == -1) {
                if (nsplit == 1) {
                    if (player.unk4 >= 22) {
                        res = 1;
                        snd_playSe(0x6B);
                        g_bjBankroll -= (player.unk0 & 4) ? g_bjWager * 2 : g_bjWager;
                    } else if (dealer.unk4 >= 22 || dealer.unk4 < player.unk4) {
                        res = 0;
                        snd_playSe(0x6C);
                        g_bjBankroll += (player.unk0 & 4) ? g_bjWager * 2 : g_bjWager;
                    } else {
                        res = 2;
                        if (player.unk4 != dealer.unk4) {
                            res = 1;
                            snd_playSe(0x6B);
                            g_bjBankroll -= (player.unk0 & 4) ? g_bjWager * 2 : g_bjWager;
                        }
                    }
                } else {
                    if (split.unk4 >= 22) {
                        res = 1;
                        snd_playSe(0x6B);
                        g_bjBankroll -= (split.unk0 & 4) ? g_bjWager * 2 : g_bjWager;
                    } else if (dealer.unk4 >= 22 || dealer.unk4 < split.unk4) {
                        res = 0;
                        snd_playSe(0x6C);
                        g_bjBankroll += (split.unk0 & 4) ? g_bjWager * 2 : g_bjWager;
                    } else {
                        res = 2;
                        if (split.unk4 != dealer.unk4) {
                            res = 1;
                            snd_playSe(0x6B);
                            g_bjBankroll -= (split.unk0 & 4) ? g_bjWager * 2 : g_bjWager;
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
                bj_drawResult(res, a, 0x6C, nsplit == 1 ? player.unkA + 12 : split.unkA + 12);
                t--;
            } else {
                t = -1;
                if (nsplit == 1) {
                    nsplit = 2;
                } else {
                    g_bjState = 12;
                }
            }
            break;
        case 8:
            if (t == -1) {
                if (player.unk4 > dealer.unk4) {
                    snd_playSe(0x6C);
                    g_bjBankroll += g_bjWager + (g_bjWager >> 1);
                } else {
                    snd_playSe(0x6B);
                    g_bjBankroll -= g_bjWager;
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
                    bj_drawResult(0, a, 0x6C, player.unkA + 12);
                } else {
                    bj_drawResult(1, a, 0x6C, player.unkA + 12);
                }
                t--;
            } else {
                t = -1;
                g_bjState = 12;
            }
            break;
        case 9:
            if (t == -1) {
                snd_playSe(0x6C);
                {
                    s32 *pb = &g_bjBankroll;
                    *pb = g_bjBankroll + ((player.unk0 & 4) ? g_bjWager * 2 : g_bjWager);
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
                bj_drawResult(0, a, 0x6C, player.unkA + 12);
                t--;
            } else {
                t = -1;
                g_bjState = 12;
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
                bj_drawResult(2, a, 0x6C, player.unkA + 12);
                t--;
            } else {
                t = -1;
                g_bjState = 12;
            }
            break;
        case 11:
            if (t == -1) {
                snd_playSe(0x6B);
                g_bjBankroll -= (player.unk0 & 4) ? g_bjWager * 2 : g_bjWager;
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
                bj_drawResult(1, a, 0x6C, player.unkA + 12);
                t--;
            } else {
                t = -1;
                g_bjState = 12;
            }
            break;
        case 12:
            if (g_bjShoe.unk0 & 4) {
                t = -1;
                g_bjState = 13;
            } else {
                nsplit = 0;
                if (g_bjWager > g_bjBankroll) {
                    g_bjWager = g_bjBankroll / 10 * 10;
                }
                t = -1;
                g_bjState = 2;
                scene_set(2);
            }
            break;
        case 13:
            if (t == -1) {
                func_800320B8(1, 1, 0x3C, 0xFFFFFF);
                t = 0x3C;
                if (g_bjBankroll < 10) {
                    g_bjBankroll = 1000;
                }
            } else if (t > 0) {
                t--;
            } else {
                t = -1;
                g_bjState = 14;
            }
            break;
        }
        if (g_bjState >= 4) {
            bj_drawHand(&dealer, ofsX[0], ofsY[0], -1);
            if (dealer.unk2 != 0 && g_bjState >= 5) {
                sprintf(buf, g_bjFmt2d, dealer.unk4);
                ui_drawSmallDigits(10, dealer.unkA + 0x20, 0x10, buf, 0);
            }
            if (nsplit == 2) {
                bj_drawHand(&split, ofsX[2], ofsY[2], -1);
                if (split.unk2 != 0) {
                    sprintf(buf, g_bjFmt2d, split.unk4);
                    ui_drawSmallDigits(10, split.unkA + 0x20, 0x10, buf, 0);
                    if (split.unk4 != 21 && (split.unk0 & 1)) {
                        sprintf(buf, g_bjFmt2d, split.unk6);
                        ui_drawSmallDigits(10, split.unkA + 0x2A, 0x10, buf, 5);
                    }
                }
            }
            bj_drawHand(&player, ofsX[1], ofsY[1], -1);
            if (player.unk2 != 0) {
                sprintf(buf, g_bjFmt2d, player.unk4);
                ui_drawSmallDigits(10, player.unkA + 0x20, 0x10, buf, 0);
                if (player.unk4 != 21 && (player.unk0 & 1)) {
                    sprintf(buf, g_bjFmt2d, player.unk6);
                    ui_drawSmallDigits(10, player.unkA + 0x2A, 0x10, buf, 5);
                }
            }
            if (nsplit == 1) {
                bj_drawHand(&split, ofsX[2], ofsY[2], -1);
                if (split.unk2 != 0) {
                    sprintf(buf, g_bjFmt2d, split.unk4);
                    ui_drawSmallDigits(10, split.unkA + 0x20, 0x10, buf, 0);
                    if (split.unk4 != 21 && (split.unk0 & 1)) {
                        sprintf(buf, g_bjFmt2d, split.unk6);
                        ui_drawSmallDigits(10, split.unkA + 0x2A, 0x10, buf, 5);
                    }
                }
            }
        }
        bj_drawShoe(0xF0, 0xA);
        addPrim(g_gameState->unk1C->unk10[g_gameState->unk1C->unk8] + 10, &g_bjTpage);
        sprintf(buf, g_bjStrBankroll);
        ui_drawSmallDigits(0x10C - (func_8002B9B8(buf) >> 1), 0x58, 0x10, buf, 4);
        sprintf(buf, g_bjFmtLong, g_bjBankroll);
        ui_drawSmallDigits(0x10C - (func_8002B9B8(buf) >> 1), 0x64, 0x10, buf, 5);
        sprintf(buf, g_bjStrWager);
        ui_drawSmallDigits(0x10C - (func_8002B9B8(buf) >> 1), 0x70, 0x10, buf, 4);
        sprintf(buf, g_bjFmtLong, g_bjWager);
        ui_drawSmallDigits(0x10C - (func_8002B9B8(buf) >> 1), 0x7C, 0x10, buf, 0);
        sprintf(buf, g_bjStrStand);
        ui_drawSmallDigits(0x10C - (func_8002B9B8(buf) >> 1), 0x90, 0x10, buf, 0);
        sprintf(buf, g_bjStrHit);
        ui_drawSmallDigits(0x10C - (func_8002B9B8(buf) >> 1), 0xA0, 0x10, buf, 0);
        sprintf(buf, g_bjStrDouble);
        ui_drawSmallDigits(0x10C - (func_8002B9B8(buf) >> 1), 0xB0, 0x10, buf, g_bjCanDouble ? 0 : 2);
        sprintf(buf, g_bjStrSplit);
        ui_drawSmallDigits(0x10C - (func_8002B9B8(buf) >> 1), 0xC0, 0x10, buf, g_bjCanSplit ? 0 : 2);
        D_800739B0[0].unk0 = 0;
        if (cnt > 0) {
            cnt--;
        }
        if (g_bjState < 14) {
            if (g_bjBankroll < 10) {
                t = -1;
                g_bjState = 13;
            }
            if (g_padPortPressed[0] & 8) {
                t = -1;
                g_bjState = 13;
            }
        }
        func_80032098();
        bj_swapBuffers();
    } while (g_bjState < 14);
    bj_exit();
    return 0;
}

s32 bj_init(void) {
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
    setRECT(&g_bjClutRect, 0x20, 0xE4, 0x10, 1);
    LoadImage(&g_bjClutRect, g_bjClut);
    DrawSync(0);
    setlen(&g_bjTpage, 1);
    g_bjTpage.code = 0xE1000697;
    if (!(g_gameState->unk1C->unk0 & 0x20000000)) {
        ending_soundCmd(0, 0x7009C, 0, 0, 0);
        ending_soundCmd(0xC401, 0x7009C, 0x5A, 0, 0x7F);
    }
    func_800320B8(1, 1, 0, 0xFFFFFF);
    g_bjState = 0;
    D_80073998[0] = 0;
    return 0;
}

s32 bj_exit(void) {
    return 0;
}

void bj_clearOt(void) {
    GameData *ctx = g_gameState->unk1C;
    ClearOTagR(ctx->unk10[ctx->unk8], 0x1000);
    g_bjSprtCount = 0;
    g_bjTpageCount = 0;
}

void bj_swapBuffers(void) {
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
