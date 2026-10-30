#include "ovl_14.h"

s32 opening_movieThenTitle(void) {
    u8 *buf;
    s32 timer;
    s32 i;
    s32 r;
    s32 end;

    buf = g_loadAreaEnd + 0x106700;
    timer = -1;
    opening_movieInit();
    do {
        opening_movieClearOt();
        pad_update();
        switch (g_movieState) {
        case MOVIE_ST_START:
            if (g_gameState->disc != DISC_NONE && (g_gameState->disc & DISC_1)) {
                g_movieSkipped = 0;
                g_movieFlags = 1;
                g_movieState = MOVIE_ST_OPEN;
                fade_clear();
            } else {
                if (!(g_gameState->gameData->unk0 & 0x20000000)) {
                    opening_soundCmd(0, TITLE_MUSIC, 0, 0, 0);
                    opening_soundCmd(0xC401, TITLE_MUSIC, 0x5A, 0, 0x7F);
                }
                g_movieSkipped = 1;
                g_movieState = MOVIE_ST_END;
                g_movieFlags |= MOVIE_DONE;
                fade_out(1);
            }
            break;
        case MOVIE_ST_OPEN:
            i = 1;
            if (g_gameState->gameData->unk8 != 0) {
                i = 2;
            }
            g_movieFlagQueue[0][i - 1] = MOVIE_NO_SCENES | MOVIE_NO_FADE | MOVIE_NO_OT;
            g_movieFlagQueue[0][i] = MOVIE_PLAYING;
            g_gameState->vsyncMode = 4;
            DrawSync(0);
            MBG_init(1, &buf);
            mbg_setRgb24(1);
            mbg_setEndCallback(opening_movieOnEnd);
            g_movieLastFrame = g_mbgFrame;
            mbg_play(1, 0);
            g_movieState = MOVIE_ST_PLAY;
            break;
        case MOVIE_ST_PLAY:
            if ((g_movieFlags & MOVIE_PLAYING) && (mbg_getStatus() & 2)) {
                r = mbg_update(0);
                end = mbg_getLastFrame();
                if (r != 0) {
                    DrawOTag(g_gameState->gameData->unk10[g_gameState->gameData->unk8 ^ 1] + 0xFFF);
                } else if (timer == -1 && g_gameState->gameData->unk8 == 0 && !(g_movieFlags & MOVIE_SKIPPING)) {
                    g_gameState->dispEnv[g_gameState->gameData->unk8].disp.x = 0x1E0;
                }
                if ((g_mbgFrame >= end || g_movieLastFrame >= g_mbgFrame || timer == 0) && !(g_movieFlags & MOVIE_ENDED)) {
                    if (timer == 0) {
                        mbg_stop();
                        cd_pause();
                        if (!(g_gameState->gameData->unk0 & 0x20000000)) {
                            opening_soundCmd(0, TITLE_MUSIC, 0, 0, 0);
                            opening_soundCmd(0xC401, TITLE_MUSIC, 0x5A, 0, 0x7F);
                        }
                    }
                    if (g_movieLastFrame >= g_mbgFrame) {
                        g_movieFlags &= ~(MOVIE_NO_SCENES | MOVIE_NO_FADE | MOVIE_NO_OT);
                        if (g_gameState->gameData->unk8 == 0) {
                            g_gameState->dispEnv[g_gameState->gameData->unk8].disp.x = 0x1E0;
                        }
                        printf("Fr: %d\n", g_movieLastFrame);
                    } else {
                        g_movieFlagQueue[1][0] = MOVIE_NO_SCENES | MOVIE_NO_FADE | MOVIE_NO_OT;
                    }
                    g_movieFlags |= MOVIE_ENDED;
                }
                if (!(g_movieFlags & MOVIE_MUSIC) && g_mbgFrame > 0) {
                    opening_soundCmd(0, TITLE_MUSIC, 0, 0, 0);
                    g_movieFlags |= MOVIE_MUSIC;
                }
                if (!(g_movieFlags & MOVIE_SKIPPING) && (g_padPortPressed[0] & (PAD_START | PAD_CROSS)) && g_mbgFrame < end) {
                    g_movieSkipped = 1;
                    mbg_setRgb24(0);
                    g_movieFlagQueue[1][g_gameState->gameData->unk8 != 0 ? 1 : 2] = 2;
                    timer = 6;
                    game_startFade(1, 1, 0, 0);
                    game_startFade(1, 1, timer, 0xFFFFFF);
                    g_padPortMask[0] = -1;
                    g_movieFlags |= MOVIE_SKIPPING;
                }
                g_movieLastFrame = g_mbgFrame;
                if (!(g_movieFlags & MOVIE_NO_FADE) && timer != -1) {
                    timer--;
                }
            }
            break;
        case MOVIE_ST_END:
            if ((g_movieFlags & MOVIE_DONE) && cd_getState() == 0) {
                g_gameState->vsyncMode = 0;
                g_gameState->dispEnv[0].disp.x = 0x140;
                timer = TITLE_IDLE_FRAMES;
                g_gameState->dispEnv[1].disp.x = 0;
                g_padPortMask[0] = 0;
                scene_setFadeScreen(g_movieSkipped);
                scene_set(SCENE_TITLE);
                g_movieState = MOVIE_ST_TITLE;
            }
            break;
        case MOVIE_ST_TITLE:
            if (!(g_movieFlags & MOVIE_IDLE_FADE) && (u8)scene_isTitleIdle() && timer < TITLE_IDLE_FADE + 1) {
                game_startFade(1, 1, 0, 0);
                game_startFade(1, 1, TITLE_IDLE_FADE, 0xFFFFFF);
                g_movieFlags |= MOVIE_IDLE_FADE;
            }
            if (timer == 0 && g_movieState != MOVIE_ST_STARTED) {
                g_movieState = MOVIE_ST_TIMEOUT;
            }
            if ((g_movieFlags & MOVIE_IDLE_FADE) || (u8)scene_isTitleIdle()) {
                timer--;
            }
            break;
        }
        if (!(g_movieFlags & MOVIE_NO_SCENES)) {
            scene_update();
            if (scene_get() == SCENE_END) {
                g_movieState = MOVIE_ST_STARTED;
            }
            menu_update();
            game_updateWindows();
        }
        if (!(g_movieFlags & MOVIE_NO_FADE)) {
            game_updateFade();
        }
        opening_movieSwap();
    } while ((u32)g_movieState < MOVIE_ST_TIMEOUT);
    return g_movieState == MOVIE_ST_STARTED;
}

s32 opening_movieInit(void) {
    FileEntry *f;
    s32 h;
    s32 i, j;

    while (cdRead_update(g_gameState->gameData->cdRead) != 0) {
    }
    f = file_find(0, 0x64);
    cdRead_init(g_gameState->gameData->cdRead, f->unk4, (f->unkC - f->unk4) << 11, g_loadAreaEnd + 0xF1700, 0, 0x15000, NULL);
    while (cdRead_update(g_gameState->gameData->cdRead) != 0) {
    }
    EnterCriticalSection();
    FlushCache();
    ExitCriticalSection();
    if (g_gameState->disc != DISC_NONE && (g_gameState->disc & DISC_1)) {
        h = cd_findFile(g_mbgMovieLists[1].movies[0].path);
        cd_seekStreamAsync(h);
        while (cd_getState() != 0) {
        }
        mbg_streamSetBaseSector(h);
    }
    game_startFade(1, 1, 0, 0xFFFFFF);
    g_movieState = MOVIE_ST_START;
    g_movieFlags = 0;
    for (i = 1; i >= 0; i--) {
        for (j = 3; j >= 0; j--) {
            g_movieFlagQueue[i][j] = 0;
        }
    }
    g_movieFrame = 0;
    scene_setFadeScreen(0);
    scene_set(SCENE_TITLE);
    g_padPortMask[0] = 0;
    return 0;
}

void opening_movieClearOt(void) {
    GameData *ctx = g_gameState->gameData;
    ClearOTagR(ctx->unk10[ctx->unk8], 0x1000);
}

void opening_movieSwap(void) {
    s32 i, j;
    GameData *g;

    DrawSync(0);
    if (mbg_getStatus() & 2) {
        if (g_mbgFrame != 0 && g_mbgFrame == g_mbgFrame / 50 * 50) {
            g_gameState->vsyncMode = 3;
        } else {
            g_gameState->vsyncMode = 4;
        }
    } else {
        g_gameState->vsyncMode = 0;
    }
    VSync(g_gameState->vsyncMode);
    ResetGraph(1);
    PutDispEnv(&g_gameState->dispEnv[g_gameState->gameData->unk8]);
    if (g_movieFlags & MOVIE_NO_OT) {
        PutDrawEnv(&g_gameState->drawEnv[g_gameState->gameData->unk8]);
    } else {
        g = g_gameState->gameData;
        DrawOTagEnv(g->unk10[g->unk8] + 0xFFF, &g_gameState->drawEnv[g->unk8]);
    }
    g_gameState->gameData->unk8 ^= 1;
    gfx_setBuffer(g_gameState->gameData->unk8);
    g_movieFlags |= g_movieFlagQueue[0][0];
    g_movieFlags &= ~g_movieFlagQueue[1][0];
    for (i = 0; i < 2; i++) {
        for (j = 0; j < 3; j++) {
            g_movieFlagQueue[i][j] = g_movieFlagQueue[i][j + 1];
        }
        g_movieFlagQueue[i][3] = 0;
    }
    g_movieFrame++;
}

void opening_movieOnEnd(void) {
    mbg_shutdown();
    cd_cancel();
    g_movieFlags |= MOVIE_DONE;
    g_movieState = MOVIE_ST_END;
}
