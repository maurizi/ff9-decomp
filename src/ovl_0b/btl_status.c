#include "ovl_0b.h"

u16 btl_getUnitMask(u32 arg0) {
    BtlUnit *u;
    s32 r;

    r = 0;
    for (u = g_gameState->gameData->ctx.battle->unitList; u != NULL; u = u->next) {
        switch (arg0) {
        case 0:
            if (u->flags & BTL_UNIT_PARTY) {
                r |= u->unitBit;
            }
            break;
        case 1:
            if (!(u->flags & BTL_UNIT_PARTY)) {
                r |= u->unitBit;
            }
            break;
        case 2:
            r |= u->unitBit;
            break;
        }
    }
    return r;
}

BtlUnit *btl_getUnitById(u16 arg0) {
    BtlUnit *u;
    for (u = g_gameState->gameData->ctx.battle->unitList; u != NULL; u = u->next) {
        if (u->unitBit == arg0) {
            return u;
        }
    }
    return NULL;
}

BtlUnit *btl_findUnitByBit(u16 arg0) {
    s32 i;
    Battle *b = g_gameState->gameData->ctx.battle;
    for (i = 0; i < 8; i++) {
        if (b->units[i].unitBit == arg0) {
            return &b->units[i];
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
        if ((u->flags & BTL_UNIT_PARTY) && (s32)u->model->motion != u->motions[1] &&
            g_gameState->gameData->ctx.battle->curCmd->caster != u) {
            u->model->motion = (BtlMotion *)u->motions[1];
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
        if ((s32)m->motion == a4[4] || (s32)m->motion == a4[8]) {
            m->texAnims[2].flags |= BTL_TEXANIM_PLAY;
            u->model->texAnims[2].pos = 0;
            u->model->texAnims[2].lastFrame = 0x1000;
            u->model->motion = (BtlMotion *)u->motions[6];
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

void btl_updateUnitStatus(BtlUnit *arg0) {
    BtlUnit34 *t = (BtlUnit34 *)&arg0->statusImmune;
    BtlCmd *c;
    Battle *b;
    u16 fl;

    btl_updateUnitTint(arg0);
    if (arg0->status & BTL_STATUS_KO) {
        btl_updateKo(arg0);
    } else if (!(arg0->status & BTL_STATUS_PETRIFY)) {
        if (!(arg0->status & (BTL_STATUS_PETRIFY | BTL_STATUS_KO | BTL_STATUS_STOP | BTL_STATUS_JUMP))) {
            arg0->flags |= BTL_UNIT_ATB_ACTIVE;
        }
        if (btl_isAtbRunning()) {
            if ((arg0->flags >> BTL_UNIT_ATB_ACTIVE_BIT) & 1) {
                if (arg0->status & BTL_STATUS_VENOM) {
                    if (t->venomTimer <= 0) {
                        btl_initDotTimer(arg0, BTL_STATUS_VENOM_BIT);
                        btl_tickPoison(arg0);
                        btl_tickVenomMp(arg0);
                        btl_spawnTickPopups(arg0);
                    } else {
                        t->venomTimer -= arg0->atbSpeed;
                    }
                }
                if (arg0->status & BTL_STATUS_POISON) {
                    if (t->poisonTimer <= 0) {
                        btl_initDotTimer(arg0, BTL_STATUS_POISON_BIT);
                        btl_tickPoison(arg0);
                        btl_spawnTickPopups(arg0);
                    } else {
                        t->poisonTimer -= arg0->atbSpeed;
                    }
                }
                if (BtlUnitHasStatus(arg0, BTL_STATUS_REGEN)) {
                    if (t->regenTimer <= 0) {
                        btl_initDotTimer(arg0, BTL_STATUS_REGEN_BIT);
                        btl_tickRegen(arg0);
                        btl_spawnTickPopups(arg0);
                    } else {
                        t->regenTimer -= arg0->atbSpeed;
                    }
                }
                if ((arg0->status & BTL_STATUS_TRANCE) && BTL_UNIT_INDEX(arg0->flags) == CHR_DAGGER) {
                    b = g_gameState->gameData->ctx.battle;
                    fl = b->cmdFlags;
                    if (fl & BTL_CMDF_EIDOLON_TIMER) {
                        if (!(fl & BTL_CMDF_EIDOLON_QUEUED)) {
                            if (b->eidolonTimer <= 0) {
                                btl_setCmd(arg0->delayedCmd, BTL_CMD_TRANCE_EIDOLON, g_gameState->gameData->ctx.battle->eidolonAbility, btl_getUnitsWithStatus(1, 0), 8);
                                g_gameState->gameData->ctx.battle->cmdFlags |= BTL_CMDF_EIDOLON_QUEUED;
                            } else {
                                b->eidolonTimer -= arg0->atbSpeed;
                            }
                        }
                    }
                }
                btl_tickStatusTimers(arg0);
            } else if ((arg0->status & BTL_STATUS_JUMP) && !(g_gameState->gameData->ctx.battle->cmdFlags & BTL_CMDF_SPEAR)) {
                t->jumpTimer -= arg0->atbSpeed;
                if (t->jumpTimer < 0) {
                    c = arg0->delayedCmd;
                    if (c->cmdNo == BTL_CMD_JUMP) {
                        btl_queueAutoCmd(arg0, BTL_CMD_SPEAR, ABIL_SPEAR, (u16)c->targets);
                    } else {
                        btl_queueAutoCmd(arg0, BTL_CMD_SPEAR_TRANCE, ABIL_SPEAR_TRANCE, (u16)c->targets);
                    }
                }
            }
        }
    }
    if ((arg0->status & BTL_STATUS_CONFUSE) && arg0 != g_gameState->gameData->ctx.battle->curCmd->caster) {
        if (arg0->model->motion == (BtlMotion *)arg0->motions[0] || arg0->model->motion == (BtlMotion *)arg0->motions[1] ||
            ((arg0->flags & BTL_UNIT_PARTY) && arg0->model->motion == (BtlMotion *)arg0->motions[9])) {
            arg0->rot[1] += 0x80;
        }
    }
}

void btl_updateUnitTint(BtlUnit *arg0) {
    BtlBgHeader *e;
    u8 *p;
    s32 t;
    u32 n;
    u32 m;
    u8 v;
    u32 w;
    s16 r, g, b;
    s32 rr, gg, bb;

    if (!((arg0->flags >> BTL_UNIT_HIDDEN_BIT) & 1) && !(arg0->status & BTL_STATUS_PETRIFY)) {
        e = btlbg_getHeader();
        if (arg0->status & (BTL_STATUS_ZOMBIE | BTL_STATUS_BERSERK | BTL_STATUS_HEAT | BTL_STATUS_FREEZE)) {
            btl_fillModelRgb(arg0->model, arg0->tint[0], arg0->tint[1], arg0->tint[2]);
            if (arg0->weaponModel != NULL) {
                btl_fillModelRgb(arg0->weaponModel, arg0->tint[0], arg0->tint[1], arg0->tint[2]);
            }
        } else {
            if (arg0->status & (BTL_STATUS_SHELL | BTL_STATUS_PROTECT)) {
                n = g_gameState->gameData->ctx.battle->tick;
                m = n;
                m = m % 24;
                /* v is the colour-select flag here and the fade level below; the table branch's level goes
                   through w (a u32), so its loads stay word-sized. */
                if ((arg0->status & (BTL_STATUS_SHELL | BTL_STATUS_PROTECT)) == (BTL_STATUS_SHELL | BTL_STATUS_PROTECT)) {
                    v = (n % 48) >= 24;
                } else {
                    v = (arg0->status & BTL_STATUS_PROTECT) < BTL_STATUS_PROTECT;
                }
                if (v) {
                    r = e->lightR - 0x40;
                    g = e->lightG + 0x18;
                    b = e->lightB + 0x48;
                } else {
                    r = e->lightR + 0x28;
                    g = e->lightG + 0x28;
                    b = e->lightB - 0x50;
                }
                if ((u8)m < 8) {
                    v = m;
                } else if ((u8)m < 16) {
                    v = 8;
                } else {
                    v = 24 - m;
                }
            } else if ((arg0->status & (BTL_STATUS_KO | BTL_STATUS_TRANCE)) == BTL_STATUS_TRANCE) {
                u8 tbl[8][3] = {
                    { 0xFF, 0x60, 0x60 }, { 0x68, 0x78, 0xFF }, { 0xFF, 0xB8, 0x78 }, { 0x80, 0xC0, 0xD0 },
                    { 0xC0, 0x68, 0x90 }, { 0xD0, 0x70, 0x78 }, { 0xC8, 0xD8, 0x68 }, { 0xD0, 0xB8, 0x68 },
                };
                p = tbl[BTL_UNIT_INDEX(arg0->flags)];
                r = e->lightR + (p[0] - 0x80);
                g = e->lightG + (p[1] - 0x80);
                b = e->lightB + (p[2] - 0x80);
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
            btl_setModelRgb(arg0->model, rr, gg, bb);
            if (arg0->weaponModel != NULL) {
                btl_setModelRgb(arg0->weaponModel, rr, gg, bb);
            }
        }
    }
end:
    n = BtlUnitHasStatus(arg0, BTL_STATUS_FLOAT);
    if ((u8)n) {
        arg0->pos[1] = t = 0xFF380000 - math_sin((g_gameState->gameData->ctx.battle->tick & 0xF) << 8) * 480;
        arg0->home->pos[1] = t;
    }
}

void btl_updateStatusColor(BtlUnit *arg0) {
    s16 a[3];
    s16 b[3];
    BtlBgHeader *e = btlbg_getHeader();
    s32 i;
    s32 v;
    b[0] = e->lightR;
    b[1] = e->lightG;
    b[2] = e->lightB;
    if (arg0->status & BTL_STATUS_ZOMBIE) {
        a[0] = 0x30;
        a[1] = 0x48;
        a[2] = 0x58;
    } else if (arg0->status & BTL_STATUS_BERSERK) {
        a[0] = -0x10;
        a[1] = 0x28;
        a[2] = 0x28;
    } else if (arg0->status & BTL_STATUS_HEAT) {
        a[0] = -0x50;
        a[1] = 0x10;
        a[2] = 0x48;
    } else if (arg0->status & BTL_STATUS_FREEZE) {
        a[0] = 0x30;
        a[1] = 0;
        a[2] = -0x60;
    } else {
        a[2] = 0;
        a[1] = 0;
        a[0] = 0;
    }
    for (i = 0; i < 3; i++) {
        v = b[i] - a[i];
        if (v < 0) {
            arg0->tint[i] = 0;
        } else if (v >= 0x100) {
            arg0->tint[i] = 0xFF;
        } else {
            arg0->tint[i] = b[i] - a[i];
        }
    }
}

void btl_setStoneClut(BtlUnit *arg0, s32 arg1) {
    s8 d;
    s8 i;
    d = 6;
    if (arg0->flags & BTL_UNIT_PARTY) {
        d = -12;
    }
    if (arg1 & 0xFF) {
        d *= -1;
    }
    i = 0;
    if (arg0->flags & BTL_UNIT_PARTY) {
        arg0->weaponModel->texPages->clut = ((d + clut_getY(arg0->weaponModel->texPages->clut)) << 6) | 0x10;
    }
    while (i < db_getModelTexCount(arg0->modelId | DB_MODEL)) {
        arg0->model->texPages[i].clut = ((d + clut_getY(arg0->model->texPages[i].clut)) << 6) | 0x10;
        i++;
    }
}

void btl_makeGreyCluts(BtlUnit *arg0) {
    s16 i;
    s16 j;
    u8 c[3];
    u8 g;

    for (i = 0; i < db_getModelTexCount(arg0->modelId | DB_MODEL); i++) {
        g_btlClutRect.x = 0x100;
        g_btlClutRect.y = clut_getY(arg0->model->texPages[i].clut);
        g_btlClutRect.w = 0x100;
        g_btlClutRect.h = 1;
        StoreImage(&g_btlClutRect, (u32 *)g_btlClutBuf[i]);
        DrawSync(0);
        for (j = 0; j < 0x100; j++) {
            c[0] = g_btlClutBuf[i][j] & 0x1F;
            c[1] = (g_btlClutBuf[i][j] >> 5) & 0x1F;
            c[2] = (g_btlClutBuf[i][j] >> 10) & 0x1F;
            if (c[0] == 0 && c[1] == 0 && c[2] == 0) {
                g = 0;
            } else {
                g = (c[0] + c[1] + c[2]) / 3;
                if (g == 0) {
                    g = 1;
                }
            }
            g_btlClutBuf[i][j] = (g << 10) + (g << 5) + g;
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

s32 btl_hasStatus(BtlUnit *arg0, u32 arg1) {
    s32 ret = 0;
    if ((arg0->autoStatus & arg1) || (arg0->status & arg1)) {
        ret = 1;
    }
    return ret;
}

void btl_saveCharStatus(Unk800C09D8 *arg0, Unk800C09D8 *arg1) {
    arg0->chrStatus = arg1->status & 0x7F;
}

void btl_clearUnitStatus(BtlUnit *arg0) {
    u32 i;
    s16 *p;
    arg0->status = 0;
    arg0->autoStatus = 0;
    arg0->statusImmune = 0;
    p = arg0->dotTimers;
    for (i = 0; i < 3; i++) {
        p[i] = 0;
    }
    for (i = 0; i < 14; i++) {
        p[i + 3] = 0;
    }
}

void btl_initDoomCount(Unk800C09D8 *arg0) {
    s32 t = (0x3C - arg0->spirit) * 8;
    arg0->countdownStep = t * g_gameState->gameData->ctx.battle->statusInfo[BTL_STATUS_DOOM_BIT].timerScale;
}

void btl_interruptUnit(BtlUnit *arg0, s32 arg1) {
    if (btl_cancelUnitCmds(arg0) & 0xFF) {
        arg0->atb = 0;
        arg0->flags &= ~BTL_UNIT_ATB_ACTIVE;
        if (!(arg1 & 0x100)) {
            arg0->cmdIssued = 0;
        }
    }
}

u32 btl_addStatuses(BtlUnit *arg0, u32 arg1) {
    u32 ret = 0;
    u32 i;
    for (i = 0; i < 32; i++) {
        if (arg1 & (1 << i)) {
            u32 r = btl_addStatus(arg0, 1 << i);
            if ((ret == 0 && r != 0) || (ret == 1 && r >= 2)) {
                ret = r;
            }
        }
    }
    return ret;
}

s32 btl_removeStatuses(BtlUnit *arg0, u32 arg1) {
    s32 ret = 1;
    u32 i;
    for (i = 0; i < 32; i++) {
        if ((arg1 & (1 << i)) && btl_removeStatus(arg0, 1 << i) == 2) {
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

void btl_saveEnemyCluts(u32 *arg0) {
    g_btlClutRect.x = 0x100;
    g_btlClutRect.y = 0xE0;
    g_btlClutRect.w = 0x100;
    g_btlClutRect.h = 6;
    StoreImage(&g_btlClutRect, arg0);
    g_btlSavedCluts = arg0;
}

void btl_savePartyCluts(u32 *arg0) {
    g_btlClutRect.x = 0x100;
    g_btlClutRect.y = 0xE6;
    g_btlClutRect.w = 0x100;
    g_btlClutRect.h = 0xC;
    StoreImage(&g_btlClutRect, arg0);
    g_btlSavedCluts = arg0;
}

void btl_reapplyStatusLook(BtlUnit *arg0) {
    if (arg0->status & BTL_STATUS_PETRIFY) {
        btl_setStoneClut(arg0, 1);
    } else if (arg0->status & (BTL_STATUS_ZOMBIE | BTL_STATUS_BERSERK | BTL_STATUS_HEAT | BTL_STATUS_FREEZE)) {
        btl_updateStatusColor(arg0);
    }
    btl_applyBgLight(arg0->model);
    if (arg0->flags & BTL_UNIT_PARTY) {
        btl_applyBgLight(arg0->weaponModel);
    }
}

void btl_tickStatusTimers(BtlUnit *arg0) {
    s32 i;
    s32 bit;
    s32 m;
    for (i = 0; i < 16; i++) {
        bit = BTL_STATUS_POISON << i;
        m = bit & BTL_STATUS_TIMED;
        if ((arg0->status & m) && (s16)(arg0->statusTimers[i] -= arg0->atbSpeed) < 0) {
            if (bit < 0) {
                if (!btl_isCmdPending(arg0->koCmd) && btl_addStatus(arg0, BTL_STATUS_PETRIFY) != 2) {
                    btl_removeStatus(arg0, BTL_STATUS_GRADUAL_PETRIFY);
                    arg0->popupFlags |= BTL_POPUP_MISS;
                    btl_spawnPopups(arg0);
                }
            } else if (bit & BTL_STATUS_DOOM) {
                btl_addStatus(arg0, BTL_STATUS_KO);
                btl_spawnPopups(arg0);
            } else {
                btl_removeStatus(arg0, bit);
            }
        }
    }
}

void btl_checkSideDefeated(BtlUnit *arg0) {
    Battle *b = g_gameState->gameData->ctx.battle;
    s32 found;
    BtlUnit *u;
    u32 f;
    s32 k;
    s32 n;

    n = BTL_KO_DONE;
    for (u = b->unitList; u != NULL; u = u->next) {
        if ((u->flags & BTL_UNIT_PARTY) == (arg0->flags & BTL_UNIT_PARTY)) {
            if (!(u->status & (BTL_STATUS_PETRIFY | BTL_STATUS_VENOM | BTL_STATUS_KO | BTL_STATUS_STOP)) || ((u->status & BTL_STATUS_KO) && u->koState != n)) {
                return;
            }
        }
    }
    if (!(arg0->flags & BTL_UNIT_PARTY)) {
        found = 0;
        for (u = b->unitList; u != NULL; u = u->next) {
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
        if (b->subPhase != BTL_END_DEFEAT) {
            b->subPhase = BTL_END_VICTORY;
        }
    } else {
        for (u = b->unitList; u != NULL; u = u->next) {
            f = u->flags;
            if (f & BTL_UNIT_PARTY) {
                k = g_gameState->gameData->characters[BTL_UNIT_INDEX(f)].info.b.kind;
            } else {
                k = CHR_KIND_NONE;
            }
            if ((u32)(k - CHR_KIND_EIKO) < 2) {
                if (u->status & (BTL_STATUS_PETRIFY | BTL_STATUS_VENOM | BTL_STATUS_ZOMBIE | BTL_STATUS_STOP)) {
                    break;
                }
                if (btl_isCmdQueued(u, BTL_CMD_REBIRTH_FLAME)) {
                    return;
                }
                if (g_randPtr >= g_randTableEnd) {
                    rand_refill();
                }
                k = item_getCount(ITEM_PHOENIX_PINION);
                if (*g_randPtr++ < k) {
                    btlui_setAtbEnabled(1);
                    btl_setCmd(u->cmd, BTL_CMD_REBIRTH_FLAME, ABIL_REBIRTH_FLAME, btl_getUnitMask(0) & 0xFFFF, 1);
                    return;
                }
            }
        }
        b->subPhase = BTL_END_DEFEAT;
        btl_showMessage(BTL_MSG_ANNIHILATED);
    }
    btlui_setAtbEnabled(0);
    b->phase = BTL_PHASE_END;
    btl_clearCmdQueue(b);
}

s32 btl_rollAmbush(u32 *arg0) {
    s32 r = 2;
    s32 th;
    if ((*arg0 >> 2) & 1) {
        r = (((*arg0 >> 3) & 1) == 0) * 2;
    } else {
        th = 0x18;
        if ((u8)btl_partyHasSupport(1, SA_BIT(SA_ALERT))) {
            th = 0;
        } else if ((*arg0 >> 3) & 1) {
            th = 0xFF;
        }
        if (g_randPtr >= g_randTableEnd) {
            rand_refill();
        }
        if (*g_randPtr++ < th) {
            r = 0;
        } else {
            th = 0x10;
            if ((u8)btl_partyHasSupport(1, SA_BIT(SA_INITIATIVE))) {
                th = 0x55;
            }
            if (g_randPtr >= g_randTableEnd) {
                rand_refill();
            }
            if (*g_randPtr++ < th) {
                r = 1;
            }
        }
    }
    return r;
}

void btl_addEnemyRewards(BtlEnemyType *arg0) {
    u8 n;
    u8 it;
    Unk800FDF30 *d;

    n = 0;
    d = &g_btlRewards;
    d->gil += arg0->gil;
    d->exp += arg0->exp;
    for (; n < 16; n++) {
        if (d->items[n] == ITEM_NONE) {
            break;
        }
    }
    if (n < 16) {
        if (rand8_inline() < 1 && (it = arg0->drops.b[3]) != ITEM_NONE) {
            d->items[n++] = it;
            arg0->drops.b[3] = ITEM_NONE;
        } else if (rand8_inline() < 0x20 && (it = arg0->drops.b[2]) != ITEM_NONE) {
            d->items[n++] = it;
            arg0->drops.b[2] = ITEM_NONE;
        } else if (rand8_inline() < 0x60 && (it = arg0->drops.b[1]) != ITEM_NONE) {
            d->items[n++] = it;
            arg0->drops.b[1] = ITEM_NONE;
        }
        if ((it = arg0->drops.b[0]) != ITEM_NONE) {
            d->items[n] = it;
        }
    }
    if (d->card == CARD_NONE && (u32)arg0->card < 100) {
        if (rand8_inline() < 0x20) {
            d->card = arg0->card;
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

void btl_checkPartyDisabled(BtlUnit *arg0) {
    Battle *b = g_gameState->gameData->ctx.battle;
    BtlUnit *u;
    BtlUnit *six = 0;
    u32 st;
    for (u = b->unitList; u != 0; u = u->next) {
        if (BTL_UNIT_INDEX(u->flags) == 6) {
            six = u;
        }
        if ((u->flags & BTL_UNIT_PARTY) == (arg0->flags & BTL_UNIT_PARTY)) {
            st = u->status;
            if (!(st & (BTL_STATUS_PETRIFY | BTL_STATUS_VENOM | BTL_STATUS_KO | BTL_STATUS_STOP))) {
                return;
            }
            if ((u->hp == 0 || (st & BTL_STATUS_KO)) && (st & BTL_STATUS_AUTO_LIFE)) {
                return;
            }
            if (btl_isCmdQueued(u, BTL_CMD_AUTO_LIFE)) {
                return;
            }
        }
    }
    if (six == 0 || !btl_isCmdQueued(six, BTL_CMD_REBIRTH_FLAME)) {
        btlui_setAtbEnabled(0);
        btl_flushCmdQueue(b);
        b->escapeHeld = 0;
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

s32 btl_saveParty(Battle *arg0) {
    GameData *gd = g_gameState->gameData;
    u8 v = gd->battleResult;
    if ((v == BATTLE_END_DEFEAT || v == BATTLE_END_GAME_OVER) && !((arg0->scene.flags >> BTL_SCENE_NO_GAMEOVER_BIT) & 1)) {
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

void btl_saveCharacter(BtlUnit *arg0, s32 arg1) {
    Character *c = &g_gameState->gameData->characters[BTL_UNIT_INDEX(arg0->flags)];
    if (arg0->status & BTL_STATUS_TRANCE) {
        c->trance = 0;
    } else {
        c->trance = arg0->trance;
    }
    btl_copyStats((Unk800BD8C0 *)&c->hp, (Unk800BD8C0 *)&arg0->hp);
    if ((arg0->support[1] & SA_BIT(SA_GUARDIAN_MOG)) && arg1 == 0) {
        c->status = 0;
        return;
    }
    if (arg0->hp == 0) {
        btl_addStatus(arg0, BTL_STATUS_KO);
    }
    btl_saveCharStatus((Unk800C09D8 *)c, (Unk800C09D8 *)arg0);
}

void btl_unlinkUnit(BtlUnit *arg0) {
    BtlUnit *p;
    for (p = (BtlUnit *)&g_gameState->gameData->ctx.battle->unitList; p != NULL; p = p->next) {
        if (p->next == arg0) {
            p->next = arg0->next;
            return;
        }
    }
}

void btl_insertUnit(BtlUnit *arg0) {
    BtlUnit **pp = &g_gameState->gameData->ctx.battle->unitList;
    BtlUnit *n;
    s32 a;
    s32 b;
    while (pp != NULL) {
        n = *pp;
        a = (u8)arg0->flags >> BTL_UNIT_SLOT_SHIFT;
        b = (u8)n->flags >> BTL_UNIT_SLOT_SHIFT;
        if (a < b) {
            arg0->next = n;
            *pp = arg0;
            return;
        }
        pp = &n->next;
    }
}

void btl_tryEscape(u8 arg0) {
    Battle *b = g_gameState->gameData->ctx.battle;
    if (!(b->cmdFlags & BTL_CMDF_ESCAPE)) {
        if ((u8)b->scene.flags >> BTL_SCENE_CAN_ESCAPE_BIT == 0) {
            btl_showMessage(BTL_MSG_CANT_ESCAPE);
        } else if (arg0 && (btl_isAtbRunning() & 0xFF)) {
            btl_calcCmd(0, 0, 0, BTL_FORMULA_FLEE);
        }
    }
}

void btl_setUnitBrightness(BtlUnit *arg0, u32 arg1) {
    s32 i;
    BtlBgHeader *e;
    s32 k;
    u32 f;
    if (arg1 < 0x20) {
        i = 0;
        e = btlbg_getHeader();
        btl_setModelAbr(arg0->model, 0x60);
        for (; i < arg0->model->header->meshCount; i++) {
            btl_setMeshRgb(arg0->model, i, (s32)((e->lightR * arg1) << 11) >> 16, (s32)((e->lightG * arg1) << 11) >> 16, (s32)((e->lightB * arg1) << 11) >> 16);
        }
        if (arg0->weaponModel != NULL) {
            btl_setModelAbr(arg0->weaponModel, 0x60);
            for (i = 0; i < arg0->weaponModel->header->meshCount; i++) {
                btl_setMeshRgb(arg0->weaponModel, i, (s32)((e->lightR * arg1) << 11) >> 16, (s32)((e->lightG * arg1) << 11) >> 16, (s32)((e->lightB * arg1) << 11) >> 16);
            }
        }
        f = arg0->flags;
        k = f >> BTL_UNIT_INDEX_SHIFT;
        k &= 0xF;
        if (!(f & BTL_UNIT_PARTY)) {
            k += 9;
        }
        g_gameState->gameData->ctx.battle->work->shadows[k].shade = (s32)(btlbg_getHeader()->shadowShade * arg1) >> 5;
    }
}

u16 btl_getUnitsWithStatus(u32 arg0, s32 arg1) {
    BtlUnit *u;
    s32 r = 0;
    s32 all;
    for (u = g_gameState->gameData->ctx.battle->unitList; u != 0; u = u->next) {
        all = arg1 == 0;
        if (all || (u8)btl_hasStatus(u, arg1)) {
            if ((u->flags >> BTL_UNIT_TARGETABLE_BIT) & 1) {
                switch (arg0) {
                case 0:
                    if (!(u->flags & BTL_UNIT_PARTY)) {
                        break;
                    }
                    r |= u->unitBit;
                    break;
                case 1:
                    if (u->flags & BTL_UNIT_PARTY) {
                        break;
                    }
                case 2:
                    r |= u->unitBit;
                    break;
                }
            }
        }
    }
    return r;
}

BtlUnit *btl_getUnitBySlot(s32 arg0) {
    BtlUnit *u;
    for (u = g_gameState->gameData->ctx.battle->unitList; u != NULL; u = u->next) {
        if ((u8)u->flags >> BTL_UNIT_SLOT_SHIFT == arg0) {
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

s32 btl_hasEnemyFlag(BtlUnit *arg0, s32 arg1) {
    u32 f = arg0->flags;
    if (f & BTL_UNIT_PARTY) {
        return 0;
    }
    return g_gameState->gameData->ctx.battle->enemies[BTL_UNIT_INDEX(f)].type->category & arg1;
}

s32 btl_getModelSlot(BtlUnit *arg0) {
    u32 f = arg0->flags;
    s32 v = BTL_UNIT_INDEX(f);
    if (!(f & BTL_UNIT_PARTY)) {
        v += 9;
    }
    return v;
}

u32 btl_countActiveUnits(u32 arg0) {
    BtlUnit *u = g_gameState->gameData->ctx.battle->unitList;
    s32 n = 0;
    while (u != NULL) {
        u32 f = u->flags;
        if ((f & BTL_UNIT_PARTY) == arg0 && ((f >> BTL_UNIT_TARGETABLE_BIT) & 1) && !(u->status & BTL_STATUS_KO)) {
            n++;
        }
        u = u->next;
    }
    return n;
}

s32 btl_pickRandomUnit(s32 arg0) {
    u16 ids[4];
    u16 n = 0;
    BtlUnit *u;
    if (arg0 != 0) {
        arg0 = 1;
    }
    for (u = g_gameState->gameData->ctx.battle->unitList; u != NULL; u = u->next) {
        if ((u->flags & BTL_UNIT_PARTY) == arg0 && !(u->status & BTL_STATUS_KO) && ((u->flags >> BTL_UNIT_TARGETABLE_BIT) & 1)) {
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

s32 btl_playVictoryMusic(GameData *arg0, u32 arg1, s32 arg2) {
    Battle *b;
    if (!(arg0->battleFlags & GD_BTL_VICTORY_MUSIC_FADE)) {
        btl_soundCmd(SND_MUSIC_VOL_INTPL, 0, arg1, 0, 0);
        arg0->battleFlags |= GD_BTL_VICTORY_MUSIC_FADE;
    }
    if (!(arg0->battleFlags & GD_BTL_VICTORY_MUSIC_LOADED)) {
        b = g_gameState->gameData->ctx.battle;
        b->partyBrightness += 4;
        if ((u32)(s8)b->partyBrightness < arg1) {
            return 0;
        }
        btl_soundCmd(SND_MUSIC_LOAD, arg2, 0, 0, 0);
        arg0->battleFlags |= GD_BTL_VICTORY_MUSIC_LOADED;
    }
    if (btl_soundCmd(SND_GET_STATE, 0, 0, 0, 0) != 0) {
        return 0;
    }
    if (!(arg0->battleFlags & GD_BTL_VICTORY_MUSIC_PLAYING)) {
        btl_soundCmd(SND_MUSIC_PLAY, arg2, 0, 0, 0);
        arg0->battleFlags |= GD_BTL_VICTORY_MUSIC_PLAYING;
    }
    return 1;
}

void btl_playHitSound(BtlUnit *arg0, BtlUnit *arg1) {
    if (!(arg1->popupFlags & BTL_POPUP_MISS)) {
        btl_playUnitSound(arg0, (u16)btl_getWeaponSound((u8)arg0->flags >> BTL_UNIT_SLOT_SHIFT, 1), 0x7F);
    }
}

void btl_playDeathCry(BtlUnit *arg0, u16 arg1) {
    Battle *b = g_gameState->gameData->ctx.battle;
    if (b->deathCryWait == 0 && !((arg0->flags >> BTL_UNIT_DEATH_CRY_BIT) & 1)) {
        b->deathCryWait = 8;
        btl_playUnitSound(arg0, arg1, 0x7F);
        arg0->flags |= BTL_UNIT_DEATH_CRY;
    }
}

void btl_playUnitSound(BtlUnit *arg0, u16 arg1, u8 arg2) {
    SVECTOR v;
    if (arg1 != 0xFFFF) {
        v.vx = arg0->pos[0] >> 16;
        v.vy = arg0->pos[1] >> 16;
        v.vz = arg0->pos[2] >> 16;
        btl_soundCmd(SND_SE_PLAY, arg1, 0, btl_soundPan((s32)&v), arg2);
    }
}

void btl_applyBgLight(BtlModel *arg0) {
    s32 i;
    BtlBgHeader *p = btlbg_getHeader();
    for (i = 0; i < arg0->header->meshCount; i++) {
        btl_setMeshRgb(arg0, i & 0xFF, p->lightR, p->lightG, p->lightB);
    }
}

