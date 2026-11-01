#include "ovl_14.h"

void opening_initNewGame(void) {
    game_newGame();
    evt_clearGlobalVars();
    gamedata_resetEventFlags();
    menu_initNewGame();
    opening_setWorldCamKeepNew();
    card_initAll();
    opening_resetGameData();
}

void opening_resetGameData(void) {
    opening_resetFieldState();
    evt_initSystem();
    opening_setWorldCamKeepLoad();
    card_resetGameResult();
}

void opening_resetFieldState(void) {
    s32 i;
    GameData *ctx = g_gameState->gameData;

    ctx->unk9 = 0;
    ctx->npcModelSlots = 0;
    ctx->npcCount = 0;
    ctx->savedMusicId = MUSIC_NONE;
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
        g_padActuator[i].largeMotor = 0;
        g_padActuator[i].smallMotor = 0;
    }
    akao_stopMusicNow();
    akao_stopAllSe();
    if (g_gameState->gameData->flags & GD_SOUND_REINIT) {
        snd_init();
        opening_resetMusicSlots();
        g_gameState->gameData->flags &= ~GD_SOUND_REINIT;
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
        ctx->musicId[i] = MUSIC_NONE;
        ctx->musicBank[i] = 0xFFFF;
        ctx->musicHandle[i] = 0;
        ctx->musicVolume[i] = 0x7F;
    }
    ctx->flags &= ~(GD_MUSIC_LEFT_PLAYING | GD_SFX0_LOADED | GD_SFX0_PLAYING | GD_SFX1_LOADED | GD_SFX1_PLAYING | GD_MUSIC_LOADED | GD_MUSIC_PLAYING | GD_MUSIC2_LOADED | GD_MUSIC2_PLAYING);
}

void opening_exit(void) {
    GameData *ctx;

    SetDispMask(0);
    ctx = g_gameState->gameData;
    if (ctx->flags & GD_MUSIC_PLAYING) {
        ctx->flags |= GD_MUSIC_LEFT_PLAYING;
    }
}

void opening_loadMusic(void) {
    s32 a;
    s32 b;

    a = (s32)db_lookupRegistered(DB_GET_ADDR, TITLE_MUSIC, 0);
    memmove(g_gameState->gameData->musicSeq, (void *)a, (s32)db_lookupRegistered(DB_GET_SIZE, TITLE_MUSIC, 0));
    a = (s32)db_lookupRegistered(DB_GET_ADDR, TITLE_MUSIC_BANK, 0);
    b = (s32)db_lookupRegistered(DB_GET_SIZE, TITLE_MUSIC_BANK, 0);
    akao_beginBankUpload();
    akao_uploadBank(a, b, 1);
}

void opening_loadData(void) {
    u8 *data;
    FileEntry *e;

    data = g_loadAreaEnd + 0x10000;
    e = file_find(DIR_COMMON, FILE_OPENING_DATA);
    cdRead_init(g_gameState->gameData->cdRead, e->sector, (e->link - e->sector) << 11, data, 0, -1, 0);
    while (cdRead_update(g_gameState->gameData->cdRead) != 0) {
    }
    g_openingData = data;
    db_register((s32)data, 0);
}

void opening_freeData(void) {
    db_unregister(g_openingData);
    g_openingData = NULL;
}

s32 opening_soundCmd(s32 cmd, s32 soundId, s32 param1, s32 param2, s32 param3) {
    GameData *g;
    s32 kind;
    s32 h;
    s32 id;

    kind = SND_CMD(cmd);
    switch (SND_TYPE(cmd)) {
    case SND_TYPE_MUSIC:
        id = (soundId & 0xFFFF) | DB_MUSIC;
        switch (kind) {
        case SND_CMD_PLAY:
            if (g_gameState->gameData->soundLoadState != 0) {
                return -1;
            }
            if (g_openingSoundPending == -1) {
                g_openingSoundPending = kind;
            }
            if (g_gameState->gameData->musicId[0] != soundId) {
                if (g_gameState->gameData->flags & GD_MUSIC_PLAYING) {
                    akao_stopMusic(g_gameState->gameData->musicHandle[0]);
                }
                g_gameState->gameData->flags &= ~(GD_MUSIC_LOADED | GD_MUSIC_PLAYING);
                g_gameState->gameData->musicBank[0] = 0x122;
                g_gameState->gameData->musicId[0] = soundId;
                g_gameState->gameData->musicVolume[0] = 0x7F;
            }
            g_gameState->gameData->musicHandle[0] = akao_playMusic(g_gameState->gameData->musicSeq);
            g = g_gameState->gameData;
            if (!(g->flags & GD_MUSIC_PLAYING)) {
                akao_fadeMusicVolumeFrom(g->musicHandle[0], 0xA, 0, g->musicVolume[0]);
            }
            g_openingSoundPending = -1;
            g_gameState->gameData->flags |= GD_MUSIC_LOADED | GD_MUSIC_PLAYING;
            return 0;
        case SND_CMD_STOP:
            if (g_gameState->gameData->musicId[0] == (u16)id) {
                akao_stopMusic(g_gameState->gameData->musicHandle[0]);
                g_gameState->gameData->musicVolume[0] = 0x7F;
                g_gameState->gameData->flags &= ~GD_MUSIC_PLAYING;
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
            opening_setMusicVolume(param2, param1);
            return 0;
        case SND_CMD_FADE_FROM:
            if (SND_PARAM(cmd) != SND_PARAM_VOLUME) {
                break;
            }
            akao_fadeMusicVolumeFrom(g_gameState->gameData->musicHandle[0], param1, param2, param3);
            opening_setMusicVolume(param3, param1);
            return 0;
        case SND_CMD_FADE_ALL:
            if (SND_PARAM(cmd) != SND_PARAM_VOLUME) {
                break;
            }
            akao_fadeMusicVolume(0, param1, param2);
            opening_setMusicVolume(param2, param1);
            return 0;
        case SND_CMD_FADE_ALL_FROM:
            if (SND_PARAM(cmd) != SND_PARAM_VOLUME) {
                break;
            }
            akao_fadeMusicVolumeFrom(0, param1, param2, param3);
            opening_setMusicVolume(param3, param1);
            return 0;
        }
        return -1;
    case SND_TYPE_SE:
        id = (soundId & 0xFFFF) | DB_SE;
        h = 0;
        if (kind != SND_CMD_SET_ALL && kind != SND_CMD_FADE_ALL) {
            h = (s32)db_lookupRegistered(DB_GET_ADDR, id, 0);
        }
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
    }
    return -1;
}

void opening_setMusicVolume(s32 volume, s32 frames) {
    GameData *ctx = g_gameState->gameData;

    ctx->musicVolume[0] = volume;
    if (volume == 0) {
        ctx->musicFadeFrames = frames;
        ctx->musicFadeStart = VSync(-1);
    }
}

s32 opening_isMusicFading(void) {
    GameData *ctx = g_gameState->gameData;
    return (u32)(VSync(-1) - ctx->musicFadeStart) < ctx->musicFadeFrames;
}
