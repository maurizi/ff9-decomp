#include "ovl_15.h"

s32 ending_playMovie(s32 arg0, s32 arg1) {
    u8 *buf;
    s32 timer;
    s32 i;
    s32 r;

    buf = g_loadAreaEnd + 0x106700;
    timer = -1;
    ending_movieInit(arg0, arg1);
    do {
        ending_movieClearOt();
        pad_update();
        switch (g_movieState) {
        case MOVIE_ST_START:
            if (g_gameState->disc != DISC_NONE && (g_gameState->disc & DISC_4)) {
                g_movieFlags = MOVIE_NO_SCENES;
                g_movieState = MOVIE_ST_OPEN;
            } else {
                g_movieState = MOVIE_ST_END;
                g_movieFlags |= MOVIE_DONE;
            }
            break;
        case MOVIE_ST_OPEN:
            i = 1;
            if (g_gameState->gameData->drawBuf != 0) {
                i = 2;
            }
            g_movieFlagQueue[0][i - 1] = MOVIE_NO_SCENES | MOVIE_NO_FADE | MOVIE_NO_OT;
            g_movieFlagQueue[0][i] = MOVIE_PLAYING;
            g_gameState->vsyncMode = 4;
            DrawSync(0);
            MBG_init(1, &buf);
            mbg_setRgb24(1);
            mbg_setEndCallback(ending_movieOnEnd);
            mbg_play(arg0, arg1);
            g_movieState = MOVIE_ST_PLAY;
            break;
        case MOVIE_ST_PLAY:
            if ((g_movieFlags & MOVIE_PLAYING) && (mbg_getStatus() & 2)) {
                r = mbg_update(0);
                mbg_getLastFrame();
                if (r != 0) {
                    DrawOTag(g_gameState->gameData->ot[g_gameState->gameData->drawBuf ^ 1] + 0xFFF);
                } else if (timer == -1 && g_gameState->gameData->drawBuf == 0) {
                    g_gameState->dispEnv[g_gameState->gameData->drawBuf].disp.x = 0x1E0;
                }
            }
            break;
        case MOVIE_ST_END:
            if (g_movieFlags & MOVIE_DONE) {
                g_gameState->vsyncMode = 0;
                timer = 900;
                g_gameState->dispEnv[0].disp.x = 0x140;
                g_movieState = MOVIE_ST_DONE;
                g_gameState->dispEnv[1].disp.x = 0;
                g_padPortMask[0] = 0;
            }
            break;
        }
        if (!(g_movieFlags & MOVIE_NO_FADE)) {
            game_updateFade();
        }
        ending_movieSwap();
    } while (g_movieState < MOVIE_ST_DONE);
    return 0;
}

s32 ending_movieInit(s32 arg0, s32 arg1) {
    FileEntry *e;
    s32 h;
    s32 i;
    s32 j;

    while (cdRead_update(g_gameState->gameData->cdRead) != 0) {
    }
    e = file_find(0, 0x64);
    cdRead_init(g_gameState->gameData->cdRead, e->sector, (e->unkC - e->sector) << 11, g_loadAreaEnd + 0xF1700, 0, 0x15000, 0);
    while (cdRead_update(g_gameState->gameData->cdRead) != 0) {
    }
    EnterCriticalSection();
    FlushCache();
    ExitCriticalSection();
    if (g_gameState->disc != DISC_NONE && (g_gameState->disc & DISC_4)) {
        h = cd_findFile(g_mbgMovieLists[arg0].movies[arg1].path);
        cd_seekStreamAsync(h);
        while (cd_getState() != 0) {
        }
        mbg_streamSetBaseSector(h);
    }
    game_startFade(1, 1, 0x78, 0xFFFFFF);
    g_movieState = MOVIE_ST_START;
    g_movieFlags = 0;
    for (i = 1; i >= 0; i--) {
        for (j = 3; j >= 0; j--) {
            g_movieFlagQueue[i][j] = 0;
        }
    }
    g_movieFrame = 0;
    g_padPortMask[0] = 0;
    return 0;
}

void ending_movieClearOt(void) {
    GameData *ctx = g_gameState->gameData;
    ClearOTagR(ctx->ot[ctx->drawBuf], 0x1000);
}

void ending_movieSwap(void) {
    GameData *ctx;
    u8 k;
    s32 i;
    s32 j;

    DrawSync(0);
    if (!(mbg_getStatus() & 2)) {
        g_gameState->vsyncMode = 0;
    }
    VSync(g_gameState->vsyncMode);
    ResetGraph(1);
    PutDispEnv(&g_gameState->dispEnv[g_gameState->gameData->drawBuf]);
    if (g_movieFlags & MOVIE_NO_OT) {
        PutDrawEnv(&g_gameState->drawEnv[g_gameState->gameData->drawBuf]);
    } else {
        ctx = g_gameState->gameData;
        k = ctx->drawBuf;
        DrawOTagEnv(ctx->ot[k] + 0xFFF, &g_gameState->drawEnv[k]);
    }
    g_gameState->gameData->drawBuf ^= 1;
    gfx_setBuffer(g_gameState->gameData->drawBuf);
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
    g_movieFlags |= MOVIE_DONE;
    g_movieState = MOVIE_ST_END;
}

s32 bj_drawResult(s32 kind, s32 c, s32 x, s32 y) {
    SPRT *p;
    DR_TPAGE *t;
    s32 x1;
    s32 x2;

    switch (kind) {
    case 0:
        p = &g_bjSprites[g_gameState->gameData->drawBuf][g_bjSprtCount];
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
        addPrim(&g_gameState->gameData->ot[g_gameState->gameData->drawBuf][BJ_OTZ], p);
        g_bjSprtCount++;
        t = &g_bjTpages[g_gameState->gameData->drawBuf][g_bjTpageCount];
        setlen(t, 1);
        t->code[0] = (GetTPage(1, 1, 0x280, 0x100) & 0x9FF) | 0xE1000400;
        addPrim(&g_gameState->gameData->ot[g_gameState->gameData->drawBuf][BJ_OTZ], t);
        g_bjTpageCount++;
        p = &g_bjSprites[g_gameState->gameData->drawBuf][g_bjSprtCount];
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
        addPrim(&g_gameState->gameData->ot[g_gameState->gameData->drawBuf][BJ_OTZ], p);
        g_bjSprtCount++;
        t = &g_bjTpages[g_gameState->gameData->drawBuf][g_bjTpageCount];
        setlen(t, 1);
        t->code[0] = (GetTPage(1, 2, 0x280, 0x100) & 0x9FF) | 0xE1000400;
        addPrim(&g_gameState->gameData->ot[g_gameState->gameData->drawBuf][BJ_OTZ], t);
        g_bjTpageCount++;
        break;
    case 1:
        x1 = x - 2;
        p = &g_bjSprites[g_gameState->gameData->drawBuf][g_bjSprtCount];
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
        addPrim(&g_gameState->gameData->ot[g_gameState->gameData->drawBuf][BJ_OTZ], p);
        g_bjSprtCount++;
        t = &g_bjTpages[g_gameState->gameData->drawBuf][g_bjTpageCount];
        setlen(t, 1);
        t->code[0] = (GetTPage(1, 1, 0x280, 0x100) & 0x9FF) | 0xE1000400;
        addPrim(&g_gameState->gameData->ot[g_gameState->gameData->drawBuf][BJ_OTZ], t);
        g_bjTpageCount++;
        p = &g_bjSprites[g_gameState->gameData->drawBuf][g_bjSprtCount];
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
        addPrim(&g_gameState->gameData->ot[g_gameState->gameData->drawBuf][BJ_OTZ], p);
        g_bjSprtCount++;
        t = &g_bjTpages[g_gameState->gameData->drawBuf][g_bjTpageCount];
        setlen(t, 1);
        t->code[0] = (GetTPage(1, 2, 0x280, 0x100) & 0x9FF) | 0xE1000400;
        addPrim(&g_gameState->gameData->ot[g_gameState->gameData->drawBuf][BJ_OTZ], t);
        g_bjTpageCount++;
        break;
    case 2:
        x2 = x - 0x10;
        p = &g_bjSprites[g_gameState->gameData->drawBuf][g_bjSprtCount];
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
        addPrim(&g_gameState->gameData->ot[g_gameState->gameData->drawBuf][BJ_OTZ], p);
        g_bjSprtCount++;
        t = &g_bjTpages[g_gameState->gameData->drawBuf][g_bjTpageCount];
        setlen(t, 1);
        t->code[0] = (GetTPage(1, 1, 0x280, 0x100) & 0x9FF) | 0xE1000400;
        addPrim(&g_gameState->gameData->ot[g_gameState->gameData->drawBuf][BJ_OTZ], t);
        g_bjTpageCount++;
        p = &g_bjSprites[g_gameState->gameData->drawBuf][g_bjSprtCount];
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
        addPrim(&g_gameState->gameData->ot[g_gameState->gameData->drawBuf][BJ_OTZ], p);
        g_bjSprtCount++;
        t = &g_bjTpages[g_gameState->gameData->drawBuf][g_bjTpageCount];
        setlen(t, 1);
        t->code[0] = (GetTPage(1, 2, 0x280, 0x100) & 0x9FF) | 0xE1000400;
        addPrim(&g_gameState->gameData->ot[g_gameState->gameData->drawBuf][BJ_OTZ], t);
        g_bjTpageCount++;
        break;
    }
    return 0;
}

s32 bj_drawCardBack(s32 x, s32 y) {
    SPRT *p = &g_bjSprites[g_gameState->gameData->drawBuf][g_bjSprtCount];

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
    addPrim(&g_gameState->gameData->ot[g_gameState->gameData->drawBuf][BJ_OTZ], p);
    g_bjSprtCount++;
    return 0;
}

s32 bj_drawCard(s32 rank, s32 suit, s32 x, s32 y) {
    SPRT *p;
    s32 i;
    s32 f;
    s32 px;
    s32 py;

    p = &g_bjSprites[g_gameState->gameData->drawBuf][g_bjSprtCount];
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
    addPrim(&g_gameState->gameData->ot[g_gameState->gameData->drawBuf][BJ_OTZ], p);
    g_bjSprtCount++;

    p = &g_bjSprites[g_gameState->gameData->drawBuf][g_bjSprtCount];
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
    addPrim(&g_gameState->gameData->ot[g_gameState->gameData->drawBuf][BJ_OTZ], p);
    g_bjSprtCount++;

    p = &g_bjSprites[g_gameState->gameData->drawBuf][g_bjSprtCount];
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
    addPrim(&g_gameState->gameData->ot[g_gameState->gameData->drawBuf][BJ_OTZ], p);
    g_bjSprtCount++;

    p = &g_bjSprites[g_gameState->gameData->drawBuf][g_bjSprtCount];
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
    addPrim(&g_gameState->gameData->ot[g_gameState->gameData->drawBuf][BJ_OTZ], p);
    g_bjSprtCount++;

    if (rank >= 9) {
        f = rank - 9;
        if (rank == 12) {
            p = &g_bjSprites[g_gameState->gameData->drawBuf][g_bjSprtCount];
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
            addPrim(&g_gameState->gameData->ot[g_gameState->gameData->drawBuf][BJ_OTZ], p);
            g_bjSprtCount++;
        } else {
            p = &g_bjSprites[g_gameState->gameData->drawBuf][g_bjSprtCount];
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
            addPrim(&g_gameState->gameData->ot[g_gameState->gameData->drawBuf][BJ_OTZ], p);
            g_bjSprtCount++;
        }
    } else {
        for (i = 0; i <= rank + 1; i++) {
            px = g_bjPips[rank][i].x;
            py = g_bjPips[rank][i].y;
            p = &g_bjSprites[g_gameState->gameData->drawBuf][g_bjSprtCount];
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
            addPrim(&g_gameState->gameData->ot[g_gameState->gameData->drawBuf][BJ_OTZ], p);
            g_bjSprtCount++;
        }
    }

    p = &g_bjSprites[g_gameState->gameData->drawBuf][g_bjSprtCount];
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
    addPrim(&g_gameState->gameData->ot[g_gameState->gameData->drawBuf][BJ_OTZ], p);
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

s32 bj_shoeInit(BjShoe *shoe) {
    s32 i;
    s32 j;

    for (i = 0; i < BJ_DECKS; i++) {
        for (j = 0; j < BJ_DECK_SIZE; j++) {
            shoe->cards[i * BJ_DECK_SIZE + j] = j;
        }
    }
    shoe->flags = BJ_SHOE_FRESH;
    shoe->next = 0;
    return 0;
}

s32 bj_shoeShuffle(BjShoe *shoe) {
    s32 pass;
    s32 n;
    s32 i;
    s32 j;
    u8 t;

    for (pass = 0; pass < BJ_SHUFFLE_PASSES; pass++) {
        for (i = 0, n = BJ_SHOE_SIZE; n > 0; n--, i++) {
            if (g_randPtr >= g_randTableEnd) {
                rand_refill();
            }
            j = (*g_randPtr++ * 2) % n;
            t = shoe->cards[i];
            shoe->cards[i] = shoe->cards[i + j];
            shoe->cards[i + j] = t;
        }
    }
    shoe->flags = (shoe->flags & ~BJ_SHOE_FRESH) | BJ_SHOE_SHUFFLED;
    return 0;
}

s32 bj_dealCard(BjHand *hand, BjShoe *shoe) {
    s32 card = shoe->cards[shoe->next++];
    s32 rank;

    hand->cards[hand->count++] = card;
    rank = card / 4;
    if (rank < BJ_RANK_JACK) {
        hand->value += rank + 2;
        hand->hardValue += rank + 2;
    } else if (rank == BJ_RANK_ACE) {
        hand->value += BJ_ACE_VALUE;
        hand->hardValue += BJ_ACE_LOW;
        hand->flags |= BJ_HAND_SOFT;
    } else {
        hand->value += BJ_TEN_VALUE;
        hand->hardValue += BJ_TEN_VALUE;
    }
    if (hand->value >= BJ_BUST) {
        if (hand->flags & BJ_HAND_SOFT) {
            hand->value -= BJ_ACE_VALUE - BJ_ACE_LOW;
            if (hand->hardValue >= hand->value) {
                hand->flags &= ~BJ_HAND_SOFT;
            }
        } else {
            hand->flags |= BJ_HAND_BUST;
        }
    }
    if (shoe->next >= BJ_CUT_CARD) {
        shoe->flags |= BJ_SHOE_CUT;
    }
    return 0;
}

s32 bj_handInit(BjHand *hand, s16 x, s16 y) {
    s32 i;

    hand->flags = 0;
    hand->count = 0;
    hand->value = 0;
    hand->hardValue = 0;
    hand->x = x;
    hand->y = y;
    for (i = BJ_HAND_MAX - 1; i >= 0; i--) {
        hand->cards[i] = 0;
    }
    return 0;
}

s32 bj_splitHand(BjHand *a, BjHand *b) {
    s32 r;
    u16 v;

    a->count = 1;
    a->x = BJ_HAND_X;
    a->y = BJ_SPLIT1_Y;
    bj_handInit(b, BJ_HAND_X, BJ_SPLIT2_Y);
    b->count = 1;
    b->cards[0] = a->cards[1];
    r = (u32)a->cards[1] >> 2;
    if (r < BJ_RANK_JACK) {
        v = r + 2;
    } else if (r == BJ_RANK_ACE) {
        a->value = BJ_ACE_VALUE;
        a->hardValue = BJ_ACE_LOW;
        a->flags |= BJ_HAND_SPLIT_ACES | BJ_HAND_SOFT;
        b->value = BJ_ACE_VALUE;
        b->hardValue = BJ_ACE_LOW;
        b->flags |= BJ_HAND_SPLIT_ACES | BJ_HAND_SOFT;
        goto end;
    } else {
        v = BJ_TEN_VALUE;
    }
    a->value = v;
    a->hardValue = v;
    b->value = v;
    b->hardValue = v;
end:
    return 0;
}

s32 bj_handOptions(BjHand *hand) {
    s32 ret = 0;

    if (hand->count == 2) {
        ret = BJ_CAN_DOUBLE;
        if ((u32)hand->cards[0] >> 2 == (u32)hand->cards[1] >> 2) {
            ret = BJ_CAN_DOUBLE | BJ_CAN_SPLIT;
        }
    }
    return ret;
}

s32 bj_drawHand(BjHand *hand, s32 dx, s32 dy, s32 count) {
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
        n = hand->count - 1;
        if (hand->count == 0) {
            return 1;
        }
    }
    first = 1;
    x = hand->x + dx + n * BJ_CARD_STEP;
    y = hand->y + dy;
    for (i = n; i >= 0; i--) {
        card = hand->cards[i];
        if (first && (hand->flags & BJ_HAND_HOLE) && n == 1) {
            bj_drawCardBack(x, y);
            first = 0;
        } else {
            bj_drawCard(card / 4, card % 4, x, y);
        }
        x -= BJ_CARD_STEP;
    }
    return 0;
}

s32 bj_isBlackjack(BjHand *h) {
    if (h->value == BJ_BLACKJACK && h->count == 2) {
        return 1;
    }
    return 0;
}

/* bj_main's format strings and Blackjack UI labels */
INCLUDE_RODATA("asm/us/ovl_15/nonmatchings/blackjack", g_bjFmt2d);

INCLUDE_RODATA("asm/us/ovl_15/nonmatchings/blackjack", g_bjStrBankroll);

INCLUDE_RODATA("asm/us/ovl_15/nonmatchings/blackjack", g_bjFmtLong);

INCLUDE_RODATA("asm/us/ovl_15/nonmatchings/blackjack", g_bjStrWager);

INCLUDE_RODATA("asm/us/ovl_15/nonmatchings/blackjack", g_bjStrStand);

INCLUDE_RODATA("asm/us/ovl_15/nonmatchings/blackjack", g_bjStrHit);

INCLUDE_RODATA("asm/us/ovl_15/nonmatchings/blackjack", g_bjStrDouble);

INCLUDE_RODATA("asm/us/ovl_15/nonmatchings/blackjack", g_bjStrSplit);

s32 bj_main(void) {
    char buf[16];
    s32 ofsX[3];
    s32 ofsY[3];
    BjHand dealer;
    BjHand player;
    BjHand split;
    s32 t;
    BjHand *cur;
    s32 nsplit;
    s32 cnt;
    s32 i;
    s32 res;
    s32 x;
    s32 y;
    s32 dx;
    s32 dy;
    BjHand *p1;
    s32 r;
    s32 a;
    s32 c;

    t = -1;
    cnt = 0;
    bj_init();
    g_bjWager = BJ_MIN_BET;
    bj_handInit(&player, BJ_HAND_X, BJ_PLAYER_Y);
    bj_handInit(&dealer, BJ_HAND_X, BJ_DEALER_Y);
    res = BJ_RESULT_WIN;
    i = 0;
    cur = &player;
    nsplit = 0;
    x = BJ_SHOE_X;
    y = BJ_SHOE_Y;
    dx = 0x10000;
    dy = 0x10000;
    p1 = cur;
    ofsX[0] = ofsX[1] = ofsX[2] = 0;
    ofsY[0] = ofsY[1] = ofsY[2] = 0;
    do {
        bj_clearOt();
        pad_update();
        switch (g_bjState) {
        case BJ_ST_START:
            t = -1;
            g_bjState = BJ_ST_SHUFFLE;
            break;
        case BJ_ST_SHUFFLE:
            if (t == -1) {
                t = 0x4E;
                bj_shoeInit(&g_bjShoe);
                bj_shoeShuffle(&g_bjShoe);
                game_startFade(1, 1, 0x3C, 0);
                i = 0;
            } else if (t != 0) {
                c = g_bjShoe.cards[i];
                bj_drawCard(c >> 2, c & 3, BJ_SHOE_X, BJ_SHOE_Y);
                i += 4;
                t--;
            } else {
                t = -1;
                g_bjState = BJ_ST_BET;
                scene_set(SCENE_BJ_WAGER);
            }
            break;
        case BJ_ST_BET:
            bj_handInit(&player, BJ_HAND_X, BJ_PLAYER_Y);
            bj_handInit(&dealer, BJ_HAND_X, BJ_DEALER_Y);
            scene_update();
            if (scene_get() == SCENE_CHOSEN) {
                t = -1;
                g_bjState = BJ_ST_DEAL;
            }
            break;
        case BJ_ST_DEAL:
            if (t == -1) {
                dealer.flags |= BJ_HAND_HOLE;
                bj_dealCard(p1, &g_bjShoe);
                bj_dealCard(&dealer, &g_bjShoe);
                bj_dealCard(p1, &g_bjShoe);
                bj_dealCard(&dealer, &g_bjShoe);
                cur = p1;
                i = 0;
                x = BJ_SHOE_X << 16;
                y = BJ_SHOE_Y << 16;
                t = 0x78;
                dx = (((player.x - BJ_SHOE_X) << 8) / BJ_DEAL_FRAMES) << 8;
                dy = (((player.y - BJ_SHOE_Y) << 8) / BJ_DEAL_FRAMES) << 8;
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
                if ((t - 1) % BJ_DEAL_FRAMES == 0 && ++i < 4) {
                    x = BJ_SHOE_X << 16;
                    y = BJ_SHOE_Y << 16;
                    switch (i) {
                    case 1:
                        dx = (((dealer.x - BJ_SHOE_X) << 8) / BJ_DEAL_FRAMES) << 8;
                        dy = (((dealer.y - BJ_SHOE_Y) << 8) / BJ_DEAL_FRAMES) << 8;
                        break;
                    case 2:
                        dx = (((player.x - 0xE3) << 8) / BJ_DEAL_FRAMES) << 8;
                        dy = (((player.y - BJ_SHOE_Y) << 8) / BJ_DEAL_FRAMES) << 8;
                        break;
                    case 3:
                        dx = (((dealer.x - 0xE3) << 8) / BJ_DEAL_FRAMES) << 8;
                        dy = (((dealer.y - BJ_SHOE_Y) << 8) / BJ_DEAL_FRAMES) << 8;
                        break;
                    }
                }
                t--;
            } else if (bj_isBlackjack(p1)) {
                if (bj_isBlackjack(&dealer)) {
                    t = -1;
                    g_bjState = BJ_ST_PUSH;
                } else {
                    t = -1;
                    g_bjState = BJ_ST_BLACKJACK;
                }
            } else if (bj_isBlackjack(&dealer)) {
                t = -1;
                if (bj_isBlackjack(p1)) {
                    g_bjState = BJ_ST_PUSH;
                } else {
                    g_bjState = BJ_ST_BLACKJACK;
                }
            } else {
                t = -1;
                g_bjState = BJ_ST_PLAYER;
                scene_set(SCENE_BJ_ACTION);
            }
            break;
        case BJ_ST_PLAYER:
            r = bj_handOptions(cur);
            g_bjCanDouble = 0;
            g_bjCanSplit = 0;
            if ((r & BJ_CAN_DOUBLE) && g_bjBankroll >= g_bjWager * 2) {
                g_bjCanDouble = 1;
            }
            if ((r & BJ_CAN_SPLIT) && nsplit == 0 && g_bjBankroll >= g_bjWager * 2) {
                g_bjCanSplit = 1;
            }
            if (t == -1) {
                scene_update();
                if (scene_get() == SCENE_CHOSEN) {
                    if (bj_actionGetCursor() == BJ_STAND) {
                        if (nsplit == 1) {
                            cur = &split;
                            g_bjState = BJ_ST_PLAYER;
                            scene_set(SCENE_BJ_ACTION);
                            x = BJ_SHOE_X << 16;
                            y = BJ_SHOE_Y << 16;
                            t = BJ_DEAL_FRAMES;
                            dx = (((cur->x + cur->count * BJ_CARD_STEP - BJ_SHOE_X) << 8) / BJ_DEAL_FRAMES) << 8;
                            dy = (((cur->y - BJ_SHOE_Y) << 8) / BJ_DEAL_FRAMES) << 8;
                            nsplit = 2;
                        } else {
                            g_bjState = BJ_ST_DEALER;
                            dealer.flags &= ~BJ_HAND_HOLE;
                        }
                    } else if (bj_actionGetCursor() == BJ_HIT) {
                        x = BJ_SHOE_X << 16;
                        y = BJ_SHOE_Y << 16;
                        t = BJ_DEAL_FRAMES;
                        dx = (((cur->x + cur->count * BJ_CARD_STEP - BJ_SHOE_X) << 8) / BJ_DEAL_FRAMES) << 8;
                        dy = (((cur->y - BJ_SHOE_Y) << 8) / BJ_DEAL_FRAMES) << 8;
                    } else if (bj_actionGetCursor() == BJ_DOUBLE) {
                        x = BJ_SHOE_X << 16;
                        y = BJ_SHOE_Y << 16;
                        t = BJ_DEAL_FRAMES;
                        cur->flags |= BJ_HAND_DOUBLED;
                        dx = (((cur->x + cur->count * BJ_CARD_STEP - BJ_SHOE_X) << 8) / BJ_DEAL_FRAMES) << 8;
                        dy = (((cur->y - BJ_SHOE_Y) << 8) / BJ_DEAL_FRAMES) << 8;
                    } else if (bj_actionGetCursor() == BJ_SPLIT) {
                        if (bj_handOptions(cur) & BJ_CAN_SPLIT) {
                            bj_splitHand(&player, &split);
                            x = BJ_SHOE_X << 16;
                            y = BJ_SHOE_Y << 16;
                            t = BJ_DEAL_FRAMES;
                            dx = (((cur->x + cur->count * BJ_CARD_STEP - BJ_SHOE_X) << 8) / BJ_DEAL_FRAMES) << 8;
                            dy = (((cur->y - BJ_SHOE_Y) << 8) / BJ_DEAL_FRAMES) << 8;
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
                if (cur->value >= BJ_BUST) {
                    g_bjState = BJ_ST_BUST;
                } else if (cur->value == BJ_BLACKJACK) {
                    if (nsplit == 1) {
                        cur = &split;
                        g_bjState = BJ_ST_PLAYER;
                        scene_set(SCENE_BJ_ACTION);
                        x = BJ_SHOE_X << 16;
                        y = BJ_SHOE_Y << 16;
                        t = BJ_DEAL_FRAMES;
                        dx = (((cur->x + cur->count * BJ_CARD_STEP - BJ_SHOE_X) << 8) / BJ_DEAL_FRAMES) << 8;
                        dy = (((cur->y - BJ_SHOE_Y) << 8) / BJ_DEAL_FRAMES) << 8;
                        nsplit = 2;
                    } else {
                        g_bjState = BJ_ST_DEALER;
                    }
                } else if (cur->flags & BJ_HAND_SPLIT_ACES) {
                    if (nsplit == 1) {
                        cur = &split;
                        g_bjState = BJ_ST_PLAYER;
                        scene_set(SCENE_BJ_ACTION);
                        x = BJ_SHOE_X << 16;
                        y = BJ_SHOE_Y << 16;
                        t = BJ_DEAL_FRAMES;
                        dx = (((cur->x + cur->count * BJ_CARD_STEP - BJ_SHOE_X) << 8) / BJ_DEAL_FRAMES) << 8;
                        dy = (((cur->y - BJ_SHOE_Y) << 8) / BJ_DEAL_FRAMES) << 8;
                        nsplit = 2;
                    } else {
                        g_bjState = BJ_ST_DEALER;
                    }
                } else if (cur->flags & BJ_HAND_DOUBLED) {
                    if (nsplit == 1) {
                        cur = &split;
                        g_bjState = BJ_ST_PLAYER;
                        scene_set(SCENE_BJ_ACTION);
                        x = BJ_SHOE_X << 16;
                        y = BJ_SHOE_Y << 16;
                        t = BJ_DEAL_FRAMES;
                        dx = (((cur->x + cur->count * BJ_CARD_STEP - BJ_SHOE_X) << 8) / BJ_DEAL_FRAMES) << 8;
                        dy = (((cur->y - BJ_SHOE_Y) << 8) / BJ_DEAL_FRAMES) << 8;
                        nsplit = 2;
                    } else {
                        g_bjState = BJ_ST_DEALER;
                    }
                } else {
                    g_bjState = BJ_ST_PLAYER;
                    scene_set(SCENE_BJ_ACTION);
                }
            }
            break;
        case BJ_ST_DEALER:
            if (t == -1) {
                if (dealer.value < BJ_DEALER_STAND) {
                    x = BJ_SHOE_X << 16;
                    y = BJ_SHOE_Y << 16;
                    t = BJ_DEAL_FRAMES;
                    dy = 0;
                    dx = (((dealer.count * BJ_CARD_STEP - 0xD2) << 8) / BJ_DEAL_FRAMES) << 8;
                } else if (dealer.flags & BJ_HAND_BUST) {
                    g_bjState = BJ_ST_BUST;
                } else if (nsplit > 0) {
                    nsplit = 1;
                    g_bjState = BJ_ST_SETTLE_SPLIT;
                } else if (player.value == dealer.value) {
                    g_bjState = BJ_ST_PUSH;
                } else if (dealer.value < player.value) {
                    g_bjState = BJ_ST_WIN;
                } else {
                    g_bjState = BJ_ST_LOSE;
                }
            } else if (t > 0) {
                bj_drawCardBack(x >> 16, y >> 16);
                x += dx;
                y += dy;
                t--;
            } else {
                t = -1;
                g_bjState = BJ_ST_DEALER;
                bj_dealCard(&dealer, &g_bjShoe);
            }
            break;
        case BJ_ST_BUST:
            if (cur->value >= BJ_BUST) {
                i = 1;
                if (nsplit == 2) {
                    i = 2;
                }
            } else {
                i = 0;
            }
            if (t == -1) {
                t = BJ_SHAKE_FRAMES;
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
                    g_bjState = BJ_ST_PLAYER;
                    scene_set(SCENE_BJ_ACTION);
                    x = BJ_SHOE_X << 16;
                    y = BJ_SHOE_Y << 16;
                    t = BJ_DEAL_FRAMES;
                    dx = (((cur->x + cur->count * BJ_CARD_STEP - BJ_SHOE_X) << 8) / BJ_DEAL_FRAMES) << 8;
                    dy = (((cur->y - BJ_SHOE_Y) << 8) / BJ_DEAL_FRAMES) << 8;
                    nsplit = 2;
                } else if (nsplit == 2) {
                    nsplit = 1;
                    g_bjState = BJ_ST_SETTLE_SPLIT;
                } else if (cur->value >= BJ_BUST) {
                    g_bjState = BJ_ST_LOSE;
                } else {
                    g_bjState = BJ_ST_WIN;
                }
            }
            break;
        case BJ_ST_SETTLE_SPLIT:
            if (t == -1) {
                if (nsplit == 1) {
                    if (player.value >= BJ_BUST) {
                        res = BJ_RESULT_LOSE;
                        snd_playSe(BJ_SE_LOSE);
                        g_bjBankroll -= (player.flags & BJ_HAND_DOUBLED) ? g_bjWager * 2 : g_bjWager;
                    } else if (dealer.value >= BJ_BUST || dealer.value < player.value) {
                        res = BJ_RESULT_WIN;
                        snd_playSe(BJ_SE_WIN);
                        g_bjBankroll += (player.flags & BJ_HAND_DOUBLED) ? g_bjWager * 2 : g_bjWager;
                    } else {
                        res = BJ_RESULT_PUSH;
                        if (player.value != dealer.value) {
                            res = BJ_RESULT_LOSE;
                            snd_playSe(BJ_SE_LOSE);
                            g_bjBankroll -= (player.flags & BJ_HAND_DOUBLED) ? g_bjWager * 2 : g_bjWager;
                        }
                    }
                } else {
                    if (split.value >= BJ_BUST) {
                        res = BJ_RESULT_LOSE;
                        snd_playSe(BJ_SE_LOSE);
                        g_bjBankroll -= (split.flags & BJ_HAND_DOUBLED) ? g_bjWager * 2 : g_bjWager;
                    } else if (dealer.value >= BJ_BUST || dealer.value < split.value) {
                        res = BJ_RESULT_WIN;
                        snd_playSe(BJ_SE_WIN);
                        g_bjBankroll += (split.flags & BJ_HAND_DOUBLED) ? g_bjWager * 2 : g_bjWager;
                    } else {
                        res = BJ_RESULT_PUSH;
                        if (split.value != dealer.value) {
                            res = BJ_RESULT_LOSE;
                            snd_playSe(BJ_SE_LOSE);
                            g_bjBankroll -= (split.flags & BJ_HAND_DOUBLED) ? g_bjWager * 2 : g_bjWager;
                        }
                    }
                }
                t = BJ_RESULT_FRAMES;
                dealer.flags &= ~BJ_HAND_HOLE;
            } else if (t > 0) {
                s32 a;

                if (t < 0x10) {
                    a = t * 16;
                } else if (t < 0x85) {
                    a = 0xFF;
                } else {
                    a = (BJ_RESULT_FRAMES - t) * 16;
                }
                bj_drawResult(res, a, 0x6C, nsplit == 1 ? player.y + 12 : split.y + 12);
                t--;
            } else {
                t = -1;
                if (nsplit == 1) {
                    nsplit = 2;
                } else {
                    g_bjState = BJ_ST_NEXT;
                }
            }
            break;
        case BJ_ST_BLACKJACK:
            if (t == -1) {
                if (player.value > dealer.value) {
                    snd_playSe(BJ_SE_WIN);
                    g_bjBankroll += g_bjWager + (g_bjWager >> 1);
                } else {
                    snd_playSe(BJ_SE_LOSE);
                    g_bjBankroll -= g_bjWager;
                }
                t = BJ_RESULT_FRAMES;
                dealer.flags &= ~BJ_HAND_HOLE;
            } else if (t > 0) {
                s32 a;

                if (t < 0x10) {
                    a = t * 16;
                } else if (t < 0x85) {
                    a = 0xFF;
                } else {
                    a = (BJ_RESULT_FRAMES - t) * 16;
                }
                if (player.value > dealer.value) {
                    bj_drawResult(BJ_RESULT_WIN, a, 0x6C, player.y + 12);
                } else {
                    bj_drawResult(BJ_RESULT_LOSE, a, 0x6C, player.y + 12);
                }
                t--;
            } else {
                t = -1;
                g_bjState = BJ_ST_NEXT;
            }
            break;
        case BJ_ST_WIN:
            if (t == -1) {
                snd_playSe(BJ_SE_WIN);
                {
                    s32 *pb = &g_bjBankroll;
                    *pb = g_bjBankroll + ((player.flags & BJ_HAND_DOUBLED) ? g_bjWager * 2 : g_bjWager);
                }
                t = BJ_RESULT_FRAMES;
                dealer.flags &= ~BJ_HAND_HOLE;
            } else if (t > 0) {
                s32 a;

                if (t < 0x10) {
                    a = t * 16;
                } else if (t < 0x85) {
                    a = 0xFF;
                } else {
                    a = (BJ_RESULT_FRAMES - t) * 16;
                }
                bj_drawResult(BJ_RESULT_WIN, a, 0x6C, player.y + 12);
                t--;
            } else {
                t = -1;
                g_bjState = BJ_ST_NEXT;
            }
            break;
        case BJ_ST_PUSH:
            if (t == -1) {
                t = BJ_RESULT_FRAMES;
                dealer.flags &= ~BJ_HAND_HOLE;
            } else if (t > 0) {
                s32 a;

                if (t < 0x10) {
                    a = t * 16;
                } else if (t < 0x85) {
                    a = 0xFF;
                } else {
                    a = (BJ_RESULT_FRAMES - t) * 16;
                }
                bj_drawResult(BJ_RESULT_PUSH, a, 0x6C, player.y + 12);
                t--;
            } else {
                t = -1;
                g_bjState = BJ_ST_NEXT;
            }
            break;
        case BJ_ST_LOSE:
            if (t == -1) {
                snd_playSe(BJ_SE_LOSE);
                g_bjBankroll -= (player.flags & BJ_HAND_DOUBLED) ? g_bjWager * 2 : g_bjWager;
                t = BJ_RESULT_FRAMES;
                dealer.flags &= ~BJ_HAND_HOLE;
            } else if (t > 0) {
                s32 a;

                if (t < 0x10) {
                    a = t * 16;
                } else if (t < 0x85) {
                    a = 0xFF;
                } else {
                    a = (BJ_RESULT_FRAMES - t) * 16;
                }
                bj_drawResult(BJ_RESULT_LOSE, a, 0x6C, player.y + 12);
                t--;
            } else {
                t = -1;
                g_bjState = BJ_ST_NEXT;
            }
            break;
        case BJ_ST_NEXT:
            if (g_bjShoe.flags & BJ_SHOE_CUT) {
                t = -1;
                g_bjState = BJ_ST_END;
            } else {
                nsplit = 0;
                if (g_bjWager > g_bjBankroll) {
                    g_bjWager = g_bjBankroll / BJ_MIN_BET * BJ_MIN_BET;
                }
                t = -1;
                g_bjState = BJ_ST_BET;
                scene_set(SCENE_BJ_WAGER);
            }
            break;
        case BJ_ST_END:
            if (t == -1) {
                game_startFade(1, 1, 0x3C, 0xFFFFFF);
                t = 0x3C;
                if (g_bjBankroll < BJ_MIN_BET) {
                    g_bjBankroll = BJ_BANKROLL;
                }
            } else if (t > 0) {
                t--;
            } else {
                t = -1;
                g_bjState = BJ_ST_DONE;
            }
            break;
        }
        if (g_bjState >= BJ_ST_PLAYER) {
            bj_drawHand(&dealer, ofsX[0], ofsY[0], -1);
            if (dealer.count != 0 && g_bjState >= BJ_ST_DEALER) {
                sprintf(buf, g_bjFmt2d, dealer.value);
                ui_drawSmallDigits(10, dealer.y + 0x20, 0x10, buf, 0);
            }
            if (nsplit == 2) {
                bj_drawHand(&split, ofsX[2], ofsY[2], -1);
                if (split.count != 0) {
                    sprintf(buf, g_bjFmt2d, split.value);
                    ui_drawSmallDigits(10, split.y + 0x20, 0x10, buf, 0);
                    if (split.value != BJ_BLACKJACK && (split.flags & BJ_HAND_SOFT)) {
                        sprintf(buf, g_bjFmt2d, split.hardValue);
                        ui_drawSmallDigits(10, split.y + 0x2A, 0x10, buf, 5);
                    }
                }
            }
            bj_drawHand(&player, ofsX[1], ofsY[1], -1);
            if (player.count != 0) {
                sprintf(buf, g_bjFmt2d, player.value);
                ui_drawSmallDigits(10, player.y + 0x20, 0x10, buf, 0);
                if (player.value != BJ_BLACKJACK && (player.flags & BJ_HAND_SOFT)) {
                    sprintf(buf, g_bjFmt2d, player.hardValue);
                    ui_drawSmallDigits(10, player.y + 0x2A, 0x10, buf, 5);
                }
            }
            if (nsplit == 1) {
                bj_drawHand(&split, ofsX[2], ofsY[2], -1);
                if (split.count != 0) {
                    sprintf(buf, g_bjFmt2d, split.value);
                    ui_drawSmallDigits(10, split.y + 0x20, 0x10, buf, 0);
                    if (split.value != BJ_BLACKJACK && (split.flags & BJ_HAND_SOFT)) {
                        sprintf(buf, g_bjFmt2d, split.hardValue);
                        ui_drawSmallDigits(10, split.y + 0x2A, 0x10, buf, 5);
                    }
                }
            }
        }
        bj_drawShoe(BJ_SHOE_X, BJ_SHOE_Y);
        addPrim(g_gameState->gameData->ot[g_gameState->gameData->drawBuf] + BJ_OTZ, &g_bjTpage);
        sprintf(buf, g_bjStrBankroll);
        ui_drawSmallDigits(0x10C - (text_measureSmall(buf) >> 1), 0x58, 0x10, buf, 4);
        sprintf(buf, g_bjFmtLong, g_bjBankroll);
        ui_drawSmallDigits(0x10C - (text_measureSmall(buf) >> 1), 0x64, 0x10, buf, 5);
        sprintf(buf, g_bjStrWager);
        ui_drawSmallDigits(0x10C - (text_measureSmall(buf) >> 1), 0x70, 0x10, buf, 4);
        sprintf(buf, g_bjFmtLong, g_bjWager);
        ui_drawSmallDigits(0x10C - (text_measureSmall(buf) >> 1), 0x7C, 0x10, buf, 0);
        sprintf(buf, g_bjStrStand);
        ui_drawSmallDigits(0x10C - (text_measureSmall(buf) >> 1), 0x90, 0x10, buf, 0);
        sprintf(buf, g_bjStrHit);
        ui_drawSmallDigits(0x10C - (text_measureSmall(buf) >> 1), 0xA0, 0x10, buf, 0);
        sprintf(buf, g_bjStrDouble);
        ui_drawSmallDigits(0x10C - (text_measureSmall(buf) >> 1), 0xB0, 0x10, buf, g_bjCanDouble ? 0 : 2);
        sprintf(buf, g_bjStrSplit);
        ui_drawSmallDigits(0x10C - (text_measureSmall(buf) >> 1), 0xC0, 0x10, buf, g_bjCanSplit ? 0 : 2);
        g_padActuator[0].unk0 = 0;
        if (cnt > 0) {
            cnt--;
        }
        if (g_bjState < BJ_ST_DONE) {
            if (g_bjBankroll < BJ_MIN_BET) {
                t = -1;
                g_bjState = BJ_ST_END;
            }
            if (g_padPortPressed[0] & PAD_START) {
                t = -1;
                g_bjState = BJ_ST_END;
            }
        }
        game_updateFade();
        bj_swapBuffers();
    } while (g_bjState < BJ_ST_DONE);
    bj_exit();
    return 0;
}

s32 bj_init(void) {
    s32 p;

    p = (s32)db_lookupRegistered(4, 0x44F4C, 0);
    file_setTimPos(p, 0, 0xE4, 0x1C0, 0x180);
    file_loadTim(p);
    file_setTimPos(p, 0, 0xE5, 0x1C0, 0x180);
    file_loadTim(p);
    p = (s32)db_lookupRegistered(4, 0x44F4D, 0);
    file_setTimPos(p, 0, 0xE6, 0x1EC, 0x100);
    file_loadTim(p);
    p = (s32)db_lookupRegistered(4, 0x44F4E, 0);
    file_setTimPos(p, 0, 0xE0, 0x1C0, 0x100);
    file_loadTim(p);
    p = (s32)db_lookupRegistered(4, 0x44F50, 0);
    file_setTimPos(p, 0, 0xE1, 0x1D6, 0x100);
    file_loadTim(p);
    p = (s32)db_lookupRegistered(4, 0x44F4F, 0);
    file_setTimPos(p, 0, 0xE2, 0x1C0, 0x140);
    file_loadTim(p);
    p = (s32)db_lookupRegistered(4, 0x44F51, 0);
    file_setTimPos(p, 0, 0xE9, 0x280, 0x100);
    file_loadTim(p);
    setRECT(&g_bjClutRect, 0x20, 0xE4, 0x10, 1);
    LoadImage(&g_bjClutRect, g_bjClut);
    DrawSync(0);
    setlen(&g_bjTpage, 1);
    g_bjTpage.code = 0xE1000697;
    if (!(g_gameState->gameData->flags & 0x20000000)) {
        ending_soundCmd(0, ENDING_MUSIC, 0, 0, 0);
        ending_soundCmd(0xC401, ENDING_MUSIC, 0x5A, 0, 0x7F);
    }
    game_startFade(1, 1, 0, 0xFFFFFF);
    g_bjState = BJ_ST_START;
    g_padPortMask[0] = 0;
    return 0;
}

s32 bj_exit(void) {
    return 0;
}

void bj_clearOt(void) {
    GameData *ctx = g_gameState->gameData;
    ClearOTagR(ctx->ot[ctx->drawBuf], 0x1000);
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
    PutDispEnv(&g_gameState->dispEnv[g_gameState->gameData->drawBuf]);
    ctx = g_gameState->gameData;
    i = ctx->drawBuf;
    DrawOTagEnv(ctx->ot[i] + 0xFFF, &g_gameState->drawEnv[i]);
    g_gameState->gameData->drawBuf ^= 1;
    gfx_setBuffer(g_gameState->gameData->drawBuf);
}
