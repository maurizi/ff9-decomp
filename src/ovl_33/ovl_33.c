#include "ovl_33.h"

INCLUDE_ASM("asm/us/ovl_33/nonmatchings/ovl_33", func_801EE7C0);

void func_801EE978(void) {
    EquipMenu *m;

    if (func_8003561C()) {
        func_801F0024();
    }
    m = &D_801FBF84;
    if (m->unk1 == 2) {
        func_8002EB74(&D_801FBF84.unk8[D_801FBF84.unk0]);
    }
    if (m->unk58.unkC != 0) {
        func_800631AC(&m->unk58);
    }
    if (++m->unk76B >= 7) {
        m->unk76B = 0;
        if (++m->unk76C >= 7) {
            m->unk76C = 0;
        }
    }
    if (D_801FBF84.unk770.unk0 != 0) {
        func_80028D2C(&D_801FBF84.unk770);
    }
}

INCLUDE_ASM("asm/us/ovl_33/nonmatchings/ovl_33", func_801EEA50);

INCLUDE_ASM("asm/us/ovl_33/nonmatchings/ovl_33", func_801EEBE0);

INCLUDE_ASM("asm/us/ovl_33/nonmatchings/ovl_33", func_801EED10);

void func_801EEEA8(void) {
    RECT box;
    RECT *r;
    Character *c;

    r = &D_801FBF84.unk30;
    c = D_8006794C->unk1C->unk54C[D_801FBF84.unk2];
    func_80024428(r->x + 8, r->y + 5, 0x1D, (Unk80024428 *)&D_801FBF84.unk770);
    box.w = 0x5B;
    box.h = 0x12;
    box.x = r->x + 0x72;
    box.y = r->y + 4;
    func_80026FC4(box.x + 3, box.y + 4, 0x1D, c->unk38, 0);
    func_8002DEE4(&box, 0x1D, 0);
    if (D_801FBF84.unk0 == 1) {
        func_800273C0(box.x + 0x19, box.y + 0x14, 0x1D, c->unk17, c->unk1F);
    }
    func_8002B524(r->x + 6, r->y - 3, 0x1D, D_80070498, 0);
    func_8002DEE4(r, 0x1D, 1);
}

INCLUDE_ASM("asm/us/ovl_33/nonmatchings/ovl_33", func_801EEFDC);

void func_801EF270(void) {
    RECT *r;
    EquipMenu *m;
    MenuList *l;
    Character *c;
    s32 idx;

    r = &D_801FBF84.unk40;
    m = &D_801FBF84;
    l = &D_801FBF84.unk8[D_801FBF84.unk0];
    idx = (l->unk6 + l->unk5) * 2 + l->unk7;
    c = D_8006794C->unk1C->unk54C[m->unk2];
    if (m->unk0 != 1) {
        if (m->unk1 != 1 && m->unk1 != 5 && m->unk1 != 0) {
            if (func_801EFB8C(m->unk668[idx].unk0, c) != 0) {
                func_8002B5FC(r->x + 8, r->y + 6, 0x1B, D_801F722C + D_801F6FEC[m->unk668[idx].unk1].unk0, 0);
            }
        }
        func_8002B524(r->x + 6, r->y - 3, 0x1B, D_8007042C, 0);
        func_8002DEE4(r, 0x1B, 2);
    }
}

INCLUDE_ASM("asm/us/ovl_33/nonmatchings/ovl_33", func_801EF3A8);

INCLUDE_ASM("asm/us/ovl_33/nonmatchings/ovl_33", func_801EF560);

INCLUDE_ASM("asm/us/ovl_33/nonmatchings/ovl_33", func_801EF644);

INCLUDE_ASM("asm/us/ovl_33/nonmatchings/ovl_33", func_801EF900);

s32 func_801EFB8C(s32 id, Character *c) {
    Unk801F34EC *info;
    s32 slot;
    s32 ap;
    s32 need;

    info = &D_801F34EC[id];
    if (D_801FBF84.unk68[id] == 0) {
        slot = func_80034430(c->unk34 & 0xF, id);
        if (slot < 0) {
            return 0;
        }
        if (func_80034A30(c)) {
            ap = c->unk58[slot];
            need = D_8006CB28[(c->unk34 >> 12) & 0x1F][slot].unk1;
            if (ap == 0) {
                return 0;
            }
            if (ap < need) {
                return 0;
            }
        }
    }
    if (c->unk38 & 9) {
        return 2;
    }
    if (!(info->unkB & 1)) {
        return 2;
    }
    if (func_801F1018(info) > c->unk12) {
        return 2;
    }
    return 3;
}

INCLUDE_ASM("asm/us/ovl_33/nonmatchings/ovl_33", func_801EFCB4);

INCLUDE_ASM("asm/us/ovl_33/nonmatchings/ovl_33", func_801EFE2C);

INCLUDE_ASM("asm/us/ovl_33/nonmatchings/ovl_33", func_801F0024);

INCLUDE_ASM("asm/us/ovl_33/nonmatchings/ovl_33", func_801F0410);

void func_801F05FC(void) {
    func_801F0664();
    func_801F06BC();
    func_801EE978();
}

void func_801F062C(void) {
    D_80077E53 = D_801FBF84.unk2;
    func_80035674(1);
    func_8003556C(NULL);
}

void func_801F0664(void) {
    void (*fn)(void);

    if (!func_8002F810()) {
        fn = D_801F33A8[D_801FBF84.unk1];
        if (fn != NULL) {
            fn();
        }
    }
}

void func_801F06BC(void) {
    func_801F07DC();
    func_801EEEA8();
    func_801EEFDC();
    func_801EF270();
    func_801EF3A8();
    func_801EF560();
    func_801EF644();
}

INCLUDE_ASM("asm/us/ovl_33/nonmatchings/ovl_33", func_801F070C);

void func_801F07DC(void) {
    RECT *r;
    s32 i;

    r = &D_801FBF84.unk28;
    func_8002DEE4(r, 0x1D, 2);
    for (i = 0; i < 2; i++) {
        func_8002B5FC(r->x + D_801F33C0[i], r->y + 6, 0x1C, func_80035774(i), D_801FBF84.unk4[i] == 0 ? 2 : 0);
    }
}

void func_801F0890(void) {
    MenuView v;
    s32 row;
    s32 col;
    s32 idx;

    row = 0;
    func_8002F344(&D_801FBF84.unk8[D_801FBF84.unk0], &v);
    idx = v.unk1 * 2;
    for (; row < v.unk0; row++) {
        for (col = 0; col < 2; col++) {
            func_801F0934(col, row, idx++, &v);
        }
    }
}

INCLUDE_ASM("asm/us/ovl_33/nonmatchings/ovl_33", func_801F0934);

void func_801F09CC(s32 x, s32 y, s32 idx, Character *c) {
    u8 buf[8];
    u8 id;
    Unk801F34EC *info;
    s32 st;
    s32 col;

    id = D_801FBF84.unk668[idx].unk0;
    info = &D_801F34EC[id];
    st = func_801EFB8C(id, c);
    if (st != 0) {
        col = D_801F740C[st];
        func_8002B5FC(x + 8, y, 0x1C, D_801F40EC + info->unkE, col);
        if (info->unkA != 0) {
            func_8002BAE4(func_801F1018(info), buf, 3, 1);
            func_8002B56C(x + 0x6C, y + 4, 0x1C, buf, col);
        }
    }
}

INCLUDE_ASM("asm/us/ovl_33/nonmatchings/ovl_33", func_801F0AB8);

u8 func_801F0C10(s32 id, Character *c) {
    s32 i;
    s32 j;
    u8 k;
    Unk801F6FEC *e;

    for (i = 0; i < 2; i++) {
        k = D_801F33D0[(c->unk34 >> 12) & 0x1F][i];
        e = &D_801F6FEC[k];
        if (e->unk6 == 1) {
            for (j = 0; j < e->unk7; j++) {
                if (id == D_801F734C[e->unk8 + j]) {
                    return k;
                }
            }
        }
    }
    return 0;
}

u32 func_801F0CC4(s32 id, Character *c) {
    Unk8006D128 *sup;
    s32 slot;
    s32 ap;
    s32 need;

    if (id < 0xC0) {
        return 0;
    }
    if (func_80034A04(c->unk88, id)) {
        return 4;
    }
    if (D_801FBF84.unk68[id] == 0) {
        slot = func_80034430(c->unk34 & 0xF, id);
        if (slot < 0) {
            return 0;
        }
        if (func_80034A30(c)) {
            ap = c->unk58[slot];
            need = D_8006CB28[(c->unk34 >> 12) & 0x1F][slot].unk1;
            if (ap == 0) {
                return 0;
            }
            if (ap < need) {
                return 0;
            }
        }
    }
    sup = &D_8006D128[id - 0xC0];
    if (sup->unk1 > c->unk17) {
        return 2;
    }
    return 3;
}

INCLUDE_ASM("asm/us/ovl_33/nonmatchings/ovl_33", func_801F0DDC);

void func_801F0E3C(void) {
    D_801FBF84.unk1 = 3;
}

void func_801F0E4C(void) {
    D_801FBF84.unk1 = 2;
    D_801FBF84.unk768 = 0;
}

INCLUDE_ASM("asm/us/ovl_33/nonmatchings/ovl_33", func_801F0E64);

void func_801F0ED4(void) {
    D_801FBF84.unk1 = 1;
}

void func_801F0EE4(void) {
    u8 *p = D_801EE164;

    LoadImage(&D_801F3484, (u32 *)p);
    DrawSync(0);
    LoadImage(&D_801F348C, (u32 *)(p + 0x120));
    DrawSync(0);
}

void func_801F0F38(s32 x, s32 y, s32 otz, s32 i) {
    func_80027710(x, y, otz, func_801F0F90(i), 0);
}

Icon *func_801F0F90(s32 i) {
    return &D_801F3494[i];
}

INCLUDE_ASM("asm/us/ovl_33/nonmatchings/ovl_33", func_801F0FA4);

s32 func_801F1018(Unk801F34EC *p) {
    s32 v;

    v = p->unkA;
    if ((p->unkB & 4) && D_8007AF02 != 0) {
        v *= 4;
    }
    return v;
}

void func_801F1048(void) {
    Character *c;
    MenuList *l;
    s32 i;
    s32 sel;
    s32 row;
    s32 top;

    sel = 0;
    i = 0;
    c = D_8006794C->unk1C->unk54C[D_801FBF84.unk2];
    l = &D_801FBF84.unk8[0];
    for (; i < D_801FBF84.unk769; i++) {
        if (func_801EFB8C(D_801FBF84.unk668[i].unk0, c) == 3) {
            sel = i;
            break;
        }
    }
    row = sel / 2;
    l->unk7 = sel % 2;
    top = row - 7;
    if (top >= 0) {
        l->unk5 = top;
    } else {
        l->unk5 = 0;
    }
    l->unk6 = row - l->unk5;
}

void func_801F1148(void) {
}

INCLUDE_ASM("asm/us/ovl_33/nonmatchings/ovl_33", func_801F1150);

INCLUDE_ASM("asm/us/ovl_33/nonmatchings/ovl_33", func_801F12C8);

INCLUDE_ASM("asm/us/ovl_33/nonmatchings/ovl_33", func_801F1454);

INCLUDE_ASM("asm/us/ovl_33/nonmatchings/ovl_33", func_801F15E0);

INCLUDE_ASM("asm/us/ovl_33/nonmatchings/ovl_33", func_801F189C);

INCLUDE_ASM("asm/us/ovl_33/nonmatchings/ovl_33", func_801F1A94);

INCLUDE_ASM("asm/us/ovl_33/nonmatchings/ovl_33", func_801F1F08);

INCLUDE_ASM("asm/us/ovl_33/nonmatchings/ovl_33", func_801F20A0);

INCLUDE_ASM("asm/us/ovl_33/nonmatchings/ovl_33", func_801F239C);

INCLUDE_ASM("asm/us/ovl_33/nonmatchings/ovl_33", func_801F24EC);

INCLUDE_ASM("asm/us/ovl_33/nonmatchings/ovl_33", func_801F26C4);

INCLUDE_ASM("asm/us/ovl_33/nonmatchings/ovl_33", func_801F2894);

INCLUDE_ASM("asm/us/ovl_33/nonmatchings/ovl_33", func_801F2AB4);

void func_801F2C0C(void) {
    func_801F2C78();
    func_801F2CD0();
    func_801F2D18();
}

void func_801F2C3C(void) {
    D_80077E53 = D_801FC840.unk3;
    func_8002DE60();
    func_80035674(1);
    func_8003556C(NULL);
}

void func_801F2C78(void) {
    void (*fn)(void);

    if (!func_8002F810()) {
        fn = D_801F7420[D_801FC840.unk1];
        if (fn != NULL) {
            fn();
        }
    }
}

void func_801F2CD0(void) {
    func_801F2E34();
    func_801F15E0();
    func_801F189C();
    func_801F1A94();
    func_801F1F08();
    func_801F239C();
}

void func_801F2D18(void) {
    if (func_8003561C()) {
        func_801F2AB4();
    }
    if (D_801FC840.unk1 == 3 && D_801FC840.unk4.unk8 != 0) {
        func_8002EB74(&D_801FC840.unk4);
    }
    if (D_801FC840.unk23C.unkC != 0) {
        func_800631AC(&D_801FC840.unk23C);
    }
    if (D_801FC840.unk24C.unk0 != 0) {
        func_80028D2C(&D_801FC840.unk24C);
    }
}

void func_801F2DB4(void) {
    if (D_801FC840.unk4.unk8 == 0) {
        if (D_80077528 & 0x10000) {
            func_8002F488(0x65);
            func_801F307C();
            return;
        }
        if (D_80077528 & 0x20000) {
            func_801F30D8();
            return;
        }
        func_8002EE8C(&D_801FC840.unk4, D_80077528);
    }
}

void func_801F2E34(void) {
    RECT *r;
    s32 i;

    r = &D_801FC840.unk14;
    func_8002DEE4(r, 0x1E, 2);
    func_8002705C(r, 0x1E, 2);
    for (i = 0; i < 3; i++) {
        func_8002B5FC(r->x + D_801F7430[i], r->y + 6, 0x1D, func_80035774(i), 0);
    }
}

void func_801F2ED4(void) {
    MenuView v;
    s32 i;

    func_8002F344(&D_801FC840.unk4, &v);
    for (i = 0; i < v.unk0; i++) {
        func_801F2F3C(0, i, v.unk1 + i, &v);
    }
}

INCLUDE_ASM("asm/us/ovl_33/nonmatchings/ovl_33", func_801F2F3C);

void func_801F301C(void) {
    WinDef *w = &D_801FC840.unk23C;

    w->unk4 = D_801FC840.unk34;
    D_801FC840.unk260 = 1;
    D_801FC840.unk1 = 0;
    func_80063118(w, D_801FBD54, 1);
}

void func_801F307C(void) {
    WinDef *w = &D_801FC840.unk23C;

    D_801FC840.unk1 = 0;
    func_80063118(w, D_801FBD74, 0);
}

void func_801F30B0(void) {
    D_801FC840.unk1 = 3;
}

void func_801F30C0(void) {
    D_801FC840.unk1 = 2;
    D_801FC840.unk260 = 0;
}

void func_801F30D8(void) {
    GameState *gs;
    AbilityMenu *m;
    MenuList *l;
    Character *c;
    s32 old;
    s32 item;

    gs = D_8006794C;
    m = &D_801FC840;
    l = &m->unk4;
    c = gs->unk1C->unk54C[m->unk3];
    old = c->unk39[m->unk2];
    item = m->unk3C[l->unk5 + l->unk6].unk0;
    if (l->unk2 == 0) {
        func_8002F488(0x66);
        return;
    }
    if (old != 0xFF) {
        func_80027D84(old, 1);
    }
    if (func_80027E5C(item, 1) != 0) {
        c->unk39[m->unk2] = item;
        func_801F32E0(c);
        func_801F307C();
    }
    func_8002F488(0x6B);
}

void func_801F31B0(void) {
    s32 se;
    Character *c;
    u8 ok;
    u8 old;

    ok = 0;
    if (D_801FC840.unk2 != 0) {
        c = D_8006794C->unk1C->unk54C[D_801FC840.unk3];
        old = c->unk39[D_801FC840.unk2];
        if (old != 0xFF) {
            func_80027D84(old, 1);
            c->unk39[D_801FC840.unk2] = 0xFF;
            func_801F32E0(c);
            ok = 1;
        }
    }
    se = 0x6B;
    if (!ok) {
        se = 0x66;
    }
    func_8002F488(se);
}

void func_801F3264(void) {
    D_801FC840.unk1 = D_801FC840.unk261;
}

void func_801F3278(s32 slot, s32 dir) {
    u8 s;

    func_80028D08(&D_801FC840.unk24C, D_801FC840.unk3, slot, dir);
    if (dir != 0) {
        s = D_801FC840.unk1;
        D_801FC840.unk1 = 0;
        D_801FC840.unk261 = s;
    }
    D_801FC840.unk3 = slot;
}

void func_801F32E0(Character *c) {
    func_801F26C4();
    func_8003384C(c);
    c->unk34 = (c->unk34 & ~0x1F0) | ((func_80033280(c->unk34 & 0xF, (c->unkA >> 4) & 1, c->unk39) & 0x1F) << 4);
}

INCLUDE_ASM("asm/us/ovl_33/nonmatchings/ovl_33", func_801F3344);

INCLUDE_ASM("asm/us/ovl_33/nonmatchings/ovl_33", func_801F33AC);

INCLUDE_RODATA("asm/us/ovl_33/nonmatchings/ovl_33", D_801EDF00);

INCLUDE_RODATA("asm/us/ovl_33/nonmatchings/ovl_33", D_801EDF04);

INCLUDE_RODATA("asm/us/ovl_33/nonmatchings/ovl_33", D_801EE164);

INCLUDE_RODATA("asm/us/ovl_33/nonmatchings/ovl_33", D_801EE644);

INCLUDE_RODATA("asm/us/ovl_33/nonmatchings/ovl_33", D_801EE648);
