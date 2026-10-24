#include "ff9.h"

void pad_init(void) {
    PadInitDirect((u8 *)&D_80073940[0], (u8 *)&D_80073940[1]);
    PadStartCom();
}

void func_80018F50(void) {
    s32 i;
    s32 j;

    D_80073720 = 0x8000;
    D_80068158.unk0[0] = 0x5A;
    D_80068158.unk0[1] = 0xA6;
    D_80068158.unk0[2] = 0x5A;
    D_80068158.unk0[3] = 0xA6;
    for (i = 0; i < 2; i++) {
        g_padPortHeld[i] = g_padPortPressed[i] = g_padPortReleased[i] = D_80073998[i] = g_padPortState[i] = 0;
        g_padPortPrevHeld[i] = 0;
        for (j = 0; j < 32; j++) {
            D_80073730[i][j] = 0;
            D_80073830[i][j] = 0;
        }
        func_80019708(i, 0x10, 0x10);
        func_80019708(i, 0x40, 0x40);
        func_80019708(i, 0x80, 0x80);
        func_80019708(i, 0x20, 0x20);
        func_80019708(i, 0x1000, 0x1000);
        func_80019708(i, 0x2000, 0x2000);
        func_80019708(i, 0x4000, 0x4000);
        func_80019708(i, 0x8000, 0x8000);
        func_80019708(i, 0x400, 0x400);
        func_80019708(i, 0x100, 0x100);
        func_80019708(i, 0x800, 0x800);
        func_80019708(i, 0x200, 0x200);
        func_80019708(i, 1, 1);
        func_80019708(i, 8, 8);
        D_80073720 |= 1 << (i * 4);
    }
}

u16 func_80019150(void) {
    return D_80073720;
}

void func_80019160(u16 a, s32 b) {
    if (b) {
        D_80073720 |= a;
    } else {
        D_80073720 &= ~a;
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
            D_80073720 |= 1 << (i * 4);
        } else if (state == 1) {
            D_80073720 |= 4 << (i * 4);
        }
        if (PADBUF(i)->unk0 == 0 && (((D_80073940[i].unk0 >> 12) & 0xF) == 7 || ((D_80073940[i].unk0 >> 12) & 0xF) == 4)) {
            if ((D_80073720 >> (i * 4)) & 1) {
                if (g_padPortState[i] == 2) {
                    D_80073720 &= ~(1 << (i * 4));
                } else if (g_padPortState[i] == 6) {
                    if (PadInfoMode(i << 4, 2, 0)) {
                        b = 2 << (i * 4);
                        if (!(D_80073720 & b)) {
                            PadSetMainMode(i << 4, 1, 0);
                            D_80073720 |= b;
                        } else {
                            PadSetAct(i << 4, &D_800739B0[i].unk0, 2);
                            PadSetActAlign(i << 4, D_8006815C);
                            D_80073720 = D_80073720 & ~((1 << (i * 4)) | b) & ~(4 << (i * 4));
                        }
                    }
                }
            }
            b = 4 << (i * 4);
            if ((D_80073720 & b) && g_padPortState[i] == 6) {
                PadSetAct(i << 4, &D_800739B0[i].unk0, 2);
                PadSetActAlign(i << 4, D_8006815C);
                g_padPortState[i] = 4;
                D_80073720 &= ~b;
            }
            j = 0;
            buttons = ~PADBUF(i)->unk2;
            buttons &= 0xFFFF;
            D_80073938[i].unk0[0] = PADBUF(i)->unk4[0];
            D_80073938[i].unk0[1] = PADBUF(i)->unk4[1];
            D_80073938[i].unk0[2] = PADBUF(i)->unk4[2];
            D_80073938[i].unk0[3] = PADBUF(i)->unk4[3];
            g_padPortPrevHeld[i] = g_padPortHeld[i];
            g_padPortHeld[i] = 0;
            for (; j < 32; j++) {
                if (buttons & D_80073730[i][j]) {
                    g_padPortHeld[i] |= 1 << j;
                    D_80073830[i][j]++;
                } else {
                    D_80073830[i][j] = 0;
                }
            }
            if ((D_80073720 & 0x8000) && ((D_80073940[i].unk0 >> 12) & 0xF) == 7) {
                if (D_80073938[i].unk0[2] < D_80068158.unk0[2]) {
                    g_padPortHeld[i] |= 0x80;
                }
                if (D_80073938[i].unk0[2] > D_80068158.unk0[3]) {
                    g_padPortHeld[i] |= 0x20;
                }
                if (D_80073938[i].unk0[3] < D_80068158.unk0[0]) {
                    g_padPortHeld[i] |= 0x10;
                }
                if (D_80073938[i].unk0[3] > D_80068158.unk0[1]) {
                    g_padPortHeld[i] |= 0x40;
                }
            }
            g_padPortHeld[i] &= ~D_80073998[i];
            if ((D_80073998[i] & 0x10) && (s8)D_80073938[i].unk0[3] >= 0) {
                D_80073938[i].unk0[3] = 0x80;
            }
            if ((D_80073998[i] & 0x40) && D_80073938[i].unk0[3] > 0x80) {
                D_80073938[i].unk0[3] = 0x80;
            }
            if ((D_80073998[i] & 0x80) && (s8)D_80073938[i].unk0[2] >= 0) {
                D_80073938[i].unk0[2] = 0x80;
            }
            if ((D_80073998[i] & 0x20) && D_80073938[i].unk0[2] > 0x80) {
                D_80073938[i].unk0[2] = 0x80;
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

void func_80019708(s32 a, u32 b, s32 c) {
    s32 n = 31;
    gte_ldlzc(b);
    if (b != 0x80000000) {
        gte_stlzc(&b);
        n -= b;
    }
    D_80073730[a][n] = c;
}

void func_80019750(Unk4 *a) {
    *a = D_80068158;
}

void func_80019774(Unk4 *a) {
    D_80068158 = *a;
}
