#include "ff9.h"

void db_init(void) {
    db_clear();
}

void db_quit(void) {
}

void db_clear(void) {
    s32 i;

    g_dbList.unk0 = 0;
    g_dbList.count = 0;
    for (i = 15; i >= 0; i--) {
        g_dbList.dbs[i] = 0;
    }
}

s32 db_register(s32 db, s32 unused) {
    g_dbList.dbs[g_dbList.count] = db;
    g_dbList.count++;
    return 0;
}

s32 db_unregister(void *db) {
    s32 i;

    for (i = g_dbList.count - 1; i >= 0; i--) {
        if (g_dbList.dbs[i] == db) {
            g_dbList.dbs[i] = g_dbList.dbs[g_dbList.count - 1];
            g_dbList.count--;
            return 0;
        }
    }
    return -1;
}

void *db_lookupRegistered(s32 mode, s32 key, s32 subKey) {
    s32 i;
    s32 val = -1; /* db_lookup result, then the NULL-or-(-1) mode bit (separate locals don't match) */
    void *result;

    for (i = g_dbList.count - 1; i >= 0; i--) {
        val = db_lookup((s32)g_dbList.dbs[i], mode, key, subKey);
        if (val != -1) {
            return (void *)val;
        }
    }
    result = (void *)-1;
    if (val == -1) {
        val = mode & 1;
        result = NULL;
        if (val != 0) {
            result = (void *)-1;
        }
    }
    return result;
}

s32 db_lookup(s32 db, s32 mode, s32 key, s32 subKey) {
    u8 *base = (u8 *)db;
    u32 *ent;
    s32 i;
    s32 typeCount;
    s32 j;
    s32 cnt;
    s32 type;
    u8 *blk;
    u16 *ids;
    u32 *ofs;
    u32 *ofsEnt;
    s32 hi;
    s32 lo;
    s32 subResult;

    if (base == NULL) {
        return 0;
    }
    ent = (u32 *)(base + 4);
    hi = (key >> 16) & 0xFF;
    typeCount = base[1];
    lo = key & 0xFFFF;
    for (i = 0; i < typeCount; i++, ent++) {
        blk = (u8 *)*ent;
        type = DB_ENTRY_KIND((u32)blk);
        if (type == hi) {
            blk = (u8 *)ent + ((u32)blk & DB_ENTRY_OFS);
            cnt = blk[1];
            if (mode == DB_GET_BLOCK) {
                return (s32)blk;
            }
            ids = (u16 *)(blk + 4);
            if (mode == DB_GET_COUNT) {
                return cnt;
            }
            ofs = (u32 *)((u8 *)ids + cnt * 2);
            if ((s32)ofs & 3) {
                ofs = (u32 *)((u8 *)ofs - (((s32)ofs & 3) - 4));
            }
            if (mode == DB_GET_KEY_AT) {
                return (type << 16) | ids[lo];
            }
            if (mode == DB_GET_AT) {
                ofsEnt = &ofs[lo];
                return (s32)ofsEnt + (*ofsEnt & DB_ENTRY_OFS);
            }
            for (j = 0; j < cnt; j++) {
                if (*ids == lo) {
                    switch (mode) {
                    case DB_GET_ADDR:
                        do {
                        } while (0);
                        return (s32)ofs + (*ofs & DB_ENTRY_OFS);
                    case DB_GET_SIZE:
                        return (ofs[1] & DB_ENTRY_OFS) - (ofs[0] & DB_ENTRY_OFS) + 4;
                    case DB_GET_KEY_AT_IN:
                        subResult = db_lookup(db, DB_GET_KEY_AT, subKey, 0);
                        return subResult;
                    case DB_GET_AT_IN:
                        subResult = db_lookup(db, DB_GET_AT, subKey, 0);
                        return subResult;
                    case DB_GET_COUNT_IN:
                        subResult = db_lookup(db, DB_GET_COUNT, subKey, 0);
                        return subResult;
                    default:
                        return -1;
                    }
                }
                ids++;
                ofs++;
            }
        }
    }
    return -1;
}

s32 db_getSize(s32 db) {
    u8 *base = (u8 *)db;
    u32 *lastEnt;
    u8 *blk;
    u32 *endEnt;
    s32 k;
    s32 tmp; /* entry count, then the id table size (separate locals don't match) */
    s32 misalign;

    if (base == NULL || base[0] != DB_MAGIC) {
        return -1;
    }
    tmp = base[1];
    if (tmp <= 0) {
        return 4;
    }
    lastEnt = (u32 *)base + tmp;
    blk = (u8 *)lastEnt + (*lastEnt & DB_ENTRY_OFS);
    k = blk[1];
    tmp = k * 2 + 4;
    misalign = tmp & 3;
    if (misalign != 0) {
        tmp += 4 - misalign;
    }
    endEnt = (u32 *)(blk + tmp + k * 4);
    return (u8 *)endEnt + (*endEnt & DB_ENTRY_OFS) - base;
}

void game_initState(void) {
    GameState *gs;

    game_allocState();
    gs = g_gameState;
    game_resetState();
    game_setScreenHeight(SCREEN_H);
    file_initImg();
    file_loadDirTables();
    config_init();
    game_initTimer();
    gs->allocTop = g_allocTop;
}

void game_quitState(void) {
}

void game_allocState(void) {
    g_allocTop = (s32)RAM_GAME_STATE(u8 *);
    g_gameState = &RAM_GAME_STATE(GameStateBlock *)->state;
    g_gameState->dispEnv = RAM_GAME_STATE(GameStateBlock *)->dispEnv;
    g_gameState->drawEnv = RAM_GAME_STATE(GameStateBlock *)->drawEnv;
    g_gameState->dirs = RAM_GAME_STATE(GameStateBlock *)->dirs;
    g_gameState->dirTables = RAM_GAME_STATE(GameStateBlock *)->dirTables;
    g_allocTop = (s32)(RAM_GAME_STATE(GameStateBlock *) + 1);
}

void game_resetState(void) {
    GameState *gs = g_gameState;
    s32 i;

    gs->flags = GS_RELOAD_FILE_TABLES;
    gs->disc = DISC_NONE;
    gs->allocTop = 0;
    gs->mode = GS_MODE_OPENING;
    gs->prevMode = GS_MODE_OPENING;
    gs->imgSector = -1;
    gs->gameData = NULL;
    game_setClearColor(0, 0, 0);
    game_setScreenHeight(SCREEN_H);
    gs->vsyncMode = 0;
    for (i = 14; i >= 0; i--) {
        gs->dirs[i].type = i;
        gs->dirs[i].count = 0;
        gs->dirs[i].tableSector = 0;
        gs->dirs[i].firstSector = 0;
        gs->dirTables[i] = NULL;
    }
    gs->dirCount = 0;
    for (i = 0; (u32)i < 15; i++) {
        gs->unk34[i] = 0;
    }
}

void game_setClearColor(s32 r, s32 g, s32 b) {
    DRAWENV *env;

    env = g_gameState->drawEnv;
    env[1].r0 = r;
    env[0].r0 = r;
    env = g_gameState->drawEnv;
    env[1].g0 = g;
    env[0].g0 = g;
    env = g_gameState->drawEnv;
    env[1].b0 = b;
    env[0].b0 = b;
}

void game_setScreenHeight(s32 h) {
    Unk4 fade_getColor(void);
    Unk4 col;
    RECT rect;
    s32 fade;
    s32 busy;
    DRAWENV *draw;

    fade = 0;
    if (fade_isActive() || fade_isBusy()) {
        fade = 1;
    }
    busy = fade_isSubtractive();
    col = fade_getColor();
    if (busy == 1) {
        col.b[0] = 0xFF - col.b[0];
        col.b[1] = 0xFF - col.b[1];
        col.b[2] = 0xFF - col.b[2];
    }
    if (!fade) {
        col.b[0] = g_gameState->drawEnv->r0;
        col.b[1] = g_gameState->drawEnv->g0;
        col.b[2] = g_gameState->drawEnv->b0;
    }
    if (g_gameState->dispEnv->disp.h < h) {
        rect.x = 0;
        rect.y = g_gameState->dispEnv->disp.h;
        rect.w = 0x280;
        rect.h = h - g_gameState->dispEnv->disp.h;
        ClearImage(&rect, col.b[0], col.b[1], col.b[2]);
        DrawSync(0);
    }
    SetDefDispEnv(&g_gameState->dispEnv[0], SCREEN_W, 0, SCREEN_W, h);
    SetDefDispEnv(&g_gameState->dispEnv[1], 0, 0, SCREEN_W, h);
    g_gameState->dispEnv[0].isinter = g_gameState->dispEnv[1].isinter = 0;
    g_gameState->dispEnv[0].screen.x = g_gameState->dispEnv[1].screen.x = 0;
    g_gameState->dispEnv[0].screen.y = g_gameState->dispEnv[1].screen.y = (SCREEN_H_NTSC - h) / 2;
    g_gameState->dispEnv[0].screen.h = g_gameState->dispEnv[1].screen.h = h;
    SetDefDrawEnv(&g_gameState->drawEnv[0], 0, 0, SCREEN_W, h);
    SetDefDrawEnv(&g_gameState->drawEnv[1], SCREEN_W, 0, SCREEN_W, h);
    draw = g_gameState->drawEnv;
    draw[1].dfe = 1;
    draw[0].dfe = 1;
    draw = g_gameState->drawEnv;
    draw[1].dtd = 1;
    draw[0].dtd = 1;
    draw = g_gameState->drawEnv;
    draw[1].isbg = 1;
    draw[0].isbg = 1;
    VSync(0);
    if (g_gameState->gameData != NULL) {
        PutDispEnv(&g_gameState->dispEnv[g_gameState->gameData->drawBuf]);
    }
}

void game_freezeScreen(void) {
    Unk4 fade_getColor(void); /* ovl_0a declares it with an out-pointer */
    Unk4 col;
    s32 fade;
    s32 busy;

    if (g_gameState->gameData != NULL) {
        VSync(0);
        PutDispEnv(&g_gameState->dispEnv[g_gameState->gameData->drawBuf]);
        fade = 0;
        memmove(&g_freezeDrawEnv, &g_gameState->drawEnv[g_gameState->gameData->drawBuf ^ 1], sizeof(DRAWENV));
        if (fade_isActive() || fade_isBusy()) {
            fade = 1;
        }
        busy = fade_isSubtractive();
        col = fade_getColor();
        if (fade && !busy) {
            g_freezeDrawEnv.isbg = 1;
            g_freezeDrawEnv.r0 = col.b[0];
            g_freezeDrawEnv.g0 = col.b[1];
            g_freezeDrawEnv.b0 = col.b[2];
        } else {
            g_freezeDrawEnv.isbg = 0;
        }
        PutDrawEnv(&g_freezeDrawEnv);
        DrawSync(0);
    }
}
