#include "ovl_14.h"

void func_800A7FF0(void) {
    s32 i;
    Ovl14Mode *m;

    i = 0;
    m = D_800AB238;
    D_800AB308.unk0 = 0;
    D_800AB308.unk2 = 0;
    D_800AB308.unk3 = 0;
    for (; i < 6; i++, m++) {
        if (m->unk0 != NULL) {
            m->unk0();
        }
    }
    if (D_800AB238[D_800AB308.unk0].unk4 != NULL) {
        D_800AB238[D_800AB308.unk0].unk4();
    }
}

void func_800A808C(void) {
    if (D_800AB238[D_800AB308.unk0].unk8 != NULL) {
        D_800AB238[D_800AB308.unk0].unk8();
    }
}

void func_800A80D4(void) {
    if (D_800AB238[D_800AB308.unk0].unkC != NULL) {
        D_800AB238[D_800AB308.unk0].unkC();
    }
}

s32 func_800A811C(void) {
    return D_800AB308.unk0;
}

void func_800A812C(s32 arg0) {
    if (arg0 != D_800AB308.unk0) {
        if (D_800AB238[D_800AB308.unk0].unkC != NULL) {
            D_800AB238[D_800AB308.unk0].unkC();
        }
        D_800AB308.unk0 = arg0;
        if (D_800AB238[D_800AB308.unk0].unk4 != NULL) {
            D_800AB238[D_800AB308.unk0].unk4();
        }
    }
}

s32 func_800A81B8(void) {
    u32 i;
    s32 prev;

    i = 0;
    prev = g_gameState->unkB;
    do {
        file_initImg();
    } while (g_gameState->unkB == 0xFF && ++i < 10);
    func_80033D1C(1);
    if (func_800A8298(prev, g_gameState->unkB)) {
        return func_800A8260(prev);
    }
    return 0xFF;
}

s32 func_800A8260(s32 arg0) {
    s32 i;
    s32 m;

    for (i = 0; i < 4; i++) {
        m = 1 << i;
        if (arg0 & m) {
            return i;
        }
    }
    return 0;
}

u8 func_800A8298(s32 arg0, s32 arg1) {
    s32 a = arg0 & 0xF;
    s32 b = arg1 & 0xF;
    return (b & a) != a;
}

s32 func_800A82B0(void) {
    if (fade_isBusy() != 0) {
        return 0;
    }
    return D_800AB308.unk0 == 1;
}

void func_800A82F4(s32 arg0) {
    D_800AB308.unk3 = arg0;
    if (func_800A811C() == 1) {
        func_800A8618();
    }
}

u8 func_800A832C(void) {
    return D_800AB308.unk3;
}
