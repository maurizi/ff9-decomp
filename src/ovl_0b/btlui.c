#include "ovl_0b.h"

void btlui_vsync(void) {
    GameData *g;
    g_btluiState.inVsync = 1;
    btlui_putDispEnvIfReady();
    btlui_onVsync();
    btlui_readPad();
    if (g_btluiState.frameReady != 0) {
        btlui_frameUpdate();
    }
    g = g_gameState->gameData;
    if (!(g->flags & GD_PAUSED) && !(g->ctx.battle->flags & BTL_FLAG_DISC_COVER_OPEN)) {
        if (g_btluiState.skipFrames == 0 && (u8)btlui_isEnabled()) {
            btlui_perfMark(0xBA);
            btlui_beginFrame();
            btlui_cacheFrame();
            btlui_updateTargetFlags();
            btlui_input();
            btlui_drawWindows();
            btlui_update();
            btlui_submitOt();
            btlui_perfMark(0xBB);
        }
    }
    g_btluiState.vsyncCount++;
    g_drawBufs.lastVsync++;
    g_btluiState.vsyncTick++;
    g_btluiState.frameReady = 0;
    g_btluiState.inVsync = 0;
}

void btlui_frameUpdate(void) {
    btlui_putDispEnvIfReady();
    g_drawBufs.cur = g_gameState->gameData->drawBuf ^ 1;
    if (g_btluiState.enabled == 0) {
        btlui_applyEnable();
    }
    if (!(g_gameState->gameData->flags & GD_PAUSED)) {
        if (!(g_gameState->gameData->ctx.battle->flags & BTL_FLAG_DISC_COVER_OPEN)) {
            if (g_btluiState.enabled != 0) {
                g_btluiState.hideWindows = (g_padHeld >> 19) & 1;
                g_btluiState.prevAtbOn = g_btluiState.atbOn;
                btlui_updateAtbFreeze();
                btlui_checkHelpClose();
                btlui_refreshPanel();
                btlui_resizeListWindow();
                btlui_collectCursorPositions();
                win_reapClosed();
            }
        }
        if (!(g_gameState->gameData->flags & GD_PAUSED) && !(g_gameState->gameData->ctx.battle->flags & BTL_FLAG_DISC_COVER_OPEN) && g_btluiState.skipFrames != 0) {
            g_btluiState.skipFrames--;
        }
    }
}

void btlui_init(void) {
    btlui_initMenu();
    btlui_initPanel();
    btlui_initPrims();
    g_btluiState.started = 1;
    g_btluiState.port = 0;
    VSyncCallback(btlui_vsync);
}

void btlui_uploadPartyNames(void) {
    void *p = db_lookupRegistered(DB_GET_ADDR, 0x14, 0);
    if (p != NULL) {
        text_uploadPartyNames((s32)p);
    }
}

void btlui_loadPortraits(void) {
    void *p = db_lookupRegistered(DB_GET_ADDR, 0x1E, 0);
    if (p != NULL) {
        ui_loadPartyPortraits(p);
        party_setChanged(0);
    }
}

void btlui_resetVsyncCount(void) {
    g_btluiState.vsyncCount = 0;
}

void btlui_reset(void) {
    memset(&g_btluiState, 0, 0x11C);
    btlui_initHeap();
    btlui_onReset();
    btlui_initDrawBufs();
    btlui_saveKeyRepeat();
    btlui_initHelp();
    btlui_initItemLists();
    text_setBank(g_battleText);
    VSyncCallback(NULL);
}

void btlui_exit(void) {
    btlui_enable(0);
    VSyncCallback(NULL);
    btlui_restoreKeyRepeat();
    btlui_clearDfe();
    g_btluiState.secondPort = 0;
}

void btlui_waitFrame(void) {
    DrawSync(0);
    btlfx_pollDrawSync();
    if (g_btluiState.vsyncCount < (u32)(g_gameState->vsyncMode - 1)) {
        VSync(g_gameState->vsyncMode - 1);
    }
    btlui_drawPortIcons();
    g_btluiState.frameReady = 1;
    VSync(0);
}

void btlui_enable(u8 enable) {
    if (enable) {
        g_btluiState.enableReq = 1;
    } else {
        g_btluiState.enabled = 0;
    }
}

void btlui_setAtbEnabled(u8 on) {
    if (g_btluiState.started != 0) {
        if (g_btluiState.atbOn != on) {
            g_btluiState.atbOn = on;
            if (on == 0) {
                btlui_closeMenu();
                btlui_closeHelp();
            }
            btlui_flushMenuQueue();
        }
    }
}

BtlUnit *btlui_getActiveUnit(void) {
    BtlUnit *u = NULL;
    if (g_btlUi->menuState != BTLUI_MENU_NONE) {
        u = g_btlUi->cmdUnit;
    }
    return u;
}

s32 btlui_isEnabled(void) {
    return g_btluiState.enabled != 0;
}

s32 btlui_isTargetSelect(void) {
    return g_btlUi->menuState == BTLUI_MENU_TARGET;
}

u8 btlui_isMultiTarget(void) {
    return g_btluiState.multiTarget;
}

u8 btl_isAtbRunning(void) {
    s32 f;
    if (g_btluiState.atbOn == 0) {
        return 0;
    }
    f = (g_gameState->config >> CFG_BIT_ATB) & 1;
    if (f != 1) {
        return 1;
    }
    return g_btlUi->menuState < BTLUI_MENU_LIST;
}

s32 btl_isTargetWindowOpen(void) {
    return btlui_isTargetWindowOpen();
}

u8 *btl_getAbilityName(s32 ability) {
    if (ability >= ABILITY_SUPPORT_FIRST) {
        return NULL;
    }
    return g_gameState->gameData->ctx.battle->abilities[ability].nameOfs + g_btlAbilityNames;
}

void btl_useReservedItem(s32 itemId) {
    btlui_unreserveItem(itemId);
    btlui_removeItem(itemId);
}

void btl_unreserveItem(s32 itemId) {
    btlui_unreserveItem(itemId);
}

void btl_reserveItem(s32 itemId) {
    btlui_reserveItem(itemId);
}

void btl_useItem(s32 itemId) {
    btlui_removeItem(itemId);
    if (btlui_getItemReserved(itemId) != 0 && item_getCount(itemId) == 0) {
        btlui_unreserveItem(itemId);
    }
}

void btl_addItem(s32 itemId) {
    btlui_addItem(itemId);
}

/* Dead code: no jal, pointer or address constant for it in any binary or effect module, no hit on an exec
 * breakpoint over 12 effects (runtime round 33). */
void btlui_latchFirstValue(s8 value) {
    if (g_btluiState.firstValueSet == 0) {
        g_btluiState.firstValueSet = 1;
        g_btluiState.firstValue = value;
    }
}

void btlui_drawOTagEnv(u32 *ot, DRAWENV *env) {
    g_btluiState.drawing = 1;
    DrawOTagEnv(ot, env);
    g_btluiState.drawing = 0;
}

void btlui_drawOTag(u32 *ot) {
    g_btluiState.drawing = 1;
    DrawOTag(ot);
    g_btluiState.drawing = 0;
}

u8 btlui_getPort(void) {
    return g_btluiState.port;
}

u8 btlui_isSecondPortUsed(void) {
    return g_btluiState.secondPort;
}

void btl_putDispEnvNoUi(void) {
    if (g_btluiState.started == 0) {
        GameData *gd = g_gameState->gameData;
        if (!(gd->ctx.battle->flags & BTL_FLAG_NO_DISPENV)) {
            PutDispEnv(&g_gameState->dispEnv[gd->drawBuf]);
        }
    }
}

u8 btlui_isPauseRequested(void) {
    return g_btluiState.started ? g_btluiState.pauseReq : 0;
}

void btlui_setPauseRequest(s8 req) {
    g_btluiState.pauseReq = req;
}

void btlui_resumeFromPause(void) {
    u8 v = g_gameState->gameData->drawBuf;
    g_btluiState.skipFrames = 2;
    g_drawBufs.cur = v;
}

void btlui_updateTargetFlags(void) {
    GameState *g = g_gameState;
    s32 r = 0;
    volatile Unk80104E68 *p = &g_btluiState;
    s32 f;
    if (((g->gameData->ctx.battle->cmdFlags >> BTL_CMDF_MARKERS_BIT) ^ 1) & 1) {
        r = 1;
    } else {
        do {
            f = (g->config >> CFG_BIT_TARGET_WINDOW) & 1;
            if (f == 1) {
                r = 1;
            }
        } while (0);
    }
    p->targetWindow = r;
    g_btluiState.multiTarget = btlui_countTargeted() >= 2;
}

void btlui_input(void) {
    if (g_btluiState.atbOn != 0) {
        btlui_helpInput();
        btlui_menuInput();
    }
}

void btlui_drawWindows(void) {
    if (g_btluiState.hideWindows == 0) {
        btlui_drawHelpWindow();
        btlui_drawPartyPanel();
        win_drawAll();
        btlui_drawCached();
    }
    btlui_drawTargetCursors();
}

void btlui_update(void) {
    btlui_updateMenu();
    btlui_updatePanelAnims();
    win_updateAll();
    btlui_updateHelpSlide();
}

void btlui_cacheFrame(void) {
    if (g_btluiState.frameReady != 0) {
        btlui_cacheRows();
        btlui_cacheCmdIcons();
        btlui_cacheHeader();
    }
}

void btlui_drawCached(void) {
    btlui_drawCachedRows();
    btlui_drawCachedCmdIcons();
    btlui_drawCachedHeader();
}

void btlui_onVsync(void) {
}

void btlui_applyEnable(void) {
    if (g_btluiState.enableReq != 0) {
        g_btluiState.enableReq = 0;
        g_btluiState.enabled = 1;
        if (g_btluiState.started != 0 && g_btluiState.atbOn != 1) {
            g_btluiState.atbOn = 1;
            btlui_flushMenuQueue();
        }
        btlui_updatePanel();
        btlui_buildItemLists();
    }
}

void btlui_putDispEnvIfReady(void) {
    if (g_btluiState.frameReady != 0 && g_btluiState.started != 0) {
        GameData *gd = g_gameState->gameData;
        if (!(gd->ctx.battle->flags & BTL_FLAG_NO_DISPENV)) {
            PutDispEnv(&g_gameState->dispEnv[gd->drawBuf]);
        }
    }
}

void btlui_initPartyEntries(void) {
    BtlUnit *u = g_gameState->gameData->ctx.battle->unitList;
    Unk80104F88Entry *e = g_btlUi->entries;
    s8 n = 0;
    s8 f;
    while (u != NULL && (u->flags & BTL_UNIT_PARTY)) {
        u->menu = e;
        u->inMenuQueue = 0;
        f = (u8)u->flags >> BTL_UNIT_SLOT_SHIFT;
        e->unit = u;
        e->slot = f;
        e->chr = &g_gameState->gameData->characters[BTL_UNIT_INDEX(u->flags)];
        e->learns = chr_canLearnAbilities(e->chr);
        e->defaultSet = chr_getAppearanceType(e->chr->info.b.id, (e->chr->classFlags >> 4) & 1);
        if ((g_gameState->config >> CFG_BIT_BTL_CONTROL) & 1) {
            f = ((s32)(g_gameState->config >> CFG_SHIFT_CHAR_PORT) >> e->defaultSet) & 1;
        } else {
            f = 0;
        }
        e->port = f;
        btlui_buildAbilityList(e);
        btlui_clearCursor(&e->cursor);
        n++;
        e++;
        u = u->next;
    }
    g_btlUi->partyCount = n;
}

void btlui_copyTranceAbilities(Unk80104F88Entry *entry) {
    s32 row = entry->chr->info.b.row;
    u8 *tranceCmds = g_btlCharTranceCmds[row];
    u8 *cmds = g_btlCharCmds[row];
    Unk800FD56C *normalCmd;
    Unk800FD56C *tranceCmd;
    s32 i;
    s32 j;
    u8 from;
    u8 to;
    if ((chr_canLearnAbilities(entry->chr) & 0xFF) && (tranceCmds[2] == 1 || tranceCmds[2] == 2)) {
        i = 0;
        j = tranceCmds[2] - 1;
        normalCmd = &g_btlCmdTable[cmds[j]];
        tranceCmd = &g_btlCmdTable[tranceCmds[j]];
        for (; i < normalCmd->count; i++) {
            from = g_btlCmdAbilities[normalCmd->first + i];
            to = g_btlCmdAbilities[tranceCmd->first + i];
            if (from != to) {
                entry->apCounters[to] = entry->apCounters[from];
                entry->apNeeded[to] = entry->apNeeded[from];
                entry->fromEquip[to] = entry->fromEquip[from];
            }
        }
    }
}

/* character 3: also list the abilities of character record 1 and its equipment */
void btlui_linkSwdMagAbilities(Unk80104F88Entry *entry) {
    Unk800FD56C *cmd = &g_btlCmdTable[BTL_CMD_SWD_MAG];
    Character *vivi = &g_gameState->gameData->characters[CHR_VIVI];
    AbilityEntry *rows = g_abilityRows[1];
    s32 i;
    s32 j;
    s32 k;
    s32 slot;
    s32 apSlot;
    s32 id;
    s32 itemId;
    ItemInfo *it;

    if (entry->chr->info.b.id == CHR_STEINER) {
        for (i = 0; i < cmd->count; i++) {
            slot = g_btlCmdAbilities[cmd->first + i];
            apSlot = chr_findAbilitySlot(CHR_VIVI, g_btlSwdMagSourceAbilities[i]);
            if (apSlot >= 0) {
                entry->apCounters[slot] = &vivi->ap[apSlot];
                entry->apNeeded[slot] = rows[apSlot].ap;
            }
        }
        for (i = 0; i < 5; i++) {
            itemId = vivi->equip[i];
            if (itemId != ITEM_NONE) {
                it = &g_itemTable[itemId];
                for (j = 0; j < 3; j++) {
                    id = it->abilities[j];
                    if (id != 0 && id < ABILITY_SUPPORT_FIRST) {
                        for (k = 0; k < cmd->count; k++) {
                            if (id == g_btlSwdMagSourceAbilities[k]) {
                                slot = g_btlCmdAbilities[cmd->first + k];
                                entry->fromEquip[slot] = 1;
                            }
                        }
                    }
                }
            }
        }
    }
}

void btl_updateAtb(void) {
    BtlUnit *u;
    BtlUnit28 *s;
    BtlUnit24 *m;

    u = g_gameState->gameData->ctx.battle->unitList;
    if (btl_isAtbRunning() == 0) {
        return;
    }
    for (; u != NULL; u = u->next) {
        if (u->cmdIssued != 0 || u->inMenuQueue != 0 || u->hp == 0) {
            continue;
        }
        if (!((u->flags >> BTL_UNIT_ATB_ACTIVE_BIT) & 1)) {
            continue;
        }
        if ((u->flags & BTL_UNIT_PARTY) == g_btlUi->frozenSide) {
            continue;
        }
        s = (BtlUnit28 *)&u->hp;
        m = u->max;
        s->atb += s->atbSpeed;
        if (s->atb < m->maxAtb) {
            continue;
        }
        s->atb = m->maxAtb;
        if ((u8)btl_hasStatus(u, (BTL_STATUS_VENOM | BTL_STATUS_SLEEP | BTL_STATUS_FREEZE)) != 0) {
            continue;
        }
        if (u->flags & BTL_UNIT_PARTY) {
            if ((u8)btl_hasStatus(u, (BTL_STATUS_CONFUSE | BTL_STATUS_BERSERK)) != 0) {
                u->cmdIssued = 1;
                btl_autoAttack(u);
            } else {
                btlui_queueMenuUnit(u);
            }
        } else if (evt_battleTrigger(0x2F, u->unitBit, 0, 0) != 0) {
            u->cmdIssued = 1;
        }
    }
}

void btlui_checkSwdMagPartner(void) {
    BtlUnit *u;
    s32 slot;

    u = g_gameState->gameData->ctx.battle->unitList;
    g_btlUi->swdMagPartner = 0;
    g_btlUi->swdMagBlocked.b[0] = 0;
    g_btlUi->swdMagBlocked.b[1] = 0;
    for (; u != NULL; u = u->next) {
        if (!(u->flags & BTL_UNIT_PARTY)) {
            break;
        }
        slot = BTL_UNIT_INDEX(u->flags);
        if (slot == 1) {
            g_btlUi->swdMagPartner = 1;
            if (u->hp == 0 || (u8)btl_hasStatus(u, (BTL_STATUS_PETRIFY | BTL_STATUS_VENOM | BTL_STATUS_SILENCE | BTL_STATUS_KO | BTL_STATUS_CONFUSE | BTL_STATUS_BERSERK | BTL_STATUS_STOP | BTL_STATUS_SLEEP | BTL_STATUS_HEAT | BTL_STATUS_FREEZE | BTL_STATUS_MINI)) != 0) {
                g_btlUi->swdMagBlocked.b[0] = 1;
            }
        } else if (slot == 3 && (u8)btl_hasStatus(u, BTL_STATUS_MINI) != 0) {
            g_btlUi->swdMagBlocked.b[1] = 1;
        }
    }
}

BtlUnit *btlui_popMenuUnit(void) {
    BtlUnit *u;
    u8 ok;
    for (;;) {
        if (g_btlUi->queueCount == 0 || g_btlUi->menuState != BTLUI_MENU_NONE) {
            return NULL;
        }
        u = g_btlUi->queue[g_btlUi->queueHead];
        g_btlUi->queueCount--;
        g_btlUi->queueHead = (g_btlUi->queueHead + 1) & 3;
        if (u == NULL || u->hp == 0 || (btl_hasStatus(u, (BTL_STATUS_PETRIFY | BTL_STATUS_VENOM | BTL_STATUS_KO | BTL_STATUS_CONFUSE | BTL_STATUS_BERSERK | BTL_STATUS_STOP | BTL_STATUS_SLEEP | BTL_STATUS_FREEZE | BTL_STATUS_JUMP)) & 0xFF) || !((g_btlRewards.expMask >> ((u8)u->flags >> BTL_UNIT_SLOT_SHIFT)) & 1)) {
            ok = 0;
        } else {
            ok = 1;
        }
        if (ok) {
            return u;
        }
        u->inMenuQueue = 0;
    }
}

void btlui_pruneMenuQueue(void) {
    BtlUnit *list[4];
    u8 seen[4];
    s32 n = 0;
    s32 head = g_btlUi->queueHead;
    s32 cnt = g_btlUi->queueCount;
    BtlUnit *u;
    s32 i;
    Unk80104F88 *q;
    BtlUnit *v;
    Unk80104F88 *r;
    memset(seen, 0, 4);
    while (cnt != 0) {
        u = g_btlUi->queue[head];
        if (BtlUnitCanAct(u) && u->cmdIssued != 1) {
            list[n++] = u;
        } else {
            u->inMenuQueue = 0;
        }
        cnt--;
        head = (head + 1) & 3;
    }
    q = g_btlUi;
    if (n != q->queueCount) {
        q->unk12C6 = 0;
        q->queueHead = 0;
        q->queueCount = 0;
        for (i = 0; i < n; i++) {
            if (seen[(u8)list[i]->flags >> BTL_UNIT_SLOT_SHIFT] == 0) {
                seen[(u8)list[i]->flags >> BTL_UNIT_SLOT_SHIFT] = 1;
                v = list[i];
                r = g_btlUi;
                v->inMenuQueue = 1;
                g_btlUi->queue[(r->queueHead + r->queueCount) & 3] = v;
                r->queueCount++;
            }
        }
    }
}

void btlui_initMenu(void) {
    s32 i;
    Unk80104F88 *p = btlui_alloc(0x12D8);
    g_btlUi = p;
    p->menuState = BTLUI_MENU_NONE;
    p->cmdUnit = NULL;
    g_btlUi->tick = 0;
    g_btlUi->ambush = g_gameState->gameData->ctx.battle->scene.flags & BTL_SCENE_AMBUSH;
    g_btlUi->frozenSide = btlui_getFrozenSide();
    btlui_clearAbilityLists();
    btlui_initPartyEntries();
    btlui_clearMenuQueue();
    for (i = 0; i < 4; i++) {
        g_btlUi->windows[i] = win_get(i);
    }
}

void btlui_menuInput(void) {
    u8 st;
    btlui_pruneMenuQueue();
    st = g_btlUi->menuState;
    if (st == BTLUI_MENU_CLOSING) {
        return;
    }
    if (st == BTLUI_MENU_NONE) {
        BtlUnit *u = btlui_popMenuUnit();
        g_btlUi->cmdUnit = u;
        if (u != NULL) {
            btlui_openCmdMenu();
        }
    } else if (!(btlui_canOpenMenu(g_btlUi->cmdUnit) & 0xFF)) {
        btlui_closeMenu();
    } else {
        g_btluiMenuInput[g_btlUi->menuState]();
    }
}

void btlui_updateMenu(void) {
    void (*fn)(void);
    btlui_applyTargetable();
    btlui_pollEscape();
    btl_updateAtb();
    btlui_checkSwdMagPartner();
    fn = g_btluiMenuUpdate[g_btlUi->menuState];
    if (fn != NULL) {
        fn();
    }
}

void btlui_updateAtbFreeze(void) {
    if (g_btlUi->tick < 75) {
        g_btlUi->frozenSide = btlui_getFrozenSide();
    } else {
        g_btlUi->frozenSide = 2;
    }
    g_btlUi->tick++;
}

s32 btlui_getMenuState(void) {
    return g_btlUi->menuState;
}

s32 btlui_setMenuState(s32 state) {
    s32 old = g_btlUi->menuState;
    if (state != BTLUI_MENU_KEEP) {
        g_btlUi->menuState = state;
    }
    return old;
}

void btlui_clearMenuQueue(void) {
    Unk80104F88 *p = g_btlUi;
    p->unk12C6 = 0;
    p->queueHead = 0;
    p->queueCount = 0;
}

void btlui_flushMenuQueue(void) {
    Unk80104F88 *p;
    s32 n = g_btlUi->queueCount;
    s32 i = g_btlUi->queueHead;
    while (n != 0) {
        n--;
        g_btlUi->queue[i]->inMenuQueue = 0;
        i = (i + 1) & 3;
    }
    p = g_btlUi;
    p->unk12C6 = 0;
    p->queueHead = 0;
    p->queueCount = 0;
}

void btlui_queueMenuUnit(BtlUnit *unit) {
    Unk80104F88 *ui = g_btlUi;
    unit->inMenuQueue = 1;
    g_btlUi->queue[(ui->queueHead + ui->queueCount) & 3] = unit;
    ui->queueCount++;
}

void btlui_resetMenuQueue(void) {
    BtlUnit *u = g_gameState->gameData->ctx.battle->unitList;
    btlui_clearMenuQueue();
    while (u != NULL) {
        if (u->flags & BTL_UNIT_PARTY) {
            u->inMenuQueue = 0;
        }
        u = u->next;
    }
}

void btlui_closeMenu(void) {
    BtlUnit *u = g_btlUi->cmdUnit;
    if (u != NULL) {
        u->inMenuQueue = 0;
    }
    if (g_btlUi->menuState != BTLUI_MENU_NONE) {
        g_btlUi->menuState = BTLUI_MENU_CLOSING;
        btlui_closeWindows();
    }
    btlui_clearTargeted();
}

void btlui_closeWindows(void) {
    s32 i;
    if (g_btlUi->menuState != BTLUI_MENU_NONE) {
        if (!btlui_isListWindowOpen()) {
            win_setOnClosed(g_btlUi->windows[3], btlui_onCmdWindowClosed);
            win_closeAll();
        } else {
            win_closeAll();
            btlui_closeListWindow(0);
        }
        for (i = 0; i < 2; i++) {
            pad_setRepeatEnabled(i, 2, 0);
            pad_setRepeatEnabled(i, 3, 0);
        }
    }
}

void btlui_saveCursor(Unk800EB518 *saved, s32 level, s32 row, s32 pos) {
    if (level == 0) {
        saved->cmdRow = row;
        return;
    }
    saved->listTop[saved->cmdRow] = row;
    saved->listCursor[saved->cmdRow] = pos;
}

void btlui_clearCursor(Unk800EB518 *saved) {
    s32 i;
    saved->cmdRow = 0;
    for (i = 0; i < 4; i++) {
        saved->listTop[i] = 0;
        saved->listCursor[i] = 0;
    }
}

void btlui_perfMark(s32 mark) {
}

u8 *btlui_getQueueCountPtr(void) {
    return &g_btlUi->queueCount;
}

BtlUnit *btlui_getQueuedUnit(s32 index) {
    return g_btlUi->queue[index];
}

void btlui_clearAbilityLists(void) {
    s32 i, j;
    for (i = 0; i < 4; i++) {
        Unk80104F88Entry *e = &g_btlUi->entries[i];
        for (j = 0; j < ABILITY_SUPPORT_FIRST; j++) {
            e->apCounters[j] = 0;
            e->apNeeded[j] = 0;
            e->fromEquip[j] = 0;
        }
    }
}

void btlui_buildAbilityList(Unk80104F88Entry *entry) {
    btlui_addLearnedAbilities(entry);
    btlui_addEquipAbilities(entry);
    btlui_copyTranceAbilities(entry);
    btlui_linkSwdMagAbilities(entry);
}

void btlui_addLearnedAbilities(Unk80104F88Entry *entry) {
    Character *chr;
    s32 i;
    AbilityEntry *row;
    s32 slot;
    chr = entry->chr;
    if (entry->learns != 0) {
        i = 0;
        row = g_abilityRows[chr->info.b.row];
        for (; i < 0xC0; i++) {
            slot = chr_findAbilitySlot(chr->info.b.id, i);
            if (slot >= 0) {
                entry->apCounters[i] = &chr->ap[slot];
                entry->apNeeded[i] = row[slot].ap;
            }
        }
    }
}

void btlui_addEquipAbilities(Unk80104F88Entry *entry) {
    Character *chr = entry->chr;
    s32 i;
    s32 j;
    memset((s32)entry->fromEquip, 0, 0xC0);
    for (i = 0; i < 5; i++) {
        u8 item = chr->equip[i];
        if (item != 0xFF) {
            ItemInfo *info = &g_itemTable[item];
            for (j = 0; j < 3; j++) {
                s32 ability = info->abilities[j];
                if (ability != 0 && ability < 0xC0) {
                    entry->fromEquip[ability] = 1;
                }
            }
        }
    }
}

s32 btlui_canOpenMenu(BtlUnit *unit) {
    if (unit == NULL) {
        return 0;
    }
    if (unit->hp != 0 && !(btl_hasStatus(unit, (BTL_STATUS_PETRIFY | BTL_STATUS_VENOM | BTL_STATUS_KO | BTL_STATUS_CONFUSE | BTL_STATUS_BERSERK | BTL_STATUS_STOP | BTL_STATUS_SLEEP | BTL_STATUS_FREEZE | BTL_STATUS_JUMP)) & 0xFF)) {
        return (g_btlRewards.expMask >> ((u8)unit->flags >> BTL_UNIT_SLOT_SHIFT)) & 1;
    }
    return 0;
}

void btlui_applyTargetable(void) {
    BtlUnit *u;
    u8 c;
    for (u = g_gameState->gameData->ctx.battle->unitList; u != 0; u = u->next) {
        c = u->targetableReq;
        if (c >= BTL_TGTREQ_SET_OFF) {
            if (c == BTL_TGTREQ_SET_OFF) {
                u->targetableReq = BTL_TGTREQ_OFF;
                u->flags &= ~BTL_UNIT_ATB_ACTIVE;
                u->flags &= ~BTL_UNIT_TARGETABLE;
                if (g_btlUi->menuState == BTLUI_MENU_TARGET) {
                    btlui_onUnitUntargetable(u);
                }
            } else if (c == BTL_TGTREQ_SET_ON) {
                u->targetableReq = BTL_TGTREQ_ON;
                u->flags |= BTL_UNIT_ATB_ACTIVE;
                u->flags |= BTL_UNIT_TARGETABLE;
                if (g_btlUi->menuState == BTLUI_MENU_TARGET) {
                    btlui_onUnitTargetable(u);
                }
            }
        }
    }
}

void btlui_pollEscape(void) {
    Unk80104E68 *p = (Unk80104E68 *)&g_btluiState;
    if (p->atbOn != 0) {
        if ((g_padHeld & (BTN_L1 | BTN_R1)) == 0x300000) {
            g_gameState->gameData->ctx.battle->escapeHeld = 1;
            if (p->escapeFrames++ >= 0x3D) {
                p->escapeFrames = 0;
                btl_tryEscape(1);
            } else {
                btl_tryEscape(0);
            }
        } else {
            g_gameState->gameData->ctx.battle->escapeHeld = 0;
        }
    }
}

s32 btlui_getFrozenSide(void) {
    s32 v = g_btlUi->ambush;
    if (v != 0) {
        return (v != 1) * 2;
    }
    return 1;
}

void btlui_onCmdWindowClosed(Window *win) {
    g_btlUi->menuState = BTLUI_MENU_NONE;
}

void btlui_drawHelpWindow(void) {
    RECT r;
    s32 m;
    if (g_btluiHelp.open != 0 && g_btluiPrevAtbOn != 0) {
        btlui_getHelpRect(&g_btluiHelpRect, &r);
        win_setClip(NULL, 2, WIN_STYLE_FRAMELESS);
        if (g_btluiHelp.opening == 0) {
            ui_drawSprite(r.x + 8, r.y, 2, UI_SPRITE_HELP_TAG, 0);
        }
        btlui_updateHelpText();
        if (g_btluiHelp.text != 0) {
            win_setClip(NULL, 2, WIN_STYLE_BALLOON);
            m = text_getColor();
            text_drawBlock(r.x + 6, r.y + 10, 2, g_btluiHelp.text, TEXT_PAL_HELP);
            text_setColor(m);
            win_setClip(&r, 2, WIN_STYLE_BALLOON);
        }
        win_drawHelpFrame(&r, 2, HELP_TAIL_NONE, 0);
        win_setClip(&r, 2, WIN_STYLE_FRAMELESS);
    }
}

void btlui_initHelp(void) {
    g_btluiHelp.open = 0;
    g_btluiHelp.opening = 0;
    g_btluiHelp.unk2 = 0;
    g_btluiHelp.closeReq = 0;
    g_btluiHelp.text = 0;
    g_btluiHelp.frame = 0;
    g_btluiHelpText = btlui_alloc(0x100);
}

void btlui_helpInput(void) {
    if (g_padRepeat & 0x08000000) {
        btlui_toggleHelp(g_btluiHelp.open == 0);
    }
}

void btlui_updateHelpSlide(void) {
    if (g_btluiHelp.open != 0 && g_btluiPrevAtbOn != 0) {
        g_btluiHelp.frame++;
        if (g_btluiHelp.opening != 0 && g_btluiHelp.frame >= 7) {
            g_btluiHelp.opening = 0;
            btlui_showPanel(0);
        }
    }
}

void btlui_checkHelpClose(void) {
    if (btlui_getPanel()->rowCount == 0) {
        if (g_btluiHelp.open != 0) {
            g_btluiHelp.closeReq = 1;
        }
    }
    if (g_btluiHelp.closeReq != 0 && g_btluiHelp.open != 0) {
        g_btluiHelp.open = 0;
        g_btluiHelp.closeReq = 0;
        btlui_showPanel(1);
    }
}

u8 btlui_isHelpOpen(void) {
    return g_btluiHelp.open;
}

void btlui_toggleHelp(u8 open) {
    if (open) {
        if (g_btluiHelp.open == 0) {
            snd_playSe(SE_HELP_OPEN);
            g_btluiHelp.open = 1;
            g_btluiHelp.opening = 1;
            g_btluiHelp.frame = 0;
        }
    } else if (g_btluiHelp.open != 0 && g_btluiHelp.closeReq == 0) {
        snd_playSe(SE_CANCEL);
        g_btluiHelp.closeReq = 1;
    }
}

void btlui_closeHelp(void) {
    if (g_btluiHelp.open != 0 && g_btluiHelp.closeReq == 0) {
        g_btluiHelp.closeReq = 1;
    }
}

void btlui_updateHelpText(void) {
    s32 (*fn)(void);
    g_btluiHelp.text = 0;
    if (g_gameState->gameData->ctx.battle->escapeHeld != 0) {
        g_btluiHelp.text = (s32)text_get(4);
    } else {
        fn = g_btluiMenuHelp[g_btlUi->menuState];
        if (fn != NULL) {
            g_btluiHelp.text = fn();
        }
    }
}

void btlui_getHelpRect(RECT *base, RECT *out) {
    Window *w;
    s16 x;
    s32 slide;
    *out = *base;
    w = btlui_getTargetWindow();
    if (w != NULL && w->state != WIN_STATE_CLOSED) {
        if (out->x < w->cur.x + w->cur.w - 4) {
            x = w->cur.x + w->cur.w - 4;
            out->x = x;
            out->w = 0x137 - x;
        }
    }
    if (g_btluiHelp.opening != 0) {
        slide = (7 - g_btluiHelp.frame) * 24;
        out->x += slide;
        out->w -= slide;
    }
}

void btlui_removeItem(s32 itemId) {
    s32 i;
    if (item_remove(itemId, 1) != 0) {
        if (itemId >= ITEM_FIRST_USABLE) {
            i = btlui_findItemListEntry(itemId);
            if (i != 0xFF && g_btluiItems->items[i]->count == 0) {
                g_btluiItems->items[i] = 0;
                g_btluiItems->itemCount--;
            }
        }
        if (itemId < BTL_WEAPON_COUNT && (g_weaponTable[itemId].flags & BTL_WEAPON_THROWABLE)) {
            i = btlui_findThrowListEntry(itemId);
            if (i != 0xFF && g_btluiItems->throwItems[i]->count == 0) {
                g_btluiItems->throwItems[i] = 0;
                g_btluiItems->throwCount--;
            }
        }
    }
}

void btlui_initItemLists(void) {
    Unk80104FA8 *p = btlui_alloc(0x2E0);
    g_btluiItems = p;
    memset((s32)p->reserved, 0, 0x100);
    btlui_buildItemLists();
}

void btlui_buildItemLists(void) {
    btlui_buildItemList();
    btlui_buildThrowList();
}

void btlui_addItem(s32 itemId) {
    if (item_add(itemId, 1) == 0) {
        return;
    }
    if (itemId >= ITEM_FIRST_USABLE && btlui_findItemListEntry(itemId) == 0xFF) {
        btlui_addItemListEntry(itemId);
    }
    if (itemId < BTL_WEAPON_COUNT && (g_weaponTable[itemId].flags & BTL_WEAPON_THROWABLE) && btlui_findThrowListEntry(itemId) == 0xFF) {
        btlui_addThrowListEntry(itemId);
    }
}

UnkPair *btlui_getItemListEntry(s32 index) {
    return g_btluiItems->items[index];
}

UnkPair *btlui_getThrowListEntry(s32 index) {
    return g_btluiItems->throwItems[index];
}

void btlui_reserveItem(s32 itemId) {
    g_btluiItems->reserved[itemId]++;
}

void btlui_unreserveItem(s32 itemId) {
    g_btluiItems->reserved[itemId]--;
}

s32 btlui_getItemReserved(s32 itemId) {
    return g_btluiItems->reserved[itemId];
}

void btlui_buildItemList(void) {
    s32 n = 0;
    s32 i;
    UnkPair *p = g_gameState->gameData->items.slots;
    Unk80104FA8 *f = g_btluiItems;
    for (i = 0; i < ITEM_SLOT_COUNT; i++, p++) {
        if (p->count != 0 && p->id != ITEM_NONE && p->id >= ITEM_FIRST_USABLE) {
            f->items[n] = p;
            n++;
        }
    }
    for (i = n; i < 31; i++) {
        g_btluiItems->items[i] = NULL;
    }
    g_btluiItems->itemCount = n;
}

void btlui_buildThrowList(void) {
    s32 n = 0;
    s32 i = 0;
    UnkPair *e;
    Unk80104FA8 *p = g_btluiItems;
    e = g_gameState->gameData->items.slots;
    for (; i < ITEM_SLOT_COUNT; i++, e++) {
        if (e->count != 0 && e->id != ITEM_NONE && e->id < BTL_WEAPON_COUNT && (g_weaponTable[e->id].flags & BTL_WEAPON_THROWABLE)) {
            p->throwItems[n] = e;
            n++;
        }
    }
    for (i = n; i < BTL_WEAPON_COUNT; i++) {
        g_btluiItems->throwItems[i] = NULL;
    }
    g_btluiItems->throwCount = n;
}

void btlui_addItemListEntry(s32 itemId) {
    s32 i;
    for (i = 0; i < 31; i++) {
        if (g_btluiItems->items[i] == NULL) {
            g_btluiItems->items[i] = item_findSlot(itemId);
            break;
        }
    }
    g_btluiItems->itemCount++;
}

void btlui_addThrowListEntry(s32 itemId) {
    s32 i;
    for (i = 0; i < 0x58; i++) {
        if (g_btluiItems->throwItems[i] == NULL) {
            g_btluiItems->throwItems[i] = item_findSlot(itemId);
            break;
        }
    }
    g_btluiItems->throwCount++;
}

s32 btlui_findItemListEntry(s32 itemId) {
    s32 i;
    Unk80104FA8 *p = g_btluiItems;
    for (i = 0; i < 31; i++) {
        if (p->items[i] != NULL && itemId == p->items[i]->id) {
            return i;
        }
    }
    return 0xFF;
}

s32 btlui_findThrowListEntry(s32 itemId) {
    s32 i;
    Unk80104FA8 *p = g_btluiItems;
    for (i = 0; i < 0x58; i++) {
        if (p->throwItems[i] != NULL && itemId == p->throwItems[i]->id) {
            return i;
        }
    }
    return 0xFF;
}

void btlui_openCmdMenu(void) {
    Unk80104F88Entry *e;
    Window *w;
    Unk80104FC0 *p;

    p = &g_btlCmdMenu.sideCmd;
    e = g_btlUi->cmdUnit->menu;
    if (!((g_gameState->config >> CFG_BIT_CURSOR) & 1)) {
        g_btlCmdMenu.cursor = 0;
    } else {
        g_btlCmdMenu.cursor = e->cursor.cmdRow;
    }
    g_btlCmdMenu.trance = (u8)btl_hasStatus(g_btlUi->cmdUnit, BTL_STATUS_TRANCE) != 0;
    g_btlCmdMenu.charIndex = e->chr->info.b.row;
    g_btlCmdMenu.unit = g_btlUi->cmdUnit;
    p->side = 0;
    p->slide = 0;
    btlui_setMenuState(BTLUI_MENU_CLOSING);
    w = win_open(&g_btluiCmdWindowRect, 0xF, WIN_STYLE_BEVEL, 3);
    g_btlUi->windows[3] = w;
    win_setDrawContent(w, btlui_drawCmdList);
    win_setDrawOverlay(g_btlUi->windows[3], btlui_drawCmdOverlay);
    win_setOnOpened(g_btlUi->windows[3], btlui_onCmdMenuOpened);
    btlui_buildCmdList();
    snd_playSe(SE_WINDOW_OPEN);
}

void btlui_drawCmdList(Window *win) {
    s32 y = win->y + 6;
    s32 x = win->x + 0xA;
    s32 i;
    u8 *name;
    if (g_btlCmdMenu.sideCmd.side != 0) {
        btlui_drawSideCmd(REG_PASSTHROUGH(win));
    }
    for (i = 0; i < 4; i++) {
        name = g_btlCmdNames + g_btlCmdTable[g_btlCmdMenu.cmdIds[i]].nameOfs;
        if (g_btlCmdMenu.enabled[i] == 0) {
            text_draw(x, y, win->otz, name, TEXT_PAL_GREY);
        } else {
            text_draw(x, y, win->otz, name, TEXT_PAL_WHITE);
        }
        do {
            y += 15;
        } while (0);
    }
}

INCLUDE_RODATA("asm/us/ovl_0b/nonmatchings/btlui", g_battleText);

/* TRANCE label */
INCLUDE_RODATA("asm/us/ovl_0b/nonmatchings/btlui", s_btluiTranceLabel);

void btlui_drawCmdOverlay(Window *win) {
    u8 st = win->state;
    RECT *r = &win->cur;
    s32 dy = 10;
    RECT rc;
    if (win->state == WIN_STATE_OPEN) {
        if (btlui_getMenuState() == BTLUI_MENU_CMD || (btlui_getMenuState() == st && btlui_getTargetReturnState() == BTLUI_MENU_CMD && !(btlui_isTargetWindowOpen() & 0xFF) && (g_btluiVsyncTick & 1))) {
            ui_drawHand(win->cur.x - 0xE, r->y + (g_btlCmdMenu.cursor * 15 + dy), win->otz - 1);
        }
    }
    if (win->state != WIN_STATE_OPEN) {
        win_setClip(NULL, win->otz, WIN_STYLE_FRAMELESS);
    }
    if (g_btlCmdMenu.unit != NULL && !(btl_hasStatus(g_btlCmdMenu.unit, BTL_STATUS_TRANCE) & 0xFF)) {
        ui_drawLabel(win->x + 6, r->y - 3, win->otz, g_winLabelStrings.command, win->active ? UI_LABEL_NORMAL : UI_LABEL_DIM);
    } else if (win->active != 0) {
        btlui_drawTranceLabel(win->x + 6, r->y - 3, win->otz, s_btluiTranceLabel);
    } else {
        ui_drawLabel(win->x + 6, r->y - 3, win->otz, (s32)s_btluiTranceLabel, UI_LABEL_DIM);
    }
    if (win->state != WIN_STATE_OPEN) {
        rc.x = r->x;
        rc.y = r->y - 3;
        rc.w = r->w;
        rc.h = 8;
        win_setClip(&rc, win->otz, WIN_STYLE_FRAMELESS);
    }
}

void btlui_drawSideCmd(Window *win) {
    s32 dx2;
    RECT r;
    s32 off = 0;
    s32 dx;
    s16 y;
    s32 x;
    Unk80104FC0 *p = &g_btlSideCmd;
    Window *w = win;

    dx2 = 6;
    if (p->slide == 1) {
        off = (5 - p->slideFrame) * 3;
    } else if (p->slide == 2) {
        off = p->slideFrame * 5;
    }
    dx = 10;
    if (p->side == 2) {
        dx = dx2;
        x = off + 4;
        r.x = w->x + x;
    } else {
        r.x = w->x - off;
    }
    y = w->y + g_btlCmdMenu.cursor * 15;
    r.w = 0x3E;
    r.h = 0x11;
    r.y = y + 4;
    text_draw(r.x + dx, r.y + 2, win->otz, text_get(g_btluiSideCmdTexts[g_btlSideCmd.side]), TEXT_PAL_WHITE);
    btlui_drawSideFrame(&r, win->otz);
}

void btlui_confirmCmd(void) {
    Unk800FD56C *e = &g_btlCmdTable[g_btlCmdMenu.cmdIds[g_btlCmdMenu.cursor]];
    void (*f)(Unk800FD56C *);
    if (g_btlCmdMenu.sideCmd.side == 0) {
        if (g_btlCmdMenu.enabled[g_btlCmdMenu.cursor] != 0) {
            do {
                snd_playSe(SE_CURSOR);
                btlui_saveCursor(&g_btlCmdMenu.unit->menu->cursor, 0, g_btlCmdMenu.cursor, 0);
                f = g_btluiCmdConfirmFns[e->kind];
            } while (0);
        } else {
            snd_playSe(SE_BUZZER);
            return;
        }
    } else {
        if (g_btlCmdMenu.sideCmd.ready == 0) {
            return;
        }
        snd_playSe(SE_CURSOR);
        btlui_saveCursor(&g_btlCmdMenu.unit->menu->cursor, 0, g_btlCmdMenu.cursor, 0);
        f = g_btluiSideCmdConfirmFn;
    }
    f(e);
}

void btlui_buildCmdList(void) {
    s32 k = g_btlCmdMenu.charIndex;
    s8 f;
    g_btlCmdMenu.trance = f = (u8)btl_hasStatus(g_btlCmdMenu.unit, BTL_STATUS_TRANCE) != 0;
    g_btlCmdMenu.cmdIds[0] = BTL_CMD_ATTACK;
    g_btlCmdMenu.enabled[0] = 1;
    g_btlCmdMenu.cmdIds[3] = BTL_CMD_ITEM;
    g_btlCmdMenu.enabled[3] = 1;
    g_btlCmdMenu.tranceRow[3] = 0;
    g_btlCmdMenu.tranceRow[2] = 0;
    g_btlCmdMenu.tranceRow[1] = 0;
    g_btlCmdMenu.tranceRow[0] = 0;
    if (f == 0) {
        g_btlCmdMenu.cmdIds[1] = g_btlCharCmds[k][0];
        g_btlCmdMenu.cmdIds[2] = g_btlCharCmds[k][1];
    } else {
        g_btlCmdMenu.cmdIds[1] = g_btlCharTranceCmds[k][0];
        g_btlCmdMenu.cmdIds[2] = g_btlCharTranceCmds[k][1];
        if (g_btlCharTranceCmds[k][2] != 0xFF) {
            g_btlCmdMenu.tranceRow[g_btlCharTranceCmds[k][2]] = 1;
        }
    }
    g_btlCmdMenu.enabled[1] = 1;
    g_btlCmdMenu.enabled[2] = 1;
    if (g_btlCmdMenu.cmdIds[2] == BTL_CMD_SWD_MAG) {
        if (g_btlUi->swdMagPartner == 0) {
            g_btlCmdMenu.cmdIds[2] = BTL_CMD_NONE;
            if (g_btlCmdMenu.cursor == 2) {
                g_btlCmdMenu.cursor = 0;
            }
        } else if (g_btlUi->swdMagBlocked.h != 0) {
            g_btlCmdMenu.enabled[2] = 0;
        }
    }
}

void btlui_initCachedCmdIcons(s32 bufIndex) {
    s32 i;
    s32 u = VRAM_BTLUI_CACHE_X;
    s32 y = 0x9A;

    for (i = 0; i < 4; i++) {
        setSprt(&g_btluiPrims->cmdIcons[i][bufIndex]);
        setRGB0(&g_btluiPrims->cmdIcons[i][bufIndex], 0x80, 0x80, 0x80);
        g_btluiPrims->cmdIcons[i][bufIndex].x0 = 0;
        g_btluiPrims->cmdIcons[i][bufIndex].y0 = y;
        g_btluiPrims->cmdIcons[i][bufIndex].u0 = u & 0x3F;
        g_btluiPrims->cmdIcons[i][bufIndex].v0 = 0xE0;
        g_btluiPrims->cmdIcons[i][bufIndex].w = 0xE;
        g_btluiPrims->cmdIcons[i][bufIndex].h = 0x10;
        g_btluiPrims->cmdIcons[i][bufIndex].clut = 0;
        u += 0x10;
        y += 0x10;
    }
    setDrawTPage(&g_btluiPrims->cmdIconsTpage[bufIndex], 1, 1, getTPage(2, 0, VRAM_BTLUI_CACHE_X, VRAM_BTLUI_CACHE_Y));
}

void btlui_cacheCmdIcons(void) {
    RECT r = s_btluiCmdIconsSrc;
    s32 x = VRAM_BTLUI_CACHE_X;
    s32 y = 0x1E0;
    s32 i = 0;
    r.x += g_btluiDispOrigin[g_gameState->gameData->drawBuf & 1][0];
    r.y += g_btluiDispOrigin[g_gameState->gameData->drawBuf & 1][1];
    do {
        SetDrawMove((DR_MOVE *)g_primPtr, &r, x, y);
        x += 0x10;
        i++;
        addPrim(&g_ot[19], g_primPtr);
        g_primPtr += sizeof(DR_MOVE);
        r.y += 0x10;
    } while (i < 4);
}

void btlui_drawCachedCmdIcons(void) {
    s32 i;
    if (g_btlUi->windows[3]->state == WIN_STATE_CLOSING || g_btlUi->windows[3]->state == WIN_STATE_OPEN) {
        for (i = 0; i < 4; i++) {
            addPrim(&g_ot[18], &g_btluiPrims->cmdIcons[i][g_drawBufs.cur]);
        }
        addPrim(&g_ot[18], &g_btluiPrims->cmdIconsTpage[g_drawBufs.cur]);
    }
}

void btlui_drawSideFrame(RECT *r, s32 otz) {
    u32 *ot;
    u32 clut;
    UiSprite *d2;
    UiSprite *d1;
    RECT tw;
    RECT r2;
    u32 w;

    ot = &g_ot[otz];
    clut = win_getPalClut(g_sideFrameSprites[0].clut) << 16;
    d2 = &g_sideFrameSprites[2];
    setlen(g_primPtr, PRIM_LEN(SPRT));
    d1 = &g_sideFrameSprites[1];
    ((SprtWords *)g_primPtr)->rgbc = GPU_RGBC(GPU_SPRT, GPU_RGB_NEUTRAL);
    ((SprtWords *)g_primPtr)->xy = r->x | (r->y << 16);
    w = g_sideFrameSprites[0].u;
    w |= g_sideFrameSprites[0].v << 8;
    ((SprtWords *)g_primPtr)->uvclut = w | clut;
    ((SprtWords *)g_primPtr)->wh = g_sideFrameSprites[0].w | (g_sideFrameSprites[0].h << 16);
    addPrim(ot, g_primPtr);
    g_primPtr += sizeof(SprtWords);

    setlen(g_primPtr, PRIM_LEN(SPRT));
    ((SprtWords *)g_primPtr)->rgbc = GPU_RGBC(GPU_SPRT, GPU_RGB_NEUTRAL);
    ((SprtWords *)g_primPtr)->xy = (r->x + r->w - d2->w) | (r->y << 16);
    ((SprtWords *)g_primPtr)->uvclut = g_sideFrameSprites[2].u | (d2->v << 8) | clut;
    ((SprtWords *)g_primPtr)->wh = d2->w | (d2->h << 16);
    addPrim(ot, g_primPtr);
    g_primPtr += sizeof(SprtWords);

    tw.y = 0;
    tw.h = 0xFF;
    tw.x = g_sideFrameSprites[1].u;
    tw.w = d1->w;
    SetTexWindow(g_primPtr, &g_btluiTitleTexWindow);
    addPrim(ot, g_primPtr);
    g_primPtr += 0xC;

    setlen(g_primPtr, PRIM_LEN(SPRT));
    ((SprtWords *)g_primPtr)->rgbc = GPU_RGBC(GPU_SPRT, GPU_RGB_NEUTRAL);
    ((SprtWords *)g_primPtr)->xy = (r->x + g_sideFrameSprites[0].w) | (r->y << 16);
    ((SprtWords *)g_primPtr)->uvclut = (d1->v << 8) | clut;
    ((SprtWords *)g_primPtr)->wh = (r->w - (g_sideFrameSprites[0].w + d2->w)) | (d1->h << 16);
    addPrim(ot, g_primPtr);
    g_primPtr += sizeof(SprtWords);

    setlen(g_primPtr, PRIM_LEN(DR_MODE));
    ((DrMode2Words *)g_primPtr)->code[0] = get_mode(1, 1, d1->tpage);
    ((DrMode2Words *)g_primPtr)->code[1] = get_tw(&tw);
    addPrim(ot, g_primPtr);
    g_primPtr += sizeof(DrMode2Words);

    r2.x = r->x + 1;
    r2.y = r->y + 1;
    r2.w = r->w - 3;
    r2.h = r->h - 3;
    win_drawFill(&r2, otz, 0, 0, CONFIG_WINDOW_COLOR());
}

void btlui_drawTranceLabel(s32 x, s32 y, s32 otz, char *str) {
    s32 w;
    u32 wh;
    u32 xy;
    u32 tp;
    u32 stp;

    w = ui_measureLabel(g_btluiHighlightLabel);
    wh = w | 0x70000;
    setlen(g_primPtr, PRIM_LEN(TILE));
    ((TileWords *)g_primPtr)->rgbc = GPU_CODE(GPU_TILE | GPU_ABE);
    xy = x | (y << 16);
    ((TileWords *)g_primPtr)->xy = xy;
    ((TileWords *)g_primPtr)->wh = wh;
    addPrim(&g_ot[otz], g_primPtr);
    g_primPtr += sizeof(TileWords);
    setlen(g_primPtr, PRIM_LEN(DR_TPAGE));
    tp = get_mode(1, 1, getTPage(0, 1, 0, 0));
    stp = GPU_STP(1, 0);
    ((DrTpageWords *)g_primPtr)->code = tp;
    addPrim(&g_ot[otz], g_primPtr);
    g_primPtr += sizeof(DrTpageWords);
    setlen(g_primPtr, PRIM_LEN(DR_MODE));
    ((DrMode2Words *)g_primPtr)->code[0] = stp;
    ((DrMode2Words *)g_primPtr)->code[1] = 0;
    addPrim(&g_ot[otz], g_primPtr);
    g_primPtr += sizeof(DrMode2Words);
    btlui_drawPulseGradient(x, y, otz, w, 7, g_btluiHighlightColors);
    setlen(g_primPtr, PRIM_LEN(DR_STP));
    ((DrMode2Words *)g_primPtr)->code[0] = GPU_STP(0, 1);
    ((DrMode2Words *)g_primPtr)->code[1] = 0;
    addPrim(&g_ot[otz], g_primPtr);
    g_primPtr += sizeof(DrMode2Words);
    ui_drawLabel(x, y, otz, (s32)str, UI_LABEL_MASK);
    setlen(g_primPtr, PRIM_LEN(DR_STP));
    ((DrMode2Words *)g_primPtr)->code[0] = GPU_STP(0, 0);
    ((DrMode2Words *)g_primPtr)->code[1] = 0;
    addPrim(&g_ot[otz], g_primPtr);
    g_primPtr += sizeof(DrMode2Words);
    setlen(g_primPtr, PRIM_LEN(TILE));
    ((TileWords *)g_primPtr)->rgbc = GPU_CODE(GPU_TILE | GPU_ABE);
    ((TileWords *)g_primPtr)->xy = xy;
    ((TileWords *)g_primPtr)->wh = wh;
    addPrim(&g_ot[otz], g_primPtr);
    g_primPtr += sizeof(TileWords);
    setlen(g_primPtr, PRIM_LEN(DR_TPAGE));
    ((DrTpageWords *)g_primPtr)->code = tp;
    addPrim(&g_ot[otz], g_primPtr);
    g_primPtr += sizeof(DrTpageWords);
    setlen(g_primPtr, PRIM_LEN(DR_MODE));
    ((DrMode2Words *)g_primPtr)->code[0] = stp;
    ((DrMode2Words *)g_primPtr)->code[1] = 0;
    addPrim(&g_ot[otz], g_primPtr);
    g_primPtr += sizeof(DrMode2Words);
    ui_drawLabel(x, y, otz, (s32)str, UI_LABEL_NORMAL);
}

void btlui_drawPulseGradient(s32 x, s32 y, s32 ot, s32 w, s32 h, CVECTOR *c) {
    CVECTOR c0;
    CVECTOR c1;
    CVECTOR *a;
    CVECTOR *b;
    CVECTOR *t;
    CVECTOR *u;
    s32 k;
    POLY_G4 *p;

    k = g_winLastVsync & 0x1F;
    if (k < 16) {
        if (k >= 8) {
            k = 15 - k;
        }
        a = &c[2];
        b = &c[3];
        u = t = &c[0];
    } else {
        k -= 16;
        if (k >= 8) {
            k = 15 - k;
        }
        a = &c[2];
        b = &c[3];
        u = t = &c[1];
    }
    c0.r = a->r + ((u->r - a->r) * k) / 7;
    c0.g = a->g + ((u->g - a->g) * k) / 7;
    c0.b = a->b + ((u->b - a->b) * k) / 7;
    c1.r = b->r + ((t->r - b->r) * k) / 7;
    c1.g = b->g + ((t->g - b->g) * k) / 7;
    c1.b = b->b + ((t->b - b->b) * k) / 7;
    p = (POLY_G4 *)g_primPtr;
    setPolyG4(p);
    setRGB0(p, c0.r, c0.g, c0.b);
    setRGB2(p, c0.r, c0.g, c0.b);
    setRGB1(p, c1.r, c1.g, c1.b);
    setRGB3(p, c1.r, c1.g, c1.b);
    setXYWH(p, x, y, w, h);
    addPrim(&g_ot[ot], p);
    g_primPtr += sizeof(POLY_G4);
}

void btlui_cmdInput(void) {
    if (g_padRepeat & 0x01000000) {
        btlui_passTurn();
    } else if (g_padRepeat & BTN_CROSS) {
        btlui_confirmCmd();
    } else if (g_padRepeat & PAD_DOWN) {
        btlui_cmdCursorDown();
    } else if (g_padRepeat & PAD_UP) {
        btlui_cmdCursorUp();
    } else {
        btlui_cmdSideInput();
    }
}

void btlui_updateCmdMenu(void) {
    btlui_buildCmdList();
}

void *btlui_getCmdHelpText(void) {
    s32 i;
    Unk800FD56C *e;
    if (g_btlCmdMenu.sideCmd.side != 0 && g_btlCmdMenu.sideCmd.ready != 0) {
        i = 7;
        if (g_btlCmdMenu.sideCmd.side == 1) {
            i = 4;
        }
    } else {
        i = g_btlCmdMenu.cmdIds[g_btlCmdMenu.cursor];
    }
    e = &g_btlCmdTable[i];
    text_copy(g_btluiHelpText, e->helpOfs + g_btlCmdHelp);
    text_append(g_btluiHelpText, text_get(0x19));
    return g_btluiHelpText;
}

void btlui_onCmdMenuOpened(void) {
    s32 v = g_btlCmdMenu.unit->menu->port;
    btlui_setMenuState(BTLUI_MENU_CMD);
    if (v != g_btluiState.port) {
        pad_resetRepeats(v);
    }
    g_btluiState.port = v;
}

void btlui_cmdCursorUp(void) {
    if (g_btlCmdMenu.sideCmd.side == 0) {
        do {
            snd_playSe(SE_CURSOR);
            g_btlCmdMenu.cursor = g_btlCmdMenu.cursor != 0 ? g_btlCmdMenu.cursor - 1 : 3;
        } while (g_btlCmdMenu.cmdIds[g_btlCmdMenu.cursor] == 0);
    }
}

void btlui_cmdCursorDown(void) {
    if (g_btlCmdMenu.sideCmd.side == 0) {
        do {
            snd_playSe(SE_CURSOR);
            g_btlCmdMenu.cursor = g_btlCmdMenu.cursor < 3 ? g_btlCmdMenu.cursor + 1 : 0;
        } while (g_btlCmdMenu.cmdIds[g_btlCmdMenu.cursor] == 0);
    }
}

void btlui_cmdSideInput(void) {
    if (g_padHeld & PAD_LEFT) {
        btlui_slideLeft();
    } else if (g_padHeld & PAD_RIGHT) {
        btlui_slideRight();
    } else {
        btlui_slideBack();
    }
}

void btlui_passTurn(void) {
    btlui_saveCursor(&g_btlCmdMenu.unit->menu->cursor, 0, g_btlCmdMenu.cursor, 0);
    btlui_setMenuState(BTLUI_MENU_CLOSING);
    win_setOnClosed(g_btlUi->windows[3], btlui_onPassClosed);
    win_close(g_btlUi->windows[3]);
}

void btlui_selectDirectCmd(Unk800EDE6C *cmdEntry) {
    u32 hidden;
    Unk800EF80C req;
    Battle16CC *ability;
    u8 cmdNo = g_btlCmdMenu.cmdIds[g_btlCmdMenu.cursor];
    ability = &g_gameState->gameData->ctx.battle->abilities[cmdEntry->ability];
    req.subNo = cmdEntry->ability;
    req.returnState = 1;
    req.cmdNo = cmdNo;
    req.trance = g_btlCmdMenu.trance;
    req.targetType = ability->targetType;
    req.defaultSide = ability->defaultSide;
    req.koFirst = ability->koFirst;
    req.listKind = ability->listKind;
    hidden = ability->allowKo;
    req.doubleCast = 0;
    req.allowKo = hidden;
    btlui_startTargetSelect(&req);
}

void btlui_openAbilityList(s32 cmdEntry) {
    Unk800EE37C req;
    req.cmdNo = g_btlCmdMenu.cmdIds[g_btlCmdMenu.cursor];
    req.cmd = cmdEntry;
    req.trance = g_btlCmdMenu.trance;
    req.label = g_winLabelStrings.abilityList;
    req.label2 = g_winLabelStrings.abilityCost;
    btlui_openList(&req);
}

void btlui_openItemList(s32 cmdEntry) {
    Unk800EE37C req;
    req.cmdNo = g_btlCmdMenu.cmdIds[g_btlCmdMenu.cursor];
    req.cmd = cmdEntry;
    req.trance = g_btlCmdMenu.trance;
    req.label = g_winLabelStrings.itemList;
    req.label2 = g_winLabelStrings.itemCount;
    btlui_openList(&req);
}

void btlui_openThrowList(s32 cmdEntry) {
    Unk800EE37C req;
    req.cmdNo = g_btlCmdMenu.cmdIds[g_btlCmdMenu.cursor];
    req.cmd = cmdEntry;
    req.trance = g_btlCmdMenu.trance;
    req.label = g_winLabelStrings.throwList;
    req.label2 = g_winLabelStrings.itemCount;
    btlui_openList(&req);
}

void btlui_issueSideCmd(void) {
    s32 v = g_btluiSideCmdIds[g_btlCmdMenu.sideCmd.side];
    g_btlCmdMenu.unit->inMenuQueue = 0;
    g_btlCmdMenu.unit->cmdIssued = 1;
    btl_setCmd((s32)g_btlCmdMenu.unit->cmd, v, g_btlCmdTable[v].first, g_btlCmdMenu.unit->unitBit, 0);
    btlui_setMenuState(BTLUI_MENU_CLOSING);
    win_setOnClosed(g_btlUi->windows[3], btlui_onSideCmdClosed);
    win_close(g_btlUi->windows[3]);
}

void btlui_onSideCmdClosed(Window *win) {
    btlui_setMenuState(BTLUI_MENU_NONE);
    g_btlCmdMenu.sideCmd.ready = 0;
}

void btlui_onPassClosed(Window *win) {
    btlui_setMenuState(BTLUI_MENU_NONE);
    g_btlCmdMenu.sideCmd.ready = 0;
    btlui_queueMenuUnit(g_btlCmdMenu.unit);
}

void btlui_slideLeft(void) {
    if (g_btlSideCmd.side == 2) {
        if (g_btlSideCmd.slide == 1) {
            if (++g_btlSideCmd.slideFrame >= 5) {
                g_btlSideCmd.ready = 1;
                g_btlSideCmd.slide = 0;
            }
        } else if (g_btlSideCmd.slide == 2 && (g_padRepeat & PAD_LEFT)) {
            snd_playSe(SE_PAGE);
            g_btlSideCmd.slide = 1;
            g_btlSideCmd.side = 2;
            g_btlSideCmd.ready = 0;
            g_btlSideCmd.slideFrame = 0;
        }
    } else if (g_btlSideCmd.side == 0) {
        if (g_padRepeat & PAD_LEFT) {
            snd_playSe(SE_PAGE);
            g_btlSideCmd.slide = 1;
            g_btlSideCmd.side = 2;
            g_btlSideCmd.ready = 0;
            g_btlSideCmd.slideFrame = 0;
        }
    } else {
        btlui_slideBack();
    }
}

void btlui_slideRight(void) {
    if (g_btlSideCmd.side == 1) {
        if (g_btlSideCmd.slide == 1) {
            if (++g_btlSideCmd.slideFrame >= 5) {
                g_btlSideCmd.ready = 1;
                g_btlSideCmd.slide = 0;
            }
        } else if (g_btlSideCmd.slide == 2 && (g_padRepeat & PAD_RIGHT)) {
            snd_playSe(SE_PAGE);
            g_btlSideCmd.slide = 1;
            g_btlSideCmd.side = 1;
            g_btlSideCmd.ready = 0;
            g_btlSideCmd.slideFrame = 0;
        }
    } else if (g_btlSideCmd.side == 0) {
        if (g_padRepeat & PAD_RIGHT) {
            snd_playSe(SE_PAGE);
            g_btlSideCmd.slide = 1;
            g_btlSideCmd.side = 1;
            g_btlSideCmd.ready = 0;
            g_btlSideCmd.slideFrame = 0;
        }
    } else {
        btlui_slideBack();
    }
}

void btlui_slideBack(void) {
    Unk80104FC0 *p = &g_btlCmdMenu.sideCmd;
    if (p->side != 0) {
        if (p->slide != 2) {
            p->slide = 2;
            p->slideFrame = 0;
        } else if (p->slideFrame++ >= 3) {
            p->side = 0;
            p->slideFrame = 0;
        }
        p->ready = 0;
    }
}

void btlui_openList(Unk800EE37C *req) {
    MenuList list;
    Unk80104F88Entry *entry;
    u8 n;
    u8 keepCursor;
    Unk800EB518 *saved;

    btlui_setMenuState(BTLUI_MENU_CLOSING);
    g_btluiList.unit = g_btlUi->cmdUnit;
    g_btluiList.cmdNo = req->cmdNo;
    g_btluiList.cmd = (struct Unk80104FD4 *)req->cmd;
    g_btluiList.label = req->label;
    g_btluiList.label2 = req->label2;
    g_btluiList.trance = req->trance;
    g_btluiList.doubleCast = 0;
    g_btluiList.label2Width = (req->label2 != 0) ? ui_measureLabel((u8 *)req->label2) : 0;
    g_btluiList.halfMp = chr_hasSupportAbility(BTL_UNIT_INDEX(g_btluiList.unit->flags), SA_HALF_MP);
    n = g_btluiList.cmd->count;
    menuList_init(&list, n, (n < 5) ? n : 4, 1, 0x44);
    keepCursor = (g_gameState->config >> CFG_BIT_CURSOR) & 1;
    if (keepCursor == 1) {
        entry = g_btluiList.unit->menu;
        saved = &entry->cursor;
        list.top = saved->listTop[entry->cursor.cmdRow];
        list.cursor = saved->listCursor[entry->cursor.cmdRow];
    }
    g_btlUi->windows[2] = win_open(&g_btluiSelectListRect, 8, WIN_STYLE_LINE_RULE, 2);
    win_setList(g_btlUi->windows[2], &list);
    win_setDrawContent(g_btlUi->windows[2], btlui_drawListContent);
    win_setDrawOverlay(g_btlUi->windows[2], btlui_drawListOverlay);
    win_setOnOpened(g_btlUi->windows[2], (void (*)(Window *))btlui_onListOpened);
}

void btlui_listInput(void) {
    MenuList *l = (MenuList *)&g_btlUi->windows[2]->list;
    Window *w;
    if (l->scroll == 0) {
        if (g_padRepeat & BTN_CIRCLE) {
            snd_playSe(SE_CANCEL);
            w = g_btlUi->windows[2];
            btlui_saveCursor(&g_btluiList.unit->menu->cursor, 1, w->list.top, w->list.cursor);
            if (g_btluiList.doubleCast == 0) {
                btlui_closeList();
            } else {
                g_btluiList.doubleCast--;
            }
        } else if (g_padRepeat & BTN_CROSS) {
            btlui_listConfirm();
        } else {
            menuList_up(l, g_padRepeat);
            menuList_down(l, g_padRepeat);
            if (g_gameState->gameData->ctx.battle->escapeHeld == 0) {
                menuList_pageUp(l, g_padRepeat);
                menuList_pageDown(l, g_padRepeat);
            }
        }
    }
}

s32 btlui_selectAbility(void) {
    Unk800EF80C s;
    MenuList *l = &g_btlUi->windows[2]->list;
    s32 n = l->cursor + l->top;
    u8 k = g_btlCmdAbilities[g_btluiList.cmd->first + n];
    s32 r = btlui_getAbilityState(k);
    if (r != 2) {
        return 0;
    }
    {
        s.returnState = r;
        s.cmdNo = g_btluiList.cmdNo;
        s.subNo = btlui_resolveAbility(k);
        s.targetType = g_gameState->gameData->ctx.battle->abilities[k].targetType;
        s.defaultSide = g_gameState->gameData->ctx.battle->abilities[k].defaultSide;
        s.koFirst = g_gameState->gameData->ctx.battle->abilities[k].koFirst;
        s.listKind = g_gameState->gameData->ctx.battle->abilities[k].listKind;
        s.allowKo = g_gameState->gameData->ctx.battle->abilities[k].allowKo;
        s.doubleCast = g_btluiList.doubleCast;
        s.trance = g_btluiList.trance;
        btlui_startTargetSelect(&s);
        return 1;
    }
}

s32 btlui_selectItem(void) {
    Unk800EF80C s;
    MenuList *l = &g_btlUi->windows[2]->list;
    UnkPair *p = btlui_getItemListEntry(l->cursor + l->top);
    u8 r = 0;
    Unk8006A774 *e = &g_usableItemTableById[p->id];
    s32 n;

    if (p == NULL) {
        return 0;
    }
    if ((n = p->count) - btlui_getItemReserved(p->id) >= 0 && n != btlui_getItemReserved(p->id)) {
        s.returnState = 2;
        s.cmdNo = g_btluiList.cmdNo;
        s.subNo = p->id;
        s.targetType = e->targetType;
        s.defaultSide = e->defaultSide;
        s.koFirst = e->koFirst;
        s.listKind = e->listKind;
        s.allowKo = e->allowKo;
        s.doubleCast = g_btluiList.doubleCast;
        s.trance = g_btluiList.trance;
        btlui_startTargetSelect(&s);
        r = 1;
    }
    return r;
}

s32 btlui_selectThrow(void) {
    Unk800EF80C s;
    MenuList *l = &g_btlUi->windows[2]->list;
    u8 r = 0;
    UnkPair *p = btlui_getThrowListEntry(l->cursor + l->top);
    Battle16CC *e = &g_gameState->gameData->ctx.battle->abilities[g_btluiList.cmd->first];
    s32 n;

    if (p == NULL) {
        return 0;
    }
    if ((n = p->count) - btlui_getItemReserved(p->id) >= 0 && n != btlui_getItemReserved(p->id)) {
        s.returnState = 2;
        s.cmdNo = g_btluiList.cmdNo;
        s.subNo = p->id;
        s.targetType = e->targetType;
        s.defaultSide = e->defaultSide;
        s.koFirst = e->koFirst;
        s.listKind = e->listKind;
        s.allowKo = e->allowKo;
        s.doubleCast = g_btluiList.doubleCast;
        s.trance = g_btluiList.trance;
        btlui_startTargetSelect(&s);
        r = 1;
    }
    return r;
}

s32 btlui_getAbilityState(s32 ability) {
    Unk80104F88Entry *entry = g_btluiList.unit->menu;
    Battle16CC *info = &g_gameState->gameData->ctx.battle->abilities[ability];

    if (entry->learns != 0 && entry->fromEquip[ability] == 0 && (s16)*entry->apCounters[ability] < (s16)entry->apNeeded[ability]) {
        return 0;
    }
    if (info->flags & BTL_ABIL_VOICE) {
        if ((u8)btl_hasStatus(g_btluiList.unit, BTL_STATUS_SILENCE)) {
            return 1;
        }
        if ((g_gameState->gameData->ctx.battle->scene.flags >> BTL_SCENE_NO_VOICE_BIT) & 1) {
            return 1;
        }
    }
    if (btl_getMpCost((BtlCmd8 *)info) > g_btluiList.unit->mp) {
        return 1;
    }
    return 2;
}

void btlui_drawListOverlay(Window *win) {
    RECT *r = &win->cur;
    s32 x;
    s32 y;
    s32 dy = 10;
    s32 dx = 0x74;
    if (g_btluiList.label != 0) {
        ui_drawLabel(win->cur.x + 6, r->y - 4, win->otz, g_btluiList.label, UI_LABEL_NORMAL);
    }
    if (g_btluiList.label2 != 0) {
        ui_drawLabel(win->cur.x - (g_btluiList.label2Width - dx), r->y - 4, win->otz, g_btluiList.label2, UI_LABEL_NORMAL);
    }
    if (btlui_getMenuState() == BTLUI_MENU_LIST || (btlui_getMenuState() == BTLUI_MENU_TARGET && btlui_getTargetReturnState() == BTLUI_MENU_LIST && !(btlui_isTargetWindowOpen() & 0xFF) && (g_btluiVsyncTick & 1))) {
        x = win->cur.x - 0x12;
        y = r->y + (win->list.cursor * 15 + dy);
        if (g_btluiList.cmdNo == BTL_CMD_DBL_BLK || g_btluiList.cmdNo == BTL_CMD_DBL_WHT) {
            ui_drawSprite(x + 7, y + 2, win->otz, g_btluiList.doubleCast + 0x5A, 0);
        }
        ui_drawHand(x, y, win->otz);
    }
}

void btlui_updateList(void) {
    if (g_btluiList.cmdNo == BTL_CMD_SWD_MAG) {
        if (g_btlUi->swdMagPartner == 0 || g_btlUi->swdMagBlocked.h != 0) {
            btlui_closeList();
            snd_playSe(SE_CANCEL);
        }
    }
    if (g_btluiList.trance == 0 && (u8)btl_hasStatus(g_btluiList.unit, BTL_STATUS_TRANCE)) {
        btlui_closeList();
        snd_playSe(SE_CANCEL);
    }
}

void btlui_nextDoubleCast(void) {
    g_btluiList.doubleCast++;
}

s32 btlui_getListHelpText(void) {
    s32 r = 0;
    s32 (*f)(void) = g_btluiListHelpFns[g_btluiList.cmd->kind];
    if (f != NULL) {
        r = f();
    }
    return r;
}

void btlui_listConfirm(void) {
    Window *w;
    if ((u8)g_btluiListConfirmFns[g_btluiList.cmd->kind]()) {
        snd_playSe(SE_CURSOR);
        w = g_btlUi->windows[2];
        btlui_saveCursor(&g_btluiList.unit->menu->cursor, 1, w->list.top, w->list.cursor);
    } else {
        snd_playSe(SE_BUZZER);
    }
}

void btlui_drawListContent(Window *win) {
    RECT r;
    MenuView view;
    RECT *rc = &win->cur;
    s32 i;
    menuList_getView((MenuList *)&win->list, &view);
    win_setClip(0, 7, WIN_STYLE_LINE_RULE);
    for (i = 0; i < view.rows; i++) {
        btlui_drawListRow(0, i, view.first + i, &view);
    }
    if (((MenuList *)&win->list)->scroll & MENULIST_SCROLL_PAGE) {
        menuList_getRowsRect((MenuList *)&win->list, rc, &r);
        win_setClip(&r, 7, WIN_STYLE_LINE_RULE);
        btlui_drawListScroll(win);
    }
    win_setClip(rc, 7, WIN_STYLE_LINE_RULE);
}

void btlui_drawListScroll(Window *win) {
    MenuView view;
    s32 i;
    menuList_getPageView((MenuList *)&win->list, &view);
    for (i = 0; i < view.rows; i++) {
        btlui_drawListRow(0, i, view.first + i, &view);
    }
}

void btlui_onListOpened(void) {
    btlui_setMenuState(BTLUI_MENU_LIST);
    g_btlUi->windows[3]->active = 0;
}

void btlui_closeList(void) {
    btlui_setMenuState(BTLUI_MENU_CLOSING);
    win_setOnClosed(g_btlUi->windows[2], btlui_onListClosed);
    win_close(g_btlUi->windows[2]);
}

void btlui_drawListRow(s32 unused, s32 row, s32 index, MenuView *view) {
    g_btluiListRowFns[g_btluiList.cmd->kind](unused, row, index, view);
}

void btlui_drawAbilityRow(s32 unused, s32 row, s32 index, MenuView *view) {
    u8 digits[8];
    u8 ability = g_btlCmdAbilities[g_btluiList.cmd->first + index];
    Window *w = g_btlUi->windows[2];
    RECT *r = &w->cur;
    s32 dy = w->h - r->h;
    s32 state = btlui_getAbilityState(ability);
    s16 x;
    s32 y;
    Battle16CC *info;
    s32 pal;

    if (state != 0) {
        x = r->x;
        info = &g_gameState->gameData->ctx.battle->abilities[ability];
        y = r->y + row * 15 + view->y - dy;
        pal = (state != 2) * TEXT_PAL_GREY;
        text_draw(x + 6, y, 7, g_btlAbilityNames + info->nameOfs, pal);
        if (info->mpCost != 0) {
            text_formatNumber(btl_getMpCost((BtlCmd8 *)info), digits, 3, TEXT_NUM_ASCII);
            ui_drawSmallDigits(x + 0x60, y + 4, 7, digits, pal);
        }
    }
}

void btlui_drawItemRow(s32 unused, s32 row, s32 index, Window *view) {
    u8 digits[8];
    s32 x5;
    UnkPair *slot = btlui_getItemListEntry(index);
    Window *w = g_btlUi->windows[2];
    RECT *r = &w->cur;
    s32 dy = w->h - r->h;
    s32 count;
    u8 id;
    s16 x;
    s32 y;
    ItemInfo *it;
    if (slot != NULL) {
        if (slot->count - btlui_getItemReserved(slot->id) >= 0) {
            count = slot->count - btlui_getItemReserved(slot->id);
        } else {
            count = 0;
        }
        if (count != 0) {
            id = slot->id;
            x = r->x;
            it = &g_itemTable[id];
            x5 = x + 5;
            y = r->y + row * 15 + view->y - dy;
            ui_drawItemIcon(x5, y, 7, id, 1);
            text_draw(x + 0x14, y, 7, g_itemNames + it->name, TEXT_PAL_WHITE);
            text_formatNumber(count, digits, 2, TEXT_NUM_ASCII);
            ui_drawSmallDigits(x + 0x67, y + 4, 7, digits, TEXT_PAL_WHITE);
        }
    }
}

void btlui_drawThrowRow(s32 unused, s32 row, s32 index, Window *view) {
    u8 digits[8];
    UnkPair *slot = btlui_getThrowListEntry(index);
    Window *w = g_btlUi->windows[2];
    RECT *r = &w->cur;
    s32 dy = w->h - r->h;
    s32 count;
    s32 x;
    s32 y;
    ItemInfo *it;
    if (slot != NULL) {
        if (slot->count - btlui_getItemReserved(slot->id) >= 0) {
            count = slot->count - btlui_getItemReserved(slot->id);
        } else {
            count = 0;
        }
        if (count != 0) {
            x = r->x;
            it = &g_itemTable[slot->id];
            y = r->y + row * 15 + view->y - dy;
            text_formatNumber(count, digits, 2, TEXT_NUM_ASCII);
            text_draw(x + 5, y, 7, g_itemNames + it->name, TEXT_PAL_WHITE);
            ui_drawSmallDigits(x + 0x67, y + 4, 7, digits, TEXT_PAL_WHITE);
        }
    }
}

s32 btlui_resolveAbility(s32 ability) {
    s32 t;
    if (ability == ABIL_CARBUNCLE) {
        switch (g_btluiList.unit->menu->chr->equip[4]) {
        case ITEM_EMERALD:
            ability = ABIL_CARBUNCLE_EMERALD;
            break;
        case ITEM_MOONSTONE:
            ability = ABIL_CARBUNCLE_MOONSTONE;
            break;
        case ITEM_DIAMOND:
            ability = ABIL_CARBUNCLE_DIAMOND;
            break;
        }
    } else if (ability == ABIL_FENRIR) {
        t = g_btluiList.unit->menu->chr->equip[4] == ITEM_MAIDEN_PRAYER;
        ability = t + ABIL_FENRIR; /* ABIL_FENRIR2 with ITEM_MAIDEN_PRAYER */
    }
    return ability;
}

s32 btl_getMpCost(BtlCmd8 *ability) {
    s32 cost = ability->mpCost;
    if (ability->flags2 & BTL_ABIL2_SUMMON) {
        if (g_evtGlobalVars.b[EVT_GB_SUMMON_MP_X4] != 0) {
            cost *= 4;
        }
    }
    if (g_btluiList.halfMp != 0) {
        cost >>= 1;
    }
    return cost;
}

char *btlui_getAbilityHelpText(void) {
    MenuList *l = &g_btlUi->windows[2]->list;
    s32 n = l->cursor + l->top;
    u8 k = g_btlCmdAbilities[g_btluiList.cmd->first + n];
    if (btlui_getAbilityState(k) == 0) {
        return NULL;
    }
    return g_btlAbilityHelp + g_btlAbilityHelpOfs[k].text;
}

u8 *btlui_getItemHelpText(void) {
    Unk16 *w = &g_btlUi->windows[2]->list;
    UnkPair *e = btlui_getItemListEntry(w->cursor + w->top);
    if (e != NULL) {
        if (e->count - btlui_getItemReserved(e->id) >= 0) {
            if (e->count != btlui_getItemReserved(e->id)) {
                return g_btlItemHelp + g_btlItemHelpOfs[e->id];
            }
            return NULL;
        }
    }
    return NULL;
}

u8 *btlui_getThrowHelpText(void) {
    Unk16 *w = &g_btlUi->windows[2]->list;
    UnkPair *e = btlui_getThrowListEntry(w->cursor + w->top);
    if (e != NULL) {
        if (e->count - btlui_getItemReserved(e->id) >= 0) {
            if (e->count != btlui_getItemReserved(e->id)) {
                return g_btlItemHelp + g_btlItemHelpOfs[e->id];
            }
            return NULL;
        }
    }
    return NULL;
}

void btlui_onListClosed(Window *win) {
    btlui_setMenuState(BTLUI_MENU_CMD);
    g_btlUi->windows[3]->active = 1;
}

void btlui_startTargetSelect(Unk800EF80C *req) {
    s32 i;

    g_btlTarget.unit = g_btlUi->cmdUnit;
    g_btlTarget.returnState = req->returnState;
    g_btlTarget.cmdNo = req->cmdNo;
    g_btlTarget.targetType = req->targetType;
    g_btlTarget.defaultSide = req->defaultSide;
    g_btlTarget.koFirst = req->koFirst;
    g_btlTarget.side = req->defaultSide;
    g_btlTarget.subNo = req->subNo;
    g_btlTarget.listKind = req->listKind;
    g_btlTarget.allowKo = req->allowKo;
    g_btlTarget.doubleCast = req->doubleCast;
    g_btlTarget.targetAll = g_btluiTargetAllByType[g_btlTarget.targetType];
    g_btlTarget.windowOpen = 0;
    g_btlTarget.halfMp = chr_hasSupportAbility(BTL_UNIT_INDEX(g_btlTarget.unit->flags), SA_HALF_MP);
    g_btlTarget.trance = req->trance;
    g_btlTarget.unk7 = (u32)(req->targetType - BTL_TGT_ALL_ANY) < 3;
    g_btlTarget.canSwitchSide = req->targetType == BTL_TGT_SINGLE_ANY || req->targetType == BTL_TGT_MULTI_ANY || req->targetType == 9 || req->targetType == BTL_TGT_ALL_ANY;
    g_btlTarget.canToggleAll = (u32)(req->targetType - BTL_TGT_MULTI_ANY) < 3;
    g_btluiCursors.partyCount = 0;
    g_btluiCursors.count = 0;
    btlui_clearTargeted();
    btlui_setDefaultTargets(1);
    if (g_btlTarget.listKind == 0) {
        if (g_btluiTargetWindow == 0) {
            btlui_setMenuState(BTLUI_MENU_TARGET);
        } else {
            btlui_openTargetWindow(3);
        }
    } else {
        if (g_btluiTargetWindow != 0) {
            btlui_openTargetWindow(5);
        }
        btlui_openListWindow(g_btlTarget.listKind);
    }
    for (i = 0; i < 2; i++) {
        pad_setRepeatEnabled(i, 2, 1);
        pad_setRepeatEnabled(i, 3, 1);
    }
}

void btlui_setDefaultTargets(s32 useDefault) {
    BtlUnit *u;
    if (g_btlTarget.targetType == BTL_TGT_EVERYONE) {
        for (u = g_gameState->gameData->ctx.battle->unitList; u != 0; u = u->next) {
            if (((u->flags >> BTL_UNIT_TARGETABLE_BIT) & 1) && (!(u->status & BTL_STATUS_KO) || g_btlTarget.allowKo != 0)) {
                u->flags |= BTL_UNIT_TARGETED;
            }
        }
    } else if (g_btlTarget.targetType == BTL_TGT_SELF) {
        g_btlTarget.unit->flags |= BTL_UNIT_TARGETED;
    } else if (useDefault & 0xFF) {
        btlui_targetDefault(g_btluiDefaultTargets[g_btlTarget.targetType][0], g_btluiDefaultTargets[g_btlTarget.targetType][1]);
    } else {
        btlui_targetFirst(g_btluiDefaultTargets[g_btlTarget.targetType][0], g_btluiDefaultTargets[g_btlTarget.targetType][1]);
    }
}

s32 btlui_cycleTarget(s32 forward) {
    BtlUnit *list[4];
    BtlUnit *p;
    BtlUnit *u;
    s32 cur = 0;
    s32 n;
    s32 k;
    if (g_btlTarget.targetAll != 0) {
        return 0;
    }
    if (g_btlTarget.targetType == BTL_TGT_SELF) {
        return 0;
    }
    u = BtlFindTargetCursor();
    if (u == NULL) {
        return 0;
    }
    memset((s32)list, 0, 0x10);
    n = 0;
    for (p = g_gameState->gameData->ctx.battle->unitList; p != NULL; p = p->next) {
        if (((p->flags >> BTL_UNIT_TARGETABLE_BIT) & 1) && (p->flags & BTL_UNIT_PARTY) == g_btlTarget.side && (!(p->status & BTL_STATUS_KO) || g_btlTarget.allowKo != 0)) {
            list[n] = p;
            if (p == u) {
                cur = n;
            }
            n++;
        }
    }
    if (n >= 2) {
        u->flags &= ~BTL_UNIT_TARGETED;
        if (!(forward & 0xFF)) {
            k = cur - 1;
            if (cur == 0) {
                k = n - 1;
            }
        } else {
            k = 0;
            if (cur < n - 1) {
                k = cur + 1;
            }
        }
        list[k]->flags |= BTL_UNIT_TARGETED;
        return 1;
    }
    return 0;
}

s32 btlui_switchTargetSide(u8 newSide) {
    BtlUnit *cur;
    BtlUnit *u;
    BtlUnit *t;
    BtlUnit *list[4];
    s32 n;
    s32 i;
    u8 side;
    s32 k;

    for (u = g_gameState->gameData->ctx.battle->unitList; u != NULL; u = u->next) {
        if ((u->flags & (BTL_UNIT_TARGETABLE | BTL_UNIT_TARGETED)) == (BTL_UNIT_TARGETABLE | BTL_UNIT_TARGETED)) {
            cur = u;
            goto found;
        }
    }
    cur = NULL;
found:
    /* cur stays NULL when no unit is both targetable and targeted (seen in d3_f1759's battle); the PS1 then reads
     * cur->flags, and clears BTL_UNIT_TARGETED in it below, through low RAM. Harmless on the PS1; a native port must
     * check for NULL (ff9-ds F38). */
    if (g_btlTarget.canSwitchSide != 0) {
        if (newSide == (k = cur->flags & BTL_UNIT_PARTY)) {
            return 0;
        }
        side = !k;
        g_btlTarget.side = side;
        memset(list, 0, sizeof(list));
        n = 0;
        for (t = g_gameState->gameData->ctx.battle->unitList; t != NULL; t = t->next) {
            if (((t->flags >> BTL_UNIT_TARGETABLE_BIT) & 1) && (t->flags & BTL_UNIT_PARTY) == side) {
                if (!(t->status & BTL_STATUS_KO) || g_btlTarget.allowKo) {
                    list[n++] = t;
                }
            }
        }
        if (n != 0) {
            cur->flags &= ~BTL_UNIT_TARGETED;
            if (g_btlTarget.targetAll == 0) {
                list[0]->flags |= BTL_UNIT_TARGETED;
            } else {
                btlui_clearTargeted();
                for (i = 0; i < n; i++) {
                    list[i]->flags |= BTL_UNIT_TARGETED;
                }
            }
            return 1;
        }
    }
    return 0;
}

void btlui_toggleTargetAll(void) {
    u8 old = g_btlTarget.targetAll;
    BtlUnit *u;
    BtlUnit *p;
    s32 n;
    if (g_btlTarget.canToggleAll != 0) {
        if (old != 0) {
            if ((g_btlTarget.prevTarget->flags >> BTL_UNIT_TARGETED_BIT) & 1) {
                u = g_btlTarget.prevTarget;
            } else {
                u = BtlFindTargetCursor();
            }
            btlui_clearTargeted();
            u->flags |= BTL_UNIT_TARGETED;
            g_btlTarget.targetAll = 0;
        } else {
            p = g_gameState->gameData->ctx.battle->unitList;
            u = BtlFindTargetCursor();
            n = 0;
            g_btlTarget.prevTarget = u;
            btlui_clearTargeted();
            for (; p != NULL; p = p->next) {
                if (((p->flags >> BTL_UNIT_TARGETABLE_BIT) & 1) && (p->flags & BTL_UNIT_PARTY) == (u->flags & BTL_UNIT_PARTY) && (!(p->status & BTL_STATUS_KO) || g_btlTarget.allowKo != 0)) {
                    p->flags |= BTL_UNIT_TARGETED;
                    n++;
                }
            }
            if (n != 0) {
                g_btlTarget.targetAll = 1;
            }
        }
        if (old != g_btlTarget.targetAll) {
            snd_playSe(SE_CURSOR);
        }
    }
}

s32 btlui_isCmdUsable(void) {
    Unk800FD56C *e = &g_btlCmdTable[g_btlTarget.cmdNo];
    Battle16CC *b;
    u8 cost;
    if (g_btlTarget.cmdNo == BTL_CMD_SWD_MAG && (g_btlUi->swdMagPartner == 0 || g_btlUi->swdMagBlocked.h != 0)) {
        return 0;
    }
    if (g_btlTarget.trance == 0 && (btl_hasStatus(g_btlTarget.unit, BTL_STATUS_TRANCE) & 0xFF)) {
        return 0;
    }
    if (e->kind == 1) {
        b = &g_gameState->gameData->ctx.battle->abilities[g_btlTarget.subNo];
        cost = b->mpCost;
        if (g_btlTarget.halfMp != 0) {
            cost >>= 1;
        }
        if (g_btlTarget.unit->mp < cost) {
            return 0;
        }
        if ((b->flags & BTL_ABIL_VOICE) && (btl_hasStatus(g_btlTarget.unit, BTL_STATUS_SILENCE) & 0xFF)) {
            return 0;
        }
    }
    if ((u32)(e->kind - 2) < 2 && item_getCount(g_btlTarget.subNo) - btlui_getItemReserved(g_btlTarget.subNo) <= 0) {
        return 0;
    }
    return 1;
}

s32 btlui_refreshTargets(void) {
    BtlUnit *u = BtlFindTargetCursor();
    BtlUnit *p;
    s32 n;
    if (u == NULL) {
        btlui_setDefaultTargets(0);
        return 1;
    }
    if (g_btlTarget.targetAll == 0) {
        if ((u->status & BTL_STATUS_KO) && g_btlTarget.allowKo == 0) {
            u->flags &= ~BTL_UNIT_TARGETED;
            btlui_setDefaultTargets(0);
            return 1;
        }
        return 1;
    }
    if (g_btlTarget.targetType != BTL_TGT_EVERYONE) {
        p = g_gameState->gameData->ctx.battle->unitList;
        n = 0;
        btlui_clearTargeted();
        while (p != NULL) {
            if (((p->flags >> BTL_UNIT_TARGETABLE_BIT) & 1) && (p->flags & BTL_UNIT_PARTY) == (u->flags & BTL_UNIT_PARTY) && (!(p->status & BTL_STATUS_KO) || g_btlTarget.allowKo != 0)) {
                p->flags |= BTL_UNIT_TARGETED;
                n++;
            }
            p = p->next;
        }
        if (n != 0) {
            g_btlTarget.targetAll = 1;
        }
    }
    return 1;
}

void btlui_confirmTarget(void) {
    s32 targets = 0;
    u8 ok = 1;
    BtlUnit *u = g_gameState->gameData->ctx.battle->unitList;
    Unk800FD56C *e = &g_btlCmdTable[g_btlTarget.cmdNo];
    Unk80104FF0 *target;
    for (; u != NULL; u = u->next) {
        if ((u->flags & (BTL_UNIT_TARGETABLE | BTL_UNIT_TARGETED)) == (BTL_UNIT_TARGETABLE | BTL_UNIT_TARGETED)) {
            targets |= u->unitBit;
        }
    }
    target = &g_btlTarget;
    btlui_setMenuState(BTLUI_MENU_CLOSING);
    if (target->cmdNo == BTL_CMD_DBL_BLK || target->cmdNo == BTL_CMD_DBL_WHT) {
        ok = btlui_confirmDoubleCast(targets);
    } else {
        target->unit->cmdIssued = 1;
        target->unit->inMenuQueue = 0;
        btl_setCmd((s32)g_btlTarget.unit->cmd, g_btlTarget.cmdNo, g_btlTarget.subNo, targets & 0xFFFF, btlui_getTargeting_inline());
    }
    btlui_clearTargeted();
    if (ok) {
        if ((u32)(e->kind - 2) < 2) {
            btlui_reserveItem(g_btlTarget.subNo);
        }
        btlui_closeWindows();
    }
    snd_playSe(SE_CURSOR);
}

s32 btlui_confirmDoubleCast(s32 targets) {
    BtlUnit *u;
    s32 n;
    s8 targeting;
    s32 r = 0;
    if (g_btlTarget.doubleCast == 0) {
        g_btlTarget.firstTargets = targets;
        g_btlTarget.firstCmdNo = g_btlTarget.cmdNo;
        g_btlTarget.firstSubNo = g_btlTarget.subNo;
        if ((u32)(g_btlTarget.targetType - 9) < 3) {
            targeting = 2;
        } else {
            n = 0;
            for (u = g_gameState->gameData->ctx.battle->unitList; u != NULL; u = u->next) {
                if ((u->flags & (BTL_UNIT_TARGETABLE | BTL_UNIT_TARGETED)) == (BTL_UNIT_TARGETABLE | BTL_UNIT_TARGETED)) {
                    n++;
                }
            }
            targeting = n >= 2;
        }
        g_btlTarget.firstTargeting = targeting;
        btlui_cancelTarget();
        btlui_nextDoubleCast();
    } else {
        g_btlTarget.unit->inMenuQueue = 0;
        g_btlTarget.unit->cmdIssued = 1;
        btl_setCmd((s32)g_btlTarget.unit->delayedCmd, g_btlTarget.firstCmdNo, g_btlTarget.firstSubNo, (u16)g_btlTarget.firstTargets, g_btlTarget.firstTargeting);
        if ((u32)(g_btlTarget.targetType - 9) < 3) {
            targeting = 2;
        } else {
            n = 0;
            for (u = g_gameState->gameData->ctx.battle->unitList; u != NULL; u = u->next) {
                if ((u->flags & (BTL_UNIT_TARGETABLE | BTL_UNIT_TARGETED)) == (BTL_UNIT_TARGETABLE | BTL_UNIT_TARGETED)) {
                    n++;
                }
            }
            targeting = n >= 2;
        }
        btl_setCmd((s32)g_btlTarget.unit->cmd, g_btlTarget.cmdNo, g_btlTarget.subNo, targets & 0xFFFF, targeting);
        r = 1;
    }
    return r;
}

void btlui_cancelTarget(void) {
    BtlUnit *u;
    s32 i;
    if (g_btlTarget.listKind == 0) {
        if (g_btlTarget.windowOpen != 0) {
            btlui_closeTargetWindow(g_btlTarget.returnState);
        } else {
            btlui_setMenuState(g_btlTarget.returnState);
        }
    } else {
        if (g_btlTarget.windowOpen != 0) {
            btlui_closeTargetWindow(5);
        }
        btlui_closeListWindow(g_btlTarget.returnState);
    }
    for (u = g_gameState->gameData->ctx.battle->unitList; u != 0; u = u->next) {
        u->flags &= ~BTL_UNIT_TARGETED;
    }
    for (i = 0; i < 2; i++) {
        pad_setRepeatEnabled(i, 2, 0);
        pad_setRepeatEnabled(i, 3, 0);
    }
}

void btlui_updateTargetWindow(void) {
    if ((g_btlTarget.windowOpen == 0 || (u32)(g_btlUi->windows[0]->state - 1) >= 2) && g_btlTarget.windowOpen != g_btluiTargetWindow) {
        if (g_btluiTargetWindow != 0) {
            if (g_btlTarget.windowOpen == 0) {
                btlui_setMenuState(BTLUI_MENU_CLOSING);
                g_btlUi->windows[0] = win_open(&g_btluiTargetWindowRect, 5, WIN_STYLE_BEVEL, 0);
                win_setOpenAnim(g_btlUi->windows[0], WIN_ANIM_ZOOM_X);
                win_setOnOpened(g_btlUi->windows[0], btlui_onTargetWindowOpened);
                win_setDrawContent(g_btlUi->windows[0], btlui_drawTargetList);
                win_setDrawOverlay(g_btlUi->windows[0], btlui_drawTargetListOverlay);
                g_btlTarget.openState = 3;
                g_btlTarget.windowOpen = 1;
            }
        } else if (g_btlTarget.windowOpen != 0) {
            g_btlTarget.closeState = 3;
            btlui_setMenuState(BTLUI_MENU_CLOSING);
            win_setOnClosed(g_btlUi->windows[0], btlui_onTargetWindowClosed);
            win_close(g_btlUi->windows[0]);
        }
    }
}

void btlui_drawTargetList(Window *win) {
    s32 y1 = win->y + 8;
    s32 y2 = y1;
    BtlUnit *u = g_gameState->gameData->ctx.battle->unitList;
    s32 color;
    u32 f;
    u8 *name;
    s32 pal;
    Window *w = win;
    ui_drawEtchedLine(win->x + 0x6E, win->y + 4, win->otz, 9 - win->h);
    color = text_getColor();
    for (; u != NULL; u = u->next) {
        name = btl_getUnitName(u);
        f = u->flags;
        if ((f >> BTL_UNIT_TARGETABLE_BIT) & 1) {
            if (f & BTL_UNIT_PARTY) {
                if (u->hp != 0) {
                    pal = ((u16)(u->max->maxHp / 6) >= u->hp) * TEXT_PAL_YELLOW;
                } else {
                    pal = TEXT_PAL_RED;
                }
                text_draw(w->x + 0x7A, y2, win->otz, name, pal);
                y2 += 15;
            } else {
                pal = ((u8)btl_hasStatus(u, BTL_STATUS_KO) != 0) * TEXT_PAL_GREY;
                text_drawBlock(w->x + 0xC, y1, win->otz, name, pal);
                y1 += 15;
            }
        }
    }
    text_setColor(color);
}

void btlui_drawTargetListOverlay(Window *win) {
    Window *w = win;
    s32 n = 0;
    BtlUnit *u = g_gameState->gameData->ctx.battle->unitList;
    RECT *r = &win->cur;
    BtlUnit *p;
    RECT rc;
    s32 yl;
    s32 yr;
    for (p = u; p != NULL; p = p->next) {
        if ((p->flags >> BTL_UNIT_TARGETED_BIT) & 1) {
            n++;
        }
    }
    if (win->state != WIN_STATE_OPEN) {
        win_setClip(NULL, win->otz, WIN_STYLE_FRAMELESS);
    }
    ui_drawLabel(w->x + 8, r->y - 3, win->otz, g_winLabelStrings.labelEnemy, UI_LABEL_NORMAL);
    ui_drawLabel(w->x + 0x76, r->y - 3, win->otz, g_winLabelStrings.labelPlayer, UI_LABEL_NORMAL);
    if (win->state != WIN_STATE_OPEN) {
        rc.x = r->x;
        rc.y = r->y - 3;
        rc.w = r->w;
        rc.h = 8;
        win_setClip(&rc, win->otz, WIN_STYLE_FRAMELESS);
    }
    if (win->state == WIN_STATE_OPEN && (n < 2 || !(g_btluiVsyncTick & 1))) {
        yl = r->y + 0xC;
        yr = yl;
        for (; u != NULL; u = u->next) {
            if ((u->flags >> BTL_UNIT_TARGETABLE_BIT) & 1) {
                if ((u->flags >> BTL_UNIT_TARGETED_BIT) & 1) {
                    if (u->flags & BTL_UNIT_PARTY) {
                        ui_drawHand(r->x + 0x62, yr, win->otz - 1);
                    } else {
                        ui_drawHand(r->x - 0xC, yl, win->otz - 1);
                    }
                }
                if (u->flags & BTL_UNIT_PARTY) {
                    yr += 15;
                } else {
                    yl += 15;
                }
            }
        }
    }
}

void btlui_drawTargetCursors(void) {
    u32 c = GPU_RGBC(GPU_SPRT, GPU_RGB_NEUTRAL);
    Unk80105020 *b = &g_btluiCursors;
    u32 *clut = &g_targetCursorSprite;
    s32 i;
    SprtWords *p;
    DR_TPAGE *t;
    if (b->count >= 2 && !(g_btluiVsyncTick & 3)) {
        c = GPU_RGBC(GPU_SPRT, 0x404040);
    }
    for (i = 0; i < b->count; i++) {
        setlen(g_primPtr, PRIM_LEN(SPRT));
        p = (SprtWords *)g_primPtr;
        p->rgbc = c;
        p->xy = (u16)b->pos[i][0] | (b->pos[i][1] << 16);
        p->uvclut = *clut;
        p->wh = 0xE0018;
        addPrim(&g_ot[17], p);
        g_primPtr = (u8 *)(p + 1);
    }
    setlen(g_primPtr, PRIM_LEN(DR_TPAGE));
    t = (DR_TPAGE *)g_primPtr;
    t->code[0] = get_mode(1, 1, getTPage(0, 0, 0x3C0, 0x100));
    addPrim(&g_ot[17], t);
    g_primPtr = (u8 *)(t + 1);
}

void *btlui_getTargetHelpText(void) {
    BtlUnit *u;
    BtlUnit *t;
    s32 n;
    s32 k;

    if (g_btlTarget.targetType < BTL_TGT_MULTI_ANY) {
        t = find_target();
        if (t == NULL) {
            return NULL;
        }
        text_copy(g_btluiHelpText, text_get(g_btlTarget.targetType + 5));
        text_append(g_btluiHelpText, btl_getUnitName(t));
    } else if ((u8)(g_btlTarget.targetType - BTL_TGT_MULTI_ANY) < 3) {
        n = 0;
        k = g_btlTarget.targetType - BTL_TGT_MULTI_ANY;
        for (u = g_gameState->gameData->ctx.battle->unitList; u != NULL; u = u->next) {
            if ((u->flags >> BTL_UNIT_TARGETED_BIT) & 1) {
                n++;
            }
        }
        if (n < 2) {
            t = find_target();
            if (t == NULL) {
                return NULL;
            }
            text_copy(g_btluiHelpText, text_get(k + 0x13));
            text_append(g_btluiHelpText, btl_getUnitName(t));
        } else {
            text_copy(g_btluiHelpText, text_get(k + 0x16));
        }
    } else {
        text_copy(g_btluiHelpText, text_get(g_btlTarget.targetType + 5));
    }
    text_append(g_btluiHelpText, text_get(0x1A));
    return g_btluiHelpText;
}

void btlui_onUnitTargetable(BtlUnit *unit) {
    if (g_btlTarget.targetAll != 0 && g_btluiRetargetOnAdd[g_btlTarget.targetType] != 0) {
        btlui_setDefaultTargets(0);
    }
}

void btlui_onUnitUntargetable(BtlUnit *unit) {
    if ((unit->flags >> BTL_UNIT_TARGETED_BIT) & 1) {
        unit->flags &= ~BTL_UNIT_TARGETED;
        if (g_btlTarget.targetAll == 0) {
            btlui_setDefaultTargets(0);
        }
    }
}

void btlui_targetInput(void) {
    u32 v;
    if (g_padRepeat & BTN_CIRCLE) {
        snd_playSe(SE_CANCEL);
        btlui_cancelTarget();
        return;
    }
    if (g_padRepeat & BTN_CROSS) {
        btlui_confirmTarget();
        return;
    }
    if (g_gameState->gameData->ctx.battle->escapeHeld == 0 && (g_padRepeat & (BTN_L1 | BTN_R1))) {
        btlui_toggleTargetAll();
        return;
    }
    if (g_padRepeat & BTN_L2) {
        snd_playSe(SE_CURSOR);
        v = g_gameState->config;
        g_gameState->config = (v & ~0x80000) | ((((v >> 19) & 1) ^ 1) << 19);
        return;
    }
    btlui_targetMove();
}

void btlui_updateTarget(void) {
    if (!(btlui_isCmdUsable() & 0xFF)) {
        snd_playSe(SE_CANCEL);
        btlui_cancelTarget();
    } else if (btlui_refreshTargets() & 0xFF) {
        btlui_updateTargetWindow();
    }
}

u8 btlui_isTargetWindowOpen(void) {
    return g_btlTarget.windowOpen;
}

Window *btlui_getTargetWindow(void) {
    return g_btlUi->windows[0];
}

s32 btlui_isTargetFromList(void) {
    s32 ret = 0;
    if (g_btlUi->menuState == BTLUI_MENU_TARGET) {
        ret = g_btlTarget.listKind != 0;
    }
    return ret;
}

s32 btlui_getTargetReturnState(void) {
    return g_btlTarget.returnState;
}

s32 btlui_countTargeted(void) {
    BtlUnit *u = g_gameState->gameData->ctx.battle->unitList;
    s32 n = 0;
    while (u != NULL) {
        if ((u->flags >> BTL_UNIT_TARGETED_BIT) & 1) {
            n++;
        }
        u = u->next;
    }
    return n;
}

void btlui_clearTargeted(void) {
    BtlUnit *u = g_gameState->gameData->ctx.battle->unitList;
    while (u != NULL) {
        u->flags &= ~BTL_UNIT_TARGETED;
        u = u->next;
    }
}

void btlui_collectCursorPositions(void) {
    s32 n;
    s32 m;
    BtlUnit *u;
    Unk80105020 *p;
    m = n = 0;
    u = g_gameState->gameData->ctx.battle->unitList;
    p = &g_btluiCursors;
    if (g_btluiAtbOn != 0) {
        for (; u != NULL; u = u->next) {
            if (((u->flags >> BTL_UNIT_TARGETABLE_BIT) & 1) && u->onScreen != 0) {
                p->pos[n][0] = u->screenX - 0x18;
                p->pos[n][1] = u->screenY - 7;
                n++;
                if (u->flags & BTL_UNIT_PARTY) {
                    p->party[m] = u;
                    m++;
                }
            }
        }
    }
    p->count = n;
    p->partyCount = m;
}

Unk80105020 *btlui_getCursorPositions(void) {
    return &g_btluiCursors;
}

void btlui_targetMove(void) {
    s32 i;
    s32 m;
    if (g_btlTarget.windowOpen == 0) {
        m = btlcam_getNextVariant();
    } else {
        m = 0;
    }
    for (i = 0; i < 4; i++) {
        if (g_padRepeat & g_btluiTargetMovePads[m][i]) {
            g_btluiTargetMoveFns[i]();
        }
    }
}

void btlui_targetPrev(void) {
    if (btlui_cycleTarget(0) & 0xFF) {
        snd_playSe(SE_CURSOR);
    }
}

void btlui_targetNext(void) {
    if (btlui_cycleTarget(1) & 0xFF) {
        snd_playSe(SE_CURSOR);
    }
}

void btlui_targetEnemySide(void) {
    if (btlui_switchTargetSide(0) & 0xFF) {
        snd_playSe(SE_CURSOR);
    }
}

void btlui_targetPartySide(void) {
    if (btlui_switchTargetSide(1) & 0xFF) {
        snd_playSe(SE_CURSOR);
    }
}

void btlui_targetFirst(u8 targetSide, u8 all) {
    BtlUnit *u = g_gameState->gameData->ctx.battle->unitList;
    if (targetSide == 0xFF) {
        targetSide = g_btlTarget.side;
    }
    for (; u != NULL; u = u->next) {
        if (((u->flags >> BTL_UNIT_TARGETABLE_BIT) & 1) && (u->flags & BTL_UNIT_PARTY) == targetSide) {
            if (!(u->status & BTL_STATUS_KO) || g_btlTarget.allowKo != 0) {
                u->flags |= BTL_UNIT_TARGETED;
                if (all == 0) {
                    return;
                }
            }
        }
    }
}

void btlui_targetDefault(u8 targetSide, u8 all) {
    BtlUnit *u = g_gameState->gameData->ctx.battle->unitList;
    u8 found = 0;
    BtlUnit *t;
    u32 f;
    u32 side;

    if (targetSide == 0xFF) {
        targetSide = g_btlTarget.side;
    }
    if (g_btlTarget.koFirst != 0) {
        t = btlui_findKoUnit(targetSide);
        if (t != NULL) {
            t->flags |= BTL_UNIT_TARGETED;
            return;
        }
    }
    for (; u != NULL; u = u->next) {
        f = u->flags;
        side = f & BTL_UNIT_PARTY;
        if (!((f >> BTL_UNIT_TARGETABLE_BIT) & 1) || side != targetSide) {
            continue;
        }
        if ((u->status & BTL_STATUS_KO) && g_btlTarget.allowKo == 0) {
            continue;
        }
        if (all == 0 && side != 0) {
            if (u == g_btlTarget.unit) {
                u->flags = f | BTL_UNIT_TARGETED;
                found = 1;
            }
        } else {
            found = 1;
            u->flags |= BTL_UNIT_TARGETED;
        }
        if (all == 0 && found) {
            return;
        }
    }
}

BtlUnit *btlui_findKoUnit(u8 side) {
    BtlUnit *u;
    for (u = g_gameState->gameData->ctx.battle->unitList; u != 0; u = u->next) {
        if (((u->flags >> BTL_UNIT_TARGETABLE_BIT) & 1) && (u->flags & BTL_UNIT_PARTY) == side && (u->status & BTL_STATUS_KO)) {
            return u;
        }
    }
    return 0;
}

void btlui_openTargetWindow(s8 openState) {
    Window *w;
    btlui_setMenuState(BTLUI_MENU_CLOSING);
    w = win_open(&g_btluiTargetWindowRect, 5, WIN_STYLE_BEVEL, 0);
    g_btlUi->windows[0] = w;
    win_setOpenAnim(w, WIN_ANIM_ZOOM_X);
    win_setOnOpened(g_btlUi->windows[0], btlui_onTargetWindowOpened);
    win_setDrawContent(g_btlUi->windows[0], btlui_drawTargetList);
    win_setDrawOverlay(g_btlUi->windows[0], btlui_drawTargetListOverlay);
    g_btlTarget.openState = openState;
    g_btlTarget.windowOpen = 1;
}

void btlui_closeTargetWindow(s32 closeState) {
    if (g_btlTarget.windowOpen != 0) {
        g_btlTarget.closeState = closeState;
        btlui_setMenuState(BTLUI_MENU_CLOSING);
        win_setOnClosed(g_btlUi->windows[0], btlui_onTargetWindowClosed);
        win_close(g_btlUi->windows[0]);
    }
}

void btlui_onTargetWindowOpened(Window *win) {
    if (g_btlTarget.returnState == BTLUI_MENU_LIST) {
        win_setVisible(g_btlUi->windows[2], 0);
    }
    win_setVisible(g_btlUi->windows[3], 0);
    btlui_setMenuState(g_btlTarget.openState);
}

void btlui_onTargetWindowClosed(Window *win) {
    if (g_btlTarget.returnState == BTLUI_MENU_LIST) {
        win_setVisible(g_btlUi->windows[2], 1);
    }
    win_setVisible(g_btlUi->windows[3], 1);
    g_btlTarget.windowOpen = 0;
    g_btlUi->windows[0] = 0;
    btlui_setMenuState((u8)g_btlTarget.closeState);
}

s32 btlui_getTargeting(void) {
    BtlUnit *u;
    s32 n;
    if (g_btlTarget.targetType >= 9 && g_btlTarget.targetType <= 11) {
        return 2;
    }
    n = 0;
    for (u = g_gameState->gameData->ctx.battle->unitList; u != 0; u = u->next) {
        if ((u->flags & (BTL_UNIT_TARGETABLE | BTL_UNIT_TARGETED)) == (BTL_UNIT_TARGETABLE | BTL_UNIT_TARGETED)) {
            n++;
        }
    }
    return n >= 2;
}

void btlui_drawListCursors(Window *win) {
    Window *w = win;
    s32 n = 0;
    BtlUnit *u = g_gameState->gameData->ctx.battle->unitList;
    Unk80105020 *b = btlui_getCursorPositions();
    Unk80105078 *m = btlui_getPanel();
    u32 c;
    s32 y;
    s32 x;
    s32 k;
    s32 yy;
    Unk80068EE8 *d;
    SprtWords *p;
    DR_TPAGE *t;

    if (b->partyCount == 0 || win->state != WIN_STATE_OPEN || m->shown == 0) {
        return;
    }
    c = GPU_RGBC(GPU_SPRT, GPU_RGB_NEUTRAL);
    if (b->partyCount >= 2 && !(g_btluiVsyncTick & 3)) {
        c = GPU_RGBC(GPU_SPRT, 0x404040);
    }
    y = w->y + 5;
    x = w->x - 0xE;
    for (; u != NULL; u = u->next) {
        if (!(u->flags & BTL_UNIT_PARTY)) {
            continue;
        }
        if (u == b->party[n]) {
            for (k = 0, yy = y; k < 2; k++, yy += 6) {
                d = &g_listCursorSprites[k];
                setlen(g_primPtr, PRIM_LEN(SPRT));
                p = (SprtWords *)g_primPtr;
                p->rgbc = c;
                p->xy = x | (yy << 16);
                p->uvclut = d->uvclut;
                p->wh = d->w | (d->h << 16);
                addPrim(&g_ot[win->otz - 1], p);
                g_primPtr = (u8 *)(p + 1);
            }
            n++;
            if (n >= b->partyCount) {
                break;
            }
        }
        y += 0xC;
    }
    setlen(g_primPtr, PRIM_LEN(DR_TPAGE));
    t = (DR_TPAGE *)g_primPtr;
    t->code[0] = get_mode(1, 1, g_listCursorSprites[0].tpage);
    addPrim(&g_ot[win->otz], t);
    g_primPtr = (u8 *)(t + 1);
}

void btlui_drawHpList(Window *win) {
    u8 hpText[8];
    u8 maxText[8];
    BtlUnit *u;
    Window *w = win;
    s32 y = win->y + 6;
    s32 dy = win->h - win->cur.h;
    s32 c;
    for (u = g_gameState->gameData->ctx.battle->unitList; u != 0 && (u->flags & BTL_UNIT_PARTY); u = u->next) {
        c = DIGITS_RED;
        if (u->hp != 0) {
            c = ((u->status & BTL_STATUS_LOW_HP) != 0) * DIGITS_YELLOW;
        }
        maxText[0] = '/';
        text_formatNumber(u->hp, hpText, 4, TEXT_NUM_ASCII);
        text_formatNumber(u->max->maxHp, maxText + 1, TEXT_NUM_NO_PAD, TEXT_NUM_ASCII);
        ui_drawDigits(w->x + 4, y, win->otz, hpText, c);
        ui_drawDigits(w->x + 0x20, y - dy, win->otz, maxText, c);
        y += 12;
    }
}

void btlui_drawMpList(Window *win) {
    u8 mpText[8];
    u8 maxText[8];
    Window *w = win;
    s32 y = win->y + 6;
    BtlUnit *u = g_gameState->gameData->ctx.battle->unitList;
    s32 dy = win->h - win->cur.h;
    s32 yy;
    s32 c;
    while (u != NULL) {
        if (u->flags & BTL_UNIT_PARTY) {
            yy = y - dy;
            c = (u->mp < (s16)(u->max->maxMp / 6)) * DIGITS_YELLOW;
            maxText[0] = '/';
            text_formatNumber(u->mp, mpText, 3, TEXT_NUM_ASCII);
            y += 0xC;
            text_formatNumber((s16)u->max->maxMp, &maxText[1], TEXT_NUM_NO_PAD, TEXT_NUM_ASCII);
            ui_drawDigits(w->x + 4, yy, win->otz, mpText, c);
            ui_drawDigits(w->x + 0x19, yy, win->otz, maxText, c);
        }
        u = u->next;
    }
}

void btlui_drawStatusList(Window *win) {
    BtlUnit *u;
    s32 i;
    s32 n;
    s32 x;
    Window *w = win;
    s32 d = win->h - win->cur.h;
    s32 y;
    u32 st;
    d -= 6;
    y = win->y - d;
    for (u = g_gameState->gameData->ctx.battle->unitList; u != 0; u = u->next) {
        if (u->flags & BTL_UNIT_PARTY) {
            n = 0;
            st = u->status | u->autoStatus;
            x = w->x + 4;
            for (i = 0; i < 16; i++) {
                if (st & g_btluiStatusIcons[i].status) {
                    ui_drawSprite(x, y, win->otz, g_btluiStatusIcons[i].icon, 0);
                    x += 12;
                    if (++n >= 9) {
                        break;
                    }
                }
            }
            y += 12;
        }
    }
}

void btlui_drawStatusList2(Window *win) {
    BtlUnit *u;
    s32 i;
    s32 x;
    Window *w = win;
    s32 d = win->h - win->cur.h;
    s32 y;
    u32 st;
    d -= 6;
    y = win->y - d;
    for (u = g_gameState->gameData->ctx.battle->unitList; u != 0; u = u->next) {
        if (u->flags & BTL_UNIT_PARTY) {
            st = u->status | u->autoStatus;
            x = w->x + 4;
            for (i = 0; i < 8; i++) {
                if (st & g_btluiStatusIcons2[i].status) {
                    ui_drawSprite(x, y, win->otz, g_btluiStatusIcons2[i].icon, 0);
                }
                x += g_btluiStatusIcons2[i].width;
            }
            y += 12;
        }
    }
}

void btlui_cacheHeader(void) {
    Unk80105078 *p;
    RECT r;

    p = btlui_getPanel();
    setRECT(&r, 0xD0, p->y - 4, 0x40, 4);
    r.x += g_btluiDispOrigin[g_gameState->gameData->drawBuf & 1][0];
    r.y += g_btluiDispOrigin[g_gameState->gameData->drawBuf & 1][1];
    SetDrawMove((DR_MOVE *)g_primPtr, &r, VRAM_BTLUI_CACHE_X, 0x1F8);
    addPrim(&g_ot[19], g_primPtr);
    g_primPtr += sizeof(DR_MOVE);
    setRECT(&r, 0x110, p->y - 4, 0x30, 4);
    r.x += g_btluiDispOrigin[g_gameState->gameData->drawBuf & 1][0];
    r.y += g_btluiDispOrigin[g_gameState->gameData->drawBuf & 1][1];
    SetDrawMove((DR_MOVE *)g_primPtr, &r, VRAM_BTLUI_CACHE_X, 0x1FC);
    addPrim(&g_ot[19], g_primPtr);
    g_primPtr += sizeof(DR_MOVE);
}

void btlui_drawCachedHeader(void) {
    SPRT *a = &g_btluiPrims->headerA[g_drawBufs.cur];
    SPRT *b = &g_btluiPrims->headerB[g_drawBufs.cur];
    a->y0 = b->y0 = btlui_getPanel()->y - 4;
    addPrim(&g_ot[18], a);
    addPrim(&g_ot[18], b);
    addPrim(&g_ot[18], &g_btluiPrims->headerTpage[g_drawBufs.cur]);
}

void btlui_initCachedHeader(s32 bufIndex) {
    setSprt(&g_btluiPrims->headerA[bufIndex]);
    setRGB0(&g_btluiPrims->headerA[bufIndex], 0x80, 0x80, 0x80);
    g_btluiPrims->headerA[bufIndex].x0 = 0xD0;
    g_btluiPrims->headerA[bufIndex].u0 = 0;
    g_btluiPrims->headerA[bufIndex].v0 = 0xF8;
    g_btluiPrims->headerA[bufIndex].clut = 0;
    g_btluiPrims->headerA[bufIndex].w = 0x40;
    g_btluiPrims->headerA[bufIndex].h = 4;
    setSprt(&g_btluiPrims->headerB[bufIndex]);
    setRGB0(&g_btluiPrims->headerB[bufIndex], 0x80, 0x80, 0x80);
    g_btluiPrims->headerB[bufIndex].x0 = 0x110;
    g_btluiPrims->headerB[bufIndex].u0 = 0;
    g_btluiPrims->headerB[bufIndex].v0 = 0xFC;
    g_btluiPrims->headerB[bufIndex].clut = 0;
    g_btluiPrims->headerB[bufIndex].w = 0x30;
    g_btluiPrims->headerB[bufIndex].h = 4;
    setDrawTPage(&g_btluiPrims->headerTpage[bufIndex], 1, 1, getTPage(2, 0, VRAM_BTLUI_CACHE_X, VRAM_BTLUI_CACHE_Y));
}

void btlui_openListWindow(s32 kind) {
    Unk80105078 *p = btlui_getPanel();
    Window *w;
    g_btluiListWindowRect.y = p->y;
    g_btluiListWindowRect.h = p->rowCount * 12 + 8;
    w = win_open(&g_btluiListWindowRect, 0xA, WIN_STYLE_LINE, 1);
    g_btlUi->windows[1] = w;
    win_setDrawContent(w, btlui_drawListWindow);
    win_setDrawOverlay(g_btlUi->windows[1], btlui_drawListCursors);
    win_setOpenFrames(g_btlUi->windows[1], 7);
    btlui_showListHeader(kind);
    g_btluiListWindow.kind = kind;
    g_btluiListWindow.open = 1;
}

void btlui_closeListWindow(s32 returnState) {
    win_setCloseFrames(g_btlUi->windows[1], 7);
    win_close(g_btlUi->windows[1]);
    btlui_hideListHeader(returnState);
    g_btluiListWindow.open = 0;
}

/* resizes the battle list window to the current entry count */
void btlui_resizeListWindow(void) {
    Unk80105078 *p = btlui_getPanel();
    Window *w;
    RECT *r;
    s16 h;

    if (g_btluiListWindow.open != 0 && p->rowCount != 0) {
        w = g_btlUi->windows[1];
        r = &w->cur;
        r->y = w->y = p->y;
        h = p->rowCount * 12 + 8;
        w->h = h;
        if (w->state == WIN_STATE_CLOSING || w->state == WIN_STATE_OPEN) {
            r->h = h;
        } else if (w->state == WIN_STATE_OPENING) {
            if (h < r->h) {
                r->h = h;
            }
        }
    }
}

u8 btlui_isListWindowOpen(void) {
    return g_btluiListWindow.open;
}

void btlui_drawListWindow(Window *win) {
    if (btlui_getPanel()->shown != 0) {
        g_btluiListWindowDrawFns[g_btluiListWindow.kind](win);
    }
}

void btlui_initHeap(void) {
    g_btluiHeap.cursor = g_gameState->gameData->packetBuf;
    g_btluiHeap.base = g_btluiHeap.cursor;
    g_btluiHeap.free = 0x8000;
    g_btluiHeap.size = g_btluiHeap.free;
    g_btluiHeap.used = 0;
}

void btlui_termHeap(void) {
}

void *btlui_alloc(s32 bytes) {
    s32 size = (bytes + 3) >> 2 << 2;
    s32 p = g_btluiHeap.cursor;
    g_btluiHeap.cursor += size;
    g_btluiHeap.used += size;
    g_btluiHeap.free -= size;
    return (void *)p;
}

void btlui_updatePanel(void) {
    BtlUnit *u = g_gameState->gameData->ctx.battle->unitList;
    s32 n = 0;
    for (; u != NULL; u = u->next) {
        if (u->flags & BTL_UNIT_PARTY) {
            g_btluiPanel->units[n] = u;
            g_btluiPanel->cantAct[n] = (btl_hasStatus(u, (BTL_STATUS_PETRIFY | BTL_STATUS_VENOM | BTL_STATUS_KO | BTL_STATUS_CONFUSE | BTL_STATUS_BERSERK | BTL_STATUS_STOP | BTL_STATUS_SLEEP | BTL_STATUS_FREEZE | BTL_STATUS_JUMP)) & 0xFF) != 0;
            g_btluiPanel->slot[n] = (u8)u->flags >> BTL_UNIT_SLOT_SHIFT;
            if (btl_hasStatus(u, (BTL_STATUS_STOP | BTL_STATUS_SLOW)) & 0xFF) {
                g_btluiPanel->atbKind[n] = 2;
            } else if (btl_hasStatus(u, BTL_STATUS_HASTE) & 0xFF) {
                g_btluiPanel->atbKind[n] = 3;
            } else {
                g_btluiPanel->atbKind[n] = 0;
            }
            if ((u->flags >> BTL_UNIT_TRANCE_ENABLED_BIT) & 1) {
                g_btluiPanel->tranceKind[n] = (btl_hasStatus(u, BTL_STATUS_TRANCE) & 0xFF) ? 2 : 1;
            } else {
                g_btluiPanel->tranceKind[n] = 0;
            }
            n++;
        }
    }
    g_btluiPanel->menuUnit = g_btlUi->cmdUnit;
    g_btluiPanel->rowCount = n;
    g_btluiPanel->y = g_btluiPanelY[n - 1];
    g_btluiPanel->unk1 = 0;
    g_btluiPanel->highlightRow = 0xFF;
}

void btlui_initHighlightPrim(s32 bufIndex) {
    setPolyG4(&g_btluiPrims->highlight[bufIndex]);
    setSemiTrans(&g_btluiPrims->highlight[bufIndex], 1);
    setRGB0(&g_btluiPrims->highlight[bufIndex], 0x40, 0x40, 0);
    setRGB1(&g_btluiPrims->highlight[bufIndex], 0x10, 0x10, 0);
    setRGB2(&g_btluiPrims->highlight[bufIndex], 0x40, 0x40, 0);
    setRGB3(&g_btluiPrims->highlight[bufIndex], 0x10, 0x10, 0);
    g_btluiPrims->highlight[bufIndex].x0 = 0x8E;
    g_btluiPrims->highlight[bufIndex].x1 = 0x137;
    g_btluiPrims->highlight[bufIndex].x3 = 0x137;
    g_btluiPrims->highlight[bufIndex].x2 = 0x8E;
    setDrawTPage(&g_btluiPrims->highlightTpage[bufIndex], 1, 1, getTPage(0, 1, 0, 0));
}

void btlui_initNamePrims(s32 bufIndex) {
    s32 i;
    SPRT *s;
    for (i = 0; i < 4; i++) {
        setSprt(&g_btluiPrims->names[i][bufIndex]);
        setRGB0(&g_btluiPrims->names[i][bufIndex], 0x80, 0x80, 0x80);
        g_btluiPrims->names[i][bufIndex].x0 = 0x92;
        g_btluiPrims->names[i][bufIndex].h = 0xC;
        g_btluiPrims->names[i][bufIndex].w = 0x54;
        if (i & 1) {
            g_btluiPrims->names[i][bufIndex].u0 = 0x54;
        } else {
            g_btluiPrims->names[i][bufIndex].u0 = 0;
        }
        g_btluiPrims->names[i][bufIndex].v0 = 0xF0;
    }
    setDrawTPage(&g_btluiPrims->namesTpage[bufIndex], 1, 1, getTPage(0, 0, 0x380, 0x100));
}

void btlui_initCachedRows(s32 bufIndex) {
    s32 i;
    SPRT *s;
    for (i = 0; i < 4; i++) {
        setSprt(&g_btluiPrims->rowsA[i][bufIndex]);
        setRGB0(&g_btluiPrims->rowsA[i][bufIndex], 0x80, 0x80, 0x80);
        g_btluiPrims->rowsA[i][bufIndex].x0 = 0xD0;
        g_btluiPrims->rowsA[i][bufIndex].u0 = 0;
        g_btluiPrims->rowsA[i][bufIndex].w = 0x1D;
        g_btluiPrims->rowsA[i][bufIndex].h = 0xA;
        g_btluiPrims->rowsA[i][bufIndex].clut = 0;
        setSprt(&g_btluiPrims->rowsB[i][bufIndex]);
        setRGB0(&g_btluiPrims->rowsB[i][bufIndex], 0x80, 0x80, 0x80);
        g_btluiPrims->rowsB[i][bufIndex].x0 = 0xEF;
        g_btluiPrims->rowsB[i][bufIndex].u0 = 0x20;
        g_btluiPrims->rowsB[i][bufIndex].w = 0x16;
        g_btluiPrims->rowsB[i][bufIndex].h = 0xA;
        g_btluiPrims->rowsB[i][bufIndex].clut = 0;
    }
    setDrawTPage(&g_btluiPrims->rowsTpage[bufIndex], 1, 1, getTPage(2, 0, VRAM_BTLUI_CACHE_X, VRAM_BTLUI_CACHE_Y));
}

void btlui_drawPartyPanel(void) {
    u32 i;
    s32 j;
    if (g_btluiPanel->shown != 0 && g_btluiPanel->rowCount != 0) {
        btlui_drawPanelHighlight();
        btlui_drawPanelHeader();
        for (i = 0; i < g_btluiPanel->rowCount; i++) {
            for (j = 0; j < 4; j++) {
                btlui_updatePanelRowColor(i);
                if (g_btluiPanel->labels[j].visible != 0 && g_btluiPanelColumnFns[j] != 0) {
                    g_btluiPanelColumnFns[j](g_btluiPanel->units[i], i);
                }
            }
        }
    }
}

void btlui_updateGauges(void) {
    BtlUnit *u;
    Unk80104F88Entry *e;
    Unk80104F88Gauge *g;
    Unk80104F88Gauge *mp;
    s32 i;
    s32 cur;
    s32 max;
    s32 d;
    s32 a;
    s32 t;

    for (u = g_gameState->gameData->ctx.battle->unitList; u != NULL; u = u->next) {
        if (!(u->flags & BTL_UNIT_PARTY)) {
            continue;
        }
        e = u->menu;
        mp = &e->gauges[1];
        for (i = 0; i < 2; i++) {
            g = mp;
            if (i == 0) {
                g = &e->gauges[0];
            }
            if (g->frames != 0) {
                if (g->step >= 0) {
                    if (g->shown + g->step >= g->target) {
                        g->shown = g->target;
                        g->frames = 0;
                    } else {
                        g->shown += g->step;
                        g->frames--;
                    }
                } else {
                    if (g->shown + g->step <= g->target) {
                        g->shown = g->target;
                        g->frames = 0;
                    } else {
                        g->shown += g->step;
                        g->frames--;
                    }
                }
            } else {
                cur = (i == 0) ? u->hp : u->mp;
                max = (i == 0) ? u->max->maxHp : u->max->maxMp;
                d = cur - g->shown;
                if (d != 0) {
                    a = (d >= 0) ? d : -d;
                    g->target = cur;
                    if (a < 0x10) {
                        g->frames = a;
                    } else {
                        g->frames = (a << 6) / max;
                        if (g->frames < 0x10) {
                            a = 0x10;
                            g->frames = a;
                        }
                    }
                    if (d >= 0) {
                        t = d - 1;
                        g->step = (t + g->frames) / g->frames;
                    } else {
                        t = d + 1;
                        g->step = (t - g->frames) / g->frames;
                    }
                }
            }
        }
    }
}

void btlui_drawPanelHeader(void) {
    RECT r;
    s32 i;
    Unk800F412C *e;

    ui_drawRule(0x8E, g_btluiPanel->y, 8, 1, 0xA9, CONFIG_WINDOW_COLOR());
    r.x = 0x8E;
    r.y = g_btluiPanel->y - 3;
    r.w = g_gameState->dispEnv->disp.w - 0x8E;
    r.h = 7;
    win_setClip(NULL, 7, WIN_STYLE_FRAMELESS);
    e = g_btluiPanel->labels;
    for (i = 0; i < 5; e++, i++) {
        if (e->visible != 0) {
            ui_drawLabel((s16)e->x, g_btluiPanel->y + (s16)e->y, 7,
                          ((s32 *)&g_winLabelStrings)[e->text], UI_LABEL_NORMAL);
        }
    }
    win_setClip(&r, 7, WIN_STYLE_FRAMELESS);
}

void btlui_drawPanelHighlight(void) {
    u8 c = g_btluiPanel->highlightRow;
    POLY_G4 *p;
    s32 y;
    s32 d = 4;
    if (c != 0xFF && g_btluiFrameReady != 0) {
        y = g_btluiPanel->y + (c * 12 + d);
        p = &g_btluiPrims->highlight[g_drawBufs.cur];
        p->y0 = p->y1 = y;
        p->y2 = p->y3 = y + 13;
        addPrim(&g_ot[20], p);
        addPrim(&g_ot[20], &g_btluiPrims->highlightTpage[g_drawBufs.cur]);
    }
}

void btlui_drawPanelName(s32 unit, u32 row) {
    s32 k = g_btluiPanel->slot[row];
    SPRT *s = &g_btluiPrims->names[k][g_drawBufs.cur];
    s->y0 = g_btluiPanel->y + row * 12 + 3;
    s->clut = g_btluiRowCluts[(u8)g_btluiPanel->rowColor[row]] + ((k >> 1) << 6);
    addPrim(&g_ot[16], s);
    if (row >= g_btluiPanel->rowCount - 1) {
        addPrim(&g_ot[16], &g_btluiPrims->namesTpage[g_drawBufs.cur]);
    }
}

void btlui_drawPanelGauges(BtlUnit *unit, s32 row) {
    s32 tranceKind = g_btluiPanel->tranceKind[row];
    u8 active = 0;
    u32 y = g_btluiPanel->y + row * 12;
    if (unit->cmdIssued == 0) {
        active = g_btluiPanel->cantAct[row] == 0;
    }
    if (tranceKind == 2) {
        ui_drawGauge(0x10C, y + 10, 14, unit->trance, 0xFF, 1, active);
        ui_drawGauge(0x108, y + 6, 14, unit->atb, unit->max->maxAtb, g_btluiPanel->atbKind[row], active);
    } else if (tranceKind == 1) {
        ui_drawGauge(0x108, y + 6, 14, unit->atb, unit->max->maxAtb, g_btluiPanel->atbKind[row], active);
        ui_drawGauge(0x10C, y + 10, 14, unit->trance, 0xFF, tranceKind, active);
    } else {
        ui_drawGauge(0x108, y + 8, 14, unit->atb, unit->max->maxAtb, g_btluiPanel->atbKind[row], active);
    }
}

void btlui_cacheRows(void) {
    RECT a = s_btluiRowsSrcA;
    RECT b = s_btluiRowsSrcB;
    s16 h;
    if (g_btluiPanel->shown != 0 && g_btluiPanel->rowCount != 0) {
        a.x += g_btluiDispOrigin[g_gameState->gameData->drawBuf & 1][0];
        b.x += g_btluiDispOrigin[g_gameState->gameData->drawBuf & 1][0];
        a.y += g_btluiDispOrigin[g_gameState->gameData->drawBuf & 1][1] + g_btluiPanel->y;
        b.y += g_btluiDispOrigin[g_gameState->gameData->drawBuf & 1][1] + g_btluiPanel->y;
        h = g_btluiPanel->rowCount * 12;
        b.h = h;
        a.h = h;
        SetDrawMove((DR_MOVE *)g_primPtr, &a, VRAM_BTLUI_CACHE_X, 0x1B0);
        addPrim(&g_ot[19], g_primPtr);
        g_primPtr += sizeof(DR_MOVE);
        SetDrawMove((DR_MOVE *)g_primPtr, &b, 0x3E0, 0x1B0);
        addPrim(&g_ot[19], g_primPtr);
        g_primPtr += sizeof(DR_MOVE);
    }
}

void btlui_drawCachedRows(void) {
    s32 y = g_btluiPanel->y + 6;
    u8 v = 0xB0;
    u32 i;
    SPRT *a;
    SPRT *b;
    if (g_btluiPanel->shown != 0 && g_btluiPanel->rowCount != 0) {
        for (i = 0; i < g_btluiPanel->rowCount; i++) {
            b = &g_btluiPrims->rowsA[i][g_drawBufs.cur];
            a = &g_btluiPrims->rowsB[i][g_drawBufs.cur];
            a->y0 = y;
            b->y0 = y;
            y += 12;
            a->v0 = v;
            b->v0 = v;
            v += 12;
            addPrim(&g_ot[18], b);
            addPrim(&g_ot[18], a);
        }
        addPrim(&g_ot[18], &g_btluiPrims->rowsTpage[g_drawBufs.cur]);
    }
}

void btlui_initPanel(void) {
    s32 i;
    g_btluiPanel = btlui_alloc(0x8C);
    btlui_updatePanel();
    for (i = 0; i < 5; i++) {
        g_btluiPanel->labels[i] = g_btluiPanelLabels[i];
        btlui_alignPanelLabel(&g_btluiPanel->labels[i]);
    }
    for (i = 0; i < g_btluiPanel->rowCount; i++) {
        btlui_resetGauges(i);
    }
    g_btluiPanel->pulse = 0;
    g_btluiPanel->shown = 1;
    g_btluiPanel->menuUnit = 0;
}

void btlui_updatePanelAnims(void) {
    btlui_slidePanelLabels();
    btlui_updateGauges();
    btlui_pulsePanelHighlight();
}

void btlui_showListHeader(s32 kind) {
    btlui_startLabelSlide(&g_btluiPanel->labels[1], &g_btluiListShowSlide);
    btlui_startLabelSlide(&g_btluiPanel->labels[2], &g_btluiListShowSlide);
    btlui_startLabelSlide(&g_btluiPanel->labels[3], &g_btluiListShowSlide);
    btlui_startLabelSlide(&g_btluiPanel->labels[4], &g_btluiListShowTitleSlide);
    g_btluiPanel->labels[4].text = g_btluiListTitles[kind].text;
    btlui_setMenuState(BTLUI_MENU_CLOSING);
}

void btlui_hideListHeader(s32 returnState) {
    g_btluiPanel->returnState = returnState;
    btlui_startLabelSlide(&g_btluiPanel->labels[1], &g_btluiListHideSlide);
    btlui_startLabelSlide(&g_btluiPanel->labels[2], &g_btluiListHideSlide);
    btlui_startLabelSlide(&g_btluiPanel->labels[3], &g_btluiListHideSlide);
    btlui_startLabelSlide(&g_btluiPanel->labels[4], &g_btluiListHideTitleSlide);
    btlui_setMenuState(BTLUI_MENU_CLOSING);
}

void btlui_refreshPanel(void) {
    btlui_updatePanel();
    if (g_btluiPanel->menuUnit != 0) {
        btlui_findHighlightRow();
    }
}

Unk80105078 *btlui_getPanel(void) {
    return g_btluiPanel;
}

u8 btlui_isPanelShown(void) {
    return g_btluiPanel->shown;
}

void btlui_showPanel(u8 shown) {
    g_btluiPanel->shown = shown;
}

void btlui_initPanelPrims(s32 bufIndex) {
    btlui_initHighlightPrim(bufIndex);
    btlui_initNamePrims(bufIndex);
    btlui_initCachedRows(bufIndex);
}

void btlui_slidePanelLabels(void) {
    s32 i;
    Unk800F412C *e = g_btluiPanel->labels;
    for (i = 0; i < 5; i++) {
        if (e->slide != 0) {
            e->x += e->slide->dx;
            e->y += e->slide->dy;
            if (++e->frame >= e->slide->frames) {
                e->frame = 0;
                if (e->slide->onDone != 0) {
                    e->slide->onDone(e);
                }
                e->slide = 0;
            }
        }
        e++;
    }
}

void btlui_pulsePanelHighlight(void) {
    Unk80105078 *p = g_btluiPanel;
    u32 v = g_btluiVsyncTick & 0x3F;
    p->pulse = v;
    if (v >= 0x20) {
        p->pulse = 0x3F - v;
    }
}

void btlui_resetGauges(s32 row) {
    BtlUnit *u = g_btluiPanel->units[row];
    Unk80104F88Entry *e = u->menu;
    Unk80104F88Gauge *g;
    s16 v;
    e->gauges[0].target = e->gauges[0].shown = u->hp;
    v = u->mp;
    g = &e->gauges[1];
    g->frames = 0;
    g->target = g->shown = v;
    e->gauges[0].frames = 0;
    e->gauges[1].step = 0;
    e->gauges[0].step = 0;
}

void btlui_updatePanelRowColor(s32 row) {
    BtlUnit *u = g_btluiPanel->units[row];
    s32 hp = u->hp;
    if (hp == 0) {
        g_btluiPanel->rowColor[row] = DIGITS_RED;
        return;
    }
    g_btluiPanel->rowColor[row] = ((s32)(u16)(u->max->maxHp / 6) >= hp) * DIGITS_YELLOW;
}

void btlui_alignPanelLabel(Unk800F412C *label) {
    if (label->align == 1) {
        label->x -= ui_measureLabel(((u8 **)&g_winLabelStrings)[label->text]);
    }
}

void btlui_startLabelSlide(Unk800F412C *label, Unk800F3F10 *slide) {
    label->slide = slide;
    label->visible = 1;
    label->frame = 0;
}

void btlui_onPanelLabelsOut(void) {
    g_btluiPanel->labels[1].visible = 0;
    g_btluiPanel->labels[2].visible = 0;
    g_btluiPanel->labels[3].visible = 0;
    btlui_setMenuState(BTLUI_MENU_TARGET);
}

void btlui_onListLabelOut(void) {
    g_btluiPanel->labels[4].visible = 0;
    btlui_setMenuState(g_btluiPanel->returnState);
}

void btlui_drawPanelHp(BtlUnit *unit, s32 row) {
    u8 digits[8];
    s32 dy = 6;
    text_formatNumber(unit->menu->gauges[0].shown, digits, 4, TEXT_NUM_ASCII);
    ui_drawDigits(0xD0, g_btluiPanel->y + (row * 12 + dy), 0xE, digits, (u8)g_btluiPanel->rowColor[row]);
}

void btlui_drawPanelMp(BtlUnit *unit, s32 row) {
    u8 digits[8];
    s32 mp;
    s32 pal;
    s32 dy = 6;
    mp = unit->menu->gauges[1].shown < 0 ? 0 : unit->menu->gauges[1].shown;
    pal = ((s16)(unit->max->maxMp / 6) >= mp) * DIGITS_YELLOW;
    text_formatNumber(mp, digits, 3, TEXT_NUM_ASCII);
    ui_drawDigits(0xEF, g_btluiPanel->y + (row * 12 + dy), 0xE, digits, pal);
}

void btlui_findHighlightRow(void) {
    BtlUnit *u = g_gameState->gameData->ctx.battle->unitList;
    s32 i = 0;
    if (g_btluiPanel->rowCount != 0 && g_btluiHideWindows == 0) {
        for (; u != 0; u = u->next) {
            if (u->flags & BTL_UNIT_PARTY) {
                if (g_btluiPanel->menuUnit == u) {
                    g_btluiPanel->highlightRow = i;
                    return;
                }
                i++;
            }
        }
    }
}

void btlui_drawPortIcons(void) {
    GameData *g = g_gameState->gameData;
    Unk80068EE8 *s;
    s32 f;

    if (!(g->flags & GD_PAUSED) && !(g->ctx.battle->flags & BTL_FLAG_DISC_COVER_OPEN) && g_btluiState.enabled != 0 &&
        (f = (g_gameState->config >> CFG_BIT_BTL_CONTROL) & 1) == 1) {
        ClearOTag(&g_btluiPortIconOt, 1);
        if (g_btluiState.vsyncTick & 0x20) {
            if (g_btluiState.port == 0) {
                s = &g_portBlinkSprite1;
            } else {
                s = &g_portBlinkSprite2;
            }
            setlen(&g_btluiPortBlinkSprt, PRIM_LEN(SPRT));
            g_btluiPortBlinkSprt.rgbc = GPU_RGBC(GPU_SPRT, GPU_RGB_NEUTRAL);
            g_btluiPortBlinkSprt.xy = 0x6012B;
            g_btluiPortBlinkSprt.uvclut = s->uvclut;
            g_btluiPortBlinkSprt.wh = s->w | (s->h << 16);
            addPrim(&g_btluiPortIconOt, &g_btluiPortBlinkSprt);
        }
        if (g_btluiPort == 0) {
            s = &g_portIconSprite1;
        } else {
            s = &g_portIconSprite2;
        }
        setlen(&g_btluiPortIconSprt, PRIM_LEN(SPRT));
        g_btluiPortIconSprt.rgbc = GPU_RGBC(GPU_SPRT, GPU_RGB_NEUTRAL);
        g_btluiPortIconSprt.xy = 0x60124;
        g_btluiPortIconSprt.uvclut = s->uvclut;
        g_btluiPortIconSprt.wh = s->w | (s->h << 16);
        addPrim(&g_btluiPortIconOt, &g_btluiPortIconSprt);
        setDrawTPage(&g_btluiPortIconTpage, 1, 1, s->tpage);
        addPrim(&g_btluiPortIconOt, &g_btluiPortIconTpage);
        setlen(&g_btluiPortIconStp, PRIM_LEN(DR_STP));
        g_btluiPortIconStp.code[0] = GPU_STP(1, 0);
        g_btluiPortIconStp.code[1] = 0;
        addPrim(&g_btluiPortIconOt, &g_btluiPortIconStp);
        DrawOTag(&g_btluiPortIconOt);
        DrawSync(0);
    }
}

void btlui_saveKeyRepeat(void) {
    s32 i;
    s32 j;
    u8 *row;
    pad_setStickAsDpad(1);
    i = 0;
    row = g_btluiSavedRepeat[0];
    for (; i < 2; i++, row += 0x18) {
        for (j = 0; j < 0x17; j++) {
            row[j] = pad_isRepeatEnabled(i, j);
        }
        pad_setRepeatEnabled(i, 2, 0);
        pad_setRepeatEnabled(i, 3, 0);
        pad_setRepeatTiming(i, 0xE, 0xF, 0xA);
        pad_setRepeatEnabled(i, 0xE, 1);
        pad_clearRepeats(i);
    }
    g_btluiSecondPort = btlui_anyOnSecondPort();
}

void btlui_restoreKeyRepeat(void) {
    s32 i, j;
    u8 *p;
    for (i = 0, p = g_btluiSavedRepeat[0]; i < 2; i++) {
        for (j = 0; j < 0x17; j++) {
            pad_setRepeatEnabled(i, j, p[j]);
        }
        p += 0x18;
    }
    pad_setStickAsDpad(0);
}

void btlui_readPad(void) {
    pad_update();
    pad_updateRepeat(g_btluiState.port);
    if ((g_padRepeat & 8) && g_btluiState.pauseReq == 0) {
        g_btluiState.pauseReq = 1;
    }
}

u8 btlui_anyOnSecondPort(void) {
    s32 i;
    s32 f = (g_gameState->config >> CFG_BIT_BTL_CONTROL) & 1;
    if (f != 1) {
        return 0;
    }
    for (i = 0; i < 4; i++) {
        Character *c = g_gameState->gameData->party[i];
        if (c == 0) {
            continue;
        }
        if (((s32)(g_gameState->config >> CFG_SHIFT_CHAR_PORT) >> chr_getDefaultSet(c)) & 1) {
            return 1;
        }
    }
    return 0;
}

void btlui_initPrims(void) {
    s32 i;
    g_btluiPrims = btlui_alloc(0x368);
    for (i = 0; i < 2; i++) {
        btlui_initCachedCmdIcons(i);
        btlui_initCachedHeader(i);
        btlui_initPanelPrims(i);
    }
}

void btlui_onReset(void) {
}

void btlui_initDrawBufs(void) {
    u32 *a = btlui_alloc(0xC0);
    s32 b = (s32)btlui_alloc(0x4000);
    DRAWENV *e;
    gfx_init(a, a + 0x18, b, b + 0x2000);
    gfx_setUiMode(2);
    gfx_setBufferIndex(g_gameState->gameData->drawBuf);
    g_btluiBuf = 0;
    g_ot = g_drawBufs.ots[0];
    g_primPtr = g_drawBufs.prims[0];
    e = g_gameState->drawEnv;
    e[1].dfe = 1;
    e[0].dfe = 1;
    g_btluiDispOrigin[0][0] = g_gameState->dispEnv[0].disp.x;
    g_btluiDispOrigin[0][1] = g_gameState->dispEnv[0].disp.y;
    g_btluiDispOrigin[1][0] = g_gameState->dispEnv[1].disp.x;
    g_btluiDispOrigin[1][1] = g_gameState->dispEnv[1].disp.y;
    ClearOTagR(g_drawBufs.ots[0], 0x18);
    ClearOTagR(g_drawBufs.ots[1], 0x18);
    DrawSync(0);
}

INCLUDE_RODATA("asm/us/ovl_0b/nonmatchings/btlui", s_btluiCmdIconsSrc);

INCLUDE_RODATA("asm/us/ovl_0b/nonmatchings/btlui", s_btluiRowsSrcA);

INCLUDE_RODATA("asm/us/ovl_0b/nonmatchings/btlui", s_btluiRowsSrcB);
