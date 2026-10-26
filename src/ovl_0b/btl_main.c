#include "ovl_0b.h"

void btl_entry(void) {
    btl_initMode();
    do {
        btl_main((s16)g_gameState->gameData->battleId);
    } while (!(g_gameState->unk0 & 0x101));
    btl_exitMode();
}

void btl_main(s32 arg0) {
    btl_load(arg0);
    SetDispMask(1);
    do {
        btl_beginFrame();
        if (!(g_gameState->gameData->unk8E4.battle->flags & 0x1000)) {
            btl_checkDiscCover();
            if (btlui_isPauseRequested() != 0) {
                btl_updatePause();
            }
            if (!(g_gameState->gameData->unk0 & 0x100)) {
                if (btl_update() == 1) {
                    g_gameState->gameData->unk8E4.battle->flags |= 0x1000;
                }
                if (!(g_gameState->gameData->unk8E4.battle->flags & 0x10) && (g_gameState->gameData->unk8E4.battle->loadFlags & 7) == 7) {
                    btlui_init();
                    g_gameState->gameData->unk8E4.battle->flags |= 0x10;
                }
                if (!(g_gameState->gameData->unk8E4.battle->flags & 0x40) && (g_gameState->gameData->unk56F & 0x40)) {
                    btl_preloadExit();
                    g_gameState->gameData->unk8E4.battle->flags |= 0x40;
                }
                game_updateTimer();
                game_updateFade();
                preload_update();
            }
        } else {
            g_gameState->mode = 8;
            g_gameState->unk0 |= 0x100;
        }
        btl_endFrame();
    } while (!(g_gameState->unk0 & 0x301));
    btl_cleanup();
}

void btl_updatePause(void) {
    GameData *gd = g_gameState->gameData;
    u16 f = gd->unk8E4.battle->flags;
    s32 i;
    s32 w;

    if (!(f & 0x1000)) {
        if (!(gd->unk0 & 0x100) || (f & 0x20)) {
            gd->unk0 |= 0x100;
            akao_sendCmd99(0);
            game_resumeClock(1);
            for (i = 1; i >= 0; i--) {
                g_padActuator[i].unk1 = 0;
                g_padActuator[i].unk0 = 0;
            }
            if (g_gameState->gameData->unk8E4.battle->flags & 0x20) {
                g_padPortMask[0] |= 8;
            }
            ClearOTag(D_800FDEB0, 1);
            setlen(&D_800FDF18, 2);
            D_800FDF18.code[0] = GPU_STP(0, 0);
            D_800FDF18.code[1] = 0;
            addPrim(D_800FDEB0, &D_800FDF18);
            if (g_gameState->gameData->unk8E4.battle->flags & 0x20) {
                text_buildSmall(0x66, 0x6D, D_800FDEB0, "Disc Cover Open", 4, (u8 *)g_gameState->gameData->unk8E4.battle->work->prims[g_gameState->gameData->unk8]);
            } else {
                ui_buildSprite((0x140 - g_uiSprites[0xEB].unk4) / 2, (0xDA - g_uiSprites[0xEB].unk5) / 2, D_800FDEB0, 0xEB, 0, &w, D_800FDE60);
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
            D_800FDEA8.code[0] = get_mode(0, 1, getTPage(0, 2, 0, 0));
            addPrim(D_800FDEB0, &D_800FDEA8);
            D_800FDF24.code[0] = GPU_STP(1, 0);
            setlen(&D_800FDF24, 2);
            D_800FDF24.code[1] = 0;
            addPrim(D_800FDEB0, &D_800FDF24);
            DrawOTag(D_800FDEB0);
            g_gameState->gameData->unk0 |= 0x20;
            btlui_setPauseRequest(0);
            return;
        }
    } else if (f & 0x20) {
        return;
    }
    game_resumeClock(0);
    akao_sendCmd98(0);
    g_padPortMask[0] &= ~8;
    func_800EA34C();
    g_gameState->gameData->unk0 &= ~0x20;
    btlui_setPauseRequest(0);
    g_gameState->gameData->unk0 &= ~0x100;
}

void btl_checkDiscCover(void) {
    volatile u8 a;
    volatile s32 b;
    u16 f;
    a = cd_isShellOpen();
    f = g_gameState->gameData->unk8E4.battle->flags;
    if (f & 0x10) {
        if (!(f & 0x20)) {
            if (a != 0) {
                btlui_setPauseRequest(1);
                g_gameState->gameData->unk8E4.battle->flags |= 0x20;
            }
        } else {
            b = cd_getState();
            if (b == 0) {
                btlui_setPauseRequest(1);
                g_gameState->gameData->unk8E4.battle->flags &= ~0x20;
            }
        }
    }
}

void btl_initMode(void) {
    GameData *g = g_gameState->gameData;
    Battle *b;
    btl_allocBattle();
    b = g->unk8E4.battle;
    btl_clearBattle();
    g_gameState->vsyncMode = 4;
    game_setScreenHeight(0xDA);
    game_freezeScreen();
    SetDispMask(1);
    g->unk838 = btl_registerModel;
    g->unk83C = btl_soundCmd;
    g_gameState->gameData->unk8BC = func_800AF1C8;
    preload_init();
    g->unk39 = 0;
    g->unk38 = 0;
    g->unk84C = 0;
    b->dataSector = g_gameState->imgSector + g_gameState->dirs->unkDC;
    btl_loadCommonData();
    func_800A8F10();
    func_800A8F18();
    func_800A8F8C();
    func_800A902C();
    func_800A9090();
    btl_initBattleState();
    b->unk4 = D_80067948;
}

void btl_exitMode(void) {
    s32 i;
    for (i = 1; i >= 0; i--) {
        g_padActuator[i].unk1 = 0;
        g_padActuator[i].unk0 = 0;
    }
    btl_unloadCommonData();
}

void btl_allocBattle(void) {
    BattleMem *m;
    D_80067948 = g_loadAreaEnd + 0x61800;
    m = (BattleMem *)D_80067948;
    g_gameState->gameData->unk8E4.battle = &m->unk0;
    D_80067948 = (s32)m->unk241C;
    m->unk0.files = (BtlFiles *)m->unk23CC;
    m->unk0.unk1538 = (struct Battle1538 **)m->unk240C;
}

void btl_clearBattle(void) {
    Battle *b = g_gameState->gameData->unk8E4.battle;
    s32 i;
    b->flags = 0;
    b->unk4 = 0;
    for (i = 0; i < 16; i++) {
        *(b->files->data + i) = 0;
    }
    for (i = 0; i < 4; i++) {
        b->unk153C[i] = 0;
        b->unk1538[i] = 0;
    }
    b->unk1530 = 0;
    b->dataSector = -1;
    b->work = 0;
    for (i = 0; (u32)i < 2; i++) {
        b->unk2[i] = 0;
    }
}

s32 func_800A8F10(void) {
    return 0;
}

s32 func_800A8F18(void) {
    s32 a = (s32)db_lookupRegistered(4, 0x70005, 0);
    file_copyViaVram(g_gameState->gameData->unk818, a, (s32)db_lookupRegistered(5, 0x70005, 0), NULL);
    DrawSync(0);
    return 0;
}

s32 func_800A8F8C(void) {
    s32 i;
    for (i = 1; i >= 0; i--) {
        s32 id = D_800F51E8[i] | 0x90000;
        s32 a = (s32)db_lookupRegistered(4, id, 0);
        s32 b = (s32)db_lookupRegistered(5, id, 0);
        akao_beginBankUpload();
        akao_uploadBank(a, b, 1);
    }
    return 0;
}

s32 func_800A902C(void) {
    s32 p = (s32)db_lookupRegistered(4, 0x42774, 0);
    file_setTimPos(p, 0x100, 0xFF, 0x200, 0xE0);
    file_loadTim(p);
    DrawSync(0);
    return 0;
}

s32 func_800A9090(void) {
    func_800E9D84();
    return 0;
}

s32 btl_loadCommonData(void) {
    s32 p = D_80067948;
    Battle *b = g_gameState->gameData->unk8E4.battle;
    FileEntry *f = file_find(1, 0x271B);
    s32 s = f->unk4;
    void *ctx = g_gameState->gameData->unk854;
    cdRead_init(ctx, s, (f->unkC - s) << 11, (void *)p, 0, -1, NULL);
    while (cdRead_update(ctx) != 0) {
    }
    b->files->data[0] = (void *)p;
    db_register(p, 0);
    file_copyViaVram((s32)b->statusInfo, (s32)db_lookupRegistered(4, 0x1F4, 0), 0x180, NULL);
    file_copyViaVram((s32)b->abilities, (s32)db_lookupRegistered(4, 0x1F5, 0), 0xC00, NULL);
    file_copyViaVram((s32)b->statusSets, (s32)db_lookupRegistered(4, 0x1F6, 0), 0x100, NULL);
    DrawSync(0);
    D_80067948 = db_lookup(p, 4, 0x1E, 0);
    return 0;
}

s32 btl_unloadCommonData(void) {
    BtlFiles *p = g_gameState->gameData->unk8E4.battle->files;
    db_unregister(p->data[0]);
    p->data[0] = NULL;
    return 0;
}

void btl_load(s32 arg0) {
    Unk800AA08C st;
    Battle11C0 *p;
    s32 i;

    D_80067948 = g_gameState->gameData->unk8E4.battle->unk4;
    btl_allocBuffers();
    p = g_gameState->gameData->unk8E4.battle->work;
    btl_initReturnInfo();
    g_gameState->unk0 &= ~0x30F;
    game_setClearColor(0, 0, 0);
    g_gameState->gameData->battleId = arg0;
    btl_loadEncounterFile((Unk800AA08C *)p, arg0);
    btl_initEncounter((Unk800AA08C *)p, arg0);
    btl_loadBackgroundStep(p, -1, &st);
    do {
    } while (btl_loadBackgroundStep(p, arg0, &st) != 0);
    btl_setupBackground(p, arg0);
    g_gameState->gameData->unk38 = func_8004AAF0(0);
    g_gameState->gameData->unk8DC = (struct Unk8D4 *)(p->weaponBufs + 4);
    g_gameState->gameData->unk8E0 = (struct Unk8E0 *)&g_gameState->gameData->unk8DC[g_gameState->gameData->unk38];
    D_80067948 = (s32)&g_gameState->gameData->unk8E0[g_gameState->gameData->unk38];
    g_gameState->gameData->unk9 = 0;
    for (i = 0; i < 9; i++) {
        func_8001E3A4(&g_gameState->gameData->unk8D8[i]);
        g_gameState->gameData->unk8D8[i].unk8 = (struct BtlUnit9C *)&g_gameState->gameData->unk8D4[i];
    }
    g_gameState->gameData->unk39 = 0;
    for (i = 0; i < g_gameState->gameData->unk38; i++) {
        func_8001E3A4(&g_gameState->gameData->unk8E0[i]);
        g_gameState->gameData->unk8E0[i].unk8 = (struct BtlUnit9C *)&g_gameState->gameData->unk8DC[i];
    }
    p->shadows = (struct BtlShadow *)D_80067948;
    D_80067948 = func_800AD4AC(D_80067948, g_gameState->gameData->unk38 + 9);
    p->unk8 = D_80067948;
    btlui_reset();
    btl_startBattle();
}

void btl_cleanup(void) {
    Battle11C0 *s;
    s32 i;
    s32 v;
    s32 w;

    s = g_gameState->gameData->unk8E4.battle->work;
    DrawSync(0);
    btlui_exit();
    vib_clear();
    btl_unloadPartyModels();
    btl_unregisterFiles(&g_gameState->gameData->unk8E4.battle->files->enemyModels[0], 2);
    btl_unregisterFiles(&g_gameState->gameData->unk8E4.battle->files->data[3], 0);
    btl_unregisterFiles(&g_gameState->gameData->unk8E4.battle->files->data[2], 0);
    for (i = 0; i < 9; i++) {
        if (g_gameState->gameData->unk8D8[i].unk0 & 1) {
            g_gameState->gameData->unk8D8[i].unk0 &= ~1;
        }
    }
    for (i = 0; i < g_gameState->gameData->unk38; i++) {
        if (g_gameState->gameData->unk8D8[i].unk0 & 1) {
            g_gameState->gameData->unk8D8[i].unk0 &= ~1;
        }
    }
    switch (s->returnMode) {
    case 1:
    case 3:
    case 5:
        for (i = 1; i >= 0; i--) {
            w = g_gameState->gameData->returnSector[i];
            g_gameState->gameData->sceneSector[i] = w;
            v = g_gameState->gameData->returnSectors[i];
            g_gameState->gameData->sceneSectors[i] = v;
        }
        if (s->returnMode == 3) {
            g_gameState->gameData->unk7DC = s->returnId;
        } else {
            g_gameState->gameData->fieldId = s->returnId;
        }
        break;
    case 8:
        break;
    default:
        SystemError(0x39, 0x12E);
        break;
    }
}

void btl_allocBuffers(void) {
    Battle11C0 *p;
    s32 i;
    g_gameState->gameData->unk8E4.battle->work = (Battle11C0 *)D_80067948;
    p = (Battle11C0 *)D_80067948;
    D_80067948 = (s32)(p + 1);
    p->sceneTables = (u16 *)((s32)g_gameState->gameData->unk8E4.battle + 0x5800);
    p->bgData = (s32)p->sceneTables + 0x2800;
    p->heapStart = (u8 *)(p->bgData + 0x6000);
    D_80067948 = (s32)p->heapStart + 0x35800;
    for (i = 0; i < 4; i++) {
        g_gameState->gameData->unk8E4.battle->unk153C[i] = D_80067948;
        D_80067948 += 0x13000;
        g_gameState->gameData->unk8E4.battle->unk1538[i] = (Battle1538 *)(D_80067948 - 0x28);
    }
    p->weaponBufs = (s32 *)(p + 1);
    for (i = 0; i < 4; i++) {
        p->weaponBufs[i] = D_80067948;
        D_80067948 += 0x1000;
    }
    p->ot[1] = D_80067948 + 0x400;
    p->ot[0] = D_80067948;
    p->prims[0] = D_80067948 + 0x800;
    p->prims[1] = p->prims[0] + 0x20000;
    D_80067948 = p->prims[1] + 0x20000;
    p->loadBuf = (s32 *)(g_loadAreaEnd + 0x136700);
}

void btl_initReturnInfo(void) {
    Battle11C0 *p = g_gameState->gameData->unk8E4.battle->work;
    s32 i;
    p->unk0 = 0;
    p->unk8 = 0;
    p->returnMode = g_gameState->unkA;
    if (g_gameState->unkA == 1) {
        p->returnId = g_gameState->gameData->fieldId;
    } else if (g_gameState->unkA == 3) {
        p->returnId = g_gameState->gameData->unk7DC;
    } else {
        p->returnId = -1;
    }
    p->encounter = NULL;
    p->unk10 = 0;
    p->texts = NULL;
    p->bgHeader = 0;
    p->bgTexAnims = NULL;
    p->shadows = NULL;
    p->heap = p->heapStart;
    for (i = 0; i < sizeof(p->unk7); i++) {
        p->unk7[i] = 0;
    }
    for (i = 0; i < 4; i++) {
        g_gameState->gameData->unk8E4.battle->unk1538[i]->unk20 = g_gameState->gameData->unk8E4.battle->unk1538[i]->unk24 = g_gameState->gameData->unk8E4.battle->unk153C[i];
    }
}

s32 btl_initEncounter(Unk800AA08C *arg0, s32 arg1) {
    s32 id = (arg1 & 0xFFFF) | 0x110000;
    arg0->unk24 = (s32)db_lookupRegistered(4, id, 0);
    arg0->unk20 = (s32)db_lookupRegistered(4, (s32)db_lookupRegistered(9, id, 0x100000), 0);
    arg0->unk10 = (s32)db_lookupRegistered(4, (s32)db_lookupRegistered(9, id, 0x50000), 0);
    arg0->unk14 = (s32)db_lookupRegistered(4, (s32)db_lookupRegistered(9, id, 0x60000), 0);
    btl_initScene();
    evt_init((u8 *)arg0->unk10);
    msg_init();
    return 0;
}

s32 btl_setupBackground(Battle11C0 *arg0, s32 arg1) {
    s32 id = btl_findBackgroundId();
    s32 i;
    RECT r;
    arg0->bgData = (s32)db_lookupRegistered(4, id, 0);
    arg0->bgHeader = (s32)db_lookupRegistered(4, (s32)db_lookupRegistered(9, id, 0x1A0000), 0);
    if (db_lookupRegistered(0xB, id, 0x190000) != 0) {
        arg0->bgTexAnims = db_lookupRegistered(4, (s32)db_lookupRegistered(9, id, 0x190000), 0);
    }
    for (i = 0; i < (s32)db_lookupRegistered(0xB, id, 0x40000); i++) {
        file_loadTim((s32)db_lookupRegistered(4, (s32)db_lookupRegistered(9, id, (i & 0xFFFF) | 0x40000), 0));
    }
    if (arg0->bgTexAnims != NULL) {
        r.y = 0;
        r.x = 0;
        r.h = 0;
        r.w = 0;
        btl_relocTexAnims((TexAnimSet *)arg0->bgTexAnims, NULL);
    }
    DrawSync(0);
    g_gameState->gameData->unk8E4.battle->flags |= 1;
    return 0;
}

s32 btl_reloadPartyStep(s32 arg0) {
    Battle *b = g_gameState->gameData->unk8E4.battle;
    s16 *state = (s16 *)&b->work->loadBuf[0x9FD8 / 4];
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
        b->flags &= ~4;
        *state = 1;
    }
    switch (*state) {
    case 1:
        if (arg0 & 0x200) {
            btl_loadSoundsStep(g_gameState->gameData->battleId, 0x8000, (Unk800AA24C *)ld);
            *state = 2;
            break;
        }
        *state = 3;
        break;
    case 2:
        if (btl_loadSoundsStep(g_gameState->gameData->battleId, arg0, (Unk800AA24C *)ld) == 0) {
            *state = 3;
        }
        break;
    case 3:
        btl_loadWeaponModelsStep(0x8000, ld);
        *state = 4;
        break;
    case 4:
        if (btl_loadWeaponModelsStep(arg0, ld) == 0) {
            *state = 5;
        }
        break;
    case 5:
        btl_loadPartyModelsStep(0x8000, ld);
        *state = 6;
        break;
    case 6:
        if (btl_loadPartyModelsStep(arg0, ld) == 0) {
            if (arg0 & 4) {
                *state = 7;
            } else {
                goto done;
            }
        }
        break;
    case 7:
        for (i = 0; i < 4; i++) {
            ch = g_gameState->gameData->party[i];
            if (ch == NULL) {
                continue;
            }
            cid = ch->unk34.b.id;
            e = &g_gameState->gameData->unk8D8[cid];
            u = btl_getUnitBySlot(i);
            if (u == NULL) {
                continue;
            }
            id = btl_getPartyModelId(i, arg0 & ~0x10);
            e->unk8->header = db_lookupRegistered(4, id, 0);
            k = (id & 0xFFFFFF) | (cid << 24);
            pa = (s32 *)func_800634A8(k);
            pb = (s32 *)func_800634D8(k);
            pc = (s32 *)func_80063514(k);
            btl_applyModelTextures(e->unk8, (s16 *)pa, (s16 *)pb);
            p = (s32)db_lookupRegistered(9, id, 0x190000);
            if (p != -1) {
                v = (s32)db_lookupRegistered(4, p, 0);
            } else {
                v = 0;
            }
            e->unk8->texAnims = (BtlUnit9C50 *)v;
            if (v != 0) {
                btl_relocTexAnims((TexAnimSet *)e->unk8->texAnims, (u16 (*)[4])pc);
            }
            btl_loadPartyMotions(u, g_gameState->gameData->party[i]->unk34.b.unk4, u - g_gameState->gameData->unk8E4.battle->units);
            if (u->status & BTL_STATUS_KO) {
                if (u != g_gameState->gameData->unk8E4.battle->curCmd->caster) {
                    u->model->motion = (BtlUnit9CC *)u->motions[4];
                } else {
                    goto use9;
                }
            } else if (u == g_gameState->gameData->unk8E4.battle->curCmd->caster || ((u->flags >> 19) & 1)) {
            use9:
                u->model->motion = (BtlUnit9CC *)u->motions[9];
            } else {
                u->model->motion = (BtlUnit9CC *)u->motions[(u->flags >> 18) & 1];
            }
            e->unkC->frame = 0;
            m = u->weaponModel;
            save = m->parentBone;
            id2 = ((Unk8006E0E8 *)u->weapon)->unk2 | 0x20000;
            m->header = (BtlUnit9C8 *)btl_getWeaponModelHeader(i);
            btl_getWeaponTexInfo(id2, i, &pa, &pb, &pc);
            btl_applyModelTextures(m, (s16 *)pa, (s16 *)pb);
            m->motion = 0;
            m->parent = e->unk8;
            m->parentBone = save;
        }
    done:
        g_gameState->gameData->unk8E4.battle->flags |= 4;
        *state = 0;
        break;
    }
    return *state;
}

s32 btl_loadEncounterFile(Unk800AA08C *arg0, s32 arg1) {
    s32 v = arg0->unk24;
    GameData *gd = g_gameState->gameData;
    u8 *cd = gd->unk854;
    Battle *b = gd->unk8E4.battle;
    cdRead_init(cd, gd->sceneSector[0], gd->sceneSectors[0] << 11, (void *)arg0->unk24, 0, 0x2800, 0);
    while (cdRead_update(cd) != 0) {
    }
    b->files->data[2] = (void *)v;
    db_register(v, 0);
    gd->depTable = (FileTable *)db_lookup(v, 2, 0x140000, 0);
    return 0;
}

s16 btl_loadBackgroundStep(Battle11C0 *arg0, s32 arg1, Unk800AA08C *arg2) {
    GameData *gd = g_gameState->gameData;
    Battle *b = gd->unk8E4.battle;
    u8 *cd = gd->unk854;
    Unk800AA08C *st;
    s32 v;
    GameData *gd2;
    st = arg2;
    if (arg1 == -1) {
        b->flags &= ~1;
        st->unk0 = 2;
    }
    if (st->unk0 == 1) {
        if (cdRead_update(cd) == 0) {
            st->unk0 = st->unk2;
        }
    } else if (st->unk0 == 2) {
        gd2 = g_gameState->gameData;
        st->unk24 = arg0->bgData;
        cdRead_init(cd, gd2->sceneSector[1], gd2->sceneSectors[1] << 11, (void *)st->unk24, 0, -1, 0);
        st->unk2 = 4;
        st->unk0 = 1;
    } else if (st->unk0 == 4) {
        v = arg0->bgData;
        b->files->data[3] = (void *)v;
        db_register(v, 0);
        b->flags |= 1;
        st->unk0 = 0;
    }
    return st->unk0;
}

s32 btl_loadSoundsStep(s16 arg0, s32 arg1, Unk800AA24C *arg2) {
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
    gd = g_gameState->gameData;
    b = gd->unk8E4.battle;
    cd = gd->unk854;
    if (arg1 & 0x8000) {
        b->flags &= ~2;
        st->unk0 = 2;
    }
loop:
    switch (st->unk0) {
    case 1:
        if (cdRead_update(cd) != 0) {
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
            btl_loadSoundsStep_collect(&arg2->unk8.h[4], 9, 2);
        }
        st->unk0 = 5;
        goto loop;
    case 5:
        hb = arg2->unk8.h;
        n = btl_findNearestFile(0xB, (u16 *)hb, (u16 *)&arg2->unk8.h[6], (u16 **)&p, &sz);
        if (n != -1) {
            cdRead_init(cd, n, sz << 11, (void *)arg2->unk24, 0, -1, 0);
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
        if (akao_getSpuTransferState() != 0) {
            break;
        }
        h = db_lookup((s32)cd->unk8, 2, 0x90000, 0);
        if (arg2->unk4 == -1) {
            akao_loadBankRetry(h, 0);
        } else {
            func_80056048(h, arg2->unk4 & 3, 0);
        }
        st->unk0 = 5;
        goto loop;
    case 7:
        if (akao_getSpuTransferState() != 0) {
            break;
        }
        st->unk0 = 0;
        break;
    }
    return st->unk0;
}

s32 btl_loadEnemyModelsStep(s32 arg0, s32 arg1, Unk800AA504 *arg2) {
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
    b = g_gameState->gameData->unk8E4.battle;
    req = (Unk800AA958Req *)&b->work->loadBuf[0x9FA0 / 4];
    if (arg1 & 0x8000) {
        b->flags &= ~2;
        st->unk0 = 2;
    }
loop:
    switch (st->unk0) {
    case 1:
        if (cd_getState() != 0) {
            break;
        }
        st->unk0 = arg2->unk2;
        goto loop;
    case 2:
        t = g_gameState->gameData->depTable;
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
                f = dep_find(g_gameState->gameData->depTable, id | 0x20000);
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
            req->unk4 = (s32)b->work->heap;
            req->unk8 = (bf->unk8 - bf->unkC) << 11;
            req->unkC = arg2->unkC;
            cd_readChunks(g_gameState->imgSector + best, req, 2, 0);
            arg2->unk2 = 6;
            st->unk0 = 1;
        } else {
            st->unk0 = 7;
        }
        break;
    case 6:
        sp1C = db_lookup((s32)b->work->heap, 2, 0x1B0000, 0);
        dst = arg2->unkC;
        tbl = (Unk800AA504Tbl *)db_lookup((s32)b->files->data[2], 2, 0x120000, 0);
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
                    x = db_lookup(dst, 2, (n & 0xFFFF) | 0x40000, 0);
                    h = clut_getX(a[n].unk2);
                    file_setTimPos(x, h, clut_getY(a[n].unk2), r[n].unk0, r[n].unk2);
                    file_loadTim(x);
                }
                break;
            }
            te--;
        } while (te >= tf);
        D_80067948 = sp1C + db_getSize(sp1C);
        b->work->heap = (u8 *)D_80067948;
        arg2->unkC += db_getSize(dst);
        *(b->files->enemyModels + arg2->unkA) = (void *)sp1C;
        db_register(sp1C, 0);
        arg2->unkA++;
        st->unk0 = 4;
        goto loop;
    case 7:
        b->flags |= 2;
        st->unk0 = 0;
        break;
    }
    return st->unk0;
}

s32 btl_loadWeaponModelsStep(s32 arg0, Unk800AA958 *arg1) {
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
    b = g_gameState->gameData->unk8E4.battle;
    req = (Unk800AA958Req *)&b->work->loadBuf[0x9FA0 / 4];
    if (arg0 & 0x8000) {
        b->flags &= ~4;
        st->unk0 = 2;
    }
loop:
    switch (st->unk0) {
        case 1:
            if (cd_getState() != 0) {
                goto end;
            }
            st->unk0 = arg1->unk2;
            goto loop;
        case 2:
            arg1->unk10.w[0] = -1;
            arg1->unk10.w[1] = -1;
            gd = g_gameState->gameData;
            e = gd->party;
            c = &gd->party[3];
            q = &arg1->unk10.h[3];
            do {
                if (*c != NULL) {
                    *q = g_weaponTable[(*c)->equip[0]].unk2;
                }
                c--;
                q--;
            } while (c >= e);
            st->unk0 = 5;
            goto loop;
        case 5:
            n = btl_findNearestFile(8, arg1->unk10.h, &arg1->unk10.h[3], &p, &sz);
            if (n != -1) {
                *(s16 *)p = -1;
                i = p - arg1->unk10.h;
                arg1->unkA = i;
                arg1->unk24 = b->work->weaponBufs[i];
                req->unk0 = 0x1000;
                req->unk4 = arg1->unk24;
                req->unk8 = (sz << 11) - 0x1000;
                req->unkC = b->unk153C[i];
                cd_readChunks(g_gameState->imgSector + n, req, 2, 0);
                arg1->unk2 = 7;
                st->unk0 = 1;
            } else {
                st->unk0 = 8;
            }
            goto end;
        case 7:
            j = arg1->unkA;
            v = btl_uploadModelTextures(arg1, req, j, 0);
            pp = &b->files->weaponFiles[j];
            *pp = (void *)v;
            db_register(v, 0);
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

s32 btl_loadPartyModelsStep(s32 arg0, Unk800AA958 *arg1) {
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
    b = g_gameState->gameData->unk8E4.battle;
    req = (Unk800AA958Req *)&b->work->loadBuf[0x9FA0 / 4];
    if (arg0 & 0x8000) {
        b->flags &= ~4;
        st->unk0 = 1;
    }
loop:
    switch (st->unk0) {
    case 2:
        if (cd_getState() != 0) {
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
            *q = btl_getPartyModelId(q - h, arg0 & ~0x10);
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
        n = btl_findNearestFile(0xA, (u16 *)h, (u16 *)q, (u16 **)&p, &sz);
        if (n != -1) {
            i = -1;
            f = (FileEntry16 *)file_find(0xA, *p);
            p[0] = i;
            p[4] = i;
            arg1->unkA = p - h;
            arg1->unk24 = b->unk153C[arg1->unkA];
            req->unkC = 0;
            if (btl_findNearestFile(0xA, (u16 *)h, (u16 *)q, (u16 **)&p2, &sz2) != i) {
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
                req->unkC = (s32)b->work->loadBuf;
            }
            req->unk0 = f->unk2 << 11;
            req->unk4 = arg1->unk24;
            req->unk8 = (sz - f->unk2) << 11;
            cd_readChunks(g_gameState->imgSector + n, req, 2, 0);
            arg1->unk2 = 7;
            st->unk0 = 2;
        } else {
            st->unk0 = 8;
        }
        goto end;
    case 7:
        j = arg1->unkA;
        v = btl_uploadModelTextures(arg1, req, j, 1);
        pp = &g_gameState->gameData->unk8E4.battle->files->partyModels[j];
        *pp = (void *)v;
        db_register(v, 0);
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

u32 btl_findNearestFile(u32 arg0, u16 *arg1, u16 *arg2, u16 **arg3, s32 *arg4) {
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
            f = file_find(arg0, id);
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
                f = dep_find(g_gameState->gameData->depTable, (k << 16) | id);
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

s32 btl_uploadModelTextures(Unk800AA958 *arg0, Unk800AA958Req *arg1, s32 arg2, s32 arg3) {
    s32 sp18;
    s32 dst;
    Unk800AA504Tbl *tbl;
    Unk800AA504Ent *te;
    s32 n;
    Unk800AA504Pos *a;
    Unk800AA504Pos *b;
    Unk800AA504Pos8 *r;
    s32 x;
    s32 y;
    s32 yy;
    s32 h;
    s32 m;

    sp18 = db_lookup(arg0->unk24, 2, 0x1B0000, 0);
    dst = arg1->unkC;
    tbl = (Unk800AA504Tbl *)db_lookup(sp18, 2, 0x120000, 0);
    te = tbl->unk4;
    n = te->unk4.b[3];
    a = (Unk800AA504Pos *)((u8 *)te + (te->unk4.w & 0xFFFFFF));
    b = a + n;
    r = (Unk800AA504Pos8 *)(b + n);
    if (n > 0) {
        do {
            n--;
            x = arg2 * 64 + 0xC0;
            y = arg2 * 3;
            m = db_lookup(dst, 2, (n & 0xFFFF) | 0x40000, 0);
            if (arg3) {
                yy = y + 0xE5 + r[n].unk2;
                h = (r[n].unk2 << 7) + 0x80;
            } else {
                yy = y + 0xE8;
                h = 0x1D0;
            }
            file_setTimPos(m, 0x100, yy, x, h);
            file_loadTim(m);
            a[n].unk0 = getTPage(1, 0, x, h);
            a[n].unk2 = getClut(0x100, yy);
            b[n].unk0 = 0;
            b[n].unk2 = h % 256 - h % 128;
            r[n].unk4 = x;
            r[n].unk0 = x;
            r[n].unk6 = h;
            r[n].unk2 = h;
        } while (n > 0);
    }
    return sp18;
}

s32 btl_unregisterFiles(void **arg0, s32 arg1) {
    void **p = &arg0[arg1];
    do {
        if (*p != NULL) {
            db_unregister(*p);
        }
        *p = NULL;
        p--;
    } while (p >= arg0);
    return 0;
}

s32 btl_findBackgroundId(void) {
    FileTable *t = g_gameState->gameData->depTable;
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

s32 btl_getDepSectors(s32 arg0, s32 *arg1, s32 *arg2) {
    FileEntry *e = dep_find(g_gameState->gameData->depTable, arg0);
    *arg1 = e->unk4;
    *arg2 = e->unk8;
    return arg0;
}

void btl_getLinkedDepSectors(s32 arg0, s32 *arg1, s32 *arg2) {
    btl_getDepSectors(dep_find(g_gameState->gameData->depTable, arg0)->unkC, arg1, arg2);
}

s32 func_800AB3E4(s32 arg0, s32 arg1) {
    s32 v = 0x17;
    if (arg1 != 0) {
        v = 0x8017;
    }
    return btl_swapPartyModelStep(arg0, v);
}

s32 btl_reloadParty(s32 arg1) {
    s32 v = 0x207;
    if (arg1 != 0) {
        v = 0x8207;
    }
    return btl_reloadPartyStep(v);
}

void btl_unloadPartyModels(void) {
    Battle *b = g_gameState->gameData->unk8E4.battle;
    BtlFiles *p = b->files;
    btl_unregisterFiles(p->weaponFiles, 3);
    btl_unregisterFiles(p->partyModels, 3);
    btl_unregisterFiles(g_gameState->gameData->unk8E4.battle->files->partyModels, 3);
    btl_unregisterFiles(g_gameState->gameData->unk8E4.battle->files->weaponFiles, 3);
    b->flags &= ~4;
}

void btl_unloadEnemyModels(void) {
    Battle *b = g_gameState->gameData->unk8E4.battle;
    btl_unregisterFiles(b->files->enemyModels, 2);
    btl_unregisterFiles(g_gameState->gameData->unk8E4.battle->files->enemyModels, 2);
    b->flags &= ~2;
}

s16 btl_loadModelsStep(s32 arg0) {
    GameData *g = g_gameState->gameData;
    s16 *state = (s16 *)&g->unk8E4.battle->work->loadBuf[0x9FD8 / 4];
    s16 *st;
    s32 id = (s16)g->battleId;
    Unk800AA958 *ld;
    s16 s;

    ld = (Unk800AA958 *)state - 1;
    st = state;
    if (arg0 != 0) {
        btl_loadSoundsStep(id, 0x8100, (Unk800AA24C *)ld);
        *state = 1;
    }
    s = *state;
    if (s == 1) {
        if (btl_loadSoundsStep(id, 0x100, (Unk800AA24C *)ld) == 0) {
            btl_loadEnemyModelsStep(id, 0x8000, (Unk800AA504 *)ld);
            (*st)++;
        }
    } else if (s == 2) {
        if (btl_loadEnemyModelsStep(id, 0, (Unk800AA504 *)ld) == 0) {
            btl_loadWeaponModelsStep(0x8000, ld);
            (*st)++;
        }
    } else if (s == 3) {
        if (btl_loadWeaponModelsStep(0x22, ld) == 0) {
            btl_loadPartyModelsStep(0x8020, ld);
            (*st)++;
        }
    } else if (s == 4) {
        if (btl_loadPartyModelsStep(0x22, ld) == 0) {
            g_gameState->gameData->unk8E4.battle->flags |= 4;
            *state = 0;
        }
    }
    return *st;
}

s32 btl_findWeaponModel(s32 arg0, s32 arg1) {
    s32 v = g_gameState->gameData->unk8E4.battle->work->weaponBufs[arg1];
    return db_lookup(db_lookup(v, 2, 0x1B0000, 0), 4, arg0, 0);
}

void btl_getWeaponTexInfo(s32 arg0, s32 arg1, s32 **arg2, s32 **arg3, s32 **arg4) {
    s32 v = g_gameState->gameData->unk8E4.battle->work->weaponBufs[arg1];
    Unk800AB758 *h = (Unk800AB758 *)(db_lookup(db_lookup(v, 2, 0x1B0000, 0), 2, 0x120000, 0) + 4);
    s32 *p = (s32 *)((u8 *)h + h->unk4);
    s32 n = h->unk7;
    *arg2 = p;
    p += n;
    *arg3 = p;
    *arg4 = p + n;
}

u32 btl_registerModel(s32 arg0, s32 arg1) {
    s16 *pa;
    s16 *pb;
    s32 pc;
    u32 n;
    Unk8E0Ld *e;
    BtlUnit9CC *c;
    s32 p;
    u32 i;
    u32 r;
    u32 cnt;

    r = func_8004AB64(arg1);
    if (r < 9) {
        n = r;
        e = (Unk8E0Ld *)game_getModel(r);
    } else {
        n = g_gameState->gameData->unk39 + 9;
        e = (Unk8E0Ld *)game_getModel(n);
        g_gameState->gameData->unk39++;
    }
    e->unk4 = arg0;
    e->unkC = arg1;
    e->unk8->header = (BtlUnit9C8 *)db_lookupRegistered(4, e->unk4, 0);
    e->unk8->motion = (BtlUnit9CC *)db_lookupRegistered(4, btl_getAnimFileId(e->unk4), 0);
    c = e->unk8->motion;
    if (IS_OFFSET(c->unkC)) {
        c->unkC = (s32)c + c->unkC;
        if (c->unk10 != 0) {
            c->unk10 = (s32)e->unk8->motion + c->unk10;
        }
    }
    btl_getModelTexInfo((e->unk4 & 0xFFFFFF) | (n << 24), (s32 **)&pa, (s32 **)&pb, (s32 **)&pc);
    /* c reused for the unk8 block (a second set keeps it a global pseudo, in v1 as in the original) */
    c = (BtlUnit9CC *)e->unk8->header;
    cnt = ((BtlUnit9C8 *)c)->meshCount;
    i = 0;
    if (cnt) {
        /* Probably macro residue: a dead store (deleted by flow, no code). The block boundary it leaves
           until flow keeps combine from folding i = 0 into the loop entry test. */
        p = 5;
    }
    for (; i < cnt; i++) {
    }
    btl_applyModelTextures(e->unk8, pa, pb);
    p = (s32)db_lookupRegistered(9, e->unk4, 0x190000);
    if (p != -1) {
        e->unk8->texAnims = (BtlUnit9C50 *)db_lookupRegistered(4, p, 0);
        btl_relocTexAnims(e->unk8->texAnims, pc);
    }
    e->unk0 |= 1;
    return n;
}

s32 btl_getPartyModelId(s32 arg0, s32 arg1) {
    BtlUnit *u;
    s32 m;
    s32 idx;

    m = 0;
    if (g_gameState->gameData->party[arg0] == NULL) {
        return -1;
    }
    if (!(arg1 & 0x20)) {
        u = btl_getUnitBySlot(arg0);
        if (u != NULL) {
            m = ((s16)u->modelId != g_btlCharModelIds[(u->flags & 1) ? (g_gameState->gameData->characters[(u->flags >> 1) & 0xF].unk34.w >> 4) & 0x1F : 0x13]) << 4;
        }
    }
    idx = g_gameState->gameData->party[arg0]->unk34.w >> 4;
    idx &= 0x1F;
    if ((arg1 & 0x10) != m) {
        if (!(arg1 & 0x20)) {
            idx += 0x13;
        }
    }
    return g_btlCharModelIds[idx] | 0x20000;
}

s32 btl_getWeaponModelHeader(s32 arg0) {
    s32 v = g_gameState->gameData->unk8E4.battle->work->weaponBufs[arg0];
    return db_lookup(db_lookup(v, 2, 0x1B0000, 0), 2, 0x20000, 0);
}

s32 btl_swapPartyModelStep(s32 arg0, s32 arg1) {
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

    st = (Unk800AA958 *)&g_gameState->gameData->unk8E4.battle->work->loadBuf[0x9FD8 / 4];
    state = &st->unk0;
    req = (Unk800AA958Req *)&g_gameState->gameData->unk8E4.battle->work->loadBuf[0x9FA0 / 4];
    if (arg1 & 0x8000) {
        st->unk0 = 1;
    }
    switch (*state) {
    case 1:
        k = arg0;
        v = (void *)g_gameState->gameData->unk8E4.battle->unk153C[k];
        st->unk2 = 0;
        st->unk4 = 0;
        st->unk6 = 0;
        st->unk8 = -1;
        st->unkE = 0;
        st->unk24 = (s32)v;
        st->unk20 = (s32)v;
        if (*(g_gameState->gameData->unk8E4.battle->files->partyModels + k) != NULL) {
            db_unregister(*(g_gameState->gameData->unk8E4.battle->files->partyModels + k));
            *(g_gameState->gameData->unk8E4.battle->files->partyModels + k) = NULL;
        }
        *state = 3;
        break;
    case 2:
        if (cd_getState() == 0) {
            *state = st->unk2;
        }
        break;
    case 3:
        f = (FileEntry16 *)file_find(0xA, btl_getPartyModelId(arg0, arg1 & ~0x10));
        src = g_gameState->imgSector + f->unk4;
        sz = f->unkC - f->unk4;
        req->unk0 = f->unk2 << 11;
        req->unk4 = st->unk24;
        req->unk8 = (sz - f->unk2) << 11;
        req->unkC = (s32)g_gameState->gameData->unk8E4.battle->work->loadBuf;
        cd_readChunks((s32)src, req, 2, 0);
        st->unk2 = 5;
        *state = 2;
        break;
    case 5:
        id = btl_getPartyModelId(arg0, arg1 & ~0x10);
        model = db_lookup(st->unk24, 2, 0x1B0000, 0);
        img = (s32)g_gameState->gameData->unk8E4.battle->work->loadBuf;
        t = (Unk800ABBC4Tbl *)db_lookup(model, 2, 0x120000, 0);
        e = t->unk4;
        for (i = 0; i < t->unk1; i++) {
            if (e[i].unk0 == (u16)id) {
                e = &e[i];
                n = e->unk4.b[3];
                a = (Unk800ABBC4Tex *)((u8 *)e + (e->unk4.w & 0xFFFFFF));
                b = a + n;
                r = (Unk800ABBC4Rect *)(b + n);
                for (j = 0; j < n; j++) {
                    w = db_lookup(img, 2, (j & 0xFFFF) | 0x40000, 0);
                    s = r[j].unk2 + 0xE5;
                    y = arg0 * 3 + s;
                    h = (r[j].unk2 << 7) + 0x80;
                    x = (arg0 << 6) + 0xC0;
                    file_setTimPos(w, 0x100, y, x, h);
                    file_loadTim(w);
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
        *(g_gameState->gameData->unk8E4.battle->files->partyModels + arg0) = (void *)model;
        db_register(model, 0);
        *state = 6;
        break;
    case 6:
        cid = g_gameState->gameData->party[arg0]->unk34.b.id;
        ent = &g_gameState->gameData->unk8D8[cid];
        u = btl_getUnitBySlot(arg0);
        if (u != NULL) {
            id2 = btl_getPartyModelId(arg0, arg1 & ~0x10);
            ent->unk8->header = db_lookupRegistered(4, id2, 0);
            k = (id2 & 0xFFFFFF) | (cid << 24);
            pa = func_800634A8(k);
            pb = func_800634D8(k);
            pc = func_80063514(k);
            btl_applyModelTextures(ent->unk8, (s16 *)pa, (s16 *)pb);
            p = (s32)db_lookupRegistered(9, id2, 0x190000);
            if (p != -1) {
                anim = (s32)db_lookupRegistered(4, p, 0);
            } else {
                anim = 0;
            }
            ent->unk8->texAnims = (BtlUnit9C50 *)anim;
            if (anim != 0) {
                btl_relocTexAnims((TexAnimSet *)ent->unk8->texAnims, (u16 (*)[4])pc);
            }
            btl_loadPartyMotions(u, g_gameState->gameData->party[arg0]->unk34.b.unk4,
                          u - g_gameState->gameData->unk8E4.battle->units);
            if (u->status & BTL_STATUS_KO) {
                u->model->motion = (BtlUnit9CC *)u->motions[4];
            } else {
                u->model->motion = (BtlUnit9CC *)u->motions[(u->flags >> 18) & 1];
            }
            ent->unkC->frame = 0;
            u->weaponModel->parent = ent->unk8;
        }
        *state = 0;
        break;
    }
    return *state;
}

s32 btl_getAnimFileId(s32 arg0) {
    u16 *p = db_findModelInfo(arg0);
    if (p != NULL) {
        return p[1] | 0x30000;
    }
    return -1;
}

s32 btl_getModelTexInfo(u32 arg0, s32 **arg1, s32 **arg2, s32 **arg3) {
    Unk800AB758 *h;
    u8 *dst;
    s32 n;

    h = (Unk800AB758 *)db_findModelInfo(arg0);
    *arg1 = (s32 *)((u8 *)h + h->unk4);
    *arg2 = *arg1 + h->unk7;
    *arg3 = *arg2 + h->unk7;
    if ((s32)(arg0 >> 24) >= 9) {
        n = h->unk7 * 4;
        dst = g_gameState->gameData->unk8E4.battle->work->heap;
        memcpy(dst, (u8 *)*arg1, n);
        *arg1 = (s32 *)dst;
        g_gameState->gameData->unk8E4.battle->work->heap += n;
    }
    return 0;
}

u8 *btl_drawShadow(BtlShadow *arg0, s32 arg1, s32 arg2, s32 arg3, u8 *prim) {
    MATRIX m;
    s32 pad;
    SVECTOR v;
    s32 z[9];
    BtlShadow *e;
    BtlObj *o;
    s32 i;
    s32 k;
    s32 tu;
    s32 tv;

    e = &arg0[arg1];
    o = (BtlObj *)game_getModel(arg1);
    if (o->unk0 & 0x10) {
        return prim;
    }
    btl_calcShadowMatrix((Unk800AD4E4 *)e, o, &m, arg2, arg3 & 0xFF, (arg3 >> 8) & 0xFF);
    for (i = 0; i < 4; i++) {
        SHADOW_QUAD(i) = (POLY_FT4 *)prim;
        setPolyFT4(SHADOW_QUAD(i));
        setRGB0(SHADOW_QUAD(i), e->shade, e->shade, e->shade);
        setSemiTrans(SHADOW_QUAD(i), 1);
        SHADOW_QUAD(i)->tpage = 0xC8;
        SHADOW_QUAD(i)->clut = 0x3FD0;
        prim += sizeof(POLY_FT4);
    }
    tu = 0;
    tv = 0xE0;
    gte_MulMatrix0(&g_gameState->gameData->unk18, &m, &m);
    gte_SetTransMatrix(&g_gameState->gameData->unk18);
    gte_ldlv0(m.t);
    gte_rtv0tr();
    gte_stir(m.t);
    gte_SetRotMatrix(&m);
    gte_SetTransMatrix(&m);
    SHADOW_PT(-0x1000, 0x1000);
    gte_stsxy(&SHADOW_QUAD(0)->x0);
    gte_stsz(&z[0]);
    SHADOW_PT(0, 0x1000);
    gte_stsxy(&SHADOW_QUAD(0)->x1);
    gte_stsxy(&SHADOW_QUAD(1)->x0);
    gte_stsz(&z[1]);
    SHADOW_PT(0x1000, 0x1000);
    gte_stsxy(&SHADOW_QUAD(1)->x1);
    gte_stsz(&z[2]);
    SHADOW_PT(-0x1000, 0);
    gte_stsxy(&SHADOW_QUAD(0)->x2);
    gte_stsxy(&SHADOW_QUAD(2)->x0);
    gte_stsz(&z[3]);
    SHADOW_PT(0, 0);
    gte_stsxy(&SHADOW_QUAD(0)->x3);
    gte_stsxy(&SHADOW_QUAD(1)->x2);
    gte_stsxy(&SHADOW_QUAD(2)->x1);
    gte_stsxy(&SHADOW_QUAD(3)->x0);
    gte_stsz(&z[4]);
    if (z[4] == 0) {
        return prim;
    }
    SHADOW_PT(0x1000, 0);
    gte_stsxy(&SHADOW_QUAD(1)->x3);
    gte_stsxy(&SHADOW_QUAD(3)->x1);
    gte_stsz(&z[5]);
    SHADOW_PT(-0x1000, -0x1000);
    gte_stsxy(&SHADOW_QUAD(2)->x2);
    gte_stsz(&z[6]);
    SHADOW_PT(0, -0x1000);
    gte_stsxy(&SHADOW_QUAD(2)->x3);
    gte_stsxy(&SHADOW_QUAD(3)->x2);
    gte_stsz(&z[7]);
    SHADOW_PT(0x1000, -0x1000);
    gte_stsxy(&SHADOW_QUAD(3)->x3);
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
    setUV4(SHADOW_QUAD(0), tu, tv, tu + 0x1F, tv, tu, tv + 0x1F, tu + 0x1F, tv + 0x1F);
    addPrim(&g_gameState->gameData->unk10[g_gameState->gameData->unk8][k >> 3], SHADOW_QUAD(0));
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
    setUV4(SHADOW_QUAD(1), tu + 0x1F, tv, tu, tv, tu + 0x1F, tv + 0x1F, tu, tv + 0x1F);
    addPrim(&g_gameState->gameData->unk10[g_gameState->gameData->unk8][k >> 3], SHADOW_QUAD(1));
    k = z[3];
    /* z[3] again where z[4] (the shared centre corner) was meant; CSE drops the test, but the branch
       still counts toward the cse path length, which keeps quad 2 out of quad 0's cse paths */
    if (k < z[3]) {
        k = z[3];
    }
    if (k < z[6]) {
        k = z[6];
    }
    if (k < z[7]) {
        k = z[7];
    }
    setUV4(SHADOW_QUAD(2), tu, tv + 0x1F, tu + 0x1F, tv + 0x1F, tu, tv, tu + 0x1F, tv);
    addPrim(&g_gameState->gameData->unk10[g_gameState->gameData->unk8][k >> 3], SHADOW_QUAD(2));
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
    setUV4(SHADOW_QUAD(3), tu + 0x1F, tv + 0x1F, tu, tv + 0x1F, tu + 0x1F, tv, tu, tv);
    addPrim(&g_gameState->gameData->unk10[g_gameState->gameData->unk8][k >> 3], SHADOW_QUAD(3));
    return prim;
}

s32 func_800AD4AC(s32 arg0, s32 arg1) {
    return func_80062E3C(arg0, 0x100, 0xFF, 0x200, 0xE0, arg1, 0);
}

void btl_calcShadowMatrix(Unk800AD4E4 *arg0, BtlObj *arg1, MATRIX *arg2, s32 arg3, s32 arg4, s32 arg5) {
    VECTOR r;
    VECTOR up;
    VECTOR n;
    VECTOR d;
    MATRIX m;
    SVECTOR v;
    s32 *p0;
    s32 *p1;

    if (arg4 == arg5) {
        COPY_MATRIX(&m, &((MATRIX *)arg1->unk8->bones)[arg4]);
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
        p0 = ((MATRIX *)arg1->unk8->bones)[arg4].t;
        p1 = ((MATRIX *)arg1->unk8->bones)[arg5].t;
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
                 ((MATRIX *)arg1->unk8->bones)[arg4].t[0];
    arg2->t[1] = (((r.vy << 1) * arg0->unk4_12 + (n.vy << 1) * arg0->unk4_22) >> 12) + arg3;
    arg2->t[2] = (((r.vz << 1) * arg0->unk4_12 + (n.vz << 1) * arg0->unk4_22) >> 12) +
                 ((MATRIX *)arg1->unk8->bones)[arg4].t[2];
}

s32 btl_soundCmd(s32 arg0, s32 arg1, s32 arg2, s32 arg3, s32 arg4) {
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
    if (kind == 12 && g_gameState->gameData->unk851 != 0) {
        goto stream;
    }
    switch ((arg0 >> 12) & 3) {
    case 0:
        id = (arg1 & 0xFFFF) | 0x70000;
        switch (kind) {
        case 0:
        case 7:
            if (g_gameState->gameData->unk851 != 0) {
                return -1;
            }
            if (D_800F51F8 == -1) {
                D_800F51F8 = kind;
            }
            r = btl_getDepLink(id);
            ga = g_gameState->gameData;
            if (ga->unk81C[0] != arg1 || ga->unk820[0] != (u16)r) {
                if (ga->unk0 & 0x20000000) {
                    akao_stopMusic(ga->unk824[0]);
                }
                g_gameState->gameData->unk0 &= 0xCFFFFFFF;
                g_gameState->gameData->unk820[0] = r;
                g_gameState->gameData->unk81C[0] = arg1;
                if ((id & 0xFFFF) != 5) {
                    g_gameState->gameData->unk8C0 = (s32)(g_loadAreaEnd + 0x140700);
                    D_800F51EC = -1;
                    g_gameState->gameData->unk878 = 0x16800;
                    D_800F51F0 = id;
                    g_gameState->gameData->unk87C = 0;
                    g_gameState->gameData->unk851 = 2;
                    return g_gameState->gameData->unk851;
                }
                memmove(g_gameState->gameData->unk814, (void *)g_gameState->gameData->unk818, 0x2000);
                g_gameState->gameData->unk826[0] = 0x7F;
                g_gameState->gameData->unk0 |= 0x10000000;
            }
        finish:
            if (D_800F51F8 != 7) {
                g_gameState->gameData->unk824[0] = akao_playMusic(g_gameState->gameData->unk814);
                gb = g_gameState->gameData;
                if (!(gb->unk0 & 0x20000000)) {
                    akao_fadeMusicVolumeFrom(gb->unk824[0], 0x1E, 0, gb->unk826[0]);
                }
                g_gameState->gameData->unk0 |= 0x30000000;
            }
            D_800F51F8 = -1;
            return 0;
        case 1:
            akao_stopMusic(g_gameState->gameData->unk824[0]);
            g_gameState->gameData->unk826[0] = 0x7F;
            g_gameState->gameData->unk0 &= 0xDFFFFFFF;
            return 0;
        case 2:
            if ((arg0 & 0x3F) != 1) {
                break;
            }
            akao_setMusicVolume(g_gameState->gameData->unk824[0], arg2);
            g_gameState->gameData->unk826[0] = arg2;
            return 0;
        case 3:
            if ((arg0 & 0x3F) != 1) {
                break;
            }
            akao_fadeMusicVolume(g_gameState->gameData->unk824[0], arg2, arg3);
            func_800AE9C8(arg3, arg2);
            return 0;
        case 4:
            if ((arg0 & 0x3F) != 1) {
                break;
            }
            akao_fadeMusicVolumeFrom(g_gameState->gameData->unk824[0], arg2, arg3, arg4);
            func_800AE9C8(arg4, arg2);
            return 0;
        case 6:
            if ((arg0 & 0x3F) != 1) {
                break;
            }
            akao_fadeMusicVolume(0, arg2, arg3);
            func_800AE9C8(arg3, arg2);
            return 0;
        case 10:
            if ((arg0 & 0x3F) != 6) {
                break;
            }
            akao_sendCmd92(arg1);
            return 0;
        case 12:
            goto stream;
        }
        return -1;
    case 1:
        id = (arg1 & 0xFFFF) | 0x80000;
        h = 0;
        if (kind != 5 && kind != 6) {
            h = (s32)db_lookupRegistered(4, id, 0);
        }
        /* padded: a zero word follows the first jump table */
        __asm__(".section .rodata\n\t.word 0\n\t.section .text");
        switch (kind) {
        case 0:
            akao_playSe(h, arg2, arg3, arg4);
            return 0;
        case 1:
            akao_stopSe(h, arg2);
            return 0;
        case 2:
            switch (arg0 & 0x3F) {
            case 1:
                akao_setSeVolume(h, arg2, arg3);
                return 0;
            case 5:
                akao_sendCmdA2(h, arg2, arg3);
                return 0;
            }
            return -1;
        case 3:
            switch (arg0 & 0x3F) {
            case 1:
                akao_fadeSeVolume(h, arg2, arg3, arg4);
                return 0;
            case 5:
                akao_sendCmdA3(h, arg2, arg3, arg4);
                return 0;
            }
            return -1;
        case 5:
            switch (arg0 & 0x3F) {
            case 1:
                akao_setAllSeVolume(arg2);
                return 0;
            case 5:
                akao_sendCmdAA(arg2);
                return 0;
            }
            return -1;
        case 6:
            switch (arg0 & 0x3F) {
            case 1:
                akao_fadeAllSeVolume(arg2, arg3);
                return -1;
            case 5:
                akao_sendCmdAB(arg2, arg3);
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
                gc = g_gameState->gameData;
                e = gc->unk8E4.battle->unk1538[arg2 % 4];
                gc->unk8C0 = e->unk24;
                g_gameState->gameData->unk878 = 0x12FD8 - (e->unk24 - e->unk20);
                gx = g_gameState->gameData;
            } else {
                g_gameState->gameData->unk8C0 = (s32)g_gameState->gameData->unk8E4.battle->work->heap;
                g_gameState->gameData->unk878 = g_gameState->gameData->unk8E4.battle->work->ot[0] -
                                            (s32)g_gameState->gameData->unk8E4.battle->work->heap;
                gx = g_gameState->gameData;
            }
            k2 = gx->unk878;
            gx->unk878 = k2 / 2048 * 2048;
            D_800F51EC = arg2 % 4;
            D_800F51F8 = kind;
            D_800F51F0 = id;
            g_gameState->gameData->unk87C = 0;
            g_gameState->gameData->unk851 = 2;
            return g_gameState->gameData->unk851;
        case 12:
            goto stream;
        }
        return -1;
    default:
        return -1;
    }
stream:
    gd = g_gameState->gameData;
    switch (gd->unk851) {
    case 2:
        if (((D_800F51F0 >> 16) & 0xFF) == 7) {
            gd->unk852 = gd->unk820[0];
        } else {
            gd->unk852 = D_800F51F0;
        }
        D_800F51F4 = btl_getSoundFileSectors(D_800F51F0, &size);
        if (D_800F51EC == -1) {
            akao_beginBankUpload();
        }
        ge = g_gameState->gameData;
        cdRead_init(ge->unk854, D_800F51F4, size, (void *)ge->unk8C0, 0, g_gameState->gameData->unk878, btl_onSoundRead);
        g_gameState->gameData->unk851 = 3;
        break;
    case 3:
        if (cdRead_update(gd->unk854) == 0 && akao_getSpuTransferState() == 0) {
            gf = g_gameState->gameData;
            if (gf->unk854[0].unkC + gf->unk854[0].unk10 < gf->unk87C) {
                gf->unk8C0 = (s32)gf->unk854[0].unk8;
                gg = g_gameState->gameData;
                m = (s32)(g_loadAreaEnd + 0x156F00) - gg->unk8C0;
                m = m / 2048 * 2048;
                cdRead_init(gg->unk854, D_800F51F4, gg->unk854[0].unk4, (void *)gg->unk8C0,
                              gg->unk854[0].unkC + gg->unk854[0].unk10, m, btl_onSoundRead);
            } else if (D_800F51F2 == 7) {
                if (D_800F51F8 != 9) {
                    gf->unk826[0] = 0x7F;
                }
                g_gameState->gameData->unk851 = 4;
            } else {
                D_800F51F8 = -1;
                g_gameState->gameData->unk851 = 0;
            }
        }
        break;
    case 4:
        n = gd->unk854[0].unkC + gd->unk854[0].unk10 - gd->unk87C;
        if (n > 0) {
            memcpy(gd->unk814, (u8 *)(gd->unk8C0 + (gd->unk87C - gd->unk854[0].unkC)), n);
        }
        g_gameState->gameData->unk8C0 = (s32)((u8 *)g_gameState->gameData->unk814 + n);
        m2 = 0x6800 - n;
        m2 = m2 / 2048 * 2048;
        gh = g_gameState->gameData;
        cdRead_init(gh->unk854, D_800F51F4, gh->unk854[0].unk4, (void *)gh->unk8C0,
                      gh->unk854[0].unkC + gh->unk854[0].unk10, m2, NULL);
        g_gameState->gameData->unk851 = 5;
        break;
    case 5:
        if (cdRead_update(gd->unk854) == 0) {
            gi = g_gameState->gameData;
            k1 = gi->unk854[0].unkC + gi->unk854[0].unk10;
            if (k1 < gi->unk854[0].unk4) {
                k2 = k1 - gi->unk87C;
                gi->unk8C0 = (s32)((u8 *)gi->unk814 + k2);
                gj = g_gameState->gameData;
                cdRead_init(gj->unk854, D_800F51F4, gj->unk854[0].unk4, (void *)gj->unk8C0, k2, 0x6800 - k2, NULL);
            } else {
                func_800AE7C8((s32)gi->unk814);
                g_gameState->gameData->unk0 |= 0x10000000;
                g_gameState->gameData->unk851 = 0;
                if (D_800F51F8 == 0) {
                    goto finish;
                }
                D_800F51F8 = -1;
            }
        }
        break;
    }
    return g_gameState->gameData->unk851;
}

s32 btl_getWeaponSound(s32 arg0, u16 arg1) {
    void **p;
    GameData *g = g_gameState->gameData;
    s32 f;
    if (g->party[arg0] == 0) {
        return -1;
    }
    p = &g->unk8E4.battle->files->weaponFiles[arg0];
    f = (s32)*p;
    if (f == 0) {
        return -1;
    }
    return db_lookup(f, 3, arg1 | 0x80000, 0);
}

s32 func_800AE4FC(s32 arg0) {
    Character *c = g_gameState->gameData->party[arg0];

    if (c == NULL) {
        /* two nested do-while(0) wrappers (macros?): they change the block structure enough to load the table base after the NULL check */
        do {
            do {
                return -1;
            } while (0);
        } while (0);
    }
    return ((FileEntry16 *)file_find(8, g_weaponTable[c->equip[0]].unk2))->unk2 | 0x90000;
}

s32 btl_getSoundFileSectors(s32 arg0, s32 *arg1) {
    FileEntry *e;
    FileEntry *f;
    if (((arg0 >> 16) & 0xFF) == 7) {
        e = dep_find(g_gameState->gameData->depTable, arg0);
        if (arg1 != 0) {
            *arg1 = e->unk8 << 11;
        }
        return e->unk4;
    } else {
        f = file_find(11, arg0);
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
        m = &g_gameState->gameData->unk8E4.battle->files->weaponFiles[arg0];
        l = (BtlAnimEvtList *)db_lookup((s32)*m, 2, 0x140000, 0);
        e = l->unk4;
        for (i = l->unk2 - 1; i >= 0; i--) {
            if (e[i].unk0.b.unk2 == 9) {
                btl_soundCmd(0x7700, e[i].unk0.w, arg0 + 4, 0, 0);
                break;
            }
        }
        return ++arg1->unk6;
    case 3:
        if (btl_soundCmd(0xC00, 0, 0, 0, 0) == 0) {
            arg1->unk6 = 0;
        }
        return arg1->unk6;
    }
    return 0;
}

s32 btl_getDepLink(s32 arg0) {
    FileTable *t = g_gameState->gameData->depTable;
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
    s32 v = db_lookup(arg0, 3, 0x70000, 0);
    s32 a = db_lookup(arg0, 4, v, 0);
    memcpy((u8 *)arg0, (u8 *)a, db_lookup(arg0, 5, v, 0));
    return 0;
}

void btl_onSoundRead(Unk800AE844 *arg0) {
    s32 p = arg0->unk8;
    s32 id = g_gameState->gameData->unk852 | 0x90000;
    s32 f;
    s32 base;
    s32 n;

    if (arg0->unkC == 0) {
        f = db_lookup(p, 2, 0x1B0000, 0);
        base = p;
        if (f == -1) {
            p = db_lookup(p, 4, id, 0);
            g_gameState->gameData->unk87C = db_getSize(base);
        } else {
            p = db_lookup(f, 4, id, 0);
            g_gameState->gameData->unk87C = db_lookup(base, 2, 0x1B0001, 0) - g_gameState->gameData->unk8C0;
        }
    }
    n = g_gameState->gameData->unk87C - arg0->unkC;
    if (arg0->unk10 < n) {
        n = arg0->unk10;
    }
    n = arg0->unk8 + n - p;
    if (D_800F51EC == -1) {
        akao_uploadBank(p, n, 0);
    } else {
        func_80056048(p, D_800F51EC, 0);
    }
    arg0->unk8 = p;
}

void func_800AE9C8(s32 arg0, s16 arg1) {
    GameData *gd = g_gameState->gameData;
    gd->unk826[0] = arg0;
    if (arg0 == 0) {
        gd->unk842 = arg1;
        gd->unk8C4 = VSync(-1);
    }
}

s32 func_800AEA08(void) {
    GameData *gd = g_gameState->gameData;
    return (u32)(VSync(-1) - gd->unk8C4) < gd->unk842;
}

void btl_beginFrame(void) {
    ClearOTagR(g_gameState->gameData->unk10[g_gameState->gameData->unk8] - 8, 0x1010);
    ClearOTagR((u32 *)g_gameState->gameData->unk8E4.battle->work->ot[g_gameState->gameData->unk8], 0x100);
    ClearOTagR(fade_getOt(g_gameState->gameData->unk8), 1);
    ClearOTagR((u32 *)btl_getBannerOt(g_gameState->gameData->unk8), 1);
    setlen(&D_800FDEB8[g_gameState->gameData->unk8].unk0, 2);
    ((u32 *)&D_800FDEB8[g_gameState->gameData->unk8].unk0)[1] = GPU_STP(0, 0);
    ((u32 *)&D_800FDEB8[g_gameState->gameData->unk8].unk0)[2] = 0;
    addPrim(g_gameState->gameData->unk10[g_gameState->gameData->unk8], &D_800FDEB8[g_gameState->gameData->unk8].unk0);
    setlen(&D_800FDEB8[g_gameState->gameData->unk8].unkC, 2);
    ((u32 *)&D_800FDEB8[g_gameState->gameData->unk8].unkC)[1] = GPU_STP(0, 0);
    ((u32 *)&D_800FDEB8[g_gameState->gameData->unk8].unkC)[2] = 0;
    addPrim((u32 *)g_gameState->gameData->unk8E4.battle->work->ot[g_gameState->gameData->unk8], &D_800FDEB8[g_gameState->gameData->unk8].unkC);
    addPrim(g_gameState->gameData->unk10[g_gameState->gameData->unk8], fade_getOt(g_gameState->gameData->unk8));
    addPrim(g_gameState->gameData->unk10[g_gameState->gameData->unk8], btl_getBannerOt(g_gameState->gameData->unk8));
    btlui_resetVsyncCount();
}

void btl_endFrame(void) {
    if (!(g_gameState->gameData->unk0 & 0x100) && ((g_padPortHeld[0] & 0xF09) == 0xF09 || (g_padPortHeld[1] & 0xF09) == 0xF09)) {
        g_gameState->unk0 |= 1;
    }
    btlui_waitFrame();
    setlen(&D_800FDED0[g_gameState->gameData->unk8].unk0, 2);
    ((u32 *)&D_800FDED0[g_gameState->gameData->unk8].unk0)[1] = GPU_STP(1, 0);
    ((u32 *)&D_800FDED0[g_gameState->gameData->unk8].unk0)[2] = 0;
    addPrim(&g_gameState->gameData->unk10[g_gameState->gameData->unk8][0xFFF], &D_800FDED0[g_gameState->gameData->unk8].unk0);
    setlen(&D_800FDED0[g_gameState->gameData->unk8].unkC, 2);
    ((u32 *)&D_800FDED0[g_gameState->gameData->unk8].unkC)[1] = GPU_STP(1, 0);
    ((u32 *)&D_800FDED0[g_gameState->gameData->unk8].unkC)[2] = 0;
    addPrim(&((u32 *)g_gameState->gameData->unk8E4.battle->work->ot[g_gameState->gameData->unk8])[0xFF], &D_800FDED0[g_gameState->gameData->unk8].unkC);
    func_800EA290();
    if (!(g_gameState->gameData->unk0 & 0x100) && !(g_gameState->gameData->unk8E4.ovl65->unk0 & 0x200)) {
        btlui_drawOTagEnv(&((u32 *)g_gameState->gameData->unk8E4.battle->work->ot[g_gameState->gameData->unk8])[0xFF], &g_gameState->drawEnv[g_gameState->gameData->unk8]);
        DrawOTag(&g_gameState->gameData->unk10[g_gameState->gameData->unk8][0xFFF]);
    }
    if (!(g_gameState->gameData->unk0 & 0x20)) {
        g_gameState->gameData->unk8 ^= 1;
    }
}

s32 func_800AF1C8(void) {
    return 0;
}

void btl_preloadExit(void) {
    Battle11C0 *b;
    FileEntry *f;
    s32 size;
    s32 *dst;

    b = g_gameState->gameData->unk8E4.battle->work;
    preload_init();
    g_gameState->gameData->unk8BC = NULL;
    f = file_find(0, 0x10065);
    size = (f->unkC - f->unk4) << 11;
    preload_setEntry(preload_add(), 0, 0x10065, 0x801EDF00, size, size, f->unk4, size);
    f = file_find(1, 0x2713);
    dst = b->loadBuf;
    size = (f->unkC - f->unk4) << 11;
    preload_setEntry(preload_add(), 0, 0x2713, (s32)dst, size, size, f->unk4, size);
}
