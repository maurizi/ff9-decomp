#include "ovl_0b.h"

void func_800A859C(void) {
    func_800A8CD4();
    do {
        func_800A8600((s16)D_8006794C->unk1C->unkE);
    } while (!(D_8006794C->unk0 & 0x101));
    func_800A8DE0();
}

INCLUDE_ASM("asm/us/ovl_0b/nonmatchings/ovl_0b", func_800A8600);

INCLUDE_RODATA("asm/us/ovl_0b/nonmatchings/ovl_0b", D_800A7000);

INCLUDE_ASM("asm/us/ovl_0b/nonmatchings/ovl_0b", func_800A882C);

void func_800A8BF4(void) {
    volatile u8 a;
    volatile s32 b;
    u16 f;
    a = func_800230A0();
    f = D_8006794C->unk1C->unk8E4.battle->unk0;
    if (f & 0x10) {
        if (!(f & 0x20)) {
            if (a != 0) {
                func_800EA338(1);
                D_8006794C->unk1C->unk8E4.battle->unk0 |= 0x20;
            }
        } else {
            b = func_80022B18();
            if (b == 0) {
                func_800EA338(1);
                D_8006794C->unk1C->unk8E4.battle->unk0 &= ~0x20;
            }
        }
    }
}

void func_800A8CD4(void) {
    GameData *g = D_8006794C->unk1C;
    Battle *b;
    func_800A8E24();
    b = g->unk8E4.battle;
    func_800A8E6C();
    D_8006794C->unk9 = 4;
    func_8001CD04(0xDA);
    func_8001CFB4();
    SetDispMask(1);
    g->unk838 = func_800AB810;
    g->unk83C = func_800AD90C;
    D_8006794C->unk1C->unk8BC = func_800AF1C8;
    func_80021C60();
    g->unk39 = 0;
    g->unk38 = 0;
    g->unk84C = 0;
    b->unk1534 = D_8006794C->unk20 + D_8006794C->unk14->unkDC;
    func_800A90B0();
    func_800A8F10();
    func_800A8F18();
    func_800A8F8C();
    func_800A902C();
    func_800A9090();
    func_800B031C();
    b->unk4 = D_80067948;
}

void func_800A8DE0(void) {
    s32 i;
    for (i = 1; i >= 0; i--) {
        D_800739B0[i].unk1 = 0;
        D_800739B0[i].unk0 = 0;
    }
    func_800A91EC();
}

INCLUDE_ASM("asm/us/ovl_0b/nonmatchings/ovl_0b", func_800A8E24);

INCLUDE_ASM("asm/us/ovl_0b/nonmatchings/ovl_0b", func_800A8E6C);

s32 func_800A8F10(void) {
    return 0;
}

s32 func_800A8F18(void) {
    s32 a = (s32)func_8001C7FC(4, 0x70005, 0);
    func_8001DAA4(D_8006794C->unk1C->unk818, a, (s32)func_8001C7FC(5, 0x70005, 0), NULL);
    DrawSync(0);
    return 0;
}

s32 func_800A8F8C(void) {
    s32 i;
    for (i = 1; i >= 0; i--) {
        s32 id = D_800F51E8[i] | 0x90000;
        s32 a = (s32)func_8001C7FC(4, id, 0);
        s32 b = (s32)func_8001C7FC(5, id, 0);
        func_80055DF4();
        func_80055E14(a, b, 1);
    }
    return 0;
}

s32 func_800A902C(void) {
    s32 p = (s32)func_8001C7FC(4, 0x42774, 0);
    func_8001D9CC(p, 0x100, 0xFF, 0x200, 0xE0);
    func_8001D8A8(p);
    DrawSync(0);
    return 0;
}

s32 func_800A9090(void) {
    func_800E9D84();
    return 0;
}

INCLUDE_ASM("asm/us/ovl_0b/nonmatchings/ovl_0b", func_800A90B0);

s32 func_800A91EC(void) {
    Battle1354 *p = D_8006794C->unk1C->unk8E4.battle->unk1354;
    func_8001C784(p->unk0[0]);
    p->unk0[0] = NULL;
    return 0;
}

INCLUDE_ASM("asm/us/ovl_0b/nonmatchings/ovl_0b", func_800A923C);

INCLUDE_ASM("asm/us/ovl_0b/nonmatchings/ovl_0b", func_800A94F8);

INCLUDE_ASM("asm/us/ovl_0b/nonmatchings/ovl_0b", func_800A9768);

INCLUDE_ASM("asm/us/ovl_0b/nonmatchings/ovl_0b", func_800A98A0);

s32 func_800A99AC(Unk800AA08C *arg0, s32 arg1) {
    s32 id = (arg1 & 0xFFFF) | 0x110000;
    arg0->unk24 = (s32)func_8001C7FC(4, id, 0);
    arg0->unk20 = (s32)func_8001C7FC(4, (s32)func_8001C7FC(9, id, 0x100000), 0);
    arg0->unk10 = (s32)func_8001C7FC(4, (s32)func_8001C7FC(9, id, 0x50000), 0);
    arg0->unk14 = (s32)func_8001C7FC(4, (s32)func_8001C7FC(9, id, 0x60000), 0);
    func_800C94B8();
    func_8004A06C(arg0->unk10);
    func_8004BFB0();
    return 0;
}

INCLUDE_ASM("asm/us/ovl_0b/nonmatchings/ovl_0b", func_800A9A7C);

INCLUDE_ASM("asm/us/ovl_0b/nonmatchings/ovl_0b", func_800A9BE4);

s32 func_800AA08C(Unk800AA08C *arg0) {
    s32 v = arg0->unk24;
    GameData *gd = D_8006794C->unk1C;
    u8 *cd = gd->unk854;
    Battle *b = gd->unk8E4.battle;
    func_8002177C(cd, gd->unk7FC, gd->unk804 << 11, (void *)arg0->unk24, 0, 0x2800, 0);
    while (func_800217C4(cd) != 0) {
    }
    b->unk1354->unk0[2] = (void *)v;
    func_8001C750(v, 0);
    gd->unk7E4 = (FileTable *)func_8001C8B0(v, 2, 0x140000, 0);
    return 0;
}

INCLUDE_ASM("asm/us/ovl_0b/nonmatchings/ovl_0b", func_800AA140);

INCLUDE_ASM("asm/us/ovl_0b/nonmatchings/ovl_0b", func_800AA24C);

INCLUDE_ASM("asm/us/ovl_0b/nonmatchings/ovl_0b", func_800AA504);

INCLUDE_ASM("asm/us/ovl_0b/nonmatchings/ovl_0b", func_800AA958);

INCLUDE_ASM("asm/us/ovl_0b/nonmatchings/ovl_0b", func_800AABC4);

INCLUDE_ASM("asm/us/ovl_0b/nonmatchings/ovl_0b", func_800AAEF0);

INCLUDE_ASM("asm/us/ovl_0b/nonmatchings/ovl_0b", func_800AB07C);

s32 func_800AB26C(void **arg0, s32 arg1) {
    void **p = &arg0[arg1];
    do {
        if (*p != NULL) {
            func_8001C784(*p);
        }
        *p = NULL;
        p--;
    } while (p >= arg0);
    return 0;
}

INCLUDE_ASM("asm/us/ovl_0b/nonmatchings/ovl_0b", func_800AB2C8);

s32 func_800AB324(s32 arg0, s32 *arg1, s32 *arg2) {
    FileEntry *e = func_80063088(D_8006794C->unk1C->unk7E4, arg0);
    *arg1 = e->unk4;
    *arg2 = e->unk8;
    return arg0;
}

void func_800AB38C(s32 arg0, s32 *arg1, s32 *arg2) {
    func_800AB324(func_80063088(D_8006794C->unk1C->unk7E4, arg0)->unkC, arg1, arg2);
}

void func_800AB3E4(s32 arg0, s32 arg1) {
    s32 v = 0x17;
    if (arg1 != 0) {
        v = 0x8017;
    }
    func_800ABBC4(arg0, v);
}

void func_800AB410(s32 arg1) {
    s32 v = 0x207;
    if (arg1 != 0) {
        v = 0x8207;
    }
    func_800A9BE4(v);
}

void func_800AB43C(void) {
    Battle *b = D_8006794C->unk1C->unk8E4.battle;
    Battle1354 *p = b->unk1354;
    func_800AB26C(p->unk20, 3);
    func_800AB26C(p->unk30, 3);
    func_800AB26C(D_8006794C->unk1C->unk8E4.battle->unk1354->unk30, 3);
    func_800AB26C(D_8006794C->unk1C->unk8E4.battle->unk1354->unk20, 3);
    b->unk0 &= ~4;
}

void func_800AB4FC(void) {
    Battle *b = D_8006794C->unk1C->unk8E4.battle;
    func_800AB26C(b->unk1354->unk10, 2);
    func_800AB26C(D_8006794C->unk1C->unk8E4.battle->unk1354->unk10, 2);
    b->unk0 &= ~2;
}

INCLUDE_ASM("asm/us/ovl_0b/nonmatchings/ovl_0b", func_800AB580);

void func_800AB6F0(s32 arg0, s32 arg1) {
    s32 v = D_8006794C->unk1C->unk8E4.battle->unk11C0->unk3C[arg1];
    func_8001C8B0(func_8001C8B0(v, 2, 0x1B0000, 0), 4, arg0, 0);
}

void func_800AB758(s32 arg0, s32 arg1, s32 **arg2, s32 **arg3, s32 **arg4) {
    s32 v = D_8006794C->unk1C->unk8E4.battle->unk11C0->unk3C[arg1];
    Unk800AB758 *h = (Unk800AB758 *)(func_8001C8B0(func_8001C8B0(v, 2, 0x1B0000, 0), 2, 0x120000, 0) + 4);
    s32 *p = (s32 *)((u8 *)h + h->unk4);
    s32 n = h->unk7;
    *arg2 = p;
    p += n;
    *arg3 = p;
    *arg4 = p + n;
}

INCLUDE_ASM("asm/us/ovl_0b/nonmatchings/ovl_0b", func_800AB810);

INCLUDE_ASM("asm/us/ovl_0b/nonmatchings/ovl_0b", func_800ABA10);

void func_800ABB60(s32 arg0) {
    s32 v = D_8006794C->unk1C->unk8E4.battle->unk11C0->unk3C[arg0];
    func_8001C8B0(func_8001C8B0(v, 2, 0x1B0000, 0), 2, 0x20000, 0);
}

INCLUDE_ASM("asm/us/ovl_0b/nonmatchings/ovl_0b", func_800ABBC4);

s32 func_800AC244(s32 arg0) {
    u16 *p = func_800633E4(arg0);
    if (p != NULL) {
        return p[1] | 0x30000;
    }
    return -1;
}

INCLUDE_ASM("asm/us/ovl_0b/nonmatchings/ovl_0b", func_800AC280);

INCLUDE_ASM("asm/us/ovl_0b/nonmatchings/ovl_0b", func_800AC37C);

void func_800AD4AC(s32 arg0, s32 arg1) {
    func_80062E3C(arg0, 0x100, 0xFF, 0x200, 0xE0, arg1, 0);
}

INCLUDE_ASM("asm/us/ovl_0b/nonmatchings/ovl_0b", func_800AD4E4);

INCLUDE_ASM("asm/us/ovl_0b/nonmatchings/ovl_0b", func_800AD90C);

s32 func_800AE47C(s32 arg0, u16 arg1) {
    void **p;
    GameData *g = D_8006794C->unk1C;
    s32 f;
    if (g->unk54C[arg0] == 0) {
        return -1;
    }
    p = &g->unk8E4.battle->unk1354->unk20[arg0];
    f = (s32)*p;
    if (f == 0) {
        return -1;
    }
    return func_8001C8B0(f, 3, arg1 | 0x80000, 0);
}

INCLUDE_ASM("asm/us/ovl_0b/nonmatchings/ovl_0b", func_800AE4FC);

s32 func_800AE570(s32 arg0, s32 *arg1) {
    FileEntry *e;
    FileEntry *f;
    if (((arg0 >> 16) & 0xFF) == 7) {
        e = func_80063088(D_8006794C->unk1C->unk7E4, arg0);
        if (arg1 != 0) {
            *arg1 = e->unk8 << 11;
        }
        return e->unk4;
    } else {
        f = func_8001E218(11, arg0);
        if (arg1 != 0) {
            *arg1 = (f->unkC - f->unk4) << 11;
        }
        return f->unk4;
    }
}

INCLUDE_ASM("asm/us/ovl_0b/nonmatchings/ovl_0b", func_800AE618);

INCLUDE_ASM("asm/us/ovl_0b/nonmatchings/ovl_0b", func_800AE764);

s32 func_800AE7C8(s32 arg0) {
    s32 v = func_8001C8B0(arg0, 3, 0x70000, 0);
    s32 a = func_8001C8B0(arg0, 4, v, 0);
    func_80015C28((u8 *)arg0, (u8 *)a, func_8001C8B0(arg0, 5, v, 0));
    return 0;
}

INCLUDE_ASM("asm/us/ovl_0b/nonmatchings/ovl_0b", func_800AE844);

void func_800AE9C8(s32 arg0, s16 arg1) {
    GameData *gd = D_8006794C->unk1C;
    gd->unk826[0] = arg0;
    if (arg0 == 0) {
        gd->unk842 = arg1;
        gd->unk8C4 = VSync(-1);
    }
}

s32 func_800AEA08(void) {
    GameData *gd = D_8006794C->unk1C;
    return (u32)(VSync(-1) - gd->unk8C4) < gd->unk842;
}

INCLUDE_ASM("asm/us/ovl_0b/nonmatchings/ovl_0b", func_800AEA48);

INCLUDE_ASM("asm/us/ovl_0b/nonmatchings/ovl_0b", func_800AEE68);

s32 func_800AF1C8(void) {
    return 0;
}

INCLUDE_ASM("asm/us/ovl_0b/nonmatchings/ovl_0b", func_800AF1D0);

INCLUDE_ASM("asm/us/ovl_0b/nonmatchings/ovl_0b", func_800AF2E0);

INCLUDE_ASM("asm/us/ovl_0b/nonmatchings/ovl_0b", func_800AF578);

INCLUDE_ASM("asm/us/ovl_0b/nonmatchings/ovl_0b", func_800AF800);

INCLUDE_ASM("asm/us/ovl_0b/nonmatchings/ovl_0b", func_800B0060);

void func_800B031C(void) {
    GameData *gd;
    s32 v;
    D_8006794C->unk1C->unk56F = 0;
    gd = D_8006794C->unk1C;
    v = 7;
    if (gd->unk571 != 0) {
        v = 8;
    }
    gd->unk3C[3].unk34 = (gd->unk3C[3].unk34 & ~0x1F0) | (v << 4);
    D_8006794C->unk1C->unk56E = 0;
    func_800C1524();
    func_800E3D80();
    func_800C2E04();
    func_800CBF90();
    func_800F4DFC();
}

INCLUDE_ASM("asm/us/ovl_0b/nonmatchings/ovl_0b", func_800B03B0);

INCLUDE_ASM("asm/us/ovl_0b/nonmatchings/ovl_0b", func_800B0468);

INCLUDE_ASM("asm/us/ovl_0b/nonmatchings/ovl_0b", func_800B05C0);

INCLUDE_ASM("asm/us/ovl_0b/nonmatchings/ovl_0b", func_800B065C);

INCLUDE_ASM("asm/us/ovl_0b/nonmatchings/ovl_0b", func_800B081C);

INCLUDE_ASM("asm/us/ovl_0b/nonmatchings/ovl_0b", func_800B0A6C);

s32 func_800B0CF8(s32 arg0, u32 arg1) {
    s32 i;
    GameData *gd = D_8006794C->unk1C;
    for (i = 0; i < 4; i++) {
        if (gd->unk54C[i]->unk88[arg0] & arg1) {
            return 1;
        }
    }
    return 0;
}

void func_800B0D4C(BtlUnit *arg0, u8 arg1) {
    s32 i;
    if (!(arg0->unk3C & 0x42001103) && arg1 < 0x30 && (arg0->unkC4[1] & 0x01000000)) {
        for (i = 0xEC; (u8)i < 0xEE; i++) {
            if (func_80027F34((u8)i) != 0) {
                func_800EA144((u8)i);
                func_800B3D54(arg0, 0x33, (u8)i, arg0->unkB0);
                return;
            }
        }
    }
}

INCLUDE_ASM("asm/us/ovl_0b/nonmatchings/ovl_0b", func_800B0E04);

void func_800B0EF8(Battle *arg0) {
    arg0->unk1088 = NULL;
    arg0->unk11BA = 0;
    arg0->unk11B8 = 2;
    arg0->unk1060.unk4 = NULL;
    arg0->unk1074.unk4 = NULL;
    func_800B3CEC_inline(&arg0->unk1074);
    func_800B3CEC_inline(&arg0->unk1060);
}

INCLUDE_ASM("asm/us/ovl_0b/nonmatchings/ovl_0b", func_800B0FC0);

INCLUDE_ASM("asm/us/ovl_0b/nonmatchings/ovl_0b", func_800B10F8);

INCLUDE_ASM("asm/us/ovl_0b/nonmatchings/ovl_0b", func_800B1250);

INCLUDE_ASM("asm/us/ovl_0b/nonmatchings/ovl_0b", func_800B1520);

INCLUDE_ASM("asm/us/ovl_0b/nonmatchings/ovl_0b", func_800B1764);

INCLUDE_ASM("asm/us/ovl_0b/nonmatchings/ovl_0b", func_800B1B48);

INCLUDE_ASM("asm/us/ovl_0b/nonmatchings/ovl_0b", func_800B23B4);

INCLUDE_ASM("asm/us/ovl_0b/nonmatchings/ovl_0b", func_800B253C);

INCLUDE_ASM("asm/us/ovl_0b/nonmatchings/ovl_0b", func_800B2804);

INCLUDE_ASM("asm/us/ovl_0b/nonmatchings/ovl_0b", func_800B2988);

INCLUDE_ASM("asm/us/ovl_0b/nonmatchings/ovl_0b", func_800B2DD4);

INCLUDE_ASM("asm/us/ovl_0b/nonmatchings/ovl_0b", func_800B2F24);

INCLUDE_ASM("asm/us/ovl_0b/nonmatchings/ovl_0b", func_800B3090);

INCLUDE_ASM("asm/us/ovl_0b/nonmatchings/ovl_0b", func_800B3188);

INCLUDE_ASM("asm/us/ovl_0b/nonmatchings/ovl_0b", func_800B3460);

void func_800B3B5C(BtlUnit *arg0) {
    s32 x;
    if (arg0->unk3C & 0x400) {
        if (D_8007B720 >= D_8007B92D) {
            func_8004BF20();
        }
        x = *D_8007B720++;
        x &= 1;
    } else if (arg0->unk3C & 0x800) {
        x = 0;
    } else {
        return;
    }
    func_800B1250((s32)arg0->unk4, 1, 0xB0, func_800C1F7C(x) & 0xFFFF, 0);
}

void func_800B3C00(void) {
    Battle *b = D_8006794C->unk1C->unk8E4.battle;
    BtlCmd *c = b->unk1088;
    s8 k = c->unk10_7;
    BtlUnit *u;
    if (k == 2) {
        c->unkC = func_800B4478(c->unk4->unk1C);
        func_800C2598(c, c->unk8->unk0_8, 0);
        b->unk11BA = k;
        return;
    }
    u = c->unk4;
    if (!(u->unk3C & 0x2000) || c->unkE != 0x3C || (s32)u->unk9C->unkC == u->unkA4[4]) {
        b->unk11BA = 4;
    }
}

void func_800B3CEC(BtlCmd *arg0) {
    arg0->unk0 = 0;
    arg0->unk8 = 0;
    arg0->unkC = 0;
    arg0->unkE = 0;
    arg0->unkF = 0;
    arg0->unk10_0 = 0;
    arg0->unk10_2 = 0;
    arg0->unk10_4 = 0;
    arg0->unk10_5 = 0;
    arg0->unk10_6 = 0;
    arg0->unk10_7 = 0;
    arg0->unk10_9 = 0;
    arg0->unk10_10 = 0;
    arg0->unk10_11 = 0;
}

void func_800B3D54(BtlUnit *arg0, s32 arg1, s32 arg2, s32 arg3) {
    if (!(arg0->unk3C & 0x02021003) && D_8006794C->unk1C->unk8E4.battle->unkD80 == 4) {
        func_800B1250(arg0->unk8, arg1, arg2, arg3 & 0xFFFF, 0);
    }
}

void func_800B3DBC(BtlUnit *arg0) {
    if ((arg0->unk68 & 0x1F) == 5) {
        Battle *b = D_8006794C->unk1C->unk8E4.battle;
        BtlCmd *p;
        for (p = &b->unk1074; p != NULL; p = p->unk0) {
            BtlCmd *n = p->unk0;
            if (n != NULL && n->unk4 == arg0 && n->unkE == 0x39) {
                func_800B4320(&p->unk0, 1);
                break;
            }
        }
        b->unk11BC = 0;
        b->unk11BB = 0;
        b->unk11B8 &= ~0xC;
    }
}

u8 func_800B3E7C(BtlCmd *arg0) {
    BtlCmd *p;
    for (p = &D_8006794C->unk1C->unk8E4.battle->unk1074; p != NULL; p = p->unk0) {
        if (arg0 == p) {
            return 1;
        }
    }
    p = D_8006794C->unk1C->unk8E4.battle->unk1088;
    if (p != NULL && p == arg0) {
        return 1;
    }
    return 0;
}

u8 func_800B3EF8(BtlUnit *arg0, u8 arg1) {
    BtlCmd *p;
    for (p = &D_8006794C->unk1C->unk8E4.battle->unk1074; p != NULL; p = p->unk0) {
        if (p->unk4 == arg0 && p->unkE == arg1) {
            return 1;
        }
    }
    p = D_8006794C->unk1C->unk8E4.battle->unk1088;
    if (p != NULL && p->unkE == arg1) {
        return 1;
    }
    return 0;
}

void func_800B3F98(BtlCmd *arg0) {
    BtlUnit *u = arg0->unk4;
    BtlCmd *p;
    for (p = &D_8006794C->unk1C->unk8E4.battle->unk1074; p != NULL; p = p->unk0) {
        if (p->unk0 == arg0) {
            if (u != NULL && arg0 != u->unk10 && arg0 != u->unk14) {
                u->unk68 &= ~0x80000;
            }
            func_800B4320(&p->unk0, 0);
            return;
        }
    }
}

s32 func_800B4040(BtlUnit *arg0) {
    BtlCmd *c;
    BtlCmd *p;
    s32 r;
    arg0->unk68 &= ~0x80000;
    p = &D_8006794C->unk1C->unk8E4.battle->unk1074;
    r = 0;
    while (p != NULL) {
        c = p->unk0;
        if (c != NULL && c->unk4 == arg0 && c->unkE < 0x36) {
            if (c->unkE < 0x30) {
                r = 1;
            }
            func_800B4434(c);
            func_800B4320(&p->unk0, 1);
        } else {
            p = c;
        }
    }
    return r;
}

void func_800B4114(BtlUnit *arg0) {
    BtlCmd *p = &D_8006794C->unk1C->unk8E4.battle->unk1074;
    while (p != NULL) {
        BtlCmd *n = p->unk0;
        if (n != NULL && n->unk4 == arg0) {
            func_800B4434(n);
            func_800B4320(&p->unk0, 1);
        } else {
            p = n;
        }
    }
}

void func_800B41A4(BtlUnit *arg0, u8 arg1) {
    BtlCmd *p;
    for (p = &D_8006794C->unk1C->unk8E4.battle->unk1074; p != NULL; p = p->unk0) {
        BtlCmd *n = p->unk0;
        if (n != NULL && n->unk4 == arg0 && n->unkE == arg1) {
            func_800B4320(&p->unk0, 1);
            return;
        }
    }
}

INCLUDE_ASM("asm/us/ovl_0b/nonmatchings/ovl_0b", func_800B4234);

void func_800B4320(BtlCmd **arg0, u8 arg1) {
    BtlCmd *p = *arg0;
    *arg0 = p->unk0;
    p->unk10_2 = 0;
    p->unk10_4 = 0;
    if (arg1 && p->unkE == 0x38) {
        D_8006794C->unk1C->unk8E4.battle->unk11B8 &= ~1;
    }
}

s32 func_800B4390(BtlCmd *arg0) {
    s32 mp = arg0->unk8->unkA;
    if (D_8007AF02 != 0 && (arg0->unk8->unkB & 4)) {
        mp <<= 2;
    }
    if (arg0->unkE == 0x32) {
        return 1;
    }
    if (func_800B9550(arg0->unk4, mp) & 0xFF) {
        return 1;
    }
    func_800CD5C8(0x16);
    return 0;
}

void func_800B440C(LinkNode *arg0, LinkNode **arg1) {
    arg0->unk0 = *arg1;
    *arg1 = arg0;
    arg0->unk10 = (arg0->unk10 & ~0xC) | 4;
}

void func_800B4434(BtlCmd *arg0) {
    u8 v = arg0->unkE;
    if ((u32)(v - 0xE) < 2 || v == 0x33) {
        func_800EA124(arg0->unkF);
    }
}

u16 func_800B4478(u16 *arg0) {
    s32 i;
    u16 r;
    r = i = 0;
    for (; i < 4; i++) {
        r |= arg0[i];
    }
    return r;
}

void func_800B44A0(Unk800B44A0 *arg0) {
    s32 i;
    for (i = 3; i >= 0; i--) {
        arg0->unk1C[i] = 0;
    }
}

INCLUDE_ASM("asm/us/ovl_0b/nonmatchings/ovl_0b", func_800B44C0);

INCLUDE_ASM("asm/us/ovl_0b/nonmatchings/ovl_0b", func_800B7954);

INCLUDE_ASM("asm/us/ovl_0b/nonmatchings/ovl_0b", func_800B7F20);

INCLUDE_ASM("asm/us/ovl_0b/nonmatchings/ovl_0b", func_800B81C4);

INCLUDE_ASM("asm/us/ovl_0b/nonmatchings/ovl_0b", func_800B8390);

INCLUDE_ASM("asm/us/ovl_0b/nonmatchings/ovl_0b", func_800B8664);

INCLUDE_ASM("asm/us/ovl_0b/nonmatchings/ovl_0b", func_800B87D0);

INCLUDE_ASM("asm/us/ovl_0b/nonmatchings/ovl_0b", func_800B8A24);

INCLUDE_ASM("asm/us/ovl_0b/nonmatchings/ovl_0b", func_800B8BE8);

INCLUDE_ASM("asm/us/ovl_0b/nonmatchings/ovl_0b", func_800B8D58);

INCLUDE_ASM("asm/us/ovl_0b/nonmatchings/ovl_0b", func_800B8E9C);

INCLUDE_ASM("asm/us/ovl_0b/nonmatchings/ovl_0b", func_800B8FF8);

INCLUDE_ASM("asm/us/ovl_0b/nonmatchings/ovl_0b", func_800B918C);

INCLUDE_ASM("asm/us/ovl_0b/nonmatchings/ovl_0b", func_800B9418);

s32 func_800B9550(BtlUnit *arg0, s16 arg1) {
    if (arg0 != NULL) {
        if (arg0->unkC4[1] & 4) {
            arg1 /= 2;
        }
        if (arg0->unk2A < arg1) {
            return 0;
        }
        arg0->unk2A -= arg1;
        return 1;
    }
    return 0;
}

void func_800B95BC(BtlCmd *arg0) {
    if (D_8007B720 >= D_8007B92D) {
        func_8004BF20();
        if (D_8007B720 >= D_8007B92D) {
            func_8004BF20();
        }
    }
    if ((arg0->unk4->unk7A >> 1) + arg0->unk4->unk33 < ((*D_8007B720 << 8) | *D_8007B720++) % 100) {
        arg0->unk10_9 = 1;
    }
}

INCLUDE_ASM("asm/us/ovl_0b/nonmatchings/ovl_0b", func_800B96AC);

void func_800B97A0(BtlUnit *arg0) {
    BtlUnit9C *m;
    arg0->unk28 = 0;
    arg0->unk68 |= 0x100000;
    if (!(arg0->unk68 & 1)) {
        func_800C2250(arg0, D_8006794C->unk1C->unk8E4.battle->unk488[(arg0->unk68 >> 1) & 0xF].unk0->unk30);
    }
    m = arg0->unk9C;
    if ((s32)m->unkC != arg0->unkA4[(arg0->unk68 >> 18) & 1] && (!(arg0->unk68 & 1) || (s32)m->unkC != arg0->unkA4[9]) && arg0 != D_8006794C->unk1C->unk8E4.battle->unk1088->unk4) {
        m->unkC = (BtlUnit9CC *)arg0->unkA4[(arg0->unk68 >> 18) & 1];
        arg0->unkA0->unk3A = 0;
    }
}

void func_800B989C(BtlCmd *arg0) {
    s32 r;
    if (!(arg0->unk4->unkC4[1] & 0x08000000)) {
        if (arg0->unk8->unkA * 2 < arg0->unk4->unk2A) {
            if (D_8007B720 >= D_8007B92D) {
                func_8004BF20();
            }
            r = *D_8007B720;
            D_8007B720++;
            r = r < 0xE6;
        } else {
            if (D_8007B720 >= D_8007B92D) {
                func_8004BF20();
            }
            r = *D_8007B720;
            D_8007B720++;
            r = r < 0xAA;
        }
        if (r) {
            arg0->unk10_10 = 1;
        }
    }
}

s32 func_800B9994(BtlCalc *arg0) {
    if (((arg0->unk18 & 0x100) && (D_8006794C->unk1C->unk8E4.battle->unk22CC[arg0->unk0->unk74[1]] & 0x42001103)) || (arg0->unk8->unk8[8] & 0x40) || ((arg0->unk8->unk10 >> 5) & 1) || (arg0->unk4->unk3C & 0x42009103) || arg0->unk0 == arg0->unk4) {
        return 0;
    }
    return 1;
}

void func_800B9A5C(BtlCalc *arg0) {
    BtlUnit *u;
    arg0->unk1B |= 1;
    u = arg0->unk4;
    arg0->unk1A |= 1;
    if ((u->unk3C & 0x40) || func_800C1E88(u, 0x10) != 0) {
        arg0->unk1B |= 2;
    } else {
        arg0->unk1A |= 2;
    }
}

void func_800B9AE4(BtlCalc *arg0) {
    arg0->unk16 = 100;
    arg0->unk12 = arg0->unk4->unk6D;
}

void func_800B9AFC(BtlCalc *arg0) {
    BtlUnit *c = arg0->unk0;
    arg0->unk16 = c->unk7A + (arg0->unk8->unk8[7] + (c->unk32 >> 2)) - arg0->unk4->unk7A;
    if (arg0->unk16 > 100) {
        arg0->unk16 = 100;
    } else if (arg0->unk16 <= 0) {
        arg0->unk16 = 1;
    }
    arg0->unk12 = arg0->unk4->unk6F;
}

s32 func_800B9B64(BtlCalc *arg0) {
    BtlUnit *t = arg0->unk4;
    if ((t->unk3C & 0x40) || func_800C1E88(t, 0x10) != 0) {
        BtlUnit *c = arg0->unk0;
        arg0->unk16 = c->unk7A + (arg0->unk8->unk8[7] + (c->unk32 >> 2)) - arg0->unk4->unk7A;
        if (arg0->unk16 > 100) {
            arg0->unk16 = 100;
        } else if (arg0->unk16 <= 0) {
            arg0->unk16 = 1;
        }
        arg0->unk12 = arg0->unk4->unk6F;
        return 0;
    }
    return 1;
}

u32 func_800B9C14(BtlCalc *arg0) {
    return *arg0->unk0->unkC4 & 0x200;
}

void func_800B9C30(BtlCalc *arg0) {
    if (arg0->unk4->unk3C & 0x400000) {
        arg0->unk16 /= 2;
    }
}

INCLUDE_ASM("asm/us/ovl_0b/nonmatchings/ovl_0b", func_800B9C74);

s32 func_800B9CDC(BtlCalc *arg0) {
    if (arg0->unk4->unk7A % arg0->unk8->unk8[7] != 0) {
        arg0->unk18 |= 1;
        return 0;
    }
    return 1;
}

s32 func_800B9D34(BtlCalc *arg0) {
    if (arg0->unk4->unk3C & 0x80) {
        arg0->unk18 |= 1;
        return 0;
    }
    return 1;
}

s32 func_800B9D6C(BtlCalc *arg0) {
    if (!(arg0->unk4->unk68 & 1)) {
        arg0->unk18 |= 1;
        return 0;
    }
    return 1;
}

s32 func_800B9DA4(BtlCalc *arg0) {
    if (arg0->unk4->unk3C & 0x101) {
        arg0->unk18 |= 1;
        return 0;
    }
    return 1;
}

s32 func_800B9DDC(BtlCalc *arg0) {
    u32 st = arg0->unk4->unk3C;
    if ((st & 1) || (st & 0x140) == 0x140) {
        arg0->unk18 |= 1;
        return 0;
    }
    return 1;
}

s32 func_800B9E20(BtlCalc *arg0) {
    if (arg0->unk4 == arg0->unk0) {
        arg0->unk18 |= 1;
        return 0;
    }
    return 1;
}

s32 func_800B9E50(BtlCalc *arg0) {
    if (arg0->unk4->unk3C & 0x80) {
        arg0->unk18 |= 0x40;
        return 0;
    }
    return 1;
}

void func_800B9E88(BtlCalc *arg0) {
    u8 r;
    s32 a;
    BtlUnit *c;
    if (D_8007B720 >= D_8007B92D) {
        func_8004BF20();
        if (D_8007B720 >= D_8007B92D) {
            func_8004BF20();
        }
    }
    r = *D_8007B720;
    c = arg0->unk0;
    a = c->unk31;
    arg0->unk14 = a + ((r << 8) | r) % (((c->unk7A + a) >> 2) + 1);
    D_8007B720++;
    arg0->unkE = arg0->unk8->unk8[5];
    arg0->unk10 = arg0->unk4->unk6C;
}

void func_800B9F80(BtlCalc *arg0) {
    u8 r;
    s32 a;
    BtlUnit *c;
    if (D_8007B720 >= D_8007B92D) {
        func_8004BF20();
        if (D_8007B720 >= D_8007B92D) {
            func_8004BF20();
        }
    }
    r = *D_8007B720;
    c = arg0->unk0;
    a = c->unk32;
    arg0->unk14 = a + ((r << 8) | r) % (((c->unk7A + a) >> 3) + 1);
    D_8007B720++;
    arg0->unkE = arg0->unk8->unk8[5];
    arg0->unk10 = arg0->unk4->unk6E;
}

void func_800BA078(BtlCalc *arg0) {
    arg0->unk14 = arg0->unk8->unk8[5];
}

void func_800BA094(BtlCalc *arg0) {
    s32 lo;
    if (D_8007B720 >= D_8007B92D) {
        func_8004BF20();
        if (D_8007B720 >= D_8007B92D) {
            func_8004BF20();
        }
    }
    lo = arg0->unk0->unk31;
    arg0->unk14 = lo + ((*D_8007B720 << 8) | *D_8007B720++) % (((arg0->unk0->unk7A + lo) >> 3) + 1);
    arg0->unkE = arg0->unk0->unk74[5] * arg0->unk8->unk8[5] / 10;
    arg0->unk10 = arg0->unk4->unk6C;
}

void func_800BA1B4(BtlCalc *arg0) {
    u8 r;
    s32 a;
    BtlUnit *c;
    if (D_8007B720 >= D_8007B92D) {
        func_8004BF20();
        if (D_8007B720 >= D_8007B92D) {
            func_8004BF20();
        }
    }
    r = *D_8007B720;
    c = arg0->unk0;
    a = c->unk31;
    arg0->unk14 = a + ((r << 8) | r) % (((c->unk7A + a) >> 2) + 1);
    D_8007B720++;
    arg0->unkE = arg0->unk8->unk8[5];
    arg0->unk10 = arg0->unk4->unk6C;
}

void func_800BA2AC(BtlCalc *arg0) {
    if (arg0->unk0->unk3C & 0x4800) {
        arg0->unk14 = arg0->unk14 * 3 >> 1;
    }
    if (arg0->unk0->unk3C & 0x10000000) {
        arg0->unk14 = 1;
    }
}

void func_800BA308(BtlCalc *arg0) {
    s32 r;
    if ((arg0->unk4->unkC4[0] & 0x04000000) && arg0->unk10 != 0) {
        if (D_8007B720 >= D_8007B92D) {
            func_8004BF20();
            if (D_8007B720 >= D_8007B92D) {
                func_8004BF20();
            }
        }
        r = *D_8007B720;
        D_8007B720++;
        arg0->unk10 = ((r << 8) | r) % (arg0->unk10 * 2);
    }
}

void func_800BA3FC(BtlCalc *arg0) {
    if (arg0->unk4->unk3C & 0x808000) {
        arg0->unk14 >>= 1;
    }
    if (arg0->unk4->unk3C & 0x10020000) {
        arg0->unk14 = arg0->unk14 * 3 >> 1;
    }
}

void func_800BA468(BtlCalc *arg0) {
    if (arg0->unk0->unk3C & 0x10000000) {
        arg0->unk14 /= 2;
    }
}

void func_800BA4AC(BtlCalc *arg0) {
    if (arg0->unk4->unk3C & 0x400000) {
        arg0->unk14 /= 2;
    }
}

void func_800BA4F0(BtlCalc *arg0) {
    if (arg0->unk0->unkC4[1] & 2) {
        arg0->unk14 = arg0->unk14 * 3 >> 1;
    }
}

INCLUDE_ASM("asm/us/ovl_0b/nonmatchings/ovl_0b", func_800BA534);

void func_800BA660(BtlCalc *arg0) {
    BtlUnit *u = arg0->unk0;
    if (u->unk79 & u->unk74[6]) {
        arg0->unk14 = arg0->unk14 * 3 >> 1;
    }
}

INCLUDE_ASM("asm/us/ovl_0b/nonmatchings/ovl_0b", func_800BA6A4);

s32 func_800BA7D0(BtlCalc *arg0) {
    if ((func_800C09A4(arg0->unk4, 0x200000) & 0xFF) || func_800C1E88(arg0->unk4, 0x80) != 0) {
        if (arg0->unk8->unk8[6] & 8) {
            arg0->unk18 |= 1;
            return 0;
        }
        return 1;
    }
    return 1;
}

void func_800BA858(BtlCalc *arg0) {
    BtlCalc8 *c = arg0->unk8;
    if ((c->unk10 & 3) == 1 && (u32)((*(u32 *)c->unk8 & 0xF) - 3) < 3 && func_800C1F00(c->unkC < 0x10) >= 2) {
        arg0->unk14 /= 2;
    }
}

void func_800BA8F0(BtlCalc *arg0) {
    if (arg0->unk0->unk79 & arg0->unk8->unk8[6]) {
        arg0->unk14 = arg0->unk14 * 3 >> 1;
    }
}

void func_800BA938(Unk800BA938 *arg0) {
    if ((D_8006794C->unk1C->unk8E4.battle->unk132C >> 10) & 1) {
        arg0->unkE = 0x3C - arg0->unkE;
        if (arg0->unkE <= 0) {
            arg0->unkE = 1;
        }
    }
}

void func_800BA994(BtlCalc *arg0) {
    s32 v = arg0->unkE * arg0->unk14;
    BtlUnit *u;
    if (v >= 10000) {
        v = 9999;
    }
    u = arg0->unk4;
    arg0->unk1B |= 1;
    if (!(u->unk3C & 0x40) && func_800C1E88(u, 0x10) == 0) {
        arg0->unk1B |= 2;
    }
    arg0->unk1E = v;
}

void func_800BAA28(BtlCalc *arg0) {
    s32 a = arg0->unkE - arg0->unk10;
    s32 v;
    if (a <= 0) {
        a = 1;
    }
    arg0->unk1B |= 1;
    if (arg0->unk14 <= 0) {
        arg0->unk14 = 1;
    }
    v = a * arg0->unk14;
    if ((arg0->unk8->unk10 >> 10) & 1) {
        v = v * 2 / 3;
    }
    if (v >= 10000) {
        v = 9999;
    }
    if (arg0->unk18 & 8) {
        arg0->unk1B |= 2;
    }
    arg0->unk1E = v;
}

void func_800BAAE4(BtlCalc *arg0) {
    s32 v;
    u8 f;
    if (arg0->unk14 > 100) {
        arg0->unk14 = 100;
    }
    v = arg0->unk4->unk24->unk0 * arg0->unk14 / 100;
    if ((arg0->unk8->unk10 >> 10) & 1) {
        v = v * 2 / 3;
    }
    if (v >= 10000) {
        v = 9999;
    }
    f = arg0->unk1B;
    arg0->unk1B = f | 1;
    if (arg0->unk18 & 8) {
        arg0->unk1B = f | 3;
    }
    arg0->unk1E = v;
}

void func_800BABAC(BtlCalc *arg0) {
    s32 r;
    s32 v;
    if (D_8007B720 >= D_8007B92D) {
        func_8004BF20();
        if (D_8007B720 >= D_8007B92D) {
            func_8004BF20();
        }
    }
    r = *D_8007B720;
    v = arg0->unk8->unk8[7];
    D_8007B720++;
    if (((r << 8) | r) % 100 < v) {
        func_800C0ACC(arg0->unk4, D_8006794C->unk1C->unk8E4.battle->unk22CC[arg0->unk8->unk8[9]]);
    }
}

void func_800BACB4(BtlCalc *arg0) {
    BtlCalc8 *c = arg0->unk8;
    u32 mask = D_8006794C->unk1C->unk8E4.battle->unk22CC[c->unk8[9]];
    u32 r;
    if (!((c->unk10 >> 10) & 1) && c->unkE == 0x14) {
        mask |= 0x800000;
    }
    r = func_800C0ACC(arg0->unk4, mask);
    if (r == 0) {
        arg0->unk18 |= 0x40;
    } else if (r == 1) {
        arg0->unk18 |= 1;
    }
}

void func_800BAD64(BtlCalc *arg0) {
    if (func_800C0B6C(arg0->unk4, D_8006794C->unk1C->unk8E4.battle->unk22CC[arg0->unk8->unk8[9]]) != 2) {
        arg0->unk18 |= 1;
    }
}

void func_800BADD4(BtlCalc *arg0) {
    if (func_800C0B6C(arg0->unk4, D_8006B1F4[arg0->unk8->unkF - 0xE0].unk8) != 2) {
        arg0->unk18 |= 1;
    }
}

void func_800BAE44(BtlCalc *arg0) {
    if (arg0->unk18 & 4) {
        BtlUnit *u = arg0->unk0;
        s32 v = u->unk24->unk2 >> 3;
        if (v == 0 || u->unk2A < v) {
            u->unk2A = 0;
        } else {
            u->unk2A -= v;
        }
    }
}

s32 func_800BAEA8(void) {
    Battle *b = D_8006794C->unk1C->unk8E4.battle;
    BtlUnit *u;
    if ((u8)b->unk132C >> 7 == 0) {
        return 0;
    }
    for (u = b->unk6F8; u != NULL; u = u->unk0) {
        if ((u->unk68 & 1) && !(u->unk3C & 0x42021143)) {
            return 1;
        }
    }
    return 0;
}

void func_800BAF3C(BtlCalc *arg0) {
    BtlUnit *u = arg0->unk4;
    if (u->unk3C & 0x20) {
        u->unk7E |= 0x100;
    }
}

void func_800BAF70(BtlCalc *arg0) {
    s16 id = arg0->unk8->unk8[7];
    if (func_80027F34(id) == 0) {
        func_800CD5C8(10);
    } else {
        func_800EA164(id);
        func_800CD5C8(0x1F, func_800CD75C(id));
    }
}

INCLUDE_ASM("asm/us/ovl_0b/nonmatchings/ovl_0b", func_800BAFEC);

INCLUDE_ASM("asm/us/ovl_0b/nonmatchings/ovl_0b", func_800BB1F4);

INCLUDE_ASM("asm/us/ovl_0b/nonmatchings/ovl_0b", func_800BB4A0);

INCLUDE_ASM("asm/us/ovl_0b/nonmatchings/ovl_0b", func_800BB974);

INCLUDE_ASM("asm/us/ovl_0b/nonmatchings/ovl_0b", func_800BBD3C);

void func_800BBF30(Unk800BBF30 *arg0, s16 arg1) {
    arg0->unk34 = arg1;
    arg0->unk3B = func_80021698();
}

INCLUDE_ASM("asm/us/ovl_0b/nonmatchings/ovl_0b", func_800BBF5C);

INCLUDE_ASM("asm/us/ovl_0b/nonmatchings/ovl_0b", func_800BC0D0);

void func_800BC1E0(Unk800BC1E0 *arg0, s16 *arg1) {
    if (!(arg0->unk0 & 8)) {
        arg1[0] = 0;
        arg1[1] = arg0->unk8;
    } else {
        arg1[0] = arg0->unk8;
        arg1[1] = 0;
    }
    arg1[2] = arg0->unkA;
}

INCLUDE_ASM("asm/us/ovl_0b/nonmatchings/ovl_0b", func_800BC21C);

INCLUDE_ASM("asm/us/ovl_0b/nonmatchings/ovl_0b", func_800BC78C);

INCLUDE_ASM("asm/us/ovl_0b/nonmatchings/ovl_0b", func_800BCAA4);

INCLUDE_ASM("asm/us/ovl_0b/nonmatchings/ovl_0b", func_800BCC64);

INCLUDE_ASM("asm/us/ovl_0b/nonmatchings/ovl_0b", func_800BCDBC);

INCLUDE_ASM("asm/us/ovl_0b/nonmatchings/ovl_0b", func_800BCEF0);

INCLUDE_ASM("asm/us/ovl_0b/nonmatchings/ovl_0b", func_800BD31C);

void func_800BD414(BtlUnit *arg0, u16 arg1) {
    s32 i;
    for (i = 0; i < 16; i++) {
        if ((arg1 >> i) & 1) {
            arg0->unk9C->unk14 |= 1 << i;
        }
    }
}

void func_800BD464(BtlUnit *arg0, u16 arg1) {
    s32 i;
    if (!(arg0->unk68 & 1)) {
        arg0->unk9C->unk0 &= ~0x20;
    }
    for (i = 0; i < 16; i++) {
        if ((arg1 >> i) & 1) {
            arg0->unk9C->unk14 &= ~(1 << i);
        }
    }
}

void func_800BD4DC(BtlUnit *arg0) {
    s32 i;
    for (i = 0; i < arg0->unkA8->unk8->unk3; i++) {
        arg0->unkA8->unk14 |= 1 << i;
    }
}

void func_800BD53C(BtlUnit *arg0) {
    s32 i;
    for (i = 0; i < arg0->unkA8->unk8->unk3; i++) {
        arg0->unkA8->unk14 &= ~(1 << i);
    }
}

s32 func_800BD5A0(BtlUnit *arg0) {
    arg0->unk9C->unkC = (BtlUnit9CC *)arg0->unkA4[4];
    arg0->unk9C->unk50[2].unk4 &= ~3;
    arg0->unk9C->unk50[0].unk4 |= 3;
    arg0->unk9C->unk50[0].unkC = 0;
    arg0->unk9C->unk50[0].unk12 = 0x1000;
    arg0->unkA0->unk3A = 0;
    if (arg0->unk3C & 0x2000) {
        arg0->unk7D = 7;
        func_800B1250(arg0->unk18, 0x3D, 0, arg0->unkB0, 0);
        func_800BF558(arg0, 0x2000);
        return 0;
    }
    arg0->unk7D = 6;
    return 1;
}

void func_800BD698(BtlUnit *arg0, s32 arg1, s32 arg2) {
    u32 i;
    for (i = 0; i < 0x22; i++) {
        D_8006794C->unk1C->unk8E4.battle->unk8[arg2][i] = (s32)func_8001C7FC(4, D_800F524C[arg1][i] | 0x30000, 0);
    }
    arg0->unkA4 = D_8006794C->unk1C->unk8E4.battle->unk8[arg2];
}

INCLUDE_ASM("asm/us/ovl_0b/nonmatchings/ovl_0b", func_800BD774);

void func_800BD8C0(Unk800BD8C0 *arg0, Unk800BD8C0 *arg1) {
    arg0->unk0 = arg1->unk0;
    arg0->unk2 = arg1->unk2;
    arg0->unk4 = arg1->unk4;
    arg0->unk7 = arg1->unk7;
}

void func_800BD8F0(Unk800BD8F0 *arg0) {
    s32 v;
    arg0->unk2E = 10;
    v = (D_8006794C->unk24 >> 8) & 7;
    if (v == 0) {
        arg0->unk2E = 8;
    } else if (v == 2) {
        arg0->unk2E = 14;
    }
}

void func_800BD93C(BtlUnit *arg0) {
    if (arg0->unk24->unk0 < arg0->unk28 * 6) {
        func_800BF558(arg0, 0x200);
        if (arg0->unk24->unk0 < arg0->unk28) {
            arg0->unk28 = arg0->unk24->unk0;
        }
        goto clamp;
    }
    if (arg0->unk28 == 0) {
        if (arg0->unk7D == 0) {
            func_800BEDBC(arg0, 0x100);
        }
    } else {
        if (!(arg0->unk3C & 0x200)) {
            func_800BEDBC(arg0, 0x200);
        }
    clamp:
        arg0->unk2A = arg0->unk24->unk2 < arg0->unk2A ? arg0->unk24->unk2 : arg0->unk2A;
        if (arg0->unk68 & 1) {
            arg0->unk68 = (arg0->unk68 & ~0x40000) | ((func_800C09A4(arg0, 0x30202) & 1) << 18);
        }
    }
}

INCLUDE_ASM("asm/us/ovl_0b/nonmatchings/ovl_0b", func_800BDA60);

void func_800BDACC(u8 *arg0, u8 *arg1) {
    arg0[0] |= arg1[0];
    arg0[1] |= arg1[1];
    arg0[2] |= arg1[2];
    arg0[3] |= arg1[3];
}

void func_800BDB20(BtlUnit *arg0, s32 arg1, u8 arg2) {
    s32 v;
    if (arg0->unk3C & 0x100) {
        arg0->unk7E = 0x20;
        return;
    }
    if (arg0->unk3C & 1) {
        arg0->unk80 = 0;
        return;
    }
    if (arg0 != D_8006794C->unk1C->unk8E4.battle->unk1088->unk4) {
        v = (arg0->unk68 & 1) << 11;
        arg0->unkA0->unk22 = v;
        arg0->unk86[1] = v;
    }
    if (arg1 < arg0->unk28) {
        arg0->unk28 -= arg1;
    } else {
        arg0->unk28 = 0;
    }
    arg0->unk80 = arg1;
    if (arg2) {
        func_800BCAA4(arg0);
        return;
    }
    if (arg0->unk28 == 0) {
        func_800B97A0(arg0);
    }
}

void func_800BDC04(BtlUnit *arg0, s32 arg1) {
    if (arg0->unk3C & 0x100) {
        arg0->unk7E = 0x20;
    } else if (arg0->unk3C & 1) {
        arg1 = 0;
    } else {
        BtlUnit24 *m = arg0->unk24;
        s32 hp = arg0->unk28 + arg1;
        if (hp < m->unk0) {
            arg0->unk28 = hp;
        } else {
            arg0->unk28 = m->unk0;
        }
    }
    arg0->unk80 = arg1;
}

void func_800BDC64(BtlUnit *arg0, s32 arg1) {
    if (arg0->unk3C & 0x100) {
        arg0->unk7E = 0x20;
    } else if (arg0->unk3C & 1) {
        arg1 = 0;
    } else if (arg1 < arg0->unk2A) {
        arg0->unk2A -= arg1;
    } else {
        arg0->unk2A = 0;
    }
    arg0->unk82 = arg1;
}

void func_800BDCBC(BtlUnit *arg0, s32 arg1) {
    if (arg0->unk3C & 0x100) {
        arg0->unk7E = 0x20;
    } else if (arg0->unk3C & 1) {
        arg1 = 0;
    } else if (arg0->unk2A + arg1 < arg0->unk24->unk2) {
        arg0->unk2A += arg1;
    } else {
        arg0->unk2A = arg0->unk24->unk2;
    }
    arg0->unk82 = arg1;
}

void func_800BDD24(BtlUnit *arg0) {
    s32 v = 0;
    if (!(arg0->unk3C & 1)) {
        v = arg0->unk24->unk0 >> 4;
        if (v < arg0->unk28) {
            arg0->unk28 -= v;
        } else {
            func_800B97A0(arg0);
        }
    }
    arg0->unkCA = v;
    arg0->unkCE |= 2;
}

void func_800BDDA0(BtlUnit *arg0) {
    s32 v = 0;
    if (!(arg0->unk3C & 1)) {
        v = arg0->unk24->unk0 >> 4;
        if ((arg0->unk3C & 0x40) || func_800C1E88(arg0, 0x10) != 0) {
            arg0->unkCE |= 8;
            if (v < arg0->unk28) {
                arg0->unk28 -= v;
            } else {
                func_800B97A0(arg0);
            }
        } else {
            if (arg0->unk28 + v < arg0->unk24->unk0) {
                arg0->unk28 += v;
            } else {
                arg0->unk28 = arg0->unk24->unk0;
            }
        }
    }
    arg0->unkC8 = v;
    arg0->unkCE |= 1;
}

void func_800BDE78(BtlUnit *arg0) {
    s32 v = 0;
    if (!(arg0->unk3C & 1)) {
        v = arg0->unk24->unk2 >> 4;
        if (v < arg0->unk2A) {
            arg0->unk2A -= v;
        } else {
            arg0->unk2A = 0;
        }
    }
    arg0->unkCC = v;
    arg0->unkCE |= 4;
}

INCLUDE_ASM("asm/us/ovl_0b/nonmatchings/ovl_0b", func_800BDED4);

INCLUDE_ASM("asm/us/ovl_0b/nonmatchings/ovl_0b", func_800BE1E0);

INCLUDE_ASM("asm/us/ovl_0b/nonmatchings/ovl_0b", func_800BE784);

INCLUDE_ASM("asm/us/ovl_0b/nonmatchings/ovl_0b", func_800BE99C);

INCLUDE_ASM("asm/us/ovl_0b/nonmatchings/ovl_0b", func_800BEC64);

BtlUnit *func_800BED10(u16 arg0) {
    BtlUnit *u;
    for (u = D_8006794C->unk1C->unk8E4.battle->unk6F8; u != NULL; u = u->unk0) {
        if (u->unkB0 == arg0) {
            return u;
        }
    }
    return NULL;
}

BtlUnit *func_800BED6C(u16 arg0) {
    s32 i;
    Battle *b = D_8006794C->unk1C->unk8E4.battle;
    for (i = 0; i < 8; i++) {
        if (b->unk6FC[i].unkB0 == arg0) {
            return &b->unk6FC[i];
        }
    }
    return NULL;
}

INCLUDE_ASM("asm/us/ovl_0b/nonmatchings/ovl_0b", func_800BEDBC);

INCLUDE_ASM("asm/us/ovl_0b/nonmatchings/ovl_0b", func_800BF558);

INCLUDE_ASM("asm/us/ovl_0b/nonmatchings/ovl_0b", func_800BFAF4);

INCLUDE_ASM("asm/us/ovl_0b/nonmatchings/ovl_0b", func_800BFF20);

INCLUDE_ASM("asm/us/ovl_0b/nonmatchings/ovl_0b", func_800C02D4);

void func_800C0430(BtlUnit *arg0, s32 arg1) {
    s8 d;
    s8 i;
    d = 6;
    if (arg0->unk68 & 1) {
        d = -12;
    }
    if (arg1 & 0xFF) {
        d *= -1;
    }
    i = 0;
    if (arg0->unk68 & 1) {
        arg0->unkA8->unk18->unk2 = ((d + func_8001DA6C(arg0->unkA8->unk18->unk2)) << 6) | 0x10;
    }
    while (i < func_80063830(arg0->unk84 | 0x20000)) {
        arg0->unk9C->unk18[i].unk2 = ((d + func_8001DA6C(arg0->unk9C->unk18[i].unk2)) << 6) | 0x10;
        i++;
    }
}

INCLUDE_ASM("asm/us/ovl_0b/nonmatchings/ovl_0b", func_800C0544);

void func_800C072C(void) {
    u16 *p = (u16 *)D_800FE360;
    s32 i;
    s8 c[3];
    s8 v;
    for (i = 0; i < 0x600; i++) {
        c[0] = *p & 0x1F;
        c[1] = (*p >> 5) & 0x1F;
        c[2] = (*p >> 10) & 0x1F;
        if (c[0] == 0 && c[1] == 0 && c[2] == 0) {
            v = 0;
        } else {
            v = (c[0] + c[1] + c[2]) / 3;
            if (v <= 0) {
                v = 1;
            }
        }
        *p = v + ((v << 10) + (v << 5));
        p++;
    }
    D_800FDF50.x = 0x100;
    D_800FDF50.y = 0xDA;
    D_800FDF50.h = 6;
    D_800FDF50.w = 0x100;
    LoadImage(&D_800FDF50, D_800FE360);
}

INCLUDE_ASM("asm/us/ovl_0b/nonmatchings/ovl_0b", func_800C0840);

s32 func_800C09A4(BtlUnit *arg0, u32 arg1) {
    s32 ret = 0;
    if ((arg0->unk38 & arg1) || (arg0->unk3C & arg1)) {
        ret = 1;
    }
    return ret;
}

void func_800C09D8(Unk800C09D8 *arg0, Unk800C09D8 *arg1) {
    arg0->unk38 = arg1->unk3C & 0x7F;
}

void func_800C09EC(BtlUnit *arg0) {
    u32 i;
    s16 *p;
    arg0->unk3C = 0;
    arg0->unk38 = 0;
    arg0->unk34 = 0;
    p = arg0->unk40;
    for (i = 0; i < 3; i++) {
        p[i] = 0;
    }
    for (i = 0; i < 14; i++) {
        p[i + 3] = 0;
    }
}

void func_800C0A3C(Unk800C09D8 *arg0) {
    s32 t = (0x3C - arg0->unk33) * 8;
    arg0->unk66 = t * D_8006794C->unk1C->unk8E4.battle->unk1692;
}

void func_800C0A70(BtlUnit *arg0, s32 arg1) {
    if (func_800B4040(arg0) & 0xFF) {
        arg0->unk2C = 0;
        arg0->unk68 &= ~0x10000;
        if (!(arg1 & 0x100)) {
            arg0->unkB3 = 0;
        }
    }
}

u32 func_800C0ACC(BtlUnit *arg0, u32 arg1) {
    u32 ret = 0;
    u32 i;
    for (i = 0; i < 32; i++) {
        if (arg1 & (1 << i)) {
            u32 r = func_800BEDBC(arg0, 1 << i);
            if ((ret == 0 && r != 0) || (ret == 1 && r >= 2)) {
                ret = r;
            }
        }
    }
    return ret;
}

s32 func_800C0B6C(BtlUnit *arg0, u32 arg1) {
    s32 ret = 1;
    u32 i;
    for (i = 0; i < 32; i++) {
        if ((arg1 & (1 << i)) && func_800BF558(arg0, 1 << i) == 2) {
            ret = 2;
        }
    }
    return ret;
}

INCLUDE_ASM("asm/us/ovl_0b/nonmatchings/ovl_0b", func_800C0C00);

void func_800C0C7C(u32 *arg0) {
    D_800FDF50.x = 0x100;
    D_800FDF50.y = 0xE0;
    D_800FDF50.w = 0x100;
    D_800FDF50.h = 6;
    StoreImage(&D_800FDF50, arg0);
    D_800FE360 = arg0;
}

void func_800C0CD0(u32 *arg0) {
    D_800FDF50.x = 0x100;
    D_800FDF50.y = 0xE6;
    D_800FDF50.w = 0x100;
    D_800FDF50.h = 0xC;
    StoreImage(&D_800FDF50, arg0);
    D_800FE360 = arg0;
}

void func_800C0D24(BtlUnit *arg0) {
    if (arg0->unk3C & 1) {
        func_800C0430(arg0, 1);
    } else if (arg0->unk3C & 0x03000840) {
        func_800C02D4(arg0);
    }
    func_800C2344(arg0->unk9C);
    if (arg0->unk68 & 1) {
        func_800C2344(arg0->unkA8);
    }
}

INCLUDE_ASM("asm/us/ovl_0b/nonmatchings/ovl_0b", func_800C0DAC);

INCLUDE_ASM("asm/us/ovl_0b/nonmatchings/ovl_0b", func_800C0EC8);

INCLUDE_ASM("asm/us/ovl_0b/nonmatchings/ovl_0b", func_800C1184);

INCLUDE_ASM("asm/us/ovl_0b/nonmatchings/ovl_0b", func_800C12CC);

void func_800C1524(void) {
    Battle *b = D_8006794C->unk1C->unk8E4.battle;
    b->unk6F8 = NULL;
    b->unkD7C = 0;
    b->unkD80 = 0;
    b->unkD81 = 0;
    b->unkD82 = 0;
    b->unkD83 = 0;
    b->unkD84 = 0x20;
    b->unkD85 = 0;
    b->unkD86 = 0;
    b->unkD87 = 0;
    b->unk11BB = 0;
    b->unk11BC = 0;
    b->unk11BE = 0;
    D_800FDF3A = 0;
    func_800C1594();
}

INCLUDE_ASM("asm/us/ovl_0b/nonmatchings/ovl_0b", func_800C1594);

void func_800C15DC(BtlUnit *arg0) {
    Battle *b = D_8006794C->unk1C->unk8E4.battle;
    BtlUnit *u;
    BtlUnit *six = 0;
    u32 st;
    for (u = b->unk6F8; u != 0; u = u->unk0) {
        if (((u->unk68 >> 1) & 0xF) == 6) {
            six = u;
        }
        if ((u->unk68 & 1) == (arg0->unk68 & 1)) {
            st = u->unk3C;
            if (!(st & 0x1103)) {
                return;
            }
            if ((u->unk28 == 0 || (st & 0x100)) && (st & 0x2000)) {
                return;
            }
            if (func_800B3EF8(u, 0x3D)) {
                return;
            }
        }
    }
    if (six == 0 || !func_800B3EF8(six, 0x3A)) {
        func_800E9F68(0);
        func_800B2DD4(b);
        b->unkD83 = 0;
    }
}

void func_800C1704(void) {
    Battle *b = D_8006794C->unk1C->unk8E4.battle;
    BtlUnit *u;
    for (u = b->unk6F8; u != NULL; u = u->unk0) {
        if (!(u->unk68 & 1) && !(u->unk3C & 0x100)) {
            return;
        }
    }
    func_800E9F68(0);
    func_800B2DD4(b);
    b->unkD83 = 0;
}

s32 func_800C1794(Battle *arg0) {
    GameData *gd = D_8006794C->unk1C;
    u8 v = gd->unk56E;
    if ((v == 3 || v == 6) && !((arg0->unk132C >> 4) & 1)) {
        gd->unk56E = 6;
    } else {
        BtlUnit *u = D_8006794C->unk1C->unk8E4.battle->unk6F8;
        while (u != NULL) {
            if (u->unk68 & 1) {
                func_800C1854(u, 0);
            }
            u = u->unk0;
        }
    }
    return 1;
}

void func_800C1854(BtlUnit *arg0, s32 arg1) {
    Character *c = &D_8006794C->unk1C->unk3C[(arg0->unk68 >> 1) & 0xF];
    if (arg0->unk3C & 0x4000) {
        c->unk20 = 0;
    } else {
        c->unk20 = arg0->unk78;
    }
    func_800BD8C0((Unk800BD8C0 *)&c->unk10, (Unk800BD8C0 *)&arg0->unk28);
    if ((arg0->unkC4[1] & 0x8000) && arg1 == 0) {
        c->unk38 = 0;
        return;
    }
    if (arg0->unk28 == 0) {
        func_800BEDBC(arg0, 0x100);
    }
    func_800C09D8((Unk800C09D8 *)c, (Unk800C09D8 *)arg0);
}

void func_800C1938(BtlUnit *arg0) {
    BtlUnit *p;
    for (p = (BtlUnit *)&D_8006794C->unk1C->unk8E4.battle->unk6F8; p != NULL; p = p->unk0) {
        if (p->unk0 == arg0) {
            p->unk0 = arg0->unk0;
            return;
        }
    }
}

INCLUDE_ASM("asm/us/ovl_0b/nonmatchings/ovl_0b", func_800C1990);

void func_800C19F8(u8 arg0) {
    Battle *b = D_8006794C->unk1C->unk8E4.battle;
    if (!(b->unk11B8 & 1)) {
        if ((u8)b->unk132C >> 7 == 0) {
            func_800CD5C8(4);
        } else if (arg0 && (func_800EA038() & 0xFF)) {
            func_800B44C0(0, 0, 0, 0x38);
        }
    }
}

INCLUDE_ASM("asm/us/ovl_0b/nonmatchings/ovl_0b", func_800C1A8C);

u16 func_800C1CA0(u32 arg0, s32 arg1) {
    BtlUnit *u;
    s32 r = 0;
    s32 all;
    for (u = D_8006794C->unk1C->unk8E4.battle->unk6F8; u != 0; u = u->unk0) {
        all = arg1 == 0;
        if (all || (u8)func_800C09A4(u, arg1)) {
            if ((u->unk68 >> 28) & 1) {
                switch (arg0) {
                case 0:
                    if (!(u->unk68 & 1)) {
                        break;
                    }
                    r |= u->unkB0;
                    break;
                case 1:
                    if (u->unk68 & 1) {
                        break;
                    }
                case 2:
                    r |= u->unkB0;
                    break;
                }
            }
        }
    }
    return r;
}

BtlUnit *func_800C1D8C(s32 arg0) {
    BtlUnit *u;
    for (u = D_8006794C->unk1C->unk8E4.battle->unk6F8; u != NULL; u = u->unk0) {
        if ((u8)u->unk68 >> 5 == arg0) {
            return u;
        }
    }
    return NULL;
}

INCLUDE_ASM("asm/us/ovl_0b/nonmatchings/ovl_0b", func_800C1DEC);

s32 func_800C1E88(BtlUnit *arg0, s32 arg1) {
    u32 f = arg0->unk68;
    if (f & 1) {
        return 0;
    }
    return D_8006794C->unk1C->unk8E4.battle->unk488[(f >> 1) & 0xF].unk0->unk2 & arg1;
}

s32 func_800C1EDC(BtlUnit *arg0) {
    u32 f = arg0->unk68;
    s32 v = (f >> 1) & 0xF;
    if (!(f & 1)) {
        v += 9;
    }
    return v;
}

u32 func_800C1F00(u32 arg0) {
    BtlUnit *u = D_8006794C->unk1C->unk8E4.battle->unk6F8;
    s32 n = 0;
    while (u != NULL) {
        u32 f = u->unk68;
        if ((f & 1) == arg0 && ((f >> 28) & 1) && !(u->unk3C & 0x100)) {
            n++;
        }
        u = u->unk0;
    }
    return n;
}

INCLUDE_ASM("asm/us/ovl_0b/nonmatchings/ovl_0b", func_800C1F7C);

INCLUDE_ASM("asm/us/ovl_0b/nonmatchings/ovl_0b", func_800C20B8);

void func_800C2200(BtlUnit *arg0, BtlUnit *arg1) {
    if (!(arg1->unk7E & 0x20)) {
        func_800C22CC(arg0, (u16)func_800AE47C((u8)arg0->unk68 >> 5, 1), 0x7F);
    }
}

void func_800C2250(BtlUnit *arg0, u16 arg1) {
    Battle *b = D_8006794C->unk1C->unk8E4.battle;
    if (b->unk11BE == 0 && !((arg0->unk68 >> 27) & 1)) {
        b->unk11BE = 8;
        func_800C22CC(arg0, arg1, 0x7F);
        arg0->unk68 |= 0x08000000;
    }
}

void func_800C22CC(BtlUnit *arg0, u16 arg1, u8 arg2) {
    SVECTOR v;
    if (arg1 != 0xFFFF) {
        v.vx = arg0->unk8C[0] >> 16;
        v.vy = arg0->unk8C[1] >> 16;
        v.vz = arg0->unk8C[2] >> 16;
        func_800AD90C(0xD000, arg1, 0, func_800E976C((s32)&v), arg2);
    }
}

void func_800C2344(BtlUnit9C *arg0) {
    s32 i;
    Unk800CEE74 *p = func_800CEE74();
    for (i = 0; i < arg0->unk8->unk3; i++) {
        func_800C8D18(arg0, i & 0xFF, p->unkC, p->unkD, p->unkE);
    }
}

INCLUDE_ASM("asm/us/ovl_0b/nonmatchings/ovl_0b", func_800C23C8);

void func_800C24E8(BtlUnit *arg0, s32 arg1) {
    Unk800CEE74 *p = func_800CEE74();
    func_800C8E80(arg0->unk9C, 0x60);
    func_800C8A3C(arg0->unk9C, (p->unkC * arg1) << 11 >> 16, (p->unkD * arg1) << 11 >> 16, (p->unkE * arg1) << 11 >> 16);
    if (arg1 == 0) {
        arg0->unk68 |= 0x400000;
    }
}

INCLUDE_ASM("asm/us/ovl_0b/nonmatchings/ovl_0b", func_800C2598);

INCLUDE_RODATA("asm/us/ovl_0b/nonmatchings/ovl_0b", D_800A7948);

INCLUDE_ASM("asm/us/ovl_0b/nonmatchings/ovl_0b", func_800C28F4);

u8 func_800C2DF4(void) {
    return D_800FE470;
}

INCLUDE_ASM("asm/us/ovl_0b/nonmatchings/ovl_0b", func_800C2E04);

INCLUDE_ASM("asm/us/ovl_0b/nonmatchings/ovl_0b", func_800C2E4C);

INCLUDE_ASM("asm/us/ovl_0b/nonmatchings/ovl_0b", func_800C2F1C);

INCLUDE_ASM("asm/us/ovl_0b/nonmatchings/ovl_0b", func_800C2F9C);

INCLUDE_ASM("asm/us/ovl_0b/nonmatchings/ovl_0b", func_800C2FFC);

void func_800C30B8(void) {
    BtlUnit *u = D_800F78A4->unkC->unk0;
    if (u->unk68 & 1) {
        D_800FE484 = u;
    }
}

INCLUDE_ASM("asm/us/ovl_0b/nonmatchings/ovl_0b", func_800C30F4);

INCLUDE_ASM("asm/us/ovl_0b/nonmatchings/ovl_0b", func_800C316C);

INCLUDE_ASM("asm/us/ovl_0b/nonmatchings/ovl_0b", func_800C36D0);

INCLUDE_ASM("asm/us/ovl_0b/nonmatchings/ovl_0b", func_800C3770);

INCLUDE_ASM("asm/us/ovl_0b/nonmatchings/ovl_0b", func_800C37BC);

INCLUDE_ASM("asm/us/ovl_0b/nonmatchings/ovl_0b", func_800C3F78);

void func_800C412C(Unk800C412C *arg0, u8 *arg1) {
    if (arg0->unk1B == 0) {
        arg0->unk1C = arg0->unk1D = arg1[0];
        arg0->unk1A = arg1[2];
        arg0->unk19 = arg1[3];
    }
}

INCLUDE_ASM("asm/us/ovl_0b/nonmatchings/ovl_0b", func_800C416C);

INCLUDE_ASM("asm/us/ovl_0b/nonmatchings/ovl_0b", func_800C493C);

INCLUDE_ASM("asm/us/ovl_0b/nonmatchings/ovl_0b", func_800C4A3C);

INCLUDE_ASM("asm/us/ovl_0b/nonmatchings/ovl_0b", func_800C4B9C);

INCLUDE_ASM("asm/us/ovl_0b/nonmatchings/ovl_0b", func_800C4DC4);

INCLUDE_ASM("asm/us/ovl_0b/nonmatchings/ovl_0b", func_800C4E84);

INCLUDE_ASM("asm/us/ovl_0b/nonmatchings/ovl_0b", func_800C52B4);

INCLUDE_ASM("asm/us/ovl_0b/nonmatchings/ovl_0b", func_800C5530);

INCLUDE_ASM("asm/us/ovl_0b/nonmatchings/ovl_0b", func_800C57A4);

s32 func_800C5890(s32 arg0) {
    s32 ret = 0;
    if (arg0 == 5 || arg0 == 9) {
        ret = D_800FE36C;
    }
    if ((u32)(arg0 - 0xD) < 2) {
        ret = D_8006794C->unk1C->unk8E4.battle->unk1310;
    }
    return ret;
}

INCLUDE_ASM("asm/us/ovl_0b/nonmatchings/ovl_0b", func_800C58E4);

INCLUDE_ASM("asm/us/ovl_0b/nonmatchings/ovl_0b", func_800C6528);

INCLUDE_ASM("asm/us/ovl_0b/nonmatchings/ovl_0b", func_800C67BC);

INCLUDE_ASM("asm/us/ovl_0b/nonmatchings/ovl_0b", func_800C7730);

INCLUDE_ASM("asm/us/ovl_0b/nonmatchings/ovl_0b", func_800C7B84);

INCLUDE_ASM("asm/us/ovl_0b/nonmatchings/ovl_0b", func_800C7CC8);

INCLUDE_ASM("asm/us/ovl_0b/nonmatchings/ovl_0b", func_800C7DAC);

INCLUDE_ASM("asm/us/ovl_0b/nonmatchings/ovl_0b", func_800C86C8);

INCLUDE_ASM("asm/us/ovl_0b/nonmatchings/ovl_0b", func_800C8A3C);

INCLUDE_ASM("asm/us/ovl_0b/nonmatchings/ovl_0b", func_800C8BE0);

void func_800C8D18(BtlUnit9C *arg0, u8 arg1, s16 arg2, s16 arg3, s16 arg4) {
    BtlMesh *g = &arg0->unk8->unk10[arg1];
    u32 *p = g->unk1C;
    u32 v = (arg4 << 16) | (arg3 << 8) | arg2;
    u32 i;
    for (i = 0; i < g->unk2; i++) {
        p[4] = (p[4] & 0xFF000000) | v;
        p += 6;
    }
    for (i = 0; i < g->unk4; i++) {
        p[2] = (p[2] & 0xFF000000) | v;
        p += 5;
    }
}

u32 func_800C8DE0(u32 arg0, s32 arg1, s32 arg2, s32 arg3) {
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

INCLUDE_ASM("asm/us/ovl_0b/nonmatchings/ovl_0b", func_800C8E80);

INCLUDE_ASM("asm/us/ovl_0b/nonmatchings/ovl_0b", func_800C9040);

INCLUDE_ASM("asm/us/ovl_0b/nonmatchings/ovl_0b", func_800C9374);

INCLUDE_ASM("asm/us/ovl_0b/nonmatchings/ovl_0b", func_800C94B8);

INCLUDE_ASM("asm/us/ovl_0b/nonmatchings/ovl_0b", func_800C9600);

void func_800C972C(void) {
    Battle *b = D_8006794C->unk1C->unk8E4.battle;
    b->unk132C = (b->unk11C0->unk20->unk4 ^ 0x30) * 4;
    D_800FDF38 = D_8006794C->unk1C->unk8E4.battle->unk1330->unk4;
}

INCLUDE_ASM("asm/us/ovl_0b/nonmatchings/ovl_0b", func_800C9794);

INCLUDE_ASM("asm/us/ovl_0b/nonmatchings/ovl_0b", func_800C9A28);

INCLUDE_ASM("asm/us/ovl_0b/nonmatchings/ovl_0b", func_800C9D1C);

INCLUDE_ASM("asm/us/ovl_0b/nonmatchings/ovl_0b", func_800C9EE0);

INCLUDE_ASM("asm/us/ovl_0b/nonmatchings/ovl_0b", func_800CA0A0);

void func_800CA28C(Vec4 *arg0, Vec4 *arg1) {
    *arg1 = *arg0;
}

s32 func_800CA2B0(s32 arg0) {
    s16 *t = D_8006794C->unk1C->unk8E4.battle->unk133C;
    return (s32)t + t[arg0];
}

INCLUDE_ASM("asm/us/ovl_0b/nonmatchings/ovl_0b", func_800CA2E4);

u16 func_800CA378(void) {
    Battle *b = D_8006794C->unk1C->unk8E4.battle;
    return b->unk1334[b->unk1330->unk8].unk62;
}

INCLUDE_ASM("asm/us/ovl_0b/nonmatchings/ovl_0b", func_800CA3CC);

void func_800CA530(BtlCmd *arg0) {
    if (D_8006794C->unk1C->unk8E4.battle->unk11CC[arg0->unkF] != 0 && func_800CA5AC(arg0) != NULL) {
        BtlUnit *u = arg0->unk4;
        if (!(u->unk68 & 1)) {
            u->unk86[1] = 0;
        }
    }
}

INCLUDE_ASM("asm/us/ovl_0b/nonmatchings/ovl_0b", func_800CA5AC);

INCLUDE_ASM("asm/us/ovl_0b/nonmatchings/ovl_0b", func_800CA694);

s32 func_800CA774(void) {
    s16 i;
    Battle122C *p = D_8006794C->unk1C->unk8E4.battle->unk122C;
    for (i = 0; i < 4; i++, p++) {
        if (p->unk4 != 0) {
            return 1;
        }
    }
    return 0;
}

INCLUDE_ASM("asm/us/ovl_0b/nonmatchings/ovl_0b", func_800CA7CC);

INCLUDE_ASM("asm/us/ovl_0b/nonmatchings/ovl_0b", func_800CAAD0);

s32 func_800CAC24(BtlScript *arg0) {
    if (D_800F78A4->unk0->unk0 == -1) {
        if (arg0->unk21 != 0) {
            return 0;
        }
        if (!((arg0->unk0 >> 3) & 1)) {
            func_800B3C00();
        }
        arg0->unk4 = 0;
    }
    return 0;
}

void func_800CAC98(BtlScript *arg0) {
    arg0->unk12 = arg0->unk8[1];
}

s32 func_800CACAC(BtlScript *arg0) {
    if (arg0->unk12 <= 0) {
        arg0->unk8 += 2;
        return 1;
    }
    return 0;
}

s32 func_800CACD8(BtlScript *arg0) {
    BtlUnit *u = D_8006794C->unk1C->unk8E4.battle->unk6F8;
    u16 mask = arg0->unk4->unkC;
    while (u != NULL) {
        if (u->unkB0 & mask) {
            func_800B2804(u);
        }
        u = u->unk0;
    }
    arg0->unk8 += 1;
    return 1;
}

INCLUDE_ASM("asm/us/ovl_0b/nonmatchings/ovl_0b", func_800CAD6C);

s32 func_800CAEC4(BtlScript *arg0, BtlUnit *arg1) {
    s16 *p = arg0->unk28;
    s16 i;
    for (i = 0; i < 3; i++) {
        s16 *q = p + i;
        arg1->unk8C[i] = ((q[3] - q[0]) * arg0->unk10 / p[6] + q[0]) << 16;
    }
    if (arg0->unk10 >= p[6]) {
        arg0->unk8 += (u16)p[7];
    }
    return 0;
}

INCLUDE_ASM("asm/us/ovl_0b/nonmatchings/ovl_0b", func_800CAF90);

void func_800CB008(BtlScript *arg0, BtlUnit *arg1) {
    s16 *p = arg0->unk28;
    s16 i;
    p[7] = 8;
    arg0->unk10 = 1;
    p[6] = arg0->unk8[1];
    for (i = 0; i < 3; i++) {
        s16 *q = p + i;
        q[0] = arg1->unk8C[i] >> 16;
        q[3] = arg0->unk8[i * 2 + 2] | (arg0->unk8[i * 2 + 3] << 8);
    }
}

s32 func_800CB090(s32 arg0, Unk800CB090 *arg1) {
    s16 i;
    func_800CAEC4((BtlScript *)arg0, (BtlUnit *)arg1);
    for (i = 0; i < 3; i++) {
        arg1->unk98[i] = arg1->unk8C[i];
    }
    return 0;
}

void func_800CB0F0(BtlScript *arg0, BtlUnit *arg1) {
    s16 *p = arg0->unk28;
    s16 i;
    p[7] = 8;
    arg0->unk10 = 1;
    p[6] = arg0->unk8[1];
    for (i = 0; i < 3; i++) {
        s16 *q = p + i;
        s16 v = arg1->unk8C[i] >> 16;
        q[0] = v;
        q[3] = v + (arg0->unk8[i * 2 + 2] | (arg0->unk8[i * 2 + 3] << 8));
    }
}

BtlUnit *func_800CB17C(u16 arg0) {
    BtlUnit *u;
    for (u = D_8006794C->unk1C->unk8E4.battle->unk6F8; u != NULL; u = u->unk0) {
        if (u->unkB0 & arg0) {
            break;
        }
    }
    return u;
}

void func_800CB1D4(u16 arg0, u16 *arg1, u16 *arg2) {
    BtlUnit *u;
    s32 n = 0;
    *arg1 = 0;
    *arg2 = 0;
    for (u = D_8006794C->unk1C->unk8E4.battle->unk6F8; u != 0; u = u->unk0) {
        if (u->unkB0 & arg0) {
            *arg1 += u->unk8C[0] >> 16;
            n++;
            *arg2 += u->unk8C[2] >> 16;
        }
    }
    if (n >= 2) {
        *arg1 = (s16)*arg1 / n;
        *arg2 = (s16)*arg2 / n;
    }
}

s32 func_800CB2E0(BtlScript *arg0, BtlUnit *arg1) {
    u32 f;
    u8 *p = arg0->unk8;
    s32 idx;
    if (p[1] == 0xFF) {
        f = arg1->unk68;
        arg1->unk9C->unkC = D_8006794C->unk1C->unk8E4.battle->unk488[(f >> 1) & 0xF].unk0->unkC[(f >> 18) & 1];
    } else {
        idx = arg0->unk16 + p[1];
        arg1->unk9C->unkC = D_8006794C->unk1C->unk8E4.battle->unk11C4[idx];
    }
    arg1->unkA0->unk3A = 0;
    arg0->unk14 = 0;
    arg0->unk8 += 2;
    return 1;
}

s32 func_800CB3A4(BtlScript *arg0) {
    arg0->unk22 = arg0->unk8[1] | (arg0->unk8[2] << 8);
    arg0->unk24 = arg0->unk8[3];
    arg0->unk25 = arg0->unk8[4] + 1;
    arg0->unk8 += 5;
    return 1;
}

s32 func_800CB3EC(BtlScript *arg0) {
    s16 v[4];
    s32 id;
    Unk800F78A40 *e = D_800F78A4->unk0;
    if ((e->unk0 != -1) & !((e->unk1C >> 15) & 1)) {
        return 0;
    }
    id = arg0->unk8[1] | (arg0->unk8[2] << 8);
    v[0] = arg0->unk8[3] | (arg0->unk8[4] << 8);
    v[1] = arg0->unk8[5] | (arg0->unk8[6] << 8);
    v[2] = arg0->unk8[7] | (arg0->unk8[8] << 8);
    v[3] = arg0->unk8[0] == 0x1A;
    func_800C2598((BtlCmd *)arg0->unk4, id, (s32)v);
    arg0->unk0 |= 1;
    arg0->unk8 += 9;
    return 1;
}

s32 func_800CB4E8(BtlScript *arg0, BtlUnit *arg1) {
    BtlUnit9C *m = arg1->unk9C;
    BtlUnitA0 *a = arg1->unkA0;
    if (arg0->unk14 != 0 && (a->unk3A >= m->unkC->unk2 || (arg1->unk3C & 0x02001002))) {
        D_800F78A4->unk2C->unk52 = 0;
        arg0->unk8 += 1;
        return 1;
    }
    return 0;
}

s32 func_800CB560(BtlScript *arg0) {
    if ((arg0->unk0 >> 1) & 1) {
        arg0->unk8 += 1;
        return 1;
    }
    return 0;
}

s32 func_800CB594(BtlScript *arg0) {
    D_800F78A4->unk0->unkF = 1;
    arg0->unk8 += 1;
    return 1;
}

s32 func_800CB5C4(BtlScript *arg0) {
    Unk800F78A40 *p = D_800F78A4->unk0;
    if ((p->unk0 == -1) | ((p->unk1C >> 15) & 1)) {
        arg0->unk8 += 1;
        return 1;
    }
    return 0;
}

s32 func_800CB618(BtlScript *arg0, BtlUnit *arg1) {
    arg1->unkAC |= arg0->unk8[1] | (arg0->unk8[2] << 8);
    func_800BD414(arg1, arg0->unk8[1] | (arg0->unk8[2] << 8));
    arg0->unk8 += 3;
    return 1;
}

s32 func_800CB68C(BtlScript *arg0, BtlUnit *arg1) {
    arg1->unkAC &= ~(arg0->unk8[1] | (arg0->unk8[2] << 8));
    func_800BD464(arg1, arg0->unk8[1] | (arg0->unk8[2] << 8));
    arg0->unk8 += 3;
    return 1;
}

INCLUDE_ASM("asm/us/ovl_0b/nonmatchings/ovl_0b", func_800CB700);

s32 func_800CB808(BtlScript *arg0, BtlUnit *arg1) {
    arg1->unk68 = (arg1->unk68 & ~0x40000) | ((arg0->unk8[1] & 1) << 18);
    arg0->unk8 += 2;
    return 1;
}

s32 func_800CB848(BtlScript *arg0) {
    D_8006794C->unk1C->unk8E4.battle->unk131C = arg0->unk8[1];
    arg0->unk8 += 2;
    return 1;
}

INCLUDE_ASM("asm/us/ovl_0b/nonmatchings/ovl_0b", func_800CB884);

INCLUDE_ASM("asm/us/ovl_0b/nonmatchings/ovl_0b", func_800CBA58);

void func_800CBBB0(BtlScript *arg0, BtlUnit *arg1) {
    s16 *p = arg0->unk28;
    s16 a = arg1->unk9C->unk54;
    s16 v;
    s16 r;
    p[0] = a;
    v = arg0->unk8[1] | (arg0->unk8[2] << 8);
    if (v == -1) {
        r = 0x1000;
    } else {
        r = a * v / 0x1000;
    }
    p[1] = r - p[0];
    arg0->unk10 = 1;
    p[2] = arg0->unk8[3];
}

s32 func_800CBC38(BtlScript *arg0, BtlUnit *arg1) {
    s16 *p = arg0->unk28;
    u16 v = arg0->unk28[0] + p[1] * arg0->unk10 / p[2];
    func_800BE1E0((s32)arg1, 0x37, v);
    if (v == 0x1000) {
        arg1->unk9C->unk0 &= ~2;
    }
    if (arg0->unk10 >= p[2]) {
        arg0->unk8 += 4;
        return 1;
    }
    return 0;
}

INCLUDE_ASM("asm/us/ovl_0b/nonmatchings/ovl_0b", func_800CBD28);

INCLUDE_ASM("asm/us/ovl_0b/nonmatchings/ovl_0b", func_800CBDC8);

s32 func_800CBE68(BtlScript *arg0, BtlUnit *arg1) {
    u8 i = arg0->unk8[1];
    BtlUnit9C *m = arg1->unk9C;
    BtlUnit9C50 *e;
    (*(e = m->unk50 + i)).unk4 &= 0xFC;
    arg0->unk8 += 2;
    return 1;
}

s32 func_800CBEB4(BtlScript *arg0) {
    arg0->unk18 = arg0->unk8[1] | (arg0->unk8[2] << 8);
    arg0->unk17 = arg0->unk8[3] + 1;
    arg0->unk1B = arg0->unk8[5];
    arg0->unk8 += 6;
    return 1;
}

s32 func_800CBEFC(BtlScript *arg0, BtlUnit *arg1) {
    arg1->unk7C = arg0->unk8[1];
    arg0->unk8 += 2;
    return 1;
}

INCLUDE_ASM("asm/us/ovl_0b/nonmatchings/ovl_0b", func_800CBF28);

s32 func_800CBF50(BtlScript *arg0, BtlUnit *arg1) {
    arg1->unk68 = (arg1->unk68 & ~0x1000000) | ((arg0->unk8[1] & 1) << 24);
    arg0->unk8 += 2;
    return 1;
}

void func_800CBF90(void) {
    s16 i;
    Battle1358 *p;
    D_8006794C->unk1C->unk8E4.battle->unk1518 = 0;
    D_8006794C->unk1C->unk8E4.battle->unk151A = 0;
    D_8006794C->unk1C->unk8E4.battle->unk152C = 0xFF;
    p = D_8006794C->unk1C->unk8E4.battle->unk1358;
    for (i = 15; i != -1; i--) {
        p->unk0 = NULL;
        p++;
    }
    D_8006794C->unk1C->unk8E4.battle->unk151C = 0;
    D_8006794C->unk1C->unk8E4.battle->unk1524 = 0;
}

INCLUDE_ASM("asm/us/ovl_0b/nonmatchings/ovl_0b", func_800CC044);

void func_800CC1A0(BtlUnit *arg0) {
    u8 f = arg0->unkCE;
    u8 n = 0;
    if (!((arg0->unk68 >> 22) & 1)) {
        if (f & 1) {
            func_800CC31C((s32)arg0, arg0->unkC8, (f & 8) ? 0 : 0xC0, 0)->unk13 = 1;
            n = 4;
        }
        if (f & 2) {
            func_800CC31C((s32)arg0, arg0->unkCA, 0, n)->unk13 = 1;
            n += 4;
        }
        if (f & 4) {
            func_800CC37C((s32)arg0, arg0->unkCC, 0, n)->unk13 = 1;
        }
    }
    arg0->unkCE = 0;
    arg0->unkC8 = 0;
    arg0->unkCA = 0;
    arg0->unkCC = 0;
}

INCLUDE_ASM("asm/us/ovl_0b/nonmatchings/ovl_0b", func_800CC278);

Unk800CC278 *func_800CC31C(s32 arg0, s16 arg1, s16 arg2, u8 arg3) {
    Unk800CC278 *p = func_800CC278(arg0);
    p->unk12 = 0;
    p->unk10 = arg3;
    p->unk18 = arg2;
    p->unk14 = func_800CC444(arg1);
    return p;
}

Unk800CC278 *func_800CC37C(s32 arg0, s16 arg1, s16 arg2, u8 arg3) {
    Unk800CC278 *p = func_800CC278(arg0);
    p->unk12 = 1;
    p->unk10 = arg3;
    p->unk18 = arg2;
    p->unk14 = func_800CC444(arg1);
    return p;
}

Unk800CC278 *func_800CC3E0(s32 arg0, u8 arg1, s16 arg2, s8 arg3) {
    Unk800CC278 *p = func_800CC278(arg0);
    p->unk12 = 2;
    p->unk10 = arg3;
    p->unk14 = (s32)&D_800F6A54[arg1];
    p->unk18 = arg2;
    return p;
}

INCLUDE_ASM("asm/us/ovl_0b/nonmatchings/ovl_0b", func_800CC444);

INCLUDE_ASM("asm/us/ovl_0b/nonmatchings/ovl_0b", func_800CC4F0);

INCLUDE_ASM("asm/us/ovl_0b/nonmatchings/ovl_0b", func_800CC818);

INCLUDE_ASM("asm/us/ovl_0b/nonmatchings/ovl_0b", func_800CC9C8);

INCLUDE_ASM("asm/us/ovl_0b/nonmatchings/ovl_0b", func_800CCAF0);

INCLUDE_ASM("asm/us/ovl_0b/nonmatchings/ovl_0b", func_800CCBC0);

void func_800CCD00(Unk800CCD00 *arg0, Unk800CCD00_1 *arg1) {
    arg0->unk0[0] = 0x04000000;
    arg0->unk0[1] = arg0->unk8 | 0x64000000;
    arg0->unk0[2] = arg0->unk10;
    arg0->unk0[3] = arg1->unk0 + (arg0->unk14 << 16);
    arg0->unk0[4] = (arg1->unk5 << 16) | arg1->unk4;
    addPrim(arg0->unk4, arg0->unk0);
    arg0->unk0 += 5;
}

INCLUDE_ASM("asm/us/ovl_0b/nonmatchings/ovl_0b", func_800CCDB8);

INCLUDE_ASM("asm/us/ovl_0b/nonmatchings/ovl_0b", func_800CD210);

INCLUDE_ASM("asm/us/ovl_0b/nonmatchings/ovl_0b", func_800CD5C8);

void func_800CD70C(u8 arg0) {
    s32 n = 0;
    while (arg0 >>= 1) {
        n++;
    }
    func_800357C0(D_800F6CB4, n + 1);
}

u8 *func_800CD75C(u8 arg0) {
    return &D_8006A218[D_80069018[arg0].unk0];
}

void *func_800CD78C(BtlUnit *arg0) {
    u32 f = arg0->unk68;
    Battle *b;
    if (!(f & 1)) {
        b = D_8006794C->unk1C->unk8E4.battle;
        return b->unk11C0->unk14 + b->unk488[(f >> 1) & 0xF].unk0->unk0;
    }
    return &D_8006794C->unk1C->unk3C[(f >> 1) & 0xF];
}

void func_800CD808(s32 arg0) {
    D_8006794C->unk1C->unk8E4.battle->unk151C = arg0;
    D_8006794C->unk1C->unk8E4.battle->unk1520 = 0;
}

INCLUDE_ASM("asm/us/ovl_0b/nonmatchings/ovl_0b", func_800CD83C);

void func_800CDC68(BtlUnit *arg0) {
    if (!(arg0->unk68 & 1)) {
        D_8006794C->unk1C->unk8E4.battle->unk1524 = &D_8006794C->unk1C->unk8E4.battle->unk488[(arg0->unk68 >> 1) & 0xF];
        if (D_8006794C->unk1C->unk8E4.battle->unk1524->unk8 == -1) {
            func_800F4F04(func_800357C0(D_800F6FB8, 9), 2);
            D_8006794C->unk1C->unk8E4.battle->unk1529 = 4;
            D_8006794C->unk1C->unk8E4.battle->unk1528 = func_800F4FC8() - 1;
            return;
        }
        D_8006794C->unk1C->unk8E4.battle->unk1529 = 0;
        D_8006794C->unk1C->unk8E4.battle->unk1528 = 0;
    }
}

INCLUDE_ASM("asm/us/ovl_0b/nonmatchings/ovl_0b", func_800CDD68);

INCLUDE_ASM("asm/us/ovl_0b/nonmatchings/ovl_0b", func_800CDEF0);

INCLUDE_ASM("asm/us/ovl_0b/nonmatchings/ovl_0b", func_800CE05C);

INCLUDE_ASM("asm/us/ovl_0b/nonmatchings/ovl_0b", func_800CE1B4);

void func_800CEBAC(void) {
    D_800F7274 = 0x1F;
}

void func_800CEBBC(void) {
    D_800F7274 = 0;
}

INCLUDE_ASM("asm/us/ovl_0b/nonmatchings/ovl_0b", func_800CEBC8);

Unk800CEE74 *func_800CEE74(void) {
    return D_800FE514;
}

void func_800CEE84(s32 arg0) {
    D_800F7270 = arg0;
}

s32 func_800CEE90(void) {
    return D_800FE50C;
}

void func_800CEEA0(s32 arg0) {
    D_800FE510 += arg0;
}

s32 func_800CEEB8(void) {
    return D_800FE510;
}

void func_800CEEC8(s32 arg0) {
    D_800FE51C = arg0;
}

INCLUDE_ASM("asm/us/ovl_0b/nonmatchings/ovl_0b", func_800CEED4);

INCLUDE_ASM("asm/us/ovl_0b/nonmatchings/ovl_0b", func_800CF448);

INCLUDE_ASM("asm/us/ovl_0b/nonmatchings/ovl_0b", func_800CF560);

INCLUDE_ASM("asm/us/ovl_0b/nonmatchings/ovl_0b", func_800CF574);

INCLUDE_ASM("asm/us/ovl_0b/nonmatchings/ovl_0b", func_800CF588);

INCLUDE_ASM("asm/us/ovl_0b/nonmatchings/ovl_0b", func_800CF58C);

INCLUDE_ASM("asm/us/ovl_0b/nonmatchings/ovl_0b", func_800CF5A4);

INCLUDE_ASM("asm/us/ovl_0b/nonmatchings/ovl_0b", func_800CF5F8);

INCLUDE_ASM("asm/us/ovl_0b/nonmatchings/ovl_0b", func_800CF690);

INCLUDE_ASM("asm/us/ovl_0b/nonmatchings/ovl_0b", func_800CF6B8);

INCLUDE_ASM("asm/us/ovl_0b/nonmatchings/ovl_0b", func_800CF708);

INCLUDE_ASM("asm/us/ovl_0b/nonmatchings/ovl_0b", func_800CF748);

INCLUDE_ASM("asm/us/ovl_0b/nonmatchings/ovl_0b", func_800CF788);

INCLUDE_ASM("asm/us/ovl_0b/nonmatchings/ovl_0b", func_800CF808);

INCLUDE_ASM("asm/us/ovl_0b/nonmatchings/ovl_0b", func_800CF8EC);

INCLUDE_ASM("asm/us/ovl_0b/nonmatchings/ovl_0b", func_800CF91C);

INCLUDE_ASM("asm/us/ovl_0b/nonmatchings/ovl_0b", func_800CF9B0);

INCLUDE_ASM("asm/us/ovl_0b/nonmatchings/ovl_0b", func_800CF9DC);

INCLUDE_ASM("asm/us/ovl_0b/nonmatchings/ovl_0b", func_800CFA00);

INCLUDE_RODATA("asm/us/ovl_0b/nonmatchings/ovl_0b", D_800A7C20);
