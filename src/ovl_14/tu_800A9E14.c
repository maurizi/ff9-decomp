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
        case 0:
            i = 0;
            timer = -1;
            g_attractState = 1;
            g_attractSkipped = 1;
            break;
        case 1:
            if (i < 4) {
                k = arg0 * 4 + i;
                id = g_attractImages[k] | 0x40000;
                if (timer == -1) {
                    tim = (s32)db_lookupRegistered(4, id, 0);
                    func_8001D9CC(tim, 0, 0xE0, 0x280, 0);
                    func_8001D8A8(tim);
                    timer = 0x3A2;
                    func_800320B8(1, 1, 0xF0, 0);
                } else if (timer != 0) {
                    pad = g_padPortPressed[0];
                    setlen(&g_attractSprites[g_gameState->unk1C->unk8][0], 4);
                    g_attractSprites[g_gameState->unk1C->unk8][0].code = 0x64;
                    g_attractSprites[g_gameState->unk1C->unk8][0].r0 = 0x80;
                    g_attractSprites[g_gameState->unk1C->unk8][0].g0 = 0x80;
                    g_attractSprites[g_gameState->unk1C->unk8][0].b0 = 0x80;
                    g_attractSprites[g_gameState->unk1C->unk8][0].x0 = 0;
                    g_attractSprites[g_gameState->unk1C->unk8][0].y0 = 0;
                    g_attractSprites[g_gameState->unk1C->unk8][0].u0 = 0;
                    g_attractSprites[g_gameState->unk1C->unk8][0].v0 = 0;
                    g_attractSprites[g_gameState->unk1C->unk8][0].w = 0xC0;
                    g_attractSprites[g_gameState->unk1C->unk8][0].h = 0xE0;
                    g_attractSprites[g_gameState->unk1C->unk8][0].clut = 0x3800;
                    addPrim(&g_gameState->unk1C->unk10[g_gameState->unk1C->unk8][8], &g_attractSprites[g_gameState->unk1C->unk8][0]);
                    setDrawTPage(&g_attractTpages[g_gameState->unk1C->unk8][0], 1, 1, 0x10A);
                    addPrim(&g_gameState->unk1C->unk10[g_gameState->unk1C->unk8][8], &g_attractTpages[g_gameState->unk1C->unk8][0]);
                    setlen((g_attractSprites[g_gameState->unk1C->unk8] + 1), 4);
                    (g_attractSprites[g_gameState->unk1C->unk8] + 1)->code = 0x64;
                    (g_attractSprites[g_gameState->unk1C->unk8] + 1)->r0 = 0x80;
                    (g_attractSprites[g_gameState->unk1C->unk8] + 1)->g0 = 0x80;
                    (g_attractSprites[g_gameState->unk1C->unk8] + 1)->b0 = 0x80;
                    (g_attractSprites[g_gameState->unk1C->unk8] + 1)->x0 = 0xC0;
                    (g_attractSprites[g_gameState->unk1C->unk8] + 1)->y0 = 0;
                    (g_attractSprites[g_gameState->unk1C->unk8] + 1)->u0 = 0;
                    (g_attractSprites[g_gameState->unk1C->unk8] + 1)->v0 = 0;
                    (g_attractSprites[g_gameState->unk1C->unk8] + 1)->w = 0x80;
                    (g_attractSprites[g_gameState->unk1C->unk8] + 1)->h = 0xE0;
                    (g_attractSprites[g_gameState->unk1C->unk8] + 1)->clut = 0x3800;
                    addPrim(&g_gameState->unk1C->unk10[g_gameState->unk1C->unk8][8], (g_attractSprites[g_gameState->unk1C->unk8] + 1));
                    setDrawTPage(g_attractTpages[g_gameState->unk1C->unk8] + 1, 1, 1, 0x10D);
                    addPrim(&g_gameState->unk1C->unk10[g_gameState->unk1C->unk8][8], (g_attractTpages[g_gameState->unk1C->unk8] + 1));
                    pad &= 0x4008;
                    btn = pad;
                    if (!(g_attractFlags.unk0 & 1) || btn) {
                        if (btn) {
                            if (timer >= 0x1F) {
                                timer = 0x1E;
                            }
                            g_attractSkipped = 1;
                            D_80073998[0] = -1;
                            g_attractFlags.unk0 |= 0x100;
                        }
                        if (!(g_attractFlags.unk0 & 1) && timer < 0xF1) {
                            func_800320B8(1, 1, timer, 0xFFFFFF);
                            g_attractFlags.unk0 |= 1;
                        }
                    }
                    timer--;
                } else {
                    timer = -1;
                    g_attractFlags.unk0 &= ~1;
                    if (g_attractFlags.unk0 & 0x100) {
                        i = 4;
                    } else {
                        i++;
                    }
                }
            } else {
                timer = 0x4EC;
                D_80073998[0] = 0;
                scene_setFadeScreen(g_attractSkipped);
                scene_set(1);
                g_attractState = 2;
            }
            break;
        case 2:
            scene_update();
            if (scene_get() == 5) {
                g_attractState = 4;
            }
            func_80032100();
            func_80023A0C();
            if (!(g_attractFlags.unk0 & 0x10) && (u8)scene_isTitleIdle() && timer < 0x5B) {
                func_800320B8(1, 1, 0x5A, 0xFFFFFF);
                if (g_gameState->unkB != 0xFF && (g_gameState->unkB & 1)) {
                    opening_soundCmd(0x8301, 0x7003A, 0x5A, 0, 0);
                }
                g_attractFlags.unk0 |= 0x10;
            }
            if (timer == 0) {
                if (g_attractState != 4) {
                    g_attractState = 3;
                }
                if (g_gameState->unkB != 0xFF && (g_gameState->unkB & 1)) {
                    opening_soundCmd(0x100, 0x7003A, 0, 0, 0);
                }
            }
            if ((g_attractFlags.unk0 & 0x10) || (u8)scene_isTitleIdle()) {
                timer--;
            }
            break;
        }
        func_80032098();
        opening_attractSwap();
    } while ((u32)g_attractState < 3);
    opening_attractFree(arg0);
    return g_attractState == 4;
}

s32 opening_attractInit(s32 idx) {
    u8 *buf;
    FileEntry *e;

    while (func_800217C4(g_gameState->unk1C->unk854) != 0) {
    }
    e = file_find(1, g_attractFiles[idx]);
    func_8002177C(g_gameState->unk1C->unk854, e->unk4, (e->unkC - e->unk4) << 11, D_80067944 + 0x10000, 0, -1, 0);
    while (func_800217C4(g_gameState->unk1C->unk854) != 0) {
    }
    buf = D_80067944 + 0x10000;
    g_attractData = buf;
    db_register((s32)buf, 0);
    func_800320B8(1, 1, 0, 0xFFFFFF);
    g_attractState = 0;
    g_attractFlags.unk0 = 0;
    D_80073998[0] = 0;
    return 0;
}

s32 opening_attractFree(s32 arg0) {
    db_unregister(g_attractData);
    g_attractData = NULL;
    return 0;
}

void opening_attractClearOt(void) {
    GameData *ctx = g_gameState->unk1C;
    ClearOTagR(ctx->unk10[ctx->unk8], 0x1000);
}

void opening_attractSwap(void) {
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
    func_8002C0DC(g_gameState->unk1C->unk8);
}
