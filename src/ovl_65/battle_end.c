#include "ovl_65.h"

void battleEnd_main(void) {
    Unk11C0 *s;
    GameData *ctx;
    u8 i;

    s = g_gameState->unk1C->unk8E4.ovl65->unk11C0;
    battleEnd_init();
    do {
        ClearOTagR(g_gameState->unk1C->unk10[g_gameState->unk1C->unk8], 0x1000);
        pad_update();
        switch (battleEnd_update()) {
        case 1:
            g_gameState->mode = s->unk6;
            break;
        case 2:
            g_gameState->mode = 0;
            break;
        }
        game_updateWindows();
        game_updateFade();
        game_updateTimer();
        preload_update();
        DrawSync(0);
        VSync(g_gameState->vsyncMode);
        ResetGraph(1);
        PutDispEnv(&g_gameState->dispEnv[g_gameState->unk1C->unk8]);
        ctx = g_gameState->unk1C;
        i = ctx->unk8;
        DrawOTagEnv(ctx->unk10[i] + 0xFFF, &g_gameState->drawEnv[i]);
        g_gameState->unk1C->unk8 ^= 1;
        gfx_setBuffer(g_gameState->unk1C->unk8);
    } while (g_gameState->mode == 8);
    battleEnd_exit();
}

void battleEnd_init(void) {
    Unk11C0 *s = g_gameState->unk1C->unk8E4.ovl65->unk11C0;
    FileEntry *e;

    game_freezeScreen();
    preload_setHook(0);
    rewards_init();
    battleEnd_loadDb();
    func_801EF6D8();
    battleEnd_unloadDb();
    if (s->unk10 != 0 && g_btlRewards.unkB != 0) {
        evt_init(s->unk10);
        if (s->unk14 != 0) {
            msg_init();
        }
    }
    if (g_gameState->unk1C->unk56F & 1) {
        e = dep_find(g_gameState->unk1C->depTable, g_gameState->unk1C->fieldId | 0x50000);
        if (e != NULL) {
            g_gameState->unk1C->unk3A = e->unkC;
            g_gameState->unk1C->sceneSector[1] = e->unk4;
            g_gameState->unk1C->sceneSectors[1] = e->unk8;
            e = dep_find(g_gameState->unk1C->depTable, e->unkC);
            if (e != NULL) {
                g_gameState->unk1C->sceneSector[0] = e->unk4;
                g_gameState->unk1C->sceneSectors[0] = e->unk8;
            }
        }
    }
    battleEnd_preloadScene();
}

void battleEnd_exit(void) {
    GameData *g = g_gameState->unk1C;
    s32 i;

    if (!(g->unk56F & 8) && (g->unk0 & 0x20000000) && g->unk81C[0] != 0xFFFF) {
        g->unk0 |= 0x400000;
    }
    g_gameState->unk1C->depTable = (FileTable *)-1;
    if (g_gameState->mode == 0) {
        btl_soundCmd(0x8601, 0, 0x1E, 0, 0);
        for (i = 29; i >= 0; i--) {
            VSync(0);
        }
        g = g_gameState->unk1C;
        g->unk0 &= ~0x400000;
        snd_quit(g);
        D_80067950++;
        g_gameState->unk1C->unk0 |= 0x100000;
    }
}

s32 func_801EF6D8(void) {
    func_800E9DBC();
    return 0;
}

s32 battleEnd_loadDb(void) {
    FileEntry *e;
    PreloadEntry *f;

    if (preload_isReady(0x2713)) {
        f = preload_find(0x2713);
        file_copyViaVram(D_80067948, f->unk8, f->unkC, 0);
        preload_remove(0x2713);
        DrawSync(0);
    } else {
        e = file_find(1, 0x2713);
        cdRead_init(g_gameState->unk1C->unk854, e->unk4, (e->unkC - e->unk4) << 11, (void *)D_80067948, 0, -1, 0);
        while (cdRead_update(g_gameState->unk1C->unk854)) {
        }
    }
    g_gameState->unk1C->unk8E4.ovl65->unk1354->unk4 = D_80067948;
    db_register(g_gameState->unk1C->unk8E4.ovl65->unk1354->unk4, 0);
    return 0;
}

s32 battleEnd_unloadDb(void) {
    Unk1354 *p = g_gameState->unk1C->unk8E4.ovl65->unk1354;
    if (p->unk4 != 0) {
        db_unregister((void *)p->unk4);
        g_gameState->unk1C->unk8E4.ovl65->unk1354->unk4 = 0;
    }
    return 0;
}

void battleEnd_preloadScene(void) {
    Unk11C0 *b;
    FileEntry *f;
    GameData *g;
    s32 id;
    s32 size;
    s32 at;
    u8 *dst;
    s32 sector;
    s32 k;

    b = g_gameState->unk1C->unk8E4.ovl65->unk11C0;
    preload_init();
    g_gameState->unk1C->unk8BC = NULL;
    if (g_gameState->unk1C->battleEndKind == 6) {
        return;
    }
    id = 0x1000C;
    if (b->unk6 == 1) {
        id = 0x1000A;
    }
    f = file_find(0, id);
    dst = b->unk1C;
    size = (f->unkC - f->unk4) << 11;
    preload_setEntry(preload_add(), 0, id, (s32)dst, size, size, f->unk4, size);
    dst += size;
    if (b->unk6 == 1) {
        id = 0x2714;
    } else if (g_evtGlobalVars.h[0] >= 0x2B52) {
        id = 0x2716;
    } else {
        id = 0x2712;
    }
    f = file_find(1, id);
    k = -0x146F00;
    size = (f->unkC - f->unk4) << 11;
    at = D_80067944 - (dst + k);
    at = at / 0x800 * 0x800;
    preload_setEntry(preload_add(), 0, id, (s32)dst, at, size, f->unk4, size);
    dst += size;
    if (b->unk6 != 1) {
        return;
    }
    sector = g_gameState->unk1C->sceneSector[0];
    id = g_gameState->unk1C->unk3A | 0x60000;
    size = g_gameState->unk1C->sceneSectors[0] << 11;
    at = D_80067944 - (dst + k);
    at = at / 0x800 * 0x800;
    preload_setEntry(preload_add(), 0, id, (s32)dst, at, size, sector, size);
    dst += size;
    sector = g_gameState->unk1C->sceneSector[1];
    id = g_gameState->unk1C->fieldId | 0x50000;
    size = g_gameState->unk1C->sceneSectors[1] << 11;
    at = D_80067944 - (dst + k);
    at = at / 0x800 * 0x800;
    preload_setEntry(preload_add(), 0, id, (s32)dst, at, size, sector, size);
    g = g_gameState->unk1C;
    if ((s16)g->fieldId == g->unk7EA && g->unk8AC != -1) {
        sector = g->unk8AC;
        id = g->unk7E8 | 0x70000;
        preload_setEntry(preload_add(), 1, id, 0, 0, 0, sector, -1);
    }
}

void rewards_drawInfo(void) {
    RECT *r = &s_rewards.wins[0].unk4;
    s32 x, y;
    ui_drawLabel(r->x + 6, r->y - 3, 0xB, D_80070460, 0);
    win_drawBox(r, 0xB, 1);
    win_drawShadow(r, 0xC, 1);
    x = r->x;
    y = r->y;
    s_rewards.prim = text_buildBlock(x + 0x13, y + 6, g_ot + 10,
                                      text_get(s_rewardsTitleText[s_rewards.screen]), 0, s_rewards.prim);
}

void rewards_drawMembers(void) {
    u8 buf[16];
    RECT *r;
    Character *c;
    u32 i;
    u8 dim;
    s32 col;
    u8 alive;
    u32 n;
    u8 *p;

    for (i = 0; i < 4; i++) {
        if (s_rewards.winShown[i + 2] != 0) {
            r = &s_rewards.wins[i + 2].unk4;
            c = g_gameState->unk1C->party[i];
            if (c != NULL) {
                alive = rewards_isRewarded(c, i);
                dim = !alive;
                col = dim * 2;
                if (!alive) {
                    rewards_drawPanelShade(r);
                }
                ui_drawPartyPortrait(r->x + 4, r->y + 4, 0xB, i, dim);
                rewards_drawLevelUp(r->x + 0x24, r->y + 0x12, 0xB, &s_rewards.levelPopups[i]);
                s_rewards.prim = text_buildBlock(r->x + 0x29, r->y + 4, g_ot + 0xB, c->unk0, col, s_rewards.prim);
                text_formatNumber(c->level, buf, 2, 1);
                ui_drawSprite(r->x + 0x72, r->y + 7, 0xB, 0xB4, dim);
                p = buf;
                s_rewards.prim = text_buildDigits(r->x + 0x84, r->y + 5, g_ot + 0xB, p, col, s_rewards.prim);
                ui_drawStatusIcons(r->x + 0x28, r->y + 0x13, 0xB, c->status, dim);
                text_drawBlock(r->x + 0x29, r->y + 0x1C, 0xB, text_get(REWARDS_TEXT_EXP), col);
                text_formatNumber(c->exp, p, 7, 1);
                ui_drawSmallDigits(r->x + 0x62, r->y + 0x20, 0xB, p, col);
                if (c->level < LEVEL_MAX) {
                    n = g_levelExp[c->level];
                } else {
                    n = c->exp;
                }
                text_formatNumber(n - c->exp, buf, 6, 1);
                ui_drawSmallDigits(r->x + 0x69, r->y + 0x2A, 0xB, buf, col);
                text_drawBlock(r->x + 0x29, r->y + 0x26, 0xB, text_get(REWARDS_TEXT_NEXT), col);
            }
            win_drawBox(r, 0xB, 0);
            win_drawShadow(r, 0xC, 0);
        }
    }
}

void rewards_drawExp(void) {
    RECT *r = &s_rewards.wins[6].unk4;
    u8 buf[16];
    s32 x, y;

    if (s_rewards.winShown[6] != 0) {
        win_drawBox(r, 0xB, 2);
        win_drawShadow(r, 0xC, 2);
        text_formatNumber(s_rewards.rewards.expEach, buf, 6, 1);
        x = r->x;
        y = r->y;
        s_rewards.prim = text_buildBlock(x + 0xC, y + 5, g_ot + 10,
                                          text_get(REWARDS_TEXT_EXP), 0, s_rewards.prim);
        s_rewards.prim = text_buildSmall(r->x + 0x5A, r->y + 9, g_ot + 10,
                                          buf, 0, s_rewards.prim);
    }
}

void rewards_drawAp(void) {
    RECT *r = &s_rewards.wins[7].unk4;
    u8 buf[16];
    s32 x, y;

    if (s_rewards.winShown[7] != 0) {
        win_drawBox(r, 0xB, 2);
        win_drawShadow(r, 0xC, 2);
        text_formatNumber(s_rewards.rewards.ap, buf, 3, 1);
        x = r->x;
        y = r->y;
        s_rewards.prim = text_buildBlock(x + 0xC, y + 5, g_ot + 10,
                                          text_get(REWARDS_TEXT_AP), 0, s_rewards.prim);
        s_rewards.prim = text_buildSmall(r->x + 0x6F, r->y + 9, g_ot + 10,
                                          buf, 0, s_rewards.prim);
    }
}

void rewards_drawReceivedGil(void) {
    RECT *r = &s_rewards.wins[8].unk4;
    u8 buf[16];
    s32 x, y;
    u32 gil = s_rewards.rewards.gil.target - s_rewards.rewards.gil.cur;

    if (gil > 999999) {
        gil = 999999;
    }
    if (s_rewards.winShown[8] != 0) {
        win_drawBox(r, 0xE, 2);
        win_drawShadow(r, 0xF, 2);
        text_formatNumber(gil, buf, 6, 1);
        strcat(buf, s_rewardsGilUnit);
        x = r->x;
        y = r->y;
        s_rewards.prim = text_buildBlock(x + 0xC, y + 5, g_ot + 13,
                                          text_get(REWARDS_TEXT_RECEIVED_GIL), 0, s_rewards.prim);
        s_rewards.prim = text_buildSmall(r->x + 0x54, r->y + 9, g_ot + 13,
                                          buf, 0, s_rewards.prim);
    }
}

void rewards_drawCurrentGil(void) {
    RECT *r = &s_rewards.wins[9].unk4;
    u8 buf[16];
    s32 x, y;
    u32 gil = g_gameState->unk1C->gil;

    if (gil > GIL_MAX) {
        gil = GIL_MAX;
    }
    if (s_rewards.winShown[9] != 0) {
        win_drawBox(r, 0xE, 2);
        win_drawShadow(r, 0xF, 2);
        text_formatNumber(gil, buf, 7, 1);
        strcat(buf, s_rewardsGilUnit2);
        x = r->x;
        y = r->y;
        s_rewards.prim = text_buildBlock(x + 0xC, y + 5, g_ot + 13,
                                          text_get(REWARDS_TEXT_CURRENT_GIL), 0, s_rewards.prim);
        s_rewards.prim = text_buildSmall(r->x + 0x4D, r->y + 9, g_ot + 13,
                                          buf, 0, s_rewards.prim);
    }
}

void rewards_drawPanelShade(RECT *r) {
    u8 *p = s_rewards.prim;

    setlen(p, 3);
    ((TileWords *)p)->rgbc = 0x62202020;
    ((TileWords *)p)->xy = ((r->x + 2) & 0xFFFF) | ((r->y + 2) << 16);
    ((TileWords *)p)->wh = (r->w - 4) | ((r->h - 4) << 16);
    addPrim(g_ot + 11, p);
    p += 0x10;
    SetDrawTPage(p, 1, 1, 0);
    addPrim(g_ot + 11, p);
    p += 8;
    s_rewards.prim = p;
}

void rewards_initWindows(void) {
    s32 i;

    for (i = 0; i < 10; i++) {
        s_rewards.winShown[i] = s_rewardsWinInit[i].shown;
        s_rewards.wins[i].unk4 = s_rewardsWinInit[i].rect;
        if (s_rewardsWinInit[i].shown != 0) {
            func_80063118(&s_rewards.wins[i], s_rewardsWinOpen[i], 1);
        } else {
            func_80063118(&s_rewards.wins[i], NULL, 0);
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
    TweenDef *t;
    WinDef *d;

    for (i = 0; i < 4; i++) {
        if (s_rewards.getsExp[i] != 0) {
            w = s_rewards.levelWins[i].win;
            if (w != NULL && w->unk1C != 0) {
                tx = w->unk0 - 0x26;
                dx = s_rewards.wins[i + 2].unk4.x - tx;
                ty = w->unk2 - 2;
                dy = s_rewards.wins[i + 2].unk4.y - ty;
                w->unk0 += dx;
                s_rewards.levelWins[i].win->unk8.x += dx;
                s_rewards.levelWins[i].win->unk2 += dy;
                s_rewards.levelWins[i].win->unk8.y += dy;
            }
        }
        if (s_rewards.getsAp[i] != 0) {
            w2 = s_rewards.learnWins[i].win;
            d = &s_rewards.wins[i + 2];
            if (w2 != NULL && w2->unk1C != 0) {
                t = d->unkC;
                if (t != NULL && d->unk2 >= 2) {
                    if (t->unk4 != NULL) {
                        w2->unk0 += t->unk4[d->unk2];
                        s_rewards.learnWins[i].win->unk8.x += t->unk4[d->unk2];
                    }
                    if (t->unk8 != NULL) {
                        s_rewards.learnWins[i].win->unk2 += t->unk8[d->unk2];
                        s_rewards.learnWins[i].win->unk8.y += t->unk8[d->unk2];
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
        func_801F325C();
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
        if (D_80077DB5 != 0) {
            menu_close();
        } else {
            fade_out(0x10);
            fade_setOnDone(rewards_onClosed);
        }
    }
}

void rewards_finishExp(void) {
    Rewards *r = &s_rewards.rewards;
    s32 i;

    for (i = 0; i < 4; i++) {
        if (s_rewards.getsExp[i] != 0) {
            rewards_addExp(i, r->exp[i].target - r->exp[i].cur);
            rewards_showLevelUp(i);
        }
    }
    func_8002F55C(SE_EXP_COUNT);
    snd_playSe(SE_CURSOR);
    for (i = 0; i < 7; i++) {
        func_80063118(&s_rewards.wins[2 + i], &s_rewardsWinClose[i], 0);
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
    Character *c;

    for (i = 0; i < 4; i++) {
        s_rewards.getsAp[i] = 0;
        s_rewards.getsExp[i] = 0;
    }
    i = 0;
    s_rewards.expMembers = 0;
    for (; i < 4; i++) {
        if ((g_btlRewards.expMask >> i) & 1) {
            if (g_gameState->unk1C->party[i] != NULL) {
                s_rewards.getsExp[i] = func_801F_canExp(g_gameState->unk1C->party[i]);
                s_rewards.getsAp[i] = func_801F_canAp(g_gameState->unk1C->party[i]);
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
    u32 q;

    rw = &s_rewards.rewards;
    memset(rw, 0, sizeof(Rewards));
    rewards_findMembers();
    if (s_rewards.expMembers != 0) {
        q = g_btlRewards.exp / s_rewards.expMembers;
    } else {
        q = 0;
    }
    rw->expEach = q;
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

void rewards_drawSmallText(s32 arg0, s32 arg1, s32 arg2, u8 *arg3, s32 arg4) {
    s_rewards.prim = text_buildSmall(arg0, arg1, g_ot + arg2, arg3, arg4, s_rewards.prim);
}

void rewards_drawText(s32 arg0, s32 arg1, s32 arg2, u8 *arg3, s32 arg4) {
    s_rewards.prim = text_buildBlock(arg0, arg1, g_ot + arg2, arg3, arg4, s_rewards.prim);
}

void rewards_init(void) {
    game_initGfx();
    func_8002C064(0);
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
        if (D_80077DB5 == 0) {
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
    u8 mode = g_gameState->unk1C->battleEndKind;
    s32 ret = 0;

    if (mode == 4) {
        ret = D_800FDF4D != 0;
    } else if (mode != 7) {
        ret = 1;
        if (g_btlRewards.unkB != 0) {
            ret = 2;
        }
    }
    return ret;
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
    RECT *r = &s_rewards.wins[1].unk4;
    ui_drawLabel(r->x + 6, r->y - 3, 0xB, D_80070450, 0);
    win_drawBox(r, 0xB, 1);
    win_drawShadow(r, 0xC, 1);
    x = r->x;
    y = r->y;
    s_rewards.prim = text_buildBlock(x + 0x13, y + 6, g_ot + 10,
                                      text_get(REWARDS_TEXT_CONFIRM), 0, s_rewards.prim);
}

void rewards_initPrimBufs(void) {
    s_rewards.primBufs[0] = s_rewardsPrimBufs[0];
    s_rewards.primBufs[1] = s_rewardsPrimBufs[1];
}

void rewards_clearWinSlot(WinSlot *arg0) {
    arg0->queued = 0;
    arg0->shown = 0;
    arg0->timer = 0;
    arg0->delay = 0;
    arg0->win = 0;
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
        if (s_rewards.winShown[i] != 0 && s_rewards.wins[i].unkC != 0) {
            func_800631AC(&s_rewards.wins[i]);
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
    Rewards *r = &s_rewards.rewards;
    s32 i;
    u8 any = 0;

    for (i = 0; i < 4; i++) {
        if (s_rewards.getsAp[i] != 0) {
            rewards_addAp(i, r->apGain[i].target);
        }
        if (s_rewards.getsExp[i] != 0) {
            any = 1;
        }
    }
    if (any) {
        func_8002F514(SE_EXP_COUNT);
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
            func_8002F514(SE_GIL_COUNT);
        }
    }
}

void rewards_finishGil(void) {
    Rewards *r = &s_rewards.rewards;

    if (r->gil.target != r->gil.cur) {
        rewards_addGil(r->gil.target - r->gil.cur);
        r->gil.cur = r->gil.target;
        func_8002F55C(SE_GIL_COUNT);
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

u8 rewards_isRewarded(Character *arg0, s32 arg1) {
    if (!func_801F_alive(arg0)) {
        return 0;
    }
    return (g_btlRewards.expMask >> arg1) & 1;
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

void rewards_drawDigits(s32 arg0, s32 arg1, s32 arg2, u8 *arg3, s32 arg4) {
    s_rewards.prim = text_buildDigits(arg0, arg1, g_ot + arg2, arg3, arg4, s_rewards.prim);
}

void rewards_updateLearnWins(void) {
    WinSlot *w;
    RECT *r;
    s32 i;
    RECT *rr;

    w = s_rewards.learnWins;
    for (i = 0; i < 4; w++, i++) {
        if (s_rewards.getsAp[i] != 0) {
            if (w->delay == 0) {
                if (w->win == NULL) {
                    if (w->queued != 0) {
                        s_rewardsLearnPanel = r = &s_rewards.wins[i + 2].unk4;
                        s_rewardsLearnRect.x = r->x + 0x26;
                        rr = &s_rewardsLearnRect;
                        rr->y = r->y + 0x1B;
                        rr->w = 0x70;
                        rr->h = 0x19;
                        w->win = win_openFree(rr, 6, 2);
                        w->win->unk50 = i;
                        win_setTitle(w->win, (s32)s_rewardsLearnLabel);
                        win_setDrawContent(w->win, rewards_drawLearnWin);
                        win_setOnClosed(w->win, rewards_onLearnWinClosed);
                        win_setOpenAnim(w->win, 0);
                        snd_playSe(SE_LEARN);
                        w->queued--;
                    }
                } else if (++w->timer >= 0x40) {
                    win_setCloseAnim(w->win, 0);
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

void rewards_addAp(s32 arg0, u32 arg1) {
    Character *c;
    s32 i;
    s32 j;
    Unk80069018 *e;
    s32 k;
    s32 need;
    s32 have;

    c = g_gameState->unk1C->party[arg0];
    if (arg1 != 0 && func_80034A30(c)) {
        s_rewards.rewards.apGain[arg0].cur += arg1;
        for (i = 0; i < 5; i++) {
            if (c->equip[i] != ITEM_NONE) {
                e = &D_80069018[c->equip[i]];
                for (j = 0; j < 3; j++) {
                    if (e->unkC[j] != 0) {
                        k = chr_findAbilitySlot(c->unk34.b.id, e->unkC[j]);
                        if (k >= 0) {
                            need = D_8006CB28[c->unk34.b.row][k].unk1;
                            have = c->ap[k];
                            if (have < need) {
                                if (arg1 >= need - have) {
                                    c->ap[k] = need;
                                    rewards_addLearned(arg0, e->unkC[j]);
                                } else {
                                    c->ap[k] = have + arg1;
                                }
                            }
                        }
                    }
                }
            }
        }
    }
}

u8 rewards_willLearn(Character *c, s32 arg1) {
    u8 ret;
    s32 i;
    s32 j;
    s32 ch;
    s32 k;
    u8 *eq;
    Unk80069018 *e;
    s32 have;

    ret = 0;
    eq = c->equip;
    ch = c->unk34.b.id;
    memset(s_rewardsAbilityItems, 0, 0x30);
    for (i = 0; i < 5; i++) {
        if (eq[i] != ITEM_NONE) {
            e = &D_80069018[eq[i]];
            for (j = 0; j < 3; j++) {
                if (e->unkC[j] != 0) {
                    k = chr_findAbilitySlot(ch & 0xFF, e->unkC[j]);
                    if (k >= 0) {
                        have = c->ap[k];
                        if (have < D_8006CB28[c->unk34.b.row][k].unk1) {
                            s_rewardsAbilityItems[k]++;
                        }
                    }
                }
            }
        }
    }
    for (i = 0; i < 0x30; i++) {
        if (s_rewardsAbilityItems[i] != 0) {
            have = c->ap[i];
            if (s_rewardsAbilityItems[i] * arg1 >= D_8006CB28[c->unk34.b.row][i].unk1 - have) {
                ret = 1;
                break;
            }
        }
    }
    return ret;
}

void rewards_drawLearnWin(Window *w) {
    Window *v;
    WinSlot *s;
    TileWords *t;
    u8 *name;
    s32 slot;
    s32 icon;
    s32 id;

    v = w;
    slot = w->unk50;
    s = &s_rewards.learnWins[slot];
    id = s->ids[s->shown];
    if (id < ABILITY_SUPPORT_FIRST) {
        name = s_rewardsActionNames + s_rewardsActionTable[id].unkE;
        ui_drawSprite(w->unk0 + 6, w->unk2 + 7, w->unk18, 0xBC, 0);
    } else {
        name = s_rewardsSupportNames + D_8006D128[id - ABILITY_SUPPORT_FIRST].unk2;
        if (chr_testSupportBit(g_gameState->unk1C->party[slot]->supportAbilities, id)) {
            icon = 0xBD;
        } else {
            icon = 0xF4;
        }
        ui_drawSprite(w->unk0 + 6, w->unk2 + 7, w->unk18, icon, 0);
    }
    text_drawBlock(v->unk0 + 0x16, v->unk2 + 6, w->unk18, name, 0);
    setlen(g_primPtr, 3);
    t = (TileWords *)g_primPtr;
    t->rgbc = 0x62101010;
    t->xy = (v->unk0 + 4) | ((v->unk2 + 4) << 16);
    t->wh = (v->unk4 - 4) | ((v->unk6 - 4) << 16);
    addPrim(&g_ot[w->unk18], t);
    g_primPtr += sizeof(TileWords);
    SetDrawTPage(g_primPtr, 1, 1, 0x20);
    addPrim(&g_ot[w->unk18], g_primPtr);
    g_primPtr += sizeof(DrTpageWords);
}

void rewards_initLearns(void) {
    s32 i;

    for (i = 0; i < 4; i++) {
        if (s_rewards.getsAp[i] != 0) {
            s_rewards.learns[i] = rewards_willLearn(g_gameState->unk1C->party[i], rewards_initApCounter(i));
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
    Counter *c = &s_rewards.rewards.apGain[i];

    if (chr_hasSupportAbility(g_gameState->unk1C->party[i]->unk34.b.id, SUPPORT_ABILITY_UP)) {
        c->target = ap * 2;
    } else {
        c->target = ap;
    }
    c->step = 1;
    c->cur = 0;
    return c->target;
}

void rewards_stepAp(void) {
    s32 i;
    Counter *c;

    for (i = 0; i < 4; i++) {
        if (s_rewards.getsAp[i] != 0) {
            c = &s_rewards.rewards.apGain[i];
            if (c->cur < c->target) {
                rewards_addAp(i, Counter_step(c));
            }
        }
    }
}

void rewards_addLearned(s32 arg0, u8 arg1) {
    s_rewards.learnWins[arg0].ids[s_rewards.learnWins[arg0].queued] = arg1;
    s_rewards.learnWins[arg0].queued++;
    s_rewards.learned[arg0].ids[s_rewards.learned[arg0].count] = arg1;
    s_rewards.learned[arg0].count++;
}

void rewards_onLearnWinClosed(Window *arg0) {
    s_rewards.learnWins[arg0->unk50].win = 0;
}

void rewards_drawLevelUp(s32 x, s32 y, s32 otz, Popup *p) {
    SprtWords *s;
    u8 *pkt;
    u32 clut;
    u32 xy;
    DR_TPAGE *t;

    if (p->active != 0 && p->phase != 3) {
        y += p->y;
        clut = GetClut(0, 0x1FF) << 16;
        setlen(g_primPtr, 4);
        xy = (x & 0xFFFF) | (y << 16);
        pkt = g_primPtr;
        s = (SprtWords *)pkt;
        s->rgbc = 0x64808080;
        s->xy = xy;
        s->uvclut = clut | 0xB000;
        s->wh = 0x14004C;
        g_primPtr = (u8 *)(s + 1);
        addPrim(&g_ot[otz], s);
        t = (DR_TPAGE *)g_primPtr;
        SetDrawTPage(t, 1, 1, 0x9F);
        addPrim(&g_ot[otz], g_primPtr);
        g_primPtr += sizeof(DR_TPAGE);
        setlen(g_primPtr, 4);
        s = (SprtWords *)g_primPtr;
        s->rgbc = 0x66808080;
        s->xy = xy;
        s->uvclut = clut | 0xC400;
        s->wh = 0x14004C;
        g_primPtr = (u8 *)(s + 1);
        addPrim(&g_ot[otz], s);
        SetDrawTPage((DR_TPAGE *)g_primPtr, 1, 1, 0xDF);
        addPrim(&g_ot[otz], g_primPtr);
        g_primPtr += sizeof(DR_TPAGE);
    }
}

void rewards_initExp(void) {
    s32 i;

    for (i = 0; i < 4; i++) {
        if (s_rewards.getsExp[i] != 0) {
            s_rewards.levelsUp[i] = rewards_willLevelUp(g_gameState->unk1C->party[i], rewards_initExpCounter(i));
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
    Character *c = g_gameState->unk1C->party[i];

    if (exp != 0) {
        if (c->exp >= EXP_MAX) {
            c->exp = EXP_MAX;
            return;
        }
        s_rewards.rewards.exp[i].cur += exp;
        c->exp += exp;
        if (c->exp >= EXP_MAX) {
            c->exp = EXP_MAX;
        }
        if (c->level < LEVEL_MAX && (need = g_levelExp[c->level]) <= c->exp) {
            do {
                rewards_levelUp(i);
            } while (c->level < LEVEL_MAX && (need = g_levelExp[c->level]) <= c->exp);
        }
    }
}

void rewards_showLevelUp(s32 arg0) {
    Popup *p = &s_rewards.levelPopups[arg0];

    if (p->pending != 0 || p->active != 0) {
        p->phase = POPUP_HOLD;
        p->y = -0x11;
        p->timer = 0;
        p->enabled = 0;
        p->active = 1;
    }
}

u32 rewards_initExpCounter(s32 arg0) {
    u32 exp = s_rewards.rewards.expEach;
    Character *m = g_gameState->unk1C->party[arg0];
    Counter *c = &s_rewards.rewards.exp[arg0];
    u32 step;

    c->target = exp;
    if (chr_hasSupportAbility(m->unk34.b.id, SUPPORT_LEVEL_UP)) {
        c->target += exp >> 1;
    }
    if (exp < 16) {
        c->step = 1;
    } else if (exp < 512) {
        c->step = (exp + 15) >> 4;
    } else {
        step = 32;
        c->step = step;
        if (exp / step > 192) {
            c->step = (exp + 191) / 192;
        }
    }
    c->cur = 0;
    return c->target;
}

s32 rewards_willLevelUp(Character *arg0, s32 arg1) {
    if (arg0->level >= LEVEL_MAX) {
        return 0;
    }
    return arg0->exp + arg1 >= g_levelExp[arg0->level];
}

void rewards_stepExp(void) {
    s32 i;
    Counter *c;
    s32 step;
    u8 done = 1;

    for (i = 0; i < 4; i++) {
        if (s_rewards.getsExp[i] != 0) {
            c = &s_rewards.rewards.exp[i];
            if (c->cur < c->target) {
            step = Counter_step(c);
            rewards_addExp(i, step);
            done = 0;
            }
        }
    }
    if (done) {
        func_8002F55C(SE_EXP_COUNT);
    }
}

void rewards_updateLevelUps(void) {
    Popup *p;
    s32 i;

    for (p = s_rewards.levelPopups, i = 0; i < 4; p++, i++) {
        if (s_rewards.getsExp[i] != 0) {
            if (p->active == 0) {
                if (p->pending != 0) {
                    rewards_startPopup(p);
                }
            } else if (p->enabled != 0 && rewards_stepPopup(p) == POPUP_DONE) {
                rewards_endPopup(p);
            }
        }
    }
}

void rewards_levelUp(s32 arg0) {
    Character *m = g_gameState->unk1C->party[arg0];

    chr_levelUp(m->unk34.b.id, m->level + 1);
    s_rewards.levelWins[arg0].queued++;
    s_rewards.levelPopups[arg0].pending++;
}

void rewards_initPopup(Popup *arg0) {
    arg0->active = 0;
    arg0->pending = 0;
    arg0->y = 0;
    arg0->timer = 0;
    arg0->phase = 0;
    arg0->enabled = 1;
}

void rewards_startPopup(Popup *arg0) {
    arg0->timer = 0;
    arg0->y = 0;
    arg0->phase = POPUP_RISE;
    arg0->active = 1;
    arg0->pending--;
    snd_playSe(SE_LEVEL_UP);
}

void rewards_endPopup(Popup *arg0) {
    arg0->active = 0;
    arg0->timer = 0;
    arg0->y = 0;
    arg0->count++;
}

s32 rewards_stepPopup(Popup *arg0) {
    switch (arg0->phase) {
    case POPUP_RISE:
        arg0->y += s_rewardsPopupRise[arg0->timer];
        if (++arg0->timer >= 10) {
            arg0->phase = POPUP_HOLD;
            arg0->timer = 0;
        }
        break;
    case 2:
        if (++arg0->timer >= 0x40) {
            arg0->timer = 0;
            if (arg0->pending != 0) {
                arg0->phase = POPUP_GAP;
            } else {
                arg0->phase = POPUP_DONE;
            }
        }
        break;
    case 3:
        if (++arg0->timer >= 4) {
            arg0->phase = POPUP_DONE;
            arg0->timer = 0;
        }
        break;
    }
    return arg0->phase;
}

void rewards_initGil(void) {
    Counter *c = &s_rewards.rewards.gil;
    s32 i;
    u8 found = 0;
    Character *m;

    for (i = 0; i < 4; i++) {
        m = g_gameState->unk1C->party[i];
        if (m != NULL && chr_hasSupportAbility(m->unk34.b.id, SUPPORT_MILLIONAIRE)) {
            found = 1;
        }
    }
    if (found) {
        c->target += c->target >> 1;
    }
    c->step = 1;
    c->cur = 0;
}

void rewards_stepGil(void) {
    Counter *c = &s_rewards.rewards.gil;
    s32 step;

    if (c->cur < c->target) {
        step = Counter_step(c);
        rewards_addGil(step);
        if (c->cur >= c->target) {
            func_8002F55C(SE_GIL_COUNT);
        }
    }
}

void rewards_addGil(s32 arg0) {
    u32 gil;

    if (arg0 != 0) {
        s_rewards.rewards.gil.cur += arg0;
        gil = g_gameState->unk1C->gil;
        if (gil + arg0 > GIL_MAX) {
            arg0 = GIL_MAX - gil;
            s_rewards.rewards.gil.cur = s_rewards.rewards.gil.target;
        }
        g_gameState->unk1C->gil += arg0;
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
    u8 f;
    Rewards *rw;
    WinSlot *ws;
    s32 n;
    s32 k;

    full = 0;
    rw = &s_rewards.rewards;
    ws = &s_rewards.fullWin;
    for (i = 0; i < rw->itemCount; i++) {
        n = rw->items[i][1];
        if (n != item_add(rw->items[i][0], n)) {
            full = 1;
        }
    }
    f = full;
    if (rw->card != ITEM_NONE) {
        card_add(rw->card);
    }
    if (f) {
        k = 0x10;
        ts = text_measure(text_get(REWARDS_TEXT_ITEMS_FULL));
        rr.x = s_rewards.itemsWin->unk0 + ((s_rewards.itemsWin->unk4 - k - ts.w) >> 1);
        rr.y = s_rewards.itemsWin->unk2 + ((s_rewards.itemsWin->unk6 - k - ts.h) >> 1);
        rr.w = ts.w + 0x10;
        rr.h = ts.h + 0xF;
        ws->win = win_openFree(&rr, 6, 2);
        win_setDrawContent(ws->win, rewards_drawFullWarning);
        win_setOnClosed(ws->win, (void (*)(Window *))rewards_onFullWarningClosed);
        win_setOpenAnim(ws->win, 0);
        win_setShadow(ws->win, 1);
        win_setTitle(ws->win, D_800704C8);
    }
    return !f;
}

void rewards_drawItems(Window *w) {
    Window *r;
    Unk80069018 *it;
    u8 *s;
    u32 i;
    s32 off;
    s32 y;
    s32 y2;
    s32 k;
    s32 k1;
    s32 k3;
    Rewards *rw;

    r = w;
    i = 0;
    rw = &s_rewards.rewards;
    off = r->unk6 - r->unk8.h;
    for (; i < rw->itemCount; i++) {
        it = &D_80069018[rw->items[i][0]];
        y = i * 15 + 7;
        y2 = i * 15 + 10;
        text_formatNumber(rw->items[i][1], s_rewardsTextBuf, 2, 1);
        ui_drawItemIcon(r->unk0 + 7, r->unk2 + y - off, w->unk18, rw->items[i][0], 1);
        text_drawBlock(r->unk0 + 0x16, r->unk2 + y - off, w->unk18, D_8006A218 + it->unk0, 0);
        ui_drawSmallDigits(r->unk0 + 0x88, r->unk2 + y2 - off, w->unk18, s_rewardsTextBuf, 0);
    }
    if (rw->itemCount == 0) {
        s = text_get(REWARDS_TEXT_NO_ITEM);
        text_drawBlock(r->unk0 + ((r->unk4 - (text_getInfo(REWARDS_TEXT_NO_ITEM) & 0xFFF)) >> 1), r->unk2 - (k1 = off - 0x28), w->unk18, s, 0);
    }
    if (rw->card != ITEM_NONE) {
        k = off - 0x66;
        ui_drawSpriteDef(r->unk0 + 8, r->unk2 - k, w->unk18, &s_rewardsCardIcon, 0);
        text_copy(s_rewardsTextBuf, (u8 *)card_getName(rw->card));
        text_append(s_rewardsTextBuf, text_get(REWARDS_TEXT_CARD));
        text_drawBlock(r->unk0 + 0x16, r->unk2 - k, w->unk18, s_rewardsTextBuf, 0);
    }
    ui_drawEtchedLine(r->unk0 + 2, r->unk2 - (k3 = off - 0x61), w->unk18, r->unk4 - 5);
}

void rewards_resetFullWarning(void) {
    s_rewardsFullShown = 0;
}

void func_801F325C(void) {
}

u8 rewards_tryAddItems(void) {
    u8 ret = 0;

    if (s_rewardsFullShown != 1) {
        if (!rewards_addItems()) {
            snd_playSe(SE_ITEMS_FULL);
            s_rewardsFullShown = 1;
        } else {
            ret = 1;
        }
    } else {
        win_setCloseAnim(s_rewards.fullWin.win, 0);
        win_close(s_rewards.fullWin.win);
        ret = 1;
    }
    if (ret) {
        if (s_rewards.rewards.itemCount != 0 || s_rewards.rewards.card != ITEM_NONE) {
            snd_playSe(SE_ITEMS);
        }
    }
    return ret;
}

void rewards_openItemsWin(void) {
    s_rewards.itemsWin = win_openFree(&s_rewardsItemsRect, 0xE, 1);
    win_setDrawContent(s_rewards.itemsWin, rewards_drawItems);
    win_setDrawOverlay(s_rewards.itemsWin, rewards_drawItemsLabels);
    win_setShadow(s_rewards.itemsWin, 1);
    win_setOpenFrames(s_rewards.itemsWin, 0x10);
}

void rewards_drawItemsLabels(Window *w) {
    s32 n = ui_measureLabel((u8 *)g_winLabelStrings.unk94);
    s32 k = 0x97;

    ui_drawLabel(w->unk0 + 8, w->unk2 - 3, w->unk18 - 1, g_winLabelStrings.unk68, 0);
    ui_drawLabel(w->unk0 - (n - k), w->unk2 - 3, w->unk18 - 1, g_winLabelStrings.unk94, 0);
}

void rewards_drawFullWarning(Window *arg0) {
    text_drawBlock(arg0->unk0 + 8, arg0->unk2 + 8, arg0->unk18, text_get(REWARDS_TEXT_ITEMS_FULL), 0);
}

void rewards_onFullWarningClosed(void) {
    s_rewards.fullWin.win = NULL;
}

INCLUDE_RODATA("asm/us/ovl_65/nonmatchings/battle_end", g_rewardsText);

INCLUDE_RODATA("asm/us/ovl_65/nonmatchings/battle_end", s_rewardsImage0);

INCLUDE_RODATA("asm/us/ovl_65/nonmatchings/battle_end", s_rewardsImage1);
