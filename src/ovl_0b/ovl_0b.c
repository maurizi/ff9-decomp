#include "ovl_0b.h"

void func_800A859C(void) {
    func_800A8CD4();
    do {
        func_800A8600((s16)D_8006794C->unk1C->unkE);
    } while (!(D_8006794C->unk0 & 0x101));
    func_800A8DE0();
}

void func_800A8600(s16 arg0) {
    func_800A923C();
    SetDispMask(1);
    do {
        func_800AEA48();
        if (!(D_8006794C->unk1C->unk8E4.battle->unk0 & 0x1000)) {
            func_800A8BF4();
            if (func_800EA30C() != 0) {
                func_800A882C();
            }
            if (!(D_8006794C->unk1C->unk0 & 0x100)) {
                if (func_800AF2E0() == 1) {
                    D_8006794C->unk1C->unk8E4.battle->unk0 |= 0x1000;
                }
                if (!(D_8006794C->unk1C->unk8E4.battle->unk0 & 0x10) && (D_8006794C->unk1C->unk8E4.battle->unkD85 & 7) == 7) {
                    func_800E9D30();
                    D_8006794C->unk1C->unk8E4.battle->unk0 |= 0x10;
                }
                if (!(D_8006794C->unk1C->unk8E4.battle->unk0 & 0x40) && (D_8006794C->unk1C->unk56F & 0x40)) {
                    func_800AF1D0();
                    D_8006794C->unk1C->unk8E4.battle->unk0 |= 0x40;
                }
                func_80023A4C();
                func_80032098();
                func_8002246C();
            }
        } else {
            D_8006794C->unk8 = 8;
            D_8006794C->unk0 |= 0x100;
        }
        func_800AEE68();
    } while (!(D_8006794C->unk0 & 0x301));
    func_800A94F8();
}

INCLUDE_RODATA("asm/us/ovl_0b/nonmatchings/ovl_0b", D_800A7000);

void func_800A882C(void) {
    GameData *gd = D_8006794C->unk1C;
    u16 f = gd->unk8E4.battle->unk0;
    s32 i;
    s32 w;

    if (!(f & 0x1000)) {
        if (!(gd->unk0 & 0x100) || (f & 0x20)) {
            gd->unk0 |= 0x100;
            func_8005562C(0);
            func_80023F60(1);
            for (i = 1; i >= 0; i--) {
                D_800739B0[i].unk1 = 0;
                D_800739B0[i].unk0 = 0;
            }
            if (D_8006794C->unk1C->unk8E4.battle->unk0 & 0x20) {
                D_80073998[0] |= 8;
            }
            ClearOTag(D_800FDEB0, 1);
            setlen(&D_800FDF18, 2);
            D_800FDF18.code[0] = 0xE6000000;
            D_800FDF18.code[1] = 0;
            addPrim(D_800FDEB0, &D_800FDF18);
            if (D_8006794C->unk1C->unk8E4.battle->unk0 & 0x20) {
                func_80029D60(0x66, 0x6D, D_800FDEB0, "Disc Cover Open", 4, (u8 *)D_8006794C->unk1C->unk8E4.battle->unk11C0->unk34[D_8006794C->unk1C->unk8]);
            } else {
                func_80027834((0x140 - D_800686D0[0xEB].unk4) / 2, (0xDA - D_800686D0[0xEB].unk5) / 2, D_800FDEB0, 0xEB, 0, &w, D_800FDE60);
            }
            setlen(&D_800FDE98, 3);
            D_800FDE98.code = 0x62;
            D_800FDE98.r0 = 0x60;
            D_800FDE98.g0 = 0x60;
            D_800FDE98.b0 = 0x60;
            D_800FDE98.w = 0x140;
            D_800FDE98.h = 0xDA;
            D_800FDE98.x0 = 0;
            D_800FDE98.y0 = 0;
            addPrim(D_800FDEB0, &D_800FDE98);
            setlen(&D_800FDEA8, 1);
            D_800FDEA8.code[0] = 0xE1000240;
            addPrim(D_800FDEB0, &D_800FDEA8);
            D_800FDF24.code[0] = 0xE6000001;
            setlen(&D_800FDF24, 2);
            D_800FDF24.code[1] = 0;
            addPrim(D_800FDEB0, &D_800FDF24);
            DrawOTag(D_800FDEB0);
            D_8006794C->unk1C->unk0 |= 0x20;
            func_800EA338(0);
            return;
        }
    } else if (f & 0x20) {
        return;
    }
    func_80023F60(0);
    func_80055698(0);
    D_80073998[0] &= ~8;
    func_800EA34C();
    D_8006794C->unk1C->unk0 &= ~0x20;
    func_800EA338(0);
    D_8006794C->unk1C->unk0 &= ~0x100;
}

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

void func_800A8E24(void) {
    BattleMem *m;
    D_80067948 = D_80067944 + 0x61800;
    m = (BattleMem *)D_80067948;
    D_8006794C->unk1C->unk8E4.battle = &m->unk0;
    D_80067948 = (s32)m->unk241C;
    m->unk0.unk1354 = (Battle1354 *)m->unk23CC;
    m->unk0.unk1538 = (struct Battle1538 **)m->unk240C;
}

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

void func_800A923C(s32 arg0) {
    Unk800AA08C st;
    Battle11C0 *p;
    s32 i;

    D_80067948 = D_8006794C->unk1C->unk8E4.battle->unk4;
    func_800A9768();
    p = D_8006794C->unk1C->unk8E4.battle->unk11C0;
    func_800A98A0();
    D_8006794C->unk0 &= ~0x30F;
    func_8001CCB4(0, 0, 0);
    D_8006794C->unk1C->unkE = arg0;
    func_800AA08C((Unk800AA08C *)p, arg0);
    func_800A99AC((Unk800AA08C *)p, arg0);
    func_800AA140(p, -1, &st);
    do {
    } while (func_800AA140(p, arg0, &st) != 0);
    func_800A9A7C(p, arg0);
    D_8006794C->unk1C->unk38 = func_8004AAF0(0);
    D_8006794C->unk1C->unk8DC = (struct Unk8D4 *)(p->unk3C + 4);
    D_8006794C->unk1C->unk8E0 = (struct Unk8E0 *)&D_8006794C->unk1C->unk8DC[D_8006794C->unk1C->unk38];
    D_80067948 = (s32)&D_8006794C->unk1C->unk8E0[D_8006794C->unk1C->unk38];
    D_8006794C->unk1C->unk9 = 0;
    for (i = 0; i < 9; i++) {
        func_8001E3A4(&D_8006794C->unk1C->unk8D8[i]);
        D_8006794C->unk1C->unk8D8[i].unk8 = (struct BtlUnit9C *)&D_8006794C->unk1C->unk8D4[i];
    }
    D_8006794C->unk1C->unk39 = 0;
    for (i = 0; i < D_8006794C->unk1C->unk38; i++) {
        func_8001E3A4(&D_8006794C->unk1C->unk8E0[i]);
        D_8006794C->unk1C->unk8E0[i].unk8 = (struct BtlUnit9C *)&D_8006794C->unk1C->unk8DC[i];
    }
    p->unk28 = (struct Battle11C028 *)D_80067948;
    D_80067948 = func_800AD4AC(D_80067948, D_8006794C->unk1C->unk38 + 9);
    p->unk8 = D_80067948;
    func_800E9E10();
    func_800B03B0();
}

void func_800A94F8(void) {
    Battle11C0 *s;
    s32 i;
    s32 v;
    s32 w;

    s = D_8006794C->unk1C->unk8E4.battle->unk11C0;
    DrawSync(0);
    func_800E9E80();
    func_80054F80();
    func_800AB43C();
    func_800AB26C(&D_8006794C->unk1C->unk8E4.battle->unk1354->unk10[0], 2);
    func_800AB26C(&D_8006794C->unk1C->unk8E4.battle->unk1354->unk0[3], 0);
    func_800AB26C(&D_8006794C->unk1C->unk8E4.battle->unk1354->unk0[2], 0);
    for (i = 0; i < 9; i++) {
        if (D_8006794C->unk1C->unk8D8[i].unk0 & 1) {
            D_8006794C->unk1C->unk8D8[i].unk0 &= ~1;
        }
    }
    for (i = 0; i < D_8006794C->unk1C->unk38; i++) {
        if (D_8006794C->unk1C->unk8D8[i].unk0 & 1) {
            D_8006794C->unk1C->unk8D8[i].unk0 &= ~1;
        }
    }
    switch (s->unk6) {
    case 1:
    case 3:
    case 5:
        for (i = 1; i >= 0; i--) {
            w = D_8006794C->unk1C->unk7EC[i];
            D_8006794C->unk1C->unk7FC[i] = w;
            v = D_8006794C->unk1C->unk7F4[i];
            D_8006794C->unk1C->unk804[i] = v;
        }
        if (s->unk6 == 3) {
            D_8006794C->unk1C->unk7DC = s->unk4;
        } else {
            D_8006794C->unk1C->unkC = s->unk4;
        }
        break;
    case 8:
        break;
    default:
        func_8002EB64(0x39, 0x12E);
        break;
    }
}

void func_800A9768(void) {
    Battle11C0 *p;
    s32 i;
    D_8006794C->unk1C->unk8E4.battle->unk11C0 = (Battle11C0 *)D_80067948;
    p = (Battle11C0 *)D_80067948;
    D_80067948 = (s32)(p + 1);
    p->unk24 = (u16 *)((s32)D_8006794C->unk1C->unk8E4.battle + 0x5800);
    p->unk1C = (s32)p->unk24 + 0x2800;
    p->unk44 = (u8 *)(p->unk1C + 0x6000);
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

void func_800A98A0(void) {
    Battle11C0 *p = D_8006794C->unk1C->unk8E4.battle->unk11C0;
    s32 i;
    p->unk0 = 0;
    p->unk8 = 0;
    p->unk6 = D_8006794C->unkA;
    if (D_8006794C->unkA == 1) {
        p->unk4 = D_8006794C->unk1C->unkC;
    } else if (D_8006794C->unkA == 3) {
        p->unk4 = D_8006794C->unk1C->unk7DC;
    } else {
        p->unk4 = -1;
    }
    p->unk20 = NULL;
    p->unk10 = 0;
    p->unk14 = NULL;
    p->unk18 = 0;
    p->unk40 = NULL;
    p->unk28 = NULL;
    p->unk48 = p->unk44;
    for (i = 0; i < sizeof(p->unk7); i++) {
        p->unk7[i] = 0;
    }
    for (i = 0; i < 4; i++) {
        D_8006794C->unk1C->unk8E4.battle->unk1538[i]->unk20 = D_8006794C->unk1C->unk8E4.battle->unk1538[i]->unk24 = D_8006794C->unk1C->unk8E4.battle->unk153C[i];
    }
}

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

s32 func_800A9A7C(Battle11C0 *arg0, s32 arg1) {
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
        func_800C9374((TexAnimSet *)arg0->unk40, NULL);
    }
    DrawSync(0);
    D_8006794C->unk1C->unk8E4.battle->unk0 |= 1;
    return 0;
}

s32 func_800A9BE4(s32 arg0) {
    Battle *b = D_8006794C->unk1C->unk8E4.battle;
    s16 *state = (s16 *)&b->unk11C0->unkC[0x9FD8 / 4];
    Unk800AA958 *ld = (Unk800AA958 *)state - 1;
    s32 i;
    Character *ch;
    s32 cid;
    Unk8E0 *e;
    BtlUnit *u;
    s32 id;
    s32 k;
    s32 id2;
    s32 *pa;
    s32 *pb;
    s32 *pc;
    s32 p;
    s32 v;
    BtlUnit9C *m;
    u8 save;

    if (arg0 & 0x8000) {
        b->unk0 &= ~4;
        *state = 1;
    }
    switch (*state) {
    case 1:
        if (arg0 & 0x200) {
            func_800AA24C(D_8006794C->unk1C->unkE, 0x8000, (Unk800AA24C *)ld);
            *state = 2;
            break;
        }
        *state = 3;
        break;
    case 2:
        if (func_800AA24C(D_8006794C->unk1C->unkE, arg0, (Unk800AA24C *)ld) == 0) {
            *state = 3;
        }
        break;
    case 3:
        func_800AA958(0x8000, ld);
        *state = 4;
        break;
    case 4:
        if (func_800AA958(arg0, ld) == 0) {
            *state = 5;
        }
        break;
    case 5:
        func_800AABC4(0x8000, ld);
        *state = 6;
        break;
    case 6:
        if (func_800AABC4(arg0, ld) == 0) {
            if (arg0 & 4) {
                *state = 7;
            } else {
                goto done;
            }
        }
        break;
    case 7:
        for (i = 0; i < 4; i++) {
            ch = D_8006794C->unk1C->unk54C[i];
            if (ch == NULL) {
                continue;
            }
            cid = ch->unk34.b.id;
            e = &D_8006794C->unk1C->unk8D8[cid];
            u = func_800C1D8C(i);
            if (u == NULL) {
                continue;
            }
            id = func_800ABA10(i, arg0 & ~0x10);
            e->unk8->unk8 = func_8001C7FC(4, id, 0);
            k = (id & 0xFFFFFF) | (cid << 24);
            pa = (s32 *)func_800634A8(k);
            pb = (s32 *)func_800634D8(k);
            pc = (s32 *)func_80063514(k);
            func_800C7730(e->unk8, (s16 *)pa, (s16 *)pb);
            p = (s32)func_8001C7FC(9, id, 0x190000);
            if (p != -1) {
                v = (s32)func_8001C7FC(4, p, 0);
            } else {
                v = 0;
            }
            e->unk8->unk50 = (BtlUnit9C50 *)v;
            if (v != 0) {
                func_800C9374((TexAnimSet *)e->unk8->unk50, (u16 (*)[4])pc);
            }
            func_800BD698(u, D_8006794C->unk1C->unk54C[i]->unk34.b.unk4, u - D_8006794C->unk1C->unk8E4.battle->unk6FC);
            if (u->unk3C & 0x100) {
                if (u != D_8006794C->unk1C->unk8E4.battle->unk1088->unk4) {
                    u->unk9C->unkC = (BtlUnit9CC *)u->unkA4[4];
                } else {
                    goto use9;
                }
            } else if (u == D_8006794C->unk1C->unk8E4.battle->unk1088->unk4 || ((u->unk68 >> 19) & 1)) {
            use9:
                u->unk9C->unkC = (BtlUnit9CC *)u->unkA4[9];
            } else {
                u->unk9C->unkC = (BtlUnit9CC *)u->unkA4[(u->unk68 >> 18) & 1];
            }
            e->unkC->unk3A = 0;
            m = u->unkA8;
            save = m->unk4;
            id2 = ((Unk8006E0E8 *)u->unk74)->unk2 | 0x20000;
            m->unk8 = (BtlUnit9C8 *)func_800ABB60(i);
            func_800AB758(id2, i, &pa, &pb, &pc);
            func_800C7730(m, (s16 *)pa, (s16 *)pb);
            m->unkC = 0;
            m->unk1C = e->unk8;
            m->unk4 = save;
        }
    done:
        D_8006794C->unk1C->unk8E4.battle->unk0 |= 4;
        *state = 0;
        break;
    }
    return *state;
}

/* padded: a zero word follows func_800A9BE4's jump table */
__asm__(".section .rodata\n\t.word 0\n\t.section .text");

s32 func_800AA08C(Unk800AA08C *arg0, s32 arg1) {
    s32 v = arg0->unk24;
    GameData *gd = D_8006794C->unk1C;
    u8 *cd = gd->unk854;
    Battle *b = gd->unk8E4.battle;
    func_8002177C(cd, gd->unk7FC[0], gd->unk804[0] << 11, (void *)arg0->unk24, 0, 0x2800, 0);
    while (func_800217C4(cd) != 0) {
    }
    b->unk1354->unk0[2] = (void *)v;
    func_8001C750(v, 0);
    gd->unk7E4 = (FileTable *)func_8001C8B0(v, 2, 0x140000, 0);
    return 0;
}

s16 func_800AA140(Battle11C0 *arg0, s32 arg1, Unk800AA08C *arg2) {
    GameData *gd = D_8006794C->unk1C;
    Battle *b = gd->unk8E4.battle;
    u8 *cd = gd->unk854;
    Unk800AA08C *st;
    s32 v;
    GameData *gd2;
    st = arg2;
    if (arg1 == -1) {
        b->unk0 &= ~1;
        st->unk0 = 2;
    }
    if (st->unk0 == 1) {
        if (func_800217C4(cd) == 0) {
            st->unk0 = st->unk2;
        }
    } else if (st->unk0 == 2) {
        gd2 = D_8006794C->unk1C;
        st->unk24 = arg0->unk1C;
        func_8002177C(cd, gd2->unk7FC[1], gd2->unk804[1] << 11, (void *)st->unk24, 0, -1, 0);
        st->unk2 = 4;
        st->unk0 = 1;
    } else if (st->unk0 == 4) {
        v = arg0->unk1C;
        b->unk1354->unk0[3] = (void *)v;
        func_8001C750(v, 0);
        b->unk0 |= 1;
        st->unk0 = 0;
    }
    return st->unk0;
}

s32 func_800AA24C(s16 arg0, s32 arg1, Unk800AA24C *arg2) {
    Unk800AA24C *st;
    GameData *gd;
    Battle *b;
    CdReadCtx *cd;
    s16 *hb;
    s32 n;
    s16 *p;
    s32 sz;
    s32 h;

    st = arg2;
    gd = D_8006794C->unk1C;
    b = gd->unk8E4.battle;
    cd = gd->unk854;
    if (arg1 & 0x8000) {
        b->unk0 &= ~2;
        st->unk0 = 2;
    }
loop:
    switch (st->unk0) {
    case 1:
        if (func_800217C4(cd) != 0) {
            break;
        }
        func_8001DA78(&arg2->unk24, cd->unk10);
        st->unk0 = arg2->unk2;
        goto loop;
    case 2:
        arg2->unk24 = b->unk153C[0];
        arg2->unk8.w[0] = -1;
        arg2->unk8.w[1] = -1;
        arg2->unk8.w[2] = -1;
        arg2->unk8.w[3] = -1;
        arg2->unk8.h[0] = 0x201;
        if (arg1 & 0x100) {
            func_800AA24C_collect(&arg2->unk8.h[4], 9, 2);
        }
        st->unk0 = 5;
        goto loop;
    case 5:
        hb = arg2->unk8.h;
        n = func_800AAEF0(0xB, (u16 *)hb, (u16 *)&arg2->unk8.h[6], (u16 **)&p, &sz);
        if (n != -1) {
            func_8002177C(cd, n, sz << 11, (void *)arg2->unk24, 0, -1, 0);
            if (*p == 0x201) {
                arg2->unk4 = -1;
            } else {
                arg2->unk4 = p - hb;
            }
            *p = -1;
            arg2->unk2 = 6;
            st->unk0 = 1;
            break;
        }
        st->unk0 = 7;
        goto loop;
    case 6:
        if (func_80055DCC() != 0) {
            break;
        }
        h = func_8001C8B0((s32)cd->unk8, 2, 0x90000, 0);
        if (arg2->unk4 == -1) {
            func_80055D80(h, 0);
        } else {
            func_80056048(h, arg2->unk4 & 3, 0);
        }
        st->unk0 = 5;
        goto loop;
    case 7:
        if (func_80055DCC() != 0) {
            break;
        }
        st->unk0 = 0;
        break;
    }
    return st->unk0;
}

/* padded: a zero word follows func_800AA24C's jump table */
__asm__(".section .rodata\n\t.word 0\n\t.section .text");

s32 func_800AA504(s32 arg0, s32 arg1, Unk800AA504 *arg2) {
    Unk800AA504 *st;
    Battle *b;
    Unk800AA958Req *req;
    FileTable *t;
    FileEntryX *first;
    FileEntryX *e;
    s16 *s;
    u32 w;
    u32 best;
    s16 *bp;
    FileEntry *bf;
    s16 *p;
    FileEntry *f;
    s32 sp1C;
    s32 dst;
    Unk800AA504Tbl *tbl;
    Unk800AA504Ent *te;
    Unk800AA504Pos *a;
    Unk800AA504Pos8 *r;
    Unk800AA504Ent *tf;
    Unk800AA504Pos *bb;
    s32 n;
    s32 h;
    s32 x;
    u16 id;
    s16 *lo;
    s16 *ids;
    s32 cnt;

    st = arg2;
    b = D_8006794C->unk1C->unk8E4.battle;
    req = (Unk800AA958Req *)&b->unk11C0->unkC[0x9FA0 / 4];
    if (arg1 & 0x8000) {
        b->unk0 &= ~2;
        st->unk0 = 2;
    }
loop:
    switch (st->unk0) {
    case 1:
        if (func_80022B18() != 0) {
            break;
        }
        st->unk0 = arg2->unk2;
        goto loop;
    case 2:
        t = D_8006794C->unk1C->unk7E4;
        ids = arg2->unk10.h;
        cnt = 0;
        arg2->unkA = 0;
        arg2->unkC = b->unk153C[0];
        arg2->unk10.w[0] = -1;
        arg2->unk10.w[1] = -1;
        first = (FileEntryX *)t->unk4;
        e = &first[t->unk2] - 1;
        do {
            w = e->unk0.w;
            if (e->unk0.b.type == 2) {
                w &= 0xFFFF;
                s = &ids[cnt] - 1;
                do {
                    if (*s == w) {
                        goto next;
                    }
                    s--;
                } while (s >= ids);
                ids[cnt++] = w;
            }
        next:
            e--;
        } while (e >= first);
        st->unk0 = 4;
        goto loop;
    case 4:
        best = -1;
        bp = NULL;
        bf = NULL;
        lo = arg2->unk10.h;
        p = &arg2->unk10.h[2];
        do {
            id = *p;
            if (id != 0xFFFF) {
                f = func_80063088(D_8006794C->unk1C->unk7E4, id | 0x20000);
                if ((u32)f->unk4 < best) {
                    best = f->unk4;
                    bf = f;
                    bp = p;
                }
            }
            p--;
        } while (p >= lo);
        if (bp != NULL) {
            arg2->unk8 = *bp;
            *bp = -1;
            req->unk0 = bf->unkC << 11;
            req->unk4 = (s32)b->unk11C0->unk48;
            req->unk8 = (bf->unk8 - bf->unkC) << 11;
            req->unkC = arg2->unkC;
            func_80022CC4(D_8006794C->unk20 + best, req, 2, 0);
            arg2->unk2 = 6;
            st->unk0 = 1;
        } else {
            st->unk0 = 7;
        }
        break;
    case 6:
        sp1C = func_8001C8B0((s32)b->unk11C0->unk48, 2, 0x1B0000, 0);
        dst = arg2->unkC;
        tbl = (Unk800AA504Tbl *)func_8001C8B0((s32)b->unk1354->unk0[2], 2, 0x120000, 0);
        tf = tbl->unk4;
        te = &tf[tbl->unk1] - 1;
        do {
            if (te->unk0 == arg2->unk8) {
                n = te->unk4.b[3];
                a = (Unk800AA504Pos *)((u8 *)te + (te->unk4.w & 0xFFFFFF));
                bb = a + n;
                r = (Unk800AA504Pos8 *)(bb + n);
                while (n > 0) {
                    n--;
                    x = func_8001C8B0(dst, 2, (n & 0xFFFF) | 0x40000, 0);
                    h = func_8001DA60(a[n].unk2);
                    func_8001D9CC(x, h, func_8001DA6C(a[n].unk2), r[n].unk0, r[n].unk2);
                    func_8001D8A8(x);
                }
                break;
            }
            te--;
        } while (te >= tf);
        D_80067948 = sp1C + func_8001CA70(sp1C);
        b->unk11C0->unk48 = (u8 *)D_80067948;
        arg2->unkC += func_8001CA70(dst);
        *(b->unk1354->unk10 + arg2->unkA) = (void *)sp1C;
        func_8001C750(sp1C, 0);
        arg2->unkA++;
        st->unk0 = 4;
        goto loop;
    case 7:
        b->unk0 |= 2;
        st->unk0 = 0;
        break;
    }
    return st->unk0;
}

/* padded: a zero word follows func_800AA504's jump table */
__asm__(".section .rodata\n\t.word 0\n\t.section .text");

s32 func_800AA958(s32 arg0, Unk800AA958 *arg1) {
    Unk800AA958 *st;
    Battle *b;
    Unk800AA958Req *req;
    u16 *p;
    u16 *q;
    s32 n;
    s32 sz;
    s32 i;
    s16 j;
    void **pp;
    s32 v;
    Character **c;
    Character **e;
    GameData *gd;

    st = arg1;
    b = D_8006794C->unk1C->unk8E4.battle;
    req = (Unk800AA958Req *)&b->unk11C0->unkC[0x9FA0 / 4];
    if (arg0 & 0x8000) {
        b->unk0 &= ~4;
        st->unk0 = 2;
    }
loop:
    switch (st->unk0) {
        case 1:
            if (func_80022B18() != 0) {
                goto end;
            }
            st->unk0 = arg1->unk2;
            goto loop;
        case 2:
            arg1->unk10.w[0] = -1;
            arg1->unk10.w[1] = -1;
            gd = D_8006794C->unk1C;
            e = gd->unk54C;
            c = &gd->unk54C[3];
            q = &arg1->unk10.h[3];
            do {
                if (*c != NULL) {
                    *q = D_8006E0E8[(*c)->unk39[0]].unk2;
                }
                c--;
                q--;
            } while (c >= e);
            st->unk0 = 5;
            goto loop;
        case 5:
            n = func_800AAEF0(8, arg1->unk10.h, &arg1->unk10.h[3], &p, &sz);
            if (n != -1) {
                *(s16 *)p = -1;
                i = p - arg1->unk10.h;
                arg1->unkA = i;
                arg1->unk24 = b->unk11C0->unk3C[i];
                req->unk0 = 0x1000;
                req->unk4 = arg1->unk24;
                req->unk8 = (sz << 11) - 0x1000;
                req->unkC = b->unk153C[i];
                func_80022CC4(D_8006794C->unk20 + n, req, 2, 0);
                arg1->unk2 = 7;
                st->unk0 = 1;
            } else {
                st->unk0 = 8;
            }
            goto end;
        case 7:
            j = arg1->unkA;
            v = func_800AB07C(arg1, req, j, 0);
            pp = &b->unk1354->unk20[j];
            *pp = (void *)v;
            func_8001C750(v, 0);
            st->unk0 = 5;
            goto loop;
        case 8:
            st->unk0 = 0;
        default:
            goto end;
    }
end:
    return st->unk0;
}

s32 func_800AABC4(s32 arg0, Unk800AA958 *arg1) {
    s32 i;
    Unk800AA958 *st;
    Battle *b;
    Unk800AA958Req *req;
    s16 *h;
    s16 *p;
    s16 *q;
    s16 *p2;
    s32 k;
    s32 sz;
    s32 sz2;
    s32 n;
    s16 j;
    s32 v;
    void **pp;
    FileEntry16 *f;

    st = arg1;
    b = D_8006794C->unk1C->unk8E4.battle;
    req = (Unk800AA958Req *)&b->unk11C0->unkC[0x9FA0 / 4];
    if (arg0 & 0x8000) {
        b->unk0 &= ~4;
        st->unk0 = 1;
    }
loop:
    switch (st->unk0) {
    case 2:
        if (func_80022B18() != 0) {
            goto end;
        }
        st->unk0 = arg1->unk2;
        goto loop;
    case 1:
        h = (s16 *)arg1->unk10.h;
        q = (s16 *)&arg1->unk10.h[3];
        arg1->unk10.w[0] = -1;
        arg1->unk10.w[1] = -1;
        arg1->unk10.w[2] = 0;
        arg1->unk10.w[3] = 0;
        do {
            *q = func_800ABA10(q - h, arg0 & ~0x10);
            q--;
        } while (q >= h);
        st->unk0 = 5;
        goto loop;
    found:
        req->unkC = b->unk153C[k];
        goto done;
    case 5:
        h = (s16 *)arg1->unk10.h;
        q = (s16 *)&arg1->unk10.h[3];
        n = func_800AAEF0(0xA, (u16 *)h, (u16 *)q, (u16 **)&p, &sz);
        if (n != -1) {
            i = -1;
            f = (FileEntry16 *)func_8001E218(0xA, *p);
            p[0] = i;
            p[4] = i;
            arg1->unkA = p - h;
            arg1->unk24 = b->unk153C[arg1->unkA];
            req->unkC = 0;
            if (func_800AAEF0(0xA, (u16 *)h, (u16 *)q, (u16 **)&p2, &sz2) != i) {
                q = (s16 *)&arg1->unk10.h[7];
                i = p2 - h;
                h = (s16 *)&arg1->unk10.h[4];
                do {
                    if (*q != -1) {
                        k = q - h;
                        if (k != i) {
                            goto found;
                        }
                    }
                    q--;
                } while (q >= h);
            }
        done:
            if (req->unkC == 0) {
                req->unkC = (s32)b->unk11C0->unkC;
            }
            req->unk0 = f->unk2 << 11;
            req->unk4 = arg1->unk24;
            req->unk8 = (sz - f->unk2) << 11;
            func_80022CC4(D_8006794C->unk20 + n, req, 2, 0);
            arg1->unk2 = 7;
            st->unk0 = 2;
        } else {
            st->unk0 = 8;
        }
        goto end;
    case 7:
        j = arg1->unkA;
        v = func_800AB07C(arg1, req, j, 1);
        pp = &D_8006794C->unk1C->unk8E4.battle->unk1354->unk30[j];
        *pp = (void *)v;
        func_8001C750(v, 0);
        st->unk0 = 5;
        goto loop;
    case 8:
        st->unk0 = 0;
    default:
        goto end;
    }
end:
    return st->unk0;
}

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

s32 func_800AB3E4(s32 arg0, s32 arg1) {
    s32 v = 0x17;
    if (arg1 != 0) {
        v = 0x8017;
    }
    return func_800ABBC4(arg0, v);
}

s32 func_800AB410(s32 arg1) {
    s32 v = 0x207;
    if (arg1 != 0) {
        v = 0x8207;
    }
    return func_800A9BE4(v);
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

s16 func_800AB580(s32 arg0) {
    GameData *g = D_8006794C->unk1C;
    s16 *state = (s16 *)&g->unk8E4.battle->unk11C0->unkC[0x9FD8 / 4];
    s16 *st;
    s32 id = (s16)g->unkE;
    Unk800AA958 *ld;
    s16 s;

    ld = (Unk800AA958 *)state - 1;
    st = state;
    if (arg0 != 0) {
        func_800AA24C(id, 0x8100, (Unk800AA24C *)ld);
        *state = 1;
    }
    s = *state;
    if (s == 1) {
        if (func_800AA24C(id, 0x100, (Unk800AA24C *)ld) == 0) {
            func_800AA504(id, 0x8000, (Unk800AA504 *)ld);
            (*st)++;
        }
    } else if (s == 2) {
        if (func_800AA504(id, 0, (Unk800AA504 *)ld) == 0) {
            func_800AA958(0x8000, ld);
            (*st)++;
        }
    } else if (s == 3) {
        if (func_800AA958(0x22, ld) == 0) {
            func_800AABC4(0x8020, ld);
            (*st)++;
        }
    } else if (s == 4) {
        if (func_800AABC4(0x22, ld) == 0) {
            D_8006794C->unk1C->unk8E4.battle->unk0 |= 4;
            *state = 0;
        }
    }
    return *st;
}

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

s32 func_800ABA10(s32 arg0, s32 arg1) {
    BtlUnit *u;
    s32 m;
    s32 idx;

    m = 0;
    if (D_8006794C->unk1C->unk54C[arg0] == NULL) {
        return -1;
    }
    if (!(arg1 & 0x20)) {
        u = func_800C1D8C(arg0);
        if (u != NULL) {
            m = ((s16)u->unk84 != D_800F5200[(u->unk68 & 1) ? (D_8006794C->unk1C->unk3C[(u->unk68 >> 1) & 0xF].unk34.w >> 4) & 0x1F : 0x13]) << 4;
        }
    }
    idx = D_8006794C->unk1C->unk54C[arg0]->unk34.w >> 4;
    idx &= 0x1F;
    if ((arg1 & 0x10) != m) {
        if (!(arg1 & 0x20)) {
            idx += 0x13;
        }
    }
    return D_800F5200[idx] | 0x20000;
}

s32 func_800ABB60(s32 arg0) {
    s32 v = D_8006794C->unk1C->unk8E4.battle->unk11C0->unk3C[arg0];
    return func_8001C8B0(func_8001C8B0(v, 2, 0x1B0000, 0), 2, 0x20000, 0);
}

s32 func_800ABBC4(s32 arg0, s32 arg1) {
    Unk800AA958 *st;
    s16 *state;
    Unk800AA958Req *req;
    s32 k;
    void *v;
    FileEntry16 *f;
    s32 id;
    s32 id2;
    s32 model;
    s32 img;
    Unk800ABBC4Tbl *t;
    Unk800ABBC4Ent *e;
    Unk800ABBC4Tex *a;
    Unk800ABBC4Tex *b;
    Unk800ABBC4Rect *r;
    s32 i;
    s32 j;
    s32 n;
    s32 x;
    s32 y;
    s32 h;
    s32 s;
    s32 cid;
    Unk8E0 *ent;
    BtlUnit *u;
    u8 *pa;
    u8 *pb;
    u8 *pc;
    s32 p;
    s32 w;
    u8 *src;
    s32 sz;
    s32 anim;

    st = (Unk800AA958 *)&D_8006794C->unk1C->unk8E4.battle->unk11C0->unkC[0x9FD8 / 4];
    state = &st->unk0;
    req = (Unk800AA958Req *)&D_8006794C->unk1C->unk8E4.battle->unk11C0->unkC[0x9FA0 / 4];
    if (arg1 & 0x8000) {
        st->unk0 = 1;
    }
    switch (*state) {
    case 1:
        k = arg0;
        v = (void *)D_8006794C->unk1C->unk8E4.battle->unk153C[k];
        st->unk2 = 0;
        st->unk4 = 0;
        st->unk6 = 0;
        st->unk8 = -1;
        st->unkE = 0;
        st->unk24 = (s32)v;
        st->unk20 = (s32)v;
        if (*(D_8006794C->unk1C->unk8E4.battle->unk1354->unk30 + k) != NULL) {
            func_8001C784(*(D_8006794C->unk1C->unk8E4.battle->unk1354->unk30 + k));
            *(D_8006794C->unk1C->unk8E4.battle->unk1354->unk30 + k) = NULL;
        }
        *state = 3;
        break;
    case 2:
        if (func_80022B18() == 0) {
            *state = st->unk2;
        }
        break;
    case 3:
        f = (FileEntry16 *)func_8001E218(0xA, func_800ABA10(arg0, arg1 & ~0x10));
        src = D_8006794C->unk20 + f->unk4;
        sz = f->unkC - f->unk4;
        req->unk0 = f->unk2 << 11;
        req->unk4 = st->unk24;
        req->unk8 = (sz - f->unk2) << 11;
        req->unkC = (s32)D_8006794C->unk1C->unk8E4.battle->unk11C0->unkC;
        func_80022CC4((s32)src, req, 2, 0);
        st->unk2 = 5;
        *state = 2;
        break;
    case 5:
        id = func_800ABA10(arg0, arg1 & ~0x10);
        model = func_8001C8B0(st->unk24, 2, 0x1B0000, 0);
        img = (s32)D_8006794C->unk1C->unk8E4.battle->unk11C0->unkC;
        t = (Unk800ABBC4Tbl *)func_8001C8B0(model, 2, 0x120000, 0);
        e = t->unk4;
        for (i = 0; i < t->unk1; i++) {
            if (e[i].unk0 == (u16)id) {
                e = &e[i];
                n = e->unk4.b[3];
                a = (Unk800ABBC4Tex *)((u8 *)e + (e->unk4.w & 0xFFFFFF));
                b = a + n;
                r = (Unk800ABBC4Rect *)(b + n);
                for (j = 0; j < n; j++) {
                    w = func_8001C8B0(img, 2, (j & 0xFFFF) | 0x40000, 0);
                    s = r[j].unk2 + 0xE5;
                    y = arg0 * 3 + s;
                    h = (r[j].unk2 << 7) + 0x80;
                    x = (arg0 << 6) + 0xC0;
                    func_8001D9CC(w, 0x100, y, x, h);
                    func_8001D8A8(w);
                    a[j].unk0 = ((h & 0x100) >> 4) | (s16)(((x & 0x3FF) >> 6) | 0x80) | ((h & 0x200) << 2);
                    a[j].unk2 = (y << 6) | 0x10;
                    b[j].unk0 = 0;
                    b[j].unk2 = h % 256 - h % 128;
                    r[j].unk4 = x;
                    r[j].unk0 = x;
                    r[j].unk6 = h;
                    r[j].unk2 = h;
                }
                break;
            }
        }
        *(D_8006794C->unk1C->unk8E4.battle->unk1354->unk30 + arg0) = (void *)model;
        func_8001C750(model, 0);
        *state = 6;
        break;
    case 6:
        cid = D_8006794C->unk1C->unk54C[arg0]->unk34.b.id;
        ent = &D_8006794C->unk1C->unk8D8[cid];
        u = func_800C1D8C(arg0);
        if (u != NULL) {
            id2 = func_800ABA10(arg0, arg1 & ~0x10);
            ent->unk8->unk8 = func_8001C7FC(4, id2, 0);
            k = (id2 & 0xFFFFFF) | (cid << 24);
            pa = func_800634A8(k);
            pb = func_800634D8(k);
            pc = func_80063514(k);
            func_800C7730(ent->unk8, (s16 *)pa, (s16 *)pb);
            p = (s32)func_8001C7FC(9, id2, 0x190000);
            if (p != -1) {
                anim = (s32)func_8001C7FC(4, p, 0);
            } else {
                anim = 0;
            }
            ent->unk8->unk50 = (BtlUnit9C50 *)anim;
            if (anim != 0) {
                func_800C9374((TexAnimSet *)ent->unk8->unk50, (u16 (*)[4])pc);
            }
            func_800BD698(u, D_8006794C->unk1C->unk54C[arg0]->unk34.b.unk4,
                          u - D_8006794C->unk1C->unk8E4.battle->unk6FC);
            if (u->unk3C & 0x100) {
                u->unk9C->unkC = (BtlUnit9CC *)u->unkA4[4];
            } else {
                u->unk9C->unkC = (BtlUnit9CC *)u->unkA4[(u->unk68 >> 18) & 1];
            }
            ent->unkC->unk3A = 0;
            u->unkA8->unk1C = ent->unk8;
        }
        *state = 0;
        break;
    }
    return *state;
}

s32 func_800AC244(s32 arg0) {
    u16 *p = func_800633E4(arg0);
    if (p != NULL) {
        return p[1] | 0x30000;
    }
    return -1;
}

s32 func_800AC280(u32 arg0, s32 **arg1, s32 **arg2, s32 **arg3) {
    Unk800AB758 *h;
    u8 *dst;
    s32 n;

    h = (Unk800AB758 *)func_800633E4(arg0);
    *arg1 = (s32 *)((u8 *)h + h->unk4);
    *arg2 = *arg1 + h->unk7;
    *arg3 = *arg2 + h->unk7;
    if ((s32)(arg0 >> 24) >= 9) {
        n = h->unk7 * 4;
        dst = D_8006794C->unk1C->unk8E4.battle->unk11C0->unk48;
        func_80015C28(dst, (u8 *)*arg1, n);
        *arg1 = (s32 *)dst;
        D_8006794C->unk1C->unk8E4.battle->unk11C0->unk48 += n;
    }
    return 0;
}

INCLUDE_ASM("asm/us/ovl_0b/nonmatchings/ovl_0b", func_800AC37C);

s32 func_800AD4AC(s32 arg0, s32 arg1) {
    return func_80062E3C(arg0, 0x100, 0xFF, 0x200, 0xE0, arg1, 0);
}

void func_800AD4E4(Unk800AD4E4 *arg0, BtlObj *arg1, MATRIX *arg2, s32 arg3, s32 arg4, s32 arg5) {
    VECTOR r;
    VECTOR up;
    VECTOR n;
    VECTOR d;
    MATRIX m;
    SVECTOR v;
    s32 *p0;
    s32 *p1;

    if (arg4 == arg5) {
        COPY_MATRIX(&m, &((MATRIX *)arg1->unk8->unk20)[arg4]);
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
        gte_stlvnl(&d);
        d.vy = 0;
        VectorNormal(&d, &n);
        n.vx = -n.vx;
        n.vy = -n.vy;
        n.vz = -n.vz;
    } else {
        p0 = ((MATRIX *)arg1->unk8->unk20)[arg4].t;
        p1 = ((MATRIX *)arg1->unk8->unk20)[arg5].t;
        d.vx = p1[0] - p0[0];
        d.vy = p1[1] - p0[1];
        d.vz = p1[2] - p0[2];
        if (d.vx == 0 && d.vy == 0 && d.vz == 0) {
            n.vx = 0;
            n.vy = 0;
            n.vz = 0x1000;
        } else {
            d.vy = 0;
            VectorNormal(&d, &n);
        }
    }
    up.vy = 0x1000;
    up.vx = 0;
    up.vz = 0;
    gte_ldopv1(&up);
    gte_ldopv2(&n);
    gte_op12();
    gte_stlvnl(&r);
    arg2->m[0][0] = (r.vx * arg0->unk0_8) >> 12;
    arg2->m[1][0] = (r.vy * arg0->unk0_8) >> 12;
    arg2->m[2][0] = (r.vz * arg0->unk0_8) >> 12;
    arg2->m[0][1] = up.vx;
    arg2->m[1][1] = up.vy;
    arg2->m[2][1] = up.vz;
    arg2->m[0][2] = (n.vx * arg0->unk0_20) >> 12;
    arg2->m[1][2] = (n.vy * arg0->unk0_20) >> 12;
    arg2->m[2][2] = (n.vz * arg0->unk0_20) >> 12;
    arg2->t[0] = (((r.vx << 1) * arg0->unk4_12 + (n.vx << 1) * arg0->unk4_22) >> 12) +
                 ((MATRIX *)arg1->unk8->unk20)[arg4].t[0];
    arg2->t[1] = (((r.vy << 1) * arg0->unk4_12 + (n.vy << 1) * arg0->unk4_22) >> 12) + arg3;
    arg2->t[2] = (((r.vz << 1) * arg0->unk4_12 + (n.vz << 1) * arg0->unk4_22) >> 12) +
                 ((MATRIX *)arg1->unk8->unk20)[arg4].t[2];
}

s32 func_800AD90C(s32 arg0, s32 arg1, s32 arg2, s32 arg3, s32 arg4) {
    s32 size;
    GameData *gx, *ga, *gb, *gc, *gd, *ge, *gf, *gg, *gh, *gi, *gj;
    Battle1538 *e;
    s32 kind;
    s32 id;
    s32 r;
    s32 h;
    s32 n;
    s32 m;
    s32 k1;
    s32 k2;
    s32 m2;

    kind = (arg0 >> 8) & 0xF;
    if (kind == 12 && D_8006794C->unk1C->unk851 != 0) {
        goto stream;
    }
    switch ((arg0 >> 12) & 3) {
    case 0:
        id = (arg1 & 0xFFFF) | 0x70000;
        switch (kind) {
        case 0:
        case 7:
            if (D_8006794C->unk1C->unk851 != 0) {
                return -1;
            }
            if (D_800F51F8 == -1) {
                D_800F51F8 = kind;
            }
            r = func_800AE764(id);
            ga = D_8006794C->unk1C;
            if (ga->unk81C[0] != arg1 || ga->unk820[0] != (u16)r) {
                if (ga->unk0 & 0x20000000) {
                    func_800552B8(ga->unk824[0]);
                }
                D_8006794C->unk1C->unk0 &= 0xCFFFFFFF;
                D_8006794C->unk1C->unk820[0] = r;
                D_8006794C->unk1C->unk81C[0] = arg1;
                if ((id & 0xFFFF) != 5) {
                    D_8006794C->unk1C->unk8C0 = (s32)(D_80067944 + 0x140700);
                    D_800F51EC = -1;
                    D_8006794C->unk1C->unk878 = 0x16800;
                    D_800F51F0 = id;
                    D_8006794C->unk1C->unk87C = 0;
                    D_8006794C->unk1C->unk851 = 2;
                    return D_8006794C->unk1C->unk851;
                }
                memmove(D_8006794C->unk1C->unk814, (void *)D_8006794C->unk1C->unk818, 0x2000);
                D_8006794C->unk1C->unk826[0] = 0x7F;
                D_8006794C->unk1C->unk0 |= 0x10000000;
            }
        finish:
            if (D_800F51F8 != 7) {
                D_8006794C->unk1C->unk824[0] = func_80055290(D_8006794C->unk1C->unk814);
                gb = D_8006794C->unk1C;
                if (!(gb->unk0 & 0x20000000)) {
                    func_80055A34(gb->unk824[0], 0x1E, 0, gb->unk826[0]);
                }
                D_8006794C->unk1C->unk0 |= 0x30000000;
            }
            D_800F51F8 = -1;
            return 0;
        case 1:
            func_800552B8(D_8006794C->unk1C->unk824[0]);
            D_8006794C->unk1C->unk826[0] = 0x7F;
            D_8006794C->unk1C->unk0 &= 0xDFFFFFFF;
            return 0;
        case 2:
            if ((arg0 & 0x3F) != 1) {
                break;
            }
            func_800559C8(D_8006794C->unk1C->unk824[0], arg2);
            D_8006794C->unk1C->unk826[0] = arg2;
            return 0;
        case 3:
            if ((arg0 & 0x3F) != 1) {
                break;
            }
            func_800559FC(D_8006794C->unk1C->unk824[0], arg2, arg3);
            func_800AE9C8(arg3, arg2);
            return 0;
        case 4:
            if ((arg0 & 0x3F) != 1) {
                break;
            }
            func_80055A34(D_8006794C->unk1C->unk824[0], arg2, arg3, arg4);
            func_800AE9C8(arg4, arg2);
            return 0;
        case 6:
            if ((arg0 & 0x3F) != 1) {
                break;
            }
            func_800559FC(0, arg2, arg3);
            func_800AE9C8(arg3, arg2);
            return 0;
        case 10:
            if ((arg0 & 0x3F) != 6) {
                break;
            }
            func_80055604(arg1);
            return 0;
        case 12:
            goto stream;
        }
        return -1;
    case 1:
        id = (arg1 & 0xFFFF) | 0x80000;
        h = 0;
        if (kind != 5 && kind != 6) {
            h = (s32)func_8001C7FC(4, id, 0);
        }
        /* padded: a zero word follows the first jump table */
        __asm__(".section .rodata\n\t.word 0\n\t.section .text");
        switch (kind) {
        case 0:
            func_800553EC(h, arg2, arg3, arg4);
            return 0;
        case 1:
            func_8005546C(h, arg2);
            return 0;
        case 2:
            switch (arg0 & 0x3F) {
            case 1:
                func_80055764(h, arg2, arg3);
                return 0;
            case 5:
                func_80055850(h, arg2, arg3);
                return 0;
            }
            return -1;
        case 3:
            switch (arg0 & 0x3F) {
            case 1:
                func_800557A8(h, arg2, arg3, arg4);
                return 0;
            case 5:
                func_80055894(h, arg2, arg3, arg4);
                return 0;
            }
            return -1;
        case 5:
            switch (arg0 & 0x3F) {
            case 1:
                func_80055704(arg2);
                return 0;
            case 5:
                func_800557F0(arg2);
                return 0;
            }
            return -1;
        case 6:
            switch (arg0 & 0x3F) {
            case 1:
                func_80055730(arg2, arg3);
                return -1;
            case 5:
                func_8005581C(arg2, arg3);
                return -1;
            }
            return -1;
        }
        return -1;
    case 3:
        id = (arg1 & 0xFFFF) | 0x90000;
        switch (kind) {
        case 7:
            if (arg2 >= 4) {
                gc = D_8006794C->unk1C;
                e = gc->unk8E4.battle->unk1538[arg2 % 4];
                gc->unk8C0 = e->unk24;
                D_8006794C->unk1C->unk878 = 0x12FD8 - (e->unk24 - e->unk20);
                gx = D_8006794C->unk1C;
            } else {
                D_8006794C->unk1C->unk8C0 = (s32)D_8006794C->unk1C->unk8E4.battle->unk11C0->unk48;
                D_8006794C->unk1C->unk878 = D_8006794C->unk1C->unk8E4.battle->unk11C0->unk2C[0] -
                                            (s32)D_8006794C->unk1C->unk8E4.battle->unk11C0->unk48;
                gx = D_8006794C->unk1C;
            }
            k2 = gx->unk878;
            gx->unk878 = k2 / 2048 * 2048;
            D_800F51EC = arg2 % 4;
            D_800F51F8 = kind;
            D_800F51F0 = id;
            D_8006794C->unk1C->unk87C = 0;
            D_8006794C->unk1C->unk851 = 2;
            return D_8006794C->unk1C->unk851;
        case 12:
            goto stream;
        }
        return -1;
    default:
        return -1;
    }
stream:
    gd = D_8006794C->unk1C;
    switch (gd->unk851) {
    case 2:
        if (((D_800F51F0 >> 16) & 0xFF) == 7) {
            gd->unk852 = gd->unk820[0];
        } else {
            gd->unk852 = D_800F51F0;
        }
        D_800F51F4 = func_800AE570(D_800F51F0, &size);
        if (D_800F51EC == -1) {
            func_80055DF4();
        }
        ge = D_8006794C->unk1C;
        func_8002177C(ge->unk854, D_800F51F4, size, (void *)ge->unk8C0, 0, D_8006794C->unk1C->unk878, func_800AE844);
        D_8006794C->unk1C->unk851 = 3;
        break;
    case 3:
        if (func_800217C4(gd->unk854) == 0 && func_80055DCC() == 0) {
            gf = D_8006794C->unk1C;
            if (gf->unk854[0].unkC + gf->unk854[0].unk10 < gf->unk87C) {
                gf->unk8C0 = (s32)gf->unk854[0].unk8;
                gg = D_8006794C->unk1C;
                m = (s32)(D_80067944 + 0x156F00) - gg->unk8C0;
                m = m / 2048 * 2048;
                func_8002177C(gg->unk854, D_800F51F4, gg->unk854[0].unk4, (void *)gg->unk8C0,
                              gg->unk854[0].unkC + gg->unk854[0].unk10, m, func_800AE844);
            } else if (D_800F51F2 == 7) {
                if (D_800F51F8 != 9) {
                    gf->unk826[0] = 0x7F;
                }
                D_8006794C->unk1C->unk851 = 4;
            } else {
                D_800F51F8 = -1;
                D_8006794C->unk1C->unk851 = 0;
            }
        }
        break;
    case 4:
        n = gd->unk854[0].unkC + gd->unk854[0].unk10 - gd->unk87C;
        if (n > 0) {
            func_80015C28(gd->unk814, (u8 *)(gd->unk8C0 + (gd->unk87C - gd->unk854[0].unkC)), n);
        }
        D_8006794C->unk1C->unk8C0 = (s32)((u8 *)D_8006794C->unk1C->unk814 + n);
        m2 = 0x6800 - n;
        m2 = m2 / 2048 * 2048;
        gh = D_8006794C->unk1C;
        func_8002177C(gh->unk854, D_800F51F4, gh->unk854[0].unk4, (void *)gh->unk8C0,
                      gh->unk854[0].unkC + gh->unk854[0].unk10, m2, NULL);
        D_8006794C->unk1C->unk851 = 5;
        break;
    case 5:
        if (func_800217C4(gd->unk854) == 0) {
            gi = D_8006794C->unk1C;
            k1 = gi->unk854[0].unkC + gi->unk854[0].unk10;
            if (k1 < gi->unk854[0].unk4) {
                k2 = k1 - gi->unk87C;
                gi->unk8C0 = (s32)((u8 *)gi->unk814 + k2);
                gj = D_8006794C->unk1C;
                func_8002177C(gj->unk854, D_800F51F4, gj->unk854[0].unk4, (void *)gj->unk8C0, k2, 0x6800 - k2, NULL);
            } else {
                func_800AE7C8((s32)gi->unk814);
                D_8006794C->unk1C->unk0 |= 0x10000000;
                D_8006794C->unk1C->unk851 = 0;
                if (D_800F51F8 == 0) {
                    goto finish;
                }
                D_800F51F8 = -1;
            }
        }
        break;
    }
    return D_8006794C->unk1C->unk851;
}

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

s32 func_800AE4FC(s32 arg0) {
    Character *c = D_8006794C->unk1C->unk54C[arg0];

    if (c == NULL) {
        /* two nested do-while(0) wrappers (macros?): they change the block structure enough to load the table base after the NULL check */
        do {
            do {
                return -1;
            } while (0);
        } while (0);
    }
    return ((FileEntry16 *)func_8001E218(8, D_8006E0E8[c->unk39[0]].unk2))->unk2 | 0x90000;
}

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

s16 func_800AE618(s32 arg0, BtlStepTask *arg1) {
    BtlAnimEvtList *l;
    s32 i;
    BtlAnimEvt *e;
    void **m;

    switch (arg1->unk6) {
    case 1:
        return ++arg1->unk6;
    case 2:
        m = &D_8006794C->unk1C->unk8E4.battle->unk1354->unk20[arg0];
        l = (BtlAnimEvtList *)func_8001C8B0((s32)*m, 2, 0x140000, 0);
        e = l->unk4;
        for (i = l->unk2 - 1; i >= 0; i--) {
            if (e[i].unk0.b.unk2 == 9) {
                func_800AD90C(0x7700, e[i].unk0.w, arg0 + 4, 0, 0);
                break;
            }
        }
        return ++arg1->unk6;
    case 3:
        if (func_800AD90C(0xC00, 0, 0, 0, 0) == 0) {
            arg1->unk6 = 0;
        }
        return arg1->unk6;
    }
    return 0;
}

s32 func_800AE764(s32 arg0) {
    FileTable *t = D_8006794C->unk1C->unk7E4;
    FileEntry *e = t->unk4;
    s32 i;
    for (i = t->unk2 - 1; i >= 0; i--) {
        if (e[i].unk0 == arg0) {
            return e[i].unkC;
        }
    }
    return -1;
}

s32 func_800AE7C8(s32 arg0) {
    s32 v = func_8001C8B0(arg0, 3, 0x70000, 0);
    s32 a = func_8001C8B0(arg0, 4, v, 0);
    func_80015C28((u8 *)arg0, (u8 *)a, func_8001C8B0(arg0, 5, v, 0));
    return 0;
}

void func_800AE844(Unk800AE844 *arg0) {
    s32 p = arg0->unk8;
    s32 id = D_8006794C->unk1C->unk852 | 0x90000;
    s32 f;
    s32 base;
    s32 n;

    if (arg0->unkC == 0) {
        f = func_8001C8B0(p, 2, 0x1B0000, 0);
        base = p;
        if (f == -1) {
            p = func_8001C8B0(p, 4, id, 0);
            D_8006794C->unk1C->unk87C = func_8001CA70(base);
        } else {
            p = func_8001C8B0(f, 4, id, 0);
            D_8006794C->unk1C->unk87C = func_8001C8B0(base, 2, 0x1B0001, 0) - D_8006794C->unk1C->unk8C0;
        }
    }
    n = D_8006794C->unk1C->unk87C - arg0->unkC;
    if (arg0->unk10 < n) {
        n = arg0->unk10;
    }
    n = arg0->unk8 + n - p;
    if (D_800F51EC == -1) {
        func_80055E14(p, n, 0);
    } else {
        func_80056048(p, D_800F51EC, 0);
    }
    arg0->unk8 = p;
}

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

void func_800AEA48(void) {
    ClearOTagR(D_8006794C->unk1C->unk10[D_8006794C->unk1C->unk8] - 8, 0x1010);
    ClearOTagR((u32 *)D_8006794C->unk1C->unk8E4.battle->unk11C0->unk2C[D_8006794C->unk1C->unk8], 0x100);
    ClearOTagR(func_8002FBC0(D_8006794C->unk1C->unk8), 1);
    ClearOTagR((u32 *)func_800F4FB4(D_8006794C->unk1C->unk8), 1);
    setlen(&D_800FDEB8[D_8006794C->unk1C->unk8].unk0, 2);
    ((u32 *)&D_800FDEB8[D_8006794C->unk1C->unk8].unk0)[1] = 0xE6000000;
    ((u32 *)&D_800FDEB8[D_8006794C->unk1C->unk8].unk0)[2] = 0;
    addPrim(D_8006794C->unk1C->unk10[D_8006794C->unk1C->unk8], &D_800FDEB8[D_8006794C->unk1C->unk8].unk0);
    setlen(&D_800FDEB8[D_8006794C->unk1C->unk8].unkC, 2);
    ((u32 *)&D_800FDEB8[D_8006794C->unk1C->unk8].unkC)[1] = 0xE6000000;
    ((u32 *)&D_800FDEB8[D_8006794C->unk1C->unk8].unkC)[2] = 0;
    addPrim((u32 *)D_8006794C->unk1C->unk8E4.battle->unk11C0->unk2C[D_8006794C->unk1C->unk8], &D_800FDEB8[D_8006794C->unk1C->unk8].unkC);
    addPrim(D_8006794C->unk1C->unk10[D_8006794C->unk1C->unk8], func_8002FBC0(D_8006794C->unk1C->unk8));
    addPrim(D_8006794C->unk1C->unk10[D_8006794C->unk1C->unk8], func_800F4FB4(D_8006794C->unk1C->unk8));
    func_800E9DFC();
}

void func_800AEE68(void) {
    if (!(D_8006794C->unk1C->unk0 & 0x100) && ((D_800739A0[0] & 0xF09) == 0xF09 || (D_800739A0[1] & 0xF09) == 0xF09)) {
        D_8006794C->unk0 |= 1;
    }
    func_800E9EC4();
    setlen(&D_800FDED0[D_8006794C->unk1C->unk8].unk0, 2);
    ((u32 *)&D_800FDED0[D_8006794C->unk1C->unk8].unk0)[1] = 0xE6000001;
    ((u32 *)&D_800FDED0[D_8006794C->unk1C->unk8].unk0)[2] = 0;
    addPrim(&D_8006794C->unk1C->unk10[D_8006794C->unk1C->unk8][0xFFF], &D_800FDED0[D_8006794C->unk1C->unk8].unk0);
    setlen(&D_800FDED0[D_8006794C->unk1C->unk8].unkC, 2);
    ((u32 *)&D_800FDED0[D_8006794C->unk1C->unk8].unkC)[1] = 0xE6000001;
    ((u32 *)&D_800FDED0[D_8006794C->unk1C->unk8].unkC)[2] = 0;
    addPrim(&((u32 *)D_8006794C->unk1C->unk8E4.battle->unk11C0->unk2C[D_8006794C->unk1C->unk8])[0xFF], &D_800FDED0[D_8006794C->unk1C->unk8].unkC);
    func_800EA290();
    if (!(D_8006794C->unk1C->unk0 & 0x100) && !(D_8006794C->unk1C->unk8E4.ovl65->unk0 & 0x200)) {
        func_800EA1F8(&((u32 *)D_8006794C->unk1C->unk8E4.battle->unk11C0->unk2C[D_8006794C->unk1C->unk8])[0xFF], &D_8006794C->unk10[D_8006794C->unk1C->unk8]);
        DrawOTag(&D_8006794C->unk1C->unk10[D_8006794C->unk1C->unk8][0xFFF]);
    }
    if (!(D_8006794C->unk1C->unk0 & 0x20)) {
        D_8006794C->unk1C->unk8 ^= 1;
    }
}

s32 func_800AF1C8(void) {
    return 0;
}

void func_800AF1D0(void) {
    Battle11C0 *b;
    FileEntry *f;
    s32 size;
    s32 *dst;

    b = D_8006794C->unk1C->unk8E4.battle->unk11C0;
    func_80021C60();
    D_8006794C->unk1C->unk8BC = NULL;
    f = func_8001E218(0, 0x10065);
    size = (f->unkC - f->unk4) << 11;
    func_80021974(func_80021CFC(), 0, 0x10065, 0x801EDF00, size, size, f->unk4, size);
    f = func_8001E218(1, 0x2713);
    dst = b->unkC;
    size = (f->unkC - f->unk4) << 11;
    func_80021974(func_80021CFC(), 0, 0x2713, (s32)dst, size, size, f->unk4, size);
}

s32 func_800AF2E0(void) {
    GameData *g = D_8006794C->unk1C;
    Battle *b = g->unk8E4.battle;
    BtlUnit *u;
    u16 id;
    s32 n;

    func_800C3770();
    switch (b->unkD80) {
    case 2:
        func_800B0060(g, b);
        break;
    case 1:
        func_800AF578(g, b);
        break;
    case 3:
        if ((u8)func_800AF578(g, b)) {
            b->unkD80 = 4;
            b->unkD86 = 0;
            b->unkD7C &= 0xF;
            for (u = b->unk6F8; u != NULL; u = u->unk0) {
                if (!(u->unk68 & 1) && (id = func_800CA378()) != 0xFFFF) {
                    func_800C22CC(u, id, 0x7F);
                    break;
                }
            }
            if (D_8007AFB1 == 0) {
                func_800E9F3C(1);
                func_800E9F68(1);
            }
            n = b->unk132C.unk0 & 3;
            if (n == 0) {
                func_800CD5C8(2);
            } else if (n == 1) {
                func_800CD5C8(1);
            }
        }
        break;
    case 4:
        func_800B0468(g, b);
        break;
    case 5:
        func_800E9F68(0);
        func_800AF800(g, b);
        if (b->unkD81 != 3) {
            b->unkD83 = 0;
        }
        break;
    case 6:
        func_800AF578(g, b);
        if (b->unkD81 == 1) {
            func_8002FAAC(0x20);
            b->unkD81++;
        } else if (b->unkD81 == 2) {
            if (b->unkD82++ > 0x20) {
                return (u8)func_800C1794(b);
            }
        }
        break;
    case 7:
    case 8:
        if ((u8)func_800AF578(g, b)) {
            g->unk56F |= 0x40;
            if (b->unkD81 == 0) {
                if (g->unk56E != 5) {
                    func_8002FAAC(0x20);
                }
                b->unkD81++;
            } else if (b->unkD81 == 1) {
                if (b->unkD82++ > 0x20) {
                    return (u8)func_800C1794(b);
                }
            }
        }
        break;
    }
    func_8004BC38();
    b->unkD7C++;
    return 0;
}

s32 func_800AF578(GameData *arg0, Battle *arg1) {
    BtlUnit *u = arg1->unk6F8;
    s32 r = 1;
    s32 sp = arg1->unk11C0->unk34[arg0->unk8];
    BtlUnit9C *m;
    BtlUnit9C *n;
    u32 f;
    u32 g;
    for (; u != NULL; u = u->unk0) {
        if (!((u->unk68 >> 22) & 1)) {
            func_800BC21C(u, &sp);
            if (arg1->unkD80 == 3) {
                n = u->unk9C;
                g = u->unk68 & ~0x200000;
                u->unk68 = g;
                if (u->unkA0->unk3A >= n->unkC->unk2) {
                    if (!(u->unk3C & 0x100)) {
                        n->unkC = (BtlUnit9CC *)u->unkA4[(g >> 18) & 1];
                    }
                    u->unkA0->unk3A = 0;
                }
                if (!(u->unk3C & 1)) {
                    if ((s32)u->unk9C->unkC != u->unkA4[(u->unk68 >> 18) & 1] && (s32)u->unk9C->unkC != u->unkA4[4]) {
                        r = 0;
                    }
                }
            } else if (arg1->unkD80 == 6) {
                f = u->unk68;
                if ((f & 1) && !(u->unk3C & 0x1103) && ((arg1->unk132C.unk0 >> 6) & 1) && (D_8006794C->unk1C->unk3C + ((f >> 1) & 0xF))->unk34.b.unkA) {
                    m = u->unk9C;
                    if (u->unkA0->unk3A >= m->unkC->unk2) {
                        m->unkC = (BtlUnit9CC *)u->unkA4[19];
                        u->unkA0->unk3A = 0;
                    }
                }
            }
            func_800BFF20(u);
        }
        func_800BCEF0(u);
    }
    if (arg1->unkD80 == 7 && !((arg1->unk132C.unk0 >> 4) & 1) && (u8)func_800C20B8(arg0, 0x1E, 6) == 0) {
        r = 0;
    }
    func_800B05C0(arg0, arg1, sp);
    return r;
}

void func_800AF800(GameData *arg0, Battle *arg1) {
    s32 sp18;
    s32 done;
    BtlUnit *u;
    u32 exp;

    sp18 = arg1->unk11C0->unk34[arg0->unk8];
    done = 1;
    if (D_800F78A4->unk0->unk0 != -1 || arg1->unk1074.unk0 != NULL || arg1->unk1088 != NULL) {
        done = 0;
    }
    for (u = arg1->unk6F8; u != NULL; u = u->unk0) {
        if (!((u->unk68 >> 22) & 1)) {
            func_800BC21C(u, &sp18);
            switch (arg1->unkD81) {
            case 0:
                func_800BD93C(u);
                if ((!(u->unk68 & 1) && !(u->unk3C & 0x1003)) || ((u->unk68 & 1) && (u->unk3C & 0x100) && u->unk9C->unkC != (BtlUnit9CC *)u->unkA4[4])) {
                    done = 0;
                }
                break;
            case 1:
                if ((u->unk68 & 1) && u->unk9C->unkC != (BtlUnit9CC *)u->unkA4[4] && !(u->unk3C & 0x1003)) {
                    done = 0;
                }
                break;
            case 2:
                func_800BD93C(u);
                if (u->unk68 & 1) {
                    func_800C0B6C(u, 0x02009400);
                }
                if (arg1->unk11BA != 0) {
                    done = 0;
                }
                if (u->unk3C & 0x100) {
                    if (u->unk7D == 0 && func_800B3E7C(u->unkC) == 0) {
                        func_800B1250(u->unkC, 0x3C, 0, u->unkB0, 0);
                    }
                    if (u->unk7D != 6) {
                        done = 0;
                    }
                } else if (!(u->unk3C & 0x02001003)) {
                    if (u->unk9C->unkC != (BtlUnit9CC *)u->unkA4[0] &&
                        u->unk9C->unkC != (BtlUnit9CC *)u->unkA4[1]) {
                        done = 0;
                    }
                }
                break;
            case 3:
                if (u->unk3C & 0x100) {
                    if (u->unk7D == 0) {
                        u->unk7D = 1;
                    }
                    if (u->unk9C->unkC != (BtlUnit9CC *)u->unkA4[4]) {
                        done = 0;
                    } else if (func_800B3EF8(u, 0x3D) != 0) {
                        done = 0;
                    }
                } else if ((u->unk68 & 1) && !(u->unk3C & 0x42021143)) {
                    u->unk8C[2] += 0xFF9C0000;
                    func_800C1A8C(u, (s8)arg1->unkD84);
                    if ((s8)arg1->unkD84 <= 0) {
                        u->unk68 |= 0x400000;
                    } else {
                        done = 0;
                        if ((s8)arg1->unkD84 == 0x20) {
                            func_800AD90C(0xD000, 0xB5A, 0, 0x80, 0x7F);
                            func_800AD90C(0xD000, 0xB5B, 0, 0x80, 0x7F);
                            func_800AD90C(0xD000, 0xB5C, 0, 0x80, 0x7F);
                            arg1->unkD84 -= 2;
                        }
                    }
                }
                break;
            }
            func_800BFF20(u);
        }
        func_800BCEF0(u);
    }
    if (arg1->unkD81 == 3 && (s8)arg1->unkD84 < 0x20) {
        arg1->unkD84 -= 2;
    }
    if (done) {
        func_800E9F3C(0);
        switch (arg1->unkD81) {
        case 0:
        case 4:
            arg0->unk56F |= 0x40;
            arg0->unk56E = 1;
            if ((arg1->unk132C.unk0 >> 6) & 1) {
                if ((u8)func_800C20B8(arg0, 0x1E, 5) == 0) {
                    break;
                }
                for (u = arg1->unk6F8; u != NULL; u = u->unk0) {
                    if (u->unk68 & 1) {
                        func_800C0B6C(u, 0xBFFFBC00);
                        if (!(u->unk3C & 0x1103)) {
                            if (u->unk28 != 0) {
                                u->unk86[1] = u->unkA0->unk22 = (u->unk68 & 1) * 0x800;
                                u->unk68 = (u->unk68 & ~0x40000) | ((func_800C09A4(u, 0x30202) & 1) << 18);
                                u->unk68 &= ~0x80000;
                                if (((D_8006794C->unk1C->unk3C + ((u->unk68 >> 1) & 0xF))->unk34.w >> 10) & 1) {
                                    u->unk9C->unkC = (BtlUnit9CC *)u->unkA4[0x12];
                                } else {
                                    u->unk9C->unkC = (BtlUnit9CC *)u->unkA4[(u->unk68 >> 18) & 1];
                                }
                                u->unkA0->unk3A = 0;
                            } else {
                                func_800BEDBC(u, 0x100);
                            }
                        }
                    }
                }
                func_800C2F1C(func_800CA2B0(2));
            } else if ((arg1->unk132C.unk0 >> 14) & 1) {
                arg0->unk56F |= 8;
            }
            arg1->unkD80 = 6;
            break;
        case 1:
            arg0->unk56E = 3;
            arg1->unkD80 = 7;
            break;
        case 2:
            if ((arg1->unk132C.unk0 & 0x4040) == 0x4000) {
                arg0->unk56F |= 8;
            }
            arg1->unkD80 = 1;
            func_800B2F24(arg1);
            func_800B0EF8(arg1);
            for (u = arg1->unk6F8; u != NULL; u = u->unk0) {
                func_800B10F8(u);
                if (!(u->unk68 & 1)) {
                    u->unkA0->unk22 = 0;
                    u->unk86[1] = 0;
                }
            }
            break;
        case 3:
            exp = D_800FDF30.unk0;
            arg0->unk56F |= 0x40;
            for (u = arg1->unk6F8; u != NULL; u = u->unk0) {
                if (!(u->unk68 & 1)) {
                    exp += D_8006794C->unk1C->unk8E4.battle->unk488[(u->unk68 >> 1) & 0xF].unk0->unk44;
                }
            }
            if ((arg1->unk132C.unk0 & 0x4040) == 0x4000) {
                arg0->unk56F |= 8;
            }
            arg0->unk56E = 4;
            arg1->unkD80 = 8;
            func_800C1594();
            if (arg0->unk56F & 4) {
                exp /= 10;
                if (exp < arg0->unk560) {
                    arg0->unk560 -= exp;
                } else {
                    exp = arg0->unk560;
                    arg0->unk560 = 0;
                }
                func_800CD5C8(0x1E, exp);
            } else if ((u8)func_800B0CF8(1, 0x4000) != 0) {
                D_800FDF30.unk1D = 1;
                D_800FDF30.unk0 = exp / 10;
            }
            break;
        }
        if (arg1->unkD80 != 5) {
            if (arg1->unkD80 == 6 && !((arg1->unk132C.unk0 >> 6) & 1)) {
                arg1->unkD81 = 1;
            } else {
                arg1->unkD81 = 0;
            }
        }
    } else {
        func_800B1764(arg1);
    }
    func_800B05C0(arg0, arg1, sp18);
}

void func_800B0060(GameData *arg0, Battle *arg1) {
    u32 *ot;
    BtlUnit *u;
    u8 f;
    u8 g;

    ot = (u32 *)arg1->unk11C0->unk34[arg0->unk8];
    func_800AB580(0);
    if ((arg1->unk0 & 2) && (*(u32 *)&arg1->unkD84 & 0x2800) == 0x800) {
        func_800C072C();
        arg1->unkD85 |= 0x20;
    } else if ((arg1->unk0 & 4) && (*(u32 *)&arg1->unkD84 & 0x5000) == 0x1000) {
        func_800C0840();
        arg1->unkD85 |= 0x40;
    }
    for (u = arg1->unk6F8; u != NULL; u = u->unk0) {
        if ((u->unk68 >> 22) & 1) {
            continue;
        }
        func_800BC21C(u, (s32 *)&ot);
        if (u->unk68 & 1) {
            func_800C1A8C(u, (s8)arg1->unkD86);
        } else if (!((u->unk68 >> 23) & 1)) {
            func_800C1A8C(u, (s8)arg1->unkD87);
        }
        func_800BFF20(u);
    }
    if (arg1->unkD85 & 2) {
        func_800CA7CC();
    }
    if (arg1->unk0 & 1) {
        ot = (u32 *)func_800CE1B4(&arg0->unk18, arg1->unk11C0->unk1C, (s32)ot, arg0->unk10[arg0->unk8],
                                  arg1->unk11C0->unk2C[arg0->unk8]);
    }
    ot = (u32 *)func_800F4E70((u8 *)ot);
    if (arg1->unk0 & 2) {
        f = arg1->unkD85;
        if (!(f & 2)) {
            if (!(f & 8)) {
                func_800C0C7C(ot);
                func_800BB1F4(arg1);
                ot = func_800BB974(arg1, ot);
            } else if (f & 0x20) {
                if ((s8)arg1->unkD87 >= 0x20) {
                    arg1->unkD85 = f | 2;
                } else {
                    arg1->unkD87 += 4;
                }
            }
        }
    }
    if (arg1->unk0 & 4) {
        g = arg1->unkD85;
        if (!(g & 4)) {
            if (!(g & 0x10)) {
                func_800C0CD0(ot);
                func_800BAFEC(arg1);
            } else if (g & 0x40) {
                if ((s8)arg1->unkD86 >= 0x20) {
                    arg1->unkD85 = g | 4;
                } else {
                    arg1->unkD86 += 4;
                }
            }
        }
    }
}

void func_800B031C(void) {
    GameData *gd;
    s32 v;
    D_8006794C->unk1C->unk56F = 0;
    gd = D_8006794C->unk1C;
    v = 7;
    if (gd->unk571 != 0) {
        v = 8;
    }
    gd->unk3C[3].unk34.w = (gd->unk3C[3].unk34.w & ~0x1F0) | (v << 4);
    D_8006794C->unk1C->unk56E = 0;
    func_800C1524();
    func_800E3D80();
    func_800C2E04();
    func_800CBF90();
    func_800F4DFC();
}

void func_800B03B0(void) {
    Battle *b;
    Battle11C0 *p;

    b = D_8006794C->unk1C->unk8E4.battle;
    p = b->unk11C0;
    func_800CE05C(p->unk18, p->unk40);
    b->unkD85 |= 1;
    func_800B0EF8(b);
    func_800B0FC0(b);
    func_800C972C();
    D_800FDF30.unkB = (b->unk132C.unk0 >> 13) & 1;
    func_8004C204();
    func_8004BC38();
    func_800AB580(1);
    func_8002FA50(0x20);
    func_800C2F1C(func_800CA2B0(0));
    b->unkD80 = 2;
}

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

void func_800B05C0(GameData *arg0, Battle *arg1, s32 arg2) {
    if (arg1->unk11BE) {
        arg1->unk11BE -= 1;
    }
    func_800CA7CC();
    arg2 = (s32)func_800CE1B4(&arg0->unk18, arg1->unk11C0->unk1C, arg2, arg0->unk10[arg0->unk8],
                              arg1->unk11C0->unk2C[arg0->unk8]);
    arg2 = (s32)func_800E3EEC((u8 *)arg2);
    arg2 = (s32)func_800CC4F0((u8 *)arg2);
    func_800F4E70((u8 *)arg2);
}

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

u16 func_800B081C(s32 arg0) {
    BtlUnit *best = NULL;
    BtlUnit *t = func_800BED10(arg0);
    BtlUnit *u;
    GameData *g;
    s32 v;
    if (t->unk3C & 0x101) {
        return 0;
    }
    g = D_8006794C->unk1C;
    if (((g->unk3C + ((t->unk68 >> 1) & 0xF))->unkA & 2) && t->unk28 < (t->unk24->unk0 >> 1)) {
        for (u = g->unk8E4.battle->unk6F8; u != NULL; u = u->unk0) {
            if ((u->unkC4[1] & 0x40) && !(u->unk3C & 0x43001103) && u != t && (best == NULL || best->unk28 < u->unk28)) {
                best = u;
            }
        }
    }
    if (best == NULL && (t->unk3C & 0x200)) {
        for (u = D_8006794C->unk1C->unk8E4.battle->unk6F8; u != NULL; u = u->unk0) {
            if ((u->unkC4[1] & 0x20) && !(u->unk3C & 0x43001103) && u != t && (best == NULL || best->unk28 < u->unk28)) {
                best = u;
            }
        }
    }
    if (best == NULL) {
        return 0;
    }
    v = (best->unk68 & 1) << 11;
    best->unkA0->unk22 = v;
    best->unk86[1] = v;
    best->unk8C[0] = t->unk8C[0];
    best->unk8C[2] = t->unk8C[2];
    t->unk8C[2] += 0xFE700000;
    best->unk9C->unkC = (BtlUnit9CC *)best->unkA4[15];
    best->unk68 |= 0x2000000;
    return best->unkB0;
}

void func_800B0A6C(BtlUnit *arg0) {
    if (arg0->unkC4[0] & 1) {
        arg0->unk38 |= 0x20000000;
    }
    if (arg0->unkC4[0] & 2) {
        arg0->unk38 |= 0x200000;
    }
    if (arg0->unkC4[0] & 4) {
        arg0->unk38 |= 0x80000;
        arg0->unk34 |= 0x100000;
        arg0->unk2E = arg0->unk2E * 3 / 2;
    }
    if (arg0->unkC4[0] & 8) {
        arg0->unk38 |= 0x40000;
        func_800C0C00(arg0, 0x12);
    }
    if (arg0->unkC4[0] & 0x10) {
        arg0->unk3C |= 0x2000;
    }
    if (arg0->unkC4[1] & 0x100) {
        arg0->unk34 |= 0x03000000;
    }
    if (arg0->unkC4[1] & 0x10000) {
        arg0->unk34 |= 0x20000;
    }
    if (arg0->unkC4[1] & 0x20000) {
        arg0->unk34 |= 0x10002;
    }
    if (arg0->unkC4[1] & 0x40000) {
        arg0->unk34 |= 0x10;
    }
    if (arg0->unkC4[1] & 0x80000) {
        arg0->unk34 |= 8;
    }
    if (arg0->unkC4[1] & 0x200000) {
        arg0->unk34 |= 0x80000001;
    }
    if (arg0->unkC4[1] & 0x02000000) {
        arg0->unk34 |= 0x1000;
    }
    if (arg0->unkC4[1] & 0x04000000) {
        arg0->unk34 |= 0x400;
    }
}

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
            func_80063640(&arg0->unk108C[i].unk0[j].unk0, GetTPage(0, 1, 0, 0));
            if (j < 3) {
                c = 0xBC - j * 0x3C;
            } else {
                c = 0xA0;
            }
            setRGB0(&arg0->unk108C[i].unk0[j].unk8, c, c, 0);
            SetSemiTrans(&arg0->unk108C[i].unk0[j].unk8, 1);
        }
    }
}

void func_800B10F8(BtlUnit *arg0) {
    Battle *b = D_8006794C->unk1C->unk8E4.battle;
    BtlCmd *c;
    s32 k;
    s32 i;
    s32 j;
    if (b->unkD80 == 2) {
        arg0->unkB2 = 1;
    }
    arg0->unkB3 = 0;
    arg0->unkBC = 0;
    if (arg0->unk68 & 1) {
        k = ((arg0->unk68 >> 5) & 7) * 6;
    } else {
        k = (((arg0->unk68 >> 5) & 7) - 4) * 3 + 0x18;
    }
    c = &b->unkD90[k];
    /* unk4..unk18 are the unit's six command slots */
    for (i = 0; i < 6; i++) {
        if ((arg0->unk68 & 1) || i < 3) {
            (&arg0->unk4)[i] = c;
            c->unk4 = arg0;
            c++;
            func_800B3CEC_inline((&arg0->unk4)[i]);
        } else {
            (&arg0->unk4)[i] = NULL;
        }
    }
    for (j = 3; j >= 0; j--) {
        arg0->unk1C[j] = 0;
    }
}

void func_800B1250(BtlCmd *arg0, u32 arg1, s32 arg2, s32 arg3, s32 arg4) {
    BtlUnit *u = arg0->unk4;
    BtlUnit9C *m;
    s32 *a;
    BtlUnit9CC *c;

    switch (arg1) {
    case 10:
    case 11:
        D_8006794C->unk1C->unk8E4.battle->unk11B8 |= 0x10;
        arg0->unk8 = (BtlCmd8 *)&D_8006794C->unk1C->unk8E4.battle->unk16CC[arg2];
        break;
    case 15:
        arg0->unk8 = (BtlCmd8 *)&D_8006794C->unk1C->unk8E4.battle->unk16CC[0xBE];
        break;
    case 0x32:
        arg0->unk8 = &D_8006794C->unk1C->unk8E4.battle->unk5D8[arg2];
        break;
    case 0x38:
        if (D_8006794C->unk1C->unk8E4.battle->unk11B8 & 1) {
            arg0->unkF = arg2;
            return;
        }
        D_8006794C->unk1C->unk8E4.battle->unk11B8 |= 1;
        arg0->unk8 = (BtlCmd8 *)&D_8006794C->unk1C->unk8E4.battle->unk16CC[0xB4];
        break;
    case 14:
    case 0x33:
    case 0x3C:
    case 0x3D:
    case 0x3E:
        arg0->unk8 = (BtlCmd8 *)&D_8006794C->unk1C->unk8E4.battle->unk16CC[0];
        break;
    case 2:
        if (u->unkC4[1] & 0x20000000) {
            arg2 = 0xB5;
        }
    default:
        arg0->unk8 = (BtlCmd8 *)&D_8006794C->unk1C->unk8E4.battle->unk16CC[arg2];
        break;
    }
    arg0->unkC = arg3;
    arg0->unkE = arg1;
    arg0->unkF = arg2;
    arg0->unk10.w = (arg0->unk10.w & ~3) | (arg4 & 3);
    arg0->unk10.w &= ~0x20;
    arg0->unk10.w &= ~0x40;
    arg0->unk10.w &= ~0x180;
    if (arg1 >= 0x31) {
        arg0->unk10.w |= 0x10;
    } else {
        func_800BF558(u, 0x8000);
    }
    if (arg1 < 0x37) {
        if (D_8006794C->unk1C->unk8E4.battle->unk1088 != u->unk14) {
            m = u->unk9C;
            a = u->unkA4;
            c = m->unkC;
            if (c == (BtlUnit9CC *)a[0]) {
                m->unkC = (BtlUnit9CC *)a[10];
                u->unkA0->unk3A = 0;
            } else if (c == (BtlUnit9CC *)a[1]) {
                m->unkC = (BtlUnit9CC *)a[11];
                u->unkA0->unk3A = 0;
            } else if (c == (BtlUnit9CC *)a[13] && arg1 < 0x30) {
                m->unkC = (BtlUnit9CC *)a[14];
                u->unkA0->unk3A = 0;
            }
        }
        u->unk68 |= 0x80000;
    }
    func_800B3090(arg0);
}

/* padded: the next jump table is 8-aligned in its object */
__asm__(".section .rodata\n\t.word 0\n\t.section .text");

void func_800B1520(s32 arg0, s16 arg1, u32 arg2, u32 arg3) {
    Battle *b;
    BtlUnit *u;
    BtlCmd *c;

    u = func_800BED10(arg0);
    b = D_8006794C->unk1C->unk8E4.battle;
    if (b->unk11B8 & 1 || (u->unk3C & 0x02021103)) {
        goto fail;
    }
    if (arg2 == 0x2F) {
        if (b->unkD80 != 4) {
            goto fail;
        }
        c = u->unk4;
        if (u->unk3C & 0x400) {
            arg1 = func_800C1F7C(RAND8() & 1);
            arg3 = D_8006794C->unk1C->unk8E4.battle->unk488[(u->unk68 >> 1) & 0xF].unk0->unk26;
        } else if (u->unk3C & 0x800) {
            arg1 = func_800C1F7C(1);
            arg3 = D_8006794C->unk1C->unk8E4.battle->unk488[(u->unk68 >> 1) & 0xF].unk0->unk26;
        }
    } else {
        if (arg2 != 0x35) {
            if (arg2 != 0x36) {
fail:
                u->unkB3 = 0;
                return;
            }
        }
        c = u->unk8;
    }
    c->unk8 = &D_8006794C->unk1C->unk8E4.battle->unk5D8[arg3];
    c->unkC = arg1;
    c->unkE = arg2;
    c->unkF = arg3;
    if ((u32)(c->unk8->unk0_0 - 6) < 7) {
        c->unk10.b.unk0_0 = 1;
    } else {
        c->unk10.b.unk0_0 = 0;
    }
    if (arg2 > 0x30) {
        c->unk10.b.unk0_4 = 1;
    } else {
        func_800BF558(c->unk4, 0x8000);
    }
    c->unk10.b.unk0_5 = 0;
    c->unk10.b.unk0_6 = 0;
    c->unk10.b.unk0_7 = 0;
    u->unk68 |= 0x80000;
    func_800B3090(c);
}

void func_800B1764(Battle *b) {
    BtlCmd *cmd;
    BtlUnit *u;
    BtlUnit *w;
    BtlCmd *p;
    u8 k;

    if (b->unk1088 == NULL) {
        if ((p = b->unk1074.unk0) == NULL) {
            return;
        }
        for (cmd = p; cmd != NULL; cmd = cmd->unk0) {
            if (!((cmd->unkE < 0x3C && cmd->unkE != 0x38 && cmd->unkE != 0x3A && (cmd->unk4->unk3C & 0x2001003)) ||
                  (cmd->unkE == 0x39 && (cmd->unk4->unk3C & 0x100)))) {
                break;
            }
        }
        if (cmd == NULL) {
            return;
        }
        if (!func_800EA038() && cmd->unkE < 0x30) {
            return;
        }
        k = cmd->unkE;
        if (k < 0x37) {
            w = cmd->unk4;
            if ((u32)(k - 0x31) < 4) {
                w->unk9C->unkC = (BtlUnit9CC *)w->unkA4[9];
                w->unkA0->unk3A = 0;
            }
            if ((w->unk68 & 1) && (s32)w->unk9C->unkC != w->unkA4[9] && (s32)w->unk9C->unkC != w->unkA4[17] &&
                !(w->unk3C & 0x40000000)) {
                if ((s32)w->unk9C->unkC == w->unkA4[(w->unk68 >> 18) & 1] && !((w->unk68 >> 19) & 1)) {
                    w->unk9C->unkC = (BtlUnit9CC *)w->unkA4[9];
                    w->unkA0->unk3A = 0;
                }
                return;
            }
            if (w->unk3C & 0x1000000) {
                func_800BEDBC(w, 0x100);
                return;
            }
        }
        b->unk1088 = cmd;
        func_800B3F98(cmd);
    }
    cmd = b->unk1088;
    if (cmd == NULL) {
        return;
    }
    u = cmd->unk4;
    switch (b->unk11BA) {
    case 0:
        if (!(u8)func_800B1B48(b, cmd) || !(u8)func_800B23B4(b, cmd) || !(u8)func_800B4390(cmd)) {
            func_800B4434(cmd);
            if (u != NULL && (u->unk68 & 1) && (s32)u->unk9C->unkC == u->unkA4[9]) {
                u->unk9C->unkC = (BtlUnit9CC *)u->unkA4[32];
            }
            b->unk11BA = 3;
            break;
        }
        if (u != NULL && (s32)u->unk9C->unkC != u->unkA4[9] && cmd->unkE < 0x2F) {
            u->unk9C->unkC = (BtlUnit9CC *)u->unkA4[9];
            u->unkA0->unk3A = 0;
        }
        b->unk11BA++;
        break;
    case 1:
        func_800C28F4(cmd);
        b->unk11BA++;
        break;
    case 2:
        if ((u->unk68 & 1) || cmd->unk10.b.unk0_11) {
            func_800B253C(cmd);
        }
        break;
    case 3:
        if (cmd->unkE != 0x38) {
            func_800B3C00();
        } else {
            func_800B2988(b);
        }
        break;
    }
    if ((u8)func_800BD31C(cmd) && b->unk11BA == 4) {
        func_800B2988(b);
    }
}

INCLUDE_ASM("asm/us/ovl_0b/nonmatchings/ovl_0b", func_800B1B48);

/* Narrow a command's target mask to units that can still be targeted. */
s32 func_800B23B4(Battle *b, BtlCmd *cmd) {
    BtlCmd8 *a;
    BtlUnit *u;
    Unk8006B1F4 *tbl;
    s32 i;
    u16 mask = 0;
    u16 f;
    u16 r;

    if ((u16)cmd->unkC != 0) {
        switch (cmd->unkE) {
        case 0xE:
        case 0x33:
            tbl = D_8006B1F4;
            i = cmd->unkF - 0xE0;
            a = (BtlCmd8 *)(tbl + i);
            f = (*(u32 *)a >> 29) & 1;
            break;
        case 0x3B:
            return 1;
        default:
            a = cmd->unk8;
            f = (*(u32 *)a >> 29) & 1;
            break;
        }
        for (u = b->unk6F8; u != NULL; u = u->unk0) {
            if (((u->unk68 >> 28) & 1) && (u->unkB0 & (u16)cmd->unkC) &&
                ((f && (u->unk68 & 1)) || !(u->unk3C & 0x100))) {
                mask |= u->unkB0;
            }
        }
        if (mask) {
            cmd->unkC = mask;
            return 1;
        }
        if (cmd->unk10.w & 3) {
            return 0;
        }
        if (!f) {
            r = cmd->unkC = func_800C1F7C(cmd->unkC & 0xF);
            if (r) {
                return 1;
            }
        }
    }
    return 0;
}

void func_800B253C(BtlCmd *arg0) {
    Battle *b;
    BtlUnit *u;
    BtlUnit9C *m;
    s32 *a;
    s32 *a2;
    BtlUnit9C *m2;
    BtlUnit9CC *c;
    s32 k;
    u16 n;
    u16 f;

    if (D_800F78A4->unk0->unk0 != -1) {
        return;
    }
    b = D_8006794C->unk1C->unk8E4.battle;
    u = arg0->unk4;
    if (b->unk1358.unk1C4 != 0 || b->unk1358.unk1CC != NULL) {
        return;
    }
    if (arg0->unkE == 4) {
        m = u->unk9C;
        a = u->unkA4;
        if (m->unkC == (BtlUnit9CC *)a[12]) {
            if (u->unkA0->unk3A < m->unkC->unk2) {
                return;
            }
            m->unkC = (BtlUnit9CC *)a[13];
            u->unkA0->unk3A = 0;
        }
    } else if (arg0->unkE == 7) {
        m = u->unk9C;
        a = u->unkA4;
        if (m->unkC == (BtlUnit9CC *)a[9]) {
            if (u->unkA0->unk3A < m->unkC->unk2) {
                return;
            }
            m->unkC = (BtlUnit9CC *)a[29 + ((u->unk68 >> 8) & 3)];
            u->unkA0->unk3A = 0;
        }
        a2 = u->unkA4;
        m2 = u->unk9C;
        k = (u->unk68 >> 8) & 3;
        c = m2->unkC;
        if (c == (BtlUnit9CC *)a2[29 + k]) {
            n = c->unk2;
            f = u->unkA0->unk3A;
            if (f < n) {
                f++;
                if (k != 0) {
                    u->unk8C[2] = (-(f * 400) / n - 0x618) << 16;
                } else {
                    u->unk8C[2] = ((f * 400) / n - 0x7A8) << 16;
                }
                return;
            }
            func_800B2804(u);
            u->unk9C->unkC = (BtlUnit9CC *)u->unkA4[32];
            u->unkA0->unk3A = 0;
            return;
        }
        if (c == (BtlUnit9CC *)a2[32]) {
            if (u->unkA0->unk3A < c->unk2) {
                return;
            }
            m2->unkC = (BtlUnit9CC *)a2[0];
            u->unkA0->unk3A = 0;
        }
    }
    func_800B3C00();
    if (arg0->unkE == 0x3B) {
        func_800C0D24(u);
    }
}

void func_800B2804(BtlUnit *arg0) {
    BtlCmd *cmd = D_8006794C->unk1C->unk8E4.battle->unk1088;
    BtlUnit *u = cmd->unk4;
    Battle *b;
    u16 s;
    u16 t;
    s32 i;
    Unk8006B1F4 *tbl;

    switch (cmd->unkE) {
    case 0x1:
    case 0x31:
    case 0x34:
        func_800B44C0(u, arg0, cmd, ((BtlCmd8 *)u->unk74)->unk4);
        break;
    case 0x3:
    case 0xC:
        u->unkB2 = 2;
        u->unk68 |= 0x400000;
        break;
    case 0xE:
    case 0x33:
        func_800EA0F8(cmd->unkF);
        tbl = D_8006B1F4;
        i = cmd->unkF - 0xE0;
        func_800B44C0(u, arg0, cmd, ((BtlCmd8 *)(tbl + i))->unk4);
        break;
    case 0x3A:
        s = func_800BEC64(1);
        t = func_800C1CA0(1, 0x1103);
        if (s == 0 || s == t) {
            b = D_8006794C->unk1C->unk8E4.battle;
            func_800E9F68(0);
            if (b->unkD80 != 5) {
                b->unkD80 = 5;
                b->unkD81 = 0;
                func_800B2F24(b);
            }
        }
        func_800B44C0(u, arg0, cmd, cmd->unk8->unk4);
        break;
    case 0xF:
        func_800EA0F8(cmd->unkF);
    default:
        func_800B44C0(u, arg0, cmd, cmd->unk8->unk4);
        break;
    }
}

void func_800B2988(Battle *b) {
    BtlCmd *cmd = b->unk1088;
    BtlUnit *u = cmd->unk4;
    u32 n;

    if (cmd->unkE < 0x30) {
        if ((cmd == u->unk4 && cmd->unkE != 3 && cmd->unkE != 0xC) ||
            (cmd->unkE == 0x15 && ((u->unk68 >> 1) & 0xF) == 6 && cmd == u->unk10 && !func_800B3E7C(u->unk4))) {
            u->unk2C = 0;
            u->unkB3 = 0;
        } else if (cmd->unkE == 0xA) {
            func_800BF558(u, 0x40000000);
            D_8006794C->unk1C->unk8E4.battle->unk11B8 &= ~0x10;
        } else if (cmd->unkE == 0xB) {
            func_800BEDBC(u, 0x40000000);
            u->unkB2 = 2;
            u->unk68 |= 0x400000;
            D_8006794C->unk1C->unk8E4.battle->unk11B8 &= ~0x10;
        }
        if ((u->unk3C & 0x4000) && cmd->unkE != 3 && cmd->unkE != 0xC) {
            n = (0x12C - u->unk7A) / u->unk33 * 10;
            if (cmd->unkE == 0x15 || cmd->unkE == 0x17) {
                n = (u8)n >> 1;
            }
            if ((u8)n < u->unk78) {
                u->unk78 -= n;
            } else {
                func_800BF558(u, 0x4000);
            }
            if (cmd->unkE == 0x12 && b->unk11BB != 0) {
                b->unk11B8 |= 4;
                b->unk11BC = (0x3C - u->unk33) * 200;
            }
        }
    } else if (cmd->unkE == 0x39) {
        b->unk11B8 &= ~8;
        b->unk11BC = (0x3C - u->unk33) * 200;
    } else if (cmd->unkE < 0x35 && func_800B3E7C(u->unk4)) {
        u->unk68 |= 0x80000;
    }
    if (cmd->unk10.b.unk0_5) {
        for (u = D_8006794C->unk1C->unk8E4.battle->unk6F8; u != NULL; u = u->unk0) {
            if (u->unk68 & 1) {
                u->unk8C[0] = u->unk98->unk0[0];
                u->unk8C[1] = u->unk98->unk0[1];
                u->unk8C[2] = u->unk98->unk0[2];
                if ((u->unk68 >> 25) & 1) {
                    u->unk68 &= ~0x2000000;
                    func_800BC78C(u);
                }
            }
        }
    }
    if (cmd->unk10.b.unk0_6) {
        u = func_800BED10(cmd->unkC);
        u->unk68 &= ~0x4000000;
        if (u->unk68 & 1) {
            func_800BC78C(u);
        } else if ((u->unk68 >> 23) & 1) {
            u = func_800C1DEC();
        }
        u->unk8C[2] = u->unk98->unk0[2];
    }
    func_800B3CEC_inline(cmd);
    b->unk1088 = NULL;
    b->unk11BA = 0;
}

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

void func_800B3090(BtlCmd *arg0) {
    BtlCmd *p = &D_8006794C->unk1C->unk8E4.battle->unk1074;
    BtlCmd *n;
    s32 c;
    u32 e;

    for (; p != NULL; p = p->unk0) {
        if (arg0->unk10.b.unk0_4) {
            n = p->unk0;
            if (n == NULL) {
                goto insert;
            }
            if (arg0->unkE >= 0x3C || arg0->unkE == 0x36) {
                e = n->unkE;
                c = e < 0x3C;
                goto chk;
            }
            if (arg0->unkE == 0x3B) {
                e = n->unkE;
                c = e < 0x3B;
            chk:
                if (c == 0 || e == 0x36) {
                    continue;
                }
            insert:
                func_800B440C((LinkNode *)arg0, (LinkNode **)&p->unk0);
                return;
            }
            c = n->unk10.b.unk0_4;
        } else {
            c = (s32)p->unk0;
        }
        if (c == 0) {
            goto insert;
        }
    }
}

s32 func_800B3188(BtlCmd *arg0) {
    u16 buf[4];
    s16 n;
    s16 m;
    s32 i;
    u32 mask = 0;
    u32 side;
    BtlUnit *v;

    if ((u32)(arg0->unkE - 14) >= 2 && arg0->unkE != 0x33 && (arg0->unk8->unk8 & 1) &&
        !(arg0->unk4->unkC4[0] & 0x40000000)) {
        n = 0;
        m = 0;
        v = D_8006794C->unk1C->unk8E4.battle->unk6F8;
        side = (u16)arg0->unkC >= 0x10;
        for (; v != NULL; v = v->unk0) {
            if (v->unkB0 & (u16)arg0->unkC) {
                if (!(v->unk3C & 1) && (u8)func_800C09A4(v, 0x20000000)) {
                    m++;
                    mask |= v->unkB0;
                }
            } else if (!(v->unk3C & 0x100) && (v->unk68 & 1) == side && ((v->unk68 >> 28) & 1)) {
                buf[n++] = v->unkB0;
            }
        }
        if (m != 0 && n != 0) {
            for (i = 0; i < 4; i++) {
                if (i < m) {
                    arg0->unk4->unk1C[i] = buf[RAND8() % n];
                } else {
                    arg0->unk4->unk1C[i] = 0;
                }
            }
            if ((u16)mask == (u16)arg0->unkC) {
                arg0->unk10.b.unk0_7 = 1;
                arg0->unkC = func_800B4478(arg0->unk4->unk1C);
            } else {
                arg0->unk10.b.unk0_7 = 2;
                arg0->unkC &= ~mask;
            }
        }
    }
    return (u16)mask;
}

void func_800B3460(GameData *arg0, Battle *arg1, BtlUnit *arg2) {
    SVECTOR v;
    SVECTOR tri[3];
    long sxy[4];
    s32 ang;
    s32 i;

    ang = (arg1->unkD7C & 0xF) << 8;
    gte_SetRotMatrix(&arg0->unk18);
    gte_SetTransMatrix(&arg0->unk18);
    v.vx = arg2->unk8C[0] >> 16;
    v.vy = (arg2->unk8C[1] >> 16) - arg2->unk9C->unk8->unk8;
    v.vz = arg2->unk8C[2] >> 16;
    gte_ldv0(&v);
    gte_rtps();
    gte_stsxy(&sxy[0]);
    for (i = 0; i < 3; i++) {
        tri[i].vx = v.vx - ((func_8004BB7C(ang) * 68) >> 12);
        tri[i].vy = v.vy - 0x85;
        tri[i].vz = v.vz - ((func_8004BB80(ang) * 68) >> 12);
        ang = (ang + 0x4F1) & 0xFFF;
    }
    gte_ldv3c(tri);
    gte_rtpt();
    gte_stsxy3(&sxy[1], &sxy[2], &sxy[3]);
    if (NormalClip(sxy[0], sxy[1], sxy[2]) > 0) {
        (arg1->unk108C + arg0->unk8)->unk0[0].unk8.x0 = sxy[0];
        (arg1->unk108C + arg0->unk8)->unk0[0].unk8.y0 = sxy[0] >> 16;
        (arg1->unk108C + arg0->unk8)->unk0[0].unk8.x1 = sxy[1];
        (arg1->unk108C + arg0->unk8)->unk0[0].unk8.y1 = sxy[1] >> 16;
        (arg1->unk108C + arg0->unk8)->unk0[0].unk8.x2 = sxy[2];
        (arg1->unk108C + arg0->unk8)->unk0[0].unk8.y2 = sxy[2] >> 16;
        addPrim(&arg0->unk10[arg0->unk8][2], &(arg1->unk108C + arg0->unk8)->unk0[0]);
    }
    if (NormalClip(sxy[0], sxy[2], sxy[3]) > 0) {
        (arg1->unk108C + arg0->unk8)->unk0[1].unk8.x0 = sxy[0];
        (arg1->unk108C + arg0->unk8)->unk0[1].unk8.y0 = sxy[0] >> 16;
        (arg1->unk108C + arg0->unk8)->unk0[1].unk8.x1 = sxy[2];
        (arg1->unk108C + arg0->unk8)->unk0[1].unk8.y1 = sxy[2] >> 16;
        (arg1->unk108C + arg0->unk8)->unk0[1].unk8.x2 = sxy[3];
        (arg1->unk108C + arg0->unk8)->unk0[1].unk8.y2 = sxy[3] >> 16;
        addPrim(&arg0->unk10[arg0->unk8][2], &(arg1->unk108C + arg0->unk8)->unk0[1]);
    }
    if (NormalClip(sxy[0], sxy[3], sxy[1]) > 0) {
        (arg1->unk108C + arg0->unk8)->unk0[2].unk8.x0 = sxy[0];
        (arg1->unk108C + arg0->unk8)->unk0[2].unk8.y0 = sxy[0] >> 16;
        (arg1->unk108C + arg0->unk8)->unk0[2].unk8.x1 = sxy[3];
        (arg1->unk108C + arg0->unk8)->unk0[2].unk8.y1 = sxy[3] >> 16;
        (arg1->unk108C + arg0->unk8)->unk0[2].unk8.x2 = sxy[1];
        (arg1->unk108C + arg0->unk8)->unk0[2].unk8.y2 = sxy[1] >> 16;
        addPrim(&arg0->unk10[arg0->unk8][2], &(arg1->unk108C + arg0->unk8)->unk0[2]);
    }
    if (NormalClip(sxy[1], sxy[3], sxy[2]) > 0) {
        (arg1->unk108C + arg0->unk8)->unk0[3].unk8.x0 = sxy[1];
        (arg1->unk108C + arg0->unk8)->unk0[3].unk8.y0 = sxy[1] >> 16;
        (arg1->unk108C + arg0->unk8)->unk0[3].unk8.x1 = sxy[3];
        (arg1->unk108C + arg0->unk8)->unk0[3].unk8.y1 = sxy[3] >> 16;
        (arg1->unk108C + arg0->unk8)->unk0[3].unk8.x2 = sxy[2];
        (arg1->unk108C + arg0->unk8)->unk0[3].unk8.y2 = sxy[2] >> 16;
        addPrim(&arg0->unk10[arg0->unk8][2], &(arg1->unk108C + arg0->unk8)->unk0[3]);
    }
}

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
    s8 k = c->unk10.b.unk0_7;
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
    arg0->unk10.b.unk0_0 = 0;
    arg0->unk10.b.unk0_2 = 0;
    arg0->unk10.b.unk0_4 = 0;
    arg0->unk10.b.unk0_5 = 0;
    arg0->unk10.b.unk0_6 = 0;
    arg0->unk10.b.unk0_7 = 0;
    arg0->unk10.b.unk0_9 = 0;
    arg0->unk10.b.unk0_10 = 0;
    arg0->unk10.b.unk0_11 = 0;
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

void func_800B4234(BtlUnit *arg0) {
    DVECTOR sxy;
    SVECTOR v;
    BtlBone *b;

    if (!((arg0->unk68 >> 22) & 1)) {
        gte_SetRotMatrix(&D_8006794C->unk1C->unk18);
        gte_SetTransMatrix(&D_8006794C->unk1C->unk18);
        b = &arg0->unk9C->unk20[arg0->unk7C];
        v.vx = b->unk14;
        v.vy = b->unk18;
        v.vz = b->unk1C;
        gte_ldv0(&v);
        gte_rtps();
        gte_stsxy(&sxy);
        arg0->unkB8 = sxy.vx;
        arg0->unkBA = sxy.vy;
    }
    arg0->unkBC = 1;
}

void func_800B4320(BtlCmd **arg0, u8 arg1) {
    BtlCmd *p = *arg0;
    *arg0 = p->unk0;
    p->unk10.b.unk0_2 = 0;
    p->unk10.b.unk0_4 = 0;
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

void func_800B44C0(BtlUnit *caster, BtlUnit *target, BtlCmd *cmd, s32 type) {
    BtlCalc c;
    BtlCmd sub;
    BtlCmd sub2;

    c.unk0 = caster;
    c.unk4 = target;
    c.unk8 = (BtlCalc8 *)cmd;
    c.unk18 = 0;
    c.unk1A = c.unk1B = 0;
    c.unk1C = c.unk1E = c.unk20 = c.unk22 = 0;
    switch ((u8)type) {
    case 1:
    case 2:
    case 3:
    case 4:
    case 5:
    case 7:
        if (func_800B9418(&c)) {
            func_800B9AE4(&c);
            if (func_800B9C14(&c) || func_800B8390(&c)) {
                func_800B87D0(&c, type);
                func_800B8A24(&c);
                func_800BA2AC(&c);
                func_800BA308(&c);
                func_800BA3FC(&c);
                if ((caster->unk3C & 0x4000) && ((caster->unk68 >> 1) & 0xF) == 3) {
                    c.unk14 *= 2;
                }
                func_800BA534(&c);
                func_800BA660(&c);
                if (func_800BA6A4(&c)) {
                    func_800B8BE8(&c);
                    func_800BA938((Unk800BA938 *)&c);
                    func_800B8E9C(&c);
                    func_800BAF3C(&c);
                    func_800BAE44(&c);
                    if (c.unk0->unkC4[0] & 0x02000000) {
                        c.unkC = caster->unk74[7];
                        RAND_REFILL2();
                        if (RAND16() % 100 < c.unkC) {
                            c.unk18 |= 0x100;
                        }
                    }
                }
            }
        }
        break;
    case 6:
        {
            s32 v;
            s32 d;
            if (func_800B9E20(&c) && func_800B9DA4(&c) && func_800B9418(&c)) {
                func_800B9AE4(&c);
                if (func_800B9C14(&c) || func_800B8390(&c)) {
                    func_800B87D0(&c, 1);
                    func_800B8A24(&c);
                    func_800BA2AC(&c);
                    func_800BA308(&c);
                    func_800BA3FC(&c);
                    func_800BA534(&c);
                    func_800B8BE8(&c);
                    func_800BA938((Unk800BA938 *)&c);
                    func_800B9A5C(&c);
                    d = c.unkE - c.unk10;
                    if (d <= 0) {
                        d = 1;
                    }
                    if (c.unk14 <= 0) {
                        c.unk14 = 1;
                    }
                    v = d * c.unk14;
                    if (v >= 10000) {
                        v = 9999;
                    }
                    c.unk1C = v;
                    c.unk1E = v;
                    func_800BAE44(&c);
                }
            }
            break;
        }
    case 8:
        if (func_800B9418(&c)) {
            func_800B9AE4(&c);
            if (func_800B8390(&c)) {
                func_800B9E88(&c);
                func_800BA2AC(&c);
                func_800BA308(&c);
                func_800BA3FC(&c);
                func_800BA534(&c);
                func_800BA8F0(&c);
                if (func_800B8D58(&c)) {
                    func_800B8BE8(&c);
                    func_800B8E9C(&c);
                    func_800BAF3C(&c);
                    func_800BABAC(&c);
                }
            }
        }
        break;
    case 9:
        func_800B9F80(&c);
        func_800BA468(&c);
        func_800BA4AC(&c);
        func_800BA858(&c);
        func_800BA8F0(&c);
        if (func_800B8D58(&c) && func_800BA7D0(&c)) {
            func_800BAA28(&c);
            func_800BABAC(&c);
        }
        break;
    case 10:
        func_800B9F80(&c);
        func_800BA4F0(&c);
        func_800BA468(&c);
        func_800BA858(&c);
        func_800BA994(&c);
        break;
    case 0x6C:
        if (func_800B9E50(&c)) {
            func_800B9AFC(&c);
            func_800B9C30(&c);
            func_800B9C74(&c);
            if (func_800B8664(&c)) {
                func_800BACB4(&c);
            }
        }
        break;
    case 0xC:
        func_800BAD64(&c);
        break;
    case 0xD:
        {
            s32 v;
            if (func_800B9DDC(&c) && (func_800B9B64(&c) || func_800B8664(&c))) {
                if (!(target->unk3C & 0x40) && func_800C1E88(target, 0x10) == 0) {
                    if (func_800B9D6C(&c)) {
                        v = target->unk24->unk0 * (target->unk33 + cmd->unk8->unk5);
                        if (caster->unkC4[1] & 2) {
                            v = v / 50;
                        } else {
                            v = v / 100;
                        }
                        if (v >= 10000) {
                            v = 9999;
                        }
                        c.unk1E = v;
                        c.unk1B |= 3;
                        func_800BAD64(&c);
                    }
                } else {
                    func_800B97A0(target);
                }
            }
            break;
        }
    case 0xE:
        if (func_800B9E50(&c)) {
            if ((target->unk3C & 0x40) || func_800C1E88(target, 0x10) != 0) {
                if (func_800B9DA4(&c)) {
                    target->unk28 = target->unk24->unk0;
                }
            } else {
                func_800B9AFC(&c);
                if (func_800B8664(&c)) {
                    func_800BACB4(&c);
                }
            }
        }
        break;
    case 0xF:
        {
            s32 v;
            s32 d;
            if (func_800B9E20(&c) && func_800B9DA4(&c)) {
                func_800B9F80(&c);
                func_800BA468(&c);
                func_800BA4AC(&c);
                v = 0;
                c.unk1B |= 8;
                d = c.unkE - c.unk10;
                c.unk1A |= 8;
                if (d > 0) {
                    if (c.unk14 <= 0) {
                        c.unk14 = 1;
                    }
                    v = d * c.unk14 >> 2;
                    if (v >= 10000) {
                        v = 9999;
                    }
                }
                if ((target->unk3C & 0x40) || func_800C1E88(target, 0x10) != 0) {
                    c.unk1B |= 0x10;
                    if (caster->unk2A < v) {
                        v = caster->unk2A;
                    }
                } else {
                    c.unk1A |= 0x10;
                    if (target->unk2A < v) {
                        v = target->unk2A;
                    }
                }
                c.unk20 = v;
                c.unk22 = v;
            }
            break;
        }
    case 0x10:
        {
            s32 v;
            s32 d;
            if (func_800B9E20(&c) && func_800B9DA4(&c)) {
                func_800B9F80(&c);
                func_800BA468(&c);
                func_800BA4AC(&c);
                func_800B9A5C(&c);
                d = c.unkE - c.unk10;
                if (d > 0) {
                    if (c.unk14 <= 0) {
                        c.unk14 = 1;
                    }
                    v = d * c.unk14;
                    if (v >= 10000) {
                        v = 9999;
                    }
                    c.unk1C = v;
                    c.unk1E = v;
                }
            }
            break;
        }
    case 0x11:
    case 0x5B:
        {
            s32 v;
            if (func_800B9D34(&c)) {
                func_800B9AFC(&c);
                func_800B9C30(&c);
                func_800B9C74(&c);
                if (func_800B8664(&c)) {
                    func_800BA078(&c);
                    func_800BA8F0(&c);
                    if (func_800B8D58(&c) && func_800BA7D0(&c)) {
                        if ((u8)type == 0x11) {
                            func_800BAAE4(&c);
                        } else {
                            if (c.unk14 > 100) {
                                c.unk14 = 100;
                            }
                            v = target->unk28 * c.unk14 / 100;
                            if (v >= 10000) {
                                v = 9999;
                            }
                            c.unk1B |= 1;
                            if (c.unk18 & 8) {
                                c.unk1B |= 2;
                            }
                            c.unk1E = v;
                        }
                    }
                }
            }
            break;
        }
    case 0x12:
        if ((cmd->unk8->unk0_8 != 0x38 || RAND8() < 0xAB) && (cmd->unkF != 0x2E || !cmd->unk10.b.unk0_9)) {
            RAND_REFILL2();
            c.unk14 = RAND16() % (caster->unk32 + caster->unk7A);
            c.unkE = cmd->unk8->unk5;
            c.unk10 = 0;
            func_800BA468(&c);
            func_800BA4AC(&c);
            func_800BA858(&c);
            func_800BA8F0(&c);
            if (func_800B8D58(&c) && func_800BA7D0(&c)) {
                func_800BAA28(&c);
                func_800BABAC(&c);
            }
        } else {
            c.unk18 |= 1;
        }
        break;
    case 0x13:
        func_800BA094(&c);
        func_800BA2AC(&c);
        func_800BA3FC(&c);
        func_800BA8F0(&c);
        if (func_800BA6A4(&c)) {
            func_800BAA28(&c);
            func_800BABAC(&c);
        }
        break;
    case 0x14:
        {
            s32 i;
            RAND_REFILL2();
            i = caster->unk31;
            c.unk14 = i + RAND16() % (((caster->unk7A + i) >> 3) + 1);
            c.unkE = caster->unk74[5] * cmd->unk8->unk5 / 10;
            c.unk10 = target->unk6E;
            func_800BA468(&c);
            func_800BA4AC(&c);
            func_800BA8F0(&c);
            if (func_800B8D58(&c) && func_800BA7D0(&c)) {
                func_800BAA28(&c);
                func_800BABAC(&c);
            }
            break;
        }
    case 0x15:
        func_800B9F80(&c);
        if (target->unk7A == caster->unk7A) {
            c.unk14 += caster->unk7A;
            c.unk10 = 0;
        }
        func_800BA468(&c);
        func_800BA4AC(&c);
        func_800BAA28(&c);
        break;
    case 0x16:
        if (func_800B9CDC(&c) && func_800B9DA4(&c)) {
            func_800B8FF8(&c);
        }
        break;
    case 0x17:
        if (func_800B9CDC(&c) && func_800B9DA4(&c)) {
            func_800B9F80(&c);
            func_800BA468(&c);
            func_800BA4AC(&c);
            func_800BA8F0(&c);
            if (func_800B8D58(&c)) {
                func_800BAA28(&c);
            }
        }
        break;
    case 0x18:
        if (func_800B9CDC(&c) && func_800B9DA4(&c)) {
            if (target->unk6C != 0) {
                RAND_REFILL2();
                target->unk6C = RAND16() % target->unk6C;
            }
            if (target->unk6E != 0) {
                RAND_REFILL2();
                target->unk6E = RAND16() % target->unk6E;
            }
        }
        break;
    case 0x19:
        if (func_800B9D34(&c) && func_800B9DA4(&c)) {
            func_800B8FF8(&c);
        }
        break;
    case 0x1B:
        if (func_800B9D34(&c) && func_800B9DA4(&c)) {
            func_800B9AFC(&c);
            func_800B9C30(&c);
            if (func_800B8664(&c)) {
                func_800B8FF8(&c);
            }
        }
        break;
    case 0x1C:
        if (caster->unk28 == 1) {
            c.unk1B |= 1;
            c.unk1E = cmd->unk8->unk7 + cmd->unk8->unk5 * 100;
        } else {
            c.unk18 |= 1;
        }
        break;
    case 0x1A:
        c.unk1B |= 1;
        c.unk1E = cmd->unk8->unk7 + cmd->unk8->unk5 * 100;
        break;
    case 0x1D:
        c.unk1B |= 1;
        c.unk1E = caster->unk24->unk0 - caster->unk28;
        break;
    case 0x1E:
        c.unk1B |= 1;
        if (!(target->unk3C & 0x40)) {
            c.unk1B |= 2;
        }
        c.unk1E = caster->unk24->unk0 / 3;
        break;
    case 0x1F:
        c.unk1B |= 8;
        if (target->unk2A != 0) {
            RAND_REFILL2();
            c.unk22 = RAND16() % target->unk2A;
            if (c.unk22 >= 10000) {
                c.unk22 = 9999;
            }
        }
        break;
    case 0x20:
        {
            s32 v;
            s32 d;
            func_800BA094(&c);
            func_800BA8F0(&c);
            if (func_800B8D58(&c)) {
                c.unk1B |= 1;
                d = c.unkE - c.unk10;
                if (d > 0) {
                    if (c.unk14 <= 0) {
                        c.unk14 = 1;
                    }
                    v = d * c.unk14;
                    c.unk1A |= 1;
                    c.unk1C = caster->unk24->unk0 >> 3;
                    if (v >= 10000) {
                        v = 9999;
                    }
                    if (c.unk18 & 8) {
                        c.unk1B |= 2;
                    }
                    c.unk1E = v;
                }
            }
            break;
        }
    case 0x21:
    case 0x22:
    case 0x23:
    case 0x24:
        func_800B9AFC(&c);
        func_800B9C30(&c);
        if (func_800B8664(&c)) {
            if ((u8)type == 0x21) {
                target->unk6C >>= 1;
            } else if ((u8)type == 0x22) {
                target->unk31 = target->unk31 * 3 / 4;
            } else if ((u8)type == 0x23) {
                target->unk6E >>= 1;
            } else if ((u8)type == 0x24) {
                target->unk32 = target->unk32 * 3 / 4;
            }
        }
        break;
    case 0x25:
        c.unk1B |= 0x1B;
        if (caster->unkC4[0] & 0x20000000) {
            c.unk1E = target->unk24->unk0 * cmd->unk8->unk5 / 50;
            c.unk22 = target->unk24->unk2 * cmd->unk8->unk5 / 50;
        } else {
            c.unk1E = target->unk24->unk0 * cmd->unk8->unk5 / 100;
            c.unk22 = target->unk24->unk2 * cmd->unk8->unk5 / 100;
        }
        break;
    case 0x26:
        {
            u32 v;
            u32 d;
            v = cmd->unk8->unk5 * caster->unk7A;
            d = D_8006794C->unk1C->unk560 + v;
            c.unk1B |= 1;
            v = v * v / 10;
            v = v * caster->unk33 / d;
            if (cmd->unkE == 0x1D) {
                v = v / func_800C1F00(0);
            }
            if (v < 10000) {
                c.unk1E = v;
            } else {
                c.unk1E = 9999;
            }
            break;
        }
    case 0x27:
        {
            s32 v;
            s32 d;
            func_800BA094(&c);
            func_800BA2AC(&c);
            func_800BA3FC(&c);
            c.unk1B |= 9;
            d = c.unkE - c.unk10;
            if (d > 0) {
                if (c.unk14 <= 0) {
                    c.unk14 = 1;
                }
                v = d * c.unk14;
                if (v >= 10000) {
                    v = 9999;
                }
                c.unk1E = v;
                c.unk22 = v >> 4;
            }
            break;
        }
    case 0x28:
        {
            s32 v;
            BtlUnit *u;
            v = target->unk24->unk0 - target->unk28;
            c.unk1B |= 1;
            if (v >= 10000) {
                v = 9999;
            }
            c.unk1E = v;
            for (u = D_8006794C->unk1C->unk8E4.battle->unk6F8; u != NULL; u = u->unk0) {
                if ((u->unk68 & 0x10000001) == 0x10000000) {
                    c.unk4 = u;
                    func_800B7954(&c);
                    func_800CC044(u);
                }
            }
            return;
        }
    case 0x29:
        {
            BtlUnit *u;
            if (!(target->unk3C & 0x100)) {
                c.unk1A = 0x18;
                RAND_REFILL2();
                c.unk20 = RAND16() % (target->unk7A * 2);
                for (u = D_8006794C->unk1C->unk8E4.battle->unk6F8; u != NULL; u = u->unk0) {
                    if ((u->unk68 & 0x10000001) == 0x10000001) {
                        c.unk0 = u;
                        func_800B7954(&c);
                        func_800CC044(u);
                    }
                }
                return;
            }
            c.unk18 |= 1;
            break;
        }
    case 0x2A:
        {
            s32 i;
            if (func_800B9418(&c)) {
                RAND_REFILL2();
                i = caster->unk31;
                c.unk14 = i + RAND16() % (((caster->unk7A + i) >> 3) + 1);
                c.unkE = D_8006E0E8[cmd->unkF].unk5 * 2;
                c.unk10 = target->unk6C;
                if (caster->unkC4[0] & 0x10000000) {
                    c.unk14 = c.unk14 * 3 >> 1;
                }
                func_800BA2AC(&c);
                func_800BA3FC(&c);
                if (func_800C1E88(target, 0x80) != 0) {
                    c.unk14 = c.unk14 * 3 >> 1;
                }
                func_800B8E9C(&c);
            }
            break;
        }
    case 0x2B:
        target->unk31 += target->unk31 / cmd->unk8->unk5;
        if (target->unk31 >= 100) {
            target->unk31 = 99;
        }
        break;
    case 0x2C:
        target->unk32 += target->unk32 / cmd->unk8->unk5;
        if (target->unk32 >= 100) {
            target->unk32 = 99;
        }
        break;
    case 0x2D:
        if (!(target->unk3C & 0x100) && !(caster->unk3C & 0x101)) {
            target->unk28 = target->unk24->unk0;
            target->unk2A = target->unk24->unk2;
            caster->unk2A = 0;
            func_800B97A0(caster);
        } else {
            c.unk18 |= 1;
        }
        break;
    case 0x2E:
        {
            s32 v;
            s32 d;
            if ((u32)(D_8006794C->unk1C->unk3C[(caster->unk68 >> 1) & 0xF].unk39[0] - 7) < 9) {
                v = D_8006794C->unk1C->unk8E4.battle->unk22CC[caster->unk74[1]];
                if (!(v & 0x100) || func_800B9E50(&c)) {
                    d = func_800C0ACC(target, v);
                    if (d == 0) {
                        c.unk18 |= 0x40;
                    } else if (d == 1) {
                        c.unk18 |= 1;
                    }
                }
            } else {
                c.unk18 |= 1;
            }
            break;
        }
    case 0x30:
    case 0x53:
        {
            s32 v;
            s32 d;
            func_800B87D0(&c, (u8)type == 0x30 ? 1 : 5);
            if (caster->unkC4[0] & 0x200000) {
                c.unk14 *= 2;
            } else {
                c.unk14 = c.unk14 * 3 >> 1;
            }
            func_800BA468(&c);
            func_800BA3FC(&c);
            if ((u8)type == 0x30) {
                func_800B8E9C(&c);
            } else {
                d = c.unkE - c.unk10;
                if (d <= 0) {
                    d = 1;
                }
                c.unk1B |= 1;
                if (c.unk14 < 4) {
                    c.unk14 = 4;
                }
                v = (u32)(d * c.unk14) / func_800C1F00(0);
                if (v >= 10000) {
                    v = 9999;
                }
                c.unk1E = v;
            }
            break;
        }
    case 0x31:
        {
            s32 i;
            func_800B3CEC(&sub);
            sub.unkE = cmd->unkE;
            sub.unk10.b.unk0_10 = cmd->unk10.b.unk0_10;
            if (target->unk68 & 1) {
                sub.unkF = 0x49;
                i = 13;
            } else {
                sub.unkF = 0x48;
                i = 9;
            }
            sub.unk8 = (BtlCmd8 *)&D_8006794C->unk1C->unk8E4.battle->unk16CC[sub.unkF];
            func_800B44C0(caster, target, &sub, i);
            return;
        }
    case 0x32:
        {
            s8 v;
            if (func_800B9DA4(&c)) {
                RAND_REFILL2();
                v = RAND16() % 100;
                if (v < 10) {
                    target->unk28 = target->unk24->unk0;
                    target->unk2A = target->unk24->unk2;
                } else if (v < 30) {
                    target->unk28 = target->unk24->unk0;
                } else if (v < 50) {
                    target->unk2A = target->unk24->unk2;
                } else if (v < 65) {
                    target->unk28 = 1;
                } else if (v < 80) {
                    target->unk2A = 1;
                } else {
                    target->unk28 = 1;
                    target->unk2A = 1;
                }
            }
            break;
        }
    case 0x33:
        target->unk73 |= cmd->unk8->unk6;
        target->unk70 &= ~cmd->unk8->unk6;
        target->unk71 &= ~cmd->unk8->unk6;
        target->unk72 &= ~cmd->unk8->unk6;
        func_800CD5C8(0xE, func_800CD70C(cmd->unk8->unk6));
        break;
    case 0x34:
        if (func_80027F34(cmd->unk8->unk5) != 0) {
            func_800B3CEC(&sub2);
            sub2.unkE = 0xE;
            sub2.unkF = cmd->unk8->unk5;
            sub2.unk8 = (BtlCmd8 *)&D_8006794C->unk1C->unk8E4.battle->unk16CC[0];
            func_800B44C0(caster, target, &sub2, D_8006B1F4[sub2.unkF - 0xE0].unk4);
            func_800EA164(sub2.unkF);
            return;
        }
        c.unk18 |= 1;
        break;
    case 0x35:
        {
            s32 v;
            c.unk1B |= 1;
            v = caster->unk28 % 10;
            if (v == 7) {
                switch (RAND8() % 4) {
                case 0:
                    c.unk1E = 7;
                    break;
                case 1:
                    c.unk1E = 77;
                    break;
                case 2:
                    c.unk1E = 777;
                    break;
                default:
                    c.unk1E = 7777;
                    break;
                }
            } else {
                c.unk1E = 1;
            }
            break;
        }
    case 0x36:
        {
            s32 v;
            if ((target->unk68 & 1) || !((D_8006794C->unk1C->unk8E4.battle->unk132C.unk0 >> 2) & 1)) {
                v = (caster->unk68 & 1) << 11;
                target->unk86[1] = v;
                target->unkA0->unk22 = v;
                if ((target->unk68 & 1) && (&D_8006794C->unk1C->unk3C[(target->unk68 >> 1) & 0xF])->unk34.b.unk9 != ((target->unk68 >> 8) & 3)) {
                    func_800BDA60(target);
                }
            } else {
                c.unk18 |= 1;
            }
            break;
        }
    case 0x37:
        func_800BDA60(target);
        break;
    case 0x38:
        if (func_800BAEA8(&c)) {
            func_800B918C(&c);
        }
        break;
    case 0x39:
        func_800C19F8(0);
        if (func_800BAEA8(&c)) {
            func_800B1250(&D_8006794C->unk1C->unk8E4.battle->unk1060, 0x38, 0, 0xF, 1);
        }
        break;
    case 0x3A:
        {
            s32 v;
            s16 d;
            s16 k;
            Battle488 *e;
            e = &D_8006794C->unk1C->unk8E4.battle->unk488[(target->unk68 >> 1) & 0xF];
            d = 0;
            for (k = 0; k < 4; k++) {
                if (e->unk8.b[k] != 0xFF) {
                    d = 1;
                }
            }
            if (!d) {
                func_800CD5C8(0xB);
                break;
            }
            if (!(caster->unkC4[1] & 0x40000000)) {
                c.unk16 = caster->unk7A + caster->unk33;
                c.unk12 = target->unk7A;
                RAND_REFILL2();
                RAND_REFILL2();
                v = ((*D_8007B720 << 8) | *D_8007B720) % c.unk16 < ((*D_8007B720 << 8) | *D_8007B720) % c.unk12;
                D_8007B720++;
                if (v) {
                    func_800CD5C8(0xA);
                    break;
                }
            }
            if (caster->unkC4[0] & 0x400000) {
                if (e->unk8.b[3] != 0xFF && RAND8() < 32) {
                    func_800B7F20(&c, (Unk800B7F20 *)e, 3);
                } else if (e->unk8.b[2] != 0xFF && RAND8() < 32) {
                    func_800B7F20(&c, (Unk800B7F20 *)e, 2);
                } else if (e->unk8.b[1] != 0xFF && RAND8() < 64) {
                    func_800B7F20(&c, (Unk800B7F20 *)e, 1);
                } else {
                    func_800B7F20(&c, (Unk800B7F20 *)e, 0);
                }
            } else {
                if (RAND8() < 1) {
                    func_800B7F20(&c, (Unk800B7F20 *)e, 3);
                } else if (RAND8() < 16) {
                    func_800B7F20(&c, (Unk800B7F20 *)e, 2);
                } else if (RAND8() < 64) {
                    func_800B7F20(&c, (Unk800B7F20 *)e, 1);
                } else {
                    func_800B7F20(&c, (Unk800B7F20 *)e, 0);
                }
            }
            break;
        }
    case 0x3B:
        if (func_800B9D34(&c)) {
            func_800CD808((s32)target);
        }
        break;
    case 0x3C:
        func_800CDC68(target);
        break;
    case 0x3D:
        {
            u16 v;
            u16 d;
            BtlUnit *u;
            if ((c.unk18 = func_800C1CA0(0, 0x200)) != 0) {
                d = 0;
                for (u = D_8006794C->unk1C->unk8E4.battle->unk6F8; u != NULL; u = u->unk0) {
                    if ((u->unkB0 & c.unk18) && !(u->unk3C & 0x42021D03)) {
                        d = 1;
                        v = func_800C1F7C(0);
                        if (v == 0) {
                            return;
                        }
                        func_800B3D54(u, 0x34, 0xB0, v);
                    }
                }
                if (!d) {
                    c.unk18 = 0;
                }
            }
            if (c.unk18 == 0) {
                func_800CD5C8(0x1A);
            }
            return;
        }
    case 0x3E:
        if (func_800C1E88(target, 0x20) != 0) {
            if (func_800B9DA4(&c)) {
                func_800B97A0(target);
                func_800CD5C8(0x19);
            }
        } else {
            func_800BADD4(&c);
        }
        break;
    case 0x3F:
        {
            s32 i;
            RAND_REFILL2();
            i = caster->unk31;
            c.unk14 = i + RAND16() % (((caster->unk7A + i) >> 3) + 1);
            c.unkE = caster->unk74[5] + cmd->unk8->unk5;
            c.unk10 = target->unk6C;
            func_800BA8F0(&c);
            if (func_800B8D58(&c) && func_800BA7D0(&c)) {
                func_800BAA28(&c);
            }
            break;
        }
    case 0x41:
        {
            u8 i;
            if (func_800B9D34(&c) && func_800B9DA4(&c) && func_800C1E88(target, 1) == 0) {
                if (target->unk24->unk0 / cmd->unk8->unk5 < target->unk28) {
                    func_800CD5C8(0x10);
                } else {
                    i = D_8006794C->unk1C->unk8E4.battle->unk488[(target->unk68 >> 1) & 0xF].unk0->unk27;
                    func_800B97A0(target);
                    if (i != 0 && !func_80034070((caster->unk68 >> 1) & 0xF, i)) {
                        func_80034220((caster->unk68 >> 1) & 0xF, i);
                        func_800CD5C8(0x11, func_800EA0B4(i));
                    } else {
                        func_800CD5C8(0x12);
                    }
                }
            } else {
                func_800CD5C8(0x1D);
            }
            break;
        }
    case 0x42:
        {
            s32 v;
            c.unk1B |= 1;
            v = D_8006794C->unk1C->unk568;
            if (v != 0) {
                v = v * caster->unk7A;
                if (v >= 10000) {
                    c.unk1E = 9999;
                } else {
                    c.unk1E = v;
                }
            } else {
                c.unk1E = 1;
            }
            break;
        }
    case 0x43:
        {
            s32 v;
            c.unk1B |= 1;
            v = D_8006794C->unk1C->unk56A * caster->unk30 / 2;
            if (v >= 10000) {
                c.unk1E = 9999;
            } else {
                c.unk1E = v;
            }
            break;
        }
    case 0x44:
        {
            c.unk1B |= 1;
            if (D_8006794C->unk1C->unk56C * D_8006794C->unk1C->unk56C < 10000) {
                c.unk1E = (u16)D_8006794C->unk1C->unk56C * (u16)D_8006794C->unk1C->unk56C;
            } else {
                c.unk1E = 9999;
            }
            break;
        }
    case 0x45:
    case 0x46:
        {
            s32 v;
            c.unk14 = 15;
            c.unkE = D_8006B1F4[cmd->unkF - 0xE0].unk5;
            c.unk10 = 0;
            if (caster->unkC4[0] & 0x08000000) {
                c.unk14 = 30;
            }
            if ((u8)type == 0x45) {
                func_800BA994(&c);
            } else {
                v = c.unkE * c.unk14;
                if (v >= 10000) {
                    v = 9999;
                }
                c.unk1B |= 8;
                if (!(target->unk3C & 0x40) && func_800C1E88(target, 0x10) == 0) {
                    c.unk1B |= 0x10;
                }
                c.unk22 = v;
            }
            break;
        }
    case 0x47:
        if (func_800B9DA4(&c)) {
            if (!(target->unk3C & 0x40) && func_800C1E88(target, 0x10) == 0) {
                target->unk28 = target->unk24->unk0;
                target->unk2A = target->unk24->unk2;
            } else {
                target->unk2A = 0;
                func_800B97A0(target);
            }
        }
        break;
    case 0x48:
        if (func_800B9DDC(&c)) {
            if ((target->unk3C & 0x40) || func_800C1E88(target, 0x10) != 0) {
                target->unk28 = RAND8() % 10;
                if (target->unk28 == 0) {
                    func_800B97A0(target);
                }
            } else if (func_800B9D6C(&c)) {
                if (target->unk3C & 0x100) {
                    target->unk28 = RAND8() % 10 + 1;
                }
                func_800BADD4(&c);
            }
        }
        break;
    case 0x49:
        func_800BADD4(&c);
        break;
    case 0x4A:
        c.unk1B |= 3;
        c.unk1E = D_8006B1F4[cmd->unkF - 0xE0].unk5 * (func_80027F34(cmd->unkF) + 1);
        break;
    case 0x4B:
        c.unk1B |= 1;
        c.unk1E = D_8006B1F4[cmd->unkF - 0xE0].unk5 * (func_80027F34(cmd->unkF) + 1);
        break;
    case 0x4C:
        c.unk1B |= 0x1B;
        c.unk1E = target->unk24->unk0 >> 1;
        c.unk22 = target->unk24->unk2 >> 1;
        RAND_REFILL2();
        if (D_8006B1F4[cmd->unkF - 0xE0].unk7 < RAND16() % 100) {
            func_800CD5C8(0xD);
            func_800C0ACC(target, D_8006B1F4[cmd->unkF - 0xE0].unk8);
        }
        break;
    case 0x4D:
        if (func_800B9DA4(&c)) {
            c.unk1E = 9999;
            c.unk1B |= 1;
        }
        break;
    case 0x54:
        func_800B9AFC(&c);
        func_800B9C30(&c);
        if (func_800B8664(&c)) {
            func_800EA1B0(0xFE);
        }
        break;
    case 0x55:
        func_800B9F80(&c);
        func_800BA468(&c);
        func_800BA4AC(&c);
        func_800BA8F0(&c);
        if (func_800B8D58(&c) && func_800BA7D0(&c)) {
            /* padded: a zero word follows the first jump table */
            __asm__(".section .rodata\n\t.word 0\n\t.section .text");
            switch (cmd->unkF) {
            case 0x31:
                c.unkE += func_80027F34(0xE9);
                break;
            case 0x33:
                c.unkE += func_80027F34(0xEA);
                break;
            case 0x35:
                c.unkE += func_80027F34(0xE7);
                break;
            case 0x3C:
                c.unkE += func_80027F34(0xE2);
                break;
            case 0x3E:
                c.unkE += func_80027F34(0xE0);
                break;
            case 0x40:
                c.unkE += func_80027F34(0xEB);
                break;
            case 0x42:
            case 0x43:
                c.unkE += func_80027F34(0xE8);
                break;
            case 0x4A:
                c.unkE += caster->unk7A;
                break;
            }
            func_800BAA28(&c);
            func_800BABAC(&c);
        }
        break;
    case 0x56:
        if (func_800B9D34(&c)) {
            func_800BA078(&c);
            func_800BA8F0(&c);
            if (func_800B8D58(&c) && func_800BA7D0(&c)) {
                c.unk14 += func_80027F34(0xE1);
                func_800BAAE4(&c);
            }
        }
        break;
    case 0x57:
        {
            s32 i;
            if (func_800B9E50(&c)) {
                func_800B9AFC(&c);
                c.unk16 += func_80027F34(0xFE) >> 1;
                if (func_800B8664(&c)) {
                    func_800BACB4(&c);
                }
            }
            if ((c.unk18 & 0x41) && (caster->unkC4[1] & 0x10000000)) {
                c.unk18 &= ~0x41;
                RAND_REFILL2();
                i = caster->unk32;
                c.unk14 = i + RAND16() % (((caster->unk7A + i) >> 3) + 1);
                c.unkE = cmd->unk8->unk5 + 100 - func_80027F34(0xFE);
                c.unk10 = target->unk6E;
                func_800BAA28(&c);
            }
            break;
        }
    case 0x58:
        c.unk1B |= 1;
        if (caster->unk28 != 0) {
            caster->unk80 = caster->unk28;
            func_800B97A0(caster);
        }
        c.unk1E = caster->unk80;
        if (c.unk1E >= 10000) {
            c.unk1E = 9999;
        }
        break;
    case 0x59:
        {
            u32 v;
            if (func_800B9D34(&c) && func_800B9DA4(&c)) {
                v = caster->unk28;
                caster->unk28 = target->unk28;
                if (caster->unk24->unk0 < caster->unk28) {
                    caster->unk28 = caster->unk24->unk0;
                }
                target->unk28 = v;
                if (target->unk24->unk0 < v) {
                    target->unk28 = target->unk24->unk0;
                }
            }
            break;
        }
    case 0x5A:
        target->unk6C >>= 1;
        target->unk6E >>= 1;
        break;
    case 0x5C:
        func_800EA1B0(c.unk8->unk8[5]);
        break;
    case 0x5D:
        if (func_800B9D34(&c) && func_800B9DA4(&c)) {
            c.unk18 |= 0x80;
            target->unk28 = RAND8() % 9 + 1;
            func_800C0ACC(target, D_8006794C->unk1C->unk8E4.battle->unk22CC[cmd->unk8->unk9]);
        }
        break;
    case 0x5E:
        {
            target->unk32 >>= 1;
            if (caster->unk32 * 2 >= 0x100) {
                caster->unk32 = 0xFF;
            } else {
                caster->unk32 = caster->unk32 * 2;
            }
            break;
        }
    case 0x5F:
        {
            target->unk31 >>= 1;
            if (caster->unk31 * 2 >= 0x100) {
                caster->unk31 = 0xFF;
            } else {
                caster->unk31 = caster->unk31 * 2;
            }
            break;
        }
    case 0x60:
        target->unk78 = 0xFF;
        func_800BEDBC(target, 0x4000);
        break;
    case 0x61:
        if ((&D_8006794C->unk1C->unk3C[(target->unk68 >> 1) & 0xF])->unkA & 2) {
            c.unk18 |= 1;
        } else {
            func_800BACB4(&c);
        }
        break;
    case 0x62:
        if ((&D_8006794C->unk1C->unk3C[(target->unk68 >> 1) & 0xF])->unkA & 8) {
            c.unk18 |= 1;
        } else {
            func_800B9E88(&c);
            func_800BAA28(&c);
        }
        break;
    case 0x63:
        func_800B9AFC(&c);
        func_800B9C30(&c);
        func_800B9C74(&c);
        if (func_800B8664(&c)) {
            c.unk1B |= 1;
            c.unk1E = target->unk7A * cmd->unk8->unk5;
        }
        break;
    case 0x64:
        if (!func_800B9418(&c)) {
            break;
        }
    case 0x6B:
        func_800BA1B4(&c);
        func_800BA2AC(&c);
        func_800BA308(&c);
        func_800BA3FC(&c);
        func_800BA534(&c);
        func_800BA8F0(&c);
        if (func_800B8D58(&c)) {
            func_800BAA28(&c);
            if ((u8)type == 0x6B && ((target->unk68 >> 8) & 3)) {
                func_800BDA60(target);
            }
            func_800BABAC(&c);
        }
        break;
    case 0x65:
        func_800B9AE4(&c);
        if (func_800B8390(&c)) {
            func_800BAF70(&c);
        } else {
            func_800CD5C8(0xA);
        }
        break;
    case 0x66:
        func_800BA1B4(&c);
        func_800BA2AC(&c);
        func_800BA308(&c);
        func_800BA3FC(&c);
        func_800BA534(&c);
        func_800BAA28(&c);
        func_800BAF70(&c);
        break;
    case 0x68:
        c.unk1B |= 1;
        if (D_8007AEF0.b[0xC0] >= 14) {
            c.unk1E = 9999;
        } else {
            c.unk1E = 1 << (D_8007AEF0.b[0xC0] - 1);
        }
        break;
    case 0x69:
        {
            s32 i;
            s32 bit;
            if (func_800B9DA4(&c)) {
                for (bit = 1, i = 0; i < 32; i++, bit <<= 1) {
                    if ((RAND8() >> 5) == 0) {
                        if (bit & 0x1B031D79) {
                            func_800BEDBC(target, bit);
                        } else if ((bit & 0x200) && !(target->unk3C & 0x100)) {
                            c.unk18 |= 0x80;
                            target->unk28 = RAND8() % 9 + 1;
                        }
                    }
                }
            }
            break;
        }
    case 0x6A:
        D_800FDF30.unkA &= ~(1 << ((u8)target->unk68 >> 5));
        func_800B3DBC(target);
        func_800B4114(target);
        func_800C1854(target, 1);
        func_800C1938(target);
        break;
    case 0x6D:
        if (!(target->unk3C & 0x10000000)) {
            func_800B9AFC(&c);
            func_800B9C30(&c);
            func_800B9C74(&c);
            if (func_800B8664(&c)) {
                func_800BACB4(&c);
            }
        } else {
            func_800BACB4(&c);
        }
        break;
    case 0xB:
        func_800B9AFC(&c);
        func_800B9C30(&c);
        func_800B9C74(&c);
        if (func_800B8664(&c)) {
            func_800BACB4(&c);
        }
        break;
    case 0x67:
        func_800BACB4(&c);
        break;
    }
    if (caster != NULL && target != NULL) {
        func_800B7954(&c);
    }
}

void func_800B7954(BtlCalc *c) {
    BtlUnit *t = c->unk4;
    BtlUnit *s = c->unk0;
    s32 back = 0;
    u16 i;
    u16 n;
    u32 a;
    u16 who;
    s32 id;
    BtlUnit *w;

    if ((t->unk68 & 1) && !(s->unk68 & 1)) {
        back = func_800B065C(t, s, (BtlCmd *)c->unk8);
    }
    if (c->unk18 & 0x40) {
        t->unk7E |= 0x80;
    } else if (c->unk18 & 1) {
        t->unk7E |= 0x20;
        if (c->unk18 & 2) {
            if (t->unk68 & 1) {
                t->unk9C->unkC = (BtlUnit9CC *)t->unkA4[16];
                t->unkA0->unk3A = 0;
                t->unk86[1] = t->unkA0->unk22 = (t->unk68 & 1) * 0x800;
            } else if (!((t->unk68 >> 23) & 1)) {
                t->unk8C[2] += 0x1900000;
            } else {
                w = func_800C1DEC();
                w->unk8C[2] += 0x1900000;
            }
            t->unk68 |= 0x4000000;
            c->unk8->unk10 |= 0x40;
        }
    } else {
        if (c->unk8->unk8[8] & 8) {
            func_800BF558(t, 0x400);
            func_800BF558(t, 0x20000);
        }
        if (c->unk8->unk8[8] & 0x10) {
            func_800BF558(t, 0x4000000);
        }
        if (c->unk1B != 0) {
            t->unk7E |= c->unk1B;
            if (c->unk1B & 1) {
                n = 0;
                if (((c->unk8->unk10 >> 7) & 3) == 1) {
                    for (i = 0; i < 4; i++) {
                        if (s->unk1C[i] & t->unkB0) {
                            n++;
                        }
                    }
                    if (c->unk1E * n >= 10000) {
                        c->unk1E = 9999;
                    } else {
                        c->unk1E = c->unk1E * n;
                    }
                    if ((s32)*s->unkC4 < 0) {
                        c->unk1E *= 2;
                        if (c->unk1E >= 10000) {
                            c->unk1E = 9999;
                        }
                    }
                }
                if (c->unk1B & 2) {
                    func_800BDC04(t, c->unk1E);
                } else {
                    func_800BDB20(t, c->unk1E, func_800B9994(c));
                    func_800B81C4(c, back);
                }
            }
            if (c->unk1B & 8) {
                if (c->unk1B & 0x10) {
                    func_800BDCBC(t, c->unk22);
                } else {
                    func_800BDC64(t, c->unk22);
                }
            }
        } else if (c->unk18 & 0x80) {
            if ((u8)func_800B9994(c)) {
                func_800BCAA4(t);
            }
            func_800B81C4(c, back);
        }
        if (c->unk1A != 0) {
            s->unk7E |= c->unk1A;
            if (c->unk1A & 1) {
                if (c->unk1A & 2) {
                    func_800BDC04(s, c->unk1C);
                } else {
                    func_800BDB20(s, c->unk1C, 0);
                }
            }
            if (c->unk1A & 8) {
                if (c->unk1A & 0x10) {
                    func_800BDCBC(s, c->unk20);
                } else {
                    func_800BDC64(s, c->unk20);
                }
            }
        }
        if ((c->unk18 & 0x100) && t->unk28 != 0) {
            a = D_8006794C->unk1C->unk8E4.battle->unk22CC[s->unk74[1]];
            if (!(a & 0x100) || !(t->unk3C & 0x80)) {
                func_800C0ACC(t, a);
            }
        }
    }
    if (!(t->unk68 & 1)) {
        who = 0x10;
        if (!((t->unk68 >> 23) & 1)) {
            who = t->unkB0;
        }
        id = c->unk8->unkF | (c->unk8->unkE << 8);
        if ((s->unk68 & 1) && !(t->unk3C & 0x2001003)) {
            if ((((D_8006794C->unk1C->unk8E4.battle->unk488 + ((t->unk68 >> 1) & 0xF))->unk4 >> 6) & 1) && t->unk28 == 0) {
                func_8004AB7C(0x36, who, s->unkB0, id);
            } else if (t->unk28 != 0 && c->unk8->unkE < 0x30) {
                func_8004AB7C(0x35, who, s->unkB0, id);
            }
        }
        func_8004AB7C(0x37, who, s->unkB0, id);
    }
}

void func_800B7F20(BtlCalc *arg0, Unk800B7F20 *arg1, s32 arg2) {
    u8 id = arg1->unk8[arg2];
    GameData *gd;
    GameData *g;
    u32 r;

    if (id != 0xFF) {
        arg1->unk8[arg2] = 0xFF;
        g = D_8006794C->unk1C;
        if (g->unk56A < 9999) {
            g->unk56A++;
        }
        func_800EA1B0(id);
        func_800CD5C8(0xC, func_800CD75C(id));
        if (arg0->unk0->unkC4[1] & 0x20000000) {
            arg0->unk1B |= 1;
            if (D_8007B720 >= D_8007B92D) {
                func_8004BF20();
                if (D_8007B720 >= D_8007B92D) {
                    func_8004BF20();
                }
            }
            arg0->unk1E = ((*D_8007B720 << 8) | *D_8007B720++) % ((arg0->unk0->unk7A * arg0->unk4->unk7A) >> 1);
        }
        if (arg0->unk0->unkC4[0] & 0x800000) {
            gd = D_8006794C->unk1C;
            if (D_8007B720 >= D_8007B92D) {
                func_8004BF20();
                if (D_8007B720 >= D_8007B92D) {
                    func_8004BF20();
                }
            }
            gd->unk560 = D_8006794C->unk1C->unk560 +
                         ((*D_8007B720 << 8) | *D_8007B720++) % ((arg0->unk0->unk7A * arg0->unk4->unk7A) / 4 + 1);
            if (D_8006794C->unk1C->unk560 > 9999999) {
                D_8006794C->unk1C->unk560 = 9999999;
            }
        }
    } else {
        func_800CD5C8(0xA);
    }
}

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

u8 func_800B8390(BtlCalc *arg0) {
    if (arg0->unk0->unk3C & 0x410) {
        arg0->unk16 /= 2;
    }
    if (arg0->unk0->unk3C & 0x04004000) {
        arg0->unk12 = 0;
    }
    if (arg0->unk4->unkC4[0] & 0x400) {
        arg0->unk16 /= 2;
    }
    if (arg0->unk4->unk3C & 0x02021417) {
        arg0->unk12 = 0;
    }
    if (arg0->unk4->unk3C & 0x8000) {
        arg0->unk16 /= 2;
    }
    if (arg0->unk4->unk3C & 0x04000000) {
        arg0->unk16 = 0;
    }
    if (D_8007B720 >= D_8007B92D) {
        func_8004BF20();
        if (D_8007B720 >= D_8007B92D) {
            func_8004BF20();
        }
    }
    if (((*D_8007B720 << 8) | *D_8007B720++) % 100 >= arg0->unk16) {
        arg0->unk18 |= 1;
        return 0;
    }
    if (D_8007B720 >= D_8007B92D) {
        func_8004BF20();
        if (D_8007B720 >= D_8007B92D) {
            func_8004BF20();
        }
    }
    if (((*D_8007B720 << 8) | *D_8007B720++) % 100 < arg0->unk12) {
        if (arg0->unk4 != arg0->unk0) {
            arg0->unk18 |= 1;
            if (!((arg0->unk4->unk68 >> 25) & 1)) {
                arg0->unk18 |= 3;
            }
            return 0;
        }
    }
    return 1;
}

u8 func_800B8664(BtlCalc *arg0) {
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

void func_800B87D0(BtlCalc *arg0, u8 arg1) {
    s32 r;

    if (D_8007B720 >= D_8007B92D) {
        func_8004BF20();
        if (D_8007B720 >= D_8007B92D) {
            func_8004BF20();
        }
    }
    r = ((*D_8007B720 << 8) | *D_8007B720++) % (((arg0->unk0->unk7A + arg0->unk0->unk31) >> 3) + 1);
    arg0->unkE = arg0->unk0->unk74[5];
    arg0->unk10 = arg0->unk4->unk6C;
    switch (arg1) {
    case 7:
        arg0->unkE += arg0->unk0->unk7A;
    case 1:
        arg0->unk14 = arg0->unk0->unk31 + r;
        break;
    case 2:
        arg0->unk14 = ((arg0->unk0->unk31 + arg0->unk0->unk33) >> 1) + r;
        break;
    case 3:
        arg0->unk14 = ((arg0->unk0->unk31 + arg0->unk0->unk30) >> 1) + r;
        break;
    case 4:
        arg0->unk14 = ((arg0->unk0->unk31 + arg0->unk0->unk32) >> 1) + r;
        break;
    case 5:
        if (D_8007B720 >= D_8007B92D) {
            func_8004BF20();
            if (D_8007B720 >= D_8007B92D) {
                func_8004BF20();
            }
        }
        arg0->unk14 = ((*D_8007B720 << 8) | *D_8007B720++) % arg0->unk0->unk31 + r;
        break;
    }
}

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

u8 func_800B8D58(BtlCalc *arg0) {
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

void func_800B918C(BtlCalc *arg0) {
    s8 np;
    s8 ne;
    s16 lp;
    s16 le;
    Battle *b;
    BtlUnit *u;
    BtlCmd *c;
    s16 r;
    ne = np = 0;
    b = D_8006794C->unk1C->unk8E4.battle;
    le = lp = 0;
    u = b->unk6F8;
    c = b->unk1088;
    for (; u != NULL; u = u->unk0) {
        if (u->unk68 & 1) {
            np++;
            lp += u->unk7A;
        } else {
            ne++;
            le += u->unk7A;
        }
    }
    if (ne == 0) {
        ne = 1;
    }
    if (np == 0) {
        np = 1;
    }
    if (le == 0) {
        le = 1;
    }
    if (lp == 0) {
        lp = 1;
    }
    r = (200 / (le / ne)) * (lp / np) / 16;
    if (D_8007B720 >= D_8007B92D) {
        func_8004BF20();
        if (D_8007B720 >= D_8007B92D) {
            func_8004BF20();
        }
    }
    if (r > ((*D_8007B720 << 8) | *D_8007B720++) % 100 && (c == NULL || (c->unk4->unk68 & 1))) {
        func_800B1250((s32)&D_8006794C->unk1C->unk8E4.battle->unk1060, 0x38, 1, 0xF, 1);
    }
}

u8 func_800B9418(BtlCalc *arg0) {
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
        arg0->unk10.b.unk0_9 = 1;
    }
}

s32 func_800B96AC(BtlUnit *arg0, s16 arg1) {
    BtlUnit *u;
    u32 f;
    if (arg0->unk2A >= arg1) {
        for (u = D_8006794C->unk1C->unk8E4.battle->unk6F8; u != NULL; u = u->unk0) {
            f = u->unk68;
            if ((f & 1) && D_8006794C->unk1C->unk3C[(f >> 1) & 0xF].unk34.b.unk4 == 2) {
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
            arg0->unk10.b.unk0_10 = 1;
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

u8 func_800B9B64(BtlCalc *arg0) {
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

void func_800B9C74(BtlCalc *arg0) {
    BtlCalc8 *c = arg0->unk8;
    if ((c->unk10 & 3) == 1 && (u32)((*(u32 *)c->unk8 & 0xF) - 3) < 3) {
        arg0->unk16 /= 2;
    }
}

u8 func_800B9CDC(BtlCalc *arg0) {
    if (arg0->unk4->unk7A % arg0->unk8->unk8[7] != 0) {
        arg0->unk18 |= 1;
        return 0;
    }
    return 1;
}

u8 func_800B9D34(BtlCalc *arg0) {
    if (arg0->unk4->unk3C & 0x80) {
        arg0->unk18 |= 1;
        return 0;
    }
    return 1;
}

u8 func_800B9D6C(BtlCalc *arg0) {
    if (!(arg0->unk4->unk68 & 1)) {
        arg0->unk18 |= 1;
        return 0;
    }
    return 1;
}

u8 func_800B9DA4(BtlCalc *arg0) {
    if (arg0->unk4->unk3C & 0x101) {
        arg0->unk18 |= 1;
        return 0;
    }
    return 1;
}

u8 func_800B9DDC(BtlCalc *arg0) {
    u32 st = arg0->unk4->unk3C;
    if ((st & 1) || (st & 0x140) == 0x140) {
        arg0->unk18 |= 1;
        return 0;
    }
    return 1;
}

u8 func_800B9E20(BtlCalc *arg0) {
    if (arg0->unk4 == arg0->unk0) {
        arg0->unk18 |= 1;
        return 0;
    }
    return 1;
}

u8 func_800B9E50(BtlCalc *arg0) {
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

u8 func_800BA6A4(BtlCalc *arg0) {
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

u8 func_800BA7D0(BtlCalc *arg0) {
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
    if ((D_8006794C->unk1C->unk8E4.battle->unk132C.unk0 >> 10) & 1) {
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

u8 func_800BAEA8(BtlCalc *arg0) {
    Battle *b = D_8006794C->unk1C->unk8E4.battle;
    BtlUnit *u;
    if ((u8)b->unk132C.unk0 >> 7 == 0) {
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

void func_800BAFEC(Battle *arg0) {
    s16 n;
    s16 i;
    Unk16D8 *p;
    Character *c;
    BtlUnit *u;
    s32 k;

    i = n = 0;
    p = func_8004AE9C()->unk0;
    for (; i < 4; i++) {
        c = D_8006794C->unk1C->unk54C[i];
        if (c != NULL) {
            u = &arg0->unk6FC[n];
            u->unk84 = D_800F5200[(c->unk34.w >> 4) & 0x1F];
            func_800BBF30(p->unk4, u->unk84);
            func_800BB4A0(c, u, i, n);
            n++;
            p = p->unk0;
            func_800C1990(u);
        }
    }
    for (; n < 4; n++) {
        arg0->unk6FC[n].unkB0 = 0;
    }
    arg0->unkD85 |= 0x10;
    func_800C9A28();
    k = arg0->unk132C.unk0 & 3;
    if (k == 0) {
        for (u = arg0->unk6F8; u != NULL; u = u->unk0) {
            if (u->unk68 & 1) {
                u->unk2C = 0;
            }
        }
    } else if (k == 1) {
        for (u = arg0->unk6F8; u != NULL; u = u->unk0) {
            if (!(u->unk68 & 1)) {
                u->unk2C = 0;
            }
        }
    }
}

void func_800BB1F4(Battle *arg0) {
    s16 i = 0;
    s16 n = 4;
    Unk16D8 *p = func_8004AEBC()->unk0;
    BtlUnit *prev = NULL;
    Battle488 *e;
    BtlUnit *u;
    s16 m;
    s32 k;

    for (; i < D_8006794C->unk1C->unk8E4.battle->unk132C.unk4->unk1; i++, n++) {
        e = &arg0->unk488[i];
        e->unk4 = (e->unk4 & ~0x3F) | 0x20;
        u = &arg0->unk6FC[n];
        u->unk84 = m = func_800CA2E4(i);
        if (m < 0) {
            e->unk4 |= 0x200;
        } else {
            func_800BBF30(p->unk4, m);
            e->unk4 &= ~0x200;
            p = p->unk0;
        }
        k = i;
        u->unkB0 = 0x10 << k;
        u->unk68 &= ~1;
        u->unk68 = (u->unk68 & ~0x1E) | ((k & 0xF) << 1);
        u->unk68 = (u->unk68 & ~0xE0) | (((k + 4) & 7) << 5);
        u->unk68 &= ~0x40000000;
        u->unk68 = (u->unk68 & ~0x800000) | (((e->unk4 >> 9) & 1) << 23);
        func_800BBD3C(u, &D_8006794C->unk1C->unk8E0[k]);
        u->unk98 = (struct BtlUnit98 *)&e->unkC;
        u->unkA8 = NULL;
        u->unkC4 = (u32 *)D_800F5244;
        u->unk68 &= ~0x40000;
        if (prev != NULL) {
            prev->unk0 = u;
        }
        prev = u;
    }
    for (; n < 8; n++) {
        arg0->unk6FC[n].unkB0 = 0;
    }
    prev->unk0 = NULL;
    arg0->unk6F8 = &arg0->unk6FC[4];
    arg0->unkD85 |= 8;
    func_800C9794();
    func_800CA3CC();
}

void func_800BB4A0(Character *c, BtlUnit *u, u16 slot, u16 n) {
    s32 k;
    s32 s;
    s32 i;

    u->unkB0 = 1 << n;
    D_800FDF30.unkA |= 1 << (s = slot);
    u->unk68 |= 1;
    u->unk68 = (u->unk68 & ~0x1E) | ((c->unk34.w & 0xF) << 1);
    u->unk68 |= 0x10000000;
    u->unk68 = (u->unk68 & ~0xE0) | ((s & 7) << 5);
    u->unk68 &= ~0x800000;
    if (D_8007AEF0.b[0x10] == 0 || (c->unkA & 0x10) ||
        (((u->unk68 >> 1) & 0xF) == 2 && D_8007AEF0.b[0x11] != 0)) {
        u->unk78 = 0;
        u->unk68 &= ~0x40000000;
    } else {
        u->unk68 |= 0x40000000;
        u->unk78 = c->unk20;
    }
    u->unk7C = 0;
    u->unkC4 = c->unk88;
    u->unk30 = c->unk24;
    u->unk31 = c->unk25;
    u->unk32 = c->unk26;
    u->unk33 = c->unk27;
    u->unk7A = c->unkB;
    u->unk24 = (BtlUnit24 *)&c->unk18;
    func_800BD8C0((Unk800BD8C0 *)&u->unk28, (Unk800BD8C0 *)&c->unk10);
    func_800BBD3C(u, &D_8006794C->unk1C->unk8D8[c->unk34.w & 0xF]);
    u->unkAE = 0xFFFF;
    func_800C0A3C((Unk800C09D8 *)u);
    u->unk24->unk4 = (0x3C - u->unk30) * 160;
    func_800BD8F0((Unk800BD8F0 *)u);
    k = D_8006794C->unk1C->unk8E4.battle->unk132C.unk0 & 3;
    if (k == 0) {
        u->unk2C = 0;
    } else if (k == 1) {
        u->unk2C = u->unk24->unk4 - 1;
    } else {
        if (D_8007B720 >= D_8007B92D) {
            func_8004BF20();
            if (D_8007B720 >= D_8007B92D) {
                func_8004BF20();
            }
        }
        u->unk2C = ((*D_8007B720 << 8) | *D_8007B720++) % u->unk24->unk4;
    }
    u->unkA8 = &D_8006794C->unk1C->unk8E4.battle->unk228[n].unk0;
    func_800BD698(u, (c->unk34.w >> 4) & 0x1F, n);
    func_800BBF5C(c, u);
    u->unk6C = c->unk28;
    u->unk6D = c->unk29;
    u->unk6E = c->unk2A;
    u->unk6F = c->unk2B;
    func_800BC0D0(c, u);
    for (i = 0; i < u->unkA8->unk8->unk3; i++) {
        func_800C8D18(u->unkA8, i, 0, 0, 0);
    }
    if (u->unk28 * 6 < u->unk24->unk0) {
        u->unk3C |= 0x200;
    }
    func_800C0ACC(u, c->unk38 & 0xFE);
    if (c->unk38 & 1) {
        func_800BEDBC(u, 1);
    }
    func_800B0A6C(u);
    u->unk98 = (struct BtlUnit98 *)&u->unkA0->unk14;
    if (u->unk28 == 0 && func_800BEDBC(u, 0x100) == 2) {
        u->unk7D = 5;
        func_800BD5A0(u);
        return;
    }
    u->unk68 = (u->unk68 & ~0x40000) | ((func_800C09A4(u, 0x30202) & 1) << 18);
    u->unk9C->unkC = (BtlUnit9CC *)u->unkA4[(u->unk68 >> 18) & 1];
    u->unkA0->unk3A = 0;
}

u32 *func_800BB974(Battle *arg0, u32 *arg1) {
    SVECTOR r2;
    SVECTOR r;
    BtlUnit *u;
    BtlUnit9CC *cc;
    s32 t;
    BtlUnitA0 *a0;
    s32 i;
    Battle4880 *p;

    for (i = 0; i < D_8006794C->unk1C->unk8E4.battle->unk132C.unk4->unk1; i++) {
        {
            p = arg0->unk488[i].unk0;
            u = &arg0->unk6FC[i + 4];
            u->unk7A = p->unk3;
            u->unk24->unk4 = (0x3C - u->unk30) * 160;
            func_800BD8F0((Unk800BD8F0 *)u);
            if (D_8007B720 >= D_8007B92D) {
                func_8004BF20();
                if (D_8007B720 >= D_8007B92D) {
                    func_8004BF20();
                }
            }
            u->unk2C = ((*D_8007B720 << 8) | *D_8007B720++) % u->unk24->unk4;
            u->unk74 = NULL;
            func_800C0A3C((Unk800C09D8 *)u);
            func_800BD414(u, u->unkAC);
            if ((u->unk68 >> 23) & 1) {
                u->unk2C = 0;
                u->unk2E = 0;
                func_8001D898(&r, 0, 8);
                r.vx = u->unk86[0];
                r.vy = u->unk86[1];
                r.vz = u->unk86[2];
                r2 = r;
                func_80031244(&r2, &u->unk9C->unk24);
                u->unk9C->unk24.t[0] = u->unkA0->unk14 >> 16;
                u->unk9C->unk24.t[1] = u->unkA0->unk18 >> 16;
                u->unk9C->unk24.t[2] = u->unkA0->unk1C >> 16;
                cc = u->unk9C->unkC;
                if (cc->unkC >= 0) {
                    t = cc->unk10;
                    cc->unkC = (s32)cc + cc->unkC;
                    if (t != 0) {
                        cc->unk10 = (s32)u->unk9C->unkC + t;
                    }
                }
                arg1 = (u32 *)func_800C7DAC(u->unk9C, 0, (u8 *)arg1);
                u->unk98->unk0[0] = u->unk9C->unk20[u->unk7C].unk14 << 16;
                u->unk98->unk0[1] = 0;
                u->unk98->unk0[2] = u->unk9C->unk20[u->unk7C].unk1C << 16;
            } else {
                u->unk98->unk0[0] = u->unkA0->unk14;
                u->unk98->unk0[1] = u->unkA0->unk18;
                u->unk98->unk0[2] = u->unkA0->unk1C;
                a0 = u->unkA0;
                if (D_8007B720 >= D_8007B92D) {
                    func_8004BF20();
                }
                a0->unk3A = *D_8007B720++ % u->unk9C->unkC->unk2;
            }
        }
    }
    return arg1;
}

void func_800BBD3C(BtlUnit *arg0, Unk8E0 *arg1) {
    s32 i;
    u32 f;
    func_800B10F8(arg0);
    func_800C09EC(arg0);
    arg0->unk7B = 0;
    arg0->unkB4 = NULL;
    arg0->unkCF = 0;
    arg0->unk7E = 0;
    arg0->unk80 = 0;
    arg0->unk82 = 0;
    arg0->unkCE = 0;
    arg0->unkC8 = 0;
    arg0->unkCA = 0;
    arg0->unkCC = 0;
    arg0->unk7D = 0;
    f = arg0->unk68;
    f &= ~0x20000;
    f &= ~0x80000;
    f &= ~0x100000;
    f &= ~0x200000;
    f &= ~0x400000;
    f |= 0x1000000;
    f &= ~0x2000000;
    f &= ~0x4000000;
    f &= ~0x8000000;
    f &= ~0x20000000;
    arg0->unk68 = f;
    arg1->unk10 = arg0;
    if ((arg0->unk68 >> 23) & 1) {
        arg0->unk9C = D_8006794C->unk1C->unk8E0->unk8;
        arg0->unkA0 = D_8006794C->unk1C->unk8E0->unkC;
    } else {
        arg0->unk9C = arg1->unk8;
        arg0->unkA0 = arg1->unkC;
        arg0->unk9C->unk50[2].unk4 |= 1;
        arg0->unk9C->unk50[2].unkC = 0;
        arg0->unk9C->unk50[2].unk12 = 0x1000;
        for (i = 0; i < arg0->unk9C->unk8->unk3; i++) {
            func_800C8D18(arg0->unk9C, i, 0, 0, 0);
        }
    }
    arg0->unkAC = 0;
}

void func_800BBF30(Unk8004AB64 *arg0, s16 arg1) {
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
    switch (arg0->unk34.b.unk4) {
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

void func_800BC0D0(Character *c, BtlUnit *u) {
    s32 i;

    u->unk79 = 0;
    u->unk73 = 0;
    u->unk72 = 0;
    u->unk71 = 0;
    u->unk70 = 0;
    for (i = 0; i < 5; i++) {
        if (c->unk39[i] != 0xFF) {
            func_800BDACC(&u->unk70, D_8006D548[D_80069018[c->unk39[i]].unkB].unk4);
            u->unk79 |= D_8006D548[D_80069018[c->unk39[i]].unkB].unk8;
        }
    }
}

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

void func_800BC21C(BtlUnit *u, s32 *ot) {
    BtlUnit9C *m = u->unk9C;
    BtlUnitA0 *a = u->unkA0;
    BtlUnit *v;
    SVECTOR tmp;
    SVECTOR rot;
    s32 i;
    s32 n;
    s32 t;
    BtlUnit9CC *p;
    s32 id;
    BtlUnit9C *w;

    if ((u->unk68 >> 23) & 1) {
        v = func_800C1DEC();
        u->unk86[0] = v->unk86[0];
        u->unk86[1] = v->unk86[1];
        u->unk86[2] = v->unk86[2];
        u->unk98->unk0[0] = u->unk9C->unk20[u->unk7C].unk14 << 16;
        u->unk98->unk0[1] = 0;
        u->unk98->unk0[2] = u->unk9C->unk20[u->unk7C].unk1C << 16;
        u->unk8C[0] = u->unk98->unk0[0];
        u->unk8C[1] = u->unk98->unk0[1];
        u->unk8C[2] = u->unk98->unk0[2];
        return;
    }
    if ((u->unk68 & 1) && (s32)m->unkC == u->unkA4[17]) {
        func_8001D898(&rot, 0, 8);
        rot.vx = u->unk86[0];
        rot.vy = 0x800;
        rot.vz = u->unk86[2];
        tmp = rot;
        func_80031244(&tmp, &m->unk24);
    } else {
        func_8001D898(&rot, 0, 8);
        rot.vx = u->unk86[0];
        rot.vy = u->unk86[1];
        rot.vz = u->unk86[2];
        tmp = rot;
        func_80031244(&tmp, &m->unk24);
    }
    m->unk24.t[0] = u->unk8C[0] >> 16;
    m->unk24.t[1] = u->unk8C[1] >> 16;
    m->unk24.t[2] = u->unk8C[2] >> 16;
    m->unk10 = 0;
    if (a->unk3A >= m->unkC->unk2) {
        if (!(u8)func_800BC78C(u)) {
            u->unkA0->unk3A = 0;
        } else if (u->unk3C & 0x2001003) {
            u->unkA0->unk3A--;
        }
    }
    func_800C7B84(m);
    p = m->unkC;
    if (p->unkC >= 0) {
        t = p->unk10;
        p->unkC = (s32)p + p->unkC;
        if (t != 0) {
            p->unk10 = (s32)m->unkC + t;
        }
    }
    *ot = (s32)func_800C7DAC(m, a->unk3A, (u8 *)*ot);
    if (!(u->unk3C & 0x2001003) && !((u->unk68 >> 21) & 1)) {
        a->unk3A++;
    }
    n = m->unk8->unk3;
    func_800C7CC8(m);
    u->unk9C->unk0 &= ~0x20;
    for (i = 0; i < n; i++) {
        if (!(m->unk14 & (1 << i))) {
            func_800C6528(m, i);
            *ot = (s32)func_800C58E4(m, i, (u8 *)*ot);
            u->unk9C->unk0 |= 0x20;
        }
    }
    if (!(u->unk3C & 0x40000000)) {
        func_800C9040((TexAnimSet *)m->unk50);
    }
    if (u->unkA8 != NULL) {
        u->unkA8->unk10 = 0;
        *ot = (s32)func_800C7DAC(u->unkA8, 0, (u8 *)*ot);
        w = u->unkA8;
        n = w->unk8->unk3;
        func_800C7CC8(w);
        u->unkA8->unk0 &= ~0x20;
        for (i = 0; i < n; i++) {
            if (!(u->unkA8->unk14 & (1 << i))) {
                func_800C6528(u->unkA8, i);
                *ot = (s32)func_800C58E4(u->unkA8, i, (u8 *)*ot);
                u->unkA8->unk0 |= 0x20;
            }
        }
    }
    if ((u->unk68 >> 24) & 1) {
        id = (u->unkBE << 8) | u->unkBD;
        if (u->unk68 & 1) {
            if (!(D_8006794C->unk1C->unk8E4.battle->unk11B8 & 1)) {
                if (u->unk7B != D_8006794C->unk1C->unk8E4.battle->unkD83) {
                    func_800BC78C(u);
                }
                u->unk7B = D_8006794C->unk1C->unk8E4.battle->unkD83;
            }
            *ot = (s32)func_800AC37C(D_8006794C->unk1C->unk8E4.battle->unk11C0->unk28, (u->unk68 >> 1) & 0xF, 0, id, (u8 *)*ot);
        } else if (u->unk7D < 4) {
            *ot = (s32)func_800AC37C(D_8006794C->unk1C->unk8E4.battle->unk11C0->unk28, ((u->unk68 >> 1) & 0xF) + 9, 0, id, (u8 *)*ot);
        }
    }
}

s32 func_800BC78C(BtlUnit *u) {
    GameData *g = D_8006794C->unk1C;
    Battle *b = g->unk8E4.battle;
    BtlCmd *cmd = b->unk1088;
    s32 cur;
    s32 *a;
    BtlUnit9C *m;

    if (u->unk3C & 0x100) {
        if ((u->unk68 & 0x20001) == 1 && u != cmd->unk4 && u->unk7D == 0) {
            cur = (s32)u->unk9C->unkC;
            if (cur != u->unkA4[4] && cur != u->unkA4[9]) {
                u->unk9C->unkC = (BtlUnit9CC *)u->unkA4[(u->unk68 >> 18) & 1];
            }
        }
        return 0;
    }
    if ((u->unk3C & 0x42001003) || ((u->unk68 >> 17) & 1)
        || ((u->unk68 & 1) && g->unk3C[(u->unk68 >> 1) & 0xF].unk34.b.unk4 == 2 && cmd->unkE == 0x1F)) {
        return 0;
    }
    {
        if (u == cmd->unk4 && (u32)(cmd->unkE - 0x37) >= 7) {
            cur = (s32)u->unk9C->unkC;
            if (cur == u->unkA4[10] || cur == u->unkA4[11]) {
                u->unk9C->unkC = (BtlUnit9CC *)u->unkA4[9];
            }
            return 0;
        }
        if (u->unk68 & 1) {
            if (!(u->unk68 & 0x06000000)) {
                if ((b->unkD83 != 0 || (b->unk11B8 & 1)) && !(u->unk3C & 0x42021143)) {
                    u->unk9C->unkC = (BtlUnit9CC *)u->unkA4[17];
                } else if (u->unk3C & 0x8000) {
                    u->unk9C->unkC = (BtlUnit9CC *)u->unkA4[13];
                } else if ((u->unk68 >> 19) & 1) {
                    if ((s32)u->unk9C->unkC == u->unkA4[(u->unk68 >> 18) & 1]) {
                        u->unk9C->unkC = (BtlUnit9CC *)u->unkA4[((u->unk68 >> 18) & 1) + 10];
                    } else {
                        u->unk9C->unkC = (BtlUnit9CC *)u->unkA4[9];
                    }
                } else {
                    m = u->unk9C;
                    a = u->unkA4;
                    cur = (s32)m->unkC;
                    if (cur == a[0] && (u->unk3C & 0x30202)) {
                        m->unkC = (BtlUnit9CC *)a[7];
                    } else if ((cur == a[1] || cur == a[6]) && !(u->unk3C & 0x30202)) {
                        m->unkC = (BtlUnit9CC *)a[5];
                    } else {
                        u->unk9C->unkC = (BtlUnit9CC *)u->unkA4[(u->unk68 >> 18) & 1];
                    }
                }
            }
        } else {
            u->unk9C->unkC = (BtlUnit9CC *)u->unkA4[(u->unk68 >> 18) & 1];
        }
        u->unkA0->unk3A = 0;
        return 1;
    }
}

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

void func_800BCDBC(BtlUnit *arg0) {
    BtlUnit9C *m;
    s32 *a;
    arg0->unk68 &= ~0x20000;
    if (arg0->unk3C & 0x100) {
        m = arg0->unk9C;
        a = arg0->unkA4;
        if ((s32)m->unkC == a[3] && ((u8)(D_8006794C->unk1C->unk8E4.battle->unk488 + ((arg0->unk68 >> 1) & 0xF))->unk4 >> 7)) {
            m->unkC = (BtlUnit9CC *)a[4];
            arg0->unkA0->unk3A--;
            arg0->unk7D = 3;
            arg0->unk68 |= 0x200000;
            return;
        }
        if (!(((D_8006794C->unk1C->unk8E4.battle->unk488 + ((arg0->unk68 >> 1) & 0xF))->unk4 >> 6) & 1) && !((arg0->unk68 >> 20) & 1)) {
            arg0->unk7D = 1;
            return;
        }
        arg0->unk9C->unkC = (BtlUnit9CC *)arg0->unkA4[(arg0->unk68 >> 18) & 1];
    }
}

void func_800BCEF0(BtlUnit *u) {
    Battle488 *e;
    BtlUnit9C *m;
    u32 f;

    if (u->unk7D == 0) {
        return;
    }
    if (!(u->unk68 & 1)) {
        e = &D_8006794C->unk1C->unk8E4.battle->unk488[(u->unk68 >> 1) & 0xF];
        switch (u->unk7D) {
        case 1:
            if (!((u->unk68 >> 23) & 1)) {
                u->unk9C->unkC = (BtlUnit9CC *)u->unkA4[4 + ((u->unk68 >> 18) & 1)];
                u->unkA0->unk3A = 0;
                u->unk7D++;
            } else {
                u->unk7D = 5;
            }
            func_800C2250(u, e->unk0->unk30);
            return;
        case 2:
            if (u->unkA0->unk3A >= u->unk9C->unkC->unk2 - 1) {
                u->unk68 |= 0x200000;
                u->unk7D++;
            }
            return;
        case 3:
            func_800C22CC(u, 0x79, 0x7F);
            func_800C22CC(u, 0x78, 0x7F);
            u->unk7D++;
        case 4:
            func_800C24E8(u, e->unk4 & 0x3F);
            if ((e->unk4 & 0x3F) == 0) {
                u->unk7D = 5;
                return;
            }
            e->unk4 = (e->unk4 & ~0x3F) | (((e->unk4 & 0x3F) - 2) & 0x3F);
            return;
        case 5:
            if (func_800C1E88(u, 8) != 0) {
                if (D_8006794C->unk1C->unk56C < 9999) {
                    D_8006794C->unk1C->unk56C++;
                }
            }
            if (D_8006794C->unk1C->unk56E != 4) {
                func_800C12CC(e->unk0);
            }
            u->unk7D++;
            if (D_8006794C->unk1C->unk8E4.battle->unkD80 != 5 ||
                (u32)(D_8006794C->unk1C->unk8E4.battle->unkD81 - 2) >= 2) {
                func_800C0EC8(u);
            }
            func_800C1938(u);
            return;
        }
    } else {
        /* padded: the first jump table is 8-aligned in its object */
        __asm__(".section .rodata\n\t.word 0\n\t.section .text");
        switch (u->unk7D) {
        case 1:
            m = u->unk9C;
            if ((s32)m->unkC == u->unkA4[9]) {
                m->unkC = (BtlUnit9CC *)u->unkA4[32];
                u->unkA0->unk3A = 0;
                return;
            }
            if ((s32)m->unkC == u->unkA4[32] && u->unkA0->unk3A < m->unkC->unk2) {
                break;
            }
            u->unk7D = 2;
        case 2:
            if ((f = (u->unk68 >> 18) & 1) == 1) {
                u->unk9C->unkC = (BtlUnit9CC *)u->unkA4[8];
                u->unk7D = 4;
            } else {
                u->unk9C->unkC = (BtlUnit9CC *)u->unkA4[7];
                u->unk7D = 3;
            }
            u->unkA0->unk3A = 0;
            return;
        case 3:
            if (u->unkA0->unk3A >= u->unk9C->unkC->unk2) {
                u->unk9C->unkC = (BtlUnit9CC *)u->unkA4[8];
                u->unkA0->unk3A = 0;
                u->unk7D++;
            }
            break;
        case 4:
            if (u->unkA0->unk3A < u->unk9C->unkC->unk2) {
                break;
            }
        case 5:
            if ((u8)func_800BD5A0(u)) {
                func_800C0EC8(u);
            }
            break;
        }
    }
}

s32 func_800BD31C(BtlCmd *arg0) {
    BtlUnit *u;
    s32 r;
    u32 bit = arg0->unk10.b.unk0_5;
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

u32 func_800BDED4(BtlUnit *arg0, s32 arg1) {
    u32 r = 0xFFFFFF;

    switch (arg1) {
    case 0x23:
        r = arg0->unk24->unk0;
        break;
    case 0x24:
        r = arg0->unk28;
        break;
    case 0x25:
        r = arg0->unk24->unk2;
        break;
    case 0x26:
        r = arg0->unk2A;
        break;
    case 0x27:
        r = arg0->unk24->unk4;
        break;
    case 0x29:
        if (arg0->unk68 & 1) {
            r = (D_8006794C->unk1C->unk3C + ((arg0->unk68 >> 1) & 0xF))->unkB;
        } else {
            r = D_8006794C->unk1C->unk8E4.battle->unk488[(arg0->unk68 >> 1) & 0xF].unk0->unk3;
        }
        break;
    case 0x2A:
        r = ((u8 *)&arg0->unk34)[3];
        break;
    case 0x2B:
        r = arg0->unk34 & 0xFFFFFF;
        break;
    case 0x2C:
        r = ((u8 *)&arg0->unk38)[3];
        break;
    case 0x2D:
        r = arg0->unk38 & 0xFFFFFF;
        break;
    case 0x2E:
        r = ((u8 *)&arg0->unk3C)[3];
        break;
    case 0x2F:
        r = arg0->unk3C & 0xFFFFFF;
        break;
    case 0x30:
        r = arg0->unk70;
        break;
    case 0x31:
        r = arg0->unk71;
        break;
    case 0x32:
        r = arg0->unk72;
        break;
    case 0x33:
        r = arg0->unk73;
        break;
    case 0x34:
        r = arg0->unk68 >> 28;
        r &= 1;
        break;
    case 0x35:
        r = arg0->unk68 >> 22;
        r &= 1;
        break;
    case 0x39:
        r = (s16)arg0->unk84;
        break;
    case 0x3A:
        r = arg0->unkAC;
        break;
    case 0x40:
        r = arg0->unk68 >> 8;
        r &= 3;
        break;
    case 0x41:
        r = (u8)arg0->unk68;
        r >>= 5;
        break;
    case 0x42:
        r = (D_8006794C->unk1C->unk3C + ((arg0->unk68 >> 1) & 0xF))->unk34.w >> 4;
        r &= 0x1F;
        break;
    case 0x43:
        r = (D_8006794C->unk1C->unk3C + ((arg0->unk68 >> 1) & 0xF))->unkA;
        break;
    case 0x44:
        r = D_8006794C->unk1C->unk8E4.battle->unk488[(arg0->unk68 >> 1) & 0xF].unk0->unk2;
        break;
    case 0x45:
        r = arg0->unk68 >> 18;
        r &= 1;
        break;
    case 0x46:
        r = arg0->unk68 >> 1;
        r &= 0xF;
        break;
    case 0x48:
        r = arg0->unk31;
        break;
    case 0x49:
        r = arg0->unk32;
        break;
    case 0x4A:
        r = arg0->unk6C;
        break;
    case 0x4B:
        r = arg0->unk6D;
        break;
    case 0x4C:
        r = arg0->unk6E;
        break;
    case 0x4D:
        r = arg0->unk6F;
        break;
    }
    return r;
}

/* padded: this jump table is 8-aligned in its object */
__asm__(".section .rodata\n\t.word 0\n\t.section .text");

void func_800BE1E0(BtlUnit *arg0, s32 arg1, u32 arg2) {
    s32 k;
    s32 off;
    s32 w;
    s32 x;

    switch (arg1) {
    case 0x20:
        func_800C1990(arg0);
        break;
    case 0x21:
        func_800B4040(arg0);
        func_800C1938(arg0);
        break;
    case 0x22:
        if ((u8)func_800B4040(arg0)) {
            arg0->unkB3 = 0;
        }
        break;
    case 0x23:
        arg0->unk24->unk0 = arg2;
        break;
    case 0x24:
        arg0->unk28 = arg2;
        break;
    case 0x25:
        arg0->unk24->unk2 = arg2;
        break;
    case 0x26:
        arg0->unk2A = arg2;
        break;
    case 0x28:
        arg0->unk2C = arg2;
        break;
    case 0x30:
        arg0->unk70 = arg2;
        break;
    case 0x31:
        arg0->unk71 = arg2;
        break;
    case 0x32:
        arg0->unk72 = arg2;
        break;
    case 0x33:
        arg0->unk73 = arg2;
        break;
    case 0x34:
        if (D_8006794C->unk1C->unk8E4.battle->unkD80 == 2) {
            arg0->unkB2 = 0;
            arg0->unk68 &= ~0x10000000;
            arg0->unk68 &= ~0x10000;
            arg0->unk68 &= ~0x20000000;
        } else {
            arg0->unkB2 = arg2 + 2;
        }
        break;
    case 0x35:
        arg0->unk68 = (arg0->unk68 & ~0x400000) | ((arg2 & 1) << 22);
        if (arg2 != 0 && D_8006794C->unk1C->unk8E4.battle->unkD80 == 2) {
            func_800C2344(arg0->unk9C);
            k = func_800C1EDC(arg0);
            D_8006794C->unk1C->unk8E4.battle->unk11C0->unk28[k].unk0_0 = func_800CEE74()->unkF;
        }
        break;
    case 0x36:
        arg0->unk68 = (arg0->unk68 & ~0x1000000) | ((arg2 & 1) << 24);
        break;
    case 0x37:
        arg0->unk9C->unk0 |= 2;
        arg0->unk9C->unk54 = arg2;
        arg0->unk9C->unk56 = arg2;
        arg0->unk9C->unk58 = arg2;
        k = func_800C1EDC(arg0);
        D_8006794C->unk1C->unk8E4.battle->unk11C0->unk28[k].unk0_8 = ((u32)(arg0->unkBF * arg2) >> 12) * 14;
        k = func_800C1EDC(arg0);
        D_8006794C->unk1C->unk8E4.battle->unk11C0->unk28[k].unk0_20 = ((u32)(arg0->unkC0 * arg2) >> 12) * 12;
        break;
    case 0x38:
        arg0->unk9C->unk0 &= ~2;
        k = func_800C1EDC(arg0);
        D_8006794C->unk1C->unk8E4.battle->unk11C0->unk28[k].unk0_8 = arg0->unkBF * 14;
        k = func_800C1EDC(arg0);
        D_8006794C->unk1C->unk8E4.battle->unk11C0->unk28[k].unk0_20 = arg0->unkC0 * 12;
        break;
    case 0x3B:
        func_800BD414(arg0, arg2);
        arg0->unkAC |= arg2;
        break;
    case 0x3C:
        func_800BD464(arg0, arg2);
        arg0->unkAC &= ~arg2;
        break;
    case 0x3D:
        off = arg2 * 20;
        ((BtlUnit9C50 *)((u8 *)arg0->unk9C->unk50 + off))->unk4 |= 1;
        ((BtlUnit9C50 *)((u8 *)arg0->unk9C->unk50 + off))->unkC = 0;
        ((BtlUnit9C50 *)((u8 *)arg0->unk9C->unk50 + off))->unk12 = 0x1000;
        break;
    case 0x3E:
        w = arg2 * 20;
        ((BtlUnit9C50 *)((u8 *)arg0->unk9C->unk50 + w))->unk4 &= ~3;
        break;
    case 0x3F:
        x = arg2 * 20;
        ((BtlUnit9C50 *)((u8 *)arg0->unk9C->unk50 + x))->unk4 |= 3;
        ((BtlUnit9C50 *)((u8 *)arg0->unk9C->unk50 + x))->unkC = 0;
        ((BtlUnit9C50 *)((u8 *)arg0->unk9C->unk50 + x))->unk12 = 0x1000;
        break;
    case 0x45:
        arg0->unk68 = (arg0->unk68 & ~0x40000) | ((arg2 & 1) << 18);
        break;
    case 0x47:
        func_800C12CC(D_8006794C->unk1C->unk8E4.battle->unk488[(arg0->unk68 >> 1) & 0xF].unk0);
        break;
    case 0x48:
        arg0->unk31 = arg2;
        break;
    case 0x49:
        arg0->unk32 = arg2;
        break;
    case 0x4A:
        arg0->unk6C = arg2;
        break;
    case 0x4B:
        arg0->unk6D = arg2;
        break;
    case 0x4C:
        arg0->unk6E = arg2;
        break;
    case 0x4D:
        arg0->unk6F = arg2;
        break;
    case 0x4E:
        arg0->unk2C = arg0->unk24->unk4;
        arg0->unkB3 = 1;
        func_800B1250(arg0->unk4, 0x14, 0xBB, (u16)arg2, 8);
        func_800E9F3C(1);
        func_800E9F68(1);
        break;
    }
}

/* padded: this jump table is 8-aligned in its object */
__asm__(".section .rodata\n\t.word 0\n\t.section .text");

s32 func_800BE784(s32 arg0) {
    Battle *b = D_8006794C->unk1C->unk8E4.battle;
    s32 r = -1;
    BtlUnit *u;
    s32 k;
    switch (arg0) {
    case 32:
        r = b->unk132C.unk0 & 3;
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
                k = D_8006794C->unk1C->unk3C[(u->unk68 >> 1) & 0xF].unk34.b.unk4;
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

/* padded: this jump table is 8-aligned in its object */
__asm__(".section .rodata\n\t.word 0\n\t.section .text");

void func_800BE99C(s32 arg0, s32 arg1) {
    Battle *b = D_8006794C->unk1C->unk8E4.battle;
    BtlUnit *u;
    u32 f;

    switch (arg0) {
    case 32:
        func_800E9F68(0);
        b->unkD80 = 5;
        b->unkD83 = 0;
        b->unkD81 = 2;
        b->unk11B8 &= ~1;
        func_800B2F24(b);
        for (u = b->unk6F8; u != NULL; u = u->unk0) {
            u->unk68 &= ~0x80000;
        }
        break;
    case 33:
        if (b->unkD80 == 1) {
            D_8006794C->unk1C->unk56E = arg1;
            if (arg1 == 1 && ((b->unk132C.unk0 >> 6) & 1)) {
                b->unkD80 = 5;
                b->unkD81 = 4;
            } else {
                if (D_8006794C->unk1C->unk56E == 1) {
                    D_8006794C->unk1C->unk56E = 2;
                }
                b->unkD80 = 8;
                b->unkD81 = 0;
            }
        }
        break;
    case 34:
        if (b->unkD80 == 1) {
            b->unkD80 = 7;
            b->unkD81 = 0;
            D_8006794C->unk1C->unk56E = 3;
        }
        break;
    case 35:
        if (b->unkD80 == 1) {
            f = b->unk132C.unk0;
            b->unkD80 = 3;
            b->unkD81 = 0;
            if ((f & 0xC) != 0xC) {
                b->unk132C.unk0 = (f & ~3) | 2;
            }
        }
        break;
    case 36:
        if (b->unkD80 == 1) {
            func_800C2F1C(func_800CA2B0(arg1));
        }
        break;
    case 37:
        D_8006794C->unk1C->unk56F |= 1;
        func_80021640(arg1);
        break;
    case 38:
        D_8006794C->unk1C->unk560 += arg1;
        if (D_8006794C->unk1C->unk560 > 9999999) {
            D_8006794C->unk1C->unk560 = 9999999;
        }
        break;
    case 39:
        if (D_8006794C->unk1C->unk56C < 9999) {
            D_8006794C->unk1C->unk56C++;
        }
        break;
    case 40:
        func_800AD90C(0x8601, 0, 0x1E, 0, 0);
        D_8006794C->unk1C->unk56F |= 0x10;
        break;
    }
}

u16 func_800BEC64(u32 arg0) {
    BtlUnit *u;
    s32 r;

    r = 0;
    for (u = D_8006794C->unk1C->unk8E4.battle->unk6F8; u != NULL; u = u->unk0) {
        switch (arg0) {
        case 0:
            if (u->unk68 & 1) {
                r |= u->unkB0;
            }
            break;
        case 1:
            if (!(u->unk68 & 1)) {
                r |= u->unkB0;
            }
            break;
        case 2:
            r |= u->unkB0;
            break;
        }
    }
    return r;
}

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

u32 func_800BEDBC(BtlUnit *u, u32 f) {
    u32 n = 0;
    Battle154C *tbl = D_8006794C->unk1C->unk8E4.battle->unk154C;
    BtlUnit34 *s = (BtlUnit34 *)&u->unk34;
    u32 i;
    u32 blk;
    s16 k;

    if (u->unk34 & f) {
        return 0;
    }
    if (s->unk4 & f) {
        return 1;
    }
    if ((s->unk8 & f) && (f & 0x8800FF7F)) {
        return 1;
    }
    i = 0;
    blk = 0;
    for (; i < 32; i++) {
        if (f & (1 << i)) {
            n = i;
        }
        if (s->unk8 & (1 << i)) {
            blk |= tbl[i].unk8;
        }
    }
    if (func_800B3EF8(u, 0x3E)) {
        blk |= tbl[0].unk8;
    }
    if (blk & f) {
        return 1;
    }
    if (!(f & 0x10180001)) {
        if ((f & 0x20D03) && (u8)func_800B4040(u)) {
            u->unk2C = 0;
            u->unk68 &= ~0x10000;
            if (!(f & 0x100)) {
                u->unkB3 = 0;
            }
        }
        s->unk8 |= f;
    }
    switch (n) {
    case 0:
        if (!func_800B3E7C(u->unkC)) {
            if (D_8006794C->unk1C->unk8E4.battle->unkD80 >= 3) {
                func_800B1250(u->unkC, 0x3E, 0, u->unkB0, 0);
            } else {
                if (u->unk68 & 1) {
                    func_800C0544(u);
                }
                s->unk8 |= f;
                u->unk68 &= ~0x10000;
                func_800C0430(u, 1);
            }
        }
        break;
    case 1:
        if (D_8006794C->unk1C->unk8E4.battle->unkD80 >= 3) {
            func_800C0EC8(u);
        }
    case 17:
        if ((u->unk68 & 1) && (s32)u->unk9C->unkC != u->unkA4[1] &&
            D_8006794C->unk1C->unk8E4.battle->unk1088->unk4 != u) {
            u->unk9C->unkC = (BtlUnit9CC *)u->unkA4[1];
            u->unkA0->unk3A = 0;
        }
        break;
    case 6:
        if ((u->unk68 & 1) && !(u->unk3C & 0x4000)) {
            u->unk78 = 0;
        }
    case 11:
    case 24:
    case 25:
        func_800C02D4(u);
        break;
    case 8:
        if (u->unk28 != 0) {
            u->unk7E |= 0x40;
            func_800B97A0(u);
        } else {
            u->unk28 = 0;
        }
        u->unk2C = 0;
        if (!func_800B3E7C(u->unkC)) {
            func_800B1250(u->unkC, 0x3C, 0, u->unkB0, 0);
            if (!(u->unk68 & 1)) {
                func_800C1704(u);
            }
        }
        break;
    case 12:
        func_800C0EC8(u);
        break;
    case 14:
        func_800B1250(u->unk14, 0x3B, 0, u->unkB0, 0);
        break;
    case 19:
        if (u->unk3C & 0x100000) {
            func_800BF558(u, 0x100000);
            return 2;
        }
        func_800BD8F0((Unk800BD8F0 *)u);
        u->unk2E = u->unk2E * 3 / 2;
        s->unk8 |= f;
        break;
    case 20:
        if (u->unk3C & 0x80000) {
            func_800BF558(u, 0x80000);
            return 2;
        }
        func_800BD8F0((Unk800BD8F0 *)u);
        u->unk2E = u->unk2E * 2 / 3;
        s->unk8 |= f;
        break;
    case 26:
        func_800BD414(u, u->unkAE);
        break;
    case 28:
        s->unk8 ^= f;
        if (u->unk3C & 0x10000000) {
            u->unk9C->unk0 |= 2;
            u->unk9C->unk54 = 0x800;
            u->unk9C->unk56 = 0x800;
            u->unk9C->unk58 = 0x800;
            D_8006794C->unk1C->unk8E4.battle->unk11C0->unk28[func_800C1EDC(u)].unk0_8 = (u->unkBF >> 1) * 14;
            D_8006794C->unk1C->unk8E4.battle->unk11C0->unk28[func_800C1EDC(u)].unk0_20 = (u->unkC0 >> 1) * 12;
        } else {
            u->unk9C->unk0 &= ~2;
            D_8006794C->unk1C->unk8E4.battle->unk11C0->unk28[func_800C1EDC(u)].unk0_8 = u->unkBF * 14;
            D_8006794C->unk1C->unk8E4.battle->unk11C0->unk28[func_800C1EDC(u)].unk0_20 = u->unkC0 * 12;
        }
        break;
    }
    func_800C0B6C(u, tbl[n].unk4);
    if (u->unk3C & 0x40001101) {
        u->unk68 &= ~0x10000;
    }
    if (f & 0xEFFF0000) {
        if (f & 0x9B130000) {
            k = (0x3C - u->unk33) * 8;
        } else if (f & 0x24EC0000) {
            k = u->unk33 * 8;
        } else {
            k = (0x3C - u->unk33) * 4;
        }
        u->unk46[n - 16] = tbl[n].unk2 * k;
    }
    if (f & 0x50002) {
        func_800C0C00(u, n);
    }
    return 2;
}

s32 func_800BF558(BtlUnit *u, u32 f) {
    BtlUnit34 *s = (BtlUnit34 *)&u->unk34;
    BtlUnit9C *m;
    s32 *a4;
    BtlCmd *c;

    if (s->unk4 & f) {
        return 1;
    }
    if (!(s->unk8 & f)) {
        return 1;
    }
    if (!(u->unk68 & 1) && D_8006794C->unk1C->unk8E4.battle->unkD80 == 5 && (f & 0x1003)) {
        return 1;
    }
    s->unk8 &= ~f;
    switch (f) {
    case 0x1:
        func_800C0430(u, 0);
        break;
    case 0x800:
        if ((u8)func_800B4040(u)) {
            u->unk2C = 0;
            u->unkB3 = 0;
            u->unk68 &= ~0x10000;
        }
    case 0x40:
    case 0x1000000:
    case 0x2000000:
        if (u->unk3C & 0x3000840) {
            func_800C02D4(u);
        }
        break;
    case 0x100:
        m = u->unk9C;
        a4 = u->unkA4;
        u->unk7D = 0;
        u->unk7B = 0;
        u->unk68 &= ~0x20000;
        u->unk68 &= ~0x80000;
        u->unk68 &= ~0x100000;
        u->unk68 &= ~0x200000;
        if ((s32)m->unkC == a4[4] || (s32)m->unkC == a4[8]) {
            m->unk50[2].unk4 |= 1;
            u->unk9C->unk50[2].unkC = 0;
            u->unk9C->unk50[2].unk12 = 0x1000;
            u->unk9C->unkC = (BtlUnit9CC *)u->unkA4[6];
            u->unkA0->unk3A = 0;
        }
        c = D_8006794C->unk1C->unk8E4.battle->unk1088;
        if (c->unk4 != u || c->unkE > 0x30) {
            u->unkB3 = 0;
        }
        func_800B41A4(u, 0x3C);
        if (u->unk38 & 0x40000) {
            func_800C0C00(u, 0x12);
        }
        break;
    case 0x400:
        u->unk86[1] = u->unkA0->unk22;
        if ((u8)func_800B4040(u)) {
            u->unk2C = 0;
            u->unkB3 = 0;
            u->unk68 &= ~0x10000;
        }
        break;
    case 0x4000:
        u->unk78 = 0;
        func_800B3DBC(u);
        if (u->unk3C & 0x40000000) {
            func_800BF558(u, 0x40000000);
            u->unk68 &= ~0x400000;
            u->unk8C[0] = u->unk98->unk0[0];
            u->unk8C[1] = u->unk98->unk0[1];
            u->unk8C[2] = u->unk98->unk0[2];
            u->unk9C->unkC = (BtlUnit9CC *)u->unkA4[(u->unk68 >> 18) & 1];
            u->unkA0->unk3A = 0;
        }
        func_800B1250(u->unk14, 0x3B, 0, u->unkB0, 0);
        break;
    case 0x80000:
    case 0x100000:
        func_800BD8F0((Unk800BD8F0 *)u);
        break;
    case 0x200000:
        u->unk8C[1] = 0;
        u->unk98->unk0[1] = 0;
        break;
    case 0x4000000:
        func_800BD464(u, u->unkAE);
        break;
    case 0x10000000:
        u->unk9C->unk0 &= ~2;
        D_8006794C->unk1C->unk8E4.battle->unk11C0->unk28[func_800C1EDC(u)].unk0_8 = u->unkBF * 14;
        D_8006794C->unk1C->unk8E4.battle->unk11C0->unk28[func_800C1EDC(u)].unk0_20 = u->unkC0 * 12;
        break;
    case 0x40000000:
        u->unkB2 = 3;
        u->unk2C = 0;
        u->unkB3 = 0;
        u->unk68 |= 0x10000;
        u->unk10->unkE = 0;
        u->unk10->unkC = 0;
        break;
    case 0x80000000:
        func_800B41A4(u, 0x3E);
        break;
    }
    return 2;
}

void func_800BFAF4(BtlUnit *arg0) {
    BtlUnit34 *t = (BtlUnit34 *)&arg0->unk34;
    BtlCmd *c;
    Battle *b;
    u16 fl;

    func_800BFF20(arg0);
    if (arg0->unk3C & 0x100) {
        func_800BCEF0(arg0);
    } else if (!(arg0->unk3C & 1)) {
        if (!(arg0->unk3C & 0x40001101)) {
            arg0->unk68 |= 0x10000;
        }
        if (func_800EA038()) {
            if ((arg0->unk68 >> 16) & 1) {
                if (arg0->unk3C & 2) {
                    if (t->unkC <= 0) {
                        func_800C0C00(arg0, 1);
                        func_800BDD24(arg0);
                        func_800BDE78(arg0);
                        func_800CC1A0(arg0);
                    } else {
                        t->unkC -= arg0->unk2E;
                    }
                }
                if (arg0->unk3C & 0x10000) {
                    if (t->unkE <= 0) {
                        func_800C0C00(arg0, 0x10);
                        func_800BDD24(arg0);
                        func_800CC1A0(arg0);
                    } else {
                        t->unkE -= arg0->unk2E;
                    }
                }
                if (BtlUnitHasStatus(arg0, 0x40000)) {
                    if (t->unk10 <= 0) {
                        func_800C0C00(arg0, 0x12);
                        func_800BDDA0(arg0);
                        func_800CC1A0(arg0);
                    } else {
                        t->unk10 -= arg0->unk2E;
                    }
                }
                if ((arg0->unk3C & 0x4000) && ((arg0->unk68 >> 1) & 0xF) == 2) {
                    b = D_8006794C->unk1C->unk8E4.battle;
                    fl = b->unk11B8;
                    if (fl & 4) {
                        if (!(fl & 8)) {
                            if (b->unk11BC <= 0) {
                                func_800B1250(arg0->unk10, 0x39, D_8006794C->unk1C->unk8E4.battle->unk11BB, func_800C1CA0(1, 0), 8);
                                D_8006794C->unk1C->unk8E4.battle->unk11B8 |= 8;
                            } else {
                                b->unk11BC -= arg0->unk2E;
                            }
                        }
                    }
                }
                func_800C0DAC(arg0);
            } else if ((arg0->unk3C & 0x40000000) && !(D_8006794C->unk1C->unk8E4.battle->unk11B8 & 0x10)) {
                t->unk2E -= arg0->unk2E;
                if (t->unk2E < 0) {
                    c = arg0->unk10;
                    if (c->unkE == 3) {
                        func_800B3D54(arg0, 0xA, 0xB9, (u16)c->unkC);
                    } else {
                        func_800B3D54(arg0, 0xB, 0xBA, (u16)c->unkC);
                    }
                }
            }
        }
    }
    if ((arg0->unk3C & 0x400) && arg0 != D_8006794C->unk1C->unk8E4.battle->unk1088->unk4) {
        if (arg0->unk9C->unkC == (BtlUnit9CC *)arg0->unkA4[0] || arg0->unk9C->unkC == (BtlUnit9CC *)arg0->unkA4[1] ||
            ((arg0->unk68 & 1) && arg0->unk9C->unkC == (BtlUnit9CC *)arg0->unkA4[9])) {
            arg0->unk86[1] += 0x80;
        }
    }
}

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

void func_800C0544(BtlUnit *arg0) {
    s16 i;
    s16 j;
    u8 c[3];
    u8 g;

    for (i = 0; i < func_80063830(arg0->unk84 | 0x20000); i++) {
        D_800FDF50.x = 0x100;
        D_800FDF50.y = func_8001DA6C(arg0->unk9C->unk18[i].unk2);
        D_800FDF50.w = 0x100;
        D_800FDF50.h = 1;
        StoreImage(&D_800FDF50, (u32 *)D_800FDF60[i]);
        DrawSync(0);
        for (j = 0; j < 0x100; j++) {
            c[0] = D_800FDF60[i][j] & 0x1F;
            c[1] = (D_800FDF60[i][j] >> 5) & 0x1F;
            c[2] = (D_800FDF60[i][j] >> 10) & 0x1F;
            if (c[0] == 0 && c[1] == 0 && c[2] == 0) {
                g = 0;
            } else {
                g = (c[0] + c[1] + c[2]) / 3;
                if (g == 0) {
                    g = 1;
                }
            }
            D_800FDF60[i][j] = (g << 10) + (g << 5) + g;
        }
        D_800FDF50.y += 12;
        LoadImage(&D_800FDF50, (u32 *)D_800FDF60[i]);
    }
    DrawSync(0);
}

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
    arg0->unk66 = t * D_8006794C->unk1C->unk8E4.battle->unk154C[27].unk2;
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

void func_800C0C00(BtlUnit *u, s32 n) {
    u8 k;
    s32 v;

    if (n == 1) {
        v = u->unk33 * 4;
        k = 0;
    } else if (n == 0x10) {
        v = u->unk33 * 4;
        k = 1;
    } else {
        v = (0x3C - u->unk33) * 4;
        k = 2;
    }
    u->unk40[k] = D_8006794C->unk1C->unk8E4.battle->unk154C[n].unk1 * v;
}

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

void func_800C0EC8(BtlUnit *arg0) {
    Battle *b = D_8006794C->unk1C->unk8E4.battle;
    s32 found;
    BtlUnit *u;
    u32 f;
    s32 k;
    s32 n;

    n = 6;
    for (u = b->unk6F8; u != NULL; u = u->unk0) {
        if ((u->unk68 & 1) == (arg0->unk68 & 1)) {
            if (!(u->unk3C & 0x1103) || ((u->unk3C & 0x100) && u->unk7D != n)) {
                return;
            }
        }
    }
    if (!(arg0->unk68 & 1)) {
        found = 0;
        for (u = b->unk6F8; u != NULL; u = u->unk0) {
            if (u->unk68 & 1) {
                if (!(u->unk3C & 0x1103) || ((u->unk28 == 0 || (u->unk3C & 0x100)) && (u->unk3C & 0x2000)) || func_800B3EF8(u, 0x3D)) {
                    found = 1;
                    break;
                }
            }
        }
        if (!found) {
            return;
        }
        if (b->unkD81 != 1) {
            b->unkD81 = 0;
        }
    } else {
        for (u = b->unk6F8; u != NULL; u = u->unk0) {
            f = u->unk68;
            if (f & 1) {
                k = D_8006794C->unk1C->unk3C[(f >> 1) & 0xF].unk34.b.unk4;
            } else {
                k = 0x13;
            }
            if ((u32)(k - 10) < 2) {
                if (u->unk3C & 0x1043) {
                    break;
                }
                if (func_800B3EF8(u, 0x3A)) {
                    return;
                }
                if (D_8007B720 >= D_8007B92D) {
                    func_8004BF20();
                }
                k = func_80027F34(0xF9);
                if (*D_8007B720++ < k) {
                    func_800E9F68(1);
                    func_800B1250(u->unk4, 0x3A, 0x49, func_800BEC64(0) & 0xFFFF, 1);
                    return;
                }
            }
        }
        b->unkD81 = 1;
        func_800CD5C8(5);
    }
    func_800E9F68(0);
    b->unkD80 = 5;
    func_800B2F24(b);
}

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

void func_800C12CC(Battle4880 *arg0) {
    u8 n;
    u8 it;
    Unk800FDF30 *d;

    n = 0;
    d = &D_800FDF30;
    d->unk0 += arg0->unk44;
    d->unk4 += arg0->unk46;
    for (; n < 16; n++) {
        if (d->unkC[n] == 0xFF) {
            break;
        }
    }
    if (n < 16) {
        if (rand8_inline() < 1 && (it = arg0->unk48.b[3]) != 0xFF) {
            d->unkC[n++] = it;
            arg0->unk48.b[3] = 0xFF;
        } else if (rand8_inline() < 0x20 && (it = arg0->unk48.b[2]) != 0xFF) {
            d->unkC[n++] = it;
            arg0->unk48.b[2] = 0xFF;
        } else if (rand8_inline() < 0x60 && (it = arg0->unk48.b[1]) != 0xFF) {
            d->unkC[n++] = it;
            arg0->unk48.b[1] = 0xFF;
        }
        if ((it = arg0->unk48.b[0]) != 0xFF) {
            d->unkC[n] = it;
        }
    }
    if (d->unk1C == 0xFF && (u32)arg0->unk4C < 100) {
        if (rand8_inline() < 0x20) {
            d->unk1C = arg0->unk4C;
        }
    }
}

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

void func_800C1594(void) {
    s32 i;
    u8 *p;
    s32 c;

    c = 0xFF;
    i = 15;
    p = (u8 *)&D_800FDF30 + i;
    D_800FDF30.unk0 = 0;
    D_800FDF30.unk4 = 0;
    D_800FDF30.unk8 = 0;
    for (; i >= 0; i--) {
        p[12] = c; /* unkC[i] */
        p--;
    }
    D_800FDF30.unk1C = 0xFF;
    D_800FDF30.unk1D = 0;
}

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
    if ((v == 3 || v == 6) && !((arg0->unk132C.unk0 >> 4) & 1)) {
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
        if ((u8)b->unk132C.unk0 >> 7 == 0) {
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

BtlUnit *func_800C1DEC(void) {
    BtlUnit *u;
    for (u = D_8006794C->unk1C->unk8E4.battle->unk6F8; u != NULL; u = u->unk0) {
        if (!(u->unk68 & 0x800001) && (((D_8006794C->unk1C->unk8E4.battle->unk488 + ((u->unk68 >> 1) & 0xF))->unk4 >> 8) & 1)) {
            return u;
        }
    }
    return NULL;
}

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

s32 func_800C1F7C(s32 arg0) {
    u16 ids[4];
    u16 n = 0;
    BtlUnit *u;
    if (arg0 != 0) {
        arg0 = 1;
    }
    for (u = D_8006794C->unk1C->unk8E4.battle->unk6F8; u != NULL; u = u->unk0) {
        if ((u->unk68 & 1) == arg0 && !(u->unk3C & 0x100) && ((u->unk68 >> 28) & 1)) {
            ids[n++] = u->unkB0;
        }
    }
    if (n == 0) {
        return 0;
    }
    if (D_8007B720 >= D_8007B92D) {
        func_8004BF20();
    }
    return ids[*D_8007B720++ % n];
}

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

void func_800C23C8(BtlUnit *arg0, s32 arg1, u32 arg2) {
    s32 k;
    k = arg0->unk68 >> 1;
    k &= 0xF;
    if (!(arg0->unk68 & 1)) {
        k += 9;
    }
    arg0->unkBF = (arg1 & 0xFFFF) / 14;
    arg0->unkC0 = arg2 / 12;
    D_8006794C->unk1C->unk8E4.battle->unk11C0->unk28[k].unk0_0 = 0;
    D_8006794C->unk1C->unk8E4.battle->unk11C0->unk28[k].unk0_8 = arg0->unkBF * 14;
    D_8006794C->unk1C->unk8E4.battle->unk11C0->unk28[k].unk0_20 = arg0->unkC0 * 12;
}

void func_800C24E8(BtlUnit *arg0, s32 arg1) {
    Unk800CEE74 *p = func_800CEE74();
    func_800C8E80(arg0->unk9C, 0x60);
    func_800C8A3C(arg0->unk9C, (p->unkC * arg1) << 11 >> 16, (p->unkD * arg1) << 11 >> 16, (p->unkE * arg1) << 11 >> 16);
    if (arg1 == 0) {
        arg0->unk68 |= 0x400000;
    }
}

void func_800C2598(BtlCmd *arg0, s32 arg1, s16 *arg2) {
    Battle *b = D_8006794C->unk1C->unk8E4.battle;
    s32 sum[4];
    BtlUnitList *l;
    BtlUnit *u;
    BtlUnit *c;
    s32 mask;
    u8 e;

    func_8001D898(sum, 0, 0x10);
    mask = 0;
    b->unk116C.unk0 = arg0->unk4;
    l = &b->unk116C;
    if (arg2 != NULL) {
        l->unk25[0] = arg2[0];
        l->unk25[1] = arg2[1];
        l->unk42 = arg2[2];
        l->unk40 = arg2[3];
    } else {
        l->unk42 = 0;
        l->unk40 = 0;
        l->unk25[1] = 0;
        l->unk25[0] = 0;
    }
    l->unk27 = 0;
    l->unk24 = 0;
    c = arg0->unk4;
    c->unk7E = 0;
    c->unk82 = 0;
    c->unk80 = 0;
    if (arg0->unk10.b.unk0_7 == 0) {
        mask = func_800B3188(arg0);
        e = arg0->unkE;
        if (e == 0xE || e == 0x33) {
            l->unk40 = 2;
        } else if (e == 0x1F) {
            l->unk40 = 4;
        } else if ((u8)(e - 0x39) < 2) {
            l->unk40 = 1;
        }
    } else if (arg0->unk10.b.unk0_7 == 2) {
        arg0->unk10.b.unk0_7 = 1;
        if (!(arg0->unk4->unk68 & 1)) {
            arg0->unk10.b.unk0_11 = 1;
        }
        l->unk40 = 0x11;
    }
    for (u = D_8006794C->unk1C->unk8E4.battle->unk6F8; u != NULL; u = u->unk0) {
        if (u->unkB0 & (u16)arg0->unkC) {
            l->unk4[l->unk24++] = u;
            u->unk7E = 0;
            u->unk82 = 0;
            u->unk80 = 0;
            sum[0] += u->unk98->unk0[0] >> 16;
            sum[2] += u->unk98->unk0[2] >> 16;
        }
        if (u->unkB0 & mask) {
            l->unk28[l->unk27++] = u;
        }
        if (arg0->unkE == 0x1F && (u->unk68 & 1) && D_8006794C->unk1C->unk3C[(u->unk68 >> 1) & 0xF].unk34.b.unk4 == 2) {
            l->unk44 = u;
        }
    }
    l->unk38 = sum[0] / (s8)l->unk24;
    l->unk3A = 0;
    l->unk3C = sum[2] / (s8)l->unk24;
    func_800E3DA8();
    func_800E701C(l);
    func_800D62A0(arg1);
}

INCLUDE_RODATA("asm/us/ovl_0b/nonmatchings/ovl_0b", D_800A7948);

void func_800C28F4(BtlCmd *cmd) {
    BtlUnit *u = cmd->unk4;
    s32 id;
    s32 k;
    u16 t;

    switch (cmd->unkE) {
    case 0x1:
    case 0x31:
    case 0x34:
        if (u->unk68 & 1) {
            id = D_8006794C->unk1C->unk3C[(u->unk68 >> 1) & 0xF].unk34.b.unk4 + 0x64;
        } else {
            id = 0x77;
        }
        func_800C2598(cmd, id, 0);
        break;
    case 0x2:
        if (u->unk68 & 1) {
            k = D_8006794C->unk1C->unk3C[(u->unk68 >> 1) & 0xF].unk34.b.unk4;
        } else {
            k = 0x13;
        }
        switch (k) {
        case 0:
            func_800C2598(cmd, 0xC8, 0);
            break;
        case 1:
            func_800C2598(cmd, 0x111, 0);
            break;
        case 0xE:
            func_800C2598(cmd, 0x13, 0);
            break;
        case 0xF:
            func_800C2598(cmd, 0x77, 0);
            break;
        default:
            func_800C2598(cmd, 0x14, 0);
            break;
        }
        break;
    case 0x4:
        func_800CDEF0(cmd);
        u->unk9C->unkC = (BtlUnit9CC *)u->unkA4[12];
        u->unkA0->unk3A = 0;
        func_800B2804(u);
        return;
    case 0x7:
        func_800CDEF0(cmd);
        u->unk9C->unkC = (BtlUnit9CC *)u->unkA4[9];
        return;
    case 0xE:
    case 0x33:
        func_800C2598(cmd, D_8006B1F4[cmd->unkF - 0xE0].unk0_8, 0);
        break;
    case 0xF:
        /* padded: the first jump table is 8-aligned in its object */
        __asm__(".section .rodata\n\t.word 0\n\t.section .text");
        switch (D_80069018[cmd->unkF].unk8) {
        case 1:
            func_800C2598(cmd, 0x110, 0);
            break;
        case 2:
            func_800C2598(cmd, 0x10A, 0);
            break;
        case 3:
        case 4:
            func_800C2598(cmd, 0x10B, 0);
            break;
        case 5:
            func_800C2598(cmd, 0x10C, 0);
            break;
        case 6:
            func_800C2598(cmd, 0x10D, 0);
            break;
        case 7:
            func_800C2598(cmd, 0x109, 0);
            break;
        case 8:
        case 9:
        case 10:
            func_800C2598(cmd, 0x10E, 0);
            break;
        case 11:
            func_800C2598(cmd, 0x10F, 0);
            break;
        case 12:
            func_800C2598(cmd, 0x115, 0);
            break;
        }
        break;
    case 0x2F:
        if ((u16)cmd->unkC < 0x10 && (cmd->unk8->unk8 & 8) && !cmd->unk10.b.unk0_0) {
            t = func_800B081C((u16)cmd->unkC);
            if (t) {
                cmd->unkC = t;
                cmd->unk10.b.unk0_5 = 1;
            }
        }
    case 0x35:
    case 0x36:
    case 0x37:
        func_800CA530(cmd);
        return;
    case 0x3B:
        id = 0x1E9;
        if (u->unk3C & 0x4000) {
            id = 0x101;
        }
        func_800C2598(cmd, id, 0);
        break;
    case 0x12:
        switch (cmd->unkF) {
        case 0x31:
            D_8006794C->unk1C->unk8E4.battle->unk11BB = 0x99;
            break;
        case 0x33:
            D_8006794C->unk1C->unk8E4.battle->unk11BB = 0x9A;
            break;
        case 0x35:
            D_8006794C->unk1C->unk8E4.battle->unk11BB = 0x9B;
            break;
        case 0x37:
            D_8006794C->unk1C->unk8E4.battle->unk11BB = 0x9C;
            break;
        case 0x3A:
            D_8006794C->unk1C->unk8E4.battle->unk11BB = 0x9D;
            break;
        case 0x3C:
            D_8006794C->unk1C->unk8E4.battle->unk11BB = 0x9E;
            break;
        case 0x3E:
            D_8006794C->unk1C->unk8E4.battle->unk11BB = 0x9F;
            break;
        case 0x40:
            D_8006794C->unk1C->unk8E4.battle->unk11BB = 0xA0;
            break;
        }
    default:
        if ((cmd->unk8->unk0_0 == 3 && !cmd->unk10.b.unk0_0) || (cmd->unk10.w & 0x600) ||
            cmd->unkE == 0x20 || cmd->unkE == 0x21) {
            func_800C2598(cmd, cmd->unk8->unkC, 0);
        } else {
            func_800C2598(cmd, cmd->unk8->unk0_8, 0);
        }
        break;
    }
}
