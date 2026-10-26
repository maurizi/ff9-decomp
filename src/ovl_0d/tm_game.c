#include "ovl_0d.h"

void tm_mainLoop(void) {
    TmState s;
    GameData *ctx;
    void *p;

    s.unk5C8.x = 0x32;
    s.unk5C8.y = 0x1E;
    s.unk5C8.w = 0xC8;
    s.unk5C8.h = 0x64;
    tm_setup(&s);
    SetDispMask(1);
    SetDispMask(1);
    while (s.task != TM_TASK_EXIT) {
        tm_clearOt();
        pad_update();
        tm_updatePad(&s);
        ctx = g_gameState->gameData;
        p = TM_PRIM_BUF + ctx->unk8 * TM_PRIM_BUF_SIZE;
        if (s.showScore != 0) {
            p = tm_drawScore(ctx->unk10[ctx->unk8], p, &s);
        }
        if (s.showBlocks != 0) {
            p = tm_drawBlocks(g_gameState->gameData->unk10[g_gameState->gameData->unk8], p, &s);
        }
        p = tm_drawBackground(g_gameState->gameData->unk10[g_gameState->gameData->unk8], p, &s);
        if (s.showBoard != 0 || s.task == TM_TASK_FIELD_FADE) {
            p = tm_drawBoard(g_gameState->gameData->unk10[g_gameState->gameData->unk8], p, &s);
        }
        p = tm_drawCards(g_gameState->gameData->unk10[g_gameState->gameData->unk8], p, &s);
        p = tm_drawCombo(g_gameState->gameData->unk10[g_gameState->gameData->unk8], p, &s);
        p = tm_drawBattleValues(g_gameState->gameData->unk10[g_gameState->gameData->unk8], p, &s);
        switch (s.task) {
        case TM_TASK_ENEMY_SELECT:
            g_menuHelp.text = 0;
            tm_debugPrint(&s, "ENEMY CARD SELECT\n");
            tm_checkGameEnd(&s);
            tm_enemySelect(&s);
            tm_layoutCards(&s);
            break;
        case TM_TASK_PLAYER_SELECT:
            help_setCursorOnTop(0);
            tm_checkGameEnd(&s);
            if (s.autoPlay == 0) {
                tm_debugPrint(&s, "PLAYER CARD SELECT\n");
                tm_taskPlayerSelect(g_gameState->gameData->unk10[g_gameState->gameData->unk8], p, &s);
            } else {
                tm_debugPrint(&s, "PLAYER AUTO SELECT\n");
                tm_autoPlace(&s);
            }
            tm_layoutCards(&s);
            break;
        case TM_TASK_PLAYER_SET:
            tm_debugPrint(&s, "PLAYER CARD SET\n");
            help_setCursorOnTop(0);
            tm_taskPlayerSet(g_gameState->gameData->unk10[g_gameState->gameData->unk8], p, &s);
            tm_layoutCards(&s);
            break;
        case TM_TASK_RESULT:
            g_menuHelp.text = 0;
            tm_drawResult(g_gameState->gameData->unk10[g_gameState->gameData->unk8], p, &s);
            tm_debugPrint(&s, "RESULT\n");
            tm_taskResult(&s);
            break;
        case TM_TASK_INIT:
            g_menuHelp.text = 0;
            tm_debugPrint(&s, "INIT\n");
            tm_taskInit(&s);
            break;
        case TM_TASK_FLASH:
            g_menuHelp.text = 0;
            tm_debugPrint(&s, "FLASH\n");
            tm_layoutCards(&s);
            tm_drawFlash(g_gameState->gameData->unk10[g_gameState->gameData->unk8], p, &s);
            break;
        case TM_TASK_BATTLE_SELECT:
            tm_debugPrint(&s, "BATTLE SELECT\n");
            help_setCursorOnTop(0);
            p = tm_taskBattleSelect(g_gameState->gameData->unk10[g_gameState->gameData->unk8], p, &s);
            tm_layoutCards(&s);
            tm_drawBattleSelect(g_gameState->gameData->unk10[g_gameState->gameData->unk8], p, &s);
            break;
        case TM_TASK_BATTLE:
            g_menuHelp.text = 0;
            tm_debugPrint(&s, "BATTLE\n");
            tm_layoutCards(&s);
            tm_taskBattle(g_gameState->gameData->unk10[g_gameState->gameData->unk8], p, &s);
            break;
        case TM_TASK_TURN:
            g_menuHelp.text = 0;
            tm_debugPrint(&s, "TURN\n");
            tm_layoutCards(&s);
            tm_taskTurn(&s);
            break;
        case TM_TASK_BATTLE_WIN:
            g_menuHelp.text = 0;
            tm_debugPrint(&s, "BATTLE WIN\n");
            tm_layoutCards(&s);
            tm_taskBattleWin(&s);
            break;
        case TM_TASK_BATTLE_LOSE:
            g_menuHelp.text = 0;
            tm_debugPrint(&s, "BATTLE LOSE\n");
            tm_layoutCards(&s);
            tm_taskBattleLose(&s);
            break;
        case TM_TASK_TRY_AGAIN:
            tm_debugPrint(&s, "TRY AGAIN\n");
            help_setCursorOnTop(1);
            tm_taskTryAgain(&s);
            tm_drawTryAgain(g_gameState->gameData->unk10[g_gameState->gameData->unk8], p, &s);
            break;
        case TM_TASK_CARD_CHANGE:
            g_menuHelp.text = 0;
            tm_debugPrint(&s, "CARD_CHANGE\n");
            tm_taskCardChange(g_gameState->gameData->unk10[g_gameState->gameData->unk8], p, &s);
            break;
        case TM_TASK_CARD_MOVE:
            g_menuHelp.text = 0;
            tm_debugPrint(&s, "CARD_MOVE\n");
            tm_taskCardMove(g_gameState->gameData->unk10[g_gameState->gameData->unk8], p, &s);
            break;
        case TM_TASK_FLASH2:
            g_menuHelp.text = 0;
            tm_debugPrint(&s, "FLASH2\n");
            tm_drawFlash2(g_gameState->gameData->unk10[g_gameState->gameData->unk8], p, &s);
            break;
        case TM_TASK_TRADE:
            g_menuHelp.text = 0;
            tm_debugPrint(&s, "TRADE\n");
            tm_taskTrade(&s);
            tm_drawTradeBadge(g_gameState->gameData->unk10[g_gameState->gameData->unk8], p, &s);
            break;
        case TM_TASK_CHANGE2:
            g_menuHelp.text = 0;
            tm_debugPrint(&s, "CHANGE2\n");
            help_setCursorOnTop(1);
            tm_taskChange2(g_gameState->gameData->unk10[g_gameState->gameData->unk8], p, &s);
            break;
        case TM_TASK_CHOICE:
            tm_debugPrint(&s, "CHOICE\n");
            help_setCursorOnTop(1);
            tm_stockInput(&s);
            tm_drawStock(g_gameState->gameData->unk10[g_gameState->gameData->unk8], p, &s);
            tm_showStockCard(&s);
            tm_layoutCards(&s);
            break;
        case TM_TASK_GAME_QUIT:
            tm_debugPrint(&s, "GAME QUIT\n");
            tm_taskGameQuit(&s);
            tm_drawStock(g_gameState->gameData->unk10[g_gameState->gameData->unk8], p, &s);
            tm_showStockCard(&s);
            tm_layoutCards(&s);
            break;
        case TM_TASK_CHOICE2:
            tm_debugPrint(&s, "CHOICE2\n");
            tm_drawStock(g_gameState->gameData->unk10[g_gameState->gameData->unk8], p, &s);
            tm_showStockCard(&s);
            tm_taskChoice2(&s);
            tm_layoutCards(&s);
            break;
        case TM_TASK_STAGE_SELECT:
            tm_debugPrint(&s, "STAGE SELECT\n");
            tm_taskStageSelect(&s);
            break;
        case TM_TASK_FIELD_FADE:
            g_menuHelp.text = 0;
            tm_debugPrint(&s, "FIELD FADE\n");
            tm_layoutCards(&s);
            break;
        case TM_TASK_AWAY_INFO:
            g_menuHelp.text = 0;
            tm_debugPrint(&s, "AWAY INFO\n");
            tm_drawStock(g_gameState->gameData->unk10[g_gameState->gameData->unk8], p, &s);
            tm_taskAwayInfo(&s);
            break;
        case TM_TASK_CARD_AWAY:
            tm_debugPrint(&s, "CARD AWAY\n");
            help_setCursorOnTop(1);
            tm_stockInput(&s);
            tm_showStockCard(&s);
            tm_drawStock(g_gameState->gameData->unk10[g_gameState->gameData->unk8], p, &s);
            break;
        case TM_TASK_CARD_AWAY2:
            tm_debugPrint(&s, "CARD AWAY2\n");
            help_setCursorOnTop(1);
            tm_showStockCard(&s);
            tm_drawStock(g_gameState->gameData->unk10[g_gameState->gameData->unk8], p, &s);
            tm_taskCardAway2(&s);
            break;
        case TM_TASK_CARD_LESS5:
            g_menuHelp.text = 0;
            tm_debugPrint(&s, "CARD LESS5\n");
            tm_taskCardLess5(&s);
            break;
        case TM_TASK_EXTENSION:
            g_menuHelp.text = 0;
            tm_debugPrint(&s, "EXTENSION\n");
            tm_taskExtension(&s);
            break;
        case TM_TASK_END_FADE:
            g_menuHelp.text = 0;
            tm_debugPrint(&s, "END FADE\n");
            if (s.quitting != 0) {
                tm_drawStock(g_gameState->gameData->unk10[g_gameState->gameData->unk8], p, &s);
                tm_showStockCard(&s);
            }
            tm_taskEndFade(&s);
            break;
        case TM_TASK_COIN:
            g_menuHelp.text = 0;
            tm_debugPrint(&s, "COIN\n");
            tm_taskCoin(g_gameState->gameData->unk10[g_gameState->gameData->unk8], p, &s);
            tm_layoutCards(&s);
            break;
        case TM_TASK_FLASH_ARROW:
            g_menuHelp.text = 0;
            tm_debugPrint(&s, "FLASH ARROW\n");
            tm_taskFlashArrow(&s);
            break;
        case TM_TASK_ENEMY_THINK:
            g_menuHelp.text = 0;
            tm_debugPrint(&s, "ENEMY THINK\n");
            tm_taskEnemyThink(g_gameState->gameData->unk10[g_gameState->gameData->unk8], p, &s);
            tm_animEnemyThink(&s);
            break;
        case TM_TASK_ENEMY_APPEAR:
            g_menuHelp.text = 0;
            tm_debugPrint(&s, "ENEMY APEAR\n");
            tm_taskEnemyAppear(&s);
            tm_revealBlocks(&s);
            break;
        case TM_TASK_GET_SELECT:
            g_menuHelp.text = 0;
            tm_debugPrint(&s, "GET SELECT\n");
            help_setCursorOnTop(0);
            tm_taskGetSelect(g_gameState->gameData->unk10[g_gameState->gameData->unk8], p, &s);
            tm_layoutCards(&s);
            break;
        default:
            g_menuHelp.text = 0;
            tm_debugPrint(&s, "NO TASK\n");
            break;
        }
        if (s.turn == TM_PLAYER) {
            tm_debugPrint(&s, "TURN PLAYER\n");
        }
        if (s.turn == TM_ENEMY) {
            tm_debugPrint(&s, "TURN ENEMY\n");
        }
        game_updateWindows();
        game_updateFade();
        if (s.task == TM_TASK_CHOICE || s.task == TM_TASK_GAME_QUIT || s.task == TM_TASK_CHOICE2 || s.task == TM_TASK_PLAYER_SELECT || s.task == TM_TASK_PLAYER_SET ||
            s.task == TM_TASK_BATTLE_SELECT || s.task == TM_TASK_TRY_AGAIN || s.task == TM_TASK_GET_SELECT || s.task == TM_TASK_CARD_AWAY || s.task == TM_TASK_CARD_AWAY2) {
            help_setLocked(0);
        } else {
            help_setLocked(1);
        }
        help_update();
        s.timer++;
        s.animFrame++;
        rand();
        tm_endFrame(&s);
    }
    pad_setRepeatEnabled(0, 0, s.savedRepeat[0]);
    pad_setRepeatEnabled(0, 1, s.savedRepeat[1]);
    pad_setRepeatEnabled(0, 2, s.savedRepeat[2]);
    pad_setRepeatEnabled(0, 3, s.savedRepeat[3]);
    pad_setRepeatEnabled(0, 8, s.savedRepeat[4]);
    pad_setRepeatEnabled(0, 0xA, s.savedRepeat[6]);
    pad_setRepeatEnabled(0, 9, s.savedRepeat[5]);
    pad_setRepeatEnabled(0, 0xB, s.savedRepeat[7]);
    pad_setRepeatEnabled(0, 4, s.savedRepeat[8]);
    pad_setRepeatEnabled(0, 5, s.savedRepeat[9]);
    pad_setRepeatEnabled(0, 6, s.savedRepeat[10]);
    pad_setRepeatEnabled(0, 7, s.savedRepeat[11]);
    DrawSync(0);
    VSync(g_gameState->vsyncMode);
    SetDispMask(0);
    help_setCursorOnTop(1);
    help_setCallback(0);
    help_setLocked(0);
}

void tm_clearOt(void) {
    ClearOTagR(g_gameState->gameData->unk10[g_gameState->gameData->unk8], 0x1000);
}

void tm_endFrame(TmState *s) {
    GameData *ctx;

    if ((g_padHeld & (PAD_L2 | PAD_R2 | PAD_L1 | PAD_R1 | PAD_START | PAD_SELECT)) == (PAD_L2 | PAD_R2 | PAD_L1 | PAD_R1 | PAD_START | PAD_SELECT)) {
        g_gameState->unk0 |= 1;
        s->task = TM_TASK_EXIT;
    }
    if (&g_gameState->drawEnv[g_gameState->gameData->unk8].dfe != NULL) {
        DrawSync(0);
        VSync(g_gameState->vsyncMode);
    } else {
        DrawSync(0);
        VSync(g_gameState->vsyncMode);
        ResetGraph(1);
    }
    PutDispEnv(&g_gameState->dispEnv[g_gameState->gameData->unk8]);
    PutDrawEnv(&g_gameState->drawEnv[g_gameState->gameData->unk8]);
    ctx = g_gameState->gameData;
    DrawOTag(ctx->unk10[ctx->unk8] + 0xFFF);
    g_gameState->gameData->unk8 ^= 1;
    gfx_setBuffer(g_gameState->gameData->unk8);
}

void tm_setup(TmState *s) {
    s32 i;
    u8 *p;
    s32 x;
    s32 y;
    s32 v;

    tm_loadImageFile();
    p = tm_getFileEntry(TM_FILE_CARDS);
    for (i = 0; i < 100; i++) {
        x = i / 30;
        y = i % 30;
        v = y / 6;
        tm_loadTim((TmTim *)(p + i * 0xA80), x * 128 + (y % 6) * 21, v * 51 + 0x100, 0x180, i + 0x19C);
    }
    tm_loadTim((TmTim *)tm_getFileEntry(TM_FILE_BG_RIGHT), 0x300, 0, 0, 0xEE);
    tm_loadTim((TmTim *)tm_getFileEntry(TM_FILE_BG), 0x280, 0, 0, 0xE0);
    tm_loadTim((TmTim *)tm_getFileEntry(TM_FILE_BOARD), 0x340, 0, 0, 0xE7);
    tm_loadTim((TmTim *)tm_getFileEntry(7), 0x300, 0x100, 0, 0xEA);
    tm_loadTim((TmTim *)tm_getFileEntry(8), 0x300, 0x180, 0, 0xEB);
    tm_loadTim((TmTim *)tm_getFileEntry(TM_FILE_BATTLE), 0x200, 0x100, 0, 0xEC);
    tm_loadTim((TmTim *)tm_getFileEntry(0xA), 0x340, 0x1E0, 0, 0xED);
    tm_loadTim((TmTim *)tm_getFileEntry(6), 0x280, 0x100, 0, 0xE9);
    tm_copyFileEntry((u8 *)g_tmOpponents, TM_FILE_OPPONENTS, 0x200);
    tm_copyFileEntry((u8 *)g_tmCardData, TM_FILE_CARD_DATA, 0x1F4);
    tm_copyFileEntry((u8 *)g_tmDecks, TM_FILE_DECKS, 0x400);
    g_evtMode = 1;
    g_gameState->vsyncMode = 0;
    game_initGfx();
    text_setBank(g_tmText);
    help_init();
    help_setCallback(help_nullHandler);
    g_menuHelp.text = 0;
    g_menuHelp.x = 0x50;
    g_menuHelp.y = 0x50;
    g_menuHelp.otz = 4;
    fade_in(0xC);
    s->wins = 0;
    s->losses = 0;
    s->draws = 0;
    s->autoPlay = 0;
    s->hasLostCard = 0;
    s->quitting = 0;
    s->opponent = (u16)g_gameState->gameData->unk8B2;
    s->deck = g_tmOpponents[s->opponent].deck;
    s->aiLevel = g_tmOpponents[s->opponent].aiLevel;
    s->unk454 = 1;
    tm_taskInit(s);
    s->savedRepeat[0] = pad_isRepeatEnabled(0, 0);
    s->savedRepeat[1] = pad_isRepeatEnabled(0, 1);
    s->savedRepeat[2] = pad_isRepeatEnabled(0, 2);
    s->savedRepeat[3] = pad_isRepeatEnabled(0, 3);
    s->savedRepeat[4] = pad_isRepeatEnabled(0, 8);
    s->savedRepeat[6] = pad_isRepeatEnabled(0, 0xA);
    s->savedRepeat[5] = pad_isRepeatEnabled(0, 9);
    s->savedRepeat[7] = pad_isRepeatEnabled(0, 0xB);
    s->savedRepeat[8] = pad_isRepeatEnabled(0, 4);
    s->savedRepeat[9] = pad_isRepeatEnabled(0, 5);
    s->savedRepeat[10] = pad_isRepeatEnabled(0, 6);
    s->savedRepeat[11] = pad_isRepeatEnabled(0, 7);
    pad_setRepeatEnabled(0, 0, 1);
    pad_setRepeatEnabled(0, 1, 1);
    pad_setRepeatEnabled(0, 2, 1);
    pad_setRepeatEnabled(0, 3, 1);
    pad_setRepeatEnabled(0, 0x12, 1);
    pad_setRepeatEnabled(0, 0x13, 1);
    pad_setRepeatEnabled(0, 0xE, 0);
    pad_setRepeatEnabled(0, 0xF, 0);
    s->arrowOdds[0] = 1;
    s->arrowOdds[1] = 4;
    s->arrowOdds[2] = 0x10;
    s->arrowOdds[3] = 0x1F;
    s->arrowOdds[4] = 0x1B;
    s->arrowOdds[5] = 0xA;
    s->arrowOdds[6] = 6;
    s->arrowOdds[7] = 3;
    s->arrowOdds[8] = 2;
    s->arrowOdds[0] = 1;
    s->arrowOdds[1] = 8;
    s->arrowOdds[2] = 0x19;
    s->arrowOdds[3] = 0x1F;
    s->arrowOdds[4] = 0x12;
    s->arrowOdds[5] = 9;
    s->arrowOdds[6] = 5;
    s->arrowOdds[7] = 2;
    s->arrowOdds[8] = 1;
    s->oddsRow = 0;
    if (card_countAll() < 5) {
        for (i = 0; i < 8; i++) {
            tm_addRandomCard(s, rand() % 100);
        }
    }
}

void tm_taskInit(TmState *arg0) {
    arg0->cursor = 0;
    arg0->boardX = 0;
    arg0->boardY = 0;
    arg0->brightness = 0x80;
    arg0->unk554 = 0;
    arg0->scrollTimer = 0;
    arg0->valuesTimer = 0;
    arg0->msgWin = NULL;
    arg0->quitWin = 0;
    arg0->infoWin = NULL;
    arg0->less5Win = NULL;
    arg0->comboTimer = 0;
    arg0->showBoard = 0;
    arg0->showBlocks = 0;
    arg0->showScore = 0;
    tm_setWindowOpening(0);
    tm_resetStockCursor(arg0);
    tm_initHands(arg0);
    tm_initBoard(arg0);
    arg0->unk4BC = 0;
    arg0->unk4C0 = 1;
    tm_openStock(arg0);
}

void tm_debugPrint(TmState *s, char *msg) {
}

void tm_loadImageFile(void) {
    FileEntry *f = file_find(1, 0x44C);
    if (f == NULL) {
        SystemError(0x46, 0x3E6);
    }
    cd_readSync(g_gameState->imgSector + f->unk4, (f->unkC - f->unk4) << 11, TM_IMAGE_FILE);
}

u8 *tm_getFileEntry(s32 i) {
    s32 *p = (s32 *)TM_IMAGE_FILE;
    p += i * 2;
    return TM_IMAGE_FILE + *p * 0x800;
}

void tm_copyFileEntry(u8 *arg0, s32 arg1, s32 arg2) {
    s32 i;
    u8 *src = tm_getFileEntry(arg1);
    for (i = 0; i < arg2; i++) {
        *arg0++ = *src++;
    }
}

void *tm_drawBackground(u32 *ot, SPRT_16 *p, TmState *s) {
    u8 c;
    s32 x;
    s32 y;

    c = s->brightness;
    for (x = 0; x < 16; x++) {
        for (y = 0; y < 15; y++) {
            if (s->showBoard == 0 || x < 3) {
                if (s->task == TM_TASK_FIELD_FADE && x >= 3) {
                    if (s->timer * 4 < 0x80) {
                        c = 0x80 - s->timer * 4;
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
            if (s->showBoard == 0 || x > 0) {
                if (s->task == TM_TASK_FIELD_FADE && x == 0) {
                    if (s->timer * 4 < 0x80) {
                        c = 0x80 - s->timer * 4;
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

void *tm_drawBoard(u32 *ot, SPRT_16 *p, TmState *s) {
    u8 c;
    s32 x;
    s32 y;
    DR_TPAGE *tp;

    c = s->brightness;
    if (s->task == TM_TASK_FIELD_FADE) {
        c = s->timer * 4;
    }
    for (x = 0; x < 14; x++) {
        for (y = 0; y < 15; y++) {
            setlen(p, 3);
            p->code = 0x7C;
            if (s->task == TM_TASK_FIELD_FADE) {
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
    if (s->task == TM_TASK_FIELD_FADE && s->timer >= 0x20) {
        s->showBoard = 1;
        tm_dealEnemyHand(s);
        s->showScore = 1;
        for (y = 4; y >= 0; y--) {
            s->landed[y] = 0;
        }
        s->showBlocks = 1;
        s->blocksDone = 0;
        s->blockTimer = 0;
        s->timer = 0;
        s->task = TM_TASK_ENEMY_APPEAR;
    }
    return tp;
}

void tm_loadTim(TmTim *tim, s32 x, s32 y, s32 cx, s32 cy) {
    TmTimImage *img;
    s32 tp;

    img = (TmTimImage *)((u8 *)tim + (tim->clutLen + 8));
    switch ((s32)(tim->flags & 3)) {
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

void *tm_drawScore(u32 *ot, SPRT *p, TmState *s) {
    s32 i;
    s32 n;
    s16 x;
    s16 y;
    u8 u;
    u8 v;
    s16 w;
    u8 c;

    c = s->brightness;
    for (i = 0; i < 2; i++) {
        if (i == 0) {
            n = tm_countOnBoard(s, TM_PLAYER);
            x = 0x28;
            y = 0xB9;
            v = 0;
        } else {
            n = tm_countOnBoard(s, TM_ENEMY);
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

void tm_updatePad(TmState *s) {
    pad_updateRepeat(0);
}

void *tm_taskPlayerSelect(u32 *ot, void *p, TmState *s) {
    RECT r;
    TmCard *c;
    SPRT *sp;
    s32 pad;
    s32 x;
    s32 y;
    s32 k;
    s32 ry;
    s32 w;

    if (tm_countInHand(s, TM_PLAYER) != 0) {
        pad = g_padRepeat;
        c = tm_getHandCard(s, TM_PLAYER, s->cursor);
        if (tm_countInHand(s, TM_PLAYER) != TM_HAND_SIZE) {
            ry = c->y + 0x34;
            if (s->cursor == 3) {
                ry = c->y - 0x19;
            }
        } else {
            ry = c->y + 0x2A;
            if (s->cursor == 4) {
                ry = c->y - 0x19;
            }
        }
        if (tm_countInHand(s, TM_PLAYER) != 0) {
            if (tm_countPlayerHand(s, TM_PLAYER) != TM_HAND_SIZE) {
                w = 0x104;
                x = c->x - 0xF;
                y = c->y + 6;
            } else {
                w = 0x108;
                x = c->x - 0x12;
                y = c->y + 3;
            }
            tm_setHelpArea(w);
            sp = tm_drawCursor(ot, p, s, x, y, 0);
            g_menuHelp.x = x + 2;
            g_menuHelp.y = y + 1;
            g_menuHelp.text = text_get(TM_HELP_SELECT);
            text_drawBlock(0x12, 9, 0xFFA, text_get(TM_TEXT_ICON_R1), 0);
            setlen(sp, 4);
            sp->code = 0x64;
            sp->x0 = 0x1F;
            sp->y0 = 0xC;
            sp->v0 = 0xD0;
            sp->w = 0x1C;
            sp->h = 8;
            sp->clut = 0x3AC0;
            sp->u0 = 0x64;
            sp->r0 = 0x80;
            sp->g0 = 0x80;
            sp->b0 = 0x80;
            AddPrim(ot + 0x28, sp);
            sp++;
            setlen((DR_TPAGE *)sp, 1);
            ((DR_TPAGE *)sp)->code[0] = (GetTPage(1, 0, 0x300, 0x100) & 0x9FF) | 0xE1000000;
            AddPrim(ot + 0x28, sp);
            p = (DR_TPAGE *)sp + 1;
            if (pad & PAD_UP) {
                if (s->cursor != 0) {
                    s->nameSlide = 0;
                    s->cursor--;
                    tm_playSe(TM_SE_CURSOR);
                }
            }
            if (pad & PAD_DOWN) {
                if (s->cursor != tm_countInHand(s, TM_PLAYER) - 1) {
                    s->nameSlide = 0;
                    s->cursor++;
                    tm_playSe(TM_SE_CURSOR);
                }
            }
            if (g_padHeld & BTN_R1) {
                if (s->nameSlide < 0xA) {
                    s->nameSlide++;
                }
            } else if (s->nameSlide > 0) {
                s->nameSlide--;
            }
            if (s->nameSlide > 0) {
                r.w = card_getNameWidth(c->id) + 0x10;
                r.x = 0x11A - r.w / 2;
                if (r.x + r.w >= 0x140) {
                    r.x = 0x130 - card_getNameWidth(c->id);
                }
                r.y = ry;
                r.h = 0x19;
                k = s->nameSlide * 0xC - 0x78;
                r.x -= k;
                text_drawBlock(r.x + 8, r.y + 6, 5, (u8 *)card_getName(c->id), 0);
                win_drawBox(&r, 5, 2);
            }
            if (pad & BTN_CROSS) {
                s->task = TM_TASK_PLAYER_SET;
                s->savedFrame = s->animFrame;
                tm_playSe(TM_SE_CURSOR);
            }
        }
    }
    return p;
}

void *tm_taskPlayerSet(u32 *ot, void *p, TmState *s) {
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

    if (tm_countInHand(s, TM_PLAYER) == 0) {
        return p;
    }
    pad = g_padRepeat;
    tm_getHandCard(s, TM_PLAYER, s->cursor);
    cell = tm_getCell(s, s->boardX, s->boardY);
    cx = s->boardX * TM_SQUARE_W + TM_BOARD_X;
    cy = s->boardY * TM_SQUARE_H + TM_BOARD_Y;
    p = tm_drawCursor(ot, p, s, cx - 0xF, cy + 6, 0);
    c = tm_getHandCard(s, TM_PLAYER, s->cursor);
    if (tm_countPlayerHand(s, TM_PLAYER) != TM_HAND_SIZE) {
        x = c->x - 0xF;
        y = c->y + 6;
    } else {
        x = c->x - 0x12;
        y = c->y + 3;
    }
    p = tm_drawCursor(ot, p, s, x, y, 1);
    g_menuHelp.x = cx - 0xD;
    g_menuHelp.y = cy + 7;
    if (cell->state == TM_CELL_EMPTY) {
        g_menuHelp.text = text_get(TM_HELP_PLACE);
    } else {
        g_menuHelp.text = text_get(TM_HELP_PLACE_BLOCKED);
    }
    help_setBounds(NULL);
    if (pad & BTN_CIRCLE) {
        s->nameSlide = 0;
        s->task = TM_TASK_PLAYER_SELECT;
        s->animFrame = s->savedFrame;
        tm_playSe(TM_SE_CANCEL);
        return p;
    }
    if (pad & BTN_CROSS) {
        if (cell->state == TM_CELL_EMPTY) {
            c = tm_getHandCard(s, TM_PLAYER, s->cursor);
            c->x = cx;
            c->y = cy;
            c->onBoard = 1;
            c->small = 0;
            cell->state = TM_CELL_CARD;
            cell->card = c;
            s->cursor = 0;
            s->srcX = s->boardX;
            s->srcY = s->boardY;
            tm_checkBattles(s);
            tm_playSe(TM_SE_CARD);
            return p;
        }
        tm_playSe(TM_SE_BUZZER);
    }
    ox = s->boardX;
    oy = s->boardY;
    if ((pad & PAD_RIGHT) && s->boardX != 3) {
        s->boardX++;
    }
    if ((pad & PAD_LEFT) && s->boardX != 0) {
        s->boardX--;
    }
    if ((pad & PAD_UP) && s->boardY != 0) {
        s->boardY--;
    }
    if ((pad & PAD_DOWN) && s->boardY != 3) {
        s->boardY++;
    }
    if (ox != s->boardX || oy != s->boardY) {
        tm_playSe(TM_SE_CURSOR);
    }
    return p;
}

void tm_taskResult(TmState *s) {
    RECT r;

    if ((g_padRepeat & (BTN_CIRCLE | BTN_CROSS)) && s->timer >= 0x22) {
        s->showBoard = 0;
        s->showScore = 0;
        s->showBlocks = 0;
        switch (s->result) {
        case 0:
            tm_startCardMove(s);
            break;
        case 1:
            tm_startCardMove(s);
            break;
        case 2:
            tm_returnHand(s);
            tm_hideCards(s);
            if ((u32)(s->opponent - TM_OPP_FORCED) < 4) {
                r.x = 0x59;
                r.w = 0x8F;
                r.w = text_measureWidth(text_get(TM_TEXT_DRAW)) + 0x12;
                r.x = 0xA0 - r.w / 2;
                r.y = 0x59;
                r.h = 0x2B;
                s->infoWin = win_openFree(&r, 5, 2);
                win_setDrawContent(s->infoWin, tm_drawDrawWin);
                win_setShadow(s->infoWin, 1);
                tm_waitWindow(s->infoWin);
                s->task = TM_TASK_EXTENSION;
            } else {
                tm_askRematch(s);
            }
            break;
        }
    }
}

void tm_taskExtension(TmState *arg0) {
    if (g_tmWindowOpening == 0 && (g_padRepeat & (BTN_CIRCLE | BTN_CROSS))) {
        win_close(arg0->infoWin);
        arg0->task = TM_TASK_INIT;
    }
}

void tm_drawDrawWin(Window *arg0) {
    text_drawBlock(arg0->unk0 + 9, arg0->unk2 + 7, arg0->unk18, text_get(TM_TEXT_DRAW), 0);
}

void tm_autoPlace(TmState *arg0) {
    s32 x;
    s32 y;
    s32 px;
    s32 py;
    TmCell *cell;
    TmCard *card;

    if (tm_countInHand(arg0, TM_PLAYER) != 0) {
        do {
            x = rand() & 3;
            y = rand() & 3;
            cell = tm_getCell(arg0, x, y);
        } while (cell->state != TM_CELL_EMPTY);
        px = x * TM_SQUARE_W + TM_BOARD_X;
        py = y * TM_SQUARE_H + TM_BOARD_Y;
        card = tm_getHandCard(arg0, TM_PLAYER, 0);
        card->x = px;
        card->y = py;
        card->small = 0;
        card->onBoard = 1;
        cell->state = TM_CELL_CARD;
        cell->card = card;
        arg0->srcX = x;
        arg0->srcY = y;
        tm_checkBattles(arg0);
    }
}

void tm_taskTrade(TmState *arg0) {
    if (g_tmWindowOpening == 0 && (g_padRepeat & (BTN_CROSS | BTN_SQUARE))) {
        tm_endTrade(arg0);
    }
}

void tm_taskTryAgain(TmState *s) {
    RECT r;
    s32 pad;

    if (g_tmWindowOpening == 0) {
        pad = g_padRepeat;
        if (pad & BTN_CROSS) {
            win_close(s->msgWin);
            if (s->cursor == 0) {
                if (card_countAll() < 5) {
                    r.w = text_measureWidth(text_get(TM_TEXT_LESS_5)) + 0x12;
                    r.x = 0xA0 - r.w / 2;
                    r.y = 0x5B;
                    r.h = 0x2B;
                    s->less5Win = win_openFree(&r, 6, 2);
                    win_setDrawContent(s->less5Win, tm_drawLess5Win);
                    win_setShadow(s->less5Win, 1);
                    tm_waitWindow(s->less5Win);
                    s->task = TM_TASK_CARD_LESS5;
                    tm_playSe(TM_SE_BUZZER);
                } else {
                    s->task = TM_TASK_INIT;
                }
            } else {
                tm_startEndFade(s);
            }
            tm_playSe(TM_SE_CURSOR);
        }
        if ((pad & BTN_CIRCLE) && s->cursor != 1) {
            tm_playSe(TM_SE_CANCEL);
            s->cursor = 1;
        }
        if (pad & PAD_UP) {
            if (s->cursor != 0) {
                tm_playSe(TM_SE_CURSOR);
                s->cursor = 0;
            } else {
                tm_playSe(TM_SE_CURSOR);
                s->cursor = 1;
            }
        }
        if (pad & PAD_DOWN) {
            if (s->cursor != 1) {
                tm_playSe(TM_SE_CURSOR);
                s->cursor = 1;
            } else {
                tm_playSe(TM_SE_CURSOR);
                s->cursor = 0;
            }
        }
        if (s->cursor == 0) {
            g_menuHelp.x = 0x7D;
            g_menuHelp.y = 0x6B;
            g_menuHelp.text = text_get(TM_HELP_PLAY_AGAIN);
            ui_drawHand(0x7D, 0x6B, 3);
        } else {
            g_menuHelp.x = 0x7D;
            g_menuHelp.y = 0x7B;
            g_menuHelp.text = text_get(TM_HELP_END_GAME);
            ui_drawHand(0x7D, 0x7B, 3);
        }
        tm_setHelpArea(0x89);
    }
}

void tm_stockInput(TmState *s) {
    RECT r;
    u32 pad;
    s32 n;
    s32 x;
    s32 y;
    TmCard *c;
    Unk6 *d;

    ui_drawHand(s->stockCol * 15 + 3, s->stockRow * 15 + 0x30, 3);
    if (s->task == TM_TASK_CHOICE || s->task == TM_TASK_CARD_AWAY) {
        g_menuHelp.x = s->stockCol * 15 + 3;
        g_menuHelp.y = s->stockRow * 15 + 0x30;
        if (s->task == TM_TASK_CHOICE) {
            g_menuHelp.text = text_get((u32)(s->opponent - TM_OPP_FORCED) >= 4 ? TM_HELP_STOCK : TM_HELP_STOCK_FORCED);
        } else {
            g_menuHelp.text = text_get(TM_HELP_DISCARD_PICK);
        }
        r.x = 0xD;
        r.w = 0x9E;
        r.y = 0;
        r.h = 0xDE;
        help_setBounds(&r);
    }
    pad = g_padRepeat;
    n = card_countOf(tm_getStockCursorId(s));
    if (s->scrollTimer != 0) {
        pad = 0;
        if (--s->scrollTimer == 0 && s->phase == 0) {
            if (n == ++s->copyIdx) {
                s->copyIdx = 0;
            }
        }
    }
    if (n >= 2) {
        if (pad & BTN_R1) {
            tm_playSe(TM_SE_CARD);
            pad = 0;
            s->phase = 0;
            s->scrollTimer = 4;
        } else if (pad & BTN_L1) {
            pad = 0;
            s->phase = 1;
            s->scrollTimer = 4;
            s->copyIdx--;
            tm_playSe(TM_SE_CARD);
            if (s->copyIdx == -1) {
                s->copyIdx = n - 1;
            }
        }
    }
    x = s->stockCol;
    y = s->stockRow;
    if ((pad & PAD_RIGHT) && x != 9) {
        s->stockCol = x + 1;
        s->copyIdx = 0;
    }
    if ((pad & PAD_LEFT) && s->stockCol != 0) {
        s->stockCol--;
        s->copyIdx = 0;
    }
    if ((pad & PAD_DOWN) && s->stockRow != 9) {
        s->stockRow++;
        s->copyIdx = 0;
    }
    if ((pad & PAD_UP) && s->stockRow != 0) {
        s->stockRow--;
        s->copyIdx = 0;
    }
    if (x != s->stockCol || y != s->stockRow) {
        tm_playSe(TM_SE_CURSOR);
        return;
    }
    if (s->task == TM_TASK_CHOICE) {
        if (((pad & BTN_CIRCLE) && s->handCount == 0) || (pad & BTN_SQUARE)) {
            if (s->opponent != TM_OPP_FORCED) if (s->opponent != TM_OPP_FORCED + 1) if (s->opponent != TM_OPP_FORCED + 2) if (s->opponent != TM_OPP_FORCED + 3) {
                r.w = text_measureWidth(text_get(TM_TEXT_QUIT_ASK)) + 0x12;
                r.x = 0xA0 - r.w / 2;
                r.y = 0x54;
                r.h = 0x38;
                s->quitChoice = 1;
                s->quitWin = win_openFree(&r, 5, 2);
                win_setDrawContent(s->quitWin, tm_drawQuitWin);
                win_setShadow(s->quitWin, 1);
                tm_waitWindow(s->quitWin);
                s->task = TM_TASK_GAME_QUIT;
                return;
            }
        }
        if (pad & BTN_CIRCLE) {
            if (s->handCount != 0) {
                s->handCount--;
                c = &s->cards[s->handCount];
                card_addWithStats(c->id, c->arrows, c->power, c->type, c->pdef, c->mdef);
                tm_playSe(TM_SE_CANCEL);
            }
        } else if (pad & BTN_CROSS) {
            if (s->autoPlay != 0) {
                card_clearAll();
                s->stockCol = rand() % 10;
                s->stockRow = rand() % 10;
                s->copyIdx = 0;
                if (card_countOf(tm_getStockCursorId(s)) == 0) {
                    tm_addRandomCard(s, tm_getStockCursorId(s));
                    tm_addRandomCard(s, tm_getStockCursorId(s));
                    tm_addRandomCard(s, tm_getStockCursorId(s));
                    tm_addRandomCard(s, tm_getStockCursorId(s));
                    tm_addRandomCard(s, tm_getStockCursorId(s));
                    tm_addRandomCard(s, tm_getStockCursorId(s));
                    tm_addRandomCard(s, tm_getStockCursorId(s));
                }
            }
            d = card_find(tm_getStockCursorId(s), s->copyIdx);
            if (d != (Unk6 *)-1) {
                c = &s->cards[s->handCount];
                c->id = d->unk0;
                c->power = d->unk2;
                c->type = d->unk3;
                c->pdef = d->unk4;
                c->mdef = d->unk5;
                c->arrows = d->unk1;
                card_remove(tm_getStockCursorId(s), s->copyIdx);
                s->copyIdx = 0;
                tm_playSe(TM_SE_CARD);
                if (++s->handCount == TM_HAND_SIZE) {
                    g_menuHelpText = 0;
                    r.w = text_measureWidth(text_get(TM_TEXT_CONFIRM)) + 0x12;
                    r.x = 0xA0 - r.w / 2;
                    r.y = 0x53;
                    r.h = 0x3A;
                    s->msgWin = win_openFree(&r, 6, 2);
                    win_setDrawContent(s->msgWin, tm_drawConfirmWin);
                    win_setShadow(s->msgWin, 1);
                    tm_waitWindow(s->msgWin);
                    s->cursor = 0;
                    s->task = TM_TASK_CHOICE2;
                }
            }
        }
    } else if ((pad & BTN_CROSS) && card_countOf(tm_getStockCursorId(s)) != 0) {
        g_menuHelpText = 0;
        r.w = text_measureWidth(text_get(TM_TEXT_DISCARD_ASK)) + 0x12;
        r.x = 0xB9;
        r.h = 0x38;
        r.y = 0x35;
        s->cursor = 1;
        s->infoWin = win_openFree(&r, 5, 2);
        win_setDrawContent(s->infoWin, tm_drawDiscardWin);
        win_setShadow(s->infoWin, 1);
        tm_waitWindow(s->infoWin);
        tm_playSe(TM_SE_CURSOR);
        s->task = TM_TASK_CARD_AWAY2;
    }
}

void tm_taskChoice2(TmState *s) {
    s32 pad;
    TmCard *c;

    if (g_tmWindowOpening == 0) {
        pad = g_padRepeat;
        if (pad & BTN_CROSS) {
            if (s->cursor == 0) {
                s->timer = 0;
                s->task = TM_TASK_FIELD_FADE;
            } else {
                s->task = TM_TASK_CHOICE;
                s->handCount = 4;
                c = &s->cards[4];
                card_addWithStats(c->id, c->arrows, c->power, c->type, c->pdef, c->mdef);
            }
            win_close(s->msgWin);
            tm_playSe(TM_SE_CURSOR);
        }
        if ((pad & BTN_CIRCLE) && s->cursor != 1) {
            s->cursor = 1;
            tm_playSe(TM_SE_CANCEL);
        }
        if (pad & PAD_UP) {
            if (s->cursor != 0) {
                s->cursor = 0;
            } else {
                s->cursor = 1;
            }
            tm_playSe(TM_SE_CURSOR);
        }
        if (pad & PAD_DOWN) {
            if (s->cursor != 1) {
                s->cursor = 1;
            } else {
                s->cursor = 0;
            }
            tm_playSe(TM_SE_CURSOR);
        }
        if (s->cursor == 0) {
            g_menuHelp.x = 0x7B;
            g_menuHelp.y = 0x6C;
            g_menuHelp.text = text_get(TM_HELP_BEGIN);
            ui_drawHand(0x7B, 0x6C, 3);
        } else {
            g_menuHelp.x = 0x7B;
            g_menuHelp.y = 0x7B;
            g_menuHelp.text = text_get(TM_HELP_RESELECT);
            ui_drawHand(0x7B, 0x7B, 3);
        }
        tm_setHelpArea(0x8D);
    }
}

void tm_drawConfirmWin(Window *arg0) {
    text_drawBlock(arg0->unk0 + 9, arg0->unk2 + 7, arg0->unk18, text_get(TM_TEXT_CONFIRM), 0);
}

void tm_drawLess5Win(Window *arg0) {
    text_drawBlock(arg0->unk0 + 9, arg0->unk2 + 7, arg0->unk18, text_get(TM_TEXT_LESS_5), 0);
}

void tm_taskStageSelect(TmState *s) {
    s32 sum;
    s32 pad;

    sum = s->arrowOdds[0] + s->arrowOdds[1] + s->arrowOdds[2] + s->arrowOdds[3] + s->arrowOdds[4] + s->arrowOdds[5] +
          s->arrowOdds[6] + s->arrowOdds[7] + s->arrowOdds[8];
    pad = g_padRepeat;
    if (!(g_padHeld & BTN_CIRCLE)) {
        if (pad & PAD_DOWN) {
            s->opponent--;
        }
        if (pad & PAD_RIGHT) {
            s->opponent += 10;
        }
        if (pad & PAD_UP) {
            s->opponent++;
        }
        if (pad & PAD_LEFT) {
            s->opponent -= 10;
        }
        if ((pad & BTN_CROSS) && sum == 100) {
            tm_openStock(s);
        }
    } else {
        if ((pad & PAD_DOWN) && s->oddsRow != 8) {
            s->oddsRow++;
        }
        if ((pad & PAD_UP) && s->oddsRow != 0) {
            s->oddsRow--;
        }
        if ((pad & PAD_RIGHT) && sum != 100) {
            s->arrowOdds[s->oddsRow]++;
        }
        if ((pad & PAD_LEFT) && s->arrowOdds[s->oddsRow] != 0) {
            s->arrowOdds[s->oddsRow]--;
        }
    }
    s->deck = g_tmOpponents[s->opponent].deck;
    s->aiLevel = g_tmOpponents[s->opponent].aiLevel;
}

void tm_taskAwayInfo(TmState *arg0) {
    if (g_tmWindowOpening == 0 && (g_padRepeat & (BTN_CROSS | BTN_SQUARE))) {
        win_close(arg0->infoWin);
        arg0->task = TM_TASK_CARD_AWAY;
    }
}

void tm_taskCardAway2(TmState *s) {
    s32 pad;

    if (g_tmWindowOpening == 0) {
        pad = g_padRepeat;
        if (pad & BTN_CROSS) {
            if (s->cursor == 0) {
                card_remove(tm_getStockCursorId(s), s->copyIdx);
                if (card_countAll() > 100) {
                    s->copyIdx = 0;
                    s->task = TM_TASK_CARD_AWAY;
                } else {
                    tm_hideCards(s);
                    tm_askRematch(s);
                }
            } else {
                s->task = TM_TASK_CARD_AWAY;
            }
            win_close(s->infoWin);
            tm_playSe(TM_SE_CURSOR);
        }
        if ((pad & BTN_CIRCLE) && s->cursor != 1) {
            s->cursor = 1;
            tm_playSe(TM_SE_CANCEL);
        }
        if (pad & PAD_UP) {
            if (s->cursor != 0) {
                s->cursor = 0;
            } else {
                s->cursor = 1;
            }
            tm_playSe(TM_SE_CURSOR);
        }
        if (pad & PAD_DOWN) {
            if (s->cursor != 1) {
                s->cursor = 1;
            } else {
                s->cursor = 0;
            }
            tm_playSe(TM_SE_CURSOR);
        }
        if (s->cursor == 0) {
            g_menuHelp.x = 0xBC;
            g_menuHelp.y = 0x4D;
            g_menuHelp.text = text_get(TM_HELP_DISCARD_YES);
            ui_drawHand(0xBC, 0x4D, 3);
        } else {
            g_menuHelp.x = 0xBC;
            g_menuHelp.y = 0x5C;
            g_menuHelp.text = text_get(TM_HELP_DISCARD_NO);
            ui_drawHand(0xBC, 0x5C, 3);
        }
        tm_setHelpArea(0xCE);
    }
    if (s->timer & 3) {
        ui_drawHand(s->stockCol * 15 + 3, s->stockRow * 15 + 0x30, 6);
    }
}

void tm_drawDiscardWin(Window *arg0) {
    text_drawBlock(arg0->unk0 + 0xA, arg0->unk2 + 6, arg0->unk18, text_get(TM_TEXT_DISCARD_ASK), 0);
}

void *tm_taskBattleSelect(u32 *ot, void *p, TmState *s) {
    s32 pad;
    s32 ox;
    s32 oy;
    s32 i;
    s32 x;
    s32 y;
    TmCell *cell;
    TmCard *c;

    pad = g_padRepeat;
    ox = s->boardX;
    oy = s->boardY;
    if ((pad & PAD_RIGHT) && ox != 3 && ox != s->srcX + 1) {
        s->boardX = ox + 1;
    }
    if ((pad & PAD_LEFT) && s->boardX != 0 && s->boardX != s->srcX - 1) {
        s->boardX--;
    }
    if ((pad & PAD_UP) && s->boardY != 0 && s->boardY != s->srcY - 1) {
        s->boardY--;
    }
    if ((pad & PAD_DOWN) && s->boardY != 3 && s->boardY != s->srcY + 1) {
        s->boardY++;
    }
    if (ox != s->boardX || oy != s->boardY) {
        tm_playSe(TM_SE_CURSOR);
    }
    if (pad & BTN_CROSS) {
        cell = tm_getCell(s, s->boardX, s->boardY);
        if (cell->state == TM_CELL_CARD) {
            c = cell->card;
            for (i = 0; i < s->numTargets; i++) {
                if (c == s->targets[i]) {
                    s->timer = 0;
                    s->task = TM_TASK_BATTLE;
                    s->dstX = s->boardX;
                    s->dstY = s->boardY;
                }
            }
        }
        if (s->task != TM_TASK_BATTLE) {
            tm_playSe(TM_SE_BUZZER);
        }
    }
    x = s->boardX * 43;
    y = s->boardY * 52;
    p = tm_drawCursor(ot, p, s, x + 0x39, y + 0xE, 0);
    g_menuHelp.x = x + 0x3B;
    g_menuHelp.y = y + 0xF;
    g_menuHelp.text = text_get(TM_HELP_NO_BATTLE);
    cell = tm_getCell(s, s->boardX, s->boardY);
    if (cell->state == TM_CELL_CARD) {
        c = cell->card;
        for (i = 0; i < s->numTargets; i++) {
            if (c == s->targets[i]) {
                g_menuHelp.text = text_get(TM_HELP_BATTLE);
            }
        }
    }
    help_setBounds(NULL);
    return p;
}

void tm_taskCardLess5(TmState *arg0) {
    if (g_padRepeat & (BTN_CIRCLE | BTN_CROSS)) {
        win_close(arg0->less5Win);
        tm_startEndFade(arg0);
    }
}

void tm_waitWindow(Window *arg0) {
    win_setOnOpened(arg0, (void (*)(Window *))tm_onWindowOpened);
    tm_setWindowOpening(1);
}

void tm_onWindowOpened(void) {
    tm_setWindowOpening(0);
}

void tm_setWindowOpening(s32 arg0) {
    g_tmWindowOpening = arg0;
}

void tm_setHelpArea(s16 arg0) {
    RECT r;
    r.x = 0;
    r.y = 0;
    r.w = arg0;
    r.h = 0xF0;
    help_setBounds(&r);
}

void *tm_taskGetSelect(u32 *ot, void *p, TmState *s) {
    RECT r;
    RECT r2;
    u8 buf[64];
    u8 num[16];
    s32 pad;
    TmCard *sel;
    TmCard *c;
    TmFlip *e;
    s32 x;
    s32 y;
    s32 i;
    s32 j;
    s32 n;
    TmCard *cards;
    s32 k;

    pad = g_padRepeat;
    if ((pad & PAD_UP) && s->getCursor != 0) {
        s->getCursor--;
        tm_playSe(TM_SE_CURSOR);
    }
    if ((pad & PAD_DOWN) && s->getCursor != tm_countCaptured(s) - 1) {
        s->getCursor++;
        tm_playSe(TM_SE_CURSOR);
    }
    sel = tm_getCaptured(s, s->getCursor);
    x = sel->x;
    y = sel->y;
    p = tm_drawCursor(ot, p, s, x + 0x22, y + 5, 2);
    g_menuHelp.x = x + 0x25;
    g_menuHelp.y = y + 6;
    g_menuHelp.text = text_get(TM_HELP_TAKE);
    r.x = x + 0x2A;
    r.w = 0x12C;
    r.y = 0;
    r.h = 0xF0;
    help_setBounds(&r);
    if (pad & BTN_CROSS) {
        if (s->hasLostCard != 0 && sel == &s->cards[s->lostSlot + 5]) {
            s->hasLostCard = 0;
        }
        tm_clearFlips(s);
        tm_clearFlips(s);
        k = 0;
        e = s->flips;
        cards = s->cards;
        for (i = 0; i < TM_NUM_CARDS; i++) {
            c = &cards[i];
            if (i < TM_HAND_SIZE) {
                if (c->owner == TM_ENEMY) {
                    e[k].card = c;
                    e[k].mode = TM_FLIP_CHANGE;
                    e[k].arg = 0;
                    k++;
                }
            } else if (c->owner == TM_PLAYER && sel != c) {
                e[k].card = c;
                e[k].mode = TM_FLIP_CHANGE;
                e[k].arg = 1;
                k++;
            }
        }
        s->flashMode = TM_FLIP_CHANGE;
        s->task = TM_TASK_FLASH2;
    }
    n = 0;
    cards = s->cards;
    for (i = 0; i < TM_NUM_CARDS; i++) {
        if (i >= 5 && tm_getCaptured(s, s->getCursor) == &cards[i]) {
            n = card_countOf(cards[i].id);
            for (j = 0; j < 5; j++) {
                if (cards[i].id == cards[j].id) {
                    n++;
                }
            }
        }
    }
    text_formatNumber(n, num, -1, 0);
    text_copy(buf, text_get(TM_TEXT_STOCK));
    text_append(buf, num);
    text_append(buf, text_get(TM_TEXT_STOCK_END));
    r2.w = text_measureWidth((s32)buf) + 0x10;
    r2.x = 0x52;
    r2.y = sel->y - 9;
    r2.h = 0x19;
    text_drawBlock(0x5A, r2.y + 6, 3, buf, 0);
    win_drawBox(&r2, 3, 2);
    return p;
}

void tm_taskGameQuit(TmState *s) {
    s32 pad;
    s32 i;
    TmCard *c;

    if (g_tmWindowOpening == 0) {
        pad = g_padRepeat;
        if (pad & PAD_UP) {
            if (s->quitChoice != 0) {
                s->quitChoice = 0;
            } else {
                s->quitChoice = 1;
            }
            tm_playSe(TM_SE_CURSOR);
        }
        if (pad & PAD_DOWN) {
            if (s->quitChoice != 1) {
                s->quitChoice = 1;
            } else {
                s->quitChoice = 0;
            }
            tm_playSe(TM_SE_CURSOR);
        }
        if (pad & BTN_CIRCLE) {
            if (s->quitChoice != 1) {
                s->quitChoice = 1;
                tm_playSe(TM_SE_CANCEL);
            }
        } else if (pad & BTN_CROSS) {
            if (s->quitChoice == 0) {
                for (i = 0; i < s->handCount; i++) {
                    c = &s->cards[i];
                    card_addWithStats(c->id, c->arrows, c->power, c->type,
                                  c->pdef, c->mdef);
                }
                s->handCount = 0;
                func_80062750(1);
                s->quitting = 1;
                tm_startEndFade(s);
            } else {
                s->task = TM_TASK_CHOICE;
            }
            win_close(s->quitWin);
            tm_playSe(TM_SE_CURSOR);
        }
        if (s->quitChoice == 0) {
            g_menuHelp.x = 0x7B;
            g_menuHelp.y = 0x6C;
            g_menuHelp.text = text_get(TM_HELP_QUIT_YES);
            ui_drawHand(0x7B, 0x6C, 3);
        } else {
            g_menuHelp.x = 0x7B;
            g_menuHelp.y = 0x7B;
            g_menuHelp.text = text_get(TM_HELP_QUIT_NO);
            ui_drawHand(0x7B, 0x7B, 3);
        }
        tm_setHelpArea(0x8D);
    } else {
        g_menuHelp.text = NULL;
    }
}

void tm_drawQuitWin(Window *arg0) {
    text_drawBlock(arg0->unk0 + 9, arg0->unk2 + 6, arg0->unk18, text_get(TM_TEXT_QUIT_ASK), 0);
}

void *tm_drawBattleSelect(u32 *ot, void *p, TmState *s) {
    return p;
}

void tm_checkBattles(TmState *s) {
    TmPos pos;
    s32 n;
    s32 dir;
    s32 y;
    u8 arrows;
    s32 owner;
    TmCard *c;
    s32 v;

    n = 0;
    s->valuesTimer = 0;
    c = tm_getCardAt(s, s->srcX, s->srcY);
    owner = c->owner;
    arrows = c->arrows;
    for (dir = 0; dir < 8; dir++) {
        if (arrows & 1) {
            pos.x = s->srcX;
            pos.y = s->srcY;
            if (tm_getDefender(s, &pos, dir, owner) != NULL) {
                s->dstX = pos.x;
                y = pos.y;
                s->timer = 0;
                s->dstY = y;
                s->attacker = tm_getCell(s, s->srcX, s->srcY)->card;
                s->targets[n] = tm_getCell(s, pos.x, pos.y)->card;
                n++;
            }
        }
        arrows >>= 1;
    }
    if (n == 1) {
        s->task = TM_TASK_BATTLE;
    } else if (n >= 2) {
        if (s->turn == TM_PLAYER && s->autoPlay == 0) {
            s->timer = 0;
            s->numTargets = n;
            s->task = TM_TASK_BATTLE_SELECT;
        } else {
            s->task = TM_TASK_BATTLE;
        }
    } else if (tm_evalPlacement(s, owner, s->srcX, s->srcY, (TmCard *)-1) != 0) {
        tm_setFlips(s, owner, s->srcX, s->srcY, TM_TASK_TURN, TM_FLIP_CHANGE);
    } else {
        s->task = TM_TASK_TURN;
    }
}

void *tm_taskBattle(u32 *ot, void *vp, TmState *s) {
    SPRT *p = vp;
    TmBattle pos;
    TmCard *a;
    TmCard *b;
    TmCard *l;
    TmFlip *e;
    s32 t;
    s16 x;
    s16 y;
    s32 h;
    u8 c;
    u32 *o;

    a = tm_getCardAt(s, s->srcX, s->srcY);
    b = tm_getCardAt(s, s->dstX, s->dstY);
    tm_getBattleValues((s32)s, &pos, a, b);
    s->valuesTimer = 0x3C;
    if (s->timer == 0x23) {
        tm_playSe(TM_SE_BATTLE);
    }
    if (s->timer < 0x23) {
        s->atkShown = pos.atk;
        s->defShown = pos.def;
    } else if (s->timer < 0x43) {
        x = (s->srcX + s->dstX) * 43 / 2 + 0x48;
        y = (s->srcY + s->dstY) * 26 + 9;
        c = s->brightness;
        h = 0x30;
        if (s->timer - 0x23 < 0xF) {
            h = 0x28;
        }
        setlen(p, 4);
        p->code = 0x66;
        p->x0 = x;
        p->y0 = y;
        p->r0 = c;
        p->g0 = c;
        p->b0 = c;
        p->w = 0x28;
        p->h = h;
        p->u0 = (((s->timer - 0x23) & 0xF) >> 2) * 40;
        p->v0 = ((s->timer - 0x23) / 16) * 40;
        o = ot + 30;
        p->clut = 0x3B00;
        AddPrim(o, p);
        p++;
        setlen((DR_TPAGE *)p, 1);
        ((DR_TPAGE *)p)->code[0] = (GetTPage(1, 1, 0x200, 0x100) & 0x9FF) | 0xE1000000;
        AddPrim(o, p);
        s->atkShown = pos.atk;
        p = (SPRT *)((DR_TPAGE *)p + 1);
        s->defShown = pos.def;
    } else {
        if (s->timer == 0x43) {
            s->atkRoll = rand() % pos.atk;
            s->defRoll = rand() % pos.def;
        }
        s->atkShown = pos.atk - (pos.atk - s->atkRoll) * (s->timer - 0x43) / 23;
        s->defShown = pos.def - (pos.def - s->defRoll) * (s->timer - 0x43) / 23;
        if (s->timer >= 0x5A) {
            e = s->flips;
            tm_clearFlips(s);
            e->mode = TM_FLIP_BATTLE;
            s->flashMode = TM_FLIP_BATTLE;
            s->task = TM_TASK_FLASH;
            tm_playSe(TM_SE_FLASH);
            if (s->atkRoll > s->defRoll) {
                l = a;
                e->x = s->dstX;
                e->y = s->dstY;
                s->flashNext = TM_TASK_BATTLE_WIN;
                s->flipOwner = s->turn;
            } else {
                if (s->turn == TM_PLAYER) {
                    s->flipOwner = TM_ENEMY;
                } else {
                    s->flipOwner = TM_PLAYER;
                }
                e->x = s->srcX;
                e->y = s->srcY;
                l = b;
                s->flashNext = TM_TASK_BATTLE_LOSE;
            }
            switch (l->type) {
            case 0:
            case 1:
                if ((rand() & 0x3F) == 0) {
                    l->type = 2;
                }
                break;
            case 2:
                if ((rand() & 0x7F) == 0) {
                    l->type = 3;
                }
                break;
            }
            s->atkShown = s->atkRoll;
            s->defShown = s->defRoll;
        }
    }
    return p;
}

void tm_getBattleValues(s32 unused, TmBattle *out, TmCard *a, TmCard *b) {
    s32 atk;
    s32 def;
    s32 aatk;
    s32 apdef;
    s32 amdef;
    s32 batk;
    s32 bpdef;
    s32 bmdef;

    atk = 0;
    aatk = a->power;
    apdef = a->pdef;
    amdef = a->mdef;
    batk = b->power;
    bpdef = b->pdef;
    bmdef = b->mdef;
    def = 0;
    switch (a->type) {
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
    out->atk = atk;
    out->def = def;
}

void tm_taskBattleWin(TmState *arg0) {
    if (tm_evalPlacement(arg0, arg0->turn, arg0->dstX, arg0->dstY, (TmCard *)-1) != 0) {
        tm_setFlips(arg0, arg0->turn, arg0->dstX, arg0->dstY, -1, TM_FLIP_COMBO);
    } else {
        tm_checkBattles(arg0);
    }
}

void tm_taskBattleLose(TmState *arg0) {
    s32 who = arg0->turn == TM_PLAYER;
    if (tm_evalPlacement(arg0, who, arg0->srcX, arg0->srcY, (TmCard *)-1) != 0) {
        tm_setFlips(arg0, who, arg0->srcX, arg0->srcY, TM_TASK_TURN, TM_FLIP_COMBO);
    } else {
        arg0->task = TM_TASK_TURN;
    }
}

void tm_taskTurn(TmState *arg0) {
    if (arg0->turn == TM_PLAYER) {
        arg0->turn = TM_ENEMY;
        arg0->timer = 0;
        arg0->phase = 0;
        arg0->thinkDelay = (rand() & 0xF) + 0xF;
        arg0->task = TM_TASK_ENEMY_THINK;
    } else {
        arg0->nameSlide = 0;
        arg0->turn = TM_PLAYER;
        arg0->task = TM_TASK_PLAYER_SELECT;
    }
}

void *tm_drawFlash(u32 *ot, TILE *p, TmState *s) {
    TmFlashTbl tbl;
    TmFlash *d;
    TmFlip *e;
    s32 i;
    s32 t;
    s32 c;
    s32 bx;
    s32 dx;
    s32 dy;
    s32 ex;
    s32 ey;
    s32 by;
    s32 x;
    s32 y;
    s32 w;
    s32 h;
    s32 k;

    tbl = g_tmFlashBars;
    h = 0;
    w = 0;
    x = 0;
    y = 0;
    e = s->flips;
    t = s->timer;
    if (s->phase == 0) {
        k = t * 30;
        if (k >= 0x100) {
            c = 0xFF;
            if (s->flashMode == TM_FLIP_COMBO) {
                s->comboTimer = 0x32;
                s->valuesTimer = 0;
                s->comboShown = s->comboCount;
                tm_playSe(TM_SE_COMBO);
            }
            s->phase = 1;
        } else {
            c = k;
        }
    } else {
        k = 0xFF - (t - 10) * 15;
        c = k;
        if (k < 0) {
            c = 0;
            if (s->flashNext == -1) {
                tm_checkBattles(s);
            } else {
                s->task = s->flashNext;
            }
        }
    }
    for (i = 0; i < 10; i++) {
        if (e[i].mode != TM_FLIP_NONE) {
            if (s->phase == 1) {
                tm_getCell(s, e[i].x, e[i].y)->card->owner = s->flipOwner;
            }
            if (s->flashMode == TM_FLIP_CHANGE) {
                setlen(p, 3);
                p->code = 0x62;
                setXY0(p, e[i].x * TM_SQUARE_W + TM_BOARD_X, e[i].y * TM_SQUARE_H + TM_BOARD_Y);
                setWH(p, TM_CARD_W, TM_CARD_H);
                setRGB0(p, c, c, c);
                AddPrim(ot + 0x1E, p);
                p++;
            }
            if (s->flashMode == TM_FLIP_COMBO) {
                k = 0xFF - t * 9;
                c = k;
                d = &tbl.v[e[i].arg];
                dx = d->dirX;
                dy = d->dirY;
                ex = d->cornerX;
                ey = d->cornerY;
                bx = e[i].x * TM_SQUARE_W + TM_BOARD_X + d->offX;
                by = e[i].y * TM_SQUARE_H + TM_BOARD_Y + d->offY;
                if (k < 0) {
                    c = 0;
                }
                switch (d->shape) {
                case 0:
                    x = bx;
                    w = 0x2A;
                    h = 4;
                    y = by - (t * 51 / 28) * dy;
                    break;
                case 1:
                    x = bx + t * 42 / 28 * dx;
                    w = t * 42 / 28;
                    h = 4;
                    y = by - (t * 51 / 28) * dy;
                    if (dy == -1) {
                        y -= 4;
                    }
                    setlen(p, 3);
                    p->code = 0x62;
                    setXY0(p, x, y);
                    setWH(p, w, h);
                    setRGB0(p, c, c, c);
                    AddPrim(ot + 0x1E, p);
                    p++;
                    x = bx + w * ex;
                    w = 4;
                    y = by - (t * 51 / 28) * ey;
                    h = t * 51 / 28;
                    if (ex == 1) {
                        x -= 4;
                    }
                    if (ey != 0) {
                        y += 4;
                    }
                    if (h >= 4) {
                        h -= 4;
                    }
                    break;
                case 2:
                    y = by;
                    x = bx + t * 42 / 28 * dx;
                    w = 4;
                    h = 0x33;
                    break;
                }
                setlen(p, 3);
                p->code = 0x62;
                setXY0(p, x, y);
                setWH(p, w, h);
                setRGB0(p, c, c, c);
                AddPrim(ot + 0x1E, p);
                p++;
            }
            if (s->flashMode == TM_FLIP_BATTLE) {
                setlen(p, 3);
                p->code = 0x62;
                setXY0(p, e[i].x * TM_SQUARE_W + TM_BOARD_X, e[i].y * TM_SQUARE_H + TM_BOARD_Y);
                setWH(p, TM_CARD_W, TM_CARD_H);
                if (s->timer & 4) {
                    setRGB0(p, 0x80, 0x80, 0x80);
                } else {
                    setRGB0(p, 0x76, 0x76, 0x76);
                }
                AddPrim(ot + 0x1E, p);
                p++;
            }
        }
    }
    /* Dead store, no code: a reference to k after case 2's `if (k < 0)` keeps cse from rewriting that test
     * to use c (make_regs_eqv picks the register whose last use comes later), so the test stays on k (a2). */
    k = 0;
    SetDrawTPage((DR_TPAGE *)p, 0, 0, GetTPage(0, 1, 0, 0));
    AddPrim(ot + 0x1E, p);
    p = (TILE *)((DR_TPAGE *)p + 1);
    return p;
}

void *tm_drawFlash2(u32 *ot, TILE *p, TmState *s) {
    s32 t;
    u8 c;
    s32 i;
    TmFlip *e;
    TmCard *card;

    e = s->flips;
    if (s->phase == 0) {
        c = t = s->timer * 30;
        if (t >= 0x100) {
            c = 0xFF;
            s->timer = 0;
            s->phase = 1;
        }
    } else {
        c = t = 0xFF - s->timer * 12;
        if (t < 0) {
            c = 0;
            if (s->result == TM_RESULT_WIN) {
                tm_returnHand(s);
            } else {
                tm_returnKept(s);
            }
            tm_nextTrade(s);
        }
    }
    for (i = 0; i < 10; i++) {
        if (e[i].mode == TM_FLIP_CHANGE) {
            card = e[i].card;
            if (s->phase == 1) {
                card->owner = e[i].arg;
            }
            setlen(p, 3);
            p->code = 0x62;
            p->x0 = card->x;
            p->y0 = card->y;
            p->w = 0x2A;
            p->h = 0x33;
            p->r0 = c;
            p->g0 = c;
            p->b0 = c;
            AddPrim(ot + card->depth - 1, p);
            p++;
            SetDrawTPage(p, 0, 0, GetTPage(0, 1, 0, 0));
            AddPrim(ot + card->depth - 1, p);
            p = (TILE *)((DR_TPAGE *)p + 1);
        }
    }
    return p;
}

void tm_taskFlashArrow(TmState *arg0) {
    if (arg0->timer >= 0x15) {
        arg0->timer = 0;
        arg0->task = TM_TASK_FLASH;
        tm_playSe(TM_SE_FLASH);
    }
}

void tm_clearFlips(TmState *arg0) {
    s32 i;
    TmFlip *p = arg0->flips;
    arg0->timer = 0;
    arg0->phase = 0;
    for (i = 0; i < 10; i++) {
        p[i].mode = TM_FLIP_NONE;
        p[i].x = 0;
        p[i].y = 0;
    }
}

void tm_layoutCards(TmState *s) {
    TmCard *a;
    TmCard *b;
    TmCard *c;
    TmCard *cards;
    s32 dx;
    s32 dy;
    s32 k;
    s32 x;
    s32 n;
    s32 i;
    s32 m;
    s32 t;

    dx = 0;
    dy = 0;
    a = NULL;
    b = NULL;
    k = 0;
    if (s->task == TM_TASK_BATTLE) {
        a = tm_getCell(s, s->srcX, s->srcY)->card;
        b = tm_getCell(s, s->dstX, s->dstY)->card;
        if ((u32)(s->timer - 0x23) < 0xF) {
            k = s->timer - 0x23;
            if (s->timer >= 0x2A) {
                k = 0x32 - s->timer;
            }
        } else {
            k = 0;
        }
        dx = (s->srcX - s->dstX) * 0x2A00 / 270;
        dy = (s->srcY - s->dstY) * 0x3300 / 270;
        k = k * k;
    }
    x = 8;
    cards = s->cards;
    for (i = 0, n = 0; i < TM_NUM_CARDS; i++) {
        c = &cards[i];
        if (s->task == TM_TASK_CHOICE || s->task == TM_TASK_GAME_QUIT || s->quitting != 0) {
            c->depth = i + 0x28;
        } else {
            c->depth = 0x3C - i;
        }
        if (c->owner == TM_PLAYER) {
            if (c->onBoard == 0) {
                c->small = 0;
                c->y = x;
                if (tm_countPlayerHand(s, TM_PLAYER) == TM_HAND_SIZE || s->task == TM_TASK_CHOICE || s->task == TM_TASK_GAME_QUIT || s->quitting != 0) {
                    x += 0x2A;
                    c->x = TM_PLAYER_HAND_X + 4;
                    c->small = 1;
                } else {
                    x += 0x34;
                    c->small = 0;
                    c->x = TM_PLAYER_HAND_X;
                }
            }
            if (s->task == TM_TASK_GET_SELECT && i >= 5) {
                if (tm_getCaptured(s, s->getCursor) == c) {
                    c->x = 0x16;
                } else {
                    c->x = TM_ENEMY_HAND_X;
                }
            }
            if (s->task == TM_TASK_FLASH2 && i >= 5) {
                c->x = TM_ENEMY_HAND_X;
            }
        }
        if (c->owner == TM_ENEMY && c->onBoard == 0) {
            c->y = n * 14 + 0x1A;
            n++;
            c->x = TM_ENEMY_HAND_X;
        }
        if (s->task == TM_TASK_BATTLE) {
            if (a == c) {
                t = s->srcX * 43;
                m = dx * k / 256 - 0x48;
                c->x = t - m;
                t = s->srcY * 52;
                m = dy * k / 256 - 8;
                c->y = t - m;
                c->depth = 0x26;
            }
            if (b == c) {
                t = s->dstX * 43;
                m = dx * k / 256 + 0x48;
                c->x = t + m;
                t = s->dstY * 52;
                m = dy * k / 256 + 8;
                c->y = t + m;
                c->depth = 0x27;
            }
        }
    }
}

void *tm_drawCards(u32 *ot, void *p, TmState *s) {
    TmCard *c;
    TmCard *cards;
    s32 i;
    s32 k;
    s32 m;
    s32 z;
    s32 sel;
    u8 a;
    u8 b;
    u32 *o;
    u32 *o2;

    cards = s->cards;
    for (i = 0; i < TM_NUM_CARDS; i++) {
        c = &cards[i];
        if (cards[i].visible == 0) {
            continue;
        }
        a = s->brightness;
        m = s->task;
        b = a;
        if (m == 6 && cards[i].onBoard == 1) {
            a = 0x40;
            b = a;
            for (k = 0; k < s->numTargets; k++) {
                if (&cards[i] == s->targets[k]) {
                    a = s->brightness - ((s->timer & 0x10) >> 4) * 24;
                    b = s->brightness;
                }
            }
            if (c == s->attacker) {
                a = s->brightness;
                b = a;
            }
        }
        if (m == 0x12 || m == 0x17 || m == 0x18 || m == 0x23 || s->quitting != 0) {
            if (i >= 5) {
                a -= (i - 5) * 22;
                b -= (i - 5) * 22;
            }
        }
        if (m == 1 && c->owner == TM_PLAYER && c->onBoard == 0) {
            b = 0x40;
            a = b;
            if (c == tm_getHandCard(s, TM_PLAYER, s->cursor)) {
                b = 0x80;
                a = b;
            }
        }
        z = c->depth;
        if (s->task == TM_TASK_BATTLE_SELECT && a != b) {
            o = ot + z;
            setlen((SPRT *)p, 4);
            ((SPRT *)p)->code = 0x64;
            ((SPRT *)p)->x0 = c->x + 5;
            ((SPRT *)p)->y0 = c->y + 0x15;
            ((SPRT *)p)->v0 = 0xF0;
            ((SPRT *)p)->w = 0x20;
            ((SPRT *)p)->h = 0x10;
            ((SPRT *)p)->u0 = 0x80;
            ((SPRT *)p)->clut = 0x3B40;
            ((SPRT *)p)->r0 = 0x80;
            ((SPRT *)p)->g0 = 0x80;
            ((SPRT *)p)->b0 = 0x80;
            AddPrim(o, p);
            p = (SPRT *)p + 1;
            setlen((DR_TPAGE *)p, 1);
            ((DR_TPAGE *)p)->code[0] = (GetTPage(0, 0, 0x340, 0x1E0) & 0x9FF) | 0xE1000000;
            AddPrim(o, p);
            p = (DR_TPAGE *)p + 1;
        }
        if (s->task == TM_TASK_GET_SELECT && tm_getCaptured(s, s->getCursor) == c) {
            z = 0x28;
        }
        if (c->hidden == 0) {
            sel = 0;
            if (s->task == TM_TASK_FLASH_ARROW) {
                sel = s->comboCard == c;
            }
            o2 = ot + z;
            p = tm_drawArrows(o2, p, s, c->x, c->y, c->arrows, c->small, b, sel);
            if (c->hidden == 0) {
                p = tm_drawCardStats(o2, p, s, c, b);
                if (c->hidden == 0) {
                    p = tm_drawCardFace(o2, p, s, c, a, c->small);
                }
            }
        }
        if (c->small == 0) {
            setlen((SPRT *)p, 4);
            ((SPRT *)p)->code = 0x64;
            ((SPRT *)p)->x0 = c->x;
            ((SPRT *)p)->y0 = cards[i].y;
            ((SPRT *)p)->w = 0x2A;
            ((SPRT *)p)->h = 0x33;
            ((SPRT *)p)->r0 = a;
            ((SPRT *)p)->g0 = a;
            ((SPRT *)p)->b0 = a;
            if (cards[i].owner == TM_PLAYER) {
                ((SPRT *)p)->u0 = 8;
            } else {
                ((SPRT *)p)->u0 = 0x38;
            }
            ((SPRT *)p)->v0 = 0x40;
            if (cards[i].hidden == 1) {
                ((SPRT *)p)->u0 = 0xC8;
                ((SPRT *)p)->v0 = 0x40;
            }
            ((SPRT *)p)->clut = 0x3A80;
            AddPrim(ot + z, p);
            p = (SPRT *)p + 1;
        } else {
            SetPolyFT4((POLY_FT4 *)p);
            ((POLY_FT4 *)p)->x0 = cards[i].x;
            ((POLY_FT4 *)p)->y0 = cards[i].y;
            ((POLY_FT4 *)p)->x1 = cards[i].x + 0x22;
            ((POLY_FT4 *)p)->y1 = cards[i].y;
            ((POLY_FT4 *)p)->x2 = cards[i].x;
            ((POLY_FT4 *)p)->y2 = cards[i].y + 0x29;
            ((POLY_FT4 *)p)->x3 = cards[i].x + 0x22;
            ((POLY_FT4 *)p)->y3 = cards[i].y + 0x29;
            ((POLY_FT4 *)p)->u0 = 8;
            ((POLY_FT4 *)p)->u1 = 0x32;
            ((POLY_FT4 *)p)->v0 = 0x40;
            ((POLY_FT4 *)p)->v1 = 0x40;
            ((POLY_FT4 *)p)->u2 = 8;
            ((POLY_FT4 *)p)->v2 = 0x73;
            ((POLY_FT4 *)p)->u3 = 0x32;
            ((POLY_FT4 *)p)->v3 = 0x73;
            ((POLY_FT4 *)p)->tpage = 0x9C;
            ((POLY_FT4 *)p)->r0 = a;
            ((POLY_FT4 *)p)->g0 = a;
            ((POLY_FT4 *)p)->b0 = a;
            ((POLY_FT4 *)p)->clut = 0x3A80;
            AddPrim(ot + z, p);
            p = (POLY_FT4 *)p + 1;
        }
        setlen((DR_TPAGE *)p, 1);
        ((DR_TPAGE *)p)->code[0] = (GetTPage(1, 0, 0x300, 0x100) & 0x9FF) | 0xE1000000;
        AddPrim(ot + z, p);
        p = (DR_TPAGE *)p + 1;
    }
    return p;
}

void tm_startCardMove(TmState *arg0) {
    s32 i;
    TmCard *c = arg0->cards;
    for (i = 0; i < TM_NUM_CARDS; i++) {
        c[i].fromX = c[i].x << 8;
        c[i].fromY = c[i].y << 8;
        if (i < TM_HAND_SIZE) {
            c[i].toX = (TM_PLAYER_HAND_X << 8);
            c[i].toY = i * 0x2000 + 0x1900;
        } else {
            c[i].toX = TM_ENEMY_HAND_X << 8;
            c[i].toY = (i - 5) * 0x2000 + 0x1900;
        }
    }
    arg0->moveFrames = 0x14;
    arg0->timer = 0;
    arg0->task = TM_TASK_CARD_MOVE;
}

void *tm_taskCardMove(u32 *ot, void *p, TmState *s) {
    TmCard *c;
    TmCard *cards;
    TmFlip *e;
    s32 i;
    s32 j;
    s32 k;
    s32 skip;
    s32 flag;
    s32 n;
    s32 a;
    s32 b;
    s32 st;

    if (s->timer < s->moveFrames) {
        cards = s->cards;
        for (i = 0; i < TM_NUM_CARDS; i++) {
            c = &cards[i];
            a = (c->toX - c->fromX) / s->moveFrames;
            b = (c->toY - c->fromY) / s->moveFrames;
            c->x = (c->fromX + a * s->timer) >> 8;
            c->y = (c->fromY + b * s->timer) >> 8;
        }
    } else {
        cards = s->cards;
        for (i = 0; i < TM_NUM_CARDS; i++) {
            if (i < TM_HAND_SIZE) {
                cards[i].x = TM_PLAYER_HAND_X;
                cards[i].y = i * 32 + 0x19;
            } else {
                cards[i].x = TM_ENEMY_HAND_X;
                cards[i].y = i * 32 - 0x87;
            }
        }
    }
    if (s->moveFrames + 10 < s->timer) {
        if (s->result == TM_RESULT_WIN) {
            if (tm_countOwned(s, TM_PLAYER) != 10 && tm_countCaptured(s) != 1) {
                s->getCursor = 0;
                st = TM_TASK_GET_SELECT;
                goto done;
            }
        }
        if (s->result == TM_RESULT_WIN) {
            if (s->hasLostCard != 0 && (&s->cards[TM_HAND_SIZE])[s->lostSlot].owner == TM_PLAYER) {
                s->hasLostCard = 0;
            }
        }
        flag = 0;
        if (s->result == TM_RESULT_WIN) {
            n = tm_countOwned(s, TM_PLAYER);
            skip = -1;
            if (n == 10) {
                s->hasLostCard = 0;
            }
        } else {
            n = tm_countKept(s, TM_PLAYER);
            skip = (rand() & 0xFF) % (5 - n);
        }
        cards = s->cards;
        e = s->flips;
        tm_clearFlips(s);
        k = 0;
        j = 0;
        for (i = 0; i < TM_NUM_CARDS; i++) {
            c = &cards[i];
            if (i < TM_HAND_SIZE) {
                if (c->owner == TM_ENEMY) {
                    if (k != skip && tm_countOwned(s, TM_ENEMY) != 10) {
                        e[j].card = c;
                        e[j].mode = TM_FLIP_CHANGE;
                        e[j].arg = 0;
                        j++;
                        flag = 1;
                    }
                    if (s->result == TM_RESULT_LOSE) {
                        k++;
                    }
                }
            } else if (c->owner == TM_PLAYER) {
                st = s->result;
                if (st == 1) {
                    e[j].card = c;
                    e[j].mode = st;
                    e[j].arg = st;
                    j++;
                    flag = 1;
                }
            }
        }
        if (flag == 0) {
            if (s->result == TM_RESULT_WIN) {
                tm_returnHand(s);
            } else {
                tm_returnKept(s);
            }
            tm_nextTrade(s);
            return p;
        }
        s->flashMode = TM_FLIP_CHANGE;
        st = TM_TASK_FLASH2;
    done:
        s->task = st;
    }
    return p;
}

s32 tm_countCapturedBeforeLost(TmState *arg0) {
    s32 n = 0;
    s32 i;
    TmCard *c = arg0->cards;
    for (i = 0; i < arg0->lostSlot; i++) {
        if (c[i + 5].owner == TM_PLAYER) {
            n++;
        }
    }
    return n;
}

s32 tm_countKept(TmState *arg0, s32 arg1) {
    s32 i;
    s32 n;
    TmCard *c;
    if (arg1 == 0) {
        c = &arg0->cards[0];
    } else {
        c = &arg0->cards[TM_HAND_SIZE];
    }
    n = 0;
    for (i = 0; i < TM_HAND_SIZE; i++) {
        if (c[i].owner == arg1) {
            n++;
        }
    }
    return n;
}

void tm_nextTrade(TmState *s) {
    RECT r;
    s32 owner;
    s32 x;
    s32 i;
    s32 found;
    s32 j;
    TmCard *c;

    owner = 1;
    if (s->result == TM_RESULT_WIN) {
        owner = 0;
        j = 5;
        x = TM_ENEMY_HAND_X;
    } else {
        j = 0;
        x = TM_PLAYER_HAND_X;
    }
    found = 0;
    c = s->cards;
    for (i = 0; i < TM_HAND_SIZE; i++) {
        if ((c + j)[i].owner == owner && (c + j)[i].x == x) {
            found = 1;
            s->tradeCard = &(c + j)[i];
            s->moveFrames = 0xA;
            s->timer = 0;
            (c + j)[i].fromX = (c + j)[i].x << 8;
            (c + j)[i].fromY = (c + j)[i].y << 8;
            break;
        }
    }
    if (found == 0) {
        if (card_countAll() > 100) {
            r.w = text_measureWidth(text_get(TM_TEXT_OVER_100)) + 0x12;
            r.x = 0xA0 - r.w / 2;
            r.y = 0x5B;
            r.h = 0x2A;
            s->infoWin = win_openFree(&r, 5, 2);
            win_setDrawContent(s->infoWin, tm_drawOver100Win);
            win_setShadow(s->infoWin, 1);
            tm_waitWindow(s->infoWin);
            tm_hideCards(s);
            tm_resetStockCursor(s);
            s->task = TM_TASK_AWAY_INFO;
            tm_playSe(TM_SE_BUZZER);
        } else {
            tm_askRematch(s);
        }
    } else {
        s->task = TM_TASK_CARD_CHANGE;
        tm_playSe(TM_SE_CARD);
    }
}

void tm_drawOver100Win(Window *arg0) {
    text_drawBlock(arg0->unk0 + 9, arg0->unk2 + 7, arg0->unk18, text_get(TM_TEXT_OVER_100), 0);
}

void tm_drawDiscardTitle(Window *arg0) {
    text_drawBlock(0x82, 0x10, arg0->unk18, text_get(TM_TEXT_DISCARD), 0);
}

void *tm_taskCardChange(u32 *ot, void *p, TmState *s) {
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

    c = s->tradeCard;
    if (s->timer < s->moveFrames) {
        dx = (0x8B00 - c->fromX) / s->moveFrames;
        dy = (0x5500 - c->fromY) / s->moveFrames;
        c->x = (c->fromX + dx * s->timer) >> 8;
        c->y = (c->fromY + dy * s->timer) >> 8;
        tm_stackHands(s, 0);
        return p;
    }
    if (s->result == TM_RESULT_WIN) {
        g_tmTradeBadge = card_addWithStats(c->id, c->arrows, c->power, c->type, c->pdef, c->mdef);
    } else {
        s->lostCard.id = c->id;
        s->lostCard.power = c->power;
        s->lostCard.arrows = c->arrows;
        s->lostCard.type = c->type;
        s->lostCard.pdef = c->pdef;
        s->lostCard.mdef = c->mdef;
        s->hasLostCard = 1;
        g_tmTradeBadge = 0;
        if (card_countOf(c->id) == 0) {
            found = NULL;
            cards = s->cards;
            for (i = 0; i < TM_HAND_SIZE; i++) {
                if (cards[i].id == c->id) {
                    found = &cards[i];
                }
            }
            if (found == c) {
                g_tmTradeBadge = 1;
            }
        }
    }
    n = s->result;
    c->x = 0x8B;
    c->y = 0x55;
    g_tmTradeLost = n;
    g_tmTradeCardId = id = c->id;
    text_copy(buf, (u8 *)card_getName(id));
    if (g_tmTradeLost == 0) {
        text_append(buf, text_get(TM_TEXT_RECEIVED));
    } else {
        text_append(buf, text_get(TM_TEXT_LOST));
    }
    text_append(buf, text_get(TM_TEXT_TRADE_END));
    w = text_measureWidth(buf) + 0x10;
    r.x = 0xA0 - w / 2;
    r.y = 0x96;
    r.w = w;
    r.h = 0x1A;
    s->msgWin = win_openFree(&r, 5, 2);
    win_setDrawContent(s->msgWin, tm_drawTradeWin);
    win_setShadow(s->msgWin, 1);
    tm_waitWindow(s->msgWin);
    tm_playSe(s->result == TM_RESULT_WIN ? TM_SE_RECEIVED : TM_SE_LOST);
    s->task = TM_TASK_TRADE;
    return p;
}

void *tm_drawTradeBadge(u32 *ot, SPRT *p, TmState *s) {
    u8 b;
    DR_TPAGE *tp;

    b = s->brightness;
    if (g_tmTradeBadge != 0) {
        setlen(p, 4);
        p->code = 0x64;
        p->x0 = 0xA3;
        p->y0 = 0x45;
        if (s->result == TM_RESULT_WIN) {
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

void tm_drawTradeWin(Window *w) {
    u8 buf[64];
    Window *v;

    v = w;
    if (g_tmTradeLost == 0) {
        text_copy(buf, text_get(TM_TEXT_RECEIVED));
    } else {
        text_copy(buf, text_get(TM_TEXT_LOST));
    }
    text_append(buf, (u8 *)card_getName(g_tmTradeCardId));
    text_append(buf, text_get(TM_TEXT_TRADE_END));
    text_drawBlock(0x9F - text_measureWidth((s32)buf) / 2, v->unk2 + 6, w->unk18, buf, 0);
}

void tm_endTrade(TmState *arg0) {
    TmCard *c = arg0->tradeCard;
    c->depth = 0x28;
    c->fromX = c->x << 8;
    c->fromY = c->y << 8;
    arg0->timer = 0;
    arg0->task = TM_TASK_CHANGE2;
    win_close(arg0->msgWin);
}

void *tm_taskChange2(u32 *ot, void *p, TmState *s) {
    TmCard *c;
    s32 dx;
    s32 dy;

    c = s->tradeCard;
    if (s->timer < s->moveFrames) {
        if (s->result == TM_RESULT_WIN) {
            dx = ((TM_PLAYER_HAND_X << 8) - c->fromX) / s->moveFrames;
        } else {
            dx = ((TM_ENEMY_HAND_X << 8) - c->fromX) / s->moveFrames;
        }
        dy = (0x9900 - c->fromY) / s->moveFrames;
        c->x = (c->fromX + dx * s->timer) >> 8;
        c->y = (c->fromY + dy * s->timer) >> 8;
    } else {
        c->y = 0x19;
        if (s->result == TM_RESULT_WIN) {
            c->x = TM_PLAYER_HAND_X;
        } else {
            c->x = TM_ENEMY_HAND_X;
        }
        tm_stackHands(s, 1);
        tm_nextTrade(s);
    }
    return p;
}

void tm_stackHands(TmState *s, s32 flag) {
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
            first = &s->cards[0];
            second = &s->cards[TM_HAND_SIZE];
            x = TM_PLAYER_HAND_X;
        } else {
            first = &s->cards[TM_HAND_SIZE];
            second = &s->cards[0];
            x = TM_ENEMY_HAND_X;
        }
        n = 0;
        c = s->cards;
        for (i = 0; i < TM_NUM_CARDS; i++) {
            if (c[i].x == x) {
                n++;
            }
        }
        step = 0x20;
        if (n >= 5) {
            step = 0x80 / (n - 1);
        }
        j = 0;
        for (i = 0; i < TM_HAND_SIZE; i++) {
            if (first[i].x == x) {
                if (flag != 0) {
                    first[i].depth = 0x3C - j;
                }
                first[i].y = j * step + 0x19;
                j++;
            }
        }
        for (i = 0; i < TM_HAND_SIZE; i++) {
            if (second[i].x == x) {
                if (flag != 0) {
                    second[i].depth = 0x3C - j;
                }
                second[i].y = j * step + 0x19;
                j++;
            }
        }
    }
}

s32 tm_countInHand(TmState *arg0, s32 arg1) {
    s32 n = 0;
    s32 i;
    TmCard *c = arg0->cards;
    for (i = 0; i < TM_NUM_CARDS; i++) {
        if (c[i].owner == arg1 && c[i].onBoard == 0) {
            n++;
        }
    }
    return n;
}

s32 tm_countPlayerHand(TmState *arg0, s32 arg1) {
    s32 n = 0;
    s32 i;
    TmCard *c = arg0->cards;
    for (i = 0; i < TM_HAND_SIZE; i++) {
        if (c[i].visible != 0 && c[i].owner == arg1 && c[i].onBoard == 0) {
            n++;
        }
    }
    return n;
}

s32 tm_countCaptured(TmState *arg0) {
    s32 n = 0;
    s32 i;
    TmCard *c = arg0->cards + 5;
    for (i = 0; i < TM_HAND_SIZE; i++) {
        if (c[i].owner == TM_PLAYER) {
            n++;
        }
    }
    return n;
}

TmCard *tm_getCaptured(TmState *arg0, s32 arg1) {
    s32 i;
    TmCard *base;
    TmCard *c;
    i = 0;
    c = base = &arg0->cards[TM_HAND_SIZE];
    for (; i < TM_HAND_SIZE; i++, c++) {
        if (c->owner == TM_PLAYER) {
            if (arg1 == 0) {
                return c;
            }
            arg1--;
        }
    }
    return base;
}

s32 tm_countOnBoard(TmState *arg0, s32 arg1) {
    s32 n = 0;
    s32 i;
    TmCard *c = arg0->cards;
    for (i = 0; i < TM_NUM_CARDS; i++) {
        if (c[i].onBoard == 1 && c[i].owner == arg1) {
            n++;
        }
    }
    return n;
}

s32 tm_countOwned(TmState *arg0, s32 arg1) {
    s32 n = 0;
    s32 i;
    TmCard *c = arg0->cards;
    for (i = 0; i < TM_NUM_CARDS; i++) {
        if (c[i].owner == arg1) {
            n++;
        }
    }
    return n;
}

TmCard *tm_getHandCard(TmState *arg0, s32 arg1, s32 arg2) {
    s32 n = 0;
    s32 i;
    TmCard *c = arg0->cards;
    for (i = 0; i < TM_NUM_CARDS; i++) {
        if (c[i].owner == arg1 && c[i].onBoard == 0) {
            if (arg2 == n) {
                return &c[i];
            }
            n++;
        }
    }
    return NULL;
}

void tm_initHands(TmState *s) {
    TmCard *c;
    TmCard *base;
    TmCardData *tbl;
    s32 v;
    s32 i;
    s32 r;
    u8 id;

    tbl = g_tmCardData;
    base = s->cards;

    for (i = 0; i < TM_NUM_CARDS; i++) {
        c = &base[i];
        c->x = 0xDC;
        c->y = i * 30;
        c->owner = TM_PLAYER;
        c->onBoard = 0;
        c->hidden = 0;
        c->depth = 0x28;
        c->id = 0;
        c->arrows = rand();
        c->power = rand();
        r = rand() & 3;
        switch (r) {
        case 0:
            c->type = 0;
            break;
        case 1:
            c->type = r;
            break;
        case 2:
            c->type = 3;
            break;
        case 3:
            c->type = 2;
            break;
        }
        c->pdef = rand();
        c->mdef = rand();
        id = i;
        c->id = id;
        c->arrows = rand();
        c->power = tbl[id].power;
        c->type = tbl[id].type;
        c->pdef = tbl[id].pdef;
        v = tbl[id].mdef;
        c->visible = 0;
        c->mdef = v;
    }
    for (i = TM_HAND_SIZE; i < TM_NUM_CARDS; i++) {
        base[i].visible = 0;
    }
}

void tm_dealEnemyHand(TmState *s) {
    s32 i;
    s32 r;
    s32 k;
    u8 id;
    TmCard *c;
    TmCard *base;
    TmDeck *odds;

    odds = g_tmDecks;
    s->lostSlot = rand() % 5;
    base = &s->cards[TM_HAND_SIZE];
    for (i = 0; i < TM_HAND_SIZE; i++) {
        c = &base[i];
        c->x = TM_ENEMY_HAND_X;
        c->y = 0x12C;
        c->owner = TM_ENEMY;
        c->onBoard = 0;
        c->hidden = 1;
        c->small = 0;
        c->depth = 0x28;
        r = rand() & 0xFF;
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
        id = odds[s->deck].ids[k];
        if (s->autoPlay != 0) {
            id = rand() % 100;
        }
        if (i == s->lostSlot && s->hasLostCard != 0) {
            c->id = s->lostCard.id;
            c->power = s->lostCard.power;
            c->arrows = s->lostCard.arrows;
            c->type = s->lostCard.type;
            c->pdef = s->lostCard.pdef;
            c->mdef = s->lostCard.mdef;
        } else {
            tm_makeCard(s, c, id, 1);
        }
        c->visible = 1;
    }
}

TmCard *tm_getCardAt(TmState *arg0, s32 arg1, s32 arg2) {
    return tm_getCell(arg0, arg1, arg2)->card;
}

INCLUDE_RODATA("asm/us/ovl_0d/nonmatchings/tm_game", g_tmFlashBars);

/* "NO" label */
INCLUDE_RODATA("asm/us/ovl_0d/nonmatchings/tm_game", g_tmNoLabel);

void tm_showStockCard(TmState *s) {
    u8 buf[0x20];
    TmCard *c;
    TmCard *c0;
    TmCard *p;
    TmCard *q;
    Unk6 *d;
    s32 i;
    s32 n;
    s32 m;
    s32 k;
    s32 dx;
    s32 w;

    q = s->cards;
    for (i = 0; i < TM_HAND_SIZE; i++) {
        p = &q[i];
        if (i < s->handCount) {
            p->visible = 1;
        } else {
            p->visible = 0;
        }
    }
    for (i = 4, q = &s->cards[9]; i >= 0; i--, q--) {
        q->visible = 0;
    }
    i = 5;
    if (s->handCount != i && s->task != TM_TASK_END_FADE) {
        c0 = (TmCard *)tm_getStockCursorId(s);
        n = card_countOf((s32)c0);
        if (n != 0) {
            c0 = (TmCard *)tm_getStockCursorId(s);
            text_formatNumber((s32)c0 + 1, buf, -1, 3);
            i = text_measureDigits(buf) + 0xE;
            w = i / 2;
            ui_drawSmallDigits(0xD8 - w, 0x78, 0xFFA, g_tmNoLabel, 0);
            ui_drawDigits(0xE6 - w, 0x76, 0xFFA, buf, 0);
            m = n;
            k = s->copyIdx;
            c0 = &s->cards[TM_HAND_SIZE];
            if (m >= 6) {
                m = 5;
            }
            for (i = 0; i < m; i++) {
                c = &c0[i];
                dx = 0;
                if (s->scrollTimer != 0) {
                    if (s->phase == 0) {
                        dx = s->scrollTimer - 4;
                        if (i == 0) {
                            dx = 0;
                        }
                    } else {
                        dx = -s->scrollTimer + 1;
                    }
                    if (i == 0) {
                        dx = 0;
                    }
                }
                d = card_find(tm_getStockCursorId(s), k);
                c->x = 0xC2 + i * 4 + dx;
                c->y = i + 0x81;
                c->depth = i + 0x28;
                c->visible = 1;
                c->small = 0;
                c->onBoard = 1;
                c->owner = TM_PLAYER;
                c->id = d->unk0;
                if (i == 0) {
                    c0->arrows = d->unk1;
                } else {
                    c->arrows = 0;
                }
                c->power = d->unk2;
                c->type = d->unk3;
                c->pdef = d->unk4;
                c->mdef = d->unk5;
                k++;
                c->visible = 1;
                if (k == n) {
                    k = 0;
                }
            }
            i = card_getNameWidth(tm_getStockCursorId(s));
            text_drawBlock(0xD9 - i / 2, 0xC7, 0xFFA, (u8 *)card_getName(tm_getStockCursorId(s)), 0);
        }
    } else {
        s->cards[5].visible = 0;
    }
}

void tm_returnKept(TmState *arg0) {
    s32 i;
    TmCard *c = arg0->cards;
    for (i = 0; i < TM_HAND_SIZE; i++) {
        if (c[i].owner == TM_PLAYER) {
            card_addWithStats(c[i].id, c[i].arrows, c[i].power, c[i].type, c[i].pdef, c[i].mdef);
        }
    }
}

void tm_returnHand(TmState *arg0) {
    s32 i;
    TmCard *c = arg0->cards;
    for (i = 0; i < TM_HAND_SIZE; i++) {
        if (arg0->result == TM_RESULT_WIN) {
            tm_levelUpRandom(arg0, &c[i]);
        }
        card_addWithStats(c[i].id, c[i].arrows, c[i].power, c[i].type, c[i].pdef, c[i].mdef);
    }
}

void tm_addRandomCard(arg0, arg1)
    TmState *arg0;
    u8 arg1;
{
    TmCard c;
    if (card_countAll() < 0x64) {
        tm_makeCard(arg0, &c, arg1, 1);
        card_addWithStats(arg1, c.arrows, c.power, c.type, c.pdef, c.mdef);
    }
}

s32 tm_getStockCursorId(TmState *arg0) {
    return arg0->stockCol * 10 + arg0->stockRow;
}

void tm_hideCards(TmState *arg0) {
    s32 i;
    for (i = 9; i >= 0; i--) {
        arg0->cards[i].visible = 0;
    }
}

void tm_showCards(TmState *arg0) {
    s32 i;
    s32 one = 1;
    for (i = 9; i >= 0; i--) {
        arg0->cards[i].visible = one;
    }
}

void tm_levelUpStat(TmState *s, TmCard *c, s32 stat) {
    TmCard base;

    tm_makeCard(s, &base, c->id, 0);
    if (stat == 0 && c->power != base.power) {
        c->power++;
    }
    if (stat == 1 && c->pdef != base.pdef) {
        c->pdef++;
    }
    if (stat == 2 && c->mdef != base.mdef) {
        c->mdef++;
    }
    printf("AP=%x : PD=%x : MD=%x \n", c->power, c->pdef, c->mdef);
}

void tm_levelUpRandom(TmState *s, TmCard *c) {
    tm_levelUpStat(s, c, rand() % 3);
}

void tm_makeCard(TmState *s, TmCard *c, s32 arg2, s32 random) {
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

    dirs = g_tmArrowBits;
    tbl = g_tmCardData;
    c->id = arg2;
    id = arg2;
    if (random == 0) {
        c->power = tbl[(u8)id].power;
        c->pdef = tbl[(u8)id].pdef;
        c->mdef = tbl[(u8)id].mdef;
    } else {
        v = tbl[(u8)id].power >> 1;
        if (v != 0) {
            c->power = v + rand() % v;
        } else {
            c->power = 0;
        }
        v = tbl[(u8)id].pdef >> 1;
        if (v != 0) {
            c->pdef = v + rand() % v;
        } else {
            c->pdef = 0;
        }
        v = tbl[(u8)id].mdef >> 1;
        if (v != 0) {
            c->mdef = v + rand() % v;
        } else {
            c->mdef = 0;
        }
    }
    r = rand() % 100;
    p0 = s->arrowOdds[0];
    p1 = s->arrowOdds[1];
    p2 = s->arrowOdds[2];
    p3 = s->arrowOdds[3];
    p4 = s->arrowOdds[4];
    p5 = s->arrowOdds[5];
    p6 = s->arrowOdds[6];
    p7 = s->arrowOdds[7];
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
    c->arrows = 0;
    for (i = 0; i < n; i++) {
        done = 0;
        while (done == 0) {
            bp = &dirs.v[rand() & 7];
            if (!(c->arrows & *bp)) {
                c->arrows |= *bp;
                done = 1;
            }
        }
    }
    c->type = tbl[(u8)id].type;
    if (!(rand() & 0x7F)) {
        c->type = 2;
    }
}

void tm_taskEnemyAppear(TmState *s) {
    s32 i;
    s32 t;
    s32 d;
    TmCard *c;

    c = &s->cards[TM_HAND_SIZE];
    for (i = 0; i < TM_HAND_SIZE; i++) {
        d = s->timer - 0xF;
        t = i * 6 - d;
        if (t <= 0) {
            t = 0;
            if (s->landed[i] == 0) {
                s->landed[i] = 1;
                tm_playSe(TM_SE_CARD);
            }
        }
        c[i].depth = 0x3C - i;
        c[i].y = t * t + i * 0xE + 0x1A;
    }
    if (s->timer >= 0x29 && s->blocksDone != 0) {
        s->timer = 0;
        s->phase = 0;
        if (rand() & 1) {
            s->turn = TM_PLAYER;
        } else {
            s->turn = TM_ENEMY;
        }
        s->task = TM_TASK_COIN;
        tm_playSe(TM_SE_COIN);
    }
}

void tm_animEnemyThink(TmState *s) {
    s32 n;
    s32 i;
    TmCard *c;

    n = 0;
    c = &s->cards[TM_HAND_SIZE];
    for (i = 0; i < TM_HAND_SIZE; i++) {
        if (c[i].onBoard == 0) {
            c[i].x = TM_ENEMY_HAND_X;
            if (s->phase == 1) {
                if (n == s->timer / 4) {
                    c[i].x = 0x1A;
                }
            } else if (n == 0) {
                c[i].x = 0x1A;
            }
            n++;
        }
    }
    if (s->timer != 0 && s->phase == 1 && !(s->timer & 3)) {
        tm_playSe(TM_SE_CARD);
    }
}

void *tm_drawCardStats(u32 *ot, SPRT *p, TmState *s, TmCard *card, u8 c) {
    s32 x;
    s32 y;

    x = card->x;
    y = card->y;
    if (card->small != 0) {
        x -= 5;
        y -= 13;
    } else {
        y -= 2;
    }
    p = tm_drawStatGlyph(ot, p, (s32)s, x + 9, y + 0x28, card->power >> 4, c);
    switch (card->type) {
    case 0:
        p = tm_drawStatGlyph(ot, p, (s32)s, x + 15, y + 0x28, 0x10, c);
        break;
    case 1:
        p = tm_drawStatGlyph(ot, p, (s32)s, x + 15, y + 0x28, 0x11, c);
        break;
    case 2:
        p = tm_drawStatGlyph(ot, p, (s32)s, x + 15, y + 0x28, 0x12, c);
        break;
    case 3:
        p = tm_drawStatGlyph(ot, p, (s32)s, x + 15, y + 0x28, 0xA, c);
        break;
    }
    p = tm_drawStatGlyph(ot, p, (s32)s, x + 0x15, y + 0x28, card->pdef >> 4, c);
    p = tm_drawStatGlyph(ot, p, (s32)s, x + 0x1B, y + 0x28, card->mdef >> 4, c);
    setlen((DR_TPAGE *)p, 1);
    ((DR_TPAGE *)p)->code[0] = (GetTPage(1, 0, 0x300, 0x100) & 0x9FF) | 0xE1000000;
    AddPrim(ot, p);
    return (DR_TPAGE *)p + 1;
}

SPRT *tm_drawStatGlyph(void *ot, SPRT *p, s32 unused, s32 x, s32 y, s32 n, s32 c) {
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

void tm_checkGameEnd(TmState *s) {
    s32 a;
    s32 b;

    if (tm_countInHand(s, TM_PLAYER) == 0 && tm_countInHand(s, TM_ENEMY) == 0) {
        a = tm_countOnBoard(s, TM_PLAYER);
        b = tm_countOnBoard(s, TM_ENEMY);
        if (a > b) {
            func_80062750(0);
            card_addWin();
            s->result = TM_RESULT_WIN;
            s->wins++;
        }
        if (a < b) {
            func_80062750(1);
            card_addLoss();
            s->result = TM_RESULT_LOSE;
            s->losses++;
        }
        if (a == b) {
            func_80062750(2);
            card_addDraw();
            s->result = TM_RESULT_DRAW;
            s->draws++;
        }
        s->timer = 0;
        s->task = TM_TASK_RESULT;
    }
}

void *tm_drawResult(u32 *ot, SPRT *p, TmState *s) {
    u8 c;
    s32 snd;

    c = s->timer * 8;
    if (s->timer * 8 >= 0x100) {
        c = 0xFF;
    }
    if ((u8)(c - 0xF5) < 8) {
        switch (s->result) {
        case 0:
            if (tm_countOnBoard(s, TM_PLAYER) != 10) {
                snd = TM_SE_WIN;
            } else {
                snd = TM_SE_PERFECT;
            }
            break;
        case 1:
            snd = TM_SE_LOSE;
            break;
        default:
            snd = TM_SE_DRAW;
            break;
        }
        tm_playSe(snd);
    }
    switch (s->result) {
    case 0:
        if (tm_countOnBoard(s, TM_PLAYER) == 10) {
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

void tm_playSe(s32 arg0) {
    tm_soundCmd(0xD000, g_tmSeIds[arg0], 0, 0x80, 0x7F);
}

void tm_stopSe(s32 arg0) {
    tm_soundCmd(0x5100, g_tmSeIds[arg0], 0, 0, 0);
}

void tm_revealBlocks(TmState *arg0) {
    if (arg0->blocksDone == 0) {
        if (arg0->numBlocks != 0) {
            arg0->blocks[arg0->blockIdx].hidden = 0;
            if (arg0->blockTimer >= 9) {
                tm_playSe(TM_SE_BLOCK);
                arg0->blockTimer = 0;
                if (++arg0->blockIdx == arg0->numBlocks) {
                    arg0->blocksDone = 1;
                }
            }
        } else {
            arg0->blocksDone = 1;
        }
    }
    arg0->blockTimer++;
}

void *tm_drawBlocks(u32 *ot, void *p, TmState *s) {
    TmBlock *e;
    s32 i;
    s32 n;
    s32 dx;
    s32 dy;
    s32 dw;
    s32 dh;
    u8 c;
    u8 u;

    c = s->brightness;
    if (s->task == TM_TASK_FIELD_FADE) {
        c = s->timer * 8;
    }
    e = s->blocks;
    for (i = 0; i < 6; i++) {
        if (e[i].hidden != 0) {
            continue;
        }
        if (s->task == TM_TASK_ENEMY_APPEAR && s->blocksDone == 0 && i == s->blockIdx) {
            n = 10 - s->blockTimer;
            if (n < 0) {
                n = 0;
            }
            dx = n * 4 / 3;
            dy = n * 5 / 3;
            dw = dx + 0x2A;
            dh = dy + 0x33;
            SetPolyFT4((POLY_FT4 *)p);
            ((POLY_FT4 *)p)->x0 = e[i].x - dx;
            ((POLY_FT4 *)p)->y0 = e[i].y - dy;
            ((POLY_FT4 *)p)->x1 = e[i].x + dw;
            ((POLY_FT4 *)p)->y1 = e[i].y - dy;
            ((POLY_FT4 *)p)->x2 = e[i].x - dx;
            ((POLY_FT4 *)p)->y2 = e[i].y + dh;
            ((POLY_FT4 *)p)->x3 = e[i].x + dw;
            ((POLY_FT4 *)p)->y3 = e[i].y + dh;
            u = 0x98;
            if (e[i].style == 0) {
                u = 0x68;
            }
            ((POLY_FT4 *)p)->u0 = u;
            ((POLY_FT4 *)p)->v0 = 0x40;
            ((POLY_FT4 *)p)->u1 = u + 0x2A;
            ((POLY_FT4 *)p)->v1 = 0x40;
            ((POLY_FT4 *)p)->u2 = u;
            ((POLY_FT4 *)p)->v2 = 0x73;
            ((POLY_FT4 *)p)->u3 = u + 0x2A;
            ((POLY_FT4 *)p)->v3 = 0x73;
            ((POLY_FT4 *)p)->r0 = c;
            ((POLY_FT4 *)p)->g0 = c;
            ((POLY_FT4 *)p)->b0 = c;
            ((POLY_FT4 *)p)->tpage = 0x9C;
            ((POLY_FT4 *)p)->clut = 0x3A80;
            AddPrim(ot + 99, p);
            p = (POLY_FT4 *)p + 1;
        } else {
            setlen((SPRT *)p, 4);
            ((SPRT *)p)->code = 0x64;
            if (s->task == TM_TASK_FIELD_FADE) {
                ((SPRT *)p)->code = 0x66;
            }
            ((SPRT *)p)->x0 = e[i].x;
            ((SPRT *)p)->y0 = e[i].y;
            ((SPRT *)p)->r0 = c;
            ((SPRT *)p)->g0 = c;
            ((SPRT *)p)->b0 = c;
            if (e[i].style == 0) {
                ((SPRT *)p)->u0 = 0x68;
                ((SPRT *)p)->v0 = 0x40;
            } else {
                ((SPRT *)p)->u0 = 0x98;
                ((SPRT *)p)->v0 = 0x40;
            }
            ((SPRT *)p)->clut = 0x3A80;
            ((SPRT *)p)->w = 0x2A;
            ((SPRT *)p)->h = 0x33;
            AddPrim(ot + 100, p);
            p = (SPRT *)p + 1;
            setlen((DR_TPAGE *)p, 1);
            ((DR_TPAGE *)p)->code[0] = (GetTPage(1, 1, 0x300, 0x100) & 0x9FF) | 0xE1000000;
            AddPrim(ot + 100, p);
            p = (DR_TPAGE *)p + 1;
        }
    }
    return p;
}

void tm_placeBlocks(TmState *s) {
    s32 i;
    u32 r;
    s32 n;
    s32 x;
    s32 y;
    TmCell *cell;
    TmBlock *base;

    base = s->blocks;
    for (i = 0; i < 6; i++) {
        base[i].hidden = 1;
        base[i].x = 0;
    }
    r = rand() % 100;
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
    s->numBlocks = n;
    s->blockIdx = 0;
    for (i = 0; i < n; i++) {
        do {
            x = rand() & 3;
            y = rand() & 3;
            cell = tm_getCell(s, x, y);
        } while (cell->state != TM_CELL_EMPTY);
        cell->state = TM_CELL_BLOCK;
        base[i].x = x * TM_SQUARE_W + TM_BOARD_X;
        base[i].hidden = 1;
        base[i].y = y * TM_SQUARE_H + TM_BOARD_Y;
        base[i].style = rand() & 1;
    }
}

void *tm_taskEnemyThink(u32 *ot, void *p, TmState *s) {
    if (tm_countInHand(s, TM_ENEMY) == 0) {
        s->task = TM_TASK_ENEMY_SELECT;
    } else if (s->phase == 0) {
        if (s->timer > s->thinkDelay) {
            s->timer = 0;
            s->phase = 1;
            s->thinkDelay = (rand() % (tm_countInHand(s, TM_ENEMY) + 1)) * 4 - 2;
        }
    } else if (s->timer > s->thinkDelay) {
        s->task = TM_TASK_ENEMY_SELECT;
    }
    return p;
}

void tm_enemySelect(TmState *s) {
    s32 score[80];
    TmBattle res;
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

    if (tm_countInHand(s, TM_ENEMY) == 0) {
        return;
    }
    if (tm_countOwned(s, TM_ENEMY) == tm_countOwned(s, TM_PLAYER)) {
        wgt = 0x40;
        mul = 1;
    } else if (tm_countOwned(s, TM_ENEMY) >= tm_countOwned(s, TM_PLAYER)) {
        wgt = 0x43;
        mul = 1;
    } else {
        wgt = 1;
        mul = 5;
    }
    if (s->aiLevel == 0 && rand() % 100 >= 0x1D) {
        wgt = 0;
        mul = 0;
    }
    if (s->aiLevel == 1 && rand() % 100 >= 0x3F) {
        wgt = 0;
        mul = 0;
    }
    if (s->aiLevel == 2 && rand() % 100 >= 0x5D) {
        wgt = 0;
        mul = 0;
    }
    best = -0x7FFF;
    cnt = 0;
    k = 0;
    for (c = 0; c < tm_countInHand(s, TM_ENEMY); c++) {
        for (x = 0; x < 4; x++) {
            for (y = 0; y < 4; y++) {
                score[k] = 0;
                if (tm_getCell(s, x, y)->state != TM_CELL_EMPTY) {
                    score[k] = -0x7FFF;
                } else {
                    card = tm_getHandCard(s, TM_ENEMY, c);
                    v = tm_evalPlacement(s, 1, x, y, card);
                    if (v > 0) {
                        score[k] += v * wgt;
                    }
                    if (v < 0) {
                        tm_getBattleValues((s32)s, &res, card, tm_findDefender(s, x, y, card));
                        r = res.atk * 100 / (res.atk + res.def);
                        if (s->aiLevel == 3) {
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
    for (c = 0; c < tm_countInHand(s, TM_ENEMY); c++) {
        for (x = 0; x < 4; x++) {
            for (y = 0; y < 4; y++) {
                f = best < score[k];
                if (best == score[k] && rand() % 1000 < 1000 / cnt) {
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
    sel = tm_getHandCard(s, TM_ENEMY, bc);
    cell = tm_getCell(s, bx, by);
    cell->state = TM_CELL_CARD;
    cell->card = sel;
    sel->x = bx * TM_SQUARE_W + TM_BOARD_X;
    sel->y = by * TM_SQUARE_H + TM_BOARD_Y;
    sel->hidden = 0;
    sel->onBoard = 1;
    s->srcX = bx;
    s->srcY = by;
    tm_playSe(TM_SE_CARD);
    tm_checkBattles(s);
}

s32 tm_setCellOwner(TmState *arg0, s32 arg1, s32 arg2, s32 arg3) {
    TmCell *cell = tm_getCell(arg0, arg2, arg3);
    if (cell->state == TM_CELL_CARD) {
        TmCard *c = cell->card;
        if (c->owner != arg1) {
            c->owner = arg1;
        }
    }
    return 0;
}

s32 tm_evalPlacement(TmState *s, s32 owner, s32 x, s32 y, TmCard *c) {
    TmPos p;
    TmDirMask m = g_tmBackArrowBits;
    TmCell *cell;
    TmCard *t;
    s32 i;
    s32 win;
    s32 lose;
    u8 bits;

    win = 0;
    lose = 0;
    if (c == (TmCard *)-1) {
        c = tm_getCardAt(s, x, y);
    }
    bits = c->arrows;
    for (i = 0; i < 8; i++, bits >>= 1) {
        if (bits & 1) {
            p.x = x;
            p.y = y;
            tm_stepDir(&p, i);
            cell = tm_getCell(s, p.x, p.y);
            if (cell->state == TM_CELL_CARD) {
                t = cell->card;
                if (t->owner != owner) {
                    if (t->arrows & m.v[i]) {
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

TmCard *tm_findDefender(TmState *s, s32 x, s32 y, TmCard *c) {
    TmPos p;
    TmDirMask m = g_tmBackArrowBits;
    TmCell *cell;
    TmCard *t;
    s32 i;
    u8 bits;

    bits = c->arrows;
    for (i = 0; i < 8; i++, bits >>= 1) {
        if (bits & 1) {
            p.x = x;
            p.y = y;
            tm_stepDir(&p, i);
            cell = tm_getCell(s, p.x, p.y);
            if (cell->state == TM_CELL_CARD) {
                t = cell->card;
                if (t->owner != TM_ENEMY && (t->arrows & m.v[i])) {
                    return t;
                }
            }
        }
    }
    return (TmCard *)-1;
}

s32 tm_setFlips(TmState *s, s32 owner, s32 x, s32 y, s32 arg4, s32 mode) {
    TmPos p;
    TmCell *cell;
    TmCard *c;
    TmFlip *e;
    s32 i;
    u8 bits;
    u8 bit;

    tm_clearFlips(s);
    cell = tm_getCell(s, x, y);
    bit = 1;
    i = 0;

    c = cell->card;
    s->comboCount = 1;
    s->comboCard = c;
    bits = c->arrows;
    e = s->flips;
    s->comboDirs = 0;
    do {
        if (bits & 1) {
            p.x = x;
            p.y = y;
            tm_stepDir(&p, i);
            cell = tm_getCell(s, p.x, p.y);
            if (cell->state == TM_CELL_CARD && (c = cell->card)->owner != owner) {
                s->comboDirs |= bit;
                if (mode == TM_FLIP_COMBO) {
                    e[i].arg = i;
                    s->comboX = x * 43 + 0x39;
                    s->comboY = y * 52 + 0x19;
                    s->comboCount++;
                }
                e[i].mode = mode;
                e[i].x = p.x;
                e[i].y = p.y;
            }
        }
        bits >>= 1;
        bit <<= 1;
        i++;
    } while (i < 8);
    s->flipOwner = owner;
    s->flashNext = arg4;
    s->flashMode = mode;
    if (mode != TM_FLIP_COMBO) {
        s->task = TM_TASK_FLASH;
        tm_playSe(TM_SE_FLASH);
    } else {
        s->timer = 0;
        s->task = TM_TASK_FLASH_ARROW;
    }
    return 0;
}

TmCard *tm_getDefender(TmState *s, TmPos *p, s32 dir, s32 owner) {
    TmDirMask m = g_tmBackArrowBits;
    TmCell *cell;
    TmCard *c;

    tm_stepDir(p, dir);
    cell = tm_getCell(s, p->x, p->y);
    if (cell->state == TM_CELL_CARD) {
        c = cell->card;
        if (c->owner != owner && (c->arrows & m.v[dir])) {
            return c;
        }
    }
    return NULL;
}

void tm_stepDir(TmPos *p, s32 dir) {
    if (dir == 0) {
        p->y--;
    } else if (dir == 1) {
        p->x++;
        p->y--;
    } else if (dir == 2) {
        p->x++;
    } else if (dir == 3) {
        p->x++;
        p->y++;
    } else if (dir == 4) {
        p->y++;
    } else if (dir == 5) {
        p->x--;
        p->y++;
    } else if (dir == 6) {
        p->x--;
    } else if (dir == 7) {
        p->x--;
        p->y--;
    }
}

void *tm_drawArrows(u32 *ot, SPRT_16 *p, TmState *s, s32 x, s32 y, u8 mask, s32 flip, u8 c, s32 blink) {
    TmArrowTbl tbl;
    s32 i;
    u8 bits;
    u8 col;

    tbl = g_tmArrowSprites;
    bits = s->comboDirs;
    for (i = 0; i < 8; i++) {
        if (mask & 1) {
            setlen(p, 3);
            p->code = 0x74;
            if (flip == 0) {
                p->x0 = tbl.v[i].x + x;
                p->y0 = tbl.v[i].y + y;
            } else {
                p->x0 = tbl.v[i].smallX + x;
                p->y0 = tbl.v[i].smallY + y;
            }
            col = c;
            if (blink != 0 && (bits & 1)) {
                col = 0;
                if (!(s->timer & 2)) {
                    col = 0xFF;
                }
            }
            p->r0 = col;
            p->g0 = col;
            p->b0 = col;
            p->u0 = tbl.v[i].u;
            p->v0 = tbl.v[i].v;
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

void *tm_drawCursor(u32 *ot, void *p, TmState *s, s32 x, s32 y, s32 mode) {
    s32 z;
    u8 c;
    s32 u;
    u8 u2;
    u32 *o;

    if (mode != 2) {
        z = 3;
        if (mode != 0) {
            z = 6;
        }
        c = s->brightness;
        setlen((SPRT *)p, 4);
        ((SPRT *)p)->code = 0x64;
        if (mode != 0) {
            ((SPRT *)p)->code = 0x66;
        }
        ((SPRT *)p)->x0 = x;
        ((SPRT *)p)->y0 = y;
        ((SPRT *)p)->r0 = c;
        ((SPRT *)p)->g0 = c;
        ((SPRT *)p)->b0 = c;
        u = ((s->animFrame / 6) & 7) * 24;
        if (mode != 0) {
            u = ((s->savedFrame / 6) & 7) * 24;
        }
        o = ot + z;
        ((SPRT *)p)->u0 = u;
        ((SPRT *)p)->v0 = 0x80;
        ((SPRT *)p)->clut = 0x3AC0;
        ((SPRT *)p)->w = 0x18;
        ((SPRT *)p)->h = 0x10;
        AddPrim(o, p);
        p = (SPRT *)p + 1;
        ((SPRT *)p)->code = 0x66;
        setlen((SPRT *)p, 4);
        ((SPRT *)p)->x0 = x + 4;
        ((SPRT *)p)->y0 = y + 4;
        ((SPRT *)p)->r0 = 0;
        ((SPRT *)p)->g0 = 0;
        ((SPRT *)p)->b0 = 0;
        ((SPRT *)p)->u0 = u;
        ((SPRT *)p)->v0 = 0x80;
        ((SPRT *)p)->clut = 0x3AC0;
        ((SPRT *)p)->w = 0x18;
        ((SPRT *)p)->h = 0x10;
        AddPrim(o, p);
        p = (SPRT *)p + 1;
        setlen((DR_TPAGE *)p, 1);
        ((DR_TPAGE *)p)->code[0] = (GetTPage(1, 0, 0x300, 0x100) & 0x9FF) | 0xE1000000;
        AddPrim(o, p);
        p = (DR_TPAGE *)p + 1;
    } else {
        u = ((s->animFrame / 6) & 7) * 24;
        SetPolyFT4((POLY_FT4 *)p);
        setXY4((POLY_FT4 *)p, x, y, x + 0x18, y, x, y + 0x10, x + 0x18, y + 0x10);
        u2 = u + 0x17;
        setUV4((POLY_FT4 *)p, u2, 0x80, u, 0x80, u2, 0x90, u, 0x90);
        ((POLY_FT4 *)p)->clut = 0x3AC0;
        setRGB0((POLY_FT4 *)p, 0x80, 0x80, 0x80);
        ((POLY_FT4 *)p)->tpage = 0x9C;
        AddPrim(ot + 3, p);
        p = (POLY_FT4 *)p + 1;
        SetPolyFT4((POLY_FT4 *)p);
        setXY4((POLY_FT4 *)p, x + 4, y + 4, x + 0x1C, y + 4, x + 4, y + 0x14, x + 0x1C, y + 0x14);
        setUV4((POLY_FT4 *)p, u2, 0x80, u, 0x80, u2, 0x90, u, 0x90);
        ((POLY_FT4 *)p)->clut = 0x3AC0;
        setRGB0((POLY_FT4 *)p, 0, 0, 0);
        ((POLY_FT4 *)p)->tpage = 0x9C;
        setSemiTrans((POLY_FT4 *)p, 1);
        AddPrim(ot + 3, p);
        p = (POLY_FT4 *)p + 1;
    }
    return p;
}

TmCell *tm_getCell(TmState *arg0, s32 arg1, s32 arg2) {
    TmCell(*board)[4] = arg0->board;
    if (arg1 < 0) {
        return &g_tmOffBoardCell;
    }
    if (arg1 >= 4) {
        return &g_tmOffBoardCell;
    }
    if (arg2 < 0) {
        return &g_tmOffBoardCell;
    }
    if (arg2 >= 4) {
        return &g_tmOffBoardCell;
    }
    return &board[arg2][arg1];
}

void tm_initBoard(TmState *arg0) {
    s32 x, y;
    TmCell(*b)[4] = arg0->board;
    g_tmOffBoardCell.state = TM_CELL_OFFBOARD;
    for (y = 0; y < 4; y++) {
        for (x = 3; x >= 0; x--) {
            b[y][x].state = TM_CELL_EMPTY;
        }
    }
    tm_placeBlocks(arg0);
}

INCLUDE_RODATA("asm/us/ovl_0d/nonmatchings/tm_game", g_tmArrowBits);

INCLUDE_RODATA("asm/us/ovl_0d/nonmatchings/tm_game", g_tmBackArrowBits);

INCLUDE_RODATA("asm/us/ovl_0d/nonmatchings/tm_game", g_tmArrowSprites);

void *tm_drawStock(u32 *ot, SPRT *p, TmState *s) {
    u8 buf[0x20];
    s32 i;
    s32 n;
    s32 m;
    s32 k;
    s32 c;

    SetSprt(p);
    if ((u32)(s->task - TM_TASK_CHOICE) < 2 || s->task == TM_TASK_GAME_QUIT || s->task == TM_TASK_END_FADE) {
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
        if (!(s->timer & 0x20)) {
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
        if (!(s->timer & 0x20)) {
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
        m = n = card_countOf(i);
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
            k = card_getRarity(i);
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

    ui_drawSmallDigits(0x19, 0xCB, 0xFFA, "Stock:", 7);
    text_formatNumber(g_tmStockCount, buf, 3, 3);
    ui_drawDigits(0x43, 0xC9, 0xFFA, buf, 0);
    ui_drawSmallDigits(0x65, 0xCB, 0xFFA, "Type:", 7);
    text_formatNumber(g_tmStockTypes, buf, 3, 3);
    ui_drawDigits(0x89, 0xC9, 0xFFA, buf, 0);

    if ((u32)(s->task - TM_TASK_CHOICE) < 2 || s->task == TM_TASK_GAME_QUIT || s->task == TM_TASK_END_FADE) {

        ui_drawSmallDigits(0xBC, 0x42, 0xFFA, "WIN", 7);
        text_formatNumber(card_getWins(), buf, 4, 1);
        ui_drawSmallDigits(0xDC, 0x42, 0xFFA, buf, 0);
        ui_drawSmallDigits(0xBC, 0x4C, 0xFFA, "LOSE", 7);
        text_formatNumber(card_getLosses(), buf, 4, 1);
        ui_drawSmallDigits(0xDC, 0x4C, 0xFFA, buf, 0);
        ui_drawSmallDigits(0xBC, 0x56, 0xFFA, "DRAW", 7);
        text_formatNumber(card_getDraws(), buf, 4, 1);
        ui_drawSmallDigits(0xDC, 0x56, 0xFFA, buf, 0);

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
    if (s->handCount != TM_HAND_SIZE && s->task != TM_TASK_AWAY_INFO && card_countOf(tm_getStockCursorId(s)) >= 2) {
        text_formatNumber(s->copyIdx + 1, buf, 3, 1);
        ui_drawSmallDigits(0xBF, 0xBA, 0xFFA, buf, 0);
        ui_drawSmallDigits(0xD4, 0xBA, 0xFFA, g_tmSlash, 0);
        text_formatNumber(card_countOf(tm_getStockCursorId(s)), buf, -1, 1);
        ui_drawSmallDigits(0xDB, 0xBA, 0xFFA, buf, 0);
        text_drawBlock(0xB5, 0xB6, 0xFFA, text_get(TM_TEXT_ICON_L1), 0);
        text_drawBlock(0xF1, 0xB6, 0xFFA, text_get(TM_TEXT_ICON_R1), 0);
    }
    if (s->handCount == 0) {
        g_tmStockTypes = card_countKinds();
        g_tmStockCount = card_countAll();
    }
    return p;
}

void tm_openStock(TmState *arg0) {
    g_tmStockTypes = card_countKinds();
    g_tmStockCount = card_countAll();
    arg0->task = TM_TASK_CHOICE;
}

void tm_drawSelectTitle(Window *arg0) {
    text_drawBlock(0x8A, 0x10, arg0->unk18, text_get(TM_TEXT_SELECT_CARDS), 0);
}

void tm_resetStockCursor(TmState *arg0) {
    arg0->stockCol = 0;
    arg0->stockRow = 0;
    arg0->handCount = 0;
    arg0->copyIdx = 0;
}

void *tm_drawCardFace(u32 *ot, void *prim, TmState *s, TmCard *card, u8 c, s32 flip) {
    u8 id;
    s32 page;
    s32 n;
    s32 u;
    s32 v;

    id = card->id;
    page = (u8)(id / 30);
    n = (u8)(id % 30);
    u = (n % 6) * 42;
    v = (n / 6) * 51;
    if (flip == 0) {
        setlen((SPRT *)prim, 4);
        ((SPRT *)prim)->code = 0x64;
        ((SPRT *)prim)->x0 = card->x;
        ((SPRT *)prim)->y0 = card->y;
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
        ((POLY_FT4 *)prim)->x0 = card->x;
        ((POLY_FT4 *)prim)->y0 = card->y;
        ((POLY_FT4 *)prim)->x1 = card->x + 0x22;
        ((POLY_FT4 *)prim)->y1 = card->y;
        ((POLY_FT4 *)prim)->x2 = card->x;
        ((POLY_FT4 *)prim)->y2 = card->y + 0x29;
        ((POLY_FT4 *)prim)->x3 = card->x + 0x22;
        ((POLY_FT4 *)prim)->y3 = card->y + 0x29;
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

void *tm_taskCoin(u32 *ot, SPRT *p, TmState *s) {
    TmCoinFrames f = g_tmCoinFrames;
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
    t = s->timer;
    flip = 0;
    if (s->phase == 0) {
        frame = t & 7;
        if (t >= 0x29 && ((s->turn == TM_PLAYER && frame == 7) || (s->turn == TM_ENEMY && frame == 3))) {
            s->phase = 1;
            s->timer = 0;
            tm_stopSe(TM_SE_COIN);
        }
    } else {
        frame = 3;
        if (s->turn == TM_PLAYER) {
            frame = 7;
        }
        if (t >= 0x1F) {
            n = t - 0x1E;
            flip = 1;
            dy = -(n * 82) / 12;
            dx = (n * 124) / 12;
            if (s->turn == TM_ENEMY) {
                dy = -(n * 66) / 12;
                dx = (n * -119) / 12;
            }
        }
        if (t >= 0x2A) {
            if (s->turn == TM_PLAYER) {
                s->nameSlide = 0;
                s->task = TM_TASK_PLAYER_SELECT;
            } else {
                s->timer = 0;
                s->phase = 0;
                s->thinkDelay = (rand() & 0xF) + 0xF;
                s->task = TM_TASK_ENEMY_THINK;
            }
        }
    }
    SetSprt(p);
    if (flip != 0) {
        p->code |= 2;
    }
    fr = &f.v[frame];
    p->w = fr->w;
    p->h = 0x28;
    p->u0 = fr->u;
    p->v0 = fr->v;
    k = dx + 0x9D;
    p->x0 = fr->x + k;
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

void *tm_drawCombo(u32 *ot, SPRT *p, TmState *s) {
    u8 c;

    if (s->comboTimer != 0) {
        c = 0x80;
        if (s->comboShown >= 0x19) {
            c = s->comboTimer * 4 - 0x80;
        }
        SetSprt(p);
        p->w = 0x38;
        p->h = 0x10;
        p->u0 = 0xA0;
        p->v0 = 0xF0;
        p->x0 = s->comboX + 0x10;
        p->y0 = s->comboY;
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
        p->x0 = s->comboX + 0x12;
        p->y0 = s->comboY + 2;
        p->r0 = 0;
        p->g0 = 0;
        p->b0 = 0;
        p->clut = 0x3A40;
        AddPrim(ot + 10, p);
        p++;
        SetSprt(p);
        p->w = 0x10;
        p->h = 0x10;
        p->u0 = s->comboShown * 16;
        p->v0 = 0xF0;
        p->x0 = s->comboX;
        p->y0 = s->comboY;
        p->r0 = c;
        p->g0 = c;
        p->b0 = c;
        p->clut = 0x3A40;
        AddPrim(ot + 10, p);
        p++;
        SetSprt(p);
        p->w = 0x38;
        p->h = 0x10;
        p->u0 = s->comboShown * 16;
        p->v0 = 0xF0;
        p->x0 = s->comboX + 2;
        p->y0 = s->comboY + 2;
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
        s->comboTimer--;
    }
    return p;
}

void *tm_drawBattleValues(u32 *ot, void *vp, TmState *s) {
    SPRT *p = vp;
    TmCard *a;
    TmCard *b;
    s32 i;
    s32 n;
    s32 x;
    s32 y;

    if (s->valuesTimer != 0) {
        a = tm_getCardAt(s, s->srcX, s->srcY);
        b = tm_getCardAt(s, s->dstX, s->dstY);
        for (i = 0; i < 2; i++) {
            if (i == 0) {
                n = s->atkShown;
                x = a->x + 8;
                y = a->y + 0x12;
            } else {
                n = s->defShown;
                x = b->x + 8;
                y = b->y + 0x12;
            }
            if (n < 10) {
                x += 9;
            } else if (n < 100) {
                x += 4;
            }
            if (n >= 100) {
                p = tm_drawBigDigit(ot, p, x, y, n / 100);
                n %= 100;
                x += 9;
                p = tm_drawBigDigit(ot, p, x, y, n / 10);
                x += 9;
                n %= 10;
            } else if (n >= 10) {
                p = tm_drawBigDigit(ot, p, x, y, n / 10);
                x += 9;
                n %= 10;
            }
            p = tm_drawBigDigit(ot, p, x, y, n);
        }
        setlen((DR_TPAGE *)p, 1);
        ((DR_TPAGE *)p)->code[0] = (GetTPage(0, 0, 0x340, 0x1E0) & 0x9FF) | 0xE1000000;
        AddPrim(ot + 11, p);
        p = (SPRT *)((DR_TPAGE *)p + 1);
        s->valuesTimer--;
    }
    return p;
}

void *tm_drawBigDigit(u32 *ot, SPRT *p, s32 x, s32 y, s32 n) {
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

void *tm_drawTryAgain(u32 *ot, void *p, TmState *s) {
    return p;
}

void tm_askRematch(TmState *arg0) {
    RECT r;

    if ((u32)(arg0->opponent - TM_OPP_FORCED) >= 4) {
        r.w = text_measureWidth((s32)text_get(TM_TEXT_REMATCH)) + 0x12;
        r.x = 0xA0 - r.w / 2;
        r.y = 0x54;
        r.h = 0x39;
        arg0->msgWin = win_openFree(&r, 5, 2);
        win_setDrawContent(arg0->msgWin, tm_drawRematchWin);
        win_setShadow(arg0->msgWin, 1);
        tm_waitWindow(arg0->msgWin);
        arg0->cursor = 0;
        arg0->task = TM_TASK_TRY_AGAIN;
        return;
    }
    tm_startEndFade(arg0);
}

void tm_drawRematchWin(Window *arg0) {
    text_drawBlock(arg0->unk0 + 9, arg0->unk2 + 6, arg0->unk18, text_get(TM_TEXT_REMATCH), 0);
}

void tm_taskEndFade(TmState *arg0) {
    fade_setAngle((arg0->timer * 0x88) & 0xFF8);
    if (arg0->timer >= 0x1F) {
        arg0->task = TM_TASK_EXIT;
    }
}

void tm_startEndFade(TmState *arg0) {
    arg0->timer = 0;
    game_startFade(2, 1, 0x1E, 0xFFFFFF);
    fade_setAngle(0);
    tm_soundCmd(0xC401, 0x42, 0x1E, 0x7F, 0);
    arg0->task = TM_TASK_END_FADE;
}

INCLUDE_RODATA("asm/us/ovl_0d/nonmatchings/tm_game", g_tmSlash);

INCLUDE_RODATA("asm/us/ovl_0d/nonmatchings/tm_game", g_tmCoinFrames);
