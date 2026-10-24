#include "ff9.h"

void pad_init(void) {
    PadInitDirect((u8 *)&g_padBuf[0], (u8 *)&g_padBuf[1]);
    PadStartCom();
}

void pad_resetState(void) {
    s32 i;
    s32 j;

    g_padFlags = 0x8000;
    g_padAnalogThresholds.unk0[0] = 0x5A;
    g_padAnalogThresholds.unk0[1] = 0xA6;
    g_padAnalogThresholds.unk0[2] = 0x5A;
    g_padAnalogThresholds.unk0[3] = 0xA6;
    for (i = 0; i < 2; i++) {
        g_padPortHeld[i] = g_padPortPressed[i] = g_padPortReleased[i] = g_padPortMask[i] = g_padPortState[i] = 0;
        g_padPortPrevHeld[i] = 0;
        for (j = 0; j < 32; j++) {
            g_padMapping[i][j] = 0;
            g_padHoldFrames[i][j] = 0;
        }
        pad_setMapping(i, 0x10, 0x10);
        pad_setMapping(i, 0x40, 0x40);
        pad_setMapping(i, 0x80, 0x80);
        pad_setMapping(i, 0x20, 0x20);
        pad_setMapping(i, 0x1000, 0x1000);
        pad_setMapping(i, 0x2000, 0x2000);
        pad_setMapping(i, 0x4000, 0x4000);
        pad_setMapping(i, 0x8000, 0x8000);
        pad_setMapping(i, 0x400, 0x400);
        pad_setMapping(i, 0x100, 0x100);
        pad_setMapping(i, 0x800, 0x800);
        pad_setMapping(i, 0x200, 0x200);
        pad_setMapping(i, 1, 1);
        pad_setMapping(i, 8, 8);
        g_padFlags |= 1 << (i * 4);
    }
}

u16 pad_getFlags(void) {
    return g_padFlags;
}

void pad_setFlags(u16 a, s32 b) {
    if (b) {
        g_padFlags |= a;
    } else {
        g_padFlags &= ~a;
    }
}

void pad_update(void) {
    s32 i, j;
    s32 state;
    s32 b;
    u32 buttons;

    for (i = 0; i < 2; i++) {
        state = PadGetState(i << 4);
        g_padPortState[i] = state;
        if (state == 0) {
            g_padFlags |= 1 << (i * 4);
        } else if (state == 1) {
            g_padFlags |= 4 << (i * 4);
        }
        if (PADBUF(i)->unk0 == 0 && (((g_padBuf[i].unk0 >> 12) & 0xF) == 7 || ((g_padBuf[i].unk0 >> 12) & 0xF) == 4)) {
            if ((g_padFlags >> (i * 4)) & 1) {
                if (g_padPortState[i] == 2) {
                    g_padFlags &= ~(1 << (i * 4));
                } else if (g_padPortState[i] == 6) {
                    if (PadInfoMode(i << 4, 2, 0)) {
                        b = 2 << (i * 4);
                        if (!(g_padFlags & b)) {
                            PadSetMainMode(i << 4, 1, 0);
                            g_padFlags |= b;
                        } else {
                            PadSetAct(i << 4, &g_padActuator[i].unk0, 2);
                            PadSetActAlign(i << 4, g_padActAlign);
                            g_padFlags = g_padFlags & ~((1 << (i * 4)) | b) & ~(4 << (i * 4));
                        }
                    }
                }
            }
            b = 4 << (i * 4);
            if ((g_padFlags & b) && g_padPortState[i] == 6) {
                PadSetAct(i << 4, &g_padActuator[i].unk0, 2);
                PadSetActAlign(i << 4, g_padActAlign);
                g_padPortState[i] = 4;
                g_padFlags &= ~b;
            }
            j = 0;
            buttons = ~PADBUF(i)->unk2;
            buttons &= 0xFFFF;
            g_padAnalog[i].unk0[0] = PADBUF(i)->unk4[0];
            g_padAnalog[i].unk0[1] = PADBUF(i)->unk4[1];
            g_padAnalog[i].unk0[2] = PADBUF(i)->unk4[2];
            g_padAnalog[i].unk0[3] = PADBUF(i)->unk4[3];
            g_padPortPrevHeld[i] = g_padPortHeld[i];
            g_padPortHeld[i] = 0;
            for (; j < 32; j++) {
                if (buttons & g_padMapping[i][j]) {
                    g_padPortHeld[i] |= 1 << j;
                    g_padHoldFrames[i][j]++;
                } else {
                    g_padHoldFrames[i][j] = 0;
                }
            }
            if ((g_padFlags & 0x8000) && ((g_padBuf[i].unk0 >> 12) & 0xF) == 7) {
                if (g_padAnalog[i].unk0[2] < g_padAnalogThresholds.unk0[2]) {
                    g_padPortHeld[i] |= 0x80;
                }
                if (g_padAnalog[i].unk0[2] > g_padAnalogThresholds.unk0[3]) {
                    g_padPortHeld[i] |= 0x20;
                }
                if (g_padAnalog[i].unk0[3] < g_padAnalogThresholds.unk0[0]) {
                    g_padPortHeld[i] |= 0x10;
                }
                if (g_padAnalog[i].unk0[3] > g_padAnalogThresholds.unk0[1]) {
                    g_padPortHeld[i] |= 0x40;
                }
            }
            g_padPortHeld[i] &= ~g_padPortMask[i];
            if ((g_padPortMask[i] & 0x10) && (s8)g_padAnalog[i].unk0[3] >= 0) {
                g_padAnalog[i].unk0[3] = 0x80;
            }
            if ((g_padPortMask[i] & 0x40) && g_padAnalog[i].unk0[3] > 0x80) {
                g_padAnalog[i].unk0[3] = 0x80;
            }
            if ((g_padPortMask[i] & 0x80) && (s8)g_padAnalog[i].unk0[2] >= 0) {
                g_padAnalog[i].unk0[2] = 0x80;
            }
            if ((g_padPortMask[i] & 0x20) && g_padAnalog[i].unk0[2] > 0x80) {
                g_padAnalog[i].unk0[2] = 0x80;
            }
            g_padPortPressed[i] = (g_padPortHeld[i] ^ g_padPortPrevHeld[i]) & g_padPortHeld[i];
            g_padPortReleased[i] = (g_padPortHeld[i] ^ g_padPortPrevHeld[i]) & g_padPortPrevHeld[i];
        } else {
            g_padPortReleased[i] = 0;
            g_padPortPressed[i] = 0;
            g_padPortHeld[i] = 0;
        }
    }
}

void pad_setMapping(s32 a, u32 b, s32 c) {
    s32 n = 31;
    gte_ldlzc(b);
    if (b != 0x80000000) {
        gte_stlzc(&b);
        n -= b;
    }
    g_padMapping[a][n] = c;
}

void pad_getAnalogThresholds(Unk4 *a) {
    *a = g_padAnalogThresholds;
}

void pad_setAnalogThresholds(Unk4 *a) {
    g_padAnalogThresholds = *a;
}
