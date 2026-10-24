#include "ovl_0b.h"

u16 btl_getUnitMask(u32 arg0) {
    BtlUnit *u;
    s32 r;

    r = 0;
    for (u = g_gameState->unk1C->unk8E4.battle->unitList; u != NULL; u = u->next) {
        switch (arg0) {
        case 0:
            if (u->unk68 & 1) {
                r |= u->unitBit;
            }
            break;
        case 1:
            if (!(u->unk68 & 1)) {
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
    for (u = g_gameState->unk1C->unk8E4.battle->unitList; u != NULL; u = u->next) {
        if (u->unitBit == arg0) {
            return u;
        }
    }
    return NULL;
}

BtlUnit *btl_findUnitByBit(u16 arg0) {
    s32 i;
    Battle *b = g_gameState->unk1C->unk8E4.battle;
    for (i = 0; i < 8; i++) {
        if (b->units[i].unitBit == arg0) {
            return &b->units[i];
        }
    }
    return NULL;
}

u32 btl_addStatus(BtlUnit *u, u32 f) {
    u32 n = 0;
    Battle154C *tbl = g_gameState->unk1C->unk8E4.battle->statusInfo;
    BtlUnit34 *s = (BtlUnit34 *)&u->statusImmune;
    u32 i;
    u32 blk;
    s16 k;

    if (u->statusImmune & f) {
        return 0;
    }
    if (s->unk4 & f) {
        return 1;
    }
    if ((s->unk8 & f) && (f & 0x8800FF7F)) {
        return 1;
    }
    i = 0;
    blk = 0;
    for (; i < 32; i++) {
        if (f & (1 << i)) {
            n = i;
        }
        if (s->unk8 & (1 << i)) {
            blk |= tbl[i].unk8;
        }
    }
    if (btl_isCmdQueued(u, BTL_CMD_PETRIFY)) {
        blk |= tbl[0].unk8;
    }
    if (blk & f) {
        return 1;
    }
    if (!(f & 0x10180001)) {
        if ((f & 0x20D03) && (u8)btl_cancelUnitCmds(u)) {
            u->atb = 0;
            u->unk68 &= ~0x10000;
            if (!(f & 0x100)) {
                u->cmdIssued = 0;
            }
        }
        s->unk8 |= f;
    }
    switch (n) {
    case 0:
        if (!btl_isCmdPending(u->koCmd)) {
            if (g_gameState->unk1C->unk8E4.battle->phase >= BTL_PHASE_ENTRANCE) {
                btl_setCmd(u->koCmd, BTL_CMD_PETRIFY, 0, u->unitBit, 0);
            } else {
                if (u->unk68 & 1) {
                    btl_makeGreyCluts(u);
                }
                s->unk8 |= f;
                u->unk68 &= ~0x10000;
                btl_setStoneClut(u, 1);
            }
        }
        break;
    case 1:
        if (g_gameState->unk1C->unk8E4.battle->phase >= BTL_PHASE_ENTRANCE) {
            btl_checkSideDefeated(u);
        }
    case 17:
        if ((u->unk68 & 1) && (s32)u->model->unkC != u->motions[1] &&
            g_gameState->unk1C->unk8E4.battle->curCmd->caster != u) {
            u->model->unkC = (BtlUnit9CC *)u->motions[1];
            u->unkA0->unk3A = 0;
        }
        break;
    case 6:
        if ((u->unk68 & 1) && !(u->status & BTL_STATUS_TRANCE)) {
            u->trance = 0;
        }
    case 11:
    case 24:
    case 25:
        btl_updateStatusColor(u);
        break;
    case 8:
        if (u->hp != 0) {
            u->popupFlags |= 0x40;
            btl_killUnit(u);
        } else {
            u->hp = 0;
        }
        u->atb = 0;
        if (!btl_isCmdPending(u->koCmd)) {
            btl_setCmd(u->koCmd, BTL_CMD_KO, 0, u->unitBit, 0);
            if (!(u->unk68 & 1)) {
                btl_checkEnemiesKO(u);
            }
        }
        break;
    case 12:
        btl_checkSideDefeated(u);
        break;
    case 14:
        btl_setCmd(u->tranceCmd, BTL_CMD_TRANCE, 0, u->unitBit, 0);
        break;
    case 19:
        if (u->status & BTL_STATUS_SLOW) {
            btl_removeStatus(u, BTL_STATUS_SLOW);
            return 2;
        }
        btl_resetAtbSpeed((Unk800BD8F0 *)u);
        u->atbSpeed = u->atbSpeed * 3 / 2;
        s->unk8 |= f;
        break;
    case 20:
        if (u->status & BTL_STATUS_HASTE) {
            btl_removeStatus(u, BTL_STATUS_HASTE);
            return 2;
        }
        btl_resetAtbSpeed((Unk800BD8F0 *)u);
        u->atbSpeed = u->atbSpeed * 2 / 3;
        s->unk8 |= f;
        break;
    case 26:
        btl_hideMeshes(u, u->unkAE);
        break;
    case 28:
        s->unk8 ^= f;
        if (u->status & BTL_STATUS_MINI) {
            u->model->unk0 |= 2;
            u->model->unk54 = 0x800;
            u->model->unk56 = 0x800;
            u->model->unk58 = 0x800;
            g_gameState->unk1C->unk8E4.battle->unk11C0->unk28[btl_getModelSlot(u)].width = (u->shadowW >> 1) * 14;
            g_gameState->unk1C->unk8E4.battle->unk11C0->unk28[btl_getModelSlot(u)].depth = (u->shadowD >> 1) * 12;
        } else {
            u->model->unk0 &= ~2;
            g_gameState->unk1C->unk8E4.battle->unk11C0->unk28[btl_getModelSlot(u)].width = u->shadowW * 14;
            g_gameState->unk1C->unk8E4.battle->unk11C0->unk28[btl_getModelSlot(u)].depth = u->shadowD * 12;
        }
        break;
    }
    btl_removeStatuses(u, tbl[n].unk4);
    if (u->status & (BTL_STATUS_PETRIFY | BTL_STATUS_KO | BTL_STATUS_STOP | BTL_STATUS_JUMP)) {
        u->unk68 &= ~0x10000;
    }
    if (f & 0xEFFF0000) {
        if (f & 0x9B130000) {
            k = (0x3C - u->spirit) * 8;
        } else if (f & 0x24EC0000) {
            k = u->spirit * 8;
        } else {
            k = (0x3C - u->spirit) * 4;
        }
        u->unk46[n - 16] = tbl[n].unk2 * k;
    }
    if (f & 0x50002) {
        btl_initDotTimer(u, n);
    }
    return 2;
}

s32 btl_removeStatus(BtlUnit *u, u32 f) {
    BtlUnit34 *s = (BtlUnit34 *)&u->statusImmune;
    BtlUnit9C *m;
    s32 *a4;
    BtlCmd *c;

    if (s->unk4 & f) {
        return 1;
    }
    if (!(s->unk8 & f)) {
        return 1;
    }
    if (!(u->unk68 & 1) && g_gameState->unk1C->unk8E4.battle->phase == BTL_PHASE_END && (f & 0x1003)) {
        return 1;
    }
    s->unk8 &= ~f;
    switch (f) {
    case 0x1:
        btl_setStoneClut(u, 0);
        break;
    case 0x800:
        if ((u8)btl_cancelUnitCmds(u)) {
            u->atb = 0;
            u->cmdIssued = 0;
            u->unk68 &= ~0x10000;
        }
    case 0x40:
    case 0x1000000:
    case 0x2000000:
        if (u->status & (BTL_STATUS_ZOMBIE | BTL_STATUS_BERSERK | BTL_STATUS_HEAT | BTL_STATUS_FREEZE)) {
            btl_updateStatusColor(u);
        }
        break;
    case 0x100:
        m = u->model;
        a4 = u->motions;
        u->unk7D = 0;
        u->unk7B = 0;
        u->unk68 &= ~0x20000;
        u->unk68 &= ~0x80000;
        u->unk68 &= ~0x100000;
        u->unk68 &= ~0x200000;
        if ((s32)m->unkC == a4[4] || (s32)m->unkC == a4[8]) {
            m->unk50[2].unk4 |= 1;
            u->model->unk50[2].unkC = 0;
            u->model->unk50[2].unk12 = 0x1000;
            u->model->unkC = (BtlUnit9CC *)u->motions[6];
            u->unkA0->unk3A = 0;
        }
        c = g_gameState->unk1C->unk8E4.battle->curCmd;
        if (c->caster != u || c->cmdNo > 0x30) {
            u->cmdIssued = 0;
        }
        btl_removeQueuedCmd(u, BTL_CMD_KO);
        if (u->autoStatus & BTL_STATUS_REGEN) {
            btl_initDotTimer(u, 0x12);
        }
        break;
    case 0x400:
        u->rot[1] = u->unkA0->unk22;
        if ((u8)btl_cancelUnitCmds(u)) {
            u->atb = 0;
            u->cmdIssued = 0;
            u->unk68 &= ~0x10000;
        }
        break;
    case 0x4000:
        u->trance = 0;
        func_800B3DBC(u);
        if (u->status & BTL_STATUS_JUMP) {
            btl_removeStatus(u, BTL_STATUS_JUMP);
            u->unk68 &= ~0x400000;
            u->pos[0] = u->unk98->unk0[0];
            u->pos[1] = u->unk98->unk0[1];
            u->pos[2] = u->unk98->unk0[2];
            u->model->unkC = (BtlUnit9CC *)u->motions[(u->unk68 >> 18) & 1];
            u->unkA0->unk3A = 0;
        }
        btl_setCmd(u->tranceCmd, BTL_CMD_TRANCE, 0, u->unitBit, 0);
        break;
    case 0x80000:
    case 0x100000:
        btl_resetAtbSpeed((Unk800BD8F0 *)u);
        break;
    case 0x200000:
        u->pos[1] = 0;
        u->unk98->unk0[1] = 0;
        break;
    case 0x4000000:
        btl_showMeshes(u, u->unkAE);
        break;
    case 0x10000000:
        u->model->unk0 &= ~2;
        g_gameState->unk1C->unk8E4.battle->unk11C0->unk28[btl_getModelSlot(u)].width = u->shadowW * 14;
        g_gameState->unk1C->unk8E4.battle->unk11C0->unk28[btl_getModelSlot(u)].depth = u->shadowD * 12;
        break;
    case 0x40000000:
        u->unkB2 = 3;
        u->atb = 0;
        u->cmdIssued = 0;
        u->unk68 |= 0x10000;
        u->delayedCmd->cmdNo = 0;
        u->delayedCmd->targets = 0;
        break;
    case 0x80000000:
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
            arg0->unk68 |= 0x10000;
        }
        if (btl_isAtbRunning()) {
            if ((arg0->unk68 >> 16) & 1) {
                if (arg0->status & BTL_STATUS_VENOM) {
                    if (t->unkC <= 0) {
                        btl_initDotTimer(arg0, 1);
                        btl_tickPoison(arg0);
                        btl_tickVenomMp(arg0);
                        btl_spawnTickPopups(arg0);
                    } else {
                        t->unkC -= arg0->atbSpeed;
                    }
                }
                if (arg0->status & BTL_STATUS_POISON) {
                    if (t->unkE <= 0) {
                        btl_initDotTimer(arg0, 0x10);
                        btl_tickPoison(arg0);
                        btl_spawnTickPopups(arg0);
                    } else {
                        t->unkE -= arg0->atbSpeed;
                    }
                }
                if (BtlUnitHasStatus(arg0, BTL_STATUS_REGEN)) {
                    if (t->unk10 <= 0) {
                        btl_initDotTimer(arg0, 0x12);
                        btl_tickRegen(arg0);
                        btl_spawnTickPopups(arg0);
                    } else {
                        t->unk10 -= arg0->atbSpeed;
                    }
                }
                if ((arg0->status & BTL_STATUS_TRANCE) && ((arg0->unk68 >> 1) & 0xF) == 2) {
                    b = g_gameState->unk1C->unk8E4.battle;
                    fl = b->cmdFlags;
                    if (fl & 4) {
                        if (!(fl & 8)) {
                            if (b->eidolonTimer <= 0) {
                                btl_setCmd(arg0->delayedCmd, BTL_CMD_TRANCE_EIDOLON, g_gameState->unk1C->unk8E4.battle->eidolonAbility, btl_getUnitsWithStatus(1, 0), 8);
                                g_gameState->unk1C->unk8E4.battle->cmdFlags |= 8;
                            } else {
                                b->eidolonTimer -= arg0->atbSpeed;
                            }
                        }
                    }
                }
                btl_tickStatusTimers(arg0);
            } else if ((arg0->status & BTL_STATUS_JUMP) && !(g_gameState->unk1C->unk8E4.battle->cmdFlags & 0x10)) {
                t->unk2E -= arg0->atbSpeed;
                if (t->unk2E < 0) {
                    c = arg0->delayedCmd;
                    if (c->cmdNo == BTL_CMD_JUMP) {
                        btl_queueAutoCmd(arg0, BTL_CMD_SPEAR, BTL_ABIL_SPEAR, (u16)c->targets);
                    } else {
                        btl_queueAutoCmd(arg0, BTL_CMD_SPEAR_TRANCE, BTL_ABIL_SPEAR_TRANCE, (u16)c->targets);
                    }
                }
            }
        }
    }
    if ((arg0->status & BTL_STATUS_CONFUSE) && arg0 != g_gameState->unk1C->unk8E4.battle->curCmd->caster) {
        if (arg0->model->unkC == (BtlUnit9CC *)arg0->motions[0] || arg0->model->unkC == (BtlUnit9CC *)arg0->motions[1] ||
            ((arg0->unk68 & 1) && arg0->model->unkC == (BtlUnit9CC *)arg0->motions[9])) {
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

    if (!((arg0->unk68 >> 22) & 1) && !(arg0->status & BTL_STATUS_PETRIFY)) {
        e = btlbg_getHeader();
        if (arg0->status & (BTL_STATUS_ZOMBIE | BTL_STATUS_BERSERK | BTL_STATUS_HEAT | BTL_STATUS_FREEZE)) {
            btl_fillModelRgb(arg0->model, arg0->unkC1[0], arg0->unkC1[1], arg0->unkC1[2]);
            if (arg0->weaponModel != NULL) {
                btl_fillModelRgb(arg0->weaponModel, arg0->unkC1[0], arg0->unkC1[1], arg0->unkC1[2]);
            }
        } else {
            if (arg0->status & (BTL_STATUS_SHELL | BTL_STATUS_PROTECT)) {
                n = g_gameState->unk1C->unk8E4.battle->tick;
                m = n;
                m = m % 24;
                /* v is the colour-select flag here and the fade level below; the table branch's level goes
                   through w (a u32), so its loads stay word-sized. */
                if ((arg0->status & (BTL_STATUS_SHELL | BTL_STATUS_PROTECT)) == 0xC00000) {
                    v = (n % 48) >= 24;
                } else {
                    v = (arg0->status & BTL_STATUS_PROTECT) < 0x800000;
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
            } else if ((arg0->status & (BTL_STATUS_KO | BTL_STATUS_TRANCE)) == 0x4000) {
                u8 tbl[8][3] = {
                    { 0xFF, 0x60, 0x60 }, { 0x68, 0x78, 0xFF }, { 0xFF, 0xB8, 0x78 }, { 0x80, 0xC0, 0xD0 },
                    { 0xC0, 0x68, 0x90 }, { 0xD0, 0x70, 0x78 }, { 0xC8, 0xD8, 0x68 }, { 0xD0, 0xB8, 0x68 },
                };
                p = tbl[(arg0->unk68 >> 1) & 0xF];
                r = e->lightR + (p[0] - 0x80);
                g = e->lightG + (p[1] - 0x80);
                b = e->lightB + (p[2] - 0x80);
                w = g_gameState->unk1C->unk8E4.battle->tick & 0xF;
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
        arg0->pos[1] = t = 0xFF380000 - math_sin((g_gameState->unk1C->unk8E4.battle->tick & 0xF) << 8) * 480;
        arg0->unk98->unk0[1] = t;
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
            arg0->unkC1[i] = 0;
        } else if (v >= 0x100) {
            arg0->unkC1[i] = 0xFF;
        } else {
            arg0->unkC1[i] = b[i] - a[i];
        }
    }
}

void btl_setStoneClut(BtlUnit *arg0, s32 arg1) {
    s8 d;
    s8 i;
    d = 6;
    if (arg0->unk68 & 1) {
        d = -12;
    }
    if (arg1 & 0xFF) {
        d *= -1;
    }
    i = 0;
    if (arg0->unk68 & 1) {
        arg0->weaponModel->unk18->unk2 = ((d + func_8001DA6C(arg0->weaponModel->unk18->unk2)) << 6) | 0x10;
    }
    while (i < func_80063830(arg0->modelId | 0x20000)) {
        arg0->model->unk18[i].unk2 = ((d + func_8001DA6C(arg0->model->unk18[i].unk2)) << 6) | 0x10;
        i++;
    }
}

void btl_makeGreyCluts(BtlUnit *arg0) {
    s16 i;
    s16 j;
    u8 c[3];
    u8 g;

    for (i = 0; i < func_80063830(arg0->modelId | 0x20000); i++) {
        D_800FDF50.x = 0x100;
        D_800FDF50.y = func_8001DA6C(arg0->model->unk18[i].unk2);
        D_800FDF50.w = 0x100;
        D_800FDF50.h = 1;
        StoreImage(&D_800FDF50, (u32 *)D_800FDF60[i]);
        DrawSync(0);
        for (j = 0; j < 0x100; j++) {
            c[0] = D_800FDF60[i][j] & 0x1F;
            c[1] = (D_800FDF60[i][j] >> 5) & 0x1F;
            c[2] = (D_800FDF60[i][j] >> 10) & 0x1F;
            if (c[0] == 0 && c[1] == 0 && c[2] == 0) {
                g = 0;
            } else {
                g = (c[0] + c[1] + c[2]) / 3;
                if (g == 0) {
                    g = 1;
                }
            }
            D_800FDF60[i][j] = (g << 10) + (g << 5) + g;
        }
        D_800FDF50.y += 12;
        LoadImage(&D_800FDF50, (u32 *)D_800FDF60[i]);
    }
    DrawSync(0);
}

void btl_makeEnemyGreyCluts(void) {
    u16 *p = (u16 *)D_800FE360;
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
    D_800FDF50.x = 0x100;
    D_800FDF50.y = 0xDA;
    D_800FDF50.h = 6;
    D_800FDF50.w = 0x100;
    LoadImage(&D_800FDF50, D_800FE360);
}

void btl_makePartyGreyCluts(void) {
    s32 i;
    s32 j;
    s8 c[3];
    s8 v;
    u16 *p;
    D_800FDF50.x = 0x100;
    D_800FDF50.y = 0xF2;
    D_800FDF50.w = 0x100;
    D_800FDF50.h = 1;
    for (i = 2; i < 0xC; i += 3) {
        j = 0;
        p = (u16 *)D_800FE360;
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
        D_800FDF50.y = i + 0xF2;
        LoadImage(&D_800FDF50, (u32 *)((u16 *)D_800FE360 + (i << 8)));
    }
}

s32 btl_hasStatus(BtlUnit *arg0, u32 arg1) {
    s32 ret = 0;
    if ((arg0->autoStatus & arg1) || (arg0->status & arg1)) {
        ret = 1;
    }
    return ret;
}

void func_800C09D8(Unk800C09D8 *arg0, Unk800C09D8 *arg1) {
    arg0->unk38 = arg1->unk3C & 0x7F;
}

void btl_clearUnitStatus(BtlUnit *arg0) {
    u32 i;
    s16 *p;
    arg0->status = 0;
    arg0->autoStatus = 0;
    arg0->statusImmune = 0;
    p = arg0->unk40;
    for (i = 0; i < 3; i++) {
        p[i] = 0;
    }
    for (i = 0; i < 14; i++) {
        p[i + 3] = 0;
    }
}

void btl_initDoomCount(Unk800C09D8 *arg0) {
    s32 t = (0x3C - arg0->unk33) * 8;
    arg0->unk66 = t * g_gameState->unk1C->unk8E4.battle->statusInfo[27].unk2;
}

void btl_interruptUnit(BtlUnit *arg0, s32 arg1) {
    if (btl_cancelUnitCmds(arg0) & 0xFF) {
        arg0->atb = 0;
        arg0->unk68 &= ~0x10000;
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

    if (n == 1) {
        v = u->spirit * 4;
        k = 0;
    } else if (n == 0x10) {
        v = u->spirit * 4;
        k = 1;
    } else {
        v = (0x3C - u->spirit) * 4;
        k = 2;
    }
    u->unk40[k] = g_gameState->unk1C->unk8E4.battle->statusInfo[n].unk1 * v;
}

void btl_saveEnemyCluts(u32 *arg0) {
    D_800FDF50.x = 0x100;
    D_800FDF50.y = 0xE0;
    D_800FDF50.w = 0x100;
    D_800FDF50.h = 6;
    StoreImage(&D_800FDF50, arg0);
    D_800FE360 = arg0;
}

void btl_savePartyCluts(u32 *arg0) {
    D_800FDF50.x = 0x100;
    D_800FDF50.y = 0xE6;
    D_800FDF50.w = 0x100;
    D_800FDF50.h = 0xC;
    StoreImage(&D_800FDF50, arg0);
    D_800FE360 = arg0;
}

void btl_reapplyStatusLook(BtlUnit *arg0) {
    if (arg0->status & BTL_STATUS_PETRIFY) {
        btl_setStoneClut(arg0, 1);
    } else if (arg0->status & (BTL_STATUS_ZOMBIE | BTL_STATUS_BERSERK | BTL_STATUS_HEAT | BTL_STATUS_FREEZE)) {
        btl_updateStatusColor(arg0);
    }
    btl_applyBgLight(arg0->model);
    if (arg0->unk68 & 1) {
        btl_applyBgLight(arg0->weaponModel);
    }
}

void btl_tickStatusTimers(BtlUnit *arg0) {
    s32 i;
    s32 bit;
    s32 m;
    for (i = 0; i < 16; i++) {
        bit = 0x10000 << i;
        m = bit & 0xEFFF0000;
        if ((arg0->status & m) && (s16)(arg0->unk46[i] -= arg0->atbSpeed) < 0) {
            if (bit < 0) {
                if (!btl_isCmdPending(arg0->koCmd) && btl_addStatus(arg0, BTL_STATUS_PETRIFY) != 2) {
                    btl_removeStatus(arg0, BTL_STATUS_GRADUAL_PETRIFY);
                    arg0->popupFlags |= 0x20;
                    btl_spawnPopups(arg0);
                }
            } else if (bit & 0x08000000) {
                btl_addStatus(arg0, BTL_STATUS_KO);
                btl_spawnPopups(arg0);
            } else {
                btl_removeStatus(arg0, bit);
            }
        }
    }
}

void btl_checkSideDefeated(BtlUnit *arg0) {
    Battle *b = g_gameState->unk1C->unk8E4.battle;
    s32 found;
    BtlUnit *u;
    u32 f;
    s32 k;
    s32 n;

    n = 6;
    for (u = b->unitList; u != NULL; u = u->next) {
        if ((u->unk68 & 1) == (arg0->unk68 & 1)) {
            if (!(u->status & (BTL_STATUS_PETRIFY | BTL_STATUS_VENOM | BTL_STATUS_KO | BTL_STATUS_STOP)) || ((u->status & BTL_STATUS_KO) && u->unk7D != n)) {
                return;
            }
        }
    }
    if (!(arg0->unk68 & 1)) {
        found = 0;
        for (u = b->unitList; u != NULL; u = u->next) {
            if (u->unk68 & 1) {
                if (!(u->status & (BTL_STATUS_PETRIFY | BTL_STATUS_VENOM | BTL_STATUS_KO | BTL_STATUS_STOP)) || ((u->hp == 0 || (u->status & BTL_STATUS_KO)) && (u->status & BTL_STATUS_AUTO_LIFE)) || btl_isCmdQueued(u, BTL_CMD_AUTO_LIFE)) {
                    found = 1;
                    break;
                }
            }
        }
        if (!found) {
            return;
        }
        if (b->subPhase != 1) {
            b->subPhase = 0;
        }
    } else {
        for (u = b->unitList; u != NULL; u = u->next) {
            f = u->unk68;
            if (f & 1) {
                k = g_gameState->unk1C->characters[(f >> 1) & 0xF].unk34.b.unk4;
            } else {
                k = 0x13;
            }
            if ((u32)(k - 10) < 2) {
                if (u->status & (BTL_STATUS_PETRIFY | BTL_STATUS_VENOM | BTL_STATUS_ZOMBIE | BTL_STATUS_STOP)) {
                    break;
                }
                if (btl_isCmdQueued(u, BTL_CMD_REBIRTH_FLAME)) {
                    return;
                }
                if (g_randPtr >= g_randTableEnd) {
                    rand_refill();
                }
                k = item_getCount(0xF9);
                if (*g_randPtr++ < k) {
                    btlui_setAtbEnabled(1);
                    btl_setCmd(u->cmd, BTL_CMD_REBIRTH_FLAME, BTL_ABIL_REBIRTH_FLAME, btl_getUnitMask(0) & 0xFFFF, 1);
                    return;
                }
            }
        }
        b->subPhase = 1;
        btl_showMessage(5);
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
        if ((u8)btl_partyHasSupport(1, 0x200)) {
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
            if ((u8)btl_partyHasSupport(1, 0x400)) {
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

void btl_addEnemyRewards(Battle4880 *arg0) {
    u8 n;
    u8 it;
    Unk800FDF30 *d;

    n = 0;
    d = &g_btlRewards;
    d->unk0 += arg0->unk44;
    d->unk4 += arg0->unk46;
    for (; n < 16; n++) {
        if (d->unkC[n] == 0xFF) {
            break;
        }
    }
    if (n < 16) {
        if (rand8_inline() < 1 && (it = arg0->unk48.b[3]) != 0xFF) {
            d->unkC[n++] = it;
            arg0->unk48.b[3] = 0xFF;
        } else if (rand8_inline() < 0x20 && (it = arg0->unk48.b[2]) != 0xFF) {
            d->unkC[n++] = it;
            arg0->unk48.b[2] = 0xFF;
        } else if (rand8_inline() < 0x60 && (it = arg0->unk48.b[1]) != 0xFF) {
            d->unkC[n++] = it;
            arg0->unk48.b[1] = 0xFF;
        }
        if ((it = arg0->unk48.b[0]) != 0xFF) {
            d->unkC[n] = it;
        }
    }
    if (d->unk1C == 0xFF && (u32)arg0->unk4C < 100) {
        if (rand8_inline() < 0x20) {
            d->unk1C = arg0->unk4C;
        }
    }
}

void btl_initState(void) {
    Battle *b = g_gameState->unk1C->unk8E4.battle;
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
    b->unk11BE = 0;
    D_800FDF3A = 0;
    btl_clearRewards();
}

void btl_clearRewards(void) {
    s32 i;
    u8 *p;
    s32 c;

    c = 0xFF;
    i = 15;
    p = (u8 *)&g_btlRewards + i;
    g_btlRewards.unk0 = 0;
    g_btlRewards.unk4 = 0;
    g_btlRewards.unk8 = 0;
    for (; i >= 0; i--) {
        p[12] = c; /* unkC[i] */
        p--;
    }
    g_btlRewards.unk1C = 0xFF;
    g_btlRewards.unk1D = 0;
}

void btl_checkPartyDisabled(BtlUnit *arg0) {
    Battle *b = g_gameState->unk1C->unk8E4.battle;
    BtlUnit *u;
    BtlUnit *six = 0;
    u32 st;
    for (u = b->unitList; u != 0; u = u->next) {
        if (((u->unk68 >> 1) & 0xF) == 6) {
            six = u;
        }
        if ((u->unk68 & 1) == (arg0->unk68 & 1)) {
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
    Battle *b = g_gameState->unk1C->unk8E4.battle;
    BtlUnit *u;
    for (u = b->unitList; u != NULL; u = u->next) {
        if (!(u->unk68 & 1) && !(u->status & BTL_STATUS_KO)) {
            return;
        }
    }
    btlui_setAtbEnabled(0);
    btl_flushCmdQueue(b);
    b->escapeHeld = 0;
}

s32 btl_saveParty(Battle *arg0) {
    GameData *gd = g_gameState->unk1C;
    u8 v = gd->unk56E;
    if ((v == 3 || v == 6) && !((arg0->scene.unk0 >> 4) & 1)) {
        gd->unk56E = 6;
    } else {
        BtlUnit *u = g_gameState->unk1C->unk8E4.battle->unitList;
        while (u != NULL) {
            if (u->unk68 & 1) {
                btl_saveCharacter(u, 0);
            }
            u = u->next;
        }
    }
    return 1;
}

void btl_saveCharacter(BtlUnit *arg0, s32 arg1) {
    Character *c = &g_gameState->unk1C->characters[(arg0->unk68 >> 1) & 0xF];
    if (arg0->status & BTL_STATUS_TRANCE) {
        c->trance = 0;
    } else {
        c->trance = arg0->trance;
    }
    btl_copyStats((Unk800BD8C0 *)&c->hp, (Unk800BD8C0 *)&arg0->hp);
    if ((arg0->support[1] & 0x8000) && arg1 == 0) {
        c->unk38 = 0;
        return;
    }
    if (arg0->hp == 0) {
        btl_addStatus(arg0, BTL_STATUS_KO);
    }
    func_800C09D8((Unk800C09D8 *)c, (Unk800C09D8 *)arg0);
}

void btl_unlinkUnit(BtlUnit *arg0) {
    BtlUnit *p;
    for (p = (BtlUnit *)&g_gameState->unk1C->unk8E4.battle->unitList; p != NULL; p = p->next) {
        if (p->next == arg0) {
            p->next = arg0->next;
            return;
        }
    }
}

void btl_insertUnit(BtlUnit *arg0) {
    BtlUnit **pp = &g_gameState->unk1C->unk8E4.battle->unitList;
    BtlUnit *n;
    s32 a;
    s32 b;
    while (pp != NULL) {
        n = *pp;
        a = (u8)arg0->unk68 >> 5;
        b = (u8)n->unk68 >> 5;
        if (a < b) {
            arg0->next = n;
            *pp = arg0;
            return;
        }
        pp = &n->next;
    }
}

void btl_tryEscape(u8 arg0) {
    Battle *b = g_gameState->unk1C->unk8E4.battle;
    if (!(b->cmdFlags & 1)) {
        if ((u8)b->scene.unk0 >> 7 == 0) {
            btl_showMessage(4);
        } else if (arg0 && (btl_isAtbRunning() & 0xFF)) {
            btl_calcCmd(0, 0, 0, 0x38);
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
        for (; i < arg0->model->unk8->meshCount; i++) {
            btl_setMeshRgb(arg0->model, i, (s32)((e->lightR * arg1) << 11) >> 16, (s32)((e->lightG * arg1) << 11) >> 16, (s32)((e->lightB * arg1) << 11) >> 16);
        }
        if (arg0->weaponModel != NULL) {
            btl_setModelAbr(arg0->weaponModel, 0x60);
            for (i = 0; i < arg0->weaponModel->unk8->meshCount; i++) {
                btl_setMeshRgb(arg0->weaponModel, i, (s32)((e->lightR * arg1) << 11) >> 16, (s32)((e->lightG * arg1) << 11) >> 16, (s32)((e->lightB * arg1) << 11) >> 16);
            }
        }
        f = arg0->unk68;
        k = f >> 1;
        k &= 0xF;
        if (!(f & 1)) {
            k += 9;
        }
        g_gameState->unk1C->unk8E4.battle->unk11C0->unk28[k].shade = (s32)(btlbg_getHeader()->shadowShade * arg1) >> 5;
    }
}

u16 btl_getUnitsWithStatus(u32 arg0, s32 arg1) {
    BtlUnit *u;
    s32 r = 0;
    s32 all;
    for (u = g_gameState->unk1C->unk8E4.battle->unitList; u != 0; u = u->next) {
        all = arg1 == 0;
        if (all || (u8)btl_hasStatus(u, arg1)) {
            if ((u->unk68 >> 28) & 1) {
                switch (arg0) {
                case 0:
                    if (!(u->unk68 & 1)) {
                        break;
                    }
                    r |= u->unitBit;
                    break;
                case 1:
                    if (u->unk68 & 1) {
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
    for (u = g_gameState->unk1C->unk8E4.battle->unitList; u != NULL; u = u->next) {
        if ((u8)u->unk68 >> 5 == arg0) {
            return u;
        }
    }
    return NULL;
}

BtlUnit *btl_findBodyUnit(void) {
    BtlUnit *u;
    for (u = g_gameState->unk1C->unk8E4.battle->unitList; u != NULL; u = u->next) {
        if (!(u->unk68 & 0x800001) && (((g_gameState->unk1C->unk8E4.battle->enemies + ((u->unk68 >> 1) & 0xF))->unk4 >> 8) & 1)) {
            return u;
        }
    }
    return NULL;
}

s32 btl_hasEnemyFlag(BtlUnit *arg0, s32 arg1) {
    u32 f = arg0->unk68;
    if (f & 1) {
        return 0;
    }
    return g_gameState->unk1C->unk8E4.battle->enemies[(f >> 1) & 0xF].unk0->unk2 & arg1;
}

s32 btl_getModelSlot(BtlUnit *arg0) {
    u32 f = arg0->unk68;
    s32 v = (f >> 1) & 0xF;
    if (!(f & 1)) {
        v += 9;
    }
    return v;
}

u32 btl_countActiveUnits(u32 arg0) {
    BtlUnit *u = g_gameState->unk1C->unk8E4.battle->unitList;
    s32 n = 0;
    while (u != NULL) {
        u32 f = u->unk68;
        if ((f & 1) == arg0 && ((f >> 28) & 1) && !(u->status & BTL_STATUS_KO)) {
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
    for (u = g_gameState->unk1C->unk8E4.battle->unitList; u != NULL; u = u->next) {
        if ((u->unk68 & 1) == arg0 && !(u->status & BTL_STATUS_KO) && ((u->unk68 >> 28) & 1)) {
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
    if (!(arg0->unk56F & 0x10)) {
        btl_soundCmd(0x8601, 0, arg1, 0, 0);
        arg0->unk56F |= 0x10;
    }
    if (!(arg0->unk56F & 2)) {
        b = g_gameState->unk1C->unk8E4.battle;
        b->partyBrightness += 4;
        if ((u32)(s8)b->partyBrightness < arg1) {
            return 0;
        }
        btl_soundCmd(0x700, arg2, 0, 0, 0);
        arg0->unk56F |= 2;
    }
    if (btl_soundCmd(0xC00, 0, 0, 0, 0) != 0) {
        return 0;
    }
    if (!(arg0->unk56F & 0x20)) {
        btl_soundCmd(0, arg2, 0, 0, 0);
        arg0->unk56F |= 0x20;
    }
    return 1;
}

void btl_playHitSound(BtlUnit *arg0, BtlUnit *arg1) {
    if (!(arg1->popupFlags & 0x20)) {
        btl_playUnitSound(arg0, (u16)btl_getWeaponSound((u8)arg0->unk68 >> 5, 1), 0x7F);
    }
}

void btl_playDeathCry(BtlUnit *arg0, u16 arg1) {
    Battle *b = g_gameState->unk1C->unk8E4.battle;
    if (b->unk11BE == 0 && !((arg0->unk68 >> 27) & 1)) {
        b->unk11BE = 8;
        btl_playUnitSound(arg0, arg1, 0x7F);
        arg0->unk68 |= 0x08000000;
    }
}

void btl_playUnitSound(BtlUnit *arg0, u16 arg1, u8 arg2) {
    SVECTOR v;
    if (arg1 != 0xFFFF) {
        v.vx = arg0->pos[0] >> 16;
        v.vy = arg0->pos[1] >> 16;
        v.vz = arg0->pos[2] >> 16;
        btl_soundCmd(0xD000, arg1, 0, btl_soundPan((s32)&v), arg2);
    }
}

void btl_applyBgLight(BtlUnit9C *arg0) {
    s32 i;
    BtlBgHeader *p = btlbg_getHeader();
    for (i = 0; i < arg0->unk8->meshCount; i++) {
        btl_setMeshRgb(arg0, i & 0xFF, p->lightR, p->lightG, p->lightB);
    }
}

