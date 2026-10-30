#include "ovl_14.h"

s32 opening_attract(s32 arg0) {
    s32 timer;
    s32 i;
    s32 tim;
    s32 pad;
    s32 id;
    s32 k;
    s32 btn;

    i = 0;
    timer = -1;
    opening_attractInit(arg0);
    do {
        opening_attractClearOt();
        pad_update();
        switch (g_attractState) {
        case ATTRACT_ST_START:
            i = 0;
            timer = -1;
            g_attractState = ATTRACT_ST_IMAGES;
            g_attractSkipped = 1;
            break;
        case ATTRACT_ST_IMAGES:
            if (i < ATTRACT_IMAGES) {
                k = arg0 * ATTRACT_IMAGES + i;
                id = g_attractImages[k] | 0x40000;
                if (timer == -1) {
                    tim = (s32)db_lookupRegistered(4, id, 0);
                    file_setTimPos(tim, 0, 0xE0, 0x280, 0);
                    file_loadTim(tim);
                    timer = ATTRACT_FRAMES;
                    game_startFade(1, 1, 0xF0, 0);
                } else if (timer != 0) {
                    pad = g_padPortPressed[0];
                    setlen(&g_attractSprites[g_gameState->gameData->drawBuf][0], 4);
                    g_attractSprites[g_gameState->gameData->drawBuf][0].code = 0x64;
                    g_attractSprites[g_gameState->gameData->drawBuf][0].r0 = 0x80;
                    g_attractSprites[g_gameState->gameData->drawBuf][0].g0 = 0x80;
                    g_attractSprites[g_gameState->gameData->drawBuf][0].b0 = 0x80;
                    g_attractSprites[g_gameState->gameData->drawBuf][0].x0 = 0;
                    g_attractSprites[g_gameState->gameData->drawBuf][0].y0 = 0;
                    g_attractSprites[g_gameState->gameData->drawBuf][0].u0 = 0;
                    g_attractSprites[g_gameState->gameData->drawBuf][0].v0 = 0;
                    g_attractSprites[g_gameState->gameData->drawBuf][0].w = 0xC0;
                    g_attractSprites[g_gameState->gameData->drawBuf][0].h = 0xE0;
                    g_attractSprites[g_gameState->gameData->drawBuf][0].clut = 0x3800;
                    addPrim(&g_gameState->gameData->ot[g_gameState->gameData->drawBuf][SCREEN_OTZ], &g_attractSprites[g_gameState->gameData->drawBuf][0]);
                    setDrawTPage(&g_attractTpages[g_gameState->gameData->drawBuf][0], 1, 1, 0x10A);
                    addPrim(&g_gameState->gameData->ot[g_gameState->gameData->drawBuf][SCREEN_OTZ], &g_attractTpages[g_gameState->gameData->drawBuf][0]);
                    setlen((g_attractSprites[g_gameState->gameData->drawBuf] + 1), 4);
                    (g_attractSprites[g_gameState->gameData->drawBuf] + 1)->code = 0x64;
                    (g_attractSprites[g_gameState->gameData->drawBuf] + 1)->r0 = 0x80;
                    (g_attractSprites[g_gameState->gameData->drawBuf] + 1)->g0 = 0x80;
                    (g_attractSprites[g_gameState->gameData->drawBuf] + 1)->b0 = 0x80;
                    (g_attractSprites[g_gameState->gameData->drawBuf] + 1)->x0 = 0xC0;
                    (g_attractSprites[g_gameState->gameData->drawBuf] + 1)->y0 = 0;
                    (g_attractSprites[g_gameState->gameData->drawBuf] + 1)->u0 = 0;
                    (g_attractSprites[g_gameState->gameData->drawBuf] + 1)->v0 = 0;
                    (g_attractSprites[g_gameState->gameData->drawBuf] + 1)->w = 0x80;
                    (g_attractSprites[g_gameState->gameData->drawBuf] + 1)->h = 0xE0;
                    (g_attractSprites[g_gameState->gameData->drawBuf] + 1)->clut = 0x3800;
                    addPrim(&g_gameState->gameData->ot[g_gameState->gameData->drawBuf][SCREEN_OTZ], (g_attractSprites[g_gameState->gameData->drawBuf] + 1));
                    setDrawTPage(g_attractTpages[g_gameState->gameData->drawBuf] + 1, 1, 1, 0x10D);
                    addPrim(&g_gameState->gameData->ot[g_gameState->gameData->drawBuf][SCREEN_OTZ], (g_attractTpages[g_gameState->gameData->drawBuf] + 1));
                    pad &= PAD_CROSS | PAD_START;
                    btn = pad;
                    if (!(g_attractFlags.bits & ATTRACT_FADING) || btn) {
                        if (btn) {
                            if (timer >= 0x1F) {
                                timer = 0x1E;
                            }
                            g_attractSkipped = 1;
                            g_padPortMask[0] = -1;
                            g_attractFlags.bits |= ATTRACT_SKIP;
                        }
                        if (!(g_attractFlags.bits & ATTRACT_FADING) && timer < 0xF1) {
                            game_startFade(1, 1, timer, 0xFFFFFF);
                            g_attractFlags.bits |= ATTRACT_FADING;
                        }
                    }
                    timer--;
                } else {
                    timer = -1;
                    g_attractFlags.bits &= ~ATTRACT_FADING;
                    if (g_attractFlags.bits & ATTRACT_SKIP) {
                        i = ATTRACT_IMAGES;
                    } else {
                        i++;
                    }
                }
            } else {
                timer = TITLE_IDLE_FRAMES;
                g_padPortMask[0] = 0;
                scene_setFadeScreen(g_attractSkipped);
                scene_set(SCENE_TITLE);
                g_attractState = ATTRACT_ST_TITLE;
            }
            break;
        case ATTRACT_ST_TITLE:
            scene_update();
            if (scene_get() == SCENE_END) {
                g_attractState = ATTRACT_ST_STARTED;
            }
            menu_update();
            game_updateWindows();
            if (!(g_attractFlags.bits & ATTRACT_IDLE_FADE) && (u8)scene_isTitleIdle() && timer < TITLE_IDLE_FADE + 1) {
                game_startFade(1, 1, TITLE_IDLE_FADE, 0xFFFFFF);
                if (g_gameState->disc != DISC_NONE && (g_gameState->disc & DISC_1)) {
                    opening_soundCmd(0x8301, TITLE_MUSIC, 0x5A, 0, 0);
                }
                g_attractFlags.bits |= ATTRACT_IDLE_FADE;
            }
            if (timer == 0) {
                if (g_attractState != ATTRACT_ST_STARTED) {
                    g_attractState = ATTRACT_ST_TIMEOUT;
                }
                if (g_gameState->disc != DISC_NONE && (g_gameState->disc & DISC_1)) {
                    opening_soundCmd(0x100, TITLE_MUSIC, 0, 0, 0);
                }
            }
            if ((g_attractFlags.bits & ATTRACT_IDLE_FADE) || (u8)scene_isTitleIdle()) {
                timer--;
            }
            break;
        }
        game_updateFade();
        opening_attractSwap();
    } while ((u32)g_attractState < ATTRACT_ST_TIMEOUT);
    opening_attractFree(arg0);
    return g_attractState == ATTRACT_ST_STARTED;
}

s32 opening_attractInit(s32 idx) {
    u8 *buf;
    FileEntry *e;

    while (cdRead_update(g_gameState->gameData->cdRead) != 0) {
    }
    e = file_find(1, g_attractFiles[idx]);
    cdRead_init(g_gameState->gameData->cdRead, e->unk4, (e->unkC - e->unk4) << 11, g_loadAreaEnd + 0x10000, 0, -1, 0);
    while (cdRead_update(g_gameState->gameData->cdRead) != 0) {
    }
    buf = g_loadAreaEnd + 0x10000;
    g_attractData = buf;
    db_register((s32)buf, 0);
    game_startFade(1, 1, 0, 0xFFFFFF);
    g_attractState = ATTRACT_ST_START;
    g_attractFlags.bits = 0;
    g_padPortMask[0] = 0;
    return 0;
}

s32 opening_attractFree(s32 arg0) {
    db_unregister(g_attractData);
    g_attractData = NULL;
    return 0;
}

void opening_attractClearOt(void) {
    GameData *ctx = g_gameState->gameData;
    ClearOTagR(ctx->ot[ctx->drawBuf], 0x1000);
}

void opening_attractSwap(void) {
    GameData *ctx;
    u8 i;

    DrawSync(0);
    g_gameState->vsyncMode = 0;
    VSync(g_gameState->vsyncMode);
    ResetGraph(1);
    PutDispEnv(&g_gameState->dispEnv[g_gameState->gameData->drawBuf]);
    ctx = g_gameState->gameData;
    i = ctx->drawBuf;
    DrawOTagEnv(ctx->ot[i] + 0xFFF, &g_gameState->drawEnv[i]);
    g_gameState->gameData->drawBuf ^= 1;
    gfx_setBuffer(g_gameState->gameData->drawBuf);
}
