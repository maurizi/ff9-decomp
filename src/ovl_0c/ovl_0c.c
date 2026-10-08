#include "ovl_0c.h"

void func_800A8860(void) {
    func_800A8B14();
    do {
        func_800A88C4(D_8006794C->unk1C->unk7DC);
    } while (!(D_8006794C->unk0 & 0x1001));
    func_800A8CA4();
}

void func_800A88C4(s16 arg0) {
    func_800A924C();
    SetDispMask(1);
    do {
        func_800AC580();
        func_80019194();
        if ((D_80073990 & 0x04000000) && (D_800739A0[0] & 0xF8) && !(D_800739A0[0] & 0xFA0001)) {
            func_800ACEEC();
        }
        if (!(D_8006794C->unk1C->unk0 & 0x100)) {
            if (!(D_8006794C->unk1C->unk8E4.world->unk0 & 0x100)) {
                switch (func_800B6BC4()) {
                case 3:
                    D_8006794C->unk1C->unk8E4.world->unk8->unk1B = 2;
                    func_800AC7D4();
                    break;
                case 4:
                    D_8006794C->unk1C->unk8E4.world->unk8->unk1B = 1;
                    D_8006794C->unk0 |= 0x1000;
                    break;
                case 5:
                    D_8006794C->unk0 |= 0x2000;
                    break;
                }
            }
            if (!(D_8006794C->unk1C->unk8E4.world->unk0 & 0x200)) {
                func_80032098();
            }
            if (!(D_8006794C->unk1C->unk8E4.world->unk0 & 0x400)) {
                func_800ACAFC();
            }
            if (!(D_8006794C->unk1C->unk8E4.world->unk0 & 0x800)) {
                func_80023A4C();
            }
        }
        func_800AC5C8();
    } while (!(D_8006794C->unk0 & 0x3001));
    func_800A95A0();
}

void func_800A8B14(void) {
    WorldMap *wm;
    GameData *gd;

    func_800A8CF0();
    wm = D_8006794C->unk1C->unk8E4.world;
    func_800A8D28();
    D_8006794C->unk9 = 3;
    func_8001CD04(0xDA);
    func_8001CFB4();
    SetDispMask(1);
    D_8006794C->unk1C->unk838 = func_800AA15C;
    D_8006794C->unk1C->unk83C = (void (*)(s32, s32, s32, s32, s32))func_800AB0C0;
    D_8006794C->unk1C->unk8BC = func_800AD10C;
    D_8006794C->unk1C->unk84C = 0;
    gd = D_8006794C->unk1C;
    gd->unk39 = 0;
    gd->unk38 = 0;
    wm->unk0 |= 1;
    func_800A8FF0();
    func_800A91A4();
    func_800A8D78();
    func_800A919C();
    func_800A8F18();
    func_800A91AC();
    func_800A8F8C();
    func_800A9220();
    func_800B69B4();
    func_800239D0();
    if (func_800C45D8() == 0) {
        D_8006794C->unk1C->unk0 |= 0x400000;
    }
    if (D_8006794C->unk1C->unk0 & 0x400000) {
        func_800AB0C0(0x8601, 0, 0x78, 0, 0);
    }
    wm->unk4 = (u8 *)D_80067948;
}

void func_800A8CA4(void) {
    s32 i;

    for (i = 1; i >= 0; i--) {
        D_800739B0[i].unk1 = 0;
        D_800739B0[i].unk0 = 0;
    }
    func_800B8068();
    func_800A91B4();
}

void func_800A8CF0(void) {
    D_80067948 = (s32)(D_80067944 + 0x32000);
    D_8006794C->unk1C->unk8E4.world = (WorldMap *)D_80067948;
    D_80067948 += 0x3000;
}

void func_800A8D28(void) {
    u32 i;
    WorldMap *wm = D_8006794C->unk1C->unk8E4.world;

    wm->unk0 = 0;
    wm->unk4 = 0;
    wm->unk10 = -1;
    wm->unkC = 0;
    wm->unk8 = 0;
    for (i = 0; i < 0x34; i++) {
        wm->unk14[i] = 0;
    }
}

INCLUDE_ASM("asm/us/ovl_0c/nonmatchings/ovl_0c", func_800A8D78);

s32 func_800A8F18(void) {
    s32 addr = (s32)func_8001C7FC(4, 0x70064, 0);

    func_800AD114(D_8006794C->unk1C->unk818, addr, (s32)func_8001C7FC(5, 0x70064, 0));
    DrawSync(0);
    return 0;
}

s32 func_800A8F8C(void) {
    s32 addr = (s32)func_8001C7FC(4, 0x42774, 0);

    func_8001D9CC(addr, 0x280, 0xDE, 0x330, 0xB0);
    func_8001D8A8(addr);
    DrawSync(0);
    return 0;
}

s32 func_800A8FF0(void) {
    s32 id;
    FileEntry *f;
    u8 *buf;
    Unk80021938 *e;

    id = func_800A9228();
    f = func_8001E218(1, id);
    buf = D_80067944 + 0x34000;
    if (func_800220E8(id) != 0) {
        e = func_80021E5C(id);
        func_8001DAA4((s32)buf, e->unk8, e->unkC, 0);
        DrawSync(0);
        if (e->unkC < e->unk1C) {
            func_8002177C(D_8006794C->unk1C->unk854, f->unk4, (f->unkC - f->unk4) << 11, buf + e->unkC, e->unkC, -1, 0);
            while (func_800217C4(D_8006794C->unk1C->unk854) != 0) {
            }
        }
        func_80022390(id);
    } else {
        func_8002177C(D_8006794C->unk1C->unk854, f->unk4, (f->unkC - f->unk4) << 11, buf, 0, -1, 0);
        while (func_800217C4(D_8006794C->unk1C->unk854) != 0) {
        }
    }
    D_8006794C->unk1C->unk8E4.world->unkC = buf;
    func_8001C750((s32)D_8006794C->unk1C->unk8E4.world->unkC, 0);
    return 0;
}

s32 func_800A919C(void) {
    return 0;
}

s32 func_800A91A4(void) {
    return 0;
}

s32 func_800A91AC(void) {
    return 0;
}

s32 func_800A91B4(void) {
    void *p = D_8006794C->unk1C->unk8E4.world->unkC;

    if (p != NULL) {
        func_8001C784(p);
        D_8006794C->unk1C->unk8E4.world->unkC = 0;
    }
    return 0;
}

s32 func_800A9220(void) {
    return 0;
}

s32 func_800A9228(void) {
    s32 v = 0x2712;
    if (D_8007AEF0.h[0] >= 0x2B52) {
        v = 0x2716;
    }
    return v;
}

INCLUDE_ASM("asm/us/ovl_0c/nonmatchings/ovl_0c", func_800A924C);

INCLUDE_ASM("asm/us/ovl_0c/nonmatchings/ovl_0c", func_800A95A0);

void func_800A98B8(void) {
    WorldMap *wm = D_8006794C->unk1C->unk8E4.world;

    wm->unk8 = &wm->unk48;
    wm->unk8->unk2C = D_80067944 + 0x5EB00;
    D_80067948 = (s32)(wm + 1);
}

INCLUDE_ASM("asm/us/ovl_0c/nonmatchings/ovl_0c", func_800A98F8);

s32 func_800A99C4(WorldMapObj *arg0, u16 arg1) {
    arg0->unk14 = func_8001C7FC(4, arg1 | 0x60000, 0);
    return 0;
}

s32 func_800A9A04(WorldMapObj *arg0, u16 arg1) {
    arg0->unk10 = func_8001C7FC(4, arg1 | 0x50000, 0);
    return 0;
}

void func_800A9A44(void) {
    GameData *gd = D_8006794C->unk1C;
    RECT *r;
    u16 h;
    u32 *dst;

    if (gd->unk8E4.world->unk10 != -1) {
        r = &D_800CE0B8[D_800CCA1C];
        h = 0xDA;
        r->x = D_8006794C->unk10[gd->unk8].clip.x;
        r->y = D_8006794C->unk10[D_8006794C->unk1C->unk8].clip.y;
        dst = (u32 *)(D_80067944 + 0x91800);
        r->h = h;
        r->w = ((u32)D_8006794C->unk1C->unk8E4.world->unk10 >> 1) / h + 1;
        StoreImage(r, dst);
        DrawSync(0);
    }
}

void func_800A9B54(void) {
    u8 *dst;

    while (func_800AB0C0(0xC00, 0, 0, 0, 0) != 0) {
    }
    if (D_8006794C->unk1C->unk8E4.world->unk10 != -1) {
        dst = D_80067944 + 0x91800;
        func_80055DF4();
        func_80055E14((s32)dst, D_8006794C->unk1C->unk8E4.world->unk10, 1);
        D_8006794C->unk1C->unk8E4.world->unk10 = -1;
    }
}

s32 func_800A9C1C(WorldMapObj *arg0) {
    GameData *gd;
    u8 *buf;
    s32 a;
    s32 b;
    s32 i;
    s32 p;
    u8 *q;
    s32 c;

    gd = D_8006794C->unk1C;
    func_8002177C(gd->unk854, gd->unk800, gd->unk808 << 11, arg0->unk30, 0, -1, 0);
    while (func_800217C4(D_8006794C->unk1C->unk854) != 0) {
    }
    buf = arg0->unk30;
    a = func_8001C8B0((s32)buf, 4, 0x1B0000, 0);
    b = func_8001C8B0((s32)buf, 4, 0x1B0001, 0);
    D_8006794C->unk1C->unk7E4 = (struct FileTable *)func_8001C8B0((s32)buf, 4, 0x140000, 0);
    for (i = func_8001C8B0(a, 1, 0x40000, 0) - 1; i >= 0; i--) {
        p = func_8001C8B0(a, 2, (i & 0xFFFF) | 0x40000, 0);
        if (p != -1) {
            func_8001D8A8(p);
        }
    }
    DrawSync(0);
    func_800AD114(a, b, func_8001CA70(b));
    DrawSync(0);
    b = a;
    arg0->unk34[1] = (void *)b;
    func_8001C750(b, 0);
    q = (u8 *)func_8001C8B0(b, 4, 0x1D0000, 0);
    arg0->unk1A = *q;
    q += 4;
    arg0->unk28 = (s32)q;
    arg0->unk30 = (u8 *)arg0->unk34[1] + func_8001CA70((s32)arg0->unk34[1]);
    return 0;
}

s32 func_800A9DE4(WorldMapObj *arg0) {
    if (arg0->unk34[1] != NULL) {
        func_8001C784(arg0->unk34[1]);
        arg0->unk34[1] = NULL;
    }
    return 0;
}

s32 func_800A9E24(u16 arg0, s32 *arg1, s32 *arg2) {
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

s32 func_800A9EC4(s32 arg0, s32 *arg1, s32 *arg2) {
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

s32 func_800A9FC0(u16 arg0, s32 *arg1, s32 *arg2) {
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

s32 func_800AA060(s32 arg0, s32 *arg1, s32 *arg2) {
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

INCLUDE_ASM("asm/us/ovl_0c/nonmatchings/ovl_0c", func_800AA15C);

s32 func_800AA39C(s32 arg0) {
    Unk800AA39C *t = func_8001C7FC(8, arg0, 0x120000);
    s32 i;
    Unk800AA39CEntry *e;

    e = t->unk4;
    for (i = 0; i < t->unk1; i++) {
        if (e->unk0 == (u16)arg0) {
            return e->unk2 | 0x30000;
        }
    }
    return -1;
}

INCLUDE_ASM("asm/us/ovl_0c/nonmatchings/ovl_0c", func_800AA418);

void func_800AAD0C(s32 arg0, s32 arg1) {
    func_80062E3C(arg0, 0x280, 0xDE, 0x330, 0xB0, arg1, 1);
}

INCLUDE_ASM("asm/us/ovl_0c/nonmatchings/ovl_0c", func_800AAD48);

INCLUDE_RODATA("asm/us/ovl_0c/nonmatchings/ovl_0c", D_800A7000);

INCLUDE_ASM("asm/us/ovl_0c/nonmatchings/ovl_0c", func_800AB0C0);

INCLUDE_ASM("asm/us/ovl_0c/nonmatchings/ovl_0c", func_800AC020);

void func_800AC21C(s32 arg0, u16 arg1) {
    GameData *gd = D_8006794C->unk1C;

    gd->unk826[0] = arg0;
    if (arg0 == 0) {
        gd->unk842 = arg1;
        gd->unk8C4 = VSync(-1);
    }
}

s32 func_800AC25C(void) {
    GameData *gd = D_8006794C->unk1C;

    return (u32)(VSync(-1) - gd->unk8C4) < gd->unk842;
}

s32 func_800AC29C(s32 arg0, s32 *arg1) {
    FileEntry *e;
    FileEntry *f;

    if (((arg0 >> 16) & 0xFF) == 7) {
        e = func_80063088(D_8006794C->unk1C->unk7E4, arg0);
        if (arg1 != NULL) {
            *arg1 = e->unk8 << 11;
        }
        return e->unk4;
    } else {
        f = func_8001E218(0xB, arg0);
        if (arg1 != NULL) {
            *arg1 = (f->unkC - f->unk4) << 11;
        }
        return f->unk4;
    }
}

s32 func_800AC344(s32 arg0) {
    FileTable *t = D_8006794C->unk1C->unk7E4;
    s32 i;
    FileEntry *e = t->unk4;

    for (i = 0; i < t->unk2; i++) {
        if (e[i].unk0 == arg0) {
            return e[i].unkC;
        }
    }
    return -1;
}

s32 func_800AC3A8(s32 arg0) {
    s32 t = func_8001C8B0(arg0, 3, 0x70000, 0);

    func_80015C28((u8 *)arg0, (u8 *)func_8001C8B0(arg0, 4, t, 0), func_8001C8B0(arg0, 5, t, 0));
    return 0;
}

INCLUDE_ASM("asm/us/ovl_0c/nonmatchings/ovl_0c", func_800AC424);

void func_800AC580(void) {
    GameData *gd = D_8006794C->unk1C;

    ClearOTagR(gd->unk10[gd->unk8] - 8, 0x1010);
}

void func_800AC5C8(void) {
    GameState *gs;
    GameData *gd;
    s32 f;

    if ((D_800739A0[0] & 0xF09) == 0xF09 || (D_800739A0[1] & 0xF09) == 0xF09) {
        D_8006794C->unk0 |= 1;
    }
    DrawSync(0);
    gs = D_8006794C;
    f = gs->unk1C->unk8E4.world->unk0;
    if (!(f & 0x400)) {
        gs->unk9 = 2;
    } else if ((f & 0x1000) || (gs->unk1C->unk0 & 0x1C) == 0x1C) {
        gs->unk9 = 0;
    } else {
        gs->unk9 = 3;
    }
    VSync(D_8006794C->unk9);
    ResetGraph(1);
    PutDispEnv(&D_8006794C->unkC[D_8006794C->unk1C->unk8]);
    gd = D_8006794C->unk1C;
    if (!(gd->unk0 & 0x100)) {
        if (gd->unk8E4.world->unk0 & 0x400) {
            DrawOTagEnv(gd->unk10[gd->unk8] + 0xFFF, &D_8006794C->unk10[gd->unk8]);
        } else {
            DrawOTag(gd->unk10[gd->unk8]);
        }
    }
    if (!(D_8006794C->unk1C->unk0 & 0x20)) {
        D_8006794C->unk1C->unk8 ^= 1;
        func_8002C0DC(D_8006794C->unk1C->unk8);
    }
}

INCLUDE_ASM("asm/us/ovl_0c/nonmatchings/ovl_0c", func_800AC7D4);

INCLUDE_ASM("asm/us/ovl_0c/nonmatchings/ovl_0c", func_800ACAFC);

void func_800ACE24(void) {
    GameData *gd = D_8006794C->unk1C;
    s32 i = 0;
    WorldMapObj *obj = gd->unk8E4.world->unk8;
    FileEntry *e;
    s32 id;
    s32 v;

    obj->unk26 = gd->unk81C[0];
    id = (u16)obj->unk24 | 0x110000;
    e = D_8006794C->unk1C->unk7E4->unk4;
    for (; i < D_8006794C->unk1C->unk7E4->unk2; i++) {
        v = e[i].unkC;
        if (e[i].unk0 == id && ((v >> 16) & 0xFF) == 7) {
            D_8006794C->unk1C->unk8E4.world->unk8->unk26 = v;
            return;
        }
    }
}

INCLUDE_ASM("asm/us/ovl_0c/nonmatchings/ovl_0c", func_800ACEEC);

s32 func_800AD10C(void) {
    return 0;
}

void func_800AD114(s32 arg0, s32 arg1, s32 arg2) {
    RECT r;
    RECT *rp = NULL;

    if (!(D_8006794C->unk1C->unk8E4.world->unk0 & 0x400)) {
        rp = &r;
        r.x = 0x280;
        r.w = 0x140;
        r.y = 0;
        r.h = 0xDA;
    }
    func_8001DAA4(arg0, arg1, arg2, rp);
}

void func_800AD180(void) {
    FileTable *t = D_8006794C->unk1C->unk7E4;
    u8 *dst;

    if (t != (FileTable *)-1) {
        dst = D_80067944 + 0x146F00;
        func_800AD114((s32)dst, (s32)t, (t->unk2 * 0x10) | 4);
        DrawSync(0);
        D_8006794C->unk1C->unk7E4 = (FileTable *)dst;
    }
}

void func_800AD204(void) {
    void *p = &D_8006794C->unk1C->unk880;

    D_800CE0E8 = p;
    D_800CE0F0 = p;
    D_8006794C->unk1C->unkA = 0xDC;
    SetGeomOffset(0xA0, 0x70);
    D_800CE1B0.vx = 0;
    D_800CE1B0.vy = -0x4E20;
    D_800CE1B0.vz = 0;
    D_800CE202 = 1;
    D_800CE0EF = 0;
    D_800CE100 = 0;
    D_800CE1E1 = 0;
    if (D_8006794C->unkA == 1 && D_8006794C->unk1C->unk890 == 0) {
        D_800CE0F0->unk2 = 0;
        D_800CE0F0->unk4 = 0;
        D_800CE0F0->unk8 = 0;
        D_800CE0F0->unkC = 0;
        D_800CE0E8->unk11 = 0;
    }
    if (D_800CE0F0->unk4 < 0) {
        D_800CE0F0->unk2 = 0;
    }
    if (D_800CE0F0->unk4 > 0) {
        D_800CE0F0->unk2 = 0x1000;
    }
    D_800CE1A8 = 0;
    D_800CE188 = 0;
    D_800CE1E8 = D_800CCA68[1];
    D_800CE0EC = 0;
    D_800CE0ED = 0;
    D_800CE0EE = 0;
    D_800CE1F2 = 1;
    D_800CE1F3 = 0;
}

INCLUDE_ASM("asm/us/ovl_0c/nonmatchings/ovl_0c", func_800AD39C);

INCLUDE_ASM("asm/us/ovl_0c/nonmatchings/ovl_0c", func_800AD738);

INCLUDE_ASM("asm/us/ovl_0c/nonmatchings/ovl_0c", func_800AE280);

void func_800AE4C8(s32 arg0, s32 arg1, s32 arg2, s32 arg3, Unk800CE1E8 *arg4) {
    Unk800CE1E8 *a;
    Unk800CE1E8 *b;
    Unk800CE1E8 *c;
    s32 t;
    s16 tmp[5];

    c = &D_800CCA68[D_800CCACC[arg1][arg2][2]];
    a = &D_800CCA68[D_800CCACC[arg1][arg2][0]];
    b = &D_800CCA68[D_800CCACC[arg1][arg2][1]];
    t = func_800AEA94(arg0) & 0xFFFF;
    tmp[0] = func_800B822C(a->unk0[0], b->unk0[0], t, 9);
    tmp[1] = func_800B822C(a->unk0[1], b->unk0[1], t, 0xE);
    tmp[2] = func_800B822C(a->unk0[2], b->unk0[2], t, 0xD);
    tmp[3] = func_800B822C(a->unk0[3], b->unk0[3], t, 0xD);
    tmp[4] = func_800B822C(a->unk0[4], b->unk0[4], t, 0xD);
    arg4->unk0[0] = func_800B822C(tmp[0], c->unk0[0], arg3, 9);
    arg4->unk0[1] = func_800B822C(tmp[1], c->unk0[1], arg3, 0xE);
    arg4->unk0[2] = func_800B822C(tmp[2], c->unk0[2], arg3, 0xD);
    arg4->unk0[3] = func_800B822C(tmp[3], c->unk0[3], arg3, 0xD);
    arg4->unk0[4] = func_800B822C(tmp[4], c->unk0[4], arg3, 0xD);
}

void func_800AE670(void) {
    u8 r;

    if (D_800CE0F0->unk8 == 0
        && (*D_800CF430 >= 0x137E || ((D_800CCCC0[D_800CE3DC->unk11].unk8 & 0x3F00) >> 8) != 0xC)) {
        r = 0;
        if (D_800CE0F0->unk2 == 0) {
            r = 1;
            D_800CE0F0->unk4 = 0x100;
        }
        if (D_800CE0F0->unk2 == 0x1000) {
            r = 1;
            D_800CE0F0->unk4 = -0x100;
        }
        if (r == 0) {
            D_800CE0F0->unk4 = -D_800CE0F0->unk4;
        }
    }
}

s32 *func_800AE744(void) {
    return D_800CE108;
}

s32 *func_800AE750(void) {
    return D_800CE198;
}

void func_800AE75C(void) {
    func_800AE280();
    func_800AD738();
}

void func_800AE784(s16 arg0, s16 arg1) {
    D_800CE1A8 = 0;
    D_800CE1C0 = arg0;
    D_800CE1F8 = D_800CE1E8;
    if (D_800D33F4 != 0) {
        D_800CE188 = arg1;
    } else {
        D_800CE188 = 0;
    }
}

s32 func_800AE7F0(void) {
    s16 v = D_800CE0F0->unk2;

    if (v == 0) {
        return 0;
    }
    if (v == 0x1000) {
        return 2;
    }
    return 1;
}

void func_800AE828(void) {
    if (D_800CE0E8->unk11 != 0) {
        D_800CE27C = 1;
    }
    D_800CE0E8->unk11 = ~D_800CE0E8->unk11;
}

void func_800AE860(void) {
    D_800CE0EC = 1;
    D_800CE0ED = 1;
    D_800CE0EE = 1;
}

INCLUDE_ASM("asm/us/ovl_0c/nonmatchings/ovl_0c", func_800AE880);

s32 func_800AEA94(s32 arg0) {
    arg0 = (arg0 - 1000) << 12;
    arg0 /= 4500;
    if (arg0 > 0x1000) {
        arg0 = 0x1000;
    }
    if (arg0 < 0) {
        arg0 = 0;
    }
    return arg0 & 0xFFFF;
}

void func_800AEADC(void) {
    s32 i;
    s32 j;

    D_800CE3DC = &D_800CE210;
    D_800CE3D8 = 0;
    D_800CE3D0 = 0;
    D_800CE268 = 0;
    D_800CE270 = 0;
    D_800CE3C4 = -1;
    D_800CE3C8 = 0;
    D_800CE25E = 0;
    D_800CE3BE = 0;
    D_800CE3C0 = 0;
    D_800CE278 = 0;
    D_800CE3E0 = 0;
    D_800CE3BC = 0;
    D_800CE276 = 0;
    D_800CE3CC = 0;
    D_800CE284 = 0;
    D_800CE3D4 = 0x1000;
    D_800CE260 = 0;
    D_800CE264 = 1;
    D_800CE274 = 0;
    D_800CE280 = 0;
    D_800CE25C = 0;
    D_800CE27C = 0;
    D_800CE25D = 0;
    D_800CE26D = 0;
    D_800CE209 = 0;
    for (i = 0; i < 11; i++) {
        D_800CE288[i].unk0 = 0;
        for (j = 0; j < 6; j++) {
            D_800CE288[i].unk4[j] = j;
            D_800CE288[i].unk10[j] = -1;
        }
    }
    for (i = 0; i < 22; i++) {
        D_800CCCC0[i].unk8 = 0;
        D_800CCCC0[i].unkC = 0;
        D_800CCCC0[i].unkE = 0;
    }
    if (D_800CF444 == 4) {
        D_800CCB70[9].unkB = 3;
        D_800CCB70[0].unkB = 3;
    } else {
        D_800CCB70[9].unkB = 2;
        D_800CCB70[0].unkB = 0;
    }
}

INCLUDE_ASM("asm/us/ovl_0c/nonmatchings/ovl_0c", func_800AEC74);

INCLUDE_ASM("asm/us/ovl_0c/nonmatchings/ovl_0c", func_800AEF10);

INCLUDE_ASM("asm/us/ovl_0c/nonmatchings/ovl_0c", func_800AF08C);

INCLUDE_ASM("asm/us/ovl_0c/nonmatchings/ovl_0c", func_800AF414);

INCLUDE_ASM("asm/us/ovl_0c/nonmatchings/ovl_0c", func_800AF66C);

INCLUDE_ASM("asm/us/ovl_0c/nonmatchings/ovl_0c", func_800AF8C0);

INCLUDE_ASM("asm/us/ovl_0c/nonmatchings/ovl_0c", func_800AFCE0);

void func_800B01F0(Unk8004AB64 *arg0) {
    u8 flat;
    s32 h;
    Unk800CCCC0 *c;
    s32 y;
    s16 d;

    if (D_800D33F4 != 0) {
        c = &D_800CCCC0[arg0->unk11];
        if (c->unk5 != 0xA) {
            h = func_800B36B4(c->unk0, c->unk8, &flat);
            if (c->unk3 == 0) {
                if (c->unk0 != 0) {
                    if (c->unk4 == 7) {
                        c->unkC = 0x118;
                    } else if (flat != 0 || h == 0) {
                        c->unkC = h;
                    } else {
                        if (c->unkC < h - 0x15) {
                            c->unkC += 0x14;
                        }
                        if (c->unkC > h + 0x15) {
                            c->unkC -= 0x14;
                        }
                    }
                }
                arg0->unk14.vy = c->unkE + c->unkC;
            } else {
                y = c->unkE + h;
                if (y < arg0->unk14.vy) {
                    arg0->unk14.vy = y;
                }
                if (D_800CE202 == 0 && D_800CE3DC->unk14.vy < -0x2A30 && D_800CE0EF == 0) {
                    D_800CE3DC->unk14.vy = -0x2A30;
                }
                d = arg0->unk14.vy - c->unkE;
                c->unkC = d;
                if (d < 0) {
                    c->unkC = 0;
                }
            }
        }
    }
}

INCLUDE_ASM("asm/us/ovl_0c/nonmatchings/ovl_0c", func_800B0378);

INCLUDE_ASM("asm/us/ovl_0c/nonmatchings/ovl_0c", func_800B0D6C);

INCLUDE_ASM("asm/us/ovl_0c/nonmatchings/ovl_0c", func_800B105C);

INCLUDE_ASM("asm/us/ovl_0c/nonmatchings/ovl_0c", func_800B153C);

INCLUDE_ASM("asm/us/ovl_0c/nonmatchings/ovl_0c", func_800B17B4);

INCLUDE_ASM("asm/us/ovl_0c/nonmatchings/ovl_0c", func_800B1CD0);

INCLUDE_ASM("asm/us/ovl_0c/nonmatchings/ovl_0c", func_800B20B8);

INCLUDE_ASM("asm/us/ovl_0c/nonmatchings/ovl_0c", func_800B2C24);

INCLUDE_ASM("asm/us/ovl_0c/nonmatchings/ovl_0c", func_800B2E54);

void func_800B30C0(void) {
    Unk16D8 *n;
    Unk8004AB64 *o;
    Unk800CCCC0 *c;
    VECTOR pos;
    s32 tmp;
    u16 h;

    n = func_8004AE9C();
    if (n != NULL) {
        do {
            o = n->unk4;
            if (o->unk5 == 4) {
                pos = o->unk14;
                c = &D_800CCCC0[o->unk11];
                func_800BE748(&pos, &c->unk8, &tmp, 0, &h);
                c->unkE = h;
                c->unkC = func_800B36B4(c->unk0, c->unk8, 0);
                func_800B01F0(o);
                o->unk40 = o->unk14;
            }
            n = n->unk0;
        } while (n != NULL);
    }
}

void func_800B31B8(void) {
    s8 old;
    Unk800CE3C8 *p;

    func_800B3638();
    D_800CE3BC = 0;
    D_800CE276 = 0;
    old = D_800CE3C4;
    p = &D_800CCB70[0];
    func_800C4850(D_800CE3C8->unkC, 0x7F, 0);
    func_800C4850(0x26, 0x7F, 0);
    func_800C42B0(p->unkB);
    func_800AE784(p->unk11, 0x40);
    if (D_800CCE50 & 0x20) {
        func_80015C38("w_movementChange SP1 : %d -> %d\n", old, 0);
    }
}

/* padded: the next jump table is 8-aligned */
const char D_800A7164[0x1C] = "w_movementChange SP2\n";

void func_800B326C(void) {
    func_8004B20C(0);
    func_800C48F4(0x26);
    func_800C48F4(D_800CE3C8->unkC);
    D_800CE3C4 = D_8007AEF0.b[0xBE];
    D_800CE3C8 = &D_800CCB70[D_800CE3C4];
    if (func_8004AE00() != NULL) {
        D_800CE3DC = func_8004AE00();
    }
    D_800CE1F2 = D_800CE3C8->unk8 == 0;
    func_800AFCE0();
    if (D_800CCE50 & 0x20) {
        func_80015C38(D_800A7164);
    }
}

INCLUDE_ASM("asm/us/ovl_0c/nonmatchings/ovl_0c", func_800B3340);

s32 func_800B3398(s32 *arg0) {
    s32 v;
    s32 b;

    *arg0 = 0;
    if (((D_80073940[0].unk0 >> 12) & 0xF) == 7) {
        v = D_8007393A - 0x80;
        if (D_8007393A - 0x40U <= 0x80) {
            v = 0;
        }
        if (v > 0x40) {
            v = (v - 0x40) * 2;
        }
        if (v < -0x40) {
            v = (v + 0x40) * 2;
        }
        if (v != 0) {
            *arg0 = v;
            return 1;
        }
    }
    b = func_800B8498();
    if (b & 0x80) {
        *arg0 = -0x80;
    }
    if (b & 0x20) {
        *arg0 = 0x80;
    }
    return 0;
}

s32 func_800B345C(s32 *arg0) {
    s32 v;

    *arg0 = 0;
    if (((D_80073940[0].unk0 >> 12) & 0xF) == 7) {
        v = D_8007393B - 0x80;
        if (D_8007393B - 0x40U <= 0x80) {
            v = 0;
        }
        if (v > 0x40) {
            v = (v - 0x40) * 2;
        }
        if (v < -0x40) {
            v = (v + 0x40) * 2;
        }
        if (v != 0) {
            *arg0 = v;
            return 1;
        }
    }
    if (func_800B8498() & 0x10) {
        *arg0 = -0x80;
    }
    if (func_800B8498() & 0x40) {
        *arg0 = 0x80;
    }
    return 0;
}

s32 func_800B3524(s32 *arg0) {
    s32 v;

    *arg0 = 0;
    if (((D_80073940[0].unk0 >> 12) & 0xF) == 7) {
        v = 0x80 - D_80073939;
        if (v + 0x40U <= 0x80) {
            v = 0;
        }
        if (v > 0x40) {
            v = (v - 0x40) * 2;
        }
        if (v < -0x40) {
            v = (v + 0x40) * 2;
        }
        if (v != 0) {
            *arg0 = v;
            return 1;
        }
    }
    if (func_800B8498() & 0x80000) {
        *arg0 = -0x80;
    }
    if (func_800B8498() & 0x20000) {
        *arg0 = 0x80;
    }
    return 0;
}

void func_800B35F4(void) {
    D_800CE25C = 2;
    D_800CE3E4 = 10000;
    D_800CF480 = D_8007AEF0.b[0x67] - 1;
    D_800CE26C = D_800CE0E8->unk11;
}

void func_800B3638(void) {
    if (D_800CE25C != 0) {
        D_800CE3E4 = 0;
        D_800CE25C = 0;
        if (D_800CE26C != D_800CE0E8->unk11) {
            func_800AE828();
        }
        D_800CE0E8->unk11 = D_800CE26C;
    }
}

void func_800B36AC(void) {
}

s32 func_800B36B4(s32 arg0, u32 arg1, u8 *arg2) {
    s32 i;
    switch ((s32)(arg1 & 0xFC) >> 2) {
    case 0x24:
    case 0x25:
    case 0x26:
        i = 0;
        break;
    case 0x35:
        i = 1;
        break;
    case 0x36:
        i = 2;
        break;
    case 0x37:
        i = 3;
        break;
    case 0x38:
        i = 4;
        break;
    case 0x39:
        i = 5;
        break;
    case 0x33:
        i = 6;
        break;
    case 0x30:
        i = 7;
        break;
    default:
        i = 8;
        break;
    }
    if (arg2 != NULL) {
        *arg2 = i < 1;
    }
    return D_800CCB28[arg0][i];
}

void func_800B3760(void) {
    switch (D_800CE3C8->unk0) {
    case 0:
        func_800B17B4();
        break;
    case 2:
        func_800B1CD0();
        break;
    case 1:
        func_800B20B8();
        break;
    }
    if (D_800CE3C8->unk9 != 0 && (func_800B84C0() & 0x800000)) {
        func_800AE670();
    }
    if (func_800B84C0() & 0x400000) {
        func_800AE828();
    }
}

void func_800B383C(void) {
}

INCLUDE_ASM("asm/us/ovl_0c/nonmatchings/ovl_0c", func_800B3844);

INCLUDE_ASM("asm/us/ovl_0c/nonmatchings/ovl_0c", func_800B3998);

INCLUDE_ASM("asm/us/ovl_0c/nonmatchings/ovl_0c", func_800B3A80);

INCLUDE_ASM("asm/us/ovl_0c/nonmatchings/ovl_0c", func_800B3FEC);

INCLUDE_ASM("asm/us/ovl_0c/nonmatchings/ovl_0c", func_800B4270);

INCLUDE_ASM("asm/us/ovl_0c/nonmatchings/ovl_0c", func_800B4720);

INCLUDE_ASM("asm/us/ovl_0c/nonmatchings/ovl_0c", func_800B4BDC);

INCLUDE_ASM("asm/us/ovl_0c/nonmatchings/ovl_0c", func_800B4FD4);

INCLUDE_ASM("asm/us/ovl_0c/nonmatchings/ovl_0c", func_800B53FC);

INCLUDE_ASM("asm/us/ovl_0c/nonmatchings/ovl_0c", func_800B5880);

INCLUDE_ASM("asm/us/ovl_0c/nonmatchings/ovl_0c", func_800B5CD0);

INCLUDE_ASM("asm/us/ovl_0c/nonmatchings/ovl_0c", func_800B5FD0);

INCLUDE_ASM("asm/us/ovl_0c/nonmatchings/ovl_0c", func_800B6338);

INCLUDE_ASM("asm/us/ovl_0c/nonmatchings/ovl_0c", func_800B6598);

void func_800B6908(void) {
    D_800CF400 = D_800CE400;
    D_800CF408 = 0x80138800;
    D_800CF420[0] = 0x801A2300;
    D_800CF420[1] = 0x801C8100;
    D_800CE3E8[0] = (s32 *)0x80180800;
    D_800CE3E8[1] = (s32 *)0x8018A000;
    D_800CE3E8[2] = (s32 *)0x80193800;
    D_800CE3F8 = 0x80100800;
    D_800CF404 = D_800CF420[*D_800CF45C];
}

void func_800B69AC(void) {
}

INCLUDE_ASM("asm/us/ovl_0c/nonmatchings/ovl_0c", func_800B69B4);

INCLUDE_RODATA("asm/us/ovl_0c/nonmatchings/ovl_0c", D_800A71D8);

INCLUDE_RODATA("asm/us/ovl_0c/nonmatchings/ovl_0c", D_800A71E0);

INCLUDE_ASM("asm/us/ovl_0c/nonmatchings/ovl_0c", func_800B6BC4);

void func_800B7098(void) {
    switch (D_800CF46C) {
    case 0:
        D_8006794C->unk9 = 0;
        func_800B83FC();
        if (func_8004AE00() != NULL) {
            D_800CE3DC = func_8004AE00();
        }
        break;
    case 1:
    case 2:
        if (D_800D3E60 != 2) {
            D_8006794C->unk9 = 3;
            D_8006794C->unk1C->unk8E4.world->unk0 &= ~0x1000;
            SetSpadStack(0x1F8003FC);
            func_800AFCE0();
            ResetSpadStack();
            func_800B83FC();
            SetSpadStack(0x1F8003FC);
            func_800C53DC();
            func_800AD39C();
            func_800BB07C();
            func_800C26EC();
            func_800BEECC();
            func_800B969C();
            ResetSpadStack();
        } else {
            D_8006794C->unk9 = 0;
            D_8006794C->unk1C->unk8E4.world->unk0 |= 0x1000;
            func_800C4850(0x26, 0x14, 0);
            func_800B83FC();
            func_800BEECC();
        }
        break;
    }
    if (D_800CE3DC->unk14.vx != D_800CE3DC->unk40.vx || D_800CE3DC->unk14.vy != D_800CE3DC->unk40.vy ||
        D_800CE3DC->unk14.vz != D_800CE3DC->unk40.vz) {
        D_800CE25E = 1;
    } else {
        D_800CE25E = 0;
    }
    D_800CCF70 = 10;
    func_800C3EAC();
    if ((u8)(D_800D30C5 - 1) < 199) {
        D_800D30C5++;
    }
}

void func_800B72A4(void) {
    u8 mode = D_800CF46C;

    if (mode == 2) {
        if (D_800D3E60 != mode) {
            D_800CCF70 = 0x64;
            func_800639D4(D_8006794C->unk1C->unkA);
            D_800CF468 = VSync(1);
            if (D_800CCE49 < 2 && D_800CE264 != 0) {
                func_800AF08C();
            }
            D_800CF468 = VSync(1) - D_800CF468;
            D_800CCF70 = 0x65;
            D_800CF4A0 = VSync(1);
            func_800BAEC0();
            D_800CF4A0 = VSync(1) - D_800CF4A0;
            D_800CCF70 = 0x66;
            func_800BE984();
            D_800CCF70 = 0x67;
            func_800C4DD8();
            D_800CCF70 = 0x68;
            if (D_800CF44C != 0 && D_800D3E60 != 2) {
                D_800CF404 = func_80050D38(D_800CF404);
            }
        } else {
            D_800CCF70 = 0x6A;
            func_800BE984();
        }
    }
}

INCLUDE_RODATA("asm/us/ovl_0c/nonmatchings/ovl_0c", D_800A71F4);

INCLUDE_RODATA("asm/us/ovl_0c/nonmatchings/ovl_0c", D_800A720C);

INCLUDE_ASM("asm/us/ovl_0c/nonmatchings/ovl_0c", func_800B73F8);

INCLUDE_ASM("asm/us/ovl_0c/nonmatchings/ovl_0c", func_800B7A14);

INCLUDE_ASM("asm/us/ovl_0c/nonmatchings/ovl_0c", func_800B7DAC);

void func_800B7F7C(void) {
    RECT r = D_800A71E0;

    D_8006794C->unk1C->unk0 &= ~0x400000;
    LoadImage(&r, (u32 *)D_800CFFD0);
    DrawSync(0);
    func_800B153C();
    func_800C3EAC();
    while (func_800AB0C0(0xC00, 0, 0, 0, 0) != 0) {
    }
}

void func_800B8024(void) {
    if (D_800CCE50 & 0x40) {
        func_80015C38(D_800A71F4);
    }
    func_800C43F0();
    func_800BC098();
}

void func_800B8068(void) {
    RECT r = D_800A71E0;

    MoveImage(&r, 0x100, 0xFD);
    DrawSync(0);
    if (D_800CCE50 & 0x40) {
        func_80015C38(D_800A720C);
    }
    func_800C6EF8();
    func_800C4654();
    func_800BC0A0();
    func_800B69AC();
    func_800B8ABC();
}

s32 func_800B8108(void) {
    return 0x801EDF00;
}

s32 func_800B8114(void) {
    return D_800CF408;
}

INCLUDE_RODATA("asm/us/ovl_0c/nonmatchings/ovl_0c", D_800A79D0);

INCLUDE_RODATA("asm/us/ovl_0c/nonmatchings/ovl_0c", D_800A7A0C);

INCLUDE_RODATA("asm/us/ovl_0c/nonmatchings/ovl_0c", D_800A7A40);

s32 func_800B8124(void) {
    s32 n = D_800CE3DC->unk11;
    if (n < 3 && n != 0) {
        switch ((u8)D_800CCCC0[n].unk8 >> 2) {
        case 3:
        case 18:
        case 21:
        case 22:
        case 28:
            return 1;
        }
    }
    return 0;
}

void func_800B81A0(void) {
    func_800B30C0();
}

s32 func_800B81C0(s32 arg0, s32 arg1, s32 arg2) {
    return ((arg0 * (0x1000 - arg2)) >> 0xC) + ((arg1 * arg2) >> 0xC);
}

s32 func_800B81F0(s8 arg0, s8 arg1, s32 arg2) {
    return ((((arg0 << 0xA) * (0x1000 - arg2)) >> 0xC) + (((arg1 << 0xA) * arg2) >> 0xC)) >> 0xA;
}

INCLUDE_ASM("asm/us/ovl_0c/nonmatchings/ovl_0c", func_800B822C);

s32 func_800B828C(s32 *arg0) {
    u32 i;

    if (!(*arg0 & 0x80000000)) {
        if (D_800CCE50 & 0x100) {
            func_80015C38(D_800A79D0, arg0, *arg0 & 0x7FFFFFFF);
        }
        *arg0 |= 0x80000000;
        for (i = 0; i < (*arg0 & 0x7FFFFFFF); i++) {
            arg0[i + 1] += (s32)arg0;
        }
    } else if (D_800CCE50 & 0x100) {
        func_80015C38(D_800A7A0C, arg0);
    }
    return *arg0 & 0x7FFFFFFF;
}

void *func_800B8388(void **arg0, s32 arg1) {
    s32 n = (s32)arg0[0] & 0x7FFFFFFF;

    if ((D_800CCE50 & 0x100) && n < arg1) {
        func_80015C38(D_800A7A40, arg1, n);
    }
    return arg0[arg1 + 1];
}

void func_800B83FC(void) {
    u8 m;

    D_800CF4B4 = VSync(1);
    m = D_800CF46C;
    if ((m == 0 || m == 2) && !(D_8006794C->unk1C->unk0 & 2) && (m == 0 || m == 2)) {
        D_800CF49C = func_8004BC38(m);
    }
    D_800CF4B4 = VSync(1) - D_800CF4B4;
}

s32 func_800B8498(void) {
    if (D_800CF46C == 2) {
        return D_800739A0[0];
    }
    return 0;
}

s32 func_800B84C0(void) {
    if (D_800CF46C == 2) {
        return D_80073990;
    }
    return 0;
}

void func_800B84E8(void) {
    func_800BAD98();
    func_800B8724();
    func_800B8B10();
    func_800AEC74();
    func_800AE75C();
    func_800C3E28();
    func_800C464C();
    func_800AEF10();
    D_800CF46C = 1;
}

s32 func_800B8548(void) {
    return (u32)(D_800D3F98 - 2) < 2U;
}

void func_800B8560(void) {
}

s32 func_800B8568(void) {
    return 0x801EDF00;
}

void func_800B8574(void) {
    D_800CF540.m[0][0] = 0;
    D_800CF540.m[0][1] = -0x1000;
    D_800CF540.m[0][2] = 0;
    D_800CF540.m[1][0] = 0x1000;
    D_800CF540.m[1][1] = 0;
    D_800CF540.m[1][2] = 0;
    D_800CF540.m[2][0] = 0;
    D_800CF540.m[2][1] = -0x400;
    D_800CF540.m[2][2] = 0x1000;
    D_800CF520.m[0][0] = 0;
    D_800CF520.m[0][1] = 0x1000;
    D_800CF520.m[0][2] = 0;
    D_800CF520.m[1][0] = 0x1000;
    D_800CF520.m[1][1] = 0;
    D_800CF520.m[1][2] = 0;
    D_800CF520.m[2][0] = 0;
    D_800CF520.m[2][1] = 0;
    D_800CF520.m[2][2] = 0x1000;
    D_800CF4C0.m[0][0] = 0;
    D_800CF4C0.m[0][1] = 0x1000;
    D_800CF4C0.m[0][2] = 0;
    D_800CF4C0.m[1][0] = 0x1000;
    D_800CF4C0.m[1][1] = 0;
    D_800CF4C0.m[1][2] = 0;
    D_800CF4C0.m[2][0] = 0;
    D_800CF4C0.m[2][1] = 0;
    D_800CF4C0.m[2][2] = 0x1000;
    SetBackColor(0, 0, 0);
    SetFarColor(0, 0, 0);
    SetFogNearFar(5000, 30000, D_8006794C->unk1C->unkA);
    func_800B8664();
}

void func_800B8664(void) {
    D_800CF560.m[0][0] = 0;
    D_800CF560.m[1][0] = 0;
    D_800CF560.m[2][0] = 0;
    D_800CF500.m[0][0] = D_800D3FA0.unk5E0[0].vx;
    D_800CF500.m[1][0] = D_800D3FA0.unk5E0[0].vy;
    D_800CF500.m[2][0] = D_800D3FA0.unk5E0[0].vz;
    D_800CF500.m[0][1] = D_800D3FA0.unk5E0[1].vx;
    D_800CF500.m[1][1] = D_800D3FA0.unk5E0[1].vy;
    D_800CF500.m[2][1] = D_800D3FA0.unk5E0[1].vz;
    D_800CF500.m[0][2] = D_800D3FA0.unk5E0[2].vx;
    D_800CF500.m[1][2] = D_800D3FA0.unk5E0[2].vy;
    D_800CF500.m[2][2] = D_800D3FA0.unk5E0[2].vz;
    D_800CF4E0.m[0][0] = D_800D3FA0.unk5F8.vx;
    D_800CF4E0.m[0][1] = D_800D3FA0.unk5F8.vx;
    D_800CF4E0.m[1][0] = D_800D3FA0.unk5F8.vy;
    D_800CF4E0.m[2][0] = D_800D3FA0.unk5F8.vz;
    D_800CF4E0.m[1][1] = D_800D3FA0.unk5F8.vy;
    D_800CF4E0.m[2][1] = D_800D3FA0.unk5F8.vz;
    D_800CF4E0.m[0][2] = D_800D3FA0.unk5F8.vx;
    D_800CF4E0.m[1][2] = D_800D3FA0.unk5F8.vy;
    D_800CF4E0.m[2][2] = D_800D3FA0.unk5F8.vz;
    D_800CF560.m[0][1] = 0;
    D_800CF560.m[1][1] = 0;
    D_800CF560.m[2][1] = 0;
    D_800CF560.m[0][2] = 0;
    D_800CF560.m[1][2] = 0;
    D_800CF560.m[2][2] = 0;
}

void func_800B8724(void) {
}

INCLUDE_RODATA("asm/us/ovl_0c/nonmatchings/ovl_0c", D_800A7AE8);

void func_800B872C(void) {
    s32 i;

    if (D_800CF444 == 1) {
        for (i = 0; i < 5; i++) {
            D_800CF580[i] = D_800CCF84[i];
        }
        func_80015C38("Read CD Image [Disc-1]\n");
        D_800CF598[0] = func_8001E218(0xC, 0x3F3)->unk4 + D_8006794C->unk20;
        D_800CF598[3] = func_8001E218(0xC, 0x3FD)->unk4 + D_8006794C->unk20;
        D_800CF598[4] = func_8001E218(0xC, 0x407)->unk4 + D_8006794C->unk20;
    } else {
        for (i = 0; i < 5; i++) {
            D_800CF580[i] = D_800CCF98[i];
        }
        func_80015C38("Read CD Image [Disc-4]\n");
        D_800CF598[0] = func_8001E218(0xC, 0x3F6)->unk4 + D_8006794C->unk20;
        D_800CF598[3] = func_8001E218(0xC, 0x400)->unk4 + D_8006794C->unk20;
        D_800CF598[4] = func_8001E218(0xC, 0x40A)->unk4 + D_8006794C->unk20;
    }
    if (D_800CCE50 & 0x40) {
        func_80015C38(D_800A7BB8, D_800CF598[0]);
    }
}

INCLUDE_RODATA("asm/us/ovl_0c/nonmatchings/ovl_0c", D_800A7BB8);

s32 func_800B88C0(s32 arg0) {
    s32 *p = &D_800CFDB0[D_800CFE14++];

    if (D_800CFE14 >= 0x19U) {
        func_80015C38("w_memorymanHeapAlloc:(Error) Heap stack overflow\n");
    } else if (D_800CCE50 & 0x40) {
        func_80015C38("w_memorymanHeapAlloc: [%d]  =  %x - ", D_800CFE14 - 1, D_800CFE18);
    }
    *p = D_800CFE18;
    D_800CFE1C = arg0;
    D_800CFE18 += arg0;
    D_800CFE10 += arg0;
    if (D_800CCE50 & 0x40) {
        func_80015C38("%x (%x/%x)[%x]\n", D_800CFE18, D_800CFE10, 0x24800, arg0);
    }
    return *p;
}

INCLUDE_ASM("asm/us/ovl_0c/nonmatchings/ovl_0c", func_800B89B0);

void func_800B8A80(void) {
    D_800CFE10 = 0;
    D_800CFE14 = 0;
    D_800CFE1C = 0;
    D_800CFE18 = D_80067948;
    D_80067948 += 0x24800;
}

void func_800B8ABC(void) {
    if (D_800CFE14 != 0) {
        func_80015C38("w_memorymanDestructor:(Error) Memory leak can be found.\n");
    }
    if (D_800CCE50 & 0x40) {
        func_80015C38(D_800A7CCC);
    }
}

INCLUDE_ASM("asm/us/ovl_0c/nonmatchings/ovl_0c", func_800B8B10);

INCLUDE_ASM("asm/us/ovl_0c/nonmatchings/ovl_0c", func_800B8DC0);

void func_800B905C(void) {
    s32 i;
    s32 x, y, w, h;
    s32 t;
    s32 n;
    RECT *r;

    for (i = 0; i < 8; i++) {
        x = D_800CFEA0[i].unk0;
        y = D_800CFEA0[i].unk2;
        w = D_800CFEA0[i].unk4;
        h = D_800CFEA0[i].unk6;
        t = D_800CFE88[i] + D_800CFEA0[i].unk8;
        n = (s16)t >> 8;
        D_800CFE88[i] = t & 0xFF;
        if (n != 0) {
            r = (RECT *)D_800CF404;
            r->x = x;
            r->y = y;
            r->w = w;
            r->h = n;
            MoveImage(r, 0x280, 0xF8);
            r++;
            r->y = y + n;
            r->x = x;
            r->w = w;
            r->h = h - n;
            MoveImage(r, x, y);
            r++;
            r->x = 0x280;
            r->y = 0xF8;
            r->w = w;
            r->h = n;
            MoveImage(r, x, y + h - n);
            r++;
            D_800CF404 = (s32)r;
        }
    }
}

void func_800B91A0(void) {
    s32 t;

    t = D_800CFEF8;
    if (t != 0 && D_800CF4B0 % t == 0) {
        func_800B8DC0(D_800CE3F4, D_800CCFAC[D_800CFE98[0]] + 8, 0, 0, 0, 0, 0);
        func_800B8DC0(D_800CE3F4, D_800CCFAC[D_800CFE98[0]], 0, 0, 0, 0, 0);
        D_800CFE98[0] = (s8)(D_800CFE98[0] + 1) % 6;
    }
    t = D_800CFF02;
    if (t != 0 && (D_800CF4B0 + 2) % t == 0) {
        func_800B8DC0(D_800CE3F4, D_800CCFAC[D_800CFE98[1]] + 4, 0, 0, 0, 0, 0);
        func_800B8DC0(D_800CE3F4, D_800CCFAC[D_800CFE98[1]] + 12, 0, 0, 0, 0, 0);
        D_800CFE98[1] = (s8)(D_800CFE98[1] + 1) % 6;
    }
}

void func_800B93B8(void) {
    s8 a = 0;
    s8 b = -1;
    s8 c = 0;
    s8 d = -1;
    u8 on = 0;
    s32 e = 0;
    WmLNode *n;

    if (D_800D1FD0 != 0) {
        if (D_800D30CB == 0) {
            a = 0xC;
            b = 0xD;
            c = 0xF;
            d = 0x10;
        } else {
            a = 0xE;
            b = 0xE;
            c = 0x11;
            d = 0x11;
        }
    }
    n = D_800D3F38.unk4;
    if ((n->unk8 & 0x3F) == 4) {
        on = 1;
        if (n->unkA == 2) {
            e = 8;
        }
    }
    n = n->unk4;
    if ((n->unk8 & 0x3F) == 4) {
        on = 1;
        if (n->unkA == 2) {
            e = 8;
        }
    }
    func_800B94F8(0, 0xB, 0);
    func_800B94F8(a, b, 0);
    if (on) {
        func_800B94F8(c, d, e);
    }
}

INCLUDE_ASM("asm/us/ovl_0c/nonmatchings/ovl_0c", func_800B94F8);

void func_800B969C(void) {
    func_800B905C();
    func_800B91A0();
    func_800B93B8();
}

void func_800B96CC(s32 *arg0, s32 arg1, s32 arg2, s32 arg3, s32 arg4) {
    s32 n = *arg0 & 0x7FFFFFFF;
    s32 i;

    for (i = 0; i < n; i++) {
        func_800B8DC0(arg0, i, arg1, arg2, arg3, arg4, 0);
    }
}

void func_800B9774(void) {
    D_800D1FE0[0].unk0.r = D_800D3FA0.unk610[1].unk4[0];
    D_800D1FE0[0].unk0.g = D_800D3FA0.unk610[1].unk4[1];
    D_800D1FE0[0].unk0.b = D_800D3FA0.unk610[1].unk4[2];
    D_800D1FE0[0].unk4 = D_800D3FA0.unk610[1].unk0;
    D_800D1FE0[0].unk6 = D_800D3FA0.unk610[1].unk2;
    D_800D1FE0[1].unk0.r = D_800D3FA0.unk610[0].unk4[0];
    D_800D1FE0[1].unk0.g = D_800D3FA0.unk610[0].unk4[1];
    D_800D1FE0[1].unk0.b = D_800D3FA0.unk610[0].unk4[2];
    D_800D1FE0[1].unk4 = D_800D3FA0.unk610[0].unk0;
    D_800D1FE0[1].unk6 = D_800D3FA0.unk610[0].unk2;
    D_800D1FD8.unk0.r = D_800D3FA0.unk610[2].unk4[0];
    D_800D1FD8.unk0.g = D_800D3FA0.unk610[2].unk4[1];
    D_800D1FD8.unk0.b = D_800D3FA0.unk610[2].unk4[2];
    D_800D1FD8.unk4 = D_800D3FA0.unk610[2].unk0;
    D_800D1FD8.unk6 = D_800D3FA0.unk610[2].unk2;
}

INCLUDE_RODATA("asm/us/ovl_0c/nonmatchings/ovl_0c", D_800A7CCC);

INCLUDE_ASM("asm/us/ovl_0c/nonmatchings/ovl_0c", func_800B9808);

INCLUDE_ASM("asm/us/ovl_0c/nonmatchings/ovl_0c", func_800BA1E4);

void func_800BA29C(void) {
    func_800BA1E4();
    D_800D30C6 = 1;
    D_800D30C5 = 0;
    D_800D30C4 = 0;
}

void func_800BA2D4(void) {
}

void func_800BA2DC(u8 *arg0, s32 arg1, s32 arg2) {
    s32 t;
    s32 unused[2];

    t = D_800CF460[*D_800CF45C];
    D_800D27F0 = &D_800D2FF8[arg0[1]];
    if (D_800CCE4B != 0) {
        func_800B4270(arg0);
    } else if (arg2 == 0) {
        func_800B6338(arg0);
    } else {
        func_800B53FC(arg0);
    }
    if (D_800CCE4B != 0) {
        D_800CF404 = func_800B4FD4(arg0, D_800CF404, t);
    } else if (arg2 == 0) {
        D_800CF404 = func_800B6598(arg0, D_800CF404, t);
    } else {
        D_800CF404 = func_800B5880(arg0, D_800CF404, t);
    }
}

INCLUDE_ASM("asm/us/ovl_0c/nonmatchings/ovl_0c", func_800BA3F4);

void func_800BA458(s32 arg0, s32 arg1, u32 arg2) {
    s32 p = D_800CF460[*D_800CF45C] + arg1 * 4;

    if (arg2 < 2) {
        func_800B4720(arg0);
    }
    if (arg2 == 2) {
        func_800B4BDC(arg0);
    }
    if (arg2 == 0 || arg2 == 2) {
        D_800CF404 = func_800B5CD0(arg0, D_800CF404, p);
    }
    if (arg2 == 1) {
        D_800CF404 = func_800B5FD0(arg0, D_800CF404, p);
    }
}

void func_800BA528(WmPack *arg0) {
    s32 i;
    WmPackEntry *e;

    i = 0;
    arg0->unk4 += (s32)arg0;
    for (; i < arg0->unk0; i++) {
        e = (WmPackEntry *)((s32)arg0->unk8[i] + (s32)arg0);
        arg0->unk8[i] = e;
        e->unk4 += (s32)arg0;
        e->unk8 += (s32)arg0;
    }
}

INCLUDE_ASM("asm/us/ovl_0c/nonmatchings/ovl_0c", func_800BA58C);

void func_800BAD98(void) {
    s32 i;
    s32 j;
    u16 (*row)[12];
    s32 mask;
    Unk800CF450 *tbl;
    u16 v;

    i = 0;
    tbl = D_800CF450;
    row = D_800D32E4;
    mask = D_8007AEF0.b[0xC2] | (D_8007AEF0.b[0xC6] << 8);
    do {
        if ((mask >> i) & 1) {
            j = 0;
            do {
                v = (*row)[j];
                if (v != 0xFFFF) {
                    tbl[v - 1].unk6 = tbl[v - 1].unk4;
                }
                j++;
            } while (j < 12);
        }
        i++;
        row++;
    } while (i < 9);
    for (i = 0; i < 3; i++) {
        D_800D32F8[i] = 0xFFFF;
        D_800D330C[i] = 0;
    }
    if (*D_800CF430 - 0x1B4EU < 0x1004) {
        D_800D5410 = 0;
    } else {
        D_800D5410 = 1;
    }
    func_800BD674();
    func_800BD57C();
}

INCLUDE_RODATA("asm/us/ovl_0c/nonmatchings/ovl_0c", D_800A7D20);

void func_800BAEC0(void) {
    Unk800B8388 *p;
    MATRIX *o;

    SetBackColor(D_800D3FA0.unk600, D_800D3FA0.unk602, D_800D3FA0.unk604);
    SetLightMatrix(&D_800CF540);
    SetColorMatrix(&D_800CF500);
    func_800BD5A8();
    func_800BD78C();
    SetBackColor(D_800D3FA0.unk608, D_800D3FA0.unk60A, D_800D3FA0.unk60C);
    if (D_800CF46D != 0) {
        MATRIX m1 = { { { 0x1000, 0, 0 }, { 0, 0x1000, 0 }, { 0, 0, 0x1000 } }, { 0, 0xE10, 0 } };
        MATRIX out;
        MATRIX m2 = *D_800CF448;

        if (D_800CCE4B != 0) {
            m1.t[1] = -D_800CE19C - 0x9C4;
        } else {
            m1.t[1] = -D_800CE19C - 0x258;
        }
        SetLightMatrix(&D_800CF520);
        SetColorMatrix(&D_800CF4E0);
        p = func_800B8388(D_800CF428, 0x3B);
        m2.t[0] = 0;
        m2.t[1] = 0;
        m2.t[2] = 0;
        o = &out;
        CompMatrix(&m1, &m2, o);
        SetRotMatrix(o);
        SetTransMatrix(o);
        func_800BA3F4(p->unk8, 0);
    }
}

INCLUDE_ASM("asm/us/ovl_0c/nonmatchings/ovl_0c", func_800BB07C);

INCLUDE_ASM("asm/us/ovl_0c/nonmatchings/ovl_0c", func_800BB508);

INCLUDE_ASM("asm/us/ovl_0c/nonmatchings/ovl_0c", func_800BBBA8);

INCLUDE_ASM("asm/us/ovl_0c/nonmatchings/ovl_0c", func_800BBE24);

void func_800BC098(void) {
}

void func_800BC0A0(void) {
}

void func_800BC0A8(s32 *arg0, s32 *arg1) {
    if (*arg0 < 0) {
        *arg0 += D_800D32D8;
    }
    if (*arg0 >= D_800D32D8) {
        *arg0 -= D_800D32D8;
    }
    if (*arg1 > 0) {
        *arg1 -= D_800D32DC;
    }
    if (*arg1 <= -D_800D32DC) {
        *arg1 += D_800D32DC;
    }
}

void func_800BC130(s32 arg0, s32 arg1, s32 *arg2) {
    s32 x;
    s32 z;

    func_800BC0A8_inline(&arg0, &arg1);
    x = arg0 / 0x2000;
    z = arg1 / 0x2000;
    *arg2 = -z * D_800D32F0 + x;
}

void func_800BC1FC(s32 arg0, s32 arg1, s32 *arg2) {
    s32 x;
    s32 z;

    func_800BC0A8_inline(&arg0, &arg1);
    x = arg0 / 0x4000;
    z = arg1 / 0x4000;
    *arg2 = -z * D_800D3304 + x;
}

void func_800BC2C8(s32 arg0, s32 *arg1, s32 *arg2) {
    *arg1 = (arg0 % D_800D32F0) << 13;
    *arg2 = -((arg0 / D_800D32F0) << 13);
}

void func_800BC354(s32 arg0, s32 *arg1) {
    *arg1 = arg0 / D_800D32F0 / 2 * D_800D3304 + arg0 % D_800D32F0 / 2;
}

void func_800BC3C4(s32 arg0, s32 *arg1, s32 *arg2) {
    *arg1 = (arg0 % D_800D3304) << 14;
    *arg2 = -((arg0 / D_800D3304) << 14);
}

INCLUDE_ASM("asm/us/ovl_0c/nonmatchings/ovl_0c", func_800BC450);

INCLUDE_ASM("asm/us/ovl_0c/nonmatchings/ovl_0c", func_800BC490);

u8 func_800BC528(s32 arg0) {
    return D_800CCFB4[arg0];
}

INCLUDE_ASM("asm/us/ovl_0c/nonmatchings/ovl_0c", func_800BC540);

void func_800BC690(void) {
    s32 t;
    s32 a;
    s32 b;

    if (D_800CE3C4 < 10) {
        if (D_800CE3C4 >= 8) {
            t = D_800CE3C0;
            if (t >= 0x200) {
                t = 0x1FF;
            }
            if (t < 0x100) {
                t = 0x100;
            }
            D_800D3730[0] = D_800CE198[0] + ((D_800CE1D0[0] * D_800CE3C8->unkA * t) >> 9);
            D_800D3730[2] = D_800CE198[2] + ((D_800CE1D0[2] * D_800CE3C8->unkA * t) >> 9);
        } else {
            D_800D3730[0] = D_800CE198[0] + D_800CE1D0[0] * D_800CE3C8->unkA;
            D_800D3730[2] = D_800CE198[2] + D_800CE1D0[2] * D_800CE3C8->unkA;
        }
    } else {
        D_800D3730[0] = D_800CE198[0] + D_800CE1D0[0] * D_800CE3C8->unkA;
        D_800D3730[2] = D_800CE198[2] + D_800CE1D0[2] * D_800CE3C8->unkA;
    }
    func_800BC0A8(&D_800D3730[0], &D_800D3730[2]);
    func_800BC130(D_800D3730[0] - 0x6000, D_800D3730[2] + 0x6000, &a);
    func_800BC354(a, &b);
    if (b != D_800D3408) {
        func_800BC84C(b);
    }
    func_800BCBDC();
    D_800D3408 = b;
}

INCLUDE_ASM("asm/us/ovl_0c/nonmatchings/ovl_0c", func_800BC84C);

INCLUDE_ASM("asm/us/ovl_0c/nonmatchings/ovl_0c", func_800BCA6C);

INCLUDE_ASM("asm/us/ovl_0c/nonmatchings/ovl_0c", func_800BCBDC);

INCLUDE_ASM("asm/us/ovl_0c/nonmatchings/ovl_0c", func_800BD150);

void func_800BD3F8(void) {
    s32 i;
    u8 st;

    for (i = 0; i < D_800D3320[D_800CD094]; i++) {
        st = D_800D33F8[i]->unkE;
        if (st == 1) {
            D_800D33F8[i]->unkE = 2;
            if (D_800D3740[i] != NULL) {
                func_800BA528(D_800D3740[i]);
            }
            if (D_800D3310[i] != NULL) {
                func_800C20B0(D_800D3310[i]);
            }
            if (D_800D33F4 == 0) {
                if (++D_800D3750 == 16) {
                    D_800D30C5 = st;
                    D_800CE202 = 0;
                    D_800D33F4 = st;
                    D_800CF46C = 2;
                    D_80073998[0] &= 0xFFF4FFFF;
                    func_800B30C0();
                }
            }
        }
    }
    D_800CF454 = 0;
}

void func_800BD57C(void) {
    func_800C1C98();
    func_800C2134();
    D_800D3338 = 0;
}

void func_800BD5A8(void) {
    func_800C2048();
    func_800C26E4();
}

void func_800BD5D0(void) {
    Unk800D335C *a;
    Unk800D335C *b;
    u32 i;

    if (D_800D33F0 != 0) {
        b = D_800D3358.unk4;
        D_800D33F0 = 0;
        a = b;
        for (i = 0; i < 16; i++) {
            b = b->unk4;
        }
        while (1) {
            if (b == &D_800D3340) {
                break;
            }
            while (a->unk8 != 0) {
                a = a->unk4;
            }
            a->unk8 = b->unk8;
            b->unk8 = 0;
            b->unkC = -1;
            b->unkE = 0;
            b = b->unk4;
        }
    }
}

void func_800BD66C(void) {
}

INCLUDE_ASM("asm/us/ovl_0c/nonmatchings/ovl_0c", func_800BD674);

INCLUDE_ASM("asm/us/ovl_0c/nonmatchings/ovl_0c", func_800BD78C);

INCLUDE_ASM("asm/us/ovl_0c/nonmatchings/ovl_0c", func_800BDAB0);

s32 func_800BE138(s16 arg0) {
    Unk800D375C *n = D_800D3758.unk4;
    u32 i = 0;

    do {
        if (n->unk10 == -1) {
            if (i >= 20) {
                n->unk10 = arg0;
                n->unk0->unk4 = n->unk4;
                n->unk4->unk0 = n->unk0;
                n->unk4 = D_800D3758.unk4;
                n->unk0 = &D_800D3758;
                D_800D3790++;
                D_800D3758.unk4->unk0 = n;
                D_800D3758.unk4 = n;
            } else {
                n->unk10 = arg0;
                n->unk12 = 0;
            }
            break;
        }
        if (n->unk10 == arg0) {
            n->unk0->unk4 = n->unk4;
            n->unk4->unk0 = n->unk0;
            n->unk4 = D_800D3758.unk4;
            n->unk0 = &D_800D3758;
            D_800D3758.unk4->unk0 = n;
            D_800D3758.unk4 = n;
            break;
        }
        n = n->unk4;
        i++;
    } while (i < 40);
    return i != 40;
}

INCLUDE_ASM("asm/us/ovl_0c/nonmatchings/ovl_0c", func_800BE238);

s32 func_800BE4A8(s16 arg0, u8 *arg1) {
    s32 id;
    s32 i;
    Unk800D335C *p;
    s16 m;
    s32 k;

    p = D_800D3358.unk4;
    func_800BC354(arg0, &id);
    for (i = 0; i < 16; i++, p = p->unk4) {
        if (id == p->unkC && p->unkE == 2) {
            if (arg1 != NULL) {
                *arg1 = 0;
            }
            m = arg0 % 2;
            k = m + (arg0 / D_800D32F0) % 2 * 2;
            if (p->unk10 != 0) {
                return p->unk8->unk8[k];
            }
            return D_800D32EC->unk8[k];
        }
    }
    if (arg1 != NULL) {
        *arg1 = 1;
    }
    m = arg0 % 2;
    k = m + (arg0 / D_800D32F0) % 2 * 2;
    return D_800D32EC->unk8[k];
}

void func_800BE674(void) {
    D_800D3770.m[2][2] = 0x1000;
    D_800D3770.m[1][1] = 0x1000;
    D_800D3770.m[0][0] = 0x1000;
    D_800D3770.t[2] = 0;
    D_800D3770.t[1] = 0;
    D_800D3770.t[0] = 0;
    D_800D3770.m[2][1] = 0;
    D_800D3770.m[2][0] = 0;
    D_800D3770.m[1][2] = 0;
    D_800D3770.m[1][0] = 0;
    D_800D3770.m[0][2] = 0;
    D_800D3770.m[0][1] = 0;
}

void func_800BE6B4(void) {
    u32 i;
    Unk800D375C *a;
    Unk800D375C *b;

    if (D_800D3790 != 0) {
        a = D_800D3758.unk4;
        D_800D3790 = 0;
        b = a;
        for (i = 0; i < 20; i++) {
            b = b->unk4;
        }
        for (i = 0; i < 20; i++) {
            if (a->unkC == 0) {
                a->unk8 = b->unk8;
                a->unkC = b->unkC;
                b->unk8 = 0;
                b->unkC = 0;
                b->unk10 = -1;
                b->unk12 = 0;
                b = b->unk4;
            }
            a = a->unk4;
        }
    }
}

s32 func_800BE748(VECTOR *arg0, u32 *arg1, s32 *arg2, s32 arg3, u16 *arg4) {
    VECTOR pos;
    VECTOR rel;
    VECTOR o;
    s32 cell;
    u8 flag;
    s32 r;

    pos = *arg0;
    func_800BC0A8(&pos.vx, &pos.vz);
    func_800BC130(pos.vx, pos.vz, &cell);
    r = func_800BE4A8(cell, &flag);
    if (flag == 0 && r != 0) {
        func_800BC2C8(cell, &o.vx, &o.vz);
        rel.vx = pos.vx - o.vx;
        rel.vy = pos.vy;
        rel.vz = pos.vz - o.vz;
        /* callers pass a 16-bit variable but a word is stored */
        *(s32 *)arg4 = func_800B9808(cell, r, &rel, arg1, arg2, arg3);
        if ((u8)D_800CE3C4 - 2U < 2) {
            if (*arg1 - 0x38 < 2) {
                *arg1 = 0x36;
            }
        }
        return 1;
    }
    *(s32 *)arg4 = pos.vy;
    return 0;
}

INCLUDE_RODATA("asm/us/ovl_0c/nonmatchings/ovl_0c", D_800A7F48);

INCLUDE_RODATA("asm/us/ovl_0c/nonmatchings/ovl_0c", D_800A7F94);

INCLUDE_RODATA("asm/us/ovl_0c/nonmatchings/ovl_0c", D_800A7FB8);

void func_800BE894(void) {
    s32 i;
    u8 *b;

    D_800D3E78 = 1;
    D_800D3E80 = 1;
    D_800D3E84 = -1;
    D_800D3E76 = 0;
    D_800D3E6C = 0x14;
    D_800D3E89 = 0x3F;
    D_800D3E79 = 0;
    D_800D3E88 = 3;
    D_800D3E77 = 1;
    D_800D3E64 = 1;
    D_800D3E60 = (b = D_8007AEF0.b)[0x64];
    i = 0x2E;
    if (*D_800CF430 >= 0x1766) {
        D_8007AEF0.h[i] |= 0x7C0;
        if (D_8007AEF0.h[i] & 0xC000) {
            D_8007AEF0.h[i] |= 0xC000;
        }
    }
    if (D_800CCE50 & 0x80) {
        func_80015C38("DEF SET NAVIMODE :%d\n", D_800D3E60);
    }
}

INCLUDE_ASM("asm/us/ovl_0c/nonmatchings/ovl_0c", func_800BE984);

INCLUDE_ASM("asm/us/ovl_0c/nonmatchings/ovl_0c", func_800BEECC);

INCLUDE_ASM("asm/us/ovl_0c/nonmatchings/ovl_0c", func_800BF544);

INCLUDE_ASM("asm/us/ovl_0c/nonmatchings/ovl_0c", func_800BF978);

INCLUDE_ASM("asm/us/ovl_0c/nonmatchings/ovl_0c", func_800C0888);

INCLUDE_ASM("asm/us/ovl_0c/nonmatchings/ovl_0c", func_800C0F34);

void func_800C1428(void) {
    CVECTOR c0;
    s32 i;
    u8 f;

    c0 = func_800C1428_color();
    {
        CVECTOR c1 = func_800C1428_color();

        for (i = 0; i < 64; i++) {
            if ((D_800CD10C[D_800D3E74][i].unk0 | D_800CD10C[D_800D3E74][i].unk2) != 0) {
                f = 0;
                if (i >= 7 && (i < 11 || i == 15)) {
                    f = 1;
                }
                if ((D_800D3E74 == 0 || !f) && (u8)func_800C1604(i)) {
                    func_800BF544(D_800CD10C[D_800D3E74][i].unk0, D_800CD10C[D_800D3E74][i].unk2, 1, 1, c0, c1, 16, 40, 9, 1);
                }
            }
        }
    }
}

s32 func_800C1604(u32 arg0) {
    u32 bit = 0;
    u16 mask = 0;

    if (arg0 < 0x10) {
        mask = D_8007AEF0.h[0x2E];
        bit = arg0;
    }
    if (arg0 - 0x10 < 0x10) {
        mask = D_8007AEF0.h[0x2F];
        bit = arg0 - 0x10;
    }
    if (arg0 - 0x20 < 0x10) {
        mask = D_8007AEF0.h[0x30];
        bit = arg0 - 0x20;
    }
    if (arg0 - 0x30 < 0x10) {
        mask = D_8007AEF0.h[0x31];
        bit = arg0 - 0x30;
    }
    if ((D_800CD10C[D_800D3E74][arg0].unk0 | D_800CD10C[D_800D3E74][arg0].unk2) == 0) {
        return 0;
    }
    return (mask >> bit) & 1;
}

void func_800C16C0(void) {
    u32 *ot;

    ot = (u32 *)D_800CF460[*D_800CF45C];
    ((POLY_FT4 *)D_800CF404)->y0 = 0x60;
    ((POLY_FT4 *)D_800CF404)->y1 = 0x60;
    ((POLY_FT4 *)D_800CF404)->x0 = 0xF9;
    ((POLY_FT4 *)D_800CF404)->x1 = 0x123;
    ((POLY_FT4 *)D_800CF404)->y2 = 0x7C;
    ((POLY_FT4 *)D_800CF404)->x2 = 0xF9;
    ((POLY_FT4 *)D_800CF404)->x3 = 0x123;
    ((POLY_FT4 *)D_800CF404)->y3 = 0x7C;
    ((POLY_FT4 *)D_800CF404)->u0 = 0x30;
    ((POLY_FT4 *)D_800CF404)->v0 = 0xB0;
    ((POLY_FT4 *)D_800CF404)->u1 = 0x5A;
    ((POLY_FT4 *)D_800CF404)->v1 = 0xB0;
    ((POLY_FT4 *)D_800CF404)->u2 = 0x30;
    ((POLY_FT4 *)D_800CF404)->v2 = 0xCC;
    ((POLY_FT4 *)D_800CF404)->u3 = 0x5A;
    ((POLY_FT4 *)D_800CF404)->v3 = 0xCC;
    ((POLY_FT4 *)D_800CF404)->r0 = 0x80;
    ((POLY_FT4 *)D_800CF404)->g0 = 0x80;
    ((POLY_FT4 *)D_800CF404)->b0 = 0x80;
    ((POLY_FT4 *)D_800CF404)->tpage = 0x8C;
    ((POLY_FT4 *)D_800CF404)->clut = 0x3DA8;
    setlen(D_800CF404, 9);
    ((POLY_FT4 *)D_800CF404)->code = 0x2C;
    addPrim(&ot[10], D_800CF404);
    D_800CF404 += sizeof(POLY_FT4);
    func_800C1824(0xF5, 0x5C, 9, 0);
}

INCLUDE_ASM("asm/us/ovl_0c/nonmatchings/ovl_0c", func_800C1824);

void func_800C1B00(s32 arg0, s32 arg1, s32 *arg2, s32 *arg3, u8 arg4) {
    switch (arg4) {
    case 0:
        *arg2 = ((arg0 - 0x30000) * 14 / 16 >> 11) + 15;
        *arg3 = ((-0x22000 - arg1) * 14 / 16 >> 11) + 6;
        break;
    case 1:
        *arg2 = (arg0 - 0x30000) / 819 * 14 / 16 + 31;
        *arg3 = (-0x22000 - arg1) / 819 * 14 / 16 + 12;
        break;
    case 2:
        *arg2 = arg0 >> 12;
        *arg3 = -arg1 >> 12;
        break;
    case 3:
        *arg2 = arg0 / 1638;
        *arg3 = -arg1 / 1638;
        break;
    }
}

INCLUDE_ASM("asm/us/ovl_0c/nonmatchings/ovl_0c", func_800C1C98);

INCLUDE_RODATA("asm/us/ovl_0c/nonmatchings/ovl_0c", D_800A7FF0);

INCLUDE_RODATA("asm/us/ovl_0c/nonmatchings/ovl_0c", D_800A7FF4);

INCLUDE_RODATA("asm/us/ovl_0c/nonmatchings/ovl_0c", D_800A7FF8);

INCLUDE_RODATA("asm/us/ovl_0c/nonmatchings/ovl_0c", D_800A7FFC);

INCLUDE_RODATA("asm/us/ovl_0c/nonmatchings/ovl_0c", D_800A8000);

INCLUDE_RODATA("asm/us/ovl_0c/nonmatchings/ovl_0c", D_800A8020);

s32 func_800C1DA4(s16 arg0) {
    Unk800D3EAC *p;
    u32 i;

    p = D_800D3EA8.unk4;
    i = 0;
    while (1) {
        if (p->unkC == arg0) {
            p->unk0->unk4 = p->unk4;
            p->unk4->unk0 = p->unk0;
            p->unk4 = D_800D3EA8.unk4;
            p->unk0 = &D_800D3EA8;
            D_800D3EA8.unk4->unk0 = p;
            D_800D3EA8.unk4 = p;
            break;
        }
        if (p->unkC == -1) {
            D_800D3E90++;
            if (D_800D3E90 >= 4) {
                sprintf(D_800CCE6C, "w_readtxRequest/reqover[%d/%d]\n", D_800D3E90, 3);
                func_80015C38("==== ERROR ===== / %s", D_800CCE6C);
                D_800CCE69 = 1;
            }
            p->unkC = arg0;
            p->unk0->unk4 = p->unk4;
            p->unk4->unk0 = p->unk0;
            p->unk4 = D_800D3EA8.unk4;
            p->unk0 = &D_800D3EA8;
            D_800D3EB8++;
            D_800D3EA8.unk4->unk0 = p;
            D_800D3EA8.unk4 = p;
            break;
        }
        p = p->unk4;
        i++;
        if (i >= 6) {
            break;
        }
    }
    if (i == 6) {
        func_80015C38("-- w_readtxRequest Error --\n");
        return 0;
    }
    return 1;
}

void func_800C1F3C(void) {
    Unk800D3EAC *p;
    Unk800D3EAC *q;
    u32 i;

    D_800D3E90 = 0;
    if (D_800D3EB8 != 0) {
        p = D_800D3EA8.unk4;
        i = 0;
        D_800D3EB8 = 0;
        q = p;
        do {
            p = p->unk4;
            i++;
        } while (i < 3);
        for (;;) {
            while ((p != &D_800D3E98) & (p->unk8 == NULL)) {
                if ((p->unk8 == NULL) & (p->unkC != -1)) {
                    p->unkC = -1;
                }
                p = p->unk4;
            }
            if (p == &D_800D3E98) {
                break;
            }
            while (q->unk8 != NULL) {
                q = q->unk4;
            }
            q->unk8 = p->unk8;
            p->unk8 = NULL;
            p->unkE = 0;
            p->unkC = -1;
            p = p->unk4;
        }
    }
}

void func_800C2040(void) {
}

void func_800C2048(void) {
}

INCLUDE_ASM("asm/us/ovl_0c/nonmatchings/ovl_0c", func_800C2050);

void func_800C20B0(s32 *arg0) {
    Unk800D3EAC *p;
    s32 i;

    if (D_800CCE50 & 0x200) {
        func_80015C38(D_800A8098, arg0);
    }
    func_800B828C(arg0);
    p = D_800D3EA8.unk4;
    for (i = 0; i < 3;) {
        if (p->unk8 == arg0) {
            p->unkE = 1;
            return;
        }
        i++;
        p = p->unk4;
    }
}

INCLUDE_ASM("asm/us/ovl_0c/nonmatchings/ovl_0c", func_800C2134);

INCLUDE_RODATA("asm/us/ovl_0c/nonmatchings/ovl_0c", D_800A8098);

const char D_800A80BC[] = "w_frmbufRequest:reqover [%d/%d]\n";
const char D_800A80E0[] = "==== ERROR ===== / %s";
const char D_800A80F8[] = "-- w_framebufRequest Error --\n";

s32 func_800C2230(s16 arg0) {
    WmLNode *p;
    u32 i;

    if (arg0 & 0x3F) {
        p = D_800D3F38.unk4;
        i = 0;
        while (1) {
            if (p->unk8 == arg0) {
                p->unk0->unk4 = p->unk4;
                p->unk4->unk0 = p->unk0;
                p->unk4 = D_800D3F38.unk4;
                p->unk0 = &D_800D3F38;
                D_800D3F38.unk4->unk0 = p;
                D_800D3F38.unk4 = p;
                break;
            }
            if (p->unk8 == -1) {
                D_800D3F20++;
                if (D_800D3F20 >= 3) {
                    sprintf(D_800CCE6C, D_800A80BC, D_800D3F20, 2);
                    func_80015C38(D_800A80E0, D_800CCE6C);
                    D_800CCE69 = 1;
                }
                p->unk8 = arg0;
                p->unk0->unk4 = p->unk4;
                p->unk4->unk0 = p->unk0;
                p->unk4 = D_800D3F38.unk4;
                p->unk0 = &D_800D3F38;
                D_800D3F44++;
                D_800D3F38.unk4->unk0 = p;
                D_800D3F38.unk4 = p;
                break;
            }
            p = p->unk4;
            i++;
            if (i >= 4) {
                break;
            }
        }
        if (i == 4) {
            func_80015C38(D_800A80F8);
            return 0;
        }
    }
    return 1;
}

void func_800C23CC(void) {
    WmLNode *p;
    WmLNode *q;
    u32 i;

    D_800D3F20 = 0;
    if (D_800D3F44 != 0) {
        p = D_800D3F38.unk4;
        i = 0;
        D_800D3F44 = 0;
        q = p;
        do {
            p = p->unk4;
            i++;
        } while (i < 2);
        for (;;) {
            while ((p != &D_800D3F28) & (p->unkA == 0)) {
                if (((u8)p->unkA == 0) & (p->unk8 != -1)) {
                    p->unk8 = -1;
                }
                p = p->unk4;
            }
            if (p == &D_800D3F28) {
                break;
            }
            while (q->unkA != 0) {
                q = q->unk4;
            }
            q->unkA = p->unkA;
            p->unkA = 0;
            p->unkB = 0;
            p->unk8 = -1;
            p = p->unk4;
        }
    }
}

INCLUDE_ASM("asm/us/ovl_0c/nonmatchings/ovl_0c", func_800C24D8);

void func_800C26DC(void) {
}

void func_800C26E4(void) {
}

INCLUDE_ASM("asm/us/ovl_0c/nonmatchings/ovl_0c", func_800C26EC);

void func_800C2830(void) {
    s32 id = func_800C291C();

    if (id != D_800D3F8C) {
        D_800D3F8C = id;
        func_800C2A80(id, func_800C3E88() & 0xFF, 0x80);
    }
    if (D_8007AEF0.b[0x66] == 0 && D_800CF4AC == 10) {
        switch ((D_800CCCC0[D_800CE3DC->unk11].unk8 & 0x3F00) >> 8) {
        case 9:
        case 12:
        case 13:
            D_800D3FA0.unk14A = 0x7F58;
            D_800D3FA0.unk14C = 0x7F58;
            func_800C2A80(id, func_800C3E88() & 0xFF, 0x80);
            D_800CF46D = 0;
            break;
        }
    }
}

s32 func_800C291C(void) {
    s32 dx;
    s32 dz;
    s32 r;
    s32 d;

    dx = (D_800CE3DC->unk14.vx >> 8) - 0x508;
    dz = (D_800CE3DC->unk14.vz >> 8) + 0x3AD;
    r = SquareRoot0(dx * dx + dz * dz) < 0x56;
    dx = (D_800CE3DC->unk14.vx >> 8) - 0x50B;
    dz = (D_800CE3DC->unk14.vz >> 8) + 0x3A0;
    if (SquareRoot0(dx * dx + dz * dz) < 0x3B) {
        r = 2;
    }
    dx = (D_800CE3DC->unk14.vx >> 8) - 0x1F4;
    dz = (D_800CE3DC->unk14.vz >> 8) + 0x316;
    if (SquareRoot0(dx * dx + dz * dz) < 0xB4) {
        r = 1;
    }
    if (D_800CF444 == 4) {
        dx = (D_800CE3DC->unk14.vx >> 8) - 0x2FE;
        dz = (D_800CE3DC->unk14.vz >> 8) + 0x140;
        d = SquareRoot0(dx * dx + dz * dz);
        D_800D3F88 = d;
        if (d < 0x65) {
            r = 3;
        }
    }
    return r;
}

INCLUDE_ASM("asm/us/ovl_0c/nonmatchings/ovl_0c", func_800C2A80);

INCLUDE_ASM("asm/us/ovl_0c/nonmatchings/ovl_0c", func_800C3954);

void func_800C3DEC(void) {
    D_800D3F9C = 1;
    D_800D3F98 = -2;
    D_800D3F8C = -1;
    D_800D3F94 = 0;
    D_800D3F90 = 0x80;
}

INCLUDE_RODATA("asm/us/ovl_0c/nonmatchings/ovl_0c", D_800A8118);

void func_800C3E28(void) {
    D_800D3F8C = func_800C291C();
    D_800CCE4B = func_800C3E88();
    func_80015C38("start weather mode : %d / fog : %d\n", D_800D3F8C, D_800CCE4B);
    func_800C2A80(D_800D3F8C, D_800CCE4B, 0x1000);
}

s32 func_800C3E88(void) {
    return (u32)(*D_800CF430 - 0x1766) >= 0x13ED;
}

INCLUDE_RODATA("asm/us/ovl_0c/nonmatchings/ovl_0c", D_800A8178);

void func_800C3EAC(void) {
    s32 st;

    switch (D_800D4818) {
    case 1:
        if (D_800CF454 == 0) {
            if (D_800CCE50 & 4) {
                func_80015C38("w_musicUpdate : Play\n");
            }
            D_800CF454 = 2;
            if (D_800D481C != -1) {
                func_800C46D8(D_800D481C);
            }
            func_800C465C(D_800D4828);
            D_800D4818 = 2;
            D_800D481C = D_800D4828;
        }
        break;
    case 2:
        st = func_800AB0C0(0xC00, 0, 0, 0, 0);
        if (st == 0) {
            D_800CF454 = 0;
            D_800D4818 = 0;
            if (D_800CCE50 & 4) {
                func_80015C38("w_musicUpdate : Ready\n");
            }
        } else if (D_800CCE50 & 4) {
            func_80015C38("w_musicUpdate : Waiting [ State : %d ] \n", st);
        }
        break;
    }
}

INCLUDE_ASM("asm/us/ovl_0c/nonmatchings/ovl_0c", func_800C3FE4);

const char D_800A832C[] = "w_musicRequestSend : Normal Song";
const char D_800A8350[] = "w_musicRequestSend : Air Ship Song";
const char D_800A8374[] = "w_musicRequestSend : Chocobo Song";
const char D_800A8398[] = "w_musicRequestSend : Disc4 Song";
const char D_800A83B8[] = "w_musicRequestSend : Field Song 1";
const char D_800A83DC[] = "w_musicRequestSend : Field Song 2";
const char D_800A8400[] = "w_musicRequestSend : Unknown Error";
const char D_800A8424[] = "(Accepted)\n";
const char D_800A8430[] = "(Denied / same request)\n";
/* padded: the jump table that follows is 8-aligned in its object */
const char D_800A844C[0x38] = "w_musicRequestSend : music status is not ready now\n";

void func_800C42B0(u32 arg0) {
    if (D_800D4818 == 0) {
        if (D_800CCE50 & 4) {
            switch (arg0) {
            case 0:
                func_80015C38(D_800A832C);
                break;
            case 2:
                func_80015C38(D_800A8350);
                break;
            case 1:
                func_80015C38(D_800A8374);
                break;
            case 3:
                func_80015C38(D_800A8398);
                break;
            case 4:
                func_80015C38(D_800A83B8);
                break;
            case 5:
                func_80015C38(D_800A83DC);
                break;
            default:
                func_80015C38(D_800A8400);
                break;
            }
        }
        if (D_800D4828 != arg0) {
            D_800D4818 = 1;
            D_800D4828 = arg0;
            if (D_800CCE50 & 4) {
                func_80015C38(D_800A8424);
            }
        } else if (D_800CCE50 & 4) {
            func_80015C38(D_800A8430);
        }
    } else if (D_800CCE50 & 4) {
        func_80015C38(D_800A844C);
    }
}

INCLUDE_ASM("asm/us/ovl_0c/nonmatchings/ovl_0c", func_800C43F0);

s32 func_800C45D8(void) {
    u16 sc = D_8007AEF0.h[0];
    u8 v = D_8007AEF0.b[0xBE];

    if (sc - 0x2580U < 0x122) {
        return 1;
    }
    if (sc - 0x26A2U < 0x14) {
        return 1;
    }
    return v - 8U < 2;
}

void func_800C461C(void) {
    D_800D4818 = 0;
    D_800D4824 = -1;
    D_800D481C = -1;
    D_800D4828 = -1;
    D_800D4820 = 0;
}

void func_800C464C(void) {
}

void func_800C4654(void) {
}

void func_800C465C(u16 arg0) {
    u8 v = D_8007AEF0.b[0x68];

    D_800D4824 = func_800C497C(arg0);
    D_800D4820 = 1;
    func_800AB0C0(0, D_800D4824, 0, 0, 0);
    if (v == 0) {
        v = 0x7D;
    }
    func_800AB0C0(0x8601, 0, 0x3C, v, 0);
}

void func_800C46D8(u16 arg0) {
    func_800AB0C0(0x100, func_800C497C(arg0), 0, 0, 0);
}

void func_800C4710(void) {
}

void func_800C4718(s32 arg0, u8 arg1) {
    Unk800C3FE4 s;
    s32 i;
    s32 v;

    if (func_800C3FE4(arg0, &s) & 0xFF) {
        v = s.unk0;
        for (i = 0; i < s.unk4; i++) {
            func_800AB0C0(0xD000, s.unk8[i], v, 0x7F, arg1);
            v >>= 1;
        }
    }
}

void func_800C47B0(s32 arg0, u8 arg1) {
    Unk800C3FE4 s;
    s32 i;
    s32 v;

    if (D_800D4818 == 0 && (func_800C3FE4(arg0, &s) & 0xFF)) {
        v = s.unk0;
        for (i = 0; i < s.unk4; i++) {
            func_800AB0C0(0x9201, s.unk8[i], v, arg1, 0);
            v >>= 1;
        }
    }
}

void func_800C4850(s32 arg0, u16 arg1, u8 arg2) {
    Unk800C3FE4 s;
    s32 i;
    s32 v;

    if (func_800C3FE4(arg0, &s) & 0xFF) {
        v = s.unk0;
        for (i = 0; i < s.unk4; i++) {
            func_800AB0C0(0xD301, s.unk8[i], v, arg1, arg2);
            v >>= 1;
        }
    }
}

void func_800C48F4(s32 arg0) {
    Unk800C3FE4 s;
    s32 i;
    s32 v;

    if (func_800C3FE4(arg0, &s) & 0xFF) {
        v = s.unk0;
        for (i = 0; i < s.unk4; i++) {
            func_800AB0C0(0x5100, s.unk8[i], v, 0, 0);
            v >>= 1;
        }
    }
}

s32 func_800C497C(u16 arg0) {
    u16 sc;

    if (arg0 == 0) {
        sc = *D_800CF430;
        if (sc - 0x2580U < 0x122) {
            arg0 = 4;
        }
        if (sc - 0x26A2U < 0x14) {
            arg0 = 5;
        }
        if (D_800CF444 == 4) {
            arg0 = 3;
        }
    }
    return D_800CDF30[arg0];
}

s32 func_800C49EC(void) {
    u8 r = D_800D4824 == D_800CDF30[0];

    if (D_800D4824 == D_800CDF30[1]) {
        r = 1;
    }
    if (D_800CE3C4 == 0) {
        if (D_800D4824 == D_800CDF30[3]) {
            r = 1;
        }
        if (D_800D4824 == D_800CDF30[4]) {
            r = 1;
        }
    }
    return r;
}

INCLUDE_ASM("asm/us/ovl_0c/nonmatchings/ovl_0c", func_800C4A5C);

INCLUDE_RODATA("asm/us/ovl_0c/nonmatchings/ovl_0c", D_800A849C);

INCLUDE_ASM("asm/us/ovl_0c/nonmatchings/ovl_0c", func_800C4DD8);

INCLUDE_ASM("asm/us/ovl_0c/nonmatchings/ovl_0c", func_800C53DC);

INCLUDE_ASM("asm/us/ovl_0c/nonmatchings/ovl_0c", func_800C5A48);

INCLUDE_RODATA("asm/us/ovl_0c/nonmatchings/ovl_0c", D_800A8588);

INCLUDE_RODATA("asm/us/ovl_0c/nonmatchings/ovl_0c", D_800A8598);

INCLUDE_ASM("asm/us/ovl_0c/nonmatchings/ovl_0c", func_800C69F4);

void func_800C6EF8(void) {
}

void func_800C6F00(s32 arg0, s32 arg1, s32 arg2, s32 arg3, s32 arg4) {
    WmNode *p;
    s32 v;
    u8 k;
    WmNode *h;

    p = D_800D4848.unk8;
    h = &D_800D53A0;
    switch (arg3) {
    case 2:
    case 3:
        k = 2;
        break;
    case 16:
        k = 0xFF;
        break;
    case 4:
    case 5:
    case 6:
    case 7:
    case 8:
    case 9:
    case 10:
    case 11:
    case 12:
    case 13:
    case 14:
    case 15:
    case 17:
    case 18:
    case 19:
    case 20:
    case 21:
    case 22:
    case 23:
    case 24:
    case 25:
    case 26:
    case 27:
    case 28:
    case 29:
        k = 1;
        break;
    default:
        k = 1;
        break;
    }
    if (p->unk8 != 0) {
        v = (s32)func_800B8388(D_800CF428, arg3 + 7);
        p->unk4->unk8 = p->unk8;
        p->unk8->unk4 = p->unk4;
        p->unk4 = h;
        p->unk8 = h->unk8;
        h->unk8->unk4 = p;
        h->unk8 = p;
        p->unkC = k;
        p->unk1C = 0;
        p->unk10 = arg0;
        p->unk14 = arg1;
        p->unk18 = arg2;
        p->unk0 = v;
        p->unkE = arg4;
        p->unkD = arg3;
        p->unk20 = 0;
        p->unk24 = 0;
        if (arg3 == 0x17) {
            p->unk20 = func_80062B3C() & 0xFFF;
        }
    }
}

void func_800C7050(WmNode *arg0) {
    arg0->unk4->unk8 = arg0->unk8;
    arg0->unk8->unk4 = arg0->unk4;
    arg0->unk4 = &D_800D4848;
    arg0->unk8 = D_800D4848.unk8;
    D_800D4848.unk8->unk4 = arg0;
    D_800D4848.unk8 = arg0;
}

INCLUDE_ASM("asm/us/ovl_0c/nonmatchings/ovl_0c", func_800C709C);

INCLUDE_ASM("asm/us/ovl_0c/nonmatchings/ovl_0c", func_800C7914);

INCLUDE_ASM("asm/us/ovl_0c/nonmatchings/ovl_0c", func_800C7BA8);

INCLUDE_ASM("asm/us/ovl_0c/nonmatchings/ovl_0c", func_800C7CF4);

INCLUDE_ASM("asm/us/ovl_0c/nonmatchings/ovl_0c", func_800C8950);

void func_800C9484(Unk800C95C8 *arg0) {
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

void func_800C95C8(Unk800C95C8 *arg0) {
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

INCLUDE_ASM("asm/us/ovl_0c/nonmatchings/ovl_0c", func_800C96AC);

INCLUDE_ASM("asm/us/ovl_0c/nonmatchings/ovl_0c", func_800C9FCC);

INCLUDE_ASM("asm/us/ovl_0c/nonmatchings/ovl_0c", func_800CA1D8);

s32 func_800CAE00(u16 *arg0) {
    return *arg0 & 0x7FFF;
}

INCLUDE_ASM("asm/us/ovl_0c/nonmatchings/ovl_0c", func_800CAE0C);

INCLUDE_ASM("asm/us/ovl_0c/nonmatchings/ovl_0c", func_800CB09C);

INCLUDE_ASM("asm/us/ovl_0c/nonmatchings/ovl_0c", func_800CB1EC);

void func_800CB434(void) {
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
    D_800CDFCC[5].w = w;
    D_800CDFCC[5].x = 0x134 - w;
    for (i = 0; i < 6; i++) {
        D_800D5438.unk8[i].unk4 = D_800CDFCC[i];
        func_80063118(&D_800D5438.unk8[i], NULL, 0);
    }
    if (D_80077E51 != 0) {
        func_80063118(&D_800D5438.unk8[0], &D_800CE00C[0], 1);
        func_80063118(&D_800D5438.unk8[1], &D_800CE00C[1], 1);
        func_80063118(&D_800D5438.unk8[2], &D_800CE00C[2], 1);
        func_80063118(&D_800D5438.unk8[3], &D_800CE00C[3], 1);
    }
}

void func_800CB594(void) {
    if (D_80077528 & 0x10000) {
        func_8002F488(0x65);
        func_800362C8();
    } else if (D_80077528 & 0x20000) {
        func_800CC54C();
    } else if (D_80077528 & 0x10) {
        func_8002F488(0x67);
        D_800D5438.unk0 = D_800D5438.unk0 != 0 ? D_800D5438.unk0 - 1 : 6;
    } else if (D_80077528 & 0x40) {
        func_8002F488(0x67);
        D_800D5438.unk0 = D_800D5438.unk0 < 6 ? D_800D5438.unk0 + 1 : 0;
    }
}

void func_800CB664(void) {
    Unk80077E50 *m;
    s32 old;
    s32 new;

    if (D_80077528 & 0x10000) {
        func_8002F488(0x65);
        D_800D5438.unk1 = 0;
        return;
    } else if (D_80077528 & 0x20000) {
        func_8002F488(0x67);
        func_800361CC(D_800D5438.unk0 + 1);
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

void func_800CB73C(void) {
    if (D_80077528 & 0x10000) {
        func_8002F488(0x65);
        if (D_800D5438.unk4 == 0) {
            D_800D5438.unk1 = 0;
        } else {
            D_800D5438.unk4 = 0;
        }
    } else if (D_80077528 & 0x10) {
        func_8002F488(0x67);
        if (D_800D5438.unk4 == 0) {
            D_800D5438.unk2 = D_800D5438.unk2 != 0 ? D_800D5438.unk2 - 1 : 3;
        } else {
            D_800D5438.unk3 = D_800D5438.unk3 != 0 ? D_800D5438.unk3 - 1 : 3;
        }
    } else if (D_80077528 & 0x40) {
        func_8002F488(0x67);
        if (D_800D5438.unk4 == 0) {
            D_800D5438.unk2 = D_800D5438.unk2 < 3 ? D_800D5438.unk2 + 1 : 0;
        } else {
            D_800D5438.unk3 = D_800D5438.unk3 < 3 ? D_800D5438.unk3 + 1 : 0;
        }
    } else if (D_80077528 & 0x20000) {
        if (D_800D5438.unk4 == 0) {
            func_8002F488(0x67);
            D_800D5438.unk4 = 1;
            D_800D5438.unk3 = D_800D5438.unk2;
        } else {
            func_8002F488(0x67);
            func_800CB8C8(D_800D5438.unk2, D_800D5438.unk3);
            D_800D5438.unk4 = 0;
            D_800D5438.unk2 = D_800D5438.unk3;
        }
    }
}

void func_800CB8C8(s32 arg0, s32 arg1) {
    Character *c;
    Character *t;

    if (arg0 == arg1) {
        c = D_8006794C->unk1C->unk54C[arg1];
        if (c != NULL) {
            c->unk34 = (c->unk34 & ~0x200) | ((((c->unk34 >> 9) & 1) ^ 1) << 9);
        }
    } else {
        t = D_8006794C->unk1C->unk54C[D_800D5438.unk2];
        D_8006794C->unk1C->unk54C[D_800D5438.unk2] = D_8006794C->unk1C->unk54C[D_800D5438.unk3];
        D_8006794C->unk1C->unk54C[D_800D5438.unk3] = t;
        func_800CB9D8(arg0, arg1);
        if (D_8006794C->unk1C->unk54C[D_80077E50.unk3] == NULL) {
            D_80077E50.unk3 = func_8003378C();
        }
    }
}

void func_800CB9D8(s32 arg0, s32 arg1) {
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

void func_800CBB4C(s32 arg0) {
    u8 buf[16];
    RECT *r;
    Character *c;
    s32 x;
    s32 color;
    s16 maxmp;

    c = D_8006794C->unk1C->unk54C[arg0];
    r = &D_800D5438.unk8[arg0].unk4;
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

void func_800CBDF8(RECT *r, s32 otz) {
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

void func_800CBF18(void) {
    u8 buf[16];
    s32 v;

    v = func_80024030();
    func_800275DC(D_800CE04C->x + 6, D_800CE04C->y + 8, 0x13, 0xB1, 0);
    func_800275DC(D_800CE04C->x + 6, D_800CE04C->y + 0x15, 0x13, 0xB2, 0);
    func_8002B56C(D_800CE04C->x + 0x13, D_800CE04C->y + 9, 0x13, buf, func_8002AA04(v, buf, 1));
    func_8002BAE4(D_8006794C->unk1C->unk560, buf, 7, 1);
    func_80063108(buf, D_800CE050);
    func_8002B56C(D_800CE04C->x + 0x13, D_800CE04C->y + 0x16, 0x13, buf, 0);
    func_8002B524(D_800CE04C->x + 6, D_800CE04C->y - 3, 0x13, D_800704C0, 0);
    func_8002DEE4(D_800CE04C, 0x14, 2);
}

void func_800CC050(void) {
    GameData *gd;

    func_8002B524(D_800CE054->x + 8, D_800CE054->y - 3, 0x13, D_8007046C, 0);
    func_8002DEE4(D_800CE054, 0x14, 2);
    func_8002705C(D_800CE054, 0x14, 2);
    gd = D_8006794C->unk1C;
    if (gd->unk80C != NULL && *gd->unk80C != 0xFF) {
        func_8002B5FC(D_800CE054->x + 8, D_800CE054->y + 7, 0x13, gd->unk80C, 0);
    } else {
        func_8002B5FC(D_800CE054->x + 8, D_800CE054->y + 7, 0x13, func_80035774(1), 0);
    }
}

void func_800CC13C(void) {
    u8 blink;

    blink = (D_80077E54 & 3) != 0;
    func_80035674(D_800D5438.unk1 - 1U >= 2);
    if (!func_8002F810()) {
        if (D_800D5438.unk1 == 0 || blink) {
            D_80077DE8.unk0 = 0xE0;
            D_80077DE8.unk4 = 1;
            D_80077DE8.unk2 = D_800D5438.unk0 * 0xF + 0x11;
            func_800355E4();
        }
        if (D_800D5438.unk1 == 1) {
            D_80077DE8.unk4 = 0xC;
            D_80077DE8.unk0 = D_800D5438.unk8[0].unk4.x - 0xE;
            D_80077DE8.unk2 = D_80077E53 * 0x2E + 0x20;
            func_800355E4();
        }
        if (D_800D5438.unk1 == 2) {
            if (D_800D5438.unk4 == 1) {
                D_80077DE8.unk4 = 0xB;
                D_80077DE8.unk0 = D_800D5438.unk8[0].unk4.x - 0xA;
                D_80077DE8.unk2 = D_800D5438.unk3 * 0x2E + 0x24;
                func_800355E4();
                if (blink) {
                    func_800270EC(D_800D5438.unk8[0].unk4.x - 0xE, D_800D5438.unk2 * 0x2E + 0x20, 0xC);
                }
            } else if (D_800D5438.unk4 == 0) {
                D_80077DE8.unk4 = 0xC;
                D_80077DE8.unk0 = D_800D5438.unk8[0].unk4.x - 0xE;
                D_80077DE8.unk2 = D_800D5438.unk2 * 0x2E + 0x20;
                func_800355E4();
            }
        }
    }
}

void func_800CC324(void) {
    D_80077DE8.unk8 = 0;
    if (D_800D5438.unk1 == 0) {
        D_80077DE8.unk8 = (u8 *)func_8003631C(D_800D5438.unk0);
    } else if (D_800D5438.unk1 == 1) {
        if (D_800D5438.unk0 + 1 == 3) {
            func_800CC5D8(D_800D5438.unk0);
        } else {
            s32 a = D_800D5438.unk0 + 2;
            if (D_800D5438.unk0 + 1 == 4) {
                func_800CC3E0(a);
            } else {
                D_80077DE8.unk8 = func_80035774(a);
            }
        }
    } else if (D_800D5438.unk1 == 2) {
        D_80077DE8.unk8 = func_80035774(D_800D5438.unk4 + 0xB);
    }
}

void func_800CC3E0(s32 arg0) {
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
        func_8002BAE4(c->unkC, D_800D5418, 7, 1);
        func_8002BAE4(next - c->unkC, D_800D5428, 7, 1);
        func_8002BE44(1, D_800CE06C);
        func_8002BE44(2, D_800D5418);
        func_8002BE44(3, D_800CE070);
        func_8002BE44(4, D_800CE078);
        func_8002BE44(5, D_800D5428);
        func_8002BE44(6, D_800CE080);
        func_8002BE44(7, c->unk38 != 0 ? D_800CE090 : D_800CE088);
        D_80077DF0 = func_80035774(5);
    }
}

void func_800CC54C(void) {
    if (D_800D5438.unk0 + 1 != 5) {
        if (func_80036300(D_800D5438.unk0 + 1) == 0) {
            func_800361CC(D_800D5438.unk0 + 1);
        } else {
            D_800D5438.unk1 = 1;
        }
    } else {
        D_800D5438.unk1 = 2;
        D_800D5438.unk4 = 0;
    }
    func_8002F488(0x67);
}

void func_800CC5CC(void) {
    D_80077E50.unk1 = 0;
}

void func_800CC5D8(u8 arg0) {
    s32 i;
    s32 j;
    Character *c;
    s32 id;

    i = 0;
    j = 5;
    c = D_8006794C->unk1C->unk54C[D_80077E53];
    do {
        func_8002BE44(i, D_800CE058[i]);
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

void func_800CC6E4(void) {
    RECT r;

    if (D_80077E50.unk1 != 0) {
        D_800D5438.unk0 = 0;
        D_800D5438.unk2 = 0;
    }
    D_800D5438.unk1 = 0;
    func_8003575C(D_800A8678);
    func_800CB434();
    r.x = 8;
    r.y = 8;
    r.w = 0xE8;
    r.h = D_8006794C->unkC->disp.h - 0xF;
    func_80035580(&r);
    func_8003556C(func_800CC324);
    func_80035674(1);
    if (D_8006794C->unk1C->unk54C[D_80077E50.unk3] == NULL) {
        D_80077E50.unk3 = func_8003378C();
    }
}

void func_800CC7C0(void) {
    func_800CC82C();
    func_800CC890();
    func_800CC8C8();
}

void func_800CC7F0(void) {
    func_80035674(1);
    func_80035580(0);
    func_8003556C(0);
}

void func_800CC820(u8 arg0) {
    D_800D5438.unk0 = arg0;
}

void func_800CC82C(void) {
    if (D_80077E50.unk1 == 0 && func_8002F810() == 0) {
        if (D_800CDFC0[D_800D5438.unk1] != NULL) {
            D_800CDFC0[D_800D5438.unk1]();
        }
    }
}

void func_800CC890(void) {
    func_800CBF18();
    func_800CC050();
    func_800CC940();
    func_800CC13C();
}

void func_800CC8C8(void) {
    s32 i;

    if (D_80077E50.unk1 != 0) {
        for (i = 0; i < 6; i++) {
            if (D_800D5438.unk8[i].unkC != 0) {
                func_800631AC(&D_800D5438.unk8[i]);
            }
        }
    }
}

void func_800CC940(void) {
    RECT r;
    s16 y;
    s32 i;
    RECT *p;

    i = 0;
    do {
        p = &D_800D5438.unk8[i].unk4;
        r.x = p->x + 0x82;
        y = p->y;
        r.w = 0x72;
        r.h = 0x12;
        r.y = y;
        if (D_8006794C->unk1C->unk54C[i] != NULL) {
            func_800CBB4C(i);
        }
        func_800CBDF8(p, 0x14);
        func_8002DEE4(&r, 0x14, 0);
        func_8002DEE4(p, 0x14, 0);
        func_8002705C(p, 0x15, 0);
        i++;
    } while (i < 4);
}

INCLUDE_RODATA("asm/us/ovl_0c/nonmatchings/ovl_0c", D_800A8644);

INCLUDE_RODATA("asm/us/ovl_0c/nonmatchings/ovl_0c", D_800A8678);
