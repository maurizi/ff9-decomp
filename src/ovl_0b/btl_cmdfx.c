#include "ovl_0b.h"

void btl_setShadowSize(BtlUnit *arg0, s32 arg1, u32 arg2) {
    s32 k;
    k = arg0->unk68 >> 1;
    k &= 0xF;
    if (!(arg0->unk68 & 1)) {
        k += 9;
    }
    arg0->shadowW = (arg1 & 0xFFFF) / 14;
    arg0->shadowD = arg2 / 12;
    g_gameState->unk1C->unk8E4.battle->unk11C0->unk28[k].shade = 0;
    g_gameState->unk1C->unk8E4.battle->unk11C0->unk28[k].width = arg0->shadowW * 14;
    g_gameState->unk1C->unk8E4.battle->unk11C0->unk28[k].depth = arg0->shadowD * 12;
}

void btl_setUnitFade(BtlUnit *arg0, s32 arg1) {
    Unk800CEE74 *p = btlbg_getHeader();
    btl_setModelAbr(arg0->model, 0x60);
    btl_fillModelRgb(arg0->model, (p->unkC * arg1) << 11 >> 16, (p->unkD * arg1) << 11 >> 16, (p->unkE * arg1) << 11 >> 16);
    if (arg1 == 0) {
        arg0->unk68 |= 0x400000;
    }
}

void btl_playCmdEffect(BtlCmd *arg0, s32 arg1, s16 *arg2) {
    Battle *b = g_gameState->unk1C->unk8E4.battle;
    s32 sum[4];
    BtlUnitList *l;
    BtlUnit *u;
    BtlUnit *c;
    s32 mask;
    u8 e;

    memset(sum, 0, 0x10);
    mask = 0;
    b->fxTargets.caster = arg0->caster;
    l = &b->fxTargets;
    if (arg2 != NULL) {
        l->unk25[0] = arg2[0];
        l->unk25[1] = arg2[1];
        l->unk42 = arg2[2];
        l->kind = arg2[3];
    } else {
        l->unk42 = 0;
        l->kind = 0;
        l->unk25[1] = 0;
        l->unk25[0] = 0;
    }
    l->bounceCount = 0;
    l->targetCount = 0;
    c = arg0->caster;
    c->popupFlags = 0;
    c->mpChange = 0;
    c->hpChange = 0;
    if (arg0->unk10.b.reflect == 0) {
        mask = btl_applyReflect(arg0);
        e = arg0->cmdNo;
        if (e == 0xE || e == 0x33) {
            l->kind = 2;
        } else if (e == 0x1F) {
            l->kind = 4;
        } else if ((u8)(e - 0x39) < 2) {
            l->kind = 1;
        }
    } else if (arg0->unk10.b.reflect == 2) {
        arg0->unk10.b.reflect = 1;
        if (!(arg0->caster->unk68 & 1)) {
            arg0->unk10.b.unk0_11 = 1;
        }
        l->kind = 0x11;
    }
    for (u = g_gameState->unk1C->unk8E4.battle->unitList; u != NULL; u = u->next) {
        if (u->unitBit & (u16)arg0->targets) {
            l->targets[l->targetCount++] = u;
            u->popupFlags = 0;
            u->mpChange = 0;
            u->hpChange = 0;
            sum[0] += u->unk98->unk0[0] >> 16;
            sum[2] += u->unk98->unk0[2] >> 16;
        }
        if (u->unitBit & mask) {
            l->bounced[l->bounceCount++] = u;
        }
        if (arg0->cmdNo == BTL_CMD_SWD_MAG && (u->unk68 & 1) && g_gameState->unk1C->characters[(u->unk68 >> 1) & 0xF].unk34.b.unk4 == 2) {
            l->partner = u;
        }
    }
    l->centerX = sum[0] / (s8)l->targetCount;
    l->centerY = 0;
    l->centerZ = sum[2] / (s8)l->targetCount;
    btlfx_resetAll();
    btlfx_setTargets(l);
    btlfx_start(arg1);
}

void btl_startCmdEffect(BtlCmd *cmd) {
    BtlUnit *u = cmd->caster;
    s32 id;
    s32 k;
    u16 t;

    switch (cmd->cmdNo) {
    case BTL_CMD_ATTACK:
    case BTL_CMD_COUNTER:
    case BTL_CMD_CHARGE_ATTACK:
        if (u->unk68 & 1) {
            id = g_gameState->unk1C->characters[(u->unk68 >> 1) & 0xF].unk34.b.unk4 + 0x64;
        } else {
            id = 0x77;
        }
        btl_playCmdEffect(cmd, id, 0);
        break;
    case BTL_CMD_STEAL:
        if (u->unk68 & 1) {
            k = g_gameState->unk1C->characters[(u->unk68 >> 1) & 0xF].unk34.b.unk4;
        } else {
            k = 0x13;
        }
        switch (k) {
        case 0:
            btl_playCmdEffect(cmd, 0xC8, 0);
            break;
        case 1:
            btl_playCmdEffect(cmd, 0x111, 0);
            break;
        case 0xE:
            btl_playCmdEffect(cmd, 0x13, 0);
            break;
        case 0xF:
            btl_playCmdEffect(cmd, 0x77, 0);
            break;
        default:
            btl_playCmdEffect(cmd, 0x14, 0);
            break;
        }
        break;
    case BTL_CMD_DEFEND:
        btl_showCmdName(cmd);
        u->model->unkC = (BtlUnit9CC *)u->motions[12];
        u->unkA0->unk3A = 0;
        btl_execCmd(u);
        return;
    case BTL_CMD_CHANGE:
        btl_showCmdName(cmd);
        u->model->unkC = (BtlUnit9CC *)u->motions[9];
        return;
    case BTL_CMD_ITEM:
    case BTL_CMD_AUTO_POTION:
        btl_playCmdEffect(cmd, D_8006B1F4[cmd->subNo - 0xE0].unk0_8, 0);
        break;
    case BTL_CMD_THROW:
        /* padded: the first jump table is 8-aligned in its object */
        __asm__(".section .rodata\n\t.word 0\n\t.section .text");
        switch (D_80069018[cmd->subNo].unk8) {
        case 1:
            btl_playCmdEffect(cmd, 0x110, 0);
            break;
        case 2:
            btl_playCmdEffect(cmd, 0x10A, 0);
            break;
        case 3:
        case 4:
            btl_playCmdEffect(cmd, 0x10B, 0);
            break;
        case 5:
            btl_playCmdEffect(cmd, 0x10C, 0);
            break;
        case 6:
            btl_playCmdEffect(cmd, 0x10D, 0);
            break;
        case 7:
            btl_playCmdEffect(cmd, 0x109, 0);
            break;
        case 8:
        case 9:
        case 10:
            btl_playCmdEffect(cmd, 0x10E, 0);
            break;
        case 11:
            btl_playCmdEffect(cmd, 0x10F, 0);
            break;
        case 12:
            btl_playCmdEffect(cmd, 0x115, 0);
            break;
        }
        break;
    case BTL_CMD_ENEMY_TURN:
        if ((u16)cmd->targets < 0x10 && (cmd->ability->flags & 8) && !cmd->unk10.b.targeting) {
            t = btl_findCover((u16)cmd->targets);
            if (t) {
                cmd->targets = t;
                cmd->unk10.b.covered = 1;
            }
        }
    case BTL_CMD_ENEMY_COUNTER:
    case BTL_CMD_ENEMY_DEATH:
    case BTL_CMD_ENEMY_REACT:
        btl_startEnemySeq(cmd);
        return;
    case BTL_CMD_TRANCE:
        id = 0x1E9;
        if (u->status & BTL_STATUS_TRANCE) {
            id = 0x101;
        }
        btl_playCmdEffect(cmd, id, 0);
        break;
    case BTL_CMD_EIDOLON:
        switch (cmd->subNo) {
        case 0x31:
            g_gameState->unk1C->unk8E4.battle->eidolonAbility = 0x99;
            break;
        case 0x33:
            g_gameState->unk1C->unk8E4.battle->eidolonAbility = 0x9A;
            break;
        case 0x35:
            g_gameState->unk1C->unk8E4.battle->eidolonAbility = 0x9B;
            break;
        case 0x37:
            g_gameState->unk1C->unk8E4.battle->eidolonAbility = 0x9C;
            break;
        case 0x3A:
            g_gameState->unk1C->unk8E4.battle->eidolonAbility = 0x9D;
            break;
        case 0x3C:
            g_gameState->unk1C->unk8E4.battle->eidolonAbility = 0x9E;
            break;
        case 0x3E:
            g_gameState->unk1C->unk8E4.battle->eidolonAbility = 0x9F;
            break;
        case 0x40:
            g_gameState->unk1C->unk8E4.battle->eidolonAbility = 0xA0;
            break;
        }
    default:
        if ((cmd->ability->targetType == 3 && !cmd->unk10.b.targeting) || (cmd->unk10.w & 0x600) ||
            cmd->cmdNo == BTL_CMD_SEIKEN || cmd->cmdNo == BTL_CMD_SEIKEN2) {
            btl_playCmdEffect(cmd, cmd->ability->altEffect, 0);
        } else {
            btl_playCmdEffect(cmd, cmd->ability->effect, 0);
        }
        break;
    }
}
