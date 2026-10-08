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

INCLUDE_ASM("asm/us/ovl_0a/nonmatchings/ovl_0a", func_800AB3CC);

INCLUDE_ASM("asm/us/ovl_0a/nonmatchings/ovl_0a", func_800ABA98);

INCLUDE_ASM("asm/us/ovl_0a/nonmatchings/ovl_0a", func_800AC02C);

INCLUDE_ASM("asm/us/ovl_0a/nonmatchings/ovl_0a", func_800AC344);

u32 func_800AC380(Unk800C9CCC **arg0) {
    u32 min;
    s32 i;

    min = 0x801EDF00;
    if (arg0 != NULL) {
        *arg0 = NULL;
    }
    for (i = 9; i >= 0; i--) {
        if (func_800C9CCC[i].unk4 != 0 && func_800C9CCC[i].unk4 < min) {
            min = func_800C9CCC[i].unk4;
            if (arg0 != NULL) {
                *arg0 = &func_800C9CCC[i];
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

INCLUDE_ASM("asm/us/ovl_0a/nonmatchings/ovl_0a", func_800AC474);

void func_800AC5F0(void) {
    s32 i;

    for (i = 9; i >= 0; i--) {
        func_800C9CCC[i].unk0 = 0;
        func_800C9CCC[i].unk4 = 0;
        func_800C9CCC[i].unk8 = 0;
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
        if (func_800C9CCC[i].unk4 != 0 && (u32)arg0 >= func_800C9CCC[i].unk4) {
            func_800C9CCC[i].unk8 = 0;
            func_800C9CCC[i].unk4 = 0;
        }
    }
}

INCLUDE_ASM("asm/us/ovl_0a/nonmatchings/ovl_0a", func_800AC6C0);

void func_800AD44C(s32 arg0, s32 arg1, s32 arg2, s32 arg3) {
    func_80062E3C(arg0, 0x100, 0xFF, arg2, arg3, arg1, 3);
}

INCLUDE_ASM("asm/us/ovl_0a/nonmatchings/ovl_0a", func_800AD488);

INCLUDE_ASM("asm/us/ovl_0a/nonmatchings/ovl_0a", func_800AD944);

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

INCLUDE_ASM("asm/us/ovl_0a/nonmatchings/ovl_0a", func_800ADB74);

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
        e->unk8 = arg2;
        e->unkA = arg3;
        e->unkC = arg4;
    } else if (arg1 == 0x8C) {
        e->unk10 = arg2;
        e->unk12 = arg3;
        e->unk14 = arg4;
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
        e->unk1C = arg2;
        e->unk1E = arg3;
        e->unk20 = arg4;
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
    h->unkC = arg0;
    h->unk10 = arg0 + 0x3000;
    D_8006794C->unk1C->unk8E4.field->unkC->unk14->unk10 = h;
    return arg0 + 0x6000;
}

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

u8 *func_800B3300(Unk800B3254 *arg0, u8 **arg1, u8 **arg2) {
    u8 *base;
    u8 *p;

    *arg1 = NULL;
    *arg2 = NULL;
    base = arg0->unkC[0];
    if (arg0->unk7 == 0) {
        return NULL;
    }
    p = arg0->unkC[arg0->unk7 - 1];
    do {
        if ((s8)p[0] == 1) {
            *arg1 = p;
        } else if ((s8)p[0] == 2) {
            *arg2 = p;
        }
        p -= 12;
    } while (p >= base);
    return base;
}

INCLUDE_ASM("asm/us/ovl_0a/nonmatchings/ovl_0a", func_800B3378);

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

void func_800B5660(void) {
    s32 y;
    Unk80077DE8 *m;

    func_8002B524(D_800C9D98.x + 8, D_800C9D98.y - 3, 0xA, "DISC CHANGE", 0);
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

INCLUDE_ASM("asm/us/ovl_0a/nonmatchings/ovl_0a", func_800B61A4);

INCLUDE_ASM("asm/us/ovl_0a/nonmatchings/ovl_0a", func_800B6438);

INCLUDE_ASM("asm/us/ovl_0a/nonmatchings/ovl_0a", func_800B6584);

INCLUDE_ASM("asm/us/ovl_0a/nonmatchings/ovl_0a", func_800B71E0);

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
        arg0->unk0 |= *arg0->unk1C & 0x10;
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

INCLUDE_ASM("asm/us/ovl_0a/nonmatchings/ovl_0a", func_800B7F14);

INCLUDE_ASM("asm/us/ovl_0a/nonmatchings/ovl_0a", func_800B8834);

INCLUDE_ASM("asm/us/ovl_0a/nonmatchings/ovl_0a", func_800B8C80);

INCLUDE_ASM("asm/us/ovl_0a/nonmatchings/ovl_0a", func_800B9134);

INCLUDE_ASM("asm/us/ovl_0a/nonmatchings/ovl_0a", func_800B9738);

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

INCLUDE_ASM("asm/us/ovl_0a/nonmatchings/ovl_0a", func_800BA560);

INCLUDE_ASM("asm/us/ovl_0a/nonmatchings/ovl_0a", func_800BB090);

INCLUDE_ASM("asm/us/ovl_0a/nonmatchings/ovl_0a", func_800BB3C4);

INCLUDE_ASM("asm/us/ovl_0a/nonmatchings/ovl_0a", func_800BB508);

INCLUDE_ASM("asm/us/ovl_0a/nonmatchings/ovl_0a", func_800BB714);

INCLUDE_ASM("asm/us/ovl_0a/nonmatchings/ovl_0a", func_800BC33C);

INCLUDE_ASM("asm/us/ovl_0a/nonmatchings/ovl_0a", func_800BC600);

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

INCLUDE_ASM("asm/us/ovl_0a/nonmatchings/ovl_0a", func_800C11E8);

s32 func_800C130C(s32 arg0, u8 *arg1, u8 *arg2, u8 *arg3) {
    *arg1 = D_800C9DA4.unk1C[arg0 * 3];
    *arg2 = D_800C9DA4.unk1C[arg0 * 3 + 1];
    *arg3 = D_800C9DA4.unk1C[arg0 * 3 + 2];
    return 1;
}

INCLUDE_ASM("asm/us/ovl_0a/nonmatchings/ovl_0a", func_800C1368);

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

INCLUDE_ASM("asm/us/ovl_0a/nonmatchings/ovl_0a", func_800C16B0);

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

INCLUDE_ASM("asm/us/ovl_0a/nonmatchings/ovl_0a", func_800C18D4);

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

INCLUDE_ASM("asm/us/ovl_0a/nonmatchings/ovl_0a", func_800C1A8C);

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

INCLUDE_ASM("asm/us/ovl_0a/nonmatchings/ovl_0a", func_800C2524);

u16 func_800C2920(Unk800C9DF4 *arg0, Unk800C9E00 **arg1, s32 arg2, s32 arg3) {
    Unk800C9E00 *p[2];
    u16 i;

    if (arg3 < 9) {
        p[0] = *arg1;
        if (func_800C4B30(D_800C9DF0.unk8, &D_800C9DF0.unk1C[p[0]->unk4], p[0], arg2)) {
            return 1;
        }
        for (i = 0; i < 3; i++) {
            p[1] = func_800C1F98(arg0, p[0], i);
            if (p[1] != NULL && func_800C4B30(D_800C9DF0.unk8, &D_800C9DF0.unk1C[p[1]->unk4], p[1], arg2)) {
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

INCLUDE_ASM("asm/us/ovl_0a/nonmatchings/ovl_0a", func_800C2AF0);

INCLUDE_ASM("asm/us/ovl_0a/nonmatchings/ovl_0a", func_800C31F8);

INCLUDE_ASM("asm/us/ovl_0a/nonmatchings/ovl_0a", func_800C3608);

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

INCLUDE_ASM("asm/us/ovl_0a/nonmatchings/ovl_0a", func_800C3D28);

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

INCLUDE_ASM("asm/us/ovl_0a/nonmatchings/ovl_0a", func_800C3EC0);

s32 func_800C420C(s32 arg0, s16 arg1) {
    Unk800C9E00 *n;
    Unk800C9DF8Tri *t;
    SVECTOR v;

    n = &D_800C9DF0.unk10[(u16)arg1];
    t = &D_800C9DF0.unk1C[n->unk4];
    v.vx = D_800C9DF0.unk8->unk8 + t->unkA[0];
    v.vy = D_800C9DF0.unk8->unkA + t->unkA[1];
    v.vz = D_800C9DF0.unk8->unkC + t->unkA[2];
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

INCLUDE_ASM("asm/us/ovl_0a/nonmatchings/ovl_0a", func_800C4378);

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
                vx = &D_800C9DF0.unkC[n->unkC];
                p.vy = vx->unk2 + (D_800C9DF0.unk8->unkA + t->unkA[1]);
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
                p.vy = D_800C9DF0.unk8->unkA;
                p.vy += D_800C9DF0.unk1C[n->unk4].unkA[1];
                p.vy += D_800C9DF0.unkC[n->unkC].unk2;
                return p.vy;
            }
        }
    }
    return -0x7FFF;
}

INCLUDE_ASM("asm/us/ovl_0a/nonmatchings/ovl_0a", func_800C4B30);

INCLUDE_ASM("asm/us/ovl_0a/nonmatchings/ovl_0a", func_800C4EC8);

INCLUDE_ASM("asm/us/ovl_0a/nonmatchings/ovl_0a", func_800C519C);

INCLUDE_RODATA("asm/us/ovl_0a/nonmatchings/ovl_0a", D_800A717C);

INCLUDE_ASM("asm/us/ovl_0a/nonmatchings/ovl_0a", func_800C5A7C);

INCLUDE_ASM("asm/us/ovl_0a/nonmatchings/ovl_0a", func_800C60F0);

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

INCLUDE_ASM("asm/us/ovl_0a/nonmatchings/ovl_0a", func_800C7100);

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

s32 func_800C77DC(u16 *arg0, u8 **arg1) {
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

s32 func_800C7914(u16 *arg0, u8 **arg1) {
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

INCLUDE_ASM("asm/us/ovl_0a/nonmatchings/ovl_0a", func_800C7EE8);

s32 func_800C7F34(s32 arg0, s32 arg1) {
    Unk800CAE98 *e = &D_800CAE98[arg0];

    e->unk14 = arg1 << 8;
    e->unk0 |= 5;
    func_800C6EEC();
    return 1;
}

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
            c->unk34 = (c->unk34 & ~0x200) | ((((c->unk34 >> 9) & 1) ^ 1) << 9);
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
    if (!((c->unk34 >> 9) & 1)) {
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

INCLUDE_ASM("asm/us/ovl_0a/nonmatchings/ovl_0a", func_800C9968);

INCLUDE_ASM("asm/us/ovl_0a/nonmatchings/ovl_0a", func_800C9AB8);

INCLUDE_ASM("asm/us/ovl_0a/nonmatchings/ovl_0a", func_800C9CCC);

INCLUDE_RODATA("asm/us/ovl_0a/nonmatchings/ovl_0a", D_800A7814);

INCLUDE_RODATA("asm/us/ovl_0a/nonmatchings/ovl_0a", D_800A7860);
