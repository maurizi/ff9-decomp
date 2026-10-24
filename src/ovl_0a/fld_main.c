#include "ovl_0a.h"

void fld_entry(void) {
    fld_init();
    do {
        fld_runArea((s16)g_gameState->unk1C->unk3A);
    } while (!(g_gameState->unk0 & (GS_RESET | GS_FLD_LEAVE)));
    fld_quit();
}

void fld_runArea(s32 arg0) {
    fld_loadArea(arg0);
    do {
        fld_main((s16)g_gameState->unk1C->fieldId);
    } while (!(g_gameState->unk0 & (GS_RESET | GS_FLD_LEAVE | GS_FLD_NEW_AREA)));
    fld_unloadArea();
}

/* field main loop */
void fld_main(s32 arg0) {
    fld_load(arg0);
    SetDispMask(1);
    do {
        fld_beginFrame();
        pad_update();
        if ((g_padPortPressed & BTN_START) && (g_padPortHeld[0] & (PAD_START | PAD_UP | PAD_RIGHT | PAD_DOWN | PAD_LEFT)) && !(g_padPortHeld[0] & (PAD_SELECT | BTN_CROSS | BTN_TRIANGLE | BTN_SQUARE | BTN_L1 | BTN_R1 | BTN_L2 | BTN_R2))) {
            fld_togglePause();
        }
        if (!(g_gameState->unk1C->unk0 & GD_PAUSED)) {
            fld_movieUpdate();
            if (!(g_gameState->unk1C->unk0 & GD_MENU_DISABLED) && (g_padPortPressed & BTN_TRIANGLE) && (g_padPortHeld[0] & (PAD_UP | PAD_RIGHT | PAD_DOWN | PAD_LEFT | BTN_TRIANGLE)) &&
                !(g_padPortHeld[0] & (PAD_SELECT | PAD_START | BTN_CROSS | BTN_SQUARE | BTN_L1 | BTN_R1 | BTN_L2 | BTN_R2 | BTN_START))) {
                g_gameState->unk1C->unk0 |= GD_MENU_OPEN;
                menu_open(0, 0);
                func_8004A9E8();
            }
            if (!(g_gameState->unk1C->unk0 & GD_MENU_OPEN)) {
                switch (func_8004BC38()) {
                case 3:
                    g_gameState->unk1C->unk8E4.field->area->scene->exitReason = FLD_EXIT_BATTLE;
                    fld_startEncounter();
                    break;
                case 4:
                    if (g_gameState->unk1C->unk8E4.field->area->scene->nextId == 0x3E80) {
                        g_gameState->unk1C->unk8E4.field->area->scene->exitReason = 4;
                        g_gameState->unk0 |= GS_FLD_LEAVE;
                    } else {
                        g_gameState->unk0 |= GS_FLD_NEW_FIELD;
                    }
                    break;
                case 5:
                    g_gameState->unk1C->unk8E4.field->area->scene->exitReason = FLD_EXIT_WORLD;
                    g_gameState->unk0 |= GS_FLD_LEAVE;
                    break;
                case 7:
                    g_gameState->unk1C->unk8E4.field->area->scene->exitReason = FLD_EXIT_TETRA_MASTER;
                    g_gameState->unk0 |= GS_FLD_LEAVE;
                    break;
                case 8:
                    g_gameState->unk1C->unk8E4.field->area->scene->exitReason = FLD_EXIT_GAME_OVER;
                    g_gameState->unk0 |= GS_FLD_LEAVE;
                    break;
                }
            }
            if (!(g_gameState->unk1C->unk0 & GD_MENU_DISABLED)) {
                func_80032100();
            }
            if (g_gameState->unk0 & GS_RELOAD_FILE_TABLES) {
                g_gameState->unk0 |= GS_FLD_NEW_FIELD;
                g_gameState->unk1C->unk8E4.field->flags |= FLD_MAP_REFIND_SECTORS;
                break;
            }
            if (!(g_gameState->unk1C->unk0 & GD_HIDE_BG)) {
                fldbg_draw();
            }
            if (!(g_gameState->unk1C->unk0 & GD_HIDE_MODELS)) {
                fld_drawModels();
            }
            if (!(g_gameState->unk1C->unk8E4.field->flags & FLD_MAP_NO_BGI_ANIM)) {
                bgi_updateAnims();
                bgi_updateFloorMoves();
            }
            if (!(g_gameState->unk1C->unk8E4.field->flags & 0x10)) {
                func_80054B78();
            }
            if (!(g_gameState->unk1C->unk0 & GD_HIDE_SPRITES)) {
                fld_drawSprites();
            }
            if (!(g_gameState->unk1C->unk0 & GD_HIDE_BG)) {
                func_800B3684();
            }
            if (!(g_gameState->unk1C->unk8E4.field->flags & FLD_MAP_NO_WINDOWS)) {
                func_80023A0C();
            }
            if (!(g_gameState->unk1C->unk8E4.field->flags & 0x800)) {
                func_80032098();
            }
            if (!(g_gameState->unk1C->unk8E4.field->flags & 4)) {
                fld_updateEncounter();
            }
            if (!(g_gameState->unk1C->unk8E4.field->flags & 0x80)) {
                func_80023A4C();
            }
            if (!(g_gameState->unk1C->unk8E4.field->flags & FLD_MAP_NO_PRELOAD)) {
                preload_update();
            }
        }
        fld_endFrame();
    } while (!(g_gameState->unk0 & (GS_RESET | GS_FLD_LEAVE | GS_FLD_NEW_AREA | GS_FLD_NEW_FIELD)));
    if (fld_needsSplashImage() != 0) {
        fld_showSplashImage();
    }
    if ((u8)fld_isDiscChangePending()) {
        fld_runDiscChange();
    }
    fld_exit();
}

void fld_gameOver(void) {
    s32 i;

    gameover_main(g_gameState->unk1C->unk8E4.field->area->scene->eventScript);
    g_gameState->mode = 0;
    g_gameState->unk0 |= GS_RELOAD_FILE_TABLES;
    if ((g_gameState->unk1C->unk0 & GD_MUSIC_PLAYING) && g_gameState->unk1C->unk81C[0] != 0xFFFF) {
        fld_soundCommand(0x8301, g_gameState->unk1C->unk81C[0], 0x1E, 0, 0);
        for (i = 0x1D; i >= 0; i--) {
            VSync(0);
        }
        fld_soundCommand(0x100, g_gameState->unk1C->unk81C[0], 0, 0, 0);
    }
    D_80067950++;
}

void fld_init(void) {
    FieldMap *f;
    GameData *gd;
    GameData *gd2;

    fld_allocMap();
    f = g_gameState->unk1C->unk8E4.field;
    fld_initMap();
    g_gameState->vsyncMode = 2;
    func_8001CD04(0xE0);
    func_8001CFB4();
    SetDispMask(1);
    g_gameState->unk1C->unk838 = fld_setupActorModel;
    g_gameState->unk1C->unk83C = fld_soundCommand;
    g_gameState->unk1C->unk8BC = fld_preloadRequest;
    func_80021CDC(0);
    g_gameState->unk0 &= ~(GS_RESET | GS_FLD_LEAVE | GS_FLD_NEW_AREA);
    g_gameState->unk1C->unk84C = 0;
    gd = g_gameState->unk1C;
    gd->unk39 = 0;
    gd->unk38 = 0;
    gd2 = g_gameState->unk1C;
    gd2->unk836 = 0xFFFF;
    gd2->unk834 = 0xFFFF;
    fld_clearSoundBankCache();
    fld_clearModelCache();
    fld_loadCommon();
    fld_readSoundBank();
    func_800239D0();
    if (g_gameState->unk1C->unk0 & 0x400000) {
        fld_soundCommand(0x8601, 0, 0x78, 0, 0);
    }
    func_8002FC30();
    f->loadEnd = D_80067948;
}

void fld_quit(void) {
    s32 i;

    for (i = 1; i >= 0; i--) {
        D_800739B0[i].unk1 = 0;
        D_800739B0[i].unk0 = 0;
    }
    fld_stopResidentSfx();
    fld_freeCommon();
}

void fld_allocMap(void) {
    D_80067948 = (s32)(D_80067944 + 0x28000);
    D_80067948 += 0x2000;
    g_gameState->unk1C->unk8E4.field = (FieldMap *)D_80067948;
    D_80067948 += 0x50;
}

void fld_initMap(void) {
    s32 i = 0x2B;
    FieldMap *fm = g_gameState->unk1C->unk8E4.field;

    fm->prevFieldId = -1;
    fm->flags = 0;
    fm->loadEnd = 0;
    fm->playerObj = 0;
    fm->unk9 = 0;
    fm->unk14 = -1;
    fm->unk18 = -1;
    fm->soundBankSize = -1;
    fm->commonFile = NULL;
    fm->area = NULL;
    do {
        fm->unk24[i] = 0;
        i--;
    } while (i >= 0);
}

s32 fld_readSoundBank(void) {
    s32 addr = (s32)db_lookupRegistered(4, 0x90164, 0);
    s32 size = (s32)db_lookupRegistered(5, 0x90164, 0);

    func_8001DAA4((s32)(D_80067944 + 0x146F00), addr, size, NULL);
    DrawSync(0);
    g_gameState->unk1C->unk8E4.field->soundBankSize = size;
    return 0;
}

s32 fld_loadCommon(void) {
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
    g_gameState->unk1C->unk8E4.field->commonFile = (void *)D_80067948;
    db_register((s32)g_gameState->unk1C->unk8E4.field->commonFile, 0);
    return 0;
}

s32 fld_freeCommon(void) {
    void *p = g_gameState->unk1C->unk8E4.field->commonFile;

    if (p != NULL) {
        db_unregister(p);
        g_gameState->unk1C->unk8E4.field->commonFile = NULL;
    }
    return 0;
}

void fld_loadArea(s32 arg0) {
    FieldMapObj *o;

    D_80067948 = g_gameState->unk1C->unk8E4.field->loadEnd;
    fld_allocArea();
    o = g_gameState->unk1C->unk8E4.field->area;
    fld_initArea();
    func_8001CFB4();
    fld_readAreaFile(o, arg0);
    fld_findAreaItem(o, arg0);
    o->loadEnd = D_80067948;
}

void fld_unloadArea(void) {
    GameData *gd = g_gameState->unk1C;
    FieldMapObj *obj = gd->unk8E4.field->area;

    fld_freeAreaFile(obj, gd->unk3A);
    g_gameState->unk1C->unk3A = obj->areaId;
}

void fld_allocArea(void) {
    FieldMapObj *p = (FieldMapObj *)D_80067948;

    D_80067948 = (s32)(p + 1);
    g_gameState->unk1C->unk8E4.field->area = p;
}

void fld_initArea(void) {
    FieldMapObj *obj = g_gameState->unk1C->unk8E4.field->area;
    s32 i;

    obj->unk0 = 0;
    obj->loadEnd = 0;
    obj->msgTable = NULL;
    obj->areaFile = NULL;
    obj->areaId = g_gameState->unk1C->unk3A;
    obj->scene = NULL;
    for (i = 1; i >= 0; i--) {
        obj->unk12[i] = 0;
    }
    for (i = 0x1F; i >= 0; i--) {
        obj->unk18[i] = 0;
    }
}

s32 fld_findAreaItem(FieldMapObj *arg0, u16 arg1) {
    arg0->msgTable = db_lookupRegistered(4, arg1 | DB_AREA, 0);
    return 0;
}

s32 fld_readAreaFile(FieldMapObj *arg0, s32 arg1) {
    s32 id;
    GameData *gd;
    PreloadEntry *e;
    CdReadCtx *cd;
    GameData *g;
    s32 t;

    gd = g_gameState->unk1C;
    id = (u16)arg1 | DB_AREA;
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
    arg0->areaFile = (void *)D_80067948;
    db_register(D_80067948, 0);
    D_80067948 += func_8001CA70((s32)arg0->areaFile);
    return 0;
}

s32 fld_freeAreaFile(FieldMapObj *arg0, s16 arg1) {
    db_unregister(arg0->areaFile);
    arg0->areaFile = NULL;
    return 0;
}

/* field start-up: load the map and set up the object slots */
void fld_load(s32 arg0) {
    FieldScene *sc;
    s32 i;
    s32 j;

    D_80067948 = g_gameState->unk1C->unk8E4.field->area->loadEnd;
    fld_allocScene();
    sc = g_gameState->unk1C->unk8E4.field->area->scene;
    fld_initScene();
    func_8001CFB4();
    g_gameState->unk0 &= ~0x30F;
    g_gameState->unk1C->unk0 |= GD_NO_FLIP;
    g_gameState->unk1C->unk8E4.field->flags |= 4;
    func_8001CCB4(0, 0, 0);
    D_80073998[0] &= 0xFFFF0006;
    for (i = 1; i >= 0; i--) {
        for (j = 3; j >= 0; j--) {
            g_fieldFlagQueue.gameFlags[i][j] = 0;
            g_fieldFlagQueue.mapFlags[i][j] = 0;
        }
    }
    g_gameState->unk1C->unk8E4.field->playerObj = 0;
    fld_clearModelQueue();
    g_gameState->unk1C->unk8E4.field->unk9 = 0;
    g_gameState->unk1C->fieldId = arg0;
    sc->nextId = arg0;
    fld_loadFile(sc, arg0);
    fld_findEventScript(sc, arg0);
    fld_loadVibration(sc, arg0);
    fld_setupBackground(sc, arg0);
    fld_findLights(sc, arg0);
    fld_getShadowTexPos(&g_gameState->unk1C->unk8E4.field->area->scene->shadowTexX, &g_gameState->unk1C->unk8E4.field->area->scene->shadowTexY);
    fld_initPreload();
    fld_startAudio();
    fld_soundCommand(0x900, -1, 0, 0, 0);
    func_80049ED8(sc->eventScript);
    evt_init(sc->eventScript);
    fld_compactModelCache();
    fld_loadModelBundlesInCdOrder();
    while (fld_soundCommand(0xC00, -1, 0, 0, 0) != 0) {
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
    sc->shadows = (FieldObjPlace *)D_80067948;
    D_80067948 = fld_initShadows(D_80067948, g_gameState->unk1C->unk38 + 9,
                               g_gameState->unk1C->unk8E4.field->area->scene->shadowTexX,
                               g_gameState->unk1C->unk8E4.field->area->scene->shadowTexY);
    fld_loadSoundBanks(sc, arg0);
    fld_resumeResidentSfx();
    sc->heapTop = D_80067948;
    sc->loadEnd = D_80067948;
    func_8004A8A4();
    D_80073998[0] |= BTN_TRIANGLE;
    g_gameState->unk1C->unk0 &= ~GD_NO_FLIP;
}

void fld_exit(void) {
    FieldScene *sc;
    s32 i;
    s32 id;

    sc = g_gameState->unk1C->unk8E4.field->area->scene;
    DrawSync(0);
    fld_stopSceneSfx(sc, g_gameState->unk1C->fieldId);
    D_80073998[0] &= 0xFFFF0006;
    fld_freeVibration(sc, g_gameState->unk1C->fieldId);
    fld_freeFieldFile(sc, g_gameState->unk1C->fieldId);
    fld_freeModelBundles();
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
    switch (g_gameState->unk1C->unk8E4.field->area->scene->exitReason) {
    case FLD_EXIT_BATTLE:
        fld_depFindBackground(fld_depFindBattle(sc->nextId, &g_gameState->unk1C->sceneSector[0], &g_gameState->unk1C->sceneSectors[0]),
                      &g_gameState->unk1C->sceneSector[1], &g_gameState->unk1C->sceneSectors[1]);
        g_gameState->unk1C->battleId = sc->nextId;
        g_gameState->mode = 2;
        g_gameState->unkA = 1;
        break;
    case FLD_EXIT_WORLD:
        id = fld_depFindArea(fld_depFindField(sc->nextId, &g_gameState->unk1C->sceneSector[1], &g_gameState->unk1C->sceneSectors[1]),
                           &g_gameState->unk1C->sceneSector[0], &g_gameState->unk1C->sceneSectors[0]);
        g_gameState->unk1C->unk7DC = sc->nextId;
        g_gameState->unk1C->unk7DE = id;
        g_gameState->mode = 3;
        g_gameState->unkA = 1;
        break;
    case FLD_EXIT_TETRA_MASTER:
        if (g_gameState->unk1C->unk0 & GD_MUSIC_PLAYING) {
            fld_soundCommand(0x800, g_gameState->unk1C->unk81C[0], 0, 0, 0);
        }
        fld_suspendResidentSfx();
        func_80056228();
        g_gameState->mode = 9;
        g_gameState->unkA = 1;
        break;
    case FLD_EXIT_GAME_OVER:
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
    case FLD_EXIT_FIELD:
        if (g_gameState->unk1C->unk8E4.field->flags & FLD_MAP_REFIND_SECTORS) {
            id = (s16)g_gameState->unk1C->unk8E4.field->area->areaId;
        } else {
            id = fld_depFindArea(fld_depFindField(sc->nextId, &g_gameState->unk1C->sceneSector[1], &g_gameState->unk1C->sceneSectors[1]),
                               &g_gameState->unk1C->sceneSector[0], &g_gameState->unk1C->sceneSectors[0]);
        }
        if (id != (g_gameState->unk1C->unk3A | DB_AREA)) {
            g_gameState->unk0 |= GS_FLD_NEW_AREA;
        }
        g_gameState->unk1C->unk8E4.field->flags &= ~FLD_MAP_REFIND_SECTORS;
        g_gameState->unk1C->unk8E4.field->area->areaId = id;
        g_gameState->unk1C->unk8E4.field->prevFieldId = g_gameState->unk1C->fieldId;
        g_gameState->unk1C->fieldId = sc->nextId;
        break;
    default:
        SystemError(0x39, 0xC9);
        break;
    }
    g_gameState->unk1C->depTable = (struct FileTable *)-1;
}

/* allocate the field scene block and its tables */
void fld_allocScene(void) {
    FieldScene *sc;

    g_gameState->unk1C->unk8E4.field->area->scene = sc = (FieldScene *)D_80067948;
    sc->modelBundles = sc->unk6C;
    D_80067948 += 0x90;
    sc->actorInfo = (FieldActorInfo *)D_80067948;
    D_80067948 += 0x1E0;
    sc->animSfx = (FieldAnimSfx *)D_80067948;
    D_80067948 += 0xC0;
}

void fld_initScene(void) {
    FieldScene *sc;
    s32 i;
    FieldActorInfo *info;
    u8 *base;
    FieldAnimSfx *t;
    u8 *c;
    u8 *c2;

    sc = g_gameState->unk1C->unk8E4.field->area->scene;
    sc->unk8 = 0;
    sc->flags = 0;
    sc->loadEnd = 0;
    sc->eventScript = NULL;
    sc->vibration = NULL;
    sc->bg = NULL;
    sc->walkmesh = NULL;
    base = D_80067944 + 0x106700;
    sc->otBias = 0;
    sc->sprites = NULL;
    sc->shadows = NULL;
    sc->heapTop = 0;
    sc->movieState = 0;
    sc->movieRgb24 = 0;
    sc->movieFile = 0;
    sc->movieList = 0;
    sc->movieEntry = 0;
    sc->lights = NULL;
    sc->movieWork = base;
    for (i = 23; i >= 0; i--) {
        info = &sc->actorInfo[i];
        info->flags = 0;
        info->litFloor = -1;
        info->abr = 0;
        info->unk7 = 0;
        info->unk6 = 0;
        c = info->color.c;
        c[2] = 0;
        c[1] = 0;
        c[0] = 0;
        info->color.c[3] = 0;
        c2 = info->nextColor.c;
        c = c2;
        c[2] = 0;
        c[1] = 0;
        c[0] = 0;
        info->nextColor.c[3] = 0;
        info->mirror = NULL;
    }
    sc->encounterScreenX = 0xA0;
    sc->encounterScreenY = 0x70;
    sc->battleMusic = 0xFFFF;
    sc->battleMusicBank = 0xFFFF;
    sc->encounterStep = 0;
    sc->encounterMusicStep = 0;
    sc->sndVolume = 0x7F;
    sc->movieSndVolume = 0x7F;
    sc->unk5F = 0;
    sc->shadowTexY = 0;
    sc->shadowTexX = 0;
    sc->fieldFile = NULL;
    for (i = 0; i < 9; i++) {
        sc->modelBundles[i] = 0;
    }
    sc->nextId = -1;
    sc->modelBundleCount = 0;
    sc->exitReason = FLD_EXIT_FIELD;
    sc->animSfxCount = 0;
    for (i = 15; i >= 0; i--) {
        t = &sc->animSfx[i];
        t->uid = 0xFF;
        t->randomPitch = 0;
        t->unk2 = 0xFFFF;
        t->anim = 0xFFFF;
        t->frame = -1;
        t->sound[1] = 0xFFFF;
        t->sound[0] = 0xFFFF;
    }
    sc->soundBankSlot = 0;
    sc->modelQueueCount = 0;
    sc->unk62 = 0;
    sc->unk64 = 0;
    for (i = 0; i >= 0; i--) {
        sc->unk61[i] = 0;
    }
    for (i = 3; i >= 0; i--) {
        sc->unk68[i] = 0;
    }
}

void fld_refindSectors(void) {
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
    if (f->flags & 0x10000) {
        gs2 = gs;
        gd2 = gd;
        g = gs2->unk14;
        t = g->unk8;
        base = D_80067944 + ((t << 11) + 0x14C700);
        start = (FieldDirEnt *)(base + ((g->unk48 - t) << 11));
        e = &start[g->unk44] - 1;
        key = f->area->scene->nextId;
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
        g_gameState->unk1C->unk8E4.field->area->areaId = id;
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
        fld_clearModelCache();
        func_80022280((s32)g_gameState->unk1C->unk854[0].unk8, g_gameState->unk1C->unk854[0].unk10);
    }
}

s32 fld_findEventScript(FieldScene *arg0, u16 arg1) {
    arg0->eventScript = db_lookupRegistered(4, arg1 | DB_FIELD, 0);
    return 0;
}

s32 fld_loadVibration(FieldScene *arg0, u16 arg1) {
    void *p = db_lookupRegistered(4, arg1 | 0x1E0000, 0);

    arg0->vibration = p;
    if (p != NULL) {
        func_80054A28(p);
    }
    return 0;
}

s32 fld_setupBackground(FieldScene *arg0, s32 arg1) {
    GameData *gd = g_gameState->unk1C;
    s32 id;

    fldbg_setRenderContext(&gd->unk8, &gd->unkA, gd->unk10[0], gd->unk10[1], &gd->unk18, D_80067948 + 0x400);
    fldbg_setWorkBuffer(D_80067948, 0x400);
    id = fld_getBgKey(arg0, arg1);
    arg0->bg = db_lookupRegistered(4, id, 0);
    fldbg_setData(arg0->bg);
    D_80067948 = fldbg_init();
    bgi_clear();
    arg0->walkmesh = db_lookupRegistered(4, (s32)db_lookupRegistered(9, id, DB_WALKMESH), 0);
    bgi_init((u8 *)arg0->walkmesh + 4);
    D_80067948 = fld_initSprites(D_80067948);
    return 0;
}

s32 fld_findLights(FieldScene *arg0) {
    arg0->lights = db_lookupRegistered(4, (u16)fld_getBgKey(arg0) | DB_LIGHTS, 0);
    return 0;
}

void fld_startAudio(void) {
    while (fld_soundCommand(0xC00, -1, 0, 0, 0) != 0) {
    }
    if (g_gameState->unk1C->unk0 & 0x400000) {
        fld_soundCommand(0x100, g_gameState->unk1C->unk81C[0], 0, 0, 0);
        g_gameState->unk1C->unk0 &= ~0x400000;
    }
    if (g_gameState->unk1C->unk8E4.field->soundBankSize != -1) {
        func_80055DF4();
        func_80055E14((s32)D_80067944 + 0x146F00, g_gameState->unk1C->unk8E4.field->soundBankSize, 1);
        g_gameState->unk1C->unk8E4.field->soundBankSize = -1;
    }
}

s32 fld_loadModelBundlesInCdOrder(void) {
    s32 best;
    s32 i;
    s32 min;
    s32 v;

    if (g_gameState->unk1C->unk8E4.field->area->scene->modelQueueCount != 0) {
        while (1) {
            best = -1;
            min = 0x7FFFFFFF;
            for (i = g_gameState->unk1C->unk8E4.field->area->scene->modelQueueCount - 1; i >= 0; i--) {
                if (g_fieldModelQueue[i] != -1) {
                    v = file_find(0xA, g_fieldModelQueue[i])->unk4;
                    if (v < min) {
                        best = i;
                        min = v;
                    }
                }
            }
            if (best == -1) {
                break;
            }
            fld_loadModelBundle(g_fieldModelQueue[best]);
            g_fieldModelQueue[best] = -1;
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
    id = (u16)arg1 | DB_FIELD;
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
    fld_invalidateModelCache((s32)g_gameState->unk1C->unk854[0].unk20);
    base = D_80067948;
    g_gameState->unk1C->depTable = (FileTable *)db_lookup(base, 4, DB_DEP_TABLE, 0);
    id = db_lookup(base, 2, DB_PACK, 0);
    if (id != 0) {
        r = db_lookup(base, 5, DB_PACK, 0);
        if ((u32)(D_80067948 + g_gameState->unk1C->unk854[0].unk10) < (u32)(id + r)) {
            rest = g_gameState->unk1C->unk854[0].unk10;
            func_8002177C(g_gameState->unk1C->unk854, g_gameState->unk1C->sceneSector[1], g_gameState->unk1C->sceneSectors[1] << 11,
                          (void *)(D_80067948 + rest), rest, size - g_gameState->unk1C->unk854[0].unk10, NULL);
            while (func_800217C4(g_gameState->unk1C->unk854) != 0) {
            }
            g_gameState->unk1C->unk854[0].unk10 += rest;
            fld_invalidateModelCache((s32)g_gameState->unk1C->unk854[0].unk20);
        }
        for (i = db_lookup(id, 1, DB_TIM, 0) - 1; i >= 0; i--) {
            k = db_lookup(id, 2, (u16)i | DB_TIM, 0);
            if (k != -1) {
                func_8001D8A8(k);
            }
        }
        for (i = db_lookup(id, 1, DB_IMAGE_PACK, 0) - 1; i >= 0; i--) {
            k = db_lookup(id, 2, (u16)i | DB_IMAGE_PACK, 0);
            if (k != -1) {
                fld_loadImagePack((s32 *)k);
            }
        }
        DrawSync(0);
    }
    sc->fieldFile = (void *)db_lookup(base, 2, (DB_PACK | 1), 0);
    size = g_gameState->unk1C->unk854[0].unk10 - ((s32)sc->fieldFile - base);
    func_8001DAA4(id, (s32)sc->fieldFile, size, NULL);
    DrawSync(0);
    sc->fieldFile = (void *)id;
    if (g_gameState->unk1C->unk854[0].unk10 < g_gameState->unk1C->unk854[0].unk4) {
        func_8002177C(g_gameState->unk1C->unk854, g_gameState->unk1C->sceneSector[1], g_gameState->unk1C->sceneSectors[1] << 11,
                      (void *)(id + size), g_gameState->unk1C->unk854[0].unk10, -1, NULL);
        while (func_800217C4(g_gameState->unk1C->unk854) != 0) {
        }
    }
    db_register((s32)sc->fieldFile, 0);
    D_80067948 = (s32)sc->fieldFile + func_8001CA70((s32)sc->fieldFile);
    return 0;
}

s32 fld_getShadowTexPos(u16 *arg0, u16 *arg1) {
    Unk800AA01C *p = db_lookupRegistered(4, 0x1C0000, 0);

    *arg0 = p->unk30;
    *arg1 = p->unk32;
    return 0;
}

s32 fld_freeFieldFile(FieldScene *arg0, s16 arg1) {
    db_unregister(arg0->fieldFile);
    arg0->fieldFile = NULL;
    return 0;
}

s32 fld_freeVibration(FieldScene *arg0, s16 arg1) {
    func_80054F80();
    return 0;
}

/* old-style definition: some callers pass a second argument */
s32 fld_getBgKey(arg0)
    FieldScene *arg0;
{
    return db_lookup((s32)arg0->fieldFile, 3, DB_BG, 0);
}

s32 fld_depFindBattle(u16 arg0, s32 *arg1, s32 *arg2) {
    s32 i;
    FileTable *t = g_gameState->unk1C->depTable;
    s32 id = arg0 | DB_BATTLE;
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
    s32 id = arg0 | DB_FIELD;
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
