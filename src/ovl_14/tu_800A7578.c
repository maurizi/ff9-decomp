#include "ovl_14.h"

void func_800A7578(void) {
    func_800651D8();
    evt_clearGlobalVars();
    func_8005392C();
    menu_initNewGame();
    func_800AB1E4();
    card_initAll();
    func_800A75C8();
}

void func_800A75C8(void) {
    func_800A7600();
    func_8004BBF0();
    func_800AB200();
    func_8006237C();
}

void func_800A7600(void) {
    s32 i;
    GameData *ctx = g_gameState->unk1C;

    ctx->unk9 = 0;
    ctx->unk39 = 0;
    ctx->unk38 = 0;
    ctx->unk7E8 = 0xFFFF;
    ctx->unk7EA = 0xFFFF;
    ctx->unk840 = 0xFFFF;
    ctx->unk850 = 0;
    ctx->unk8AC = -1;
    ctx->unk8B0 = 0xFFFF;
    for (i = 1; i >= 0; i--) {
        ctx->unk8B4[i] = 0xFFFF;
    }
    for (i = 1; i >= 0; i--) {
        ctx->returnSector[i] = -1;
        ctx->returnSectors[i] = -1;
    }
}

void func_800A767C(void) {
    s32 i;

    for (i = 1; i >= 0; i--) {
        g_padActuator[i].unk1 = 0;
        g_padActuator[i].unk0 = 0;
    }
    func_80055D40();
    func_80055D60();
    if (g_gameState->unk1C->unk0 & 0x100000) {
        snd_init();
        func_800A7768();
        g_gameState->unk1C->unk0 &= ~0x100000;
    }
    cd_checkShellOpen();
    g_gameState->unk1C->unk83C = func_800A799C;
    g_gameState->unk1C->unk84C = 0;
    func_800A78C0();
    func_800A780C();
    func_800A796C();
    game_initGfx();
    SetDispMask(1);
}

void func_800A7768(void) {
    s32 i;
    GameData *ctx = g_gameState->unk1C;

    for (i = 1; i >= 0; i--) {
        ctx->unk81C[i] = 0xFFFF;
        ctx->unk820[i] = 0xFFFF;
        ctx->unk824[i] = 0;
        ctx->unk826[i] = 0x7F;
    }
    ctx->unk0 &= 0xBFFFFF;
}

void func_800A77BC(void) {
    GameData *ctx;

    SetDispMask(0);
    ctx = g_gameState->unk1C;
    if (ctx->unk0 & 0x20000000) {
        ctx->unk0 |= 0x400000;
    }
}

void func_800A780C(void) {
    s32 a;
    s32 b;

    a = (s32)db_lookupRegistered(4, 0x7003A, 0);
    memmove(g_gameState->unk1C->unk814, (void *)a, (s32)db_lookupRegistered(5, 0x7003A, 0));
    a = (s32)db_lookupRegistered(4, 0x90122, 0);
    b = (s32)db_lookupRegistered(5, 0x90122, 0);
    func_80055DF4();
    func_80055E14(a, b, 1);
}

void func_800A78C0(void) {
    u8 *buf;
    FileEntry *e;

    buf = D_80067944 + 0x10000;
    e = file_find(1, 0x2717);
    cdRead_init(g_gameState->unk1C->unk854, e->unk4, (e->unkC - e->unk4) << 11, buf, 0, -1, 0);
    while (cdRead_update(g_gameState->unk1C->unk854) != 0) {
    }
    D_800AB230 = buf;
    db_register((s32)buf, 0);
}

void func_800A796C(void) {
    db_unregister(D_800AB230);
    D_800AB230 = NULL;
}

s32 func_800A799C(s32 arg0, s32 arg1, s32 arg2, s32 arg3, s32 arg4) {
    GameData *g;
    s32 kind;
    s32 h;
    s32 id;

    kind = (arg0 >> 8) & 0xF;
    switch ((arg0 >> 12) & 3) {
    case 0:
        id = (arg1 & 0xFFFF) | 0x70000;
        switch (kind) {
        case 0:
            if (g_gameState->unk1C->unk851 != 0) {
                return -1;
            }
            if (D_800AB234 == -1) {
                D_800AB234 = kind;
            }
            if (g_gameState->unk1C->unk81C[0] != arg1) {
                if (g_gameState->unk1C->unk0 & 0x20000000) {
                    akao_stopMusic(g_gameState->unk1C->unk824[0]);
                }
                g_gameState->unk1C->unk0 &= 0xCFFFFFFF;
                g_gameState->unk1C->unk820[0] = 0x122;
                g_gameState->unk1C->unk81C[0] = arg1;
                g_gameState->unk1C->unk826[0] = 0x7F;
            }
            g_gameState->unk1C->unk824[0] = akao_playMusic(g_gameState->unk1C->unk814);
            g = g_gameState->unk1C;
            if (!(g->unk0 & 0x20000000)) {
                func_80055A34(g->unk824[0], 0xA, 0, g->unk826[0]);
            }
            D_800AB234 = -1;
            g_gameState->unk1C->unk0 |= 0x30000000;
            return 0;
        case 1:
            if (g_gameState->unk1C->unk81C[0] == (u16)id) {
                akao_stopMusic(g_gameState->unk1C->unk824[0]);
                g_gameState->unk1C->unk826[0] = 0x7F;
                g_gameState->unk1C->unk0 &= 0xDFFFFFFF;
            }
            return 0;
        case 2:
            if ((arg0 & 0x3F) != 1) {
                break;
            }
            func_800559C8(g_gameState->unk1C->unk824[0], arg2);
            g_gameState->unk1C->unk826[0] = arg2;
            return 0;
        case 3:
            if ((arg0 & 0x3F) != 1) {
                break;
            }
            func_800559FC(g_gameState->unk1C->unk824[0], arg2, arg3);
            func_800A7F70(arg3, arg2);
            return 0;
        case 4:
            if ((arg0 & 0x3F) != 1) {
                break;
            }
            func_80055A34(g_gameState->unk1C->unk824[0], arg2, arg3, arg4);
            func_800A7F70(arg4, arg2);
            return 0;
        case 6:
            if ((arg0 & 0x3F) != 1) {
                break;
            }
            func_800559FC(0, arg2, arg3);
            func_800A7F70(arg3, arg2);
            return 0;
        case 11:
            if ((arg0 & 0x3F) != 1) {
                break;
            }
            func_80055A34(0, arg2, arg3, arg4);
            func_800A7F70(arg4, arg2);
            return 0;
        }
        return -1;
    case 1:
        id = (arg1 & 0xFFFF) | 0x80000;
        h = 0;
        if (kind != 5 && kind != 6) {
            h = (s32)db_lookupRegistered(4, id, 0);
        }
        switch (kind) {
        case 0:
            akao_playSe(h, arg2, arg3, arg4);
            return 0;
        case 1:
            akao_stopSe(h, arg2);
            return 0;
        case 2:
            switch (arg0 & 0x3F) {
            case 1:
                func_80055764(h, arg2, arg3);
                return 0;
            case 5:
                func_80055850(h, arg2, arg3);
                return 0;
            case 3:
                func_8005593C(h, arg2, arg3);
                return 0;
            }
            return -1;
        case 3:
            switch (arg0 & 0x3F) {
            case 1:
                func_800557A8(h, arg2, arg3, arg4);
                return 0;
            case 5:
                func_80055894(h, arg2, arg3, arg4);
                return 0;
            case 3:
                func_80055980(h, arg2, arg3, arg4);
                return 0;
            }
            return -1;
        case 5:
            switch (arg0 & 0x3F) {
            case 1:
                func_80055704(arg2);
                return 0;
            case 5:
                func_800557F0(arg2);
                return 0;
            case 3:
                func_800558DC(arg2);
                return 0;
            }
            return -1;
        case 6:
            switch (arg0 & 0x3F) {
            case 1:
                func_80055730(arg2, arg3);
                return -1;
            case 5:
                func_8005581C(arg2, arg3);
                return -1;
            case 3:
                func_80055908(arg2, arg3);
                return -1;
            }
            return -1;
        }
        return -1;
    }
    return -1;
}

void func_800A7F70(s32 arg0, s32 arg1) {
    GameData *ctx = g_gameState->unk1C;

    ctx->unk826[0] = arg0;
    if (arg0 == 0) {
        ctx->unk842 = arg1;
        ctx->unk8C4 = VSync(-1);
    }
}

s32 func_800A7FB0(void) {
    GameData *ctx = g_gameState->unk1C;
    return (u32)(VSync(-1) - ctx->unk8C4) < ctx->unk842;
}
