#include "ovl_38.h"

void func_801EE440(void) {
    s32 i;
    RECT *r;

    D_801F120C.unk2 = 0xFF;
    D_801F120C.unk0 = 1;
    D_801F120C.unk1 = 0;
    for (i = 0; i < 6; i++) {
        D_801F120C.unk16[i] = 0;
        D_801F120C.unk10[i] = 0;
    }
    func_8003575C(D_801EE2A0);
    func_80028CF8(&D_801F120C.unk8C, D_80077E50.unk3, func_801EFAF8);
    func_801EF568(D_80077E50.unk3, 0);
    r = &D_801F120C.unk8;
    r->x = 8;
    r->y = 0x26;
    r->w = 0x12C;
    r->h = 0xB6;
    D_801F120C.unk5 = func_80035648();
    func_80035658(0);
    func_8003556C(func_801EFB08);
    D_801F120C.unk80[0] = (u8 *)0x801F5F00;
    D_801F120C.unk80[1] = (u8 *)0x801F9F00;
    D_801F120C.unk7C = D_801F120C.unk80[D_8006794C->unk1C->unk8];
}

void func_801EE568(void) {
    u8 cur;
    s32 next;

    if (D_80077528 & 0x10000) {
        func_8002F488(0x65);
        func_800361CC(0);
        return;
    }
    if (D_80077528 & 0x20000) {
        func_801EF494();
        return;
    }
    if (D_80077528 & 0x200000) {
        cur = D_801F120C.unk2;
        next = func_8003380C(cur);
        if (cur != next) {
            func_801EF568(next, 2);
            func_8002F488(0x417);
        }
    } else if (D_80077528 & 0x100000) {
        cur = D_801F120C.unk2;
        next = func_800337D0(cur);
        if (cur != next) {
            func_8002F488(0x417);
            func_801EF568(next, 1);
        }
    }
}

void func_801EE650(void) {
    RECT r;
    Character *c;
    RECT *w;
    s16 x;
    s16 y;

    w = &D_801F120C.unk8;
    c = D_8006794C->unk1C->unk54C[D_801F120C.unk2];
    x = w->x + 0x72;
    y = w->y + 4;
    setRECT(&r, x, y, 0x5C, 0x12);
    func_800273C0(x + 0x19, y + 0x14, 0x1E, c->unk17, c->unk1F);
    func_80024428(w->x + 8, w->y + 6, 0x1D, (Unk80024428 *)&D_801F120C.unk8C);
    func_80026FC4(r.x + 4, r.y + 4, 0x1D, c->unk38, 0);
    func_8002DEE4(&r, 0x1D, 0);
}

INCLUDE_ASM("asm/us/ovl_38/nonmatchings/ovl_38", func_801EE740);

INCLUDE_ASM("asm/us/ovl_38/nonmatchings/ovl_38", func_801EE9E8);

INCLUDE_ASM("asm/us/ovl_38/nonmatchings/ovl_38", func_801EED18);

INCLUDE_RODATA("asm/us/ovl_38/nonmatchings/ovl_38", D_801EDF00);

INCLUDE_RODATA("asm/us/ovl_38/nonmatchings/ovl_38", D_801EDF04);

INCLUDE_RODATA("asm/us/ovl_38/nonmatchings/ovl_38", D_801EE144);

INCLUDE_ASM("asm/us/ovl_38/nonmatchings/ovl_38", func_801EEF00);

INCLUDE_ASM("asm/us/ovl_38/nonmatchings/ovl_38", func_801EF0A0);

INCLUDE_ASM("asm/us/ovl_38/nonmatchings/ovl_38", func_801EF258);

void func_801EF494(void) {
    WinDef *w;

    if (D_801F120C.unk1 >= D_801F120C.unk3) {
        func_801EF9D4();
        return;
    }
    D_801F120C.unk0 = D_801F120C.unk1 != 0 ? 3 : 0;
    func_8002F488(0x417);
    w = &D_801F120C.unk1C[D_801F120C.unk1];
    D_801F120C.unk10[D_801F120C.unk1] = 1;
    D_801F120C.unk16[D_801F120C.unk1] = 1;
    w->unk4 = D_801F10E4[D_801F120C.unk1];
    func_80063118(w, &D_801F1134, 1);
}

void func_801EF568(s32 slot, s32 dir) {
    Character *c;
    s32 i;

    c = D_8006794C->unk1C->unk54C[slot];
    func_80028D08(&D_801F120C.unk8C, D_801F120C.unk2, slot, dir);
    if (dir != 0) {
        D_801F120C.unk0 = 3;
    }
    D_801F120C.unk2 = slot;
    D_801F120C.unk3 = 0;
    D_801F120C.unk4 = 0;
    if ((s32)((c->unk34 >> 12) & 0x1F) < 0x10) {
        for (i = 0; i < 0x30; i++) {
            if (D_8006CB28[(c->unk34 >> 12) & 0x1F][i].unk0 != 0) {
                D_801F120C.unk4++;
            }
        }
        D_801F120C.unk3 = (D_801F120C.unk4 + 7) / 8;
    }
}

void func_801EF69C(void) {
    func_801EF708();
    func_801EF760();
    func_801EF80C();
}

void func_801EF6CC(void) {
    D_80077E53 = D_801F120C.unk2;
    func_80035658(D_801F120C.unk5);
    func_8003556C(NULL);
}

void func_801EF708(void) {
    void (*fn)(void);

    if (!func_8002F810()) {
        fn = D_801EFBA4[D_801F120C.unk0];
        if (fn != NULL) {
            fn();
        }
    }
}

void func_801EF760(void) {
    u8 *p = D_801F120C.unk7C - 0x4000;

    D_801F120C.unk88 = D_801F120C.unk80[D_8006794C->unk1C->unk8 ^ 1] - p;
    D_801F120C.unk7C = D_801F120C.unk80[D_8006794C->unk1C->unk8];
    func_801EF904();
    func_801EE650();
    func_801EE740();
    func_801EE9E8();
    func_801EED18();
    func_801EEF00();
    func_801EF0A0();
    func_801EF9B4();
}

void func_801EF80C(void) {
    s32 i;

    for (i = 0; i < 6; i++) {
        if (D_801F120C.unk10[i] != 0 && D_801F120C.unk1C[i].unkC != 0) {
            func_800631AC(&D_801F120C.unk1C[i]);
        }
    }
    func_80028D2C(&D_801F120C.unk8C);
}

void func_801EF8A0(void) {
    if (D_80077528 & 0x10000) {
        func_8002F488(0x65);
        func_801EF9D4();
        return;
    }
    if (D_80077528 & 0x20000) {
        func_8002F488(0x67);
        func_801EF494();
    }
}

void func_801EF904(void) {
    RECT *r = &D_801F120C.unk8;

    func_800269F4(r->x + 0x6D, r->y + 0x63, 0x1E, r->w - 0x71);
    func_800269F4(r->x + 0x6B, r->y + 0x38, 0x1E, 0x3E - r->h);
    func_800269F4(r->x + 4, r->y + 0x36, 0x1E, r->w - 9);
    func_8002DEE4(r, 0x1E, 2);
    func_8002705C(r, 0x1E, 2);
}

void func_801EF9B4(void) {
    func_8002F810();
}

void func_801EF9D4(void) {
    s32 i;

    if (D_801F120C.unk1 != 0) {
        D_801F120C.unk0 = 0;
        for (i = 0; i < D_801F120C.unk1; i++) {
            if (D_801F120C.unk10[i] != 0) {
                D_801F114C[i].unk0 = D_801F120C.unk1 - 1 - i;
                func_80063118(&D_801F120C.unk1C[i], &D_801F114C[i], 0);
            }
        }
        func_8002F488(0x417);
    }
}

void func_801EFA80(void) {
    D_801F120C.unk0 = 2;
    if (D_801F120C.unk1 != 0) {
        D_801F120C.unk16[D_801F120C.unk1 - 1] = 0;
    }
    D_801F120C.unk1++;
}

void func_801EFABC(void) {
    s32 i;

    D_801F120C.unk0 = 1;
    for (i = 0; i < 6; i++) {
        D_801F120C.unk16[i] = 0;
        D_801F120C.unk10[i] = 0;
    }
    D_801F120C.unk1 = 0;
}

void func_801EFAF8(void) {
    D_801F120C.unk0 = 1;
}

INCLUDE_ASM("asm/us/ovl_38/nonmatchings/ovl_38", func_801EFB08);

INCLUDE_ASM("asm/us/ovl_38/nonmatchings/ovl_38", func_801EFBA8);

INCLUDE_RODATA("asm/us/ovl_38/nonmatchings/ovl_38", D_801EE2A0);
