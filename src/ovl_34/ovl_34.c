#include "ovl_34.h"

INCLUDE_RODATA("asm/us/ovl_34/nonmatchings/ovl_34", D_801EDF00);

INCLUDE_RODATA("asm/us/ovl_34/nonmatchings/ovl_34", jtbl_801EDF04);

INCLUDE_ASM("asm/us/ovl_34/nonmatchings/ovl_34", func_801EEA1C);

INCLUDE_ASM("asm/us/ovl_34/nonmatchings/ovl_34", func_801EEB68);

INCLUDE_ASM("asm/us/ovl_34/nonmatchings/ovl_34", func_801EECB4);

INCLUDE_ASM("asm/us/ovl_34/nonmatchings/ovl_34", func_801EED9C);

INCLUDE_ASM("asm/us/ovl_34/nonmatchings/ovl_34", func_801EEEF8);

INCLUDE_ASM("asm/us/ovl_34/nonmatchings/ovl_34", func_801EF110);

INCLUDE_ASM("asm/us/ovl_34/nonmatchings/ovl_34", func_801EF2B4);

INCLUDE_ASM("asm/us/ovl_34/nonmatchings/ovl_34", func_801EF3F8);

INCLUDE_ASM("asm/us/ovl_34/nonmatchings/ovl_34", func_801EF50C);

INCLUDE_ASM("asm/us/ovl_34/nonmatchings/ovl_34", func_801EF938);

void func_801EFA54(void) {
    s32 i;
    s32 d;

    if (D_801F1558.unk4 != 0 && ++D_801F1558.unk5 >= 5) {
        i = D_801F1558.unk3C.unk5 + D_801F1558.unk3C.unk6;
        d = 0xFF;
        if (D_801F1558.unk4 == 1) {
            d = 1;
        }
        D_801F1558.unk5 = 0;
        D_801F1558.unk4 = 0;
        switch (i) {
        case 7:
            D_8006794C->unk24 = (D_8006794C->unk24 & ~0x700) | (((d + ((D_8006794C->unk24 >> 8) & 7)) & 7) << 8);
            break;
        case 8:
            D_8006794C->unk24 = (D_8006794C->unk24 & ~0x1C000) | (((d + ((D_8006794C->unk24 >> 14) & 7)) & 7) << 14);
            break;
        }
    }
}

INCLUDE_ASM("asm/us/ovl_34/nonmatchings/ovl_34", func_801EFB38);

INCLUDE_ASM("asm/us/ovl_34/nonmatchings/ovl_34", func_801EFC20);

INCLUDE_ASM("asm/us/ovl_34/nonmatchings/ovl_34", func_801EFF58);

void func_801F00A8(void) {
    func_801F00F8();
    func_801F0184();
    func_801F025C();
}

void func_801F00D8(void) {
    func_8003556C(NULL);
}

void func_801F00F8(void) {
    s32 i;
    void (*fn)(void);

    i = D_801F1558.unk3C.unk5 + D_801F1558.unk3C.unk6;
    if (!func_8002F810()) {
        if (D_801F1558.unk2 == 0) {
            func_801EECB4();
            return;
        }
        fn = D_801F1424[i];
        if (fn != NULL) {
            fn();
        }
    }
}

INCLUDE_ASM("asm/us/ovl_34/nonmatchings/ovl_34", func_801F0184);

void func_801F025C(void) {
    if (func_8003561C()) {
        func_801EFB38();
    }
    if (D_801F1558.unk2 == 0) {
        func_801EFA54();
    } else {
        func_801F1074();
    }
    func_801F10C8();
    func_801F1168();
}

INCLUDE_ASM("asm/us/ovl_34/nonmatchings/ovl_34", func_801F02CC);

INCLUDE_ASM("asm/us/ovl_34/nonmatchings/ovl_34", func_801F0380);

void func_801F043C(MenuList *m, RECT *src, RECT *dst) {
    dst->x = src->x;
    dst->w = src->w;
    if (m->unk8 & 2) {
        dst->y = src->y + m->unk9;
    } else {
        dst->y = src->y;
    }
    dst->h = src->h - m->unk9;
}

INCLUDE_ASM("asm/us/ovl_34/nonmatchings/ovl_34", func_801F049C);

void func_801F0550(RECT *r, s32 otz, s32 i) {
    func_800269F4(r->x + 0x70, r->y + 1, otz, 4 - r->h);
    D_801F14BC[i](r, otz);
}

void func_801F05D0(RECT *r, s32 otz) {
    func_801F0FE8(r->x + 0xF, r->y + 6, otz, D_801F13F0[0][0], 0);
    func_801F0D20(r, otz, D_8006794C->unk24 & 1, 0);
}

void func_801F0640(RECT *r, s32 otz) {
    func_801F0FE8(r->x + 0xF, r->y + 6, otz, D_801F13F0[1][0], 0);
    func_801F0D20(r, otz, (D_8006794C->unk24 >> 1) & 1, 1);
}

void func_801F06B4(RECT *r, s32 otz) {
    func_801F0FE8(r->x + 0xF, r->y + 6, otz, D_801F13F0[2][0], 0);
    func_801F0D20(r, otz, (D_8006794C->unk24 >> 2) & 1, 2);
}

void func_801F0728(RECT *r, s32 otz) {
    func_801F0FE8(r->x + 0xF, r->y + 6, otz, D_801F13F0[3][0], 0);
    func_801F0D20(r, otz, (D_8006794C->unk24 >> 3) & 1, 3);
}

void func_801F079C(RECT *r, s32 otz) {
    func_801F0FE8(r->x + 0xF, r->y + 6, otz, D_801F13F0[4][0], 0);
    func_801F0D20(r, otz, (D_8006794C->unk24 >> 4) & 1, 4);
}

void func_801F0810(RECT *r, s32 otz) {
    func_801F0FE8(r->x + 0xF, r->y + 6, otz, D_801F13F0[5][0], 0);
    func_801F0D20(r, otz, (D_8006794C->unk24 >> 5) & 1, 5);
}

void func_801F0884(RECT *r, s32 otz) {
    func_801F0FE8(r->x + 0xF, r->y + 6, otz, D_801F13F0[6][0], 0);
    func_801F0D20(r, otz, (D_8006794C->unk24 >> 6) & 1, 6);
}

void func_801F08F8(RECT *r, s32 otz) {
    func_801F0FE8(r->x + 0xF, r->y + 6, otz, D_801F13F0[7][0], 0);
    func_801F0DD8(r, otz, (D_8006794C->unk24 >> 8) & 7, 2, 7);
}

void func_801F0974(RECT *r, s32 otz) {
    func_801F0FE8(r->x + 0xF, r->y + 6, otz, D_801F13F0[8][0], 0);
    func_801F0DD8(r, otz, (D_8006794C->unk24 >> 14) & 7, 6, 8);
}

void func_801F09F0(RECT *r, s32 otz) {
    func_801F0FE8(r->x + 0xF, r->y + 6, otz, D_801F13F0[9][0], 0);
    func_801F0D20(r, otz, (D_8006794C->unk24 >> 17) & 1, 9);
}

void func_801F0A64(RECT *r, s32 otz) {
    func_801F0FE8(r->x + 0xF, r->y + 6, otz, D_801F13F0[10][0], 0);
    func_801F0D20(r, otz, (D_8006794C->unk24 >> 18) & 1, 10);
}

void func_801F0AD8(RECT *r, s32 otz) {
    func_801F0FE8(r->x + 0xF, r->y + 6, otz, D_801F13F0[11][0], 0);
    func_801F0D20(r, otz, (D_8006794C->unk24 >> 19) & 1, 11);
}

INCLUDE_ASM("asm/us/ovl_34/nonmatchings/ovl_34", func_801F0B4C);

void func_801F0BE8(void) {
    s32 i;

    i = D_801F1558.unk3C.unk5 + D_801F1558.unk3C.unk6;
    if (D_801F1558.unk2 != 0) {
        func_8002DEE4(&D_801F1558.unk1C.unk4, 0x12, 2);
        func_8002DEE4(&D_801F1558.unk2C.unk4, 0x12, 2);
        if (i == 1) {
            func_801EF110();
            return;
        }
        if (i == 5) {
            func_801EF2B4();
        }
    }
}

INCLUDE_ASM("asm/us/ovl_34/nonmatchings/ovl_34", func_801F0C78);

INCLUDE_ASM("asm/us/ovl_34/nonmatchings/ovl_34", func_801F0D20);

INCLUDE_ASM("asm/us/ovl_34/nonmatchings/ovl_34", func_801F0DD8);

void func_801F0F48(s32 x, s32 y, s32 otz, u8 *s, s32 arg4) {
    D_801F1558.unk4C = func_80029D60(x, y, &D_8007716C[otz], s, arg4, D_801F1558.unk4C);
}

void func_801F0F94(s32 x, s32 y, s32 otz, s32 n, u8 arg4) {
    u8 buf[8];

    D_801F1558.unk4C = func_80027834(x, y, &D_8007716C[otz], n, arg4, buf, D_801F1558.unk4C);
}

void func_801F0FE8(s32 x, s32 y, s32 otz, s32 text, s32 color) {
    D_801F1558.unk4C = func_8002920C(x, y - 4, &D_8007716C[otz], func_80035774(text & 0xFF), color, D_801F1558.unk4C);
}

void func_801F1074(void) {
    if (D_801F1558.unk1C.unkC != 0) {
        func_800631AC(&D_801F1558.unk1C);
    }
    if (D_801F1558.unk2C.unkC != 0) {
        func_800631AC(&D_801F1558.unk2C);
    }
}

INCLUDE_ASM("asm/us/ovl_34/nonmatchings/ovl_34", func_801F10C8);

void func_801F1168(void) {
    if (D_801F1558.unk3 != 0) {
        if (++D_801F1558.unk6 >= 9) {
            D_801F1558.unk3 = 0;
            D_801F1558.unk6 = 0;
            func_801F12D0(0);
        }
    }
}

INCLUDE_ASM("asm/us/ovl_34/nonmatchings/ovl_34", func_801F11C4);

void func_801F1250(void) {
    D_801F1558.unk2 = 0;
}

s32 func_801F125C(void) {
    return D_801F1558.unk1C.unkC != 0;
}

u8 func_801F126C(void) {
    s32 i;

    for (i = 0; i < 2; i++) {
        if (((D_80073940[i].unk0 >> 12) & 0xF) == 7) {
            return 1;
        }
        if (D_800739A8[i] == 6) {
            return 1;
        }
    }
    return 0;
}

void func_801F12D0(u8 on) {
    s32 i;

    if (on) {
        for (i = 0; i < 2; i++) {
            D_800739B0[i].unk1 = 0xFF;
            D_800739B0[i].unk0 = 1;
        }
        return;
    }
    for (i = 0; i < 2; i++) {
        D_800739B0[i].unk0 = 0;
        D_800739B0[i].unk1 = 0;
    }
}

INCLUDE_ASM("asm/us/ovl_34/nonmatchings/ovl_34", func_801F1334);
