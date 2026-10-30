#include "ff9.h"

s32 wrain_draw(s32 r) {
    GameData *gd;

    if (g_worldRain.stopping < 2) {
        wrain_spawn(g_worldRain.speed == 0xFF0 ? -0x1770 : -0xFA0);
    }
    if (g_worldRain.stopping != 0) {
        g_worldRain.stopping++;
    }
    if (g_worldRain.active != 0) {
        gd = g_gameState->gameData;
        gte_SetRotMatrix(&gd->viewMatrix);
        gte_SetTransMatrix(&gd->viewMatrix);
        r = wrain_drawDrops(g_worldRain.active, gd->drawBuf, gd->ot[gd->drawBuf] + 3, r);
    }
    return r;
}

u8 *wrain_init(RainNode *buf) {
    RainNode *p = buf;
    RainNode *end;
    g_worldRain.freeList = buf;
    if (buf != NULL) {
        end = buf + 222;
        for (; p < end; p++) {
            p->next = p + 1;
        }
        p[-1].next = NULL;
    }
    g_worldRain.active = 0;
    SetDrawTPage(&g_worldRain.tpage, 0, 0, GetTPage(0, 3, 0, 0));
    g_worldRain.tpage2 = g_worldRain.tpage;
    g_worldRain.count = 0;
    g_worldRain.speed = 0;
    g_worldRain.stopping = 0;
    g_worldRainPrims = (u8 *)(buf + 222);
    return (u8 *)p;
}

void wrain_set(s32 a, u32 b) {
    if (a != 0 && b != 0) {
        RainState *p = &g_worldRain;

        if (a < 0x83) {
            p->count = a;
        } else {
            p->count = 0x82;
        }
    }
    g_worldRain.speed = b * 16;
    g_worldRain.stopping = a == 0 && b != 0;
}
