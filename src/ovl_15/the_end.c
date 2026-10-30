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
                setlen(&g_theEndSprites[g_gameState->gameData->unk8][0], 4);
                g_theEndSprites[g_gameState->gameData->unk8][0].code = 0x64;
                g_theEndSprites[g_gameState->gameData->unk8][0].r0 = 0x80;
                g_theEndSprites[g_gameState->gameData->unk8][0].g0 = 0x80;
                g_theEndSprites[g_gameState->gameData->unk8][0].b0 = 0x80;
                g_theEndSprites[g_gameState->gameData->unk8][0].x0 = 0;
                g_theEndSprites[g_gameState->gameData->unk8][0].y0 = 0;
                g_theEndSprites[g_gameState->gameData->unk8][0].u0 = 0;
                g_theEndSprites[g_gameState->gameData->unk8][0].v0 = 0;
                g_theEndSprites[g_gameState->gameData->unk8][0].w = 0xC0;
                g_theEndSprites[g_gameState->gameData->unk8][0].h = 0xE0;
                g_theEndSprites[g_gameState->gameData->unk8][0].clut = 0x3800;
                addPrim(&g_gameState->gameData->unk10[g_gameState->gameData->unk8][SCREEN_OTZ], &g_theEndSprites[g_gameState->gameData->unk8][0]);
                setDrawTPage(&g_theEndTpages[g_gameState->gameData->unk8][0], 1, 1, 0x10A);
                addPrim(&g_gameState->gameData->unk10[g_gameState->gameData->unk8][SCREEN_OTZ], &g_theEndTpages[g_gameState->gameData->unk8][0]);
                setlen((g_theEndSprites[g_gameState->gameData->unk8] + 1), 4);
                (g_theEndSprites[g_gameState->gameData->unk8] + 1)->code = 0x64;
                (g_theEndSprites[g_gameState->gameData->unk8] + 1)->r0 = 0x80;
                (g_theEndSprites[g_gameState->gameData->unk8] + 1)->g0 = 0x80;
                (g_theEndSprites[g_gameState->gameData->unk8] + 1)->b0 = 0x80;
                (g_theEndSprites[g_gameState->gameData->unk8] + 1)->x0 = 0xC0;
                (g_theEndSprites[g_gameState->gameData->unk8] + 1)->y0 = 0;
                (g_theEndSprites[g_gameState->gameData->unk8] + 1)->u0 = 0;
                (g_theEndSprites[g_gameState->gameData->unk8] + 1)->v0 = 0;
                (g_theEndSprites[g_gameState->gameData->unk8] + 1)->w = 0x80;
                (g_theEndSprites[g_gameState->gameData->unk8] + 1)->h = 0xE0;
                (g_theEndSprites[g_gameState->gameData->unk8] + 1)->clut = 0x3800;
                addPrim(&g_gameState->gameData->unk10[g_gameState->gameData->unk8][SCREEN_OTZ], (g_theEndSprites[g_gameState->gameData->unk8] + 1));
                setDrawTPage(g_theEndTpages[g_gameState->gameData->unk8] + 1, 1, 1, 0x10D);
                addPrim(&g_gameState->gameData->unk10[g_gameState->gameData->unk8][SCREEN_OTZ], (g_theEndTpages[g_gameState->gameData->unk8] + 1));
            }
            break;
        case THEEND_ST_FADE:
            setlen(&g_theEndSprites[g_gameState->gameData->unk8][0], 4);
            g_theEndSprites[g_gameState->gameData->unk8][0].code = 0x64;
            g_theEndSprites[g_gameState->gameData->unk8][0].r0 = 0x80;
            g_theEndSprites[g_gameState->gameData->unk8][0].g0 = 0x80;
            g_theEndSprites[g_gameState->gameData->unk8][0].b0 = 0x80;
            g_theEndSprites[g_gameState->gameData->unk8][0].x0 = 0;
            g_theEndSprites[g_gameState->gameData->unk8][0].y0 = 0;
            g_theEndSprites[g_gameState->gameData->unk8][0].u0 = 0;
            g_theEndSprites[g_gameState->gameData->unk8][0].v0 = 0;
            g_theEndSprites[g_gameState->gameData->unk8][0].w = 0xC0;
            g_theEndSprites[g_gameState->gameData->unk8][0].h = 0xE0;
            g_theEndSprites[g_gameState->gameData->unk8][0].clut = 0x3800;
            addPrim(&g_gameState->gameData->unk10[g_gameState->gameData->unk8][SCREEN_OTZ], &g_theEndSprites[g_gameState->gameData->unk8][0]);
            setDrawTPage(&g_theEndTpages[g_gameState->gameData->unk8][0], 1, 1, 0x10A);
            addPrim(&g_gameState->gameData->unk10[g_gameState->gameData->unk8][SCREEN_OTZ], &g_theEndTpages[g_gameState->gameData->unk8][0]);
            setlen((g_theEndSprites[g_gameState->gameData->unk8] + 1), 4);
            (g_theEndSprites[g_gameState->gameData->unk8] + 1)->code = 0x64;
            (g_theEndSprites[g_gameState->gameData->unk8] + 1)->r0 = 0x80;
            (g_theEndSprites[g_gameState->gameData->unk8] + 1)->g0 = 0x80;
            (g_theEndSprites[g_gameState->gameData->unk8] + 1)->b0 = 0x80;
            (g_theEndSprites[g_gameState->gameData->unk8] + 1)->x0 = 0xC0;
            (g_theEndSprites[g_gameState->gameData->unk8] + 1)->y0 = 0;
            (g_theEndSprites[g_gameState->gameData->unk8] + 1)->u0 = 0;
            (g_theEndSprites[g_gameState->gameData->unk8] + 1)->v0 = 0;
            (g_theEndSprites[g_gameState->gameData->unk8] + 1)->w = 0x80;
            (g_theEndSprites[g_gameState->gameData->unk8] + 1)->h = 0xE0;
            (g_theEndSprites[g_gameState->gameData->unk8] + 1)->clut = 0x3800;
            addPrim(&g_gameState->gameData->unk10[g_gameState->gameData->unk8][SCREEN_OTZ], (g_theEndSprites[g_gameState->gameData->unk8] + 1));
            setDrawTPage(g_theEndTpages[g_gameState->gameData->unk8] + 1, 1, 1, 0x10D);
            addPrim(&g_gameState->gameData->unk10[g_gameState->gameData->unk8][SCREEN_OTZ], (g_theEndTpages[g_gameState->gameData->unk8] + 1));
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
        g_padActuator[0].unk0 = 0;
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
    s32 p = (s32)db_lookupRegistered(4, g_endingImage | 0x40000, 0);

    file_setTimPos(p, 0, 0xE0, 0x280, 0);
    file_loadTim(p);
    akao_stopMusic(0);
    ending_soundCmd(0, ENDING_MUSIC, 0, 0, 0);
    ending_soundCmd(0xC401, ENDING_MUSIC, 0x5A, 0, 0x7F);
    game_startFade(1, 1, 0, 0xFFFFFF);
    g_theEndState = THEEND_ST_START;
    D_800AE430 = 0;
    g_padPortMask[0] = 0;
    return 0;
}

s32 ending_theEndExit(void) {
    return 0;
}

void ending_theEndClearOt(void) {
    GameData *ctx = g_gameState->gameData;
    ClearOTagR(ctx->unk10[ctx->unk8], 0x1000);
}

void ending_theEndSwap(void) {
    GameData *ctx;
    u8 i;

    DrawSync(0);
    g_gameState->vsyncMode = 0;
    VSync(g_gameState->vsyncMode);
    ResetGraph(1);
    PutDispEnv(&g_gameState->dispEnv[g_gameState->gameData->unk8]);
    ctx = g_gameState->gameData;
    i = ctx->unk8;
    DrawOTagEnv(ctx->unk10[i] + 0xFFF, &g_gameState->drawEnv[i]);
    g_gameState->gameData->unk8 ^= 1;
    gfx_setBuffer(g_gameState->gameData->unk8);
}

s32 ending_soundCmd(s32 arg0, s32 arg1, s32 arg2, s32 arg3, s32 arg4) {
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
            if (g_endingSoundPending == -1) {
                g_endingSoundPending = kind;
            }
            if (g_gameState->gameData->musicId[0] != arg1) {
                if (g_gameState->gameData->unk0 & 0x20000000) {
                    akao_stopMusic(g_gameState->gameData->musicHandle[0]);
                }
                g_gameState->gameData->unk0 &= 0xCFFFFFFF;
                g_gameState->gameData->musicBank[0] = 0x1F3;
                g_gameState->gameData->musicId[0] = arg1;
                g_gameState->gameData->musicVolume[0] = 0x7F;
            }
            g_gameState->gameData->musicHandle[0] = akao_playMusic(g_gameState->gameData->musicSeq);
            g = g_gameState->gameData;
            if (!(g->unk0 & 0x20000000)) {
                akao_fadeMusicVolumeFrom(g->musicHandle[0], 0xA, 0, g->musicVolume[0]);
            }
            g_endingSoundPending = -1;
            g_gameState->gameData->unk0 |= 0x30000000;
            return 0;
        case 1:
            if (g_gameState->gameData->musicId[0] == (u16)id) {
                akao_stopMusic(g_gameState->gameData->musicHandle[0]);
                g_gameState->gameData->musicVolume[0] = 0x7F;
                g_gameState->gameData->unk0 &= 0xDFFFFFFF;
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

void ending_init(void) {
    s32 i;

    for (i = 1; i >= 0; i--) {
        g_padActuator[i].unk1 = 0;
        g_padActuator[i].unk0 = 0;
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
    if (g_gameState->gameData->unk0 & 0x20000000) {
        ending_soundCmd(0x8301, ENDING_MUSIC, 0x5A, 0, 0);
        for (i = 0x59; i >= 0; i--) {
            VSync(0);
        }
    }
    ending_soundCmd(0x100, ENDING_MUSIC, 0, 0, 0);
    db_unregister(g_endingData);
    g_endingData = NULL;
}

void ending_loadMusic(void) {
    s32 a;
    s32 b;

    akao_stopMusic(0);
    a = (s32)db_lookupRegistered(4, ENDING_MUSIC, 0);
    memmove(g_gameState->gameData->musicSeq, (void *)a, (s32)db_lookupRegistered(5, ENDING_MUSIC, 0));
    a = (s32)db_lookupRegistered(4, ENDING_MUSIC_BANK, 0);
    b = (s32)db_lookupRegistered(5, ENDING_MUSIC_BANK, 0);
    akao_beginBankUpload();
    akao_uploadBank(a, b, 1);
}

void ending_loadData(void) {
    u8 *buf;
    FileEntry *e;

    buf = g_loadAreaEnd + 0x10000;
    e = file_find(1, 0x2F44);
    cdRead_init(g_gameState->gameData->cdRead, e->unk4, (e->unkC - e->unk4) << 11, buf, 0, -1, 0);
    while (cdRead_update(g_gameState->gameData->cdRead) != 0) {
    }
    g_endingData = buf;
    db_register((s32)buf, 0);
}
