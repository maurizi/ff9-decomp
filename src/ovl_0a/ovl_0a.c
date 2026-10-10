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
        func_800A7B10((s16)D_8006794C->unk1C->unkC);
    } while (!(D_8006794C->unk0 & 7));
    func_800A8698();
}

INCLUDE_RODATA("asm/us/ovl_0a/nonmatchings/ovl_0a", D_800A7000);

/* field main loop */
void func_800A7B10(s32 arg0) {
    func_800A89C4(arg0);
    SetDispMask(1);
    do {
        func_800AE788();
        func_80019194();
        if ((D_80073990 & 0x04000000) && (D_800739A0[0] & 0xF8) && !(D_800739A0[0] & 0xFE0001)) {
            func_800AF740();
        }
        if (!(D_8006794C->unk1C->unk0 & 0x100)) {
            func_800ADB74();
            if (!(D_8006794C->unk1C->unk0 & 1) && (D_80073990 & 0x40000) && (D_800739A0[0] & 0x400F0) &&
                !(D_800739A0[0] & 0x04FA0009)) {
                D_8006794C->unk1C->unk0 |= 2;
                func_80032120(0, 0);
                func_8004A9E8();
            }
            if (!(D_8006794C->unk1C->unk0 & 2)) {
                switch (func_8004BC38()) {
                case 3:
                    D_8006794C->unk1C->unk8E4.field->unkC->unk14->unk4A = 2;
                    func_800AED04();
                    break;
                case 4:
                    if (D_8006794C->unk1C->unk8E4.field->unkC->unk14->unkA == 0x3E80) {
                        D_8006794C->unk1C->unk8E4.field->unkC->unk14->unk4A = 4;
                        D_8006794C->unk0 |= 2;
                    } else {
                        D_8006794C->unk0 |= 8;
                    }
                    break;
                case 5:
                    D_8006794C->unk1C->unk8E4.field->unkC->unk14->unk4A = 3;
                    D_8006794C->unk0 |= 2;
                    break;
                case 7:
                    D_8006794C->unk1C->unk8E4.field->unkC->unk14->unk4A = 9;
                    D_8006794C->unk0 |= 2;
                    break;
                case 8:
                    D_8006794C->unk1C->unk8E4.field->unkC->unk14->unk4A = 7;
                    D_8006794C->unk0 |= 2;
                    break;
                }
            }
            if (!(D_8006794C->unk1C->unk0 & 1)) {
                func_80032100();
            }
            if (D_8006794C->unk0 & 0x100000) {
                D_8006794C->unk0 |= 8;
                D_8006794C->unk1C->unk8E4.field->unk0 |= 0x10000;
                break;
            }
            if (!(D_8006794C->unk1C->unk0 & 8)) {
                func_800BFEE0();
            }
            if (!(D_8006794C->unk1C->unk0 & 4)) {
                func_800AB3CC();
            }
            if (!(D_8006794C->unk1C->unk8E4.field->unk0 & 8)) {
                func_800C60F0();
                func_800C7100();
            }
            if (!(D_8006794C->unk1C->unk8E4.field->unk0 & 0x10)) {
                func_80054B78();
            }
            if (!(D_8006794C->unk1C->unk0 & 0x10)) {
                func_800AFDE8();
            }
            if (!(D_8006794C->unk1C->unk0 & 8)) {
                func_800B3684();
            }
            if (!(D_8006794C->unk1C->unk8E4.field->unk0 & 1)) {
                func_80023A0C();
            }
            if (!(D_8006794C->unk1C->unk8E4.field->unk0 & 0x800)) {
                func_80032098();
            }
            if (!(D_8006794C->unk1C->unk8E4.field->unk0 & 4)) {
                func_800AED6C();
            }
            if (!(D_8006794C->unk1C->unk8E4.field->unk0 & 0x80)) {
                func_80023A4C();
            }
            if (!(D_8006794C->unk1C->unk8E4.field->unk0 & 0x1000)) {
                func_8002246C();
            }
        }
        func_800AE930();
    } while (!(D_8006794C->unk0 & 0xF));
    if (func_800B4F9C() != 0) {
        func_800B4A5C();
    }
    if ((u8)func_800B54CC()) {
        func_800B50A4();
    }
    func_800A8E58();
}

void func_800A80A4(void) {
    s32 i;

    func_801F3488(D_8006794C->unk1C->unk8E4.field->unkC->unk14->unkC);
    D_8006794C->unk8 = 0;
    D_8006794C->unk0 |= 0x100000;
    if ((D_8006794C->unk1C->unk0 & 0x20000000) && D_8006794C->unk1C->unk81C[0] != 0xFFFF) {
        func_800B0124(0x8301, D_8006794C->unk1C->unk81C[0], 0x1E, 0, 0);
        for (i = 0x1D; i >= 0; i--) {
            VSync(0);
        }
        func_800B0124(0x100, D_8006794C->unk1C->unk81C[0], 0, 0, 0);
    }
    D_80067950++;
}

void func_800A8198(void) {
    FieldMap *f;
    GameData *gd;
    GameData *gd2;

    func_800A8344();
    f = D_8006794C->unk1C->unk8E4.field;
    func_800A8380();
    D_8006794C->unk9 = 2;
    func_8001CD04(0xE0);
    func_8001CFB4();
    SetDispMask(1);
    D_8006794C->unk1C->unk838 = func_800AAB30;
    D_8006794C->unk1C->unk83C = (void (*)(s32, s32, s32, s32, s32))func_800B0124;
    D_8006794C->unk1C->unk8BC = func_800B36E0;
    func_80021CDC(0);
    D_8006794C->unk0 &= ~7;
    D_8006794C->unk1C->unk84C = 0;
    gd = D_8006794C->unk1C;
    gd->unk39 = 0;
    gd->unk38 = 0;
    gd2 = D_8006794C->unk1C;
    gd2->unk836 = 0xFFFF;
    gd2->unk834 = 0xFFFF;
    func_800B1D60();
    func_800AC5F0();
    func_800A847C();
    func_800A83E0();
    func_800239D0();
    if (D_8006794C->unk1C->unk0 & 0x400000) {
        func_800B0124(0x8601, 0, 0x78, 0, 0);
    }
    func_8002FC30();
    f->unk4 = D_80067948;
}

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

s32 func_800A83E0(void) {
    s32 addr = (s32)func_8001C7FC(4, 0x90164, 0);
    s32 size = (s32)func_8001C7FC(5, 0x90164, 0);

    func_8001DAA4((s32)(D_80067944 + 0x146F00), addr, size, NULL);
    DrawSync(0);
    D_8006794C->unk1C->unk8E4.field->unk20 = size;
    return 0;
}

s32 func_800A847C(void) {
    Unk80021938 *p;
    FileEntry *f;

    if (func_800220E8(0x2714) != 0) {
        p = func_80021E5C(0x2714);
        func_8001DAA4(D_80067948, p->unk8, p->unkC, NULL);
        func_80022390(0x2714);
        DrawSync(0);
    } else {
        f = func_8001E218(1, 0x2714);
        func_8002177C(D_8006794C->unk1C->unk854, f->unk4, (f->unkC - f->unk4) << 11, (void *)D_80067948, 0, -1, 0);
        while (func_800217C4(D_8006794C->unk1C->unk854) != 0) {
        }
    }
    D_8006794C->unk1C->unk8E4.field->unk10 = (void *)D_80067948;
    func_8001C750((s32)D_8006794C->unk1C->unk8E4.field->unk10, 0);
    return 0;
}

s32 func_800A8588(void) {
    void *p = D_8006794C->unk1C->unk8E4.field->unk10;

    if (p != NULL) {
        func_8001C784(p);
        D_8006794C->unk1C->unk8E4.field->unk10 = NULL;
    }
    return 0;
}

void func_800A85F4(s32 arg0) {
    FieldMapObj *o;

    D_80067948 = D_8006794C->unk1C->unk8E4.field->unk4;
    func_800A86FC();
    o = D_8006794C->unk1C->unk8E4.field->unkC;
    func_800A8724();
    func_8001CFB4();
    func_800A87DC(o, arg0);
    func_800A879C(o, arg0);
    o->unk4 = D_80067948;
}

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

s32 func_800A87DC(FieldMapObj *arg0, s32 arg1) {
    s32 id;
    GameData *gd;
    Unk80021938 *e;
    CdReadCtx *cd;
    GameData *g;
    s32 t;

    gd = D_8006794C->unk1C;
    id = (u16)arg1 | 0x60000;
    cd = gd->unk854;
    if (func_800220E8(id) != 0) {
        e = func_80021E5C(id);
        D_8006794C->unk1C->unk854->unk20 = (u8 *)(e->unk8 + e->unkC);
        D_8006794C->unk1C->unk854->unk10 = e->unkC;
        D_8006794C->unk1C->unk854->unk4 = e->unk1C;
        func_8001DAA4(D_80067948, e->unk8, e->unkC, NULL);
        func_80022390(id);
        DrawSync(0);
        g = D_8006794C->unk1C;
        t = g->unk854->unk10;
        if (t < g->unk854->unk4) {
            func_8002177C(g->unk854, g->unk7FC[0], g->unk804[0] << 11, (void *)(D_80067948 + t), t, -1, NULL);
            while (func_800217C4(D_8006794C->unk1C->unk854) != 0) {
            }
        }
    } else {
        func_8002177C(cd, gd->unk7FC[0], gd->unk804[0] << 11, (void *)D_80067948, 0, -1, NULL);
        while (func_800217C4(cd) != 0) {
        }
    }
    arg0->unkC = (void *)D_80067948;
    func_8001C750(D_80067948, 0);
    D_80067948 += func_8001CA70((s32)arg0->unkC);
    return 0;
}

s32 func_800A8990(FieldMapObj *arg0, s16 arg1) {
    func_8001C784(arg0->unkC);
    arg0->unkC = NULL;
    return 0;
}

/* field start-up: load the map and set up the object slots */
void func_800A89C4(s32 arg0) {
    FieldScene *sc;
    s32 i;
    s32 j;

    D_80067948 = D_8006794C->unk1C->unk8E4.field->unkC->unk4;
    func_800A9350();
    sc = D_8006794C->unk1C->unk8E4.field->unkC->unk14;
    func_800A939C();
    func_8001CFB4();
    D_8006794C->unk0 &= ~0x30F;
    D_8006794C->unk1C->unk0 |= 0x20;
    D_8006794C->unk1C->unk8E4.field->unk0 |= 4;
    func_8001CCB4(0, 0, 0);
    D_80073998[0] &= 0xFFFF0006;
    for (i = 1; i >= 0; i--) {
        for (j = 3; j >= 0; j--) {
            D_800C9EF0.unk0[i][j] = 0;
            D_800C9EF0.unk10[i][j] = 0;
        }
    }
    D_8006794C->unk1C->unk8E4.field->unk8 = 0;
    func_800AA4BC();
    D_8006794C->unk1C->unk8E4.field->unk9 = 0;
    D_8006794C->unk1C->unkC = arg0;
    sc->unkA = arg0;
    func_800A9BDC(sc, arg0);
    func_800A97D4(sc, arg0);
    func_800A9814(sc, arg0);
    func_800A9860(sc, arg0);
    func_800A9954(sc, arg0);
    func_800AA01C(&D_8006794C->unk1C->unk8E4.field->unkC->unk14->unk50, &D_8006794C->unk1C->unk8E4.field->unkC->unk14->unk52);
    func_800B3A80();
    func_800A9998();
    func_800B0124(0x900, -1, 0, 0, 0);
    func_80049ED8(sc->unkC);
    func_8004A06C(sc->unkC);
    func_800AC474();
    func_800A9AAC();
    while (func_800B0124(0xC00, -1, 0, 0, 0) != 0) {
    }
    func_8004BFB0();
    D_8006794C->unk1C->unk38 = func_8004AAF0(0);
    D_8006794C->unk1C->unk8DC = (Unk8D4 *)D_80067948;
    D_8006794C->unk1C->unk8E0 = (Unk8E0 *)(D_8006794C->unk1C->unk8DC + D_8006794C->unk1C->unk38);
    D_80067948 = (s32)(D_8006794C->unk1C->unk8E0 + D_8006794C->unk1C->unk38);
    D_8006794C->unk1C->unk9 = 0;
    for (i = 0; i < 9; i++) {
        func_8001E3A4(&D_8006794C->unk1C->unk8D8[i]);
        D_8006794C->unk1C->unk8D8[i].unk8 = &D_8006794C->unk1C->unk8D4[i];
    }
    D_8006794C->unk1C->unk39 = 0;
    for (i = 0; i < D_8006794C->unk1C->unk38; i++) {
        func_8001E3A4(&D_8006794C->unk1C->unk8E0[i]);
        D_8006794C->unk1C->unk8E0[i].unk8 = &D_8006794C->unk1C->unk8DC[i];
    }
    sc->unk2C = (FieldObjPlace *)D_80067948;
    D_80067948 = func_800AD44C(D_80067948, D_8006794C->unk1C->unk38 + 9,
                               D_8006794C->unk1C->unk8E4.field->unkC->unk14->unk50,
                               D_8006794C->unk1C->unk8E4.field->unkC->unk14->unk52);
    func_800B2528(sc, arg0);
    func_800B1C98();
    sc->unk20 = D_80067948;
    sc->unk4 = D_80067948;
    func_8004A8A4();
    D_80073998[0] |= 0x40000;
    D_8006794C->unk1C->unk0 &= ~0x20;
}

void func_800A8E58(void) {
    FieldScene *sc;
    s32 i;
    s32 id;

    sc = D_8006794C->unk1C->unk8E4.field->unkC->unk14;
    DrawSync(0);
    func_800B1E04(sc, D_8006794C->unk1C->unkC);
    D_80073998[0] &= 0xFFFF0006;
    func_800AA0A8(sc, D_8006794C->unk1C->unkC);
    func_800AA074(sc, D_8006794C->unk1C->unkC);
    func_800AAA54();
    for (i = 0; i < 9; i++) {
        if (D_8006794C->unk1C->unk8D8[i].unk0 & 1) {
            D_8006794C->unk1C->unk8D8[i].unk0 &= ~1;
        }
    }
    for (i = 0; i < D_8006794C->unk1C->unk38; i++) {
        if (D_8006794C->unk1C->unk8E0[i].unk0 & 1) {
            D_8006794C->unk1C->unk8E0[i].unk0 &= ~1;
        }
    }
    for (i = 1; i >= 0; i--) {
        D_8006794C->unk1C->unk7EC[i] = D_8006794C->unk1C->unk7FC[i];
        D_8006794C->unk1C->unk7F4[i] = D_8006794C->unk1C->unk804[i];
    }
    switch (D_8006794C->unk1C->unk8E4.field->unkC->unk14->unk4A) {
    case 2:
        func_800AA194(func_800AA0F4(sc->unkA, &D_8006794C->unk1C->unk7FC[0], &D_8006794C->unk1C->unk804[0]),
                      &D_8006794C->unk1C->unk7FC[1], &D_8006794C->unk1C->unk804[1]);
        D_8006794C->unk1C->unkE = sc->unkA;
        D_8006794C->unk8 = 2;
        D_8006794C->unkA = 1;
        break;
    case 3:
        id = func_800AA330(func_800AA290(sc->unkA, &D_8006794C->unk1C->unk7FC[1], &D_8006794C->unk1C->unk804[1]),
                           &D_8006794C->unk1C->unk7FC[0], &D_8006794C->unk1C->unk804[0]);
        D_8006794C->unk1C->unk7DC = sc->unkA;
        D_8006794C->unk1C->unk7DE = id;
        D_8006794C->unk8 = 3;
        D_8006794C->unkA = 1;
        break;
    case 9:
        if (D_8006794C->unk1C->unk0 & 0x20000000) {
            func_800B0124(0x800, D_8006794C->unk1C->unk81C[0], 0, 0, 0);
        }
        func_800B1C4C();
        func_80056228();
        D_8006794C->unk8 = 9;
        D_8006794C->unkA = 1;
        break;
    case 7:
        func_8005546C(-1, 0);
        func_80056228();
        D_8006794C->unk8 = 7;
        D_8006794C->unkA = 1;
        break;
    case 4:
        func_8005546C(-1, 0);
        func_80056228();
        D_8006794C->unk8 = 4;
        D_8006794C->unkA = 1;
        break;
    case 1:
        if (D_8006794C->unk1C->unk8E4.field->unk0 & 0x10000) {
            id = (s16)D_8006794C->unk1C->unk8E4.field->unkC->unk10;
        } else {
            id = func_800AA330(func_800AA290(sc->unkA, &D_8006794C->unk1C->unk7FC[1], &D_8006794C->unk1C->unk804[1]),
                               &D_8006794C->unk1C->unk7FC[0], &D_8006794C->unk1C->unk804[0]);
        }
        if (id != (D_8006794C->unk1C->unk3A | 0x60000)) {
            D_8006794C->unk0 |= 4;
        }
        D_8006794C->unk1C->unk8E4.field->unk0 &= ~0x10000;
        D_8006794C->unk1C->unk8E4.field->unkC->unk10 = id;
        D_8006794C->unk1C->unk8E4.field->unkA = D_8006794C->unk1C->unkC;
        D_8006794C->unk1C->unkC = sc->unkA;
        break;
    default:
        func_8002EB64(0x39, 0xC9);
        break;
    }
    D_8006794C->unk1C->unk7E4 = (struct FileTable *)-1;
}

/* allocate the field scene block and its tables */
void func_800A9350(void) {
    FieldScene *sc;

    D_8006794C->unk1C->unk8E4.field->unkC->unk14 = sc = (FieldScene *)D_80067948;
    sc->unk28 = sc->unk6C;
    D_80067948 += 0x90;
    sc->unk54 = (FieldActorInfo *)D_80067948;
    D_80067948 += 0x1E0;
    sc->unk58 = (FieldTrig *)D_80067948;
    D_80067948 += 0xC0;
}

void func_800A939C(void) {
    FieldScene *sc;
    s32 i;
    FieldActorInfo *info;
    u8 *base;
    FieldTrig *t;
    u8 *c;
    u8 *c2;

    sc = D_8006794C->unk1C->unk8E4.field->unkC->unk14;
    sc->unk8 = 0;
    sc->unk0 = 0;
    sc->unk4 = 0;
    sc->unkC = NULL;
    sc->unk4C = NULL;
    sc->unk14 = NULL;
    sc->unk18 = NULL;
    base = D_80067944 + 0x106700;
    sc->unk30 = 0;
    sc->unk10 = NULL;
    sc->unk2C = NULL;
    sc->unk20 = 0;
    sc->unk42 = 0;
    sc->unk43 = 0;
    sc->unk44 = 0;
    sc->unk48 = 0;
    sc->unk49 = 0;
    sc->unk1C = NULL;
    sc->unk34 = base;
    for (i = 23; i >= 0; i--) {
        info = &sc->unk54[i];
        info->unk0 = 0;
        info->unk4 = -1;
        info->unk5 = 0;
        info->unk7 = 0;
        info->unk6 = 0;
        c = info->unk8.c;
        c[2] = 0;
        c[1] = 0;
        c[0] = 0;
        info->unk8.c[3] = 0;
        c2 = info->unkC.c;
        c = c2;
        c[2] = 0;
        c[1] = 0;
        c[0] = 0;
        info->unkC.c[3] = 0;
        info->unk10 = NULL;
    }
    sc->unk38 = 0xA0;
    sc->unk3A = 0x70;
    sc->unk3C = 0xFFFF;
    sc->unk3E = 0xFFFF;
    sc->unk41 = 0;
    sc->unk9 = 0;
    sc->unk5C = 0x7F;
    sc->unk5D = 0x7F;
    sc->unk5F = 0;
    sc->unk52 = 0;
    sc->unk50 = 0;
    sc->unk24 = NULL;
    for (i = 0; i < 9; i++) {
        sc->unk28[i] = 0;
    }
    sc->unkA = -1;
    sc->unk40 = 0;
    sc->unk4A = 1;
    sc->unk4B = 0;
    for (i = 15; i >= 0; i--) {
        t = &sc->unk58[i];
        t->unk0 = 0xFF;
        t->unk1 = 0;
        t->unk2 = 0xFFFF;
        t->unk4 = 0xFFFF;
        t->unk6 = -1;
        t->unk8[1] = 0xFFFF;
        t->unk8[0] = 0xFFFF;
    }
    sc->unk5E = 0;
    sc->unk60 = 0;
    sc->unk62 = 0;
    sc->unk64 = 0;
    for (i = 0; i >= 0; i--) {
        sc->unk61[i] = 0;
    }
    for (i = 3; i >= 0; i--) {
        sc->unk68[i] = 0;
    }
}

void func_800A9570(void) {
    GameState *gs;
    GameState *gs2;
    GameData *gd;
    GameData *gd2;
    s32 key;
    FieldMap *f;
    GameStateUnk14 *g;
    s32 id;
    s32 t;
    u8 *base;
    FieldDirEnt *start;
    FieldDirEnt *e;

    gs = D_8006794C;
    gd = gs->unk1C;
    f = gd->unk8E4.field;
    id = -1;
    if (f->unk0 & 0x10000) {
        gs2 = gs;
        gd2 = gd;
        g = gs2->unk14;
        t = g->unk8;
        base = D_80067944 + ((t << 11) + 0x14C700);
        start = (FieldDirEnt *)(base + ((g->unk48 - t) << 11));
        e = &start[g->unk44] - 1;
        key = f->unkC->unk14->unkA;
        do {
            if (e->unk0 == key) {
                id = e->unk2;
                gd2->unk7FC[1] = e->unk4;
                gd2->unk804[1] = e[1].unk4 - e->unk4;
                break;
            }
            e--;
        } while (e >= start);
        start = (FieldDirEnt *)(base + ((gs2->unk14->unk28 - t) << 11));
        e = &start[gs2->unk14->unk24] - 1;
        do {
            if (e->unk0 == id) {
                gd2->unk7FC[0] = e->unk4;
                gd2->unk804[0] = e[1].unk4 - e->unk4;
                break;
            }
            e--;
        } while (e >= start);
        D_8006794C->unk1C->unk8E4.field->unkC->unk10 = id;
        D_8006794C->unk1C->unk8A8 = -1;
        D_8006794C->unk1C->unk84A = 0xFFFF;
        start = (FieldDirEnt *)(base + ((gs2->unk14->unk98 - t) << 11));
        e = &start[gs2->unk14->unk94] - 1;
        do {
            if (e->unk0 == D_8006794C->unk1C->unk81C[0]) {
                D_8006794C->unk1C->unk8A8 = e->unk4;
                D_8006794C->unk1C->unk84A = e[1].unk4 - e->unk4;
                break;
            }
            e--;
        } while (e >= start);
        func_800AC5F0();
        func_80022280((s32)D_8006794C->unk1C->unk854[0].unk8, D_8006794C->unk1C->unk854[0].unk10);
    }
}

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

s32 func_800A9860(FieldScene *arg0, s32 arg1) {
    GameData *gd = D_8006794C->unk1C;
    s32 id;

    func_800C0D40(&gd->unk8, &gd->unkA, gd->unk10[0], gd->unk10[1], &gd->unk18, D_80067948 + 0x400);
    func_800C0D8C(D_80067948, 0x400);
    id = func_800AA0C8(arg0, arg1);
    arg0->unk14 = func_8001C7FC(4, id, 0);
    func_800C0DA4(arg0->unk14);
    D_80067948 = func_800BFCE8();
    func_800C7430();
    arg0->unk18 = func_8001C7FC(4, (s32)func_8001C7FC(9, id, 0xB0000), 0);
    func_800C3B80((u8 *)arg0->unk18 + 4);
    D_80067948 = func_800AFCBC(D_80067948);
    return 0;
}

s32 func_800A9954(FieldScene *arg0) {
    arg0->unk1C = func_8001C7FC(4, (u16)func_800AA0C8(arg0) | 0x1F0000, 0);
    return 0;
}

void func_800A9998(void) {
    while (func_800B0124(0xC00, -1, 0, 0, 0) != 0) {
    }
    if (D_8006794C->unk1C->unk0 & 0x400000) {
        func_800B0124(0x100, D_8006794C->unk1C->unk81C[0], 0, 0, 0);
        D_8006794C->unk1C->unk0 &= ~0x400000;
    }
    if (D_8006794C->unk1C->unk8E4.field->unk20 != -1) {
        func_80055DF4();
        func_80055E14((s32)D_80067944 + 0x146F00, D_8006794C->unk1C->unk8E4.field->unk20, 1);
        D_8006794C->unk1C->unk8E4.field->unk20 = -1;
    }
}

s32 func_800A9AAC(void) {
    s32 best;
    s32 i;
    s32 min;
    s32 v;

    if (D_8006794C->unk1C->unk8E4.field->unkC->unk14->unk60 != 0) {
        while (1) {
            best = -1;
            min = 0x7FFFFFFF;
            for (i = D_8006794C->unk1C->unk8E4.field->unkC->unk14->unk60 - 1; i >= 0; i--) {
                if (D_800C9F20[i] != -1) {
                    v = func_8001E218(0xA, D_800C9F20[i])->unk4;
                    if (v < min) {
                        best = i;
                        min = v;
                    }
                }
            }
            if (best == -1) {
                break;
            }
            func_800AA4E8(D_800C9F20[best]);
            D_800C9F20[best] = -1;
        }
    }
    return 0;
}

s32 func_800A9BDC(FieldScene *sc, s32 arg1) {
    s32 size;
    s32 id;
    Unk80021938 *e;
    s32 base;
    s32 i;
    s32 r;
    s32 rest;
    s32 k;

    size = D_8006794C->unk1C->unk804[1] << 11;
    if ((s32)(0x801EDF00 - (D_80067948 + size)) < 0x17000) {
        size = 0x801D6F00 - D_80067948;
        size = size / 2048 * 2048;
    }
    id = (u16)arg1 | 0x50000;
    if (func_800220E8(id)) {
        e = func_80021E5C(id);
        D_8006794C->unk1C->unk854[0].unk20 = (u8 *)(e->unk8 + e->unkC);
        D_8006794C->unk1C->unk854[0].unk10 = e->unkC;
        D_8006794C->unk1C->unk854[0].unk4 = e->unk1C;
        func_8001DAA4(D_80067948, e->unk8, e->unkC, NULL);
        func_80022390(id);
        DrawSync(0);
    } else {
        func_8002177C(D_8006794C->unk1C->unk854, D_8006794C->unk1C->unk7FC[1], D_8006794C->unk1C->unk804[1] << 11,
                      (void *)D_80067948, 0, size, NULL);
        while (func_800217C4(D_8006794C->unk1C->unk854) != 0) {
        }
    }
    func_800AC67C((s32)D_8006794C->unk1C->unk854[0].unk20);
    base = D_80067948;
    D_8006794C->unk1C->unk7E4 = (FileTable *)func_8001C8B0(base, 4, 0x140000, 0);
    id = func_8001C8B0(base, 2, 0x1B0000, 0);
    if (id != 0) {
        r = func_8001C8B0(base, 5, 0x1B0000, 0);
        if ((u32)(D_80067948 + D_8006794C->unk1C->unk854[0].unk10) < (u32)(id + r)) {
            rest = D_8006794C->unk1C->unk854[0].unk10;
            func_8002177C(D_8006794C->unk1C->unk854, D_8006794C->unk1C->unk7FC[1], D_8006794C->unk1C->unk804[1] << 11,
                          (void *)(D_80067948 + rest), rest, size - D_8006794C->unk1C->unk854[0].unk10, NULL);
            while (func_800217C4(D_8006794C->unk1C->unk854) != 0) {
            }
            D_8006794C->unk1C->unk854[0].unk10 += rest;
            func_800AC67C((s32)D_8006794C->unk1C->unk854[0].unk20);
        }
        for (i = func_8001C8B0(id, 1, 0x40000, 0) - 1; i >= 0; i--) {
            k = func_8001C8B0(id, 2, (u16)i | 0x40000, 0);
            if (k != -1) {
                func_8001D8A8(k);
            }
        }
        for (i = func_8001C8B0(id, 1, 0x170000, 0) - 1; i >= 0; i--) {
            k = func_8001C8B0(id, 2, (u16)i | 0x170000, 0);
            if (k != -1) {
                func_800C9968((s32 *)k);
            }
        }
        DrawSync(0);
    }
    sc->unk24 = (void *)func_8001C8B0(base, 2, 0x1B0001, 0);
    size = D_8006794C->unk1C->unk854[0].unk10 - ((s32)sc->unk24 - base);
    func_8001DAA4(id, (s32)sc->unk24, size, NULL);
    DrawSync(0);
    sc->unk24 = (void *)id;
    if (D_8006794C->unk1C->unk854[0].unk10 < D_8006794C->unk1C->unk854[0].unk4) {
        func_8002177C(D_8006794C->unk1C->unk854, D_8006794C->unk1C->unk7FC[1], D_8006794C->unk1C->unk804[1] << 11,
                      (void *)(id + size), D_8006794C->unk1C->unk854[0].unk10, -1, NULL);
        while (func_800217C4(D_8006794C->unk1C->unk854) != 0) {
        }
    }
    func_8001C750((s32)sc->unk24, 0);
    D_80067948 = (s32)sc->unk24 + func_8001CA70((s32)sc->unk24);
    return 0;
}

s32 func_800AA01C(u16 *arg0, u16 *arg1) {
    Unk800AA01C *p = func_8001C7FC(4, 0x1C0000, 0);

    *arg0 = p->unk30;
    *arg1 = p->unk32;
    return 0;
}

s32 func_800AA074(FieldScene *arg0, s16 arg1) {
    func_8001C784(arg0->unk24);
    arg0->unk24 = NULL;
    return 0;
}

s32 func_800AA0A8(FieldScene *arg0, s16 arg1) {
    func_80054F80();
    return 0;
}

/* old-style definition: some callers pass a second argument */
s32 func_800AA0C8(arg0)
    FieldScene *arg0;
{
    return func_8001C8B0((s32)arg0->unk24, 3, 0xA0000, 0);
}

s32 func_800AA0F4(u16 arg0, s32 *arg1, s32 *arg2) {
    s32 i;
    FileTable *t = D_8006794C->unk1C->unk7E4;
    s32 id = arg0 | 0x110000;
    FileEntry *e = t->unk4;

    for (i = 0; i < D_8006794C->unk1C->unk7E4->unk2; i++) {
        if (e[i].unk0 == id) {
            if (arg1 != NULL) {
                *arg1 = e[i].unk4;
            }
            if (arg2 != NULL) {
                *arg2 = e[i].unk8;
            }
            break;
        }
    }
    return id;
}

s32 func_800AA194(s32 arg0, s32 *arg1, s32 *arg2) {
    s32 id = -1;
    FileTable *t = D_8006794C->unk1C->unk7E4;
    s32 i;
    FileEntry *e = t->unk4;

    for (i = 0; i < t->unk2; i++) {
        if (e[i].unk0 == arg0) {
            id = e[i].unkC;
            break;
        }
    }
    for (i = 0; i < D_8006794C->unk1C->unk7E4->unk2; i++) {
        if (e[i].unk0 == id) {
            if (arg1 != NULL) {
                *arg1 = e[i].unk4;
            }
            if (arg2 != NULL) {
                *arg2 = e[i].unk8;
            }
            break;
        }
    }
    return id;
}

s32 func_800AA290(u16 arg0, s32 *arg1, s32 *arg2) {
    s32 i;
    FileTable *t = D_8006794C->unk1C->unk7E4;
    s32 id = arg0 | 0x50000;
    FileEntry *e = t->unk4;

    for (i = 0; i < D_8006794C->unk1C->unk7E4->unk2; i++) {
        if (e[i].unk0 == id) {
            if (arg1 != NULL) {
                *arg1 = e[i].unk4;
            }
            if (arg2 != NULL) {
                *arg2 = e[i].unk8;
            }
            break;
        }
    }
    return id;
}

s32 func_800AA330(s32 arg0, s32 *arg1, s32 *arg2) {
    FileTable *t = D_8006794C->unk1C->unk7E4;
    FileEntry *e = t->unk4;
    FileEntry *end = e + t->unk2;
    s32 id = -1;

    for (; e < end; e++) {
        if (e->unk0 == arg0) {
            id = e->unkC;
            break;
        }
    }
    for (e = t->unk4; e < end; e++) {
        if (e->unk0 == id) {
            if (arg1 != NULL) {
                *arg1 = e->unk4;
            }
            if (arg2 != NULL) {
                *arg2 = e->unk8;
            }
            break;
        }
    }
    return id;
}

s32 func_800AA3F4(void) {
    s32 id;

    id = func_800AAEF8();
    if (id == -1) {
        return 0;
    }
    id = func_800AAEC4(id);
    if (func_8001C7FC(4, id, 0) == NULL) {
        D_800C9F20[D_8006794C->unk1C->unk8E4.field->unkC->unk14->unk60] = id;
        D_8006794C->unk1C->unk8E4.field->unkC->unk14->unk60++;
    }
    return 0;
}

void func_800AA4BC(void) {
    s32 i;

    for (i = 15; i >= 0; i--) {
        D_800C9F20[i] = -1;
    }
}

INCLUDE_ASM("asm/us/ovl_0a/nonmatchings/ovl_0a", func_800AA4E8);

s32 func_800AAA54(void) {
    s32 i;

    for (i = D_8006794C->unk1C->unk8E4.field->unkC->unk14->unk40 - 1; i >= 0; i--) {
        func_8001C784((void *)D_8006794C->unk1C->unk8E4.field->unkC->unk14->unk28[i]);
    }
    D_8006794C->unk1C->unk8E4.field->unkC->unk14->unk40 = 0;
    return 0;
}

s32 func_800AAB30(s32 arg0, void *arg1) {
    s32 *tex;
    s32 *uv;
    s32 *spr;
    s32 sprId;
    FieldObjSlot *s;
    s32 slot;
    u32 k;
    u32 sum;
    WmModel *m;
    WmModelGroup *g;
    Unk800AA39C *t;
    Unk800AA39CEntry *e;
    s32 i;
    u32 r;
    WmModel *a;

    sprId = -1;
    r = func_8004AB64(arg1);
    if (r < 9) {
        slot = r;
        s = (FieldObjSlot *)func_8001E350(r);
    } else {
        slot = D_8006794C->unk1C->unk39 + 9;
        s = (FieldObjSlot *)func_8001E350(slot);
        D_8006794C->unk1C->unk39++;
    }
    arg0 = func_800AAEC4(arg0);
    s->unk4 = arg0;
    s->unkC = arg1;
    s->unk8->unk8 = func_8001C7FC(4, arg0, 0);
    if ((s->unk8->unkC = func_8001C7FC(4, func_800AAF5C(arg0), 0)) != NULL) {
        a = s->unk8->unkC;
        if (a->unkC >= 0) {
            a->unkC = (s32)a + a->unkC;
            if (a->unk10 != NULL) {
                a->unk10 = (WmModelGroup *)((u8 *)s->unk8->unkC + (s32)a->unk10);
            }
        }
    }
    func_800AAFD4(arg0, &tex, &uv, &spr);
    m = s->unk8->unk8;
    g = m->unk10;
    sum = 0;
    if ((s32)g >= 0) {
        g = (WmModelGroup *)((u8 *)m + (s32)g);
    }
    for (k = 0; k < m->unk3; k++) {
        sum += g->unk0;
        g++;
    }
    sum *= 2;
    sum += s->unk8->unk8->unk2 * 32 + s->unk8->unk8->unk3 * 12;
    func_800B6438(s->unk8, (u8 *)D_8006794C->unk1C->unk8E4.field->unkC->unk14->unk20);
    func_800B6584(s->unk8, (WmTexInfo *)tex, (s16(*)[2])uv);
    D_8006794C->unk1C->unk8E4.field->unkC->unk14->unk20 += sum;
    func_800AC620();
    t = func_8001C7FC(8, arg0, 0x120000);
    e = t->unk4;
    for (i = t->unk1 - 1; i >= 0; i--) {
        if (e->unk0 == (u16)arg0) {
            sprId = e->unk8 | 0x190000;
        }
    }
    if (sprId != -1) {
        s->unk8->unk50 = func_8001C7FC(4, sprId, 0);
        func_800BB3C4((BgSprHdr *)s->unk8->unk50, (BgSprOfs *)spr);
    }
    if (D_8006794C->unk1C->unk8E4.field->unkC->unk14->unk18 != NULL) {
        if (slot < 9) {
            func_800C74A8(slot, s);
        } else {
            func_800C74F0(s);
        }
        func_800C3E18(slot, 1);
    }
    D_8006794C->unk1C->unk8E4.field->unkC->unk14->unk0 |= 0x10000;
    s->unk0 |= 1;
    return slot;
}

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

/* look up the three tables of an area entry */
s32 func_800AAFD4(s32 arg0, s32 **arg1, s32 **arg2, s32 **arg3) {
    Unk800AA39C *t = func_8001C7FC(8, arg0, 0x120000);
    Unk800AA39CEntry *e = t->unk4;
    s32 i;

    for (i = t->unk1 - 1; i >= 0; i--) {
        if (e->unk0 == (u16)arg0) {
            *arg1 = (s32 *)((u8 *)e + e->unk4);
            *arg2 = *arg1 + e->unk7;
            *arg3 = *arg2 + e->unk7;
            break;
        }
        e++;
    }
    return 0;
}

s32 func_800AB0B0(Unk800C95C8 *arg0) {
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

s32 func_800AB110(FieldObjSlot **arg0) {
    s32 n;
    Unk16D8 *e;
    FieldObjSlot *s;
    s32 i;
    WmModel *m;
    WmModelGroup *g;
    u32 sum;
    u32 k;
    Unk800C95C8 *o;
    Unk8004AB64 *a;
    u16 *p;

    n = 0;
    for (e = func_8004AE9C(); e != NULL; e = e->unk0) {
        a = e->unk4;
        if (a->unkE == 1 && (a->unkF & 1) && a->unk5 == 4) {
            s = (FieldObjSlot *)func_8001E350(a->unk3B);
            if (s->unk8->unk1C == NULL) {
                arg0[n++] = s;
            }
        }
    }
    for (e = func_8004AE9C(); e != NULL; e = e->unk0) {
        a = e->unk4;
        if (a->unkE == 1 && (a->unkF & 1) && a->unk5 == 4) {
            s = (FieldObjSlot *)func_8001E350(a->unk3B);
            p = s->unk8->unk1C;
            if (p != NULL) {
                for (i = 0; i < n; i++) {
                    if ((u16 *)arg0[i]->unk8 == p) {
                        arg0[n++] = s;
                        break;
                    }
                }
            }
        }
    }
    if (D_8006794C->unk1C->unk8E4.field->unkC->unk14->unk42 != 0) {
        for (i = 0; i < n; i++) {
            o = arg0[i]->unk8;
            m = o->unk8;
            g = m->unk10;
            sum = 0;
            if ((s32)g >= 0) {
                g = (WmModelGroup *)((u8 *)m + (s32)g);
            }
            for (k = 0; k < m->unk3; k++) {
                sum += g->unk0;
                g++;
            }
            if (o->unk10 + (sum * 2 + (o->unk8->unk2 * 32 + o->unk8->unk3 * 12)) >= D_80067944 + 0xF1700) {
                arg0[i] = NULL;
            }
        }
    }
    return n;
}

void func_800AB3CC(void) {
    /* block scope: func_800AC02C calls it undeclared (int argument) */
    void func_800B61A4(Unk800C95C8 *, u8);
    FieldEvtCtx *list[16];
    SVECTOR rot;
    DVECTOR sxy;
    s32 ofs[2];
    s16 ax;
    s16 az;
    s32 z;
    u32 *ot;
    s32 n;
    s32 i;
    s32 j;
    s32 j1;
    s32 cnt;
    s32 lim;
    s32 f;
    Unk800C95C8 *m;
    FieldEvtObj *o;
    FieldEvtCtx *c;
    WmModel *w;

    lim = 100;
    j = 0;
    if (D_800C9D44 & 0x40) {
        func_8019AE0C(&ofs[0], &ofs[1]);
        SetGeomOffset(ofs[0], ofs[1]);
    } else {
        func_800C0F08();
    }
    i = 0;
    ot = D_8006794C->unk1C->unk10[D_8006794C->unk1C->unk8];
    D_8006794C->unk1C->unk10[D_8006794C->unk1C->unk8] = ot + D_8006794C->unk1C->unk8E4.field->unkC->unk14->unk30;
    n = func_800AB110((FieldObjSlot **)list);
    for (; i < n; i++) {
        c = list[i];
        if (c != NULL) {
            m = c->unk8;
            o = c->unkC;
            if ((c->unk0 & 2) && func_800C768C(o->unk3B, &ax, &az)) {
                rot.vx = ax;
                rot.vy = o->unk22;
                rot.vz = az;
                func_80030EA4(&rot, &m->unk24);
            } else {
                rot.vx = o->unk20;
                rot.vy = (o->unk22 + j) & 0xFFF;
                rot.vz = o->unk24;
                func_80031244(&rot, &m->unk24);
            }
            m->unk24.t[0] = o->unk14.vx;
            m->unk24.t[1] = o->unk14.vy;
            m->unk24.t[2] = o->unk14.vz;
            func_800B7CEC(m);
        }
    }
    for (i = 0; i < n; i++) {
        c = list[i];
        if (c != NULL) {
            o = c->unkC;
            m = c->unk8;
            w = func_8001C7FC(4, o->unk36 | 0x30000, 0);
            m->unkC = w;
            if (w != NULL && (s32)w->unkC >= 0) {
                w->unkC = (void *)((u8 *)w + (s32)w->unkC);
                if (w->unk10 != NULL) {
                    w->unk10 = (void *)((u8 *)m->unkC + (s32)w->unk10);
                }
            }
        }
    }
    for (i = 0; i < n; i++) {
        c = list[i];
        if (c != NULL) {
            m = c->unk8;
            o = c->unkC;
            if (m->unk0 & 1) {
                func_800BA560(m, o->unk3A);
            } else {
                func_800B7F14(m, o->unk3A);
            }
        }
    }
    gte_SetRotMatrix(&D_8006794C->unk1C->unk18);
    gte_SetTransMatrix(&D_8006794C->unk1C->unk18);
    for (i = 0; i < n; i++) {
        c = list[i];
        if (c != NULL) {
            m = c->unk8;
            rot.vx = ((Unk8004AB64 *)m->unk20)->unk14.vx;
            rot.vy = ((Unk8004AB64 *)m->unk20)->unk14.vy;
            rot.vz = ((Unk8004AB64 *)m->unk20)->unk14.vz;
            gte_ldv0(&rot);
            gte_rtps();
            gte_stsxy(&sxy);
            gte_stsz(&z);
            z = z / 4 + D_8006794C->unk1C->unk8E4.field->unkC->unk14->unk30;
            if (z < lim || z > 0x1000 - lim) {
                list[i] = NULL;
            }
        }
    }
    for (i = 0; i < n; i++) {
        c = list[i];
        if (c != NULL) {
            m = c->unk8;
            f = c->unk0 & 0x100000;
            lim = m->unk8->unk3;
            SetSpadStack12(0x1F8003FC);
            if (f) {
                for (j = 0; j < lim; j++) {
                    f = 1 << j;
                    if (!(m->unk14 & (u32)f)) {
                        func_800BB508(m, j);
                        func_800BB714(m, j);
                    }
                }
            } else {
                cnt = lim;
                for (j1 = 0; j1 < cnt; j1++) {
                    if (!(m->unk14 & (u32)(1 << j1))) {
                        func_800B61A4(m, j1);
                        func_800B5958(m, j1);
                    }
                }
            }
            ResetSpadStack12();
        }
    }
    for (i = 0; i < n; i++) {
        c = list[i];
        if (c != NULL) {
            func_800AC6C0(D_8006794C->unk1C->unk8E4.field->unkC->unk14->unk2C, c->unkC->unk3B);
        }
    }
    for (i = 0; i < n; i++) {
        c = list[i];
        if (c != NULL && !(c->unk0 & 0x1000)) {
            m = c->unk8;
            if (m->unk50->unk0 >= 2 && func_800AB0B0(m) == 0) {
                m->unk50->unk4[2].unk0 |= 1;
                m->unk50->unk4[2].unk8 = 0;
                m->unk50->unk4[2].unkE = 0x1000;
            }
        }
    }
    for (i = 0; i < n; i++) {
        c = list[i];
        if (c != NULL) {
            func_800BB090((BgAnimHdr *)((Unk800C95C8 *)c->unk8)->unk50);
        }
    }
    func_800B2D10(list, n);
    if (!(D_8006794C->unk1C->unk0 & 2)) {
        func_800B1E8C(list, n);
    }
    func_800AC02C(list, n);
    D_8006794C->unk1C->unk10[D_8006794C->unk1C->unk8] = ot;
    if (!(D_800C9D44 & 0x40)) {
        func_800C0FB0();
    }
}

/* object command arg1 for object arg0 (palette, mirror, triggers) */
s32 func_800ABA98(s32 arg0, s32 arg1, s32 arg2, s32 arg3, s32 arg4) {
    FieldActorInfo *info;
    FieldEvtCtx *c;
    FieldActorExt *x;
    FieldActorExt *y;
    s32 *tex;
    s32 *uv;
    s32 *spr;
    u32 k;
    u32 sum;
    WmModel *m;
    WmModelGroup *g;
    s32 id;
    FieldTrig *t;

    info = &D_8006794C->unk1C->unk8E4.field->unkC->unk14->unk54[arg0];
    c = (FieldEvtCtx *)func_8001E350(arg0);
    switch (arg1) {
    case 0:
        if ((u8)arg2 == 0xFF) {
            c->unk0 &= ~0x10000;
        } else {
            if (arg2 != (s8)info->unk5) {
                c->unk0 |= 0x40000;
            }
            c->unk0 |= 0x10000;
        }
        info->unk5 = arg2;
        break;
    case 4:
        if (arg2 != 0) {
            ((Unk800C95C8 *)c->unk8)->unk5A = arg3;
            c->unk0 |= 0x100000;
        } else {
            c->unk0 &= ~0x100000;
        }
        break;
    case 8:
    case 9:
    case 10:
    case 11:
        x = info->unk10;
        if (x == NULL) {
            x = info->unk10 = (FieldActorExt *)D_8006794C->unk1C->unk8E4.field->unkC->unk14->unk20;
            x->unk0 = 0;
            x->unk4 = c->unk4;
            x->unk8 = (Unk800C95C8 *)x->unk14;
            x->unkC = (FieldEvtObj *)x->unkAC;
            x->unk10 = 0;
            x->unkF8 = c;
            x->unkFC.v.vx = 0;
            x->unkFC.v.vy = 0;
            x->unkFC.v.vz = 0;
            x->unk104.v.vx = 0;
            x->unk104.v.vy = 0x1000;
            x->unk104.v.vz = 0;
            x->unk10C.w = 0;
            D_8006794C->unk1C->unk8E4.field->unkC->unk14->unk20 += sizeof(FieldActorExt);
        }
        y = x;
        if (!(x->unk0 & 1)) {
            *y->unkC = *c->unkC;
            y->unk8->unk8 = ((Unk800C95C8 *)c->unk8)->unk8;
            y->unk8->unkC = ((Unk800C95C8 *)c->unk8)->unkC;
            func_800AAFD4(y->unk4, &tex, &uv, &spr);
            m = y->unk8->unk8;
            g = m->unk10;
            sum = 0;
            if ((s32)g >= 0) {
                g = (WmModelGroup *)((u8 *)m + (s32)g);
            }
            for (k = 0; k < m->unk3; k++) {
                sum += g->unk0;
                g++;
            }
            sum *= 2;
            sum += y->unk8->unk8->unk2 * 32 + y->unk8->unk8->unk3 * 12;
            func_800B6438(y->unk8, (u8 *)D_8006794C->unk1C->unk8E4.field->unkC->unk14->unk20);
            func_800B6584(y->unk8, (WmTexInfo *)tex, (s16(*)[2])uv);
            D_8006794C->unk1C->unk8E4.field->unkC->unk14->unk20 += sum;
            id = (s32)func_8001C7FC(9, y->unk4, 0x190000);
            if (id != -1) {
                y->unk8->unk50 = func_8001C7FC(4, id, 0);
                func_800BB3C4((BgSprHdr *)y->unk8->unk50, (BgSprOfs *)spr);
            }
            func_800AC620();
            x->unk0 |= 0x02000001;
        }
        switch (arg1) {
        case 8:
            if (arg2 != 0) {
                y->unk8->unk60 = (Unk800C95C8 *)c->unk8;
                y->unk8->unk64.w[0] = x->unk104.w[0];
                y->unk8->unk64.w[1] = x->unk104.w[1];
                y->unk8->unk6C.w[0] = x->unkFC.w[0];
                y->unk8->unk6C.w[1] = x->unkFC.w[1];
                x->unk0 |= 0x01000000;
            } else {
                x->unk0 &= ~0x01000000;
            }
            break;
        case 9:
            x->unkFC.v.vx = arg2;
            x->unkFC.v.vy = arg3;
            x->unkFC.v.vz = arg4;
            break;
        case 10:
            x->unk104.v.vx = arg2;
            x->unk104.v.vy = arg3;
            x->unk104.v.vz = arg4;
            break;
        case 11:
            x->unk10C.c[0] = arg2;
            x->unk10C.c[1] = arg3;
            x->unk10C.c[2] = arg4;
            x->unk10C.c[3] = 2;
            break;
        }
        break;
    case 16:
    case 17:
    case 18:
    case 19:
        t = func_800B2100(c, arg2, arg3);
        if (t == NULL && arg1 != 19) {
            t = func_800B21A0(c, arg2, arg3);
        }
        switch (arg1) {
        case 16:
            t->unk8[0] = arg4;
            break;
        case 17:
            t->unk8[1] = arg4;
            break;
        case 18:
            t->unk1 = arg4 != 0;
            break;
        case 19:
            func_800B2218(c, arg2, arg3);
            break;
        }
        break;
    }
    return 0;
}

void func_800AC02C(FieldEvtCtx **arg0, s32 arg1) {
    s32 i;
    s32 j;
    FieldEvtCtx *c;
    FieldActorInfo *info;
    FieldActorExt *x;
    u32 vis;
    u32 chg;
    u32 f4;
    u32 f2;
    s32 upd;
    s32 n;
    FieldActorExt *y;

    for (i = arg1 - 1; i >= 0; i--) {
        c = arg0[i];
        if (c == NULL) {
            continue;
        }
        upd = 0;
        info = &D_8006794C->unk1C->unk8E4.field->unkC->unk14->unk54[c->unkC->unk3B];
        vis = c->unk0 & 0x10000;
        chg = (info->unk0 & 0x10000) ^ vis;
        f4 = c->unk0 & 0x40000;
        f2 = c->unk0 & 0x20000;
        if (chg || f4 || f2) {
            upd = 1;
        }
        if (upd) {
            if (c->unk0 & 0x10000) {
                func_800BA190(c->unk8, 1);
            } else {
                func_800BA190(c->unk8, 0);
            }
            if (vis) {
                func_800BA3CC(c->unk8, ((s32)(info->unk5 << 24) >> 19) & 0xFFE0);
            }
            if (c->unk0 & 0x20000) {
                c->unk0 &= ~0x20000;
            }
            c->unk0 &= ~0x40000;
            if (chg || f4) {
                c->unk0 |= 0x20000;
            }
            info->unk0 = (info->unk0 & ~0x10000) | vis;
        }
    }
    for (i = arg1 - 1; i >= 0; i--) {
        c = arg0[i];
        if (c == NULL) {
            continue;
        }
        info = &D_8006794C->unk1C->unk8E4.field->unkC->unk14->unk54[c->unkC->unk3B];
        x = info->unk10;
        if (x != NULL && (x->unk0 & 0x01000000)) {
            func_800BC33C(x->unk8);
            y = x;
            func_800B7F14(x->unk8, x->unkF8->unkC->unk3A);
            n = x->unk8->unk8->unk3;
            for (j = 0; j < n; j++) {
                func_800B61A4(y->unk8, j & 0xFF);
                func_800BC600(y->unk8, j & 0xFF);
            }
            if (x->unk10C.c[3] != 0) {
                for (j = n - 1; j >= 0; j--) {
                    func_800B9E4C(y->unk8, j & 0xFF, x->unk10C.c[0], x->unk10C.c[1], x->unk10C.c[2]);
                }
                x->unk10C.c[3]--;
            }
        }
    }
}

Unk800C9CCC *func_800AC344(s32 arg0) {
    u32 id;
    s32 i;

    id = (u16)arg0;
    for (i = 9; i >= 0; i--) {
        if (D_800C9CCC[i].unk2 == id) {
            return &D_800C9CCC[i];
        }
    }
    return NULL;
}

u32 func_800AC380(Unk800C9CCC **arg0) {
    u32 min;
    s32 i;

    min = 0x801EDF00;
    if (arg0 != NULL) {
        *arg0 = NULL;
    }
    for (i = 9; i >= 0; i--) {
        if (D_800C9CCC[i].unk4 != 0 && D_800C9CCC[i].unk4 < min) {
            min = D_800C9CCC[i].unk4;
            if (arg0 != NULL) {
                *arg0 = &D_800C9CCC[i];
            }
        }
    }
    return min;
}

s32 func_800AC3D8(s32 arg0, s32 arg1) {
    Unk800C9CCC *e;
    u32 lo;
    u32 min;
    u32 a;

    e = func_800AC344(arg0);
    min = func_800AC380(NULL);
    lo = D_8006794C->unk1C->unk8E4.field->unkC->unk14->unk20;
    if (lo == 0) {
        lo = D_80067948;
    }
    a = min - arg1;
    if (lo < a) {
        e->unk4 = a;
        e->unk8 = arg1;
        return 1;
    }
    return 0;
}

/* drop the cached textures no longer listed and pack the rest at the top of RAM */
void func_800AC474(void) {
    u32 top;
    Unk800C9CCC *s;
    s32 i;
    s32 j;
    s32 found;
    s32 bi;
    u32 end;
    u32 best;
    u32 addr;
    s32 id;
    u32 sz;
    Unk800C9CCC *t;

    top = 0x801EDF00;
    for (i = 9; i >= 0; i--) {
        s = &D_800C9CCC[i];
        id = s->unk2;
        if (s->unk4 != 0) {
            found = 0;
            for (j = D_8006794C->unk1C->unk8E4.field->unkC->unk14->unk60 - 1; j >= 0; j--) {
                if ((u16)D_800C9F20[j] == id) {
                    found = 1;
                    break;
                }
            }
            if (!found) {
                s->unk4 = 0;
                s->unk8 = 0;
            }
        }
    }
    for (;;) {
        best = 0;
        bi = -1;
        for (i = 9; i >= 0; i--) {
            t = &D_800C9CCC[i];
            sz = t->unk8;
            end = t->unk4 + sz;
            if (t->unk4 != 0 && end <= top && best < end) {
                best = end;
                bi = i;
            }
        }
        if (bi == -1) {
            break;
        }
        s = &D_800C9CCC[bi];
        addr = top - s->unk8;
        if (s->unk4 != addr) {
            func_8001DAA4(addr, s->unk4, s->unk8, NULL);
            s->unk4 = addr;
        }
        top = addr;
    }
    DrawSync(0);
}

void func_800AC5F0(void) {
    s32 i;

    for (i = 9; i >= 0; i--) {
        D_800C9CCC[i].unk0 = 0;
        D_800C9CCC[i].unk4 = 0;
        D_800C9CCC[i].unk8 = 0;
    }
}

void func_800AC620(void) {
    s32 v = D_8006794C->unk1C->unk8E4.field->unkC->unk14->unk20;

    if (v == 0) {
        v = D_80067948;
    }
    func_800AC67C(v);
}

void func_800AC67C(s32 arg0) {
    s32 i;

    for (i = 9; i >= 0; i--) {
        if (D_800C9CCC[i].unk4 != 0 && (u32)arg0 >= D_800C9CCC[i].unk4) {
            D_800C9CCC[i].unk8 = 0;
            D_800C9CCC[i].unk4 = 0;
        }
    }
}

void func_800AC6C0(FieldObjPlace *arg0, s32 arg1) {
    MATRIX m;
    POLY_FT4 *p[4];
    SVECTOR v;
    s32 z[9];
    s32 bias;
    FieldObjPlace *e;
    FieldObjSlot *s;
    void *lt;
    Unk800B3294Bias *t;
    u32 *mask;
    u32 *mk;
    s32 i;
    s32 k;
    s32 u0;
    s32 u1;
    s32 v0;
    s32 v1;

    e = &arg0[arg1];
    s = (FieldObjSlot *)func_8001E350(arg1);
    bias = 0;
    if (s->unk0 & 0x10) {
        return;
    }
    lt = D_8006794C->unk1C->unk8E4.field->unkC->unk14->unk1C;
    if (lt != NULL) {
        t = (Unk800B3294Bias *)func_800B3294(lt, s->unk4);
        if (t == NULL) {
            t = (Unk800B3294Bias *)func_800B3294(lt, -1);
        }
        bias = t->unk7;
    }
    func_800AD488(&e->unk0, s, &m);
    u0 = (s16)(D_8006794C->unk1C->unk8E4.field->unkC->unk14->unk50 % 64) * 2;
    v0 = (s16)(D_8006794C->unk1C->unk8E4.field->unkC->unk14->unk52 % 256);
    for (i = 0; i < 4; i++) {
        mask = NULL;
        if (D_8006794C->unk1C->unk8E4.field->unk0 & 0x200) {
            mask = e->unk8[D_8006794C->unk1C->unk8][i];
            setlen(mask, 2);
            mask[1] = 0xE6000002;
            mask[2] = 0;
            p[i] = (POLY_FT4 *)(mask + 3);
        } else {
            p[i] = e->unk8[D_8006794C->unk1C->unk8][i];
        }
        setlen(p[i], 9);
        setcode(p[i], 0x2C);
        setSemiTrans(p[i], 1);
        setRGB0(p[i], e->unk0.unk0, e->unk0.unk0, e->unk0.unk0);
        p[i]->tpage = getTPage(1, 2, D_8006794C->unk1C->unk8E4.field->unkC->unk14->unk50,
                               D_8006794C->unk1C->unk8E4.field->unkC->unk14->unk52);
        p[i]->clut = 0x3FD0;
        switch (i) {
        case 0:
            setUV4(p[0], u0, v0, u0 + 31, v0, u0, v0 + 31, u0 + 31, v0 + 31);
            break;
        case 1:
            setUV4(p[1], u0 + 31, v0, u0, v0, u0 + 31, v0 + 31, u0, v0 + 31);
            break;
        case 2:
            setUV4(p[2], u0, v0 + 31, u0 + 31, v0 + 31, u0, v0, u0 + 31, v0);
            break;
        case 3:
            setUV4(p[3], u0 + 31, v0 + 31, u0, v0 + 31, u0 + 31, v0, u0, v0);
            break;
        }
        if (D_8006794C->unk1C->unk8E4.field->unk0 & 0x200) {
            mk = (u32 *)(p[i] + 1);
            setlen(mk, 2);
            mk[1] = 0xE6000002;
            mk[2] = 0;
            MargePrim(p[i], mk);
            MargePrim(mask, p[i]);
        }
    }
    gte_MulMatrix0(&D_8006794C->unk1C->unk18, &m, &m);
    gte_SetTransMatrix(&D_8006794C->unk1C->unk18);
    gte_ldlv0(m.t);
    gte_rtv0tr();
    gte_stir(m.t);
    gte_SetRotMatrix(&m);
    gte_SetTransMatrix(&m);
    FIELD_SHADOW_PT(-0x1000, 0x1000);
    gte_stsxy(&p[0]->x0);
    gte_stsz(&z[0]);
    FIELD_SHADOW_PT(0, 0x1000);
    gte_stsxy(&p[0]->x1);
    gte_stsxy(&p[1]->x0);
    gte_stsz(&z[1]);
    FIELD_SHADOW_PT(0x1000, 0x1000);
    gte_stsxy(&p[1]->x1);
    gte_stsz(&z[2]);
    FIELD_SHADOW_PT(-0x1000, 0);
    gte_stsxy(&p[0]->x2);
    gte_stsxy(&p[2]->x0);
    gte_stsz(&z[3]);
    FIELD_SHADOW_PT(0, 0);
    gte_stsxy(&p[0]->x3);
    gte_stsxy(&p[1]->x2);
    gte_stsxy(&p[2]->x1);
    gte_stsxy(&p[3]->x0);
    gte_stsz(&z[4]);
    FIELD_SHADOW_PT(0x1000, 0);
    gte_stsxy(&p[1]->x3);
    gte_stsxy(&p[3]->x1);
    gte_stsz(&z[5]);
    FIELD_SHADOW_PT(-0x1000, -0x1000);
    gte_stsxy(&p[2]->x2);
    gte_stsz(&z[6]);
    FIELD_SHADOW_PT(0, -0x1000);
    gte_stsxy(&p[2]->x3);
    gte_stsxy(&p[3]->x2);
    gte_stsz(&z[7]);
    FIELD_SHADOW_PT(0x1000, -0x1000);
    gte_stsxy(&p[3]->x3);
    gte_stsz(&z[8]);

    k = z[0];
    if (k < z[1]) {
        k = z[1];
    }
    if (k < z[3]) {
        k = z[3];
    }
    if (k < z[4]) {
        k = z[4];
    }
    FIELD_SHADOW_ADD(k, 0);
    k = z[1];
    if (k < z[2]) {
        k = z[2];
    }
    if (k < z[4]) {
        k = z[4];
    }
    if (k < z[5]) {
        k = z[5];
    }
    FIELD_SHADOW_ADD(k, 1);
    k = z[3];
    if (k < z[6]) {
        k = z[6];
    }
    if (k < z[7]) {
        k = z[7];
    }
    FIELD_SHADOW_ADD(k, 2);
    k = z[4];
    if (k < z[5]) {
        k = z[5];
    }
    if (k < z[7]) {
        k = z[7];
    }
    if (k < z[8]) {
        k = z[8];
    }
    FIELD_SHADOW_ADD(k, 3);
}

void func_800AD44C(s32 arg0, s32 arg1, s32 arg2, s32 arg3) {
    func_80062E3C(arg0, 0x100, 0xFF, arg2, arg3, arg1, 3);
}

/* object placement matrix: either a plain y rotation or facing the model's joint */
void func_800AD488(FieldObjEnt *arg0, FieldObjSlot *arg1, MATRIX *arg2) {
    MATRIX m;
    SVECTOR v;
    VECTOR dir;
    VECTOR side;
    VECTOR up;
    VECTOR fwd;
    s32 *d;
    s32 *s;
    s32 c;
    s32 sn;

    if (arg1->unk0 & 0x20) {
        c = func_8004BB7C(arg0->unk4);
        sn = func_8004BB80(arg0->unk4);
        arg2->m[0][0] = c * arg0->unk1 >> 12;
        arg2->m[1][0] = 0;
        arg2->m[2][0] = -sn * arg0->unk1 >> 12;
        arg2->m[0][1] = 0;
        arg2->m[1][1] = 0x1000;
        arg2->m[2][1] = 0;
        arg2->m[0][2] = sn * arg0->unk2 >> 12;
        arg2->m[1][2] = 0;
        arg2->m[2][2] = c * arg0->unk2 >> 12;
        arg2->t[0] = ((c * arg0->unk4_12 + sn * arg0->unk4_22) >> 12) + ((Unk8004AB64 *)arg1->unkC)->unk14.vx;
        arg2->t[1] = ((Unk8004AB64 *)arg1->unkC)->unk14.vy;
        arg2->t[2] = ((-sn * arg0->unk4_12 + c * arg0->unk4_22) >> 12) + ((Unk8004AB64 *)arg1->unkC)->unk14.vz;
    } else {
        /* word-wise copy of the joint matrix */
        d = (s32 *)&m;
        s = (s32 *)&((MATRIX *)arg1->unk8->unk20)[1];
        d[0] = s[0];
        d[1] = s[1];
        d[2] = s[2];
        d[3] = s[3];
        d[4] = s[4];
        d[5] = s[5];
        d[6] = s[6];
        d[7] = s[7];
        m.t[2] = 0;
        m.t[1] = 0;
        m.t[0] = 0;
        v.vx = 0;
        v.vy = -0x1000;
        v.vz = 0;
        gte_SetRotMatrix(&m);
        gte_SetTransMatrix(&m);
        gte_ldv0(&v);
        gte_rtv0tr();
        gte_stlvnl(&dir);
        dir.vy = 0;
        VectorNormal(&dir, &fwd);
        up.vy = 0x1000;
        up.vx = 0;
        up.vz = 0;
        fwd.vx = -fwd.vx;
        fwd.vy = -fwd.vy;
        fwd.vz = -fwd.vz;
        gte_ldopv1(&up);
        gte_ldopv2(&fwd);
        gte_op12();
        gte_stlvnl(&side);
        arg2->m[0][0] = side.vx * arg0->unk1 >> 12;
        arg2->m[1][0] = side.vy * arg0->unk1 >> 12;
        arg2->m[2][0] = side.vz * arg0->unk1 >> 12;
        arg2->m[0][1] = up.vx;
        arg2->m[1][1] = up.vy;
        arg2->m[2][1] = up.vz;
        arg2->m[0][2] = fwd.vx * arg0->unk2 >> 12;
        arg2->m[1][2] = fwd.vy * arg0->unk2 >> 12;
        arg2->m[2][2] = fwd.vz * arg0->unk2 >> 12;
        arg2->t[0] = ((side.vx * arg0->unk4_12 + fwd.vx * arg0->unk4_22) >> 12) + ((MATRIX *)arg1->unk8->unk20)[1].t[0];
        arg2->t[1] = ((side.vy * arg0->unk4_12 + fwd.vy * arg0->unk4_22) >> 12) + ((Unk8004AB64 *)arg1->unkC)->unk14.vy;
        arg2->t[2] = ((side.vz * arg0->unk4_12 + fwd.vz * arg0->unk4_22) >> 12) + ((MATRIX *)arg1->unk8->unk20)[1].t[2];
    }
}

/* field request dispatcher */
s32 func_800AD944(s32 arg0, s32 arg1, s32 arg2) {
    switch (arg0 & 0xF) {
    case 0:
        func_800ADA7C((arg1 >> 8) & 0xFF, arg1 & 0xFF, arg2);
        return 0;
    case 1:
        func_800AE4F8();
        return 0;
    case 2:
        D_800C9D44 |= 1;
        break;
    case 3:
        return func_800AD944_state();
    case 4:
        return func_8019AB70();
    case 5:
        D_8006794C->unk1C->unk8E4.field->unkC->unk14->unk43 = arg1;
        func_8019A5B0(arg1);
        break;
    }
    return 0;
}

void func_800ADA7C(s8 arg0, s8 arg1, s8 arg2) {
    D_800C9D44 = 0;
    D_8006794C->unk1C->unk8E4.field->unkC->unk14->unk48 = arg0;
    D_8006794C->unk1C->unk8E4.field->unkC->unk14->unk49 = arg1;
    D_8006794C->unk1C->unk8E4.field->unkC->unk14->unk43 = arg2;
    func_800AC5F0();
    func_800223CC();
    D_8006794C->unk1C->unk8E4.field->unk0 |= 0x1000;
    D_8006794C->unk1C->unk8E4.field->unkC->unk14->unk42 = 1;
}

void func_800ADB74(void) {
    s32 b;
    s32 done;
    s32 t;
    s32 d;

    switch (D_8006794C->unk1C->unk8E4.field->unkC->unk14->unk42) {
    case 1:
        if (func_80022B18() == 0) {
            if (func_800217C4(D_8006794C->unk1C->unk854) == 0) {
                FileEntry *f = func_8001E218(0, 0x64);
                func_8002177C(D_8006794C->unk1C->unk854, f->unk4, (f->unkC - f->unk4) << 11,
                              (void *)(D_80067944 + 0xF1700), 0, 0x15000, NULL);
            }
            D_8006794C->unk1C->unk8E4.field->unkC->unk14->unk42 = 2;
        }
        break;
    case 2:
        if (func_800217C4(D_8006794C->unk1C->unk854) == 0) {
            D_800C9D44 |= 8;
            D_8006794C->unk1C->unk8E4.field->unkC->unk14->unk42 = 3;
        }
        break;
    case 3:
        if (!(D_800C9D44 & 8)) {
            D_8006794C->unk1C->unk8E4.field->unkC->unk14->unk44 = func_80022AC4(
                D_8019DC5C[D_8006794C->unk1C->unk8E4.field->unkC->unk14->unk48]
                    .unk4[D_8006794C->unk1C->unk8E4.field->unkC->unk14->unk49].unk0);
            if (func_80022B18() == 0) {
                func_80022ED4(D_8006794C->unk1C->unk8E4.field->unkC->unk14->unk44);
                D_8006794C->unk1C->unk8E4.field->unkC->unk14->unk42 = 4;
            }
        }
        break;
    case 4:
        if (func_80022B18() == 0) {
            D_8006794C->unk1C->unk8E4.field->unkC->unk14->unk42 = 5;
        }
        break;
    case 5:
        if (D_800C9D44 & 1) {
            b = 1;
            if (D_8006794C->unk1C->unk8 != 0) {
                b = 2;
            }
            D_800C9EF0.unk10[0][b - 1] = 0x600;
            D_800C9EF0.unk0[0][b - 1] = 0x19;
            D_800C9EF0.unk10[0][b - 1] |= 0x800;
            if (D_8006794C->unk1C->unk8E4.field->unkC->unk14->unk43 != 0) {
                D_800C9EF0.unk0[0][b - 1] |= 0x44;
                D_800C9EF0.unk10[0][b - 1] |= 1;
            }
            D_800C9EF0.unk20[0][b - 1] = 0x40;
            D_800C9EF0.unk20[1][b] = 0x40;
            D_800C9EF0.unk20[0][b] = 2;
            if (!(D_80073998[0] & 0x04000000)) {
                D_8006794C->unk1C->unk8E4.field->unkC->unk14->unk0 |= 0x1000;
                D_80073998[0] |= 0x04000000;
            }
            D_8006794C->unk9 = 4;
            func_80056248(0);
            func_80056228();
            DrawSync(0);
            func_801999B4(1, &D_8006794C->unk1C->unk8E4.field->unkC->unk14->unk34);
            func_8019A5B0(D_8006794C->unk1C->unk8E4.field->unkC->unk14->unk43);
            func_8019AC3C(func_800AE4F8);
            D_800C9D46 = D_80071E34;
            func_8019AB98(2, 4, D_8006794C->unk1C->unk8E4.field->unkC->unk14->unk5D);
            func_80199B28(D_8006794C->unk1C->unk8E4.field->unkC->unk14->unk48,
                          D_8006794C->unk1C->unk8E4.field->unkC->unk14->unk49);
            D_8006794C->unk1C->unk8E4.field->unkC->unk14->unk42 = 6;
        }
        break;
    case 6:
        if (D_800C9D44 & 0x40) {
            func_8019AD4C((void *)&D_8006794C->unk1C->unk18, &D_8006794C->unk1C->unkA);
        }
        if ((D_800C9D44 & 2) && (func_80012AD4() & 2)) {
            done = func_80199F30(0);
            t = func_8019AD0C();
            if (!(D_800C9D44 & 0x20) && (u32)t < 99999) {
                d = t - D_80071E34;
                if (d < 4) {
                    func_80056278(d * 4, 0);
                    D_800C9D44 |= 0x20;
                }
            }
            if (done != 0) {
                setlen(&D_800C9F6C[D_8006794C->unk1C->unk8 ^ 1], 2);
                D_800C9F6C[D_8006794C->unk1C->unk8 ^ 1].code[0] = 0xE6000002;
                D_800C9F6C[D_8006794C->unk1C->unk8 ^ 1].code[1] = 0;
                addPrim(D_8006794C->unk1C->unk10[D_8006794C->unk1C->unk8 ^ 1],
                        &D_800C9F6C[D_8006794C->unk1C->unk8 ^ 1]);
                DrawOTag(D_8006794C->unk1C->unk10[D_8006794C->unk1C->unk8 ^ 1]);
            } else if (D_8006794C->unk1C->unk8E4.field->unkC->unk14->unk43 != 0 && D_8006794C->unk1C->unk8 == 0) {
                D_8006794C->unkC[D_8006794C->unk1C->unk8].disp.x = 0x1E0;
            }
            if ((D_80071E34 >= t || D_800C9D46 >= D_80071E34) && !(D_800C9D44 & 0x100)) {
                if (D_800C9D46 >= D_80071E34) {
                    ClearOTagR(D_8006794C->unk1C->unk10[D_8006794C->unk1C->unk8], 0x1000);
                    D_8006794C->unk1C->unk0 &= ~0x1D;
                    D_8006794C->unk1C->unk8E4.field->unk0 &= ~0xE01;
                    if (D_8006794C->unk1C->unk8E4.field->unkC->unk14->unk43 != 0 && D_8006794C->unk1C->unk8 == 0) {
                        D_8006794C->unkC[D_8006794C->unk1C->unk8].disp.x = 0x1E0;
                    }
                    func_80015C38("Fr: %d\n", D_800C9D46);
                } else {
                    D_800C9EF0.unk0[1][0] = 0x1D;
                    D_800C9EF0.unk10[1][0] = 0xE01;
                }
                D_800C9D44 |= 0x100;
            }
            D_800C9D46 = D_80071E34;
        }
        break;
    case 7:
        if ((D_800C9D44 & 4) && func_80022B18() == 0) {
            func_800AE5D4();
            D_8006794C->unk1C->unk8E4.field->unkC->unk14->unk42 = 8;
        }
        break;
    }
}

void func_800AE4F8(void) {
    if (D_8006794C->unk1C->unk8E4.field->unkC->unk14->unk0 & 0x1000) {
        D_80073998[0] &= ~0x04000000;
        D_8006794C->unk1C->unk8E4.field->unkC->unk14->unk0 &= ~0x1000;
    }
    func_8019A9D8();
    func_80022B28();
    D_800C9D44 |= 4;
    D_8006794C->unk1C->unk8E4.field->unkC->unk14->unk42 = 7;
}

void func_800AE5D4(void) {
    FieldMap *f;

    D_8006794C->unk1C->unk0 &= ~0x5D;
    f = D_8006794C->unk1C->unk8E4.field;
    D_800C9D44 &= 0xFE00;
    f->unk0 &= ~0x1E01;
    D_8006794C->unk9 = 2;
    D_8006794C->unkC[0].disp.x = 0x140;
    D_8006794C->unkC[1].disp.x = 0;
    func_800B0124(0x6201, -1, D_8006794C->unk1C->unk8E4.field->unkC->unk14->unk5C, 0, 0);
}

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

void func_800AE788(void) {
    if (D_8006794C->unk1C->unk8E4.field->unk0 & 0x200) {
        ClearOTag(D_8006794C->unk1C->unk10[D_8006794C->unk1C->unk8], 0x1000);
        setlen(&D_800C9F60[D_8006794C->unk1C->unk8 ^ 1], 2);
        D_800C9F60[D_8006794C->unk1C->unk8 ^ 1].code[0] = 0xE6000000;
        D_800C9F60[D_8006794C->unk1C->unk8 ^ 1].code[1] = 0;
        addPrim(&D_8006794C->unk1C->unk10[D_8006794C->unk1C->unk8 ^ 1][0xFFF], &D_800C9F60[D_8006794C->unk1C->unk8 ^ 1]);
    } else {
        ClearOTagR(D_8006794C->unk1C->unk10[D_8006794C->unk1C->unk8], 0x1000);
    }
    func_8004C204();
}

/* end of frame: flip buffers, draw and apply the queued flag changes */
void func_800AE930(void) {
    s32 i;
    s32 j;
    u32 f;

    if ((D_800739A0[0] & 0xF09) == 0xF09 || (D_800739A0[1] & 0xF09) == 0xF09) {
        D_8006794C->unk0 |= 1;
    }
    DrawSync(0);
    if (!(func_80012AD4() & 2)) {
        if ((D_8006794C->unk1C->unk0 & 0x1C) == 0x1C && (D_8006794C->unk1C->unk8E4.field->unk0 & 4)) {
            D_8006794C->unk9 = 0;
        } else {
            D_8006794C->unk9 = 2;
        }
    }
    VSync(D_8006794C->unk9);
    ResetGraph(1);
    if (D_800C9D44 & 8) {
        func_80012D08();
        func_80012CF8();
        func_80012D18();
        D_800C9D44 &= ~8;
    }
    if (!(D_8006794C->unk1C->unk8E4.field->unk0 & 0x20)) {
        PutDispEnv(&D_8006794C->unkC[D_8006794C->unk1C->unk8]);
    }
    if (!(D_8006794C->unk1C->unk0 & 0x100)) {
        f = D_8006794C->unk1C->unk8E4.field->unk0;
        if (!(f & 0x40)) {
            if (f & 0x400) {
                PutDrawEnv(&D_8006794C->unk10[D_8006794C->unk1C->unk8]);
            } else if (f & 4) {
                DrawOTagEnv(&D_8006794C->unk1C->unk10[D_8006794C->unk1C->unk8][0xFFF], &D_8006794C->unk10[D_8006794C->unk1C->unk8]);
            } else {
                DrawOTag(D_8006794C->unk1C->unk10[D_8006794C->unk1C->unk8]);
            }
        }
    }
    if (!(D_8006794C->unk1C->unk0 & 0x20)) {
        D_8006794C->unk1C->unk8 ^= 1;
        func_8002C0DC(D_8006794C->unk1C->unk8);
    }
    if (!(D_8006794C->unk1C->unk0 & 0x100)) {
        D_8006794C->unk1C->unk0 |= D_800C9EF0.unk0[0][0];
        D_8006794C->unk1C->unk0 &= ~D_800C9EF0.unk0[1][0];
        D_8006794C->unk1C->unk8E4.field->unk0 |= D_800C9EF0.unk10[0][0];
        D_8006794C->unk1C->unk8E4.field->unk0 &= ~D_800C9EF0.unk10[1][0];
        D_800C9D44 |= D_800C9EF0.unk20[0][0];
        D_800C9D44 &= ~D_800C9EF0.unk20[1][0];
        for (i = 0; i < 2; i++) {
            for (j = 0; j < 3; j++) {
                D_800C9EF0.unk0[i][j] = D_800C9EF0.unk0[i][j + 1];
                D_800C9EF0.unk10[i][j] = D_800C9EF0.unk10[i][j + 1];
                D_800C9EF0.unk20[i][j] = D_800C9EF0.unk20[i][j + 1];
            }
            D_800C9EF0.unk0[i][3] = 0;
            D_800C9EF0.unk10[i][3] = 0;
            D_800C9EF0.unk20[i][3] = 0;
        }
    }
}

void func_800AED04(void) {
    D_8006794C->unk1C->unk8E4.field->unkC->unk14->unk41 = 0;
    D_800C9EF0.unk10[1][3] = 4;
    D_800C9EF0.unk0[0][0] = 0x43;
    D_800C9EF0.unk0[0][2] = 0x3C;
    D_800C9EF0.unk10[0][2] = 0x1919;
    D_80073998[0] |= 0x04000000;
}

void func_800AED6C(void) {
    Unk8004AB64 *a;
    SVECTOR v;
    RECT r;
    Unk80021938 *e;
    s32 i;
    s16 w;

    if (D_8006794C->unk1C->unk8E4.field->unkC->unk14->unk41 == 0) {
        D_8007BD28 = 0;
        D_8007BD30 = D_80067944 + 0x153F00;
        func_800AC5F0();
        D_8006794C->unk1C->unk8E4.field->unkC->unk14->unk20 = D_80067948;
        a = func_8004AE00();
        if (a != NULL) {
            func_800C0F08();
            v.vx = a->unk14.vx;
            v.vy = a->unk14.vy;
            v.vz = a->unk14.vz;
            gte_SetRotMatrix(&D_8006794C->unk1C->unk18);
            gte_SetTransMatrix(&D_8006794C->unk1C->unk18);
            gte_ldv0(&v);
            gte_rtps();
            gte_stsxy(&D_8006794C->unk1C->unk8E4.field->unkC->unk14->unk38);
            if ((u16)D_8006794C->unk1C->unk8E4.field->unkC->unk14->unk38 >= 320
                || D_8006794C->unk1C->unk8E4.field->unkC->unk14->unk3A < 0
                || D_8006794C->unk1C->unk8E4.field->unkC->unk14->unk3A >= 224) {
                D_8006794C->unk1C->unk8E4.field->unkC->unk14->unk38 = 160;
                D_8006794C->unk1C->unk8E4.field->unkC->unk14->unk3A = 112;
            }
            func_800C0FB0();
        }
        func_800B238C();
        if (D_8006794C->unk1C->unk81C[0] != D_8006794C->unk1C->unk8E4.field->unkC->unk14->unk3C
            && (D_8006794C->unk1C->unk0 & 0x20000000)) {
            func_800559FC(0, 0xF, 0);
            D_8006794C->unk1C->unk8E4.field->unkC->unk14->unk9 = 7;
        }
        if (D_8006794C->unk1C->unk830[0] != 0xFFFF && (D_8006794C->unk1C->unk0 & 0x02000000)) {
            func_800557A8(D_8006794C->unk1C->unk828, 0x100000, 0xF, 0);
        }
        if (D_8006794C->unk1C->unk830[1] != 0xFFFF && (D_8006794C->unk1C->unk0 & 0x08000000)) {
            func_800557A8(D_8006794C->unk1C->unk82C, 0x200000, 0xF, 0);
        }
        for (i = 1; i >= 0; i--) {
            D_800739B0[i].unk1 = 0;
            D_800739B0[i].unk0 = 0;
        }
        func_80056228();
        D_8006794C->unk1C->unk8E4.field->unkC->unk14->unk41++;
    } else {
        if (!(D_8006794C->unk1C->unk8E4.field->unkC->unk14->unk0 & 2)) {
            if (func_800220E8(0x1000B) != 0) {
                e = func_80021E5C(0x1000B);
                ClearOTag(&D_800C9F98, 1);
                setlen(&D_800C9F60[D_8006794C->unk1C->unk8 ^ 1], 2);
                D_800C9F60[D_8006794C->unk1C->unk8 ^ 1].code[0] = 0xE6000000;
                D_800C9F60[D_8006794C->unk1C->unk8 ^ 1].code[1] = 0;
                addPrim(&D_800C9F98, &D_800C9F60[D_8006794C->unk1C->unk8]);
                DrawOTag(&D_800C9F98);
                r.x = 0;
                r.y = 0x100;
                r.h = 0x100;
                r.w = w = (u32)(e->unkC / r.h) >> 1;
                if (e->unkC % r.h != 0) {
                    r.w = w + 1;
                }
                LoadImage(&r, (u32 *)e->unk8);
                DrawSync(0);
                func_800219D8(e, r.x, r.y, r.w, r.h);
            } else {
                func_80022390(func_8002208C()->unk4);
            }
            func_80022280((s32)D_80067944, 0x156F00);
            func_800B0124(0xD000, 0x27C, 0, 0x80, 0x7F);
            func_800B0124(0xD000, 0x27B, 0, 0x80, 0x7F);
            func_800B0124(0xD000, 0x27A, 0, 0x80, 0x7F);
            D_8006794C->unk1C->unk8E4.field->unkC->unk14->unk0 |= 2;
        }
        if (!(D_8006794C->unk1C->unk8E4.field->unkC->unk14->unk0 & 1)
            && func_80053A10(D_8006794C->unk1C->unk8E4.field->unkC->unk14->unk38,
                             D_8006794C->unk1C->unk8E4.field->unkC->unk14->unk3A)) {
            D_8006794C->unk1C->unk8E4.field->unkC->unk14->unk0 |= 1;
        }
        if (D_8006794C->unk1C->unk8E4.field->unkC->unk14->unk9 > 0) {
            D_8006794C->unk1C->unk8E4.field->unkC->unk14->unk9--;
        } else if (D_8006794C->unk1C->unk8E4.field->unkC->unk14->unk9 == 0) {
            if (D_8006794C->unk1C->unk8E4.field->unkC->unk14->unk3C != 0xFFFF
                && D_8006794C->unk1C->unk8E4.field->unkC->unk14->unk3C != D_8006794C->unk1C->unk81C[0]) {
                if (D_8006794C->unk1C->unk0 & 0x20000000) {
                    func_800B0124(0x800, D_8006794C->unk1C->unk81C[0], 0, 0, 0);
                }
                func_800B0124(0x700, D_8006794C->unk1C->unk8E4.field->unkC->unk14->unk3C, 0, 0, 0);
                D_8006794C->unk1C->unk8E4.field->unkC->unk14->unk9 = -1;
            } else {
                D_8006794C->unk1C->unk8E4.field->unkC->unk14->unk9 = -2;
            }
        } else if (D_8006794C->unk1C->unk8E4.field->unkC->unk14->unk9 == -1) {
            if (func_800B0124(0xC00, -1, 0, 0, 0) == 0) {
                D_8006794C->unk1C->unk8E4.field->unkC->unk14->unk9 = -2;
            }
        } else if (D_8006794C->unk1C->unk8E4.field->unkC->unk14->unk9 == -2) {
            func_800B1C4C();
            if (D_8006794C->unk1C->unk8E4.field->unkC->unk14->unk3C != 0xFFFF
                && !(D_8006794C->unk1C->unk0 & 0x20000000)) {
                func_800B0124(0, D_8006794C->unk1C->unk8E4.field->unkC->unk14->unk3C, 0, 0, 0);
                func_800559C8(0, 0x7F);
            }
            D_8006794C->unk1C->unk8E4.field->unkC->unk14->unk9 = -3;
        }
    }
    if ((D_8006794C->unk1C->unk8E4.field->unkC->unk14->unk0 & 1)
        && D_8006794C->unk1C->unk8E4.field->unkC->unk14->unk9 == -3) {
        D_8006794C->unk1C->unk0 &= ~0x7F;
        D_8006794C->unk1C->unk8E4.field->unk0 &= ~0x1919;
        D_80073998[0] &= 0xFBFFFFFF;
        D_8006794C->unk0 |= 2;
    }
}

/* toggle the pause screen */
void func_800AF740(void) {
    u8 c[4];
    u8 buf[8];
    s32 tmp;
    s32 i;
    s32 bright;
    s32 v;
    u32 a;
    SPRT *sp;

    D_8006794C->unk1C->unk0 ^= 0x100;
    if (D_8006794C->unk1C->unk0 & 0x100) {
        func_8005562C(0);
        func_80023F60(1);
        for (i = 1; i >= 0; i--) {
            D_800739B0[i].unk1 = 0;
            D_800739B0[i].unk0 = 0;
        }
        ClearOTag(&D_800C9F98, 1);
        func_80027834((320 - D_800686D0[235].unk4) / 2, (224 - D_800686D0[235].unk5) / 2, &D_800C9F98, 0xEB, 0, &tmp, &D_800C9FA0);
        bright = 0;
        if (func_8002F800() || func_8002F810()) {
            bright = 1;
        }
        v = func_8002F864();
        if (bright && v == 0) {
            if (func_8002F854() == 2) {
                func_8002FC10(buf);
                c[0] = (buf[0] + buf[4]) >> 1;
                c[1] = (buf[1] + buf[5]) >> 1;
                c[2] = (buf[2] + buf[6]) >> 1;
            } else {
                func_8002F9A8(c);
            }
            if (c[0] >= 200 && c[1] >= 200 && c[2] >= 200) {
                D_800C9FA0.unk0.code |= 2;
                sp = &D_800C9FA0.unk0;
                sp++;
                SetDrawTPage(sp, 1, 1,
                             getTPage(0, 2, func_8001DA30((s16)D_800686D0[235].unk6), func_8001DA3C((s16)D_800686D0[235].unk6)));
            }
        }
        setlen(&D_800C9FE0, 3);
        D_800C9FE0.code = 0x62;
        setRGB0(&D_800C9FE0, 0x60, 0x60, 0x60);
        setWH(&D_800C9FE0, 320, 224);
        setXY0(&D_800C9FE0, 0, 0);
        setlen(&D_800C9F90, 1);
        a = getaddr(&D_800C9F98);
        D_800C9F90.code[0] = 0xE1000240;
        setaddr(&D_800C9FE0, a), setaddr(&D_800C9F98, &D_800C9FE0);
        addPrim(&D_800C9F98, &D_800C9F90);
        DrawOTag(&D_800C9F98);
        D_8006794C->unk1C->unk0 |= 0x20;
    } else {
        func_80023F60(0);
        func_80055698(0);
        D_8006794C->unk1C->unk0 &= ~0x20;
    }
}

void func_800AFAC0(s32 arg0, s32 arg1, s32 arg2, s32 arg3, s32 arg4) {
    Unk800AFCBC *h;
    FieldEnt *e;

    h = D_8006794C->unk1C->unk8E4.field->unkC->unk14->unk10;
    if (h == NULL) {
        return;
    }
    e = &h->unk8[arg0];
    if (arg1 == 0x82) {
        if (arg2 != -1) {
            func_800B00B0(e);
            e->unk1 = arg2;
            e->unk18 = func_8001C7FC(4, (arg2 & 0xFFFF) | 0x160000, 0);
            e->unkE = func_800C9700(e->unk18) * 16;
        } else {
            e->unk18 = NULL;
        }
    } else if (arg1 == 0x83) {
        if (arg3 != 0) {
            e->unk0_0 |= arg2;
        } else {
            e->unk0_0 &= ~arg2;
        }
    } else if (arg1 == 0x87) {
        e->unk8[0] = arg2;
        e->unk8[1] = arg3;
        e->unk8[2] = arg4;
    } else if (arg1 == 0x8C) {
        e->unk10[0] = arg2;
        e->unk10[1] = arg3;
        e->unk10[2] = arg4;
    } else if (arg1 == 0x91) {
        e->unk4 = arg2;
    } else if (arg1 == 0x96) {
        e->unk2 = func_8004B098(arg2);
        e->unk3 = arg3;
    } else if (arg1 == 0x9B) {
        e->unk7 = arg2;
    } else if (arg1 == 0x9C) {
        e->unk0_4 = arg2;
    } else if (arg1 == 0xA0) {
        e->unk6 = arg2;
    } else if (arg1 == 0xA1) {
        e->unk16 = arg2 * 16;
    } else if (arg1 == 0xA5) {
        e->unk1C[0] = arg2;
        e->unk1C[1] = arg3;
        e->unk1C[2] = arg4;
    } else if (arg1 == 0xAA) {
        e->unk22 = arg2;
    }
}

u8 *func_800AFCBC(u8 *arg0) {
    s32 i;
    s32 n;
    Unk800AFCBC *h;

    func_800AA0C8(D_8006794C->unk1C->unk8E4.field->unkC->unk14, (s16)D_8006794C->unk1C->unkC);
    i = 0;
    n = 0x10;
    h = (Unk800AFCBC *)arg0;
    arg0 += sizeof(Unk800AFCBC);
    h->unk4 = (u16 *)arg0;
    arg0 += 0x20;
    h->unk0 = 1;
    h->unk2 = n;
    h->unk3 = h->unk2;
    for (; i < n; i++) {
        h->unk4[i] = 0xFFFF;
    }
    h->unk8 = (FieldEnt *)arg0;
    arg0 += h->unk3 * 0x24;
    for (i = 0; i < h->unk3; i++) {
        func_800B00B0(&h->unk8[i]);
    }
    h->unkC[0] = arg0;
    h->unkC[1] = arg0 + 0x3000;
    D_8006794C->unk1C->unk8E4.field->unkC->unk14->unk10 = h;
    return arg0 + 0x6000;
}

void func_800AFDE8(void) {
    Unk800AFCBC *h;
    SVECTOR rot;
    s32 xy[2];
    s32 i;
    FieldEnt *e;
    u8 *prim;
    u32 *ot;
    long *t;

    h = D_8006794C->unk1C->unk8E4.field->unkC->unk14->unk10;
    if (h == NULL) {
        return;
    }
    if (D_800C9D44 & 0x40) {
        func_8019AE0C(&xy[0], &xy[1]);
        SetGeomOffset(xy[0], xy[1]);
    } else {
        func_800C0F08();
    }
    prim = h->unkC[D_8006794C->unk1C->unk8];
    ot = &D_8006794C->unk1C->unk10[D_8006794C->unk1C->unk8][D_8006794C->unk1C->unk8E4.field->unkC->unk14->unk30];
    for (i = 0; i < h->unk3; i++) {
        e = &h->unk8[i];
        if (e->unk18 != NULL && (e->unk0_0 & 1)) {
            if ((s8)e->unk2 != -1) {
                t = ((MATRIX *)((FieldObjSlot *)func_8001E350((s8)e->unk2))->unk8->unk20)[e->unk3].t;
                e->unk8[0] = t[0] + e->unk1C[0];
                e->unk8[1] = t[1] + e->unk1C[1];
                e->unk8[2] = t[2] + e->unk1C[2];
            }
            rot.vx = e->unk10[0];
            rot.vy = e->unk10[1];
            rot.vz = e->unk10[2];
            rot.pad = 0;
            prim = func_800C970C(prim, e->unk18, e->unk8, &rot, e->unk4, e->unk16 >> 4, e->unk0_4 < 4 ? e->unk0_4 : 0xFF, e->unk7 << 4, &ot[e->unk22]);
            e->unk16 += (s8)e->unk6;
            if (e->unk16 >= e->unkE) {
                e->unk16 = 0;
            } else if (e->unk16 < 0) {
                e->unk16 = ((e->unkE >> 4) - 1) << 4;
            }
        }
    }
    if (!(D_800C9D44 & 0x40)) {
        func_800C0FB0();
    }
}

/* reset a field entity */
void func_800B00B0(FieldEnt *arg0) {
    s32 i;

    arg0->unk4 = 0x1000;
    arg0->unk6 = 0x10;
    arg0->unk7 = 0x80;
    arg0->unk0_0 = 1;
    arg0->unk0_4 = 0xF;
    arg0->unk1 = -1;
    arg0->unk2 = -1;
    arg0->unk3 = 0;
    for (i = 0; i < 3; i++) {
        arg0->unk8[i] = 0;
        arg0->unk10[i] = 0;
        arg0->unk1C[i] = 0;
    }
    arg0->unk16 = 0;
    arg0->unkE = 0;
    arg0->unk18 = NULL;
    arg0->unk22 = 0;
}

INCLUDE_ASM("asm/us/ovl_0a/nonmatchings/ovl_0a", func_800B0124);

void func_800B1A20(s16 arg0, s16 arg1, s16 arg2, u8 *arg4, u8 *arg5) {
    SVECTOR v;
    DVECTOR sxy;
    s32 z;
    s32 a;
    s32 pan;
    s32 b;

    v.vx = arg0;
    v.vy = arg1;
    v.vz = arg2;
    gte_SetRotMatrix(&D_8006794C->unk1C->unk18);
    gte_SetTransMatrix(&D_8006794C->unk1C->unk18);
    gte_ldv0(&v);
    gte_rtps();
    gte_stsxy(&sxy);
    gte_stsz(&z);
    z = z / 4 + D_8006794C->unk1C->unk8E4.field->unkC->unk14->unk30;
    if (z < 0) {
        z = 0;
    } else if (z >= 0x1000) {
        z = 0xFFF;
    }
    a = z / 32;
    b = (128 - a) * 2;
    if ((u16)sxy.vx < 320) {
        pan = sxy.vx * b / 320;
        pan += a;
    } else if (sxy.vx < 0) {
        if (sxy.vx < -320) {
            sxy.vx = -320;
        }
        pan = sxy.vx + 320;
        pan = pan * a / 320;
    } else {
        if (sxy.vx >= 640) {
            sxy.vx = 639;
        }
        pan = (sxy.vx - 320) * a / 320;
        pan += 255;
        pan -= a;
    }
    *arg4 = pan;
    *arg5 = (0x1000 - z) * 127 / 4096;
}

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

/* drop the queued entries except categories 1 and 4 */
void func_800B1D9C(s32 arg0) {
    s32 i;
    u8 t;

    if (arg0 >= 0x40000) {
        for (i = 4; i >= 0; i--) {
            t = (u32)D_800C9FF8[i] >> 24;
            if (t != 4 && t != 1) {
                D_800C9FF8[i] = -1;
                D_800C9FF0[i] = -1;
            }
        }
    }
}

void func_800B1E04(FieldScene *arg0, s16 arg1) {
    s32 t = (s32)arg0->unk24;
    s32 i;

    for (i = func_8001C8B0(t, 1, 0x80000, 0) - 1; i >= 0; i--) {
        func_8005546C(func_8001C8B0(t, 2, (i & 0xFFFF) | 0x80000, 0), 0);
    }
    func_80056228();
}

void func_800B1E8C(FieldEvtCtx **arg0, s32 arg1) {
    s16 a;
    s16 b;
    u16 c;
    u8 vol;
    u8 pan;
    FieldTrig *base;
    u8 n;
    s32 i;
    s32 j;
    FieldEvtCtx *ctx;
    FieldTrig *t;
    s32 r;
    s32 ch;
    s32 snd;
    s32 v;

    base = D_8006794C->unk1C->unk8E4.field->unkC->unk14->unk58;
    n = D_8006794C->unk1C->unk8E4.field->unkC->unk14->unk4B;
    for (i = arg1 - 1; i >= 0; i--) {
        ctx = arg0[i];
        if (ctx == NULL || (ctx->unk0 & 0x40)) {
            continue;
        }
        for (j = n - 1; j >= 0; j--) {
            t = &base[j];
            if (t->unk0 == ctx->unkC->unk3B && t->unk4 == ctx->unkC->unk36 && t->unk6 == ctx->unkC->unk3A) {
                r = 0;
                a = 0;
                b = 0;
                c = 0;
                vol = 0x80;
                pan = 0x7F;
                func_800C76E8(ctx->unkC->unk3B, (u16 *)&a, (u16 *)&b);
                if (a != -1) {
                    func_800C7800(a, &c);
                    c = (s16)c >> 8;
                }
                ch = t->unk0 & 3;
                ch = (0x10000 << ch) & 0xF0000;
                snd = t->unk8[(c >> 4) & 1] | 0x80000;
                if (t->unk1 != 0) {
                    if (D_8007B720 >= D_8007B92D) {
                        func_8004BF20();
                    }
                    v = *D_8007B720++;
                    r = (v >> 4) - 8;
                }
                func_800B1A20(ctx->unkC->unk14.vx, ctx->unkC->unk14.vy, ctx->unkC->unk14.vz, &vol, &pan);
                func_800B0124(0xD000, snd, ch, vol, pan);
                if (r != 0) {
                    func_800B0124(0x9203, snd, ch, r, 0);
                }
            }
        }
    }
}

/* find the trigger matching the object, id and value (searching backwards) */
FieldTrig *func_800B2100(FieldEvtCtx *arg0, u16 arg1, s16 arg2) {
    FieldScene *sc = D_8006794C->unk1C->unk8E4.field->unkC->unk14;
    FieldTrig *base = sc->unk58;
    FieldTrig *e = &base[sc->unk4B] - 1;

    do {
        if (e->unk0 == arg0->unkC->unk3B && e->unk4 == arg1 && e->unk6 == arg2) {
            return e;
        }
        e--;
    } while (e >= base);
    return NULL;
}

void func_800B21A0(FieldEvtCtx *arg0, s16 arg1, s16 arg2) {
    FieldScene *sc = D_8006794C->unk1C->unk8E4.field->unkC->unk14;
    u8 i = sc->unk4B;
    FieldTrig *e;
    u16 id;
    s32 n;

    n = i + 1;
    sc->unk4B = n;
    e = &sc->unk58[i];
    e->unk0 = arg0->unkC->unk3B;
    id = arg0->unkC->unk34;
    e->unk4 = arg1;
    do {
        e->unk6 = arg2;
    } while (0);
    e->unk2 = id;
}

s32 func_800B2218(FieldEvtCtx *arg0, u16 arg1, s16 arg2) {
    FieldTrig *base;
    FieldTrig *last;
    FieldTrig *e;

    base = D_8006794C->unk1C->unk8E4.field->unkC->unk14->unk58;
    e = &base[D_8006794C->unk1C->unk8E4.field->unkC->unk14->unk4B] - 1;
    last = e;
    do {
        if (e->unk0 == arg0->unkC->unk3B && e->unk4 == arg1 && e->unk6 == arg2) {
            if (e != last) {
                *e = *last;
            }
            D_8006794C->unk1C->unk8E4.field->unkC->unk14->unk4B--;
        }
        e--;
    } while (e >= base);
    return -1;
}

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

void func_800B238C(void) {
    FieldScene *o;
    FileEntry *base;
    s32 i;
    s32 j;
    s32 id;
    s32 key;
    s32 v;
    s32 w;

    o = D_8006794C->unk1C->unk8E4.field->unkC->unk14;
    o->unk3C = D_8006794C->unk1C->unk81C[0];
    key = (u16)o->unkA;
    D_8006794C->unk1C->unk8E4.field->unkC->unk14->unk3E = D_8006794C->unk1C->unk820[0];
    base = D_8006794C->unk1C->unk7E4->unk4;
    key |= 0x110000;
    w = key;
    for (i = 0; i < D_8006794C->unk1C->unk7E4->unk2; i++) {
        id = base[i].unkC;
        if (base[i].unk0 == w && ((id >> 16) & 0xFF) == 7) {
            D_8006794C->unk1C->unk8E4.field->unkC->unk14->unk3C = id;
            for (j = 0; j < D_8006794C->unk1C->unk7E4->unk2; j++) {
                w = base[j].unkC;
                v = base[j].unk0;
                if (((base[j].unk0 >> 16) & 0xFF) == 9 && w == id) {
                    D_8006794C->unk1C->unk8E4.field->unkC->unk14->unk3E = v;
                    return;
                }
            }
            return;
        }
    }
}

/* assign sound slots to the field's sound files and load the missing ones (smallest first) */
s32 func_800B2528(void) {
    u32 id[5];
    s32 sz[5];
    u8 used[5];
    s8 slot[5];
    s32 n;
    s32 i;
    s32 j;
    s32 cnt;
    s32 found;
    s32 s;
    s32 k;
    s32 done;
    s32 t;
    s32 u;
    s32 m;
    s32 min;
    s32 best;
    u32 x;
    u32 y;
    u32 z;
    FileEntry *e;

    n = 0;
    e = D_8006794C->unk1C->unk7E4->unk4;
    for (i = 4; i >= 0; i--) {
        id[i] = -1;
        sz[i] = -1;
        used[i] = 0;
        slot[i] = -1;
    }
    for (i = D_8006794C->unk1C->unk7E4->unk2 - 1; i >= 0; i--) {
        x = e[i].unk0;
        if (((x >> 16) & 0xFF) == 9) {
            found = 0;
            if (((e[i].unkC >> 16) & 0xFF) == 8) {
                s = -1;
                for (j = 4; j >= 0; j--) {
                    if (D_800C9FF8[j] == x) {
                        found = 1;
                        s = D_800C9FF0[j];
                        break;
                    }
                }
                id[n] = x;
                sz[n] = e[i].unk4;
                used[n] = found;
                slot[n] = s;
                n++;
            }
        }
    }
    for (i = n - 1; i >= 0; i--) {
        if ((id[i] >> 24) == 4 && slot[i] == -1) {
            k = 0;
            do {
                done = 1;
                for (j = n - 1; j >= 0; j--) {
                    if (slot[j] == k) {
                        k++;
                        done = 0;
                        break;
                    }
                }
            } while (done == 0);
            slot[i] = k;
        }
    }
    cnt = 0;
    for (i = n - 1; i >= 0; i--) {
        if ((id[i] >> 24) == 4) {
            cnt++;
        }
    }
    if (cnt < 2) {
        for (m = 4; m >= 0; m--) {
            y = D_800C9FF8[m];
            if (y != -1 && (y >> 24) == 4) {
                t = -1;
                for (j = n - 1; j >= 0; j--) {
                    if (id[j] == y) {
                        t = m;
                        break;
                    }
                }
                if (t == -1) {
                    u = -1;
                    j = n - 1;
                    t = D_800C9FF0[m];
                    for (; j >= 0; j--) {
                        if (slot[j] == t) {
                            u = j;
                            break;
                        }
                    }
                    if (u == -1) {
                        id[n] = y;
                        sz[n] = 0;
                        used[n] = 1;
                        slot[n] = t;
                        n++;
                        cnt++;
                        if (cnt >= 2) {
                            break;
                        }
                    }
                }
            }
        }
    }
    for (i = 4; i >= 0; i--) {
        D_800C9FF8[i] = id[i];
        D_800C9FF0[i] = slot[i];
    }
    for (;;) {
        min = 0x7FFFFFFF;
        best = -1;
        for (i = n - 1; i >= 0; i--) {
            if (id[i] != -1 && used[i] == 0 && sz[i] < min) {
                min = sz[i];
                best = i;
            }
        }
        if (best == -1) {
            break;
        }
        z = id[best];
        func_800B0124(0x7700, z, z >> 24, slot[best], 0);
        while (func_800B0124(0xC00, -1, 0, 0, 0) != 0) {
        }
        id[best] = -1;
        sz[best] = -1;
    }
    return 0;
}

s32 func_800B29A0(s32 arg0, s32 *arg1) {
    FileEntry *e = func_80063088(D_8006794C->unk1C->unk7E4, arg0);

    if (arg1 != NULL) {
        *arg1 = e->unk8 << 11;
    }
    return e->unk4;
}

s32 func_800B29F8(Unk800B29F8 *arg0) {
    s32 a;
    s32 id;
    s32 t;
    s32 n;
    s32 b;
    s32 r;
    GameData *gd;
    FieldScene *sc;

    r = 0;
    a = arg0->unk8;
    id = D_8006794C->unk1C->unk852 | 0x90000;
    if (arg0->unkC == 0) {
        b = a;
        t = func_8001C8B0(a, 2, 0x1B0000, 0);
        if (t == -1) {
            a = func_8001C8B0(a, 4, id, 0);
            D_8006794C->unk1C->unk87C = func_8001CA70(b);
        } else {
            a = func_8001C8B0(t, 4, id, 0);
            D_8006794C->unk1C->unk87C = func_8001C8B0(b, 2, 0x1B0001, 0) - D_8006794C->unk1C->unk8C0;
            func_800B1D9C(D_8006794C->unk1C->unk87C - arg0->unkC);
        }
    }
    gd = D_8006794C->unk1C;
    n = gd->unk87C - arg0->unkC;
    if (arg0->unk10 < n) {
        n = arg0->unk10;
    }
    n = arg0->unk8 + n - a;
    if ((id & 0xFFFF) != 0x84) {
        sc = gd->unk8E4.field->unkC->unk14;
        if (sc->unk0 & 0x10) {
            r = func_80056048(a, sc->unk5E, 0);
        } else {
            r = func_80055E14(a, n, 0);
        }
    }
    arg0->unk8 = a;
    return r;
}

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
    func_800B31B8((FieldEvtCtx *)func_8001E350(arg0), arg1, arg2, arg3);
}

/* apply the field lights to the object colours */
void func_800B2D10(FieldEvtCtx **arg0, s32 arg1) {
    FieldScene *sc;
    Unk800B3254 *lt;
    FieldLight *amb;
    FieldLight *dir;
    FieldActorInfo *infos;
    Unk800B3294 *def;
    s32 i;
    FieldEvtCtx *c;
    s32 chg;
    FieldActorInfo *info;
    Unk800B3294 *l;
    u32 shade;
    FieldLight *fl;
    s16 floor;
    u32 a;
    u32 b;
    u32 col;
    s32 dx;
    s32 dy;
    s32 j;
    s32 k;
    FieldActorInfo *inf2;

    sc = D_8006794C->unk1C->unk8E4.field->unkC->unk14;
    lt = sc->unk1C;
    if (lt == NULL || func_800B3254(lt) == NULL) {
        return;
    }
    amb = NULL;
    dir = NULL;
    infos = sc->unk54;
    def = (Unk800B3294 *)func_800B3294(lt, -1);
    func_800B3300(lt, &amb, &dir);
    for (i = arg1 - 1; i >= 0; i--) {
        c = arg0[i];
        chg = 0;
        if (c == NULL) {
            continue;
        }
        info = &infos[c->unkC->unk3B];
        l = (Unk800B3294 *)func_800B3294(lt, c->unk4);
        shade = c->unk0 & 4;
        fl = func_800B3378(lt, c, &floor);
        if ((u8)info->unk4 != (u8)floor || shade != (info->unk0 & 4) || !(c->unk0 & 0x100)) {
            chg = 1;
        }
        if (chg) {
            if (!(c->unk0 & 0x400)) {
                b = 0;
                if (l != NULL) {
                    a = l->unk4;
                } else {
                    a = def->unk4;
                }
                if (shade && dir != NULL) {
                    b = dir->unk4;
                } else if (fl != NULL) {
                    b = fl->unk4;
                } else if (amb != NULL) {
                    b = amb->unk4;
                }
                col = ((((a & 0xFF) + (b & 0xFF)) * 8) & 0xFF) | ((((a & 0xFF00) + (b & 0xFF00)) * 8) & 0xFF00)
                    | ((((a & 0xFF0000) + (b & 0xFF0000)) * 8) & 0xFF0000) | 0x01000000;
                info->unkC.w = col;
                info->unk8.w = col;
                c->unk0 |= 0x100;
            }
            if (l != NULL) {
                dx = l->unk2;
                dy = l->unk3;
            } else {
                dx = def->unk2;
                dy = def->unk3;
            }
            if (fl != NULL) {
                dx += fl->unk2;
                dy += fl->unk3;
            } else if (amb != NULL) {
                dx += amb->unk2;
                dy += amb->unk3;
            }
            dx *= 8;
            D_8006794C->unk1C->unk8E4.field->unkC->unk14->unk2C[c->unkC->unk3B].unk0.unk0 = dx;
            D_8006794C->unk1C->unk8E4.field->unkC->unk14->unk2C[c->unkC->unk3B].unk0.unk1 = dy * 14;
            D_8006794C->unk1C->unk8E4.field->unkC->unk14->unk2C[c->unkC->unk3B].unk0.unk2 = dy * 12;
        }
        info->unk4 = floor;
        info->unk0 = (info->unk0 & ~4) | shade;
    }
    for (i = arg1 - 1; i >= 0; i--) {
        c = arg0[i];
        if (c == NULL) {
            continue;
        }
        inf2 = &infos[c->unkC->unk3B];
        k = inf2->unk8.w;
        if (k >> 24 != 0) {
            for (j = ((Unk800C95C8 *)c->unk8)->unk8->unk3 - 1; j >= 0; j--) {
                func_800B9E4C(c->unk8, j & 0xFF, (s16)(k & 0xFF), (s16)((k >> 8) & 0xFF), ((u32)k >> 16) & 0xFF);
            }
            inf2->unk8.w = inf2->unkC.w;
            inf2->unkC.w = 0;
        }
    }
}

/* set the object's light colour */
s32 func_800B31B8(FieldEvtCtx *arg0, s32 arg1, s32 arg2, s32 arg3) {
    u32 f = arg0->unk0;
    FieldActorInfo *info = &D_8006794C->unk1C->unk8E4.field->unkC->unk14->unk54[arg0->unkC->unk3B];
    u32 col;

    if (!(f & 0x800)) {
        col = (arg1 & 0xFF) | ((arg2 << 8) & 0xFF00) | ((arg3 << 16) & 0xFF0000) | 0x01000000;
        info->unkC.w = col;
        info->unk8.w = col;
        arg0->unk0 |= 0xC00;
        return 1;
    }
    arg0->unk0 = f & ~0x800;
    return 0;
}

FieldLight *func_800B3254(Unk800B3254 *arg0) {
    if (arg0 != NULL && arg0->unk9 != 0) {
        return &arg0->unkC[arg0->unk6];
    }
    return NULL;
}

Unk800B3294 *func_800B3294(Unk800B3254 *arg0, u16 arg1) {
    Unk800B3294 *base;
    Unk800B3294 *p;
    s32 id;
    u32 v;

    v = arg1;
    if (arg0->unk9 == 0) {
        return NULL;
    }
    id = v & 0xFFFF;
    base = (Unk800B3294 *)&arg0->unkC[arg0->unk6];
    p = &base[arg0->unk9] - 1;
    do {
        if (p->unk0 == id) {
            return p;
        }
        p--;
    } while (p >= base);
    return NULL;
}

FieldLight *func_800B3300(Unk800B3254 *arg0, FieldLight **arg1, FieldLight **arg2) {
    FieldLight *base;
    FieldLight *p;

    *arg1 = NULL;
    *arg2 = NULL;
    base = arg0->unkC;
    if (arg0->unk7 == 0) {
        return NULL;
    }
    p = &arg0->unkC[arg0->unk7 - 1];
    do {
        if (p->unk0 == 1) {
            *arg1 = p;
        } else if (p->unk0 == 2) {
            *arg2 = p;
        }
        p--;
    } while (p >= base);
    return base;
}

/* find the floor light covering the object's current floor */
FieldLight *func_800B3378(Unk800B3254 *arg0, FieldEvtCtx *arg1, s16 *arg2) {
    u16 tri;
    FieldLight *p;
    s32 n;
    s32 v;

    func_800C76E8(arg1->unkC->unk3B, &tri, (u16 *)arg2);
    if (arg0->unk7 != 0) {
        if ((u8)*arg2 == 0xFF) {
            return NULL;
        }
        p = &arg0->unkC[arg0->unk7 - 1];
        do {
            if (p->unk0 == 0) {
                v = p->unk8;
                for (n = 3; n >= 0; n--) {
                    if ((v & 0xFF) == *arg2) {
                        return p;
                    }
                    v >>= 8;
                }
            }
            p--;
        } while (p >= arg0->unkC);
    }
    return NULL;
}

s32 func_800B3444(s32 arg0, s32 arg1, s32 arg2) {
    s32 start;
    s32 size;
    s32 end;

    if (arg0 == 0 && arg1 == 0) {
        if (arg2 != 0) {
            if (!(D_8006794C->unk1C->unk8E4.field->unkC->unk14->unk0 & 0x100)) {
                start = D_8006794C->unk1C->unk8E4.field->unkC->unk14->unk20;
                if (start == 0) {
                    start = D_80067948;
                }
                if (func_800221C0(start, 0x5000) != 0) {
                    func_80022390(func_8002208C()->unk4);
                }
                if (D_8006794C->unk1C->unk8E4.field->unkC->unk14->unk20 != 0) {
                    start = D_8006794C->unk1C->unk8E4.field->unkC->unk14->unk20;
                    D_8006794C->unk1C->unk8E4.field->unkC->unk14->unk20 = func_80050BE0(start);
                    size = D_8006794C->unk1C->unk8E4.field->unkC->unk14->unk20 - start;
                } else {
                    start = D_80067948;
                    end = func_80050BE0(start);
                    size = end - start;
                    D_80067948 = end;
                }
                func_800AC620();
                func_80022280(start, size);
                D_8006794C->unk1C->unk8E4.field->unkC->unk14->unk0 |= 0x100;
            }
            D_8006794C->unk1C->unk8E4.field->unkC->unk14->unk0 |= 0x200;
        } else {
            D_8006794C->unk1C->unk8E4.field->unkC->unk14->unk0 &= ~0x200;
        }
    }
    return 0;
}

void func_800B3684(void) {
    if (D_8006794C->unk1C->unk8E4.field->unkC->unk14->unk0 & 0x200) {
        func_80050AAC();
    }
}

s32 func_800B36E0(s32 arg0, s32 arg1) {
    s32 flag;
    s32 id;
    s32 k;
    u8 type;
    s32 x;
    Unk80021938 *e;
    Unk80021938 *p;
    Unk80021938 *q;
    FileEntry *f;

    type = arg0;
    flag = 0;
    if ((s8)type < 0) {
        type -= 0x80;
        flag = 1;
    }
    switch (type & 0xFF) {
    default:
        return -1;
    case 0x11:
        k = 0x1000B;
        if (flag && func_800B3D10(0x1000B)) {
            return -1;
        }
        if (func_800B3BD8(0x1000B)) {
            return -1;
        }
        if (func_80021E5C(0x1000B)) {
            return -1;
        }
        if (flag) {
            func_800B3DE4(0x1000B);
        } else {
            func_800B3CAC(0x1000B);
        }
        if (!(D_8006794C->unk1C->unk8E4.field->unkC->unk14->unk0 & 0x100000)) {
            goto done;
        }
        p = func_80021E20();
        if (p != NULL && p->unk4 != k) {
            func_80021C20(p);
        }
        if (flag || func_80021E5C(k) != NULL) {
            goto set;
        }
        goto clear;
    case 5:
        id = (u16)arg1 | 0x50000;
        if ((u16)arg1 >= 9000) {
            return -1;
        }
        f = func_80063088(D_8006794C->unk1C->unk7E4, id);
        if (f == NULL) {
            return -1;
        }
        if (f->unk8 == 0) {
            return -1;
        }
        if (flag && func_800B3D10(id)) {
            return -1;
        }
        if (func_800B3BD8(id)) {
            return -1;
        }
        e = func_80021E5C(id);
        if (e != NULL && e->unk2 == 0) {
            if (func_80021A1C(e)) {
                return -1;
            }
            if (!func_80021A1C(e) && (e->unk1C >> 2) < e->unkC) {
                return -1;
            }
        }
        if (flag) {
            x = id;
        } else {
            func_800B3CAC(id);
            x = 0x1000B;
        }
        func_800B3DE4(x);
        if (!(D_8006794C->unk1C->unk8E4.field->unkC->unk14->unk0 & 0x100000)) {
            goto done;
        }
        e = func_80021E20();
        if (e != NULL && e->unk4 != id && func_80021A1C(e)) {
            func_80021C20(e);
        }
        q = func_80021E5C(id);
        if (!flag && (q == NULL || q->unk2 != 0)) {
        clear:
            D_8006794C->unk1C->unk8E4.field->unkC->unk14->unk0 &= ~0x100000;
        }
    set:
        D_8006794C->unk1C->unk8E4.field->unkC->unk14->unk0 |= 0x800000;
    done:
        D_8006794C->unk1C->unk8E4.field->unkC->unk14->unk0 |= 0x200000;
        return 0;
    }
}

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

/* is any active entry of category 5? */
s32 func_800B3B88(void) {
    s32 i;

    for (i = D_800CA050 - 1; i >= 0; i--) {
        if (((D_800CA010[i] >> 16) & 0xFF) == 5) {
            return 1;
        }
    }
    return 0;
}

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

/* start streaming a file into the free area */
s32 func_800B3E48(s32 arg0, s32 arg1, s32 arg2, s32 arg3) {
    u32 top;
    s32 size;
    Unk80021938 *p;

    if (arg0 == -1 || arg2 == 0) {
        return 0;
    }
    if (arg1 == 0 && arg3 == 0) {
        return 0;
    }
    top = func_800AC380(NULL);
    if ((u32)(D_80067944 + 0x153F00) < top) {
        top = (u32)(D_80067944 + 0x153F00);
    }
    size = top - D_800C9D58;
    size = size / 2048 * 2048;
    if (arg1 == 0 && size < 0x8000) {
        return 0;
    }
    p = func_80021CFC();
    if (p == NULL) {
        return 0;
    }
    func_80021974(p, arg1, arg0, arg1 == 0 ? D_800C9D58 : 0, size, arg3, arg2, arg3);
    D_800C9D58 += arg3;
    return (s32)p;
}

static inline s32 func_800B3F8C_grp(s32 id) {
    return id / 50;
}

void func_800B3F8C(void) {
    FileEntry *tbl;
    FileEntry *e;
    FileEntry *f;
    FileEntry *g;
    s32 i;
    s32 cnt;
    s32 best;
    s32 sel;
    s32 cur;
    s32 key;
    s32 x;

    tbl = D_8006794C->unk1C->unk7E4->unk4;
    key = func_800B3B64();
    if (key == 0) {
        cnt = 0;
        key = func_800B3B40();
        if (key != 0) {
            cnt = 1;
        } else {
            for (i = D_8006794C->unk1C->unk7E4->unk2 - 1; i >= 0; i--) {
                if (((tbl[i].unk0 >> 16) & 0xFF) == 0x11) {
                    cnt = 1;
                    break;
                }
            }
        }
        if (cnt != 0) {
            g = func_8001E218(0, 0x1000B);
            func_800B3E48(0x1000B, 0, g->unk4, (g->unkC - g->unk4) << 11);
            key = func_800B3B40();
            if (key != 0) {
                func_800B3C24(0x1000B);
            }
        }
    }
    if (func_800B3B88() != 0) {
        for (i = 0; i < D_800CA050; i++) {
            key = D_800CA010[i];
            if (((D_800CA010[i] >> 16) & 0xFF) == 5) {
                e = func_80063088(D_8006794C->unk1C->unk7E4, key);
                if (e != NULL) {
                    if ((e->unkC & 0xFFFF) != (s16)D_8006794C->unk1C->unk3A) {
                        f = func_80063088(D_8006794C->unk1C->unk7E4, e->unkC);
                        if (f != NULL) {
                            func_800B3E48(e->unkC & 0xFFFFFF, 0, f->unk4, f->unk8 << 11);
                        }
                    }
                    func_800B3E48(key, 0, e->unk4, e->unk8 << 11);
                    func_800B3C24(key);
                }
            }
        }
    } else {
        best = 0;
        sel = -1;
        cur = -1;
        for (i = D_8006794C->unk1C->unk7E4->unk2 - 1; i >= 0; i--) {
            x = tbl[i].unk0;
            if (tbl[i].unk8 != 0 && ((x >> 16) & 0xFF) == 5) {
                if ((x & 0xFFFF) < 9000 && (x & 0xFFFF) != D_8006794C->unk1C->unk8E4.field->unkA && func_800B3D10(x) == NULL &&
                    func_800B3F8C_grp(x & 0xFFFF) == func_800B3F8C_grp(D_8006794C->unk1C->unkC) && best < tbl[i].unk8) {
                    best = tbl[i].unk8;
                    sel = i;
                }
            }
        }
        if (sel == -1) {
            for (i = D_8006794C->unk1C->unk7E4->unk2 - 1; i >= 0; i--) {
                x = tbl[i].unk0;
                if (tbl[i].unk8 != 0 && ((x >> 16) & 0xFF) == 5) {
                    if ((x & 0xFFFF) < 9000 && func_800B3D10(x) == NULL && (x & 0xFFFF) == D_8006794C->unk1C->unk8E4.field->unkA) {
                        best = tbl[i].unk8;
                        sel = i;
                        break;
                    }
                }
            }
        }
        if (sel == -1) {
            for (i = D_8006794C->unk1C->unk7E4->unk2 - 1; i >= 0; i--) {
                x = tbl[i].unk0;
                if (tbl[i].unk8 != 0 && ((x >> 16) & 0xFF) == 5) {
                    if ((x & 0xFFFF) < 9000 && (x & 0xFFFF) != D_8006794C->unk1C->unk8E4.field->unkA && func_800B3D10(x) == NULL &&
                        best < tbl[i].unk8) {
                        sel = i;
                        break;
                    }
                }
            }
        }
        if (sel != -1) {
            if ((tbl[sel].unkC & 0xFFFF) != (s16)D_8006794C->unk1C->unk3A) {
                for (i = D_8006794C->unk1C->unk7E4->unk2 - 1; i >= 0; i--) {
                    if (tbl[i].unk0 == tbl[sel].unkC) {
                        func_800B3E48(tbl[i].unk0 & 0xFFFFFF, 0, tbl[i].unk4, tbl[i].unk8 << 11);
                        break;
                    }
                }
            }
            cnt = tbl[sel].unk0 & 0xFFFFFF;
            func_800B3E48(cnt, 0, tbl[sel].unk4, tbl[sel].unk8 << 11);
            cur = cnt;
        }
        if (cur != -1) {
            best = 0;
            sel = -1;
            for (i = D_8006794C->unk1C->unk7E4->unk2 - 1; i >= 0; i--) {
                x = tbl[i].unk0;
                if (tbl[i].unk8 != 0 && ((x >> 16) & 0xFF) == 5) {
                    key = x & 0xFFFF;
                    if (key < 9000 && (x & 0xFFFF) != D_8006794C->unk1C->unk8E4.field->unkA && (x & 0xFFFF) != (cur & 0xFFFF) &&
                        func_800B3D10(x) == NULL && func_800B3F8C_grp(key) == func_800B3F8C_grp(D_8006794C->unk1C->unkC) &&
                        best < tbl[i].unk8) {
                        best = tbl[i].unk8;
                        sel = i;
                        break;
                    }
                }
            }
            if (sel == -1) {
                for (i = D_8006794C->unk1C->unk7E4->unk2 - 1; i >= 0; i--) {
                    x = tbl[i].unk0;
                    if (tbl[i].unk8 != 0 && ((x >> 16) & 0xFF) == 5) {
                        if ((x & 0xFFFF) < 9000 && (x & 0xFFFF) != (cur & 0xFFFF) && func_800B3D10(x) == NULL &&
                            (x & 0xFFFF) == D_8006794C->unk1C->unk8E4.field->unkA) {
                            best = tbl[i].unk8;
                            sel = i;
                            break;
                        }
                    }
                }
            }
            if (sel == -1) {
                for (i = D_8006794C->unk1C->unk7E4->unk2 - 1; i >= 0; i--) {
                    x = tbl[i].unk0;
                    if (tbl[i].unk8 != 0 && ((x >> 16) & 0xFF) == 5) {
                        if ((x & 0xFFFF) < 9000 && (x & 0xFFFF) != D_8006794C->unk1C->unk8E4.field->unkA && (x & 0xFFFF) != (cur & 0xFFFF) &&
                            func_800B3D10(x) == NULL && best < tbl[i].unk8) {
                            sel = i;
                            break;
                        }
                    }
                }
            }
            if (sel != -1) {
                if ((tbl[sel].unkC & 0xFFFF) != (s16)D_8006794C->unk1C->unk3A) {
                    for (i = D_8006794C->unk1C->unk7E4->unk2 - 1; i >= 0; i--) {
                        if (tbl[i].unk0 == tbl[sel].unkC) {
                            func_800B3E48(tbl[i].unk0 & 0xFFFFFF, 1, tbl[i].unk4, -1);
                            return;
                        }
                    }
                } else {
                    func_800B3E48(tbl[sel].unk0 & 0xFFFFFF, 1, tbl[sel].unk4, -1);
                }
            }
        }
    }
}

s32 func_800B4894(void) {
    FieldScene *s;

    if ((u32)(D_8006794C->unk1C->unk851 - 1) < 5) {
        return 1;
    }
    s = D_8006794C->unk1C->unk8E4.field->unkC->unk14;
    if (s->unk0 & 0x800000) {
        if (func_80021FB4() <= 0) {
            func_800223CC();
            if (!(D_8006794C->unk1C->unk8E4.field->unkC->unk14->unk0 & 0x100000)) {
                func_800B3B2C(D_8006794C->unk1C->unk8E4.field->unkC->unk14->unk20);
                func_800B3F8C();
                D_8006794C->unk1C->unk8E4.field->unkC->unk14->unk0 |= 0x100000;
            }
            D_8006794C->unk1C->unk8E4.field->unkC->unk14->unk0 &= ~0x800000;
        }
        return 1;
    }
    if (s->unk0 & 0x100000) {
        return 0;
    }
    if (s->unk0 & 0x10000) {
        func_800B3B2C(s->unk20);
        func_800B3F8C();
        D_8006794C->unk1C->unk8E4.field->unkC->unk14->unk0 |= 0x100000;
    }
    return 0;
}

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

void func_800B5114(void) {
    s32 i;

    DrawSync(0);
    for (i = 0; i < 2; i++) {
        D_800CA058.unk8[i] = D_80073998[i];
        D_80073998[i] = 0x04000300;
    }
    D_800CA058.unk0 = 1;
    D_800CA058.unk3 = 0;
    D_8006794C->unk1C->unk8E4.field->unk0 = 0;
    func_8002DE60();
    D_800C9D5C[D_800CA058.unk0].unk0();
}

/* leave the current field mode */
void func_800B51D0(void) {
    FieldMap *f;
    s32 i;

    if (D_800C9D5C[D_800CA058.unk0].unk8 != NULL) {
        D_800C9D5C[D_800CA058.unk0].unk8();
    }
    DrawSync(0);
    for (i = 0; i < 2; i++) {
        D_80073998[i] = D_800CA058.unk8[i];
    }
    f = D_8006794C->unk1C->unk8E4.field;
    f->unk0 |= 0x10000;
    func_8001DE50((GameState *)f);
    func_800A9570();
}

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

void func_800B532C(void) {
    func_80032100();
    func_80023A0C();
    func_80032098();
    func_80023A4C();
    DrawSync(0);
    VSync(0);
    ResetGraph(1);
    PutDispEnv(&D_8006794C->unkC[D_8006794C->unk1C->unk8]);
    DrawOTagEnv(&D_8006794C->unk1C->unk10[D_8006794C->unk1C->unk8][0xFFF], &D_8006794C->unk10[D_8006794C->unk1C->unk8]);
    D_8006794C->unk1C->unk8 ^= 1;
    func_8002C0DC(D_8006794C->unk1C->unk8);
}

void func_800B5430(s32 arg0) {
    if (arg0 != D_800CA058.unk0) {
        if (D_800C9D5C[D_800CA058.unk0].unk8 != NULL) {
            D_800C9D5C[D_800CA058.unk0].unk8();
        }
        D_800CA058.unk0 = arg0;
        if (D_800C9D5C[D_800CA058.unk0].unk0 != NULL) {
            D_800C9D5C[D_800CA058.unk0].unk0();
        }
    }
}

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

void func_800B559C(void) {
    if (!func_8002F810()) {
        if (D_80077528 & 0x50) {
            func_8002F488(0x67);
            D_800CA058.unk3 ^= 1;
        } else if (D_80077528 & 0x10000) {
            if (D_800CA058.unk3 == 0) {
                func_8002F488(0x67);
                D_800CA058.unk3 = 1;
            }
        } else if (D_80077528 & 0x20000) {
            func_8002F488(0x67);
            func_8002F9F0();
            func_8002F8A8(func_800B5794);
        }
    }
}

/* DISC CHANGE label */
INCLUDE_RODATA("asm/us/ovl_0a/nonmatchings/ovl_0a", D_800A7170);

void func_800B5660(void) {
    s32 y;
    Unk80077DE8 *m;

    func_8002B524(D_800C9D98.x + 8, D_800C9D98.y - 3, 0xA, D_800A7170, 0);
    func_8002B5FC(D_800C9D98.x + 0xE, D_800C9D98.y + 0xA, 0xA, func_80035774(0), 0);
    func_8002B5FC(D_800C9D98.x + 0x30, D_800C9D98.y + 0x2E, 0xA, func_80035774(1), 0);
    func_8002B5FC(D_800C9D98.x + 0x30, D_800C9D98.y + 0x3D, 0xA, func_80035774(2), 0);
    if (!func_8002F810()) {
        m = &D_80077DE8;
        m->unk0 = D_800C9D98.x + 0x16;
        y = D_800C9D98.y;
        if (D_800CA058.unk3 == 0) {
            m->unk2 = y + 0x32;
        } else {
            m->unk2 = y + 0x41;
        }
        D_80077DEC = 9;
        func_800355E4();
    }
    func_8002DEE4(&D_800C9D98, 0xA, 2);
}

void func_800B5794(void) {
    if (D_800CA058.unk3 == 0) {
        func_800B5430(2);
    } else {
        func_800B5430(3);
    }
}

void func_800B57C8(void) {
    RECT r;
    s32 id;
    Unk80077DE8 *m = &D_80077DE8;

    m->unk8 = NULL;
    if (!func_8002F810()) {
        if (D_800CA058.unk3 == 0) {
            func_80035580(NULL);
            id = 3;
        } else {
            r.x = 0;
            r.y = m->unk2;
            r.w = D_8006794C->unkC->disp.w;
            r.h = D_8006794C->unkC->disp.h - m->unk2;
            func_80035580(&r);
            id = 4;
        }
        m->unk8 = func_80035774(id);
    }
}

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

void func_800B61A4(Unk800C95C8 *arg0, u8 arg1) {
    MATRIX m;
    WmModelGroup *g;
    MATRIX *bone;
    SVECTOR *vtx;
    s32 *sxy;
    s32 *sz;
    s32 k;
    u32 i;
    s32 n;
    u32 cnt;
    MATRIX *b;
    SVECTOR *v;
    s16 *c1;
    s16 *c2;

    g = &arg0->unk8->unk10[arg1];
    gte_SetGeomScreen(D_8006794C->unk1C->unkA);
    sxy = D_800711A8;
    sz = D_80070520;
    k = 0;
    cnt = arg0->unk8->unk2;
    bone = (MATRIX *)arg0->unk20;
    for (i = 0; i < cnt; bone++) {
        n = ((u16 *)g->unk14)[i++];
        if (n != 0) {
            gte_SetRotMatrix(&D_8006794C->unk1C->unk18);
            gte_SetTransMatrix(&D_8006794C->unk1C->unk18);
            b = bone;
            gte_ldclmv(b);
            c1 = &b->m[0][1];
            c2 = &b->m[0][2];
            gte_rtir_b();
            sxy = &D_800711A8[k];
            gte_stclmv(&m);
            gte_ldclmv(c1);
            gte_rtir();
            sz = &D_80070520[k];
            gte_stclmv(&m.m[0][1]);
            gte_ldclmv(c2);
            c1 = (s16 *)b->t;
            gte_rtir_n1();
            vtx = &((SVECTOR *)g->unk18)[k];
            gte_stclmv(&m.m[0][2]);
            gte_ldlv0(c1);
            k += n;
            gte_rtv0tr_n1();
            gte_ldv3c(vtx);
            gte_stlvnl(m.t);
            gte_SetRotMatrix(&m);
            gte_SetTransMatrix(&m);
            v = vtx;
            do {
                v += 3;
                n -= 3;
                gte_rtpt_b();
                gte_ldv3c(v);
                gte_stsxy3c(sxy);
                gte_stsz3c(sz);
                sxy += 3;
                sz += 3;
            } while (n > 0);
        }
    }
}

/* attach a model file (relocating it once) and lay out its work buffers at arg1 */
void func_800B6438(Unk800C95C8 *arg0, u8 *arg1) {
    WmModel *m;
    WmModelGroup *g;
    u32 i;
    u32 n;
    u32 k;
    s32 size;
    u8 *pp;
    WmModelGroup *p;
    WmModelGroup *g2;

    m = arg0->unk8;
    if (!(m->unk0 & 1)) {
        i = 0;
        m->unk0 |= 1;
        m->unkC = (s32)m + m->unkC;
        g = (WmModelGroup *)((u8 *)m + (s32)m->unk10);
        m->unk10 = g;
        for (p = g; i < m->unk3; i++, p++) {
            p->unk14 = (u8 *)m + (s32)p->unk14;
            p->unk18 = (u8 *)m + (s32)p->unk18;
            p->unk1C = (u8 *)m + (s32)p->unk1C;
            p->unk20 = (void *)((u8 *)m + (s32)p->unk20);
            p->unk24 = (void *)((u8 *)m + (s32)p->unk24);
        }
    }
    g2 = arg0->unk8->unk10;
    size = 0;
    if ((s32)g2 >= 0) {
        g2 = (WmModelGroup *)((u8 *)g2 + (s32)arg0->unk8);
    }
    n = arg0->unk8->unk3;
    for (k = 0; k < n; k++) {
        size += g2->unk0;
        g2++;
    }
    arg0->unk54 = 0x1000;
    arg0->unk56 = 0x1000;
    arg0->unk58 = 0x1000;
    arg0->unk1C = NULL;
    arg0->unk60 = NULL;
    arg0->unk5C = 0;
    arg0->unk14 = 0;
    arg0->unk74 = 0;
    arg0->unk76 = 0;
    arg0->unk78 = 0;
    arg0->unk10 = arg1;
    arg0->unk50 = NULL;
    pp = arg1 + size * 2;
    arg0->unk20 = pp;
    arg0->unk0 |= 2;
    *(volatile u16 *)&arg0->unk0 = 0;
    arg0->unk44 = (WmModelOut *)(pp + m->unk2 * 32);
}

void func_800B6584(Unk800C95C8 *arg0, WmTexInfo *arg1, s16 (*arg2)[2]) {
    WmModel *m;
    WmModelGroup *g;
    WmModelOut *out;
    WmModelOut *o;
    u8 *prim;
    u8 *src;
    u8 (*col)[4];
    u8 *c;
    u32 k;
    u32 j;
    u32 i;
    s32 du;
    s32 dv;
    u16 tp;
    POLY_FT4 *pFT4;
    WmSrcFT4 *sFT4;
    POLY_FT3 *pFT3;
    WmSrcFT3 *sFT3;
    POLY_GT4 *pGT4;
    WmSrcGT4 *sGT4;
    POLY_GT3 *pGT3;
    WmSrcGT3 *sGT3;
    POLY_G4 *pG4;
    WmSrcG4 *sG4;
    POLY_G3 *pG3;
    WmSrcG3 *sG3;
    POLY_F4 *pF4;
    WmSrcF4 *sF4;
    POLY_F3 *pF3;
    WmSrcF3 *sF3;

    prim = arg0->unk10;
    m = arg0->unk8;
    arg0->unk18 = arg1;
    o = arg0->unk44;
    for (k = 0; k < m->unk3; k++) {
        out = &o[k];
        g = &m->unk10[k];
        col = g->unk24;
        out->unk4 = prim;
        out->unk0 = 0;
        out->unk8 = 0;
        for (j = 0; j < 2; j++) {
            src = g->unk1C;
            pFT4 = (POLY_FT4 *)prim;
            sFT4 = (WmSrcFT4 *)src;
            for (i = 0; i < g->unk2; i++) {
                setPolyFT4(pFT4);
                pFT4->r0 = sFT4->unk10[0];
                pFT4->g0 = sFT4->unk10[1];
                pFT4->b0 = sFT4->unk10[2];
                pFT4->u0 = g->unk20[sFT4->unk8[0]][0];
                pFT4->v0 = g->unk20[sFT4->unk8[0]][1];
                pFT4->u1 = g->unk20[sFT4->unk8[1]][0];
                pFT4->v1 = g->unk20[sFT4->unk8[1]][1];
                pFT4->u2 = g->unk20[sFT4->unk8[2]][0];
                pFT4->v2 = g->unk20[sFT4->unk8[2]][1];
                pFT4->u3 = g->unk20[sFT4->unk8[3]][0];
                pFT4->v3 = g->unk20[sFT4->unk8[3]][1];
                pFT4->clut = arg0->unk18[sFT4->unk13].unk2;
                pFT4->tpage = tp = arg0->unk18[sFT4->unk13].unk0 & ~0x20;
                if (sFT4->unk15 & 1) {
                    pFT4->tpage = tp | (sFT4->unk15 & 0xE0);
                    setSemiTrans(pFT4, 1);
                }
                prim += sizeof(POLY_FT4);
                du = arg2[sFT4->unk13][0];
                dv = arg2[sFT4->unk13][1];
                pFT4->u0 += du;
                pFT4->v0 += dv;
                pFT4->u1 += du;
                pFT4->v1 += dv;
                pFT4->u2 += du;
                pFT4->v2 += dv;
                pFT4->u3 += du;
                pFT4->v3 += dv;
                pFT4++;
                sFT4++;
                src += sizeof(WmSrcFT4);
            }
            pFT3 = (POLY_FT3 *)prim;
            sFT3 = (WmSrcFT3 *)src;
            for (i = 0; i < g->unk4; i++) {
                setPolyFT3(pFT3);
                pFT3->r0 = sFT3->unk8[0];
                pFT3->g0 = sFT3->unk8[1];
                pFT3->b0 = sFT3->unk8[2];
                pFT3->u0 = g->unk20[sFT3->unkC[0]][0];
                pFT3->v0 = g->unk20[sFT3->unkC[0]][1];
                pFT3->u1 = g->unk20[sFT3->unkC[1]][0];
                pFT3->v1 = g->unk20[sFT3->unkC[1]][1];
                pFT3->u2 = g->unk20[sFT3->unkC[2]][0];
                pFT3->v2 = g->unk20[sFT3->unkC[2]][1];
                pFT3->clut = arg0->unk18[sFT3->unk6].unk2;
                pFT3->tpage = tp = arg0->unk18[sFT3->unk6].unk0 & ~0x20;
                if (sFT3->unk12 & 1) {
                    pFT3->tpage = tp | (sFT3->unk12 & 0xE0);
                    setSemiTrans(pFT3, 1);
                }
                prim += sizeof(POLY_FT3);
                du = arg2[sFT3->unk6][0];
                dv = arg2[sFT3->unk6][1];
                pFT3->u0 += du;
                pFT3->v0 += dv;
                pFT3->u1 += du;
                pFT3->v1 += dv;
                pFT3->u2 += du;
                pFT3->v2 += dv;
                pFT3++;
                sFT3++;
                src += sizeof(WmSrcFT3);
            }
            pGT4 = (POLY_GT4 *)prim;
            sGT4 = (WmSrcGT4 *)src;
            for (i = 0; i < g->unk6; i++) {
                setPolyGT4(pGT4);
                c = col[sGT4->unk10[0]];
                pGT4->r0 = c[0];
                pGT4->g0 = c[1];
                pGT4->b0 = c[2];
                c = col[sGT4->unk10[1]];
                pGT4->r1 = c[0];
                pGT4->g1 = c[1];
                pGT4->b1 = c[2];
                c = col[sGT4->unk10[2]];
                pGT4->r2 = c[0];
                pGT4->g2 = c[1];
                pGT4->b2 = c[2];
                c = col[sGT4->unk10[3]];
                pGT4->r3 = c[0];
                pGT4->g3 = c[1];
                pGT4->b3 = c[2];
                pGT4->u0 = g->unk20[sGT4->unk8[0]][0];
                pGT4->v0 = g->unk20[sGT4->unk8[0]][1];
                pGT4->u1 = g->unk20[sGT4->unk8[1]][0];
                pGT4->v1 = g->unk20[sGT4->unk8[1]][1];
                pGT4->u2 = g->unk20[sGT4->unk8[2]][0];
                pGT4->v2 = g->unk20[sGT4->unk8[2]][1];
                pGT4->u3 = g->unk20[sGT4->unk8[3]][0];
                pGT4->v3 = g->unk20[sGT4->unk8[3]][1];
                pGT4->clut = arg0->unk18[sGT4->unk1D].unk2;
                pGT4->tpage = tp = arg0->unk18[sGT4->unk1D].unk0 & ~0x20;
                if (sGT4->unk1C & 1) {
                    pGT4->tpage = tp | (sGT4->unk1C & 0xE0);
                    setSemiTrans(pGT4, 1);
                }
                prim += sizeof(POLY_GT4);
                du = arg2[sGT4->unk1D][0];
                dv = arg2[sGT4->unk1D][1];
                pGT4->u0 += du;
                pGT4->v0 += dv;
                pGT4->u1 += du;
                pGT4->v1 += dv;
                pGT4->u2 += du;
                pGT4->v2 += dv;
                pGT4->u3 += du;
                pGT4->v3 += dv;
                pGT4++;
                sGT4++;
                src += sizeof(WmSrcGT4);
            }
            pGT3 = (POLY_GT3 *)prim;
            sGT3 = (WmSrcGT3 *)src;
            for (i = 0; i < g->unk8; i++) {
                setPolyGT3(pGT3);
                c = col[sGT3->unk10[0]];
                pGT3->r0 = c[0];
                pGT3->g0 = c[1];
                pGT3->b0 = c[2];
                c = col[sGT3->unk10[1]];
                pGT3->r1 = c[0];
                pGT3->g1 = c[1];
                pGT3->b1 = c[2];
                c = col[sGT3->unk10[2]];
                pGT3->r2 = c[0];
                pGT3->g2 = c[1];
                pGT3->b2 = c[2];
                pGT3->u0 = g->unk20[sGT3->unk6[0]][0];
                pGT3->v0 = g->unk20[sGT3->unk6[0]][1];
                pGT3->u1 = g->unk20[sGT3->unk6[1]][0];
                pGT3->v1 = g->unk20[sGT3->unk6[1]][1];
                pGT3->u2 = g->unk20[sGT3->unk6[2]][0];
                pGT3->v2 = g->unk20[sGT3->unk6[2]][1];
                pGT3->clut = arg0->unk18[sGT3->unk16].unk2;
                pGT3->tpage = tp = arg0->unk18[sGT3->unk16].unk0 & ~0x20;
                if (sGT3->unkF & 1) {
                    pGT3->tpage = tp | (sGT3->unkF & 0xE0);
                    setSemiTrans(pGT3, 1);
                }
                prim += sizeof(POLY_GT3);
                du = arg2[sGT3->unk16][0];
                dv = arg2[sGT3->unk16][1];
                pGT3->u0 += du;
                pGT3->v0 += dv;
                pGT3->u1 += du;
                pGT3->v1 += dv;
                pGT3->u2 += du;
                pGT3->v2 += dv;
                pGT3++;
                sGT3++;
                src += sizeof(WmSrcGT3);
            }
            pG4 = (POLY_G4 *)prim;
            sG4 = (WmSrcG4 *)src;
            for (i = 0; i < g->unkA; i++) {
                setPolyG4(pG4);
                c = col[sG4->unk8[0]];
                pG4->r0 = c[0];
                pG4->g0 = c[1];
                pG4->b0 = c[2];
                c = col[sG4->unk8[1]];
                pG4->r1 = c[0];
                pG4->g1 = c[1];
                pG4->b1 = c[2];
                c = col[sG4->unk8[2]];
                pG4->r2 = c[0];
                pG4->g2 = c[1];
                pG4->b2 = c[2];
                c = col[sG4->unk8[3]];
                pG4->r3 = c[0];
                pG4->g3 = c[1];
                pG4->b3 = c[2];
                prim += sizeof(POLY_G4);
                pG4++;
                sG4++;
                src += sizeof(WmSrcG4);
            }
            pG3 = (POLY_G3 *)prim;
            sG3 = (WmSrcG3 *)src;
            for (i = 0; i < g->unkC; i++) {
                setPolyG3(pG3);
                c = col[sG3->unk8[0]];
                pG3->r0 = c[0];
                pG3->g0 = c[1];
                pG3->b0 = c[2];
                c = col[sG3->unk8[1]];
                pG3->r1 = c[0];
                pG3->g1 = c[1];
                pG3->b1 = c[2];
                c = col[sG3->unk8[2]];
                pG3->r2 = c[0];
                pG3->g2 = c[1];
                pG3->b2 = c[2];
                prim += sizeof(POLY_G3);
                pG3++;
                sG3++;
                src += sizeof(WmSrcG3);
            }
            pF4 = (POLY_F4 *)prim;
            sF4 = (WmSrcF4 *)src;
            for (i = 0; i < g->unkE; i++) {
                setPolyF4(pF4);
                pF4->r0 = sF4->unk8[0];
                pF4->g0 = sF4->unk8[1];
                pF4->b0 = sF4->unk8[2];
                prim += sizeof(POLY_F4);
                pF4++;
                sF4++;
                src += sizeof(WmSrcF4);
            }
            for (i = 0; i < g->unk10; i++) {
                pF3 = (POLY_F3 *)prim;
                sF3 = (WmSrcF3 *)src;
                setPolyF3(pF3);
                pF3->r0 = sF3->unk8[0];
                pF3->g0 = sF3->unk8[1];
                pF3->b0 = sF3->unk8[2];
                prim += sizeof(POLY_F3);
                src += sizeof(WmSrcF3);
            }
        }
    }
}

void func_800B71E0(Unk800C95C8 *arg0, u8 arg1) {
    WmModelGroup *g;
    u32 *ot;
    s32 opz;
    s32 otz;
    u32 n;
    u32 i;
    s32 xy0, xy1, xy2;
    u32 a, b;
    u32 i0, i1, i2, i3;
    u8 *prim;
    WmSrcFT4 *sFT4;
    POLY_FT4 *pFT4;
    WmSrcFT3 *sFT3;
    POLY_FT3 *pFT3;
    WmSrcGT4 *sGT4;
    POLY_GT4 *pGT4;
    WmSrcGT3 *sGT3;
    POLY_GT3 *pGT3;
    WmSrcG4 *sG4;
    POLY_G4 *pG4;
    WmSrcG3 *sG3;
    POLY_G3 *pG3;
    WmSrcF4 *sF4;
    POLY_F4 *pF4;
    WmSrcF3 *sF3;
    POLY_F3 *pF3;

    ot = D_8006794C->unk1C->unk10[D_8006794C->unk1C->unk8];
    g = &arg0->unk8->unk10[arg1];
    if (D_8006794C->unk1C->unk8 != 0) {
        prim = (u8 *)arg0->unk44[arg1].unk4 + g->unk0;
    } else {
        prim = arg0->unk44[arg1].unk4;
    }

    n = g->unk2;
    pFT4 = (POLY_FT4 *)prim;
    ot -= g->unk13;
    sFT4 = (WmSrcFT4 *)(void *)g->unk1C;
    for (i = 0; i < n; sFT4++, pFT4++) {
        a = *(u32 *)&sFT4->unk0[0];
        b = *(u32 *)&sFT4->unk0[2];
        i1 = a >> 16;
        i3 = b >> 16;
        i0 = a & 0xFFFF;
        i2 = b & 0xFFFF;
        xy0 = D_800711A8[i0];
        xy1 = D_800711A8[i1];
        xy2 = D_800711A8[i2];
        i++;
        if (!(sFT4->unk15 & 2)) {
            gte_ldsxy3(xy0, xy1, xy2);
            setaddr(pFT4, 0);
            gte_nclip_b();
            gte_stopz(&opz);
            if (opz <= 0) {
                continue;
            }
        }
        otz = (D_80070520[i0] + D_80070520[i1] + D_80070520[i2] + D_80070520[i3]) >> 4;
        *(s32 *)&pFT4->x0 = xy0;
        *(s32 *)&pFT4->x1 = xy1;
        *(s32 *)&pFT4->x2 = xy2;
        *(s32 *)&pFT4->x3 = D_800711A8[i3];
        addPrim(&ot[otz], pFT4);
    }

    n = g->unk4;
    pFT3 = (POLY_FT3 *)pFT4;
    sFT3 = (WmSrcFT3 *)sFT4;
    for (i = 0; i < n; sFT3++, pFT3++) {
        a = *(u32 *)&sFT3->unk0[0];
        i2 = sFT3->unk0[2];
        i1 = a >> 16;
        i0 = a & 0xFFFF;
        xy0 = D_800711A8[i0];
        xy1 = D_800711A8[i1];
        xy2 = D_800711A8[i2];
        i++;
        if (!(sFT3->unk12 & 2)) {
            gte_ldsxy3(xy0, xy1, xy2);
            setaddr(pFT3, 0);
            gte_nclip_b();
            gte_stopz(&opz);
            if (opz <= 0) {
                continue;
            }
        }
        gte_ldsz3(D_80070520[i0], D_80070520[i1], D_80070520[i2]);
        *(s32 *)&pFT3->x0 = xy0;
        *(s32 *)&pFT3->x1 = xy1;
        gte_avsz3_b();
        *(s32 *)&pFT3->x2 = xy2;
        gte_stotz(&otz);
        addPrim(&ot[otz], pFT3);
    }

    n = g->unk6;
    pGT4 = (POLY_GT4 *)pFT3;
    sGT4 = (WmSrcGT4 *)sFT3;
    for (i = 0; i < n; sGT4++, pGT4++) {
        a = *(u32 *)&sGT4->unk0[0];
        b = *(u32 *)&sGT4->unk0[2];
        i1 = a >> 16;
        i3 = b >> 16;
        i0 = a & 0xFFFF;
        i2 = b & 0xFFFF;
        xy0 = D_800711A8[i0];
        xy1 = D_800711A8[i1];
        xy2 = D_800711A8[i2];
        i++;
        if (!(sGT4->unk1C & 2)) {
            gte_ldsxy3(xy0, xy1, xy2);
            setaddr(pGT4, 0);
            gte_nclip_b();
            gte_stopz(&opz);
            if (opz <= 0) {
                continue;
            }
        }
        otz = D_80070520[i0] + D_80070520[i1] + D_80070520[i2] + D_80070520[i3];
        *(s32 *)&pGT4->x0 = xy0;
        *(s32 *)&pGT4->x1 = xy1;
        *(s32 *)&pGT4->x2 = xy2;
        *(s32 *)&pGT4->x3 = D_800711A8[i3];
        otz >>= 4;
        addPrim(&ot[otz], pGT4);
    }

    n = g->unk8;
    pGT3 = (POLY_GT3 *)pGT4;
    sGT3 = (WmSrcGT3 *)sGT4;
    for (i = 0; i < n; sGT3++, pGT3++) {
        a = *(u32 *)&sGT3->unk0[0];
        i2 = sGT3->unk0[2];
        i1 = a >> 16;
        i0 = a & 0xFFFF;
        xy0 = D_800711A8[i0];
        xy1 = D_800711A8[i1];
        xy2 = D_800711A8[i2];
        i++;
        if (!(sGT3->unkF & 2)) {
            gte_ldsxy3(xy0, xy1, xy2);
            setaddr(pGT3, 0);
            gte_nclip_b();
            gte_stopz(&opz);
            if (opz <= 0) {
                continue;
            }
        }
        gte_ldsz3(D_80070520[i0], D_80070520[i1], D_80070520[i2]);
        *(s32 *)&pGT3->x0 = xy0;
        *(s32 *)&pGT3->x1 = xy1;
        gte_avsz3_b();
        *(s32 *)&pGT3->x2 = xy2;
        gte_stotz(&otz);
        addPrim(&ot[otz], pGT3);
    }

    n = g->unkA;
    pG4 = (POLY_G4 *)pGT3;
    sG4 = (WmSrcG4 *)sGT3;
    for (i = 0; i < n; sG4++, pG4++) {
        a = *(u32 *)&sG4->unk0[0];
        b = *(u32 *)&sG4->unk0[2];
        i1 = a >> 16;
        i3 = b >> 16;
        i0 = a & 0xFFFF;
        i2 = b & 0xFFFF;
        xy0 = D_800711A8[i0];
        xy1 = D_800711A8[i1];
        xy2 = D_800711A8[i2];
        i++;
        if (!(sG4->unk14 & 2)) {
            gte_ldsxy3(xy0, xy1, xy2);
            setaddr(pG4, 0);
            gte_nclip_b();
            gte_stopz(&opz);
            if (opz <= 0) {
                continue;
            }
        }
        otz = (D_80070520[i0] + D_80070520[i1] + D_80070520[i2] + D_80070520[i3]) >> 4;
        *(s32 *)&pG4->x0 = xy0;
        *(s32 *)&pG4->x1 = xy1;
        *(s32 *)&pG4->x2 = xy2;
        *(s32 *)&pG4->x3 = D_800711A8[i3];
        addPrim(&ot[otz], pG4);
    }

    n = g->unkC;
    pG3 = (POLY_G3 *)pG4;
    sG3 = (WmSrcG3 *)sG4;
    for (i = 0; i < n; sG3++, pG3++) {
        a = *(u32 *)&sG3->unk0[0];
        i2 = sG3->unk0[2];
        i1 = a >> 16;
        i0 = a & 0xFFFF;
        xy0 = D_800711A8[i0];
        xy1 = D_800711A8[i1];
        xy2 = D_800711A8[i2];
        i++;
        if (!(sG3->unk13 & 2)) {
            gte_ldsxy3(xy0, xy1, xy2);
            setaddr(pG3, 0);
            gte_nclip_b();
            gte_stopz(&opz);
            if (opz <= 0) {
                continue;
            }
        }
        gte_ldsz3(D_80070520[i0], D_80070520[i1], D_80070520[i2]);
        *(s32 *)&pG3->x0 = xy0;
        *(s32 *)&pG3->x1 = xy1;
        gte_avsz3_b();
        *(s32 *)&pG3->x2 = xy2;
        gte_stotz(&otz);
        addPrim(&ot[otz], pG3);
    }

    n = g->unkE;
    pF4 = (POLY_F4 *)pG3;
    sF4 = (WmSrcF4 *)sG3;
    for (i = 0; i < n; sF4++, pF4++) {
        a = *(u32 *)&sF4->unk0[0];
        b = *(u32 *)&sF4->unk0[2];
        i1 = a >> 16;
        i3 = b >> 16;
        i0 = a & 0xFFFF;
        i2 = b & 0xFFFF;
        xy0 = D_800711A8[i0];
        xy1 = D_800711A8[i1];
        xy2 = D_800711A8[i2];
        i++;
        if (!(sF4->unkD & 2)) {
            gte_ldsxy3(xy0, xy1, xy2);
            setaddr(pF4, 0);
            gte_nclip_b();
            gte_stopz(&opz);
            if (opz <= 0) {
                continue;
            }
        }
        otz = D_80070520[i0] + D_80070520[i1] + D_80070520[i2] + D_80070520[i3];
        *(s32 *)&pF4->x0 = xy0;
        *(s32 *)&pF4->x1 = xy1;
        *(s32 *)&pF4->x2 = xy2;
        *(s32 *)&pF4->x3 = D_800711A8[i3];
        otz >>= 4;
        addPrim(&ot[otz], pF4);
    }

    n = g->unk10;
    pF3 = (POLY_F3 *)pF4;
    sF3 = (WmSrcF3 *)sF4;
    for (i = 0; i < n; sF3++, pF3++) {
        a = *(u32 *)&sF3->unk0[0];
        i2 = sF3->unk0[2];
        i1 = a >> 16;
        i0 = a & 0xFFFF;
        xy0 = D_800711A8[i0];
        xy1 = D_800711A8[i1];
        xy2 = D_800711A8[i2];
        i++;
        if (!(sF3->unkB & 2)) {
            gte_ldsxy3(xy0, xy1, xy2);
            setaddr(pF3, 0);
            gte_nclip_b();
            gte_stopz(&opz);
            if (opz <= 0) {
                continue;
            }
        }
        gte_ldsz3(D_80070520[i0], D_80070520[i1], D_80070520[i2]);
        *(s32 *)&pF3->x0 = xy0;
        *(s32 *)&pF3->x1 = xy1;
        gte_avsz3_b();
        *(s32 *)&pF3->x2 = xy2;
        gte_stotz(&otz);
        addPrim(&ot[otz], pF3);
    }
}

void func_800B7CEC(Unk800C95C8 *arg0) {
    MATRIX m;

    if (arg0->unk0 & 2) {
        *(s32 *)&m.m[0][0] = 0x1000;
        *(s32 *)&m.m[0][2] = 0;
        *(s32 *)&m.m[1][1] = 0x1000;
        *(s32 *)&m.m[2][0] = 0;
        *(s32 *)&m.m[2][2] = 0x1000;
        m.m[0][0] = arg0->unk54;
        gte_SetRotMatrix(&arg0->unk24);
        gte_ldclmv(&m);
        gte_rtir();
        m.m[1][1] = arg0->unk56;
        gte_stclmv(&arg0->unk24);
        gte_ldclmv(&m.m[0][1]);
        gte_rtir();
        m.m[2][2] = arg0->unk58;
        gte_stclmv(&arg0->unk24.m[0][1]);
        gte_ldclmv(&m.m[0][2]);
        gte_rtir();
        gte_stclmv(&arg0->unk24.m[0][2]);
    }
}

void func_800B7E30(Unk800C95C8 *arg0) {
    SVECTOR v;
    s32 sz;

    arg0->unk0 &= ~0x10;
    if (arg0->unk1C != NULL) {
        arg0->unk0 |= arg0->unk1C->unk0 & 0x10;
    } else {
        v.vx = arg0->unk24.t[0];
        v.vy = arg0->unk24.t[1];
        v.vz = arg0->unk24.t[2];
        gte_SetRotMatrix(&D_8006794C->unk1C->unk18);
        gte_SetTransMatrix(&D_8006794C->unk1C->unk18);
        gte_ldv0(&v);
        gte_rtps();
        gte_stsz(&sz);
        if (sz == 0) {
            arg0->unk0 |= 0x10;
        }
    }
}

void func_800B7F14(Unk800C95C8 *arg0, u16 arg1) {
    SVECTOR sv;
    u16 nb;
    VECTOR *vec;
    WmAnim *anim;
    MATRIX *bones;
    MATRIX *m;
    MATRIX *end;
    MATRIX *p;
    WmAnimKey *key;
    WmAnimKey *key2;
    WmBone *b;
    SVECTOR *rot;
    s16 *c1;
    s16 *c2;
    s32 w0, w1, w2, w3;

    anim = (WmAnim *)arg0->unkC;
    if (anim == NULL) {
        if (arg0->unk1C != NULL) {
            /* attached to a bone of the parent model */
            w0 = ((s32 *)&((MATRIX *)arg0->unk1C->unk20)[arg0->unk4])[0];
            w1 = ((s32 *)&((MATRIX *)arg0->unk1C->unk20)[arg0->unk4])[1];
            w2 = ((s32 *)&((MATRIX *)arg0->unk1C->unk20)[arg0->unk4])[2];
            w3 = ((s32 *)&((MATRIX *)arg0->unk1C->unk20)[arg0->unk4])[3];
            ((s32 *)arg0->unk20)[0] = w0;
            ((s32 *)arg0->unk20)[1] = w1;
            ((s32 *)arg0->unk20)[2] = w2;
            ((s32 *)arg0->unk20)[3] = w3;
            w0 = ((s32 *)&((MATRIX *)arg0->unk1C->unk20)[arg0->unk4])[4];
            w1 = ((s32 *)&((MATRIX *)arg0->unk1C->unk20)[arg0->unk4])[5];
            w2 = ((s32 *)&((MATRIX *)arg0->unk1C->unk20)[arg0->unk4])[6];
            w3 = ((s32 *)&((MATRIX *)arg0->unk1C->unk20)[arg0->unk4])[7];
            ((s32 *)arg0->unk20)[4] = w0;
            ((s32 *)arg0->unk20)[5] = w1;
            ((s32 *)arg0->unk20)[6] = w2;
            ((s32 *)arg0->unk20)[7] = w3;
            gte_SetRotMatrix(arg0->unk20);
            gte_ldv0(&arg0->unk74);
            gte_rtv0();
            gte_stsv(&sv);
            ((MATRIX *)arg0->unk20)->t[0] += sv.vx;
            ((MATRIX *)arg0->unk20)->t[1] += sv.vy;
            ((MATRIX *)arg0->unk20)->t[2] += sv.vz;
        } else {
            ((MATRIX *)arg0->unk20)->m[0][0] = arg0->unk24.m[0][0];
            ((MATRIX *)arg0->unk20)->m[0][1] = -arg0->unk24.m[0][1];
            ((MATRIX *)arg0->unk20)->m[0][2] = -arg0->unk24.m[0][2];
            ((MATRIX *)arg0->unk20)->m[1][0] = arg0->unk24.m[1][0];
            ((MATRIX *)arg0->unk20)->m[1][1] = -arg0->unk24.m[1][1];
            ((MATRIX *)arg0->unk20)->m[1][2] = -arg0->unk24.m[1][2];
            ((MATRIX *)arg0->unk20)->m[2][0] = arg0->unk24.m[2][0];
            ((MATRIX *)arg0->unk20)->m[2][1] = -arg0->unk24.m[2][1];
            ((MATRIX *)arg0->unk20)->m[2][2] = -arg0->unk24.m[2][2];
            ((MATRIX *)arg0->unk20)->t[0] = arg0->unk24.t[0];
            ((MATRIX *)arg0->unk20)->t[1] = arg0->unk24.t[1];
            ((MATRIX *)arg0->unk20)->t[2] = arg0->unk24.t[2];
        }
        return;
    }
    nb = arg0->unk8->unk2;
    bones = (MATRIX *)0x1F800018;
    if (nb >= 31) {
        bones = (MATRIX *)arg0->unk20;
    }
    m = bones;
    vec = (VECTOR *)0x1F800008;
    vec->vx = 0;
    vec->vy = 0;
    key = anim->unkC;
    key2 = anim->unk10;
    rot = (SVECTOR *)0x1F800000;
    if (anim->unkA & 1) {
        bones->t[0] = anim->unk4[0];
    } else {
        bones->t[0] = ((s16 *)((u8 *)anim + (u16)anim->unk4[0]))[arg1];
    }
    if (anim->unkA & 2) {
        m->t[1] = anim->unk4[1];
    } else {
        m->t[1] = ((s16 *)((u8 *)anim + (u16)anim->unk4[1]))[arg1];
    }
    if (anim->unkA & 4) {
        m->t[2] = anim->unk4[2];
    } else {
        m->t[2] = ((s16 *)((u8 *)anim + (u16)anim->unk4[2]))[arg1];
    }
    end = &bones[nb];
    do {
        gte_ldrotkey(key, anim, arg1, rot);
        if (key2 != NULL) {
            gte_ldrotkey_lo(key2, anim, arg1, rot);
            key2++;
        }
        key++;
        func_80031264(rot, m);
        m++;
    } while (m < end);
    if (arg0->unk1C != NULL) {
        {
            s32 x0, x1, x2, x3;

            x0 = ((s32 *)&((MATRIX *)arg0->unk1C->unk20)[arg0->unk4])[0];
            x1 = ((s32 *)&((MATRIX *)arg0->unk1C->unk20)[arg0->unk4])[1];
            x2 = ((s32 *)&((MATRIX *)arg0->unk1C->unk20)[arg0->unk4])[2];
            x3 = ((s32 *)&((MATRIX *)arg0->unk1C->unk20)[arg0->unk4])[3];
            ((s32 *)bones)[0] = x0;
            ((s32 *)bones)[1] = x1;
            ((s32 *)bones)[2] = x2;
            ((s32 *)bones)[3] = x3;
        }
        {
            s32 x0, x1, x2, x3;

            x0 = ((s32 *)&((MATRIX *)arg0->unk1C->unk20)[arg0->unk4])[4];
            x1 = ((s32 *)&((MATRIX *)arg0->unk1C->unk20)[arg0->unk4])[5];
            x2 = ((s32 *)&((MATRIX *)arg0->unk1C->unk20)[arg0->unk4])[6];
            x3 = ((s32 *)&((MATRIX *)arg0->unk1C->unk20)[arg0->unk4])[7];
            ((s32 *)bones)[4] = x0;
            ((s32 *)bones)[5] = x1;
            ((s32 *)bones)[6] = x2;
            ((s32 *)bones)[7] = x3;
        }
        gte_SetRotMatrix(bones);
        gte_ldv0(&arg0->unk74);
        gte_rtv0();
        gte_stsv(&sv);
        bones->t[0] += sv.vx;
        bones->t[1] += sv.vy;
        bones->t[2] += sv.vz;
    } else {
        gte_SetRotMatrix(&arg0->unk24);
        gte_SetTransMatrix(&arg0->unk24);
        gte_ldclmv(bones);
        c1 = &bones->m[0][1];
        c2 = &bones->m[0][2];
        gte_rtir_b();
        gte_stclmv(bones);
        gte_ldclmv(c1);
        gte_rtir();
        gte_stclmv(c1);
        gte_ldclmv(c2);
        c1 = (s16 *)bones->t;
        gte_rtir_n1();
        gte_stclmv(c2);
        gte_ldlv0(c1);
        gte_rtv0tr();
        gte_stlvnl(c1);
    }
    b = (WmBone *)arg0->unk8->unkC + 1;
    m = bones + 1;
    p = &bones[b->unk3];
    do {
        gte_SetRotMatrix(p);
        gte_ldclmv(m);
        gte_rtir();
        gte_SetTransMatrix(p);
        gte_stclmv(m);
        gte_ldclmv(&m->m[0][1]);
        gte_rtir();
        vec->vz = b->unk0;
        gte_stclmv(&m->m[0][1]);
        gte_ldclmv(&m->m[0][2]);
        gte_rtir();
        b++;
        gte_stclmv(&m->m[0][2]);
        gte_ldlv0(vec);
        gte_rtv0tr();
        p = &bones[b->unk3];
        gte_stlvnl(m->t);
        m++;
    } while (m < end);
    if (nb < 31) {
        m = (MATRIX *)arg0->unk20;
        do {
            s32 x0, x1, x2, x3;

            x0 = ((s32 *)bones)[0];
            x1 = ((s32 *)bones)[1];
            x2 = ((s32 *)bones)[2];
            x3 = ((s32 *)bones)[3];
            ((s32 *)m)[0] = x0;
            ((s32 *)m)[1] = x1;
            ((s32 *)m)[2] = x2;
            ((s32 *)m)[3] = x3;
            x0 = ((s32 *)bones)[4];
            x1 = ((s32 *)bones)[5];
            x2 = ((s32 *)bones)[6];
            x3 = ((s32 *)bones)[7];
            ((s32 *)m)[4] = x0;
            ((s32 *)m)[5] = x1;
            ((s32 *)m)[6] = x2;
            ((s32 *)m)[7] = x3;
            m++;
            bones++;
        } while (bones < end);
    }
}

void func_800B8834(Unk800C95C8 *arg0, u8 arg1, s16 arg2, s16 arg3, s16 arg4) {
    WmModelGroup *g;
    u8 *p;
    u32 j;
    u32 k;

    if (arg0->unk0 & 0x10) {
        return;
    }
    g = &arg0->unk8->unk10[arg1];
    for (k = 0; k < 2; k++) {
        p = arg0->unk44[arg1].unk4;
        if (k != 0) {
            p += g->unk0;
        }
        for (j = 0; j < g->unk2; j++) {
            *(u32 *)&((POLY_FT4 *)p)->r0 = func_800BA0F0(*(u32 *)&((POLY_FT4 *)p)->r0, arg2, arg3, arg4);
            p += sizeof(POLY_FT4);
        }
        for (j = 0; j < g->unk4; j++) {
            *(u32 *)&((POLY_FT3 *)p)->r0 = func_800BA0F0(*(u32 *)&((POLY_FT3 *)p)->r0, arg2, arg3, arg4);
            p += sizeof(POLY_FT3);
        }
        for (j = 0; j < g->unk6; j++) {
            *(u32 *)&((POLY_GT4 *)p)->r0 = func_800BA0F0(*(u32 *)&((POLY_GT4 *)p)->r0, arg2, arg3, arg4);
            *(u32 *)&((POLY_GT4 *)p)->r1 = func_800BA0F0(*(u32 *)&((POLY_GT4 *)p)->r1, arg2, arg3, arg4);
            *(u32 *)&((POLY_GT4 *)p)->r2 = func_800BA0F0(*(u32 *)&((POLY_GT4 *)p)->r2, arg2, arg3, arg4);
            *(u32 *)&((POLY_GT4 *)p)->r3 = func_800BA0F0(*(u32 *)&((POLY_GT4 *)p)->r3, arg2, arg3, arg4);
            p += sizeof(POLY_GT4);
        }
        for (j = 0; j < g->unk8; j++) {
            *(u32 *)&((POLY_GT3 *)p)->r0 = func_800BA0F0(*(u32 *)&((POLY_GT3 *)p)->r0, arg2, arg3, arg4);
            *(u32 *)&((POLY_GT3 *)p)->r1 = func_800BA0F0(*(u32 *)&((POLY_GT3 *)p)->r1, arg2, arg3, arg4);
            *(u32 *)&((POLY_GT3 *)p)->r2 = func_800BA0F0(*(u32 *)&((POLY_GT3 *)p)->r2, arg2, arg3, arg4);
            p += sizeof(POLY_GT3);
        }
        for (j = 0; j < g->unkA; j++) {
            *(u32 *)&((POLY_G4 *)p)->r0 = func_800BA0F0(*(u32 *)&((POLY_G4 *)p)->r0, arg2, arg3, arg4);
            *(u32 *)&((POLY_G4 *)p)->r1 = func_800BA0F0(*(u32 *)&((POLY_G4 *)p)->r1, arg2, arg3, arg4);
            *(u32 *)&((POLY_G4 *)p)->r2 = func_800BA0F0(*(u32 *)&((POLY_G4 *)p)->r2, arg2, arg3, arg4);
            *(u32 *)&((POLY_G4 *)p)->r3 = func_800BA0F0(*(u32 *)&((POLY_G4 *)p)->r3, arg2, arg3, arg4);
            p += sizeof(POLY_G4);
        }
        for (j = 0; j < g->unkC; j++) {
            *(u32 *)&((POLY_G3 *)p)->r0 = func_800BA0F0(*(u32 *)&((POLY_G3 *)p)->r0, arg2, arg3, arg4);
            *(u32 *)&((POLY_G3 *)p)->r1 = func_800BA0F0(*(u32 *)&((POLY_G3 *)p)->r1, arg2, arg3, arg4);
            *(u32 *)&((POLY_G3 *)p)->r2 = func_800BA0F0(*(u32 *)&((POLY_G3 *)p)->r2, arg2, arg3, arg4);
            p += sizeof(POLY_G3);
        }
        for (j = 0; j < g->unkE; j++) {
            *(u32 *)&((POLY_F4 *)p)->r0 = func_800BA0F0(*(u32 *)&((POLY_F4 *)p)->r0, arg2, arg3, arg4);
            p += sizeof(POLY_F4);
        }
        for (j = 0; j < g->unk10; j++) {
            *(u32 *)&((POLY_F3 *)p)->r0 = func_800BA0F0(*(u32 *)&((POLY_F3 *)p)->r0, arg2, arg3, arg4);
            p += sizeof(POLY_F3);
        }
    }
}

void func_800B8C80(Unk800C95C8 *arg0, u8 arg1, s16 arg2, s16 arg3, s16 arg4) {
    WmModelGroup *g;
    u8 *p;
    u32 j;

    if (arg0->unk0 & 0x10) {
        return;
    }
    g = &arg0->unk8->unk10[arg1];
    p = arg0->unk44[arg1].unk4;
    if (D_8006794C->unk1C->unk8 != 0) {
        p += g->unk0;
    }
    for (j = 0; j < g->unk2; j++) {
        *(u32 *)&((POLY_FT4 *)p)->r0 = func_800BA0F0(*(u32 *)&((POLY_FT4 *)p)->r0, arg2, arg3, arg4);
        p += sizeof(POLY_FT4);
    }
    for (j = 0; j < g->unk4; j++) {
        *(u32 *)&((POLY_FT3 *)p)->r0 = func_800BA0F0(*(u32 *)&((POLY_FT3 *)p)->r0, arg2, arg3, arg4);
        p += sizeof(POLY_FT3);
    }
    for (j = 0; j < g->unk6; j++) {
        *(u32 *)&((POLY_GT4 *)p)->r0 = func_800BA0F0(*(u32 *)&((POLY_GT4 *)p)->r0, arg2, arg3, arg4);
        *(u32 *)&((POLY_GT4 *)p)->r1 = func_800BA0F0(*(u32 *)&((POLY_GT4 *)p)->r1, arg2, arg3, arg4);
        *(u32 *)&((POLY_GT4 *)p)->r2 = func_800BA0F0(*(u32 *)&((POLY_GT4 *)p)->r2, arg2, arg3, arg4);
        *(u32 *)&((POLY_GT4 *)p)->r3 = func_800BA0F0(*(u32 *)&((POLY_GT4 *)p)->r3, arg2, arg3, arg4);
        p += sizeof(POLY_GT4);
    }
    for (j = 0; j < g->unk8; j++) {
        *(u32 *)&((POLY_GT3 *)p)->r0 = func_800BA0F0(*(u32 *)&((POLY_GT3 *)p)->r0, arg2, arg3, arg4);
        *(u32 *)&((POLY_GT3 *)p)->r1 = func_800BA0F0(*(u32 *)&((POLY_GT3 *)p)->r1, arg2, arg3, arg4);
        *(u32 *)&((POLY_GT3 *)p)->r2 = func_800BA0F0(*(u32 *)&((POLY_GT3 *)p)->r2, arg2, arg3, arg4);
        p += sizeof(POLY_GT3);
    }
    for (j = 0; j < g->unkA; j++) {
        *(u32 *)&((POLY_G4 *)p)->r0 = func_800BA0F0(*(u32 *)&((POLY_G4 *)p)->r0, arg2, arg3, arg4);
        *(u32 *)&((POLY_G4 *)p)->r1 = func_800BA0F0(*(u32 *)&((POLY_G4 *)p)->r1, arg2, arg3, arg4);
        *(u32 *)&((POLY_G4 *)p)->r2 = func_800BA0F0(*(u32 *)&((POLY_G4 *)p)->r2, arg2, arg3, arg4);
        *(u32 *)&((POLY_G4 *)p)->r3 = func_800BA0F0(*(u32 *)&((POLY_G4 *)p)->r3, arg2, arg3, arg4);
        p += sizeof(POLY_G4);
    }
    for (j = 0; j < g->unkC; j++) {
        *(u32 *)&((POLY_G3 *)p)->r0 = func_800BA0F0(*(u32 *)&((POLY_G3 *)p)->r0, arg2, arg3, arg4);
        *(u32 *)&((POLY_G3 *)p)->r1 = func_800BA0F0(*(u32 *)&((POLY_G3 *)p)->r1, arg2, arg3, arg4);
        *(u32 *)&((POLY_G3 *)p)->r2 = func_800BA0F0(*(u32 *)&((POLY_G3 *)p)->r2, arg2, arg3, arg4);
        p += sizeof(POLY_G3);
    }
    for (j = 0; j < g->unkE; j++) {
        *(u32 *)&((POLY_F4 *)p)->r0 = func_800BA0F0(*(u32 *)&((POLY_F4 *)p)->r0, arg2, arg3, arg4);
        p += sizeof(POLY_F4);
    }
    for (j = 0; j < g->unk10; j++) {
        *(u32 *)&((POLY_F3 *)p)->r0 = func_800BA0F0(*(u32 *)&((POLY_F3 *)p)->r0, arg2, arg3, arg4);
        p += sizeof(POLY_F3);
    }
}

void func_800B9134(Unk800C95C8 *arg0, u8 arg1, s16 arg2, s16 arg3, s16 arg4) {
    u32 *col;
    WmModelGroup *g;
    u8 *p;
    u8 *s;
    u32 j;
    u32 k;
    u8 c;

    if (arg0->unk0 & 0x10) {
        return;
    }
    g = &arg0->unk8->unk10[arg1];
    for (k = 0; k < 2; k++) {
        s = g->unk1C;
        p = arg0->unk44[arg1].unk4;
        if (k != 0) {
            p += g->unk0;
        }
        for (j = 0; j < g->unk2; j++) {
            c = ((POLY_FT4 *)p)->code;
            *(u32 *)&((POLY_FT4 *)p)->r0 = func_800BA0F0(*(u32 *)((WmSrcFT4 *)s)->unk10, arg2, arg3, arg4);
            ((POLY_FT4 *)p)->code = c;
            p += sizeof(POLY_FT4);
            s += sizeof(WmSrcFT4);
        }

        for (j = 0; j < g->unk4; j++) {
            c = ((POLY_FT3 *)p)->code;
            *(u32 *)&((POLY_FT3 *)p)->r0 = func_800BA0F0(*(u32 *)((WmSrcFT3 *)s)->unk8, arg2, arg3, arg4);
            ((POLY_FT3 *)p)->code = c;
            p += sizeof(POLY_FT3);
            s += sizeof(WmSrcFT3);
        }

        for (j = 0; j < g->unk6; j++) {
            c = ((POLY_GT4 *)p)->code;
            col = (u32 *)g->unk24[((WmSrcGT4 *)s)->unk10[0]];
            *(u32 *)&((POLY_GT4 *)p)->r0 = func_800BA0F0(*col, arg2, arg3, arg4);
            col = (u32 *)g->unk24[((WmSrcGT4 *)s)->unk10[1]];
            *(u32 *)&((POLY_GT4 *)p)->r1 = func_800BA0F0(*col, arg2, arg3, arg4);
            col = (u32 *)g->unk24[((WmSrcGT4 *)s)->unk10[2]];
            *(u32 *)&((POLY_GT4 *)p)->r2 = func_800BA0F0(*col, arg2, arg3, arg4);
            col = (u32 *)g->unk24[((WmSrcGT4 *)s)->unk10[3]];
            *(u32 *)&((POLY_GT4 *)p)->r3 = func_800BA0F0(*col, arg2, arg3, arg4);
            ((POLY_GT4 *)p)->code = c;
            p += sizeof(POLY_GT4);
            s += sizeof(WmSrcGT4);
        }

        for (j = 0; j < g->unk8; j++) {
            c = ((POLY_GT3 *)p)->code;
            col = (u32 *)g->unk24[((WmSrcGT3 *)s)->unk10[0]];
            *(u32 *)&((POLY_GT3 *)p)->r0 = func_800BA0F0(*col, arg2, arg3, arg4);
            col = (u32 *)g->unk24[((WmSrcGT3 *)s)->unk10[1]];
            *(u32 *)&((POLY_GT3 *)p)->r1 = func_800BA0F0(*col, arg2, arg3, arg4);
            col = (u32 *)g->unk24[((WmSrcGT3 *)s)->unk10[2]];
            *(u32 *)&((POLY_GT3 *)p)->r2 = func_800BA0F0(*col, arg2, arg3, arg4);
            ((POLY_GT3 *)p)->code = c;
            p += sizeof(POLY_GT3);
            s += sizeof(WmSrcGT3);
        }

        for (j = 0; j < g->unkA; j++) {
            c = ((POLY_G4 *)p)->code;
            col = (u32 *)g->unk24[((WmSrcG4 *)s)->unk8[0]];
            *(u32 *)&((POLY_G4 *)p)->r0 = func_800BA0F0(*col, arg2, arg3, arg4);
            col = (u32 *)g->unk24[((WmSrcG4 *)s)->unk8[1]];
            *(u32 *)&((POLY_G4 *)p)->r1 = func_800BA0F0(*col, arg2, arg3, arg4);
            col = (u32 *)g->unk24[((WmSrcG4 *)s)->unk8[2]];
            *(u32 *)&((POLY_G4 *)p)->r2 = func_800BA0F0(*col, arg2, arg3, arg4);
            col = (u32 *)g->unk24[((WmSrcG4 *)s)->unk8[3]];
            *(u32 *)&((POLY_G4 *)p)->r3 = func_800BA0F0(*col, arg2, arg3, arg4);
            ((POLY_G4 *)p)->code = c;
            p += sizeof(POLY_G4);
            s += sizeof(WmSrcG4);
        }

        for (j = 0; j < g->unkC; j++) {
            c = ((POLY_G3 *)p)->code;
            col = (u32 *)g->unk24[((WmSrcG3 *)s)->unk8[0]];
            *(u32 *)&((POLY_G3 *)p)->r0 = func_800BA0F0(*col, arg2, arg3, arg4);
            col = (u32 *)g->unk24[((WmSrcG3 *)s)->unk8[1]];
            *(u32 *)&((POLY_G3 *)p)->r1 = func_800BA0F0(*col, arg2, arg3, arg4);
            col = (u32 *)g->unk24[((WmSrcG3 *)s)->unk8[2]];
            *(u32 *)&((POLY_G3 *)p)->r2 = func_800BA0F0(*col, arg2, arg3, arg4);
            ((POLY_G3 *)p)->code = c;
            p += sizeof(POLY_G3);
            s += sizeof(WmSrcG3);
        }

        for (j = 0; j < g->unkE; j++) {
            c = ((POLY_F4 *)p)->code;
            *(u32 *)&((POLY_F4 *)p)->r0 |= func_800BA0F0(*(u32 *)((WmSrcF4 *)s)->unk8, arg2, arg3, arg4);
            ((POLY_F4 *)p)->code = c;
            p += sizeof(POLY_F4);
            s += sizeof(WmSrcF4);
        }

        for (j = 0; j < g->unk10; j++) {
            c = ((POLY_F3 *)p)->code;
            *(u32 *)&((POLY_F3 *)p)->r0 = func_800BA0F0(*(u32 *)((WmSrcF3 *)s)->unk8, arg2, arg3, arg4);
            ((POLY_F3 *)p)->code = c;
            p += sizeof(POLY_F3);
            s += sizeof(WmSrcF3);
        }
    }
}

/* recolour a model group in the current draw buffer (linked prims only) */
void func_800B9738(Unk800C95C8 *arg0, u8 arg1, s16 arg2, s16 arg3, s16 arg4) {
    u32 *col;
    WmModelGroup *g;
    u8 *p;
    u8 *s;
    u32 j;
    u8 c;

    if (arg0->unk0 & 0x10) {
        return;
    }
    g = &arg0->unk8->unk10[arg1];
    p = arg0->unk44[arg1].unk4;
    s = g->unk1C;
    if (D_8006794C->unk1C->unk8 != 0) {
        p += g->unk0;
    }
    for (j = 0; j < g->unk2; j++) {
        if (*(u32 *)p & 0xFFFFFF) {
            c = ((POLY_FT4 *)p)->code;
            *(u32 *)&((POLY_FT4 *)p)->r0 = func_800BA0F0(*(u32 *)((WmSrcFT4 *)s)->unk10, arg2, arg3, arg4);
            ((POLY_FT4 *)p)->code = c;
        }
        p += sizeof(POLY_FT4);
        s += sizeof(WmSrcFT4);
    }

    for (j = 0; j < g->unk4; j++) {
        if (*(u32 *)p & 0xFFFFFF) {
            c = ((POLY_FT3 *)p)->code;
            *(u32 *)&((POLY_FT3 *)p)->r0 = func_800BA0F0(*(u32 *)((WmSrcFT3 *)s)->unk8, arg2, arg3, arg4);
            ((POLY_FT3 *)p)->code = c;
        }
        p += sizeof(POLY_FT3);
        s += sizeof(WmSrcFT3);
    }

    for (j = 0; j < g->unk6; j++) {
        if (*(u32 *)p & 0xFFFFFF) {
            c = ((POLY_GT4 *)p)->code;
            col = (u32 *)g->unk24[((WmSrcGT4 *)s)->unk10[0]];
            *(u32 *)&((POLY_GT4 *)p)->r0 = func_800BA0F0(*col, arg2, arg3, arg4);
            col = (u32 *)g->unk24[((WmSrcGT4 *)s)->unk10[1]];
            *(u32 *)&((POLY_GT4 *)p)->r1 = func_800BA0F0(*col, arg2, arg3, arg4);
            col = (u32 *)g->unk24[((WmSrcGT4 *)s)->unk10[2]];
            *(u32 *)&((POLY_GT4 *)p)->r2 = func_800BA0F0(*col, arg2, arg3, arg4);
            col = (u32 *)g->unk24[((WmSrcGT4 *)s)->unk10[3]];
            *(u32 *)&((POLY_GT4 *)p)->r3 = func_800BA0F0(*col, arg2, arg3, arg4);
            ((POLY_GT4 *)p)->code = c;
        }
        p += sizeof(POLY_GT4);
        s += sizeof(WmSrcGT4);
    }

    for (j = 0; j < g->unk8; j++) {
        if (*(u32 *)p & 0xFFFFFF) {
            c = ((POLY_GT3 *)p)->code;
            col = (u32 *)g->unk24[((WmSrcGT3 *)s)->unk10[0]];
            *(u32 *)&((POLY_GT3 *)p)->r0 = func_800BA0F0(*col, arg2, arg3, arg4);
            col = (u32 *)g->unk24[((WmSrcGT3 *)s)->unk10[1]];
            *(u32 *)&((POLY_GT3 *)p)->r1 = func_800BA0F0(*col, arg2, arg3, arg4);
            col = (u32 *)g->unk24[((WmSrcGT3 *)s)->unk10[2]];
            *(u32 *)&((POLY_GT3 *)p)->r2 = func_800BA0F0(*col, arg2, arg3, arg4);
            ((POLY_GT3 *)p)->code = c;
        }
        p += sizeof(POLY_GT3);
        s += sizeof(WmSrcGT3);
    }

    for (j = 0; j < g->unkA; j++) {
        if (*(u32 *)p & 0xFFFFFF) {
            c = ((POLY_G4 *)p)->code;
            col = (u32 *)g->unk24[((WmSrcG4 *)s)->unk8[0]];
            *(u32 *)&((POLY_G4 *)p)->r0 = func_800BA0F0(*col, arg2, arg3, arg4);
            col = (u32 *)g->unk24[((WmSrcG4 *)s)->unk8[1]];
            *(u32 *)&((POLY_G4 *)p)->r1 = func_800BA0F0(*col, arg2, arg3, arg4);
            col = (u32 *)g->unk24[((WmSrcG4 *)s)->unk8[2]];
            *(u32 *)&((POLY_G4 *)p)->r2 = func_800BA0F0(*col, arg2, arg3, arg4);
            col = (u32 *)g->unk24[((WmSrcG4 *)s)->unk8[3]];
            *(u32 *)&((POLY_G4 *)p)->r3 = func_800BA0F0(*col, arg2, arg3, arg4);
            ((POLY_G4 *)p)->code = c;
        }
        p += sizeof(POLY_G4);
        s += sizeof(WmSrcG4);
    }

    for (j = 0; j < g->unkC; j++) {
        if (*(u32 *)p & 0xFFFFFF) {
            c = ((POLY_G3 *)p)->code;
            col = (u32 *)g->unk24[((WmSrcG3 *)s)->unk8[0]];
            *(u32 *)&((POLY_G3 *)p)->r0 = func_800BA0F0(*col, arg2, arg3, arg4);
            col = (u32 *)g->unk24[((WmSrcG3 *)s)->unk8[1]];
            *(u32 *)&((POLY_G3 *)p)->r1 = func_800BA0F0(*col, arg2, arg3, arg4);
            col = (u32 *)g->unk24[((WmSrcG3 *)s)->unk8[2]];
            *(u32 *)&((POLY_G3 *)p)->r2 = func_800BA0F0(*col, arg2, arg3, arg4);
            ((POLY_G3 *)p)->code = c;
        }
        p += sizeof(POLY_G3);
        s += sizeof(WmSrcG3);
    }

    for (j = 0; j < g->unkE; j++) {
        if (*(u32 *)p & 0xFFFFFF) {
            c = ((POLY_F4 *)p)->code;
            *(u32 *)&((POLY_F4 *)p)->r0 |= func_800BA0F0(*(u32 *)((WmSrcF4 *)s)->unk8, arg2, arg3, arg4);
            ((POLY_F4 *)p)->code = c;
        }
        p += sizeof(POLY_F4);
        s += sizeof(WmSrcF4);
    }

    for (j = 0; j < g->unk10; j++) {
        if (*(u32 *)p & 0xFFFFFF) {
            c = ((POLY_F3 *)p)->code;
            *(u32 *)&((POLY_F3 *)p)->r0 = func_800BA0F0(*(u32 *)((WmSrcF3 *)s)->unk8, arg2, arg3, arg4);
            ((POLY_F3 *)p)->code = c;
        }
        p += sizeof(POLY_F3);
        s += sizeof(WmSrcF3);
    }
}

void func_800B9E4C(Unk800C95C8 *arg0, u8 arg1, s16 arg2, s16 arg3, s16 arg4) {
    WmModelGroup *g;
    u32 c;
    u8 *p;
    u32 j;

    if (arg0->unk0 & 0x10) {
        return;
    }
    g = &arg0->unk8->unk10[arg1];
    c = (arg4 << 16) | (arg3 << 8) | arg2;
    if (D_8006794C->unk1C->unk8 != 0) {
        p = arg0->unk44[arg1].unk4 + g->unk0;
    } else {
        p = arg0->unk44[arg1].unk4;
    }
    for (j = 0; j < g->unk2; j++) {
        *(u32 *)&((POLY_FT4 *)p)->r0 = c | (((POLY_FT4 *)p)->code << 24);
        p += sizeof(POLY_FT4);
    }
    for (j = 0; j < g->unk4; j++) {
        *(u32 *)&((POLY_FT3 *)p)->r0 = c | (((POLY_FT3 *)p)->code << 24);
        p += sizeof(POLY_FT3);
    }
    for (j = 0; j < g->unk6; j++) {
        *(u32 *)&((POLY_GT4 *)p)->r0 = c | (((POLY_GT4 *)p)->code << 24);
        p += sizeof(POLY_GT4);
    }
    for (j = 0; j < g->unk8; j++) {
        *(u32 *)&((POLY_GT3 *)p)->r0 = c | (((POLY_GT3 *)p)->code << 24);
        p += sizeof(POLY_GT3);
    }
    for (j = 0; j < g->unkA; j++) {
        *(u32 *)&((POLY_G4 *)p)->r0 = c | (((POLY_G4 *)p)->code << 24);
        p += sizeof(POLY_G4);
    }
    for (j = 0; j < g->unkC; j++) {
        *(u32 *)&((POLY_G3 *)p)->r0 = c | (((POLY_G3 *)p)->code << 24);
        p += sizeof(POLY_G3);
    }
    for (j = 0; j < g->unkE; j++) {
        *(u32 *)&((POLY_F4 *)p)->r0 = c | (((POLY_F4 *)p)->code << 24);
        p += sizeof(POLY_F4);
    }
    for (j = 0; j < g->unk10; j++) {
        *(u32 *)&((POLY_F3 *)p)->r0 = c | (((POLY_F3 *)p)->code << 24);
        p += sizeof(POLY_F3);
    }
}

u32 func_800BA0F0(u32 arg0, s32 arg1, s32 arg2, s32 arg3) {
    s16 b = arg3 + ((arg0 >> 16) & 0xFF);
    s16 g = arg2 + ((arg0 >> 8) & 0xFF);
    s16 r = arg1 + (arg0 & 0xFF);
    arg0 &= 0xFF000000;
    if (r >= 0x100) {
        arg0 |= 0xFF;
    } else if (r > 0) {
        arg0 |= r;
    }
    if (g >= 0x100) {
        arg0 |= 0xFF00;
    } else if (g > 0) {
        arg0 |= g << 8;
    }
    if (b >= 0x100) {
        arg0 |= 0xFF0000;
    } else if (b > 0) {
        arg0 |= b << 16;
    }
    return arg0;
}

void func_800BA190(Unk800C95C8 *arg0, u16 arg1) {
    u32 i;
    u32 j;
    u8 *p;
    WmModelGroup *g;
    u8 *s;

    if (arg0->unk0 & 0x10) {
        return;
    }
    for (i = 0; i < arg0->unk8->unk3; i++) {
        g = &arg0->unk8->unk10[i];
        s = g->unk1C;
        p = arg0->unk44[i].unk4;
        if (D_8006794C->unk1C->unk8 != 0) {
            p += g->unk0;
        }
        for (j = 0; j < g->unk2; j++) {
            if ((((WmSrcFT4 *)s)->unk15 & 1) || arg1 != 0) {
                ((POLY_FT4 *)p)->code |= 2;
            } else {
                ((POLY_FT4 *)p)->code &= ~2;
            }
            p += sizeof(POLY_FT4);
            s += sizeof(WmSrcFT4);
        }
        for (j = 0; j < g->unk4; j++) {
            if ((((WmSrcFT3 *)s)->unk12 & 1) || arg1 != 0) {
                ((POLY_FT3 *)p)->code |= 2;
            } else {
                ((POLY_FT3 *)p)->code &= ~2;
            }
            s += sizeof(WmSrcFT3);
            p += sizeof(POLY_FT3);
        }
        for (j = 0; j < g->unk6; j++) {
            if ((((WmSrcGT4 *)s)->unk1C & 1) || arg1 != 0) {
                ((POLY_GT4 *)p)->code |= 2;
            } else {
                ((POLY_GT4 *)p)->code &= ~2;
            }
            s += sizeof(WmSrcGT4);
            p += sizeof(POLY_GT4);
        }
        for (j = 0; j < g->unk8; j++) {
            if ((((WmSrcGT3 *)s)->unkF & 1) || arg1 != 0) {
                ((POLY_GT3 *)p)->code |= 2;
            } else {
                ((POLY_GT3 *)p)->code &= ~2;
            }
            s += sizeof(WmSrcGT3);
            p += sizeof(POLY_GT3);
        }
    }
}

void func_800BA3CC(Unk800C95C8 *arg0, u16 arg1) {
    u32 i;
    u32 j;
    u8 *p;
    WmModelGroup *g;

    if (arg0->unk0 & 0x10) {
        return;
    }
    for (i = 0; i < arg0->unk8->unk3; i++) {
        g = &arg0->unk8->unk10[i];
        p = arg0->unk44[i].unk4;
        if (D_8006794C->unk1C->unk8 != 0) {
            p += g->unk0;
        }
        for (j = 0; j < g->unk2; j++) {
            ((POLY_FT4 *)p)->tpage = arg1 | (((POLY_FT4 *)p)->tpage & 0xFF9F);
            p += sizeof(POLY_FT4);
        }
        for (j = 0; j < g->unk4; j++) {
            ((POLY_FT3 *)p)->tpage = arg1 | (((POLY_FT3 *)p)->tpage & 0xFF9F);
            p += sizeof(POLY_FT3);
        }
        for (j = 0; j < g->unk6; j++) {
            ((POLY_GT4 *)p)->tpage = arg1 | (((POLY_GT4 *)p)->tpage & 0xFF9F);
            p += sizeof(POLY_GT4);
        }
        for (j = 0; j < g->unk8; j++) {
            ((POLY_GT3 *)p)->tpage = arg1 | (((POLY_GT3 *)p)->tpage & 0xFF9F);
            p += sizeof(POLY_GT3);
        }
    }
}

void func_800BA560(Unk800C95C8 *arg0, s32 arg1) {
    VECTOR v;
    VECTOR up;
    VECTOR r0;
    VECTOR r2;
    VECTOR r1;
    MATRIX s;
    u32 idx;
    u32 head;
    MATRIX *end;
    u16 nb;
    VECTOR *vec;
    VECTOR *up_p;
    WmAnim *anim;
    MATRIX *bones;
    MATRIX *m;
    MATRIX *p;
    WmAnimKey *key;
    WmAnimKey *key2;
    SVECTOR *rot;
    s16 *d1;
    s16 *d2;
    s32 w0, w1, w2, w3;

    anim = (WmAnim *)arg0->unkC;
    if (anim == NULL) {
        if (arg0->unk1C != NULL) {
            /* attached to a bone of the parent model */
            w0 = ((s32 *)&((MATRIX *)arg0->unk1C->unk20)[arg0->unk4])[0];
            w1 = ((s32 *)&((MATRIX *)arg0->unk1C->unk20)[arg0->unk4])[1];
            w2 = ((s32 *)&((MATRIX *)arg0->unk1C->unk20)[arg0->unk4])[2];
            w3 = ((s32 *)&((MATRIX *)arg0->unk1C->unk20)[arg0->unk4])[3];
            ((s32 *)arg0->unk20)[0] = w0;
            ((s32 *)arg0->unk20)[1] = w1;
            ((s32 *)arg0->unk20)[2] = w2;
            ((s32 *)arg0->unk20)[3] = w3;
            w0 = ((s32 *)&((MATRIX *)arg0->unk1C->unk20)[arg0->unk4])[4];
            w1 = ((s32 *)&((MATRIX *)arg0->unk1C->unk20)[arg0->unk4])[5];
            w2 = ((s32 *)&((MATRIX *)arg0->unk1C->unk20)[arg0->unk4])[6];
            w3 = ((s32 *)&((MATRIX *)arg0->unk1C->unk20)[arg0->unk4])[7];
            ((s32 *)arg0->unk20)[4] = w0;
            ((s32 *)arg0->unk20)[5] = w1;
            ((s32 *)arg0->unk20)[6] = w2;
            ((s32 *)arg0->unk20)[7] = w3;
            ((MATRIX *)arg0->unk20)->t[0] += arg0->unk74;
            ((MATRIX *)arg0->unk20)->t[1] += arg0->unk76;
            ((MATRIX *)arg0->unk20)->t[2] += arg0->unk78;
        } else {
            ((MATRIX *)arg0->unk20)->m[0][0] = arg0->unk24.m[0][0];
            ((MATRIX *)arg0->unk20)->m[0][1] = -arg0->unk24.m[0][1];
            ((MATRIX *)arg0->unk20)->m[0][2] = -arg0->unk24.m[0][2];
            ((MATRIX *)arg0->unk20)->m[1][0] = arg0->unk24.m[1][0];
            ((MATRIX *)arg0->unk20)->m[1][1] = -arg0->unk24.m[1][1];
            ((MATRIX *)arg0->unk20)->m[1][2] = -arg0->unk24.m[1][2];
            ((MATRIX *)arg0->unk20)->m[2][0] = arg0->unk24.m[2][0];
            ((MATRIX *)arg0->unk20)->m[2][1] = -arg0->unk24.m[2][1];
            ((MATRIX *)arg0->unk20)->m[2][2] = -arg0->unk24.m[2][2];
            ((MATRIX *)arg0->unk20)->t[0] = arg0->unk24.t[0];
            ((MATRIX *)arg0->unk20)->t[1] = arg0->unk24.t[1];
            ((MATRIX *)arg0->unk20)->t[2] = arg0->unk24.t[2];
        }
        return;
    }
    nb = arg0->unk8->unk2;
    bones = (MATRIX *)0x1F800018;
    if (nb >= 31) {
        bones = (MATRIX *)arg0->unk20;
    }
    m = bones;
    vec = (VECTOR *)0x1F800008;
    vec->vx = 0;
    vec->vy = 0;
    key = anim->unkC;
    key2 = anim->unk10;
    rot = (SVECTOR *)0x1F800000;
    if (anim->unkA & 1) {
        bones->t[0] = anim->unk4[0];
    } else {
        bones->t[0] = ((s16 *)((u8 *)anim + (u16)anim->unk4[0]))[(u16)arg1];
    }
    if (anim->unkA & 2) {
        m->t[1] = anim->unk4[1];
    } else {
        m->t[1] = ((s16 *)((u8 *)anim + (u16)anim->unk4[1]))[(u16)arg1];
    }
    if (anim->unkA & 4) {
        m->t[2] = anim->unk4[2];
    } else {
        m->t[2] = ((s16 *)((u8 *)anim + (u16)anim->unk4[2]))[(u16)arg1];
    }
    end = &bones[nb];
loop1:
    gte_ldrotkey(key, anim, arg1, rot);
    if (key2 != NULL) {
        gte_ldrotkey_lo(key2, anim, arg1, rot);
        key2++;
    }
    key++;
    func_80031264(rot, m);
    m++;
    if (m < end) {
        goto loop1;
    }
    if (arg0->unk1C != NULL) {
        {
            s32 x0, x1, x2, x3;

            x0 = ((s32 *)&((MATRIX *)arg0->unk1C->unk20)[arg0->unk4])[0];
            x1 = ((s32 *)&((MATRIX *)arg0->unk1C->unk20)[arg0->unk4])[1];
            x2 = ((s32 *)&((MATRIX *)arg0->unk1C->unk20)[arg0->unk4])[2];
            x3 = ((s32 *)&((MATRIX *)arg0->unk1C->unk20)[arg0->unk4])[3];
            ((s32 *)bones)[0] = x0;
            ((s32 *)bones)[1] = x1;
            ((s32 *)bones)[2] = x2;
            ((s32 *)bones)[3] = x3;
        }
        {
            s32 x0, x1, x2, x3;

            x0 = ((s32 *)&((MATRIX *)arg0->unk1C->unk20)[arg0->unk4])[4];
            x1 = ((s32 *)&((MATRIX *)arg0->unk1C->unk20)[arg0->unk4])[5];
            x2 = ((s32 *)&((MATRIX *)arg0->unk1C->unk20)[arg0->unk4])[6];
            x3 = ((s32 *)&((MATRIX *)arg0->unk1C->unk20)[arg0->unk4])[7];
            ((s32 *)bones)[4] = x0;
            ((s32 *)bones)[5] = x1;
            ((s32 *)bones)[6] = x2;
            ((s32 *)bones)[7] = x3;
        }
        bones->t[0] += arg0->unk74;
        bones->t[1] += arg0->unk76;
        bones->t[2] += arg0->unk78;
    } else {
        gte_SetRotMatrix(&arg0->unk24);
        gte_SetTransMatrix(&arg0->unk24);
        gte_ldclmv(bones);
        d1 = &bones->m[0][1];
        d2 = &bones->m[0][2];
        gte_rtir_b();
        gte_stclmv(bones);
        gte_ldclmv(d1);
        gte_rtir();
        gte_stclmv(d1);
        gte_ldclmv(d2);
        d1 = (s16 *)bones->t;
        gte_rtir_n1();
        gte_stclmv(d2);
        gte_ldlv0(d1);
        gte_rtv0tr();
        gte_stlvnl(d1);
    }
    m = bones + 1;
    idx = 1;
    up_p = &r2;
    key2 = (WmAnimKey *)((WmBone *)arg0->unk8->unkC + 1);
    head = arg0->unk8->unk6;
    rot = (SVECTOR *)&v;
    p = &bones[((WmBone *)key2)->unk3];
loop:
    gte_SetRotMatrix(p);
    gte_ldclmv(m);
    gte_rtir();
    gte_SetTransMatrix(p);
    gte_stclmv(m);
    key = (WmAnimKey *)&m->m[0][1];
    gte_ldclmv(key);
    gte_rtir();
    vec->vz = ((WmBone *)key2)->unk0;
    gte_stclmv(key);
    arg1 = (s32)&m->m[0][2];
    gte_ldclmv((s16 *)arg1);
    gte_rtir();
    key2 = (WmAnimKey *)((WmBone *)key2 + 1);
    gte_stclmv((s16 *)arg1);
    gte_ldlv0(vec);
    gte_rtv0tr();
    p = &bones[((WmBone *)key2)->unk3];
    gte_stlvnl(m->t);
    if (head == idx) {
        up.vx = 0;
        up.vy = 0x1000;
        up.vz = 0;
        v.vx = m->t[0] - arg0->unk48[0];
        v.vy = m->t[1] - arg0->unk48[1];
        v.vz = m->t[2] - arg0->unk48[2];
        VectorNormal(&v, up_p);
        gte_ldopv1(&up);
        gte_ldopv2(up_p);
        gte_op12();
        gte_stlvnl(rot);
        VectorNormal((VECTOR *)rot, &r0);
        gte_ldopv1(&r0);
        gte_ldopv2(up_p);
        gte_op12();
        gte_stlvnl(rot);
        VectorNormal((VECTOR *)rot, &r1);
        m->m[0][0] = r0.vx;
        m->m[0][1] = r2.vx;
        m->m[0][2] = r1.vx;
        m->m[1][0] = r0.vy;
        m->m[1][1] = r2.vy;
        m->m[1][2] = r1.vy;
        m->m[2][0] = r0.vz;
        m->m[2][1] = r2.vz;
        m->m[2][2] = r1.vz;
        if (arg0->unk0 & 2) {
            *(s32 *)&s.m[0][0] = 0x1000;
            *(s32 *)&s.m[0][2] = 0;
            *(s32 *)&s.m[1][1] = 0x1000;
            *(s32 *)&s.m[2][0] = 0;
            *(s32 *)&s.m[2][2] = 0x1000;
            s.m[0][0] = arg0->unk54;
            gte_SetRotMatrix(m);
            gte_ldclmv(&s);
            gte_rtir();
            s.m[1][1] = arg0->unk56;
            gte_stclmv(m);
            gte_ldclmv(&s.m[0][1]);
            gte_rtir();
            s.m[2][2] = arg0->unk58;
            gte_stclmv(key);
            gte_ldclmv(&s.m[0][2]);
            gte_rtir();
            gte_stclmv((s16 *)arg1);
        }
    }
    m++;
    idx++;
    if (m < end) {
        goto loop;
    }
    if (nb < 31) {
        m = (MATRIX *)arg0->unk20;
        do {
            s32 x0, x1, x2, x3;

            x0 = ((s32 *)bones)[0];
            x1 = ((s32 *)bones)[1];
            x2 = ((s32 *)bones)[2];
            x3 = ((s32 *)bones)[3];
            ((s32 *)m)[0] = x0;
            ((s32 *)m)[1] = x1;
            ((s32 *)m)[2] = x2;
            ((s32 *)m)[3] = x3;
            x0 = ((s32 *)bones)[4];
            x1 = ((s32 *)bones)[5];
            x2 = ((s32 *)bones)[6];
            x3 = ((s32 *)bones)[7];
            ((s32 *)m)[4] = x0;
            ((s32 *)m)[5] = x1;
            ((s32 *)m)[6] = x2;
            ((s32 *)m)[7] = x3;
            m++;
            bones++;
        } while (bones < end);
    }
}

void func_800BB090(BgAnimHdr *arg0) {
    RECT r;
    u32 i;
    u32 j;
    BgAnim *a;
    BgAnimRect *ar;
    s32 pos;
    s16 f;
    s32 r8;
    s32 v;
    s16 last;

    if (arg0 == NULL) {
        return;
    }
    for (i = 0; i < arg0->unk0; i++) {
        a = &arg0->unk4[i];
        if (!(a->unk0 & 1)) {
            continue;
        }
        if (a->unk1 != 0) {
            pos = a->unk8;
            f = pos >> 12;
            last = a->unkE;
            if (f >= 0) {
                if (f != last) {
                    for (j = 0; j < a->unkC; j++) {
                        ar = &a->unk10[j];
                        r.x = ar->unk8[f].vx;
                        r.y = ar->unk8[f].vy;
                        r.w = ar->unk0.w;
                        r.h = ar->unk0.h;
                        MoveImage(&r, ar->unk0.x, ar->unk0.y);
                    }
                    a->unkE = f;
                }
                pos += a->unk2;
            } else {
                pos += 0x1000;
            }
            if ((pos >> 12) < a->unk1) {
                a->unk8 = pos;
            } else if (a->unk6 != 0) {
                if (D_8007B720 >= D_8007B92D) {
                    func_8004BF20();
                    if (D_8007B720 >= D_8007B92D) {
                        func_8004BF20();
                    }
                }
                r8 = *D_8007B720++;
                v = (a->unk6 + 1) * ((r8 << 8) | r8);
                a->unk8 = -((a->unk4 + v / 0x10000) << 12);
            } else if (a->unk0 & 2) {
                a->unk0 &= ~3;
            } else {
                a->unk8 = 0;
            }
        } else if (a->unk0 & 4) {
            pos = a->unk8;
            f = pos >> 12;
            for (j = 0; j < a->unkC; j++) {
                ar = &a->unk10[j];
                MoveImage(&ar->unk0, ar->unk0.x, ar->unk0.y - f);
                r.x = ar->unk0.x;
                r.y = ar->unk0.y - f;
                r.w = ar->unk0.w;
                r.h = f;
                MoveImage(&r, ar->unk0.x, ar->unk0.y + ar->unk0.h - f);
            }
            a->unk8 = (pos & 0xFFF) + a->unk2;
        }
    }
}

void func_800BB3C4(BgSprHdr *hdr, BgSprOfs *ofs) {
    u32 i, j, k;
    u32 p;
    BgSprPart *part;
    u8 idx;

    if (hdr == NULL) {
        return;
    }
    for (i = 0; i < hdr->unk0; i++) {
        hdr->unk4[i].unkE = 1;
        p = hdr->unk4[i].unk10;
        if (p > 0x80000000) {
            return;
        }
        if (p == 0) {
            continue;
        }
        idx = hdr->unk4[i].unkD;
        p += (u32)hdr;
        hdr->unk4[i].unk10 = p;
        if (ofs != NULL) {
            ((BgSprPart *)p)->unk0 += ofs[idx].unk4;
            ((BgSprPart *)p)->unk2 += ofs[idx].unk6;
        }
        part = (BgSprPart *)p;
        for (j = 0; j < hdr->unk4[i].unkC; j++) {
            p = part[j].unk8;
            p += (u32)hdr;
            part[j].unk8 = p;
            if (ofs != NULL) {
                for (k = 0; k < hdr->unk4[i].unk1; k++) {
                    ((BgSprVtx *)p)->unk0 += ofs[idx].unk0;
                    ((BgSprVtx *)p)->unk2 += ofs[idx].unk2;
                    p += sizeof(BgSprVtx);
                }
            }
        }
    }
}

void func_800BB508(Unk800C95C8 *arg0, u8 arg1) {
    WmModelGroup *g;
    MATRIX *bone;
    SVECTOR *vtx;
    s32 *sxy;
    s32 *sz;
    s32 k;
    u32 i;
    u32 j;
    u32 n;
    u32 cnt;
    s32 ymax;

    gte_SetGeomScreen(D_8006794C->unk1C->unkA);
    sxy = D_800711A8;
    sz = D_80070520;
    k = 0;
    g = &arg0->unk8->unk10[arg1];
    bone = (MATRIX *)arg0->unk20;
    cnt = arg0->unk8->unk2;
    ymax = arg0->unk5A;
    vtx = g->unk18;
    for (i = 0; i < cnt; bone++) {
        n = ((u16 *)g->unk14)[i++];
        if (n != 0) {
            sxy = &D_800711A8[k];
            sz = &D_80070520[k];
            gte_SetRotMatrix(bone);
            gte_SetTransMatrix(bone);
            j = 0;
            do {
                gte_ldv0(vtx);
                j++;
                vtx++;
                gte_rtv0tr_b();
                gte_stsvclamp(sxy, sz, ymax);
                sxy++;
                sz++;
            } while (j < n);
            sxy = &D_800711A8[k];
            sz = &D_80070520[k];
            gte_SetRotMatrix(&D_8006794C->unk1C->unk18);
            gte_SetTransMatrix(&D_8006794C->unk1C->unk18);
            j = 0;
            do {
                gte_ldv3xyz(sxy, sz);
                j += 3;
                gte_rtpt_b();
                gte_stsxy3c(sxy);
                sxy += 3;
                gte_stsz3h(sz);
                sz += 3;
            } while (j < n);
            k += n;
        }
    }
}

INCLUDE_ASM("asm/us/ovl_0a/nonmatchings/ovl_0a", func_800BB714);

/* reflect the mirrored model across the plane (unk64 normal, unk6C point) */
void func_800BC33C(Unk800C95C8 *arg0) {
    MATRIX *m;
    s32 nx;
    s32 ny;
    s32 nz;
    s32 d;

    if (arg0->unk60 != NULL) {
        nx = arg0->unk64.v.vx;
        ny = arg0->unk64.v.vy;
        nz = arg0->unk64.v.vz;
        d = (arg0->unk6C.v.vx * nx + arg0->unk6C.v.vy * ny + arg0->unk6C.v.vz * nz) >> 11;
        m = (MATRIX *)0x1F800000;
        m->m[0][0] = 0x1000 - ((nx * nx) >> 11);
        m->m[1][1] = 0x1000 - ((ny * ny) >> 11);
        m->m[2][2] = 0x1000 - ((nz * nz) >> 11);
        m->m[0][1] = m->m[1][0] = (nx * ny) >> 11;
        m->m[0][2] = m->m[2][0] = (nx * nz) >> 11;
        m->m[1][2] = m->m[2][1] = (ny * nz) >> 11;
        m->t[0] = (d * nx) >> 12;
        m->t[1] = (d * ny) >> 12;
        m->t[2] = (d * nz) >> 12;
        gte_SetRotMatrix(m);
        gte_ldclmv(&arg0->unk60->unk24);
        gte_rtir();
        gte_stclmv(&arg0->unk24);
        gte_ldclmv(&arg0->unk60->unk24.m[0][1]);
        gte_rtir();
        gte_stclmv(&arg0->unk24.m[0][1]);
        gte_ldclmv(&arg0->unk60->unk24.m[0][2]);
        gte_rtir();
        gte_stclmv(&arg0->unk24.m[0][2]);
        gte_SetTransMatrix(m);
        gte_ldlv0(arg0->unk60->unk24.t);
        gte_rtv0tr();
        gte_stlvnl(arg0->unk24.t);
        arg0->unkC = arg0->unk60->unkC;
    }
}

void func_800BC600(Unk800C95C8 *arg0, u8 arg1) {
    WmModelGroup *g;
    u32 *ot;
    s32 opz;
    s32 otz;
    u32 n;
    u32 i;
    s32 xy0, xy1, xy2;
    u32 i0, i1, i2, i3;
    u8 *prim;
    WmSrcFT4 *sFT4;
    POLY_FT4 *pFT4;
    WmSrcFT3 *sFT3;
    POLY_FT3 *pFT3;
    WmSrcGT4 *sGT4;
    POLY_GT4 *pGT4;
    WmSrcGT3 *sGT3;
    POLY_GT3 *pGT3;
    WmSrcG4 *sG4;
    POLY_G4 *pG4;
    WmSrcG3 *sG3;
    POLY_G3 *pG3;
    WmSrcF4 *sF4;
    POLY_F4 *pF4;
    WmSrcF3 *sF3;
    POLY_F3 *pF3;

    ot = D_8006794C->unk1C->unk10[D_8006794C->unk1C->unk8];
    g = &arg0->unk8->unk10[arg1];
    if (D_8006794C->unk1C->unk8 != 0) {
        prim = (u8 *)arg0->unk44[arg1].unk4 + g->unk0;
    } else {
        prim = arg0->unk44[arg1].unk4;
    }

    n = g->unk2;
    pFT4 = (POLY_FT4 *)prim;
    ot -= g->unk13;
    sFT4 = (WmSrcFT4 *)(void *)g->unk1C;
    for (i = 0; i < n; sFT4++, pFT4++) {
        i0 = *(u32 *)&sFT4->unk0[0];
        i2 = *(u32 *)&sFT4->unk0[2];
        i1 = i0 >> 16;
        i3 = i2 >> 16;
        i0 &= 0xFFFF;
        i2 &= 0xFFFF;
        xy0 = D_800711A8[i0];
        xy1 = D_800711A8[i1];
        xy2 = D_800711A8[i2];
        i++;
        if (!(sFT4->unk15 & 2)) {
            gte_ldsxy3(xy0, xy1, xy2);
            setaddr(pFT4, 0);
            gte_nclip_b();
            gte_stopz(&opz);
            if (opz >= 0) {
                continue;
            }
        }
        otz = (D_80070520[i0] + D_80070520[i1] + D_80070520[i2] + D_80070520[i3]) >> 4;
        *(s32 *)&pFT4->x0 = xy0;
        *(s32 *)&pFT4->x1 = xy1;
        *(s32 *)&pFT4->x2 = xy2;
        *(s32 *)&pFT4->x3 = D_800711A8[i3];
        addPrim(&ot[otz], pFT4);
    }

    n = g->unk4;
    sFT3 = (WmSrcFT3 *)sFT4;
    pFT3 = (POLY_FT3 *)pFT4;
    for (i = 0; i < n; sFT3++, pFT3++) {
        i0 = *(u32 *)&sFT3->unk0[0];
        i2 = sFT3->unk0[2];
        i1 = i0 >> 16;
        i0 &= 0xFFFF;
        xy0 = D_800711A8[i0];
        xy1 = D_800711A8[i1];
        xy2 = D_800711A8[i2];
        i++;
        if (!(sFT3->unk12 & 2)) {
            gte_ldsxy3(xy0, xy1, xy2);
            setaddr(pFT3, 0);
            gte_nclip_b();
            gte_stopz(&opz);
            if (opz >= 0) {
                continue;
            }
        }
        gte_ldsz3(D_80070520[(u16)i0], D_80070520[i1], D_80070520[i2]);
        *(s32 *)&pFT3->x0 = xy0;
        *(s32 *)&pFT3->x1 = xy1;
        gte_avsz3_b();
        *(s32 *)&pFT3->x2 = xy2;
        gte_stotz(&otz);
        addPrim(&ot[otz], pFT3);
    }

    n = g->unk6;
    sGT4 = (WmSrcGT4 *)sFT3;
    pGT4 = (POLY_GT4 *)pFT3;
    for (i = 0; i < n; sGT4++, pGT4++) {
        i0 = *(u32 *)&sGT4->unk0[0];
        i2 = *(u32 *)&sGT4->unk0[2];
        i1 = i0 >> 16;
        i3 = i2 >> 16;
        i0 &= 0xFFFF;
        i2 &= 0xFFFF;
        xy0 = D_800711A8[i0];
        xy1 = D_800711A8[i1];
        xy2 = D_800711A8[i2];
        i++;
        if (!(sGT4->unk1C & 2)) {
            gte_ldsxy3(xy0, xy1, xy2);
            setaddr(pGT4, 0);
            gte_nclip_b();
            gte_stopz(&opz);
            if (opz >= 0) {
                continue;
            }
        }
        otz = D_80070520[i0] + D_80070520[i1] + D_80070520[i2] + D_80070520[i3];
        *(s32 *)&pGT4->x0 = xy0;
        *(s32 *)&pGT4->x1 = xy1;
        *(s32 *)&pGT4->x2 = xy2;
        *(s32 *)&pGT4->x3 = D_800711A8[i3];
        otz >>= 4;
        addPrim(&ot[otz], pGT4);
    }

    n = g->unk8;
    sGT3 = (WmSrcGT3 *)sGT4;
    pGT3 = (POLY_GT3 *)pGT4;
    for (i = 0; i < n; sGT3++, pGT3++) {
        i0 = *(u32 *)&sGT3->unk0[0];
        i2 = sGT3->unk0[2];
        i1 = i0 >> 16;
        i0 &= 0xFFFF;
        xy0 = D_800711A8[i0];
        xy1 = D_800711A8[i1];
        xy2 = D_800711A8[i2];
        i++;
        if (!(sGT3->unkF & 2)) {
            gte_ldsxy3(xy0, xy1, xy2);
            setaddr(pGT3, 0);
            gte_nclip_b();
            gte_stopz(&opz);
            if (opz >= 0) {
                continue;
            }
        }
        gte_ldsz3(D_80070520[i0], D_80070520[i1], D_80070520[i2]);
        *(s32 *)&pGT3->x0 = xy0;
        *(s32 *)&pGT3->x1 = xy1;
        gte_avsz3_b();
        *(s32 *)&pGT3->x2 = xy2;
        gte_stotz(&otz);
        addPrim(&ot[otz], pGT3);
    }

    n = g->unkA;
    sG4 = (WmSrcG4 *)sGT3;
    pG4 = (POLY_G4 *)pGT3;
    for (i = 0; i < n; sG4++, pG4++) {
        i0 = *(u32 *)&sG4->unk0[0];
        i2 = *(u32 *)&sG4->unk0[2];
        i1 = i0 >> 16;
        i3 = i2 >> 16;
        i0 &= 0xFFFF;
        i2 &= 0xFFFF;
        xy0 = D_800711A8[i0];
        xy1 = D_800711A8[i1];
        xy2 = D_800711A8[i2];
        i++;
        if (!(sG4->unk14 & 2)) {
            gte_ldsxy3(xy0, xy1, xy2);
            setaddr(pG4, 0);
            gte_nclip_b();
            gte_stopz(&opz);
            if (opz >= 0) {
                continue;
            }
        }
        otz = (D_80070520[i0] + D_80070520[i1] + D_80070520[i2] + D_80070520[i3]) >> 4;
        *(s32 *)&pG4->x0 = xy0;
        *(s32 *)&pG4->x1 = xy1;
        *(s32 *)&pG4->x2 = xy2;
        *(s32 *)&pG4->x3 = D_800711A8[i3];
        addPrim(&ot[otz], pG4);
    }

    n = g->unkC;
    sG3 = (WmSrcG3 *)sG4;
    pG3 = (POLY_G3 *)pG4;
    for (i = 0; i < n; sG3++, pG3++) {
        i0 = *(u32 *)&sG3->unk0[0];
        i2 = sG3->unk0[2];
        i1 = i0 >> 16;
        i0 &= 0xFFFF;
        xy0 = D_800711A8[i0];
        xy1 = D_800711A8[i1];
        xy2 = D_800711A8[i2];
        i++;
        if (!(sG3->unk13 & 2)) {
            gte_ldsxy3(xy0, xy1, xy2);
            setaddr(pG3, 0);
            gte_nclip_b();
            gte_stopz(&opz);
            if (opz >= 0) {
                continue;
            }
        }
        gte_ldsz3(D_80070520[i0], D_80070520[i1], D_80070520[i2]);
        *(s32 *)&pG3->x0 = xy0;
        *(s32 *)&pG3->x1 = xy1;
        gte_avsz3_b();
        *(s32 *)&pG3->x2 = xy2;
        gte_stotz(&otz);
        addPrim(&ot[otz], pG3);
    }

    n = g->unkE;
    sF4 = (WmSrcF4 *)sG3;
    pF4 = (POLY_F4 *)pG3;
    for (i = 0; i < n; sF4++, pF4++) {
        i0 = *(u32 *)&sF4->unk0[0];
        i2 = *(u32 *)&sF4->unk0[2];
        i1 = i0 >> 16;
        i3 = i2 >> 16;
        i0 &= 0xFFFF;
        i2 &= 0xFFFF;
        xy0 = D_800711A8[i0];
        xy1 = D_800711A8[i1];
        xy2 = D_800711A8[i2];
        i++;
        if (!(sF4->unkD & 2)) {
            gte_ldsxy3(xy0, xy1, xy2);
            setaddr(pF4, 0);
            gte_nclip_b();
            gte_stopz(&opz);
            if (opz >= 0) {
                continue;
            }
        }
        otz = D_80070520[i0] + D_80070520[i1] + D_80070520[i2] + D_80070520[i3];
        *(s32 *)&pF4->x0 = xy0;
        *(s32 *)&pF4->x1 = xy1;
        *(s32 *)&pF4->x2 = xy2;
        *(s32 *)&pF4->x3 = D_800711A8[i3];
        otz >>= 4;
        addPrim(&ot[otz], pF4);
    }

    n = g->unk10;
    sF3 = (WmSrcF3 *)sF4;
    pF3 = (POLY_F3 *)pF4;
    for (i = 0; i < n; sF3++, pF3++) {
        i0 = *(u32 *)&sF3->unk0[0];
        i2 = sF3->unk0[2];
        i1 = i0 >> 16;
        i0 &= 0xFFFF;
        xy0 = D_800711A8[i0];
        xy1 = D_800711A8[i1];
        xy2 = D_800711A8[i2];
        i++;
        if (!(sF3->unkB & 2)) {
            gte_ldsxy3(xy0, xy1, xy2);
            setaddr(pF3, 0);
            gte_nclip_b();
            gte_stopz(&opz);
            if (opz >= 0) {
                continue;
            }
        }
        gte_ldsz3(D_80070520[i0], D_80070520[i1], D_80070520[i2]);
        *(s32 *)&pF3->x0 = xy0;
        *(s32 *)&pF3->x1 = xy1;
        gte_avsz3_b();
        *(s32 *)&pF3->x2 = xy2;
        gte_stotz(&otz);
        addPrim(&ot[otz], pF3);
    }
}

INCLUDE_ASM("asm/us/ovl_0a/nonmatchings/ovl_0a", func_800BD130);

s32 func_800BD3A0(void) {
    u32 n;
    u32 i;
    Unk800C9DA4Tbl *tbl;
    Unk800C9DA4Trk *trk;

    tbl = (Unk800C9DA4Tbl *)((u8 *)D_800C9DA4.unk10 + D_800C9DA4.unk10->unkC);
    trk = (Unk800C9DA4Trk *)((u8 *)D_800C9DA4.unk10 + D_800C9DA4.unk10->unk10);
    n = D_800C9DA4.unk10->unk6;
    i = 0;
    D_800C9DA4.unk24 = 0;
    D_800C9DA4.unk28 = (Unk800C9DA4Ent *)D_800C9DA4.unk2C;
    for (; i < n; i++) {
        if (trk[i].unk0 & 0x40) {
            D_800C9DA4.unk28[D_800C9DA4.unk24].unk0 = 0x10;
            D_800C9DA4.unk28[D_800C9DA4.unk24].unk1 = i;
            D_800C9DA4.unk28[D_800C9DA4.unk24].unk4 = 0;
            D_800C9DA4.unk28[D_800C9DA4.unk24].unk8 = 0;
            D_800C9DA4.unk34 += 0xC;
            D_800C9DA4.unk24++;
            if (trk[i].unk0 & 0x20) {
                i += tbl->unk0 >> 8;
                tbl++;
            }
        }
    }
    return 1;
}

s32 func_800BD4D4(void) {
    Unk800C9DB4 *d = D_800C9DB4;
    u32 i;
    Unk800C9DB4Path *p;
    Unk800C9DB4Path *paths;
    u32 cnt;
    Unk800C9DB4Obj *objs;
    Unk800C9DB4Step *st;
    u32 n;
    s32 pos;
    u32 j;
    s32 v;

    i = 0;
    paths = (Unk800C9DB4Path *)((u8 *)d + d->unkC);
    objs = (Unk800C9DB4Obj *)((u8 *)d + d->unk10);
    n = d->unk4;
    for (; i < n; i++) {
        p = &paths[i];
        pos = p->unk5;
        if ((p->unk0 & 2) && (p->unk0 & 0x14) && p->unk4 == D_800CA068.unk75) {
            st = (Unk800C9DB4Step *)((u8 *)d + p->unkC);
            cnt = p->unk1;
            for (j = 0; j < cnt; j++) {
                objs[st[j].unk0].unk0 &= ~2;
            }
            objs[st[pos >> 8].unk0].unk0 |= 2;
            if (st[pos >> 8].unk1 < 0) {
                st[pos >> 8].unk1 = 0;
                p->unkA = 0;
                p->unk0 &= ~4;
                return 1;
            }
            p->unkA++;
            if ((u16)p->unkA < st[pos >> 8].unk1) {
                continue;
            }
            p->unkA = 0;
            p->unk5 = pos + p->unk8;
            v = p->unk5 >> 8;
            if (v >= p->unk1) {
                if (p->unk0 & 0x20) {
                    p->unk5 = (p->unk1 - 1) << 8;
                    func_800C17F0(i, -p->unk8);
                } else {
                    p->unk5 = 0;
                }
            } else if (v < 0) {
                if (p->unk0 & 0x20) {
                    p->unk5 = 0;
                    func_800C17F0(i, -p->unk8);
                } else {
                    p->unk5 = (p->unk1 - 1) << 8;
                }
            } else {
                continue;
            }
            p->unk0 &= ~4;
        }
    }
    return 1;
}

/* initialise the screen fade quads */
s32 func_800BD724(void) {
    u16 i;
    u16 j;

    D_800C9DA0 = 0;
    for (i = 0; i < 10; i++) {
        D_800CA140[i].unk0 = -1;
        D_800CA140[i].unk1 = 0x80;
        D_800CA140[i].unk2 = 0x80;
        D_800CA140[i].unk3 = 0x80;
        D_800CA140[i].unk4 = -1;
        D_800CA140[i].unk6 = 0;
        D_800CA140[i].unk8 = 0;
        for (j = 0; j < 4; j++) {
            SetPolyF4(&D_800CA140[i].unkC[0][j]);
            setRGB0(&D_800CA140[i].unkC[0][j], D_800CA140[i].unk1, D_800CA140[i].unk2, D_800CA140[i].unk3);
            SetPolyF4(&D_800CA140[i].unkC[1][j]);
            setRGB0(&D_800CA140[i].unkC[1][j], D_800CA140[i].unk1, D_800CA140[i].unk2, D_800CA140[i].unk3);
        }
    }
    return 1;
}

s32 func_800BD884(Unk800C9DB4 *bg, Unk800C9DB4Obj *o, Unk800CA068 *st, FieldFade *f) {
    u32 *ot;
    POLY_F4 *p;
    DR_TPAGE *tp;
    s16 x;
    s16 y;
    s16 u;
    s16 v;
    s16 w;

    ot = D_800C9DA4.unk8[*D_800C9DA4.unk0];
    x = o->unkC + bg->unk24 - (st->unk4.unk0 - 0xA0);
    y = o->unkE + bg->unk26 - (st->unk4.unk2 - 0x70);
    if (x > 0) {
        p = &f->unkC[*D_800C9DA4.unk0][0];
        u = y;
        if (u < 0) {
            u = 0;
        }
        setXY4(p, 0, u, x, u, 0, 0xE0, x, 0xE0);
        addPrim(&ot[7], p);
    }
    if (y > 0) {
        u = x + o->unk4;
        p = &f->unkC[*D_800C9DA4.unk0][1];
        if (u > 0x140) {
            u = 0x140;
        }
        v = y;
        if (v < 0) {
            v = 0;
        }
        setXY4(p, 0, 0, u, 0, 0, v, u, v);
        addPrim(&ot[7], p);
    }
    if (x + o->unk4 < 0x140) {
        u = x + o->unk4;
        v = y + o->unk6;
        p = &f->unkC[*D_800C9DA4.unk0][2];
        w = 0x140 - u;
        if (v > 0xE0) {
            v = 0xE0;
        }
        setXYWH(p, u, 0, w, v);
        addPrim(&ot[7], p);
    }
    if (y + o->unk6 < 0xE0) {
        p = &f->unkC[*D_800C9DA4.unk0][3];
        v = x;
        if (v < 0) {
            v = 0;
        }
        u = y + o->unk6;
        if (u < 0) {
            u = 0;
        }
        setXY4(p, v, u, 0x140, u, v, 0xE0, 0x140, 0xE0);
        addPrim(&ot[7], p);
    }
    if (f->unk0 < 4) {
        tp = &f->unkCC[*D_800C9DA4.unk0];
        SetDrawTPage(tp, 1, 1, GetTPage(2, f->unk0, 0, 0));
        addPrim(&ot[7], tp);
    }
    return 1;
}

s32 func_800BDC44(void) {
    SVECTOR v;
    s32 sxy;
    Unk800CA068 *st;
    Unk800C9DB4 *bg;
    Unk800C9DB4Obj *objs;
    Unk800C9DB4Obj *o;
    Unk8004AB64 *a;
    FieldFade *f;
    u8 i;
    u8 mode;
    u16 sx;
    u16 sy;
    s16 y;
    s16 x;

    if (D_800C9DA0 == 0) {
        return 0;
    } else {
        st = &D_800CA068;
        bg = D_800C9DB4;
        objs = (Unk800C9DB4Obj *)((u8 *)bg + bg->unk10);
        mode = D_8006794C->unk8;
        if (mode == 5) {
            a = (Unk8004AB64 *)D_8006794C->unk1C->unk8E0[D_8006794C->unk1C->unk8E4.field->unk8].unkC;
        } else if (mode == 1) {
            a = func_8004AE00();
        } else {
            a = NULL;
        }
        if (a != NULL) {
            v.vx = a->unk14.vx;
            v.vy = a->unk14.vy - D_800CA068.unk76;
            v.vz = a->unk14.vz;
        } else {
            v.vx = 0;
            v.vy = 0;
            v.vz = 0;
        }
        func_800C0F08();
        gte_SetRotMatrix(D_800CA068.unk1C);
        gte_SetTransMatrix(D_800CA068.unk1C);
        gte_SetGeomScreen(*D_800CA068.unk20);
        gte_ldv0(&v);
        gte_rtps();
        gte_stsxy(&sxy);
        sx = sxy;
        sy = sxy >> 16;
        f = D_800CA140;
        for (i = 0; i < D_800C9DA0; i++, f++) {
            o = (Unk800C9DB4Obj *)((u16)f->unk4 * sizeof(Unk800C9DB4Obj) + (u32)objs);
            if ((o->unk0 & 2) && o->unk24 == st->unk75) {
                x = st->unk4.unk0 + (sx - bg->unk24 - (f->unk6 + 0xA0));
                y = f->unk8;
                o->unkC = x;
                o->unkE = st->unk4.unk2 + (sy - bg->unk24 - (y + 0x70));
                if (f->unk0 >= 0) {
                    func_800BD884(bg, o, st, f);
                }
            }
        }
        return 1;
    }
}

s32 func_800BDED4(u8 *arg0, BgSprt **arg1) {
    Unk800C9DA4Hdr *h;
    Unk800C9DB4Cam *c;
    s32 x;
    s32 y;
    u32 n;
    u32 size;
    u32 i;
    Unk800C9DB4Obj *o;
    Unk800C9DB4Obj *objs;
    Unk800C9DB4Obj *p;

    h = D_800C9DA4.unk10;
    func_800C0BCC(D_800CA068.unk75);
    c = &((Unk800C9DB4Cam *)((u8 *)h + h->unk18))[D_800CA068.unk75];
    x = (c->unk28 + c->unk2A) / 2;
    D_800CA068.unk4.unk0 = (D_800CA068.unkA4 = x);
    D_800CA068.unk4.unk2 = (D_800CA068.unkA6 = (c->unk2C + c->unk2E) / 2);
    x -= 160;
    h->unk30 = h->unk24 - x;
    n = h->unk6;
    objs = (Unk800C9DB4Obj *)((u8 *)h + h->unk10);
    size = n * 3;
    y = D_800CA068.unk4.unk2 - 112;
    h->unk32 = h->unk26 - (s16)y;
    D_800C9DA4.unk1C = arg0;
    while (size / 4 * 4 != size) {
        size++;
    }
    D_800C9DA4.unk20 = arg0 + size;
    size += n;
    while (size / 4 * 4 != size) {
        size++;
    }
    *arg1 = (BgSprt *)(arg0 + size);
    for (i = 0; i < 4; i++) {
        D_800CA068.unkA8[i][0] = 0;
        D_800CA068.unkA8[i][1] = 0;
        D_800CA068.unkB8[i][0] = c->unk24;
        D_800CA068.unkB8[i][1] = c->unk26;
        D_800CA068.unkC8[i] = 0x100;
        D_800CA068.unkD0[i] = 0x100;
    }
    for (i = 0; i < n; i++) {
        o = &objs[i];
        D_800C9DA4.unk1C[i * 3 + 0] = 0x80;
        D_800C9DA4.unk1C[i * 3 + 1] = 0x80;
        D_800C9DA4.unk1C[i * 3 + 2] = 0x80;
        D_800C9DA4.unk20[i] = 0;
        p = o;
        if (func_800BD130(p, *arg1, arg1) != 1) {
            return 0;
        }
        o->unk10 = -0x8000;
        p->unk12 = 0x7FFF;
        o->unk14 = -0x8000;
        p->unk16 = 0x7FFF;
    }
    D_800CA068.unk73 = 0;
    D_800CA068.unk72 = 0;
    D_800CA068.unk0 &= ~0xC00;
    return 1;
}

INCLUDE_ASM("asm/us/ovl_0a/nonmatchings/ovl_0a", func_800BE158);

s32 func_800BEDE8(Unk800C9DA4Hdr *arg0) {
    s32 i;
    s32 n;
    Unk800C9DB4Obj *o;
    Unk800C9DB4Obj *objs;
    s32 v;

    n = arg0->unk6;
    objs = (Unk800C9DB4Obj *)((u8 *)arg0 + arg0->unk10);
    for (i = 0; i < n; i++) {
        o = &objs[i];
        if (o->unk0 & 4) {
            if (o->unk1C != 0 && o->unk1C != 0x7FFF) {
                v = (o->unkC << 8) | (o->unk20 & 0xFF);
                v = v + o->unk1C;
                o->unk20 = v & 0xFF;
                o->unkC = (v >> 8) % o->unk4;
            }
            if (o->unk1E != 0 && o->unk1E != 0x7FFF) {
                v = (o->unkE << 8) | (o->unk22 & 0xFF);
                v = v + o->unk1E;
                o->unk22 = v & 0xFF;
                o->unkE = (v >> 8) % o->unk6;
            }
        }
        if (o->unk0 & 0x80) {
            if (o->unk25) {
                if (o->unk1E != 0x7FFF) {
                    v = (o->unkE << 8) | (o->unk22 & 0xFF);
                    v = v + o->unk1E;
                    o->unk22 = v & 0xFF;
                    o->unkE = (v >> 8) % o->unk6;
                }
            } else if (o->unk1C != 0x7FFF) {
                v = (o->unkC << 8) | (o->unk20 & 0xFF);
                v = v + o->unk1C;
                o->unk20 = v & 0xFF;
                o->unkC = (v >> 8) % o->unk4;
            }
        }
        if (o->unk0 & 8) {
            v = (o->unk8 << 8) + ((s16)D_800CA068.unk4.unk0 - D_800CA068.unkA4) * o->unk1C;
            o->unkC = v >> 8;
            o->unk20 = v & 0xFF;
            v = (o->unkA << 8) + ((s16)D_800CA068.unk4.unk2 - D_800CA068.unkA6) * o->unk1E;
            o->unkE = v >> 8;
            o->unk22 = v & 0xFF;
        }
    }
    if (D_800CA068.unk0 & 0x80) {
        D_800CA068.unk0 &= ~0x80;
        D_800CA068.unk8.unk0 = D_800CA068.unk4.unk0;
        D_800CA068.unk8.unk2 = D_800CA068.unk4.unk2;
    }
    return 1;
}

s32 func_800BF0C4(void) {
    s16 dur;
    u16 ox;
    u16 oy;
    Unk800C9DB4Obj *o;
    Unk800C9DB4 *h;
    s16 wx;
    s16 wy;
    s32 dx;
    s32 flags;
    s32 smooth;
    s32 ax;
    s32 ay;
    s32 i;
    Unk800C9DB4Cam *c;
    s16 cnt;
    s32 s;
    s32 v;
    s32 t;
    s32 y;
    s32 mode;
    flags = D_800CA068.unk0;
    if (!(flags & 0x40)) {
        return 0;
    }
    mode = flags & 7;
    smooth = flags & 8;
    if (mode == 1 || mode == 2 || mode == 3) {
        ox = D_800CA068.unk4.unk0;
        oy = D_800CA068.unk4.unk2;
        h = D_800C9DB4;
        wx = D_800CA068.unk14 - D_800CA068.unk10.unk0;
        wy = D_800CA068.unk16 - D_800CA068.unk10.unk2;
        cnt = D_800CA068.unk18;
        dur = D_800CA068.unk1A;
        if (!smooth) {
            D_800CA068.unk4.unk0 = D_800CA068.unk10.unk0 + wx * cnt / dur;
            D_800CA068.unk4.unk2 = D_800CA068.unk10.unk2 + wy * cnt / dur;
        } else {
            y = func_8004BB7C(((cnt << 16) >> 5) / dur + 0x800) + 0x1000;
            ax = wx * y / 0x2000;
            ay = wy * y / 0x2000;
            D_800CA068.unk4.unk0 = D_800CA068.unk10.unk0 + ax;
            D_800CA068.unk4.unk2 = D_800CA068.unk10.unk2 + ay;
        }
        ox = D_800CA068.unk4.unk0 - ox;
        oy = D_800CA068.unk4.unk2 - oy;
        o = (Unk800C9DB4Obj *)((u8 *)h + h->unk10);
        for (i = 0; i < h->unk6; o++, i++) {
            if (o->unk0 & 4) {
                if (o->unk1C != 0) {
                    o->unkC += ox;
                }
                if (o->unk1E != 0) {
                    o->unkE += oy;
                }
                dx = (s16)ox << 16;
            s = D_800CA068.unkC8[o->unk25_1] << 8;
            v = dx;
            if (s != 0x10000) {
                if (s < 0) {
                    v = (o->unkC << 16) - func_800C1DC4(v, -s);
                } else {
                    v = (o->unkC << 16) + func_800C1DC4(v, s);
                }
                o->unkC = v >> 16;
                o->unk20 = (v >> 8) & 0xFF;
            }
            s = D_800CA068.unkD0[o->unk25_1] << 8;
            v = oy << 16;
            if (s != 0x10000) {
                if (s < 0) {
                    v = (o->unkE << 16) - func_800C1DC4(v, -s);
                } else {
                    v = (o->unkE << 16) + func_800C1DC4(v, s);
                }
                o->unkE = v >> 16;
                o->unk22 = (v >> 8) & 0xFF;
            }
            } else if (o->unk0 & 0x80) {
                if (o->unk25) {
                    o->unkE += oy;
                } else {
                    o->unkC += ox;
                }
            s = D_800CA068.unkC8[o->unk25_1] << 8;
            v = (s16)ox << 16;
            if (s != 0x10000) {
                if (s < 0) {
                    v = (o->unkC << 16) - func_800C1DC4(v, -s);
                } else {
                    v = (o->unkC << 16) + func_800C1DC4(v, s);
                }
                o->unkC = v >> 16;
                o->unk20 = (v >> 8) & 0xFF;
            }
            s = D_800CA068.unkD0[o->unk25_1] << 8;
            v = oy << 16;
            if (s != 0x10000) {
                if (s < 0) {
                    v = (o->unkE << 16) - func_800C1DC4(v, -s);
                    o->unkE = v >> 16;
                    o->unk22 = (v >> 8) & 0xFF;
                } else {
                    v = (o->unkE << 16) + func_800C1DC4(v, s);
                    o->unkE = v >> 16;
                    o->unk22 = (v >> 8) & 0xFF;
                }
            }
            }
        }
        s = h->unk18;
        s = (s32)h - -s; /* keeps the h + off operand order (plus h s) */
        s = s + D_800CA068.unk75 * sizeof(Unk800C9DB4Cam);
        v = 160;
        y = *(s16 *)&D_800CA068.unk4.unk0 - (((Unk800C9DB4Cam *)s)->unk24 >> 1);
        i = v - y;
        t = 112 - (*(s16 *)&D_800CA068.unk4.unk2 - (((Unk800C9DB4Cam *)s)->unk26 >> 1));
        D_800CA068.unkC = i;
        D_800CA068.unkE = t;
        if (mode == 1) {
            D_800CA068.unk0 = (D_800CA068.unk0 & ~7) | 2;
        }
        if (++D_800CA068.unk18 > dur) {
            if ((D_800CA068.unk0 & 7) == 2) {
                D_800CA068.unk0 = (D_800CA068.unk0 & ~7) | 0x84;
            } else {
                D_800CA068.unk0 = (D_800CA068.unk0 & ~7) | 0x80;
            }
        }
    }
    return 1;
}

INCLUDE_ASM("asm/us/ovl_0a/nonmatchings/ovl_0a", func_800BF660);

/* reset the field background state; returns the end of its allocations */
BgSprt *func_800BFCE8(void) {
    SVECTOR v;
    BgSprt *spr;
    Unk800C9DA4Hdr *h;
    Unk800C9DA4Anim *anims;
    Unk800C9DA4Trk *trk;
    u32 i;
    u32 n;
    s32 t;
    s32 x;
    union {
        s32 w;
        DVECTOR v;
    } u; /* projected origin */

    D_800CA068.unk0 = 0x60;
    D_800CA068.unk74 = 0x40;
    D_800CA068.unk76 = 0x144;
    D_800CA068.unk78 = 0xE10;
    *(s32 *)&D_800CA068.unk4 = 0;
    *(s32 *)&D_800CA068.unk8 = 0;
    *(s32 *)&D_800CA068.unkC = 0;
    *(s32 *)&D_800CA068.unk10 = 0;
    *(s32 *)&D_800CA068.unk14 = 0;
    *(s32 *)&D_800CA068.unk18 = 0;
    *(s32 *)&D_800CA068.unk24 = 0;
    *(s32 *)&D_800CA068.unk28 = 0;
    *(s32 *)&D_800CA068.unk2C = 0;
    D_800CA068.unk75 = 0;
    D_800CA068.unk9C = 0;
    D_800CA068.unkA0 = 0;
    D_800CA068.unkA1 = 0xFF;
    D_800CA068.unkA2 = 0xFF;
    D_800CA068.unkA3 = 0xFF;
    *(s32 *)&D_800CA068.unkA4 = 0;
    D_800CA068.unk1C = (MATRIX *)D_800C9DA4.unk14;
    D_800CA068.unk20 = (u16 *)D_800C9DA4.unk4;
    if (func_800BDED4(D_800C9DA4.unk18, &spr) != 1) {
        return NULL;
    }
    h = D_800C9DA4.unk10;
    anims = (Unk800C9DA4Anim *)((u8 *)h + h->unkC);
    trk = (Unk800C9DA4Trk *)((u8 *)h + h->unk10);
    n = h->unk4;
    for (i = 0; i < n; i++) {
        anims[i].unk4 &= 0xFF;
        anims[i].unk8 = 0x100;
        anims[i].unkA = 0;
        anims[i].unk0 = 1;
        trk[*((u8 *)h + anims[i].unkC)].unk0 |= 2;
    }
    func_800BD3A0();
    func_800BD724();
    v.vx = 0;
    v.vy = 0;
    v.vz = 0;
    gte_SetGeomOffset(0xA0, 0x70);
    gte_SetRotMatrix(D_800CA068.unk1C);
    gte_SetTransMatrix(D_800CA068.unk1C);
    gte_ldv0(&v);
    gte_rtps();
    gte_stsxy(&u.w);
    t = u.w;
    x = u.v.vx;
    D_800CA068.unk2C = x;
    D_800CA068.unk2E = t >> 16;
    return spr;
}

s32 func_800BFEE0(void) {
    Unk800C9DA4Hdr *h;
    Unk800C9DB4Obj *o;
    Unk800CA068 *c;
    u32 i;
    s32 j;

    func_800BD4D4();
    /* the switch results are dead; only the loads and the hoisted 0x10/0x40 survive */
    do {
        for (i = 0; i < D_800C9DA4.unk24; i++) {
            switch (D_800C9DA4.unk28[i].unk0) {
            case 0x10:
                j = 1;
                break;
            case 0x40:
                j = 2;
                break;
            }
        }
    } while (0);
    j = 0;
    func_800BF0C4();
    func_800BF660();
    func_800BEDE8(D_800C9DA4.unk10);
    func_800BDC44();
    c = &D_800CA068;
    h = D_800C9DA4.unk10;
    h->unk30 = h->unk24 - (s16)(c->unk4.unk0 - 0xA0);
    h->unk32 = h->unk26 - (s16)(c->unk4.unk2 - 0x70);
    o = (Unk800C9DB4Obj *)((u8 *)h + h->unk10);
    for (j = 0; j < h->unk6; j++) {
        if ((o[j].unk0 & 2) && o[j].unk24 == c->unk75) {
            func_800BE158(&o[j], D_800C9DA4.unk1C + j * 3);
        }
    }
    return 1;
}

INCLUDE_ASM("asm/us/ovl_0a/nonmatchings/ovl_0a", func_800C0038);

/* add a screen fade: rectangle (arg0..arg2), mode arg3, colour arg4..arg6 */
s32 func_800C02D0(s16 arg0, s16 arg1, s16 arg2, s8 arg3, u8 arg4, u8 arg5, u8 arg6) {
    u32 i;

    D_800CA140[D_800C9DA0].unk4 = arg0;
    D_800CA140[D_800C9DA0].unk6 = arg1;
    D_800CA140[D_800C9DA0].unk8 = arg2;
    D_800CA140[D_800C9DA0].unk0 = arg3;
    if (arg3 >= 0) {
        D_800CA140[D_800C9DA0].unk1 = arg4;
        D_800CA140[D_800C9DA0].unk2 = arg5;
        D_800CA140[D_800C9DA0].unk3 = arg6;
        for (i = 0; i < 4; i++) {
            SetPolyF4(&D_800CA140[D_800C9DA0].unkC[0][i]);
            setRGB0(&D_800CA140[D_800C9DA0].unkC[0][i], arg4, arg5, arg6);
            SetPolyF4(&D_800CA140[D_800C9DA0].unkC[0][i + 4]); /* = unkC[1][i] */
            setRGB0(&D_800CA140[D_800C9DA0].unkC[1][i], arg4, arg5, arg6);
            if (arg3 >= 4) {
                setSemiTrans(&D_800CA140[D_800C9DA0].unkC[0][i], 0);
                setSemiTrans(&D_800CA140[D_800C9DA0].unkC[1][i], 0);
            } else {
                setSemiTrans(&D_800CA140[D_800C9DA0].unkC[0][i], 1);
                setSemiTrans(&D_800CA140[D_800C9DA0].unkC[1][i], 1);
            }
        }
    }
    D_800C9DA0++;
    return 1;
}

/* toggle a background object's tint */
s32 func_800C0650(s32 arg0, u8 arg1, u8 arg2, u8 arg3) {
    Unk800C9DA4Hdr *h;
    Unk800C9DB4Obj *o;
    BgSprt *s;
    u8 t;
    u16 n;
    s32 off;
    u16 i;

    t = D_800C9DA4.unk20[arg0];
    h = D_800C9DA4.unk10;
    if (t != 0) {
        o = &((Unk800C9DB4Obj *)((u8 *)h + h->unk10))[arg0];
        n = o->unk26;
        s = o->unk30;
        off = 0;
        if (*D_800C9DA4.unk0 != 0) {
            off = n;
        }
        for (i = 0; i < n; i++) {
            setRGB0(&s[i + off].unk8, arg1, arg2, arg3);
        }
        D_800C9DA4.unk20[arg0] = 0;
        return 0;
    }
    D_800C9DA4.unk1C[arg0 * 3 + 0] = arg1;
    D_800C9DA4.unk1C[arg0 * 3 + 1] = arg2;
    D_800C9DA4.unk1C[arg0 * 3 + 2] = arg3;
    o = &((Unk800C9DB4Obj *)((u8 *)h + h->unk10))[arg0];
    n = o->unk26;
    s = o->unk30;
    off = 0;
    if (*D_800C9DA4.unk0 != 0) {
        off = n;
    }
    for (i = 0; i < n; i++) {
        setRGB0(&s[i + off].unk8, arg1, arg2, arg3);
    }
    D_800C9DA4.unk20[arg0] = 1;
    return 1;
}

INCLUDE_ASM("asm/us/ovl_0a/nonmatchings/ovl_0a", func_800C07D8);

s32 func_800C0BCC(s32 arg0) {
    Unk800C9DB4Cam *c;
    Unk800C9DB4 *h;

    h = D_800C9DB4;
    c = (Unk800C9DB4Cam *)((u8 *)h + h->unk18) + arg0;
    *D_800CA068.unk20 = c->unk0;
    gte_SetGeomScreen(c->unk0);
    D_800CA068.unk1C->m[0][0] = c->unk2[0][0];
    D_800CA068.unk1C->m[0][1] = c->unk2[0][1];
    D_800CA068.unk1C->m[0][2] = c->unk2[0][2];
    D_800CA068.unk1C->m[1][0] = c->unk2[1][0];
    D_800CA068.unk1C->m[1][1] = c->unk2[1][1];
    D_800CA068.unk1C->m[1][2] = c->unk2[1][2];
    D_800CA068.unk1C->m[2][0] = c->unk2[2][0];
    D_800CA068.unk1C->m[2][1] = c->unk2[2][1];
    D_800CA068.unk1C->m[2][2] = c->unk2[2][2];
    D_800CA068.unk1C->t[0] = c->unk14[0];
    D_800CA068.unk1C->t[1] = c->unk14[1];
    D_800CA068.unk1C->t[2] = c->unk14[2];
    h->unk2A = c->unk2A;
    h->unk2E = c->unk2E;
    D_800CA068.unk75 = arg0;
    D_800CA068.unk0 |= 0x80;
    D_8006794C->unk1C->unk8E4.field->unkC->unk14->unk30 = c->unk30;
    return 1;
}

void func_800C0D38(void) {
}

void func_800C0D40(s32 arg0, s32 arg1, s32 arg2, s32 arg3, s32 arg4, u8 *arg5) {
    D_800C9DA4.unk0 = (u8 *)arg0;
    D_800C9DA4.unk4 = arg1;
    D_800C9DA4.unk8[0] = (u32 *)arg2;
    D_800C9DA4.unk8[1] = (u32 *)arg3;
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

s32 func_800C0F08(void) {
    Unk800C9DB4Cam *c = func_800C9DB4_cam(0);

    c += D_800CA068.unk75;
    if (!(func_80012AD4() & 2) || D_80071E34 < 2) {
        gte_SetGeomOffset((s16)c->unk20 + D_800CA068.unkC, (s16)c->unk22 + D_800CA068.unkE);
    }
    return 1;
}

s32 func_800C0FB0(void) {
    if (!(func_80012AD4() & 2)) {
        gte_SetGeomOffset(0xA0, 0x70);
    }
    return 1;
}

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

s32 func_800C104C(void) {
    Unk800C9DB4Cam *c;

    if (!(D_800CA068.unk0 & 0x20)) {
        return 0;
    }
    if (D_800C9DB4 != NULL) {
        c = func_800C9DB4_cam(D_800CA068.unk75);
        D_800CA068.unk24 = c->unk28;
        D_800CA068.unk26 = c->unk2A;
        D_800CA068.unk28 = c->unk2C;
        D_800CA068.unk2A = c->unk2E;
        c->unk28 = -0x8000;
        c->unk2A = 0x7FFF;
        c->unk2C = -0x8000;
        c->unk2E = 0x7FFF;
        D_800CA068.unk0 &= ~0x20;
    }
    return 1;
}

s32 func_800C1100(void) {
    Unk800C9DB4Cam *c;

    if (D_800CA068.unk0 & 0x20) {
        return 0;
    }
    if (D_800C9DB4 != NULL) {
        c = func_800C9DB4_cam(D_800CA068.unk75);
        c->unk28 = D_800CA068.unk24;
        c->unk2A = D_800CA068.unk26;
        c->unk2C = D_800CA068.unk28;
        c->unk2E = D_800CA068.unk2A;
        D_800CA068.unk0 |= 0x20;
    }
    return 1;
}

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

/* move a background object, clamped to its limits */
s32 func_800C11E8(s32 arg0, s16 arg1, s16 arg2, s16 arg3) {
    Unk800C9DB4Obj *o = &((Unk800C9DB4Obj *)((u8 *)D_800C9DB4 + D_800C9DB4->unk10))[arg0];
    s16 x = o->unk8 + arg1;
    s16 y = o->unkA + arg2;
    s16 z = arg3 + o->unk2;

    if (x < o->unk10) {
        x = o->unk10;
    } else if (x > o->unk12) {
        x = o->unk12;
    }
    if (y < o->unk14) {
        y = o->unk14;
    } else if (y > o->unk16) {
        y = o->unk16;
    }
    o->unk8 = x;
    o->unk2 = z;
    o->unkA = y;
    o->unkC = x;
    o->unkE = y;
    o->unk1 = z;
    return 1;
}

s32 func_800C130C(s32 arg0, u8 *arg1, u8 *arg2, u8 *arg3) {
    *arg1 = D_800C9DA4.unk1C[arg0 * 3];
    *arg2 = D_800C9DA4.unk1C[arg0 * 3 + 1];
    *arg3 = D_800C9DA4.unk1C[arg0 * 3 + 2];
    return 1;
}

s32 func_800C1368(s32 arg0, u8 arg1, u8 arg2, u8 arg3) {
    Unk800C9DA4Hdr *h;
    Unk800C9DB4Obj *o;
    BgSprt *s;
    u16 n;
    s32 off;
    u16 i;

    h = D_800C9DA4.unk10;
    D_800C9DA4.unk1C[arg0 * 3 + 0] = arg1;
    D_800C9DA4.unk1C[arg0 * 3 + 1] = arg2;
    D_800C9DA4.unk1C[arg0 * 3 + 2] = arg3;
    o = &((Unk800C9DB4Obj *)((u8 *)h + h->unk10))[arg0];
    n = o->unk26;
    s = o->unk30;
    off = 0;
    if (*D_800C9DA4.unk0 != 0) {
        off = n;
    }
    for (i = 0; i < n; i++) {
        setRGB0(&s[i + off].unk8, arg1, arg2, arg3);
    }
    return 1;
}

s32 func_800C142C(s32 arg0, s32 arg1) {
    Unk800C9DB4Obj *o = func_800C9DB4_obj(arg0);
    u8 v;

    if (arg1 != 0) {
        v = o->unk0 | 2;
    } else {
        v = o->unk0 & ~2;
    }
    o->unk0 = v;
    return 1;
}

s32 func_800C147C(s32 arg0, s32 arg1, s16 arg2, s16 arg3) {
    Unk800C9DB4Obj *o = func_800C9DB4_obj(arg0);

    if (arg1 != 0) {
        o->unk0 |= 4;
    } else {
        o->unk0 &= ~4;
    }
    o->unk1C = arg2;
    o->unk20 = 0;
    o->unk1E = arg3;
    o->unk22 = 0;
    return 1;
}

s32 func_800C14DC(s32 arg0, s32 arg1) {
    Unk800C9DB4Obj *o = func_800C9DB4_obj(arg0);
    u8 v;

    if (arg1 != 0) {
        v = o->unk0 | 1;
    } else {
        v = o->unk0 & ~1;
    }
    o->unk0 = v;
    return 1;
}

s32 func_800C152C(s32 arg0, s32 arg1, s16 arg2, s16 arg3, s32 arg4) {
    Unk800C9DB4Obj *o = func_800C9DB4_obj(arg0);

    if (arg1 != 0) {
        o->unk0 |= 0x80;
    } else {
        o->unk0 &= ~0x80;
    }
    if (arg4 != 0) {
        o->unk1C = arg3;
        o->unk1E = arg2;
        o->unk25 = 1;
    } else {
        o->unk1C = arg2;
        o->unk1E = arg3;
        o->unk25 = 0;
    }
    o->unk20 = 0;
    o->unk22 = 0;
    return 1;
}

s32 func_800C15BC(s32 arg0, s32 arg1, s16 arg2, s16 arg3) {
    Unk800C9DB4Obj *o = func_800C9DB4_obj(arg0);

    if (arg1 != 0) {
        o->unk0 |= 8;
    } else {
        o->unk0 &= ~8;
    }
    o->unk1C = arg2;
    o->unk1E = arg3;
    return 1;
}

s32 func_800C1614(s32 arg0, s16 arg1, s16 arg2) {
    Unk800C9DB4Obj *o = func_800C9DB4_obj(arg0);

    o->unkC = arg1;
    o->unk8 = arg1;
    o->unkE = arg2;
    o->unkA = arg2;
    D_800CA068.unk0 |= 0x80;
    return 1;
}

s32 func_800C1668(s32 arg0, s32 arg1) {
    Unk800C9DB4Obj *o = func_800C9DB4_obj(arg0);

    o->unk25_1 = arg1;
    return 1;
}

s32 func_800C16B0(s32 arg0, s32 arg1) {
    Unk800C9DB4Path *p = func_800C9DB4_path(arg0);

    p->unkA = 0;
    p->unk0 |= 2;
    p->unk5 = arg1 << 8;
    return 1;
}

s32 func_800C16F4(s32 arg0, s32 arg1) {
    s32 i;
    Unk800C9DB4Path *p;
    Unk800C9DB4Step *st;
    Unk800C9DB4Obj *objs;

    i = 0;
    p = func_800C9DB4_path(arg0);
    st = func_800C9DB4_steps(p);
    objs = func_800C9DB4_obj(0);
    for (; i < p->unk1; st++, i++) {
        if (i == arg1) {
            objs[st->unk0].unk0 |= 2;
        } else {
            objs[st->unk0].unk0 &= ~2;
        }
    }
    p->unk0 = 1;
    return 1;
}

s32 func_800C17AC(s32 arg0, s32 arg1) {
    Unk800C9DB4Path *p = func_800C9DB4_path(arg0);
    u8 v;

    if (arg1 != 0) {
        v = p->unk0 | 6;
    } else {
        v = p->unk0 & ~6;
    }
    p->unk0 = v;
    return 1;
}

s32 func_800C17F0(s32 arg0, s16 arg1) {
    func_800C9DB4_path(arg0)->unk8 = arg1;
    return 1;
}

s32 func_800C1818(s32 arg0, u8 arg1) {
    Unk800C9DB4Path *p = func_800C9DB4_path(arg0);
    Unk800C9DB4Step *st = func_800C9DB4_steps(p);
    s32 n = p->unk1;
    s32 i;

    for (i = 0; i < n; i++) {
        st[i].unk1 = arg1;
    }
    return 1;
}

s32 func_800C1868(s32 arg0, s32 arg1, u8 arg2) {
    func_800C9DB4_steps(func_800C9DB4_path(arg0))[arg1].unk1 = arg2;
    return 1;
}

s32 func_800C18A0(s32 arg0, s32 arg1) {
    func_800C9DB4_path(arg0)->unk0 |= arg1 & 0x30;
    return 1;
}

s32 func_800C18D4(s32 arg0, u32 arg1, u32 arg2) {
    Unk800C9DB4 *b = D_800C9DB4;
    Unk800C9DB4Path *p = &((Unk800C9DB4Path *)((u8 *)b + b->unkC))[arg0];
    Unk800C9DB4Step *st = (Unk800C9DB4Step *)((u8 *)b + p->unkC);

    p->unkA = 0;
    p->unk0 |= 6;
    p->unk5 = arg1 << 8;
    st[arg2].unk1 = -1;
    if ((p->unk8 > 0 && arg2 < arg1) || (p->unk8 < 0 && arg1 < arg2)) {
        p->unk8 = -p->unk8;
    }
    return 1;
}

s32 func_800C196C(s32 arg0, s32 arg1) {
    Unk800C9DB4 *d = D_800C9DB4;
    Unk800C9DB4Path *p = &((Unk800C9DB4Path *)((u8 *)d + d->unkC))[arg0];
    Unk800C9DB4Obj *objs;
    Unk800C9DB4Step *st;
    s32 i;

    p->unk8 = 0x100;
    i = 0;
    p->unkA = 0;
    p->unk0 = 1;
    p->unk5 = 0;
    objs = (Unk800C9DB4Obj *)((u8 *)d + d->unk10);
    st = (Unk800C9DB4Step *)((u8 *)d + p->unkC);
    for (; i < p->unk1; i++) {
        if (arg1 != 0 && i == 0) {
            objs[st[i].unk0].unk0 |= 2;
        } else {
            objs[st[i].unk0].unk0 &= ~2;
        }
    }
    return 1;
}

s32 func_800C1A4C(s32 arg0, u8 *arg1) {
    *arg1 = D_800C9DCC[arg0].unk0;
    return 1;
}

s32 func_800C1A78(s32 arg0) {
    return D_800C9DD0 + (arg0 << 9);
}

/* set the camera scroll limits */
s32 func_800C1A8C(s32 arg0, s16 arg1, s16 arg2, s16 arg3, s16 arg4) {
    Unk800C9DB4Cam *c = &((Unk800C9DB4Cam *)((u8 *)D_800C9DB4 + D_800C9DB4->unk18))[arg0];
    s16 v;

    c->unk28 = v = arg1 + 160;
    if (v > c->unk24 - 160) {
        c->unk28 = c->unk24 - 160;
    }
    c->unk2A = v = arg2 - 160;
    if (v < 160) {
        c->unk2A = 160;
    }
    c->unk2C = v = arg3 + 112;
    if (v > c->unk26 - 112) {
        c->unk2C = c->unk26 - 112;
    }
    c->unk2E = v = arg4 - 112;
    if (v < 112) {
        c->unk2E = 112;
    }
    return 1;
}

s32 func_800C1B5C(u8 *arg0) {
    *arg0 = D_800CA068.unk75;
    return 1;
}

s32 func_800C1B74(s32 arg0, s16 arg1, s16 arg2) {
    Unk800C9DB4Cam *c = func_800C9DB4_cam(arg0);

    c->unk20 = arg1;
    c->unk22 = arg2;
    return 1;
}

s32 func_800C1BB4(s32 arg0, u16 *arg1, u16 *arg2) {
    Unk800C9DB4Cam *c = func_800C9DB4_cam(arg0);

    *arg1 = c->unk20;
    *arg2 = c->unk22;
    return 1;
}

s32 func_800C1C04(s32 arg0) {
    return arg0 * 4 + 0x2C;
}

s32 func_800C1C10(s32 arg0) {
    s32 r = D_800C9DA4.unk2C + D_800C9DA4.unk34;

    D_800C9DA4.unk34 += arg0;
    return r;
}

s32 func_800C1C30(void) {
    Unk800C9DB4 *d = D_800C9DB4;
    u32 i;
    Unk800C9DB4Path *p;
    Unk800C9DB4Obj *objs;
    u16 n;

    i = 0;
    p = (Unk800C9DB4Path *)((u8 *)d + d->unkC);
    objs = (Unk800C9DB4Obj *)((u8 *)d + d->unk10);
    n = d->unk4;
    for (; i < n; i++) {
        p[i].unk8 = 0x100;
        p[i].unkA = 0;
        p[i].unk0 = 1;
        p[i].unk5 = 0;
        objs[((Unk800C9DB4Step *)((u8 *)d + p[i].unkC))->unk0].unk0 |= 2;
    }
    return 1;
}

s32 func_800C1CC0(void) {
    u32 i;
    s32 ret;
    s32 type;

    ret = 1;
    for (i = 0; i < D_800C9DA4.unk24; i++) {
        type = D_800C9DA4.unk28[i].unk0;
        if (type == 0x10) {
            type = 0;
        } else if (type == 0x40) {
            type = ret;
        }
    }
    return ret;
}

s32 func_800C1D08(void) {
    return 1;
}

s32 func_800C1D10(s32 arg0, u8 arg1, u8 arg2, u8 arg3) {
    Unk800C9DB4Obj *o = func_800C9DB4_obj(arg0);
    u16 n = o->unk26;
    BgSprt *s = o->unk30;
    u16 i;

    for (i = 0; i < n; i++) {
        setRGB0(&s[i].unk8, arg1, arg2, arg3);
        setRGB0(&s[i + n].unk8, arg1, arg2, arg3);
    }
    return 1;
}

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

/* does the segment from arg0 to arg1 cross the edge arg2-arg3 (on axes x, z)? */
s32 func_800C1E14(s16 *arg0, s16 *arg1, s16 *arg2, s16 *arg3, u8 x, u8 z) {
    s32 t2;
    s32 t3;
    s32 t4;
    s32 ex;
    s32 ez;

    t2 = (arg1[x] - arg0[x]) * (ez = arg3[z] - arg2[z]);
    ex = arg3[x] - arg2[x];
    t3 = (arg0[z] - arg2[z]) * ex;
    t2 -= (arg1[z] - arg0[z]) * ex;
    t3 -= (arg0[x] - arg2[x]) * ez;
    if (ABS(t3) <= ABS(t2)) {
        if ((t3 >= 0 && t2 >= 0) || (t3 <= 0 && t2 < 0)) {
            t4 = (arg0[z] - arg2[z]) * (arg1[x] - arg0[x]);
            t4 -= (arg0[x] - arg2[x]) * (arg1[z] - arg0[z]);
            if ((t4 >= 0 && t2 >= 0) || (t4 <= 0 && t2 < 0)) {
                return 1;
            }
        }
    }
    return 0;
}

Unk800C9E00 *func_800C1F98(Unk800C9DF4 *arg0, Unk800C9E00 *arg1, s32 arg2) {
    Unk800C9E00 *n;
    Unk8004AB64 *a;
    s16 id;
    u8 m;
    u8 mode;

    id = arg1->unk18[arg2];
    if (id == -1) {
        goto fail;
    }
    n = &D_800C9DF0.unk10[id];
    if (!(n->unk0 & 1)) {
        goto fail;
    }
    if ((m = (n->unk0 >> 8) & D_800C9DF0.unk1) & 0xC0) {
        mode = D_8006794C->unk8;
        if (mode == 5) {
            a = (Unk8004AB64 *)D_8006794C->unk1C->unk8E0[D_8006794C->unk1C->unk8E4.field->unk8].unkC;
        } else if (mode == 1) {
            a = func_8004AE00();
        } else {
            a = NULL;
        }
        if (arg0->unk24 == a) {
            if (m & 0x80) {
            fail:
                return NULL;
            }
            return n;
        }
        if (m & 0x40) {
            return NULL;
        }
    }
    return n;
}

/* closest point to arg0 on segment arg1-arg2 -> arg3; returns squared distance */
s32 func_800C20BC(SVECTOR *arg0, SVECTOR *arg1, SVECTOR *arg2, SVECTOR *arg3, s16 *arg4) {
    SVECTOR c;
    s32 ta;
    s32 tb;
    s32 s;
    s32 dx;
    s32 dy;
    s32 dz;
    s16 px;
    s16 py;
    s16 pz;
    s16 ax;
    s16 ay;
    s16 az;
    s16 bx;
    s16 by;
    s16 bz;

    px = arg0->vx;
    ax = arg1->vx;
    bx = arg2->vx;
    py = arg0->vy;
    ay = arg1->vy;
    by = arg2->vy;
    pz = arg0->vz;
    az = arg1->vz;
    bz = arg2->vz;
    ta = (px - ax) * (bx - ax);
    ta += (py - ay) * (by - ay);
    ta += (pz - az) * (bz - az);
    if (ta <= 0) {
        c.vx = arg1->vx;
        c.vy = arg1->vy;
        c.vz = arg1->vz;
        if (ta == 0) {
            *arg4 = 1;
        } else {
            *arg4 = 0;
        }
    } else {
        tb = (px - bx) * (ax - bx);
        tb += (py - by) * (ay - by);
        tb += (pz - bz) * (az - bz);
        if (tb <= 0) {
            c.vx = arg2->vx;
            c.vy = arg2->vy;
            c.vz = arg2->vz;
            if (tb == 0) {
                *arg4 = 1;
            } else {
                *arg4 = 0;
            }
        } else {
            while (ta > 0x10000 || tb > 0x10000) {
                ta >>= 1;
                tb = tb >> 1;
            }
            s = ta + tb;
            c.vx = arg1->vx + (arg2->vx - arg1->vx) * ta / s;
            c.vy = arg1->vy + (arg2->vy - arg1->vy) * ta / s;
            c.vz = arg1->vz + (arg2->vz - arg1->vz) * ta / s;
            *arg4 = 1;
        }
    }
    dx = c.vx - arg0->vx;
    dy = c.vy - arg0->vy;
    dz = c.vz - arg0->vz;
    arg3->vx = c.vx;
    arg3->vy = c.vy;
    s = dx * dx;
    s += dy * dy;
    arg3->vz = c.vz;
    return s + dz * dz;
}

s32 func_800C2390(VECTOR *arg0, SVECTOR *arg1, s32 arg2, VECTOR *arg3) {
    VECTOR a;
    VECTOR v;

    v.vx = arg1->vx << 16;
    v.vy = arg1->vy << 16;
    v.vz = arg1->vz << 16;
    a.vy = 0;
    a.vz = v.vx;
    a.vx = -v.vz;
    arg3->vx = -arg1->vz;
    arg3->vy = 0;
    arg3->vz = arg1->vx;
    if (arg0->vy >= 0) {
        v.vx = func_800C1DC4(arg0->vy, a.vz);
        v.vy = func_800C1DC4(arg0->vz, a.vx) - func_800C1DC4(arg0->vx, a.vz);
        v.vz = -func_800C1DC4(arg0->vy, a.vx);
    } else {
        v.vx = -func_800C1DC4(arg0->vy, a.vz);
        v.vy = func_800C1DC4(arg0->vx, a.vz) - func_800C1DC4(arg0->vz, a.vx);
        v.vz = func_800C1DC4(arg0->vy, a.vx);
    }
    v.vx = (v.vx + 0x8000) & 0xFFFF0000;
    v.vy = (v.vy + 0x8000) & 0xFFFF0000;
    v.vz = (v.vz + 0x8000) & 0xFFFF0000;
    arg1->vx = v.vx >> 16;
    arg1->vy = v.vy >> 16;
    arg1->vz = v.vz >> 16;
    return 1;
}

/* push a position out of the walkmesh triangle edges (recurses into neighbours) */
s32 func_800C2524(Unk800C9DF4 *arg0, SVECTOR *arg1, Unk800C9E00 *arg2, s16 *arg3) {
    Unk800C9DFC *e[2];
    SVECTOR p;
    Unk800C9DFC v[3];
    VECTOR push;
    s16 on;
    Unk800C9DF8Tri *t;
    Unk800C9E00 *nb;
    s16 i;
    s32 hit;
    s32 d;
    s16 n;
    Unk800C9DFC *vt;

    t = &D_800C9DF0.unk1C[arg2->unk4];
    p.vx = D_800C9DF0.unk8->unk8[0] + t->unkA[0];
    p.vy = D_800C9DF0.unk8->unk8[1] + t->unkA[1];
    p.vz = D_800C9DF0.unk8->unk8[2] + t->unkA[2];
    vt = D_800C9DF0.unkC;
    for (i = 0; i < 3; i++) {
        v[i].unk0 = p.vx + vt[arg2->unkC[i]].unk0;
        v[i].unk2 = p.vy + vt[arg2->unkC[i]].unk2;
        v[i].unk4 = p.vz + vt[arg2->unkC[i]].unk4;
    }
    e[0] = &v[2];
    hit = 0;
    for (i = 0; i < 3; i++) {
        e[1] = &v[i];
        d = func_800C20BC(arg1, (SVECTOR *)e[0], (SVECTOR *)e[1], &p, &on);
        if ((u32)arg0->unk20 >= d) {
            nb = func_800C1F98(arg0, arg2, i);
            if (nb != NULL) {
                if (!(nb->unk0 & 0x80)) {
                    nb->unk0 |= 0x80;
                    if (func_800C2524(arg0, arg1, nb, arg3) == 0) {
                        *arg3 = i;
                        hit = 1;
                    }
                }
            } else {
                do {
                    hit = 1;
                } while (0);
                if (d != 0) {
                    d = (s16)SquareRoot0(d);
                    if (p.vx == arg1->vx) {
                        push.vx = 0;
                    } else {
                        push.vx = (s16)((arg1->vx - p.vx) * arg0->unk1C / d);
                    }
                    if (p.vz == arg1->vz) {
                        push.vz = 0;
                    } else {
                        push.vz = (s16)((arg1->vz - p.vz) * arg0->unk1C / d);
                    }
                    n = D_800C9DDC;
                    if (n + 1 < 16) {
                        push.vx = p.vx - arg1->vx + push.vx;
                        push.vz = p.vz - arg1->vz + push.vz;
                        D_800CA9D8[n][0] = push.vx;
                        D_800C9DDC++;
                        D_800CA9D8[n][2] = push.vz;
                        D_800CAA38[n] = on;
                        D_800CAA58[n] = d;
                    }
                    hit = 1;
                    arg2->unk0 |= 0x80;
                    *arg3 = i;
                }
            }
        }
        e[0] = e[1];
    }
    return hit ^ 1;
}

u16 func_800C2920(Unk800C9DF4 *arg0, Unk800C9E00 **arg1, s32 arg2, s32 arg3) {
    Unk800C9E00 *p[2];
    u16 i;

    if (arg3 < 9) {
        p[0] = *arg1;
        if ((u8)func_800C4B30(D_800C9DF0.unk8, &D_800C9DF0.unk1C[p[0]->unk4], p[0], (s16 *)arg2)) {
            return 1;
        }
        for (i = 0; i < 3; i++) {
            p[1] = func_800C1F98(arg0, p[0], i);
            if (p[1] != NULL && (u8)func_800C4B30(D_800C9DF0.unk8, &D_800C9DF0.unk1C[p[1]->unk4], p[1], (s16 *)arg2)) {
                arg0->unk6 = p[0]->unk18[i];
                if (arg0->unk6 >= 0) {
                    p[1] = &D_800C9DF0.unk10[arg0->unk6];
                    arg0->unk4 = p[1]->unk4;
                    *arg1 = p[1];
                    return 1;
                }
            }
        }
        for (i = 0; i < 3; i++) {
            p[1] = func_800C1F98(arg0, p[0], i);
            if (p[1] != NULL && !(p[1]->unk0 & 0x80)) {
                p[1]->unk0 |= 0x80;
                if (func_800C2920(arg0, &p[1], arg2, arg3 + 1) == 1) {
                    *arg1 = p[1];
                    return 1;
                }
            }
        }
    }
    return 0;
}

/* slide a position along the walkmesh from arg2 towards arg3, following neighbour triangles */
s32 func_800C2AF0(Unk800C9DF4 *arg0, Unk800C9E00 **arg1, s16 *arg2, SVECTOR *arg3, s16 arg4, s16 arg5, s32 arg6, s32 arg7) {
    SVECTOR p;
    SVECTOR *pp;
    VECTOR n;
    SVECTOR save;
    SVECTOR d;
    SVECTOR e;
    VECTOR f;
    s16 edge;
    Unk800C9E00 *cur;
    s16 prev;
    u8 done;
    s16 saved;
    s32 off;
    s16 s;
    s16 l;
    s16 nb;
    Unk800C9E00 *q;
    u8 r;

    cur = *arg1;
    prev = -1;
    p.vx = arg2[0];
    p.vy = arg2[1];
    p.vz = arg2[2];
    save.vx = arg3->vx;
    save.vy = arg3->vy;
    done = 0;
    pp = &p;
    save.vz = arg3->vz;
    edge = -1;
    saved = arg0->unk6;
    off = arg0->unk6 * sizeof(Unk800C9E00);
    do {
        d.vx = arg3->vx - p.vx;
        d.vy = arg3->vy - pp->vy;
        d.vz = arg3->vz - pp->vz;
        if (cur->unk6 >= 0) {
            func_800C2390((VECTOR *)&D_800C9DF0.unk14[cur->unk6], &d, (s32)pp, &n);
        } else {
            func_800C2390(&D_800C9DE4, &d, (s32)pp, &n);
        }
        arg3->vx = pp->vx + d.vx;
        arg3->vy = pp->vy + d.vy;
        arg3->vz = pp->vz + d.vz;
        if ((u8)func_800C4B30(D_800C9DF0.unk8, &D_800C9DF0.unk1C[cur->unk4], cur, (s16 *)arg3)) {
            done = 1;
        } else if ((r = func_800C519C(D_800C9DF0.unk8, &D_800C9DF0.unk1C[cur->unk4], cur, arg3, (SVECTOR *)arg2, pp, &edge, edge))) {
            if (func_800C1F98(arg0, cur, edge) != NULL) {
                if (cur->unk6 >= 0) {
                    e.vx = arg3->vx - p.vx;
                    e.vy = arg3->vy - pp->vy;
                    e.vz = arg3->vz - pp->vz;
                    s = SquareRoot0(n.vx * n.vx + n.vz * n.vz);
                    if (s == 0) {
                        arg3->vy = pp->vy;
                    } else {
                        l = SquareRoot0(e.vx * e.vx + e.vy * e.vy + e.vz * e.vz);
                        f.vz = n.vz * l / s;
                        f.vx = n.vx * l / s;
                        f.vy = 0;
                        e.vy = 0;
                        e.vx = f.vz;
                        e.vz = -f.vx;
                        arg3->vx = p.vx + e.vx;
                        arg3->vy = pp->vy + e.vy;
                        arg3->vz = pp->vz + e.vz;
                    }
                }
                nb = cur->unk18[edge];
                if (nb == prev) {
                    arg0->unk6 = saved;
                    cur = (Unk800C9E00 *)((u8 *)D_800C9DF0.unk10 + off);
                    arg0->unk4 = cur->unk4;
                    *arg1 = cur;
                    return 1;
                }
                prev = arg0->unk6;
                arg0->unk6 = cur->unk18[edge];
                q = &D_800C9DF0.unk10[nb];
                {
                    Unk800C9E08 *ee = &D_800C9DF0.unk18[cur->unk12[edge]];
                    arg0->unk4 = q->unk4;
                    edge = ee->unk2;
                }
                cur = q;
                if (edge == -1) {
                    arg0->unk6 = saved;
                    cur = (Unk800C9E00 *)((u8 *)D_800C9DF0.unk10 + off);
                    arg0->unk4 = cur->unk4;
                    *arg1 = cur;
                    return 1;
                }
            } else {
                arg0->unk6 = saved;
                cur = (Unk800C9E00 *)((u8 *)D_800C9DF0.unk10 + off);
                arg0->unk4 = cur->unk4;
                *arg1 = cur;
                return 1;
            }
        } else {
            func_800C2AF0_clear();
            if ((u8)func_800C2920(arg0, &cur, (s32)arg3, 0)) {
                done = 1;
            } else {
                if (arg5 < 6) {
                    arg3->vx = save.vx;
                    arg3->vy = save.vy;
                    arg3->vz = save.vz;
                    func_800C2AF0_push(arg3, arg2, &arg6, &arg7);
                    arg0->unk24->unk14.vx = arg3->vx;
                    arg0->unk24->unk14.vy = arg3->vy;
                    arg0->unk24->unk14.vz = arg3->vz;
                    arg0->unk6 = saved;
                    cur = (Unk800C9E00 *)((u8 *)D_800C9DF0.unk10 + off);
                    arg0->unk4 = cur->unk4;
                    if (func_800C2AF0(arg0, &cur, arg2, arg3, arg4, arg5 + 1, arg6, arg7) == 0) {
                        break;
                    }
                }
                arg0->unk6 = saved;
                cur = (Unk800C9E00 *)((u8 *)D_800C9DF0.unk10 + off);
                arg0->unk4 = cur->unk4;
                *arg1 = cur;
                return 1;
            }
        }
    } while (!done);
    *arg1 = cur;
    return 0;
}

s32 func_800C31F8(Unk800C9DF4 *arg0, SVECTOR *arg1, Unk800C9E00 **arg2, s16 *arg3) {
    VECTOR sum;
    SVECTOR prev;
    u8 i;
    u8 j;
    u8 dup;
    u8 any;
    s32 n;

    if (D_800C9DDC == 0) {
        return 1;
    }
    prev.vx = arg1->vx;
    prev.vy = arg1->vy;
    prev.vz = arg1->vz;
    if (D_800C9DDC == 1) {
        arg1->vx += D_800CA9D8[0][0];
        arg1->vz += D_800CA9D8[0][2];
        arg0->unk24->unk14.vx = arg1->vx;
        arg0->unk24->unk14.vz = arg1->vz;
    } else {
        any = 0;
        i = 0;
        n = 0;
        sum.vx = 0;
        sum.vz = 0;
        for (; i < D_800C9DDC; i++) {
            if (D_800CAA38[i] != 0) {
                dup = 0;
                for (j = 0; j < i; j++) {
                    if (D_800CA9D8[j][0] == D_800CA9D8[i][0] && D_800CA9D8[j][2] == D_800CA9D8[i][2]) {
                        dup = 1;
                    }
                }
                if (!dup) {
                    any = 1;
                    sum.vx += D_800CA9D8[i][0];
                    n++;
                    sum.vz += D_800CA9D8[i][2];
                }
            }
        }
        if (any) {
            if (n >= 2) {
                sum.vx /= n;
                sum.vz /= n;
            }
        } else {
            n = 0;
            for (i = 0; i < D_800C9DDC; i++) {
                dup = 0;
                for (j = 0; j < i; j++) {
                    if (D_800CA9D8[j][0] == D_800CA9D8[i][0] && D_800CA9D8[j][2] == D_800CA9D8[i][2]) {
                        dup = 1;
                    }
                }
                if (!dup) {
                    sum.vx += D_800CA9D8[i][0];
                    n++;
                    sum.vz += D_800CA9D8[i][2];
                }
            }
            if (n > 0) {
                sum.vx /= n;
                sum.vy /= n;
            }
        }
        arg1->vx += sum.vx;
        arg1->vz += sum.vz;
        arg0->unk24->unk14.vx = arg1->vx;
        arg0->unk24->unk14.vz = arg1->vz;
    }
    D_800C9DDC = 0;
    return func_800C2AF0(arg0, arg2, &prev, arg1, 0, 0, 0, 0) == 0;
}

/* clears the visited flag of every walkmesh triangle first */
static inline s32 func_800C2524_inline(Unk800C9DF4 *e, SVECTOR *pos, Unk800C9E00 *nd, s16 *edge) {
    u16 k;
    Unk800C9E00 *n;
    s32 cnt;
    u16 m = ~0x80;

    n = D_800C9DF0.unk10;
    cnt = D_800C9DF0.unk8->unk24;
    for (k = 0; k < cnt; k++) {
        n->unk0 &= m;
        n++;
    }
    D_800C9DDC = 0;
    return func_800C2524(e, pos, nd, edge);
}

s32 func_800C3608(Unk800C9DF4 *arg0) {
    SVECTOR cur;
    SVECTOR prev;
    Unk800C9E00 *node;
    s16 edge;
    s32 ret;
    s32 id;
    s16 save;
    s32 k;
    Unk800C9DFC *vt;
    Unk800C9E04 *pl;
    s32 a;
    Unk800C9DF8Tri *t;

    ret = 2;
    D_800C9DDE = 0;
    for (k = 0; k < 10; k++) {
        D_800CAA98[k] = -1;
    }
    D_800C9DE0 = 0;
    for (k = 0; k < 10; k++) {
        D_800CAAB8[k] = -1;
    }
    cur.vx = arg0->unk24->unk14.vx;
    cur.vy = arg0->unk24->unk14.vy;
    cur.vz = arg0->unk24->unk14.vz;
    prev.vx = arg0->unk24->unk40.vx;
    prev.vy = arg0->unk24->unk40.vy;
    prev.vz = arg0->unk24->unk40.vz;
    if (cur.vx == prev.vx && cur.vy == prev.vy && cur.vz == prev.vz) {
        arg0->unk8 = arg0->unk4;
        arg0->unkA = arg0->unk6;
        return ret | 4;
    }
    id = arg0->unk6;
    node = &D_800C9DF0.unk10[id];
    save = arg0->unk6;
    if (func_800C2AF0(arg0, &node, (s16 *)&prev, &cur, 1, 0, 0, 0) != 0) {
        arg0->unk6 = save;
        if (id >= 0) {
            arg0->unk4 = D_800C9DF0.unk10[id].unk4;
        } else {
            arg0->unk4 = -1;
        }
        arg0->unk24->unk14.vx = arg0->unk24->unk40.vx;
        arg0->unk24->unk14.vy = arg0->unk24->unk40.vy;
        arg0->unk24->unk14.vz = arg0->unk24->unk40.vz;
        ret |= 1;
        if (arg0->unkA >= 0) {
            if (!(u8)func_800C4B30(D_800C9DF0.unk8, &D_800C9DF0.unk1C[arg0->unk8], &D_800C9DF0.unk10[arg0->unkA], &prev.vx)) {
                arg0->unk8 = arg0->unk4;
                arg0->unkA = arg0->unk6;
            }
        }
        return ret;
    }
    if (!(u8)func_800C2524_inline(arg0, &cur, node, &edge)) {
        if ((u8)func_800C31F8(arg0, &cur, &node, &edge)) {
            ret |= 9;
        } else {
            arg0->unk6 = save;
            if (save >= 0) {
                arg0->unk4 = D_800C9E00[save].unk4;
            } else {
                arg0->unk4 = -1;
            }
            arg0->unk24->unk14.vx = arg0->unk24->unk40.vx;
            arg0->unk24->unk14.vy = arg0->unk24->unk40.vy;
            arg0->unk24->unk14.vz = arg0->unk24->unk40.vz;
            ret |= 1;
            if (arg0->unkA >= 0 && arg0->unk8 >= 0) {
            if (!(u8)func_800C4B30(D_800C9DF0.unk8, &D_800C9DF0.unk1C[arg0->unk8], &D_800C9DF0.unk10[arg0->unkA], &prev.vx)) {
                arg0->unk8 = arg0->unk4;
                arg0->unkA = arg0->unk6;
            }
            }
            return ret;
        }
    }
    if (cur.vx != arg0->unk24->unk14.vx || cur.vz != arg0->unk24->unk14.vz) {
        arg0->unk24->unk14.vx = cur.vx;
        ret |= 0x10;
        arg0->unk24->unk14.vz = cur.vz;
    }
    if (node->unk6 >= 0) {
        pl = &D_800C9DF0.unk14[node->unk6];
        if ((u32)(pl->unk4 + 0x50F4) > 0xA1E8) {
            a = func_800C1DC4(pl->unk0, arg0->unk24->unk14.vx << 16);
            arg0->unk24->unk14.vy = func_800C1DC4(node->unk24 - a - func_800C1DC4(pl->unk8, arg0->unk24->unk14.vz << 16), pl->unkC) >> 16;
            arg0->unk24->unk14.vy += (s16)D_800C9DF0.unk1C[node->unk4].unkA[1] - D_800C9DF0.unk1C[node->unk4].unk4[1];
        } else {
            arg0->unk24->unk14.vy = cur.vy;
        }
    } else {
        t = D_800C9DF0.unk1C + node->unk4;
        vt = D_800C9DF0.unkC + node->unkC[0];
        arg0->unk24->unk14.vy = (s16)(vt->unk2 + (D_800C9DF0.unk8->unk8[1] + t->unkA[1]));
    }
    arg0->unkA = save;
    if (save >= 0) {
        arg0->unk8 = D_800C9E00[save].unk4;
    } else {
        arg0->unk8 = -1;
    }
    return ret;
}

s32 func_800C3B80(Unk800C9DF8 *arg0) {
    u16 i;
    Unk800C9E10 *p;

    D_800C9DF0.unk8 = arg0;
    D_800C9DF0.unkC = (Unk800C9DFC *)((u8 *)arg0 + arg0->unk3A);
    D_800C9DF0.unk10 = (Unk800C9E00 *)((u8 *)arg0 + arg0->unk26);
    D_800C9DF0.unk14 = (Unk800C9E04 *)((u8 *)arg0 + arg0->unk36);
    D_800C9DF0.unk18 = (u8 *)arg0 + arg0->unk2A;
    D_800C9DF0.unk1C = (Unk800C9DF8Tri *)((u8 *)arg0 + arg0->unk32);
    p = (Unk800C9E10 *)((u8 *)arg0 + arg0->unk2E);
    D_800C9DF0.unk1 = 0xFF;
    D_800C9DF0.unk2 = 9;
    D_800C9DF0.unk4 = D_800CAAD8;
    D_800C9DF0.unk20 = p;
    for (i = 0; i < 24; i++) {
        D_800C9DF0.unk4[i].unk0 = 0;
        D_800C9DF0.unk4[i].unk2 = 0;
        D_800C9DF0.unk4[i].unk4 = -1;
        D_800C9DF0.unk4[i].unk6 = -1;
        D_800C9DF0.unk4[i].unk8 = -1;
        D_800C9DF0.unk4[i].unkA = -1;
        D_800C9DF0.unk4[i].unk1C = 0x60;
        D_800C9DF0.unk4[i].unk20 = 0x2400;
        D_800C9DF0.unk4[i].unk24 = NULL;
    }
    D_800C9DF0.unk0 = 1;
    for (i = 0; i < D_800C9DF0.unk8->unk2C; i++) {
        func_800C79B0(i);
    }
    func_800C6E94();
    return 1;
}

Unk800C9DF4 *func_800C3D28(s32 *arg0) {
    Unk8004AB64 *a;
    s32 i;
    u8 mode;

    *arg0 = 0;
    mode = D_8006794C->unk8;
    if (mode == 5) {
        a = (Unk8004AB64 *)D_8006794C->unk1C->unk8E0[D_8006794C->unk1C->unk8E4.field->unk8].unkC;
    } else if (mode == 1) {
        a = func_8004AE00();
    } else {
        a = NULL;
    }
    if (a != NULL) {
        for (i = 0; i < D_800C9DF0.unk2; i++) {
            if (D_800C9DF0.unk4[i].unk24 == a) {
                *arg0 = i;
                return &D_800C9DF0.unk4[i];
            }
        }
    }
    return NULL;
}

s32 func_800C3E18(s32 arg0, s32 arg1) {
    if (arg1 != 0) {
        D_800C9DF0.unk4[arg0].unk0 |= 1;
    } else {
        D_800C9DF0.unk4[arg0].unk0 &= ~1;
        D_800C9DF0.unk4[arg0].unk4 = -1;
        D_800C9DF0.unk4[arg0].unk6 = -1;
        D_800C9DF0.unk4[arg0].unk8 = -1;
        D_800C9DF0.unk4[arg0].unkA = -1;
    }
    return 1;
}

/* put entry arg0 on the floor below (arg1, arg2, arg3) */
s32 func_800C3EC0(s32 arg0, s16 arg1, s16 arg2, s16 arg3) {
    SVECTOR pos;
    VECTOR v;
    s16 tmp;
    Unk800C9E00 *n;
    Unk800C9DF8 *b;
    Unk800C9DF4 *a;
    Unk800C9E04 *pl;
    s16 ymin;
    u16 found;
    s16 bi;
    s16 bt;
    u16 i;
    u16 hit;
    s32 t;
    s32 cnt;
    Unk800C9E00 *p;
    u16 j;
    u16 m;
    s32 k;
    Unk800C9DF8Tri *tri;
    Unk800C9DFC *vt;

    ymin = 0x7FFF;
    bi = -1;
    bt = -1;
    found = 0;
    i = 0;
    pos.vx = arg1;
    pos.vy = arg2;
    pos.vz = arg3;
    n = D_800C9DF0.unk10;
    b = D_800C9DF0.unk8;
    a = &D_800C9DF0.unk4[arg0];
    for (; i < b->unk24; i++, n++) {
        hit = 0;
        if ((u8)func_800C4EC8(b, &D_800C9DF0.unk1C[n->unk4], n, &pos)) {
            m = ~0x80;
            p = D_800C9DF0.unk10;
            cnt = D_800C9DF0.unk8->unk24;
            for (j = hit; j < cnt; j++) {
                p->unk0 &= m;
                p++;
            }
            D_800C9DDC = 0;
            if (func_800C2524(a, &pos, n, &tmp) || func_800C31F8(a, &pos, &n, &tmp)) {
                hit = 1;
            }
            if (hit) {
                k = n->unk6;
                if (k >= 0) {
                    v.vx = pos.vx;
                    v.vy = pos.vy;
                    v.vz = pos.vz;
                    pl = &D_800C9DF0.unk14[k];
                    t = func_800C1DC4(pl->unk0, v.vx << 16);
                    v.vy = func_800C1DC4(n->unk24 - t - func_800C1DC4(pl->unk8, v.vz << 16), pl->unkC) >> 16;
                    v.vy += (s16)D_800C9DF0.unk1C[n->unk4].unkA[1] - D_800C9DF0.unk1C[n->unk4].unk4[1];
                    pos.vy = v.vy;
                } else {
                    tri = D_800C9DF0.unk1C + n->unk4;
                    vt = D_800C9DF0.unkC + n->unkC[0];
                    pos.vy = vt->unk2 + (D_800C9DF0.unk8->unk8[1] + tri->unkA[1]);
                }
                if (pos.vy >= arg2 && pos.vy < ymin) {
                    found = 1;
                    ymin = pos.vy;
                    bi = i;
                    bt = n->unk4;
                }
            }
        }
    }
    if (found) {
        a->unk4 = bt;
        a->unk6 = bi;
        a->unk24->unk14.vx = arg1;
        a->unk24->unk14.vy = ymin;
        a->unk24->unk14.vz = arg3;
        return 1;
    }
    return 0;
}

s32 func_800C420C(s32 arg0, s16 arg1) {
    Unk800C9E00 *n;
    Unk800C9DF8Tri *t;
    SVECTOR v;

    n = &D_800C9DF0.unk10[(u16)arg1];
    t = &D_800C9DF0.unk1C[n->unk4];
    v.vx = D_800C9DF0.unk8->unk8[0] + t->unkA[0];
    v.vy = D_800C9DF0.unk8->unk8[1] + t->unkA[1];
    v.vz = D_800C9DF0.unk8->unk8[2] + t->unkA[2];
    D_800C9DF0.unk4[arg0].unk4 = n->unk4;
    D_800C9DF0.unk4[arg0].unk6 = arg1;
    D_800C9DF0.unk4[arg0].unk24->unk14.vx = v.vx + n->unk1E;
    D_800C9DF0.unk4[arg0].unk24->unk14.vy = v.vy + n->unk20;
    D_800C9DF0.unk4[arg0].unk24->unk14.vz = v.vz + n->unk22;
    D_800C9DF0.unk4[arg0].unk24->unk40.vx = v.vx + n->unk1E;
    D_800C9DF0.unk4[arg0].unk24->unk40.vy = v.vy + n->unk20;
    D_800C9DF0.unk4[arg0].unk24->unk40.vz = v.vz + n->unk22;
    return 1;
}

s32 func_800C4378(s32 arg0, u16 arg1, u16 arg2) {
    Unk800C9DF8Tri *t;
    Unk800C9E00 *n;
    s32 *ids;
    s32 id;
    SVECTOR v;

    t = &D_800C9DF0.unk1C[arg1];
    ids = (s32 *)((u8 *)D_800C9DF0.unk8 + t->unk1E);
    id = ids[arg2];
    v.vx = D_800C9DF0.unk8->unk8[0] + t->unkA[0];
    v.vy = D_800C9DF0.unk8->unk8[1] + t->unkA[1];
    v.vz = D_800C9DF0.unk8->unk8[2] + t->unkA[2];
    n = &D_800C9DF0.unk10[id];
    D_800C9DF0.unk4[arg0].unk4 = arg1;
    D_800C9DF0.unk4[arg0].unk6 = ids[arg2];
    D_800C9DF0.unk4[arg0].unk24->unk14.vx = v.vx + n->unk1E;
    D_800C9DF0.unk4[arg0].unk24->unk14.vy = v.vy + n->unk20;
    D_800C9DF0.unk4[arg0].unk24->unk14.vz = v.vz + n->unk22;
    D_800C9DF0.unk4[arg0].unk24->unk40.vx = v.vx + n->unk1E;
    D_800C9DF0.unk4[arg0].unk24->unk40.vy = v.vy + n->unk20;
    D_800C9DF0.unk4[arg0].unk24->unk40.vz = v.vz + n->unk22;
    return 1;
}

s32 func_800C44F4(u32 arg0) {
    Unk800C9DF4 *e;

    if (D_800C9DF0.unk0 == 0) {
        return 0x8000;
    }
    if (arg0 >= D_800C9DF0.unk2) {
        return 0x4000;
    }
    e = &D_800C9DF0.unk4[arg0];
    if (e->unk0 & 1) {
        if (e->unk4 == -1 || e->unk6 == -1) {
            return (func_800C3EC0(arg0, e->unk24->unk14.vx, e->unk24->unk14.vy, e->unk24->unk14.vz) != 0) * 2;
        }
        return func_800C3608(e);
    }
    return 0;
}

s16 func_800C45C0(s16 arg0, s16 arg1, s16 arg2, s16 *arg3, Unk800C9E00 **arg4) {
    SVECTOR p;
    VECTOR v;
    s16 y0;
    s32 best;
    s16 found;
    s16 i;
    Unk800C9E00 *n;
    Unk800C9DF8 *b;
    Unk800C9E04 *pl;
    s32 a;
    Unk800C9DF8Tri *t;
    s16 k;
    s16 y;
    Unk800C9DFC *vx;

    best = 0x7FFF;
    *arg3 = -1;
    *arg4 = NULL;
    n = D_800C9DF0.unk10;
    b = D_800C9DF0.unk8;
    found = 0;
    p.vx = arg0;
    p.vy = arg1;
    p.vz = arg2;
    i = 0;
    y0 = arg1;
    for (; i < b->unk24; n++, i++) {
        if ((u8)func_800C4EC8(b, &D_800C9DF0.unk1C[n->unk4], n, &p)) {
            k = n->unk6;
            if (k >= 0) {
                v.vx = p.vx;
                v.vy = p.vy;
                v.vz = p.vz;
                pl = &D_800C9DF0.unk14[k];
                a = func_800C1DC4(pl->unk0, v.vx << 16);
                v.vy = func_800C1DC4(n->unk24 - a - func_800C1DC4(pl->unk8, v.vz << 16), pl->unkC) >> 16;
                v.vy += (s16)D_800C9DF0.unk1C[n->unk4].unkA[1] - D_800C9DF0.unk1C[n->unk4].unk4[1];
                p.vy = v.vy;
            } else {
                t = &D_800C9DF0.unk1C[n->unk4];
                vx = &D_800C9DF0.unkC[n->unkC[0]];
                p.vy = vx->unk2 + (D_800C9DF0.unk8->unk8[1] + t->unkA[1]);
            }
            y = p.vy;
            if (y >= y0 && y < best) {
                found = 1;
                *arg3 = i;
                best = y;
                *arg4 = n;
            }
        }
    }
    return found;
}

s32 func_800C47E4(s32 arg0, s32 arg1) {
    Unk800C9DF8 *b = D_800C9DF0.unk8;
    Unk800C9DF8Tri *t = &((Unk800C9DF8Tri *)((u8 *)b + b->unk32))[arg0];
    s32 *ids;
    s32 i;
    Unk800C9E00 *n;

    if (arg1 != 0) {
        ids = (s32 *)((u8 *)b + t->unk1E);
        t->unk0 |= 1;
        n = D_800C9DF0.unk10;
        for (i = 0; i < t->unk1C; i++) {
            n[ids[i]].unk0 |= 1;
        }
    } else {
        ids = (s32 *)((u8 *)b + t->unk1E);
        t->unk0 &= ~1;
        n = D_800C9DF0.unk10;
        for (i = 0; i < t->unk1C; i++) {
            n[ids[i]].unk0 &= ~1;
        }
    }
    return 1;
}

s16 func_800C48DC(s16 arg0, s16 arg1) {
    SVECTOR p;
    VECTOR v;
    u16 i;
    Unk800C9E00 *n;
    Unk800C9E04 *pl;
    s32 a;
    s16 k;
    u8 hit;
    GameData *g;

    if (D_800C9DF0.unk8 == NULL) {
        if (D_8006794C->unk8 == 5) {
            g = D_8006794C->unk1C;
            return ((Unk8004AB64 *)g->unk8E0[g->unk8E4.field->unk8].unkC)->unk14.vy;
        }
        if (D_8006794C->unk8 == 1) {
            return func_8004AE00()->unk14.vy;
        }
        return 0;
    }
    goto start;
plane:
    v.vx = p.vx;
    v.vy = p.vy;
    v.vz = p.vz;
    pl = &D_800C9DF0.unk14[k];
    a = func_800C1DC4(pl->unk0, v.vx << 16);
    v.vy = func_800C1DC4(n->unk24 - a - func_800C1DC4(pl->unk8, v.vz << 16), pl->unkC) >> 16;
    v.vy += (s16)D_800C9DF0.unk1C[n->unk4].unkA[1] - D_800C9DF0.unk1C[n->unk4].unk4[1];
    p.vy = v.vy;
    return p.vy;
start:
    p.vx = arg0;
    p.vy = -0x5CD7;
    p.vz = arg1;
    n = D_800C9DF0.unk10;
    for (i = 0; i < D_800C9DF0.unk8->unk24; i++, n++) {
        if ((u8)func_800C4EC8(D_800C9DF0.unk8, &D_800C9DF0.unk1C[n->unk4], n, &p)) {
            k = n->unk6;
            if (k >= 0) {
                goto plane;
            }
            {
                p.vy = D_800C9DF0.unk8->unk8[1];
                p.vy += D_800C9DF0.unk1C[n->unk4].unkA[1];
                p.vy += D_800C9DF0.unkC[n->unkC[0]].unk2;
                return p.vy;
            }
        }
    }
    return -0x7FFF;
}

s32 func_800C4B30(Unk800C9DF8 *b, Unk800C9DF8Tri *t, Unk800C9E00 *n, s16 *p) {
    s8 c[2];
    s32 d[2];
    Unk800C9DFCv *pv[2];
    s16 o[3];
    Unk800C9DFCv v[3];
    u8 i;
    s32 r;
    u8 x;
    u8 z;
    Unk800C9DFCv *vtx;
    Unk800C9E04 *pl;
    s32 a;
    s32 cz;

    x = 0;
    z = 2;
    if (n->unk6 >= 0) {
        pl = &D_800C9DF0.unk14[n->unk6];
        if ((u32)(pl->unk4 + 0x50F4) <= 0xA1E8) {
            a = pl->unk0;
            cz = pl->unk8;
            if (ABS(a) > ABS(cz)) {
                x = 1;
                z = 2;
            } else {
                x = 0;
                z = 1;
            }
        }
    }
    r = 0;
    i = 0;
    vtx = (Unk800C9DFCv *)D_800C9DF0.unkC;
    o[x] = b->unk8[x] + t->unkA[x];
    o[z] = b->unk8[z] + t->unkA[z];
    c[0] = 0;
    c[1] = 0;
    for (; i < 3; i++) {
        v[i].unk0[x] = o[x] + vtx[n->unkC[i]].unk0[x];
        v[i].unk0[z] = o[z] + vtx[n->unkC[i]].unk0[z];
        if (v[i].unk0[x] == p[x]) {
            c[0]++;
            if (v[i].unk0[z] <= p[z]) {
                c[0] = -c[0];
            }
        }
        if (v[i].unk0[z] == p[z]) {
            c[1]++;
            if (v[i].unk0[x] <= p[x]) {
                c[1] = -c[1];
            }
        }
    }
    if (c[0] == -2) {
        return 1;
    }
    if (c[1] == -2) {
        return 1;
    }
    pv[0] = &v[2];
    c[0] = p[z] >= pv[0]->unk0[z];
    for (i = 0; i < 3; i++) {
        pv[1] = &v[i];
        c[1] = p[z] >= pv[1]->unk0[z];
        if (c[0] != c[1]) {
            d[0] = (pv[1]->unk0[z] - p[z]) * (pv[0]->unk0[x] - pv[1]->unk0[x]);
            d[1] = (pv[1]->unk0[x] - p[x]) * (pv[0]->unk0[z] - pv[1]->unk0[z]);
            if (d[0] == d[1]) {
                return 1;
            }
            if ((d[1] < d[0]) == c[1]) {
                r ^= 1;
            }
        }
        pv[0] = pv[1];
        c[0] = c[1];
    }
    return r;
}

s32 func_800C4EC8(Unk800C9DF8 *b, Unk800C9DF8Tri *t, Unk800C9E00 *n, SVECTOR *sv) {
    s16 *p = (s16 *)sv;
    s8 c[2];
    s32 d[2];
    Unk800C9DFCv *pv[2];
    s16 o[3];
    Unk800C9DFCv v[3];
    u8 i;
    u8 r;
    u8 x;
    u8 z;
    Unk800C9DFCv *vtx;

    x = 0;
    z = 2;
    r = 0;
    i = 0;
    vtx = (Unk800C9DFCv *)D_800C9DF0.unkC;
    o[x] = b->unk8[x] + t->unkA[x];
    o[z] = b->unk8[z] + t->unkA[z];
    c[0] = 0;
    c[1] = 0;
    for (; i < 3; i++) {
        v[i].unk0[x] = o[x] + vtx[n->unkC[i]].unk0[x];
        v[i].unk0[z] = o[z] + vtx[n->unkC[i]].unk0[z];
        if (v[i].unk0[x] == p[x]) {
            c[0]++;
            if (v[i].unk0[z] <= p[z]) {
                c[0] = -c[0];
            }
        }
        if (v[i].unk0[z] == p[z]) {
            c[1]++;
            if (v[i].unk0[x] <= p[x]) {
                c[1] = -c[1];
            }
        }
    }
    if (c[0] == -2) {
        return 1;
    }
    if (c[1] == -2) {
        return 1;
    }
    pv[0] = &v[2];
    c[0] = p[z] >= pv[0]->unk0[z];
    for (i = 0; i < 3; i++) {
        pv[1] = &v[i];
        c[1] = p[z] >= pv[1]->unk0[z];
        if (c[0] != c[1]) {
            d[0] = (pv[1]->unk0[z] - p[z]) * (pv[0]->unk0[x] - pv[1]->unk0[x]);
            d[1] = (pv[1]->unk0[x] - p[x]) * (pv[0]->unk0[z] - pv[1]->unk0[z]);
            if (d[0] == d[1]) {
                return 1;
            }
            if ((d[1] < d[0]) == c[1]) {
                r = !r;
            }
        }
        pv[0] = pv[1];
        c[0] = c[1];
    }
    return r;
}

INCLUDE_ASM("asm/us/ovl_0a/nonmatchings/ovl_0a", func_800C519C);

INCLUDE_RODATA("asm/us/ovl_0a/nonmatchings/ovl_0a", D_800A717C);

/* debug: print a walkmesh */
void func_800C5A7C(Unk800C9DF8 *arg0) {
    Unk800C9E00 *tri;
    Unk800C9DF8Link *edge;
    Unk800C9E10 *anim;
    Unk800C9E10Frame *frame;
    Unk800C9DF8Tri *floor;
    Unk800C9DFC *vtx;
    Unk800C9E04 *nrm;
    s32 *ids;
    s32 i;
    s32 j;
    s32 k;

    if (arg0 == NULL) {
        return;
    }
    func_80015C38("BGI Text Dump\n");
    func_80015C38("=============\n");
    func_80015C38("Data Size\t\t : %d [+4 for file magic]\n", arg0->unk0);
    func_80015C38("Original Position: (%6d,%6d,%6d)\n", arg0->unk2, arg0->unk4, arg0->unk6);
    func_80015C38("Current Position : (%6d,%6d,%6d)\n", arg0->unk8[0], arg0->unk8[1], arg0->unk8[2]);
    func_80015C38("Minimum Values   : (%6d,%6d,%6d)\n", arg0->unkE, arg0->unk10, arg0->unk12);
    func_80015C38("Maximum Values   : (%6d,%6d,%6d)\n", arg0->unk14, arg0->unk16, arg0->unk18);
    func_80015C38("Default Char Pos : (%6d,%6d,%6d)\n", arg0->unk1A, arg0->unk1C, arg0->unk1E);
    func_80015C38("Active Floor\t : %d\n", arg0->unk20);
    func_80015C38("Active Triangle  : %d\n", arg0->unk22);
    func_80015C38("Triangle Count   : %d\n", arg0->unk24);
    func_80015C38("Triangle Offset  : %d\n", arg0->unk26);
    tri = (Unk800C9E00 *)((u8 *)arg0 + arg0->unk26);
    for (i = 0; i < arg0->unk24; tri++, i++) {
        func_80015C38("  %4d: Flags\t\t\t= 0x%04X\n", i, tri->unk0);
        func_80015C38("\t\tData\t\t\t = %d\n", tri->unk2);
        func_80015C38("\t\tFloor Index \t = %d\n", tri->unk4);
        func_80015C38("\t\tNormal Index\t = %d\n", tri->unk6);
        func_80015C38("\t\tTheta X \t\t = %d\n", (s16)tri->unk8);
        func_80015C38("\t\tTheta Z \t\t = %d\n", tri->unkA);
        func_80015C38("\t\tD\t\t\t\t = %ld\n", tri->unk24);
        func_80015C38("\t\tCenter  \t\t = (%6d,%6d,%6d)\n", tri->unk1E, tri->unk20, tri->unk22);
        func_80015C38("\t\tVertex Indices   = (%6d,%6d,%6d)\n", tri->unkC[0], tri->unkC[1], tri->unkC[2]);
        func_80015C38("\t\tEdge Indices\t = (%6d,%6d,%6d)\n", tri->unk12[0], tri->unk12[1], tri->unk12[2]);
        func_80015C38("\t\tNeighbor Indices = (%6d,%6d,%6d)\n", tri->unk18[0], tri->unk18[1], tri->unk18[2]);
    }
    func_80015C38("Edge Count  \t : %d\n", arg0->unk28);
    func_80015C38("Edge Offset \t : %d\n", arg0->unk2A);
    edge = (Unk800C9DF8Link *)((u8 *)arg0 + arg0->unk2A);
    for (i = 0; i < arg0->unk28; edge++, i++) {
        func_80015C38("  %4d: Flags\t   = 0x%04X\n", i, edge->unk0);
        func_80015C38("\t\tEdge Clone  = %d\n", edge->unk2);
    }
    func_80015C38("Keyframe Animation Count  : %d\n", arg0->unk2C);
    func_80015C38("Keyframe Animation Offset : %d\n", arg0->unk2E);
    anim = (Unk800C9E10 *)((u8 *)arg0 + arg0->unk2E);
    for (i = 0; i < arg0->unk2C; anim++, i++) {
        func_80015C38("  %4d: Flags\t\t  = 0x%04X\n", i, anim->unk0);
        func_80015C38("\t\tFrame Count    = %d\n", anim->unk2);
        func_80015C38("\t\tCurrent Frame  = %ld\n", anim->unk8);
        func_80015C38("\t\tFrame Rate     = %d\n", anim->unk4);
        func_80015C38("\t\tCounter \t   = %d\n", (u16)anim->unk6);
        func_80015C38("\t\tFrame Offset   = %ld\n", anim->unkC);
        frame = (Unk800C9E10Frame *)((u8 *)arg0 + anim->unkC);
        for (j = 0; j < anim->unk2; frame++, j++) {
            func_80015C38("\t\t%4d: Frame Flags     = 0x%04X\n", j, frame->unk0);
            func_80015C38("              Value           = %d\n", frame->unk2);
            func_80015C38("              Triangle Count  = %d\n", frame->unk4);
            func_80015C38("\t\t\t  Triangle Offset = %d\n", frame->unk6);
            ids = (s32 *)((u8 *)arg0 + frame->unk6);
            for (k = 0; k < frame->unk4; ids++, k++) {
                func_80015C38("\t\t\t\t%4d: Triangle Ndx = %ld\n", j, *ids);
            }
        }
    }
    func_80015C38("Floor Count \t : %d\n", arg0->unk30);
    func_80015C38("Floor Offset\t : %d\n", arg0->unk32);
    floor = (Unk800C9DF8Tri *)((u8 *)arg0 + arg0->unk32);
    for (i = 0; i < arg0->unk30; floor++, i++) {
        func_80015C38("  %4d: Flags\t\t\t\t= %d\n", i, floor->unk0);
        func_80015C38("\t\tFloor Index \t\t  = %d\n", floor->unk2);
        func_80015C38("\t\tOriginal Position\t  = (%6d,%6d,%6d)\n", floor->unk4[0], floor->unk4[1], floor->unk4[2]);
        func_80015C38("\t\tCurrent Position\t  = (%6d,%6d,%6d)\n", (s16)floor->unkA[0], (s16)floor->unkA[1], (s16)floor->unkA[2]);
        func_80015C38("\t\tMinumim Values  \t  = (%6d,%6d,%6d)\n", floor->unk10[0], floor->unk10[1], floor->unk10[2]);
        func_80015C38("\t\tMaximum Values  \t  = (%6d,%6d,%6d)\n", floor->unk16[0], floor->unk16[1], floor->unk16[2]);
        func_80015C38("\t\tTriangle Count  \t  = %d\n", floor->unk1C);
        func_80015C38("\t\tTriangle Index Offset = %d\n", floor->unk1E);
        ids = (s32 *)((u8 *)arg0 + floor->unk1E);
        for (j = 0; j < floor->unk1C; ids++, j++) {
            func_80015C38("\t\t  %4d: Triangle Ndx = %ld\n", j, *ids);
        }
    }
    func_80015C38("Vertex Count\t : %d\n", arg0->unk38);
    func_80015C38("Vertex Offset\t : %d\n", arg0->unk3A);
    vtx = (Unk800C9DFC *)((u8 *)arg0 + arg0->unk3A);
    for (i = 0; i < arg0->unk38; vtx++, i++) {
        func_80015C38("  %4d: (%d,%d,%d)\n", i, vtx->unk0, vtx->unk2, vtx->unk4);
    }
    func_80015C38("Normal Count\t : %d\n", arg0->unk34);
    func_80015C38("Normal Offset\t : %d\n", arg0->unk36);
    nrm = (Unk800C9E04 *)((u8 *)arg0 + arg0->unk36);
    for (i = 0; i < arg0->unk34; nrm++, i++) {
        func_80015C38("  %4d: (%lx,%lx,%lx)\n", i, nrm->unk0, nrm->unk4, nrm->unk8);
        func_80015C38(D_800A7814, nrm->unkC);
    }
}

s32 func_800C60F0(void) {
    Unk800C9DF8 *b;
    Unk800C9E10 *e;
    u16 cnt;
    Unk800C9E00 *nodes;
    u16 i;
    s16 old;
    Unk800C9DF8Pt *pts;
    Unk800C9DF8Pt *pt;
    u16 j;
    u16 k;
    s32 *ids;
    Unk800C9E00 *n;
    s32 pos;

    s32 v;
    u16 f;
    u16 num;
    u16 m;

    b = D_800C9DF0.unk8;
    e = D_800C9DF0.unk20;
    cnt = b->unk2C;
    nodes = D_800C9DF0.unk10;
    for (i = 0; i < cnt; e++, i++) {
        if (!(e->unk0 & 1)) {
            continue;
        }
        if (!(e->unk0 & 0xC)) {
            continue;
        }
        old = -1;
        pts = (Unk800C9DF8Pt *)((u8 *)b + e->unkC);
        num = e->unk2;
        for (j = 0; j < num; j++) {
            ids = (s32 *)((u8 *)b + pts[j].unk6);
            m = pts[j].unk4;
            for (k = 0; k < m; k++, ids++) {
                f = nodes[*ids].unk0;
                if (f & 1) {
                    old = j;
                }
                nodes[*ids].unk0 = f & ~1;
            }
        }
        pos = e->unk8;
        j = pos >> 8;
        ids = (s32 *)((u8 *)b + pts[j].unk6);
        m = pts[j].unk4;
        for (k = 0; k < m; k++) {
            nodes[*ids++].unk0 |= 1;
        }
        if (old != j) {
            pt = &pts[j];
            func_800C6C04(b, e, pt, old, j);
            func_800C6DA4(b, e, pt);
        }
        if (pts[j].unk2 < 0) {
            pts[j].unk2 = 0;
            e->unk6 = 0;
            e->unk0 &= ~4;
            return 1;
        }
        e->unk6++;
        if ((u16)e->unk6 >= pts[j].unk2) {
            e->unk6 = 0;
            e->unk8 = pos + e->unk4;
            v = e->unk8 >> 8;
            if (v >= e->unk2) {
                if (e->unk0 & 0x10) {
                    e->unk8 = ((e->unk2 - 1) << 8) | 0xFF;
                    func_800C7BC4(i, (s16)-e->unk4);
                } else {
                    e->unk8 = 0;
                }
                e->unk0 &= ~4;
            } else if (v < 0) {
                if (e->unk0 & 0x10) {
                    e->unk8 = 0;
                    func_800C7BC4(i, (s16)-e->unk4);
                } else {
                    e->unk8 = ((e->unk2 - 1) << 8) | 0xFF;
                }
                e->unk0 &= ~4;
            }
        }
    }
    return 1;
}

s32 func_800C6408(u32 arg0, u32 arg1) {
    Unk800C9DF8 *b;
    Unk800C9E10 *e;
    Unk800C9DF8Pt *pt;
    s32 *ids;
    s32 i;
    s32 j;
    s16 old;
    Unk800C9E00 *n;
    u16 f;

    b = D_800C9DF0.unk8;
    if (arg0 >= b->unk2C) {
        return 0;
    }
    e = &D_800C9DF0.unk20[arg0];
    old = -1;
    if (arg1 >= e->unk2) {
        return 0;
    }
    pt = (Unk800C9DF8Pt *)((u8 *)b + e->unkC);
    for (i = 0; i < e->unk2; pt++, i++) {
        ids = (s32 *)((u8 *)b + pt->unk6);
        for (j = 0; j < pt->unk4; ids++, j++) {
            n = &D_800C9DF0.unk10[*ids];
            if (n->unk0 & 1) {
                old = i;
                n->unk0 &= ~1;
            } else {
                n->unk0 &= ~1;
            }
        }
    }
    pt = (Unk800C9DF8Pt *)((u8 *)b + e->unkC);
    pt += arg1;
    ids = (s32 *)((u8 *)b + pt->unk6);
    for (i = 0; i < pt->unk4; ids++, i++) {
        n = &D_800C9DF0.unk10[*ids];
        n->unk0 |= 1;
    }
    if (arg1 != old) {
        func_800C6C04(b, e, pt, old, arg1 & 0xFFFF);
        func_800C6DA4(b, e, pt);
    }
    e->unk8 = arg1 << 8;
    return 1;
}

INCLUDE_ASM("asm/us/ovl_0a/nonmatchings/ovl_0a", func_800C65CC);

s32 func_800C6C04(u8 *arg0, Unk800C6C04A *arg1, Unk800C6C04B *arg2, s16 arg3) {
    Unk800C6C04E *e;
    s32 *tbl;
    s32 *ids;
    Unk800C9DF4 *a;
    u16 i;
    u16 j;

    if (arg3 == -1) {
        return 0;
    }
    e = (Unk800C6C04E *)(arg0 + arg1->unkC);
    e += arg3;
    tbl = (s32 *)(arg0 + arg2->unk6);
    a = D_800C9DF0.unk4;
    for (i = 0; i < D_800C9DF0.unk2; a++, i++) {
        if (a->unk0 & 1) {
            ids = (s32 *)(arg0 + e->unk6);
            for (j = 0; j < e->unk4; j++, ids++) {
                if (a->unk6 == *ids) {
                    func_800C65CC(arg0, a, &D_800C9DF0.unk10[a->unk6], &D_800C9DF0.unk10[tbl[j]]);
                    a->unk6 = tbl[j];
                    a->unk4 = D_800C9DF0.unk10[tbl[j]].unk4;
                }
            }
        }
    }
    return 1;
}

/* link the neighbour edges between the nodes of different triangles */
s32 func_800C6DA4(Unk800C9DF8 *arg0, Unk800C9E10 *arg1, Unk800C9DF8Pt *arg2) {
    s32 *ids = (s32 *)((u8 *)arg0 + arg2->unk6);
    Unk800C9E00 *n;
    Unk800C9E00 *m;
    Unk800C9E08 *e;
    s16 nb;
    u16 i;
    u16 j;
    u16 k;

    for (i = 0; i < arg2->unk4; i++, ids++) {
        n = &D_800C9DF0.unk10[*ids];
        for (j = 0; j < 3; j++) {
            nb = n->unk18[j];
            if (nb >= 0) {
                m = &D_800C9DF0.unk10[nb];
                if (n->unk4 != m->unk4) {
                    e = &D_800C9DF0.unk18[n->unk12[j]];
                    if (e->unk2 >= 0) {
                        k = e->unk2;
                        m->unk18[k] = *ids;
                        e = &D_800C9DF0.unk18[m->unk12[k]];
                        e->unk2 = j;
                    }
                }
            }
        }
    }
    return 1;
}

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

/* old-style definition: func_800C7F34/func_800C7FBC call it without arguments */
s32 func_800C6EEC(arg0)
s32 arg0;
{
    Unk800CAE98 *e;
    s32 step;
    s16 range;
    s16 v;

    e = &D_800CAE98[arg0];
    e->unk14 += e->unk12;
    step = e->unk14 >> 8;
    range = e->unkA - e->unk8;
    if (e->unk10 != 0 && e->unk12 != 0 && range > 0) {
        switch (e->unk6) {
        case 0:
            v = e->unk8 + step;
            break;
        case 1:
            v = e->unk8 + range * ((s16)func_8004BB7C((s16)((step << 12) / range) + 0x800) + 0x1000) / 0x2000;
            break;
        default:
            v = e->unk8 + step;
            break;
        }
        e->unkE = e->unkC;
        e->unkC = v;
        if (e->unkA < v) {
            if (e->unk0 & 0x10) {
                e->unkC = e->unkA;
                e->unk12 = -e->unk12;
            } else {
                e->unkC = e->unk8;
            }
            e->unk14 = (range << 8) - 0x100;
            e->unk0 &= ~4;
        } else if (v < e->unk8) {
            if (e->unk0 & 0x10) {
                e->unkC = e->unk8;
                e->unk12 = -e->unk12;
            } else {
                e->unkC = e->unkA;
            }
            e->unk14 = 0;
            e->unk0 &= ~4;
        }
    } else {
        return 0;
    }
    return 1;
}

s32 func_800C7100(void) {
    Unk800CAE98 *e;
    Unk800C9DF4 *a;
    Unk800C9DF8Tri *t;
    s32 i;
    s32 j;
    Unk8004AB64 *p;
    u16 flags;

    e = D_800CAE98;
    for (i = 0; i < 30; e++, i++) {
        if (!(e->unk0 & 1) || !(e->unk0 & 0xC)) {
            continue;
        }
        t = &D_800C9DF0.unk1C[e->unk2];
        t->unkA[e->unk4] = t->unk4[e->unk4] + e->unkC;
        a = D_800C9DF0.unk4;
        for (j = 0; j < D_800C9DF0.unk2; a++, j++) {
            flags = a->unk0;
            if ((flags & 1) && a->unk4 == e->unk2 && a->unk6 >= 0) {
                p = a->unk24;
                ((ActorPosView *)p)->unk14[e->unk4] += e->unkC - e->unkE;
                switch (e->unk4) {
                case 0:
                    a->unk24->unk40.vx = a->unk24->unk14.vx;
                    break;
                case 1:
                    a->unk24->unk40.vy = a->unk24->unk14.vy;
                    break;
                case 2:
                    a->unk24->unk40.vz = a->unk24->unk14.vz;
                    break;
                }
            }
        }
        func_800C6EEC(i);
    }
    return 1;
}

s32 func_800C72D8(Unk800C9DF8 *arg0) {
    s32 i;

    D_800C9DF0.unk8 = arg0;
    D_800C9DF0.unkC = (Unk800C9DFC *)((u8 *)arg0 + arg0->unk3A);
    D_800C9DF0.unk10 = (Unk800C9E00 *)((u8 *)arg0 + arg0->unk26);
    D_800C9DF0.unk14 = (Unk800C9E04 *)((u8 *)arg0 + arg0->unk36);
    D_800C9DF0.unk18 = (u8 *)arg0 + arg0->unk2A;
    D_800C9DF0.unk1C = (Unk800C9DF8Tri *)((u8 *)arg0 + arg0->unk32);
    D_800C9DF0.unk20 = (Unk800C9E10 *)((u8 *)arg0 + arg0->unk2E);
    for (i = 0; i < D_800C9DF0.unk8->unk2C; i++) {
        func_800C79B0(i);
    }
    func_800C6E94();
    return 1;
}

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

s32 func_800C74A8(s32 arg0, FieldObjSlot *arg1) {
    D_800C9DF0.unk4[arg0].unk0 = 0;
    D_800C9DF0.unk4[arg0].unk2 = 0;
    D_800C9DF0.unk4[arg0].unk24 = arg1->unkC;
    return 1;
}

s32 func_800C74F0(FieldObjSlot *arg0) {
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

s32 func_800C760C(s32 arg0, s32 *arg1, s32 *arg2, s32 *arg3, s32 *arg4) {
    Unk800C9E04 *e;
    Unk800C9E00 *n;
    struct Unk800C9DF4 *a;

    a = D_800C9DF0.unk4 + arg0;
    n = D_800C9DF0.unk10 + a->unk6;
    e = D_800C9DF0.unk14 + n->unk6;
    *arg1 = e->unk0;
    *arg2 = e->unk4;
    *arg3 = e->unk8;
    *arg4 = e->unkC;
    return 1;
}

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

s32 func_800C778C(s32 arg0, s32 arg1) {
    Unk800C9DF8Node *n = func_800C9DF8_node(arg0);
    u16 v;

    if (arg1 != 0) {
        v = n->unk0 | 1;
    } else {
        v = n->unk0 & ~1;
    }
    n->unk0 = v;
    return 1;
}

s32 func_800C77DC(u16 *arg0, s16 **arg1) {
    *arg0 = D_800C9DDE;
    *arg1 = D_800CAA98;
    return 1;
}

s32 func_800C7800(s16 arg0, u16 *arg1) {
    *arg1 = func_800C9DF8_node(arg0)->unk0 & ((D_800C9DF0.unk1 << 8) | 0xFF);
    return 1;
}

s32 func_800C784C(s16 arg0, s32 arg1) {
    Unk800C9DF8Node *n = func_800C9DF8_node(arg0);

    n->unk0 = ((arg1 & 0xFF) << 8) | (n->unk0 & 0xFF);
    return 1;
}

s32 func_800C7890(s16 arg0, u8 *arg1) {
    *arg1 = (func_800C9DF8_node(arg0)->unk0 >> 8) & D_800C9DF0.unk1;
    return 1;
}

s32 func_800C78D8(s16 arg0, u16 *arg1) {
    *arg1 = func_800C9DF8_node(arg0)->unk4;
    return 1;
}

s32 func_800C7914(u16 *arg0, s16 **arg1) {
    *arg0 = D_800C9DE0;
    *arg1 = D_800CAAB8;
    return 1;
}

s32 func_800C7938(s16 arg0, u16 *arg1) {
    *arg1 = func_800C9DF8_link(arg0)->unk0;
    return 1;
}

s32 func_800C7968(s32 arg0, u16 *arg1, u16 *arg2, u16 *arg3) {
    Unk800C9DF8Tri *t = func_800C9DF8_tri(arg0);

    *arg1 = t->unkA[0];
    *arg2 = t->unkA[1];
    *arg3 = t->unkA[2];
    return 1;
}

s32 func_800C79B0(u32 arg0) {
    Unk800C9DF8 *b;
    Unk800C9E10 *e;
    Unk800C9DF8Pt *pt;
    s32 *ids;
    s32 i;
    s32 j;
    Unk800C9E00 *n;

    b = D_800C9DF0.unk8;
    if (arg0 >= b->unk2C) {
        return 0;
    }
    e = &D_800C9DF0.unk20[arg0];
    pt = (Unk800C9DF8Pt *)((u8 *)b + e->unkC);
    for (i = 0; i < e->unk2; pt++, i++) {
        ids = (s32 *)((u8 *)b + pt->unk6);
        for (j = 0; j < pt->unk4; j++) {
            n = &D_800C9DF0.unk10[*ids];
            if (i == 0) {
                n->unk0 |= 1;
                func_800C6DA4(b, e, pt);
            } else {
                n->unk0 &= ~1;
            }
            ids++;
        }
    }
    return 1;
}

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

s32 func_800C7BC4(u32 arg0, s16 arg1) {
    Unk800C9E10 *e;

    if (arg0 >= D_800C9DF0.unk8->unk2C) {
        return 0;
    }
    e = &D_800C9DF0.unk20[arg0];
    e->unk4 = arg1;
    return 1;
}

s32 func_800C7C04(u32 arg0, s16 arg1) {
    Unk800C9DF8 *b = D_800C9DF0.unk8;
    Unk800C9E10 *e;
    Unk800C9DF8Pt *pt;
    s32 i;

    if (arg0 >= b->unk2C) {
        return 0;
    }
    e = &D_800C9DF0.unk20[arg0];
    pt = (Unk800C9DF8Pt *)((u8 *)b + e->unkC);
    for (i = 0; i < e->unk2; pt++, i++) {
        pt->unk2 = arg1;
    }
    return 1;
}

s32 func_800C7C70(u32 arg0, u32 arg1, s16 arg2) {
    Unk800C9DF8 *b = D_800C9DF0.unk8;
    Unk800C9E10 *e;
    Unk800C9DF8Pt *pt;

    if (arg0 >= b->unk2C) {
        return 0;
    }
    e = &D_800C9DF0.unk20[arg0];
    if (arg1 >= e->unk2) {
        return 0;
    }
    pt = &((Unk800C9DF8Pt *)((u8 *)b + e->unkC))[arg1];
    pt->unk2 = arg2;
    return 1;
}

s32 func_800C7CDC(u32 arg0, s32 arg1) {
    Unk800C9E10 *e;

    if (arg0 >= D_800C9DF0.unk8->unk2C) {
        return 0;
    }
    e = &D_800C9DF0.unk20[arg0];
    e->unk0 = (e->unk0 & 0xFFE7) | (arg1 & 0x18);
    return 1;
}

s32 func_800C7D30(u32 arg0, u32 arg1, u32 arg2) {
    Unk800C9DF8 *b = D_800C9DF0.unk8;
    Unk800C9E10 *e;
    Unk800C9DF8Pt *pt;

    if (arg0 >= b->unk2C) {
        return 0;
    }
    e = &D_800C9DF0.unk20[arg0];
    if (arg1 >= e->unk2) {
        return 0;
    }
    if (arg2 >= e->unk2) {
        return 0;
    }
    pt = (Unk800C9DF8Pt *)((u8 *)b + e->unkC);
    e->unk8 = arg1 << 8;
    e->unk6 = 0;
    e->unk0 |= 5;
    pt += arg2;
    pt->unk2 = -1;
    if ((e->unk4 > 0 && arg2 < arg1) || (e->unk4 < 0 && arg1 < arg2)) {
        e->unk4 = -e->unk4;
    }
    return 1;
}

s32 func_800C7DF0(u32 arg0, s32 arg1) {
    Unk800C9DF8 *b = D_800C9DF0.unk8;
    Unk800C9E10 *e;
    Unk800C9DF8Pt *pt;
    s32 *ids;
    s32 i;
    s32 j;
    Unk800C9E00 *n;

    if (arg0 >= b->unk2C) {
        return 0;
    }
    e = &D_800C9DF0.unk20[arg0];
    e->unk4 = 0x100;
    e->unk0 = 2;
    e->unk8 = 0;
    e->unk6 = 0;
    pt = (Unk800C9DF8Pt *)((u8 *)b + e->unkC);
    for (i = 0; i < e->unk2; pt++, i++) {
        ids = (s32 *)((u8 *)b + pt->unk6);
        for (j = 0; j < pt->unk4; ids++, j++) {
            n = &D_800C9DF0.unk10[*ids];
            if (arg1 != 0 && i == 0) {
                n->unk0 |= 1;
            } else {
                n->unk0 &= ~1;
            }
        }
    }
    return 1;
}

s32 func_800C7EE8(u32 arg0, s32 *arg1) {
    Unk800C9E10 *e;

    if (arg0 >= D_800C9DF0.unk8->unk2C) {
        return 0;
    }
    e = &D_800C9DF0.unk20[arg0];
    *arg1 = e->unk8 >> 8;
    return 1;
}

s32 func_800C7F34(s32 arg0, s32 arg1) {
    Unk800CAE98 *e = &D_800CAE98[arg0];

    e->unk14 = arg1 << 8;
    e->unk0 |= 5;
    func_800C6EEC();
    return 1;
}

s32 func_800C7F7C(s32 arg0, s32 arg1) {
    Unk800CAE98 *e = func_800C8108_inline(arg0);

    arg1 <<= 8;
    e->unk14 = arg1;
    e->unk0 = 0;
    func_800C6EEC();
    return 1;
}

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

void func_800C8124(void) {
    u8 *s;
    u8 *p;
    s32 w;
    s32 i;

    p = D_8006794C->unk1C->unk80C;
    if (p != NULL && *p != 0xFF) {
        s = p;
    } else {
        s = func_80035774(1);
    }
    w = func_8002B980(s, 0) + 0x10;
    if (w < 0x44) {
        w = 0x44;
    }
    D_800C9E20[5].w = w;
    D_800C9E20[5].x = 0x134 - w;
    for (i = 0; i < 6; i++) {
        D_800CB188.unk8[i].unk4 = D_800C9E20[i];
        func_80063118(&D_800CB188.unk8[i], NULL, 0);
    }
    if (D_80077E51 != 0) {
        func_80063118(&D_800CB188.unk8[0], &D_800C9E60[0], 1);
        func_80063118(&D_800CB188.unk8[1], &D_800C9E60[1], 1);
        func_80063118(&D_800CB188.unk8[2], &D_800C9E60[2], 1);
        func_80063118(&D_800CB188.unk8[3], &D_800C9E60[3], 1);
    }
}

void func_800C8284(void) {
    if (D_80077528 & 0x10000) {
        func_8002F488(0x65);
        func_800362C8();
    } else if (D_80077528 & 0x20000) {
        func_800C923C();
    } else if (D_80077528 & 0x10) {
        func_8002F488(0x67);
        D_800CB188.unk0 = D_800CB188.unk0 != 0 ? D_800CB188.unk0 - 1 : 6;
    } else if (D_80077528 & 0x40) {
        func_8002F488(0x67);
        D_800CB188.unk0 = D_800CB188.unk0 < 6 ? D_800CB188.unk0 + 1 : 0;
    }
}

void func_800C8354(void) {
    Unk80077E50 *m;
    s32 old;
    s32 new;

    if (D_80077528 & 0x10000) {
        func_8002F488(0x65);
        D_800CB188.unk1 = 0;
        return;
    } else if (D_80077528 & 0x20000) {
        func_8002F488(0x67);
        func_800361CC(D_800CB188.unk0 + 1);
        return;
    } else if (D_80077528 & 0x10) {
        m = &D_80077E50;
        old = m->unk3;
        new = func_800337D0(old);
    } else if (D_80077528 & 0x40) {
        m = &D_80077E50;
        old = m->unk3;
        new = func_8003380C(old);
    } else {
        return;
    }
    if (old != new) {
        func_8002F488(0x67);
        old = new;
        m->unk3 = old;
    }
}

void func_800C842C(void) {
    if (D_80077528 & 0x10000) {
        func_8002F488(0x65);
        if (D_800CB188.unk4 == 0) {
            D_800CB188.unk1 = 0;
        } else {
            D_800CB188.unk4 = 0;
        }
    } else if (D_80077528 & 0x10) {
        func_8002F488(0x67);
        if (D_800CB188.unk4 == 0) {
            D_800CB188.unk2 = D_800CB188.unk2 != 0 ? D_800CB188.unk2 - 1 : 3;
        } else {
            D_800CB188.unk3 = D_800CB188.unk3 != 0 ? D_800CB188.unk3 - 1 : 3;
        }
    } else if (D_80077528 & 0x40) {
        func_8002F488(0x67);
        if (D_800CB188.unk4 == 0) {
            D_800CB188.unk2 = D_800CB188.unk2 < 3 ? D_800CB188.unk2 + 1 : 0;
        } else {
            D_800CB188.unk3 = D_800CB188.unk3 < 3 ? D_800CB188.unk3 + 1 : 0;
        }
    } else if (D_80077528 & 0x20000) {
        if (D_800CB188.unk4 == 0) {
            func_8002F488(0x67);
            D_800CB188.unk4 = 1;
            D_800CB188.unk3 = D_800CB188.unk2;
        } else {
            func_8002F488(0x67);
            func_800C85B8(D_800CB188.unk2, D_800CB188.unk3);
            D_800CB188.unk4 = 0;
            D_800CB188.unk2 = D_800CB188.unk3;
        }
    }
}

void func_800C85B8(s32 arg0, s32 arg1) {
    Character *c;
    Character *t;

    if (arg0 == arg1) {
        c = D_8006794C->unk1C->unk54C[arg1];
        if (c != NULL) {
            c->unk34.b.unk9 ^= 1;
        }
    } else {
        t = D_8006794C->unk1C->unk54C[D_800CB188.unk2];
        D_8006794C->unk1C->unk54C[D_800CB188.unk2] = D_8006794C->unk1C->unk54C[D_800CB188.unk3];
        D_8006794C->unk1C->unk54C[D_800CB188.unk3] = t;
        func_800C86C8(arg0, arg1);
        if (D_8006794C->unk1C->unk54C[D_80077E50.unk3] == NULL) {
            D_80077E50.unk3 = func_8003378C();
        }
    }
}

void func_800C86C8(s32 arg0, s32 arg1) {
    RECT a0;
    RECT a1;
    RECT a2;
    RECT b0;
    RECT b1;
    RECT b2;

    func_80028A40(arg0, &a0, &a1, &a2);
    func_80028A40(arg1, &b0, &b1, &b2);
    StoreImage(&a0, D_80076F80);
    DrawSync(0);
    MoveImage(&a1, 0x3C0, 0x1B0);
    DrawSync(0);
    MoveImage(&a2, 0x3D0, 0x1B0);
    DrawSync(0);
    MoveImage(&b0, a0.x, a0.y);
    DrawSync(0);
    MoveImage(&b1, a1.x, a1.y);
    DrawSync(0);
    MoveImage(&b2, a2.x, a2.y);
    DrawSync(0);
    a1.x = 0x3C0;
    a1.y = 0x1B0;
    a2.x = 0x3D0;
    a2.y = 0x1B0;
    LoadImage(&b0, D_80076F80);
    DrawSync(0);
    MoveImage(&a1, b1.x, b1.y);
    DrawSync(0);
    MoveImage(&a2, b2.x, b2.y);
    DrawSync(0);
}

void func_800C883C(s32 arg0) {
    u8 buf[16];
    RECT *r;
    Character *c;
    s32 x;
    s32 color;
    s16 maxmp;

    c = D_8006794C->unk1C->unk54C[arg0];
    r = &D_800CB188.unk8[arg0].unk4;
    x = r->x + 8;
    if (!c->unk34.b.unk9) {
        x = r->x + 0x16;
    }
    func_800283E0(x, r->y + 1, 0x14, arg0, 0x2D, c->unk10 == 0);
    func_8002B5FC(r->x + 0x3C, r->y + 3, 0x14, c->unk0, 0);
    func_80026FC4(r->x + 0x86, r->y + 4, 0x14, c->unk38, 0);
    func_8002BAE4(c->unkB, buf, 2, 1);
    func_800275DC(r->x + 0x3C, r->y + 0x12, 0x14, 0xB4, 0);
    func_8002B56C(r->x + 0x60, r->y + 0x12, 0x14, buf, 0);
    func_800275DC(r->x + 0x3C, r->y + 0x1B, 0x14, 0xAF, 0);
    func_8002BAE4(c->unk10, buf, 4, 1);
    buf[4] = 0x2F;
    if (c->unk10 != 0) {
        color = ((u16)(c->unk18 / 6) >= c->unk10) * 4;
    } else {
        color = 3;
    }
    func_8002BAE4(c->unk18, &buf[5], -1, 1);
    func_8002B56C(r->x + 0x52, r->y + 0x1B, 0x14, buf, color);
    func_800275DC(r->x + 0x3C, r->y + 0x24, 0x14, 0xB0, 0);
    func_8002BAE4(c->unk12, buf, 3, 1);
    buf[3] = 0x2F;
    maxmp = c->unk1A;
    color = ((s16)(maxmp / 6) >= c->unk12) * 4;
    func_8002BAE4(maxmp, &buf[4], -1, 1);
    func_8002B56C(r->x + 0x59, r->y + 0x24, 0x14, buf, color);
    func_800273C0(r->x + 0x9B, r->y + 0x14, 0x14, c->unk17, c->unk1F);
}

void func_800C8AE8(RECT *r, s32 otz) {
    TileWords *p;

    setlen(D_80077170, 3);
    p = (TileWords *)D_80077170;
    p->rgbc = 0x62000000;
    p->xy = ((r->x + 8) & 0xFFFF) | (r->y << 16);
    p->wh = 0x30002E;
    D_80077170 = (u8 *)(p + 1);
    addPrim(&D_8007716C[otz], p);
    SetDrawTPage(D_80077170, 1, 1, 0);
    addPrim(&D_8007716C[otz], D_80077170);
    D_80077170 += 8;
}

void func_800C8C08(void) {
    u8 buf[16];
    s32 v;

    v = func_80024030();
    func_800275DC(D_800C9EA0->x + 6, D_800C9EA0->y + 8, 0x13, 0xB1, 0);
    func_800275DC(D_800C9EA0->x + 6, D_800C9EA0->y + 0x15, 0x13, 0xB2, 0);
    func_8002B56C(D_800C9EA0->x + 0x13, D_800C9EA0->y + 9, 0x13, buf, func_8002AA04(v, buf, 1));
    func_8002BAE4(D_8006794C->unk1C->unk560, buf, 7, 1);
    func_80063108(buf, D_800C9EA4);
    func_8002B56C(D_800C9EA0->x + 0x13, D_800C9EA0->y + 0x16, 0x13, buf, 0);
    func_8002B524(D_800C9EA0->x + 6, D_800C9EA0->y - 3, 0x13, D_800704C0, 0);
    func_8002DEE4(D_800C9EA0, 0x14, 2);
}

void func_800C8D40(void) {
    GameData *gd;

    func_8002B524(D_800C9EA8->x + 8, D_800C9EA8->y - 3, 0x13, D_8007046C, 0);
    func_8002DEE4(D_800C9EA8, 0x14, 2);
    func_8002705C(D_800C9EA8, 0x14, 2);
    gd = D_8006794C->unk1C;
    if (gd->unk80C != NULL && *gd->unk80C != 0xFF) {
        func_8002B5FC(D_800C9EA8->x + 8, D_800C9EA8->y + 7, 0x13, gd->unk80C, 0);
    } else {
        func_8002B5FC(D_800C9EA8->x + 8, D_800C9EA8->y + 7, 0x13, func_80035774(1), 0);
    }
}

void func_800C8E2C(void) {
    u8 blink;

    blink = (D_80077E54 & 3) != 0;
    func_80035674(D_800CB188.unk1 - 1U >= 2);
    if (!func_8002F810()) {
        if (D_800CB188.unk1 == 0 || blink) {
            D_80077DE8.unk0 = 0xE0;
            D_80077DE8.unk4 = 1;
            D_80077DE8.unk2 = D_800CB188.unk0 * 0xF + 0x11;
            func_800355E4();
        }
        if (D_800CB188.unk1 == 1) {
            D_80077DE8.unk4 = 0xC;
            D_80077DE8.unk0 = D_800CB188.unk8[0].unk4.x - 0xE;
            D_80077DE8.unk2 = D_80077E53 * 0x2E + 0x20;
            func_800355E4();
        }
        if (D_800CB188.unk1 == 2) {
            if (D_800CB188.unk4 == 1) {
                D_80077DE8.unk4 = 0xB;
                D_80077DE8.unk0 = D_800CB188.unk8[0].unk4.x - 0xA;
                D_80077DE8.unk2 = D_800CB188.unk3 * 0x2E + 0x24;
                func_800355E4();
                if (blink) {
                    func_800270EC(D_800CB188.unk8[0].unk4.x - 0xE, D_800CB188.unk2 * 0x2E + 0x20, 0xC);
                }
            } else if (D_800CB188.unk4 == 0) {
                D_80077DE8.unk4 = 0xC;
                D_80077DE8.unk0 = D_800CB188.unk8[0].unk4.x - 0xE;
                D_80077DE8.unk2 = D_800CB188.unk2 * 0x2E + 0x20;
                func_800355E4();
            }
        }
    }
}

void func_800C9014(void) {
    D_80077DE8.unk8 = 0;
    if (D_800CB188.unk1 == 0) {
        D_80077DE8.unk8 = (u8 *)func_8003631C(D_800CB188.unk0);
    } else if (D_800CB188.unk1 == 1) {
        if (D_800CB188.unk0 + 1 == 3) {
            func_800C92C8(D_800CB188.unk0);
        } else {
            s32 a = D_800CB188.unk0 + 2;
            if (D_800CB188.unk0 + 1 == 4) {
                func_800C90D0(a);
            } else {
                D_80077DE8.unk8 = func_80035774(a);
            }
        }
    } else if (D_800CB188.unk1 == 2) {
        D_80077DE8.unk8 = func_80035774(D_800CB188.unk4 + 0xB);
    }
}

void func_800C90D0(s32 arg0) {
    Character *c;
    u32 next;

    c = D_8006794C->unk1C->unk54C[D_80077E53];
    if (c != NULL) {
        if (c->unkB >= 99) {
            next = c->unkC;
        } else {
            next = D_8006DF5C[c->unkB];
        }
        func_8002BE44(0, c->unk0);
        func_8002BAE4(c->unkC, D_800CB168, 7, 1);
        func_8002BAE4(next - c->unkC, D_800CB178, 7, 1);
        func_8002BE44(1, D_800C9EC0);
        func_8002BE44(2, D_800CB168);
        func_8002BE44(3, D_800C9EC4);
        func_8002BE44(4, D_800C9ECC);
        func_8002BE44(5, D_800CB178);
        func_8002BE44(6, D_800C9ED4);
        func_8002BE44(7, c->unk38 != 0 ? D_800C9EE4 : D_800C9EDC);
        D_80077DF0 = func_80035774(5);
    }
}

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

void func_800C92C8(u8 arg0) {
    s32 i;
    s32 j;
    Character *c;
    s32 id;

    i = 0;
    j = 5;
    c = D_8006794C->unk1C->unk54C[D_80077E53];
    do {
        func_8002BE44(i, D_800C9EAC[i]);
        id = c->unk39[i];
        if (id != 0xFF) {
            func_8002BE44(j, D_8006A218 + D_80069018[id].unk0);
        } else {
            func_8002BE44(j, func_80035774(0xD));
        }
        j++;
        func_8002BE74(i, id);
        i++;
    } while (i < 5);
    D_80077DE8.unk8 = func_80035774(4);
}

void func_800C93D4(void) {
    RECT r;

    if (D_80077E50.unk1 != 0) {
        D_800CB188.unk0 = 0;
        D_800CB188.unk2 = 0;
    }
    D_800CB188.unk1 = 0;
    func_8003575C(D_800A7860);
    func_800C8124();
    r.x = 8;
    r.y = 8;
    r.w = 0xE8;
    r.h = D_8006794C->unkC->disp.h - 0xF;
    func_80035580(&r);
    func_8003556C(func_800C9014);
    func_80035674(1);
    if (D_8006794C->unk1C->unk54C[D_80077E50.unk3] == NULL) {
        D_80077E50.unk3 = func_8003378C();
    }
}

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

void func_800C9630(void) {
    RECT r;
    s16 y;
    s32 i;
    RECT *p;

    i = 0;
    do {
        p = &D_800CB188.unk8[i].unk4;
        r.x = p->x + 0x82;
        y = p->y;
        r.w = 0x72;
        r.h = 0x12;
        r.y = y;
        if (D_8006794C->unk1C->unk54C[i] != NULL) {
            func_800C883C(i);
        }
        func_800C8AE8(p, 0x14);
        func_8002DEE4(&r, 0x14, 0);
        func_8002DEE4(p, 0x14, 0);
        func_8002705C(p, 0x15, 0);
        i++;
    } while (i < 4);
}

s32 func_800C9700(u16 *arg0) {
    return *arg0 & 0x7FFF;
}

INCLUDE_ASM("asm/us/ovl_0a/nonmatchings/ovl_0a", func_800C970C);

/* upload the images and palettes of a texture file to VRAM */
void func_800C9968(s32 *arg0) {
    RECT r;
    RECT *q;
    u16 *p;
    u8 *data;
    s32 n;
    s32 i;
    s32 sz;
    s32 m;
    s32 ofs;

    ofs = arg0[0];
    q = (RECT *)((u8 *)arg0 + ofs);
    ofs = arg0[1];
    i = 0;
    p = (u16 *)((u8 *)arg0 + ofs);
    ofs = *(s32 *)q;
    n = arg0[2];
    data = (u8 *)arg0 + ofs;
    for (; i < n; i++) {
        r.x = p[0];
        r.y = p[1];
        r.w = p[2];
        r.h = p[3];
        sz = r.w * r.h * 2;
        p += 4;
        LoadImage(&r, (u32 *)p);
        p = (u16 *)((u8 *)p + sz);
    }
    m = ((s32 *)q)[1];
    q++;
    for (i = 0; i < m; i++) {
        r.x = q->x;
        r.y = q->y;
        r.w = q->w;
        r.h = q->h;
        LoadImage(&r, (u32 *)data);
        q++;
        data += r.w * r.h * 2;
    }
}

INCLUDE_ASM("asm/us/ovl_0a/nonmatchings/ovl_0a", func_800C9AB8);

INCLUDE_RODATA("asm/us/ovl_0a/nonmatchings/ovl_0a", D_800A7814);

INCLUDE_RODATA("asm/us/ovl_0a/nonmatchings/ovl_0a", D_800A7860);
