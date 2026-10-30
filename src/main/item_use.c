#include "ff9.h"

u8 item_useOnMember(Character *a, Character *t, MainCalcCmd *cmd, u8 kind, s32 e) {
    MainCalc c;

    c.caster = a;
    c.target = t;
    c.cmd = cmd;
    c.all = e;
    c.flags = 0;
    c.mpChange = 0;
    c.hpChange = 0;
    switch (kind) {
    case 10:
        if (fieldCalc_checkAlive(&c)) {
            if (t->hp == t->maxHp) {
                goto full;
            }
            fieldCalc_setMagicHeal(&c);
            fieldCalc_supportBoost(&c);
            fieldCalc_casterStatusHalf(&c);
            fieldCalc_multiTargetHalf(&c);
            fieldCalc_calcHpGain(&c);
        }
        break;
    case 12:
        fieldCalc_cureStatusType(&c);
        break;
    case 13:
        if (fieldCalc_checkDead(&c)) {
            fieldCalc_calcReviveHp(&c);
        }
        break;
    case 69:
        if (fieldCalc_checkAlive(&c)) {
            if (t->hp == t->maxHp) {
                goto full;
            }
            fieldCalc_setItemHeal(&c);
            fieldCalc_calcHpGain(&c);
        }
        break;
    case 70:
        if (fieldCalc_checkAlive(&c)) {
            if (t->mp == t->maxMp) {
                goto full;
            }
            fieldCalc_setItemHeal(&c);
            fieldCalc_calcMpGain(&c);
        }
        break;
    case 71:
        if (fieldCalc_checkAlive(&c)) {
            if (*(u32 *)&t->hp == *(u32 *)&t->maxHp) { /* hp and mp both full */
            full:
                c.flags |= 1;
                break;
            }
            fieldCalc_restoreAll(&c);
        }
        break;
    case 72:
        if (fieldCalc_checkDead(&c)) {
            fieldCalc_reviveRandom(&c);
        }
        break;
    case 62:
    case 73:
        if (((cmd->unk0 >> 8) & 0x1FF) == 0x121) {
            return menuItem_callChocobo();
        }
        fieldCalc_cureStatus(&c);
        break;
    case 76:
        if (fieldCalc_checkAlive(&c)) {
            fieldCalc_restoreHalf(&c);
        }
        break;
    }
    return fieldCalc_apply(&c);
}

s32 chr_payMp(Unk8006573C *a, s16 b) {
    if (a->unk12 < b) {
        return 0;
    }
    a->unk12 -= b;
    return 1;
}

u8 fieldCalc_apply(MainCalc *c) {
    if (c->flags & 1) {
        c->result |= 0x20;
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
    if (chr_hasStatus(c->target, 0x41) || c->target->hp == 0) {
        c->flags |= 1;
        return 0;
    }
    return 1;
}

u8 fieldCalc_checkDead(MainCalc *c) {
    if (chr_hasStatus(c->target, 1) || c->target->hp != 0 ||
        (chr_hasStatus(c->target, 0x40) && c->target->hp == 0)) {
        c->flags |= 1;
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

void fieldCalc_supportBoost(MainCalc *a) {
    if (a->caster->supportAbilities[1] & 2) {
        a->multiplier = (a->multiplier * 3) >> 1;
    }
}

void fieldCalc_multiTargetHalf(MainCalc *c) {
    if (c->all == 1 && (u32)((c->cmd->unk0 & 0xF) - 3) < 3) {
        c->multiplier /= 2;
    }
}

void fieldCalc_calcHpGain(MainCalc *a) {
    u16 v = a->power * a->multiplier;
    if ((s16)v >= 10000) {
        v = 9999;
    }
    a->hpChange = v;
}

void fieldCalc_calcReviveHp(MainCalc *c) {
    Character *t = c->target;
    s32 v = t->maxHp * (t->spirit + c->cmd->power);

    if (c->caster->supportAbilities[1] & 2) {
        v /= 50;
    } else {
        v /= 100;
    }
    if (v >= 10000) {
        v = 9999;
    }
    c->hpChange = v;
}

void fieldCalc_calcMpGain(MainCalc *a) {
    u16 v = a->power * a->multiplier;
    if ((s16)v >= 10000) {
        v = 9999;
    }
    a->mpChange = v;
}

void fieldCalc_restoreAll(MainCalc *c) {
    c->target->hp = c->target->maxHp;
    c->target->mp = c->target->maxMp;
}

void fieldCalc_reviveRandom(MainCalc *c) {
    Character *t = c->target;
    s32 r;

    if (g_randPtr >= g_randTableEnd) {
        rand_refill();
    }
    r = *g_randPtr++;
    t->hp = r % 10 + 1;
}

void fieldCalc_restoreHalf(MainCalc *c) {
    c->hpChange = c->target->maxHp >> 1;
    c->mpChange = c->target->maxMp >> 1;
}

void fieldCalc_cureStatusType(MainCalc *c) {
    u8 masks[6] = { 0, 0, 0, 2, 1, 24 };

    if (chr_cureStatusMask(c->target, masks[c->cmd->statusSet]) != 2) {
        c->flags |= 1;
    }
}

void fieldCalc_cureStatus(MainCalc *c) {
    if (chr_cureStatusMask(c->target, c->cmd->statusMask) != 2) {
        c->flags |= 1;
    }
}

void chr_addHp(Character *u, s32 n) {
    if (!chr_hasStatus(u, 1)) {
        u->hp += n;
        if (u->hp > u->maxHp) {
            u->hp = u->maxHp;
        }
    }
}

void chr_addMp(Character *u, s32 n) {
    if (!chr_hasStatus(u, 1)) {
        u->mp += n;
        if (u->mp > u->maxMp) {
            u->mp = u->maxMp;
        }
    }
}

