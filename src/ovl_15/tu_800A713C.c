#include "ovl_15.h"

void ending_main(void) {
    ending_init();
    scene_init();
    g_endingState = 0;
    while (g_endingState < 5) {
        switch (g_endingState) {
        case 0:
            ending_playMovie(4, 0x12);
            g_endingState = 3;
            break;
        case 1:
            ending_messages();
            g_endingState = 2;
            break;
        case 2:
            ending_playMovie(4, 0x13);
            g_endingState = 3;
            break;
        case 3:
            if (ending_theEnd() == 1) {
                g_endingState = 4;
            } else {
                g_endingState = 3;
            }
            break;
        case 4:
            bj_main();
            g_endingState = 3;
            break;
        }
    }
    scene_exit();
    ending_exit();
}

void ending_clearOt(void) {
    GameData *ctx = g_gameState->unk1C;
    ClearOTagR(ctx->unk10[ctx->unk8], 0x1000);
}

void ending_swapBuffers(void) {
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
    func_8002C0DC(g_gameState->unk1C->unk8);
}
