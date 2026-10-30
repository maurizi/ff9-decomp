#include "ovl_14.h"

void opening_bootScreen(void) {
    RECT *r;
    s32 t;

    t = 0xD2;
    opening_bootScreenLoad();
    file_getTimRects((void *)g_bootScreenImage, NULL, &r);
    r->w *= 4;
    do {
        opening_bootClearOt();
        pad_update();
        if (t == 0xD2) {
            game_startFade(1, 1, 0x3C, 0);
        } else if (t == 0x3C) {
            game_startFade(1, 1, 0x3C, 0xFFFFFF);
        }
        setlen(&g_bootScreenSprite[g_gameState->gameData->unk8], 4);
        g_bootScreenSprite[g_gameState->gameData->unk8].code = 0x64;
        g_bootScreenSprite[g_gameState->gameData->unk8].r0 = 0x80;
        g_bootScreenSprite[g_gameState->gameData->unk8].g0 = 0x80;
        g_bootScreenSprite[g_gameState->gameData->unk8].b0 = 0x80;
        g_bootScreenSprite[g_gameState->gameData->unk8].x0 = (0x140 - r->w) / 2;
        g_bootScreenSprite[g_gameState->gameData->unk8].y0 = (0xE0 - r->h) / 2;
        g_bootScreenSprite[g_gameState->gameData->unk8].u0 = 0;
        g_bootScreenSprite[g_gameState->gameData->unk8].v0 = 0;
        g_bootScreenSprite[g_gameState->gameData->unk8].w = r->w;
        g_bootScreenSprite[g_gameState->gameData->unk8].h = r->h;
        g_bootScreenSprite[g_gameState->gameData->unk8].clut = 0x3800;
        addPrim(&g_gameState->gameData->unk10[g_gameState->gameData->unk8][SCREEN_OTZ], &g_bootScreenSprite[g_gameState->gameData->unk8]);
        setDrawTPage(&g_bootScreenTpage[g_gameState->gameData->unk8], 1, 1, 0xA);
        addPrim(&g_gameState->gameData->unk10[g_gameState->gameData->unk8][SCREEN_OTZ], &g_bootScreenTpage[g_gameState->gameData->unk8]);
        t--;
        game_updateFade();
        opening_bootSwap();
    } while (t > 0);
    opening_bootScreenFree();
}

void opening_bootScreenLoad(void) {
    u8 *buf;
    FileEntry *e;

    while (cdRead_update(g_gameState->gameData->cdRead) != 0) {
    }
    e = file_find(1, 0x2718);
    cdRead_init(g_gameState->gameData->cdRead, e->unk4, (e->unkC - e->unk4) << 11, g_loadAreaEnd + 0x10000, 0, -1, 0);
    while (cdRead_update(g_gameState->gameData->cdRead) != 0) {
    }
    buf = g_loadAreaEnd + 0x10000;
    g_bootScreenData = buf;
    db_register((s32)buf, 0);
    g_bootScreenImage = (s32)db_lookupRegistered(4, 0x44FB0, 0);
    file_setTimPos(g_bootScreenImage, 0, 0xE0, 0x280, 0);
    file_loadTim(g_bootScreenImage);
    game_startFade(1, 1, 0, 0xFFFFFF);
    g_padPortMask[0] = 0;
}

void opening_bootScreenFree(void) {
    db_unregister(g_bootScreenData);
    g_bootScreenData = NULL;
}

void opening_bootClearOt(void) {
    GameData *ctx = g_gameState->gameData;
    ClearOTagR(ctx->unk10[ctx->unk8], 0x1000);
}

void opening_bootSwap(void) {
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

void func_800AB1E4(void) {
    g_gameState->gameData->unk890 = 1;
}

void func_800AB200(void) {
    GameData *ctx = g_gameState->gameData;
    if (ctx->unk890 == 0) {
        ctx->unk890 = 2;
    }
}
