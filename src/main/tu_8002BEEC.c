#include "ff9.h"

void gfx_init(u32 *ot0, u32 *ot1, s32 pk0, s32 pk1) {
    GameState *gs = g_gameState;
    g_drawBufs.ots[0] = ot0;
    g_drawBufs.ots[1] = ot1;
    g_drawBufs.prims[0] = (u8 *)pk0;
    g_drawBufs.prims[1] = (u8 *)pk1;
    g_drawBufs.dispPos[0].vx = gs->dispEnv[0].disp.x;
    g_drawBufs.dispPos[0].vy = gs->dispEnv[0].disp.y;
    g_drawBufs.dispPos[1].vx = gs->dispEnv[1].disp.x;
    g_drawBufs.dispPos[1].vy = gs->dispEnv[1].disp.y;
    g_drawBufs.cur = gs->gameData->drawBuf;
    g_primPtr = g_drawBufs.prims[g_drawBufs.cur];
    g_ot = g_drawBufs.ots[g_drawBufs.cur];
    pad_initRepeat();
    win_initAll();
    g_drawBufs.uiMode = 0;
    g_drawBufs.lastVsync = VSync(-1);
    g_drawBufs.unk2 = 0;
}

void win_runFrame(void) {
    s32 now = VSync(-1);
    s32 dt = now - g_winLastVsync;
    s32 i;

    if (dt == 0) {
        dt = 1;
    }
    if (dt >= 16) {
        dt = 1;
    }
    for (i = 0; i < dt; i++) {
        win_updateAll();
    }
    win_drawAll();
    g_winLastVsync = now;
}

void gfx_nop(void) {
}

void gfx_setUiMode(s32 mode) {
    g_drawBufs.uiMode = mode;
    if (mode == 2) {
        g_drawBufs.lastVsync = 0;
    }
}

void gfx_selectBuffer(s32 i) {
    g_drawBufs.freeBytes = (g_drawBufs.prims[1] - g_drawBufs.prims[0]) - (g_primPtr - g_drawBufs.prims[i ^ 1]);
    g_ot = g_drawBufs.ots[i];
    g_primPtr = g_drawBufs.prims[i];
}

void gfx_setBuffer(s32 i) {
    u8 *prev = g_drawBufs.prims[i ^ 1];
    g_drawBufs.cur = i;
    g_drawBufs.freeBytes = (g_drawBufs.prims[1] - g_drawBufs.prims[0]) - (g_primPtr - prev);
    g_ot = g_drawBufs.ots[i];
    g_primPtr = g_drawBufs.prims[i];
}

void gfx_setBufferIndex(u8 idx) {
    g_drawBufs.cur = idx;
}

void gfx_setTarget(u32 *ot, u8 *prim) {
    g_ot = ot;
    g_primPtr = prim;
}

void text_uploadPartyNames(s32 work) {
    s32 image = text_initNameImage(work);
    s32 i;
    for (i = 0; i < PARTY_SIZE; i++) {
        if (g_gameState->gameData->party[i] != NULL) {
            text_renderPartyName(i);
        }
    }
    LoadImage(&g_partyNameRect, (u32 *)image);
    DrawSync(0);
}

void pad_onStartPressed(void) {
}

s32 win_resetClock(void) {
    return g_winLastVsync = VSync(-1);
}

void pad_initRepeat(void) {
    s32 port;
    s32 i;
    u32 held;
    g_padSavedStickFlag = 1;
    held = g_padPortHeld[0];
    g_padRepeat = held;
    g_padPressed = held;
    g_padHeld = held;
    pad_getAnalogThresholds(&g_padSavedThresholds);
    for (port = 0; port < 2; port++) {
        pad_disableRepeat(port);
        for (i = 0; i < PAD_RPT_COUNT; i++) {
            pad_resetRepeat(port, i);
            pad_setRepeatEnabled(port, i, g_padRepeatDefs[i].enabled);
            pad_setRepeatMask(port, i, g_padRepeatDefs[i].mask);
            pad_setRepeatDelay(port, i, g_padRepeatDefs[i].delay);
            pad_setRepeatInterval(port, i, g_padRepeatDefs[i].interval);
        }
    }
}

void pad_updateRepeat(s32 port) {
    u32 repeat = 0;
    PadRepeat *chan = g_padRepeats[port];
    u32 now = VSync(-1);
    s32 i;
    u32 heldBits;
    u32 out;
    u32 elapsed;

    g_padHeld = g_padPortHeld[port];
    g_padPressed = g_padPortPressed[port];
    for (i = 0; i < PAD_RPT_COUNT; chan++, i++) {
        if (chan->enabled != 0) {
            heldBits = g_padHeld & chan->mask & chan->prevHeld;
            chan->prevHeld = g_padHeld;
            out = 0;
            if (heldBits != 0) {
                if (chan->armed != 0) {
                    elapsed = now - chan->lastVsync;
                    if (chan->repeating == 0) {
                        if (elapsed >= chan->delay) {
                            out = heldBits;
                            chan->repeating = 1;
                            chan->lastVsync = now;
                        }
                    } else if (elapsed >= chan->interval) {
                        out = heldBits;
                        chan->lastVsync = now;
                    }
                }
            } else {
                chan->repeating = 0;
                chan->armed = 1;
                chan->lastVsync = now;
            }
            repeat |= out;
        }
    }
    g_padRepeat = repeat | g_padPressed;
}

s32 pad_isAnalog(s32 i) {
    return PAD_TYPE(g_padBuf[i].hdr) == PAD_ID_ANALOG;
}

void pad_setStickAsDpad(u8 on) {
    if (on) {
        g_padSavedStickFlag = pad_getFlags() >> 15;
        pad_setFlags(PAD_FLAG_STICK_DPAD, 1);
        pad_getAnalogThresholds(&g_padSavedThresholds);
        pad_setAnalogThresholds(&g_padStickDpadThresholds);
    } else {
        pad_setFlags(PAD_FLAG_STICK_DPAD, g_padSavedStickFlag);
        pad_setAnalogThresholds(&g_padSavedThresholds);
    }
}

void pad_disableRepeat(s32 port) {
    s32 i;
    for (i = 0; i < PAD_RPT_COUNT; i++) {
        pad_resetRepeat(port, i);
        pad_setRepeatEnabled(port, i, 0);
    }
}

void pad_resetRepeats(s32 port) {
    s32 i;
    for (i = 0; i < PAD_RPT_COUNT; i++) {
        pad_resetRepeat(port, i);
    }
}

void pad_resetRepeat(s32 port, s32 button) {
    PadRepeat *chan = &g_padRepeats[port][button];
    chan->prevHeld = g_padPortHeld[port];
    chan->lastVsync = VSync(-1);
    chan->armed = 0;
    chan->repeating = 0;
}

void pad_clearRepeats(s32 port) {
    s32 i;
    for (i = 0; i < PAD_RPT_COUNT; i++) {
        pad_clearRepeat(port, i);
    }
}

void pad_clearRepeat(s32 port, s32 button) {
    PadRepeat *chan = &g_padRepeats[port][button];
    chan->prevHeld = 0;
    chan->lastVsync = VSync(-1);
    chan->armed = 0;
    chan->repeating = 0;
}

u8 pad_isRepeatEnabled(s32 port, s32 button) {
    return g_padRepeats[port][button].enabled;
}

void pad_setRepeatEnabled(s32 port, s32 button, s32 enabled) {
    PadRepeat *chan = &g_padRepeats[port][button];

    chan->prevHeld = g_padPortHeld[port];
    chan->lastVsync = VSync(-1);
    chan->armed = 0;
    chan->repeating = 0;
    g_padRepeats[port][button].enabled = enabled;
}

u16 pad_getRepeatDelay(s32 port, s32 button) {
    return g_padRepeats[port][button].delay;
}

u16 pad_getRepeatInterval(s32 port, s32 button) {
    return g_padRepeats[port][button].interval;
}

void pad_setRepeatDelay(s32 port, s32 button, s32 delay) {
    g_padRepeats[port][button].delay = delay;
}

void pad_setRepeatInterval(s32 port, s32 button, s32 interval) {
    g_padRepeats[port][button].interval = interval;
}

void pad_setRepeatTiming(s32 port, s32 button, s32 delay, s32 interval) {
    pad_setRepeatDelay(port, button, delay);
    pad_setRepeatInterval(port, button, interval);
}

s32 pad_getRepeatMask(s32 port, s32 button) {
    return g_padRepeats[port][button].mask;
}

void pad_setRepeatMask(s32 port, s32 button, s32 mask) {
    g_padRepeats[port][button].mask = mask;
}
