#include "ovl_14.h"

void func_800A7088(void) {
    s32 n;

    n = 0;
    func_800A767C();
    func_800A7FF0();
    D_800AB300 = D_80067950 == 0 ? 6 : 0;
    while (D_800AB300 < 7) {
        switch (D_800AB300) {
        case 0:
            if (func_800A9220() != 0) {
                D_800AB300 = 7;
            } else {
                D_800AB300 = 1;
            }
            break;
        case 1:
            if (func_800A9E14(0) != 0) {
                D_800AB300 = 7;
            } else if (g_gameState->unkB != 0xFF && (g_gameState->unkB & 1)) {
                n = 180;
                D_800AB300 = 2;
            } else {
                D_800AB300 = 4;
            }
            break;
        case 2:
            VSync(0);
            if (--n == 0) {
                D_800AB300 = 3;
            }
            break;
        case 3:
            if (func_800A9220() == 0) {
                D_800AB300 = 4;
            } else {
                D_800AB300 = 7;
            }
            break;
        case 4:
            if (func_800A9E14(1) != 0) {
                D_800AB300 = 7;
            } else if (g_gameState->unkB != 0xFF && (g_gameState->unkB & 1)) {
                n = 180;
                D_800AB300 = 5;
            } else {
                D_800AB300 = 1;
            }
            break;
        case 5:
            VSync(0);
            if (--n == 0) {
                D_800AB300 = 0;
            }
            break;
        case 6:
            func_800AAADC();
            D_800AB300 = 0;
            break;
        }
    }
    g_gameState->unk0 |= 0x100000;
    func_8001DE50(g_gameState);
    func_800A741C();
    func_800A80D4();
    func_800A77BC();
}

void func_800A727C(void) {
    GameData *ctx = g_gameState->unk1C;
    ClearOTagR(ctx->unk10[ctx->unk8], 0x1000);
}

void func_800A72C0(void) {
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

void func_800A73B8(void) {
    GameData *ctx = g_gameState->unk1C;
    game_setClearColor(0, 0, 0);
    g_gameState->mode = 1;
    g_gameState->vsyncMode = 2;
    ctx->fieldId = 0;
    ctx->unk3A = 1;
}

/* looks up the current area/field ids in the two directories at D_80067944 + 0x14C700 and stores their sector ranges */
void func_800A741C(void) {
    GameState *gs = g_gameState;
    s32 mode = gs->mode;
    GameData *ctx = gs->unk1C;
    GameStateUnk14 *f;
    s32 base;
    u8 *area;
    u8 *start;
    Ovl14Dir *e;
    s32 key;
    s32 id;

    if (mode == 1 || mode == 3) {
        id = -1;
        f = gs->unk14;
        base = f->unk8;
        area = D_80067944 + ((base << 11) + 0x14C700);
        if (mode == 1) {
            key = (s16)ctx->fieldId;
            start = area + ((f->unk48 - base) << 11);
            e = (Ovl14Dir *)start + f->unk44 - 1;
        } else {
            key = ctx->unk7DC;
            start = area + ((f->unk38 - base) << 11);
            e = (Ovl14Dir *)start + f->unk34 - 1;
        }
        for (;;) {
            if (e->unk0 == key) {
                id = e->unk2;
                ctx->sceneSector[1] = e->unk4;
                ctx->sceneSectors[1] = e[1].unk4 - e->unk4;
                break;
            }
            e--;
            if ((u8 *)e < start) {
                break;
            }
        }
        start = area + ((gs->unk14->unk28 - base) << 11);
        e = (Ovl14Dir *)start + gs->unk14->unk24 - 1;
        for (;;) {
            if (e->unk0 == id) {
                ctx->sceneSector[0] = e->unk4;
                ctx->sceneSectors[0] = e[1].unk4 - e->unk4;
                break;
            }
            e--;
            if ((u8 *)e < start) {
                break;
            }
        }
        if (mode == 1) {
            ctx->unk3A = id;
        } else {
            ctx->unk7DE = id;
        }
    }
}
