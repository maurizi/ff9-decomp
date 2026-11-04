#include "ovl_0d.h"

void tm_mainLoop(void) {
    /* tm.animFrame is never initialised: on the PS1 it starts as stack residue, the first row of ovl_0a
     * fld_drawShadow's MATRIX local from the last character shadow the field drew (e.g. 0x0000FFD6 from Dali 354's
     * shopkeeper, 0x00450042 in the enter_tm fixtures), and only picks the hand cursor's animation frame.
     * timer, turn and comboDirs also start as residue but nothing reads them before they are set (F42). */
    TmState tm;
    GameData *ctx;
    void *prim;

    tm.unk5C8.x = 0x32;
    tm.unk5C8.y = 0x1E;
    tm.unk5C8.w = 0xC8;
    tm.unk5C8.h = 0x64;
    PSX_STACK_RESIDUE(tm.animFrame);
    tm_setup(&tm);
    SetDispMask(1);
    SetDispMask(1);
    while (tm.task != TM_TASK_EXIT) {
        tm_clearOt();
        pad_update();
        tm_updatePad(&tm);
        ctx = g_gameState->gameData;
        prim = TM_PRIM_BUF + ctx->drawBuf * TM_PRIM_BUF_SIZE;
        if (tm.showScore != 0) {
            prim = tm_drawScore(ctx->ot[ctx->drawBuf], prim, &tm);
        }
        if (tm.showBlocks != 0) {
            prim = tm_drawBlocks(g_gameState->gameData->ot[g_gameState->gameData->drawBuf], prim, &tm);
        }
        prim = tm_drawBackground(g_gameState->gameData->ot[g_gameState->gameData->drawBuf], prim, &tm);
        if (tm.showBoard != 0 || tm.task == TM_TASK_FIELD_FADE) {
            prim = tm_drawBoard(g_gameState->gameData->ot[g_gameState->gameData->drawBuf], prim, &tm);
        }
        prim = tm_drawCards(g_gameState->gameData->ot[g_gameState->gameData->drawBuf], prim, &tm);
        prim = tm_drawCombo(g_gameState->gameData->ot[g_gameState->gameData->drawBuf], prim, &tm);
        prim = tm_drawBattleValues(g_gameState->gameData->ot[g_gameState->gameData->drawBuf], prim, &tm);
        switch (tm.task) {
        case TM_TASK_ENEMY_SELECT:
            g_menuHelp.text = 0;
            tm_debugPrint(&tm, "ENEMY CARD SELECT\n");
            tm_checkGameEnd(&tm);
            tm_enemySelect(&tm);
            tm_layoutCards(&tm);
            break;
        case TM_TASK_PLAYER_SELECT:
            help_setCursorOnTop(0);
            tm_checkGameEnd(&tm);
            if (tm.autoPlay == 0) {
                tm_debugPrint(&tm, "PLAYER CARD SELECT\n");
                tm_taskPlayerSelect(g_gameState->gameData->ot[g_gameState->gameData->drawBuf], prim, &tm);
            } else {
                tm_debugPrint(&tm, "PLAYER AUTO SELECT\n");
                tm_autoPlace(&tm);
            }
            tm_layoutCards(&tm);
            break;
        case TM_TASK_PLAYER_SET:
            tm_debugPrint(&tm, "PLAYER CARD SET\n");
            help_setCursorOnTop(0);
            tm_taskPlayerSet(g_gameState->gameData->ot[g_gameState->gameData->drawBuf], prim, &tm);
            tm_layoutCards(&tm);
            break;
        case TM_TASK_RESULT:
            g_menuHelp.text = 0;
            tm_drawResult(g_gameState->gameData->ot[g_gameState->gameData->drawBuf], prim, &tm);
            tm_debugPrint(&tm, "RESULT\n");
            tm_taskResult(&tm);
            break;
        case TM_TASK_INIT:
            g_menuHelp.text = 0;
            tm_debugPrint(&tm, "INIT\n");
            tm_taskInit(&tm);
            break;
        case TM_TASK_FLASH:
            g_menuHelp.text = 0;
            tm_debugPrint(&tm, "FLASH\n");
            tm_layoutCards(&tm);
            tm_drawFlash(g_gameState->gameData->ot[g_gameState->gameData->drawBuf], prim, &tm);
            break;
        case TM_TASK_BATTLE_SELECT:
            tm_debugPrint(&tm, "BATTLE SELECT\n");
            help_setCursorOnTop(0);
            prim = tm_taskBattleSelect(g_gameState->gameData->ot[g_gameState->gameData->drawBuf], prim, &tm);
            tm_layoutCards(&tm);
            tm_drawBattleSelect(g_gameState->gameData->ot[g_gameState->gameData->drawBuf], prim, &tm);
            break;
        case TM_TASK_BATTLE:
            g_menuHelp.text = 0;
            tm_debugPrint(&tm, "BATTLE\n");
            tm_layoutCards(&tm);
            tm_taskBattle(g_gameState->gameData->ot[g_gameState->gameData->drawBuf], prim, &tm);
            break;
        case TM_TASK_TURN:
            g_menuHelp.text = 0;
            tm_debugPrint(&tm, "TURN\n");
            tm_layoutCards(&tm);
            tm_taskTurn(&tm);
            break;
        case TM_TASK_BATTLE_WIN:
            g_menuHelp.text = 0;
            tm_debugPrint(&tm, "BATTLE WIN\n");
            tm_layoutCards(&tm);
            tm_taskBattleWin(&tm);
            break;
        case TM_TASK_BATTLE_LOSE:
            g_menuHelp.text = 0;
            tm_debugPrint(&tm, "BATTLE LOSE\n");
            tm_layoutCards(&tm);
            tm_taskBattleLose(&tm);
            break;
        case TM_TASK_TRY_AGAIN:
            tm_debugPrint(&tm, "TRY AGAIN\n");
            help_setCursorOnTop(1);
            tm_taskTryAgain(&tm);
            tm_drawTryAgain(g_gameState->gameData->ot[g_gameState->gameData->drawBuf], prim, &tm);
            break;
        case TM_TASK_CARD_CHANGE:
            g_menuHelp.text = 0;
            tm_debugPrint(&tm, "CARD_CHANGE\n");
            tm_taskCardChange(g_gameState->gameData->ot[g_gameState->gameData->drawBuf], prim, &tm);
            break;
        case TM_TASK_CARD_MOVE:
            g_menuHelp.text = 0;
            tm_debugPrint(&tm, "CARD_MOVE\n");
            tm_taskCardMove(g_gameState->gameData->ot[g_gameState->gameData->drawBuf], prim, &tm);
            break;
        case TM_TASK_FLASH2:
            g_menuHelp.text = 0;
            tm_debugPrint(&tm, "FLASH2\n");
            tm_drawFlash2(g_gameState->gameData->ot[g_gameState->gameData->drawBuf], prim, &tm);
            break;
        case TM_TASK_TRADE:
            g_menuHelp.text = 0;
            tm_debugPrint(&tm, "TRADE\n");
            tm_taskTrade(&tm);
            tm_drawTradeBadge(g_gameState->gameData->ot[g_gameState->gameData->drawBuf], prim, &tm);
            break;
        case TM_TASK_CHANGE2:
            g_menuHelp.text = 0;
            tm_debugPrint(&tm, "CHANGE2\n");
            help_setCursorOnTop(1);
            tm_taskChange2(g_gameState->gameData->ot[g_gameState->gameData->drawBuf], prim, &tm);
            break;
        case TM_TASK_CHOICE:
            tm_debugPrint(&tm, "CHOICE\n");
            help_setCursorOnTop(1);
            tm_stockInput(&tm);
            tm_drawStock(g_gameState->gameData->ot[g_gameState->gameData->drawBuf], prim, &tm);
            tm_showStockCard(&tm);
            tm_layoutCards(&tm);
            break;
        case TM_TASK_GAME_QUIT:
            tm_debugPrint(&tm, "GAME QUIT\n");
            tm_taskGameQuit(&tm);
            tm_drawStock(g_gameState->gameData->ot[g_gameState->gameData->drawBuf], prim, &tm);
            tm_showStockCard(&tm);
            tm_layoutCards(&tm);
            break;
        case TM_TASK_CHOICE2:
            tm_debugPrint(&tm, "CHOICE2\n");
            tm_drawStock(g_gameState->gameData->ot[g_gameState->gameData->drawBuf], prim, &tm);
            tm_showStockCard(&tm);
            tm_taskChoice2(&tm);
            tm_layoutCards(&tm);
            break;
        case TM_TASK_STAGE_SELECT:
            tm_debugPrint(&tm, "STAGE SELECT\n");
            tm_taskStageSelect(&tm);
            break;
        case TM_TASK_FIELD_FADE:
            g_menuHelp.text = 0;
            tm_debugPrint(&tm, "FIELD FADE\n");
            tm_layoutCards(&tm);
            break;
        case TM_TASK_AWAY_INFO:
            g_menuHelp.text = 0;
            tm_debugPrint(&tm, "AWAY INFO\n");
            tm_drawStock(g_gameState->gameData->ot[g_gameState->gameData->drawBuf], prim, &tm);
            tm_taskAwayInfo(&tm);
            break;
        case TM_TASK_CARD_AWAY:
            tm_debugPrint(&tm, "CARD AWAY\n");
            help_setCursorOnTop(1);
            tm_stockInput(&tm);
            tm_showStockCard(&tm);
            tm_drawStock(g_gameState->gameData->ot[g_gameState->gameData->drawBuf], prim, &tm);
            break;
        case TM_TASK_CARD_AWAY2:
            tm_debugPrint(&tm, "CARD AWAY2\n");
            help_setCursorOnTop(1);
            tm_showStockCard(&tm);
            tm_drawStock(g_gameState->gameData->ot[g_gameState->gameData->drawBuf], prim, &tm);
            tm_taskCardAway2(&tm);
            break;
        case TM_TASK_CARD_LESS5:
            g_menuHelp.text = 0;
            tm_debugPrint(&tm, "CARD LESS5\n");
            tm_taskCardLess5(&tm);
            break;
        case TM_TASK_EXTENSION:
            g_menuHelp.text = 0;
            tm_debugPrint(&tm, "EXTENSION\n");
            tm_taskExtension(&tm);
            break;
        case TM_TASK_END_FADE:
            g_menuHelp.text = 0;
            tm_debugPrint(&tm, "END FADE\n");
            if (tm.quitting != 0) {
                tm_drawStock(g_gameState->gameData->ot[g_gameState->gameData->drawBuf], prim, &tm);
                tm_showStockCard(&tm);
            }
            tm_taskEndFade(&tm);
            break;
        case TM_TASK_COIN:
            g_menuHelp.text = 0;
            tm_debugPrint(&tm, "COIN\n");
            tm_taskCoin(g_gameState->gameData->ot[g_gameState->gameData->drawBuf], prim, &tm);
            tm_layoutCards(&tm);
            break;
        case TM_TASK_FLASH_ARROW:
            g_menuHelp.text = 0;
            tm_debugPrint(&tm, "FLASH ARROW\n");
            tm_taskFlashArrow(&tm);
            break;
        case TM_TASK_ENEMY_THINK:
            g_menuHelp.text = 0;
            tm_debugPrint(&tm, "ENEMY THINK\n");
            tm_taskEnemyThink(g_gameState->gameData->ot[g_gameState->gameData->drawBuf], prim, &tm);
            tm_animEnemyThink(&tm);
            break;
        case TM_TASK_ENEMY_APPEAR:
            g_menuHelp.text = 0;
            tm_debugPrint(&tm, "ENEMY APEAR\n");
            tm_taskEnemyAppear(&tm);
            tm_revealBlocks(&tm);
            break;
        case TM_TASK_GET_SELECT:
            g_menuHelp.text = 0;
            tm_debugPrint(&tm, "GET SELECT\n");
            help_setCursorOnTop(0);
            tm_taskGetSelect(g_gameState->gameData->ot[g_gameState->gameData->drawBuf], prim, &tm);
            tm_layoutCards(&tm);
            break;
        default:
            g_menuHelp.text = 0;
            tm_debugPrint(&tm, "NO TASK\n");
            break;
        }
        if (tm.turn == TM_PLAYER) {
            tm_debugPrint(&tm, "TURN PLAYER\n");
        }
        if (tm.turn == TM_ENEMY) {
            tm_debugPrint(&tm, "TURN ENEMY\n");
        }
        game_updateWindows();
        game_updateFade();
        if (tm.task == TM_TASK_CHOICE || tm.task == TM_TASK_GAME_QUIT || tm.task == TM_TASK_CHOICE2 || tm.task == TM_TASK_PLAYER_SELECT || tm.task == TM_TASK_PLAYER_SET ||
            tm.task == TM_TASK_BATTLE_SELECT || tm.task == TM_TASK_TRY_AGAIN || tm.task == TM_TASK_GET_SELECT || tm.task == TM_TASK_CARD_AWAY || tm.task == TM_TASK_CARD_AWAY2) {
            help_setLocked(0);
        } else {
            help_setLocked(1);
        }
        help_update();
        tm.timer++;
        tm.animFrame++;
        rand();
        tm_endFrame(&tm);
    }
    pad_setRepeatEnabled(0, PAD_RPT_UP, tm.savedRepeat[0]);
    pad_setRepeatEnabled(0, PAD_RPT_DOWN, tm.savedRepeat[1]);
    pad_setRepeatEnabled(0, PAD_RPT_LEFT, tm.savedRepeat[2]);
    pad_setRepeatEnabled(0, PAD_RPT_RIGHT, tm.savedRepeat[3]);
    pad_setRepeatEnabled(0, PAD_RPT_L1, tm.savedRepeat[4]);
    pad_setRepeatEnabled(0, PAD_RPT_L2, tm.savedRepeat[6]);
    pad_setRepeatEnabled(0, PAD_RPT_R1, tm.savedRepeat[5]);
    pad_setRepeatEnabled(0, PAD_RPT_R2, tm.savedRepeat[7]);
    pad_setRepeatEnabled(0, PAD_RPT_CIRCLE, tm.savedRepeat[8]);
    pad_setRepeatEnabled(0, PAD_RPT_CROSS, tm.savedRepeat[9]);
    pad_setRepeatEnabled(0, PAD_RPT_TRIANGLE, tm.savedRepeat[10]);
    pad_setRepeatEnabled(0, PAD_RPT_SQUARE, tm.savedRepeat[11]);
    DrawSync(0);
    VSync(g_gameState->vsyncMode);
    SetDispMask(0);
    help_setCursorOnTop(1);
    help_setCallback(0);
    help_setLocked(0);
}

void tm_clearOt(void) {
    ClearOTagR(g_gameState->gameData->ot[g_gameState->gameData->drawBuf], OT_LEN);
}

void tm_endFrame(TmState *tm) {
    GameData *ctx;

    if ((g_padHeld & PAD_SOFT_RESET) == PAD_SOFT_RESET) {
        g_gameState->flags |= GS_RESET;
        tm->task = TM_TASK_EXIT;
    }
    if (&g_gameState->drawEnv[g_gameState->gameData->drawBuf].dfe != NULL) {
        DrawSync(0);
        VSync(g_gameState->vsyncMode);
    } else {
        DrawSync(0);
        VSync(g_gameState->vsyncMode);
        ResetGraph(1);
    }
    PutDispEnv(&g_gameState->dispEnv[g_gameState->gameData->drawBuf]);
    PutDrawEnv(&g_gameState->drawEnv[g_gameState->gameData->drawBuf]);
    ctx = g_gameState->gameData;
    DrawOTag(ctx->ot[ctx->drawBuf] + OT_LAST);
    g_gameState->gameData->drawBuf ^= 1;
    gfx_setBuffer(g_gameState->gameData->drawBuf);
}

void tm_setup(TmState *tm) {
    s32 i;
    u8 *cardTims;
    s32 x;
    s32 y;
    s32 row;

    tm_loadImageFile();
    cardTims = tm_getFileEntry(TM_FILE_CARDS);
    for (i = 0; i < CARD_KINDS; i++) {
        x = i / 30;
        y = i % 30;
        row = y / 6;
        tm_loadTim((TmTim *)(cardTims + i * 0xA80), x * 128 + (y % 6) * 21, row * 51 + 0x100, 0x180, i + 0x19C);
    }
    tm_loadTim((TmTim *)tm_getFileEntry(TM_FILE_BG_RIGHT), 0x300, 0, 0, 0xEE);
    tm_loadTim((TmTim *)tm_getFileEntry(TM_FILE_BG), 0x280, 0, 0, 0xE0);
    tm_loadTim((TmTim *)tm_getFileEntry(TM_FILE_BOARD), 0x340, 0, 0, 0xE7);
    tm_loadTim((TmTim *)tm_getFileEntry(TM_FILE_SCORE_DIGITS), 0x300, 0x100, 0, 0xEA);
    tm_loadTim((TmTim *)tm_getFileEntry(TM_FILE_COIN), 0x300, 0x180, 0, 0xEB);
    tm_loadTim((TmTim *)tm_getFileEntry(TM_FILE_BATTLE), 0x200, 0x100, 0, 0xEC);
    tm_loadTim((TmTim *)tm_getFileEntry(TM_FILE_SELECT_PARTS), 0x340, 0x1E0, 0, 0xED);
    tm_loadTim((TmTim *)tm_getFileEntry(TM_FILE_RESULT_TEXT), 0x280, 0x100, 0, 0xE9);
    tm_copyFileEntry((u8 *)g_tmOpponents, TM_FILE_OPPONENTS, 0x200);
    tm_copyFileEntry((u8 *)g_tmCardData, TM_FILE_CARD_DATA, 0x1F4);
    tm_copyFileEntry((u8 *)g_tmDecks, TM_FILE_DECKS, 0x400);
    g_evtMode = EVT_MODE_FIELD;
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
    tm->wins = 0;
    tm->losses = 0;
    tm->draws = 0;
    tm->autoPlay = 0;
    tm->hasLostCard = 0;
    tm->quitting = 0;
    tm->opponent = (u16)g_gameState->gameData->cardOpponent;
    tm->deck = g_tmOpponents[tm->opponent].deck;
    tm->aiLevel = g_tmOpponents[tm->opponent].aiLevel;
    tm->unk454 = 1;
    tm_taskInit(tm);
    tm->savedRepeat[0] = pad_isRepeatEnabled(0, PAD_RPT_UP);
    tm->savedRepeat[1] = pad_isRepeatEnabled(0, PAD_RPT_DOWN);
    tm->savedRepeat[2] = pad_isRepeatEnabled(0, PAD_RPT_LEFT);
    tm->savedRepeat[3] = pad_isRepeatEnabled(0, PAD_RPT_RIGHT);
    tm->savedRepeat[4] = pad_isRepeatEnabled(0, PAD_RPT_L1);
    tm->savedRepeat[6] = pad_isRepeatEnabled(0, PAD_RPT_L2);
    tm->savedRepeat[5] = pad_isRepeatEnabled(0, PAD_RPT_R1);
    tm->savedRepeat[7] = pad_isRepeatEnabled(0, PAD_RPT_R2);
    tm->savedRepeat[8] = pad_isRepeatEnabled(0, PAD_RPT_CIRCLE);
    tm->savedRepeat[9] = pad_isRepeatEnabled(0, PAD_RPT_CROSS);
    tm->savedRepeat[10] = pad_isRepeatEnabled(0, PAD_RPT_TRIANGLE);
    tm->savedRepeat[11] = pad_isRepeatEnabled(0, PAD_RPT_SQUARE);
    pad_setRepeatEnabled(0, PAD_RPT_UP, 1);
    pad_setRepeatEnabled(0, PAD_RPT_DOWN, 1);
    pad_setRepeatEnabled(0, PAD_RPT_LEFT, 1);
    pad_setRepeatEnabled(0, PAD_RPT_RIGHT, 1);
    pad_setRepeatEnabled(0, PAD_RPT_BTN_L1, 1);
    pad_setRepeatEnabled(0, PAD_RPT_BTN_R1, 1);
    pad_setRepeatEnabled(0, PAD_RPT_BTN_CROSS, 0);
    pad_setRepeatEnabled(0, PAD_RPT_BTN_CIRCLE, 0);
    tm->arrowOdds[0] = 1;
    tm->arrowOdds[1] = 4;
    tm->arrowOdds[2] = 0x10;
    tm->arrowOdds[3] = 0x1F;
    tm->arrowOdds[4] = 0x1B;
    tm->arrowOdds[5] = 0xA;
    tm->arrowOdds[6] = 6;
    tm->arrowOdds[7] = 3;
    tm->arrowOdds[8] = 2;
    tm->arrowOdds[0] = 1;
    tm->arrowOdds[1] = 8;
    tm->arrowOdds[2] = 0x19;
    tm->arrowOdds[3] = 0x1F;
    tm->arrowOdds[4] = 0x12;
    tm->arrowOdds[5] = 9;
    tm->arrowOdds[6] = 5;
    tm->arrowOdds[7] = 2;
    tm->arrowOdds[8] = 1;
    tm->oddsRow = 0;
    if (card_countAll() < TM_HAND_SIZE) {
        for (i = 0; i < 8; i++) {
            tm_addRandomCard(tm, rand() % CARD_KINDS);
        }
    }
}

void tm_taskInit(TmState *tm) {
    tm->cursor = 0;
    tm->boardX = 0;
    tm->boardY = 0;
    tm->brightness = 0x80;
    tm->unk554 = 0;
    tm->scrollTimer = 0;
    tm->valuesTimer = 0;
    tm->msgWin = NULL;
    tm->quitWin = 0;
    tm->infoWin = NULL;
    tm->less5Win = NULL;
    tm->comboTimer = 0;
    tm->showBoard = 0;
    tm->showBlocks = 0;
    tm->showScore = 0;
    tm_setWindowOpening(0);
    tm_resetStockCursor(tm);
    tm_initHands(tm);
    tm_initBoard(tm);
    tm->unk4BC = 0;
    tm->unk4C0 = 1;
    tm_openStock(tm);
}

void tm_debugPrint(TmState *tm, char *msg) {
}

void tm_loadImageFile(void) {
    FileEntry *entry = file_find(DIR_COMMON, FILE_TM_IMAGES);
    if (entry == NULL) {
        SystemError(0x46, 0x3E6);
    }
    cd_readSync(g_gameState->imgSector + entry->sector, (entry->link - entry->sector) << 11, TM_IMAGE_FILE);
}

u8 *tm_getFileEntry(s32 i) {
    s32 *toc = (s32 *)TM_IMAGE_FILE;
    toc += i * 2;
    return TM_IMAGE_FILE + *toc * 0x800;
}

void tm_copyFileEntry(u8 *dst, s32 entry, s32 len) {
    s32 i;
    u8 *src = tm_getFileEntry(entry);
    for (i = 0; i < len; i++) {
        *dst++ = *src++;
    }
}

void *tm_drawBackground(u32 *ot, SPRT_16 *prim, TmState *tm) {
    u8 bright;
    s32 x;
    s32 y;

    bright = tm->brightness;
    for (x = 0; x < 16; x++) {
        for (y = 0; y < 15; y++) {
            if (tm->showBoard == 0 || x < 3) {
                if (tm->task == TM_TASK_FIELD_FADE && x >= 3) {
                    if (tm->timer * 4 < 0x80) {
                        bright = 0x80 - tm->timer * 4;
                    } else {
                        bright = 0;
                    }
                }
                setSprt16(prim);
                prim->x0 = x * 16;
                prim->y0 = y * 16;
                prim->v0 = y * 16;
                prim->r0 = bright;
                prim->g0 = bright;
                prim->b0 = bright;
                prim->u0 = x * 16;
                prim->clut = getClut(0, 0xE0);
                AddPrim(ot + 0xFFE, prim);
                prim++;
            }
        }
    }
    setDrawTPage((DR_TPAGE *)prim, 0, 0, GetTPage(1, 0, 0x280, 0));
    AddPrim(ot + 0xFFE, prim);
    do {
        prim = (SPRT_16 *)((DR_TPAGE *)prim + 1);
    } while (0);
    for (x = 0; x < 4; x++) {
        for (y = 0; y < 15; y++) {
            if (tm->showBoard == 0 || x > 0) {
                if (tm->task == TM_TASK_FIELD_FADE && x == 0) {
                    if (tm->timer * 4 < 0x80) {
                        bright = 0x80 - tm->timer * 4;
                    } else {
                        bright = 0;
                    }
                }
                setSprt16(prim);
                prim->x0 = x * 16 + 0x100;
                prim->y0 = y * 16;
                prim->v0 = y * 16;
                prim->r0 = bright;
                prim->g0 = bright;
                prim->b0 = bright;
                prim->u0 = x * 16;
                prim->clut = getClut(0, 0xE0);
                AddPrim(ot + 0xFFE, prim);
                prim++;
            }
        }
    }
    setDrawTPage((DR_TPAGE *)prim, 0, 0, GetTPage(1, 0, 0x300, 0));
    AddPrim(ot + 0xFFE, prim);
    return (DR_TPAGE *)prim + 1;
}

void *tm_drawBoard(u32 *ot, SPRT_16 *prim, TmState *tm) {
    u8 bright;
    s32 x;
    s32 y;
    DR_TPAGE *tp;

    bright = tm->brightness;
    if (tm->task == TM_TASK_FIELD_FADE) {
        bright = tm->timer * 4;
    }
    for (x = 0; x < 14; x++) {
        for (y = 0; y < 15; y++) {
            setSprt16(prim);
            if (tm->task == TM_TASK_FIELD_FADE) {
                prim->code = GPU_SPRT_16 | GPU_ABE;
            }
            prim->x0 = x * 16 + 0x30;
            prim->y0 = y * 16;
            prim->v0 = y * 16;
            prim->r0 = bright;
            prim->g0 = bright;
            prim->b0 = bright;
            prim->u0 = x * 16;
            prim->clut = getClut(0, 0xE7);
            AddPrim(ot + 0xFFA, prim);
            prim++;
        }
    }
    tp = (DR_TPAGE *)prim;
    setDrawTPage(tp, 0, 0, GetTPage(1, 1, 0x340, 0));
    AddPrim(ot + 0xFFA, tp);
    tp++;
    if (tm->task == TM_TASK_FIELD_FADE && tm->timer >= 0x20) {
        tm->showBoard = 1;
        tm_dealEnemyHand(tm);
        tm->showScore = 1;
        for (y = 4; y >= 0; y--) {
            tm->landed[y] = 0;
        }
        tm->showBlocks = 1;
        tm->blocksDone = 0;
        tm->blockTimer = 0;
        tm->timer = 0;
        tm->task = TM_TASK_ENEMY_APPEAR;
    }
    return tp;
}

void tm_loadTim(TmTim *tim, s32 x, s32 y, s32 cx, s32 cy) {
    TmTimImage *img;
    s32 tp;

    img = (TmTimImage *)((u8 *)tim + (tim->clutLen + 8));
    switch ((s32)(tim->flags & TM_TIM_PMODE)) {
    case TM_TIM_4BIT:
        tp = 0;
        LoadTPage(img->data, tp, 0, x, y, img->w << 2, img->h);
        DrawSync(0);
        LoadClut(tim->clut, cx, cy);
        DrawSync(0);
        break;
    case TM_TIM_8BIT:
        tp = 1;
        LoadTPage(img->data, tp, 0, x, y, img->w << tp, img->h);
        DrawSync(0);
        LoadClut(tim->clut, cx, cy);
        DrawSync(0);
        break;
    case TM_TIM_16BIT:
        break;
    default:
        return;
    }
}

void *tm_drawScore(u32 *ot, SPRT *prim, TmState *tm) {
    s32 i;
    s32 count;
    s16 x;
    s16 y;
    u8 u;
    u8 v;
    s16 w;
    u8 bright;

    bright = tm->brightness;
    for (i = 0; i < 2; i++) {
        if (i == 0) {
            count = tm_countOnBoard(tm, TM_PLAYER);
            x = 0x28;
            y = 0xB9;
            v = 0;
        } else {
            count = tm_countOnBoard(tm, TM_ENEMY);
            x = 0x1A;
            y = 0x93;
            v = 0x20;
        }
        u = (count - 1) * 16;
        if (count == 0) {
            u = 0x90;
        }
        if (count == 10) {
            u = 0xA0;
            x -= 8;
            w = 0x20;
        } else {
            w = 0x10;
        }
        setSprt(prim);
        prim->x0 = x;
        prim->y0 = y;
        prim->r0 = bright;
        prim->g0 = bright;
        prim->b0 = bright;
        prim->u0 = u;
        prim->v0 = v;
        prim->clut = getClut(0, 0xEA);
        prim->w = w;
        prim->h = 0x20;
        AddPrim(ot + 0xFF0, prim);
        prim++;
    }
    setSprt(prim);
    prim->x0 = 0x10;
    prim->y0 = 0xAA;
    prim->r0 = bright;
    prim->g0 = bright;
    prim->b0 = bright;
    prim->u0 = 0xC0;
    prim->v0 = 0;
    prim->clut = getClut(0, 0xEA);
    prim->w = 0x30;
    prim->h = 0x20;
    AddPrim(ot + 0xFF0, prim);
    prim++;
    setDrawTPage((DR_TPAGE *)prim, 0, 0, GetTPage(1, 0, 0x300, 0x100));
    AddPrim(ot + 0xFF0, prim);
    return (DR_TPAGE *)prim + 1;
}

void tm_updatePad(TmState *tm) {
    pad_updateRepeat(0);
}

void *tm_taskPlayerSelect(u32 *ot, void *prim, TmState *tm) {
    RECT nameRect;
    TmCard *card;
    SPRT *sp;
    s32 pad;
    s32 x;
    s32 y;
    s32 k;
    s32 ry;
    s32 w;

    if (tm_countInHand(tm, TM_PLAYER) != 0) {
        pad = g_padRepeat;
        card = tm_getHandCard(tm, TM_PLAYER, tm->cursor);
        if (tm_countInHand(tm, TM_PLAYER) != TM_HAND_SIZE) {
            ry = card->y + 0x34;
            if (tm->cursor == 3) {
                ry = card->y - 0x19;
            }
        } else {
            ry = card->y + 0x2A;
            if (tm->cursor == 4) {
                ry = card->y - 0x19;
            }
        }
        if (tm_countInHand(tm, TM_PLAYER) != 0) {
            if (tm_countPlayerHand(tm, TM_PLAYER) != TM_HAND_SIZE) {
                w = 0x104;
                x = card->x - 0xF;
                y = card->y + 6;
            } else {
                w = 0x108;
                x = card->x - 0x12;
                y = card->y + 3;
            }
            tm_setHelpArea(w);
            sp = tm_drawCursor(ot, prim, tm, x, y, 0);
            g_menuHelp.x = x + 2;
            g_menuHelp.y = y + 1;
            g_menuHelp.text = text_get(TM_HELP_SELECT);
            text_drawBlock(0x12, 9, 0xFFA, text_get(TM_TEXT_ICON_R1), TEXT_PAL_WHITE);
            setSprt(sp);
            sp->x0 = 0x1F;
            sp->y0 = 0xC;
            sp->v0 = 0xD0;
            sp->w = 0x1C;
            sp->h = 8;
            sp->clut = getClut(0, 0xEB);
            sp->u0 = 0x64;
            sp->r0 = 0x80;
            sp->g0 = 0x80;
            sp->b0 = 0x80;
            AddPrim(ot + 0x28, sp);
            sp++;
            setDrawTPage((DR_TPAGE *)sp, 0, 0, GetTPage(1, 0, 0x300, 0x100));
            AddPrim(ot + 0x28, sp);
            prim = (DR_TPAGE *)sp + 1;
            if (pad & PAD_UP) {
                if (tm->cursor != 0) {
                    tm->nameSlide = 0;
                    tm->cursor--;
                    tm_playSe(TM_SE_CURSOR);
                }
            }
            if (pad & PAD_DOWN) {
                if (tm->cursor != tm_countInHand(tm, TM_PLAYER) - 1) {
                    tm->nameSlide = 0;
                    tm->cursor++;
                    tm_playSe(TM_SE_CURSOR);
                }
            }
            if (g_padHeld & BTN_R1) {
                if (tm->nameSlide < 0xA) {
                    tm->nameSlide++;
                }
            } else if (tm->nameSlide > 0) {
                tm->nameSlide--;
            }
            if (tm->nameSlide > 0) {
                nameRect.w = card_getNameWidth(card->id) + 0x10;
                nameRect.x = 0x11A - nameRect.w / 2;
                if (nameRect.x + nameRect.w >= 0x140) {
                    nameRect.x = 0x130 - card_getNameWidth(card->id);
                }
                nameRect.y = ry;
                nameRect.h = 0x19;
                k = tm->nameSlide * 0xC - 0x78;
                nameRect.x -= k;
                text_drawBlock(nameRect.x + 8, nameRect.y + 6, 5, (u8 *)card_getName(card->id), TEXT_PAL_WHITE);
                win_drawBox(&nameRect, 5, WIN_STYLE_BEVEL);
            }
            if (pad & BTN_CROSS) {
                tm->task = TM_TASK_PLAYER_SET;
                tm->savedFrame = tm->animFrame;
                tm_playSe(TM_SE_CURSOR);
            }
        }
    }
    return prim;
}

void *tm_taskPlayerSet(u32 *ot, void *prim, TmState *tm) {
    TmCell *cell;
    TmCard *card;
    s32 pad;
    s32 cx;
    s32 cy;
    s32 ox;
    s32 oy;
    s32 x;
    s32 y;
    s32 one;

    if (tm_countInHand(tm, TM_PLAYER) == 0) {
        return prim;
    }
    pad = g_padRepeat;
    tm_getHandCard(tm, TM_PLAYER, tm->cursor);
    cell = tm_getCell(tm, tm->boardX, tm->boardY);
    cx = tm->boardX * TM_SQUARE_W + TM_BOARD_X;
    cy = tm->boardY * TM_SQUARE_H + TM_BOARD_Y;
    prim = tm_drawCursor(ot, prim, tm, cx - 0xF, cy + 6, 0);
    card = tm_getHandCard(tm, TM_PLAYER, tm->cursor);
    if (tm_countPlayerHand(tm, TM_PLAYER) != TM_HAND_SIZE) {
        x = card->x - 0xF;
        y = card->y + 6;
    } else {
        x = card->x - 0x12;
        y = card->y + 3;
    }
    prim = tm_drawCursor(ot, prim, tm, x, y, 1);
    g_menuHelp.x = cx - 0xD;
    g_menuHelp.y = cy + 7;
    if (cell->state == TM_CELL_EMPTY) {
        g_menuHelp.text = text_get(TM_HELP_PLACE);
    } else {
        g_menuHelp.text = text_get(TM_HELP_PLACE_BLOCKED);
    }
    help_setBounds(NULL);
    if (pad & BTN_CIRCLE) {
        tm->nameSlide = 0;
        tm->task = TM_TASK_PLAYER_SELECT;
        tm->animFrame = tm->savedFrame;
        tm_playSe(TM_SE_CANCEL);
        return prim;
    }
    if (pad & BTN_CROSS) {
        if (cell->state == TM_CELL_EMPTY) {
            card = tm_getHandCard(tm, TM_PLAYER, tm->cursor);
            card->x = cx;
            card->y = cy;
            card->onBoard = 1;
            card->small = 0;
            cell->state = TM_CELL_CARD;
            cell->card = card;
            tm->cursor = 0;
            tm->srcX = tm->boardX;
            tm->srcY = tm->boardY;
            tm_checkBattles(tm);
            tm_playSe(TM_SE_CARD);
            return prim;
        }
        tm_playSe(TM_SE_BUZZER);
    }
    ox = tm->boardX;
    oy = tm->boardY;
    if ((pad & PAD_RIGHT) && tm->boardX != TM_BOARD_SIZE - 1) {
        tm->boardX++;
    }
    if ((pad & PAD_LEFT) && tm->boardX != 0) {
        tm->boardX--;
    }
    if ((pad & PAD_UP) && tm->boardY != 0) {
        tm->boardY--;
    }
    if ((pad & PAD_DOWN) && tm->boardY != TM_BOARD_SIZE - 1) {
        tm->boardY++;
    }
    if (ox != tm->boardX || oy != tm->boardY) {
        tm_playSe(TM_SE_CURSOR);
    }
    return prim;
}

void tm_taskResult(TmState *tm) {
    RECT rect;

    if ((g_padRepeat & (BTN_CIRCLE | BTN_CROSS)) && tm->timer >= 0x22) {
        tm->showBoard = 0;
        tm->showScore = 0;
        tm->showBlocks = 0;
        switch (tm->result) {
        case TM_RESULT_WIN:
            tm_startCardMove(tm);
            break;
        case TM_RESULT_LOSE:
            tm_startCardMove(tm);
            break;
        case TM_RESULT_DRAW:
            tm_returnHand(tm);
            tm_hideCards(tm);
            if ((u32)(tm->opponent - TM_OPP_FORCED) < TM_OPP_FORCED_NUM) {
                rect.x = 0x59;
                rect.w = 0x8F;
                rect.w = text_measureWidth(text_get(TM_TEXT_DRAW)) + 0x12;
                rect.x = 0xA0 - rect.w / 2;
                rect.y = 0x59;
                rect.h = 0x2B;
                tm->infoWin = win_openFree(&rect, 5, WIN_STYLE_BEVEL);
                win_setDrawContent(tm->infoWin, tm_drawDrawWin);
                win_setShadow(tm->infoWin, 1);
                tm_waitWindow(tm->infoWin);
                tm->task = TM_TASK_EXTENSION;
            } else {
                tm_askRematch(tm);
            }
            break;
        }
    }
}

void tm_taskExtension(TmState *tm) {
    if (g_tmWindowOpening == 0 && (g_padRepeat & (BTN_CIRCLE | BTN_CROSS))) {
        win_close(tm->infoWin);
        tm->task = TM_TASK_INIT;
    }
}

void tm_drawDrawWin(Window *win) {
    text_drawBlock(win->x + 9, win->y + 7, win->otz, text_get(TM_TEXT_DRAW), TEXT_PAL_WHITE);
}

void tm_autoPlace(TmState *tm) {
    s32 x;
    s32 y;
    s32 px;
    s32 py;
    TmCell *cell;
    TmCard *card;

    if (tm_countInHand(tm, TM_PLAYER) != 0) {
        do {
            x = rand() & (TM_BOARD_SIZE - 1);
            y = rand() & (TM_BOARD_SIZE - 1);
            cell = tm_getCell(tm, x, y);
        } while (cell->state != TM_CELL_EMPTY);
        px = x * TM_SQUARE_W + TM_BOARD_X;
        py = y * TM_SQUARE_H + TM_BOARD_Y;
        card = tm_getHandCard(tm, TM_PLAYER, 0);
        card->x = px;
        card->y = py;
        card->small = 0;
        card->onBoard = 1;
        cell->state = TM_CELL_CARD;
        cell->card = card;
        tm->srcX = x;
        tm->srcY = y;
        tm_checkBattles(tm);
    }
}

void tm_taskTrade(TmState *tm) {
    if (g_tmWindowOpening == 0 && (g_padRepeat & (BTN_CROSS | BTN_SQUARE))) {
        tm_endTrade(tm);
    }
}

void tm_taskTryAgain(TmState *tm) {
    RECT rect;
    s32 pad;

    if (g_tmWindowOpening == 0) {
        pad = g_padRepeat;
        if (pad & BTN_CROSS) {
            win_close(tm->msgWin);
            if (tm->cursor == TM_SEL_YES) {
                if (card_countAll() < TM_HAND_SIZE) {
                    rect.w = text_measureWidth(text_get(TM_TEXT_LESS_5)) + 0x12;
                    rect.x = 0xA0 - rect.w / 2;
                    rect.y = 0x5B;
                    rect.h = 0x2B;
                    tm->less5Win = win_openFree(&rect, 6, WIN_STYLE_BEVEL);
                    win_setDrawContent(tm->less5Win, tm_drawLess5Win);
                    win_setShadow(tm->less5Win, 1);
                    tm_waitWindow(tm->less5Win);
                    tm->task = TM_TASK_CARD_LESS5;
                    tm_playSe(TM_SE_BUZZER);
                } else {
                    tm->task = TM_TASK_INIT;
                }
            } else {
                tm_startEndFade(tm);
            }
            tm_playSe(TM_SE_CURSOR);
        }
        if ((pad & BTN_CIRCLE) && tm->cursor != TM_SEL_NO) {
            tm_playSe(TM_SE_CANCEL);
            tm->cursor = TM_SEL_NO;
        }
        if (pad & PAD_UP) {
            if (tm->cursor != TM_SEL_YES) {
                tm_playSe(TM_SE_CURSOR);
                tm->cursor = TM_SEL_YES;
            } else {
                tm_playSe(TM_SE_CURSOR);
                tm->cursor = TM_SEL_NO;
            }
        }
        if (pad & PAD_DOWN) {
            if (tm->cursor != TM_SEL_NO) {
                tm_playSe(TM_SE_CURSOR);
                tm->cursor = TM_SEL_NO;
            } else {
                tm_playSe(TM_SE_CURSOR);
                tm->cursor = TM_SEL_YES;
            }
        }
        if (tm->cursor == TM_SEL_YES) {
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

void tm_stockInput(TmState *tm) {
    RECT rect;
    u32 pad;
    s32 copies;
    s32 x;
    s32 y;
    TmCard *card;
    CardEntry *entry;

    ui_drawHand(tm->stockCol * 15 + 3, tm->stockRow * 15 + 0x30, 3);
    if (tm->task == TM_TASK_CHOICE || tm->task == TM_TASK_CARD_AWAY) {
        g_menuHelp.x = tm->stockCol * 15 + 3;
        g_menuHelp.y = tm->stockRow * 15 + 0x30;
        if (tm->task == TM_TASK_CHOICE) {
            g_menuHelp.text = text_get((u32)(tm->opponent - TM_OPP_FORCED) >= TM_OPP_FORCED_NUM ? TM_HELP_STOCK : TM_HELP_STOCK_FORCED);
        } else {
            g_menuHelp.text = text_get(TM_HELP_DISCARD_PICK);
        }
        rect.x = 0xD;
        rect.w = 0x9E;
        rect.y = 0;
        rect.h = 0xDE;
        help_setBounds(&rect);
    }
    pad = g_padRepeat;
    copies = card_countOf(tm_getStockCursorId(tm));
    if (tm->scrollTimer != 0) {
        pad = 0;
        if (--tm->scrollTimer == 0 && tm->phase == 0) {
            if (copies == ++tm->copyIdx) {
                tm->copyIdx = 0;
            }
        }
    }
    if (copies >= 2) {
        if (pad & BTN_R1) {
            tm_playSe(TM_SE_CARD);
            pad = 0;
            tm->phase = 0;
            tm->scrollTimer = 4;
        } else if (pad & BTN_L1) {
            pad = 0;
            tm->phase = 1;
            tm->scrollTimer = 4;
            tm->copyIdx--;
            tm_playSe(TM_SE_CARD);
            if (tm->copyIdx == -1) {
                tm->copyIdx = copies - 1;
            }
        }
    }
    x = tm->stockCol;
    y = tm->stockRow;
    if ((pad & PAD_RIGHT) && x != TM_STOCK_SIDE - 1) {
        tm->stockCol = x + 1;
        tm->copyIdx = 0;
    }
    if ((pad & PAD_LEFT) && tm->stockCol != 0) {
        tm->stockCol--;
        tm->copyIdx = 0;
    }
    if ((pad & PAD_DOWN) && tm->stockRow != TM_STOCK_SIDE - 1) {
        tm->stockRow++;
        tm->copyIdx = 0;
    }
    if ((pad & PAD_UP) && tm->stockRow != 0) {
        tm->stockRow--;
        tm->copyIdx = 0;
    }
    if (x != tm->stockCol || y != tm->stockRow) {
        tm_playSe(TM_SE_CURSOR);
        return;
    }
    if (tm->task == TM_TASK_CHOICE) {
        if (((pad & BTN_CIRCLE) && tm->handCount == 0) || (pad & BTN_SQUARE)) {
            if (tm->opponent != TM_OPP_FORCED) if (tm->opponent != TM_OPP_FORCED + 1) if (tm->opponent != TM_OPP_FORCED + 2) if (tm->opponent != TM_OPP_FORCED + 3) {
                rect.w = text_measureWidth(text_get(TM_TEXT_QUIT_ASK)) + 0x12;
                rect.x = 0xA0 - rect.w / 2;
                rect.y = 0x54;
                rect.h = 0x38;
                tm->quitChoice = 1;
                tm->quitWin = win_openFree(&rect, 5, WIN_STYLE_BEVEL);
                win_setDrawContent(tm->quitWin, tm_drawQuitWin);
                win_setShadow(tm->quitWin, 1);
                tm_waitWindow(tm->quitWin);
                tm->task = TM_TASK_GAME_QUIT;
                return;
            }
        }
        if (pad & BTN_CIRCLE) {
            if (tm->handCount != 0) {
                tm->handCount--;
                card = &tm->cards[tm->handCount];
                card_addWithStats(card->id, card->arrows, card->power, card->type, card->pdef, card->mdef);
                tm_playSe(TM_SE_CANCEL);
            }
        } else if (pad & BTN_CROSS) {
            if (tm->autoPlay != 0) {
                card_clearAll();
                tm->stockCol = rand() % 10;
                tm->stockRow = rand() % 10;
                tm->copyIdx = 0;
                if (card_countOf(tm_getStockCursorId(tm)) == 0) {
                    tm_addRandomCard(tm, tm_getStockCursorId(tm));
                    tm_addRandomCard(tm, tm_getStockCursorId(tm));
                    tm_addRandomCard(tm, tm_getStockCursorId(tm));
                    tm_addRandomCard(tm, tm_getStockCursorId(tm));
                    tm_addRandomCard(tm, tm_getStockCursorId(tm));
                    tm_addRandomCard(tm, tm_getStockCursorId(tm));
                    tm_addRandomCard(tm, tm_getStockCursorId(tm));
                }
            }
            entry = card_find(tm_getStockCursorId(tm), tm->copyIdx);
            if (entry != (CardEntry *)-1) {
                card = &tm->cards[tm->handCount];
                card->id = entry->id;
                card->power = entry->power;
                card->type = entry->type;
                card->pdef = entry->physDef;
                card->mdef = entry->magicDef;
                card->arrows = entry->arrows;
                card_remove(tm_getStockCursorId(tm), tm->copyIdx);
                tm->copyIdx = 0;
                tm_playSe(TM_SE_CARD);
                if (++tm->handCount == TM_HAND_SIZE) {
                    g_menuHelpText = 0;
                    rect.w = text_measureWidth(text_get(TM_TEXT_CONFIRM)) + 0x12;
                    rect.x = 0xA0 - rect.w / 2;
                    rect.y = 0x53;
                    rect.h = 0x3A;
                    tm->msgWin = win_openFree(&rect, 6, WIN_STYLE_BEVEL);
                    win_setDrawContent(tm->msgWin, tm_drawConfirmWin);
                    win_setShadow(tm->msgWin, 1);
                    tm_waitWindow(tm->msgWin);
                    tm->cursor = 0;
                    tm->task = TM_TASK_CHOICE2;
                }
            }
        }
    } else if ((pad & BTN_CROSS) && card_countOf(tm_getStockCursorId(tm)) != 0) {
        g_menuHelpText = 0;
        rect.w = text_measureWidth(text_get(TM_TEXT_DISCARD_ASK)) + 0x12;
        rect.x = 0xB9;
        rect.h = 0x38;
        rect.y = 0x35;
        tm->cursor = 1;
        tm->infoWin = win_openFree(&rect, 5, WIN_STYLE_BEVEL);
        win_setDrawContent(tm->infoWin, tm_drawDiscardWin);
        win_setShadow(tm->infoWin, 1);
        tm_waitWindow(tm->infoWin);
        tm_playSe(TM_SE_CURSOR);
        tm->task = TM_TASK_CARD_AWAY2;
    }
}

void tm_taskChoice2(TmState *tm) {
    s32 pad;
    TmCard *card;

    if (g_tmWindowOpening == 0) {
        pad = g_padRepeat;
        if (pad & BTN_CROSS) {
            if (tm->cursor == TM_SEL_YES) {
                tm->timer = 0;
                tm->task = TM_TASK_FIELD_FADE;
            } else {
                tm->task = TM_TASK_CHOICE;
                tm->handCount = 4;
                card = &tm->cards[4];
                card_addWithStats(card->id, card->arrows, card->power, card->type, card->pdef, card->mdef);
            }
            win_close(tm->msgWin);
            tm_playSe(TM_SE_CURSOR);
        }
        if ((pad & BTN_CIRCLE) && tm->cursor != TM_SEL_NO) {
            tm->cursor = TM_SEL_NO;
            tm_playSe(TM_SE_CANCEL);
        }
        if (pad & PAD_UP) {
            if (tm->cursor != TM_SEL_YES) {
                tm->cursor = TM_SEL_YES;
            } else {
                tm->cursor = TM_SEL_NO;
            }
            tm_playSe(TM_SE_CURSOR);
        }
        if (pad & PAD_DOWN) {
            if (tm->cursor != TM_SEL_NO) {
                tm->cursor = TM_SEL_NO;
            } else {
                tm->cursor = TM_SEL_YES;
            }
            tm_playSe(TM_SE_CURSOR);
        }
        if (tm->cursor == TM_SEL_YES) {
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

void tm_drawConfirmWin(Window *win) {
    text_drawBlock(win->x + 9, win->y + 7, win->otz, text_get(TM_TEXT_CONFIRM), TEXT_PAL_WHITE);
}

void tm_drawLess5Win(Window *win) {
    text_drawBlock(win->x + 9, win->y + 7, win->otz, text_get(TM_TEXT_LESS_5), TEXT_PAL_WHITE);
}

void tm_taskStageSelect(TmState *tm) {
    s32 sum;
    s32 pad;

    sum = tm->arrowOdds[0] + tm->arrowOdds[1] + tm->arrowOdds[2] + tm->arrowOdds[3] + tm->arrowOdds[4] + tm->arrowOdds[5] +
          tm->arrowOdds[6] + tm->arrowOdds[7] + tm->arrowOdds[8];
    pad = g_padRepeat;
    if (!(g_padHeld & BTN_CIRCLE)) {
        if (pad & PAD_DOWN) {
            tm->opponent--;
        }
        if (pad & PAD_RIGHT) {
            tm->opponent += 10;
        }
        if (pad & PAD_UP) {
            tm->opponent++;
        }
        if (pad & PAD_LEFT) {
            tm->opponent -= 10;
        }
        if ((pad & BTN_CROSS) && sum == 100) {
            tm_openStock(tm);
        }
    } else {
        if ((pad & PAD_DOWN) && tm->oddsRow != 8) {
            tm->oddsRow++;
        }
        if ((pad & PAD_UP) && tm->oddsRow != 0) {
            tm->oddsRow--;
        }
        if ((pad & PAD_RIGHT) && sum != 100) {
            tm->arrowOdds[tm->oddsRow]++;
        }
        if ((pad & PAD_LEFT) && tm->arrowOdds[tm->oddsRow] != 0) {
            tm->arrowOdds[tm->oddsRow]--;
        }
    }
    tm->deck = g_tmOpponents[tm->opponent].deck;
    tm->aiLevel = g_tmOpponents[tm->opponent].aiLevel;
}

void tm_taskAwayInfo(TmState *tm) {
    if (g_tmWindowOpening == 0 && (g_padRepeat & (BTN_CROSS | BTN_SQUARE))) {
        win_close(tm->infoWin);
        tm->task = TM_TASK_CARD_AWAY;
    }
}

void tm_taskCardAway2(TmState *tm) {
    s32 pad;

    if (g_tmWindowOpening == 0) {
        pad = g_padRepeat;
        if (pad & BTN_CROSS) {
            if (tm->cursor == TM_SEL_YES) {
                card_remove(tm_getStockCursorId(tm), tm->copyIdx);
                if (card_countAll() > CARD_MAX) {
                    tm->copyIdx = 0;
                    tm->task = TM_TASK_CARD_AWAY;
                } else {
                    tm_hideCards(tm);
                    tm_askRematch(tm);
                }
            } else {
                tm->task = TM_TASK_CARD_AWAY;
            }
            win_close(tm->infoWin);
            tm_playSe(TM_SE_CURSOR);
        }
        if ((pad & BTN_CIRCLE) && tm->cursor != TM_SEL_NO) {
            tm->cursor = TM_SEL_NO;
            tm_playSe(TM_SE_CANCEL);
        }
        if (pad & PAD_UP) {
            if (tm->cursor != TM_SEL_YES) {
                tm->cursor = TM_SEL_YES;
            } else {
                tm->cursor = TM_SEL_NO;
            }
            tm_playSe(TM_SE_CURSOR);
        }
        if (pad & PAD_DOWN) {
            if (tm->cursor != TM_SEL_NO) {
                tm->cursor = TM_SEL_NO;
            } else {
                tm->cursor = TM_SEL_YES;
            }
            tm_playSe(TM_SE_CURSOR);
        }
        if (tm->cursor == TM_SEL_YES) {
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
    if (tm->timer & 3) {
        ui_drawHand(tm->stockCol * 15 + 3, tm->stockRow * 15 + 0x30, 6);
    }
}

void tm_drawDiscardWin(Window *win) {
    text_drawBlock(win->x + 0xA, win->y + 6, win->otz, text_get(TM_TEXT_DISCARD_ASK), TEXT_PAL_WHITE);
}

void *tm_taskBattleSelect(u32 *ot, void *prim, TmState *tm) {
    s32 pad;
    s32 ox;
    s32 oy;
    s32 i;
    s32 x;
    s32 y;
    TmCell *cell;
    TmCard *card;

    pad = g_padRepeat;
    ox = tm->boardX;
    oy = tm->boardY;
    if ((pad & PAD_RIGHT) && ox != TM_BOARD_SIZE - 1 && ox != tm->srcX + 1) {
        tm->boardX = ox + 1;
    }
    if ((pad & PAD_LEFT) && tm->boardX != 0 && tm->boardX != tm->srcX - 1) {
        tm->boardX--;
    }
    if ((pad & PAD_UP) && tm->boardY != 0 && tm->boardY != tm->srcY - 1) {
        tm->boardY--;
    }
    if ((pad & PAD_DOWN) && tm->boardY != TM_BOARD_SIZE - 1 && tm->boardY != tm->srcY + 1) {
        tm->boardY++;
    }
    if (ox != tm->boardX || oy != tm->boardY) {
        tm_playSe(TM_SE_CURSOR);
    }
    if (pad & BTN_CROSS) {
        cell = tm_getCell(tm, tm->boardX, tm->boardY);
        if (cell->state == TM_CELL_CARD) {
            card = cell->card;
            for (i = 0; i < tm->numTargets; i++) {
                if (card == tm->targets[i]) {
                    tm->timer = 0;
                    tm->task = TM_TASK_BATTLE;
                    tm->dstX = tm->boardX;
                    tm->dstY = tm->boardY;
                }
            }
        }
        if (tm->task != TM_TASK_BATTLE) {
            tm_playSe(TM_SE_BUZZER);
        }
    }
    x = tm->boardX * 43;
    y = tm->boardY * 52;
    prim = tm_drawCursor(ot, prim, tm, x + 0x39, y + 0xE, 0);
    g_menuHelp.x = x + 0x3B;
    g_menuHelp.y = y + 0xF;
    g_menuHelp.text = text_get(TM_HELP_NO_BATTLE);
    cell = tm_getCell(tm, tm->boardX, tm->boardY);
    if (cell->state == TM_CELL_CARD) {
        card = cell->card;
        for (i = 0; i < tm->numTargets; i++) {
            if (card == tm->targets[i]) {
                g_menuHelp.text = text_get(TM_HELP_BATTLE);
            }
        }
    }
    help_setBounds(NULL);
    return prim;
}

void tm_taskCardLess5(TmState *tm) {
    if (g_padRepeat & (BTN_CIRCLE | BTN_CROSS)) {
        win_close(tm->less5Win);
        tm_startEndFade(tm);
    }
}

void tm_waitWindow(Window *win) {
    win_setOnOpened(win, (void (*)(Window *))tm_onWindowOpened);
    tm_setWindowOpening(1);
}

void tm_onWindowOpened(void) {
    tm_setWindowOpening(0);
}

void tm_setWindowOpening(s32 opening) {
    g_tmWindowOpening = opening;
}

void tm_setHelpArea(s16 width) {
    RECT rect;
    rect.x = 0;
    rect.y = 0;
    rect.w = width;
    rect.h = 0xF0;
    help_setBounds(&rect);
}

void *tm_taskGetSelect(u32 *ot, void *prim, TmState *tm) {
    RECT helpRect;
    RECT r2;
    u8 msg[64];
    u8 num[16];
    s32 pad;
    TmCard *sel;
    TmCard *card;
    TmFlip *flipArr;
    s32 x;
    s32 y;
    s32 i;
    s32 j;
    s32 copies;
    TmCard *cards;
    s32 k;

    pad = g_padRepeat;
    if ((pad & PAD_UP) && tm->getCursor != 0) {
        tm->getCursor--;
        tm_playSe(TM_SE_CURSOR);
    }
    if ((pad & PAD_DOWN) && tm->getCursor != tm_countCaptured(tm) - 1) {
        tm->getCursor++;
        tm_playSe(TM_SE_CURSOR);
    }
    sel = tm_getCaptured(tm, tm->getCursor);
    x = sel->x;
    y = sel->y;
    prim = tm_drawCursor(ot, prim, tm, x + 0x22, y + 5, 2);
    g_menuHelp.x = x + 0x25;
    g_menuHelp.y = y + 6;
    g_menuHelp.text = text_get(TM_HELP_TAKE);
    helpRect.x = x + 0x2A;
    helpRect.w = 0x12C;
    helpRect.y = 0;
    helpRect.h = 0xF0;
    help_setBounds(&helpRect);
    if (pad & BTN_CROSS) {
        if (tm->hasLostCard != 0 && sel == &tm->cards[tm->lostSlot + 5]) {
            tm->hasLostCard = 0;
        }
        tm_clearFlips(tm);
        tm_clearFlips(tm);
        k = 0;
        flipArr = tm->flips;
        cards = tm->cards;
        for (i = 0; i < TM_NUM_CARDS; i++) {
            card = &cards[i];
            if (i < TM_HAND_SIZE) {
                if (card->owner == TM_ENEMY) {
                    flipArr[k].card = card;
                    flipArr[k].mode = TM_FLIP_CHANGE;
                    flipArr[k].arg = 0;
                    k++;
                }
            } else if (card->owner == TM_PLAYER && sel != card) {
                flipArr[k].card = card;
                flipArr[k].mode = TM_FLIP_CHANGE;
                flipArr[k].arg = 1;
                k++;
            }
        }
        tm->flashMode = TM_FLIP_CHANGE;
        tm->task = TM_TASK_FLASH2;
    }
    copies = 0;
    cards = tm->cards;
    for (i = 0; i < TM_NUM_CARDS; i++) {
        if (i >= TM_HAND_SIZE && tm_getCaptured(tm, tm->getCursor) == &cards[i]) {
            copies = card_countOf(cards[i].id);
            for (j = 0; j < TM_HAND_SIZE; j++) {
                if (cards[i].id == cards[j].id) {
                    copies++;
                }
            }
        }
    }
    text_formatNumber(copies, num, TEXT_NUM_NO_PAD, TEXT_NUM_CODES);
    text_copy(msg, text_get(TM_TEXT_STOCK));
    text_append(msg, num);
    text_append(msg, text_get(TM_TEXT_STOCK_END));
    r2.w = text_measureWidth((s32)msg) + 0x10;
    r2.x = 0x52;
    r2.y = sel->y - 9;
    r2.h = 0x19;
    text_drawBlock(0x5A, r2.y + 6, 3, msg, TEXT_PAL_WHITE);
    win_drawBox(&r2, 3, WIN_STYLE_BEVEL);
    return prim;
}

void tm_taskGameQuit(TmState *tm) {
    s32 pad;
    s32 i;
    TmCard *card;

    if (g_tmWindowOpening == 0) {
        pad = g_padRepeat;
        if (pad & PAD_UP) {
            if (tm->quitChoice != TM_SEL_YES) {
                tm->quitChoice = TM_SEL_YES;
            } else {
                tm->quitChoice = TM_SEL_NO;
            }
            tm_playSe(TM_SE_CURSOR);
        }
        if (pad & PAD_DOWN) {
            if (tm->quitChoice != TM_SEL_NO) {
                tm->quitChoice = TM_SEL_NO;
            } else {
                tm->quitChoice = TM_SEL_YES;
            }
            tm_playSe(TM_SE_CURSOR);
        }
        if (pad & BTN_CIRCLE) {
            if (tm->quitChoice != TM_SEL_NO) {
                tm->quitChoice = TM_SEL_NO;
                tm_playSe(TM_SE_CANCEL);
            }
        } else if (pad & BTN_CROSS) {
            if (tm->quitChoice == TM_SEL_YES) {
                for (i = 0; i < tm->handCount; i++) {
                    card = &tm->cards[i];
                    card_addWithStats(card->id, card->arrows, card->power, card->type,
                                  card->pdef, card->mdef);
                }
                tm->handCount = 0;
                card_setGameResult(1);
                tm->quitting = 1;
                tm_startEndFade(tm);
            } else {
                tm->task = TM_TASK_CHOICE;
            }
            win_close(tm->quitWin);
            tm_playSe(TM_SE_CURSOR);
        }
        if (tm->quitChoice == TM_SEL_YES) {
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

void tm_drawQuitWin(Window *win) {
    text_drawBlock(win->x + 9, win->y + 6, win->otz, text_get(TM_TEXT_QUIT_ASK), TEXT_PAL_WHITE);
}

void *tm_drawBattleSelect(u32 *ot, void *prim, TmState *tm) {
    return prim;
}

void tm_checkBattles(TmState *tm) {
    TmPos pos;
    s32 targetCount;
    s32 dir;
    s32 y;
    u8 arrows;
    s32 owner;
    TmCard *card;
    s32 unused;

    targetCount = 0;
    tm->valuesTimer = 0;
    card = tm_getCardAt(tm, tm->srcX, tm->srcY);
    owner = card->owner;
    arrows = card->arrows;
    for (dir = 0; dir < TM_NUM_DIRS; dir++) {
        if (arrows & 1) {
            pos.x = tm->srcX;
            pos.y = tm->srcY;
            if (tm_getDefender(tm, &pos, dir, owner) != NULL) {
                tm->dstX = pos.x;
                y = pos.y;
                tm->timer = 0;
                tm->dstY = y;
                tm->attacker = tm_getCell(tm, tm->srcX, tm->srcY)->card;
                tm->targets[targetCount] = tm_getCell(tm, pos.x, pos.y)->card;
                targetCount++;
            }
        }
        arrows >>= 1;
    }
    if (targetCount == 1) {
        tm->task = TM_TASK_BATTLE;
    } else if (targetCount >= 2) {
        if (tm->turn == TM_PLAYER && tm->autoPlay == 0) {
            tm->timer = 0;
            tm->numTargets = targetCount;
            tm->task = TM_TASK_BATTLE_SELECT;
        } else {
            tm->task = TM_TASK_BATTLE;
        }
    } else if (tm_evalPlacement(tm, owner, tm->srcX, tm->srcY, (TmCard *)-1) != 0) {
        tm_setFlips(tm, owner, tm->srcX, tm->srcY, TM_TASK_TURN, TM_FLIP_CHANGE);
    } else {
        tm->task = TM_TASK_TURN;
    }
}

void *tm_taskBattle(u32 *ot, void *vp, TmState *tm) {
    SPRT *prim = vp;
    TmBattle pos;
    TmCard *atkCard;
    TmCard *defCard;
    TmCard *winner;
    TmFlip *flip;
    s32 unused;
    s16 x;
    s16 y;
    s32 h;
    u8 bright;
    u32 *otSlot;

    atkCard = tm_getCardAt(tm, tm->srcX, tm->srcY);
    defCard = tm_getCardAt(tm, tm->dstX, tm->dstY);
    tm_getBattleValues((s32)tm, &pos, atkCard, defCard);
    tm->valuesTimer = 0x3C;
    if (tm->timer == TM_FIGHT_CLASH) {
        tm_playSe(TM_SE_BATTLE);
    }
    if (tm->timer < TM_FIGHT_CLASH) {
        tm->atkShown = pos.atk;
        tm->defShown = pos.def;
    } else if (tm->timer < TM_FIGHT_ROLL) {
        x = (tm->srcX + tm->dstX) * 43 / 2 + 0x48;
        y = (tm->srcY + tm->dstY) * 26 + 9;
        bright = tm->brightness;
        h = 0x30;
        if (tm->timer - TM_FIGHT_CLASH < 0xF) {
            h = 0x28;
        }
        setlen(prim, PRIM_LEN(SPRT));
        prim->code = GPU_SPRT | GPU_ABE;
        prim->x0 = x;
        prim->y0 = y;
        prim->r0 = bright;
        prim->g0 = bright;
        prim->b0 = bright;
        prim->w = 0x28;
        prim->h = h;
        prim->u0 = (((tm->timer - TM_FIGHT_CLASH) & 0xF) >> 2) * 40;
        prim->v0 = ((tm->timer - TM_FIGHT_CLASH) / 16) * 40;
        otSlot = ot + 30;
        prim->clut = getClut(0, 0xEC);
        AddPrim(otSlot, prim);
        prim++;
        setDrawTPage((DR_TPAGE *)prim, 0, 0, GetTPage(1, 1, 0x200, 0x100));
        AddPrim(otSlot, prim);
        tm->atkShown = pos.atk;
        prim = (SPRT *)((DR_TPAGE *)prim + 1);
        tm->defShown = pos.def;
    } else {
        if (tm->timer == TM_FIGHT_ROLL) {
            tm->atkRoll = rand() % pos.atk;
            tm->defRoll = rand() % pos.def;
        }
        tm->atkShown = pos.atk - (pos.atk - tm->atkRoll) * (tm->timer - TM_FIGHT_ROLL) / 23;
        tm->defShown = pos.def - (pos.def - tm->defRoll) * (tm->timer - TM_FIGHT_ROLL) / 23;
        if (tm->timer >= TM_FIGHT_END) {
            flip = tm->flips;
            tm_clearFlips(tm);
            flip->mode = TM_FLIP_BATTLE;
            tm->flashMode = TM_FLIP_BATTLE;
            tm->task = TM_TASK_FLASH;
            tm_playSe(TM_SE_FLASH);
            if (tm->atkRoll > tm->defRoll) {
                winner = atkCard;
                flip->x = tm->dstX;
                flip->y = tm->dstY;
                tm->flashNext = TM_TASK_BATTLE_WIN;
                tm->flipOwner = tm->turn;
            } else {
                if (tm->turn == TM_PLAYER) {
                    tm->flipOwner = TM_ENEMY;
                } else {
                    tm->flipOwner = TM_PLAYER;
                }
                flip->x = tm->srcX;
                flip->y = tm->srcY;
                winner = defCard;
                tm->flashNext = TM_TASK_BATTLE_LOSE;
            }
            switch (winner->type) {
            case TM_TYPE_P:
            case TM_TYPE_M:
                if ((rand() & 0x3F) == 0) {
                    winner->type = TM_TYPE_X;
                }
                break;
            case TM_TYPE_X:
                if ((rand() & 0x7F) == 0) {
                    winner->type = TM_TYPE_A;
                }
                break;
            }
            tm->atkShown = tm->atkRoll;
            tm->defShown = tm->defRoll;
        }
    }
    return prim;
}

void tm_getBattleValues(s32 unused, TmBattle *out, TmCard *atkCard, TmCard *defCard) {
    s32 atk;
    s32 def;
    s32 aatk;
    s32 apdef;
    s32 amdef;
    s32 batk;
    s32 bpdef;
    s32 bmdef;

    atk = 0;
    aatk = atkCard->power;
    apdef = atkCard->pdef;
    amdef = atkCard->mdef;
    batk = defCard->power;
    bpdef = defCard->pdef;
    bmdef = defCard->mdef;
    def = 0;
    switch (atkCard->type) {
    case TM_TYPE_P:
        atk = aatk;
        def = bpdef;
        break;
    case TM_TYPE_M:
        atk = aatk;
        def = bmdef;
        break;
    case TM_TYPE_X:
        atk = aatk;
        def = bpdef;
        if (bmdef < def) {
            def = bmdef;
        }
        break;
    case TM_TYPE_A:
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

void tm_taskBattleWin(TmState *tm) {
    if (tm_evalPlacement(tm, tm->turn, tm->dstX, tm->dstY, (TmCard *)-1) != 0) {
        tm_setFlips(tm, tm->turn, tm->dstX, tm->dstY, -1, TM_FLIP_COMBO);
    } else {
        tm_checkBattles(tm);
    }
}

void tm_taskBattleLose(TmState *tm) {
    s32 who = tm->turn == TM_PLAYER;
    if (tm_evalPlacement(tm, who, tm->srcX, tm->srcY, (TmCard *)-1) != 0) {
        tm_setFlips(tm, who, tm->srcX, tm->srcY, TM_TASK_TURN, TM_FLIP_COMBO);
    } else {
        tm->task = TM_TASK_TURN;
    }
}

void tm_taskTurn(TmState *tm) {
    if (tm->turn == TM_PLAYER) {
        tm->turn = TM_ENEMY;
        tm->timer = 0;
        tm->phase = 0;
        tm->thinkDelay = (rand() & 0xF) + 0xF;
        tm->task = TM_TASK_ENEMY_THINK;
    } else {
        tm->nameSlide = 0;
        tm->turn = TM_PLAYER;
        tm->task = TM_TASK_PLAYER_SELECT;
    }
}

void *tm_drawFlash(u32 *ot, TILE *prim, TmState *tm) {
    TmFlashTbl tbl;
    TmFlash *bar;
    TmFlip *flipArr;
    s32 i;
    s32 tick;
    s32 bright;
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
    flipArr = tm->flips;
    tick = tm->timer;
    if (tm->phase == 0) {
        k = tick * 30;
        if (k >= 0x100) {
            bright = 0xFF;
            if (tm->flashMode == TM_FLIP_COMBO) {
                tm->comboTimer = 0x32;
                tm->valuesTimer = 0;
                tm->comboShown = tm->comboCount;
                tm_playSe(TM_SE_COMBO);
            }
            tm->phase = 1;
        } else {
            bright = k;
        }
    } else {
        k = 0xFF - (tick - 10) * 15;
        bright = k;
        if (k < 0) {
            bright = 0;
            if (tm->flashNext == -1) {
                tm_checkBattles(tm);
            } else {
                tm->task = tm->flashNext;
            }
        }
    }
    for (i = 0; i < TM_NUM_CARDS; i++) {
        if (flipArr[i].mode != TM_FLIP_NONE) {
            if (tm->phase == 1) {
                tm_getCell(tm, flipArr[i].x, flipArr[i].y)->card->owner = tm->flipOwner;
            }
            if (tm->flashMode == TM_FLIP_CHANGE) {
                setlen(prim, PRIM_LEN(TILE));
                prim->code = GPU_TILE | GPU_ABE;
                setXY0(prim, flipArr[i].x * TM_SQUARE_W + TM_BOARD_X, flipArr[i].y * TM_SQUARE_H + TM_BOARD_Y);
                setWH(prim, TM_CARD_W, TM_CARD_H);
                setRGB0(prim, bright, bright, bright);
                AddPrim(ot + 0x1E, prim);
                prim++;
            }
            if (tm->flashMode == TM_FLIP_COMBO) {
                k = 0xFF - tick * 9;
                bright = k;
                bar = &tbl.v[flipArr[i].arg];
                dx = bar->dirX;
                dy = bar->dirY;
                ex = bar->cornerX;
                ey = bar->cornerY;
                bx = flipArr[i].x * TM_SQUARE_W + TM_BOARD_X + bar->offX;
                by = flipArr[i].y * TM_SQUARE_H + TM_BOARD_Y + bar->offY;
                if (k < 0) {
                    bright = 0;
                }
                switch (bar->shape) {
                case 0:
                    x = bx;
                    w = 0x2A;
                    h = 4;
                    y = by - (tick * 51 / 28) * dy;
                    break;
                case 1:
                    x = bx + tick * 42 / 28 * dx;
                    w = tick * 42 / 28;
                    h = 4;
                    y = by - (tick * 51 / 28) * dy;
                    if (dy == -1) {
                        y -= 4;
                    }
                    setlen(prim, PRIM_LEN(TILE));
                    prim->code = GPU_TILE | GPU_ABE;
                    setXY0(prim, x, y);
                    setWH(prim, w, h);
                    setRGB0(prim, bright, bright, bright);
                    AddPrim(ot + 0x1E, prim);
                    prim++;
                    x = bx + w * ex;
                    w = 4;
                    y = by - (tick * 51 / 28) * ey;
                    h = tick * 51 / 28;
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
                    x = bx + tick * 42 / 28 * dx;
                    w = 4;
                    h = 0x33;
                    break;
                }
                setlen(prim, PRIM_LEN(TILE));
                prim->code = GPU_TILE | GPU_ABE;
                setXY0(prim, x, y);
                setWH(prim, w, h);
                setRGB0(prim, bright, bright, bright);
                AddPrim(ot + 0x1E, prim);
                prim++;
            }
            if (tm->flashMode == TM_FLIP_BATTLE) {
                setlen(prim, PRIM_LEN(TILE));
                prim->code = GPU_TILE | GPU_ABE;
                setXY0(prim, flipArr[i].x * TM_SQUARE_W + TM_BOARD_X, flipArr[i].y * TM_SQUARE_H + TM_BOARD_Y);
                setWH(prim, TM_CARD_W, TM_CARD_H);
                if (tm->timer & 4) {
                    setRGB0(prim, 0x80, 0x80, 0x80);
                } else {
                    setRGB0(prim, 0x76, 0x76, 0x76);
                }
                AddPrim(ot + 0x1E, prim);
                prim++;
            }
        }
    }
    /* Dead store, no code: a reference to k after case 2's `if (k < 0)` keeps cse from rewriting that test
     * to use bright (make_regs_eqv picks the register whose last use comes later), so the test stays on k (a2). */
    k = 0;
    SetDrawTPage((DR_TPAGE *)prim, 0, 0, GetTPage(0, 1, 0, 0));
    AddPrim(ot + 0x1E, prim);
    prim = (TILE *)((DR_TPAGE *)prim + 1);
    return prim;
}

void *tm_drawFlash2(u32 *ot, TILE *prim, TmState *tm) {
    s32 level;
    u8 bright;
    s32 i;
    TmFlip *flipArr;
    TmCard *card;

    flipArr = tm->flips;
    if (tm->phase == 0) {
        bright = level = tm->timer * 30;
        if (level >= 0x100) {
            bright = 0xFF;
            tm->timer = 0;
            tm->phase = 1;
        }
    } else {
        bright = level = 0xFF - tm->timer * 12;
        if (level < 0) {
            bright = 0;
            if (tm->result == TM_RESULT_WIN) {
                tm_returnHand(tm);
            } else {
                tm_returnKept(tm);
            }
            tm_nextTrade(tm);
        }
    }
    for (i = 0; i < TM_NUM_CARDS; i++) {
        if (flipArr[i].mode == TM_FLIP_CHANGE) {
            card = flipArr[i].card;
            if (tm->phase == 1) {
                card->owner = flipArr[i].arg;
            }
            setlen(prim, PRIM_LEN(TILE));
            prim->code = GPU_TILE | GPU_ABE;
            prim->x0 = card->x;
            prim->y0 = card->y;
            prim->w = 0x2A;
            prim->h = 0x33;
            prim->r0 = bright;
            prim->g0 = bright;
            prim->b0 = bright;
            AddPrim(ot + card->depth - 1, prim);
            prim++;
            SetDrawTPage(prim, 0, 0, GetTPage(0, 1, 0, 0));
            AddPrim(ot + card->depth - 1, prim);
            prim = (TILE *)((DR_TPAGE *)prim + 1);
        }
    }
    return prim;
}

void tm_taskFlashArrow(TmState *tm) {
    if (tm->timer >= 0x15) {
        tm->timer = 0;
        tm->task = TM_TASK_FLASH;
        tm_playSe(TM_SE_FLASH);
    }
}

void tm_clearFlips(TmState *tm) {
    s32 i;
    TmFlip *flipArr = tm->flips;
    tm->timer = 0;
    tm->phase = 0;
    for (i = 0; i < TM_NUM_CARDS; i++) {
        flipArr[i].mode = TM_FLIP_NONE;
        flipArr[i].x = 0;
        flipArr[i].y = 0;
    }
}

void tm_layoutCards(TmState *tm) {
    TmCard *atkCard;
    TmCard *defCard;
    TmCard *card;
    TmCard *cards;
    s32 dx;
    s32 dy;
    s32 k;
    s32 x;
    s32 enemyIdx;
    s32 i;
    s32 offset;
    s32 base;

    dx = 0;
    dy = 0;
    atkCard = NULL;
    defCard = NULL;
    k = 0;
    if (tm->task == TM_TASK_BATTLE) {
        atkCard = tm_getCell(tm, tm->srcX, tm->srcY)->card;
        defCard = tm_getCell(tm, tm->dstX, tm->dstY)->card;
        if ((u32)(tm->timer - 0x23) < 0xF) {
            k = tm->timer - 0x23;
            if (tm->timer >= 0x2A) {
                k = 0x32 - tm->timer;
            }
        } else {
            k = 0;
        }
        dx = (tm->srcX - tm->dstX) * 0x2A00 / 270;
        dy = (tm->srcY - tm->dstY) * 0x3300 / 270;
        k = k * k;
    }
    x = 8;
    cards = tm->cards;
    for (i = 0, enemyIdx = 0; i < TM_NUM_CARDS; i++) {
        card = &cards[i];
        if (tm->task == TM_TASK_CHOICE || tm->task == TM_TASK_GAME_QUIT || tm->quitting != 0) {
            card->depth = i + 0x28;
        } else {
            card->depth = 0x3C - i;
        }
        if (card->owner == TM_PLAYER) {
            if (card->onBoard == 0) {
                card->small = 0;
                card->y = x;
                if (tm_countPlayerHand(tm, TM_PLAYER) == TM_HAND_SIZE || tm->task == TM_TASK_CHOICE || tm->task == TM_TASK_GAME_QUIT || tm->quitting != 0) {
                    x += 0x2A;
                    card->x = TM_PLAYER_HAND_X + 4;
                    card->small = 1;
                } else {
                    x += 0x34;
                    card->small = 0;
                    card->x = TM_PLAYER_HAND_X;
                }
            }
            if (tm->task == TM_TASK_GET_SELECT && i >= TM_HAND_SIZE) {
                if (tm_getCaptured(tm, tm->getCursor) == card) {
                    card->x = 0x16;
                } else {
                    card->x = TM_ENEMY_HAND_X;
                }
            }
            if (tm->task == TM_TASK_FLASH2 && i >= TM_HAND_SIZE) {
                card->x = TM_ENEMY_HAND_X;
            }
        }
        if (card->owner == TM_ENEMY && card->onBoard == 0) {
            card->y = enemyIdx * 14 + 0x1A;
            enemyIdx++;
            card->x = TM_ENEMY_HAND_X;
        }
        if (tm->task == TM_TASK_BATTLE) {
            if (atkCard == card) {
                base = tm->srcX * 43;
                offset = dx * k / 256 - 0x48;
                card->x = base - offset;
                base = tm->srcY * 52;
                offset = dy * k / 256 - 8;
                card->y = base - offset;
                card->depth = 0x26;
            }
            if (defCard == card) {
                base = tm->dstX * 43;
                offset = dx * k / 256 + 0x48;
                card->x = base + offset;
                base = tm->dstY * 52;
                offset = dy * k / 256 + 8;
                card->y = base + offset;
                card->depth = 0x27;
            }
        }
    }
}

void *tm_drawCards(u32 *ot, void *prim, TmState *tm) {
    TmCard *card;
    TmCard *cards;
    s32 i;
    s32 k;
    s32 curTask;
    s32 z;
    s32 sel;
    u8 faceBright;
    u8 statBright;
    u32 *otSlot;
    u32 *o2;

    cards = tm->cards;
    for (i = 0; i < TM_NUM_CARDS; i++) {
        card = &cards[i];
        if (cards[i].visible == 0) {
            continue;
        }
        faceBright = tm->brightness;
        curTask = tm->task;
        statBright = faceBright;
        if (curTask == TM_TASK_BATTLE_SELECT && cards[i].onBoard == 1) {
            faceBright = 0x40;
            statBright = faceBright;
            for (k = 0; k < tm->numTargets; k++) {
                if (&cards[i] == tm->targets[k]) {
                    faceBright = tm->brightness - ((tm->timer & 0x10) >> 4) * 24;
                    statBright = tm->brightness;
                }
            }
            if (card == tm->attacker) {
                faceBright = tm->brightness;
                statBright = faceBright;
            }
        }
        if (curTask == TM_TASK_CHOICE || curTask == TM_TASK_CARD_AWAY || curTask == TM_TASK_CARD_AWAY2 || curTask == TM_TASK_GAME_QUIT || tm->quitting != 0) {
            if (i >= TM_HAND_SIZE) {
                faceBright -= (i - 5) * 22;
                statBright -= (i - 5) * 22;
            }
        }
        if (curTask == 1 && card->owner == TM_PLAYER && card->onBoard == 0) {
            statBright = 0x40;
            faceBright = statBright;
            if (card == tm_getHandCard(tm, TM_PLAYER, tm->cursor)) {
                statBright = 0x80;
                faceBright = statBright;
            }
        }
        z = card->depth;
        if (tm->task == TM_TASK_BATTLE_SELECT && faceBright != statBright) {
            otSlot = ot + z;
            setSprt((SPRT *)prim);
            ((SPRT *)prim)->x0 = card->x + 5;
            ((SPRT *)prim)->y0 = card->y + 0x15;
            ((SPRT *)prim)->v0 = 0xF0;
            ((SPRT *)prim)->w = 0x20;
            ((SPRT *)prim)->h = 0x10;
            ((SPRT *)prim)->u0 = 0x80;
            ((SPRT *)prim)->clut = getClut(0, 0xED);
            ((SPRT *)prim)->r0 = 0x80;
            ((SPRT *)prim)->g0 = 0x80;
            ((SPRT *)prim)->b0 = 0x80;
            AddPrim(otSlot, prim);
            prim = (SPRT *)prim + 1;
            setDrawTPage((DR_TPAGE *)prim, 0, 0, GetTPage(0, 0, 0x340, 0x1E0));
            AddPrim(otSlot, prim);
            prim = (DR_TPAGE *)prim + 1;
        }
        if (tm->task == TM_TASK_GET_SELECT && tm_getCaptured(tm, tm->getCursor) == card) {
            z = 0x28;
        }
        if (card->hidden == 0) {
            sel = 0;
            if (tm->task == TM_TASK_FLASH_ARROW) {
                sel = tm->comboCard == card;
            }
            o2 = ot + z;
            prim = tm_drawArrows(o2, prim, tm, card->x, card->y, card->arrows, card->small, statBright, sel);
            if (card->hidden == 0) {
                prim = tm_drawCardStats(o2, prim, tm, card, statBright);
                if (card->hidden == 0) {
                    prim = tm_drawCardFace(o2, prim, tm, card, faceBright, card->small);
                }
            }
        }
        if (card->small == 0) {
            setSprt((SPRT *)prim);
            ((SPRT *)prim)->x0 = card->x;
            ((SPRT *)prim)->y0 = cards[i].y;
            ((SPRT *)prim)->w = 0x2A;
            ((SPRT *)prim)->h = 0x33;
            ((SPRT *)prim)->r0 = faceBright;
            ((SPRT *)prim)->g0 = faceBright;
            ((SPRT *)prim)->b0 = faceBright;
            if (cards[i].owner == TM_PLAYER) {
                ((SPRT *)prim)->u0 = 8;
            } else {
                ((SPRT *)prim)->u0 = 0x38;
            }
            ((SPRT *)prim)->v0 = 0x40;
            if (cards[i].hidden == 1) {
                ((SPRT *)prim)->u0 = 0xC8;
                ((SPRT *)prim)->v0 = 0x40;
            }
            ((SPRT *)prim)->clut = getClut(0, 0xEA);
            AddPrim(ot + z, prim);
            prim = (SPRT *)prim + 1;
        } else {
            SetPolyFT4((POLY_FT4 *)prim);
            ((POLY_FT4 *)prim)->x0 = cards[i].x;
            ((POLY_FT4 *)prim)->y0 = cards[i].y;
            ((POLY_FT4 *)prim)->x1 = cards[i].x + 0x22;
            ((POLY_FT4 *)prim)->y1 = cards[i].y;
            ((POLY_FT4 *)prim)->x2 = cards[i].x;
            ((POLY_FT4 *)prim)->y2 = cards[i].y + 0x29;
            ((POLY_FT4 *)prim)->x3 = cards[i].x + 0x22;
            ((POLY_FT4 *)prim)->y3 = cards[i].y + 0x29;
            ((POLY_FT4 *)prim)->u0 = 8;
            ((POLY_FT4 *)prim)->u1 = 0x32;
            ((POLY_FT4 *)prim)->v0 = 0x40;
            ((POLY_FT4 *)prim)->v1 = 0x40;
            ((POLY_FT4 *)prim)->u2 = 8;
            ((POLY_FT4 *)prim)->v2 = 0x73;
            ((POLY_FT4 *)prim)->u3 = 0x32;
            ((POLY_FT4 *)prim)->v3 = 0x73;
            ((POLY_FT4 *)prim)->tpage = getTPage(1, 0, 0x300, 0x100);
            ((POLY_FT4 *)prim)->r0 = faceBright;
            ((POLY_FT4 *)prim)->g0 = faceBright;
            ((POLY_FT4 *)prim)->b0 = faceBright;
            ((POLY_FT4 *)prim)->clut = getClut(0, 0xEA);
            AddPrim(ot + z, prim);
            prim = (POLY_FT4 *)prim + 1;
        }
        setDrawTPage((DR_TPAGE *)prim, 0, 0, GetTPage(1, 0, 0x300, 0x100));
        AddPrim(ot + z, prim);
        prim = (DR_TPAGE *)prim + 1;
    }
    return prim;
}

void tm_startCardMove(TmState *tm) {
    s32 i;
    TmCard *cardArr = tm->cards;
    for (i = 0; i < TM_NUM_CARDS; i++) {
        cardArr[i].fromX = cardArr[i].x << 8;
        cardArr[i].fromY = cardArr[i].y << 8;
        if (i < TM_HAND_SIZE) {
            cardArr[i].toX = (TM_PLAYER_HAND_X << 8);
            cardArr[i].toY = i * 0x2000 + 0x1900;
        } else {
            cardArr[i].toX = TM_ENEMY_HAND_X << 8;
            cardArr[i].toY = (i - 5) * 0x2000 + 0x1900;
        }
    }
    tm->moveFrames = 0x14;
    tm->timer = 0;
    tm->task = TM_TASK_CARD_MOVE;
}

void *tm_taskCardMove(u32 *ot, void *prim, TmState *tm) {
    TmCard *card;
    TmCard *cards;
    TmFlip *flipArr;
    s32 i;
    s32 j;
    s32 k;
    s32 skip;
    s32 flag;
    s32 ownedCount;
    s32 stepX;
    s32 stepY;
    s32 st;

    if (tm->timer < tm->moveFrames) {
        cards = tm->cards;
        for (i = 0; i < TM_NUM_CARDS; i++) {
            card = &cards[i];
            stepX = (card->toX - card->fromX) / tm->moveFrames;
            stepY = (card->toY - card->fromY) / tm->moveFrames;
            card->x = (card->fromX + stepX * tm->timer) >> 8;
            card->y = (card->fromY + stepY * tm->timer) >> 8;
        }
    } else {
        cards = tm->cards;
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
    if (tm->moveFrames + 10 < tm->timer) {
        if (tm->result == TM_RESULT_WIN) {
            if (tm_countOwned(tm, TM_PLAYER) != TM_NUM_CARDS && tm_countCaptured(tm) != 1) {
                tm->getCursor = 0;
                st = TM_TASK_GET_SELECT;
                goto done;
            }
        }
        if (tm->result == TM_RESULT_WIN) {
            if (tm->hasLostCard != 0 && (&tm->cards[TM_HAND_SIZE])[tm->lostSlot].owner == TM_PLAYER) {
                tm->hasLostCard = 0;
            }
        }
        flag = 0;
        if (tm->result == TM_RESULT_WIN) {
            ownedCount = tm_countOwned(tm, TM_PLAYER);
            skip = -1;
            if (ownedCount == TM_NUM_CARDS) {
                tm->hasLostCard = 0;
            }
        } else {
            ownedCount = tm_countKept(tm, TM_PLAYER);
            skip = (rand() & 0xFF) % (5 - ownedCount);
        }
        cards = tm->cards;
        flipArr = tm->flips;
        tm_clearFlips(tm);
        k = 0;
        j = 0;
        for (i = 0; i < TM_NUM_CARDS; i++) {
            card = &cards[i];
            if (i < TM_HAND_SIZE) {
                if (card->owner == TM_ENEMY) {
                    if (k != skip && tm_countOwned(tm, TM_ENEMY) != TM_NUM_CARDS) {
                        flipArr[j].card = card;
                        flipArr[j].mode = TM_FLIP_CHANGE;
                        flipArr[j].arg = 0;
                        j++;
                        flag = 1;
                    }
                    if (tm->result == TM_RESULT_LOSE) {
                        k++;
                    }
                }
            } else if (card->owner == TM_PLAYER) {
                st = tm->result;
                if (st == 1) {
                    flipArr[j].card = card;
                    flipArr[j].mode = st;
                    flipArr[j].arg = st;
                    j++;
                    flag = 1;
                }
            }
        }
        if (flag == 0) {
            if (tm->result == TM_RESULT_WIN) {
                tm_returnHand(tm);
            } else {
                tm_returnKept(tm);
            }
            tm_nextTrade(tm);
            return prim;
        }
        tm->flashMode = TM_FLIP_CHANGE;
        st = TM_TASK_FLASH2;
    done:
        tm->task = st;
    }
    return prim;
}

s32 tm_countCapturedBeforeLost(TmState *tm) {
    s32 count = 0;
    s32 i;
    TmCard *cardArr = tm->cards;
    for (i = 0; i < tm->lostSlot; i++) {
        if (cardArr[i + 5].owner == TM_PLAYER) {
            count++;
        }
    }
    return count;
}

s32 tm_countKept(TmState *tm, s32 owner) {
    s32 i;
    s32 count;
    TmCard *hand;
    if (owner == 0) {
        hand = &tm->cards[0];
    } else {
        hand = &tm->cards[TM_HAND_SIZE];
    }
    count = 0;
    for (i = 0; i < TM_HAND_SIZE; i++) {
        if (hand[i].owner == owner) {
            count++;
        }
    }
    return count;
}

void tm_nextTrade(TmState *tm) {
    RECT rect;
    s32 owner;
    s32 x;
    s32 i;
    s32 found;
    s32 j;
    TmCard *cardArr;

    owner = 1;
    if (tm->result == TM_RESULT_WIN) {
        owner = 0;
        j = 5;
        x = TM_ENEMY_HAND_X;
    } else {
        j = 0;
        x = TM_PLAYER_HAND_X;
    }
    found = 0;
    cardArr = tm->cards;
    for (i = 0; i < TM_HAND_SIZE; i++) {
        if ((cardArr + j)[i].owner == owner && (cardArr + j)[i].x == x) {
            found = 1;
            tm->tradeCard = &(cardArr + j)[i];
            tm->moveFrames = 0xA;
            tm->timer = 0;
            (cardArr + j)[i].fromX = (cardArr + j)[i].x << 8;
            (cardArr + j)[i].fromY = (cardArr + j)[i].y << 8;
            break;
        }
    }
    if (found == 0) {
        if (card_countAll() > CARD_MAX) {
            rect.w = text_measureWidth(text_get(TM_TEXT_OVER_100)) + 0x12;
            rect.x = 0xA0 - rect.w / 2;
            rect.y = 0x5B;
            rect.h = 0x2A;
            tm->infoWin = win_openFree(&rect, 5, WIN_STYLE_BEVEL);
            win_setDrawContent(tm->infoWin, tm_drawOver100Win);
            win_setShadow(tm->infoWin, 1);
            tm_waitWindow(tm->infoWin);
            tm_hideCards(tm);
            tm_resetStockCursor(tm);
            tm->task = TM_TASK_AWAY_INFO;
            tm_playSe(TM_SE_BUZZER);
        } else {
            tm_askRematch(tm);
        }
    } else {
        tm->task = TM_TASK_CARD_CHANGE;
        tm_playSe(TM_SE_CARD);
    }
}

void tm_drawOver100Win(Window *win) {
    text_drawBlock(win->x + 9, win->y + 7, win->otz, text_get(TM_TEXT_OVER_100), TEXT_PAL_WHITE);
}

void tm_drawDiscardTitle(Window *win) {
    text_drawBlock(0x82, 0x10, win->otz, text_get(TM_TEXT_DISCARD), TEXT_PAL_WHITE);
}

void *tm_taskCardChange(u32 *ot, void *prim, TmState *tm) {
    RECT rect;
    u8 msg[64];
    TmCard *card;
    TmCard *found;
    TmCard *cards;
    s32 i;
    s32 dx;
    s32 dy;
    s32 tradeLost;
    s32 w;
    u8 id;

    card = tm->tradeCard;
    if (tm->timer < tm->moveFrames) {
        dx = (0x8B00 - card->fromX) / tm->moveFrames;
        dy = (0x5500 - card->fromY) / tm->moveFrames;
        card->x = (card->fromX + dx * tm->timer) >> 8;
        card->y = (card->fromY + dy * tm->timer) >> 8;
        tm_stackHands(tm, 0);
        return prim;
    }
    if (tm->result == TM_RESULT_WIN) {
        g_tmTradeBadge = card_addWithStats(card->id, card->arrows, card->power, card->type, card->pdef, card->mdef);
    } else {
        tm->lostCard.id = card->id;
        tm->lostCard.power = card->power;
        tm->lostCard.arrows = card->arrows;
        tm->lostCard.type = card->type;
        tm->lostCard.pdef = card->pdef;
        tm->lostCard.mdef = card->mdef;
        tm->hasLostCard = 1;
        g_tmTradeBadge = 0;
        if (card_countOf(card->id) == 0) {
            found = NULL;
            cards = tm->cards;
            for (i = 0; i < TM_HAND_SIZE; i++) {
                if (cards[i].id == card->id) {
                    found = &cards[i];
                }
            }
            if (found == card) {
                g_tmTradeBadge = 1;
            }
        }
    }
    tradeLost = tm->result;
    card->x = 0x8B;
    card->y = 0x55;
    g_tmTradeLost = tradeLost;
    g_tmTradeCardId = id = card->id;
    text_copy(msg, (u8 *)card_getName(id));
    if (g_tmTradeLost == 0) {
        text_append(msg, text_get(TM_TEXT_RECEIVED));
    } else {
        text_append(msg, text_get(TM_TEXT_LOST));
    }
    text_append(msg, text_get(TM_TEXT_TRADE_END));
    w = text_measureWidth(msg) + 0x10;
    rect.x = 0xA0 - w / 2;
    rect.y = 0x96;
    rect.w = w;
    rect.h = 0x1A;
    tm->msgWin = win_openFree(&rect, 5, WIN_STYLE_BEVEL);
    win_setDrawContent(tm->msgWin, tm_drawTradeWin);
    win_setShadow(tm->msgWin, 1);
    tm_waitWindow(tm->msgWin);
    tm_playSe(tm->result == TM_RESULT_WIN ? TM_SE_RECEIVED : TM_SE_LOST);
    tm->task = TM_TASK_TRADE;
    return prim;
}

void *tm_drawTradeBadge(u32 *ot, SPRT *prim, TmState *tm) {
    u8 bright;
    DR_TPAGE *tp;

    bright = tm->brightness;
    if (g_tmTradeBadge != 0) {
        setSprt(prim);
        prim->x0 = 0xA3;
        prim->y0 = 0x45;
        if (tm->result == TM_RESULT_WIN) {
            prim->u0 = 0x20;
            prim->v0 = 0x90;
        } else {
            prim->u0 = 0xC0;
            prim->v0 = 0x80;
        }
        prim->w = 0x38;
        prim->h = 0x18;
        prim->clut = getClut(0, 0xEB);
        prim->r0 = bright;
        prim->g0 = bright;
        prim->b0 = bright;
        AddPrim(ot + 30, prim);
        prim++;
        tp = (DR_TPAGE *)prim;
        setDrawTPage(tp, 0, 0, GetTPage(1, 0, 0x300, 0x100));
        AddPrim(ot + 30, tp);
        prim = (SPRT *)(tp + 1);
    }
    return prim;
}

void tm_drawTradeWin(Window *w) {
    u8 msg[64];
    Window *win;

    win = w;
    if (g_tmTradeLost == 0) {
        text_copy(msg, text_get(TM_TEXT_RECEIVED));
    } else {
        text_copy(msg, text_get(TM_TEXT_LOST));
    }
    text_append(msg, (u8 *)card_getName(g_tmTradeCardId));
    text_append(msg, text_get(TM_TEXT_TRADE_END));
    text_drawBlock(0x9F - text_measureWidth((s32)msg) / 2, win->y + 6, w->otz, msg, TEXT_PAL_WHITE);
}

void tm_endTrade(TmState *tm) {
    TmCard *card = tm->tradeCard;
    card->depth = 0x28;
    card->fromX = card->x << 8;
    card->fromY = card->y << 8;
    tm->timer = 0;
    tm->task = TM_TASK_CHANGE2;
    win_close(tm->msgWin);
}

void *tm_taskChange2(u32 *ot, void *prim, TmState *tm) {
    TmCard *card;
    s32 dx;
    s32 dy;

    card = tm->tradeCard;
    if (tm->timer < tm->moveFrames) {
        if (tm->result == TM_RESULT_WIN) {
            dx = ((TM_PLAYER_HAND_X << 8) - card->fromX) / tm->moveFrames;
        } else {
            dx = ((TM_ENEMY_HAND_X << 8) - card->fromX) / tm->moveFrames;
        }
        dy = (0x9900 - card->fromY) / tm->moveFrames;
        card->x = (card->fromX + dx * tm->timer) >> 8;
        card->y = (card->fromY + dy * tm->timer) >> 8;
    } else {
        card->y = 0x19;
        if (tm->result == TM_RESULT_WIN) {
            card->x = TM_PLAYER_HAND_X;
        } else {
            card->x = TM_ENEMY_HAND_X;
        }
        tm_stackHands(tm, 1);
        tm_nextTrade(tm);
    }
    return prim;
}

void tm_stackHands(TmState *tm, s32 flag) {
    s32 side;
    s32 count;
    s32 i;
    s32 j;
    s32 x;
    s32 y;
    s32 step;
    TmCard *cardArr;
    TmCard *first;
    TmCard *second;

    for (side = 0; side < 2; side++) {
        if (side == 0) {
            first = &tm->cards[0];
            second = &tm->cards[TM_HAND_SIZE];
            x = TM_PLAYER_HAND_X;
        } else {
            first = &tm->cards[TM_HAND_SIZE];
            second = &tm->cards[0];
            x = TM_ENEMY_HAND_X;
        }
        count = 0;
        cardArr = tm->cards;
        for (i = 0; i < TM_NUM_CARDS; i++) {
            if (cardArr[i].x == x) {
                count++;
            }
        }
        step = 0x20;
        if (count >= 5) {
            step = 0x80 / (count - 1);
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

s32 tm_countInHand(TmState *tm, s32 owner) {
    s32 count = 0;
    s32 i;
    TmCard *cardArr = tm->cards;
    for (i = 0; i < TM_NUM_CARDS; i++) {
        if (cardArr[i].owner == owner && cardArr[i].onBoard == 0) {
            count++;
        }
    }
    return count;
}

s32 tm_countPlayerHand(TmState *tm, s32 owner) {
    s32 count = 0;
    s32 i;
    TmCard *cardArr = tm->cards;
    for (i = 0; i < TM_HAND_SIZE; i++) {
        if (cardArr[i].visible != 0 && cardArr[i].owner == owner && cardArr[i].onBoard == 0) {
            count++;
        }
    }
    return count;
}

s32 tm_countCaptured(TmState *tm) {
    s32 count = 0;
    s32 i;
    TmCard *enemyHand = tm->cards + 5;
    for (i = 0; i < TM_HAND_SIZE; i++) {
        if (enemyHand[i].owner == TM_PLAYER) {
            count++;
        }
    }
    return count;
}

TmCard *tm_getCaptured(TmState *tm, s32 idx) {
    s32 i;
    TmCard *base;
    TmCard *card;
    i = 0;
    card = base = &tm->cards[TM_HAND_SIZE];
    for (; i < TM_HAND_SIZE; i++, card++) {
        if (card->owner == TM_PLAYER) {
            if (idx == 0) {
                return card;
            }
            idx--;
        }
    }
    return base;
}

s32 tm_countOnBoard(TmState *tm, s32 owner) {
    s32 count = 0;
    s32 i;
    TmCard *cardArr = tm->cards;
    for (i = 0; i < TM_NUM_CARDS; i++) {
        if (cardArr[i].onBoard == 1 && cardArr[i].owner == owner) {
            count++;
        }
    }
    return count;
}

s32 tm_countOwned(TmState *tm, s32 owner) {
    s32 count = 0;
    s32 i;
    TmCard *cardArr = tm->cards;
    for (i = 0; i < TM_NUM_CARDS; i++) {
        if (cardArr[i].owner == owner) {
            count++;
        }
    }
    return count;
}

TmCard *tm_getHandCard(TmState *tm, s32 owner, s32 idx) {
    s32 handIdx = 0;
    s32 i;
    TmCard *cardArr = tm->cards;
    for (i = 0; i < TM_NUM_CARDS; i++) {
        if (cardArr[i].owner == owner && cardArr[i].onBoard == 0) {
            if (idx == handIdx) {
                return &cardArr[i];
            }
            handIdx++;
        }
    }
    return NULL;
}

void tm_initHands(TmState *tm) {
    TmCard *card;
    TmCard *base;
    TmCardData *tbl;
    s32 mdefVal;
    s32 i;
    s32 typeRoll;
    u8 id;

    tbl = g_tmCardData;
    base = tm->cards;

    for (i = 0; i < TM_NUM_CARDS; i++) {
        card = &base[i];
        card->x = 0xDC;
        card->y = i * 30;
        card->owner = TM_PLAYER;
        card->onBoard = 0;
        card->hidden = 0;
        card->depth = 0x28;
        card->id = 0;
        card->arrows = rand();
        card->power = rand();
        typeRoll = rand() & 3;
        switch (typeRoll) {
        case 0:
            card->type = TM_TYPE_P;
            break;
        case 1:
            card->type = typeRoll;
            break;
        case 2:
            card->type = TM_TYPE_A;
            break;
        case 3:
            card->type = TM_TYPE_X;
            break;
        }
        card->pdef = rand();
        card->mdef = rand();
        id = i;
        card->id = id;
        card->arrows = rand();
        card->power = tbl[id].power;
        card->type = tbl[id].type;
        card->pdef = tbl[id].pdef;
        mdefVal = tbl[id].mdef;
        card->visible = 0;
        card->mdef = mdefVal;
    }
    for (i = TM_HAND_SIZE; i < TM_NUM_CARDS; i++) {
        base[i].visible = 0;
    }
}

void tm_dealEnemyHand(TmState *tm) {
    s32 i;
    s32 roll;
    s32 k;
    u8 id;
    TmCard *card;
    TmCard *base;
    TmDeck *odds;

    odds = g_tmDecks;
    tm->lostSlot = rand() % TM_HAND_SIZE;
    base = &tm->cards[TM_HAND_SIZE];
    for (i = 0; i < TM_HAND_SIZE; i++) {
        card = &base[i];
        card->x = TM_ENEMY_HAND_X;
        card->y = 0x12C;
        card->owner = TM_ENEMY;
        card->onBoard = 0;
        card->hidden = 1;
        card->small = 0;
        card->depth = 0x28;
        roll = rand() & 0xFF;
        if (roll < 20) {
            k = 0;
        } else if (roll < 40) {
            k = 1;
        } else if (roll < 60) {
            k = 2;
        } else if (roll < 80) {
            k = 3;
        } else if (roll < 100) {
            k = 4;
        } else if (roll < 120) {
            k = 5;
        } else if (roll < 140) {
            k = 6;
        } else if (roll < 160) {
            k = 7;
        } else if (roll < 180) {
            k = 8;
        } else if (roll < 200) {
            k = 9;
        } else if (roll < 220) {
            k = 10;
        } else if (roll < 240) {
            k = 11;
        } else if (roll < 252) {
            k = 12;
        } else if (roll < 254) {
            k = 13;
        } else if (roll < 255) {
            k = 14;
        } else {
            k = 15;
        }
        id = odds[tm->deck].ids[k];
        if (tm->autoPlay != 0) {
            id = rand() % CARD_KINDS;
        }
        if (i == tm->lostSlot && tm->hasLostCard != 0) {
            card->id = tm->lostCard.id;
            card->power = tm->lostCard.power;
            card->arrows = tm->lostCard.arrows;
            card->type = tm->lostCard.type;
            card->pdef = tm->lostCard.pdef;
            card->mdef = tm->lostCard.mdef;
        } else {
            tm_makeCard(tm, card, id, 1);
        }
        card->visible = 1;
    }
}

TmCard *tm_getCardAt(TmState *tm, s32 x, s32 y) {
    return tm_getCell(tm, x, y)->card;
}

INCLUDE_RODATA("asm/us/ovl_0d/nonmatchings/tm_game", g_tmFlashBars);

/* "NO" label */
INCLUDE_RODATA("asm/us/ovl_0d/nonmatchings/tm_game", g_tmNoLabel);

void tm_showStockCard(TmState *tm) {
    u8 numText[0x20];
    TmCard *card;
    TmCard *c0;
    TmCard *handCard;
    TmCard *cardPtr;
    CardEntry *entry;
    s32 i;
    s32 copies;
    s32 shown;
    s32 k;
    s32 dx;
    s32 w;

    cardPtr = tm->cards;
    for (i = 0; i < TM_HAND_SIZE; i++) {
        handCard = &cardPtr[i];
        if (i < tm->handCount) {
            handCard->visible = 1;
        } else {
            handCard->visible = 0;
        }
    }
    for (i = 4, cardPtr = &tm->cards[9]; i >= 0; i--, cardPtr--) {
        cardPtr->visible = 0;
    }
    i = 5;
    if (tm->handCount != i && tm->task != TM_TASK_END_FADE) {
        c0 = (TmCard *)tm_getStockCursorId(tm);
        copies = card_countOf((s32)c0);
        if (copies != 0) {
            c0 = (TmCard *)tm_getStockCursorId(tm);
            text_formatNumber((s32)c0 + 1, numText, TEXT_NUM_NO_PAD, 3);
            i = text_measureDigits(numText) + 0xE;
            w = i / 2;
            ui_drawSmallDigits(0xD8 - w, 0x78, 0xFFA, g_tmNoLabel, TEXT_PAL_WHITE);
            ui_drawDigits(0xE6 - w, 0x76, 0xFFA, numText, DIGITS_WHITE);
            shown = copies;
            k = tm->copyIdx;
            c0 = &tm->cards[TM_HAND_SIZE];
            if (shown >= 6) {
                shown = 5;
            }
            for (i = 0; i < shown; i++) {
                card = &c0[i];
                dx = 0;
                if (tm->scrollTimer != 0) {
                    if (tm->phase == 0) {
                        dx = tm->scrollTimer - 4;
                        if (i == 0) {
                            dx = 0;
                        }
                    } else {
                        dx = -tm->scrollTimer + 1;
                    }
                    if (i == 0) {
                        dx = 0;
                    }
                }
                entry = card_find(tm_getStockCursorId(tm), k);
                card->x = 0xC2 + i * 4 + dx;
                card->y = i + 0x81;
                card->depth = i + 0x28;
                card->visible = 1;
                card->small = 0;
                card->onBoard = 1;
                card->owner = TM_PLAYER;
                card->id = entry->id;
                if (i == 0) {
                    c0->arrows = entry->arrows;
                } else {
                    card->arrows = 0;
                }
                card->power = entry->power;
                card->type = entry->type;
                card->pdef = entry->physDef;
                card->mdef = entry->magicDef;
                k++;
                card->visible = 1;
                if (k == copies) {
                    k = 0;
                }
            }
            i = card_getNameWidth(tm_getStockCursorId(tm));
            text_drawBlock(0xD9 - i / 2, 0xC7, 0xFFA, (u8 *)card_getName(tm_getStockCursorId(tm)), TEXT_PAL_WHITE);
        }
    } else {
        tm->cards[5].visible = 0;
    }
}

void tm_returnKept(TmState *tm) {
    s32 i;
    TmCard *cardArr = tm->cards;
    for (i = 0; i < TM_HAND_SIZE; i++) {
        if (cardArr[i].owner == TM_PLAYER) {
            card_addWithStats(cardArr[i].id, cardArr[i].arrows, cardArr[i].power, cardArr[i].type, cardArr[i].pdef, cardArr[i].mdef);
        }
    }
}

void tm_returnHand(TmState *tm) {
    s32 i;
    TmCard *cardArr = tm->cards;
    for (i = 0; i < TM_HAND_SIZE; i++) {
        if (tm->result == TM_RESULT_WIN) {
            tm_levelUpRandom(tm, &cardArr[i]);
        }
        card_addWithStats(cardArr[i].id, cardArr[i].arrows, cardArr[i].power, cardArr[i].type, cardArr[i].pdef, cardArr[i].mdef);
    }
}

void tm_addRandomCard(tm, cardId)
    TmState *tm;
    u8 cardId;
{
    TmCard card;
    if (card_countAll() < CARD_MAX) {
        tm_makeCard(tm, &card, cardId, 1);
        card_addWithStats(cardId, card.arrows, card.power, card.type, card.pdef, card.mdef);
    }
}

s32 tm_getStockCursorId(TmState *tm) {
    return tm->stockCol * TM_STOCK_SIDE + tm->stockRow;
}

void tm_hideCards(TmState *tm) {
    s32 i;
    for (i = 9; i >= 0; i--) {
        tm->cards[i].visible = 0;
    }
}

void tm_showCards(TmState *tm) {
    s32 i;
    s32 one = 1;
    for (i = 9; i >= 0; i--) {
        tm->cards[i].visible = one;
    }
}

void tm_levelUpStat(TmState *tm, TmCard *card, s32 stat) {
    TmCard base;

    tm_makeCard(tm, &base, card->id, 0);
    if (stat == 0 && card->power != base.power) {
        card->power++;
    }
    if (stat == 1 && card->pdef != base.pdef) {
        card->pdef++;
    }
    if (stat == 2 && card->mdef != base.mdef) {
        card->mdef++;
    }
    printf("AP=%x : PD=%x : MD=%x \n", card->power, card->pdef, card->mdef);
}

void tm_levelUpRandom(TmState *tm, TmCard *card) {
    tm_levelUpStat(tm, card, rand() % 3);
}

void tm_makeCard(TmState *tm, TmCard *card, s32 cardId, s32 random) {
    TmDirMask dirs;
    TmCardData *tbl;
    u8 half;
    s32 roll;
    s32 arrowCount;
    s32 id;
    s32 i;
    s32 done;
    u8 *bp;
    s32 p0, p1, p2, p3, p4, p5, p6, p7;

    dirs = g_tmArrowBits;
    tbl = g_tmCardData;
    card->id = cardId;
    id = cardId;
    if (random == 0) {
        card->power = tbl[(u8)id].power;
        card->pdef = tbl[(u8)id].pdef;
        card->mdef = tbl[(u8)id].mdef;
    } else {
        half = tbl[(u8)id].power >> 1;
        if (half != 0) {
            card->power = half + rand() % half;
        } else {
            card->power = 0;
        }
        half = tbl[(u8)id].pdef >> 1;
        if (half != 0) {
            card->pdef = half + rand() % half;
        } else {
            card->pdef = 0;
        }
        half = tbl[(u8)id].mdef >> 1;
        if (half != 0) {
            card->mdef = half + rand() % half;
        } else {
            card->mdef = 0;
        }
    }
    roll = rand() % 100;
    p0 = tm->arrowOdds[0];
    p1 = tm->arrowOdds[1];
    p2 = tm->arrowOdds[2];
    p3 = tm->arrowOdds[3];
    p4 = tm->arrowOdds[4];
    p5 = tm->arrowOdds[5];
    p6 = tm->arrowOdds[6];
    p7 = tm->arrowOdds[7];
    if (roll < p0) {
        arrowCount = 0;
    } else if (roll < p0 + p1) {
        arrowCount = 1;
    } else if (roll < p0 + p1 + p2) {
        arrowCount = 2;
    } else if (roll < p0 + p1 + p2 + p3) {
        arrowCount = 3;
    } else if (roll < p0 + p1 + p2 + p3 + p4) {
        arrowCount = 4;
    } else if (roll < p0 + p1 + p2 + p3 + p4 + p5) {
        arrowCount = 5;
    } else if (roll < p0 + p1 + p2 + p3 + p4 + p5 + p6) {
        arrowCount = 6;
    } else if (roll < p0 + p1 + p2 + p3 + p4 + p5 + p6 + p7) {
        arrowCount = 7;
    } else {
        arrowCount = 8;
    }
    card->arrows = 0;
    for (i = 0; i < arrowCount; i++) {
        done = 0;
        while (done == 0) {
            bp = &dirs.v[rand() & 7];
            if (!(card->arrows & *bp)) {
                card->arrows |= *bp;
                done = 1;
            }
        }
    }
    card->type = tbl[(u8)id].type;
    if (!(rand() & 0x7F)) {
        card->type = TM_TYPE_X;
    }
}

void tm_taskEnemyAppear(TmState *tm) {
    s32 i;
    s32 drop;
    s32 elapsed;
    TmCard *enemyHand;

    enemyHand = &tm->cards[TM_HAND_SIZE];
    for (i = 0; i < TM_HAND_SIZE; i++) {
        elapsed = tm->timer - 0xF;
        drop = i * 6 - elapsed;
        if (drop <= 0) {
            drop = 0;
            if (tm->landed[i] == 0) {
                tm->landed[i] = 1;
                tm_playSe(TM_SE_CARD);
            }
        }
        enemyHand[i].depth = 0x3C - i;
        enemyHand[i].y = drop * drop + i * 0xE + 0x1A;
    }
    if (tm->timer >= 0x29 && tm->blocksDone != 0) {
        tm->timer = 0;
        tm->phase = 0;
        if (rand() & 1) {
            tm->turn = TM_PLAYER;
        } else {
            tm->turn = TM_ENEMY;
        }
        tm->task = TM_TASK_COIN;
        tm_playSe(TM_SE_COIN);
    }
}

void tm_animEnemyThink(TmState *tm) {
    s32 handIdx;
    s32 i;
    TmCard *enemyHand;

    handIdx = 0;
    enemyHand = &tm->cards[TM_HAND_SIZE];
    for (i = 0; i < TM_HAND_SIZE; i++) {
        if (enemyHand[i].onBoard == 0) {
            enemyHand[i].x = TM_ENEMY_HAND_X;
            if (tm->phase == 1) {
                if (handIdx == tm->timer / 4) {
                    enemyHand[i].x = 0x1A;
                }
            } else if (handIdx == 0) {
                enemyHand[i].x = 0x1A;
            }
            handIdx++;
        }
    }
    if (tm->timer != 0 && tm->phase == 1 && !(tm->timer & 3)) {
        tm_playSe(TM_SE_CARD);
    }
}

void *tm_drawCardStats(u32 *ot, SPRT *prim, TmState *tm, TmCard *card, u8 bright) {
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
    prim = tm_drawStatGlyph(ot, prim, (s32)tm, x + 9, y + 0x28, card->power >> 4, bright);
    switch (card->type) {
    case TM_TYPE_P:
        prim = tm_drawStatGlyph(ot, prim, (s32)tm, x + 15, y + 0x28, 0x10, bright);
        break;
    case TM_TYPE_M:
        prim = tm_drawStatGlyph(ot, prim, (s32)tm, x + 15, y + 0x28, 0x11, bright);
        break;
    case TM_TYPE_X:
        prim = tm_drawStatGlyph(ot, prim, (s32)tm, x + 15, y + 0x28, 0x12, bright);
        break;
    case TM_TYPE_A:
        prim = tm_drawStatGlyph(ot, prim, (s32)tm, x + 15, y + 0x28, 0xA, bright);
        break;
    }
    prim = tm_drawStatGlyph(ot, prim, (s32)tm, x + 0x15, y + 0x28, card->pdef >> 4, bright);
    prim = tm_drawStatGlyph(ot, prim, (s32)tm, x + 0x1B, y + 0x28, card->mdef >> 4, bright);
    setDrawTPage((DR_TPAGE *)prim, 0, 0, GetTPage(1, 0, 0x300, 0x100));
    AddPrim(ot, prim);
    return (DR_TPAGE *)prim + 1;
}

SPRT *tm_drawStatGlyph(void *ot, SPRT *prim, s32 unused, s32 x, s32 y, s32 glyph, s32 bright) {
    setSprt(prim);
    prim->v0 = 0xC0;
    prim->clut = getClut(0, 0xEB);
    prim->x0 = x;
    prim->w = 8;
    prim->h = 8;
    setRGB0(prim, bright, bright, bright);
    prim->y0 = y;
    prim->u0 = glyph * 8 + 0x50;
    AddPrim(ot, prim);
    return prim + 1;
}

void tm_checkGameEnd(TmState *tm) {
    s32 playerCount;
    s32 enemyCount;

    if (tm_countInHand(tm, TM_PLAYER) == 0 && tm_countInHand(tm, TM_ENEMY) == 0) {
        playerCount = tm_countOnBoard(tm, TM_PLAYER);
        enemyCount = tm_countOnBoard(tm, TM_ENEMY);
        if (playerCount > enemyCount) {
            card_setGameResult(0);
            card_addWin();
            tm->result = TM_RESULT_WIN;
            tm->wins++;
        }
        if (playerCount < enemyCount) {
            card_setGameResult(1);
            card_addLoss();
            tm->result = TM_RESULT_LOSE;
            tm->losses++;
        }
        if (playerCount == enemyCount) {
            card_setGameResult(2);
            card_addDraw();
            tm->result = TM_RESULT_DRAW;
            tm->draws++;
        }
        tm->timer = 0;
        tm->task = TM_TASK_RESULT;
    }
}

void *tm_drawResult(u32 *ot, SPRT *prim, TmState *tm) {
    u8 bright;
    s32 snd;

    bright = tm->timer * 8;
    if (tm->timer * 8 >= 0x100) {
        bright = 0xFF;
    }
    if ((u8)(bright - 0xF5) < 8) {
        switch (tm->result) {
        case 0:
            if (tm_countOnBoard(tm, TM_PLAYER) != TM_NUM_CARDS) {
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
    switch (tm->result) {
    case 0:
        if (tm_countOnBoard(tm, TM_PLAYER) == TM_NUM_CARDS) {
            setlen(prim, PRIM_LEN(SPRT));
            prim->code = GPU_SPRT | GPU_ABE;
            prim->x0 = 0x4A;
            prim->y0 = 0x58;
            prim->w = 0xA8;
            prim->h = 0x28;
            prim->r0 = bright;
            prim->g0 = bright;
            prim->b0 = bright;
            prim->u0 = 0;
            prim->v0 = 0x50;
            prim->clut = getClut(0, 0xE9);
            AddPrim(ot + 1, prim);
            prim++;
            setDrawTPage((DR_TPAGE *)prim, 1, 0, GetTPage(1, 1, 0x280, 0x100));
            AddPrim(ot + 1, prim);
            prim = (SPRT *)((DR_TPAGE *)prim + 1);
            setlen(prim, PRIM_LEN(SPRT));
            prim->code = GPU_SPRT | GPU_ABE;
            prim->x0 = 0x4A;
            prim->y0 = 0x58;
            prim->w = 0xB0;
            prim->h = 0x28;
            prim->r0 = bright;
            prim->g0 = bright;
            prim->b0 = bright;
            prim->u0 = 0;
            prim->v0 = 0x78;
            prim->clut = getClut(0, 0xE9);
            AddPrim(ot + 1, prim);
            prim++;
            setDrawTPage((DR_TPAGE *)prim, 1, 0, GetTPage(1, 2, 0x280, 0x100));
            AddPrim(ot + 1, prim);
            prim = (SPRT *)((DR_TPAGE *)prim + 1);
        } else {
            setlen(prim, PRIM_LEN(SPRT));
            prim->code = GPU_SPRT | GPU_ABE;
            prim->x0 = 0x6C;
            prim->y0 = 0x58;
            prim->w = 0x68;
            prim->h = 0x28;
            prim->u0 = 0;
            prim->v0 = 0;
            prim->r0 = bright;
            prim->g0 = bright;
            prim->b0 = bright;
            prim->clut = getClut(0, 0xE9);
            AddPrim(ot + 1, prim);
            prim++;
            setDrawTPage((DR_TPAGE *)prim, 1, 0, GetTPage(1, 1, 0x280, 0x100));
            AddPrim(ot + 1, prim);
            prim = (SPRT *)((DR_TPAGE *)prim + 1);
            setlen(prim, PRIM_LEN(SPRT));
            prim->code = GPU_SPRT | GPU_ABE;
            prim->x0 = 0x6C;
            prim->y0 = 0x58;
            prim->w = 0x70;
            prim->h = 0x28;
            prim->r0 = bright;
            prim->g0 = bright;
            prim->b0 = bright;
            prim->u0 = 0x68;
            prim->v0 = 0;
            prim->clut = getClut(0, 0xE9);
            AddPrim(ot + 1, prim);
            prim++;
            setDrawTPage((DR_TPAGE *)prim, 1, 0, GetTPage(1, 2, 0x280, 0x100));
            AddPrim(ot + 1, prim);
            prim = (SPRT *)((DR_TPAGE *)prim + 1);
        }
        break;
    case 1:
        setlen(prim, PRIM_LEN(SPRT));
        prim->code = GPU_SPRT | GPU_ABE;
        prim->x0 = 0x6A;
        prim->y0 = 0x58;
        prim->w = 0x68;
        prim->h = 0x28;
        prim->r0 = bright;
        prim->g0 = bright;
        prim->b0 = bright;
        prim->u0 = 0;
        prim->v0 = 0x28;
        prim->clut = getClut(0, 0xE9);
        AddPrim(ot + 1, prim);
        prim++;
        setDrawTPage((DR_TPAGE *)prim, 1, 0, GetTPage(1, 1, 0x280, 0x100));
        AddPrim(ot + 1, prim);
        prim = (SPRT *)((DR_TPAGE *)prim + 1);
        setlen(prim, PRIM_LEN(SPRT));
        prim->code = GPU_SPRT | GPU_ABE;
        prim->x0 = 0x6A;
        prim->y0 = 0x58;
        prim->w = 0x70;
        prim->h = 0x28;
        prim->r0 = bright;
        prim->g0 = bright;
        prim->b0 = bright;
        prim->u0 = 0x68;
        prim->v0 = 0x28;
        prim->clut = getClut(0, 0xE9);
        AddPrim(ot + 1, prim);
        prim++;
        setDrawTPage((DR_TPAGE *)prim, 1, 0, GetTPage(1, 2, 0x280, 0x100));
        AddPrim(ot + 1, prim);
        prim = (SPRT *)((DR_TPAGE *)prim + 1);
        break;
    default:
        setlen(prim, PRIM_LEN(SPRT));
        prim->code = GPU_SPRT | GPU_ABE;
        prim->x0 = 0x5C;
        prim->y0 = 0x58;
        prim->w = 0x88;
        prim->h = 0x28;
        prim->r0 = bright;
        prim->g0 = bright;
        prim->b0 = bright;
        prim->u0 = 0;
        prim->v0 = 0xA0;
        prim->clut = getClut(0, 0xE9);
        AddPrim(ot + 1, prim);
        prim++;
        setDrawTPage((DR_TPAGE *)prim, 1, 0, GetTPage(1, 1, 0x280, 0x100));
        AddPrim(ot + 1, prim);
        prim = (SPRT *)((DR_TPAGE *)prim + 1);
        setlen(prim, PRIM_LEN(SPRT));
        prim->code = GPU_SPRT | GPU_ABE;
        prim->x0 = 0x5C;
        prim->y0 = 0x58;
        prim->w = 0x90;
        prim->h = 0x28;
        prim->r0 = bright;
        prim->g0 = bright;
        prim->b0 = bright;
        prim->u0 = 0;
        prim->v0 = 0xC8;
        prim->clut = getClut(0, 0xE9);
        AddPrim(ot + 1, prim);
        prim++;
        setDrawTPage((DR_TPAGE *)prim, 1, 0, GetTPage(1, 2, 0x280, 0x100));
        AddPrim(ot + 1, prim);
        prim = (SPRT *)((DR_TPAGE *)prim + 1);
        break;
    }
    return prim;
}

void tm_playSe(s32 se) {
    tm_soundCmd(SND_SE_PLAY, g_tmSeIds[se], 0, SND_PAN_CENTER, SND_VOL_MAX);
}

void tm_stopSe(s32 se) {
    tm_soundCmd(SND_SE_STOP, g_tmSeIds[se], 0, 0, 0);
}

void tm_revealBlocks(TmState *tm) {
    if (tm->blocksDone == 0) {
        if (tm->numBlocks != 0) {
            tm->blocks[tm->blockIdx].hidden = 0;
            if (tm->blockTimer >= 9) {
                tm_playSe(TM_SE_BLOCK);
                tm->blockTimer = 0;
                if (++tm->blockIdx == tm->numBlocks) {
                    tm->blocksDone = 1;
                }
            }
        } else {
            tm->blocksDone = 1;
        }
    }
    tm->blockTimer++;
}

void *tm_drawBlocks(u32 *ot, void *prim, TmState *tm) {
    TmBlock *blockArr;
    s32 i;
    s32 zoom;
    s32 dx;
    s32 dy;
    s32 dw;
    s32 dh;
    u8 bright;
    u8 u;

    bright = tm->brightness;
    if (tm->task == TM_TASK_FIELD_FADE) {
        bright = tm->timer * 8;
    }
    blockArr = tm->blocks;
    for (i = 0; i < TM_MAX_BLOCKS; i++) {
        if (blockArr[i].hidden != 0) {
            continue;
        }
        if (tm->task == TM_TASK_ENEMY_APPEAR && tm->blocksDone == 0 && i == tm->blockIdx) {
            zoom = 10 - tm->blockTimer;
            if (zoom < 0) {
                zoom = 0;
            }
            dx = zoom * 4 / 3;
            dy = zoom * 5 / 3;
            dw = dx + 0x2A;
            dh = dy + 0x33;
            SetPolyFT4((POLY_FT4 *)prim);
            ((POLY_FT4 *)prim)->x0 = blockArr[i].x - dx;
            ((POLY_FT4 *)prim)->y0 = blockArr[i].y - dy;
            ((POLY_FT4 *)prim)->x1 = blockArr[i].x + dw;
            ((POLY_FT4 *)prim)->y1 = blockArr[i].y - dy;
            ((POLY_FT4 *)prim)->x2 = blockArr[i].x - dx;
            ((POLY_FT4 *)prim)->y2 = blockArr[i].y + dh;
            ((POLY_FT4 *)prim)->x3 = blockArr[i].x + dw;
            ((POLY_FT4 *)prim)->y3 = blockArr[i].y + dh;
            u = 0x98;
            if (blockArr[i].style == 0) {
                u = 0x68;
            }
            ((POLY_FT4 *)prim)->u0 = u;
            ((POLY_FT4 *)prim)->v0 = 0x40;
            ((POLY_FT4 *)prim)->u1 = u + 0x2A;
            ((POLY_FT4 *)prim)->v1 = 0x40;
            ((POLY_FT4 *)prim)->u2 = u;
            ((POLY_FT4 *)prim)->v2 = 0x73;
            ((POLY_FT4 *)prim)->u3 = u + 0x2A;
            ((POLY_FT4 *)prim)->v3 = 0x73;
            ((POLY_FT4 *)prim)->r0 = bright;
            ((POLY_FT4 *)prim)->g0 = bright;
            ((POLY_FT4 *)prim)->b0 = bright;
            ((POLY_FT4 *)prim)->tpage = getTPage(1, 0, 0x300, 0x100);
            ((POLY_FT4 *)prim)->clut = getClut(0, 0xEA);
            AddPrim(ot + 99, prim);
            prim = (POLY_FT4 *)prim + 1;
        } else {
            setSprt((SPRT *)prim);
            if (tm->task == TM_TASK_FIELD_FADE) {
                ((SPRT *)prim)->code = GPU_SPRT | GPU_ABE;
            }
            ((SPRT *)prim)->x0 = blockArr[i].x;
            ((SPRT *)prim)->y0 = blockArr[i].y;
            ((SPRT *)prim)->r0 = bright;
            ((SPRT *)prim)->g0 = bright;
            ((SPRT *)prim)->b0 = bright;
            if (blockArr[i].style == 0) {
                ((SPRT *)prim)->u0 = 0x68;
                ((SPRT *)prim)->v0 = 0x40;
            } else {
                ((SPRT *)prim)->u0 = 0x98;
                ((SPRT *)prim)->v0 = 0x40;
            }
            ((SPRT *)prim)->clut = getClut(0, 0xEA);
            ((SPRT *)prim)->w = 0x2A;
            ((SPRT *)prim)->h = 0x33;
            AddPrim(ot + 100, prim);
            prim = (SPRT *)prim + 1;
            setDrawTPage((DR_TPAGE *)prim, 0, 0, GetTPage(1, 1, 0x300, 0x100));
            AddPrim(ot + 100, prim);
            prim = (DR_TPAGE *)prim + 1;
        }
    }
    return prim;
}

void tm_placeBlocks(TmState *tm) {
    s32 i;
    u32 roll;
    s32 count;
    s32 x;
    s32 y;
    TmCell *cell;
    TmBlock *base;

    base = tm->blocks;
    for (i = 0; i < TM_MAX_BLOCKS; i++) {
        base[i].hidden = 1;
        base[i].x = 0;
    }
    roll = rand() % 100;
    if (roll < 3) {
        count = 0;
    } else if (roll >= 3 && roll < 7) {
        count = 1;
    } else if (roll >= 7 && roll < 12) {
        count = 2;
    } else if (roll >= 12 && roll < 17) {
        count = 3;
    } else if (roll >= 17 && roll < 27) {
        count = 4;
    } else if (roll >= 27 && roll < 97) {
        count = 5;
    } else {
        count = 6;
    }
    tm->numBlocks = count;
    tm->blockIdx = 0;
    for (i = 0; i < count; i++) {
        do {
            x = rand() & (TM_BOARD_SIZE - 1);
            y = rand() & (TM_BOARD_SIZE - 1);
            cell = tm_getCell(tm, x, y);
        } while (cell->state != TM_CELL_EMPTY);
        cell->state = TM_CELL_BLOCK;
        base[i].x = x * TM_SQUARE_W + TM_BOARD_X;
        base[i].hidden = 1;
        base[i].y = y * TM_SQUARE_H + TM_BOARD_Y;
        base[i].style = rand() & 1;
    }
}

void *tm_taskEnemyThink(u32 *ot, void *prim, TmState *tm) {
    if (tm_countInHand(tm, TM_ENEMY) == 0) {
        tm->task = TM_TASK_ENEMY_SELECT;
    } else if (tm->phase == 0) {
        if (tm->timer > tm->thinkDelay) {
            tm->timer = 0;
            tm->phase = 1;
            tm->thinkDelay = (rand() % (tm_countInHand(tm, TM_ENEMY) + 1)) * 4 - 2;
        }
    } else if (tm->timer > tm->thinkDelay) {
        tm->task = TM_TASK_ENEMY_SELECT;
    }
    return prim;
}

void tm_enemySelect(TmState *tm) {
    s32 score[80];
    TmBattle battle;
    s32 bc;
    s32 by;
    s32 bx;
    s32 wgt;
    s32 mul;
    s32 best;
    s32 cnt;
    s32 k;
    s32 handIdx;
    s32 x;
    s32 y;
    s32 eval;
    s32 take;
    s32 winPct;
    TmCard *card;
    TmCell *cell;
    TmCard *sel;

    if (tm_countInHand(tm, TM_ENEMY) == 0) {
        return;
    }
    if (tm_countOwned(tm, TM_ENEMY) == tm_countOwned(tm, TM_PLAYER)) {
        wgt = 0x40;
        mul = 1;
    } else if (tm_countOwned(tm, TM_ENEMY) >= tm_countOwned(tm, TM_PLAYER)) {
        wgt = 0x43;
        mul = 1;
    } else {
        wgt = 1;
        mul = 5;
    }
    if (tm->aiLevel == TM_AI_WEAK && rand() % 100 >= 0x1D) {
        wgt = 0;
        mul = 0;
    }
    if (tm->aiLevel == TM_AI_FAIR && rand() % 100 >= 0x3F) {
        wgt = 0;
        mul = 0;
    }
    if (tm->aiLevel == TM_AI_STRONG && rand() % 100 >= 0x5D) {
        wgt = 0;
        mul = 0;
    }
    best = -0x7FFF;
    cnt = 0;
    k = 0;
    for (handIdx = 0; handIdx < tm_countInHand(tm, TM_ENEMY); handIdx++) {
        for (x = 0; x < TM_BOARD_SIZE; x++) {
            for (y = 0; y < TM_BOARD_SIZE; y++) {
                score[k] = 0;
                if (tm_getCell(tm, x, y)->state != TM_CELL_EMPTY) {
                    score[k] = -0x7FFF;
                } else {
                    card = tm_getHandCard(tm, TM_ENEMY, handIdx);
                    eval = tm_evalPlacement(tm, 1, x, y, card);
                    if (eval > 0) {
                        score[k] += eval * wgt;
                    }
                    if (eval < 0) {
                        tm_getBattleValues((s32)tm, &battle, card, tm_findDefender(tm, x, y, card));
                        winPct = battle.atk * 100 / (battle.atk + battle.def);
                        if (tm->aiLevel == TM_AI_EXPERT) {
                            mul = 1;
                            if (winPct < 0x3C) {
                                winPct = winPct - 100;
                            } else {
                                winPct = 0x91 - winPct;
                            }
                        }
                        score[k] += winPct * mul;
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
    for (handIdx = 0; handIdx < tm_countInHand(tm, TM_ENEMY); handIdx++) {
        for (x = 0; x < TM_BOARD_SIZE; x++) {
            for (y = 0; y < TM_BOARD_SIZE; y++) {
                take = best < score[k];
                if (best == score[k] && rand() % 1000 < 1000 / cnt) {
                    take++;
                }
                if (take) {
                    bx = x;
                    bc = handIdx;
                    by = y;
                    best = score[k];
                }
                k++;
            }
        }
    }
    sel = tm_getHandCard(tm, TM_ENEMY, bc);
    cell = tm_getCell(tm, bx, by);
    cell->state = TM_CELL_CARD;
    cell->card = sel;
    sel->x = bx * TM_SQUARE_W + TM_BOARD_X;
    sel->y = by * TM_SQUARE_H + TM_BOARD_Y;
    sel->hidden = 0;
    sel->onBoard = 1;
    tm->srcX = bx;
    tm->srcY = by;
    tm_playSe(TM_SE_CARD);
    tm_checkBattles(tm);
}

s32 tm_setCellOwner(TmState *tm, s32 owner, s32 x, s32 y) {
    TmCell *cell = tm_getCell(tm, x, y);
    if (cell->state == TM_CELL_CARD) {
        TmCard *card = cell->card;
        if (card->owner != owner) {
            card->owner = owner;
        }
    }
    return 0;
}

s32 tm_evalPlacement(TmState *tm, s32 owner, s32 x, s32 y, TmCard *card) {
    TmPos pos;
    TmDirMask backBits = g_tmBackArrowBits;
    TmCell *cell;
    TmCard *target;
    s32 i;
    s32 win;
    s32 lose;
    u8 bits;

    win = 0;
    lose = 0;
    if (card == (TmCard *)-1) {
        card = tm_getCardAt(tm, x, y);
    }
    bits = card->arrows;
    for (i = 0; i < TM_NUM_DIRS; i++, bits >>= 1) {
        if (bits & 1) {
            pos.x = x;
            pos.y = y;
            tm_stepDir(&pos, i);
            cell = tm_getCell(tm, pos.x, pos.y);
            if (cell->state == TM_CELL_CARD) {
                target = cell->card;
                if (target->owner != owner) {
                    if (target->arrows & backBits.v[i]) {
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

TmCard *tm_findDefender(TmState *tm, s32 x, s32 y, TmCard *card) {
    TmPos pos;
    TmDirMask backBits = g_tmBackArrowBits;
    TmCell *cell;
    TmCard *target;
    s32 i;
    u8 bits;

    bits = card->arrows;
    for (i = 0; i < TM_NUM_DIRS; i++, bits >>= 1) {
        if (bits & 1) {
            pos.x = x;
            pos.y = y;
            tm_stepDir(&pos, i);
            cell = tm_getCell(tm, pos.x, pos.y);
            if (cell->state == TM_CELL_CARD) {
                target = cell->card;
                if (target->owner != TM_ENEMY && (target->arrows & backBits.v[i])) {
                    return target;
                }
            }
        }
    }
    return (TmCard *)-1;
}

s32 tm_setFlips(TmState *tm, s32 owner, s32 x, s32 y, s32 nextTask, s32 mode) {
    TmPos pos;
    TmCell *cell;
    TmCard *card;
    TmFlip *flipArr;
    s32 i;
    u8 bits;
    u8 bit;

    tm_clearFlips(tm);
    cell = tm_getCell(tm, x, y);
    bit = 1;
    i = 0;

    card = cell->card;
    tm->comboCount = 1;
    tm->comboCard = card;
    bits = card->arrows;
    flipArr = tm->flips;
    tm->comboDirs = 0;
    do {
        if (bits & 1) {
            pos.x = x;
            pos.y = y;
            tm_stepDir(&pos, i);
            cell = tm_getCell(tm, pos.x, pos.y);
            if (cell->state == TM_CELL_CARD && (card = cell->card)->owner != owner) {
                tm->comboDirs |= bit;
                if (mode == TM_FLIP_COMBO) {
                    flipArr[i].arg = i;
                    tm->comboX = x * 43 + 0x39;
                    tm->comboY = y * 52 + 0x19;
                    tm->comboCount++;
                }
                flipArr[i].mode = mode;
                flipArr[i].x = pos.x;
                flipArr[i].y = pos.y;
            }
        }
        bits >>= 1;
        bit <<= 1;
        i++;
    } while (i < TM_NUM_DIRS);
    tm->flipOwner = owner;
    tm->flashNext = nextTask;
    tm->flashMode = mode;
    if (mode != TM_FLIP_COMBO) {
        tm->task = TM_TASK_FLASH;
        tm_playSe(TM_SE_FLASH);
    } else {
        tm->timer = 0;
        tm->task = TM_TASK_FLASH_ARROW;
    }
    return 0;
}

TmCard *tm_getDefender(TmState *tm, TmPos *pos, s32 dir, s32 owner) {
    TmDirMask backBits = g_tmBackArrowBits;
    TmCell *cell;
    TmCard *card;

    tm_stepDir(pos, dir);
    cell = tm_getCell(tm, pos->x, pos->y);
    if (cell->state == TM_CELL_CARD) {
        card = cell->card;
        if (card->owner != owner && (card->arrows & backBits.v[dir])) {
            return card;
        }
    }
    return NULL;
}

void tm_stepDir(TmPos *pos, s32 dir) {
    if (dir == TM_DIR_UP) {
        pos->y--;
    } else if (dir == TM_DIR_UP_RIGHT) {
        pos->x++;
        pos->y--;
    } else if (dir == TM_DIR_RIGHT) {
        pos->x++;
    } else if (dir == TM_DIR_DOWN_RIGHT) {
        pos->x++;
        pos->y++;
    } else if (dir == TM_DIR_DOWN) {
        pos->y++;
    } else if (dir == TM_DIR_DOWN_LEFT) {
        pos->x--;
        pos->y++;
    } else if (dir == TM_DIR_LEFT) {
        pos->x--;
    } else if (dir == TM_DIR_UP_LEFT) {
        pos->x--;
        pos->y--;
    }
}

void *tm_drawArrows(u32 *ot, SPRT_16 *prim, TmState *tm, s32 x, s32 y, u8 arrowMask, s32 flip, u8 bright, s32 blink) {
    TmArrowTbl tbl;
    s32 i;
    u8 bits;
    u8 col;

    tbl = g_tmArrowSprites;
    bits = tm->comboDirs;
    for (i = 0; i < TM_NUM_DIRS; i++) {
        if (arrowMask & 1) {
            setSprt8(prim);
            if (flip == 0) {
                prim->x0 = tbl.v[i].x + x;
                prim->y0 = tbl.v[i].y + y;
            } else {
                prim->x0 = tbl.v[i].smallX + x;
                prim->y0 = tbl.v[i].smallY + y;
            }
            col = bright;
            if (blink != 0 && (bits & 1)) {
                col = 0;
                if (!(tm->timer & 2)) {
                    col = 0xFF;
                }
            }
            prim->r0 = col;
            prim->g0 = col;
            prim->b0 = col;
            prim->u0 = tbl.v[i].u;
            prim->v0 = tbl.v[i].v;
            prim->clut = getClut(0, 0xEA);
            AddPrim(ot, prim);
            prim++;
            setDrawTPage((DR_TPAGE *)prim, 0, 0, GetTPage(1, 0, 0x300, 0x100));
            AddPrim(ot, prim);
            prim = (SPRT_16 *)((DR_TPAGE *)prim + 1);
        }
        arrowMask >>= 1;
        bits >>= 1;
    }
    return prim;
}

void *tm_drawCursor(u32 *ot, void *prim, TmState *tm, s32 x, s32 y, s32 mode) {
    s32 z;
    u8 bright;
    s32 u;
    u8 u2;
    u32 *otSlot;

    if (mode != 2) {
        z = 3;
        if (mode != 0) {
            z = 6;
        }
        bright = tm->brightness;
        setSprt((SPRT *)prim);
        if (mode != 0) {
            ((SPRT *)prim)->code = GPU_SPRT | GPU_ABE;
        }
        ((SPRT *)prim)->x0 = x;
        ((SPRT *)prim)->y0 = y;
        ((SPRT *)prim)->r0 = bright;
        ((SPRT *)prim)->g0 = bright;
        ((SPRT *)prim)->b0 = bright;
        u = ((tm->animFrame / 6) & 7) * 24;
        if (mode != 0) {
            u = ((tm->savedFrame / 6) & 7) * 24;
        }
        otSlot = ot + z;
        ((SPRT *)prim)->u0 = u;
        ((SPRT *)prim)->v0 = 0x80;
        ((SPRT *)prim)->clut = getClut(0, 0xEB);
        ((SPRT *)prim)->w = 0x18;
        ((SPRT *)prim)->h = 0x10;
        AddPrim(otSlot, prim);
        prim = (SPRT *)prim + 1;
        ((SPRT *)prim)->code = GPU_SPRT | GPU_ABE;
        setlen((SPRT *)prim, PRIM_LEN(SPRT));
        ((SPRT *)prim)->x0 = x + 4;
        ((SPRT *)prim)->y0 = y + 4;
        ((SPRT *)prim)->r0 = 0;
        ((SPRT *)prim)->g0 = 0;
        ((SPRT *)prim)->b0 = 0;
        ((SPRT *)prim)->u0 = u;
        ((SPRT *)prim)->v0 = 0x80;
        ((SPRT *)prim)->clut = getClut(0, 0xEB);
        ((SPRT *)prim)->w = 0x18;
        ((SPRT *)prim)->h = 0x10;
        AddPrim(otSlot, prim);
        prim = (SPRT *)prim + 1;
        setDrawTPage((DR_TPAGE *)prim, 0, 0, GetTPage(1, 0, 0x300, 0x100));
        AddPrim(otSlot, prim);
        prim = (DR_TPAGE *)prim + 1;
    } else {
        u = ((tm->animFrame / 6) & 7) * 24;
        SetPolyFT4((POLY_FT4 *)prim);
        setXY4((POLY_FT4 *)prim, x, y, x + 0x18, y, x, y + 0x10, x + 0x18, y + 0x10);
        u2 = u + 0x17;
        setUV4((POLY_FT4 *)prim, u2, 0x80, u, 0x80, u2, 0x90, u, 0x90);
        ((POLY_FT4 *)prim)->clut = getClut(0, 0xEB);
        setRGB0((POLY_FT4 *)prim, 0x80, 0x80, 0x80);
        ((POLY_FT4 *)prim)->tpage = getTPage(1, 0, 0x300, 0x100);
        AddPrim(ot + 3, prim);
        prim = (POLY_FT4 *)prim + 1;
        SetPolyFT4((POLY_FT4 *)prim);
        setXY4((POLY_FT4 *)prim, x + 4, y + 4, x + 0x1C, y + 4, x + 4, y + 0x14, x + 0x1C, y + 0x14);
        setUV4((POLY_FT4 *)prim, u2, 0x80, u, 0x80, u2, 0x90, u, 0x90);
        ((POLY_FT4 *)prim)->clut = getClut(0, 0xEB);
        setRGB0((POLY_FT4 *)prim, 0, 0, 0);
        ((POLY_FT4 *)prim)->tpage = getTPage(1, 0, 0x300, 0x100);
        setSemiTrans((POLY_FT4 *)prim, 1);
        AddPrim(ot + 3, prim);
        prim = (POLY_FT4 *)prim + 1;
    }
    return prim;
}

TmCell *tm_getCell(TmState *tm, s32 x, s32 y) {
    TmCell(*board)[TM_BOARD_SIZE] = tm->board;
    if (x < 0) {
        return &g_tmOffBoardCell;
    }
    if (x >= TM_BOARD_SIZE) {
        return &g_tmOffBoardCell;
    }
    if (y < 0) {
        return &g_tmOffBoardCell;
    }
    if (y >= TM_BOARD_SIZE) {
        return &g_tmOffBoardCell;
    }
    return &board[y][x];
}

void tm_initBoard(TmState *tm) {
    s32 x, y;
    TmCell(*grid)[TM_BOARD_SIZE] = tm->board;
    g_tmOffBoardCell.state = TM_CELL_OFFBOARD;
    for (y = 0; y < TM_BOARD_SIZE; y++) {
        for (x = TM_BOARD_SIZE - 1; x >= 0; x--) {
            grid[y][x].state = TM_CELL_EMPTY;
        }
    }
    tm_placeBlocks(tm);
}

INCLUDE_RODATA("asm/us/ovl_0d/nonmatchings/tm_game", g_tmArrowBits);

INCLUDE_RODATA("asm/us/ovl_0d/nonmatchings/tm_game", g_tmBackArrowBits);

INCLUDE_RODATA("asm/us/ovl_0d/nonmatchings/tm_game", g_tmArrowSprites);

void *tm_drawStock(u32 *ot, SPRT *prim, TmState *tm) {
    u8 numText[0x20];
    s32 i;
    s32 copies;
    s32 rest;
    s32 k;
    s32 digit;

    SetSprt(prim);
    if ((u32)(tm->task - TM_TASK_CHOICE) < 2 || tm->task == TM_TASK_GAME_QUIT || tm->task == TM_TASK_END_FADE) {
        SetSprt(prim);
        prim->w = 0x30;
        prim->h = 0x18;
        prim->u0 = 0xB8;
        prim->v0 = 0x78;
        prim->x0 = 0x58;
        prim->y0 = 0xA;
        prim->r0 = 0x80;
        prim->g0 = 0x80;
        prim->b0 = 0x80;
        prim->clut = getClut(0, 0xE9);
        AddPrim(ot + 0xFFA, prim);
        prim++;
        SetSprt(prim);
        prim->w = 0x61;
        prim->h = 0x20;
        prim->u0 = 0x94;
        prim->v0 = 0xD2;
        prim->x0 = 0x8C;
        prim->y0 = 0xC;
        prim->r0 = 0x80;
        prim->g0 = 0x80;
        prim->b0 = 0x80;
        prim->clut = getClut(0, 0xE9);
        AddPrim(ot + 0xFFA, prim);
    } else {
        SetSprt(prim);
        prim->w = 0x4E;
        prim->h = 0x15;
        prim->u0 = 0x98;
        prim->v0 = 0xBC;
        prim->x0 = 0x79;
        prim->y0 = 0xE;
        prim->r0 = 0x80;
        prim->g0 = 0x80;
        prim->b0 = 0x80;
        prim->clut = getClut(0, 0xE9);
        AddPrim(ot + 0xFFA, prim);
        prim++;
        SetSprt(prim);
        prim->w = 0x10;
        prim->h = 0x10;
        prim->u0 = 0xF0;
        prim->v0 = 0xB8;
        prim->x0 = 0xCA;
        prim->y0 = 0x12;
        if (!(tm->timer & 0x20)) {
            prim->r0 = 0x80;
            prim->g0 = 0x80;
            prim->b0 = 0x80;
        } else {
            prim->r0 = 0x40;
            prim->g0 = 0x40;
            prim->b0 = 0x40;
        }
        prim->clut = getClut(0, 0xE9);
        AddPrim(ot + 0xFFA, prim);
        prim++;
        SetSprt(prim);
        prim->w = 0x10;
        prim->h = 0x10;
        prim->u0 = 0xF0;
        prim->v0 = 0xB8;
        prim->x0 = 0x66;
        prim->y0 = 0x12;
        if (!(tm->timer & 0x20)) {
            prim->r0 = 0x80;
            prim->g0 = 0x80;
            prim->b0 = 0x80;
        } else {
            prim->r0 = 0x40;
            prim->g0 = 0x40;
            prim->b0 = 0x40;
        }
        prim->clut = getClut(0, 0xE9);
        AddPrim(ot + 0xFFA, prim);
    }
    prim++;
    setDrawTPage((DR_TPAGE *)prim, 0, 0, GetTPage(1, 0, 0x280, 0x100));
    AddPrim(ot + 0xFFA, prim);
    prim = (SPRT *)((DR_TPAGE *)prim + 1);

    for (i = 0; i < CARD_KINDS; i++) {
        rest = copies = card_countOf(i);
        if (copies != 1) {
            digit = rest % 10;
            if (digit != 0 || copies >= 10) {
                SetSprt(prim);
                prim->r0 = 0x80;
                prim->g0 = 0x80;
                prim->b0 = 0x80;
                prim->clut = getClut(0, 0xED);
                prim->w = 4;
                prim->h = 5;
                prim->u0 = (digit % 5) * 4;
                prim->v0 = (digit / 5) * 8 - 0x10;
                prim->x0 = (i / 10) * 15 + 0x1B;
                prim->y0 = (i % 10) * 15 + 0x35;
                AddPrim(ot + 11, prim);
                prim++;
            }
            rest /= 10;
            digit = rest / 10;
            if (rest != digit * 10 || copies >= 100) {
                digit = rest;
                digit %= 10;
                SetSprt(prim);
                prim->w = 4;
                prim->h = 5;
                prim->r0 = 0x80;
                prim->g0 = 0x80;
                prim->b0 = 0x80;
                prim->clut = getClut(0, 0xED);
                prim->u0 = (digit % 5) * 4;
                prim->v0 = (digit / 5) * 8 - 0x10;
                prim->x0 = (i / 10) * 15 + 0x17;
                prim->y0 = (i % 10) * 15 + 0x35;
                AddPrim(ot + 11, prim);
                prim++;
                if (copies >= 100) {
                    SetSprt(prim);
                    prim->w = 4;
                    prim->h = 5;
                    prim->u0 = 4;
                    prim->v0 = 0xF0;
                    prim->x0 = (i / 10) * 15 + 0x13;
                    prim->y0 = (i % 10) * 15 + 0x35;
                    prim->r0 = 0x80;
                    prim->g0 = 0x80;
                    prim->b0 = 0x80;
                    prim->clut = getClut(0, 0xED);
                    AddPrim(ot + 11, prim);
                    prim++;
                }
            }
        }
        SetSprt16((SPRT_16 *)prim);
        ((SPRT_16 *)prim)->x0 = (i / 10) * 15 + 0x12;
        ((SPRT_16 *)prim)->y0 = (i % 10) * 15 + 0x2D;
        if (copies == 0) {
            ((SPRT_16 *)prim)->u0 = 0;
            ((SPRT_16 *)prim)->v0 = 0xE0;
        } else {
            k = card_getRarity(i);
            if (copies >= 2) {
                k += 7;
            }
            ((SPRT_16 *)prim)->u0 = k * 16 + 0x10;
            ((SPRT_16 *)prim)->v0 = 0xE0;
        }
        do {
            ((SPRT_16 *)prim)->r0 = 0x80;
            ((SPRT_16 *)prim)->g0 = 0x80;
            ((SPRT_16 *)prim)->b0 = 0x80;
            ((SPRT_16 *)prim)->clut = getClut(0, 0xED);
            AddPrim(ot + 11, prim);
        } while (0);
        prim = (SPRT *)((SPRT_16 *)prim + 1);
    }

    setDrawTPage((DR_TPAGE *)prim, 0, 0, GetTPage(0, 0, 0x340, 0x1E0));
    AddPrim(ot + 11, prim);
    prim = (SPRT *)((DR_TPAGE *)prim + 1);
    SetSprt(prim);
    prim->w = 0xA8;
    prim->h = 8;
    prim->u0 = 0x58;
    prim->v0 = 0xF0;
    prim->x0 = 9;
    prim->y0 = 0x29;
    prim->r0 = 0x80;
    prim->g0 = 0x80;
    prim->b0 = 0x80;
    prim->clut = getClut(0, 0xEE);
    AddPrim(ot + 11, prim);
    prim++;
    SetSprt(prim);
    prim->w = 0xA8;
    prim->h = 8;
    prim->u0 = 0x58;
    prim->v0 = 0xF8;
    prim->x0 = 9;
    prim->y0 = 0xBE;
    prim->r0 = 0x80;
    prim->g0 = 0x80;
    prim->b0 = 0x80;
    prim->clut = getClut(0, 0xEE);
    AddPrim(ot + 11, prim);
    prim++;
    SetSprt(prim);
    prim->w = 8;
    prim->h = 0x98;
    prim->u0 = 0xF0;
    prim->v0 = 0x58;
    prim->x0 = 0xD;
    prim->y0 = 0x2B;
    prim->r0 = 0x80;
    prim->g0 = 0x80;
    prim->b0 = 0x80;
    prim->clut = getClut(0, 0xEE);
    AddPrim(ot + 11, prim);
    prim++;
    SetSprt(prim);
    prim->w = 8;
    prim->h = 0x98;
    prim->u0 = 0xF8;
    prim->v0 = 0x58;
    prim->x0 = 0xA5;
    prim->y0 = 0x2B;
    prim->r0 = 0x80;
    prim->g0 = 0x80;
    prim->b0 = 0x80;
    prim->clut = getClut(0, 0xEE);
    AddPrim(ot + 11, prim);
    prim++;
    setDrawTPage((DR_TPAGE *)prim, 0, 0, GetTPage(0, 0, 0x300, 0));
    AddPrim(ot + 11, prim);
    prim = (SPRT *)((DR_TPAGE *)prim + 1);
    setTile((TILE *)prim);
    ((TILE *)prim)->x0 = 0x12;
    ((TILE *)prim)->y0 = 0x2D;
    ((TILE *)prim)->w = 0x95;
    ((TILE *)prim)->h = 0x95;
    ((TILE *)prim)->r0 = 0x20;
    ((TILE *)prim)->g0 = 0x18;
    ((TILE *)prim)->b0 = 0x18;
    AddPrim(ot + 11, prim);
    prim = (SPRT *)((TILE *)prim + 1);
    setDrawTPage((DR_TPAGE *)prim, 0, 0, GetTPage(0, 2, 0, 0));
    AddPrim(ot + 11, prim);
    prim = (SPRT *)((DR_TPAGE *)prim + 1);

    ui_drawSmallDigits(0x19, 0xCB, 0xFFA, "Stock:", TEXT_PAL_GREEN);
    text_formatNumber(g_tmStockCount, numText, 3, 3);
    ui_drawDigits(0x43, 0xC9, 0xFFA, numText, DIGITS_WHITE);
    ui_drawSmallDigits(0x65, 0xCB, 0xFFA, "Type:", TEXT_PAL_GREEN);
    text_formatNumber(g_tmStockTypes, numText, 3, 3);
    ui_drawDigits(0x89, 0xC9, 0xFFA, numText, DIGITS_WHITE);

    if ((u32)(tm->task - TM_TASK_CHOICE) < 2 || tm->task == TM_TASK_GAME_QUIT || tm->task == TM_TASK_END_FADE) {

        ui_drawSmallDigits(0xBC, 0x42, 0xFFA, "WIN", TEXT_PAL_GREEN);
        text_formatNumber(card_getWins(), numText, 4, TEXT_NUM_ASCII);
        ui_drawSmallDigits(0xDC, 0x42, 0xFFA, numText, TEXT_PAL_WHITE);
        ui_drawSmallDigits(0xBC, 0x4C, 0xFFA, "LOSE", TEXT_PAL_GREEN);
        text_formatNumber(card_getLosses(), numText, 4, TEXT_NUM_ASCII);
        ui_drawSmallDigits(0xDC, 0x4C, 0xFFA, numText, TEXT_PAL_WHITE);
        ui_drawSmallDigits(0xBC, 0x56, 0xFFA, "DRAW", TEXT_PAL_GREEN);
        text_formatNumber(card_getDraws(), numText, 4, TEXT_NUM_ASCII);
        ui_drawSmallDigits(0xDC, 0x56, 0xFFA, numText, TEXT_PAL_WHITE);

        setSprt8(prim);
        ((SPRT_8 *)prim)->x0 = 0xB5;
        ((SPRT_8 *)prim)->y0 = 0x3D;
        ((SPRT_8 *)prim)->r0 = 0x80;
        ((SPRT_8 *)prim)->g0 = 0x80;
        ((SPRT_8 *)prim)->b0 = 0x80;
        ((SPRT_8 *)prim)->u0 = 0x38;
        ((SPRT_8 *)prim)->v0 = 0xF0;
        ((SPRT_8 *)prim)->clut = getClut(0, 0xEE);
        AddPrim(ot + 0xFFA, prim);
        prim = (SPRT *)((SPRT_8 *)prim + 1);
        setSprt8(prim);
        ((SPRT_8 *)prim)->x0 = 0xF6;
        ((SPRT_8 *)prim)->y0 = 0x3D;
        ((SPRT_8 *)prim)->r0 = 0x80;
        ((SPRT_8 *)prim)->g0 = 0x80;
        ((SPRT_8 *)prim)->b0 = 0x80;
        ((SPRT_8 *)prim)->u0 = 0x48;
        ((SPRT_8 *)prim)->v0 = 0xF0;
        ((SPRT_8 *)prim)->clut = getClut(0, 0xEE);
        AddPrim(ot + 0xFFA, prim);
        prim = (SPRT *)((SPRT_8 *)prim + 1);
        setSprt8(prim);
        ((SPRT_8 *)prim)->x0 = 0xB5;
        ((SPRT_8 *)prim)->y0 = 0x5A;
        ((SPRT_8 *)prim)->r0 = 0x80;
        ((SPRT_8 *)prim)->g0 = 0x80;
        ((SPRT_8 *)prim)->b0 = 0x80;
        ((SPRT_8 *)prim)->u0 = 0x38;
        ((SPRT_8 *)prim)->v0 = 0xF8;
        ((SPRT_8 *)prim)->clut = getClut(0, 0xEE);
        AddPrim(ot + 0xFFA, prim);
        prim = (SPRT *)((SPRT_8 *)prim + 1);
        setSprt8(prim);
        ((SPRT_8 *)prim)->x0 = 0xF6;
        ((SPRT_8 *)prim)->y0 = 0x5A;
        ((SPRT_8 *)prim)->r0 = 0x80;
        ((SPRT_8 *)prim)->g0 = 0x80;
        ((SPRT_8 *)prim)->b0 = 0x80;
        ((SPRT_8 *)prim)->u0 = 0x48;
        ((SPRT_8 *)prim)->v0 = 0xF8;
        ((SPRT_8 *)prim)->clut = getClut(0, 0xEE);
        AddPrim(ot + 0xFFA, prim);
        prim = (SPRT *)((SPRT_8 *)prim + 1);
        for (i = 0; i < 8; i++) {
            setSprt8(prim);
            ((SPRT_8 *)prim)->y0 = 0x3D;
            ((SPRT_8 *)prim)->x0 = i * 8 + 0xB9;
            ((SPRT_8 *)prim)->r0 = 0x80;
            ((SPRT_8 *)prim)->g0 = 0x80;
            ((SPRT_8 *)prim)->b0 = 0x80;
            ((SPRT_8 *)prim)->u0 = 0x40;
            ((SPRT_8 *)prim)->v0 = 0xF0;
            ((SPRT_8 *)prim)->clut = getClut(0, 0xEE);
            AddPrim(ot + 0xFFA, prim);
            prim = (SPRT *)((SPRT_8 *)prim + 1);
            setSprt8(prim);
            ((SPRT_8 *)prim)->y0 = 0x5A;
            ((SPRT_8 *)prim)->x0 = i * 8 + 0xB9;
            ((SPRT_8 *)prim)->r0 = 0x80;
            ((SPRT_8 *)prim)->g0 = 0x80;
            ((SPRT_8 *)prim)->b0 = 0x80;
            ((SPRT_8 *)prim)->u0 = 0x40;
            ((SPRT_8 *)prim)->v0 = 0xF8;
            ((SPRT_8 *)prim)->clut = getClut(0, 0xEE);
            AddPrim(ot + 0xFFA, prim);
            prim = (SPRT *)((SPRT_8 *)prim + 1);
        }
        for (i = 0; i < 3; i++) {
            setSprt8(prim);
            ((SPRT_8 *)prim)->x0 = 0xB5;
            ((SPRT_8 *)prim)->y0 = i * 8 + 0x43;
            ((SPRT_8 *)prim)->r0 = 0x80;
            ((SPRT_8 *)prim)->g0 = 0x80;
            ((SPRT_8 *)prim)->b0 = 0x80;
            ((SPRT_8 *)prim)->u0 = 0x50;
            ((SPRT_8 *)prim)->v0 = 0xF8;
            ((SPRT_8 *)prim)->clut = getClut(0, 0xEE);
            AddPrim(ot + 0xFFA, prim);
            prim = (SPRT *)((SPRT_8 *)prim + 1);
            setSprt8(prim);
            ((SPRT_8 *)prim)->x0 = 0xF6;
            ((SPRT_8 *)prim)->y0 = i * 8 + 0x43;
            ((SPRT_8 *)prim)->r0 = 0x80;
            ((SPRT_8 *)prim)->g0 = 0x80;
            ((SPRT_8 *)prim)->b0 = 0x80;
            ((SPRT_8 *)prim)->u0 = 0x50;
            ((SPRT_8 *)prim)->v0 = 0xF0;
            ((SPRT_8 *)prim)->clut = getClut(0, 0xEE);
            AddPrim(ot + 0xFFA, prim);
            prim = (SPRT *)((SPRT_8 *)prim + 1);
        }
        setTile((TILE *)prim);
        ((TILE *)prim)->x0 = 0xB8;
        ((TILE *)prim)->y0 = 0x41;
        ((TILE *)prim)->w = 0x46;
        ((TILE *)prim)->h = 0x1E;
        ((TILE *)prim)->r0 = 0x20;
        ((TILE *)prim)->g0 = 0x18;
        ((TILE *)prim)->b0 = 0x18;
        AddPrim(ot + 0xFFA, prim);
        prim = (SPRT *)((TILE *)prim + 1);
        setDrawTPage((DR_TPAGE *)prim, 0, 0, GetTPage(0, 0, 0x300, 0));
        AddPrim(ot + 0xFFA, prim);
        prim = (SPRT *)((DR_TPAGE *)prim + 1);
    }
    if (tm->handCount != TM_HAND_SIZE && tm->task != TM_TASK_AWAY_INFO && card_countOf(tm_getStockCursorId(tm)) >= 2) {
        text_formatNumber(tm->copyIdx + 1, numText, 3, TEXT_NUM_ASCII);
        ui_drawSmallDigits(0xBF, 0xBA, 0xFFA, numText, TEXT_PAL_WHITE);
        ui_drawSmallDigits(0xD4, 0xBA, 0xFFA, g_tmSlash, TEXT_PAL_WHITE);
        text_formatNumber(card_countOf(tm_getStockCursorId(tm)), numText, TEXT_NUM_NO_PAD, TEXT_NUM_ASCII);
        ui_drawSmallDigits(0xDB, 0xBA, 0xFFA, numText, TEXT_PAL_WHITE);
        text_drawBlock(0xB5, 0xB6, 0xFFA, text_get(TM_TEXT_ICON_L1), TEXT_PAL_WHITE);
        text_drawBlock(0xF1, 0xB6, 0xFFA, text_get(TM_TEXT_ICON_R1), TEXT_PAL_WHITE);
    }
    if (tm->handCount == 0) {
        g_tmStockTypes = card_countKinds();
        g_tmStockCount = card_countAll();
    }
    return prim;
}

void tm_openStock(TmState *tm) {
    g_tmStockTypes = card_countKinds();
    g_tmStockCount = card_countAll();
    tm->task = TM_TASK_CHOICE;
}

void tm_drawSelectTitle(Window *win) {
    text_drawBlock(0x8A, 0x10, win->otz, text_get(TM_TEXT_SELECT_CARDS), TEXT_PAL_WHITE);
}

void tm_resetStockCursor(TmState *tm) {
    tm->stockCol = 0;
    tm->stockRow = 0;
    tm->handCount = 0;
    tm->copyIdx = 0;
}

void *tm_drawCardFace(u32 *ot, void *prim, TmState *tm, TmCard *card, u8 bright, s32 flip) {
    u8 id;
    s32 page;
    s32 slot;
    s32 u;
    s32 v;

    id = card->id;
    page = (u8)(id / 30);
    slot = (u8)(id % 30);
    u = (slot % 6) * 42;
    v = (slot / 6) * 51;
    if (flip == 0) {
        setSprt((SPRT *)prim);
        ((SPRT *)prim)->x0 = card->x;
        ((SPRT *)prim)->y0 = card->y;
        ((SPRT *)prim)->w = 0x2A;
        ((SPRT *)prim)->h = 0x33;
        ((SPRT *)prim)->r0 = bright;
        ((SPRT *)prim)->g0 = bright;
        ((SPRT *)prim)->b0 = bright;
        ((SPRT *)prim)->u0 = u;
        ((SPRT *)prim)->v0 = v;
        ((SPRT *)prim)->clut = getClut(0x180, id + 0x19C);
        AddPrim(ot, prim);
        prim = (SPRT *)prim + 1;
        setDrawTPage((DR_TPAGE *)prim, 1, 0, GetTPage(1, 0, page << 7, 0x100));
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
        ((POLY_FT4 *)prim)->r0 = bright;
        ((POLY_FT4 *)prim)->g0 = bright;
        ((POLY_FT4 *)prim)->b0 = bright;
        ((POLY_FT4 *)prim)->clut = getClut(0x180, id + 0x19C);
        ((POLY_FT4 *)prim)->tpage = ((page * 2) & 0xE) | TPAGE_8BIT | TPAGE_Y_BIT; /* x = page * 128 */
        AddPrim(ot, prim);
        prim = (POLY_FT4 *)prim + 1;
    }
    return prim;
}

void *tm_taskCoin(u32 *ot, SPRT *prim, TmState *tm) {
    TmCoinFrames frames = g_tmCoinFrames;
    s32 dx;
    s32 dy;
    s32 flip;
    s32 tick;
    s32 frame;
    s32 flyFrame;
    u8 bright;
    s32 k;
    TmCoinFrame *fr;

    dx = 0;
    dy = 0;
    tick = tm->timer;
    flip = 0;
    if (tm->phase == 0) {
        frame = tick & 7;
        if (tick >= 0x29 && ((tm->turn == TM_PLAYER && frame == 7) || (tm->turn == TM_ENEMY && frame == 3))) {
            tm->phase = 1;
            tm->timer = 0;
            tm_stopSe(TM_SE_COIN);
        }
    } else {
        frame = 3;
        if (tm->turn == TM_PLAYER) {
            frame = 7;
        }
        if (tick >= 0x1F) {
            flyFrame = tick - 0x1E;
            flip = 1;
            dy = -(flyFrame * 82) / 12;
            dx = (flyFrame * 124) / 12;
            if (tm->turn == TM_ENEMY) {
                dy = -(flyFrame * 66) / 12;
                dx = (flyFrame * -119) / 12;
            }
        }
        if (tick >= 0x2A) {
            if (tm->turn == TM_PLAYER) {
                tm->nameSlide = 0;
                tm->task = TM_TASK_PLAYER_SELECT;
            } else {
                tm->timer = 0;
                tm->phase = 0;
                tm->thinkDelay = (rand() & 0xF) + 0xF;
                tm->task = TM_TASK_ENEMY_THINK;
            }
        }
    }
    SetSprt(prim);
    if (flip != 0) {
        prim->code |= 2;
    }
    fr = &frames.v[frame];
    prim->w = fr->w;
    prim->h = 0x28;
    prim->u0 = fr->u;
    prim->v0 = fr->v;
    k = dx + 0x9D;
    prim->x0 = fr->x + k;
    prim->y0 = dy + 0x5C;
    if (flip == 0) {
        bright = 0x80;
        prim->r0 = bright;
        prim->g0 = bright;
        prim->b0 = bright;
    } else {
        bright = (0x2A - tick) * 10;
        prim->r0 = bright;
        prim->g0 = bright;
        prim->b0 = bright;
    }
    prim->clut = getClut(0, 0xEB);
    AddPrim(ot + 10, prim);
    prim++;
    setDrawTPage((DR_TPAGE *)prim, 0, 0, GetTPage(1, 1, 0x300, 0x100));
    AddPrim(ot + 10, prim);
    return (DR_TPAGE *)prim + 1;
}

void *tm_drawCombo(u32 *ot, SPRT *prim, TmState *tm) {
    u8 bright;

    if (tm->comboTimer != 0) {
        bright = 0x80;
        if (tm->comboShown >= 0x19) {
            bright = tm->comboTimer * 4 - 0x80;
        }
        SetSprt(prim);
        prim->w = 0x38;
        prim->h = 0x10;
        prim->u0 = 0xA0;
        prim->v0 = 0xF0;
        prim->x0 = tm->comboX + 0x10;
        prim->y0 = tm->comboY;
        prim->r0 = bright;
        prim->g0 = bright;
        prim->b0 = bright;
        prim->clut = getClut(0, 0xE9);
        AddPrim(ot + 10, prim);
        prim++;
        SetSprt(prim);
        prim->w = 0x38;
        prim->h = 0x10;
        prim->u0 = 0xA0;
        prim->v0 = 0xF0;
        prim->x0 = tm->comboX + 0x12;
        prim->y0 = tm->comboY + 2;
        prim->r0 = 0;
        prim->g0 = 0;
        prim->b0 = 0;
        prim->clut = getClut(0, 0xE9);
        AddPrim(ot + 10, prim);
        prim++;
        SetSprt(prim);
        prim->w = 0x10;
        prim->h = 0x10;
        prim->u0 = tm->comboShown * 16;
        prim->v0 = 0xF0;
        prim->x0 = tm->comboX;
        prim->y0 = tm->comboY;
        prim->r0 = bright;
        prim->g0 = bright;
        prim->b0 = bright;
        prim->clut = getClut(0, 0xE9);
        AddPrim(ot + 10, prim);
        prim++;
        SetSprt(prim);
        prim->w = 0x38;
        prim->h = 0x10;
        prim->u0 = tm->comboShown * 16;
        prim->v0 = 0xF0;
        prim->x0 = tm->comboX + 2;
        prim->y0 = tm->comboY + 2;
        prim->r0 = 0;
        prim->g0 = 0;
        prim->b0 = 0;
        prim->clut = getClut(0, 0xE9);
        AddPrim(ot + 10, prim);
        prim++;
        setDrawTPage((DR_TPAGE *)prim, 0, 0, GetTPage(1, 0, 0x280, 0x100));
        AddPrim(ot + 10, prim);
        prim = (SPRT *)((DR_TPAGE *)prim + 1);
        tm->comboTimer--;
    }
    return prim;
}

void *tm_drawBattleValues(u32 *ot, void *vp, TmState *tm) {
    SPRT *prim = vp;
    TmCard *atkCard;
    TmCard *defCard;
    s32 i;
    s32 value;
    s32 x;
    s32 y;

    if (tm->valuesTimer != 0) {
        atkCard = tm_getCardAt(tm, tm->srcX, tm->srcY);
        defCard = tm_getCardAt(tm, tm->dstX, tm->dstY);
        for (i = 0; i < 2; i++) {
            if (i == 0) {
                value = tm->atkShown;
                x = atkCard->x + 8;
                y = atkCard->y + 0x12;
            } else {
                value = tm->defShown;
                x = defCard->x + 8;
                y = defCard->y + 0x12;
            }
            if (value < 10) {
                x += 9;
            } else if (value < 100) {
                x += 4;
            }
            if (value >= 100) {
                prim = tm_drawBigDigit(ot, prim, x, y, value / 100);
                value %= 100;
                x += 9;
                prim = tm_drawBigDigit(ot, prim, x, y, value / 10);
                x += 9;
                value %= 10;
            } else if (value >= 10) {
                prim = tm_drawBigDigit(ot, prim, x, y, value / 10);
                x += 9;
                value %= 10;
            }
            prim = tm_drawBigDigit(ot, prim, x, y, value);
        }
        setDrawTPage((DR_TPAGE *)prim, 0, 0, GetTPage(0, 0, 0x340, 0x1E0));
        AddPrim(ot + 11, prim);
        prim = (SPRT *)((DR_TPAGE *)prim + 1);
        tm->valuesTimer--;
    }
    return prim;
}

void *tm_drawBigDigit(u32 *ot, SPRT *prim, s32 x, s32 y, s32 digit) {
    s32 u;

    SetSprt(prim);
    prim->w = 10;
    prim->h = 16;
    u = digit * 10 + 0x18;
    prim->v0 = 0xF0;
    prim->x0 = x;
    prim->y0 = y;
    prim->r0 = 0x80;
    prim->g0 = 0x80;
    prim->b0 = 0x80;
    prim->u0 = u;
    prim->clut = getClut(0, 0xED);
    AddPrim(ot + 11, prim);
    return prim + 1;
}

void *tm_drawTryAgain(u32 *ot, void *prim, TmState *tm) {
    return prim;
}

void tm_askRematch(TmState *tm) {
    RECT rect;

    if ((u32)(tm->opponent - TM_OPP_FORCED) >= TM_OPP_FORCED_NUM) {
        rect.w = text_measureWidth((s32)text_get(TM_TEXT_REMATCH)) + 0x12;
        rect.x = 0xA0 - rect.w / 2;
        rect.y = 0x54;
        rect.h = 0x39;
        tm->msgWin = win_openFree(&rect, 5, WIN_STYLE_BEVEL);
        win_setDrawContent(tm->msgWin, tm_drawRematchWin);
        win_setShadow(tm->msgWin, 1);
        tm_waitWindow(tm->msgWin);
        tm->cursor = 0;
        tm->task = TM_TASK_TRY_AGAIN;
        return;
    }
    tm_startEndFade(tm);
}

void tm_drawRematchWin(Window *win) {
    text_drawBlock(win->x + 9, win->y + 6, win->otz, text_get(TM_TEXT_REMATCH), TEXT_PAL_WHITE);
}

void tm_taskEndFade(TmState *tm) {
    fade_setAngle((tm->timer * 0x88) & 0xFF8);
    if (tm->timer >= 0x1F) {
        tm->task = TM_TASK_EXIT;
    }
}

void tm_startEndFade(TmState *tm) {
    tm->timer = 0;
    game_startFade(FADE_WIPE, FADE_SUB, 0x1E, FADE_COLOR_FULL);
    fade_setAngle(0);
    tm_soundCmd(SND_SONG_VOL_FROM_INTPL, MUSIC_TETRA_MASTER, 0x1E, 0x7F, 0);
    tm->task = TM_TASK_END_FADE;
}

INCLUDE_RODATA("asm/us/ovl_0d/nonmatchings/tm_game", g_tmSlash);

INCLUDE_RODATA("asm/us/ovl_0d/nonmatchings/tm_game", g_tmCoinFrames);
