#include "ff9.h"

void func_80032078(void) {
    func_8002FC38();
}

void func_80032098(void) {
    func_8002F6C8();
}

void func_800320B8(s32 a, s32 b, s32 c, s32 d) {
    func_8002FC8C(a, b, c, d);
}

void func_800320D8(void) {
    func_800325A4();
    func_8003264C();
}

void func_80032100(void) {
    func_80032658();
}

void menu_open(s32 a, s32 b) {
    menu_doOpen(a, b);
}

void menu_doOpen(s32 a, s32 b) {
    GameState *st = g_gameState;

    if (!(st->unk1C->unk0 & 1) && g_menu.unk5 == 0 && g_menu.unk0 == 0 && (u32)a < 9) {
        g_menu.unk5 = 1;
        g_menu.unk1 = a;
        g_menu.unkC = b;
        g_menu.unk1C = 0;
        g_menu.unk6 = 0;
        g_menu.unk7 = 0;
        g_menu.unk9 = 1;
        g_menu.unkA = 1;
        g_menu.unkB = 1;
        g_menu.unk24 = 0;
        g_menu.unk0 = 1;
        if (st->mode == 1) {
            st->unk1C->unk8E4.ovl65->unk0 |= 0x1000;
        }
        g_gameState->unk1C->unk0 |= 2;
        help_init();
        func_800326B0();
        func_80032980();
    }
}

void func_80032238(void) {
    GameState *g;
    s32 i;

    g_menu.unk7 = 0;
    g = g_gameState;
    g->unk1C->unk0 |= 0x1C;
    g_menu.unk14 = g->vsyncMode;
    g->vsyncMode = 0;
    g_menu.unk20 = g_gameState->unk10[0].r0;
    g_menu.unk21 = g_gameState->unk10[0].g0;
    g_menu.unk22 = g_gameState->unk10[0].b0;
    g_gameState->unk10[0].r0 = 0;
    g_gameState->unk10[0].g0 = 0;
    g_gameState->unk10[0].b0 = 0;
    g_gameState->unk10[1].r0 = 0;
    g_gameState->unk10[1].g0 = 0;
    g_gameState->unk10[1].b0 = 0;
    func_8002DE60();
    g_menu.unk18 = g_padPortMask[0];
    g_padPortMask[0] = 0x4000000;
    func_8002C4B0(1);
    for (i = 0; i < 2; i++) {
        func_8002C584(i);
    }
}

void func_80032384(void) {
    void (*fn)(u8);
    u8 idx;

    if (g_gameState->mode == 1) {
        g_gameState->unk1C->unk8E4.ovl65->unk0 &= ~0x1000;
    }
    g_gameState->unk1C->unk0 &= ~2;
    g_gameState->unk1C->unk0 &= ~0x1C;
    func_8002DE60();
    g_gameState->vsyncMode = g_menu.unk14;
    g_gameState->unk10[0].r0 = g_menu.unk20;
    g_gameState->unk10[0].g0 = g_menu.unk21;
    g_gameState->unk10[0].b0 = g_menu.unk22;
    g_gameState->unk10[1].r0 = g_menu.unk20;
    g_gameState->unk10[1].g0 = g_menu.unk21;
    g_gameState->unk10[1].b0 = g_menu.unk22;
    g_menu.unk0 = 0;
    g_menu.unk5 = 0;
    fn = (void (*)(u8))D_8006C9D4[g_menu.unk1].unk8;
    idx = g_menu.unk1;
    if (fn != NULL) {
        fn(idx);
    }
    func_8002C4B0(0);
    g_padPortMask[0] = g_menu.unk18;
    if (g_menu.unk24 != 0) {
        ((void (*)(void))g_menu.unk24)();
    }
    if (g_menu.unkA != 0) {
        func_8002F9D0();
    }
}

void func_8003253C(void) {
    func_80023918();
    func_80032078();
    func_80023A2C();
    func_800325A4();
}

void func_80032574(void) {
    func_80023918();
    func_80032078();
    help_init();
}

void func_800325A4(void) {
    g_menu.unk0 = 0;
    g_menu.unk5 = 0;
    g_menu.unk1C = 0;
    g_menu.unk24 = 0;
    g_menu.unk4 = 0;
    g_menu.unk3 = 0;
    help_init();
}

void func_800325E0(void) {
    if (D_80077DBB != 0) {
        func_8002F9F0();
        func_8002F8A8(func_80032384);
    } else {
        func_80032384();
    }
}

void func_80032628(void (*a)(void)) {
    D_80077DD4 = (s32)a;
}

void func_80032634(u8 a) {
    D_80077DBA = a;
}

void func_80032640(u8 a) {
    D_80077DBB = a;
}

void func_8003264C(void) {
    D_80077DB2 = 0;
}

void func_80032658(void) {
    void (*fn)(void);
    if (g_menu.unk5 != 0) {
        fn = D_8006CA64[g_menu.unk0];
        if (fn != NULL) {
            fn();
        }
    }
}

void func_800326B0(void) {
    if (D_8006CA40[g_menu.unk1] != 0xFF) {
        func_80032708();
    } else {
        func_80032798();
    }
}

void func_80032708(void) {
    FileEntry *f;
    if (cd_getState() == 0) {
        g_menu.unk0 = 2;
        g_menu.unk6 = 1;
        f = file_find(0, D_8006CA40[g_menu.unk1]);
        cd_readAsync(f->unk4 + g_gameState->unk20, (f->unkC - f->unk4) << 11, (void *)0x801EDF00, func_800329FC);

    }
}

void func_80032798(void) {
    s32 busy = cd_getState();
    FileEntry *f;

    if (g_menu.unk1 == 0) {
        if (func_80033D28() == 0) {
            g_menu.unk0 = 2;
        } else if (busy == 0 || g_gameState->mode != 3) {
            g_menu.unk6 = 1;
            f = file_find(1, 30);
            if (cd_readAsync(g_gameState->unk20 + f->unk4, (f->unkC - f->unk4) << 11, (void *)0x801EDF00,
                              func_80032A18)) {
                g_menu.unk0 = 2;
            }
        }
    }
}

void func_8003286C(void) {
    if (g_gameState->mode != 3 || cd_getState() == 0) {
        func_800326B0();
    }
}

void func_800328B4(void) {
    if ((D_80077DB4 & 0xFFFF0000) == 0) {
        func_80032A44();
    }
}

void func_800328E8(void) {
    void (*fn)(u8);
    u8 idx;

    func_8002C308(0);
    if (g_menu.unk9 != 0) {
        func_80024BD8();
    }
    fn = D_8006C9D4[g_menu.unk1].unk4;
    idx = g_menu.unk1;
    if (fn != NULL) {
        fn(idx);
    }
    help_update();
    func_8002C1EC();
    g_menu.unk10++;
}

void func_80032980(void) {
    g_menu.unk7 = 1;
    if (func_8002F800() == 0) {
        if (func_8002F810() == 0) {
            func_8002F9F0();
            func_8002F8A8(func_80032238);
        } else {
            func_80032238();
        }
    } else {
        func_8002F5A0(func_80032238);
    }
}

void func_800329FC(u8 *a) {
    if (*a == 0) {
        D_80077DB6 = 0;
    }
}

void func_80032A18(u8 *a) {
    if (*a == 0) {
        D_80077DB6 = 0;
    }
}

s32 func_80032A34(void) {
    return D_80077DCC;
}

void func_80032A44(void) {
    void (*fn)(u8);

    g_menu.unk10 = 0;
    DrawSync(0);
    EnterCriticalSection();
    FlushCache();
    ExitCriticalSection();
    g_menu.unk0 = 3;
    if (g_menu.unk1 == 0 && func_80033D28()) {
        func_80028B20((u8 *)0x801EDF00);
        func_80033D1C(0);
    }
    func_8002FA50(0x10);
    fn = D_8006C9D4[g_menu.unk1].unk0;
    if (fn != NULL) {
        fn(g_menu.unk1);
    }
}
