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
            case GS_MODE_OPENING:
                game_loadModeOverlay(GS_MODE_OPENING);
                opening_main();
                break;
            case GS_MODE_FIELD:
                game_loadModeOverlay(GS_MODE_FIELD);
                fld_entry();
                break;
            case GS_MODE_BATTLE:
                game_loadModeOverlay(GS_MODE_BATTLE);
                btl_entry();
                break;
            case GS_MODE_BATTLE_END:
                game_loadModeOverlay(GS_MODE_BATTLE_END);
                battleEnd_main();
                break;
            case GS_MODE_WORLD:
                game_loadModeOverlay(GS_MODE_WORLD);
                wm_entry();
                break;
            case GS_MODE_TETRA_MASTER:
                game_loadModeOverlay(GS_MODE_TETRA_MASTER);
                tm_main();
                break;
            case GS_MODE_GAME_OVER:
                game_loadModeOverlay(GS_MODE_GAME_OVER);
                fld_gameOver();
                break;
            case GS_MODE_ENDING:
                game_loadModeOverlay(GS_MODE_ENDING);
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
    u8 files[10] = { OVL_OPENING, OVL_FIELD, OVL_BATTLE, OVL_WORLD, OVL_ENDING, 0xFF, 0xFF, OVL_BATTLE_END, OVL_BATTLE_END, OVL_TETRA_MASTER };
    LoadAddrs addrs = g_modeOverlayAddrs;
    s32 id;
    PreloadEntry *e;
    FileEntry *f;

    game_freezeScreen();
    id = OVL_KEY(files[mode]);
    while (cd_getState()) {
    }
    if (preload_isReady(id)) {
        e = preload_find(id);
        if (preload_isInVram(e)) {
            StoreImage((RECT *)&e->vramX, addrs.addr[mode]);
        } else if ((s32)addrs.addr[mode] != e->dest) {
            file_copyViaVram((s32)addrs.addr[mode], e->dest, e->loaded, NULL);
        }
        loaded = e->loaded >> 11;
        preload_remove(id);
        DrawSync(0);
    }
    f = file_find(DIR_OVERLAYS, id);
    if (loaded < f->link - f->sector) {
        cd_readSync(g_gameState->imgSector + f->sector + loaded, (f->link - (f->sector + loaded)) << 11,
                      (u8 *)addrs.addr[mode] + (loaded << 11));
    }
    EnterCriticalSection();
    FlushCache();
    ExitCriticalSection();
}

INCLUDE_RODATA("asm/us/main/nonmatchings/main", g_modeOverlayAddrs);

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
        if (g_mbgIsBackground == 1) {
            r |= 8;
        }
    }
    return r;
}

s8 mbg_getState(void) {
    return g_mbgState;
}

