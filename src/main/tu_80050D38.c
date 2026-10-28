#include "ff9.h"

s32 wrain_draw(s32 r) {
    GameData *gd;

    if (g_worldRain.unk24 < 2) {
        wrain_spawn(g_worldRain.unkC == 0xFF0 ? -0x1770 : -0xFA0);
    }
    if (g_worldRain.unk24 != 0) {
        g_worldRain.unk24++;
    }
    if (g_worldRain.unk4 != 0) {
        gd = g_gameState->gameData;
        gte_SetRotMatrix(&gd->unk18);
        gte_SetTransMatrix(&gd->unk18);
        r = wrain_drawDrops(g_worldRain.unk4, gd->unk8, gd->unk10[gd->unk8] + 3, r);
    }
    return r;
}

u8 *wrain_init(Unk8007BC20Node *buf) {
    Unk8007BC20Node *p = buf;
    Unk8007BC20Node *end;
    g_worldRain.unk0 = buf;
    if (buf != NULL) {
        end = buf + 222;
        for (; p < end; p++) {
            p->unk0 = p + 1;
        }
        p[-1].unk0 = NULL;
    }
    g_worldRain.unk4 = 0;
    SetDrawTPage(&g_worldRain.unk10, 0, 0, GetTPage(0, 3, 0, 0));
    g_worldRain.unk18 = g_worldRain.unk10;
    g_worldRain.unk8 = 0;
    g_worldRain.unkC = 0;
    g_worldRain.unk24 = 0;
    g_worldRainPrims = (u8 *)(buf + 222);
    return (u8 *)p;
}

void wrain_set(s32 a, u32 b) {
    if (a != 0 && b != 0) {
        Unk8007BC20 *p = &g_worldRain;

        if (a < 0x83) {
            p->unk8 = a;
        } else {
            p->unk8 = 0x82;
        }
    }
    g_worldRain.unkC = b * 16;
    g_worldRain.unk24 = a == 0 && b != 0;
}
