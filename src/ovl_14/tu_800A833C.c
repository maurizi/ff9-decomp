#include "ovl_14.h"

void func_800A833C(void) {
}

void func_800A8344(void) {
    func_80032628(func_800A8378);
}

void func_800A8368(void) {
}

void func_800A8370(void) {
}

void func_800A8378(void) {
    func_800A812C(4);
}

void func_800A8398(void) {
}

void func_800A83A0(void) {
    D_800AB310 = 0;
}

void func_800A83AC(void) {
    if (++D_800AB310 >= 2) {
        func_800A812C(5);
    }
}

void func_800A83E8(void) {
    if (func_800A8FCC() == 0) {
        func_800A7578();
        g_gameState->mode = 1;
        g_gameState->unk1C->fieldId = 0x46;
        g_gameState->unk1C->unk3A = 2;
    }
}

void func_800A8448(void) {
}

void func_800A8450(void) {
    D_800AB318 = 0;
    func_80032628(func_800A84B0);
}

void func_800A847C(void) {
    if (D_800AB318 != 0) {
        func_800A84EC();
    }
}

void func_800A84A8(void) {
}

void func_800A84B0(void) {
    D_800AB318 = 1;
    if (func_801F5330()) {
        func_80032634(0);
    }
}

void func_800A84EC(void) {
    s32 mode;
    s32 r;

    if (func_801F5330()) {
        func_800A75C8();
        r = func_800A81B8();
        if (r != 0xFF) {
            menu_open(8, ((r & 3) << 14) | 0x3A98);
            mode = 3;
        } else {
            func_80032634(0);
            mode = 4;
        }
    } else {
        func_800A82F4(1);
        mode = 1;
    }
    func_800A812C(mode);
}
