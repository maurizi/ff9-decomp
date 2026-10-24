#include "ovl_0b.h"

void btl_startBattle(void) {
    Battle *b;
    Battle11C0 *p;

    b = g_gameState->unk1C->unk8E4.battle;
    p = b->unk11C0;
    btlbg_init(p->unk18, p->unk40);
    b->unkD85 |= 1;
    btl_initCmdQueue(b);
    func_800B0FC0(b);
    func_800C972C();
    g_btlRewards.unkB = (b->unk132C.unk0 >> 13) & 1;
    func_8004C204();
    func_8004BC38();
    func_800AB580(1);
    func_8002FA50(0x20);
    btlcam_playScene(func_800CA2B0(0));
    b->unkD80 = 2;
}

void btl_updateCombat(GameData *arg0, Battle *arg1) {
    BtlUnit *u;
    s32 sp10;
    u = arg1->unk6F8;
    sp10 = arg1->unk11C0->unk34[arg0->unk8];
    while (u != NULL) {
        if (((u->unk68 >> 29) & 1) && (arg1->unk11B8 & 2)) {
            btl_projectUnitPos(u);
        } else {
            u->unkBC = 0;
        }
        u = u->unk0;
    }
    u = arg1->unk6F8;
    while (u != NULL) {
        if (!((u->unk68 >> 22) & 1)) {
            btl_updateUnitModel(u, &sp10);
        }
        if (btlui_getActiveUnit() == u && (arg1->unk11B8 & 2) && !(u->unk9C->unk0 & 0x10)) {
            btl_drawActiveMarker(arg0, arg1, u);
        }
        btl_updateHpStatus(u);
        btl_updateUnitStatus(u);
        u = u->unk0;
    }
    btl_processCmdQueue(arg1);
    btl_drawScene(arg0, arg1, sp10);
}

void btl_drawScene(GameData *arg0, Battle *arg1, s32 arg2) {
    if (arg1->unk11BE) {
        arg1->unk11BE -= 1;
    }
    btlseq_update();
    arg2 = (s32)btlbg_draw(&arg0->unk18, arg1->unk11C0->unk1C, arg2, arg0->unk10[arg0->unk8],
                              arg1->unk11C0->unk2C[arg0->unk8]);
    arg2 = (s32)btlfx_runFrame((u8 *)arg2);
    arg2 = (s32)btl_drawPopups((u8 *)arg2);
    btl_drawBanner((u8 *)arg2);
}

s32 btl_tryCounter(BtlUnit *arg0, BtlUnit *arg1, BtlCmd *arg2) {
    s32 r;
    if (arg0->status & 0x42001103) {
        return 0;
    }
    if (arg2->cmdNo >= 0x30) {
        return 0;
    }
    if ((arg0->unkC4[1] & 0x10) && (arg2->ability->unk8 & 8)) {
        r = arg0->unk33;
        if (arg0->unkC4[1] & 0x80) {
            r *= 2;
        }
        if (g_randPtr >= g_randTableEnd) {
            rand_refill();
            if (g_randPtr >= g_randTableEnd) {
                rand_refill();
            }
        }
        if (((*g_randPtr << 8) | *g_randPtr++) % 100 < r) {
            btl_queueAutoCmd(arg0, 0x31, 0xB0, arg1->unkB0);
            return 1;
        }
    }
    if ((arg0->unkC4[1] & 0x400000) && (arg2->ability->unk8 & 0x80)) {
        btl_queueAutoCmd(arg0, 0x32, arg2->subNo, arg1->unkB0);
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
    if (t->status & 0x101) {
        return 0;
    }
    g = g_gameState->unk1C;
    if (((g->characters + ((t->unk68 >> 1) & 0xF))->unkA & 2) && t->hp < (t->max->maxHp >> 1)) {
        for (u = g->unk8E4.battle->unk6F8; u != NULL; u = u->unk0) {
            if ((u->unkC4[1] & 0x40) && !(u->status & 0x43001103) && u != t && (best == NULL || best->hp < u->hp)) {
                best = u;
            }
        }
    }
    if (best == NULL && (t->status & BTL_STATUS_LOW_HP)) {
        for (u = g_gameState->unk1C->unk8E4.battle->unk6F8; u != NULL; u = u->unk0) {
            if ((u->unkC4[1] & 0x20) && !(u->status & 0x43001103) && u != t && (best == NULL || best->hp < u->hp)) {
                best = u;
            }
        }
    }
    if (best == NULL) {
        return 0;
    }
    v = (best->unk68 & 1) << 11;
    best->unkA0->unk22 = v;
    best->unk86[1] = v;
    best->unk8C[0] = t->unk8C[0];
    best->unk8C[2] = t->unk8C[2];
    t->unk8C[2] += 0xFE700000;
    best->unk9C->unkC = (BtlUnit9CC *)best->unkA4[15];
    best->unk68 |= 0x2000000;
    return best->unkB0;
}

void btl_applyAutoAbilities(BtlUnit *arg0) {
    if (arg0->unkC4[0] & 1) {
        arg0->unk38 |= 0x20000000;
    }
    if (arg0->unkC4[0] & 2) {
        arg0->unk38 |= 0x200000;
    }
    if (arg0->unkC4[0] & 4) {
        arg0->unk38 |= 0x80000;
        arg0->statusImmune |= 0x100000;
        arg0->unk2E = arg0->unk2E * 3 / 2;
    }
    if (arg0->unkC4[0] & 8) {
        arg0->unk38 |= BTL_STATUS_REGEN;
        btl_initDotTimer(arg0, 0x12);
    }
    if (arg0->unkC4[0] & 0x10) {
        arg0->status |= 0x2000;
    }
    if (arg0->unkC4[1] & 0x100) {
        arg0->statusImmune |= 0x03000000;
    }
    if (arg0->unkC4[1] & 0x10000) {
        arg0->statusImmune |= 0x20000;
    }
    if (arg0->unkC4[1] & 0x20000) {
        arg0->statusImmune |= 0x10002;
    }
    if (arg0->unkC4[1] & 0x40000) {
        arg0->statusImmune |= 0x10;
    }
    if (arg0->unkC4[1] & 0x80000) {
        arg0->statusImmune |= 8;
    }
    if (arg0->unkC4[1] & 0x200000) {
        arg0->statusImmune |= 0x80000001;
    }
    if (arg0->unkC4[1] & 0x02000000) {
        arg0->statusImmune |= 0x1000;
    }
    if (arg0->unkC4[1] & 0x04000000) {
        arg0->statusImmune |= 0x400;
    }
}

s32 btl_partyHasSupport(s32 arg0, u32 arg1) {
    s32 i;
    GameData *gd = g_gameState->unk1C;
    for (i = 0; i < 4; i++) {
        if (gd->party[i]->unk88[arg0] & arg1) {
            return 1;
        }
    }
    return 0;
}

void btl_tryAutoPotion(BtlUnit *arg0, u8 arg1) {
    s32 i;
    if (!(arg0->status & 0x42001103) && arg1 < 0x30 && (arg0->unkC4[1] & 0x01000000)) {
        for (i = 0xEC; (u8)i < 0xEE; i++) {
            if (item_getCount((u8)i) != 0) {
                func_800EA144((u8)i);
                btl_queueAutoCmd(arg0, 0x33, (u8)i, arg0->unkB0);
                return;
            }
        }
    }
}

void func_800B0E04(BtlUnit *arg0, u8 *arg1) {
    u32 max;
    u32 lt;
    s32 h;
    if (!(arg0->status & 0x42001103)) {
        if ((arg0->unkC4[1] & 0x100000) && arg0->hp != 0 && (arg0->status & BTL_STATUS_LOW_HP)) {
            max = arg0->max->maxHp;
            h = arg0->hp;
            lt = (s32)(h + (max >> 1)) < (s32)max;
            if (lt) {
                arg0->hp = h + (max >> 1);
            } else {
                arg0->hp = max;
            }
        }
        if ((arg0->unkC4[1] & 0x800000) && arg1[0xA] != 0) {
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
    arg0->unk11BA = 0;
    arg0->unk11B8 = 2;
    arg0->unk1060.unk4 = NULL;
    arg0->cmdQueue.unk4 = NULL;
    btl_clearCmd_inline(&arg0->cmdQueue);
    btl_clearCmd_inline(&arg0->unk1060);
}

void func_800B0FC0(Battle *arg0) {
    s8 i;
    s8 j;
    s32 c;
    for (i = 0; i < 2; i++) {
        for (j = 0; j < 4; j++) {
            func_80063640(&arg0->unk108C[i].unk0[j].unk0, GetTPage(0, 1, 0, 0));
            if (j < 3) {
                c = 0xBC - j * 0x3C;
            } else {
                c = 0xA0;
            }
            setRGB0(&arg0->unk108C[i].unk0[j].unk8, c, c, 0);
            SetSemiTrans(&arg0->unk108C[i].unk0[j].unk8, 1);
        }
    }
}

void btl_initUnitCmds(BtlUnit *arg0) {
    Battle *b = g_gameState->unk1C->unk8E4.battle;
    BtlCmd *c;
    s32 k;
    s32 i;
    s32 j;
    if (b->unkD80 == 2) {
        arg0->unkB2 = 1;
    }
    arg0->unkB3 = 0;
    arg0->unkBC = 0;
    if (arg0->unk68 & 1) {
        k = ((arg0->unk68 >> 5) & 7) * 6;
    } else {
        k = (((arg0->unk68 >> 5) & 7) - 4) * 3 + 0x18;
    }
    c = &b->unkD90[k];
    /* unk4..unk18 are the unit's six command slots */
    for (i = 0; i < 6; i++) {
        if ((arg0->unk68 & 1) || i < 3) {
            (&arg0->unk4)[i] = c;
            c->unk4 = arg0;
            c++;
            btl_clearCmd_inline((&arg0->unk4)[i]);
        } else {
            (&arg0->unk4)[i] = NULL;
        }
    }
    for (j = 3; j >= 0; j--) {
        arg0->unk1C[j] = 0;
    }
}

void btl_setCmd(BtlCmd *arg0, u32 arg1, s32 arg2, s32 arg3, s32 arg4) {
    BtlUnit *u = arg0->unk4;
    BtlUnit9C *m;
    s32 *a;
    BtlUnit9CC *c;

    switch (arg1) {
    case 10:
    case 11:
        g_gameState->unk1C->unk8E4.battle->unk11B8 |= 0x10;
        arg0->ability = (BtlCmd8 *)&g_gameState->unk1C->unk8E4.battle->abilities[arg2];
        break;
    case 15:
        arg0->ability = (BtlCmd8 *)&g_gameState->unk1C->unk8E4.battle->abilities[0xBE];
        break;
    case 0x32:
        arg0->ability = &g_gameState->unk1C->unk8E4.battle->unk5D8[arg2];
        break;
    case 0x38:
        if (g_gameState->unk1C->unk8E4.battle->unk11B8 & 1) {
            arg0->subNo = arg2;
            return;
        }
        g_gameState->unk1C->unk8E4.battle->unk11B8 |= 1;
        arg0->ability = (BtlCmd8 *)&g_gameState->unk1C->unk8E4.battle->abilities[0xB4];
        break;
    case 14:
    case 0x33:
    case 0x3C:
    case 0x3D:
    case 0x3E:
        arg0->ability = (BtlCmd8 *)&g_gameState->unk1C->unk8E4.battle->abilities[0];
        break;
    case 2:
        if (u->unkC4[1] & 0x20000000) {
            arg2 = 0xB5;
        }
    default:
        arg0->ability = (BtlCmd8 *)&g_gameState->unk1C->unk8E4.battle->abilities[arg2];
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
        if (g_gameState->unk1C->unk8E4.battle->curCmd != u->unk14) {
            m = u->unk9C;
            a = u->unkA4;
            c = m->unkC;
            if (c == (BtlUnit9CC *)a[0]) {
                m->unkC = (BtlUnit9CC *)a[10];
                u->unkA0->unk3A = 0;
            } else if (c == (BtlUnit9CC *)a[1]) {
                m->unkC = (BtlUnit9CC *)a[11];
                u->unkA0->unk3A = 0;
            } else if (c == (BtlUnit9CC *)a[13] && arg1 < 0x30) {
                m->unkC = (BtlUnit9CC *)a[14];
                u->unkA0->unk3A = 0;
            }
        }
        u->unk68 |= 0x80000;
    }
    btl_enqueueCmd(arg0);
}

void btl_setEnemyCmd(s32 arg0, s16 arg1, u32 arg2, u32 arg3) {
    Battle *b;
    BtlUnit *u;
    BtlCmd *c;

    u = btl_getUnitById(arg0);
    b = g_gameState->unk1C->unk8E4.battle;
    if (b->unk11B8 & 1 || (u->status & 0x02021103)) {
        goto fail;
    }
    if (arg2 == 0x2F) {
        if (b->unkD80 != 4) {
            goto fail;
        }
        c = u->unk4;
        if (u->status & 0x400) {
            arg1 = btl_pickRandomUnit(RAND8() & 1);
            arg3 = g_gameState->unk1C->unk8E4.battle->unk488[(u->unk68 >> 1) & 0xF].unk0->unk26;
        } else if (u->status & 0x800) {
            arg1 = btl_pickRandomUnit(1);
            arg3 = g_gameState->unk1C->unk8E4.battle->unk488[(u->unk68 >> 1) & 0xF].unk0->unk26;
        }
    } else {
        if (arg2 != 0x35) {
            if (arg2 != 0x36) {
fail:
                u->unkB3 = 0;
                return;
            }
        }
        c = u->unk8;
    }
    c->ability = &g_gameState->unk1C->unk8E4.battle->unk5D8[arg3];
    c->targets = arg1;
    c->cmdNo = arg2;
    c->subNo = arg3;
    if ((u32)(c->ability->unk0_0 - 6) < 7) {
        c->unk10.b.unk0_0 = 1;
    } else {
        c->unk10.b.unk0_0 = 0;
    }
    if (arg2 > 0x30) {
        c->unk10.b.unk0_4 = 1;
    } else {
        btl_removeStatus(c->unk4, BTL_STATUS_DEFEND);
    }
    c->unk10.b.unk0_5 = 0;
    c->unk10.b.unk0_6 = 0;
    c->unk10.b.unk0_7 = 0;
    u->unk68 |= 0x80000;
    btl_enqueueCmd(c);
}

void btl_processCmdQueue(Battle *b) {
    BtlCmd *cmd;
    BtlUnit *u;
    BtlUnit *w;
    BtlCmd *p;
    u8 k;

    if (b->curCmd == NULL) {
        if ((p = b->cmdQueue.unk0) == NULL) {
            return;
        }
        for (cmd = p; cmd != NULL; cmd = cmd->unk0) {
            if (!((cmd->cmdNo < 0x3C && cmd->cmdNo != 0x38 && cmd->cmdNo != 0x3A && (cmd->unk4->status & 0x2001003)) ||
                  (cmd->cmdNo == 0x39 && (cmd->unk4->status & BTL_STATUS_KO)))) {
                break;
            }
        }
        if (cmd == NULL) {
            return;
        }
        if (!btl_isAtbRunning() && cmd->cmdNo < 0x30) {
            return;
        }
        k = cmd->cmdNo;
        if (k < 0x37) {
            w = cmd->unk4;
            if ((u32)(k - 0x31) < 4) {
                w->unk9C->unkC = (BtlUnit9CC *)w->unkA4[9];
                w->unkA0->unk3A = 0;
            }
            if ((w->unk68 & 1) && (s32)w->unk9C->unkC != w->unkA4[9] && (s32)w->unk9C->unkC != w->unkA4[17] &&
                !(w->status & 0x40000000)) {
                if ((s32)w->unk9C->unkC == w->unkA4[(w->unk68 >> 18) & 1] && !((w->unk68 >> 19) & 1)) {
                    w->unk9C->unkC = (BtlUnit9CC *)w->unkA4[9];
                    w->unkA0->unk3A = 0;
                }
                return;
            }
            if (w->status & 0x1000000) {
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
    u = cmd->unk4;
    switch (b->unk11BA) {
    case 0:
        if (!(u8)btl_checkCmdUsable(b, cmd) || !(u8)btl_validateTargets(b, cmd) || !(u8)btl_payMpCost(cmd)) {
            btl_releaseItem(cmd);
            if (u != NULL && (u->unk68 & 1) && (s32)u->unk9C->unkC == u->unkA4[9]) {
                u->unk9C->unkC = (BtlUnit9CC *)u->unkA4[32];
            }
            b->unk11BA = 3;
            break;
        }
        if (u != NULL && (s32)u->unk9C->unkC != u->unkA4[9] && cmd->cmdNo < 0x2F) {
            u->unk9C->unkC = (BtlUnit9CC *)u->unkA4[9];
            u->unkA0->unk3A = 0;
        }
        b->unk11BA++;
        break;
    case 1:
        btl_startCmdEffect(cmd);
        b->unk11BA++;
        break;
    case 2:
        if ((u->unk68 & 1) || cmd->unk10.b.unk0_11) {
            btl_updatePartyCmd(cmd);
        }
        break;
    case 3:
        if (cmd->cmdNo != 0x38) {
            btl_onCmdEffectDone();
        } else {
            btl_finishCmd(b);
        }
        break;
    }
    if ((u8)btl_waitHitMotions(cmd) && b->unk11BA == 4) {
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

    u = arg1->unk4;
    if (u != NULL) {
        if (arg1->cmdNo < 0x37) {
            v = (u->unk68 & 1) << 11;
            u->unkA0->unk22 = v;
            u->unk86[1] = v;
        }
        if (D_8007AF01 != 0 && (u->unk68 & 0x1F) == 5 && arg1->cmdNo < 0x30) {
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
    case 1:
        if (((arg0->unk132C.unk0 >> 8) & 1) && (*u->unk74 & 1) && (u16)arg1->targets >= 0x10) {
            msg = 0x17;
            goto fail;
        }
        return 1;
    case 3:
    case 0xC:
        btl_addStatus(u, BTL_STATUS_JUMP);
        u->unk10->cmdNo = arg1->cmdNo;
        m = u->unk10;
        if (arg1->cmdNo == 3) {
            m->targets = arg1->targets;
        } else {
            m->targets = btl_getUnitsWithStatus(1, 0);
        }
        arg1->targets = u->unkB0;
        break;
    case 0xB:
        u->unk10->targets = btl_getUnitsWithStatus(1, 0);
        break;
    case 0xE:
    case 0xF:
    case 0x33:
        if (item_getCount(arg1->subNo) == 0) {
            msg = 0x18;
            goto fail;
        }
        if (arg1->cmdNo == 0x33) {
            btl_showMessage(9);
        }
        return 1;
    case 0x10:
    case 0x12:
    case 0x14:
        btl_rollShortSummon(arg1);
        return 1;
    case 0x16:
    case 0x17:
        if (arg1->subNo == 0x2E) {
            func_800B95BC(arg1);
        }
        return 1;
    case 0x18:
        if (arg1->subNo == 0x52) {
            arg1->targets = btl_pickRandomUnit(evt_rollEncounter_rand() & 1);
        } else if (arg1->subNo == 0x5D) {
            a = item_getCount(arg1->ability->unk5);
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
    case 0x1C:
    case 0x1D:
        if (arg1->subNo == 0x7E || arg1->subNo == 0x86) {
            g = arg1->ability->unk5 * u->unk7A;
            if (g_gameState->unk1C->gil < g) {
                msg = 0x14;
                goto fail;
            }
            g_gameState->unk1C->gil -= g;
        } else if (arg1->subNo == 0x81 || arg1->subNo == 0x89) {
            c = arg1->ability;
            r = evt_rollEncounter_rand();
            c->unk6 = 1 << (r % 8);
        } else {
            return 1;
        }
        break;
    case 0x1F:
        return (u8)func_800B96AC(u, arg1->ability->mpCost);
    case 0x31:
        t = btl_getUnitById(arg1->targets);
        if (t == NULL || !((t->unk68 >> 28) & 1) || (t->status & BTL_STATUS_KO)) {
            goto ret0;
        }
    case 0x34:
        if ((arg0->unk132C.unk0 >> 8) & 1) {
            msg = 0x17;
            if (*u->unk74 & 1) {
            fail:
                btl_showMessage(msg);
                return 0;
            }
        }
        if (arg1->cmdNo == 0x31) {
            btl_showMessage(7);
        }
        return 1;
    case 0x32:
        t = btl_getUnitById(arg1->targets);
        if (t != NULL && ((t->unk68 >> 28) & 1) && !(t->status & BTL_STATUS_KO)) {
            btl_showMessage(8);
            return 1;
        }
    ret0:
        return 0;
    case 0x38:
        if (arg0->unkD80 != 4) {
            return 1;
        }
        for (t = arg0->unk6F8; t != NULL; t = t->unk0) {
            if ((t->unk68 & 1) && !(t->status & 0x42021143) && t->hp != 0) {
                if (t->unk9C->unkC != (BtlUnit9CC *)t->unkA4[0x11]) {
                    t->unk9C->unkC = (BtlUnit9CC *)t->unkA4[0x11];
                    t->unkA0->unk3A = 0;
                }
                arg0->unkD80 = 5;
                arg0->unkD81 = 3;
            }
        }
        if (*(u16 *)&arg0->unkD80 == 0x305) {
            func_800E9F68(0);
            g_gameState->unk1C->unk55C++;
            if (arg1->subNo == 0) {
                g_gameState->unk1C->unk56F |= 4;
            }
            btl_clearCmdQueue(arg0);
        } else {
            arg0->unk11B8 &= ~1;
        }
        return 0;
    case 0x39:
        if (arg1->subNo != arg0->unk11BB) {
            arg1->subNo = arg0->unk11BB;
            arg1->ability = (BtlCmd8 *)&arg0->abilities[arg1->subNo];
        }
        break;
    case 0x3B:
        if (u->status & 0x4000) {
            btl_showMessage(6);
            u->unk84 = D_800F5200[(u->unk68 & 1) ? g_gameState->unk1C->characters[(u->unk68 >> 1) & 0xF].unk34.b.unk4 + 0x13 : 0x26];
        } else {
            u->unk84 = D_800F5200[(u->unk68 & 1) ? (g_gameState->unk1C->characters[(u->unk68 >> 1) & 0xF].unk34.w >> 4) & 0x1F : 0x13];
        }
        break;
    case 0x3C:
        btl_checkPartyDisabled(u);
        if (u->unk7D == 0) {
            if (u->unk68 & 1) {
                if (u->unk9C->unkC == (BtlUnit9CC *)u->unkA4[4]) {
                    u->unk7D = 5;
                    return 0;
                }
                u->unk7D = 1;
            } else if ((u->unk68 & 0x900000) == 0x100000) {
                u->unk7D = 3;
            } else {
                u->unk7D = 1;
            }
        }
        return 0;
    case 0x3D:
        u->hp = 1;
        btl_removeStatus(u, BTL_STATUS_KO);
        u->unk68 |= 0x20000;
        return 0;
    case 0x3E:
        btl_interruptUnit(u, 1);
        u->status |= 1;
        u->unk68 &= ~0x10000;
        btl_checkSideDefeated(u);
        if (u->unk68 & 1) {
            func_800C0544(u);
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
        case 0xE:
        case 0x33:
            tbl = D_8006B1F4;
            i = cmd->subNo - 0xE0;
            a = (BtlCmd8 *)(tbl + i);
            f = (*(u32 *)a >> 29) & 1;
            break;
        case 0x3B:
            return 1;
        default:
            a = cmd->ability;
            f = (*(u32 *)a >> 29) & 1;
            break;
        }
        for (u = b->unk6F8; u != NULL; u = u->unk0) {
            if (((u->unk68 >> 28) & 1) && (u->unkB0 & (u16)cmd->targets) &&
                ((f && (u->unk68 & 1)) || !(u->status & BTL_STATUS_KO))) {
                mask |= u->unkB0;
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

    if (g_btlfxEnv->unk0->unk0 != -1) {
        return;
    }
    b = g_gameState->unk1C->unk8E4.battle;
    u = arg0->unk4;
    if (b->unk1358.unk1C4 != 0 || b->unk1358.unk1CC != NULL) {
        return;
    }
    if (arg0->cmdNo == 4) {
        m = u->unk9C;
        a = u->unkA4;
        if (m->unkC == (BtlUnit9CC *)a[12]) {
            if (u->unkA0->unk3A < m->unkC->unk2) {
                return;
            }
            m->unkC = (BtlUnit9CC *)a[13];
            u->unkA0->unk3A = 0;
        }
    } else if (arg0->cmdNo == 7) {
        m = u->unk9C;
        a = u->unkA4;
        if (m->unkC == (BtlUnit9CC *)a[9]) {
            if (u->unkA0->unk3A < m->unkC->unk2) {
                return;
            }
            m->unkC = (BtlUnit9CC *)a[29 + ((u->unk68 >> 8) & 3)];
            u->unkA0->unk3A = 0;
        }
        a2 = u->unkA4;
        m2 = u->unk9C;
        k = (u->unk68 >> 8) & 3;
        c = m2->unkC;
        if (c == (BtlUnit9CC *)a2[29 + k]) {
            n = c->unk2;
            f = u->unkA0->unk3A;
            if (f < n) {
                f++;
                if (k != 0) {
                    u->unk8C[2] = (-(f * 400) / n - 0x618) << 16;
                } else {
                    u->unk8C[2] = ((f * 400) / n - 0x7A8) << 16;
                }
                return;
            }
            btl_execCmd(u);
            u->unk9C->unkC = (BtlUnit9CC *)u->unkA4[32];
            u->unkA0->unk3A = 0;
            return;
        }
        if (c == (BtlUnit9CC *)a2[32]) {
            if (u->unkA0->unk3A < c->unk2) {
                return;
            }
            m2->unkC = (BtlUnit9CC *)a2[0];
            u->unkA0->unk3A = 0;
        }
    }
    btl_onCmdEffectDone();
    if (arg0->cmdNo == 0x3B) {
        func_800C0D24(u);
    }
}

void btl_execCmd(BtlUnit *arg0) {
    BtlCmd *cmd = g_gameState->unk1C->unk8E4.battle->curCmd;
    BtlUnit *u = cmd->unk4;
    Battle *b;
    u16 s;
    u16 t;
    s32 i;
    Unk8006B1F4 *tbl;

    switch (cmd->cmdNo) {
    case 0x1:
    case 0x31:
    case 0x34:
        btl_calcCmd(u, arg0, cmd, ((BtlCmd8 *)u->unk74)->unk4);
        break;
    case 0x3:
    case 0xC:
        u->unkB2 = 2;
        u->unk68 |= 0x400000;
        break;
    case 0xE:
    case 0x33:
        func_800EA0F8(cmd->subNo);
        tbl = D_8006B1F4;
        i = cmd->subNo - 0xE0;
        btl_calcCmd(u, arg0, cmd, ((BtlCmd8 *)(tbl + i))->unk4);
        break;
    case 0x3A:
        s = btl_getUnitMask(1);
        t = btl_getUnitsWithStatus(1, 0x1103);
        if (s == 0 || s == t) {
            b = g_gameState->unk1C->unk8E4.battle;
            func_800E9F68(0);
            if (b->unkD80 != 5) {
                b->unkD80 = 5;
                b->unkD81 = 0;
                btl_clearCmdQueue(b);
            }
        }
        btl_calcCmd(u, arg0, cmd, cmd->ability->unk4);
        break;
    case 0xF:
        func_800EA0F8(cmd->subNo);
    default:
        btl_calcCmd(u, arg0, cmd, cmd->ability->unk4);
        break;
    }
}

void btl_finishCmd(Battle *b) {
    BtlCmd *cmd = b->curCmd;
    BtlUnit *u = cmd->unk4;
    u32 n;

    if (cmd->cmdNo < 0x30) {
        if ((cmd == u->unk4 && cmd->cmdNo != 3 && cmd->cmdNo != 0xC) ||
            (cmd->cmdNo == 0x15 && ((u->unk68 >> 1) & 0xF) == 6 && cmd == u->unk10 && !btl_isCmdPending(u->unk4))) {
            u->atb = 0;
            u->unkB3 = 0;
        } else if (cmd->cmdNo == 0xA) {
            btl_removeStatus(u, BTL_STATUS_JUMP);
            g_gameState->unk1C->unk8E4.battle->unk11B8 &= ~0x10;
        } else if (cmd->cmdNo == 0xB) {
            btl_addStatus(u, BTL_STATUS_JUMP);
            u->unkB2 = 2;
            u->unk68 |= 0x400000;
            g_gameState->unk1C->unk8E4.battle->unk11B8 &= ~0x10;
        }
        if ((u->status & 0x4000) && cmd->cmdNo != 3 && cmd->cmdNo != 0xC) {
            n = (0x12C - u->unk7A) / u->unk33 * 10;
            if (cmd->cmdNo == 0x15 || cmd->cmdNo == 0x17) {
                n = (u8)n >> 1;
            }
            if ((u8)n < u->unk78) {
                u->unk78 -= n;
            } else {
                btl_removeStatus(u, BTL_STATUS_TRANCE);
            }
            if (cmd->cmdNo == 0x12 && b->unk11BB != 0) {
                b->unk11B8 |= 4;
                b->unk11BC = (0x3C - u->unk33) * 200;
            }
        }
    } else if (cmd->cmdNo == 0x39) {
        b->unk11B8 &= ~8;
        b->unk11BC = (0x3C - u->unk33) * 200;
    } else if (cmd->cmdNo < 0x35 && btl_isCmdPending(u->unk4)) {
        u->unk68 |= 0x80000;
    }
    if (cmd->unk10.b.unk0_5) {
        for (u = g_gameState->unk1C->unk8E4.battle->unk6F8; u != NULL; u = u->unk0) {
            if (u->unk68 & 1) {
                u->unk8C[0] = u->unk98->unk0[0];
                u->unk8C[1] = u->unk98->unk0[1];
                u->unk8C[2] = u->unk98->unk0[2];
                if ((u->unk68 >> 25) & 1) {
                    u->unk68 &= ~0x2000000;
                    btl_setIdleMotion(u);
                }
            }
        }
    }
    if (cmd->unk10.b.unk0_6) {
        u = btl_getUnitById(cmd->targets);
        u->unk68 &= ~0x4000000;
        if (u->unk68 & 1) {
            btl_setIdleMotion(u);
        } else if ((u->unk68 >> 23) & 1) {
            u = func_800C1DEC();
        }
        u->unk8C[2] = u->unk98->unk0[2];
    }
    btl_clearCmd_inline(cmd);
    b->curCmd = NULL;
    b->unk11BA = 0;
}

void btl_flushCmdQueue(Battle *arg0) {
    BtlCmd *p = &arg0->cmdQueue;
    BtlCmd *c;
    BtlUnit *u;
    BtlUnit9C *m;
    Battle *b;
    s32 id;
    while (p != NULL) {
        c = p->unk0;
        if (c != NULL && c->cmdNo != 0x36 && c->cmdNo < 0x3A) {
            u = c->unk4;
            b = g_gameState->unk1C->unk8E4.battle;
            if (c->cmdNo < 0x30 && u != NULL && !(u->status & BTL_STATUS_KO)) {
                m = u->unk9C;
                u->unkB3 = 0;
                if ((s32)m->unkC == u->unkA4[9] && ((id = (u->unk68 >> 1) & 0xF) != 6 || b->curCmd->cmdNo != 0x15) && (id != 1 || (b->curCmd->cmdNo != 0x1F && b->curCmd->cmdNo != 0x17))) {
                    m->unkC = (BtlUnit9CC *)u->unkA4[32];
                    u->unkA0->unk3A = 0;
                    u->unk68 &= ~0x80000;
                }
            }
            btl_releaseItem(c);
            btl_unlinkCmd(&p->unk0, 1);
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
        c = p->unk0;
        if (c != NULL && c->cmdNo < 0x3B) {
            u = c->unk4;
            b = g_gameState->unk1C->unk8E4.battle;
            if (c->cmdNo < 0x30 && u != NULL && !(u->status & BTL_STATUS_KO)) {
                m = u->unk9C;
                u->unkB3 = 0;
                if ((s32)m->unkC == u->unkA4[9] && ((id = (u->unk68 >> 1) & 0xF) != 6 || b->curCmd->cmdNo != 0x15) && (id != 1 || (b->curCmd->cmdNo != 0x1F && b->curCmd->cmdNo != 0x17))) {
                    m->unkC = (BtlUnit9CC *)u->unkA4[32];
                    u->unkA0->unk3A = 0;
                    u->unk68 &= ~0x80000;
                }
            }
            btl_releaseItem(c);
            btl_unlinkCmd(&p->unk0, 1);
        } else {
            p = c;
        }
    }
    arg0->unk11BC = 0;
    arg0->unk11BB = 0;
    arg0->unk11B8 &= ~0xC;
}

void btl_enqueueCmd(BtlCmd *arg0) {
    BtlCmd *p = &g_gameState->unk1C->unk8E4.battle->cmdQueue;
    BtlCmd *n;
    s32 c;
    u32 e;

    for (; p != NULL; p = p->unk0) {
        if (arg0->unk10.b.unk0_4) {
            n = p->unk0;
            if (n == NULL) {
                goto insert;
            }
            if (arg0->cmdNo >= 0x3C || arg0->cmdNo == 0x36) {
                e = n->cmdNo;
                c = e < 0x3C;
                goto chk;
            }
            if (arg0->cmdNo == 0x3B) {
                e = n->cmdNo;
                c = e < 0x3B;
            chk:
                if (c == 0 || e == 0x36) {
                    continue;
                }
            insert:
                btl_linkCmd((LinkNode *)arg0, (LinkNode **)&p->unk0);
                return;
            }
            c = n->unk10.b.unk0_4;
        } else {
            c = (s32)p->unk0;
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

    if ((u32)(arg0->cmdNo - 14) >= 2 && arg0->cmdNo != 0x33 && (arg0->ability->unk8 & 1) &&
        !(arg0->unk4->unkC4[0] & 0x40000000)) {
        n = 0;
        m = 0;
        v = g_gameState->unk1C->unk8E4.battle->unk6F8;
        side = (u16)arg0->targets >= 0x10;
        for (; v != NULL; v = v->unk0) {
            if (v->unkB0 & (u16)arg0->targets) {
                if (!(v->status & 1) && (u8)btl_hasStatus(v, BTL_STATUS_REFLECT)) {
                    m++;
                    mask |= v->unkB0;
                }
            } else if (!(v->status & BTL_STATUS_KO) && (v->unk68 & 1) == side && ((v->unk68 >> 28) & 1)) {
                buf[n++] = v->unkB0;
            }
        }
        if (m != 0 && n != 0) {
            for (i = 0; i < 4; i++) {
                if (i < m) {
                    arg0->unk4->unk1C[i] = buf[RAND8() % n];
                } else {
                    arg0->unk4->unk1C[i] = 0;
                }
            }
            if ((u16)mask == (u16)arg0->targets) {
                arg0->unk10.b.unk0_7 = 1;
                arg0->targets = btl_orTargets(arg0->unk4->unk1C);
            } else {
                arg0->unk10.b.unk0_7 = 2;
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

    ang = (arg1->unkD7C & 0xF) << 8;
    gte_SetRotMatrix(&arg0->unk18);
    gte_SetTransMatrix(&arg0->unk18);
    v.vx = arg2->unk8C[0] >> 16;
    v.vy = (arg2->unk8C[1] >> 16) - arg2->unk9C->unk8->unk8;
    v.vz = arg2->unk8C[2] >> 16;
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
        (arg1->unk108C + arg0->unk8)->unk0[0].unk8.x0 = sxy[0];
        (arg1->unk108C + arg0->unk8)->unk0[0].unk8.y0 = sxy[0] >> 16;
        (arg1->unk108C + arg0->unk8)->unk0[0].unk8.x1 = sxy[1];
        (arg1->unk108C + arg0->unk8)->unk0[0].unk8.y1 = sxy[1] >> 16;
        (arg1->unk108C + arg0->unk8)->unk0[0].unk8.x2 = sxy[2];
        (arg1->unk108C + arg0->unk8)->unk0[0].unk8.y2 = sxy[2] >> 16;
        addPrim(&arg0->unk10[arg0->unk8][2], &(arg1->unk108C + arg0->unk8)->unk0[0]);
    }
    if (NormalClip(sxy[0], sxy[2], sxy[3]) > 0) {
        (arg1->unk108C + arg0->unk8)->unk0[1].unk8.x0 = sxy[0];
        (arg1->unk108C + arg0->unk8)->unk0[1].unk8.y0 = sxy[0] >> 16;
        (arg1->unk108C + arg0->unk8)->unk0[1].unk8.x1 = sxy[2];
        (arg1->unk108C + arg0->unk8)->unk0[1].unk8.y1 = sxy[2] >> 16;
        (arg1->unk108C + arg0->unk8)->unk0[1].unk8.x2 = sxy[3];
        (arg1->unk108C + arg0->unk8)->unk0[1].unk8.y2 = sxy[3] >> 16;
        addPrim(&arg0->unk10[arg0->unk8][2], &(arg1->unk108C + arg0->unk8)->unk0[1]);
    }
    if (NormalClip(sxy[0], sxy[3], sxy[1]) > 0) {
        (arg1->unk108C + arg0->unk8)->unk0[2].unk8.x0 = sxy[0];
        (arg1->unk108C + arg0->unk8)->unk0[2].unk8.y0 = sxy[0] >> 16;
        (arg1->unk108C + arg0->unk8)->unk0[2].unk8.x1 = sxy[3];
        (arg1->unk108C + arg0->unk8)->unk0[2].unk8.y1 = sxy[3] >> 16;
        (arg1->unk108C + arg0->unk8)->unk0[2].unk8.x2 = sxy[1];
        (arg1->unk108C + arg0->unk8)->unk0[2].unk8.y2 = sxy[1] >> 16;
        addPrim(&arg0->unk10[arg0->unk8][2], &(arg1->unk108C + arg0->unk8)->unk0[2]);
    }
    if (NormalClip(sxy[1], sxy[3], sxy[2]) > 0) {
        (arg1->unk108C + arg0->unk8)->unk0[3].unk8.x0 = sxy[1];
        (arg1->unk108C + arg0->unk8)->unk0[3].unk8.y0 = sxy[1] >> 16;
        (arg1->unk108C + arg0->unk8)->unk0[3].unk8.x1 = sxy[3];
        (arg1->unk108C + arg0->unk8)->unk0[3].unk8.y1 = sxy[3] >> 16;
        (arg1->unk108C + arg0->unk8)->unk0[3].unk8.x2 = sxy[2];
        (arg1->unk108C + arg0->unk8)->unk0[3].unk8.y2 = sxy[2] >> 16;
        addPrim(&arg0->unk10[arg0->unk8][2], &(arg1->unk108C + arg0->unk8)->unk0[3]);
    }
}

void btl_autoAttack(BtlUnit *arg0) {
    s32 x;
    if (arg0->status & 0x400) {
        if (g_randPtr >= g_randTableEnd) {
            rand_refill();
        }
        x = *g_randPtr++;
        x &= 1;
    } else if (arg0->status & 0x800) {
        x = 0;
    } else {
        return;
    }
    btl_setCmd((s32)arg0->unk4, 1, 0xB0, btl_pickRandomUnit(x) & 0xFFFF, 0);
}

void btl_onCmdEffectDone(void) {
    Battle *b = g_gameState->unk1C->unk8E4.battle;
    BtlCmd *c = b->curCmd;
    s8 k = c->unk10.b.unk0_7;
    BtlUnit *u;
    if (k == 2) {
        c->targets = btl_orTargets(c->unk4->unk1C);
        btl_playCmdEffect(c, c->ability->unk0_8, 0);
        b->unk11BA = k;
        return;
    }
    u = c->unk4;
    if (!(u->status & 0x2000) || c->cmdNo != 0x3C || (s32)u->unk9C->unkC == u->unkA4[4]) {
        b->unk11BA = 4;
    }
}

void btl_clearCmd(BtlCmd *arg0) {
    arg0->unk0 = 0;
    arg0->ability = 0;
    arg0->targets = 0;
    arg0->cmdNo = 0;
    arg0->subNo = 0;
    arg0->unk10.b.unk0_0 = 0;
    arg0->unk10.b.unk0_2 = 0;
    arg0->unk10.b.unk0_4 = 0;
    arg0->unk10.b.unk0_5 = 0;
    arg0->unk10.b.unk0_6 = 0;
    arg0->unk10.b.unk0_7 = 0;
    arg0->unk10.b.unk0_9 = 0;
    arg0->unk10.b.unk0_10 = 0;
    arg0->unk10.b.unk0_11 = 0;
}

void btl_queueAutoCmd(BtlUnit *arg0, s32 arg1, s32 arg2, s32 arg3) {
    if (!(arg0->status & 0x02021003) && g_gameState->unk1C->unk8E4.battle->unkD80 == 4) {
        btl_setCmd(arg0->unk8, arg1, arg2, arg3 & 0xFFFF, 0);
    }
}

void func_800B3DBC(BtlUnit *arg0) {
    if ((arg0->unk68 & 0x1F) == 5) {
        Battle *b = g_gameState->unk1C->unk8E4.battle;
        BtlCmd *p;
        for (p = &b->cmdQueue; p != NULL; p = p->unk0) {
            BtlCmd *n = p->unk0;
            if (n != NULL && n->unk4 == arg0 && n->cmdNo == 0x39) {
                btl_unlinkCmd(&p->unk0, 1);
                break;
            }
        }
        b->unk11BC = 0;
        b->unk11BB = 0;
        b->unk11B8 &= ~0xC;
    }
}

u8 btl_isCmdPending(BtlCmd *arg0) {
    BtlCmd *p;
    for (p = &g_gameState->unk1C->unk8E4.battle->cmdQueue; p != NULL; p = p->unk0) {
        if (arg0 == p) {
            return 1;
        }
    }
    p = g_gameState->unk1C->unk8E4.battle->curCmd;
    if (p != NULL && p == arg0) {
        return 1;
    }
    return 0;
}

u8 btl_isCmdQueued(BtlUnit *arg0, u8 arg1) {
    BtlCmd *p;
    for (p = &g_gameState->unk1C->unk8E4.battle->cmdQueue; p != NULL; p = p->unk0) {
        if (p->unk4 == arg0 && p->cmdNo == arg1) {
            return 1;
        }
    }
    p = g_gameState->unk1C->unk8E4.battle->curCmd;
    if (p != NULL && p->cmdNo == arg1) {
        return 1;
    }
    return 0;
}

void btl_removeCmd(BtlCmd *arg0) {
    BtlUnit *u = arg0->unk4;
    BtlCmd *p;
    for (p = &g_gameState->unk1C->unk8E4.battle->cmdQueue; p != NULL; p = p->unk0) {
        if (p->unk0 == arg0) {
            if (u != NULL && arg0 != u->unk10 && arg0 != u->unk14) {
                u->unk68 &= ~0x80000;
            }
            btl_unlinkCmd(&p->unk0, 0);
            return;
        }
    }
}

s32 btl_cancelUnitCmds(BtlUnit *arg0) {
    BtlCmd *c;
    BtlCmd *p;
    s32 r;
    arg0->unk68 &= ~0x80000;
    p = &g_gameState->unk1C->unk8E4.battle->cmdQueue;
    r = 0;
    while (p != NULL) {
        c = p->unk0;
        if (c != NULL && c->unk4 == arg0 && c->cmdNo < 0x36) {
            if (c->cmdNo < 0x30) {
                r = 1;
            }
            btl_releaseItem(c);
            btl_unlinkCmd(&p->unk0, 1);
        } else {
            p = c;
        }
    }
    return r;
}

void btl_removeUnitCmds(BtlUnit *arg0) {
    BtlCmd *p = &g_gameState->unk1C->unk8E4.battle->cmdQueue;
    while (p != NULL) {
        BtlCmd *n = p->unk0;
        if (n != NULL && n->unk4 == arg0) {
            btl_releaseItem(n);
            btl_unlinkCmd(&p->unk0, 1);
        } else {
            p = n;
        }
    }
}

void btl_removeQueuedCmd(BtlUnit *arg0, u8 arg1) {
    BtlCmd *p;
    for (p = &g_gameState->unk1C->unk8E4.battle->cmdQueue; p != NULL; p = p->unk0) {
        BtlCmd *n = p->unk0;
        if (n != NULL && n->unk4 == arg0 && n->cmdNo == arg1) {
            btl_unlinkCmd(&p->unk0, 1);
            return;
        }
    }
}

void btl_projectUnitPos(BtlUnit *arg0) {
    DVECTOR sxy;
    SVECTOR v;
    BtlBone *b;

    if (!((arg0->unk68 >> 22) & 1)) {
        gte_SetRotMatrix(&g_gameState->unk1C->unk18);
        gte_SetTransMatrix(&g_gameState->unk1C->unk18);
        b = &arg0->unk9C->unk20[arg0->unk7C];
        v.vx = b->unk14;
        v.vy = b->unk18;
        v.vz = b->unk1C;
        gte_ldv0(&v);
        gte_rtps();
        gte_stsxy(&sxy);
        arg0->unkB8 = sxy.vx;
        arg0->unkBA = sxy.vy;
    }
    arg0->unkBC = 1;
}

void btl_unlinkCmd(BtlCmd **arg0, u8 arg1) {
    BtlCmd *p = *arg0;
    *arg0 = p->unk0;
    p->unk10.b.unk0_2 = 0;
    p->unk10.b.unk0_4 = 0;
    if (arg1 && p->cmdNo == 0x38) {
        g_gameState->unk1C->unk8E4.battle->unk11B8 &= ~1;
    }
}

s32 btl_payMpCost(BtlCmd *arg0) {
    s32 mp = arg0->ability->mpCost;
    if (D_8007AF02 != 0 && (arg0->ability->unkB & 4)) {
        mp <<= 2;
    }
    if (arg0->cmdNo == 0x32) {
        return 1;
    }
    if (btl_consumeMp(arg0->unk4, mp) & 0xFF) {
        return 1;
    }
    btl_showMessage(0x16);
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
        func_800EA124(arg0->subNo);
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

void func_800B44A0(Unk800B44A0 *arg0) {
    s32 i;
    for (i = 3; i >= 0; i--) {
        arg0->unk1C[i] = 0;
    }
}

void btl_calcCmd(BtlUnit *caster, BtlUnit *target, BtlCmd *cmd, s32 type) {
    BtlCalc c;
    BtlCmd sub;
    BtlCmd sub2;

    c.unk0 = caster;
    c.unk4 = target;
    c.unk8 = (BtlCalc8 *)cmd;
    c.unk18 = 0;
    c.unk1A = c.unk1B = 0;
    c.unk1C = c.unk1E = c.unk20 = c.unk22 = 0;
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
                if ((caster->status & 0x4000) && ((caster->unk68 >> 1) & 0xF) == 3) {
                    c.unk14 *= 2;
                }
                btl_applyRowMod(&c);
                btl_applyWeaponElementBoost(&c);
                if (btl_applyWeaponElement(&c)) {
                    btl_rollCritical(&c);
                    func_800BA938((Unk800BA938 *)&c);
                    btl_finishPhysDamage(&c);
                    btl_markTrouble(&c);
                    btl_payMpAttack(&c);
                    if (c.unk0->unkC4[0] & 0x02000000) {
                        c.unkC = caster->unk74[7];
                        RAND_REFILL2();
                        if (RAND16() % 100 < c.unkC) {
                            c.unk18 |= 0x100;
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
                    d = c.unkE - c.unk10;
                    if (d <= 0) {
                        d = 1;
                    }
                    if (c.unk14 <= 0) {
                        c.unk14 = 1;
                    }
                    v = d * c.unk14;
                    if (v >= 10000) {
                        v = 9999;
                    }
                    c.unk1C = v;
                    c.unk1E = v;
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
            if (func_800B9DDC(&c) && (func_800B9B64(&c) || btl_rollStatusHit(&c))) {
                if (!(target->status & 0x40) && btl_hasEnemyFlag(target, 0x10) == 0) {
                    if (btl_checkTargetIsPlayer(&c)) {
                        v = target->max->maxHp * (target->unk33 + cmd->ability->unk5);
                        if (caster->unkC4[1] & 2) {
                            v = v / 50;
                        } else {
                            v = v / 100;
                        }
                        if (v >= 10000) {
                            v = 9999;
                        }
                        c.unk1E = v;
                        c.unk1B |= 3;
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
            if ((target->status & 0x40) || btl_hasEnemyFlag(target, 0x10) != 0) {
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
                c.unk1B |= 8;
                d = c.unkE - c.unk10;
                c.unk1A |= 8;
                if (d > 0) {
                    if (c.unk14 <= 0) {
                        c.unk14 = 1;
                    }
                    v = d * c.unk14 >> 2;
                    if (v >= 10000) {
                        v = 9999;
                    }
                }
                if ((target->status & 0x40) || btl_hasEnemyFlag(target, 0x10) != 0) {
                    c.unk1B |= 0x10;
                    if (caster->mp < v) {
                        v = caster->mp;
                    }
                } else {
                    c.unk1A |= 0x10;
                    if (target->mp < v) {
                        v = target->mp;
                    }
                }
                c.unk20 = v;
                c.unk22 = v;
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
                d = c.unkE - c.unk10;
                if (d > 0) {
                    if (c.unk14 <= 0) {
                        c.unk14 = 1;
                    }
                    v = d * c.unk14;
                    if (v >= 10000) {
                        v = 9999;
                    }
                    c.unk1C = v;
                    c.unk1E = v;
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
                    func_800BA078(&c);
                    btl_applyAbilityElementBoost(&c);
                    if (btl_applyAbilityElement(&c) && btl_missIfAirborne(&c)) {
                        if ((u8)type == 0x11) {
                            btl_calcPercentDamage(&c);
                        } else {
                            if (c.unk14 > 100) {
                                c.unk14 = 100;
                            }
                            v = target->hp * c.unk14 / 100;
                            if (v >= 10000) {
                                v = 9999;
                            }
                            c.unk1B |= 1;
                            if (c.unk18 & 8) {
                                c.unk1B |= 2;
                            }
                            c.unk1E = v;
                        }
                    }
                }
            }
            break;
        }
    case 0x12:
        if ((cmd->ability->unk0_8 != 0x38 || RAND8() < 0xAB) && (cmd->subNo != 0x2E || !cmd->unk10.b.unk0_9)) {
            RAND_REFILL2();
            c.unk14 = RAND16() % (caster->unk32 + caster->unk7A);
            c.unkE = cmd->ability->unk5;
            c.unk10 = 0;
            btl_applyCasterMiniMagic(&c);
            btl_applyTargetShell(&c);
            btl_halveForSpread(&c);
            btl_applyAbilityElementBoost(&c);
            if (btl_applyAbilityElement(&c) && btl_missIfAirborne(&c)) {
                btl_finishMagicDamage(&c);
                btl_rollAbilityStatus(&c);
            }
        } else {
            c.unk18 |= 1;
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
            i = caster->unk31;
            c.unk14 = i + RAND16() % (((caster->unk7A + i) >> 3) + 1);
            c.unkE = caster->unk74[5] * cmd->ability->unk5 / 10;
            c.unk10 = target->unk6E;
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
        if (target->unk7A == caster->unk7A) {
            c.unk14 += caster->unk7A;
            c.unk10 = 0;
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
            if (target->unk6C != 0) {
                RAND_REFILL2();
                target->unk6C = RAND16() % target->unk6C;
            }
            if (target->unk6E != 0) {
                RAND_REFILL2();
                target->unk6E = RAND16() % target->unk6E;
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
            c.unk1B |= 1;
            c.unk1E = cmd->ability->unk7 + cmd->ability->unk5 * 100;
        } else {
            c.unk18 |= 1;
        }
        break;
    case 0x1A:
        c.unk1B |= 1;
        c.unk1E = cmd->ability->unk7 + cmd->ability->unk5 * 100;
        break;
    case 0x1D:
        c.unk1B |= 1;
        c.unk1E = caster->max->maxHp - caster->hp;
        break;
    case 0x1E:
        c.unk1B |= 1;
        if (!(target->status & 0x40)) {
            c.unk1B |= 2;
        }
        c.unk1E = caster->max->maxHp / 3;
        break;
    case 0x1F:
        c.unk1B |= 8;
        if (target->mp != 0) {
            RAND_REFILL2();
            c.unk22 = RAND16() % target->mp;
            if (c.unk22 >= 10000) {
                c.unk22 = 9999;
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
                c.unk1B |= 1;
                d = c.unkE - c.unk10;
                if (d > 0) {
                    if (c.unk14 <= 0) {
                        c.unk14 = 1;
                    }
                    v = d * c.unk14;
                    c.unk1A |= 1;
                    c.unk1C = caster->max->maxHp >> 3;
                    if (v >= 10000) {
                        v = 9999;
                    }
                    if (c.unk18 & 8) {
                        c.unk1B |= 2;
                    }
                    c.unk1E = v;
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
                target->unk6C >>= 1;
            } else if ((u8)type == 0x22) {
                target->unk31 = target->unk31 * 3 / 4;
            } else if ((u8)type == 0x23) {
                target->unk6E >>= 1;
            } else if ((u8)type == 0x24) {
                target->unk32 = target->unk32 * 3 / 4;
            }
        }
        break;
    case 0x25:
        c.unk1B |= 0x1B;
        if (caster->unkC4[0] & 0x20000000) {
            c.unk1E = target->max->maxHp * cmd->ability->unk5 / 50;
            c.unk22 = target->max->maxMp * cmd->ability->unk5 / 50;
        } else {
            c.unk1E = target->max->maxHp * cmd->ability->unk5 / 100;
            c.unk22 = target->max->maxMp * cmd->ability->unk5 / 100;
        }
        break;
    case 0x26:
        {
            u32 v;
            u32 d;
            v = cmd->ability->unk5 * caster->unk7A;
            d = g_gameState->unk1C->gil + v;
            c.unk1B |= 1;
            v = v * v / 10;
            v = v * caster->unk33 / d;
            if (cmd->cmdNo == 0x1D) {
                v = v / btl_countActiveUnits(0);
            }
            if (v < 10000) {
                c.unk1E = v;
            } else {
                c.unk1E = 9999;
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
            c.unk1B |= 9;
            d = c.unkE - c.unk10;
            if (d > 0) {
                if (c.unk14 <= 0) {
                    c.unk14 = 1;
                }
                v = d * c.unk14;
                if (v >= 10000) {
                    v = 9999;
                }
                c.unk1E = v;
                c.unk22 = v >> 4;
            }
            break;
        }
    case 0x28:
        {
            s32 v;
            BtlUnit *u;
            v = target->max->maxHp - target->hp;
            c.unk1B |= 1;
            if (v >= 10000) {
                v = 9999;
            }
            c.unk1E = v;
            for (u = g_gameState->unk1C->unk8E4.battle->unk6F8; u != NULL; u = u->unk0) {
                if ((u->unk68 & 0x10000001) == 0x10000000) {
                    c.unk4 = u;
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
                c.unk1A = 0x18;
                RAND_REFILL2();
                c.unk20 = RAND16() % (target->unk7A * 2);
                for (u = g_gameState->unk1C->unk8E4.battle->unk6F8; u != NULL; u = u->unk0) {
                    if ((u->unk68 & 0x10000001) == 0x10000001) {
                        c.unk0 = u;
                        btl_applyCalcResult(&c);
                        btl_spawnPopups(u);
                    }
                }
                return;
            }
            c.unk18 |= 1;
            break;
        }
    case 0x2A:
        {
            s32 i;
            if (btl_checkFrozenShatter(&c)) {
                RAND_REFILL2();
                i = caster->unk31;
                c.unk14 = i + RAND16() % (((caster->unk7A + i) >> 3) + 1);
                c.unkE = D_8006E0E8[cmd->subNo].unk5 * 2;
                c.unk10 = target->unk6C;
                if (caster->unkC4[0] & 0x10000000) {
                    c.unk14 = c.unk14 * 3 >> 1;
                }
                btl_applyCasterStatusMod(&c);
                btl_applyTargetStatusMod(&c);
                if (btl_hasEnemyFlag(target, 0x80) != 0) {
                    c.unk14 = c.unk14 * 3 >> 1;
                }
                btl_finishPhysDamage(&c);
            }
            break;
        }
    case 0x2B:
        target->unk31 += target->unk31 / cmd->ability->unk5;
        if (target->unk31 >= 100) {
            target->unk31 = 99;
        }
        break;
    case 0x2C:
        target->unk32 += target->unk32 / cmd->ability->unk5;
        if (target->unk32 >= 100) {
            target->unk32 = 99;
        }
        break;
    case 0x2D:
        if (!(target->status & BTL_STATUS_KO) && !(caster->status & 0x101)) {
            target->hp = target->max->maxHp;
            target->mp = target->max->maxMp;
            caster->mp = 0;
            btl_killUnit(caster);
        } else {
            c.unk18 |= 1;
        }
        break;
    case 0x2E:
        {
            s32 v;
            s32 d;
            if ((u32)(g_gameState->unk1C->characters[(caster->unk68 >> 1) & 0xF].equip[0] - 7) < 9) {
                v = g_gameState->unk1C->unk8E4.battle->unk22CC[caster->unk74[1]];
                if (!(v & 0x100) || btl_guardIfDeathImmune(&c)) {
                    d = btl_addStatuses(target, v);
                    if (d == 0) {
                        c.unk18 |= 0x40;
                    } else if (d == 1) {
                        c.unk18 |= 1;
                    }
                }
            } else {
                c.unk18 |= 1;
            }
            break;
        }
    case 0x30:
    case 0x53:
        {
            s32 v;
            s32 d;
            btl_setPhysParams(&c, (u8)type == 0x30 ? 1 : 5);
            if (caster->unkC4[0] & 0x200000) {
                c.unk14 *= 2;
            } else {
                c.unk14 = c.unk14 * 3 >> 1;
            }
            btl_applyCasterMiniMagic(&c);
            btl_applyTargetStatusMod(&c);
            if ((u8)type == 0x30) {
                btl_finishPhysDamage(&c);
            } else {
                d = c.unkE - c.unk10;
                if (d <= 0) {
                    d = 1;
                }
                c.unk1B |= 1;
                if (c.unk14 < 4) {
                    c.unk14 = 4;
                }
                v = (u32)(d * c.unk14) / btl_countActiveUnits(0);
                if (v >= 10000) {
                    v = 9999;
                }
                c.unk1E = v;
            }
            break;
        }
    case 0x31:
        {
            s32 i;
            btl_clearCmd(&sub);
            sub.cmdNo = cmd->cmdNo;
            sub.unk10.b.unk0_10 = cmd->unk10.b.unk0_10;
            if (target->unk68 & 1) {
                sub.subNo = 0x49;
                i = 13;
            } else {
                sub.subNo = 0x48;
                i = 9;
            }
            sub.ability = (BtlCmd8 *)&g_gameState->unk1C->unk8E4.battle->abilities[sub.subNo];
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
        target->unk73 |= cmd->ability->unk6;
        target->unk70 &= ~cmd->ability->unk6;
        target->unk71 &= ~cmd->ability->unk6;
        target->unk72 &= ~cmd->ability->unk6;
        btl_showMessage(0xE, btl_getElementName(cmd->ability->unk6));
        break;
    case 0x34:
        if (item_getCount(cmd->ability->unk5) != 0) {
            btl_clearCmd(&sub2);
            sub2.cmdNo = 0xE;
            sub2.subNo = cmd->ability->unk5;
            sub2.ability = (BtlCmd8 *)&g_gameState->unk1C->unk8E4.battle->abilities[0];
            btl_calcCmd(caster, target, &sub2, D_8006B1F4[sub2.subNo - 0xE0].unk4);
            func_800EA164(sub2.subNo);
            return;
        }
        c.unk18 |= 1;
        break;
    case 0x35:
        {
            s32 v;
            c.unk1B |= 1;
            v = caster->hp % 10;
            if (v == 7) {
                switch (RAND8() % 4) {
                case 0:
                    c.unk1E = 7;
                    break;
                case 1:
                    c.unk1E = 77;
                    break;
                case 2:
                    c.unk1E = 777;
                    break;
                default:
                    c.unk1E = 7777;
                    break;
                }
            } else {
                c.unk1E = 1;
            }
            break;
        }
    case 0x36:
        {
            s32 v;
            if ((target->unk68 & 1) || !((g_gameState->unk1C->unk8E4.battle->unk132C.unk0 >> 2) & 1)) {
                v = (caster->unk68 & 1) << 11;
                target->unk86[1] = v;
                target->unkA0->unk22 = v;
                if ((target->unk68 & 1) && (&g_gameState->unk1C->characters[(target->unk68 >> 1) & 0xF])->unk34.b.unk9 != ((target->unk68 >> 8) & 3)) {
                    func_800BDA60(target);
                }
            } else {
                c.unk18 |= 1;
            }
            break;
        }
    case 0x37:
        func_800BDA60(target);
        break;
    case 0x38:
        if (btl_canFlee(&c)) {
            btl_rollFlee(&c);
        }
        break;
    case 0x39:
        btl_tryEscape(0);
        if (btl_canFlee(&c)) {
            btl_setCmd(&g_gameState->unk1C->unk8E4.battle->unk1060, 0x38, 0, 0xF, 1);
        }
        break;
    case 0x3A:
        {
            s32 v;
            s16 d;
            s16 k;
            Battle488 *e;
            e = &g_gameState->unk1C->unk8E4.battle->unk488[(target->unk68 >> 1) & 0xF];
            d = 0;
            for (k = 0; k < 4; k++) {
                if (e->unk8.b[k] != 0xFF) {
                    d = 1;
                }
            }
            if (!d) {
                btl_showMessage(0xB);
                break;
            }
            if (!(caster->unkC4[1] & 0x40000000)) {
                c.unk16 = caster->unk7A + caster->unk33;
                c.unk12 = target->unk7A;
                RAND_REFILL2();
                RAND_REFILL2();
                v = ((*g_randPtr << 8) | *g_randPtr) % c.unk16 < ((*g_randPtr << 8) | *g_randPtr) % c.unk12;
                g_randPtr++;
                if (v) {
                    btl_showMessage(0xA);
                    break;
                }
            }
            if (caster->unkC4[0] & 0x400000) {
                if (e->unk8.b[3] != 0xFF && RAND8() < 32) {
                    btl_calcSteal(&c, (Unk800B7F20 *)e, 3);
                } else if (e->unk8.b[2] != 0xFF && RAND8() < 32) {
                    btl_calcSteal(&c, (Unk800B7F20 *)e, 2);
                } else if (e->unk8.b[1] != 0xFF && RAND8() < 64) {
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
            if ((c.unk18 = btl_getUnitsWithStatus(0, 0x200)) != 0) {
                d = 0;
                for (u = g_gameState->unk1C->unk8E4.battle->unk6F8; u != NULL; u = u->unk0) {
                    if ((u->unkB0 & c.unk18) && !(u->status & 0x42021D03)) {
                        d = 1;
                        v = btl_pickRandomUnit(0);
                        if (v == 0) {
                            return;
                        }
                        btl_queueAutoCmd(u, 0x34, 0xB0, v);
                    }
                }
                if (!d) {
                    c.unk18 = 0;
                }
            }
            if (c.unk18 == 0) {
                btl_showMessage(0x1A);
            }
            return;
        }
    case 0x3E:
        if (btl_hasEnemyFlag(target, 0x20) != 0) {
            if (btl_checkTargetAlive(&c)) {
                btl_killUnit(target);
                btl_showMessage(0x19);
            }
        } else {
            btl_cureItemStatus(&c);
        }
        break;
    case 0x3F:
        {
            s32 i;
            RAND_REFILL2();
            i = caster->unk31;
            c.unk14 = i + RAND16() % (((caster->unk7A + i) >> 3) + 1);
            c.unkE = caster->unk74[5] + cmd->ability->unk5;
            c.unk10 = target->unk6C;
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
                if (target->max->maxHp / cmd->ability->unk5 < target->hp) {
                    btl_showMessage(0x10);
                } else {
                    i = g_gameState->unk1C->unk8E4.battle->unk488[(target->unk68 >> 1) & 0xF].unk0->unk27;
                    btl_killUnit(target);
                    if (i != 0 && !func_80034070((caster->unk68 >> 1) & 0xF, i)) {
                        func_80034220((caster->unk68 >> 1) & 0xF, i);
                        btl_showMessage(0x11, btl_getAbilityName(i));
                    } else {
                        btl_showMessage(0x12);
                    }
                }
            } else {
                btl_showMessage(0x1D);
            }
            break;
        }
    case 0x42:
        {
            s32 v;
            c.unk1B |= 1;
            v = g_gameState->unk1C->unk568;
            if (v != 0) {
                v = v * caster->unk7A;
                if (v >= 10000) {
                    c.unk1E = 9999;
                } else {
                    c.unk1E = v;
                }
            } else {
                c.unk1E = 1;
            }
            break;
        }
    case 0x43:
        {
            s32 v;
            c.unk1B |= 1;
            v = g_gameState->unk1C->unk56A * caster->unk30 / 2;
            if (v >= 10000) {
                c.unk1E = 9999;
            } else {
                c.unk1E = v;
            }
            break;
        }
    case 0x44:
        {
            c.unk1B |= 1;
            if (g_gameState->unk1C->unk56C * g_gameState->unk1C->unk56C < 10000) {
                c.unk1E = (u16)g_gameState->unk1C->unk56C * (u16)g_gameState->unk1C->unk56C;
            } else {
                c.unk1E = 9999;
            }
            break;
        }
    case 0x45:
    case 0x46:
        {
            s32 v;
            c.unk14 = 15;
            c.unkE = D_8006B1F4[cmd->subNo - 0xE0].unk5;
            c.unk10 = 0;
            if (caster->unkC4[0] & 0x08000000) {
                c.unk14 = 30;
            }
            if ((u8)type == 0x45) {
                btl_calcHeal(&c);
            } else {
                v = c.unkE * c.unk14;
                if (v >= 10000) {
                    v = 9999;
                }
                c.unk1B |= 8;
                if (!(target->status & 0x40) && btl_hasEnemyFlag(target, 0x10) == 0) {
                    c.unk1B |= 0x10;
                }
                c.unk22 = v;
            }
            break;
        }
    case 0x47:
        if (btl_checkTargetAlive(&c)) {
            if (!(target->status & 0x40) && btl_hasEnemyFlag(target, 0x10) == 0) {
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
            if ((target->status & 0x40) || btl_hasEnemyFlag(target, 0x10) != 0) {
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
        c.unk1B |= 3;
        c.unk1E = D_8006B1F4[cmd->subNo - 0xE0].unk5 * (item_getCount(cmd->subNo) + 1);
        break;
    case 0x4B:
        c.unk1B |= 1;
        c.unk1E = D_8006B1F4[cmd->subNo - 0xE0].unk5 * (item_getCount(cmd->subNo) + 1);
        break;
    case 0x4C:
        c.unk1B |= 0x1B;
        c.unk1E = target->max->maxHp >> 1;
        c.unk22 = target->max->maxMp >> 1;
        RAND_REFILL2();
        if (D_8006B1F4[cmd->subNo - 0xE0].unk7 < RAND16() % 100) {
            btl_showMessage(0xD);
            btl_addStatuses(target, D_8006B1F4[cmd->subNo - 0xE0].unk8);
        }
        break;
    case 0x4D:
        if (btl_checkTargetAlive(&c)) {
            c.unk1E = 9999;
            c.unk1B |= 1;
        }
        break;
    case 0x54:
        btl_initStatusHitRate(&c);
        btl_halveHitForShell(&c);
        if (btl_rollStatusHit(&c)) {
            func_800EA1B0(0xFE);
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
                c.unkE += item_getCount(0xE9);
                break;
            case 0x33:
                c.unkE += item_getCount(0xEA);
                break;
            case 0x35:
                c.unkE += item_getCount(0xE7);
                break;
            case 0x3C:
                c.unkE += item_getCount(0xE2);
                break;
            case 0x3E:
                c.unkE += item_getCount(0xE0);
                break;
            case 0x40:
                c.unkE += item_getCount(0xEB);
                break;
            case 0x42:
            case 0x43:
                c.unkE += item_getCount(0xE8);
                break;
            case 0x4A:
                c.unkE += caster->unk7A;
                break;
            }
            btl_finishMagicDamage(&c);
            btl_rollAbilityStatus(&c);
        }
        break;
    case 0x56:
        if (btl_missIfDeathImmune(&c)) {
            func_800BA078(&c);
            btl_applyAbilityElementBoost(&c);
            if (btl_applyAbilityElement(&c) && btl_missIfAirborne(&c)) {
                c.unk14 += item_getCount(0xE1);
                btl_calcPercentDamage(&c);
            }
        }
        break;
    case 0x57:
        {
            s32 i;
            if (btl_guardIfDeathImmune(&c)) {
                btl_initStatusHitRate(&c);
                c.unk16 += item_getCount(0xFE) >> 1;
                if (btl_rollStatusHit(&c)) {
                    btl_applyAbilityStatus(&c);
                }
            }
            if ((c.unk18 & 0x41) && (caster->unkC4[1] & 0x10000000)) {
                c.unk18 &= ~0x41;
                RAND_REFILL2();
                i = caster->unk32;
                c.unk14 = i + RAND16() % (((caster->unk7A + i) >> 3) + 1);
                c.unkE = cmd->ability->unk5 + 100 - item_getCount(0xFE);
                c.unk10 = target->unk6E;
                btl_finishMagicDamage(&c);
            }
            break;
        }
    case 0x58:
        c.unk1B |= 1;
        if (caster->hp != 0) {
            caster->unk80 = caster->hp;
            btl_killUnit(caster);
        }
        c.unk1E = caster->unk80;
        if (c.unk1E >= 10000) {
            c.unk1E = 9999;
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
        target->unk6C >>= 1;
        target->unk6E >>= 1;
        break;
    case 0x5C:
        func_800EA1B0(c.unk8->unk8[5]);
        break;
    case 0x5D:
        if (btl_missIfDeathImmune(&c) && btl_checkTargetAlive(&c)) {
            c.unk18 |= 0x80;
            target->hp = RAND8() % 9 + 1;
            btl_addStatuses(target, g_gameState->unk1C->unk8E4.battle->unk22CC[cmd->ability->unk9]);
        }
        break;
    case 0x5E:
        {
            target->unk32 >>= 1;
            if (caster->unk32 * 2 >= 0x100) {
                caster->unk32 = 0xFF;
            } else {
                caster->unk32 = caster->unk32 * 2;
            }
            break;
        }
    case 0x5F:
        {
            target->unk31 >>= 1;
            if (caster->unk31 * 2 >= 0x100) {
                caster->unk31 = 0xFF;
            } else {
                caster->unk31 = caster->unk31 * 2;
            }
            break;
        }
    case 0x60:
        target->unk78 = 0xFF;
        btl_addStatus(target, BTL_STATUS_TRANCE);
        break;
    case 0x61:
        if ((&g_gameState->unk1C->characters[(target->unk68 >> 1) & 0xF])->unkA & 2) {
            c.unk18 |= 1;
        } else {
            btl_applyAbilityStatus(&c);
        }
        break;
    case 0x62:
        if ((&g_gameState->unk1C->characters[(target->unk68 >> 1) & 0xF])->unkA & 8) {
            c.unk18 |= 1;
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
            c.unk1B |= 1;
            c.unk1E = target->unk7A * cmd->ability->unk5;
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
            if ((u8)type == 0x6B && ((target->unk68 >> 8) & 3)) {
                func_800BDA60(target);
            }
            btl_rollAbilityStatus(&c);
        }
        break;
    case 0x65:
        btl_initHitRates(&c);
        if (btl_rollHit(&c)) {
            func_800BAF70(&c);
        } else {
            btl_showMessage(0xA);
        }
        break;
    case 0x66:
        btl_setEnemyAttackParams(&c);
        btl_applyCasterStatusMod(&c);
        func_800BA308(&c);
        btl_applyTargetStatusMod(&c);
        btl_applyRowMod(&c);
        btl_finishMagicDamage(&c);
        func_800BAF70(&c);
        break;
    case 0x68:
        c.unk1B |= 1;
        if (g_evtGlobalVars.b[0xC0] >= 14) {
            c.unk1E = 9999;
        } else {
            c.unk1E = 1 << (g_evtGlobalVars.b[0xC0] - 1);
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
                            c.unk18 |= 0x80;
                            target->hp = RAND8() % 9 + 1;
                        }
                    }
                }
            }
            break;
        }
    case 0x6A:
        g_btlRewards.unkA &= ~(1 << ((u8)target->unk68 >> 5));
        func_800B3DBC(target);
        btl_removeUnitCmds(target);
        btl_saveCharacter(target, 1);
        btl_unlinkUnit(target);
        break;
    case 0x6D:
        if (!(target->status & 0x10000000)) {
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
    BtlUnit *t = c->unk4;
    BtlUnit *s = c->unk0;
    s32 back = 0;
    u16 i;
    u16 n;
    u32 a;
    u16 who;
    s32 id;
    BtlUnit *w;

    if ((t->unk68 & 1) && !(s->unk68 & 1)) {
        back = btl_tryCounter(t, s, (BtlCmd *)c->unk8);
    }
    if (c->unk18 & 0x40) {
        t->unk7E |= 0x80;
    } else if (c->unk18 & 1) {
        t->unk7E |= 0x20;
        if (c->unk18 & 2) {
            if (t->unk68 & 1) {
                t->unk9C->unkC = (BtlUnit9CC *)t->unkA4[16];
                t->unkA0->unk3A = 0;
                t->unk86[1] = t->unkA0->unk22 = (t->unk68 & 1) * 0x800;
            } else if (!((t->unk68 >> 23) & 1)) {
                t->unk8C[2] += 0x1900000;
            } else {
                w = func_800C1DEC();
                w->unk8C[2] += 0x1900000;
            }
            t->unk68 |= 0x4000000;
            c->unk8->unk10 |= 0x40;
        }
    } else {
        if (c->unk8->unk8[8] & 8) {
            btl_removeStatus(t, BTL_STATUS_CONFUSE);
            btl_removeStatus(t, BTL_STATUS_SLEEP);
        }
        if (c->unk8->unk8[8] & 0x10) {
            btl_removeStatus(t, BTL_STATUS_VANISH);
        }
        if (c->unk1B != 0) {
            t->unk7E |= c->unk1B;
            if (c->unk1B & 1) {
                n = 0;
                if (((c->unk8->unk10 >> 7) & 3) == 1) {
                    for (i = 0; i < 4; i++) {
                        if (s->unk1C[i] & t->unkB0) {
                            n++;
                        }
                    }
                    if (c->unk1E * n >= 10000) {
                        c->unk1E = 9999;
                    } else {
                        c->unk1E = c->unk1E * n;
                    }
                    if ((s32)*s->unkC4 < 0) {
                        c->unk1E *= 2;
                        if (c->unk1E >= 10000) {
                            c->unk1E = 9999;
                        }
                    }
                }
                if (c->unk1B & 2) {
                    btl_healHp(t, c->unk1E);
                } else {
                    btl_damageHp(t, c->unk1E, btl_canAddHitStatus(c));
                    btl_gainTrance(c, back);
                }
            }
            if (c->unk1B & 8) {
                if (c->unk1B & 0x10) {
                    btl_healMp(t, c->unk22);
                } else {
                    btl_damageMp(t, c->unk22);
                }
            }
        } else if (c->unk18 & 0x80) {
            if ((u8)btl_canAddHitStatus(c)) {
                btl_startHitMotion(t);
            }
            btl_gainTrance(c, back);
        }
        if (c->unk1A != 0) {
            s->unk7E |= c->unk1A;
            if (c->unk1A & 1) {
                if (c->unk1A & 2) {
                    btl_healHp(s, c->unk1C);
                } else {
                    btl_damageHp(s, c->unk1C, 0);
                }
            }
            if (c->unk1A & 8) {
                if (c->unk1A & 0x10) {
                    btl_healMp(s, c->unk20);
                } else {
                    btl_damageMp(s, c->unk20);
                }
            }
        }
        if ((c->unk18 & 0x100) && t->hp != 0) {
            a = g_gameState->unk1C->unk8E4.battle->unk22CC[s->unk74[1]];
            if (!(a & 0x100) || !(t->status & 0x80)) {
                btl_addStatuses(t, a);
            }
        }
    }
    if (!(t->unk68 & 1)) {
        who = 0x10;
        if (!((t->unk68 >> 23) & 1)) {
            who = t->unkB0;
        }
        id = c->unk8->unkF | (c->unk8->unkE << 8);
        if ((s->unk68 & 1) && !(t->status & 0x2001003)) {
            if ((((g_gameState->unk1C->unk8E4.battle->unk488 + ((t->unk68 >> 1) & 0xF))->unk4 >> 6) & 1) && t->hp == 0) {
                evt_battleTrigger(0x36, who, s->unkB0, id);
            } else if (t->hp != 0 && c->unk8->unkE < 0x30) {
                evt_battleTrigger(0x35, who, s->unkB0, id);
            }
        }
        evt_battleTrigger(0x37, who, s->unkB0, id);
    }
}

void btl_calcSteal(BtlCalc *arg0, Unk800B7F20 *arg1, s32 arg2) {
    u8 id = arg1->unk8[arg2];
    GameData *gd;
    GameData *g;
    u32 r;

    if (id != 0xFF) {
        arg1->unk8[arg2] = 0xFF;
        g = g_gameState->unk1C;
        if (g->unk56A < 9999) {
            g->unk56A++;
        }
        func_800EA1B0(id);
        btl_showMessage(0xC, btl_getItemName(id));
        if (arg0->unk0->unkC4[1] & 0x20000000) {
            arg0->unk1B |= 1;
            if (g_randPtr >= g_randTableEnd) {
                rand_refill();
                if (g_randPtr >= g_randTableEnd) {
                    rand_refill();
                }
            }
            arg0->unk1E = ((*g_randPtr << 8) | *g_randPtr++) % ((arg0->unk0->unk7A * arg0->unk4->unk7A) >> 1);
        }
        if (arg0->unk0->unkC4[0] & 0x800000) {
            gd = g_gameState->unk1C;
            if (g_randPtr >= g_randTableEnd) {
                rand_refill();
                if (g_randPtr >= g_randTableEnd) {
                    rand_refill();
                }
            }
            gd->gil = g_gameState->unk1C->gil +
                         ((*g_randPtr << 8) | *g_randPtr++) % ((arg0->unk0->unk7A * arg0->unk4->unk7A) / 4 + 1);
            if (g_gameState->unk1C->gil > 9999999) {
                g_gameState->unk1C->gil = 9999999;
            }
        }
    } else {
        btl_showMessage(0xA);
    }
}

void btl_gainTrance(BtlCalc *arg0, u8 arg1) {
    BtlUnit *u;
    u8 v;
    if ((arg0->unk4->unk68 & 1) && !(arg0->unk0->unk68 & 1)) {
        if (arg1 == 0) {
            btl_tryAutoPotion(arg0->unk4, arg0->unk8->unkE);
        }
        func_800B0E04(arg0->unk4, arg0->unk8->unk8);
        u = arg0->unk4;
        if (((u->unk68 >> 30) & 1) && u->hp != 0 && !(u->status & 0x02005143)) {
            if (u->unkC4[1] & 8) {
                v = u->unk33;
            } else {
                if (g_randPtr >= g_randTableEnd) {
                    rand_refill();
                    if (g_randPtr >= g_randTableEnd) {
                        rand_refill();
                    }
                }
                v = ((*g_randPtr << 8) | *g_randPtr++) % arg0->unk4->unk33;
            }
            u = arg0->unk4;
            if (v + u->unk78 < 0xFF) {
                u->unk78 += v;
            } else {
                u->unk78 = 0xFF;
                btl_addStatus(arg0->unk4, BTL_STATUS_TRANCE);
            }
        }
    }
}

u8 btl_rollHit(BtlCalc *arg0) {
    if (arg0->unk0->status & 0x410) {
        arg0->unk16 /= 2;
    }
    if (arg0->unk0->status & 0x04004000) {
        arg0->unk12 = 0;
    }
    if (arg0->unk4->unkC4[0] & 0x400) {
        arg0->unk16 /= 2;
    }
    if (arg0->unk4->status & 0x02021417) {
        arg0->unk12 = 0;
    }
    if (arg0->unk4->status & 0x8000) {
        arg0->unk16 /= 2;
    }
    if (arg0->unk4->status & 0x04000000) {
        arg0->unk16 = 0;
    }
    if (g_randPtr >= g_randTableEnd) {
        rand_refill();
        if (g_randPtr >= g_randTableEnd) {
            rand_refill();
        }
    }
    if (((*g_randPtr << 8) | *g_randPtr++) % 100 >= arg0->unk16) {
        arg0->unk18 |= 1;
        return 0;
    }
    if (g_randPtr >= g_randTableEnd) {
        rand_refill();
        if (g_randPtr >= g_randTableEnd) {
            rand_refill();
        }
    }
    if (((*g_randPtr << 8) | *g_randPtr++) % 100 < arg0->unk12) {
        if (arg0->unk4 != arg0->unk0) {
            arg0->unk18 |= 1;
            if (!((arg0->unk4->unk68 >> 25) & 1)) {
                arg0->unk18 |= 3;
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
    if (((*g_randPtr << 8) | *g_randPtr++) % 100 < arg0->unk16) {
        if (g_randPtr >= g_randTableEnd) {
            rand_refill();
            if (g_randPtr >= g_randTableEnd) {
                rand_refill();
            }
        }
        if (((*g_randPtr << 8) | *g_randPtr++) % 100 >= arg0->unk12) {
            return 1;
        }
    }
    arg0->unk18 |= 1;
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
    r = ((*g_randPtr << 8) | *g_randPtr++) % (((arg0->unk0->unk7A + arg0->unk0->unk31) >> 3) + 1);
    arg0->unkE = arg0->unk0->unk74[5];
    arg0->unk10 = arg0->unk4->unk6C;
    switch (arg1) {
    case 7:
        arg0->unkE += arg0->unk0->unk7A;
    case 1:
        arg0->unk14 = arg0->unk0->unk31 + r;
        break;
    case 2:
        arg0->unk14 = ((arg0->unk0->unk31 + arg0->unk0->unk33) >> 1) + r;
        break;
    case 3:
        arg0->unk14 = ((arg0->unk0->unk31 + arg0->unk0->unk30) >> 1) + r;
        break;
    case 4:
        arg0->unk14 = ((arg0->unk0->unk31 + arg0->unk0->unk32) >> 1) + r;
        break;
    case 5:
        if (g_randPtr >= g_randTableEnd) {
            rand_refill();
            if (g_randPtr >= g_randTableEnd) {
                rand_refill();
            }
        }
        arg0->unk14 = ((*g_randPtr << 8) | *g_randPtr++) % arg0->unk0->unk31 + r;
        break;
    }
}

void btl_applyKillerBonus(BtlCalc *arg0) {
    u32 f = arg0->unk4->unk68;
    u32 c;
    Battle4880 *e;
    BtlUnit *u;
    if (!(f & 1)) {
        c = arg0->unk0->unkC4[0];
        e = g_gameState->unk1C->unk8E4.battle->unk488[(f >> 1) & 0xF].unk0;
        if (((c & 0x2000) && (e->unk2 & 0x80)) || ((c & 0x4000) && (e->unk2 & 0x40)) || ((c & 0x8000) && (e->unk2 & 0x20)) || ((c & 0x10000) && (e->unk2 & 0x10)) || ((c & 0x20000) && (e->unk2 & 8)) || ((c & 0x40000) && (e->unk2 & 4)) || ((c & 0x80000) && (e->unk2 & 2)) || ((c & 0x100000) && (e->unk2 & 1))) {
            arg0->unk14 = arg0->unk14 * 3 >> 1;
        }
    }
    u = arg0->unk0;
    if ((u->unkC4[0] & 0x1000) && u->mp > 0) {
        arg0->unk14 = arg0->unk14 * 3 >> 1;
        arg0->unk18 |= 4;
    }
}

void btl_rollCritical(BtlCalc *arg0) {
    u8 s = arg0->unk0->unk33 >> 2;
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
            arg0->unk14 *= 2;
            arg0->unk1B |= 4;
        }
    }
}

u8 btl_applyAbilityElement(BtlCalc *arg0) {
    BtlUnit *t;
    u8 e = arg0->unk8->unk8[6];
    if (arg0->unk0->unkC4[1] & 1) {
        return 1;
    }
    t = arg0->unk4;
    if (t->unk70 & e) {
        arg0->unk18 |= 0x40;
        return 0;
    }
    if (t->unk72 & e) {
        arg0->unk14 /= 2;
    }
    if (arg0->unk4->unk71 & e) {
        arg0->unk10 = 0;
        arg0->unk18 |= 8;
    }
    if (arg0->unk4->unk73 & e) {
        arg0->unk14 = arg0->unk14 * 3 >> 1;
    }
    if (e & 0x12) {
        btl_removeStatus(arg0->unk4, BTL_STATUS_HEAT);
    }
    if (e & 1) {
        btl_removeStatus(arg0->unk4, BTL_STATUS_FREEZE);
    }
    return 1;
}

void btl_finishPhysDamage(BtlCalc *arg0) {
    s32 d = arg0->unkE - arg0->unk10;
    s32 v;
    BtlUnit *u;
    BtlUnit *t;
    if (d <= 0) {
        d = 1;
    }
    arg0->unk1B |= 1;
    if (arg0->unk14 <= 0) {
        arg0->unk14 = 1;
    }
    v = d * arg0->unk14;
    if (v >= 10000) {
        v = 9999;
    }
    if (!(arg0->unk18 & 8)) {
        u = arg0->unk0;
        if ((u->unk68 & 1) && ((*u->unkC4 & 0x01000000) || g_gameState->unk1C->characters[(u->unk68 >> 1) & 0xF].equip[0] == 0x3C)) {
            if (!(arg0->unk4->status & 0x40) && btl_hasEnemyFlag(arg0->unk4, 0x10) == 0) {
                t = arg0->unk4;
                t->unk86[1] = t->unkA0->unk22 = (t->unk68 & 1) * 0x800;
                arg0->unk1B |= 2;
            }
        }
    } else {
        arg0->unk1B |= 2;
    }
    arg0->unk1E = v;
}

void btl_setTargetHp(BtlCalc *arg0) {
    BtlUnit *t;
    BtlUnit *u;
    BtlUnit9C *m;
    if (arg0->unk8->unk8[5] != 0) {
        if (arg0->unk4->status & BTL_STATUS_KO) {
            arg0->unk18 |= 1;
            return;
        }
        arg0->unk18 |= 0x80;
        arg0->unk4->hp = arg0->unk8->unk8[5];
        t = arg0->unk4;
        t->unk86[1] = t->unkA0->unk22 = (t->unk68 & 1) * 0x800;
        return;
    }
    u = arg0->unk4;
    u->hp = 0;
    u->unk68 |= 0x100000;
    if (!(u->unk68 & 1)) {
        btl_playDeathCry(u, g_gameState->unk1C->unk8E4.battle->unk488[(u->unk68 >> 1) & 0xF].unk0->unk30);
    }
    m = u->unk9C;
    if ((s32)m->unkC != u->unkA4[(u->unk68 >> 18) & 1] && (!(u->unk68 & 1) || (s32)m->unkC != u->unkA4[9]) && u != g_gameState->unk1C->unk8E4.battle->curCmd->unk4) {
        m->unkC = (BtlUnit9CC *)u->unkA4[(u->unk68 >> 18) & 1];
        u->unkA0->unk3A = 0;
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
    b = g_gameState->unk1C->unk8E4.battle;
    le = lp = 0;
    u = b->unk6F8;
    c = b->curCmd;
    for (; u != NULL; u = u->unk0) {
        if (u->unk68 & 1) {
            np++;
            lp += u->unk7A;
        } else {
            ne++;
            le += u->unk7A;
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
    if (r > ((*g_randPtr << 8) | *g_randPtr++) % 100 && (c == NULL || (c->unk4->unk68 & 1))) {
        btl_setCmd((s32)&g_gameState->unk1C->unk8E4.battle->unk1060, 0x38, 1, 0xF, 1);
    }
}

u8 btl_checkFrozenShatter(BtlCalc *arg0) {
    BtlUnit *u;
    BtlUnit9C *m;
    if ((arg0->unk4->status & 0x02000001) != 0x02000000) {
        return 1;
    }
    btl_removeQueuedCmd(arg0->unk4, 0x3E);
    u = arg0->unk4;
    u->hp = 0;
    u->unk68 |= 0x100000;
    if (!(u->unk68 & 1)) {
        btl_playDeathCry(u, g_gameState->unk1C->unk8E4.battle->unk488[(u->unk68 >> 1) & 0xF].unk0->unk30);
    }
    m = u->unk9C;
    if ((s32)m->unkC != u->unkA4[(u->unk68 >> 18) & 1] && (!(u->unk68 & 1) || (s32)m->unkC != u->unkA4[9]) && u != g_gameState->unk1C->unk8E4.battle->curCmd->unk4) {
        m->unkC = (BtlUnit9CC *)u->unkA4[(u->unk68 >> 18) & 1];
        u->unkA0->unk3A = 0;
    }
    btl_showMessage(0x13);
    return 0;
}

s32 btl_consumeMp(BtlUnit *arg0, s16 arg1) {
    if (arg0 != NULL) {
        if (arg0->unkC4[1] & 4) {
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
    if ((arg0->unk4->unk7A >> 1) + arg0->unk4->unk33 < ((*g_randPtr << 8) | *g_randPtr++) % 100) {
        arg0->unk10.b.unk0_9 = 1;
    }
}

s32 func_800B96AC(BtlUnit *arg0, s16 arg1) {
    BtlUnit *u;
    u32 f;
    if (arg0->mp >= arg1) {
        for (u = g_gameState->unk1C->unk8E4.battle->unk6F8; u != NULL; u = u->unk0) {
            f = u->unk68;
            if ((f & 1) && g_gameState->unk1C->characters[(f >> 1) & 0xF].unk34.b.unk4 == 2) {
                if (!(u->status & 0x13021D0B) && !(arg0->status & 0x13021D0B)) {
                    return 1;
                }
                break;
            }
        }
    }
    btl_showMessage(0xF);
    return 0;
}

void btl_killUnit(BtlUnit *arg0) {
    BtlUnit9C *m;
    arg0->hp = 0;
    arg0->unk68 |= 0x100000;
    if (!(arg0->unk68 & 1)) {
        btl_playDeathCry(arg0, g_gameState->unk1C->unk8E4.battle->unk488[(arg0->unk68 >> 1) & 0xF].unk0->unk30);
    }
    m = arg0->unk9C;
    if ((s32)m->unkC != arg0->unkA4[(arg0->unk68 >> 18) & 1] && (!(arg0->unk68 & 1) || (s32)m->unkC != arg0->unkA4[9]) && arg0 != g_gameState->unk1C->unk8E4.battle->curCmd->unk4) {
        m->unkC = (BtlUnit9CC *)arg0->unkA4[(arg0->unk68 >> 18) & 1];
        arg0->unkA0->unk3A = 0;
    }
}

void btl_rollShortSummon(BtlCmd *arg0) {
    s32 r;
    if (!(arg0->unk4->unkC4[1] & 0x08000000)) {
        if (arg0->ability->mpCost * 2 < arg0->unk4->mp) {
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
            arg0->unk10.b.unk0_10 = 1;
        }
    }
}

s32 btl_canAddHitStatus(BtlCalc *arg0) {
    if (((arg0->unk18 & 0x100) && (g_gameState->unk1C->unk8E4.battle->unk22CC[arg0->unk0->unk74[1]] & 0x42001103)) || (arg0->unk8->unk8[8] & 0x40) || ((arg0->unk8->unk10 >> 5) & 1) || (arg0->unk4->status & 0x42009103) || arg0->unk0 == arg0->unk4) {
        return 0;
    }
    return 1;
}

void btl_setDrainFlags(BtlCalc *arg0) {
    BtlUnit *u;
    arg0->unk1B |= 1;
    u = arg0->unk4;
    arg0->unk1A |= 1;
    if ((u->status & 0x40) || btl_hasEnemyFlag(u, 0x10) != 0) {
        arg0->unk1B |= 2;
    } else {
        arg0->unk1A |= 2;
    }
}

void btl_initHitRates(BtlCalc *arg0) {
    arg0->unk16 = 100;
    arg0->unk12 = arg0->unk4->unk6D;
}

void btl_initStatusHitRate(BtlCalc *arg0) {
    BtlUnit *c = arg0->unk0;
    arg0->unk16 = c->unk7A + (arg0->unk8->unk8[7] + (c->unk32 >> 2)) - arg0->unk4->unk7A;
    if (arg0->unk16 > 100) {
        arg0->unk16 = 100;
    } else if (arg0->unk16 <= 0) {
        arg0->unk16 = 1;
    }
    arg0->unk12 = arg0->unk4->unk6F;
}

u8 func_800B9B64(BtlCalc *arg0) {
    BtlUnit *t = arg0->unk4;
    if ((t->status & 0x40) || btl_hasEnemyFlag(t, 0x10) != 0) {
        BtlUnit *c = arg0->unk0;
        arg0->unk16 = c->unk7A + (arg0->unk8->unk8[7] + (c->unk32 >> 2)) - arg0->unk4->unk7A;
        if (arg0->unk16 > 100) {
            arg0->unk16 = 100;
        } else if (arg0->unk16 <= 0) {
            arg0->unk16 = 1;
        }
        arg0->unk12 = arg0->unk4->unk6F;
        return 0;
    }
    return 1;
}

u32 btl_hasSureHit(BtlCalc *arg0) {
    return *arg0->unk0->unkC4 & 0x200;
}

void btl_halveHitForShell(BtlCalc *arg0) {
    if (arg0->unk4->status & 0x400000) {
        arg0->unk16 /= 2;
    }
}

void btl_halveHitForSpread(BtlCalc *arg0) {
    BtlCalc8 *c = arg0->unk8;
    if ((c->unk10 & 3) == 1 && (u32)((*(u32 *)c->unk8 & 0xF) - 3) < 3) {
        arg0->unk16 /= 2;
    }
}

u8 btl_checkLevelMultiple(BtlCalc *arg0) {
    if (arg0->unk4->unk7A % arg0->unk8->unk8[7] != 0) {
        arg0->unk18 |= 1;
        return 0;
    }
    return 1;
}

u8 btl_missIfDeathImmune(BtlCalc *arg0) {
    if (arg0->unk4->status & 0x80) {
        arg0->unk18 |= 1;
        return 0;
    }
    return 1;
}

u8 btl_checkTargetIsPlayer(BtlCalc *arg0) {
    if (!(arg0->unk4->unk68 & 1)) {
        arg0->unk18 |= 1;
        return 0;
    }
    return 1;
}

u8 btl_checkTargetAlive(BtlCalc *arg0) {
    if (arg0->unk4->status & 0x101) {
        arg0->unk18 |= 1;
        return 0;
    }
    return 1;
}

u8 func_800B9DDC(BtlCalc *arg0) {
    u32 st = arg0->unk4->status;
    if ((st & 1) || (st & 0x140) == 0x140) {
        arg0->unk18 |= 1;
        return 0;
    }
    return 1;
}

u8 btl_checkNotSelf(BtlCalc *arg0) {
    if (arg0->unk4 == arg0->unk0) {
        arg0->unk18 |= 1;
        return 0;
    }
    return 1;
}

u8 btl_guardIfDeathImmune(BtlCalc *arg0) {
    if (arg0->unk4->status & 0x80) {
        arg0->unk18 |= 0x40;
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
    c = arg0->unk0;
    a = c->unk31;
    arg0->unk14 = a + ((r << 8) | r) % (((c->unk7A + a) >> 2) + 1);
    g_randPtr++;
    arg0->unkE = arg0->unk8->unk8[5];
    arg0->unk10 = arg0->unk4->unk6C;
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
    c = arg0->unk0;
    a = c->unk32;
    arg0->unk14 = a + ((r << 8) | r) % (((c->unk7A + a) >> 3) + 1);
    g_randPtr++;
    arg0->unkE = arg0->unk8->unk8[5];
    arg0->unk10 = arg0->unk4->unk6E;
}

void func_800BA078(BtlCalc *arg0) {
    arg0->unk14 = arg0->unk8->unk8[5];
}

void btl_setWeaponAbilityParams(BtlCalc *arg0) {
    s32 lo;
    if (g_randPtr >= g_randTableEnd) {
        rand_refill();
        if (g_randPtr >= g_randTableEnd) {
            rand_refill();
        }
    }
    lo = arg0->unk0->unk31;
    arg0->unk14 = lo + ((*g_randPtr << 8) | *g_randPtr++) % (((arg0->unk0->unk7A + lo) >> 3) + 1);
    arg0->unkE = arg0->unk0->unk74[5] * arg0->unk8->unk8[5] / 10;
    arg0->unk10 = arg0->unk4->unk6C;
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
    c = arg0->unk0;
    a = c->unk31;
    arg0->unk14 = a + ((r << 8) | r) % (((c->unk7A + a) >> 2) + 1);
    g_randPtr++;
    arg0->unkE = arg0->unk8->unk8[5];
    arg0->unk10 = arg0->unk4->unk6C;
}

void btl_applyCasterStatusMod(BtlCalc *arg0) {
    if (arg0->unk0->status & 0x4800) {
        arg0->unk14 = arg0->unk14 * 3 >> 1;
    }
    if (arg0->unk0->status & 0x10000000) {
        arg0->unk14 = 1;
    }
}

void func_800BA308(BtlCalc *arg0) {
    s32 r;
    if ((arg0->unk4->unkC4[0] & 0x04000000) && arg0->unk10 != 0) {
        if (g_randPtr >= g_randTableEnd) {
            rand_refill();
            if (g_randPtr >= g_randTableEnd) {
                rand_refill();
            }
        }
        r = *g_randPtr;
        g_randPtr++;
        arg0->unk10 = ((r << 8) | r) % (arg0->unk10 * 2);
    }
}

void btl_applyTargetStatusMod(BtlCalc *arg0) {
    if (arg0->unk4->status & 0x808000) {
        arg0->unk14 >>= 1;
    }
    if (arg0->unk4->status & 0x10020000) {
        arg0->unk14 = arg0->unk14 * 3 >> 1;
    }
}

void btl_applyCasterMiniMagic(BtlCalc *arg0) {
    if (arg0->unk0->status & 0x10000000) {
        arg0->unk14 /= 2;
    }
}

void btl_applyTargetShell(BtlCalc *arg0) {
    if (arg0->unk4->status & 0x400000) {
        arg0->unk14 /= 2;
    }
}

void btl_applyHealBoost(BtlCalc *arg0) {
    if (arg0->unk0->unkC4[1] & 2) {
        arg0->unk14 = arg0->unk14 * 3 >> 1;
    }
}

void btl_applyRowMod(BtlCalc *arg0) {
    BtlUnit *t = arg0->unk4;
    BtlUnit *u;
    s32 d;
    if (arg0->unk0->unkA0->unk22 == t->unkA0->unk22 || (g_gameState->unk1C->unk8E4.battle->escapeHeld != 0 && (t->unk68 & 1))) {
        arg0->unk14 = arg0->unk14 * 3 >> 1;
    }
    u = arg0->unk0;
    if (!(u->unk68 & 1) || (!(u->unkC4[0] & 0x800) && !(u->unk74[0] & 2))) {
        d = (u->unk68 >> 8) & 3;
        d -= (arg0->unk4->unk68 >> 8) & 3;
        if ((d < 0 ? -d : d) >= 2) {
            arg0->unk14 /= 2;
        }
    }
}

void btl_applyWeaponElementBoost(BtlCalc *arg0) {
    BtlUnit *u = arg0->unk0;
    if (u->unk79 & u->unk74[6]) {
        arg0->unk14 = arg0->unk14 * 3 >> 1;
    }
}

u8 btl_applyWeaponElement(BtlCalc *arg0) {
    BtlUnit *t = arg0->unk4;
    u8 e = arg0->unk0->unk74[6];
    if (t->unk70 & e) {
        arg0->unk18 |= 0x40;
        return 0;
    }
    if (t->unk72 & e) {
        arg0->unk14 /= 2;
    }
    if (arg0->unk4->unk71 & e) {
        arg0->unk10 = 0;
        arg0->unk18 |= 8;
    }
    if (arg0->unk4->unk73 & e) {
        arg0->unk14 = arg0->unk14 * 3 >> 1;
    }
    if (e & 0x12) {
        btl_removeStatus(arg0->unk4, BTL_STATUS_HEAT);
    }
    if (e & 1) {
        btl_removeStatus(arg0->unk4, BTL_STATUS_FREEZE);
    }
    return 1;
}

u8 btl_missIfAirborne(BtlCalc *arg0) {
    if ((btl_hasStatus(arg0->unk4, BTL_STATUS_FLOAT) & 0xFF) || btl_hasEnemyFlag(arg0->unk4, 0x80) != 0) {
        if (arg0->unk8->unk8[6] & 8) {
            arg0->unk18 |= 1;
            return 0;
        }
        return 1;
    }
    return 1;
}

void btl_halveForSpread(BtlCalc *arg0) {
    BtlCalc8 *c = arg0->unk8;
    if ((c->unk10 & 3) == 1 && (u32)((*(u32 *)c->unk8 & 0xF) - 3) < 3 && btl_countActiveUnits(c->unkC < 0x10) >= 2) {
        arg0->unk14 /= 2;
    }
}

void btl_applyAbilityElementBoost(BtlCalc *arg0) {
    if (arg0->unk0->unk79 & arg0->unk8->unk8[6]) {
        arg0->unk14 = arg0->unk14 * 3 >> 1;
    }
}

void func_800BA938(Unk800BA938 *arg0) {
    if ((g_gameState->unk1C->unk8E4.battle->unk132C.unk0 >> 10) & 1) {
        arg0->unkE = 0x3C - arg0->unkE;
        if (arg0->unkE <= 0) {
            arg0->unkE = 1;
        }
    }
}

void btl_calcHeal(BtlCalc *arg0) {
    s32 v = arg0->unkE * arg0->unk14;
    BtlUnit *u;
    if (v >= 10000) {
        v = 9999;
    }
    u = arg0->unk4;
    arg0->unk1B |= 1;
    if (!(u->status & 0x40) && btl_hasEnemyFlag(u, 0x10) == 0) {
        arg0->unk1B |= 2;
    }
    arg0->unk1E = v;
}

void btl_finishMagicDamage(BtlCalc *arg0) {
    s32 a = arg0->unkE - arg0->unk10;
    s32 v;
    if (a <= 0) {
        a = 1;
    }
    arg0->unk1B |= 1;
    if (arg0->unk14 <= 0) {
        arg0->unk14 = 1;
    }
    v = a * arg0->unk14;
    if ((arg0->unk8->unk10 >> 10) & 1) {
        v = v * 2 / 3;
    }
    if (v >= 10000) {
        v = 9999;
    }
    if (arg0->unk18 & 8) {
        arg0->unk1B |= 2;
    }
    arg0->unk1E = v;
}

void btl_calcPercentDamage(BtlCalc *arg0) {
    s32 v;
    u8 f;
    if (arg0->unk14 > 100) {
        arg0->unk14 = 100;
    }
    v = arg0->unk4->max->maxHp * arg0->unk14 / 100;
    if ((arg0->unk8->unk10 >> 10) & 1) {
        v = v * 2 / 3;
    }
    if (v >= 10000) {
        v = 9999;
    }
    f = arg0->unk1B;
    arg0->unk1B = f | 1;
    if (arg0->unk18 & 8) {
        arg0->unk1B = f | 3;
    }
    arg0->unk1E = v;
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
    v = arg0->unk8->unk8[7];
    g_randPtr++;
    if (((r << 8) | r) % 100 < v) {
        btl_addStatuses(arg0->unk4, g_gameState->unk1C->unk8E4.battle->unk22CC[arg0->unk8->unk8[9]]);
    }
}

void btl_applyAbilityStatus(BtlCalc *arg0) {
    BtlCalc8 *c = arg0->unk8;
    u32 mask = g_gameState->unk1C->unk8E4.battle->unk22CC[c->unk8[9]];
    u32 r;
    if (!((c->unk10 >> 10) & 1) && c->unkE == 0x14) {
        mask |= 0x800000;
    }
    r = btl_addStatuses(arg0->unk4, mask);
    if (r == 0) {
        arg0->unk18 |= 0x40;
    } else if (r == 1) {
        arg0->unk18 |= 1;
    }
}

void btl_cureAbilityStatus(BtlCalc *arg0) {
    if (btl_removeStatuses(arg0->unk4, g_gameState->unk1C->unk8E4.battle->unk22CC[arg0->unk8->unk8[9]]) != 2) {
        arg0->unk18 |= 1;
    }
}

void btl_cureItemStatus(BtlCalc *arg0) {
    if (btl_removeStatuses(arg0->unk4, D_8006B1F4[arg0->unk8->unkF - 0xE0].unk8) != 2) {
        arg0->unk18 |= 1;
    }
}

void btl_payMpAttack(BtlCalc *arg0) {
    if (arg0->unk18 & 4) {
        BtlUnit *u = arg0->unk0;
        s32 v = u->max->maxMp >> 3;
        if (v == 0 || u->mp < v) {
            u->mp = 0;
        } else {
            u->mp -= v;
        }
    }
}

u8 btl_canFlee(BtlCalc *arg0) {
    Battle *b = g_gameState->unk1C->unk8E4.battle;
    BtlUnit *u;
    if ((u8)b->unk132C.unk0 >> 7 == 0) {
        return 0;
    }
    for (u = b->unk6F8; u != NULL; u = u->unk0) {
        if ((u->unk68 & 1) && !(u->status & 0x42021143)) {
            return 1;
        }
    }
    return 0;
}

void btl_markTrouble(BtlCalc *arg0) {
    BtlUnit *u = arg0->unk4;
    if (u->status & 0x20) {
        u->unk7E |= 0x100;
    }
}

void func_800BAF70(BtlCalc *arg0) {
    s16 id = arg0->unk8->unk8[7];
    if (item_getCount(id) == 0) {
        btl_showMessage(10);
    } else {
        func_800EA164(id);
        btl_showMessage(0x1F, btl_getItemName(id));
    }
}
