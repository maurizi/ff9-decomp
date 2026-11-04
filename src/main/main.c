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
    PreloadEntry *preload;
    FileEntry *file;

    game_freezeScreen();
    id = OVL_KEY(files[mode]);
    while (cd_getState()) {
    }
    if (preload_isReady(id)) {
        preload = preload_find(id);
        if (preload_isInVram(preload)) {
            StoreImage((RECT *)&preload->vramX, addrs.addr[mode]);
        } else if ((s32)addrs.addr[mode] != preload->dest) {
            file_copyViaVram((s32)addrs.addr[mode], preload->dest, preload->loaded, NULL);
        }
        loaded = preload->loaded >> 11;
        preload_remove(id);
        DrawSync(0);
    }
    file = file_find(DIR_OVERLAYS, id);
    if (loaded < file->link - file->sector) {
        cd_readSync(g_gameState->imgSector + file->sector + loaded, (file->link - (file->sector + loaded)) << 11,
                      (u8 *)addrs.addr[mode] + (loaded << 11));
    }
    EnterCriticalSection();
    FlushCache();
    ExitCriticalSection();
}

INCLUDE_RODATA("asm/us/main/nonmatchings/main", g_modeOverlayAddrs);

s32 mbg_getStatus(void) {
    s32 status = 0;

    if (g_mbgState == 1) {
        return MBG_STATUS_OPEN;
    }
    if (g_mbgState >= 2) {
        status = MBG_STATUS_OPEN;
        if (g_mbgFrame > 0) {
            status = MBG_STATUS_OPEN | MBG_STATUS_PLAYING;
        }
        if (g_mbgPaused != 0) {
            status |= MBG_STATUS_PAUSED;
        }
        if (g_mbgRgb24 != 0) {
            status |= MBG_STATUS_RGB24;
        }
        if (g_mbgIsBackground == 1) {
            status |= MBG_STATUS_BACKGROUND;
        }
    }
    return status;
}

s8 mbg_getState(void) {
    return g_mbgState;
}

