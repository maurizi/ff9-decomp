#include "ovl_0b.h"

void btl_entry(void) {
    btl_initMode();
    do {
        btl_main((s16)g_gameState->gameData->battleId);
    } while (!(g_gameState->flags & (GS_RESET | GS_BTL_LEAVE)));
    btl_exitMode();
}

void btl_main(s32 battleId) {
    btl_load(battleId);
    SetDispMask(1);
    do {
        btl_beginFrame();
        if (!(g_gameState->gameData->ctx.battle->flags & BTL_FLAG_OVER)) {
            btl_checkDiscCover();
            if (btlui_isPauseRequested() != 0) {
                btl_updatePause();
            }
            if (!(g_gameState->gameData->flags & GD_PAUSED)) {
                if (btl_update() == 1) {
                    g_gameState->gameData->ctx.battle->flags |= BTL_FLAG_OVER;
                }
                if (!(g_gameState->gameData->ctx.battle->flags & BTL_FLAG_UI_STARTED) && (g_gameState->gameData->ctx.battle->loadFlags & BTL_LOAD_READY) == BTL_LOAD_READY) {
                    btlui_init();
                    g_gameState->gameData->ctx.battle->flags |= BTL_FLAG_UI_STARTED;
                }
                if (!(g_gameState->gameData->ctx.battle->flags & BTL_FLAG_EXIT_PRELOADED) && (g_gameState->gameData->battleFlags & GD_BTL_EXIT_PRELOAD)) {
                    btl_preloadExit();
                    g_gameState->gameData->ctx.battle->flags |= BTL_FLAG_EXIT_PRELOADED;
                }
                game_updateTimer();
                game_updateFade();
                preload_update();
            }
        } else {
            g_gameState->mode = GS_MODE_BATTLE_END;
            g_gameState->flags |= GS_BTL_LEAVE;
        }
        btl_endFrame();
    } while (!(g_gameState->flags & (GS_RESET | GS_BTL_LEAVE | GS_BTL_NEXT)));
    btl_cleanup();
}

void btl_updatePause(void) {
    GameData *gd = g_gameState->gameData;
    u16 btlFlags = gd->ctx.battle->flags;
    s32 i;
    s32 w;

    if (!(btlFlags & BTL_FLAG_OVER)) {
        if (!(gd->flags & GD_PAUSED) || (btlFlags & BTL_FLAG_DISC_COVER_OPEN)) {
            gd->flags |= GD_PAUSED;
            akao_pause(0);
            game_resumeClock(1);
            for (i = 1; i >= 0; i--) {
                g_padActuator[i].largeMotor = 0;
                g_padActuator[i].smallMotor = 0;
            }
            if (g_gameState->gameData->ctx.battle->flags & BTL_FLAG_DISC_COVER_OPEN) {
                g_padPortMask[0] |= 8;
            }
            ClearOTag(g_btlPauseOt, 1);
            setlen(&g_btlPauseStpOff, PRIM_LEN(DR_STP));
            g_btlPauseStpOff.code[0] = GPU_STP(0, 0);
            g_btlPauseStpOff.code[1] = 0;
            addPrim(g_btlPauseOt, &g_btlPauseStpOff);
            if (g_gameState->gameData->ctx.battle->flags & BTL_FLAG_DISC_COVER_OPEN) {
                text_buildSmall(0x66, 0x6D, g_btlPauseOt, "Disc Cover Open", TEXT_PAL_YELLOW, (u8 *)g_gameState->gameData->ctx.battle->work->prims[g_gameState->gameData->drawBuf]);
            } else {
                ui_buildSprite((0x140 - g_uiSprites[0xEB].w) / 2, (0xDA - g_uiSprites[0xEB].h) / 2, g_btlPauseOt, 0xEB, 0, &w, g_btlPauseSprite);
            }
            setlen(&g_btlPauseDimTile, PRIM_LEN(TILE));
            g_btlPauseDimTile.code = GPU_TILE | GPU_ABE;
            g_btlPauseDimTile.r0 = 0x60;
            g_btlPauseDimTile.g0 = 0x60;
            g_btlPauseDimTile.b0 = 0x60;
            g_btlPauseDimTile.w = 0x140;
            g_btlPauseDimTile.h = 0xDA;
            g_btlPauseDimTile.x0 = 0;
            g_btlPauseDimTile.y0 = 0;
            addPrim(g_btlPauseOt, &g_btlPauseDimTile);
            setDrawTPage(&g_btlPauseDrawMode, 0, 1, getTPage(0, 2, 0, 0));
            addPrim(g_btlPauseOt, &g_btlPauseDrawMode);
            g_btlPauseStpOn.code[0] = GPU_STP(1, 0);
            setlen(&g_btlPauseStpOn, PRIM_LEN(DR_STP));
            g_btlPauseStpOn.code[1] = 0;
            addPrim(g_btlPauseOt, &g_btlPauseStpOn);
            DrawOTag(g_btlPauseOt);
            g_gameState->gameData->flags |= GD_NO_FLIP;
            btlui_setPauseRequest(0);
            return;
        }
    } else if (btlFlags & BTL_FLAG_DISC_COVER_OPEN) {
        return;
    }
    game_resumeClock(0);
    akao_resume(0);
    g_padPortMask[0] &= ~8;
    btlui_resumeFromPause();
    g_gameState->gameData->flags &= ~GD_NO_FLIP;
    btlui_setPauseRequest(0);
    g_gameState->gameData->flags &= ~GD_PAUSED;
}

void btl_checkDiscCover(void) {
    volatile u8 shellOpen;
    volatile s32 cdState;
    u16 btlFlags;
    shellOpen = cd_isShellOpen();
    btlFlags = g_gameState->gameData->ctx.battle->flags;
    if (btlFlags & BTL_FLAG_UI_STARTED) {
        if (!(btlFlags & BTL_FLAG_DISC_COVER_OPEN)) {
            if (shellOpen != 0) {
                btlui_setPauseRequest(1);
                g_gameState->gameData->ctx.battle->flags |= BTL_FLAG_DISC_COVER_OPEN;
            }
        } else {
            cdState = cd_getState();
            if (cdState == 0) {
                btlui_setPauseRequest(1);
                g_gameState->gameData->ctx.battle->flags &= ~BTL_FLAG_DISC_COVER_OPEN;
            }
        }
    }
}

void btl_initMode(void) {
    GameData *gd = g_gameState->gameData;
    Battle *battle;
    btl_allocBattle();
    battle = gd->ctx.battle;
    btl_clearBattle();
    g_gameState->vsyncMode = 4;
    game_setScreenHeight(0xDA);
    game_freezeScreen();
    SetDispMask(1);
    gd->modelHook = btl_registerModel;
    gd->soundHook = btl_soundCmd;
    g_gameState->gameData->preloadRequest = btl_preloadRequest;
    preload_init();
    gd->npcModelSlots = 0;
    gd->npcCount = 0;
    gd->kanjiFont = 0;
    battle->dataSector = g_gameState->imgSector + g_gameState->dirs[13].firstSector;
    btl_loadCommonData();
    btl_loadKanjiFont();
    btl_loadResidentMusic();
    btl_loadSoundBanks();
    btl_loadShadowTexture();
    btl_initPartyNames();
    btl_initBattleState();
    battle->savedHeap = g_modeHeap;
}

void btl_exitMode(void) {
    s32 i;
    for (i = 1; i >= 0; i--) {
        g_padActuator[i].largeMotor = 0;
        g_padActuator[i].smallMotor = 0;
    }
    btl_unloadCommonData();
}

void btl_allocBattle(void) {
    BattleMem *mem;
    g_modeHeap = g_loadAreaEnd + 0x61800;
    mem = (BattleMem *)g_modeHeap;
    g_gameState->gameData->ctx.battle = &mem->battle;
    g_modeHeap = (s32)mem->heap;
    mem->battle.files = (BtlFiles *)mem->filesBuf;
    mem->battle.partyBufEnds = (struct Battle1538 **)mem->partyBufEndsBuf;
}

void btl_clearBattle(void) {
    Battle *battle = g_gameState->gameData->ctx.battle;
    s32 i;
    battle->flags = 0;
    battle->savedHeap = 0;
    for (i = 0; i < 16; i++) {
        *(battle->files->data + i) = 0;
    }
    for (i = 0; i < 4; i++) {
        battle->partyBufs[i] = 0;
        battle->partyBufEnds[i] = 0;
    }
    battle->unk1530 = 0;
    battle->dataSector = -1;
    battle->work = 0;
    for (i = 0; (u32)i < 2; i++) {
        battle->unk2[i] = 0;
    }
}

s32 btl_loadKanjiFont(void) {
    return 0;
}

s32 btl_loadResidentMusic(void) {
    s32 seqAddr = (s32)db_lookupRegistered(DB_GET_ADDR, 0x70005, 0);
    file_copyViaVram(g_gameState->gameData->musicSeq2, seqAddr, (s32)db_lookupRegistered(DB_GET_SIZE, 0x70005, 0), NULL);
    DrawSync(0);
    return 0;
}

s32 btl_loadSoundBanks(void) {
    s32 i;
    for (i = 1; i >= 0; i--) {
        s32 id = g_btlSoundBankIds[i] | DB_SOUND_BANK;
        s32 bankAddr = (s32)db_lookupRegistered(DB_GET_ADDR, id, 0);
        s32 bankSize = (s32)db_lookupRegistered(DB_GET_SIZE, id, 0);
        akao_beginBankUpload();
        akao_uploadBank(bankAddr, bankSize, 1);
    }
    return 0;
}

s32 btl_loadShadowTexture(void) {
    s32 tim = (s32)db_lookupRegistered(DB_GET_ADDR, DB_SHADOW_TIM, 0);
    file_setTimPos(tim, VRAM_SHADOW_CLUT_X, VRAM_SHADOW_CLUT_Y, 0x200, 0xE0);
    file_loadTim(tim);
    DrawSync(0);
    return 0;
}

s32 btl_initPartyNames(void) {
    btlui_uploadPartyNames();
    return 0;
}

s32 btl_loadCommonData(void) {
    s32 buf = g_modeHeap;
    Battle *battle = g_gameState->gameData->ctx.battle;
    FileEntry *file = file_find(DIR_COMMON, FILE_BTL_COMMON);
    s32 startSector = file->sector;
    void *ctx = g_gameState->gameData->cdRead;
    cdRead_init(ctx, startSector, (file->link - startSector) << 11, (void *)buf, 0, -1, NULL);
    while (cdRead_update(ctx) != 0) {
    }
    battle->files->data[0] = (void *)buf;
    db_register(buf, 0);
    file_copyViaVram((s32)battle->statusInfo, (s32)db_lookupRegistered(DB_GET_ADDR, 0x1F4, 0), 0x180, NULL);
    file_copyViaVram((s32)battle->abilities, (s32)db_lookupRegistered(DB_GET_ADDR, 0x1F5, 0), 0xC00, NULL);
    file_copyViaVram((s32)battle->statusSets, (s32)db_lookupRegistered(DB_GET_ADDR, 0x1F6, 0), 0x100, NULL);
    DrawSync(0);
    g_modeHeap = db_lookup(buf, DB_GET_ADDR, 0x1E, 0);
    return 0;
}

s32 btl_unloadCommonData(void) {
    BtlFiles *btlFiles = g_gameState->gameData->ctx.battle->files;
    db_unregister(btlFiles->data[0]);
    btlFiles->data[0] = NULL;
    return 0;
}

void btl_load(s32 battleId) {
    Unk800AA08C bgTask;
    Battle11C0 *work;
    s32 i;

    g_modeHeap = g_gameState->gameData->ctx.battle->savedHeap;
    btl_allocBuffers();
    work = g_gameState->gameData->ctx.battle->work;
    btl_initReturnInfo();
    g_gameState->flags &= ~GS_EXIT_BITS;
    game_setClearColor(0, 0, 0);
    g_gameState->gameData->battleId = battleId;
    btl_loadEncounterFile(work, battleId);
    btl_initEncounter(work, battleId);
    btl_loadBackgroundStep(work, -1, &bgTask);
    do {
    } while (btl_loadBackgroundStep(work, battleId, &bgTask) != 0);
    btl_setupBackground(work, battleId);
    g_gameState->gameData->npcCount = evt_countNpcActors(0);
    g_gameState->gameData->npcModelObjs = (struct Unk8D4 *)(work->weaponBufs + 4);
    g_gameState->gameData->npcModels = (struct ModelSlot *)&g_gameState->gameData->npcModelObjs[g_gameState->gameData->npcCount];
    g_modeHeap = (s32)&g_gameState->gameData->npcModels[g_gameState->gameData->npcCount];
    g_gameState->gameData->unk9 = 0;
    for (i = 0; i < 9; i++) {
        game_clearModelSlot(&g_gameState->gameData->chrModels[i]);
        g_gameState->gameData->chrModels[i].model = (struct BtlModel *)&g_gameState->gameData->chrModelObjs[i];
    }
    g_gameState->gameData->npcModelSlots = 0;
    for (i = 0; i < g_gameState->gameData->npcCount; i++) {
        game_clearModelSlot(&g_gameState->gameData->npcModels[i]);
        g_gameState->gameData->npcModels[i].model = (struct BtlModel *)&g_gameState->gameData->npcModelObjs[i];
    }
    work->shadows = (struct BtlShadow *)g_modeHeap;
    g_modeHeap = btl_allocShadows(g_modeHeap, g_gameState->gameData->npcCount + 9);
    work->freeHeap = g_modeHeap;
    btlui_reset();
    btl_startBattle();
}

void btl_cleanup(void) {
    Battle11C0 *work;
    s32 i;
    s32 sectorCount;
    s32 w;

    work = g_gameState->gameData->ctx.battle->work;
    DrawSync(0);
    btlui_exit();
    vib_clear();
    btl_unloadPartyModels();
    btl_unregisterFiles(&g_gameState->gameData->ctx.battle->files->enemyModels[0], 2);
    btl_unregisterFiles(&g_gameState->gameData->ctx.battle->files->data[3], 0);
    btl_unregisterFiles(&g_gameState->gameData->ctx.battle->files->data[2], 0);
    for (i = 0; i < 9; i++) {
        if (g_gameState->gameData->chrModels[i].flags & MODEL_SLOT_LOADED) {
            g_gameState->gameData->chrModels[i].flags &= ~MODEL_SLOT_LOADED;
        }
    }
    for (i = 0; i < g_gameState->gameData->npcCount; i++) {
        if (g_gameState->gameData->chrModels[i].flags & MODEL_SLOT_LOADED) {
            g_gameState->gameData->chrModels[i].flags &= ~MODEL_SLOT_LOADED;
        }
    }
    switch (work->returnMode) {
    case 1:
    case 3:
    case 5:
        for (i = 1; i >= 0; i--) {
            w = g_gameState->gameData->returnSector[i];
            g_gameState->gameData->sceneSector[i] = w;
            sectorCount = g_gameState->gameData->returnSectors[i];
            g_gameState->gameData->sceneSectors[i] = sectorCount;
        }
        if (work->returnMode == 3) {
            g_gameState->gameData->worldId = work->returnId;
        } else {
            g_gameState->gameData->fieldId = work->returnId;
        }
        break;
    case 8:
        break;
    default:
        SystemError(0x39, 0x12E);
        break;
    }
}

void btl_allocBuffers(void) {
    Battle11C0 *work;
    s32 i;
    g_gameState->gameData->ctx.battle->work = (Battle11C0 *)g_modeHeap;
    work = (Battle11C0 *)g_modeHeap;
    g_modeHeap = (s32)(work + 1);
    work->bundle = (u16 *)((s32)g_gameState->gameData->ctx.battle + 0x5800);
    work->bgData = (s32)work->bundle + 0x2800;
    work->heapStart = (u8 *)(work->bgData + 0x6000);
    g_modeHeap = (s32)work->heapStart + 0x35800;
    for (i = 0; i < 4; i++) {
        g_gameState->gameData->ctx.battle->partyBufs[i] = g_modeHeap;
        g_modeHeap += 0x13000;
        g_gameState->gameData->ctx.battle->partyBufEnds[i] = (Battle1538 *)(g_modeHeap - 0x28);
    }
    work->weaponBufs = (s32 *)(work + 1);
    for (i = 0; i < 4; i++) {
        work->weaponBufs[i] = g_modeHeap;
        g_modeHeap += 0x1000;
    }
    work->ot[1] = g_modeHeap + 0x400;
    work->ot[0] = g_modeHeap;
    work->prims[0] = g_modeHeap + 0x800;
    work->prims[1] = work->prims[0] + 0x20000;
    g_modeHeap = work->prims[1] + 0x20000;
    work->loadBuf = (s32 *)(g_loadAreaEnd + 0x136700);
}

void btl_initReturnInfo(void) {
    Battle11C0 *work = g_gameState->gameData->ctx.battle->work;
    s32 i;
    work->unk0 = 0;
    work->freeHeap = 0;
    work->returnMode = g_gameState->prevMode;
    if (g_gameState->prevMode == GS_MODE_FIELD) {
        work->returnId = g_gameState->gameData->fieldId;
    } else if (g_gameState->prevMode == GS_MODE_WORLD) {
        work->returnId = g_gameState->gameData->worldId;
    } else {
        work->returnId = -1;
    }
    work->encounter = NULL;
    work->script = NULL;
    work->texts = NULL;
    work->bgHeader = 0;
    work->bgTexAnims = NULL;
    work->shadows = NULL;
    work->heap = work->heapStart;
    for (i = 0; i < sizeof(work->unk7); i++) {
        work->unk7[i] = 0;
    }
    for (i = 0; i < 4; i++) {
        g_gameState->gameData->ctx.battle->partyBufEnds[i]->base = g_gameState->gameData->ctx.battle->partyBufEnds[i]->cursor = g_gameState->gameData->ctx.battle->partyBufs[i];
    }
}

s32 btl_initEncounter(Battle11C0 *work, s32 battleId) {
    s32 id = (battleId & 0xFFFF) | DB_BATTLE;
    work->bundle = (u16 *)db_lookupRegistered(DB_GET_ADDR, id, 0);
    work->encounter = (Battle11C020 *)db_lookupRegistered(DB_GET_ADDR, (s32)db_lookupRegistered(DB_GET_KEY_AT_IN, id, 0x100000), 0);
    work->script = (u8 *)db_lookupRegistered(DB_GET_ADDR, (s32)db_lookupRegistered(DB_GET_KEY_AT_IN, id, DB_FIELD), 0);
    work->texts = (u8 *)db_lookupRegistered(DB_GET_ADDR, (s32)db_lookupRegistered(DB_GET_KEY_AT_IN, id, DB_AREA), 0);
    btl_initScene();
    evt_init(work->script);
    msg_init();
    return 0;
}

s32 btl_setupBackground(Battle11C0 *work, s32 battleId) {
    s32 id = btl_findBackgroundId();
    s32 i;
    RECT rect;
    work->bgData = (s32)db_lookupRegistered(DB_GET_ADDR, id, 0);
    work->bgHeader = (s32)db_lookupRegistered(DB_GET_ADDR, (s32)db_lookupRegistered(DB_GET_KEY_AT_IN, id, DB_BTL_BG_HEADER), 0);
    if (db_lookupRegistered(DB_GET_COUNT_IN, id, DB_TEX_ANIM) != 0) {
        work->bgTexAnims = db_lookupRegistered(DB_GET_ADDR, (s32)db_lookupRegistered(DB_GET_KEY_AT_IN, id, DB_TEX_ANIM), 0);
    }
    for (i = 0; i < (s32)db_lookupRegistered(DB_GET_COUNT_IN, id, DB_TIM); i++) {
        file_loadTim((s32)db_lookupRegistered(DB_GET_ADDR, (s32)db_lookupRegistered(DB_GET_KEY_AT_IN, id, (i & 0xFFFF) | DB_TIM), 0));
    }
    if (work->bgTexAnims != NULL) {
        rect.y = 0;
        rect.x = 0;
        rect.h = 0;
        rect.w = 0;
        btl_relocTexAnims((TexAnimSet *)work->bgTexAnims, NULL);
    }
    DrawSync(0);
    g_gameState->gameData->ctx.battle->flags |= BTL_FLAG_BG_LOADED;
    return 0;
}

s32 btl_reloadPartyStep(s32 flags) {
    Battle *battle = g_gameState->gameData->ctx.battle;
    s16 *state = (s16 *)&battle->work->loadBuf[0x9FD8 / 4];
    Unk800AA958 *ld = (Unk800AA958 *)state - 1;
    s32 i;
    Character *ch;
    s32 cid;
    ModelSlot *slot;
    BtlUnit *unit;
    s32 id;
    s32 texKey;
    s32 weaponModelId;
    s32 *pa;
    s32 *pb;
    s32 *pc;
    s32 animKey;
    s32 texAnims;
    BtlModel *wpnModel;
    u8 parentBone;

    if (flags & BTL_STEPF_BEGIN) {
        battle->flags &= ~BTL_FLAG_PARTY_LOADED;
        *state = 1;
    }
    switch (*state) {
    case 1:
        if (flags & BTL_STEPF_SOUNDS) {
            btl_loadSoundsStep(g_gameState->gameData->battleId, BTL_STEPF_BEGIN, (Unk800AA24C *)ld);
            *state = 2;
            break;
        }
        *state = 3;
        break;
    case 2:
        if (btl_loadSoundsStep(g_gameState->gameData->battleId, flags, (Unk800AA24C *)ld) == 0) {
            *state = 3;
        }
        break;
    case 3:
        btl_loadWeaponModelsStep(BTL_STEPF_BEGIN, ld);
        *state = 4;
        break;
    case 4:
        if (btl_loadWeaponModelsStep(flags, ld) == 0) {
            *state = 5;
        }
        break;
    case 5:
        btl_loadPartyModelsStep(BTL_STEPF_BEGIN, ld);
        *state = 6;
        break;
    case 6:
        if (btl_loadPartyModelsStep(flags, ld) == 0) {
            if (flags & BTL_STEPF_RELOAD_UNITS) {
                *state = 7;
            } else {
                goto done;
            }
        }
        break;
    case 7:
        for (i = 0; i < PARTY_SIZE; i++) {
            ch = g_gameState->gameData->party[i];
            if (ch == NULL) {
                continue;
            }
            cid = ch->info.b.id;
            slot = &g_gameState->gameData->chrModels[cid];
            unit = btl_getUnitBySlot(i);
            if (unit == NULL) {
                continue;
            }
            /* model/texture/motion reload: same steps as btl_swapPartyModelStep case 6, which does it for one slot */
            id = btl_getPartyModelId(i, flags & ~BTL_STEPF_FLIP_TRANCE);
            slot->model->header = db_lookupRegistered(DB_GET_ADDR, id, 0);
            texKey = (id & 0xFFFFFF) | (cid << 24);
            pa = (s32 *)db_getModelTexPages(texKey);
            pb = (s32 *)db_getModelUvOffsets(texKey);
            pc = (s32 *)db_getModelTexPos(texKey);
            btl_applyModelTextures(slot->model, (s16 *)pa, (s16 *)pb);
            animKey = (s32)db_lookupRegistered(DB_GET_KEY_AT_IN, id, DB_TEX_ANIM);
            if (animKey != -1) {
                texAnims = (s32)db_lookupRegistered(DB_GET_ADDR, animKey, 0);
            } else {
                texAnims = 0;
            }
            slot->model->texAnims = (BtlUnit9C50 *)texAnims;
            if (texAnims != 0) {
                btl_relocTexAnims((TexAnimSet *)slot->model->texAnims, (u16 (*)[4])pc);
            }
            btl_loadPartyMotions(unit, g_gameState->gameData->party[i]->info.b.kind, unit - g_gameState->gameData->ctx.battle->units);
            if (unit->status & BTL_STATUS_KO) {
                if (unit != g_gameState->gameData->ctx.battle->curCmd->caster) {
                    unit->model->motion = (BtlMotion *)unit->motions[BTL_MOTION_KO];
                } else {
                    goto use9;
                }
            } else if (unit == g_gameState->gameData->ctx.battle->curCmd->caster || ((unit->flags >> BTL_UNIT_CMD_WAITING_BIT) & 1)) {
            use9:
                unit->model->motion = (BtlMotion *)unit->motions[BTL_MOTION_READY];
            } else {
                unit->model->motion = (BtlMotion *)unit->motions[(unit->flags >> BTL_UNIT_WEAK_STANCE_BIT) & 1];
            }
            slot->actor->frame = 0;
            wpnModel = unit->weaponModel;
            parentBone = wpnModel->parentBone;
            weaponModelId = ((WeaponEntry *)unit->weapon)->modelId | DB_MODEL;
            wpnModel->header = (BtlModelHeader *)btl_getWeaponModelHeader(i);
            btl_getWeaponTexInfo(weaponModelId, i, &pa, &pb, &pc);
            btl_applyModelTextures(wpnModel, (s16 *)pa, (s16 *)pb);
            wpnModel->motion = 0;
            wpnModel->parent = slot->model;
            wpnModel->parentBone = parentBone;
        }
    done:
        g_gameState->gameData->ctx.battle->flags |= BTL_FLAG_PARTY_LOADED;
        *state = 0;
        break;
    }
    return *state;
}

s32 btl_loadEncounterFile(Battle11C0 *work, s32 battleId) {
    s32 bundle = (s32)work->bundle;
    GameData *gd = g_gameState->gameData;
    u8 *cd = gd->cdRead;
    Battle *battle = gd->ctx.battle;
    cdRead_init(cd, gd->sceneSector[0], gd->sceneSectors[0] << 11, work->bundle, 0, 0x2800, 0);
    while (cdRead_update(cd) != 0) {
    }
    battle->files->data[2] = (void *)bundle;
    db_register(bundle, 0);
    gd->depTable = (FileTable *)db_lookup(bundle, DB_GET_AT, DB_DEP_TABLE, 0);
    return 0;
}

s32 btl_loadBackgroundStep(Battle11C0 *work, s32 battleId, Unk800AA08C *task) {
    GameData *gd = g_gameState->gameData;
    Battle *battle = gd->ctx.battle;
    u8 *cd = gd->cdRead;
    Unk800AA08C *st;
    s32 bgData;
    GameData *gd2;
    st = task;
    if (battleId == -1) {
        battle->flags &= ~BTL_FLAG_BG_LOADED;
        st->state = 2;
    }
    if (st->state == 1) {
        if (cdRead_update(cd) == 0) {
            st->state = st->nextState;
        }
    } else if (st->state == 2) {
        gd2 = g_gameState->gameData;
        st->sceneBuf = work->bgData;
        cdRead_init(cd, gd2->sceneSector[1], gd2->sceneSectors[1] << 11, (void *)st->sceneBuf, 0, -1, 0);
        st->nextState = 4;
        st->state = 1;
    } else if (st->state == 4) {
        bgData = work->bgData;
        battle->files->data[3] = (void *)bgData;
        db_register(bgData, 0);
        battle->flags |= BTL_FLAG_BG_LOADED;
        st->state = 0;
    }
    return st->state;
}

s32 btl_loadSoundsStep(s16 battleId, s32 flags, Unk800AA24C *task) {
    Unk800AA24C *st;
    GameData *gd;
    Battle *battle;
    CdReadCtx *cd;
    s16 *ids;
    s32 sector;
    s16 *found;
    s32 sz;
    s32 bank;

    st = task;
    gd = g_gameState->gameData;
    battle = gd->ctx.battle;
    cd = gd->cdRead;
    if (flags & BTL_STEPF_BEGIN) {
        battle->flags &= ~BTL_FLAG_ENEMIES_LOADED;
        st->state = 2;
    }
loop:
    switch (st->state) {
    case 1:
        if (cdRead_update(cd) != 0) {
            break;
        }
        file_advanceLoadPtr(&task->buf, cd->chunk);
        st->state = task->nextState;
        goto loop;
    case 2:
        task->buf = battle->partyBufs[0];
        task->fileIds.w[0] = -1;
        task->fileIds.w[1] = -1;
        task->fileIds.w[2] = -1;
        task->fileIds.w[3] = -1;
        task->fileIds.h[0] = 0x201;
        if (flags & BTL_STEPF_DEP_SOUNDS) {
            btl_loadSoundsStep_collect(&task->fileIds.h[4], 9, 2);
        }
        st->state = 5;
        goto loop;
    case 5:
        ids = task->fileIds.h;
        sector = btl_findNearestFile(DIR_SOUND_BANKS, (u16 *)ids, (u16 *)&task->fileIds.h[6], (u16 **)&found, &sz);
        if (sector != -1) {
            cdRead_init(cd, sector, sz << 11, (void *)task->buf, 0, -1, 0);
            if (*found == 0x201) {
                task->bank = -1;
            } else {
                task->bank = found - ids;
            }
            *found = -1;
            task->nextState = 6;
            st->state = 1;
            break;
        }
        st->state = 7;
        goto loop;
    case 6:
        if (akao_getSpuTransferState() != 0) {
            break;
        }
        bank = db_lookup((s32)cd->dest, DB_GET_AT, DB_SOUND_BANK, 0);
        if (task->bank == -1) {
            akao_loadBankRetry(bank, 0);
        } else {
            akao_loadBankInSlot(bank, task->bank & 3, 0);
        }
        st->state = 5;
        goto loop;
    case 7:
        if (akao_getSpuTransferState() != 0) {
            break;
        }
        st->state = 0;
        break;
    }
    return st->state;
}

s32 btl_loadEnemyModelsStep(s32 battleId, s32 flags, Unk800AA504 *task) {
    Unk800AA504 *st;
    Battle *battle;
    Unk800AA958Req *req;
    FileTable *table;
    FileEntryX *first;
    FileEntryX *entry;
    s16 *dup;
    u32 w;
    u32 best;
    s16 *bp;
    FileEntry *bf;
    s16 *cur;
    FileEntry *file;
    s32 pack;
    s32 dst;
    Unk800AA504Tbl *tbl;
    Unk800AA504Ent *te;
    ModelTexWord *pages;
    ModelTexPos *texPos;
    Unk800AA504Ent *tf;
    ModelTexWord *bb;
    s32 texCount;
    s32 clutX;
    s32 tim;
    u16 id;
    s16 *lo;
    s16 *ids;
    s32 cnt;

    st = task;
    battle = g_gameState->gameData->ctx.battle;
    req = (Unk800AA958Req *)&battle->work->loadBuf[0x9FA0 / 4];
    if (flags & BTL_STEPF_BEGIN) {
        battle->flags &= ~BTL_FLAG_ENEMIES_LOADED;
        st->state = 2;
    }
loop:
    switch (st->state) {
    case 1:
        if (cd_getState() != 0) {
            break;
        }
        st->state = task->nextState;
        goto loop;
    case 2:
        table = g_gameState->gameData->depTable;
        ids = task->fileIds.h;
        cnt = 0;
        task->loaded = 0;
        task->dst = battle->partyBufs[0];
        task->fileIds.w[0] = -1;
        task->fileIds.w[1] = -1;
        /* same walk as btl_loadSoundsStep_collect, but testing the type only and appending through ids[cnt] */
        first = (FileEntryX *)table->entries;
        entry = &first[table->count] - 1;
        do {
            w = entry->id.w;
            if (entry->id.b.type == 2) {
                w &= 0xFFFF;
                dup = &ids[cnt] - 1;
                do {
                    if (*dup == w) {
                        goto next;
                    }
                    dup--;
                } while (dup >= ids);
                ids[cnt++] = w;
            }
        next:
            entry--;
        } while (entry >= first);
        st->state = 4;
        goto loop;
    case 4:
        best = -1;
        bp = NULL;
        bf = NULL;
        lo = task->fileIds.h;
        cur = &task->fileIds.h[2];
        do {
            id = *cur;
            if (id != 0xFFFF) {
                file = dep_find(g_gameState->gameData->depTable, id | DB_MODEL);
                if ((u32)file->sector < best) {
                    best = file->sector;
                    bf = file;
                    bp = cur;
                }
            }
            cur--;
        } while (cur >= lo);
        if (bp != NULL) {
            task->fileId = *bp;
            *bp = -1;
            req->size0 = bf->link << 11;
            req->dst0 = (s32)battle->work->heap;
            req->size1 = (bf->sectors - bf->link) << 11;
            req->dst1 = task->dst;
            cd_readChunks(g_gameState->imgSector + best, req, 2, 0);
            task->nextState = 6;
            st->state = 1;
        } else {
            st->state = 7;
        }
        break;
    case 6:
        pack = db_lookup((s32)battle->work->heap, DB_GET_AT, DB_PACK, 0);
        dst = task->dst;
        tbl = (Unk800AA504Tbl *)db_lookup((s32)battle->files->data[2], DB_GET_AT, DB_MODEL_INFO, 0);
        tf = tbl->entries;
        te = &tf[tbl->count] - 1;
        do {
            if (te->modelId == task->fileId) {
                texCount = te->tex.b[3];
                pages = (ModelTexWord *)((u8 *)te + (te->tex.w & 0xFFFFFF));
                bb = pages + texCount;
                texPos = (ModelTexPos *)(bb + texCount);
                while (texCount > 0) {
                    texCount--;
                    tim = db_lookup(dst, DB_GET_AT, (texCount & 0xFFFF) | DB_TIM, 0);
                    clutX = clut_getX(pages[texCount].clut);
                    file_setTimPos(tim, clutX, clut_getY(pages[texCount].clut), texPos[texCount].x, texPos[texCount].y);
                    file_loadTim(tim);
                }
                break;
            }
            te--;
        } while (te >= tf);
        g_modeHeap = pack + db_getSize(pack);
        battle->work->heap = (u8 *)g_modeHeap;
        task->dst += db_getSize(dst);
        *(battle->files->enemyModels + task->loaded) = (void *)pack;
        db_register(pack, 0);
        task->loaded++;
        st->state = 4;
        goto loop;
    case 7:
        battle->flags |= BTL_FLAG_ENEMIES_LOADED;
        st->state = 0;
        break;
    }
    return st->state;
}

s32 btl_loadWeaponModelsStep(s32 flags, Unk800AA958 *task) {
    Unk800AA958 *st;
    Battle *battle;
    Unk800AA958Req *req;
    u16 *found;
    u16 *dstId;
    s32 sector;
    s32 sz;
    s32 i;
    s16 j;
    void **pp;
    s32 pack;
    Character **chr;
    Character **first;
    GameData *gd;

    st = task;
    battle = g_gameState->gameData->ctx.battle;
    req = (Unk800AA958Req *)&battle->work->loadBuf[0x9FA0 / 4];
    if (flags & BTL_STEPF_BEGIN) {
        battle->flags &= ~BTL_FLAG_PARTY_LOADED;
        st->state = 2;
    }
loop:
    switch (st->state) {
        case 1:
            if (cd_getState() != 0) {
                goto end;
            }
            st->state = task->nextState;
            goto loop;
        case 2:
            task->fileIds.w[0] = -1;
            task->fileIds.w[1] = -1;
            gd = g_gameState->gameData;
            first = gd->party;
            chr = &gd->party[3];
            dstId = &task->fileIds.h[3];
            do {
                if (*chr != NULL) {
                    *dstId = g_weaponTable[(*chr)->equip[0]].modelId;
                }
                chr--;
                dstId--;
            } while (chr >= first);
            st->state = 5;
            goto loop;
        case 5:
            sector = btl_findNearestFile(DIR_WEAPON_MODELS, task->fileIds.h, &task->fileIds.h[3], &found, &sz);
            if (sector != -1) {
                *(s16 *)found = -1;
                i = found - task->fileIds.h;
                task->slot = i;
                task->buf = battle->work->weaponBufs[i];
                req->size0 = 0x1000;
                req->dst0 = task->buf;
                req->size1 = (sz << 11) - 0x1000;
                req->dst1 = battle->partyBufs[i];
                cd_readChunks(g_gameState->imgSector + sector, req, 2, 0);
                task->nextState = 7;
                st->state = 1;
            } else {
                st->state = 8;
            }
            goto end;
        case 7:
            j = task->slot;
            pack = btl_uploadModelTextures(task, req, j, 0);
            pp = &battle->files->weaponFiles[j];
            *pp = (void *)pack;
            db_register(pack, 0);
            st->state = 5;
            goto loop;
        case 8:
            st->state = 0;
        default:
            goto end;
    }
end:
    return st->state;
}

s32 btl_loadPartyModelsStep(s32 flags, Unk800AA958 *task) {
    s32 i;
    Unk800AA958 *st;
    Battle *battle;
    Unk800AA958Req *req;
    s16 *h;
    s16 *match;
    s16 *cur;
    s16 *p2;
    s32 k;
    s32 sz;
    s32 sz2;
    s32 sector;
    s16 j;
    s32 pack;
    void **pp;
    FileEntry16 *file;

    st = task;
    battle = g_gameState->gameData->ctx.battle;
    req = (Unk800AA958Req *)&battle->work->loadBuf[0x9FA0 / 4];
    if (flags & BTL_STEPF_BEGIN) {
        battle->flags &= ~BTL_FLAG_PARTY_LOADED;
        st->state = 1;
    }
loop:
    switch (st->state) {
    case 2:
        if (cd_getState() != 0) {
            goto end;
        }
        st->state = task->nextState;
        goto loop;
    case 1:
        h = (s16 *)task->fileIds.h;
        cur = (s16 *)&task->fileIds.h[3];
        task->fileIds.w[0] = -1;
        task->fileIds.w[1] = -1;
        task->fileIds.w[2] = 0;
        task->fileIds.w[3] = 0;
        do {
            *cur = btl_getPartyModelId(cur - h, flags & ~BTL_STEPF_FLIP_TRANCE);
            cur--;
        } while (cur >= h);
        st->state = 5;
        goto loop;
    found:
        req->dst1 = battle->partyBufs[k];
        goto done;
    case 5:
        h = (s16 *)task->fileIds.h;
        cur = (s16 *)&task->fileIds.h[3];
        sector = btl_findNearestFile(DIR_CHAR_MODELS, (u16 *)h, (u16 *)cur, (u16 **)&match, &sz);
        if (sector != -1) {
            i = -1;
            file = (FileEntry16 *)file_find(DIR_CHAR_MODELS, *match);
            match[0] = i;
            match[4] = i;
            task->slot = match - h;
            task->buf = battle->partyBufs[task->slot];
            req->dst1 = 0;
            if (btl_findNearestFile(DIR_CHAR_MODELS, (u16 *)h, (u16 *)cur, (u16 **)&p2, &sz2) != i) {
                cur = (s16 *)&task->fileIds.h[7];
                i = p2 - h;
                h = (s16 *)&task->fileIds.h[4];
                do {
                    if (*cur != -1) {
                        k = cur - h;
                        if (k != i) {
                            goto found;
                        }
                    }
                    cur--;
                } while (cur >= h);
            }
        done:
            if (req->dst1 == 0) {
                req->dst1 = (s32)battle->work->loadBuf;
            }
            req->size0 = file->idHi << 11;
            req->dst0 = task->buf;
            req->size1 = (sz - file->idHi) << 11;
            cd_readChunks(g_gameState->imgSector + sector, req, 2, 0);
            task->nextState = 7;
            st->state = 2;
        } else {
            st->state = 8;
        }
        goto end;
    case 7:
        j = task->slot;
        pack = btl_uploadModelTextures(task, req, j, 1);
        pp = &g_gameState->gameData->ctx.battle->files->partyModels[j];
        *pp = (void *)pack;
        db_register(pack, 0);
        st->state = 5;
        goto loop;
    case 8:
        st->state = 0;
    default:
        goto end;
    }
end:
    return st->state;
}

u32 btl_findNearestFile(u32 dir, u16 *first, u16 *last, u16 **outFound, s32 *outSectors) {
    u32 best = -1;
    s32 bestn = 0;
    u16 id;
    u32 start;
    s32 sectors;
    s32 dbKind;
    FileEntry *file;
    do {
        id = *last;
        start = -1;
        if (id != 0xFFFF) {
            sectors = 0;
            file = file_find(dir, id);
            if (file != NULL) {
                start = file->sector;
                sectors = file->link - start;
            } else {
                dbKind = 0;
                switch (dir) {
                case DIR_OVERLAYS:
                    dbKind = DB_OVERLAY >> 16;
                    break;
                case DIR_COMMON:
                    dbKind = 0;
                    break;
                case DIR_WORLDS:
                case DIR_FIELDS:
                    dbKind = DB_FIELD >> 16;
                    break;
                case DIR_ENCOUNTERS:
                    dbKind = DB_BATTLE >> 16;
                    break;
                case DIR_ENEMY_MODELS:
                case DIR_WEAPON_MODELS:
                case DIR_CHAR_MODELS:
                    dbKind = DB_MODEL >> 16;
                    break;
                case DIR_AREAS:
                    dbKind = DB_AREA >> 16;
                    break;
                case DIR_BTL_BGS:
                    dbKind = DB_BTL_BG >> 16;
                    break;
                case DIR_SOUND_BANKS:
                    dbKind = DB_SOUND_BANK >> 16;
                    break;
                case 9:
                    dbKind = DB_MUSIC >> 16;
                    break;
                }
                file = dep_find(g_gameState->gameData->depTable, (dbKind << 16) | id);
                if (file != NULL) {
                    start = file->sector;
                    sectors = file->sectors;
                }
            }
            if (start < best) {
                best = start;
                bestn = sectors;
                *outFound = last;
            }
        }
        last--;
    } while (last >= first);
    *outSectors = bestn;
    return best;
}

s32 btl_uploadModelTextures(Unk800AA958 *task, Unk800AA958Req *req, s32 slot, s32 isParty) {
    s32 pack;
    s32 dst;
    Unk800AA504Tbl *tbl;
    Unk800AA504Ent *te;
    s32 texCount;
    ModelTexWord *pages;
    ModelTexWord *uvs;
    ModelTexPos *texPos;
    s32 x;
    s32 y;
    s32 clutY;
    s32 texY;
    s32 tim;

    pack = db_lookup(task->buf, DB_GET_AT, DB_PACK, 0);
    dst = req->dst1;
    tbl = (Unk800AA504Tbl *)db_lookup(pack, DB_GET_AT, DB_MODEL_INFO, 0);
    te = tbl->entries;
    texCount = te->tex.b[3];
    pages = (ModelTexWord *)((u8 *)te + (te->tex.w & 0xFFFFFF));
    uvs = pages + texCount;
    texPos = (ModelTexPos *)(uvs + texCount);
    if (texCount > 0) {
        do {
            texCount--;
            x = slot * 64 + 0xC0;
            y = slot * 3;
            tim = db_lookup(dst, DB_GET_AT, (texCount & 0xFFFF) | DB_TIM, 0);
            if (isParty) {
                clutY = y + 0xE5 + texPos[texCount].y;
                texY = (texPos[texCount].y << 7) + 0x80;
            } else {
                clutY = y + 0xE8;
                texY = 0x1D0;
            }
            file_setTimPos(tim, VRAM_MODEL_CLUT_X, clutY, x, texY);
            file_loadTim(tim);
            pages[texCount].tpage = getTPage(1, 0, x, texY);
            pages[texCount].clut = getClut(VRAM_MODEL_CLUT_X, clutY);
            uvs[texCount].tpage = 0;
            uvs[texCount].clut = texY % 256 - texY % 128;
            texPos[texCount].w = x;
            texPos[texCount].x = x;
            texPos[texCount].h = texY;
            texPos[texCount].y = texY;
        } while (texCount > 0);
    }
    return pack;
}

s32 btl_unregisterFiles(void **files, s32 last) {
    void **cur = &files[last];
    do {
        if (*cur != NULL) {
            db_unregister(*cur);
        }
        *cur = NULL;
        cur--;
    } while (cur >= files);
    return 0;
}

s32 btl_findBackgroundId(void) {
    FileTable *table = g_gameState->gameData->depTable;
    FileEntry *base = table->entries;
    FileEntry *entry = &base[table->count] - 1;
    s32 fileId;
    do {
        fileId = entry->id;
        if ((u8)(entry->id >> 16) == 0xC) {
            return fileId;
        }
        entry--;
    } while (entry >= base);
    return -1;
}

s32 btl_getDepSectors(s32 fileId, s32 *outSector, s32 *outSectors) {
    FileEntry *entry = dep_find(g_gameState->gameData->depTable, fileId);
    *outSector = entry->sector;
    *outSectors = entry->sectors;
    return fileId;
}

void btl_getLinkedDepSectors(s32 fileId, s32 *outSector, s32 *outSectors) {
    btl_getDepSectors(dep_find(g_gameState->gameData->depTable, fileId)->link, outSector, outSectors);
}

s32 btl_swapPartyModel(s32 slot, s32 begin) {
    s32 flags = BTL_STEPF_FLIP_TRANCE | BTL_STEPF_RELOAD_UNITS | BTL_STEPF_UNTESTED1 | BTL_STEPF_UNTESTED0;
    if (begin != 0) {
        flags = BTL_STEPF_BEGIN | BTL_STEPF_FLIP_TRANCE | BTL_STEPF_RELOAD_UNITS | BTL_STEPF_UNTESTED1 | BTL_STEPF_UNTESTED0;
    }
    return btl_swapPartyModelStep(slot, flags);
}

s32 btl_reloadParty(s32 begin) {
    s32 flags = BTL_STEPF_SOUNDS | BTL_STEPF_RELOAD_UNITS | BTL_STEPF_UNTESTED1 | BTL_STEPF_UNTESTED0;
    if (begin != 0) {
        flags = BTL_STEPF_BEGIN | BTL_STEPF_SOUNDS | BTL_STEPF_RELOAD_UNITS | BTL_STEPF_UNTESTED1 | BTL_STEPF_UNTESTED0;
    }
    return btl_reloadPartyStep(flags);
}

void btl_unloadPartyModels(void) {
    Battle *battle = g_gameState->gameData->ctx.battle;
    BtlFiles *btlFiles = battle->files;
    btl_unregisterFiles(btlFiles->weaponFiles, 3);
    btl_unregisterFiles(btlFiles->partyModels, 3);
    btl_unregisterFiles(g_gameState->gameData->ctx.battle->files->partyModels, 3);
    btl_unregisterFiles(g_gameState->gameData->ctx.battle->files->weaponFiles, 3);
    battle->flags &= ~BTL_FLAG_PARTY_LOADED;
}

void btl_unloadEnemyModels(void) {
    Battle *battle = g_gameState->gameData->ctx.battle;
    btl_unregisterFiles(battle->files->enemyModels, 2);
    btl_unregisterFiles(g_gameState->gameData->ctx.battle->files->enemyModels, 2);
    battle->flags &= ~BTL_FLAG_ENEMIES_LOADED;
}

s16 btl_loadModelsStep(s32 begin) {
    GameData *gd = g_gameState->gameData;
    s16 *state = (s16 *)&gd->ctx.battle->work->loadBuf[0x9FD8 / 4];
    s16 *st;
    s32 battleId = (s16)gd->battleId;
    Unk800AA958 *ld;
    s16 step;

    ld = (Unk800AA958 *)state - 1;
    st = state;
    if (begin != 0) {
        btl_loadSoundsStep(battleId, BTL_STEPF_BEGIN | BTL_STEPF_DEP_SOUNDS, (Unk800AA24C *)ld);
        *state = 1;
    }
    step = *state;
    if (step == 1) {
        if (btl_loadSoundsStep(battleId, BTL_STEPF_DEP_SOUNDS, (Unk800AA24C *)ld) == 0) {
            btl_loadEnemyModelsStep(battleId, BTL_STEPF_BEGIN, (Unk800AA504 *)ld);
            (*st)++;
        }
    } else if (step == 2) {
        if (btl_loadEnemyModelsStep(battleId, 0, (Unk800AA504 *)ld) == 0) {
            btl_loadWeaponModelsStep(BTL_STEPF_BEGIN, ld);
            (*st)++;
        }
    } else if (step == 3) {
        if (btl_loadWeaponModelsStep(BTL_STEPF_BASE_MODEL | BTL_STEPF_UNTESTED1, ld) == 0) {
            btl_loadPartyModelsStep(BTL_STEPF_BEGIN | BTL_STEPF_BASE_MODEL, ld);
            (*st)++;
        }
    } else if (step == 4) {
        if (btl_loadPartyModelsStep(BTL_STEPF_BASE_MODEL | BTL_STEPF_UNTESTED1, ld) == 0) {
            g_gameState->gameData->ctx.battle->flags |= BTL_FLAG_PARTY_LOADED;
            *state = 0;
        }
    }
    return *st;
}

s32 btl_findWeaponModel(s32 modelId, s32 slot) {
    s32 weaponBuf = g_gameState->gameData->ctx.battle->work->weaponBufs[slot];
    return db_lookup(db_lookup(weaponBuf, DB_GET_AT, DB_PACK, 0), DB_GET_ADDR, modelId, 0);
}

void btl_getWeaponTexInfo(s32 modelId, s32 slot, s32 **outTexPages, s32 **outUvOffsets, s32 **outTexPos) {
    s32 weaponBuf = g_gameState->gameData->ctx.battle->work->weaponBufs[slot];
    Unk800AB758 *h = (Unk800AB758 *)(db_lookup(db_lookup(weaponBuf, DB_GET_AT, DB_PACK, 0), DB_GET_AT, DB_MODEL_INFO, 0) + 4);
    s32 *tables = (s32 *)((u8 *)h + h->tablesOfs);
    s32 texCount = h->count;
    *outTexPages = tables;
    tables += texCount;
    *outUvOffsets = tables;
    *outTexPos = tables + texCount;
}

u32 btl_registerModel(s32 modelId, s32 obj) {
    s16 *pa;
    s16 *pb;
    s32 pc;
    u32 slotIndex;
    Unk8E0Ld *modelSlot;
    BtlMotion *motion;
    s32 animKey;
    u32 i;
    u32 chrIndex;
    u32 cnt;

    chrIndex = evt_getObjectChrIndex(obj);
    if (chrIndex < 9) {
        slotIndex = chrIndex;
        modelSlot = (Unk8E0Ld *)game_getModel(chrIndex);
    } else {
        slotIndex = g_gameState->gameData->npcModelSlots + 9;
        modelSlot = (Unk8E0Ld *)game_getModel(slotIndex);
        g_gameState->gameData->npcModelSlots++;
    }
    modelSlot->modelId = modelId;
    modelSlot->obj = obj;
    modelSlot->model->header = (BtlModelHeader *)db_lookupRegistered(DB_GET_ADDR, modelSlot->modelId, 0);
    modelSlot->model->motion = (BtlMotion *)db_lookupRegistered(DB_GET_ADDR, btl_getAnimFileId(modelSlot->modelId), 0);
    motion = PSX_NULLABLE(modelSlot->model->motion);
    if (IS_OFFSET(motion->rotKeys)) {
        motion->rotKeys = (s32)motion + motion->rotKeys;
        if (motion->rotKeysLo != 0) {
            motion->rotKeysLo = (s32)modelSlot->model->motion + motion->rotKeysLo;
        }
    }
    btl_getModelTexInfo((modelSlot->modelId & 0xFFFFFF) | (slotIndex << 24), (s32 **)&pa, (s32 **)&pb, (s32 **)&pc);
    /* c reused for the unk8 block (a second set keeps it a global pseudo, in v1 as in the original) */
    motion = (BtlMotion *)modelSlot->model->header;
    cnt = ((BtlModelHeader *)motion)->meshCount;
    i = 0;
    if (cnt) {
        /* Probably macro residue: a dead store (deleted by flow, no code). The block boundary it leaves
           until flow keeps combine from folding i = 0 into the loop entry test. */
        animKey = 5;
    }
    for (; i < cnt; i++) {
    }
    btl_applyModelTextures(modelSlot->model, pa, pb);
    animKey = (s32)db_lookupRegistered(DB_GET_KEY_AT_IN, modelSlot->modelId, DB_TEX_ANIM);
    if (animKey != -1) {
        modelSlot->model->texAnims = (BtlUnit9C50 *)db_lookupRegistered(DB_GET_ADDR, animKey, 0);
        btl_relocTexAnims(modelSlot->model->texAnims, pc);
    }
    modelSlot->flags |= MODEL_SLOT_LOADED;
    return slotIndex;
}

s32 btl_getPartyModelId(s32 slot, s32 flags) {
    BtlUnit *unit;
    s32 tranceFlag;
    s32 idx;

    tranceFlag = 0;
    if (g_gameState->gameData->party[slot] == NULL) {
        return -1;
    }
    if (!(flags & BTL_STEPF_BASE_MODEL)) {
        unit = btl_getUnitBySlot(slot);
        if (unit != NULL) {
            tranceFlag = ((s16)unit->modelId != g_btlCharModelIds[(unit->flags & BTL_UNIT_PARTY) ? (g_gameState->gameData->characters[BTL_UNIT_INDEX(unit->flags)].info.w >> CHR_INFO_KIND_SHIFT) & 0x1F : CHR_KIND_NONE]) << 4;
        }
    }
    idx = g_gameState->gameData->party[slot]->info.w >> CHR_INFO_KIND_SHIFT;
    idx &= 0x1F;
    if ((flags & BTL_STEPF_FLIP_TRANCE) != tranceFlag) {
        if (!(flags & BTL_STEPF_BASE_MODEL)) {
            idx += CHR_KIND_TRANCE;
        }
    }
    return g_btlCharModelIds[idx] | DB_MODEL;
}

s32 btl_getWeaponModelHeader(s32 slot) {
    s32 weaponBuf = g_gameState->gameData->ctx.battle->work->weaponBufs[slot];
    return db_lookup(db_lookup(weaponBuf, DB_GET_AT, DB_PACK, 0), DB_GET_AT, DB_MODEL, 0);
}

s32 btl_swapPartyModelStep(s32 slot, s32 flags) {
    Unk800AA958 *st;
    s16 *state;
    Unk800AA958Req *req;
    s32 k;
    void *partyBuf;
    FileEntry16 *file;
    s32 id;
    s32 id2;
    s32 model;
    s32 img;
    Unk800ABBC4Tbl *table;
    Unk800ABBC4Ent *entry;
    Unk800ABBC4Tex *pages;
    Unk800ABBC4Tex *uvs;
    Unk800ABBC4Rect *texPos;
    s32 i;
    s32 j;
    s32 texCount;
    s32 x;
    s32 y;
    s32 h;
    s32 clutRow;
    s32 cid;
    ModelSlot *ent;
    BtlUnit *unit;
    u8 *pa;
    u8 *pb;
    u8 *pc;
    s32 animKey;
    s32 w;
    u8 *src;
    s32 sz;
    s32 anim;

    st = (Unk800AA958 *)&g_gameState->gameData->ctx.battle->work->loadBuf[0x9FD8 / 4];
    state = &st->state;
    req = (Unk800AA958Req *)&g_gameState->gameData->ctx.battle->work->loadBuf[0x9FA0 / 4];
    if (flags & BTL_STEPF_BEGIN) {
        st->state = 1;
    }
    switch (*state) {
    case 1:
        k = slot;
        partyBuf = (void *)g_gameState->gameData->ctx.battle->partyBufs[k];
        st->nextState = 0;
        st->unk4 = 0;
        st->unk6 = 0;
        st->unk8 = -1;
        st->unkE = 0;
        st->buf = (s32)partyBuf;
        st->unk20 = (s32)partyBuf;
        if (*(g_gameState->gameData->ctx.battle->files->partyModels + k) != NULL) {
            db_unregister(*(g_gameState->gameData->ctx.battle->files->partyModels + k));
            *(g_gameState->gameData->ctx.battle->files->partyModels + k) = NULL;
        }
        *state = 3;
        break;
    case 2:
        if (cd_getState() == 0) {
            *state = st->nextState;
        }
        break;
    case 3:
        file = (FileEntry16 *)file_find(DIR_CHAR_MODELS, btl_getPartyModelId(slot, flags & ~BTL_STEPF_FLIP_TRANCE));
        src = g_gameState->imgSector + file->sector;
        sz = file->endSector - file->sector;
        req->size0 = file->idHi << 11;
        req->dst0 = st->buf;
        req->size1 = (sz - file->idHi) << 11;
        req->dst1 = (s32)g_gameState->gameData->ctx.battle->work->loadBuf;
        cd_readChunks((s32)src, req, 2, 0);
        st->nextState = 5;
        *state = 2;
        break;
    case 5:
        id = btl_getPartyModelId(slot, flags & ~BTL_STEPF_FLIP_TRANCE);
        model = db_lookup(st->buf, DB_GET_AT, DB_PACK, 0);
        img = (s32)g_gameState->gameData->ctx.battle->work->loadBuf;
        table = (Unk800ABBC4Tbl *)db_lookup(model, DB_GET_AT, DB_MODEL_INFO, 0);
        entry = table->entries;
        for (i = 0; i < table->count; i++) {
            if (entry[i].modelId == (u16)id) {
                entry = &entry[i];
                texCount = entry->tex.b[3];
                pages = (Unk800ABBC4Tex *)((u8 *)entry + (entry->tex.w & 0xFFFFFF));
                uvs = pages + texCount;
                texPos = (Unk800ABBC4Rect *)(uvs + texCount);
                for (j = 0; j < texCount; j++) {
                    w = db_lookup(img, DB_GET_AT, (j & 0xFFFF) | DB_TIM, 0);
                    clutRow = texPos[j].y + 0xE5;
                    y = slot * 3 + clutRow;
                    h = (texPos[j].y << 7) + 0x80;
                    x = (slot << 6) + 0xC0;
                    file_setTimPos(w, VRAM_MODEL_CLUT_X, y, x, h);
                    file_loadTim(w);
                    pages[j].tpage = ((h & 0x100) >> 4) | (s16)(((x & 0x3FF) >> 6) | 0x80) | ((h & 0x200) << 2);
                    pages[j].clut = getClut(VRAM_MODEL_CLUT_X, y);
                    uvs[j].tpage = 0;
                    uvs[j].clut = h % 256 - h % 128;
                    texPos[j].w = x;
                    texPos[j].x = x;
                    texPos[j].h = h;
                    texPos[j].y = h;
                }
                break;
            }
        }
        *(g_gameState->gameData->ctx.battle->files->partyModels + slot) = (void *)model;
        db_register(model, 0);
        *state = 6;
        break;
    case 6:
        cid = g_gameState->gameData->party[slot]->info.b.id;
        ent = &g_gameState->gameData->chrModels[cid];
        unit = btl_getUnitBySlot(slot);
        if (unit != NULL) {
            /* same steps as btl_reloadPartyStep case 7 (there per party slot, with pa/pb/pc reused for the weapon) */
            id2 = btl_getPartyModelId(slot, flags & ~BTL_STEPF_FLIP_TRANCE);
            ent->model->header = db_lookupRegistered(DB_GET_ADDR, id2, 0);
            k = (id2 & 0xFFFFFF) | (cid << 24);
            pa = db_getModelTexPages(k);
            pb = db_getModelUvOffsets(k);
            pc = db_getModelTexPos(k);
            btl_applyModelTextures(ent->model, (s16 *)pa, (s16 *)pb);
            animKey = (s32)db_lookupRegistered(DB_GET_KEY_AT_IN, id2, DB_TEX_ANIM);
            if (animKey != -1) {
                anim = (s32)db_lookupRegistered(DB_GET_ADDR, animKey, 0);
            } else {
                anim = 0;
            }
            ent->model->texAnims = (BtlUnit9C50 *)anim;
            if (anim != 0) {
                btl_relocTexAnims((TexAnimSet *)ent->model->texAnims, (u16 (*)[4])pc);
            }
            btl_loadPartyMotions(unit, g_gameState->gameData->party[slot]->info.b.kind,
                          unit - g_gameState->gameData->ctx.battle->units);
            if (unit->status & BTL_STATUS_KO) {
                unit->model->motion = (BtlMotion *)unit->motions[BTL_MOTION_KO];
            } else {
                unit->model->motion = (BtlMotion *)unit->motions[(unit->flags >> BTL_UNIT_WEAK_STANCE_BIT) & 1];
            }
            ent->actor->frame = 0;
            unit->weaponModel->parent = ent->model;
        }
        *state = 0;
        break;
    }
    return *state;
}

s32 btl_getAnimFileId(s32 modelId) {
    u16 *info = db_findModelInfo(modelId);
    if (info != NULL) {
        return info[1] | DB_ANIM;
    }
    return -1;
}

s32 btl_getModelTexInfo(u32 key, s32 **outTexPages, s32 **outUvOffsets, s32 **outTexPos) {
    Unk800AB758 *h;
    u8 *dst;
    s32 size;

    h = (Unk800AB758 *)db_findModelInfo(key);
    *outTexPages = (s32 *)((u8 *)h + h->tablesOfs);
    *outUvOffsets = *outTexPages + h->count;
    *outTexPos = *outUvOffsets + h->count;
    if ((s32)(key >> 24) >= 9) {
        size = h->count * 4;
        dst = g_gameState->gameData->ctx.battle->work->heap;
        memcpy(dst, (u8 *)*outTexPages, size);
        *outTexPages = (s32 *)dst;
        g_gameState->gameData->ctx.battle->work->heap += size;
    }
    return 0;
}

u8 *btl_drawShadow(BtlShadow *shadows, s32 slot, s32 groundY, s32 bones, u8 *prim) {
    MATRIX mtx;
    s32 pad;
    SVECTOR corner;
    s32 z[9];
    BtlShadow *shadow;
    BtlObj *obj;
    s32 i;
    s32 k;
    s32 tu;
    s32 tv;

    shadow = &shadows[slot];
    obj = (BtlObj *)game_getModel(slot);
    if (obj->flags & MODEL_SLOT_NO_SHADOW) {
        return prim;
    }
    btl_calcShadowMatrix((Unk800AD4E4 *)shadow, obj, &mtx, groundY, bones & 0xFF, (bones >> 8) & 0xFF);
    for (i = 0; i < 4; i++) {
        SHADOW_QUAD(i) = (POLY_FT4 *)prim;
        setPolyFT4(SHADOW_QUAD(i));
        setRGB0(SHADOW_QUAD(i), shadow->shade, shadow->shade, shadow->shade);
        setSemiTrans(SHADOW_QUAD(i), 1);
        SHADOW_QUAD(i)->tpage = 0xC8;
        SHADOW_QUAD(i)->clut = getClut(VRAM_SHADOW_CLUT_X, VRAM_SHADOW_CLUT_Y);
        prim += sizeof(POLY_FT4);
    }
    tu = 0;
    tv = 0xE0;
    gte_MulMatrix0(&g_gameState->gameData->viewMatrix, &mtx, &mtx);
    gte_SetTransMatrix(&g_gameState->gameData->viewMatrix);
    gte_ldlv0(mtx.t);
    gte_rtv0tr();
    gte_stir(mtx.t);
    gte_SetRotMatrix(&mtx);
    gte_SetTransMatrix(&mtx);
    SHADOW_PT(-0x1000, 0x1000);
    gte_stsxy(&SHADOW_QUAD(0)->x0);
    gte_stsz(&z[0]);
    SHADOW_PT(0, 0x1000);
    gte_stsxy(&SHADOW_QUAD(0)->x1);
    gte_stsxy(&SHADOW_QUAD(1)->x0);
    gte_stsz(&z[1]);
    SHADOW_PT(0x1000, 0x1000);
    gte_stsxy(&SHADOW_QUAD(1)->x1);
    gte_stsz(&z[2]);
    SHADOW_PT(-0x1000, 0);
    gte_stsxy(&SHADOW_QUAD(0)->x2);
    gte_stsxy(&SHADOW_QUAD(2)->x0);
    gte_stsz(&z[3]);
    SHADOW_PT(0, 0);
    gte_stsxy(&SHADOW_QUAD(0)->x3);
    gte_stsxy(&SHADOW_QUAD(1)->x2);
    gte_stsxy(&SHADOW_QUAD(2)->x1);
    gte_stsxy(&SHADOW_QUAD(3)->x0);
    gte_stsz(&z[4]);
    if (z[4] == 0) {
        return prim;
    }
    SHADOW_PT(0x1000, 0);
    gte_stsxy(&SHADOW_QUAD(1)->x3);
    gte_stsxy(&SHADOW_QUAD(3)->x1);
    gte_stsz(&z[5]);
    SHADOW_PT(-0x1000, -0x1000);
    gte_stsxy(&SHADOW_QUAD(2)->x2);
    gte_stsz(&z[6]);
    SHADOW_PT(0, -0x1000);
    gte_stsxy(&SHADOW_QUAD(2)->x3);
    gte_stsxy(&SHADOW_QUAD(3)->x2);
    gte_stsz(&z[7]);
    SHADOW_PT(0x1000, -0x1000);
    gte_stsxy(&SHADOW_QUAD(3)->x3);
    gte_stsz(&z[8]);

    k = z[0];
    if (k < z[1]) {
        k = z[1];
    }
    if (k < z[3]) {
        k = z[3];
    }
    if (k < z[4]) {
        k = z[4];
    }
    setUV4(SHADOW_QUAD(0), tu, tv, tu + 0x1F, tv, tu, tv + 0x1F, tu + 0x1F, tv + 0x1F);
    addPrim(&g_gameState->gameData->ot[g_gameState->gameData->drawBuf][k >> 3], SHADOW_QUAD(0));
    k = z[1];
    if (k < z[2]) {
        k = z[2];
    }
    if (k < z[4]) {
        k = z[4];
    }
    if (k < z[5]) {
        k = z[5];
    }
    setUV4(SHADOW_QUAD(1), tu + 0x1F, tv, tu, tv, tu + 0x1F, tv + 0x1F, tu, tv + 0x1F);
    addPrim(&g_gameState->gameData->ot[g_gameState->gameData->drawBuf][k >> 3], SHADOW_QUAD(1));
    k = z[3];
    /* z[3] again where z[4] (the shared centre corner) was meant; CSE drops the test, but the branch
       still counts toward the cse path length, which keeps quad 2 out of quad 0's cse paths */
    if (k < z[3]) {
        k = z[3];
    }
    if (k < z[6]) {
        k = z[6];
    }
    if (k < z[7]) {
        k = z[7];
    }
    setUV4(SHADOW_QUAD(2), tu, tv + 0x1F, tu + 0x1F, tv + 0x1F, tu, tv, tu + 0x1F, tv);
    addPrim(&g_gameState->gameData->ot[g_gameState->gameData->drawBuf][k >> 3], SHADOW_QUAD(2));
    k = z[4];
    if (k < z[5]) {
        k = z[5];
    }
    if (k < z[7]) {
        k = z[7];
    }
    if (k < z[8]) {
        k = z[8];
    }
    setUV4(SHADOW_QUAD(3), tu + 0x1F, tv + 0x1F, tu, tv + 0x1F, tu + 0x1F, tv, tu, tv);
    addPrim(&g_gameState->gameData->ot[g_gameState->gameData->drawBuf][k >> 3], SHADOW_QUAD(3));
    return prim;
}

s32 btl_allocShadows(s32 heap, s32 count) {
    return shadow_initSlots(heap, 0x100, 0xFF, 0x200, 0xE0, count, 0);
}

void btl_calcShadowMatrix(Unk800AD4E4 *shadow, BtlObj *obj, MATRIX *out, s32 groundY, s32 bone, s32 dirBone) {
    VECTOR right;
    VECTOR up;
    VECTOR fwd;
    VECTOR dir;
    MATRIX boneMtx;
    SVECTOR downVec;
    s32 *p0;
    s32 *p1;

    if (bone == dirBone) {
        COPY_MATRIX(&boneMtx, &((MATRIX *)obj->model->bones)[bone]);
        boneMtx.t[2] = 0;
        boneMtx.t[1] = 0;
        boneMtx.t[0] = 0;
        downVec.vx = 0;
        downVec.vy = -0x1000;
        downVec.vz = 0;
        gte_SetRotMatrix(&boneMtx);
        gte_SetTransMatrix(&boneMtx);
        gte_ldv0(&downVec);
        gte_rtv0tr();
        gte_stlvnl(&dir);
        dir.vy = 0;
        VectorNormal(&dir, &fwd);
        fwd.vx = -fwd.vx;
        fwd.vy = -fwd.vy;
        fwd.vz = -fwd.vz;
    } else {
        p0 = ((MATRIX *)obj->model->bones)[bone].t;
        p1 = ((MATRIX *)obj->model->bones)[dirBone].t;
        dir.vx = p1[0] - p0[0];
        dir.vy = p1[1] - p0[1];
        dir.vz = p1[2] - p0[2];
        if (dir.vx == 0 && dir.vy == 0 && dir.vz == 0) {
            fwd.vx = 0;
            fwd.vy = 0;
            fwd.vz = 0x1000;
        } else {
            dir.vy = 0;
            VectorNormal(&dir, &fwd);
        }
    }
    up.vy = 0x1000;
    up.vx = 0;
    up.vz = 0;
    gte_ldopv1(&up);
    gte_ldopv2(&fwd);
    gte_op12();
    gte_stlvnl(&right);
    out->m[0][0] = (right.vx * shadow->width) >> 12;
    out->m[1][0] = (right.vy * shadow->width) >> 12;
    out->m[2][0] = (right.vz * shadow->width) >> 12;
    out->m[0][1] = up.vx;
    out->m[1][1] = up.vy;
    out->m[2][1] = up.vz;
    out->m[0][2] = (fwd.vx * shadow->depth) >> 12;
    out->m[1][2] = (fwd.vy * shadow->depth) >> 12;
    out->m[2][2] = (fwd.vz * shadow->depth) >> 12;
    out->t[0] = (((right.vx << 1) * shadow->offsetX + (fwd.vx << 1) * shadow->offsetZ) >> 12) +
                 ((MATRIX *)obj->model->bones)[bone].t[0];
    out->t[1] = (((right.vy << 1) * shadow->offsetX + (fwd.vy << 1) * shadow->offsetZ) >> 12) + groundY;
    out->t[2] = (((right.vz << 1) * shadow->offsetX + (fwd.vz << 1) * shadow->offsetZ) >> 12) +
                 ((MATRIX *)obj->model->bones)[bone].t[2];
}

s32 btl_soundCmd(s32 cmd, s32 soundId, s32 param1, s32 param2, s32 param3) {
    s32 size;
    GameData *gx, *ga, *gb, *gc, *gd, *ge, *gf, *gg, *gh, *gi, *gj;
    Battle1538 *bufEnd;
    s32 kind;
    s32 id;
    s32 bank;
    s32 seHandle;
    s32 seqPart;
    s32 chunk;
    s32 readEnd;
    s32 k2;
    s32 seqChunk;

    kind = SND_CMD(cmd);
    if (kind == SND_CMD_GET_STATE && g_gameState->gameData->soundLoadState != SND_LOAD_IDLE) {
        goto stream;
    }
    switch (SND_TYPE(cmd)) {
    case SND_TYPE_MUSIC:
        id = (soundId & 0xFFFF) | DB_MUSIC;
        switch (kind) {
        case SND_CMD_PLAY:
        case SND_CMD_LOAD:
            if (g_gameState->gameData->soundLoadState != SND_LOAD_IDLE) {
                return -1;
            }
            if (g_btlSoundLoadKind == -1) {
                g_btlSoundLoadKind = kind;
            }
            bank = btl_getDepLink(id);
            ga = g_gameState->gameData;
            if (ga->musicId[0] != soundId || ga->musicBank[0] != (u16)bank) {
                if (ga->flags & GD_MUSIC_PLAYING) {
                    akao_stopMusic(ga->musicHandle[0]);
                }
                g_gameState->gameData->flags &= ~(GD_MUSIC_LOADED | GD_MUSIC_PLAYING);
                g_gameState->gameData->musicBank[0] = bank;
                g_gameState->gameData->musicId[0] = soundId;
                if ((id & 0xFFFF) != 5) {
                    g_gameState->gameData->soundLoadDest = (s32)(g_loadAreaEnd + 0x140700);
                    g_btlSoundLoadSlot = -1;
                    g_gameState->gameData->soundLoadChunk = 0x16800;
                    g_btlSoundLoadId = id;
                    g_gameState->gameData->soundBankSize = 0;
                    g_gameState->gameData->soundLoadState = SND_LOAD_START;
                    return g_gameState->gameData->soundLoadState;
                }
                memmove(g_gameState->gameData->musicSeq, (void *)g_gameState->gameData->musicSeq2, 0x2000);
                g_gameState->gameData->musicVolume[0] = 0x7F;
                g_gameState->gameData->flags |= GD_MUSIC_LOADED;
            }
        finish:
            if (g_btlSoundLoadKind != SND_CMD_LOAD) {
                g_gameState->gameData->musicHandle[0] = akao_playMusic(g_gameState->gameData->musicSeq);
                gb = g_gameState->gameData;
                if (!(gb->flags & GD_MUSIC_PLAYING)) {
                    akao_fadeMusicVolumeFrom(gb->musicHandle[0], 0x1E, 0, gb->musicVolume[0]);
                }
                g_gameState->gameData->flags |= GD_MUSIC_LOADED | GD_MUSIC_PLAYING;
            }
            g_btlSoundLoadKind = -1;
            return 0;
        case SND_CMD_STOP:
            akao_stopMusic(g_gameState->gameData->musicHandle[0]);
            g_gameState->gameData->musicVolume[0] = 0x7F;
            g_gameState->gameData->flags &= ~GD_MUSIC_PLAYING;
            return 0;
        case SND_CMD_SET:
            if (SND_PARAM(cmd) != SND_PARAM_VOLUME) {
                break;
            }
            akao_setMusicVolume(g_gameState->gameData->musicHandle[0], param1);
            g_gameState->gameData->musicVolume[0] = param1;
            return 0;
        case SND_CMD_FADE:
            if (SND_PARAM(cmd) != SND_PARAM_VOLUME) {
                break;
            }
            akao_fadeMusicVolume(g_gameState->gameData->musicHandle[0], param1, param2);
            btl_setMusicVolumeFade(param2, param1);
            return 0;
        case SND_CMD_FADE_FROM:
            if (SND_PARAM(cmd) != SND_PARAM_VOLUME) {
                break;
            }
            akao_fadeMusicVolumeFrom(g_gameState->gameData->musicHandle[0], param1, param2, param3);
            btl_setMusicVolumeFade(param3, param1);
            return 0;
        case SND_CMD_FADE_ALL:
            if (SND_PARAM(cmd) != SND_PARAM_VOLUME) {
                break;
            }
            akao_fadeMusicVolume(0, param1, param2);
            btl_setMusicVolumeFade(param2, param1);
            return 0;
        case SND_CMD_SET_CONDITION:
            if (SND_PARAM(cmd) != SND_PARAM_CONDITION) {
                break;
            }
            akao_setCondition(soundId);
            return 0;
        case SND_CMD_GET_STATE:
            goto stream;
        }
        return -1;
    case SND_TYPE_SE:
        id = (soundId & 0xFFFF) | DB_SE;
        seHandle = 0;
        if (kind != SND_CMD_SET_ALL && kind != SND_CMD_FADE_ALL) {
            seHandle = (s32)db_lookupRegistered(DB_GET_ADDR, id, 0);
        }
        /* padded: a zero word follows the first jump table */
        __asm__(".section .rodata\n\t.word 0\n\t.section .text");
        switch (kind) {
        case SND_CMD_PLAY:
            akao_playSe(seHandle, param1, param2, param3);
            return 0;
        case SND_CMD_STOP:
            akao_stopSe(seHandle, param1);
            return 0;
        case SND_CMD_SET:
            switch (SND_PARAM(cmd)) {
            case SND_PARAM_VOLUME:
                akao_setSeVolume(seHandle, param1, param2);
                return 0;
            case SND_PARAM_PAN:
                akao_sendCmdA2(seHandle, param1, param2);
                return 0;
            }
            return -1;
        case SND_CMD_FADE:
            switch (SND_PARAM(cmd)) {
            case SND_PARAM_VOLUME:
                akao_fadeSeVolume(seHandle, param1, param2, param3);
                return 0;
            case SND_PARAM_PAN:
                akao_sendCmdA3(seHandle, param1, param2, param3);
                return 0;
            }
            return -1;
        case SND_CMD_SET_ALL:
            switch (SND_PARAM(cmd)) {
            case SND_PARAM_VOLUME:
                akao_setAllSeVolume(param1);
                return 0;
            case SND_PARAM_PAN:
                akao_sendCmdAA(param1);
                return 0;
            }
            return -1;
        case SND_CMD_FADE_ALL:
            switch (SND_PARAM(cmd)) {
            case SND_PARAM_VOLUME:
                akao_fadeAllSeVolume(param1, param2);
                return -1;
            case SND_PARAM_PAN:
                akao_sendCmdAB(param1, param2);
                return -1;
            }
            return -1;
        }
        return -1;
    case SND_TYPE_BANK:
        id = (soundId & 0xFFFF) | DB_SOUND_BANK;
        switch (kind) {
        case SND_CMD_LOAD:
            if (param1 >= 4) {
                gc = g_gameState->gameData;
                bufEnd = gc->ctx.battle->partyBufEnds[param1 % 4];
                gc->soundLoadDest = bufEnd->cursor;
                g_gameState->gameData->soundLoadChunk = 0x12FD8 - (bufEnd->cursor - bufEnd->base);
                gx = g_gameState->gameData;
            } else {
                g_gameState->gameData->soundLoadDest = (s32)g_gameState->gameData->ctx.battle->work->heap;
                g_gameState->gameData->soundLoadChunk = g_gameState->gameData->ctx.battle->work->ot[0] -
                                            (s32)g_gameState->gameData->ctx.battle->work->heap;
                gx = g_gameState->gameData;
            }
            k2 = gx->soundLoadChunk;
            gx->soundLoadChunk = k2 / 2048 * 2048;
            g_btlSoundLoadSlot = param1 % 4;
            g_btlSoundLoadKind = kind;
            g_btlSoundLoadId = id;
            g_gameState->gameData->soundBankSize = 0;
            g_gameState->gameData->soundLoadState = SND_LOAD_START;
            return g_gameState->gameData->soundLoadState;
        case SND_CMD_GET_STATE:
            goto stream;
        }
        return -1;
    default:
        return -1;
    }
stream:
    gd = g_gameState->gameData;
    switch (gd->soundLoadState) {
    case SND_LOAD_START:
        if (((g_btlSoundLoadId >> 16) & 0xFF) == DB_MUSIC >> 16) {
            gd->soundLoadBank = gd->musicBank[0];
        } else {
            gd->soundLoadBank = g_btlSoundLoadId;
        }
        g_btlSoundLoadSector = btl_getSoundFileSectors(g_btlSoundLoadId, &size);
        if (g_btlSoundLoadSlot == -1) {
            akao_beginBankUpload();
        }
        ge = g_gameState->gameData;
        cdRead_init(ge->cdRead, g_btlSoundLoadSector, size, (void *)ge->soundLoadDest, 0, g_gameState->gameData->soundLoadChunk, btl_onSoundRead);
        g_gameState->gameData->soundLoadState = SND_LOAD_BANK;
        break;
    case SND_LOAD_BANK:
        if (cdRead_update(gd->cdRead) == 0 && akao_getSpuTransferState() == 0) {
            gf = g_gameState->gameData;
            if (gf->cdRead[0].offset + gf->cdRead[0].chunk < gf->soundBankSize) {
                gf->soundLoadDest = (s32)gf->cdRead[0].dest;
                gg = g_gameState->gameData;
                chunk = (s32)(g_loadAreaEnd + 0x156F00) - gg->soundLoadDest;
                chunk = chunk / 2048 * 2048;
                cdRead_init(gg->cdRead, g_btlSoundLoadSector, gg->cdRead[0].size, (void *)gg->soundLoadDest,
                              gg->cdRead[0].offset + gg->cdRead[0].chunk, chunk, btl_onSoundRead);
            } else if (g_btlSoundLoadType == 7) {
                if (g_btlSoundLoadKind != SND_CMD_RESUME) {
                    gf->musicVolume[0] = 0x7F;
                }
                g_gameState->gameData->soundLoadState = SND_LOAD_SEQ_START;
            } else {
                g_btlSoundLoadKind = -1;
                g_gameState->gameData->soundLoadState = SND_LOAD_IDLE;
            }
        }
        break;
    case SND_LOAD_SEQ_START:
        seqPart = gd->cdRead[0].offset + gd->cdRead[0].chunk - gd->soundBankSize;
        if (seqPart > 0) {
            memcpy(gd->musicSeq, (u8 *)(gd->soundLoadDest + (gd->soundBankSize - gd->cdRead[0].offset)), seqPart);
        }
        g_gameState->gameData->soundLoadDest = (s32)((u8 *)g_gameState->gameData->musicSeq + seqPart);
        seqChunk = 0x6800 - seqPart;
        seqChunk = seqChunk / 2048 * 2048;
        gh = g_gameState->gameData;
        cdRead_init(gh->cdRead, g_btlSoundLoadSector, gh->cdRead[0].size, (void *)gh->soundLoadDest,
                      gh->cdRead[0].offset + gh->cdRead[0].chunk, seqChunk, NULL);
        g_gameState->gameData->soundLoadState = SND_LOAD_SEQ;
        break;
    case SND_LOAD_SEQ:
        if (cdRead_update(gd->cdRead) == 0) {
            gi = g_gameState->gameData;
            readEnd = gi->cdRead[0].offset + gi->cdRead[0].chunk;
            if (readEnd < gi->cdRead[0].size) {
                k2 = readEnd - gi->soundBankSize;
                gi->soundLoadDest = (s32)((u8 *)gi->musicSeq + k2);
                gj = g_gameState->gameData;
                cdRead_init(gj->cdRead, g_btlSoundLoadSector, gj->cdRead[0].size, (void *)gj->soundLoadDest, k2, 0x6800 - k2, NULL);
            } else {
                btl_unpackMusic((s32)gi->musicSeq);
                g_gameState->gameData->flags |= GD_MUSIC_LOADED;
                g_gameState->gameData->soundLoadState = SND_LOAD_IDLE;
                if (g_btlSoundLoadKind == SND_CMD_PLAY) {
                    goto finish;
                }
                g_btlSoundLoadKind = -1;
            }
        }
        break;
    }
    return g_gameState->gameData->soundLoadState;
}

s32 btl_getWeaponSound(s32 slot, u16 index) {
    void **fileSlot;
    GameData *gd = g_gameState->gameData;
    s32 pack;
    if (gd->party[slot] == 0) {
        return -1;
    }
    fileSlot = &gd->ctx.battle->files->weaponFiles[slot];
    pack = (s32)*fileSlot;
    if (pack == 0) {
        return -1;
    }
    return db_lookup(pack, DB_GET_KEY_AT, index | DB_SE, 0);
}

s32 btl_getWeaponSoundBank(s32 slot) {
    Character *chr = g_gameState->gameData->party[slot];

    if (chr == NULL) {
        /* two nested do-while(0) wrappers (macros?): they change the block structure enough to load the table base after the NULL check */
        do {
            do {
                return -1;
            } while (0);
        } while (0);
    }
    return ((FileEntry16 *)file_find(DIR_WEAPON_MODELS, g_weaponTable[chr->equip[0]].modelId))->idHi | DB_SOUND_BANK;
}

s32 btl_getSoundFileSectors(s32 fileId, s32 *outSize) {
    FileEntry *depEntry;
    FileEntry *file;
    if (((fileId >> 16) & 0xFF) == 7) {
        depEntry = dep_find(g_gameState->gameData->depTable, fileId);
        if (outSize != 0) {
            *outSize = depEntry->sectors << 11;
        }
        return depEntry->sector;
    } else {
        file = file_find(DIR_SOUND_BANKS, fileId);
        if (outSize != 0) {
            *outSize = (file->link - file->sector) << 11;
        }
        return file->sector;
    }
}

s16 btl_loadWeaponSoundStep(s32 slot, BtlStepTask *task) {
    BtlAnimEvtList *evtList;
    s32 i;
    BtlAnimEvt *evts;
    void **fileSlot;

    switch (task->step) {
    case 1:
        return ++task->step;
    case 2:
        fileSlot = &g_gameState->gameData->ctx.battle->files->weaponFiles[slot];
        evtList = (BtlAnimEvtList *)db_lookup((s32)*fileSlot, DB_GET_AT, DB_DEP_TABLE, 0);
        evts = evtList->events;
        for (i = evtList->count - 1; i >= 0; i--) {
            if (evts[i].data.b.kind == 9) {
                btl_soundCmd(SND_BANK_LOAD, evts[i].data.w, slot + 4, 0, 0);
                break;
            }
        }
        return ++task->step;
    case 3:
        if (btl_soundCmd(SND_GET_STATE, 0, 0, 0, 0) == 0) {
            task->step = 0;
        }
        return task->step;
    }
    return 0;
}

s32 btl_getDepLink(s32 fileId) {
    FileTable *table = g_gameState->gameData->depTable;
    FileEntry *ents = table->entries;
    s32 i;
    for (i = table->count - 1; i >= 0; i--) {
        if (ents[i].id == fileId) {
            return ents[i].link;
        }
    }
    return -1;
}

s32 btl_unpackMusic(s32 seq) {
    s32 key = db_lookup(seq, DB_GET_KEY_AT, DB_MUSIC, 0);
    s32 src = db_lookup(seq, DB_GET_ADDR, key, 0);
    memcpy((u8 *)seq, (u8 *)src, db_lookup(seq, DB_GET_SIZE, key, 0));
    return 0;
}

void btl_onSoundRead(Unk800AE844 *progress) {
    s32 addr = progress->addr;
    s32 id = g_gameState->gameData->soundLoadBank | DB_SOUND_BANK;
    s32 pack;
    s32 base;
    s32 len;

    if (progress->done == 0) {
        pack = db_lookup(addr, DB_GET_AT, DB_PACK, 0);
        base = addr;
        if (pack == -1) {
            addr = db_lookup(addr, DB_GET_ADDR, id, 0);
            g_gameState->gameData->soundBankSize = db_getSize(base);
        } else {
            addr = db_lookup(pack, DB_GET_ADDR, id, 0);
            g_gameState->gameData->soundBankSize = db_lookup(base, DB_GET_AT, 0x1B0001, 0) - g_gameState->gameData->soundLoadDest;
        }
    }
    len = g_gameState->gameData->soundBankSize - progress->done;
    if (progress->chunk < len) {
        len = progress->chunk;
    }
    len = progress->addr + len - addr;
    if (g_btlSoundLoadSlot == -1) {
        akao_uploadBank(addr, len, 0);
    } else {
        akao_loadBankInSlot(addr, g_btlSoundLoadSlot, 0);
    }
    progress->addr = addr;
}

void btl_setMusicVolumeFade(s32 volume, s32 frames) {
    GameData *gd = g_gameState->gameData;
    gd->musicVolume[0] = volume;
    if (volume == 0) {
        gd->musicFadeFrames = frames;
        gd->musicFadeStart = VSync(-1);
    }
}

s32 btl_isMusicFadingOut(void) {
    GameData *gd = g_gameState->gameData;
    return (u32)(VSync(-1) - gd->musicFadeStart) < gd->musicFadeFrames;
}

void btl_beginFrame(void) {
    ClearOTagR(g_gameState->gameData->ot[g_gameState->gameData->drawBuf] - OT_PAD, OT_LEN + 2 * OT_PAD);
    ClearOTagR((u32 *)g_gameState->gameData->ctx.battle->work->ot[g_gameState->gameData->drawBuf], BTL_BG_OT_LEN);
    ClearOTagR(fade_getOt(g_gameState->gameData->drawBuf), 1);
    ClearOTagR((u32 *)btl_getBannerOt(g_gameState->gameData->drawBuf), 1);
    setlen(&g_btlFrameStartStp[g_gameState->gameData->drawBuf].mainStp, PRIM_LEN(DR_STP));
    ((u32 *)&g_btlFrameStartStp[g_gameState->gameData->drawBuf].mainStp)[1] = GPU_STP(0, 0);
    ((u32 *)&g_btlFrameStartStp[g_gameState->gameData->drawBuf].mainStp)[2] = 0;
    addPrim(g_gameState->gameData->ot[g_gameState->gameData->drawBuf], &g_btlFrameStartStp[g_gameState->gameData->drawBuf].mainStp);
    setlen(&g_btlFrameStartStp[g_gameState->gameData->drawBuf].bgStp, PRIM_LEN(DR_STP));
    ((u32 *)&g_btlFrameStartStp[g_gameState->gameData->drawBuf].bgStp)[1] = GPU_STP(0, 0);
    ((u32 *)&g_btlFrameStartStp[g_gameState->gameData->drawBuf].bgStp)[2] = 0;
    addPrim((u32 *)g_gameState->gameData->ctx.battle->work->ot[g_gameState->gameData->drawBuf], &g_btlFrameStartStp[g_gameState->gameData->drawBuf].bgStp);
    addPrim(g_gameState->gameData->ot[g_gameState->gameData->drawBuf], fade_getOt(g_gameState->gameData->drawBuf));
    addPrim(g_gameState->gameData->ot[g_gameState->gameData->drawBuf], btl_getBannerOt(g_gameState->gameData->drawBuf));
    btlui_resetVsyncCount();
}

void btl_endFrame(void) {
    if (!(g_gameState->gameData->flags & GD_PAUSED) && ((g_padPortHeld[0] & PAD_SOFT_RESET) == PAD_SOFT_RESET || (g_padPortHeld[1] & PAD_SOFT_RESET) == PAD_SOFT_RESET)) {
        g_gameState->flags |= GS_RESET;
    }
    btlui_waitFrame();
    setlen(&g_btlFrameEndStp[g_gameState->gameData->drawBuf].mainStp, PRIM_LEN(DR_STP));
    ((u32 *)&g_btlFrameEndStp[g_gameState->gameData->drawBuf].mainStp)[1] = GPU_STP(1, 0);
    ((u32 *)&g_btlFrameEndStp[g_gameState->gameData->drawBuf].mainStp)[2] = 0;
    addPrim(&g_gameState->gameData->ot[g_gameState->gameData->drawBuf][OT_LAST], &g_btlFrameEndStp[g_gameState->gameData->drawBuf].mainStp);
    setlen(&g_btlFrameEndStp[g_gameState->gameData->drawBuf].bgStp, PRIM_LEN(DR_STP));
    ((u32 *)&g_btlFrameEndStp[g_gameState->gameData->drawBuf].bgStp)[1] = GPU_STP(1, 0);
    ((u32 *)&g_btlFrameEndStp[g_gameState->gameData->drawBuf].bgStp)[2] = 0;
    addPrim(&((u32 *)g_gameState->gameData->ctx.battle->work->ot[g_gameState->gameData->drawBuf])[BTL_BG_OT_LEN - 1], &g_btlFrameEndStp[g_gameState->gameData->drawBuf].bgStp);
    btl_putDispEnvNoUi();
    if (!(g_gameState->gameData->flags & GD_PAUSED) && !(g_gameState->gameData->ctx.ovl65->flags & BTL_FLAG_NO_DRAW)) {
        btlui_drawOTagEnv(&((u32 *)g_gameState->gameData->ctx.battle->work->ot[g_gameState->gameData->drawBuf])[BTL_BG_OT_LEN - 1], &g_gameState->drawEnv[g_gameState->gameData->drawBuf]);
        DrawOTag(&g_gameState->gameData->ot[g_gameState->gameData->drawBuf][OT_LAST]);
    }
    if (!(g_gameState->gameData->flags & GD_NO_FLIP)) {
        g_gameState->gameData->drawBuf ^= 1;
    }
}

s32 btl_preloadRequest(void) {
    return 0;
}

void btl_preloadExit(void) {
    Battle11C0 *work;
    FileEntry *file;
    s32 size;
    s32 *dst;

    work = g_gameState->gameData->ctx.battle->work;
    preload_init();
    g_gameState->gameData->preloadRequest = NULL;
    file = file_find(DIR_OVERLAYS, OVL_KEY(OVL_BATTLE_END));
    size = (file->link - file->sector) << 11;
    preload_setEntry(preload_add(), PRELOAD_KIND_READ, OVL_KEY(OVL_BATTLE_END), (s32)RAM_SUB_OVL(u8 *), size, size, file->sector, size);
    file = file_find(DIR_COMMON, FILE_BTL_END_DB);
    dst = work->loadBuf;
    size = (file->link - file->sector) << 11;
    preload_setEntry(preload_add(), PRELOAD_KIND_READ, FILE_BTL_END_DB, (s32)dst, size, size, file->sector, size);
}
