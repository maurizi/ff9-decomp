#include "ff9.h"

void func_8004BBF0(void) {
    func_80051568();
    msg_resetWindows();
    msg_resetPad();
    rand_init(GetRCnt(2));
    func_8004AC80();
}

s32 func_8004BC38(a)
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
