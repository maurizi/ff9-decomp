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
        pad_setMapping(i, PAD_UP, PAD_UP);
        pad_setMapping(i, PAD_DOWN, PAD_DOWN);
        pad_setMapping(i, PAD_LEFT, PAD_LEFT);
        pad_setMapping(i, PAD_RIGHT, PAD_RIGHT);
        pad_setMapping(i, PAD_TRIANGLE, PAD_TRIANGLE);
        pad_setMapping(i, PAD_CIRCLE, PAD_CIRCLE);
        pad_setMapping(i, PAD_CROSS, PAD_CROSS);
        pad_setMapping(i, PAD_SQUARE, PAD_SQUARE);
        pad_setMapping(i, PAD_L1, PAD_L1);
        pad_setMapping(i, PAD_L2, PAD_L2);
        pad_setMapping(i, PAD_R1, PAD_R1);
        pad_setMapping(i, PAD_R2, PAD_R2);
        pad_setMapping(i, PAD_SELECT, PAD_SELECT);
        pad_setMapping(i, PAD_START, PAD_START);
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
        if (PADBUF(i)->unk0 == 0 && (((g_padBuf[i].unk0 >> 12) & 0xF) == PAD_ID_ANALOG || ((g_padBuf[i].unk0 >> 12) & 0xF) == PAD_ID_DIGITAL)) {
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
            if ((g_padFlags & 0x8000) && ((g_padBuf[i].unk0 >> 12) & 0xF) == PAD_ID_ANALOG) {
                if (g_padAnalog[i].unk0[2] < g_padAnalogThresholds.unk0[2]) {
                    g_padPortHeld[i] |= PAD_LEFT;
                }
                if (g_padAnalog[i].unk0[2] > g_padAnalogThresholds.unk0[3]) {
                    g_padPortHeld[i] |= PAD_RIGHT;
                }
                if (g_padAnalog[i].unk0[3] < g_padAnalogThresholds.unk0[0]) {
                    g_padPortHeld[i] |= PAD_UP;
                }
                if (g_padAnalog[i].unk0[3] > g_padAnalogThresholds.unk0[1]) {
                    g_padPortHeld[i] |= PAD_DOWN;
                }
            }
            g_padPortHeld[i] &= ~g_padPortMask[i];
            if ((g_padPortMask[i] & PAD_UP) && (s8)g_padAnalog[i].unk0[3] >= 0) {
                g_padAnalog[i].unk0[3] = 0x80;
            }
            if ((g_padPortMask[i] & PAD_DOWN) && g_padAnalog[i].unk0[3] > 0x80) {
                g_padAnalog[i].unk0[3] = 0x80;
            }
            if ((g_padPortMask[i] & PAD_LEFT) && (s8)g_padAnalog[i].unk0[2] >= 0) {
                g_padAnalog[i].unk0[2] = 0x80;
            }
            if ((g_padPortMask[i] & PAD_RIGHT) && g_padAnalog[i].unk0[2] > 0x80) {
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
