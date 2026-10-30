#include "ff9.h"

/* __main and __do_global_dtors (SN startup) walk an empty constructor list
 * that starts at the beginning of .rodata, where main's jump table sits. */
__asm__(".section .rodata\n.globl jtbl_80010000\njtbl_80010000:\n.section .text");

void game_main(void) {
    __main();
    game_initLibraries();
    for (;;) {
        game_reset();
        game_initState();
        game_initData();
        do {
            switch (g_gameState->mode) {
            case 0:
                game_loadModeOverlay(0);
                opening_main();
                break;
            case 1:
                game_loadModeOverlay(1);
                fld_entry();
                break;
            case 2:
                game_loadModeOverlay(2);
                btl_entry();
                break;
            case 8:
                game_loadModeOverlay(8);
                battleEnd_main();
                break;
            case 3:
                game_loadModeOverlay(3);
                wm_entry();
                break;
            case 9:
                game_loadModeOverlay(9);
                tm_main();
                break;
            case 7:
                game_loadModeOverlay(7);
                fld_gameOver();
                break;
            case 4:
                game_loadModeOverlay(4);
                ending_main();
                break;
            }
        } while (!(g_gameState->flags & GS_RESET));
        game_quitData();
        game_quitState();
        game_shutdown();
    }
}

void game_loadModeOverlay(s32 mode) {
    u32 loaded = 0;
    u8 files[10] = { 20, 10, 11, 12, 21, 0xFF, 0xFF, 101, 101, 13 };
    LoadAddrs addrs = D_80010034;
    s32 id;
    PreloadEntry *e;
    FileEntry *f;

    game_freezeScreen();
    id = files[mode] | 0x10000;
    while (cd_getState()) {
    }
    if (preload_isReady(id)) {
        e = preload_find(id);
        if (preload_isInVram(e)) {
            StoreImage((RECT *)&e->vramX, addrs.unk0[mode]);
        } else if ((s32)addrs.unk0[mode] != e->dest) {
            file_copyViaVram((s32)addrs.unk0[mode], e->dest, e->loaded, NULL);
        }
        loaded = e->loaded >> 11;
        preload_remove(id);
        DrawSync(0);
    }
    f = file_find(0, id);
    if (loaded < f->unkC - f->sector) {
        cd_readSync(g_gameState->imgSector + f->sector + loaded, (f->unkC - (f->sector + loaded)) << 11,
                      (u8 *)addrs.unk0[mode] + (loaded << 11));
    }
    EnterCriticalSection();
    FlushCache();
    ExitCriticalSection();
}

INCLUDE_RODATA("asm/us/main/nonmatchings/main", D_80010034);

s32 mbg_getStatus(void) {
    s32 r = 0;

    if (g_mbgState == 1) {
        return 1;
    }
    if (g_mbgState >= 2) {
        r = 1;
        if (g_mbgFrame > 0) {
            r = 3;
        }
        if (g_mbgPaused != 0) {
            r |= 0x10;
        }
        if (g_mbgRgb24 != 0) {
            r |= 4;
        }
        if (D_80071E30 == 1) {
            r |= 8;
        }
    }
    return r;
}

s8 mbg_getState(void) {
    return g_mbgState;
}

