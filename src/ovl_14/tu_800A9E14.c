#include "ovl_14.h"

s32 func_800A9E14(s32 arg0) {
    s32 timer;
    s32 i;
    s32 tim;
    s32 pad;
    s32 id;
    s32 k;
    s32 btn;

    i = 0;
    timer = -1;
    func_800AA848(arg0);
    do {
        func_800AA994();
        pad_update();
        switch (D_800AB440) {
        case 0:
            i = 0;
            timer = -1;
            D_800AB440 = 1;
            D_800AB2EC = 1;
            break;
        case 1:
            if (i < 4) {
                k = arg0 * 4 + i;
                id = D_800AB2DC[k] | 0x40000;
                if (timer == -1) {
                    tim = (s32)db_lookupRegistered(4, id, 0);
                    file_setTimPos(tim, 0, 0xE0, 0x280, 0);
                    file_loadTim(tim);
                    timer = 0x3A2;
                    func_800320B8(1, 1, 0xF0, 0);
                } else if (timer != 0) {
                    pad = g_padPortPressed[0];
                    setlen(&D_800AB3D0[g_gameState->unk1C->unk8][0], 4);
                    D_800AB3D0[g_gameState->unk1C->unk8][0].code = 0x64;
                    D_800AB3D0[g_gameState->unk1C->unk8][0].r0 = 0x80;
                    D_800AB3D0[g_gameState->unk1C->unk8][0].g0 = 0x80;
                    D_800AB3D0[g_gameState->unk1C->unk8][0].b0 = 0x80;
                    D_800AB3D0[g_gameState->unk1C->unk8][0].x0 = 0;
                    D_800AB3D0[g_gameState->unk1C->unk8][0].y0 = 0;
                    D_800AB3D0[g_gameState->unk1C->unk8][0].u0 = 0;
                    D_800AB3D0[g_gameState->unk1C->unk8][0].v0 = 0;
                    D_800AB3D0[g_gameState->unk1C->unk8][0].w = 0xC0;
                    D_800AB3D0[g_gameState->unk1C->unk8][0].h = 0xE0;
                    D_800AB3D0[g_gameState->unk1C->unk8][0].clut = 0x3800;
                    addPrim(&g_gameState->unk1C->unk10[g_gameState->unk1C->unk8][8], &D_800AB3D0[g_gameState->unk1C->unk8][0]);
                    setDrawTPage(&D_800AB420[g_gameState->unk1C->unk8][0], 1, 1, 0x10A);
                    addPrim(&g_gameState->unk1C->unk10[g_gameState->unk1C->unk8][8], &D_800AB420[g_gameState->unk1C->unk8][0]);
                    setlen((D_800AB3D0[g_gameState->unk1C->unk8] + 1), 4);
                    (D_800AB3D0[g_gameState->unk1C->unk8] + 1)->code = 0x64;
                    (D_800AB3D0[g_gameState->unk1C->unk8] + 1)->r0 = 0x80;
                    (D_800AB3D0[g_gameState->unk1C->unk8] + 1)->g0 = 0x80;
                    (D_800AB3D0[g_gameState->unk1C->unk8] + 1)->b0 = 0x80;
                    (D_800AB3D0[g_gameState->unk1C->unk8] + 1)->x0 = 0xC0;
                    (D_800AB3D0[g_gameState->unk1C->unk8] + 1)->y0 = 0;
                    (D_800AB3D0[g_gameState->unk1C->unk8] + 1)->u0 = 0;
                    (D_800AB3D0[g_gameState->unk1C->unk8] + 1)->v0 = 0;
                    (D_800AB3D0[g_gameState->unk1C->unk8] + 1)->w = 0x80;
                    (D_800AB3D0[g_gameState->unk1C->unk8] + 1)->h = 0xE0;
                    (D_800AB3D0[g_gameState->unk1C->unk8] + 1)->clut = 0x3800;
                    addPrim(&g_gameState->unk1C->unk10[g_gameState->unk1C->unk8][8], (D_800AB3D0[g_gameState->unk1C->unk8] + 1));
                    setDrawTPage(D_800AB420[g_gameState->unk1C->unk8] + 1, 1, 1, 0x10D);
                    addPrim(&g_gameState->unk1C->unk10[g_gameState->unk1C->unk8][8], (D_800AB420[g_gameState->unk1C->unk8] + 1));
                    pad &= 0x4008;
                    btn = pad;
                    if (!(D_800AB2D0.unk0 & 1) || btn) {
                        if (btn) {
                            if (timer >= 0x1F) {
                                timer = 0x1E;
                            }
                            D_800AB2EC = 1;
                            g_padPortMask[0] = -1;
                            D_800AB2D0.unk0 |= 0x100;
                        }
                        if (!(D_800AB2D0.unk0 & 1) && timer < 0xF1) {
                            func_800320B8(1, 1, timer, 0xFFFFFF);
                            D_800AB2D0.unk0 |= 1;
                        }
                    }
                    timer--;
                } else {
                    timer = -1;
                    D_800AB2D0.unk0 &= ~1;
                    if (D_800AB2D0.unk0 & 0x100) {
                        i = 4;
                    } else {
                        i++;
                    }
                }
            } else {
                timer = 0x4EC;
                g_padPortMask[0] = 0;
                func_800A82F4(D_800AB2EC);
                func_800A812C(1);
                D_800AB440 = 2;
            }
            break;
        case 2:
            func_800A808C();
            if (func_800A811C() == 5) {
                D_800AB440 = 4;
            }
            func_80032100();
            game_updateWindows();
            if (!(D_800AB2D0.unk0 & 0x10) && (u8)func_800A82B0() && timer < 0x5B) {
                func_800320B8(1, 1, 0x5A, 0xFFFFFF);
                if (g_gameState->unkB != 0xFF && (g_gameState->unkB & 1)) {
                    func_800A799C(0x8301, 0x7003A, 0x5A, 0, 0);
                }
                D_800AB2D0.unk0 |= 0x10;
            }
            if (timer == 0) {
                if (D_800AB440 != 4) {
                    D_800AB440 = 3;
                }
                if (g_gameState->unkB != 0xFF && (g_gameState->unkB & 1)) {
                    func_800A799C(0x100, 0x7003A, 0, 0, 0);
                }
            }
            if ((D_800AB2D0.unk0 & 0x10) || (u8)func_800A82B0()) {
                timer--;
            }
            break;
        }
        func_80032098();
        func_800AA9D8();
    } while ((u32)D_800AB440 < 3);
    func_800AA960(arg0);
    return D_800AB440 == 4;
}

s32 func_800AA848(s32 idx) {
    u8 *buf;
    FileEntry *e;

    while (cdRead_update(g_gameState->unk1C->unk854) != 0) {
    }
    e = file_find(1, D_800AB2D8[idx]);
    cdRead_init(g_gameState->unk1C->unk854, e->unk4, (e->unkC - e->unk4) << 11, D_80067944 + 0x10000, 0, -1, 0);
    while (cdRead_update(g_gameState->unk1C->unk854) != 0) {
    }
    buf = D_80067944 + 0x10000;
    D_800AB2D4 = buf;
    db_register((s32)buf, 0);
    func_800320B8(1, 1, 0, 0xFFFFFF);
    D_800AB440 = 0;
    D_800AB2D0.unk0 = 0;
    g_padPortMask[0] = 0;
    return 0;
}

s32 func_800AA960(s32 arg0) {
    db_unregister(D_800AB2D4);
    D_800AB2D4 = NULL;
    return 0;
}

void func_800AA994(void) {
    GameData *ctx = g_gameState->unk1C;
    ClearOTagR(ctx->unk10[ctx->unk8], 0x1000);
}

void func_800AA9D8(void) {
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
