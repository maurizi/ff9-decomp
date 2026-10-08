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
    g->unk83C = (void (*)(s32, s32, s32, s32, s32))func_800AD90C;
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

void func_800A8E6C(void) {
    Battle *b = D_8006794C->unk1C->unk8E4.battle;
    s32 i;
    b->unk0 = 0;
    b->unk4 = 0;
    for (i = 0; i < 16; i++) {
        *(b->unk1354->unk0 + i) = 0;
    }
    for (i = 0; i < 4; i++) {
        b->unk153C[i] = 0;
        b->unk1538[i] = 0;
    }
    b->unk1530 = 0;
    b->unk1534 = -1;
    b->unk11C0 = 0;
    for (i = 0; (u32)i < 2; i++) {
        b->unk2[i] = 0;
    }
}

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

s32 func_800A90B0(void) {
    s32 p = D_80067948;
    Battle *b = D_8006794C->unk1C->unk8E4.battle;
    FileEntry *f = func_8001E218(1, 0x271B);
    s32 s = f->unk4;
    void *ctx = D_8006794C->unk1C->unk854;
    func_8002177C(ctx, s, (f->unkC - s) << 11, (void *)p, 0, -1, NULL);
    while (func_800217C4(ctx) != 0) {
    }
    b->unk1354->unk0[0] = (void *)p;
    func_8001C750(p, 0);
    func_8001DAA4((s32)b->unk154C, (s32)func_8001C7FC(4, 0x1F4, 0), 0x180, NULL);
    func_8001DAA4((s32)b->unk16CC, (s32)func_8001C7FC(4, 0x1F5, 0), 0xC00, NULL);
    func_8001DAA4((s32)b->unk22CC, (s32)func_8001C7FC(4, 0x1F6, 0), 0x100, NULL);
    DrawSync(0);
    D_80067948 = func_8001C8B0(p, 4, 0x1E, 0);
    return 0;
}

s32 func_800A91EC(void) {
    Battle1354 *p = D_8006794C->unk1C->unk8E4.battle->unk1354;
    func_8001C784(p->unk0[0]);
    p->unk0[0] = NULL;
    return 0;
}

INCLUDE_ASM("asm/us/ovl_0b/nonmatchings/ovl_0b", func_800A923C);

INCLUDE_ASM("asm/us/ovl_0b/nonmatchings/ovl_0b", func_800A94F8);

void func_800A9768(void) {
    Battle11C0 *p;
    s32 i;
    D_8006794C->unk1C->unk8E4.battle->unk11C0 = (Battle11C0 *)D_80067948;
    p = (Battle11C0 *)D_80067948;
    D_80067948 = (s32)(p + 1);
    p->unk24 = (s32)D_8006794C->unk1C->unk8E4.battle + 0x5800;
    p->unk1C = p->unk24 + 0x2800;
    p->unk44 = (s32 *)(p->unk1C + 0x6000);
    D_80067948 = (s32)p->unk44 + 0x35800;
    for (i = 0; i < 4; i++) {
        D_8006794C->unk1C->unk8E4.battle->unk153C[i] = D_80067948;
        D_80067948 += 0x13000;
        D_8006794C->unk1C->unk8E4.battle->unk1538[i] = (Battle1538 *)(D_80067948 - 0x28);
    }
    p->unk3C = (s32 *)(p + 1);
    for (i = 0; i < 4; i++) {
        p->unk3C[i] = D_80067948;
        D_80067948 += 0x1000;
    }
    p->unk2C[1] = D_80067948 + 0x400;
    p->unk2C[0] = D_80067948;
    p->unk34[0] = D_80067948 + 0x800;
    p->unk34[1] = p->unk34[0] + 0x20000;
    D_80067948 = p->unk34[1] + 0x20000;
    p->unkC = (s32 *)(D_80067944 + 0x136700);
}

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

s32 func_800A9A7C(Battle11C0 *arg0) {
    s32 id = func_800AB2C8();
    s32 i;
    RECT r;
    arg0->unk1C = (s32)func_8001C7FC(4, id, 0);
    arg0->unk18 = (s32)func_8001C7FC(4, (s32)func_8001C7FC(9, id, 0x1A0000), 0);
    if (func_8001C7FC(0xB, id, 0x190000) != 0) {
        arg0->unk40 = func_8001C7FC(4, (s32)func_8001C7FC(9, id, 0x190000), 0);
    }
    for (i = 0; i < (s32)func_8001C7FC(0xB, id, 0x40000); i++) {
        func_8001D8A8((s32)func_8001C7FC(4, (s32)func_8001C7FC(9, id, (i & 0xFFFF) | 0x40000), 0));
    }
    if (arg0->unk40 != NULL) {
        r.y = 0;
        r.x = 0;
        r.h = 0;
        r.w = 0;
        func_800C9374(arg0->unk40, 0);
    }
    DrawSync(0);
    D_8006794C->unk1C->unk8E4.battle->unk0 |= 1;
    return 0;
}

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

u32 func_800AAEF0(u32 arg0, u16 *arg1, u16 *arg2, u16 **arg3, s32 *arg4) {
    u32 best = -1;
    s32 bestn = 0;
    u16 id;
    u32 start;
    s32 n;
    s32 k;
    FileEntry *f;
    do {
        id = *arg2;
        start = -1;
        if (id != 0xFFFF) {
            n = 0;
            f = func_8001E218(arg0, id);
            if (f != NULL) {
                start = f->unk4;
                n = f->unkC - start;
            } else {
                k = 0;
                switch (arg0) {
                case 0:
                    k = 1;
                    break;
                case 1:
                    k = 0;
                    break;
                case 3:
                case 4:
                    k = 5;
                    break;
                case 5:
                    k = 0x11;
                    break;
                case 7:
                case 8:
                case 10:
                    k = 2;
                    break;
                case 2:
                    k = 6;
                    break;
                case 6:
                    k = 0xC;
                    break;
                case 11:
                    k = 9;
                    break;
                case 9:
                    k = 7;
                    break;
                }
                f = func_80063088(D_8006794C->unk1C->unk7E4, (k << 16) | id);
                if (f != NULL) {
                    start = f->unk4;
                    n = f->unk8;
                }
            }
            if (start < best) {
                best = start;
                bestn = n;
                *arg3 = arg2;
            }
        }
        arg2--;
    } while (arg2 >= arg1);
    *arg4 = bestn;
    return best;
}

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

s32 func_800AB2C8(void) {
    FileTable *t = D_8006794C->unk1C->unk7E4;
    FileEntry *base = t->unk4;
    FileEntry *e = &base[t->unk2] - 1;
    s32 v;
    do {
        v = e->unk0;
        if ((u8)(e->unk0 >> 16) == 0xC) {
            return v;
        }
        e--;
    } while (e >= base);
    return -1;
}

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

s32 func_800AB6F0(s32 arg0, s32 arg1) {
    s32 v = D_8006794C->unk1C->unk8E4.battle->unk11C0->unk3C[arg1];
    return func_8001C8B0(func_8001C8B0(v, 2, 0x1B0000, 0), 4, arg0, 0);
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

void func_800B0468(GameData *arg0, Battle *arg1) {
    BtlUnit *u;
    s32 sp10;
    u = arg1->unk6F8;
    sp10 = arg1->unk11C0->unk34[arg0->unk8];
    while (u != NULL) {
        if (((u->unk68 >> 29) & 1) && (arg1->unk11B8 & 2)) {
            func_800B4234(u);
        } else {
            u->unkBC = 0;
        }
        u = u->unk0;
    }
    u = arg1->unk6F8;
    while (u != NULL) {
        if (!((u->unk68 >> 22) & 1)) {
            func_800BC21C(u, &sp10);
        }
        if (func_800E9FCC() == u && (arg1->unk11B8 & 2) && !(u->unk9C->unk0 & 0x10)) {
            func_800B3460(arg0, arg1, u);
        }
        func_800BD93C(u);
        func_800BFAF4(u);
        u = u->unk0;
    }
    func_800B1764(arg1);
    func_800B05C0(arg0, arg1, sp10);
}

INCLUDE_ASM("asm/us/ovl_0b/nonmatchings/ovl_0b", func_800B05C0);

s32 func_800B065C(BtlUnit *arg0, BtlUnit *arg1, BtlCmd *arg2) {
    s32 r;
    if (arg0->unk3C & 0x42001103) {
        return 0;
    }
    if (arg2->unkE >= 0x30) {
        return 0;
    }
    if ((arg0->unkC4[1] & 0x10) && (arg2->unk8->unk8 & 8)) {
        r = arg0->unk33;
        if (arg0->unkC4[1] & 0x80) {
            r *= 2;
        }
        if (D_8007B720 >= D_8007B92D) {
            func_8004BF20();
            if (D_8007B720 >= D_8007B92D) {
                func_8004BF20();
            }
        }
        if (((*D_8007B720 << 8) | *D_8007B720++) % 100 < r) {
            func_800B3D54(arg0, 0x31, 0xB0, arg1->unkB0);
            return 1;
        }
    }
    if ((arg0->unkC4[1] & 0x400000) && (arg2->unk8->unk8 & 0x80)) {
        func_800B3D54(arg0, 0x32, arg2->unkF, arg1->unkB0);
        return 1;
    }
    return 0;
}

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

void func_800B0E04(BtlUnit *arg0, u8 *arg1) {
    u32 max;
    u32 lt;
    s32 h;
    if (!(arg0->unk3C & 0x42001103)) {
        if ((arg0->unkC4[1] & 0x100000) && arg0->unk28 != 0 && (arg0->unk3C & 0x200)) {
            max = arg0->unk24->unk0;
            h = arg0->unk28;
            lt = (s32)(h + (max >> 1)) < (s32)max;
            if (lt) {
                arg0->unk28 = h + (max >> 1);
            } else {
                arg0->unk28 = max;
            }
        }
        if ((arg0->unkC4[1] & 0x800000) && arg1[0xA] != 0) {
            if (arg0->unk2A + arg1[0xA] < arg0->unk24->unk2) {
                arg0->unk2A += arg1[0xA];
            } else {
                arg0->unk2A = arg0->unk24->unk2;
            }
        }
    }
}

void func_800B0EF8(Battle *arg0) {
    arg0->unk1088 = NULL;
    arg0->unk11BA = 0;
    arg0->unk11B8 = 2;
    arg0->unk1060.unk4 = NULL;
    arg0->unk1074.unk4 = NULL;
    func_800B3CEC_inline(&arg0->unk1074);
    func_800B3CEC_inline(&arg0->unk1060);
}

void func_800B0FC0(Battle *arg0) {
    s8 i;
    s8 j;
    s32 c;
    for (i = 0; i < 2; i++) {
        for (j = 0; j < 4; j++) {
            func_80063640(&arg0->unk108C[i][j].unk0, GetTPage(0, 1, 0, 0));
            if (j < 3) {
                c = 0xBC - j * 0x3C;
            } else {
                c = 0xA0;
            }
            setRGB0(&arg0->unk108C[i][j].unk8, c, c, 0);
            SetSemiTrans(&arg0->unk108C[i][j].unk8, 1);
        }
    }
}

INCLUDE_ASM("asm/us/ovl_0b/nonmatchings/ovl_0b", func_800B10F8);

INCLUDE_ASM("asm/us/ovl_0b/nonmatchings/ovl_0b", func_800B1250);

INCLUDE_ASM("asm/us/ovl_0b/nonmatchings/ovl_0b", func_800B1520);

INCLUDE_ASM("asm/us/ovl_0b/nonmatchings/ovl_0b", func_800B1764);

INCLUDE_ASM("asm/us/ovl_0b/nonmatchings/ovl_0b", func_800B1B48);

INCLUDE_ASM("asm/us/ovl_0b/nonmatchings/ovl_0b", func_800B23B4);

INCLUDE_ASM("asm/us/ovl_0b/nonmatchings/ovl_0b", func_800B253C);

INCLUDE_ASM("asm/us/ovl_0b/nonmatchings/ovl_0b", func_800B2804);

INCLUDE_ASM("asm/us/ovl_0b/nonmatchings/ovl_0b", func_800B2988);

void func_800B2DD4(Battle *arg0) {
    BtlCmd *p = &arg0->unk1074;
    BtlCmd *c;
    BtlUnit *u;
    BtlUnit9C *m;
    Battle *b;
    s32 id;
    while (p != NULL) {
        c = p->unk0;
        if (c != NULL && c->unkE != 0x36 && c->unkE < 0x3A) {
            u = c->unk4;
            b = D_8006794C->unk1C->unk8E4.battle;
            if (c->unkE < 0x30 && u != NULL && !(u->unk3C & 0x100)) {
                m = u->unk9C;
                u->unkB3 = 0;
                if ((s32)m->unkC == u->unkA4[9] && ((id = (u->unk68 >> 1) & 0xF) != 6 || b->unk1088->unkE != 0x15) && (id != 1 || (b->unk1088->unkE != 0x1F && b->unk1088->unkE != 0x17))) {
                    m->unkC = (BtlUnit9CC *)u->unkA4[32];
                    u->unkA0->unk3A = 0;
                    u->unk68 &= ~0x80000;
                }
            }
            func_800B4434(c);
            func_800B4320(&p->unk0, 1);
        } else {
            p = c;
        }
    }
}

void func_800B2F24(Battle *arg0) {
    BtlCmd *p = &arg0->unk1074;
    BtlCmd *c;
    BtlUnit *u;
    BtlUnit9C *m;
    Battle *b;
    s32 id;
    while (p != NULL) {
        c = p->unk0;
        if (c != NULL && c->unkE < 0x3B) {
            u = c->unk4;
            b = D_8006794C->unk1C->unk8E4.battle;
            if (c->unkE < 0x30 && u != NULL && !(u->unk3C & 0x100)) {
                m = u->unk9C;
                u->unkB3 = 0;
                if ((s32)m->unkC == u->unkA4[9] && ((id = (u->unk68 >> 1) & 0xF) != 6 || b->unk1088->unkE != 0x15) && (id != 1 || (b->unk1088->unkE != 0x1F && b->unk1088->unkE != 0x17))) {
                    m->unkC = (BtlUnit9CC *)u->unkA4[32];
                    u->unkA0->unk3A = 0;
                    u->unk68 &= ~0x80000;
                }
            }
            func_800B4434(c);
            func_800B4320(&p->unk0, 1);
        } else {
            p = c;
        }
    }
    arg0->unk11BC = 0;
    arg0->unk11BB = 0;
    arg0->unk11B8 &= ~0xC;
}

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

void func_800B81C4(BtlCalc *arg0, u8 arg1) {
    BtlUnit *u;
    u8 v;
    if ((arg0->unk4->unk68 & 1) && !(arg0->unk0->unk68 & 1)) {
        if (arg1 == 0) {
            func_800B0D4C(arg0->unk4, arg0->unk8->unkE);
        }
        func_800B0E04(arg0->unk4, arg0->unk8->unk8);
        u = arg0->unk4;
        if (((u->unk68 >> 30) & 1) && u->unk28 != 0 && !(u->unk3C & 0x02005143)) {
            if (u->unkC4[1] & 8) {
                v = u->unk33;
            } else {
                if (D_8007B720 >= D_8007B92D) {
                    func_8004BF20();
                    if (D_8007B720 >= D_8007B92D) {
                        func_8004BF20();
                    }
                }
                v = ((*D_8007B720 << 8) | *D_8007B720++) % arg0->unk4->unk33;
            }
            u = arg0->unk4;
            if (v + u->unk78 < 0xFF) {
                u->unk78 += v;
            } else {
                u->unk78 = 0xFF;
                func_800BEDBC(arg0->unk4, 0x4000);
            }
        }
    }
}

INCLUDE_ASM("asm/us/ovl_0b/nonmatchings/ovl_0b", func_800B8390);

s32 func_800B8664(BtlCalc *arg0) {
    if (D_8007B720 >= D_8007B92D) {
        func_8004BF20();
        if (D_8007B720 >= D_8007B92D) {
            func_8004BF20();
        }
    }
    if (((*D_8007B720 << 8) | *D_8007B720++) % 100 < arg0->unk16) {
        if (D_8007B720 >= D_8007B92D) {
            func_8004BF20();
            if (D_8007B720 >= D_8007B92D) {
                func_8004BF20();
            }
        }
        if (((*D_8007B720 << 8) | *D_8007B720++) % 100 >= arg0->unk12) {
            return 1;
        }
    }
    arg0->unk18 |= 1;
    return 0;
}

INCLUDE_ASM("asm/us/ovl_0b/nonmatchings/ovl_0b", func_800B87D0);

void func_800B8A24(BtlCalc *arg0) {
    u32 f = arg0->unk4->unk68;
    u32 c;
    Battle4880 *e;
    BtlUnit *u;
    if (!(f & 1)) {
        c = arg0->unk0->unkC4[0];
        e = D_8006794C->unk1C->unk8E4.battle->unk488[(f >> 1) & 0xF].unk0;
        if (((c & 0x2000) && (e->unk2 & 0x80)) || ((c & 0x4000) && (e->unk2 & 0x40)) || ((c & 0x8000) && (e->unk2 & 0x20)) || ((c & 0x10000) && (e->unk2 & 0x10)) || ((c & 0x20000) && (e->unk2 & 8)) || ((c & 0x40000) && (e->unk2 & 4)) || ((c & 0x80000) && (e->unk2 & 2)) || ((c & 0x100000) && (e->unk2 & 1))) {
            arg0->unk14 = arg0->unk14 * 3 >> 1;
        }
    }
    u = arg0->unk0;
    if ((u->unkC4[0] & 0x1000) && u->unk2A > 0) {
        arg0->unk14 = arg0->unk14 * 3 >> 1;
        arg0->unk18 |= 4;
    }
}

void func_800B8BE8(BtlCalc *arg0) {
    u8 s = arg0->unk0->unk33 >> 2;
    s32 r;
    if (s != 0) {
        if (D_8007B720 >= D_8007B92D) {
            func_8004BF20();
            if (D_8007B720 >= D_8007B92D) {
                func_8004BF20();
                if (D_8007B720 >= D_8007B92D) {
                    func_8004BF20();
                    if (D_8007B720 >= D_8007B92D) {
                        func_8004BF20();
                    }
                }
            }
        }
        r = (*D_8007B720 << 8) | *D_8007B720++;
        if (r % 100 < r % s) {
            arg0->unk14 *= 2;
            arg0->unk1B |= 4;
        }
    }
}

s32 func_800B8D58(BtlCalc *arg0) {
    BtlUnit *t;
    u8 e = arg0->unk8->unk8[6];
    if (arg0->unk0->unkC4[1] & 1) {
        return 1;
    }
    t = arg0->unk4;
    if (t->unk70 & e) {
        arg0->unk18 |= 0x40;
        return 0;
    }
    if (t->unk72 & e) {
        arg0->unk14 /= 2;
    }
    if (arg0->unk4->unk71 & e) {
        arg0->unk10 = 0;
        arg0->unk18 |= 8;
    }
    if (arg0->unk4->unk73 & e) {
        arg0->unk14 = arg0->unk14 * 3 >> 1;
    }
    if (e & 0x12) {
        func_800BF558(arg0->unk4, 0x01000000);
    }
    if (e & 1) {
        func_800BF558(arg0->unk4, 0x02000000);
    }
    return 1;
}

void func_800B8E9C(BtlCalc *arg0) {
    s32 d = arg0->unkE - arg0->unk10;
    s32 v;
    BtlUnit *u;
    BtlUnit *t;
    if (d <= 0) {
        d = 1;
    }
    arg0->unk1B |= 1;
    if (arg0->unk14 <= 0) {
        arg0->unk14 = 1;
    }
    v = d * arg0->unk14;
    if (v >= 10000) {
        v = 9999;
    }
    if (!(arg0->unk18 & 8)) {
        u = arg0->unk0;
        if ((u->unk68 & 1) && ((*u->unkC4 & 0x01000000) || D_8006794C->unk1C->unk3C[(u->unk68 >> 1) & 0xF].unk39[0] == 0x3C)) {
            if (!(arg0->unk4->unk3C & 0x40) && func_800C1E88(arg0->unk4, 0x10) == 0) {
                t = arg0->unk4;
                t->unk86[1] = t->unkA0->unk22 = (t->unk68 & 1) * 0x800;
                arg0->unk1B |= 2;
            }
        }
    } else {
        arg0->unk1B |= 2;
    }
    arg0->unk1E = v;
}

void func_800B8FF8(BtlCalc *arg0) {
    BtlUnit *t;
    BtlUnit *u;
    BtlUnit9C *m;
    if (arg0->unk8->unk8[5] != 0) {
        if (arg0->unk4->unk3C & 0x100) {
            arg0->unk18 |= 1;
            return;
        }
        arg0->unk18 |= 0x80;
        arg0->unk4->unk28 = arg0->unk8->unk8[5];
        t = arg0->unk4;
        t->unk86[1] = t->unkA0->unk22 = (t->unk68 & 1) * 0x800;
        return;
    }
    u = arg0->unk4;
    u->unk28 = 0;
    u->unk68 |= 0x100000;
    if (!(u->unk68 & 1)) {
        func_800C2250(u, D_8006794C->unk1C->unk8E4.battle->unk488[(u->unk68 >> 1) & 0xF].unk0->unk30);
    }
    m = u->unk9C;
    if ((s32)m->unkC != u->unkA4[(u->unk68 >> 18) & 1] && (!(u->unk68 & 1) || (s32)m->unkC != u->unkA4[9]) && u != D_8006794C->unk1C->unk8E4.battle->unk1088->unk4) {
        m->unkC = (BtlUnit9CC *)u->unkA4[(u->unk68 >> 18) & 1];
        u->unkA0->unk3A = 0;
    }
}

INCLUDE_ASM("asm/us/ovl_0b/nonmatchings/ovl_0b", func_800B918C);

s32 func_800B9418(BtlCalc *arg0) {
    BtlUnit *u;
    BtlUnit9C *m;
    if ((arg0->unk4->unk3C & 0x02000001) != 0x02000000) {
        return 1;
    }
    func_800B41A4(arg0->unk4, 0x3E);
    u = arg0->unk4;
    u->unk28 = 0;
    u->unk68 |= 0x100000;
    if (!(u->unk68 & 1)) {
        func_800C2250(u, D_8006794C->unk1C->unk8E4.battle->unk488[(u->unk68 >> 1) & 0xF].unk0->unk30);
    }
    m = u->unk9C;
    if ((s32)m->unkC != u->unkA4[(u->unk68 >> 18) & 1] && (!(u->unk68 & 1) || (s32)m->unkC != u->unkA4[9]) && u != D_8006794C->unk1C->unk8E4.battle->unk1088->unk4) {
        m->unkC = (BtlUnit9CC *)u->unkA4[(u->unk68 >> 18) & 1];
        u->unkA0->unk3A = 0;
    }
    func_800CD5C8(0x13);
    return 0;
}

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

s32 func_800B96AC(BtlUnit *arg0, s16 arg1) {
    BtlUnit *u;
    u32 f;
    if (arg0->unk2A >= arg1) {
        for (u = D_8006794C->unk1C->unk8E4.battle->unk6F8; u != NULL; u = u->unk0) {
            f = u->unk68;
            if ((f & 1) && ((D_8006794C->unk1C->unk3C[(f >> 1) & 0xF].unk34 >> 4) & 0x1F) == 2) {
                if (!(u->unk3C & 0x13021D0B) && !(arg0->unk3C & 0x13021D0B)) {
                    return 1;
                }
                break;
            }
        }
    }
    func_800CD5C8(0xF);
    return 0;
}

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

void func_800BA534(BtlCalc *arg0) {
    BtlUnit *t = arg0->unk4;
    BtlUnit *u;
    s32 d;
    if (arg0->unk0->unkA0->unk22 == t->unkA0->unk22 || (D_8006794C->unk1C->unk8E4.battle->unkD83 != 0 && (t->unk68 & 1))) {
        arg0->unk14 = arg0->unk14 * 3 >> 1;
    }
    u = arg0->unk0;
    if (!(u->unk68 & 1) || (!(u->unkC4[0] & 0x800) && !(u->unk74[0] & 2))) {
        d = (u->unk68 >> 8) & 3;
        d -= (arg0->unk4->unk68 >> 8) & 3;
        if ((d < 0 ? -d : d) >= 2) {
            arg0->unk14 /= 2;
        }
    }
}

void func_800BA660(BtlCalc *arg0) {
    BtlUnit *u = arg0->unk0;
    if (u->unk79 & u->unk74[6]) {
        arg0->unk14 = arg0->unk14 * 3 >> 1;
    }
}

s32 func_800BA6A4(BtlCalc *arg0) {
    BtlUnit *t = arg0->unk4;
    u8 e = arg0->unk0->unk74[6];
    if (t->unk70 & e) {
        arg0->unk18 |= 0x40;
        return 0;
    }
    if (t->unk72 & e) {
        arg0->unk14 /= 2;
    }
    if (arg0->unk4->unk71 & e) {
        arg0->unk10 = 0;
        arg0->unk18 |= 8;
    }
    if (arg0->unk4->unk73 & e) {
        arg0->unk14 = arg0->unk14 * 3 >> 1;
    }
    if (e & 0x12) {
        func_800BF558(arg0->unk4, 0x01000000);
    }
    if (e & 1) {
        func_800BF558(arg0->unk4, 0x02000000);
    }
    return 1;
}

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

void func_800BBF5C(Character *arg0, BtlUnit *arg1) {
    s32 *a;
    s32 *b;
    s32 *c;
    s32 id;
    arg1->unk74 = (u8 *)&D_8006E0E8[arg0->unk39[0]];
    id = ((Unk8006E0E8 *)arg1->unk74)->unk2 | 0x20000;
    arg1->unkA8->unk8 = (BtlUnit9C8 *)func_800AB6F0(id, (u8)arg1->unk68 >> 5);
    func_800AB758(id, (u8)arg1->unk68 >> 5, &a, &b, &c);
    func_800C7730(arg1->unkA8, (s16 *)a, (s16 *)b);
    func_800C2344(arg1->unkA8);
    arg1->unkA8->unkC = 0;
    switch ((arg0->unk34 >> 4) & 0x1F) {
    case 0:
        arg0->unk21 = 0xD;
        break;
    case 1:
        arg0->unk21 = 0xD;
        break;
    case 3:
    case 5:
        arg0->unk21 = 0xF;
        break;
    case 4:
    case 6:
        arg0->unk21 = 0xF;
        break;
    case 9:
        arg0->unk21 = 0xE;
        break;
    case 10:
    case 11:
        arg0->unk21 = 0xF;
        break;
    case 12:
        arg0->unk21 = 6;
        break;
    case 14:
        arg0->unk21 = 0x19;
        break;
    case 15:
        arg0->unk21 = 6;
        break;
    case 16:
    case 17:
        arg0->unk21 = 0xE;
        break;
    case 2:
    case 7:
    case 8:
    case 13:
    case 18:
        arg0->unk21 = 0x10;
        break;
    }
    arg1->unkA8->unk1C = arg1->unk9C;
    arg1->unkA8->unk4 = arg0->unk21;
}

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

void func_800BCAA4(BtlUnit *arg0) {
    u32 f;
    if (arg0->unk7E & 4) {
        arg0->unk8C[2] += (arg0->unkA0->unk22 == 0) ? 0xC80000 : 0xFF380000;
    }
    f = arg0->unk68;
    if (f & 1) {
        if ((D_8006794C->unk1C->unk8E4.battle->unk1088->unk8->unkB & 0x81) == 0x81) {
            arg0->unk9C->unkC = (BtlUnit9CC *)arg0->unkA4[3];
        } else {
            arg0->unk9C->unkC = (BtlUnit9CC *)arg0->unkA4[2];
        }
    } else if (arg0->unk28 == 0 && (u8)(D_8006794C->unk1C->unk8E4.battle->unk488 + ((f >> 1) & 0xF))->unk4 >> 7) {
        arg0->unk9C->unkC = (BtlUnit9CC *)arg0->unkA4[3];
        func_800C2250(arg0, D_8006794C->unk1C->unk8E4.battle->unk488[(arg0->unk68 >> 1) & 0xF].unk0->unk30);
    } else {
        if ((arg0->unk68 >> 23) & 1) {
            arg0 = func_800C1DEC();
        }
        arg0->unk9C->unkC = (BtlUnit9CC *)arg0->unkA4[((arg0->unk68 >> 18) & 1) + 2];
    }
    arg0->unk68 |= 0x20000;
    arg0->unkA0->unk3A = 0;
}

void func_800BCC64(BtlUnit *arg0) {
    BtlUnit9C *m = arg0->unk9C;
    s32 *a = arg0->unkA4;
    s32 c = (s32)m->unkC;
    if (c == a[2]) {
        if (arg0->unk3C & 0x100) {
            arg0->unk7D = 1;
            arg0->unk68 &= ~0x20000;
        } else if ((arg0->unk68 >> 19) & 1) {
            m->unkC = (BtlUnit9CC *)a[10];
        } else {
            arg0->unk68 &= ~0x20000;
        }
    } else if (c == a[3]) {
        if (arg0->unk3C & 0x100) {
            arg0->unk7D = 5;
            arg0->unk68 &= ~0x20000;
            arg0->unk9C->unkC = (BtlUnit9CC *)arg0->unkA4[4];
        } else {
            m->unkC = (BtlUnit9CC *)a[6];
        }
    } else if (c == a[6]) {
        if ((arg0->unk68 >> 19) & 1) {
            m->unkC = (BtlUnit9CC *)a[11];
        } else if (arg0->unk3C & 0x30202) {
            arg0->unk68 &= ~0x20000;
        } else {
            m->unkC = (BtlUnit9CC *)a[5];
        }
    } else {
        arg0->unk68 &= ~0x20000;
    }
    arg0->unkA0->unk3A = 0;
}

INCLUDE_ASM("asm/us/ovl_0b/nonmatchings/ovl_0b", func_800BCDBC);

INCLUDE_ASM("asm/us/ovl_0b/nonmatchings/ovl_0b", func_800BCEF0);

s32 func_800BD31C(BtlCmd *arg0) {
    BtlUnit *u;
    s32 r;
    u32 bit = arg0->unk10_5;
    do {
        r = 1;
        if (bit) {
            return r;
        }
    } while (0);
    for (u = D_8006794C->unk1C->unk8E4.battle->unk6F8; u != 0; u = u->unk0) {
        if ((u->unk68 >> 17) & 1) {
            r = 0;
            if (u->unkA0->unk3A >= u->unk9C->unkC->unk2) {
                u->unk8C[2] = u->unk98->unk0[2];
                if (u->unk68 & 1) {
                    func_800BCC64(u);
                } else {
                    func_800BCDBC(u);
                }
                func_800BC78C(u);
            }
        }
    }
    return r;
}

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

void func_800BD774(BtlUnit *arg0) {
    BtlUnit *u;
    s32 d;
    s32 v;
    for (u = D_8006794C->unk1C->unk8E4.battle->unk6F8; u != 0; u = u->unk0) {
        if ((u->unk68 & 1) == (arg0->unk68 & 1) && u != arg0 && ((u->unk68 >> 28) & 1)) {
            u->unk7E = 1;
            d = arg0->unk80 >> 1;
            if (u->unk3C & 0x100) {
                u->unk7E = 0x20;
            } else if (u->unk3C & 1) {
                u->unk80 = 0;
            } else {
                if (u != D_8006794C->unk1C->unk8E4.battle->unk1088->unk4) {
                    v = (u->unk68 & 1) << 11;
                    u->unkA0->unk22 = v;
                    u->unk86[1] = v;
                }
                if (d < u->unk28) {
                    u->unk28 -= d;
                } else {
                    u->unk28 = 0;
                }
                u->unk80 = d;
                if (u->unk28 == 0) {
                    func_800B97A0(u);
                }
            }
            func_800CC044(u);
        }
    }
}

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

void func_800BDA60(BtlUnit *arg0) {
    s32 d = 0x01900000;
    u32 f;
    u32 v;
    u32 m;
    if ((arg0->unk68 >> 8) & 3) {
        d = 0xFE700000;
    }
    arg0->unk8C[2] = arg0->unk98->unk0[2] + d;
    arg0->unk98->unk0[2] += d;
    f = arg0->unk68;
    m = f & ~0x300;
    v = (f >> 8) & 3;
    v = v == 0;
    arg0->unk68 = m | (v << 8);
}

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

s32 func_800BE784(s32 arg0) {
    Battle *b = D_8006794C->unk1C->unk8E4.battle;
    s32 r = -1;
    BtlUnit *u;
    s32 k;
    switch (arg0) {
    case 32:
        r = b->unk132C & 3;
        break;
    case 33:
        r = D_8006794C->unk1C->unk560;
        break;
    case 34:
        r = b->unkD80;
        break;
    case 35:
        r = b->unkD81;
        break;
    case 36:
        if (b->unk1088 != NULL && b->unk1088->unk4 != NULL) {
            r = b->unk1088->unk4->unkB0;
        }
        break;
    case 37:
        if (b->unk1088 != NULL) {
            r = (u16)b->unk1088->unkC;
        }
        break;
    case 38:
        if (b->unk1088 != NULL && b->unk1088->unk4 != NULL && b->unk1088->unk4->unk74 != NULL) {
            r = b->unk1088->unk4->unk74[6];
        }
        break;
    case 39:
        if (b->unk1088 != NULL && b->unk1088->unk4 != NULL) {
            r = 0;
            if (!(b->unk1088->unk4->unkC4[1] & 1)) {
                r = b->unk1088->unk8->unk6;
            }
        }
        break;
    case 40:
        r = b->unkD85;
        break;
    case 41:
        r = D_800FDF34;
        break;
    case 42:
        r = 0;
        for (u = b->unk6F8; u != NULL; u = u->unk0) {
            if (u->unk68 & 1) {
                k = (D_8006794C->unk1C->unk3C[(u->unk68 >> 1) & 0xF].unk34 >> 4) & 0x1F;
            } else {
                k = 0x13;
            }
            if ((u32)(k - 0xA) < 2) {
                r = func_800B3EF8(u, 0x3A);
                break;
            }
        }
        break;
    }
    return r;
}

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

void func_800C02D4(BtlUnit *arg0) {
    s16 a[3];
    s16 b[3];
    Unk800CEE74 *e = func_800CEE74();
    s32 i;
    s32 v;
    b[0] = e->unkC;
    b[1] = e->unkD;
    b[2] = e->unkE;
    if (arg0->unk3C & 0x40) {
        a[0] = 0x30;
        a[1] = 0x48;
        a[2] = 0x58;
    } else if (arg0->unk3C & 0x800) {
        a[0] = -0x10;
        a[1] = 0x28;
        a[2] = 0x28;
    } else if (arg0->unk3C & 0x01000000) {
        a[0] = -0x50;
        a[1] = 0x10;
        a[2] = 0x48;
    } else if (arg0->unk3C & 0x02000000) {
        a[0] = 0x30;
        a[1] = 0;
        a[2] = -0x60;
    } else {
        a[2] = 0;
        a[1] = 0;
        a[0] = 0;
    }
    for (i = 0; i < 3; i++) {
        v = b[i] - a[i];
        if (v < 0) {
            arg0->unkC1[i] = 0;
        } else if (v >= 0x100) {
            arg0->unkC1[i] = 0xFF;
        } else {
            arg0->unkC1[i] = b[i] - a[i];
        }
    }
}

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

void func_800C0840(void) {
    s32 i;
    s32 j;
    s8 c[3];
    s8 v;
    u16 *p;
    D_800FDF50.x = 0x100;
    D_800FDF50.y = 0xF2;
    D_800FDF50.w = 0x100;
    D_800FDF50.h = 1;
    for (i = 2; i < 0xC; i += 3) {
        j = 0;
        p = (u16 *)D_800FE360;
        for (; j < 0x100; j++) {
            c[0] = p[(i << 8) + j] & 0x1F;
            c[1] = (p[(i << 8) + j] >> 5) & 0x1F;
            c[2] = (p[(i << 8) + j] >> 10) & 0x1F;
            if (c[0] == 0 && c[1] == 0 && c[2] == 0) {
                v = 0;
            } else {
                v = (c[0] + c[1] + c[2]) / 3;
                if (v <= 0) {
                    v = 1;
                }
            }
            p[(i << 8) + j] = v + ((v << 10) + (v << 5));
        }
        D_800FDF50.y = i + 0xF2;
        LoadImage(&D_800FDF50, (u32 *)((u16 *)D_800FE360 + (i << 8)));
    }
}

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

void func_800C0DAC(BtlUnit *arg0) {
    s32 i;
    s32 bit;
    s32 m;
    for (i = 0; i < 16; i++) {
        bit = 0x10000 << i;
        m = bit & 0xEFFF0000;
        if ((arg0->unk3C & m) && (s16)(arg0->unk46[i] -= arg0->unk2E) < 0) {
            if (bit < 0) {
                if (!func_800B3E7C(arg0->unkC) && func_800BEDBC(arg0, 1) != 2) {
                    func_800BF558(arg0, 0x80000000);
                    arg0->unk7E |= 0x20;
                    func_800CC044(arg0);
                }
            } else if (bit & 0x08000000) {
                func_800BEDBC(arg0, 0x100);
                func_800CC044(arg0);
            } else {
                func_800BF558(arg0, bit);
            }
        }
    }
}

INCLUDE_ASM("asm/us/ovl_0b/nonmatchings/ovl_0b", func_800C0EC8);

s32 func_800C1184(u32 *arg0) {
    s32 r = 2;
    s32 th;
    if ((*arg0 >> 2) & 1) {
        r = (((*arg0 >> 3) & 1) == 0) * 2;
    } else {
        th = 0x18;
        if ((u8)func_800B0CF8(1, 0x200)) {
            th = 0;
        } else if ((*arg0 >> 3) & 1) {
            th = 0xFF;
        }
        if (D_8007B720 >= D_8007B92D) {
            func_8004BF20();
        }
        if (*D_8007B720++ < th) {
            r = 0;
        } else {
            th = 0x10;
            if ((u8)func_800B0CF8(1, 0x400)) {
                th = 0x55;
            }
            if (D_8007B720 >= D_8007B92D) {
                func_8004BF20();
            }
            if (*D_8007B720++ < th) {
                r = 1;
            }
        }
    }
    return r;
}

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

void func_800C1990(BtlUnit *arg0) {
    BtlUnit **pp = &D_8006794C->unk1C->unk8E4.battle->unk6F8;
    BtlUnit *n;
    s32 a;
    s32 b;
    while (pp != NULL) {
        n = *pp;
        a = (u8)arg0->unk68 >> 5;
        b = (u8)n->unk68 >> 5;
        if (a < b) {
            arg0->unk0 = n;
            *pp = arg0;
            return;
        }
        pp = &n->unk0;
    }
}

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

void func_800C1A8C(BtlUnit *arg0, u32 arg1) {
    s32 i;
    Unk800CEE74 *e;
    s32 k;
    u32 f;
    if (arg1 < 0x20) {
        i = 0;
        e = func_800CEE74();
        func_800C8E80(arg0->unk9C, 0x60);
        for (; i < arg0->unk9C->unk8->unk3; i++) {
            func_800C8D18(arg0->unk9C, i, (s32)((e->unkC * arg1) << 11) >> 16, (s32)((e->unkD * arg1) << 11) >> 16, (s32)((e->unkE * arg1) << 11) >> 16);
        }
        if (arg0->unkA8 != NULL) {
            func_800C8E80(arg0->unkA8, 0x60);
            for (i = 0; i < arg0->unkA8->unk8->unk3; i++) {
                func_800C8D18(arg0->unkA8, i, (s32)((e->unkC * arg1) << 11) >> 16, (s32)((e->unkD * arg1) << 11) >> 16, (s32)((e->unkE * arg1) << 11) >> 16);
            }
        }
        f = arg0->unk68;
        k = f >> 1;
        k &= 0xF;
        if (!(f & 1)) {
            k += 9;
        }
        D_8006794C->unk1C->unk8E4.battle->unk11C0->unk28[k].unk0_0 = (s32)(func_800CEE74()->unkF * arg1) >> 5;
    }
}

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

s32 func_800C20B8(GameData *arg0, u32 arg1, s32 arg2) {
    Battle *b;
    if (!(arg0->unk56F & 0x10)) {
        func_800AD90C(0x8601, 0, arg1, 0, 0);
        arg0->unk56F |= 0x10;
    }
    if (!(arg0->unk56F & 2)) {
        b = D_8006794C->unk1C->unk8E4.battle;
        b->unkD86 += 4;
        if ((u32)(s8)b->unkD86 < arg1) {
            return 0;
        }
        func_800AD90C(0x700, arg2, 0, 0, 0);
        arg0->unk56F |= 2;
    }
    if (func_800AD90C(0xC00, 0, 0, 0, 0) != 0) {
        return 0;
    }
    if (!(arg0->unk56F & 0x20)) {
        func_800AD90C(0, arg2, 0, 0, 0);
        arg0->unk56F |= 0x20;
    }
    return 1;
}

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

void func_800C8BE0(BtlUnit9C *arg0, u8 arg1, s16 arg2, s16 arg3, s16 arg4) {
    BtlMesh *g = &arg0->unk8->unk10[arg1];
    u32 *p = g->unk1C;
    u32 i;
    for (i = 0; i < g->unk2; i++) {
        p[4] = func_800C8DE0(p[4], arg2, arg3, arg4);
        p += 6;
    }
    for (i = 0; i < g->unk4; i++) {
        p[2] = func_800C8DE0(p[2], arg2, arg3, arg4);
        p += 5;
    }
}

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

Battle1330 *func_800C9600(Battle11C020 *arg0) {
    Battle1330 *e;
    u16 r;
    u16 sum;
    u16 i;
    s8 v;
    if (D_8007B720 >= D_8007B92D) {
        func_8004BF20();
    }
    r = (*D_8007B720++ * 100) / 256;
    e = arg0->unk8;
    sum = e->unk0;
    i = 0;
    while (r >= sum) {
        e++;
        sum += e->unk0;
        i++;
    }
    v = D_8006794C->unk1C->unk848;
    if (v != -1) {
        i = v;
        if (i >= arg0->unk1) {
            i = 0;
        }
        e = &arg0->unk8[i];
    }
    D_8006794C->unk1C->unk8E4.battle->unk1340 = i;
    return e;
}

void func_800C972C(void) {
    Battle *b = D_8006794C->unk1C->unk8E4.battle;
    b->unk132C = (b->unk11C0->unk20->unk4 ^ 0x30) * 4;
    D_800FDF38 = D_8006794C->unk1C->unk8E4.battle->unk1330->unk4;
}

INCLUDE_ASM("asm/us/ovl_0b/nonmatchings/ovl_0b", func_800C9794);

INCLUDE_ASM("asm/us/ovl_0b/nonmatchings/ovl_0b", func_800C9A28);

void func_800C9D1C(Unk800C9D1C *arg0, Battle4880 *arg1) {
    s16 i;
    BtlUnit9CC *h;
    arg1->unk24 = arg0->unk1C;
    arg1->unk2 = arg0->unk41;
    arg1->unk3 = arg0->unk40;
    arg1->unk27 = arg0->unk47;
    arg1->unk4.unk0 = arg0->unkC;
    arg1->unk4.unk2 = arg0->unkE;
    arg1->unk44 = arg0->unk10;
    arg1->unk46 = arg0->unk12;
    arg1->unk4C = arg0->unk69;
    arg1->unk48 = arg0->unk14;
    for (i = 0; i < 6; i++) {
        h = func_8001C7FC(4, arg0->unk20[i] | 0x30000, 0);
        arg1->unkC[i] = h;
        if (h->unkC >= 0) {
            h->unkC = (s32)h + h->unkC;
            if (h->unk10 != 0) {
                h->unk10 = (s32)arg1->unkC[i] + h->unk10;
            }
        }
    }
    for (i = 0; i < 3; i++) {
        arg1->unk29[i] = arg0->unk48[i];
    }
    arg1->unk30 = arg0->unk4C;
    arg1->unk26 = arg0->unk4E;
    for (i = 0; i < 6; i++) {
        arg1->unk32[i] = arg0->unk50[i];
        arg1->unk38[i] = arg0->unk56[i];
        arg1->unk3E[i] = arg0->unk5C[i];
    }
}

void func_800C9EE0(Unk800C9EE0 *arg0, BtlUnit *arg1, s32 *arg2) {
    Battle *b = D_8006794C->unk1C->unk8E4.battle;
    Battle488 *s = &b->unk488[(arg1->unk68 >> 1) & 0xF];
    Battle4880 *e;
    s16 r;
    s->unk0 = &b->unk4E8[arg0->unk0];
    arg1->unk68 = (((arg1->unk68 & ~0x10000000) | ((arg0->unk1 & 1) << 28)) & ~0x300) | 0x200;
    e = &D_8006794C->unk1C->unk8E4.battle->unk4E8[arg0->unk0];
    arg1->unk24 = &e->unk4;
    arg1->unk28 = e->unk4.unk0;
    arg1->unk2A = arg1->unk24->unk2;
    s->unk4 = (s->unk4 & ~0x100) | ((arg0->unk1 << 7) & 0x100);
    if (!((s->unk4 >> 9) & 1)) {
        arg1->unk8C[0] = arg1->unk98->unk0[0] = arg1->unkA0->unk14 = arg0->unk4 << 16;
        arg1->unk8C[1] = arg1->unk98->unk0[1] = arg1->unkA0->unk18 = arg0->unk6 << 16;
        arg1->unk8C[2] = arg1->unk98->unk0[2] = arg1->unkA0->unk1C = arg0->unk8 << 16;
        arg1->unkA0->unk20 = 0;
        arg1->unk86[0] = 0;
        arg1->unkA0->unk24 = 0;
        arg1->unk86[2] = 0;
        r = (arg0->unkA + D_800F5850[*arg2 & 3]) & 0xFFF;
        arg1->unkA0->unk22 = r;
        arg1->unk86[1] = r;
    }
    arg1->unkA4 = (s32 *)s->unk0->unkC;
    arg1->unk9C->unkC = s->unk0->unkC[0];
}

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
    if (D_8006794C->unk1C->unk8E4.battle->unk11C4.unk8[arg0->unkF] != 0 && func_800CA5AC(arg0) != NULL) {
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
    Battle122C *p = D_8006794C->unk1C->unk8E4.battle->unk11C4.unk68;
    for (i = 0; i < 4; i++, p++) {
        if (p->unk4 != 0) {
            return 1;
        }
    }
    return 0;
}

INCLUDE_ASM("asm/us/ovl_0b/nonmatchings/ovl_0b", func_800CA7CC);

s32 func_800CAAD0(BtlScript *arg0, BtlUnit *arg1) {
    BtlUnit9C *m = arg1->unk9C;
    if (!((arg0->unk0 >> 2) & 1) && arg0->unk14 != 0 && arg1->unkA0->unk3A >= m->unkC->unk2) {
        m->unkC = (BtlUnit9CC *)arg1->unkA4[(arg1->unk68 >> 18) & 1];
        arg1->unkA0->unk3A = 0;
        arg0->unk0 |= 4;
    }
    if ((arg1->unk3C & 0x02001002) || ((arg0->unk0 >> 2) & 1)) {
        if (D_800F78A4->unk0->unk0 != -1) {
            return 0;
        }
        if (arg0->unk21 != 0) {
            return 0;
        }
        if (D_8006794C->unk1C->unk8E4.battle->unk151C != 0) {
            return 0;
        }
        if (!((arg0->unk0 >> 3) & 1)) {
            func_800B3C00();
        }
        arg0->unk4 = 0;
    }
    return 0;
}

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
        arg1->unk9C->unkC = D_8006794C->unk1C->unk8E4.battle->unk11C4.unk0[idx];
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

s32 func_800CBD28(BtlScript *arg0, BtlUnit *arg1) {
    BtlUnit9C50 *g;
    BtlUnit9C50 *e = &arg1->unk9C->unk50[arg0->unk8[1]];
    e->unk4 |= 1;
    g = &arg1->unk9C->unk50[arg0->unk8[1]];
    g->unkC = 0;
    e = &arg1->unk9C->unk50[arg0->unk8[1]];
    e->unk12 = 0x1000;
    arg0->unk8 += 2;
    return 1;
}

s32 func_800CBDC8(BtlScript *arg0, BtlUnit *arg1) {
    BtlUnit9C50 *g;
    BtlUnit9C50 *e = &arg1->unk9C->unk50[arg0->unk8[1]];
    e->unk4 |= 3;
    g = &arg1->unk9C->unk50[arg0->unk8[1]];
    g->unkC = 0;
    e = &arg1->unk9C->unk50[arg0->unk8[1]];
    e->unk12 = 0x1000;
    arg0->unk8 += 2;
    return 1;
}

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

s32 func_800CBF28(BtlScript *arg0, BtlUnit *arg1) {
    arg0->unk26 = arg0->unk27 = arg0->unk8[1];
    arg0->unk8 += 2;
    return 1;
}

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

void func_800CC044(BtlUnit *arg0) {
    u16 f = arg0->unk7E;
    u8 n = 0;
    if (!((arg0->unk68 >> 22) & 1)) {
        if (f & 0x100) {
            func_800BD774(arg0);
        }
        if (f & 0x80) {
            func_800CC3E0((s32)arg0, 2, 0, 0);
            arg0->unk7E = 0;
        } else if (f & 0x60) {
            if (f & 0x20) {
                func_800CC3E0((s32)arg0, 0, 0, 0);
                n = 2;
            }
            if (f & 0x40) {
                func_800CC3E0((s32)arg0, 1, 0, n);
            }
            arg0->unk7E = 0;
        } else {
            if (f & 1) {
                if (f & 4) {
                    func_800CC3E0((s32)arg0, 3, 0x80, 0);
                    n = 2;
                }
                if (f & 2) {
                    func_800CC31C((s32)arg0, arg0->unk80, 0xC0, n);
                } else {
                    func_800CC31C((s32)arg0, arg0->unk80, 0, n);
                }
                n += 4;
            }
            if (f & 8) {
                if (f & 0x10) {
                    func_800CC37C((s32)arg0, arg0->unk82, 0xC0, n);
                } else {
                    func_800CC37C((s32)arg0, arg0->unk82, 0, n);
                }
            }
            arg0->unk7E = 0;
        }
    } else {
        arg0->unk7E = 0;
    }
    arg0->unk80 = 0;
    arg0->unk82 = 0;
}

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

void func_800CDEF0(BtlCmd *arg0) {
    u8 *s = NULL;
    u8 buf[32];
    u16 t;
    switch (arg0->unkE) {
    case 0xE:
    case 0xF:
        s = func_800CD75C(arg0->unkF);
        break;
    case 0x32:
        s = D_8006794C->unk1C->unk8E4.battle->unk11C0->unk14 + arg0->unk8->unkE;
        break;
    default:
        if (arg0->unkF < 0xC0) {
            t = D_800F7124[arg0->unkF];
            switch (t) {
            case 0:
                break;
            case 0xFF:
                s = func_800EA0B4(arg0->unkF);
                break;
            case 0xFE:
                s = buf;
                func_8002BD54(s, func_800EA0B4(arg0->unkF));
                func_8002AD38(s, (u8 *)func_800357C0(D_800F71E4, 0));
                break;
            default:
                if (t < 0xC0) {
                    s = func_800EA0B4(t);
                } else {
                    s = (u8 *)func_800357C0(D_800F71E4, (t & 0x3F) + 1);
                }
                break;
            }
        }
        break;
    }
    if (s != NULL) {
        func_800F4F44((s32)s, 1);
    }
}

void func_800CE05C(Unk800CEE74 *arg0, BtlUnit9C50 *arg1) {
    s32 unused[4];
    s32 i;
    D_800FE514 = arg0;
    D_800FE504 = arg0->unk10;
    D_800FE50C = arg0->unk0;
    func_800CF58C(0);
    D_800FE4F8 = 0;
    if (D_800FE514->unk6 != 0) {
        D_800FE4F8 = 1;
    }
    D_800FE510 = D_800FE514->unk4;
    D_800FE4F4 = arg1;
    D_800FE508 = D_800FE514->unk2;
    if (D_800FE508 != 0) {
        func_800C9374(arg1, 0);
        for (i = 0; i < D_800FE508; i++) {
            BtlUnit9C50 *a;
            BtlUnit9C50 *e;
            a = D_800FE4F4 + i;
            a->unk4 |= 1;
            e = D_800FE4F4 + i;
            e->unkC = 0;
            e->unk12 = 0x1000;
        }
    }
    D_800FE518 = D_800FE514->unkA;
    func_800CEEC8(0xF);
    D_800FE4F0 = 0;
    D_800F7274 = D_8006794C->unk1C->unk570;
    if (D_800F7274 >= 0x1F) {
        D_800F7274 = 0x1F;
    }
}

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
