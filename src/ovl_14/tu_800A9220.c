#include "ovl_14.h"

s32 func_800A9220(void) {
    u8 *buf;
    s32 timer;
    s32 i;
    s32 r;
    s32 end;

    buf = D_80067944 + 0x106700;
    timer = -1;
    func_800A9968();
    do {
        func_800A9B14();
        pad_update();
        switch (D_800AB3C8) {
        case 0:
            if (g_gameState->unkB != 0xFF && (g_gameState->unkB & 1)) {
                D_800AB2CC = 0;
                D_800AB2C4 = 1;
                D_800AB3C8 = 1;
                func_8002F884();
            } else {
                if (!(g_gameState->unk1C->unk0 & 0x20000000)) {
                    func_800A799C(0, 0x7003A, 0, 0, 0);
                    func_800A799C(0xC401, 0x7003A, 0x5A, 0, 0x7F);
                }
                D_800AB2CC = 1;
                D_800AB3C8 = 3;
                D_800AB2C4 |= 0x80;
                func_8002FAAC(1);
            }
            break;
        case 1:
            i = 1;
            if (g_gameState->unk1C->unk8 != 0) {
                i = 2;
            }
            D_800AB3B8[0][i - 1] = 7;
            D_800AB3B8[0][i] = 0x10;
            g_gameState->vsyncMode = 4;
            DrawSync(0);
            MBG_init(1, &buf);
            mbg_setRgb24(1);
            mbg_setEndCallback(func_800A9DD4);
            D_800AB2CE = g_mbgFrame;
            mbg_play(1, 0);
            D_800AB3C8 = 2;
            break;
        case 2:
            if ((D_800AB2C4 & 0x10) && (func_80012AD4() & 2)) {
                r = mbg_update(0);
                end = mbg_getLastFrame();
                if (r != 0) {
                    DrawOTag(g_gameState->unk1C->unk10[g_gameState->unk1C->unk8 ^ 1] + 0xFFF);
                } else if (timer == -1 && g_gameState->unk1C->unk8 == 0 && !(D_800AB2C4 & 0x20)) {
                    g_gameState->unkC[g_gameState->unk1C->unk8].disp.x = 0x1E0;
                }
                if ((g_mbgFrame >= end || D_800AB2CE >= g_mbgFrame || timer == 0) && !(D_800AB2C4 & 0x40)) {
                    if (timer == 0) {
                        mbg_stop();
                        func_80023018();
                        if (!(g_gameState->unk1C->unk0 & 0x20000000)) {
                            func_800A799C(0, 0x7003A, 0, 0, 0);
                            func_800A799C(0xC401, 0x7003A, 0x5A, 0, 0x7F);
                        }
                    }
                    if (D_800AB2CE >= g_mbgFrame) {
                        D_800AB2C4 &= ~7;
                        if (g_gameState->unk1C->unk8 == 0) {
                            g_gameState->unkC[g_gameState->unk1C->unk8].disp.x = 0x1E0;
                        }
                        printf("Fr: %d\n", D_800AB2CE);
                    } else {
                        D_800AB3B8[1][0] = 7;
                    }
                    D_800AB2C4 |= 0x40;
                }
                if (!(D_800AB2C4 & 0x100) && g_mbgFrame > 0) {
                    func_800A799C(0, 0x7003A, 0, 0, 0);
                    D_800AB2C4 |= 0x100;
                }
                if (!(D_800AB2C4 & 0x20) && (g_padPortPressed[0] & (PAD_START | PAD_CROSS)) && g_mbgFrame < end) {
                    D_800AB2CC = 1;
                    mbg_setRgb24(0);
                    D_800AB3B8[1][g_gameState->unk1C->unk8 != 0 ? 1 : 2] = 2;
                    timer = 6;
                    func_800320B8(1, 1, 0, 0);
                    func_800320B8(1, 1, timer, 0xFFFFFF);
                    D_80073998[0] = -1;
                    D_800AB2C4 |= 0x20;
                }
                D_800AB2CE = g_mbgFrame;
                if (!(D_800AB2C4 & 2) && timer != -1) {
                    timer--;
                }
            }
            break;
        case 3:
            if ((D_800AB2C4 & 0x80) && cd_getState() == 0) {
                g_gameState->vsyncMode = 0;
                g_gameState->unkC[0].disp.x = 0x140;
                timer = 0x4EC;
                g_gameState->unkC[1].disp.x = 0;
                D_80073998[0] = 0;
                func_800A82F4(D_800AB2CC);
                func_800A812C(1);
                D_800AB3C8 = 4;
            }
            break;
        case 4:
            if (!(D_800AB2C4 & 0x1000) && (u8)func_800A82B0() && timer < 0x5B) {
                func_800320B8(1, 1, 0, 0);
                func_800320B8(1, 1, 0x5A, 0xFFFFFF);
                D_800AB2C4 |= 0x1000;
            }
            if (timer == 0 && D_800AB3C8 != 6) {
                D_800AB3C8 = 5;
            }
            if ((D_800AB2C4 & 0x1000) || (u8)func_800A82B0()) {
                timer--;
            }
            break;
        }
        if (!(D_800AB2C4 & 1)) {
            func_800A808C();
            if (func_800A811C() == 5) {
                D_800AB3C8 = 6;
            }
            func_80032100();
            func_80023A0C();
        }
        if (!(D_800AB2C4 & 2)) {
            func_80032098();
        }
        func_800A9B58();
    } while ((u32)D_800AB3C8 < 5);
    return D_800AB3C8 == 6;
}

s32 func_800A9968(void) {
    FileEntry *f;
    s32 h;
    s32 i, j;

    while (func_800217C4(g_gameState->unk1C->unk854) != 0) {
    }
    f = file_find(0, 0x64);
    func_8002177C(g_gameState->unk1C->unk854, f->unk4, (f->unkC - f->unk4) << 11, D_80067944 + 0xF1700, 0, 0x15000, NULL);
    while (func_800217C4(g_gameState->unk1C->unk854) != 0) {
    }
    EnterCriticalSection();
    FlushCache();
    ExitCriticalSection();
    if (g_gameState->unkB != 0xFF && (g_gameState->unkB & 1)) {
        h = cd_findFile(*D_8019DC68);
        func_80022ED4(h);
        while (cd_getState() != 0) {
        }
        mbg_streamSetBaseSector(h);
    }
    func_800320B8(1, 1, 0, 0xFFFFFF);
    D_800AB3C8 = 0;
    D_800AB2C4 = 0;
    for (i = 1; i >= 0; i--) {
        for (j = 3; j >= 0; j--) {
            D_800AB3B8[i][j] = 0;
        }
    }
    D_800AB2C8 = 0;
    func_800A82F4(0);
    func_800A812C(1);
    D_80073998[0] = 0;
    return 0;
}

void func_800A9B14(void) {
    GameData *ctx = g_gameState->unk1C;
    ClearOTagR(ctx->unk10[ctx->unk8], 0x1000);
}

void func_800A9B58(void) {
    s32 i, j;
    GameData *g;

    DrawSync(0);
    if (func_80012AD4() & 2) {
        if (g_mbgFrame != 0 && g_mbgFrame == g_mbgFrame / 50 * 50) {
            g_gameState->vsyncMode = 3;
        } else {
            g_gameState->vsyncMode = 4;
        }
    } else {
        g_gameState->vsyncMode = 0;
    }
    VSync(g_gameState->vsyncMode);
    ResetGraph(1);
    PutDispEnv(&g_gameState->unkC[g_gameState->unk1C->unk8]);
    if (D_800AB2C4 & 4) {
        PutDrawEnv(&g_gameState->unk10[g_gameState->unk1C->unk8]);
    } else {
        g = g_gameState->unk1C;
        DrawOTagEnv(g->unk10[g->unk8] + 0xFFF, &g_gameState->unk10[g->unk8]);
    }
    g_gameState->unk1C->unk8 ^= 1;
    func_8002C0DC(g_gameState->unk1C->unk8);
    D_800AB2C4 |= D_800AB3B8[0][0];
    D_800AB2C4 &= ~D_800AB3B8[1][0];
    for (i = 0; i < 2; i++) {
        for (j = 0; j < 3; j++) {
            D_800AB3B8[i][j] = D_800AB3B8[i][j + 1];
        }
        D_800AB3B8[i][3] = 0;
    }
    D_800AB2C8++;
}

void func_800A9DD4(void) {
    mbg_shutdown();
    cd_cancel();
    D_800AB2C4 |= 0x80;
    D_800AB3C8 = 3;
}
