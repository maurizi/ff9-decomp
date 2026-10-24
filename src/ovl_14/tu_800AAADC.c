#include "ovl_14.h"

void func_800AAADC(void) {
    RECT *r;
    s32 t;

    t = 0xD2;
    func_800AAF40();
    func_8001D910((void *)D_800AB2F4, NULL, &r);
    r->w *= 4;
    do {
        func_800AB09C();
        pad_update();
        if (t == 0xD2) {
            func_800320B8(1, 1, 0x3C, 0);
        } else if (t == 0x3C) {
            func_800320B8(1, 1, 0x3C, 0xFFFFFF);
        }
        setlen(&D_800AB448[g_gameState->unk1C->unk8], 4);
        D_800AB448[g_gameState->unk1C->unk8].code = 0x64;
        D_800AB448[g_gameState->unk1C->unk8].r0 = 0x80;
        D_800AB448[g_gameState->unk1C->unk8].g0 = 0x80;
        D_800AB448[g_gameState->unk1C->unk8].b0 = 0x80;
        D_800AB448[g_gameState->unk1C->unk8].x0 = (0x140 - r->w) / 2;
        D_800AB448[g_gameState->unk1C->unk8].y0 = (0xE0 - r->h) / 2;
        D_800AB448[g_gameState->unk1C->unk8].u0 = 0;
        D_800AB448[g_gameState->unk1C->unk8].v0 = 0;
        D_800AB448[g_gameState->unk1C->unk8].w = r->w;
        D_800AB448[g_gameState->unk1C->unk8].h = r->h;
        D_800AB448[g_gameState->unk1C->unk8].clut = 0x3800;
        addPrim(&g_gameState->unk1C->unk10[g_gameState->unk1C->unk8][8], &D_800AB448[g_gameState->unk1C->unk8]);
        setDrawTPage(&D_800AB470[g_gameState->unk1C->unk8], 1, 1, 0xA);
        addPrim(&g_gameState->unk1C->unk10[g_gameState->unk1C->unk8][8], &D_800AB470[g_gameState->unk1C->unk8]);
        t--;
        func_80032098();
        func_800AB0E0();
    } while (t > 0);
    func_800AB06C();
}

void func_800AAF40(void) {
    u8 *buf;
    FileEntry *e;

    while (func_800217C4(g_gameState->unk1C->unk854) != 0) {
    }
    e = file_find(1, 0x2718);
    func_8002177C(g_gameState->unk1C->unk854, e->unk4, (e->unkC - e->unk4) << 11, D_80067944 + 0x10000, 0, -1, 0);
    while (func_800217C4(g_gameState->unk1C->unk854) != 0) {
    }
    buf = D_80067944 + 0x10000;
    D_800AB2F0 = buf;
    db_register((s32)buf, 0);
    D_800AB2F4 = (s32)db_lookupRegistered(4, 0x44FB0, 0);
    func_8001D9CC(D_800AB2F4, 0, 0xE0, 0x280, 0);
    func_8001D8A8(D_800AB2F4);
    func_800320B8(1, 1, 0, 0xFFFFFF);
    g_padPortMask[0] = 0;
}

void func_800AB06C(void) {
    db_unregister(D_800AB2F0);
    D_800AB2F0 = NULL;
}

void func_800AB09C(void) {
    GameData *ctx = g_gameState->unk1C;
    ClearOTagR(ctx->unk10[ctx->unk8], 0x1000);
}

void func_800AB0E0(void) {
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

void func_800AB1E4(void) {
    g_gameState->unk1C->unk890 = 1;
}

void func_800AB200(void) {
    GameData *ctx = g_gameState->unk1C;
    if (ctx->unk890 == 0) {
        ctx->unk890 = 2;
    }
}
