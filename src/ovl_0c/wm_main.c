#include "ovl_0c.h"

void wm_entry(void) {
    wm_init();
    do {
        wm_main(g_gameState->gameData->worldId);
    } while (!(g_gameState->flags & (GS_RESET | GS_WM_LEAVE)));
    wm_shutdown();
}

void wm_main(s32 worldId) {
    wm_load(worldId);
    SetDispMask(1);
    do {
        wm_clearOT();
        pad_update();
        if ((g_padPortPressed & BTN_START) && (g_padPortHeld[0] & (PAD_START | PAD_UP | PAD_RIGHT | PAD_DOWN | PAD_LEFT)) && !(g_padPortHeld[0] & (PAD_SELECT | BTN_CROSS | BTN_SQUARE | BTN_L1 | BTN_R1 | BTN_L2 | BTN_R2))) {
            wm_togglePause();
        }
        if (!(g_gameState->gameData->flags & GD_PAUSED)) {
            if (!(g_gameState->gameData->ctx.world->flags & WM_MAP_NO_FRAME)) {
                switch (wm_frameMain()) {
                case WM_EVT_BATTLE:
                    g_gameState->gameData->ctx.world->obj->exitReason = WM_EXIT_BATTLE;
                    wm_startBattleTransition();
                    break;
                case WM_EVT_FIELD:
                    g_gameState->gameData->ctx.world->obj->exitReason = WM_EXIT_FIELD;
                    g_gameState->flags |= GS_WM_LEAVE;
                    break;
                case WM_EVT_WORLD:
                    g_gameState->flags |= GS_WM_NEW_MAP;
                    break;
                }
            }
            if (!(g_gameState->gameData->ctx.world->flags & WM_MAP_NO_FADE)) {
                game_updateFade();
            }
            if (!(g_gameState->gameData->ctx.world->flags & WM_MAP_NO_TRANSITION)) {
                wm_updateBattleTransition();
            }
            if (!(g_gameState->gameData->ctx.world->flags & WM_MAP_NO_TIMER)) {
                game_updateTimer();
            }
        }
        wm_endFrame();
    } while (!(g_gameState->flags & (GS_RESET | GS_WM_LEAVE | GS_WM_NEW_MAP)));
    wm_exit();
}

void wm_init(void) {
    WorldMap *wm;
    GameData *gd;

    wm_allocContext();
    wm = g_gameState->gameData->ctx.world;
    wm_resetContext();
    g_gameState->vsyncMode = 3;
    game_setScreenHeight(0xDA);
    game_freezeScreen();
    SetDispMask(1);
    g_gameState->gameData->modelHook = (s32 (*)(s32, void *))wm_loadActorModel;
    g_gameState->gameData->soundHook = wm_soundCommand;
    g_gameState->gameData->preloadRequest = wm_preloadRequest;
    g_gameState->gameData->kanjiFont = 0;
    gd = g_gameState->gameData;
    gd->npcModelSlots = 0;
    gd->npcCount = 0;
    wm->flags |= WM_MAP_LOADING;
    wm_loadWorldData();
    wm_afterLoadWorldData();
    wm_loadSoundBanks();
    wm_afterLoadSoundBanks();
    wm_loadResidentMusic();
    wm_afterLoadResidentMusic();
    wm_loadShadowTexture();
    wm_afterLoadShadowTexture();
    wm_systemInit();
    game_initGfx();
    if (wm_keepEntryMusic() == 0) {
        g_gameState->gameData->flags |= GD_MUSIC_LEFT_PLAYING;
    }
    if (g_gameState->gameData->flags & GD_MUSIC_LEFT_PLAYING) {
        wm_soundCommand(SND_MUSIC_VOL_INTPL, 0, 0x78, 0, 0);
    }
    wm->mapHeapBase = (u8 *)g_modeHeap;
}

void wm_shutdown(void) {
    s32 i;

    for (i = 1; i >= 0; i--) {
        g_padActuator[i].largeMotor = 0;
        g_padActuator[i].smallMotor = 0;
    }
    w_frameSystemDestructor();
    wm_freeWorldData();
}

void wm_allocContext(void) {
    g_modeHeap = (s32)(g_loadAreaEnd + 0x32000);
    g_gameState->gameData->ctx.world = (WorldMap *)g_modeHeap;
    g_modeHeap += 0x3000;
}

void wm_resetContext(void) {
    u32 i;
    WorldMap *wm = g_gameState->gameData->ctx.world;

    wm->flags = 0;
    wm->mapHeapBase = 0;
    wm->parkedSoundSize = -1;
    wm->worldData = 0;
    wm->obj = 0;
    for (i = 0; i < 0x34; i++) {
        wm->unk14[i] = 0;
    }
}

/* load the world map background image and the sound bank */
s32 wm_loadSoundBanks(void) {
    u16 ids[2] = { WM_SOUND_BANK_PARKED, WM_SOUND_BANK };
    s32 i;
    s32 id;
    u32 *addr;
    u32 size;
    RECT *rect;

    for (i = 1; i >= 0; i--) {
        id = ids[i] | DB_SOUND_BANK;
        addr = db_lookupRegistered(DB_GET_ADDR, id, 0);
        size = (u32)db_lookupRegistered(DB_GET_SIZE, id, 0);
        if (ids[i] == WM_SOUND_BANK_PARKED) {
            rect = &g_worldParkedSoundRects[g_worldParkedSoundRectIdx];
            rect->x = g_gameState->drawEnv[g_gameState->gameData->drawBuf].clip.x;
            rect->y = g_gameState->drawEnv[g_gameState->gameData->drawBuf].clip.y;
            rect->h = 218;
            rect->w = size / 2 / rect->h + 1;
            LoadImage(rect, addr);
            DrawSync(0);
            g_gameState->gameData->ctx.world->parkedSoundSize = size;
        } else {
            akao_beginBankUpload();
            akao_uploadBank((s32)addr, size, 1);
        }
    }
    return 0;
}

s32 wm_loadResidentMusic(void) {
    s32 addr = (s32)db_lookupRegistered(DB_GET_ADDR, DB_MUSIC | WM_MUSIC_RESIDENT, 0);

    wm_copyViaVram(g_gameState->gameData->musicSeq2, addr, (s32)db_lookupRegistered(DB_GET_SIZE, DB_MUSIC | WM_MUSIC_RESIDENT, 0));
    DrawSync(0);
    return 0;
}

s32 wm_loadShadowTexture(void) {
    s32 addr = (s32)db_lookupRegistered(DB_GET_ADDR, DB_SHADOW_TIM, 0);

    file_setTimPos(addr, 0x280, 0xDE, 0x330, 0xB0);
    file_loadTim(addr);
    DrawSync(0);
    return 0;
}

s32 wm_loadWorldData(void) {
    s32 id;
    FileEntry *file;
    u8 *worldData;
    PreloadEntry *preload;

    id = wm_getWorldDataFileId();
    file = file_find(DIR_COMMON, id);
    worldData = g_loadAreaEnd + 0x34000;
    if (preload_isReady(id) != 0) {
        preload = preload_find(id);
        file_copyViaVram((s32)worldData, preload->dest, preload->loaded, 0);
        DrawSync(0);
        if (preload->loaded < preload->readSize) {
            cdRead_init(g_gameState->gameData->cdRead, file->sector, (file->link - file->sector) << 11, worldData + preload->loaded, preload->loaded, -1, 0);
            while (cdRead_update(g_gameState->gameData->cdRead) != 0) {
            }
        }
        preload_remove(id);
    } else {
        cdRead_init(g_gameState->gameData->cdRead, file->sector, (file->link - file->sector) << 11, worldData, 0, -1, 0);
        while (cdRead_update(g_gameState->gameData->cdRead) != 0) {
        }
    }
    g_gameState->gameData->ctx.world->worldData = worldData;
    db_register((s32)g_gameState->gameData->ctx.world->worldData, 0);
    return 0;
}

s32 wm_afterLoadSoundBanks(void) {
    return 0;
}

s32 wm_afterLoadWorldData(void) {
    return 0;
}

s32 wm_afterLoadResidentMusic(void) {
    return 0;
}

s32 wm_freeWorldData(void) {
    void *data = g_gameState->gameData->ctx.world->worldData;

    if (data != NULL) {
        db_unregister(data);
        g_gameState->gameData->ctx.world->worldData = 0;
    }
    return 0;
}

s32 wm_afterLoadShadowTexture(void) {
    return 0;
}

s32 wm_getWorldDataFileId(void) {
    s32 fileId = FILE_WM_DATA;
    if (g_evtGlobalVars.h[0] >= 0x2B52) {
        fileId = FILE_WM_DATA_LATE;
    }
    return fileId;
}

void wm_load(s32 worldId) {
    WorldMapObj *obj;
    s32 i;

    g_modeHeap = (s32)g_gameState->gameData->ctx.world->mapHeapBase;
    wm_setupMapObj();
    obj = g_gameState->gameData->ctx.world->obj;
    wm_resetMapObj();
    g_gameState->flags &= ~(GS_RESET | GS_WM_LEAVE | GS_WM_NEW_MAP);
    g_gameState->gameData->ctx.world->flags |= WM_MAP_NO_TRANSITION;
    g_gameState->gameData->worldId = worldId;
    obj->sceneId = worldId;
    wm_fetchParkedSound();
    wm_loadFile(obj, worldId);
    wm_findMapScript(obj, worldId);
    wm_findAreaData(obj, (s16)g_gameState->gameData->worldAreaId);
    wm_uploadParkedSound();
    wm_soundCommand(SND_MUSIC_RESUME, 0, 0, 0, 0);
    evt_loadScripts(obj->mapScript);
    evt_init((u8 *)obj->mapScript);
    while (wm_soundCommand(SND_GET_STATE, 0, 0, 0, 0) != 0) {
    }
    msg_init();
    g_gameState->gameData->npcCount = evt_countNpcActors(0);
    g_gameState->gameData->npcModelObjs = (Unk8D4 *)(obj + 1);
    g_gameState->gameData->npcModels = (ModelSlot *)&g_gameState->gameData->npcModelObjs[g_gameState->gameData->npcCount];
    g_modeHeap = (s32)&g_gameState->gameData->npcModels[g_gameState->gameData->npcCount];
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
    obj->shadows = (void *)g_modeHeap;
    g_modeHeap = wm_initShadowPrims(g_modeHeap, g_gameState->gameData->npcCount + 9);
    obj->heapEnd = (void *)g_modeHeap;
    obj->primCursor = (s32)obj->loadPtr;
    evt_createModels();
    wm_finishLoad();
    preload_init();
    g_gameState->gameData->ctx.world->flags &= ~WM_MAP_LOADING;
}

void wm_exit(void) {
    WorldMapObj *obj;
    GameData *gd;
    s32 i;
    s32 mode;

    obj = g_gameState->gameData->ctx.world->obj;
    DrawSync(0);
    while (wm_soundCommand(SND_GET_STATE, 0, 0, 0, 0) != 0) {
    }
    akao_stopSe(-1, 0);
    wm_freeFile(obj, g_gameState->gameData->worldId);
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
    w_frameMapDestructor();
    gd = g_gameState->gameData;
    mode = gd->ctx.world->obj->exitReason;
    switch (mode) {
    case WM_EXIT_BATTLE:
        wm_depFindBackground(wm_depFindBattle(obj->sceneId, &gd->sceneSector[0], &gd->sceneSectors[0]), &g_gameState->gameData->sceneSector[1],
                      &g_gameState->gameData->sceneSectors[1]);
        g_gameState->gameData->battleId = obj->sceneId;
        g_gameState->mode = mode;
        g_gameState->prevMode = GS_MODE_WORLD;
        break;
    case WM_EXIT_WORLD:
        i = wm_depFindArea(wm_depFindField(obj->sceneId, &gd->sceneSector[1], &gd->sceneSectors[1]), &g_gameState->gameData->sceneSector[0],
                          &g_gameState->gameData->sceneSectors[0]);
        g_gameState->gameData->worldId = obj->sceneId;
        g_gameState->gameData->worldAreaId = i;
        break;
    case WM_EXIT_FIELD:
        g_gameState->gameData->fieldAreaId = wm_depFindArea(wm_depFindField(obj->sceneId, &gd->sceneSector[1], &gd->sceneSectors[1]),
                                                 &g_gameState->gameData->sceneSector[0], &g_gameState->gameData->sceneSectors[0]);
        g_gameState->gameData->fieldId = obj->sceneId;
        g_gameState->mode = mode;
        g_gameState->prevMode = GS_MODE_WORLD;
        break;
    default:
        SystemError(0x39, 0x194);
        break;
    }
    g_gameState->gameData->depTable = (struct FileTable *)-1;
}

void wm_setupMapObj(void) {
    WorldMap *wm = g_gameState->gameData->ctx.world;

    wm->obj = &wm->objData;
    wm->obj->loadBase = g_loadAreaEnd + 0x5EB00;
    g_modeHeap = (s32)(wm + 1);
}

/* reset the world map object */
void wm_resetMapObj(void) {
    WorldMapObj *mapObj = g_gameState->gameData->ctx.world->obj;
    s32 i;

    mapObj->swirlX = 0xA0;
    mapObj->swirlY = 0x6D;
    mapObj->flags = 0;
    mapObj->heapEnd = NULL;
    mapObj->mapScript = NULL;
    mapObj->areaData = NULL;
    mapObj->shadows = NULL;
    mapObj->palAnims = NULL;
    mapObj->palAnimCount = 0;
    mapObj->primCursor = 0;
    mapObj->swirlFrames = 0;
    mapObj->battleStep = 0;
    mapObj->battleMusic = MUSIC_NONE;
    mapObj->loadPtr = mapObj->loadBase;
    for (i = 1; i >= 0; i--) {
        mapObj->files[i] = NULL;
    }
    mapObj->sceneId = -1;
    mapObj->exitReason = WM_EXIT_WORLD;
    for (i = 0; i < sizeof(mapObj->unk8); i++) {
        mapObj->unk8[i] = 0;
    }
    for (i = 0; i < sizeof(mapObj->unk3C); i++) {
        mapObj->unk3C[i] = 0;
    }
}

s32 wm_findAreaData(obj, areaId)
    WorldMapObj *obj;
    u16 areaId;
{
    obj->areaData = db_lookupRegistered(DB_GET_ADDR, areaId | DB_AREA, 0);
    return 0;
}

s32 wm_findMapScript(obj, scriptId)
    WorldMapObj *obj;
    u16 scriptId;
{
    obj->mapScript = db_lookupRegistered(DB_GET_ADDR, scriptId | DB_FIELD, 0);
    return 0;
}

void wm_fetchParkedSound(void) {
    GameData *gd = g_gameState->gameData;
    RECT *rect;
    u16 h;
    u32 *dst;

    if (gd->ctx.world->parkedSoundSize != -1) {
        rect = &g_worldParkedSoundRects[g_worldParkedSoundRectIdx];
        h = 0xDA;
        rect->x = g_gameState->drawEnv[gd->drawBuf].clip.x;
        rect->y = g_gameState->drawEnv[g_gameState->gameData->drawBuf].clip.y;
        dst = (u32 *)(g_loadAreaEnd + 0x91800);
        rect->h = h;
        rect->w = ((u32)g_gameState->gameData->ctx.world->parkedSoundSize >> 1) / h + 1;
        StoreImage(rect, dst);
        DrawSync(0);
    }
}

void wm_uploadParkedSound(void) {
    u8 *dst;

    while (wm_soundCommand(SND_GET_STATE, 0, 0, 0, 0) != 0) {
    }
    if (g_gameState->gameData->ctx.world->parkedSoundSize != -1) {
        dst = g_loadAreaEnd + 0x91800;
        akao_beginBankUpload();
        akao_uploadBank((s32)dst, g_gameState->gameData->ctx.world->parkedSoundSize, 1);
        g_gameState->gameData->ctx.world->parkedSoundSize = -1;
    }
}

s32 wm_loadFile(WorldMapObj *obj, s32 unused) { /* its caller passes this, unused */
    GameData *gd;
    u8 *file;
    s32 pack0;
    s32 pack1;
    s32 i;
    s32 tim;
    u8 *palData;
    s32 spare;

    gd = g_gameState->gameData;
    cdRead_init(gd->cdRead, gd->sceneSector[1], gd->sceneSectors[1] << 11, obj->loadPtr, 0, -1, 0);
    while (cdRead_update(g_gameState->gameData->cdRead) != 0) {
    }
    file = obj->loadPtr;
    pack0 = db_lookup((s32)file, DB_GET_ADDR, DB_PACK, 0);
    pack1 = db_lookup((s32)file, DB_GET_ADDR, (DB_PACK | 1), 0);
    g_gameState->gameData->depTable = (struct FileTable *)db_lookup((s32)file, DB_GET_ADDR, DB_DEP_TABLE, 0);
    for (i = db_lookup(pack0, DB_GET_COUNT, DB_TIM, 0) - 1; i >= 0; i--) {
        tim = db_lookup(pack0, DB_GET_AT, (i & 0xFFFF) | DB_TIM, 0);
        if (tim != -1) {
            file_loadTim(tim);
        }
    }
    DrawSync(0);
    wm_copyViaVram(pack0, pack1, db_getSize(pack1));
    DrawSync(0);
    pack1 = pack0;
    obj->files[1] = (void *)pack1;
    db_register(pack1, 0);
    palData = (u8 *)db_lookup(pack1, DB_GET_ADDR, DB_PAL_ANIM, 0);
    obj->palAnimCount = *palData;
    palData += 4;
    obj->palAnims = (WmPalAnim *)palData;
    obj->loadPtr = (u8 *)obj->files[1] + db_getSize((s32)obj->files[1]);
    return 0;
}

s32 wm_freeFile(WorldMapObj *obj, s16 unused) {
    if (obj->files[1] != NULL) {
        db_unregister(obj->files[1]);
        obj->files[1] = NULL;
    }
    return 0;
}

s32 wm_depFindBattle(s16 battleId, s32 *outSector, s32 *outSectors) {
    s32 i;
    FileTable *table = g_gameState->gameData->depTable;
    s32 id = (u16)battleId | DB_BATTLE;
    FileEntry *files = table->entries;

    for (i = 0; i < g_gameState->gameData->depTable->count; i++) {
        if (files[i].id == id) {
            if (outSector != NULL) {
                *outSector = files[i].sector;
            }
            if (outSectors != NULL) {
                *outSectors = files[i].sectors;
            }
            break;
        }
    }
    return id;
}

s32 wm_depFindBackground(s32 fileId, s32 *outSector, s32 *outSectors) {
    s32 id = -1;
    FileTable *table = g_gameState->gameData->depTable;
    s32 i;
    FileEntry *files = table->entries;

    for (i = 0; i < table->count; i++) {
        if (files[i].id == fileId) {
            id = files[i].link;
            break;
        }
    }
    for (i = 0; i < g_gameState->gameData->depTable->count; i++) {
        if (files[i].id == id) {
            if (outSector != NULL) {
                *outSector = files[i].sector;
            }
            if (outSectors != NULL) {
                *outSectors = files[i].sectors;
            }
            break;
        }
    }
    return id;
}

s32 wm_depFindField(s16 fieldId, s32 *outSector, s32 *outSectors) {
    s32 i;
    FileTable *table = g_gameState->gameData->depTable;
    s32 id = (u16)fieldId | DB_FIELD;
    FileEntry *files = table->entries;

    for (i = 0; i < g_gameState->gameData->depTable->count; i++) {
        if (files[i].id == id) {
            if (outSector != NULL) {
                *outSector = files[i].sector;
            }
            if (outSectors != NULL) {
                *outSectors = files[i].sectors;
            }
            break;
        }
    }
    return id;
}

s32 wm_depFindArea(s32 fileId, s32 *outSector, s32 *outSectors) {
    s32 id = -1;
    FileTable *table = g_gameState->gameData->depTable;
    s32 i;
    FileEntry *files = table->entries;

    for (i = 0; i < table->count; i++) {
        if (files[i].id == fileId) {
            id = files[i].link;
            break;
        }
    }
    for (i = 0; i < g_gameState->gameData->depTable->count; i++) {
        if (files[i].id == id) {
            if (outSector != NULL) {
                *outSector = files[i].sector;
            }
            if (outSectors != NULL) {
                *outSectors = files[i].sectors;
            }
            break;
        }
    }
    return id;
}

u32 wm_loadActorModel(s32 modelId, EvtObject *actor) {
    WmObjSlot *objSlot;
    u32 id;
    WmAnimHdr *h;
    s32 chrIdx;
    s32 rotKeysOfs;
    WmModel *mdl;
    WmModelGroup *grp;
    u32 i;
    s32 primSize;

    chrIdx = evt_getObjectChrIndex(actor);
    if ((u32)chrIdx < 9) {
        id = chrIdx;
        objSlot = (WmObjSlot *)game_getModel(chrIdx);
    } else {
        id = g_gameState->gameData->npcModelSlots + 9;
        objSlot = (WmObjSlot *)game_getModel(id);
        g_gameState->gameData->npcModelSlots++;
    }
    objSlot->modelId = modelId;
    objSlot->model->model = db_lookupRegistered(DB_GET_ADDR, modelId, 0);
    if ((objSlot->model->anim = db_lookupRegistered(DB_GET_ADDR, wm_findDefaultAnim(objSlot->modelId), 0)) != NULL) {
        h = objSlot->model->anim;
        if (IS_OFFSET(h->rotKeys)) {
            rotKeysOfs = h->rotKeysLo;
            h->rotKeys = (s32)h + h->rotKeys;
            if (rotKeysOfs != 0) {
                h->rotKeysLo = (s32)objSlot->model->anim + rotKeysOfs;
            }
        }
    }
    objSlot->actor = actor;
    mdl = objSlot->model->model;
    grp = mdl->groups;
    primSize = 0;
    if (IS_OFFSET(grp)) {
        grp = (WmModelGroup *)((s32)grp + (s32)mdl);
    }
    for (i = 0; i < mdl->groupCount; i++) {
        primSize += grp[i].packetSize;
    }
    primSize *= 2;
    primSize += (objSlot->model->model->boneCount << 5) + objSlot->model->model->groupCount * 12;
    wm_initModel(objSlot->model, g_gameState->gameData->ctx.world->obj->primCursor);
    wm_buildPackets(objSlot->model, db_getModelTexPages(objSlot->modelId), db_getModelUvOffsets(objSlot->modelId));
    g_gameState->gameData->ctx.world->obj->primCursor += primSize;
    objSlot->flags |= WM_SLOT_LOADED;
    return id;
}

s32 wm_findDefaultAnim(s32 modelId) {
    WmAnimTable *table = db_lookupRegistered(DB_GET_AT_IN, modelId, DB_MODEL_INFO);
    s32 i;
    WmAnimTableEntry *entry;

    entry = table->entries;
    for (i = 0; i < table->count; i++) {
        if (entry->modelId == (u16)modelId) {
            return entry->animId | DB_ANIM;
        }
    }
    return -1;
}

void wm_drawShadow(WmObjEnt *shadows, s32 slot, s32 groundY, s32 jointIdx) {
    MATRIX mat;
    POLY_FT4 *quads[4];
    SVECTOR corner;
    s32 z[9];
    WmObjEnt *shadow;
    WmObjSlot *objSlot;
    s32 i;
    s32 k;

    shadow = &shadows[slot];
    objSlot = (WmObjSlot *)game_getModel(slot);
    if (objSlot->flags & WM_SLOT_NO_SHADOW) {
        return;
    }
    wm_calcShadowMatrix(shadow, objSlot, &mat, groundY, jointIdx);
    for (i = 0; i < 4; i++) {
        quads[i] = shadow->prims[g_gameState->gameData->drawBuf][i];
        setRGB0(quads[i], shadow->shade, shadow->shade, shadow->shade);
    }
    gte_MulMatrix0(&g_gameState->gameData->viewMatrix, &mat, &mat);
    gte_SetTransMatrix(&g_gameState->gameData->viewMatrix);
    gte_ldlv0(mat.t);
    gte_rtv0tr();
    gte_stir(mat.t);
    gte_SetRotMatrix(&mat);
    gte_SetTransMatrix(&mat);
    SHADOW_PT(-0x1000, 0x1000);
    gte_stsxy(&quads[0]->x0);
    gte_stsz(&z[0]);
    SHADOW_PT(0, 0x1000);
    gte_stsxy(&quads[0]->x1);
    gte_stsxy(&quads[1]->x0);
    gte_stsz(&z[1]);
    SHADOW_PT(0x1000, 0x1000);
    gte_stsxy(&quads[1]->x1);
    gte_stsz(&z[2]);
    SHADOW_PT(-0x1000, 0);
    gte_stsxy(&quads[0]->x2);
    gte_stsxy(&quads[2]->x0);
    gte_stsz(&z[3]);
    SHADOW_PT(0, 0);
    gte_stsxy(&quads[0]->x3);
    gte_stsxy(&quads[1]->x2);
    gte_stsxy(&quads[2]->x1);
    gte_stsxy(&quads[3]->x0);
    gte_stsz(&z[4]);
    SHADOW_PT(0x1000, 0);
    gte_stsxy(&quads[1]->x3);
    gte_stsxy(&quads[3]->x1);
    gte_stsz(&z[5]);
    SHADOW_PT(-0x1000, -0x1000);
    gte_stsxy(&quads[2]->x2);
    gte_stsz(&z[6]);
    SHADOW_PT(0, -0x1000);
    gte_stsxy(&quads[2]->x3);
    gte_stsxy(&quads[3]->x2);
    gte_stsz(&z[7]);
    SHADOW_PT(0x1000, -0x1000);
    gte_stsxy(&quads[3]->x3);
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
    SHADOW_ADD(k, 0);
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
    SHADOW_ADD(k, 1);
    k = z[3];
    if (k < z[6]) {
        k = z[6];
    }
    if (k < z[7]) {
        k = z[7];
    }
    SHADOW_ADD(k, 2);
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
    SHADOW_ADD(k, 3);
}

s32 wm_initShadowPrims(s32 heap, s32 count) {
    return shadow_initSlots(heap, 0x280, 0xDE, 0x330, 0xB0, count, 1);
}

/* object matrix facing the joint's direction (in the xz plane) */
void wm_calcShadowMatrix(WmObjEnt *shadow, WmObjSlot *obj, MATRIX *out, s32 groundY, s32 jointIdx) {
    MATRIX mat;
    s32 *dst;
    s32 *src;
    SVECTOR axis;
    VECTOR dir;
    VECTOR side;
    VECTOR up;
    VECTOR fwd;

    /* word-wise copy of the joint matrix */
    dst = (s32 *)&mat;
    src = (s32 *)&((MATRIX *)obj->model->boneMatrices)[jointIdx];
    dst[0] = src[0];
    dst[1] = src[1];
    dst[2] = src[2];
    dst[3] = src[3];
    dst[4] = src[4];
    dst[5] = src[5];
    dst[6] = src[6];
    dst[7] = src[7];
    mat.t[2] = 0;
    mat.t[1] = 0;
    mat.t[0] = 0;
    axis.vx = 0;
    axis.vy = -0x1000;
    axis.vz = 0;
    gte_SetRotMatrix(&mat);
    gte_SetTransMatrix(&mat);
    gte_ldv0(&axis);
    gte_rtv0tr();
    gte_stlvnl(&dir);
    dir.vy = 0;
    VectorNormal(&dir, &fwd);
    up.vy = 0x1000;
    up.vx = 0;
    up.vz = 0;
    fwd.vx = -fwd.vx;
    fwd.vy = -fwd.vy;
    fwd.vz = -fwd.vz;
    gte_ldopv1(&up);
    gte_ldopv2(&fwd);
    gte_op12();
    gte_stlvnl(&side);
    out->m[0][0] = side.vx * shadow->scaleX >> 12;
    out->m[1][0] = side.vy * shadow->scaleX >> 12;
    out->m[2][0] = side.vz * shadow->scaleX >> 12;
    out->m[0][1] = up.vx;
    out->m[1][1] = up.vy;
    out->m[2][1] = up.vz;
    out->m[0][2] = fwd.vx * shadow->scaleZ >> 12;
    out->m[1][2] = fwd.vy * shadow->scaleZ >> 12;
    out->m[2][2] = fwd.vz * shadow->scaleZ >> 12;
    out->t[0] = ((side.vx * shadow->offsetX + fwd.vx * shadow->offsetZ) >> 12) + ((MATRIX *)obj->model->boneMatrices)[jointIdx].t[0];
    out->t[1] = ((side.vy * shadow->offsetX + fwd.vy * shadow->offsetZ) >> 12) + groundY;
    out->t[2] = ((side.vz * shadow->offsetX + fwd.vz * shadow->offsetZ) >> 12) + ((MATRIX *)obj->model->boneMatrices)[jointIdx].t[2];
}

/* world map music/sound request handler (GameData.unk83C); unk851 drives the streamed music load */
s32 wm_soundCommand(s32 cmd, s32 soundId, s32 param1, s32 param2, s32 param3) {
    s32 kind;
    s32 id;
    s32 h;
    s32 link;
    s32 readLen;
    s32 seqBytes;
    s32 sz;
    s32 seqOffset;
    s32 room;

    kind = SND_CMD(cmd);
    if (kind == SND_CMD_GET_STATE && g_gameState->gameData->soundLoadState != SND_LOAD_IDLE) {
        goto state;
    }
    switch (SND_TYPE(cmd)) {
    case SND_TYPE_MUSIC:
        id = (soundId & 0xFFFF) | DB_MUSIC;
        switch (kind) {
        case SND_CMD_RESUME:
            if (g_gameState->gameData->savedMusicId == MUSIC_NONE) {
                return 0;
            }
            if (g_gameState->gameData->savedMusicField != g_gameState->gameData->worldId) {
                g_gameState->gameData->savedMusicId = MUSIC_NONE;
                g_gameState->gameData->savedMusicField = 0xFFFF;
                return 0;
            }
            g_worldMusicReqKind = kind;
            soundId = g_gameState->gameData->savedMusicId;
            g_gameState->gameData->musicVolume[0] = g_gameState->gameData->savedMusicVolume;
            id = soundId | DB_MUSIC;
            g_gameState->gameData->savedMusicId = MUSIC_NONE;
        case SND_CMD_PLAY:
        case SND_CMD_LOAD:
            if (g_gameState->gameData->soundLoadState != SND_LOAD_IDLE) {
                return -1;
            }
            if (g_worldMusicReqKind == -1) {
                g_worldMusicReqKind = kind;
            }
            if (soundId == 100) {
                if (g_gameState->gameData->flags & GD_MUSIC_PLAYING) {
                    akao_stopMusic(g_gameState->gameData->musicHandle[0]);
                    g_gameState->gameData->flags &= ~GD_MUSIC_PLAYING;
                }
                g_gameState->gameData->musicId[1] = soundId;
                g_gameState->gameData->musicBank[1] = wm_depGetLink(id);
                g_gameState->gameData->musicVolume[1] = 0x7F;
                g_gameState->gameData->musicHandle[1] = akao_playMusic((void *)g_gameState->gameData->musicSeq2);
                if (!(g_gameState->gameData->flags & GD_MUSIC2_PLAYING)) {
                    akao_fadeMusicVolumeFrom(g_gameState->gameData->musicHandle[1], 0x1E, 0, g_gameState->gameData->musicVolume[1]);
                }
                g_gameState->gameData->flags |= (GD_MUSIC2_LOADED | GD_MUSIC2_PLAYING);
                goto done;
            }
            link = wm_depGetLink(id);
            if (g_gameState->gameData->musicId[0] != soundId || g_gameState->gameData->musicBank[0] != (u16)link || (g_gameState->gameData->flags & GD_MUSIC2_PLAYING)) {
                if (g_gameState->gameData->flags & GD_MUSIC_PLAYING) {
                    g_gameState->gameData->flags |= GD_MUSIC_STOP_PENDING;
                }
                if (g_gameState->gameData->flags & GD_MUSIC2_PLAYING) {
                    g_gameState->gameData->flags |= GD_MUSIC_STOP_PENDING;
                }
                if (g_gameState->gameData->musicId[0] != soundId || (g_gameState->gameData->flags & GD_MUSIC_PLAYING)) {
                    g_gameState->gameData->flags &= ~GD_MUSIC_LOADED;
                }
                g_gameState->gameData->flags &= ~(GD_MUSIC_PLAYING | GD_MUSIC2_PLAYING);
                g_gameState->gameData->musicBank[0] = link;
                g_gameState->gameData->musicId[0] = soundId;
                if (!(g_gameState->gameData->flags & GD_MUSIC_LOADED)) {
                    g_worldMusicReqId = id;
                    g_gameState->gameData->soundLoadState = SND_LOAD_WAIT_FADE;
                    return g_gameState->gameData->soundLoadState;
                }
            }
            if (g_worldMusicReqKind == SND_CMD_LOAD) {
                goto done;
            }
        start:
            g_gameState->gameData->musicHandle[0] = akao_playMusic(g_gameState->gameData->musicSeq);
            if (!(g_gameState->gameData->flags & GD_MUSIC_PLAYING)) {
                akao_fadeMusicVolumeFrom(g_gameState->gameData->musicHandle[0], 0x1E, 0, g_gameState->gameData->musicVolume[0]);
            }
            g_gameState->gameData->flags |= (GD_MUSIC_LOADED | GD_MUSIC_PLAYING);
        done:
            g_worldMusicReqKind = -1;
            return 0;
        case SND_CMD_GET_STATE:
            if (g_gameState->gameData->soundLoadState != SND_LOAD_IDLE) {
                goto state;
            }
            return wm_isMusicFadingOut() ? 6 : 0;
        case SND_CMD_SUSPEND:
            akao_saveMusic();
            g_gameState->gameData->savedMusicId = g_gameState->gameData->musicId[0];
            g_gameState->gameData->savedMusicVolume = g_gameState->gameData->musicVolume[0];
            g_gameState->gameData->savedMusicField = g_gameState->gameData->worldId;
        case SND_CMD_STOP:
            if (g_gameState->gameData->musicId[0] == soundId) {
                akao_stopMusic(g_gameState->gameData->musicHandle[0]);
                g_gameState->gameData->musicVolume[0] = 0x7F;
                g_gameState->gameData->flags &= ~GD_MUSIC_PLAYING;
            } else if (g_gameState->gameData->musicId[1] == soundId) {
                akao_stopMusic(g_gameState->gameData->musicHandle[1]);
                g_gameState->gameData->musicVolume[0] = 0x7F;
                g_gameState->gameData->flags &= ~GD_MUSIC2_PLAYING;
            }
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
            wm_setMusicVolumeFade(param2, param1);
            return 0;
        case SND_CMD_FADE_FROM:
            if (SND_PARAM(cmd) != SND_PARAM_VOLUME) {
                break;
            }
            akao_fadeMusicVolumeFrom(g_gameState->gameData->musicHandle[0], param1, param2, param3);
            wm_setMusicVolumeFade(param3, param1);
            return 0;
        case SND_CMD_FADE_ALL:
            if (SND_PARAM(cmd) != SND_PARAM_VOLUME) {
                break;
            }
            akao_fadeMusicVolume(0, param1, param2);
            wm_setMusicVolumeFade(param2, param1);
            return 0;
        case SND_CMD_FADE_ALL_FROM:
            if (SND_PARAM(cmd) != SND_PARAM_VOLUME) {
                break;
            }
            akao_fadeMusicVolumeFrom(0, param1, param2, param3);
            wm_setMusicVolumeFade(param3, param1);
            return 0;
        case SND_CMD_SET_CONDITION:
            if (SND_PARAM(cmd) != SND_PARAM_CONDITION) {
                break;
            }
            akao_setCondition(soundId);
            return 0;
        }
        return -1;
    case SND_TYPE_SE:
        id = (soundId & 0xFFFF) | DB_SE;
        h = 0;
        if (kind != SND_CMD_SET_ALL && kind != SND_CMD_FADE_ALL) {
            h = (s32)db_lookupRegistered(DB_GET_ADDR, id, 0);
        }
        /* the original jump tables are 8-aligned within their object */
        __asm__(".rdata\n\t.word 0\n\t.text");
        switch (kind) {
        case SND_CMD_PLAY:
            akao_playSe(h, param1, param2, param3);
            return 0;
        case SND_CMD_STOP:
            akao_stopSe(h, param1);
            return 0;
        case SND_CMD_SET:
            switch (SND_PARAM(cmd)) {
            case SND_PARAM_VOLUME:
                akao_setSeVolume(h, param1, param2);
                return 0;
            case SND_PARAM_PAN:
                akao_sendCmdA2(h, param1, param2);
                return 0;
            case SND_PARAM_PITCH:
                akao_sendCmdA4(h, param1, param2);
                return 0;
            }
            return -1;
        case SND_CMD_FADE:
            switch (SND_PARAM(cmd)) {
            case SND_PARAM_VOLUME:
                akao_fadeSeVolume(h, param1, param2, param3);
                return 0;
            case SND_PARAM_PAN:
                akao_sendCmdA3(h, param1, param2, param3);
                return 0;
            case SND_PARAM_PITCH:
                akao_sendCmdA5(h, param1, param2, param3);
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
            case SND_PARAM_PITCH:
                akao_sendCmdAC(param1);
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
            case SND_PARAM_PITCH:
                akao_sendCmdAD(param1, param2);
                return -1;
            }
            return -1;
        }
        return -1;
    case SND_TYPE_BANK:
        __asm__(".rdata\n\t.word 0\n\t.text");
        id = (soundId & 0xFFFF) | DB_SOUND_BANK;
        switch (kind) {
        case SND_CMD_LOAD:
            if (param1 != 0) {
                if (g_gameState->gameData->worldBankReq == (soundId & 0xFFFF)) {
                    return 0;
                }
                g_gameState->gameData->worldBankReq = soundId;
            }
            g_worldMusicReqKind = kind;
            g_worldMusicReqId = id;
            g_gameState->gameData->soundLoadState = SND_LOAD_WAIT_FADE;
            return g_gameState->gameData->soundLoadState;
        case SND_CMD_GET_STATE:
            goto state;
        }
        return -1;
    }
    return -1;

state:
    switch (g_gameState->gameData->soundLoadState) {
    case SND_LOAD_WAIT_FADE:
        if (wm_isMusicFadingOut() == 0) {
            if (g_gameState->gameData->flags & GD_MUSIC_STOP_PENDING) {
                akao_stopMusic(g_gameState->gameData->musicHandle[0]);
                akao_stopMusic(g_gameState->gameData->musicHandle[1]);
                g_gameState->gameData->flags &= ~GD_MUSIC_STOP_PENDING;
            }
            g_gameState->gameData->soundLoadState = SND_LOAD_START;
        }
        break;
    case SND_LOAD_START:
        if (((g_worldMusicReqId >> 16) & 0xFF) == (DB_MUSIC >> 16)) {
            g_gameState->gameData->soundLoadBank = g_gameState->gameData->musicBank[0];
        } else {
            g_gameState->gameData->soundLoadBank = g_worldMusicReqId;
        }
        g_worldMusicSector = wm_findSoundFile(g_worldMusicReqId, &sz);
        akao_beginBankUpload();
        if (!(g_gameState->gameData->ctx.world->flags & WM_MAP_NO_TRANSITION)) {
            g_gameState->gameData->soundLoadDest = (s32)(g_loadAreaEnd + 0x35000);
            g_gameState->gameData->soundLoadChunk = -1;
        } else if (g_gameState->gameData->ctx.world->flags & WM_MAP_LOADING) {
            g_gameState->gameData->soundLoadDest = (s32)(g_loadAreaEnd + 0x91800);
            g_gameState->gameData->soundLoadChunk = 0x48000;
        } else {
            g_gameState->gameData->soundLoadDest = wm_getModelCacheBottom();
            g_gameState->gameData->soundLoadChunk = 0x10000;
        }
        g_gameState->gameData->soundBankSize = 0;
        cdRead_init(g_gameState->gameData->cdRead, g_worldMusicSector, sz, (void *)g_gameState->gameData->soundLoadDest, 0,
                      g_gameState->gameData->soundLoadChunk, wm_streamSoundChunk);
        g_gameState->gameData->soundLoadState = SND_LOAD_BANK;
        break;
    case SND_LOAD_BANK:
        if (cdRead_update(g_gameState->gameData->cdRead) == 0 && akao_getSpuTransferState() == 0) {
            if (g_gameState->gameData->cdRead[0].offset + g_gameState->gameData->cdRead[0].chunk < g_gameState->gameData->soundBankSize) {
                g_gameState->gameData->soundLoadDest = (s32)g_gameState->gameData->cdRead[0].dest;
                room = 0x10000 + wm_getModelCacheBottom() - g_gameState->gameData->soundLoadDest;
                room = room / 2048 * 2048;
                cdRead_init(g_gameState->gameData->cdRead, g_worldMusicSector, g_gameState->gameData->cdRead[0].size, (void *)g_gameState->gameData->soundLoadDest,
                              g_gameState->gameData->cdRead[0].offset + g_gameState->gameData->cdRead[0].chunk, room, wm_streamSoundChunk);
            } else if (g_worldMusicReqIdKind == (DB_MUSIC >> 16)) {
                if (g_worldMusicReqKind != SND_CMD_RESUME) {
                    g_gameState->gameData->musicVolume[0] = 0x7F;
                }
                g_gameState->gameData->soundLoadState = SND_LOAD_SEQ_START;
            } else {
                g_worldMusicReqKind = -1;
                g_gameState->gameData->soundLoadState = SND_LOAD_IDLE;
            }
        }
        break;
    case SND_LOAD_SEQ_START:
        seqBytes = g_gameState->gameData->cdRead[0].offset + g_gameState->gameData->cdRead[0].chunk - g_gameState->gameData->soundBankSize;
        if (seqBytes > 0) {
            memcpy(g_gameState->gameData->musicSeq, (u8 *)g_gameState->gameData->soundLoadDest + (g_gameState->gameData->soundBankSize - g_gameState->gameData->cdRead[0].offset), seqBytes);
        }
        readLen = 0x6800 - seqBytes;
        g_gameState->gameData->soundLoadDest = (s32)((u8 *)g_gameState->gameData->musicSeq + seqBytes);
        readLen = readLen / 2048 * 2048;
        cdRead_init(g_gameState->gameData->cdRead, g_worldMusicSector, g_gameState->gameData->cdRead[0].size, (void *)g_gameState->gameData->soundLoadDest,
                      g_gameState->gameData->cdRead[0].offset + g_gameState->gameData->cdRead[0].chunk, readLen, NULL);
        g_gameState->gameData->soundLoadState = SND_LOAD_SEQ;
        break;
    case SND_LOAD_SEQ:
        if (cdRead_update(g_gameState->gameData->cdRead) == 0) {
            if (g_gameState->gameData->cdRead[0].offset + g_gameState->gameData->cdRead[0].chunk < g_gameState->gameData->cdRead[0].size) {
                seqOffset = g_gameState->gameData->cdRead[0].offset + g_gameState->gameData->cdRead[0].chunk - g_gameState->gameData->soundBankSize;
                g_gameState->gameData->soundLoadDest = (s32)((u8 *)g_gameState->gameData->musicSeq + seqOffset);
                cdRead_init(g_gameState->gameData->cdRead, g_worldMusicSector, g_gameState->gameData->cdRead[0].size, (void *)g_gameState->gameData->soundLoadDest, seqOffset, 0x6800 - seqOffset, NULL);
            } else {
                wm_unpackMusic((s32)g_gameState->gameData->musicSeq);
                g_gameState->gameData->flags |= GD_MUSIC_LOADED;
                g_gameState->gameData->soundLoadState = SND_LOAD_IDLE;
                if (g_worldMusicReqKind != SND_CMD_LOAD) {
                    goto start;
                }
                g_worldMusicReqKind = -1;
            }
        }
        break;
    }
    return g_gameState->gameData->soundLoadState;
}

void wm_calcSoundPanVolume(s16 posX, s16 posY, s16 posZ, s32 unused, u8 *outPan, u8 *outVolume) {
    SVECTOR pos;
    DVECTOR sxy;
    s32 z;
    s32 margin;
    s32 pan;
    s32 span;

    pos.vx = posX;
    pos.vy = posY;
    pos.vz = posZ;
    gte_SetRotMatrix(&g_gameState->gameData->viewMatrix);
    gte_SetTransMatrix(&g_gameState->gameData->viewMatrix);
    gte_ldv0(&pos);
    gte_rtps();
    gte_stsxy(&sxy);
    gte_stsz(&z);
    z /= 8;
    if (z < 0) {
        z = 0;
    } else if (z >= 0x1000) {
        z = 0xFFF;
    }
    margin = z / 32;
    span = (128 - margin) * 2;
    if ((u16)sxy.vx < 320) {
        pan = sxy.vx * span / 320;
        pan += margin;
    } else if (sxy.vx < 0) {
        if (sxy.vx < -320) {
            sxy.vx = -320;
        }
        pan = (sxy.vx + 320) * margin / 320;
    } else {
        if (sxy.vx >= 640) {
            sxy.vx = 639;
        }
        pan = (sxy.vx - 320) * margin / 320;
        pan += 255 - margin;
    }
    *outPan = pan;
    *outVolume = (0x1000 - z) * 127 / 4096;
}

void wm_setMusicVolumeFade(s32 volume, s32 frames) {
    GameData *gd = g_gameState->gameData;

    gd->musicVolume[0] = volume;
    if (volume == 0) {
        gd->musicFadeFrames = frames;
        gd->musicFadeStart = VSync(-1);
    }
}

s32 wm_isMusicFadingOut(void) {
    GameData *gd = g_gameState->gameData;

    return (u32)(VSync(-1) - gd->musicFadeStart) < gd->musicFadeFrames;
}

/* old-style definition: wm_updateBattleTransition passes a third (unused) argument */
s32 wm_findSoundFile(fileId, outSize)
s32 fileId;
s32 *outSize;
{
    FileEntry *depEntry;
    FileEntry *file;

    if (((fileId >> 16) & 0xFF) == (DB_MUSIC >> 16)) {
        depEntry = dep_find(g_gameState->gameData->depTable, fileId);
        if (outSize != NULL) {
            *outSize = depEntry->sectors << 11;
        }
        return depEntry->sector;
    } else {
        file = file_find(DIR_SOUND_BANKS, fileId);
        if (outSize != NULL) {
            *outSize = (file->link - file->sector) << 11;
        }
        return file->sector;
    }
}

s32 wm_depGetLink(s32 fileId) {
    FileTable *table = g_gameState->gameData->depTable;
    s32 i;
    FileEntry *files = table->entries;

    for (i = 0; i < table->count; i++) {
        if (files[i].id == fileId) {
            return files[i].link;
        }
    }
    return -1;
}

s32 wm_unpackMusic(s32 db) {
    s32 key = db_lookup(db, DB_GET_KEY_AT, DB_MUSIC, 0);

    memcpy((u8 *)db, (u8 *)db_lookup(db, DB_GET_ADDR, key, 0), db_lookup(db, DB_GET_SIZE, key, 0));
    return 0;
}

void wm_streamSoundChunk(WmStreamReq *req) {
    s32 bankAddr;
    s32 id;
    s32 packAddr;
    s32 chunk;
    s32 fileAddr;

    bankAddr = req->addr;
    id = g_gameState->gameData->soundLoadBank | DB_SOUND_BANK;
    if (req->offset == 0) {
        fileAddr = bankAddr;
        packAddr = db_lookup(bankAddr, DB_GET_AT, DB_PACK, 0);
        if (packAddr == -1) {
            bankAddr = db_lookup(bankAddr, DB_GET_ADDR, id, 0);
            g_gameState->gameData->soundBankSize = db_getSize(fileAddr);
        } else {
            bankAddr = db_lookup(packAddr, DB_GET_ADDR, id, 0);
            g_gameState->gameData->soundBankSize = db_lookup(fileAddr, DB_GET_AT, (DB_PACK | 1), 0) - g_gameState->gameData->soundLoadDest;
        }
    }
    chunk = g_gameState->gameData->soundBankSize - req->offset;
    if (req->size < chunk) {
        chunk = req->size;
    }
    akao_uploadBank(bankAddr, req->addr + chunk - bankAddr, 0);
    req->addr = bankAddr;
}

void wm_clearOT(void) {
    GameData *gd = g_gameState->gameData;

    ClearOTagR(gd->ot[gd->drawBuf] - OT_PAD, OT_LEN + 2 * OT_PAD);
}

void wm_endFrame(void) {
    GameState *gs;
    GameData *gd;
    s32 mapFlags;

    if ((g_padPortHeld[0] & PAD_SOFT_RESET) == PAD_SOFT_RESET || (g_padPortHeld[1] & PAD_SOFT_RESET) == PAD_SOFT_RESET) {
        g_gameState->flags |= GS_RESET;
    }
    DrawSync(0);
    gs = g_gameState;
    mapFlags = gs->gameData->ctx.world->flags;
    if (!(mapFlags & WM_MAP_NO_TRANSITION)) {
        gs->vsyncMode = 2;
    } else if ((mapFlags & WM_MAP_NAVI_SCREEN) || (gs->gameData->flags & GD_HIDE_ALL) == GD_HIDE_ALL) {
        gs->vsyncMode = 0;
    } else {
        gs->vsyncMode = 3;
    }
    VSync(g_gameState->vsyncMode);
    ResetGraph(1);
    PutDispEnv(&g_gameState->dispEnv[g_gameState->gameData->drawBuf]);
    gd = g_gameState->gameData;
    if (!(gd->flags & GD_PAUSED)) {
        if (gd->ctx.world->flags & WM_MAP_NO_TRANSITION) {
            DrawOTagEnv(gd->ot[gd->drawBuf] + OT_LAST, &g_gameState->drawEnv[gd->drawBuf]);
        } else {
            DrawOTag(gd->ot[gd->drawBuf]);
        }
    }
    if (!(g_gameState->gameData->flags & GD_NO_FLIP)) {
        g_gameState->gameData->drawBuf ^= 1;
        gfx_setBuffer(g_gameState->gameData->drawBuf);
    }
}

void wm_startBattleTransition(void) {
    SVECTOR pos;
    WorldMapObj *mapObj;
    EvtObject *player;
    WmObjSlot *objSlot;

    g_swirlFrame = 0;
    g_swirlBuf = wm_getChunkBuffer();
    mapObj = g_gameState->gameData->ctx.world->obj;
    mapObj->swirlFrames = 0;
    player = evt_getPlayer();
    if (player != NULL && (objSlot = (WmObjSlot *)game_getModel(player->modelSlot)) != NULL) {
        pos.vx = ((MATRIX *)objSlot->model->boneMatrices)[1].t[0];
        pos.vy = ((MATRIX *)objSlot->model->boneMatrices)[1].t[1];
        pos.vz = ((MATRIX *)objSlot->model->boneMatrices)[1].t[2];
        gte_SetRotMatrix(&g_gameState->gameData->viewMatrix);
        gte_SetTransMatrix(&g_gameState->gameData->viewMatrix);
        gte_ldv0(&pos);
        gte_rtps();
        gte_stsxy(&g_gameState->gameData->ctx.world->obj->swirlX);
        if ((u16)g_gameState->gameData->ctx.world->obj->swirlX >= 320 ||
            g_gameState->gameData->ctx.world->obj->swirlY < 0 ||
            g_gameState->gameData->ctx.world->obj->swirlY >= 218) {
            g_gameState->gameData->ctx.world->obj->swirlX = 160;
            g_gameState->gameData->ctx.world->obj->swirlY = 109;
        }
    }
    g_gameState->gameData->ctx.world->flags &= ~WM_MAP_NO_TRANSITION;
    wm_saveDepTable();
    wm_findBattleMusic();
    if (g_gameState->gameData->musicId[0] != g_gameState->gameData->ctx.world->obj->battleMusic &&
        (g_gameState->gameData->flags & GD_MUSIC_PLAYING)) {
        akao_fadeMusicVolume(0, 0xF, 0);
    }
    g_gameState->gameData->ctx.world->obj->battleStep = 5;
    wm_soundCommand(SND_SE_PLAY, SE_ENCOUNTER_1, 0, SND_PAN_CENTER, SND_VOL_MAX);
    wm_soundCommand(SND_SE_PLAY, SE_ENCOUNTER_2, 0, SND_PAN_CENTER, SND_VOL_MAX);
    wm_soundCommand(SND_SE_PLAY, SE_ENCOUNTER_3, 0, SND_PAN_CENTER, SND_VOL_MAX);
    g_gameState->gameData->flags |= (GD_MENU_DISABLED | GD_MENU_OPEN | GD_HIDE_MODELS | GD_HIDE_BG | GD_NO_FLIP);
    g_gameState->gameData->ctx.world->flags |= WM_MAP_NO_FADE | WM_MAP_NO_FRAME;
    g_gameState->vsyncMode = 0;
    g_padPortMask[0] |= BTN_START;
}

void wm_updateBattleTransition(void) {
    WorldMapObj *w;
    WorldMapObj *mapObj;
    GameState *gs;
    GameData *gd;
    s32 size;

    w = g_gameState->gameData->ctx.world->obj;
    if (!(w->flags & WM_OBJ_SWIRL_DONE)) {
        if (w->swirlFrames >= 2) {
            if ((u8)swirl_update(w->swirlX, w->swirlY)) {
                g_gameState->gameData->ctx.world->obj->flags |= WM_OBJ_SWIRL_DONE;
            }
        } else {
            w->swirlFrames++;
        }
    }
    gs = g_gameState;
    gd = gs->gameData;
    mapObj = gd->ctx.world->obj;
    if (mapObj->battleStep > 0) {
        mapObj->battleStep--;
    } else if (mapObj->battleStep == 0) {
        if (mapObj->battleMusic != MUSIC_NONE && mapObj->battleMusic != gd->musicId[0]) {
            cd_seekAsync(g_gameState->imgSector + wm_findSoundFile(mapObj->battleMusic | DB_MUSIC, &size, gs), 0);
            if (g_gameState->gameData->flags & GD_MUSIC_PLAYING) {
                wm_soundCommand(SND_MUSIC_SUSPEND, g_gameState->gameData->musicId[0], 0, 0, 0);
            }
            wm_soundCommand(SND_MUSIC_LOAD, g_gameState->gameData->ctx.world->obj->battleMusic, 0, 0, 0);
            g_gameState->gameData->ctx.world->obj->battleStep = WM_BTLSTEP_MUSIC;
        } else {
            cd_seekAsync(g_gameState->imgSector + file_find(DIR_OVERLAYS, OVL_KEY(OVL_BATTLE))->sector, 0);
            g_gameState->gameData->ctx.world->obj->battleStep = WM_BTLSTEP_SWIRL;
        }
    } else if (mapObj->battleStep == WM_BTLSTEP_MUSIC) {
        if (wm_soundCommand(SND_GET_STATE, 0, 0, 0, 0) == 0) {
            cd_seekAsync(g_gameState->imgSector + file_find(DIR_OVERLAYS, OVL_KEY(OVL_BATTLE))->sector, 0);
            wm_soundCommand(SND_MUSIC_PLAY, g_gameState->gameData->ctx.world->obj->battleMusic, 0, 0, 0);
            akao_setMusicVolume(0, 0x7F);
            g_gameState->gameData->ctx.world->obj->battleStep = WM_BTLSTEP_SWIRL;
        }
    } else if (mapObj->battleStep == WM_BTLSTEP_SWIRL && (mapObj->flags & WM_OBJ_SWIRL_DONE)) {
        gd->flags &= ~(GD_MENU_DISABLED | GD_MENU_OPEN | GD_HIDE_MODELS | GD_HIDE_BG | GD_NO_FLIP);
        g_gameState->gameData->ctx.world->flags &= ~(WM_MAP_NO_FADE | WM_MAP_NO_FRAME);
        g_padPortMask[0] &= ~BTN_START;
        g_gameState->flags |= GS_WM_LEAVE;
    }
}

void wm_findBattleMusic(void) {
    GameData *gd = g_gameState->gameData;
    s32 i = 0;
    WorldMapObj *obj = gd->ctx.world->obj;
    FileEntry *files;
    s32 id;
    s32 musicLink;

    obj->battleMusic = gd->musicId[0];
    id = (u16)obj->sceneId | DB_BATTLE;
    files = g_gameState->gameData->depTable->entries;
    for (; i < g_gameState->gameData->depTable->count; i++) {
        musicLink = files[i].link;
        if (files[i].id == id && ((musicLink >> 16) & 0xFF) == (DB_MUSIC >> 16)) {
            g_gameState->gameData->ctx.world->obj->battleMusic = musicLink;
            return;
        }
    }
}

void wm_togglePause(void) {
    u8 spriteW[8];
    s32 i;
    u32 link;

    g_gameState->gameData->flags ^= GD_PAUSED;
    if (g_gameState->gameData->flags & GD_PAUSED) {
        akao_pause(0);
        game_resumeClock(1);
        for (i = 1; i >= 0; i--) {
            g_padActuator[i].largeMotor = 0;
            g_padActuator[i].smallMotor = 0;
        }
        ClearOTag(&g_worldPauseOT, 1);
        ui_buildSprite((320 - g_uiSprites[235].w) / 2, (218 - g_uiSprites[235].h) / 2, &g_worldPauseOT, 0xEB, 0, spriteW, g_worldPauseSprite);
        setlen(&g_worldPauseTile, PRIM_LEN(TILE));
        g_worldPauseTile.code = GPU_TILE | GPU_ABE;
        setRGB0(&g_worldPauseTile, 0x60, 0x60, 0x60);
        setXY0(&g_worldPauseTile, 0, 0);
        setWH(&g_worldPauseTile, 320, 218);
        setlen(&g_worldPauseTpage, PRIM_LEN(DR_TPAGE));
        link = getaddr(&g_worldPauseOT);
        g_worldPauseTpage.code[0] = _get_mode(0, 1, getTPage(0, 2, 0, 0));
        setaddr(&g_worldPauseTile, link), setaddr(&g_worldPauseOT, &g_worldPauseTile);
        addPrim(&g_worldPauseOT, &g_worldPauseTpage);
        DrawOTag(&g_worldPauseOT);
        g_gameState->gameData->flags |= GD_NO_FLIP;
    } else {
        game_resumeClock(0);
        akao_resume(0);
        g_gameState->gameData->flags &= ~GD_NO_FLIP;
    }
}

s32 wm_preloadRequest(void) {
    return 0;
}

void wm_copyViaVram(s32 dst, s32 src, s32 size) {
    RECT rect;
    RECT *rp = NULL;

    if (!(g_gameState->gameData->ctx.world->flags & WM_MAP_NO_TRANSITION)) {
        rp = &rect;
        rect.x = 0x280;
        rect.w = 0x140;
        rect.y = 0;
        rect.h = 0xDA;
    }
    file_copyViaVram(dst, src, size, rp);
}

void wm_saveDepTable(void) {
    FileTable *table = g_gameState->gameData->depTable;
    u8 *dst;

    if (table != (FileTable *)-1) {
        dst = g_loadAreaEnd + 0x146F00;
        wm_copyViaVram((s32)dst, (s32)table, (table->count * 0x10) | 4);
        DrawSync(0);
        g_gameState->gameData->depTable = (FileTable *)dst;
    }
}
