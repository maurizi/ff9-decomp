#include "ovl_0b.h"

void btl_hideMeshes(BtlUnit *arg0, u16 arg1) {
    s32 i;
    for (i = 0; i < 16; i++) {
        if ((arg1 >> i) & 1) {
            arg0->model->unk14 |= 1 << i;
        }
    }
}

void btl_showMeshes(BtlUnit *arg0, u16 arg1) {
    s32 i;
    if (!(arg0->unk68 & 1)) {
        arg0->model->unk0 &= ~0x20;
    }
    for (i = 0; i < 16; i++) {
        if ((arg1 >> i) & 1) {
            arg0->model->unk14 &= ~(1 << i);
        }
    }
}

void btl_hideWeapon(BtlUnit *arg0) {
    s32 i;
    for (i = 0; i < arg0->weaponModel->unk8->unk3; i++) {
        arg0->weaponModel->unk14 |= 1 << i;
    }
}

void btl_showWeapon(BtlUnit *arg0) {
    s32 i;
    for (i = 0; i < arg0->weaponModel->unk8->unk3; i++) {
        arg0->weaponModel->unk14 &= ~(1 << i);
    }
}

s32 func_800BD5A0(BtlUnit *arg0) {
    arg0->model->unkC = (BtlUnit9CC *)arg0->motions[4];
    arg0->model->unk50[2].unk4 &= ~3;
    arg0->model->unk50[0].unk4 |= 3;
    arg0->model->unk50[0].unkC = 0;
    arg0->model->unk50[0].unk12 = 0x1000;
    arg0->unkA0->unk3A = 0;
    if (arg0->status & BTL_STATUS_AUTO_LIFE) {
        arg0->unk7D = 7;
        btl_setCmd(arg0->reviveCmd, BTL_CMD_AUTO_LIFE, 0, arg0->unitBit, 0);
        btl_removeStatus(arg0, BTL_STATUS_AUTO_LIFE);
        return 0;
    }
    arg0->unk7D = 6;
    return 1;
}

void func_800BD698(BtlUnit *arg0, s32 arg1, s32 arg2) {
    u32 i;
    for (i = 0; i < 0x22; i++) {
        g_gameState->unk1C->unk8E4.battle->partyMotions[arg2][i] = (s32)db_lookupRegistered(4, D_800F524C[arg1][i] | 0x30000, 0);
    }
    arg0->motions = g_gameState->unk1C->unk8E4.battle->partyMotions[arg2];
}

void btl_spreadTroubleDamage(BtlUnit *arg0) {
    BtlUnit *u;
    s32 d;
    s32 v;
    for (u = g_gameState->unk1C->unk8E4.battle->unitList; u != 0; u = u->next) {
        if ((u->unk68 & 1) == (arg0->unk68 & 1) && u != arg0 && ((u->unk68 >> 28) & 1)) {
            u->popupFlags = 1;
            d = arg0->hpChange >> 1;
            if (u->status & BTL_STATUS_KO) {
                u->popupFlags = 0x20;
            } else if (u->status & BTL_STATUS_PETRIFY) {
                u->hpChange = 0;
            } else {
                if (u != g_gameState->unk1C->unk8E4.battle->curCmd->caster) {
                    v = (u->unk68 & 1) << 11;
                    u->unkA0->unk22 = v;
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

void func_800BD8C0(Unk800BD8C0 *arg0, Unk800BD8C0 *arg1) {
    arg0->unk0 = arg1->unk0;
    arg0->unk2 = arg1->unk2;
    arg0->unk4 = arg1->unk4;
    arg0->unk7 = arg1->unk7;
}

void btl_resetAtbSpeed(Unk800BD8F0 *arg0) {
    s32 v;
    arg0->unk2E = 10;
    v = (g_gameState->unk24 >> 8) & 7;
    if (v == 0) {
        arg0->unk2E = 8;
    } else if (v == 2) {
        arg0->unk2E = 14;
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
        if (arg0->unk7D == 0) {
            btl_addStatus(arg0, BTL_STATUS_KO);
        }
    } else {
        if (!(arg0->status & BTL_STATUS_LOW_HP)) {
            btl_addStatus(arg0, BTL_STATUS_LOW_HP);
        }
    clamp:
        arg0->mp = arg0->max->maxMp < arg0->mp ? arg0->max->maxMp : arg0->mp;
        if (arg0->unk68 & 1) {
            arg0->unk68 = (arg0->unk68 & ~0x40000) | ((btl_hasStatus(arg0, (BTL_STATUS_VENOM | BTL_STATUS_LOW_HP | BTL_STATUS_POISON | BTL_STATUS_SLEEP)) & 1) << 18);
        }
    }
}

void func_800BDA60(BtlUnit *arg0) {
    s32 d = 0x01900000;
    u32 f;
    u32 v;
    u32 m;
    if ((arg0->unk68 >> 8) & 3) {
        d = 0xFE700000;
    }
    arg0->pos[2] = arg0->unk98->unk0[2] + d;
    arg0->unk98->unk0[2] += d;
    f = arg0->unk68;
    m = f & ~0x300;
    v = (f >> 8) & 3;
    v = v == 0;
    arg0->unk68 = m | (v << 8);
}

void func_800BDACC(u8 *arg0, u8 *arg1) {
    arg0[0] |= arg1[0];
    arg0[1] |= arg1[1];
    arg0[2] |= arg1[2];
    arg0[3] |= arg1[3];
}

void btl_damageHp(BtlUnit *arg0, s32 arg1, u8 arg2) {
    s32 v;
    if (arg0->status & BTL_STATUS_KO) {
        arg0->popupFlags = 0x20;
        return;
    }
    if (arg0->status & BTL_STATUS_PETRIFY) {
        arg0->hpChange = 0;
        return;
    }
    if (arg0 != g_gameState->unk1C->unk8E4.battle->curCmd->caster) {
        v = (arg0->unk68 & 1) << 11;
        arg0->unkA0->unk22 = v;
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
        arg0->popupFlags = 0x20;
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
        arg0->popupFlags = 0x20;
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
        arg0->popupFlags = 0x20;
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
    arg0->tickFlags |= 2;
}

void btl_tickRegen(BtlUnit *arg0) {
    s32 v = 0;
    if (!(arg0->status & BTL_STATUS_PETRIFY)) {
        v = arg0->max->maxHp >> 4;
        if ((arg0->status & BTL_STATUS_ZOMBIE) || btl_hasEnemyFlag(arg0, 0x10) != 0) {
            arg0->tickFlags |= 8;
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
    arg0->tickFlags |= 1;
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
    arg0->tickFlags |= 4;
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
        if (arg0->unk68 & 1) {
            r = (g_gameState->unk1C->characters + ((arg0->unk68 >> 1) & 0xF))->level;
        } else {
            r = g_gameState->unk1C->unk8E4.battle->enemies[(arg0->unk68 >> 1) & 0xF].unk0->unk3;
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
    case 0x34:
        r = arg0->unk68 >> 28;
        r &= 1;
        break;
    case 0x35:
        r = arg0->unk68 >> 22;
        r &= 1;
        break;
    case BTL_ATTR_MODEL:
        r = (s16)arg0->modelId;
        break;
    case 0x3A:
        r = arg0->hiddenMeshes;
        break;
    case 0x40:
        r = arg0->unk68 >> 8;
        r &= 3;
        break;
    case 0x41:
        r = (u8)arg0->unk68;
        r >>= 5;
        break;
    case 0x42:
        r = (g_gameState->unk1C->characters + ((arg0->unk68 >> 1) & 0xF))->unk34.w >> 4;
        r &= 0x1F;
        break;
    case 0x43:
        r = (g_gameState->unk1C->characters + ((arg0->unk68 >> 1) & 0xF))->unkA;
        break;
    case 0x44:
        r = g_gameState->unk1C->unk8E4.battle->enemies[(arg0->unk68 >> 1) & 0xF].unk0->unk2;
        break;
    case 0x45:
        r = arg0->unk68 >> 18;
        r &= 1;
        break;
    case 0x46:
        r = arg0->unk68 >> 1;
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
    case 0x34:
        if (g_gameState->unk1C->unk8E4.battle->phase == BTL_PHASE_INTRO) {
            arg0->unkB2 = 0;
            arg0->unk68 &= ~0x10000000;
            arg0->unk68 &= ~0x10000;
            arg0->unk68 &= ~0x20000000;
        } else {
            arg0->unkB2 = arg2 + 2;
        }
        break;
    case 0x35:
        arg0->unk68 = (arg0->unk68 & ~0x400000) | ((arg2 & 1) << 22);
        if (arg2 != 0 && g_gameState->unk1C->unk8E4.battle->phase == BTL_PHASE_INTRO) {
            func_800C2344(arg0->model);
            k = btl_getModelSlot(arg0);
            g_gameState->unk1C->unk8E4.battle->unk11C0->unk28[k].unk0_0 = btlbg_getHeader()->unkF;
        }
        break;
    case 0x36:
        arg0->unk68 = (arg0->unk68 & ~0x1000000) | ((arg2 & 1) << 24);
        break;
    case BTL_ATTR_SCALE:
        arg0->model->unk0 |= 2;
        arg0->model->unk54 = arg2;
        arg0->model->unk56 = arg2;
        arg0->model->unk58 = arg2;
        k = btl_getModelSlot(arg0);
        g_gameState->unk1C->unk8E4.battle->unk11C0->unk28[k].unk0_8 = ((u32)(arg0->unkBF * arg2) >> 12) * 14;
        k = btl_getModelSlot(arg0);
        g_gameState->unk1C->unk8E4.battle->unk11C0->unk28[k].unk0_20 = ((u32)(arg0->unkC0 * arg2) >> 12) * 12;
        break;
    case BTL_ATTR_RESET_SCALE:
        arg0->model->unk0 &= ~2;
        k = btl_getModelSlot(arg0);
        g_gameState->unk1C->unk8E4.battle->unk11C0->unk28[k].unk0_8 = arg0->unkBF * 14;
        k = btl_getModelSlot(arg0);
        g_gameState->unk1C->unk8E4.battle->unk11C0->unk28[k].unk0_20 = arg0->unkC0 * 12;
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
        ((BtlUnit9C50 *)((u8 *)arg0->model->unk50 + off))->unk4 |= 1;
        ((BtlUnit9C50 *)((u8 *)arg0->model->unk50 + off))->unkC = 0;
        ((BtlUnit9C50 *)((u8 *)arg0->model->unk50 + off))->unk12 = 0x1000;
        break;
    case 0x3E:
        w = arg2 * 20;
        ((BtlUnit9C50 *)((u8 *)arg0->model->unk50 + w))->unk4 &= ~3;
        break;
    case 0x3F:
        x = arg2 * 20;
        ((BtlUnit9C50 *)((u8 *)arg0->model->unk50 + x))->unk4 |= 3;
        ((BtlUnit9C50 *)((u8 *)arg0->model->unk50 + x))->unkC = 0;
        ((BtlUnit9C50 *)((u8 *)arg0->model->unk50 + x))->unk12 = 0x1000;
        break;
    case 0x45:
        arg0->unk68 = (arg0->unk68 & ~0x40000) | ((arg2 & 1) << 18);
        break;
    case BTL_ATTR_ADD_REWARDS:
        btl_addEnemyRewards(g_gameState->unk1C->unk8E4.battle->enemies[(arg0->unk68 >> 1) & 0xF].unk0);
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
        btl_setCmd(arg0->cmd, BTL_CMD_SUMMON2, BTL_ABIL_TERRA_HOMING, (u16)arg2, 8);
        btlui_enable(1);
        btlui_setAtbEnabled(1);
        break;
    }
}

s32 btl_getBattleVar(s32 arg0) {
    Battle *b = g_gameState->unk1C->unk8E4.battle;
    s32 r = -1;
    BtlUnit *u;
    s32 k;
    switch (arg0) {
    case 32:
        r = b->scene.unk0 & 3;
        break;
    case 33:
        r = g_gameState->unk1C->gil;
        break;
    case 34:
        r = b->phase;
        break;
    case 35:
        r = b->subPhase;
        break;
    case 36:
        if (b->curCmd != NULL && b->curCmd->caster != NULL) {
            r = b->curCmd->caster->unitBit;
        }
        break;
    case 37:
        if (b->curCmd != NULL) {
            r = (u16)b->curCmd->targets;
        }
        break;
    case 38:
        if (b->curCmd != NULL && b->curCmd->caster != NULL && b->curCmd->caster->weapon != NULL) {
            r = b->curCmd->caster->weapon[6];
        }
        break;
    case 39:
        if (b->curCmd != NULL && b->curCmd->caster != NULL) {
            r = 0;
            if (!(b->curCmd->caster->support[1] & 1)) {
                r = b->curCmd->ability->element;
            }
        }
        break;
    case 40:
        r = b->loadFlags;
        break;
    case 41:
        r = D_800FDF34;
        break;
    case 42:
        r = 0;
        for (u = b->unitList; u != NULL; u = u->next) {
            if (u->unk68 & 1) {
                k = g_gameState->unk1C->characters[(u->unk68 >> 1) & 0xF].unk34.b.unk4;
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
    Battle *b = g_gameState->unk1C->unk8E4.battle;
    BtlUnit *u;
    u32 f;

    switch (arg0) {
    case 32:
        btlui_setAtbEnabled(0);
        b->phase = BTL_PHASE_END;
        b->escapeHeld = 0;
        b->subPhase = 2;
        b->cmdFlags &= ~1;
        btl_clearCmdQueue(b);
        for (u = b->unitList; u != NULL; u = u->next) {
            u->unk68 &= ~0x80000;
        }
        break;
    case 33:
        if (b->phase == BTL_PHASE_SCRIPT) {
            g_gameState->unk1C->unk56E = arg1;
            if (arg1 == 1 && ((b->scene.unk0 >> 6) & 1)) {
                b->phase = BTL_PHASE_END;
                b->subPhase = 4;
            } else {
                if (g_gameState->unk1C->unk56E == 1) {
                    g_gameState->unk1C->unk56E = 2;
                }
                b->phase = BTL_PHASE_EXIT_OTHER;
                b->subPhase = 0;
            }
        }
        break;
    case 34:
        if (b->phase == BTL_PHASE_SCRIPT) {
            b->phase = BTL_PHASE_EXIT_DEFEAT;
            b->subPhase = 0;
            g_gameState->unk1C->unk56E = 3;
        }
        break;
    case 35:
        if (b->phase == BTL_PHASE_SCRIPT) {
            f = b->scene.unk0;
            b->phase = BTL_PHASE_ENTRANCE;
            b->subPhase = 0;
            if ((f & 0xC) != 0xC) {
                b->scene.unk0 = (f & ~3) | 2;
            }
        }
        break;
    case 36:
        if (b->phase == BTL_PHASE_SCRIPT) {
            btlcam_playScene(func_800CA2B0(arg1));
        }
        break;
    case 37:
        g_gameState->unk1C->unk56F |= 1;
        evt_setNextLocation(arg1);
        break;
    case 38:
        g_gameState->unk1C->gil += arg1;
        if (g_gameState->unk1C->gil > 9999999) {
            g_gameState->unk1C->gil = 9999999;
        }
        break;
    case 39:
        if (g_gameState->unk1C->unk56C < 9999) {
            g_gameState->unk1C->unk56C++;
        }
        break;
    case 40:
        btl_soundCmd(0x8601, 0, 0x1E, 0, 0);
        g_gameState->unk1C->unk56F |= 0x10;
        break;
    }
}
