#include "ovl_14.h"

void opening_main(void) {
    s32 n;

    n = 0;
    opening_init();
    scene_init();
    g_openingState = g_resetCount == 0 ? OPENING_BOOT : OPENING_MOVIE;
    while (g_openingState < OPENING_DONE) {
        switch (g_openingState) {
        case OPENING_MOVIE:
            if (opening_movieThenTitle() != 0) {
                g_openingState = OPENING_DONE;
            } else {
                g_openingState = OPENING_ATTRACT1;
            }
            break;
        case OPENING_ATTRACT1:
            if (opening_attract(0) != 0) {
                g_openingState = OPENING_DONE;
            } else if (g_gameState->disc != DISC_NONE && (g_gameState->disc & DISC_1)) {
                n = OPENING_PAUSE_FRAMES;
                g_openingState = OPENING_PAUSE1;
            } else {
                g_openingState = OPENING_ATTRACT2;
            }
            break;
        case OPENING_PAUSE1:
            VSync(0);
            if (--n == 0) {
                g_openingState = OPENING_MOVIE2;
            }
            break;
        case OPENING_MOVIE2:
            if (opening_movieThenTitle() == 0) {
                g_openingState = OPENING_ATTRACT2;
            } else {
                g_openingState = OPENING_DONE;
            }
            break;
        case OPENING_ATTRACT2:
            if (opening_attract(1) != 0) {
                g_openingState = OPENING_DONE;
            } else if (g_gameState->disc != DISC_NONE && (g_gameState->disc & DISC_1)) {
                n = OPENING_PAUSE_FRAMES;
                g_openingState = OPENING_PAUSE2;
            } else {
                g_openingState = OPENING_ATTRACT1;
            }
            break;
        case OPENING_PAUSE2:
            VSync(0);
            if (--n == 0) {
                g_openingState = OPENING_MOVIE;
            }
            break;
        case OPENING_BOOT:
            opening_bootScreen();
            g_openingState = OPENING_MOVIE;
            break;
        }
    }
    g_gameState->unk0 |= 0x100000;
    file_loadDirTables(g_gameState);
    opening_findSceneSectors();
    scene_exit();
    opening_exit();
}

void opening_clearOt(void) {
    GameData *ctx = g_gameState->gameData;
    ClearOTagR(ctx->ot[ctx->drawBuf], 0x1000);
}

void opening_swapBuffers(void) {
    GameData *ctx;
    u8 i;

    DrawSync(0);
    VSync(g_gameState->vsyncMode);
    ResetGraph(1);
    PutDispEnv(&g_gameState->dispEnv[g_gameState->gameData->drawBuf]);
    ctx = g_gameState->gameData;
    i = ctx->drawBuf;
    DrawOTagEnv(ctx->ot[i] + 0xFFF, &g_gameState->drawEnv[i]);
    g_gameState->gameData->drawBuf ^= 1;
    gfx_setBuffer(g_gameState->gameData->drawBuf);
}

void func_800A73B8(void) {
    GameData *ctx = g_gameState->gameData;
    game_setClearColor(0, 0, 0);
    g_gameState->mode = 1;
    g_gameState->vsyncMode = 2;
    ctx->fieldId = 0;
    ctx->fieldAreaId = 1;
}

/* looks up the current area/field ids in the two directories at g_loadAreaEnd + 0x14C700 and stores their sector ranges */
void opening_findSceneSectors(void) {
    GameState *gs = g_gameState;
    s32 mode = gs->mode;
    GameData *ctx = gs->gameData;
    DirInfo *f;
    s32 base;
    u8 *area;
    u8 *start;
    SceneDirEntry *e;
    s32 key;
    s32 id;

    if (mode == 1 || mode == 3) {
        id = -1;
        f = gs->dirs;
        base = f[0].tableSector;
        area = g_loadAreaEnd + ((base << 11) + 0x14C700);
        if (mode == 1) {
            key = (s16)ctx->fieldId;
            start = area + ((f[4].tableSector - base) << 11);
            e = (SceneDirEntry *)start + f[4].count - 1;
        } else {
            key = ctx->worldId;
            start = area + ((f[3].tableSector - base) << 11);
            e = (SceneDirEntry *)start + f[3].count - 1;
        }
        for (;;) {
            if (e->id == key) {
                id = e->link;
                ctx->sceneSector[1] = e->sector;
                ctx->sceneSectors[1] = e[1].sector - e->sector;
                break;
            }
            e--;
            if ((u8 *)e < start) {
                break;
            }
        }
        start = area + ((gs->dirs[2].tableSector - base) << 11);
        e = (SceneDirEntry *)start + gs->dirs[2].count - 1;
        for (;;) {
            if (e->id == id) {
                ctx->sceneSector[0] = e->sector;
                ctx->sceneSectors[0] = e[1].sector - e->sector;
                break;
            }
            e--;
            if ((u8 *)e < start) {
                break;
            }
        }
        if (mode == 1) {
            ctx->fieldAreaId = id;
        } else {
            ctx->worldAreaId = id;
        }
    }
}
