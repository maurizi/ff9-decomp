#include "ovl_15.h"

s32 ending_theEnd(void) {
    s32 timer;
    s32 wait;
    s32 n;

    timer = -1;
    wait = 0;
    ending_theEndInit();
    g_endingCodePos = 0;
    do {
        ending_theEndClearOt();
        pad_update();
        switch (g_theEndState) {
        case THEEND_ST_START:
            timer = -1;
            g_theEndState = THEEND_ST_SHOW;
            break;
        case THEEND_ST_SHOW:
            if (timer == -1) {
                game_startFade(1, 1, 0, 0xFFFFFF);
                timer = 0x21C;
                game_startFade(1, 1, 0xB4, 0);
            } else if (timer != 0) {
                setSprt(&g_theEndSprites[g_gameState->gameData->drawBuf][0]);
                g_theEndSprites[g_gameState->gameData->drawBuf][0].r0 = 0x80;
                g_theEndSprites[g_gameState->gameData->drawBuf][0].g0 = 0x80;
                g_theEndSprites[g_gameState->gameData->drawBuf][0].b0 = 0x80;
                g_theEndSprites[g_gameState->gameData->drawBuf][0].x0 = 0;
                g_theEndSprites[g_gameState->gameData->drawBuf][0].y0 = 0;
                g_theEndSprites[g_gameState->gameData->drawBuf][0].u0 = 0;
                g_theEndSprites[g_gameState->gameData->drawBuf][0].v0 = 0;
                g_theEndSprites[g_gameState->gameData->drawBuf][0].w = 0xC0;
                g_theEndSprites[g_gameState->gameData->drawBuf][0].h = 0xE0;
                g_theEndSprites[g_gameState->gameData->drawBuf][0].clut = 0x3800;
                addPrim(&g_gameState->gameData->ot[g_gameState->gameData->drawBuf][SCREEN_OTZ], &g_theEndSprites[g_gameState->gameData->drawBuf][0]);
                setDrawTPage(&g_theEndTpages[g_gameState->gameData->drawBuf][0], 1, 1, 0x10A);
                addPrim(&g_gameState->gameData->ot[g_gameState->gameData->drawBuf][SCREEN_OTZ], &g_theEndTpages[g_gameState->gameData->drawBuf][0]);
                setlen((g_theEndSprites[g_gameState->gameData->drawBuf] + 1), PRIM_LEN(SPRT));
                (g_theEndSprites[g_gameState->gameData->drawBuf] + 1)->code = GPU_SPRT;
                (g_theEndSprites[g_gameState->gameData->drawBuf] + 1)->r0 = 0x80;
                (g_theEndSprites[g_gameState->gameData->drawBuf] + 1)->g0 = 0x80;
                (g_theEndSprites[g_gameState->gameData->drawBuf] + 1)->b0 = 0x80;
                (g_theEndSprites[g_gameState->gameData->drawBuf] + 1)->x0 = 0xC0;
                (g_theEndSprites[g_gameState->gameData->drawBuf] + 1)->y0 = 0;
                (g_theEndSprites[g_gameState->gameData->drawBuf] + 1)->u0 = 0;
                (g_theEndSprites[g_gameState->gameData->drawBuf] + 1)->v0 = 0;
                (g_theEndSprites[g_gameState->gameData->drawBuf] + 1)->w = 0x80;
                (g_theEndSprites[g_gameState->gameData->drawBuf] + 1)->h = 0xE0;
                (g_theEndSprites[g_gameState->gameData->drawBuf] + 1)->clut = 0x3800;
                addPrim(&g_gameState->gameData->ot[g_gameState->gameData->drawBuf][SCREEN_OTZ], (g_theEndSprites[g_gameState->gameData->drawBuf] + 1));
                setDrawTPage(g_theEndTpages[g_gameState->gameData->drawBuf] + 1, 1, 1, 0x10D);
                addPrim(&g_gameState->gameData->ot[g_gameState->gameData->drawBuf][SCREEN_OTZ], (g_theEndTpages[g_gameState->gameData->drawBuf] + 1));
            }
            break;
        case THEEND_ST_FADE:
            setSprt(&g_theEndSprites[g_gameState->gameData->drawBuf][0]);
            g_theEndSprites[g_gameState->gameData->drawBuf][0].r0 = 0x80;
            g_theEndSprites[g_gameState->gameData->drawBuf][0].g0 = 0x80;
            g_theEndSprites[g_gameState->gameData->drawBuf][0].b0 = 0x80;
            g_theEndSprites[g_gameState->gameData->drawBuf][0].x0 = 0;
            g_theEndSprites[g_gameState->gameData->drawBuf][0].y0 = 0;
            g_theEndSprites[g_gameState->gameData->drawBuf][0].u0 = 0;
            g_theEndSprites[g_gameState->gameData->drawBuf][0].v0 = 0;
            g_theEndSprites[g_gameState->gameData->drawBuf][0].w = 0xC0;
            g_theEndSprites[g_gameState->gameData->drawBuf][0].h = 0xE0;
            g_theEndSprites[g_gameState->gameData->drawBuf][0].clut = 0x3800;
            addPrim(&g_gameState->gameData->ot[g_gameState->gameData->drawBuf][SCREEN_OTZ], &g_theEndSprites[g_gameState->gameData->drawBuf][0]);
            setDrawTPage(&g_theEndTpages[g_gameState->gameData->drawBuf][0], 1, 1, 0x10A);
            addPrim(&g_gameState->gameData->ot[g_gameState->gameData->drawBuf][SCREEN_OTZ], &g_theEndTpages[g_gameState->gameData->drawBuf][0]);
            setlen((g_theEndSprites[g_gameState->gameData->drawBuf] + 1), PRIM_LEN(SPRT));
            (g_theEndSprites[g_gameState->gameData->drawBuf] + 1)->code = GPU_SPRT;
            (g_theEndSprites[g_gameState->gameData->drawBuf] + 1)->r0 = 0x80;
            (g_theEndSprites[g_gameState->gameData->drawBuf] + 1)->g0 = 0x80;
            (g_theEndSprites[g_gameState->gameData->drawBuf] + 1)->b0 = 0x80;
            (g_theEndSprites[g_gameState->gameData->drawBuf] + 1)->x0 = 0xC0;
            (g_theEndSprites[g_gameState->gameData->drawBuf] + 1)->y0 = 0;
            (g_theEndSprites[g_gameState->gameData->drawBuf] + 1)->u0 = 0;
            (g_theEndSprites[g_gameState->gameData->drawBuf] + 1)->v0 = 0;
            (g_theEndSprites[g_gameState->gameData->drawBuf] + 1)->w = 0x80;
            (g_theEndSprites[g_gameState->gameData->drawBuf] + 1)->h = 0xE0;
            (g_theEndSprites[g_gameState->gameData->drawBuf] + 1)->clut = 0x3800;
            addPrim(&g_gameState->gameData->ot[g_gameState->gameData->drawBuf][SCREEN_OTZ], (g_theEndSprites[g_gameState->gameData->drawBuf] + 1));
            setDrawTPage(g_theEndTpages[g_gameState->gameData->drawBuf] + 1, 1, 1, 0x10D);
            addPrim(&g_gameState->gameData->ot[g_gameState->gameData->drawBuf][SCREEN_OTZ], (g_theEndTpages[g_gameState->gameData->drawBuf] + 1));
            if (timer == -1) {
                timer = 0x5A;
                game_startFade(1, 1, timer, 0xFFFFFF);
            } else if (--timer == 0) {
                g_theEndState = THEEND_ST_BLACKJACK;
            }
            break;
        }
        if (g_endingCodePos == ENDING_CODE_LEN) {
            if (g_padPortHeld[0] & PAD_START) {
                timer = -1;
                g_theEndState = THEEND_ST_FADE;
                snd_playSe(SE_GAME_START);
                g_padPortMask[0] = -1;
            }
        } else if (g_padPortPressed[0] & g_endingSecretCode[g_endingCodePos]) {
            if (++g_endingCodePos == ENDING_CODE_LEN) {
                snd_playSe(SE_CURSOR);
            }
        } else if (g_padPortPressed[0] != 0) {
            wait = 5;
            g_endingCodePos = 0;
        }
        g_padActuator[0].smallMotor = 0;
        if (wait > 0) {
            wait--;
        }
        game_updateFade();
        ending_theEndSwap();
    } while ((u32)g_theEndState < THEEND_ST_UNUSED);
    ending_theEndExit();
    return g_theEndState != THEEND_ST_UNUSED;
}

s32 ending_theEndInit(void) {
    s32 p = (s32)db_lookupRegistered(DB_GET_ADDR, g_endingImage | DB_TIM, 0);

    file_setTimPos(p, VRAM_IMAGE_CLUT_X, VRAM_IMAGE_CLUT_Y, VRAM_IMAGE_X, VRAM_IMAGE_Y);
    file_loadTim(p);
    akao_stopMusic(0);
    ending_soundCmd(SND_MUSIC_PLAY, ENDING_MUSIC, 0, 0, 0);
    ending_soundCmd(SND_SONG_VOL_FROM_INTPL, ENDING_MUSIC, 0x5A, 0, SND_VOL_MAX);
    game_startFade(1, 1, 0, 0xFFFFFF);
    g_theEndState = THEEND_ST_START;
    g_theEndInitWord = 0;
    g_padPortMask[0] = 0;
    return 0;
}

s32 ending_theEndExit(void) {
    return 0;
}

void ending_theEndClearOt(void) {
    GameData *ctx = g_gameState->gameData;
    ClearOTagR(ctx->ot[ctx->drawBuf], OT_LEN);
}

void ending_theEndSwap(void) {
    GameData *ctx;
    u8 i;

    DrawSync(0);
    g_gameState->vsyncMode = 0;
    VSync(g_gameState->vsyncMode);
    ResetGraph(1);
    PutDispEnv(&g_gameState->dispEnv[g_gameState->gameData->drawBuf]);
    ctx = g_gameState->gameData;
    i = ctx->drawBuf;
    DrawOTagEnv(ctx->ot[i] + OT_LAST, &g_gameState->drawEnv[i]);
    g_gameState->gameData->drawBuf ^= 1;
    gfx_setBuffer(g_gameState->gameData->drawBuf);
}

s32 ending_soundCmd(s32 cmd, s32 soundId, s32 param1, s32 param2, s32 param3) {
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
            if (g_gameState->gameData->soundLoadState != SND_LOAD_IDLE) {
                return -1;
            }
            if (g_endingSoundPending == -1) {
                g_endingSoundPending = kind;
            }
            if (g_gameState->gameData->musicId[0] != soundId) {
                if (g_gameState->gameData->flags & GD_MUSIC_PLAYING) {
                    akao_stopMusic(g_gameState->gameData->musicHandle[0]);
                }
                g_gameState->gameData->flags &= ~(GD_MUSIC_LOADED | GD_MUSIC_PLAYING);
                g_gameState->gameData->musicBank[0] = 0x1F3;
                g_gameState->gameData->musicId[0] = soundId;
                g_gameState->gameData->musicVolume[0] = 0x7F;
            }
            g_gameState->gameData->musicHandle[0] = akao_playMusic(g_gameState->gameData->musicSeq);
            g = g_gameState->gameData;
            if (!(g->flags & GD_MUSIC_PLAYING)) {
                akao_fadeMusicVolumeFrom(g->musicHandle[0], 0xA, 0, g->musicVolume[0]);
            }
            g_endingSoundPending = -1;
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
            g_gameState->gameData->musicVolume[0] = param2;
            return 0;
        case SND_CMD_FADE_FROM:
            if (SND_PARAM(cmd) != SND_PARAM_VOLUME) {
                break;
            }
            akao_fadeMusicVolumeFrom(g_gameState->gameData->musicHandle[0], param1, param2, param3);
            g_gameState->gameData->musicVolume[0] = param3;
            return 0;
        case SND_CMD_FADE_ALL:
            if (SND_PARAM(cmd) != SND_PARAM_VOLUME) {
                break;
            }
            akao_fadeMusicVolume(0, param1, param2);
            g_gameState->gameData->musicVolume[0] = param2;
            return 0;
        case SND_CMD_FADE_ALL_FROM:
            if (SND_PARAM(cmd) != SND_PARAM_VOLUME) {
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

void ending_init(void) {
    s32 i;

    for (i = 1; i >= 0; i--) {
        g_padActuator[i].largeMotor = 0;
        g_padActuator[i].smallMotor = 0;
    }
    g_gameState->gameData->soundHook = ending_soundCmd;
    ending_loadData();
    ending_loadMusic();
    game_initGfx();
    SetDispMask(1);
}

void ending_exit(void) {
    s32 i;

    SetDispMask(0);
    if (g_gameState->gameData->flags & GD_MUSIC_PLAYING) {
        ending_soundCmd(SND_SONG_VOL_INTPL, ENDING_MUSIC, 0x5A, 0, 0);
        for (i = 0x59; i >= 0; i--) {
            VSync(0);
        }
    }
    ending_soundCmd(SND_MUSIC_STOP, ENDING_MUSIC, 0, 0, 0);
    db_unregister(g_endingData);
    g_endingData = NULL;
}

void ending_loadMusic(void) {
    s32 a;
    s32 b;

    akao_stopMusic(0);
    a = (s32)db_lookupRegistered(DB_GET_ADDR, ENDING_MUSIC, 0);
    memmove(g_gameState->gameData->musicSeq, (void *)a, (s32)db_lookupRegistered(DB_GET_SIZE, ENDING_MUSIC, 0));
    a = (s32)db_lookupRegistered(DB_GET_ADDR, ENDING_MUSIC_BANK, 0);
    b = (s32)db_lookupRegistered(DB_GET_SIZE, ENDING_MUSIC_BANK, 0);
    akao_beginBankUpload();
    akao_uploadBank(a, b, 1);
}

void ending_loadData(void) {
    u8 *data;
    FileEntry *e;

    data = g_loadAreaEnd + 0x10000;
    e = file_find(DIR_COMMON, FILE_ENDING_DATA);
    cdRead_init(g_gameState->gameData->cdRead, e->sector, (e->link - e->sector) << 11, data, 0, -1, 0);
    while (cdRead_update(g_gameState->gameData->cdRead) != 0) {
    }
    g_endingData = data;
    db_register((s32)data, 0);
}
