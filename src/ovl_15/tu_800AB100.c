#include "ovl_15.h"

void func_800AB100(void) {
    D_800B0F50.unk3 = 0;
}

void func_800AB10C(void) {
    func_80032574();
    func_8002C4B0(1);
    D_800B0F50.unk3 = 0;
    D_800B0F50.unk0 = 1;
    D_800B0F50.unk7 = 0;
    D_800B0F50.unk6 = 0;
    D_800B0F50.unk1 = 1;
    D_800B0F50.unk2 = 0;
    D_800B0F50.unk8 = 0;
    D_800B0F50.unkC = 0;
}

void func_800AB160(void) {
    func_8002C308(0);
    func_800AB1C8();
    func_800AB34C();
    func_800AB37C();
}

void func_800AB198(void) {
    func_8002C4B0(0);
}

u8 func_800AB1B8(void) {
    return D_800B0F50.unk3;
}

void func_800AB1C8(void) {
    if (D_800B0F50.unk1 != 0 && D_800B0F50.unk2 == 0) {
        if (g_padRepeat & PAD_UP) {
            snd_playSe(0x67);
            D_800B0F50.unk3 = D_800B0F50.unk3 != 0 ? D_800B0F50.unk3 - 1 : 3;
            if (D_800B0F4C == 0 && D_800B0F50.unk3 == 3) {
                D_800B0F50.unk3--;
            }
            if (D_800B0F44 == 0 && D_800B0F50.unk3 == 2) {
                D_800B0F50.unk3--;
            }
        } else if (g_padRepeat & PAD_DOWN) {
            snd_playSe(0x67);
            D_800B0F50.unk3 = D_800B0F50.unk3 < 3 ? D_800B0F50.unk3 + 1 : 0;
            if (D_800B0F44 == 0 && D_800B0F50.unk3 == 2) {
                D_800B0F50.unk3++;
            }
            if (D_800B0F4C == 0 && D_800B0F50.unk3 == 3) {
                D_800B0F50.unk3 = 0;
            }
        } else if (g_padRepeat & BTN_CROSS) {
            func_800AB39C();
        }
    }
}

void func_800AB34C(void) {
    func_800AB3C4();
    func_800AB3CC();
    func_800AB3D4();
}

void func_800AB37C(void) {
    func_800AB45C();
}

void func_800AB39C(void) {
    snd_playSe(0x67);
    func_800ACF58(4);
}

void func_800AB3C4(void) {
}

void func_800AB3CC(void) {
}

void func_800AB3D4(void) {
    if (D_800B0F50.unk1 != 0 && D_800B0F50.unk2 == 0) {
        g_help.x = D_800AE3DC[D_800B0F50.unk3].x - 0x1A;
        g_help.y = D_800AE3DC[D_800B0F50.unk3].y + 3;
        g_help.otz = 3;
        func_800355E4(&g_help, &D_800B0F50);
    }
}

void func_800AB45C(void) {
    D_800B0F50.unk8++;
    if (D_800B0F50.unk8 > 48) {
        D_800B0F50.unk8 = 0;
        D_800B0F50.unk2 = 0;
    }
}
