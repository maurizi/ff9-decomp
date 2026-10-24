#include "ff9.h"

void func_800236C4(void) {
    GameStateUnk24 *p = (GameStateUnk24 *)&g_gameState->unk24;
    s32 i;

    p->unk0_0 = 0;
    p->unk0_1 = 0;
    p->unk0_2 = 0;
    p->unk0_3 = 1;
    p->unk0_4 = 0;
    p->unk0_5 = 0;
    p->unk0_6 = 0;
    p->unk0_7 = 1;
    p->unk0_8 = 1;
    p->unk0_11 = 3;
    p->unk0_14 = 3;
    p->unk0_17 = 0;
    p->unk4 = 0;
    for (i = 0; i < 8; i++) {
        p->unk4 |= i << (i * 4);
    }
    func_8002397C();
}

void func_80023784(void) {
    s32 *act;
    s32 port;
    s32 i;
    u32 *tbl;

    port = 0;
    tbl = D_80068680;
    for (; port < 2; port++) {
        for (i = 0; i < 8; i++) {
            act = D_800686A0;
            pad_setMapping(port, tbl[i], act[func_80023784_remap(i)]);
        }
        for (i = 0; i < 1; i++) {
            pad_setMapping(port, D_800686C0[i], act[func_80023784_remap(D_800686C4[i])]);
        }
        pad_setMapping(port, 0x04000000, 8);
        pad_setMapping(port, 0x02000000, 1);
        pad_setMapping(port, 0x08000000, 1);
    }
}

void func_80023918(void) {
    func_800236C4();
}

/* K&R: func_8002BE10 calls it without an argument */
s32 func_80023938(arg0)
s32 arg0;
{
    GameStateUnk24 *p = (GameStateUnk24 *)&g_gameState->unk24;

    if (p->unk0_1 != 1) {
        return arg0;
    }
    return (p->unk4 >> (arg0 * 4)) & 0xF;
}

void func_8002397C(void) {
    func_800239A4();
    func_80023784();
}

void func_800239A4(void) {
    func_800555A8(g_gameState->unk24 & 1);
}

void func_800239D0(void) {
    GameData *d = g_gameState->unk1C;
    func_8002BEEC(d->unk10[0], d->unk10[1], d->unk7E0, d->unk7E0 + 0x4000);
}

void func_80023A0C(void) {
    func_8002BFD0();
}

void func_80023A2C(void) {
    func_80023EB8();
}

void func_80023A4C(void) {
    func_80023F00();
}

void func_80023A6C(void) {
    UnkTimer *t;
    u8 blink;
    u8 *pkt;
    u32 *ot;
    u8 shown;
    u8 colon;
    s32 w;
    s32 d;
    s32 v;
    s32 dg;
    s32 hr;
    s32 min;

    t = &D_80076B60.unk408;
    d = D_80076B60.unk0 - t->unk4;
    blink = 0;
    min = t->unkC / 60;
    hr = min / 60;
    ot = g_gameState->unk1C->unk10[g_gameState->unk1C->unk8] + 1;
    D_800686C8.x = 4;
    pkt = D_80076B60.unk404;
    min %= 60;
    if (d < 8) {
        D_800686C8.x = d * 8 - 0x3C;
        blink = 1;
    }
    if (t->unk1 == 0) {
        v = VSync(-1) % 60;
        colon = 0;
        if (blink || v >= 30) {
            colon = 1;
        }
        shown = colon;
    } else if (hr != 0 || min != 0) {
        v = t->unkC % 60;
        shown = 1;
        colon = v >= 30;
    } else {
        v = VSync(-1) % 60;
        colon = v >= 30;
        shown = colon;
    }
    if (shown) {
        dg = hr / 10;
        if (dg != 0) {
            pkt = func_80027834(D_800686C8.x + 5, 8, ot, dg + 0xE0, t->unk1 == 0, &w, pkt);
        }
        dg = hr - dg * 10;
        pkt = func_80027834(D_800686C8.x + 0xF, 8, ot, dg + 0xE0, t->unk1 == 0, &w, pkt);
        dg = min / 10;
        pkt = func_80027834(D_800686C8.x + 0x23, 8, ot, dg + 0xE0, t->unk1 == 0, &w, pkt);
        dg = min - dg * 10;
        pkt = func_80027834(D_800686C8.x + 0x2D, 8, ot, dg + 0xE0, t->unk1 == 0, &w, pkt);
    }
    if (colon) {
        pkt = func_80027834(D_800686C8.x + 0x19, 8, ot, 0xEA, t->unk1 == 0, &w, pkt);
    }
    pkt = win_buildBorder(&D_800686C8, ot, 1, 1, 1, (g_gameState->unk24 >> 18) & 1, pkt);
    setlen(pkt, 3);
    ((TileWords *)pkt)->rgbc = 0x62000000;
    ((TileWords *)pkt)->xy = ((D_800686C8.x + 2) & 0xFFFF) | ((D_800686C8.y + 2) << 16);
    ((TileWords *)pkt)->wh = (D_800686C8.w - 4) | ((D_800686C8.h - 4) << 16);
    addPrim(ot, pkt);
    pkt += sizeof(TileWords);
    SetDrawTPage(pkt, 1, 1, 0);
    addPrim(ot, pkt);
}

void func_80023EB8(void) {
    func_80024070();
    D_80076B60.unk404 = D_80076B60.unk4;
    g_gameState->playTime = 0;
    D_80076B60.unk0 = VSync(-1);
}

void func_80023F00(void) {
    D_80076B60.unk404 = &D_80076B60.unk4[g_gameState->unk1C->unk8 << 9];
    if (func_800240E8() != 0) {
        func_80023A6C();
    }
    func_8002417C();
}

void func_80023F60(u8 arg0) {
    s32 now = VSync(-1);
    s32 d;
    UnkTimer *t = &D_80076B60.unk408;

    if (arg0) {
        d = now - D_80076B60.unk0;
        if (t->unk0 != 0 && t->unk1 != 0 && t->unkC != 0) {
            t->unkC -= d;
            if (t->unkC <= 0) {
                t->unkC = 0;
            }
        }
        g_gameState->playTime += d;
    } else {
        D_80076B60.unk0 = now;
    }
}

void game_setPlayTime(s32 arg0) {
    g_gameState->playTime = arg0;
    D_80076B60.unk0 = VSync(-1);
}

s32 game_getPlayTime(void) {
    return g_gameState->playTime;
}

u32 game_getPlayTimeSeconds(void) {
    return g_gameState->playTime / 60;
}

void func_80024070(void) {
    D_80076F68.unk0 = 0;
    D_80076F68.unkC = 0;
    D_80076F68.unk8 = 0;
    D_80076F68.unk1 = 0;
    D_80076F68.unk4 = VSync(-1);
}

void func_800240B0(u8 arg0) {
    D_80076F68.unk0 = arg0;
    D_80076F68.unk1 = 0;
    D_80076F68.unk4 = VSync(-1);
}

u8 func_800240E8(void) {
    return D_80076F68.unk0;
}

void func_800240F8(u8 arg0) {
    if (D_80076B60.unk408.unk0 != 0) {
        D_80076B60.unk408.unk1 = arg0;
    }
}

u8 func_8002411C(void) {
    return D_80076F68.unk1;
}

void func_8002412C(s32 arg0) {
    Unk80076B60 *p = &D_80076B60;
    s32 t = arg0 * 60;

    p->unk408.unkC = t;
    p->unk408.unk8 = t;
}

s32 func_8002414C(void) {
    return (D_80076F68.unkC + 59) / 60;
}

void func_8002417C(void) {
    s32 now;
    s32 dt;
    UnkTimer *t;

    now = VSync(-1);
    dt = now - D_80076B60.unk0;
    t = &D_80076B60.unk408;
    if (t->unk0 != 0 && t->unk1 != 0 && t->unkC != 0) {
        t->unkC -= dt;
        if (t->unkC <= 0) {
            t->unkC = 0;
        }
    }
    g_gameState->playTime += dt;
    D_80076B60.unk0 = now;
}
