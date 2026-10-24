#include "ff9.h"

void evt_initSystem(void) {
    pen_init();
    msg_resetWindows();
    msg_resetPad();
    rand_init(GetRCnt(2));
    evt_reset();
}

s32 evt_update(a)
u8 a;
{
    s32 r = 0;
    if (D_8007B718 == 0) {
        msg_update();
        evt_updateIdleTimer();
        r = evt_updateObjects();
        msg_updatePlayerBubble();
        msg_drawAtePrompt();
    }
    return r;
}
