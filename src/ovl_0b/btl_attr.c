#include "ovl_0b.h"

void btl_hideMeshes(BtlUnit *arg0, u16 arg1) {
    s32 i;
    for (i = 0; i < 16; i++) {
        if ((arg1 >> i) & 1) {
            arg0->model->hiddenMeshes |= 1 << i;
        }
    }
}

void btl_showMeshes(BtlUnit *arg0, u16 arg1) {
    s32 i;
    if (!(arg0->flags & BTL_UNIT_PARTY)) {
        arg0->model->flags &= ~BTL_MODEL_DRAWN;
    }
    for (i = 0; i < 16; i++) {
        if ((arg1 >> i) & 1) {
            arg0->model->hiddenMeshes &= ~(1 << i);
        }
    }
}

void btl_hideWeapon(BtlUnit *arg0) {
    s32 i;
    for (i = 0; i < arg0->weaponModel->header->meshCount; i++) {
        arg0->weaponModel->hiddenMeshes |= 1 << i;
    }
}

void btl_showWeapon(BtlUnit *arg0) {
    s32 i;
    for (i = 0; i < arg0->weaponModel->header->meshCount; i++) {
        arg0->weaponModel->hiddenMeshes &= ~(1 << i);
    }
}

s32 btl_setKoMotion(BtlUnit *arg0) {
    arg0->model->motion = (BtlMotion *)arg0->motions[4];
    arg0->model->texAnims[2].flags &= ~(BTL_TEXANIM_PLAY | BTL_TEXANIM_ONCE);
    arg0->model->texAnims[0].flags |= (BTL_TEXANIM_PLAY | BTL_TEXANIM_ONCE);
    arg0->model->texAnims[0].pos = 0;
    arg0->model->texAnims[0].lastFrame = 0x1000;
    arg0->obj->frame = 0;
    if (arg0->status & BTL_STATUS_AUTO_LIFE) {
        arg0->koState = BTL_KO_REVIVING;
        btl_setCmd(arg0->reviveCmd, BTL_CMD_AUTO_LIFE, 0, arg0->unitBit, 0);
        btl_removeStatus(arg0, BTL_STATUS_AUTO_LIFE);
        return 0;
    }
    arg0->koState = BTL_KO_DONE;
    return 1;
}

void btl_loadPartyMotions(BtlUnit *arg0, s32 arg1, s32 arg2) {
    u32 i;
    for (i = 0; i < 0x22; i++) {
        g_gameState->gameData->ctx.battle->partyMotions[arg2][i] = (s32)db_lookupRegistered(DB_GET_ADDR, g_btlPartyMotionIds[arg1][i] | DB_ANIM, 0);
    }
    arg0->motions = g_gameState->gameData->ctx.battle->partyMotions[arg2];
}

void btl_spreadTroubleDamage(BtlUnit *arg0) {
    BtlUnit *u;
    s32 d;
    s32 v;
    for (u = g_gameState->gameData->ctx.battle->unitList; u != 0; u = u->next) {
        if ((u->flags & BTL_UNIT_PARTY) == (arg0->flags & BTL_UNIT_PARTY) && u != arg0 && ((u->flags >> BTL_UNIT_TARGETABLE_BIT) & 1)) {
            u->popupFlags = BTL_POPUP_HP;
            d = arg0->hpChange >> 1;
            if (u->status & BTL_STATUS_KO) {
                u->popupFlags = BTL_POPUP_MISS;
            } else if (u->status & BTL_STATUS_PETRIFY) {
                u->hpChange = 0;
            } else {
                if (u != g_gameState->gameData->ctx.battle->curCmd->caster) {
                    v = (u->flags & BTL_UNIT_PARTY) << 11;
                    u->obj->rotY = v;
                    u->rot[1] = v;
                }
                if (d < u->hp) {
                    u->hp -= d;
                } else {
                    u->hp = 0;
                }
                u->hpChange = d;
                if (u->hp == 0) {
                    btl_killUnit(u);
                }
            }
            btl_spawnPopups(u);
        }
    }
}

void btl_copyStats(Unk800BD8C0 *arg0, Unk800BD8C0 *arg1) {
    arg0->hp = arg1->hp;
    arg0->mp = arg1->mp;
    arg0->atb = arg1->atb;
    arg0->stones = arg1->stones;
}

void btl_resetAtbSpeed(Unk800BD8F0 *arg0) {
    s32 v;
    arg0->atbSpeed = 10;
    v = (g_gameState->config >> CFG_SHIFT_BTL_SPEED) & 7;
    if (v == 0) {
        arg0->atbSpeed = 8;
    } else if (v == 2) {
        arg0->atbSpeed = 14;
    }
}

void btl_updateHpStatus(BtlUnit *arg0) {
    if (arg0->max->maxHp < arg0->hp * 6) {
        btl_removeStatus(arg0, BTL_STATUS_LOW_HP);
        if (arg0->max->maxHp < arg0->hp) {
            arg0->hp = arg0->max->maxHp;
        }
        goto clamp;
    }
    if (arg0->hp == 0) {
        if (arg0->koState == BTL_KO_NONE) {
            btl_addStatus(arg0, BTL_STATUS_KO);
        }
    } else {
        if (!(arg0->status & BTL_STATUS_LOW_HP)) {
            btl_addStatus(arg0, BTL_STATUS_LOW_HP);
        }
    clamp:
        arg0->mp = arg0->max->maxMp < arg0->mp ? arg0->max->maxMp : arg0->mp;
        if (arg0->flags & BTL_UNIT_PARTY) {
            arg0->flags = (arg0->flags & ~BTL_UNIT_WEAK_STANCE) | ((btl_hasStatus(arg0, (BTL_STATUS_VENOM | BTL_STATUS_LOW_HP | BTL_STATUS_POISON | BTL_STATUS_SLEEP)) & 1) << BTL_UNIT_WEAK_STANCE_BIT);
        }
    }
}

void btl_toggleRow(BtlUnit *arg0) {
    s32 d = 0x01900000;
    u32 f;
    u32 v;
    u32 m;
    if (BTL_UNIT_ROW(arg0->flags)) {
        d = 0xFE700000;
    }
    arg0->pos[2] = arg0->home->pos[2] + d;
    arg0->home->pos[2] += d;
    f = arg0->flags;
    m = f & ~BTL_UNIT_ROW_MASK;
    v = BTL_UNIT_ROW(f);
    v = v == 0;
    arg0->flags = m | (v << BTL_UNIT_ROW_SHIFT);
}

void btl_orElements(u8 *arg0, u8 *arg1) {
    arg0[0] |= arg1[0];
    arg0[1] |= arg1[1];
    arg0[2] |= arg1[2];
    arg0[3] |= arg1[3];
}

void btl_damageHp(BtlUnit *arg0, s32 arg1, u8 arg2) {
    s32 v;
    if (arg0->status & BTL_STATUS_KO) {
        arg0->popupFlags = BTL_POPUP_MISS;
        return;
    }
    if (arg0->status & BTL_STATUS_PETRIFY) {
        arg0->hpChange = 0;
        return;
    }
    if (arg0 != g_gameState->gameData->ctx.battle->curCmd->caster) {
        v = (arg0->flags & BTL_UNIT_PARTY) << 11;
        arg0->obj->rotY = v;
        arg0->rot[1] = v;
    }
    if (arg1 < arg0->hp) {
        arg0->hp -= arg1;
    } else {
        arg0->hp = 0;
    }
    arg0->hpChange = arg1;
    if (arg2) {
        btl_startHitMotion(arg0);
        return;
    }
    if (arg0->hp == 0) {
        btl_killUnit(arg0);
    }
}

void btl_healHp(BtlUnit *arg0, s32 arg1) {
    if (arg0->status & BTL_STATUS_KO) {
        arg0->popupFlags = BTL_POPUP_MISS;
    } else if (arg0->status & BTL_STATUS_PETRIFY) {
        arg1 = 0;
    } else {
        BtlUnit24 *m = arg0->max;
        s32 hp = arg0->hp + arg1;
        if (hp < m->maxHp) {
            arg0->hp = hp;
        } else {
            arg0->hp = m->maxHp;
        }
    }
    arg0->hpChange = arg1;
}

void btl_damageMp(BtlUnit *arg0, s32 arg1) {
    if (arg0->status & BTL_STATUS_KO) {
        arg0->popupFlags = BTL_POPUP_MISS;
    } else if (arg0->status & BTL_STATUS_PETRIFY) {
        arg1 = 0;
    } else if (arg1 < arg0->mp) {
        arg0->mp -= arg1;
    } else {
        arg0->mp = 0;
    }
    arg0->mpChange = arg1;
}

void btl_healMp(BtlUnit *arg0, s32 arg1) {
    if (arg0->status & BTL_STATUS_KO) {
        arg0->popupFlags = BTL_POPUP_MISS;
    } else if (arg0->status & BTL_STATUS_PETRIFY) {
        arg1 = 0;
    } else if (arg0->mp + arg1 < arg0->max->maxMp) {
        arg0->mp += arg1;
    } else {
        arg0->mp = arg0->max->maxMp;
    }
    arg0->mpChange = arg1;
}

void btl_tickPoison(BtlUnit *arg0) {
    s32 v = 0;
    if (!(arg0->status & BTL_STATUS_PETRIFY)) {
        v = arg0->max->maxHp >> 4;
        if (v < arg0->hp) {
            arg0->hp -= v;
        } else {
            btl_killUnit(arg0);
        }
    }
    arg0->tickDamage = v;
    arg0->tickFlags |= BTL_TICK_POISON;
}

void btl_tickRegen(BtlUnit *arg0) {
    s32 v = 0;
    if (!(arg0->status & BTL_STATUS_PETRIFY)) {
        v = arg0->max->maxHp >> 4;
        if ((arg0->status & BTL_STATUS_ZOMBIE) || btl_hasEnemyFlag(arg0, BTL_ENEMY_UNDEAD) != 0) {
            arg0->tickFlags |= BTL_TICK_REGEN_HURTS;
            if (v < arg0->hp) {
                arg0->hp -= v;
            } else {
                btl_killUnit(arg0);
            }
        } else {
            if (arg0->hp + v < arg0->max->maxHp) {
                arg0->hp += v;
            } else {
                arg0->hp = arg0->max->maxHp;
            }
        }
    }
    arg0->tickHeal = v;
    arg0->tickFlags |= BTL_TICK_REGEN;
}

void btl_tickVenomMp(BtlUnit *arg0) {
    s32 v = 0;
    if (!(arg0->status & BTL_STATUS_PETRIFY)) {
        v = arg0->max->maxMp >> 4;
        if (v < arg0->mp) {
            arg0->mp -= v;
        } else {
            arg0->mp = 0;
        }
    }
    arg0->tickMpDamage = v;
    arg0->tickFlags |= BTL_TICK_VENOM_MP;
}

u32 btl_getUnitAttr(BtlUnit *arg0, s32 arg1) {
    u32 r = 0xFFFFFF;

    switch (arg1) {
    case BTL_ATTR_MAX_HP:
        r = arg0->max->maxHp;
        break;
    case BTL_ATTR_HP:
        r = arg0->hp;
        break;
    case BTL_ATTR_MAX_MP:
        r = arg0->max->maxMp;
        break;
    case BTL_ATTR_MP:
        r = arg0->mp;
        break;
    case BTL_ATTR_MAX_ATB:
        r = arg0->max->maxAtb;
        break;
    case BTL_ATTR_LEVEL:
        if (arg0->flags & BTL_UNIT_PARTY) {
            r = (g_gameState->gameData->characters + BTL_UNIT_INDEX(arg0->flags))->level;
        } else {
            r = g_gameState->gameData->ctx.battle->enemies[BTL_UNIT_INDEX(arg0->flags)].type->level;
        }
        break;
    case BTL_ATTR_IMMUNE_HI:
        r = ((u8 *)&arg0->statusImmune)[3];
        break;
    case BTL_ATTR_IMMUNE_LO:
        r = arg0->statusImmune & 0xFFFFFF;
        break;
    case BTL_ATTR_AUTO_STATUS_HI:
        r = ((u8 *)&arg0->autoStatus)[3];
        break;
    case BTL_ATTR_AUTO_STATUS_LO:
        r = arg0->autoStatus & 0xFFFFFF;
        break;
    case BTL_ATTR_STATUS_HI:
        r = ((u8 *)&arg0->status)[3];
        break;
    case BTL_ATTR_STATUS_LO:
        r = arg0->status & 0xFFFFFF;
        break;
    case BTL_ATTR_ELEM_GUARD:
        r = arg0->elemGuard;
        break;
    case BTL_ATTR_ELEM_ABSORB:
        r = arg0->elemAbsorb;
        break;
    case BTL_ATTR_ELEM_HALF:
        r = arg0->elemHalf;
        break;
    case BTL_ATTR_ELEM_WEAK:
        r = arg0->elemWeak;
        break;
    case BTL_ATTR_TARGETABLE:
        r = arg0->flags >> BTL_UNIT_TARGETABLE_BIT;
        r &= 1;
        break;
    case BTL_ATTR_HIDDEN:
        r = arg0->flags >> BTL_UNIT_HIDDEN_BIT;
        r &= 1;
        break;
    case BTL_ATTR_MODEL:
        r = (s16)arg0->modelId;
        break;
    case 0x3A:
        r = arg0->hiddenMeshes;
        break;
    case 0x40:
        r = arg0->flags >> BTL_UNIT_ROW_SHIFT;
        r &= 3;
        break;
    case 0x41:
        r = (u8)arg0->flags;
        r >>= 5;
        break;
    case 0x42:
        r = (g_gameState->gameData->characters + BTL_UNIT_INDEX(arg0->flags))->info.w >> CHR_INFO_KIND_SHIFT;
        r &= 0x1F;
        break;
    case 0x43:
        r = (g_gameState->gameData->characters + BTL_UNIT_INDEX(arg0->flags))->classFlags;
        break;
    case 0x44:
        r = g_gameState->gameData->ctx.battle->enemies[BTL_UNIT_INDEX(arg0->flags)].type->category;
        break;
    case 0x45:
        r = arg0->flags >> BTL_UNIT_WEAK_STANCE_BIT;
        r &= 1;
        break;
    case 0x46:
        r = arg0->flags >> BTL_UNIT_INDEX_SHIFT;
        r &= 0xF;
        break;
    case BTL_ATTR_STRENGTH:
        r = arg0->strength;
        break;
    case BTL_ATTR_MAGIC:
        r = arg0->magic;
        break;
    case BTL_ATTR_DEFENCE:
        r = arg0->defense;
        break;
    case BTL_ATTR_EVADE:
        r = arg0->evade;
        break;
    case BTL_ATTR_MAGIC_DEFENCE:
        r = arg0->magicDef;
        break;
    case BTL_ATTR_MAGIC_EVADE:
        r = arg0->magicEva;
        break;
    }
    return r;
}

void btl_setUnitAttr(BtlUnit *arg0, s32 arg1, u32 arg2) {
    s32 k;
    s32 off;
    s32 w;
    s32 x;

    switch (arg1) {
    case BTL_ATTR_INSERT:
        btl_insertUnit(arg0);
        break;
    case BTL_ATTR_REMOVE:
        btl_cancelUnitCmds(arg0);
        btl_unlinkUnit(arg0);
        break;
    case BTL_ATTR_CANCEL_CMDS:
        if ((u8)btl_cancelUnitCmds(arg0)) {
            arg0->cmdIssued = 0;
        }
        break;
    case BTL_ATTR_MAX_HP:
        arg0->max->maxHp = arg2;
        break;
    case BTL_ATTR_HP:
        arg0->hp = arg2;
        break;
    case BTL_ATTR_MAX_MP:
        arg0->max->maxMp = arg2;
        break;
    case BTL_ATTR_MP:
        arg0->mp = arg2;
        break;
    case BTL_ATTR_ATB:
        arg0->atb = arg2;
        break;
    case BTL_ATTR_ELEM_GUARD:
        arg0->elemGuard = arg2;
        break;
    case BTL_ATTR_ELEM_ABSORB:
        arg0->elemAbsorb = arg2;
        break;
    case BTL_ATTR_ELEM_HALF:
        arg0->elemHalf = arg2;
        break;
    case BTL_ATTR_ELEM_WEAK:
        arg0->elemWeak = arg2;
        break;
    case BTL_ATTR_TARGETABLE:
        if (g_gameState->gameData->ctx.battle->phase == BTL_PHASE_INTRO) {
            arg0->targetableReq = BTL_TGTREQ_OFF;
            arg0->flags &= ~BTL_UNIT_TARGETABLE;
            arg0->flags &= ~BTL_UNIT_ATB_ACTIVE;
            arg0->flags &= ~BTL_UNIT_TARGETED;
        } else {
            arg0->targetableReq = arg2 + BTL_TGTREQ_SET_OFF;
        }
        break;
    case BTL_ATTR_HIDDEN:
        arg0->flags = (arg0->flags & ~BTL_UNIT_HIDDEN) | ((arg2 & 1) << BTL_UNIT_HIDDEN_BIT);
        if (arg2 != 0 && g_gameState->gameData->ctx.battle->phase == BTL_PHASE_INTRO) {
            btl_applyBgLight(arg0->model);
            k = btl_getModelSlot(arg0);
            g_gameState->gameData->ctx.battle->work->shadows[k].shade = btlbg_getHeader()->shadowShade;
        }
        break;
    case BTL_ATTR_SHADOW:
        arg0->flags = (arg0->flags & ~BTL_UNIT_SHADOW) | ((arg2 & 1) << BTL_UNIT_SHADOW_BIT);
        break;
    case BTL_ATTR_SCALE:
        arg0->model->flags |= BTL_MODEL_SCALED;
        arg0->model->scaleX = arg2;
        arg0->model->scaleY = arg2;
        arg0->model->scaleZ = arg2;
        k = btl_getModelSlot(arg0);
        g_gameState->gameData->ctx.battle->work->shadows[k].width = ((u32)(arg0->shadowW * arg2) >> 12) * 14;
        k = btl_getModelSlot(arg0);
        g_gameState->gameData->ctx.battle->work->shadows[k].depth = ((u32)(arg0->shadowD * arg2) >> 12) * 12;
        break;
    case BTL_ATTR_RESET_SCALE:
        arg0->model->flags &= ~BTL_MODEL_SCALED;
        k = btl_getModelSlot(arg0);
        g_gameState->gameData->ctx.battle->work->shadows[k].width = arg0->shadowW * 14;
        k = btl_getModelSlot(arg0);
        g_gameState->gameData->ctx.battle->work->shadows[k].depth = arg0->shadowD * 12;
        break;
    case BTL_ATTR_HIDE_MESHES:
        btl_hideMeshes(arg0, arg2);
        arg0->hiddenMeshes |= arg2;
        break;
    case BTL_ATTR_SHOW_MESHES:
        btl_showMeshes(arg0, arg2);
        arg0->hiddenMeshes &= ~arg2;
        break;
    case 0x3D:
        off = arg2 * 20;
        ((BtlUnit9C50 *)((u8 *)arg0->model->texAnims + off))->flags |= BTL_TEXANIM_PLAY;
        ((BtlUnit9C50 *)((u8 *)arg0->model->texAnims + off))->pos = 0;
        ((BtlUnit9C50 *)((u8 *)arg0->model->texAnims + off))->lastFrame = 0x1000;
        break;
    case 0x3E:
        w = arg2 * 20;
        ((BtlUnit9C50 *)((u8 *)arg0->model->texAnims + w))->flags &= ~(BTL_TEXANIM_PLAY | BTL_TEXANIM_ONCE);
        break;
    case 0x3F:
        x = arg2 * 20;
        ((BtlUnit9C50 *)((u8 *)arg0->model->texAnims + x))->flags |= (BTL_TEXANIM_PLAY | BTL_TEXANIM_ONCE);
        ((BtlUnit9C50 *)((u8 *)arg0->model->texAnims + x))->pos = 0;
        ((BtlUnit9C50 *)((u8 *)arg0->model->texAnims + x))->lastFrame = 0x1000;
        break;
    case 0x45:
        arg0->flags = (arg0->flags & ~BTL_UNIT_WEAK_STANCE) | ((arg2 & 1) << BTL_UNIT_WEAK_STANCE_BIT);
        break;
    case BTL_ATTR_ADD_REWARDS:
        btl_addEnemyRewards(g_gameState->gameData->ctx.battle->enemies[BTL_UNIT_INDEX(arg0->flags)].type);
        break;
    case BTL_ATTR_STRENGTH:
        arg0->strength = arg2;
        break;
    case BTL_ATTR_MAGIC:
        arg0->magic = arg2;
        break;
    case BTL_ATTR_DEFENCE:
        arg0->defense = arg2;
        break;
    case BTL_ATTR_EVADE:
        arg0->evade = arg2;
        break;
    case BTL_ATTR_MAGIC_DEFENCE:
        arg0->magicDef = arg2;
        break;
    case BTL_ATTR_MAGIC_EVADE:
        arg0->magicEva = arg2;
        break;
    case 0x4E:
        arg0->atb = arg0->max->maxAtb;
        arg0->cmdIssued = 1;
        btl_setCmd(arg0->cmd, BTL_CMD_SUMMON2, ABIL_TERRA_HOMING, (u16)arg2, 8);
        btlui_enable(1);
        btlui_setAtbEnabled(1);
        break;
    }
}

s32 btl_getBattleVar(s32 arg0) {
    Battle *b = g_gameState->gameData->ctx.battle;
    s32 r = -1;
    BtlUnit *u;
    s32 k;
    switch (arg0) {
    case BTL_GETVAR_AMBUSH:
        r = b->scene.flags & BTL_SCENE_AMBUSH;
        break;
    case BTL_GETVAR_GIL:
        r = g_gameState->gameData->gil;
        break;
    case BTL_GETVAR_PHASE:
        r = b->phase;
        break;
    case BTL_GETVAR_SUB_PHASE:
        r = b->subPhase;
        break;
    case BTL_GETVAR_ATTACKER:
        if (b->curCmd != NULL && b->curCmd->caster != NULL) {
            r = b->curCmd->caster->unitBit;
        }
        break;
    case BTL_GETVAR_TARGETS:
        if (b->curCmd != NULL) {
            r = (u16)b->curCmd->targets;
        }
        break;
    case BTL_GETVAR_WEAPON_ELEMENT:
        if (b->curCmd != NULL && b->curCmd->caster != NULL && b->curCmd->caster->weapon != NULL) {
            r = b->curCmd->caster->weapon[6];
        }
        break;
    case BTL_GETVAR_ABILITY_ELEMENT:
        if (b->curCmd != NULL && b->curCmd->caster != NULL) {
            r = 0;
            if (!(b->curCmd->caster->support[1] & SA_BIT(SA_MAG_ELEM_NULL))) {
                r = b->curCmd->ability->element;
            }
        }
        break;
    case BTL_GETVAR_LOAD_FLAGS:
        r = b->loadFlags;
        break;
    case 41:
        r = g_btlRewards.exp;
        break;
    case 42:
        r = 0;
        for (u = b->unitList; u != NULL; u = u->next) {
            if (u->flags & BTL_UNIT_PARTY) {
                k = g_gameState->gameData->characters[BTL_UNIT_INDEX(u->flags)].info.b.kind;
            } else {
                k = 0x13;
            }
            if ((u32)(k - 0xA) < 2) {
                r = btl_isCmdQueued(u, BTL_CMD_REBIRTH_FLAME);
                break;
            }
        }
        break;
    }
    return r;
}

void btl_setBattleVar(s32 arg0, s32 arg1) {
    Battle *b = g_gameState->gameData->ctx.battle;
    BtlUnit *u;
    u32 f;

    switch (arg0) {
    case BTL_SETVAR_STOP_COMBAT:
        btlui_setAtbEnabled(0);
        b->phase = BTL_PHASE_END;
        b->escapeHeld = 0;
        b->subPhase = BTL_END_STOPPED;
        b->cmdFlags &= ~BTL_CMDF_ESCAPE;
        btl_clearCmdQueue(b);
        for (u = b->unitList; u != NULL; u = u->next) {
            u->flags &= ~BTL_UNIT_CMD_WAITING;
        }
        break;
    case BTL_SETVAR_END:
        if (b->phase == BTL_PHASE_SCRIPT) {
            g_gameState->gameData->battleResult = arg1;
            if (arg1 == 1 && ((b->scene.flags >> BTL_SCENE_VICTORY_BIT) & 1)) {
                b->phase = BTL_PHASE_END;
                b->subPhase = BTL_END_SCRIPT_VICTORY;
            } else {
                if (g_gameState->gameData->battleResult == 1) {
                    g_gameState->gameData->battleResult = 2;
                }
                b->phase = BTL_PHASE_EXIT_OTHER;
                b->subPhase = 0;
            }
        }
        break;
    case BTL_SETVAR_DEFEAT:
        if (b->phase == BTL_PHASE_SCRIPT) {
            b->phase = BTL_PHASE_EXIT_DEFEAT;
            b->subPhase = 0;
            g_gameState->gameData->battleResult = 3;
        }
        break;
    case BTL_SETVAR_RESUME:
        if (b->phase == BTL_PHASE_SCRIPT) {
            f = b->scene.flags;
            b->phase = BTL_PHASE_ENTRANCE;
            b->subPhase = 0;
            if ((f & 0xC) != 0xC) {
                b->scene.flags = (f & ~BTL_SCENE_AMBUSH) | BTL_AMBUSH_NORMAL;
            }
        }
        break;
    case BTL_SETVAR_PLAY_CAMERA:
        if (b->phase == BTL_PHASE_SCRIPT) {
            btlcam_playScene(btl_getSceneData(arg1));
        }
        break;
    case BTL_SETVAR_NEXT_LOCATION:
        g_gameState->gameData->battleFlags |= GD_BTL_NEXT_LOCATION;
        evt_setNextLocation(arg1);
        break;
    case BTL_SETVAR_ADD_GIL:
        g_gameState->gameData->gil += arg1;
        if (g_gameState->gameData->gil > GIL_MAX) {
            g_gameState->gameData->gil = GIL_MAX;
        }
        break;
    case 39:
        if (g_gameState->gameData->dragonKills < 9999) {
            g_gameState->gameData->dragonKills++;
        }
        break;
    case 40:
        btl_soundCmd(SND_MUSIC_VOL_INTPL, 0, 0x1E, 0, 0);
        g_gameState->gameData->battleFlags |= GD_BTL_VICTORY_MUSIC_FADE;
        break;
    }
}
