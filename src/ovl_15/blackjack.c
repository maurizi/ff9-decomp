#include "ovl_15.h"

s32 ending_playMovie(s32 list, s32 entry) {
    u8 *mbgWork;
    s32 timer;
    s32 i;
    s32 done;

    mbgWork = g_loadAreaEnd + 0x106700;
    timer = -1;
    ending_movieInit(list, entry);
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
            MBG_init(1, &mbgWork);
            mbg_setRgb24(1);
            mbg_setEndCallback(ending_movieOnEnd);
            mbg_play(list, entry);
            g_movieState = MOVIE_ST_PLAY;
            break;
        case MOVIE_ST_PLAY:
            if ((g_movieFlags & MOVIE_PLAYING) && (mbg_getStatus() & MBG_STATUS_PLAYING)) {
                done = mbg_update(0);
                mbg_getLastFrame();
                if (done != 0) {
                    DrawOTag(g_gameState->gameData->ot[g_gameState->gameData->drawBuf ^ 1] + OT_LAST);
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

s32 ending_movieInit(s32 list, s32 entry) {
    FileEntry *file;
    s32 h;
    s32 i;
    s32 j;

    while (cdRead_update(g_gameState->gameData->cdRead) != 0) {
    }
    file = file_find(DIR_OVERLAYS, OVL_MOVIE);
    cdRead_init(g_gameState->gameData->cdRead, file->sector, (file->link - file->sector) << 11, g_loadAreaEnd + 0xF1700, 0, 0x15000, 0);
    while (cdRead_update(g_gameState->gameData->cdRead) != 0) {
    }
    EnterCriticalSection();
    FlushCache();
    ExitCriticalSection();
    if (g_gameState->disc != DISC_NONE && (g_gameState->disc & DISC_4)) {
        h = cd_findFile(g_mbgMovieLists[list].movies[entry].path);
        cd_seekStreamAsync(h);
        while (cd_getState() != 0) {
        }
        mbg_streamSetBaseSector(h);
    }
    game_startFade(FADE_FLAT, FADE_SUB, 0x78, FADE_COLOR_FULL);
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
    ClearOTagR(ctx->ot[ctx->drawBuf], OT_LEN);
}

void ending_movieSwap(void) {
    GameData *ctx;
    u8 k;
    s32 i;
    s32 j;

    DrawSync(0);
    if (!(mbg_getStatus() & MBG_STATUS_PLAYING)) {
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
        DrawOTagEnv(ctx->ot[k] + OT_LAST, &g_gameState->drawEnv[k]);
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

s32 bj_drawResult(s32 kind, s32 brightness, s32 x, s32 y) {
    SPRT *sprt;
    DR_TPAGE *tpage;
    s32 x1;
    s32 x2;

    switch (kind) {
    case BJ_RESULT_WIN:
        sprt = &g_bjSprites[g_gameState->gameData->drawBuf][g_bjSprtCount];
        setlen(sprt, PRIM_LEN(SPRT));
        sprt->code = GPU_SPRT | GPU_ABE;
        sprt->x0 = x;
        sprt->y0 = y;
        sprt->u0 = 0;
        sprt->v0 = 0;
        setRGB0(sprt, brightness, brightness, brightness);
        sprt->w = 0x68;
        sprt->h = BJ_RESULT_H;
        sprt->clut = getClut(0, BJ_CLUT_RESULT_Y);
        addPrim(&g_gameState->gameData->ot[g_gameState->gameData->drawBuf][BJ_OTZ], sprt);
        g_bjSprtCount++;
        tpage = &g_bjTpages[g_gameState->gameData->drawBuf][g_bjTpageCount];
        setDrawTPage(tpage, 1, 0, GetTPage(1, 1, BJ_VRAM_X, BJ_VRAM_Y));
        addPrim(&g_gameState->gameData->ot[g_gameState->gameData->drawBuf][BJ_OTZ], tpage);
        g_bjTpageCount++;
        sprt = &g_bjSprites[g_gameState->gameData->drawBuf][g_bjSprtCount];
        setlen(sprt, PRIM_LEN(SPRT));
        sprt->code = GPU_SPRT | GPU_ABE;
        sprt->x0 = x;
        sprt->y0 = y;
        setRGB0(sprt, brightness, brightness, brightness);
        sprt->u0 = 0x68;
        sprt->v0 = 0;
        sprt->w = 0x70;
        sprt->h = BJ_RESULT_H;
        sprt->clut = getClut(0, BJ_CLUT_RESULT_Y);
        addPrim(&g_gameState->gameData->ot[g_gameState->gameData->drawBuf][BJ_OTZ], sprt);
        g_bjSprtCount++;
        tpage = &g_bjTpages[g_gameState->gameData->drawBuf][g_bjTpageCount];
        setDrawTPage(tpage, 1, 0, GetTPage(1, 2, BJ_VRAM_X, BJ_VRAM_Y));
        addPrim(&g_gameState->gameData->ot[g_gameState->gameData->drawBuf][BJ_OTZ], tpage);
        g_bjTpageCount++;
        break;
    case BJ_RESULT_LOSE:
        x1 = x - 2;
        sprt = &g_bjSprites[g_gameState->gameData->drawBuf][g_bjSprtCount];
        setlen(sprt, PRIM_LEN(SPRT));
        sprt->code = GPU_SPRT | GPU_ABE;
        sprt->x0 = x1;
        sprt->y0 = y;
        setRGB0(sprt, brightness, brightness, brightness);
        sprt->u0 = 0;
        sprt->w = 0x68;
        sprt->h = BJ_RESULT_H;
        sprt->v0 = 0x28;
        sprt->clut = getClut(0, BJ_CLUT_RESULT_Y);
        addPrim(&g_gameState->gameData->ot[g_gameState->gameData->drawBuf][BJ_OTZ], sprt);
        g_bjSprtCount++;
        tpage = &g_bjTpages[g_gameState->gameData->drawBuf][g_bjTpageCount];
        setDrawTPage(tpage, 1, 0, GetTPage(1, 1, BJ_VRAM_X, BJ_VRAM_Y));
        addPrim(&g_gameState->gameData->ot[g_gameState->gameData->drawBuf][BJ_OTZ], tpage);
        g_bjTpageCount++;
        sprt = &g_bjSprites[g_gameState->gameData->drawBuf][g_bjSprtCount];
        setlen(sprt, PRIM_LEN(SPRT));
        sprt->code = GPU_SPRT | GPU_ABE;
        sprt->x0 = x1;
        sprt->y0 = y;
        sprt->w = 0x70;
        setRGB0(sprt, brightness, brightness, brightness);
        sprt->u0 = 0x68;
        sprt->v0 = 0x28;
        sprt->h = BJ_RESULT_H;
        sprt->clut = getClut(0, BJ_CLUT_RESULT_Y);
        addPrim(&g_gameState->gameData->ot[g_gameState->gameData->drawBuf][BJ_OTZ], sprt);
        g_bjSprtCount++;
        tpage = &g_bjTpages[g_gameState->gameData->drawBuf][g_bjTpageCount];
        setDrawTPage(tpage, 1, 0, GetTPage(1, 2, BJ_VRAM_X, BJ_VRAM_Y));
        addPrim(&g_gameState->gameData->ot[g_gameState->gameData->drawBuf][BJ_OTZ], tpage);
        g_bjTpageCount++;
        break;
    case BJ_RESULT_PUSH:
        x2 = x - 0x10;
        sprt = &g_bjSprites[g_gameState->gameData->drawBuf][g_bjSprtCount];
        setlen(sprt, PRIM_LEN(SPRT));
        sprt->code = GPU_SPRT | GPU_ABE;
        sprt->x0 = x2;
        sprt->y0 = y;
        sprt->w = 0x88;
        setRGB0(sprt, brightness, brightness, brightness);
        sprt->u0 = 0;
        sprt->v0 = 0xA0;
        sprt->h = BJ_RESULT_H;
        sprt->clut = getClut(0, BJ_CLUT_RESULT_Y);
        addPrim(&g_gameState->gameData->ot[g_gameState->gameData->drawBuf][BJ_OTZ], sprt);
        g_bjSprtCount++;
        tpage = &g_bjTpages[g_gameState->gameData->drawBuf][g_bjTpageCount];
        setDrawTPage(tpage, 1, 0, GetTPage(1, 1, BJ_VRAM_X, BJ_VRAM_Y));
        addPrim(&g_gameState->gameData->ot[g_gameState->gameData->drawBuf][BJ_OTZ], tpage);
        g_bjTpageCount++;
        sprt = &g_bjSprites[g_gameState->gameData->drawBuf][g_bjSprtCount];
        setlen(sprt, PRIM_LEN(SPRT));
        sprt->code = GPU_SPRT | GPU_ABE;
        sprt->x0 = x2;
        sprt->y0 = y;
        setRGB0(sprt, brightness, brightness, brightness);
        sprt->v0 = 0xC8;
        sprt->u0 = 0;
        sprt->w = 0x90;
        sprt->h = BJ_RESULT_H;
        sprt->clut = getClut(0, BJ_CLUT_RESULT_Y);
        addPrim(&g_gameState->gameData->ot[g_gameState->gameData->drawBuf][BJ_OTZ], sprt);
        g_bjSprtCount++;
        tpage = &g_bjTpages[g_gameState->gameData->drawBuf][g_bjTpageCount];
        setDrawTPage(tpage, 1, 0, GetTPage(1, 2, BJ_VRAM_X, BJ_VRAM_Y));
        addPrim(&g_gameState->gameData->ot[g_gameState->gameData->drawBuf][BJ_OTZ], tpage);
        g_bjTpageCount++;
        break;
    }
    return 0;
}

s32 bj_drawCardBack(s32 x, s32 y) {
    SPRT *sprt = &g_bjSprites[g_gameState->gameData->drawBuf][g_bjSprtCount];

    setSprt(sprt);
    setRGB0(sprt, 0x80, 0x80, 0x80);
    sprt->x0 = x;
    sprt->u0 = 0;
    sprt->v0 = BJ_CARD_V;
    sprt->w = BJ_CARD_W;
    sprt->h = BJ_CARD_H;
    sprt->y0 = y;
    sprt->clut = getClut(0, BJ_CLUT_BACK_Y);
    addPrim(&g_gameState->gameData->ot[g_gameState->gameData->drawBuf][BJ_OTZ], sprt);
    g_bjSprtCount++;
    return 0;
}

s32 bj_drawCard(s32 rank, s32 suit, s32 x, s32 y) {
    SPRT *sprt;
    s32 i;
    s32 face;
    s32 px;
    s32 py;

    sprt = &g_bjSprites[g_gameState->gameData->drawBuf][g_bjSprtCount];
    setSprt(sprt);
    setRGB0(sprt, 0x80, 0x80, 0x80);
    sprt->x0 = x + BJ_RANK_X;
    sprt->y0 = y + BJ_RANK_Y;
    sprt->u0 = (suit & 1) ? 0x58 : 0x6C;
    sprt->v0 = rank * BJ_RANK_H;
    sprt->w = BJ_RANK_W;
    sprt->h = BJ_RANK_H;
    sprt->clut = getClut(0, BJ_CLUT_RANK_Y);
    addPrim(&g_gameState->gameData->ot[g_gameState->gameData->drawBuf][BJ_OTZ], sprt);
    g_bjSprtCount++;

    sprt = &g_bjSprites[g_gameState->gameData->drawBuf][g_bjSprtCount];
    setSprt(sprt);
    setRGB0(sprt, 0x80, 0x80, 0x80);
    sprt->x0 = x + BJ_RANK2_X;
    sprt->y0 = y + BJ_RANK2_Y;
    sprt->u0 = (suit & 1) ? 0x62 : 0x76;
    sprt->v0 = rank * BJ_RANK_H;
    sprt->w = BJ_RANK_W;
    sprt->h = BJ_RANK_H;
    sprt->clut = getClut(0, BJ_CLUT_RANK_Y);
    addPrim(&g_gameState->gameData->ot[g_gameState->gameData->drawBuf][BJ_OTZ], sprt);
    g_bjSprtCount++;

    sprt = &g_bjSprites[g_gameState->gameData->drawBuf][g_bjSprtCount];
    sprt->x0 = x + BJ_SUIT_X;
    sprt->y0 = y + BJ_SUIT_Y;
    setSprt(sprt);
    setRGB0(sprt, 0x80, 0x80, 0x80);
    sprt->u0 = suit * BJ_PIP_U_STEP;
    sprt->v0 = BJ_PIP_V;
    sprt->w = BJ_PIP_SIZE;
    sprt->h = BJ_PIP_SIZE;
    sprt->clut = getClut(0, BJ_CLUT_FACE_Y);
    addPrim(&g_gameState->gameData->ot[g_gameState->gameData->drawBuf][BJ_OTZ], sprt);
    g_bjSprtCount++;

    sprt = &g_bjSprites[g_gameState->gameData->drawBuf][g_bjSprtCount];
    sprt->x0 = x + BJ_SUIT2_X;
    setSprt(sprt);
    setRGB0(sprt, 0x80, 0x80, 0x80);
    sprt->u0 = suit * BJ_PIP_U_STEP;
    sprt->v0 = BJ_PIP_V_FLIPPED;
    sprt->y0 = y + BJ_SUIT2_Y;
    sprt->w = BJ_PIP_SIZE;
    sprt->h = BJ_PIP_SIZE;
    sprt->clut = getClut(0, BJ_CLUT_FACE_Y);
    addPrim(&g_gameState->gameData->ot[g_gameState->gameData->drawBuf][BJ_OTZ], sprt);
    g_bjSprtCount++;

    if (rank >= BJ_RANK_JACK) {
        face = rank - BJ_RANK_JACK;
        if (rank == BJ_RANK_ACE) {
            sprt = &g_bjSprites[g_gameState->gameData->drawBuf][g_bjSprtCount];
            setSprt(sprt);
            setRGB0(sprt, 0x80, 0x80, 0x80);
            sprt->u0 = suit * BJ_PIP_U_STEP;
            sprt->v0 = BJ_PIP_V;
            sprt->x0 = x + BJ_ACE_PIP_X;
            sprt->y0 = y + BJ_ACE_PIP_Y;
            sprt->w = BJ_PIP_SIZE;
            sprt->h = BJ_PIP_SIZE;
            sprt->clut = getClut(0, BJ_CLUT_FACE_Y);
            addPrim(&g_gameState->gameData->ot[g_gameState->gameData->drawBuf][BJ_OTZ], sprt);
            g_bjSprtCount++;
        } else {
            sprt = &g_bjSprites[g_gameState->gameData->drawBuf][g_bjSprtCount];
            sprt->x0 = x + BJ_PICTURE_X;
            sprt->y0 = y + BJ_PICTURE_Y;
            setSprt(sprt);
            setRGB0(sprt, 0x80, 0x80, 0x80);
            if (face & 1) {
                sprt->u0 = 0x2C;
            } else {
                sprt->u0 = 0;
            }
            if (face >> 1) {
                sprt->v0 = 0x40;
            } else {
                sprt->v0 = 0;
            }
            sprt->w = BJ_PICTURE_W;
            sprt->h = BJ_PICTURE_H;
            sprt->clut = (face + BJ_CLUT_PICTURE_Y) << 6;
            addPrim(&g_gameState->gameData->ot[g_gameState->gameData->drawBuf][BJ_OTZ], sprt);
            g_bjSprtCount++;
        }
    } else {
        for (i = 0; i <= rank + 1; i++) {
            px = g_bjPips[rank][i].x;
            py = g_bjPips[rank][i].y;
            sprt = &g_bjSprites[g_gameState->gameData->drawBuf][g_bjSprtCount];
            setSprt(sprt);
            setRGB0(sprt, 0x80, 0x80, 0x80);
            sprt->u0 = suit * BJ_PIP_U_STEP;
            sprt->x0 = x + px;
            sprt->y0 = y + py;
            if (py >= BJ_PIP_FLIP_Y) {
                sprt->v0 = BJ_PIP_V_FLIPPED;
            } else {
                sprt->v0 = BJ_PIP_V;
            }
            sprt->w = BJ_PIP_SIZE;
            sprt->h = BJ_PIP_SIZE;
            sprt->clut = getClut(0, BJ_CLUT_FACE_Y);
            addPrim(&g_gameState->gameData->ot[g_gameState->gameData->drawBuf][BJ_OTZ], sprt);
            g_bjSprtCount++;
        }
    }

    sprt = &g_bjSprites[g_gameState->gameData->drawBuf][g_bjSprtCount];
    setSprt(sprt);
    setRGB0(sprt, 0x80, 0x80, 0x80);
    sprt->u0 = 0;
    sprt->v0 = BJ_CARD_V;
    sprt->w = BJ_CARD_W;
    sprt->h = BJ_CARD_H;
    sprt->x0 = x;
    sprt->y0 = y;
    sprt->clut = getClut(0, BJ_CLUT_FACE_Y);
    addPrim(&g_gameState->gameData->ot[g_gameState->gameData->drawBuf][BJ_OTZ], sprt);
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
    s32 remain;
    s32 i;
    s32 j;
    u8 card;

    for (pass = 0; pass < BJ_SHUFFLE_PASSES; pass++) {
        for (i = 0, remain = BJ_SHOE_SIZE; remain > 0; remain--, i++) {
            j = (rand8_inline() * 2) % remain;
            card = shoe->cards[i];
            shoe->cards[i] = shoe->cards[i + j];
            shoe->cards[i + j] = card;
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

s32 bj_splitHand(BjHand *hand, BjHand *split) {
    s32 rank;
    u16 value;

    hand->count = 1;
    hand->x = BJ_HAND_X;
    hand->y = BJ_SPLIT1_Y;
    bj_handInit(split, BJ_HAND_X, BJ_SPLIT2_Y);
    split->count = 1;
    split->cards[0] = hand->cards[1];
    rank = (u32)hand->cards[1] >> 2;
    if (rank < BJ_RANK_JACK) {
        value = rank + 2;
    } else if (rank == BJ_RANK_ACE) {
        hand->value = BJ_ACE_VALUE;
        hand->hardValue = BJ_ACE_LOW;
        hand->flags |= BJ_HAND_SPLIT_ACES | BJ_HAND_SOFT;
        split->value = BJ_ACE_VALUE;
        split->hardValue = BJ_ACE_LOW;
        split->flags |= BJ_HAND_SPLIT_ACES | BJ_HAND_SOFT;
        goto end;
    } else {
        value = BJ_TEN_VALUE;
    }
    hand->value = value;
    hand->hardValue = value;
    split->value = value;
    split->hardValue = value;
end:
    return 0;
}

s32 bj_handOptions(BjHand *hand) {
    s32 options = 0;

    if (hand->count == 2) {
        options = BJ_CAN_DOUBLE;
        if ((u32)hand->cards[0] >> 2 == (u32)hand->cards[1] >> 2) {
            options = BJ_CAN_DOUBLE | BJ_CAN_SPLIT;
        }
    }
    return options;
}

s32 bj_drawHand(BjHand *hand, s32 dx, s32 dy, s32 count) {
    s32 first;
    s32 last;
    s32 i;
    s32 x;
    s32 y;
    s32 card;

    if (count == 0) {
        return 1;
    }
    last = count - 1;
    if (count < 0) {
        last = hand->count - 1;
        if (hand->count == 0) {
            return 1;
        }
    }
    first = 1;
    x = hand->x + dx + last * BJ_CARD_STEP;
    y = hand->y + dy;
    for (i = last; i >= 0; i--) {
        card = hand->cards[i];
        if (first && (hand->flags & BJ_HAND_HOLE) && last == 1) {
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
    char text[16];
    s32 ofsX[3];
    s32 ofsY[3];
    BjHand dealer;
    BjHand player;
    BjHand split;
    s32 timer;
    BjHand *cur;
    s32 nsplit;
    s32 cnt;
    s32 i;
    s32 result;
    s32 x;
    s32 y;
    s32 dx;
    s32 dy;
    BjHand *p1;
    s32 options;
    s32 shown;
    s32 card;

    timer = -1;
    cnt = 0;
    bj_init();
    g_bjWager = BJ_MIN_BET;
    bj_handInit(&player, BJ_HAND_X, BJ_PLAYER_Y);
    bj_handInit(&dealer, BJ_HAND_X, BJ_DEALER_Y);
    result = BJ_RESULT_WIN;
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
            timer = -1;
            g_bjState = BJ_ST_SHUFFLE;
            break;
        case BJ_ST_SHUFFLE:
            if (timer == -1) {
                timer = 0x4E;
                bj_shoeInit(&g_bjShoe);
                bj_shoeShuffle(&g_bjShoe);
                game_startFade(FADE_FLAT, FADE_SUB, 0x3C, FADE_COLOR_NONE);
                i = 0;
            } else if (timer != 0) {
                card = g_bjShoe.cards[i];
                bj_drawCard(card >> 2, card & 3, BJ_SHOE_X, BJ_SHOE_Y);
                i += 4;
                timer--;
            } else {
                timer = -1;
                g_bjState = BJ_ST_BET;
                scene_set(SCENE_BJ_WAGER);
            }
            break;
        case BJ_ST_BET:
            bj_handInit(&player, BJ_HAND_X, BJ_PLAYER_Y);
            bj_handInit(&dealer, BJ_HAND_X, BJ_DEALER_Y);
            scene_update();
            if (scene_get() == SCENE_CHOSEN) {
                timer = -1;
                g_bjState = BJ_ST_DEAL;
            }
            break;
        case BJ_ST_DEAL:
            if (timer == -1) {
                dealer.flags |= BJ_HAND_HOLE;
                bj_dealCard(p1, &g_bjShoe);
                bj_dealCard(&dealer, &g_bjShoe);
                bj_dealCard(p1, &g_bjShoe);
                bj_dealCard(&dealer, &g_bjShoe);
                cur = p1;
                i = 0;
                x = BJ_SHOE_X << 16;
                y = BJ_SHOE_Y << 16;
                timer = 4 * BJ_DEAL_FRAMES; /* the four opening cards fly one after the other */
                dx = (((player.x - BJ_SHOE_X) << 8) / BJ_DEAL_FRAMES) << 8;
                dy = (((player.y - BJ_SHOE_Y) << 8) / BJ_DEAL_FRAMES) << 8;
            } else if (timer > 0) {
                bj_drawCardBack(x >> 16, y >> 16);
                bj_drawHand(&dealer, 0, 0, i >= 2);
                if (i > 0) {
                    shown = 2;
                    if (i < 3) {
                        shown = 1;
                    }
                } else {
                    shown = 0;
                }
                bj_drawHand(p1, 0, 0, shown);
                x += dx;
                y += dy;
                if ((timer - 1) % BJ_DEAL_FRAMES == 0 && ++i < 4) {
                    x = BJ_SHOE_X << 16;
                    y = BJ_SHOE_Y << 16;
                    switch (i) {
                    case 1:
                        dx = (((dealer.x - BJ_SHOE_X) << 8) / BJ_DEAL_FRAMES) << 8;
                        dy = (((dealer.y - BJ_SHOE_Y) << 8) / BJ_DEAL_FRAMES) << 8;
                        break;
                    case 2:
                        dx = (((player.x + BJ_CARD_STEP - BJ_SHOE_X) << 8) / BJ_DEAL_FRAMES) << 8;
                        dy = (((player.y - BJ_SHOE_Y) << 8) / BJ_DEAL_FRAMES) << 8;
                        break;
                    case 3:
                        dx = (((dealer.x + BJ_CARD_STEP - BJ_SHOE_X) << 8) / BJ_DEAL_FRAMES) << 8;
                        dy = (((dealer.y - BJ_SHOE_Y) << 8) / BJ_DEAL_FRAMES) << 8;
                        break;
                    }
                }
                timer--;
            } else if (bj_isBlackjack(p1)) {
                if (bj_isBlackjack(&dealer)) {
                    timer = -1;
                    g_bjState = BJ_ST_PUSH;
                } else {
                    timer = -1;
                    g_bjState = BJ_ST_BLACKJACK;
                }
            } else if (bj_isBlackjack(&dealer)) {
                timer = -1;
                if (bj_isBlackjack(p1)) {
                    g_bjState = BJ_ST_PUSH;
                } else {
                    g_bjState = BJ_ST_BLACKJACK;
                }
            } else {
                timer = -1;
                g_bjState = BJ_ST_PLAYER;
                scene_set(SCENE_BJ_ACTION);
            }
            break;
        case BJ_ST_PLAYER:
            options = bj_handOptions(cur);
            g_bjCanDouble = 0;
            g_bjCanSplit = 0;
            if ((options & BJ_CAN_DOUBLE) && g_bjBankroll >= g_bjWager * 2) {
                g_bjCanDouble = 1;
            }
            if ((options & BJ_CAN_SPLIT) && nsplit == 0 && g_bjBankroll >= g_bjWager * 2) {
                g_bjCanSplit = 1;
            }
            if (timer == -1) {
                scene_update();
                if (scene_get() == SCENE_CHOSEN) {
                    if (bj_actionGetCursor() == BJ_STAND) {
                        if (nsplit == 1) {
                            cur = &split;
                            g_bjState = BJ_ST_PLAYER;
                            scene_set(SCENE_BJ_ACTION);
                            x = BJ_SHOE_X << 16;
                            y = BJ_SHOE_Y << 16;
                            timer = BJ_DEAL_FRAMES;
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
                        timer = BJ_DEAL_FRAMES;
                        dx = (((cur->x + cur->count * BJ_CARD_STEP - BJ_SHOE_X) << 8) / BJ_DEAL_FRAMES) << 8;
                        dy = (((cur->y - BJ_SHOE_Y) << 8) / BJ_DEAL_FRAMES) << 8;
                    } else if (bj_actionGetCursor() == BJ_DOUBLE) {
                        x = BJ_SHOE_X << 16;
                        y = BJ_SHOE_Y << 16;
                        timer = BJ_DEAL_FRAMES;
                        cur->flags |= BJ_HAND_DOUBLED;
                        dx = (((cur->x + cur->count * BJ_CARD_STEP - BJ_SHOE_X) << 8) / BJ_DEAL_FRAMES) << 8;
                        dy = (((cur->y - BJ_SHOE_Y) << 8) / BJ_DEAL_FRAMES) << 8;
                    } else if (bj_actionGetCursor() == BJ_SPLIT) {
                        if (bj_handOptions(cur) & BJ_CAN_SPLIT) {
                            bj_splitHand(&player, &split);
                            x = BJ_SHOE_X << 16;
                            y = BJ_SHOE_Y << 16;
                            timer = BJ_DEAL_FRAMES;
                            dx = (((cur->x + cur->count * BJ_CARD_STEP - BJ_SHOE_X) << 8) / BJ_DEAL_FRAMES) << 8;
                            dy = (((cur->y - BJ_SHOE_Y) << 8) / BJ_DEAL_FRAMES) << 8;
                            nsplit = 1;
                        }
                    }
                }
            } else if (timer > 0) {
                bj_drawCardBack(x >> 16, y >> 16);
                x += dx;
                y += dy;
                timer--;
            } else {
                bj_dealCard(cur, &g_bjShoe);
                timer = -1;
                if (cur->value >= BJ_BUST) {
                    g_bjState = BJ_ST_BUST;
                } else if (cur->value == BJ_BLACKJACK) {
                    if (nsplit == 1) {
                        cur = &split;
                        g_bjState = BJ_ST_PLAYER;
                        scene_set(SCENE_BJ_ACTION);
                        x = BJ_SHOE_X << 16;
                        y = BJ_SHOE_Y << 16;
                        timer = BJ_DEAL_FRAMES;
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
                        timer = BJ_DEAL_FRAMES;
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
                        timer = BJ_DEAL_FRAMES;
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
            if (timer == -1) {
                if (dealer.value < BJ_DEALER_STAND) {
                    x = BJ_SHOE_X << 16;
                    y = BJ_SHOE_Y << 16;
                    timer = BJ_DEAL_FRAMES;
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
            } else if (timer > 0) {
                bj_drawCardBack(x >> 16, y >> 16);
                x += dx;
                y += dy;
                timer--;
            } else {
                timer = -1;
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
            if (timer == -1) {
                timer = BJ_SHAKE_FRAMES;
                ofsX[i] = RND3();
                ofsY[i] = RND3();
            } else if (timer > 0) {
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
                timer--;
            } else {
                timer = -1;
                ofsX[0] = ofsX[1] = ofsX[2] = 0;
                ofsY[0] = ofsY[1] = ofsY[2] = 0;
                if (nsplit == 1) {
                    cur = &split;
                    g_bjState = BJ_ST_PLAYER;
                    scene_set(SCENE_BJ_ACTION);
                    x = BJ_SHOE_X << 16;
                    y = BJ_SHOE_Y << 16;
                    timer = BJ_DEAL_FRAMES;
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
            if (timer == -1) {
                if (nsplit == 1) {
                    if (player.value >= BJ_BUST) {
                        result = BJ_RESULT_LOSE;
                        snd_playSe(BJ_SE_LOSE);
                        g_bjBankroll -= (player.flags & BJ_HAND_DOUBLED) ? g_bjWager * 2 : g_bjWager;
                    } else if (dealer.value >= BJ_BUST || dealer.value < player.value) {
                        result = BJ_RESULT_WIN;
                        snd_playSe(BJ_SE_WIN);
                        g_bjBankroll += (player.flags & BJ_HAND_DOUBLED) ? g_bjWager * 2 : g_bjWager;
                    } else {
                        result = BJ_RESULT_PUSH;
                        if (player.value != dealer.value) {
                            result = BJ_RESULT_LOSE;
                            snd_playSe(BJ_SE_LOSE);
                            g_bjBankroll -= (player.flags & BJ_HAND_DOUBLED) ? g_bjWager * 2 : g_bjWager;
                        }
                    }
                } else {
                    if (split.value >= BJ_BUST) {
                        result = BJ_RESULT_LOSE;
                        snd_playSe(BJ_SE_LOSE);
                        g_bjBankroll -= (split.flags & BJ_HAND_DOUBLED) ? g_bjWager * 2 : g_bjWager;
                    } else if (dealer.value >= BJ_BUST || dealer.value < split.value) {
                        result = BJ_RESULT_WIN;
                        snd_playSe(BJ_SE_WIN);
                        g_bjBankroll += (split.flags & BJ_HAND_DOUBLED) ? g_bjWager * 2 : g_bjWager;
                    } else {
                        result = BJ_RESULT_PUSH;
                        if (split.value != dealer.value) {
                            result = BJ_RESULT_LOSE;
                            snd_playSe(BJ_SE_LOSE);
                            g_bjBankroll -= (split.flags & BJ_HAND_DOUBLED) ? g_bjWager * 2 : g_bjWager;
                        }
                    }
                }
                timer = BJ_RESULT_FRAMES;
                dealer.flags &= ~BJ_HAND_HOLE;
            } else if (timer > 0) {
                s32 bright;

                if (timer < 0x10) {
                    bright = timer * 16;
                } else if (timer < 0x85) {
                    bright = 0xFF;
                } else {
                    bright = (BJ_RESULT_FRAMES - timer) * 16;
                }
                bj_drawResult(result, bright, BJ_RESULT_X, nsplit == 1 ? player.y + BJ_RESULT_DY : split.y + BJ_RESULT_DY);
                timer--;
            } else {
                timer = -1;
                if (nsplit == 1) {
                    nsplit = 2;
                } else {
                    g_bjState = BJ_ST_NEXT;
                }
            }
            break;
        case BJ_ST_BLACKJACK:
            if (timer == -1) {
                if (player.value > dealer.value) {
                    snd_playSe(BJ_SE_WIN);
                    g_bjBankroll += g_bjWager + (g_bjWager >> 1);
                } else {
                    snd_playSe(BJ_SE_LOSE);
                    g_bjBankroll -= g_bjWager;
                }
                timer = BJ_RESULT_FRAMES;
                dealer.flags &= ~BJ_HAND_HOLE;
            } else if (timer > 0) {
                s32 bright;

                if (timer < 0x10) {
                    bright = timer * 16;
                } else if (timer < 0x85) {
                    bright = 0xFF;
                } else {
                    bright = (BJ_RESULT_FRAMES - timer) * 16;
                }
                if (player.value > dealer.value) {
                    bj_drawResult(BJ_RESULT_WIN, bright, BJ_RESULT_X, player.y + BJ_RESULT_DY);
                } else {
                    bj_drawResult(BJ_RESULT_LOSE, bright, BJ_RESULT_X, player.y + BJ_RESULT_DY);
                }
                timer--;
            } else {
                timer = -1;
                g_bjState = BJ_ST_NEXT;
            }
            break;
        case BJ_ST_WIN:
            if (timer == -1) {
                snd_playSe(BJ_SE_WIN);
                {
                    s32 *pb = &g_bjBankroll;
                    *pb = g_bjBankroll + ((player.flags & BJ_HAND_DOUBLED) ? g_bjWager * 2 : g_bjWager);
                }
                timer = BJ_RESULT_FRAMES;
                dealer.flags &= ~BJ_HAND_HOLE;
            } else if (timer > 0) {
                s32 bright;

                if (timer < 0x10) {
                    bright = timer * 16;
                } else if (timer < 0x85) {
                    bright = 0xFF;
                } else {
                    bright = (BJ_RESULT_FRAMES - timer) * 16;
                }
                bj_drawResult(BJ_RESULT_WIN, bright, BJ_RESULT_X, player.y + BJ_RESULT_DY);
                timer--;
            } else {
                timer = -1;
                g_bjState = BJ_ST_NEXT;
            }
            break;
        case BJ_ST_PUSH:
            if (timer == -1) {
                timer = BJ_RESULT_FRAMES;
                dealer.flags &= ~BJ_HAND_HOLE;
            } else if (timer > 0) {
                s32 bright;

                if (timer < 0x10) {
                    bright = timer * 16;
                } else if (timer < 0x85) {
                    bright = 0xFF;
                } else {
                    bright = (BJ_RESULT_FRAMES - timer) * 16;
                }
                bj_drawResult(BJ_RESULT_PUSH, bright, BJ_RESULT_X, player.y + BJ_RESULT_DY);
                timer--;
            } else {
                timer = -1;
                g_bjState = BJ_ST_NEXT;
            }
            break;
        case BJ_ST_LOSE:
            if (timer == -1) {
                snd_playSe(BJ_SE_LOSE);
                g_bjBankroll -= (player.flags & BJ_HAND_DOUBLED) ? g_bjWager * 2 : g_bjWager;
                timer = BJ_RESULT_FRAMES;
                dealer.flags &= ~BJ_HAND_HOLE;
            } else if (timer > 0) {
                s32 bright;

                if (timer < 0x10) {
                    bright = timer * 16;
                } else if (timer < 0x85) {
                    bright = 0xFF;
                } else {
                    bright = (BJ_RESULT_FRAMES - timer) * 16;
                }
                bj_drawResult(BJ_RESULT_LOSE, bright, BJ_RESULT_X, player.y + BJ_RESULT_DY);
                timer--;
            } else {
                timer = -1;
                g_bjState = BJ_ST_NEXT;
            }
            break;
        case BJ_ST_NEXT:
            if (g_bjShoe.flags & BJ_SHOE_CUT) {
                timer = -1;
                g_bjState = BJ_ST_END;
            } else {
                nsplit = 0;
                if (g_bjWager > g_bjBankroll) {
                    g_bjWager = g_bjBankroll / BJ_MIN_BET * BJ_MIN_BET;
                }
                timer = -1;
                g_bjState = BJ_ST_BET;
                scene_set(SCENE_BJ_WAGER);
            }
            break;
        case BJ_ST_END:
            if (timer == -1) {
                game_startFade(FADE_FLAT, FADE_SUB, 0x3C, FADE_COLOR_FULL);
                timer = 0x3C;
                if (g_bjBankroll < BJ_MIN_BET) {
                    g_bjBankroll = BJ_BANKROLL;
                }
            } else if (timer > 0) {
                timer--;
            } else {
                timer = -1;
                g_bjState = BJ_ST_DONE;
            }
            break;
        }
        if (g_bjState >= BJ_ST_PLAYER) {
            bj_drawHand(&dealer, ofsX[0], ofsY[0], -1);
            if (dealer.count != 0 && g_bjState >= BJ_ST_DEALER) {
                sprintf(text, g_bjFmt2d, dealer.value);
                ui_drawSmallDigits(BJ_VALUE_X, dealer.y + BJ_VALUE_DY, BJ_VALUE_OTZ, text, TEXT_PAL_WHITE);
            }
            if (nsplit == 2) {
                bj_drawHand(&split, ofsX[2], ofsY[2], -1);
                if (split.count != 0) {
                    sprintf(text, g_bjFmt2d, split.value);
                    ui_drawSmallDigits(BJ_VALUE_X, split.y + BJ_VALUE_DY, BJ_VALUE_OTZ, text, TEXT_PAL_WHITE);
                    if (split.value != BJ_BLACKJACK && (split.flags & BJ_HAND_SOFT)) {
                        sprintf(text, g_bjFmt2d, split.hardValue);
                        ui_drawSmallDigits(BJ_VALUE_X, split.y + BJ_HARD_VALUE_DY, BJ_VALUE_OTZ, text, TEXT_PAL_CYAN);
                    }
                }
            }
            bj_drawHand(&player, ofsX[1], ofsY[1], -1);
            if (player.count != 0) {
                sprintf(text, g_bjFmt2d, player.value);
                ui_drawSmallDigits(BJ_VALUE_X, player.y + BJ_VALUE_DY, BJ_VALUE_OTZ, text, TEXT_PAL_WHITE);
                if (player.value != BJ_BLACKJACK && (player.flags & BJ_HAND_SOFT)) {
                    sprintf(text, g_bjFmt2d, player.hardValue);
                    ui_drawSmallDigits(BJ_VALUE_X, player.y + BJ_HARD_VALUE_DY, BJ_VALUE_OTZ, text, TEXT_PAL_CYAN);
                }
            }
            if (nsplit == 1) {
                bj_drawHand(&split, ofsX[2], ofsY[2], -1);
                if (split.count != 0) {
                    sprintf(text, g_bjFmt2d, split.value);
                    ui_drawSmallDigits(BJ_VALUE_X, split.y + BJ_VALUE_DY, BJ_VALUE_OTZ, text, TEXT_PAL_WHITE);
                    if (split.value != BJ_BLACKJACK && (split.flags & BJ_HAND_SOFT)) {
                        sprintf(text, g_bjFmt2d, split.hardValue);
                        ui_drawSmallDigits(BJ_VALUE_X, split.y + BJ_HARD_VALUE_DY, BJ_VALUE_OTZ, text, TEXT_PAL_CYAN);
                    }
                }
            }
        }
        bj_drawShoe(BJ_SHOE_X, BJ_SHOE_Y);
        addPrim(g_gameState->gameData->ot[g_gameState->gameData->drawBuf] + BJ_OTZ, &g_bjTpage);
        sprintf(text, g_bjStrBankroll);
        ui_drawSmallDigits(0x10C - (text_measureSmall(text) >> 1), 0x58, 0x10, text, TEXT_PAL_YELLOW);
        sprintf(text, g_bjFmtLong, g_bjBankroll);
        ui_drawSmallDigits(0x10C - (text_measureSmall(text) >> 1), 0x64, 0x10, text, TEXT_PAL_CYAN);
        sprintf(text, g_bjStrWager);
        ui_drawSmallDigits(0x10C - (text_measureSmall(text) >> 1), 0x70, 0x10, text, TEXT_PAL_YELLOW);
        sprintf(text, g_bjFmtLong, g_bjWager);
        ui_drawSmallDigits(0x10C - (text_measureSmall(text) >> 1), 0x7C, 0x10, text, TEXT_PAL_WHITE);
        sprintf(text, g_bjStrStand);
        ui_drawSmallDigits(0x10C - (text_measureSmall(text) >> 1), 0x90, 0x10, text, TEXT_PAL_WHITE);
        sprintf(text, g_bjStrHit);
        ui_drawSmallDigits(0x10C - (text_measureSmall(text) >> 1), 0xA0, 0x10, text, TEXT_PAL_WHITE);
        sprintf(text, g_bjStrDouble);
        ui_drawSmallDigits(0x10C - (text_measureSmall(text) >> 1), 0xB0, 0x10, text, g_bjCanDouble ? TEXT_PAL_WHITE : TEXT_PAL_GREY);
        sprintf(text, g_bjStrSplit);
        ui_drawSmallDigits(0x10C - (text_measureSmall(text) >> 1), 0xC0, 0x10, text, g_bjCanSplit ? TEXT_PAL_WHITE : TEXT_PAL_GREY);
        g_padActuator[0].smallMotor = 0;
        if (cnt > 0) {
            cnt--;
        }
        if (g_bjState < BJ_ST_DONE) {
            if (g_bjBankroll < BJ_MIN_BET) {
                timer = -1;
                g_bjState = BJ_ST_END;
            }
            if (g_padPortPressed[0] & PAD_START) {
                timer = -1;
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
    s32 tim;

    tim = (s32)db_lookupRegistered(DB_GET_ADDR, 0x44F4C, 0);
    file_setTimPos(tim, 0, 0xE4, 0x1C0, 0x180);
    file_loadTim(tim);
    file_setTimPos(tim, 0, 0xE5, 0x1C0, 0x180);
    file_loadTim(tim);
    tim = (s32)db_lookupRegistered(DB_GET_ADDR, 0x44F4D, 0);
    file_setTimPos(tim, 0, 0xE6, 0x1EC, 0x100);
    file_loadTim(tim);
    tim = (s32)db_lookupRegistered(DB_GET_ADDR, 0x44F4E, 0);
    file_setTimPos(tim, 0, 0xE0, 0x1C0, 0x100);
    file_loadTim(tim);
    tim = (s32)db_lookupRegistered(DB_GET_ADDR, 0x44F50, 0);
    file_setTimPos(tim, 0, 0xE1, 0x1D6, 0x100);
    file_loadTim(tim);
    tim = (s32)db_lookupRegistered(DB_GET_ADDR, 0x44F4F, 0);
    file_setTimPos(tim, 0, 0xE2, 0x1C0, 0x140);
    file_loadTim(tim);
    tim = (s32)db_lookupRegistered(DB_GET_ADDR, 0x44F51, 0);
    file_setTimPos(tim, 0, 0xE9, 0x280, 0x100);
    file_loadTim(tim);
    setRECT(&g_bjClutRect, 0x20, 0xE4, 0x10, 1);
    LoadImage(&g_bjClutRect, g_bjClut);
    DrawSync(0);
    setDrawTPage(&g_bjTpage, 1, 1, getTPage(1, 0, 0x1C0, 0x100));
    if (!(g_gameState->gameData->flags & GD_MUSIC_PLAYING)) {
        ending_soundCmd(SND_MUSIC_PLAY, ENDING_MUSIC, 0, 0, 0);
        ending_soundCmd(SND_SONG_VOL_FROM_INTPL, ENDING_MUSIC, 0x5A, 0, SND_VOL_MAX);
    }
    game_startFade(FADE_FLAT, FADE_SUB, 0, FADE_COLOR_FULL);
    g_bjState = BJ_ST_START;
    g_padPortMask[0] = 0;
    return 0;
}

s32 bj_exit(void) {
    return 0;
}

void bj_clearOt(void) {
    GameData *ctx = g_gameState->gameData;
    ClearOTagR(ctx->ot[ctx->drawBuf], OT_LEN);
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
    DrawOTagEnv(ctx->ot[i] + OT_LAST, &g_gameState->drawEnv[i]);
    g_gameState->gameData->drawBuf ^= 1;
    gfx_setBuffer(g_gameState->gameData->drawBuf);
}
