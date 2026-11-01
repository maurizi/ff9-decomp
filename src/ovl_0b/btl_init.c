#include "ovl_0b.h"

void btl_initParty(Battle *battle) {
    s16 count;
    s16 i;
    EvtListNode *node;
    Character *chr;
    BtlUnit *unit;
    s32 ambush;

    i = count = 0;
    node = evt_getObjectList()->next;
    for (; i < 4; i++) {
        chr = g_gameState->gameData->party[i];
        if (chr != NULL) {
            unit = &battle->units[count];
            unit->modelId = g_btlCharModelIds[(chr->info.w >> CHR_INFO_KIND_SHIFT) & 0x1F];
            btl_createObjModel(node->obj, unit->modelId);
            btl_initPartyUnit(chr, unit, i, count);
            count++;
            node = node->next;
            btl_insertUnit(unit);
        }
    }
    for (; count < 4; count++) {
        battle->units[count].unitBit = 0;
    }
    battle->loadFlags |= BTL_LOAD_PARTY_INIT;
    btl_placeParty();
    ambush = battle->scene.flags & BTL_SCENE_AMBUSH;
    if (ambush == BTL_AMBUSH_BACK) {
        for (unit = battle->unitList; unit != NULL; unit = unit->next) {
            if (unit->flags & BTL_UNIT_PARTY) {
                unit->atb = 0;
            }
        }
    } else if (ambush == BTL_AMBUSH_PREEMPTIVE) {
        for (unit = battle->unitList; unit != NULL; unit = unit->next) {
            if (!(unit->flags & BTL_UNIT_PARTY)) {
                unit->atb = 0;
            }
        }
    }
}

void btl_initEnemies(Battle *battle) {
    s16 i = 0;
    s16 slot = 4;
    EvtListNode *node = evt_getBattleObjects()->next;
    BtlUnit *prev = NULL;
    BtlEnemy *enemy;
    BtlUnit *unit;
    s16 modelId;
    s32 index;

    for (; i < g_gameState->gameData->ctx.battle->scene.group->enemyCount; i++, slot++) {
        enemy = &battle->enemies[i];
        enemy->flags = (enemy->flags & ~BTL_ENEMYF_FADE) | BTL_ENEMYF_FADE_OPAQUE;
        unit = &battle->units[slot];
        unit->modelId = modelId = btl_getEnemyModelId(i);
        if (modelId < 0) {
            enemy->flags |= BTL_ENEMYF_NO_MODEL;
        } else {
            btl_createObjModel(node->obj, modelId);
            enemy->flags &= ~BTL_ENEMYF_NO_MODEL;
            node = node->next;
        }
        index = i;
        unit->unitBit = 0x10 << index;
        unit->flags &= ~BTL_UNIT_PARTY;
        unit->flags = (unit->flags & ~BTL_UNIT_INDEX_MASK) | ((index & 0xF) << BTL_UNIT_INDEX_SHIFT);
        unit->flags = (unit->flags & ~BTL_UNIT_SLOT_MASK) | (((index + 4) & 7) << BTL_UNIT_SLOT_SHIFT);
        unit->flags &= ~BTL_UNIT_TRANCE_ENABLED;
        unit->flags = (unit->flags & ~BTL_UNIT_MODEL_PART) | (((enemy->flags >> BTL_ENEMYF_NO_MODEL_BIT) & 1) << BTL_UNIT_MODEL_PART_BIT);
        btl_resetUnit(unit, &g_gameState->gameData->npcModels[index]);
        unit->home = (struct BtlUnit98 *)&enemy->home;
        unit->weaponModel = NULL;
        unit->support = (u32 *)g_btlEnemySupport;
        unit->flags &= ~BTL_UNIT_WEAK_STANCE;
        if (prev != NULL) {
            prev->next = unit;
        }
        prev = unit;
    }
    for (; slot < 8; slot++) {
        battle->units[slot].unitBit = 0;
    }
    prev->next = NULL;
    battle->unitList = &battle->units[4];
    battle->loadFlags |= BTL_LOAD_ENEMIES_INIT;
    btl_setupEnemies();
    btlseq_init();
}

void btl_initPartyUnit(Character *chr, BtlUnit *unit, u16 slot, u16 unitNo) {
    s32 ambush;
    s32 slotNo;
    s32 i;

    unit->unitBit = 1 << unitNo;
    g_btlRewards.expMask |= 1 << (slotNo = slot);
    unit->flags |= BTL_UNIT_PARTY;
    unit->flags = (unit->flags & ~BTL_UNIT_INDEX_MASK) | ((chr->info.w & CHR_INFO_ID) << BTL_UNIT_INDEX_SHIFT);
    unit->flags |= BTL_UNIT_TARGETABLE;
    unit->flags = (unit->flags & ~BTL_UNIT_SLOT_MASK) | ((slotNo & 7) << BTL_UNIT_SLOT_SHIFT);
    unit->flags &= ~BTL_UNIT_MODEL_PART;
    if (g_evtGlobalVars.b[EVT_GB_TRANCE_ENABLED] == 0 || (chr->classFlags & CHR_CLASS_GUEST) ||
        (BTL_UNIT_INDEX(unit->flags) == CHR_DAGGER && g_evtGlobalVars.b[EVT_GB_DAGGER_FALTERS] != 0)) {
        unit->trance = 0;
        unit->flags &= ~BTL_UNIT_TRANCE_ENABLED;
    } else {
        unit->flags |= BTL_UNIT_TRANCE_ENABLED;
        unit->trance = chr->trance;
    }
    unit->baseBone = 0;
    unit->support = chr->supportAbilities;
    unit->speed = chr->speed;
    unit->strength = chr->strength;
    unit->magic = chr->magic;
    unit->spirit = chr->spirit;
    unit->level = chr->level;
    unit->max = (BtlUnit24 *)&chr->maxHp;
    btl_copyStats((Unk800BD8C0 *)&unit->hp, (Unk800BD8C0 *)&chr->hp);
    btl_resetUnit(unit, &g_gameState->gameData->chrModels[chr->info.w & CHR_INFO_ID]);
    unit->vanishMeshes = 0xFFFF;
    btl_initDoomCount((Unk800C09D8 *)unit);
    unit->max->maxAtb = (0x3C - unit->speed) * 160;
    btl_resetAtbSpeed((Unk800BD8F0 *)unit);
    ambush = g_gameState->gameData->ctx.battle->scene.flags & BTL_SCENE_AMBUSH;
    if (ambush == BTL_AMBUSH_BACK) {
        unit->atb = 0;
    } else if (ambush == BTL_AMBUSH_PREEMPTIVE) {
        unit->atb = unit->max->maxAtb - 1;
    } else {
        if (g_randPtr >= g_randTableEnd) {
            rand_refill();
            if (g_randPtr >= g_randTableEnd) {
                rand_refill();
            }
        }
        unit->atb = ((*g_randPtr << 8) | *g_randPtr++) % unit->max->maxAtb;
    }
    unit->weaponModel = &g_gameState->gameData->ctx.battle->weaponModels[unitNo].model;
    btl_loadPartyMotions(unit, (chr->info.w >> CHR_INFO_KIND_SHIFT) & 0x1F, unitNo);
    btl_initWeapon(chr, unit);
    unit->defense = chr->defense;
    unit->evade = chr->evade;
    unit->magicDef = chr->magicDef;
    unit->magicEva = chr->magicEva;
    btl_initEquipElements(chr, unit);
    for (i = 0; i < unit->weaponModel->header->meshCount; i++) {
        btl_setMeshRgb(unit->weaponModel, i, 0, 0, 0);
    }
    if (unit->hp * 6 < unit->max->maxHp) {
        unit->status |= BTL_STATUS_LOW_HP;
    }
    btl_addStatuses(unit, chr->status & 0xFE);
    if (chr->status & CHR_STATUS_PETRIFY) {
        btl_addStatus(unit, BTL_STATUS_PETRIFY);
    }
    btl_applyAutoAbilities(unit);
    unit->home = (struct BtlUnit98 *)&unit->obj->x;
    if (unit->hp == 0 && btl_addStatus(unit, BTL_STATUS_KO) == 2) {
        unit->koState = BTL_KO_PARTY_SET;
        btl_setKoMotion(unit);
        return;
    }
    unit->flags = (unit->flags & ~BTL_UNIT_WEAK_STANCE) | ((btl_hasStatus(unit, (BTL_STATUS_VENOM | BTL_STATUS_LOW_HP | BTL_STATUS_POISON | BTL_STATUS_SLEEP)) & 1) << BTL_UNIT_WEAK_STANCE_BIT);
    unit->model->motion = (BtlMotion *)unit->motions[(unit->flags >> BTL_UNIT_WEAK_STANCE_BIT) & 1];
    unit->obj->frame = 0;
}

u32 *btl_initEnemyState(Battle *battle, u32 *prim) {
    SVECTOR r2;
    SVECTOR rot;
    BtlUnit *unit;
    BtlMotion *motion;
    s32 rotKeysLo;
    BtlUnitA0 *obj;
    s32 i;
    BtlEnemyType *type;

    for (i = 0; i < g_gameState->gameData->ctx.battle->scene.group->enemyCount; i++) {
        {
            type = battle->enemies[i].type;
            unit = &battle->units[i + 4];
            unit->level = type->level;
            unit->max->maxAtb = (0x3C - unit->speed) * 160;
            btl_resetAtbSpeed((Unk800BD8F0 *)unit);
            if (g_randPtr >= g_randTableEnd) {
                rand_refill();
                if (g_randPtr >= g_randTableEnd) {
                    rand_refill();
                }
            }
            unit->atb = ((*g_randPtr << 8) | *g_randPtr++) % unit->max->maxAtb;
            unit->weapon = NULL;
            btl_initDoomCount((Unk800C09D8 *)unit);
            btl_hideMeshes(unit, unit->hiddenMeshes);
            if ((unit->flags >> BTL_UNIT_MODEL_PART_BIT) & 1) {
                unit->atb = 0;
                unit->atbSpeed = 0;
                memset(&rot, 0, 8);
                rot.vx = unit->rot[0];
                rot.vy = unit->rot[1];
                rot.vz = unit->rot[2];
                r2 = rot;
                math_rotMatrixZYX(&r2, &unit->model->matrix);
                unit->model->matrix.t[0] = unit->obj->x >> 16;
                unit->model->matrix.t[1] = unit->obj->y >> 16;
                unit->model->matrix.t[2] = unit->obj->z >> 16;
                motion = unit->model->motion;
                if (IS_OFFSET(motion->rotKeys)) {
                    rotKeysLo = motion->rotKeysLo;
                    motion->rotKeys = (s32)motion + motion->rotKeys;
                    if (rotKeysLo != 0) {
                        motion->rotKeysLo = (s32)unit->model->motion + rotKeysLo;
                    }
                }
                prim = (u32 *)btl_calcBoneMatrices(unit->model, 0, (u8 *)prim);
                unit->home->pos[0] = unit->model->bones[unit->baseBone].tx << 16;
                unit->home->pos[1] = 0;
                unit->home->pos[2] = unit->model->bones[unit->baseBone].tz << 16;
            } else {
                unit->home->pos[0] = unit->obj->x;
                unit->home->pos[1] = unit->obj->y;
                unit->home->pos[2] = unit->obj->z;
                obj = unit->obj;
                if (g_randPtr >= g_randTableEnd) {
                    rand_refill();
                }
                obj->frame = *g_randPtr++ % unit->model->motion->frameCount;
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
