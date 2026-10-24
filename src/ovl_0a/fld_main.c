#include "ovl_0a.h"

void func_800A7A48(void) {
    func_800A8198();
    do {
        func_800A7AAC((s16)g_gameState->unk1C->unk3A);
    } while (!(g_gameState->unk0 & 3));
    func_800A82F8();
}

void func_800A7AAC(s32 arg0) {
    func_800A85F4(arg0);
    do {
        fld_main((s16)g_gameState->unk1C->fieldId);
    } while (!(g_gameState->unk0 & 7));
    func_800A8698();
}

/* field main loop */
void fld_main(s32 arg0) {
    fld_load(arg0);
    SetDispMask(1);
    do {
        func_800AE788();
        pad_update();
        if ((g_padPortPressed & 0x04000000) && (g_padPortHeld[0] & 0xF8) && !(g_padPortHeld[0] & 0xFE0001)) {
            func_800AF740();
        }
        if (!(g_gameState->unk1C->unk0 & 0x100)) {
            fld_movieUpdate();
            if (!(g_gameState->unk1C->unk0 & 1) && (g_padPortPressed & BTN_TRIANGLE) && (g_padPortHeld[0] & 0x400F0) &&
                !(g_padPortHeld[0] & 0x04FA0009)) {
                g_gameState->unk1C->unk0 |= 2;
                menu_open(0, 0);
                func_8004A9E8();
            }
            if (!(g_gameState->unk1C->unk0 & 2)) {
                switch (func_8004BC38()) {
                case 3:
                    g_gameState->unk1C->unk8E4.field->unkC->unk14->unk4A = 2;
                    func_800AED04();
                    break;
                case 4:
                    if (g_gameState->unk1C->unk8E4.field->unkC->unk14->unkA == 0x3E80) {
                        g_gameState->unk1C->unk8E4.field->unkC->unk14->unk4A = 4;
                        g_gameState->unk0 |= 2;
                    } else {
                        g_gameState->unk0 |= 8;
                    }
                    break;
                case 5:
                    g_gameState->unk1C->unk8E4.field->unkC->unk14->unk4A = 3;
                    g_gameState->unk0 |= 2;
                    break;
                case 7:
                    g_gameState->unk1C->unk8E4.field->unkC->unk14->unk4A = 9;
                    g_gameState->unk0 |= 2;
                    break;
                case 8:
                    g_gameState->unk1C->unk8E4.field->unkC->unk14->unk4A = 7;
                    g_gameState->unk0 |= 2;
                    break;
                }
            }
            if (!(g_gameState->unk1C->unk0 & 1)) {
                func_80032100();
            }
            if (g_gameState->unk0 & 0x100000) {
                g_gameState->unk0 |= 8;
                g_gameState->unk1C->unk8E4.field->unk0 |= 0x10000;
                break;
            }
            if (!(g_gameState->unk1C->unk0 & 8)) {
                func_800BFEE0();
            }
            if (!(g_gameState->unk1C->unk0 & 4)) {
                func_800AB3CC();
            }
            if (!(g_gameState->unk1C->unk8E4.field->unk0 & 8)) {
                func_800C60F0();
                func_800C7100();
            }
            if (!(g_gameState->unk1C->unk8E4.field->unk0 & 0x10)) {
                func_80054B78();
            }
            if (!(g_gameState->unk1C->unk0 & 0x10)) {
                func_800AFDE8();
            }
            if (!(g_gameState->unk1C->unk0 & 8)) {
                func_800B3684();
            }
            if (!(g_gameState->unk1C->unk8E4.field->unk0 & 1)) {
                func_80023A0C();
            }
            if (!(g_gameState->unk1C->unk8E4.field->unk0 & 0x800)) {
                func_80032098();
            }
            if (!(g_gameState->unk1C->unk8E4.field->unk0 & 4)) {
                func_800AED6C();
            }
            if (!(g_gameState->unk1C->unk8E4.field->unk0 & 0x80)) {
                func_80023A4C();
            }
            if (!(g_gameState->unk1C->unk8E4.field->unk0 & 0x1000)) {
                preload_update();
            }
        }
        func_800AE930();
    } while (!(g_gameState->unk0 & 0xF));
    if (func_800B4F9C() != 0) {
        func_800B4A5C();
    }
    if ((u8)func_800B54CC()) {
        func_800B50A4();
    }
    fld_exit();
}

void fld_gameOver(void) {
    s32 i;

    gameover_main(g_gameState->unk1C->unk8E4.field->unkC->unk14->unkC);
    g_gameState->mode = 0;
    g_gameState->unk0 |= 0x100000;
    if ((g_gameState->unk1C->unk0 & 0x20000000) && g_gameState->unk1C->unk81C[0] != 0xFFFF) {
        func_800B0124(0x8301, g_gameState->unk1C->unk81C[0], 0x1E, 0, 0);
        for (i = 0x1D; i >= 0; i--) {
            VSync(0);
        }
        func_800B0124(0x100, g_gameState->unk1C->unk81C[0], 0, 0, 0);
    }
    D_80067950++;
}

void func_800A8198(void) {
    FieldMap *f;
    GameData *gd;
    GameData *gd2;

    func_800A8344();
    f = g_gameState->unk1C->unk8E4.field;
    func_800A8380();
    g_gameState->vsyncMode = 2;
    game_setScreenHeight(0xE0);
    game_freezeScreen();
    SetDispMask(1);
    g_gameState->unk1C->unk838 = func_800AAB30;
    g_gameState->unk1C->unk83C = func_800B0124;
    g_gameState->unk1C->unk8BC = func_800B36E0;
    func_80021CDC(0);
    g_gameState->unk0 &= ~7;
    g_gameState->unk1C->unk84C = 0;
    gd = g_gameState->unk1C;
    gd->unk39 = 0;
    gd->unk38 = 0;
    gd2 = g_gameState->unk1C;
    gd2->unk836 = 0xFFFF;
    gd2->unk834 = 0xFFFF;
    func_800B1D60();
    func_800AC5F0();
    func_800A847C();
    func_800A83E0();
    func_800239D0();
    if (g_gameState->unk1C->unk0 & 0x400000) {
        func_800B0124(0x8601, 0, 0x78, 0, 0);
    }
    func_8002FC30();
    f->unk4 = D_80067948;
}

void func_800A82F8(void) {
    s32 i;

    for (i = 1; i >= 0; i--) {
        g_padActuator[i].unk1 = 0;
        g_padActuator[i].unk0 = 0;
    }
    func_800B1CE4();
    func_800A8588();
}

void func_800A8344(void) {
    D_80067948 = (s32)(D_80067944 + 0x28000);
    D_80067948 += 0x2000;
    g_gameState->unk1C->unk8E4.field = (FieldMap *)D_80067948;
    D_80067948 += 0x50;
}

void func_800A8380(void) {
    s32 i = 0x2B;
    FieldMap *fm = g_gameState->unk1C->unk8E4.field;

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
    s32 addr = (s32)db_lookupRegistered(4, 0x90164, 0);
    s32 size = (s32)db_lookupRegistered(5, 0x90164, 0);

    func_8001DAA4((s32)(D_80067944 + 0x146F00), addr, size, NULL);
    DrawSync(0);
    g_gameState->unk1C->unk8E4.field->unk20 = size;
    return 0;
}

s32 func_800A847C(void) {
    PreloadEntry *p;
    FileEntry *f;

    if (func_800220E8(0x2714) != 0) {
        p = preload_find(0x2714);
        func_8001DAA4(D_80067948, p->unk8, p->unkC, NULL);
        func_80022390(0x2714);
        DrawSync(0);
    } else {
        f = file_find(1, 0x2714);
        func_8002177C(g_gameState->unk1C->unk854, f->unk4, (f->unkC - f->unk4) << 11, (void *)D_80067948, 0, -1, 0);
        while (func_800217C4(g_gameState->unk1C->unk854) != 0) {
        }
    }
    g_gameState->unk1C->unk8E4.field->unk10 = (void *)D_80067948;
    db_register((s32)g_gameState->unk1C->unk8E4.field->unk10, 0);
    return 0;
}

s32 func_800A8588(void) {
    void *p = g_gameState->unk1C->unk8E4.field->unk10;

    if (p != NULL) {
        db_unregister(p);
        g_gameState->unk1C->unk8E4.field->unk10 = NULL;
    }
    return 0;
}

void func_800A85F4(s32 arg0) {
    FieldMapObj *o;

    D_80067948 = g_gameState->unk1C->unk8E4.field->unk4;
    func_800A86FC();
    o = g_gameState->unk1C->unk8E4.field->unkC;
    func_800A8724();
    game_freezeScreen();
    func_800A87DC(o, arg0);
    func_800A879C(o, arg0);
    o->unk4 = D_80067948;
}

void func_800A8698(void) {
    GameData *gd = g_gameState->unk1C;
    FieldMapObj *obj = gd->unk8E4.field->unkC;

    func_800A8990(obj, gd->unk3A);
    g_gameState->unk1C->unk3A = obj->unk10;
}

void func_800A86FC(void) {
    FieldMapObj *p = (FieldMapObj *)D_80067948;

    D_80067948 = (s32)(p + 1);
    g_gameState->unk1C->unk8E4.field->unkC = p;
}

void func_800A8724(void) {
    FieldMapObj *obj = g_gameState->unk1C->unk8E4.field->unkC;
    s32 i;

    obj->unk0 = 0;
    obj->unk4 = 0;
    obj->unk8 = NULL;
    obj->unkC = NULL;
    obj->unk10 = g_gameState->unk1C->unk3A;
    obj->unk14 = NULL;
    for (i = 1; i >= 0; i--) {
        obj->unk12[i] = 0;
    }
    for (i = 0x1F; i >= 0; i--) {
        obj->unk18[i] = 0;
    }
}

s32 func_800A879C(FieldMapObj *arg0, u16 arg1) {
    arg0->unk8 = db_lookupRegistered(4, arg1 | 0x60000, 0);
    return 0;
}

s32 func_800A87DC(FieldMapObj *arg0, s32 arg1) {
    s32 id;
    GameData *gd;
    PreloadEntry *e;
    CdReadCtx *cd;
    GameData *g;
    s32 t;

    gd = g_gameState->unk1C;
    id = (u16)arg1 | 0x60000;
    cd = gd->unk854;
    if (func_800220E8(id) != 0) {
        e = preload_find(id);
        g_gameState->unk1C->unk854->unk20 = (u8 *)(e->unk8 + e->unkC);
        g_gameState->unk1C->unk854->unk10 = e->unkC;
        g_gameState->unk1C->unk854->unk4 = e->unk1C;
        func_8001DAA4(D_80067948, e->unk8, e->unkC, NULL);
        func_80022390(id);
        DrawSync(0);
        g = g_gameState->unk1C;
        t = g->unk854->unk10;
        if (t < g->unk854->unk4) {
            func_8002177C(g->unk854, g->sceneSector[0], g->sceneSectors[0] << 11, (void *)(D_80067948 + t), t, -1, NULL);
            while (func_800217C4(g_gameState->unk1C->unk854) != 0) {
            }
        }
    } else {
        func_8002177C(cd, gd->sceneSector[0], gd->sceneSectors[0] << 11, (void *)D_80067948, 0, -1, NULL);
        while (func_800217C4(cd) != 0) {
        }
    }
    arg0->unkC = (void *)D_80067948;
    db_register(D_80067948, 0);
    D_80067948 += db_getSize((s32)arg0->unkC);
    return 0;
}

s32 func_800A8990(FieldMapObj *arg0, s16 arg1) {
    db_unregister(arg0->unkC);
    arg0->unkC = NULL;
    return 0;
}

/* field start-up: load the map and set up the object slots */
void fld_load(s32 arg0) {
    FieldScene *sc;
    s32 i;
    s32 j;

    D_80067948 = g_gameState->unk1C->unk8E4.field->unkC->unk4;
    func_800A9350();
    sc = g_gameState->unk1C->unk8E4.field->unkC->unk14;
    func_800A939C();
    game_freezeScreen();
    g_gameState->unk0 &= ~0x30F;
    g_gameState->unk1C->unk0 |= 0x20;
    g_gameState->unk1C->unk8E4.field->unk0 |= 4;
    game_setClearColor(0, 0, 0);
    g_padPortMask[0] &= 0xFFFF0006;
    for (i = 1; i >= 0; i--) {
        for (j = 3; j >= 0; j--) {
            D_800C9EF0.unk0[i][j] = 0;
            D_800C9EF0.unk10[i][j] = 0;
        }
    }
    g_gameState->unk1C->unk8E4.field->unk8 = 0;
    func_800AA4BC();
    g_gameState->unk1C->unk8E4.field->unk9 = 0;
    g_gameState->unk1C->fieldId = arg0;
    sc->unkA = arg0;
    fld_loadFile(sc, arg0);
    func_800A97D4(sc, arg0);
    func_800A9814(sc, arg0);
    func_800A9860(sc, arg0);
    func_800A9954(sc, arg0);
    func_800AA01C(&g_gameState->unk1C->unk8E4.field->unkC->unk14->unk50, &g_gameState->unk1C->unk8E4.field->unkC->unk14->unk52);
    func_800B3A80();
    func_800A9998();
    func_800B0124(0x900, -1, 0, 0, 0);
    func_80049ED8(sc->unkC);
    evt_init(sc->unkC);
    func_800AC474();
    func_800A9AAC();
    while (func_800B0124(0xC00, -1, 0, 0, 0) != 0) {
    }
    func_8004BFB0();
    g_gameState->unk1C->unk38 = func_8004AAF0(0);
    g_gameState->unk1C->unk8DC = (Unk8D4 *)D_80067948;
    g_gameState->unk1C->unk8E0 = (Unk8E0 *)(g_gameState->unk1C->unk8DC + g_gameState->unk1C->unk38);
    D_80067948 = (s32)(g_gameState->unk1C->unk8E0 + g_gameState->unk1C->unk38);
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
    sc->unk2C = (FieldObjPlace *)D_80067948;
    D_80067948 = func_800AD44C(D_80067948, g_gameState->unk1C->unk38 + 9,
                               g_gameState->unk1C->unk8E4.field->unkC->unk14->unk50,
                               g_gameState->unk1C->unk8E4.field->unkC->unk14->unk52);
    func_800B2528(sc, arg0);
    func_800B1C98();
    sc->unk20 = D_80067948;
    sc->unk4 = D_80067948;
    func_8004A8A4();
    g_padPortMask[0] |= 0x40000;
    g_gameState->unk1C->unk0 &= ~0x20;
}

void fld_exit(void) {
    FieldScene *sc;
    s32 i;
    s32 id;

    sc = g_gameState->unk1C->unk8E4.field->unkC->unk14;
    DrawSync(0);
    func_800B1E04(sc, g_gameState->unk1C->fieldId);
    g_padPortMask[0] &= 0xFFFF0006;
    func_800AA0A8(sc, g_gameState->unk1C->fieldId);
    func_800AA074(sc, g_gameState->unk1C->fieldId);
    func_800AAA54();
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
    switch (g_gameState->unk1C->unk8E4.field->unkC->unk14->unk4A) {
    case 2:
        fld_depFindBackground(fld_depFindBattle(sc->unkA, &g_gameState->unk1C->sceneSector[0], &g_gameState->unk1C->sceneSectors[0]),
                      &g_gameState->unk1C->sceneSector[1], &g_gameState->unk1C->sceneSectors[1]);
        g_gameState->unk1C->battleId = sc->unkA;
        g_gameState->mode = 2;
        g_gameState->unkA = 1;
        break;
    case 3:
        id = fld_depFindArea(fld_depFindField(sc->unkA, &g_gameState->unk1C->sceneSector[1], &g_gameState->unk1C->sceneSectors[1]),
                           &g_gameState->unk1C->sceneSector[0], &g_gameState->unk1C->sceneSectors[0]);
        g_gameState->unk1C->unk7DC = sc->unkA;
        g_gameState->unk1C->unk7DE = id;
        g_gameState->mode = 3;
        g_gameState->unkA = 1;
        break;
    case 9:
        if (g_gameState->unk1C->unk0 & 0x20000000) {
            func_800B0124(0x800, g_gameState->unk1C->unk81C[0], 0, 0, 0);
        }
        func_800B1C4C();
        func_80056228();
        g_gameState->mode = 9;
        g_gameState->unkA = 1;
        break;
    case 7:
        func_8005546C(-1, 0);
        func_80056228();
        g_gameState->mode = 7;
        g_gameState->unkA = 1;
        break;
    case 4:
        func_8005546C(-1, 0);
        func_80056228();
        g_gameState->mode = 4;
        g_gameState->unkA = 1;
        break;
    case 1:
        if (g_gameState->unk1C->unk8E4.field->unk0 & 0x10000) {
            id = (s16)g_gameState->unk1C->unk8E4.field->unkC->unk10;
        } else {
            id = fld_depFindArea(fld_depFindField(sc->unkA, &g_gameState->unk1C->sceneSector[1], &g_gameState->unk1C->sceneSectors[1]),
                               &g_gameState->unk1C->sceneSector[0], &g_gameState->unk1C->sceneSectors[0]);
        }
        if (id != (g_gameState->unk1C->unk3A | 0x60000)) {
            g_gameState->unk0 |= 4;
        }
        g_gameState->unk1C->unk8E4.field->unk0 &= ~0x10000;
        g_gameState->unk1C->unk8E4.field->unkC->unk10 = id;
        g_gameState->unk1C->unk8E4.field->unkA = g_gameState->unk1C->fieldId;
        g_gameState->unk1C->fieldId = sc->unkA;
        break;
    default:
        SystemError(0x39, 0xC9);
        break;
    }
    g_gameState->unk1C->depTable = (struct FileTable *)-1;
}

/* allocate the field scene block and its tables */
void func_800A9350(void) {
    FieldScene *sc;

    g_gameState->unk1C->unk8E4.field->unkC->unk14 = sc = (FieldScene *)D_80067948;
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

    sc = g_gameState->unk1C->unk8E4.field->unkC->unk14;
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

    gs = g_gameState;
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
                gd2->sceneSector[1] = e->unk4;
                gd2->sceneSectors[1] = e[1].unk4 - e->unk4;
                break;
            }
            e--;
        } while (e >= start);
        start = (FieldDirEnt *)(base + ((gs2->unk14->unk28 - t) << 11));
        e = &start[gs2->unk14->unk24] - 1;
        do {
            if (e->unk0 == id) {
                gd2->sceneSector[0] = e->unk4;
                gd2->sceneSectors[0] = e[1].unk4 - e->unk4;
                break;
            }
            e--;
        } while (e >= start);
        g_gameState->unk1C->unk8E4.field->unkC->unk10 = id;
        g_gameState->unk1C->unk8A8 = -1;
        g_gameState->unk1C->unk84A = 0xFFFF;
        start = (FieldDirEnt *)(base + ((gs2->unk14->unk98 - t) << 11));
        e = &start[gs2->unk14->unk94] - 1;
        do {
            if (e->unk0 == g_gameState->unk1C->unk81C[0]) {
                g_gameState->unk1C->unk8A8 = e->unk4;
                g_gameState->unk1C->unk84A = e[1].unk4 - e->unk4;
                break;
            }
            e--;
        } while (e >= start);
        func_800AC5F0();
        func_80022280((s32)g_gameState->unk1C->unk854[0].unk8, g_gameState->unk1C->unk854[0].unk10);
    }
}

s32 func_800A97D4(FieldScene *arg0, u16 arg1) {
    arg0->unkC = db_lookupRegistered(4, arg1 | 0x50000, 0);
    return 0;
}

s32 func_800A9814(FieldScene *arg0, u16 arg1) {
    void *p = db_lookupRegistered(4, arg1 | 0x1E0000, 0);

    arg0->unk4C = p;
    if (p != NULL) {
        func_80054A28(p);
    }
    return 0;
}

s32 func_800A9860(FieldScene *arg0, s32 arg1) {
    GameData *gd = g_gameState->unk1C;
    s32 id;

    func_800C0D40(&gd->unk8, &gd->unkA, gd->unk10[0], gd->unk10[1], &gd->unk18, D_80067948 + 0x400);
    func_800C0D8C(D_80067948, 0x400);
    id = func_800AA0C8(arg0, arg1);
    arg0->unk14 = db_lookupRegistered(4, id, 0);
    func_800C0DA4(arg0->unk14);
    D_80067948 = func_800BFCE8();
    func_800C7430();
    arg0->unk18 = db_lookupRegistered(4, (s32)db_lookupRegistered(9, id, 0xB0000), 0);
    func_800C3B80((u8 *)arg0->unk18 + 4);
    D_80067948 = func_800AFCBC(D_80067948);
    return 0;
}

s32 func_800A9954(FieldScene *arg0) {
    arg0->unk1C = db_lookupRegistered(4, (u16)func_800AA0C8(arg0) | 0x1F0000, 0);
    return 0;
}

void func_800A9998(void) {
    while (func_800B0124(0xC00, -1, 0, 0, 0) != 0) {
    }
    if (g_gameState->unk1C->unk0 & 0x400000) {
        func_800B0124(0x100, g_gameState->unk1C->unk81C[0], 0, 0, 0);
        g_gameState->unk1C->unk0 &= ~0x400000;
    }
    if (g_gameState->unk1C->unk8E4.field->unk20 != -1) {
        func_80055DF4();
        func_80055E14((s32)D_80067944 + 0x146F00, g_gameState->unk1C->unk8E4.field->unk20, 1);
        g_gameState->unk1C->unk8E4.field->unk20 = -1;
    }
}

s32 func_800A9AAC(void) {
    s32 best;
    s32 i;
    s32 min;
    s32 v;

    if (g_gameState->unk1C->unk8E4.field->unkC->unk14->unk60 != 0) {
        while (1) {
            best = -1;
            min = 0x7FFFFFFF;
            for (i = g_gameState->unk1C->unk8E4.field->unkC->unk14->unk60 - 1; i >= 0; i--) {
                if (D_800C9F20[i] != -1) {
                    v = file_find(0xA, D_800C9F20[i])->unk4;
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

s32 fld_loadFile(FieldScene *sc, s32 arg1) {
    s32 size;
    s32 id;
    PreloadEntry *e;
    s32 base;
    s32 i;
    s32 r;
    s32 rest;
    s32 k;

    size = g_gameState->unk1C->sceneSectors[1] << 11;
    if ((s32)(0x801EDF00 - (D_80067948 + size)) < 0x17000) {
        size = 0x801D6F00 - D_80067948;
        size = size / 2048 * 2048;
    }
    id = (u16)arg1 | 0x50000;
    if (func_800220E8(id)) {
        e = preload_find(id);
        g_gameState->unk1C->unk854[0].unk20 = (u8 *)(e->unk8 + e->unkC);
        g_gameState->unk1C->unk854[0].unk10 = e->unkC;
        g_gameState->unk1C->unk854[0].unk4 = e->unk1C;
        func_8001DAA4(D_80067948, e->unk8, e->unkC, NULL);
        func_80022390(id);
        DrawSync(0);
    } else {
        func_8002177C(g_gameState->unk1C->unk854, g_gameState->unk1C->sceneSector[1], g_gameState->unk1C->sceneSectors[1] << 11,
                      (void *)D_80067948, 0, size, NULL);
        while (func_800217C4(g_gameState->unk1C->unk854) != 0) {
        }
    }
    func_800AC67C((s32)g_gameState->unk1C->unk854[0].unk20);
    base = D_80067948;
    g_gameState->unk1C->depTable = (FileTable *)db_lookup(base, 4, 0x140000, 0);
    id = db_lookup(base, 2, 0x1B0000, 0);
    if (id != 0) {
        r = db_lookup(base, 5, 0x1B0000, 0);
        if ((u32)(D_80067948 + g_gameState->unk1C->unk854[0].unk10) < (u32)(id + r)) {
            rest = g_gameState->unk1C->unk854[0].unk10;
            func_8002177C(g_gameState->unk1C->unk854, g_gameState->unk1C->sceneSector[1], g_gameState->unk1C->sceneSectors[1] << 11,
                          (void *)(D_80067948 + rest), rest, size - g_gameState->unk1C->unk854[0].unk10, NULL);
            while (func_800217C4(g_gameState->unk1C->unk854) != 0) {
            }
            g_gameState->unk1C->unk854[0].unk10 += rest;
            func_800AC67C((s32)g_gameState->unk1C->unk854[0].unk20);
        }
        for (i = db_lookup(id, 1, 0x40000, 0) - 1; i >= 0; i--) {
            k = db_lookup(id, 2, (u16)i | 0x40000, 0);
            if (k != -1) {
                func_8001D8A8(k);
            }
        }
        for (i = db_lookup(id, 1, 0x170000, 0) - 1; i >= 0; i--) {
            k = db_lookup(id, 2, (u16)i | 0x170000, 0);
            if (k != -1) {
                func_800C9968((s32 *)k);
            }
        }
        DrawSync(0);
    }
    sc->unk24 = (void *)db_lookup(base, 2, 0x1B0001, 0);
    size = g_gameState->unk1C->unk854[0].unk10 - ((s32)sc->unk24 - base);
    func_8001DAA4(id, (s32)sc->unk24, size, NULL);
    DrawSync(0);
    sc->unk24 = (void *)id;
    if (g_gameState->unk1C->unk854[0].unk10 < g_gameState->unk1C->unk854[0].unk4) {
        func_8002177C(g_gameState->unk1C->unk854, g_gameState->unk1C->sceneSector[1], g_gameState->unk1C->sceneSectors[1] << 11,
                      (void *)(id + size), g_gameState->unk1C->unk854[0].unk10, -1, NULL);
        while (func_800217C4(g_gameState->unk1C->unk854) != 0) {
        }
    }
    db_register((s32)sc->unk24, 0);
    D_80067948 = (s32)sc->unk24 + db_getSize((s32)sc->unk24);
    return 0;
}

s32 func_800AA01C(u16 *arg0, u16 *arg1) {
    Unk800AA01C *p = db_lookupRegistered(4, 0x1C0000, 0);

    *arg0 = p->unk30;
    *arg1 = p->unk32;
    return 0;
}

s32 func_800AA074(FieldScene *arg0, s16 arg1) {
    db_unregister(arg0->unk24);
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
    return db_lookup((s32)arg0->unk24, 3, 0xA0000, 0);
}

s32 fld_depFindBattle(u16 arg0, s32 *arg1, s32 *arg2) {
    s32 i;
    FileTable *t = g_gameState->unk1C->depTable;
    s32 id = arg0 | 0x110000;
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

s32 fld_depFindBackground(s32 arg0, s32 *arg1, s32 *arg2) {
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

s32 fld_depFindField(u16 arg0, s32 *arg1, s32 *arg2) {
    s32 i;
    FileTable *t = g_gameState->unk1C->depTable;
    s32 id = arg0 | 0x50000;
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

s32 fld_depFindArea(s32 arg0, s32 *arg1, s32 *arg2) {
    FileTable *t = g_gameState->unk1C->depTable;
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
