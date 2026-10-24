#include "ovl_15.h"

void func_800AD44C(void) {
    D_800B0FF0.unk3 = 0;
}

void func_800AD458(void) {
    menu_initForMode();
    pad_setStickAsDpad(1);
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
    pad_updateRepeat(0);
    func_800AD514();
    func_800AD5EC();
    func_800AD624();
}

void func_800AD4E4(void) {
    pad_setStickAsDpad(0);
}

u8 func_800AD504(void) {
    return D_800B0FF0.unk3;
}

void func_800AD514(void) {
    if (fade_isBusy() == 0 && D_800B0FF0.unk1 != 0 && D_800B0FF0.unk2 == 0) {
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
    fade_out(0x10);
    fade_setOnDone(func_800AD75C);
}

void func_800AD678(void) {
}

void func_800AD680(void) {
}

void func_800AD688(void) {
}

void func_800AD690(void) {
    if (fade_isBusy() == 0 && D_800B0FF0.unk1 != 0 && D_800B0FF0.unk2 == 0) {
        g_menuHelp.x = D_800AE498[D_800B0FF0.unk3].x - 0x1A;
        g_menuHelp.y = D_800AE498[D_800B0FF0.unk3].y + 3;
        g_menuHelp.otz = 3;
        help_drawCursor(&g_menuHelp, &D_800B0FF0);
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
    fade_in(0x20);
}
