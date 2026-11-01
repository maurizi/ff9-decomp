#include "ff9.h"

u8 item_useOnMember(Character *caster, Character *target, MainCalcCmd *cmd, u8 kind, s32 all) {
    MainCalc c;

    c.caster = caster;
    c.target = target;
    c.cmd = cmd;
    c.all = all;
    c.flags = 0;
    c.mpChange = 0;
    c.hpChange = 0;
    switch (kind) {
    case BTL_FORMULA_MAGIC_HEAL:
        if (fieldCalc_checkAlive(&c)) {
            if (target->hp == target->maxHp) {
                goto full;
            }
            fieldCalc_setMagicHeal(&c);
            fieldCalc_supportBoost(&c);
            fieldCalc_casterStatusHalf(&c);
            fieldCalc_multiTargetHalf(&c);
            fieldCalc_calcHpGain(&c);
        }
        break;
    case BTL_FORMULA_CURE_STATUS:
        fieldCalc_cureStatusType(&c);
        break;
    case BTL_FORMULA_REVIVE:
        if (fieldCalc_checkDead(&c)) {
            fieldCalc_calcReviveHp(&c);
        }
        break;
    case BTL_FORMULA_ITEM_HEAL:
        if (fieldCalc_checkAlive(&c)) {
            if (target->hp == target->maxHp) {
                goto full;
            }
            fieldCalc_setItemHeal(&c);
            fieldCalc_calcHpGain(&c);
        }
        break;
    case BTL_FORMULA_ITEM_MP_HEAL:
        if (fieldCalc_checkAlive(&c)) {
            if (target->mp == target->maxMp) {
                goto full;
            }
            fieldCalc_setItemHeal(&c);
            fieldCalc_calcMpGain(&c);
        }
        break;
    case BTL_FORMULA_ELIXIR:
        if (fieldCalc_checkAlive(&c)) {
            if (*(u32 *)&target->hp == *(u32 *)&target->maxHp) { /* hp and mp both full */
            full:
                c.flags |= MAINCALC_MISS;
                break;
            }
            fieldCalc_restoreAll(&c);
        }
        break;
    case BTL_FORMULA_PHOENIX_DOWN:
        if (fieldCalc_checkDead(&c)) {
            fieldCalc_reviveRandom(&c);
        }
        break;
    case BTL_FORMULA_SOFT:
    case BTL_FORMULA_ITEM_CURE_STATUS:
        if (((cmd->info >> 8) & 0x1FF) == 0x121) {
            return menuItem_callChocobo();
        }
        fieldCalc_cureStatus(&c);
        break;
    case BTL_FORMULA_TENT:
        if (fieldCalc_checkAlive(&c)) {
            fieldCalc_restoreHalf(&c);
        }
        break;
    }
    return fieldCalc_apply(&c);
}

s32 chr_payMp(Unk8006573C *c, s16 cost) {
    if (c->mp < cost) {
        return 0;
    }
    c->mp -= cost;
    return 1;
}

u8 fieldCalc_apply(MainCalc *c) {
    if (c->flags & MAINCALC_MISS) {
        c->result |= MAINCALC_RESULT_MISS;
        return 0;
    }
    if (c->hpChange > 0) {
        chr_addHp(c->target, c->hpChange);
    }
    if (c->mpChange > 0) {
        chr_addMp(c->target, c->mpChange);
    }
    return 1;
}

u8 fieldCalc_checkAlive(MainCalc *c) {
    if (chr_hasStatus(c->target, CHR_STATUS_PETRIFY | CHR_STATUS_ZOMBIE) || c->target->hp == 0) {
        c->flags |= MAINCALC_MISS;
        return 0;
    }
    return 1;
}

u8 fieldCalc_checkDead(MainCalc *c) {
    if (chr_hasStatus(c->target, CHR_STATUS_PETRIFY) || c->target->hp != 0 ||
        (chr_hasStatus(c->target, CHR_STATUS_ZOMBIE) && c->target->hp == 0)) {
        c->flags |= MAINCALC_MISS;
        return 0;
    }
    return 1;
}

void fieldCalc_setMagicHeal(MainCalc *c) {
    Character *a;
    s32 lv;

    if (g_randPtr >= g_randTableEnd) {
        rand_refill();
    }
    if (g_randPtr >= g_randTableEnd) {
        rand_refill();
    }
    a = c->caster;
    lv = a->magic;
    c->multiplier = lv + ((*g_randPtr << 8) | *g_randPtr) % (((a->level + lv) >> 3) + 1);
    g_randPtr++;
    c->power = c->cmd->power;
    c->defense = c->target->magicDef;
}

void fieldCalc_setItemHeal(MainCalc *c) {
    c->multiplier = 10;
    c->power = c->cmd->power;
    c->defense = 0;
}

void fieldCalc_casterStatusHalf(MainCalc *c) {
    if (chr_hasStatus(c->caster, 0)) {
        c->multiplier /= 2;
    }
}

void fieldCalc_supportBoost(MainCalc *calc) {
    if (calc->caster->supportAbilities[1] & SA_BIT(SA_CONCENTRATE)) {
        calc->multiplier = (calc->multiplier * 3) >> 1;
    }
}

void fieldCalc_multiTargetHalf(MainCalc *c) {
    if (c->all == 1 && (u32)((c->cmd->info & 0xF) - BTL_TGT_MULTI_ANY) < 3) {
        c->multiplier /= 2;
    }
}

void fieldCalc_calcHpGain(MainCalc *calc) {
    u16 v = calc->power * calc->multiplier;
    if ((s16)v >= 10000) {
        v = 9999;
    }
    calc->hpChange = v;
}

void fieldCalc_calcReviveHp(MainCalc *c) {
    Character *t = c->target;
    s32 v = t->maxHp * (t->spirit + c->cmd->power);

    if (c->caster->supportAbilities[1] & SA_BIT(SA_CONCENTRATE)) {
        v /= 50;
    } else {
        v /= 100;
    }
    if (v >= 10000) {
        v = 9999;
    }
    c->hpChange = v;
}

void fieldCalc_calcMpGain(MainCalc *calc) {
    u16 v = calc->power * calc->multiplier;
    if ((s16)v >= 10000) {
        v = 9999;
    }
    calc->mpChange = v;
}

void fieldCalc_restoreAll(MainCalc *c) {
    c->target->hp = c->target->maxHp;
    c->target->mp = c->target->maxMp;
}

void fieldCalc_reviveRandom(MainCalc *c) {
    Character *t = c->target;
    s32 r;

    r = rand8_inline();
    t->hp = r % 10 + 1;
}

void fieldCalc_restoreHalf(MainCalc *c) {
    c->hpChange = c->target->maxHp >> 1;
    c->mpChange = c->target->maxMp >> 1;
}

void fieldCalc_cureStatusType(MainCalc *c) {
    u8 masks[6] = { 0, 0, 0, 2, 1, 24 };

    if (chr_cureStatusMask(c->target, masks[c->cmd->statusSet]) != 2) {
        c->flags |= MAINCALC_MISS;
    }
}

void fieldCalc_cureStatus(MainCalc *c) {
    if (chr_cureStatusMask(c->target, c->cmd->statusMask) != 2) {
        c->flags |= MAINCALC_MISS;
    }
}

void chr_addHp(Character *c, s32 amount) {
    if (!chr_hasStatus(c, CHR_STATUS_PETRIFY)) {
        c->hp += amount;
        if (c->hp > c->maxHp) {
            c->hp = c->maxHp;
        }
    }
}

void chr_addMp(Character *c, s32 amount) {
    if (!chr_hasStatus(c, CHR_STATUS_PETRIFY)) {
        c->mp += amount;
        if (c->mp > c->maxMp) {
            c->mp = c->maxMp;
        }
    }
}

