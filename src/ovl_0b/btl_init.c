#include "ovl_0b.h"

void btl_initParty(Battle *battle) {
    s16 n;
    s16 i;
    EvtListNode *node;
    Character *chr;
    BtlUnit *u;
    s32 ambush;

    i = n = 0;
    node = evt_getObjectList()->next;
    for (; i < 4; i++) {
        chr = g_gameState->gameData->party[i];
        if (chr != NULL) {
            u = &battle->units[n];
            u->modelId = g_btlCharModelIds[(chr->info.w >> CHR_INFO_KIND_SHIFT) & 0x1F];
            btl_createObjModel(node->obj, u->modelId);
            btl_initPartyUnit(chr, u, i, n);
            n++;
            node = node->next;
            btl_insertUnit(u);
        }
    }
    for (; n < 4; n++) {
        battle->units[n].unitBit = 0;
    }
    battle->loadFlags |= BTL_LOAD_PARTY_INIT;
    btl_placeParty();
    ambush = battle->scene.flags & BTL_SCENE_AMBUSH;
    if (ambush == BTL_AMBUSH_BACK) {
        for (u = battle->unitList; u != NULL; u = u->next) {
            if (u->flags & BTL_UNIT_PARTY) {
                u->atb = 0;
            }
        }
    } else if (ambush == BTL_AMBUSH_PREEMPTIVE) {
        for (u = battle->unitList; u != NULL; u = u->next) {
            if (!(u->flags & BTL_UNIT_PARTY)) {
                u->atb = 0;
            }
        }
    }
}

void btl_initEnemies(Battle *battle) {
    s16 i = 0;
    s16 n = 4;
    EvtListNode *node = evt_getBattleObjects()->next;
    BtlUnit *prev = NULL;
    BtlEnemy *enemy;
    BtlUnit *u;
    s16 modelId;
    s32 index;

    for (; i < g_gameState->gameData->ctx.battle->scene.group->enemyCount; i++, n++) {
        enemy = &battle->enemies[i];
        enemy->flags = (enemy->flags & ~BTL_ENEMYF_FADE) | BTL_ENEMYF_FADE_OPAQUE;
        u = &battle->units[n];
        u->modelId = modelId = btl_getEnemyModelId(i);
        if (modelId < 0) {
            enemy->flags |= BTL_ENEMYF_NO_MODEL;
        } else {
            btl_createObjModel(node->obj, modelId);
            enemy->flags &= ~BTL_ENEMYF_NO_MODEL;
            node = node->next;
        }
        index = i;
        u->unitBit = 0x10 << index;
        u->flags &= ~BTL_UNIT_PARTY;
        u->flags = (u->flags & ~BTL_UNIT_INDEX_MASK) | ((index & 0xF) << BTL_UNIT_INDEX_SHIFT);
        u->flags = (u->flags & ~BTL_UNIT_SLOT_MASK) | (((index + 4) & 7) << BTL_UNIT_SLOT_SHIFT);
        u->flags &= ~BTL_UNIT_TRANCE_ENABLED;
        u->flags = (u->flags & ~BTL_UNIT_MODEL_PART) | (((enemy->flags >> BTL_ENEMYF_NO_MODEL_BIT) & 1) << BTL_UNIT_MODEL_PART_BIT);
        btl_resetUnit(u, &g_gameState->gameData->npcModels[index]);
        u->home = (struct BtlUnit98 *)&enemy->home;
        u->weaponModel = NULL;
        u->support = (u32 *)g_btlEnemySupport;
        u->flags &= ~BTL_UNIT_WEAK_STANCE;
        if (prev != NULL) {
            prev->next = u;
        }
        prev = u;
    }
    for (; n < 8; n++) {
        battle->units[n].unitBit = 0;
    }
    prev->next = NULL;
    battle->unitList = &battle->units[4];
    battle->loadFlags |= BTL_LOAD_ENEMIES_INIT;
    btl_setupEnemies();
    btlseq_init();
}

void btl_initPartyUnit(Character *c, BtlUnit *u, u16 slot, u16 n) {
    s32 ambush;
    s32 s;
    s32 i;

    u->unitBit = 1 << n;
    g_btlRewards.expMask |= 1 << (s = slot);
    u->flags |= BTL_UNIT_PARTY;
    u->flags = (u->flags & ~BTL_UNIT_INDEX_MASK) | ((c->info.w & CHR_INFO_ID) << BTL_UNIT_INDEX_SHIFT);
    u->flags |= BTL_UNIT_TARGETABLE;
    u->flags = (u->flags & ~BTL_UNIT_SLOT_MASK) | ((s & 7) << BTL_UNIT_SLOT_SHIFT);
    u->flags &= ~BTL_UNIT_MODEL_PART;
    if (g_evtGlobalVars.b[EVT_GB_TRANCE_ENABLED] == 0 || (c->classFlags & CHR_CLASS_GUEST) ||
        (BTL_UNIT_INDEX(u->flags) == CHR_DAGGER && g_evtGlobalVars.b[EVT_GB_DAGGER_FALTERS] != 0)) {
        u->trance = 0;
        u->flags &= ~BTL_UNIT_TRANCE_ENABLED;
    } else {
        u->flags |= BTL_UNIT_TRANCE_ENABLED;
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
    btl_resetUnit(u, &g_gameState->gameData->chrModels[c->info.w & CHR_INFO_ID]);
    u->vanishMeshes = 0xFFFF;
    btl_initDoomCount((Unk800C09D8 *)u);
    u->max->maxAtb = (0x3C - u->speed) * 160;
    btl_resetAtbSpeed((Unk800BD8F0 *)u);
    ambush = g_gameState->gameData->ctx.battle->scene.flags & BTL_SCENE_AMBUSH;
    if (ambush == BTL_AMBUSH_BACK) {
        u->atb = 0;
    } else if (ambush == BTL_AMBUSH_PREEMPTIVE) {
        u->atb = u->max->maxAtb - 1;
    } else {
        RAND_REFILL2();
        u->atb = RAND16() % u->max->maxAtb;
    }
    u->weaponModel = &g_gameState->gameData->ctx.battle->weaponModels[n].model;
    btl_loadPartyMotions(u, (c->info.w >> CHR_INFO_KIND_SHIFT) & 0x1F, n);
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
    if (c->status & CHR_STATUS_PETRIFY) {
        btl_addStatus(u, BTL_STATUS_PETRIFY);
    }
    btl_applyAutoAbilities(u);
    u->home = (struct BtlUnit98 *)&u->obj->x;
    if (u->hp == 0 && btl_addStatus(u, BTL_STATUS_KO) == 2) {
        u->koState = BTL_KO_PARTY_SET;
        btl_setKoMotion(u);
        return;
    }
    u->flags = (u->flags & ~BTL_UNIT_WEAK_STANCE) | ((btl_hasStatus(u, (BTL_STATUS_VENOM | BTL_STATUS_LOW_HP | BTL_STATUS_POISON | BTL_STATUS_SLEEP)) & 1) << BTL_UNIT_WEAK_STANCE_BIT);
    u->model->motion = (BtlMotion *)u->motions[(u->flags >> BTL_UNIT_WEAK_STANCE_BIT) & 1];
    u->obj->frame = 0;
}

u32 *btl_initEnemyState(Battle *battle, u32 *prim) {
    SVECTOR r2;
    SVECTOR r;
    BtlUnit *u;
    BtlMotion *motion;
    s32 rotKeysLo;
    BtlUnitA0 *obj;
    s32 i;
    BtlEnemyType *type;

    for (i = 0; i < g_gameState->gameData->ctx.battle->scene.group->enemyCount; i++) {
        {
            type = battle->enemies[i].type;
            u = &battle->units[i + 4];
            u->level = type->level;
            u->max->maxAtb = (0x3C - u->speed) * 160;
            btl_resetAtbSpeed((Unk800BD8F0 *)u);
            RAND_REFILL2();
            u->atb = RAND16() % u->max->maxAtb;
            u->weapon = NULL;
            btl_initDoomCount((Unk800C09D8 *)u);
            btl_hideMeshes(u, u->hiddenMeshes);
            if ((u->flags >> BTL_UNIT_MODEL_PART_BIT) & 1) {
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
                motion = u->model->motion;
                if (IS_OFFSET(motion->rotKeys)) {
                    rotKeysLo = motion->rotKeysLo;
                    motion->rotKeys = (s32)motion + motion->rotKeys;
                    if (rotKeysLo != 0) {
                        motion->rotKeysLo = (s32)u->model->motion + rotKeysLo;
                    }
                }
                prim = (u32 *)btl_calcBoneMatrices(u->model, 0, (u8 *)prim);
                u->home->pos[0] = u->model->bones[u->baseBone].tx << 16;
                u->home->pos[1] = 0;
                u->home->pos[2] = u->model->bones[u->baseBone].tz << 16;
            } else {
                u->home->pos[0] = u->obj->x;
                u->home->pos[1] = u->obj->y;
                u->home->pos[2] = u->obj->z;
                obj = u->obj;
                /* rand8_inline() written out: calling it changes the register allocation here */
                if (g_randPtr >= g_randTableEnd) {
                    rand_refill();
                }
                obj->frame = *g_randPtr++ % u->model->motion->frameCount;
            }
        }
    }
    return prim;
}

void btl_resetUnit(BtlUnit *unit, ModelSlot *modelSlot) {
    s32 i;
    u32 flags;
    btl_initUnitCmds(unit);
    btl_clearUnitStatus(unit);
    unit->escapeStance = 0;
    unit->menu = NULL;
    unit->inMenuQueue = 0;
    unit->popupFlags = 0;
    unit->hpChange = 0;
    unit->mpChange = 0;
    unit->tickFlags = 0;
    unit->tickHeal = 0;
    unit->tickDamage = 0;
    unit->tickMpDamage = 0;
    unit->koState = BTL_KO_NONE;
    flags = unit->flags;
    flags &= ~BTL_UNIT_HIT_MOTION;
    flags &= ~BTL_UNIT_CMD_WAITING;
    flags &= ~BTL_UNIT_KILLED;
    flags &= ~BTL_UNIT_MOTION_HELD;
    flags &= ~BTL_UNIT_HIDDEN;
    flags |= BTL_UNIT_SHADOW;
    flags &= ~BTL_UNIT_COVERING;
    flags &= ~BTL_UNIT_DODGED;
    flags &= ~BTL_UNIT_DEATH_CRY;
    flags &= ~BTL_UNIT_TARGETED;
    unit->flags = flags;
    modelSlot->unit = unit;
    if ((unit->flags >> BTL_UNIT_MODEL_PART_BIT) & 1) {
        unit->model = g_gameState->gameData->npcModels->model;
        unit->obj = g_gameState->gameData->npcModels->actor;
    } else {
        unit->model = modelSlot->model;
        unit->obj = modelSlot->actor;
        unit->model->texAnims[2].flags |= BTL_TEXANIM_PLAY;
        unit->model->texAnims[2].pos = 0;
        unit->model->texAnims[2].lastFrame = 0x1000;
        for (i = 0; i < unit->model->header->meshCount; i++) {
            btl_setMeshRgb(unit->model, i, 0, 0, 0);
        }
    }
    unit->hiddenMeshes = 0;
}

void btl_createObjModel(EvtObject *obj, s16 modelId) {
    obj->modelId = modelId;
    obj->modelSlot = evt_createModel(obj);
}

void btl_initWeapon(Character *chr, BtlUnit *unit) {
    s32 *texPages;
    s32 *uvOffsets;
    s32 *unusedTex;
    s32 id;
    unit->weapon = (u8 *)&g_weaponTable[chr->equip[0]];
    id = ((WeaponEntry *)unit->weapon)->modelId | DB_MODEL;
    unit->weaponModel->header = (BtlModelHeader *)btl_findWeaponModel(id, (u8)unit->flags >> BTL_UNIT_SLOT_SHIFT);
    btl_getWeaponTexInfo(id, (u8)unit->flags >> BTL_UNIT_SLOT_SHIFT, &texPages, &uvOffsets, &unusedTex);
    btl_applyModelTextures(unit->weaponModel, (s16 *)texPages, (s16 *)uvOffsets);
    btl_applyBgLight(unit->weaponModel);
    unit->weaponModel->motion = 0;
    switch (chr->info.b.kind) {
    case CHR_KIND_ZIDANE_DAGGER:
        chr->weaponBone = 0xD;
        break;
    case CHR_KIND_ZIDANE:
        chr->weaponBone = 0xD;
        break;
    case CHR_KIND_GARNET_LONG:
    case CHR_KIND_GARNET_SHORT:
        chr->weaponBone = 0xF;
        break;
    case CHR_KIND_GARNET_LONG_RACKET:
    case CHR_KIND_GARNET_SHORT_RACKET:
        chr->weaponBone = 0xF;
        break;
    case CHR_KIND_QUINA:
        chr->weaponBone = 0xE;
        break;
    case CHR_KIND_EIKO:
    case CHR_KIND_EIKO_RACKET:
        chr->weaponBone = 0xF;
        break;
    case CHR_KIND_FREYA:
        chr->weaponBone = 6;
        break;
    case CHR_KIND_CINNA:
        chr->weaponBone = 0x19;
        break;
    case CHR_KIND_MARCUS:
        chr->weaponBone = 6;
        break;
    case CHR_KIND_BLANK:
    case CHR_KIND_BLANK_ALT:
        chr->weaponBone = 0xE;
        break;
    case CHR_KIND_VIVI:
    case CHR_KIND_STEINER:
    case CHR_KIND_STEINER_DARK:
    case CHR_KIND_AMARANT:
    case CHR_KIND_BEATRIX:
        chr->weaponBone = 0x10;
        break;
    }
    unit->weaponModel->parent = unit->model;
    unit->weaponModel->parentBone = chr->weaponBone;
}
