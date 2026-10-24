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
        if (func_800657F0(&c)) {
            if (t->hp == t->maxHp) {
                goto full;
            }
            func_800658DC(&c);
            func_80065A44((Unk80065A44 *)&c);
            func_800659EC(&c);
            func_80065A80(&c);
            func_80065ADC((Unk80065ADC *)&c);
        }
        break;
    case 12:
        func_80065CBC(&c);
        break;
    case 13:
        if (func_80065850(&c)) {
            func_80065B14(&c);
        }
        break;
    case 69:
        if (func_800657F0(&c)) {
            if (t->hp == t->maxHp) {
                goto full;
            }
            func_800659D0(&c);
            func_80065ADC((Unk80065ADC *)&c);
        }
        break;
    case 70:
        if (func_800657F0(&c)) {
            if (t->mp == t->maxMp) {
                goto full;
            }
            func_800659D0(&c);
            func_80065B9C((Unk80065ADC *)&c);
        }
        break;
    case 71:
        if (func_800657F0(&c)) {
            if (*(u32 *)&t->hp == *(u32 *)&t->maxHp) { /* hp and mp both full */
            full:
                c.unk10 |= 1;
                break;
            }
            func_80065BD4(&c);
        }
        break;
    case 72:
        if (func_80065850(&c)) {
            func_80065BFC(&c);
        }
        break;
    case 62:
    case 73:
        if (((cmd->unk0 >> 8) & 0x1FF) == 0x121) {
            return func_801F1A7C();
        }
        func_80065D40(&c);
        break;
    case 76:
        if (func_800657F0(&c)) {
            func_80065C8C(&c);
        }
        break;
    }
    return func_8006576C(&c);
}

s32 func_8006573C(Unk8006573C *a, s16 b) {
    if (a->unk12 < b) {
        return 0;
    }
    a->unk12 -= b;
    return 1;
}

u8 func_8006576C(MainCalc *c) {
    if (c->unk10 & 1) {
        c->unk11 |= 0x20;
        return 0;
    }
    if (c->unk18 > 0) {
        func_80065D90(c->unk4, c->unk18);
    }
    if (c->unk1A > 0) {
        func_80065DF4(c->unk4, c->unk1A);
    }
    return 1;
}

u8 func_800657F0(MainCalc *c) {
    if (func_800539FC(c->unk4, 0x41) || c->unk4->hp == 0) {
        c->unk10 |= 1;
        return 0;
    }
    return 1;
}

u8 func_80065850(MainCalc *c) {
    if (func_800539FC(c->unk4, 1) || c->unk4->hp != 0 ||
        (func_800539FC(c->unk4, 0x40) && c->unk4->hp == 0)) {
        c->unk10 |= 1;
        return 0;
    }
    return 1;
}

void func_800658DC(MainCalc *c) {
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

void func_800659D0(MainCalc *c) {
    c->unk16 = 10;
    c->unk12 = c->unk8->unk5;
    c->unk14 = 0;
}

void func_800659EC(MainCalc *c) {
    if (func_800539FC(c->unk0, 0)) {
        c->unk16 /= 2;
    }
}

void func_80065A44(Unk80065A44 *a) {
    if (a->unk0->unk8C & 2) {
        a->unk16 = (a->unk16 * 3) >> 1;
    }
}

void func_80065A80(MainCalc *c) {
    if (c->unkC == 1 && (u32)((c->unk8->unk0 & 0xF) - 3) < 3) {
        c->unk16 /= 2;
    }
}

void func_80065ADC(Unk80065ADC *a) {
    u16 v = a->unk12 * a->unk16;
    if ((s16)v >= 10000) {
        v = 9999;
    }
    a->unk18 = v;
}

void func_80065B14(MainCalc *c) {
    Character *t = c->unk4;
    s32 v = t->maxHp * (t->spirit + c->unk8->unk5);

    if (c->unk0->unk88[1] & 2) {
        v /= 50;
    } else {
        v /= 100;
    }
    if (v >= 10000) {
        v = 9999;
    }
    c->unk18 = v;
}

void func_80065B9C(Unk80065ADC *a) {
    u16 v = a->unk12 * a->unk16;
    if ((s16)v >= 10000) {
        v = 9999;
    }
    a->unk1A = v;
}

void func_80065BD4(MainCalc *c) {
    c->unk4->hp = c->unk4->maxHp;
    c->unk4->mp = c->unk4->maxMp;
}

void func_80065BFC(MainCalc *c) {
    Character *t = c->unk4;
    s32 r;

    if (g_randPtr >= g_randTableEnd) {
        rand_refill();
    }
    r = *g_randPtr++;
    t->hp = r % 10 + 1;
}

void func_80065C8C(MainCalc *c) {
    c->unk18 = c->unk4->maxHp >> 1;
    c->unk1A = c->unk4->maxMp >> 1;
}

void func_80065CBC(MainCalc *c) {
    u8 masks[6] = { 0, 0, 0, 2, 1, 24 };

    if (func_8005398C(c->unk4, masks[c->unk8->unk9]) != 2) {
        c->unk10 |= 1;
    }
}

void func_80065D40(MainCalc *c) {
    if (func_8005398C(c->unk4, c->unk8->unk8) != 2) {
        c->unk10 |= 1;
    }
}

void func_80065D90(Character *u, s32 n) {
    if (!func_800539FC(u, 1)) {
        u->hp += n;
        if (u->hp > u->maxHp) {
            u->hp = u->maxHp;
        }
    }
}

void func_80065DF4(Character *u, s32 n) {
    if (!func_800539FC(u, 1)) {
        u->mp += n;
        if (u->mp > u->maxMp) {
            u->mp = u->maxMp;
        }
    }
}

