#include "ovl_0c.h"

void func_800A8860(void) {
    func_800A8B14();
    do {
        func_800A88C4(g_gameState->unk1C->unk7DC);
    } while (!(g_gameState->unk0 & 0x1001));
    func_800A8CA4();
}

void func_800A88C4(s32 arg0) {
    func_800A924C(arg0);
    SetDispMask(1);
    do {
        func_800AC580();
        pad_update();
        if ((g_padPortPressed & 0x04000000) && (g_padPortHeld[0] & 0xF8) && !(g_padPortHeld[0] & 0xFA0001)) {
            func_800ACEEC();
        }
        if (!(g_gameState->unk1C->unk0 & 0x100)) {
            if (!(g_gameState->unk1C->unk8E4.world->unk0 & 0x100)) {
                switch (func_800B6BC4()) {
                case 3:
                    g_gameState->unk1C->unk8E4.world->unk8->unk1B = 2;
                    func_800AC7D4();
                    break;
                case 4:
                    g_gameState->unk1C->unk8E4.world->unk8->unk1B = 1;
                    g_gameState->unk0 |= 0x1000;
                    break;
                case 5:
                    g_gameState->unk0 |= 0x2000;
                    break;
                }
            }
            if (!(g_gameState->unk1C->unk8E4.world->unk0 & 0x200)) {
                func_80032098();
            }
            if (!(g_gameState->unk1C->unk8E4.world->unk0 & 0x400)) {
                func_800ACAFC();
            }
            if (!(g_gameState->unk1C->unk8E4.world->unk0 & 0x800)) {
                func_80023A4C();
            }
        }
        func_800AC5C8();
    } while (!(g_gameState->unk0 & 0x3001));
    func_800A95A0();
}

void func_800A8B14(void) {
    WorldMap *wm;
    GameData *gd;

    func_800A8CF0();
    wm = g_gameState->unk1C->unk8E4.world;
    func_800A8D28();
    g_gameState->vsyncMode = 3;
    func_8001CD04(0xDA);
    func_8001CFB4();
    SetDispMask(1);
    g_gameState->unk1C->unk838 = (s32 (*)(s32, void *))func_800AA15C;
    g_gameState->unk1C->unk83C = func_800AB0C0;
    g_gameState->unk1C->unk8BC = func_800AD10C;
    g_gameState->unk1C->unk84C = 0;
    gd = g_gameState->unk1C;
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
        g_gameState->unk1C->unk0 |= 0x400000;
    }
    if (g_gameState->unk1C->unk0 & 0x400000) {
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
    w_frameSystemDestructor();
    func_800A91B4();
}

void func_800A8CF0(void) {
    D_80067948 = (s32)(D_80067944 + 0x32000);
    g_gameState->unk1C->unk8E4.world = (WorldMap *)D_80067948;
    D_80067948 += 0x3000;
}

void func_800A8D28(void) {
    u32 i;
    WorldMap *wm = g_gameState->unk1C->unk8E4.world;

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
        addr = db_lookupRegistered(4, id, 0);
        size = (u32)db_lookupRegistered(5, id, 0);
        if (ids[i] == 0x17B) {
            r = &D_800CE0B8[D_800CCA1C];
            r->x = g_gameState->unk10[g_gameState->unk1C->unk8].clip.x;
            r->y = g_gameState->unk10[g_gameState->unk1C->unk8].clip.y;
            r->h = 218;
            r->w = size / 2 / r->h + 1;
            LoadImage(r, addr);
            DrawSync(0);
            g_gameState->unk1C->unk8E4.world->unk10 = size;
        } else {
            func_80055DF4();
            func_80055E14((s32)addr, size, 1);
        }
    }
    return 0;
}

s32 func_800A8F18(void) {
    s32 addr = (s32)db_lookupRegistered(4, 0x70064, 0);

    func_800AD114(g_gameState->unk1C->unk818, addr, (s32)db_lookupRegistered(5, 0x70064, 0));
    DrawSync(0);
    return 0;
}

s32 func_800A8F8C(void) {
    s32 addr = (s32)db_lookupRegistered(4, 0x42774, 0);

    func_8001D9CC(addr, 0x280, 0xDE, 0x330, 0xB0);
    func_8001D8A8(addr);
    DrawSync(0);
    return 0;
}

s32 func_800A8FF0(void) {
    s32 id;
    FileEntry *f;
    u8 *buf;
    PreloadEntry *e;

    id = func_800A9228();
    f = file_find(1, id);
    buf = D_80067944 + 0x34000;
    if (func_800220E8(id) != 0) {
        e = preload_find(id);
        func_8001DAA4((s32)buf, e->unk8, e->unkC, 0);
        DrawSync(0);
        if (e->unkC < e->unk1C) {
            func_8002177C(g_gameState->unk1C->unk854, f->unk4, (f->unkC - f->unk4) << 11, buf + e->unkC, e->unkC, -1, 0);
            while (func_800217C4(g_gameState->unk1C->unk854) != 0) {
            }
        }
        func_80022390(id);
    } else {
        func_8002177C(g_gameState->unk1C->unk854, f->unk4, (f->unkC - f->unk4) << 11, buf, 0, -1, 0);
        while (func_800217C4(g_gameState->unk1C->unk854) != 0) {
        }
    }
    g_gameState->unk1C->unk8E4.world->unkC = buf;
    db_register((s32)g_gameState->unk1C->unk8E4.world->unkC, 0);
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
    void *p = g_gameState->unk1C->unk8E4.world->unkC;

    if (p != NULL) {
        db_unregister(p);
        g_gameState->unk1C->unk8E4.world->unkC = 0;
    }
    return 0;
}

s32 func_800A9220(void) {
    return 0;
}

s32 func_800A9228(void) {
    s32 v = 0x2712;
    if (g_evtGlobalVars.h[0] >= 0x2B52) {
        v = 0x2716;
    }
    return v;
}

void func_800A924C(s32 arg0) {
    WorldMapObj *obj;
    s32 i;

    D_80067948 = (s32)g_gameState->unk1C->unk8E4.world->unk4;
    func_800A98B8();
    obj = g_gameState->unk1C->unk8E4.world->unk8;
    func_800A98F8();
    g_gameState->unk0 &= ~0x3001;
    g_gameState->unk1C->unk8E4.world->unk0 |= 0x400;
    g_gameState->unk1C->unk7DC = arg0;
    obj->unk24 = arg0;
    func_800A9A44();
    wm_loadFile(obj, arg0);
    func_800A9A04(obj, arg0);
    func_800A99C4(obj, (s16)g_gameState->unk1C->unk7DE);
    func_800A9B54();
    func_800AB0C0(0x900, 0, 0, 0, 0);
    func_80049ED8(obj->unk10);
    evt_init(obj->unk10);
    while (func_800AB0C0(0xC00, 0, 0, 0, 0) != 0) {
    }
    func_8004BFB0();
    g_gameState->unk1C->unk38 = func_8004AAF0(0);
    g_gameState->unk1C->unk8DC = (Unk8D4 *)(obj + 1);
    g_gameState->unk1C->unk8E0 = (Unk8E0 *)&g_gameState->unk1C->unk8DC[g_gameState->unk1C->unk38];
    D_80067948 = (s32)&g_gameState->unk1C->unk8E0[g_gameState->unk1C->unk38];
    g_gameState->unk1C->unk9 = 0;
    for (i = 0; i < 9; i++) {
        func_8001E3A4(&g_gameState->unk1C->unk8D8[i]);
        g_gameState->unk1C->unk8D8[i].unk8 = &g_gameState->unk1C->unk8D4[i];
    }
    g_gameState->unk1C->unk39 = 0;
    for (i = 0; i < g_gameState->unk1C->unk38; i++) {
        func_8001E3A4(&g_gameState->unk1C->unk8E0[i]);
        g_gameState->unk1C->unk8E0[i].unk8 = &g_gameState->unk1C->unk8DC[i];
    }
    obj->unk1C = (void *)D_80067948;
    D_80067948 = func_800AAD0C(D_80067948, g_gameState->unk1C->unk38 + 9);
    obj->unk4 = (void *)D_80067948;
    obj->unk20 = (s32)obj->unk30;
    func_8004A8A4();
    func_800B7F7C();
    preload_init();
    g_gameState->unk1C->unk8E4.world->unk0 &= ~1;
}

void func_800A95A0(void) {
    WorldMapObj *obj;
    GameData *gd;
    s32 i;
    s32 mode;

    obj = g_gameState->unk1C->unk8E4.world->unk8;
    DrawSync(0);
    while (func_800AB0C0(0xC00, 0, 0, 0, 0) != 0) {
    }
    func_8005546C(-1, 0);
    func_800A9DE4(obj, g_gameState->unk1C->unk7DC);
    for (i = 0; i < 9; i++) {
        if (g_gameState->unk1C->unk8D8[i].unk0 & 1) {
            g_gameState->unk1C->unk8D8[i].unk0 &= ~1;
        }
    }
    for (i = 0; i < g_gameState->unk1C->unk38; i++) {
        if (g_gameState->unk1C->unk8E0[i].unk0 & 1) {
            g_gameState->unk1C->unk8E0[i].unk0 &= ~1;
        }
    }
    for (i = 1; i >= 0; i--) {
        g_gameState->unk1C->returnSector[i] = g_gameState->unk1C->sceneSector[i];
        g_gameState->unk1C->returnSectors[i] = g_gameState->unk1C->sceneSectors[i];
    }
    w_frameMapDestructor();
    gd = g_gameState->unk1C;
    mode = gd->unk8E4.world->unk8->unk1B;
    switch (mode) {
    case 2:
        wm_depFindBackground(wm_depFindBattle(obj->unk24, &gd->sceneSector[0], &gd->sceneSectors[0]), &g_gameState->unk1C->sceneSector[1],
                      &g_gameState->unk1C->sceneSectors[1]);
        g_gameState->unk1C->battleId = obj->unk24;
        g_gameState->mode = mode;
        g_gameState->unkA = 3;
        break;
    case 3:
        i = wm_depFindArea(wm_depFindField(obj->unk24, &gd->sceneSector[1], &gd->sceneSectors[1]), &g_gameState->unk1C->sceneSector[0],
                          &g_gameState->unk1C->sceneSectors[0]);
        g_gameState->unk1C->unk7DC = obj->unk24;
        g_gameState->unk1C->unk7DE = i;
        break;
    case 1:
        g_gameState->unk1C->unk3A = wm_depFindArea(wm_depFindField(obj->unk24, &gd->sceneSector[1], &gd->sceneSectors[1]),
                                                 &g_gameState->unk1C->sceneSector[0], &g_gameState->unk1C->sceneSectors[0]);
        g_gameState->unk1C->fieldId = obj->unk24;
        g_gameState->mode = mode;
        g_gameState->unkA = 3;
        break;
    default:
        SystemError(0x39, 0x194);
        break;
    }
    g_gameState->unk1C->depTable = (struct FileTable *)-1;
}

void func_800A98B8(void) {
    WorldMap *wm = g_gameState->unk1C->unk8E4.world;

    wm->unk8 = &wm->unk48;
    wm->unk8->unk2C = D_80067944 + 0x5EB00;
    D_80067948 = (s32)(wm + 1);
}

/* reset the world map object */
void func_800A98F8(void) {
    WorldMapObj *o = g_gameState->unk1C->unk8E4.world->unk8;
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
    arg0->unk14 = db_lookupRegistered(4, arg1 | 0x60000, 0);
    return 0;
}

s32 func_800A9A04(WorldMapObj *arg0, u16 arg1) {
    arg0->unk10 = db_lookupRegistered(4, arg1 | 0x50000, 0);
    return 0;
}

void func_800A9A44(void) {
    GameData *gd = g_gameState->unk1C;
    RECT *r;
    u16 h;
    u32 *dst;

    if (gd->unk8E4.world->unk10 != -1) {
        r = &D_800CE0B8[D_800CCA1C];
        h = 0xDA;
        r->x = g_gameState->unk10[gd->unk8].clip.x;
        r->y = g_gameState->unk10[g_gameState->unk1C->unk8].clip.y;
        dst = (u32 *)(D_80067944 + 0x91800);
        r->h = h;
        r->w = ((u32)g_gameState->unk1C->unk8E4.world->unk10 >> 1) / h + 1;
        StoreImage(r, dst);
        DrawSync(0);
    }
}

void func_800A9B54(void) {
    u8 *dst;

    while (func_800AB0C0(0xC00, 0, 0, 0, 0) != 0) {
    }
    if (g_gameState->unk1C->unk8E4.world->unk10 != -1) {
        dst = D_80067944 + 0x91800;
        func_80055DF4();
        func_80055E14((s32)dst, g_gameState->unk1C->unk8E4.world->unk10, 1);
        g_gameState->unk1C->unk8E4.world->unk10 = -1;
    }
}

s32 wm_loadFile(WorldMapObj *arg0) {
    GameData *gd;
    u8 *buf;
    s32 a;
    s32 b;
    s32 i;
    s32 p;
    u8 *q;
    s32 c;

    gd = g_gameState->unk1C;
    func_8002177C(gd->unk854, gd->sceneSector[1], gd->sceneSectors[1] << 11, arg0->unk30, 0, -1, 0);
    while (func_800217C4(g_gameState->unk1C->unk854) != 0) {
    }
    buf = arg0->unk30;
    a = db_lookup((s32)buf, 4, 0x1B0000, 0);
    b = db_lookup((s32)buf, 4, 0x1B0001, 0);
    g_gameState->unk1C->depTable = (struct FileTable *)db_lookup((s32)buf, 4, 0x140000, 0);
    for (i = db_lookup(a, 1, 0x40000, 0) - 1; i >= 0; i--) {
        p = db_lookup(a, 2, (i & 0xFFFF) | 0x40000, 0);
        if (p != -1) {
            func_8001D8A8(p);
        }
    }
    DrawSync(0);
    func_800AD114(a, b, func_8001CA70(b));
    DrawSync(0);
    b = a;
    arg0->unk34[1] = (void *)b;
    db_register(b, 0);
    q = (u8 *)db_lookup(b, 4, 0x1D0000, 0);
    arg0->unk1A = *q;
    q += 4;
    arg0->unk28 = (WmPalAnim *)q;
    arg0->unk30 = (u8 *)arg0->unk34[1] + func_8001CA70((s32)arg0->unk34[1]);
    return 0;
}

s32 func_800A9DE4(WorldMapObj *arg0, s16 arg1) {
    if (arg0->unk34[1] != NULL) {
        db_unregister(arg0->unk34[1]);
        arg0->unk34[1] = NULL;
    }
    return 0;
}

s32 wm_depFindBattle(s16 arg0, s32 *arg1, s32 *arg2) {
    s32 i;
    FileTable *t = g_gameState->unk1C->depTable;
    s32 id = (u16)arg0 | 0x110000;
    FileEntry *e = t->unk4;

    for (i = 0; i < g_gameState->unk1C->depTable->unk2; i++) {
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

s32 wm_depFindBackground(s32 arg0, s32 *arg1, s32 *arg2) {
    s32 id = -1;
    FileTable *t = g_gameState->unk1C->depTable;
    s32 i;
    FileEntry *e = t->unk4;

    for (i = 0; i < t->unk2; i++) {
        if (e[i].unk0 == arg0) {
            id = e[i].unkC;
            break;
        }
    }
    for (i = 0; i < g_gameState->unk1C->depTable->unk2; i++) {
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

s32 wm_depFindField(s16 arg0, s32 *arg1, s32 *arg2) {
    s32 i;
    FileTable *t = g_gameState->unk1C->depTable;
    s32 id = (u16)arg0 | 0x50000;
    FileEntry *e = t->unk4;

    for (i = 0; i < g_gameState->unk1C->depTable->unk2; i++) {
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

s32 wm_depFindArea(s32 arg0, s32 *arg1, s32 *arg2) {
    s32 id = -1;
    FileTable *t = g_gameState->unk1C->depTable;
    s32 i;
    FileEntry *e = t->unk4;

    for (i = 0; i < t->unk2; i++) {
        if (e[i].unk0 == arg0) {
            id = e[i].unkC;
            break;
        }
    }
    for (i = 0; i < g_gameState->unk1C->depTable->unk2; i++) {
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
        id = g_gameState->unk1C->unk39 + 9;
        o = (WmObjSlot *)func_8001E350(id);
        g_gameState->unk1C->unk39++;
    }
    o->unk4 = arg0;
    o->unk8->unk8 = db_lookupRegistered(4, arg0, 0);
    if ((o->unk8->unkC = db_lookupRegistered(4, func_800AA39C(o->unk4), 0)) != NULL) {
        h = o->unk8->unkC;
        if (IS_OFFSET(h->unkC)) {
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
    if (IS_OFFSET(g)) {
        g = (WmModelGroup *)((s32)g + (s32)m);
    }
    for (i = 0; i < m->unk3; i++) {
        n += g[i].unk0;
    }
    n *= 2;
    n += (o->unk8->unk8->unk2 << 5) + o->unk8->unk8->unk3 * 12;
    func_800C7BA8(o->unk8, g_gameState->unk1C->unk8E4.world->unk8->unk20);
    func_800C7CF4(o->unk8, func_800634A8(o->unk4), func_800634D8(o->unk4));
    g_gameState->unk1C->unk8E4.world->unk8->unk20 += n;
    o->unk0 |= 1;
    return id;
}

s32 func_800AA39C(s32 arg0) {
    Unk800AA39C *t = db_lookupRegistered(8, arg0, 0x120000);
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
        p[i] = e->unk8[g_gameState->unk1C->unk8][i];
        setRGB0(p[i], e->unk0, e->unk0, e->unk0);
    }
    gte_MulMatrix0(&g_gameState->unk1C->unk18, &m, &m);
    gte_SetTransMatrix(&g_gameState->unk1C->unk18);
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

s32 func_800AAD0C(s32 arg0, s32 arg1) {
    return func_80062E3C(arg0, 0x280, 0xDE, 0x330, 0xB0, arg1, 1);
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

INCLUDE_RODATA("asm/us/ovl_0c/nonmatchings/wm_main", D_800A7000);

/* world map music/sound request handler (GameData.unk83C); unk851 drives the streamed music load */
s32 func_800AB0C0(s32 arg0, s32 arg1, s32 arg2, s32 arg3, s32 arg4) {
    s32 kind;
    s32 id;
    s32 h;
    s32 t;
    s32 n;
    s32 sz;
    s32 m;
    s32 d;

    kind = (arg0 >> 8) & 0xF;
    if (kind == 12 && g_gameState->unk1C->unk851 != 0) {
        goto state;
    }
    switch ((arg0 >> 12) & 3) {
    case 0:
        id = (arg1 & 0xFFFF) | 0x70000;
        switch (kind) {
        case 9:
            if (g_gameState->unk1C->unk7E8 == 0xFFFF) {
                return 0;
            }
            if (g_gameState->unk1C->unk7EA != g_gameState->unk1C->unk7DC) {
                g_gameState->unk1C->unk7E8 = 0xFFFF;
                g_gameState->unk1C->unk7EA = 0xFFFF;
                return 0;
            }
            D_800CCA14 = kind;
            arg1 = g_gameState->unk1C->unk7E8;
            g_gameState->unk1C->unk826[0] = g_gameState->unk1C->unk850;
            id = arg1 | 0x70000;
            g_gameState->unk1C->unk7E8 = 0xFFFF;
        case 0:
        case 7:
            if (g_gameState->unk1C->unk851 != 0) {
                return -1;
            }
            if (D_800CCA14 == -1) {
                D_800CCA14 = kind;
            }
            if (arg1 == 100) {
                if (g_gameState->unk1C->unk0 & 0x20000000) {
                    func_800552B8(g_gameState->unk1C->unk824[0]);
                    g_gameState->unk1C->unk0 &= ~0x20000000;
                }
                g_gameState->unk1C->unk81C[1] = arg1;
                g_gameState->unk1C->unk820[1] = func_800AC344(id);
                g_gameState->unk1C->unk826[1] = 0x7F;
                g_gameState->unk1C->unk824[1] = func_80055290((void *)g_gameState->unk1C->unk818);
                if (!(g_gameState->unk1C->unk0 & 0x80000000)) {
                    func_80055A34(g_gameState->unk1C->unk824[1], 0x1E, 0, g_gameState->unk1C->unk826[1]);
                }
                g_gameState->unk1C->unk0 |= 0xC0000000;
                goto done;
            }
            t = func_800AC344(id);
            if (g_gameState->unk1C->unk81C[0] != arg1 || g_gameState->unk1C->unk820[0] != (u16)t || (g_gameState->unk1C->unk0 & 0x80000000)) {
                if (g_gameState->unk1C->unk0 & 0x20000000) {
                    g_gameState->unk1C->unk0 |= 0x200000;
                }
                if (g_gameState->unk1C->unk0 & 0x80000000) {
                    g_gameState->unk1C->unk0 |= 0x200000;
                }
                if (g_gameState->unk1C->unk81C[0] != arg1 || (g_gameState->unk1C->unk0 & 0x20000000)) {
                    g_gameState->unk1C->unk0 &= ~0x10000000;
                }
                g_gameState->unk1C->unk0 &= 0x5FFFFFFF;
                g_gameState->unk1C->unk820[0] = t;
                g_gameState->unk1C->unk81C[0] = arg1;
                if (!(g_gameState->unk1C->unk0 & 0x10000000)) {
                    D_800CCA10 = id;
                    g_gameState->unk1C->unk851 = 1;
                    return g_gameState->unk1C->unk851;
                }
            }
            if (D_800CCA14 == 7) {
                goto done;
            }
        start:
            g_gameState->unk1C->unk824[0] = func_80055290(g_gameState->unk1C->unk814);
            if (!(g_gameState->unk1C->unk0 & 0x20000000)) {
                func_80055A34(g_gameState->unk1C->unk824[0], 0x1E, 0, g_gameState->unk1C->unk826[0]);
            }
            g_gameState->unk1C->unk0 |= 0x30000000;
        done:
            D_800CCA14 = -1;
            return 0;
        case 12:
            if (g_gameState->unk1C->unk851 != 0) {
                goto state;
            }
            return func_800AC25C() ? 6 : 0;
        case 8:
            func_80055310();
            g_gameState->unk1C->unk7E8 = g_gameState->unk1C->unk81C[0];
            g_gameState->unk1C->unk850 = g_gameState->unk1C->unk826[0];
            g_gameState->unk1C->unk7EA = g_gameState->unk1C->unk7DC;
        case 1:
            if (g_gameState->unk1C->unk81C[0] == arg1) {
                func_800552B8(g_gameState->unk1C->unk824[0]);
                g_gameState->unk1C->unk826[0] = 0x7F;
                g_gameState->unk1C->unk0 &= 0xDFFFFFFF;
            } else if (g_gameState->unk1C->unk81C[1] == arg1) {
                func_800552B8(g_gameState->unk1C->unk824[1]);
                g_gameState->unk1C->unk826[0] = 0x7F;
                g_gameState->unk1C->unk0 &= 0x7FFFFFFF;
            }
            return 0;
        case 2:
            if ((arg0 & 0x3F) != 1) {
                break;
            }
            func_800559C8(g_gameState->unk1C->unk824[0], arg2);
            g_gameState->unk1C->unk826[0] = arg2;
            return 0;
        case 3:
            if ((arg0 & 0x3F) != 1) {
                break;
            }
            func_800559FC(g_gameState->unk1C->unk824[0], arg2, arg3);
            func_800AC21C(arg3, arg2);
            return 0;
        case 4:
            if ((arg0 & 0x3F) != 1) {
                break;
            }
            func_80055A34(g_gameState->unk1C->unk824[0], arg2, arg3, arg4);
            func_800AC21C(arg4, arg2);
            return 0;
        case 6:
            if ((arg0 & 0x3F) != 1) {
                break;
            }
            func_800559FC(0, arg2, arg3);
            func_800AC21C(arg3, arg2);
            return 0;
        case 11:
            if ((arg0 & 0x3F) != 1) {
                break;
            }
            func_80055A34(0, arg2, arg3, arg4);
            func_800AC21C(arg4, arg2);
            return 0;
        case 10:
            if ((arg0 & 0x3F) != 6) {
                break;
            }
            func_80055604(arg1);
            return 0;
        }
        return -1;
    case 1:
        id = (arg1 & 0xFFFF) | 0x80000;
        h = 0;
        if (kind != 5 && kind != 6) {
            h = (s32)db_lookupRegistered(4, id, 0);
        }
        /* the original jump tables are 8-aligned within their object */
        __asm__(".rdata\n\t.word 0\n\t.text");
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
            case 3:
                func_8005593C(h, arg2, arg3);
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
            case 3:
                func_80055980(h, arg2, arg3, arg4);
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
            case 3:
                func_800558DC(arg2);
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
            case 3:
                func_80055908(arg2, arg3);
                return -1;
            }
            return -1;
        }
        return -1;
    case 3:
        __asm__(".rdata\n\t.word 0\n\t.text");
        id = (arg1 & 0xFFFF) | 0x90000;
        switch (kind) {
        case 7:
            if (arg2 != 0) {
                if (g_gameState->unk1C->unk834 == (arg1 & 0xFFFF)) {
                    return 0;
                }
                g_gameState->unk1C->unk834 = arg1;
            }
            D_800CCA14 = kind;
            D_800CCA10 = id;
            g_gameState->unk1C->unk851 = 1;
            return g_gameState->unk1C->unk851;
        case 12:
            goto state;
        }
        return -1;
    }
    return -1;

state:
    switch (g_gameState->unk1C->unk851) {
    case 1:
        if (func_800AC25C() == 0) {
            if (g_gameState->unk1C->unk0 & 0x200000) {
                func_800552B8(g_gameState->unk1C->unk824[0]);
                func_800552B8(g_gameState->unk1C->unk824[1]);
                g_gameState->unk1C->unk0 &= ~0x200000;
            }
            g_gameState->unk1C->unk851 = 2;
        }
        break;
    case 2:
        if (((D_800CCA10 >> 16) & 0xFF) == 7) {
            g_gameState->unk1C->unk852 = g_gameState->unk1C->unk820[0];
        } else {
            g_gameState->unk1C->unk852 = D_800CCA10;
        }
        D_800CCA18 = func_800AC29C(D_800CCA10, &sz);
        func_80055DF4();
        if (!(g_gameState->unk1C->unk8E4.world->unk0 & 0x400)) {
            g_gameState->unk1C->unk8C0 = (s32)(D_80067944 + 0x35000);
            g_gameState->unk1C->unk878 = -1;
        } else if (g_gameState->unk1C->unk8E4.world->unk0 & 1) {
            g_gameState->unk1C->unk8C0 = (s32)(D_80067944 + 0x91800);
            g_gameState->unk1C->unk878 = 0x48000;
        } else {
            g_gameState->unk1C->unk8C0 = func_800B8108();
            g_gameState->unk1C->unk878 = 0x10000;
        }
        g_gameState->unk1C->unk87C = 0;
        func_8002177C(g_gameState->unk1C->unk854, D_800CCA18, sz, (void *)g_gameState->unk1C->unk8C0, 0,
                      g_gameState->unk1C->unk878, func_800AC424);
        g_gameState->unk1C->unk851 = 3;
        break;
    case 3:
        if (func_800217C4(g_gameState->unk1C->unk854) == 0 && func_80055DCC() == 0) {
            if (g_gameState->unk1C->unk854[0].unkC + g_gameState->unk1C->unk854[0].unk10 < g_gameState->unk1C->unk87C) {
                g_gameState->unk1C->unk8C0 = (s32)g_gameState->unk1C->unk854[0].unk8;
                d = 0x10000 + func_800B8108() - g_gameState->unk1C->unk8C0;
                d = d / 2048 * 2048;
                func_8002177C(g_gameState->unk1C->unk854, D_800CCA18, g_gameState->unk1C->unk854[0].unk4, (void *)g_gameState->unk1C->unk8C0,
                              g_gameState->unk1C->unk854[0].unkC + g_gameState->unk1C->unk854[0].unk10, d, func_800AC424);
            } else if (D_800CCA12 == 7) {
                if (D_800CCA14 != 9) {
                    g_gameState->unk1C->unk826[0] = 0x7F;
                }
                g_gameState->unk1C->unk851 = 4;
            } else {
                D_800CCA14 = -1;
                g_gameState->unk1C->unk851 = 0;
            }
        }
        break;
    case 4:
        n = g_gameState->unk1C->unk854[0].unkC + g_gameState->unk1C->unk854[0].unk10 - g_gameState->unk1C->unk87C;
        if (n > 0) {
            memcpy(g_gameState->unk1C->unk814, (u8 *)g_gameState->unk1C->unk8C0 + (g_gameState->unk1C->unk87C - g_gameState->unk1C->unk854[0].unkC), n);
        }
        t = 0x6800 - n;
        g_gameState->unk1C->unk8C0 = (s32)((u8 *)g_gameState->unk1C->unk814 + n);
        t = t / 2048 * 2048;
        func_8002177C(g_gameState->unk1C->unk854, D_800CCA18, g_gameState->unk1C->unk854[0].unk4, (void *)g_gameState->unk1C->unk8C0,
                      g_gameState->unk1C->unk854[0].unkC + g_gameState->unk1C->unk854[0].unk10, t, NULL);
        g_gameState->unk1C->unk851 = 5;
        break;
    case 5:
        if (func_800217C4(g_gameState->unk1C->unk854) == 0) {
            if (g_gameState->unk1C->unk854[0].unkC + g_gameState->unk1C->unk854[0].unk10 < g_gameState->unk1C->unk854[0].unk4) {
                m = g_gameState->unk1C->unk854[0].unkC + g_gameState->unk1C->unk854[0].unk10 - g_gameState->unk1C->unk87C;
                g_gameState->unk1C->unk8C0 = (s32)((u8 *)g_gameState->unk1C->unk814 + m);
                func_8002177C(g_gameState->unk1C->unk854, D_800CCA18, g_gameState->unk1C->unk854[0].unk4, (void *)g_gameState->unk1C->unk8C0, m, 0x6800 - m, NULL);
            } else {
                func_800AC3A8((s32)g_gameState->unk1C->unk814);
                g_gameState->unk1C->unk0 |= 0x10000000;
                g_gameState->unk1C->unk851 = 0;
                if (D_800CCA14 != 7) {
                    goto start;
                }
                D_800CCA14 = -1;
            }
        }
        break;
    }
    return g_gameState->unk1C->unk851;
}

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
    gte_SetRotMatrix(&g_gameState->unk1C->unk18);
    gte_SetTransMatrix(&g_gameState->unk1C->unk18);
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
    GameData *gd = g_gameState->unk1C;

    gd->unk826[0] = arg0;
    if (arg0 == 0) {
        gd->unk842 = arg1;
        gd->unk8C4 = VSync(-1);
    }
}

s32 func_800AC25C(void) {
    GameData *gd = g_gameState->unk1C;

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
        e = dep_find(g_gameState->unk1C->depTable, arg0);
        if (arg1 != NULL) {
            *arg1 = e->unk8 << 11;
        }
        return e->unk4;
    } else {
        f = file_find(0xB, arg0);
        if (arg1 != NULL) {
            *arg1 = (f->unkC - f->unk4) << 11;
        }
        return f->unk4;
    }
}

s32 func_800AC344(s32 arg0) {
    FileTable *t = g_gameState->unk1C->depTable;
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
    s32 t = db_lookup(arg0, 3, 0x70000, 0);

    memcpy((u8 *)arg0, (u8 *)db_lookup(arg0, 4, t, 0), db_lookup(arg0, 5, t, 0));
    return 0;
}

void func_800AC424(Unk800AC424 *arg0) {
    s32 a;
    s32 id;
    s32 t;
    s32 n;
    s32 b;

    a = arg0->unk8;
    id = g_gameState->unk1C->unk852 | 0x90000;
    if (arg0->unkC == 0) {
        b = a;
        t = db_lookup(a, 2, 0x1B0000, 0);
        if (t == -1) {
            a = db_lookup(a, 4, id, 0);
            g_gameState->unk1C->unk87C = func_8001CA70(b);
        } else {
            a = db_lookup(t, 4, id, 0);
            g_gameState->unk1C->unk87C = db_lookup(b, 2, 0x1B0001, 0) - g_gameState->unk1C->unk8C0;
        }
    }
    n = g_gameState->unk1C->unk87C - arg0->unkC;
    if (arg0->unk10 < n) {
        n = arg0->unk10;
    }
    func_80055E14(a, arg0->unk8 + n - a, 0);
    arg0->unk8 = a;
}

void func_800AC580(void) {
    GameData *gd = g_gameState->unk1C;

    ClearOTagR(gd->unk10[gd->unk8] - 8, 0x1010);
}

void func_800AC5C8(void) {
    GameState *gs;
    GameData *gd;
    s32 f;

    if ((g_padPortHeld[0] & 0xF09) == 0xF09 || (g_padPortHeld[1] & 0xF09) == 0xF09) {
        g_gameState->unk0 |= 1;
    }
    DrawSync(0);
    gs = g_gameState;
    f = gs->unk1C->unk8E4.world->unk0;
    if (!(f & 0x400)) {
        gs->vsyncMode = 2;
    } else if ((f & 0x1000) || (gs->unk1C->unk0 & 0x1C) == 0x1C) {
        gs->vsyncMode = 0;
    } else {
        gs->vsyncMode = 3;
    }
    VSync(g_gameState->vsyncMode);
    ResetGraph(1);
    PutDispEnv(&g_gameState->unkC[g_gameState->unk1C->unk8]);
    gd = g_gameState->unk1C;
    if (!(gd->unk0 & 0x100)) {
        if (gd->unk8E4.world->unk0 & 0x400) {
            DrawOTagEnv(gd->unk10[gd->unk8] + 0xFFF, &g_gameState->unk10[gd->unk8]);
        } else {
            DrawOTag(gd->unk10[gd->unk8]);
        }
    }
    if (!(g_gameState->unk1C->unk0 & 0x20)) {
        g_gameState->unk1C->unk8 ^= 1;
        func_8002C0DC(g_gameState->unk1C->unk8);
    }
}

void func_800AC7D4(void) {
    SVECTOR v;
    WorldMapObj *o;
    Unk8004AB64 *a;
    WmObjSlot *s;

    D_8007BD28 = 0;
    D_8007BD30 = func_800B8114();
    o = g_gameState->unk1C->unk8E4.world->unk8;
    o->unk19 = 0;
    a = func_8004AE00();
    if (a != NULL && (s = (WmObjSlot *)func_8001E350(a->unk3B)) != NULL) {
        v.vx = ((MATRIX *)s->unk8->unk20)[1].t[0];
        v.vy = ((MATRIX *)s->unk8->unk20)[1].t[1];
        v.vz = ((MATRIX *)s->unk8->unk20)[1].t[2];
        gte_SetRotMatrix(&g_gameState->unk1C->unk18);
        gte_SetTransMatrix(&g_gameState->unk1C->unk18);
        gte_ldv0(&v);
        gte_rtps();
        gte_stsxy(&g_gameState->unk1C->unk8E4.world->unk8->unkC);
        if ((u16)g_gameState->unk1C->unk8E4.world->unk8->unkC >= 320 ||
            g_gameState->unk1C->unk8E4.world->unk8->unkE < 0 ||
            g_gameState->unk1C->unk8E4.world->unk8->unkE >= 218) {
            g_gameState->unk1C->unk8E4.world->unk8->unkC = 160;
            g_gameState->unk1C->unk8E4.world->unk8->unkE = 109;
        }
    }
    g_gameState->unk1C->unk8E4.world->unk0 &= ~0x400;
    func_800AD180();
    func_800ACE24();
    if (g_gameState->unk1C->unk81C[0] != g_gameState->unk1C->unk8E4.world->unk8->unk26 &&
        (g_gameState->unk1C->unk0 & 0x20000000)) {
        func_800559FC(0, 0xF, 0);
    }
    g_gameState->unk1C->unk8E4.world->unk8->unk18 = 5;
    func_800AB0C0(0xD000, 0x27C, 0, 0x80, 0x7F);
    func_800AB0C0(0xD000, 0x27B, 0, 0x80, 0x7F);
    func_800AB0C0(0xD000, 0x27A, 0, 0x80, 0x7F);
    g_gameState->unk1C->unk0 |= 0x2F;
    g_gameState->unk1C->unk8E4.world->unk0 |= 0x300;
    g_gameState->vsyncMode = 0;
    D_80073998[0] |= 0x04000000;
}

void func_800ACAFC(void) {
    WorldMapObj *w;
    WorldMapObj *o;
    GameState *gs;
    GameData *gd;
    s32 size;

    w = g_gameState->unk1C->unk8E4.world->unk8;
    if (!(w->unk0 & 1)) {
        if (w->unk19 >= 2) {
            if ((u8)func_80053A10(w->unkC, w->unkE)) {
                g_gameState->unk1C->unk8E4.world->unk8->unk0 |= 1;
            }
        } else {
            w->unk19++;
        }
    }
    gs = g_gameState;
    gd = gs->unk1C;
    o = gd->unk8E4.world->unk8;
    if (o->unk18 > 0) {
        o->unk18--;
    } else if (o->unk18 == 0) {
        if (o->unk26 != 0xFFFF && o->unk26 != gd->unk81C[0]) {
            cd_seekAsync(g_gameState->unk20 + func_800AC29C(o->unk26 | 0x70000, &size, gs), 0);
            if (g_gameState->unk1C->unk0 & 0x20000000) {
                func_800AB0C0(0x800, g_gameState->unk1C->unk81C[0], 0, 0, 0);
            }
            func_800AB0C0(0x700, g_gameState->unk1C->unk8E4.world->unk8->unk26, 0, 0, 0);
            g_gameState->unk1C->unk8E4.world->unk8->unk18 = -1;
        } else {
            cd_seekAsync(g_gameState->unk20 + file_find(0, 0x1000B)->unk4, 0);
            g_gameState->unk1C->unk8E4.world->unk8->unk18 = -2;
        }
    } else if (o->unk18 == -1) {
        if (func_800AB0C0(0xC00, 0, 0, 0, 0) == 0) {
            cd_seekAsync(g_gameState->unk20 + file_find(0, 0x1000B)->unk4, 0);
            func_800AB0C0(0, g_gameState->unk1C->unk8E4.world->unk8->unk26, 0, 0, 0);
            func_800559C8(0, 0x7F);
            g_gameState->unk1C->unk8E4.world->unk8->unk18 = -2;
        }
    } else if (o->unk18 == -2 && (o->unk0 & 1)) {
        gd->unk0 &= ~0x2F;
        g_gameState->unk1C->unk8E4.world->unk0 &= ~0x300;
        D_80073998[0] &= ~0x04000000;
        g_gameState->unk0 |= 0x1000;
    }
}

void func_800ACE24(void) {
    GameData *gd = g_gameState->unk1C;
    s32 i = 0;
    WorldMapObj *obj = gd->unk8E4.world->unk8;
    FileEntry *e;
    s32 id;
    s32 v;

    obj->unk26 = gd->unk81C[0];
    id = (u16)obj->unk24 | 0x110000;
    e = g_gameState->unk1C->depTable->unk4;
    for (; i < g_gameState->unk1C->depTable->unk2; i++) {
        v = e[i].unkC;
        if (e[i].unk0 == id && ((v >> 16) & 0xFF) == 7) {
            g_gameState->unk1C->unk8E4.world->unk8->unk26 = v;
            return;
        }
    }
}

void func_800ACEEC(void) {
    u8 buf[8];
    s32 i;
    u32 a;

    g_gameState->unk1C->unk0 ^= 0x100;
    if (g_gameState->unk1C->unk0 & 0x100) {
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
        g_gameState->unk1C->unk0 |= 0x20;
    } else {
        func_80023F60(0);
        func_80055698(0);
        g_gameState->unk1C->unk0 &= ~0x20;
    }
}

s32 func_800AD10C(void) {
    return 0;
}

void func_800AD114(s32 arg0, s32 arg1, s32 arg2) {
    RECT r;
    RECT *rp = NULL;

    if (!(g_gameState->unk1C->unk8E4.world->unk0 & 0x400)) {
        rp = &r;
        r.x = 0x280;
        r.w = 0x140;
        r.y = 0;
        r.h = 0xDA;
    }
    func_8001DAA4(arg0, arg1, arg2, rp);
}

void func_800AD180(void) {
    FileTable *t = g_gameState->unk1C->depTable;
    u8 *dst;

    if (t != (FileTable *)-1) {
        dst = D_80067944 + 0x146F00;
        func_800AD114((s32)dst, (s32)t, (t->unk2 * 0x10) | 4);
        DrawSync(0);
        g_gameState->unk1C->depTable = (FileTable *)dst;
    }
}
