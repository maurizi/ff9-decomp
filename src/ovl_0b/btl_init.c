#include "ovl_0b.h"

void btl_initParty(Battle *arg0) {
    s16 n;
    s16 i;
    Unk16D8 *p;
    Character *c;
    BtlUnit *u;
    s32 k;

    i = n = 0;
    p = evt_getObjectList()->unk0;
    for (; i < 4; i++) {
        c = g_gameState->gameData->party[i];
        if (c != NULL) {
            u = &arg0->units[n];
            u->modelId = g_btlCharModelIds[(c->unk34.w >> 4) & 0x1F];
            func_800BBF30(p->unk4, u->modelId);
            btl_initPartyUnit(c, u, i, n);
            n++;
            p = p->unk0;
            btl_insertUnit(u);
        }
    }
    for (; n < 4; n++) {
        arg0->units[n].unitBit = 0;
    }
    arg0->loadFlags |= 0x10;
    btl_placeParty();
    k = arg0->scene.unk0 & 3;
    if (k == 0) {
        for (u = arg0->unitList; u != NULL; u = u->next) {
            if (u->flags & 1) {
                u->atb = 0;
            }
        }
    } else if (k == 1) {
        for (u = arg0->unitList; u != NULL; u = u->next) {
            if (!(u->flags & 1)) {
                u->atb = 0;
            }
        }
    }
}

void btl_initEnemies(Battle *arg0) {
    s16 i = 0;
    s16 n = 4;
    Unk16D8 *p = evt_getBattleObjects()->unk0;
    BtlUnit *prev = NULL;
    BtlEnemy *e;
    BtlUnit *u;
    s16 m;
    s32 k;

    for (; i < g_gameState->gameData->unk8E4.battle->scene.unk4->unk1; i++, n++) {
        e = &arg0->enemies[i];
        e->flags = (e->flags & ~0x3F) | 0x20;
        u = &arg0->units[n];
        u->modelId = m = btl_getEnemyModelId(i);
        if (m < 0) {
            e->flags |= 0x200;
        } else {
            func_800BBF30(p->unk4, m);
            e->flags &= ~0x200;
            p = p->unk0;
        }
        k = i;
        u->unitBit = 0x10 << k;
        u->flags &= ~1;
        u->flags = (u->flags & ~0x1E) | ((k & 0xF) << 1);
        u->flags = (u->flags & ~0xE0) | (((k + 4) & 7) << 5);
        u->flags &= ~0x40000000;
        u->flags = (u->flags & ~0x800000) | (((e->flags >> 9) & 1) << 23);
        btl_resetUnit(u, &g_gameState->gameData->unk8E0[k]);
        u->home = (struct BtlUnit98 *)&e->home;
        u->weaponModel = NULL;
        u->support = (u32 *)D_800F5244;
        u->flags &= ~0x40000;
        if (prev != NULL) {
            prev->next = u;
        }
        prev = u;
    }
    for (; n < 8; n++) {
        arg0->units[n].unitBit = 0;
    }
    prev->next = NULL;
    arg0->unitList = &arg0->units[4];
    arg0->loadFlags |= 8;
    btl_setupEnemies();
    btlseq_init();
}

void btl_initPartyUnit(Character *c, BtlUnit *u, u16 slot, u16 n) {
    s32 k;
    s32 s;
    s32 i;

    u->unitBit = 1 << n;
    g_btlRewards.unkA |= 1 << (s = slot);
    u->flags |= 1;
    u->flags = (u->flags & ~0x1E) | ((c->unk34.w & 0xF) << 1);
    u->flags |= 0x10000000;
    u->flags = (u->flags & ~0xE0) | ((s & 7) << 5);
    u->flags &= ~0x800000;
    if (g_evtGlobalVars.b[0x10] == 0 || (c->unkA & 0x10) ||
        (((u->flags >> 1) & 0xF) == 2 && g_evtGlobalVars.b[0x11] != 0)) {
        u->trance = 0;
        u->flags &= ~0x40000000;
    } else {
        u->flags |= 0x40000000;
        u->trance = c->trance;
    }
    u->baseBone = 0;
    u->support = c->supportAbilities;
    u->speed = c->speed;
    u->strength = c->strength;
    u->magic = c->magic;
    u->spirit = c->spirit;
    u->level = c->level;
    u->max = (BtlUnit24 *)&c->maxHp;
    btl_copyStats((Unk800BD8C0 *)&u->hp, (Unk800BD8C0 *)&c->hp);
    btl_resetUnit(u, &g_gameState->gameData->unk8D8[c->unk34.w & 0xF]);
    u->vanishMeshes = 0xFFFF;
    btl_initDoomCount((Unk800C09D8 *)u);
    u->max->maxAtb = (0x3C - u->speed) * 160;
    btl_resetAtbSpeed((Unk800BD8F0 *)u);
    k = g_gameState->gameData->unk8E4.battle->scene.unk0 & 3;
    if (k == 0) {
        u->atb = 0;
    } else if (k == 1) {
        u->atb = u->max->maxAtb - 1;
    } else {
        if (g_randPtr >= g_randTableEnd) {
            rand_refill();
            if (g_randPtr >= g_randTableEnd) {
                rand_refill();
            }
        }
        u->atb = ((*g_randPtr << 8) | *g_randPtr++) % u->max->maxAtb;
    }
    u->weaponModel = &g_gameState->gameData->unk8E4.battle->weaponModels[n].unk0;
    btl_loadPartyMotions(u, (c->unk34.w >> 4) & 0x1F, n);
    btl_initWeapon(c, u);
    u->defense = c->defense;
    u->evade = c->evade;
    u->magicDef = c->magicDef;
    u->magicEva = c->magicEva;
    btl_initEquipElements(c, u);
    for (i = 0; i < u->weaponModel->header->meshCount; i++) {
        btl_setMeshRgb(u->weaponModel, i, 0, 0, 0);
    }
    if (u->hp * 6 < u->max->maxHp) {
        u->status |= BTL_STATUS_LOW_HP;
    }
    btl_addStatuses(u, c->status & 0xFE);
    if (c->status & 1) {
        btl_addStatus(u, BTL_STATUS_PETRIFY);
    }
    btl_applyAutoAbilities(u);
    u->home = (struct BtlUnit98 *)&u->obj->x;
    if (u->hp == 0 && btl_addStatus(u, BTL_STATUS_KO) == 2) {
        u->koState = 5;
        btl_setKoMotion(u);
        return;
    }
    u->flags = (u->flags & ~0x40000) | ((btl_hasStatus(u, (BTL_STATUS_VENOM | BTL_STATUS_LOW_HP | BTL_STATUS_POISON | BTL_STATUS_SLEEP)) & 1) << 18);
    u->model->motion = (BtlUnit9CC *)u->motions[(u->flags >> 18) & 1];
    u->obj->frame = 0;
}

u32 *btl_initEnemyState(Battle *arg0, u32 *arg1) {
    SVECTOR r2;
    SVECTOR r;
    BtlUnit *u;
    BtlUnit9CC *cc;
    s32 t;
    BtlUnitA0 *a0;
    s32 i;
    BtlEnemyType *p;

    for (i = 0; i < g_gameState->gameData->unk8E4.battle->scene.unk4->unk1; i++) {
        {
            p = arg0->enemies[i].type;
            u = &arg0->units[i + 4];
            u->level = p->level;
            u->max->maxAtb = (0x3C - u->speed) * 160;
            btl_resetAtbSpeed((Unk800BD8F0 *)u);
            if (g_randPtr >= g_randTableEnd) {
                rand_refill();
                if (g_randPtr >= g_randTableEnd) {
                    rand_refill();
                }
            }
            u->atb = ((*g_randPtr << 8) | *g_randPtr++) % u->max->maxAtb;
            u->weapon = NULL;
            btl_initDoomCount((Unk800C09D8 *)u);
            btl_hideMeshes(u, u->hiddenMeshes);
            if ((u->flags >> 23) & 1) {
                u->atb = 0;
                u->atbSpeed = 0;
                memset(&r, 0, 8);
                r.vx = u->rot[0];
                r.vy = u->rot[1];
                r.vz = u->rot[2];
                r2 = r;
                math_rotMatrixZYX(&r2, &u->model->matrix);
                u->model->matrix.t[0] = u->obj->x >> 16;
                u->model->matrix.t[1] = u->obj->y >> 16;
                u->model->matrix.t[2] = u->obj->z >> 16;
                cc = u->model->motion;
                if (IS_OFFSET(cc->unkC)) {
                    t = cc->unk10;
                    cc->unkC = (s32)cc + cc->unkC;
                    if (t != 0) {
                        cc->unk10 = (s32)u->model->motion + t;
                    }
                }
                arg1 = (u32 *)btl_calcBoneMatrices(u->model, 0, (u8 *)arg1);
                u->home->pos[0] = u->model->bones[u->baseBone].unk14 << 16;
                u->home->pos[1] = 0;
                u->home->pos[2] = u->model->bones[u->baseBone].unk1C << 16;
            } else {
                u->home->pos[0] = u->obj->x;
                u->home->pos[1] = u->obj->y;
                u->home->pos[2] = u->obj->z;
                a0 = u->obj;
                if (g_randPtr >= g_randTableEnd) {
                    rand_refill();
                }
                a0->frame = *g_randPtr++ % u->model->motion->unk2;
            }
        }
    }
    return arg1;
}

void btl_resetUnit(BtlUnit *arg0, Unk8E0 *arg1) {
    s32 i;
    u32 f;
    btl_initUnitCmds(arg0);
    btl_clearUnitStatus(arg0);
    arg0->unk7B = 0;
    arg0->menu = NULL;
    arg0->inMenuQueue = 0;
    arg0->popupFlags = 0;
    arg0->hpChange = 0;
    arg0->mpChange = 0;
    arg0->tickFlags = 0;
    arg0->tickHeal = 0;
    arg0->tickDamage = 0;
    arg0->tickMpDamage = 0;
    arg0->koState = 0;
    f = arg0->flags;
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
    arg0->flags = f;
    arg1->unk10 = arg0;
    if ((arg0->flags >> 23) & 1) {
        arg0->model = g_gameState->gameData->unk8E0->unk8;
        arg0->obj = g_gameState->gameData->unk8E0->unkC;
    } else {
        arg0->model = arg1->unk8;
        arg0->obj = arg1->unkC;
        arg0->model->texAnims[2].unk4 |= 1;
        arg0->model->texAnims[2].unkC = 0;
        arg0->model->texAnims[2].unk12 = 0x1000;
        for (i = 0; i < arg0->model->header->meshCount; i++) {
            btl_setMeshRgb(arg0->model, i, 0, 0, 0);
        }
    }
    arg0->hiddenMeshes = 0;
}

void func_800BBF30(Unk8004AB64 *arg0, s16 arg1) {
    arg0->unk34 = arg1;
    arg0->unk3B = evt_createModel(arg0);
}

void btl_initWeapon(Character *arg0, BtlUnit *arg1) {
    s32 *a;
    s32 *b;
    s32 *c;
    s32 id;
    arg1->weapon = (u8 *)&g_weaponTable[arg0->equip[0]];
    id = ((Unk8006E0E8 *)arg1->weapon)->unk2 | 0x20000;
    arg1->weaponModel->header = (BtlUnit9C8 *)btl_findWeaponModel(id, (u8)arg1->flags >> 5);
    btl_getWeaponTexInfo(id, (u8)arg1->flags >> 5, &a, &b, &c);
    btl_applyModelTextures(arg1->weaponModel, (s16 *)a, (s16 *)b);
    btl_applyBgLight(arg1->weaponModel);
    arg1->weaponModel->motion = 0;
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
    arg1->weaponModel->parent = arg1->model;
    arg1->weaponModel->parentBone = arg0->unk21;
}
