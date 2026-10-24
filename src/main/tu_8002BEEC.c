#include "ff9.h"

void gfx_init(u32 *ot0, u32 *ot1, s32 pk0, s32 pk1) {
    GameState *gs = g_gameState;
    g_drawBufs.ots[0] = ot0;
    g_drawBufs.ots[1] = ot1;
    g_drawBufs.prims[0] = (u8 *)pk0;
    g_drawBufs.prims[1] = (u8 *)pk1;
    g_drawBufs.dispPos[0].vx = gs->unkC[0].disp.x;
    g_drawBufs.dispPos[0].vy = gs->unkC[0].disp.y;
    g_drawBufs.dispPos[1].vx = gs->unkC[1].disp.x;
    g_drawBufs.dispPos[1].vy = gs->unkC[1].disp.y;
    g_drawBufs.cur = gs->unk1C->unk8;
    g_primPtr = g_drawBufs.prims[g_drawBufs.cur];
    g_ot = g_drawBufs.ots[g_drawBufs.cur];
    pad_initRepeat();
    win_initAll();
    g_drawBufs.unk1 = 0;
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

void func_8002C05C(void) {
}

void func_8002C064(s32 arg0) {
    g_drawBufs.unk1 = arg0;
    if (arg0 == 2) {
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

void gfx_setBufferIndex(u8 a) {
    g_drawBufs.cur = a;
}

void gfx_setTarget(u32 *a, u8 *b) {
    g_ot = a;
    g_primPtr = b;
}

void text_uploadPartyNames(s32 a) {
    s32 buf = text_initNameImage(a);
    s32 i;
    for (i = 0; i < 4; i++) {
        if (g_gameState->unk1C->party[i] != NULL) {
            text_renderPartyName(i);
        }
    }
    LoadImage(&D_8006BC38, (u32 *)buf);
    DrawSync(0);
}

void func_8002C1E4(void) {
}

s32 win_resetClock(void) {
    return g_winLastVsync = VSync(-1);
}

void pad_initRepeat(void) {
    s32 p;
    s32 i;
    u32 v;
    g_padSavedStickFlag = 1;
    v = g_padPortHeld[0];
    g_padRepeat = v;
    g_padPressed = v;
    g_padHeld = v;
    pad_getAnalogThresholds(&g_padSavedThresholds);
    for (p = 0; p < 2; p++) {
        pad_disableRepeat(p);
        for (i = 0; i < 23; i++) {
            pad_resetRepeat(p, i);
            pad_setRepeatEnabled(p, i, g_padRepeatDefs[i].enabled);
            pad_setRepeatMask(p, i, g_padRepeatDefs[i].mask);
            pad_setRepeatDelay(p, i, g_padRepeatDefs[i].delay);
            pad_setRepeatInterval(p, i, g_padRepeatDefs[i].interval);
        }
    }
}

void pad_updateRepeat(s32 port) {
    u32 res = 0;
    PadRepeat *e = g_padRepeats[port];
    u32 now = VSync(-1);
    s32 i;
    u32 m;
    u32 out;
    u32 d;

    g_padHeld = g_padPortHeld[port];
    g_padPressed = g_padPortPressed[port];
    for (i = 0; i < 23; e++, i++) {
        if (e->enabled != 0) {
            m = g_padHeld & e->mask & e->prevHeld;
            e->prevHeld = g_padHeld;
            out = 0;
            if (m != 0) {
                if (e->armed != 0) {
                    d = now - e->lastVsync;
                    if (e->repeating == 0) {
                        if (d >= e->delay) {
                            out = m;
                            e->repeating = 1;
                            e->lastVsync = now;
                        }
                    } else if (d >= e->interval) {
                        out = m;
                        e->lastVsync = now;
                    }
                }
            } else {
                e->repeating = 0;
                e->armed = 1;
                e->lastVsync = now;
            }
            res |= out;
        }
    }
    g_padRepeat = res | g_padPressed;
}

s32 pad_isAnalog(s32 i) {
    return ((g_padBuf[i].unk0 >> 12) & 0xF) == PAD_ID_ANALOG;
}

void pad_setStickAsDpad(u8 on) {
    if (on) {
        g_padSavedStickFlag = pad_getFlags() >> 15;
        pad_setFlags(0x8000, 1);
        pad_getAnalogThresholds(&g_padSavedThresholds);
        pad_setAnalogThresholds(&g_padStickDpadThresholds);
    } else {
        pad_setFlags(0x8000, g_padSavedStickFlag);
        pad_setAnalogThresholds(&g_padSavedThresholds);
    }
}

void pad_disableRepeat(s32 a) {
    s32 i;
    for (i = 0; i < 0x17; i++) {
        pad_resetRepeat(a, i);
        pad_setRepeatEnabled(a, i, 0);
    }
}

void pad_resetRepeats(s32 a) {
    s32 i;
    for (i = 0; i < 0x17; i++) {
        pad_resetRepeat(a, i);
    }
}

void pad_resetRepeat(s32 a, s32 b) {
    PadRepeat *e = &g_padRepeats[a][b];
    e->prevHeld = g_padPortHeld[a];
    e->lastVsync = VSync(-1);
    e->armed = 0;
    e->repeating = 0;
}

void pad_clearRepeats(s32 a) {
    s32 i;
    for (i = 0; i < 0x17; i++) {
        pad_clearRepeat(a, i);
    }
}

void pad_clearRepeat(s32 a, s32 b) {
    PadRepeat *e = &g_padRepeats[a][b];
    e->prevHeld = 0;
    e->lastVsync = VSync(-1);
    e->armed = 0;
    e->repeating = 0;
}

u8 pad_isRepeatEnabled(s32 a, s32 b) {
    return g_padRepeats[a][b].enabled;
}

void pad_setRepeatEnabled(s32 a, s32 b, s32 c) {
    PadRepeat *e = &g_padRepeats[a][b];

    e->prevHeld = g_padPortHeld[a];
    e->lastVsync = VSync(-1);
    e->armed = 0;
    e->repeating = 0;
    g_padRepeats[a][b].enabled = c;
}

u16 pad_getRepeatDelay(s32 a, s32 b) {
    return g_padRepeats[a][b].delay;
}

u16 pad_getRepeatInterval(s32 a, s32 b) {
    return g_padRepeats[a][b].interval;
}

void pad_setRepeatDelay(s32 a, s32 b, s32 v) {
    g_padRepeats[a][b].delay = v;
}

void pad_setRepeatInterval(s32 a, s32 b, s32 v) {
    g_padRepeats[a][b].interval = v;
}

void pad_setRepeatTiming(s32 a, s32 b, s32 c, s32 d) {
    pad_setRepeatDelay(a, b, c);
    pad_setRepeatInterval(a, b, d);
}

s32 pad_getRepeatMask(s32 a, s32 b) {
    return g_padRepeats[a][b].mask;
}

void pad_setRepeatMask(s32 a, s32 b, s32 v) {
    g_padRepeats[a][b].mask = v;
}
