#include "ovl_0d.h"

/* This file's .data: linked right after .text (config/ovl_0d.yaml). */
s32 g_tmDataFile = 0;

void tm_main(void) {
    tm_init();
    tm_mainLoop();
    tm_exit();
}

void tm_init(void) {
    g_tmDataFile = 0;
    g_tmDataCopy = 0;
    g_gameState->gameData->soundHook = tm_soundCmd;
    preload_init();
    tm_loadDataFile();
    tm_loadSoundBlocks();
    tm_loadMusic();
    tm_soundCmd(SND_MUSIC_PLAY, MUSIC_TETRA_MASTER, 0, 0, 0);
    tm_copyDataFile();
    tm_freeDataFile();
}

void tm_exit(void) {
    tm_freeDataCopy();
    tm_soundCmd(SND_MUSIC_STOP, MUSIC_TETRA_MASTER, 0, 0, 0);
    g_gameState->prevMode = GS_MODE_TETRA_MASTER;
    g_gameState->mode = GS_MODE_FIELD;
}

s32 tm_loadSoundBlocks(void) {
    s32 i;
    s32 h;
    s32 a, b;
    for (i = db_lookup(g_tmDataFile, DB_GET_COUNT, DB_SOUND_BANK, 0) - 1; i >= 0; i--) {
        h = db_lookup(g_tmDataFile, DB_GET_KEY_AT, (i & 0xFFFF) | DB_SOUND_BANK, 0);
        a = (s32)db_lookupRegistered(DB_GET_ADDR, h, 0);
        b = (s32)db_lookupRegistered(DB_GET_SIZE, h, 0);
        akao_beginBankUpload();
        akao_uploadBank(a, b, 1);
    }
    return 0;
}

s32 tm_loadMusic(void) {
    s32 p = (s32)db_lookupRegistered(DB_GET_ADDR, 0x70042, 0);
    memmove(g_gameState->gameData->musicSeq, (void *)p, (s32)db_lookupRegistered(DB_GET_SIZE, 0x70042, 0));
    return 0;
}

s32 tm_loadDataFile(void) {
    s32 dst = (s32)(g_loadAreaEnd + 0x57800);
    FileEntry *f = file_find(DIR_COMMON, FILE_TM_DATA);
    cdRead_init(g_gameState->gameData->cdRead, f->sector, (f->link - f->sector) << 11, (void *)dst, 0, -1, 0);
    while (cdRead_update(g_gameState->gameData->cdRead) != 0) {
    }
    g_tmDataFile = dst;
    db_register(dst, 0);
    return 0;
}

s32 tm_copyDataFile(void) {
    s32 end = db_lookup(g_tmDataFile, DB_GET_BLOCK, DB_MUSIC, 0);
    g_tmDataCopy = (s32)(g_loadAreaEnd + 0x141000);
    memmove((void *)g_tmDataCopy, (void *)g_tmDataFile, end - g_tmDataFile);
    db_register(g_tmDataCopy, 0);
    return 0;
}

s32 tm_freeDataFile(void) {
    db_unregister((void *)g_tmDataFile);
    g_tmDataFile = 0;
    return 0;
}

s32 tm_freeDataCopy(void) {
    db_unregister((void *)g_tmDataCopy);
    g_tmDataCopy = 0;
    return 0;
}

s32 tm_soundCmd(s32 cmd, s32 soundId, s32 param1, s32 param2, s32 param3) {
    GameData *g;
    s32 kind;
    s32 h;
    s32 id;

    kind = (cmd >> 8) & 0xF;
    switch ((cmd >> 12) & 3) {
    case SND_TYPE_MUSIC:
        switch (kind) {
        case SND_CMD_PLAY:
            if (g_gameState->gameData->soundLoadState != 0) {
                return -1;
            }
            if (g_tmMusicRequest == -1) {
                g_tmMusicRequest = kind;
            }
            if (g_gameState->gameData->musicId[0] != soundId) {
                if (g_gameState->gameData->flags & GD_MUSIC_PLAYING) {
                    akao_stopMusic(g_gameState->gameData->musicHandle[0]);
                }
                g_gameState->gameData->flags &= ~(GD_MUSIC_LOADED | GD_MUSIC_PLAYING);
                g_gameState->gameData->musicBank[0] = 0xE2;
                g_gameState->gameData->musicId[0] = soundId;
                g_gameState->gameData->musicVolume[0] = 0x7F;
            }
            g_gameState->gameData->musicHandle[0] = akao_playMusic(g_gameState->gameData->musicSeq);
            g = g_gameState->gameData;
            if (!(g->flags & GD_MUSIC_PLAYING)) {
                akao_fadeMusicVolumeFrom(g->musicHandle[0], 0xA, 0, g->musicVolume[0]);
            }
            g_tmMusicRequest = -1;
            g_gameState->gameData->flags |= GD_MUSIC_LOADED | GD_MUSIC_PLAYING;
            return 0;
        case SND_CMD_STOP:
            akao_stopMusic(g_gameState->gameData->musicHandle[0]);
            g_gameState->gameData->musicVolume[0] = 0x7F;
            g_gameState->gameData->flags &= ~GD_MUSIC_PLAYING;
            return 0;
        case SND_CMD_SET:
            if ((cmd & 0x3F) != SND_PARAM_VOLUME) {
                break;
            }
            akao_setMusicVolume(g_gameState->gameData->musicHandle[0], param1);
            g_gameState->gameData->musicVolume[0] = param1;
            return 0;
        case SND_CMD_FADE:
            if ((cmd & 0x3F) != SND_PARAM_VOLUME) {
                break;
            }
            akao_fadeMusicVolume(g_gameState->gameData->musicHandle[0], param1, param2);
            g_gameState->gameData->musicVolume[0] = param2;
            return 0;
        case SND_CMD_FADE_FROM:
            if ((cmd & 0x3F) != SND_PARAM_VOLUME) {
                break;
            }
            akao_fadeMusicVolumeFrom(g_gameState->gameData->musicHandle[0], param1, param2, param3);
            g_gameState->gameData->musicVolume[0] = param3;
            return 0;
        case SND_CMD_FADE_ALL:
            if ((cmd & 0x3F) != SND_PARAM_VOLUME) {
                break;
            }
            akao_fadeMusicVolume(0, param1, param2);
            g_gameState->gameData->musicVolume[0] = param2;
            return 0;
        case SND_CMD_FADE_ALL_FROM:
            if ((cmd & 0x3F) != SND_PARAM_VOLUME) {
                break;
            }
            akao_fadeMusicVolumeFrom(0, param1, param2, param3);
            g_gameState->gameData->musicVolume[0] = param3;
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
            switch (cmd & 0x3F) {
            case SND_PARAM_VOLUME:
                akao_setSeVolume(h, param1, param2);
                return 0;
            case SND_PARAM_PAN:
                akao_sendCmdA2(h, param1, param2);
                return 0;
            }
            return -1;
        case SND_CMD_FADE:
            switch (cmd & 0x3F) {
            case SND_PARAM_VOLUME:
                akao_fadeSeVolume(h, param1, param2, param3);
                return 0;
            case SND_PARAM_PAN:
                akao_sendCmdA3(h, param1, param2, param3);
                return 0;
            }
            return -1;
        case SND_CMD_SET_ALL:
            switch (cmd & 0x3F) {
            case SND_PARAM_VOLUME:
                akao_setAllSeVolume(param1);
                return 0;
            case SND_PARAM_PAN:
                akao_sendCmdAA(param1);
                return 0;
            }
            return -1;
        case SND_CMD_FADE_ALL:
            switch (cmd & 0x3F) {
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
    }
    return -1;
}
