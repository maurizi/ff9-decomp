#include "ovl_0b.h"

void btlui_vsync(void) {
    GameData *gd;
    g_btluiState.inVsync = 1;
    btlui_putDispEnvIfReady();
    btlui_onVsync();
    btlui_readPad();
    if (g_btluiState.frameReady != 0) {
        btlui_frameUpdate();
    }
    gd = g_gameState->gameData;
    if (!(gd->flags & GD_PAUSED) && !(gd->ctx.battle->flags & BTL_FLAG_DISC_COVER_OPEN)) {
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
    void *names = db_lookupRegistered(DB_GET_ADDR, BTL_COMMON_NAME_GLYPHS, 0);
    if (names != NULL) {
        text_uploadPartyNames((s32)names);
    }
}

void btlui_loadPortraits(void) {
    void *portraits = db_lookupRegistered(DB_GET_ADDR, BTL_COMMON_PORTRAITS, 0);
    if (portraits != NULL) {
        ui_loadPartyPortraits(portraits);
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
    BtlUnit *unit = NULL;
    if (g_btlUi->menuState != BTLUI_MENU_NONE) {
        unit = g_btlUi->cmdUnit;
    }
    return unit;
}

s32 btlui_isEnabled(void) {
    return g_btluiState.enabled != 0;
}

s32 btlui_isTargetSelect(void) {
    return PSX_NULLABLE(g_btlUi)->menuState == BTLUI_MENU_TARGET;
}

u8 btlui_isMultiTarget(void) {
    return g_btluiState.multiTarget;
}

u8 btl_isAtbRunning(void) {
    s32 atbWait;
    if (g_btluiState.atbOn == 0) {
        return 0;
    }
    atbWait = (g_gameState->config >> CFG_BIT_ATB) & 1;
    if (atbWait != 1) {
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
    u8 drawBuf = g_gameState->gameData->drawBuf;
    g_btluiState.skipFrames = 2;
    g_drawBufs.cur = drawBuf;
}

void btlui_updateTargetFlags(void) {
    GameState *gs = g_gameState;
    s32 show = 0;
    volatile Unk80104E68 *state = &g_btluiState;
    s32 winCfg;
    if (((gs->gameData->ctx.battle->cmdFlags >> BTL_CMDF_MARKERS_BIT) ^ 1) & 1) {
        show = 1;
    } else {
        do {
            winCfg = (gs->config >> CFG_BIT_TARGET_WINDOW) & 1;
            if (winCfg == 1) {
                show = 1;
            }
        } while (0);
    }
    state->targetWindow = show;
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
    BtlUnit *unit = g_gameState->gameData->ctx.battle->unitList;
    Unk80104F88Entry *entry = g_btlUi->entries;
    s8 count = 0;
    s8 slot;
    s8 port;
    while (unit != NULL && (unit->flags & BTL_UNIT_PARTY)) {
        unit->menu = entry;
        unit->inMenuQueue = 0;
        slot = (u8)unit->flags >> BTL_UNIT_SLOT_SHIFT;
        entry->unit = unit;
        entry->slot = slot;
        entry->chr = &g_gameState->gameData->characters[BTL_UNIT_INDEX(unit->flags)];
        entry->learns = chr_canLearnAbilities(entry->chr);
        entry->defaultSet = chr_getAppearanceType(entry->chr->info.b.id, (entry->chr->classFlags >> 4) & 1);
        if ((g_gameState->config >> CFG_BIT_BTL_CONTROL) & 1) {
            port = ((s32)(g_gameState->config >> CFG_SHIFT_CHAR_PORT) >> entry->defaultSet) & 1;
        } else {
            port = 0;
        }
        entry->port = port;
        btlui_buildAbilityList(entry);
        btlui_clearCursor(&entry->cursor);
        count++;
        entry++;
        unit = unit->next;
    }
    g_btlUi->partyCount = count;
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
        for (i = 0; i < EQUIP_SLOT_COUNT; i++) {
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
    BtlUnit *unit;
    BtlUnit28 *stats;
    BtlUnit24 *maxStats;

    unit = g_gameState->gameData->ctx.battle->unitList;
    if (btl_isAtbRunning() == 0) {
        return;
    }
    for (; unit != NULL; unit = unit->next) {
        if (unit->cmdIssued != 0 || unit->inMenuQueue != 0 || unit->hp == 0) {
            continue;
        }
        if (!((unit->flags >> BTL_UNIT_ATB_ACTIVE_BIT) & 1)) {
            continue;
        }
        if ((unit->flags & BTL_UNIT_PARTY) == g_btlUi->frozenSide) {
            continue;
        }
        stats = (BtlUnit28 *)&unit->hp;
        maxStats = unit->max;
        stats->atb += stats->atbSpeed;
        if (stats->atb < maxStats->maxAtb) {
            continue;
        }
        stats->atb = maxStats->maxAtb;
        if ((u8)btl_hasStatus(unit, (BTL_STATUS_VENOM | BTL_STATUS_SLEEP | BTL_STATUS_FREEZE)) != 0) {
            continue;
        }
        if (unit->flags & BTL_UNIT_PARTY) {
            if ((u8)btl_hasStatus(unit, (BTL_STATUS_CONFUSE | BTL_STATUS_BERSERK)) != 0) {
                unit->cmdIssued = 1;
                btl_autoAttack(unit);
            } else {
                btlui_queueMenuUnit(unit);
            }
        } else if (evt_battleTrigger(BTL_CMD_ENEMY_TURN, unit->unitBit, 0, 0) != 0) {
            unit->cmdIssued = 1;
        }
    }
}

void btlui_checkSwdMagPartner(void) {
    BtlUnit *unit;
    s32 slot;

    unit = g_gameState->gameData->ctx.battle->unitList;
    g_btlUi->swdMagPartner = 0;
    g_btlUi->swdMagBlocked.b[0] = 0;
    g_btlUi->swdMagBlocked.b[1] = 0;
    for (; unit != NULL; unit = unit->next) {
        if (!(unit->flags & BTL_UNIT_PARTY)) {
            break;
        }
        slot = BTL_UNIT_INDEX(unit->flags);
        if (slot == 1) {
            g_btlUi->swdMagPartner = 1;
            if (unit->hp == 0 || (u8)btl_hasStatus(unit, (BTL_STATUS_PETRIFY | BTL_STATUS_VENOM | BTL_STATUS_SILENCE | BTL_STATUS_KO | BTL_STATUS_CONFUSE | BTL_STATUS_BERSERK | BTL_STATUS_STOP | BTL_STATUS_SLEEP | BTL_STATUS_HEAT | BTL_STATUS_FREEZE | BTL_STATUS_MINI)) != 0) {
                g_btlUi->swdMagBlocked.b[0] = 1;
            }
        } else if (slot == 3 && (u8)btl_hasStatus(unit, BTL_STATUS_MINI) != 0) {
            g_btlUi->swdMagBlocked.b[1] = 1;
        }
    }
}

BtlUnit *btlui_popMenuUnit(void) {
    BtlUnit *unit;
    for (;;) {
        if (g_btlUi->queueCount == 0 || g_btlUi->menuState != BTLUI_MENU_NONE) {
            return NULL;
        }
        unit = g_btlUi->queue[g_btlUi->queueHead];
        g_btlUi->queueCount--;
        g_btlUi->queueHead = (g_btlUi->queueHead + 1) & 3;
        if (BtlUnitCanAct(unit)) {
            return unit;
        }
        unit->inMenuQueue = 0;
    }
}

void btlui_pruneMenuQueue(void) {
    BtlUnit *list[4];
    u8 seen[4];
    s32 keepCount = 0;
    s32 head = g_btlUi->queueHead;
    s32 cnt = g_btlUi->queueCount;
    BtlUnit *unit;
    s32 i;
    memset(seen, 0, sizeof(seen));
    while (cnt != 0) {
        unit = g_btlUi->queue[head];
        if (BtlUnitCanAct(unit) && unit->cmdIssued != 1) {
            list[keepCount++] = unit;
        } else {
            unit->inMenuQueue = 0;
        }
        cnt--;
        head = (head + 1) & 3;
    }
    if (keepCount != g_btlUi->queueCount) {
        btlui_clearMenuQueue_inline();
        for (i = 0; i < keepCount; i++) {
            if (seen[(u8)list[i]->flags >> BTL_UNIT_SLOT_SHIFT] == 0) {
                seen[(u8)list[i]->flags >> BTL_UNIT_SLOT_SHIFT] = 1;
                btlui_queueMenuUnit_inline(list[i]);
            }
        }
    }
}

void btlui_initMenu(void) {
    s32 i;
    Unk80104F88 *ui = btlui_alloc(sizeof(Unk80104F88));
    g_btlUi = ui;
    ui->menuState = BTLUI_MENU_NONE;
    ui->cmdUnit = NULL;
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
        BtlUnit *unit = btlui_popMenuUnit();
        g_btlUi->cmdUnit = unit;
        if (unit != NULL) {
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
    btlui_clearMenuQueue_inline();
}

void btlui_flushMenuQueue(void) {
    s32 remaining = g_btlUi->queueCount;
    s32 i = g_btlUi->queueHead;
    while (remaining != 0) {
        remaining--;
        g_btlUi->queue[i]->inMenuQueue = 0;
        i = (i + 1) & 3;
    }
    btlui_clearMenuQueue_inline();
}

void btlui_queueMenuUnit(BtlUnit *unit) {
    btlui_queueMenuUnit_inline(unit);
}

void btlui_resetMenuQueue(void) {
    BtlUnit *unit = g_gameState->gameData->ctx.battle->unitList;
    btlui_clearMenuQueue();
    while (unit != NULL) {
        if (unit->flags & BTL_UNIT_PARTY) {
            unit->inMenuQueue = 0;
        }
        unit = unit->next;
    }
}

void btlui_closeMenu(void) {
    BtlUnit *unit = g_btlUi->cmdUnit;
    if (unit != NULL) {
        unit->inMenuQueue = 0;
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
            pad_setRepeatEnabled(i, PAD_RPT_LEFT, 0);
            pad_setRepeatEnabled(i, PAD_RPT_RIGHT, 0);
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
        Unk80104F88Entry *entry = &g_btlUi->entries[i];
        for (j = 0; j < ABILITY_SUPPORT_FIRST; j++) {
            entry->apCounters[j] = 0;
            entry->apNeeded[j] = 0;
            entry->fromEquip[j] = 0;
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
    memset((s32)entry->fromEquip, 0, sizeof(entry->fromEquip));
    for (i = 0; i < EQUIP_SLOT_COUNT; i++) {
        u8 item = chr->equip[i];
        if (item != ITEM_NONE) {
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

/* Same logic as BtlUnitCanAct (ovl_0b.h). Written out because a call of the inline schedules the prologue
   differently. */
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
    BtlUnit *unit;
    u8 req;
    for (unit = g_gameState->gameData->ctx.battle->unitList; unit != 0; unit = unit->next) {
        req = unit->targetableReq;
        if (req >= BTL_TGTREQ_SET_OFF) {
            if (req == BTL_TGTREQ_SET_OFF) {
                unit->targetableReq = BTL_TGTREQ_OFF;
                unit->flags &= ~BTL_UNIT_ATB_ACTIVE;
                unit->flags &= ~BTL_UNIT_TARGETABLE;
                if (g_btlUi->menuState == BTLUI_MENU_TARGET) {
                    btlui_onUnitUntargetable(unit);
                }
            } else if (req == BTL_TGTREQ_SET_ON) {
                unit->targetableReq = BTL_TGTREQ_ON;
                unit->flags |= BTL_UNIT_ATB_ACTIVE;
                unit->flags |= BTL_UNIT_TARGETABLE;
                if (g_btlUi->menuState == BTLUI_MENU_TARGET) {
                    btlui_onUnitTargetable(unit);
                }
            }
        }
    }
}

void btlui_pollEscape(void) {
    Unk80104E68 *state = (Unk80104E68 *)&g_btluiState;
    if (state->atbOn != 0) {
        if ((g_padHeld & (BTN_L1 | BTN_R1)) == (BTN_L1 | BTN_R1)) {
            g_gameState->gameData->ctx.battle->escapeHeld = 1;
            if (state->escapeFrames++ >= BTLUI_ESCAPE_HOLD_FRAMES) {
                state->escapeFrames = 0;
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
    s32 ambush = g_btlUi->ambush;
    if (ambush != 0) {
        return (ambush != 1) * 2;
    }
    return 1;
}

void btlui_onCmdWindowClosed(Window *win) {
    g_btlUi->menuState = BTLUI_MENU_NONE;
}

void btlui_drawHelpWindow(void) {
    RECT rect;
    s32 savedColor;
    if (g_btluiHelp.open != 0 && g_btluiPrevAtbOn != 0) {
        btlui_getHelpRect(&g_btluiHelpRect, &rect);
        win_setClip(NULL, 2, WIN_STYLE_FRAMELESS);
        if (g_btluiHelp.opening == 0) {
            ui_drawSprite(rect.x + 8, rect.y, 2, UI_SPRITE_HELP_TAG, 0);
        }
        btlui_updateHelpText();
        if (g_btluiHelp.text != 0) {
            win_setClip(NULL, 2, WIN_STYLE_BALLOON);
            savedColor = text_getColor();
            text_drawBlock(rect.x + 6, rect.y + 10, 2, g_btluiHelp.text, TEXT_PAL_HELP);
            text_setColor(savedColor);
            win_setClip(&rect, 2, WIN_STYLE_BALLOON);
        }
        win_drawHelpFrame(&rect, 2, HELP_TAIL_NONE, 0);
        win_setClip(&rect, 2, WIN_STYLE_FRAMELESS);
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
    if (g_padRepeat & BTN_SELECT) {
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
        g_btluiHelp.text = (s32)text_get(BTL_TEXT_ESCAPE);
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
        if (itemId < WEAPON_COUNT && (g_weaponTable[itemId].flags & WEAPON_THROWABLE)) {
            i = btlui_findThrowListEntry(itemId);
            if (i != 0xFF && g_btluiItems->throwItems[i]->count == 0) {
                g_btluiItems->throwItems[i] = 0;
                g_btluiItems->throwCount--;
            }
        }
    }
}

void btlui_initItemLists(void) {
    Unk80104FA8 *lists = btlui_alloc(sizeof(Unk80104FA8));
    g_btluiItems = lists;
    memset((s32)lists->reserved, 0, sizeof(lists->reserved));
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
    if (itemId < WEAPON_COUNT && (g_weaponTable[itemId].flags & WEAPON_THROWABLE) && btlui_findThrowListEntry(itemId) == 0xFF) {
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
    s32 count = 0;
    s32 i;
    UnkPair *slot = g_gameState->gameData->items.slots;
    Unk80104FA8 *lists = g_btluiItems;
    for (i = 0; i < ITEM_SLOT_COUNT; i++, slot++) {
        if (slot->count != 0 && slot->id != ITEM_NONE && slot->id >= ITEM_FIRST_USABLE) {
            lists->items[count] = slot;
            count++;
        }
    }
    for (i = count; i < 31; i++) {
        g_btluiItems->items[i] = NULL;
    }
    g_btluiItems->itemCount = count;
}

void btlui_buildThrowList(void) {
    s32 count = 0;
    s32 i = 0;
    UnkPair *slot;
    Unk80104FA8 *lists = g_btluiItems;
    slot = g_gameState->gameData->items.slots;
    for (; i < ITEM_SLOT_COUNT; i++, slot++) {
        if (slot->count != 0 && slot->id != ITEM_NONE && slot->id < WEAPON_COUNT && (g_weaponTable[slot->id].flags & WEAPON_THROWABLE)) {
            lists->throwItems[count] = slot;
            count++;
        }
    }
    for (i = count; i < WEAPON_COUNT; i++) {
        g_btluiItems->throwItems[i] = NULL;
    }
    g_btluiItems->throwCount = count;
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
    Unk80104FA8 *lists = g_btluiItems;
    for (i = 0; i < 31; i++) {
        if (lists->items[i] != NULL && itemId == lists->items[i]->id) {
            return i;
        }
    }
    return 0xFF;
}

s32 btlui_findThrowListEntry(s32 itemId) {
    s32 i;
    Unk80104FA8 *lists = g_btluiItems;
    for (i = 0; i < 0x58; i++) {
        if (lists->throwItems[i] != NULL && itemId == lists->throwItems[i]->id) {
            return i;
        }
    }
    return 0xFF;
}

void btlui_openCmdMenu(void) {
    Unk80104F88Entry *entry;
    Window *w;
    Unk80104FC0 *sideCmd;

    sideCmd = &g_btlCmdMenu.sideCmd;
    entry = g_btlUi->cmdUnit->menu;
    if (!((g_gameState->config >> CFG_BIT_CURSOR) & 1)) {
        g_btlCmdMenu.cursor = 0;
    } else {
        g_btlCmdMenu.cursor = entry->cursor.cmdRow;
    }
    g_btlCmdMenu.trance = (u8)btl_hasStatus(g_btlUi->cmdUnit, BTL_STATUS_TRANCE) != 0;
    g_btlCmdMenu.charIndex = entry->chr->info.b.row;
    g_btlCmdMenu.unit = g_btlUi->cmdUnit;
    sideCmd->side = BTLUI_SIDE_NONE;
    sideCmd->slide = 0;
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
    s32 y = win->y + BTLUI_CMD_TEXT_Y;
    s32 x = win->x + BTLUI_CMD_TEXT_X;
    s32 i;
    u8 *name;
    if (g_btlCmdMenu.sideCmd.side != BTLUI_SIDE_NONE) {
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
            y += BTLUI_LIST_ROW_H;
        } while (0);
    }
}

INCLUDE_RODATA("asm/us/ovl_0b/nonmatchings/btlui", g_battleText);

/* TRANCE label */
INCLUDE_RODATA("asm/us/ovl_0b/nonmatchings/btlui", s_btluiTranceLabel);

void btlui_drawCmdOverlay(Window *win) {
    u8 st = win->state;
    RECT *rect = &win->cur;
    s32 dy = 10;
    RECT rc;
    if (win->state == WIN_STATE_OPEN) {
        if (btlui_getMenuState() == BTLUI_MENU_CMD || (btlui_getMenuState() == st && btlui_getTargetReturnState() == BTLUI_MENU_CMD && !(btlui_isTargetWindowOpen() & 0xFF) && (g_btluiVsyncTick & 1))) {
            ui_drawHand(win->cur.x - 0xE, rect->y + (g_btlCmdMenu.cursor * BTLUI_LIST_ROW_H + dy), win->otz - 1);
        }
    }
    if (win->state != WIN_STATE_OPEN) {
        win_setClip(NULL, win->otz, WIN_STYLE_FRAMELESS);
    }
    if (g_btlCmdMenu.unit != NULL && !(btl_hasStatus(g_btlCmdMenu.unit, BTL_STATUS_TRANCE) & 0xFF)) {
        ui_drawLabel(win->x + WIN_TITLE_X, rect->y + WIN_TITLE_Y, win->otz, g_winLabelStrings.command, win->active ? UI_LABEL_NORMAL : UI_LABEL_DIM);
    } else if (win->active != 0) {
        btlui_drawTranceLabel(win->x + 6, rect->y - 3, win->otz, s_btluiTranceLabel);
    } else {
        ui_drawLabel(win->x + WIN_TITLE_X, rect->y + WIN_TITLE_Y, win->otz, (s32)s_btluiTranceLabel, UI_LABEL_DIM);
    }
    if (win->state != WIN_STATE_OPEN) {
        rc.x = rect->x;
        rc.y = rect->y - 3;
        rc.w = rect->w;
        rc.h = 8;
        win_setClip(&rc, win->otz, WIN_STYLE_FRAMELESS);
    }
}

void btlui_drawSideCmd(Window *win) {
    s32 dx2;
    RECT rect;
    s32 off = 0;
    s32 dx;
    s16 y;
    s32 x;
    Unk80104FC0 *sideCmd = &g_btlSideCmd;
    Window *w = win;

    dx2 = 6;
    if (sideCmd->slide == BTLUI_SLIDE_OUT) {
        off = (BTLUI_SIDE_SLIDE_FRAMES - sideCmd->slideFrame) * 3;
    } else if (sideCmd->slide == BTLUI_SLIDE_BACK) {
        off = sideCmd->slideFrame * 5;
    }
    dx = 10;
    if (sideCmd->side == BTLUI_SIDE_LEFT) {
        dx = dx2;
        x = off + 4;
        rect.x = w->x + x;
    } else {
        rect.x = w->x - off;
    }
    y = w->y + g_btlCmdMenu.cursor * BTLUI_LIST_ROW_H;
    rect.w = BTLUI_SIDE_W;
    rect.h = BTLUI_SIDE_H;
    rect.y = y + 4;
    text_draw(rect.x + dx, rect.y + 2, win->otz, text_get(g_btluiSideCmdTexts[g_btlSideCmd.side]), TEXT_PAL_WHITE);
    btlui_drawSideFrame(&rect, win->otz);
}

void btlui_confirmCmd(void) {
    Unk800FD56C *cmd = &g_btlCmdTable[g_btlCmdMenu.cmdIds[g_btlCmdMenu.cursor]];
    void (*confirmFn)(Unk800FD56C *);
    if (g_btlCmdMenu.sideCmd.side == BTLUI_SIDE_NONE) {
        if (g_btlCmdMenu.enabled[g_btlCmdMenu.cursor] != 0) {
            do {
                snd_playSe(SE_CURSOR);
                btlui_saveCursor(&g_btlCmdMenu.unit->menu->cursor, 0, g_btlCmdMenu.cursor, 0);
                confirmFn = g_btluiCmdConfirmFns[cmd->kind];
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
        confirmFn = g_btluiSideCmdConfirmFn;
    }
    confirmFn(cmd);
}

void btlui_buildCmdList(void) {
    s32 k = g_btlCmdMenu.charIndex;
    s8 inTrance;
    g_btlCmdMenu.trance = inTrance = (u8)btl_hasStatus(g_btlCmdMenu.unit, BTL_STATUS_TRANCE) != 0;
    g_btlCmdMenu.cmdIds[0] = BTL_CMD_ATTACK;
    g_btlCmdMenu.enabled[0] = 1;
    g_btlCmdMenu.cmdIds[3] = BTL_CMD_ITEM;
    g_btlCmdMenu.enabled[3] = 1;
    g_btlCmdMenu.tranceRow[3] = 0;
    g_btlCmdMenu.tranceRow[2] = 0;
    g_btlCmdMenu.tranceRow[1] = 0;
    g_btlCmdMenu.tranceRow[0] = 0;
    if (inTrance == 0) {
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
    RECT srcRect = s_btluiCmdIconsSrc;
    s32 x = VRAM_BTLUI_CACHE_X;
    s32 y = 0x1E0;
    s32 i = 0;
    srcRect.x += g_btluiDispOrigin[g_gameState->gameData->drawBuf & 1][0];
    srcRect.y += g_btluiDispOrigin[g_gameState->gameData->drawBuf & 1][1];
    do {
        SetDrawMove((DR_MOVE *)g_primPtr, &srcRect, x, y);
        x += 0x10;
        i++;
        addPrim(&g_ot[19], g_primPtr);
        g_primPtr += sizeof(DR_MOVE);
        srcRect.y += 0x10;
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

void btlui_drawSideFrame(RECT *rect, s32 otz) {
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
    ((SprtWords *)g_primPtr)->xy = rect->x | (rect->y << 16);
    w = g_sideFrameSprites[0].u;
    w |= g_sideFrameSprites[0].v << 8;
    ((SprtWords *)g_primPtr)->uvclut = w | clut;
    ((SprtWords *)g_primPtr)->wh = g_sideFrameSprites[0].w | (g_sideFrameSprites[0].h << 16);
    addPrim(ot, g_primPtr);
    g_primPtr += sizeof(SprtWords);

    setlen(g_primPtr, PRIM_LEN(SPRT));
    ((SprtWords *)g_primPtr)->rgbc = GPU_RGBC(GPU_SPRT, GPU_RGB_NEUTRAL);
    ((SprtWords *)g_primPtr)->xy = (rect->x + rect->w - d2->w) | (rect->y << 16);
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
    ((SprtWords *)g_primPtr)->xy = (rect->x + g_sideFrameSprites[0].w) | (rect->y << 16);
    ((SprtWords *)g_primPtr)->uvclut = (d1->v << 8) | clut;
    ((SprtWords *)g_primPtr)->wh = (rect->w - (g_sideFrameSprites[0].w + d2->w)) | (d1->h << 16);
    addPrim(ot, g_primPtr);
    g_primPtr += sizeof(SprtWords);

    setlen(g_primPtr, PRIM_LEN(DR_MODE));
    ((DrMode2Words *)g_primPtr)->code[0] = get_mode(1, 1, d1->tpage);
    ((DrMode2Words *)g_primPtr)->code[1] = get_tw(&tw);
    addPrim(ot, g_primPtr);
    g_primPtr += sizeof(DrMode2Words);

    r2.x = rect->x + 1;
    r2.y = rect->y + 1;
    r2.w = rect->w - 3;
    r2.h = rect->h - 3;
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

void btlui_drawPulseGradient(s32 x, s32 y, s32 ot, s32 w, s32 h, CVECTOR *colors) {
    CVECTOR c0;
    CVECTOR c1;
    CVECTOR *baseTop;
    CVECTOR *baseBottom;
    CVECTOR *peakBottom;
    CVECTOR *peakTop;
    s32 k;
    POLY_G4 *prim;

    k = g_winLastVsync & 0x1F;
    if (k < 16) {
        if (k >= 8) {
            k = 15 - k;
        }
        baseTop = &colors[2];
        baseBottom = &colors[3];
        peakTop = peakBottom = &colors[0];
    } else {
        k -= 16;
        if (k >= 8) {
            k = 15 - k;
        }
        baseTop = &colors[2];
        baseBottom = &colors[3];
        peakTop = peakBottom = &colors[1];
    }
    c0.r = baseTop->r + ((peakTop->r - baseTop->r) * k) / 7;
    c0.g = baseTop->g + ((peakTop->g - baseTop->g) * k) / 7;
    c0.b = baseTop->b + ((peakTop->b - baseTop->b) * k) / 7;
    c1.r = baseBottom->r + ((peakBottom->r - baseBottom->r) * k) / 7;
    c1.g = baseBottom->g + ((peakBottom->g - baseBottom->g) * k) / 7;
    c1.b = baseBottom->b + ((peakBottom->b - baseBottom->b) * k) / 7;
    prim = (POLY_G4 *)g_primPtr;
    setPolyG4(prim);
    setRGB0(prim, c0.r, c0.g, c0.b);
    setRGB2(prim, c0.r, c0.g, c0.b);
    setRGB1(prim, c1.r, c1.g, c1.b);
    setRGB3(prim, c1.r, c1.g, c1.b);
    setXYWH(prim, x, y, w, h);
    addPrim(&g_ot[ot], prim);
    g_primPtr += sizeof(POLY_G4);
}

void btlui_cmdInput(void) {
    if (g_padRepeat & BTN_TRIANGLE_ALT) {
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
    Unk800FD56C *cmd;
    if (g_btlCmdMenu.sideCmd.side != BTLUI_SIDE_NONE && g_btlCmdMenu.sideCmd.ready != 0) {
        i = 7;
        if (g_btlCmdMenu.sideCmd.side == BTLUI_SIDE_RIGHT) {
            i = 4;
        }
    } else {
        i = g_btlCmdMenu.cmdIds[g_btlCmdMenu.cursor];
    }
    cmd = &g_btlCmdTable[i];
    text_copy(g_btluiHelpText, cmd->helpOfs + g_btlCmdHelp);
    text_append(g_btluiHelpText, text_get(BTL_TEXT_HELP_CMD));
    return g_btluiHelpText;
}

void btlui_onCmdMenuOpened(void) {
    s32 port = g_btlCmdMenu.unit->menu->port;
    btlui_setMenuState(BTLUI_MENU_CMD);
    if (port != g_btluiState.port) {
        pad_resetRepeats(port);
    }
    g_btluiState.port = port;
}

void btlui_cmdCursorUp(void) {
    if (g_btlCmdMenu.sideCmd.side == BTLUI_SIDE_NONE) {
        do {
            snd_playSe(SE_CURSOR);
            g_btlCmdMenu.cursor = g_btlCmdMenu.cursor != 0 ? g_btlCmdMenu.cursor - 1 : 3;
        } while (g_btlCmdMenu.cmdIds[g_btlCmdMenu.cursor] == 0);
    }
}

void btlui_cmdCursorDown(void) {
    if (g_btlCmdMenu.sideCmd.side == BTLUI_SIDE_NONE) {
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
    s32 cmdNo = g_btluiSideCmdIds[g_btlCmdMenu.sideCmd.side];
    g_btlCmdMenu.unit->inMenuQueue = 0;
    g_btlCmdMenu.unit->cmdIssued = 1;
    btl_setCmd((s32)g_btlCmdMenu.unit->cmd, cmdNo, g_btlCmdTable[cmdNo].first, g_btlCmdMenu.unit->unitBit, 0);
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
    if (g_btlSideCmd.side == BTLUI_SIDE_LEFT) {
        if (g_btlSideCmd.slide == BTLUI_SLIDE_OUT) {
            if (++g_btlSideCmd.slideFrame >= BTLUI_SIDE_SLIDE_FRAMES) {
                g_btlSideCmd.ready = 1;
                g_btlSideCmd.slide = 0;
            }
        } else if (g_btlSideCmd.slide == BTLUI_SLIDE_BACK && (g_padRepeat & PAD_LEFT)) {
            snd_playSe(SE_PAGE);
            g_btlSideCmd.slide = BTLUI_SLIDE_OUT;
            g_btlSideCmd.side = BTLUI_SIDE_LEFT;
            g_btlSideCmd.ready = 0;
            g_btlSideCmd.slideFrame = 0;
        }
    } else if (g_btlSideCmd.side == BTLUI_SIDE_NONE) {
        if (g_padRepeat & PAD_LEFT) {
            snd_playSe(SE_PAGE);
            g_btlSideCmd.slide = BTLUI_SLIDE_OUT;
            g_btlSideCmd.side = BTLUI_SIDE_LEFT;
            g_btlSideCmd.ready = 0;
            g_btlSideCmd.slideFrame = 0;
        }
    } else {
        btlui_slideBack();
    }
}

void btlui_slideRight(void) {
    if (g_btlSideCmd.side == BTLUI_SIDE_RIGHT) {
        if (g_btlSideCmd.slide == BTLUI_SLIDE_OUT) {
            if (++g_btlSideCmd.slideFrame >= BTLUI_SIDE_SLIDE_FRAMES) {
                g_btlSideCmd.ready = 1;
                g_btlSideCmd.slide = 0;
            }
        } else if (g_btlSideCmd.slide == BTLUI_SLIDE_BACK && (g_padRepeat & PAD_RIGHT)) {
            snd_playSe(SE_PAGE);
            g_btlSideCmd.slide = BTLUI_SLIDE_OUT;
            g_btlSideCmd.side = BTLUI_SIDE_RIGHT;
            g_btlSideCmd.ready = 0;
            g_btlSideCmd.slideFrame = 0;
        }
    } else if (g_btlSideCmd.side == BTLUI_SIDE_NONE) {
        if (g_padRepeat & PAD_RIGHT) {
            snd_playSe(SE_PAGE);
            g_btlSideCmd.slide = BTLUI_SLIDE_OUT;
            g_btlSideCmd.side = BTLUI_SIDE_RIGHT;
            g_btlSideCmd.ready = 0;
            g_btlSideCmd.slideFrame = 0;
        }
    } else {
        btlui_slideBack();
    }
}

void btlui_slideBack(void) {
    Unk80104FC0 *sideCmd = &g_btlCmdMenu.sideCmd;
    if (sideCmd->side != BTLUI_SIDE_NONE) {
        if (sideCmd->slide != BTLUI_SLIDE_BACK) {
            sideCmd->slide = BTLUI_SLIDE_BACK;
            sideCmd->slideFrame = 0;
        } else if (sideCmd->slideFrame++ >= BTLUI_SIDE_BACK_FRAMES) {
            sideCmd->side = BTLUI_SIDE_NONE;
            sideCmd->slideFrame = 0;
        }
        sideCmd->ready = 0;
    }
}

void btlui_openList(Unk800EE37C *req) {
    MenuList list;
    Unk80104F88Entry *entry;
    u8 count;
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
    count = g_btluiList.cmd->count;
    menuList_init(&list, count, (count < 5) ? count : 4, 1, 0x44);
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
    MenuList *list = (MenuList *)&g_btlUi->windows[2]->list;
    Window *w;
    if (list->scroll == 0) {
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
            menuList_up(list, g_padRepeat);
            menuList_down(list, g_padRepeat);
            if (g_gameState->gameData->ctx.battle->escapeHeld == 0) {
                menuList_pageUp(list, g_padRepeat);
                menuList_pageDown(list, g_padRepeat);
            }
        }
    }
}

s32 btlui_selectAbility(void) {
    Unk800EF80C req;
    MenuList *list = &g_btlUi->windows[2]->list;
    s32 index = list->cursor + list->top;
    u8 k = g_btlCmdAbilities[g_btluiList.cmd->first + index];
    s32 state = btlui_getAbilityState(k);
    if (state != 2) {
        return 0;
    }
    {
        req.returnState = state;
        req.cmdNo = g_btluiList.cmdNo;
        req.subNo = btlui_resolveAbility(k);
        req.targetType = g_gameState->gameData->ctx.battle->abilities[k].targetType;
        req.defaultSide = g_gameState->gameData->ctx.battle->abilities[k].defaultSide;
        req.koFirst = g_gameState->gameData->ctx.battle->abilities[k].koFirst;
        req.listKind = g_gameState->gameData->ctx.battle->abilities[k].listKind;
        req.allowKo = g_gameState->gameData->ctx.battle->abilities[k].allowKo;
        req.doubleCast = g_btluiList.doubleCast;
        req.trance = g_btluiList.trance;
        btlui_startTargetSelect(&req);
        return 1;
    }
}

s32 btlui_selectItem(void) {
    Unk800EF80C req;
    MenuList *list = &g_btlUi->windows[2]->list;
    UnkPair *slot = btlui_getItemListEntry(list->cursor + list->top);
    u8 ok = 0;
    Unk8006A774 *info = &g_usableItemTableById[PSX_NULLABLE(slot)->id];
    s32 count;

    if (slot == NULL) {
        return 0;
    }
    if ((count = slot->count) - btlui_getItemReserved(slot->id) >= 0 && count != btlui_getItemReserved(slot->id)) {
        req.returnState = 2;
        req.cmdNo = g_btluiList.cmdNo;
        req.subNo = slot->id;
        req.targetType = info->targetType;
        req.defaultSide = info->defaultSide;
        req.koFirst = info->koFirst;
        req.listKind = info->listKind;
        req.allowKo = info->allowKo;
        req.doubleCast = g_btluiList.doubleCast;
        req.trance = g_btluiList.trance;
        btlui_startTargetSelect(&req);
        ok = 1;
    }
    return ok;
}

s32 btlui_selectThrow(void) {
    Unk800EF80C req;
    MenuList *list = &g_btlUi->windows[2]->list;
    u8 ok = 0;
    UnkPair *slot = btlui_getThrowListEntry(list->cursor + list->top);
    Battle16CC *ability = &g_gameState->gameData->ctx.battle->abilities[g_btluiList.cmd->first];
    s32 count;

    if (slot == NULL) {
        return 0;
    }
    if ((count = slot->count) - btlui_getItemReserved(slot->id) >= 0 && count != btlui_getItemReserved(slot->id)) {
        req.returnState = 2;
        req.cmdNo = g_btluiList.cmdNo;
        req.subNo = slot->id;
        req.targetType = ability->targetType;
        req.defaultSide = ability->defaultSide;
        req.koFirst = ability->koFirst;
        req.listKind = ability->listKind;
        req.allowKo = ability->allowKo;
        req.doubleCast = g_btluiList.doubleCast;
        req.trance = g_btluiList.trance;
        btlui_startTargetSelect(&req);
        ok = 1;
    }
    return ok;
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
    RECT *rect = &win->cur;
    s32 x;
    s32 y;
    s32 dy = 10;
    s32 dx = 0x74;
    if (g_btluiList.label != 0) {
        ui_drawLabel(win->cur.x + 6, rect->y - 4, win->otz, g_btluiList.label, UI_LABEL_NORMAL);
    }
    if (g_btluiList.label2 != 0) {
        ui_drawLabel(win->cur.x - (g_btluiList.label2Width - dx), rect->y - 4, win->otz, g_btluiList.label2, UI_LABEL_NORMAL);
    }
    if (btlui_getMenuState() == BTLUI_MENU_LIST || (btlui_getMenuState() == BTLUI_MENU_TARGET && btlui_getTargetReturnState() == BTLUI_MENU_LIST && !(btlui_isTargetWindowOpen() & 0xFF) && (g_btluiVsyncTick & 1))) {
        x = win->cur.x - 0x12;
        y = rect->y + (win->list.cursor * BTLUI_LIST_ROW_H + dy);
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
    s32 text = 0;
    s32 (*helpFn)(void) = g_btluiListHelpFns[g_btluiList.cmd->kind];
    if (helpFn != NULL) {
        text = helpFn();
    }
    return text;
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
    RECT rowsRect;
    MenuView view;
    RECT *rc = &win->cur;
    s32 i;
    menuList_getView((MenuList *)&win->list, &view);
    win_setClip(0, 7, WIN_STYLE_LINE_RULE);
    for (i = 0; i < view.rows; i++) {
        btlui_drawListRow(0, i, view.first + i, &view);
    }
    if (((MenuList *)&win->list)->scroll & MENULIST_SCROLL_PAGE) {
        menuList_getRowsRect((MenuList *)&win->list, rc, &rowsRect);
        win_setClip(&rowsRect, 7, WIN_STYLE_LINE_RULE);
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
    RECT *rect = &w->cur;
    s32 dy = w->h - rect->h;
    s32 state = btlui_getAbilityState(ability);
    s16 x;
    s32 y;
    Battle16CC *info;
    s32 pal;

    if (state != 0) {
        x = rect->x;
        info = &g_gameState->gameData->ctx.battle->abilities[ability];
        y = rect->y + row * BTLUI_LIST_ROW_H + view->y - dy;
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
    RECT *rect = &w->cur;
    s32 dy = w->h - rect->h;
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
            x = rect->x;
            it = &g_itemTable[id];
            x5 = x + 5;
            y = rect->y + row * BTLUI_LIST_ROW_H + view->y - dy;
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
    RECT *rect = &w->cur;
    s32 dy = w->h - rect->h;
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
            x = rect->x;
            it = &g_itemTable[slot->id];
            y = rect->y + row * BTLUI_LIST_ROW_H + view->y - dy;
            text_formatNumber(count, digits, 2, TEXT_NUM_ASCII);
            text_draw(x + 5, y, 7, g_itemNames + it->name, TEXT_PAL_WHITE);
            ui_drawSmallDigits(x + 0x67, y + 4, 7, digits, TEXT_PAL_WHITE);
        }
    }
}

s32 btlui_resolveAbility(s32 ability) {
    s32 maidenPrayer;
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
        maidenPrayer = g_btluiList.unit->menu->chr->equip[4] == ITEM_MAIDEN_PRAYER;
        ability = maidenPrayer + ABIL_FENRIR; /* ABIL_FENRIR2 with ITEM_MAIDEN_PRAYER */
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
    MenuList *list = &g_btlUi->windows[2]->list;
    s32 index = list->cursor + list->top;
    u8 k = g_btlCmdAbilities[g_btluiList.cmd->first + index];
    if (btlui_getAbilityState(k) == 0) {
        return NULL;
    }
    return g_btlAbilityHelp + g_btlAbilityHelpOfs[k].text;
}

u8 *btlui_getItemHelpText(void) {
    Unk16 *w = &g_btlUi->windows[2]->list;
    UnkPair *slot = btlui_getItemListEntry(w->cursor + w->top);
    if (slot != NULL) {
        if (slot->count - btlui_getItemReserved(slot->id) >= 0) {
            if (slot->count != btlui_getItemReserved(slot->id)) {
                return g_btlItemHelp + g_btlItemHelpOfs[slot->id];
            }
            return NULL;
        }
    }
    return NULL;
}

u8 *btlui_getThrowHelpText(void) {
    Unk16 *w = &g_btlUi->windows[2]->list;
    UnkPair *slot = btlui_getThrowListEntry(w->cursor + w->top);
    if (slot != NULL) {
        if (slot->count - btlui_getItemReserved(slot->id) >= 0) {
            if (slot->count != btlui_getItemReserved(slot->id)) {
                return g_btlItemHelp + g_btlItemHelpOfs[slot->id];
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
        pad_setRepeatEnabled(i, PAD_RPT_LEFT, 1);
        pad_setRepeatEnabled(i, PAD_RPT_RIGHT, 1);
    }
}

void btlui_setDefaultTargets(s32 useDefault) {
    BtlUnit *unit;
    if (g_btlTarget.targetType == BTL_TGT_EVERYONE) {
        for (unit = g_gameState->gameData->ctx.battle->unitList; unit != 0; unit = unit->next) {
            if (((unit->flags >> BTL_UNIT_TARGETABLE_BIT) & 1) && (!(unit->status & BTL_STATUS_KO) || g_btlTarget.allowKo != 0)) {
                unit->flags |= BTL_UNIT_TARGETED;
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
    BtlUnit *unit;
    BtlUnit *target;
    s32 cur = 0;
    s32 count;
    s32 k;
    if (g_btlTarget.targetAll != 0) {
        return 0;
    }
    if (g_btlTarget.targetType == BTL_TGT_SELF) {
        return 0;
    }
    target = BtlFindTargetCursor();
    if (target == NULL) {
        return 0;
    }
    memset((s32)list, 0, sizeof(list));
    count = 0;
    for (unit = g_gameState->gameData->ctx.battle->unitList; unit != NULL; unit = unit->next) {
        if (((unit->flags >> BTL_UNIT_TARGETABLE_BIT) & 1) && (unit->flags & BTL_UNIT_PARTY) == g_btlTarget.side && (!(unit->status & BTL_STATUS_KO) || g_btlTarget.allowKo != 0)) {
            list[count] = unit;
            if (unit == target) {
                cur = count;
            }
            count++;
        }
    }
    if (count >= 2) {
        target->flags &= ~BTL_UNIT_TARGETED;
        if (!(forward & 0xFF)) {
            k = cur - 1;
            if (cur == 0) {
                k = count - 1;
            }
        } else {
            k = 0;
            if (cur < count - 1) {
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
    BtlUnit *sideUnit;
    BtlUnit *list[4];
    s32 count;
    s32 i;
    u8 side;
    s32 k;

    cur = BtlFindTargetCursor();
    /* cur stays NULL when no unit is both targetable and targeted (seen in d3_f1759's battle); the PS1 then reads
     * cur->flags, and clears BTL_UNIT_TARGETED in it below, through low RAM; a port guards it via
     * PSX_NULLABLE (ff9-ds F38). */
    if (g_btlTarget.canSwitchSide != 0) {
        if (newSide == (k = PSX_NULLABLE(cur)->flags & BTL_UNIT_PARTY)) {
            return 0;
        }
        side = !k;
        g_btlTarget.side = side;
        memset(list, 0, sizeof(list));
        count = 0;
        for (sideUnit = g_gameState->gameData->ctx.battle->unitList; sideUnit != NULL; sideUnit = sideUnit->next) {
            if (((sideUnit->flags >> BTL_UNIT_TARGETABLE_BIT) & 1) && (sideUnit->flags & BTL_UNIT_PARTY) == side) {
                if (!(sideUnit->status & BTL_STATUS_KO) || g_btlTarget.allowKo) {
                    list[count++] = sideUnit;
                }
            }
        }
        if (count != 0) {
            PSX_NULLABLE(cur)->flags &= ~BTL_UNIT_TARGETED;
            if (g_btlTarget.targetAll == 0) {
                list[0]->flags |= BTL_UNIT_TARGETED;
            } else {
                btlui_clearTargeted();
                for (i = 0; i < count; i++) {
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
    BtlUnit *target;
    BtlUnit *unit;
    s32 count;
    if (g_btlTarget.canToggleAll != 0) {
        if (old != 0) {
            if ((g_btlTarget.prevTarget->flags >> BTL_UNIT_TARGETED_BIT) & 1) {
                target = g_btlTarget.prevTarget;
            } else {
                target = BtlFindTargetCursor();
            }
            btlui_clearTargeted();
            target->flags |= BTL_UNIT_TARGETED;
            g_btlTarget.targetAll = 0;
        } else {
            unit = g_gameState->gameData->ctx.battle->unitList;
            target = BtlFindTargetCursor();
            count = 0;
            g_btlTarget.prevTarget = target;
            btlui_clearTargeted();
            for (; unit != NULL; unit = unit->next) {
                if (((unit->flags >> BTL_UNIT_TARGETABLE_BIT) & 1) && (unit->flags & BTL_UNIT_PARTY) == (target->flags & BTL_UNIT_PARTY) && (!(unit->status & BTL_STATUS_KO) || g_btlTarget.allowKo != 0)) {
                    unit->flags |= BTL_UNIT_TARGETED;
                    count++;
                }
            }
            if (count != 0) {
                g_btlTarget.targetAll = 1;
            }
        }
        if (old != g_btlTarget.targetAll) {
            snd_playSe(SE_CURSOR);
        }
    }
}

s32 btlui_isCmdUsable(void) {
    Unk800FD56C *cmd = &g_btlCmdTable[g_btlTarget.cmdNo];
    Battle16CC *ability;
    u8 cost;
    if (g_btlTarget.cmdNo == BTL_CMD_SWD_MAG && (g_btlUi->swdMagPartner == 0 || g_btlUi->swdMagBlocked.h != 0)) {
        return 0;
    }
    if (g_btlTarget.trance == 0 && (btl_hasStatus(g_btlTarget.unit, BTL_STATUS_TRANCE) & 0xFF)) {
        return 0;
    }
    if (cmd->kind == 1) {
        ability = &g_gameState->gameData->ctx.battle->abilities[g_btlTarget.subNo];
        cost = ability->mpCost;
        if (g_btlTarget.halfMp != 0) {
            cost >>= 1;
        }
        if (g_btlTarget.unit->mp < cost) {
            return 0;
        }
        if ((ability->flags & BTL_ABIL_VOICE) && (btl_hasStatus(g_btlTarget.unit, BTL_STATUS_SILENCE) & 0xFF)) {
            return 0;
        }
    }
    if ((u32)(cmd->kind - 2) < 2 && item_getCount(g_btlTarget.subNo) - btlui_getItemReserved(g_btlTarget.subNo) <= 0) {
        return 0;
    }
    return 1;
}

s32 btlui_refreshTargets(void) {
    BtlUnit *target = BtlFindTargetCursor();
    BtlUnit *unit;
    s32 count;
    if (target == NULL) {
        btlui_setDefaultTargets(0);
        return 1;
    }
    if (g_btlTarget.targetAll == 0) {
        if ((target->status & BTL_STATUS_KO) && g_btlTarget.allowKo == 0) {
            target->flags &= ~BTL_UNIT_TARGETED;
            btlui_setDefaultTargets(0);
            return 1;
        }
        return 1;
    }
    if (g_btlTarget.targetType != BTL_TGT_EVERYONE) {
        unit = g_gameState->gameData->ctx.battle->unitList;
        count = 0;
        btlui_clearTargeted();
        while (unit != NULL) {
            if (((unit->flags >> BTL_UNIT_TARGETABLE_BIT) & 1) && (unit->flags & BTL_UNIT_PARTY) == (target->flags & BTL_UNIT_PARTY) && (!(unit->status & BTL_STATUS_KO) || g_btlTarget.allowKo != 0)) {
                unit->flags |= BTL_UNIT_TARGETED;
                count++;
            }
            unit = unit->next;
        }
        if (count != 0) {
            g_btlTarget.targetAll = 1;
        }
    }
    return 1;
}

void btlui_confirmTarget(void) {
    s32 targets = 0;
    u8 ok = 1;
    BtlUnit *unit = g_gameState->gameData->ctx.battle->unitList;
    Unk800FD56C *cmd = &g_btlCmdTable[g_btlTarget.cmdNo];
    Unk80104FF0 *target;
    for (; unit != NULL; unit = unit->next) {
        if ((unit->flags & (BTL_UNIT_TARGETABLE | BTL_UNIT_TARGETED)) == (BTL_UNIT_TARGETABLE | BTL_UNIT_TARGETED)) {
            targets |= unit->unitBit;
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
        if ((u32)(cmd->kind - 2) < 2) {
            btlui_reserveItem(g_btlTarget.subNo);
        }
        btlui_closeWindows();
    }
    snd_playSe(SE_CURSOR);
}

s32 btlui_confirmDoubleCast(s32 targets) {
    s8 targeting;
    s32 done = 0;
    if (g_btlTarget.doubleCast == 0) {
        g_btlTarget.firstTargets = targets;
        g_btlTarget.firstCmdNo = g_btlTarget.cmdNo;
        g_btlTarget.firstSubNo = g_btlTarget.subNo;
        targeting = btlui_getTargeting_inline();
        g_btlTarget.firstTargeting = targeting;
        btlui_cancelTarget();
        btlui_nextDoubleCast();
    } else {
        g_btlTarget.unit->inMenuQueue = 0;
        g_btlTarget.unit->cmdIssued = 1;
        btl_setCmd((s32)g_btlTarget.unit->delayedCmd, g_btlTarget.firstCmdNo, g_btlTarget.firstSubNo, (u16)g_btlTarget.firstTargets, g_btlTarget.firstTargeting);
        targeting = btlui_getTargeting_inline();
        btl_setCmd((s32)g_btlTarget.unit->cmd, g_btlTarget.cmdNo, g_btlTarget.subNo, targets & 0xFFFF, targeting);
        done = 1;
    }
    return done;
}

void btlui_cancelTarget(void) {
    BtlUnit *unit;
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
    for (unit = g_gameState->gameData->ctx.battle->unitList; unit != 0; unit = unit->next) {
        unit->flags &= ~BTL_UNIT_TARGETED;
    }
    for (i = 0; i < 2; i++) {
        pad_setRepeatEnabled(i, PAD_RPT_LEFT, 0);
        pad_setRepeatEnabled(i, PAD_RPT_RIGHT, 0);
    }
}

void btlui_updateTargetWindow(void) {
    if ((g_btlTarget.windowOpen == 0 || (u32)(g_btlUi->windows[0]->state - 1) >= 2) && g_btlTarget.windowOpen != g_btluiTargetWindow) {
        if (g_btluiTargetWindow != 0) {
            if (g_btlTarget.windowOpen == 0) {
                btlui_openTargetWindow_inline(3);
            }
        } else {
            btlui_closeTargetWindow_inline(3);
        }
    }
}

void btlui_drawTargetList(Window *win) {
    s32 y1 = win->y + 8;
    s32 y2 = y1;
    BtlUnit *unit = g_gameState->gameData->ctx.battle->unitList;
    s32 color;
    u32 flags;
    u8 *name;
    s32 pal;
    Window *w = win;
    ui_drawEtchedLine(win->x + BTLUI_TGT_DIVIDER_X, win->y + 4, win->otz, 9 - win->h);
    color = text_getColor();
    for (; unit != NULL; unit = unit->next) {
        name = btl_getUnitName(unit);
        flags = unit->flags;
        if ((flags >> BTL_UNIT_TARGETABLE_BIT) & 1) {
            if (flags & BTL_UNIT_PARTY) {
                if (unit->hp != 0) {
                    pal = ((u16)(unit->max->maxHp / UI_LOW_STAT_DIV) >= unit->hp) * TEXT_PAL_YELLOW;
                } else {
                    pal = TEXT_PAL_RED;
                }
                text_draw(w->x + BTLUI_TGT_PARTY_X, y2, win->otz, name, pal);
                y2 += BTLUI_LIST_ROW_H;
            } else {
                pal = ((u8)btl_hasStatus(unit, BTL_STATUS_KO) != 0) * TEXT_PAL_GREY;
                text_drawBlock(w->x + BTLUI_TGT_ENEMY_X, y1, win->otz, name, pal);
                y1 += BTLUI_LIST_ROW_H;
            }
        }
    }
    text_setColor(color);
}

void btlui_drawTargetListOverlay(Window *win) {
    Window *w = win;
    s32 targetedCount = 0;
    BtlUnit *unit = g_gameState->gameData->ctx.battle->unitList;
    RECT *rect = &win->cur;
    BtlUnit *scan;
    RECT rc;
    s32 yl;
    s32 yr;
    for (scan = unit; scan != NULL; scan = scan->next) {
        if ((scan->flags >> BTL_UNIT_TARGETED_BIT) & 1) {
            targetedCount++;
        }
    }
    if (win->state != WIN_STATE_OPEN) {
        win_setClip(NULL, win->otz, WIN_STYLE_FRAMELESS);
    }
    ui_drawLabel(w->x + BTLUI_TGT_ENEMY_LABEL_X, rect->y + WIN_TITLE_Y, win->otz, g_winLabelStrings.labelEnemy, UI_LABEL_NORMAL);
    ui_drawLabel(w->x + BTLUI_TGT_PARTY_LABEL_X, rect->y + WIN_TITLE_Y, win->otz, g_winLabelStrings.labelPlayer, UI_LABEL_NORMAL);
    if (win->state != WIN_STATE_OPEN) {
        rc.x = rect->x;
        rc.y = rect->y - 3;
        rc.w = rect->w;
        rc.h = 8;
        win_setClip(&rc, win->otz, WIN_STYLE_FRAMELESS);
    }
    if (win->state == WIN_STATE_OPEN && (targetedCount < 2 || !(g_btluiVsyncTick & 1))) {
        yl = rect->y + 0xC;
        yr = yl;
        for (; unit != NULL; unit = unit->next) {
            if ((unit->flags >> BTL_UNIT_TARGETABLE_BIT) & 1) {
                if ((unit->flags >> BTL_UNIT_TARGETED_BIT) & 1) {
                    if (unit->flags & BTL_UNIT_PARTY) {
                        ui_drawHand(rect->x + BTLUI_TGT_PARTY_HAND_X, yr, win->otz - 1);
                    } else {
                        ui_drawHand(rect->x + BTLUI_TGT_ENEMY_HAND_X, yl, win->otz - 1);
                    }
                }
                if (unit->flags & BTL_UNIT_PARTY) {
                    yr += BTLUI_LIST_ROW_H;
                } else {
                    yl += BTLUI_LIST_ROW_H;
                }
            }
        }
    }
}

void btlui_drawTargetCursors(void) {
    u32 color = GPU_RGBC(GPU_SPRT, GPU_RGB_NEUTRAL);
    Unk80105020 *cursors = &g_btluiCursors;
    u32 *clut = &g_targetCursorSprite;
    s32 i;
    SprtWords *sprt;
    DR_TPAGE *tpage;
    if (cursors->count >= 2 && !(g_btluiVsyncTick & 3)) {
        color = GPU_RGBC(GPU_SPRT, UI_RGB_HALF);
    }
    for (i = 0; i < cursors->count; i++) {
        setlen(g_primPtr, PRIM_LEN(SPRT));
        sprt = (SprtWords *)g_primPtr;
        sprt->rgbc = color;
        sprt->xy = (u16)cursors->pos[i][0] | (cursors->pos[i][1] << 16);
        sprt->uvclut = *clut;
        sprt->wh = 0xE0018;
        addPrim(&g_ot[17], sprt);
        g_primPtr = (u8 *)(sprt + 1);
    }
    setlen(g_primPtr, PRIM_LEN(DR_TPAGE));
    tpage = (DR_TPAGE *)g_primPtr;
    tpage->code[0] = get_mode(1, 1, getTPage(0, 0, VRAM_UI_SHEET_X, VRAM_UI_SHEET_Y));
    addPrim(&g_ot[17], tpage);
    g_primPtr = (u8 *)(tpage + 1);
}

void *btlui_getTargetHelpText(void) {
    BtlUnit *unit;
    BtlUnit *target;
    s32 count;
    s32 k;

    if (g_btlTarget.targetType < BTL_TGT_MULTI_ANY) {
        target = BtlFindTargetCursor();
        if (target == NULL) {
            return NULL;
        }
        text_copy(g_btluiHelpText, text_get(g_btlTarget.targetType + BTL_TEXT_TARGET));
        text_append(g_btluiHelpText, btl_getUnitName(target));
    } else if ((u8)(g_btlTarget.targetType - BTL_TGT_MULTI_ANY) < 3) {
        count = 0;
        k = g_btlTarget.targetType - BTL_TGT_MULTI_ANY;
        for (unit = g_gameState->gameData->ctx.battle->unitList; unit != NULL; unit = unit->next) {
            if ((unit->flags >> BTL_UNIT_TARGETED_BIT) & 1) {
                count++;
            }
        }
        if (count < 2) {
            target = BtlFindTargetCursor();
            if (target == NULL) {
                return NULL;
            }
            text_copy(g_btluiHelpText, text_get(k + BTL_TEXT_TARGET_SM));
            text_append(g_btluiHelpText, btl_getUnitName(target));
        } else {
            text_copy(g_btluiHelpText, text_get(k + BTL_TEXT_TARGET_MS));
        }
    } else {
        text_copy(g_btluiHelpText, text_get(g_btlTarget.targetType + BTL_TEXT_TARGET));
    }
    text_append(g_btluiHelpText, text_get(BTL_TEXT_HELP_TARGET));
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
    u32 config;
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
        config = g_gameState->config;
        g_gameState->config = (config & ~(1 << CFG_BIT_TARGET_WINDOW)) | ((((config >> CFG_BIT_TARGET_WINDOW) & 1) ^ 1) << CFG_BIT_TARGET_WINDOW);
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
    BtlUnit *unit = g_gameState->gameData->ctx.battle->unitList;
    s32 count = 0;
    while (unit != NULL) {
        if ((unit->flags >> BTL_UNIT_TARGETED_BIT) & 1) {
            count++;
        }
        unit = unit->next;
    }
    return count;
}

void btlui_clearTargeted(void) {
    BtlUnit *unit = g_gameState->gameData->ctx.battle->unitList;
    while (unit != NULL) {
        unit->flags &= ~BTL_UNIT_TARGETED;
        unit = unit->next;
    }
}

void btlui_collectCursorPositions(void) {
    s32 count;
    s32 partyCount;
    BtlUnit *unit;
    Unk80105020 *cursors;
    partyCount = count = 0;
    unit = g_gameState->gameData->ctx.battle->unitList;
    cursors = &g_btluiCursors;
    if (g_btluiAtbOn != 0) {
        for (; unit != NULL; unit = unit->next) {
            if (((unit->flags >> BTL_UNIT_TARGETABLE_BIT) & 1) && unit->onScreen != 0) {
                cursors->pos[count][0] = unit->screenX - 0x18;
                cursors->pos[count][1] = unit->screenY - 7;
                count++;
                if (unit->flags & BTL_UNIT_PARTY) {
                    cursors->party[partyCount] = unit;
                    partyCount++;
                }
            }
        }
    }
    cursors->count = count;
    cursors->partyCount = partyCount;
}

Unk80105020 *btlui_getCursorPositions(void) {
    return &g_btluiCursors;
}

void btlui_targetMove(void) {
    s32 i;
    s32 variant;
    if (g_btlTarget.windowOpen == 0) {
        variant = btlcam_getNextVariant();
    } else {
        variant = 0;
    }
    for (i = 0; i < 4; i++) {
        if (g_padRepeat & g_btluiTargetMovePads[variant][i]) {
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
    BtlUnit *unit = g_gameState->gameData->ctx.battle->unitList;
    if (targetSide == 0xFF) {
        targetSide = g_btlTarget.side;
    }
    for (; unit != NULL; unit = unit->next) {
        if (((unit->flags >> BTL_UNIT_TARGETABLE_BIT) & 1) && (unit->flags & BTL_UNIT_PARTY) == targetSide) {
            if (!(unit->status & BTL_STATUS_KO) || g_btlTarget.allowKo != 0) {
                unit->flags |= BTL_UNIT_TARGETED;
                if (all == 0) {
                    return;
                }
            }
        }
    }
}

void btlui_targetDefault(u8 targetSide, u8 all) {
    BtlUnit *unit = g_gameState->gameData->ctx.battle->unitList;
    u8 found = 0;
    BtlUnit *koUnit;
    u32 flags;
    u32 side;

    if (targetSide == 0xFF) {
        targetSide = g_btlTarget.side;
    }
    if (g_btlTarget.koFirst != 0) {
        koUnit = btlui_findKoUnit(targetSide);
        if (koUnit != NULL) {
            koUnit->flags |= BTL_UNIT_TARGETED;
            return;
        }
    }
    for (; unit != NULL; unit = unit->next) {
        flags = unit->flags;
        side = flags & BTL_UNIT_PARTY;
        if (!((flags >> BTL_UNIT_TARGETABLE_BIT) & 1) || side != targetSide) {
            continue;
        }
        if ((unit->status & BTL_STATUS_KO) && g_btlTarget.allowKo == 0) {
            continue;
        }
        if (all == 0 && side != 0) {
            if (unit == g_btlTarget.unit) {
                unit->flags = flags | BTL_UNIT_TARGETED;
                found = 1;
            }
        } else {
            found = 1;
            unit->flags |= BTL_UNIT_TARGETED;
        }
        if (all == 0 && found) {
            return;
        }
    }
}

BtlUnit *btlui_findKoUnit(u8 side) {
    BtlUnit *unit;
    for (unit = g_gameState->gameData->ctx.battle->unitList; unit != 0; unit = unit->next) {
        if (((unit->flags >> BTL_UNIT_TARGETABLE_BIT) & 1) && (unit->flags & BTL_UNIT_PARTY) == side && (unit->status & BTL_STATUS_KO)) {
            return unit;
        }
    }
    return 0;
}

void btlui_openTargetWindow(s8 openState) {
    btlui_openTargetWindow_inline(openState);
}

void btlui_closeTargetWindow(s32 closeState) {
    btlui_closeTargetWindow_inline(closeState);
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
    return btlui_getTargeting_inline();
}

void btlui_drawListCursors(Window *win) {
    Window *w = win;
    s32 partyIndex = 0;
    BtlUnit *unit = g_gameState->gameData->ctx.battle->unitList;
    Unk80105020 *cursors = btlui_getCursorPositions();
    Unk80105078 *panel = btlui_getPanel();
    u32 color;
    s32 y;
    s32 x;
    s32 k;
    s32 yy;
    Unk80068EE8 *sprite;
    SprtWords *sprt;
    DR_TPAGE *tpage;

    if (cursors->partyCount == 0 || win->state != WIN_STATE_OPEN || panel->shown == 0) {
        return;
    }
    color = GPU_RGBC(GPU_SPRT, GPU_RGB_NEUTRAL);
    if (cursors->partyCount >= 2 && !(g_btluiVsyncTick & 3)) {
        color = GPU_RGBC(GPU_SPRT, UI_RGB_HALF);
    }
    y = w->y + 5;
    x = w->x - 0xE;
    for (; unit != NULL; unit = unit->next) {
        if (!(unit->flags & BTL_UNIT_PARTY)) {
            continue;
        }
        if (unit == cursors->party[partyIndex]) {
            for (k = 0, yy = y; k < 2; k++, yy += 6) {
                sprite = &g_listCursorSprites[k];
                setlen(g_primPtr, PRIM_LEN(SPRT));
                sprt = (SprtWords *)g_primPtr;
                sprt->rgbc = color;
                sprt->xy = x | (yy << 16);
                sprt->uvclut = sprite->uvclut;
                sprt->wh = sprite->w | (sprite->h << 16);
                addPrim(&g_ot[win->otz - 1], sprt);
                g_primPtr = (u8 *)(sprt + 1);
            }
            partyIndex++;
            if (partyIndex >= cursors->partyCount) {
                break;
            }
        }
        y += BTLUI_PANEL_ROW_H;
    }
    setlen(g_primPtr, PRIM_LEN(DR_TPAGE));
    tpage = (DR_TPAGE *)g_primPtr;
    tpage->code[0] = get_mode(1, 1, g_listCursorSprites[0].tpage);
    addPrim(&g_ot[win->otz], tpage);
    g_primPtr = (u8 *)(tpage + 1);
}

void btlui_drawHpList(Window *win) {
    u8 hpText[8];
    u8 maxText[8];
    BtlUnit *unit;
    Window *w = win;
    s32 y = win->y + 6;
    s32 dy = win->h - win->cur.h;
    s32 pal;
    for (unit = g_gameState->gameData->ctx.battle->unitList; unit != 0 && (unit->flags & BTL_UNIT_PARTY); unit = unit->next) {
        pal = DIGITS_RED;
        if (unit->hp != 0) {
            pal = ((unit->status & BTL_STATUS_LOW_HP) != 0) * DIGITS_YELLOW;
        }
        maxText[0] = '/';
        text_formatNumber(unit->hp, hpText, 4, TEXT_NUM_ASCII);
        text_formatNumber(unit->max->maxHp, maxText + 1, TEXT_NUM_NO_PAD, TEXT_NUM_ASCII);
        ui_drawDigits(w->x + 4, y, win->otz, hpText, pal);
        ui_drawDigits(w->x + 0x20, y - dy, win->otz, maxText, pal);
        y += BTLUI_PANEL_ROW_H;
    }
}

void btlui_drawMpList(Window *win) {
    u8 mpText[8];
    u8 maxText[8];
    Window *w = win;
    s32 y = win->y + 6;
    BtlUnit *unit = g_gameState->gameData->ctx.battle->unitList;
    s32 dy = win->h - win->cur.h;
    s32 yy;
    s32 pal;
    while (unit != NULL) {
        if (unit->flags & BTL_UNIT_PARTY) {
            yy = y - dy;
            pal = (unit->mp < (s16)(unit->max->maxMp / UI_LOW_STAT_DIV)) * DIGITS_YELLOW;
            maxText[0] = '/';
            text_formatNumber(unit->mp, mpText, 3, TEXT_NUM_ASCII);
            y += BTLUI_PANEL_ROW_H;
            text_formatNumber((s16)unit->max->maxMp, &maxText[1], TEXT_NUM_NO_PAD, TEXT_NUM_ASCII);
            ui_drawDigits(w->x + 4, yy, win->otz, mpText, pal);
            ui_drawDigits(w->x + 0x19, yy, win->otz, maxText, pal);
        }
        unit = unit->next;
    }
}

void btlui_drawStatusList(Window *win) {
    BtlUnit *unit;
    s32 i;
    s32 iconCount;
    s32 x;
    Window *w = win;
    s32 scroll = win->h - win->cur.h;
    s32 y;
    u32 st;
    scroll -= 6;
    y = win->y - scroll;
    for (unit = g_gameState->gameData->ctx.battle->unitList; unit != 0; unit = unit->next) {
        if (unit->flags & BTL_UNIT_PARTY) {
            iconCount = 0;
            st = unit->status | unit->autoStatus;
            x = w->x + 4;
            for (i = 0; i < 16; i++) {
                if (st & g_btluiStatusIcons[i].status) {
                    ui_drawSprite(x, y, win->otz, g_btluiStatusIcons[i].icon, 0);
                    x += BTLUI_STATUS_ICON_W;
                    if (++iconCount >= BTLUI_STATUS_ICONS_MAX) {
                        break;
                    }
                }
            }
            y += BTLUI_PANEL_ROW_H;
        }
    }
}

void btlui_drawStatusList2(Window *win) {
    BtlUnit *unit;
    s32 i;
    s32 x;
    Window *w = win;
    s32 scroll = win->h - win->cur.h;
    s32 y;
    u32 st;
    scroll -= 6;
    y = win->y - scroll;
    for (unit = g_gameState->gameData->ctx.battle->unitList; unit != 0; unit = unit->next) {
        if (unit->flags & BTL_UNIT_PARTY) {
            st = unit->status | unit->autoStatus;
            x = w->x + 4;
            for (i = 0; i < 8; i++) {
                if (st & g_btluiStatusIcons2[i].status) {
                    ui_drawSprite(x, y, win->otz, g_btluiStatusIcons2[i].icon, 0);
                }
                x += g_btluiStatusIcons2[i].width;
            }
            y += BTLUI_PANEL_ROW_H;
        }
    }
}

void btlui_cacheHeader(void) {
    Unk80105078 *panel;
    RECT srcRect;

    panel = btlui_getPanel();
    setRECT(&srcRect, 0xD0, panel->y - 4, 0x40, 4);
    srcRect.x += g_btluiDispOrigin[g_gameState->gameData->drawBuf & 1][0];
    srcRect.y += g_btluiDispOrigin[g_gameState->gameData->drawBuf & 1][1];
    SetDrawMove((DR_MOVE *)g_primPtr, &srcRect, VRAM_BTLUI_CACHE_X, 0x1F8);
    addPrim(&g_ot[19], g_primPtr);
    g_primPtr += sizeof(DR_MOVE);
    setRECT(&srcRect, 0x110, panel->y - 4, 0x30, 4);
    srcRect.x += g_btluiDispOrigin[g_gameState->gameData->drawBuf & 1][0];
    srcRect.y += g_btluiDispOrigin[g_gameState->gameData->drawBuf & 1][1];
    SetDrawMove((DR_MOVE *)g_primPtr, &srcRect, VRAM_BTLUI_CACHE_X, 0x1FC);
    addPrim(&g_ot[19], g_primPtr);
    g_primPtr += sizeof(DR_MOVE);
}

void btlui_drawCachedHeader(void) {
    SPRT *headerA = &g_btluiPrims->headerA[g_drawBufs.cur];
    SPRT *headerB = &g_btluiPrims->headerB[g_drawBufs.cur];
    headerA->y0 = headerB->y0 = btlui_getPanel()->y - 4;
    addPrim(&g_ot[18], headerA);
    addPrim(&g_ot[18], headerB);
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
    Unk80105078 *panel = btlui_getPanel();
    Window *w;
    g_btluiListWindowRect.y = panel->y;
    g_btluiListWindowRect.h = panel->rowCount * BTLUI_PANEL_ROW_H + 8;
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
    Unk80105078 *panel = btlui_getPanel();
    Window *w;
    RECT *rect;
    s16 h;

    if (g_btluiListWindow.open != 0 && panel->rowCount != 0) {
        w = g_btlUi->windows[1];
        rect = &w->cur;
        rect->y = w->y = panel->y;
        h = panel->rowCount * BTLUI_PANEL_ROW_H + 8;
        w->h = h;
        if (w->state == WIN_STATE_CLOSING || w->state == WIN_STATE_OPEN) {
            rect->h = h;
        } else if (w->state == WIN_STATE_OPENING) {
            if (h < rect->h) {
                rect->h = h;
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
    s32 addr = g_btluiHeap.cursor;
    g_btluiHeap.cursor += size;
    g_btluiHeap.used += size;
    g_btluiHeap.free -= size;
    return (void *)addr;
}

void btlui_updatePanel(void) {
    BtlUnit *unit = g_gameState->gameData->ctx.battle->unitList;
    s32 row = 0;
    for (; unit != NULL; unit = unit->next) {
        if (unit->flags & BTL_UNIT_PARTY) {
            g_btluiPanel->units[row] = unit;
            g_btluiPanel->cantAct[row] = (btl_hasStatus(unit, (BTL_STATUS_PETRIFY | BTL_STATUS_VENOM | BTL_STATUS_KO | BTL_STATUS_CONFUSE | BTL_STATUS_BERSERK | BTL_STATUS_STOP | BTL_STATUS_SLEEP | BTL_STATUS_FREEZE | BTL_STATUS_JUMP)) & 0xFF) != 0;
            g_btluiPanel->slot[row] = (u8)unit->flags >> BTL_UNIT_SLOT_SHIFT;
            if (btl_hasStatus(unit, (BTL_STATUS_STOP | BTL_STATUS_SLOW)) & 0xFF) {
                g_btluiPanel->atbKind[row] = 2;
            } else if (btl_hasStatus(unit, BTL_STATUS_HASTE) & 0xFF) {
                g_btluiPanel->atbKind[row] = 3;
            } else {
                g_btluiPanel->atbKind[row] = 0;
            }
            if ((unit->flags >> BTL_UNIT_TRANCE_ENABLED_BIT) & 1) {
                g_btluiPanel->tranceKind[row] = (btl_hasStatus(unit, BTL_STATUS_TRANCE) & 0xFF) ? 2 : 1;
            } else {
                g_btluiPanel->tranceKind[row] = 0;
            }
            row++;
        }
    }
    g_btluiPanel->menuUnit = g_btlUi->cmdUnit;
    g_btluiPanel->rowCount = row;
    g_btluiPanel->y = g_btluiPanelY[row - 1];
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
    SPRT *sprt;
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
    setDrawTPage(&g_btluiPrims->namesTpage[bufIndex], 1, 1, getTPage(0, 0, VRAM_FONT_X, VRAM_FONT_Y));
}

void btlui_initCachedRows(s32 bufIndex) {
    s32 i;
    SPRT *sprt;
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
    BtlUnit *unit;
    Unk80104F88Entry *entry;
    Unk80104F88Gauge *gauge;
    Unk80104F88Gauge *mp;
    s32 i;
    s32 cur;
    s32 max;
    s32 delta;
    s32 dist;
    s32 adj;

    for (unit = g_gameState->gameData->ctx.battle->unitList; unit != NULL; unit = unit->next) {
        if (!(unit->flags & BTL_UNIT_PARTY)) {
            continue;
        }
        entry = unit->menu;
        mp = &entry->gauges[1];
        for (i = 0; i < 2; i++) {
            gauge = mp;
            if (i == 0) {
                gauge = &entry->gauges[0];
            }
            if (gauge->frames != 0) {
                if (gauge->step >= 0) {
                    if (gauge->shown + gauge->step >= gauge->target) {
                        gauge->shown = gauge->target;
                        gauge->frames = 0;
                    } else {
                        gauge->shown += gauge->step;
                        gauge->frames--;
                    }
                } else {
                    if (gauge->shown + gauge->step <= gauge->target) {
                        gauge->shown = gauge->target;
                        gauge->frames = 0;
                    } else {
                        gauge->shown += gauge->step;
                        gauge->frames--;
                    }
                }
            } else {
                cur = (i == 0) ? unit->hp : unit->mp;
                max = (i == 0) ? unit->max->maxHp : unit->max->maxMp;
                delta = cur - gauge->shown;
                if (delta != 0) {
                    dist = (delta >= 0) ? delta : -delta;
                    gauge->target = cur;
                    if (dist < 0x10) {
                        gauge->frames = dist;
                    } else {
                        gauge->frames = (dist << 6) / max;
                        if (gauge->frames < 0x10) {
                            dist = 0x10;
                            gauge->frames = dist;
                        }
                    }
                    if (delta >= 0) {
                        adj = delta - 1;
                        gauge->step = (adj + gauge->frames) / gauge->frames;
                    } else {
                        adj = delta + 1;
                        gauge->step = (adj - gauge->frames) / gauge->frames;
                    }
                }
            }
        }
    }
}

void btlui_drawPanelHeader(void) {
    RECT clipRect;
    s32 i;
    Unk800F412C *label;

    ui_drawRule(BTLUI_PANEL_X, g_btluiPanel->y, 8, 1, 0xA9, CONFIG_WINDOW_COLOR());
    clipRect.x = BTLUI_PANEL_X;
    clipRect.y = g_btluiPanel->y - 3;
    clipRect.w = g_gameState->dispEnv->disp.w - BTLUI_PANEL_X;
    clipRect.h = 7;
    win_setClip(NULL, 7, WIN_STYLE_FRAMELESS);
    label = g_btluiPanel->labels;
    for (i = 0; i < 5; label++, i++) {
        if (label->visible != 0) {
            ui_drawLabel((s16)label->x, g_btluiPanel->y + (s16)label->y, 7,
                          ((s32 *)&g_winLabelStrings)[label->text], UI_LABEL_NORMAL);
        }
    }
    win_setClip(&clipRect, 7, WIN_STYLE_FRAMELESS);
}

void btlui_drawPanelHighlight(void) {
    u8 row = g_btluiPanel->highlightRow;
    POLY_G4 *prim;
    s32 y;
    s32 rowOfs = 4;
    if (row != 0xFF && g_btluiFrameReady != 0) {
        y = g_btluiPanel->y + (row * BTLUI_PANEL_ROW_H + rowOfs);
        prim = &g_btluiPrims->highlight[g_drawBufs.cur];
        prim->y0 = prim->y1 = y;
        prim->y2 = prim->y3 = y + 13;
        addPrim(&g_ot[20], prim);
        addPrim(&g_ot[20], &g_btluiPrims->highlightTpage[g_drawBufs.cur]);
    }
}

void btlui_drawPanelName(s32 unit, u32 row) {
    s32 k = g_btluiPanel->slot[row];
    SPRT *sprt = &g_btluiPrims->names[k][g_drawBufs.cur];
    sprt->y0 = g_btluiPanel->y + row * BTLUI_PANEL_ROW_H + 3;
    sprt->clut = g_btluiRowCluts[(u8)g_btluiPanel->rowColor[row]] + ((k >> 1) << 6);
    addPrim(&g_ot[16], sprt);
    if (row >= g_btluiPanel->rowCount - 1) {
        addPrim(&g_ot[16], &g_btluiPrims->namesTpage[g_drawBufs.cur]);
    }
}

void btlui_drawPanelGauges(BtlUnit *unit, s32 row) {
    s32 tranceKind = g_btluiPanel->tranceKind[row];
    u8 active = 0;
    u32 y = g_btluiPanel->y + row * BTLUI_PANEL_ROW_H;
    if (unit->cmdIssued == 0) {
        active = g_btluiPanel->cantAct[row] == 0;
    }
    if (tranceKind == BTLUI_TRANCE_ACTIVE) {
        ui_drawGauge(BTLUI_PANEL_TRANCE_X, y + 10, 14, unit->trance, 0xFF, 1, active);
        ui_drawGauge(BTLUI_PANEL_ATB_X, y + 6, 14, unit->atb, unit->max->maxAtb, g_btluiPanel->atbKind[row], active);
    } else if (tranceKind == BTLUI_TRANCE_GAUGE) {
        ui_drawGauge(BTLUI_PANEL_ATB_X, y + 6, 14, unit->atb, unit->max->maxAtb, g_btluiPanel->atbKind[row], active);
        ui_drawGauge(BTLUI_PANEL_TRANCE_X, y + 10, 14, unit->trance, 0xFF, tranceKind, active);
    } else {
        ui_drawGauge(BTLUI_PANEL_ATB_X, y + 8, 14, unit->atb, unit->max->maxAtb, g_btluiPanel->atbKind[row], active);
    }
}

void btlui_cacheRows(void) {
    RECT srcA = s_btluiRowsSrcA;
    RECT srcB = s_btluiRowsSrcB;
    s16 h;
    if (g_btluiPanel->shown != 0 && g_btluiPanel->rowCount != 0) {
        srcA.x += g_btluiDispOrigin[g_gameState->gameData->drawBuf & 1][0];
        srcB.x += g_btluiDispOrigin[g_gameState->gameData->drawBuf & 1][0];
        srcA.y += g_btluiDispOrigin[g_gameState->gameData->drawBuf & 1][1] + g_btluiPanel->y;
        srcB.y += g_btluiDispOrigin[g_gameState->gameData->drawBuf & 1][1] + g_btluiPanel->y;
        h = g_btluiPanel->rowCount * BTLUI_PANEL_ROW_H;
        srcB.h = h;
        srcA.h = h;
        SetDrawMove((DR_MOVE *)g_primPtr, &srcA, VRAM_BTLUI_CACHE_X, 0x1B0);
        addPrim(&g_ot[19], g_primPtr);
        g_primPtr += sizeof(DR_MOVE);
        SetDrawMove((DR_MOVE *)g_primPtr, &srcB, 0x3E0, 0x1B0);
        addPrim(&g_ot[19], g_primPtr);
        g_primPtr += sizeof(DR_MOVE);
    }
}

void btlui_drawCachedRows(void) {
    s32 y = g_btluiPanel->y + 6;
    u8 v = 0xB0;
    u32 i;
    SPRT *rowB;
    SPRT *rowA;
    if (g_btluiPanel->shown != 0 && g_btluiPanel->rowCount != 0) {
        for (i = 0; i < g_btluiPanel->rowCount; i++) {
            rowA = &g_btluiPrims->rowsA[i][g_drawBufs.cur];
            rowB = &g_btluiPrims->rowsB[i][g_drawBufs.cur];
            rowB->y0 = y;
            rowA->y0 = y;
            y += BTLUI_PANEL_ROW_H;
            rowB->v0 = v;
            rowA->v0 = v;
            v += BTLUI_PANEL_ROW_H;
            addPrim(&g_ot[18], rowA);
            addPrim(&g_ot[18], rowB);
        }
        addPrim(&g_ot[18], &g_btluiPrims->rowsTpage[g_drawBufs.cur]);
    }
}

void btlui_initPanel(void) {
    s32 i;
    g_btluiPanel = btlui_alloc(sizeof(*g_btluiPanel));
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
    Unk800F412C *label = g_btluiPanel->labels;
    for (i = 0; i < 5; i++) {
        if (label->slide != 0) {
            label->x += label->slide->dx;
            label->y += label->slide->dy;
            if (++label->frame >= label->slide->frames) {
                label->frame = 0;
                if (label->slide->onDone != 0) {
                    label->slide->onDone(label);
                }
                label->slide = 0;
            }
        }
        label++;
    }
}

void btlui_pulsePanelHighlight(void) {
    Unk80105078 *panel = g_btluiPanel;
    u32 phase = g_btluiVsyncTick & 0x3F;
    panel->pulse = phase;
    if (phase >= 0x20) {
        panel->pulse = 0x3F - phase;
    }
}

void btlui_resetGauges(s32 row) {
    BtlUnit *unit = g_btluiPanel->units[row];
    Unk80104F88Entry *entry = unit->menu;
    Unk80104F88Gauge *mpGauge;
    s16 mp;
    entry->gauges[0].target = entry->gauges[0].shown = unit->hp;
    mp = unit->mp;
    mpGauge = &entry->gauges[1];
    mpGauge->frames = 0;
    mpGauge->target = mpGauge->shown = mp;
    entry->gauges[0].frames = 0;
    entry->gauges[1].step = 0;
    entry->gauges[0].step = 0;
}

void btlui_updatePanelRowColor(s32 row) {
    BtlUnit *unit = g_btluiPanel->units[row];
    s32 hp = unit->hp;
    if (hp == 0) {
        g_btluiPanel->rowColor[row] = DIGITS_RED;
        return;
    }
    g_btluiPanel->rowColor[row] = ((s32)(u16)(unit->max->maxHp / UI_LOW_STAT_DIV) >= hp) * DIGITS_YELLOW;
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
    ui_drawDigits(BTLUI_PANEL_HP_X, g_btluiPanel->y + (row * BTLUI_PANEL_ROW_H + dy), 0xE, digits, (u8)g_btluiPanel->rowColor[row]);
}

void btlui_drawPanelMp(BtlUnit *unit, s32 row) {
    u8 digits[8];
    s32 mp;
    s32 pal;
    s32 dy = 6;
    mp = unit->menu->gauges[1].shown < 0 ? 0 : unit->menu->gauges[1].shown;
    pal = ((s16)(unit->max->maxMp / UI_LOW_STAT_DIV) >= mp) * DIGITS_YELLOW;
    text_formatNumber(mp, digits, 3, TEXT_NUM_ASCII);
    ui_drawDigits(BTLUI_PANEL_MP_X, g_btluiPanel->y + (row * BTLUI_PANEL_ROW_H + dy), 0xE, digits, pal);
}

void btlui_findHighlightRow(void) {
    BtlUnit *unit = g_gameState->gameData->ctx.battle->unitList;
    s32 i = 0;
    if (g_btluiPanel->rowCount != 0 && g_btluiHideWindows == 0) {
        for (; unit != 0; unit = unit->next) {
            if (unit->flags & BTL_UNIT_PARTY) {
                if (g_btluiPanel->menuUnit == unit) {
                    g_btluiPanel->highlightRow = i;
                    return;
                }
                i++;
            }
        }
    }
}

void btlui_drawPortIcons(void) {
    GameData *gd = g_gameState->gameData;
    Unk80068EE8 *sprite;
    s32 multiCtrl;

    if (!(gd->flags & GD_PAUSED) && !(gd->ctx.battle->flags & BTL_FLAG_DISC_COVER_OPEN) && g_btluiState.enabled != 0 &&
        (multiCtrl = (g_gameState->config >> CFG_BIT_BTL_CONTROL) & 1) == 1) {
        ClearOTag(&g_btluiPortIconOt, 1);
        if (g_btluiState.vsyncTick & 0x20) {
            if (g_btluiState.port == 0) {
                sprite = &g_portBlinkSprite1;
            } else {
                sprite = &g_portBlinkSprite2;
            }
            setlen(&g_btluiPortBlinkSprt, PRIM_LEN(SPRT));
            g_btluiPortBlinkSprt.rgbc = GPU_RGBC(GPU_SPRT, GPU_RGB_NEUTRAL);
            g_btluiPortBlinkSprt.xy = 0x6012B;
            g_btluiPortBlinkSprt.uvclut = sprite->uvclut;
            g_btluiPortBlinkSprt.wh = sprite->w | (sprite->h << 16);
            addPrim(&g_btluiPortIconOt, &g_btluiPortBlinkSprt);
        }
        if (g_btluiPort == 0) {
            sprite = &g_portIconSprite1;
        } else {
            sprite = &g_portIconSprite2;
        }
        setlen(&g_btluiPortIconSprt, PRIM_LEN(SPRT));
        g_btluiPortIconSprt.rgbc = GPU_RGBC(GPU_SPRT, GPU_RGB_NEUTRAL);
        g_btluiPortIconSprt.xy = 0x60124;
        g_btluiPortIconSprt.uvclut = sprite->uvclut;
        g_btluiPortIconSprt.wh = sprite->w | (sprite->h << 16);
        addPrim(&g_btluiPortIconOt, &g_btluiPortIconSprt);
        setDrawTPage(&g_btluiPortIconTpage, 1, 1, sprite->tpage);
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
        for (j = 0; j < PAD_RPT_COUNT; j++) {
            row[j] = pad_isRepeatEnabled(i, j);
        }
        pad_setRepeatEnabled(i, PAD_RPT_LEFT, 0);
        pad_setRepeatEnabled(i, PAD_RPT_RIGHT, 0);
        pad_setRepeatTiming(i, PAD_RPT_BTN_CROSS, 0xF, 0xA);
        pad_setRepeatEnabled(i, PAD_RPT_BTN_CROSS, 1);
        pad_clearRepeats(i);
    }
    g_btluiSecondPort = btlui_anyOnSecondPort();
}

void btlui_restoreKeyRepeat(void) {
    s32 i, j;
    u8 *saved;
    for (i = 0, saved = g_btluiSavedRepeat[0]; i < 2; i++) {
        for (j = 0; j < PAD_RPT_COUNT; j++) {
            pad_setRepeatEnabled(i, j, saved[j]);
        }
        saved += 0x18;
    }
    pad_setStickAsDpad(0);
}

void btlui_readPad(void) {
    pad_update();
    pad_updateRepeat(g_btluiState.port);
    if ((g_padRepeat & PAD_START) && g_btluiState.pauseReq == 0) {
        g_btluiState.pauseReq = 1;
    }
}

u8 btlui_anyOnSecondPort(void) {
    s32 i;
    s32 multiCtrl = (g_gameState->config >> CFG_BIT_BTL_CONTROL) & 1;
    if (multiCtrl != 1) {
        return 0;
    }
    for (i = 0; i < PARTY_SIZE; i++) {
        Character *chr = g_gameState->gameData->party[i];
        if (chr == 0) {
            continue;
        }
        if (((s32)(g_gameState->config >> CFG_SHIFT_CHAR_PORT) >> chr_getDefaultSet(chr)) & 1) {
            return 1;
        }
    }
    return 0;
}

void btlui_initPrims(void) {
    s32 i;
    g_btluiPrims = btlui_alloc(sizeof(*g_btluiPrims));
    for (i = 0; i < 2; i++) {
        btlui_initCachedCmdIcons(i);
        btlui_initCachedHeader(i);
        btlui_initPanelPrims(i);
    }
}

void btlui_onReset(void) {
}

void btlui_initDrawBufs(void) {
    u32 *ots = btlui_alloc(0xC0);
    s32 prims = (s32)btlui_alloc(0x4000);
    DRAWENV *env;
    gfx_init(ots, ots + 0x18, prims, prims + 0x2000);
    gfx_setUiMode(2);
    gfx_setBufferIndex(g_gameState->gameData->drawBuf);
    g_btluiBuf = 0;
    g_ot = g_drawBufs.ots[0];
    g_primPtr = g_drawBufs.prims[0];
    env = g_gameState->drawEnv;
    env[1].dfe = 1;
    env[0].dfe = 1;
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
