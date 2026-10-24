#include "ovl_15.h"

void func_800AD138(void) {
    D_800B0FE0.unk3 = 0;
}

void func_800AD144(void) {
    func_80032574();
    pad_setStickAsDpad(1);
    D_800B0FE0.unk0 = 1;
    D_800B0FE0.unk7 = 0;
    D_800B0FE0.unk6 = 0;
    D_800B0FE0.unk1 = 1;
    D_800B0FE0.unk2 = 0;
    D_800B0FE0.unk8 = 0;
    D_800B0FE0.unkC = 0;
}

void func_800AD194(void) {
    pad_updateRepeat(0);
    func_800AD1FC();
    func_800AD308();
    func_800AD338();
}

void func_800AD1CC(void) {
    pad_setStickAsDpad(0);
}

u8 func_800AD1EC(void) {
    return D_800B0FE0.unk3;
}

void func_800AD1FC(void) {
    if (D_800B0FE0.unk1 != 0 && D_800B0FE0.unk2 == 0) {
        if (g_padRepeat & PAD_RIGHT) {
            D_800B0F48 += 10;
            if (D_800AE0E8 < D_800B0F48) {
                D_800B0F48 = D_800AE0E8 / 10 * 10;
            }
            if (D_800B0F48 > 1000) {
                D_800B0F48 = 1000;
            }
            snd_playSe(0x67);
        } else if (g_padRepeat & PAD_LEFT) {
            D_800B0F48 -= 10;
            if (D_800B0F48 < 10) {
                D_800B0F48 = 10;
            }
            snd_playSe(0x67);
        } else if (g_padRepeat & BTN_CROSS) {
            func_800AD358();
        }
    }
}

void func_800AD308(void) {
    func_800AD380();
    func_800AD388();
    func_800AD390();
}

void func_800AD338(void) {
    func_800AD418();
}

void func_800AD358(void) {
    snd_playSe(0x67);
    func_800ACF58(4);
}

void func_800AD380(void) {
}

void func_800AD388(void) {
}

void func_800AD390(void) {
    if (D_800B0FE0.unk1 != 0 && D_800B0FE0.unk2 == 0) {
        g_menuHelp.x = D_800AE494[D_800B0FE0.unk3].x - 0x1A;
        g_menuHelp.y = D_800AE494[D_800B0FE0.unk3].y + 3;
        g_menuHelp.otz = 3;
        func_800355E4(&g_menuHelp, &D_800B0FE0);
    }
}

void func_800AD418(void) {
    D_800B0FE0.unk8++;
    if (D_800B0FE0.unk8 > 48) {
        D_800B0FE0.unk8 = 0;
        D_800B0FE0.unk2 = 0;
    }
}
