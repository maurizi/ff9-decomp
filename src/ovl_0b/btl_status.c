#include "ovl_0b.h"

u16 btl_getUnitMask(u32 side) {
    BtlUnit *u;
    s32 bits;

    bits = 0;
    for (u = g_gameState->gameData->ctx.battle->unitList; u != NULL; u = u->next) {
        switch (side) {
        case 0:
            if (u->flags & BTL_UNIT_PARTY) {
                bits |= u->unitBit;
            }
            break;
        case 1:
            if (!(u->flags & BTL_UNIT_PARTY)) {
                bits |= u->unitBit;
            }
            break;
        case 2:
            bits |= u->unitBit;
            break;
        }
    }
    return bits;
}

BtlUnit *btl_getUnitById(u16 unitBit) {
    BtlUnit *u;
    for (u = g_gameState->gameData->ctx.battle->unitList; u != NULL; u = u->next) {
        if (u->unitBit == unitBit) {
            return u;
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

u32 btl_addStatus(BtlUnit *u, u32 f) {
    u32 n = 0;
    BtlStatusInfo *tbl = g_gameState->gameData->ctx.battle->statusInfo;
    BtlUnit34 *s = (BtlUnit34 *)&u->statusImmune;
    u32 i;
    u32 blk;
    s16 k;

    if (u->statusImmune & f) {
        return 0;
    }
    if (s->autoStatus & f) {
        return 1;
    }
    if ((s->status & f) && (f & (BTL_STATUS_PETRIFY | BTL_STATUS_VENOM | BTL_STATUS_VIRUS | BTL_STATUS_SILENCE | BTL_STATUS_BLIND | BTL_STATUS_TROUBLE | BTL_STATUS_ZOMBIE | BTL_STATUS_KO | BTL_STATUS_LOW_HP | BTL_STATUS_CONFUSE | BTL_STATUS_BERSERK | BTL_STATUS_STOP | BTL_STATUS_AUTO_LIFE | BTL_STATUS_TRANCE | BTL_STATUS_DEFEND | BTL_STATUS_DOOM | BTL_STATUS_GRADUAL_PETRIFY))) {
        return 1;
    }
    i = 0;
    blk = 0;
    for (; i < 32; i++) {
        if (f & (1 << i)) {
            n = i;
        }
        if (s->status & (1 << i)) {
            blk |= tbl[i].prevents;
        }
    }
    if (btl_isCmdQueued(u, BTL_CMD_PETRIFY)) {
        blk |= tbl[BTL_STATUS_PETRIFY_BIT].prevents;
    }
    if (blk & f) {
        return 1;
    }
    if (!(f & (BTL_STATUS_PETRIFY | BTL_STATUS_HASTE | BTL_STATUS_SLOW | BTL_STATUS_MINI))) {
        if ((f & (BTL_STATUS_PETRIFY | BTL_STATUS_VENOM | BTL_STATUS_KO | BTL_STATUS_CONFUSE | BTL_STATUS_BERSERK | BTL_STATUS_SLEEP)) && (u8)btl_cancelUnitCmds(u)) {
            u->atb = 0;
            u->flags &= ~BTL_UNIT_ATB_ACTIVE;
            if (!(f & BTL_STATUS_KO)) {
                u->cmdIssued = 0;
            }
        }
        s->status |= f;
    }
    switch (n) {
    case BTL_STATUS_PETRIFY_BIT:
        if (!btl_isCmdPending(u->koCmd)) {
            if (g_gameState->gameData->ctx.battle->phase >= BTL_PHASE_ENTRANCE) {
                btl_setCmd(u->koCmd, BTL_CMD_PETRIFY, 0, u->unitBit, 0);
            } else {
                if (u->flags & BTL_UNIT_PARTY) {
                    btl_makeGreyCluts(u);
                }
                s->status |= f;
                u->flags &= ~BTL_UNIT_ATB_ACTIVE;
                btl_setStoneClut(u, 1);
            }
        }
        break;
    case BTL_STATUS_VENOM_BIT:
        if (g_gameState->gameData->ctx.battle->phase >= BTL_PHASE_ENTRANCE) {
            btl_checkSideDefeated(u);
        }
        /* Falls through into Sleep's motion switch, as in the original. At battle start (btl_initPartyUnit ->
         * btl_addStatuses, a party member carrying Venom over from the last battle) u->model and curCmd are still
         * NULL, so the PS1 reads (and may write) low RAM through them here. Harmless on the PS1; a native port must
         * guard both pointers (ff9-ds F36: battles d2_b0779, d4_b0280/0779/0828/0940). */
    case BTL_STATUS_SLEEP_BIT:
        if ((u->flags & BTL_UNIT_PARTY) && (s32)u->model->motion != u->motions[BTL_MOTION_IDLE_WEAK] &&
            g_gameState->gameData->ctx.battle->curCmd->caster != u) {
            u->model->motion = (BtlMotion *)u->motions[BTL_MOTION_IDLE_WEAK];
            u->obj->frame = 0;
        }
        break;
    case BTL_STATUS_ZOMBIE_BIT:
        if ((u->flags & BTL_UNIT_PARTY) && !(u->status & BTL_STATUS_TRANCE)) {
            u->trance = 0;
        }
    case BTL_STATUS_BERSERK_BIT:
    case BTL_STATUS_HEAT_BIT:
    case BTL_STATUS_FREEZE_BIT:
        btl_updateStatusColor(u);
        break;
    case BTL_STATUS_KO_BIT:
        if (u->hp != 0) {
            u->popupFlags |= BTL_POPUP_DEATH;
            btl_killUnit(u);
        } else {
            u->hp = 0;
        }
        u->atb = 0;
        if (!btl_isCmdPending(u->koCmd)) {
            btl_setCmd(u->koCmd, BTL_CMD_KO, 0, u->unitBit, 0);
            if (!(u->flags & BTL_UNIT_PARTY)) {
                btl_checkEnemiesKO(u);
            }
        }
        break;
    case BTL_STATUS_STOP_BIT:
        btl_checkSideDefeated(u);
        break;
    case BTL_STATUS_TRANCE_BIT:
        btl_setCmd(u->tranceCmd, BTL_CMD_TRANCE, 0, u->unitBit, 0);
        break;
    case BTL_STATUS_HASTE_BIT:
        if (u->status & BTL_STATUS_SLOW) {
            btl_removeStatus(u, BTL_STATUS_SLOW);
            return 2;
        }
        btl_resetAtbSpeed((Unk800BD8F0 *)u);
        u->atbSpeed = u->atbSpeed * 3 / 2;
        s->status |= f;
        break;
    case BTL_STATUS_SLOW_BIT:
        if (u->status & BTL_STATUS_HASTE) {
            btl_removeStatus(u, BTL_STATUS_HASTE);
            return 2;
        }
        btl_resetAtbSpeed((Unk800BD8F0 *)u);
        u->atbSpeed = u->atbSpeed * 2 / 3;
        s->status |= f;
        break;
    case BTL_STATUS_VANISH_BIT:
        btl_hideMeshes(u, u->vanishMeshes);
        break;
    case BTL_STATUS_MINI_BIT:
        s->status ^= f;
        if (u->status & BTL_STATUS_MINI) {
            u->model->flags |= BTL_MODEL_SCALED;
            u->model->scaleX = 0x800;
            u->model->scaleY = 0x800;
            u->model->scaleZ = 0x800;
            g_gameState->gameData->ctx.battle->work->shadows[btl_getModelSlot(u)].width = (u->shadowW >> 1) * 14;
            g_gameState->gameData->ctx.battle->work->shadows[btl_getModelSlot(u)].depth = (u->shadowD >> 1) * 12;
        } else {
            u->model->flags &= ~BTL_MODEL_SCALED;
            g_gameState->gameData->ctx.battle->work->shadows[btl_getModelSlot(u)].width = u->shadowW * 14;
            g_gameState->gameData->ctx.battle->work->shadows[btl_getModelSlot(u)].depth = u->shadowD * 12;
        }
        break;
    }
    btl_removeStatuses(u, tbl[n].removes);
    if (u->status & (BTL_STATUS_PETRIFY | BTL_STATUS_KO | BTL_STATUS_STOP | BTL_STATUS_JUMP)) {
        u->flags &= ~BTL_UNIT_ATB_ACTIVE;
    }
    if (f & BTL_STATUS_TIMED) {
        if (f & (BTL_STATUS_POISON | BTL_STATUS_SLEEP | BTL_STATUS_SLOW | BTL_STATUS_HEAT | BTL_STATUS_FREEZE | BTL_STATUS_DOOM | BTL_STATUS_MINI | BTL_STATUS_GRADUAL_PETRIFY)) {
            k = (0x3C - u->spirit) * 8;
        } else if (f & (BTL_STATUS_REGEN | BTL_STATUS_HASTE | BTL_STATUS_FLOAT | BTL_STATUS_SHELL | BTL_STATUS_PROTECT | BTL_STATUS_VANISH | BTL_STATUS_REFLECT)) {
            k = u->spirit * 8;
        } else {
            k = (0x3C - u->spirit) * 4;
        }
        u->statusTimers[n - BTL_STATUS_TIMED_FIRST_BIT] = tbl[n].timerScale * k;
    }
    if (f & (BTL_STATUS_VENOM | BTL_STATUS_POISON | BTL_STATUS_REGEN)) {
        btl_initDotTimer(u, n);
    }
    return 2;
}

s32 btl_removeStatus(BtlUnit *u, u32 f) {
    BtlUnit34 *s = (BtlUnit34 *)&u->statusImmune;
    BtlModel *m;
    s32 *a4;
    BtlCmd *c;

    if (s->autoStatus & f) {
        return 1;
    }
    if (!(s->status & f)) {
        return 1;
    }
    if (!(u->flags & BTL_UNIT_PARTY) && g_gameState->gameData->ctx.battle->phase == BTL_PHASE_END && (f & (BTL_STATUS_PETRIFY | BTL_STATUS_VENOM | BTL_STATUS_STOP))) {
        return 1;
    }
    s->status &= ~f;
    switch (f) {
    case BTL_STATUS_PETRIFY:
        btl_setStoneClut(u, 0);
        break;
    case BTL_STATUS_BERSERK:
        if ((u8)btl_cancelUnitCmds(u)) {
            u->atb = 0;
            u->cmdIssued = 0;
            u->flags &= ~BTL_UNIT_ATB_ACTIVE;
        }
    case BTL_STATUS_ZOMBIE:
    case BTL_STATUS_HEAT:
    case BTL_STATUS_FREEZE:
        if (u->status & (BTL_STATUS_ZOMBIE | BTL_STATUS_BERSERK | BTL_STATUS_HEAT | BTL_STATUS_FREEZE)) {
            btl_updateStatusColor(u);
        }
        break;
    case BTL_STATUS_KO:
        m = u->model;
        a4 = u->motions;
        u->koState = BTL_KO_NONE;
        u->escapeStance = 0;
        u->flags &= ~BTL_UNIT_HIT_MOTION;
        u->flags &= ~BTL_UNIT_CMD_WAITING;
        u->flags &= ~BTL_UNIT_KILLED;
        u->flags &= ~BTL_UNIT_MOTION_HELD;
        if ((s32)m->motion == a4[BTL_MOTION_KO] || (s32)m->motion == a4[8]) {
            m->texAnims[2].flags |= BTL_TEXANIM_PLAY;
            u->model->texAnims[2].pos = 0;
            u->model->texAnims[2].lastFrame = 0x1000;
            u->model->motion = (BtlMotion *)u->motions[BTL_MOTION_GET_UP];
            u->obj->frame = 0;
        }
        c = g_gameState->gameData->ctx.battle->curCmd;
        if (c->caster != u || c->cmdNo > BTL_CMD_TABLE_COUNT) {
            u->cmdIssued = 0;
        }
        btl_removeQueuedCmd(u, BTL_CMD_KO);
        if (u->autoStatus & BTL_STATUS_REGEN) {
            btl_initDotTimer(u, BTL_STATUS_REGEN_BIT);
        }
        break;
    case BTL_STATUS_CONFUSE:
        u->rot[1] = u->obj->rotY;
        if ((u8)btl_cancelUnitCmds(u)) {
            u->atb = 0;
            u->cmdIssued = 0;
            u->flags &= ~BTL_UNIT_ATB_ACTIVE;
        }
        break;
    case BTL_STATUS_TRANCE:
        u->trance = 0;
        btl_cancelTranceEidolon(u);
        if (u->status & BTL_STATUS_JUMP) {
            btl_removeStatus(u, BTL_STATUS_JUMP);
            u->flags &= ~BTL_UNIT_HIDDEN;
            u->pos[0] = u->home->pos[0];
            u->pos[1] = u->home->pos[1];
            u->pos[2] = u->home->pos[2];
            u->model->motion = (BtlMotion *)u->motions[(u->flags >> BTL_UNIT_WEAK_STANCE_BIT) & 1];
            u->obj->frame = 0;
        }
        btl_setCmd(u->tranceCmd, BTL_CMD_TRANCE, 0, u->unitBit, 0);
        break;
    case BTL_STATUS_HASTE:
    case BTL_STATUS_SLOW:
        btl_resetAtbSpeed((Unk800BD8F0 *)u);
        break;
    case BTL_STATUS_FLOAT:
        u->pos[1] = 0;
        u->home->pos[1] = 0;
        break;
    case BTL_STATUS_VANISH:
        btl_showMeshes(u, u->vanishMeshes);
        break;
    case BTL_STATUS_MINI:
        u->model->flags &= ~BTL_MODEL_SCALED;
        g_gameState->gameData->ctx.battle->work->shadows[btl_getModelSlot(u)].width = u->shadowW * 14;
        g_gameState->gameData->ctx.battle->work->shadows[btl_getModelSlot(u)].depth = u->shadowD * 12;
        break;
    case BTL_STATUS_JUMP:
        u->targetableReq = BTL_TGTREQ_SET_ON;
        u->atb = 0;
        u->cmdIssued = 0;
        u->flags |= BTL_UNIT_ATB_ACTIVE;
        u->delayedCmd->cmdNo = 0;
        u->delayedCmd->targets = 0;
        break;
    case BTL_STATUS_GRADUAL_PETRIFY:
        btl_removeQueuedCmd(u, BTL_CMD_PETRIFY);
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
    if ((unit->status & BTL_STATUS_CONFUSE) && unit != g_gameState->gameData->ctx.battle->curCmd->caster) {
        if (unit->model->motion == (BtlMotion *)unit->motions[BTL_MOTION_IDLE] || unit->model->motion == (BtlMotion *)unit->motions[BTL_MOTION_IDLE_WEAK] ||
            ((unit->flags & BTL_UNIT_PARTY) && unit->model->motion == (BtlMotion *)unit->motions[BTL_MOTION_READY])) {
            unit->rot[1] += 0x80;
        }
    }
}

void btl_updateUnitTint(BtlUnit *unit) {
    BtlBgHeader *bg;
    u8 *p;
    s32 t;
    u32 n;
    u32 m;
    u8 v;
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
                n = g_gameState->gameData->ctx.battle->tick;
                m = n;
                m = m % 24;
                /* v is the colour-select flag here and the fade level below; the table branch's level goes
                   through w (a u32), so its loads stay word-sized. */
                if ((unit->status & (BTL_STATUS_SHELL | BTL_STATUS_PROTECT)) == (BTL_STATUS_SHELL | BTL_STATUS_PROTECT)) {
                    v = (n % 48) >= 24;
                } else {
                    v = (unit->status & BTL_STATUS_PROTECT) < BTL_STATUS_PROTECT;
                }
                if (v) {
                    r = bg->lightR - 0x40;
                    g = bg->lightG + 0x18;
                    b = bg->lightB + 0x48;
                } else {
                    r = bg->lightR + 0x28;
                    g = bg->lightG + 0x28;
                    b = bg->lightB - 0x50;
                }
                if ((u8)m < 8) {
                    v = m;
                } else if ((u8)m < 16) {
                    v = 8;
                } else {
                    v = 24 - m;
                }
            } else if ((unit->status & (BTL_STATUS_KO | BTL_STATUS_TRANCE)) == BTL_STATUS_TRANCE) {
                u8 tbl[8][3] = {
                    { 0xFF, 0x60, 0x60 }, { 0x68, 0x78, 0xFF }, { 0xFF, 0xB8, 0x78 }, { 0x80, 0xC0, 0xD0 },
                    { 0xC0, 0x68, 0x90 }, { 0xD0, 0x70, 0x78 }, { 0xC8, 0xD8, 0x68 }, { 0xD0, 0xB8, 0x68 },
                };
                p = tbl[BTL_UNIT_INDEX(unit->flags)];
                r = bg->lightR + (p[0] - 0x80);
                g = bg->lightG + (p[1] - 0x80);
                b = bg->lightB + (p[2] - 0x80);
                w = g_gameState->gameData->ctx.battle->tick & 0xF;
                if (w >= 8) {
                    w = 16 - w;
                }
                v = w;
            } else {
                goto end;
            }
            rr = (r * v) << 13 >> 16;
            gg = (g * v) << 13 >> 16;
            bb = (b * v) << 13 >> 16;
            btl_setModelRgb(unit->model, rr, gg, bb);
            if (unit->weaponModel != NULL) {
                btl_setModelRgb(unit->weaponModel, rr, gg, bb);
            }
        }
    }
end:
    n = BtlUnitHasStatus(unit, BTL_STATUS_FLOAT);
    if ((u8)n) {
        unit->pos[1] = t = 0xFF380000 - math_sin((g_gameState->gameData->ctx.battle->tick & 0xF) << 8) * 480;
        unit->home->pos[1] = t;
    }
}

void btl_updateStatusColor(BtlUnit *unit) {
    s16 shift[3];
    s16 light[3];
    BtlBgHeader *bg = btlbg_getHeader();
    s32 i;
    s32 v;
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
        v = light[i] - shift[i];
        if (v < 0) {
            unit->tint[i] = 0;
        } else if (v >= 0x100) {
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
        unit->weaponModel->texPages->clut = ((delta + clut_getY(unit->weaponModel->texPages->clut)) << 6) | 0x10;
    }
    while (i < db_getModelTexCount(unit->modelId | DB_MODEL)) {
        unit->model->texPages[i].clut = ((delta + clut_getY(unit->model->texPages[i].clut)) << 6) | 0x10;
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
    s8 c[3];
    s8 v;
    for (i = 0; i < 0x600; i++) {
        c[0] = *p & 0x1F;
        c[1] = (*p >> 5) & 0x1F;
        c[2] = (*p >> 10) & 0x1F;
        if (c[0] == 0 && c[1] == 0 && c[2] == 0) {
            v = 0;
        } else {
            v = (c[0] + c[1] + c[2]) / 3;
            if (v <= 0) {
                v = 1;
            }
        }
        *p = v + ((v << 10) + (v << 5));
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
    s8 c[3];
    s8 v;
    u16 *p;
    g_btlClutRect.x = 0x100;
    g_btlClutRect.y = 0xF2;
    g_btlClutRect.w = 0x100;
    g_btlClutRect.h = 1;
    for (i = 2; i < 0xC; i += 3) {
        j = 0;
        p = (u16 *)g_btlSavedCluts;
        for (; j < 0x100; j++) {
            c[0] = p[(i << 8) + j] & 0x1F;
            c[1] = (p[(i << 8) + j] >> 5) & 0x1F;
            c[2] = (p[(i << 8) + j] >> 10) & 0x1F;
            if (c[0] == 0 && c[1] == 0 && c[2] == 0) {
                v = 0;
            } else {
                v = (c[0] + c[1] + c[2]) / 3;
                if (v <= 0) {
                    v = 1;
                }
            }
            p[(i << 8) + j] = v + ((v << 10) + (v << 5));
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
    chr->chrStatus = unit->status & 0x7F;
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

void btl_interruptUnit(BtlUnit *unit, s32 flags) {
    if (btl_cancelUnitCmds(unit) & 0xFF) {
        unit->atb = 0;
        unit->flags &= ~BTL_UNIT_ATB_ACTIVE;
        if (!(flags & 0x100)) {
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

void btl_initDotTimer(BtlUnit *u, s32 n) {
    u8 k;
    s32 v;

    if (n == BTL_STATUS_VENOM_BIT) {
        v = u->spirit * 4;
        k = 0;
    } else if (n == BTL_STATUS_POISON_BIT) {
        v = u->spirit * 4;
        k = 1;
    } else {
        v = (0x3C - u->spirit) * 4;
        k = 2;
    }
    u->dotTimers[k] = g_gameState->gameData->ctx.battle->statusInfo[n].tickScale * v;
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
    BtlUnit *u;
    u32 flags;
    s32 kind;
    s32 n;

    n = BTL_KO_DONE;
    for (u = battle->unitList; u != NULL; u = u->next) {
        if ((u->flags & BTL_UNIT_PARTY) == (unit->flags & BTL_UNIT_PARTY)) {
            if (!(u->status & (BTL_STATUS_PETRIFY | BTL_STATUS_VENOM | BTL_STATUS_KO | BTL_STATUS_STOP)) || ((u->status & BTL_STATUS_KO) && u->koState != n)) {
                return;
            }
        }
    }
    if (!(unit->flags & BTL_UNIT_PARTY)) {
        found = 0;
        for (u = battle->unitList; u != NULL; u = u->next) {
            if (u->flags & BTL_UNIT_PARTY) {
                if (!(u->status & (BTL_STATUS_PETRIFY | BTL_STATUS_VENOM | BTL_STATUS_KO | BTL_STATUS_STOP)) || ((u->hp == 0 || (u->status & BTL_STATUS_KO)) && (u->status & BTL_STATUS_AUTO_LIFE)) || btl_isCmdQueued(u, BTL_CMD_AUTO_LIFE)) {
                    found = 1;
                    break;
                }
            }
        }
        if (!found) {
            return;
        }
        if (battle->subPhase != BTL_END_DEFEAT) {
            battle->subPhase = BTL_END_VICTORY;
        }
    } else {
        for (u = battle->unitList; u != NULL; u = u->next) {
            flags = u->flags;
            if (flags & BTL_UNIT_PARTY) {
                kind = g_gameState->gameData->characters[BTL_UNIT_INDEX(flags)].info.b.kind;
            } else {
                kind = CHR_KIND_NONE;
            }
            if ((u32)(kind - CHR_KIND_EIKO) < 2) {
                if (u->status & (BTL_STATUS_PETRIFY | BTL_STATUS_VENOM | BTL_STATUS_ZOMBIE | BTL_STATUS_STOP)) {
                    break;
                }
                if (btl_isCmdQueued(u, BTL_CMD_REBIRTH_FLAME)) {
                    return;
                }
                if (g_randPtr >= g_randTableEnd) {
                    rand_refill();
                }
                kind = item_getCount(ITEM_PHOENIX_PINION);
                if (*g_randPtr++ < kind) {
                    btlui_setAtbEnabled(1);
                    btl_setCmd(u->cmd, BTL_CMD_REBIRTH_FLAME, ABIL_REBIRTH_FLAME, btl_getUnitMask(0) & 0xFFFF, 1);
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
    s32 result = 2;
    s32 threshold;
    if ((*sceneFlags >> 2) & 1) {
        result = (((*sceneFlags >> 3) & 1) == 0) * 2;
    } else {
        threshold = 0x18;
        if ((u8)btl_partyHasSupport(1, SA_BIT(SA_ALERT))) {
            threshold = 0;
        } else if ((*sceneFlags >> 3) & 1) {
            threshold = 0xFF;
        }
        if (g_randPtr >= g_randTableEnd) {
            rand_refill();
        }
        if (*g_randPtr++ < threshold) {
            result = 0;
        } else {
            threshold = 0x10;
            if ((u8)btl_partyHasSupport(1, SA_BIT(SA_INITIATIVE))) {
                threshold = 0x55;
            }
            if (g_randPtr >= g_randTableEnd) {
                rand_refill();
            }
            if (*g_randPtr++ < threshold) {
                result = 1;
            }
        }
    }
    return result;
}

void btl_addEnemyRewards(BtlEnemyType *enemyType) {
    u8 n;
    u8 itemId;
    Unk800FDF30 *rewards;

    n = 0;
    rewards = &g_btlRewards;
    rewards->gil += enemyType->gil;
    rewards->exp += enemyType->exp;
    for (; n < 16; n++) {
        if (rewards->items[n] == ITEM_NONE) {
            break;
        }
    }
    if (n < 16) {
        if (rand8_inline() < 1 && (itemId = enemyType->drops.b[3]) != ITEM_NONE) {
            rewards->items[n++] = itemId;
            enemyType->drops.b[3] = ITEM_NONE;
        } else if (rand8_inline() < 0x20 && (itemId = enemyType->drops.b[2]) != ITEM_NONE) {
            rewards->items[n++] = itemId;
            enemyType->drops.b[2] = ITEM_NONE;
        } else if (rand8_inline() < 0x60 && (itemId = enemyType->drops.b[1]) != ITEM_NONE) {
            rewards->items[n++] = itemId;
            enemyType->drops.b[1] = ITEM_NONE;
        }
        if ((itemId = enemyType->drops.b[0]) != ITEM_NONE) {
            rewards->items[n] = itemId;
        }
    }
    if (rewards->card == CARD_NONE && (u32)enemyType->card < 100) {
        if (rand8_inline() < 0x20) {
            rewards->card = enemyType->card;
        }
    }
}

void btl_initState(void) {
    Battle *b = g_gameState->gameData->ctx.battle;
    b->unitList = NULL;
    b->tick = 0;
    b->phase = 0;
    b->subPhase = 0;
    b->exitTimer = 0;
    b->escapeHeld = 0;
    b->fleeBrightness = 0x20;
    b->loadFlags = 0;
    b->partyBrightness = 0;
    b->enemyBrightness = 0;
    b->eidolonAbility = 0;
    b->eidolonTimer = 0;
    b->deathCryWait = 0;
    g_btlRewards.expMask = 0;
    btl_clearRewards();
}

void btl_clearRewards(void) {
    s32 i;
    u8 *p;
    s32 c;

    c = 0xFF;
    i = 15;
    p = (u8 *)&g_btlRewards + i;
    g_btlRewards.gil = 0;
    g_btlRewards.exp = 0;
    g_btlRewards.ap = 0;
    for (; i >= 0; i--) {
        p[12] = c; /* unkC[i] */
        p--;
    }
    g_btlRewards.card = CARD_NONE;
    g_btlRewards.fleeGil = 0;
}

void btl_checkPartyDisabled(BtlUnit *unit) {
    Battle *battle = g_gameState->gameData->ctx.battle;
    BtlUnit *u;
    BtlUnit *six = 0;
    u32 status;
    for (u = battle->unitList; u != 0; u = u->next) {
        if (BTL_UNIT_INDEX(u->flags) == 6) {
            six = u;
        }
        if ((u->flags & BTL_UNIT_PARTY) == (unit->flags & BTL_UNIT_PARTY)) {
            status = u->status;
            if (!(status & (BTL_STATUS_PETRIFY | BTL_STATUS_VENOM | BTL_STATUS_KO | BTL_STATUS_STOP))) {
                return;
            }
            if ((u->hp == 0 || (status & BTL_STATUS_KO)) && (status & BTL_STATUS_AUTO_LIFE)) {
                return;
            }
            if (btl_isCmdQueued(u, BTL_CMD_AUTO_LIFE)) {
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
    Battle *b = g_gameState->gameData->ctx.battle;
    BtlUnit *u;
    for (u = b->unitList; u != NULL; u = u->next) {
        if (!(u->flags & BTL_UNIT_PARTY) && !(u->status & BTL_STATUS_KO)) {
            return;
        }
    }
    btlui_setAtbEnabled(0);
    btl_flushCmdQueue(b);
    b->escapeHeld = 0;
}

s32 btl_saveParty(Battle *battle) {
    GameData *gd = g_gameState->gameData;
    u8 result = gd->battleResult;
    if ((result == BATTLE_END_DEFEAT || result == BATTLE_END_GAME_OVER) && !((battle->scene.flags >> BTL_SCENE_NO_GAMEOVER_BIT) & 1)) {
        gd->battleResult = BATTLE_END_GAME_OVER;
    } else {
        BtlUnit *u = g_gameState->gameData->ctx.battle->unitList;
        while (u != NULL) {
            if (u->flags & BTL_UNIT_PARTY) {
                btl_saveCharacter(u, 0);
            }
            u = u->next;
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
        btl_setModelAbr(unit->model, 0x60);
        for (; i < unit->model->header->meshCount; i++) {
            btl_setMeshRgb(unit->model, i, (s32)((bg->lightR * brightness) << 11) >> 16, (s32)((bg->lightG * brightness) << 11) >> 16, (s32)((bg->lightB * brightness) << 11) >> 16);
        }
        if (unit->weaponModel != NULL) {
            btl_setModelAbr(unit->weaponModel, 0x60);
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
    BtlUnit *u;
    s32 bits = 0;
    s32 all;
    for (u = g_gameState->gameData->ctx.battle->unitList; u != 0; u = u->next) {
        all = statuses == 0;
        if (all || (u8)btl_hasStatus(u, statuses)) {
            if ((u->flags >> BTL_UNIT_TARGETABLE_BIT) & 1) {
                switch (side) {
                case 0:
                    if (!(u->flags & BTL_UNIT_PARTY)) {
                        break;
                    }
                    bits |= u->unitBit;
                    break;
                case 1:
                    if (u->flags & BTL_UNIT_PARTY) {
                        break;
                    }
                case 2:
                    bits |= u->unitBit;
                    break;
                }
            }
        }
    }
    return bits;
}

BtlUnit *btl_getUnitBySlot(s32 slot) {
    BtlUnit *u;
    for (u = g_gameState->gameData->ctx.battle->unitList; u != NULL; u = u->next) {
        if ((u8)u->flags >> BTL_UNIT_SLOT_SHIFT == slot) {
            return u;
        }
    }
    return NULL;
}

BtlUnit *btl_findBodyUnit(void) {
    BtlUnit *u;
    for (u = g_gameState->gameData->ctx.battle->unitList; u != NULL; u = u->next) {
        if (!(u->flags & (BTL_UNIT_PARTY | BTL_UNIT_MODEL_PART)) && (((g_gameState->gameData->ctx.battle->enemies + BTL_UNIT_INDEX(u->flags))->flags >> BTL_ENEMYF_SHARED_MODEL_BIT) & 1)) {
            return u;
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
    BtlUnit *u = g_gameState->gameData->ctx.battle->unitList;
    s32 n = 0;
    while (u != NULL) {
        u32 f = u->flags;
        if ((f & BTL_UNIT_PARTY) == isParty && ((f >> BTL_UNIT_TARGETABLE_BIT) & 1) && !(u->status & BTL_STATUS_KO)) {
            n++;
        }
        u = u->next;
    }
    return n;
}

s32 btl_pickRandomUnit(s32 isParty) {
    u16 ids[4];
    u16 n = 0;
    BtlUnit *u;
    if (isParty != 0) {
        isParty = 1;
    }
    for (u = g_gameState->gameData->ctx.battle->unitList; u != NULL; u = u->next) {
        if ((u->flags & BTL_UNIT_PARTY) == isParty && !(u->status & BTL_STATUS_KO) && ((u->flags >> BTL_UNIT_TARGETABLE_BIT) & 1)) {
            ids[n++] = u->unitBit;
        }
    }
    if (n == 0) {
        return 0;
    }
    if (g_randPtr >= g_randTableEnd) {
        rand_refill();
    }
    return ids[*g_randPtr++ % n];
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
        btl_playUnitSound(caster, (u16)btl_getWeaponSound((u8)caster->flags >> BTL_UNIT_SLOT_SHIFT, 1), 0x7F);
    }
}

void btl_playDeathCry(BtlUnit *unit, u16 soundId) {
    Battle *battle = g_gameState->gameData->ctx.battle;
    if (battle->deathCryWait == 0 && !((unit->flags >> BTL_UNIT_DEATH_CRY_BIT) & 1)) {
        battle->deathCryWait = 8;
        btl_playUnitSound(unit, soundId, 0x7F);
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

