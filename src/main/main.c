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
        } while (!(g_gameState->unk0 & GS_RESET));
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
            StoreImage((RECT *)&e->unk20, addrs.unk0[mode]);
        } else if ((s32)addrs.unk0[mode] != e->unk8) {
            file_copyViaVram((s32)addrs.unk0[mode], e->unk8, e->unkC, NULL);
        }
        loaded = e->unkC >> 11;
        preload_remove(id);
        DrawSync(0);
    }
    f = file_find(0, id);
    if (loaded < f->unkC - f->unk4) {
        cd_readSync(g_gameState->imgSector + f->unk4 + loaded, (f->unkC - (f->unk4 + loaded)) << 11,
                      (u8 *)addrs.unk0[mode] + (loaded << 11));
    }
    EnterCriticalSection();
    FlushCache();
    ExitCriticalSection();
}

INCLUDE_RODATA("asm/us/main/nonmatchings/main", D_80010034);

INCLUDE_ASM("asm/us/main/nonmatchings/main", func_80012AD4);

s8 mbg_getState(void) {
    return g_mbgState;
}

