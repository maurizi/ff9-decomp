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
    D_8006794C->unk1C->unk838 = (s32 (*)(s32, void *))func_800AA15C;
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

/* load the world map background image and the sound bank */
s32 func_800A8D78(void) {
    u16 ids[2] = { 0x17B, 0x139 };
    s32 i;
    s32 id;
    u32 *addr;
    u32 size;
    RECT *r;

    for (i = 1; i >= 0; i--) {
        id = ids[i] | 0x90000;
        addr = func_8001C7FC(4, id, 0);
        size = (u32)func_8001C7FC(5, id, 0);
        if (ids[i] == 0x17B) {
            r = &D_800CE0B8[D_800CCA1C];
            r->x = D_8006794C->unk10[D_8006794C->unk1C->unk8].clip.x;
            r->y = D_8006794C->unk10[D_8006794C->unk1C->unk8].clip.y;
            r->h = 218;
            r->w = size / 2 / r->h + 1;
            LoadImage(r, addr);
            DrawSync(0);
            D_8006794C->unk1C->unk8E4.world->unk10 = size;
        } else {
            func_80055DF4();
            func_80055E14((s32)addr, size, 1);
        }
    }
    return 0;
}

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

void func_800A924C(s32 arg0) {
    WorldMapObj *obj;
    s32 i;

    D_80067948 = (s32)D_8006794C->unk1C->unk8E4.world->unk4;
    func_800A98B8();
    obj = D_8006794C->unk1C->unk8E4.world->unk8;
    func_800A98F8();
    D_8006794C->unk0 &= ~0x3001;
    D_8006794C->unk1C->unk8E4.world->unk0 |= 0x400;
    D_8006794C->unk1C->unk7DC = arg0;
    obj->unk24 = arg0;
    func_800A9A44();
    func_800A9C1C(obj, arg0);
    func_800A9A04(obj, arg0);
    func_800A99C4(obj, (s16)D_8006794C->unk1C->unk7DE);
    func_800A9B54();
    func_800AB0C0(0x900, 0, 0, 0, 0);
    func_80049ED8(obj->unk10);
    func_8004A06C(obj->unk10);
    while (func_800AB0C0(0xC00, 0, 0, 0, 0) != 0) {
    }
    func_8004BFB0();
    D_8006794C->unk1C->unk38 = func_8004AAF0(0);
    D_8006794C->unk1C->unk8DC = (Unk8D4 *)(obj + 1);
    D_8006794C->unk1C->unk8E0 = (Unk8E0 *)&D_8006794C->unk1C->unk8DC[D_8006794C->unk1C->unk38];
    D_80067948 = (s32)&D_8006794C->unk1C->unk8E0[D_8006794C->unk1C->unk38];
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
    obj->unk1C = (void *)D_80067948;
    D_80067948 = func_800AAD0C(D_80067948, D_8006794C->unk1C->unk38 + 9);
    obj->unk4 = (void *)D_80067948;
    obj->unk20 = (s32)obj->unk30;
    func_8004A8A4();
    func_800B7F7C();
    func_80021C60();
    D_8006794C->unk1C->unk8E4.world->unk0 &= ~1;
}

void func_800A95A0(void) {
    WorldMapObj *obj;
    GameData *gd;
    s32 i;
    s32 mode;

    obj = D_8006794C->unk1C->unk8E4.world->unk8;
    DrawSync(0);
    while (func_800AB0C0(0xC00, 0, 0, 0, 0) != 0) {
    }
    func_8005546C(-1, 0);
    func_800A9DE4(obj, D_8006794C->unk1C->unk7DC);
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
    func_800B8024();
    gd = D_8006794C->unk1C;
    mode = gd->unk8E4.world->unk8->unk1B;
    switch (mode) {
    case 2:
        func_800A9EC4(func_800A9E24(obj->unk24, &gd->unk7FC[0], &gd->unk804[0]), &D_8006794C->unk1C->unk7FC[1],
                      &D_8006794C->unk1C->unk804[1]);
        D_8006794C->unk1C->unkE = obj->unk24;
        D_8006794C->unk8 = mode;
        D_8006794C->unkA = 3;
        break;
    case 3:
        i = func_800AA060(func_800A9FC0(obj->unk24, &gd->unk7FC[1], &gd->unk804[1]), &D_8006794C->unk1C->unk7FC[0],
                          &D_8006794C->unk1C->unk804[0]);
        D_8006794C->unk1C->unk7DC = obj->unk24;
        D_8006794C->unk1C->unk7DE = i;
        break;
    case 1:
        D_8006794C->unk1C->unk3A = func_800AA060(func_800A9FC0(obj->unk24, &gd->unk7FC[1], &gd->unk804[1]),
                                                 &D_8006794C->unk1C->unk7FC[0], &D_8006794C->unk1C->unk804[0]);
        D_8006794C->unk1C->unkC = obj->unk24;
        D_8006794C->unk8 = mode;
        D_8006794C->unkA = 3;
        break;
    default:
        func_8002EB64(0x39, 0x194);
        break;
    }
    D_8006794C->unk1C->unk7E4 = (struct FileTable *)-1;
}

void func_800A98B8(void) {
    WorldMap *wm = D_8006794C->unk1C->unk8E4.world;

    wm->unk8 = &wm->unk48;
    wm->unk8->unk2C = D_80067944 + 0x5EB00;
    D_80067948 = (s32)(wm + 1);
}

/* reset the world map object */
void func_800A98F8(void) {
    WorldMapObj *o = D_8006794C->unk1C->unk8E4.world->unk8;
    s32 i;

    o->unkC = 0xA0;
    o->unkE = 0x6D;
    o->unk0 = 0;
    o->unk4 = NULL;
    o->unk10 = NULL;
    o->unk14 = NULL;
    o->unk1C = NULL;
    o->unk28 = NULL;
    o->unk1A = 0;
    o->unk20 = 0;
    o->unk19 = 0;
    o->unk18 = 0;
    o->unk26 = 0xFFFF;
    o->unk30 = o->unk2C;
    for (i = 1; i >= 0; i--) {
        o->unk34[i] = NULL;
    }
    o->unk24 = -1;
    o->unk1B = 3;
    for (i = 0; i < sizeof(o->unk8); i++) {
        o->unk8[i] = 0;
    }
    for (i = 0; i < sizeof(o->unk3C); i++) {
        o->unk3C[i] = 0;
    }
}

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
    func_8002177C(gd->unk854, gd->unk7FC[1], gd->unk804[1] << 11, arg0->unk30, 0, -1, 0);
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
    arg0->unk28 = (WmPalAnim *)q;
    arg0->unk30 = (u8 *)arg0->unk34[1] + func_8001CA70((s32)arg0->unk34[1]);
    return 0;
}

s32 func_800A9DE4(WorldMapObj *arg0, s16 arg1) {
    if (arg0->unk34[1] != NULL) {
        func_8001C784(arg0->unk34[1]);
        arg0->unk34[1] = NULL;
    }
    return 0;
}

s32 func_800A9E24(s16 arg0, s32 *arg1, s32 *arg2) {
    s32 i;
    FileTable *t = D_8006794C->unk1C->unk7E4;
    s32 id = (u16)arg0 | 0x110000;
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

s32 func_800A9FC0(s16 arg0, s32 *arg1, s32 *arg2) {
    s32 i;
    FileTable *t = D_8006794C->unk1C->unk7E4;
    s32 id = (u16)arg0 | 0x50000;
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

u32 func_800AA15C(s32 arg0, Unk8004AB64 *arg1) {
    WmObjSlot *o;
    u32 id;
    WmAnimHdr *h;
    s32 t;
    WmModel *m;
    WmModelGroup *g;
    u32 i;
    s32 n;

    t = func_8004AB64(arg1);
    if ((u32)t < 9) {
        id = t;
        o = (WmObjSlot *)func_8001E350(t);
    } else {
        id = D_8006794C->unk1C->unk39 + 9;
        o = (WmObjSlot *)func_8001E350(id);
        D_8006794C->unk1C->unk39++;
    }
    o->unk4 = arg0;
    o->unk8->unk8 = func_8001C7FC(4, arg0, 0);
    if ((o->unk8->unkC = func_8001C7FC(4, func_800AA39C(o->unk4), 0)) != NULL) {
        h = o->unk8->unkC;
        if (h->unkC >= 0) {
            t = h->unk10;
            h->unkC = (s32)h + h->unkC;
            if (t != 0) {
                h->unk10 = (s32)o->unk8->unkC + t;
            }
        }
    }
    o->unkC = arg1;
    m = o->unk8->unk8;
    g = m->unk10;
    n = 0;
    if ((s32)g >= 0) {
        g = (WmModelGroup *)((s32)g + (s32)m);
    }
    for (i = 0; i < m->unk3; i++) {
        n += g[i].unk0;
    }
    n *= 2;
    n += (o->unk8->unk8->unk2 << 5) + o->unk8->unk8->unk3 * 12;
    func_800C7BA8(o->unk8, D_8006794C->unk1C->unk8E4.world->unk8->unk20);
    func_800C7CF4(o->unk8, func_800634A8(o->unk4), func_800634D8(o->unk4));
    D_8006794C->unk1C->unk8E4.world->unk8->unk20 += n;
    o->unk0 |= 1;
    return id;
}

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

void func_800AA418(WmObjEnt *arg0, s32 arg1, s32 arg2, s32 arg3) {
    MATRIX m;
    POLY_FT4 *p[4];
    SVECTOR v;
    s32 z[9];
    WmObjEnt *e;
    WmObjSlot *s;
    s32 i;
    s32 k;

    e = &arg0[arg1];
    s = (WmObjSlot *)func_8001E350(arg1);
    if (s->unk0 & 0x10) {
        return;
    }
    func_800AAD48(e, s, &m, arg2, arg3);
    for (i = 0; i < 4; i++) {
        p[i] = e->unk8[D_8006794C->unk1C->unk8][i];
        setRGB0(p[i], e->unk0, e->unk0, e->unk0);
    }
    gte_MulMatrix0(&D_8006794C->unk1C->unk18, &m, &m);
    gte_SetTransMatrix(&D_8006794C->unk1C->unk18);
    gte_ldlv0(m.t);
    gte_rtv0tr();
    gte_stir(m.t);
    gte_SetRotMatrix(&m);
    gte_SetTransMatrix(&m);
    SHADOW_PT(-0x1000, 0x1000);
    gte_stsxy(&p[0]->x0);
    gte_stsz(&z[0]);
    SHADOW_PT(0, 0x1000);
    gte_stsxy(&p[0]->x1);
    gte_stsxy(&p[1]->x0);
    gte_stsz(&z[1]);
    SHADOW_PT(0x1000, 0x1000);
    gte_stsxy(&p[1]->x1);
    gte_stsz(&z[2]);
    SHADOW_PT(-0x1000, 0);
    gte_stsxy(&p[0]->x2);
    gte_stsxy(&p[2]->x0);
    gte_stsz(&z[3]);
    SHADOW_PT(0, 0);
    gte_stsxy(&p[0]->x3);
    gte_stsxy(&p[1]->x2);
    gte_stsxy(&p[2]->x1);
    gte_stsxy(&p[3]->x0);
    gte_stsz(&z[4]);
    SHADOW_PT(0x1000, 0);
    gte_stsxy(&p[1]->x3);
    gte_stsxy(&p[3]->x1);
    gte_stsz(&z[5]);
    SHADOW_PT(-0x1000, -0x1000);
    gte_stsxy(&p[2]->x2);
    gte_stsz(&z[6]);
    SHADOW_PT(0, -0x1000);
    gte_stsxy(&p[2]->x3);
    gte_stsxy(&p[3]->x2);
    gte_stsz(&z[7]);
    SHADOW_PT(0x1000, -0x1000);
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
    SHADOW_ADD(k, 0);
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
    SHADOW_ADD(k, 1);
    k = z[3];
    if (k < z[6]) {
        k = z[6];
    }
    if (k < z[7]) {
        k = z[7];
    }
    SHADOW_ADD(k, 2);
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
    SHADOW_ADD(k, 3);
}

void func_800AAD0C(s32 arg0, s32 arg1) {
    func_80062E3C(arg0, 0x280, 0xDE, 0x330, 0xB0, arg1, 1);
}

/* object matrix facing the joint's direction (in the xz plane) */
void func_800AAD48(WmObjEnt *arg0, WmObjSlot *arg1, MATRIX *arg2, s32 arg3, s32 arg4) {
    MATRIX m;
    s32 *d;
    s32 *s;
    SVECTOR v;
    VECTOR dir;
    VECTOR side;
    VECTOR up;
    VECTOR fwd;

    /* word-wise copy of the joint matrix */
    d = (s32 *)&m;
    s = (s32 *)&((MATRIX *)arg1->unk8->unk20)[arg4];
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
    arg2->t[0] = ((side.vx * arg0->unk4_12 + fwd.vx * arg0->unk4_22) >> 12) + ((MATRIX *)arg1->unk8->unk20)[arg4].t[0];
    arg2->t[1] = ((side.vy * arg0->unk4_12 + fwd.vy * arg0->unk4_22) >> 12) + arg3;
    arg2->t[2] = ((side.vz * arg0->unk4_12 + fwd.vz * arg0->unk4_22) >> 12) + ((MATRIX *)arg1->unk8->unk20)[arg4].t[2];
}

INCLUDE_RODATA("asm/us/ovl_0c/nonmatchings/ovl_0c", D_800A7000);

INCLUDE_ASM("asm/us/ovl_0c/nonmatchings/ovl_0c", func_800AB0C0);

void func_800AC020(s16 arg0, s16 arg1, s16 arg2, s32 arg3, u8 *arg4, u8 *arg5) {
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
    z /= 8;
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
        pan = (sxy.vx + 320) * a / 320;
    } else {
        if (sxy.vx >= 640) {
            sxy.vx = 639;
        }
        pan = (sxy.vx - 320) * a / 320;
        pan += 255 - a;
    }
    *arg4 = pan;
    *arg5 = (0x1000 - z) * 127 / 4096;
}

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

/* old-style definition: func_800ACAFC passes a third (unused) argument */
s32 func_800AC29C(arg0, arg1)
s32 arg0;
s32 *arg1;
{
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

void func_800AC424(Unk800AC424 *arg0) {
    s32 a;
    s32 id;
    s32 t;
    s32 n;
    s32 b;

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
        }
    }
    n = D_8006794C->unk1C->unk87C - arg0->unkC;
    if (arg0->unk10 < n) {
        n = arg0->unk10;
    }
    func_80055E14(a, arg0->unk8 + n - a, 0);
    arg0->unk8 = a;
}

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

void func_800AC7D4(void) {
    SVECTOR v;
    WorldMapObj *o;
    Unk8004AB64 *a;
    WmObjSlot *s;

    D_8007BD28 = 0;
    D_8007BD30 = func_800B8114();
    o = D_8006794C->unk1C->unk8E4.world->unk8;
    o->unk19 = 0;
    a = func_8004AE00();
    if (a != NULL && (s = (WmObjSlot *)func_8001E350(a->unk3B)) != NULL) {
        v.vx = ((MATRIX *)s->unk8->unk20)[1].t[0];
        v.vy = ((MATRIX *)s->unk8->unk20)[1].t[1];
        v.vz = ((MATRIX *)s->unk8->unk20)[1].t[2];
        gte_SetRotMatrix(&D_8006794C->unk1C->unk18);
        gte_SetTransMatrix(&D_8006794C->unk1C->unk18);
        gte_ldv0(&v);
        gte_rtps();
        gte_stsxy(&D_8006794C->unk1C->unk8E4.world->unk8->unkC);
        if ((u16)D_8006794C->unk1C->unk8E4.world->unk8->unkC >= 320 ||
            D_8006794C->unk1C->unk8E4.world->unk8->unkE < 0 ||
            D_8006794C->unk1C->unk8E4.world->unk8->unkE >= 218) {
            D_8006794C->unk1C->unk8E4.world->unk8->unkC = 160;
            D_8006794C->unk1C->unk8E4.world->unk8->unkE = 109;
        }
    }
    D_8006794C->unk1C->unk8E4.world->unk0 &= ~0x400;
    func_800AD180();
    func_800ACE24();
    if (D_8006794C->unk1C->unk81C[0] != D_8006794C->unk1C->unk8E4.world->unk8->unk26 &&
        (D_8006794C->unk1C->unk0 & 0x20000000)) {
        func_800559FC(0, 0xF, 0);
    }
    D_8006794C->unk1C->unk8E4.world->unk8->unk18 = 5;
    func_800AB0C0(0xD000, 0x27C, 0, 0x80, 0x7F);
    func_800AB0C0(0xD000, 0x27B, 0, 0x80, 0x7F);
    func_800AB0C0(0xD000, 0x27A, 0, 0x80, 0x7F);
    D_8006794C->unk1C->unk0 |= 0x2F;
    D_8006794C->unk1C->unk8E4.world->unk0 |= 0x300;
    D_8006794C->unk9 = 0;
    D_80073998[0] |= 0x04000000;
}

void func_800ACAFC(void) {
    WorldMapObj *w;
    WorldMapObj *o;
    GameState *gs;
    GameData *gd;
    s32 size;

    w = D_8006794C->unk1C->unk8E4.world->unk8;
    if (!(w->unk0 & 1)) {
        if (w->unk19 >= 2) {
            if ((u8)func_80053A10(w->unkC, w->unkE)) {
                D_8006794C->unk1C->unk8E4.world->unk8->unk0 |= 1;
            }
        } else {
            w->unk19++;
        }
    }
    gs = D_8006794C;
    gd = gs->unk1C;
    o = gd->unk8E4.world->unk8;
    if (o->unk18 > 0) {
        o->unk18--;
    } else if (o->unk18 == 0) {
        if (o->unk26 != 0xFFFF && o->unk26 != gd->unk81C[0]) {
            func_80022C10(D_8006794C->unk20 + func_800AC29C(o->unk26 | 0x70000, &size, gs), 0);
            if (D_8006794C->unk1C->unk0 & 0x20000000) {
                func_800AB0C0(0x800, D_8006794C->unk1C->unk81C[0], 0, 0, 0);
            }
            func_800AB0C0(0x700, D_8006794C->unk1C->unk8E4.world->unk8->unk26, 0, 0, 0);
            D_8006794C->unk1C->unk8E4.world->unk8->unk18 = -1;
        } else {
            func_80022C10(D_8006794C->unk20 + func_8001E218(0, 0x1000B)->unk4, 0);
            D_8006794C->unk1C->unk8E4.world->unk8->unk18 = -2;
        }
    } else if (o->unk18 == -1) {
        if (func_800AB0C0(0xC00, 0, 0, 0, 0) == 0) {
            func_80022C10(D_8006794C->unk20 + func_8001E218(0, 0x1000B)->unk4, 0);
            func_800AB0C0(0, D_8006794C->unk1C->unk8E4.world->unk8->unk26, 0, 0, 0);
            func_800559C8(0, 0x7F);
            D_8006794C->unk1C->unk8E4.world->unk8->unk18 = -2;
        }
    } else if (o->unk18 == -2 && (o->unk0 & 1)) {
        gd->unk0 &= ~0x2F;
        D_8006794C->unk1C->unk8E4.world->unk0 &= ~0x300;
        D_80073998[0] &= ~0x04000000;
        D_8006794C->unk0 |= 0x1000;
    }
}

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

void func_800ACEEC(void) {
    u8 buf[8];
    s32 i;
    u32 a;

    D_8006794C->unk1C->unk0 ^= 0x100;
    if (D_8006794C->unk1C->unk0 & 0x100) {
        func_8005562C(0);
        func_80023F60(1);
        for (i = 1; i >= 0; i--) {
            D_800739B0[i].unk1 = 0;
            D_800739B0[i].unk0 = 0;
        }
        ClearOTag(&D_800CE0E0, 1);
        func_80027834((320 - D_800686D0[235].unk4) / 2, (218 - D_800686D0[235].unk5) / 2, &D_800CE0E0, 0xEB, 0, buf, D_800CE098);
        setlen(&D_800CE0C8, 3);
        D_800CE0C8.code = 0x62;
        setRGB0(&D_800CE0C8, 0x60, 0x60, 0x60);
        setXY0(&D_800CE0C8, 0, 0);
        setWH(&D_800CE0C8, 320, 218);
        setlen(&D_800CE0D8, 1);
        a = getaddr(&D_800CE0E0);
        D_800CE0D8.code[0] = 0xE1000240;
        setaddr(&D_800CE0C8, a), setaddr(&D_800CE0E0, &D_800CE0C8);
        addPrim(&D_800CE0E0, &D_800CE0D8);
        DrawOTag(&D_800CE0E0);
        D_8006794C->unk1C->unk0 |= 0x20;
    } else {
        func_80023F60(0);
        func_80055698(0);
        D_8006794C->unk1C->unk0 &= ~0x20;
    }
}

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

void func_800AD39C(void) {
    SVECTOR r;
    VECTOR v;
    MATRIX m;
    s32 t;
    s32 a;

    if (D_800CE202 == 0) {
        func_800AE280();
    }
    D_8006794C->unk1C->unkA = D_800CE1E8.unk0[0];
    D_800CE0F0->unkC &= 0xFFF;
    D_800CE1C4 = D_800CE1E8.unk0[4];
    if ((u8)D_800CE3C4 == 8 || (u8)D_800CE3C4 == 9) {
        t = D_800CE3DC->unk14.vy + 0x1518;
        D_800CE1C4 = D_800CE1E8.unk0[4] - (t > 0 ? t >> 3 : t / 6);
    }
    D_800CE1C8.vx = func_8004BB80(D_800CE0F0->unkC) * (D_800CE1E8.unk0[1] * D_800CCA24 / 4096) >> 12;
    D_800CE1C8.vy = D_800CE1E8.unk0[2];
    D_800CE1C8.vz = func_8004BB7C(D_800CE0F0->unkC) * (D_800CE1E8.unk0[1] * D_800CCA24 / 4096) >> 12;
    if (D_800CE202 == 0) {
        func_800AD738();
    }
    D_800CE190.vx = D_800CE198[0] - D_800CE108[0];
    D_800CE190.vy = D_800CE198[1] - D_800CE108[1];
    D_800CE190.vz = D_800CE198[2] - D_800CE108[2];
    func_8001D898(&r, 0, 8);
    r.vz = D_800CE100;
    r.vy = D_800CE0F0->unkC;
    func_80030AF4(&r, &m);
    m.t[0] = 0;
    m.t[1] = 0;
    m.t[2] = 0;
    ApplyMatrixLV(&m, &D_800CE1B0, &v);
    func_800AE880(D_800CF448, &D_800CE190, &D_800CE0F8, &v);
    D_800CE1D0[1] = 0;
    D_800CE1D0[0] = D_800CE108[0] - D_800CE198[0];
    D_800CE1D0[2] = D_800CE108[2] - D_800CE198[2];
    if (D_800CE1D0[0] > 0xFFFF) {
        D_800CE1D0[0] -= 0x60000;
    }
    if (D_800CE1D0[2] > 0xFFFF) {
        D_800CE1D0[2] -= 0x50000;
    }
    if (D_800CE1D0[0] < -0xFFFF) {
        D_800CE1D0[0] += 0x60000;
    }
    if (D_800CE1D0[2] < -0xFFFF) {
        D_800CE1D0[2] += 0x50000;
    }
    a = ratan2(D_800CE1D0[2], D_800CE1D0[0]);
    D_800CE1D0[0] = func_8004BB7C(a);
    D_800CE1D0[2] = func_8004BB80(a);
    VectorNormal((VECTOR *)D_800CE1D0, (VECTOR *)D_800CE1D0);
    if (*D_800CF430 < 0x137E && ((D_800CCCC0[D_800CE3DC->unk11].unk8 & 0x3F00) >> 8) == 0xC) {
        D_800CE0F0->unk8 = 1;
    }
}

INCLUDE_ASM("asm/us/ovl_0c/nonmatchings/ovl_0c", func_800AD738);

void func_800AE280(void) {
    s32 k;
    s32 m;
    Unk800CE1E8 tmp;

    k = D_800CCA28[func_800B7A14(0xC0)];
    m = D_800CE3C8->unk11;
    if (D_800CCA20 != -1) {
        k = D_800CCA20;
    }
    if (D_800CE0F0->unk8 != 0) {
        D_800CE0F0->unk4 = -0x80;
    }
    D_800CE0F0->unk2 += D_800CE0F0->unk4;
    if (D_800CE0F0->unk2 <= 0) {
        D_800CE0F0->unk2 = 0;
        D_800CE0F0->unk4 = 0;
        D_800CE0F0->unk8 = 0;
    }
    if (D_800CE0F0->unk2 >= 0x1000) {
        D_800CE0F0->unk2 = 0x1000;
        D_800CE0F0->unk4 = 0;
        D_800CE0F0->unk8 = 0;
    }
    if (D_800CE188 != 0) {
        D_800CE1A8 += D_800CE188;
        if (D_800CE1A8 > 0x1000) {
            D_800CE1A8 = 0x1000;
        }
        func_800AE4C8(-D_800CE3DC->unk14.vy, D_800CE1C0, k, D_800CE0F0->unk2, &tmp);
        D_800CE1E8.unk0[0] = func_800B822C(D_800CE1F8.unk0[0], tmp.unk0[0], D_800CE1A8, 9);
        D_800CE1E8.unk0[1] = func_800B822C(D_800CE1F8.unk0[1], tmp.unk0[1], D_800CE1A8, 0xF);
        D_800CE1E8.unk0[2] = func_800B822C(D_800CE1F8.unk0[2], tmp.unk0[2], D_800CE1A8, 0xE);
        D_800CE1E8.unk0[3] = func_800B822C(D_800CE1F8.unk0[3], tmp.unk0[3], D_800CE1A8, 0xE);
        D_800CE1E8.unk0[4] = func_800B822C(D_800CE1F8.unk0[4], tmp.unk0[4], D_800CE1A8, 0xE);
        if (D_800CE1A8 >= 0x1000) {
            D_800CE188 = 0;
        }
    } else {
        func_800AE4C8(-D_800CE3DC->unk14.vy, m, k, func_8004BB80(D_800CE0F0->unk2 / 4), &D_800CE1E8);
    }
}

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

s32 func_800AE7F0() {
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

void func_800AE880(MATRIX *arg0, SVECTOR *arg1, SVECTOR *arg2, VECTOR *arg3) {
    VECTOR d;
    VECTOR x;
    VECTOR y;
    VECTOR z;

    d.vx = arg2->vx - arg1->vx;
    d.vy = arg2->vy - arg1->vy;
    d.vz = arg2->vz - arg1->vz;
    VectorNormal(&d, &z);
    if (z.vz == arg3->vz) {
        z.vz++;
    }
    gte_ldopv1(&z);
    gte_ldopv2(arg3);
    gte_op12();
    gte_stlvnl(&d);
    VectorNormal(&d, &x);
    gte_ldopv1(&z);
    gte_ldopv2(&x);
    gte_op12();
    gte_stlvnl(&d);
    VectorNormal(&d, &y);
    arg0->m[0][0] = x.vx;
    arg0->m[0][1] = x.vy;
    arg0->m[0][2] = x.vz;
    arg0->m[1][0] = y.vx;
    arg0->m[1][1] = y.vy;
    arg0->m[1][2] = y.vz;
    arg0->m[2][0] = z.vx;
    arg0->m[2][1] = z.vy;
    arg0->m[2][2] = z.vz;
    gte_SetRotMatrix(arg0);
    gte_ldv0(arg1);
    gte_rtv0();
    gte_stlvnl(arg0->t);
    arg0->t[0] = -arg0->t[0];
    arg0->t[1] = -arg0->t[1];
    arg0->t[2] = -arg0->t[2];
}

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
    D_800CE3D8 = NULL;
    D_800CE3D0 = NULL;
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

__asm__(".section .rodata\n\t.align 3\n\t.section .text");

void func_800AEC74(void) {
    s32 id;
    s32 k;
    WmPalAnim *e;
    u16 *pal;
    u16 *dst;
    Unk16D8 *n;
    Unk8004AB64 *a;
    s32 c;
    s32 i;
    s32 j;

    id = 0;
    k = 0;
    e = D_8006794C->unk1C->unk8E4.world->unk8->unk28;
    pal = func_800B8388(D_800CF428, 1);
    func_800B36AC();
    for (n = func_8004AE9C(); n != NULL; n = n->unk0) {
        a = n->unk4;
        if (a->unkE == 1 && (a->unkF & 1) && a->unk5 == 4) {
            if ((a->unk11 >= 3 && a->unk11 < 8)) {
                id = a->unk34;
                switch (a->unk11) {
                case 3:
                    k = 0;
                    break;
                case 4:
                    k = 1;
                    break;
                case 5:
                    k = 2;
                    break;
                case 6:
                    k = 3;
                    break;
                case 7:
                    k = 4;
                    break;
                }
                if (D_800CE280 != 0) {
                    k = D_800CE280 - 1;
                }
            }
        }
    }
    c = 0;
    for (i = 0; i < D_8006794C->unk1C->unk8E4.world->unk8->unk1A; i++, e++) {
        if (e->unk0 == id) {
            dst = e->unk8;
            switch (c) {
            case 0:
                for (j = 0; j < 16; j++) {
                    *dst++ = pal[k * 48 + j + 32];
                }
                break;
            case 1:
                for (j = 0; j < 16; j++) {
                    *dst++ = pal[k * 48 + j + 16];
                }
                break;
            case 2:
                for (j = 0; j < 16; j++) {
                    *dst++ = pal[k * 48 + j + 0];
                }
                break;
            }
            c++;
        }
    }
}

/* jump table padding emitted by the original toolchain */
__asm__(".section .rodata\n\t.word 0\n\t.section .text");

/* find the world map actors (vehicles) and sync the camera with the player */
void func_800AEF10(void) {
    Unk16D8 *n;
    Unk8004AB64 *a;

    for (n = func_8004AE9C(); n != NULL; n = n->unk0) {
        if (n->unk4->unk5 == 4) {
            a = n->unk4;
            switch (a->unk11) {
            case 1:
            case 2:
                D_800CE3D8 = a;
                break;
            case 3:
            case 4:
            case 5:
            case 6:
            case 7:
                D_800CE268 = a;
                break;
            case 8:
                D_800CE3D0 = a;
                break;
            case 9:
                D_800CE3D0 = a;
                break;
            case 10:
                D_800CE3D0 = a;
                break;
            }
            if (a == func_8004AE00()) {
                if (D_8006794C->unkA == 1 && D_8006794C->unk1C->unk892 == 0) {
                    if (D_800CE3C8->unk10 != 0) {
                        D_800CE0F0->unkE = 0x800;
                    } else {
                        D_800CE0F0->unkE = 0;
                    }
                    D_800CE0F0->unkC = a->unk14.unk22 + D_800CE0F0->unkE;
                }
                D_800CE3CC = a->unk14.unk22;
            }
        }
    }
    D_8006794C->unk1C->unk890 = 0;
    D_800CF47C = 1;
}

void func_800AF08C(void) {
    SVECTOR n;
    VECTOR a;
    VECTOR b;
    VECTOR dv;
    SVECTOR s;
    s32 z;
    s32 base;
    s32 d;
    s32 ang;
    Unk16D8 *e;
    Unk8004AB64 *act;
    u8 f;

    VectorNormalS((VECTOR *)D_800CE1D0, &n);
    base = (-ratan2(n.vz, n.vx) + 0x400) % 4096;
    func_80064CDC(10, 100);
    for (e = func_8004AE9C(); e != NULL; e = e->unk0) {
        act = e->unk4;
        if (act->unk5 == 4 && act->unkE == 1 && (act->unkF & 1) && (u8)func_800BBE24((VECTOR *)&act->unk14, &a, &b, &z)) {
            f = 1;
            dv.vx = act->unk14.vx - (D_800CE198[0] - n.vx);
            dv.vz = act->unk14.vz - (D_800CE198[2] - n.vz);
            if (dv.vx > D_800D32D8 - 0x8000) {
                dv.vx -= D_800D32D8;
            }
            if (dv.vz > D_800D32DC - 0x8000) {
                dv.vz -= D_800D32DC;
            }
            if (dv.vx < 0x8000 - D_800D32D8) {
                dv.vx = D_800D32D8 + dv.vx;
            }
            if (dv.vz < 0x8000 - D_800D32DC) {
                dv.vz = D_800D32DC + dv.vz;
            }
            ang = -ratan2(dv.vz, dv.vx) + 0x400;
            d = ang % 4096;
            d = d - base;
            if (d < -0x800) {
                d += 0x1000;
            }
            if (d > 0x800) {
                d -= 0x1000;
            }
            if (((d < 0 ? -d : d) < 400 || func_800AE7F0(ang) != 0) && (b.vz >= 2001 && b.vz < 29000) && b.vx > -20000 && b.vx < 20000 && b.vy > -20000 && b.vy < 20000 && a.vx > -29000 && a.vx < 29000 && a.vz > -29000 && a.vz < 29000) {
                s.vx = a.vx;
                s.vy = a.vy;
                s.vz = a.vz;
                func_800AF414(act, b.vz >> 3);
                func_800AF66C(act, &s, z);
                if (act->unk11 == 8) {
                    f = 0;
                }
                if (act->unk11 == 1 || act->unk11 == 2) {
                    if ((u8)D_800CE3C4 >= 1 && (u8)D_800CE3C4 <= 6) {
                        f = 0;
                    }
                }
                if (D_800CCCC0[act->unk11].unkC < 4 && f) {
                    func_800AF8C0(act, &s, b.vz);
                }
            }
        }
    }
    func_80064C24(0);
    SetFarColor(0, 0, 0);
}

void func_800AF414(Unk8004AB64 *arg0, s32 arg1) {
    s16 col[3];
    s32 t;
    s32 f;
    s32 a;
    s32 d;
    s32 i;
    s32 id;
    WmPalAnim *e;
    RECT *r;

    id = arg0->unk34;
    e = D_8006794C->unk1C->unk8E4.world->unk8->unk28;
    t = -arg0->unk14.vy;
    t -= 0xC1F;
    t <<= 12;
    t /= 2176;
    if (t > 0x1000) {
        t = 0x1000;
    }
    if (t < 0) {
        t = 0;
    }
    a = func_800B81C0(D_800D3FA0.unk5E0[0].unk30[1].unk0, D_800D3FA0.unk5E0[0].unk30[0].unk0, t);
    col[0] = func_800B81C0(D_800D3FA0.unk5E0[0].unk30[1].unk4[0], D_800D3FA0.unk5E0[0].unk30[0].unk4[0], t);
    col[1] = func_800B81C0(D_800D3FA0.unk5E0[0].unk30[1].unk4[1], D_800D3FA0.unk5E0[0].unk30[0].unk4[1], t);
    col[2] = func_800B81C0(D_800D3FA0.unk5E0[0].unk30[1].unk4[2], D_800D3FA0.unk5E0[0].unk30[0].unk4[2], t);
    SetFarColor((u8)col[0], (u8)col[1], (u8)col[2]);
    f = (arg1 * 8 + (d = a - 0x8000)) >> 2;
    if (f < 0) {
        f = 0;
    }
    if (f >= 0x1000) {
        f = 0xFFF;
    }
    for (i = 0; i < D_8006794C->unk1C->unk8E4.world->unk8->unk1A; i++, e++) {
        if (e->unk0 == id) {
            r = (RECT *)D_800CF404;
            if (f < 0) {
                f = 0;
            }
            if (f > 0x1000) {
                f = 0x1000;
            }
            func_800B3998(e->unk8, e->unk28[*D_800CF45C], f, D_800D45D4);
            r->x = e->unk4;
            r->y = e->unk6;
            r->w = 16;
            r->h = 1;
            LoadImage(r, (u32 *)e->unk28[*D_800CF45C]);
        }
    }
}

s32 func_800AF66C(Unk8004AB64 *arg0, SVECTOR *arg1) {
    Unk800CCCC0 *c;
    WmObjSlot *s;
    Unk800C95C8 *m;
    WmAnimHdr *h;
    SVECTOR r2;
    SVECTOR r;
    s32 t;
    void *t2;
    s32 n;
    s32 i;

    c = &D_800CCCC0[arg0->unk11];
    s = (WmObjSlot *)func_8001E350(arg0->unk3B);
    m = s->unk8;
    func_8001D898(&r, 0, 8);
    r.vx = s->unkC->unk14.unk20;
    r.vy = s->unkC->unk14.unk22;
    r.vz = s->unkC->unk24;
    r2 = r;
    func_800316D8(&r2, &m->unk24);
    m->unk24.t[0] = arg1->vx;
    m->unk24.t[1] = arg1->vy;
    m->unk24.t[2] = arg1->vz;
    func_800C9484(m);
    m->unkC = h = func_8001C7FC(4, s->unkC->unk36 | 0x30000, 0);
    if (h != NULL) {
        if (h->unkC >= 0) {
            t = h->unk10;
            h->unkC = (s32)h + h->unkC;
            if (t != 0) {
                h->unk10 = (s32)m->unkC + t;
            }
        }
        func_800C96AC(m, s->unkC->unk3A);
        if (arg0->unk11 != 0xE) {
            n = m->unk8->unk3;
            if (c->unk0 != 0 && c->unkC > 0) {
                m->unk5A = arg1->vy - c->unkC + 100;
                if (m->unk1C != NULL) {
                    m->unk5A = m->unk1C->unk5A;
                }
                for (i = 0; i < n; i++) {
                    func_800C9FCC(m, i);
                    func_800CA1D8(m, i);
                }
            } else {
                for (i = 0; i < n; i++) {
                    func_800C7914(m, i);
                    func_800C709C(m, i);
                }
            }
        }
    } else {
        func_80015C38("Illigal Animation Data\n");
    }
    return 0;
}

/* shadow under world map object arg0 at arg1; arg2 = distance from the camera */
void func_800AF8C0(Unk8004AB64 *arg0, SVECTOR *arg1, s32 arg2) {
    Unk800CCCC0 *c;
    s32 k;
    s32 d;
    s32 sx;
    s32 sz;
    s32 f;
    s32 a;
    s32 s;
    u32 *ot;

    c = &D_800CCCC0[arg0->unk11];
    k = arg0->unk3B;
    sx = 0x1000;
    if (arg0->unk11 != 0xE) {
        d = c->unkE - arg0->unk14.vy;
        arg1->vy += d;
        arg1->vy -= d / 8;
        sz = sx - d / 2;
        sx -= d / 4;
        D_800CF442 = -c->unk1 * 4 - (s8)c->unk7;
        if (c->unk1 == 0) {
            return;
        }
        sx = (c->unk1 * sx) >> 11;
        sz = (c->unk2 * sz) >> 12;
        f = (0x8000 - arg2) >> 3;
        f *= D_800D45DC;
        f >>= 12;
        if (f < 0) {
            f = 0;
        }
        if (f >= 0x1000) {
            f = 0xFFF;
        }
        sz *= f;
        sz >>= 12;
        if (sz >= 0x100) {
            sz = 0xFF;
        }
        if (sz < 0) {
            sz = 0;
        }
        D_8006794C->unk1C->unk8E4.world->unk8->unk1C[k].unk1 = sx * 14;
        D_8006794C->unk1C->unk8E4.world->unk8->unk1C[k].unk2 = sx * 12;
        D_8006794C->unk1C->unk8E4.world->unk8->unk1C[k].unk0 = sz;
        ot = D_8006794C->unk1C->unk10[D_8006794C->unk1C->unk8];
        D_8006794C->unk1C->unk10[D_8006794C->unk1C->unk8] = ot + D_800CF442;
        func_800AA418(D_8006794C->unk1C->unk8E4.world->unk8->unk1C, k, arg1->vy, c->unk6);
        D_8006794C->unk1C->unk10[D_8006794C->unk1C->unk8] = ot;
    } else {
        s = 0x7D0;
        if (D_800CCE4B != 0) {
            s = 0xAF0;
        }
        f = (0x7060 - arg2) >> 2;
        f *= D_800D4754;
        f >>= 12;
        if (f < 0) {
            f = 0;
        }
        if (f >= 0x1000) {
            f = 0xFFF;
        }
        a = (c->unk2 * s) >> 12;
        a *= f;
        a >>= 12;
        if (a >= 0x100) {
            a = 0xFF;
        }
        if (a < 0) {
            a = 0;
        }
        D_8006794C->unk1C->unk8E4.world->unk8->unk1C[k].unk1 = 0x302;
        D_8006794C->unk1C->unk8E4.world->unk8->unk1C[k].unk2 = 0x294;
        D_8006794C->unk1C->unk8E4.world->unk8->unk1C[k].unk0 = a;
        ot = D_8006794C->unk1C->unk10[D_8006794C->unk1C->unk8];
        D_800CF442 = -0x46;
        D_8006794C->unk1C->unk10[D_8006794C->unk1C->unk8] -= 0x46;
        func_800AA418(D_8006794C->unk1C->unk8E4.world->unk8->unk1C, k, arg1->vy, c->unk6);
        D_8006794C->unk1C->unk10[D_8006794C->unk1C->unk8] = ot;
    }
}

void func_800AFCE0(void) {
    s32 tmp;
    s32 h;
    s32 cell;
    Unk16D8 *e;
    Unk8004AB64 *act;
    Unk800CCCC0 *cc;

    D_800CCE54 = 0;
    D_800CE3C0 = 0;
    D_800CE278 = 0;
    D_800CE3E0 = 0;
    if (func_8004AE00() != NULL) {
        D_800CE3DC = func_8004AE00();
        D_800CE3DC->unk40.vx = D_800CE3DC->unk14.vx;
        D_800CE3DC->unk40.vy = D_800CE3DC->unk14.vy;
        D_800CE3DC->unk40.vz = D_800CE3DC->unk14.vz;
    }
    for (e = func_8004AE9C(); e != NULL; e = e->unk0) {
        act = e->unk4;
        cc = &D_800CCCC0[act->unk11];
        if (act->unk5 == 4 && act->unkE == 1 && (act->unkF & 1)) {
            switch (act->unk11) {
            case 7:
                cc->unk3 = D_800CE3C4 == 6;
                break;
            case 11:
                D_800CE270 = act;
                break;
            }
            if (act == D_800CE3DC && func_8004AE84() != 0) {
                func_800B0378(cc);
            } else {
                cc = &D_800CCCC0[act->unk11];
                func_800BE748((VECTOR *)&act->unk14, &cc->unk8, &tmp, &D_800CE288[cc->unk5], (u16 *)&h);
                if (act->unk11 == 8) {
                    h = 0;
                }
                cc->unkE = h;
            }
            func_800B01F0(act);
        }
    }
    func_80064C24(0);
    switch (D_800CE3C8->unk0) {
    case 0:
    case 3:
        if (D_800D33F4 != 0) {
            func_800B2C24();
        }
        break;
    case 1:
    case 2:
        func_800B2E54();
        break;
    }
    if ((u8)D_800CE3C4 >= 1 && (u8)D_800CE3C4 <= 6) {
        D_800CCCC0[1].unkC = D_800CCCC0[D_800CE3DC->unk11].unkC;
    }
    if (D_800CE268 != NULL) {
        if (D_800CE268->unk24 < 0) {
            D_800CE268->unk24 += 10;
        }
        if (D_800CE268->unk24 > 0) {
            D_800CE268->unk24 -= 10;
        }
        if (D_800CE268->unk24 > -10 && D_800CE268->unk24 < 10) {
            D_800CE268->unk24 = 0;
        }
    }
    if (D_800CE3D0 != NULL) {
        if (D_800CE3C8->unk0 != 1) {
            if (D_800CE3D0->unk24 < 14) {
                D_800CE3D0->unk24 += 10;
            }
            if (D_800CE3D0->unk24 >= 15) {
                D_800CE3D0->unk24 -= 10;
            }
            if (D_800CE3D0->unk24 >= 5 && D_800CE3D0->unk24 < 24) {
                D_800CE3D0->unk24 = 14;
            }
        } else if (D_800CE209 == 0) {
            if (D_800CE3D0->unk24 < 14) {
                D_800CE3D0->unk24 += 16;
            }
            if (D_800CE3D0->unk24 >= 15) {
                D_800CE3D0->unk24 -= 16;
            }
            if (D_800CE3D0->unk24 >= -1 && D_800CE3D0->unk24 < 30) {
                D_800CE3D0->unk24 = 14;
            }
        }
    }
    if (func_8004AE00() != NULL) {
        if (D_800CE3E0 == 0 || D_800CE3C0 < 300) {
            if (D_800CE100 > 0) {
                D_800CE100 -= 10;
                if (D_800CE100 < 0) {
                    D_800CE100 = 0;
                }
            }
            if (D_800CE100 < 0) {
                D_800CE100 += 10;
                if (D_800CE100 > 0) {
                    D_800CE100 = 0;
                }
            }
        }
        cc = &D_800CCCC0[D_800CE3DC->unk11];
        if ((cc->unk8 & 0xC000) && D_800CCE4D != 0) {
            func_800BC130(D_800CE3DC->unk14.vx, D_800CE3DC->unk14.vz, &cell);
            func_8004B158(cell % D_800D32F0, cell / D_800D32F0, (cc->unk8 & 0xC000) >> 14);
        }
    }
}

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

u8 func_800B0D6C(VECTOR *arg0, s32 arg1, s32 arg2, s32 arg3, u32 *arg4, s32 *arg5) {
    VECTOR v;
    u32 cell;
    s32 h;
    Unk800CE3C8 *mode;
    Unk800CE288 *c;
    u8 ok;
    s32 k;
    s32 ang;
    u8 a;
    u8 b;
    u8 x;
    u8 y;

    k = D_800CCCC0[D_800CE3DC->unk11].unk5;
    mode = &D_800CCB70[arg2];
    v = *arg0;
    c = &D_800CE288[k];
    v.vx += func_8004BB80(D_800CE3CC + (ang = arg1 + 0x800)) * arg3 >> 12;
    v.vz += func_8004BB7C(D_800CE3CC + ang) * arg3 >> 12;
    if ((u8)D_800CCCC0[D_800CE3DC->unk11].unk8 >> 2 == 0x34 && D_800CE3C8->unk0 != 1) {
        func_800BE748(&v, &cell, &h, NULL, (u16 *)&v.vy);
    } else {
        func_800BE748(&v, &cell, &h, c, (u16 *)&v.vy);
    }
    if (h >= 0) {
        ok = func_800B3340(mode->unk14, cell);
        if (ok) {
            if (mode->unk1 != 0) {
                a = func_800B3340(D_800CCB20, cell);
                b = func_800B3340(D_800CCB18, cell);
                x = func_800B3340(D_800CCB20, D_800CCCC0[D_800CE3DC->unk11].unk8);
                y = func_800B3340(D_800CCB18, D_800CCCC0[D_800CE3DC->unk11].unk8);
                if (b & x) {
                    ok = 0;
                }
                if (y & a) {
                    ok = 0;
                }
            }
            if (ok) {
                if (arg0 != NULL) {
                    *arg0 = v;
                }
                if (arg5 != NULL) {
                    *arg5 = h;
                }
                if (arg4 != NULL) {
                    *arg4 = cell;
                }
                return 1;
            }
        }
    }
    return 0;
}

u8 func_800B105C(ActorPos *out) {
    Unk8004AB64 *act;
    s32 d[2];
    ActorPos v;
    u32 cell;
    u8 ok;
    u8 r;
    s32 a;
    s32 i;
    s32 j;
    s32 t;
    s32 k;
    s32 x;
    s32 z;

    func_8001D898(&v, 0, sizeof(v));
    ok = 0;
    if (D_800D32FC < 0x16 && ((u8)D_800CCCC0[D_800CE3DC->unk11].unk8 >> 2) == 0x17) {
        return 0;
    }
    D_800CE209 = 0;
    act = D_800CE3DC;
    D_800D30C4 = 1;
    v = act->unk14;
    switch (D_800CE3C4) {
    case 8:
    case 9:
        x = (D_800CE3DC->unk14.vx >> 3) - 0x17FC;
        z = (D_800CE3DC->unk14.vz >> 3) + 0x3B48;
        if (SquareRoot0(x * x + z * z) < 0xB4) {
            return 0;
        }
        if (!func_800B0D6C((VECTOR *)&v, 0, 0, 0, &cell, 0)) {
            return 0;
        }
        t = (u8)cell >> 2;
        if (t >= 0x2D && (t < 0x2F || t == 0x34)) {
            return 0;
        }
        break;
    case 7:
        if (!func_800B0D6C((VECTOR *)&v, 0, 10, 0, &cell, 0)) {
            return 0;
        }
        if (((u8)cell >> 2) != 0x35) {
            return 0;
        }
        break;
    default:
        if (!func_800B0D6C((VECTOR *)&v, 0, 0, 0, &cell, 0)) {
            return 0;
        }
        if (((u8)cell >> 2) == 0x34) {
            return 0;
        }
        break;
    }
    D_800D30C4 = 0;
    for (a = 0; a < 0x800; a += 0x100) {
        d[0] = (a + 0xC00) % 0x1000;
        d[1] = -a + 0xC00;
        for (i = 0; i < 2; i++) {
            r = 1;
            if (ok) {
                continue;
            }
            for (j = 1; j < 9; j++) {
                t = D_800CE3C8->unkE * j / 8;
                v = D_800CE3DC->unk14;
                r &= func_800B0D6C((VECTOR *)&v, d[i], 0, t, &cell, 0);
            }
            ok = r;
            if (ok) {
                switch (D_800CE3C4) {
                case 0:
                    if (abs(v.vy - D_800CE3DC->unk14.vy) >= 0x1F5) {
                        ok = 0;
                    }
                    break;
                case 7:
                    if (func_800B3340(D_800CCB18, cell)) {
                        ok = 0;
                    }
                    break;
                case 8:
                case 9:
                    if (abs(v.vy - func_800B7A14(0xC2)) >= 0x15F) {
                        ok = 0;
                    }
                    break;
                }
            }
        }
        if (ok) {
            k = (u8)cell >> 2;
            switch (k) {
            case 0x24:
            case 0x25:
            case 0x26:
                k = 0;
                break;
            case 0x35:
                k = 1;
                break;
            case 0x36:
                k = 2;
                break;
            case 0x37:
                k = 3;
                break;
            case 0x38:
                k = 4;
                break;
            case 0x39:
                k = 5;
                break;
            case 0x33:
                k = 6;
                break;
            case 0x30:
                k = 7;
                break;
            default:
                k = 8;
                break;
            }
            v.vy += D_800CCB28[1][k];
            *out = v;
            return 1;
        }
    }
    return 0;
}

void func_800B153C(void) {
    s8 old;
    Unk800CE3C8 *p;
    Unk800CE3C8 *prev;

    func_8004B20C(0);
    D_800CE3BC = 0;
    D_800CE276 = 0;
    func_800B3638();
    old = D_800CE3C4;
    prev = D_800CE3C8;
    D_800CE3C4 = D_8007AEF0.b[0xBE];
    D_800CE3C8 = p = &D_800CCB70[D_800CE3C4];
    if (old == 7) {
        func_800C48F4(prev->unkC);
    }
    if (p->unkC != 0) {
        func_800C4718(p->unkC, 0);
        if (D_800CE3C4 != 7) {
            func_800C4850(p->unkC, 0x3C, 0x3F);
        }
        if ((u8)(D_800CE3C4 - 8) < 2) {
            func_800C4718(0x26, 0);
        }
        D_800CE0F0->unk8 = 1;
    }
    if (prev == NULL || prev->unkB != p->unkB) {
        func_800C42B0(p->unkB);
    }
    if (D_800CE3C4 == 0 && (u8)(old - 1) < 6) {
        D_800CE3CC = (D_800CE268->unk14.unk22 + 0x400) & 0xFFF;
    }
    if ((u8)(D_800CE3C4 - 1) < 6 && old == 0) {
        D_800CE3CC = D_800CE268->unk14.unk22 & 0xFFF;
    }
    if (D_800CCB70[old].unk8 == D_800CCB70[D_800CE3C4].unk8) {
        func_800AE784(p->unk11, 0x200);
    } else {
        func_800AE784(p->unk11, 0x40);
    }
    D_800CE1F2 = D_800CE3C8->unk8 == 0;
    func_800AFCE0();
    if (D_800CCE50 & 0x20) {
        func_80015C38("w_movementChange : %d -> %d\n", old, D_800CE3C4);
    }
}

void func_800B17B4(void) {
    s32 x;
    s32 z;
    s32 ang;
    u16 mv;
    s32 a;
    s16 cur;
    u32 k;
    s32 rx;
    s32 lo;
    s32 t;

    func_800B3398(&x);
    func_800B345C(&z);
    z = -z;
    t = -ratan2(z, x) + 0x400;
    D_800CE25D = 0;
    ang = t & 0xFFF;
    D_800CE26D = 0;
    D_800CE209 = 0;
    mv = ((x | z) != 0) << 12;
    if (func_800B8498() & 0x40) {
        D_800CE26D = 1;
    } else {
        D_800CE26D = 0;
    }
    if (func_800B8498() & 0x100000) {
        D_800CE25D = 1;
        D_800CE3BE = 1;
        D_800CE0F0->unkC -= 0x20;
    }
    if (func_800B8498() & 0x200000) {
        D_800CE25D = 1;
        D_800CE3BE = 1;
        D_800CE0F0->unkC += 0x20;
    }
    D_800CE0F0->unkC &= 0xFFF;
    if (mv != 0 || D_800CE25D != 0) {
        D_800CE0F0->unk0 = func_8004BB80(ang);
        if (mv != 0) {
            D_800CE3BE = 0;
            if ((u32)(ang - 0x600) <= 0x400) {
                D_800CE0F0->unkE = 0x800;
            } else {
                D_800CE0F0->unkE = 0;
            }
            D_800CE3CC = (D_800CE0F0->unkC + ang) & 0xFFF;
            D_800CE3C0 = mv * D_800CE3C8->unk2 >> 12;
        }
    }
    a = D_800CE3CC;
    cur = D_800CE3DC->unk14.unk22;
    if (a - cur > 0x800) {
        a -= 0x1000;
    }
    if (a - cur < -0x800) {
        a += 0x1000;
    }
    a -= cur;
    if ((x | z) != 0) {
        D_800CE3DC->unk14.unk22 = (D_800CE3DC->unk14.unk22 + a / 3) & 0xFFF;
    }
    if (D_800CE25E != 0 && mv != 0) {
        if (func_800B3340_inline(D_800CCB20, (u8)D_800CCCC0[D_800CE3DC->unk11].unk8 >> 2)) {
            func_800C6F00(D_800CE3DC->unk14.vx, -100, D_800CE3DC->unk14.vz, 22, 24000);
        }
        if (!(D_800CF4B0 & 3)) {
            k = (u8)D_800CCCC0[D_800CE3DC->unk11].unk8 >> 2;
            if (k == 0x1E || k == 0x22 || k == 0x23) {
                func_800C6F00(D_800CE3DC->unk14.vx, D_800CE3DC->unk14.vy, D_800CE3DC->unk14.vz, 7, 0x900);
            }
            if (((u8)D_800CCCC0[D_800CE3DC->unk11].unk8 >> 2) == 0x29) {
                func_800C6F00(D_800CE3DC->unk14.vx, D_800CE3DC->unk14.vy, D_800CE3DC->unk14.vz, 27, 0x2000);
            }
        }
        if (!(D_800CF4B0 & 1) && (u32)(((u8)D_800CCCC0[D_800CE3DC->unk11].unk8 >> 2) - 0x24) < 3) {
            if (D_8007B720 >= D_8007B92D) {
                func_8004BF20();
            }
            rx = D_800CE3DC->unk40.vx + *D_8007B720++ - 0x80;
            if (D_8007B720 >= D_8007B92D) {
                func_8004BF20();
            }
            lo = *D_8007B720++;
            func_800C6F00(rx, D_800CE3DC->unk40.vy, D_800CE3DC->unk40.vz + lo - 0x80, 16, 0x6D80);
            if (D_8007B720 >= D_8007B92D) {
                func_8004BF20();
            }
            rx = D_800CE3DC->unk40.vx + *D_8007B720++ - 0x80;
            if (D_8007B720 >= D_8007B92D) {
                func_8004BF20();
            }
            lo = *D_8007B720++;
            func_800C6F00(rx, D_800CE3DC->unk40.vy, D_800CE3DC->unk40.vz + lo - 0x80, 16, 0x6D80);
        }
    }
}

void func_800B1CD0(void) {
    s32 lx;
    s32 rx;
    s32 ly;
    s16 v;
    s32 sp;
    s32 vol;
    s32 a;
    s32 ang;
    Unk800CE3C8 *m;
    s32 dx;
    s32 dz;

    func_800B3524(&lx);
    func_800B3398(&rx);
    if ((u8)func_800B345C(&ly)) {
        ly = 0;
    }
    ly = -ly;
    if (lx > 0 && ly < lx) {
        ly = lx;
    }
    if (lx < 0 && lx < ly) {
        ly = lx;
    }
    a = D_800CE3C8->unk4 * rx >> 7;
    D_800CE3BE = 0;
    D_800CE3E0 = a;
    if (func_800B8498() & 0x100000) {
        D_800CE3BE = 1;
        D_800CE0F0->unkC -= 0x20;
    }
    if (func_800B8498() & 0x200000) {
        D_800CE3BE = 1;
        D_800CE0F0->unkC += 0x20;
    }
    D_800CE0F0->unkE = 0;
    D_800CE0F0->unkC &= 0xFFF;
    m = D_800CE3C8;
    v = D_800CE3BC + (m->unk2 * ly / D_800CCE20 - D_800CE3BC) / D_800CCE24;
    D_800CE3BC = v;
    sp = v / D_800CCE28;
    D_800CE3C0 = sp;
    vol = ABS(sp) / 8 + 0x14;
    if (vol >= 0x80) {
        vol = 0x7F;
    }
    func_800C4850(m->unkC, 0x3C, vol);
    D_800CE3CC = D_800CE3DC->unk14.unk22 = (D_800CE3DC->unk14.unk22 + D_800CE3E0) & 0xFFF;
    if (ABS(D_800CE3C0) > 80 && D_800CE25E != 0) {
        ang = (D_800CE3C0 <= 0) << 11;
        dx = func_8004BB80(ang + D_800CE3DC->unk14.unk22) >> 2;
        dz = func_8004BB7C(ang + D_800CE3DC->unk14.unk22) >> 2;
        func_800C6F00(D_800CE3DC->unk14.vx + dx, -100, D_800CE3DC->unk14.vz + dz, 15, 15000);
        if (D_800CF4B0 & 1) {
            dx = func_8004BB80(ang + D_800CE3DC->unk14.unk22 + 0x80) >> 3;
            dz = func_8004BB7C(ang + D_800CE3DC->unk14.unk22 + 0x80) >> 3;
            func_800C6F00(D_800CE3DC->unk14.vx + dx, -100, D_800CE3DC->unk14.vz + dz, 10, 15000);
            dx = func_8004BB80(ang + D_800CE3DC->unk14.unk22 - 0x80) >> 3;
            dz = func_8004BB7C(ang + D_800CE3DC->unk14.unk22 - 0x80) >> 3;
            func_800C6F00(D_800CE3DC->unk14.vx + dx, -100, D_800CE3DC->unk14.vz + dz, 10, 15000);
        }
    }
}

INCLUDE_ASM("asm/us/ovl_0c/nonmatchings/ovl_0c", func_800B20B8);

void func_800B2C24(void) {
    s32 a;
    s32 cam;
    s32 d;
    s32 t;
    s32 q;

    a = (D_800CE0F0->unkC + D_800CE0F0->unkE) & 0xFFF;
    cam = D_800CE3CC & 0xFFF;
    if (a - cam >= 0x800) {
        a -= 0x1000;
    }
    if (a - cam < -0x800) {
        a += 0x1000;
    }
    d = (D_800CE3CC - a) * 0xC00 / 4096;
    D_800CCE3C = abs(D_800CE0F0->unk0) >> 3;
    if (abs(d) < D_800CCE3C) {
        d = 0;
    }
    if (D_800CE3C0 != 0) {
        D_800CE3D4 = 0x1000;
    } else {
        D_800CE3D4 -= 0x200;
        if (D_800CE3D4 < 0) {
            D_800CE3D4 = 0;
        }
    }
    if (D_800CE3BE == 0 && D_800CE0E8->unk11 == 0 && D_800CF46C == 2) {
        D_800CCE30 = abs(d);
        if (abs(d) < D_800CCE34) {
            D_800CCE30 = D_800CCE34;
        }
        if (D_800CCE30 > D_800CCE38) {
            D_800CCE30 = D_800CCE38;
        }
        t = (((0x800 - abs(d)) * D_800CCE2C >> 12) + 0x800) * (2 * D_800CE3D4);
        q = (d * 256 / D_800CCE30) * (t / 4096);
        D_800CE0F0->unkC += q / 4096;
    }
    D_800CE0F0->unkC &= 0xFFF;
}

void func_800B2E54(void) {
    s32 a;
    s32 step;
    s32 q;
    s32 cur;

    a = D_800CE0F0->unkC;
    if (a - D_800CE3DC->unk14.unk22 > 0x800) {
        D_800CE0F0->unkC = a - 0x1000;
    }
    if (D_800CE0F0->unkC - D_800CE3DC->unk14.unk22 < -0x800) {
        D_800CE0F0->unkC += 0x1000;
    }
    if (a < 0) {
        a += 0x1000;
    }
    a %= 4096;
    cur = D_800CE3DC->unk14.unk22;
    if (a - cur > 0x800) {
        a -= 0x1000;
    }
    if (a - cur < -0x800) {
        a += 0x1000;
    }
    a = cur - a;
    a %= 4096;
    if (D_800CE3BE == 0 && D_800CE0E8->unk11 == 0) {
        step = 16;
        if (D_800CE27C != 0) {
            step = ABS(a) * 256 >> 12;
            if (step == 0) {
                step = 1;
            }
        }
        q = a % 4096 / step;
        if (q != 0) {
            D_800CE0F0->unkC += q;
        } else if (step >= 2) {
            do {
                step /= 2;
                if (step == 0) {
                    break;
                }
                q = a % 4096 / step;
            } while (q == 0);
            D_800CE0F0->unkC += q;
        } else {
            D_800CE0F0->unkC = D_800CE3DC->unk14.unk22;
        }
        D_800CE0F0->unkC &= 0xFFF;
        if (ABS(a) < 100) {
            D_800CE27C = 0;
        }
    }
}

void func_800B30C0(void) {
    Unk16D8 *n;
    Unk8004AB64 *o;
    Unk800CCCC0 *c;
    ActorPos pos;
    s32 tmp;
    u16 h;

    n = func_8004AE9C();
    if (n != NULL) {
        do {
            o = n->unk4;
            if (o->unk5 == 4) {
                pos = o->unk14;
                c = &D_800CCCC0[o->unk11];
                func_800BE748((VECTOR *)&pos, &c->unk8, &tmp, NULL, &h);
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

u8 func_800B3340(u32 *a, u32 c) {
    c = (c & 0xFC) >> 2;
    if (c >= 32) {
        if ((a[0] >> (c - 32)) & 1) {
            return 1;
        }
    } else if ((a[1] >> c) & 1) {
        return 1;
    }
    return 0;
}

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

/* world map start-up */
void func_800B69B4(void) {
    RECT r = D_800A71D8;

    D_800CF47D = 1;
    if (D_8007AEF0.b[0x65] & 1) {
        func_8002FB64(1);
    } else {
        func_8002FAAC(1);
    }
    D_800CF430 = D_8007AEF0.h;
    if ((u16)D_8007AEF0.h[0] >= 0x2B52) {
        D_800CF444 = 4;
    } else {
        D_800CF444 = 1;
    }
    func_800B8A80();
    D_800CF4B0 = 0;
    D_800CF4AC = 0;
    D_800CF454 = 0;
    D_800CF46C = 0;
    D_800CF44C = 0;
    D_800CF46D = 1;
    D_800CF440 = 0;
    D_800CF47C = 0;
    D_800CF442 = 0;
    D_800CF47E = 0;
    D_800CF434 = 0;
    D_800CF4A8 = 0;
    D_800CF45C = &D_8006794C->unk1C->unk8;
    D_800CF460[0] = (s32)D_8006794C->unk1C->unk10[0];
    D_800CF460[1] = (s32)D_8006794C->unk1C->unk10[1];
    D_800CF448 = &D_8006794C->unk1C->unk18;
    ResetGraph(1);
    func_800C461C();
    StoreImage(&r, (u32 *)D_800CFFD0);
    DrawSync(0);
    func_800B6908();
    func_800B872C();
    func_800AD204();
    func_800C4A5C();
    func_800AEADC();
    DrawSync(0);
    func_800BA58C();
    func_800BE894();
    func_800C3DEC();
    func_800B8574();
    func_800BA29C();
    func_80050E34(D_800CF400);
    func_80050EFC(0x24, 0x30);
    func_800325A4();
    D_80073998[0] |= 0xF0000;
}

INCLUDE_RODATA("asm/us/ovl_0c/nonmatchings/ovl_0c", D_800A71D8);

INCLUDE_RODATA("asm/us/ovl_0c/nonmatchings/ovl_0c", D_800A71E0);

/* world map frame */
s32 func_800B6BC4(void) {
    s32 t;
    s32 busy;

    D_800CF47D = 0;
    if (D_800CF47C != 0) {
        D_8006794C->unk1C->unk892 = 1;
    }
    D_800CF49C = 0;
    if (func_800B84C0() & 0x40000) {
        if (D_800CF454 == 0 && D_800D33F4 != 0 && D_800D4818 == 0) {
            D_800CF454 = 3;
            D_8006794C->unk1C->unk0 |= 2;
            func_80032120(0, 0);
            func_8004A9E8();
        }
    }
    if (!(D_8006794C->unk1C->unk0 & 0x1C)) {
        D_800CF404 = D_800CF420[*D_800CF45C];
        func_8004C204();
        D_800CF43C = VSync(1);
        if (!(D_8006794C->unk1C->unk0 & 2)) {
            func_800B7098();
        }
        if (D_800CF4B0 == 4 && D_800CF46C == 0) {
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
        D_800CF43C = VSync(1) - D_800CF43C;
        D_800CF458 = VSync(1);
        func_800B72A4();
        func_800B8560();
        D_800CF458 = VSync(1) - D_800CF458;
        t = D_800CF404 - D_800CF420[*D_800CF45C];
        D_800CF4B0++;
        D_800CF470 = t;
        D_800CCE5C = (u32)(t * 100) / 155136;
        if (D_800D33F4 != 0) {
            D_800CF4AC++;
        }
        if (D_800CF434 != 0 && D_800CF454 == 3) {
            D_800CF434 = 0;
            D_800CF454 = 0;
        }
    } else {
        D_800CF434 = 1;
    }
    if (!(D_8006794C->unk1C->unk0 & 1)) {
        func_80032100();
    }
    func_80023A0C();
    busy = D_800CF49C != 0;
    if (D_8006794C->unk1C->unk0 & 0x1C) {
        busy = 1;
    }
    if (D_800CF440 != 0) {
        busy = 1;
    }
    func_80019160(0x8000, busy);
    switch (D_800CF49C) {
    case 3:
        if ((u8)D_800CCCC0[D_800CE3DC->unk11].unk8 >> 2 == 0x34) {
            D_800CF49C = 0;
        }
        D_8006794C->unk1C->unk571 = func_800B8548();
        func_80022D3C(0);
        while (func_80022B18() != 0) {
        }
        while (func_800AB0C0(0xC00, 0, 0, 0, 0) != 0) {
        }
        break;
    case 4:
        func_80015C38("MAP JUMP\n");
        if (D_800D3E75 >= 0) {
            D_80073998[0] &= 0xFDF7FFFF;
        }
        func_80022D3C(0);
        while (func_80022B18() != 0) {
        }
        while (func_800AB0C0(0xC00, 0, 0, 0, 0) != 0) {
        }
        break;
    }
    if (D_800CF47C != 0) {
        D_8006794C->unk1C->unk892 = 0;
    }
    D_800CF4B8 = 0;
    D_800CF4B8 = D_800CF43C;
    D_800CF4B8 = D_800CF43C + D_800CF4B4;
    D_800CF4B8 = D_800CF4B8 + D_800CF458;
    return D_800CF49C;
}

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

/* script request (w_frameSetParameter) */
void func_800B73F8(u32 arg0, s32 arg1) {
    switch (arg0) {
    case 1:
        func_800B153C();
        break;
    case 0:
        if (arg1 == 4) {
            D_800D3F9C = 1;
            D_800D3F8C = -1;
        } else {
            D_800D3F9C = 0;
            func_800C2A80(arg1, 2, D_800CF488[0]);
        }
        break;
    case 4:
        D_800D3E60 = arg1;
        break;
    case 2:
        D_800D3E80 = arg1;
        break;
    case 5:
        D_800CE0EF = arg1;
        break;
    case 10:
        func_800C6F00(D_800CF488[0], D_800CF488[1], D_800CF488[2], arg1, D_800CF488[3]);
        break;
    case 11:
        D_800D3E6C = arg1;
        break;
    case 15:
        D_800D3F9C = arg1;
        break;
    case 6:
    case 14:
    case 16:
        D_800CF488[0] = arg1;
        break;
    case 7:
    case 17:
        D_800CF488[1] = arg1;
        break;
    case 8:
    case 18:
        D_800CF488[2] = arg1;
        break;
    case 9:
    case 19:
        D_800CF488[3] = arg1;
        break;
    case 20:
        func_800C4718(D_800CF488[0], D_800CF488[1]);
        break;
    case 21:
        func_800C47B0(D_800CF488[0], D_800CF488[1]);
        break;
    case 22:
        func_800C4850(D_800CF488[0], D_800CF488[2], D_800CF488[1]);
        break;
    case 23:
        func_800C48F4(D_800CF488[0]);
        break;
    case 24:
        func_800AB0C0(0x8601, 0, D_800CF488[2], D_800CF488[1], 0);
        break;
    case 25:
        func_800AE860();
        break;
    case 26:
        D_800CF474 = arg1;
        break;
    case 27:
        D_800D3E78 = arg1;
        break;
    case 28:
        if (!func_800B105C((ActorPos *)D_800CF488)) {
            D_800CF488[0] = 10000;
            D_800CF488[1] = 10000;
            D_800CF488[2] = 10000;
        }
        break;
    case 29:
        func_800B81A0();
        break;
    case 30:
        func_800B31B8();
        break;
    case 31:
        func_800B326C();
        break;
    case 32:
        func_800C4710(arg1);
        break;
    case 33:
        func_800AB0C0(0x8601, 0, arg1, 0, 0);
        break;
    case 34:
        func_800B35F4();
        break;
    case 35:
        D_800CF440 = arg1;
        break;
    case 36:
        D_800D3E77 = arg1;
        break;
    case 37:
        D_800CE276 = 0;
        D_800CE3BC = 0;
        break;
    case 38:
        func_800B3638();
        break;
    case 39:
        D_800CF454 = 3;
        break;
    case 40:
        D_800CF454 = 0;
        break;
    case 41:
        D_800CF4A8 = 1;
        break;
    }
    if (D_800CCE50 & 0x80) {
        switch (arg0) {
        case 0:
            func_80015C38("*[ Request from script:Weather:%d ]\n", arg1);
            break;
        case 2:
            func_80015C38("*[ Request from script:Navi Active:%d ]\n", arg1);
            break;
        case 1:
            func_80015C38("*[ Request from script:Actor:%d ]\n", arg1);
            break;
        case 4:
            func_80015C38("*[ Request from script:Navi mode:%d ]\n", arg1);
            break;
        case 5:
            func_80015C38("*[ Request from script:Camera Lookup Fix mode:%d ]\n", arg1);
            break;
        case 11:
            func_80015C38("*[ Request from script:Title Draw Time:%d ]\n", arg1);
            break;
        case 14:
            func_80015C38("*[ Request from script:Whather Speed:%d ]\n", arg1);
            break;
        case 15:
            func_80015C38("*[ Request from script:Whather Auto:%d ]\n", arg1);
            break;
        case 16:
            func_80015C38("*[ Request from script:SE Param0:%d ]\n", arg1);
            break;
        case 17:
            func_80015C38("*[ Request from script:SE Param1:%d ]\n", arg1);
            break;
        case 18:
            func_80015C38("*[ Request from script:SE Param2:%d ]\n", arg1);
            break;
        case 19:
            func_80015C38("*[ Request from script:SE Param3:%d ]\n", arg1);
            break;
        case 20:
            func_80015C38("*[ Request from script:SE Play ]\n");
            break;
        case 21:
            func_80015C38("*[ Request from script:SE Vol ]\n");
            break;
        case 22:
            func_80015C38("*[ Request from script:SE Vol Intpl ]\n");
            break;
        case 23:
            func_80015C38("*[ Request from script:SE Stop ]\n");
            break;
        case 24:
            func_80015C38("*[ Request from script:Music Vol Intpl:Prm1 %d Prm2 %d]\n", D_800CF488[1],
                          D_800CF488[2]);
            break;
        case 25:
            func_80015C38("*[ Request from script:Camera Smooth ]\n");
            break;
        case 26:
            func_80015C38("*[ Request from script:EventBattleProbability %d]\n", arg1);
            break;
        case 27:
            func_80015C38("*[ Request from script:Navi Cursol\n");
            break;
        case 28:
            func_80015C38("*[ Request from script:Get Getoffpos ]\n");
            func_80015C38("Value X:%d\n", D_800CF488[0]);
            func_80015C38("Value Y:%d\n", D_800CF488[1]);
            func_80015C38("Value Z:%d\n", D_800CF488[2]);
            break;
        case 29:
            func_80015C38("*[ Request from script:Call Chocobo ]\n");
            break;
        case 30:
            func_80015C38("*[ Request from script:Get Getoff SP1 ]\n");
            break;
        case 31:
            func_80015C38("*[ Request from script:Get Getoff SP2 ]\n");
            break;
        case 32:
            func_80015C38("*[ Request from script:Music Fade ]\n");
            break;
        case 33:
            func_80015C38("*[ Request from script:Music Fade Force]\n");
            break;
        case 34:
            func_80015C38("*[ Request from script:Auto Pilot ON]\n");
            break;
        case 35:
            func_80015C38("*[ Request from script:Mog choice %d]\n", arg1);
            break;
        case 36:
            func_80015C38("*[ Request from script:Cursol Draw %d]\n", arg1);
            break;
        case 37:
            func_80015C38("*[ Request from script:Alpha 0%d]\n", arg1);
            break;
        case 38:
            func_80015C38("*[ Request from script:Auto Pilot OFF]\n");
            break;
        case 39:
            func_80015C38("*[ Request from script:CD Use ON]\n");
            break;
        case 40:
            func_80015C38("*[ Request from script:CD Use OFF]\n");
            break;
        case 41:
            func_80015C38("*[ Request from script:Map Jump]\n");
            break;
        case 3:
        case 6:
        case 7:
        case 8:
        case 9:
        case 10:
            break;
        default:
            func_80015C38("w_frameSetParameter: illigal function #%d\n", arg0);
            break;
        }
    }
}

s32 func_800B7A14(s32 arg0) {
    u32 v;
    s32 hi;
    s32 lo;
    s32 r;
    u8 ret;
    s32 k;

    v = D_800CCCC0[D_800CE3DC->unk11].unk8;
    if ((D_800CCE50 & 0x80) && arg0 == 0xC8) {
        func_80015C38("Event:CDAccessWait\n");
    }
    switch (arg0) {
    case 0xC0:
        return (v & 0x3F00) >> 8;
    case 0xC1:
        return (v & 0xFC) >> 2;
    case 0xC2:
        if (D_800CE3C4 == 6) {
            return D_800CCCC0[D_800CE3DC->unk11].unkE + 400;
        }
        return D_800CCCC0[D_800CE3DC->unk11].unkE + D_800CCCC0[D_800CE3DC->unk11].unkC;
    case 0xC3:
        return D_800CF488[0];
    case 0xC4:
        return D_800CF48C;
    case 0xC5:
        return D_800CF490;
    case 0xC6:
        return D_8006794C->unk1C->unk844;
    case 0xC7:
        return D_800CE0F0->unkC >> 4;
    case 0xC8:
        return D_800CF454 | (D_800D4818 != 0);
    case 0xC9:
        return D_800CE3DC->unk14.vx;
    case 0xCA:
        return D_800CE3DC->unk14.vy;
    case 0xCB:
        return D_800CE3DC->unk14.vz;
    case 0xCC:
        return D_800D3F88;
    case 0xCD:
        if (D_8007B720 >= D_8007B92D) {
            func_8004BF20();
        }
        hi = *D_8007B720++;
        if (D_8007B720 >= D_8007B92D) {
            func_8004BF20();
        }
        lo = *D_8007B720++;
        r = ((hi << 8) | lo) % (D_800CF474 + 1);
        ret = 0;
        if (D_800CCE54 != 0 && D_800CE25E != 0) {
            k = (s32)(v & 0xFC) >> 2;
            if (k >= 0x24) {
                if (k < 0x27 && D_800CF4B0 > 400) {
                    ret = r == 1;
                }
            }
        }
        return ret;
    case 0xCE:
        return ((u16)D_800CE3DC->unk14.unk22 & 0xFFF) >> 4;
    case 0xCF:
        return func_800BC528((v & 0x3F00) >> 8);
    case 0xD0:
        return D_800CCD7C;
    case 0xD1:
        return D_800CF46C == 2;
    case 0xD2:
        return D_800D3E88;
    case 0xD3:
        return (u8)D_800D3E64;
    default:
        func_80015C38("w_frameGetParameter: illigal function #%d\n", arg0);
        return -1;
    }
}

void func_800B7DAC(void) {
    DISPENV env;
    s32 mode;
    u32 pad;

    mode = 0;
    SetDefDispEnv(&env, 0, 0, 640, 480);
    env.screen.y = 0;
    env.screen.x = 0;
    env.screen.h = 255;
    env.screen.w = 255;
    PutDispEnv(&env);
    while (1) {
        func_80019194();
        pad = func_800B7DAC_pad();
        if (func_800B7DAC_trg() & 1) {
            switch (mode) {
            case 0:
                mode = 1;
                SetDefDispEnv(&env, 0, 0, 640, 480);
                break;
            case 1:
                mode = 0;
                SetDefDispEnv(&env, 0, 0, 256, 256);
                break;
            }
            env.screen.y = 0;
            env.screen.x = 0;
            env.screen.h = 255;
            env.screen.w = 255;
            PutDispEnv(&env);
        }
        if (pad & 0x80) {
            env.disp.x -= 2;
        }
        if (pad & 0x20) {
            env.disp.x += 2;
        }
        if (pad & 0x10) {
            env.disp.y -= 2;
        }
        if (pad & 0x40) {
            env.disp.y += 2;
        }
        if (pad & 0x4000) {
            break;
        }
        VSync(0);
        PutDispEnv(&env);
    }
}

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

/* fixed-point lerp of two values with arg3 bits of headroom */
s32 func_800B822C(s32 arg0, s16 arg1, s32 arg2, s32 arg3) {
    s32 s = 16 - arg3;

    arg0 = (((arg0 << 16) >> 16 << s) * (0x400 - arg2 / 4)) >> 10;
    arg0 += ((arg1 << s) * (arg2 / 4)) >> 10;
    return arg0 >> s;
}

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
    D_800CF500.m[0][0] = D_800D3FA0.unk5E0[0].unk0[0].vx;
    D_800CF500.m[1][0] = D_800D3FA0.unk5E0[0].unk0[0].vy;
    D_800CF500.m[2][0] = D_800D3FA0.unk5E0[0].unk0[0].vz;
    D_800CF500.m[0][1] = D_800D3FA0.unk5E0[0].unk0[1].vx;
    D_800CF500.m[1][1] = D_800D3FA0.unk5E0[0].unk0[1].vy;
    D_800CF500.m[2][1] = D_800D3FA0.unk5E0[0].unk0[1].vz;
    D_800CF500.m[0][2] = D_800D3FA0.unk5E0[0].unk0[2].vx;
    D_800CF500.m[1][2] = D_800D3FA0.unk5E0[0].unk0[2].vy;
    D_800CF500.m[2][2] = D_800D3FA0.unk5E0[0].unk0[2].vz;
    D_800CF4E0.m[0][0] = D_800D3FA0.unk5E0[0].unk18.vx;
    D_800CF4E0.m[0][1] = D_800D3FA0.unk5E0[0].unk18.vx;
    D_800CF4E0.m[1][0] = D_800D3FA0.unk5E0[0].unk18.vy;
    D_800CF4E0.m[2][0] = D_800D3FA0.unk5E0[0].unk18.vz;
    D_800CF4E0.m[1][1] = D_800D3FA0.unk5E0[0].unk18.vy;
    D_800CF4E0.m[2][1] = D_800D3FA0.unk5E0[0].unk18.vz;
    D_800CF4E0.m[0][2] = D_800D3FA0.unk5E0[0].unk18.vx;
    D_800CF4E0.m[1][2] = D_800D3FA0.unk5E0[0].unk18.vy;
    D_800CF4E0.m[2][2] = D_800D3FA0.unk5E0[0].unk18.vz;
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

/* pop the heap stack */
s32 func_800B89B0(void) {
    s32 *stack;
    s32 n;
    s32 prev;
    s32 size;

    if (D_800CFE14 == 0) {
        func_80015C38("w_memorymanHeapFree:no allocated heap.\n");
        return 0;
    }
    if (D_800CCE50 & 0x40) {
        func_80015C38("w_memorymanHeapFree : [%d]  =  %x - ", D_800CFE14 - 1, D_800CFE18);
    }
    stack = D_800CFDB0;
    n = D_800CFE14 - 1;
    prev = stack[n];
    size = D_800CFE18;
    size -= prev;
    D_800CFE14 = n;
    D_800CFE18 = prev;
    D_800CFE10 -= size;
    if (D_800CCE50 & 0x40) {
        func_80015C38("%x\n", D_800CFE18);
    }
    return 1;
}

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

void func_800B8B10(void) {
    RECT r;
    u16 off;
    s32 src;
    s32 i;
    s32 j;
    Unk800CFF18 *e;
    s32 a;
    s32 b;
    u16 *dst;

    off = 0;
    src = 0;
    func_8001D898(&r, 0, 8);
    r.h = 1;
    D_800D1FD0 = 0;
    D_800CFE98[0] = 0;
    D_800CFE98[1] = 0;
    for (i = 7; i >= 0; i--) {
        D_800CFE88[i] = 0;
    }
    for (i = 0; i < 18; i++) {
        e = &D_800CFF18[i];
        r.x = e->unk0 + e->unk4;
        r.y = e->unk2;
        r.w = e->unk6;
        r.h = 1;
        if (r.w == 0) {
            r.w = 1;
        }
        if (i < 18) {
            if (i >= 15) {
                dst = &D_800CFFD0[0][off];
                func_80015C28((u8 *)dst, (u8 *)func_800B8388(D_800CF428, 2) + src, (u16)e->unk6 * 2);
                src += (u16)e->unk6 * 2;
            } else {
                StoreImage(&r, (u32 *)&D_800CFFD0[0][off]);
                DrawSync(0);
            }
        } else {
            StoreImage(&r, (u32 *)&D_800CFFD0[0][off]);
            DrawSync(0);
        }
        for (j = 0; j < r.w; j++) {
            a = (j + r.w) % r.w;
            b = (j + r.w - 1) % r.w;
            D_800CFFD0[0][off + a + r.w] = D_800CFFD0[0][off + j];
            D_800CFFD0[1][off + b + r.w] = D_800CFFD0[0][off + j];
            D_800CFFD0[1][off + b] = D_800CFFD0[0][off + j];
        }
        D_800CFE20[i] = off;
        D_800CFE70[i] = 0;
        D_800CFE48[i] = 0;
        off += (u16)e->unk6 * 2;
    }
}

/* upload TIM arg1 of table arg0, offset by (arg2, arg3) (image) and (arg4, arg5) (clut) */
void func_800B8DC0(u32 *arg0, s32 arg1, s32 arg2, s32 arg3, s32 arg4, s32 arg5, s8 arg6) {
    u32 *tim;
    RECT *clut;
    u32 *cdata;
    u32 *img;
    u8 has;
    RECT r;
    RECT r2;
    RECT *q;
    RECT *q2;
    RECT *q3;
    s32 n;
    RECT *rect;
    u32 *data;

    tim = (u32 *)arg0[arg1 + 1];
    if ((arg2 | arg3 | arg4 | arg5) == 0) {
        if (arg6 != 0) {
            func_8001D8A8((s32)tim);
            return;
        }
    }
    clut = NULL;
    cdata = NULL;
    has = tim[1] >> 3;
    img = tim + 2;
    if (has) {
        clut = (RECT *)(tim + 3);
        cdata = tim + 5;
        n = (tim[2] >> 2) - 3;
        img = cdata + n;
    }
    rect = (RECT *)++img;
    data = (u32 *)(rect + 1);
    if (D_800CF47D != 0) {
        r = *rect;
        r.x += arg2;
        r.y += arg3;
        LoadImage(&r, data);
    } else {
        q = (RECT *)D_800CF404;
        q->x = rect->x + arg2;
        q->y = rect->y + arg3;
        q->w = rect->w;
        q->h = rect->h;
        LoadImage(q, data);
        q++;
        D_800CF404 = (s32)q;
    }
    if (arg6 != 2) {
        if (has) {
            if (D_800CF47D != 0) {
                r2 = *clut;
                r2.x += arg4;
                r2.y += arg5;
                if (r2.w == 0x100) {
                    r2.w = 0xF0;
                }
                LoadImage(&r2, cdata);
            } else {
                q2 = (RECT *)D_800CF404;
                q2->x = clut->x + arg4;
                q2->y = clut->y + arg5;
                q2->w = clut->w;
                q2->h = clut->h;
                if (q2->w == 0x100) {
                    q2->w = 0xF0;
                }
                LoadImage(q2, cdata);
                q2++;
                D_800CF404 = (s32)q2;
            }
        }
    } else {
        q3 = (RECT *)D_800CF404;
        q3->x = 0x280;
        q3->y = 0xF6;
        q3->w = 0x80;
        q3->h = 1;
        LoadImage(q3, cdata);
        q3++;
        D_800CF404 = (s32)q3;
    }
}

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

/* advance the palette animations arg0..arg1 and upload the new frames (y offset arg2) */
void func_800B94F8(s32 arg0, s32 arg1, s32 arg2) {
    RECT *r;
    s32 i;
    s32 n;

    for (i = arg0; i <= arg1; i++) {
        D_800CFE48[i] += D_800CFF18[i].unk8;
        n = D_800CFE48[i] / 256;
        D_800CFE48[i] &= 0xFF;
        if (n != 0) {
            r = (RECT *)D_800CF404;
            r->x = D_800CFF18[i].unk0 + D_800CFF18[i].unk4;
            r->y = D_800CFF18[i].unk2 + arg2;
            r->w = D_800CFF18[i].unk6;
            r->h = 1;
            D_800CFE70[i] += n;
            D_800CFE70[i] %= r->w;
            LoadImage(r++, (u32 *)&D_800CFFD0[D_800CFE70[i] & 1][D_800CFE20[i] + (D_800CFE70[i] & 0xFFFE)]);
            D_800CF404 = (s32)r;
        }
    }
}

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
    D_800D1FE0[0].unk0.r = D_800D3FA0.unk5E0[0].unk30[1].unk4[0];
    D_800D1FE0[0].unk0.g = D_800D3FA0.unk5E0[0].unk30[1].unk4[1];
    D_800D1FE0[0].unk0.b = D_800D3FA0.unk5E0[0].unk30[1].unk4[2];
    D_800D1FE0[0].unk4 = D_800D3FA0.unk5E0[0].unk30[1].unk0;
    D_800D1FE0[0].unk6 = D_800D3FA0.unk5E0[0].unk30[1].unk2;
    D_800D1FE0[1].unk0.r = D_800D3FA0.unk5E0[0].unk30[0].unk4[0];
    D_800D1FE0[1].unk0.g = D_800D3FA0.unk5E0[0].unk30[0].unk4[1];
    D_800D1FE0[1].unk0.b = D_800D3FA0.unk5E0[0].unk30[0].unk4[2];
    D_800D1FE0[1].unk4 = D_800D3FA0.unk5E0[0].unk30[0].unk0;
    D_800D1FE0[1].unk6 = D_800D3FA0.unk5E0[0].unk30[0].unk2;
    D_800D1FD8.unk0.r = D_800D3FA0.unk5E0[0].unk30[2].unk4[0];
    D_800D1FD8.unk0.g = D_800D3FA0.unk5E0[0].unk30[2].unk4[1];
    D_800D1FD8.unk0.b = D_800D3FA0.unk5E0[0].unk30[2].unk4[2];
    D_800D1FD8.unk4 = D_800D3FA0.unk5E0[0].unk30[2].unk0;
    D_800D1FD8.unk6 = D_800D3FA0.unk5E0[0].unk30[2].unk2;
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

void func_800BA3F4(s32 arg0, s32 arg1) {
    s32 ot = D_800CF460[*D_800CF45C];

    func_800B3A80(arg0);
    D_800CF404 = func_800B3FEC(arg0, D_800CF404, ot);
}

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

void func_800BA58C(void) {
    s32 unused[26];
    void **f[6]; /* f[6] below overruns into the saved s0 slot, as in the original */
    void *p;
    void **t;
    Unk800CFEA0Tbl *dst;
    s32 i;
    s32 acc;
    s32 w;
    s32 h;

    acc = 0;
    func_800BE674();
    i = 0;
    func_800BC540();
    do {
        D_800D30D8[i] = acc;
        acc += D_800CCFF4[i] * 2;
        i++;
    } while (i < 26);
    while (func_80022B18() != 0) {
    }
    if (D_800CF444 == 1) {
        D_800CE3F4 = func_8001C7FC(4, 0x411, 0);
        D_800CF428 = func_8001C7FC(4, 0x41B, 0);
        D_800CF40C = func_8001C7FC(4, 0x425, 0);
        D_800CF410 = func_8001C7FC(4, 0x42F, 0);
        f[4] = func_8001C7FC(4, 0x439, 0);
        f[5] = func_8001C7FC(4, 0x443, 0);
    } else {
        D_800CE3F4 = func_8001C7FC(4, 0x414, 0);
        D_800CF428 = func_8001C7FC(4, 0x41E, 0);
        D_800CF40C = func_8001C7FC(4, 0x428, 0);
        D_800CF410 = func_8001C7FC(4, 0x432, 0);
        f[4] = func_8001C7FC(4, 0x43C, 0);
        f[5] = func_8001C7FC(4, 0x446, 0);
    }
    f[0] = func_8001C7FC(4, 0x5DC, 0);
    f[1] = func_8001C7FC(4, 0x5DD, 0);
    f[2] = func_8001C7FC(4, 0x5DE, 0);
    f[3] = func_8001C7FC(4, 0x5DF, 0);
    f[6] = func_8001C7FC(4, 0x5E0, 0);
    w = D_800CF40C->unk0;
    h = D_800CF40C->unk2;
    D_800D3304 = w;
    D_800D3308 = h;
    D_800D32F0 = w * 2;
    D_800D32F4 = h * 2;
    D_800D32D8 = w << 14;
    D_800D32DC = h << 14;
    *(WmCellMap *)D_800D30F8 = *(WmCellMap *)f[0];
    func_800BB508();
    func_800B828C(D_800CE3F4);
    func_800B828C((s32 *)D_800CF428);
    func_800B828C((s32 *)f[1]);
    func_800B828C((s32 *)f[2]);
    func_800B828C((s32 *)f[3]);
    func_800B828C((s32 *)f[4]);
    func_800CB09C(f[6]);
    t = f[5];
    func_800B828C((s32 *)t);
    for (i = 0; i < 16; i++) {
        p = func_800B8388(t, i + D_800D30C8[i] * 16);
        func_800B828C(p);
        func_800B96CC(p, 0, 0, 0, 0);
    }
    func_800B96CC((s32 *)f[4], 0, 0, 0, 0);
    func_800B96CC((s32 *)f[1], 0, 0, 0, 0);
    if (D_8006794C->unk1C->unk844 != 0) {
        func_800B8DC0((u32 *)f[3], D_8006794C->unk1C->unk844 - 1, 0x318, 0xB0, 0x280, 0xF6, 2);
    }
    if (*D_800CF430 >= 0x1766) {
        D_800D3E74 = 1;
    } else {
        D_800D3E74 = 0;
    }
    func_800B8DC0((u32 *)f[2], D_800D3E74 * 2, 0, 0, 0, 0, 0);
    func_800B8DC0((u32 *)f[2], (D_800D3E74 << 1) | 1, 0, 0, 0, 0, 1);
    func_800B8DC0((u32 *)f[2], 4, 0, 0, 0, 0, 0);
    D_800D3E75 = -1;
    switch (*D_800CF430) {
    case 0x960:
        D_800D3E75 = 0;
        break;
    case 0x1766:
        D_800D3E75 = 1;
        break;
    case 0x2585:
        D_800D3E75 = 2;
        break;
    case 0x26A2:
        D_800D3E75 = 3;
        break;
    }
    if (D_800D3E75 != -1) {
        if (D_800CCE40 == 0) {
            func_800B8DC0((u32 *)f[2], D_800D3E75 * 2 + 5, 0x300, 0x40, 0x280, 0xF4, 0);
            func_800B8DC0((u32 *)f[2], D_800D3E75 * 2 + 6, 0x320, 0x40, 0x290, 0xF4, 0);
        } else {
            func_800B8DC0((u32 *)f[2], D_800D3E75 * 2 + 5, 0x300, 0x40, 0x280, 0xF4, 0);
            func_800B8DC0((u32 *)f[2], D_800D3E75 * 2 + 6, 0x300, 0x60, 0x290, 0xF4, 0);
        }
    }
    for (i = 0x35; i < 0x43; i++) {
        func_800BA528(func_800B8388(D_800CF428, i));
    }
    D_800D32EC = func_800B8388(D_800CF428, 0x42);
    D_800CF478 = func_800B8388(D_800CF428, 0);
    D_800CF450 = func_800B8388(D_800CF428, 3);
    D_800D3FA0 = *(Unk800D3FA0 *)func_800B8388(D_800CF428, 5);
    dst = (Unk800CFEA0Tbl *)D_800CFEA0;
    *dst = *(Unk800CFEA0Tbl *)func_800B8388(D_800CF428, 6);
    D_800D32E4 = func_800B8388(D_800CF428, 4);
}

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

    SetBackColor(D_800D3FA0.unk5E0[0].unk20.c[0], D_800D3FA0.unk5E0[0].unk20.c[2], D_800D3FA0.unk5E0[0].unk20.c[4]);
    SetLightMatrix(&D_800CF540);
    SetColorMatrix(&D_800CF500);
    func_800BD5A8();
    func_800BD78C();
    SetBackColor(D_800D3FA0.unk5E0[0].unk28.c[0], D_800D3FA0.unk5E0[0].unk28.c[2], D_800D3FA0.unk5E0[0].unk28.c[4]);
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

void func_800BB07C(void) {
    s32 m;
    s32 flags;
    s32 i;
    s32 dx;
    s32 dz;
    s32 d;
    s32 h;
    s32 v;

    m = D_800CE3DC->unk11;
    flags = D_800CCCC0[m].unk8;
    if (D_800D4820 != 0) {
        func_800BC690();
        func_800BDAB0();
    }
    for (i = 0; i < 3; i++) {
        dx = (D_800CE3DC->unk14.vx >> 8) - (D_800CD078[i] >> 8);
        dz = (D_800CE3DC->unk14.vz >> 8) - (D_800CD084[i] >> 8);
        D_800D32F8[i] = SquareRoot0(dx * dx + dz * dz);
        if (D_800D30C8[5] != 0 && i == 0) {
            D_800D32F8[0] = 100;
        }
        v = D_800D32F8[i];
        if (i == 0 && D_800CF444 == 4) {
            continue;
        }
        if (v < 0x3F) {
            if (D_800D330C[i] == 0) {
                D_800D330C[i] = 1;
                func_800C4718(D_800CD090[i], 0);
            } else {
                dz = 0x7F - v * 2;
                if (dz > 0) {
                    func_800C47B0(D_800CD090[i], dz);
                }
            }
        } else {
            if (D_800D330C[i] == 1) {
                D_800D330C[i] = 0;
                func_800C48F4(D_800CD090[i]);
            }
        }
    }
    if (D_800D32F8[2] < 0x28) {
        d = 0x78 - D_800D32F8[2] * 3;
        if (d != 0 && D_800D33F4 != 0) {
            D_800CE108[1] += func_80062B3C() % d - d / 2;
            D_800CE108[0] += func_80062B3C() % d - d / 2;
            D_800CE108[2] += func_80062B3C() % d - d / 2;
        }
    }
    if (D_800D32F8[1] < 0x22 && func_8004AE84() != 0 && ((flags & 0xFC) >> 2) == 0x17 && m - 1 < 7U) {
        D_800CE3DC->unk14.vz += (0x22 - D_800D32F8[1]) * 8;
        if (D_800CE3DC->unk14.vx == D_800CE3DC->unk40.vx && D_800CE3DC->unk14.vz == D_800CE3DC->unk40.vz) {
            D_800CE3DC->unk40.vx++;
        }
    }
    if (D_8007AEF0.b[0x66] == 8 || D_8007AEF0.b[0x66] == 0) {
        dx = (D_800CE3DC->unk14.vx >> 3) - 0x790A;
        dz = (D_800CE3DC->unk14.vz >> 3) + 0x5ACB;
        v = SquareRoot0(dx * dx + ((D_800CE3DC->unk14.vy >> 3) + 0x66) * ((D_800CE3DC->unk14.vy >> 3) + 0x66) * 8 + dz * dz);
        if (v < 0x47C) {
            h = 0x47C - v;
            if (h > 0x400) {
                h = 0x400;
            }
            if (h > 0x10) {
                h >>= 4;
                func_80050EFC(h, 0x40);
                D_800CF44C = 1;
                D_8006794C->unk1C->unk570 = h >> 4;
                return;
            }
        }
    }
    D_800CF44C = 0;
    D_8006794C->unk1C->unk570 = 0;
}

/* mark the map cells changed by the story progress */
void func_800BB508(void) {
    s32 i;
    s32 n;
    u8 *p;

    i = 15;
    p = &D_800D30C8[i];
    for (; i >= 0; i--) {
        *p-- = 0;
    }
    for (i = 10; i >= 0; i--) {
        if (D_800CD060[i] <= *D_800CF430) {
            break;
        }
    }
    if (D_800CCE50 & 0x10) {
        func_80015C38("Scene:%d / Status:%d\n", *D_800CF430, i);
    }
    if (i >= 0 && D_800CF444 == 1) {
        for (n = 0; n < 7; n++) {
            switch (D_800CD010[n][i]) {
            case 1:
                WM_CELL(18, 14) |= 0x80;
                WM_CELL(17, 12) |= 0x80;
                D_800D30C8[0] = 1;
                break;
            case 6:
                WM_CELL(13, 12) |= 0x80;
                WM_CELL(14, 12) |= 0x80;
                D_800D30C8[5] = 1;
                break;
            case 5:
                WM_CELL(13, 16) |= 0x80;
                WM_CELL(13, 17) |= 0x80;
                WM_CELL(14, 16) |= 0x80;
                WM_CELL(14, 17) |= 0x80;
                D_800D30C8[4] = 1;
                break;
            case 9:
                WM_CELL(14, 6) |= 0x80;
                WM_CELL(21, 10) |= 0x80;
                WM_CELL(22, 14) |= 0x80;
                D_800D30C8[8] = 1;
                break;
            case 3:
                WM_CELL(19, 10) |= 0x80;
                WM_CELL(19, 11) |= 0x80;
                WM_CELL(20, 10) |= 0x80;
                WM_CELL(20, 11) |= 0x80;
                D_800D30C8[2] = 1;
                break;
            case 11:
                WM_CELL(3, 9) |= 0x80;
                WM_CELL(9, 1) |= 0x80;
                D_800D30C8[10] = 1;
                break;
            case 4:
                WM_CELL(7, 1) |= 0x80;
                WM_CELL(8, 1) |= 0x80;
                WM_CELL(14, 15) |= 0x80;
                D_800D30C8[3] = 1;
                break;
            }
            /* the original second jump table is 8-aligned */
            __asm__(".rdata\n\t.word 0\n\t.text");
            if (D_800CCE50 & 0x10) {
                switch (D_800CD010[n][i]) {
                case 0:
                    func_80015C38("BGI Form : SGATE\n");
                    break;
                case 5:
                    func_80015C38("BGI Form : CLAY\n");
                    break;
                case 4:
                    func_80015C38("BGI Form : LIND\n");
                    break;
                case 8:
                    func_80015C38("BGI Form : KUROMA\n");
                    break;
                case 2:
                    func_80015C38("BGI Form : ALEX\n");
                    break;
                case 10:
                    func_80015C38("BGI Form : HOKORA(MIZU)\n");
                    break;
                case 3:
                    func_80015C38("BGI Form : HOKORA(HI)\n");
                    break;
                }
            }
        }
    }
    func_80015C38("General Global Flag = %x\n", D_8007AF55);
    if (D_8007AF55 & 0x80) {
        func_80015C38("- Replace Mog Net -\n");
        WM_CELL(16, 1) |= 0x80;
        WM_CELL(13, 4) |= 0x80;
        WM_CELL(14, 5) |= 0x80;
        D_800D30C8[12] = 1;
    }
    if (D_8007AF55 & 0x40) {
        func_80015C38("- Replace Chocobo Heaven -\n");
        WM_CELL(0, 0) |= 0x80;
        WM_CELL(16, 14) |= 0x80;
        WM_CELL(9, 17) |= 0x80;
        D_800D30C8[7] = 1;
    }
}

/* pick the encounter table entry for the player's area, zone and fog */
Unk800CF450 *func_800BBBA8(void) {
    u8 zone;
    s32 area;
    s32 lo;
    s32 hi;
    s32 i;
    u32 t;
    s32 f;
    s32 add;

    area = (D_800CCCC0[D_800CE3DC->unk11].unk8 & 0x3F00) >> 8;
    zone = D_800CCFB4[area];
    lo = D_800D30D8[zone];
    hi = D_800D30D8[zone + 1];
    if (D_800CCE50 & 8) {
        func_80015C38("- Encount Information -\n");
        func_80015C38("Now Area :%d\n", (D_800CCCC0[D_800CE3DC->unk11].unk8 & 0x3F00) >> 8);
        func_80015C38("Now Zone :%d\n", zone);
        func_80015C38("Now Fog  :%d\n", D_800CCE4B);
        func_80015C38("Topograph:%d\n", (u8)D_800CCCC0[D_800CE3DC->unk11].unk8 >> 2);
    }
    for (i = lo; i < hi; i++) {
        t = D_800CF450[i].unk8 >> 2;
        f = D_800CF450[i].unk9 & 1;
        if (D_800CCE50 & 8) {
            func_80015C38("Matching:%d  ( T:%d / F:%d )\n", i, t, f);
        }
        if (t == (u8)D_800CCCC0[D_800CE3DC->unk11].unk8 >> 2 && f == D_800CCE4B) {
            if (D_800CCE50 & 8) {
                func_80015C38("Hit %d encount data\n", i);
            }
            add = 0;
            if (D_800CF444 == 4 && i < 100) {
                add = 0xFE;
            }
            return &D_800CF450[i + add];
        }
    }
    func_80015C38(D_800A7F48);
    return &D_800CF450[i + add - 1];
}

s32 func_800BBE24(VECTOR *arg0, VECTOR *arg1, VECTOR *arg2, s32 *arg3) {
    VECTOR v;
    s32 s;

    v.vx = arg0->vx - D_800CE198[0];
    v.vy = arg0->vy - D_800CE198[1];
    v.vz = arg0->vz - D_800CE198[2];
    if (v.vx > D_800D32D8 - 0x8000) {
        v.vx -= D_800D32D8;
    }
    if (v.vz > D_800D32DC - 0x8000) {
        v.vz -= D_800D32DC;
    }
    if (v.vx < 0x8000 - D_800D32D8) {
        v.vx = D_800D32D8 + v.vx;
    }
    if (v.vz < 0x8000 - D_800D32DC) {
        v.vz = D_800D32DC + v.vz;
    }
    ApplyMatrixLV(D_800CF448, &v, arg2);
    s = 0x1000 - ((arg2->vz >> 4) + (arg2->vz >> 6));
    v.vy = ((arg0->vy * s) >> 12) - D_800CE198[1];
    ApplyMatrixLV(D_800CF448, &v, arg2);
    arg1->vx = arg0->vx - D_800CE108[0];
    arg1->vy = ((arg0->vy * s) >> 12) - D_800CE108[1];
    arg1->vz = arg0->vz - D_800CE108[2];
    if (arg1->vx > 0xFFFF) {
        arg1->vx -= 0x60000;
    }
    if (arg1->vz > 0xFFFF) {
        arg1->vz -= 0x50000;
    }
    if (arg1->vx < -0xFFFF) {
        arg1->vx += 0x60000;
    }
    if (arg1->vz < -0xFFFF) {
        arg1->vz += 0x50000;
    }
    if (arg3 != NULL) {
        *arg3 = s;
    }
    if (arg1->vx < -0x8000 || arg1->vx > 0x7FFF || arg1->vz > 0x7FFF || arg1->vz < -0x8000) {
        return 0;
    }
    return 1;
}

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

void func_800BC450(s32 arg0, s32 arg1, WmCell *arg2) {
    WmCell *cells;

    cells = D_800CF40C->unk4;
    arg2->unk0 = cells[arg0 * 2 + arg1].unk0;
    arg2->unk2 = cells[arg0 * 2 + arg1].unk2;
    arg2->unk3 = cells[arg0 * 2 + arg1].unk3;
}

void func_800BC490(s32 arg0, s32 arg1, WmCell *arg2) {
    s32 k;
    WmCell *cells;

    k = arg0 % D_800D3304 * D_800D3308 + arg0 / D_800D3304;
    cells = D_800CF410->unk4;
    arg2->unk0 = cells[k * 2 + arg1].unk0;
    arg2->unk2 = cells[k * 2 + arg1].unk2;
    arg2->unk3 = cells[k * 2 + arg1].unk3;
}

u8 func_800BC528(s32 arg0) {
    return D_800CCFB4[arg0];
}

/* initialise the texture pack list */
void func_800BC540(void) {
    u32 i;

    D_800D33F0 = 0;
    D_800D34A8[0].unk0 = &D_800D3358;
    D_800D34A8[0].unkE = 0;
    D_800D34A8[0].unk4 = &D_800D34A8[1];
    D_800D34A8[0].unkC = -1;
    D_800D34A8[0].unkF = 0;
    D_800D34A8[0].unk8 = (WmTexPack *)D_800CF408;
    for (i = 1; i < 31; i++) {
        D_800D34A8[i].unk0 = &D_800D34A8[i - 1];
        D_800D34A8[i].unk4 = &D_800D34A8[i + 1];
        D_800D34A8[i].unk8 = NULL;
        D_800D34A8[i].unkE = 0;
        D_800D34A8[i].unkC = -1;
        D_800D34A8[i].unkF = 0;
    }
    for (i = 1; i < 16; i++) {
        D_800D34A8[i].unk8 = (WmTexPack *)(D_800CF408 + i * 0x4800);
        D_800D34A8[i].unkF = 0;
    }
    i = 31;
    D_800D34A8[i].unk0 = &D_800D34A8[i - 1];
    D_800D34A8[i].unk4 = &D_800D3340;
    D_800D34A8[i].unkC = -1;
    D_800D34A8[i].unk8 = NULL;
    D_800D34A8[i].unkE = 0;
    D_800D34A8[i].unkF = 0;
    D_800D3358.unk0 = NULL;
    D_800D3358.unk4 = &D_800D34A8[0];
    D_800D3340.unk4 = &D_800D34A8[16];
    D_800D3750 = 0;
    D_800D3408 = -1;
    D_800D3340.unk0 = NULL;
    D_800D33F4 = 0;
    func_800C2040();
    func_800C26DC();
}

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

void func_800BC84C(s32 arg0) {
    s32 missing[16];
    s32 x;
    s32 z;
    s32 id;
    s32 n;
    s32 i;
    s32 j;
    s32 k;
    Unk800D335C *p;
    Unk800D335C *h;
    u8 c;

    n = 0;
    h = &D_800D3358;
    func_800BC3C4(arg0, &x, &z);
    for (j = 0; j < 4; j++) {
        for (i = 0; i < 4; i++) {
            func_800BC1FC(x + (i << 14), z - (j << 14), &id);
            p = h->unk4;
            for (k = 0; k < 16; k++, p = p->unk4) {
                if (p->unkC == id) {
                    c = D_800D30F8[p->unkC];
                    if (c & 0x3F) {
                        func_800C1DA4(c);
                    }
                    p->unk0->unk4 = p->unk4;
                    p->unk4->unk0 = p->unk0;
                    p->unk4 = h->unk4;
                    p->unk0 = h;
                    h->unk4->unk0 = p;
                    h->unk4 = p;
                    break;
                }
            }
            if (k == 16) {
                missing[n++] = id;
            }
        }
    }
    for (k = 0; k < n; k++) {
        if (k == 4) {
            D_800D3338++;
    D_800D3338 %= 4;
        }
        func_800BCA6C(missing[k]);
        c = D_800D30F8[missing[k]];
        if (c & 0x3F) {
            func_800C1DA4(c);
        }
    }
    func_800BD5D0();
    func_800C1F3C();
    D_800D3338++;
    D_800D3338 %= 4;
}

/* request texture pack arg0: reuse a free or matching slot and move it behind the loaded ones */
s32 func_800BCA6C(s16 arg0) {
    Unk800D335C *prev;
    Unk800D335C *p;
    u32 n;

    n = 0;
    prev = &D_800D3358;
    while (prev->unk4->unkE == 1) {
        if (prev->unk4->unkC == arg0) {
            return 0;
        }
        prev = prev->unk4;
        n++;
    }
    p = prev->unk4;
    do {
        if (p->unkC == -1) {
            if (n >= 16) {
                p->unkC = arg0;
                p->unk0->unk4 = p->unk4;
                p->unk4->unk0 = p->unk0;
                p->unk4 = prev->unk4;
                p->unk0 = prev;
                D_800D33F0++;
                prev->unk4->unk0 = p;
                prev->unk4 = p;
            } else {
                p->unkC = arg0;
                p->unkE = 0;
            }
            break;
        }
        if (p->unkC == arg0) {
            p->unk0->unk4 = p->unk4;
            p->unk4->unk0 = p->unk0;
            p->unk4 = prev->unk4;
            p->unk0 = prev;
            prev->unk4->unk0 = p;
            prev->unk4 = p;
            break;
        }
        p = p->unk4;
        n++;
    } while (p != &D_800D3340);
    if (p != &D_800D3340) {
        p->unkF = D_800D3338;
    }
    return n != 32;
}

void func_800BCBDC(void) {
    WmCell a;
    WmCell b;
    Unk800D335C *n;
    u8 *cell;
    s32 id;
    s16 id2;
    s32 i;
    s32 r;
    s32 base;
    Unk800D335C *h;

    if (D_800CF454 == 0) {
        func_800BD150(D_800D33F8, &D_800D3320[D_800CD094], &D_800D3490[D_800CD094]);
        if (D_800D3490[D_800CD094] == 0) {
            goto end;
        }
        for (i = 0; i < D_800D3320[D_800CD094]; i++) {
            n = D_800D33F8[i];
            h = &D_800D3358;
            id2 = n->unkC;
            id = n->unkC;
            n->unk0->unk4 = n->unk4;
            n->unk4->unk0 = n->unk0;
            n->unk4 = h->unk4;
            n->unk0 = &D_800D3358;
            cell = &D_800D30F8[id];
            h->unk4->unk0 = n;
            h->unk4 = n;
            n->unkE = 1;
            if (D_800D3490[D_800CD094] == 1) {
                func_800BC450(id, 0, &a);
                func_800BC450(id, 1, &b);
            } else {
                func_800BC490(id, 0, &a);
                func_800BC490(id, 1, &b);
            }
            if ((*cell & 0x80) && b.unk2 == 0) {
                if (D_800CCE50 & 0x10) {
                    func_80015C38(D_800A7F94, id2);
                }
                *cell &= 0x7F;
            }
            r = (*cell & 0x3F) ? func_800C2050(*cell, 0) : 0;
            n->unk10 = 1;
            if (!(*cell & 0x80)) {
                D_800D3410[i * 4 + 0].unk0 = a.unk2 << 11;
                D_800D3410[i * 4 + 0].unk4 = n->unk8;
                D_800D3410[i * 4 + 1].unk0 = a.unk3 << 11;
                D_800D3410[i * 4 + 1].unk4 = (void *)r;
                D_800D3410[i * 4 + 2].unk0 = b.unk2 << 11;
                D_800D3410[i * 4 + 2].unk4 = NULL;
                D_800D3410[i * 4 + 3].unk0 = b.unk3 << 11;
                D_800D3410[i * 4 + 3].unk4 = NULL;
                if (D_800D3410[i * 4 + 1].unk0 == 0) {
                    D_800D3410[i * 4 + 1].unk4 = NULL;
                }
                if (D_800D3410[i * 4 + 0].unk0 == 0) {
                    D_800D3410[i * 4 + 0].unk4 = NULL;
                    n->unk10 = 0;
                    D_800D3740[i] = NULL;
                    D_800D3310[i] = NULL;
                } else {
                    D_800D3740[i] = D_800D3410[i * 4 + 0].unk4;
                    D_800D3310[i] = D_800D3410[i * 4 + 1].unk4;
                }
            } else {
                D_800D3410[i * 4 + 0].unk0 = a.unk2 << 11;
                D_800D3410[i * 4 + 0].unk4 = NULL;
                D_800D3410[i * 4 + 1].unk0 = a.unk3 << 11;
                D_800D3410[i * 4 + 1].unk4 = NULL;
                D_800D3410[i * 4 + 2].unk0 = b.unk2 << 11;
                D_800D3410[i * 4 + 2].unk4 = n->unk8;
                D_800D3410[i * 4 + 3].unk0 = b.unk3 << 11;
                D_800D3410[i * 4 + 3].unk4 = (void *)r;
                if (D_800D3410[i * 4 + 3].unk0 == 0) {
                    D_800D3410[i * 4 + 3].unk4 = NULL;
                }
                if (D_800D3410[i * 4 + 2].unk0 == 0) {
                    D_800D3410[i * 4 + 2].unk4 = NULL;
                    n->unk10 = 0;
                    D_800D3740[i] = NULL;
                    D_800D3310[i] = NULL;
                } else {
                    D_800D3740[i] = D_800D3410[i * 4 + 2].unk4;
                    D_800D3310[i] = D_800D3410[i * 4 + 3].unk4;
                }
            }
        }
        if (D_800D3490[D_800CD094] == 1) {
            func_800BC450(D_800D33F8[0]->unkC, 0, &a);
            base = D_800CF5A4;
        } else {
            func_800BC490(D_800D33F8[0]->unkC, 0, &a);
            base = D_800CF5A8;
        }
        D_800D3728 = 0;
        for (i = 0; i < D_800D3320[D_800CD094] * 4; i++) {
            if (D_800D3410[i].unk0 != 0) {
                D_800D3370[D_800D3728].unk4 = D_800D3410[i].unk4;
                D_800D3370[D_800D3728++].unk0 = D_800D3410[i].unk0;
            }
        }
        D_800CF454 = 1;
        if (D_800D3728 != 0) {
            func_80022CC4(a.unk0 + base, D_800D3370, D_800D3728, 0);
        }
    }
end:
    if (D_800CF454 == 1 && func_80022B18() == 0) {
        func_800BD3F8();
    }
}

u8 func_800BD150(Unk800D335C **out, s32 *count, s32 *dir) {
    Unk800D335C *h[10];
    Unk800D335C *v[10];
    s32 hhi;
    s32 hlo;
    s32 vhi;
    s32 vlo;
    s32 i;
    Unk800D335C *p;

    hhi = 4;
    hlo = 4;
    *dir = 0;
    *count = 0;
    vhi = 4;
    vlo = 4;
    for (p = D_800D3358.unk4; p->unk8 != NULL; p = p->unk4) {
        if (p->unkE == 0 && p->unkC != -1 && p->unkF == D_800CD098) {
            h[hlo] = p;
            h[hhi] = p;
            v[vlo] = p;
            v[vhi] = p;
            for (i = 0; i < 4; i++) {
                for (p = D_800D3358.unk4; p->unk8 != NULL; p = p->unk4) {
                    if (p->unkE == 0) {
                        if (h[hlo]->unkC - 1 == p->unkC) {
                            h[--hlo] = p;
                        }
                        if (h[hhi]->unkC + 1 == p->unkC) {
                            h[++hhi] = p;
                        }
                        if (v[vlo]->unkC - 24 == p->unkC) {
                            v[--vlo] = p;
                        }
                        if (v[vhi]->unkC + 24 == p->unkC) {
                            v[++vhi] = p;
                        }
                    }
                }
            }
            if (hhi - hlo < vhi - vlo) {
                *dir = 24;
                for (i = vlo; i <= vhi; i++) {
                    out[(*count)++] = v[i];
                }
            } else {
                *dir = 1;
                for (i = hlo; i <= hhi; i++) {
                    out[(*count)++] = h[i];
                }
            }
            break;
        }
    }
    D_800CD098++;
    D_800CD098 %= 4;
    return *count;
}

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

/* initialise the texture list */
void func_800BD674(void) {
    u32 i;

    D_800D3790 = 0;
    D_800D3798[0].unk0 = &D_800D3758;
    D_800D3798[0].unkC = &D_800D3AB8[0];
    D_800D3798[0].unk4 = &D_800D3798[1];
    D_800D3798[0].unk8 = NULL;
    D_800D3798[0].unk12 = 0;
    D_800D3798[0].unk10 = -1;
    D_800D3798[0].unk13 = 0;
    for (i = 1; i < 39; i++) {
        D_800D3798[i].unk0 = &D_800D3798[i - 1];
        D_800D3798[i].unk4 = &D_800D3798[i + 1];
        D_800D3798[i].unk8 = NULL;
        D_800D3798[i].unkC = NULL;
        D_800D3798[i].unk12 = 0;
        D_800D3798[i].unk10 = -1;
        D_800D3798[i].unk13 = -1;
    }
    for (i = 1; i < 20; i++) {
        D_800D3798[i].unkC = &D_800D3AB8[i];
        D_800D3798[i].unk13 = i;
    }
    i = 39;
    D_800D3798[i].unk0 = &D_800D3798[i - 1];
    D_800D3798[i].unk4 = &D_800D3BF8;
    D_800D3798[i].unk8 = NULL;
    D_800D3798[i].unkC = NULL;
    D_800D3798[i].unk12 = 0;
    D_800D3798[i].unk10 = -1;
    D_800D3798[i].unk13 = -1;
    D_800D3758.unk0 = NULL;
    D_800D3758.unk4 = &D_800D3798[0];
    D_800D3BF8.unk4 = &D_800D3798[20];
    D_800D3BF8.unk0 = NULL;
}

void func_800BD78C(void) {
    Unk800D375C *n;
    Unk800D3AB8 *pos;
    MATRIX m;
    Unk800A7FB8 quad = D_800A7FB8;
    long sxy[4];
    VECTOR a;
    VECTOR b;
    long p;
    long flag;
    long xy3;
    u8 snd;
    s32 far;
    s32 k;
    u32 *ot;
    POLY_F4 *f4;

    n = D_800D3758.unk4;
    while (n->unk10 != -1) {
        pos = n->unkC;
        D_800D3770.t[0] = pos->unk0[0] - D_800CE108[0];
        D_800D3770.t[1] = pos->unk0[1] - D_800CE108[1];
        D_800D3770.t[2] = pos->unk0[2] - D_800CE108[2];
        CompMatrix(D_800CF448, &D_800D3770, &m);
        SetRotMatrix(&m);
        SetTransMatrix(&m);
        func_800BBE24((VECTOR *)pos, &a, &b, 0);
        far = b.vz < -10000 || b.vz > 11000;
        k = n->unk10 % D_800D32F0 + n->unk10 / D_800D32F0;
        if (n->unk12 == 2) {
            func_800BA2DC((u8 *)n->unk8, (n->unk10 % D_800D32F0 + n->unk10 / D_800D32F0) % 2, far);
            if (RotTransPers4(&quad.v[0], &quad.v[1], &quad.v[2], &quad.v[3], &sxy[0], &sxy[1], &sxy[2], &sxy[3], &p, &flag) < 7000
                && D_800CE3DC->unk14.vy > -7000) {
                f4 = (POLY_F4 *)D_800CF404;
                ot = (u32 *)D_800CF460[*D_800CF45C];
                *(s32 *)&f4->x0 = sxy[0];
                *(s32 *)&f4->x1 = sxy[1];
                *(s32 *)&f4->x2 = sxy[2];
                xy3 = sxy[3];
                setlen(f4, 5);
                setRGB0(f4, 0, 0, 0);
                f4->code = 0x28;
                *(s32 *)&f4->x3 = xy3;
                addPrim(&ot[0xFFA], f4);
                f4++;
                D_800CF404 = (s32)f4;
            }
        }
        if (n->unk8 != NULL) {
            snd = n->unk8->unk21;
            if (snd != 0) {
                func_800C5A48(snd);
            }
        }
        n = n->unk4;
    }
}

void func_800BDAB0(void) {
    VECTOR pos;
    VECTOR p2;
    s16 sx[20];
    s16 sy[20];
    s32 z[20];
    VECTOR d;
    WmNearCell tmp;
    MATRIX m;
    SVECTOR sv;
    VECTOR out;
    s32 cell;
    long flag;
    long p;
    long sxy;
    s32 n;
    s32 i;
    s32 j;
    s32 k;
    s32 hit;
    s32 cnt;
    s32 cnt2;
    s32 last;
    s32 id;
    WmTexEnt *e;
    SVECTOR *verts;
    WmScrRect *r;
    WmNearCell *c;
    Unk800D375C *nd;
    s32 lim;
    s32 t;

    pos.vy = 0;
    func_800BD674();
    func_80064CDC(10, 10);
    n = 0;
    if (D_800CE3C4 < 6) {
        if (D_800CE3DC->unk14.vy >= -0x9C3) {
            D_800CD09C[1].y0 = D_800CD09C[0].y0 = -0x3C;
        } else {
            D_800CD09C[1].y0 = D_800CD09C[0].y0 = -0x1E;
        }
    }
    for (j = -3; j < 4; j++) {
        for (i = -3; i < 4; i++) {
            func_800BC130(D_800CE108[0] + (i << 13) + D_800CE1D0[0] * 2,
                          D_800CE108[2] - (j << 13) + D_800CE1D0[2] * 2, &cell);
            e = (WmTexEnt *)func_800BE4A8(cell, NULL);
            if (e != NULL) {
                func_800BC2C8(cell, &pos.vx, &pos.vz);
                D_800D3770.t[0] = pos.vx - D_800CE108[0];
                D_800D3770.t[1] = pos.vy - D_800CE108[1];
                D_800D3770.t[2] = pos.vz - D_800CE108[2];
                CompMatrix(D_800CF448, &D_800D3770, &m);
                SetRotMatrix(&m);
                SetTransMatrix(&m);
                hit = 0;
                if (m.t[2] >= -0x1D4B) {
                    k = (func_800AE7F0() == 0) * 2;
                    if (D_800CE3C8->unk0 == 1) {
                        k += 4;
                    }
                    if (D_800CE3C8->unk0 == 2) {
                        k += 8;
                    }
                    verts = e->unk4;
                    for (cnt = 0; cnt < e->unk20; cnt++) {
                        sv = verts[e->unkC[cnt]];
                        RotTrans(&sv, &out, &flag);
                        sv.vy = sv.vy * (0x1000 - (out.vz >> 4)) >> 12;
                        z[cnt] = RotTransPers(&sv, &sxy, &p, &flag);
                        sy[cnt] = (u32)sxy >> 16;
                        sx[cnt] = sxy;
                        if (z[cnt] < 0x898) {
                            k |= 1;
                        } else {
                            k &= ~1;
                        }
                        if ((sx[cnt] >= D_800CD09C[k].x0) & (D_800CD09C[k].x1 >= sx[cnt]) & (sy[cnt] >= D_800CD09C[k].y0) & (D_800CD09C[k].y1 >= sy[cnt])) {
                            hit = 1;
                            break;
                        }
                    }
                }
                if (hit == 1) {
                    p2.vx = pos.vx + 0x1000;
                    p2.vy = pos.vy;
                    p2.vz = pos.vz - 0x1000;
                    d.vx = p2.vx - D_800CE198[0];
                    d.vz = p2.vz - D_800CE198[2];
                    if (d.vx > 0xFFFF) {
                        d.vx -= D_800D32D8;
                    }
                    if (d.vx < -0xFFFF) {
                        d.vx += D_800D32D8;
                    }
                    if (d.vz > 0xFFFF) {
                        d.vz -= D_800D32DC;
                    }
                    if (d.vz < -0xFFFF) {
                        d.vz += D_800D32DC;
                    }
                    D_800D3C10[n].unk8 = SquareRoot0((d.vx >> 2) * (d.vx >> 2) + (d.vz >> 2) * (d.vz >> 2));
                    D_800D3C10[n].unk4 = e;
                    D_800D3C10[n].unk0 = cell;
                    n++;
                }
            }
        }
    }
    for (j = 0; j < n - 1; j++) {
        for (i = n - 1; j < i; i = t) {
            t = i - 1;
            if (D_800D3C10[t].unk8 > D_800D3C10[i].unk8) {
                tmp = D_800D3C10[i];
                D_800D3C10[i] = D_800D3C10[t];
                D_800D3C10[t] = tmp;
            }
        }
    }
    lim = 16;
    if (D_800CE3C4 == 7) {
        lim = 14;
    }
    if (D_800CCE4B == 0) {
        lim++;
    }
    if (lim < n) {
        n = lim;
    }
    cnt2 = 0;
    last = -1;
    for (i = 0; i < n; i++) {
        id = D_800D3C10[i].unk4->unk1;
        func_800BE138(D_800D3C10[i].unk0);
        if (id != 0) {
            if (cnt2 < 2) {
                func_800C2230(id);
            }
            if (id != last) {
                cnt2++;
            }
            if (cnt2 == 1) {
                last = id;
            }
        }
    }
    func_800C23CC();
    func_800C24D8();
    func_800BE6B4();
    func_800BE238();
    nd = D_800D3758.unk4;
    D_800D1FD0 = 0;
    while (nd->unk10 != -1) {
        {
            u8 s = nd->unk8->unk21;
            if (s != 0) {
                func_800C69F4(s);
            }
        }
        nd = nd->unk4;
    }
}

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

void func_800BE238(void) {
    Unk800D375C *n;
    Unk800D335C *p;
    u32 i;
    u32 j;
    s32 id;
    s16 m;
    s32 k;

    n = D_800D3758.unk4;
    for (i = 0; i < 20; i++, n = n->unk4) {
        if (n->unk10 < 0) {
            return;
        }
        func_800BC354(n->unk10, &id);
        n->unk12 = 0;
        p = D_800D3358.unk4;
        for (j = 0; j < 16; j++, p = p->unk4) {
            if (id == p->unkC) {
                if (p->unkE == 2) {
                    n->unk12 = 2;
                    func_800BC2C8(n->unk10, &n->unkC->unk0[0], &n->unkC->unk0[2]);
                    n->unkC->unk0[1] = 0;
                    m = n->unk10 % 2;
                    k = m + (n->unk10 / D_800D32F0) % 2 * 2;
                    if (p->unk10 != 0) {
                        n->unk8 = p->unk8->unk8[k];
                        n->unk8->unk1 = p->unk8->unk2;
                    } else {
                        n->unk8 = D_800D32EC->unk8[k];
                        n->unk8->unk1 = 0;
                    }
                    break;
                }
                n->unk12 = 2;
                n->unkC->unk0[1] = 0;
                m = n->unk10 % 2;
                k = m + (n->unk10 / D_800D32F0) % 2 * 2;
                func_800BC2C8(n->unk10, &n->unkC->unk0[0], &n->unkC->unk0[2]);
                n->unk8 = D_800D32EC->unk8[k];
                n->unk8->unk1 = 0;
            }
        }
    }
}

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
                return (s32)p->unk8->unk8[k];
            }
            return (s32)D_800D32EC->unk8[k];
        }
    }
    if (arg1 != NULL) {
        *arg1 = 1;
    }
    m = arg0 % 2;
    k = m + (arg0 / D_800D32F0) % 2 * 2;
    return (s32)D_800D32EC->unk8[k];
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

s32 func_800BE748(VECTOR *arg0, u32 *arg1, s32 *arg2, Unk800CE288 *arg3, u16 *arg4) {
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

/* navi map markers */
void func_800BE984(void) {
    s32 x;
    s32 z;
    u8 page;

    if (D_800D3E75 >= 0) {
        func_800C0F34();
    }
    if (D_800D3E80 == 0) {
        return;
    }
    page = D_800D3E74;
    switch (D_800D3E60) {
    case 1:
        if (D_800CE3D0 != NULL && D_800CE3D0->unkE == 1 && (D_800CE3D0->unkF & 1) &&
            !((u8)D_800CE3C4 >= 7 && (u8)D_800CE3C4 < 10)) {
            CVECTOR c0 = D_800A7FF0;
            CVECTOR c1 = D_800A7FF4;
            func_800C1B00(D_800CE3D0->unk14.vx, D_800CE3D0->unk14.vz, &x, &z, page * 2);
            func_800BF544(x, z, 1, 1, c0, c1, 0x40, 0xC8, 0x82, 1);
        }
        if (D_800CE268 != NULL && D_800CE268->unkE == 1 && (D_800CE268->unkF & 1) &&
            !((u8)D_800CE3C4 >= 1 && (u8)D_800CE3C4 < 7)) {
            CVECTOR c0 = D_800A7FF8;
            CVECTOR c2 = D_800A7FFC;
            func_800C1B00(D_800CE268->unk14.vx, D_800CE268->unk14.vz, &x, &z, page * 2);
            func_800BF544(x, z, 1, 1, c0, c2, 0x40, 0xC8, 0x82, 1);
        }
        func_800C0888();
        if (D_8006794C->unk1C->unk844 != 0) {
            func_800C16C0();
        }
        break;
    case 2:
        if (D_800D3E64 != 0) {
            if (D_800CE3D0 != NULL && D_800CE3D0->unkE == 1 && (D_800CE3D0->unkF & 1) &&
                !((u8)D_800CE3C4 >= 7 && (u8)D_800CE3C4 < 10)) {
                CVECTOR c0 = D_800A7FF0;
                CVECTOR c3 = D_800A7FF4;
                func_800C1B00(D_800CE3D0->unk14.vx, D_800CE3D0->unk14.vz, &x, &z, page * 2 + 1);
                func_800BF544(x, z, 1, 1, c0, c3, 0x40, 0x28, 9, 1);
            }
            if (D_800CE268 != NULL && D_800CE268->unkE == 1 && (D_800CE268->unkF & 1) &&
                !((u8)D_800CE3C4 >= 1 && (u8)D_800CE3C4 < 7)) {
                CVECTOR c0 = D_800A7FF8;
                CVECTOR c1 = D_800A7FFC;
                func_800C1B00(D_800CE268->unk14.vx, D_800CE268->unk14.vz, &x, &z, page * 2 + 1);
                func_800BF544(x, z, 1, 1, c0, c1, 0x40, 0x28, 9, 1);
            }
            if (D_800D3E77 != 0) {
                func_8004E76C(5);
                func_800515B8();
                func_800512B8((D_800CD104 >> 12) + 18, (D_800CD108 >> 12) + 6);
                func_800513CC(1);
                func_800515EC();
                func_8004E76C(2);
            }
            func_800C1428();
        }
        func_800BF978();
        break;
    }
}

INCLUDE_ASM("asm/us/ovl_0c/nonmatchings/ovl_0c", func_800BEECC);

INCLUDE_ASM("asm/us/ovl_0c/nonmatchings/ovl_0c", func_800BF544);

#define TILE(x0_, y0_, x1_, y1_, u0_, v0_, u1_, v1_, cl)                                   \
    setXY4((POLY_FT4 *)D_800CF404, x0_, y0_, x1_, y0_, x0_, y1_, x1_, y1_);              \
    setUV4((POLY_FT4 *)D_800CF404, u0_, v0_, u1_, v0_, u0_, v1_, u1_, v1_);              \
    setRGB0((POLY_FT4 *)D_800CF404, D_800CD734, D_800CD738, D_800CD73C);                 \
    ((POLY_FT4 *)D_800CF404)->tpage = 0xA;                                                \
    ((POLY_FT4 *)D_800CF404)->clut = cl;                                                  \
    setPolyFT4((POLY_FT4 *)D_800CF404);                                                   \
    addPrim(&ot[10], (POLY_FT4 *)D_800CF404);                                             \
    D_800CF404 += sizeof(POLY_FT4);

void func_800BF978(void) {
    Unk800A7FB8 arrow;
    VECTOR v[4];
    SVECTOR rot;
    SVECTOR rot0;
    MATRIX m;
    s32 x;
    s32 y;
    s32 *px;
    s32 *py;
    u32 *ot;
    s32 i;
    s32 c;
    POLY_FT4 *p;
    s32 a;
    s32 b;
    s32 ax;
    s32 ay;
    s32 bx;
    s32 by;
    s32 x1;
    s32 y1;
    s32 x2;
    s32 y2;

    ot = (u32 *)D_800CF460[*D_800CF45C];
    arrow = D_800A8000;
    func_8001D898(&rot0, 0, 8);
    rot0.vz = D_800CE3DC->unk14.unk22 - 0x400;
    rot = rot0;
    func_80030AF4(&rot, &m);
    for (i = 0; i < 4; i++) {
        ApplyMatrix(&m, &arrow.v[i], &v[i]);
    }
    px = &x;
    py = &y;
    func_800C1B00(D_800CE3DC->unk14.vx, D_800CE3DC->unk14.vz, px, py, D_800D3E74 * 2 + 1);
    c = (func_8004BB80(D_800CF4B0 << 6) >> 7) + 0x80;
    p = (POLY_FT4 *)D_800CF404;
    setRGB0(p, c, c, c);
    p->x0 = x + v[0].vx + 0x28;
    p->y0 = y + v[0].vy + 9;
    p->x1 = x + v[1].vx + 0x28;
    p->y1 = y + v[1].vy + 9;
    p->x2 = x + v[2].vx + 0x28;
    p->y2 = y + v[2].vy + 9;
    p->x3 = x + v[3].vx + 0x28;
    p->y3 = y + v[3].vy + 9;
    setUV4(p, 0x20, 0xD0, 0x40, 0xD0, 0x20, 0xD7, 0x40, 0xD7);
    p->tpage = GetTPage(0, 0, 0x300, 0xD0);
    p->clut = GetClut(0x280, 0xF5);
    setPolyFT4(p);
    if (D_800D3E64) {
        addPrim(&ot[10], p);
    }
    p++;
    setRGB0(p, 0x80, 0x80, 0x80);
    p->x0 = x + v[0].vx + 0x28;
    p->y0 = y + v[0].vy + 9;
    p->x1 = x + v[1].vx + 0x28;
    p->y1 = y + v[1].vy + 9;
    p->x2 = x + v[2].vx + 0x28;
    p->y2 = y + v[2].vy + 9;
    p->x3 = x + v[3].vx + 0x28;
    p->y3 = y + v[3].vy + 9;
    setUV4(p, 0, 0xD0, 0x20, 0xD0, 0, 0xD7, 0x20, 0xD7);
    p->tpage = GetTPage(0, 0, 0x300, 0xD0);
    p->clut = GetClut(0x280, 0xF5);
    setPolyFT4(p);
    if (D_800D3E64) {
        addPrim(&ot[10], p);
    }
    p++;
    D_800CF404 = (s32)p;
    a = 0xF00 - D_800CE0F0->unkC;
    b = 0x1100 - D_800CE0F0->unkC;
    if (a < 0) {
        a += 0x1000;
    }
    if (b < 0) {
        b += 0x1000;
    }
    a %= 0x1000;
    b %= 0x1000;
    ax = func_8004BB80(a) * 0x18 >> 12;
    ay = func_8004BB7C(a) * 0x18 >> 12;
    bx = func_8004BB80(b) * 0x18 >> 12;
    by = func_8004BB7C(b) * 0x18 >> 12;
    func_800C1B00(D_800CE3DC->unk14.vx, D_800CE3DC->unk14.vz, px, py, D_800D3E74 * 2 + 1);
    x1 = x + ax + 0x28;
    y1 = y + ay + 9;
    x2 = x + bx + 0x28;
    y2 = y + by + 9;
    D_800D3E7C = (POLY_G3 *)D_800CF404;
    D_800D3E7C->r0 = D_800CD728;
    D_800D3E7C->g0 = D_800CD72C;
    D_800D3E7C->b0 = D_800CD730;
    D_800D3E7C->r1 = 0;
    D_800D3E7C->g1 = 0;
    D_800D3E7C->b1 = 0;
    D_800D3E7C->r2 = 0;
    D_800D3E7C->g2 = 0;
    D_800D3E7C->b2 = 0;
    D_800D3E7C->x0 = x + 0x28;
    D_800D3E7C->y0 = y + 9;
    D_800D3E7C->x1 = x1;
    D_800D3E7C->y1 = y1;
    D_800D3E7C->x2 = x2;
    D_800D3E7C->y2 = y2;
    setPolyG3(D_800D3E7C);
    setSemiTrans(D_800D3E7C, 1);
    if (D_800D3E64) {
        addPrim(&ot[10], D_800D3E7C);
    }
    D_800D3E7C++;
    D_800CF404 = (s32)D_800D3E7C;
    setDrawTPage((DR_TPAGE *)D_800CF404, 0, 1, GetTPage(0, 1, 0, 0));
    addPrim(&ot[10], (DR_TPAGE *)D_800CF404);
    D_800CF404 += sizeof(DR_TPAGE);
    TILE(0x20, 0xB5, 0x4E, 0xD9, 0, 0xB4, 0x2E, 0xD7, 0x3C6B)
    if (D_800D3E74 == 0) {
        TILE(0x28, 9, 0x118, 0xD1, 8, 8, 0xF7, 0xCF, 0x3C6A)
    } else {
        TILE(0x28, 9, 0x93, 0x42, 8, 8, 0x73, 0x41, 0x3C6C)
        TILE(0x93, 9, 0x118, 0x5C, 0x73, 8, 0xF8, 0x5B, 0x3CEC)
        TILE(0x28, 0x42, 0x93, 0xD1, 8, 0x41, 0x73, 0xD0, 0x3CAC)
        TILE(0x93, 0x5C, 0x118, 0xD1, 0x73, 0x5B, 0xF8, 0xD0, 0x3C2C)
    }
    TILE(0x20, 1, 0x120, 0xD9, 0, 0, 0xFF, 0xD8, 0x3C6B)
    setXY4((POLY_F4 *)D_800CF404, 0, 0, 0x140, 0, 0, 0xDA, 0x140, 0xDA);
    setRGB0((POLY_F4 *)D_800CF404, 0, 0, 0);
    setPolyF4((POLY_F4 *)D_800CF404);
    addPrim(&ot[10], (POLY_F4 *)D_800CF404);
    D_800CF404 += sizeof(POLY_F4);
}

void func_800C0888(void) {
    Unk800A7FB8 arrow;
    VECTOR v[4];
    SVECTOR rot;
    SVECTOR rot0;
    MATRIX m;
    s32 x;
    s32 y;
    s32 *px;
    s32 *py;
    u32 *ot;
    s32 i;
    s32 c;
    POLY_FT4 *p;
    s32 a;
    s32 b;
    s32 ax;
    s32 ay;
    s32 bx;
    s32 by;
    s32 x1;
    s32 y1;
    s32 x2;
    s32 y2;

    ot = (u32 *)D_800CF460[*D_800CF45C];
    arrow = D_800A8020;
    func_8001D898(&rot0, 0, 8);
    rot0.vz = D_800CE3DC->unk14.unk22 - 0x400;
    rot = rot0;
    c = (func_8004BB80(D_800CF4B0 << 6) >> 7) + 0x80;
    func_80030AF4(&rot, &m);
    for (i = 0; i < 4; i++) {
        ApplyMatrix(&m, &arrow.v[i], &v[i]);
    }
    px = &x;
    py = &y;
    func_800C1B00(D_800CE3DC->unk14.vx, D_800CE3DC->unk14.vz, px, py, D_800D3E74 * 2);
    p = (POLY_FT4 *)D_800CF404;
    setRGB0(p, c, c, c);
    p->x0 = x + v[0].vx + 200;
    p->y0 = y + v[0].vy + 130;
    p->x1 = x + v[1].vx + 200;
    p->y1 = y + v[1].vy + 130;
    p->x2 = x + v[2].vx + 200;
    p->y2 = y + v[2].vy + 130;
    p->x3 = x + v[3].vx + 200;
    p->y3 = y + v[3].vy + 130;
    setUV4(p, 0, 0xD0, 0x20, 0xD0, 0, 0xD7, 0x20, 0xD7);
    p->tpage = GetTPage(0, 0, 0x300, 0xD0);
    p->clut = GetClut(0x280, 0xF5);
    setPolyFT4(p);
    addPrim(&ot[10], p);
    p++;
    D_800CF404 = (s32)p;
    a = 0xF00 - D_800CE0F0->unkC;
    b = 0x1100 - D_800CE0F0->unkC;
    if (a < 0) {
        a += 0x1000;
    }
    if (b < 0) {
        b += 0x1000;
    }
    a %= 0x1000;
    b %= 0x1000;
    ax = func_8004BB80(a) * 12 >> 12;
    ay = func_8004BB7C(a) * 12 >> 12;
    bx = func_8004BB80(b) * 12 >> 12;
    by = func_8004BB7C(b) * 12 >> 12;
    func_800C1B00(D_800CE3DC->unk14.vx, D_800CE3DC->unk14.vz, px, py, D_800D3E74 * 2);
    D_800D3E7C = (POLY_G3 *)D_800CF404;
    x1 = x + ax + 200;
    y1 = y + ay + 130;
    x2 = x + bx + 200;
    y2 = y + by + 130;
    D_800D3E7C->r0 = 0x8E;
    D_800D3E7C->g0 = 0xC8;
    D_800D3E7C->b0 = 0;
    D_800D3E7C->r1 = 0;
    D_800D3E7C->g1 = 0;
    D_800D3E7C->b1 = 0;
    D_800D3E7C->r2 = 0;
    D_800D3E7C->g2 = 0;
    D_800D3E7C->b2 = 0;
    D_800D3E7C->x0 = x + 200;
    D_800D3E7C->y0 = y + 130;
    D_800D3E7C->x1 = x1;
    D_800D3E7C->y1 = y1;
    D_800D3E7C->x2 = x2;
    D_800D3E7C->y2 = y2;
    setPolyG3(D_800D3E7C);
    setSemiTrans(D_800D3E7C, 1);
    addPrim(&ot[10], D_800D3E7C);
    D_800D3E7C++;
    D_800CF404 = (s32)D_800D3E7C;
    setDrawTPage((DR_TPAGE *)D_800CF404, 0, 1, GetTPage(0, 1, 0, 0));
    addPrim(&ot[10], (DR_TPAGE *)D_800CF404);
    D_800CF404 += sizeof(DR_TPAGE);
    setXY4((POLY_FT4 *)D_800CF404, 200, 130, 296, 130, 200, 210, 296, 210);
    setUV4((POLY_FT4 *)D_800CF404, 0, 0x80, 0x5F, 0x80, 0, 0xCF, 0x5F, 0xCF);
    setRGB0((POLY_FT4 *)D_800CF404, 0x80, 0x80, 0x80);
    ((POLY_FT4 *)D_800CF404)->tpage = 0xC;
    ((POLY_FT4 *)D_800CF404)->clut = 0x3C69;
    setPolyFT4((POLY_FT4 *)D_800CF404);
    setSemiTrans((POLY_FT4 *)D_800CF404, 1);
    addPrim(&ot[10], (POLY_FT4 *)D_800CF404);
    D_800CF404 += sizeof(POLY_FT4);
}

void func_800C0F34(void) {
    u32 *ot;
    POLY_FT4 *p;
    u32 a;
    u32 b;

    ot = (u32 *)D_800CF460[*D_800CF45C];
    D_800D3E88 = 0;
    a = D_800D3E6C + 0x78;
    b = D_800D3E6C + 0xB4;
    if ((u32)D_800D3E6C < (u32)D_800CF4AC) {
        D_800D3E88 = 1;
    }
    if (a < (u32)D_800CF4AC) {
        D_800D3E88 = 2;
    }
    if (b < (u32)D_800CF4AC) {
        D_800D3E88 = 3;
    }
    if (b <= (u32)D_800CF4AC) {
        D_80073998[0] &= ~0x02080000;
    } else {
        D_80073998[0] |= 0x02080000;
    }
    if (D_800D3E88 == 1) {
        if (D_800D3E76 == 0) {
            *D_800CF430 += 10;
        }
        if (D_800D3E76 != 0x80) {
            D_800D3E76 += 4;
        }
    }
    if (D_800D3E88 == 2) {
        if (D_800D3E76 != 0) {
            D_800D3E76 -= 4;
        }
    }
    if (D_800D3E88 == 3) {
        D_800D3E75 = -1;
    }
    D_800D3E68 = (POLY_FT4 *)D_800CF404;
    if (D_800D3E76 != 0 && D_800D3E88 != 0) {
        setRGB0(D_800D3E68, D_800D3E76, D_800D3E76, D_800D3E76);
        if (D_800CCE40 == 0) {
            setXY4(D_800D3E68, 0x60, 0x32, 0xDF, 0x32, 0x60, 0x71, 0xDF, 0x71);
            setUV4(D_800D3E68, 0, 0x40, 0x7F, 0x40, 0, 0x7F, 0x7F, 0x7F);
        } else {
            setXY4(D_800D3E68, 0x20, 0x32, 0x11F, 0x32, 0x20, 0x51, 0x11F, 0x51);
            setUV4(D_800D3E68, 0, 0x40, 0xFF, 0x40, 0, 0x5F, 0xFF, 0x5F);
        }
        D_800D3E68->tpage = 0x2C;
        D_800D3E68->clut = 0x3D28;
        setlen(D_800D3E68, 9);
        setcode(D_800D3E68, 0x2C);
        setSemiTrans(D_800D3E68, 1);
        addPrim(&ot[10], D_800D3E68);
        D_800D3E68++;
        setRGB0(D_800D3E68, D_800D3E76, D_800D3E76, D_800D3E76);
        if (D_800CCE40 == 0) {
            setXY4(D_800D3E68, 0x60, 0x32, 0xDF, 0x32, 0x60, 0x71, 0xDF, 0x71);
            setUV4(D_800D3E68, 0x80, 0x40, 0xFF, 0x40, 0x80, 0x7F, 0xFF, 0x7F);
        } else {
            setXY4(D_800D3E68, 0x20, 0x32, 0x11F, 0x32, 0x20, 0x51, 0x11F, 0x51);
            setUV4(D_800D3E68, 0, 0x60, 0xFF, 0x60, 0, 0x7F, 0xFF, 0x7F);
        }
        D_800D3E68->tpage = 0x4C;
        D_800D3E68->clut = 0x3D28;
        setlen(D_800D3E68, 9);
        setcode(D_800D3E68, 0x2C);
        setSemiTrans(D_800D3E68, 1);
        p = D_800D3E68;
        setaddr(p, getaddr(&ot[10]));
        D_800D3E68 = p + 1;
        setaddr(&ot[10], p);
    }
    D_800CF404 = (s32)D_800D3E68;
}

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

/* draw the four compass frame pieces at (arg0, arg1) */
void func_800C1824(s32 arg0, s32 arg1) {
    POLY_FT4 *p;
    s16 clut;
    u32 *ot;
    s32 i;

    p = (POLY_FT4 *)D_800CF404;
    clut = GetClut(0x280, 0xF0);
    ot = (u32 *)D_800CF460[*D_800CF45C];
    for (i = 0; i < 4; i++) {
        SetPolyFT4(p);
        if (i < 2) {
            setXYWH(p, D_800CD744[i].unk0 + arg0, D_800CD744[i].unk2 + arg1, D_800CD744[i].unk8,
                    D_800CD744[i].unkA);
        } else {
            /* rotated a quarter turn */
            setXY4(p, D_800CD744[i].unk0 + arg0, D_800CD744[i].unk2 + arg1 + D_800CD744[i].unk8,
                   D_800CD744[i].unk0 + arg0, arg1 + D_800CD744[i].unk2 - (i != 2),
                   D_800CD744[i].unk0 + arg0 + D_800CD744[i].unkA,
                   D_800CD744[i].unk2 + arg1 + D_800CD744[i].unk8,
                   D_800CD744[i].unk0 + arg0 + D_800CD744[i].unkA, arg1 + D_800CD744[i].unk2 - (i != 2));
        }
        setUVWH(p, D_800CD744[i].unk4, D_800CD744[i].unk6, D_800CD744[i].unk8, D_800CD744[i].unkA);
        setRGB0(p, 0x80, 0x80, 0x80);
        p->tpage = 0xC;
        p->clut = clut;
        addPrim(&ot[10], p);
        p++;
    }
    D_800CF404 = (s32)p;
}

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

/* initialise the frame pack slot list */
void func_800C1C98(void) {
    u32 i;

    D_800D3EC0[0].unk0 = &D_800D3EA8;
    D_800D3EC0[0].unk4 = &D_800D3EC0[1];
    D_800D3EC0[0].unkE = 0;
    D_800D3EC0[0].unkC = -1;
    D_800D3EC0[0].unk8 = D_800CE3E8[0];
    for (i = 1; i < 5; i++) {
        D_800D3EC0[i].unk0 = &D_800D3EC0[i - 1];
        D_800D3EC0[i].unk4 = &D_800D3EC0[i + 1];
        D_800D3EC0[i].unk8 = NULL;
        D_800D3EC0[i].unkE = 0;
        D_800D3EC0[i].unkC = -1;
    }
    for (i = 1; i < 3; i++) {
        D_800D3EC0[i].unk8 = D_800CE3E8[i];
    }
    i = 5;
    D_800D3EC0[i].unk0 = &D_800D3EC0[i - 1];
    D_800D3EC0[i].unk4 = &D_800D3E98;
    D_800D3EC0[i].unkC = -1;
    D_800D3EC0[i].unk8 = NULL;
    D_800D3EC0[i].unkE = 0;
    D_800D3EA8.unk0 = NULL;
    D_800D3EA8.unk4 = &D_800D3EC0[0];
    D_800D3E98.unk4 = &D_800D3EC0[3];
    D_800D3E90 = 0;
    D_800D3E98.unk0 = NULL;
    D_800D3EB8 = 0;
}

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

/* find the loaded frame pack for (id, variant) */
s32 func_800C2050(s32 arg0, s32 arg1) {
    Unk800D3EAC *p = D_800D3EA8.unk4;
    s32 i;

    for (i = 0; i < 3; i++, p = p->unk4) {
        if ((p->unkC & 0x3F) == (arg0 & 0x3F) && p->unkE == (arg1 & 0xFF)) {
            return (s32)p->unk8;
        }
    }
    return 0;
}

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

void func_800C2134(void) {
    u32 i;
    WmLNode *cur;

    D_800D3F48[0].unk0 = &D_800D3F38;
    D_800D3F48[0].unkA = 1;
    D_800D3F48[0].unk4 = &D_800D3F48[1];
    D_800D3F48[0].unkB = 0;
    D_800D3F48[0].unk8 = -1;
    for (i = 1; i < 3; i++) {
        D_800D3F48[i].unk0 = &D_800D3F48[i - 1];
        D_800D3F48[i].unkA = 0;
        D_800D3F48[i].unkB = 0;
        D_800D3F48[i].unk4 = &D_800D3F48[i + 1];
        D_800D3F48[i].unk8 = -1;
    }
    for (i = 1; i < 2; i++) {
        D_800D3F48[i].unkA = i + 1;
    }
    D_800D3F48[3].unk0 = &D_800D3F48[2];
    cur = &D_800D3F48[3];
    cur->unk4 = &D_800D3F28;
    cur->unk8 = -1;
    cur->unkA = 0;
    cur->unkB = 0;
    D_800D3F38.unk0 = 0;
    D_800D3F38.unk4 = D_800D3F48;
    D_800D3F20 = 0;
    D_800D3F28.unk4 = &D_800D3F48[2];
    D_800D3F28.unk0 = 0;
    D_800D3F44 = 0;
}

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

void func_800C24D8(void) {
    WmLNode *p;
    u32 n;
    u32 i;
    s32 r;
    s32 id;
    s32 k;
    s16 *px;
    s16 *py;
    s16 *cx;
    s16 *cy;

    n = 0;
    p = D_800D3F38.unk4;
    for (i = 0; i < 2; i++, p = p->unk4) {
        if (p->unk8 != -1 && p->unkA != 0 && p->unkB == 0) {
            r = func_800C2050(p->unk8, 1);
            if (r != 0) {
                D_800D3F78[n].unk0 = r;
                D_800D3F78[n].unk5 = p->unkA;
                D_800D3F78[n].unk4 = p->unk8 & 0x3F;
                n++;
                p->unkB = 1;
            }
        }
    }
    for (i = 0; i < n; i++) {
        id = D_800D3F78[i].unk4;
        k = D_800D3F78[i].unk5 - 1;
        if (D_800CCE50 & 0x200) {
            func_80015C38(D_800A8118, (u8)(id + 0x60), D_800D3F78[i].unk0, k);
        }
        px = &D_800CDF14[k];
        py = &D_800CDF18[k];
        cx = &D_800CDF1C[k];
        cy = &D_800CDF20[k];
        func_800B96CC((s32 *)D_800D3F78[i].unk0, *px, *py, *cx, *cy);
        D_800D2FF8[id].unk8 = getTPage(1, 1, *px, *py);
        D_800D2FF8[id].unkA = getClut(*cx, *cy);
    }
}

void func_800C26DC(void) {
}

void func_800C26E4(void) {
}

void func_800C26EC(void) {
    s32 t;

    if (D_800D3F9C != 0) {
        func_800C2830();
    }
    if (D_800D3F94 >= 0) {
        D_800D3F94 += D_800D3F90;
        if (D_800D3F94 > 0x1000) {
            D_800D3F94 = 0x1000;
        }
        func_800C3954(&D_800D3FA0.unk5E0[1], &D_800D3FA0.unk5E0[3], &D_800D3FA0.unk5E0[4], D_800D3F94, D_800D3F94);
        func_800C3954(&D_800D3FA0.unk5E0[2], &D_800D3FA0.unk5E0[5], &D_800D3FA0.unk5E0[6], D_800D3F94, D_800D3F94);
        if (D_800D3F94 == 0x1000) {
            D_800D3F94 = -1;
        }
    }
    t = -D_800CE108[1];
    t -= 0x898;
    t <<= 12;
    t /= 4500;
    if (t > 0x1000) {
        t = 0x1000;
    }
    if (t < 0) {
        t = 0;
    }
    func_800C3954(&D_800D3FA0.unk5E0[0], &D_800D3FA0.unk5E0[1], &D_800D3FA0.unk5E0[2], t, t);
    func_800B9774();
    func_800B8664();
    func_8001CCB4((u8)D_800D3FA0.unk5E0[0].unk30[1].unk4[0], (u8)D_800D3FA0.unk5E0[0].unk30[1].unk4[1],
                  (u8)D_800D3FA0.unk5E0[0].unk30[1].unk4[2]);
}

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
            D_800D3FA0.unk0[3].unk30[0].unk0 = 0x7F58;
            D_800D3FA0.unk0[3].unk30[0].unk2 = 0x7F58;
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

void func_800C2A80(s32 mode, s32 flag, s32 arg2) {
    if (mode == 5) {
        mode = D_800D3F98;
    }
    if (flag == 2) {
        flag = D_800CCE4B;
    }
    D_800D3F98 = mode;
    D_800CCE4B = flag;
    D_800D3F90 = arg2;
    D_800D3F94 = 0;
    D_800D3FA0.unk5E0[3] = D_800D3FA0.unk5E0[1];
    D_800D3FA0.unk5E0[5] = D_800D3FA0.unk5E0[2];
    if (D_800CCE4B != 0) {
        switch (mode) {
        case 0:
            D_800D3FA0.unk5E0[4] = D_800D3FA0.unk0[3];
            D_800D3FA0.unk5E0[6] = D_800D3FA0.unk0[2];
            return;
        case 1:
            D_800D3FA0.unk5E0[4] = D_800D3FA0.unk0[7];
            D_800D3FA0.unk5E0[6] = D_800D3FA0.unk0[6];
            return;
        case 2:
            D_800D3FA0.unk5E0[4] = D_800D3FA0.unk0[11];
            D_800D3FA0.unk5E0[6] = D_800D3FA0.unk0[10];
            return;
        case 3:
            D_800D3FA0.unk5E0[4] = D_800D3FA0.unk0[15];
            D_800D3FA0.unk5E0[6] = D_800D3FA0.unk0[14];
            return;
        }
    } else {
        switch (mode) {
        case 0:
            D_800D3FA0.unk5E0[4] = D_800D3FA0.unk0[1];
            D_800D3FA0.unk5E0[6] = D_800D3FA0.unk0[0];
            return;
        case 1:
            D_800D3FA0.unk5E0[4] = D_800D3FA0.unk0[5];
            D_800D3FA0.unk5E0[6] = D_800D3FA0.unk0[4];
            return;
        case 2:
            D_800D3FA0.unk5E0[4] = D_800D3FA0.unk0[9];
            D_800D3FA0.unk5E0[6] = D_800D3FA0.unk0[8];
            return;
        case 3:
            D_800D3FA0.unk5E0[4] = D_800D3FA0.unk0[13];
            D_800D3FA0.unk5E0[6] = D_800D3FA0.unk0[12];
            return;
        }
    }
    func_80015C38(D_800A8178, mode);
}

/* blend the sky parameter blocks arg1 -> arg2 by arg3 (0..0x1000) into arg0 */
void func_800C3954(WmSky *arg0, WmSky *arg1, WmSky *arg2, s32 arg3, s32 arg4) {
    s16 *d;
    s16 *a;
    s16 *b;

    if (arg3 < 0) {
        arg3 = 0;
    }
    if (arg3 > 0x1000) {
        arg3 = 0x1000;
    }
    if (arg4 < 0) {
        arg4 = 0;
    }
    if (arg4 > 0x1000) {
        arg4 = 0x1000;
    }
    arg0->unk0[0].vx = func_800B822C(arg1->unk0[0].vx, arg2->unk0[0].vx, arg3, 0xD);
    arg0->unk0[0].vy = func_800B822C(arg1->unk0[0].vy, arg2->unk0[0].vy, arg3, 0xD);
    arg0->unk0[0].vz = func_800B822C(arg1->unk0[0].vz, arg2->unk0[0].vz, arg3, 0xD);
    d = &arg0->unk0[1].vx;
    a = &arg1->unk0[1].vx;
    b = &arg2->unk0[1].vx;
    d[0] = func_800B822C(a[0], b[0], arg3, 0xD);
    d[1] = func_800B822C(a[1], b[1], arg3, 0xD);
    d[2] = func_800B822C(a[2], b[2], arg3, 0xD);
    d = &arg0->unk0[2].vx;
    a = &arg1->unk0[2].vx;
    b = &arg2->unk0[2].vx;
    d[0] = func_800B822C(a[0], b[0], arg3, 0xD);
    d[1] = func_800B822C(a[1], b[1], arg3, 0xD);
    d[2] = func_800B822C(a[2], b[2], arg3, 0xD);
    d = &arg0->unk18.vx;
    a = &arg1->unk18.vx;
    b = &arg2->unk18.vx;
    d[0] = func_800B822C(a[0], b[0], arg3, 0xD);
    d[1] = func_800B822C(a[1], b[1], arg3, 0xD);
    d[2] = func_800B822C(a[2], b[2], arg3, 0xD);
    d = arg0->unk54;
    a = arg1->unk54;
    b = arg2->unk54;
    d[0] = func_800B822C(a[0], b[0], arg3, 0xD);
    d[1] = func_800B822C(a[1], b[1], arg3, 0xD);
    d[2] = func_800B822C(a[2], b[2], arg3, 0xD);
    arg0->unk5C = func_800B822C((s16)arg1->unk5C, (s16)arg2->unk5C, arg3, 0xD);
    d = &arg0->unk20.v.vx;
    a = &arg1->unk20.v.vx;
    b = &arg2->unk20.v.vx;
    d[0] = func_800B81C0(a[0] << 12, b[0] << 12, arg3) >> 12;
    d[1] = func_800B81C0(a[1] << 12, b[1] << 12, arg3) >> 12;
    d[2] = func_800B81C0(a[2] << 12, b[2] << 12, arg3) >> 12;
    d = &arg0->unk28.v.vx;
    a = &arg1->unk28.v.vx;
    b = &arg2->unk28.v.vx;
    d[0] = func_800B81C0(a[0] << 12, b[0] << 12, arg3) >> 12;
    d[1] = func_800B81C0(a[1] << 12, b[1] << 12, arg3) >> 12;
    d[2] = func_800B81C0(a[2] << 12, b[2] << 12, arg3) >> 12;
    d = arg0->unk30[0].unk4;
    a = arg1->unk30[0].unk4;
    b = arg2->unk30[0].unk4;
    d[0] = func_800B81C0(a[0], b[0], arg3);
    d[1] = func_800B81C0(a[1], b[1], arg3);
    d[2] = func_800B81C0(a[2], b[2], arg3);
    d = arg0->unk30[1].unk4;
    a = arg1->unk30[1].unk4;
    b = arg2->unk30[1].unk4;
    d[0] = func_800B81C0(a[0], b[0], arg3);
    d[1] = func_800B81C0(a[1], b[1], arg3);
    d[2] = func_800B81C0(a[2], b[2], arg3);
    d = arg0->unk30[2].unk4;
    a = arg1->unk30[2].unk4;
    b = arg2->unk30[2].unk4;
    d[0] = func_800B81C0(a[0], b[0], arg3);
    d[1] = func_800B81C0(a[1], b[1], arg3);
    d[2] = func_800B81C0(a[2], b[2], arg3);
    arg0->unk30[0].unk0 = func_800B81C0(arg1->unk30[0].unk0, arg2->unk30[0].unk0, arg3);
    arg0->unk30[0].unk2 = func_800B81C0((u16)arg1->unk30[0].unk2, (u16)arg2->unk30[0].unk2, arg3);
    arg0->unk30[1].unk0 = func_800B81C0(arg1->unk30[1].unk0, arg2->unk30[1].unk0, arg3);
    arg0->unk30[1].unk2 = func_800B81C0((u16)arg1->unk30[1].unk2, (u16)arg2->unk30[1].unk2, arg3);
    arg0->unk30[2].unk0 = func_800B81C0(arg1->unk30[2].unk0, arg2->unk30[2].unk0, arg3);
    arg0->unk30[2].unk2 = func_800B81C0((u16)arg1->unk30[2].unk2, (u16)arg2->unk30[2].unk2, arg3);
    arg0->unk30[0].unk0 &= 0xFFFE;
    arg0->unk30[0].unk2 = (u16)arg0->unk30[0].unk2 & 0xFFFE;
    arg0->unk30[1].unk0 &= 0xFFFE;
    arg0->unk30[1].unk2 = (u16)arg0->unk30[1].unk2 & 0xFFFE;
    arg0->unk30[2].unk0 &= 0xFFFE;
    arg0->unk30[2].unk2 = (u16)arg0->unk30[2].unk2 & 0xFFFE;
}

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

/* sound effect ids of world-map sound arg0 */
s32 func_800C3FE4(s32 arg0, Unk800C3FE4 *arg1) {
    Unk800C3FE4 s;

    s.unk8[0] = -1;
    switch (arg0) {
    case 0:
        s.unk8[0] = 0x68;
        break;
    case 1:
        s.unk8[0] = 0x66;
        break;
    case 2:
        s.unk8[0] = 0x6C;
        break;
    case 3:
        s.unk8[0] = 0x27D;
        break;
    case 4:
        s.unk8[0] = 0x27E;
        break;
    case 5:
        s.unk8[0] = 0x2AA;
        break;
    case 39:
        s.unk8[0] = 0x67;
        break;
    }
    if (s.unk8[0] != -1) {
        s.unk0 = 0x800000;
        s.unk4 = 1;
        *arg1 = s;
        return 1;
    } else {
        switch (arg0) {
        case 30:
            s.unk8[0] = 0x44B;
            break;
        case 31:
            s.unk8[0] = 0x4CF;
            break;
        case 32:
            s.unk8[0] = 0x596;
            break;
        case 34:
            s.unk8[0] = 0x44C;
            break;
        case 35:
            s.unk8[0] = 0x37C;
            break;
        case 36:
            s.unk8[0] = 0x44A;
            break;
        case 37:
            s.unk8[0] = 0x37A;
            break;
        case 38:
            s.unk8[0] = 0x5DB;
            break;
        case 40:
            s.unk8[0] = 0x37B;
            break;
        case 41:
            s.unk8[0] = 0xA47;
            break;
        case 42:
            s.unk8[0] = 0xC26;
            break;
        }
        if (s.unk8[0] != -1) {
            s.unk0 = 0;
            s.unk4 = 1;
            *arg1 = s;
            return 1;
        } else {
            switch (arg0) {
            case 6:
                s.unk8[0] = 0x552;
                break;
            case 7:
                s.unk8[0] = 0x553;
                break;
            }
            if (s.unk8[0] != -1) {
                s.unk0 = 0x8000;
                s.unk4 = 1;
                *arg1 = s;
                return 1;
            } else {
                /* the original third jump table is 8-aligned */
                __asm__(".rdata\n\t.word 0\n\t.text");
                switch (arg0) {
                case 33:
                    s.unk8[0] = 0x37A;
                    s.unk0 = 0;
                    s.unk8[1] = 0x37B;
                    break;
                case 20:
                    s.unk0 = 0x8000;
                    s.unk8[0] = 0x374;
                    s.unk8[1] = 0x375;
                    break;
                case 21:
                    s.unk0 = 0x8000;
                    s.unk8[0] = 0x376;
                    s.unk8[1] = 0x377;
                    break;
                case 22:
                    s.unk0 = 0x8000;
                    s.unk8[0] = 0x378;
                    s.unk8[1] = 0x379;
                    break;
                case 23:
                    s.unk0 = 0x8000;
                    s.unk8[0] = 0x554;
                    s.unk8[1] = 0x555;
                    break;
                case 24:
                    s.unk0 = 0x8000;
                    s.unk8[0] = 0x5D9;
                    s.unk8[1] = 0x5DA;
                    break;
                case 25:
                    s.unk0 = 0x800;
                    s.unk8[0] = 0x37D;
                    s.unk8[1] = 0x37E;
                    break;
                case 26:
                    s.unk0 = 0x800;
                    s.unk8[0] = 0x37F;
                    s.unk8[1] = 0x380;
                    break;
                }
                if (s.unk8[0] != -1) {
                    s.unk4 = 2;
                    *arg1 = s;
                    return 1;
                }
                func_80015C38("w_musicGetID : Illigal SE Number #%d\n", arg0);
                return 0;
                func_80015C38(""); /* unreachable; its empty string is in .rodata */
            }
        }
    }
}

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

void func_800C43F0(void) {
    Unk800C3FE4 s;
    s32 i;
    s32 j0;
    s32 m0;
    s32 j1;
    s32 m1;
    s32 j2;
    s32 m2;

    for (i = 0; i < 7; i++) {
        if (func_800C3FE4(i, &s) & 0xFF) {
            m0 = s.unk0;
            for (j0 = 0; j0 < s.unk4; j0++) {
                func_800AB0C0(0x5100, s.unk8[j0], m0, 0, 0);
                m0 >>= 1;
            }
        }
    }
    for (i = 20; i < 26; i++) {
        if (func_800C3FE4(i, &s) & 0xFF) {
            m1 = s.unk0;
            for (j1 = 0; j1 < s.unk4; j1++) {
                func_800AB0C0(0x5100, s.unk8[j1], m1, 0, 0);
                m1 >>= 1;
            }
        }
    }
    for (i = 30; i < 42; i++) {
        if (func_800C3FE4(i, &s) & 0xFF) {
            m2 = s.unk0;
            for (j2 = 0; j2 < s.unk4; j2++) {
                func_800AB0C0(0x5100, s.unk8[j2], m2, 0, 0);
                m2 >>= 1;
            }
        }
    }
    if ((u8)func_800C49EC() && D_800CF49C != 3) {
        D_8006794C->unk1C->unk0 |= 0x400000;
    }
}

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

void func_800C4A5C(void) {
    s32 i;

    for (i = 0; i < 10; i++) {
        D_800D4870[i].vx = i << 7;
        D_800D4870[i].vy = func_80062B3C() % 4096;
        D_800D4870[i].vz = 0;
        D_800D4830[i] = i * 0x14 + 0x200;
        D_800D48F8[i] = 0x800;
        D_800D48E8[i] = 0;
        D_800D4910[i].vx = func_80062B3C() % 4096;
        D_800D4910[i].vy = func_80062B3C() % 4096;
        D_800D4910[i].vz = 0;
    }
    for (i = 0; i < 9; i++) {
        D_800D4960[i].vx = 0;
        D_800D4960[i].vy = 0;
        D_800D4960[i].vz = 0;
        D_800D49F0[i].vx = 0;
        D_800D49F0[i].vy = 0;
        D_800D49F0[i].vz = 0;
    }
    setVector(&D_800D4960[0], 0x4464C, -0x1E68, -0x32681);
    setVector(&D_800D4960[1], 0x3805B, 0x1F4, -0x30847);
    setVector(&D_800D4960[6], 0x2FFFA, -0x2A82, -0x14052);
    D_800D4960[2] = D_800D4960[1];
    D_800D4960[3] = D_800D4960[1];
    D_800D4960[4] = D_800D4960[1];
    D_800D4960[5] = D_800D4960[1];
    D_800D4960[7] = D_800D4960[6];
    D_800D4960[8] = D_800D4960[6];
    D_800D4848.unk4 = NULL;
    D_800D4848.unk8 = &D_800D4A40[0];
    D_800D4A40[0].unk4 = &D_800D4848;
    D_800D4A40[0].unk8 = &D_800D4A40[1];
    for (i = 1; i < 59; i++) {
        D_800D4A40[i].unk4 = &D_800D4A40[i - 1];
        D_800D4A40[i].unk8 = &D_800D4A40[i + 1];
    }
    D_800D4A40[i].unk4 = &D_800D4A40[i - 1];
    D_800D4A40[i].unk8 = &D_800D53E8;
    D_800D53E8.unk4 = &D_800D4A40[i];
    D_800D53E8.unk8 = NULL;
    D_800D53A0.unk4 = NULL;
    D_800D53A0.unk8 = &D_800D48C0;
    D_800D48C0.unk4 = &D_800D53A0;
    D_800D48C0.unk8 = NULL;
    D_800CDF80.vy = 0;
    D_800CDF88.vy = 0;
    D_800CDF90.vy = 0;
    D_800CDFA0[0].vy = 0;
    D_800CDFA0[1].vy = 0;
    D_800CDFA0[2].vy = 0;
    D_800D4A38 = 0;
    D_800D53CC = 0;
}

INCLUDE_RODATA("asm/us/ovl_0c/nonmatchings/ovl_0c", D_800A849C);

void func_800C4DD8(void) {
    WmNode *o;
    VECTOR a;
    VECTOR b;
    SVECTOR pos;
    s32 tex;
    s32 k;

    o = D_800D53A0.unk8;
    SetRotMatrix(D_800CF448);
    SetTransMatrix(D_800CF448);
    while (o->unk8 != NULL) {
        if ((u8)func_800BBE24((VECTOR *)&o->unk10, &a, &b, 0)) {
            pos.vx = a.vx;
            pos.vy = a.vy;
            pos.vz = a.vz;
            if (b.vz > 1000) {
                tex = D_800CF460[*D_800CF45C];
                switch ((u8)o->unkD) {
                case 2:
                    D_800CF442 = 0;
                    D_800CF404 = func_800CAE0C(D_800CF404, (WmSprHdr *)o->unk0, &pos, NULL, o->unkE = o->unk1C * 400 + 6000,
                                               (s16)((s16)(o->unk1C / 4) % 26), (u8)o->unkC, -1, tex, 0, 2);
                    break;
                case 3:
                    D_800CF442 = 0;
                    D_800CF404 = func_800CAE0C(D_800CF404, (WmSprHdr *)o->unk0, &pos, NULL, o->unkE = o->unk1C * 400 + 6000,
                                               (s16)((s16)(o->unk1C / 2) % 23), (u8)o->unkC, -1, tex, 0, 2);
                    break;
                case 8: {
                    SVECTOR r = D_800A849C;
                    D_800CF442 = 20;
                    D_800CF404 = func_800CAE0C(D_800CF404, (WmSprHdr *)o->unk0, &pos, &r, o->unkE, o->unk1C, (u8)o->unkC,
                                               -1, tex, 0, 0);
                    break;
                }
                case 15:
                case 22: {
                    SVECTOR r = D_800A849C;
                    D_800CF442 = 20;
                    D_800CF404 = func_800CAE0C(D_800CF404, (WmSprHdr *)o->unk0, &pos, &r, o->unkE, o->unk1C, (u8)o->unkC,
                                               D_800D4A38, tex, 0, 0);
                    break;
                }
                case 16: {
                    SVECTOR r = D_800A849C;
                    pos.vy -= 400;
                    o->unkE = (8 - o->unk1C) * 4000;
                    r.vy = o->unk1C * 128;
                    D_800CF442 = 0;
                    D_800CF404 = func_800CAE0C(D_800CF404, (WmSprHdr *)o->unk0, &pos, &r, o->unkE, 0, 1, -1, tex, 0, 0);
                    r.vy = o->unk1C * 200;
                    D_800CF404 = func_800CAE0C(D_800CF404, (WmSprHdr *)o->unk0, &pos, &r, o->unkE, 0, 2, -1, tex, 0, 0);
                    break;
                }
                case 14:
                case 18:
                case 19:
                    if (D_800D53E0 >= 0x1000) {
                        D_800D53E0 = 0xFFF;
                    }
                    if (D_800D53E0 < 0) {
                        D_800D53E0 = 0;
                    }
                    D_800D53CC += (D_800D53E0 - D_800D53CC) / 32;
                    D_800CF442 = 50;
                    k = D_800D4A38 * D_800D53CC >> 12;
                    if (k >= 25) {
                        D_800CF404 = func_800CAE0C(D_800CF404, (WmSprHdr *)o->unk0, &pos, NULL, o->unkE, o->unk1C,
                                                   (u8)o->unkC, k, tex, 0, 0);
                    }
                    break;
                case 25:
                case 26: {
                    SVECTOR r;
                    func_8001D898(&r, 0, 8);
                    r.pad = 1;
                    r.vy = ratan2(0x30031 - o->unk10, -0x13F6E - o->unk18);
                    D_800CF442 = 0;
                    D_800CF404 = func_800CAE0C(D_800CF404, (WmSprHdr *)o->unk0, &pos, &r, o->unkE, o->unk1C, (u8)o->unkC,
                                               -1, tex, 0, 0);
                    break;
                }
                default:
                    D_800CF442 = 40;
                    D_800CF404 = func_800CAE0C(D_800CF404, (WmSprHdr *)o->unk0, &pos, NULL, o->unkE, o->unk1C, (u8)o->unkC,
                                               -1, tex, 0, 0);
                    break;
                }
            }
        }
        o = o->unk8;
    }
}

/* jump table padding emitted by the original toolchain */
__asm__(".section .rodata\n\t.word 0\n\t.section .text");

INCLUDE_ASM("asm/us/ovl_0c/nonmatchings/ovl_0c", func_800C53DC);

INCLUDE_ASM("asm/us/ovl_0c/nonmatchings/ovl_0c", func_800C5A48);

INCLUDE_RODATA("asm/us/ovl_0c/nonmatchings/ovl_0c", D_800A8588);

INCLUDE_RODATA("asm/us/ovl_0c/nonmatchings/ovl_0c", D_800A8598);

void func_800C69F4(s32 arg0) {
    WmEmitter *e;
    u8 on;
    s32 i;
    s32 r;
    s32 x;
    s32 y;
    s32 z;
    s32 b;
    u16 t;

    on = 0;
    switch ((u8)arg0) {
    case 2:
        D_800D1FD0 = 1;
        on = 1;
        break;
    case 5:
        on = 1;
        break;
    case 10:
        if (D_800D30D2 != 0) {
            on = 1;
        }
        break;
    case 3:
        if ((u16)(*D_800CF430 - 0x24EA) < 0x1B9) {
            on = 1;
        }
        break;
    case 7:
        if (D_800D5410 != 0) {
            D_800CDF48.vy += 0x40;
        }
        break;
    case 4:
        on = 1;
        D_800CDF80.vy -= 0x100;
        D_800CDFA0[0].vy += 0x100;
        D_800CDFA0[1].vy -= 0x40;
        D_800CDFA0[2].vy -= 0x180;
        break;
    case 6:
        D_800CDF88.vz -= 8;
        D_800CDF88.vy -= 0x10;
        D_800CDF90.vz -= 0x10;
        D_800CDF90.vy -= 8;
        for (i = 0; i < 10; i++) {
            t = D_800D4870[i].vy - 10;
            D_800D4870[i].vy = t - i / 4;
        }
        for (i = 0; i < 10; i++) {
            D_800D4910[i].vx += -6 + func_80062B3C() % 13;
            D_800D4910[i].vy += -6 + func_80062B3C() % 13;
        }
        break;
    }
    if (on) {
        e = func_800B8388(D_800CF428, (u8)arg0 + 0x28);
        for (; e->unk24 != -1; e++) {
            if (func_80062B3C() % 4096 < e->unk28) {
                x = e->unk0 - e->unkC / 2;
                y = e->unk4 - e->unk10 / 2;
                z = e->unk8 - e->unk14 / 2;
                if (e->unkC != 0) {
                    if (D_8007B720 >= D_8007B92D) {
                        func_8004BF20();
                        if (D_8007B720 >= D_8007B92D) {
                            func_8004BF20();
                        }
                    }
                    b = *D_8007B720++;
                    x += ((b << 8) | b) % e->unkC;
                }
                if (e->unk10 != 0) {
                    if (D_8007B720 >= D_8007B92D) {
                        func_8004BF20();
                        if (D_8007B720 >= D_8007B92D) {
                            func_8004BF20();
                        }
                    }
                    b = *D_8007B720++;
                    y += ((b << 8) | b) % e->unk10;
                }
                if (e->unk14 != 0) {
                    if (D_8007B720 >= D_8007B92D) {
                        func_8004BF20();
                        if (D_8007B720 >= D_8007B92D) {
                            func_8004BF20();
                        }
                    }
                    b = *D_8007B720++;
                    z += ((b << 8) | b) % e->unk14;
                }
                func_800C6F00(x, y, z, e->unk24, e->unk26);
            }
        }
    }
}

/* jump table padding emitted by the original toolchain */
__asm__(".section .rodata\n\t.word 0\n\t.section .text");

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

void func_800C7914(Unk800C95C8 *arg0, u8 arg1) {
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
void func_800C7BA8(Unk800C95C8 *arg0, u8 *arg1) {
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

void func_800C7CF4(Unk800C95C8 *arg0, WmTexInfo *arg1, s16 (*arg2)[2]) {
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

void func_800C8950(Unk800C95C8 *arg0, u8 arg1) {
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
        otz = (D_80070520[i0] + D_80070520[i1] + D_80070520[i2] + D_80070520[i3]) >> 5;
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
        otz >>= 1; /* shifts the previous value: overwritten below */
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
        otz >>= 5;
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
        otz >>= 1;
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
        otz = (D_80070520[i0] + D_80070520[i1] + D_80070520[i2] + D_80070520[i3]) >> 5;
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
        otz >>= 1;
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
        otz >>= 5;
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
        otz >>= 1;
        addPrim(&ot[otz], pF3);
    }
}

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

void func_800C96AC(Unk800C95C8 *arg0, u16 arg1) {
    SVECTOR sv;
    u16 nb;
    VECTOR *vec;
    WmAnimHdr *anim;
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

    anim = arg0->unkC;
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
    key = (WmAnimKey *)anim->unkC;
    key2 = (WmAnimKey *)anim->unk10;
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

void func_800C9FCC(Unk800C95C8 *arg0, u8 arg1) {
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
    ymax = (s16)arg0->unk5A;
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

INCLUDE_ASM("asm/us/ovl_0c/nonmatchings/ovl_0c", func_800CA1D8);

s32 func_800CAE00(u16 *arg0) {
    return *arg0 & 0x7FFF;
}

s32 func_800CAE0C(s32 arg0, WmSprHdr *h, SVECTOR *pos, SVECTOR *rot, s32 scale, s32 frame, s32 abr, s32 arg7,
                   s32 arg8, s32 arg9, s32 argA) {
    VECTOR s;
    SVECTOR zero;
    MATRIX m;
    WmSprCtx c;
    u16 *p;
    u8 *b;
    s32 n;
    s32 f;
    u16 *o;

    if (rot == NULL) {
        rot = &zero;
        ((s32 *)&zero)[0] = 0;
        ((s32 *)&zero)[1] = 0;
    }
    if (arg9 != 0) {
        h->unk4 += arg9;
    }
    n = h->unk0 & 0x7FFF;
    f = frame;
    if (f >= n) {
        f = frame % n;
    }
    o = &h->unk8[f];
    n = n * 2 + 8;
    b = (u8 *)h + n;
    p = (u16 *)b;
    f = *p++;
    c.unk0 = p;
    c.unk4 = p + f + 1;
    c.unk10 = h->unk2 | ((abr & 3) << 5);
    c.unk14 = h->unk4;
    c.unkC = (h->unk6 - 1) * 2;
    c.unk8 = (h->unk7 - 1) * 2;
    n = *o;
    c.unk1C = ((u8 *)h)[n++];
    h = (WmSprHdr *)((u8 *)h + n);
    gte_SetRotMatrix(&D_8006794C->unk1C->unk18);
    gte_SetTransMatrix(&D_8006794C->unk1C->unk18);
    gte_ldv0(pos);
    gte_rtv0tr();
    func_800316D8(rot, &m);
    if (scale >= 0) {
        s.vx = s.vy = s.vz = scale;
        ScaleMatrix(&m, &s);
    }
    gte_stlvnl(m.t);
    if (rot->pad != 0) {
        MulRotMatrix(&m);
    }
    gte_SetRotMatrix(&m);
    gte_SetTransMatrix(&m);
    c.unk20 = (abr == 0xFF) ? 0x2C : 0x2E;
    c.unk28 = -D_800CF442;
    c.unk18 = arg7;
    c.unk24 = argA;
    return func_800CB1EC((u8 *)h, &c, arg0, arg8);
}

/* upload every image and clut of an image pack (c walks the clut RECTs in the second loop) */
void func_800CB09C(Unk800CB09C *arg0) {
    RECT r;
    Unk800CB09CClut *c;
    s16 *img;
    s32 i;
    s32 n;
    s32 n2;
    s32 ofs;
    s32 size;
    u8 *pix;

    ofs = arg0->unk0;
    c = (Unk800CB09CClut *)((u8 *)arg0 + ofs);
    ofs = arg0->unk4;
    i = 0;
    img = (s16 *)((u8 *)arg0 + ofs);
    ofs = c->unk0;
    pix = (u8 *)arg0 + ofs;
    n = arg0->unk8;
    for (; i < n; i++) {
        r.x = img[0];
        r.y = img[1];
        r.w = img[2];
        r.h = img[3];
        size = r.w * r.h * 2;
        img += 4;
        LoadImage(&r, (u32 *)img);
        img = (s16 *)((u8 *)img + size);
    }
    n2 = c->unk4;
    c = (Unk800CB09CClut *)c->unk8;
    for (i = 0; i < n2; i++) {
        r.x = ((RECT *)c)->x;
        r.y = ((RECT *)c)->y;
        r.w = ((RECT *)c)->w;
        r.h = ((RECT *)c)->h;
        LoadImage(&r, (u32 *)pix);
        c = (Unk800CB09CClut *)((RECT *)c + 1);
        pix += r.w * r.h * 2;
    }
}

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
            c->unk34.b.unk9 ^= 1;
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
