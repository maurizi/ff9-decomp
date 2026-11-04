#include "ovl_0b.h"

void btl_setShadowSize(BtlUnit *unit, s32 width, u32 depth) {
    s32 slot;
    slot = unit->flags >> BTL_UNIT_INDEX_SHIFT;
    slot &= 0xF;
    if (!(unit->flags & BTL_UNIT_PARTY)) {
        slot += 9;
    }
    unit->shadowW = (width & 0xFFFF) / 14;
    unit->shadowD = depth / 12;
    g_gameState->gameData->ctx.battle->work->shadows[slot].shade = 0;
    g_gameState->gameData->ctx.battle->work->shadows[slot].width = unit->shadowW * 14;
    g_gameState->gameData->ctx.battle->work->shadows[slot].depth = unit->shadowD * 12;
}

void btl_setUnitFade(BtlUnit *unit, s32 level) {
    BtlBgHeader *bg = btlbg_getHeader();
    btl_setModelAbr(unit->model, TPAGE_ABR(3));
    btl_fillModelRgb(unit->model, (bg->lightR * level) << 11 >> 16, (bg->lightG * level) << 11 >> 16, (bg->lightB * level) << 11 >> 16);
    if (level == 0) {
        unit->flags |= BTL_UNIT_HIDDEN;
    }
}

void btl_playCmdEffect(BtlCmd *cmd, s32 effectId, s16 *fxArgs) {
    Battle *battle = g_gameState->gameData->ctx.battle;
    s32 sum[4];
    BtlUnitList *list;
    BtlUnit *unit;
    BtlUnit *caster;
    s32 bounceMask;
    u8 cmdNo;

    memset(sum, 0, 0x10);
    bounceMask = 0;
    battle->fxTargets.caster = cmd->caster;
    list = &battle->fxTargets;
    if (fxArgs != NULL) {
        list->fxArgs[0] = fxArgs[0];
        list->fxArgs[1] = fxArgs[1];
        list->fxParam = fxArgs[2];
        list->kind = fxArgs[3];
    } else {
        list->fxParam = 0;
        list->kind = 0;
        list->fxArgs[1] = 0;
        list->fxArgs[0] = 0;
    }
    list->bounceCount = 0;
    list->targetCount = 0;
    caster = cmd->caster;
    caster->popupFlags = 0;
    caster->mpChange = 0;
    caster->hpChange = 0;
    if (cmd->info.b.reflect == 0) {
        bounceMask = btl_applyReflect(cmd);
        cmdNo = cmd->cmdNo;
        if (cmdNo == BTL_CMD_ITEM || cmdNo == BTL_CMD_AUTO_POTION) {
            list->kind = BTL_FXK_ITEM;
        } else if (cmdNo == BTL_CMD_SWD_MAG) {
            list->kind = BTL_FXK_SWD_MAG;
        } else if ((u8)(cmdNo - BTL_CMD_TRANCE_EIDOLON) < 2) { /* TRANCE_EIDOLON, REBIRTH_FLAME */
            list->kind = BTL_FXK_NO_CAST;
        }
    } else if (cmd->info.b.reflect == 2) {
        cmd->info.b.reflect = 1;
        if (!(cmd->caster->flags & BTL_UNIT_PARTY)) {
            cmd->info.b.enemyBounce = 1;
        }
        list->kind = (BTL_FXK_NO_CAST | BTL_FXK_BOUNCE);
    }
    for (unit = g_gameState->gameData->ctx.battle->unitList; unit != NULL; unit = unit->next) {
        if (unit->unitBit & (u16)cmd->targets) {
            list->targets[list->targetCount++] = unit;
            unit->popupFlags = 0;
            unit->mpChange = 0;
            unit->hpChange = 0;
            sum[0] += unit->home->pos[0] >> 16;
            sum[2] += unit->home->pos[2] >> 16;
        }
        if (unit->unitBit & bounceMask) {
            list->bounced[list->bounceCount++] = unit;
        }
        if (cmd->cmdNo == BTL_CMD_SWD_MAG && (unit->flags & BTL_UNIT_PARTY) && g_gameState->gameData->characters[BTL_UNIT_INDEX(unit->flags)].info.b.kind == CHR_KIND_VIVI) {
            list->partner = unit;
        }
    }
    list->centerX = sum[0] / (s8)list->targetCount;
    list->centerY = 0;
    list->centerZ = sum[2] / (s8)list->targetCount;
    btlfx_resetAll();
    btlfx_setTargets(list);
    btlfx_start(effectId);
}

void btl_startCmdEffect(BtlCmd *cmd) {
    BtlUnit *caster = cmd->caster;
    s32 id;
    s32 k;
    u16 cover;

    switch (cmd->cmdNo) {
    case BTL_CMD_ATTACK:
    case BTL_CMD_COUNTER:
    case BTL_CMD_CHARGE_ATTACK:
        if (caster->flags & BTL_UNIT_PARTY) {
            id = g_gameState->gameData->characters[BTL_UNIT_INDEX(caster->flags)].info.b.kind + BTLFX_EFFECT_ATTACK;
        } else {
            id = BTLFX_EFFECT_ATTACK_ENEMY;
        }
        btl_playCmdEffect(cmd, id, 0);
        break;
    case BTL_CMD_STEAL:
        if (caster->flags & BTL_UNIT_PARTY) {
            k = g_gameState->gameData->characters[BTL_UNIT_INDEX(caster->flags)].info.b.kind;
        } else {
            k = CHR_KIND_NONE;
        }
        switch (k) {
        case CHR_KIND_ZIDANE_DAGGER:
            btl_playCmdEffect(cmd, BTLFX_EFFECT_STEAL_ZIDANE_DAGGER, 0);
            break;
        case CHR_KIND_ZIDANE:
            btl_playCmdEffect(cmd, BTLFX_EFFECT_STEAL_ZIDANE, 0);
            break;
        case CHR_KIND_CINNA:
            btl_playCmdEffect(cmd, BTLFX_EFFECT_STEAL_CINNA, 0);
            break;
        case CHR_KIND_MARCUS:
            btl_playCmdEffect(cmd, BTLFX_EFFECT_ATTACK_ENEMY, 0);
            break;
        default:
            btl_playCmdEffect(cmd, BTLFX_EFFECT_STEAL, 0);
            break;
        }
        break;
    case BTL_CMD_DEFEND:
        btl_showCmdName(cmd);
        caster->model->motion = (BtlMotion *)caster->motions[BTL_MOTION_DEFEND];
        caster->obj->frame = 0;
        btl_execCmd(caster);
        return;
    case BTL_CMD_CHANGE:
        btl_showCmdName(cmd);
        caster->model->motion = (BtlMotion *)caster->motions[BTL_MOTION_READY];
        return;
    case BTL_CMD_ITEM:
    case BTL_CMD_AUTO_POTION:
        btl_playCmdEffect(cmd, g_usableItemTable[cmd->subNo - ITEM_FIRST_USABLE].effect, 0);
        break;
    case BTL_CMD_THROW:
        /* padded: the first jump table is 8-aligned in its object */
        __asm__(".section .rodata\n\t.word 0\n\t.section .text");
        switch (g_itemTable[cmd->subNo].icon) {
        case ITEM_ICON_DAGGER:
            btl_playCmdEffect(cmd, BTLFX_EFFECT_THROW_DAGGER, 0);
            break;
        case ITEM_ICON_THIEF_SWORD:
            btl_playCmdEffect(cmd, BTLFX_EFFECT_THROW_THIEF_SWORD, 0);
            break;
        case ITEM_ICON_SWORD:
        case ITEM_ICON_KNIGHT_SWORD:
            btl_playCmdEffect(cmd, BTLFX_EFFECT_THROW_SWORD, 0);
            break;
        case ITEM_ICON_SPEAR:
            btl_playCmdEffect(cmd, BTLFX_EFFECT_THROW_SPEAR, 0);
            break;
        case ITEM_ICON_CLAW:
            btl_playCmdEffect(cmd, BTLFX_EFFECT_THROW_CLAW, 0);
            break;
        case ITEM_ICON_RACKET:
            btl_playCmdEffect(cmd, BTLFX_EFFECT_THROW_RACKET, 0);
            break;
        case ITEM_ICON_ROD:
        case ITEM_ICON_FLUTE:
        case ITEM_ICON_STAFF:
            btl_playCmdEffect(cmd, BTLFX_EFFECT_THROW_ROD, 0);
            break;
        case ITEM_ICON_FORK:
            btl_playCmdEffect(cmd, BTLFX_EFFECT_THROW_FORK, 0);
            break;
        case ITEM_ICON_THROWING:
            btl_playCmdEffect(cmd, BTLFX_EFFECT_THROW_THROWING, 0);
            break;
        }
        break;
    case BTL_CMD_ENEMY_TURN:
        if ((u16)cmd->targets < BTL_TARGETS_FIRST_ENEMY && (cmd->ability->flags & BTL_ABIL_PHYSICAL) && !cmd->info.b.targeting) {
            cover = btl_findCover((u16)cmd->targets);
            if (cover) {
                cmd->targets = cover;
                cmd->info.b.covered = 1;
            }
        }
    case BTL_CMD_ENEMY_COUNTER:
    case BTL_CMD_ENEMY_DEATH:
    case BTL_CMD_ENEMY_REACT:
        btl_startEnemySeq(cmd);
        return;
    case BTL_CMD_TRANCE:
        id = BTLFX_EFFECT_TRANCE_CMD;
        if (caster->status & BTL_STATUS_TRANCE) {
            id = BTLFX_EFFECT_TRANCE;
        }
        btl_playCmdEffect(cmd, id, 0);
        break;
    case BTL_CMD_EIDOLON:
        switch (cmd->subNo) {
        case ABIL_SHIVA:
            g_gameState->gameData->ctx.battle->eidolonAbility = ABIL_DIAMOND_DUST;
            break;
        case ABIL_IFRIT:
            g_gameState->gameData->ctx.battle->eidolonAbility = ABIL_FLAMES_OF_HELL;
            break;
        case ABIL_RAMUH:
            g_gameState->gameData->ctx.battle->eidolonAbility = ABIL_JUDGEMENT_BOLT;
            break;
        case ABIL_ATOMOS:
            g_gameState->gameData->ctx.battle->eidolonAbility = ABIL_WORM_HOLE;
            break;
        case ABIL_ODIN:
            g_gameState->gameData->ctx.battle->eidolonAbility = ABIL_ZANTETSUKEN;
            break;
        case ABIL_LEVIATHAN:
            g_gameState->gameData->ctx.battle->eidolonAbility = ABIL_TSUNAMI;
            break;
        case ABIL_BAHAMUT:
            g_gameState->gameData->ctx.battle->eidolonAbility = ABIL_MEGA_FLARE;
            break;
        case ABIL_ARK:
            g_gameState->gameData->ctx.battle->eidolonAbility = ABIL_ETERNAL_DARKNESS;
            break;
        }
    default:
        if ((cmd->ability->targetType == BTL_TGT_MULTI_ANY && !cmd->info.b.targeting) || (cmd->info.w & (BTL_CMDINFO_MAGIC_FAILED | BTL_CMDINFO_SHORT_SUMMON)) ||
            cmd->cmdNo == BTL_CMD_SEIKEN || cmd->cmdNo == BTL_CMD_SEIKEN2) {
            btl_playCmdEffect(cmd, cmd->ability->altEffect, 0);
        } else {
            btl_playCmdEffect(cmd, cmd->ability->effect, 0);
        }
        break;
    }
}
