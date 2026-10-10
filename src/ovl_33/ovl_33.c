#include "ovl_33.h"

/* Inlined copy of func_801F0CC4 (support ability state). */
static inline s32 func_801F0CC4_inline(s32 id, Character *c) {
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
        slot = func_80034430(c->unk34.b.id, id);
        if (slot < 0) {
            return 0;
        }
        if (func_80034A30(c)) {
            ap = c->unk58[slot];
            need = D_8006CB28[c->unk34.b.row][slot].unk1;
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

void func_801EE7C0(void) {
    s32 i;

    D_801FBF84.unk2 = 0xFF;
    D_801FBF84.unk1 = 1;
    D_801FBF84.unk0 = 0;
    func_80028CF8(&D_801FBF84.unk770, D_80077E50.unk3, func_801F0ED4);
    func_801EF900(D_80077E50.unk3, 0);
    D_801FBF84.unk76C = 0;
    D_801FBF84.unk76B = 0;
    func_8003575C(D_801EDF04);
    func_801F1148();
    func_8003556C(D_800355DC);
    func_80035674(1);
    setRECT(&D_801FBF84.unk28, 8, 8, 0xE4, 0x1A);
    setRECT(&D_801FBF84.unk30, 8, 0x26, 0xD0, 0x38);
    setRECT(&D_801FBF84.unk38, 0xD8, 0x26, 0x5C, 0x38);
    setRECT(&D_801FBF84.unk40, 0x8E, 0x40, 0x46, 0x1A);
    setRECT(&D_801FBF84.unk48, 8, 0x5E, 0x12C, 0x80);
    setRECT(&D_801FBF84.unk50, 0, 0x26, 0x90, 0xB6);
    D_801FBF84.unk768 = 0;
    func_80063118(&D_801FBF84.unk58, 0, 0);
    func_801F0EE4();
    D_801FBF84.unk76E = 0;
    for (i = 0; i < 4; i++) {
        if (D_8006794C->unk1C->unk54C[i] != NULL) {
            D_801FBF84.unk76E++;
        }
    }
}

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

void func_801EEA50(void) {
    u8 slot;
    s32 next;

    if (D_80077528 & 0x10000) {
        func_8002F488(0x65);
        func_800361CC(0);
    } else if (D_80077528 & 0x80000) {
        func_8002F488(0x67);
        func_80036270(3);
    } else if (D_80077528 & 0x80) {
        func_8002F488(0x67);
        D_801FBF84.unk0 = D_801FBF84.unk0 != 0 ? D_801FBF84.unk0 - 1 : 1;
    } else if (D_80077528 & 0x20) {
        func_8002F488(0x67);
        D_801FBF84.unk0 = D_801FBF84.unk0 == 0 ? D_801FBF84.unk0 + 1 : 0;
    } else if (D_80077528 & 0x200000) {
        slot = D_801FBF84.unk2;
        next = func_8003380C(slot);
        if (slot != next) {
            func_801EF900(next, 2);
            func_8002F488(0x417);
        }
    } else if (D_80077528 & 0x100000) {
        slot = D_801FBF84.unk2;
        next = func_800337D0(slot);
        if (slot != next) {
            func_801EF900(next, 1);
            func_8002F488(0x417);
        }
    } else if (D_80077528 & 0x20000) {
        if (D_801FBF84.unk4[D_801FBF84.unk0] != 0) {
            func_8002F488(0x67);
            D_801FBF84.unk1 = 2;
        } else {
            func_8002F488(0x66);
        }
    }
}

void func_801EEBE0(void) {
    s32 cur;
    s32 next;

    if (D_80077528 & 0x10000) {
        func_8002F488(0x65);
        func_801F0DDC();
        return;
    }
    if (D_80077528 & 0x10) {
        if (!(D_801FBF84.unk3 & 0x80)) {
            cur = D_801FBF84.unk3;
            next = func_800337D0(cur);
            if (cur != next) {
                D_801FBF84.unk3 = next;
                func_8002F488(0x67);
            }
        }
    } else if (D_80077528 & 0x40) {
        if (!(D_801FBF84.unk3 & 0x80)) {
            cur = D_801FBF84.unk3;
            next = func_8003380C(cur);
            if (cur != next) {
                D_801FBF84.unk3 = next;
                func_8002F488(0x67);
            }
        }
    } else if (D_80077528 & 0x3000A0) {
        if (D_801FBF84.unk76F != 0) {
            func_8002F488(0x67);
            D_801FBF84.unk3 ^= 0x80;
        }
    } else if (D_80077528 & 0x20000) {
        func_801EED10();
    }
}

void func_801EED10(void) {
    u8 ok;
    MenuList *l;
    Character *c;
    Unk801F34EC *it;
    s32 i;
    s32 cost;

    ok = 0;
    c = D_8006794C->unk1C->unk54C[D_801FBF84.unk2];
    l = &D_801FBF84.unk8[D_801FBF84.unk0];
    it = &D_801F34EC[D_801FBF84.unk668[(l->unk5 + l->unk6) * 2 + l->unk7].unk0];
    if (!(D_801FBF84.unk3 >> 7)) {
        ok = func_800654F8(c, D_8006794C->unk1C->unk54C[D_801FBF84.unk3], it, it->unk4, 0);
    } else {
        for (i = 0; i < 4; i++) {
            if (D_8006794C->unk1C->unk54C[i] != NULL) {
                ok |= func_800654F8(c, D_8006794C->unk1C->unk54C[i], it, it->unk4, 1);
            }
        }
    }
    if (ok) {
        func_8002F488(0x6A);
        cost = func_801F1018(it);
        c->unk12 -= cost;
        if (c->unk12 < cost) {
            func_801F0DDC();
        }
    } else {
        func_8002F488(0x66);
    }
}

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

void func_801EEFDC(void) {
    RECT *r;
    MenuList *l;
    Character *c;
    s32 n;
    s32 id;
    s32 ok;
    u8 flag;
    s32 i;
    s32 x;

    r = &D_801FBF84.unk38;
    c = D_8006794C->unk1C->unk54C[D_801FBF84.unk2];
    l = &D_801FBF84.unk8[D_801FBF84.unk0];
    n = (l->unk5 + l->unk6) * 2 + l->unk7;
    func_8002B524(r->x + 6, r->y - 3, 0x1D, D_80070460, 0);
    func_8002DEE4(r, 0x1D, 2);
    func_8002B5FC(r->x + 8, r->y + 8, 0x1C, func_80035774(0x10), 0);
    func_8002B5FC(r->x + 8, r->y + 0x16, 0x1C, func_80035774(0x11), 0);
    if (D_801FBF84.unk1 == 1 || D_801FBF84.unk1 == 5 || D_801FBF84.unk1 == 0 || l->unk0 == 0) {
        return;
    }
    if (D_801FBF84.unk0 == 0) {
        id = D_801FBF84.unk668[n].unk0;
        ok = func_801EFB8C(id, c);
        flag = !(D_801F34EC[id].unkB & 2);
    } else {
        id = D_801FBF84.unk728[n];
        ok = func_801F0CC4(id, c);
        flag = 1;
    }
    if (ok == 0) {
        return;
    }
    if (func_80034A30(c)) {
        func_80027110(r->x + 0x2D, r->y + 0xC, 0x1C, func_80034584(c->unk34.b.id, id), func_800347D8(c->unk34.b.id, id), flag, 0);
    }
    if (D_801FBF84.unk68[id] != 0) {
        x = r->x;
        for (i = 0; i < 5; i++) {
            if ((D_801FBF84.unk68[id] >> i) & 1) {
                func_80026C34(x + 8, r->y + 0x24, 0x1C, D_801FBF84.unk168[id][i], 1);
                x += 0x10;
            }
        }
    }
}

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

void func_801EF3A8(void) {
    RECT sub;
    MenuView view;
    s32 row;
    s32 col;
    s32 idx;
    MenuList *l;
    RECT *r = &D_801FBF84.unk48;

    l = &D_801FBF84.unk8[D_801FBF84.unk0];
    func_8002B524(r->x + 0xE, r->y - 3, 0x1C, D_800703FC.unk4, 0);
    func_8002B524(r->x + 0xA4, r->y - 3, 0x1C, D_800703FC.unk4, 0);
    func_8002B524(r->x + 0x7C, r->y - 3, 0x1C, D_800703FC.unk7C, 0);
    func_8002B524(r->x + 0x112, r->y - 3, 0x1C, D_800703FC.unk7C, 0);
    func_8002E040(r, 0x1D, 1, l);
    if (l->unk0 != 0) {
        func_8002ECA0(l, &view);
        func_8002D9B4(NULL, 0x1C, 1);
        idx = view.unk1 * 2;
        for (row = 0; row < view.unk0; row++) {
            for (col = 0; col < 2; col++) {
                func_801F0934(col, row, idx++, &view);
            }
        }
        if (l->unk8 & 4) {
            func_8002F40C(l, r, &sub);
            func_8002D9B4(&sub, 0x1C, 1);
            func_801F0890();
        }
        func_8002D9B4(r, 0x1C, 1);
    }
}

void func_801EF560(void) {
    RECT *r = &D_801FBF84.unk58.unk4;
    EquipMenu *m = &D_801FBF84;
    s32 i;

    if (m->unk768 != 0) {
        func_8002B524(r->x + 8, r->y - 3, 0x14, D_800704B8, 0);
        func_8002DEE4(r, 0x14, 2);
        for (i = 0; i < 4; i++) {
            if (D_8006794C->unk1C->unk54C[i] != NULL) {
                func_80026EA0(r->x + 8, r->y + 6 + i * 0x2B, 0x13, i, D_801FBF84.unk76D);
            }
        }
    }
}

INCLUDE_ASM("asm/us/ovl_33/nonmatchings/ovl_33", func_801EF644);

INCLUDE_ASM("asm/us/ovl_33/nonmatchings/ovl_33", func_801EF900);

s32 func_801EFB8C(s32 id, Character *c) {
    Unk801F34EC *info;
    s32 slot;
    s32 ap;
    s32 need;

    info = &D_801F34EC[id];
    if (D_801FBF84.unk68[id] == 0) {
        slot = func_80034430(c->unk34.b.id, id);
        if (slot < 0) {
            return 0;
        }
        if (func_80034A30(c)) {
            ap = c->unk58[slot];
            need = D_8006CB28[c->unk34.b.row][slot].unk1;
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

void func_801EFCB4(void) {
    MenuList *l = D_801FBF84.unk8 + D_801FBF84.unk0;
    MenuList *ls;
    Character *c = D_8006794C->unk1C->unk54C[D_801FBF84.unk2];
    s32 i;
    u8 id;

    if (l->unk2 != 0) {
        i = (l->unk5 + l->unk6) * 2 + l->unk7;
        if (i < D_801FBF84.unk769) {
            id = D_801FBF84.unk668[i].unk0;
            if (func_801EFB8C(id, c) == 3) {
                D_801FBF84.unk76D = D_801F34EC[id].unk0_5;
                D_801FBF84.unk76F = func_801F0E64(id);
                ls = D_801FBF84.unk8 + D_801FBF84.unk0;
                D_801FBF84.unk58.unk4 = D_801FBF84.unk50;
                if (ls->unk7 == 0) {
                    D_801FBF84.unk58.unk4.x = 0xA4;
                    func_80063118(&D_801FBF84.unk58, D_801F3428, 1);
                } else {
                    D_801FBF84.unk58.unk4.x = 8;
                    func_80063118(&D_801FBF84.unk58, D_801F3418, 1);
                }
                D_801FBF84.unk768 = 1;
                D_801FBF84.unk3 = func_8003378C();
                D_801FBF84.unk1 = 4;
                func_8002F488(0x67);
            } else {
                func_8002F488(0x66);
            }
        }
    }
}

void func_801EFE2C(void) {
    MenuList *l;
    Character *c;
    s32 idx;
    s32 id;
    u8 ap;
    Unk8006D128 *sup;
    s32 st;

    l = &D_801FBF84.unk8[D_801FBF84.unk0];
    c = D_8006794C->unk1C->unk54C[D_801FBF84.unk2];
    if (l->unk2 != 0 && (idx = (l->unk5 + l->unk6) * 2 + l->unk7) < D_801FBF84.unk76A) {
        id = D_801FBF84.unk728[idx];
        sup = &D_8006D128[id - 0xC0];
        st = func_801F0CC4_inline(id, c);
        if (st >= 3) {
            func_8003493C(c->unk34.b.id, id, st == 3);
            ap = c->unk17;
            if (st == 3) {
                c->unk17 = ap - sup->unk1;
            } else {
                c->unk17 = ap + sup->unk1;
            }
            func_8003384C(c);
            func_8002F488(0x6B);
            return;
        }
    }
    func_8002F488(0x66);
}

INCLUDE_ASM("asm/us/ovl_33/nonmatchings/ovl_33", func_801F0024);

void func_801F0410(void) {
    s32 i;
    s32 sel;
    s32 row;
    Character *c;
    MenuList *l;

    sel = 0;
    i = 0;
    c = D_8006794C->unk1C->unk54C[D_801FBF84.unk2];
    l = &D_801FBF84.unk8[1];
    for (; i < D_801FBF84.unk76A; i++) {
        if (func_801F0CC4_inline(D_801FBF84.unk728[i], c) >= 3) {
            sel = i;
            break;
        }
    }
    row = sel / 2;
    l->unk7 = sel % 2;
    if (row - 7 >= 0) {
        l->unk5 = row - 7;
    } else {
        l->unk5 = 0;
    }
    l->unk6 = row - l->unk5;
}

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

void func_801F070C(void) {
    MenuList *l;

    l = &D_801FBF84.unk8[D_801FBF84.unk0];
    if (l->unk8 == 0) {
        if (D_80077528 & 0x10000) {
            func_8002F488(0x65);
            D_801FBF84.unk1 = 1;
            return;
        }
        if (D_80077528 & 0x20000) {
            if (D_801FBF84.unk0 == 0) {
                func_801EFCB4();
            } else {
                func_801EFE2C();
            }
            return;
        }
        func_8002EE8C(l, D_80077528);
        func_8002F044(l, D_80077528);
        func_8002F0EC(l, D_80077528);
    }
}

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

void func_801F0934(s32 col, s32 row, s32 z, MenuView *v) {
    Character *c = D_8006794C->unk1C->unk54C[D_801FBF84.unk2];
    s32 x, y;
    s32 dx = 8;
    s32 dy = 0x5E;

    x = col * 0x96 + (D_801FBF84.unk48.x + dx);
    y = row * 0xF + (v->unk2 + dy);
    if (D_801FBF84.unk0 == 0) {
        func_801F09CC(x, y, z, c);
    } else {
        func_801F0AB8(x, y, z, c);
    }
}

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

void func_801F0AB8(s32 x, s32 y, s32 i, Character *c) {
    u8 buf[0x10];
    Unk8006D128 *sup;
    s32 id;
    u32 st;
    s32 col;

    id = D_801FBF84.unk728[i];
    sup = &D_8006D128[id - 0xC0];
    st = func_801F0CC4(id, c);
    if (st != 0) {
        col = D_801F740C[st];
        if (st == 4) {
            func_801F0F38(x + 8, y, 0x1C, D_801FBF84.unk76C);
        }
        if (!((D_8006794C->unk24 >> 18) & 1)) {
            func_801F0F38(x + 8, y, 0x1C, st >= 3 ? 9 : 10);
        } else {
            func_801F0F38(x + 8, y, 0x1C, st >= 3 ? 7 : 8);
        }
        func_8002B5FC(x + 0x18, y, 0x1C, D_801F6540 + sup->unk2, col);
        func_8002BAE4(sup->unk1, buf, 3, 1);
        func_8002B56C(x + 0x6C, y + 4, 0x1C, buf, col);
    }
}

u8 func_801F0C10(s32 id, Character *c) {
    s32 i;
    s32 j;
    u8 k;
    Unk801F6FEC *e;

    for (i = 0; i < 2; i++) {
        k = D_801F33D0[c->unk34.b.row][i];
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
        slot = func_80034430(c->unk34.b.id, id);
        if (slot < 0) {
            return 0;
        }
        if (func_80034A30(c)) {
            ap = c->unk58[slot];
            need = D_8006CB28[c->unk34.b.row][slot].unk1;
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

void func_801F0DDC(void) {
    MenuList *l = &D_801FBF84.unk8[D_801FBF84.unk0];

    func_80063118(&D_801FBF84.unk58, l->unk7 == 0 ? D_801F3458 : D_801F3468, 0);
    D_801FBF84.unk1 = 4;
}

void func_801F0E3C(void) {
    D_801FBF84.unk1 = 3;
}

void func_801F0E4C(void) {
    D_801FBF84.unk1 = 2;
    D_801FBF84.unk768 = 0;
}

s32 func_801F0E64(s32 id) {
    s32 kind = D_801F34EC[id].unk0;
    s32 n = 0;
    s32 i;

    for (i = 0; i < 4; i++) {
        if (D_8006794C->unk1C->unk54C[i] != NULL) {
            n++;
        }
    }
    if (n < 2) {
        return 0;
    }
    return (u32)(kind - 3) < 3;
}

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

s32 func_801F0FA4(MenuList *l) {
    s32 r, t;
    s32 step;

    if (l->unk8 & 8) {
        t = l->unk7 ^ 1;
        step = 1;
        r = l->unk6;
        if (l->unk8 & 2) {
            return (l->unk5 + (r - step)) * 2 + t;
        }
        return (l->unk5 + (r + step)) * 2 + t;
    }
    return (l->unk5 + l->unk6) * 2 + l->unk7;
}

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

void func_801F12C8(void) {
    u8 slot;
    s32 next;

    if (D_80077528 & 0x10000) {
        func_8002F488(0x65);
        func_800361CC(0);
    } else if (D_80077528 & 0x80000) {
        func_8002F488(0x67);
        func_80036270(2);
    } else if (D_80077528 & 0x80) {
        func_8002F488(0x67);
        D_801FC840.unk0 = D_801FC840.unk0 != 0 ? D_801FC840.unk0 - 1 : 2;
    } else if (D_80077528 & 0x20) {
        func_8002F488(0x67);
        D_801FC840.unk0 = D_801FC840.unk0 >= 2 ? 0 : D_801FC840.unk0 + 1;
    } else if (D_80077528 & 0x200000) {
        slot = D_801FC840.unk3;
        next = func_8003380C(slot);
        if (slot != next) {
            func_801F3278(next, 2);
            func_8002F488(0x417);
        }
    } else if (D_80077528 & 0x100000) {
        slot = D_801FC840.unk3;
        next = func_800337D0(slot);
        if (slot != next) {
            func_801F3278(next, 1);
            func_8002F488(0x417);
        }
    } else if (D_80077528 & 0x20000) {
        if (D_801FC840.unk0 == 1) {
            func_801F24EC();
        } else {
            func_8002F488(0x67);
            D_801FC840.unk1 = 2;
        }
    }
}

void func_801F1454(void) {
    u8 slot;
    s32 next;

    if (D_80077528 & 0x10000) {
        func_8002F488(0x65);
        D_801FC840.unk1 = 1;
    } else if (D_80077528 & 0x10) {
        func_8002F488(0x67);
        D_801FC840.unk2 = D_801FC840.unk2 == 0 ? 4 : D_801FC840.unk2 - 1;
    } else if (D_80077528 & 0x40) {
        func_8002F488(0x67);
        D_801FC840.unk2 = D_801FC840.unk2 >= 4 ? 0 : D_801FC840.unk2 + 1;
    } else if (D_80077528 & 0x200000) {
        slot = D_801FC840.unk3;
        next = func_8003380C(slot);
        if (slot != next) {
            func_8002F488(0x417);
            func_801F3278(next, 2);
        }
    } else if (D_80077528 & 0x100000) {
        slot = D_801FC840.unk3;
        next = func_800337D0(slot);
        if (slot != next) {
            func_8002F488(0x417);
            func_801F3278(next, 1);
        }
    } else if (D_80077528 & 0x20000) {
        if (D_801FC840.unk0 == 0) {
            func_8002F488(0x67);
            func_801F2894();
            func_801F301C();
        } else {
            func_801F31B0();
        }
    }
}

INCLUDE_ASM("asm/us/ovl_33/nonmatchings/ovl_33", func_801F15E0);

void func_801F189C(void) {
    Character *c;
    RECT *r;
    s32 i;
    s32 grey;
    u8 sel;
    s32 y8;
    s32 y12;
    s32 y10;
    Unk80069018 *it;

    r = &D_801FC840.unk24;
    c = D_8006794C->unk1C->unk54C[D_801FC840.unk3];
    if (D_801FC840.unk1 == 3) {
        return;
    }
    func_8002B524(r->x + 6, r->y - 3, 0x1E, D_8007043C, 0);
    func_8002DEE4(r, 0x1E, 2);
    func_8002705C(r, 0x1E, 2);
    for (i = 0, grey = 2, y8 = 8, y12 = 12, y10 = 10; i < 5; i++) {
        sel = 0;
        if (D_801FC840.unk0 == grey) {
            sel = (i == 0);
        }
        func_800275DC(r->x + 10, r->y + y10, 0x1D, i + 0xF9, sel);
        func_8002B56C(r->x + 0x19, r->y + y12, 0x1D, D_801EE644, sel ? grey : 0);
        if (c->unk39[i] != 0xFF) {
            it = &D_80069018[c->unk39[i]];
            func_80026C34(r->x + 0x21, r->y + y8, 0x1D, c->unk39[i], !sel);
            func_8002B5FC(r->x + 0x31, r->y + y8, 0x1D, D_8006A218 + it->unk0, sel ? grey : 0);
        }
        y8 += 17;
        y12 += 17;
        y10 += 17;
    }
}

INCLUDE_ASM("asm/us/ovl_33/nonmatchings/ovl_33", func_801F1A94);

void func_801F1F08(void) {
    RECT sub;
    MenuView view;
    WinDef *wd = &D_801FC840.unk23C;
    RECT *r = &wd->unk4;
    AbilityMenu *m = &D_801FC840;
    MenuList *l = &D_801FC840.unk4;
    s32 w;
    s32 k = 0x97;
    s32 i;

    if (m->unk260 != 0) {
        w = func_8002AE7C((u8 *)D_800703FC.unk94);
        func_8002B524(wd->unk4.x + 6, r->y - 3, 0x14, D_800703FC.unk0[D_801F8918[m->unk2]], 0);
        func_8002B524(wd->unk4.x - (w - k), r->y - 3, 0x14, D_800703FC.unk94, 0);
        func_8002E040(r, 0x14, 1, l);
        func_8002ECA0(l, &view);
        func_8002D9B4(NULL, 0x13, 1);
        for (i = 0; i < view.unk0; i++) {
            func_801F2F3C(0, i, view.unk1 + i, &view);
        }
        if (l->unk8 & 4) {
            func_8002F40C(l, r, &sub);
            func_8002D9B4(&sub, 0x13, 1);
            func_801F2ED4();
        }
        if (wd->unkC == 0) {
            func_8002D9B4(r, 0x13, 1);
        }
    }
}

INCLUDE_ASM("asm/us/ovl_33/nonmatchings/ovl_33", func_801F20A0);

void func_801F239C(void) {
    AbilityMenu *m = &D_801FC840;
    u16 x;
    u16 k;
    Unk80077DE8 *h;

    func_80035674(m->unk1 != 1);
    if (func_8002F810() || m->unk1 == 0) {
        return;
    }
    if (m->unk1 == 1 || (D_80077E54 & 3)) {
        h = &D_80077DE8;
        x = D_801F893C[D_801FC840.unk0][0];
        h->unk2 = m->unk14.y + 8;
        h->unk4 = 0x1B;
        h->unk0 = m->unk14.x + x - 0x18;
        func_800355E4();
    }
    if (D_801FC840.unk1 == 2) {
        D_80077DE8.unk4 = 0x12;
        D_80077DE8.unk0 = D_801FC840.unk24.x - 0x10;
        k = D_801FC840.unk2 * 17 + 0xC;
        D_80077DE8.unk2 = D_801FC840.unk24.y + k;
        func_800355E4();
    } else if (D_801FC840.unk1 == 3) {
        D_80077DE8.unk4 = 0x12;
        D_80077DE8.unk0 = D_801FC840.unk23C.unk4.x - 0xD;
        k = D_801FC840.unk4.unk6 * 15 + 0xA;
        D_80077DE8.unk2 = D_801FC840.unk23C.unk4.y + k;
        func_800355E4();
    }
}

void func_801F24EC(void) {
    s32 best;
    s32 i;
    s32 j;
    s32 max;
    s32 kind;
    s32 cur;
    Character *c;
    UnkPair *p;
    Unk80069018 *it;

    best = 0;
    i = 0;
    c = D_8006794C->unk1C->unk54C[D_801FC840.unk3];
    kind = func_80033A1C(c);
    for (; i < 5; i++) {
        if (c->unk39[i] != 0xFF) {
            cur = D_80069018[c->unk39[i]].unkA;
        } else {
            cur = -1;
        }
        for (j = 0, max = -1, p = D_8006794C->unk1C->unk574.unk0; j < 0x100; j++, p++) {
            if (p->unk1 != 0) {
                it = &D_80069018[p->unk0];
                if ((it->unkF & D_801F8948[i]) && (it->unk6 & D_801F8950[kind])) {
                    if (it->unkA > max) {
                        max = it->unkA;
                        best = p->unk0;
                    }
                }
            }
        }
        if (cur < max) {
            if (c->unk39[i] != 0xFF) {
                func_80027D84(c->unk39[i], 1);
            }
            if (func_80027E5C(best, 1) != 0) {
                c->unk39[i] = best;
            }
        }
    }
    func_801F32E0(c);
    func_8002F488(0x6B);
}

void func_801F26C4(Character *c) {
    s32 i;
    s32 j;
    AbilityEntry *a;
    Unk80069018 *it;
    s32 id;

    if (func_80034A30(c)) {
        func_8001D898(D_801FC700, 0, 0x40);
        for (i = 0; i < 5; i++) {
            if (c->unk39[i] != 0xFF) {
                it = &D_80069018[c->unk39[i]];
                for (j = 0; j < 3; j++) {
                    id = it->unkC[j];
                    if (id != 0) {
                        if (id >= 0xC0) {
                            D_801FC700[id - 0xC0] = 1;
                        }
                    }
                }
            }
        }
        a = D_8006CB28[c->unk34.b.row];
        for (i = 0; i < 0x30; i++, a++) {
            if (a->unk0 >= 0xC0 && func_800349BC(c->unk34.b.id, a->unk0)) {
                if (D_801FC700[a->unk0 - 0xC0] == 0 && c->unk58[i] < a->unk1) {
                    func_8003493C(c->unk34.b.id, a->unk0, 0);
                    if (c->unk1F - c->unk17 >= D_8006D128[a->unk0 - 0xC0].unk1) {
                        c->unk17 += D_8006D128[a->unk0 - 0xC0].unk1;
                    } else {
                        c->unk17 = c->unk1F;
                    }
                }
            }
        }
    }
}

void func_801F2894(void) {
    UnkPair tmp;
    Unk80069018 *it;
    s32 n;
    s32 i;
    s32 j;
    u16 mask;
    u8 slot;
    u8 a;
    u8 b;
    u8 ra;
    u8 rb;

    n = 0;
    mask = D_801F8968[func_80033A1C(D_8006794C->unk1C->unk54C[D_801FC840.unk3])];
    slot = D_801F8980[D_801FC840.unk2];
    D_801FC840.unk3C[0].unk0 = 0xFF;
    for (i = 0; i < 0x100; i++) {

        if (D_8006794C->unk1C->unk574.unk0[i].unk1 != 0) {
            it = &D_80069018[D_8006794C->unk1C->unk574.unk0[i].unk0];
            if ((it->unk6 & mask) && (it->unkF & slot)) {
                D_801FC840.unk3C[n] = D_8006794C->unk1C->unk574.unk0[i];
                n++;
            }
        }
    }
    for (i = 0; i < n - 1; i++) {
        for (j = i + 1; j < n; j++) {
            a = D_801FC840.unk3C[i].unk0;
            b = D_801FC840.unk3C[j].unk0;
            ra = D_80069018[a].unkA;
            rb = D_80069018[b].unkA;
            if ((ra == rb && b < a) || rb < ra) {
                tmp = D_801FC840.unk3C[i];
                D_801FC840.unk3C[i] = D_801FC840.unk3C[j];
                D_801FC840.unk3C[j] = tmp;
            }
        }
    }
    func_8002EDFC(&D_801FC840.unk4, n, 6, 1, 0x62);
}

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

void func_801F2F3C(s32 col, s32 row, s32 i, MenuView *v) {
    WinDef *w;
    UnkPair *p;
    s32 x;
    s32 y;
    u8 buf[16];

    p = D_801FC840.unk3C + i;
    x = D_801FC840.unk23C.unk4.x;
    w = &D_801FC840.unk23C;
    y = w->unk4.y + row * 0xF + v->unk2;
    func_80026C34(x + 0xB, y, 0x13, p->unk0, 1);
    func_8002B5FC(x + 0x1B, y, 0x13, D_8006A218 + D_80069018[p->unk0].unk0, 0);
    func_8002BAE4(p->unk1, buf, 2, 1);
    func_8002B56C(x + 0x8A, y + 4, 0x13, buf, 0);
}

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
    func_801F26C4(c);
    func_8003384C(c);
    c->unk34.b.unk4 = func_80033280(c->unk34.b.id, (c->unkA >> 4) & 1, c->unk39);
}

void func_801F3344(s32 x, s32 y, s32 z, u8 id, s32 color) {
    func_8002B5FC(x, y - 4, z, func_80035774(id), color);
}

INCLUDE_RODATA("asm/us/ovl_33/nonmatchings/ovl_33", D_801EDF00);

INCLUDE_RODATA("asm/us/ovl_33/nonmatchings/ovl_33", D_801EDF04);

INCLUDE_RODATA("asm/us/ovl_33/nonmatchings/ovl_33", D_801EE164);

INCLUDE_RODATA("asm/us/ovl_33/nonmatchings/ovl_33", D_801EE644);

INCLUDE_RODATA("asm/us/ovl_33/nonmatchings/ovl_33", D_801EE648);
