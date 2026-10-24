#include "ff9.h"

void game_initLibraries(void) {
    ResetCallback();
    ResetGraph(0);
    SetGraphDebug(0);
    SetDispMask(0);
    InitGeom();
    SpuInit();
    InitCARD(0);
    StartCARD();
    _bu_init();
    pad_init();
    cd_init();
}

void game_reset(void) {
    RECT r;
    s32 i;

    SetDispMask(0);
    VSyncCallback(NULL);
    DrawSyncCallback(NULL);
    SetGeomOffset(0xA0, 0x70);
    r.w = 0x3FF;
    r.x = 0;
    r.y = 0;
    r.h = 0x1FF;
    ClearImage(&r, 0, 0, 0);
    DrawSync(0);
    cd_resetState();
    db_init();
    pad_resetState();
    g_reverbAttr.mask = 0x19;
    g_reverbAttr.mode = 0x100;
    g_reverbAttr.depth.left = 0;
    g_reverbAttr.depth.right = 0;
    g_reverbAttr.delay = 0;
    g_reverbAttr.feedback = 0;
    SpuSetReverbModeParam(&g_reverbAttr);
    SpuSetReverbDepth(&g_reverbAttr);
    SpuGetReverbModeType(&g_reverbMode);
    SpuClearReverbWorkArea(g_reverbMode);
    snd_init();
    SpuGetReverbModeType(&g_reverbMode);
    SpuClearReverbWorkArea(g_reverbMode);
    for (i = 0; i < 90; i++) {
        VSync(0);
    }
    evt_initSystem();
    func_80053924();
    menu_init();
    game_initFade();
    mbg_resetState();
}

void game_shutdown(void) {
    snd_quit();
    db_quit();
    g_resetCount++;
}

INCLUDE_ASM("asm/us/main/nonmatchings/init", _bu_init);
