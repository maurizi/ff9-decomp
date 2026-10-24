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
        setlen(&g_bootScreenSprite[g_gameState->unk1C->unk8], 4);
        g_bootScreenSprite[g_gameState->unk1C->unk8].code = 0x64;
        g_bootScreenSprite[g_gameState->unk1C->unk8].r0 = 0x80;
        g_bootScreenSprite[g_gameState->unk1C->unk8].g0 = 0x80;
        g_bootScreenSprite[g_gameState->unk1C->unk8].b0 = 0x80;
        g_bootScreenSprite[g_gameState->unk1C->unk8].x0 = (0x140 - r->w) / 2;
        g_bootScreenSprite[g_gameState->unk1C->unk8].y0 = (0xE0 - r->h) / 2;
        g_bootScreenSprite[g_gameState->unk1C->unk8].u0 = 0;
        g_bootScreenSprite[g_gameState->unk1C->unk8].v0 = 0;
        g_bootScreenSprite[g_gameState->unk1C->unk8].w = r->w;
        g_bootScreenSprite[g_gameState->unk1C->unk8].h = r->h;
        g_bootScreenSprite[g_gameState->unk1C->unk8].clut = 0x3800;
        addPrim(&g_gameState->unk1C->unk10[g_gameState->unk1C->unk8][SCREEN_OTZ], &g_bootScreenSprite[g_gameState->unk1C->unk8]);
        setDrawTPage(&g_bootScreenTpage[g_gameState->unk1C->unk8], 1, 1, 0xA);
        addPrim(&g_gameState->unk1C->unk10[g_gameState->unk1C->unk8][SCREEN_OTZ], &g_bootScreenTpage[g_gameState->unk1C->unk8]);
        t--;
        game_updateFade();
        opening_bootSwap();
    } while (t > 0);
    opening_bootScreenFree();
}

void opening_bootScreenLoad(void) {
    u8 *buf;
    FileEntry *e;

    while (cdRead_update(g_gameState->unk1C->unk854) != 0) {
    }
    e = file_find(1, 0x2718);
    cdRead_init(g_gameState->unk1C->unk854, e->unk4, (e->unkC - e->unk4) << 11, D_80067944 + 0x10000, 0, -1, 0);
    while (cdRead_update(g_gameState->unk1C->unk854) != 0) {
    }
    buf = D_80067944 + 0x10000;
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
    GameData *ctx = g_gameState->unk1C;
    ClearOTagR(ctx->unk10[ctx->unk8], 0x1000);
}

void opening_bootSwap(void) {
    GameData *ctx;
    u8 i;

    DrawSync(0);
    g_gameState->vsyncMode = 0;
    VSync(g_gameState->vsyncMode);
    ResetGraph(1);
    PutDispEnv(&g_gameState->unkC[g_gameState->unk1C->unk8]);
    ctx = g_gameState->unk1C;
    i = ctx->unk8;
    DrawOTagEnv(ctx->unk10[i] + 0xFFF, &g_gameState->unk10[i]);
    g_gameState->unk1C->unk8 ^= 1;
    gfx_setBuffer(g_gameState->unk1C->unk8);
}

void func_800AB1E4(void) {
    g_gameState->unk1C->unk890 = 1;
}

void func_800AB200(void) {
    GameData *ctx = g_gameState->unk1C;
    if (ctx->unk890 == 0) {
        ctx->unk890 = 2;
    }
}
