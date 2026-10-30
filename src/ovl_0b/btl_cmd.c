#include "ovl_0b.h"

void btl_startBattle(void) {
    Battle *b;
    Battle11C0 *p;

    b = g_gameState->gameData->ctx.battle;
    p = b->work;
    btlbg_init(p->bgHeader, p->bgTexAnims);
    b->loadFlags |= 1;
    btl_initCmdQueue(b);
    btl_initTargetCursor(b);
    func_800C972C();
    g_btlRewards.unkB = (b->scene.unk0 >> 13) & 1;
    msg_beginDraw();
    evt_update();
    btl_loadModelsStep(1);
    fade_in(0x20);
    btlcam_playScene(btl_getSceneData(0));
    b->phase = BTL_PHASE_INTRO;
}

void btl_updateCombat(GameData *arg0, Battle *arg1) {
    BtlUnit *u;
    s32 sp10;
    u = arg1->unitList;
    sp10 = arg1->work->prims[arg0->drawBuf];
    while (u != NULL) {
        if (((u->flags >> 29) & 1) && (arg1->cmdFlags & 2)) {
            btl_projectUnitPos(u);
        } else {
            u->onScreen = 0;
        }
        u = u->next;
    }
    u = arg1->unitList;
    while (u != NULL) {
        if (!((u->flags >> 22) & 1)) {
            btl_updateUnitModel(u, &sp10);
        }
        if (btlui_getActiveUnit() == u && (arg1->cmdFlags & 2) && !(u->model->flags & 0x10)) {
            btl_drawActiveMarker(arg0, arg1, u);
        }
        btl_updateHpStatus(u);
        btl_updateUnitStatus(u);
        u = u->next;
    }
    btl_processCmdQueue(arg1);
    btl_drawScene(arg0, arg1, sp10);
}

void btl_drawScene(GameData *arg0, Battle *arg1, s32 arg2) {
    if (arg1->unk11BE) {
        arg1->unk11BE -= 1;
    }
    btlseq_update();
    arg2 = (s32)btlbg_draw(&arg0->viewMatrix, arg1->work->bgData, arg2, arg0->ot[arg0->drawBuf],
                              arg1->work->ot[arg0->drawBuf]);
    arg2 = (s32)btlfx_runFrame((u8 *)arg2);
    arg2 = (s32)btl_drawPopups((u8 *)arg2);
    btl_drawBanner((u8 *)arg2);
}

s32 btl_tryCounter(BtlUnit *arg0, BtlUnit *arg1, BtlCmd *arg2) {
    s32 r;
    if (arg0->status & (BTL_STATUS_PETRIFY | BTL_STATUS_VENOM | BTL_STATUS_KO | BTL_STATUS_STOP | BTL_STATUS_FREEZE | BTL_STATUS_JUMP)) {
        return 0;
    }
    if (arg2->cmdNo >= BTL_CMD_TABLE_COUNT) {
        return 0;
    }
    if ((arg0->support[1] & 0x10) && (arg2->ability->flags & 8)) {
        r = arg0->spirit;
        if (arg0->support[1] & 0x80) {
            r *= 2;
        }
        if (g_randPtr >= g_randTableEnd) {
            rand_refill();
            if (g_randPtr >= g_randTableEnd) {
                rand_refill();
            }
        }
        if (((*g_randPtr << 8) | *g_randPtr++) % 100 < r) {
            btl_queueAutoCmd(arg0, BTL_CMD_COUNTER, BTL_ABIL_ATTACK, arg1->unitBit);
            return 1;
        }
    }
    if ((arg0->support[1] & 0x400000) && (arg2->ability->flags & 0x80)) {
        btl_queueAutoCmd(arg0, BTL_CMD_RETURN_MAGIC, arg2->subNo, arg1->unitBit);
        return 1;
    }
    return 0;
}

u16 btl_findCover(s32 arg0) {
    BtlUnit *best = NULL;
    BtlUnit *t = btl_getUnitById(arg0);
    BtlUnit *u;
    GameData *g;
    s32 v;
    if (t->status & (BTL_STATUS_PETRIFY | BTL_STATUS_KO)) {
        return 0;
    }
    g = g_gameState->gameData;
    if (((g->characters + ((t->flags >> 1) & 0xF))->classFlags & 2) && t->hp < (t->max->maxHp >> 1)) {
        for (u = g->ctx.battle->unitList; u != NULL; u = u->next) {
            if ((u->support[1] & 0x40) && !(u->status & (BTL_STATUS_PETRIFY | BTL_STATUS_VENOM | BTL_STATUS_KO | BTL_STATUS_STOP | BTL_STATUS_HEAT | BTL_STATUS_FREEZE | BTL_STATUS_JUMP)) && u != t && (best == NULL || best->hp < u->hp)) {
                best = u;
            }
        }
    }
    if (best == NULL && (t->status & BTL_STATUS_LOW_HP)) {
        for (u = g_gameState->gameData->ctx.battle->unitList; u != NULL; u = u->next) {
            if ((u->support[1] & 0x20) && !(u->status & (BTL_STATUS_PETRIFY | BTL_STATUS_VENOM | BTL_STATUS_KO | BTL_STATUS_STOP | BTL_STATUS_HEAT | BTL_STATUS_FREEZE | BTL_STATUS_JUMP)) && u != t && (best == NULL || best->hp < u->hp)) {
                best = u;
            }
        }
    }
    if (best == NULL) {
        return 0;
    }
    v = (best->flags & 1) << 11;
    best->obj->rotY = v;
    best->rot[1] = v;
    best->pos[0] = t->pos[0];
    best->pos[2] = t->pos[2];
    t->pos[2] += 0xFE700000;
    best->model->motion = (BtlUnit9CC *)best->motions[15];
    best->flags |= 0x2000000;
    return best->unitBit;
}

void btl_applyAutoAbilities(BtlUnit *arg0) {
    if (arg0->support[0] & 1) {
        arg0->autoStatus |= BTL_STATUS_REFLECT;
    }
    if (arg0->support[0] & 2) {
        arg0->autoStatus |= BTL_STATUS_FLOAT;
    }
    if (arg0->support[0] & 4) {
        arg0->autoStatus |= BTL_STATUS_HASTE;
        arg0->statusImmune |= BTL_STATUS_SLOW;
        arg0->atbSpeed = arg0->atbSpeed * 3 / 2;
    }
    if (arg0->support[0] & 8) {
        arg0->autoStatus |= BTL_STATUS_REGEN;
        btl_initDotTimer(arg0, 0x12);
    }
    if (arg0->support[0] & 0x10) {
        arg0->status |= BTL_STATUS_AUTO_LIFE;
    }
    if (arg0->support[1] & 0x100) {
        arg0->statusImmune |= (BTL_STATUS_HEAT | BTL_STATUS_FREEZE);
    }
    if (arg0->support[1] & 0x10000) {
        arg0->statusImmune |= BTL_STATUS_SLEEP;
    }
    if (arg0->support[1] & 0x20000) {
        arg0->statusImmune |= (BTL_STATUS_VENOM | BTL_STATUS_POISON);
    }
    if (arg0->support[1] & 0x40000) {
        arg0->statusImmune |= BTL_STATUS_BLIND;
    }
    if (arg0->support[1] & 0x80000) {
        arg0->statusImmune |= BTL_STATUS_SILENCE;
    }
    if (arg0->support[1] & 0x200000) {
        arg0->statusImmune |= (BTL_STATUS_PETRIFY | BTL_STATUS_GRADUAL_PETRIFY);
    }
    if (arg0->support[1] & 0x02000000) {
        arg0->statusImmune |= BTL_STATUS_STOP;
    }
    if (arg0->support[1] & 0x04000000) {
        arg0->statusImmune |= BTL_STATUS_CONFUSE;
    }
}

s32 btl_partyHasSupport(s32 arg0, u32 arg1) {
    s32 i;
    GameData *gd = g_gameState->gameData;
    for (i = 0; i < 4; i++) {
        if (gd->party[i]->supportAbilities[arg0] & arg1) {
            return 1;
        }
    }
    return 0;
}

void btl_tryAutoPotion(BtlUnit *arg0, u8 arg1) {
    s32 i;
    if (!(arg0->status & (BTL_STATUS_PETRIFY | BTL_STATUS_VENOM | BTL_STATUS_KO | BTL_STATUS_STOP | BTL_STATUS_FREEZE | BTL_STATUS_JUMP)) && arg1 < 0x30 && (arg0->support[1] & 0x01000000)) {
        for (i = 0xEC; (u8)i < 0xEE; i++) {
            if (item_getCount((u8)i) != 0) {
                btl_reserveItem((u8)i);
                btl_queueAutoCmd(arg0, BTL_CMD_AUTO_POTION, (u8)i, arg0->unitBit);
                return;
            }
        }
    }
}

void btl_applyHitSupports(BtlUnit *arg0, u8 *arg1) {
    u32 max;
    u32 lt;
    s32 h;
    if (!(arg0->status & (BTL_STATUS_PETRIFY | BTL_STATUS_VENOM | BTL_STATUS_KO | BTL_STATUS_STOP | BTL_STATUS_FREEZE | BTL_STATUS_JUMP))) {
        if ((arg0->support[1] & 0x100000) && arg0->hp != 0 && (arg0->status & BTL_STATUS_LOW_HP)) {
            max = arg0->max->maxHp;
            h = arg0->hp;
            lt = (s32)(h + (max >> 1)) < (s32)max;
            if (lt) {
                arg0->hp = h + (max >> 1);
            } else {
                arg0->hp = max;
            }
        }
        if ((arg0->support[1] & 0x800000) && arg1[0xA] != 0) {
            if (arg0->mp + arg1[0xA] < arg0->max->maxMp) {
                arg0->mp += arg1[0xA];
            } else {
                arg0->mp = arg0->max->maxMp;
            }
        }
    }
}

void btl_initCmdQueue(Battle *arg0) {
    arg0->curCmd = NULL;
    arg0->cmdPhase = 0;
    arg0->cmdFlags = 2;
    arg0->escapeCmd.caster = NULL;
    arg0->cmdQueue.caster = NULL;
    btl_clearCmd_inline(&arg0->cmdQueue);
    btl_clearCmd_inline(&arg0->escapeCmd);
}

void btl_initTargetCursor(Battle *arg0) {
    s8 i;
    s8 j;
    s32 c;
    for (i = 0; i < 2; i++) {
        for (j = 0; j < 4; j++) {
            gfx_initTpagePolyF3(&arg0->cursorPrims[i].unk0[j].unk0, GetTPage(0, 1, 0, 0));
            if (j < 3) {
                c = 0xBC - j * 0x3C;
            } else {
                c = 0xA0;
            }
            setRGB0(&arg0->cursorPrims[i].unk0[j].unk8, c, c, 0);
            SetSemiTrans(&arg0->cursorPrims[i].unk0[j].unk8, 1);
        }
    }
}

void btl_initUnitCmds(BtlUnit *arg0) {
    Battle *b = g_gameState->gameData->ctx.battle;
    BtlCmd *c;
    s32 k;
    s32 i;
    s32 j;
    if (b->phase == BTL_PHASE_INTRO) {
        arg0->unkB2 = 1;
    }
    arg0->cmdIssued = 0;
    arg0->onScreen = 0;
    if (arg0->flags & 1) {
        k = ((arg0->flags >> 5) & 7) * 6;
    } else {
        k = (((arg0->flags >> 5) & 7) - 4) * 3 + 0x18;
    }
    c = &b->cmdSlots[k];
    /* unk4..unk18 are the unit's six command slots */
    for (i = 0; i < 6; i++) {
        if ((arg0->flags & 1) || i < 3) {
            (&arg0->cmd)[i] = c;
            c->caster = arg0;
            c++;
            btl_clearCmd_inline((&arg0->cmd)[i]);
        } else {
            (&arg0->cmd)[i] = NULL;
        }
    }
    for (j = 3; j >= 0; j--) {
        arg0->bounceMasks[j] = 0;
    }
}

void btl_setCmd(BtlCmd *arg0, u32 arg1, s32 arg2, s32 arg3, s32 arg4) {
    BtlUnit *u = arg0->caster;
    BtlUnit9C *m;
    s32 *a;
    BtlUnit9CC *c;

    switch (arg1) {
    case BTL_CMD_SPEAR:
    case BTL_CMD_SPEAR_TRANCE:
        g_gameState->gameData->ctx.battle->cmdFlags |= 0x10;
        arg0->ability = (BtlCmd8 *)&g_gameState->gameData->ctx.battle->abilities[arg2];
        break;
    case BTL_CMD_THROW:
        arg0->ability = (BtlCmd8 *)&g_gameState->gameData->ctx.battle->abilities[BTL_ABIL_THROW];
        break;
    case BTL_CMD_RETURN_MAGIC:
        arg0->ability = &g_gameState->gameData->ctx.battle->enemyAbilities[arg2];
        break;
    case BTL_CMD_ESCAPE:
        if (g_gameState->gameData->ctx.battle->cmdFlags & 1) {
            arg0->subNo = arg2;
            return;
        }
        g_gameState->gameData->ctx.battle->cmdFlags |= 1;
        arg0->ability = (BtlCmd8 *)&g_gameState->gameData->ctx.battle->abilities[BTL_ABIL_FLEE];
        break;
    case BTL_CMD_ITEM:
    case BTL_CMD_AUTO_POTION:
    case BTL_CMD_KO:
    case BTL_CMD_AUTO_LIFE:
    case BTL_CMD_PETRIFY:
        arg0->ability = (BtlCmd8 *)&g_gameState->gameData->ctx.battle->abilities[0];
        break;
    case BTL_CMD_STEAL:
        if (u->support[1] & 0x20000000) {
            arg2 = BTL_ABIL_MUG;
        }
    default:
        arg0->ability = (BtlCmd8 *)&g_gameState->gameData->ctx.battle->abilities[arg2];
        break;
    }
    arg0->targets = arg3;
    arg0->cmdNo = arg1;
    arg0->subNo = arg2;
    arg0->unk10.w = (arg0->unk10.w & ~3) | (arg4 & 3);
    arg0->unk10.w &= ~0x20;
    arg0->unk10.w &= ~0x40;
    arg0->unk10.w &= ~0x180;
    if (arg1 >= 0x31) {
        arg0->unk10.w |= 0x10;
    } else {
        btl_removeStatus(u, BTL_STATUS_DEFEND);
    }
    if (arg1 < 0x37) {
        if (g_gameState->gameData->ctx.battle->curCmd != u->tranceCmd) {
            m = u->model;
            a = u->motions;
            c = m->motion;
            if (c == (BtlUnit9CC *)a[0]) {
                m->motion = (BtlUnit9CC *)a[10];
                u->obj->frame = 0;
            } else if (c == (BtlUnit9CC *)a[1]) {
                m->motion = (BtlUnit9CC *)a[11];
                u->obj->frame = 0;
            } else if (c == (BtlUnit9CC *)a[13] && arg1 < 0x30) {
                m->motion = (BtlUnit9CC *)a[14];
                u->obj->frame = 0;
            }
        }
        u->flags |= 0x80000;
    }
    btl_enqueueCmd(arg0);
}

void btl_setEnemyCmd(s32 arg0, s16 arg1, u32 arg2, u32 arg3) {
    Battle *b;
    BtlUnit *u;
    BtlCmd *c;

    u = btl_getUnitById(arg0);
    b = g_gameState->gameData->ctx.battle;
    if (b->cmdFlags & 1 || (u->status & (BTL_STATUS_PETRIFY | BTL_STATUS_VENOM | BTL_STATUS_KO | BTL_STATUS_STOP | BTL_STATUS_SLEEP | BTL_STATUS_FREEZE))) {
        goto fail;
    }
    if (arg2 == 0x2F) {
        if (b->phase != BTL_PHASE_COMBAT) {
            goto fail;
        }
        c = u->cmd;
        if (u->status & BTL_STATUS_CONFUSE) {
            arg1 = btl_pickRandomUnit(RAND8() & 1);
            arg3 = g_gameState->gameData->ctx.battle->enemies[(u->flags >> 1) & 0xF].type->confuseAbility;
        } else if (u->status & BTL_STATUS_BERSERK) {
            arg1 = btl_pickRandomUnit(1);
            arg3 = g_gameState->gameData->ctx.battle->enemies[(u->flags >> 1) & 0xF].type->confuseAbility;
        }
    } else {
        if (arg2 != 0x35) {
            if (arg2 != 0x36) {
fail:
                u->cmdIssued = 0;
                return;
            }
        }
        c = u->autoCmd;
    }
    c->ability = &g_gameState->gameData->ctx.battle->enemyAbilities[arg3];
    c->targets = arg1;
    c->cmdNo = arg2;
    c->subNo = arg3;
    if ((u32)(c->ability->targetType - 6) < 7) {
        c->unk10.b.targeting = 1;
    } else {
        c->unk10.b.targeting = 0;
    }
    if (arg2 > 0x30) {
        c->unk10.b.automatic = 1;
    } else {
        btl_removeStatus(c->caster, BTL_STATUS_DEFEND);
    }
    c->unk10.b.covered = 0;
    c->unk10.b.dodged = 0;
    c->unk10.b.reflect = 0;
    u->flags |= 0x80000;
    btl_enqueueCmd(c);
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
            if (!((cmd->cmdNo < 0x3C && cmd->cmdNo != BTL_CMD_ESCAPE && cmd->cmdNo != BTL_CMD_REBIRTH_FLAME && (cmd->caster->status & (BTL_STATUS_PETRIFY | BTL_STATUS_VENOM | BTL_STATUS_STOP | BTL_STATUS_FREEZE))) ||
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
                w->model->motion = (BtlUnit9CC *)w->motions[9];
                w->obj->frame = 0;
            }
            if ((w->flags & 1) && (s32)w->model->motion != w->motions[9] && (s32)w->model->motion != w->motions[17] &&
                !(w->status & BTL_STATUS_JUMP)) {
                if ((s32)w->model->motion == w->motions[(w->flags >> 18) & 1] && !((w->flags >> 19) & 1)) {
                    w->model->motion = (BtlUnit9CC *)w->motions[9];
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
            if (u != NULL && (u->flags & 1) && (s32)u->model->motion == u->motions[9]) {
                u->model->motion = (BtlUnit9CC *)u->motions[32];
            }
            b->cmdPhase = 3;
            break;
        }
        if (u != NULL && (s32)u->model->motion != u->motions[9] && cmd->cmdNo < 0x2F) {
            u->model->motion = (BtlUnit9CC *)u->motions[9];
            u->obj->frame = 0;
        }
        b->cmdPhase++;
        break;
    case 1:
        btl_startCmdEffect(cmd);
        b->cmdPhase++;
        break;
    case 2:
        if ((u->flags & 1) || cmd->unk10.b.unk0_11) {
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

s32 btl_checkCmdUsable(Battle *arg0, BtlCmd *arg1) {
    BtlUnit *u;
    BtlUnit *t;
    BtlCmd *m;
    BtlCmd8 *c;
    u32 a;
    u32 g;
    s32 r;
    s32 k;
    s32 msg;
    u8 st;
    s32 v;

    u = arg1->caster;
    if (u != NULL) {
        if (arg1->cmdNo < 0x37) {
            v = (u->flags & 1) << 11;
            u->obj->rotY = v;
            u->rot[1] = v;
        }
        if (D_8007AF01 != 0 && (u->flags & 0x1F) == 5 && arg1->cmdNo < BTL_CMD_TABLE_COUNT) {
            if (evt_rollEncounter_rand() < 0x40) {
                msg = 0x1B;
                goto fail;
            }
        }
        if (btl_checkCmdUsable_can(arg1) == 0) {
            return 0;
        }
    }
    switch (arg1->cmdNo) {
    case BTL_CMD_ATTACK:
        if (((arg0->scene.unk0 >> 8) & 1) && (*u->weapon & 1) && (u16)arg1->targets >= 0x10) {
            msg = 0x17;
            goto fail;
        }
        return 1;
    case BTL_CMD_JUMP:
    case BTL_CMD_JUMP_TRANCE:
        btl_addStatus(u, BTL_STATUS_JUMP);
        u->delayedCmd->cmdNo = arg1->cmdNo;
        m = u->delayedCmd;
        if (arg1->cmdNo == BTL_CMD_JUMP) {
            m->targets = arg1->targets;
        } else {
            m->targets = btl_getUnitsWithStatus(1, 0);
        }
        arg1->targets = u->unitBit;
        break;
    case BTL_CMD_SPEAR_TRANCE:
        u->delayedCmd->targets = btl_getUnitsWithStatus(1, 0);
        break;
    case BTL_CMD_ITEM:
    case BTL_CMD_THROW:
    case BTL_CMD_AUTO_POTION:
        if (item_getCount(arg1->subNo) == 0) {
            msg = 0x18;
            goto fail;
        }
        if (arg1->cmdNo == BTL_CMD_AUTO_POTION) {
            btl_showMessage(BTL_MSG_AUTO_POTION);
        }
        return 1;
    case BTL_CMD_SUMMON:
    case BTL_CMD_EIDOLON:
    case BTL_CMD_SUMMON2:
        btl_rollShortSummon(arg1);
        return 1;
    case BTL_CMD_BLK_MAG:
    case BTL_CMD_DBL_BLK:
        if (arg1->subNo == 0x2E) {
            func_800B95BC(arg1);
        }
        return 1;
    case BTL_CMD_BLU_MAG:
        if (arg1->subNo == 0x52) {
            arg1->targets = btl_pickRandomUnit(evt_rollEncounter_rand() & 1);
        } else if (arg1->subNo == 0x5D) {
            a = item_getCount(arg1->ability->power);
            g = btl_countActiveUnits(1);
            msg = 0x18;
            if (a >= g) {
                break;
            }
            goto fail;
        } else {
            return 1;
        }
        break;
    case BTL_CMD_FLAIR:
    case BTL_CMD_ELAN:
        if (arg1->subNo == 0x7E || arg1->subNo == 0x86) {
            g = arg1->ability->power * u->level;
            if (g_gameState->gameData->gil < g) {
                msg = 0x14;
                goto fail;
            }
            g_gameState->gameData->gil -= g;
        } else if (arg1->subNo == 0x81 || arg1->subNo == 0x89) {
            c = arg1->ability;
            r = evt_rollEncounter_rand();
            c->element = 1 << (r % 8);
        } else {
            return 1;
        }
        break;
    case BTL_CMD_SWD_MAG:
        return (u8)btl_checkSwdMagUsable(u, arg1->ability->mpCost);
    case BTL_CMD_COUNTER:
        t = btl_getUnitById(arg1->targets);
        if (t == NULL || !((t->flags >> 28) & 1) || (t->status & BTL_STATUS_KO)) {
            goto ret0;
        }
    case BTL_CMD_CHARGE_ATTACK:
        if ((arg0->scene.unk0 >> 8) & 1) {
            msg = 0x17;
            if (*u->weapon & 1) {
            fail:
                btl_showMessage(msg);
                return 0;
            }
        }
        if (arg1->cmdNo == BTL_CMD_COUNTER) {
            btl_showMessage(BTL_MSG_COUNTER);
        }
        return 1;
    case BTL_CMD_RETURN_MAGIC:
        t = btl_getUnitById(arg1->targets);
        if (t != NULL && ((t->flags >> 28) & 1) && !(t->status & BTL_STATUS_KO)) {
            btl_showMessage(BTL_MSG_RETURN_MAGIC);
            return 1;
        }
    ret0:
        return 0;
    case BTL_CMD_ESCAPE:
        if (arg0->phase != BTL_PHASE_COMBAT) {
            return 1;
        }
        for (t = arg0->unitList; t != NULL; t = t->next) {
            if ((t->flags & 1) && !(t->status & (BTL_STATUS_PETRIFY | BTL_STATUS_VENOM | BTL_STATUS_ZOMBIE | BTL_STATUS_KO | BTL_STATUS_STOP | BTL_STATUS_SLEEP | BTL_STATUS_FREEZE | BTL_STATUS_JUMP)) && t->hp != 0) {
                if (t->model->motion != (BtlUnit9CC *)t->motions[0x11]) {
                    t->model->motion = (BtlUnit9CC *)t->motions[0x11];
                    t->obj->frame = 0;
                }
                arg0->phase = BTL_PHASE_END;
                arg0->subPhase = BTL_END_ESCAPE;
            }
        }
        if (*(u16 *)&arg0->phase == 0x305) {
            btlui_setAtbEnabled(0);
            g_gameState->gameData->escapeCount++;
            if (arg1->subNo == 0) {
                g_gameState->gameData->battleFlags |= 4;
            }
            btl_clearCmdQueue(arg0);
        } else {
            arg0->cmdFlags &= ~1;
        }
        return 0;
    case BTL_CMD_TRANCE_EIDOLON:
        if (arg1->subNo != arg0->eidolonAbility) {
            arg1->subNo = arg0->eidolonAbility;
            arg1->ability = (BtlCmd8 *)&arg0->abilities[arg1->subNo];
        }
        break;
    case BTL_CMD_TRANCE:
        if (u->status & BTL_STATUS_TRANCE) {
            btl_showMessage(BTL_MSG_TRANCE);
            u->modelId = g_btlCharModelIds[(u->flags & 1) ? g_gameState->gameData->characters[(u->flags >> 1) & 0xF].info.b.kind + 0x13 : 0x26];
        } else {
            u->modelId = g_btlCharModelIds[(u->flags & 1) ? (g_gameState->gameData->characters[(u->flags >> 1) & 0xF].info.w >> 4) & 0x1F : 0x13];
        }
        break;
    case BTL_CMD_KO:
        btl_checkPartyDisabled(u);
        if (u->koState == 0) {
            if (u->flags & 1) {
                if (u->model->motion == (BtlUnit9CC *)u->motions[4]) {
                    u->koState = 5;
                    return 0;
                }
                u->koState = 1;
            } else if ((u->flags & 0x900000) == 0x100000) {
                u->koState = 3;
            } else {
                u->koState = 1;
            }
        }
        return 0;
    case BTL_CMD_AUTO_LIFE:
        u->hp = 1;
        btl_removeStatus(u, BTL_STATUS_KO);
        u->flags |= 0x20000;
        return 0;
    case BTL_CMD_PETRIFY:
        btl_interruptUnit(u, 1);
        u->status |= BTL_STATUS_PETRIFY;
        u->flags &= ~0x10000;
        btl_checkSideDefeated(u);
        if (u->flags & 1) {
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
    BtlCmd8 *a;
    BtlUnit *u;
    Unk8006B1F4 *tbl;
    s32 i;
    u16 mask = 0;
    u16 f;
    u16 r;

    if ((u16)cmd->targets != 0) {
        switch (cmd->cmdNo) {
        case BTL_CMD_ITEM:
        case BTL_CMD_AUTO_POTION:
            tbl = D_8006B1F4;
            i = cmd->subNo - 0xE0;
            a = (BtlCmd8 *)(tbl + i);
            f = (*(u32 *)a >> 29) & 1;
            break;
        case BTL_CMD_TRANCE:
            return 1;
        default:
            a = cmd->ability;
            f = (*(u32 *)a >> 29) & 1;
            break;
        }
        for (u = b->unitList; u != NULL; u = u->next) {
            if (((u->flags >> 28) & 1) && (u->unitBit & (u16)cmd->targets) &&
                ((f && (u->flags & 1)) || !(u->status & BTL_STATUS_KO))) {
                mask |= u->unitBit;
            }
        }
        if (mask) {
            cmd->targets = mask;
            return 1;
        }
        if (cmd->unk10.w & 3) {
            return 0;
        }
        if (!f) {
            r = cmd->targets = btl_pickRandomUnit(cmd->targets & 0xF);
            if (r) {
                return 1;
            }
        }
    }
    return 0;
}

void btl_updatePartyCmd(BtlCmd *arg0) {
    Battle *b;
    BtlUnit *u;
    BtlUnit9C *m;
    s32 *a;
    s32 *a2;
    BtlUnit9C *m2;
    BtlUnit9CC *c;
    s32 k;
    u16 n;
    u16 f;

    if (g_btlfxEnv->state->phase != -1) {
        return;
    }
    b = g_gameState->gameData->ctx.battle;
    u = arg0->caster;
    if (b->popups.scanUnit != 0 || b->popups.unk1CC != NULL) {
        return;
    }
    if (arg0->cmdNo == BTL_CMD_DEFEND) {
        m = u->model;
        a = u->motions;
        if (m->motion == (BtlUnit9CC *)a[12]) {
            if (u->obj->frame < m->motion->frameCount) {
                return;
            }
            m->motion = (BtlUnit9CC *)a[13];
            u->obj->frame = 0;
        }
    } else if (arg0->cmdNo == BTL_CMD_CHANGE) {
        m = u->model;
        a = u->motions;
        if (m->motion == (BtlUnit9CC *)a[9]) {
            if (u->obj->frame < m->motion->frameCount) {
                return;
            }
            m->motion = (BtlUnit9CC *)a[29 + ((u->flags >> 8) & 3)];
            u->obj->frame = 0;
        }
        a2 = u->motions;
        m2 = u->model;
        k = (u->flags >> 8) & 3;
        c = m2->motion;
        if (c == (BtlUnit9CC *)a2[29 + k]) {
            n = c->frameCount;
            f = u->obj->frame;
            if (f < n) {
                f++;
                if (k != 0) {
                    u->pos[2] = (-(f * 400) / n - 0x618) << 16;
                } else {
                    u->pos[2] = ((f * 400) / n - 0x7A8) << 16;
                }
                return;
            }
            btl_execCmd(u);
            u->model->motion = (BtlUnit9CC *)u->motions[32];
            u->obj->frame = 0;
            return;
        }
        if (c == (BtlUnit9CC *)a2[32]) {
            if (u->obj->frame < c->frameCount) {
                return;
            }
            m2->motion = (BtlUnit9CC *)a2[0];
            u->obj->frame = 0;
        }
    }
    btl_onCmdEffectDone();
    if (arg0->cmdNo == BTL_CMD_TRANCE) {
        btl_reapplyStatusLook(u);
    }
}

void btl_execCmd(BtlUnit *arg0) {
    BtlCmd *cmd = g_gameState->gameData->ctx.battle->curCmd;
    BtlUnit *u = cmd->caster;
    Battle *b;
    u16 s;
    u16 t;
    s32 i;
    Unk8006B1F4 *tbl;

    switch (cmd->cmdNo) {
    case BTL_CMD_ATTACK:
    case BTL_CMD_COUNTER:
    case BTL_CMD_CHARGE_ATTACK:
        btl_calcCmd(u, arg0, cmd, ((BtlCmd8 *)u->weapon)->formula);
        break;
    case BTL_CMD_JUMP:
    case BTL_CMD_JUMP_TRANCE:
        u->unkB2 = 2;
        u->flags |= 0x400000;
        break;
    case BTL_CMD_ITEM:
    case BTL_CMD_AUTO_POTION:
        btl_useReservedItem(cmd->subNo);
        tbl = D_8006B1F4;
        i = cmd->subNo - 0xE0;
        btl_calcCmd(u, arg0, cmd, ((BtlCmd8 *)(tbl + i))->formula);
        break;
    case BTL_CMD_REBIRTH_FLAME:
        s = btl_getUnitMask(1);
        t = btl_getUnitsWithStatus(1, (BTL_STATUS_PETRIFY | BTL_STATUS_VENOM | BTL_STATUS_KO | BTL_STATUS_STOP));
        if (s == 0 || s == t) {
            b = g_gameState->gameData->ctx.battle;
            btlui_setAtbEnabled(0);
            if (b->phase != BTL_PHASE_END) {
                b->phase = BTL_PHASE_END;
                b->subPhase = BTL_END_VICTORY;
                btl_clearCmdQueue(b);
            }
        }
        btl_calcCmd(u, arg0, cmd, cmd->ability->formula);
        break;
    case BTL_CMD_THROW:
        btl_useReservedItem(cmd->subNo);
    default:
        btl_calcCmd(u, arg0, cmd, cmd->ability->formula);
        break;
    }
}

void btl_finishCmd(Battle *b) {
    BtlCmd *cmd = b->curCmd;
    BtlUnit *u = cmd->caster;
    u32 n;

    if (cmd->cmdNo < BTL_CMD_TABLE_COUNT) {
        if ((cmd == u->cmd && cmd->cmdNo != BTL_CMD_JUMP && cmd->cmdNo != BTL_CMD_JUMP_TRANCE) ||
            (cmd->cmdNo == BTL_CMD_DBL_WHT && ((u->flags >> 1) & 0xF) == 6 && cmd == u->delayedCmd && !btl_isCmdPending(u->cmd))) {
            u->atb = 0;
            u->cmdIssued = 0;
        } else if (cmd->cmdNo == BTL_CMD_SPEAR) {
            btl_removeStatus(u, BTL_STATUS_JUMP);
            g_gameState->gameData->ctx.battle->cmdFlags &= ~0x10;
        } else if (cmd->cmdNo == BTL_CMD_SPEAR_TRANCE) {
            btl_addStatus(u, BTL_STATUS_JUMP);
            u->unkB2 = 2;
            u->flags |= 0x400000;
            g_gameState->gameData->ctx.battle->cmdFlags &= ~0x10;
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
                b->cmdFlags |= 4;
                b->eidolonTimer = (0x3C - u->spirit) * 200;
            }
        }
    } else if (cmd->cmdNo == BTL_CMD_TRANCE_EIDOLON) {
        b->cmdFlags &= ~8;
        b->eidolonTimer = (0x3C - u->spirit) * 200;
    } else if (cmd->cmdNo < 0x35 && btl_isCmdPending(u->cmd)) {
        u->flags |= 0x80000;
    }
    if (cmd->unk10.b.covered) {
        for (u = g_gameState->gameData->ctx.battle->unitList; u != NULL; u = u->next) {
            if (u->flags & 1) {
                u->pos[0] = u->home->pos[0];
                u->pos[1] = u->home->pos[1];
                u->pos[2] = u->home->pos[2];
                if ((u->flags >> 25) & 1) {
                    u->flags &= ~0x2000000;
                    btl_setIdleMotion(u);
                }
            }
        }
    }
    if (cmd->unk10.b.dodged) {
        u = btl_getUnitById(cmd->targets);
        u->flags &= ~0x4000000;
        if (u->flags & 1) {
            btl_setIdleMotion(u);
        } else if ((u->flags >> 23) & 1) {
            u = btl_findBodyUnit();
        }
        u->pos[2] = u->home->pos[2];
    }
    btl_clearCmd_inline(cmd);
    b->curCmd = NULL;
    b->cmdPhase = 0;
}

void btl_flushCmdQueue(Battle *arg0) {
    BtlCmd *p = &arg0->cmdQueue;
    BtlCmd *c;
    BtlUnit *u;
    BtlUnit9C *m;
    Battle *b;
    s32 id;
    while (p != NULL) {
        c = p->next;
        if (c != NULL && c->cmdNo != BTL_CMD_ENEMY_DEATH && c->cmdNo < 0x3A) {
            u = c->caster;
            b = g_gameState->gameData->ctx.battle;
            if (c->cmdNo < BTL_CMD_TABLE_COUNT && u != NULL && !(u->status & BTL_STATUS_KO)) {
                m = u->model;
                u->cmdIssued = 0;
                if ((s32)m->motion == u->motions[9] && ((id = (u->flags >> 1) & 0xF) != 6 || b->curCmd->cmdNo != BTL_CMD_DBL_WHT) && (id != 1 || (b->curCmd->cmdNo != BTL_CMD_SWD_MAG && b->curCmd->cmdNo != BTL_CMD_DBL_BLK))) {
                    m->motion = (BtlUnit9CC *)u->motions[32];
                    u->obj->frame = 0;
                    u->flags &= ~0x80000;
                }
            }
            btl_releaseItem(c);
            btl_unlinkCmd(&p->next, 1);
        } else {
            p = c;
        }
    }
}

void btl_clearCmdQueue(Battle *arg0) {
    BtlCmd *p = &arg0->cmdQueue;
    BtlCmd *c;
    BtlUnit *u;
    BtlUnit9C *m;
    Battle *b;
    s32 id;
    while (p != NULL) {
        c = p->next;
        if (c != NULL && c->cmdNo < 0x3B) {
            u = c->caster;
            b = g_gameState->gameData->ctx.battle;
            if (c->cmdNo < BTL_CMD_TABLE_COUNT && u != NULL && !(u->status & BTL_STATUS_KO)) {
                m = u->model;
                u->cmdIssued = 0;
                if ((s32)m->motion == u->motions[9] && ((id = (u->flags >> 1) & 0xF) != 6 || b->curCmd->cmdNo != BTL_CMD_DBL_WHT) && (id != 1 || (b->curCmd->cmdNo != BTL_CMD_SWD_MAG && b->curCmd->cmdNo != BTL_CMD_DBL_BLK))) {
                    m->motion = (BtlUnit9CC *)u->motions[32];
                    u->obj->frame = 0;
                    u->flags &= ~0x80000;
                }
            }
            btl_releaseItem(c);
            btl_unlinkCmd(&p->next, 1);
        } else {
            p = c;
        }
    }
    arg0->eidolonTimer = 0;
    arg0->eidolonAbility = 0;
    arg0->cmdFlags &= ~0xC;
}

void btl_enqueueCmd(BtlCmd *arg0) {
    BtlCmd *p = &g_gameState->gameData->ctx.battle->cmdQueue;
    BtlCmd *n;
    s32 c;
    u32 e;

    for (; p != NULL; p = p->next) {
        if (arg0->unk10.b.automatic) {
            n = p->next;
            if (n == NULL) {
                goto insert;
            }
            if (arg0->cmdNo >= 0x3C || arg0->cmdNo == BTL_CMD_ENEMY_DEATH) {
                e = n->cmdNo;
                c = e < 0x3C;
                goto chk;
            }
            if (arg0->cmdNo == BTL_CMD_TRANCE) {
                e = n->cmdNo;
                c = e < 0x3B;
            chk:
                if (c == 0 || e == 0x36) {
                    continue;
                }
            insert:
                btl_linkCmd((LinkNode *)arg0, (LinkNode **)&p->next);
                return;
            }
            c = n->unk10.b.automatic;
        } else {
            c = (s32)p->next;
        }
        if (c == 0) {
            goto insert;
        }
    }
}

s32 btl_applyReflect(BtlCmd *arg0) {
    u16 buf[4];
    s16 n;
    s16 m;
    s32 i;
    u32 mask = 0;
    u32 side;
    BtlUnit *v;

    if ((u32)(arg0->cmdNo - 14) >= 2 && arg0->cmdNo != BTL_CMD_AUTO_POTION && (arg0->ability->flags & 1) &&
        !(arg0->caster->support[0] & 0x40000000)) {
        n = 0;
        m = 0;
        v = g_gameState->gameData->ctx.battle->unitList;
        side = (u16)arg0->targets >= 0x10;
        for (; v != NULL; v = v->next) {
            if (v->unitBit & (u16)arg0->targets) {
                if (!(v->status & BTL_STATUS_PETRIFY) && (u8)btl_hasStatus(v, BTL_STATUS_REFLECT)) {
                    m++;
                    mask |= v->unitBit;
                }
            } else if (!(v->status & BTL_STATUS_KO) && (v->flags & 1) == side && ((v->flags >> 28) & 1)) {
                buf[n++] = v->unitBit;
            }
        }
        if (m != 0 && n != 0) {
            for (i = 0; i < 4; i++) {
                if (i < m) {
                    arg0->caster->bounceMasks[i] = buf[RAND8() % n];
                } else {
                    arg0->caster->bounceMasks[i] = 0;
                }
            }
            if ((u16)mask == (u16)arg0->targets) {
                arg0->unk10.b.reflect = 1;
                arg0->targets = btl_orTargets(arg0->caster->bounceMasks);
            } else {
                arg0->unk10.b.reflect = 2;
                arg0->targets &= ~mask;
            }
        }
    }
    return (u16)mask;
}

void btl_drawActiveMarker(GameData *arg0, Battle *arg1, BtlUnit *arg2) {
    SVECTOR v;
    SVECTOR tri[3];
    long sxy[4];
    s32 ang;
    s32 i;

    ang = (arg1->tick & 0xF) << 8;
    gte_SetRotMatrix(&arg0->viewMatrix);
    gte_SetTransMatrix(&arg0->viewMatrix);
    v.vx = arg2->pos[0] >> 16;
    v.vy = (arg2->pos[1] >> 16) - arg2->model->header->height;
    v.vz = arg2->pos[2] >> 16;
    gte_ldv0(&v);
    gte_rtps();
    gte_stsxy(&sxy[0]);
    for (i = 0; i < 3; i++) {
        tri[i].vx = v.vx - ((math_cos(ang) * 68) >> 12);
        tri[i].vy = v.vy - 0x85;
        tri[i].vz = v.vz - ((math_sin(ang) * 68) >> 12);
        ang = (ang + 0x4F1) & 0xFFF;
    }
    gte_ldv3c(tri);
    gte_rtpt();
    gte_stsxy3(&sxy[1], &sxy[2], &sxy[3]);
    if (NormalClip(sxy[0], sxy[1], sxy[2]) > 0) {
        (arg1->cursorPrims + arg0->drawBuf)->unk0[0].unk8.x0 = sxy[0];
        (arg1->cursorPrims + arg0->drawBuf)->unk0[0].unk8.y0 = sxy[0] >> 16;
        (arg1->cursorPrims + arg0->drawBuf)->unk0[0].unk8.x1 = sxy[1];
        (arg1->cursorPrims + arg0->drawBuf)->unk0[0].unk8.y1 = sxy[1] >> 16;
        (arg1->cursorPrims + arg0->drawBuf)->unk0[0].unk8.x2 = sxy[2];
        (arg1->cursorPrims + arg0->drawBuf)->unk0[0].unk8.y2 = sxy[2] >> 16;
        addPrim(&arg0->ot[arg0->drawBuf][2], &(arg1->cursorPrims + arg0->drawBuf)->unk0[0]);
    }
    if (NormalClip(sxy[0], sxy[2], sxy[3]) > 0) {
        (arg1->cursorPrims + arg0->drawBuf)->unk0[1].unk8.x0 = sxy[0];
        (arg1->cursorPrims + arg0->drawBuf)->unk0[1].unk8.y0 = sxy[0] >> 16;
        (arg1->cursorPrims + arg0->drawBuf)->unk0[1].unk8.x1 = sxy[2];
        (arg1->cursorPrims + arg0->drawBuf)->unk0[1].unk8.y1 = sxy[2] >> 16;
        (arg1->cursorPrims + arg0->drawBuf)->unk0[1].unk8.x2 = sxy[3];
        (arg1->cursorPrims + arg0->drawBuf)->unk0[1].unk8.y2 = sxy[3] >> 16;
        addPrim(&arg0->ot[arg0->drawBuf][2], &(arg1->cursorPrims + arg0->drawBuf)->unk0[1]);
    }
    if (NormalClip(sxy[0], sxy[3], sxy[1]) > 0) {
        (arg1->cursorPrims + arg0->drawBuf)->unk0[2].unk8.x0 = sxy[0];
        (arg1->cursorPrims + arg0->drawBuf)->unk0[2].unk8.y0 = sxy[0] >> 16;
        (arg1->cursorPrims + arg0->drawBuf)->unk0[2].unk8.x1 = sxy[3];
        (arg1->cursorPrims + arg0->drawBuf)->unk0[2].unk8.y1 = sxy[3] >> 16;
        (arg1->cursorPrims + arg0->drawBuf)->unk0[2].unk8.x2 = sxy[1];
        (arg1->cursorPrims + arg0->drawBuf)->unk0[2].unk8.y2 = sxy[1] >> 16;
        addPrim(&arg0->ot[arg0->drawBuf][2], &(arg1->cursorPrims + arg0->drawBuf)->unk0[2]);
    }
    if (NormalClip(sxy[1], sxy[3], sxy[2]) > 0) {
        (arg1->cursorPrims + arg0->drawBuf)->unk0[3].unk8.x0 = sxy[1];
        (arg1->cursorPrims + arg0->drawBuf)->unk0[3].unk8.y0 = sxy[1] >> 16;
        (arg1->cursorPrims + arg0->drawBuf)->unk0[3].unk8.x1 = sxy[3];
        (arg1->cursorPrims + arg0->drawBuf)->unk0[3].unk8.y1 = sxy[3] >> 16;
        (arg1->cursorPrims + arg0->drawBuf)->unk0[3].unk8.x2 = sxy[2];
        (arg1->cursorPrims + arg0->drawBuf)->unk0[3].unk8.y2 = sxy[2] >> 16;
        addPrim(&arg0->ot[arg0->drawBuf][2], &(arg1->cursorPrims + arg0->drawBuf)->unk0[3]);
    }
}

void btl_autoAttack(BtlUnit *arg0) {
    s32 x;
    if (arg0->status & BTL_STATUS_CONFUSE) {
        if (g_randPtr >= g_randTableEnd) {
            rand_refill();
        }
        x = *g_randPtr++;
        x &= 1;
    } else if (arg0->status & BTL_STATUS_BERSERK) {
        x = 0;
    } else {
        return;
    }
    btl_setCmd((s32)arg0->cmd, BTL_CMD_ATTACK, BTL_ABIL_ATTACK, btl_pickRandomUnit(x) & 0xFFFF, 0);
}

void btl_onCmdEffectDone(void) {
    Battle *b = g_gameState->gameData->ctx.battle;
    BtlCmd *c = b->curCmd;
    s8 k = c->unk10.b.reflect;
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

void btl_clearCmd(BtlCmd *arg0) {
    arg0->next = 0;
    arg0->ability = 0;
    arg0->targets = 0;
    arg0->cmdNo = 0;
    arg0->subNo = 0;
    arg0->unk10.b.targeting = 0;
    arg0->unk10.b.unk0_2 = 0;
    arg0->unk10.b.automatic = 0;
    arg0->unk10.b.covered = 0;
    arg0->unk10.b.dodged = 0;
    arg0->unk10.b.reflect = 0;
    arg0->unk10.b.unk0_9 = 0;
    arg0->unk10.b.shortSummon = 0;
    arg0->unk10.b.unk0_11 = 0;
}

void btl_queueAutoCmd(BtlUnit *arg0, s32 arg1, s32 arg2, s32 arg3) {
    if (!(arg0->status & (BTL_STATUS_PETRIFY | BTL_STATUS_VENOM | BTL_STATUS_STOP | BTL_STATUS_SLEEP | BTL_STATUS_FREEZE)) && g_gameState->gameData->ctx.battle->phase == BTL_PHASE_COMBAT) {
        btl_setCmd(arg0->autoCmd, arg1, arg2, arg3 & 0xFFFF, 0);
    }
}

void func_800B3DBC(BtlUnit *arg0) {
    if ((arg0->flags & 0x1F) == 5) {
        Battle *b = g_gameState->gameData->ctx.battle;
        BtlCmd *p;
        for (p = &b->cmdQueue; p != NULL; p = p->next) {
            BtlCmd *n = p->next;
            if (n != NULL && n->caster == arg0 && n->cmdNo == BTL_CMD_TRANCE_EIDOLON) {
                btl_unlinkCmd(&p->next, 1);
                break;
            }
        }
        b->eidolonTimer = 0;
        b->eidolonAbility = 0;
        b->cmdFlags &= ~0xC;
    }
}

u8 btl_isCmdPending(BtlCmd *arg0) {
    BtlCmd *p;
    for (p = &g_gameState->gameData->ctx.battle->cmdQueue; p != NULL; p = p->next) {
        if (arg0 == p) {
            return 1;
        }
    }
    p = g_gameState->gameData->ctx.battle->curCmd;
    if (p != NULL && p == arg0) {
        return 1;
    }
    return 0;
}

u8 btl_isCmdQueued(BtlUnit *arg0, u8 arg1) {
    BtlCmd *p;
    for (p = &g_gameState->gameData->ctx.battle->cmdQueue; p != NULL; p = p->next) {
        if (p->caster == arg0 && p->cmdNo == arg1) {
            return 1;
        }
    }
    p = g_gameState->gameData->ctx.battle->curCmd;
    if (p != NULL && p->cmdNo == arg1) {
        return 1;
    }
    return 0;
}

void btl_removeCmd(BtlCmd *arg0) {
    BtlUnit *u = arg0->caster;
    BtlCmd *p;
    for (p = &g_gameState->gameData->ctx.battle->cmdQueue; p != NULL; p = p->next) {
        if (p->next == arg0) {
            if (u != NULL && arg0 != u->delayedCmd && arg0 != u->tranceCmd) {
                u->flags &= ~0x80000;
            }
            btl_unlinkCmd(&p->next, 0);
            return;
        }
    }
}

s32 btl_cancelUnitCmds(BtlUnit *arg0) {
    BtlCmd *c;
    BtlCmd *p;
    s32 r;
    arg0->flags &= ~0x80000;
    p = &g_gameState->gameData->ctx.battle->cmdQueue;
    r = 0;
    while (p != NULL) {
        c = p->next;
        if (c != NULL && c->caster == arg0 && c->cmdNo < 0x36) {
            if (c->cmdNo < BTL_CMD_TABLE_COUNT) {
                r = 1;
            }
            btl_releaseItem(c);
            btl_unlinkCmd(&p->next, 1);
        } else {
            p = c;
        }
    }
    return r;
}

void btl_removeUnitCmds(BtlUnit *arg0) {
    BtlCmd *p = &g_gameState->gameData->ctx.battle->cmdQueue;
    while (p != NULL) {
        BtlCmd *n = p->next;
        if (n != NULL && n->caster == arg0) {
            btl_releaseItem(n);
            btl_unlinkCmd(&p->next, 1);
        } else {
            p = n;
        }
    }
}

void btl_removeQueuedCmd(BtlUnit *arg0, u8 arg1) {
    BtlCmd *p;
    for (p = &g_gameState->gameData->ctx.battle->cmdQueue; p != NULL; p = p->next) {
        BtlCmd *n = p->next;
        if (n != NULL && n->caster == arg0 && n->cmdNo == arg1) {
            btl_unlinkCmd(&p->next, 1);
            return;
        }
    }
}

void btl_projectUnitPos(BtlUnit *arg0) {
    DVECTOR sxy;
    SVECTOR v;
    BtlBone *b;

    if (!((arg0->flags >> 22) & 1)) {
        gte_SetRotMatrix(&g_gameState->gameData->viewMatrix);
        gte_SetTransMatrix(&g_gameState->gameData->viewMatrix);
        b = &arg0->model->bones[arg0->baseBone];
        v.vx = b->tx;
        v.vy = b->ty;
        v.vz = b->tz;
        gte_ldv0(&v);
        gte_rtps();
        gte_stsxy(&sxy);
        arg0->screenX = sxy.vx;
        arg0->screenY = sxy.vy;
    }
    arg0->onScreen = 1;
}

void btl_unlinkCmd(BtlCmd **arg0, u8 arg1) {
    BtlCmd *p = *arg0;
    *arg0 = p->next;
    p->unk10.b.unk0_2 = 0;
    p->unk10.b.automatic = 0;
    if (arg1 && p->cmdNo == BTL_CMD_ESCAPE) {
        g_gameState->gameData->ctx.battle->cmdFlags &= ~1;
    }
}

s32 btl_payMpCost(BtlCmd *arg0) {
    s32 mp = arg0->ability->mpCost;
    if (D_8007AF02 != 0 && (arg0->ability->unkB & 4)) {
        mp <<= 2;
    }
    if (arg0->cmdNo == BTL_CMD_RETURN_MAGIC) {
        return 1;
    }
    if (btl_consumeMp(arg0->caster, mp) & 0xFF) {
        return 1;
    }
    btl_showMessage(BTL_MSG_NOT_ENOUGH_MP);
    return 0;
}

void btl_linkCmd(LinkNode *arg0, LinkNode **arg1) {
    arg0->unk0 = *arg1;
    *arg1 = arg0;
    arg0->unk10 = (arg0->unk10 & ~0xC) | 4;
}

void btl_releaseItem(BtlCmd *arg0) {
    u8 v = arg0->cmdNo;
    if ((u32)(v - 0xE) < 2 || v == 0x33) {
        btl_unreserveItem(arg0->subNo);
    }
}

u16 btl_orTargets(u16 *arg0) {
    s32 i;
    u16 r;
    r = i = 0;
    for (; i < 4; i++) {
        r |= arg0[i];
    }
    return r;
}

void btl_clearBounceMasks(Unk800B44A0 *arg0) {
    s32 i;
    for (i = 3; i >= 0; i--) {
        arg0->unk1C[i] = 0;
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
    case 1:
    case 2:
    case 3:
    case 4:
    case 5:
    case 7:
        if (btl_checkFrozenShatter(&c)) {
            btl_initHitRates(&c);
            if (btl_hasSureHit(&c) || btl_rollHit(&c)) {
                btl_setPhysParams(&c, type);
                btl_applyKillerBonus(&c);
                btl_applyCasterStatusMod(&c);
                func_800BA308(&c);
                btl_applyTargetStatusMod(&c);
                if ((caster->status & BTL_STATUS_TRANCE) && ((caster->flags >> 1) & 0xF) == 3) {
                    c.multiplier *= 2;
                }
                btl_applyRowMod(&c);
                btl_applyWeaponElementBoost(&c);
                if (btl_applyWeaponElement(&c)) {
                    btl_rollCritical(&c);
                    func_800BA938((Unk800BA938 *)&c);
                    btl_finishPhysDamage(&c);
                    btl_markTrouble(&c);
                    btl_payMpAttack(&c);
                    if (c.caster->support[0] & 0x02000000) {
                        c.statusRate = caster->weapon[7];
                        RAND_REFILL2();
                        if (RAND16() % 100 < c.statusRate) {
                            c.flags |= 0x100;
                        }
                    }
                }
            }
        }
        break;
    case 6:
        {
            s32 v;
            s32 d;
            if (btl_checkNotSelf(&c) && btl_checkTargetAlive(&c) && btl_checkFrozenShatter(&c)) {
                btl_initHitRates(&c);
                if (btl_hasSureHit(&c) || btl_rollHit(&c)) {
                    btl_setPhysParams(&c, 1);
                    btl_applyKillerBonus(&c);
                    btl_applyCasterStatusMod(&c);
                    func_800BA308(&c);
                    btl_applyTargetStatusMod(&c);
                    btl_applyRowMod(&c);
                    btl_rollCritical(&c);
                    func_800BA938((Unk800BA938 *)&c);
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
    case 8:
        if (btl_checkFrozenShatter(&c)) {
            btl_initHitRates(&c);
            if (btl_rollHit(&c)) {
                btl_setAbilityPhysParams(&c);
                btl_applyCasterStatusMod(&c);
                func_800BA308(&c);
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
    case 9:
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
    case 10:
        btl_setMagicParams(&c);
        btl_applyHealBoost(&c);
        btl_applyCasterMiniMagic(&c);
        btl_halveForSpread(&c);
        btl_calcHeal(&c);
        break;
    case 0x6C:
        if (btl_guardIfDeathImmune(&c)) {
            btl_initStatusHitRate(&c);
            btl_halveHitForShell(&c);
            btl_halveHitForSpread(&c);
            if (btl_rollStatusHit(&c)) {
                btl_applyAbilityStatus(&c);
            }
        }
        break;
    case 0xC:
        btl_cureAbilityStatus(&c);
        break;
    case 0xD:
        {
            s32 v;
            if (func_800B9DDC(&c) && (btl_initUndeadHitRate(&c) || btl_rollStatusHit(&c))) {
                if (!(target->status & BTL_STATUS_ZOMBIE) && btl_hasEnemyFlag(target, BTL_ENEMY_UNDEAD) == 0) {
                    if (btl_checkTargetIsPlayer(&c)) {
                        v = target->max->maxHp * (target->spirit + cmd->ability->power);
                        if (caster->support[1] & 2) {
                            v = v / 50;
                        } else {
                            v = v / 100;
                        }
                        if (v >= 10000) {
                            v = 9999;
                        }
                        c.targetHpChange = v;
                        c.targetResult |= 3;
                        btl_cureAbilityStatus(&c);
                    }
                } else {
                    btl_killUnit(target);
                }
            }
            break;
        }
    case 0xE:
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
    case 0xF:
        {
            s32 v;
            s32 d;
            if (btl_checkNotSelf(&c) && btl_checkTargetAlive(&c)) {
                btl_setMagicParams(&c);
                btl_applyCasterMiniMagic(&c);
                btl_applyTargetShell(&c);
                v = 0;
                c.targetResult |= 8;
                d = c.power - c.defense;
                c.casterResult |= 8;
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
                    c.targetResult |= 0x10;
                    if (caster->mp < v) {
                        v = caster->mp;
                    }
                } else {
                    c.casterResult |= 0x10;
                    if (target->mp < v) {
                        v = target->mp;
                    }
                }
                c.casterMpChange = v;
                c.targetMpChange = v;
            }
            break;
        }
    case 0x10:
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
    case 0x11:
    case 0x5B:
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
                        if ((u8)type == 0x11) {
                            btl_calcPercentDamage(&c);
                        } else {
                            if (c.multiplier > 100) {
                                c.multiplier = 100;
                            }
                            v = target->hp * c.multiplier / 100;
                            if (v >= 10000) {
                                v = 9999;
                            }
                            c.targetResult |= 1;
                            if (c.flags & 8) {
                                c.targetResult |= 2;
                            }
                            c.targetHpChange = v;
                        }
                    }
                }
            }
            break;
        }
    case 0x12:
        if ((cmd->ability->effect != 0x38 || RAND8() < 0xAB) && (cmd->subNo != 0x2E || !cmd->unk10.b.unk0_9)) {
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
            c.flags |= 1;
        }
        break;
    case 0x13:
        btl_setWeaponAbilityParams(&c);
        btl_applyCasterStatusMod(&c);
        btl_applyTargetStatusMod(&c);
        btl_applyAbilityElementBoost(&c);
        if (btl_applyWeaponElement(&c)) {
            btl_finishMagicDamage(&c);
            btl_rollAbilityStatus(&c);
        }
        break;
    case 0x14:
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
    case 0x15:
        btl_setMagicParams(&c);
        if (target->level == caster->level) {
            c.multiplier += caster->level;
            c.defense = 0;
        }
        btl_applyCasterMiniMagic(&c);
        btl_applyTargetShell(&c);
        btl_finishMagicDamage(&c);
        break;
    case 0x16:
        if (btl_checkLevelMultiple(&c) && btl_checkTargetAlive(&c)) {
            btl_setTargetHp(&c);
        }
        break;
    case 0x17:
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
    case 0x18:
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
    case 0x19:
        if (btl_missIfDeathImmune(&c) && btl_checkTargetAlive(&c)) {
            btl_setTargetHp(&c);
        }
        break;
    case 0x1B:
        if (btl_missIfDeathImmune(&c) && btl_checkTargetAlive(&c)) {
            btl_initStatusHitRate(&c);
            btl_halveHitForShell(&c);
            if (btl_rollStatusHit(&c)) {
                btl_setTargetHp(&c);
            }
        }
        break;
    case 0x1C:
        if (caster->hp == 1) {
            c.targetResult |= 1;
            c.targetHpChange = cmd->ability->rate + cmd->ability->power * 100;
        } else {
            c.flags |= 1;
        }
        break;
    case 0x1A:
        c.targetResult |= 1;
        c.targetHpChange = cmd->ability->rate + cmd->ability->power * 100;
        break;
    case 0x1D:
        c.targetResult |= 1;
        c.targetHpChange = caster->max->maxHp - caster->hp;
        break;
    case 0x1E:
        c.targetResult |= 1;
        if (!(target->status & BTL_STATUS_ZOMBIE)) {
            c.targetResult |= 2;
        }
        c.targetHpChange = caster->max->maxHp / 3;
        break;
    case 0x1F:
        c.targetResult |= 8;
        if (target->mp != 0) {
            RAND_REFILL2();
            c.targetMpChange = RAND16() % target->mp;
            if (c.targetMpChange >= 10000) {
                c.targetMpChange = 9999;
            }
        }
        break;
    case 0x20:
        {
            s32 v;
            s32 d;
            btl_setWeaponAbilityParams(&c);
            btl_applyAbilityElementBoost(&c);
            if (btl_applyAbilityElement(&c)) {
                c.targetResult |= 1;
                d = c.power - c.defense;
                if (d > 0) {
                    if (c.multiplier <= 0) {
                        c.multiplier = 1;
                    }
                    v = d * c.multiplier;
                    c.casterResult |= 1;
                    c.casterHpChange = caster->max->maxHp >> 3;
                    if (v >= 10000) {
                        v = 9999;
                    }
                    if (c.flags & 8) {
                        c.targetResult |= 2;
                    }
                    c.targetHpChange = v;
                }
            }
            break;
        }
    case 0x21:
    case 0x22:
    case 0x23:
    case 0x24:
        btl_initStatusHitRate(&c);
        btl_halveHitForShell(&c);
        if (btl_rollStatusHit(&c)) {
            if ((u8)type == 0x21) {
                target->defense >>= 1;
            } else if ((u8)type == 0x22) {
                target->strength = target->strength * 3 / 4;
            } else if ((u8)type == 0x23) {
                target->magicDef >>= 1;
            } else if ((u8)type == 0x24) {
                target->magic = target->magic * 3 / 4;
            }
        }
        break;
    case 0x25:
        c.targetResult |= 0x1B;
        if (caster->support[0] & 0x20000000) {
            c.targetHpChange = target->max->maxHp * cmd->ability->power / 50;
            c.targetMpChange = target->max->maxMp * cmd->ability->power / 50;
        } else {
            c.targetHpChange = target->max->maxHp * cmd->ability->power / 100;
            c.targetMpChange = target->max->maxMp * cmd->ability->power / 100;
        }
        break;
    case 0x26:
        {
            u32 v;
            u32 d;
            v = cmd->ability->power * caster->level;
            d = g_gameState->gameData->gil + v;
            c.targetResult |= 1;
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
    case 0x27:
        {
            s32 v;
            s32 d;
            btl_setWeaponAbilityParams(&c);
            btl_applyCasterStatusMod(&c);
            btl_applyTargetStatusMod(&c);
            c.targetResult |= 9;
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
    case 0x28:
        {
            s32 v;
            BtlUnit *u;
            v = target->max->maxHp - target->hp;
            c.targetResult |= 1;
            if (v >= 10000) {
                v = 9999;
            }
            c.targetHpChange = v;
            for (u = g_gameState->gameData->ctx.battle->unitList; u != NULL; u = u->next) {
                if ((u->flags & 0x10000001) == 0x10000000) {
                    c.target = u;
                    btl_applyCalcResult(&c);
                    btl_spawnPopups(u);
                }
            }
            return;
        }
    case 0x29:
        {
            BtlUnit *u;
            if (!(target->status & BTL_STATUS_KO)) {
                c.casterResult = 0x18;
                RAND_REFILL2();
                c.casterMpChange = RAND16() % (target->level * 2);
                for (u = g_gameState->gameData->ctx.battle->unitList; u != NULL; u = u->next) {
                    if ((u->flags & 0x10000001) == 0x10000001) {
                        c.caster = u;
                        btl_applyCalcResult(&c);
                        btl_spawnPopups(u);
                    }
                }
                return;
            }
            c.flags |= 1;
            break;
        }
    case 0x2A:
        {
            s32 i;
            if (btl_checkFrozenShatter(&c)) {
                RAND_REFILL2();
                i = caster->strength;
                c.multiplier = i + RAND16() % (((caster->level + i) >> 3) + 1);
                c.power = g_weaponTable[cmd->subNo].unk5 * 2;
                c.defense = target->defense;
                if (caster->support[0] & 0x10000000) {
                    c.multiplier = c.multiplier * 3 >> 1;
                }
                btl_applyCasterStatusMod(&c);
                btl_applyTargetStatusMod(&c);
                if (btl_hasEnemyFlag(target, BTL_ENEMY_FLYING) != 0) {
                    c.multiplier = c.multiplier * 3 >> 1;
                }
                btl_finishPhysDamage(&c);
            }
            break;
        }
    case 0x2B:
        target->strength += target->strength / cmd->ability->power;
        if (target->strength >= 100) {
            target->strength = 99;
        }
        break;
    case 0x2C:
        target->magic += target->magic / cmd->ability->power;
        if (target->magic >= 100) {
            target->magic = 99;
        }
        break;
    case 0x2D:
        if (!(target->status & BTL_STATUS_KO) && !(caster->status & (BTL_STATUS_PETRIFY | BTL_STATUS_KO))) {
            target->hp = target->max->maxHp;
            target->mp = target->max->maxMp;
            caster->mp = 0;
            btl_killUnit(caster);
        } else {
            c.flags |= 1;
        }
        break;
    case 0x2E:
        {
            s32 v;
            s32 d;
            if ((u32)(g_gameState->gameData->characters[(caster->flags >> 1) & 0xF].equip[0] - 7) < 9) {
                v = g_gameState->gameData->ctx.battle->statusSets[caster->weapon[1]];
                if (!(v & 0x100) || btl_guardIfDeathImmune(&c)) {
                    d = btl_addStatuses(target, v);
                    if (d == 0) {
                        c.flags |= 0x40;
                    } else if (d == 1) {
                        c.flags |= 1;
                    }
                }
            } else {
                c.flags |= 1;
            }
            break;
        }
    case 0x30:
    case 0x53:
        {
            s32 v;
            s32 d;
            btl_setPhysParams(&c, (u8)type == 0x30 ? 1 : 5);
            if (caster->support[0] & 0x200000) {
                c.multiplier *= 2;
            } else {
                c.multiplier = c.multiplier * 3 >> 1;
            }
            btl_applyCasterMiniMagic(&c);
            btl_applyTargetStatusMod(&c);
            if ((u8)type == 0x30) {
                btl_finishPhysDamage(&c);
            } else {
                d = c.power - c.defense;
                if (d <= 0) {
                    d = 1;
                }
                c.targetResult |= 1;
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
    case 0x31:
        {
            s32 i;
            btl_clearCmd(&sub);
            sub.cmdNo = cmd->cmdNo;
            sub.unk10.b.shortSummon = cmd->unk10.b.shortSummon;
            if (target->flags & 1) {
                sub.subNo = 0x49;
                i = 13;
            } else {
                sub.subNo = 0x48;
                i = 9;
            }
            sub.ability = (BtlCmd8 *)&g_gameState->gameData->ctx.battle->abilities[sub.subNo];
            btl_calcCmd(caster, target, &sub, i);
            return;
        }
    case 0x32:
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
    case 0x33:
        target->elemWeak |= cmd->ability->element;
        target->elemGuard &= ~cmd->ability->element;
        target->elemAbsorb &= ~cmd->ability->element;
        target->elemHalf &= ~cmd->ability->element;
        btl_showMessage(BTL_MSG_WEAK_AGAINST, btl_getElementName(cmd->ability->element));
        break;
    case 0x34:
        if (item_getCount(cmd->ability->power) != 0) {
            btl_clearCmd(&sub2);
            sub2.cmdNo = 0xE;
            sub2.subNo = cmd->ability->power;
            sub2.ability = (BtlCmd8 *)&g_gameState->gameData->ctx.battle->abilities[0];
            btl_calcCmd(caster, target, &sub2, D_8006B1F4[sub2.subNo - 0xE0].unk4);
            btl_useItem(sub2.subNo);
            return;
        }
        c.flags |= 1;
        break;
    case 0x35:
        {
            s32 v;
            c.targetResult |= 1;
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
    case 0x36:
        {
            s32 v;
            if ((target->flags & 1) || !((g_gameState->gameData->ctx.battle->scene.unk0 >> 2) & 1)) {
                v = (caster->flags & 1) << 11;
                target->rot[1] = v;
                target->obj->rotY = v;
                if ((target->flags & 1) && (&g_gameState->gameData->characters[(target->flags >> 1) & 0xF])->info.b.battleRow != ((target->flags >> 8) & 3)) {
                    btl_toggleRow(target);
                }
            } else {
                c.flags |= 1;
            }
            break;
        }
    case 0x37:
        btl_toggleRow(target);
        break;
    case 0x38:
        if (btl_canFlee(&c)) {
            btl_rollFlee(&c);
        }
        break;
    case 0x39:
        btl_tryEscape(0);
        if (btl_canFlee(&c)) {
            btl_setCmd(&g_gameState->gameData->ctx.battle->escapeCmd, BTL_CMD_ESCAPE, 0, 0xF, 1);
        }
        break;
    case 0x3A:
        {
            s32 v;
            s16 d;
            s16 k;
            BtlEnemy *e;
            e = &g_gameState->gameData->ctx.battle->enemies[(target->flags >> 1) & 0xF];
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
            if (!(caster->support[1] & 0x40000000)) {
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
            if (caster->support[0] & 0x400000) {
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
    case 0x3B:
        if (btl_missIfDeathImmune(&c)) {
            btl_startScan((s32)target);
        }
        break;
    case 0x3C:
        btl_startDetect(target);
        break;
    case 0x3D:
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
                        btl_queueAutoCmd(u, BTL_CMD_CHARGE_ATTACK, BTL_ABIL_ATTACK, v);
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
    case 0x3E:
        if (btl_hasEnemyFlag(target, 0x20) != 0) {
            if (btl_checkTargetAlive(&c)) {
                btl_killUnit(target);
                btl_showMessage(BTL_MSG_TOO_SOFT);
            }
        } else {
            btl_cureItemStatus(&c);
        }
        break;
    case 0x3F:
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
    case 0x41:
        {
            u8 i;
            if (btl_missIfDeathImmune(&c) && btl_checkTargetAlive(&c) && btl_hasEnemyFlag(target, 1) == 0) {
                if (target->max->maxHp / cmd->ability->power < target->hp) {
                    btl_showMessage(BTL_MSG_CANT_EAT_YET);
                } else {
                    i = g_gameState->gameData->ctx.battle->enemies[(target->flags >> 1) & 0xF].type->eatAbility;
                    btl_killUnit(target);
                    if (i != 0 && !chr_isAbilityLearned((caster->flags >> 1) & 0xF, i)) {
                        chr_learnAbility((caster->flags >> 1) & 0xF, i);
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
    case 0x42:
        {
            s32 v;
            c.targetResult |= 1;
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
    case 0x43:
        {
            s32 v;
            c.targetResult |= 1;
            v = g_gameState->gameData->stealCount * caster->speed / 2;
            if (v >= 10000) {
                c.targetHpChange = 9999;
            } else {
                c.targetHpChange = v;
            }
            break;
        }
    case 0x44:
        {
            c.targetResult |= 1;
            if (g_gameState->gameData->dragonKills * g_gameState->gameData->dragonKills < 10000) {
                c.targetHpChange = (u16)g_gameState->gameData->dragonKills * (u16)g_gameState->gameData->dragonKills;
            } else {
                c.targetHpChange = 9999;
            }
            break;
        }
    case 0x45:
    case 0x46:
        {
            s32 v;
            c.multiplier = 15;
            c.power = D_8006B1F4[cmd->subNo - 0xE0].unk5;
            c.defense = 0;
            if (caster->support[0] & 0x08000000) {
                c.multiplier = 30;
            }
            if ((u8)type == 0x45) {
                btl_calcHeal(&c);
            } else {
                v = c.power * c.multiplier;
                if (v >= 10000) {
                    v = 9999;
                }
                c.targetResult |= 8;
                if (!(target->status & BTL_STATUS_ZOMBIE) && btl_hasEnemyFlag(target, BTL_ENEMY_UNDEAD) == 0) {
                    c.targetResult |= 0x10;
                }
                c.targetMpChange = v;
            }
            break;
        }
    case 0x47:
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
    case 0x48:
        if (func_800B9DDC(&c)) {
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
    case 0x49:
        btl_cureItemStatus(&c);
        break;
    case 0x4A:
        c.targetResult |= 3;
        c.targetHpChange = D_8006B1F4[cmd->subNo - 0xE0].unk5 * (item_getCount(cmd->subNo) + 1);
        break;
    case 0x4B:
        c.targetResult |= 1;
        c.targetHpChange = D_8006B1F4[cmd->subNo - 0xE0].unk5 * (item_getCount(cmd->subNo) + 1);
        break;
    case 0x4C:
        c.targetResult |= 0x1B;
        c.targetHpChange = target->max->maxHp >> 1;
        c.targetMpChange = target->max->maxMp >> 1;
        RAND_REFILL2();
        if (D_8006B1F4[cmd->subNo - 0xE0].unk7 < RAND16() % 100) {
            btl_showMessage(BTL_MSG_SNAKE_BITE);
            btl_addStatuses(target, D_8006B1F4[cmd->subNo - 0xE0].unk8);
        }
        break;
    case 0x4D:
        if (btl_checkTargetAlive(&c)) {
            c.targetHpChange = 9999;
            c.targetResult |= 1;
        }
        break;
    case 0x54:
        btl_initStatusHitRate(&c);
        btl_halveHitForShell(&c);
        if (btl_rollStatusHit(&c)) {
            btl_addItem(0xFE);
        }
        break;
    case 0x55:
        btl_setMagicParams(&c);
        btl_applyCasterMiniMagic(&c);
        btl_applyTargetShell(&c);
        btl_applyAbilityElementBoost(&c);
        if (btl_applyAbilityElement(&c) && btl_missIfAirborne(&c)) {
            /* padded: a zero word follows the first jump table */
            __asm__(".section .rodata\n\t.word 0\n\t.section .text");
            switch (cmd->subNo) {
            case 0x31:
                c.power += item_getCount(0xE9);
                break;
            case 0x33:
                c.power += item_getCount(0xEA);
                break;
            case 0x35:
                c.power += item_getCount(0xE7);
                break;
            case 0x3C:
                c.power += item_getCount(0xE2);
                break;
            case 0x3E:
                c.power += item_getCount(0xE0);
                break;
            case 0x40:
                c.power += item_getCount(0xEB);
                break;
            case 0x42:
            case 0x43:
                c.power += item_getCount(0xE8);
                break;
            case 0x4A:
                c.power += caster->level;
                break;
            }
            btl_finishMagicDamage(&c);
            btl_rollAbilityStatus(&c);
        }
        break;
    case 0x56:
        if (btl_missIfDeathImmune(&c)) {
            btl_setMultiplierFromPower(&c);
            btl_applyAbilityElementBoost(&c);
            if (btl_applyAbilityElement(&c) && btl_missIfAirborne(&c)) {
                c.multiplier += item_getCount(0xE1);
                btl_calcPercentDamage(&c);
            }
        }
        break;
    case 0x57:
        {
            s32 i;
            if (btl_guardIfDeathImmune(&c)) {
                btl_initStatusHitRate(&c);
                c.hitRate += item_getCount(0xFE) >> 1;
                if (btl_rollStatusHit(&c)) {
                    btl_applyAbilityStatus(&c);
                }
            }
            if ((c.flags & 0x41) && (caster->support[1] & 0x10000000)) {
                c.flags &= ~0x41;
                RAND_REFILL2();
                i = caster->magic;
                c.multiplier = i + RAND16() % (((caster->level + i) >> 3) + 1);
                c.power = cmd->ability->power + 100 - item_getCount(0xFE);
                c.defense = target->magicDef;
                btl_finishMagicDamage(&c);
            }
            break;
        }
    case 0x58:
        c.targetResult |= 1;
        if (caster->hp != 0) {
            caster->hpChange = caster->hp;
            btl_killUnit(caster);
        }
        c.targetHpChange = caster->hpChange;
        if (c.targetHpChange >= 10000) {
            c.targetHpChange = 9999;
        }
        break;
    case 0x59:
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
    case 0x5A:
        target->defense >>= 1;
        target->magicDef >>= 1;
        break;
    case 0x5C:
        btl_addItem(c.cmd->ability->power);
        break;
    case 0x5D:
        if (btl_missIfDeathImmune(&c) && btl_checkTargetAlive(&c)) {
            c.flags |= 0x80;
            target->hp = RAND8() % 9 + 1;
            btl_addStatuses(target, g_gameState->gameData->ctx.battle->statusSets[cmd->ability->statusSet]);
        }
        break;
    case 0x5E:
        {
            target->magic >>= 1;
            if (caster->magic * 2 >= 0x100) {
                caster->magic = 0xFF;
            } else {
                caster->magic = caster->magic * 2;
            }
            break;
        }
    case 0x5F:
        {
            target->strength >>= 1;
            if (caster->strength * 2 >= 0x100) {
                caster->strength = 0xFF;
            } else {
                caster->strength = caster->strength * 2;
            }
            break;
        }
    case 0x60:
        target->trance = 0xFF;
        btl_addStatus(target, BTL_STATUS_TRANCE);
        break;
    case 0x61:
        if ((&g_gameState->gameData->characters[(target->flags >> 1) & 0xF])->classFlags & 2) {
            c.flags |= 1;
        } else {
            btl_applyAbilityStatus(&c);
        }
        break;
    case 0x62:
        if ((&g_gameState->gameData->characters[(target->flags >> 1) & 0xF])->classFlags & 8) {
            c.flags |= 1;
        } else {
            btl_setAbilityPhysParams(&c);
            btl_finishMagicDamage(&c);
        }
        break;
    case 0x63:
        btl_initStatusHitRate(&c);
        btl_halveHitForShell(&c);
        btl_halveHitForSpread(&c);
        if (btl_rollStatusHit(&c)) {
            c.targetResult |= 1;
            c.targetHpChange = target->level * cmd->ability->power;
        }
        break;
    case 0x64:
        if (!btl_checkFrozenShatter(&c)) {
            break;
        }
    case 0x6B:
        btl_setEnemyAttackParams(&c);
        btl_applyCasterStatusMod(&c);
        func_800BA308(&c);
        btl_applyTargetStatusMod(&c);
        btl_applyRowMod(&c);
        btl_applyAbilityElementBoost(&c);
        if (btl_applyAbilityElement(&c)) {
            btl_finishMagicDamage(&c);
            if ((u8)type == 0x6B && ((target->flags >> 8) & 3)) {
                btl_toggleRow(target);
            }
            btl_rollAbilityStatus(&c);
        }
        break;
    case 0x65:
        btl_initHitRates(&c);
        if (btl_rollHit(&c)) {
            btl_calcEnemySteal(&c);
        } else {
            btl_showMessage(BTL_MSG_STEAL_FAILED);
        }
        break;
    case 0x66:
        btl_setEnemyAttackParams(&c);
        btl_applyCasterStatusMod(&c);
        func_800BA308(&c);
        btl_applyTargetStatusMod(&c);
        btl_applyRowMod(&c);
        btl_finishMagicDamage(&c);
        btl_calcEnemySteal(&c);
        break;
    case 0x68:
        c.targetResult |= 1;
        if (g_evtGlobalVars.b[0xC0] >= 14) {
            c.targetHpChange = 9999;
        } else {
            c.targetHpChange = 1 << (g_evtGlobalVars.b[0xC0] - 1);
        }
        break;
    case 0x69:
        {
            s32 i;
            s32 bit;
            if (btl_checkTargetAlive(&c)) {
                for (bit = 1, i = 0; i < 32; i++, bit <<= 1) {
                    if ((RAND8() >> 5) == 0) {
                        if (bit & 0x1B031D79) {
                            btl_addStatus(target, bit);
                        } else if ((bit & 0x200) && !(target->status & BTL_STATUS_KO)) {
                            c.flags |= 0x80;
                            target->hp = RAND8() % 9 + 1;
                        }
                    }
                }
            }
            break;
        }
    case 0x6A:
        g_btlRewards.unkA &= ~(1 << ((u8)target->flags >> 5));
        func_800B3DBC(target);
        btl_removeUnitCmds(target);
        btl_saveCharacter(target, 1);
        btl_unlinkUnit(target);
        break;
    case 0x6D:
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
    case 0xB:
        btl_initStatusHitRate(&c);
        btl_halveHitForShell(&c);
        btl_halveHitForSpread(&c);
        if (btl_rollStatusHit(&c)) {
            btl_applyAbilityStatus(&c);
        }
        break;
    case 0x67:
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

    if ((t->flags & 1) && !(s->flags & 1)) {
        back = btl_tryCounter(t, s, (BtlCmd *)c->cmd);
    }
    if (c->flags & 0x40) {
        t->popupFlags |= 0x80;
    } else if (c->flags & 1) {
        t->popupFlags |= 0x20;
        if (c->flags & 2) {
            if (t->flags & 1) {
                t->model->motion = (BtlUnit9CC *)t->motions[16];
                t->obj->frame = 0;
                t->rot[1] = t->obj->rotY = (t->flags & 1) * 0x800;
            } else if (!((t->flags >> 23) & 1)) {
                t->pos[2] += 0x1900000;
            } else {
                w = btl_findBodyUnit();
                w->pos[2] += 0x1900000;
            }
            t->flags |= 0x4000000;
            c->cmd->flags |= 0x40;
        }
    } else {
        if (c->cmd->ability->flags & 8) {
            btl_removeStatus(t, BTL_STATUS_CONFUSE);
            btl_removeStatus(t, BTL_STATUS_SLEEP);
        }
        if (c->cmd->ability->flags & 0x10) {
            btl_removeStatus(t, BTL_STATUS_VANISH);
        }
        if (c->targetResult != 0) {
            t->popupFlags |= c->targetResult;
            if (c->targetResult & 1) {
                n = 0;
                if (((c->cmd->flags >> 7) & 3) == 1) {
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
                if (c->targetResult & 2) {
                    btl_healHp(t, c->targetHpChange);
                } else {
                    btl_damageHp(t, c->targetHpChange, btl_canAddHitStatus(c));
                    btl_gainTrance(c, back);
                }
            }
            if (c->targetResult & 8) {
                if (c->targetResult & 0x10) {
                    btl_healMp(t, c->targetMpChange);
                } else {
                    btl_damageMp(t, c->targetMpChange);
                }
            }
        } else if (c->flags & 0x80) {
            if ((u8)btl_canAddHitStatus(c)) {
                btl_startHitMotion(t);
            }
            btl_gainTrance(c, back);
        }
        if (c->casterResult != 0) {
            s->popupFlags |= c->casterResult;
            if (c->casterResult & 1) {
                if (c->casterResult & 2) {
                    btl_healHp(s, c->casterHpChange);
                } else {
                    btl_damageHp(s, c->casterHpChange, 0);
                }
            }
            if (c->casterResult & 8) {
                if (c->casterResult & 0x10) {
                    btl_healMp(s, c->casterMpChange);
                } else {
                    btl_damageMp(s, c->casterMpChange);
                }
            }
        }
        if ((c->flags & 0x100) && t->hp != 0) {
            a = g_gameState->gameData->ctx.battle->statusSets[s->weapon[1]];
            if (!(a & 0x100) || !(t->status & BTL_STATUS_DEATH_IMMUNE)) {
                btl_addStatuses(t, a);
            }
        }
    }
    if (!(t->flags & 1)) {
        who = 0x10;
        if (!((t->flags >> 23) & 1)) {
            who = t->unitBit;
        }
        id = c->cmd->subNo | (c->cmd->cmdNo << 8);
        if ((s->flags & 1) && !(t->status & (BTL_STATUS_PETRIFY | BTL_STATUS_VENOM | BTL_STATUS_STOP | BTL_STATUS_FREEZE))) {
            if ((((g_gameState->gameData->ctx.battle->enemies + ((t->flags >> 1) & 0xF))->flags >> 6) & 1) && t->hp == 0) {
                evt_battleTrigger(0x36, who, s->unitBit, id);
            } else if (t->hp != 0 && c->cmd->cmdNo < 0x30) {
                evt_battleTrigger(0x35, who, s->unitBit, id);
            }
        }
        evt_battleTrigger(0x37, who, s->unitBit, id);
    }
}

void btl_calcSteal(BtlCalc *arg0, Unk800B7F20 *arg1, s32 arg2) {
    u8 id = arg1->unk8[arg2];
    GameData *gd;
    GameData *g;
    u32 r;

    if (id != 0xFF) {
        arg1->unk8[arg2] = 0xFF;
        g = g_gameState->gameData;
        if (g->stealCount < 9999) {
            g->stealCount++;
        }
        btl_addItem(id);
        btl_showMessage(BTL_MSG_STOLE, btl_getItemName(id));
        if (arg0->caster->support[1] & 0x20000000) {
            arg0->targetResult |= 1;
            if (g_randPtr >= g_randTableEnd) {
                rand_refill();
                if (g_randPtr >= g_randTableEnd) {
                    rand_refill();
                }
            }
            arg0->targetHpChange = ((*g_randPtr << 8) | *g_randPtr++) % ((arg0->caster->level * arg0->target->level) >> 1);
        }
        if (arg0->caster->support[0] & 0x800000) {
            gd = g_gameState->gameData;
            if (g_randPtr >= g_randTableEnd) {
                rand_refill();
                if (g_randPtr >= g_randTableEnd) {
                    rand_refill();
                }
            }
            gd->gil = g_gameState->gameData->gil +
                         ((*g_randPtr << 8) | *g_randPtr++) % ((arg0->caster->level * arg0->target->level) / 4 + 1);
            if (g_gameState->gameData->gil > 9999999) {
                g_gameState->gameData->gil = 9999999;
            }
        }
    } else {
        btl_showMessage(BTL_MSG_STEAL_FAILED);
    }
}

void btl_gainTrance(BtlCalc *arg0, u8 arg1) {
    BtlUnit *u;
    u8 v;
    if ((arg0->target->flags & 1) && !(arg0->caster->flags & 1)) {
        if (arg1 == 0) {
            btl_tryAutoPotion(arg0->target, arg0->cmd->cmdNo);
        }
        btl_applyHitSupports(arg0->target, (u8 *)arg0->cmd->ability);
        u = arg0->target;
        if (((u->flags >> 30) & 1) && u->hp != 0 && !(u->status & (BTL_STATUS_PETRIFY | BTL_STATUS_VENOM | BTL_STATUS_ZOMBIE | BTL_STATUS_KO | BTL_STATUS_STOP | BTL_STATUS_TRANCE | BTL_STATUS_FREEZE))) {
            if (u->support[1] & 8) {
                v = u->spirit;
            } else {
                if (g_randPtr >= g_randTableEnd) {
                    rand_refill();
                    if (g_randPtr >= g_randTableEnd) {
                        rand_refill();
                    }
                }
                v = ((*g_randPtr << 8) | *g_randPtr++) % arg0->target->spirit;
            }
            u = arg0->target;
            if (v + u->trance < 0xFF) {
                u->trance += v;
            } else {
                u->trance = 0xFF;
                btl_addStatus(arg0->target, BTL_STATUS_TRANCE);
            }
        }
    }
}

u8 btl_rollHit(BtlCalc *arg0) {
    if (arg0->caster->status & (BTL_STATUS_BLIND | BTL_STATUS_CONFUSE)) {
        arg0->hitRate /= 2;
    }
    if (arg0->caster->status & (BTL_STATUS_TRANCE | BTL_STATUS_VANISH)) {
        arg0->evade = 0;
    }
    if (arg0->target->support[0] & 0x400) {
        arg0->hitRate /= 2;
    }
    if (arg0->target->status & 0x02021417) {
        arg0->evade = 0;
    }
    if (arg0->target->status & BTL_STATUS_DEFEND) {
        arg0->hitRate /= 2;
    }
    if (arg0->target->status & BTL_STATUS_VANISH) {
        arg0->hitRate = 0;
    }
    if (g_randPtr >= g_randTableEnd) {
        rand_refill();
        if (g_randPtr >= g_randTableEnd) {
            rand_refill();
        }
    }
    if (((*g_randPtr << 8) | *g_randPtr++) % 100 >= arg0->hitRate) {
        arg0->flags |= 1;
        return 0;
    }
    if (g_randPtr >= g_randTableEnd) {
        rand_refill();
        if (g_randPtr >= g_randTableEnd) {
            rand_refill();
        }
    }
    if (((*g_randPtr << 8) | *g_randPtr++) % 100 < arg0->evade) {
        if (arg0->target != arg0->caster) {
            arg0->flags |= 1;
            if (!((arg0->target->flags >> 25) & 1)) {
                arg0->flags |= 3;
            }
            return 0;
        }
    }
    return 1;
}

u8 btl_rollStatusHit(BtlCalc *arg0) {
    if (g_randPtr >= g_randTableEnd) {
        rand_refill();
        if (g_randPtr >= g_randTableEnd) {
            rand_refill();
        }
    }
    if (((*g_randPtr << 8) | *g_randPtr++) % 100 < arg0->hitRate) {
        if (g_randPtr >= g_randTableEnd) {
            rand_refill();
            if (g_randPtr >= g_randTableEnd) {
                rand_refill();
            }
        }
        if (((*g_randPtr << 8) | *g_randPtr++) % 100 >= arg0->evade) {
            return 1;
        }
    }
    arg0->flags |= 1;
    return 0;
}

void btl_setPhysParams(BtlCalc *arg0, u8 arg1) {
    s32 r;

    if (g_randPtr >= g_randTableEnd) {
        rand_refill();
        if (g_randPtr >= g_randTableEnd) {
            rand_refill();
        }
    }
    r = ((*g_randPtr << 8) | *g_randPtr++) % (((arg0->caster->level + arg0->caster->strength) >> 3) + 1);
    arg0->power = arg0->caster->weapon[5];
    arg0->defense = arg0->target->defense;
    switch (arg1) {
    case 7:
        arg0->power += arg0->caster->level;
    case 1:
        arg0->multiplier = arg0->caster->strength + r;
        break;
    case 2:
        arg0->multiplier = ((arg0->caster->strength + arg0->caster->spirit) >> 1) + r;
        break;
    case 3:
        arg0->multiplier = ((arg0->caster->strength + arg0->caster->speed) >> 1) + r;
        break;
    case 4:
        arg0->multiplier = ((arg0->caster->strength + arg0->caster->magic) >> 1) + r;
        break;
    case 5:
        if (g_randPtr >= g_randTableEnd) {
            rand_refill();
            if (g_randPtr >= g_randTableEnd) {
                rand_refill();
            }
        }
        arg0->multiplier = ((*g_randPtr << 8) | *g_randPtr++) % arg0->caster->strength + r;
        break;
    }
}

void btl_applyKillerBonus(BtlCalc *arg0) {
    u32 f = arg0->target->flags;
    u32 c;
    BtlEnemyType *e;
    BtlUnit *u;
    if (!(f & 1)) {
        c = arg0->caster->support[0];
        e = g_gameState->gameData->ctx.battle->enemies[(f >> 1) & 0xF].type;
        if (((c & 0x2000) && (e->category & 0x80)) || ((c & 0x4000) && (e->category & 0x40)) || ((c & 0x8000) && (e->category & 0x20)) || ((c & 0x10000) && (e->category & 0x10)) || ((c & 0x20000) && (e->category & 8)) || ((c & 0x40000) && (e->category & 4)) || ((c & 0x80000) && (e->category & 2)) || ((c & 0x100000) && (e->category & 1))) {
            arg0->multiplier = arg0->multiplier * 3 >> 1;
        }
    }
    u = arg0->caster;
    if ((u->support[0] & 0x1000) && u->mp > 0) {
        arg0->multiplier = arg0->multiplier * 3 >> 1;
        arg0->flags |= 4;
    }
}

void btl_rollCritical(BtlCalc *arg0) {
    u8 s = arg0->caster->spirit >> 2;
    s32 r;
    if (s != 0) {
        if (g_randPtr >= g_randTableEnd) {
            rand_refill();
            if (g_randPtr >= g_randTableEnd) {
                rand_refill();
                if (g_randPtr >= g_randTableEnd) {
                    rand_refill();
                    if (g_randPtr >= g_randTableEnd) {
                        rand_refill();
                    }
                }
            }
        }
        r = (*g_randPtr << 8) | *g_randPtr++;
        if (r % 100 < r % s) {
            arg0->multiplier *= 2;
            arg0->targetResult |= 4;
        }
    }
}

u8 btl_applyAbilityElement(BtlCalc *arg0) {
    BtlUnit *t;
    u8 e = arg0->cmd->ability->element;
    if (arg0->caster->support[1] & 1) {
        return 1;
    }
    t = arg0->target;
    if (t->elemGuard & e) {
        arg0->flags |= 0x40;
        return 0;
    }
    if (t->elemHalf & e) {
        arg0->multiplier /= 2;
    }
    if (arg0->target->elemAbsorb & e) {
        arg0->defense = 0;
        arg0->flags |= 8;
    }
    if (arg0->target->elemWeak & e) {
        arg0->multiplier = arg0->multiplier * 3 >> 1;
    }
    if (e & (BTL_ELEM_ICE | BTL_ELEM_WATER)) {
        btl_removeStatus(arg0->target, BTL_STATUS_HEAT);
    }
    if (e & BTL_ELEM_FIRE) {
        btl_removeStatus(arg0->target, BTL_STATUS_FREEZE);
    }
    return 1;
}

void btl_finishPhysDamage(BtlCalc *arg0) {
    s32 d = arg0->power - arg0->defense;
    s32 v;
    BtlUnit *u;
    BtlUnit *t;
    if (d <= 0) {
        d = 1;
    }
    arg0->targetResult |= 1;
    if (arg0->multiplier <= 0) {
        arg0->multiplier = 1;
    }
    v = d * arg0->multiplier;
    if (v >= 10000) {
        v = 9999;
    }
    if (!(arg0->flags & 8)) {
        u = arg0->caster;
        if ((u->flags & 1) && ((*u->support & 0x01000000) || g_gameState->gameData->characters[(u->flags >> 1) & 0xF].equip[0] == 0x3C)) {
            if (!(arg0->target->status & BTL_STATUS_ZOMBIE) && btl_hasEnemyFlag(arg0->target, 0x10) == 0) {
                t = arg0->target;
                t->rot[1] = t->obj->rotY = (t->flags & 1) * 0x800;
                arg0->targetResult |= 2;
            }
        }
    } else {
        arg0->targetResult |= 2;
    }
    arg0->targetHpChange = v;
}

void btl_setTargetHp(BtlCalc *arg0) {
    BtlUnit *t;
    BtlUnit *u;
    BtlUnit9C *m;
    if (arg0->cmd->ability->power != 0) {
        if (arg0->target->status & BTL_STATUS_KO) {
            arg0->flags |= 1;
            return;
        }
        arg0->flags |= 0x80;
        arg0->target->hp = arg0->cmd->ability->power;
        t = arg0->target;
        t->rot[1] = t->obj->rotY = (t->flags & 1) * 0x800;
        return;
    }
    u = arg0->target;
    u->hp = 0;
    u->flags |= 0x100000;
    if (!(u->flags & 1)) {
        btl_playDeathCry(u, g_gameState->gameData->ctx.battle->enemies[(u->flags >> 1) & 0xF].type->deathCry);
    }
    m = u->model;
    if ((s32)m->motion != u->motions[(u->flags >> 18) & 1] && (!(u->flags & 1) || (s32)m->motion != u->motions[9]) && u != g_gameState->gameData->ctx.battle->curCmd->caster) {
        m->motion = (BtlUnit9CC *)u->motions[(u->flags >> 18) & 1];
        u->obj->frame = 0;
    }
}

void btl_rollFlee(BtlCalc *arg0) {
    s8 np;
    s8 ne;
    s16 lp;
    s16 le;
    Battle *b;
    BtlUnit *u;
    BtlCmd *c;
    s16 r;
    ne = np = 0;
    b = g_gameState->gameData->ctx.battle;
    le = lp = 0;
    u = b->unitList;
    c = b->curCmd;
    for (; u != NULL; u = u->next) {
        if (u->flags & 1) {
            np++;
            lp += u->level;
        } else {
            ne++;
            le += u->level;
        }
    }
    if (ne == 0) {
        ne = 1;
    }
    if (np == 0) {
        np = 1;
    }
    if (le == 0) {
        le = 1;
    }
    if (lp == 0) {
        lp = 1;
    }
    r = (200 / (le / ne)) * (lp / np) / 16;
    if (g_randPtr >= g_randTableEnd) {
        rand_refill();
        if (g_randPtr >= g_randTableEnd) {
            rand_refill();
        }
    }
    if (r > ((*g_randPtr << 8) | *g_randPtr++) % 100 && (c == NULL || (c->caster->flags & 1))) {
        btl_setCmd((s32)&g_gameState->gameData->ctx.battle->escapeCmd, BTL_CMD_ESCAPE, 1, 0xF, 1);
    }
}

u8 btl_checkFrozenShatter(BtlCalc *arg0) {
    BtlUnit *u;
    BtlUnit9C *m;
    if ((arg0->target->status & (BTL_STATUS_PETRIFY | BTL_STATUS_FREEZE)) != 0x02000000) {
        return 1;
    }
    btl_removeQueuedCmd(arg0->target, BTL_CMD_PETRIFY);
    u = arg0->target;
    u->hp = 0;
    u->flags |= 0x100000;
    if (!(u->flags & 1)) {
        btl_playDeathCry(u, g_gameState->gameData->ctx.battle->enemies[(u->flags >> 1) & 0xF].type->deathCry);
    }
    m = u->model;
    if ((s32)m->motion != u->motions[(u->flags >> 18) & 1] && (!(u->flags & 1) || (s32)m->motion != u->motions[9]) && u != g_gameState->gameData->ctx.battle->curCmd->caster) {
        m->motion = (BtlUnit9CC *)u->motions[(u->flags >> 18) & 1];
        u->obj->frame = 0;
    }
    btl_showMessage(BTL_MSG_IMPACT_CRUSHES);
    return 0;
}

s32 btl_consumeMp(BtlUnit *arg0, s16 arg1) {
    if (arg0 != NULL) {
        if (arg0->support[1] & 4) {
            arg1 /= 2;
        }
        if (arg0->mp < arg1) {
            return 0;
        }
        arg0->mp -= arg1;
        return 1;
    }
    return 0;
}

void func_800B95BC(BtlCmd *arg0) {
    if (g_randPtr >= g_randTableEnd) {
        rand_refill();
        if (g_randPtr >= g_randTableEnd) {
            rand_refill();
        }
    }
    if ((arg0->caster->level >> 1) + arg0->caster->spirit < ((*g_randPtr << 8) | *g_randPtr++) % 100) {
        arg0->unk10.b.unk0_9 = 1;
    }
}

s32 btl_checkSwdMagUsable(BtlUnit *arg0, s16 arg1) {
    BtlUnit *u;
    u32 f;
    if (arg0->mp >= arg1) {
        for (u = g_gameState->gameData->ctx.battle->unitList; u != NULL; u = u->next) {
            f = u->flags;
            if ((f & 1) && g_gameState->gameData->characters[(f >> 1) & 0xF].info.b.kind == 2) {
                if (!(u->status & (BTL_STATUS_PETRIFY | BTL_STATUS_VENOM | BTL_STATUS_SILENCE | BTL_STATUS_KO | BTL_STATUS_CONFUSE | BTL_STATUS_BERSERK | BTL_STATUS_STOP | BTL_STATUS_SLEEP | BTL_STATUS_HEAT | BTL_STATUS_FREEZE | BTL_STATUS_MINI)) && !(arg0->status & (BTL_STATUS_PETRIFY | BTL_STATUS_VENOM | BTL_STATUS_SILENCE | BTL_STATUS_KO | BTL_STATUS_CONFUSE | BTL_STATUS_BERSERK | BTL_STATUS_STOP | BTL_STATUS_SLEEP | BTL_STATUS_HEAT | BTL_STATUS_FREEZE | BTL_STATUS_MINI))) {
                    return 1;
                }
                break;
            }
        }
    }
    btl_showMessage(BTL_MSG_COMBINATION_FAILED);
    return 0;
}

void btl_killUnit(BtlUnit *arg0) {
    BtlUnit9C *m;
    arg0->hp = 0;
    arg0->flags |= 0x100000;
    if (!(arg0->flags & 1)) {
        btl_playDeathCry(arg0, g_gameState->gameData->ctx.battle->enemies[(arg0->flags >> 1) & 0xF].type->deathCry);
    }
    m = arg0->model;
    if ((s32)m->motion != arg0->motions[(arg0->flags >> 18) & 1] && (!(arg0->flags & 1) || (s32)m->motion != arg0->motions[9]) && arg0 != g_gameState->gameData->ctx.battle->curCmd->caster) {
        m->motion = (BtlUnit9CC *)arg0->motions[(arg0->flags >> 18) & 1];
        arg0->obj->frame = 0;
    }
}

void btl_rollShortSummon(BtlCmd *arg0) {
    s32 r;
    if (!(arg0->caster->support[1] & 0x08000000)) {
        if (arg0->ability->mpCost * 2 < arg0->caster->mp) {
            if (g_randPtr >= g_randTableEnd) {
                rand_refill();
            }
            r = *g_randPtr;
            g_randPtr++;
            r = r < 0xE6;
        } else {
            if (g_randPtr >= g_randTableEnd) {
                rand_refill();
            }
            r = *g_randPtr;
            g_randPtr++;
            r = r < 0xAA;
        }
        if (r) {
            arg0->unk10.b.shortSummon = 1;
        }
    }
}

s32 btl_canAddHitStatus(BtlCalc *arg0) {
    if (((arg0->flags & 0x100) && (g_gameState->gameData->ctx.battle->statusSets[arg0->caster->weapon[1]] & 0x42001103)) || (arg0->cmd->ability->flags & 0x40) || ((arg0->cmd->flags >> 5) & 1) || (arg0->target->status & (BTL_STATUS_PETRIFY | BTL_STATUS_VENOM | BTL_STATUS_KO | BTL_STATUS_STOP | BTL_STATUS_DEFEND | BTL_STATUS_FREEZE | BTL_STATUS_JUMP)) || arg0->caster == arg0->target) {
        return 0;
    }
    return 1;
}

void btl_setDrainFlags(BtlCalc *arg0) {
    BtlUnit *u;
    arg0->targetResult |= 1;
    u = arg0->target;
    arg0->casterResult |= 1;
    if ((u->status & BTL_STATUS_ZOMBIE) || btl_hasEnemyFlag(u, BTL_ENEMY_UNDEAD) != 0) {
        arg0->targetResult |= 2;
    } else {
        arg0->casterResult |= 2;
    }
}

void btl_initHitRates(BtlCalc *arg0) {
    arg0->hitRate = 100;
    arg0->evade = arg0->target->evade;
}

void btl_initStatusHitRate(BtlCalc *arg0) {
    BtlUnit *c = arg0->caster;
    arg0->hitRate = c->level + (arg0->cmd->ability->rate + (c->magic >> 2)) - arg0->target->level;
    if (arg0->hitRate > 100) {
        arg0->hitRate = 100;
    } else if (arg0->hitRate <= 0) {
        arg0->hitRate = 1;
    }
    arg0->evade = arg0->target->magicEva;
}

u8 btl_initUndeadHitRate(BtlCalc *arg0) {
    BtlUnit *t = arg0->target;
    if ((t->status & BTL_STATUS_ZOMBIE) || btl_hasEnemyFlag(t, BTL_ENEMY_UNDEAD) != 0) {
        BtlUnit *c = arg0->caster;
        arg0->hitRate = c->level + (arg0->cmd->ability->rate + (c->magic >> 2)) - arg0->target->level;
        if (arg0->hitRate > 100) {
            arg0->hitRate = 100;
        } else if (arg0->hitRate <= 0) {
            arg0->hitRate = 1;
        }
        arg0->evade = arg0->target->magicEva;
        return 0;
    }
    return 1;
}

u32 btl_hasSureHit(BtlCalc *arg0) {
    return *arg0->caster->support & 0x200;
}

void btl_halveHitForShell(BtlCalc *arg0) {
    if (arg0->target->status & BTL_STATUS_SHELL) {
        arg0->hitRate /= 2;
    }
}

void btl_halveHitForSpread(BtlCalc *arg0) {
    BtlCalc8 *c = arg0->cmd;
    if ((c->flags & 3) == 1 && (u32)((*(u32 *)c->ability & 0xF) - 3) < 3) {
        arg0->hitRate /= 2;
    }
}

u8 btl_checkLevelMultiple(BtlCalc *arg0) {
    if (arg0->target->level % arg0->cmd->ability->rate != 0) {
        arg0->flags |= 1;
        return 0;
    }
    return 1;
}

u8 btl_missIfDeathImmune(BtlCalc *arg0) {
    if (arg0->target->status & BTL_STATUS_DEATH_IMMUNE) {
        arg0->flags |= 1;
        return 0;
    }
    return 1;
}

u8 btl_checkTargetIsPlayer(BtlCalc *arg0) {
    if (!(arg0->target->flags & 1)) {
        arg0->flags |= 1;
        return 0;
    }
    return 1;
}

u8 btl_checkTargetAlive(BtlCalc *arg0) {
    if (arg0->target->status & (BTL_STATUS_PETRIFY | BTL_STATUS_KO)) {
        arg0->flags |= 1;
        return 0;
    }
    return 1;
}

u8 func_800B9DDC(BtlCalc *arg0) {
    u32 st = arg0->target->status;
    if ((st & BTL_STATUS_PETRIFY) || (st & (BTL_STATUS_ZOMBIE | BTL_STATUS_KO)) == (BTL_STATUS_ZOMBIE | BTL_STATUS_KO)) {
        arg0->flags |= 1;
        return 0;
    }
    return 1;
}

u8 btl_checkNotSelf(BtlCalc *arg0) {
    if (arg0->target == arg0->caster) {
        arg0->flags |= 1;
        return 0;
    }
    return 1;
}

u8 btl_guardIfDeathImmune(BtlCalc *arg0) {
    if (arg0->target->status & BTL_STATUS_DEATH_IMMUNE) {
        arg0->flags |= 0x40;
        return 0;
    }
    return 1;
}

void btl_setAbilityPhysParams(BtlCalc *arg0) {
    u8 r;
    s32 a;
    BtlUnit *c;
    if (g_randPtr >= g_randTableEnd) {
        rand_refill();
        if (g_randPtr >= g_randTableEnd) {
            rand_refill();
        }
    }
    r = *g_randPtr;
    c = arg0->caster;
    a = c->strength;
    arg0->multiplier = a + ((r << 8) | r) % (((c->level + a) >> 2) + 1);
    g_randPtr++;
    arg0->power = arg0->cmd->ability->power;
    arg0->defense = arg0->target->defense;
}

void btl_setMagicParams(BtlCalc *arg0) {
    u8 r;
    s32 a;
    BtlUnit *c;
    if (g_randPtr >= g_randTableEnd) {
        rand_refill();
        if (g_randPtr >= g_randTableEnd) {
            rand_refill();
        }
    }
    r = *g_randPtr;
    c = arg0->caster;
    a = c->magic;
    arg0->multiplier = a + ((r << 8) | r) % (((c->level + a) >> 3) + 1);
    g_randPtr++;
    arg0->power = arg0->cmd->ability->power;
    arg0->defense = arg0->target->magicDef;
}

void btl_setMultiplierFromPower(BtlCalc *arg0) {
    arg0->multiplier = arg0->cmd->ability->power;
}

void btl_setWeaponAbilityParams(BtlCalc *arg0) {
    s32 lo;
    if (g_randPtr >= g_randTableEnd) {
        rand_refill();
        if (g_randPtr >= g_randTableEnd) {
            rand_refill();
        }
    }
    lo = arg0->caster->strength;
    arg0->multiplier = lo + ((*g_randPtr << 8) | *g_randPtr++) % (((arg0->caster->level + lo) >> 3) + 1);
    arg0->power = arg0->caster->weapon[5] * arg0->cmd->ability->power / 10;
    arg0->defense = arg0->target->defense;
}

void btl_setEnemyAttackParams(BtlCalc *arg0) {
    u8 r;
    s32 a;
    BtlUnit *c;
    if (g_randPtr >= g_randTableEnd) {
        rand_refill();
        if (g_randPtr >= g_randTableEnd) {
            rand_refill();
        }
    }
    r = *g_randPtr;
    c = arg0->caster;
    a = c->strength;
    arg0->multiplier = a + ((r << 8) | r) % (((c->level + a) >> 2) + 1);
    g_randPtr++;
    arg0->power = arg0->cmd->ability->power;
    arg0->defense = arg0->target->defense;
}

void btl_applyCasterStatusMod(BtlCalc *arg0) {
    if (arg0->caster->status & (BTL_STATUS_BERSERK | BTL_STATUS_TRANCE)) {
        arg0->multiplier = arg0->multiplier * 3 >> 1;
    }
    if (arg0->caster->status & BTL_STATUS_MINI) {
        arg0->multiplier = 1;
    }
}

void func_800BA308(BtlCalc *arg0) {
    s32 r;
    if ((arg0->target->support[0] & 0x04000000) && arg0->defense != 0) {
        if (g_randPtr >= g_randTableEnd) {
            rand_refill();
            if (g_randPtr >= g_randTableEnd) {
                rand_refill();
            }
        }
        r = *g_randPtr;
        g_randPtr++;
        arg0->defense = ((r << 8) | r) % (arg0->defense * 2);
    }
}

void btl_applyTargetStatusMod(BtlCalc *arg0) {
    if (arg0->target->status & (BTL_STATUS_DEFEND | BTL_STATUS_PROTECT)) {
        arg0->multiplier >>= 1;
    }
    if (arg0->target->status & (BTL_STATUS_SLEEP | BTL_STATUS_MINI)) {
        arg0->multiplier = arg0->multiplier * 3 >> 1;
    }
}

void btl_applyCasterMiniMagic(BtlCalc *arg0) {
    if (arg0->caster->status & BTL_STATUS_MINI) {
        arg0->multiplier /= 2;
    }
}

void btl_applyTargetShell(BtlCalc *arg0) {
    if (arg0->target->status & BTL_STATUS_SHELL) {
        arg0->multiplier /= 2;
    }
}

void btl_applyHealBoost(BtlCalc *arg0) {
    if (arg0->caster->support[1] & 2) {
        arg0->multiplier = arg0->multiplier * 3 >> 1;
    }
}

void btl_applyRowMod(BtlCalc *arg0) {
    BtlUnit *t = arg0->target;
    BtlUnit *u;
    s32 d;
    if (arg0->caster->obj->rotY == t->obj->rotY || (g_gameState->gameData->ctx.battle->escapeHeld != 0 && (t->flags & 1))) {
        arg0->multiplier = arg0->multiplier * 3 >> 1;
    }
    u = arg0->caster;
    if (!(u->flags & 1) || (!(u->support[0] & 0x800) && !(u->weapon[0] & 2))) {
        d = (u->flags >> 8) & 3;
        d -= (arg0->target->flags >> 8) & 3;
        if ((d < 0 ? -d : d) >= 2) {
            arg0->multiplier /= 2;
        }
    }
}

void btl_applyWeaponElementBoost(BtlCalc *arg0) {
    BtlUnit *u = arg0->caster;
    if (u->elemBoost & u->weapon[6]) {
        arg0->multiplier = arg0->multiplier * 3 >> 1;
    }
}

u8 btl_applyWeaponElement(BtlCalc *arg0) {
    BtlUnit *t = arg0->target;
    u8 e = arg0->caster->weapon[6];
    if (t->elemGuard & e) {
        arg0->flags |= 0x40;
        return 0;
    }
    if (t->elemHalf & e) {
        arg0->multiplier /= 2;
    }
    if (arg0->target->elemAbsorb & e) {
        arg0->defense = 0;
        arg0->flags |= 8;
    }
    if (arg0->target->elemWeak & e) {
        arg0->multiplier = arg0->multiplier * 3 >> 1;
    }
    if (e & (BTL_ELEM_ICE | BTL_ELEM_WATER)) {
        btl_removeStatus(arg0->target, BTL_STATUS_HEAT);
    }
    if (e & BTL_ELEM_FIRE) {
        btl_removeStatus(arg0->target, BTL_STATUS_FREEZE);
    }
    return 1;
}

u8 btl_missIfAirborne(BtlCalc *arg0) {
    if ((btl_hasStatus(arg0->target, BTL_STATUS_FLOAT) & 0xFF) || btl_hasEnemyFlag(arg0->target, BTL_ENEMY_FLYING) != 0) {
        if (arg0->cmd->ability->element & BTL_ELEM_EARTH) {
            arg0->flags |= 1;
            return 0;
        }
        return 1;
    }
    return 1;
}

void btl_halveForSpread(BtlCalc *arg0) {
    BtlCalc8 *c = arg0->cmd;
    if ((c->flags & 3) == 1 && (u32)((*(u32 *)c->ability & 0xF) - 3) < 3 && btl_countActiveUnits(c->targets < 0x10) >= 2) {
        arg0->multiplier /= 2;
    }
}

void btl_applyAbilityElementBoost(BtlCalc *arg0) {
    if (arg0->caster->elemBoost & arg0->cmd->ability->element) {
        arg0->multiplier = arg0->multiplier * 3 >> 1;
    }
}

void func_800BA938(Unk800BA938 *arg0) {
    if ((g_gameState->gameData->ctx.battle->scene.unk0 >> 10) & 1) {
        arg0->unkE = 0x3C - arg0->unkE;
        if (arg0->unkE <= 0) {
            arg0->unkE = 1;
        }
    }
}

void btl_calcHeal(BtlCalc *arg0) {
    s32 v = arg0->power * arg0->multiplier;
    BtlUnit *u;
    if (v >= 10000) {
        v = 9999;
    }
    u = arg0->target;
    arg0->targetResult |= 1;
    if (!(u->status & BTL_STATUS_ZOMBIE) && btl_hasEnemyFlag(u, BTL_ENEMY_UNDEAD) == 0) {
        arg0->targetResult |= 2;
    }
    arg0->targetHpChange = v;
}

void btl_finishMagicDamage(BtlCalc *arg0) {
    s32 a = arg0->power - arg0->defense;
    s32 v;
    if (a <= 0) {
        a = 1;
    }
    arg0->targetResult |= 1;
    if (arg0->multiplier <= 0) {
        arg0->multiplier = 1;
    }
    v = a * arg0->multiplier;
    if ((arg0->cmd->flags >> 10) & 1) {
        v = v * 2 / 3;
    }
    if (v >= 10000) {
        v = 9999;
    }
    if (arg0->flags & 8) {
        arg0->targetResult |= 2;
    }
    arg0->targetHpChange = v;
}

void btl_calcPercentDamage(BtlCalc *arg0) {
    s32 v;
    u8 f;
    if (arg0->multiplier > 100) {
        arg0->multiplier = 100;
    }
    v = arg0->target->max->maxHp * arg0->multiplier / 100;
    if ((arg0->cmd->flags >> 10) & 1) {
        v = v * 2 / 3;
    }
    if (v >= 10000) {
        v = 9999;
    }
    f = arg0->targetResult;
    arg0->targetResult = f | 1;
    if (arg0->flags & 8) {
        arg0->targetResult = f | 3;
    }
    arg0->targetHpChange = v;
}

void btl_rollAbilityStatus(BtlCalc *arg0) {
    s32 r;
    s32 v;
    if (g_randPtr >= g_randTableEnd) {
        rand_refill();
        if (g_randPtr >= g_randTableEnd) {
            rand_refill();
        }
    }
    r = *g_randPtr;
    v = arg0->cmd->ability->rate;
    g_randPtr++;
    if (((r << 8) | r) % 100 < v) {
        btl_addStatuses(arg0->target, g_gameState->gameData->ctx.battle->statusSets[arg0->cmd->ability->statusSet]);
    }
}

void btl_applyAbilityStatus(BtlCalc *arg0) {
    BtlCalc8 *c = arg0->cmd;
    u32 mask = g_gameState->gameData->ctx.battle->statusSets[c->ability->statusSet];
    u32 r;
    if (!((c->flags >> 10) & 1) && c->cmdNo == 0x14) {
        mask |= 0x800000;
    }
    r = btl_addStatuses(arg0->target, mask);
    if (r == 0) {
        arg0->flags |= 0x40;
    } else if (r == 1) {
        arg0->flags |= 1;
    }
}

void btl_cureAbilityStatus(BtlCalc *arg0) {
    if (btl_removeStatuses(arg0->target, g_gameState->gameData->ctx.battle->statusSets[arg0->cmd->ability->statusSet]) != 2) {
        arg0->flags |= 1;
    }
}

void btl_cureItemStatus(BtlCalc *arg0) {
    if (btl_removeStatuses(arg0->target, D_8006B1F4[arg0->cmd->subNo - 0xE0].unk8) != 2) {
        arg0->flags |= 1;
    }
}

void btl_payMpAttack(BtlCalc *arg0) {
    if (arg0->flags & 4) {
        BtlUnit *u = arg0->caster;
        s32 v = u->max->maxMp >> 3;
        if (v == 0 || u->mp < v) {
            u->mp = 0;
        } else {
            u->mp -= v;
        }
    }
}

u8 btl_canFlee(BtlCalc *arg0) {
    Battle *b = g_gameState->gameData->ctx.battle;
    BtlUnit *u;
    if ((u8)b->scene.unk0 >> 7 == 0) {
        return 0;
    }
    for (u = b->unitList; u != NULL; u = u->next) {
        if ((u->flags & 1) && !(u->status & (BTL_STATUS_PETRIFY | BTL_STATUS_VENOM | BTL_STATUS_ZOMBIE | BTL_STATUS_KO | BTL_STATUS_STOP | BTL_STATUS_SLEEP | BTL_STATUS_FREEZE | BTL_STATUS_JUMP))) {
            return 1;
        }
    }
    return 0;
}

void btl_markTrouble(BtlCalc *arg0) {
    BtlUnit *u = arg0->target;
    if (u->status & BTL_STATUS_TROUBLE) {
        u->popupFlags |= 0x100;
    }
}

void btl_calcEnemySteal(BtlCalc *arg0) {
    s16 id = arg0->cmd->ability->rate;
    if (item_getCount(id) == 0) {
        btl_showMessage(BTL_MSG_STEAL_FAILED);
    } else {
        btl_useItem(id);
        btl_showMessage(BTL_MSG_WAS_STOLEN, btl_getItemName(id));
    }
}
