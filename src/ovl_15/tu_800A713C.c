#include "ovl_15.h"

void func_800A713C(void) {
    func_800ACB98();
    func_800ACE1C();
    D_800AE4B8 = 0;
    while (D_800AE4B8 < 5) {
        switch (D_800AE4B8) {
        case 0:
            func_800A7378(4, 0x12);
            D_800AE4B8 = 3;
            break;
        case 1:
            func_800AD7DC();
            D_800AE4B8 = 2;
            break;
        case 2:
            func_800A7378(4, 0x13);
            D_800AE4B8 = 3;
            break;
        case 3:
            if (func_800AB490() == 1) {
                D_800AE4B8 = 4;
            } else {
                D_800AE4B8 = 3;
            }
            break;
        case 4:
            func_800A9488();
            D_800AE4B8 = 3;
            break;
        }
    }
    func_800ACF00();
    func_800ACC08();
}

void func_800A723C(void) {
    GameData *ctx = g_gameState->unk1C;
    ClearOTagR(ctx->unk10[ctx->unk8], 0x1000);
}

void func_800A7280(void) {
    GameData *ctx;
    u8 i;

    DrawSync(0);
    VSync(g_gameState->vsyncMode);
    ResetGraph(1);
    PutDispEnv(&g_gameState->unkC[g_gameState->unk1C->unk8]);
    ctx = g_gameState->unk1C;
    i = ctx->unk8;
    DrawOTagEnv(ctx->unk10[i] + 0xFFF, &g_gameState->unk10[i]);
    g_gameState->unk1C->unk8 ^= 1;
    gfx_setBuffer(g_gameState->unk1C->unk8);
}
