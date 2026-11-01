#include "ff9.h"

u8 item_useOnMember(Character *caster, Character *target, MainCalcCmd *cmd, u8 kind, s32 all) {
    MainCalc calc;

    calc.caster = caster;
    calc.target = target;
    calc.cmd = cmd;
    calc.all = all;
    calc.flags = 0;
    calc.mpChange = 0;
    calc.hpChange = 0;
    switch (kind) {
    case BTL_FORMULA_MAGIC_HEAL:
        if (fieldCalc_checkAlive(&calc)) {
            if (target->hp == target->maxHp) {
                goto full;
            }
            fieldCalc_setMagicHeal(&calc);
            fieldCalc_supportBoost(&calc);
            fieldCalc_casterStatusHalf(&calc);
            fieldCalc_multiTargetHalf(&calc);
            fieldCalc_calcHpGain(&calc);
        }
        break;
    case BTL_FORMULA_CURE_STATUS:
        fieldCalc_cureStatusType(&calc);
        break;
    case BTL_FORMULA_REVIVE:
        if (fieldCalc_checkDead(&calc)) {
            fieldCalc_calcReviveHp(&calc);
        }
        break;
    case BTL_FORMULA_ITEM_HEAL:
        if (fieldCalc_checkAlive(&calc)) {
            if (target->hp == target->maxHp) {
                goto full;
            }
            fieldCalc_setItemHeal(&calc);
            fieldCalc_calcHpGain(&calc);
        }
        break;
    case BTL_FORMULA_ITEM_MP_HEAL:
        if (fieldCalc_checkAlive(&calc)) {
            if (target->mp == target->maxMp) {
                goto full;
            }
            fieldCalc_setItemHeal(&calc);
            fieldCalc_calcMpGain(&calc);
        }
        break;
    case BTL_FORMULA_ELIXIR:
        if (fieldCalc_checkAlive(&calc)) {
            if (*(u32 *)&target->hp == *(u32 *)&target->maxHp) { /* hp and mp both full */
            full:
                calc.flags |= MAINCALC_MISS;
                break;
            }
            fieldCalc_restoreAll(&calc);
        }
        break;
    case BTL_FORMULA_PHOENIX_DOWN:
        if (fieldCalc_checkDead(&calc)) {
            fieldCalc_reviveRandom(&calc);
        }
        break;
    case BTL_FORMULA_SOFT:
    case BTL_FORMULA_ITEM_CURE_STATUS:
        if (((cmd->info >> 8) & 0x1FF) == 0x121) {
            return menuItem_callChocobo();
        }
        fieldCalc_cureStatus(&calc);
        break;
    case BTL_FORMULA_TENT:
        if (fieldCalc_checkAlive(&calc)) {
            fieldCalc_restoreHalf(&calc);
        }
        break;
    }
    return fieldCalc_apply(&calc);
}

s32 chr_payMp(Unk8006573C *chr, s16 cost) {
    if (chr->mp < cost) {
        return 0;
    }
    chr->mp -= cost;
    return 1;
}

u8 fieldCalc_apply(MainCalc *calc) {
    if (calc->flags & MAINCALC_MISS) {
        calc->result |= MAINCALC_RESULT_MISS;
        return 0;
    }
    if (calc->hpChange > 0) {
        chr_addHp(calc->target, calc->hpChange);
    }
    if (calc->mpChange > 0) {
        chr_addMp(calc->target, calc->mpChange);
    }
    return 1;
}

u8 fieldCalc_checkAlive(MainCalc *calc) {
    if (chr_hasStatus(calc->target, CHR_STATUS_PETRIFY | CHR_STATUS_ZOMBIE) || calc->target->hp == 0) {
        calc->flags |= MAINCALC_MISS;
        return 0;
    }
    return 1;
}

u8 fieldCalc_checkDead(MainCalc *calc) {
    if (chr_hasStatus(calc->target, CHR_STATUS_PETRIFY) || calc->target->hp != 0 ||
        (chr_hasStatus(calc->target, CHR_STATUS_ZOMBIE) && calc->target->hp == 0)) {
        calc->flags |= MAINCALC_MISS;
        return 0;
    }
    return 1;
}

void fieldCalc_setMagicHeal(MainCalc *calc) {
    Character *caster;
    s32 lv;

    if (g_randPtr >= g_randTableEnd) {
        rand_refill();
    }
    if (g_randPtr >= g_randTableEnd) {
        rand_refill();
    }
    caster = calc->caster;
    lv = caster->magic;
    calc->multiplier = lv + ((*g_randPtr << 8) | *g_randPtr) % (((caster->level + lv) >> 3) + 1);
    g_randPtr++;
    calc->power = calc->cmd->power;
    calc->defense = calc->target->magicDef;
}

void fieldCalc_setItemHeal(MainCalc *calc) {
    calc->multiplier = 10;
    calc->power = calc->cmd->power;
    calc->defense = 0;
}

void fieldCalc_casterStatusHalf(MainCalc *calc) {
    if (chr_hasStatus(calc->caster, 0)) {
        calc->multiplier /= 2;
    }
}

void fieldCalc_supportBoost(MainCalc *calc) {
    if (calc->caster->supportAbilities[1] & SA_BIT(SA_CONCENTRATE)) {
        calc->multiplier = (calc->multiplier * 3) >> 1;
    }
}

void fieldCalc_multiTargetHalf(MainCalc *calc) {
    if (calc->all == 1 && (u32)((calc->cmd->info & 0xF) - BTL_TGT_MULTI_ANY) < 3) {
        calc->multiplier /= 2;
    }
}

void fieldCalc_calcHpGain(MainCalc *calc) {
    u16 gain = calc->power * calc->multiplier;
    if ((s16)gain >= 10000) {
        gain = 9999;
    }
    calc->hpChange = gain;
}

void fieldCalc_calcReviveHp(MainCalc *calc) {
    Character *target = calc->target;
    s32 heal = target->maxHp * (target->spirit + calc->cmd->power);

    if (calc->caster->supportAbilities[1] & SA_BIT(SA_CONCENTRATE)) {
        heal /= 50;
    } else {
        heal /= 100;
    }
    if (heal >= 10000) {
        heal = 9999;
    }
    calc->hpChange = heal;
}

void fieldCalc_calcMpGain(MainCalc *calc) {
    u16 gain = calc->power * calc->multiplier;
    if ((s16)gain >= 10000) {
        gain = 9999;
    }
    calc->mpChange = gain;
}

void fieldCalc_restoreAll(MainCalc *calc) {
    calc->target->hp = calc->target->maxHp;
    calc->target->mp = calc->target->maxMp;
}

void fieldCalc_reviveRandom(MainCalc *calc) {
    Character *target = calc->target;
    s32 roll;

    roll = rand8_inline();
    target->hp = roll % 10 + 1;
}

void fieldCalc_restoreHalf(MainCalc *calc) {
    calc->hpChange = calc->target->maxHp >> 1;
    calc->mpChange = calc->target->maxMp >> 1;
}

void fieldCalc_cureStatusType(MainCalc *calc) {
    u8 masks[6] = { 0, 0, 0, 2, 1, 24 };

    if (chr_cureStatusMask(calc->target, masks[calc->cmd->statusSet]) != 2) {
        calc->flags |= MAINCALC_MISS;
    }
}

void fieldCalc_cureStatus(MainCalc *calc) {
    if (chr_cureStatusMask(calc->target, calc->cmd->statusMask) != 2) {
        calc->flags |= MAINCALC_MISS;
    }
}

void chr_addHp(Character *chr, s32 amount) {
    if (!chr_hasStatus(chr, CHR_STATUS_PETRIFY)) {
        chr->hp += amount;
        if (chr->hp > chr->maxHp) {
            chr->hp = chr->maxHp;
        }
    }
}

void chr_addMp(Character *chr, s32 amount) {
    if (!chr_hasStatus(chr, CHR_STATUS_PETRIFY)) {
        chr->mp += amount;
        if (chr->mp > chr->maxMp) {
            chr->mp = chr->maxMp;
        }
    }
}

