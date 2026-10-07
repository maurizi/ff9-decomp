#include "ovl_3b.h"

void func_801F37A8(void) {
    if ((D_80077528 & 0x10) && D_801F4748.unk4[D_801F4748.unk0].unk0 != 0) {
        D_801F4748.unk4[D_801F4748.unk0].unk0--;
        func_8002F488(0x67);
    } else if ((D_80077528 & 0x40) && D_801F4748.unk4[D_801F4748.unk0].unk0 < D_801F4748.unk1 - 1) {
        D_801F4748.unk4[D_801F4748.unk0].unk0++;
        func_8002F488(0x67);
    } else if (D_80077520 & 0x20) {
        if (D_801F4748.unk0 == 1 && func_801F4614(0)) {
            return;
        }
        D_801F4748.unk4[D_801F4748.unk0].unk1 = 1;
        D_801F4748.unk8 = D_801F4748.unk4[D_801F4748.unk0].unk0;
        D_801F4748.unk4[D_801F4748.unk0].unk0 = D_801F4748.unk9;
        func_8002F488(0x67);
    }
}

void func_801F38D8(void) {
    Cursor *c;
    u8 v;

    if ((D_80077520 & 0x80) && !(D_801F4748.unk4[D_801F4748.unk0].unk0 & 3)) {
        D_801F4748.unk4[D_801F4748.unk0].unk1 = 0;
        v = D_801F4748.unk8;
        D_801F4748.unk9 = D_801F4748.unk4[D_801F4748.unk0].unk0;
        D_801F4748.unk4[D_801F4748.unk0].unk0 = v;
        func_8002F488(0x67);
    } else if ((D_80077528 & 0x10) && D_801F4748.unk4[D_801F4748.unk0].unk0 >= 4) {
        D_801F4748.unk4[D_801F4748.unk0].unk0 -= 4;
        func_8002F488(0x67);
    } else if ((D_80077528 & 0x40) && D_801F4748.unk4[D_801F4748.unk0].unk0 < 4) {
        D_801F4748.unk4[D_801F4748.unk0].unk0 += 4;
        func_8002F488(0x67);
    } else if ((D_80077528 & 0x20) && (D_801F4748.unk4[D_801F4748.unk0].unk0 & 3) != 3) {
        D_801F4748.unk4[D_801F4748.unk0].unk0++;
        func_8002F488(0x67);
    } else if ((D_80077528 & 0x80) && (D_801F4748.unk4[D_801F4748.unk0].unk0 & 3)) {
        D_801F4748.unk4[D_801F4748.unk0].unk0--;
        func_8002F488(0x67);
    }
}

void func_801F3A60(void) {
    s32 x;
    s32 y;
    s32 sx;
    s32 sy;
    u8 t;

    if (D_801F4748.unk0 == 0) {
        D_801F4748.unk0 = 1;
        D_801F4748.unk4[1] = D_801F4748.unk4[0];
        func_8002F488(0x67);
    } else {
        x = D_801F4748.unk4[0].unk0;
        sx = D_801F4748.unk4[1].unk0;
        y = D_801F4748.unk4[0].unk1;
        sy = D_801F4748.unk4[1].unk1;
        if (func_801F4614(1) && y != sy) {
            func_8002F488(0x66);
        } else {
            t = D_801F4748.unkA[y][x];
            D_801F4748.unkA[y][x] = D_801F4748.unkA[sy][sx];
            D_801F4748.unkA[sy][sx] = t;
            D_801F4748.unk4[0] = D_801F4748.unk4[1];
            D_801F4748.unk0 = 0;
            func_8002F488(0x67);
        }
    }
}

INCLUDE_ASM("asm/us/ovl_3b/nonmatchings/ovl_3b", func_801F3B3C);

INCLUDE_ASM("asm/us/ovl_3b/nonmatchings/ovl_3b", func_801F3CF0);

INCLUDE_ASM("asm/us/ovl_3b/nonmatchings/ovl_3b", func_801F3E58);

INCLUDE_RODATA("asm/us/ovl_3b/nonmatchings/ovl_3b", D_801EDF00);

INCLUDE_ASM("asm/us/ovl_3b/nonmatchings/ovl_3b", func_801F3FEC);

void func_801F405C(void) {
    func_801F40AC();
    func_801F4150();
}

void func_801F4084(void) {
    func_8003556C(NULL);
    func_8002886C();
}

void func_801F40AC(void) {
    if (!func_8002F810()) {
        if (D_80077520 & 0x10000) {
            func_801F42A4();
            return;
        }
        if (D_80077520 & 0x20000) {
            func_801F3A60();
            return;
        }
        if (D_801F4748.unk4[D_801F4748.unk0].unk1 == 0) {
            func_801F37A8();
            return;
        }
        func_801F38D8();
    }
}

void func_801F4150(void) {
    func_801F4190();
    func_801F4204();
    func_801F3B3C();
    func_801F3CF0();
    func_801F3E58();
}

void func_801F4190(void) {
    func_8002DEE4(&func_801F4728, 8, 2);
    func_8002705C(&func_801F4728, 8, 2);
    func_8002B5FC(func_801F4728.x + 9, func_801F4728.y + 6, 6, func_80035774(0), 0);
}

void func_801F4204(void) {
    func_8002B524(D_801F4730.x + 0xC, D_801F4730.y - 3, 8, D_80070450, 0);
    func_8002DEE4(&D_801F4730, 8, 1);
    func_8002705C(&D_801F4730, 9, 1);
    func_8002B5FC(D_801F4730.x + 0x12, D_801F4730.y + 6, 6, func_80035774(D_801F4748.unk1), 0);
}

void func_801F42A4(void) {
    s32 i;

    if (D_801F4748.unk0 == 1) {
        D_801F4748.unk0 = 0;
        func_8002F488(0x65);
    } else if (!func_801F465C()) {
        func_8002F488(0x66);
    } else {
        for (i = 0; i < D_801F4748.unk1; i++) {
            func_80033AF4(i, D_801F4748.unkA[0][i] != 0xFF ? D_801F4748.unkA[0][i] : -1);
        }
        func_800325E0();
        func_8002F488(0x67);
    }
}

void func_801F4360(s32 x, s32 y, s32 otz, s32 id) {
    if (id != 0xFF) {
        func_801F43FC(x, y, otz, id, 0x2D, D_801F4748.unk1A[id]);
        func_8002423C(x + 0x26, y + 3, otz, &D_8006794C->unk1C->unk3C[id]);
    }
}

INCLUDE_ASM("asm/us/ovl_3b/nonmatchings/ovl_3b", func_801F43FC);

void func_801F457C(PartyRequest *req) {
    s32 i;

    D_801F4748.unk1 = req->unk0;
    for (i = 0; i < 4; i++) {
        D_801F4748.unkA[0][i] = req->unk4[i];
    }
    for (i = 0; i < 8; i++) {
        D_801F4748.unkA[1][i] = req->unk8[i];
    }
    for (i = 0; i < 8; i++) {
        D_801F4748.unk1A[i] = req->unk10[i];
    }
}

u8 func_801F4614(s32 i) {
    u8 id;

    id = D_801F4748.unkA[D_801F4748.unk4[i].unk1][D_801F4748.unk4[i].unk0];
    if (id == 0xFF) {
        return 0;
    }
    return D_801F4748.unk1A[id];
}

u8 func_801F465C(void) {
    s32 i;
    s32 n;
    u32 alive;
    u8 id;

    alive = 0;
    n = 0;
    for (i = 0; i < 4; i++) {
        id = D_801F4748.unkA[0][i];
        if (id != 0xFF) {
            n++;
            if (D_8006794C->unk1C->unk3C[id].unk10 != 0 && !(D_8006794C->unk1C->unk3C[id].unk38 & 3)) {
                alive++;
            }
        }
    }
    return n >= D_801F4748.unk1 ? alive > 0 : 0;
}

void func_801F46FC(void) {
    D_80077DE8.unk8 = func_80035774(D_801F4748.unk0 + 0xA);
}

INCLUDE_ASM("asm/us/ovl_3b/nonmatchings/ovl_3b", func_801F4728);

INCLUDE_RODATA("asm/us/ovl_3b/nonmatchings/ovl_3b", D_801EDFE8);
