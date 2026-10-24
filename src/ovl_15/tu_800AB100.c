#include "ovl_15.h"

void bj_actionInit(void) {
    g_bjAction.cursor = 0;
}

void bj_actionEnter(void) {
    func_80032574();
    func_8002C4B0(1);
    g_bjAction.cursor = 0;
    g_bjAction.unk0 = 1;
    g_bjAction.unk7 = 0;
    g_bjAction.unk6 = 0;
    g_bjAction.shown = 1;
    g_bjAction.fadingIn = 0;
    g_bjAction.fadeFrames = 0;
    g_bjAction.unkC = 0;
}

void bj_actionUpdate(void) {
    func_8002C308(0);
    bj_actionInput();
    bj_actionDraw();
    bj_actionTick();
}

void bj_actionLeave(void) {
    func_8002C4B0(0);
}

u8 bj_actionGetCursor(void) {
    return g_bjAction.cursor;
}

void bj_actionInput(void) {
    if (g_bjAction.shown != 0 && g_bjAction.fadingIn == 0) {
        if (g_padRepeat & PAD_UP) {
            snd_playSe(0x67);
            g_bjAction.cursor = g_bjAction.cursor != 0 ? g_bjAction.cursor - 1 : 3;
            if (g_bjCanSplit == 0 && g_bjAction.cursor == 3) {
                g_bjAction.cursor--;
            }
            if (g_bjCanDouble == 0 && g_bjAction.cursor == 2) {
                g_bjAction.cursor--;
            }
        } else if (g_padRepeat & PAD_DOWN) {
            snd_playSe(0x67);
            g_bjAction.cursor = g_bjAction.cursor < 3 ? g_bjAction.cursor + 1 : 0;
            if (g_bjCanDouble == 0 && g_bjAction.cursor == 2) {
                g_bjAction.cursor++;
            }
            if (g_bjCanSplit == 0 && g_bjAction.cursor == 3) {
                g_bjAction.cursor = 0;
            }
        } else if (g_padRepeat & BTN_CROSS) {
            bj_actionDecide();
        }
    }
}

void bj_actionDraw(void) {
    func_800AB3C4();
    func_800AB3CC();
    bj_actionDrawCursor();
}

void bj_actionTick(void) {
    bj_actionStepFadeIn();
}

void bj_actionDecide(void) {
    snd_playSe(0x67);
    scene_set(4);
}

void func_800AB3C4(void) {
}

void func_800AB3CC(void) {
}

void bj_actionDrawCursor(void) {
    if (g_bjAction.shown != 0 && g_bjAction.fadingIn == 0) {
        D_80077DE8.unk0 = g_bjActionCursorPos[g_bjAction.cursor].x - 0x1A;
        D_80077DE8.unk2 = g_bjActionCursorPos[g_bjAction.cursor].y + 3;
        D_80077DE8.unk4 = 3;
        func_800355E4(&D_80077DE8, &g_bjAction);
    }
}

void bj_actionStepFadeIn(void) {
    g_bjAction.fadeFrames++;
    if (g_bjAction.fadeFrames > 48) {
        g_bjAction.fadeFrames = 0;
        g_bjAction.fadingIn = 0;
    }
}
