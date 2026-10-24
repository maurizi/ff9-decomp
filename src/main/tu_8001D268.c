#include "ff9.h"

void game_initData(void) {
    GameData *gd;

    g_allocTop = g_gameState->unk4;
    game_allocData();
    gd = g_gameState->unk1C;
    game_resetData();
    PutDispEnv(&g_gameState->unkC[gd->unk8]);
    preload_init();
    game_loadCommonDb();
    game_loadCommonAkao();
    game_loadCommonImages();
    gd->unk4 = g_allocTop;
}

void game_quitData(void) {
    game_unloadCommonDb();
}

void game_allocData(void) {
    GameData *gd;
    u8 *q;
    s32 i;
    s32 w;

    g_gameState->unk1C = (GameData *)g_allocTop;
    gd = g_gameState->unk1C;
    q = (u8 *)(gd + 1);
    for (i = 0; i < 2; i++) {
        gd->unk10[i] = (u32 *)(q + 0x20);
        q += 0x4040;
    }
    gd->unk8D8 = (void *)(q + 0x558);
    gd->unk8D4 = (void *)q;
    gd->unk7E0 = (s32)(q + 0x60C);
    g_allocTop = gd->unk7E0 + 0x8000;
    gd->unk8C8 = (void *)g_allocTop;
    g_allocTop += 0x120;
    gd->unk814 = (void *)g_allocTop;
    w = g_allocTop + 0x6800;
    gd->unk818 = w;
    gd->unk828 = (void *)(w + 0x2000);
    gd->unk82C = (void *)(w + 0x2800);
    g_allocTop = w + 0x3000;
}

void game_resetData(void) {
    GameData *gd = g_gameState->unk1C;
    s32 i;

    gd->unkA = 0x200;
    gd->unk18.m[0][0] = gd->unk18.m[1][1] = gd->unk18.m[2][2] = 0x1000;
    gd->unk3A = 1;
    gd->battleId = 0x235;
    gd->unk848 = -1;
    gd->unk7DC = 0x2331;
    gd->unk7DE = 0x44;
    gd->unk0 = 0;
    gd->unk4 = 0;
    gd->unk8 = 0;
    gd->unk9 = 0;
    gd->unk38 = 0;
    gd->unk39 = 0;
    gd->unk18.t[0] = gd->unk18.t[1] = gd->unk18.t[2] = 0;
    gd->unk18.m[0][1] = gd->unk18.m[0][2] = gd->unk18.m[1][0] = gd->unk18.m[1][2] = gd->unk18.m[2][0] =
        gd->unk18.m[2][1] = 0;
    gd->fieldId = 0;
    gd->unk7D4 = NULL;
    gd->unk84C = NULL;
    gd->unk80C = NULL;
    gd->unk810 = NULL;
    gd->unk844 = 0;
    gd->unk851 = 0;
    gd->unk8C4 = 0;
    gd->unk852 = 0xFFFF;
    for (i = 1; i >= 0; i--) {
        gd->unk81C[i] = 0xFFFF;
        gd->unk820[i] = 0xFFFF;
        gd->unk824[i] = 0;
        gd->unk826[i] = 0x7F;
    }
    gd->unk842 = 0x1E;
    gd->unk7E8 = 0xFFFF;
    gd->unk840 = 0xFFFF;
    gd->unk7EA = 0xFFFF;
    gd->unk850 = 0;
    gd->unk8A8 = -1;
    gd->unk84A = 0xFFFF;
    gd->unk8AC = -1;
    gd->unk8B0 = 0xFFFF;
    for (i = 1; i >= 0; i--) {
        gd->unk830[i] = 0xFFFF;
        (&gd->unk834)[i] = 0xFFFF;
        gd->unk8B8[i] = 0x7F;
        gd->unk8BA[i] = 0x80;
        gd->unk8B4[i] = 0xFFFF;
    }
    gd->unk838 = NULL;
    gd->unk83C = NULL;
    for (i = 8; i >= 0; i--) {
        func_8001E3A4(&gd->unk8D8[i]);
        gd->unk8D8[i].unk8 = (struct BtlUnit9C *)&gd->unk8D4[i];
    }
    gd->unk8DC = NULL;
    gd->unk8E0 = NULL;
    gd->unk7D8 = NULL;
    for (i = 1; i >= 0; i--) {
        gd->returnSector[i] = -1;
        gd->returnSectors[i] = -1;
        gd->sceneSector[i] = -1;
        gd->sceneSectors[i] = -1;
    }
    gd->depTable = (struct FileTable *)-1;
    gd->unk8C0 = 0;
    gd->unk878 = 0;
    gd->unk87C = 0;
    memset(&gd->unk880, 0, 0x28);
    gd->unk8B2 = 0;
    gd->unk8BC = NULL;
    gd->unk8E4.ovl65 = NULL;
    for (i = 0; i >= 0; i--) {
        gd->unk849[i] = 0;
    }
    for (i = 7; i >= 0; i--) {
        gd->unk8CC[i] = 0;
    }
}

s32 game_loadCommonAkao(void) {
    s32 addr = (s32)db_lookupRegistered(4, 0x90084, 0);
    s32 size = (s32)db_lookupRegistered(5, 0x90084, 0);

    akao_beginBankUpload();
    akao_uploadBank(addr, size, 1);
    return 0;
}

s32 game_loadCommonImages(void) {
    GameData *gd = g_gameState->unk1C;
    u16 ids[8] = { 0, 0x2710, 0x2711, 0x2712, 0x2713, 0x2714, 0x2715, 0x2716 };
    u32 i;
    s32 addr;

    gd->unk7D4 = db_lookupRegistered(4, 0xD0000, 0);
    for (i = 0; i < 8; i++) {
        addr = (s32)db_lookupRegistered(4, ids[i] | 0x40000, 0);
        if (addr != 0) {
            file_loadTim(addr);
            DrawSync(0);
        }
    }
    return 0;
}

s32 game_loadCommonDb(void) {
    FileEntry *f = file_find(1, 0x271A);

    cdRead_init(g_gameState->unk1C->unk854, f->unk4, (f->unkC - f->unk4) << 11, (void *)g_allocTop, 0,
                  -1, NULL);
    while (cdRead_update(g_gameState->unk1C->unk854)) {
    }
    g_gameState->unk1C->unk7D8 = (void *)g_allocTop;
    db_register((s32)g_gameState->unk1C->unk7D8, 0);
    g_allocTop = db_lookup((s32)g_gameState->unk1C->unk7D8, 0, 0x90000, 0);
    return 0;
}

s32 game_unloadCommonDb(void) {
    db_unregister(g_gameState->unk1C->unk7D8);
    g_gameState->unk1C->unk7D8 = 0;
    return 0;
}

