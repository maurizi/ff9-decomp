#include "ovl_0b.h"

void btl_startBattle(void) {
    Battle *b;
    Battle11C0 *p;

    b = g_gameState->gameData->ctx.battle;
    p = b->work;
    btlbg_init(p->bgHeader, p->bgTexAnims);
    b->loadFlags |= BTL_LOAD_STARTED;
    btl_initCmdQueue(b);
    btl_initTargetCursor(b);
    btl_initSceneFlags();
    g_btlRewards.hasPostEvent = (b->scene.flags >> BTL_SCENE_POST_EVENT_BIT) & 1;
    msg_beginDraw();
    evt_update();
    btl_loadModelsStep(1);
    fade_in(0x20);
    btlcam_playScene(btl_getSceneData(0));
    b->phase = BTL_PHASE_INTRO;
}

void btl_updateCombat(GameData *gd, Battle *battle) {
    BtlUnit *u;
    s32 prim;
    u = battle->unitList;
    prim = battle->work->prims[gd->drawBuf];
    while (u != NULL) {
        if (((u->flags >> BTL_UNIT_TARGETED_BIT) & 1) && (battle->cmdFlags & BTL_CMDF_MARKERS)) {
            btl_projectUnitPos(u);
        } else {
            u->onScreen = 0;
        }
        u = u->next;
    }
    u = battle->unitList;
    while (u != NULL) {
        if (!((u->flags >> BTL_UNIT_HIDDEN_BIT) & 1)) {
            btl_updateUnitModel(u, &prim);
        }
        if (btlui_getActiveUnit() == u && (battle->cmdFlags & BTL_CMDF_MARKERS) && !(u->model->flags & BTL_MODEL_OFFSCREEN)) {
            btl_drawActiveMarker(gd, battle, u);
        }
        btl_updateHpStatus(u);
        btl_updateUnitStatus(u);
        u = u->next;
    }
    btl_processCmdQueue(battle);
    btl_drawScene(gd, battle, prim);
}

void btl_drawScene(GameData *gd, Battle *battle, s32 prim) {
    if (battle->deathCryWait) {
        battle->deathCryWait -= 1;
    }
    btlseq_update();
    prim = (s32)btlbg_draw(&gd->viewMatrix, battle->work->bgData, prim, gd->ot[gd->drawBuf],
                              battle->work->ot[gd->drawBuf]);
    prim = (s32)btlfx_runFrame((u8 *)prim);
    prim = (s32)btl_drawPopups((u8 *)prim);
    btl_drawBanner((u8 *)prim);
}

s32 btl_tryCounter(BtlUnit *unit, BtlUnit *attacker, BtlCmd *cmd) {
    s32 chance;
    if (unit->status & (BTL_STATUS_PETRIFY | BTL_STATUS_VENOM | BTL_STATUS_KO | BTL_STATUS_STOP | BTL_STATUS_FREEZE | BTL_STATUS_JUMP)) {
        return 0;
    }
    if (cmd->cmdNo >= BTL_CMD_TABLE_COUNT) {
        return 0;
    }
    if ((unit->support[1] & SA_BIT(SA_COUNTER)) && (cmd->ability->flags & BTL_ABIL_PHYSICAL)) {
        chance = unit->spirit;
        if (unit->support[1] & SA_BIT(SA_EYE_4_EYE)) {
            chance *= 2;
        }
        RAND_REFILL2();
        if (RAND16() % 100 < chance) {
            btl_queueAutoCmd(unit, BTL_CMD_COUNTER, ABIL_ATTACK, attacker->unitBit);
            return 1;
        }
    }
    if ((unit->support[1] & SA_BIT(SA_RETURN_MAGIC)) && (cmd->ability->flags & BTL_ABIL_RETURNABLE)) {
        btl_queueAutoCmd(unit, BTL_CMD_RETURN_MAGIC, cmd->subNo, attacker->unitBit);
        return 1;
    }
    return 0;
}

u16 btl_findCover(s32 targets) {
    BtlUnit *best = NULL;
    BtlUnit *target = btl_getUnitById(targets);
    BtlUnit *u;
    GameData *g;
    s32 facing;
    if (target->status & (BTL_STATUS_PETRIFY | BTL_STATUS_KO)) {
        return 0;
    }
    g = g_gameState->gameData;
    if (((g->characters + BTL_UNIT_INDEX(target->flags))->classFlags & CHR_CLASS_FEMALE) && target->hp < (target->max->maxHp >> 1)) {
        for (u = g->ctx.battle->unitList; u != NULL; u = u->next) {
            if ((u->support[1] & SA_BIT(SA_PROTECT_GIRLS)) && !(u->status & (BTL_STATUS_PETRIFY | BTL_STATUS_VENOM | BTL_STATUS_KO | BTL_STATUS_STOP | BTL_STATUS_HEAT | BTL_STATUS_FREEZE | BTL_STATUS_JUMP)) && u != target && (best == NULL || best->hp < u->hp)) {
                best = u;
            }
        }
    }
    if (best == NULL && (target->status & BTL_STATUS_LOW_HP)) {
        for (u = g_gameState->gameData->ctx.battle->unitList; u != NULL; u = u->next) {
            if ((u->support[1] & SA_BIT(SA_COVER)) && !(u->status & (BTL_STATUS_PETRIFY | BTL_STATUS_VENOM | BTL_STATUS_KO | BTL_STATUS_STOP | BTL_STATUS_HEAT | BTL_STATUS_FREEZE | BTL_STATUS_JUMP)) && u != target && (best == NULL || best->hp < u->hp)) {
                best = u;
            }
        }
    }
    if (best == NULL) {
        return 0;
    }
    facing = (best->flags & BTL_UNIT_PARTY) << 11;
    best->obj->rotY = facing;
    best->rot[1] = facing;
    best->pos[0] = target->pos[0];
    best->pos[2] = target->pos[2];
    target->pos[2] += 0xFE700000;
    best->model->motion = (BtlMotion *)best->motions[15];
    best->flags |= BTL_UNIT_COVERING;
    return best->unitBit;
}

void btl_applyAutoAbilities(BtlUnit *unit) {
    if (unit->support[0] & SA_BIT(SA_AUTO_REFLECT)) {
        unit->autoStatus |= BTL_STATUS_REFLECT;
    }
    if (unit->support[0] & SA_BIT(SA_AUTO_FLOAT)) {
        unit->autoStatus |= BTL_STATUS_FLOAT;
    }
    if (unit->support[0] & SA_BIT(SA_AUTO_HASTE)) {
        unit->autoStatus |= BTL_STATUS_HASTE;
        unit->statusImmune |= BTL_STATUS_SLOW;
        unit->atbSpeed = unit->atbSpeed * 3 / 2;
    }
    if (unit->support[0] & SA_BIT(SA_AUTO_REGEN)) {
        unit->autoStatus |= BTL_STATUS_REGEN;
        btl_initDotTimer(unit, BTL_STATUS_REGEN_BIT);
    }
    if (unit->support[0] & SA_BIT(SA_AUTO_LIFE)) {
        unit->status |= BTL_STATUS_AUTO_LIFE;
    }
    if (unit->support[1] & SA_BIT(SA_BODY_TEMP)) {
        unit->statusImmune |= (BTL_STATUS_HEAT | BTL_STATUS_FREEZE);
    }
    if (unit->support[1] & SA_BIT(SA_INSOMNIAC)) {
        unit->statusImmune |= BTL_STATUS_SLEEP;
    }
    if (unit->support[1] & SA_BIT(SA_ANTIBODY)) {
        unit->statusImmune |= (BTL_STATUS_VENOM | BTL_STATUS_POISON);
    }
    if (unit->support[1] & SA_BIT(SA_BRIGHT_EYES)) {
        unit->statusImmune |= BTL_STATUS_BLIND;
    }
    if (unit->support[1] & SA_BIT(SA_LOUDMOUTH)) {
        unit->statusImmune |= BTL_STATUS_SILENCE;
    }
    if (unit->support[1] & SA_BIT(SA_JELLY)) {
        unit->statusImmune |= (BTL_STATUS_PETRIFY | BTL_STATUS_GRADUAL_PETRIFY);
    }
    if (unit->support[1] & SA_BIT(SA_LOCOMOTION)) {
        unit->statusImmune |= BTL_STATUS_STOP;
    }
    if (unit->support[1] & SA_BIT(SA_CLEAR_HEADED)) {
        unit->statusImmune |= BTL_STATUS_CONFUSE;
    }
}

s32 btl_partyHasSupport(s32 word, u32 bits) {
    s32 i;
    GameData *gd = g_gameState->gameData;
    for (i = 0; i < 4; i++) {
        if (gd->party[i]->supportAbilities[word] & bits) {
            return 1;
        }
    }
    return 0;
}

void btl_tryAutoPotion(BtlUnit *unit, u8 cmdNo) {
    s32 i;
    if (!(unit->status & (BTL_STATUS_PETRIFY | BTL_STATUS_VENOM | BTL_STATUS_KO | BTL_STATUS_STOP | BTL_STATUS_FREEZE | BTL_STATUS_JUMP)) && cmdNo < BTL_CMD_TABLE_COUNT && (unit->support[1] & SA_BIT(SA_AUTO_POTION))) {
        for (i = 0xEC; (u8)i < 0xEE; i++) {
            if (item_getCount((u8)i) != 0) {
                btl_reserveItem((u8)i);
                btl_queueAutoCmd(unit, BTL_CMD_AUTO_POTION, (u8)i, unit->unitBit);
                return;
            }
        }
    }
}

void btl_applyHitSupports(BtlUnit *unit, u8 *ability) {
    u32 maxHp;
    u32 lt;
    s32 hp;
    if (!(unit->status & (BTL_STATUS_PETRIFY | BTL_STATUS_VENOM | BTL_STATUS_KO | BTL_STATUS_STOP | BTL_STATUS_FREEZE | BTL_STATUS_JUMP))) {
        if ((unit->support[1] & SA_BIT(SA_RESTORE_HP)) && unit->hp != 0 && (unit->status & BTL_STATUS_LOW_HP)) {
            maxHp = unit->max->maxHp;
            hp = unit->hp;
            lt = (s32)(hp + (maxHp >> 1)) < (s32)maxHp;
            if (lt) {
                unit->hp = hp + (maxHp >> 1);
            } else {
                unit->hp = maxHp;
            }
        }
        if ((unit->support[1] & SA_BIT(SA_ABSORB_MP)) && ability[0xA] != 0) {
            if (unit->mp + ability[0xA] < unit->max->maxMp) {
                unit->mp += ability[0xA];
            } else {
                unit->mp = unit->max->maxMp;
            }
        }
    }
}

void btl_initCmdQueue(Battle *battle) {
    battle->curCmd = NULL;
    battle->cmdPhase = 0;
    battle->cmdFlags = BTL_CMDF_MARKERS;
    battle->escapeCmd.caster = NULL;
    battle->cmdQueue.caster = NULL;
    btl_clearCmd_inline(&battle->cmdQueue);
    btl_clearCmd_inline(&battle->escapeCmd);
}

void btl_initTargetCursor(Battle *battle) {
    s8 i;
    s8 j;
    s32 shade;
    for (i = 0; i < 2; i++) {
        for (j = 0; j < 4; j++) {
            gfx_initTpagePolyF3(&battle->cursorPrims[i].faces[j].tpage, GetTPage(0, 1, 0, 0));
            if (j < 3) {
                shade = 0xBC - j * 0x3C;
            } else {
                shade = 0xA0;
            }
            setRGB0(&battle->cursorPrims[i].faces[j].poly, shade, shade, 0);
            SetSemiTrans(&battle->cursorPrims[i].faces[j].poly, 1);
        }
    }
}

void btl_initUnitCmds(BtlUnit *unit) {
    Battle *b = g_gameState->gameData->ctx.battle;
    BtlCmd *cmd;
    s32 firstSlot;
    s32 i;
    s32 j;
    if (b->phase == BTL_PHASE_INTRO) {
        unit->targetableReq = BTL_TGTREQ_ON;
    }
    unit->cmdIssued = 0;
    unit->onScreen = 0;
    if (unit->flags & BTL_UNIT_PARTY) {
        firstSlot = BTL_UNIT_SLOT(unit->flags) * 6;
    } else {
        firstSlot = (BTL_UNIT_SLOT(unit->flags) - 4) * 3 + 0x18;
    }
    cmd = &b->cmdSlots[firstSlot];
    /* unk4..unk18 are the unit's six command slots */
    for (i = 0; i < 6; i++) {
        if ((unit->flags & BTL_UNIT_PARTY) || i < 3) {
            (&unit->cmd)[i] = cmd;
            cmd->caster = unit;
            cmd++;
            btl_clearCmd_inline((&unit->cmd)[i]);
        } else {
            (&unit->cmd)[i] = NULL;
        }
    }
    for (j = 3; j >= 0; j--) {
        unit->bounceMasks[j] = 0;
    }
}

void btl_setCmd(BtlCmd *cmd, u32 cmdNo, s32 subNo, s32 targets, s32 targeting) {
    BtlUnit *u = cmd->caster;
    BtlModel *model;
    s32 *motions;
    BtlMotion *motion;

    switch (cmdNo) {
    case BTL_CMD_SPEAR:
    case BTL_CMD_SPEAR_TRANCE:
        g_gameState->gameData->ctx.battle->cmdFlags |= BTL_CMDF_SPEAR;
        cmd->ability = (BtlCmd8 *)&g_gameState->gameData->ctx.battle->abilities[subNo];
        break;
    case BTL_CMD_THROW:
        cmd->ability = (BtlCmd8 *)&g_gameState->gameData->ctx.battle->abilities[ABIL_THROW];
        break;
    case BTL_CMD_RETURN_MAGIC:
        cmd->ability = &g_gameState->gameData->ctx.battle->enemyAbilities[subNo];
        break;
    case BTL_CMD_ESCAPE:
        if (g_gameState->gameData->ctx.battle->cmdFlags & BTL_CMDF_ESCAPE) {
            cmd->subNo = subNo;
            return;
        }
        g_gameState->gameData->ctx.battle->cmdFlags |= BTL_CMDF_ESCAPE;
        cmd->ability = (BtlCmd8 *)&g_gameState->gameData->ctx.battle->abilities[ABIL_FLEE];
        break;
    case BTL_CMD_ITEM:
    case BTL_CMD_AUTO_POTION:
    case BTL_CMD_KO:
    case BTL_CMD_AUTO_LIFE:
    case BTL_CMD_PETRIFY:
        cmd->ability = (BtlCmd8 *)&g_gameState->gameData->ctx.battle->abilities[0];
        break;
    case BTL_CMD_STEAL:
        if (u->support[1] & SA_BIT(SA_MUG)) {
            subNo = ABIL_MUG;
        }
    default:
        cmd->ability = (BtlCmd8 *)&g_gameState->gameData->ctx.battle->abilities[subNo];
        break;
    }
    cmd->targets = targets;
    cmd->cmdNo = cmdNo;
    cmd->subNo = subNo;
    cmd->info.w = (cmd->info.w & ~BTL_CMDINFO_TARGETING) | (targeting & 3);
    cmd->info.w &= ~BTL_CMDINFO_COVERED;
    cmd->info.w &= ~BTL_CMDINFO_DODGED;
    cmd->info.w &= ~BTL_CMDINFO_REFLECT;
    if (cmdNo > BTL_CMD_TABLE_COUNT) {
        cmd->info.w |= BTL_CMDINFO_AUTOMATIC;
    } else {
        btl_removeStatus(u, BTL_STATUS_DEFEND);
    }
    if (cmdNo < BTL_CMD_ENEMY_REACT) {
        if (g_gameState->gameData->ctx.battle->curCmd != u->tranceCmd) {
            model = u->model;
            motions = u->motions;
            motion = model->motion;
            if (motion == (BtlMotion *)motions[0]) {
                model->motion = (BtlMotion *)motions[10];
                u->obj->frame = 0;
            } else if (motion == (BtlMotion *)motions[1]) {
                model->motion = (BtlMotion *)motions[11];
                u->obj->frame = 0;
            } else if (motion == (BtlMotion *)motions[13] && cmdNo < BTL_CMD_TABLE_COUNT) {
                model->motion = (BtlMotion *)motions[14];
                u->obj->frame = 0;
            }
        }
        u->flags |= BTL_UNIT_CMD_WAITING;
    }
    btl_enqueueCmd(cmd);
}

void btl_setEnemyCmd(s32 casterBit, s16 targets, u32 cmdNo, u32 abilityId) {
    Battle *b;
    BtlUnit *u;
    BtlCmd *cmd;

    u = btl_getUnitById(casterBit);
    b = g_gameState->gameData->ctx.battle;
    if (b->cmdFlags & BTL_CMDF_ESCAPE || (u->status & (BTL_STATUS_PETRIFY | BTL_STATUS_VENOM | BTL_STATUS_KO | BTL_STATUS_STOP | BTL_STATUS_SLEEP | BTL_STATUS_FREEZE))) {
        goto fail;
    }
    if (cmdNo == BTL_CMD_ENEMY_TURN) {
        if (b->phase != BTL_PHASE_COMBAT) {
            goto fail;
        }
        cmd = u->cmd;
        if (u->status & BTL_STATUS_CONFUSE) {
            targets = btl_pickRandomUnit(RAND8() & 1);
            abilityId = g_gameState->gameData->ctx.battle->enemies[BTL_UNIT_INDEX(u->flags)].type->confuseAbility;
        } else if (u->status & BTL_STATUS_BERSERK) {
            targets = btl_pickRandomUnit(1);
            abilityId = g_gameState->gameData->ctx.battle->enemies[BTL_UNIT_INDEX(u->flags)].type->confuseAbility;
        }
    } else {
        if (cmdNo != BTL_CMD_ENEMY_COUNTER) {
            if (cmdNo != BTL_CMD_ENEMY_DEATH) {
fail:
                u->cmdIssued = 0;
                return;
            }
        }
        cmd = u->autoCmd;
    }
    cmd->ability = &g_gameState->gameData->ctx.battle->enemyAbilities[abilityId];
    cmd->targets = targets;
    cmd->cmdNo = cmdNo;
    cmd->subNo = abilityId;
    if ((u32)(cmd->ability->targetType - BTL_TGT_ALL_ANY) < 7) { /* ALL_ANY .. EVERYONE */
        cmd->info.b.targeting = 1;
    } else {
        cmd->info.b.targeting = 0;
    }
    if (cmdNo > BTL_CMD_TABLE_COUNT) {
        cmd->info.b.automatic = 1;
    } else {
        btl_removeStatus(cmd->caster, BTL_STATUS_DEFEND);
    }
    cmd->info.b.covered = 0;
    cmd->info.b.dodged = 0;
    cmd->info.b.reflect = 0;
    u->flags |= BTL_UNIT_CMD_WAITING;
    btl_enqueueCmd(cmd);
}

void btl_processCmdQueue(Battle *b) {
    BtlCmd *cmd;
    BtlUnit *u;
    BtlUnit *w;
    BtlCmd *p;
    u8 k;

    if (b->curCmd == NULL) {
        if ((p = b->cmdQueue.next) == NULL) {
            return;
        }
        for (cmd = p; cmd != NULL; cmd = cmd->next) {
            if (!((cmd->cmdNo < BTL_CMD_KO && cmd->cmdNo != BTL_CMD_ESCAPE && cmd->cmdNo != BTL_CMD_REBIRTH_FLAME && (cmd->caster->status & (BTL_STATUS_PETRIFY | BTL_STATUS_VENOM | BTL_STATUS_STOP | BTL_STATUS_FREEZE))) ||
                  (cmd->cmdNo == BTL_CMD_TRANCE_EIDOLON && (cmd->caster->status & BTL_STATUS_KO)))) {
                break;
            }
        }
        if (cmd == NULL) {
            return;
        }
        if (!btl_isAtbRunning() && cmd->cmdNo < BTL_CMD_TABLE_COUNT) {
            return;
        }
        k = cmd->cmdNo;
        if (k < 0x37) {
            w = cmd->caster;
            if ((u32)(k - 0x31) < 4) {
                w->model->motion = (BtlMotion *)w->motions[9];
                w->obj->frame = 0;
            }
            if ((w->flags & BTL_UNIT_PARTY) && (s32)w->model->motion != w->motions[9] && (s32)w->model->motion != w->motions[17] &&
                !(w->status & BTL_STATUS_JUMP)) {
                if ((s32)w->model->motion == w->motions[(w->flags >> BTL_UNIT_WEAK_STANCE_BIT) & 1] && !((w->flags >> BTL_UNIT_CMD_WAITING_BIT) & 1)) {
                    w->model->motion = (BtlMotion *)w->motions[9];
                    w->obj->frame = 0;
                }
                return;
            }
            if (w->status & BTL_STATUS_HEAT) {
                btl_addStatus(w, BTL_STATUS_KO);
                return;
            }
        }
        b->curCmd = cmd;
        btl_removeCmd(cmd);
    }
    cmd = b->curCmd;
    if (cmd == NULL) {
        return;
    }
    u = cmd->caster;
    switch (b->cmdPhase) {
    case 0:
        if (!(u8)btl_checkCmdUsable(b, cmd) || !(u8)btl_validateTargets(b, cmd) || !(u8)btl_payMpCost(cmd)) {
            btl_releaseItem(cmd);
            if (u != NULL && (u->flags & BTL_UNIT_PARTY) && (s32)u->model->motion == u->motions[9]) {
                u->model->motion = (BtlMotion *)u->motions[32];
            }
            b->cmdPhase = 3;
            break;
        }
        if (u != NULL && (s32)u->model->motion != u->motions[9] && cmd->cmdNo < BTL_CMD_ENEMY_TURN) {
            u->model->motion = (BtlMotion *)u->motions[9];
            u->obj->frame = 0;
        }
        b->cmdPhase++;
        break;
    case 1:
        btl_startCmdEffect(cmd);
        b->cmdPhase++;
        break;
    case 2:
        if ((u->flags & BTL_UNIT_PARTY) || cmd->info.b.enemyBounce) {
            btl_updatePartyCmd(cmd);
        }
        break;
    case 3:
        if (cmd->cmdNo != BTL_CMD_ESCAPE) {
            btl_onCmdEffectDone();
        } else {
            btl_finishCmd(b);
        }
        break;
    }
    if ((u8)btl_waitHitMotions(cmd) && b->cmdPhase == 4) {
        btl_finishCmd(b);
    }
}

s32 btl_checkCmdUsable(Battle *battle, BtlCmd *cmd) {
    BtlUnit *u;
    BtlUnit *target;
    BtlCmd *delayed;
    BtlCmd8 *ability;
    u32 have;
    u32 need;
    s32 roll;
    s32 k;
    s32 msg;
    u8 st;
    s32 facing;

    u = cmd->caster;
    if (u != NULL) {
        if (cmd->cmdNo < BTL_CMD_ENEMY_REACT) {
            facing = (u->flags & BTL_UNIT_PARTY) << 11;
            u->obj->rotY = facing;
            u->rot[1] = facing;
        }
        if (g_evtGlobalVars.b[EVT_GB_DAGGER_FALTERS] != 0 && (u->flags & (BTL_UNIT_INDEX_MASK | BTL_UNIT_PARTY)) == (CHR_DAGGER << BTL_UNIT_INDEX_SHIFT | BTL_UNIT_PARTY) && cmd->cmdNo < BTL_CMD_TABLE_COUNT) {
            if (rand8_inline() < 0x40) {
                msg = 0x1B;
                goto fail;
            }
        }
        if (btl_checkCmdUsable_can(cmd) == 0) {
            return 0;
        }
    }
    switch (cmd->cmdNo) {
    case BTL_CMD_ATTACK:
        if (((battle->scene.flags >> BTL_SCENE_OUT_OF_REACH_BIT) & 1) && (*u->weapon & 1) && (u16)cmd->targets >= BTL_TARGETS_FIRST_ENEMY) {
            msg = 0x17;
            goto fail;
        }
        return 1;
    case BTL_CMD_JUMP:
    case BTL_CMD_JUMP_TRANCE:
        btl_addStatus(u, BTL_STATUS_JUMP);
        u->delayedCmd->cmdNo = cmd->cmdNo;
        delayed = u->delayedCmd;
        if (cmd->cmdNo == BTL_CMD_JUMP) {
            delayed->targets = cmd->targets;
        } else {
            delayed->targets = btl_getUnitsWithStatus(1, 0);
        }
        cmd->targets = u->unitBit;
        break;
    case BTL_CMD_SPEAR_TRANCE:
        u->delayedCmd->targets = btl_getUnitsWithStatus(1, 0);
        break;
    case BTL_CMD_ITEM:
    case BTL_CMD_THROW:
    case BTL_CMD_AUTO_POTION:
        if (item_getCount(cmd->subNo) == 0) {
            msg = 0x18;
            goto fail;
        }
        if (cmd->cmdNo == BTL_CMD_AUTO_POTION) {
            btl_showMessage(BTL_MSG_AUTO_POTION);
        }
        return 1;
    case BTL_CMD_SUMMON:
    case BTL_CMD_EIDOLON:
    case BTL_CMD_SUMMON2:
        btl_rollShortSummon(cmd);
        return 1;
    case BTL_CMD_BLK_MAG:
    case BTL_CMD_DBL_BLK:
        if (cmd->subNo == ABIL_METEOR) {
            btl_rollMagicFail(cmd);
        }
        return 1;
    case BTL_CMD_BLU_MAG:
        if (cmd->subNo == ABIL_ROULETTE) {
            cmd->targets = btl_pickRandomUnit(rand8_inline() & 1);
        } else if (cmd->subNo == ABIL_ANGELS_SNACK) {
            have = item_getCount(cmd->ability->power);
            need = btl_countActiveUnits(1);
            msg = 0x18;
            if (have >= need) {
                break;
            }
            goto fail;
        } else {
            return 1;
        }
        break;
    case BTL_CMD_FLAIR:
    case BTL_CMD_ELAN:
        if (cmd->subNo == ABIL_SPARE_CHANGE || cmd->subNo == ABIL_SPARE_CHANGE_ELAN) {
            need = cmd->ability->power * u->level;
            if (g_gameState->gameData->gil < need) {
                msg = 0x14;
                goto fail;
            }
            g_gameState->gameData->gil -= need;
        } else if (cmd->subNo == ABIL_CURSE || cmd->subNo == ABIL_CURSE_ELAN) {
            ability = cmd->ability;
            roll = rand8_inline();
            ability->element = 1 << (roll % 8);
        } else {
            return 1;
        }
        break;
    case BTL_CMD_SWD_MAG:
        return (u8)btl_checkSwdMagUsable(u, cmd->ability->mpCost);
    case BTL_CMD_COUNTER:
        target = btl_getUnitById(cmd->targets);
        if (target == NULL || !((target->flags >> BTL_UNIT_TARGETABLE_BIT) & 1) || (target->status & BTL_STATUS_KO)) {
            goto ret0;
        }
    case BTL_CMD_CHARGE_ATTACK:
        if ((battle->scene.flags >> BTL_SCENE_OUT_OF_REACH_BIT) & 1) {
            msg = 0x17;
            if (*u->weapon & 1) {
            fail:
                btl_showMessage(msg);
                return 0;
            }
        }
        if (cmd->cmdNo == BTL_CMD_COUNTER) {
            btl_showMessage(BTL_MSG_COUNTER);
        }
        return 1;
    case BTL_CMD_RETURN_MAGIC:
        target = btl_getUnitById(cmd->targets);
        if (target != NULL && ((target->flags >> BTL_UNIT_TARGETABLE_BIT) & 1) && !(target->status & BTL_STATUS_KO)) {
            btl_showMessage(BTL_MSG_RETURN_MAGIC);
            return 1;
        }
    ret0:
        return 0;
    case BTL_CMD_ESCAPE:
        if (battle->phase != BTL_PHASE_COMBAT) {
            return 1;
        }
        for (target = battle->unitList; target != NULL; target = target->next) {
            if ((target->flags & BTL_UNIT_PARTY) && !(target->status & (BTL_STATUS_PETRIFY | BTL_STATUS_VENOM | BTL_STATUS_ZOMBIE | BTL_STATUS_KO | BTL_STATUS_STOP | BTL_STATUS_SLEEP | BTL_STATUS_FREEZE | BTL_STATUS_JUMP)) && target->hp != 0) {
                if (target->model->motion != (BtlMotion *)target->motions[0x11]) {
                    target->model->motion = (BtlMotion *)target->motions[0x11];
                    target->obj->frame = 0;
                }
                battle->phase = BTL_PHASE_END;
                battle->subPhase = BTL_END_ESCAPE;
            }
        }
        if (*(u16 *)&battle->phase == ((BTL_END_ESCAPE << 8) | BTL_PHASE_END)) { /* phase and subPhase at once */
            btlui_setAtbEnabled(0);
            g_gameState->gameData->escapeCount++;
            if (cmd->subNo == 0) {
                g_gameState->gameData->battleFlags |= GD_BTL_FLED;
            }
            btl_clearCmdQueue(battle);
        } else {
            battle->cmdFlags &= ~BTL_CMDF_ESCAPE;
        }
        return 0;
    case BTL_CMD_TRANCE_EIDOLON:
        if (cmd->subNo != battle->eidolonAbility) {
            cmd->subNo = battle->eidolonAbility;
            cmd->ability = (BtlCmd8 *)&battle->abilities[cmd->subNo];
        }
        break;
    case BTL_CMD_TRANCE:
        if (u->status & BTL_STATUS_TRANCE) {
            btl_showMessage(BTL_MSG_TRANCE);
            u->modelId = g_btlCharModelIds[(u->flags & BTL_UNIT_PARTY) ? g_gameState->gameData->characters[BTL_UNIT_INDEX(u->flags)].info.b.kind + CHR_KIND_TRANCE : CHR_KIND_NONE + CHR_KIND_TRANCE];
        } else {
            u->modelId = g_btlCharModelIds[(u->flags & BTL_UNIT_PARTY) ? (g_gameState->gameData->characters[BTL_UNIT_INDEX(u->flags)].info.w >> CHR_INFO_KIND_SHIFT) & 0x1F : CHR_KIND_NONE];
        }
        break;
    case BTL_CMD_KO:
        btl_checkPartyDisabled(u);
        if (u->koState == BTL_KO_NONE) {
            if (u->flags & BTL_UNIT_PARTY) {
                if (u->model->motion == (BtlMotion *)u->motions[4]) {
                    u->koState = BTL_KO_PARTY_SET;
                    return 0;
                }
                u->koState = BTL_KO_START;
            } else if ((u->flags & (BTL_UNIT_KILLED | BTL_UNIT_MODEL_PART)) == BTL_UNIT_KILLED) {
                u->koState = BTL_KO_ENEMY_SOUND;
            } else {
                u->koState = BTL_KO_START;
            }
        }
        return 0;
    case BTL_CMD_AUTO_LIFE:
        u->hp = 1;
        btl_removeStatus(u, BTL_STATUS_KO);
        u->flags |= BTL_UNIT_HIT_MOTION;
        return 0;
    case BTL_CMD_PETRIFY:
        btl_interruptUnit(u, 1);
        u->status |= BTL_STATUS_PETRIFY;
        u->flags &= ~BTL_UNIT_ATB_ACTIVE;
        btl_checkSideDefeated(u);
        if (u->flags & BTL_UNIT_PARTY) {
            btl_makeGreyCluts(u);
        }
        btl_removeStatus(u, BTL_STATUS_GRADUAL_PETRIFY);
        btl_setStoneClut(u, 1);
        return 0;
    }
    return 1;
}

/* Narrow a command's target mask to units that can still be targeted. */
s32 btl_validateTargets(Battle *b, BtlCmd *cmd) {
    BtlCmd8 *ability;
    BtlUnit *u;
    Unk8006B1F4 *tbl;
    s32 i;
    u16 valid = 0;
    u16 koTargetable;
    u16 picked;

    if ((u16)cmd->targets != 0) {
        switch (cmd->cmdNo) {
        case BTL_CMD_ITEM:
        case BTL_CMD_AUTO_POTION:
            tbl = g_usableItemTable;
            i = cmd->subNo - ITEM_FIRST_USABLE;
            ability = (BtlCmd8 *)(tbl + i);
            koTargetable = (*(u32 *)ability >> 29) & 1;
            break;
        case BTL_CMD_TRANCE:
            return 1;
        default:
            ability = cmd->ability;
            koTargetable = (*(u32 *)ability >> 29) & 1;
            break;
        }
        for (u = b->unitList; u != NULL; u = u->next) {
            if (((u->flags >> BTL_UNIT_TARGETABLE_BIT) & 1) && (u->unitBit & (u16)cmd->targets) &&
                ((koTargetable && (u->flags & BTL_UNIT_PARTY)) || !(u->status & BTL_STATUS_KO))) {
                valid |= u->unitBit;
            }
        }
        if (valid) {
            cmd->targets = valid;
            return 1;
        }
        if (cmd->info.w & BTL_CMDINFO_TARGETING) {
            return 0;
        }
        if (!koTargetable) {
            picked = cmd->targets = btl_pickRandomUnit(cmd->targets & 0xF);
            if (picked) {
                return 1;
            }
        }
    }
    return 0;
}

void btl_updatePartyCmd(BtlCmd *cmd) {
    Battle *b;
    BtlUnit *u;
    BtlModel *model;
    s32 *motions;
    s32 *motions2;
    BtlModel *model2;
    BtlMotion *motion;
    s32 row;
    u16 frameCount;
    u16 frame;

    if (g_btlfxEnv->state->phase != BTLFX_PHASE_IDLE) {
        return;
    }
    b = g_gameState->gameData->ctx.battle;
    u = cmd->caster;
    if (b->popups.scanUnit != 0 || b->popups.detectEnemy != NULL) {
        return;
    }
    if (cmd->cmdNo == BTL_CMD_DEFEND) {
        model = u->model;
        motions = u->motions;
        if (model->motion == (BtlMotion *)motions[12]) {
            if (u->obj->frame < model->motion->frameCount) {
                return;
            }
            model->motion = (BtlMotion *)motions[13];
            u->obj->frame = 0;
        }
    } else if (cmd->cmdNo == BTL_CMD_CHANGE) {
        model = u->model;
        motions = u->motions;
        if (model->motion == (BtlMotion *)motions[9]) {
            if (u->obj->frame < model->motion->frameCount) {
                return;
            }
            model->motion = (BtlMotion *)motions[29 + BTL_UNIT_ROW(u->flags)];
            u->obj->frame = 0;
        }
        motions2 = u->motions;
        model2 = u->model;
        row = BTL_UNIT_ROW(u->flags);
        motion = model2->motion;
        if (motion == (BtlMotion *)motions2[29 + row]) {
            frameCount = motion->frameCount;
            frame = u->obj->frame;
            if (frame < frameCount) {
                frame++;
                if (row != 0) {
                    u->pos[2] = (-(frame * 400) / frameCount - 0x618) << 16;
                } else {
                    u->pos[2] = ((frame * 400) / frameCount - 0x7A8) << 16;
                }
                return;
            }
            btl_execCmd(u);
            u->model->motion = (BtlMotion *)u->motions[32];
            u->obj->frame = 0;
            return;
        }
        if (motion == (BtlMotion *)motions2[32]) {
            if (u->obj->frame < motion->frameCount) {
                return;
            }
            model2->motion = (BtlMotion *)motions2[0];
            u->obj->frame = 0;
        }
    }
    btl_onCmdEffectDone();
    if (cmd->cmdNo == BTL_CMD_TRANCE) {
        btl_reapplyStatusLook(u);
    }
}

void btl_execCmd(BtlUnit *target) {
    BtlCmd *cmd = g_gameState->gameData->ctx.battle->curCmd;
    BtlUnit *u = cmd->caster;
    Battle *b;
    u16 partyMask;
    u16 disabledMask;
    s32 i;
    Unk8006B1F4 *tbl;

    switch (cmd->cmdNo) {
    case BTL_CMD_ATTACK:
    case BTL_CMD_COUNTER:
    case BTL_CMD_CHARGE_ATTACK:
        btl_calcCmd(u, target, cmd, ((BtlCmd8 *)u->weapon)->formula);
        break;
    case BTL_CMD_JUMP:
    case BTL_CMD_JUMP_TRANCE:
        u->targetableReq = BTL_TGTREQ_SET_OFF;
        u->flags |= BTL_UNIT_HIDDEN;
        break;
    case BTL_CMD_ITEM:
    case BTL_CMD_AUTO_POTION:
        btl_useReservedItem(cmd->subNo);
        tbl = g_usableItemTable;
        i = cmd->subNo - ITEM_FIRST_USABLE;
        btl_calcCmd(u, target, cmd, ((BtlCmd8 *)(tbl + i))->formula);
        break;
    case BTL_CMD_REBIRTH_FLAME:
        partyMask = btl_getUnitMask(1);
        disabledMask = btl_getUnitsWithStatus(1, (BTL_STATUS_PETRIFY | BTL_STATUS_VENOM | BTL_STATUS_KO | BTL_STATUS_STOP));
        if (partyMask == 0 || partyMask == disabledMask) {
            b = g_gameState->gameData->ctx.battle;
            btlui_setAtbEnabled(0);
            if (b->phase != BTL_PHASE_END) {
                b->phase = BTL_PHASE_END;
                b->subPhase = BTL_END_VICTORY;
                btl_clearCmdQueue(b);
            }
        }
        btl_calcCmd(u, target, cmd, cmd->ability->formula);
        break;
    case BTL_CMD_THROW:
        btl_useReservedItem(cmd->subNo);
    default:
        btl_calcCmd(u, target, cmd, cmd->ability->formula);
        break;
    }
}

void btl_finishCmd(Battle *b) {
    BtlCmd *cmd = b->curCmd;
    BtlUnit *u = cmd->caster;
    u32 n;

    if (cmd->cmdNo < BTL_CMD_TABLE_COUNT) {
        if ((cmd == u->cmd && cmd->cmdNo != BTL_CMD_JUMP && cmd->cmdNo != BTL_CMD_JUMP_TRANCE) ||
            (cmd->cmdNo == BTL_CMD_DBL_WHT && BTL_UNIT_INDEX(u->flags) == CHR_EIKO && cmd == u->delayedCmd && !btl_isCmdPending(u->cmd))) {
            u->atb = 0;
            u->cmdIssued = 0;
        } else if (cmd->cmdNo == BTL_CMD_SPEAR) {
            btl_removeStatus(u, BTL_STATUS_JUMP);
            g_gameState->gameData->ctx.battle->cmdFlags &= ~BTL_CMDF_SPEAR;
        } else if (cmd->cmdNo == BTL_CMD_SPEAR_TRANCE) {
            btl_addStatus(u, BTL_STATUS_JUMP);
            u->targetableReq = BTL_TGTREQ_SET_OFF;
            u->flags |= BTL_UNIT_HIDDEN;
            g_gameState->gameData->ctx.battle->cmdFlags &= ~BTL_CMDF_SPEAR;
        }
        if ((u->status & BTL_STATUS_TRANCE) && cmd->cmdNo != BTL_CMD_JUMP && cmd->cmdNo != BTL_CMD_JUMP_TRANCE) {
            n = (0x12C - u->level) / u->spirit * 10;
            if (cmd->cmdNo == BTL_CMD_DBL_WHT || cmd->cmdNo == BTL_CMD_DBL_BLK) {
                n = (u8)n >> 1;
            }
            if ((u8)n < u->trance) {
                u->trance -= n;
            } else {
                btl_removeStatus(u, BTL_STATUS_TRANCE);
            }
            if (cmd->cmdNo == BTL_CMD_EIDOLON && b->eidolonAbility != 0) {
                b->cmdFlags |= BTL_CMDF_EIDOLON_TIMER;
                b->eidolonTimer = (0x3C - u->spirit) * 200;
            }
        }
    } else if (cmd->cmdNo == BTL_CMD_TRANCE_EIDOLON) {
        b->cmdFlags &= ~BTL_CMDF_EIDOLON_QUEUED;
        b->eidolonTimer = (0x3C - u->spirit) * 200;
    } else if (cmd->cmdNo < BTL_CMD_ENEMY_COUNTER && btl_isCmdPending(u->cmd)) {
        u->flags |= BTL_UNIT_CMD_WAITING;
    }
    if (cmd->info.b.covered) {
        for (u = g_gameState->gameData->ctx.battle->unitList; u != NULL; u = u->next) {
            if (u->flags & BTL_UNIT_PARTY) {
                u->pos[0] = u->home->pos[0];
                u->pos[1] = u->home->pos[1];
                u->pos[2] = u->home->pos[2];
                if ((u->flags >> BTL_UNIT_COVERING_BIT) & 1) {
                    u->flags &= ~BTL_UNIT_COVERING;
                    btl_setIdleMotion(u);
                }
            }
        }
    }
    if (cmd->info.b.dodged) {
        u = btl_getUnitById(cmd->targets);
        u->flags &= ~BTL_UNIT_DODGED;
        if (u->flags & BTL_UNIT_PARTY) {
            btl_setIdleMotion(u);
        } else if ((u->flags >> BTL_UNIT_MODEL_PART_BIT) & 1) {
            u = btl_findBodyUnit();
        }
        u->pos[2] = u->home->pos[2];
    }
    btl_clearCmd_inline(cmd);
    b->curCmd = NULL;
    b->cmdPhase = 0;
}

void btl_flushCmdQueue(Battle *battle) {
    BtlCmd *p = &battle->cmdQueue;
    BtlCmd *cmd;
    BtlUnit *u;
    BtlModel *model;
    Battle *b;
    s32 charIndex;
    while (p != NULL) {
        cmd = p->next;
        if (cmd != NULL && cmd->cmdNo != BTL_CMD_ENEMY_DEATH && cmd->cmdNo < BTL_CMD_REBIRTH_FLAME) {
            u = cmd->caster;
            b = g_gameState->gameData->ctx.battle;
            if (cmd->cmdNo < BTL_CMD_TABLE_COUNT && u != NULL && !(u->status & BTL_STATUS_KO)) {
                model = u->model;
                u->cmdIssued = 0;
                if ((s32)model->motion == u->motions[9] && ((charIndex = BTL_UNIT_INDEX(u->flags)) != CHR_EIKO || b->curCmd->cmdNo != BTL_CMD_DBL_WHT) && (charIndex != CHR_VIVI || (b->curCmd->cmdNo != BTL_CMD_SWD_MAG && b->curCmd->cmdNo != BTL_CMD_DBL_BLK))) {
                    model->motion = (BtlMotion *)u->motions[32];
                    u->obj->frame = 0;
                    u->flags &= ~BTL_UNIT_CMD_WAITING;
                }
            }
            btl_releaseItem(cmd);
            btl_unlinkCmd(&p->next, 1);
        } else {
            p = cmd;
        }
    }
}

void btl_clearCmdQueue(Battle *battle) {
    BtlCmd *p = &battle->cmdQueue;
    BtlCmd *cmd;
    BtlUnit *u;
    BtlModel *model;
    Battle *b;
    s32 charIndex;
    while (p != NULL) {
        cmd = p->next;
        if (cmd != NULL && cmd->cmdNo < BTL_CMD_TRANCE) {
            u = cmd->caster;
            b = g_gameState->gameData->ctx.battle;
            if (cmd->cmdNo < BTL_CMD_TABLE_COUNT && u != NULL && !(u->status & BTL_STATUS_KO)) {
                model = u->model;
                u->cmdIssued = 0;
                if ((s32)model->motion == u->motions[9] && ((charIndex = BTL_UNIT_INDEX(u->flags)) != CHR_EIKO || b->curCmd->cmdNo != BTL_CMD_DBL_WHT) && (charIndex != CHR_VIVI || (b->curCmd->cmdNo != BTL_CMD_SWD_MAG && b->curCmd->cmdNo != BTL_CMD_DBL_BLK))) {
                    model->motion = (BtlMotion *)u->motions[32];
                    u->obj->frame = 0;
                    u->flags &= ~BTL_UNIT_CMD_WAITING;
                }
            }
            btl_releaseItem(cmd);
            btl_unlinkCmd(&p->next, 1);
        } else {
            p = cmd;
        }
    }
    battle->eidolonTimer = 0;
    battle->eidolonAbility = 0;
    battle->cmdFlags &= ~(BTL_CMDF_EIDOLON_TIMER | BTL_CMDF_EIDOLON_QUEUED);
}

void btl_enqueueCmd(BtlCmd *cmd) {
    BtlCmd *p = &g_gameState->gameData->ctx.battle->cmdQueue;
    BtlCmd *next;
    s32 cond;
    u32 nextCmdNo;

    for (; p != NULL; p = p->next) {
        if (cmd->info.b.automatic) {
            next = p->next;
            if (next == NULL) {
                goto insert;
            }
            if (cmd->cmdNo >= BTL_CMD_KO || cmd->cmdNo == BTL_CMD_ENEMY_DEATH) {
                nextCmdNo = next->cmdNo;
                cond = nextCmdNo < 0x3C;
                goto chk;
            }
            if (cmd->cmdNo == BTL_CMD_TRANCE) {
                nextCmdNo = next->cmdNo;
                cond = nextCmdNo < 0x3B;
            chk:
                if (cond == 0 || nextCmdNo == 0x36) {
                    continue;
                }
            insert:
                btl_linkCmd((LinkNode *)cmd, (LinkNode **)&p->next);
                return;
            }
            cond = next->info.b.automatic;
        } else {
            cond = (s32)p->next;
        }
        if (cond == 0) {
            goto insert;
        }
    }
}

s32 btl_applyReflect(BtlCmd *cmd) {
    u16 candidates[4];
    s16 candCount;
    s16 reflectCount;
    s32 i;
    u32 reflectMask = 0;
    u32 side;
    BtlUnit *unit;

    if ((u32)(cmd->cmdNo - BTL_CMD_ITEM) >= 2 && cmd->cmdNo != BTL_CMD_AUTO_POTION && (cmd->ability->flags & BTL_ABIL_REFLECTABLE) &&
        !(cmd->caster->support[0] & SA_BIT(SA_REFLECT_NULL))) {
        candCount = 0;
        reflectCount = 0;
        unit = g_gameState->gameData->ctx.battle->unitList;
        side = (u16)cmd->targets >= BTL_TARGETS_FIRST_ENEMY;
        for (; unit != NULL; unit = unit->next) {
            if (unit->unitBit & (u16)cmd->targets) {
                if (!(unit->status & BTL_STATUS_PETRIFY) && (u8)btl_hasStatus(unit, BTL_STATUS_REFLECT)) {
                    reflectCount++;
                    reflectMask |= unit->unitBit;
                }
            } else if (!(unit->status & BTL_STATUS_KO) && (unit->flags & BTL_UNIT_PARTY) == side && ((unit->flags >> BTL_UNIT_TARGETABLE_BIT) & 1)) {
                candidates[candCount++] = unit->unitBit;
            }
        }
        if (reflectCount != 0 && candCount != 0) {
            for (i = 0; i < 4; i++) {
                if (i < reflectCount) {
                    cmd->caster->bounceMasks[i] = candidates[RAND8() % candCount];
                } else {
                    cmd->caster->bounceMasks[i] = 0;
                }
            }
            if ((u16)reflectMask == (u16)cmd->targets) {
                cmd->info.b.reflect = 1;
                cmd->targets = btl_orTargets(cmd->caster->bounceMasks);
            } else {
                cmd->info.b.reflect = 2;
                cmd->targets &= ~reflectMask;
            }
        }
    }
    return (u16)reflectMask;
}

void btl_drawActiveMarker(GameData *gd, Battle *battle, BtlUnit *unit) {
    SVECTOR pos;
    SVECTOR tri[3];
    long sxy[4];
    s32 angle;
    s32 i;

    angle = (battle->tick & 0xF) << 8;
    gte_SetRotMatrix(&gd->viewMatrix);
    gte_SetTransMatrix(&gd->viewMatrix);
    pos.vx = unit->pos[0] >> 16;
    pos.vy = (unit->pos[1] >> 16) - unit->model->header->height;
    pos.vz = unit->pos[2] >> 16;
    gte_ldv0(&pos);
    gte_rtps();
    gte_stsxy(&sxy[0]);
    for (i = 0; i < 3; i++) {
        tri[i].vx = pos.vx - ((math_cos(angle) * 68) >> 12);
        tri[i].vy = pos.vy - 0x85;
        tri[i].vz = pos.vz - ((math_sin(angle) * 68) >> 12);
        angle = (angle + 0x4F1) & 0xFFF;
    }
    gte_ldv3c(tri);
    gte_rtpt();
    gte_stsxy3(&sxy[1], &sxy[2], &sxy[3]);
    if (NormalClip(sxy[0], sxy[1], sxy[2]) > 0) {
        (battle->cursorPrims + gd->drawBuf)->faces[0].poly.x0 = sxy[0];
        (battle->cursorPrims + gd->drawBuf)->faces[0].poly.y0 = sxy[0] >> 16;
        (battle->cursorPrims + gd->drawBuf)->faces[0].poly.x1 = sxy[1];
        (battle->cursorPrims + gd->drawBuf)->faces[0].poly.y1 = sxy[1] >> 16;
        (battle->cursorPrims + gd->drawBuf)->faces[0].poly.x2 = sxy[2];
        (battle->cursorPrims + gd->drawBuf)->faces[0].poly.y2 = sxy[2] >> 16;
        addPrim(&gd->ot[gd->drawBuf][2], &(battle->cursorPrims + gd->drawBuf)->faces[0]);
    }
    if (NormalClip(sxy[0], sxy[2], sxy[3]) > 0) {
        (battle->cursorPrims + gd->drawBuf)->faces[1].poly.x0 = sxy[0];
        (battle->cursorPrims + gd->drawBuf)->faces[1].poly.y0 = sxy[0] >> 16;
        (battle->cursorPrims + gd->drawBuf)->faces[1].poly.x1 = sxy[2];
        (battle->cursorPrims + gd->drawBuf)->faces[1].poly.y1 = sxy[2] >> 16;
        (battle->cursorPrims + gd->drawBuf)->faces[1].poly.x2 = sxy[3];
        (battle->cursorPrims + gd->drawBuf)->faces[1].poly.y2 = sxy[3] >> 16;
        addPrim(&gd->ot[gd->drawBuf][2], &(battle->cursorPrims + gd->drawBuf)->faces[1]);
    }
    if (NormalClip(sxy[0], sxy[3], sxy[1]) > 0) {
        (battle->cursorPrims + gd->drawBuf)->faces[2].poly.x0 = sxy[0];
        (battle->cursorPrims + gd->drawBuf)->faces[2].poly.y0 = sxy[0] >> 16;
        (battle->cursorPrims + gd->drawBuf)->faces[2].poly.x1 = sxy[3];
        (battle->cursorPrims + gd->drawBuf)->faces[2].poly.y1 = sxy[3] >> 16;
        (battle->cursorPrims + gd->drawBuf)->faces[2].poly.x2 = sxy[1];
        (battle->cursorPrims + gd->drawBuf)->faces[2].poly.y2 = sxy[1] >> 16;
        addPrim(&gd->ot[gd->drawBuf][2], &(battle->cursorPrims + gd->drawBuf)->faces[2]);
    }
    if (NormalClip(sxy[1], sxy[3], sxy[2]) > 0) {
        (battle->cursorPrims + gd->drawBuf)->faces[3].poly.x0 = sxy[1];
        (battle->cursorPrims + gd->drawBuf)->faces[3].poly.y0 = sxy[1] >> 16;
        (battle->cursorPrims + gd->drawBuf)->faces[3].poly.x1 = sxy[3];
        (battle->cursorPrims + gd->drawBuf)->faces[3].poly.y1 = sxy[3] >> 16;
        (battle->cursorPrims + gd->drawBuf)->faces[3].poly.x2 = sxy[2];
        (battle->cursorPrims + gd->drawBuf)->faces[3].poly.y2 = sxy[2] >> 16;
        addPrim(&gd->ot[gd->drawBuf][2], &(battle->cursorPrims + gd->drawBuf)->faces[3]);
    }
}

void btl_autoAttack(BtlUnit *unit) {
    s32 side;
    if (unit->status & BTL_STATUS_CONFUSE) {
        /* rand8_inline() written out: calling it changes the register allocation here */
        if (g_randPtr >= g_randTableEnd) {
            rand_refill();
        }
        side = *g_randPtr++;
        side &= 1;
    } else if (unit->status & BTL_STATUS_BERSERK) {
        side = 0;
    } else {
        return;
    }
    btl_setCmd((s32)unit->cmd, BTL_CMD_ATTACK, ABIL_ATTACK, btl_pickRandomUnit(side) & 0xFFFF, 0);
}

void btl_onCmdEffectDone(void) {
    Battle *b = g_gameState->gameData->ctx.battle;
    BtlCmd *c = b->curCmd;
    s8 k = c->info.b.reflect;
    BtlUnit *u;
    if (k == 2) {
        c->targets = btl_orTargets(c->caster->bounceMasks);
        btl_playCmdEffect(c, c->ability->effect, 0);
        b->cmdPhase = k;
        return;
    }
    u = c->caster;
    if (!(u->status & BTL_STATUS_AUTO_LIFE) || c->cmdNo != BTL_CMD_KO || (s32)u->model->motion == u->motions[4]) {
        b->cmdPhase = 4;
    }
}

void btl_clearCmd(BtlCmd *cmd) {
    btl_clearCmd_inline(cmd);
}

void btl_queueAutoCmd(BtlUnit *unit, s32 cmdNo, s32 subNo, s32 targets) {
    if (!(unit->status & (BTL_STATUS_PETRIFY | BTL_STATUS_VENOM | BTL_STATUS_STOP | BTL_STATUS_SLEEP | BTL_STATUS_FREEZE)) && g_gameState->gameData->ctx.battle->phase == BTL_PHASE_COMBAT) {
        btl_setCmd(unit->autoCmd, cmdNo, subNo, targets & 0xFFFF, 0);
    }
}

void btl_cancelTranceEidolon(BtlUnit *unit) {
    if ((unit->flags & (BTL_UNIT_INDEX_MASK | BTL_UNIT_PARTY)) == (CHR_DAGGER << BTL_UNIT_INDEX_SHIFT | BTL_UNIT_PARTY)) {
        Battle *b = g_gameState->gameData->ctx.battle;
        BtlCmd *p;
        for (p = &b->cmdQueue; p != NULL; p = p->next) {
            BtlCmd *n = p->next;
            if (n != NULL && n->caster == unit && n->cmdNo == BTL_CMD_TRANCE_EIDOLON) {
                btl_unlinkCmd(&p->next, 1);
                break;
            }
        }
        b->eidolonTimer = 0;
        b->eidolonAbility = 0;
        b->cmdFlags &= ~(BTL_CMDF_EIDOLON_TIMER | BTL_CMDF_EIDOLON_QUEUED);
    }
}

u8 btl_isCmdPending(BtlCmd *cmd) {
    BtlCmd *p;
    for (p = &g_gameState->gameData->ctx.battle->cmdQueue; p != NULL; p = p->next) {
        if (cmd == p) {
            return 1;
        }
    }
    p = g_gameState->gameData->ctx.battle->curCmd;
    if (p != NULL && p == cmd) {
        return 1;
    }
    return 0;
}

u8 btl_isCmdQueued(BtlUnit *unit, u8 cmdNo) {
    BtlCmd *p;
    for (p = &g_gameState->gameData->ctx.battle->cmdQueue; p != NULL; p = p->next) {
        if (p->caster == unit && p->cmdNo == cmdNo) {
            return 1;
        }
    }
    p = g_gameState->gameData->ctx.battle->curCmd;
    if (p != NULL && p->cmdNo == cmdNo) {
        return 1;
    }
    return 0;
}

void btl_removeCmd(BtlCmd *cmd) {
    BtlUnit *u = cmd->caster;
    BtlCmd *p;
    for (p = &g_gameState->gameData->ctx.battle->cmdQueue; p != NULL; p = p->next) {
        if (p->next == cmd) {
            if (u != NULL && cmd != u->delayedCmd && cmd != u->tranceCmd) {
                u->flags &= ~BTL_UNIT_CMD_WAITING;
            }
            btl_unlinkCmd(&p->next, 0);
            return;
        }
    }
}

s32 btl_cancelUnitCmds(BtlUnit *unit) {
    BtlCmd *cmd;
    BtlCmd *p;
    s32 cancelled;
    unit->flags &= ~BTL_UNIT_CMD_WAITING;
    p = &g_gameState->gameData->ctx.battle->cmdQueue;
    cancelled = 0;
    while (p != NULL) {
        cmd = p->next;
        if (cmd != NULL && cmd->caster == unit && cmd->cmdNo < BTL_CMD_ENEMY_DEATH) {
            if (cmd->cmdNo < BTL_CMD_TABLE_COUNT) {
                cancelled = 1;
            }
            btl_releaseItem(cmd);
            btl_unlinkCmd(&p->next, 1);
        } else {
            p = cmd;
        }
    }
    return cancelled;
}

void btl_removeUnitCmds(BtlUnit *unit) {
    BtlCmd *p = &g_gameState->gameData->ctx.battle->cmdQueue;
    while (p != NULL) {
        BtlCmd *n = p->next;
        if (n != NULL && n->caster == unit) {
            btl_releaseItem(n);
            btl_unlinkCmd(&p->next, 1);
        } else {
            p = n;
        }
    }
}

void btl_removeQueuedCmd(BtlUnit *unit, u8 cmdNo) {
    BtlCmd *p;
    for (p = &g_gameState->gameData->ctx.battle->cmdQueue; p != NULL; p = p->next) {
        BtlCmd *n = p->next;
        if (n != NULL && n->caster == unit && n->cmdNo == cmdNo) {
            btl_unlinkCmd(&p->next, 1);
            return;
        }
    }
}

void btl_projectUnitPos(BtlUnit *unit) {
    DVECTOR sxy;
    SVECTOR v;
    BtlBone *bone;

    if (!((unit->flags >> BTL_UNIT_HIDDEN_BIT) & 1)) {
        gte_SetRotMatrix(&g_gameState->gameData->viewMatrix);
        gte_SetTransMatrix(&g_gameState->gameData->viewMatrix);
        bone = &unit->model->bones[unit->baseBone];
        v.vx = bone->tx;
        v.vy = bone->ty;
        v.vz = bone->tz;
        gte_ldv0(&v);
        gte_rtps();
        gte_stsxy(&sxy);
        unit->screenX = sxy.vx;
        unit->screenY = sxy.vy;
    }
    unit->onScreen = 1;
}

void btl_unlinkCmd(BtlCmd **link, u8 cancel) {
    BtlCmd *p = *link;
    *link = p->next;
    p->info.b.unk0_2 = 0;
    p->info.b.automatic = 0;
    if (cancel && p->cmdNo == BTL_CMD_ESCAPE) {
        g_gameState->gameData->ctx.battle->cmdFlags &= ~BTL_CMDF_ESCAPE;
    }
}

s32 btl_payMpCost(BtlCmd *cmd) {
    s32 mp = cmd->ability->mpCost;
    if (g_evtGlobalVars.b[EVT_GB_SUMMON_MP_X4] != 0 && (cmd->ability->flags2 & BTL_ABIL2_SUMMON)) {
        mp <<= 2;
    }
    if (cmd->cmdNo == BTL_CMD_RETURN_MAGIC) {
        return 1;
    }
    if (btl_consumeMp(cmd->caster, mp) & 0xFF) {
        return 1;
    }
    btl_showMessage(BTL_MSG_NOT_ENOUGH_MP);
    return 0;
}

void btl_linkCmd(LinkNode *node, LinkNode **link) {
    node->next = *link;
    *link = node;
    node->flags = (node->flags & ~0xC) | 4;
}

void btl_releaseItem(BtlCmd *cmd) {
    u8 cmdNo = cmd->cmdNo;
    if ((u32)(cmdNo - 0xE) < 2 || cmdNo == 0x33) {
        btl_unreserveItem(cmd->subNo);
    }
}

u16 btl_orTargets(u16 *masks) {
    s32 i;
    u16 targets;
    targets = i = 0;
    for (; i < 4; i++) {
        targets |= masks[i];
    }
    return targets;
}

void btl_clearBounceMasks(Unk800B44A0 *unit) {
    s32 i;
    for (i = 3; i >= 0; i--) {
        unit->bounceMasks[i] = 0;
    }
}

void btl_calcCmd(BtlUnit *caster, BtlUnit *target, BtlCmd *cmd, s32 type) {
    BtlCalc c;
    BtlCmd sub;
    BtlCmd sub2;

    c.caster = caster;
    c.target = target;
    c.cmd = (BtlCalc8 *)cmd;
    c.flags = 0;
    c.casterResult = c.targetResult = 0;
    c.casterHpChange = c.targetHpChange = c.casterMpChange = c.targetMpChange = 0;
    switch ((u8)type) {
    case BTL_FORMULA_WEAPON_STR:
    case BTL_FORMULA_WEAPON_STR_SPR:
    case BTL_FORMULA_WEAPON_STR_SPD:
    case BTL_FORMULA_WEAPON_STR_MAG:
    case BTL_FORMULA_WEAPON_RANDOM:
    case BTL_FORMULA_WEAPON_STR_LEVEL:
        if (btl_checkFrozenShatter(&c)) {
            btl_initHitRates(&c);
            if (btl_hasSureHit(&c) || btl_rollHit(&c)) {
                btl_setPhysParams(&c, type);
                btl_applyKillerBonus(&c);
                btl_applyCasterStatusMod(&c);
                btl_randomizeDefense(&c);
                btl_applyTargetStatusMod(&c);
                if ((caster->status & BTL_STATUS_TRANCE) && BTL_UNIT_INDEX(caster->flags) == CHR_STEINER) {
                    c.multiplier *= 2;
                }
                btl_applyRowMod(&c);
                btl_applyWeaponElementBoost(&c);
                if (btl_applyWeaponElement(&c)) {
                    btl_rollCritical(&c);
                    btl_applyInvertedPower((Unk800BA938 *)&c);
                    btl_finishPhysDamage(&c);
                    btl_markTrouble(&c);
                    btl_payMpAttack(&c);
                    if (c.caster->support[0] & SA_BIT(SA_ADD_STATUS)) {
                        c.statusRate = caster->weapon[7];
                        RAND_REFILL2();
                        if (RAND16() % 100 < c.statusRate) {
                            c.flags |= BTL_CALC_WEAPON_STATUS;
                        }
                    }
                }
            }
        }
        break;
    case BTL_FORMULA_WEAPON_DRAIN:
        {
            s32 v;
            s32 d;
            if (btl_checkNotSelf(&c) && btl_checkTargetAlive(&c) && btl_checkFrozenShatter(&c)) {
                btl_initHitRates(&c);
                if (btl_hasSureHit(&c) || btl_rollHit(&c)) {
                    btl_setPhysParams(&c, BTL_FORMULA_WEAPON_STR);
                    btl_applyKillerBonus(&c);
                    btl_applyCasterStatusMod(&c);
                    btl_randomizeDefense(&c);
                    btl_applyTargetStatusMod(&c);
                    btl_applyRowMod(&c);
                    btl_rollCritical(&c);
                    btl_applyInvertedPower((Unk800BA938 *)&c);
                    btl_setDrainFlags(&c);
                    d = c.power - c.defense;
                    if (d <= 0) {
                        d = 1;
                    }
                    if (c.multiplier <= 0) {
                        c.multiplier = 1;
                    }
                    v = d * c.multiplier;
                    if (v >= 10000) {
                        v = 9999;
                    }
                    c.casterHpChange = v;
                    c.targetHpChange = v;
                    btl_payMpAttack(&c);
                }
            }
            break;
        }
    case BTL_FORMULA_PHYSICAL:
        if (btl_checkFrozenShatter(&c)) {
            btl_initHitRates(&c);
            if (btl_rollHit(&c)) {
                btl_setAbilityPhysParams(&c);
                btl_applyCasterStatusMod(&c);
                btl_randomizeDefense(&c);
                btl_applyTargetStatusMod(&c);
                btl_applyRowMod(&c);
                btl_applyAbilityElementBoost(&c);
                if (btl_applyAbilityElement(&c)) {
                    btl_rollCritical(&c);
                    btl_finishPhysDamage(&c);
                    btl_markTrouble(&c);
                    btl_rollAbilityStatus(&c);
                }
            }
        }
        break;
    case BTL_FORMULA_MAGIC:
        btl_setMagicParams(&c);
        btl_applyCasterMiniMagic(&c);
        btl_applyTargetShell(&c);
        btl_halveForSpread(&c);
        btl_applyAbilityElementBoost(&c);
        if (btl_applyAbilityElement(&c) && btl_missIfAirborne(&c)) {
            btl_finishMagicDamage(&c);
            btl_rollAbilityStatus(&c);
        }
        break;
    case BTL_FORMULA_MAGIC_HEAL:
        btl_setMagicParams(&c);
        btl_applyHealBoost(&c);
        btl_applyCasterMiniMagic(&c);
        btl_halveForSpread(&c);
        btl_calcHeal(&c);
        break;
    case BTL_FORMULA_ADD_STATUS_GUARD:
        if (btl_guardIfDeathImmune(&c)) {
            btl_initStatusHitRate(&c);
            btl_halveHitForShell(&c);
            btl_halveHitForSpread(&c);
            if (btl_rollStatusHit(&c)) {
                btl_applyAbilityStatus(&c);
            }
        }
        break;
    case BTL_FORMULA_CURE_STATUS:
        btl_cureAbilityStatus(&c);
        break;
    case BTL_FORMULA_REVIVE:
        {
            s32 v;
            if (btl_checkRevivable(&c) && (btl_initUndeadHitRate(&c) || btl_rollStatusHit(&c))) {
                if (!(target->status & BTL_STATUS_ZOMBIE) && btl_hasEnemyFlag(target, BTL_ENEMY_UNDEAD) == 0) {
                    if (btl_checkTargetIsPlayer(&c)) {
                        v = target->max->maxHp * (target->spirit + cmd->ability->power);
                        if (caster->support[1] & SA_BIT(SA_CONCENTRATE)) {
                            v = v / 50;
                        } else {
                            v = v / 100;
                        }
                        if (v >= 10000) {
                            v = 9999;
                        }
                        c.targetHpChange = v;
                        c.targetResult |= (BTL_POPUP_HP | BTL_POPUP_HEAL);
                        btl_cureAbilityStatus(&c);
                    }
                } else {
                    btl_killUnit(target);
                }
            }
            break;
        }
    case BTL_FORMULA_DEATH:
        if (btl_guardIfDeathImmune(&c)) {
            if ((target->status & BTL_STATUS_ZOMBIE) || btl_hasEnemyFlag(target, BTL_ENEMY_UNDEAD) != 0) {
                if (btl_checkTargetAlive(&c)) {
                    target->hp = target->max->maxHp;
                }
            } else {
                btl_initStatusHitRate(&c);
                if (btl_rollStatusHit(&c)) {
                    btl_applyAbilityStatus(&c);
                }
            }
        }
        break;
    case BTL_FORMULA_OSMOSE:
        {
            s32 v;
            s32 d;
            if (btl_checkNotSelf(&c) && btl_checkTargetAlive(&c)) {
                btl_setMagicParams(&c);
                btl_applyCasterMiniMagic(&c);
                btl_applyTargetShell(&c);
                v = 0;
                c.targetResult |= BTL_POPUP_MP;
                d = c.power - c.defense;
                c.casterResult |= BTL_POPUP_MP;
                if (d > 0) {
                    if (c.multiplier <= 0) {
                        c.multiplier = 1;
                    }
                    v = d * c.multiplier >> 2;
                    if (v >= 10000) {
                        v = 9999;
                    }
                }
                if ((target->status & BTL_STATUS_ZOMBIE) || btl_hasEnemyFlag(target, BTL_ENEMY_UNDEAD) != 0) {
                    c.targetResult |= BTL_POPUP_MP_HEAL;
                    if (caster->mp < v) {
                        v = caster->mp;
                    }
                } else {
                    c.casterResult |= BTL_POPUP_MP_HEAL;
                    if (target->mp < v) {
                        v = target->mp;
                    }
                }
                c.casterMpChange = v;
                c.targetMpChange = v;
            }
            break;
        }
    case BTL_FORMULA_DRAIN:
        {
            s32 v;
            s32 d;
            if (btl_checkNotSelf(&c) && btl_checkTargetAlive(&c)) {
                btl_setMagicParams(&c);
                btl_applyCasterMiniMagic(&c);
                btl_applyTargetShell(&c);
                btl_setDrainFlags(&c);
                d = c.power - c.defense;
                if (d > 0) {
                    if (c.multiplier <= 0) {
                        c.multiplier = 1;
                    }
                    v = d * c.multiplier;
                    if (v >= 10000) {
                        v = 9999;
                    }
                    c.casterHpChange = v;
                    c.targetHpChange = v;
                }
            }
            break;
        }
    case BTL_FORMULA_PERCENT_MAX_HP:
    case BTL_FORMULA_PERCENT_CUR_HP:
        {
            s32 v;
            if (btl_missIfDeathImmune(&c)) {
                btl_initStatusHitRate(&c);
                btl_halveHitForShell(&c);
                btl_halveHitForSpread(&c);
                if (btl_rollStatusHit(&c)) {
                    btl_setMultiplierFromPower(&c);
                    btl_applyAbilityElementBoost(&c);
                    if (btl_applyAbilityElement(&c) && btl_missIfAirborne(&c)) {
                        if ((u8)type == BTL_FORMULA_PERCENT_MAX_HP) {
                            btl_calcPercentDamage(&c);
                        } else {
                            if (c.multiplier > 100) {
                                c.multiplier = 100;
                            }
                            v = target->hp * c.multiplier / 100;
                            if (v >= 10000) {
                                v = 9999;
                            }
                            c.targetResult |= BTL_POPUP_HP;
                            if (c.flags & BTL_CALC_ABSORB) {
                                c.targetResult |= BTL_POPUP_HEAL;
                            }
                            c.targetHpChange = v;
                        }
                    }
                }
            }
            break;
        }
    case BTL_FORMULA_RANDOM_MAGIC:
        if ((cmd->ability->effect != 0x38 || RAND8() < 0xAB) && (cmd->subNo != ABIL_METEOR || !cmd->info.b.magicFailed)) {
            RAND_REFILL2();
            c.multiplier = RAND16() % (caster->magic + caster->level);
            c.power = cmd->ability->power;
            c.defense = 0;
            btl_applyCasterMiniMagic(&c);
            btl_applyTargetShell(&c);
            btl_halveForSpread(&c);
            btl_applyAbilityElementBoost(&c);
            if (btl_applyAbilityElement(&c) && btl_missIfAirborne(&c)) {
                btl_finishMagicDamage(&c);
                btl_rollAbilityStatus(&c);
            }
        } else {
            c.flags |= BTL_CALC_MISS;
        }
        break;
    case BTL_FORMULA_WEAPON_SKILL:
        btl_setWeaponAbilityParams(&c);
        btl_applyCasterStatusMod(&c);
        btl_applyTargetStatusMod(&c);
        btl_applyAbilityElementBoost(&c);
        if (btl_applyWeaponElement(&c)) {
            btl_finishMagicDamage(&c);
            btl_rollAbilityStatus(&c);
        }
        break;
    case BTL_FORMULA_WEAPON_SKILL_MDEF:
        {
            s32 i;
            RAND_REFILL2();
            i = caster->strength;
            c.multiplier = i + RAND16() % (((caster->level + i) >> 3) + 1);
            c.power = caster->weapon[5] * cmd->ability->power / 10;
            c.defense = target->magicDef;
            btl_applyCasterMiniMagic(&c);
            btl_applyTargetShell(&c);
            btl_applyAbilityElementBoost(&c);
            if (btl_applyAbilityElement(&c) && btl_missIfAirborne(&c)) {
                btl_finishMagicDamage(&c);
                btl_rollAbilityStatus(&c);
            }
            break;
        }
    case BTL_FORMULA_GOBLIN_PUNCH:
        btl_setMagicParams(&c);
        if (target->level == caster->level) {
            c.multiplier += caster->level;
            c.defense = 0;
        }
        btl_applyCasterMiniMagic(&c);
        btl_applyTargetShell(&c);
        btl_finishMagicDamage(&c);
        break;
    case BTL_FORMULA_LEVEL_SET_HP:
        if (btl_checkLevelMultiple(&c) && btl_checkTargetAlive(&c)) {
            btl_setTargetHp(&c);
        }
        break;
    case BTL_FORMULA_LEVEL_MAGIC:
        if (btl_checkLevelMultiple(&c) && btl_checkTargetAlive(&c)) {
            btl_setMagicParams(&c);
            btl_applyCasterMiniMagic(&c);
            btl_applyTargetShell(&c);
            btl_applyAbilityElementBoost(&c);
            if (btl_applyAbilityElement(&c)) {
                btl_finishMagicDamage(&c);
            }
        }
        break;
    case BTL_FORMULA_LEVEL_DEFLESS:
        if (btl_checkLevelMultiple(&c) && btl_checkTargetAlive(&c)) {
            if (target->defense != 0) {
                RAND_REFILL2();
                target->defense = RAND16() % target->defense;
            }
            if (target->magicDef != 0) {
                RAND_REFILL2();
                target->magicDef = RAND16() % target->magicDef;
            }
        }
        break;
    case BTL_FORMULA_SET_HP:
        if (btl_missIfDeathImmune(&c) && btl_checkTargetAlive(&c)) {
            btl_setTargetHp(&c);
        }
        break;
    case BTL_FORMULA_SET_HP_ROLL:
        if (btl_missIfDeathImmune(&c) && btl_checkTargetAlive(&c)) {
            btl_initStatusHitRate(&c);
            btl_halveHitForShell(&c);
            if (btl_rollStatusHit(&c)) {
                btl_setTargetHp(&c);
            }
        }
        break;
    case BTL_FORMULA_FIXED_DAMAGE_1HP:
        if (caster->hp == 1) {
            c.targetResult |= BTL_POPUP_HP;
            c.targetHpChange = cmd->ability->rate + cmd->ability->power * 100;
        } else {
            c.flags |= BTL_CALC_MISS;
        }
        break;
    case BTL_FORMULA_FIXED_DAMAGE:
        c.targetResult |= BTL_POPUP_HP;
        c.targetHpChange = cmd->ability->rate + cmd->ability->power * 100;
        break;
    case BTL_FORMULA_HP_LOST:
        c.targetResult |= BTL_POPUP_HP;
        c.targetHpChange = caster->max->maxHp - caster->hp;
        break;
    case BTL_FORMULA_THIRD_MAX_HP_HEAL:
        c.targetResult |= BTL_POPUP_HP;
        if (!(target->status & BTL_STATUS_ZOMBIE)) {
            c.targetResult |= BTL_POPUP_HEAL;
        }
        c.targetHpChange = caster->max->maxHp / 3;
        break;
    case BTL_FORMULA_MP_DAMAGE_RANDOM:
        c.targetResult |= BTL_POPUP_MP;
        if (target->mp != 0) {
            RAND_REFILL2();
            c.targetMpChange = RAND16() % target->mp;
            if (c.targetMpChange >= 10000) {
                c.targetMpChange = 9999;
            }
        }
        break;
    case BTL_FORMULA_DARKSIDE:
        {
            s32 v;
            s32 d;
            btl_setWeaponAbilityParams(&c);
            btl_applyAbilityElementBoost(&c);
            if (btl_applyAbilityElement(&c)) {
                c.targetResult |= BTL_POPUP_HP;
                d = c.power - c.defense;
                if (d > 0) {
                    if (c.multiplier <= 0) {
                        c.multiplier = 1;
                    }
                    v = d * c.multiplier;
                    c.casterResult |= BTL_POPUP_HP;
                    c.casterHpChange = caster->max->maxHp >> 3;
                    if (v >= 10000) {
                        v = 9999;
                    }
                    if (c.flags & BTL_CALC_ABSORB) {
                        c.targetResult |= BTL_POPUP_HEAL;
                    }
                    c.targetHpChange = v;
                }
            }
            break;
        }
    case BTL_FORMULA_ARMOR_BREAK:
    case BTL_FORMULA_POWER_BREAK:
    case BTL_FORMULA_MENTAL_BREAK:
    case BTL_FORMULA_MAGIC_BREAK:
        btl_initStatusHitRate(&c);
        btl_halveHitForShell(&c);
        if (btl_rollStatusHit(&c)) {
            if ((u8)type == BTL_FORMULA_ARMOR_BREAK) {
                target->defense >>= 1;
            } else if ((u8)type == BTL_FORMULA_POWER_BREAK) {
                target->strength = target->strength * 3 / 4;
            } else if ((u8)type == BTL_FORMULA_MENTAL_BREAK) {
                target->magicDef >>= 1;
            } else if ((u8)type == BTL_FORMULA_MAGIC_BREAK) {
                target->magic = target->magic * 3 / 4;
            }
        }
        break;
    case BTL_FORMULA_PERCENT_HEAL:
        c.targetResult |= (BTL_POPUP_HP | BTL_POPUP_HEAL | BTL_POPUP_MP | BTL_POPUP_MP_HEAL);
        if (caster->support[0] & SA_BIT(SA_POWER_UP)) {
            c.targetHpChange = target->max->maxHp * cmd->ability->power / 50;
            c.targetMpChange = target->max->maxMp * cmd->ability->power / 50;
        } else {
            c.targetHpChange = target->max->maxHp * cmd->ability->power / 100;
            c.targetMpChange = target->max->maxMp * cmd->ability->power / 100;
        }
        break;
    case BTL_FORMULA_SPARE_CHANGE:
        {
            u32 v;
            u32 d;
            v = cmd->ability->power * caster->level;
            d = g_gameState->gameData->gil + v;
            c.targetResult |= BTL_POPUP_HP;
            v = v * v / 10;
            v = v * caster->spirit / d;
            if (cmd->cmdNo == BTL_CMD_ELAN) {
                v = v / btl_countActiveUnits(0);
            }
            if (v < 10000) {
                c.targetHpChange = v;
            } else {
                c.targetHpChange = 9999;
            }
            break;
        }
    case BTL_FORMULA_LANCER:
        {
            s32 v;
            s32 d;
            btl_setWeaponAbilityParams(&c);
            btl_applyCasterStatusMod(&c);
            btl_applyTargetStatusMod(&c);
            c.targetResult |= (BTL_POPUP_HP | BTL_POPUP_MP);
            d = c.power - c.defense;
            if (d > 0) {
                if (c.multiplier <= 0) {
                    c.multiplier = 1;
                }
                v = d * c.multiplier;
                if (v >= 10000) {
                    v = 9999;
                }
                c.targetHpChange = v;
                c.targetMpChange = v >> 4;
            }
            break;
        }
    case BTL_FORMULA_DRAGON_BREATH:
        {
            s32 v;
            BtlUnit *u;
            v = target->max->maxHp - target->hp;
            c.targetResult |= BTL_POPUP_HP;
            if (v >= 10000) {
                v = 9999;
            }
            c.targetHpChange = v;
            for (u = g_gameState->gameData->ctx.battle->unitList; u != NULL; u = u->next) {
                if ((u->flags & (BTL_UNIT_PARTY | BTL_UNIT_TARGETABLE)) == BTL_UNIT_TARGETABLE) {
                    c.target = u;
                    btl_applyCalcResult(&c);
                    btl_spawnPopups(u);
                }
            }
            return;
        }
    case BTL_FORMULA_WHITE_DRAW:
        {
            BtlUnit *u;
            if (!(target->status & BTL_STATUS_KO)) {
                c.casterResult = (BTL_POPUP_MP | BTL_POPUP_MP_HEAL);
                RAND_REFILL2();
                c.casterMpChange = RAND16() % (target->level * 2);
                for (u = g_gameState->gameData->ctx.battle->unitList; u != NULL; u = u->next) {
                    if ((u->flags & (BTL_UNIT_PARTY | BTL_UNIT_TARGETABLE)) == (BTL_UNIT_PARTY | BTL_UNIT_TARGETABLE)) {
                        c.caster = u;
                        btl_applyCalcResult(&c);
                        btl_spawnPopups(u);
                    }
                }
                return;
            }
            c.flags |= BTL_CALC_MISS;
            break;
        }
    case BTL_FORMULA_THROW:
        {
            s32 i;
            if (btl_checkFrozenShatter(&c)) {
                RAND_REFILL2();
                i = caster->strength;
                c.multiplier = i + RAND16() % (((caster->level + i) >> 3) + 1);
                c.power = g_weaponTable[cmd->subNo].attack * 2;
                c.defense = target->defense;
                if (caster->support[0] & SA_BIT(SA_POWER_THROW)) {
                    c.multiplier = c.multiplier * 3 >> 1;
                }
                btl_applyCasterStatusMod(&c);
                btl_applyTargetStatusMod(&c);
                if (btl_hasEnemyFlag(target, BTL_ENEMY_AERIAL) != 0) {
                    c.multiplier = c.multiplier * 3 >> 1;
                }
                btl_finishPhysDamage(&c);
            }
            break;
        }
    case BTL_FORMULA_STRENGTH_UP:
        target->strength += target->strength / cmd->ability->power;
        if (target->strength >= 100) {
            target->strength = 99;
        }
        break;
    case BTL_FORMULA_MAGIC_UP:
        target->magic += target->magic / cmd->ability->power;
        if (target->magic >= 100) {
            target->magic = 99;
        }
        break;
    case BTL_FORMULA_SACRIFICE:
        if (!(target->status & BTL_STATUS_KO) && !(caster->status & (BTL_STATUS_PETRIFY | BTL_STATUS_KO))) {
            target->hp = target->max->maxHp;
            target->mp = target->max->maxMp;
            caster->mp = 0;
            btl_killUnit(caster);
        } else {
            c.flags |= BTL_CALC_MISS;
        }
        break;
    case BTL_FORMULA_SOUL_BLADE:
        {
            s32 v;
            s32 d;
            if ((u32)(g_gameState->gameData->characters[BTL_UNIT_INDEX(caster->flags)].equip[0] - 7) < 9) {
                v = g_gameState->gameData->ctx.battle->statusSets[caster->weapon[1]];
                if (!(v & BTL_STATUS_KO) || btl_guardIfDeathImmune(&c)) {
                    d = btl_addStatuses(target, v);
                    if (d == 0) {
                        c.flags |= BTL_CALC_GUARD;
                    } else if (d == 1) {
                        c.flags |= BTL_CALC_MISS;
                    }
                }
            } else {
                c.flags |= BTL_CALC_MISS;
            }
            break;
        }
    case BTL_FORMULA_SPEAR:
    case BTL_FORMULA_SPEAR_TRANCE:
        {
            s32 v;
            s32 d;
            btl_setPhysParams(&c, (u8)type == BTL_FORMULA_SPEAR ? BTL_FORMULA_WEAPON_STR : BTL_FORMULA_WEAPON_RANDOM);
            if (caster->support[0] & SA_BIT(SA_HIGH_JUMP)) {
                c.multiplier *= 2;
            } else {
                c.multiplier = c.multiplier * 3 >> 1;
            }
            btl_applyCasterMiniMagic(&c);
            btl_applyTargetStatusMod(&c);
            if ((u8)type == BTL_FORMULA_SPEAR) {
                btl_finishPhysDamage(&c);
            } else {
                d = c.power - c.defense;
                if (d <= 0) {
                    d = 1;
                }
                c.targetResult |= BTL_POPUP_HP;
                if (c.multiplier < 4) {
                    c.multiplier = 4;
                }
                v = (u32)(d * c.multiplier) / btl_countActiveUnits(0);
                if (v >= 10000) {
                    v = 9999;
                }
                c.targetHpChange = v;
            }
            break;
        }
    case BTL_FORMULA_PHOENIX:
        {
            s32 i;
            btl_clearCmd(&sub);
            sub.cmdNo = cmd->cmdNo;
            sub.info.b.shortSummon = cmd->info.b.shortSummon;
            if (target->flags & BTL_UNIT_PARTY) {
                sub.subNo = ABIL_REBIRTH_FLAME;
                i = BTL_FORMULA_REVIVE;
            } else {
                sub.subNo = ABIL_PHOENIX;
                i = BTL_FORMULA_MAGIC;
            }
            sub.ability = (BtlCmd8 *)&g_gameState->gameData->ctx.battle->abilities[sub.subNo];
            btl_calcCmd(caster, target, &sub, i);
            return;
        }
    case BTL_FORMULA_SIX_DRAGONS:
        {
            s8 v;
            if (btl_checkTargetAlive(&c)) {
                RAND_REFILL2();
                v = RAND16() % 100;
                if (v < 10) {
                    target->hp = target->max->maxHp;
                    target->mp = target->max->maxMp;
                } else if (v < 30) {
                    target->hp = target->max->maxHp;
                } else if (v < 50) {
                    target->mp = target->max->maxMp;
                } else if (v < 65) {
                    target->hp = 1;
                } else if (v < 80) {
                    target->mp = 1;
                } else {
                    target->hp = 1;
                    target->mp = 1;
                }
            }
            break;
        }
    case BTL_FORMULA_CURSE:
        target->elemWeak |= cmd->ability->element;
        target->elemGuard &= ~cmd->ability->element;
        target->elemAbsorb &= ~cmd->ability->element;
        target->elemHalf &= ~cmd->ability->element;
        btl_showMessage(BTL_MSG_WEAK_AGAINST, btl_getElementName(cmd->ability->element));
        break;
    case BTL_FORMULA_USE_ITEM:
        if (item_getCount(cmd->ability->power) != 0) {
            btl_clearCmd(&sub2);
            sub2.cmdNo = BTL_CMD_ITEM;
            sub2.subNo = cmd->ability->power;
            sub2.ability = (BtlCmd8 *)&g_gameState->gameData->ctx.battle->abilities[0];
            btl_calcCmd(caster, target, &sub2, g_usableItemTable[sub2.subNo - ITEM_FIRST_USABLE].formula);
            btl_useItem(sub2.subNo);
            return;
        }
        c.flags |= BTL_CALC_MISS;
        break;
    case BTL_FORMULA_LUCKY_SEVEN:
        {
            s32 v;
            c.targetResult |= BTL_POPUP_HP;
            v = caster->hp % 10;
            if (v == 7) {
                switch (RAND8() % 4) {
                case 0:
                    c.targetHpChange = 7;
                    break;
                case 1:
                    c.targetHpChange = 77;
                    break;
                case 2:
                    c.targetHpChange = 777;
                    break;
                default:
                    c.targetHpChange = 7777;
                    break;
                }
            } else {
                c.targetHpChange = 1;
            }
            break;
        }
    case BTL_FORMULA_WHATS_THAT:
        {
            s32 v;
            if ((target->flags & BTL_UNIT_PARTY) || !((g_gameState->gameData->ctx.battle->scene.flags >> BTL_SCENE_SCRIPT_START_BIT) & 1)) {
                v = (caster->flags & BTL_UNIT_PARTY) << 11;
                target->rot[1] = v;
                target->obj->rotY = v;
                if ((target->flags & BTL_UNIT_PARTY) && (&g_gameState->gameData->characters[BTL_UNIT_INDEX(target->flags)])->info.b.battleRow != BTL_UNIT_ROW(target->flags)) {
                    btl_toggleRow(target);
                }
            } else {
                c.flags |= BTL_CALC_MISS;
            }
            break;
        }
    case BTL_FORMULA_CHANGE_ROW:
        btl_toggleRow(target);
        break;
    case BTL_FORMULA_FLEE:
        if (btl_canFlee(&c)) {
            btl_rollFlee(&c);
        }
        break;
    case BTL_FORMULA_FLEE_SKILL:
        btl_tryEscape(0);
        if (btl_canFlee(&c)) {
            btl_setCmd(&g_gameState->gameData->ctx.battle->escapeCmd, BTL_CMD_ESCAPE, 0, BTL_TARGETS_PARTY, 1);
        }
        break;
    case BTL_FORMULA_STEAL:
        {
            s32 v;
            s16 d;
            s16 k;
            BtlEnemy *e;
            e = &g_gameState->gameData->ctx.battle->enemies[BTL_UNIT_INDEX(target->flags)];
            d = 0;
            for (k = 0; k < 4; k++) {
                if (e->steal.b[k] != 0xFF) {
                    d = 1;
                }
            }
            if (!d) {
                btl_showMessage(BTL_MSG_NOTHING_TO_STEAL);
                break;
            }
            if (!(caster->support[1] & SA_BIT(SA_BANDIT))) {
                c.hitRate = caster->level + caster->spirit;
                c.evade = target->level;
                RAND_REFILL2();
                RAND_REFILL2();
                v = ((*g_randPtr << 8) | *g_randPtr) % c.hitRate < ((*g_randPtr << 8) | *g_randPtr) % c.evade;
                g_randPtr++;
                if (v) {
                    btl_showMessage(BTL_MSG_STEAL_FAILED);
                    break;
                }
            }
            if (caster->support[0] & SA_BIT(SA_MASTER_THIEF)) {
                if (e->steal.b[3] != 0xFF && RAND8() < 32) {
                    btl_calcSteal(&c, (Unk800B7F20 *)e, 3);
                } else if (e->steal.b[2] != 0xFF && RAND8() < 32) {
                    btl_calcSteal(&c, (Unk800B7F20 *)e, 2);
                } else if (e->steal.b[1] != 0xFF && RAND8() < 64) {
                    btl_calcSteal(&c, (Unk800B7F20 *)e, 1);
                } else {
                    btl_calcSteal(&c, (Unk800B7F20 *)e, 0);
                }
            } else {
                if (RAND8() < 1) {
                    btl_calcSteal(&c, (Unk800B7F20 *)e, 3);
                } else if (RAND8() < 16) {
                    btl_calcSteal(&c, (Unk800B7F20 *)e, 2);
                } else if (RAND8() < 64) {
                    btl_calcSteal(&c, (Unk800B7F20 *)e, 1);
                } else {
                    btl_calcSteal(&c, (Unk800B7F20 *)e, 0);
                }
            }
            break;
        }
    case BTL_FORMULA_SCAN:
        if (btl_missIfDeathImmune(&c)) {
            btl_startScan((s32)target);
        }
        break;
    case BTL_FORMULA_DETECT:
        btl_startDetect(target);
        break;
    case BTL_FORMULA_CHARGE:
        {
            u16 v;
            u16 d;
            BtlUnit *u;
            if ((c.flags = btl_getUnitsWithStatus(0, BTL_STATUS_LOW_HP)) != 0) {
                d = 0;
                for (u = g_gameState->gameData->ctx.battle->unitList; u != NULL; u = u->next) {
                    if ((u->unitBit & c.flags) && !(u->status & (BTL_STATUS_PETRIFY | BTL_STATUS_VENOM | BTL_STATUS_KO | BTL_STATUS_CONFUSE | BTL_STATUS_BERSERK | BTL_STATUS_STOP | BTL_STATUS_SLEEP | BTL_STATUS_FREEZE | BTL_STATUS_JUMP))) {
                        d = 1;
                        v = btl_pickRandomUnit(0);
                        if (v == 0) {
                            return;
                        }
                        btl_queueAutoCmd(u, BTL_CMD_CHARGE_ATTACK, ABIL_ATTACK, v);
                    }
                }
                if (!d) {
                    c.flags = 0;
                }
            }
            if (c.flags == 0) {
                btl_showMessage(BTL_MSG_CHARGE_FAILED);
            }
            return;
        }
    case BTL_FORMULA_SOFT:
        if (btl_hasEnemyFlag(target, BTL_ENEMY_STONE) != 0) {
            if (btl_checkTargetAlive(&c)) {
                btl_killUnit(target);
                btl_showMessage(BTL_MSG_TOO_SOFT);
            }
        } else {
            btl_cureItemStatus(&c);
        }
        break;
    case BTL_FORMULA_SWORD_MAGIC:
        {
            s32 i;
            RAND_REFILL2();
            i = caster->strength;
            c.multiplier = i + RAND16() % (((caster->level + i) >> 3) + 1);
            c.power = caster->weapon[5] + cmd->ability->power;
            c.defense = target->defense;
            btl_applyAbilityElementBoost(&c);
            if (btl_applyAbilityElement(&c) && btl_missIfAirborne(&c)) {
                btl_finishMagicDamage(&c);
            }
            break;
        }
    case BTL_FORMULA_EAT:
        {
            u8 i;
            if (btl_missIfDeathImmune(&c) && btl_checkTargetAlive(&c) && btl_hasEnemyFlag(target, BTL_ENEMY_HUMANOID) == 0) {
                if (target->max->maxHp / cmd->ability->power < target->hp) {
                    btl_showMessage(BTL_MSG_CANT_EAT_YET);
                } else {
                    i = g_gameState->gameData->ctx.battle->enemies[BTL_UNIT_INDEX(target->flags)].type->eatAbility;
                    btl_killUnit(target);
                    if (i != 0 && !chr_isAbilityLearned(BTL_UNIT_INDEX(caster->flags), i)) {
                        chr_learnAbility(BTL_UNIT_INDEX(caster->flags), i);
                        btl_showMessage(BTL_MSG_LEARNED, btl_getAbilityName(i));
                    } else {
                        btl_showMessage(BTL_MSG_TASTE_BAD);
                    }
                }
            } else {
                btl_showMessage(BTL_MSG_CANT_EAT);
            }
            break;
        }
    case BTL_FORMULA_FROG_DROP:
        {
            s32 v;
            c.targetResult |= BTL_POPUP_HP;
            v = g_gameState->gameData->frogCount;
            if (v != 0) {
                v = v * caster->level;
                if (v >= 10000) {
                    c.targetHpChange = 9999;
                } else {
                    c.targetHpChange = v;
                }
            } else {
                c.targetHpChange = 1;
            }
            break;
        }
    case BTL_FORMULA_THIEVERY:
        {
            s32 v;
            c.targetResult |= BTL_POPUP_HP;
            v = g_gameState->gameData->stealCount * caster->speed / 2;
            if (v >= 10000) {
                c.targetHpChange = 9999;
            } else {
                c.targetHpChange = v;
            }
            break;
        }
    case BTL_FORMULA_DRAGONS_CREST:
        {
            c.targetResult |= BTL_POPUP_HP;
            if (g_gameState->gameData->dragonKills * g_gameState->gameData->dragonKills < 10000) {
                c.targetHpChange = (u16)g_gameState->gameData->dragonKills * (u16)g_gameState->gameData->dragonKills;
            } else {
                c.targetHpChange = 9999;
            }
            break;
        }
    case BTL_FORMULA_ITEM_HEAL:
    case BTL_FORMULA_ITEM_MP_HEAL:
        {
            s32 v;
            c.multiplier = 15;
            c.power = g_usableItemTable[cmd->subNo - ITEM_FIRST_USABLE].power;
            c.defense = 0;
            if (caster->support[0] & SA_BIT(SA_CHEMIST)) {
                c.multiplier = 30;
            }
            if ((u8)type == BTL_FORMULA_ITEM_HEAL) {
                btl_calcHeal(&c);
            } else {
                v = c.power * c.multiplier;
                if (v >= 10000) {
                    v = 9999;
                }
                c.targetResult |= BTL_POPUP_MP;
                if (!(target->status & BTL_STATUS_ZOMBIE) && btl_hasEnemyFlag(target, BTL_ENEMY_UNDEAD) == 0) {
                    c.targetResult |= BTL_POPUP_MP_HEAL;
                }
                c.targetMpChange = v;
            }
            break;
        }
    case BTL_FORMULA_ELIXIR:
        if (btl_checkTargetAlive(&c)) {
            if (!(target->status & BTL_STATUS_ZOMBIE) && btl_hasEnemyFlag(target, BTL_ENEMY_UNDEAD) == 0) {
                target->hp = target->max->maxHp;
                target->mp = target->max->maxMp;
            } else {
                target->mp = 0;
                btl_killUnit(target);
            }
        }
        break;
    case BTL_FORMULA_PHOENIX_DOWN:
        if (btl_checkRevivable(&c)) {
            if ((target->status & BTL_STATUS_ZOMBIE) || btl_hasEnemyFlag(target, BTL_ENEMY_UNDEAD) != 0) {
                target->hp = RAND8() % 10;
                if (target->hp == 0) {
                    btl_killUnit(target);
                }
            } else if (btl_checkTargetIsPlayer(&c)) {
                if (target->status & BTL_STATUS_KO) {
                    target->hp = RAND8() % 10 + 1;
                }
                btl_cureItemStatus(&c);
            }
        }
        break;
    case BTL_FORMULA_ITEM_CURE_STATUS:
        btl_cureItemStatus(&c);
        break;
    case BTL_FORMULA_ITEM_COUNT_HEAL:
        c.targetResult |= (BTL_POPUP_HP | BTL_POPUP_HEAL);
        c.targetHpChange = g_usableItemTable[cmd->subNo - ITEM_FIRST_USABLE].power * (item_getCount(cmd->subNo) + 1);
        break;
    case BTL_FORMULA_ITEM_COUNT_DAMAGE:
        c.targetResult |= BTL_POPUP_HP;
        c.targetHpChange = g_usableItemTable[cmd->subNo - ITEM_FIRST_USABLE].power * (item_getCount(cmd->subNo) + 1);
        break;
    case BTL_FORMULA_TENT:
        c.targetResult |= (BTL_POPUP_HP | BTL_POPUP_HEAL | BTL_POPUP_MP | BTL_POPUP_MP_HEAL);
        c.targetHpChange = target->max->maxHp >> 1;
        c.targetMpChange = target->max->maxMp >> 1;
        RAND_REFILL2();
        if (g_usableItemTable[cmd->subNo - ITEM_FIRST_USABLE].rate < RAND16() % 100) {
            btl_showMessage(BTL_MSG_SNAKE_BITE);
            btl_addStatuses(target, g_usableItemTable[cmd->subNo - ITEM_FIRST_USABLE].statuses);
        }
        break;
    case BTL_FORMULA_9999_DAMAGE:
        if (btl_checkTargetAlive(&c)) {
            c.targetHpChange = 9999;
            c.targetResult |= BTL_POPUP_HP;
        }
        break;
    case BTL_FORMULA_JEWEL:
        btl_initStatusHitRate(&c);
        btl_halveHitForShell(&c);
        if (btl_rollStatusHit(&c)) {
            btl_addItem(ITEM_ORE);
        }
        break;
    case BTL_FORMULA_SUMMON:
        btl_setMagicParams(&c);
        btl_applyCasterMiniMagic(&c);
        btl_applyTargetShell(&c);
        btl_applyAbilityElementBoost(&c);
        if (btl_applyAbilityElement(&c) && btl_missIfAirborne(&c)) {
            /* padded: a zero word follows the first jump table */
            __asm__(".section .rodata\n\t.word 0\n\t.section .text");
            switch (cmd->subNo) {
            case ABIL_SHIVA:
                c.power += item_getCount(ITEM_OPAL);
                break;
            case ABIL_IFRIT:
                c.power += item_getCount(ITEM_TOPAZ);
                break;
            case ABIL_RAMUH:
                c.power += item_getCount(ITEM_PERIDOT);
                break;
            case ABIL_LEVIATHAN:
                c.power += item_getCount(ITEM_AQUAMARINE);
                break;
            case ABIL_BAHAMUT:
                c.power += item_getCount(ITEM_GARNET);
                break;
            case ABIL_ARK:
                c.power += item_getCount(ITEM_LAPIS_LAZULI);
                break;
            case ABIL_FENRIR:
            case ABIL_FENRIR2:
                c.power += item_getCount(ITEM_SAPPHIRE);
                break;
            case ABIL_MADEEN:
                c.power += caster->level;
                break;
            }
            btl_finishMagicDamage(&c);
            btl_rollAbilityStatus(&c);
        }
        break;
    case BTL_FORMULA_ATOMOS:
        if (btl_missIfDeathImmune(&c)) {
            btl_setMultiplierFromPower(&c);
            btl_applyAbilityElementBoost(&c);
            if (btl_applyAbilityElement(&c) && btl_missIfAirborne(&c)) {
                c.multiplier += item_getCount(ITEM_AMETHYST);
                btl_calcPercentDamage(&c);
            }
        }
        break;
    case BTL_FORMULA_ODIN:
        {
            s32 i;
            if (btl_guardIfDeathImmune(&c)) {
                btl_initStatusHitRate(&c);
                c.hitRate += item_getCount(ITEM_ORE) >> 1;
                if (btl_rollStatusHit(&c)) {
                    btl_applyAbilityStatus(&c);
                }
            }
            if ((c.flags & (BTL_CALC_MISS | BTL_CALC_GUARD)) && (caster->support[1] & SA_BIT(SA_ODINS_SWORD))) {
                c.flags &= ~(BTL_CALC_MISS | BTL_CALC_GUARD);
                RAND_REFILL2();
                i = caster->magic;
                c.multiplier = i + RAND16() % (((caster->level + i) >> 3) + 1);
                c.power = cmd->ability->power + 100 - item_getCount(ITEM_ORE);
                c.defense = target->magicDef;
                btl_finishMagicDamage(&c);
            }
            break;
        }
    case BTL_FORMULA_SELF_DESTRUCT:
        c.targetResult |= BTL_POPUP_HP;
        if (caster->hp != 0) {
            caster->hpChange = caster->hp;
            btl_killUnit(caster);
        }
        c.targetHpChange = caster->hpChange;
        if (c.targetHpChange >= 10000) {
            c.targetHpChange = 9999;
        }
        break;
    case BTL_FORMULA_SWAP_HP:
        {
            u32 v;
            if (btl_missIfDeathImmune(&c) && btl_checkTargetAlive(&c)) {
                v = caster->hp;
                caster->hp = target->hp;
                if (caster->max->maxHp < caster->hp) {
                    caster->hp = caster->max->maxHp;
                }
                target->hp = v;
                if (target->max->maxHp < v) {
                    target->hp = target->max->maxHp;
                }
            }
            break;
        }
    case BTL_FORMULA_HALVE_DEFENSES:
        target->defense >>= 1;
        target->magicDef >>= 1;
        break;
    case BTL_FORMULA_ADD_ITEM:
        btl_addItem(c.cmd->ability->power);
        break;
    case BTL_FORMULA_LOW_HP_STATUS:
        if (btl_missIfDeathImmune(&c) && btl_checkTargetAlive(&c)) {
            c.flags |= BTL_CALC_NO_DAMAGE;
            target->hp = RAND8() % 9 + 1;
            btl_addStatuses(target, g_gameState->gameData->ctx.battle->statusSets[cmd->ability->statusSet]);
        }
        break;
    case BTL_FORMULA_ABSORB_MAGIC:
        {
            target->magic >>= 1;
            if (caster->magic * 2 >= 0x100) {
                caster->magic = 0xFF;
            } else {
                caster->magic = caster->magic * 2;
            }
            break;
        }
    case BTL_FORMULA_ABSORB_STRENGTH:
        {
            target->strength >>= 1;
            if (caster->strength * 2 >= 0x100) {
                caster->strength = 0xFF;
            } else {
                caster->strength = caster->strength * 2;
            }
            break;
        }
    case BTL_FORMULA_ADD_TRANCE:
        target->trance = 0xFF;
        btl_addStatus(target, BTL_STATUS_TRANCE);
        break;
    case BTL_FORMULA_STATUS_NOT_FEMALE:
        if ((&g_gameState->gameData->characters[BTL_UNIT_INDEX(target->flags)])->classFlags & CHR_CLASS_FEMALE) {
            c.flags |= BTL_CALC_MISS;
        } else {
            btl_applyAbilityStatus(&c);
        }
        break;
    case BTL_FORMULA_PHYSICAL_CLASS8:
        if ((&g_gameState->gameData->characters[BTL_UNIT_INDEX(target->flags)])->classFlags & 8) {
            c.flags |= BTL_CALC_MISS;
        } else {
            btl_setAbilityPhysParams(&c);
            btl_finishMagicDamage(&c);
        }
        break;
    case BTL_FORMULA_LEVEL_DAMAGE:
        btl_initStatusHitRate(&c);
        btl_halveHitForShell(&c);
        btl_halveHitForSpread(&c);
        if (btl_rollStatusHit(&c)) {
            c.targetResult |= BTL_POPUP_HP;
            c.targetHpChange = target->level * cmd->ability->power;
        }
        break;
    case BTL_FORMULA_ENEMY_ATTACK:
        if (!btl_checkFrozenShatter(&c)) {
            break;
        }
    case BTL_FORMULA_ENEMY_ATTACK_ROW:
        btl_setEnemyAttackParams(&c);
        btl_applyCasterStatusMod(&c);
        btl_randomizeDefense(&c);
        btl_applyTargetStatusMod(&c);
        btl_applyRowMod(&c);
        btl_applyAbilityElementBoost(&c);
        if (btl_applyAbilityElement(&c)) {
            btl_finishMagicDamage(&c);
            if ((u8)type == BTL_FORMULA_ENEMY_ATTACK_ROW && BTL_UNIT_ROW(target->flags)) {
                btl_toggleRow(target);
            }
            btl_rollAbilityStatus(&c);
        }
        break;
    case BTL_FORMULA_ENEMY_STEAL:
        btl_initHitRates(&c);
        if (btl_rollHit(&c)) {
            btl_calcEnemySteal(&c);
        } else {
            btl_showMessage(BTL_MSG_STEAL_FAILED);
        }
        break;
    case BTL_FORMULA_ENEMY_ATTACK_STEAL:
        btl_setEnemyAttackParams(&c);
        btl_applyCasterStatusMod(&c);
        btl_randomizeDefense(&c);
        btl_applyTargetStatusMod(&c);
        btl_applyRowMod(&c);
        btl_finishMagicDamage(&c);
        btl_calcEnemySteal(&c);
        break;
    case BTL_FORMULA_EVT_POWER_DAMAGE:
        c.targetResult |= BTL_POPUP_HP;
        if (g_evtGlobalVars.b[0xC0] >= 14) {
            c.targetHpChange = 9999;
        } else {
            c.targetHpChange = 1 << (g_evtGlobalVars.b[0xC0] - 1);
        }
        break;
    case BTL_FORMULA_RANDOM_STATUS:
        {
            s32 i;
            s32 bit;
            if (btl_checkTargetAlive(&c)) {
                for (bit = 1, i = 0; i < 32; i++, bit <<= 1) {
                    if ((RAND8() >> 5) == 0) {
                        if (bit & (BTL_STATUS_PETRIFY | BTL_STATUS_SILENCE | BTL_STATUS_BLIND | BTL_STATUS_TROUBLE | BTL_STATUS_ZOMBIE | BTL_STATUS_KO | BTL_STATUS_CONFUSE | BTL_STATUS_BERSERK | BTL_STATUS_STOP | BTL_STATUS_POISON | BTL_STATUS_SLEEP | BTL_STATUS_HEAT | BTL_STATUS_FREEZE | BTL_STATUS_DOOM | BTL_STATUS_MINI)) {
                            btl_addStatus(target, bit);
                        } else if ((bit & BTL_STATUS_LOW_HP) && !(target->status & BTL_STATUS_KO)) {
                            c.flags |= BTL_CALC_NO_DAMAGE;
                            target->hp = RAND8() % 9 + 1;
                        }
                    }
                }
            }
            break;
        }
    case BTL_FORMULA_REMOVE_UNIT:
        g_btlRewards.expMask &= ~(1 << ((u8)target->flags >> BTL_UNIT_SLOT_SHIFT));
        btl_cancelTranceEidolon(target);
        btl_removeUnitCmds(target);
        btl_saveCharacter(target, 1);
        btl_unlinkUnit(target);
        break;
    case BTL_FORMULA_MINI:
        if (!(target->status & BTL_STATUS_MINI)) {
            btl_initStatusHitRate(&c);
            btl_halveHitForShell(&c);
            btl_halveHitForSpread(&c);
            if (btl_rollStatusHit(&c)) {
                btl_applyAbilityStatus(&c);
            }
        } else {
            btl_applyAbilityStatus(&c);
        }
        break;
    case BTL_FORMULA_ADD_STATUS:
        btl_initStatusHitRate(&c);
        btl_halveHitForShell(&c);
        btl_halveHitForSpread(&c);
        if (btl_rollStatusHit(&c)) {
            btl_applyAbilityStatus(&c);
        }
        break;
    case BTL_FORMULA_ADD_STATUS_SURE:
        btl_applyAbilityStatus(&c);
        break;
    }
    if (caster != NULL && target != NULL) {
        btl_applyCalcResult(&c);
    }
}

void btl_applyCalcResult(BtlCalc *c) {
    BtlUnit *t = c->target;
    BtlUnit *s = c->caster;
    s32 back = 0;
    u16 i;
    u16 n;
    u32 a;
    u16 who;
    s32 id;
    BtlUnit *w;

    if ((t->flags & BTL_UNIT_PARTY) && !(s->flags & BTL_UNIT_PARTY)) {
        back = btl_tryCounter(t, s, (BtlCmd *)c->cmd);
    }
    if (c->flags & BTL_CALC_GUARD) {
        t->popupFlags |= BTL_POPUP_GUARD;
    } else if (c->flags & BTL_CALC_MISS) {
        t->popupFlags |= BTL_POPUP_MISS;
        if (c->flags & BTL_CALC_DODGED) {
            if (t->flags & BTL_UNIT_PARTY) {
                t->model->motion = (BtlMotion *)t->motions[16];
                t->obj->frame = 0;
                t->rot[1] = t->obj->rotY = (t->flags & BTL_UNIT_PARTY) * 0x800;
            } else if (!((t->flags >> BTL_UNIT_MODEL_PART_BIT) & 1)) {
                t->pos[2] += 0x1900000;
            } else {
                w = btl_findBodyUnit();
                w->pos[2] += 0x1900000;
            }
            t->flags |= BTL_UNIT_DODGED;
            c->cmd->flags |= BTL_CMDINFO_DODGED;
        }
    } else {
        if (c->cmd->ability->flags & BTL_ABIL_PHYSICAL) {
            btl_removeStatus(t, BTL_STATUS_CONFUSE);
            btl_removeStatus(t, BTL_STATUS_SLEEP);
        }
        if (c->cmd->ability->flags & BTL_ABIL_DISPELS_VANISH) {
            btl_removeStatus(t, BTL_STATUS_VANISH);
        }
        if (c->targetResult != 0) {
            t->popupFlags |= c->targetResult;
            if (c->targetResult & BTL_POPUP_HP) {
                n = 0;
                if (((c->cmd->flags >> BTL_CMDINFO_REFLECT_SHIFT) & 3) == 1) {
                    for (i = 0; i < 4; i++) {
                        if (s->bounceMasks[i] & t->unitBit) {
                            n++;
                        }
                    }
                    if (c->targetHpChange * n >= 10000) {
                        c->targetHpChange = 9999;
                    } else {
                        c->targetHpChange = c->targetHpChange * n;
                    }
                    if ((s32)*s->support < 0) {
                        c->targetHpChange *= 2;
                        if (c->targetHpChange >= 10000) {
                            c->targetHpChange = 9999;
                        }
                    }
                }
                if (c->targetResult & BTL_POPUP_HEAL) {
                    btl_healHp(t, c->targetHpChange);
                } else {
                    btl_damageHp(t, c->targetHpChange, btl_canAddHitStatus(c));
                    btl_gainTrance(c, back);
                }
            }
            if (c->targetResult & BTL_POPUP_MP) {
                if (c->targetResult & BTL_POPUP_MP_HEAL) {
                    btl_healMp(t, c->targetMpChange);
                } else {
                    btl_damageMp(t, c->targetMpChange);
                }
            }
        } else if (c->flags & BTL_CALC_NO_DAMAGE) {
            if ((u8)btl_canAddHitStatus(c)) {
                btl_startHitMotion(t);
            }
            btl_gainTrance(c, back);
        }
        if (c->casterResult != 0) {
            s->popupFlags |= c->casterResult;
            if (c->casterResult & BTL_POPUP_HP) {
                if (c->casterResult & BTL_POPUP_HEAL) {
                    btl_healHp(s, c->casterHpChange);
                } else {
                    btl_damageHp(s, c->casterHpChange, 0);
                }
            }
            if (c->casterResult & BTL_POPUP_MP) {
                if (c->casterResult & BTL_POPUP_MP_HEAL) {
                    btl_healMp(s, c->casterMpChange);
                } else {
                    btl_damageMp(s, c->casterMpChange);
                }
            }
        }
        if ((c->flags & BTL_CALC_WEAPON_STATUS) && t->hp != 0) {
            a = g_gameState->gameData->ctx.battle->statusSets[s->weapon[1]];
            if (!(a & 0x100) || !(t->status & BTL_STATUS_DEATH_IMMUNE)) {
                btl_addStatuses(t, a);
            }
        }
    }
    if (!(t->flags & BTL_UNIT_PARTY)) {
        who = 0x10;
        if (!((t->flags >> BTL_UNIT_MODEL_PART_BIT) & 1)) {
            who = t->unitBit;
        }
        id = c->cmd->subNo | (c->cmd->cmdNo << 8);
        if ((s->flags & BTL_UNIT_PARTY) && !(t->status & (BTL_STATUS_PETRIFY | BTL_STATUS_VENOM | BTL_STATUS_STOP | BTL_STATUS_FREEZE))) {
            if ((((g_gameState->gameData->ctx.battle->enemies + BTL_UNIT_INDEX(t->flags))->flags >> BTL_ENEMYF_SCRIPT_DEATH_BIT) & 1) && t->hp == 0) {
                evt_battleTrigger(0x36, who, s->unitBit, id);
            } else if (t->hp != 0 && c->cmd->cmdNo < BTL_CMD_TABLE_COUNT) {
                evt_battleTrigger(0x35, who, s->unitBit, id);
            }
        }
        evt_battleTrigger(0x37, who, s->unitBit, id);
    }
}

void btl_calcSteal(BtlCalc *calc, Unk800B7F20 *enemy, s32 slot) {
    u8 itemId = enemy->steal[slot];
    GameData *gd;
    GameData *g;
    u32 r;

    if (itemId != 0xFF) {
        enemy->steal[slot] = 0xFF;
        g = g_gameState->gameData;
        if (g->stealCount < 9999) {
            g->stealCount++;
        }
        btl_addItem(itemId);
        btl_showMessage(BTL_MSG_STOLE, btl_getItemName(itemId));
        if (calc->caster->support[1] & SA_BIT(SA_MUG)) {
            calc->targetResult |= BTL_POPUP_HP;
            RAND_REFILL2();
            calc->targetHpChange = RAND16() % ((calc->caster->level * calc->target->level) >> 1);
        }
        if (calc->caster->support[0] & SA_BIT(SA_STEAL_GIL)) {
            gd = g_gameState->gameData;
            RAND_REFILL2();
            gd->gil = g_gameState->gameData->gil +
                         RAND16() % ((calc->caster->level * calc->target->level) / 4 + 1);
            if (g_gameState->gameData->gil > GIL_MAX) {
                g_gameState->gameData->gil = GIL_MAX;
            }
        }
    } else {
        btl_showMessage(BTL_MSG_STEAL_FAILED);
    }
}

void btl_gainTrance(BtlCalc *calc, u8 countered) {
    BtlUnit *u;
    u8 gain;
    if ((calc->target->flags & BTL_UNIT_PARTY) && !(calc->caster->flags & BTL_UNIT_PARTY)) {
        if (countered == 0) {
            btl_tryAutoPotion(calc->target, calc->cmd->cmdNo);
        }
        btl_applyHitSupports(calc->target, (u8 *)calc->cmd->ability);
        u = calc->target;
        if (((u->flags >> BTL_UNIT_TRANCE_ENABLED_BIT) & 1) && u->hp != 0 && !(u->status & (BTL_STATUS_PETRIFY | BTL_STATUS_VENOM | BTL_STATUS_ZOMBIE | BTL_STATUS_KO | BTL_STATUS_STOP | BTL_STATUS_TRANCE | BTL_STATUS_FREEZE))) {
            if (u->support[1] & SA_BIT(SA_HIGH_TIDE)) {
                gain = u->spirit;
            } else {
                RAND_REFILL2();
                gain = RAND16() % calc->target->spirit;
            }
            u = calc->target;
            if (gain + u->trance < 0xFF) {
                u->trance += gain;
            } else {
                u->trance = 0xFF;
                btl_addStatus(calc->target, BTL_STATUS_TRANCE);
            }
        }
    }
}

u8 btl_rollHit(BtlCalc *calc) {
    if (calc->caster->status & (BTL_STATUS_BLIND | BTL_STATUS_CONFUSE)) {
        calc->hitRate /= 2;
    }
    if (calc->caster->status & (BTL_STATUS_TRANCE | BTL_STATUS_VANISH)) {
        calc->evade = 0;
    }
    if (calc->target->support[0] & SA_BIT(SA_DISTRACT)) {
        calc->hitRate /= 2;
    }
    if (calc->target->status & (BTL_STATUS_PETRIFY | BTL_STATUS_VENOM | BTL_STATUS_VIRUS | BTL_STATUS_BLIND | BTL_STATUS_CONFUSE | BTL_STATUS_STOP | BTL_STATUS_SLEEP | BTL_STATUS_FREEZE)) {
        calc->evade = 0;
    }
    if (calc->target->status & BTL_STATUS_DEFEND) {
        calc->hitRate /= 2;
    }
    if (calc->target->status & BTL_STATUS_VANISH) {
        calc->hitRate = 0;
    }
    RAND_REFILL2();
    if (RAND16() % 100 >= calc->hitRate) {
        calc->flags |= BTL_CALC_MISS;
        return 0;
    }
    RAND_REFILL2();
    if (RAND16() % 100 < calc->evade) {
        if (calc->target != calc->caster) {
            calc->flags |= BTL_CALC_MISS;
            if (!((calc->target->flags >> BTL_UNIT_COVERING_BIT) & 1)) {
                calc->flags |= (BTL_CALC_MISS | BTL_CALC_DODGED);
            }
            return 0;
        }
    }
    return 1;
}

u8 btl_rollStatusHit(BtlCalc *calc) {
    RAND_REFILL2();
    if (RAND16() % 100 < calc->hitRate) {
        RAND_REFILL2();
        if (RAND16() % 100 >= calc->evade) {
            return 1;
        }
    }
    calc->flags |= BTL_CALC_MISS;
    return 0;
}

void btl_setPhysParams(BtlCalc *calc, u8 formula) {
    s32 bonus;

    RAND_REFILL2();
    bonus = RAND16() % (((calc->caster->level + calc->caster->strength) >> 3) + 1);
    calc->power = calc->caster->weapon[5];
    calc->defense = calc->target->defense;
    switch (formula) {
    case BTL_FORMULA_WEAPON_STR_LEVEL:
        calc->power += calc->caster->level;
    case BTL_FORMULA_WEAPON_STR:
        calc->multiplier = calc->caster->strength + bonus;
        break;
    case BTL_FORMULA_WEAPON_STR_SPR:
        calc->multiplier = ((calc->caster->strength + calc->caster->spirit) >> 1) + bonus;
        break;
    case BTL_FORMULA_WEAPON_STR_SPD:
        calc->multiplier = ((calc->caster->strength + calc->caster->speed) >> 1) + bonus;
        break;
    case BTL_FORMULA_WEAPON_STR_MAG:
        calc->multiplier = ((calc->caster->strength + calc->caster->magic) >> 1) + bonus;
        break;
    case BTL_FORMULA_WEAPON_RANDOM:
        RAND_REFILL2();
        calc->multiplier = RAND16() % calc->caster->strength + bonus;
        break;
    }
}

void btl_applyKillerBonus(BtlCalc *calc) {
    u32 targetFlags = calc->target->flags;
    u32 support;
    BtlEnemyType *enemyType;
    BtlUnit *u;
    if (!(targetFlags & BTL_UNIT_PARTY)) {
        support = calc->caster->support[0];
        enemyType = g_gameState->gameData->ctx.battle->enemies[BTL_UNIT_INDEX(targetFlags)].type;
        if (((support & SA_BIT(SA_BIRD_KILLER)) && (enemyType->category & BTL_ENEMY_AERIAL))
            || ((support & SA_BIT(SA_BUG_KILLER)) && (enemyType->category & BTL_ENEMY_INSECT))
            || ((support & SA_BIT(SA_STONE_KILLER)) && (enemyType->category & BTL_ENEMY_STONE))
            || ((support & SA_BIT(SA_UNDEAD_KILLER)) && (enemyType->category & BTL_ENEMY_UNDEAD))
            || ((support & SA_BIT(SA_DRAGON_KILLER)) && (enemyType->category & BTL_ENEMY_DRAGON))
            || ((support & SA_BIT(SA_DEVIL_KILLER)) && (enemyType->category & BTL_ENEMY_DEMON))
            || ((support & SA_BIT(SA_BEAST_KILLER)) && (enemyType->category & BTL_ENEMY_BEAST))
            || ((support & SA_BIT(SA_MAN_EATER)) && (enemyType->category & BTL_ENEMY_HUMANOID))) {
            calc->multiplier = calc->multiplier * 3 >> 1;
        }
    }
    u = calc->caster;
    if ((u->support[0] & SA_BIT(SA_MP_ATTACK)) && u->mp > 0) {
        calc->multiplier = calc->multiplier * 3 >> 1;
        calc->flags |= BTL_CALC_MP_ATTACK;
    }
}

void btl_rollCritical(BtlCalc *calc) {
    u8 critRange = calc->caster->spirit >> 2;
    s32 roll;
    if (critRange != 0) {
        RAND_REFILL2();
        RAND_REFILL2();
        roll = RAND16();
        if (roll % 100 < roll % critRange) {
            calc->multiplier *= 2;
            calc->targetResult |= BTL_POPUP_CRITICAL;
        }
    }
}

u8 btl_applyAbilityElement(BtlCalc *calc) {
    BtlUnit *target;
    u8 element = calc->cmd->ability->element;
    if (calc->caster->support[1] & SA_BIT(SA_MAG_ELEM_NULL)) {
        return 1;
    }
    target = calc->target;
    if (target->elemGuard & element) {
        calc->flags |= BTL_CALC_GUARD;
        return 0;
    }
    if (target->elemHalf & element) {
        calc->multiplier /= 2;
    }
    if (calc->target->elemAbsorb & element) {
        calc->defense = 0;
        calc->flags |= BTL_CALC_ABSORB;
    }
    if (calc->target->elemWeak & element) {
        calc->multiplier = calc->multiplier * 3 >> 1;
    }
    if (element & (ELEM_ICE | ELEM_WATER)) {
        btl_removeStatus(calc->target, BTL_STATUS_HEAT);
    }
    if (element & ELEM_FIRE) {
        btl_removeStatus(calc->target, BTL_STATUS_FREEZE);
    }
    return 1;
}

void btl_finishPhysDamage(BtlCalc *calc) {
    s32 base = calc->power - calc->defense;
    s32 damage;
    BtlUnit *u;
    BtlUnit *t;
    if (base <= 0) {
        base = 1;
    }
    calc->targetResult |= BTL_POPUP_HP;
    if (calc->multiplier <= 0) {
        calc->multiplier = 1;
    }
    damage = base * calc->multiplier;
    if (damage >= 10000) {
        damage = 9999;
    }
    if (!(calc->flags & BTL_CALC_ABSORB)) {
        u = calc->caster;
        if ((u->flags & BTL_UNIT_PARTY) && ((*u->support & SA_BIT(SA_HEALER)) || g_gameState->gameData->characters[BTL_UNIT_INDEX(u->flags)].equip[0] == ITEM_HEALING_ROD)) {
            if (!(calc->target->status & BTL_STATUS_ZOMBIE) && btl_hasEnemyFlag(calc->target, BTL_ENEMY_UNDEAD) == 0) {
                t = calc->target;
                t->rot[1] = t->obj->rotY = (t->flags & BTL_UNIT_PARTY) * 0x800;
                calc->targetResult |= BTL_POPUP_HEAL;
            }
        }
    } else {
        calc->targetResult |= BTL_POPUP_HEAL;
    }
    calc->targetHpChange = damage;
}

void btl_setTargetHp(BtlCalc *calc) {
    BtlUnit *t;
    BtlUnit *u;
    BtlModel *m;
    if (calc->cmd->ability->power != 0) {
        if (calc->target->status & BTL_STATUS_KO) {
            calc->flags |= BTL_CALC_MISS;
            return;
        }
        calc->flags |= BTL_CALC_NO_DAMAGE;
        calc->target->hp = calc->cmd->ability->power;
        t = calc->target;
        t->rot[1] = t->obj->rotY = (t->flags & BTL_UNIT_PARTY) * 0x800;
        return;
    }
    u = calc->target;
    u->hp = 0;
    u->flags |= BTL_UNIT_KILLED;
    if (!(u->flags & BTL_UNIT_PARTY)) {
        btl_playDeathCry(u, g_gameState->gameData->ctx.battle->enemies[BTL_UNIT_INDEX(u->flags)].type->deathCry);
    }
    m = u->model;
    if ((s32)m->motion != u->motions[(u->flags >> BTL_UNIT_WEAK_STANCE_BIT) & 1] && (!(u->flags & BTL_UNIT_PARTY) || (s32)m->motion != u->motions[9]) && u != g_gameState->gameData->ctx.battle->curCmd->caster) {
        m->motion = (BtlMotion *)u->motions[(u->flags >> BTL_UNIT_WEAK_STANCE_BIT) & 1];
        u->obj->frame = 0;
    }
}

void btl_rollFlee(BtlCalc *calc) {
    s8 partyCount;
    s8 enemyCount;
    s16 partyLevels;
    s16 enemyLevels;
    Battle *b;
    BtlUnit *u;
    BtlCmd *cmd;
    s16 chance;
    enemyCount = partyCount = 0;
    b = g_gameState->gameData->ctx.battle;
    enemyLevels = partyLevels = 0;
    u = b->unitList;
    cmd = b->curCmd;
    for (; u != NULL; u = u->next) {
        if (u->flags & BTL_UNIT_PARTY) {
            partyCount++;
            partyLevels += u->level;
        } else {
            enemyCount++;
            enemyLevels += u->level;
        }
    }
    if (enemyCount == 0) {
        enemyCount = 1;
    }
    if (partyCount == 0) {
        partyCount = 1;
    }
    if (enemyLevels == 0) {
        enemyLevels = 1;
    }
    if (partyLevels == 0) {
        partyLevels = 1;
    }
    chance = (200 / (enemyLevels / enemyCount)) * (partyLevels / partyCount) / 16;
    RAND_REFILL2();
    if (chance > RAND16() % 100 && (cmd == NULL || (cmd->caster->flags & BTL_UNIT_PARTY))) {
        btl_setCmd((s32)&g_gameState->gameData->ctx.battle->escapeCmd, BTL_CMD_ESCAPE, 1, BTL_TARGETS_PARTY, 1);
    }
}

u8 btl_checkFrozenShatter(BtlCalc *calc) {
    BtlUnit *u;
    BtlModel *m;
    if ((calc->target->status & (BTL_STATUS_PETRIFY | BTL_STATUS_FREEZE)) != 0x02000000) {
        return 1;
    }
    btl_removeQueuedCmd(calc->target, BTL_CMD_PETRIFY);
    u = calc->target;
    u->hp = 0;
    u->flags |= BTL_UNIT_KILLED;
    if (!(u->flags & BTL_UNIT_PARTY)) {
        btl_playDeathCry(u, g_gameState->gameData->ctx.battle->enemies[BTL_UNIT_INDEX(u->flags)].type->deathCry);
    }
    m = u->model;
    if ((s32)m->motion != u->motions[(u->flags >> BTL_UNIT_WEAK_STANCE_BIT) & 1] && (!(u->flags & BTL_UNIT_PARTY) || (s32)m->motion != u->motions[9]) && u != g_gameState->gameData->ctx.battle->curCmd->caster) {
        m->motion = (BtlMotion *)u->motions[(u->flags >> BTL_UNIT_WEAK_STANCE_BIT) & 1];
        u->obj->frame = 0;
    }
    btl_showMessage(BTL_MSG_IMPACT_CRUSHES);
    return 0;
}

s32 btl_consumeMp(BtlUnit *unit, s16 mp) {
    if (unit != NULL) {
        if (unit->support[1] & SA_BIT(SA_HALF_MP)) {
            mp /= 2;
        }
        if (unit->mp < mp) {
            return 0;
        }
        unit->mp -= mp;
        return 1;
    }
    return 0;
}

void btl_rollMagicFail(BtlCmd *cmd) {
    RAND_REFILL2();
    if ((cmd->caster->level >> 1) + cmd->caster->spirit < RAND16() % 100) {
        cmd->info.b.magicFailed = 1;
    }
}

s32 btl_checkSwdMagUsable(BtlUnit *unit, s16 mpCost) {
    BtlUnit *u;
    u32 f;
    if (unit->mp >= mpCost) {
        for (u = g_gameState->gameData->ctx.battle->unitList; u != NULL; u = u->next) {
            f = u->flags;
            if ((f & BTL_UNIT_PARTY) && g_gameState->gameData->characters[BTL_UNIT_INDEX(f)].info.b.kind == CHR_KIND_VIVI) {
                if (!(u->status & (BTL_STATUS_PETRIFY | BTL_STATUS_VENOM | BTL_STATUS_SILENCE | BTL_STATUS_KO | BTL_STATUS_CONFUSE | BTL_STATUS_BERSERK | BTL_STATUS_STOP | BTL_STATUS_SLEEP | BTL_STATUS_HEAT | BTL_STATUS_FREEZE | BTL_STATUS_MINI)) && !(unit->status & (BTL_STATUS_PETRIFY | BTL_STATUS_VENOM | BTL_STATUS_SILENCE | BTL_STATUS_KO | BTL_STATUS_CONFUSE | BTL_STATUS_BERSERK | BTL_STATUS_STOP | BTL_STATUS_SLEEP | BTL_STATUS_HEAT | BTL_STATUS_FREEZE | BTL_STATUS_MINI))) {
                    return 1;
                }
                break;
            }
        }
    }
    btl_showMessage(BTL_MSG_COMBINATION_FAILED);
    return 0;
}

void btl_killUnit(BtlUnit *unit) {
    BtlModel *model;
    unit->hp = 0;
    unit->flags |= BTL_UNIT_KILLED;
    if (!(unit->flags & BTL_UNIT_PARTY)) {
        btl_playDeathCry(unit, g_gameState->gameData->ctx.battle->enemies[BTL_UNIT_INDEX(unit->flags)].type->deathCry);
    }
    model = unit->model;
    if ((s32)model->motion != unit->motions[(unit->flags >> BTL_UNIT_WEAK_STANCE_BIT) & 1] && (!(unit->flags & BTL_UNIT_PARTY) || (s32)model->motion != unit->motions[9]) && unit != g_gameState->gameData->ctx.battle->curCmd->caster) {
        model->motion = (BtlMotion *)unit->motions[(unit->flags >> BTL_UNIT_WEAK_STANCE_BIT) & 1];
        unit->obj->frame = 0;
    }
}

void btl_rollShortSummon(BtlCmd *cmd) {
    s32 roll;
    if (!(cmd->caster->support[1] & SA_BIT(SA_BOOST))) {
        if (cmd->ability->mpCost * 2 < cmd->caster->mp) {
            roll = rand8_inline() < 0xE6;
        } else {
            roll = rand8_inline() < 0xAA;
        }
        if (roll) {
            cmd->info.b.shortSummon = 1;
        }
    }
}

s32 btl_canAddHitStatus(BtlCalc *calc) {
    if (((calc->flags & BTL_CALC_WEAPON_STATUS) && (g_gameState->gameData->ctx.battle->statusSets[calc->caster->weapon[1]] & 0x42001103)) || (calc->cmd->ability->flags & BTL_ABIL_NO_HIT_MOTION) || ((calc->cmd->flags >> BTL_CMDINFO_COVERED_BIT) & 1) || (calc->target->status & (BTL_STATUS_PETRIFY | BTL_STATUS_VENOM | BTL_STATUS_KO | BTL_STATUS_STOP | BTL_STATUS_DEFEND | BTL_STATUS_FREEZE | BTL_STATUS_JUMP)) || calc->caster == calc->target) {
        return 0;
    }
    return 1;
}

void btl_setDrainFlags(BtlCalc *calc) {
    BtlUnit *target;
    calc->targetResult |= BTL_POPUP_HP;
    target = calc->target;
    calc->casterResult |= BTL_POPUP_HP;
    if ((target->status & BTL_STATUS_ZOMBIE) || btl_hasEnemyFlag(target, BTL_ENEMY_UNDEAD) != 0) {
        calc->targetResult |= BTL_POPUP_HEAL;
    } else {
        calc->casterResult |= BTL_POPUP_HEAL;
    }
}

void btl_initHitRates(BtlCalc *calc) {
    calc->hitRate = 100;
    calc->evade = calc->target->evade;
}

void btl_initStatusHitRate(BtlCalc *calc) {
    BtlUnit *caster = calc->caster;
    calc->hitRate = caster->level + (calc->cmd->ability->rate + (caster->magic >> 2)) - calc->target->level;
    if (calc->hitRate > 100) {
        calc->hitRate = 100;
    } else if (calc->hitRate <= 0) {
        calc->hitRate = 1;
    }
    calc->evade = calc->target->magicEva;
}

u8 btl_initUndeadHitRate(BtlCalc *calc) {
    BtlUnit *target = calc->target;
    if ((target->status & BTL_STATUS_ZOMBIE) || btl_hasEnemyFlag(target, BTL_ENEMY_UNDEAD) != 0) {
        BtlUnit *caster = calc->caster;
        calc->hitRate = caster->level + (calc->cmd->ability->rate + (caster->magic >> 2)) - calc->target->level;
        if (calc->hitRate > 100) {
            calc->hitRate = 100;
        } else if (calc->hitRate <= 0) {
            calc->hitRate = 1;
        }
        calc->evade = calc->target->magicEva;
        return 0;
    }
    return 1;
}

u32 btl_hasSureHit(BtlCalc *calc) {
    return *calc->caster->support & SA_BIT(SA_ACCURACY_PLUS);
}

void btl_halveHitForShell(BtlCalc *calc) {
    if (calc->target->status & BTL_STATUS_SHELL) {
        calc->hitRate /= 2;
    }
}

void btl_halveHitForSpread(BtlCalc *calc) {
    BtlCalc8 *cmd = calc->cmd;
    if ((cmd->flags & BTL_CMDINFO_TARGETING) == 1 && (u32)((*(u32 *)cmd->ability & 0xF) - BTL_TGT_MULTI_ANY) < 3) {
        calc->hitRate /= 2;
    }
}

u8 btl_checkLevelMultiple(BtlCalc *calc) {
    if (calc->target->level % calc->cmd->ability->rate != 0) {
        calc->flags |= BTL_CALC_MISS;
        return 0;
    }
    return 1;
}

u8 btl_missIfDeathImmune(BtlCalc *calc) {
    if (calc->target->status & BTL_STATUS_DEATH_IMMUNE) {
        calc->flags |= BTL_CALC_MISS;
        return 0;
    }
    return 1;
}

u8 btl_checkTargetIsPlayer(BtlCalc *calc) {
    if (!(calc->target->flags & BTL_UNIT_PARTY)) {
        calc->flags |= BTL_CALC_MISS;
        return 0;
    }
    return 1;
}

u8 btl_checkTargetAlive(BtlCalc *calc) {
    if (calc->target->status & (BTL_STATUS_PETRIFY | BTL_STATUS_KO)) {
        calc->flags |= BTL_CALC_MISS;
        return 0;
    }
    return 1;
}

u8 btl_checkRevivable(BtlCalc *calc) {
    u32 status = calc->target->status;
    if ((status & BTL_STATUS_PETRIFY) || (status & (BTL_STATUS_ZOMBIE | BTL_STATUS_KO)) == (BTL_STATUS_ZOMBIE | BTL_STATUS_KO)) {
        calc->flags |= BTL_CALC_MISS;
        return 0;
    }
    return 1;
}

u8 btl_checkNotSelf(BtlCalc *calc) {
    if (calc->target == calc->caster) {
        calc->flags |= BTL_CALC_MISS;
        return 0;
    }
    return 1;
}

u8 btl_guardIfDeathImmune(BtlCalc *calc) {
    if (calc->target->status & BTL_STATUS_DEATH_IMMUNE) {
        calc->flags |= BTL_CALC_GUARD;
        return 0;
    }
    return 1;
}

void btl_setAbilityPhysParams(BtlCalc *calc) {
    u8 roll;
    s32 strength;
    BtlUnit *caster;
    RAND_REFILL2();
    roll = *g_randPtr;
    caster = calc->caster;
    strength = caster->strength;
    calc->multiplier = strength + ((roll << 8) | roll) % (((caster->level + strength) >> 2) + 1);
    g_randPtr++;
    calc->power = calc->cmd->ability->power;
    calc->defense = calc->target->defense;
}

void btl_setMagicParams(BtlCalc *calc) {
    u8 roll;
    s32 magic;
    BtlUnit *caster;
    RAND_REFILL2();
    roll = *g_randPtr;
    caster = calc->caster;
    magic = caster->magic;
    calc->multiplier = magic + ((roll << 8) | roll) % (((caster->level + magic) >> 3) + 1);
    g_randPtr++;
    calc->power = calc->cmd->ability->power;
    calc->defense = calc->target->magicDef;
}

void btl_setMultiplierFromPower(BtlCalc *calc) {
    calc->multiplier = calc->cmd->ability->power;
}

void btl_setWeaponAbilityParams(BtlCalc *calc) {
    s32 strength;
    RAND_REFILL2();
    strength = calc->caster->strength;
    calc->multiplier = strength + RAND16() % (((calc->caster->level + strength) >> 3) + 1);
    calc->power = calc->caster->weapon[5] * calc->cmd->ability->power / 10;
    calc->defense = calc->target->defense;
}

void btl_setEnemyAttackParams(BtlCalc *calc) {
    u8 roll;
    s32 strength;
    BtlUnit *caster;
    RAND_REFILL2();
    roll = *g_randPtr;
    caster = calc->caster;
    strength = caster->strength;
    calc->multiplier = strength + ((roll << 8) | roll) % (((caster->level + strength) >> 2) + 1);
    g_randPtr++;
    calc->power = calc->cmd->ability->power;
    calc->defense = calc->target->defense;
}

void btl_applyCasterStatusMod(BtlCalc *calc) {
    if (calc->caster->status & (BTL_STATUS_BERSERK | BTL_STATUS_TRANCE)) {
        calc->multiplier = calc->multiplier * 3 >> 1;
    }
    if (calc->caster->status & BTL_STATUS_MINI) {
        calc->multiplier = 1;
    }
}

void btl_randomizeDefense(BtlCalc *calc) {
    if ((calc->target->support[0] & SA_BIT(SA_GAMBLE_DEFENSE)) && calc->defense != 0) {
        calc->defense = rand16_inline() % (calc->defense * 2);
    }
}

void btl_applyTargetStatusMod(BtlCalc *calc) {
    if (calc->target->status & (BTL_STATUS_DEFEND | BTL_STATUS_PROTECT)) {
        calc->multiplier >>= 1;
    }
    if (calc->target->status & (BTL_STATUS_SLEEP | BTL_STATUS_MINI)) {
        calc->multiplier = calc->multiplier * 3 >> 1;
    }
}

void btl_applyCasterMiniMagic(BtlCalc *calc) {
    if (calc->caster->status & BTL_STATUS_MINI) {
        calc->multiplier /= 2;
    }
}

void btl_applyTargetShell(BtlCalc *calc) {
    if (calc->target->status & BTL_STATUS_SHELL) {
        calc->multiplier /= 2;
    }
}

void btl_applyHealBoost(BtlCalc *calc) {
    if (calc->caster->support[1] & SA_BIT(SA_CONCENTRATE)) {
        calc->multiplier = calc->multiplier * 3 >> 1;
    }
}

void btl_applyRowMod(BtlCalc *calc) {
    BtlUnit *target = calc->target;
    BtlUnit *caster;
    s32 rowDiff;
    if (calc->caster->obj->rotY == target->obj->rotY || (g_gameState->gameData->ctx.battle->escapeHeld != 0 && (target->flags & BTL_UNIT_PARTY))) {
        calc->multiplier = calc->multiplier * 3 >> 1;
    }
    caster = calc->caster;
    if (!(caster->flags & BTL_UNIT_PARTY) || (!(caster->support[0] & SA_BIT(SA_LONG_REACH)) && !(caster->weapon[0] & 2))) {
        rowDiff = BTL_UNIT_ROW(caster->flags);
        rowDiff -= BTL_UNIT_ROW(calc->target->flags);
        if ((rowDiff < 0 ? -rowDiff : rowDiff) >= 2) {
            calc->multiplier /= 2;
        }
    }
}

void btl_applyWeaponElementBoost(BtlCalc *calc) {
    BtlUnit *caster = calc->caster;
    if (caster->elemBoost & caster->weapon[6]) {
        calc->multiplier = calc->multiplier * 3 >> 1;
    }
}

u8 btl_applyWeaponElement(BtlCalc *calc) {
    BtlUnit *target = calc->target;
    u8 element = calc->caster->weapon[6];
    if (target->elemGuard & element) {
        calc->flags |= BTL_CALC_GUARD;
        return 0;
    }
    if (target->elemHalf & element) {
        calc->multiplier /= 2;
    }
    if (calc->target->elemAbsorb & element) {
        calc->defense = 0;
        calc->flags |= BTL_CALC_ABSORB;
    }
    if (calc->target->elemWeak & element) {
        calc->multiplier = calc->multiplier * 3 >> 1;
    }
    if (element & (ELEM_ICE | ELEM_WATER)) {
        btl_removeStatus(calc->target, BTL_STATUS_HEAT);
    }
    if (element & ELEM_FIRE) {
        btl_removeStatus(calc->target, BTL_STATUS_FREEZE);
    }
    return 1;
}

u8 btl_missIfAirborne(BtlCalc *calc) {
    if ((btl_hasStatus(calc->target, BTL_STATUS_FLOAT) & 0xFF) || btl_hasEnemyFlag(calc->target, BTL_ENEMY_AERIAL) != 0) {
        if (calc->cmd->ability->element & ELEM_EARTH) {
            calc->flags |= BTL_CALC_MISS;
            return 0;
        }
        return 1;
    }
    return 1;
}

void btl_halveForSpread(BtlCalc *calc) {
    BtlCalc8 *cmd = calc->cmd;
    if ((cmd->flags & BTL_CMDINFO_TARGETING) == 1 && (u32)((*(u32 *)cmd->ability & 0xF) - BTL_TGT_MULTI_ANY) < 3 && btl_countActiveUnits(cmd->targets < BTL_TARGETS_FIRST_ENEMY) >= 2) {
        calc->multiplier /= 2;
    }
}

void btl_applyAbilityElementBoost(BtlCalc *calc) {
    if (calc->caster->elemBoost & calc->cmd->ability->element) {
        calc->multiplier = calc->multiplier * 3 >> 1;
    }
}

void btl_applyInvertedPower(Unk800BA938 *calc) {
    if ((g_gameState->gameData->ctx.battle->scene.flags >> BTL_SCENE_INVERT_POWER_BIT) & 1) {
        calc->power = 0x3C - calc->power;
        if (calc->power <= 0) {
            calc->power = 1;
        }
    }
}

void btl_calcHeal(BtlCalc *calc) {
    s32 heal = calc->power * calc->multiplier;
    BtlUnit *target;
    if (heal >= 10000) {
        heal = 9999;
    }
    target = calc->target;
    calc->targetResult |= BTL_POPUP_HP;
    if (!(target->status & BTL_STATUS_ZOMBIE) && btl_hasEnemyFlag(target, BTL_ENEMY_UNDEAD) == 0) {
        calc->targetResult |= BTL_POPUP_HEAL;
    }
    calc->targetHpChange = heal;
}

void btl_finishMagicDamage(BtlCalc *calc) {
    s32 base = calc->power - calc->defense;
    s32 damage;
    if (base <= 0) {
        base = 1;
    }
    calc->targetResult |= BTL_POPUP_HP;
    if (calc->multiplier <= 0) {
        calc->multiplier = 1;
    }
    damage = base * calc->multiplier;
    if ((calc->cmd->flags >> BTL_CMDINFO_SHORT_SUMMON_BIT) & 1) {
        damage = damage * 2 / 3;
    }
    if (damage >= 10000) {
        damage = 9999;
    }
    if (calc->flags & BTL_CALC_ABSORB) {
        calc->targetResult |= BTL_POPUP_HEAL;
    }
    calc->targetHpChange = damage;
}

void btl_calcPercentDamage(BtlCalc *calc) {
    s32 damage;
    u8 result;
    if (calc->multiplier > 100) {
        calc->multiplier = 100;
    }
    damage = calc->target->max->maxHp * calc->multiplier / 100;
    if ((calc->cmd->flags >> BTL_CMDINFO_SHORT_SUMMON_BIT) & 1) {
        damage = damage * 2 / 3;
    }
    if (damage >= 10000) {
        damage = 9999;
    }
    result = calc->targetResult;
    calc->targetResult = result | BTL_POPUP_HP;
    if (calc->flags & BTL_CALC_ABSORB) {
        calc->targetResult = result | (BTL_POPUP_HP | BTL_POPUP_HEAL);
    }
    calc->targetHpChange = damage;
}

void btl_rollAbilityStatus(BtlCalc *calc) {
    s32 roll;
    s32 rate;
    RAND_REFILL2();
    roll = *g_randPtr;
    rate = calc->cmd->ability->rate;
    g_randPtr++;
    if (((roll << 8) | roll) % 100 < rate) {
        btl_addStatuses(calc->target, g_gameState->gameData->ctx.battle->statusSets[calc->cmd->ability->statusSet]);
    }
}

void btl_applyAbilityStatus(BtlCalc *calc) {
    BtlCalc8 *cmd = calc->cmd;
    u32 statuses = g_gameState->gameData->ctx.battle->statusSets[cmd->ability->statusSet];
    u32 result;
    if (!((cmd->flags >> BTL_CMDINFO_SHORT_SUMMON_BIT) & 1) && cmd->cmdNo == BTL_CMD_SUMMON2) {
        statuses |= 0x800000;
    }
    result = btl_addStatuses(calc->target, statuses);
    if (result == 0) {
        calc->flags |= BTL_CALC_GUARD;
    } else if (result == 1) {
        calc->flags |= BTL_CALC_MISS;
    }
}

void btl_cureAbilityStatus(BtlCalc *calc) {
    if (btl_removeStatuses(calc->target, g_gameState->gameData->ctx.battle->statusSets[calc->cmd->ability->statusSet]) != 2) {
        calc->flags |= BTL_CALC_MISS;
    }
}

void btl_cureItemStatus(BtlCalc *calc) {
    if (btl_removeStatuses(calc->target, g_usableItemTable[calc->cmd->subNo - ITEM_FIRST_USABLE].statuses) != 2) {
        calc->flags |= BTL_CALC_MISS;
    }
}

void btl_payMpAttack(BtlCalc *calc) {
    if (calc->flags & BTL_CALC_MP_ATTACK) {
        BtlUnit *u = calc->caster;
        s32 cost = u->max->maxMp >> 3;
        if (cost == 0 || u->mp < cost) {
            u->mp = 0;
        } else {
            u->mp -= cost;
        }
    }
}

u8 btl_canFlee(BtlCalc *calc) {
    Battle *b = g_gameState->gameData->ctx.battle;
    BtlUnit *u;
    if ((u8)b->scene.flags >> BTL_SCENE_CAN_ESCAPE_BIT == 0) {
        return 0;
    }
    for (u = b->unitList; u != NULL; u = u->next) {
        if ((u->flags & BTL_UNIT_PARTY) && !(u->status & (BTL_STATUS_PETRIFY | BTL_STATUS_VENOM | BTL_STATUS_ZOMBIE | BTL_STATUS_KO | BTL_STATUS_STOP | BTL_STATUS_SLEEP | BTL_STATUS_FREEZE | BTL_STATUS_JUMP))) {
            return 1;
        }
    }
    return 0;
}

void btl_markTrouble(BtlCalc *calc) {
    BtlUnit *target = calc->target;
    if (target->status & BTL_STATUS_TROUBLE) {
        target->popupFlags |= BTL_POPUP_TROUBLE;
    }
}

void btl_calcEnemySteal(BtlCalc *calc) {
    s16 itemId = calc->cmd->ability->rate;
    if (item_getCount(itemId) == 0) {
        btl_showMessage(BTL_MSG_STEAL_FAILED);
    } else {
        btl_useItem(itemId);
        btl_showMessage(BTL_MSG_WAS_STOLEN, btl_getItemName(itemId));
    }
}
