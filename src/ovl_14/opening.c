#include "ovl_14.h"

void opening_main(void) {
    s32 pauseLeft;

    pauseLeft = 0;
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
                pauseLeft = OPENING_PAUSE_FRAMES;
                g_openingState = OPENING_PAUSE1;
            } else {
                g_openingState = OPENING_ATTRACT2;
            }
            break;
        case OPENING_PAUSE1:
            VSync(0);
            if (--pauseLeft == 0) {
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
                pauseLeft = OPENING_PAUSE_FRAMES;
                g_openingState = OPENING_PAUSE2;
            } else {
                g_openingState = OPENING_ATTRACT1;
            }
            break;
        case OPENING_PAUSE2:
            VSync(0);
            if (--pauseLeft == 0) {
                g_openingState = OPENING_MOVIE;
            }
            break;
        case OPENING_BOOT:
            opening_bootScreen();
            g_openingState = OPENING_MOVIE;
            break;
        }
    }
    g_gameState->flags |= GS_RELOAD_FILE_TABLES;
    file_loadDirTables(g_gameState);
    opening_findSceneSectors();
    scene_exit();
    opening_exit();
}

void opening_clearOt(void) {
    GameData *ctx = g_gameState->gameData;
    ClearOTagR(ctx->ot[ctx->drawBuf], OT_LEN);
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
    DrawOTagEnv(ctx->ot[i] + OT_LAST, &g_gameState->drawEnv[i]);
    g_gameState->gameData->drawBuf ^= 1;
    gfx_setBuffer(g_gameState->gameData->drawBuf);
}

void opening_startField0(void) {
    GameData *ctx = g_gameState->gameData;
    game_setClearColor(0, 0, 0);
    g_gameState->mode = GS_MODE_FIELD;
    g_gameState->vsyncMode = 2;
    ctx->fieldId = 0;
    ctx->fieldAreaId = 1;
}

/* looks up the current area/field ids in the two directories at g_loadAreaEnd + 0x14C700 and stores their sector ranges */
void opening_findSceneSectors(void) {
    GameState *gs = g_gameState;
    s32 mode = gs->mode;
    GameData *ctx = gs->gameData;
    DirInfo *dirs;
    s32 base;
    u8 *area;
    u8 *start;
    SceneDirEntry *entry;
    s32 key;
    s32 id;

    if (mode == GS_MODE_FIELD || mode == GS_MODE_WORLD) {
        id = -1;
        dirs = gs->dirs;
        base = dirs[0].tableSector;
        area = g_loadAreaEnd + ((base << 11) + 0x14C700);
        if (mode == GS_MODE_FIELD) {
            key = (s16)ctx->fieldId;
            start = area + ((dirs[4].tableSector - base) << 11);
            entry = (SceneDirEntry *)start + dirs[4].count - 1;
        } else {
            key = ctx->worldId;
            start = area + ((dirs[3].tableSector - base) << 11);
            entry = (SceneDirEntry *)start + dirs[3].count - 1;
        }
        for (;;) {
            if (entry->id == key) {
                id = entry->link;
                ctx->sceneSector[1] = entry->sector;
                ctx->sceneSectors[1] = entry[1].sector - entry->sector;
                break;
            }
            entry--;
            if ((u8 *)entry < start) {
                break;
            }
        }
        start = area + ((gs->dirs[2].tableSector - base) << 11);
        entry = (SceneDirEntry *)start + gs->dirs[2].count - 1;
        for (;;) {
            if (entry->id == id) {
                ctx->sceneSector[0] = entry->sector;
                ctx->sceneSectors[0] = entry[1].sector - entry->sector;
                break;
            }
            entry--;
            if ((u8 *)entry < start) {
                break;
            }
        }
        if (mode == GS_MODE_FIELD) {
            ctx->fieldAreaId = id;
        } else {
            ctx->worldAreaId = id;
        }
    }
}
