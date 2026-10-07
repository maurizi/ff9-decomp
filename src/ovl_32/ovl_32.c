#include "ovl_32.h"

INCLUDE_RODATA("asm/us/ovl_32/nonmatchings/ovl_32", D_801EDF00);

INCLUDE_RODATA("asm/us/ovl_32/nonmatchings/ovl_32", D_801EDF04);

/* text bank for this menu (count + {offset, info} table, then strings) */
INCLUDE_RODATA("asm/us/ovl_32/nonmatchings/ovl_32", D_801EDF84);

void func_801F7AC0(void) {
    Ovl32State *st;

    if (D_8006794C->unk1C->unk844 > 24) {
        D_8006794C->unk1C->unk844 = 0;
    }
    st = &D_801F9850;
    st->unk1 = 1;
    D_801F9850.unk0 = 0;
    func_8003575C(D_801EDF84);
    func_8003556C(func_801F84F0);
    func_80035674(1);
    setRECT(&D_801F9850.unk20, 0xE4, 8, 0x50, 0x1A);
    setRECT(&D_801F9850.unk28, 8, 8, 0xD8, 0x1A);
    setRECT(&D_801F9850.unk30, 8, 0x26, 0xB7, 0x43);
    setRECT(&D_801F9850.unk48, 8, 0x6B, 0x86, 0x71);
    setRECT(&D_801F9850.unk40, 0xBF, 0x47, 0x75, 0x22);
    setRECT(&D_801F9850.unk38, 0xBF, 0x26, 0x75, 0x21);
    setRECT(&D_801F9850.unk50, 0x8E, 0x6B, 0xA6, 0x71);
    func_8002EDFC(&D_801F9850.unk10, 0x18, 7, 1, 0x71);
    func_801F85EC();
    func_801F86D0();
    if (D_8006794C->unk1C->unk844 != 0) {
        func_801F9600(D_8006794C->unk1C->unk844 - 1);
    }
    D_801F9850.unk2058 = D_801F9850.unk58 + (D_8006794C->unk1C->unk8 << 12);
}

void func_801F7C8C(void) {
    u8 v;
    Ovl32State *st;

    if (D_80077528 & 0x10000) {
        func_8002F488(0x65);
        func_800325E0();
    } else if (D_80077528 & 0x20000) {
        if (D_801F9850.unk0 == 0) {
            D_801F9850.unk1 = 2;
            func_8002F488(0x67);
        } else if (D_8006794C->unk1C->unk844 == 0) {
            func_8002F488(0x66);
        } else {
            D_8006794C->unk1C->unk844 = 0;
            func_8002F488(0x6B);
        }
    } else {
        if (D_80077528 & 0x80) {
            func_8002F488(0x67);
            st = &D_801F9850;
            if (st->unk0 != 0) {
                v = st->unk0 - 1;
            } else {
                v = 1;
            }
        } else if (D_80077528 & 0x20) {
            func_8002F488(0x67);
            st = &D_801F9850;
            if (st->unk0 == 0) {
                v = st->unk0 + 1;
            } else {
                v = 0;
            }
        } else {
            return;
        }
        st->unk0 = v;
    }
}

INCLUDE_ASM("asm/us/ovl_32/nonmatchings/ovl_32", func_801F7DC0);

void func_801F7EB4(void) {
    RECT *r = &D_801F9850.unk30;
    s32 sel;

    func_8002DEE4(r, 0x1D, 2);
    func_8002705C(r, 0x1E, 2);
    func_8002B524(r->x + 8, r->y - 3, 0x1C, D_800704AC, 0);
    sel = D_8006794C->unk1C->unk844;
    if (sel != 0) {
        func_801F9518(r->x + 0x10, r->y + 0x2C, 0x1C, (D_801F9850.unkC & (1 << (sel - 1))) ? 0xD : 0xC, 0);
        func_8002B5FC(r->x + 0x21, r->y + 0x2E, 0x1C, func_80035774(D_8006794C->unk1C->unk844 + 9), 0);
        func_801F88F4(r->x + 0x4C, r->y + 0xB, 0x1C, D_8006794C->unk1C->unk844 - 1, 0);
        func_801F8AB0(r->x + 0x48, r->y + 7, 0x1C, 0);
    }
}

void func_801F8010(void) {
    RECT sub;
    MenuView view;
    s32 i;
    RECT *r = &D_801F9850.unk48;
    MenuList *list;

    func_8002B524(r->x + 0x10, r->y - 3, 0x1C, D_80070428, 0);
    list = &D_801F9850.unk10;
    func_8002E040(r, 0x1D, 1, list);
    func_8002705C(r, 0x1D, 1);
    func_8002ECA0(list, &view);
    func_8002D9B4(NULL, 0x1C, 1);
    for (i = 0; i < view.unk0; i++) {
        func_801F93FC(0, i, view.unk1 + i, &view);
    }
    if (list->unk8 & 4) {
        func_8002F40C(list, r, &sub);
        func_8002D9B4(&sub, 0x1C, 1);
        func_801F9394();
    }
    func_8002D9B4(r, 0x1C, 1);
}

void func_801F8130(void) {
    u8 buf[8];
    RECT *r = &D_801F9850.unk40;
    Ovl32State *st;

    func_8002DEE4(r, 0x1D, 2);
    func_8002705C(r, 0x1E, 2);
    func_8002B5FC(r->x + 9, r->y + 3, 0x1C, func_80035774(7), 0);
    func_8002B5FC(r->x + 9, r->y + 0xF, 0x1C, func_80035774(8), 0);
    st = &D_801F9850;
    func_8002BAE4(st->unk2, buf, 2, 1);
    func_8002B56C(r->x + 0x5E, r->y + 7, 0x1C, buf, 0);
    func_8002BAE4(st->unk3, buf, 2, 1);
    func_8002B56C(r->x + 0x5E, r->y + 0x13, 0x1C, buf, 0);
}

INCLUDE_ASM("asm/us/ovl_32/nonmatchings/ovl_32", func_801F8240);

INCLUDE_ASM("asm/us/ovl_32/nonmatchings/ovl_32", func_801F8404);

void func_801F84F0(void) {
    s32 idx;

    D_80077DE8.unk8 = NULL;
    func_80035580(0);
    if (D_801F9850.unk1 == 1) {
        D_80077DE8.unk8 = func_80035774(D_801F9850.unk0 + 3);
    } else if (D_801F9850.unk1 == 2) {
        idx = D_801F9850.unk10.unk5 + D_801F9850.unk10.unk6;
        if (D_801F9850.unk8 & (1 << idx)) {
            func_8002BD54(D_801F9750, func_80035774(idx + 0x22));
            if (D_801F9850.unk4 <= func_801F95E8_inline(idx)) {
                func_8002AD38(D_801F9750, func_80035774(5));
            }
            D_80077DE8.unk8 = D_801F9750;
        }
    }
}

INCLUDE_ASM("asm/us/ovl_32/nonmatchings/ovl_32", func_801F85EC);

void func_801F86D0(void) {
    RECT r0;
    RECT r1;
    Ovl32Image *src = &D_801EF1E4[1];
    Ovl32Image *dst = &D_801EF1E4[0];
    s32 i;

    func_801F9660();
    LoadImage(&D_801F970C, (u32 *)D_801F713C);
    DrawSync(0);
    LoadImage(&D_801F9714, (u32 *)(D_801F713C + 0x1C0));
    DrawSync(0);
    LoadImage(&D_801F971C, (u32 *)D_801F7948);
    DrawSync(0);
    LoadImage(&D_801F9724, (u32 *)(D_801F7948 + 0x40));
    DrawSync(0);
    for (i = 0; i < 24; i++) {
        func_801F9668(i, &r0, &r1);
        StoreImage(&r0, (u32 *)dst->unk0);
        DrawSync(0);
        StoreImage(&r1, (u32 *)dst->unk80);
        DrawSync(0);
        LoadImage(&r0, (u32 *)src->unk0);
        DrawSync(0);
        LoadImage(&r1, (u32 *)src->unk80);
        DrawSync(0);
        dst++;
        src++;
    }
}

void func_801F8800(void) {
    RECT r0;
    RECT r1;
    s32 i;
    Ovl32Image *img = D_801EF1E4;
    s32 sel;

    if (D_8006794C->unk8 == 3) {
        sel = D_8006794C->unk1C->unk844;
        if (sel != 0) {
            func_801F9668(sel - 1, &r0, &r1);
            MoveImage(&r0, 0x280, 0xF6);
            DrawSync(0);
            MoveImage(&r1, 0x318, 0xB0);
            DrawSync(0);
        }
    }
    for (i = 0; i < 24; i++, img++) {
        func_801F9668(i, &r0, &r1);
        LoadImage(&r0, (u32 *)img->unk0);
        DrawSync(0);
        LoadImage(&r1, (u32 *)img->unk80);
        DrawSync(0);
    }
}

INCLUDE_ASM("asm/us/ovl_32/nonmatchings/ovl_32", func_801F88F4);

INCLUDE_ASM("asm/us/ovl_32/nonmatchings/ovl_32", func_801F8AB0);

void func_801F8FF8(void) {
    D_801F9850.unk2058 = D_801F9850.unk58 + (D_8006794C->unk1C->unk8 << 12);
    func_801F907C();
    func_801F90D4();
    func_801F912C();
}

void func_801F904C(void) {
    func_80035674(1);
    func_8003556C(0);
    func_801F8800();
}

void func_801F907C(void) {
    void (*f)(void);

    if (!func_8002F810()) {
        f = D_801F96E4[D_801F9850.unk1];
        if (f != NULL) {
            f();
        }
    }
}

void func_801F90D4(void) {
    func_801F9164();
    func_801F91D8();
    func_801F7EB4();
    func_801F8010();
    func_801F8130();
    func_801F92C0();
    func_801F8240();
    func_801F8404();
}

void func_801F912C(void) {
    if (D_801F9850.unk1 == 2) {
        func_8002EB74(&D_801F9850.unk10);
    }
}

void func_801F9164(void) {
    RECT *r = &D_801F9850.unk20;

    func_8002DEE4(r, 0x1D, 2);
    func_8002705C(r, 0x1D, 2);
    func_8002B5FC(r->x + 7, r->y + 6, 0x1C, func_80035774(0), 0);
}

void func_801F91D8(void) {
    RECT *r = &D_801F9850.unk28;
    s32 i;
    s32 x;
    u8 none = D_8006794C->unk1C->unk844 == 0;

    func_8002DEE4(r, 0x1D, 2);
    func_8002705C(r, 0x1D, 2);
    for (i = 0, x = 0x2C; i < 2; i++, x += 0x60) {
        func_8002B5FC(r->x + x, r->y + 6, 0x1C, func_80035774(i + 1), (D_801F96F0[i] != 0 && none) ? 2 : 0);
    }
}

INCLUDE_ASM("asm/us/ovl_32/nonmatchings/ovl_32", func_801F92C0);

void func_801F9394(void) {
    MenuView view;
    s32 i;

    func_8002F344(&D_801F9850.unk10, &view);
    for (i = 0; i < view.unk0; i++) {
        func_801F93FC(0, i, view.unk1 + i, &view);
    }
}

INCLUDE_ASM("asm/us/ovl_32/nonmatchings/ovl_32", func_801F93FC);

void func_801F9518(s32 x, s32 y, s32 z, s32 icon, u8 arg4) {
    RectTbl16 rects = D_801EDF04;

    func_80027710(x, y, z, &rects.r[icon], arg4);
}

s32 func_801F95E8(s32 id) {
    return D_801F96F4[id];
}

void func_801F9600(s32 idx) {
    MenuList *l = &D_801F9850.unk10;

    if (idx > 0x10) {
        l->unk5 = 0x11;
        l->unk6 = idx - 0x11;
    } else {
        l->unk6 = idx % 7;
        l->unk5 = idx - l->unk6;
    }
}

void func_801F9660(void) {
}

INCLUDE_ASM("asm/us/ovl_32/nonmatchings/ovl_32", func_801F9668);

INCLUDE_ASM("asm/us/ovl_32/nonmatchings/ovl_32", func_801F96E8);

INCLUDE_RODATA("asm/us/ovl_32/nonmatchings/ovl_32", D_801EF1E4);

INCLUDE_RODATA("asm/us/ovl_32/nonmatchings/ovl_32", D_801EF6FC);

INCLUDE_RODATA("asm/us/ovl_32/nonmatchings/ovl_32", D_801F713C);

INCLUDE_RODATA("asm/us/ovl_32/nonmatchings/ovl_32", D_801F7948);
