#include "ff9.h"

void game_initData(void) {
    GameData *gd;

    g_allocTop = g_gameState->allocTop;
    game_allocData();
    gd = g_gameState->gameData;
    game_resetData();
    PutDispEnv(&g_gameState->dispEnv[gd->unk8]);
    preload_init();
    game_loadCommonDb();
    game_loadCommonAkao();
    game_loadCommonImages();
    gd->allocTop = g_allocTop;
}

void game_quitData(void) {
    game_unloadCommonDb();
}

void game_allocData(void) {
    GameData *gd;
    u8 *q;
    s32 i;
    s32 w;

    g_gameState->gameData = (GameData *)g_allocTop;
    gd = g_gameState->gameData;
    q = (u8 *)(gd + 1);
    for (i = 0; i < 2; i++) {
        gd->unk10[i] = (u32 *)(q + 0x20);
        q += 0x4040;
    }
    gd->chrModels = (void *)(q + 0x558);
    gd->chrModelObjs = (void *)q;
    gd->packetBuf = (s32)(q + 0x60C);
    g_allocTop = gd->packetBuf + 0x8000;
    gd->preloadQueue = (void *)g_allocTop;
    g_allocTop += 0x120;
    gd->musicSeq = (void *)g_allocTop;
    w = g_allocTop + 0x6800;
    gd->musicSeq2 = w;
    gd->sfxSlotBuf0 = (void *)(w + 0x2000);
    gd->sfxSlotBuf1 = (void *)(w + 0x2800);
    g_allocTop = w + 0x3000;
}

void game_resetData(void) {
    GameData *gd = g_gameState->gameData;
    s32 i;

    gd->projDist = 0x200;
    gd->unk18.m[0][0] = gd->unk18.m[1][1] = gd->unk18.m[2][2] = 0x1000;
    gd->fieldAreaId = 1;
    gd->battleId = 0x235;
    gd->enemyGroup = -1;
    gd->worldId = 0x2331;
    gd->worldAreaId = 0x44;
    gd->unk0 = 0;
    gd->allocTop = 0;
    gd->unk8 = 0;
    gd->unk9 = 0;
    gd->npcCount = 0;
    gd->npcModelSlots = 0;
    gd->unk18.t[0] = gd->unk18.t[1] = gd->unk18.t[2] = 0;
    gd->unk18.m[0][1] = gd->unk18.m[0][2] = gd->unk18.m[1][0] = gd->unk18.m[1][2] = gd->unk18.m[2][0] =
        gd->unk18.m[2][1] = 0;
    gd->fieldId = 0;
    gd->font = NULL;
    gd->kanjiFont = NULL;
    gd->locationName = NULL;
    gd->unk810 = NULL;
    gd->chocograph = 0;
    gd->soundLoadState = 0;
    gd->musicFadeStart = 0;
    gd->soundLoadBank = 0xFFFF;
    for (i = 1; i >= 0; i--) {
        gd->musicId[i] = 0xFFFF;
        gd->musicBank[i] = 0xFFFF;
        gd->musicHandle[i] = 0;
        gd->musicVolume[i] = 0x7F;
    }
    gd->musicFadeFrames = 0x1E;
    gd->savedMusicId = 0xFFFF;
    gd->savedMusicBank = 0xFFFF;
    gd->savedMusicField = 0xFFFF;
    gd->savedMusicVolume = 0;
    gd->musicFileSector = -1;
    gd->musicFileSectors = 0xFFFF;
    gd->savedMusicFileSector = -1;
    gd->savedMusicFileSectors = 0xFFFF;
    for (i = 1; i >= 0; i--) {
        gd->sfxSlotId[i] = 0xFFFF;
        (&gd->worldBankReq)[i] = 0xFFFF;
        gd->sfxSlotVolume[i] = 0x7F;
        gd->sfxSlotPan[i] = 0x80;
        gd->sfxSlotSuspendField[i] = 0xFFFF;
    }
    gd->modelHook = NULL;
    gd->soundHook = NULL;
    for (i = 8; i >= 0; i--) {
        func_8001E3A4(&gd->chrModels[i]);
        gd->chrModels[i].unk8 = (struct BtlUnit9C *)&gd->chrModelObjs[i];
    }
    gd->npcModelObjs = NULL;
    gd->npcModels = NULL;
    gd->commonDb = NULL;
    for (i = 1; i >= 0; i--) {
        gd->returnSector[i] = -1;
        gd->returnSectors[i] = -1;
        gd->sceneSector[i] = -1;
        gd->sceneSectors[i] = -1;
    }
    gd->depTable = (struct FileTable *)-1;
    gd->soundLoadDest = 0;
    gd->soundLoadChunk = 0;
    gd->soundBankSize = 0;
    memset(&gd->worldCamera, 0, 0x28);
    gd->cardOpponent = 0;
    gd->preloadRequest = NULL;
    gd->unk8E4.ovl65 = NULL;
    for (i = 0; i >= 0; i--) {
        gd->unk849[i] = 0;
    }
    for (i = 7; i >= 0; i--) {
        gd->unk8CC[i] = 0;
    }
}

s32 game_loadCommonAkao(void) {
    s32 addr = (s32)db_lookupRegistered(4, DB_SOUND_BANK | 0x84, 0);
    s32 size = (s32)db_lookupRegistered(5, DB_SOUND_BANK | 0x84, 0);

    akao_beginBankUpload();
    akao_uploadBank(addr, size, 1);
    return 0;
}

s32 game_loadCommonImages(void) {
    GameData *gd = g_gameState->gameData;
    u16 ids[8] = { 0, 0x2710, 0x2711, 0x2712, 0x2713, 0x2714, 0x2715, 0x2716 };
    u32 i;
    s32 addr;

    gd->font = db_lookupRegistered(4, DB_FONT, 0);
    for (i = 0; i < 8; i++) {
        addr = (s32)db_lookupRegistered(4, ids[i] | DB_TIM, 0);
        if (addr != 0) {
            file_loadTim(addr);
            DrawSync(0);
        }
    }
    return 0;
}

s32 game_loadCommonDb(void) {
    FileEntry *f = file_find(1, 0x271A);

    cdRead_init(g_gameState->gameData->cdRead, f->unk4, (f->unkC - f->unk4) << 11, (void *)g_allocTop, 0,
                  -1, NULL);
    while (cdRead_update(g_gameState->gameData->cdRead)) {
    }
    g_gameState->gameData->commonDb = (void *)g_allocTop;
    db_register((s32)g_gameState->gameData->commonDb, 0);
    g_allocTop = db_lookup((s32)g_gameState->gameData->commonDb, 0, DB_SOUND_BANK, 0);
    return 0;
}

s32 game_unloadCommonDb(void) {
    db_unregister(g_gameState->gameData->commonDb);
    g_gameState->gameData->commonDb = 0;
    return 0;
}

