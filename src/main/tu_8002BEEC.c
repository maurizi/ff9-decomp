#include "ff9.h"

void func_8002BEEC(u32 *ot0, u32 *ot1, s32 pk0, s32 pk1) {
    GameState *gs = g_gameState;
    g_drawBufs.unk14[0] = ot0;
    g_drawBufs.unk14[1] = ot1;
    g_drawBufs.unkC[0] = (u8 *)pk0;
    g_drawBufs.unkC[1] = (u8 *)pk1;
    g_drawBufs.unk1C[0].vx = gs->unkC[0].disp.x;
    g_drawBufs.unk1C[0].vy = gs->unkC[0].disp.y;
    g_drawBufs.unk1C[1].vx = gs->unkC[1].disp.x;
    g_drawBufs.unk1C[1].vy = gs->unkC[1].disp.y;
    g_drawBufs.unk0 = gs->unk1C->unk8;
    g_primPtr = g_drawBufs.unkC[g_drawBufs.unk0];
    g_ot = g_drawBufs.unk14[g_drawBufs.unk0];
    func_8002C210();
    func_8002E490();
    g_drawBufs.unk1 = 0;
    g_drawBufs.unk8 = VSync(-1);
    g_drawBufs.unk2 = 0;
}

void func_8002BFD0(void) {
    s32 now = VSync(-1);
    s32 dt = now - D_80077150;
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
    D_80077150 = now;
}

void func_8002C05C(void) {
}

void func_8002C064(s32 arg0) {
    g_drawBufs.unk1 = arg0;
    if (arg0 == 2) {
        g_drawBufs.unk8 = 0;
    }
}

void gfx_selectBuffer(s32 i) {
    g_drawBufs.unk4 = (g_drawBufs.unkC[1] - g_drawBufs.unkC[0]) - (g_primPtr - g_drawBufs.unkC[i ^ 1]);
    g_ot = g_drawBufs.unk14[i];
    g_primPtr = g_drawBufs.unkC[i];
}

void func_8002C0DC(s32 i) {
    u8 *prev = g_drawBufs.unkC[i ^ 1];
    g_drawBufs.unk0 = i;
    g_drawBufs.unk4 = (g_drawBufs.unkC[1] - g_drawBufs.unkC[0]) - (g_primPtr - prev);
    g_ot = g_drawBufs.unk14[i];
    g_primPtr = g_drawBufs.unkC[i];
}

void func_8002C138(u8 a) {
    g_drawBufs.unk0 = a;
}

void func_8002C144(u32 *a, u8 *b) {
    g_ot = a;
    g_primPtr = b;
}

void func_8002C158(s32 a) {
    s32 buf = func_8002B43C(a);
    s32 i;
    for (i = 0; i < 4; i++) {
        if (g_gameState->unk1C->party[i] != NULL) {
            func_8002B47C(i);
        }
    }
    LoadImage(&D_8006BC38, (u32 *)buf);
    DrawSync(0);
}

void func_8002C1E4(void) {
}

s32 func_8002C1EC(void) {
    return D_80077150 = VSync(-1);
}

void func_8002C210(void) {
    s32 p;
    s32 i;
    u32 v;
    D_80077178 = 1;
    v = g_padPortHeld[0];
    g_padRepeat = v;
    g_padPressed = v;
    g_padHeld = v;
    pad_getAnalogThresholds(&D_8007717C);
    for (p = 0; p < 2; p++) {
        func_8002C52C(p);
        for (i = 0; i < 23; i++) {
            func_8002C5CC(p, i);
            func_8002C740(p, i, D_8006BC40[i].unk4);
            func_8002C974(p, i, D_8006BC40[i].unk0);
            func_8002C870(p, i, D_8006BC40[i].unk5);
            func_8002C8AC(p, i, D_8006BC40[i].unk6);
        }
    }
}

void func_8002C308(s32 port) {
    u32 res = 0;
    Unk80077188 *e = D_80077188[port];
    u32 now = VSync(-1);
    s32 i;
    u32 m;
    u32 out;
    u32 d;

    g_padHeld = g_padPortHeld[port];
    g_padPressed = g_padPortPressed[port];
    for (i = 0; i < 23; e++, i++) {
        if (e->unk0 != 0) {
            m = g_padHeld & e->unkC & e->unk4;
            e->unk4 = g_padHeld;
            out = 0;
            if (m != 0) {
                if (e->unk2 != 0) {
                    d = now - e->unk8;
                    if (e->unk1 == 0) {
                        if (d >= e->unk10) {
                            out = m;
                            e->unk1 = 1;
                            e->unk8 = now;
                        }
                    } else if (d >= e->unk12) {
                        out = m;
                        e->unk8 = now;
                    }
                }
            } else {
                e->unk1 = 0;
                e->unk2 = 1;
                e->unk8 = now;
            }
            res |= out;
        }
    }
    g_padRepeat = res | g_padPressed;
}

s32 func_8002C47C(s32 i) {
    return ((g_padBuf[i].unk0 >> 12) & 0xF) == 7;
}

void func_8002C4B0(u8 on) {
    if (on) {
        D_80077178 = pad_getFlags() >> 15;
        pad_setFlags(0x8000, 1);
        pad_getAnalogThresholds(&D_8007717C);
        pad_setAnalogThresholds(&D_8006BCF8);
    } else {
        pad_setFlags(0x8000, D_80077178);
        pad_setAnalogThresholds(&D_8007717C);
    }
}

void func_8002C52C(s32 a) {
    s32 i;
    for (i = 0; i < 0x17; i++) {
        func_8002C5CC(a, i);
        func_8002C740(a, i, 0);
    }
}

void func_8002C584(s32 a) {
    s32 i;
    for (i = 0; i < 0x17; i++) {
        func_8002C5CC(a, i);
    }
}

void func_8002C5CC(s32 a, s32 b) {
    Unk80077188 *e = &D_80077188[a][b];
    e->unk4 = g_padPortHeld[a];
    e->unk8 = VSync(-1);
    e->unk2 = 0;
    e->unk1 = 0;
}

void func_8002C64C(s32 a) {
    s32 i;
    for (i = 0; i < 0x17; i++) {
        func_8002C694(a, i);
    }
}

void func_8002C694(s32 a, s32 b) {
    Unk80077188 *e = &D_80077188[a][b];
    e->unk4 = 0;
    e->unk8 = VSync(-1);
    e->unk2 = 0;
    e->unk1 = 0;
}

u8 func_8002C700(s32 a, s32 b) {
    return D_80077188[a][b].unk0;
}

void func_8002C740(s32 a, s32 b, s32 c) {
    Unk80077188 *e = &D_80077188[a][b];

    e->unk4 = g_padPortHeld[a];
    e->unk8 = VSync(-1);
    e->unk2 = 0;
    e->unk1 = 0;
    D_80077188[a][b].unk0 = c;
}

u16 func_8002C7F0(s32 a, s32 b) {
    return D_80077188[a][b].unk10;
}

u16 func_8002C830(s32 a, s32 b) {
    return D_80077188[a][b].unk12;
}

void func_8002C870(s32 a, s32 b, s32 v) {
    D_80077188[a][b].unk10 = v;
}

void func_8002C8AC(s32 a, s32 b, s32 v) {
    D_80077188[a][b].unk12 = v;
}

void func_8002C8E8(s32 a, s32 b, s32 c, s32 d) {
    func_8002C870(a, b, c);
    func_8002C8AC(a, b, d);
}

s32 func_8002C934(s32 a, s32 b) {
    return D_80077188[a][b].unkC;
}

void func_8002C974(s32 a, s32 b, s32 v) {
    D_80077188[a][b].unkC = v;
}
