#include "ovl_37.h"

INCLUDE_RODATA("asm/us/ovl_37/nonmatchings/ovl_37", D_801EDF00);

INCLUDE_ASM("asm/us/ovl_37/nonmatchings/ovl_37", func_801F0C18);

void func_801F0D18(void) {
    if (D_80077528 & 0x10000) {
        func_8002F488(0x65);
        func_800361CC(0);
        return;
    }
    if (D_80077528 & 0x20000) {
        func_8002F488(0x67);
        if (D_801FAB78.unk0 != 1) {
            D_801FAB78.unk1 = 2;
            return;
        }
        func_801F2D58();
        return;
    }
    if (D_80077528 & 0x80) {
        func_8002F488(0x67);
        D_801FAB78.unk0 = D_801FAB78.unk0 != 0 ? D_801FAB78.unk0 - 1 : 2;
        return;
    }
    if (D_80077528 & 0x20) {
        func_8002F488(0x67);
        D_801FAB78.unk0 = D_801FAB78.unk0 >= 2 ? 0 : D_801FAB78.unk0 + 1;
    }
}

INCLUDE_ASM("asm/us/ovl_37/nonmatchings/ovl_37", func_801F0E10);

INCLUDE_ASM("asm/us/ovl_37/nonmatchings/ovl_37", func_801F0F50);

INCLUDE_ASM("asm/us/ovl_37/nonmatchings/ovl_37", func_801F10C4);

INCLUDE_ASM("asm/us/ovl_37/nonmatchings/ovl_37", func_801F11E4);

INCLUDE_ASM("asm/us/ovl_37/nonmatchings/ovl_37", func_801F13F8);

INCLUDE_ASM("asm/us/ovl_37/nonmatchings/ovl_37", func_801F1778);

INCLUDE_ASM("asm/us/ovl_37/nonmatchings/ovl_37", func_801F187C);

s32 func_801F1A7C(void) {
    ItemMenu *m = &D_801FAB78;

    if (D_8006794C->unk8 == 3 && D_8007AEF0.b[0xBF] != 0 && D_8007AEF0.b[0xB5] == 0) {
        if (func_800B8124()) {
            D_8007AEF0.b[0xB5] = 1;
            func_800B81A0();
            m->unk4C = func_8002DC54(&D_801F3148, 0xF, 2);
            func_8002E2E4(m->unk4C, func_801F2B20);
            func_8002E2EC(m->unk4C, func_801F2B58);
            func_8002E304(m->unk4C, (s32)D_801F3140);
            func_8002E31C(m->unk4C, 1);
            m->unk50 = 1;
            m->unk1 = 0;
            func_8003563C(1);
            return 1;
        }
        return 0;
    }
    return 0;
}

INCLUDE_ASM("asm/us/ovl_37/nonmatchings/ovl_37", func_801F1B78);

void func_801F1D88(void) {
    if (D_801FAB78.unk54 != 0 && D_801FAB78.unk55 != 0) {
        if (D_801FAB78.unk55 == 1) {
            D_801FAB78.unk58 += D_801F3064[D_801FAB78.unk57++];
            if (D_801FAB78.unk57 >= 0x16) {
                D_801FAB78.unk55 = 2;
                D_801FAB78.unk57 = 0;
                D_801FAB78.unk1 = 6;
            }
        } else if (D_801FAB78.unk55 == 3) {
            D_801FAB78.unk58 += D_801F3064[D_801FAB78.unk57++];
            if (D_801FAB78.unk57 >= 8) {
                D_801FAB78.unk57 = 0;
                D_801FAB78.unk55 = 0;
                D_801FAB78.unk54 = 0;
                D_801FAB78.unk1 = 2;
                func_80028128(D_801FAB78.unk56);
                func_8003563C(0);
            }
        }
    }
}

INCLUDE_ASM("asm/us/ovl_37/nonmatchings/ovl_37", func_801F1E88);

void func_801F2160(void) {
    func_801F21E8();
    func_801F2240();
    func_801F2280();
}

void func_801F2190(void) {
    if (D_8006794C->unk8 == 3) {
        func_8002F55C(0x44A);
    }
    func_8003563C(0);
    func_80035674(1);
    func_8003556C(NULL);
    func_801F2F30();
}

void func_801F21E8(void) {
    void (*fn)(void);

    if (!func_8002F810()) {
        fn = D_801F307C[D_801FAB78.unk1];
        if (fn != NULL) {
            fn();
        }
    }
}

void func_801F2240(void) {
    func_801F262C();
    func_801F11E4();
    func_801F27A0();
    func_801F1E88();
    func_801F13F8();
}

void func_801F2280(void) {
    func_801F1B78();
    func_801F1D88();
    func_801F2BC0();
    func_800631AC(&D_801FAB78.unk3C);
    func_8002EB74(&D_801FAB78.unkC);
    func_8002EB74(&D_801FAB78.unk1C);
}

INCLUDE_ASM("asm/us/ovl_37/nonmatchings/ovl_37", func_801F22C8);

void func_801F23C4(void) {
    if (D_80077528 & 0x10000) {
        func_8002F488(0x65);
        func_801F2E18();
        return;
    }
    if (D_80077528 & 0x50) {
        func_8002F488(0x67);
        D_801FAB78.unk5 ^= 1;
        return;
    }
    if (D_80077528 & 0x20000) {
        if (D_801FAB78.unk5 == 0) {
            func_8002F488(0x67);
            func_801F187C();
            return;
        }
        func_8002F488(0x67);
        D_801FAB78.unk7 = 0;
        D_801FAB78.unk1 = 5;
    }
}

void func_801F2488(void) {
    if (D_80077528 & 0x10000) {
        func_8002F488(0x65);
        func_801F2FA0();
    } else if (D_80077528 & 0x20000) {
        func_8002F488(0x67);
        func_801F2FA0();
    }
}

INCLUDE_ASM("asm/us/ovl_37/nonmatchings/ovl_37", func_801F24DC);

void func_801F253C(s32 i) {
    if (i < D_801FAB78.unkB0) {
        func_801F2F38(D_801FAB78.unk60[i]);
        func_8002F488(0x67);
    } else {
        func_8002F488(0x66);
    }
}

void func_801F2590(s32 id) {
    Unk8006B1F4 *e;
    Character *c;
    s32 i;

    i = id - 0xE0;
    e = &D_8006B1F4[i];
    c = D_8006794C->unk1C->unk54C[D_801FAB78.unk2];
    if (func_800654F8(c, c, e, e->unk4, 0)) {
        func_8002F488(0x6A);
        func_80027E5C(id, 1);
    } else {
        func_8002F488(0x66);
    }
}

void func_801F262C(void) {
    RECT *r;
    s32 i;

    r = &D_801FAB78.unk2C;
    for (i = 0; i < 3; i++) {
        func_8002B5FC(r->x + D_801F309C[i], r->y + 6, 0x1C, func_80035774(D_801F3098[i]), 0);
    }
    func_8002DEE4(r, 0x1D, 2);
    func_8002705C(r, 0x1D, 2);
}

void func_801F26E0(void) {
    MenuView v;
    ItemList *it;
    s32 row;
    s32 col;
    s32 idx;

    it = &D_801F3040[D_801FAB78.unk0];
    row = 0;
    func_8002F344(it->unk8, &v);
    idx = v.unk1 * 2;
    for (; row < v.unk0; row++) {
        for (col = 0; col < 2; col++) {
            it->unk4(col, row, idx++, &v);
        }
    }
}

INCLUDE_ASM("asm/us/ovl_37/nonmatchings/ovl_37", func_801F27A0);

INCLUDE_ASM("asm/us/ovl_37/nonmatchings/ovl_37", func_801F2870);

INCLUDE_ASM("asm/us/ovl_37/nonmatchings/ovl_37", func_801F29D0);

void func_801F2B20(Window *w) {
    if (D_8006794C->unk8 == 3) {
        func_8002F514(0x44A);
    }
}

void func_801F2B58(Window *w) {
    u8 *s;

    s = func_80035774(0xB);
    func_8002B5FC(w->unk0 + ((w->unk4 - func_8002BA68((s32)s)) >> 1), w->unk2 + 6, w->unk18, s, 0);
}

void func_801F2BC0(void) {
    u32 t;

    if (D_801FAB78.unk1 == 0) {
        t = D_801FAB78.unk50;
        if (t != 0) {
            D_801FAB78.unk50 = t + 1;
            if (t >= 0x60) {
                D_801FAB78.unk50 = 0;
                if (D_8006794C->unk8 == 3) {
                    func_8002F55C(0x44A);
                }
                func_8003563C(0);
                func_80035674(1);
                func_8003556C(NULL);
                func_801F2F30();
                func_800362C8();
            }
        }
    }
}

void func_801F2C58(void) {
    ItemList *it;
    ItemMenu *m;

    it = D_801F3040;
    m = &D_801FAB78;
    func_80063118(&m->unk3C, D_801F3120[it[D_801FAB78.unk0].unk8->unk7 & 1], 0);
    m->unk1 = 0;
}

void func_801F2CC4(void) {
    D_801FAB78.unk1 = 3;
}

void func_801F2CD4(void) {
    D_801FAB78.unk1 = 2;
    D_801FAB78.unk8 = 0;
}

void func_801F2CEC(void) {
    s32 n;
    s32 i;

    n = 0;
    for (i = 0; i < 0x50; i++) {
        if ((u8)func_800281A4(i)) {
            D_801FAB78.unk60[n] = i;
            n++;
        }
    }
    D_801FAB78.unkB0 = n;
}

void func_801F2D58(void) {
    MenuList *l;

    l = D_801F3040[1].unk8;
    D_801FAB78.unk4C = func_8002DC54(&D_801F30B8, 0x14, 2);
    func_8002E2EC(D_801FAB78.unk4C, func_801F2E40);
    func_8002E2E4(D_801FAB78.unk4C, func_801F2EBC);
    func_8002E2FC(D_801FAB78.unk4C, func_801F2ECC);
    func_8002E31C(D_801FAB78.unk4C, 1);
    func_8002E304(D_801FAB78.unk4C, D_80070408);
    D_801FAB78.unk5 = 0;
    D_801FAB78.unk6 = l->unk7 + (l->unk5 + l->unk6) * 2;
    D_801FAB78.unk1 = 0;
}

void func_801F2E18(void) {
    func_8002DD00(D_801FAB78.unk4C);
}

INCLUDE_ASM("asm/us/ovl_37/nonmatchings/ovl_37", func_801F2E40);

void func_801F2EBC(Window *w) {
    D_801FAB78.unk1 = 4;
}

void func_801F2ECC(Window *w) {
    D_801FAB78.unk1 = 1;
}

void func_801F2EDC(void) {
    u8 *p = D_801EDFD8;

    LoadImage(&D_801F3158, (u32 *)p);
    DrawSync(0);
    LoadImage(&D_801F3160, (u32 *)(p + 0x40));
    DrawSync(0);
}

void func_801F2F30(void) {
}

void func_801F2F38(s32 id) {
    D_801FAB78.unk56 = id;
    D_801FAB78.unk54 = 1;
    D_801FAB78.unk55 = 1;
    D_801FAB78.unk1 = 0;
    D_801FAB78.unk57 = 0;
    D_801FAB78.unk58 = 0x48;
    D_801FAB78.unk58 -= func_800632B4(0x16, D_801F3064);
    func_8003563C(1);
}

void func_801F2FA0(void) {
    D_801FAB78.unk55 = 3;
    D_801FAB78.unk1 = 0;
    D_801FAB78.unk57 = 0;
    D_801FAB78.unk58 = 0x48;
}

INCLUDE_ASM("asm/us/ovl_37/nonmatchings/ovl_37", func_801F2FC4);

void func_801F3038(void) {
}

INCLUDE_RODATA("asm/us/ovl_37/nonmatchings/ovl_37", D_801EDFD8);
