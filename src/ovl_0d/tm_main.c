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
    tm_soundCmd(0, 0x42, 0, 0, 0);
    tm_copyDataFile();
    tm_freeDataFile();
}

void tm_exit(void) {
    tm_freeDataCopy();
    tm_soundCmd(0x100, 0x42, 0, 0, 0);
    g_gameState->unkA = 9;
    g_gameState->mode = 1;
}

s32 tm_loadSoundBlocks(void) {
    s32 i;
    s32 h;
    s32 a, b;
    for (i = db_lookup(g_tmDataFile, 1, 0x90000, 0) - 1; i >= 0; i--) {
        h = db_lookup(g_tmDataFile, 3, (i & 0xFFFF) | 0x90000, 0);
        a = (s32)db_lookupRegistered(4, h, 0);
        b = (s32)db_lookupRegistered(5, h, 0);
        akao_beginBankUpload();
        akao_uploadBank(a, b, 1);
    }
    return 0;
}

s32 tm_loadMusic(void) {
    s32 p = (s32)db_lookupRegistered(4, 0x70042, 0);
    memmove(g_gameState->gameData->musicSeq, (void *)p, (s32)db_lookupRegistered(5, 0x70042, 0));
    return 0;
}

s32 tm_loadDataFile(void) {
    s32 dst = (s32)(g_loadAreaEnd + 0x57800);
    FileEntry *f = file_find(1, 0x2715);
    cdRead_init(g_gameState->gameData->cdRead, f->sector, (f->unkC - f->sector) << 11, (void *)dst, 0, -1, 0);
    while (cdRead_update(g_gameState->gameData->cdRead) != 0) {
    }
    g_tmDataFile = dst;
    db_register(dst, 0);
    return 0;
}

s32 tm_copyDataFile(void) {
    s32 end = db_lookup(g_tmDataFile, 0, 0x70000, 0);
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

s32 tm_soundCmd(s32 arg0, s32 arg1, s32 arg2, s32 arg3, s32 arg4) {
    GameData *g;
    s32 kind;
    s32 h;
    s32 id;

    kind = (arg0 >> 8) & 0xF;
    switch ((arg0 >> 12) & 3) {
    case 0:
        switch (kind) {
        case 0:
            if (g_gameState->gameData->soundLoadState != 0) {
                return -1;
            }
            if (D_800B3664 == -1) {
                D_800B3664 = kind;
            }
            if (g_gameState->gameData->musicId[0] != arg1) {
                if (g_gameState->gameData->flags & 0x20000000) {
                    akao_stopMusic(g_gameState->gameData->musicHandle[0]);
                }
                g_gameState->gameData->flags &= 0xCFFFFFFF;
                g_gameState->gameData->musicBank[0] = 0xE2;
                g_gameState->gameData->musicId[0] = arg1;
                g_gameState->gameData->musicVolume[0] = 0x7F;
            }
            g_gameState->gameData->musicHandle[0] = akao_playMusic(g_gameState->gameData->musicSeq);
            g = g_gameState->gameData;
            if (!(g->flags & 0x20000000)) {
                akao_fadeMusicVolumeFrom(g->musicHandle[0], 0xA, 0, g->musicVolume[0]);
            }
            D_800B3664 = -1;
            g_gameState->gameData->flags |= 0x30000000;
            return 0;
        case 1:
            akao_stopMusic(g_gameState->gameData->musicHandle[0]);
            g_gameState->gameData->musicVolume[0] = 0x7F;
            g_gameState->gameData->flags &= 0xDFFFFFFF;
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
            g_gameState->gameData->musicVolume[0] = arg3;
            return 0;
        case 4:
            if ((arg0 & 0x3F) != 1) {
                break;
            }
            akao_fadeMusicVolumeFrom(g_gameState->gameData->musicHandle[0], arg2, arg3, arg4);
            g_gameState->gameData->musicVolume[0] = arg4;
            return 0;
        case 6:
            if ((arg0 & 0x3F) != 1) {
                break;
            }
            akao_fadeMusicVolume(0, arg2, arg3);
            g_gameState->gameData->musicVolume[0] = arg3;
            return 0;
        case 11:
            if ((arg0 & 0x3F) != 1) {
                break;
            }
            akao_fadeMusicVolumeFrom(0, arg2, arg3, arg4);
            g_gameState->gameData->musicVolume[0] = arg4;
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
            }
            return -1;
        }
        return -1;
    }
    return -1;
}
