#include "ovl_0a.h"

void func_800A7A48(void) {
    func_800A8198();
    do {
        func_800A7AAC((s16)D_8006794C->unk1C->unk3A);
    } while (!(D_8006794C->unk0 & 3));
    func_800A82F8();
}

void func_800A7AAC(s32 arg0) {
    func_800A85F4(arg0);
    do {
        func_800A7B10(D_8006794C->unk1C->unkC);
    } while (!(D_8006794C->unk0 & 7));
    func_800A8698();
}

INCLUDE_RODATA("asm/us/ovl_0a/nonmatchings/ovl_0a", D_800A7000);

INCLUDE_ASM("asm/us/ovl_0a/nonmatchings/ovl_0a", func_800A7B10);

INCLUDE_ASM("asm/us/ovl_0a/nonmatchings/ovl_0a", func_800A80A4);

INCLUDE_ASM("asm/us/ovl_0a/nonmatchings/ovl_0a", func_800A8198);

void func_800A82F8(void) {
    s32 i;

    for (i = 1; i >= 0; i--) {
        D_800739B0[i].unk1 = 0;
        D_800739B0[i].unk0 = 0;
    }
    func_800B1CE4();
    func_800A8588();
}

void func_800A8344(void) {
    D_80067948 = (s32)(D_80067944 + 0x28000);
    D_80067948 += 0x2000;
    D_8006794C->unk1C->unk8E4.field = (FieldMap *)D_80067948;
    D_80067948 += 0x50;
}

void func_800A8380(void) {
    s32 i = 0x2B;
    FieldMap *fm = D_8006794C->unk1C->unk8E4.field;

    fm->unkA = -1;
    fm->unk0 = 0;
    fm->unk4 = 0;
    fm->unk8 = 0;
    fm->unk9 = 0;
    fm->unk14 = -1;
    fm->unk18 = -1;
    fm->unk20 = -1;
    fm->unk10 = NULL;
    fm->unkC = NULL;
    do {
        fm->unk24[i] = 0;
        i--;
    } while (i >= 0);
}

INCLUDE_ASM("asm/us/ovl_0a/nonmatchings/ovl_0a", func_800A83E0);

INCLUDE_ASM("asm/us/ovl_0a/nonmatchings/ovl_0a", func_800A847C);

s32 func_800A8588(void) {
    void *p = D_8006794C->unk1C->unk8E4.field->unk10;

    if (p != NULL) {
        func_8001C784(p);
        D_8006794C->unk1C->unk8E4.field->unk10 = NULL;
    }
    return 0;
}

INCLUDE_ASM("asm/us/ovl_0a/nonmatchings/ovl_0a", func_800A85F4);

void func_800A8698(void) {
    GameData *gd = D_8006794C->unk1C;
    FieldMapObj *obj = gd->unk8E4.field->unkC;

    func_800A8990(obj, gd->unk3A);
    D_8006794C->unk1C->unk3A = obj->unk10;
}

void func_800A86FC(void) {
    FieldMapObj *p = (FieldMapObj *)D_80067948;

    D_80067948 = (s32)(p + 1);
    D_8006794C->unk1C->unk8E4.field->unkC = p;
}

void func_800A8724(void) {
    FieldMapObj *obj = D_8006794C->unk1C->unk8E4.field->unkC;
    s32 i;

    obj->unk0 = 0;
    obj->unk4 = 0;
    obj->unk8 = NULL;
    obj->unkC = NULL;
    obj->unk10 = D_8006794C->unk1C->unk3A;
    obj->unk14 = NULL;
    for (i = 1; i >= 0; i--) {
        obj->unk12[i] = 0;
    }
    for (i = 0x1F; i >= 0; i--) {
        obj->unk18[i] = 0;
    }
}

s32 func_800A879C(FieldMapObj *arg0, u16 arg1) {
    arg0->unk8 = func_8001C7FC(4, arg1 | 0x60000, 0);
    return 0;
}

INCLUDE_ASM("asm/us/ovl_0a/nonmatchings/ovl_0a", func_800A87DC);

s32 func_800A8990(FieldMapObj *arg0, s16 arg1) {
    func_8001C784(arg0->unkC);
    arg0->unkC = NULL;
    return 0;
}

INCLUDE_ASM("asm/us/ovl_0a/nonmatchings/ovl_0a", func_800A89C4);

INCLUDE_ASM("asm/us/ovl_0a/nonmatchings/ovl_0a", func_800A8E58);

INCLUDE_ASM("asm/us/ovl_0a/nonmatchings/ovl_0a", func_800A9350);

INCLUDE_ASM("asm/us/ovl_0a/nonmatchings/ovl_0a", func_800A939C);

INCLUDE_ASM("asm/us/ovl_0a/nonmatchings/ovl_0a", func_800A9570);

s32 func_800A97D4(FieldScene *arg0, u16 arg1) {
    arg0->unkC = func_8001C7FC(4, arg1 | 0x50000, 0);
    return 0;
}

s32 func_800A9814(FieldScene *arg0, u16 arg1) {
    void *p = func_8001C7FC(4, arg1 | 0x1E0000, 0);

    arg0->unk4C = p;
    if (p != NULL) {
        func_80054A28(p);
    }
    return 0;
}

INCLUDE_ASM("asm/us/ovl_0a/nonmatchings/ovl_0a", func_800A9860);

s32 func_800A9954(FieldScene *arg0) {
    arg0->unk1C = func_8001C7FC(4, (u16)func_800AA0C8(arg0) | 0x1F0000, 0);
    return 0;
}

INCLUDE_ASM("asm/us/ovl_0a/nonmatchings/ovl_0a", func_800A9998);

INCLUDE_ASM("asm/us/ovl_0a/nonmatchings/ovl_0a", func_800A9AAC);

INCLUDE_ASM("asm/us/ovl_0a/nonmatchings/ovl_0a", func_800A9BDC);

s32 func_800AA01C(u16 *arg0, u16 *arg1) {
    Unk800AA01C *p = func_8001C7FC(4, 0x1C0000, 0);

    *arg0 = p->unk30;
    *arg1 = p->unk32;
    return 0;
}

s32 func_800AA074(FieldScene *arg0) {
    func_8001C784(arg0->unk24);
    arg0->unk24 = NULL;
    return 0;
}

s32 func_800AA0A8(void) {
    func_80054F80();
    return 0;
}

s32 func_800AA0C8(FieldScene *arg0) {
    return func_8001C8B0((s32)arg0->unk24, 3, 0xA0000, 0);
}

INCLUDE_ASM("asm/us/ovl_0a/nonmatchings/ovl_0a", func_800AA0F4);

INCLUDE_ASM("asm/us/ovl_0a/nonmatchings/ovl_0a", func_800AA194);

INCLUDE_ASM("asm/us/ovl_0a/nonmatchings/ovl_0a", func_800AA290);

INCLUDE_ASM("asm/us/ovl_0a/nonmatchings/ovl_0a", func_800AA330);

INCLUDE_ASM("asm/us/ovl_0a/nonmatchings/ovl_0a", func_800AA3F4);

void func_800AA4BC(void) {
    s32 i;

    for (i = 15; i >= 0; i--) {
        D_800C9F20[i] = -1;
    }
}

INCLUDE_ASM("asm/us/ovl_0a/nonmatchings/ovl_0a", func_800AA4E8);

INCLUDE_ASM("asm/us/ovl_0a/nonmatchings/ovl_0a", func_800AAA54);

INCLUDE_ASM("asm/us/ovl_0a/nonmatchings/ovl_0a", func_800AAB30);

s32 func_800AAEC4(s32 arg0) {
    if ((arg0 & 0xFFFF) == 0xB9 && D_8007AEF0.h[0] >= 0x283C) {
        arg0 = 0x2004C;
    }
    return arg0;
}

s32 func_800AAEF8(s32 arg0) {
    FileTable *t = D_8006794C->unk1C->unk7E4;
    s32 i;
    FileEntry *e = t->unk4;

    for (i = t->unk2 - 1; i >= 0; i--) {
        if (e[i].unkC == arg0) {
            return e[i].unk0;
        }
    }
    return -1;
}

s32 func_800AAF5C(s32 arg0) {
    Unk800AA39C *t = func_8001C7FC(8, arg0, 0x120000);
    s32 i;
    Unk800AA39CEntry *e;

    e = t->unk4;
    for (i = t->unk1 - 1; i >= 0; i--) {
        if (e->unk0 == (u16)arg0) {
            return e->unk2 | 0x30000;
        }
    }
    return -1;
}

INCLUDE_ASM("asm/us/ovl_0a/nonmatchings/ovl_0a", func_800AAFD4);

s32 func_800AB0B0(FieldScene *arg0) {
    Unk800AB0B0 *t = arg0->unk50;
    s32 i;
    Unk800AB0B0Entry *e;

    if (t != NULL) {
        e = t->unk4;
        for (i = t->unk0 - 1; i >= 0; i--) {
            if (e[i].unk0 & 1) {
                return 1;
            }
        }
    }
    return 0;
}

INCLUDE_ASM("asm/us/ovl_0a/nonmatchings/ovl_0a", func_800AB110);

INCLUDE_ASM("asm/us/ovl_0a/nonmatchings/ovl_0a", func_800AB3CC);

INCLUDE_ASM("asm/us/ovl_0a/nonmatchings/ovl_0a", func_800ABA98);

INCLUDE_ASM("asm/us/ovl_0a/nonmatchings/ovl_0a", func_800AC02C);

INCLUDE_ASM("asm/us/ovl_0a/nonmatchings/ovl_0a", func_800AC344);

INCLUDE_ASM("asm/us/ovl_0a/nonmatchings/ovl_0a", func_800AC380);

INCLUDE_ASM("asm/us/ovl_0a/nonmatchings/ovl_0a", func_800AC3D8);

INCLUDE_ASM("asm/us/ovl_0a/nonmatchings/ovl_0a", func_800AC474);

INCLUDE_ASM("asm/us/ovl_0a/nonmatchings/ovl_0a", func_800AC5F0);

void func_800AC620(void) {
    s32 v = D_8006794C->unk1C->unk8E4.field->unkC->unk14->unk20;

    if (v == 0) {
        v = D_80067948;
    }
    func_800AC67C(v);
}

INCLUDE_ASM("asm/us/ovl_0a/nonmatchings/ovl_0a", func_800AC67C);

INCLUDE_ASM("asm/us/ovl_0a/nonmatchings/ovl_0a", func_800AC6C0);

void func_800AD44C(s32 arg0, s32 arg1, s32 arg2, s32 arg3) {
    func_80062E3C(arg0, 0x100, 0xFF, arg2, arg3, arg1, 3);
}

INCLUDE_ASM("asm/us/ovl_0a/nonmatchings/ovl_0a", func_800AD488);

INCLUDE_ASM("asm/us/ovl_0a/nonmatchings/ovl_0a", func_800AD944);

INCLUDE_ASM("asm/us/ovl_0a/nonmatchings/ovl_0a", func_800ADA7C);

INCLUDE_ASM("asm/us/ovl_0a/nonmatchings/ovl_0a", func_800ADB74);

INCLUDE_ASM("asm/us/ovl_0a/nonmatchings/ovl_0a", func_800AE4F8);

INCLUDE_ASM("asm/us/ovl_0a/nonmatchings/ovl_0a", func_800AE5D4);

s32 func_800AE68C(void) {
    if ((u32)(D_8006794C->unk1C->unk8E4.field->unkC->unk14->unk42 - 6) < 2 && (D_800C9D44 & 2)
        && (func_80012AD4() & 2)) {
        return 1;
    }
    return 0;
}

void func_800AE710(u16 arg0, s32 arg1) {
    func_800AD944(0, arg0, arg1);
}

void func_800AE738(void) {
    func_800AD944(2, 0, 0);
}

void func_800AE760(void) {
    func_800AD944(1, 0, 0);
}

INCLUDE_ASM("asm/us/ovl_0a/nonmatchings/ovl_0a", func_800AE788);

INCLUDE_ASM("asm/us/ovl_0a/nonmatchings/ovl_0a", func_800AE930);

void func_800AED04(void) {
    D_8006794C->unk1C->unk8E4.field->unkC->unk14->unk41 = 0;
    D_800C9EF0.unk1E = 4;
    D_800C9EF0.unk0 = 0x43;
    D_800C9EF0.unk4 = 0x3C;
    D_800C9EF0.unk14 = 0x1919;
    D_80073998[0] |= 0x04000000;
}

INCLUDE_ASM("asm/us/ovl_0a/nonmatchings/ovl_0a", func_800AED6C);

INCLUDE_ASM("asm/us/ovl_0a/nonmatchings/ovl_0a", func_800AF740);

INCLUDE_ASM("asm/us/ovl_0a/nonmatchings/ovl_0a", func_800AFAC0);

INCLUDE_ASM("asm/us/ovl_0a/nonmatchings/ovl_0a", func_800AFCBC);

INCLUDE_ASM("asm/us/ovl_0a/nonmatchings/ovl_0a", func_800AFDE8);

INCLUDE_ASM("asm/us/ovl_0a/nonmatchings/ovl_0a", func_800B00B0);

INCLUDE_ASM("asm/us/ovl_0a/nonmatchings/ovl_0a", func_800B0124);

INCLUDE_ASM("asm/us/ovl_0a/nonmatchings/ovl_0a", func_800B1A20);

void func_800B1C4C(void) {
    s32 i;

    for (i = 1; i >= 0; i--) {
        func_800B0124(((i & 1) << 6) | 0x1880, i, 0, 0, 0);
    }
}

void func_800B1C98(void) {
    s32 i;

    for (i = 1; i >= 0; i--) {
        func_800B0124(((i & 1) << 6) | 0x1980, i, 0, 0, 0);
    }
}

void func_800B1CE4(void) {
    s32 i;

    for (i = 1; i >= 0; i--) {
        if (D_8006794C->unk1C->unk830[i] != 0xFFFF) {
            func_800B0124(0x5180, D_8006794C->unk1C->unk830[i], 0, 0, 0);
        }
    }
}

void func_800B1D60(void) {
    s32 i;

    for (i = 4; i >= 0; i--) {
        D_800C9FF8[i] = -1;
        D_800C9FF0[i] = -1;
    }
}

INCLUDE_ASM("asm/us/ovl_0a/nonmatchings/ovl_0a", func_800B1D9C);

void func_800B1E04(FieldScene *arg0) {
    s32 t = (s32)arg0->unk24;
    s32 i;

    for (i = func_8001C8B0(t, 1, 0x80000, 0) - 1; i >= 0; i--) {
        func_8005546C(func_8001C8B0(t, 2, (i & 0xFFFF) | 0x80000, 0), 0);
    }
    func_80056228();
}

INCLUDE_ASM("asm/us/ovl_0a/nonmatchings/ovl_0a", func_800B1E8C);

INCLUDE_ASM("asm/us/ovl_0a/nonmatchings/ovl_0a", func_800B2100);

INCLUDE_ASM("asm/us/ovl_0a/nonmatchings/ovl_0a", func_800B21A0);

INCLUDE_ASM("asm/us/ovl_0a/nonmatchings/ovl_0a", func_800B2218);

s32 func_800B2328(s32 arg0) {
    FileTable *t = D_8006794C->unk1C->unk7E4;
    s32 i;
    FileEntry *e = t->unk4;

    for (i = t->unk2 - 1; i >= 0; i--) {
        if (e[i].unk0 == arg0) {
            return e[i].unkC;
        }
    }
    return -1;
}

INCLUDE_ASM("asm/us/ovl_0a/nonmatchings/ovl_0a", func_800B238C);

INCLUDE_ASM("asm/us/ovl_0a/nonmatchings/ovl_0a", func_800B2528);

s32 func_800B29A0(s32 arg0, s32 *arg1) {
    FileEntry *e = func_80063088(D_8006794C->unk1C->unk7E4, arg0);

    if (arg1 != NULL) {
        *arg1 = e->unk8 << 11;
    }
    return e->unk4;
}

INCLUDE_ASM("asm/us/ovl_0a/nonmatchings/ovl_0a", func_800B29F8);

s32 func_800B2BC4(s32 arg0) {
    s32 t = func_8001C8B0(arg0, 3, 0x70000, 0);

    func_80015C28((u8 *)arg0, (u8 *)func_8001C8B0(arg0, 4, t, 0), func_8001C8B0(arg0, 5, t, 0));
    return 0;
}

void func_800B2C40(s32 arg0, u16 arg1) {
    GameData *gd = D_8006794C->unk1C;

    gd->unk826[0] = arg0;
    if (arg0 == 0) {
        gd->unk842 = arg1;
        gd->unk8C4 = VSync(-1);
    }
}

s32 func_800B2C80(void) {
    GameData *gd = D_8006794C->unk1C;

    return (u32)(VSync(-1) - gd->unk8C4) < gd->unk842;
}

void func_800B2CC0(s32 arg0, s32 arg1, s32 arg2, s32 arg3) {
    func_800B31B8(func_8001E350(arg0), arg1, arg2, arg3);
}

INCLUDE_ASM("asm/us/ovl_0a/nonmatchings/ovl_0a", func_800B2D10);

INCLUDE_ASM("asm/us/ovl_0a/nonmatchings/ovl_0a", func_800B31B8);

u8 *func_800B3254(Unk800B3254 *arg0) {
    if (arg0 != NULL && arg0->unk9 != 0) {
        return arg0->unkC[arg0->unk6];
    }
    return NULL;
}

INCLUDE_ASM("asm/us/ovl_0a/nonmatchings/ovl_0a", func_800B3294);

INCLUDE_ASM("asm/us/ovl_0a/nonmatchings/ovl_0a", func_800B3300);

INCLUDE_ASM("asm/us/ovl_0a/nonmatchings/ovl_0a", func_800B3378);

INCLUDE_ASM("asm/us/ovl_0a/nonmatchings/ovl_0a", func_800B3444);

void func_800B3684(void) {
    if (D_8006794C->unk1C->unk8E4.field->unkC->unk14->unk0 & 0x200) {
        func_80050AAC();
    }
}

INCLUDE_ASM("asm/us/ovl_0a/nonmatchings/ovl_0a", func_800B36E0);

void func_800B3A80(void) {
    func_80021C60();
    D_8006794C->unk1C->unk8BC = func_800B36E0;
    func_80021CDC((s32)func_800B4894);
    func_800B3ACC();
}

void func_800B3ACC(void) {
    s32 i;

    D_800CA050 = 0;
    for (i = 7; i >= 0; i--) {
        D_800CA010[i] = -1;
    }
    D_800CA051 = 0;
    for (i = 7; i >= 0; i--) {
        D_800CA030[i] = -1;
    }
}

void func_800B3B2C(s32 arg0) {
    if (arg0 != 0) {
        D_800C9D58 = arg0;
    }
}

s32 func_800B3B40(void) {
    return func_800B3BD8(0x1000B) != 0;
}

s32 func_800B3B64(void) {
    return func_800B3D10(0x1000B) != 0;
}

INCLUDE_ASM("asm/us/ovl_0a/nonmatchings/ovl_0a", func_800B3B88);

s32 *func_800B3BD8(s32 arg0) {
    s32 i;

    for (i = D_800CA050 - 1; i >= 0; i--) {
        if (D_800CA010[i] == arg0) {
            return &D_800CA010[i];
        }
    }
    return NULL;
}

void func_800B3C24(s32 arg0) {
    s32 i;

    for (i = D_800CA050 - 1; i >= 0; i--) {
        if (D_800CA010[i] == arg0) {
            if (i < D_800CA050 - 1) {
                D_800CA010[i] = D_800CA010[D_800CA050 - 1];
            }
            D_800CA050--;
            return;
        }
    }
}

s32 *func_800B3CAC(s32 arg0) {
    s32 *p = func_800B3BD8(arg0);

    if (p != NULL) {
        return p;
    }
    if (D_800CA050 < 8) {
        p = &D_800CA010[D_800CA050];
        D_800CA050++;
        *p = arg0;
        return p;
    }
    return NULL;
}

s32 *func_800B3D10(s32 arg0) {
    s32 i;

    for (i = D_800CA051 - 1; i >= 0; i--) {
        if (D_800CA030[i] == arg0) {
            return &D_800CA030[i];
        }
    }
    return NULL;
}

void func_800B3D5C(s32 arg0) {
    s32 i;

    for (i = D_800CA051 - 1; i >= 0; i--) {
        if (D_800CA030[i] == arg0) {
            if (i < D_800CA051 - 1) {
                D_800CA030[i] = D_800CA030[D_800CA051 - 1];
            }
            D_800CA051--;
            return;
        }
    }
}

s32 *func_800B3DE4(s32 arg0) {
    s32 *p = func_800B3D10(arg0);

    if (p != NULL) {
        return p;
    }
    if (D_800CA051 < 8) {
        p = &D_800CA030[D_800CA051];
        D_800CA051++;
        *p = arg0;
        return p;
    }
    return NULL;
}

INCLUDE_ASM("asm/us/ovl_0a/nonmatchings/ovl_0a", func_800B3E48);

INCLUDE_ASM("asm/us/ovl_0a/nonmatchings/ovl_0a", func_800B3F8C);

INCLUDE_ASM("asm/us/ovl_0a/nonmatchings/ovl_0a", func_800B4894);

INCLUDE_ASM("asm/us/ovl_0a/nonmatchings/ovl_0a", func_800B4A5C);

s32 func_800B4F9C(void) {
    GameData *gd = D_8006794C->unk1C;
    FieldScene *sc = gd->unk8E4.field->unkC->unk14;

    if (sc->unk4A == 1 && (s16)gd->unkC == 0x34 && sc->unkA == 100) {
        return 1;
    }
    return 0;
}

void func_800B5004(s32 arg0, s16 arg1) {
    D_800CA058.unk1 = 1 << arg0;
    D_800CA058.unk2 = arg0;
    D_800CA058.unk4 = arg1;
    D_800CA058.unk6 = -1;
    D_8006794C->unk1C->unk8E4.field->unk0 |= 0x100000;
    D_8006794C->unk0 |= 8;
}

u8 func_800B5064(void) {
    return D_800CA058.unk2;
}

u8 func_800B5074(void) {
    return D_800CA058.unk1;
}

s16 func_800B5084(void) {
    return D_800CA058.unk4;
}

s16 func_800B5094(void) {
    return D_800CA058.unk6;
}

void func_800B50A4(void) {
    func_800B5114();
    while (D_800CA058.unk0 != 4) {
        func_800B5290();
        func_800B52DC();
        func_800B532C();
    }
    func_800B51D0();
}

INCLUDE_ASM("asm/us/ovl_0a/nonmatchings/ovl_0a", func_800B5114);

INCLUDE_ASM("asm/us/ovl_0a/nonmatchings/ovl_0a", func_800B51D0);

void func_800B5290(void) {
    GameData *gd = D_8006794C->unk1C;

    ClearOTagR(gd->unk10[gd->unk8], 0x1000);
    func_80019194();
}

void func_800B52DC(void) {
    if (D_800C9D5C[D_800CA058.unk0].unk4 != NULL) {
        D_800C9D5C[D_800CA058.unk0].unk4(D_800CA058.unk0);
    }
}

INCLUDE_ASM("asm/us/ovl_0a/nonmatchings/ovl_0a", func_800B532C);

INCLUDE_ASM("asm/us/ovl_0a/nonmatchings/ovl_0a", func_800B5430);

s32 func_800B54CC(void) {
    return (D_8006794C->unk1C->unk8E4.field->unk0 >> 20) & 1;
}

void func_800B54FC(void) {
    func_8002F9D0();
    func_80035430();
    func_8003556C(func_800B57C8);
    func_8003575C(D_800A717C);
}

void func_800B553C(void) {
    func_8002C308(0);
    func_800B559C();
    func_800B5660();
    func_80035490();
}

void func_800B5574(void) {
    func_80035580(NULL);
    func_8003556C(NULL);
}

INCLUDE_ASM("asm/us/ovl_0a/nonmatchings/ovl_0a", func_800B559C);

INCLUDE_ASM("asm/us/ovl_0a/nonmatchings/ovl_0a", func_800B5660);

void func_800B5794(void) {
    if (D_800CA058.unk3 == 0) {
        func_800B5430(2);
    } else {
        func_800B5430(3);
    }
}

INCLUDE_ASM("asm/us/ovl_0a/nonmatchings/ovl_0a", func_800B57C8);

void func_800B5870(void) {
    func_80032120(4, 2);
    func_80032628(func_800B58B8);
    func_80032634(0);
}

void func_800B58A8(void) {
}

void func_800B58B0(void) {
}

void func_800B58B8(void) {
    func_800B5430(1);
}

void func_800B58D8(void) {
    func_80032120(8, ((D_800CA058.unk2 & 3) << 14) | D_800CA058.unk4);
    func_80032628(func_800B5938);
    func_80032634(0);
}

void func_800B5928(void) {
}

void func_800B5930(void) {
}

void func_800B5938(void) {
    func_800B5430(4);
}

INCLUDE_ASM("asm/us/ovl_0a/nonmatchings/ovl_0a", func_800B5958);

INCLUDE_ASM("asm/us/ovl_0a/nonmatchings/ovl_0a", func_800B61A4);

INCLUDE_ASM("asm/us/ovl_0a/nonmatchings/ovl_0a", func_800B6438);

INCLUDE_ASM("asm/us/ovl_0a/nonmatchings/ovl_0a", func_800B6584);

INCLUDE_ASM("asm/us/ovl_0a/nonmatchings/ovl_0a", func_800B71E0);

INCLUDE_ASM("asm/us/ovl_0a/nonmatchings/ovl_0a", func_800B7CEC);

INCLUDE_ASM("asm/us/ovl_0a/nonmatchings/ovl_0a", func_800B7E30);

INCLUDE_ASM("asm/us/ovl_0a/nonmatchings/ovl_0a", func_800B7F14);

INCLUDE_ASM("asm/us/ovl_0a/nonmatchings/ovl_0a", func_800B8834);

INCLUDE_ASM("asm/us/ovl_0a/nonmatchings/ovl_0a", func_800B8C80);

INCLUDE_ASM("asm/us/ovl_0a/nonmatchings/ovl_0a", func_800B9134);

INCLUDE_ASM("asm/us/ovl_0a/nonmatchings/ovl_0a", func_800B9738);

INCLUDE_ASM("asm/us/ovl_0a/nonmatchings/ovl_0a", func_800B9E4C);

INCLUDE_ASM("asm/us/ovl_0a/nonmatchings/ovl_0a", func_800BA0F0);

INCLUDE_ASM("asm/us/ovl_0a/nonmatchings/ovl_0a", func_800BA190);

INCLUDE_ASM("asm/us/ovl_0a/nonmatchings/ovl_0a", func_800BA3CC);

INCLUDE_ASM("asm/us/ovl_0a/nonmatchings/ovl_0a", func_800BA560);

INCLUDE_ASM("asm/us/ovl_0a/nonmatchings/ovl_0a", func_800BB090);

INCLUDE_ASM("asm/us/ovl_0a/nonmatchings/ovl_0a", func_800BB3C4);

INCLUDE_ASM("asm/us/ovl_0a/nonmatchings/ovl_0a", func_800BB508);

INCLUDE_ASM("asm/us/ovl_0a/nonmatchings/ovl_0a", func_800BB714);

INCLUDE_ASM("asm/us/ovl_0a/nonmatchings/ovl_0a", func_800BC33C);

INCLUDE_ASM("asm/us/ovl_0a/nonmatchings/ovl_0a", func_800BC600);

INCLUDE_ASM("asm/us/ovl_0a/nonmatchings/ovl_0a", func_800BD130);

INCLUDE_ASM("asm/us/ovl_0a/nonmatchings/ovl_0a", func_800BD3A0);

INCLUDE_ASM("asm/us/ovl_0a/nonmatchings/ovl_0a", func_800BD4D4);

INCLUDE_ASM("asm/us/ovl_0a/nonmatchings/ovl_0a", func_800BD724);

INCLUDE_ASM("asm/us/ovl_0a/nonmatchings/ovl_0a", func_800BD884);

INCLUDE_ASM("asm/us/ovl_0a/nonmatchings/ovl_0a", func_800BDC44);

INCLUDE_ASM("asm/us/ovl_0a/nonmatchings/ovl_0a", func_800BDED4);

INCLUDE_ASM("asm/us/ovl_0a/nonmatchings/ovl_0a", func_800BE158);

INCLUDE_ASM("asm/us/ovl_0a/nonmatchings/ovl_0a", func_800BEDE8);

INCLUDE_ASM("asm/us/ovl_0a/nonmatchings/ovl_0a", func_800BF0C4);

INCLUDE_ASM("asm/us/ovl_0a/nonmatchings/ovl_0a", func_800BF660);

INCLUDE_ASM("asm/us/ovl_0a/nonmatchings/ovl_0a", func_800BFCE8);

INCLUDE_ASM("asm/us/ovl_0a/nonmatchings/ovl_0a", func_800BFEE0);

INCLUDE_ASM("asm/us/ovl_0a/nonmatchings/ovl_0a", func_800C0038);

INCLUDE_ASM("asm/us/ovl_0a/nonmatchings/ovl_0a", func_800C02D0);

INCLUDE_ASM("asm/us/ovl_0a/nonmatchings/ovl_0a", func_800C0650);

INCLUDE_ASM("asm/us/ovl_0a/nonmatchings/ovl_0a", func_800C07D8);

INCLUDE_ASM("asm/us/ovl_0a/nonmatchings/ovl_0a", func_800C0BCC);

void func_800C0D38(void) {
}

void func_800C0D40(s32 arg0, s32 arg1, s32 arg2, s32 arg3, s32 arg4, u8 *arg5) {
    D_800C9DA4.unk0 = arg0;
    D_800C9DA4.unk4 = arg1;
    D_800C9DA4.unk8 = arg2;
    D_800C9DA4.unkC = arg3;
    D_800C9DA4.unk10 = 0;
    D_800C9DA4.unk24 = 0;
    D_800C9DA4.unk28 = NULL;
    D_800C9DA4.unk2C = 0;
    D_800C9DA4.unk30 = 0;
    D_800C9DA4.unk34 = 0;
    D_800C9DA4.unk14 = arg4;
    D_800C9DA4.unk18 = arg5;
    D_800C9DA4.unk1C = arg5;
    D_800C9DA4.unk20 = arg5;
}

void func_800C0D8C(s32 arg0, s32 arg1) {
    D_800C9DA4.unk2C = arg0;
    D_800C9DA4.unk30 = arg1;
    D_800C9DA4.unk34 = 0;
}

void func_800C0DA4(Unk800C9DB4 *arg0) {
    D_800C9DB4 = arg0;
}

s32 func_800C0DB0(s32 arg0, s32 arg1, s32 arg2) {
    u16 x = D_800C9DB4->unk20 + arg0;
    u16 y = D_800C9DB4->unk22 + arg1;
    u16 z;

    D_800C9DB4->unk20 = x;
    D_800C9DB4->unk22 = y;
    D_800C9DB4->unk24 = x;
    D_800C9DB4->unk26 = y;
    z = D_800C9DB4->unk1C + arg2;
    D_800C9DB4->unk1C = z;
    D_800C9DB4->unk1E = z;
    return 1;
}

s32 func_800C0DF4(s16 arg0, s16 arg1, s16 arg2, s32 arg3) {
    s32 f;

    if (!(D_800CA068.unk0 & 0x40)) {
        return 0;
    }
    D_800CA068.unk18 = 1;
    D_800CA068.unk14 = arg0;
    f = D_800CA068.unk0 & ~0xF;
    D_800CA068.unk16 = arg1;
    D_800CA068.unk1A = arg2;
    D_800CA068.unk0 = f;
    D_800CA068.unk10 = D_800CA068.unk4;
    if (arg3 == 8) {
        D_800CA068.unk0 = f | 8;
    } else {
        D_800CA068.unk0 = f;
    }
    D_800CA068.unk0 |= 1;
    return 1;
}

s32 func_800C0E6C(u16 *arg0, u16 *arg1) {
    *arg0 = D_800CA068.unk4.unk0;
    *arg1 = D_800CA068.unk4.unk2;
    return 1;
}

s32 func_800C0E94(s32 arg0, s32 arg1, s32 arg2) {
    if (arg0 != 0) {
        D_800CA068.unk0 |= 0x40;
        if (arg1 > 0) {
            func_800C0038(arg1, arg2);
        }
    } else {
        D_800CA068.unk0 &= ~0x40;
    }
    return 1;
}

s32 func_800C0EF0(void) {
    if (D_800CA068.unk0 & 0x40) {
        return 1;
    }
    return 0;
}

INCLUDE_ASM("asm/us/ovl_0a/nonmatchings/ovl_0a", func_800C0F08);

INCLUDE_ASM("asm/us/ovl_0a/nonmatchings/ovl_0a", func_800C0FB0);

s32 func_800C0FF4(s32 arg0) {
    if (arg0 == -1) {
        D_800CA0E0 = 3600;
    } else {
        D_800CA0E0 = arg0 * arg0;
    }
    return 1;
}

s32 func_800C1024(s16 arg0) {
    D_800CA068.unk76 = arg0;
    return 1;
}

s32 func_800C1034(void) {
    if (D_800CA068.unk0 & 0x20) {
        return 1;
    }
    return 0;
}

INCLUDE_ASM("asm/us/ovl_0a/nonmatchings/ovl_0a", func_800C104C);

INCLUDE_ASM("asm/us/ovl_0a/nonmatchings/ovl_0a", func_800C1100);

s32 func_800C119C(s32 arg0, s16 arg1, s16 arg2, s16 arg3, s16 arg4) {
    D_800CA068.unkA8[arg0][0] = arg1;
    D_800CA068.unkA8[arg0][1] = arg2;
    D_800CA068.unkB8[arg0][0] = arg3;
    D_800CA068.unkB8[arg0][1] = arg4;
    return 1;
}

s32 func_800C11C8(s32 arg0, s16 arg1, s16 arg2) {
    D_800CA068.unkC8[arg0] = arg1;
    D_800CA068.unkD0[arg0] = arg2;
    return 1;
}

INCLUDE_ASM("asm/us/ovl_0a/nonmatchings/ovl_0a", func_800C11E8);

s32 func_800C130C(s32 arg0, u8 *arg1, u8 *arg2, u8 *arg3) {
    *arg1 = D_800C9DA4.unk1C[arg0 * 3];
    *arg2 = D_800C9DA4.unk1C[arg0 * 3 + 1];
    *arg3 = D_800C9DA4.unk1C[arg0 * 3 + 2];
    return 1;
}

INCLUDE_ASM("asm/us/ovl_0a/nonmatchings/ovl_0a", func_800C1368);

INCLUDE_ASM("asm/us/ovl_0a/nonmatchings/ovl_0a", func_800C142C);

INCLUDE_ASM("asm/us/ovl_0a/nonmatchings/ovl_0a", func_800C147C);

INCLUDE_ASM("asm/us/ovl_0a/nonmatchings/ovl_0a", func_800C14DC);

INCLUDE_ASM("asm/us/ovl_0a/nonmatchings/ovl_0a", func_800C152C);

INCLUDE_ASM("asm/us/ovl_0a/nonmatchings/ovl_0a", func_800C15BC);

INCLUDE_ASM("asm/us/ovl_0a/nonmatchings/ovl_0a", func_800C1614);

INCLUDE_ASM("asm/us/ovl_0a/nonmatchings/ovl_0a", func_800C1668);

INCLUDE_ASM("asm/us/ovl_0a/nonmatchings/ovl_0a", func_800C16B0);

INCLUDE_ASM("asm/us/ovl_0a/nonmatchings/ovl_0a", func_800C16F4);

INCLUDE_ASM("asm/us/ovl_0a/nonmatchings/ovl_0a", func_800C17AC);

INCLUDE_ASM("asm/us/ovl_0a/nonmatchings/ovl_0a", func_800C17F0);

INCLUDE_ASM("asm/us/ovl_0a/nonmatchings/ovl_0a", func_800C1818);

INCLUDE_ASM("asm/us/ovl_0a/nonmatchings/ovl_0a", func_800C1868);

INCLUDE_ASM("asm/us/ovl_0a/nonmatchings/ovl_0a", func_800C18A0);

INCLUDE_ASM("asm/us/ovl_0a/nonmatchings/ovl_0a", func_800C18D4);

INCLUDE_ASM("asm/us/ovl_0a/nonmatchings/ovl_0a", func_800C196C);

s32 func_800C1A4C(s32 arg0, u8 *arg1) {
    *arg1 = D_800C9DCC[arg0].unk0;
    return 1;
}

s32 func_800C1A78(s32 arg0) {
    return D_800C9DD0 + (arg0 << 9);
}

INCLUDE_ASM("asm/us/ovl_0a/nonmatchings/ovl_0a", func_800C1A8C);

s32 func_800C1B5C(u8 *arg0) {
    *arg0 = D_800CA068.unk75;
    return 1;
}

INCLUDE_ASM("asm/us/ovl_0a/nonmatchings/ovl_0a", func_800C1B74);

INCLUDE_ASM("asm/us/ovl_0a/nonmatchings/ovl_0a", func_800C1BB4);

s32 func_800C1C04(s32 arg0) {
    return arg0 * 4 + 0x2C;
}

s32 func_800C1C10(s32 arg0) {
    s32 r = D_800C9DA4.unk2C + D_800C9DA4.unk34;

    D_800C9DA4.unk34 += arg0;
    return r;
}

INCLUDE_ASM("asm/us/ovl_0a/nonmatchings/ovl_0a", func_800C1C30);

INCLUDE_ASM("asm/us/ovl_0a/nonmatchings/ovl_0a", func_800C1CC0);

s32 func_800C1D08(void) {
    return 1;
}

INCLUDE_ASM("asm/us/ovl_0a/nonmatchings/ovl_0a", func_800C1D10);

s32 func_800C1DAC(void) {
    return 1;
}

s32 func_800C1DB4(void) {
    return 1;
}

s32 func_800C1DBC(void) {
    return 1;
}

INCLUDE_ASM("asm/us/ovl_0a/nonmatchings/ovl_0a", func_800C1DC4);

INCLUDE_ASM("asm/us/ovl_0a/nonmatchings/ovl_0a", func_800C1DE0);

INCLUDE_ASM("asm/us/ovl_0a/nonmatchings/ovl_0a", func_800C1DF4);

INCLUDE_ASM("asm/us/ovl_0a/nonmatchings/ovl_0a", func_800C1E04);

INCLUDE_ASM("asm/us/ovl_0a/nonmatchings/ovl_0a", func_800C1E14);

INCLUDE_ASM("asm/us/ovl_0a/nonmatchings/ovl_0a", func_800C1F98);

INCLUDE_ASM("asm/us/ovl_0a/nonmatchings/ovl_0a", func_800C20BC);

INCLUDE_ASM("asm/us/ovl_0a/nonmatchings/ovl_0a", func_800C2390);

INCLUDE_ASM("asm/us/ovl_0a/nonmatchings/ovl_0a", func_800C2524);

INCLUDE_ASM("asm/us/ovl_0a/nonmatchings/ovl_0a", func_800C2920);

INCLUDE_ASM("asm/us/ovl_0a/nonmatchings/ovl_0a", func_800C2AF0);

INCLUDE_ASM("asm/us/ovl_0a/nonmatchings/ovl_0a", func_800C31F8);

INCLUDE_ASM("asm/us/ovl_0a/nonmatchings/ovl_0a", func_800C3608);

INCLUDE_ASM("asm/us/ovl_0a/nonmatchings/ovl_0a", func_800C3B80);

INCLUDE_ASM("asm/us/ovl_0a/nonmatchings/ovl_0a", func_800C3D28);

INCLUDE_ASM("asm/us/ovl_0a/nonmatchings/ovl_0a", func_800C3E18);

INCLUDE_ASM("asm/us/ovl_0a/nonmatchings/ovl_0a", func_800C3EC0);

INCLUDE_ASM("asm/us/ovl_0a/nonmatchings/ovl_0a", func_800C420C);

INCLUDE_ASM("asm/us/ovl_0a/nonmatchings/ovl_0a", func_800C4378);

INCLUDE_ASM("asm/us/ovl_0a/nonmatchings/ovl_0a", func_800C44F4);

INCLUDE_ASM("asm/us/ovl_0a/nonmatchings/ovl_0a", func_800C45C0);

INCLUDE_ASM("asm/us/ovl_0a/nonmatchings/ovl_0a", func_800C47E4);

INCLUDE_ASM("asm/us/ovl_0a/nonmatchings/ovl_0a", func_800C48DC);

INCLUDE_ASM("asm/us/ovl_0a/nonmatchings/ovl_0a", func_800C4B30);

INCLUDE_ASM("asm/us/ovl_0a/nonmatchings/ovl_0a", func_800C4EC8);

INCLUDE_ASM("asm/us/ovl_0a/nonmatchings/ovl_0a", func_800C519C);

INCLUDE_RODATA("asm/us/ovl_0a/nonmatchings/ovl_0a", D_800A717C);

INCLUDE_ASM("asm/us/ovl_0a/nonmatchings/ovl_0a", func_800C5A7C);

INCLUDE_ASM("asm/us/ovl_0a/nonmatchings/ovl_0a", func_800C60F0);

INCLUDE_ASM("asm/us/ovl_0a/nonmatchings/ovl_0a", func_800C6408);

INCLUDE_ASM("asm/us/ovl_0a/nonmatchings/ovl_0a", func_800C65CC);

INCLUDE_ASM("asm/us/ovl_0a/nonmatchings/ovl_0a", func_800C6C04);

INCLUDE_ASM("asm/us/ovl_0a/nonmatchings/ovl_0a", func_800C6DA4);

s32 func_800C6E94(void) {
    s32 i;

    for (i = 0; i < 30; i++) {
        D_800CAE98[i].unk0 = 2;
        D_800CAE98[i].unk2 = 0;
        D_800CAE98[i].unk4 = 0;
        D_800CAE98[i].unk6 = 0;
        D_800CAE98[i].unk10 = 0;
        D_800CAE98[i].unk14 = 0;
        D_800CAE98[i].unk12 = 0x100;
        D_800CAE98[i].unk8 = 0;
        D_800CAE98[i].unkA = 0;
        D_800CAE98[i].unkC = 0;
        D_800CAE98[i].unkE = 0;
    }
    return 1;
}

INCLUDE_ASM("asm/us/ovl_0a/nonmatchings/ovl_0a", func_800C6EEC);

INCLUDE_ASM("asm/us/ovl_0a/nonmatchings/ovl_0a", func_800C7100);

INCLUDE_ASM("asm/us/ovl_0a/nonmatchings/ovl_0a", func_800C72D8);

s32 func_800C73AC(void) {
    s32 n = 0;
    s16 i = 0;

    if (D_800C9DF0.unk2 != 0) {
        do {
            if (func_800C44F4(i) > 0) {
                n++;
            }
            i++;
        } while (i < D_800C9DF0.unk2);
    }
    return n & 0xFFFF;
}

s32 func_800C7430(void) {
    D_800C9DF0.unk8 = 0;
    D_800C9DF0.unk2 = 0;
    D_800C9DF0.unk4 = 0;
    D_800C9DF0.unk0 = 0;
    return 1;
}

s32 func_800C7450(void) {
    return 1;
}

void func_800C7458(s32 *arg0) {
    *arg0 = 1;
}

s32 func_800C7464(u8 arg0) {
    D_800C9DF0.unk1 = arg0;
    return 1;
}

s32 func_800C7474(u8 *arg0) {
    *arg0 = D_800C9DF0.unk1;
    return 1;
}

Unk800C9DF4 *func_800C748C(s32 arg0) {
    return &D_800C9DF0.unk4[arg0];
}

s32 func_800C74A8(s32 arg0, FieldScene *arg1) {
    D_800C9DF0.unk4[arg0].unk0 = 0;
    D_800C9DF0.unk4[arg0].unk2 = 0;
    D_800C9DF0.unk4[arg0].unk24 = arg1->unkC;
    return 1;
}

s32 func_800C74F0(FieldScene *arg0) {
    D_800C9DF0.unk4[D_800C9DF0.unk2].unk0 = 0;
    D_800C9DF0.unk4[D_800C9DF0.unk2].unk2 = 0;
    D_800C9DF0.unk4[D_800C9DF0.unk2].unk24 = arg0->unkC;
    D_800C9DF0.unk2++;
    return 1;
}

s32 func_800C7570(s32 arg0, u16 arg1) {
    D_800C9DF0.unk4[arg0].unk1C = arg1;
    D_800C9DF0.unk4[arg0].unk20 = arg1 * arg1;
    return 1;
}

s32 func_800C75B4(s32 arg0, s32 *arg1) {
    *arg1 = D_800C9DF0.unk4[arg0].unk1C;
    return 1;
}

s32 func_800C75E0(s32 arg0, s32 *arg1) {
    *arg1 = D_800C9DF0.unk4[arg0].unk20;
    return 1;
}

INCLUDE_ASM("asm/us/ovl_0a/nonmatchings/ovl_0a", func_800C760C);

s32 func_800C768C(s32 arg0, s16 *arg1, u16 *arg2) {
    Unk800C9DF4 *a = &D_800C9DF0.unk4[arg0];
    Unk800C9E00 *e = &D_800C9DF0.unk10[a->unk6];

    *arg1 = -e->unkA;
    *arg2 = e->unk8;
    return 1;
}

s32 func_800C76E8(s32 arg0, u16 *arg1, u16 *arg2) {
    Unk800C9DF4 *e = &D_800C9DF0.unk4[arg0];

    *arg1 = e->unk6;
    *arg2 = e->unk4;
    return 1;
}

s32 func_800C7720(s32 arg0) {
    Unk800C9DF4 *e = &D_800C9DF0.unk4[arg0];

    e->unk24->unk14.vx = e->unk24->unk40.vx;
    e->unk24->unk14.vy = e->unk24->unk40.vy;
    e->unk24->unk14.vz = e->unk24->unk40.vz;
    e->unk4 = e->unk8;
    e->unk6 = e->unkA;
    return 1;
}

INCLUDE_ASM("asm/us/ovl_0a/nonmatchings/ovl_0a", func_800C778C);

s32 func_800C77DC(u16 *arg0, u8 **arg1) {
    *arg0 = D_800C9DDE;
    *arg1 = D_800CAA98;
    return 1;
}

INCLUDE_ASM("asm/us/ovl_0a/nonmatchings/ovl_0a", func_800C7800);

INCLUDE_ASM("asm/us/ovl_0a/nonmatchings/ovl_0a", func_800C784C);

INCLUDE_ASM("asm/us/ovl_0a/nonmatchings/ovl_0a", func_800C7890);

INCLUDE_ASM("asm/us/ovl_0a/nonmatchings/ovl_0a", func_800C78D8);

s32 func_800C7914(u16 *arg0, u8 **arg1) {
    *arg0 = D_800C9DE0;
    *arg1 = D_800CAAB8;
    return 1;
}

INCLUDE_ASM("asm/us/ovl_0a/nonmatchings/ovl_0a", func_800C7938);

INCLUDE_ASM("asm/us/ovl_0a/nonmatchings/ovl_0a", func_800C7968);

INCLUDE_ASM("asm/us/ovl_0a/nonmatchings/ovl_0a", func_800C79B0);

s32 func_800C7AF8(u32 arg0, u32 arg1) {
    Unk800C9E10 *e;

    if (arg0 >= D_800C9DF0.unk8->unk2C) {
        return 0;
    }
    e = &D_800C9DF0.unk20[arg0];
    if (arg1 >= e->unk2) {
        return 0;
    }
    e->unk8 = arg1 << 8;
    e->unk6 = 0;
    e->unk0 |= 1;
    return 1;
}

s32 func_800C7B68(u32 arg0, s32 arg1) {
    Unk800C9E10 *e;

    if (arg0 >= D_800C9DF0.unk8->unk2C) {
        return 0;
    }
    e = &D_800C9DF0.unk20[arg0];
    if (arg1 != 0) {
        e->unk0 |= 5;
    } else {
        e->unk0 &= ~5;
    }
    return 1;
}

INCLUDE_ASM("asm/us/ovl_0a/nonmatchings/ovl_0a", func_800C7BC4);

INCLUDE_ASM("asm/us/ovl_0a/nonmatchings/ovl_0a", func_800C7C04);

INCLUDE_ASM("asm/us/ovl_0a/nonmatchings/ovl_0a", func_800C7C70);

s32 func_800C7CDC(u32 arg0, s32 arg1) {
    Unk800C9E10 *e;

    if (arg0 >= D_800C9DF0.unk8->unk2C) {
        return 0;
    }
    e = &D_800C9DF0.unk20[arg0];
    e->unk0 = (e->unk0 & 0xFFE7) | (arg1 & 0x18);
    return 1;
}

INCLUDE_ASM("asm/us/ovl_0a/nonmatchings/ovl_0a", func_800C7D30);

INCLUDE_ASM("asm/us/ovl_0a/nonmatchings/ovl_0a", func_800C7DF0);

INCLUDE_ASM("asm/us/ovl_0a/nonmatchings/ovl_0a", func_800C7EE8);

INCLUDE_ASM("asm/us/ovl_0a/nonmatchings/ovl_0a", func_800C7F34);

INCLUDE_ASM("asm/us/ovl_0a/nonmatchings/ovl_0a", func_800C7F7C);

s32 func_800C7FBC(s32 arg0, s32 arg1) {
    Unk800CAE98 *e = func_800C8108_inline(arg0);
    u16 v;

    if (arg1 != 0) {
        v = e->unk0 | 5;
    } else {
        v = e->unk0 & ~5;
    }
    e->unk0 = v;
    func_800C6EEC();
    return 1;
}

s32 func_800C8010(s32 arg0, s32 arg1) {
    Unk800CAE98 *e = func_800C8108_inline(arg0);

    e->unk0 = (e->unk0 & 0xFFE7) | (arg1 & 0x18);
    return 1;
}

s32 func_800C8044(s32 arg0, s16 arg1) {
    func_800C8108_inline(arg0)->unk2 = arg1;
    return 1;
}

s32 func_800C8068(s32 arg0, s16 arg1) {
    func_800C8108_inline(arg0)->unk12 = arg1;
    return 1;
}

s32 func_800C808C(s32 arg0, s16 arg1) {
    func_800C8108_inline(arg0)->unk6 = arg1;
    return 1;
}

s32 func_800C80B0(s32 arg0, s16 arg1, s16 arg2) {
    Unk800CAE98 *e = func_800C8108_inline(arg0);

    e->unk8 = arg1;
    e->unkA = arg2;
    e->unk10 = e->unkA - e->unk8;
    return 1;
}

s32 func_800C80E4(s32 arg0, s16 arg1) {
    func_800C8108_inline(arg0)->unk4 = arg1;
    return 1;
}

Unk800CAE98 *func_800C8108(s32 arg0) {
    return &D_800CAE98[arg0];
}

INCLUDE_ASM("asm/us/ovl_0a/nonmatchings/ovl_0a", func_800C8124);

INCLUDE_ASM("asm/us/ovl_0a/nonmatchings/ovl_0a", func_800C8284);

INCLUDE_ASM("asm/us/ovl_0a/nonmatchings/ovl_0a", func_800C8354);

INCLUDE_ASM("asm/us/ovl_0a/nonmatchings/ovl_0a", func_800C842C);

INCLUDE_ASM("asm/us/ovl_0a/nonmatchings/ovl_0a", func_800C85B8);

INCLUDE_ASM("asm/us/ovl_0a/nonmatchings/ovl_0a", func_800C86C8);

INCLUDE_ASM("asm/us/ovl_0a/nonmatchings/ovl_0a", func_800C883C);

INCLUDE_ASM("asm/us/ovl_0a/nonmatchings/ovl_0a", func_800C8AE8);

INCLUDE_ASM("asm/us/ovl_0a/nonmatchings/ovl_0a", func_800C8C08);

INCLUDE_ASM("asm/us/ovl_0a/nonmatchings/ovl_0a", func_800C8D40);

INCLUDE_ASM("asm/us/ovl_0a/nonmatchings/ovl_0a", func_800C8E2C);

INCLUDE_ASM("asm/us/ovl_0a/nonmatchings/ovl_0a", func_800C9014);

INCLUDE_ASM("asm/us/ovl_0a/nonmatchings/ovl_0a", func_800C90D0);

void func_800C923C(void) {
    if (D_800CB188.unk0 + 1 != 5) {
        if (func_80036300(D_800CB188.unk0 + 1) == 0) {
            func_800361CC(D_800CB188.unk0 + 1);
        } else {
            D_800CB188.unk1 = 1;
        }
    } else {
        D_800CB188.unk1 = 2;
        D_800CB188.unk4 = 0;
    }
    func_8002F488(0x67);
}

void func_800C92BC(void) {
    D_80077E50.unk1 = 0;
}

INCLUDE_ASM("asm/us/ovl_0a/nonmatchings/ovl_0a", func_800C92C8);

INCLUDE_ASM("asm/us/ovl_0a/nonmatchings/ovl_0a", func_800C93D4);

void func_800C94B0(void) {
    func_800C951C();
    func_800C9580();
    func_800C95B8();
}

void func_800C94E0(void) {
    func_80035674(1);
    func_80035580(NULL);
    func_8003556C(NULL);
}

void func_800C9510(u8 arg0) {
    D_800CB188.unk0 = arg0;
}

void func_800C951C(void) {
    if (D_80077E50.unk1 == 0 && func_8002F810() == 0) {
        if (D_800C9E14[D_800CB188.unk1] != NULL) {
            D_800C9E14[D_800CB188.unk1]();
        }
    }
}

void func_800C9580(void) {
    func_800C8C08();
    func_800C8D40();
    func_800C9630();
    func_800C8E2C();
}

void func_800C95B8(void) {
    s32 i;

    if (D_80077E50.unk1 != 0) {
        for (i = 0; i < 6; i++) {
            if (D_800CB188.unk8[i].unkC != 0) {
                func_800631AC(&D_800CB188.unk8[i]);
            }
        }
    }
}

INCLUDE_ASM("asm/us/ovl_0a/nonmatchings/ovl_0a", func_800C9630);

s32 func_800C9700(u16 *arg0) {
    return *arg0 & 0x7FFF;
}

INCLUDE_ASM("asm/us/ovl_0a/nonmatchings/ovl_0a", func_800C970C);

INCLUDE_ASM("asm/us/ovl_0a/nonmatchings/ovl_0a", func_800C9968);

INCLUDE_ASM("asm/us/ovl_0a/nonmatchings/ovl_0a", func_800C9AB8);

INCLUDE_ASM("asm/us/ovl_0a/nonmatchings/ovl_0a", func_800C9CCC);

INCLUDE_RODATA("asm/us/ovl_0a/nonmatchings/ovl_0a", D_800A7814);

INCLUDE_RODATA("asm/us/ovl_0a/nonmatchings/ovl_0a", D_800A7860);
