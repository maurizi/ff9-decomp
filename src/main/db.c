#include "ff9.h"

void db_init(void) {
    db_clear();
}

void db_quit(void) {
}

void db_clear(void) {
    s32 i;

    g_dbList.unk0 = 0;
    g_dbList.unk2 = 0;
    for (i = 15; i >= 0; i--) {
        g_dbList.unk4[i] = 0;
    }
}

s32 db_register(s32 a, s32 b) {
    g_dbList.unk4[g_dbList.unk2] = a;
    g_dbList.unk2++;
    return 0;
}

s32 db_unregister(void *arg0) {
    s32 i;

    for (i = g_dbList.unk2 - 1; i >= 0; i--) {
        if (g_dbList.unk4[i] == arg0) {
            g_dbList.unk4[i] = g_dbList.unk4[g_dbList.unk2 - 1];
            g_dbList.unk2--;
            return 0;
        }
    }
    return -1;
}

void *db_lookupRegistered(s32 mode, s32 a1, s32 a2) {
    s32 i;
    s32 r = -1;
    void *ret;

    for (i = g_dbList.unk2 - 1; i >= 0; i--) {
        r = db_lookup((s32)g_dbList.unk4[i], mode, a1, a2);
        if (r != -1) {
            return (void *)r;
        }
    }
    ret = (void *)-1;
    if (r == -1) {
        r = mode & 1;
        ret = NULL;
        if (r != 0) {
            ret = (void *)-1;
        }
    }
    return ret;
}

s32 db_lookup(s32 arg0, s32 mode, s32 key, s32 arg3) {
    u8 *base = (u8 *)arg0;
    u32 *ent;
    s32 i;
    s32 n;
    s32 j;
    s32 cnt;
    s32 type;
    u8 *blk;
    u16 *ids;
    u32 *ofs;
    u32 *p;
    s32 hi;
    s32 lo;
    s32 r;

    if (base == NULL) {
        return 0;
    }
    ent = (u32 *)(base + 4);
    hi = (key >> 16) & 0xFF;
    n = base[1];
    lo = key & 0xFFFF;
    for (i = 0; i < n; i++, ent++) {
        blk = (u8 *)*ent;
        type = ((u32)blk >> 24) & 0x7F;
        if (type == hi) {
            blk = (u8 *)ent + ((u32)blk & 0xFFFFFF);
            cnt = blk[1];
            if (mode == 0) {
                return (s32)blk;
            }
            ids = (u16 *)(blk + 4);
            if (mode == 1) {
                return cnt;
            }
            ofs = (u32 *)((u8 *)ids + cnt * 2);
            if ((s32)ofs & 3) {
                ofs = (u32 *)((u8 *)ofs - (((s32)ofs & 3) - 4));
            }
            if (mode == 3) {
                return (type << 16) | ids[lo];
            }
            if (mode == 2) {
                p = &ofs[lo];
                return (s32)p + (*p & 0xFFFFFF);
            }
            for (j = 0; j < cnt; j++) {
                if (*ids == lo) {
                    switch (mode) {
                    case 4:
                        do {
                        } while (0);
                        return (s32)ofs + (*ofs & 0xFFFFFF);
                    case 5:
                        return (ofs[1] & 0xFFFFFF) - (ofs[0] & 0xFFFFFF) + 4;
                    case 9:
                        r = db_lookup(arg0, 3, arg3, 0);
                        return r;
                    case 8:
                        r = db_lookup(arg0, 2, arg3, 0);
                        return r;
                    case 11:
                        r = db_lookup(arg0, 1, arg3, 0);
                        return r;
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

s32 db_getSize(s32 a) {
    u8 *p = (u8 *)a;
    u32 *q;
    u8 *e;
    u32 *r;
    s32 k;
    s32 n;
    s32 m;

    if (p == NULL || p[0] != 0xDB) {
        return -1;
    }
    n = p[1];
    if (n <= 0) {
        return 4;
    }
    q = (u32 *)p + n;
    e = (u8 *)q + (*q & 0xFFFFFF);
    k = e[1];
    n = k * 2 + 4;
    m = n & 3;
    if (m != 0) {
        n += 4 - m;
    }
    r = (u32 *)(e + n + k * 4);
    return (u8 *)r + (*r & 0xFFFFFF) - p;
}

void game_initState(void) {
    GameState *gs;

    game_allocState();
    gs = g_gameState;
    game_resetState();
    game_setScreenHeight(0xE0);
    file_initImg();
    file_loadDirTables();
    config_init();
    game_initTimer();
    gs->allocTop = g_allocTop;
}

void game_quitState(void) {
}

void game_allocState(void) {
    g_allocTop = (s32)PSX_RAM(u8 *, 0x8008B000);
    g_gameState = PSX_RAM(GameState *, 0x8008B000);
    g_gameState->dispEnv = PSX_RAM(DISPENV *, 0x8008B044);
    g_gameState->drawEnv = PSX_RAM(DRAWENV *, 0x8008B06C);
    g_gameState->dirs = PSX_RAM(DirInfo *, 0x8008B124);
    g_gameState->dirTables = PSX_RAM(void *, 0x8008B214);
    g_allocTop = (s32)PSX_RAM(u8 *, 0x8008B250);
}

void game_resetState(void) {
    GameState *gs = g_gameState;
    s32 i;

    gs->flags = GS_RELOAD_FILE_TABLES;
    gs->disc = 0xFF;
    gs->allocTop = 0;
    gs->mode = 0;
    gs->prevMode = 0;
    gs->imgSector = -1;
    gs->gameData = NULL;
    game_setClearColor(0, 0, 0);
    game_setScreenHeight(0xE0);
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
    RECT r;
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
        col.unk0[0] = 0xFF - col.unk0[0];
        col.unk0[1] = 0xFF - col.unk0[1];
        col.unk0[2] = 0xFF - col.unk0[2];
    }
    if (!fade) {
        col.unk0[0] = g_gameState->drawEnv->r0;
        col.unk0[1] = g_gameState->drawEnv->g0;
        col.unk0[2] = g_gameState->drawEnv->b0;
    }
    if (g_gameState->dispEnv->disp.h < h) {
        r.x = 0;
        r.y = g_gameState->dispEnv->disp.h;
        r.w = 0x280;
        r.h = h - g_gameState->dispEnv->disp.h;
        ClearImage(&r, col.unk0[0], col.unk0[1], col.unk0[2]);
        DrawSync(0);
    }
    SetDefDispEnv(&g_gameState->dispEnv[0], 0x140, 0, 0x140, h);
    SetDefDispEnv(&g_gameState->dispEnv[1], 0, 0, 0x140, h);
    g_gameState->dispEnv[0].isinter = g_gameState->dispEnv[1].isinter = 0;
    g_gameState->dispEnv[0].screen.x = g_gameState->dispEnv[1].screen.x = 0;
    g_gameState->dispEnv[0].screen.y = g_gameState->dispEnv[1].screen.y = (0xF0 - h) / 2;
    g_gameState->dispEnv[0].screen.h = g_gameState->dispEnv[1].screen.h = h;
    SetDefDrawEnv(&g_gameState->drawEnv[0], 0, 0, 0x140, h);
    SetDefDrawEnv(&g_gameState->drawEnv[1], 0x140, 0, 0x140, h);
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
            g_freezeDrawEnv.r0 = col.unk0[0];
            g_freezeDrawEnv.g0 = col.unk0[1];
            g_freezeDrawEnv.b0 = col.unk0[2];
        } else {
            g_freezeDrawEnv.isbg = 0;
        }
        PutDrawEnv(&g_freezeDrawEnv);
        DrawSync(0);
    }
}
