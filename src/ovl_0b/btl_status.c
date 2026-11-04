#include "ovl_0b.h"

u16 btl_getUnitMask(u32 side) {
    BtlUnit *unit;
    s32 bits;

    bits = 0;
    for (unit = g_gameState->gameData->ctx.battle->unitList; unit != NULL; unit = unit->next) {
        switch (side) {
        case 0:
            if (unit->flags & BTL_UNIT_PARTY) {
                bits |= unit->unitBit;
            }
            break;
        case 1:
            if (!(unit->flags & BTL_UNIT_PARTY)) {
                bits |= unit->unitBit;
            }
            break;
        case 2:
            bits |= unit->unitBit;
            break;
        }
    }
    return bits;
}

BtlUnit *btl_getUnitById(u16 unitBit) {
    BtlUnit *unit;
    for (unit = g_gameState->gameData->ctx.battle->unitList; unit != NULL; unit = unit->next) {
        if (unit->unitBit == unitBit) {
            return unit;
        }
    }
    return NULL;
}

BtlUnit *btl_findUnitByBit(u16 unitBit) {
    s32 i;
    Battle *battle = g_gameState->gameData->ctx.battle;
    for (i = 0; i < 8; i++) {
        if (battle->units[i].unitBit == unitBit) {
            return &battle->units[i];
        }
    }
    return NULL;
}

u32 btl_addStatus(BtlUnit *unit, u32 mask) {
    u32 bitNo = 0;
    BtlStatusInfo *tbl = g_gameState->gameData->ctx.battle->statusInfo;
    BtlUnit34 *statBlk = (BtlUnit34 *)&unit->statusImmune;
    u32 i;
    u32 blk;
    s16 k;

    if (unit->statusImmune & mask) {
        return 0;
    }
    if (statBlk->autoStatus & mask) {
        return 1;
    }
    if ((statBlk->status & mask) && (mask & (BTL_STATUS_PETRIFY | BTL_STATUS_VENOM | BTL_STATUS_VIRUS | BTL_STATUS_SILENCE | BTL_STATUS_BLIND | BTL_STATUS_TROUBLE | BTL_STATUS_ZOMBIE | BTL_STATUS_KO | BTL_STATUS_LOW_HP | BTL_STATUS_CONFUSE | BTL_STATUS_BERSERK | BTL_STATUS_STOP | BTL_STATUS_AUTO_LIFE | BTL_STATUS_TRANCE | BTL_STATUS_DEFEND | BTL_STATUS_DOOM | BTL_STATUS_GRADUAL_PETRIFY))) {
        return 1;
    }
    i = 0;
    blk = 0;
    for (; i < 32; i++) {
        if (mask & (1 << i)) {
            bitNo = i;
        }
        if (statBlk->status & (1 << i)) {
            blk |= tbl[i].prevents;
        }
    }
    if (btl_isCmdQueued(unit, BTL_CMD_PETRIFY)) {
        blk |= tbl[BTL_STATUS_PETRIFY_BIT].prevents;
    }
    if (blk & mask) {
        return 1;
    }
    if (!(mask & (BTL_STATUS_PETRIFY | BTL_STATUS_HASTE | BTL_STATUS_SLOW | BTL_STATUS_MINI))) {
        if ((mask & (BTL_STATUS_PETRIFY | BTL_STATUS_VENOM | BTL_STATUS_KO | BTL_STATUS_CONFUSE | BTL_STATUS_BERSERK | BTL_STATUS_SLEEP)) && (u8)btl_cancelUnitCmds(unit)) {
            unit->atb = 0;
            unit->flags &= ~BTL_UNIT_ATB_ACTIVE;
            if (!(mask & BTL_STATUS_KO)) {
                unit->cmdIssued = 0;
            }
        }
        statBlk->status |= mask;
    }
    switch (bitNo) {
    case BTL_STATUS_PETRIFY_BIT:
        if (!btl_isCmdPending(unit->koCmd)) {
            if (g_gameState->gameData->ctx.battle->phase >= BTL_PHASE_ENTRANCE) {
                btl_setCmd(unit->koCmd, BTL_CMD_PETRIFY, 0, unit->unitBit, 0);
            } else {
                if (unit->flags & BTL_UNIT_PARTY) {
                    btl_makeGreyCluts(unit);
                }
                statBlk->status |= mask;
                unit->flags &= ~BTL_UNIT_ATB_ACTIVE;
                btl_setStoneClut(unit, 1);
            }
        }
        break;
    case BTL_STATUS_VENOM_BIT:
        if (g_gameState->gameData->ctx.battle->phase >= BTL_PHASE_ENTRANCE) {
            btl_checkSideDefeated(unit);
        }
        /* Falls through into Sleep's motion switch, as in the original. At battle start (btl_initPartyUnit ->
         * btl_addStatuses, a party member carrying Venom over from the last battle) unit->model and curCmd are still
         * NULL, so the PS1 reads (and may write) low RAM through them here; a port guards both via PSX_NULLABLE
         * (ff9-ds F36: battles d2_b0779, d4_b0280/0779/0828/0940). */
    case BTL_STATUS_SLEEP_BIT:
        if ((unit->flags & BTL_UNIT_PARTY) && (s32)PSX_NULLABLE(unit->model)->motion != unit->motions[BTL_MOTION_IDLE_WEAK] &&
            PSX_NULLABLE(g_gameState->gameData->ctx.battle->curCmd)->caster != unit) {
            PSX_NULLABLE(unit->model)->motion = (BtlMotion *)unit->motions[BTL_MOTION_IDLE_WEAK];
            unit->obj->frame = 0;
        }
        break;
    case BTL_STATUS_ZOMBIE_BIT:
        if ((unit->flags & BTL_UNIT_PARTY) && !(unit->status & BTL_STATUS_TRANCE)) {
            unit->trance = 0;
        }
    case BTL_STATUS_BERSERK_BIT:
    case BTL_STATUS_HEAT_BIT:
    case BTL_STATUS_FREEZE_BIT:
        btl_updateStatusColor(unit);
        break;
    case BTL_STATUS_KO_BIT:
        if (unit->hp != 0) {
            unit->popupFlags |= BTL_POPUP_DEATH;
            btl_killUnit(unit);
        } else {
            unit->hp = 0;
        }
        unit->atb = 0;
        if (!btl_isCmdPending(unit->koCmd)) {
            btl_setCmd(unit->koCmd, BTL_CMD_KO, 0, unit->unitBit, 0);
            if (!(unit->flags & BTL_UNIT_PARTY)) {
                btl_checkEnemiesKO(unit);
            }
        }
        break;
    case BTL_STATUS_STOP_BIT:
        btl_checkSideDefeated(unit);
        break;
    case BTL_STATUS_TRANCE_BIT:
        btl_setCmd(unit->tranceCmd, BTL_CMD_TRANCE, 0, unit->unitBit, 0);
        break;
    case BTL_STATUS_HASTE_BIT:
        if (unit->status & BTL_STATUS_SLOW) {
            btl_removeStatus(unit, BTL_STATUS_SLOW);
            return 2;
        }
        btl_resetAtbSpeed((Unk800BD8F0 *)unit);
        unit->atbSpeed = unit->atbSpeed * 3 / 2;
        statBlk->status |= mask;
        break;
    case BTL_STATUS_SLOW_BIT:
        if (unit->status & BTL_STATUS_HASTE) {
            btl_removeStatus(unit, BTL_STATUS_HASTE);
            return 2;
        }
        btl_resetAtbSpeed((Unk800BD8F0 *)unit);
        unit->atbSpeed = unit->atbSpeed * 2 / 3;
        statBlk->status |= mask;
        break;
    case BTL_STATUS_VANISH_BIT:
        btl_hideMeshes(unit, unit->vanishMeshes);
        break;
    case BTL_STATUS_MINI_BIT:
        statBlk->status ^= mask;
        if (unit->status & BTL_STATUS_MINI) {
            unit->model->flags |= BTL_MODEL_SCALED;
            unit->model->scaleX = 0x800;
            unit->model->scaleY = 0x800;
            unit->model->scaleZ = 0x800;
            g_gameState->gameData->ctx.battle->work->shadows[btl_getModelSlot(unit)].width = (unit->shadowW >> 1) * 14;
            g_gameState->gameData->ctx.battle->work->shadows[btl_getModelSlot(unit)].depth = (unit->shadowD >> 1) * 12;
        } else {
            unit->model->flags &= ~BTL_MODEL_SCALED;
            g_gameState->gameData->ctx.battle->work->shadows[btl_getModelSlot(unit)].width = unit->shadowW * 14;
            g_gameState->gameData->ctx.battle->work->shadows[btl_getModelSlot(unit)].depth = unit->shadowD * 12;
        }
        break;
    }
    btl_removeStatuses(unit, tbl[bitNo].removes);
    if (unit->status & (BTL_STATUS_PETRIFY | BTL_STATUS_KO | BTL_STATUS_STOP | BTL_STATUS_JUMP)) {
        unit->flags &= ~BTL_UNIT_ATB_ACTIVE;
    }
    if (mask & BTL_STATUS_TIMED) {
        if (mask & (BTL_STATUS_POISON | BTL_STATUS_SLEEP | BTL_STATUS_SLOW | BTL_STATUS_HEAT | BTL_STATUS_FREEZE | BTL_STATUS_DOOM | BTL_STATUS_MINI | BTL_STATUS_GRADUAL_PETRIFY)) {
            k = (0x3C - unit->spirit) * 8;
        } else if (mask & (BTL_STATUS_REGEN | BTL_STATUS_HASTE | BTL_STATUS_FLOAT | BTL_STATUS_SHELL | BTL_STATUS_PROTECT | BTL_STATUS_VANISH | BTL_STATUS_REFLECT)) {
            k = unit->spirit * 8;
        } else {
            k = (0x3C - unit->spirit) * 4;
        }
        unit->statusTimers[bitNo - BTL_STATUS_TIMED_FIRST_BIT] = tbl[bitNo].timerScale * k;
    }
    if (mask & (BTL_STATUS_VENOM | BTL_STATUS_POISON | BTL_STATUS_REGEN)) {
        btl_initDotTimer(unit, bitNo);
    }
    return 2;
}

s32 btl_removeStatus(BtlUnit *unit, u32 mask) {
    BtlUnit34 *statBlk = (BtlUnit34 *)&unit->statusImmune;
    BtlModel *model;
    s32 *a4;
    BtlCmd *cmd;

    if (statBlk->autoStatus & mask) {
        return 1;
    }
    if (!(statBlk->status & mask)) {
        return 1;
    }
    if (!(unit->flags & BTL_UNIT_PARTY) && g_gameState->gameData->ctx.battle->phase == BTL_PHASE_END && (mask & (BTL_STATUS_PETRIFY | BTL_STATUS_VENOM | BTL_STATUS_STOP))) {
        return 1;
    }
    statBlk->status &= ~mask;
    switch (mask) {
    case BTL_STATUS_PETRIFY:
        btl_setStoneClut(unit, 0);
        break;
    case BTL_STATUS_BERSERK:
        if ((u8)btl_cancelUnitCmds(unit)) {
            unit->atb = 0;
            unit->cmdIssued = 0;
            unit->flags &= ~BTL_UNIT_ATB_ACTIVE;
        }
    case BTL_STATUS_ZOMBIE:
    case BTL_STATUS_HEAT:
    case BTL_STATUS_FREEZE:
        if (unit->status & (BTL_STATUS_ZOMBIE | BTL_STATUS_BERSERK | BTL_STATUS_HEAT | BTL_STATUS_FREEZE)) {
            btl_updateStatusColor(unit);
        }
        break;
    case BTL_STATUS_KO:
        model = unit->model;
        a4 = unit->motions;
        unit->koState = BTL_KO_NONE;
        unit->escapeStance = 0;
        unit->flags &= ~BTL_UNIT_HIT_MOTION;
        unit->flags &= ~BTL_UNIT_CMD_WAITING;
        unit->flags &= ~BTL_UNIT_KILLED;
        unit->flags &= ~BTL_UNIT_MOTION_HELD;
        if ((s32)model->motion == a4[BTL_MOTION_KO] || (s32)model->motion == a4[8]) {
            PSX_NULLABLE(model->texAnims)[2].flags |= BTL_TEXANIM_PLAY;
            PSX_NULLABLE(unit->model->texAnims)[2].pos = 0;
            PSX_NULLABLE(unit->model->texAnims)[2].lastFrame = 0x1000;
            unit->model->motion = (BtlMotion *)unit->motions[BTL_MOTION_GET_UP];
            unit->obj->frame = 0;
        }
        cmd = g_gameState->gameData->ctx.battle->curCmd;
        if (cmd->caster != unit || cmd->cmdNo > BTL_CMD_TABLE_COUNT) {
            unit->cmdIssued = 0;
        }
        btl_removeQueuedCmd(unit, BTL_CMD_KO);
        if (unit->autoStatus & BTL_STATUS_REGEN) {
            btl_initDotTimer(unit, BTL_STATUS_REGEN_BIT);
        }
        break;
    case BTL_STATUS_CONFUSE:
        unit->rot[1] = unit->obj->rotY;
        if ((u8)btl_cancelUnitCmds(unit)) {
            unit->atb = 0;
            unit->cmdIssued = 0;
            unit->flags &= ~BTL_UNIT_ATB_ACTIVE;
        }
        break;
    case BTL_STATUS_TRANCE:
        unit->trance = 0;
        btl_cancelTranceEidolon(unit);
        if (unit->status & BTL_STATUS_JUMP) {
            btl_removeStatus(unit, BTL_STATUS_JUMP);
            unit->flags &= ~BTL_UNIT_HIDDEN;
            unit->pos[0] = unit->home->pos[0];
            unit->pos[1] = unit->home->pos[1];
            unit->pos[2] = unit->home->pos[2];
            unit->model->motion = (BtlMotion *)unit->motions[(unit->flags >> BTL_UNIT_WEAK_STANCE_BIT) & 1];
            unit->obj->frame = 0;
        }
        btl_setCmd(unit->tranceCmd, BTL_CMD_TRANCE, 0, unit->unitBit, 0);
        break;
    case BTL_STATUS_HASTE:
    case BTL_STATUS_SLOW:
        btl_resetAtbSpeed((Unk800BD8F0 *)unit);
        break;
    case BTL_STATUS_FLOAT:
        unit->pos[1] = 0;
        unit->home->pos[1] = 0;
        break;
    case BTL_STATUS_VANISH:
        btl_showMeshes(unit, unit->vanishMeshes);
        break;
    case BTL_STATUS_MINI:
        unit->model->flags &= ~BTL_MODEL_SCALED;
        g_gameState->gameData->ctx.battle->work->shadows[btl_getModelSlot(unit)].width = unit->shadowW * 14;
        g_gameState->gameData->ctx.battle->work->shadows[btl_getModelSlot(unit)].depth = unit->shadowD * 12;
        break;
    case BTL_STATUS_JUMP:
        unit->targetableReq = BTL_TGTREQ_SET_ON;
        unit->atb = 0;
        unit->cmdIssued = 0;
        unit->flags |= BTL_UNIT_ATB_ACTIVE;
        unit->delayedCmd->cmdNo = 0;
        unit->delayedCmd->targets = 0;
        break;
    case BTL_STATUS_GRADUAL_PETRIFY:
        btl_removeQueuedCmd(unit, BTL_CMD_PETRIFY);
        break;
    }
    return 2;
}

void btl_updateUnitStatus(BtlUnit *unit) {
    BtlUnit34 *timers = (BtlUnit34 *)&unit->statusImmune;
    BtlCmd *cmd;
    Battle *battle;
    u16 cmdFlags;

    btl_updateUnitTint(unit);
    if (unit->status & BTL_STATUS_KO) {
        btl_updateKo(unit);
    } else if (!(unit->status & BTL_STATUS_PETRIFY)) {
        if (!(unit->status & (BTL_STATUS_PETRIFY | BTL_STATUS_KO | BTL_STATUS_STOP | BTL_STATUS_JUMP))) {
            unit->flags |= BTL_UNIT_ATB_ACTIVE;
        }
        if (btl_isAtbRunning()) {
            if ((unit->flags >> BTL_UNIT_ATB_ACTIVE_BIT) & 1) {
                if (unit->status & BTL_STATUS_VENOM) {
                    if (timers->venomTimer <= 0) {
                        btl_initDotTimer(unit, BTL_STATUS_VENOM_BIT);
                        btl_tickPoison(unit);
                        btl_tickVenomMp(unit);
                        btl_spawnTickPopups(unit);
                    } else {
                        timers->venomTimer -= unit->atbSpeed;
                    }
                }
                if (unit->status & BTL_STATUS_POISON) {
                    if (timers->poisonTimer <= 0) {
                        btl_initDotTimer(unit, BTL_STATUS_POISON_BIT);
                        btl_tickPoison(unit);
                        btl_spawnTickPopups(unit);
                    } else {
                        timers->poisonTimer -= unit->atbSpeed;
                    }
                }
                if (BtlUnitHasStatus(unit, BTL_STATUS_REGEN)) {
                    if (timers->regenTimer <= 0) {
                        btl_initDotTimer(unit, BTL_STATUS_REGEN_BIT);
                        btl_tickRegen(unit);
                        btl_spawnTickPopups(unit);
                    } else {
                        timers->regenTimer -= unit->atbSpeed;
                    }
                }
                if ((unit->status & BTL_STATUS_TRANCE) && BTL_UNIT_INDEX(unit->flags) == CHR_DAGGER) {
                    battle = g_gameState->gameData->ctx.battle;
                    cmdFlags = battle->cmdFlags;
                    if (cmdFlags & BTL_CMDF_EIDOLON_TIMER) {
                        if (!(cmdFlags & BTL_CMDF_EIDOLON_QUEUED)) {
                            if (battle->eidolonTimer <= 0) {
                                btl_setCmd(unit->delayedCmd, BTL_CMD_TRANCE_EIDOLON, g_gameState->gameData->ctx.battle->eidolonAbility, btl_getUnitsWithStatus(1, 0), 8);
                                g_gameState->gameData->ctx.battle->cmdFlags |= BTL_CMDF_EIDOLON_QUEUED;
                            } else {
                                battle->eidolonTimer -= unit->atbSpeed;
                            }
                        }
                    }
                }
                btl_tickStatusTimers(unit);
            } else if ((unit->status & BTL_STATUS_JUMP) && !(g_gameState->gameData->ctx.battle->cmdFlags & BTL_CMDF_SPEAR)) {
                timers->jumpTimer -= unit->atbSpeed;
                if (timers->jumpTimer < 0) {
                    cmd = unit->delayedCmd;
                    if (cmd->cmdNo == BTL_CMD_JUMP) {
                        btl_queueAutoCmd(unit, BTL_CMD_SPEAR, ABIL_SPEAR, (u16)cmd->targets);
                    } else {
                        btl_queueAutoCmd(unit, BTL_CMD_SPEAR_TRANCE, ABIL_SPEAR_TRANCE, (u16)cmd->targets);
                    }
                }
            }
        }
    }
    if ((unit->status & BTL_STATUS_CONFUSE) && unit != PSX_NULLABLE(g_gameState->gameData->ctx.battle->curCmd)->caster) {
        if (unit->model->motion == (BtlMotion *)unit->motions[BTL_MOTION_IDLE] || unit->model->motion == (BtlMotion *)unit->motions[BTL_MOTION_IDLE_WEAK] ||
            ((unit->flags & BTL_UNIT_PARTY) && unit->model->motion == (BtlMotion *)unit->motions[BTL_MOTION_READY])) {
            unit->rot[1] += 0x80;
        }
    }
}

void btl_updateUnitTint(BtlUnit *unit) {
    BtlBgHeader *bg;
    u8 *rgb;
    s32 floatY;
    u32 tmp; /* battle tick, then the Float test (separate locals don't match) */
    u32 phase;
    u8 val;
    u32 w;
    s16 r, g, b;
    s32 rr, gg, bb;

    if (!((unit->flags >> BTL_UNIT_HIDDEN_BIT) & 1) && !(unit->status & BTL_STATUS_PETRIFY)) {
        bg = btlbg_getHeader();
        if (unit->status & (BTL_STATUS_ZOMBIE | BTL_STATUS_BERSERK | BTL_STATUS_HEAT | BTL_STATUS_FREEZE)) {
            btl_fillModelRgb(unit->model, unit->tint[0], unit->tint[1], unit->tint[2]);
            if (unit->weaponModel != NULL) {
                btl_fillModelRgb(unit->weaponModel, unit->tint[0], unit->tint[1], unit->tint[2]);
            }
        } else {
            if (unit->status & (BTL_STATUS_SHELL | BTL_STATUS_PROTECT)) {
                tmp = g_gameState->gameData->ctx.battle->tick;
                phase = tmp;
                phase = phase % 24;
                /* val is the colour-select flag here and the fade level below; the table branch's level goes
                   through w (a u32), so its loads stay word-sized. */
                if ((unit->status & (BTL_STATUS_SHELL | BTL_STATUS_PROTECT)) == (BTL_STATUS_SHELL | BTL_STATUS_PROTECT)) {
                    val = (tmp % 48) >= 24;
                } else {
                    val = (unit->status & BTL_STATUS_PROTECT) < BTL_STATUS_PROTECT;
                }
                if (val) {
                    r = bg->lightR - 0x40;
                    g = bg->lightG + 0x18;
                    b = bg->lightB + 0x48;
                } else {
                    r = bg->lightR + 0x28;
                    g = bg->lightG + 0x28;
                    b = bg->lightB - 0x50;
                }
                if ((u8)phase < 8) {
                    val = phase;
                } else if ((u8)phase < 16) {
                    val = 8;
                } else {
                    val = 24 - phase;
                }
            } else if ((unit->status & (BTL_STATUS_KO | BTL_STATUS_TRANCE)) == BTL_STATUS_TRANCE) {
                u8 tbl[8][3] = {
                    { 0xFF, 0x60, 0x60 }, { 0x68, 0x78, 0xFF }, { 0xFF, 0xB8, 0x78 }, { 0x80, 0xC0, 0xD0 },
                    { 0xC0, 0x68, 0x90 }, { 0xD0, 0x70, 0x78 }, { 0xC8, 0xD8, 0x68 }, { 0xD0, 0xB8, 0x68 },
                };
                rgb = tbl[BTL_UNIT_INDEX(unit->flags)];
                r = bg->lightR + (rgb[0] - 0x80);
                g = bg->lightG + (rgb[1] - 0x80);
                b = bg->lightB + (rgb[2] - 0x80);
                w = g_gameState->gameData->ctx.battle->tick & 0xF;
                if (w >= 8) {
                    w = 16 - w;
                }
                val = w;
            } else {
                goto end;
            }
            rr = (r * val) << 13 >> 16;
            gg = (g * val) << 13 >> 16;
            bb = (b * val) << 13 >> 16;
            btl_setModelRgb(unit->model, rr, gg, bb);
            if (unit->weaponModel != NULL) {
                btl_setModelRgb(unit->weaponModel, rr, gg, bb);
            }
        }
    }
end:
    tmp = BtlUnitHasStatus(unit, BTL_STATUS_FLOAT);
    if ((u8)tmp) {
        unit->pos[1] = floatY = 0xFF380000 - math_sin((g_gameState->gameData->ctx.battle->tick & 0xF) << 8) * 480;
        unit->home->pos[1] = floatY;
    }
}

void btl_updateStatusColor(BtlUnit *unit) {
    s16 shift[3];
    s16 light[3];
    BtlBgHeader *bg = btlbg_getHeader();
    s32 i;
    s32 level;
    light[0] = bg->lightR;
    light[1] = bg->lightG;
    light[2] = bg->lightB;
    if (unit->status & BTL_STATUS_ZOMBIE) {
        shift[0] = 0x30;
        shift[1] = 0x48;
        shift[2] = 0x58;
    } else if (unit->status & BTL_STATUS_BERSERK) {
        shift[0] = -0x10;
        shift[1] = 0x28;
        shift[2] = 0x28;
    } else if (unit->status & BTL_STATUS_HEAT) {
        shift[0] = -0x50;
        shift[1] = 0x10;
        shift[2] = 0x48;
    } else if (unit->status & BTL_STATUS_FREEZE) {
        shift[0] = 0x30;
        shift[1] = 0;
        shift[2] = -0x60;
    } else {
        shift[2] = 0;
        shift[1] = 0;
        shift[0] = 0;
    }
    for (i = 0; i < 3; i++) {
        level = light[i] - shift[i];
        if (level < 0) {
            unit->tint[i] = 0;
        } else if (level >= 0x100) {
            unit->tint[i] = 0xFF;
        } else {
            unit->tint[i] = light[i] - shift[i];
        }
    }
}

void btl_setStoneClut(BtlUnit *unit, s32 stone) {
    s8 delta;
    s8 i;
    delta = 6;
    if (unit->flags & BTL_UNIT_PARTY) {
        delta = -12;
    }
    if (stone & 0xFF) {
        delta *= -1;
    }
    i = 0;
    if (unit->flags & BTL_UNIT_PARTY) {
        unit->weaponModel->texPages->clut = getClut(VRAM_MODEL_CLUT_X, delta + clut_getY(unit->weaponModel->texPages->clut));
    }
    while (i < db_getModelTexCount(unit->modelId | DB_MODEL)) {
        unit->model->texPages[i].clut = getClut(VRAM_MODEL_CLUT_X, delta + clut_getY(unit->model->texPages[i].clut));
        i++;
    }
}

void btl_makeGreyCluts(BtlUnit *unit) {
    s16 i;
    s16 j;
    u8 rgb[3];
    u8 grey;

    for (i = 0; i < db_getModelTexCount(unit->modelId | DB_MODEL); i++) {
        g_btlClutRect.x = 0x100;
        g_btlClutRect.y = clut_getY(unit->model->texPages[i].clut);
        g_btlClutRect.w = 0x100;
        g_btlClutRect.h = 1;
        StoreImage(&g_btlClutRect, (u32 *)g_btlClutBuf[i]);
        DrawSync(0);
        for (j = 0; j < 0x100; j++) {
            rgb[0] = g_btlClutBuf[i][j] & 0x1F;
            rgb[1] = (g_btlClutBuf[i][j] >> 5) & 0x1F;
            rgb[2] = (g_btlClutBuf[i][j] >> 10) & 0x1F;
            if (rgb[0] == 0 && rgb[1] == 0 && rgb[2] == 0) {
                grey = 0;
            } else {
                grey = (rgb[0] + rgb[1] + rgb[2]) / 3;
                if (grey == 0) {
                    grey = 1;
                }
            }
            g_btlClutBuf[i][j] = (grey << 10) + (grey << 5) + grey;
        }
        g_btlClutRect.y += 12;
        LoadImage(&g_btlClutRect, (u32 *)g_btlClutBuf[i]);
    }
    DrawSync(0);
}

void btl_makeEnemyGreyCluts(void) {
    u16 *p = (u16 *)g_btlSavedCluts;
    s32 i;
    s8 rgb[3];
    s8 grey;
    for (i = 0; i < 0x600; i++) {
        rgb[0] = *p & 0x1F;
        rgb[1] = (*p >> 5) & 0x1F;
        rgb[2] = (*p >> 10) & 0x1F;
        if (rgb[0] == 0 && rgb[1] == 0 && rgb[2] == 0) {
            grey = 0;
        } else {
            grey = (rgb[0] + rgb[1] + rgb[2]) / 3;
            if (grey <= 0) {
                grey = 1;
            }
        }
        *p = grey + ((grey << 10) + (grey << 5));
        p++;
    }
    g_btlClutRect.x = 0x100;
    g_btlClutRect.y = 0xDA;
    g_btlClutRect.h = 6;
    g_btlClutRect.w = 0x100;
    LoadImage(&g_btlClutRect, g_btlSavedCluts);
}

void btl_makePartyGreyCluts(void) {
    s32 i;
    s32 j;
    s8 rgb[3];
    s8 grey;
    u16 *cluts;
    g_btlClutRect.x = 0x100;
    g_btlClutRect.y = 0xF2;
    g_btlClutRect.w = 0x100;
    g_btlClutRect.h = 1;
    for (i = 2; i < 0xC; i += 3) {
        j = 0;
        cluts = (u16 *)g_btlSavedCluts;
        for (; j < 0x100; j++) {
            rgb[0] = cluts[(i << 8) + j] & 0x1F;
            rgb[1] = (cluts[(i << 8) + j] >> 5) & 0x1F;
            rgb[2] = (cluts[(i << 8) + j] >> 10) & 0x1F;
            if (rgb[0] == 0 && rgb[1] == 0 && rgb[2] == 0) {
                grey = 0;
            } else {
                grey = (rgb[0] + rgb[1] + rgb[2]) / 3;
                if (grey <= 0) {
                    grey = 1;
                }
            }
            cluts[(i << 8) + j] = grey + ((grey << 10) + (grey << 5));
        }
        g_btlClutRect.y = i + 0xF2;
        LoadImage(&g_btlClutRect, (u32 *)((u16 *)g_btlSavedCluts + (i << 8)));
    }
}

s32 btl_hasStatus(BtlUnit *unit, u32 statuses) {
    s32 ret = 0;
    if ((unit->autoStatus & statuses) || (unit->status & statuses)) {
        ret = 1;
    }
    return ret;
}

void btl_saveCharStatus(Unk800C09D8 *chr, Unk800C09D8 *unit) {
    chr->chrStatus = unit->status & BTL_STATUS_KEPT;
}

void btl_clearUnitStatus(BtlUnit *unit) {
    u32 i;
    s16 *timers;
    unit->status = 0;
    unit->autoStatus = 0;
    unit->statusImmune = 0;
    timers = unit->dotTimers;
    for (i = 0; i < 3; i++) {
        timers[i] = 0;
    }
    for (i = 0; i < 14; i++) {
        timers[i + 3] = 0;
    }
}

void btl_initDoomCount(Unk800C09D8 *unit) {
    s32 base = (0x3C - unit->spirit) * 8;
    unit->countdownStep = base * g_gameState->gameData->ctx.battle->statusInfo[BTL_STATUS_DOOM_BIT].timerScale;
}

/* status: the status that interrupts (BTL_CMD_PETRIFY passes BTL_STATUS_PETRIFY); with BTL_STATUS_KO the unit
 * keeps cmdIssued */
void btl_interruptUnit(BtlUnit *unit, s32 status) {
    if (btl_cancelUnitCmds(unit) & 0xFF) {
        unit->atb = 0;
        unit->flags &= ~BTL_UNIT_ATB_ACTIVE;
        if (!(status & BTL_STATUS_KO)) {
            unit->cmdIssued = 0;
        }
    }
}

u32 btl_addStatuses(BtlUnit *unit, u32 statuses) {
    u32 ret = 0;
    u32 i;
    for (i = 0; i < 32; i++) {
        if (statuses & (1 << i)) {
            u32 result = btl_addStatus(unit, 1 << i);
            if ((ret == 0 && result != 0) || (ret == 1 && result >= 2)) {
                ret = result;
            }
        }
    }
    return ret;
}

s32 btl_removeStatuses(BtlUnit *unit, u32 statuses) {
    s32 ret = 1;
    u32 i;
    for (i = 0; i < 32; i++) {
        if ((statuses & (1 << i)) && btl_removeStatus(unit, 1 << i) == 2) {
            ret = 2;
        }
    }
    return ret;
}

void btl_initDotTimer(BtlUnit *unit, s32 bitNo) {
    u8 k;
    s32 base;

    if (bitNo == BTL_STATUS_VENOM_BIT) {
        base = unit->spirit * 4;
        k = 0;
    } else if (bitNo == BTL_STATUS_POISON_BIT) {
        base = unit->spirit * 4;
        k = 1;
    } else {
        base = (0x3C - unit->spirit) * 4;
        k = 2;
    }
    unit->dotTimers[k] = g_gameState->gameData->ctx.battle->statusInfo[bitNo].tickScale * base;
}

void btl_saveEnemyCluts(u32 *dst) {
    g_btlClutRect.x = 0x100;
    g_btlClutRect.y = 0xE0;
    g_btlClutRect.w = 0x100;
    g_btlClutRect.h = 6;
    StoreImage(&g_btlClutRect, dst);
    g_btlSavedCluts = dst;
}

void btl_savePartyCluts(u32 *dst) {
    g_btlClutRect.x = 0x100;
    g_btlClutRect.y = 0xE6;
    g_btlClutRect.w = 0x100;
    g_btlClutRect.h = 0xC;
    StoreImage(&g_btlClutRect, dst);
    g_btlSavedCluts = dst;
}

void btl_reapplyStatusLook(BtlUnit *unit) {
    if (unit->status & BTL_STATUS_PETRIFY) {
        btl_setStoneClut(unit, 1);
    } else if (unit->status & (BTL_STATUS_ZOMBIE | BTL_STATUS_BERSERK | BTL_STATUS_HEAT | BTL_STATUS_FREEZE)) {
        btl_updateStatusColor(unit);
    }
    btl_applyBgLight(unit->model);
    if (unit->flags & BTL_UNIT_PARTY) {
        btl_applyBgLight(unit->weaponModel);
    }
}

void btl_tickStatusTimers(BtlUnit *unit) {
    s32 i;
    s32 bit;
    s32 timed;
    for (i = 0; i < 16; i++) {
        bit = BTL_STATUS_POISON << i;
        timed = bit & BTL_STATUS_TIMED;
        if ((unit->status & timed) && (s16)(unit->statusTimers[i] -= unit->atbSpeed) < 0) {
            if (bit < 0) {
                if (!btl_isCmdPending(unit->koCmd) && btl_addStatus(unit, BTL_STATUS_PETRIFY) != 2) {
                    btl_removeStatus(unit, BTL_STATUS_GRADUAL_PETRIFY);
                    unit->popupFlags |= BTL_POPUP_MISS;
                    btl_spawnPopups(unit);
                }
            } else if (bit & BTL_STATUS_DOOM) {
                btl_addStatus(unit, BTL_STATUS_KO);
                btl_spawnPopups(unit);
            } else {
                btl_removeStatus(unit, bit);
            }
        }
    }
}

void btl_checkSideDefeated(BtlUnit *unit) {
    Battle *battle = g_gameState->gameData->ctx.battle;
    s32 found;
    BtlUnit *other;
    u32 flags;
    s32 kind;
    s32 koDone;

    koDone = BTL_KO_DONE;
    for (other = battle->unitList; other != NULL; other = other->next) {
        if ((other->flags & BTL_UNIT_PARTY) == (unit->flags & BTL_UNIT_PARTY)) {
            if (!(other->status & (BTL_STATUS_PETRIFY | BTL_STATUS_VENOM | BTL_STATUS_KO | BTL_STATUS_STOP)) || ((other->status & BTL_STATUS_KO) && other->koState != koDone)) {
                return;
            }
        }
    }
    if (!(unit->flags & BTL_UNIT_PARTY)) {
        found = 0;
        for (other = battle->unitList; other != NULL; other = other->next) {
            if (other->flags & BTL_UNIT_PARTY) {
                if (!(other->status & (BTL_STATUS_PETRIFY | BTL_STATUS_VENOM | BTL_STATUS_KO | BTL_STATUS_STOP)) || ((other->hp == 0 || (other->status & BTL_STATUS_KO)) && (other->status & BTL_STATUS_AUTO_LIFE)) || btl_isCmdQueued(other, BTL_CMD_AUTO_LIFE)) {
                    found = 1;
                    break;
                }
            }
        }
        if (!found) {
            return;
        }
        /* subPhase only means the end kind in BTL_PHASE_END; in combat it is 0 on the PS1 (camera cues with an
         * argument other than BTLCAM_CUE_END_INTRO are only in victory cameras), so this only keeps a defeat
         * decided earlier in the same step. A stray nonzero subPhase here turns the win into a defeat. */
        if (battle->subPhase != BTL_END_DEFEAT) {
            battle->subPhase = BTL_END_VICTORY;
        }
    } else {
        for (other = battle->unitList; other != NULL; other = other->next) {
            flags = other->flags;
            if (flags & BTL_UNIT_PARTY) {
                kind = g_gameState->gameData->characters[BTL_UNIT_INDEX(flags)].info.b.kind;
            } else {
                kind = CHR_KIND_NONE;
            }
            if ((u32)(kind - CHR_KIND_EIKO) < 2) {
                if (other->status & (BTL_STATUS_PETRIFY | BTL_STATUS_VENOM | BTL_STATUS_ZOMBIE | BTL_STATUS_STOP)) {
                    break;
                }
                if (btl_isCmdQueued(other, BTL_CMD_REBIRTH_FLAME)) {
                    return;
                }
                /* rand8_inline() split around item_getCount: the refill check comes before the call */
                if (g_randPtr >= g_randTableEnd) {
                    rand_refill();
                }
                kind = item_getCount(ITEM_PHOENIX_PINION);
                if (*g_randPtr++ < kind) {
                    btlui_setAtbEnabled(1);
                    btl_setCmd(other->cmd, BTL_CMD_REBIRTH_FLAME, ABIL_REBIRTH_FLAME, btl_getUnitMask(0) & 0xFFFF, 1);
                    return;
                }
            }
        }
        battle->subPhase = BTL_END_DEFEAT;
        btl_showMessage(BTL_MSG_ANNIHILATED);
    }
    btlui_setAtbEnabled(0);
    battle->phase = BTL_PHASE_END;
    btl_clearCmdQueue(battle);
}

s32 btl_rollAmbush(u32 *sceneFlags) {
    s32 result = BTL_AMBUSH_NORMAL;
    s32 threshold;
    if ((*sceneFlags >> BTL_SCENE_SCRIPT_START_BIT) & 1) {
        result = (((*sceneFlags >> BTL_SCENE_BACK_ATTACK_BIT) & 1) == 0) * BTL_AMBUSH_NORMAL;
    } else {
        threshold = 0x18;
        if ((u8)btl_partyHasSupport(1, SA_BIT(SA_ALERT))) {
            threshold = 0;
        } else if ((*sceneFlags >> BTL_SCENE_BACK_ATTACK_BIT) & 1) {
            threshold = 0xFF;
        }
        if (rand8_inline() < threshold) {
            result = BTL_AMBUSH_BACK;
        } else {
            threshold = 0x10;
            if ((u8)btl_partyHasSupport(1, SA_BIT(SA_INITIATIVE))) {
                threshold = 0x55;
            }
            if (rand8_inline() < threshold) {
                result = BTL_AMBUSH_PREEMPTIVE;
            }
        }
    }
    return result;
}

void btl_addEnemyRewards(BtlEnemyType *enemyType) {
    u8 slot;
    u8 itemId;
    Unk800FDF30 *rewards;

    slot = 0;
    rewards = &g_btlRewards;
    rewards->gil += enemyType->gil;
    rewards->exp += enemyType->exp;
    for (; slot < BTL_REWARD_ITEM_SLOTS; slot++) {
        if (rewards->items[slot] == ITEM_NONE) {
            break;
        }
    }
    if (slot < BTL_REWARD_ITEM_SLOTS) {
        if (rand8_inline() < 1 && (itemId = enemyType->drops.b[3]) != ITEM_NONE) {
            rewards->items[slot++] = itemId;
            enemyType->drops.b[3] = ITEM_NONE;
        } else if (rand8_inline() < 0x20 && (itemId = enemyType->drops.b[2]) != ITEM_NONE) {
            rewards->items[slot++] = itemId;
            enemyType->drops.b[2] = ITEM_NONE;
        } else if (rand8_inline() < 0x60 && (itemId = enemyType->drops.b[1]) != ITEM_NONE) {
            rewards->items[slot++] = itemId;
            enemyType->drops.b[1] = ITEM_NONE;
        }
        if ((itemId = enemyType->drops.b[0]) != ITEM_NONE) {
            rewards->items[slot] = itemId;
        }
    }
    if (rewards->card == CARD_NONE && (u32)enemyType->card < CARD_COUNT) {
        if (rand8_inline() < 0x20) {
            rewards->card = enemyType->card;
        }
    }
}

void btl_initState(void) {
    Battle *battle = g_gameState->gameData->ctx.battle;
    battle->unitList = NULL;
    battle->tick = 0;
    battle->phase = 0;
    battle->subPhase = 0;
    battle->exitTimer = 0;
    battle->escapeHeld = 0;
    battle->fleeBrightness = 0x20;
    battle->loadFlags = 0;
    battle->partyBrightness = 0;
    battle->enemyBrightness = 0;
    battle->eidolonAbility = 0;
    battle->eidolonTimer = 0;
    battle->deathCryWait = 0;
    g_btlRewards.expMask = 0;
    btl_clearRewards();
}

void btl_clearRewards(void) {
    s32 i;
    u8 *p;
    s32 fill;

    fill = 0xFF;
    i = 15;
    p = (u8 *)&g_btlRewards + i;
    g_btlRewards.gil = 0;
    g_btlRewards.exp = 0;
    g_btlRewards.ap = 0;
    for (; i >= 0; i--) {
        p[12] = fill; /* unkC[i] */
        p--;
    }
    g_btlRewards.card = CARD_NONE;
    g_btlRewards.fleeGil = 0;
}

void btl_checkPartyDisabled(BtlUnit *unit) {
    Battle *battle = g_gameState->gameData->ctx.battle;
    BtlUnit *other;
    BtlUnit *six = 0;
    u32 status;
    for (other = battle->unitList; other != 0; other = other->next) {
        if (BTL_UNIT_INDEX(other->flags) == 6) {
            six = other;
        }
        if ((other->flags & BTL_UNIT_PARTY) == (unit->flags & BTL_UNIT_PARTY)) {
            status = other->status;
            if (!(status & (BTL_STATUS_PETRIFY | BTL_STATUS_VENOM | BTL_STATUS_KO | BTL_STATUS_STOP))) {
                return;
            }
            if ((other->hp == 0 || (status & BTL_STATUS_KO)) && (status & BTL_STATUS_AUTO_LIFE)) {
                return;
            }
            if (btl_isCmdQueued(other, BTL_CMD_AUTO_LIFE)) {
                return;
            }
        }
    }
    if (six == 0 || !btl_isCmdQueued(six, BTL_CMD_REBIRTH_FLAME)) {
        btlui_setAtbEnabled(0);
        btl_flushCmdQueue(battle);
        battle->escapeHeld = 0;
    }
}

void btl_checkEnemiesKO(void) {
    Battle *battle = g_gameState->gameData->ctx.battle;
    BtlUnit *unit;
    for (unit = battle->unitList; unit != NULL; unit = unit->next) {
        if (!(unit->flags & BTL_UNIT_PARTY) && !(unit->status & BTL_STATUS_KO)) {
            return;
        }
    }
    btlui_setAtbEnabled(0);
    btl_flushCmdQueue(battle);
    battle->escapeHeld = 0;
}

s32 btl_saveParty(Battle *battle) {
    GameData *gd = g_gameState->gameData;
    u8 result = gd->battleResult;
    if ((result == BATTLE_END_DEFEAT || result == BATTLE_END_GAME_OVER) && !((battle->scene.flags >> BTL_SCENE_NO_GAMEOVER_BIT) & 1)) {
        gd->battleResult = BATTLE_END_GAME_OVER;
    } else {
        BtlUnit *unit = g_gameState->gameData->ctx.battle->unitList;
        while (unit != NULL) {
            if (unit->flags & BTL_UNIT_PARTY) {
                btl_saveCharacter(unit, 0);
            }
            unit = unit->next;
        }
    }
    return 1;
}

void btl_saveCharacter(BtlUnit *unit, s32 mode) {
    Character *chr = &g_gameState->gameData->characters[BTL_UNIT_INDEX(unit->flags)];
    if (unit->status & BTL_STATUS_TRANCE) {
        chr->trance = 0;
    } else {
        chr->trance = unit->trance;
    }
    btl_copyStats((Unk800BD8C0 *)&chr->hp, (Unk800BD8C0 *)&unit->hp);
    if ((unit->support[1] & SA_BIT(SA_GUARDIAN_MOG)) && mode == 0) {
        chr->status = 0;
        return;
    }
    if (unit->hp == 0) {
        btl_addStatus(unit, BTL_STATUS_KO);
    }
    btl_saveCharStatus((Unk800C09D8 *)chr, (Unk800C09D8 *)unit);
}

void btl_unlinkUnit(BtlUnit *unit) {
    BtlUnit *prev;
    for (prev = (BtlUnit *)&g_gameState->gameData->ctx.battle->unitList; prev != NULL; prev = prev->next) {
        if (prev->next == unit) {
            prev->next = unit->next;
            return;
        }
    }
}

void btl_insertUnit(BtlUnit *unit) {
    BtlUnit **pp = &g_gameState->gameData->ctx.battle->unitList;
    BtlUnit *cur;
    s32 slot;
    s32 curSlot;
    while (pp != NULL) {
        cur = *pp;
        slot = (u8)unit->flags >> BTL_UNIT_SLOT_SHIFT;
        curSlot = (u8)cur->flags >> BTL_UNIT_SLOT_SHIFT;
        if (slot < curSlot) {
            unit->next = cur;
            *pp = unit;
            return;
        }
        pp = &cur->next;
    }
}

void btl_tryEscape(u8 flee) {
    Battle *battle = g_gameState->gameData->ctx.battle;
    if (!(battle->cmdFlags & BTL_CMDF_ESCAPE)) {
        if ((u8)battle->scene.flags >> BTL_SCENE_CAN_ESCAPE_BIT == 0) {
            btl_showMessage(BTL_MSG_CANT_ESCAPE);
        } else if (flee && (btl_isAtbRunning() & 0xFF)) {
            btl_calcCmd(0, 0, 0, BTL_FORMULA_FLEE);
        }
    }
}

void btl_setUnitBrightness(BtlUnit *unit, u32 brightness) {
    s32 i;
    BtlBgHeader *bg;
    s32 shadow;
    u32 flags;
    if (brightness < 0x20) {
        i = 0;
        bg = btlbg_getHeader();
        btl_setModelAbr(unit->model, TPAGE_ABR(3));
        for (; i < unit->model->header->meshCount; i++) {
            btl_setMeshRgb(unit->model, i, (s32)((bg->lightR * brightness) << 11) >> 16, (s32)((bg->lightG * brightness) << 11) >> 16, (s32)((bg->lightB * brightness) << 11) >> 16);
        }
        if (unit->weaponModel != NULL) {
            btl_setModelAbr(unit->weaponModel, TPAGE_ABR(3));
            for (i = 0; i < unit->weaponModel->header->meshCount; i++) {
                btl_setMeshRgb(unit->weaponModel, i, (s32)((bg->lightR * brightness) << 11) >> 16, (s32)((bg->lightG * brightness) << 11) >> 16, (s32)((bg->lightB * brightness) << 11) >> 16);
            }
        }
        flags = unit->flags;
        shadow = flags >> BTL_UNIT_INDEX_SHIFT;
        shadow &= 0xF;
        if (!(flags & BTL_UNIT_PARTY)) {
            shadow += 9;
        }
        g_gameState->gameData->ctx.battle->work->shadows[shadow].shade = (s32)(btlbg_getHeader()->shadowShade * brightness) >> 5;
    }
}

u16 btl_getUnitsWithStatus(u32 side, s32 statuses) {
    BtlUnit *unit;
    s32 bits = 0;
    s32 all;
    for (unit = g_gameState->gameData->ctx.battle->unitList; unit != 0; unit = unit->next) {
        all = statuses == 0;
        if (all || (u8)btl_hasStatus(unit, statuses)) {
            if ((unit->flags >> BTL_UNIT_TARGETABLE_BIT) & 1) {
                switch (side) {
                case 0:
                    if (!(unit->flags & BTL_UNIT_PARTY)) {
                        break;
                    }
                    bits |= unit->unitBit;
                    break;
                case 1:
                    if (unit->flags & BTL_UNIT_PARTY) {
                        break;
                    }
                case 2:
                    bits |= unit->unitBit;
                    break;
                }
            }
        }
    }
    return bits;
}

BtlUnit *btl_getUnitBySlot(s32 slot) {
    BtlUnit *unit;
    for (unit = g_gameState->gameData->ctx.battle->unitList; unit != NULL; unit = unit->next) {
        if ((u8)unit->flags >> BTL_UNIT_SLOT_SHIFT == slot) {
            return unit;
        }
    }
    return NULL;
}

BtlUnit *btl_findBodyUnit(void) {
    BtlUnit *unit;
    for (unit = g_gameState->gameData->ctx.battle->unitList; unit != NULL; unit = unit->next) {
        if (!(unit->flags & (BTL_UNIT_PARTY | BTL_UNIT_MODEL_PART)) && (((g_gameState->gameData->ctx.battle->enemies + BTL_UNIT_INDEX(unit->flags))->flags >> BTL_ENEMYF_SHARED_MODEL_BIT) & 1)) {
            return unit;
        }
    }
    return NULL;
}

s32 btl_hasEnemyFlag(BtlUnit *unit, s32 category) {
    u32 flags = unit->flags;
    if (flags & BTL_UNIT_PARTY) {
        return 0;
    }
    return g_gameState->gameData->ctx.battle->enemies[BTL_UNIT_INDEX(flags)].type->category & category;
}

s32 btl_getModelSlot(BtlUnit *unit) {
    u32 flags = unit->flags;
    s32 slot = BTL_UNIT_INDEX(flags);
    if (!(flags & BTL_UNIT_PARTY)) {
        slot += 9;
    }
    return slot;
}

u32 btl_countActiveUnits(u32 isParty) {
    BtlUnit *unit = g_gameState->gameData->ctx.battle->unitList;
    s32 count = 0;
    while (unit != NULL) {
        u32 flags = unit->flags;
        if ((flags & BTL_UNIT_PARTY) == isParty && ((flags >> BTL_UNIT_TARGETABLE_BIT) & 1) && !(unit->status & BTL_STATUS_KO)) {
            count++;
        }
        unit = unit->next;
    }
    return count;
}

s32 btl_pickRandomUnit(s32 isParty) {
    u16 ids[4];
    u16 count = 0;
    BtlUnit *unit;
    if (isParty != 0) {
        isParty = 1;
    }
    for (unit = g_gameState->gameData->ctx.battle->unitList; unit != NULL; unit = unit->next) {
        if ((unit->flags & BTL_UNIT_PARTY) == isParty && !(unit->status & BTL_STATUS_KO) && ((unit->flags >> BTL_UNIT_TARGETABLE_BIT) & 1)) {
            ids[count++] = unit->unitBit;
        }
    }
    if (count == 0) {
        return 0;
    }
    /* rand8_inline() written out: calling it changes the register allocation here */
    if (g_randPtr >= g_randTableEnd) {
        rand_refill();
    }
    return ids[*g_randPtr++ % count];
}

s32 btl_playVictoryMusic(GameData *gd, u32 fadeFrames, s32 musicId) {
    Battle *battle;
    if (!(gd->battleFlags & GD_BTL_VICTORY_MUSIC_FADE)) {
        btl_soundCmd(SND_MUSIC_VOL_INTPL, 0, fadeFrames, 0, 0);
        gd->battleFlags |= GD_BTL_VICTORY_MUSIC_FADE;
    }
    if (!(gd->battleFlags & GD_BTL_VICTORY_MUSIC_LOADED)) {
        battle = g_gameState->gameData->ctx.battle;
        battle->partyBrightness += 4;
        if ((u32)(s8)battle->partyBrightness < fadeFrames) {
            return 0;
        }
        btl_soundCmd(SND_MUSIC_LOAD, musicId, 0, 0, 0);
        gd->battleFlags |= GD_BTL_VICTORY_MUSIC_LOADED;
    }
    if (btl_soundCmd(SND_GET_STATE, 0, 0, 0, 0) != 0) {
        return 0;
    }
    if (!(gd->battleFlags & GD_BTL_VICTORY_MUSIC_PLAYING)) {
        btl_soundCmd(SND_MUSIC_PLAY, musicId, 0, 0, 0);
        gd->battleFlags |= GD_BTL_VICTORY_MUSIC_PLAYING;
    }
    return 1;
}

void btl_playHitSound(BtlUnit *caster, BtlUnit *target) {
    if (!(target->popupFlags & BTL_POPUP_MISS)) {
        btl_playUnitSound(caster, (u16)btl_getWeaponSound((u8)caster->flags >> BTL_UNIT_SLOT_SHIFT, 1), SND_VOL_MAX);
    }
}

void btl_playDeathCry(BtlUnit *unit, u16 soundId) {
    Battle *battle = g_gameState->gameData->ctx.battle;
    if (battle->deathCryWait == 0 && !((unit->flags >> BTL_UNIT_DEATH_CRY_BIT) & 1)) {
        battle->deathCryWait = 8;
        btl_playUnitSound(unit, soundId, SND_VOL_MAX);
        unit->flags |= BTL_UNIT_DEATH_CRY;
    }
}

void btl_playUnitSound(BtlUnit *unit, u16 soundId, u8 volume) {
    SVECTOR pos;
    if (soundId != 0xFFFF) {
        pos.vx = unit->pos[0] >> 16;
        pos.vy = unit->pos[1] >> 16;
        pos.vz = unit->pos[2] >> 16;
        btl_soundCmd(SND_SE_PLAY, soundId, 0, btl_soundPan((s32)&pos), volume);
    }
}

void btl_applyBgLight(BtlModel *model) {
    s32 i;
    BtlBgHeader *bg = btlbg_getHeader();
    for (i = 0; i < model->header->meshCount; i++) {
        btl_setMeshRgb(model, i & 0xFF, bg->lightR, bg->lightG, bg->lightB);
    }
}

