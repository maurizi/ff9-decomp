#include "ff9.h"

u8 item_useOnMember(Character *a, Character *t, MainCalcCmd *cmd, u8 kind, s32 e) {
    MainCalc c;

    c.unk0 = a;
    c.unk4 = t;
    c.unk8 = cmd;
    c.unkC = e;
    c.unk10 = 0;
    c.unk1A = 0;
    c.unk18 = 0;
    switch (kind) {
    case 10:
        if (fieldCalc_checkAlive(&c)) {
            if (t->hp == t->maxHp) {
                goto full;
            }
            fieldCalc_setMagicHeal(&c);
            fieldCalc_supportBoost((Unk80065A44 *)&c);
            fieldCalc_casterStatusHalf(&c);
            fieldCalc_multiTargetHalf(&c);
            fieldCalc_calcHpGain((Unk80065ADC *)&c);
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
            fieldCalc_calcHpGain((Unk80065ADC *)&c);
        }
        break;
    case 70:
        if (fieldCalc_checkAlive(&c)) {
            if (t->mp == t->maxMp) {
                goto full;
            }
            fieldCalc_setItemHeal(&c);
            fieldCalc_calcMpGain((Unk80065ADC *)&c);
        }
        break;
    case 71:
        if (fieldCalc_checkAlive(&c)) {
            if (*(u32 *)&t->hp == *(u32 *)&t->maxHp) { /* hp and mp both full */
            full:
                c.unk10 |= 1;
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
    if (c->unk10 & 1) {
        c->unk11 |= 0x20;
        return 0;
    }
    if (c->unk18 > 0) {
        chr_addHp(c->unk4, c->unk18);
    }
    if (c->unk1A > 0) {
        chr_addMp(c->unk4, c->unk1A);
    }
    return 1;
}

u8 fieldCalc_checkAlive(MainCalc *c) {
    if (chr_hasStatus(c->unk4, 0x41) || c->unk4->hp == 0) {
        c->unk10 |= 1;
        return 0;
    }
    return 1;
}

u8 fieldCalc_checkDead(MainCalc *c) {
    if (chr_hasStatus(c->unk4, 1) || c->unk4->hp != 0 ||
        (chr_hasStatus(c->unk4, 0x40) && c->unk4->hp == 0)) {
        c->unk10 |= 1;
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
    a = c->unk0;
    lv = a->magic;
    c->unk16 = lv + ((*g_randPtr << 8) | *g_randPtr) % (((a->level + lv) >> 3) + 1);
    g_randPtr++;
    c->unk12 = c->unk8->unk5;
    c->unk14 = c->unk4->magicDef;
}

void fieldCalc_setItemHeal(MainCalc *c) {
    c->unk16 = 10;
    c->unk12 = c->unk8->unk5;
    c->unk14 = 0;
}

void fieldCalc_casterStatusHalf(MainCalc *c) {
    if (chr_hasStatus(c->unk0, 0)) {
        c->unk16 /= 2;
    }
}

void fieldCalc_supportBoost(Unk80065A44 *a) {
    if (a->unk0->unk8C & 2) {
        a->unk16 = (a->unk16 * 3) >> 1;
    }
}

void fieldCalc_multiTargetHalf(MainCalc *c) {
    if (c->unkC == 1 && (u32)((c->unk8->unk0 & 0xF) - 3) < 3) {
        c->unk16 /= 2;
    }
}

void fieldCalc_calcHpGain(Unk80065ADC *a) {
    u16 v = a->unk12 * a->unk16;
    if ((s16)v >= 10000) {
        v = 9999;
    }
    a->unk18 = v;
}

void fieldCalc_calcReviveHp(MainCalc *c) {
    Character *t = c->unk4;
    s32 v = t->maxHp * (t->spirit + c->unk8->unk5);

    if (c->unk0->supportAbilities[1] & 2) {
        v /= 50;
    } else {
        v /= 100;
    }
    if (v >= 10000) {
        v = 9999;
    }
    c->unk18 = v;
}

void fieldCalc_calcMpGain(Unk80065ADC *a) {
    u16 v = a->unk12 * a->unk16;
    if ((s16)v >= 10000) {
        v = 9999;
    }
    a->unk1A = v;
}

void fieldCalc_restoreAll(MainCalc *c) {
    c->unk4->hp = c->unk4->maxHp;
    c->unk4->mp = c->unk4->maxMp;
}

void fieldCalc_reviveRandom(MainCalc *c) {
    Character *t = c->unk4;
    s32 r;

    if (g_randPtr >= g_randTableEnd) {
        rand_refill();
    }
    r = *g_randPtr++;
    t->hp = r % 10 + 1;
}

void fieldCalc_restoreHalf(MainCalc *c) {
    c->unk18 = c->unk4->maxHp >> 1;
    c->unk1A = c->unk4->maxMp >> 1;
}

void fieldCalc_cureStatusType(MainCalc *c) {
    u8 masks[6] = { 0, 0, 0, 2, 1, 24 };

    if (chr_cureStatusMask(c->unk4, masks[c->unk8->unk9]) != 2) {
        c->unk10 |= 1;
    }
}

void fieldCalc_cureStatus(MainCalc *c) {
    if (chr_cureStatusMask(c->unk4, c->unk8->unk8) != 2) {
        c->unk10 |= 1;
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

