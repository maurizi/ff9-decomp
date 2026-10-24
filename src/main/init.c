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
    D_80073690.mask = 0x19;
    D_80073690.mode = 0x100;
    D_80073690.depth.left = 0;
    D_80073690.depth.right = 0;
    D_80073690.delay = 0;
    D_80073690.feedback = 0;
    SpuSetReverbModeParam(&D_80073690);
    SpuSetReverbDepth(&D_80073690);
    SpuGetReverbModeType(&D_800736A4);
    SpuClearReverbWorkArea(D_800736A4);
    snd_init();
    SpuGetReverbModeType(&D_800736A4);
    SpuClearReverbWorkArea(D_800736A4);
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
    D_80067950++;
}

INCLUDE_ASM("asm/us/main/nonmatchings/init", _bu_init);
