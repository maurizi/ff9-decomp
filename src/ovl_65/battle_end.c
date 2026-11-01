#include "ovl_65.h"

void battleEnd_main(void) {
    Unk11C0 *work;
    GameData *ctx;
    u8 i;

    work = g_gameState->gameData->ctx.ovl65->work;
    battleEnd_init();
    do {
        ClearOTagR(g_gameState->gameData->ot[g_gameState->gameData->drawBuf], OT_LEN);
        pad_update();
        switch (battleEnd_update()) {
        case 1:
            g_gameState->mode = work->returnMode;
            break;
        case 2:
            g_gameState->mode = GS_MODE_OPENING;
            break;
        }
        game_updateWindows();
        game_updateFade();
        game_updateTimer();
        preload_update();
        DrawSync(0);
        VSync(g_gameState->vsyncMode);
        ResetGraph(1);
        PutDispEnv(&g_gameState->dispEnv[g_gameState->gameData->drawBuf]);
        ctx = g_gameState->gameData;
        i = ctx->drawBuf;
        DrawOTagEnv(ctx->ot[i] + OT_LAST, &g_gameState->drawEnv[i]);
        g_gameState->gameData->drawBuf ^= 1;
        gfx_setBuffer(g_gameState->gameData->drawBuf);
    } while (g_gameState->mode == GS_MODE_BATTLE_END);
    battleEnd_exit();
}

void battleEnd_init(void) {
    Unk11C0 *work = g_gameState->gameData->ctx.ovl65->work;
    FileEntry *entry;

    game_freezeScreen();
    preload_setHook(0);
    rewards_init();
    battleEnd_loadDb();
    battleEnd_loadPortraits();
    battleEnd_unloadDb();
    if (work->eventScript != 0 && g_btlRewards.hasPostEvent != 0) {
        evt_init((u8 *)work->eventScript);
        if (work->texts != 0) {
            msg_init();
        }
    }
    if (g_gameState->gameData->battleFlags & GD_BTL_NEXT_LOCATION) {
        entry = dep_find(g_gameState->gameData->depTable, g_gameState->gameData->fieldId | DB_FIELD);
        if (entry != NULL) {
            g_gameState->gameData->fieldAreaId = entry->link;
            g_gameState->gameData->sceneSector[1] = entry->sector;
            g_gameState->gameData->sceneSectors[1] = entry->sectors;
            entry = dep_find(g_gameState->gameData->depTable, entry->link);
            if (entry != NULL) {
                g_gameState->gameData->sceneSector[0] = entry->sector;
                g_gameState->gameData->sceneSectors[0] = entry->sectors;
            }
        }
    }
    battleEnd_preloadScene();
}

void battleEnd_exit(void) {
    GameData *gd = g_gameState->gameData;
    s32 i;

    if (!(gd->battleFlags & GD_BTL_KEEP_MUSIC) && (gd->flags & GD_MUSIC_PLAYING) && gd->musicId[0] != MUSIC_NONE) {
        gd->flags |= GD_MUSIC_LEFT_PLAYING;
    }
    g_gameState->gameData->depTable = (FileTable *)-1;
    if (g_gameState->mode == GS_MODE_OPENING) {
        btl_soundCmd(SND_MUSIC_VOL_INTPL, 0, 0x1E, 0, 0);
        for (i = 29; i >= 0; i--) {
            VSync(0);
        }
        gd = g_gameState->gameData;
        gd->flags &= ~GD_MUSIC_LEFT_PLAYING;
        snd_quit(gd);
        g_resetCount++;
        g_gameState->gameData->flags |= GD_SOUND_REINIT;
    }
}

s32 battleEnd_loadPortraits(void) {
    btlui_loadPortraits();
    return 0;
}

s32 battleEnd_loadDb(void) {
    FileEntry *entry;
    PreloadEntry *preloaded;

    if (preload_isReady(FILE_BTL_END_DB)) {
        preloaded = preload_find(FILE_BTL_END_DB);
        file_copyViaVram(g_modeHeap, preloaded->dest, preloaded->loaded, 0);
        preload_remove(FILE_BTL_END_DB);
        DrawSync(0);
    } else {
        entry = file_find(DIR_COMMON, FILE_BTL_END_DB);
        cdRead_init(g_gameState->gameData->cdRead, entry->sector, (entry->link - entry->sector) << 11, (void *)g_modeHeap, 0, -1, 0);
        while (cdRead_update(g_gameState->gameData->cdRead)) {
        }
    }
    g_gameState->gameData->ctx.ovl65->files->data1 = g_modeHeap;
    db_register(g_gameState->gameData->ctx.ovl65->files->data1, 0);
    return 0;
}

s32 battleEnd_unloadDb(void) {
    Unk1354 *files = g_gameState->gameData->ctx.ovl65->files;
    if (files->data1 != 0) {
        db_unregister((void *)files->data1);
        g_gameState->gameData->ctx.ovl65->files->data1 = 0;
    }
    return 0;
}

void battleEnd_preloadScene(void) {
    Unk11C0 *work;
    FileEntry *entry;
    GameData *gd;
    s32 id;
    s32 size;
    s32 at;
    u8 *dst;
    s32 sector;
    s32 k;

    work = g_gameState->gameData->ctx.ovl65->work;
    preload_init();
    g_gameState->gameData->preloadRequest = NULL;
    if (g_gameState->gameData->battleResult == BATTLE_END_GAME_OVER) {
        return;
    }
    id = OVL_KEY(OVL_WORLD);
    if (work->returnMode == GS_MODE_FIELD) {
        id = OVL_KEY(OVL_FIELD);
    }
    entry = file_find(DIR_OVERLAYS, id);
    dst = work->bgData;
    size = (entry->link - entry->sector) << 11;
    preload_setEntry(preload_add(), PRELOAD_KIND_READ, id, (s32)dst, size, size, entry->sector, size);
    dst += size;
    if (work->returnMode == GS_MODE_FIELD) {
        id = FILE_FLD_COMMON;
    } else if (g_evtGlobalVars.h[0] >= 0x2B52) {
        id = FILE_WM_DATA_LATE;
    } else {
        id = FILE_WM_DATA;
    }
    entry = file_find(DIR_COMMON, id);
    k = -0x146F00;
    size = (entry->link - entry->sector) << 11;
    at = g_loadAreaEnd - (dst + k);
    at = at / 0x800 * 0x800;
    preload_setEntry(preload_add(), PRELOAD_KIND_READ, id, (s32)dst, at, size, entry->sector, size);
    dst += size;
    if (work->returnMode != GS_MODE_FIELD) {
        return;
    }
    sector = g_gameState->gameData->sceneSector[0];
    id = g_gameState->gameData->fieldAreaId | DB_AREA;
    size = g_gameState->gameData->sceneSectors[0] << 11;
    at = g_loadAreaEnd - (dst + k);
    at = at / 0x800 * 0x800;
    preload_setEntry(preload_add(), PRELOAD_KIND_READ, id, (s32)dst, at, size, sector, size);
    dst += size;
    sector = g_gameState->gameData->sceneSector[1];
    id = g_gameState->gameData->fieldId | DB_FIELD;
    size = g_gameState->gameData->sceneSectors[1] << 11;
    at = g_loadAreaEnd - (dst + k);
    at = at / 0x800 * 0x800;
    preload_setEntry(preload_add(), PRELOAD_KIND_READ, id, (s32)dst, at, size, sector, size);
    gd = g_gameState->gameData;
    if ((s16)gd->fieldId == gd->savedMusicField && gd->savedMusicFileSector != -1) {
        sector = gd->savedMusicFileSector;
        id = gd->savedMusicId | DB_MUSIC;
        preload_setEntry(preload_add(), PRELOAD_KIND_SEEK, id, 0, 0, 0, sector, -1);
    }
}

void rewards_drawInfo(void) {
    RECT *rect = &s_rewards.wins[0].rect;
    s32 x, y;
    ui_drawLabel(rect->x + 6, rect->y - 3, 0xB, g_labelInfo, UI_LABEL_NORMAL);
    win_drawBox(rect, 0xB, WIN_STYLE_LINE_RULE);
    win_drawShadow(rect, 0xC, WIN_STYLE_LINE_RULE);
    x = rect->x;
    y = rect->y;
    s_rewards.prim = text_buildBlock(x + 0x13, y + 6, g_ot + 10,
                                      text_get(s_rewardsTitleText[s_rewards.screen]), TEXT_PAL_WHITE, s_rewards.prim);
}

void rewards_drawMembers(void) {
    u8 numText[16];
    RECT *rect;
    Character *chr;
    u32 i;
    u8 dim;
    s32 col;
    u8 alive;
    u32 nextExp;
    u8 *numStr;

    for (i = 0; i < 4; i++) {
        if (s_rewards.winShown[i + 2] != 0) {
            rect = &s_rewards.wins[i + 2].rect;
            chr = g_gameState->gameData->party[i];
            if (chr != NULL) {
                alive = rewards_isRewarded(chr, i);
                dim = !alive;
                col = dim * 2;
                if (!alive) {
                    rewards_drawPanelShade(rect);
                }
                ui_drawPartyPortrait(rect->x + 4, rect->y + 4, 0xB, i, dim);
                rewards_drawLevelUp(rect->x + 0x24, rect->y + 0x12, 0xB, &s_rewards.levelPopups[i]);
                s_rewards.prim = text_buildBlock(rect->x + 0x29, rect->y + 4, g_ot + 0xB, chr->name, col, s_rewards.prim);
                text_formatNumber(chr->level, numText, 2, TEXT_NUM_ASCII);
                ui_drawSprite(rect->x + 0x72, rect->y + 7, 0xB, 0xB4, dim);
                numStr = numText;
                s_rewards.prim = text_buildDigits(rect->x + 0x84, rect->y + 5, g_ot + 0xB, numStr, col, s_rewards.prim);
                ui_drawStatusIcons(rect->x + 0x28, rect->y + 0x13, 0xB, chr->status, dim);
                text_drawBlock(rect->x + 0x29, rect->y + 0x1C, 0xB, text_get(REWARDS_TEXT_EXP), col);
                text_formatNumber(chr->exp, numStr, 7, TEXT_NUM_ASCII);
                ui_drawSmallDigits(rect->x + 0x62, rect->y + 0x20, 0xB, numStr, col);
                if (chr->level < LEVEL_MAX) {
                    nextExp = g_levelExp[chr->level];
                } else {
                    nextExp = chr->exp;
                }
                text_formatNumber(nextExp - chr->exp, numText, 6, TEXT_NUM_ASCII);
                ui_drawSmallDigits(rect->x + 0x69, rect->y + 0x2A, 0xB, numText, col);
                text_drawBlock(rect->x + 0x29, rect->y + 0x26, 0xB, text_get(REWARDS_TEXT_NEXT), col);
            }
            win_drawBox(rect, 0xB, WIN_STYLE_LINE);
            win_drawShadow(rect, 0xC, WIN_STYLE_LINE);
        }
    }
}

void rewards_drawExp(void) {
    RECT *rect = &s_rewards.wins[6].rect;
    u8 numText[16];
    s32 x, y;

    if (s_rewards.winShown[6] != 0) {
        win_drawBox(rect, 0xB, WIN_STYLE_BEVEL);
        win_drawShadow(rect, 0xC, WIN_STYLE_BEVEL);
        text_formatNumber(s_rewards.rewards.expEach, numText, 6, TEXT_NUM_ASCII);
        x = rect->x;
        y = rect->y;
        s_rewards.prim = text_buildBlock(x + 0xC, y + 5, g_ot + 10,
                                          text_get(REWARDS_TEXT_EXP), TEXT_PAL_WHITE, s_rewards.prim);
        s_rewards.prim = text_buildSmall(rect->x + 0x5A, rect->y + 9, g_ot + 10,
                                          numText, TEXT_PAL_WHITE, s_rewards.prim);
    }
}

void rewards_drawAp(void) {
    RECT *rect = &s_rewards.wins[7].rect;
    u8 numText[16];
    s32 x, y;

    if (s_rewards.winShown[7] != 0) {
        win_drawBox(rect, 0xB, WIN_STYLE_BEVEL);
        win_drawShadow(rect, 0xC, WIN_STYLE_BEVEL);
        text_formatNumber(s_rewards.rewards.ap, numText, 3, TEXT_NUM_ASCII);
        x = rect->x;
        y = rect->y;
        s_rewards.prim = text_buildBlock(x + 0xC, y + 5, g_ot + 10,
                                          text_get(REWARDS_TEXT_AP), TEXT_PAL_WHITE, s_rewards.prim);
        s_rewards.prim = text_buildSmall(rect->x + 0x6F, rect->y + 9, g_ot + 10,
                                          numText, TEXT_PAL_WHITE, s_rewards.prim);
    }
}

void rewards_drawReceivedGil(void) {
    RECT *rect = &s_rewards.wins[8].rect;
    u8 numText[16];
    s32 x, y;
    u32 gil = s_rewards.rewards.gil.target - s_rewards.rewards.gil.cur;

    if (gil > 999999) {
        gil = 999999;
    }
    if (s_rewards.winShown[8] != 0) {
        win_drawBox(rect, 0xE, WIN_STYLE_BEVEL);
        win_drawShadow(rect, 0xF, WIN_STYLE_BEVEL);
        text_formatNumber(gil, numText, 6, TEXT_NUM_ASCII);
        strcat(numText, s_rewardsGilUnit);
        x = rect->x;
        y = rect->y;
        s_rewards.prim = text_buildBlock(x + 0xC, y + 5, g_ot + 13,
                                          text_get(REWARDS_TEXT_RECEIVED_GIL), TEXT_PAL_WHITE, s_rewards.prim);
        s_rewards.prim = text_buildSmall(rect->x + 0x54, rect->y + 9, g_ot + 13,
                                          numText, TEXT_PAL_WHITE, s_rewards.prim);
    }
}

void rewards_drawCurrentGil(void) {
    RECT *rect = &s_rewards.wins[9].rect;
    u8 numText[16];
    s32 x, y;
    u32 gil = g_gameState->gameData->gil;

    if (gil > GIL_MAX) {
        gil = GIL_MAX;
    }
    if (s_rewards.winShown[9] != 0) {
        win_drawBox(rect, 0xE, WIN_STYLE_BEVEL);
        win_drawShadow(rect, 0xF, WIN_STYLE_BEVEL);
        text_formatNumber(gil, numText, 7, TEXT_NUM_ASCII);
        strcat(numText, s_rewardsGilUnit2);
        x = rect->x;
        y = rect->y;
        s_rewards.prim = text_buildBlock(x + 0xC, y + 5, g_ot + 13,
                                          text_get(REWARDS_TEXT_CURRENT_GIL), TEXT_PAL_WHITE, s_rewards.prim);
        s_rewards.prim = text_buildSmall(rect->x + 0x4D, rect->y + 9, g_ot + 13,
                                          numText, TEXT_PAL_WHITE, s_rewards.prim);
    }
}

void rewards_drawPanelShade(RECT *rect) {
    u8 *pkt = s_rewards.prim;

    setlen(pkt, 3);
    ((TileWords *)pkt)->rgbc = GPU_RGBC(GPU_TILE | GPU_ABE, 0x202020);
    ((TileWords *)pkt)->xy = ((rect->x + 2) & 0xFFFF) | ((rect->y + 2) << 16);
    ((TileWords *)pkt)->wh = (rect->w - 4) | ((rect->h - 4) << 16);
    addPrim(g_ot + 11, pkt);
    pkt += 0x10;
    SetDrawTPage(pkt, 1, 1, 0);
    addPrim(g_ot + 11, pkt);
    pkt += 8;
    s_rewards.prim = pkt;
}

void rewards_initWindows(void) {
    s32 i;

    for (i = 0; i < 10; i++) {
        s_rewards.winShown[i] = s_rewardsWinInit[i].shown;
        s_rewards.wins[i].rect = s_rewardsWinInit[i].rect;
        if (s_rewardsWinInit[i].shown != 0) {
            tween_start(&s_rewards.wins[i], s_rewardsWinOpen[i], 1);
        } else {
            tween_start(&s_rewards.wins[i], NULL, 0);
        }
    }
}

void rewards_moveMemberWins(void) {
    s32 i;
    Window *w;
    Window *w2;
    s32 dx;
    s32 dy;
    s32 tx;
    s32 ty;
    TweenDef *tween;
    WinDef *winDef;

    for (i = 0; i < 4; i++) {
        if (s_rewards.getsExp[i] != 0) {
            w = s_rewards.levelWins[i].win;
            if (w != NULL && w->state != WIN_STATE_CLOSED) {
                tx = w->x - 0x26;
                dx = s_rewards.wins[i + 2].rect.x - tx;
                ty = w->y - 2;
                dy = s_rewards.wins[i + 2].rect.y - ty;
                w->x += dx;
                s_rewards.levelWins[i].win->cur.x += dx;
                s_rewards.levelWins[i].win->y += dy;
                s_rewards.levelWins[i].win->cur.y += dy;
            }
        }
        if (s_rewards.getsAp[i] != 0) {
            w2 = s_rewards.learnWins[i].win;
            winDef = &s_rewards.wins[i + 2];
            if (w2 != NULL && w2->state != 0) {
                tween = winDef->anim;
                if (tween != NULL && winDef->frame >= 2) {
                    if (tween->dx != NULL) {
                        w2->x += tween->dx[winDef->frame];
                        s_rewards.learnWins[i].win->cur.x += tween->dx[winDef->frame];
                    }
                    if (tween->dy != NULL) {
                        s_rewards.learnWins[i].win->y += tween->dy[winDef->frame];
                        s_rewards.learnWins[i].win->cur.y += tween->dy[winDef->frame];
                    }
                }
            }
        }
    }
}

void rewards_stateGil(void) {
    s32 se;

    if (s_rewards.gilSkipped == 0) {
        rewards_stepGil();
        rewards_onStepGil();
        if (!fade_isBusy() && (g_padRepeat & BTN_CROSS)) {
            rewards_finishGil();
        }
    } else if (++s_rewards.timer >= 24) {
        se = s_rewardsStateSe[s_rewards.state];
        if (se != 0) {
            snd_playSe(se);
        }
        s_rewards.state++;
        fade_setOtz(s_rewards.savedOtz);
        if (g_menu.open != 0) {
            menu_close();
        } else {
            fade_out(0x10);
            fade_setOnDone(rewards_onClosed);
        }
    }
}

void rewards_finishExp(void) {
    Rewards *rw = &s_rewards.rewards;
    s32 i;

    for (i = 0; i < 4; i++) {
        if (s_rewards.getsExp[i] != 0) {
            rewards_addExp(i, rw->exp[i].target - rw->exp[i].cur);
            rewards_showLevelUp(i);
        }
    }
    snd_stopSe(SE_EXP_COUNT);
    snd_playSe(SE_CURSOR);
    for (i = 0; i < 7; i++) {
        tween_start(&s_rewards.wins[2 + i], &s_rewardsWinClose[i], 0);
    }
    s_rewards.screen = 1;
    s_rewards.winShown[9] = 1;
    s_rewards.winShown[8] = 1;
    s_rewards.savedOtz = fade_setOtz(0xC);
    fade_out(4);
    fade_setOnDone(rewards_onItemsFaded);
}

void rewards_onExpWindowsClosed(void) {
    s32 i;
    s32 se;

    s_rewards.winShown[7] = 0;
    s_rewards.winShown[6] = 0;
    s_rewards.winShown[5] = 0;
    s_rewards.winShown[4] = 0;
    s_rewards.winShown[3] = 0;
    s_rewards.winShown[2] = 0;
    rewards_closeMemberWins();
    for (i = 0; i < 4; i++) {
        WinSlot_clear(&s_rewards.learnWins[i]);
        WinSlot_clear(&s_rewards.levelWins[i]);
    }
    se = s_rewardsStateSe[s_rewards.state];
    if (se != 0) {
        snd_playSe(se);
    }
    s_rewards.state++;
}

void rewards_findMembers(void) {
    s32 i;
    Character *chr;

    for (i = 0; i < 4; i++) {
        s_rewards.getsAp[i] = 0;
        s_rewards.getsExp[i] = 0;
    }
    i = 0;
    s_rewards.expMembers = 0;
    for (; i < 4; i++) {
        if ((g_btlRewards.expMask >> i) & 1) {
            if (g_gameState->gameData->party[i] != NULL) {
                s_rewards.getsExp[i] = func_801F_canExp(g_gameState->gameData->party[i]);
                s_rewards.getsAp[i] = func_801F_canAp(g_gameState->gameData->party[i]);
                if (s_rewards.getsExp[i]) {
                    s_rewards.expMembers++;
                }
            }
        }
    }
}

void rewards_compute(void) {
    Rewards *rw;
    u32 j;
    u8 found;
    s32 i;
    u32 share;

    rw = &s_rewards.rewards;
    memset(rw, 0, sizeof(Rewards));
    rewards_findMembers();
    if (s_rewards.expMembers != 0) {
        share = g_btlRewards.exp / s_rewards.expMembers;
    } else {
        share = 0;
    }
    rw->expEach = share;
    rw->itemCount = 0;
    rw->ap = g_btlRewards.ap;
    rw->gil.target = g_btlRewards.gil;
    rw->card = (card_countAll() < 100) ? g_btlRewards.card : ITEM_NONE;
    for (i = 0; i < BTL_DROP_COUNT; i++) {
        found = 0;
        if (g_btlRewards.items[i] == ITEM_NONE) {
            continue;
        }
        for (j = 0; j < rw->itemCount; j++) {
            if (g_btlRewards.items[i] == rw->items[j][0]) {
                found = 1;
                rw->items[j][1]++;
                break;
            }
        }
        if (!found) {
            if (rw->itemCount >= REWARDS_ITEM_KINDS) {
                return;
            }
            rw->items[rw->itemCount][0] = g_btlRewards.items[i];
            rw->items[rw->itemCount][1] = 1;
            rw->itemCount++;
        }
    }
}

void rewards_drawSmallText(s32 x, s32 y, s32 otz, u8 *str, s32 pal) {
    s_rewards.prim = text_buildSmall(x, y, g_ot + otz, str, pal, s_rewards.prim);
}

void rewards_drawText(s32 x, s32 y, s32 otz, u8 *str, s32 mode) {
    s_rewards.prim = text_buildBlock(x, y, g_ot + otz, str, mode, s_rewards.prim);
}

void rewards_init(void) {
    game_initGfx();
    gfx_setUiMode(0);
    help_setCallback(0);
    g_gameState->vsyncMode = 2;
    s_rewards.screen = 0;
    s_rewards.state = REWARDS_STATE_OPENING;
    s_rewards.frame = 0;
    s_rewards.closeFrames = 0;
    s_rewards.active = 1;
    s_rewards.timer = 0;
    s_rewards.gilSkipped = 0;
    s_rewards.done = 0;
    switch (rewards_getKind()) {
    case 0:
        rewards_skip();
        break;
    case 1:
        rewards_open();
        break;
    case 2:
        rewards_waitEvent();
        break;
    }
}

u8 rewards_update(void) {
    if (s_rewards.running != 0) {
        if (s_rewards.closing != 0) {
            goto count;
        }
        s_rewards.prim = s_rewards.primBufs[g_drawBufs.cur];
        if (g_menu.open == 0) {
            pad_updateRepeat(0);
        }
        rewards_draw();
        rewards_updateState();
    }
    if (s_rewards.closing != 0) {
    count:
        if (++s_rewards.closeFrames >= 2) {
            s_rewards.done = 1;
        }
    }
    return s_rewards.done == 0;
}

void rewards_exit(void) {
    help_setCallback(0);
    rewards_restoreVram();
    s_rewards.closing = 1;
}

void rewards_requestClose(void) {
    s_rewards.closing = 1;
}

s32 rewards_getKind(void) {
    u8 mode = g_gameState->gameData->battleResult;
    s32 kind = 0;

    if (mode == BATTLE_END_ESCAPE) {
        kind = g_btlRewards.fleeGil != 0;
    } else if (mode != 7) {
        kind = 1;
        if (g_btlRewards.hasPostEvent != 0) {
            kind = 2;
        }
    }
    return kind;
}

void rewards_skip(void) {
    s_rewards.running = 0;
    s_rewards.closing = 1;
    s_rewards.done = 0;
}

void rewards_open(void) {
    s32 i;

    s_rewards.running = 1;
    s_rewards.closing = 0;
    rewards_compute();
    rewards_clearWinSlot(&s_rewards.fullWin);
    for (i = 0; i < 4; i++) {
        rewards_clearWinSlot(&s_rewards.levelWins[i]);
        rewards_clearWinSlot(&s_rewards.learnWins[i]);
    }
    text_setBank(g_rewardsText);
    rewards_initPrimBufs();
    rewards_initExp();
    rewards_initLearns();
    rewards_resetFullWarning();
    rewards_loadImages();
    rewards_initGil();
    rewards_initWindows();
    g_gameState->vsyncMode = 0;
    fade_inDefault();
}

void rewards_waitEvent(void) {
    s_rewards.running = 0;
    s_rewards.closing = 0;
}

void rewards_draw(void) {
    ui_drawBackground();
    rewards_drawInfo();
    rewards_drawHelp();
    rewards_drawMembers();
    rewards_drawExp();
    rewards_drawAp();
    rewards_drawReceivedGil();
    rewards_drawCurrentGil();
}

void rewards_updateState(void) {
    void (*fn)(void) = s_rewardsStates[s_rewards.state];

    if (fn != NULL) {
        fn();
    }
    rewards_stepWindows();
    rewards_moveMemberWins();
    s_rewards.frame++;
}

void rewards_drawHelp(void) {
    s32 x, y;
    RECT *rect = &s_rewards.wins[1].rect;
    ui_drawLabel(rect->x + 6, rect->y - 3, 0xB, g_labelHelp, UI_LABEL_NORMAL);
    win_drawBox(rect, 0xB, WIN_STYLE_LINE_RULE);
    win_drawShadow(rect, 0xC, WIN_STYLE_LINE_RULE);
    x = rect->x;
    y = rect->y;
    s_rewards.prim = text_buildBlock(x + 0x13, y + 6, g_ot + 10,
                                      text_get(REWARDS_TEXT_CONFIRM), TEXT_PAL_WHITE, s_rewards.prim);
}

void rewards_initPrimBufs(void) {
    s_rewards.primBufs[0] = s_rewardsPrimBufs[0];
    s_rewards.primBufs[1] = s_rewardsPrimBufs[1];
}

void rewards_clearWinSlot(WinSlot *slot) {
    slot->queued = 0;
    slot->shown = 0;
    slot->timer = 0;
    slot->delay = 0;
    slot->win = 0;
}

void rewards_closeMemberWins(void) {
    s32 i;

    for (i = 0; i < 4; i++) {
        if (s_rewards.getsExp[i] != 0 && s_rewards.levelWins[i].win != NULL) {
            win_close(s_rewards.levelWins[i].win);
        }
        if (s_rewards.getsAp[i] != 0 && s_rewards.learnWins[i].win != NULL) {
            win_close(s_rewards.learnWins[i].win);
        }
    }
}

void rewards_stepWindows(void) {
    s32 i;

    for (i = 0; i < 10; i++) {
        if (s_rewards.winShown[i] != 0 && s_rewards.wins[i].anim != 0) {
            tween_step(&s_rewards.wins[i]);
        }
    }
}

void rewards_stateReceived(void) {
    s32 se;

    if (!fade_isBusy() && (g_padRepeat & BTN_CROSS)) {
        se = s_rewardsStateSe[s_rewards.state];
        if (se != 0) {
            snd_playSe(se);
        }
        s_rewards.state++;
        rewards_giveAp();
    }
}

void rewards_stateExp(void) {
    rewards_updateExp();
    rewards_updateAp();
    if (!fade_isBusy() && (g_padRepeat & BTN_CROSS)) {
        rewards_finishExp();
    }
}

void rewards_stateItems(void) {
    if (!fade_isBusy() && (g_padRepeat & BTN_CROSS)) {
        rewards_takeItems();
    }
}

void rewards_giveAp(void) {
    Rewards *rw = &s_rewards.rewards;
    s32 i;
    u8 any = 0;

    for (i = 0; i < 4; i++) {
        if (s_rewards.getsAp[i] != 0) {
            rewards_addAp(i, rw->apGain[i].target);
        }
        if (s_rewards.getsExp[i] != 0) {
            any = 1;
        }
    }
    if (any) {
        snd_startSe(SE_EXP_COUNT);
    }
}

void rewards_takeItems(void) {
    s32 se;

    if (rewards_tryAddItems()) {
        se = s_rewardsStateSe[s_rewards.state];
        if (se != 0) {
            snd_playSe(se);
        }
        s_rewards.state++;
        if (g_btlRewards.gil != 0) {
            snd_startSe(SE_GIL_COUNT);
        }
    }
}

void rewards_finishGil(void) {
    Rewards *rw = &s_rewards.rewards;

    if (rw->gil.target != rw->gil.cur) {
        rewards_addGil(rw->gil.target - rw->gil.cur);
        rw->gil.cur = rw->gil.target;
        snd_stopSe(SE_GIL_COUNT);
        s_rewards.timer = 0;
    } else {
        s_rewards.timer = 24;
    }
    s_rewards.gilSkipped = 1;
}

void rewards_onItemsFaded(void) {
    rewards_openItemsWin();
    fade_in(0xC);
}

u8 rewards_isRewarded(Character *chr, s32 slot) {
    if (!func_801F_alive(chr)) {
        return 0;
    }
    return (g_btlRewards.expMask >> slot) & 1;
}

void rewards_onWindowsOpened(void) {
    s32 se = s_rewardsStateSe[s_rewards.state];

    if (se != 0) {
        snd_playSe(se);
    }
    s_rewards.state++;
}

void rewards_onClosed(void) {
    s_rewards.closing = 1;
    win_closeAllNow();
}

void rewards_drawDigits(s32 x, s32 y, s32 otz, u8 *str, s32 style) {
    s_rewards.prim = text_buildDigits(x, y, g_ot + otz, str, style, s_rewards.prim);
}

void rewards_updateLearnWins(void) {
    WinSlot *w;
    RECT *panel;
    s32 i;
    RECT *rr;

    w = s_rewards.learnWins;
    for (i = 0; i < 4; w++, i++) {
        if (s_rewards.getsAp[i] != 0) {
            if (w->delay == 0) {
                if (w->win == NULL) {
                    if (w->queued != 0) {
                        s_rewardsLearnPanel = panel = &s_rewards.wins[i + 2].rect;
                        s_rewardsLearnRect.x = panel->x + 0x26;
                        rr = &s_rewardsLearnRect;
                        rr->y = panel->y + 0x1B;
                        rr->w = 0x70;
                        rr->h = 0x19;
                        w->win = win_openFree(rr, 6, WIN_STYLE_BEVEL);
                        w->win->userData = i;
                        win_setTitle(w->win, (s32)s_rewardsLearnLabel);
                        win_setDrawContent(w->win, rewards_drawLearnWin);
                        win_setOnClosed(w->win, rewards_onLearnWinClosed);
                        win_setOpenAnim(w->win, WIN_ANIM_ZOOM);
                        snd_playSe(SE_LEARN);
                        w->queued--;
                    }
                } else if (++w->timer >= 0x40) {
                    win_setCloseAnim(w->win, WIN_ANIM_ZOOM);
                    win_close(w->win);
                    w->delay = 4;
                    w->timer = 0;
                    w->shown++;
                }
            } else {
                w->delay--;
            }
        }
    }
}

void rewards_addAp(s32 slot, u32 ap) {
    Character *chr;
    s32 i;
    s32 j;
    ItemInfo *item;
    s32 k;
    s32 need;
    s32 have;

    chr = g_gameState->gameData->party[slot];
    if (ap != 0 && chr_canLearnAbilities(chr)) {
        s_rewards.rewards.apGain[slot].cur += ap;
        for (i = 0; i < 5; i++) {
            if (chr->equip[i] != ITEM_NONE) {
                item = &g_itemTable[chr->equip[i]];
                for (j = 0; j < 3; j++) {
                    if (item->abilities[j] != 0) {
                        k = chr_findAbilitySlot(chr->info.b.id, item->abilities[j]);
                        if (k >= 0) {
                            need = g_abilityRows[chr->info.b.row][k].ap;
                            have = chr->ap[k];
                            if (have < need) {
                                if (ap >= need - have) {
                                    chr->ap[k] = need;
                                    rewards_addLearned(slot, item->abilities[j]);
                                } else {
                                    chr->ap[k] = have + ap;
                                }
                            }
                        }
                    }
                }
            }
        }
    }
}

u8 rewards_willLearn(Character *chr, s32 ap) {
    u8 learns;
    s32 i;
    s32 j;
    s32 ch;
    s32 k;
    u8 *eq;
    ItemInfo *item;
    s32 have;

    learns = 0;
    eq = chr->equip;
    ch = chr->info.b.id;
    memset(s_rewardsAbilityItems, 0, 0x30);
    for (i = 0; i < 5; i++) {
        if (eq[i] != ITEM_NONE) {
            item = &g_itemTable[eq[i]];
            for (j = 0; j < 3; j++) {
                if (item->abilities[j] != 0) {
                    k = chr_findAbilitySlot(ch & 0xFF, item->abilities[j]);
                    if (k >= 0) {
                        have = chr->ap[k];
                        if (have < g_abilityRows[chr->info.b.row][k].ap) {
                            s_rewardsAbilityItems[k]++;
                        }
                    }
                }
            }
        }
    }
    for (i = 0; i < 0x30; i++) {
        if (s_rewardsAbilityItems[i] != 0) {
            have = chr->ap[i];
            if (s_rewardsAbilityItems[i] * ap >= g_abilityRows[chr->info.b.row][i].ap - have) {
                learns = 1;
                break;
            }
        }
    }
    return learns;
}

void rewards_drawLearnWin(Window *w) {
    Window *win;
    WinSlot *ws;
    TileWords *tile;
    u8 *name;
    s32 slot;
    s32 icon;
    s32 id;

    win = w;
    slot = w->userData;
    ws = &s_rewards.learnWins[slot];
    id = ws->ids[ws->shown];
    if (id < ABILITY_SUPPORT_FIRST) {
        name = s_rewardsActionNames + s_rewardsActionTable[id].nameOffset;
        ui_drawSprite(w->x + 6, w->y + 7, w->otz, 0xBC, 0);
    } else {
        name = s_rewardsSupportNames + g_supportAbilities[id - ABILITY_SUPPORT_FIRST].name;
        if (chr_testSupportBit(g_gameState->gameData->party[slot]->supportAbilities, id)) {
            icon = 0xBD;
        } else {
            icon = 0xF4;
        }
        ui_drawSprite(w->x + 6, w->y + 7, w->otz, icon, 0);
    }
    text_drawBlock(win->x + 0x16, win->y + 6, w->otz, name, TEXT_PAL_WHITE);
    setlen(g_primPtr, 3);
    tile = (TileWords *)g_primPtr;
    tile->rgbc = GPU_RGBC(GPU_TILE | GPU_ABE, 0x101010);
    tile->xy = (win->x + 4) | ((win->y + 4) << 16);
    tile->wh = (win->w - 4) | ((win->h - 4) << 16);
    addPrim(&g_ot[w->otz], tile);
    g_primPtr += sizeof(TileWords);
    SetDrawTPage(g_primPtr, 1, 1, 0x20);
    addPrim(&g_ot[w->otz], g_primPtr);
    g_primPtr += sizeof(DrTpageWords);
}

void rewards_initLearns(void) {
    s32 i;

    for (i = 0; i < 4; i++) {
        if (s_rewards.getsAp[i] != 0) {
            s_rewards.learns[i] = rewards_willLearn(g_gameState->gameData->party[i], rewards_initApCounter(i));
        } else {
            s_rewards.learns[i] = 0;
        }
    }
}

void rewards_updateAp(void) {
    rewards_stepAp();
    rewards_updateLearnWins();
}

s32 rewards_initApCounter(s32 i) {
    u32 ap = s_rewards.rewards.ap;
    Counter *counter = &s_rewards.rewards.apGain[i];

    if (chr_hasSupportAbility(g_gameState->gameData->party[i]->info.b.id, SA_ABILITY_UP)) {
        counter->target = ap * 2;
    } else {
        counter->target = ap;
    }
    counter->step = 1;
    counter->cur = 0;
    return counter->target;
}

void rewards_stepAp(void) {
    s32 i;
    Counter *counter;

    for (i = 0; i < 4; i++) {
        if (s_rewards.getsAp[i] != 0) {
            counter = &s_rewards.rewards.apGain[i];
            if (counter->cur < counter->target) {
                rewards_addAp(i, Counter_step(counter));
            }
        }
    }
}

void rewards_addLearned(s32 slot, u8 abilityId) {
    s_rewards.learnWins[slot].ids[s_rewards.learnWins[slot].queued] = abilityId;
    s_rewards.learnWins[slot].queued++;
    s_rewards.learned[slot].ids[s_rewards.learned[slot].count] = abilityId;
    s_rewards.learned[slot].count++;
}

void rewards_onLearnWinClosed(Window *win) {
    s_rewards.learnWins[win->userData].win = 0;
}

void rewards_drawLevelUp(s32 x, s32 y, s32 otz, Popup *popup) {
    SprtWords *sprt;
    u8 *pkt;
    u32 clut;
    u32 xy;
    DR_TPAGE *tpage;

    if (popup->active != 0 && popup->phase != POPUP_GAP) {
        y += popup->y;
        clut = GetClut(0, 0x1FF) << 16;
        setlen(g_primPtr, 4);
        xy = (x & 0xFFFF) | (y << 16);
        pkt = g_primPtr;
        sprt = (SprtWords *)pkt;
        sprt->rgbc = GPU_RGBC(GPU_SPRT, GPU_RGB_NEUTRAL);
        sprt->xy = xy;
        sprt->uvclut = clut | 0xB000;
        sprt->wh = 0x14004C;
        g_primPtr = (u8 *)(sprt + 1);
        addPrim(&g_ot[otz], sprt);
        tpage = (DR_TPAGE *)g_primPtr;
        SetDrawTPage(tpage, 1, 1, 0x9F);
        addPrim(&g_ot[otz], g_primPtr);
        g_primPtr += sizeof(DR_TPAGE);
        setlen(g_primPtr, 4);
        sprt = (SprtWords *)g_primPtr;
        sprt->rgbc = GPU_RGBC(GPU_SPRT | GPU_ABE, GPU_RGB_NEUTRAL);
        sprt->xy = xy;
        sprt->uvclut = clut | 0xC400;
        sprt->wh = 0x14004C;
        g_primPtr = (u8 *)(sprt + 1);
        addPrim(&g_ot[otz], sprt);
        SetDrawTPage((DR_TPAGE *)g_primPtr, 1, 1, 0xDF);
        addPrim(&g_ot[otz], g_primPtr);
        g_primPtr += sizeof(DR_TPAGE);
    }
}

void rewards_initExp(void) {
    s32 i;

    for (i = 0; i < 4; i++) {
        if (s_rewards.getsExp[i] != 0) {
            s_rewards.levelsUp[i] = rewards_willLevelUp(g_gameState->gameData->party[i], rewards_initExpCounter(i));
        } else {
            s_rewards.levelsUp[i] = 0;
        }
        rewards_initPopup(&s_rewards.levelPopups[i]);
    }
}

void rewards_updateExp(void) {
    rewards_stepExp();
    rewards_updateLevelUps();
}

void rewards_addExp(s32 i, u32 exp) {
    u32 need;
    Character *chr = g_gameState->gameData->party[i];

    if (exp != 0) {
        if (chr->exp >= EXP_MAX) {
            chr->exp = EXP_MAX;
            return;
        }
        s_rewards.rewards.exp[i].cur += exp;
        chr->exp += exp;
        if (chr->exp >= EXP_MAX) {
            chr->exp = EXP_MAX;
        }
        if (chr->level < LEVEL_MAX && (need = g_levelExp[chr->level]) <= chr->exp) {
            do {
                rewards_levelUp(i);
            } while (chr->level < LEVEL_MAX && (need = g_levelExp[chr->level]) <= chr->exp);
        }
    }
}

void rewards_showLevelUp(s32 slot) {
    Popup *popup = &s_rewards.levelPopups[slot];

    if (popup->pending != 0 || popup->active != 0) {
        popup->phase = POPUP_HOLD;
        popup->y = -0x11;
        popup->timer = 0;
        popup->enabled = 0;
        popup->active = 1;
    }
}

u32 rewards_initExpCounter(s32 slot) {
    u32 exp = s_rewards.rewards.expEach;
    Character *chr = g_gameState->gameData->party[slot];
    Counter *counter = &s_rewards.rewards.exp[slot];
    u32 step;

    counter->target = exp;
    if (chr_hasSupportAbility(chr->info.b.id, SA_LEVEL_UP)) {
        counter->target += exp >> 1;
    }
    if (exp < 16) {
        counter->step = 1;
    } else if (exp < 512) {
        counter->step = (exp + 15) >> 4;
    } else {
        step = 32;
        counter->step = step;
        if (exp / step > 192) {
            counter->step = (exp + 191) / 192;
        }
    }
    counter->cur = 0;
    return counter->target;
}

s32 rewards_willLevelUp(Character *chr, s32 exp) {
    if (chr->level >= LEVEL_MAX) {
        return 0;
    }
    return chr->exp + exp >= g_levelExp[chr->level];
}

void rewards_stepExp(void) {
    s32 i;
    Counter *counter;
    s32 step;
    u8 done = 1;

    for (i = 0; i < 4; i++) {
        if (s_rewards.getsExp[i] != 0) {
            counter = &s_rewards.rewards.exp[i];
            if (counter->cur < counter->target) {
            step = Counter_step(counter);
            rewards_addExp(i, step);
            done = 0;
            }
        }
    }
    if (done) {
        snd_stopSe(SE_EXP_COUNT);
    }
}

void rewards_updateLevelUps(void) {
    Popup *popup;
    s32 i;

    for (popup = s_rewards.levelPopups, i = 0; i < 4; popup++, i++) {
        if (s_rewards.getsExp[i] != 0) {
            if (popup->active == 0) {
                if (popup->pending != 0) {
                    rewards_startPopup(popup);
                }
            } else if (popup->enabled != 0 && rewards_stepPopup(popup) == POPUP_DONE) {
                rewards_endPopup(popup);
            }
        }
    }
}

void rewards_levelUp(s32 slot) {
    Character *chr = g_gameState->gameData->party[slot];

    chr_levelUp(chr->info.b.id, chr->level + 1);
    s_rewards.levelWins[slot].queued++;
    s_rewards.levelPopups[slot].pending++;
}

void rewards_initPopup(Popup *popup) {
    popup->active = 0;
    popup->pending = 0;
    popup->y = 0;
    popup->timer = 0;
    popup->phase = 0;
    popup->enabled = 1;
}

void rewards_startPopup(Popup *popup) {
    popup->timer = 0;
    popup->y = 0;
    popup->phase = POPUP_RISE;
    popup->active = 1;
    popup->pending--;
    snd_playSe(SE_LEVEL_UP);
}

void rewards_endPopup(Popup *popup) {
    popup->active = 0;
    popup->timer = 0;
    popup->y = 0;
    popup->count++;
}

s32 rewards_stepPopup(Popup *popup) {
    switch (popup->phase) {
    case POPUP_RISE:
        popup->y += s_rewardsPopupRise[popup->timer];
        if (++popup->timer >= 10) {
            popup->phase = POPUP_HOLD;
            popup->timer = 0;
        }
        break;
    case POPUP_HOLD:
        if (++popup->timer >= 0x40) {
            popup->timer = 0;
            if (popup->pending != 0) {
                popup->phase = POPUP_GAP;
            } else {
                popup->phase = POPUP_DONE;
            }
        }
        break;
    case POPUP_GAP:
        if (++popup->timer >= 4) {
            popup->phase = POPUP_DONE;
            popup->timer = 0;
        }
        break;
    }
    return popup->phase;
}

void rewards_initGil(void) {
    Counter *counter = &s_rewards.rewards.gil;
    s32 i;
    u8 found = 0;
    Character *chr;

    for (i = 0; i < 4; i++) {
        chr = g_gameState->gameData->party[i];
        if (chr != NULL && chr_hasSupportAbility(chr->info.b.id, SA_MILLIONAIRE)) {
            found = 1;
        }
    }
    if (found) {
        counter->target += counter->target >> 1;
    }
    counter->step = 1;
    counter->cur = 0;
}

void rewards_stepGil(void) {
    Counter *counter = &s_rewards.rewards.gil;
    s32 step;

    if (counter->cur < counter->target) {
        step = Counter_step(counter);
        rewards_addGil(step);
        if (counter->cur >= counter->target) {
            snd_stopSe(SE_GIL_COUNT);
        }
    }
}

void rewards_addGil(s32 amount) {
    u32 gil;

    if (amount != 0) {
        s_rewards.rewards.gil.cur += amount;
        gil = g_gameState->gameData->gil;
        if (gil + amount > GIL_MAX) {
            amount = GIL_MAX - gil;
            s_rewards.rewards.gil.cur = s_rewards.rewards.gil.target;
        }
        g_gameState->gameData->gil += amount;
    }
}

void rewards_loadImages(void) {
    StoreImage(&s_rewardsClutRect1, s_rewardsVramSave);
    DrawSync(0);
    LoadImage(&s_rewardsClutRect0, (u32 *)s_rewardsImage0);
    DrawSync(0);
    LoadImage(&s_rewardsImageRect0, (u32 *)(s_rewardsImage0 + 0x20));
    DrawSync(0);
    LoadImage(&s_rewardsClutRect1, (u32 *)s_rewardsImage1);
    DrawSync(0);
    LoadImage(&s_rewardsImageRect1, (u32 *)(s_rewardsImage1 + 0x200));
    DrawSync(0);
}

void rewards_restoreVram(void) {
    LoadImage(&s_rewardsVramRestoreRect, s_rewardsVramSave);
    DrawSync(0);
}

u8 rewards_addItems(void) {
    TextSize ts;
    RECT rr;
    u32 i;
    u8 full;
    u8 wasFull;
    Rewards *rw;
    WinSlot *ws;
    s32 qty;
    s32 k;

    full = 0;
    rw = &s_rewards.rewards;
    ws = &s_rewards.fullWin;
    for (i = 0; i < rw->itemCount; i++) {
        qty = rw->items[i][1];
        if (qty != item_add(rw->items[i][0], qty)) {
            full = 1;
        }
    }
    wasFull = full;
    if (rw->card != ITEM_NONE) {
        card_add(rw->card);
    }
    if (wasFull) {
        k = 0x10;
        ts = text_measure(text_get(REWARDS_TEXT_ITEMS_FULL));
        rr.x = s_rewards.itemsWin->x + ((s_rewards.itemsWin->w - k - ts.w) >> 1);
        rr.y = s_rewards.itemsWin->y + ((s_rewards.itemsWin->h - k - ts.h) >> 1);
        rr.w = ts.w + 0x10;
        rr.h = ts.h + 0xF;
        ws->win = win_openFree(&rr, 6, WIN_STYLE_BEVEL);
        win_setDrawContent(ws->win, rewards_drawFullWarning);
        win_setOnClosed(ws->win, (void (*)(Window *))rewards_onFullWarningClosed);
        win_setOpenAnim(ws->win, WIN_ANIM_ZOOM);
        win_setShadow(ws->win, 1);
        win_setTitle(ws->win, g_labelWarning);
    }
    return !wasFull;
}

void rewards_drawItems(Window *w) {
    Window *win;
    ItemInfo *it;
    u8 *msg;
    u32 i;
    s32 off;
    s32 y;
    s32 y2;
    s32 k;
    s32 k1;
    s32 k3;
    Rewards *rw;

    win = w;
    i = 0;
    rw = &s_rewards.rewards;
    off = win->h - win->cur.h;
    for (; i < rw->itemCount; i++) {
        it = &g_itemTable[rw->items[i][0]];
        y = i * 15 + 7;
        y2 = i * 15 + 10;
        text_formatNumber(rw->items[i][1], s_rewardsTextBuf, 2, TEXT_NUM_ASCII);
        ui_drawItemIcon(win->x + 7, win->y + y - off, w->otz, rw->items[i][0], 1);
        text_drawBlock(win->x + 0x16, win->y + y - off, w->otz, g_itemNames + it->name, TEXT_PAL_WHITE);
        ui_drawSmallDigits(win->x + 0x88, win->y + y2 - off, w->otz, s_rewardsTextBuf, TEXT_PAL_WHITE);
    }
    if (rw->itemCount == 0) {
        msg = text_get(REWARDS_TEXT_NO_ITEM);
        text_drawBlock(win->x + ((win->w - (text_getInfo(REWARDS_TEXT_NO_ITEM) & 0xFFF)) >> 1), win->y - (k1 = off - 0x28), w->otz, msg, TEXT_PAL_WHITE);
    }
    if (rw->card != ITEM_NONE) {
        k = off - 0x66;
        ui_drawSpriteDef(win->x + 8, win->y - k, w->otz, &s_rewardsCardIcon, 0);
        text_copy(s_rewardsTextBuf, (u8 *)card_getName(rw->card));
        text_append(s_rewardsTextBuf, text_get(REWARDS_TEXT_CARD));
        text_drawBlock(win->x + 0x16, win->y - k, w->otz, s_rewardsTextBuf, TEXT_PAL_WHITE);
    }
    ui_drawEtchedLine(win->x + 2, win->y - (k3 = off - 0x61), w->otz, win->w - 5);
}

void rewards_resetFullWarning(void) {
    s_rewardsFullShown = 0;
}

void rewards_onStepGil(void) {
}

u8 rewards_tryAddItems(void) {
    u8 done = 0;

    if (s_rewardsFullShown != 1) {
        if (!rewards_addItems()) {
            snd_playSe(SE_MESSAGE);
            s_rewardsFullShown = 1;
        } else {
            done = 1;
        }
    } else {
        win_setCloseAnim(s_rewards.fullWin.win, WIN_ANIM_ZOOM);
        win_close(s_rewards.fullWin.win);
        done = 1;
    }
    if (done) {
        if (s_rewards.rewards.itemCount != 0 || s_rewards.rewards.card != ITEM_NONE) {
            snd_playSe(SE_ITEMS);
        }
    }
    return done;
}

void rewards_openItemsWin(void) {
    s_rewards.itemsWin = win_openFree(&s_rewardsItemsRect, 0xE, WIN_STYLE_LINE_RULE);
    win_setDrawContent(s_rewards.itemsWin, rewards_drawItems);
    win_setDrawOverlay(s_rewards.itemsWin, rewards_drawItemsLabels);
    win_setShadow(s_rewards.itemsWin, 1);
    win_setOpenFrames(s_rewards.itemsWin, 0x10);
}

void rewards_drawItemsLabels(Window *w) {
    s32 labelW = ui_measureLabel((u8 *)g_winLabelStrings.itemCount);
    s32 k = 0x97;

    ui_drawLabel(w->x + 8, w->y - 3, w->otz - 1, g_winLabelStrings.itemList, UI_LABEL_NORMAL);
    ui_drawLabel(w->x - (labelW - k), w->y - 3, w->otz - 1, g_winLabelStrings.itemCount, UI_LABEL_NORMAL);
}

void rewards_drawFullWarning(Window *win) {
    text_drawBlock(win->x + 8, win->y + 8, win->otz, text_get(REWARDS_TEXT_ITEMS_FULL), TEXT_PAL_WHITE);
}

void rewards_onFullWarningClosed(void) {
    s_rewards.fullWin.win = NULL;
}

INCLUDE_RODATA("asm/us/ovl_65/nonmatchings/battle_end", g_rewardsText);

INCLUDE_RODATA("asm/us/ovl_65/nonmatchings/battle_end", s_rewardsImage0);

INCLUDE_RODATA("asm/us/ovl_65/nonmatchings/battle_end", s_rewardsImage1);
