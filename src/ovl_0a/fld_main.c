#include "ovl_0a.h"

void fld_entry(void) {
    fld_init();
    do {
        fld_runArea((s16)g_gameState->gameData->fieldAreaId);
    } while (!(g_gameState->flags & (GS_RESET | GS_FLD_LEAVE)));
    fld_quit();
}

void fld_runArea(s32 areaId) {
    fld_loadArea(areaId);
    do {
        fld_main((s16)g_gameState->gameData->fieldId);
    } while (!(g_gameState->flags & (GS_RESET | GS_FLD_LEAVE | GS_FLD_NEW_AREA)));
    fld_unloadArea();
}

/* field main loop */
void fld_main(s32 fieldId) {
    fld_load(fieldId);
    SetDispMask(1);
    do {
        fld_beginFrame();
        pad_update();
        if ((g_padPortPressed & BTN_START) && (g_padPortHeld[0] & (PAD_START | PAD_UP | PAD_RIGHT | PAD_DOWN | PAD_LEFT)) && !(g_padPortHeld[0] & (PAD_SELECT | BTN_CROSS | BTN_TRIANGLE | BTN_SQUARE | BTN_L1 | BTN_R1 | BTN_L2 | BTN_R2))) {
            fld_togglePause();
        }
        if (!(g_gameState->gameData->flags & GD_PAUSED)) {
            fld_movieUpdate();
            if (!(g_gameState->gameData->flags & GD_MENU_DISABLED) && (g_padPortPressed & BTN_TRIANGLE) && (g_padPortHeld[0] & (PAD_UP | PAD_RIGHT | PAD_DOWN | PAD_LEFT | BTN_TRIANGLE)) &&
                !(g_padPortHeld[0] & (PAD_SELECT | PAD_START | BTN_CROSS | BTN_SQUARE | BTN_L1 | BTN_R1 | BTN_L2 | BTN_R2 | BTN_START))) {
                g_gameState->gameData->flags |= GD_MENU_OPEN;
                menu_open(MENU_MAIN, 0);
                evt_onMenuOpen();
            }
            if (!(g_gameState->gameData->flags & GD_MENU_OPEN)) {
                switch (evt_update()) {
                case EVT_UPD_BATTLE:
                    g_gameState->gameData->ctx.field->area->scene->exitReason = FLD_EXIT_BATTLE;
                    fld_startEncounter();
                    break;
                case EVT_UPD_FIELD:
                    if (g_gameState->gameData->ctx.field->area->scene->nextId == FLD_NEXT_ENDING) {
                        g_gameState->gameData->ctx.field->area->scene->exitReason = FLD_EXIT_ENDING;
                        g_gameState->flags |= GS_FLD_LEAVE;
                    } else {
                        g_gameState->flags |= GS_FLD_NEW_FIELD;
                    }
                    break;
                case EVT_UPD_WORLD:
                    g_gameState->gameData->ctx.field->area->scene->exitReason = FLD_EXIT_WORLD;
                    g_gameState->flags |= GS_FLD_LEAVE;
                    break;
                case EVT_UPD_CARD_GAME:
                    g_gameState->gameData->ctx.field->area->scene->exitReason = FLD_EXIT_TETRA_MASTER;
                    g_gameState->flags |= GS_FLD_LEAVE;
                    break;
                case EVT_UPD_GAME_OVER:
                    g_gameState->gameData->ctx.field->area->scene->exitReason = FLD_EXIT_GAME_OVER;
                    g_gameState->flags |= GS_FLD_LEAVE;
                    break;
                }
            }
            if (!(g_gameState->gameData->flags & GD_MENU_DISABLED)) {
                menu_update();
            }
            if (g_gameState->flags & GS_RELOAD_FILE_TABLES) {
                g_gameState->flags |= GS_FLD_NEW_FIELD;
                g_gameState->gameData->ctx.field->flags |= FLD_MAP_REFIND_SECTORS;
                break;
            }
            if (!(g_gameState->gameData->flags & GD_HIDE_BG)) {
                fldbg_draw();
            }
            if (!(g_gameState->gameData->flags & GD_HIDE_MODELS)) {
                fld_drawModels();
            }
            if (!(g_gameState->gameData->ctx.field->flags & FLD_MAP_NO_BGI_ANIM)) {
                bgi_updateAnims();
                bgi_updateFloorMoves();
            }
            if (!(g_gameState->gameData->ctx.field->flags & FLD_MAP_NO_VIBRATION)) {
                vib_update();
            }
            if (!(g_gameState->gameData->flags & GD_HIDE_SPRITES)) {
                fld_drawSprites();
            }
            if (!(g_gameState->gameData->flags & GD_HIDE_BG)) {
                fld_drawRain();
            }
            if (!(g_gameState->gameData->ctx.field->flags & FLD_MAP_NO_WINDOWS)) {
                game_updateWindows();
            }
            if (!(g_gameState->gameData->ctx.field->flags & FLD_MAP_NO_FADE)) {
                game_updateFade();
            }
            if (!(g_gameState->gameData->ctx.field->flags & FLD_MAP_ACTIVE)) {
                fld_updateEncounter();
            }
            if (!(g_gameState->gameData->ctx.field->flags & FLD_MAP_NO_TIMER)) {
                game_updateTimer();
            }
            if (!(g_gameState->gameData->ctx.field->flags & FLD_MAP_NO_PRELOAD)) {
                preload_update();
            }
        }
        fld_endFrame();
    } while (!(g_gameState->flags & (GS_RESET | GS_FLD_LEAVE | GS_FLD_NEW_AREA | GS_FLD_NEW_FIELD)));
    if (fld_needsSplashImage() != 0) {
        fld_showSplashImage();
    }
    if ((u8)fld_isDiscChangePending()) {
        fld_runDiscChange();
    }
    fld_exit();
}

void fld_gameOver(void) {
    s32 i;

    gameover_main(g_gameState->gameData->ctx.field->area->scene->eventScript);
    g_gameState->mode = GS_MODE_OPENING;
    g_gameState->flags |= GS_RELOAD_FILE_TABLES;
    if ((g_gameState->gameData->flags & GD_MUSIC_PLAYING) && g_gameState->gameData->musicId[0] != MUSIC_NONE) {
        fld_soundCommand(SND_SONG_VOL_INTPL, g_gameState->gameData->musicId[0], 0x1E, 0, 0);
        for (i = 0x1D; i >= 0; i--) {
            VSync(0);
        }
        fld_soundCommand(SND_MUSIC_STOP, g_gameState->gameData->musicId[0], 0, 0, 0);
    }
    g_resetCount++;
}

void fld_init(void) {
    FieldMap *fieldMap;
    GameData *gd;
    GameData *gd2;

    fld_allocMap();
    fieldMap = g_gameState->gameData->ctx.field;
    fld_initMap();
    g_gameState->vsyncMode = 2;
    game_setScreenHeight(SCREEN_H);
    game_freezeScreen();
    SetDispMask(1);
    g_gameState->gameData->modelHook = fld_setupActorModel;
    g_gameState->gameData->soundHook = fld_soundCommand;
    g_gameState->gameData->preloadRequest = fld_preloadRequest;
    preload_setHook(0);
    g_gameState->flags &= ~(GS_RESET | GS_FLD_LEAVE | GS_FLD_NEW_AREA);
    g_gameState->gameData->kanjiFont = 0;
    gd = g_gameState->gameData;
    gd->npcModelSlots = 0;
    gd->npcCount = 0;
    gd2 = g_gameState->gameData;
    gd2->unk836 = 0xFFFF;
    gd2->worldBankReq = 0xFFFF;
    fld_clearSoundBankCache();
    fld_clearModelCache();
    fld_loadCommon();
    fld_readSoundBank();
    game_initGfx();
    if (g_gameState->gameData->flags & GD_MUSIC_LEFT_PLAYING) {
        fld_soundCommand(SND_MUSIC_VOL_INTPL, 0, 0x78, 0, 0);
    }
    fade_initField();
    fieldMap->loadEnd = g_modeHeap;
}

void fld_quit(void) {
    s32 i;

    for (i = 1; i >= 0; i--) {
        g_padActuator[i].largeMotor = 0;
        g_padActuator[i].smallMotor = 0;
    }
    fld_stopResidentSfx();
    fld_freeCommon();
}

void fld_allocMap(void) {
    g_modeHeap = (s32)(g_loadAreaEnd + 0x28000);
    g_modeHeap += 0x2000;
    g_gameState->gameData->ctx.field = (FieldMap *)g_modeHeap;
    g_modeHeap += 0x50;
}

void fld_initMap(void) {
    s32 i = 0x2B;
    FieldMap *fm = g_gameState->gameData->ctx.field;

    fm->prevFieldId = -1;
    fm->flags = 0;
    fm->loadEnd = 0;
    fm->playerObj = 0;
    fm->unk9 = 0;
    fm->unk14 = -1;
    fm->unk18 = -1;
    fm->soundBankSize = -1;
    fm->commonFile = NULL;
    fm->area = NULL;
    do {
        fm->unk24[i] = 0;
        i--;
    } while (i >= 0);
}

s32 fld_readSoundBank(void) {
    s32 addr = (s32)db_lookupRegistered(DB_GET_ADDR, 0x90164, 0);
    s32 size = (s32)db_lookupRegistered(DB_GET_SIZE, 0x90164, 0);

    file_copyViaVram((s32)(g_loadAreaEnd + 0x146F00), addr, size, NULL);
    DrawSync(0);
    g_gameState->gameData->ctx.field->soundBankSize = size;
    return 0;
}

s32 fld_loadCommon(void) {
    PreloadEntry *preload;
    FileEntry *file;

    if (preload_isReady(FILE_FLD_COMMON) != 0) {
        preload = preload_find(FILE_FLD_COMMON);
        file_copyViaVram(g_modeHeap, preload->dest, preload->loaded, NULL);
        preload_remove(FILE_FLD_COMMON);
        DrawSync(0);
    } else {
        file = file_find(DIR_COMMON, FILE_FLD_COMMON);
        cdRead_init(g_gameState->gameData->cdRead, file->sector, (file->link - file->sector) << 11, (void *)g_modeHeap, 0, -1, 0);
        while (cdRead_update(g_gameState->gameData->cdRead) != 0) {
        }
    }
    g_gameState->gameData->ctx.field->commonFile = (void *)g_modeHeap;
    db_register((s32)g_gameState->gameData->ctx.field->commonFile, 0);
    return 0;
}

s32 fld_freeCommon(void) {
    void *common = g_gameState->gameData->ctx.field->commonFile;

    if (common != NULL) {
        db_unregister(common);
        g_gameState->gameData->ctx.field->commonFile = NULL;
    }
    return 0;
}

void fld_loadArea(s32 areaId) {
    FieldMapObj *obj;

    g_modeHeap = g_gameState->gameData->ctx.field->loadEnd;
    fld_allocArea();
    obj = g_gameState->gameData->ctx.field->area;
    fld_initArea();
    game_freezeScreen();
    fld_readAreaFile(obj, areaId);
    fld_findAreaItem(obj, areaId);
    obj->loadEnd = g_modeHeap;
}

void fld_unloadArea(void) {
    GameData *gd = g_gameState->gameData;
    FieldMapObj *obj = gd->ctx.field->area;

    fld_freeAreaFile(obj, gd->fieldAreaId);
    g_gameState->gameData->fieldAreaId = obj->areaId;
}

void fld_allocArea(void) {
    FieldMapObj *obj = (FieldMapObj *)g_modeHeap;

    g_modeHeap = (s32)(obj + 1);
    g_gameState->gameData->ctx.field->area = obj;
}

void fld_initArea(void) {
    FieldMapObj *obj = g_gameState->gameData->ctx.field->area;
    s32 i;

    obj->overlaidDbHeader = 0;
    obj->loadEnd = 0;
    obj->msgTable = NULL;
    obj->areaFile = NULL;
    obj->areaId = g_gameState->gameData->fieldAreaId;
    obj->scene = NULL;
    for (i = 1; i >= 0; i--) {
        obj->unk12[i] = 0;
    }
    for (i = 0x1F; i >= 0; i--) {
        obj->unk18[i] = 0;
    }
}

s32 fld_findAreaItem(obj, areaId)
    FieldMapObj *obj;
    u16 areaId;
{
    obj->msgTable = db_lookupRegistered(DB_GET_ADDR, areaId | DB_AREA, 0);
    return 0;
}

s32 fld_readAreaFile(FieldMapObj *obj, s32 areaId) {
    s32 id;
    GameData *gd;
    PreloadEntry *preload;
    CdReadCtx *cd;
    GameData *gd2;
    s32 bytesRead;

    gd = g_gameState->gameData;
    id = (u16)areaId | DB_AREA;
    cd = gd->cdRead;
    if (preload_isReady(id) != 0) {
        preload = preload_find(id);
        g_gameState->gameData->cdRead->end = (u8 *)(preload->dest + preload->loaded);
        g_gameState->gameData->cdRead->chunk = preload->loaded;
        g_gameState->gameData->cdRead->size = preload->readSize;
        file_copyViaVram(g_modeHeap, preload->dest, preload->loaded, NULL);
        preload_remove(id);
        DrawSync(0);
        gd2 = g_gameState->gameData;
        bytesRead = gd2->cdRead->chunk;
        if (bytesRead < gd2->cdRead->size) {
            cdRead_init(gd2->cdRead, gd2->sceneSector[0], gd2->sceneSectors[0] << 11, (void *)(g_modeHeap + bytesRead), bytesRead, -1, NULL);
            while (cdRead_update(g_gameState->gameData->cdRead) != 0) {
            }
        }
    } else {
        cdRead_init(cd, gd->sceneSector[0], gd->sceneSectors[0] << 11, (void *)g_modeHeap, 0, -1, NULL);
        while (cdRead_update(cd) != 0) {
        }
    }
    obj->areaFile = (void *)g_modeHeap;
    db_register(g_modeHeap, 0);
    g_modeHeap += db_getSize((s32)obj->areaFile);
    return 0;
}

s32 fld_freeAreaFile(FieldMapObj *obj, s16 unused) {
    db_unregister(obj->areaFile);
    obj->areaFile = NULL;
    return 0;
}

/* field start-up: load the map and set up the object slots */
void fld_load(s32 fieldId) {
    FieldScene *sc;
    s32 i;
    s32 j;

    g_modeHeap = g_gameState->gameData->ctx.field->area->loadEnd;
    fld_allocScene();
    sc = g_gameState->gameData->ctx.field->area->scene;
    fld_initScene();
    game_freezeScreen();
    g_gameState->flags &= ~GS_EXIT_BITS;
    g_gameState->gameData->flags |= GD_NO_FLIP;
    g_gameState->gameData->ctx.field->flags |= FLD_MAP_ACTIVE;
    game_setClearColor(0, 0, 0);
    g_padPortMask[0] &= ~PAD_ALL;
    for (i = 1; i >= 0; i--) {
        for (j = 3; j >= 0; j--) {
            g_fieldFlagQueue.gameFlags[i][j] = 0;
            g_fieldFlagQueue.mapFlags[i][j] = 0;
        }
    }
    g_gameState->gameData->ctx.field->playerObj = 0;
    fld_clearModelQueue();
    g_gameState->gameData->ctx.field->unk9 = 0;
    g_gameState->gameData->fieldId = fieldId;
    sc->nextId = fieldId;
    fld_loadFile(sc, fieldId);
    fld_findEventScript(sc, fieldId);
    fld_loadVibration(sc, fieldId);
    fld_setupBackground(sc, fieldId);
    fld_findLights(sc, fieldId);
    fld_getShadowTexPos(&g_gameState->gameData->ctx.field->area->scene->shadowTexX, &g_gameState->gameData->ctx.field->area->scene->shadowTexY);
    fld_initPreload();
    fld_startAudio();
    fld_soundCommand(SND_MUSIC_RESUME, -1, 0, 0, 0);
    evt_loadScripts(sc->eventScript);
    evt_init((u8 *)sc->eventScript);
    fld_compactModelCache();
    fld_loadModelBundlesInCdOrder();
    while (fld_soundCommand(SND_GET_STATE, -1, 0, 0, 0) != 0) {
    }
    msg_init();
    g_gameState->gameData->npcCount = evt_countNpcActors(0);
    g_gameState->gameData->npcModelObjs = (Unk8D4 *)g_modeHeap;
    g_gameState->gameData->npcModels = (ModelSlot *)(g_gameState->gameData->npcModelObjs + g_gameState->gameData->npcCount);
    g_modeHeap = (s32)(g_gameState->gameData->npcModels + g_gameState->gameData->npcCount);
    g_gameState->gameData->unk9 = 0;
    for (i = 0; i < 9; i++) {
        game_clearModelSlot(&g_gameState->gameData->chrModels[i]);
        g_gameState->gameData->chrModels[i].model = &g_gameState->gameData->chrModelObjs[i];
    }
    g_gameState->gameData->npcModelSlots = 0;
    for (i = 0; i < g_gameState->gameData->npcCount; i++) {
        game_clearModelSlot(&g_gameState->gameData->npcModels[i]);
        g_gameState->gameData->npcModels[i].model = &g_gameState->gameData->npcModelObjs[i];
    }
    sc->shadows = (FieldObjPlace *)g_modeHeap;
    g_modeHeap = fld_initShadows(g_modeHeap, g_gameState->gameData->npcCount + 9,
                               g_gameState->gameData->ctx.field->area->scene->shadowTexX,
                               g_gameState->gameData->ctx.field->area->scene->shadowTexY);
    fld_loadSoundBanks(sc, fieldId);
    fld_resumeResidentSfx();
    sc->heapTop = g_modeHeap;
    sc->loadEnd = g_modeHeap;
    evt_createModels();
    g_padPortMask[0] |= BTN_TRIANGLE;
    g_gameState->gameData->flags &= ~GD_NO_FLIP;
}

void fld_exit(void) {
    FieldScene *sc;
    s32 i;
    s32 id;

    sc = g_gameState->gameData->ctx.field->area->scene;
    DrawSync(0);
    fld_stopSceneSfx(sc, g_gameState->gameData->fieldId);
    g_padPortMask[0] &= ~PAD_ALL;
    fld_freeVibration(sc, g_gameState->gameData->fieldId);
    fld_freeFieldFile(sc, g_gameState->gameData->fieldId);
    fld_freeModelBundles();
    for (i = 0; i < 9; i++) {
        if (g_gameState->gameData->chrModels[i].flags & MODEL_SLOT_LOADED) {
            g_gameState->gameData->chrModels[i].flags &= ~MODEL_SLOT_LOADED;
        }
    }
    for (i = 0; i < g_gameState->gameData->npcCount; i++) {
        if (g_gameState->gameData->npcModels[i].flags & MODEL_SLOT_LOADED) {
            g_gameState->gameData->npcModels[i].flags &= ~MODEL_SLOT_LOADED;
        }
    }
    for (i = 1; i >= 0; i--) {
        g_gameState->gameData->returnSector[i] = g_gameState->gameData->sceneSector[i];
        g_gameState->gameData->returnSectors[i] = g_gameState->gameData->sceneSectors[i];
    }
    switch (g_gameState->gameData->ctx.field->area->scene->exitReason) {
    case FLD_EXIT_BATTLE:
        fld_depFindBackground(fld_depFindBattle(sc->nextId, &g_gameState->gameData->sceneSector[0], &g_gameState->gameData->sceneSectors[0]),
                      &g_gameState->gameData->sceneSector[1], &g_gameState->gameData->sceneSectors[1]);
        g_gameState->gameData->battleId = sc->nextId;
        g_gameState->mode = GS_MODE_BATTLE;
        g_gameState->prevMode = GS_MODE_FIELD;
        break;
    case FLD_EXIT_WORLD:
        id = fld_depFindArea(fld_depFindField(sc->nextId, &g_gameState->gameData->sceneSector[1], &g_gameState->gameData->sceneSectors[1]),
                           &g_gameState->gameData->sceneSector[0], &g_gameState->gameData->sceneSectors[0]);
        g_gameState->gameData->worldId = sc->nextId;
        g_gameState->gameData->worldAreaId = id;
        g_gameState->mode = GS_MODE_WORLD;
        g_gameState->prevMode = GS_MODE_FIELD;
        break;
    case FLD_EXIT_TETRA_MASTER:
        if (g_gameState->gameData->flags & GD_MUSIC_PLAYING) {
            fld_soundCommand(SND_MUSIC_SUSPEND, g_gameState->gameData->musicId[0], 0, 0, 0);
        }
        fld_suspendResidentSfx();
        akao_sendCmdE2();
        g_gameState->mode = GS_MODE_TETRA_MASTER;
        g_gameState->prevMode = GS_MODE_FIELD;
        break;
    case FLD_EXIT_GAME_OVER:
        akao_stopSe(-1, 0);
        akao_sendCmdE2();
        g_gameState->mode = GS_MODE_GAME_OVER;
        g_gameState->prevMode = GS_MODE_FIELD;
        break;
    case FLD_EXIT_ENDING:
        akao_stopSe(-1, 0);
        akao_sendCmdE2();
        g_gameState->mode = GS_MODE_ENDING;
        g_gameState->prevMode = GS_MODE_FIELD;
        break;
    case FLD_EXIT_FIELD:
        if (g_gameState->gameData->ctx.field->flags & FLD_MAP_REFIND_SECTORS) {
            id = (s16)g_gameState->gameData->ctx.field->area->areaId;
        } else {
            id = fld_depFindArea(fld_depFindField(sc->nextId, &g_gameState->gameData->sceneSector[1], &g_gameState->gameData->sceneSectors[1]),
                               &g_gameState->gameData->sceneSector[0], &g_gameState->gameData->sceneSectors[0]);
        }
        if (id != (g_gameState->gameData->fieldAreaId | DB_AREA)) {
            g_gameState->flags |= GS_FLD_NEW_AREA;
        }
        g_gameState->gameData->ctx.field->flags &= ~FLD_MAP_REFIND_SECTORS;
        g_gameState->gameData->ctx.field->area->areaId = id;
        g_gameState->gameData->ctx.field->prevFieldId = g_gameState->gameData->fieldId;
        g_gameState->gameData->fieldId = sc->nextId;
        break;
    default:
        SystemError(0x39, 0xC9);
        break;
    }
    g_gameState->gameData->depTable = (struct FileTable *)-1;
}

/* allocate the field scene block and its tables */
void fld_allocScene(void) {
    FieldScene *sc;

    g_gameState->gameData->ctx.field->area->scene = sc = (FieldScene *)g_modeHeap;
    sc->modelBundles = sc->modelBundleBuf;
    g_modeHeap += 0x90;
    sc->actorInfo = (FieldActorInfo *)g_modeHeap;
    g_modeHeap += 0x1E0;
    sc->animSfx = (FieldAnimSfx *)g_modeHeap;
    g_modeHeap += 0xC0;
}

void fld_initScene(void) {
    FieldScene *sc;
    s32 i;
    FieldActorInfo *info;
    u8 *base;
    FieldAnimSfx *sfx;
    u8 *rgb;
    u8 *c2;

    sc = g_gameState->gameData->ctx.field->area->scene;
    sc->unk8 = 0;
    sc->flags = 0;
    sc->loadEnd = 0;
    sc->eventScript = NULL;
    sc->vibration = NULL;
    sc->bg = NULL;
    sc->walkmesh = NULL;
    base = g_loadAreaEnd + 0x106700;
    sc->otBias = 0;
    sc->sprites = NULL;
    sc->shadows = NULL;
    sc->heapTop = 0;
    sc->movieState = FLD_MOVIE_IDLE;
    sc->movieRgb24 = 0;
    sc->movieFile = 0;
    sc->movieList = 0;
    sc->movieEntry = 0;
    sc->lights = NULL;
    sc->movieWork = base;
    for (i = 23; i >= 0; i--) {
        info = &sc->actorInfo[i];
        info->flags = 0;
        info->litFloor = -1;
        info->abr = 0;
        info->unk7 = 0;
        info->unk6 = 0;
        rgb = info->color.c;
        rgb[2] = 0;
        rgb[1] = 0;
        rgb[0] = 0;
        info->color.c[3] = 0;
        c2 = info->nextColor.c;
        rgb = c2;
        rgb[2] = 0;
        rgb[1] = 0;
        rgb[0] = 0;
        info->nextColor.c[3] = 0;
        info->mirror = NULL;
    }
    sc->encounterScreenX = FLD_SCREEN_CX;
    sc->encounterScreenY = FLD_SCREEN_CY;
    sc->battleMusic = MUSIC_NONE;
    sc->battleMusicBank = 0xFFFF;
    sc->encounterStep = 0;
    sc->encounterMusicStep = 0;
    sc->sndVolume = 0x7F;
    sc->movieSndVolume = 0x7F;
    sc->unk5F = 0;
    sc->shadowTexY = 0;
    sc->shadowTexX = 0;
    sc->fieldFile = NULL;
    for (i = 0; i < 9; i++) {
        sc->modelBundles[i] = 0;
    }
    sc->nextId = -1;
    sc->modelBundleCount = 0;
    sc->exitReason = FLD_EXIT_FIELD;
    sc->animSfxCount = 0;
    for (i = 15; i >= 0; i--) {
        sfx = &sc->animSfx[i];
        sfx->modelSlot = 0xFF;
        sfx->randomPitch = 0;
        sfx->modelId = 0xFFFF;
        sfx->anim = 0xFFFF;
        sfx->frame = -1;
        sfx->sound[1] = 0xFFFF;
        sfx->sound[0] = 0xFFFF;
    }
    sc->soundBankSlot = 0;
    sc->modelQueueCount = 0;
    sc->unk62 = 0;
    sc->unk64 = 0;
    for (i = 0; i >= 0; i--) {
        sc->unk61[i] = 0;
    }
    for (i = 3; i >= 0; i--) {
        sc->unk68[i] = 0;
    }
}

void fld_refindSectors(void) {
    GameState *gs;
    GameState *gs2;
    GameData *gd;
    GameData *gd2;
    s32 key;
    FieldMap *fieldMap;
    DirInfo *dirTbl;
    s32 id;
    s32 firstSector;
    u8 *base;
    FieldDirEnt *start;
    FieldDirEnt *dirEnt;

    gs = g_gameState;
    gd = gs->gameData;
    fieldMap = gd->ctx.field;
    id = -1;
    if (fieldMap->flags & FLD_MAP_REFIND_SECTORS) {
        gs2 = gs;
        gd2 = gd;
        dirTbl = gs2->dirs;
        firstSector = dirTbl[0].tableSector;
        base = g_loadAreaEnd + ((firstSector << 11) + 0x14C700);
        start = (FieldDirEnt *)(base + ((dirTbl[4].tableSector - firstSector) << 11));
        dirEnt = &start[dirTbl[4].count] - 1;
        key = fieldMap->area->scene->nextId;
        do {
            if (dirEnt->id == key) {
                id = dirEnt->areaId;
                gd2->sceneSector[1] = dirEnt->sector;
                gd2->sceneSectors[1] = dirEnt[1].sector - dirEnt->sector;
                break;
            }
            dirEnt--;
        } while (dirEnt >= start);
        start = (FieldDirEnt *)(base + ((gs2->dirs[2].tableSector - firstSector) << 11));
        dirEnt = &start[gs2->dirs[2].count] - 1;
        do {
            if (dirEnt->id == id) {
                gd2->sceneSector[0] = dirEnt->sector;
                gd2->sceneSectors[0] = dirEnt[1].sector - dirEnt->sector;
                break;
            }
            dirEnt--;
        } while (dirEnt >= start);
        g_gameState->gameData->ctx.field->area->areaId = id;
        g_gameState->gameData->musicFileSector = -1;
        g_gameState->gameData->musicFileSectors = 0xFFFF;
        start = (FieldDirEnt *)(base + ((gs2->dirs[9].tableSector - firstSector) << 11));
        dirEnt = &start[gs2->dirs[9].count] - 1;
        do {
            if (dirEnt->id == g_gameState->gameData->musicId[0]) {
                g_gameState->gameData->musicFileSector = dirEnt->sector;
                g_gameState->gameData->musicFileSectors = dirEnt[1].sector - dirEnt->sector;
                break;
            }
            dirEnt--;
        } while (dirEnt >= start);
        fld_clearModelCache();
        preload_discardRange((s32)g_gameState->gameData->cdRead[0].dest, g_gameState->gameData->cdRead[0].chunk);
    }
}

s32 fld_findEventScript(scene, fieldId)
    FieldScene *scene;
    u16 fieldId;
{
    scene->eventScript = db_lookupRegistered(DB_GET_ADDR, fieldId | DB_FIELD, 0);
    return 0;
}

s32 fld_loadVibration(scene, fieldId)
    FieldScene *scene;
    u16 fieldId;
{
    void *vibData = db_lookupRegistered(DB_GET_ADDR, fieldId | DB_VIBRATION, 0);

    scene->vibration = vibData;
    if (vibData != NULL) {
        vib_init(vibData);
    }
    return 0;
}

s32 fld_setupBackground(FieldScene *scene, s32 fieldId) {
    GameData *gd = g_gameState->gameData;
    s32 id;

    fldbg_setRenderContext(&gd->drawBuf, &gd->projDist, gd->ot[0], gd->ot[1], &gd->viewMatrix, g_modeHeap + 0x400);
    fldbg_setWorkBuffer(g_modeHeap, 0x400);
    id = fld_getBgKey(scene, fieldId);
    scene->bg = db_lookupRegistered(DB_GET_ADDR, id, 0);
    fldbg_setData(scene->bg);
    g_modeHeap = fldbg_init();
    bgi_clear();
    scene->walkmesh = db_lookupRegistered(DB_GET_ADDR, (s32)db_lookupRegistered(DB_GET_KEY_AT_IN, id, DB_WALKMESH), 0);
    bgi_init((u8 *)scene->walkmesh + 4);
    g_modeHeap = fld_initSprites(g_modeHeap);
    return 0;
}

s32 fld_findLights(FieldScene *scene, s32 unused) { /* its caller passes this, unused */
    scene->lights = db_lookupRegistered(DB_GET_ADDR, (u16)fld_getBgKey(scene) | DB_LIGHTS, 0);
    return 0;
}

void fld_startAudio(void) {
    while (fld_soundCommand(SND_GET_STATE, -1, 0, 0, 0) != 0) {
    }
    if (g_gameState->gameData->flags & GD_MUSIC_LEFT_PLAYING) {
        fld_soundCommand(SND_MUSIC_STOP, g_gameState->gameData->musicId[0], 0, 0, 0);
        g_gameState->gameData->flags &= ~GD_MUSIC_LEFT_PLAYING;
    }
    if (g_gameState->gameData->ctx.field->soundBankSize != -1) {
        akao_beginBankUpload();
        akao_uploadBank((s32)g_loadAreaEnd + 0x146F00, g_gameState->gameData->ctx.field->soundBankSize, 1);
        g_gameState->gameData->ctx.field->soundBankSize = -1;
    }
}

s32 fld_loadModelBundlesInCdOrder(void) {
    s32 best;
    s32 i;
    s32 min;
    s32 fileSector;

    if (g_gameState->gameData->ctx.field->area->scene->modelQueueCount != 0) {
        while (1) {
            best = -1;
            min = 0x7FFFFFFF;
            for (i = g_gameState->gameData->ctx.field->area->scene->modelQueueCount - 1; i >= 0; i--) {
                if (g_fieldModelQueue[i] != -1) {
                    fileSector = file_find(DIR_CHAR_MODELS, g_fieldModelQueue[i])->sector;
                    if (fileSector < min) {
                        best = i;
                        min = fileSector;
                    }
                }
            }
            if (best == -1) {
                break;
            }
            fld_loadModelBundle(g_fieldModelQueue[best]);
            g_fieldModelQueue[best] = -1;
        }
    }
    return 0;
}

s32 fld_loadFile(FieldScene *sc, s32 fieldId) {
    s32 size;
    s32 id;
    PreloadEntry *preload;
    s32 base;
    s32 i;
    s32 packSize;
    s32 rest;
    s32 k;

    size = g_gameState->gameData->sceneSectors[1] << 11;
    if ((s32)((s32)RAM_SUB_OVL(u8 *) - (g_modeHeap + size)) < 0x17000) {
        size = (s32)(RAM_SUB_OVL(u8 *) - 0x17000) - g_modeHeap;
        size = size / 2048 * 2048;
    }
    id = (u16)fieldId | DB_FIELD;
    if (preload_isReady(id)) {
        preload = preload_find(id);
        g_gameState->gameData->cdRead[0].end = (u8 *)(preload->dest + preload->loaded);
        g_gameState->gameData->cdRead[0].chunk = preload->loaded;
        g_gameState->gameData->cdRead[0].size = preload->readSize;
        file_copyViaVram(g_modeHeap, preload->dest, preload->loaded, NULL);
        preload_remove(id);
        DrawSync(0);
    } else {
        cdRead_init(g_gameState->gameData->cdRead, g_gameState->gameData->sceneSector[1], g_gameState->gameData->sceneSectors[1] << 11,
                      (void *)g_modeHeap, 0, size, NULL);
        while (cdRead_update(g_gameState->gameData->cdRead) != 0) {
        }
    }
    fld_invalidateModelCache((s32)g_gameState->gameData->cdRead[0].end);
    base = g_modeHeap;
    g_gameState->gameData->depTable = (FileTable *)db_lookup(base, DB_GET_ADDR, DB_DEP_TABLE, 0);
    id = db_lookup(base, DB_GET_AT, DB_PACK, 0);
    if (id != 0) {
        packSize = db_lookup(base, DB_GET_SIZE, DB_PACK, 0);
        if ((u32)(g_modeHeap + g_gameState->gameData->cdRead[0].chunk) < (u32)(id + packSize)) {
            rest = g_gameState->gameData->cdRead[0].chunk;
            cdRead_init(g_gameState->gameData->cdRead, g_gameState->gameData->sceneSector[1], g_gameState->gameData->sceneSectors[1] << 11,
                          (void *)(g_modeHeap + rest), rest, size - g_gameState->gameData->cdRead[0].chunk, NULL);
            while (cdRead_update(g_gameState->gameData->cdRead) != 0) {
            }
            g_gameState->gameData->cdRead[0].chunk += rest;
            fld_invalidateModelCache((s32)g_gameState->gameData->cdRead[0].end);
        }
        for (i = db_lookup(id, DB_GET_COUNT, DB_TIM, 0) - 1; i >= 0; i--) {
            k = db_lookup(id, DB_GET_AT, (u16)i | DB_TIM, 0);
            if (k != -1) {
                file_loadTim(k);
            }
        }
        for (i = db_lookup(id, DB_GET_COUNT, DB_IMAGE_PACK, 0) - 1; i >= 0; i--) {
            k = db_lookup(id, DB_GET_AT, (u16)i | DB_IMAGE_PACK, 0);
            if (k != -1) {
                fld_loadImagePack((s32 *)k);
            }
        }
        DrawSync(0);
    }
    sc->fieldFile = (void *)db_lookup(base, DB_GET_AT, (DB_PACK | 1), 0);
    size = g_gameState->gameData->cdRead[0].chunk - ((s32)sc->fieldFile - base);
    file_copyViaVram(id, (s32)sc->fieldFile, size, NULL);
    DrawSync(0);
    sc->fieldFile = (void *)id;
    if (g_gameState->gameData->cdRead[0].chunk < g_gameState->gameData->cdRead[0].size) {
        cdRead_init(g_gameState->gameData->cdRead, g_gameState->gameData->sceneSector[1], g_gameState->gameData->sceneSectors[1] << 11,
                      (void *)(id + size), g_gameState->gameData->cdRead[0].chunk, -1, NULL);
        while (cdRead_update(g_gameState->gameData->cdRead) != 0) {
        }
    }
    db_register((s32)sc->fieldFile, 0);
    g_modeHeap = (s32)sc->fieldFile + db_getSize((s32)sc->fieldFile);
    return 0;
}

s32 fld_getShadowTexPos(u16 *outX, u16 *outY) {
    FieldVramPos *vramPos = db_lookupRegistered(DB_GET_ADDR, DB_VRAM_ALLOC, 0);

    *outX = vramPos->shadowTexX;
    *outY = vramPos->shadowTexY;
    return 0;
}

s32 fld_freeFieldFile(FieldScene *scene, s16 unused) {
    db_unregister(scene->fieldFile);
    scene->fieldFile = NULL;
    return 0;
}

s32 fld_freeVibration(FieldScene *unused0, s16 unused1) {
    vib_clear();
    return 0;
}

/* old-style definition: some callers pass a second argument */
s32 fld_getBgKey(scene)
    FieldScene *scene;
{
    return db_lookup((s32)scene->fieldFile, DB_GET_KEY_AT, DB_BG, 0);
}

s32 fld_depFindBattle(battleId, outSector, outSectors)
    u16 battleId;
    s32 *outSector;
    s32 *outSectors;
{
    s32 i;
    FileTable *depTbl = g_gameState->gameData->depTable;
    s32 id = battleId | DB_BATTLE;
    FileEntry *file = depTbl->entries;

    for (i = 0; i < g_gameState->gameData->depTable->count; i++) {
        if (file[i].id == id) {
            if (outSector != NULL) {
                *outSector = file[i].sector;
            }
            if (outSectors != NULL) {
                *outSectors = file[i].sectors;
            }
            break;
        }
    }
    return id;
}

s32 fld_depFindBackground(s32 fileId, s32 *outSector, s32 *outSectors) {
    s32 id = -1;
    FileTable *depTbl = g_gameState->gameData->depTable;
    s32 i;
    FileEntry *file = depTbl->entries;

    for (i = 0; i < depTbl->count; i++) {
        if (file[i].id == fileId) {
            id = file[i].link;
            break;
        }
    }
    for (i = 0; i < g_gameState->gameData->depTable->count; i++) {
        if (file[i].id == id) {
            if (outSector != NULL) {
                *outSector = file[i].sector;
            }
            if (outSectors != NULL) {
                *outSectors = file[i].sectors;
            }
            break;
        }
    }
    return id;
}

s32 fld_depFindField(fieldId, outSector, outSectors)
    u16 fieldId;
    s32 *outSector;
    s32 *outSectors;
{
    s32 i;
    FileTable *depTbl = g_gameState->gameData->depTable;
    s32 id = fieldId | DB_FIELD;
    FileEntry *file = depTbl->entries;

    for (i = 0; i < g_gameState->gameData->depTable->count; i++) {
        if (file[i].id == id) {
            if (outSector != NULL) {
                *outSector = file[i].sector;
            }
            if (outSectors != NULL) {
                *outSectors = file[i].sectors;
            }
            break;
        }
    }
    return id;
}

s32 fld_depFindArea(s32 fileId, s32 *outSector, s32 *outSectors) {
    FileTable *depTbl = g_gameState->gameData->depTable;
    FileEntry *file = depTbl->entries;
    FileEntry *end = file + depTbl->count;
    s32 id = -1;

    for (; file < end; file++) {
        if (file->id == fileId) {
            id = file->link;
            break;
        }
    }
    for (file = depTbl->entries; file < end; file++) {
        if (file->id == id) {
            if (outSector != NULL) {
                *outSector = file->sector;
            }
            if (outSectors != NULL) {
                *outSectors = file->sectors;
            }
            break;
        }
    }
    return id;
}
