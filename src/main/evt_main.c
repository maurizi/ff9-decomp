#include "ff9.h"

void evt_initSystem(void) {
    pen_init();
    msg_resetWindows();
    msg_resetPad();
    rand_init(GetRCnt(2));
    evt_reset();
}

s32 evt_update(unused)
u8 unused;
{
    s32 result = 0;
    if (g_evtHalted == 0) {
        msg_update();
        evt_updateIdleTimer();
        result = evt_updateObjects();
        msg_updatePlayerBubble();
        msg_drawAtePrompt();
    }
    return result;
}
