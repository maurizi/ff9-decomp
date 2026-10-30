#include "ovl_14.h"

void opening_initNewGame(void) {
    game_newGame();
    evt_clearGlobalVars();
    gamedata_resetEventFlags();
    menu_initNewGame();
    func_800AB1E4();
    card_initAll();
    opening_resetGameData();
}

void opening_resetGameData(void) {
    opening_resetFieldState();
    evt_initSystem();
    func_800AB200();
    card_resetGameResult();
}

void opening_resetFieldState(void) {
    s32 i;
    GameData *ctx = g_gameState->gameData;

    ctx->unk9 = 0;
    ctx->npcModelSlots = 0;
    ctx->npcCount = 0;
    ctx->savedMusicId = 0xFFFF;
    ctx->savedMusicField = 0xFFFF;
    ctx->savedMusicBank = 0xFFFF;
    ctx->savedMusicVolume = 0;
    ctx->savedMusicFileSector = -1;
    ctx->savedMusicFileSectors = 0xFFFF;
    for (i = 1; i >= 0; i--) {
        ctx->sfxSlotSuspendField[i] = 0xFFFF;
    }
    for (i = 1; i >= 0; i--) {
        ctx->returnSector[i] = -1;
        ctx->returnSectors[i] = -1;
    }
}

void opening_init(void) {
    s32 i;

    for (i = 1; i >= 0; i--) {
        g_padActuator[i].unk1 = 0;
        g_padActuator[i].unk0 = 0;
    }
    akao_stopMusicNow();
    akao_stopAllSe();
    if (g_gameState->gameData->flags & 0x100000) {
        snd_init();
        opening_resetMusicSlots();
        g_gameState->gameData->flags &= ~0x100000;
    }
    cd_checkShellOpen();
    g_gameState->gameData->soundHook = opening_soundCmd;
    g_gameState->gameData->kanjiFont = 0;
    opening_loadData();
    opening_loadMusic();
    opening_freeData();
    game_initGfx();
    SetDispMask(1);
}

void opening_resetMusicSlots(void) {
    s32 i;
    GameData *ctx = g_gameState->gameData;

    for (i = 1; i >= 0; i--) {
        ctx->musicId[i] = 0xFFFF;
        ctx->musicBank[i] = 0xFFFF;
        ctx->musicHandle[i] = 0;
        ctx->musicVolume[i] = 0x7F;
    }
    ctx->flags &= 0xBFFFFF;
}

void opening_exit(void) {
    GameData *ctx;

    SetDispMask(0);
    ctx = g_gameState->gameData;
    if (ctx->flags & 0x20000000) {
        ctx->flags |= 0x400000;
    }
}

void opening_loadMusic(void) {
    s32 a;
    s32 b;

    a = (s32)db_lookupRegistered(4, TITLE_MUSIC, 0);
    memmove(g_gameState->gameData->musicSeq, (void *)a, (s32)db_lookupRegistered(5, TITLE_MUSIC, 0));
    a = (s32)db_lookupRegistered(4, TITLE_MUSIC_BANK, 0);
    b = (s32)db_lookupRegistered(5, TITLE_MUSIC_BANK, 0);
    akao_beginBankUpload();
    akao_uploadBank(a, b, 1);
}

void opening_loadData(void) {
    u8 *buf;
    FileEntry *e;

    buf = g_loadAreaEnd + 0x10000;
    e = file_find(1, 0x2717);
    cdRead_init(g_gameState->gameData->cdRead, e->unk4, (e->unkC - e->unk4) << 11, buf, 0, -1, 0);
    while (cdRead_update(g_gameState->gameData->cdRead) != 0) {
    }
    g_openingData = buf;
    db_register((s32)buf, 0);
}

void opening_freeData(void) {
    db_unregister(g_openingData);
    g_openingData = NULL;
}

s32 opening_soundCmd(s32 arg0, s32 arg1, s32 arg2, s32 arg3, s32 arg4) {
    GameData *g;
    s32 kind;
    s32 h;
    s32 id;

    kind = (arg0 >> 8) & 0xF;
    switch ((arg0 >> 12) & 3) {
    case 0:
        id = (arg1 & 0xFFFF) | 0x70000;
        switch (kind) {
        case 0:
            if (g_gameState->gameData->soundLoadState != 0) {
                return -1;
            }
            if (g_openingSoundPending == -1) {
                g_openingSoundPending = kind;
            }
            if (g_gameState->gameData->musicId[0] != arg1) {
                if (g_gameState->gameData->flags & 0x20000000) {
                    akao_stopMusic(g_gameState->gameData->musicHandle[0]);
                }
                g_gameState->gameData->flags &= 0xCFFFFFFF;
                g_gameState->gameData->musicBank[0] = 0x122;
                g_gameState->gameData->musicId[0] = arg1;
                g_gameState->gameData->musicVolume[0] = 0x7F;
            }
            g_gameState->gameData->musicHandle[0] = akao_playMusic(g_gameState->gameData->musicSeq);
            g = g_gameState->gameData;
            if (!(g->flags & 0x20000000)) {
                akao_fadeMusicVolumeFrom(g->musicHandle[0], 0xA, 0, g->musicVolume[0]);
            }
            g_openingSoundPending = -1;
            g_gameState->gameData->flags |= 0x30000000;
            return 0;
        case 1:
            if (g_gameState->gameData->musicId[0] == (u16)id) {
                akao_stopMusic(g_gameState->gameData->musicHandle[0]);
                g_gameState->gameData->musicVolume[0] = 0x7F;
                g_gameState->gameData->flags &= 0xDFFFFFFF;
            }
            return 0;
        case 2:
            if ((arg0 & 0x3F) != 1) {
                break;
            }
            akao_setMusicVolume(g_gameState->gameData->musicHandle[0], arg2);
            g_gameState->gameData->musicVolume[0] = arg2;
            return 0;
        case 3:
            if ((arg0 & 0x3F) != 1) {
                break;
            }
            akao_fadeMusicVolume(g_gameState->gameData->musicHandle[0], arg2, arg3);
            opening_setMusicVolume(arg3, arg2);
            return 0;
        case 4:
            if ((arg0 & 0x3F) != 1) {
                break;
            }
            akao_fadeMusicVolumeFrom(g_gameState->gameData->musicHandle[0], arg2, arg3, arg4);
            opening_setMusicVolume(arg4, arg2);
            return 0;
        case 6:
            if ((arg0 & 0x3F) != 1) {
                break;
            }
            akao_fadeMusicVolume(0, arg2, arg3);
            opening_setMusicVolume(arg3, arg2);
            return 0;
        case 11:
            if ((arg0 & 0x3F) != 1) {
                break;
            }
            akao_fadeMusicVolumeFrom(0, arg2, arg3, arg4);
            opening_setMusicVolume(arg4, arg2);
            return 0;
        }
        return -1;
    case 1:
        id = (arg1 & 0xFFFF) | 0x80000;
        h = 0;
        if (kind != 5 && kind != 6) {
            h = (s32)db_lookupRegistered(4, id, 0);
        }
        switch (kind) {
        case 0:
            akao_playSe(h, arg2, arg3, arg4);
            return 0;
        case 1:
            akao_stopSe(h, arg2);
            return 0;
        case 2:
            switch (arg0 & 0x3F) {
            case 1:
                akao_setSeVolume(h, arg2, arg3);
                return 0;
            case 5:
                akao_sendCmdA2(h, arg2, arg3);
                return 0;
            case 3:
                akao_sendCmdA4(h, arg2, arg3);
                return 0;
            }
            return -1;
        case 3:
            switch (arg0 & 0x3F) {
            case 1:
                akao_fadeSeVolume(h, arg2, arg3, arg4);
                return 0;
            case 5:
                akao_sendCmdA3(h, arg2, arg3, arg4);
                return 0;
            case 3:
                akao_sendCmdA5(h, arg2, arg3, arg4);
                return 0;
            }
            return -1;
        case 5:
            switch (arg0 & 0x3F) {
            case 1:
                akao_setAllSeVolume(arg2);
                return 0;
            case 5:
                akao_sendCmdAA(arg2);
                return 0;
            case 3:
                akao_sendCmdAC(arg2);
                return 0;
            }
            return -1;
        case 6:
            switch (arg0 & 0x3F) {
            case 1:
                akao_fadeAllSeVolume(arg2, arg3);
                return -1;
            case 5:
                akao_sendCmdAB(arg2, arg3);
                return -1;
            case 3:
                akao_sendCmdAD(arg2, arg3);
                return -1;
            }
            return -1;
        }
        return -1;
    }
    return -1;
}

void opening_setMusicVolume(s32 arg0, s32 arg1) {
    GameData *ctx = g_gameState->gameData;

    ctx->musicVolume[0] = arg0;
    if (arg0 == 0) {
        ctx->musicFadeFrames = arg1;
        ctx->musicFadeStart = VSync(-1);
    }
}

s32 opening_isMusicFading(void) {
    GameData *ctx = g_gameState->gameData;
    return (u32)(VSync(-1) - ctx->musicFadeStart) < ctx->musicFadeFrames;
}
