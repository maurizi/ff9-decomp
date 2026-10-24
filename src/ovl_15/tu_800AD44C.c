#include "ovl_15.h"

void func_800AD44C(void) {
    D_800B0FF0.unk3 = 0;
}

void func_800AD458(void) {
    func_80032574();
    func_8002C4B0(1);
    D_800B0FF0.unk0 = 1;
    D_800B0FF0.unk7 = 0;
    D_800B0FF0.unk6 = 0;
    D_800B0FF0.unk1 = 0;
    D_800B0FF0.unk2 = 0;
    D_800B0FF0.unk8 = 0;
    D_800B0FF0.unkC = 0;
    func_800AD77C();
}

void func_800AD4AC(void) {
    func_8002C308(0);
    func_800AD514();
    func_800AD5EC();
    func_800AD624();
}

void func_800AD4E4(void) {
    func_8002C4B0(0);
}

u8 func_800AD504(void) {
    return D_800B0FF0.unk3;
}

void func_800AD514(void) {
    if (func_8002F810() == 0 && D_800B0FF0.unk1 != 0 && D_800B0FF0.unk2 == 0) {
        if (g_padRepeat & PAD_UP) {
            snd_playSe(0x67);
            D_800B0FF0.unk3 = D_800B0FF0.unk3 != 0 ? D_800B0FF0.unk3 - 1 : 3;
        } else if (g_padRepeat & PAD_DOWN) {
            snd_playSe(0x67);
            D_800B0FF0.unk3 = D_800B0FF0.unk3 < 3 ? D_800B0FF0.unk3 + 1 : 0;
        } else if (g_padRepeat & BTN_CROSS) {
            func_800AD644();
        }
    }
}

void func_800AD5EC(void) {
    func_800AD678();
    func_800AD680();
    func_800AD688();
    func_800AD690();
}

void func_800AD624(void) {
    func_800AD728();
}

void func_800AD644(void) {
    snd_playSe(0x67);
    func_8002FAAC(0x10);
    func_8002F8A8(func_800AD75C);
}

void func_800AD678(void) {
}

void func_800AD680(void) {
}

void func_800AD688(void) {
}

void func_800AD690(void) {
    if (func_8002F810() == 0 && D_800B0FF0.unk1 != 0 && D_800B0FF0.unk2 == 0) {
        g_help.x = D_800AE498[D_800B0FF0.unk3].x - 0x1A;
        g_help.y = D_800AE498[D_800B0FF0.unk3].y + 3;
        g_help.otz = 3;
        func_800355E4(&g_help, &D_800B0FF0);
    }
}

void func_800AD728(void) {
    D_800B0FF0.unk8++;
    if (D_800B0FF0.unk8 > 48) {
        D_800B0FF0.unk8 = 0;
        D_800B0FF0.unk2 = 0;
    }
}

void func_800AD75C(void) {
    func_800ACF58(4);
}

void func_800AD77C(void) {
    if (func_800AD128() == 0) {
        D_800B0FF0.unk1 = 1;
        D_800B0FF0.unk2 = 1;
        D_800B0FF0.unk8 = 0;
        return;
    }
    D_800B0FF0.unk1 = 1;
    D_800B0FF0.unk2 = 0;
    D_800B0FF0.unk8 = 0;
    func_8002FA50(0x20);
}
