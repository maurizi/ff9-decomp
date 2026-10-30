#include "ovl_0b.h"

void btlui_vsync(void) {
    GameData *g;
    g_btluiState.inVsync = 1;
    func_800EA5C4();
    func_800EA548();
    btlui_readPad();
    if (g_btluiState.frameReady != 0) {
        btlui_frameUpdate();
    }
    g = g_gameState->gameData;
    if (!(g->unk0 & 0x100) && !(g->unk8E4.battle->flags & 0x20)) {
        if (g_btluiState.skipFrames == 0 && (u8)btlui_isEnabled()) {
            func_800EB578(0xBA);
            btlui_beginFrame();
            func_800EA4D0();
            func_800EA378();
            btlui_input();
            btlui_drawWindows();
            btlui_update();
            btlui_submitOt();
            func_800EB578(0xBB);
        }
    }
    g_btluiState.vsyncCount++;
    g_drawBufs.lastVsync++;
    g_btluiState.vsyncTick++;
    g_btluiState.frameReady = 0;
    g_btluiState.inVsync = 0;
}

void btlui_frameUpdate(void) {
    func_800EA5C4();
    g_drawBufs.cur = g_gameState->gameData->unk8 ^ 1;
    if (g_btluiState.enabled == 0) {
        btlui_applyEnable();
    }
    if (!(g_gameState->gameData->unk0 & 0x100)) {
        if (!(g_gameState->gameData->unk8E4.battle->flags & 0x20)) {
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
        if (!(g_gameState->gameData->unk0 & 0x100) && !(g_gameState->gameData->unk8E4.battle->flags & 0x20) && g_btluiState.skipFrames != 0) {
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

void func_800E9D84(void) {
    void *p = db_lookupRegistered(4, 0x14, 0);
    if (p != NULL) {
        text_uploadPartyNames((s32)p);
    }
}

void btlui_loadPortraits(void) {
    void *p = db_lookupRegistered(4, 0x1E, 0);
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
    func_800F48DC();
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
    g_btluiState.unkE = 0;
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

void btlui_enable(u8 arg0) {
    if (arg0) {
        g_btluiState.enableReq = 1;
    } else {
        g_btluiState.enabled = 0;
    }
}

void btlui_setAtbEnabled(u8 arg0) {
    if (g_btluiState.started != 0) {
        if (g_btluiState.atbOn != arg0) {
            g_btluiState.atbOn = arg0;
            if (arg0 == 0) {
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

u8 func_800EA024(void) {
    return g_btluiState.multiTarget;
}

u8 btl_isAtbRunning(void) {
    s32 f;
    if (g_btluiState.atbOn == 0) {
        return 0;
    }
    f = (g_gameState->config >> 3) & 1;
    if (f != 1) {
        return 1;
    }
    return g_btlUi->menuState < BTLUI_MENU_LIST;
}

s32 func_800EA094(void) {
    return func_800F1360();
}

u8 *btl_getAbilityName(s32 arg0) {
    if (arg0 >= 0xC0) {
        return NULL;
    }
    return g_gameState->gameData->unk8E4.battle->abilities[arg0].nameOfs + D_800F909C;
}

void btl_useReservedItem(s32 arg0) {
    btlui_unreserveItem(arg0);
    btlui_removeItem(arg0);
}

void btl_unreserveItem(s32 arg0) {
    btlui_unreserveItem(arg0);
}

void btl_reserveItem(s32 arg0) {
    btlui_reserveItem(arg0);
}

void btl_useItem(s32 arg0) {
    btlui_removeItem(arg0);
    if (btlui_getItemReserved(arg0) != 0 && item_getCount(arg0) == 0) {
        btlui_unreserveItem(arg0);
    }
}

void btl_addItem(s32 arg0) {
    btlui_addItem(arg0);
}

void func_800EA1D0(s8 arg0) {
    if (g_btluiState.unk9 == 0) {
        g_btluiState.unk9 = 1;
        g_btluiState.unk14 = arg0;
    }
}

void btlui_drawOTagEnv(u32 *arg0, DRAWENV *arg1) {
    g_btluiState.drawing = 1;
    DrawOTagEnv(arg0, arg1);
    g_btluiState.drawing = 0;
}

void btlui_drawOTag(u32 *arg0) {
    g_btluiState.drawing = 1;
    DrawOTag(arg0);
    g_btluiState.drawing = 0;
}

u8 func_800EA268(void) {
    return g_btluiState.port;
}

u8 func_800EA27C(void) {
    return g_btluiState.unkE;
}

void func_800EA290(void) {
    if (g_btluiState.started == 0) {
        GameData *gd = g_gameState->gameData;
        if (!(gd->unk8E4.battle->flags & 0x100)) {
            PutDispEnv(&g_gameState->dispEnv[gd->unk8]);
        }
    }
}

u8 btlui_isPauseRequested(void) {
    return g_btluiState.started ? g_btluiState.pauseReq : 0;
}

void btlui_setPauseRequest(s8 arg0) {
    g_btluiState.pauseReq = arg0;
}

void func_800EA34C(void) {
    u8 v = g_gameState->gameData->unk8;
    g_btluiState.skipFrames = 2;
    g_drawBufs.cur = v;
}

void func_800EA378(void) {
    GameState *g = g_gameState;
    s32 r = 0;
    volatile Unk80104E68 *p = &g_btluiState;
    s32 f;
    if (((g->gameData->unk8E4.battle->cmdFlags >> 1) ^ 1) & 1) {
        r = 1;
    } else {
        do {
            f = (g->config >> 19) & 1;
            if (f == 1) {
                r = 1;
            }
        } while (0);
    }
    p->unk3 = r;
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
        func_800EA518();
    }
    btlui_drawTargetCursors();
}

void btlui_update(void) {
    btlui_updateMenu();
    btlui_updatePanelAnims();
    win_updateAll();
    btlui_updateHelpSlide();
}

void func_800EA4D0(void) {
    if (g_btluiState.frameReady != 0) {
        btlui_cacheRows();
        btlui_cacheCmdIcons();
        btlui_cacheHeader();
    }
}

void func_800EA518(void) {
    btlui_drawCachedRows();
    btlui_drawCachedCmdIcons();
    btlui_drawCachedHeader();
}

void func_800EA548(void) {
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

void func_800EA5C4(void) {
    if (g_btluiState.frameReady != 0 && g_btluiState.started != 0) {
        GameData *gd = g_gameState->gameData;
        if (!(gd->unk8E4.battle->flags & 0x100)) {
            PutDispEnv(&g_gameState->dispEnv[gd->unk8]);
        }
    }
}

void btlui_initPartyEntries(void) {
    BtlUnit *u = g_gameState->gameData->unk8E4.battle->unitList;
    Unk80104F88Entry *e = g_btlUi->entries;
    s8 n = 0;
    s8 f;
    while (u != NULL && (u->flags & 1)) {
        u->menu = e;
        u->inMenuQueue = 0;
        f = (u8)u->flags >> 5;
        e->unit = u;
        e->slot = f;
        e->chr = &g_gameState->gameData->characters[(u->flags >> 1) & 0xF];
        e->learns = func_80034A30(e->chr);
        e->defaultSet = chr_getAppearanceType(e->chr->unk34.b.id, (e->chr->unkA >> 4) & 1);
        if ((g_gameState->config >> 5) & 1) {
            f = ((s32)(g_gameState->config >> 20) >> e->defaultSet) & 1;
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

void btlui_copyTranceAbilities(Unk80104F88Entry *arg0) {
    s32 k = arg0->chr->unk34.b.row;
    u8 *s = g_btlCharTranceCmds[k];
    u8 *t = g_btlCharCmds[k];
    Unk800FD56C *a;
    Unk800FD56C *b;
    s32 i;
    s32 j;
    u8 x;
    u8 y;
    if ((func_80034A30(arg0->chr) & 0xFF) && (s[2] == 1 || s[2] == 2)) {
        i = 0;
        j = s[2] - 1;
        a = &g_btlCmdTable[t[j]];
        b = &g_btlCmdTable[s[j]];
        for (; i < a->count; i++) {
            x = g_btlCmdAbilities[a->first + i];
            y = g_btlCmdAbilities[b->first + i];
            if (x != y) {
                arg0->apCounters[y] = arg0->apCounters[x];
                arg0->apNeeded[y] = arg0->apNeeded[x];
                arg0->fromEquip[y] = arg0->fromEquip[x];
            }
        }
    }
}

/* character 3: also list the abilities of character record 1 and its equipment */
void func_800EA908(Unk80104F88Entry *arg0) {
    Unk800FD6E0 *m = &D_800FD6E0;
    Character *c = &g_gameState->gameData->characters[1];
    Unk8006CB88 *ab = D_8006CB88;
    s32 i;
    s32 j;
    s32 k;
    s32 slot;
    s32 v;
    s32 id;
    s32 e;
    Unk80069018 *it;

    if (arg0->chr->unk34.b.id == 3) {
        for (i = 0; i < m->count; i++) {
            slot = g_btlCmdAbilities[m->first + i];
            v = chr_findAbilitySlot(1, D_800F8D84[i]);
            if (v >= 0) {
                arg0->apCounters[slot] = &c->ap[v];
                arg0->apNeeded[slot] = ab[v].apNeeded;
            }
        }
        for (i = 0; i < 5; i++) {
            e = c->equip[i];
            if (e != 0xFF) {
                it = &g_itemTable[e];
                for (j = 0; j < 3; j++) {
                    id = it->unkC[j];
                    if (id != 0 && id < 0xC0) {
                        for (k = 0; k < m->count; k++) {
                            if (id == D_800F8D84[k]) {
                                slot = g_btlCmdAbilities[m->first + k];
                                arg0->fromEquip[slot] = 1;
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

    u = g_gameState->gameData->unk8E4.battle->unitList;
    if (btl_isAtbRunning() == 0) {
        return;
    }
    for (; u != NULL; u = u->next) {
        if (u->cmdIssued != 0 || u->inMenuQueue != 0 || u->hp == 0) {
            continue;
        }
        if (!((u->flags >> 16) & 1)) {
            continue;
        }
        if ((u->flags & 1) == g_btlUi->frozenSide) {
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
        if (u->flags & 1) {
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

    u = g_gameState->gameData->unk8E4.battle->unitList;
    g_btlUi->swdMagPartner = 0;
    g_btlUi->swdMagBlocked.b[0] = 0;
    g_btlUi->swdMagBlocked.b[1] = 0;
    for (; u != NULL; u = u->next) {
        if (!(u->flags & 1)) {
            break;
        }
        slot = (u->flags >> 1) & 0xF;
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
        if (u == NULL || u->hp == 0 || (btl_hasStatus(u, (BTL_STATUS_PETRIFY | BTL_STATUS_VENOM | BTL_STATUS_KO | BTL_STATUS_CONFUSE | BTL_STATUS_BERSERK | BTL_STATUS_STOP | BTL_STATUS_SLEEP | BTL_STATUS_FREEZE | BTL_STATUS_JUMP)) & 0xFF) || !((g_btlRewards.expMask >> ((u8)u->flags >> 5)) & 1)) {
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
            if (seen[(u8)list[i]->flags >> 5] == 0) {
                seen[(u8)list[i]->flags >> 5] = 1;
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
    g_btlUi->ambush = g_gameState->gameData->unk8E4.battle->scene.flags & 3;
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

s32 btlui_setMenuState(s32 arg0) {
    s32 old = g_btlUi->menuState;
    if (arg0 != BTLUI_MENU_KEEP) {
        g_btlUi->menuState = arg0;
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

void btlui_queueMenuUnit(BtlUnit *arg0) {
    Unk80104F88 *p = g_btlUi;
    arg0->inMenuQueue = 1;
    g_btlUi->queue[(p->queueHead + p->queueCount) & 3] = arg0;
    p->queueCount++;
}

void btlui_resetMenuQueue(void) {
    BtlUnit *u = g_gameState->gameData->unk8E4.battle->unitList;
    btlui_clearMenuQueue();
    while (u != NULL) {
        if (u->flags & 1) {
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

void btlui_saveCursor(Unk800EB518 *arg0, s32 arg1, s32 arg2, s32 arg3) {
    if (arg1 == 0) {
        arg0->cmdRow = arg2;
        return;
    }
    arg0->listTop[arg0->cmdRow] = arg2;
    arg0->listCursor[arg0->cmdRow] = arg3;
}

void btlui_clearCursor(Unk800EB518 *arg0) {
    s32 i;
    arg0->cmdRow = 0;
    for (i = 0; i < 4; i++) {
        arg0->listTop[i] = 0;
        arg0->listCursor[i] = 0;
    }
}

void func_800EB578(s32 arg0) {
}

u8 *func_800EB580(void) {
    return &g_btlUi->queueCount;
}

BtlUnit *btlui_getQueuedUnit(s32 arg0) {
    return g_btlUi->queue[arg0];
}

void btlui_clearAbilityLists(void) {
    s32 i, j;
    for (i = 0; i < 4; i++) {
        Unk80104F88Entry *e = &g_btlUi->entries[i];
        for (j = 0; j < 0xC0; j++) {
            e->apCounters[j] = 0;
            e->apNeeded[j] = 0;
            e->fromEquip[j] = 0;
        }
    }
}

void btlui_buildAbilityList(Unk80104F88Entry *arg0) {
    btlui_addLearnedAbilities(arg0);
    btlui_addEquipAbilities(arg0);
    btlui_copyTranceAbilities(arg0);
    func_800EA908(arg0);
}

void btlui_addLearnedAbilities(Unk80104F88Entry *arg0) {
    Character *c;
    s32 i;
    AbilityEntry *row;
    s32 v;
    c = arg0->chr;
    if (arg0->learns != 0) {
        i = 0;
        row = g_abilityRows[c->unk34.b.row];
        for (; i < 0xC0; i++) {
            v = chr_findAbilitySlot(c->unk34.b.id, i);
            if (v >= 0) {
                arg0->apCounters[i] = &c->ap[v];
                arg0->apNeeded[i] = row[v].unk1;
            }
        }
    }
}

void btlui_addEquipAbilities(Unk80104F88Entry *arg0) {
    Character *c = arg0->chr;
    s32 i;
    s32 j;
    memset((s32)arg0->fromEquip, 0, 0xC0);
    for (i = 0; i < 5; i++) {
        u8 item = c->equip[i];
        if (item != 0xFF) {
            Unk80069018 *e = &g_itemTable[item];
            for (j = 0; j < 3; j++) {
                s32 a = e->unkC[j];
                if (a != 0 && a < 0xC0) {
                    arg0->fromEquip[a] = 1;
                }
            }
        }
    }
}

s32 btlui_canOpenMenu(BtlUnit *arg0) {
    if (arg0 == NULL) {
        return 0;
    }
    if (arg0->hp != 0 && !(btl_hasStatus(arg0, (BTL_STATUS_PETRIFY | BTL_STATUS_VENOM | BTL_STATUS_KO | BTL_STATUS_CONFUSE | BTL_STATUS_BERSERK | BTL_STATUS_STOP | BTL_STATUS_SLEEP | BTL_STATUS_FREEZE | BTL_STATUS_JUMP)) & 0xFF)) {
        return (D_800FDF3A >> ((u8)arg0->flags >> 5)) & 1;
    }
    return 0;
}

void btlui_applyTargetable(void) {
    BtlUnit *u;
    u8 c;
    for (u = g_gameState->gameData->unk8E4.battle->unitList; u != 0; u = u->next) {
        c = u->targetableReq;
        if (c >= 2) {
            if (c == 2) {
                u->targetableReq = 0;
                u->flags &= ~0x10000;
                u->flags &= ~0x10000000;
                if (g_btlUi->menuState == BTLUI_MENU_TARGET) {
                    btlui_onUnitUntargetable(u);
                }
            } else if (c == 3) {
                u->targetableReq = 1;
                u->flags |= 0x10000;
                u->flags |= 0x10000000;
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
            g_gameState->gameData->unk8E4.battle->escapeHeld = 1;
            if (p->escapeFrames++ >= 0x3D) {
                p->escapeFrames = 0;
                btl_tryEscape(1);
            } else {
                btl_tryEscape(0);
            }
        } else {
            g_gameState->gameData->unk8E4.battle->escapeHeld = 0;
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

void btlui_onCmdWindowClosed(Window *arg0) {
    g_btlUi->menuState = BTLUI_MENU_NONE;
}

void btlui_drawHelpWindow(void) {
    RECT r;
    s32 m;
    if (g_btluiHelp.open != 0 && D_80104E75 != 0) {
        btlui_getHelpRect(&D_800F8DBC, &r);
        win_setClip(NULL, 2, 4);
        if (g_btluiHelp.opening == 0) {
            ui_drawSprite(r.x + 8, r.y, 2, 0xB7, 0);
        }
        btlui_updateHelpText();
        if (g_btluiHelp.text != 0) {
            win_setClip(NULL, 2, 3);
            m = text_getColor();
            text_drawBlock(r.x + 6, r.y + 10, 2, g_btluiHelp.text, 8);
            text_setColor(m);
            win_setClip(&r, 2, 3);
        }
        win_drawHelpFrame(&r, 2, 4, 0);
        win_setClip(&r, 2, 4);
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
    if (g_btluiHelp.open != 0 && D_80104E75 != 0) {
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

void btlui_toggleHelp(u8 arg0) {
    if (arg0) {
        if (g_btluiHelp.open == 0) {
            snd_playSe(0x2AA);
            g_btluiHelp.open = 1;
            g_btluiHelp.opening = 1;
            g_btluiHelp.frame = 0;
        }
    } else if (g_btluiHelp.open != 0 && g_btluiHelp.closeReq == 0) {
        snd_playSe(0x65);
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
    if (g_gameState->gameData->unk8E4.battle->escapeHeld != 0) {
        g_btluiHelp.text = (s32)text_get(4);
    } else {
        fn = g_btluiMenuHelp[g_btlUi->menuState];
        if (fn != NULL) {
            g_btluiHelp.text = fn();
        }
    }
}

void btlui_getHelpRect(RECT *arg0, RECT *arg1) {
    Window *w;
    s16 x;
    s32 d;
    *arg1 = *arg0;
    w = btlui_getTargetWindow();
    if (w != NULL && w->state != 0) {
        if (arg1->x < w->unk8.x + w->unk8.w - 4) {
            x = w->unk8.x + w->unk8.w - 4;
            arg1->x = x;
            arg1->w = 0x137 - x;
        }
    }
    if (g_btluiHelp.opening != 0) {
        d = (7 - g_btluiHelp.frame) * 24;
        arg1->x += d;
        arg1->w -= d;
    }
}

void btlui_removeItem(s32 arg0) {
    s32 i;
    if (item_remove(arg0, 1) != 0) {
        if (arg0 >= 0xE0) {
            i = btlui_findItemListEntry(arg0);
            if (i != 0xFF && g_btluiItems->items[i]->unk1 == 0) {
                g_btluiItems->items[i] = 0;
                g_btluiItems->itemCount--;
            }
        }
        if (arg0 < 0x58 && (g_weaponTable[arg0].flags & 4)) {
            i = btlui_findThrowListEntry(arg0);
            if (i != 0xFF && g_btluiItems->throwItems[i]->unk1 == 0) {
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

void btlui_addItem(s32 arg0) {
    if (item_add(arg0, 1) == 0) {
        return;
    }
    if (arg0 >= 0xE0 && btlui_findItemListEntry(arg0) == 0xFF) {
        btlui_addItemListEntry(arg0);
    }
    if (arg0 < 0x58 && (g_weaponTable[arg0].flags & 4) && btlui_findThrowListEntry(arg0) == 0xFF) {
        btlui_addThrowListEntry(arg0);
    }
}

UnkPair *btlui_getItemListEntry(s32 arg0) {
    return g_btluiItems->items[arg0];
}

UnkPair *btlui_getThrowListEntry(s32 arg0) {
    return g_btluiItems->throwItems[arg0];
}

void btlui_reserveItem(s32 arg0) {
    g_btluiItems->reserved[arg0]++;
}

void btlui_unreserveItem(s32 arg0) {
    g_btluiItems->reserved[arg0]--;
}

s32 btlui_getItemReserved(s32 arg0) {
    return g_btluiItems->reserved[arg0];
}

void btlui_buildItemList(void) {
    s32 n = 0;
    s32 i;
    UnkPair *p = g_gameState->gameData->items.slots;
    Unk80104FA8 *f = g_btluiItems;
    for (i = 0; i < 0x100; i++, p++) {
        if (p->unk1 != 0 && p->unk0 != 0xFF && p->unk0 >= 0xE0) {
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
    for (; i < 0x100; i++, e++) {
        if (e->unk1 != 0 && e->unk0 != 0xFF && e->unk0 < 0x58 && (g_weaponTable[e->unk0].flags & 4)) {
            p->throwItems[n] = e;
            n++;
        }
    }
    for (i = n; i < 0x58; i++) {
        g_btluiItems->throwItems[i] = NULL;
    }
    g_btluiItems->throwCount = n;
}

void btlui_addItemListEntry(s32 arg0) {
    s32 i;
    for (i = 0; i < 31; i++) {
        if (g_btluiItems->items[i] == NULL) {
            g_btluiItems->items[i] = item_findSlot(arg0);
            break;
        }
    }
    g_btluiItems->itemCount++;
}

void btlui_addThrowListEntry(s32 arg0) {
    s32 i;
    for (i = 0; i < 0x58; i++) {
        if (g_btluiItems->throwItems[i] == NULL) {
            g_btluiItems->throwItems[i] = item_findSlot(arg0);
            break;
        }
    }
    g_btluiItems->throwCount++;
}

s32 btlui_findItemListEntry(s32 arg0) {
    s32 i;
    Unk80104FA8 *p = g_btluiItems;
    for (i = 0; i < 31; i++) {
        if (p->items[i] != NULL && arg0 == p->items[i]->unk0) {
            return i;
        }
    }
    return 0xFF;
}

s32 btlui_findThrowListEntry(s32 arg0) {
    s32 i;
    Unk80104FA8 *p = g_btluiItems;
    for (i = 0; i < 0x58; i++) {
        if (p->throwItems[i] != NULL && arg0 == p->throwItems[i]->unk0) {
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
    if (!((g_gameState->config >> 2) & 1)) {
        g_btlCmdMenu.cursor = 0;
    } else {
        g_btlCmdMenu.cursor = e->cursor.cmdRow;
    }
    g_btlCmdMenu.trance = (u8)btl_hasStatus(g_btlUi->cmdUnit, BTL_STATUS_TRANCE) != 0;
    g_btlCmdMenu.charIndex = e->chr->unk34.b.row;
    g_btlCmdMenu.unit = g_btlUi->cmdUnit;
    p->side = 0;
    p->slide = 0;
    btlui_setMenuState(BTLUI_MENU_CLOSING);
    w = win_open(&D_800F8DDC, 0xF, 2, 3);
    g_btlUi->windows[3] = w;
    win_setDrawContent(w, btlui_drawCmdList);
    win_setDrawOverlay(g_btlUi->windows[3], btlui_drawCmdOverlay);
    win_setOnOpened(g_btlUi->windows[3], btlui_onCmdMenuOpened);
    btlui_buildCmdList();
    snd_playSe(0x414);
}

void btlui_drawCmdList(Window *arg0) {
    s32 y = arg0->unk2 + 6;
    s32 x = arg0->unk0 + 0xA;
    s32 i;
    u8 *t;
    if (g_btlCmdMenu.sideCmd.side != 0) {
        btlui_drawSideCmd(REG_PASSTHROUGH(arg0));
    }
    for (i = 0; i < 4; i++) {
        t = g_btlCmdNames + g_btlCmdTable[g_btlCmdMenu.cmdIds[i]].nameOfs;
        if (g_btlCmdMenu.enabled[i] == 0) {
            text_draw(x, y, arg0->unk18, t, 2);
        } else {
            text_draw(x, y, arg0->unk18, t, 0);
        }
        do {
            y += 15;
        } while (0);
    }
}

INCLUDE_RODATA("asm/us/ovl_0b/nonmatchings/btlui", g_battleText);

/* TRANCE label */
INCLUDE_RODATA("asm/us/ovl_0b/nonmatchings/btlui", D_800A857C);

void btlui_drawCmdOverlay(Window *arg0) {
    u8 st = arg0->state;
    RECT *r = &arg0->unk8;
    s32 dy = 10;
    RECT rc;
    if (arg0->state == 3) {
        if (btlui_getMenuState() == BTLUI_MENU_CMD || (btlui_getMenuState() == st && btlui_getTargetReturnState() == BTLUI_MENU_CMD && !(func_800F1360() & 0xFF) && (D_80104E80 & 1))) {
            ui_drawHand(arg0->unk8.x - 0xE, r->y + (g_btlCmdMenu.cursor * 15 + dy), arg0->unk18 - 1);
        }
    }
    if (arg0->state != 3) {
        win_setClip(NULL, arg0->unk18, 4);
    }
    if (g_btlCmdMenu.unit != NULL && !(btl_hasStatus(g_btlCmdMenu.unit, BTL_STATUS_TRANCE) & 0xFF)) {
        ui_drawLabel(arg0->unk0 + 6, r->y - 3, arg0->unk18, g_winLabelStrings.unk30, arg0->unk1E ? 0 : 2);
    } else if (arg0->unk1E != 0) {
        btlui_drawTranceLabel(arg0->unk0 + 6, r->y - 3, arg0->unk18, D_800A857C);
    } else {
        ui_drawLabel(arg0->unk0 + 6, r->y - 3, arg0->unk18, (s32)D_800A857C, 2);
    }
    if (arg0->state != 3) {
        rc.x = r->x;
        rc.y = r->y - 3;
        rc.w = r->w;
        rc.h = 8;
        win_setClip(&rc, arg0->unk18, 4);
    }
}

void btlui_drawSideCmd(Window *arg0) {
    s32 dx2;
    RECT r;
    s32 off = 0;
    s32 dx;
    s16 y;
    s32 x;
    Unk80104FC0 *p = &D_80104FC0;
    Window *w = arg0;

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
        r.x = w->unk0 + x;
    } else {
        r.x = w->unk0 - off;
    }
    y = w->unk2 + g_btlCmdMenu.cursor * 15;
    r.w = 0x3E;
    r.h = 0x11;
    r.y = y + 4;
    text_draw(r.x + dx, r.y + 2, arg0->unk18, text_get(D_800F8DE4[D_80104FC0.side]), 0);
    btlui_drawSideFrame(&r, arg0->unk18);
}

void btlui_confirmCmd(void) {
    Unk800FD56C *e = &g_btlCmdTable[g_btlCmdMenu.cmdIds[g_btlCmdMenu.cursor]];
    void (*f)(Unk800FD56C *);
    if (g_btlCmdMenu.sideCmd.side == 0) {
        if (g_btlCmdMenu.enabled[g_btlCmdMenu.cursor] != 0) {
            do {
                snd_playSe(0x67);
                btlui_saveCursor(&g_btlCmdMenu.unit->menu->cursor, 0, g_btlCmdMenu.cursor, 0);
                f = D_800F8DF0[e->kind];
            } while (0);
        } else {
            snd_playSe(0x66);
            return;
        }
    } else {
        if (g_btlCmdMenu.sideCmd.ready == 0) {
            return;
        }
        snd_playSe(0x67);
        btlui_saveCursor(&g_btlCmdMenu.unit->menu->cursor, 0, g_btlCmdMenu.cursor, 0);
        f = D_800F8E00;
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

void btlui_initCachedCmdIcons(s32 arg0) {
    s32 i;
    s32 u = 0x3C0;
    s32 y = 0x9A;

    for (i = 0; i < 4; i++) {
        setlen(&g_btluiPrims->unk0[i][arg0], 4);
        setcode(&g_btluiPrims->unk0[i][arg0], 0x64);
        setRGB0(&g_btluiPrims->unk0[i][arg0], 0x80, 0x80, 0x80);
        g_btluiPrims->unk0[i][arg0].x0 = 0;
        g_btluiPrims->unk0[i][arg0].y0 = y;
        g_btluiPrims->unk0[i][arg0].u0 = u & 0x3F;
        g_btluiPrims->unk0[i][arg0].v0 = 0xE0;
        g_btluiPrims->unk0[i][arg0].w = 0xE;
        g_btluiPrims->unk0[i][arg0].h = 0x10;
        g_btluiPrims->unk0[i][arg0].clut = 0;
        u += 0x10;
        y += 0x10;
    }
    setlen(&g_btluiPrims->unkA0[arg0], 1);
    *g_btluiPrims->unkA0[arg0].code = get_mode(1, 1, getTPage(2, 0, 0x3C0, 0x100));
}

void btlui_cacheCmdIcons(void) {
    RECT r = D_800A8584;
    s32 x = 0x3C0;
    s32 y = 0x1E0;
    s32 i = 0;
    r.x += g_btluiDispOrigin[g_gameState->gameData->unk8 & 1][0];
    r.y += g_btluiDispOrigin[g_gameState->gameData->unk8 & 1][1];
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
    if (g_btlUi->windows[3]->state == 2 || g_btlUi->windows[3]->state == 3) {
        for (i = 0; i < 4; i++) {
            addPrim(&g_ot[18], &g_btluiPrims->unk0[i][g_drawBufs.cur]);
        }
        addPrim(&g_ot[18], &g_btluiPrims->unkA0[g_drawBufs.cur]);
    }
}

void btlui_drawSideFrame(RECT *r, s32 otz) {
    u32 *ot;
    u32 clut;
    Unk800689B0 *d2;
    Unk800689B0 *d1;
    RECT tw;
    RECT r2;
    u32 w;

    ot = &g_ot[otz];
    clut = win_getPalClut(D_80068E78[0].unk2) << 16;
    d2 = &D_80068E78[2];
    setlen(g_primPtr, 4);
    d1 = &D_80068E78[1];
    ((SprtWords *)g_primPtr)->rgbc = 0x64808080;
    ((SprtWords *)g_primPtr)->xy = r->x | (r->y << 16);
    w = D_80068E78[0].unk0;
    w |= D_80068E78[0].unk1 << 8;
    ((SprtWords *)g_primPtr)->uvclut = w | clut;
    ((SprtWords *)g_primPtr)->wh = D_80068E78[0].unk4 | (D_80068E78[0].unk5 << 16);
    addPrim(ot, g_primPtr);
    g_primPtr += sizeof(SprtWords);

    setlen(g_primPtr, 4);
    ((SprtWords *)g_primPtr)->rgbc = 0x64808080;
    ((SprtWords *)g_primPtr)->xy = (r->x + r->w - d2->unk4) | (r->y << 16);
    ((SprtWords *)g_primPtr)->uvclut = D_80068E78[2].unk0 | (d2->unk1 << 8) | clut;
    ((SprtWords *)g_primPtr)->wh = d2->unk4 | (d2->unk5 << 16);
    addPrim(ot, g_primPtr);
    g_primPtr += sizeof(SprtWords);

    tw.y = 0;
    tw.h = 0xFF;
    tw.x = D_80068E78[1].unk0;
    tw.w = d1->unk4;
    SetTexWindow(g_primPtr, &D_800F8E10);
    addPrim(ot, g_primPtr);
    g_primPtr += 0xC;

    setlen(g_primPtr, 4);
    ((SprtWords *)g_primPtr)->rgbc = 0x64808080;
    ((SprtWords *)g_primPtr)->xy = (r->x + D_80068E78[0].unk4) | (r->y << 16);
    ((SprtWords *)g_primPtr)->uvclut = (d1->unk1 << 8) | clut;
    ((SprtWords *)g_primPtr)->wh = (r->w - (D_80068E78[0].unk4 + d2->unk4)) | (d1->unk5 << 16);
    addPrim(ot, g_primPtr);
    g_primPtr += sizeof(SprtWords);

    setlen(g_primPtr, 2);
    ((DrMode2Words *)g_primPtr)->code[0] = get_mode(1, 1, d1->unk6);
    ((DrMode2Words *)g_primPtr)->code[1] = get_tw(&tw);
    addPrim(ot, g_primPtr);
    g_primPtr += sizeof(DrMode2Words);

    r2.x = r->x + 1;
    r2.y = r->y + 1;
    r2.w = r->w - 3;
    r2.h = r->h - 3;
    win_drawFill(&r2, otz, 0, 0, (g_gameState->config >> 18) & 1);
}

void btlui_drawTranceLabel(s32 x, s32 y, s32 otz, char *str) {
    s32 w;
    u32 wh;
    u32 xy;
    u32 tp;
    u32 stp;

    w = ui_measureLabel(D_800F8E18);
    wh = w | 0x70000;
    setlen(g_primPtr, 3);
    ((TileWords *)g_primPtr)->rgbc = 0x62000000;
    xy = x | (y << 16);
    ((TileWords *)g_primPtr)->xy = xy;
    ((TileWords *)g_primPtr)->wh = wh;
    addPrim(&g_ot[otz], g_primPtr);
    g_primPtr += sizeof(TileWords);
    setlen(g_primPtr, 1);
    tp = get_mode(1, 1, getTPage(0, 1, 0, 0));
    stp = GPU_STP(1, 0);
    ((DrTpageWords *)g_primPtr)->code = tp;
    addPrim(&g_ot[otz], g_primPtr);
    g_primPtr += sizeof(DrTpageWords);
    setlen(g_primPtr, 2);
    ((DrMode2Words *)g_primPtr)->code[0] = stp;
    ((DrMode2Words *)g_primPtr)->code[1] = 0;
    addPrim(&g_ot[otz], g_primPtr);
    g_primPtr += sizeof(DrMode2Words);
    btlui_drawPulseGradient(x, y, otz, w, 7, D_800F8E20);
    setlen(g_primPtr, 2);
    ((DrMode2Words *)g_primPtr)->code[0] = GPU_STP(0, 1);
    ((DrMode2Words *)g_primPtr)->code[1] = 0;
    addPrim(&g_ot[otz], g_primPtr);
    g_primPtr += sizeof(DrMode2Words);
    ui_drawLabel(x, y, otz, (s32)str, 1);
    setlen(g_primPtr, 2);
    ((DrMode2Words *)g_primPtr)->code[0] = GPU_STP(0, 0);
    ((DrMode2Words *)g_primPtr)->code[1] = 0;
    addPrim(&g_ot[otz], g_primPtr);
    g_primPtr += sizeof(DrMode2Words);
    setlen(g_primPtr, 3);
    ((TileWords *)g_primPtr)->rgbc = 0x62000000;
    ((TileWords *)g_primPtr)->xy = xy;
    ((TileWords *)g_primPtr)->wh = wh;
    addPrim(&g_ot[otz], g_primPtr);
    g_primPtr += sizeof(TileWords);
    setlen(g_primPtr, 1);
    ((DrTpageWords *)g_primPtr)->code = tp;
    addPrim(&g_ot[otz], g_primPtr);
    g_primPtr += sizeof(DrTpageWords);
    setlen(g_primPtr, 2);
    ((DrMode2Words *)g_primPtr)->code[0] = stp;
    ((DrMode2Words *)g_primPtr)->code[1] = 0;
    addPrim(&g_ot[otz], g_primPtr);
    g_primPtr += sizeof(DrMode2Words);
    ui_drawLabel(x, y, otz, (s32)str, 0);
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
            snd_playSe(0x67);
            g_btlCmdMenu.cursor = g_btlCmdMenu.cursor != 0 ? g_btlCmdMenu.cursor - 1 : 3;
        } while (g_btlCmdMenu.cmdIds[g_btlCmdMenu.cursor] == 0);
    }
}

void btlui_cmdCursorDown(void) {
    if (g_btlCmdMenu.sideCmd.side == 0) {
        do {
            snd_playSe(0x67);
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

void btlui_selectDirectCmd(Unk800EDE6C *arg0) {
    u32 hidden;
    Unk800EF80C s;
    Battle16CC *e;
    u8 b = g_btlCmdMenu.cmdIds[g_btlCmdMenu.cursor];
    e = &g_gameState->gameData->unk8E4.battle->abilities[arg0->ability];
    s.subNo = arg0->ability;
    s.returnState = 1;
    s.cmdNo = b;
    s.trance = g_btlCmdMenu.trance;
    s.targetType = e->targetType;
    s.defaultSide = e->defaultSide;
    s.koFirst = e->koFirst;
    s.listKind = e->listKind;
    hidden = e->allowKo;
    s.doubleCast = 0;
    s.allowKo = hidden;
    btlui_startTargetSelect(&s);
}

void btlui_openAbilityList(s32 arg0) {
    Unk800EE37C v;
    v.cmdNo = g_btlCmdMenu.cmdIds[g_btlCmdMenu.cursor];
    v.cmd = arg0;
    v.trance = g_btlCmdMenu.trance;
    v.label = g_winLabelStrings.unk4;
    v.label2 = g_winLabelStrings.unk7C;
    btlui_openList(&v);
}

void btlui_openItemList(s32 arg0) {
    Unk800EE37C v;
    v.cmdNo = g_btlCmdMenu.cmdIds[g_btlCmdMenu.cursor];
    v.cmd = arg0;
    v.trance = g_btlCmdMenu.trance;
    v.label = g_winLabelStrings.unk68;
    v.label2 = g_winLabelStrings.unk94;
    btlui_openList(&v);
}

void btlui_openThrowList(s32 arg0) {
    Unk800EE37C v;
    v.cmdNo = g_btlCmdMenu.cmdIds[g_btlCmdMenu.cursor];
    v.cmd = arg0;
    v.trance = g_btlCmdMenu.trance;
    v.label = g_winLabelStrings.unkC0;
    v.label2 = g_winLabelStrings.unk94;
    btlui_openList(&v);
}

void btlui_issueSideCmd(void) {
    s32 v = D_800F8E04[g_btlCmdMenu.sideCmd.side];
    g_btlCmdMenu.unit->inMenuQueue = 0;
    g_btlCmdMenu.unit->cmdIssued = 1;
    btl_setCmd((s32)g_btlCmdMenu.unit->cmd, v, g_btlCmdTable[v].first, g_btlCmdMenu.unit->unitBit, 0);
    btlui_setMenuState(BTLUI_MENU_CLOSING);
    win_setOnClosed(g_btlUi->windows[3], btlui_onSideCmdClosed);
    win_close(g_btlUi->windows[3]);
}

void btlui_onSideCmdClosed(Window *arg0) {
    btlui_setMenuState(BTLUI_MENU_NONE);
    g_btlCmdMenu.sideCmd.ready = 0;
}

void btlui_onPassClosed(Window *arg0) {
    btlui_setMenuState(BTLUI_MENU_NONE);
    g_btlCmdMenu.sideCmd.ready = 0;
    btlui_queueMenuUnit(g_btlCmdMenu.unit);
}

void btlui_slideLeft(void) {
    if (D_80104FC0.side == 2) {
        if (D_80104FC0.slide == 1) {
            if (++D_80104FC0.slideFrame >= 5) {
                D_80104FC0.ready = 1;
                D_80104FC0.slide = 0;
            }
        } else if (D_80104FC0.slide == 2 && (g_padRepeat & PAD_LEFT)) {
            snd_playSe(0x417);
            D_80104FC0.slide = 1;
            D_80104FC0.side = 2;
            D_80104FC0.ready = 0;
            D_80104FC0.slideFrame = 0;
        }
    } else if (D_80104FC0.side == 0) {
        if (g_padRepeat & PAD_LEFT) {
            snd_playSe(0x417);
            D_80104FC0.slide = 1;
            D_80104FC0.side = 2;
            D_80104FC0.ready = 0;
            D_80104FC0.slideFrame = 0;
        }
    } else {
        btlui_slideBack();
    }
}

void btlui_slideRight(void) {
    if (D_80104FC0.side == 1) {
        if (D_80104FC0.slide == 1) {
            if (++D_80104FC0.slideFrame >= 5) {
                D_80104FC0.ready = 1;
                D_80104FC0.slide = 0;
            }
        } else if (D_80104FC0.slide == 2 && (g_padRepeat & PAD_RIGHT)) {
            snd_playSe(0x417);
            D_80104FC0.slide = 1;
            D_80104FC0.side = 1;
            D_80104FC0.ready = 0;
            D_80104FC0.slideFrame = 0;
        }
    } else if (D_80104FC0.side == 0) {
        if (g_padRepeat & PAD_RIGHT) {
            snd_playSe(0x417);
            D_80104FC0.slide = 1;
            D_80104FC0.side = 1;
            D_80104FC0.ready = 0;
            D_80104FC0.slideFrame = 0;
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

void btlui_openList(Unk800EE37C *arg0) {
    MenuList l;
    Unk80104F88Entry *e;
    u8 n;
    u8 f;
    Unk800EB518 *c;

    btlui_setMenuState(BTLUI_MENU_CLOSING);
    g_btluiList.unit = g_btlUi->cmdUnit;
    g_btluiList.cmdNo = arg0->cmdNo;
    g_btluiList.cmd = (struct Unk80104FD4 *)arg0->cmd;
    g_btluiList.label = arg0->label;
    g_btluiList.label2 = arg0->label2;
    g_btluiList.trance = arg0->trance;
    g_btluiList.doubleCast = 0;
    g_btluiList.label2Width = (arg0->label2 != 0) ? ui_measureLabel((u8 *)arg0->label2) : 0;
    g_btluiList.halfMp = chr_hasSupportAbility((g_btluiList.unit->flags >> 1) & 0xF, 0xE2);
    n = g_btluiList.cmd->count;
    menuList_init(&l, n, (n < 5) ? n : 4, 1, 0x44);
    f = (g_gameState->config >> 2) & 1;
    if (f == 1) {
        e = g_btluiList.unit->menu;
        c = &e->cursor;
        l.top = c->listTop[e->cursor.cmdRow];
        l.cursor = c->listCursor[e->cursor.cmdRow];
    }
    g_btlUi->windows[2] = win_open(&D_800F8E30, 8, 1, 2);
    win_setList(g_btlUi->windows[2], &l);
    win_setDrawContent(g_btlUi->windows[2], btlui_drawListContent);
    win_setDrawOverlay(g_btlUi->windows[2], btlui_drawListOverlay);
    win_setOnOpened(g_btlUi->windows[2], (void (*)(Window *))btlui_onListOpened);
}

void btlui_listInput(void) {
    MenuList *l = (MenuList *)&g_btlUi->windows[2]->list;
    Window *w;
    if (l->scroll == 0) {
        if (g_padRepeat & BTN_CIRCLE) {
            snd_playSe(0x65);
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
            if (g_gameState->gameData->unk8E4.battle->escapeHeld == 0) {
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
        s.targetType = g_gameState->gameData->unk8E4.battle->abilities[k].targetType;
        s.defaultSide = g_gameState->gameData->unk8E4.battle->abilities[k].defaultSide;
        s.koFirst = g_gameState->gameData->unk8E4.battle->abilities[k].koFirst;
        s.listKind = g_gameState->gameData->unk8E4.battle->abilities[k].listKind;
        s.allowKo = g_gameState->gameData->unk8E4.battle->abilities[k].allowKo;
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
    Unk8006A774 *e = &D_8006A774[p->unk0];
    s32 n;

    if (p == NULL) {
        return 0;
    }
    if ((n = p->unk1) - btlui_getItemReserved(p->unk0) >= 0 && n != btlui_getItemReserved(p->unk0)) {
        s.returnState = 2;
        s.cmdNo = g_btluiList.cmdNo;
        s.subNo = p->unk0;
        s.targetType = e->unk0_0;
        s.defaultSide = e->unk0_4;
        s.koFirst = e->unk0_31;
        s.listKind = e->unk0_5;
        s.allowKo = e->unk0_29;
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
    Battle16CC *e = &g_gameState->gameData->unk8E4.battle->abilities[g_btluiList.cmd->first];
    s32 n;

    if (p == NULL) {
        return 0;
    }
    if ((n = p->unk1) - btlui_getItemReserved(p->unk0) >= 0 && n != btlui_getItemReserved(p->unk0)) {
        s.returnState = 2;
        s.cmdNo = g_btluiList.cmdNo;
        s.subNo = p->unk0;
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

s32 btlui_getAbilityState(s32 arg0) {
    Unk80104F88Entry *e = g_btluiList.unit->menu;
    Battle16CC *a = &g_gameState->gameData->unk8E4.battle->abilities[arg0];

    if (e->learns != 0 && e->fromEquip[arg0] == 0 && (s16)*e->apCounters[arg0] < (s16)e->apNeeded[arg0]) {
        return 0;
    }
    if (a->flags & 2) {
        if ((u8)btl_hasStatus(g_btluiList.unit, BTL_STATUS_SILENCE)) {
            return 1;
        }
        if ((g_gameState->gameData->unk8E4.battle->scene.flags >> 9) & 1) {
            return 1;
        }
    }
    if (btl_getMpCost((BtlCmd8 *)a) > g_btluiList.unit->mp) {
        return 1;
    }
    return 2;
}

void btlui_drawListOverlay(Window *arg0) {
    RECT *r = &arg0->unk8;
    s32 x;
    s32 y;
    s32 dy = 10;
    s32 dx = 0x74;
    if (g_btluiList.label != 0) {
        ui_drawLabel(arg0->unk8.x + 6, r->y - 4, arg0->unk18, g_btluiList.label, 0);
    }
    if (g_btluiList.label2 != 0) {
        ui_drawLabel(arg0->unk8.x - (g_btluiList.label2Width - dx), r->y - 4, arg0->unk18, g_btluiList.label2, 0);
    }
    if (btlui_getMenuState() == BTLUI_MENU_LIST || (btlui_getMenuState() == BTLUI_MENU_TARGET && btlui_getTargetReturnState() == BTLUI_MENU_LIST && !(func_800F1360() & 0xFF) && (D_80104E80 & 1))) {
        x = arg0->unk8.x - 0x12;
        y = r->y + (arg0->list.cursor * 15 + dy);
        if (g_btluiList.cmdNo == BTL_CMD_DBL_BLK || g_btluiList.cmdNo == BTL_CMD_DBL_WHT) {
            ui_drawSprite(x + 7, y + 2, arg0->unk18, g_btluiList.doubleCast + 0x5A, 0);
        }
        ui_drawHand(x, y, arg0->unk18);
    }
}

void btlui_updateList(void) {
    if (g_btluiList.cmdNo == BTL_CMD_SWD_MAG) {
        if (g_btlUi->swdMagPartner == 0 || g_btlUi->swdMagBlocked.h != 0) {
            btlui_closeList();
            snd_playSe(0x65);
        }
    }
    if (g_btluiList.trance == 0 && (u8)btl_hasStatus(g_btluiList.unit, BTL_STATUS_TRANCE)) {
        btlui_closeList();
        snd_playSe(0x65);
    }
}

void btlui_nextDoubleCast(void) {
    g_btluiList.doubleCast++;
}

s32 btlui_getListHelpText(void) {
    s32 r = 0;
    s32 (*f)(void) = D_800F8E58[g_btluiList.cmd->kind];
    if (f != NULL) {
        r = f();
    }
    return r;
}

void btlui_listConfirm(void) {
    Window *w;
    if ((u8)D_800F8E38[g_btluiList.cmd->kind]()) {
        snd_playSe(0x67);
        w = g_btlUi->windows[2];
        btlui_saveCursor(&g_btluiList.unit->menu->cursor, 1, w->list.top, w->list.cursor);
    } else {
        snd_playSe(0x66);
    }
}

void btlui_drawListContent(Window *arg0) {
    RECT r;
    MenuView v;
    RECT *rc = &arg0->unk8;
    s32 i;
    menuList_getView((MenuList *)&arg0->list, &v);
    win_setClip(0, 7, 1);
    for (i = 0; i < v.unk0; i++) {
        btlui_drawListRow(0, i, v.unk1 + i, &v);
    }
    if (((MenuList *)&arg0->list)->scroll & 4) {
        menuList_getRowsRect((MenuList *)&arg0->list, rc, &r);
        win_setClip(&r, 7, 1);
        btlui_drawListScroll(arg0);
    }
    win_setClip(rc, 7, 1);
}

void btlui_drawListScroll(Window *arg0) {
    MenuView v;
    s32 i;
    menuList_getPageView((MenuList *)&arg0->list, &v);
    for (i = 0; i < v.unk0; i++) {
        btlui_drawListRow(0, i, v.unk1 + i, &v);
    }
}

void btlui_onListOpened(void) {
    btlui_setMenuState(BTLUI_MENU_LIST);
    g_btlUi->windows[3]->unk1E = 0;
}

void btlui_closeList(void) {
    btlui_setMenuState(BTLUI_MENU_CLOSING);
    win_setOnClosed(g_btlUi->windows[2], btlui_onListClosed);
    win_close(g_btlUi->windows[2]);
}

void btlui_drawListRow(s32 arg0, s32 arg1, s32 arg2, MenuView *arg3) {
    D_800F8E48[g_btluiList.cmd->kind](arg0, arg1, arg2, arg3);
}

void btlui_drawAbilityRow(s32 arg0, s32 arg1, s32 arg2, MenuView *arg3) {
    u8 buf[8];
    u8 k = g_btlCmdAbilities[g_btluiList.cmd->first + arg2];
    Window *w = g_btlUi->windows[2];
    RECT *r = &w->unk8;
    s32 dy = w->unk6 - r->h;
    s32 res = btlui_getAbilityState(k);
    s16 x;
    s32 y;
    Battle16CC *a;
    s32 c;

    if (res != 0) {
        x = r->x;
        a = &g_gameState->gameData->unk8E4.battle->abilities[k];
        y = r->y + arg1 * 15 + arg3->unk2 - dy;
        c = (res != 2) * 2;
        text_draw(x + 6, y, 7, D_800F909C + a->nameOfs, c);
        if (a->mpCost != 0) {
            text_formatNumber(btl_getMpCost((BtlCmd8 *)a), buf, 3, 1);
            ui_drawSmallDigits(x + 0x60, y + 4, 7, buf, c);
        }
    }
}

void btlui_drawItemRow(s32 arg0, s32 arg1, s32 arg2, Window *arg3) {
    u8 buf[8];
    s32 x5;
    UnkPair *e = btlui_getItemListEntry(arg2);
    Window *w = g_btlUi->windows[2];
    RECT *r = &w->unk8;
    s32 dy = w->unk6 - r->h;
    s32 n;
    u8 id;
    s16 x;
    s32 y;
    Unk80069018 *it;
    if (e != NULL) {
        if (e->unk1 - btlui_getItemReserved(e->unk0) >= 0) {
            n = e->unk1 - btlui_getItemReserved(e->unk0);
        } else {
            n = 0;
        }
        if (n != 0) {
            id = e->unk0;
            x = r->x;
            it = &g_itemTable[id];
            x5 = x + 5;
            y = r->y + arg1 * 15 + arg3->unk2 - dy;
            ui_drawItemIcon(x5, y, 7, id, 1);
            text_draw(x + 0x14, y, 7, g_itemNames + it->unk0, 0);
            text_formatNumber(n, buf, 2, 1);
            ui_drawSmallDigits(x + 0x67, y + 4, 7, buf, 0);
        }
    }
}

void btlui_drawThrowRow(s32 arg0, s32 arg1, s32 arg2, Window *arg3) {
    u8 buf[8];
    UnkPair *e = btlui_getThrowListEntry(arg2);
    Window *w = g_btlUi->windows[2];
    RECT *r = &w->unk8;
    s32 dy = w->unk6 - r->h;
    s32 n;
    s32 x;
    s32 y;
    Unk80069018 *it;
    if (e != NULL) {
        if (e->unk1 - btlui_getItemReserved(e->unk0) >= 0) {
            n = e->unk1 - btlui_getItemReserved(e->unk0);
        } else {
            n = 0;
        }
        if (n != 0) {
            x = r->x;
            it = &g_itemTable[e->unk0];
            y = r->y + arg1 * 15 + arg3->unk2 - dy;
            text_formatNumber(n, buf, 2, 1);
            text_draw(x + 5, y, 7, g_itemNames + it->unk0, 0);
            ui_drawSmallDigits(x + 0x67, y + 4, 7, buf, 0);
        }
    }
}

s32 btlui_resolveAbility(s32 arg0) {
    s32 t;
    if (arg0 == 0x44) {
        switch (g_btluiList.unit->menu->chr->equip[4]) {
        case 0xE4:
            arg0 = 0x45;
            break;
        case 0xE5:
            arg0 = 0x46;
            break;
        case 0xE3:
            arg0 = 0x47;
            break;
        }
    } else if (arg0 == 0x42) {
        t = g_btluiList.unit->menu->chr->equip[4] == 0xDE;
        arg0 = t + 0x42;
    }
    return arg0;
}

s32 btl_getMpCost(BtlCmd8 *arg0) {
    s32 cost = arg0->mpCost;
    if (arg0->unkB & 4) {
        if (D_8007AF02 != 0) {
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
    return D_800F96E4 + D_800FB1F0[k].unk0;
}

u8 *btlui_getItemHelpText(void) {
    Unk16 *w = &g_btlUi->windows[2]->list;
    UnkPair *e = btlui_getItemListEntry(w->cursor + w->top);
    if (e != NULL) {
        if (e->unk1 - btlui_getItemReserved(e->unk0) >= 0) {
            if (e->unk1 != btlui_getItemReserved(e->unk0)) {
                return D_800FB4F0 + D_800FD36C[e->unk0];
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
        if (e->unk1 - btlui_getItemReserved(e->unk0) >= 0) {
            if (e->unk1 != btlui_getItemReserved(e->unk0)) {
                return D_800FB4F0 + D_800FD36C[e->unk0];
            }
            return NULL;
        }
    }
    return NULL;
}

void btlui_onListClosed(Window *arg0) {
    btlui_setMenuState(BTLUI_MENU_CMD);
    g_btlUi->windows[3]->unk1E = 1;
}

void btlui_startTargetSelect(Unk800EF80C *arg0) {
    s32 i;

    g_btlTarget.unit = g_btlUi->cmdUnit;
    g_btlTarget.returnState = arg0->returnState;
    g_btlTarget.cmdNo = arg0->cmdNo;
    g_btlTarget.targetType = arg0->targetType;
    g_btlTarget.defaultSide = arg0->defaultSide;
    g_btlTarget.koFirst = arg0->koFirst;
    g_btlTarget.side = arg0->defaultSide;
    g_btlTarget.subNo = arg0->subNo;
    g_btlTarget.listKind = arg0->listKind;
    g_btlTarget.allowKo = arg0->allowKo;
    g_btlTarget.doubleCast = arg0->doubleCast;
    g_btlTarget.targetAll = D_800F8E84[g_btlTarget.targetType];
    g_btlTarget.windowOpen = 0;
    g_btlTarget.halfMp = chr_hasSupportAbility((g_btlTarget.unit->flags >> 1) & 0xF, 0xE2);
    g_btlTarget.trance = arg0->trance;
    g_btlTarget.unk7 = (u32)(arg0->targetType - 6) < 3;
    g_btlTarget.canSwitchSide = arg0->targetType == 0 || arg0->targetType == 3 || arg0->targetType == 9 || arg0->targetType == 6;
    g_btlTarget.canToggleAll = (u32)(arg0->targetType - 3) < 3;
    g_btluiCursors.partyCount = 0;
    g_btluiCursors.count = 0;
    btlui_clearTargeted();
    btlui_setDefaultTargets(1);
    if (g_btlTarget.listKind == 0) {
        if (D_80104E6B == 0) {
            btlui_setMenuState(BTLUI_MENU_TARGET);
        } else {
            btlui_openTargetWindow(3);
        }
    } else {
        if (D_80104E6B != 0) {
            btlui_openTargetWindow(5);
        }
        btlui_openListWindow(g_btlTarget.listKind);
    }
    for (i = 0; i < 2; i++) {
        pad_setRepeatEnabled(i, 2, 1);
        pad_setRepeatEnabled(i, 3, 1);
    }
}

void btlui_setDefaultTargets(s32 arg0) {
    BtlUnit *u;
    if (g_btlTarget.targetType == 0xC) {
        for (u = g_gameState->gameData->unk8E4.battle->unitList; u != 0; u = u->next) {
            if (((u->flags >> 28) & 1) && (!(u->status & BTL_STATUS_KO) || g_btlTarget.allowKo != 0)) {
                u->flags |= 0x20000000;
            }
        }
    } else if (g_btlTarget.targetType == 0xD) {
        g_btlTarget.unit->flags |= 0x20000000;
    } else if (arg0 & 0xFF) {
        btlui_targetDefault(D_800F8E68[g_btlTarget.targetType][0], D_800F8E68[g_btlTarget.targetType][1]);
    } else {
        btlui_targetFirst(D_800F8E68[g_btlTarget.targetType][0], D_800F8E68[g_btlTarget.targetType][1]);
    }
}

s32 btlui_cycleTarget(s32 arg0) {
    BtlUnit *list[4];
    BtlUnit *p;
    BtlUnit *u;
    s32 cur = 0;
    s32 n;
    s32 k;
    if (g_btlTarget.targetAll != 0) {
        return 0;
    }
    if (g_btlTarget.targetType == 0xD) {
        return 0;
    }
    u = BtlFindTargetCursor();
    if (u == NULL) {
        return 0;
    }
    memset((s32)list, 0, 0x10);
    n = 0;
    for (p = g_gameState->gameData->unk8E4.battle->unitList; p != NULL; p = p->next) {
        if (((p->flags >> 28) & 1) && (p->flags & 1) == g_btlTarget.side && (!(p->status & BTL_STATUS_KO) || g_btlTarget.allowKo != 0)) {
            list[n] = p;
            if (p == u) {
                cur = n;
            }
            n++;
        }
    }
    if (n >= 2) {
        u->flags &= ~0x20000000;
        if (!(arg0 & 0xFF)) {
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
        list[k]->flags |= 0x20000000;
        return 1;
    }
    return 0;
}

s32 btlui_switchTargetSide(u8 arg0) {
    BtlUnit *cur;
    BtlUnit *u;
    BtlUnit *t;
    BtlUnit *list[4];
    s32 n;
    s32 i;
    u8 side;
    s32 k;

    for (u = g_gameState->gameData->unk8E4.battle->unitList; u != NULL; u = u->next) {
        if ((u->flags & 0x30000000) == 0x30000000) {
            cur = u;
            goto found;
        }
    }
    cur = NULL;
found:
    if (g_btlTarget.canSwitchSide != 0) {
        if (arg0 == (k = cur->flags & 1)) {
            return 0;
        }
        side = !k;
        g_btlTarget.side = side;
        memset(list, 0, sizeof(list));
        n = 0;
        for (t = g_gameState->gameData->unk8E4.battle->unitList; t != NULL; t = t->next) {
            if (((t->flags >> 28) & 1) && (t->flags & 1) == side) {
                if (!(t->status & BTL_STATUS_KO) || g_btlTarget.allowKo) {
                    list[n++] = t;
                }
            }
        }
        if (n != 0) {
            cur->flags &= ~0x20000000;
            if (g_btlTarget.targetAll == 0) {
                list[0]->flags |= 0x20000000;
            } else {
                btlui_clearTargeted();
                for (i = 0; i < n; i++) {
                    list[i]->flags |= 0x20000000;
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
            if ((g_btlTarget.prevTarget->flags >> 29) & 1) {
                u = g_btlTarget.prevTarget;
            } else {
                u = BtlFindTargetCursor();
            }
            btlui_clearTargeted();
            u->flags |= 0x20000000;
            g_btlTarget.targetAll = 0;
        } else {
            p = g_gameState->gameData->unk8E4.battle->unitList;
            u = BtlFindTargetCursor();
            n = 0;
            g_btlTarget.prevTarget = u;
            btlui_clearTargeted();
            for (; p != NULL; p = p->next) {
                if (((p->flags >> 28) & 1) && (p->flags & 1) == (u->flags & 1) && (!(p->status & BTL_STATUS_KO) || g_btlTarget.allowKo != 0)) {
                    p->flags |= 0x20000000;
                    n++;
                }
            }
            if (n != 0) {
                g_btlTarget.targetAll = 1;
            }
        }
        if (old != g_btlTarget.targetAll) {
            snd_playSe(0x67);
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
        b = &g_gameState->gameData->unk8E4.battle->abilities[g_btlTarget.subNo];
        cost = b->mpCost;
        if (g_btlTarget.halfMp != 0) {
            cost >>= 1;
        }
        if (g_btlTarget.unit->mp < cost) {
            return 0;
        }
        if ((b->flags & 2) && (btl_hasStatus(g_btlTarget.unit, BTL_STATUS_SILENCE) & 0xFF)) {
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
            u->flags &= ~0x20000000;
            btlui_setDefaultTargets(0);
            return 1;
        }
        return 1;
    }
    if (g_btlTarget.targetType != 0xC) {
        p = g_gameState->gameData->unk8E4.battle->unitList;
        n = 0;
        btlui_clearTargeted();
        while (p != NULL) {
            if (((p->flags >> 28) & 1) && (p->flags & 1) == (u->flags & 1) && (!(p->status & BTL_STATUS_KO) || g_btlTarget.allowKo != 0)) {
                p->flags |= 0x20000000;
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
    s32 mask = 0;
    u8 ok = 1;
    BtlUnit *u = g_gameState->gameData->unk8E4.battle->unitList;
    Unk800FD56C *e = &g_btlCmdTable[g_btlTarget.cmdNo];
    Unk80104FF0 *d;
    for (; u != NULL; u = u->next) {
        if ((u->flags & 0x30000000) == 0x30000000) {
            mask |= u->unitBit;
        }
    }
    d = &g_btlTarget;
    btlui_setMenuState(BTLUI_MENU_CLOSING);
    if (d->cmdNo == BTL_CMD_DBL_BLK || d->cmdNo == BTL_CMD_DBL_WHT) {
        ok = btlui_confirmDoubleCast(mask);
    } else {
        d->unit->cmdIssued = 1;
        d->unit->inMenuQueue = 0;
        btl_setCmd((s32)g_btlTarget.unit->cmd, g_btlTarget.cmdNo, g_btlTarget.subNo, mask & 0xFFFF, btlui_getTargeting_inline());
    }
    btlui_clearTargeted();
    if (ok) {
        if ((u32)(e->kind - 2) < 2) {
            btlui_reserveItem(g_btlTarget.subNo);
        }
        btlui_closeWindows();
    }
    snd_playSe(0x67);
}

s32 btlui_confirmDoubleCast(s32 arg0) {
    BtlUnit *u;
    s32 n;
    s8 v;
    s32 r = 0;
    if (g_btlTarget.doubleCast == 0) {
        g_btlTarget.firstTargets = arg0;
        g_btlTarget.firstCmdNo = g_btlTarget.cmdNo;
        g_btlTarget.firstSubNo = g_btlTarget.subNo;
        if ((u32)(g_btlTarget.targetType - 9) < 3) {
            v = 2;
        } else {
            n = 0;
            for (u = g_gameState->gameData->unk8E4.battle->unitList; u != NULL; u = u->next) {
                if ((u->flags & 0x30000000) == 0x30000000) {
                    n++;
                }
            }
            v = n >= 2;
        }
        g_btlTarget.firstTargeting = v;
        btlui_cancelTarget();
        btlui_nextDoubleCast();
    } else {
        g_btlTarget.unit->inMenuQueue = 0;
        g_btlTarget.unit->cmdIssued = 1;
        btl_setCmd((s32)g_btlTarget.unit->delayedCmd, g_btlTarget.firstCmdNo, g_btlTarget.firstSubNo, (u16)g_btlTarget.firstTargets, g_btlTarget.firstTargeting);
        if ((u32)(g_btlTarget.targetType - 9) < 3) {
            v = 2;
        } else {
            n = 0;
            for (u = g_gameState->gameData->unk8E4.battle->unitList; u != NULL; u = u->next) {
                if ((u->flags & 0x30000000) == 0x30000000) {
                    n++;
                }
            }
            v = n >= 2;
        }
        btl_setCmd((s32)g_btlTarget.unit->cmd, g_btlTarget.cmdNo, g_btlTarget.subNo, arg0 & 0xFFFF, v);
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
    for (u = g_gameState->gameData->unk8E4.battle->unitList; u != 0; u = u->next) {
        u->flags &= ~0x20000000;
    }
    for (i = 0; i < 2; i++) {
        pad_setRepeatEnabled(i, 2, 0);
        pad_setRepeatEnabled(i, 3, 0);
    }
}

void btlui_updateTargetWindow(void) {
    if ((g_btlTarget.windowOpen == 0 || (u32)(g_btlUi->windows[0]->state - 1) >= 2) && g_btlTarget.windowOpen != D_80104E6B) {
        if (D_80104E6B != 0) {
            if (g_btlTarget.windowOpen == 0) {
                btlui_setMenuState(BTLUI_MENU_CLOSING);
                g_btlUi->windows[0] = win_open(&D_800F8EF4, 5, 2, 0);
                win_setOpenAnim(g_btlUi->windows[0], 4);
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

void btlui_drawTargetList(Window *arg0) {
    s32 y1 = arg0->unk2 + 8;
    s32 y2 = y1;
    BtlUnit *u = g_gameState->gameData->unk8E4.battle->unitList;
    s32 m;
    u32 f;
    u8 *name;
    s32 c;
    Window *w = arg0;
    ui_drawEtchedLine(arg0->unk0 + 0x6E, arg0->unk2 + 4, arg0->unk18, 9 - arg0->unk6);
    m = text_getColor();
    for (; u != NULL; u = u->next) {
        name = btl_getUnitName(u);
        f = u->flags;
        if ((f >> 28) & 1) {
            if (f & 1) {
                if (u->hp != 0) {
                    c = ((u16)(u->max->maxHp / 6) >= u->hp) * 4;
                } else {
                    c = 3;
                }
                text_draw(w->unk0 + 0x7A, y2, arg0->unk18, name, c);
                y2 += 15;
            } else {
                c = ((u8)btl_hasStatus(u, BTL_STATUS_KO) != 0) * 2;
                text_drawBlock(w->unk0 + 0xC, y1, arg0->unk18, name, c);
                y1 += 15;
            }
        }
    }
    text_setColor(m);
}

void btlui_drawTargetListOverlay(Window *arg0) {
    Window *w = arg0;
    s32 n = 0;
    BtlUnit *u = g_gameState->gameData->unk8E4.battle->unitList;
    RECT *r = &arg0->unk8;
    BtlUnit *p;
    RECT rc;
    s32 yl;
    s32 yr;
    for (p = u; p != NULL; p = p->next) {
        if ((p->flags >> 29) & 1) {
            n++;
        }
    }
    if (arg0->state != 3) {
        win_setClip(NULL, arg0->unk18, 4);
    }
    ui_drawLabel(w->unk0 + 8, r->y - 3, arg0->unk18, g_winLabelStrings.unk3C, 0);
    ui_drawLabel(w->unk0 + 0x76, r->y - 3, arg0->unk18, g_winLabelStrings.unkA0, 0);
    if (arg0->state != 3) {
        rc.x = r->x;
        rc.y = r->y - 3;
        rc.w = r->w;
        rc.h = 8;
        win_setClip(&rc, arg0->unk18, 4);
    }
    if (arg0->state == 3 && (n < 2 || !(D_80104E80 & 1))) {
        yl = r->y + 0xC;
        yr = yl;
        for (; u != NULL; u = u->next) {
            if ((u->flags >> 28) & 1) {
                if ((u->flags >> 29) & 1) {
                    if (u->flags & 1) {
                        ui_drawHand(r->x + 0x62, yr, arg0->unk18 - 1);
                    } else {
                        ui_drawHand(r->x - 0xC, yl, arg0->unk18 - 1);
                    }
                }
                if (u->flags & 1) {
                    yr += 15;
                } else {
                    yl += 15;
                }
            }
        }
    }
}

void btlui_drawTargetCursors(void) {
    u32 c = 0x64808080;
    Unk80105020 *b = &g_btluiCursors;
    u32 *clut = &D_800686D8;
    s32 i;
    SprtWords *p;
    DR_TPAGE *t;
    if (b->count >= 2 && !(D_80104E80 & 3)) {
        c = 0x64404040;
    }
    for (i = 0; i < b->count; i++) {
        setlen(g_primPtr, 4);
        p = (SprtWords *)g_primPtr;
        p->rgbc = c;
        p->xy = (u16)b->pos[i][0] | (b->pos[i][1] << 16);
        p->uvclut = *clut;
        p->wh = 0xE0018;
        addPrim(&g_ot[17], p);
        g_primPtr = (u8 *)(p + 1);
    }
    setlen(g_primPtr, 1);
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

    if (g_btlTarget.targetType < 3) {
        t = find_target();
        if (t == NULL) {
            return NULL;
        }
        text_copy(g_btluiHelpText, text_get(g_btlTarget.targetType + 5));
        text_append(g_btluiHelpText, btl_getUnitName(t));
    } else if ((u8)(g_btlTarget.targetType - 3) < 3) {
        n = 0;
        k = g_btlTarget.targetType - 3;
        for (u = g_gameState->gameData->unk8E4.battle->unitList; u != NULL; u = u->next) {
            if ((u->flags >> 29) & 1) {
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

void btlui_onUnitTargetable(BtlUnit *arg0) {
    if (g_btlTarget.targetAll != 0 && D_800F8EE4[g_btlTarget.targetType] != 0) {
        btlui_setDefaultTargets(0);
    }
}

void btlui_onUnitUntargetable(BtlUnit *arg0) {
    if ((arg0->flags >> 29) & 1) {
        arg0->flags &= ~0x20000000;
        if (g_btlTarget.targetAll == 0) {
            btlui_setDefaultTargets(0);
        }
    }
}

void btlui_targetInput(void) {
    u32 v;
    if (g_padRepeat & BTN_CIRCLE) {
        snd_playSe(0x65);
        btlui_cancelTarget();
        return;
    }
    if (g_padRepeat & BTN_CROSS) {
        btlui_confirmTarget();
        return;
    }
    if (g_gameState->gameData->unk8E4.battle->escapeHeld == 0 && (g_padRepeat & (BTN_L1 | BTN_R1))) {
        btlui_toggleTargetAll();
        return;
    }
    if (g_padRepeat & BTN_L2) {
        snd_playSe(0x67);
        v = g_gameState->config;
        g_gameState->config = (v & ~0x80000) | ((((v >> 19) & 1) ^ 1) << 19);
        return;
    }
    btlui_targetMove();
}

void btlui_updateTarget(void) {
    if (!(btlui_isCmdUsable() & 0xFF)) {
        snd_playSe(0x65);
        btlui_cancelTarget();
    } else if (btlui_refreshTargets() & 0xFF) {
        btlui_updateTargetWindow();
    }
}

u8 func_800F1360(void) {
    return g_btlTarget.windowOpen;
}

Window *btlui_getTargetWindow(void) {
    return g_btlUi->windows[0];
}

s32 func_800F1388(void) {
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
    BtlUnit *u = g_gameState->gameData->unk8E4.battle->unitList;
    s32 n = 0;
    while (u != NULL) {
        if ((u->flags >> 29) & 1) {
            n++;
        }
        u = u->next;
    }
    return n;
}

void btlui_clearTargeted(void) {
    BtlUnit *u = g_gameState->gameData->unk8E4.battle->unitList;
    while (u != NULL) {
        u->flags &= ~0x20000000;
        u = u->next;
    }
}

void btlui_collectCursorPositions(void) {
    s32 n;
    s32 m;
    BtlUnit *u;
    Unk80105020 *p;
    m = n = 0;
    u = g_gameState->gameData->unk8E4.battle->unitList;
    p = &g_btluiCursors;
    if (D_80104E6A != 0) {
        for (; u != NULL; u = u->next) {
            if (((u->flags >> 28) & 1) && u->onScreen != 0) {
                p->pos[n][0] = u->screenX - 0x18;
                p->pos[n][1] = u->screenY - 7;
                n++;
                if (u->flags & 1) {
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
        m = func_800C2DF4();
    } else {
        m = 0;
    }
    for (i = 0; i < 4; i++) {
        if (g_padRepeat & D_800F8EA4[m][i]) {
            D_800F8E94[i]();
        }
    }
}

void btlui_targetPrev(void) {
    if (btlui_cycleTarget(0) & 0xFF) {
        snd_playSe(0x67);
    }
}

void btlui_targetNext(void) {
    if (btlui_cycleTarget(1) & 0xFF) {
        snd_playSe(0x67);
    }
}

void btlui_targetEnemySide(void) {
    if (btlui_switchTargetSide(0) & 0xFF) {
        snd_playSe(0x67);
    }
}

void btlui_targetPartySide(void) {
    if (btlui_switchTargetSide(1) & 0xFF) {
        snd_playSe(0x67);
    }
}

void btlui_targetFirst(u8 arg0, u8 arg1) {
    BtlUnit *u = g_gameState->gameData->unk8E4.battle->unitList;
    if (arg0 == 0xFF) {
        arg0 = g_btlTarget.side;
    }
    for (; u != NULL; u = u->next) {
        if (((u->flags >> 28) & 1) && (u->flags & 1) == arg0) {
            if (!(u->status & BTL_STATUS_KO) || g_btlTarget.allowKo != 0) {
                u->flags |= 0x20000000;
                if (arg1 == 0) {
                    return;
                }
            }
        }
    }
}

void btlui_targetDefault(u8 arg0, u8 arg1) {
    BtlUnit *u = g_gameState->gameData->unk8E4.battle->unitList;
    u8 found = 0;
    BtlUnit *t;
    u32 f;
    u32 side;

    if (arg0 == 0xFF) {
        arg0 = g_btlTarget.side;
    }
    if (g_btlTarget.koFirst != 0) {
        t = btlui_findKoUnit(arg0);
        if (t != NULL) {
            t->flags |= 0x20000000;
            return;
        }
    }
    for (; u != NULL; u = u->next) {
        f = u->flags;
        side = f & 1;
        if (!((f >> 28) & 1) || side != arg0) {
            continue;
        }
        if ((u->status & BTL_STATUS_KO) && g_btlTarget.allowKo == 0) {
            continue;
        }
        if (arg1 == 0 && side != 0) {
            if (u == g_btlTarget.unit) {
                u->flags = f | 0x20000000;
                found = 1;
            }
        } else {
            found = 1;
            u->flags |= 0x20000000;
        }
        if (arg1 == 0 && found) {
            return;
        }
    }
}

BtlUnit *btlui_findKoUnit(u8 arg0) {
    BtlUnit *u;
    for (u = g_gameState->gameData->unk8E4.battle->unitList; u != 0; u = u->next) {
        if (((u->flags >> 28) & 1) && (u->flags & 1) == arg0 && (u->status & BTL_STATUS_KO)) {
            return u;
        }
    }
    return 0;
}

void btlui_openTargetWindow(s8 arg0) {
    Window *w;
    btlui_setMenuState(BTLUI_MENU_CLOSING);
    w = win_open(&D_800F8EF4, 5, 2, 0);
    g_btlUi->windows[0] = w;
    win_setOpenAnim(w, 4);
    win_setOnOpened(g_btlUi->windows[0], btlui_onTargetWindowOpened);
    win_setDrawContent(g_btlUi->windows[0], btlui_drawTargetList);
    win_setDrawOverlay(g_btlUi->windows[0], btlui_drawTargetListOverlay);
    g_btlTarget.openState = arg0;
    g_btlTarget.windowOpen = 1;
}

void btlui_closeTargetWindow(s32 arg0) {
    if (g_btlTarget.windowOpen != 0) {
        g_btlTarget.closeState = arg0;
        btlui_setMenuState(BTLUI_MENU_CLOSING);
        win_setOnClosed(g_btlUi->windows[0], btlui_onTargetWindowClosed);
        win_close(g_btlUi->windows[0]);
    }
}

void btlui_onTargetWindowOpened(Window *arg0) {
    if (g_btlTarget.returnState == BTLUI_MENU_LIST) {
        win_setVisible(g_btlUi->windows[2], 0);
    }
    win_setVisible(g_btlUi->windows[3], 0);
    btlui_setMenuState(g_btlTarget.openState);
}

void btlui_onTargetWindowClosed(Window *arg0) {
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
    for (u = g_gameState->gameData->unk8E4.battle->unitList; u != 0; u = u->next) {
        if ((u->flags & 0x30000000) == 0x30000000) {
            n++;
        }
    }
    return n >= 2;
}

void btlui_drawListCursors(Window *arg0) {
    Window *w = arg0;
    s32 n = 0;
    BtlUnit *u = g_gameState->gameData->unk8E4.battle->unitList;
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

    if (b->partyCount == 0 || arg0->state != 3 || m->shown == 0) {
        return;
    }
    c = 0x64808080;
    if (b->partyCount >= 2 && !(D_80104E80 & 3)) {
        c = 0x64404040;
    }
    y = w->unk2 + 5;
    x = w->unk0 - 0xE;
    for (; u != NULL; u = u->next) {
        if (!(u->flags & 1)) {
            continue;
        }
        if (u == b->party[n]) {
            for (k = 0, yy = y; k < 2; k++, yy += 6) {
                d = &D_80068EE8[k];
                setlen(g_primPtr, 4);
                p = (SprtWords *)g_primPtr;
                p->rgbc = c;
                p->xy = x | (yy << 16);
                p->uvclut = d->uvclut;
                p->wh = d->w | (d->h << 16);
                addPrim(&g_ot[arg0->unk18 - 1], p);
                g_primPtr = (u8 *)(p + 1);
            }
            n++;
            if (n >= b->partyCount) {
                break;
            }
        }
        y += 0xC;
    }
    setlen(g_primPtr, 1);
    t = (DR_TPAGE *)g_primPtr;
    t->code[0] = get_mode(1, 1, D_80068EEE);
    addPrim(&g_ot[arg0->unk18], t);
    g_primPtr = (u8 *)(t + 1);
}

void btlui_drawHpList(Window *arg0) {
    u8 a[8];
    u8 b[8];
    BtlUnit *u;
    Window *w = arg0;
    s32 y = arg0->unk2 + 6;
    s32 dy = arg0->unk6 - arg0->unk8.h;
    s32 c;
    for (u = g_gameState->gameData->unk8E4.battle->unitList; u != 0 && (u->flags & 1); u = u->next) {
        c = 3;
        if (u->hp != 0) {
            c = ((u->status & BTL_STATUS_LOW_HP) != 0) * 4;
        }
        b[0] = 0x2F;
        text_formatNumber(u->hp, a, 4, 1);
        text_formatNumber(u->max->maxHp, b + 1, -1, 1);
        ui_drawDigits(w->unk0 + 4, y, arg0->unk18, a, c);
        ui_drawDigits(w->unk0 + 0x20, y - dy, arg0->unk18, b, c);
        y += 12;
    }
}

void btlui_drawMpList(Window *arg0) {
    u8 hp[8];
    u8 max[8];
    Window *w = arg0;
    s32 y = arg0->unk2 + 6;
    BtlUnit *u = g_gameState->gameData->unk8E4.battle->unitList;
    s32 dy = arg0->unk6 - arg0->unk8.h;
    s32 yy;
    s32 c;
    while (u != NULL) {
        if (u->flags & 1) {
            yy = y - dy;
            c = (u->mp < (s16)(u->max->maxMp / 6)) * 4;
            max[0] = 0x2F;
            text_formatNumber(u->mp, hp, 3, 1);
            y += 0xC;
            text_formatNumber((s16)u->max->maxMp, &max[1], -1, 1);
            ui_drawDigits(w->unk0 + 4, yy, arg0->unk18, hp, c);
            ui_drawDigits(w->unk0 + 0x19, yy, arg0->unk18, max, c);
        }
        u = u->next;
    }
}

void btlui_drawStatusList(Window *arg0) {
    BtlUnit *u;
    s32 i;
    s32 n;
    s32 x;
    Window *w = arg0;
    s32 d = arg0->unk6 - arg0->unk8.h;
    s32 y;
    u32 st;
    d -= 6;
    y = arg0->unk2 - d;
    for (u = g_gameState->gameData->unk8E4.battle->unitList; u != 0; u = u->next) {
        if (u->flags & 1) {
            n = 0;
            st = u->status | u->autoStatus;
            x = w->unk0 + 4;
            for (i = 0; i < 16; i++) {
                if (st & D_800F8F18[i].status) {
                    ui_drawSprite(x, y, arg0->unk18, D_800F8F18[i].icon, 0);
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

void btlui_drawStatusList2(Window *arg0) {
    BtlUnit *u;
    s32 i;
    s32 x;
    Window *w = arg0;
    s32 d = arg0->unk6 - arg0->unk8.h;
    s32 y;
    u32 st;
    d -= 6;
    y = arg0->unk2 - d;
    for (u = g_gameState->gameData->unk8E4.battle->unitList; u != 0; u = u->next) {
        if (u->flags & 1) {
            st = u->status | u->autoStatus;
            x = w->unk0 + 4;
            for (i = 0; i < 8; i++) {
                if (st & D_800F8F98[i].status) {
                    ui_drawSprite(x, y, arg0->unk18, D_800F8F98[i].icon, 0);
                }
                x += D_800F8F98[i].width;
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
    r.x += g_btluiDispOrigin[g_gameState->gameData->unk8 & 1][0];
    r.y += g_btluiDispOrigin[g_gameState->gameData->unk8 & 1][1];
    SetDrawMove((DR_MOVE *)g_primPtr, &r, 0x3C0, 0x1F8);
    addPrim(&g_ot[19], g_primPtr);
    g_primPtr += sizeof(DR_MOVE);
    setRECT(&r, 0x110, p->y - 4, 0x30, 4);
    r.x += g_btluiDispOrigin[g_gameState->gameData->unk8 & 1][0];
    r.y += g_btluiDispOrigin[g_gameState->gameData->unk8 & 1][1];
    SetDrawMove((DR_MOVE *)g_primPtr, &r, 0x3C0, 0x1FC);
    addPrim(&g_ot[19], g_primPtr);
    g_primPtr += sizeof(DR_MOVE);
}

void btlui_drawCachedHeader(void) {
    SPRT *a = &g_btluiPrims->unkB0[g_drawBufs.cur];
    SPRT *b = &g_btluiPrims->unkD8[g_drawBufs.cur];
    a->y0 = b->y0 = btlui_getPanel()->y - 4;
    addPrim(&g_ot[18], a);
    addPrim(&g_ot[18], b);
    addPrim(&g_ot[18], &g_btluiPrims->unk100[g_drawBufs.cur]);
}

void btlui_initCachedHeader(s32 arg0) {
    setlen(&g_btluiPrims->unkB0[arg0], 4);
    setcode(&g_btluiPrims->unkB0[arg0], 0x64);
    setRGB0(&g_btluiPrims->unkB0[arg0], 0x80, 0x80, 0x80);
    g_btluiPrims->unkB0[arg0].x0 = 0xD0;
    g_btluiPrims->unkB0[arg0].u0 = 0;
    g_btluiPrims->unkB0[arg0].v0 = 0xF8;
    g_btluiPrims->unkB0[arg0].clut = 0;
    g_btluiPrims->unkB0[arg0].w = 0x40;
    g_btluiPrims->unkB0[arg0].h = 4;
    setlen(&g_btluiPrims->unkD8[arg0], 4);
    setcode(&g_btluiPrims->unkD8[arg0], 0x64);
    setRGB0(&g_btluiPrims->unkD8[arg0], 0x80, 0x80, 0x80);
    g_btluiPrims->unkD8[arg0].x0 = 0x110;
    g_btluiPrims->unkD8[arg0].u0 = 0;
    g_btluiPrims->unkD8[arg0].v0 = 0xFC;
    g_btluiPrims->unkD8[arg0].clut = 0;
    g_btluiPrims->unkD8[arg0].w = 0x30;
    g_btluiPrims->unkD8[arg0].h = 4;
    setlen(&g_btluiPrims->unk100[arg0], 1);
    *g_btluiPrims->unk100[arg0].code = get_mode(1, 1, getTPage(2, 0, 0x3C0, 0x100));
}

void btlui_openListWindow(s32 arg0) {
    Unk80105078 *p = btlui_getPanel();
    Window *w;
    D_800F8EFC.y = p->y;
    D_800F8EFC.h = p->rowCount * 12 + 8;
    w = win_open(&D_800F8EFC, 0xA, 0, 1);
    g_btlUi->windows[1] = w;
    win_setDrawContent(w, btlui_drawListWindow);
    win_setDrawOverlay(g_btlUi->windows[1], btlui_drawListCursors);
    win_setOpenFrames(g_btlUi->windows[1], 7);
    btlui_showListHeader(arg0);
    D_80105058.kind = arg0;
    D_80105058.open = 1;
}

void btlui_closeListWindow(s32 arg0) {
    win_setCloseFrames(g_btlUi->windows[1], 7);
    win_close(g_btlUi->windows[1]);
    btlui_hideListHeader(arg0);
    D_80105058.open = 0;
}

/* resizes the battle list window to the current entry count */
void btlui_resizeListWindow(void) {
    Unk80105078 *p = btlui_getPanel();
    Window *w;
    RECT *r;
    s16 h;

    if (D_80105058.open != 0 && p->rowCount != 0) {
        w = g_btlUi->windows[1];
        r = &w->unk8;
        r->y = w->unk2 = p->y;
        h = p->rowCount * 12 + 8;
        w->unk6 = h;
        if (w->state == 2 || w->state == 3) {
            r->h = h;
        } else if (w->state == 1) {
            if (h < r->h) {
                r->h = h;
            }
        }
    }
}

u8 btlui_isListWindowOpen(void) {
    return D_80105058.open;
}

void btlui_drawListWindow(Window *arg0) {
    if (btlui_getPanel()->shown != 0) {
        D_800F8F04[D_80105058.kind](arg0);
    }
}

void btlui_initHeap(void) {
    g_btluiHeap.cursor = g_gameState->gameData->unk7E0;
    g_btluiHeap.base = g_btluiHeap.cursor;
    g_btluiHeap.free = 0x8000;
    g_btluiHeap.size = g_btluiHeap.free;
    g_btluiHeap.used = 0;
}

void func_800F2A58(void) {
}

void *btlui_alloc(s32 arg0) {
    s32 size = (arg0 + 3) >> 2 << 2;
    s32 p = g_btluiHeap.cursor;
    g_btluiHeap.cursor += size;
    g_btluiHeap.used += size;
    g_btluiHeap.free -= size;
    return (void *)p;
}

void btlui_updatePanel(void) {
    BtlUnit *u = g_gameState->gameData->unk8E4.battle->unitList;
    s32 n = 0;
    for (; u != NULL; u = u->next) {
        if (u->flags & 1) {
            g_btluiPanel->units[n] = u;
            g_btluiPanel->cantAct[n] = (btl_hasStatus(u, (BTL_STATUS_PETRIFY | BTL_STATUS_VENOM | BTL_STATUS_KO | BTL_STATUS_CONFUSE | BTL_STATUS_BERSERK | BTL_STATUS_STOP | BTL_STATUS_SLEEP | BTL_STATUS_FREEZE | BTL_STATUS_JUMP)) & 0xFF) != 0;
            g_btluiPanel->slot[n] = (u8)u->flags >> 5;
            if (btl_hasStatus(u, (BTL_STATUS_STOP | BTL_STATUS_SLOW)) & 0xFF) {
                g_btluiPanel->atbKind[n] = 2;
            } else if (btl_hasStatus(u, BTL_STATUS_HASTE) & 0xFF) {
                g_btluiPanel->atbKind[n] = 3;
            } else {
                g_btluiPanel->atbKind[n] = 0;
            }
            if ((u->flags >> 30) & 1) {
                g_btluiPanel->tranceKind[n] = (btl_hasStatus(u, BTL_STATUS_TRANCE) & 0xFF) ? 2 : 1;
            } else {
                g_btluiPanel->tranceKind[n] = 0;
            }
            n++;
        }
    }
    g_btluiPanel->menuUnit = g_btlUi->cmdUnit;
    g_btluiPanel->rowCount = n;
    g_btluiPanel->y = D_800F9028[n - 1];
    g_btluiPanel->unk1 = 0;
    g_btluiPanel->highlightRow = 0xFF;
}

void btlui_initHighlightPrim(s32 arg0) {
    setPolyG4(&g_btluiPrims->unk310[arg0]);
    setSemiTrans(&g_btluiPrims->unk310[arg0], 1);
    setRGB0(&g_btluiPrims->unk310[arg0], 0x40, 0x40, 0);
    setRGB1(&g_btluiPrims->unk310[arg0], 0x10, 0x10, 0);
    setRGB2(&g_btluiPrims->unk310[arg0], 0x40, 0x40, 0);
    setRGB3(&g_btluiPrims->unk310[arg0], 0x10, 0x10, 0);
    g_btluiPrims->unk310[arg0].x0 = 0x8E;
    g_btluiPrims->unk310[arg0].x1 = 0x137;
    g_btluiPrims->unk310[arg0].x3 = 0x137;
    g_btluiPrims->unk310[arg0].x2 = 0x8E;
    setlen(&g_btluiPrims->unk358[arg0], 1);
    *g_btluiPrims->unk358[arg0].code = get_mode(1, 1, getTPage(0, 1, 0, 0));
}

void btlui_initNamePrims(s32 arg0) {
    s32 i;
    SPRT *s;
    for (i = 0; i < 4; i++) {
        setlen(&g_btluiPrims->unk110[i][arg0], 4);
        setcode(&g_btluiPrims->unk110[i][arg0], 0x64);
        setRGB0(&g_btluiPrims->unk110[i][arg0], 0x80, 0x80, 0x80);
        g_btluiPrims->unk110[i][arg0].x0 = 0x92;
        g_btluiPrims->unk110[i][arg0].h = 0xC;
        g_btluiPrims->unk110[i][arg0].w = 0x54;
        if (i & 1) {
            g_btluiPrims->unk110[i][arg0].u0 = 0x54;
        } else {
            g_btluiPrims->unk110[i][arg0].u0 = 0;
        }
        g_btluiPrims->unk110[i][arg0].v0 = 0xF0;
    }
    setlen(&g_btluiPrims->unk2F0[arg0], 1);
    *g_btluiPrims->unk2F0[arg0].code = get_mode(1, 1, getTPage(0, 0, 0x380, 0x100));
}

void btlui_initCachedRows(s32 arg0) {
    s32 i;
    SPRT *s;
    for (i = 0; i < 4; i++) {
        setlen(&g_btluiPrims->unk1B0[i][arg0], 4);
        setcode(&g_btluiPrims->unk1B0[i][arg0], 0x64);
        setRGB0(&g_btluiPrims->unk1B0[i][arg0], 0x80, 0x80, 0x80);
        g_btluiPrims->unk1B0[i][arg0].x0 = 0xD0;
        g_btluiPrims->unk1B0[i][arg0].u0 = 0;
        g_btluiPrims->unk1B0[i][arg0].w = 0x1D;
        g_btluiPrims->unk1B0[i][arg0].h = 0xA;
        g_btluiPrims->unk1B0[i][arg0].clut = 0;
        setlen(&g_btluiPrims->unk250[i][arg0], 4);
        setcode(&g_btluiPrims->unk250[i][arg0], 0x64);
        setRGB0(&g_btluiPrims->unk250[i][arg0], 0x80, 0x80, 0x80);
        g_btluiPrims->unk250[i][arg0].x0 = 0xEF;
        g_btluiPrims->unk250[i][arg0].u0 = 0x20;
        g_btluiPrims->unk250[i][arg0].w = 0x16;
        g_btluiPrims->unk250[i][arg0].h = 0xA;
        g_btluiPrims->unk250[i][arg0].clut = 0;
    }
    setlen(&g_btluiPrims->unk300[arg0], 1);
    *g_btluiPrims->unk300[arg0].code = get_mode(1, 1, getTPage(2, 0, 0x3C0, 0x100));
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
                if (g_btluiPanel->labels[j].visible != 0 && D_800F9038[j] != 0) {
                    D_800F9038[j](g_btluiPanel->units[i], i);
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

    for (u = g_gameState->gameData->unk8E4.battle->unitList; u != NULL; u = u->next) {
        if (!(u->flags & 1)) {
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

    ui_drawRule(0x8E, g_btluiPanel->y, 8, 1, 0xA9, (g_gameState->config >> 18) & 1);
    r.x = 0x8E;
    r.y = g_btluiPanel->y - 3;
    r.w = g_gameState->dispEnv->disp.w - 0x8E;
    r.h = 7;
    win_setClip(NULL, 7, 4);
    e = g_btluiPanel->labels;
    for (i = 0; i < 5; e++, i++) {
        if (e->visible != 0) {
            ui_drawLabel((s16)e->x, g_btluiPanel->y + (s16)e->y, 7,
                          ((s32 *)&g_winLabelStrings)[e->text], 0);
        }
    }
    win_setClip(&r, 7, 4);
}

void btlui_drawPanelHighlight(void) {
    u8 c = g_btluiPanel->highlightRow;
    POLY_G4 *p;
    s32 y;
    s32 d = 4;
    if (c != 0xFF && D_80104E77 != 0) {
        y = g_btluiPanel->y + (c * 12 + d);
        p = &g_btluiPrims->unk310[g_drawBufs.cur];
        p->y0 = p->y1 = y;
        p->y2 = p->y3 = y + 13;
        addPrim(&g_ot[20], p);
        addPrim(&g_ot[20], &g_btluiPrims->unk358[g_drawBufs.cur]);
    }
}

void btlui_drawPanelName(s32 arg0, u32 arg1) {
    s32 k = g_btluiPanel->slot[arg1];
    SPRT *s = &g_btluiPrims->unk110[k][g_drawBufs.cur];
    s->y0 = g_btluiPanel->y + arg1 * 12 + 3;
    s->clut = D_800F9048[(u8)g_btluiPanel->rowColor[arg1]] + ((k >> 1) << 6);
    addPrim(&g_ot[16], s);
    if (arg1 >= g_btluiPanel->rowCount - 1) {
        addPrim(&g_ot[16], &g_btluiPrims->unk2F0[g_drawBufs.cur]);
    }
}

void btlui_drawPanelGauges(BtlUnit *arg0, s32 arg1) {
    s32 m = g_btluiPanel->tranceKind[arg1];
    u8 f = 0;
    u32 y = g_btluiPanel->y + arg1 * 12;
    if (arg0->cmdIssued == 0) {
        f = g_btluiPanel->cantAct[arg1] == 0;
    }
    if (m == 2) {
        ui_drawGauge(0x10C, y + 10, 14, arg0->trance, 0xFF, 1, f);
        ui_drawGauge(0x108, y + 6, 14, arg0->atb, arg0->max->maxAtb, g_btluiPanel->atbKind[arg1], f);
    } else if (m == 1) {
        ui_drawGauge(0x108, y + 6, 14, arg0->atb, arg0->max->maxAtb, g_btluiPanel->atbKind[arg1], f);
        ui_drawGauge(0x10C, y + 10, 14, arg0->trance, 0xFF, m, f);
    } else {
        ui_drawGauge(0x108, y + 8, 14, arg0->atb, arg0->max->maxAtb, g_btluiPanel->atbKind[arg1], f);
    }
}

void btlui_cacheRows(void) {
    RECT a = D_800A858C;
    RECT b = D_800A8594;
    s16 h;
    if (g_btluiPanel->shown != 0 && g_btluiPanel->rowCount != 0) {
        a.x += g_btluiDispOrigin[g_gameState->gameData->unk8 & 1][0];
        b.x += g_btluiDispOrigin[g_gameState->gameData->unk8 & 1][0];
        a.y += g_btluiDispOrigin[g_gameState->gameData->unk8 & 1][1] + g_btluiPanel->y;
        b.y += g_btluiDispOrigin[g_gameState->gameData->unk8 & 1][1] + g_btluiPanel->y;
        h = g_btluiPanel->rowCount * 12;
        b.h = h;
        a.h = h;
        SetDrawMove((DR_MOVE *)g_primPtr, &a, 0x3C0, 0x1B0);
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
            b = &g_btluiPrims->unk1B0[i][g_drawBufs.cur];
            a = &g_btluiPrims->unk250[i][g_drawBufs.cur];
            a->y0 = y;
            b->y0 = y;
            y += 12;
            a->v0 = v;
            b->v0 = v;
            v += 12;
            addPrim(&g_ot[18], b);
            addPrim(&g_ot[18], a);
        }
        addPrim(&g_ot[18], &g_btluiPrims->unk300[g_drawBufs.cur]);
    }
}

void btlui_initPanel(void) {
    s32 i;
    g_btluiPanel = btlui_alloc(0x8C);
    btlui_updatePanel();
    for (i = 0; i < 5; i++) {
        g_btluiPanel->labels[i] = D_800F8FD8[i];
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

void btlui_showListHeader(s32 arg0) {
    btlui_startLabelSlide(&g_btluiPanel->labels[1], &D_800F9058);
    btlui_startLabelSlide(&g_btluiPanel->labels[2], &D_800F9058);
    btlui_startLabelSlide(&g_btluiPanel->labels[3], &D_800F9058);
    btlui_startLabelSlide(&g_btluiPanel->labels[4], &D_800F9064);
    g_btluiPanel->labels[4].text = D_800F9070[arg0].unk0;
    btlui_setMenuState(BTLUI_MENU_CLOSING);
}

void btlui_hideListHeader(s32 arg0) {
    g_btluiPanel->returnState = arg0;
    btlui_startLabelSlide(&g_btluiPanel->labels[1], &D_800F9084);
    btlui_startLabelSlide(&g_btluiPanel->labels[2], &D_800F9084);
    btlui_startLabelSlide(&g_btluiPanel->labels[3], &D_800F9084);
    btlui_startLabelSlide(&g_btluiPanel->labels[4], &D_800F9090);
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

void btlui_showPanel(u8 arg0) {
    g_btluiPanel->shown = arg0;
}

void btlui_initPanelPrims(s32 arg0) {
    btlui_initHighlightPrim(arg0);
    btlui_initNamePrims(arg0);
    btlui_initCachedRows(arg0);
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
    u32 v = D_80104E80 & 0x3F;
    p->pulse = v;
    if (v >= 0x20) {
        p->pulse = 0x3F - v;
    }
}

void btlui_resetGauges(s32 arg0) {
    BtlUnit *u = g_btluiPanel->units[arg0];
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

void btlui_updatePanelRowColor(s32 arg0) {
    BtlUnit *u = g_btluiPanel->units[arg0];
    s32 hp = u->hp;
    if (hp == 0) {
        g_btluiPanel->rowColor[arg0] = 3;
        return;
    }
    g_btluiPanel->rowColor[arg0] = ((s32)(u16)(u->max->maxHp / 6) >= hp) << 2;
}

void btlui_alignPanelLabel(Unk800F412C *arg0) {
    if (arg0->align == 1) {
        arg0->x -= ui_measureLabel(((u8 **)&g_winLabelStrings)[arg0->text]);
    }
}

void btlui_startLabelSlide(Unk800F412C *arg0, Unk800F3F10 *arg1) {
    arg0->slide = arg1;
    arg0->visible = 1;
    arg0->frame = 0;
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

void btlui_drawPanelHp(BtlUnit *arg0, s32 arg1) {
    u8 buf[8];
    s32 dy = 6;
    text_formatNumber(arg0->menu->gauges[0].shown, buf, 4, 1);
    ui_drawDigits(0xD0, g_btluiPanel->y + (arg1 * 12 + dy), 0xE, buf, (u8)g_btluiPanel->rowColor[arg1]);
}

void btlui_drawPanelMp(BtlUnit *arg0, s32 arg1) {
    u8 buf[8];
    s32 mp;
    s32 c;
    s32 dy = 6;
    mp = arg0->menu->gauges[1].shown < 0 ? 0 : arg0->menu->gauges[1].shown;
    c = ((s16)(arg0->max->maxMp / 6) >= mp) << 2;
    text_formatNumber(mp, buf, 3, 1);
    ui_drawDigits(0xEF, g_btluiPanel->y + (arg1 * 12 + dy), 0xE, buf, c);
}

void btlui_findHighlightRow(void) {
    BtlUnit *u = g_gameState->gameData->unk8E4.battle->unitList;
    s32 i = 0;
    if (g_btluiPanel->rowCount != 0 && D_80104E70 == 0) {
        for (; u != 0; u = u->next) {
            if (u->flags & 1) {
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

    if (!(g->unk0 & 0x100) && !(g->unk8E4.battle->flags & 0x20) && g_btluiState.enabled != 0 &&
        (f = (g_gameState->config >> 5) & 1) == 1) {
        ClearOTag(&D_801050D4, 1);
        if (g_btluiState.vsyncTick & 0x20) {
            if (g_btluiState.port == 0) {
                s = &D_80068ED8;
            } else {
                s = &D_80068EE0;
            }
            setlen(&D_801050C0, 4);
            D_801050C0.rgbc = 0x64808080;
            D_801050C0.xy = 0x6012B;
            D_801050C0.uvclut = s->uvclut;
            D_801050C0.wh = s->w | (s->h << 16);
            addPrim(&D_801050D4, &D_801050C0);
        }
        if (D_80104E7D == 0) {
            s = &D_80068E90;
        } else {
            s = &D_80068ED0;
        }
        setlen(&D_801050A0, 4);
        D_801050A0.rgbc = 0x64808080;
        D_801050A0.xy = 0x60124;
        D_801050A0.uvclut = s->uvclut;
        D_801050A0.wh = s->w | (s->h << 16);
        addPrim(&D_801050D4, &D_801050A0);
        setlen(&D_80105080, 1);
        D_80105080.code[0] = get_mode(1, 1, s->tpage);
        addPrim(&D_801050D4, &D_80105080);
        setlen(&D_80105090, 2);
        D_80105090.code[0] = GPU_STP(1, 0);
        D_80105090.code[1] = 0;
        addPrim(&D_801050D4, &D_80105090);
        DrawOTag(&D_801050D4);
        DrawSync(0);
    }
}

void btlui_saveKeyRepeat(void) {
    s32 i;
    s32 j;
    u8 *row;
    pad_setStickAsDpad(1);
    i = 0;
    row = D_80104E94[0];
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
    D_80104E76 = btlui_anyOnSecondPort();
}

void btlui_restoreKeyRepeat(void) {
    s32 i, j;
    u8 *p;
    for (i = 0, p = D_80104E94[0]; i < 2; i++) {
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
    s32 f = (g_gameState->config >> 5) & 1;
    if (f != 1) {
        return 0;
    }
    for (i = 0; i < 4; i++) {
        Character *c = g_gameState->gameData->party[i];
        if (c == 0) {
            continue;
        }
        if (((s32)(g_gameState->config >> 20) >> chr_getDefaultSet(c)) & 1) {
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

void func_800F48DC(void) {
}

void btlui_initDrawBufs(void) {
    u32 *a = btlui_alloc(0xC0);
    s32 b = (s32)btlui_alloc(0x4000);
    DRAWENV *e;
    gfx_init(a, a + 0x18, b, b + 0x2000);
    func_8002C064(2);
    gfx_setBufferIndex(g_gameState->gameData->unk8);
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

INCLUDE_RODATA("asm/us/ovl_0b/nonmatchings/btlui", D_800A8584);

INCLUDE_RODATA("asm/us/ovl_0b/nonmatchings/btlui", D_800A858C);

INCLUDE_RODATA("asm/us/ovl_0b/nonmatchings/btlui", D_800A8594);
