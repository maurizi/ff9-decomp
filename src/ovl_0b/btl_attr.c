#include "ovl_0b.h"

void btl_hideMeshes(BtlUnit *unit, u16 meshMask) {
    s32 i;
    for (i = 0; i < 16; i++) {
        if ((meshMask >> i) & 1) {
            unit->model->hiddenMeshes |= 1 << i;
        }
    }
}

void btl_showMeshes(BtlUnit *unit, u16 meshMask) {
    s32 i;
    if (!(unit->flags & BTL_UNIT_PARTY)) {
        unit->model->flags &= ~BTL_MODEL_DRAWN;
    }
    for (i = 0; i < 16; i++) {
        if ((meshMask >> i) & 1) {
            unit->model->hiddenMeshes &= ~(1 << i);
        }
    }
}

void btl_hideWeapon(BtlUnit *unit) {
    s32 i;
    for (i = 0; i < unit->weaponModel->header->meshCount; i++) {
        unit->weaponModel->hiddenMeshes |= 1 << i;
    }
}

void btl_showWeapon(BtlUnit *unit) {
    s32 i;
    for (i = 0; i < unit->weaponModel->header->meshCount; i++) {
        unit->weaponModel->hiddenMeshes &= ~(1 << i);
    }
}

s32 btl_setKoMotion(BtlUnit *unit) {
    unit->model->motion = (BtlMotion *)unit->motions[BTL_MOTION_KO];
    unit->model->texAnims[2].flags &= ~(BTL_TEXANIM_PLAY | BTL_TEXANIM_ONCE);
    unit->model->texAnims[0].flags |= (BTL_TEXANIM_PLAY | BTL_TEXANIM_ONCE);
    unit->model->texAnims[0].pos = 0;
    unit->model->texAnims[0].lastFrame = 0x1000;
    unit->obj->frame = 0;
    if (unit->status & BTL_STATUS_AUTO_LIFE) {
        unit->koState = BTL_KO_REVIVING;
        btl_setCmd(unit->reviveCmd, BTL_CMD_AUTO_LIFE, 0, unit->unitBit, 0);
        btl_removeStatus(unit, BTL_STATUS_AUTO_LIFE);
        return 0;
    }
    unit->koState = BTL_KO_DONE;
    return 1;
}

void btl_loadPartyMotions(BtlUnit *unit, s32 kind, s32 slot) {
    u32 i;
    for (i = 0; i < 0x22; i++) {
        g_gameState->gameData->ctx.battle->partyMotions[slot][i] = (s32)db_lookupRegistered(DB_GET_ADDR, g_btlPartyMotionIds[kind][i] | DB_ANIM, 0);
    }
    unit->motions = g_gameState->gameData->ctx.battle->partyMotions[slot];
}

void btl_spreadTroubleDamage(BtlUnit *source) {
    BtlUnit *unit;
    s32 damage;
    s32 facing;
    for (unit = g_gameState->gameData->ctx.battle->unitList; unit != 0; unit = unit->next) {
        if ((unit->flags & BTL_UNIT_PARTY) == (source->flags & BTL_UNIT_PARTY) && unit != source && ((unit->flags >> BTL_UNIT_TARGETABLE_BIT) & 1)) {
            unit->popupFlags = BTL_POPUP_HP;
            damage = source->hpChange >> 1;
            if (unit->status & BTL_STATUS_KO) {
                unit->popupFlags = BTL_POPUP_MISS;
            } else if (unit->status & BTL_STATUS_PETRIFY) {
                unit->hpChange = 0;
            } else {
                if (unit != g_gameState->gameData->ctx.battle->curCmd->caster) {
                    facing = (unit->flags & BTL_UNIT_PARTY) << 11;
                    unit->obj->rotY = facing;
                    unit->rot[1] = facing;
                }
                if (damage < unit->hp) {
                    unit->hp -= damage;
                } else {
                    unit->hp = 0;
                }
                unit->hpChange = damage;
                if (unit->hp == 0) {
                    btl_killUnit(unit);
                }
            }
            btl_spawnPopups(unit);
        }
    }
}

void btl_copyStats(Unk800BD8C0 *dst, Unk800BD8C0 *src) {
    dst->hp = src->hp;
    dst->mp = src->mp;
    dst->atb = src->atb;
    dst->stones = src->stones;
}

void btl_resetAtbSpeed(Unk800BD8F0 *unit) {
    s32 speed;
    unit->atbSpeed = 10;
    speed = (g_gameState->config >> CFG_SHIFT_BTL_SPEED) & 7;
    if (speed == 0) {
        unit->atbSpeed = 8;
    } else if (speed == 2) {
        unit->atbSpeed = 14;
    }
}

void btl_updateHpStatus(BtlUnit *unit) {
    if (unit->max->maxHp < unit->hp * 6) {
        btl_removeStatus(unit, BTL_STATUS_LOW_HP);
        if (unit->max->maxHp < unit->hp) {
            unit->hp = unit->max->maxHp;
        }
        goto clamp;
    }
    if (unit->hp == 0) {
        if (unit->koState == BTL_KO_NONE) {
            btl_addStatus(unit, BTL_STATUS_KO);
        }
    } else {
        if (!(unit->status & BTL_STATUS_LOW_HP)) {
            btl_addStatus(unit, BTL_STATUS_LOW_HP);
        }
    clamp:
        unit->mp = unit->max->maxMp < unit->mp ? unit->max->maxMp : unit->mp;
        if (unit->flags & BTL_UNIT_PARTY) {
            unit->flags = (unit->flags & ~BTL_UNIT_WEAK_STANCE) | ((btl_hasStatus(unit, (BTL_STATUS_VENOM | BTL_STATUS_LOW_HP | BTL_STATUS_POISON | BTL_STATUS_SLEEP)) & 1) << BTL_UNIT_WEAK_STANCE_BIT);
        }
    }
}

void btl_toggleRow(BtlUnit *unit) {
    s32 dz = 0x01900000;
    u32 oldFlags;
    u32 newRow;
    u32 keptFlags;
    if (BTL_UNIT_ROW(unit->flags)) {
        dz = 0xFE700000;
    }
    unit->pos[2] = unit->home->pos[2] + dz;
    unit->home->pos[2] += dz;
    oldFlags = unit->flags;
    keptFlags = oldFlags & ~BTL_UNIT_ROW_MASK;
    newRow = BTL_UNIT_ROW(oldFlags);
    newRow = newRow == 0;
    unit->flags = keptFlags | (newRow << BTL_UNIT_ROW_SHIFT);
}

void btl_orElements(u8 *dst, u8 *src) {
    dst[0] |= src[0];
    dst[1] |= src[1];
    dst[2] |= src[2];
    dst[3] |= src[3];
}

void btl_damageHp(BtlUnit *unit, s32 damage, u8 hitMotion) {
    s32 facing;
    if (unit->status & BTL_STATUS_KO) {
        unit->popupFlags = BTL_POPUP_MISS;
        return;
    }
    if (unit->status & BTL_STATUS_PETRIFY) {
        unit->hpChange = 0;
        return;
    }
    if (unit != g_gameState->gameData->ctx.battle->curCmd->caster) {
        facing = (unit->flags & BTL_UNIT_PARTY) << 11;
        unit->obj->rotY = facing;
        unit->rot[1] = facing;
    }
    if (damage < unit->hp) {
        unit->hp -= damage;
    } else {
        unit->hp = 0;
    }
    unit->hpChange = damage;
    if (hitMotion) {
        btl_startHitMotion(unit);
        return;
    }
    if (unit->hp == 0) {
        btl_killUnit(unit);
    }
}

void btl_healHp(BtlUnit *unit, s32 amount) {
    if (unit->status & BTL_STATUS_KO) {
        unit->popupFlags = BTL_POPUP_MISS;
    } else if (unit->status & BTL_STATUS_PETRIFY) {
        amount = 0;
    } else {
        BtlUnit24 *max = unit->max;
        s32 hp = unit->hp + amount;
        if (hp < max->maxHp) {
            unit->hp = hp;
        } else {
            unit->hp = max->maxHp;
        }
    }
    unit->hpChange = amount;
}

void btl_damageMp(BtlUnit *unit, s32 damage) {
    if (unit->status & BTL_STATUS_KO) {
        unit->popupFlags = BTL_POPUP_MISS;
    } else if (unit->status & BTL_STATUS_PETRIFY) {
        damage = 0;
    } else if (damage < unit->mp) {
        unit->mp -= damage;
    } else {
        unit->mp = 0;
    }
    unit->mpChange = damage;
}

void btl_healMp(BtlUnit *unit, s32 amount) {
    if (unit->status & BTL_STATUS_KO) {
        unit->popupFlags = BTL_POPUP_MISS;
    } else if (unit->status & BTL_STATUS_PETRIFY) {
        amount = 0;
    } else if (unit->mp + amount < unit->max->maxMp) {
        unit->mp += amount;
    } else {
        unit->mp = unit->max->maxMp;
    }
    unit->mpChange = amount;
}

void btl_tickPoison(BtlUnit *unit) {
    s32 damage = 0;
    if (!(unit->status & BTL_STATUS_PETRIFY)) {
        damage = unit->max->maxHp >> 4;
        if (damage < unit->hp) {
            unit->hp -= damage;
        } else {
            btl_killUnit(unit);
        }
    }
    unit->tickDamage = damage;
    unit->tickFlags |= BTL_TICK_POISON;
}

void btl_tickRegen(BtlUnit *unit) {
    s32 amount = 0;
    if (!(unit->status & BTL_STATUS_PETRIFY)) {
        amount = unit->max->maxHp >> 4;
        if ((unit->status & BTL_STATUS_ZOMBIE) || btl_hasEnemyFlag(unit, BTL_ENEMY_UNDEAD) != 0) {
            unit->tickFlags |= BTL_TICK_REGEN_HURTS;
            if (amount < unit->hp) {
                unit->hp -= amount;
            } else {
                btl_killUnit(unit);
            }
        } else {
            if (unit->hp + amount < unit->max->maxHp) {
                unit->hp += amount;
            } else {
                unit->hp = unit->max->maxHp;
            }
        }
    }
    unit->tickHeal = amount;
    unit->tickFlags |= BTL_TICK_REGEN;
}

void btl_tickVenomMp(BtlUnit *unit) {
    s32 damage = 0;
    if (!(unit->status & BTL_STATUS_PETRIFY)) {
        damage = unit->max->maxMp >> 4;
        if (damage < unit->mp) {
            unit->mp -= damage;
        } else {
            unit->mp = 0;
        }
    }
    unit->tickMpDamage = damage;
    unit->tickFlags |= BTL_TICK_VENOM_MP;
}

u32 btl_getUnitAttr(BtlUnit *unit, s32 attr) {
    u32 value = 0xFFFFFF;

    switch (attr) {
    case BTL_ATTR_MAX_HP:
        value = unit->max->maxHp;
        break;
    case BTL_ATTR_HP:
        value = unit->hp;
        break;
    case BTL_ATTR_MAX_MP:
        value = unit->max->maxMp;
        break;
    case BTL_ATTR_MP:
        value = unit->mp;
        break;
    case BTL_ATTR_MAX_ATB:
        value = unit->max->maxAtb;
        break;
    case BTL_ATTR_LEVEL:
        if (unit->flags & BTL_UNIT_PARTY) {
            value = (g_gameState->gameData->characters + BTL_UNIT_INDEX(unit->flags))->level;
        } else {
            value = g_gameState->gameData->ctx.battle->enemies[BTL_UNIT_INDEX(unit->flags)].type->level;
        }
        break;
    case BTL_ATTR_IMMUNE_HI:
        value = ((u8 *)&unit->statusImmune)[3];
        break;
    case BTL_ATTR_IMMUNE_LO:
        value = unit->statusImmune & 0xFFFFFF;
        break;
    case BTL_ATTR_AUTO_STATUS_HI:
        value = ((u8 *)&unit->autoStatus)[3];
        break;
    case BTL_ATTR_AUTO_STATUS_LO:
        value = unit->autoStatus & 0xFFFFFF;
        break;
    case BTL_ATTR_STATUS_HI:
        value = ((u8 *)&unit->status)[3];
        break;
    case BTL_ATTR_STATUS_LO:
        value = unit->status & 0xFFFFFF;
        break;
    case BTL_ATTR_ELEM_GUARD:
        value = unit->elemGuard;
        break;
    case BTL_ATTR_ELEM_ABSORB:
        value = unit->elemAbsorb;
        break;
    case BTL_ATTR_ELEM_HALF:
        value = unit->elemHalf;
        break;
    case BTL_ATTR_ELEM_WEAK:
        value = unit->elemWeak;
        break;
    case BTL_ATTR_TARGETABLE:
        value = unit->flags >> BTL_UNIT_TARGETABLE_BIT;
        value &= 1;
        break;
    case BTL_ATTR_HIDDEN:
        value = unit->flags >> BTL_UNIT_HIDDEN_BIT;
        value &= 1;
        break;
    case BTL_ATTR_MODEL:
        value = (s16)unit->modelId;
        break;
    case BTL_ATTR_HIDDEN_MESHES:
        value = unit->hiddenMeshes;
        break;
    case BTL_ATTR_ROW:
        value = unit->flags >> BTL_UNIT_ROW_SHIFT;
        value &= 3;
        break;
    case BTL_ATTR_SLOT:
        value = (u8)unit->flags;
        value >>= 5;
        break;
    case BTL_ATTR_KIND:
        value = (g_gameState->gameData->characters + BTL_UNIT_INDEX(unit->flags))->info.w >> CHR_INFO_KIND_SHIFT;
        value &= 0x1F;
        break;
    case BTL_ATTR_CLASS_FLAGS:
        value = (g_gameState->gameData->characters + BTL_UNIT_INDEX(unit->flags))->classFlags;
        break;
    case BTL_ATTR_CATEGORY:
        value = g_gameState->gameData->ctx.battle->enemies[BTL_UNIT_INDEX(unit->flags)].type->category;
        break;
    case BTL_ATTR_WEAK_STANCE:
        value = unit->flags >> BTL_UNIT_WEAK_STANCE_BIT;
        value &= 1;
        break;
    case BTL_ATTR_INDEX:
        value = unit->flags >> BTL_UNIT_INDEX_SHIFT;
        value &= 0xF;
        break;
    case BTL_ATTR_STRENGTH:
        value = unit->strength;
        break;
    case BTL_ATTR_MAGIC:
        value = unit->magic;
        break;
    case BTL_ATTR_DEFENCE:
        value = unit->defense;
        break;
    case BTL_ATTR_EVADE:
        value = unit->evade;
        break;
    case BTL_ATTR_MAGIC_DEFENCE:
        value = unit->magicDef;
        break;
    case BTL_ATTR_MAGIC_EVADE:
        value = unit->magicEva;
        break;
    }
    return value;
}

void btl_setUnitAttr(BtlUnit *unit, s32 attr, u32 value) {
    s32 slot;
    s32 playOfs;
    s32 stopOfs;
    s32 onceOfs;

    switch (attr) {
    case BTL_ATTR_INSERT:
        btl_insertUnit(unit);
        break;
    case BTL_ATTR_REMOVE:
        btl_cancelUnitCmds(unit);
        btl_unlinkUnit(unit);
        break;
    case BTL_ATTR_CANCEL_CMDS:
        if ((u8)btl_cancelUnitCmds(unit)) {
            unit->cmdIssued = 0;
        }
        break;
    case BTL_ATTR_MAX_HP:
        unit->max->maxHp = value;
        break;
    case BTL_ATTR_HP:
        unit->hp = value;
        break;
    case BTL_ATTR_MAX_MP:
        unit->max->maxMp = value;
        break;
    case BTL_ATTR_MP:
        unit->mp = value;
        break;
    case BTL_ATTR_ATB:
        unit->atb = value;
        break;
    case BTL_ATTR_ELEM_GUARD:
        unit->elemGuard = value;
        break;
    case BTL_ATTR_ELEM_ABSORB:
        unit->elemAbsorb = value;
        break;
    case BTL_ATTR_ELEM_HALF:
        unit->elemHalf = value;
        break;
    case BTL_ATTR_ELEM_WEAK:
        unit->elemWeak = value;
        break;
    case BTL_ATTR_TARGETABLE:
        if (g_gameState->gameData->ctx.battle->phase == BTL_PHASE_INTRO) {
            unit->targetableReq = BTL_TGTREQ_OFF;
            unit->flags &= ~BTL_UNIT_TARGETABLE;
            unit->flags &= ~BTL_UNIT_ATB_ACTIVE;
            unit->flags &= ~BTL_UNIT_TARGETED;
        } else {
            unit->targetableReq = value + BTL_TGTREQ_SET_OFF;
        }
        break;
    case BTL_ATTR_HIDDEN:
        unit->flags = (unit->flags & ~BTL_UNIT_HIDDEN) | ((value & 1) << BTL_UNIT_HIDDEN_BIT);
        if (value != 0 && g_gameState->gameData->ctx.battle->phase == BTL_PHASE_INTRO) {
            btl_applyBgLight(unit->model);
            slot = btl_getModelSlot(unit);
            g_gameState->gameData->ctx.battle->work->shadows[slot].shade = btlbg_getHeader()->shadowShade;
        }
        break;
    case BTL_ATTR_SHADOW:
        unit->flags = (unit->flags & ~BTL_UNIT_SHADOW) | ((value & 1) << BTL_UNIT_SHADOW_BIT);
        break;
    case BTL_ATTR_SCALE:
        unit->model->flags |= BTL_MODEL_SCALED;
        unit->model->scaleX = value;
        unit->model->scaleY = value;
        unit->model->scaleZ = value;
        slot = btl_getModelSlot(unit);
        g_gameState->gameData->ctx.battle->work->shadows[slot].width = ((u32)(unit->shadowW * value) >> 12) * 14;
        slot = btl_getModelSlot(unit);
        g_gameState->gameData->ctx.battle->work->shadows[slot].depth = ((u32)(unit->shadowD * value) >> 12) * 12;
        break;
    case BTL_ATTR_RESET_SCALE:
        unit->model->flags &= ~BTL_MODEL_SCALED;
        slot = btl_getModelSlot(unit);
        g_gameState->gameData->ctx.battle->work->shadows[slot].width = unit->shadowW * 14;
        slot = btl_getModelSlot(unit);
        g_gameState->gameData->ctx.battle->work->shadows[slot].depth = unit->shadowD * 12;
        break;
    case BTL_ATTR_HIDE_MESHES:
        btl_hideMeshes(unit, value);
        unit->hiddenMeshes |= value;
        break;
    case BTL_ATTR_SHOW_MESHES:
        btl_showMeshes(unit, value);
        unit->hiddenMeshes &= ~value;
        break;
    case BTL_ATTR_TEXANIM_PLAY:
        playOfs = value * 20;
        ((BtlUnit9C50 *)((u8 *)unit->model->texAnims + playOfs))->flags |= BTL_TEXANIM_PLAY;
        ((BtlUnit9C50 *)((u8 *)unit->model->texAnims + playOfs))->pos = 0;
        ((BtlUnit9C50 *)((u8 *)unit->model->texAnims + playOfs))->lastFrame = 0x1000;
        break;
    case BTL_ATTR_TEXANIM_STOP:
        stopOfs = value * 20;
        ((BtlUnit9C50 *)((u8 *)unit->model->texAnims + stopOfs))->flags &= ~(BTL_TEXANIM_PLAY | BTL_TEXANIM_ONCE);
        break;
    case BTL_ATTR_TEXANIM_ONCE:
        onceOfs = value * 20;
        ((BtlUnit9C50 *)((u8 *)unit->model->texAnims + onceOfs))->flags |= (BTL_TEXANIM_PLAY | BTL_TEXANIM_ONCE);
        ((BtlUnit9C50 *)((u8 *)unit->model->texAnims + onceOfs))->pos = 0;
        ((BtlUnit9C50 *)((u8 *)unit->model->texAnims + onceOfs))->lastFrame = 0x1000;
        break;
    case BTL_ATTR_WEAK_STANCE:
        unit->flags = (unit->flags & ~BTL_UNIT_WEAK_STANCE) | ((value & 1) << BTL_UNIT_WEAK_STANCE_BIT);
        break;
    case BTL_ATTR_ADD_REWARDS:
        btl_addEnemyRewards(g_gameState->gameData->ctx.battle->enemies[BTL_UNIT_INDEX(unit->flags)].type);
        break;
    case BTL_ATTR_STRENGTH:
        unit->strength = value;
        break;
    case BTL_ATTR_MAGIC:
        unit->magic = value;
        break;
    case BTL_ATTR_DEFENCE:
        unit->defense = value;
        break;
    case BTL_ATTR_EVADE:
        unit->evade = value;
        break;
    case BTL_ATTR_MAGIC_DEFENCE:
        unit->magicDef = value;
        break;
    case BTL_ATTR_MAGIC_EVADE:
        unit->magicEva = value;
        break;
    case BTL_ATTR_TERRA_HOMING:
        unit->atb = unit->max->maxAtb;
        unit->cmdIssued = 1;
        btl_setCmd(unit->cmd, BTL_CMD_SUMMON2, ABIL_TERRA_HOMING, (u16)value, 8);
        btlui_enable(1);
        btlui_setAtbEnabled(1);
        break;
    }
}

s32 btl_getBattleVar(s32 var) {
    Battle *battle = g_gameState->gameData->ctx.battle;
    s32 value = -1;
    BtlUnit *unit;
    s32 kind;
    switch (var) {
    case BTL_GETVAR_AMBUSH:
        value = battle->scene.flags & BTL_SCENE_AMBUSH;
        break;
    case BTL_GETVAR_GIL:
        value = g_gameState->gameData->gil;
        break;
    case BTL_GETVAR_PHASE:
        value = battle->phase;
        break;
    case BTL_GETVAR_SUB_PHASE:
        value = battle->subPhase;
        break;
    case BTL_GETVAR_ATTACKER:
        if (battle->curCmd != NULL && battle->curCmd->caster != NULL) {
            value = battle->curCmd->caster->unitBit;
        }
        break;
    case BTL_GETVAR_TARGETS:
        if (battle->curCmd != NULL) {
            value = (u16)battle->curCmd->targets;
        }
        break;
    case BTL_GETVAR_WEAPON_ELEMENT:
        if (battle->curCmd != NULL && battle->curCmd->caster != NULL && battle->curCmd->caster->weapon != NULL) {
            value = battle->curCmd->caster->weapon[6];
        }
        break;
    case BTL_GETVAR_ABILITY_ELEMENT:
        if (battle->curCmd != NULL && battle->curCmd->caster != NULL) {
            value = 0;
            if (!(battle->curCmd->caster->support[1] & SA_BIT(SA_MAG_ELEM_NULL))) {
                value = battle->curCmd->ability->element;
            }
        }
        break;
    case BTL_GETVAR_LOAD_FLAGS:
        value = battle->loadFlags;
        break;
    case BTL_GETVAR_EXP:
        value = g_btlRewards.exp;
        break;
    case BTL_GETVAR_EIKO_REBIRTH:
        value = 0;
        for (unit = battle->unitList; unit != NULL; unit = unit->next) {
            if (unit->flags & BTL_UNIT_PARTY) {
                kind = g_gameState->gameData->characters[BTL_UNIT_INDEX(unit->flags)].info.b.kind;
            } else {
                kind = CHR_KIND_NONE;
            }
            if ((u32)(kind - CHR_KIND_EIKO) < 2) {
                value = btl_isCmdQueued(unit, BTL_CMD_REBIRTH_FLAME);
                break;
            }
        }
        break;
    }
    return value;
}

void btl_setBattleVar(s32 var, s32 value) {
    Battle *battle = g_gameState->gameData->ctx.battle;
    BtlUnit *unit;
    u32 sceneFlags;

    switch (var) {
    case BTL_SETVAR_STOP_COMBAT:
        btlui_setAtbEnabled(0);
        battle->phase = BTL_PHASE_END;
        battle->escapeHeld = 0;
        battle->subPhase = BTL_END_STOPPED;
        battle->cmdFlags &= ~BTL_CMDF_ESCAPE;
        btl_clearCmdQueue(battle);
        for (unit = battle->unitList; unit != NULL; unit = unit->next) {
            unit->flags &= ~BTL_UNIT_CMD_WAITING;
        }
        break;
    case BTL_SETVAR_END:
        if (battle->phase == BTL_PHASE_SCRIPT) {
            g_gameState->gameData->battleResult = value;
            if (value == BATTLE_END_VICTORY && ((battle->scene.flags >> BTL_SCENE_VICTORY_BIT) & 1)) {
                battle->phase = BTL_PHASE_END;
                battle->subPhase = BTL_END_SCRIPT_VICTORY;
            } else {
                if (g_gameState->gameData->battleResult == BATTLE_END_VICTORY) {
                    g_gameState->gameData->battleResult = 2;
                }
                battle->phase = BTL_PHASE_EXIT_OTHER;
                battle->subPhase = 0;
            }
        }
        break;
    case BTL_SETVAR_DEFEAT:
        if (battle->phase == BTL_PHASE_SCRIPT) {
            battle->phase = BTL_PHASE_EXIT_DEFEAT;
            battle->subPhase = 0;
            g_gameState->gameData->battleResult = BATTLE_END_DEFEAT;
        }
        break;
    case BTL_SETVAR_RESUME:
        if (battle->phase == BTL_PHASE_SCRIPT) {
            sceneFlags = battle->scene.flags;
            battle->phase = BTL_PHASE_ENTRANCE;
            battle->subPhase = 0;
            if ((sceneFlags & 0xC) != 0xC) {
                battle->scene.flags = (sceneFlags & ~BTL_SCENE_AMBUSH) | BTL_AMBUSH_NORMAL;
            }
        }
        break;
    case BTL_SETVAR_PLAY_CAMERA:
        if (battle->phase == BTL_PHASE_SCRIPT) {
            btlcam_playScene(btl_getSceneData(value));
        }
        break;
    case BTL_SETVAR_NEXT_LOCATION:
        g_gameState->gameData->battleFlags |= GD_BTL_NEXT_LOCATION;
        evt_setNextLocation(value);
        break;
    case BTL_SETVAR_ADD_GIL:
        g_gameState->gameData->gil += value;
        if (g_gameState->gameData->gil > GIL_MAX) {
            g_gameState->gameData->gil = GIL_MAX;
        }
        break;
    case BTL_SETVAR_ADD_DRAGON_KILL:
        if (g_gameState->gameData->dragonKills < RECORD_MAX) {
            g_gameState->gameData->dragonKills++;
        }
        break;
    case BTL_SETVAR_FADE_MUSIC:
        btl_soundCmd(SND_MUSIC_VOL_INTPL, 0, 0x1E, 0, 0);
        g_gameState->gameData->battleFlags |= GD_BTL_VICTORY_MUSIC_FADE;
        break;
    }
}
