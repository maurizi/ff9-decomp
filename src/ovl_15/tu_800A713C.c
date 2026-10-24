#include "ovl_15.h"

void ending_main(void) {
    ending_init();
    scene_init();
    g_endingState = ENDING_MOVIE;
    while (g_endingState < ENDING_DONE) {
        switch (g_endingState) {
        case ENDING_MOVIE:
            ending_playMovie(4, 0x12);
            g_endingState = ENDING_THE_END;
            break;
        case ENDING_MESSAGES:
            ending_messages();
            g_endingState = ENDING_MOVIE2;
            break;
        case ENDING_MOVIE2:
            ending_playMovie(4, 0x13);
            g_endingState = ENDING_THE_END;
            break;
        case ENDING_THE_END:
            if (ending_theEnd() == 1) {
                g_endingState = ENDING_BLACKJACK;
            } else {
                g_endingState = ENDING_THE_END;
            }
            break;
        case ENDING_BLACKJACK:
            bj_main();
            g_endingState = ENDING_THE_END;
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
