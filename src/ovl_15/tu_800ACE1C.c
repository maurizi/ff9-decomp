#include "ovl_15.h"

void func_800ACE1C(void) {
    s32 i;
    Ovl15Mode *m;

    i = 0;
    m = D_800AE444;
    D_800B0FD8.unk0 = 0;
    D_800B0FD8.unk2 = 0;
    D_800B0FD8.unk3 = 0;
    for (; i < 5; i++, m++) {
        if (m->unk0 != NULL) {
            m->unk0();
        }
    }
    if (D_800AE444[D_800B0FD8.unk0].unk4 != NULL) {
        D_800AE444[D_800B0FD8.unk0].unk4();
    }
}

void func_800ACEB8(void) {
    void (*f)(void) = D_800AE444[D_800B0FD8.unk0].unk8;
    if (f != NULL) {
        f();
    }
}

void func_800ACF00(void) {
    void (*f)(void) = D_800AE444[D_800B0FD8.unk0].unkC;
    if (f != NULL) {
        f();
    }
}

u8 func_800ACF48(void) {
    return D_800B0FD8.unk0;
}

void func_800ACF58(s32 arg0) {
    if (arg0 != D_800B0FD8.unk0) {
        if (D_800AE444[D_800B0FD8.unk0].unkC != NULL) {
            D_800AE444[D_800B0FD8.unk0].unkC();
        }
        D_800B0FD8.unk0 = arg0;
        if (D_800AE444[D_800B0FD8.unk0].unk4 != NULL) {
            D_800AE444[D_800B0FD8.unk0].unk4();
        }
    }
}

s32 func_800ACFE4(void) {
    u32 i = 0;
    s32 pad = g_gameState->unkB;

    do {
        func_8001DCB8();
    } while (g_gameState->unkB == 0xFF && ++i < 10);
    func_80033D1C(1);
    if (func_800AD0C4(pad, g_gameState->unkB)) {
        return func_800AD08C(pad);
    }
    return 0xFF;
}

s32 func_800AD08C(s32 arg0) {
    s32 i;
    s32 mask;

    for (i = 0; i < 4; i++) {
        mask = 1 << i;
        if (arg0 & mask) {
            return i;
        }
    }
    return 0;
}

u8 func_800AD0C4(s32 arg0, s32 arg1) {
    s32 a = arg0 & 0xF;
    s32 b = arg1 & 0xF;
    return (b & a) != a;
}

s32 func_800AD0DC(void) {

    if (func_8002F810() == 0) {
        return (u32)(D_800B0FD8.unk0 - 1) < 3;
    }
    return 0;
}

void func_800AD11C(s8 arg0) {
    D_800B0FD8.unk3 = arg0;
}

u8 func_800AD128(void) {
    return D_800B0FD8.unk3;
}
