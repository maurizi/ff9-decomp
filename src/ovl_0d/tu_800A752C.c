#include "ovl_0d.h"

/* This file's .data: linked right after .text (config/ovl_0d.yaml). */
s32 g_tmDataFile = 0;

void tm_main(void) {
    tm_init();
    tm_mainLoop();
    tm_exit();
}

void tm_init(void) {
    g_tmDataFile = 0;
    g_tmDataCopy = 0;
    g_gameState->unk1C->unk83C = tm_soundCmd;
    preload_init();
    tm_loadDataFile();
    tm_loadSoundBlocks();
    tm_loadMusic();
    tm_soundCmd(0, 0x42, 0, 0, 0);
    tm_copyDataFile();
    tm_freeDataFile();
}

void tm_exit(void) {
    tm_freeDataCopy();
    tm_soundCmd(0x100, 0x42, 0, 0, 0);
    g_gameState->unkA = 9;
    g_gameState->mode = 1;
}

s32 tm_loadSoundBlocks(void) {
    s32 i;
    s32 h;
    s32 a, b;
    for (i = db_lookup(g_tmDataFile, 1, 0x90000, 0) - 1; i >= 0; i--) {
        h = db_lookup(g_tmDataFile, 3, (i & 0xFFFF) | 0x90000, 0);
        a = (s32)db_lookupRegistered(4, h, 0);
        b = (s32)db_lookupRegistered(5, h, 0);
        func_80055DF4();
        func_80055E14(a, b, 1);
    }
    return 0;
}

s32 tm_loadMusic(void) {
    s32 p = (s32)db_lookupRegistered(4, 0x70042, 0);
    memmove(g_gameState->unk1C->unk814, (void *)p, (s32)db_lookupRegistered(5, 0x70042, 0));
    return 0;
}

s32 tm_loadDataFile(void) {
    s32 dst = (s32)(D_80067944 + 0x57800);
    FileEntry *f = file_find(1, 0x2715);
    func_8002177C(g_gameState->unk1C->unk854, f->unk4, (f->unkC - f->unk4) << 11, (void *)dst, 0, -1, 0);
    while (func_800217C4(g_gameState->unk1C->unk854) != 0) {
    }
    g_tmDataFile = dst;
    db_register(dst, 0);
    return 0;
}

s32 tm_copyDataFile(void) {
    s32 end = db_lookup(g_tmDataFile, 0, 0x70000, 0);
    g_tmDataCopy = (s32)(D_80067944 + 0x141000);
    memmove((void *)g_tmDataCopy, (void *)g_tmDataFile, end - g_tmDataFile);
    db_register(g_tmDataCopy, 0);
    return 0;
}

s32 tm_freeDataFile(void) {
    db_unregister((void *)g_tmDataFile);
    g_tmDataFile = 0;
    return 0;
}

s32 tm_freeDataCopy(void) {
    db_unregister((void *)g_tmDataCopy);
    g_tmDataCopy = 0;
    return 0;
}

s32 tm_soundCmd(s32 arg0, s32 arg1, s32 arg2, s32 arg3, s32 arg4) {
    GameData *g;
    s32 kind;
    s32 h;
    s32 id;

    kind = (arg0 >> 8) & 0xF;
    switch ((arg0 >> 12) & 3) {
    case 0:
        switch (kind) {
        case 0:
            if (g_gameState->unk1C->unk851 != 0) {
                return -1;
            }
            if (D_800B3664 == -1) {
                D_800B3664 = kind;
            }
            if (g_gameState->unk1C->unk81C[0] != arg1) {
                if (g_gameState->unk1C->unk0 & 0x20000000) {
                    func_800552B8(g_gameState->unk1C->unk824[0]);
                }
                g_gameState->unk1C->unk0 &= 0xCFFFFFFF;
                g_gameState->unk1C->unk820[0] = 0xE2;
                g_gameState->unk1C->unk81C[0] = arg1;
                g_gameState->unk1C->unk826[0] = 0x7F;
            }
            g_gameState->unk1C->unk824[0] = func_80055290(g_gameState->unk1C->unk814);
            g = g_gameState->unk1C;
            if (!(g->unk0 & 0x20000000)) {
                func_80055A34(g->unk824[0], 0xA, 0, g->unk826[0]);
            }
            D_800B3664 = -1;
            g_gameState->unk1C->unk0 |= 0x30000000;
            return 0;
        case 1:
            func_800552B8(g_gameState->unk1C->unk824[0]);
            g_gameState->unk1C->unk826[0] = 0x7F;
            g_gameState->unk1C->unk0 &= 0xDFFFFFFF;
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
            g_gameState->unk1C->unk826[0] = arg3;
            return 0;
        case 4:
            if ((arg0 & 0x3F) != 1) {
                break;
            }
            func_80055A34(g_gameState->unk1C->unk824[0], arg2, arg3, arg4);
            g_gameState->unk1C->unk826[0] = arg4;
            return 0;
        case 6:
            if ((arg0 & 0x3F) != 1) {
                break;
            }
            func_800559FC(0, arg2, arg3);
            g_gameState->unk1C->unk826[0] = arg3;
            return 0;
        case 11:
            if ((arg0 & 0x3F) != 1) {
                break;
            }
            func_80055A34(0, arg2, arg3, arg4);
            g_gameState->unk1C->unk826[0] = arg4;
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
            func_800553EC(h, arg2, arg3, arg4);
            return 0;
        case 1:
            func_8005546C(h, arg2);
            return 0;
        case 2:
            switch (arg0 & 0x3F) {
            case 1:
                func_80055764(h, arg2, arg3);
                return 0;
            case 5:
                func_80055850(h, arg2, arg3);
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
            }
            return -1;
        }
        return -1;
    }
    return -1;
}
