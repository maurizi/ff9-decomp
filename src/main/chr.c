#include "ff9.h"

void chr_init(s32 ch) {
    Character *c = &g_gameState->gameData->characters[ch];
    u32 *init;
    u32 *tbl;
    s32 r;

    memset(c, 0, 0x90);
    tbl = D_8006CA78;
    init = tbl + ch;
    c->info.w = *init;
    c->status = 0;
    c->classFlags = chr_getClassFlags(c->info.b.row);
    memset(c->equipBonus, 0, 10);
    chr_clearAbilities(ch);
    r = chr_getDefaultSet(c);
    chr_setDefaultName(c->name, r);
    chr_setDefaultEquip(c->equip, r);
    c->info.b.kind = chr_calcAppearance(c->info.b.id, (c->classFlags >> 4) & 1, c->equip);
    chr_setLevel(ch, 1, init, 0);
    c->hp = c->maxHp;
    c->mp = c->maxMp;
    c->stones = c->maxStones;
}

void chr_setLevel(s32 ch, s32 lv, u32 *heal, u8 f) {
    Character *c = &g_gameState->gameData->characters[ch];
    s32 hp;
    s32 mp;
    s32 d;

    if (!f && c->level != lv) {
        c->exp = g_levelExp[lv - 1];
    }
    hp = c->hp;
    mp = c->mp;
    d = c->maxStones - c->stones;
    c->level = lv;
    memset(&c->hp, 0, 8);
    memset(&c->maxHp, 0, 8);
    c->unk16 = 10;
    c->baseSpeed = chr_calcBaseSpeed(ch, c->level, f);
    c->baseStrength = chr_calcBaseStrength(ch, c->level, f);
    c->baseMagic = chr_calcBaseMagic(ch, c->level, f);
    c->baseSpirit = chr_calcBaseSpirit(ch, c->level, f);
    c->maxHp = c->baseMaxHp = chr_calcMaxHp(c->level, c->baseStrength);
    c->maxMp = c->baseMaxMp = chr_calcMaxMp(c->level, c->baseMagic);
    c->maxStones = chr_calcMaxStones(ch, c->level, f);
    c->hp = (hp < c->maxHp) ? hp : c->maxHp;
    c->mp = (mp < c->maxMp) ? mp : c->maxMp;
    c->stones = c->maxStones - d;
    if (heal) {
        c->hp = c->maxHp;
        c->mp = c->maxMp;
        c->stones = c->maxStones;
    }
    chr_updateStats(c);
    if (f) {
        c->hp = (hp < c->maxHp) ? hp : c->maxHp;
        c->mp = (mp < c->maxMp) ? mp : c->maxMp;
    }
    if (c->maxStones < d) {
        chr_fitSupportAbilities(c);
    }
}

EquipStatsOut *chr_calcEquipStats(EquipStatsIn *in, EquipStatsOut *out) {
    s32 i;
    u8 *a;
    u8 *e;
    s32 id;

    memset(out, 0, sizeof(EquipStatsOut));
    out->hp = in->hp;
    out->mp = in->mp;
    out->maxHp = *(u16 *)&in->base.unk0[0];
    out->maxMp = *(u16 *)&in->base.unk0[2];
    out->stats[0] = in->base.unk0[4];
    out->stats[1] = in->base.unk0[5];
    out->stats[2] = in->base.unk0[6];
    out->stats[3] = in->base.unk0[7];
    id = in->equip.unk0[0];
    if (id != 0xFF) {
        out->defs[0] = g_weaponTable[id].unk5;
    }
    for (i = 0; i < 4; i++) {
        id = (&in->equip.unk0[1])[i];
        if (id != 0xFF) {
            if ((u32)(id - 0x58) < 0x88) {
                a = D_8006D1C8[id];
                out->defs[1] += a[0];
                out->defs[2] += a[1];
                out->defs[3] += a[2];
                out->defs[4] += a[3];
            }
        }
    }
    for (i = 0; i < 5; i++) {
        id = in->equip.unk0[i];
        if (id != 0xFF) {
            e = g_equipStatBonus[g_itemTable[id].statBonus].unk0;
            out->stats[0] += e[0];
            out->stats[1] += e[1];
            out->stats[2] += e[2];
            out->stats[3] += e[3];
        }
    }
    for (i = 0; i < 4; i++) {
        if (chr_testSupportBit(in->support.unk0, D_8006CA9C[i])) {
            switch (D_8006CA9C[i]) {
            case 0xC5:
                out->maxHp += out->maxHp / 10;
                break;
            case 0xC6:
                out->maxHp += out->maxHp / 5;
                break;
            case 0xC7:
                out->maxMp += out->maxMp / 10;
                break;
            case 0xC8:
                out->maxMp += out->maxMp / 5;
                break;
            }
        }
    }
    for (i = 0; i < 4; i++) {
        if (out->stats[i] > D_8006CAA0[i]) {
            out->stats[i] = D_8006CAA0[i];
        }
    }
    for (i = 0; i < 5; i++) {
        if (out->defs[i] >= 1000) {
            out->defs[i] = 999;
        }
    }
    if (out->maxHp >= 10000) {
        out->maxHp = 9999;
    }
    if (out->maxMp >= 1000) {
        out->maxMp = 999;
    }
    if (out->maxHp < out->hp) {
        out->hp = out->maxHp;
    }
    if (out->maxMp < out->mp) {
        out->mp = out->maxMp;
    }
    return out;
}

s32 chr_calcAppearance(s32 kind, s32 flag, u8 *item) {
    u8 t = D_8006CAA4[kind][(u8)flag != 0];
    u8 k = g_itemTable[*item].icon;
    s32 r;

    switch (t) {
    case 0:
        return k != 1;
    case 1:
        return 2;
    case 2:
        if ((u16)g_evtGlobalVars.h[0] >= 10300) {
            r = 5;
            if (k == 7) {
                r = 6;
            }
            return r;
        }
        r = 3;
        if (k == 7) {
            r = 4;
        }
        return r;
    case 3:
        return 7;
    case 4:
        return 12;
    case 5:
        return 9;
    case 6:
        r = 10;
        if (k == 7) {
            r = 11;
        }
        return r;
    case 7:
        return 13;
    case 8:
        return 14;
    case 9:
        return 15;
    case 10:
        r = 16;
        if ((u16)(g_evtGlobalVars.h[0] - 1500) < 100) {
            r = 17;
        }
        return r;
    case 11:
        return 18;
    }
    return -1;
}

void chr_join(s32 ch, u8 avg, s32 eq) {
    Character *c = &g_gameState->gameData->characters[ch];
    u8 *p;
    s32 i;
    s32 n;
    s32 sum;
    s32 lv;

    if (eq != 0xFF) {
        p = c->equip;
        for (i = 0; i < 5; i++) {
            p[i] = D_8006CAD8[eq][i];
        }
    }
    c->info.b.kind = chr_calcAppearance(c->info.b.id, (c->classFlags >> 4) & 1, c->equip);
    if (avg == 0) {
        if (eq == 0xFF) {
            return;
        }
        goto calc;
    }
    n = 0;
    sum = 0;
    for (i = 0; i < 9; i++) {
        if (ch != i) {
            if (g_gameState->gameData->characters[i].info.b.recruited) {
                n++;
                sum += g_gameState->gameData->characters[i].level;
            }
        }
    }
    if (n != 0) {
        lv = sum / n;
    } else {
        lv = 1;
    }
    if ((u32)(c->info.b.kind - 10) >= 2) {
        if (lv < c->level) {
            lv = c->level;
        }
    }
    chr_setLevel(ch, lv, NULL, 0);
    return;
calc:
    chr_updateStats_inline(c);
}

void party_initNewGame(void) {
    Character **party = g_gameState->gameData->party;
    s32 i;
    party_setChanged(0);
    memset(party, 0, 0x1C);
    for (i = 0; i < 9; i++) {
        chr_init(i);
    }
    chr_setRecruited(0);
    chr_setRecruited(1);
    chr_setRecruited(2);
    chr_setRecruited(3);
    chr_setRecruited(4);
    chr_setRecruited(5);
    chr_setRecruited(7);
    chr_setRecruited(6);
    party_setMember(0, 0);
    party_setMember(1, 1);
    party_setMember(2, 2);
    party_setMember(3, 3);
    g_gameState->gameData->gil = 500;
    g_gameState->gameData->firstUseFxSeen = 0;
}

s32 party_getFirstMember(void) {
    GameData *gd = g_gameState->gameData;
    s32 i = 0;
    do {
    loop:
        if (gd->party[i] == NULL) {
            i = i < 3 ? i + 1 : 0;
            goto loop;
        }
    } while (0);
    return i;
}

s32 party_getPrevMember(s32 i) {
    do {
        i = i == 0 ? 3 : i - 1;
    } while (g_gameState->gameData->party[i] == NULL);
    return i;
}

s32 party_getNextMember(s32 i) {
    do {
        i = i < 3 ? i + 1 : 0;
    } while (g_gameState->gameData->party[i] == NULL);
    return i;
}

void chr_updateStats(Character *c) {
    EquipStatsIn in;
    EquipStatsOut out;

    in.base = *(Bytes8 *)&c->baseMaxHp;
    in.hp = c->hp;
    in.mp = c->mp;
    in.support = *(Words2 *)c->supportAbilities;
    in.equip = *(Bytes5 *)c->equip;
    chr_calcEquipStats(&in, &out);
    c->speed = out.stats[0];
    c->strength = out.stats[1];
    c->magic = out.stats[2];
    c->spirit = out.stats[3];
    c->defense = out.defs[1];
    c->evade = out.defs[2];
    c->magicDef = out.defs[3];
    c->magicEva = out.defs[4];
    c->hp = out.hp;
    c->mp = out.mp;
    c->maxHp = out.maxHp;
    c->maxMp = out.maxMp;
}

s32 chr_getClass(u32 x) {
    switch (x) {
    case 0:
    case 16:
        return 0;
    case 1:
        return 1;
    case 2:
        return 2;
    case 3:
        return 3;
    case 4:
        return 4;
    case 5:
        return 5;
    case 6:
        return 6;
    case 7:
        return 7;
    case 8:
    case 9:
    case 17:
        return 8;
    case 10:
    case 11:
    case 18:
        return 9;
    case 12:
    case 13:
    case 19:
        return 10;
    case 14:
    case 15:
        return 11;
    }
    return -1;
}

u8 chr_getAppearanceType(s32 a, u8 b) {
    return D_8006CAA4[a][b != 0];
}

s32 chr_getDefaultSet(Character *c) {
    return D_8006CAB8[c->info.b.id][(c->classFlags & 0x10) != 0];
}

u8 chr_getClassFlags(u32 x) {
    u8 *tbl = D_8006CACC;
    s32 i;
    switch (x) {
    case 0:
    case 16:
        i = 0;
        break;
    case 1:
        i = 1;
        break;
    case 2:
        i = 2;
        break;
    case 3:
        i = 3;
        break;
    case 4:
        i = 4;
        break;
    case 5:
        i = 5;
        break;
    case 6:
        i = 6;
        break;
    case 7:
        i = 7;
        break;
    case 8:
    case 9:
    case 17:
        i = 8;
        break;
    case 10:
    case 11:
    case 18:
        i = 9;
        break;
    case 12:
    case 13:
    case 19:
        i = 10;
        break;
    case 14:
    case 15:
        i = 11;
        break;
    default:
        i = -1;
        break;
    }
    return tbl[i];
}

void party_setMember(s32 slot, s32 ch) {
    Character *old = g_gameState->gameData->party[slot];
    if (ch >= 9) {
        ch = -1;
    }
    if (ch < 0) {
        g_gameState->gameData->party[slot] = NULL;
    } else {
        chr_setRecruited(ch);
        g_gameState->gameData->party[slot] = &g_gameState->gameData->characters[ch];
    }
    if (old != g_gameState->gameData->party[slot]) {
        party_setChanged(1);
    }
}

void chr_setRecruited(u32 i) {
    if (i < 9) {
        g_gameState->gameData->characters[i].info.b.recruited = 1;
    }
}

void chr_clearRecruited(u32 i) {
    if (i < 9) {
        g_gameState->gameData->characters[i].info.b.recruited = 0;
    }
}

s32 chr_resetToLevel(s32 ch, s32 b, u8 flag) {
    Character *c = &g_gameState->gameData->characters[ch];
    s32 lv = c->level;
    c->level = 0;
    chr_setLevel(ch, b, 0, 0);
    if (flag) {
        c->hp = c->maxHp;
        c->mp = c->maxMp;
    }
    return lv;
}

s32 chr_levelUp(s32 ch, s32 b) {
    Character *c = &g_gameState->gameData->characters[ch];
    s32 r = c->level;
    chr_setLevel(ch, b, 0, 1);
    return r;
}

void party_setChanged(u8 a) {
    g_partyChanged = a;
}

u8 party_hasChanged(void) {
    return g_partyChanged;
}

/* default character names: count, then {offset, length} entries */
INCLUDE_RODATA("asm/us/main/nonmatchings/chr", D_80010DC4);

void chr_setDefaultName(u8 *dst, s32 id) {
    memset(dst, 0xFF, 10);
    text_copy(dst, (u8 *)text_getFrom((u8 *)D_80010DC4, id));
}

void chr_setDefaultEquip(u8 *dst, s32 b) {
    s32 i;
    for (i = 0; i < 5; i++) {
        dst[i] = D_8006CAD8[b][i];
    }
}

void chr_fitSupportAbilities(Character *c) {
    s32 i;
    Unk8006D128 *e;
    s32 id;

    i = 0;
    e = g_supportAbilities;
    id = c->info.b.id;
    c->stones = c->maxStones;
    do {
        if (chr_hasSupportAbility(id, i + 0xC0)) {
            if (c->stones >= e->unk1) {
                c->stones -= e->unk1;
            } else {
                chr_setSupportAbility(id, i + 0xC0, 0);
            }
        }
        i++;
        e++;
    } while (i < 64);
}

s32 chr_addAp(s32 ch, s32 id, s32 add) {
    s32 i = chr_findAbilitySlot_inline(ch, id);
    s32 old;
    s32 need;

    if (i < 0) {
        return 0;
    }
    old = g_gameState->gameData->characters[ch].ap[i];
    need = chr_getApNeeded_inline(ch, id);
    if (need - g_gameState->gameData->characters[ch].ap[i] < add) {
        g_gameState->gameData->characters[ch].ap[i] = need;
    } else {
        g_gameState->gameData->characters[ch].ap[i] += add;
    }
    return g_gameState->gameData->characters[ch].ap[i] - old;
}

s32 chr_isAbilityLearned(s32 ch, s32 id) {
    s32 ap = chr_findAbilitySlot_ap_inline(ch, id);
    s32 need = chr_getApNeeded_inline(ch, id);

    if (ap < 0 || need < 0) {
        return 0;
    }
    return ap >= need;
}

s32 chr_learnAbility(s32 ch, s32 id) {
    s32 i = chr_findAbilitySlot_inline(ch, id);
    Character *c = g_gameState->gameData->characters + ch;
    if (i < 0) {
        return 0;
    }
    c->ap[i] = g_abilityRows[c->info.b.row][i].unk1;
    return 1;
}

void func_8003431C(void) {
}

void chr_clearAllAp(void) {
    s32 i;
    for (i = 0; i < 9; i++) {
        chr_clearAp(i);
    }
}

void chr_clearAllSupport(void) {
    s32 i;
    for (i = 0; i < 9; i++) {
        chr_clearSupport(i);
    }
}

void chr_clearAbilities(s32 a) {
    chr_clearAp(a);
    chr_clearSupport(a);
}

void chr_clearAp(s32 ch) {
    s32 i;
    Character *c;

    if (ch < 8) {
        c = &g_gameState->gameData->characters[ch];
        for (i = 0; i < 0x30; i++) {
            c->ap[i] = 0;
        }
    }
}

void chr_clearSupport(s32 i) {
    Character *c = &g_gameState->gameData->characters[i];
    c->supportAbilities[1] = 0;
    c->supportAbilities[0] = 0;
}

s32 chr_findAbilitySlot(s32 ch, s32 id) {
    return chr_findAbilitySlot_inline(ch, id);
}

u8 *chr_getApPtr(s32 ch, s32 id) {
    s32 i = chr_findAbilitySlot_inline(ch, id);
    if (i < 0) {
        return NULL;
    }
    return &g_gameState->gameData->characters[ch].ap[i];
}

s32 chr_getAp(s32 ch, s32 id) {
    s32 i = chr_findAbilitySlot_inline(ch, id);
    if (i < 0) {
        return -1;
    }
    return g_gameState->gameData->characters[ch].ap[i];
}

s32 chr_setAp(s32 ch, s32 id, u8 v) {
    s32 i = chr_findAbilitySlot_inline(ch, id);
    s32 old;
    if (i < 0) {
        return -1;
    }
    old = g_gameState->gameData->characters[ch].ap[i];
    g_gameState->gameData->characters[ch].ap[i] = v;
    return old;
}

static inline s32 chr_setAp_inline(s32 ch, s32 id, s32 v) {
    s32 i = chr_findAbilitySlot_inline(ch, id);
    s32 old;
    if (i < 0) {
        return -1;
    }
    old = g_gameState->gameData->characters[ch].ap[i];
    g_gameState->gameData->characters[ch].ap[i] = v;
    return old;
}

s32 chr_resetAp(s32 ch, s32 id) {
    s32 r = chr_setAp_inline(ch, id, 0);
    return r;
}

u8 chr_getApNeeded(s32 ch, s32 id) {
    s32 i = chr_findAbilitySlot_inline(ch, id);
    Character *c = g_gameState->gameData->characters + ch;
    if (i < 0) {
        return 0;
    }
    return g_abilityRows[c->info.b.row][i].unk1;
}

s32 chr_countSupportStones(s32 ch) {
    s32 sum = 0;
    s32 i;
    u32 *bits = g_gameState->gameData->characters[ch].supportAbilities;
    for (i = 0; i < 0x40; i++) {
        if (bits[i >> 5] & (1 << (i & 0x1F))) {
            sum += g_supportAbilities[i].unk1;
        }
    }
    return sum;
}

void chr_setSupportAbility(s32 ch, s32 id, u8 on) {
    u32 *bits = g_gameState->gameData->characters[ch].supportAbilities;
    id -= 0xC0;
    if (on) {
        bits[id >> 5] |= 1 << (id & 0x1F);
    } else {
        bits[id >> 5] &= ~(1 << (id & 0x1F));
    }
}

u8 chr_hasSupportAbility(s32 ch, s32 id) {
    return chr_testSupportBit_inline(g_gameState->gameData->characters[ch].supportAbilities, id);
}

u8 chr_testSupportBit(u32 *bits, s32 b) {
    b -= 0xC0;
    return (bits[b >> 5] & (1 << (b & 0x1F))) != 0;
}

u8 func_80034A30(Character *a) {
    if (a->classFlags & 0x10) {
        return 0;
    }
    return (s32)a->info.b.row < 8;
}

s32 chr_calcBaseSpeed(s32 ch, s32 lv, u8 add) {
    Character *c = g_gameState->gameData->characters + ch;
    ClassStats *e = &g_charClassStats[chr_getClass(c->info.b.row)];
    s32 r;
    if (add) {
        c->equipBonus[0] += chr_sumEquipBonus(c->equip, 0);
    }
    r = e->speed + lv / 10 + (c->equipBonus[0] >> 5);
    if (r >= 51) {
        r = 50;
    }
    return r;
}

s32 chr_calcBaseStrength(s32 ch, s32 lv, u8 add) {
    Character *c = g_gameState->gameData->characters + ch;
    s32 k = chr_getClass(c->info.b.row);
    u16 *bonus = c->equipBonus;
    ClassStats *e = &g_charClassStats[k];
    s32 r;
    s32 n;
    if (add) {
        n = 0;
        if (c->stones == 0) {
            n = 3;
        }
        bonus[1] += n + chr_sumEquipBonus(c->equip, 1);
    }
    r = e->strength + lv * 3 / 10 + (bonus[1] >> 5);
    if (r >= 100) {
        r = 99;
    }
    return r;
}

s32 chr_calcBaseMagic(s32 ch, s32 lv, u8 add) {
    Character *c = g_gameState->gameData->characters + ch;
    s32 k = chr_getClass(c->info.b.row);
    u16 *bonus = c->equipBonus;
    ClassStats *e = &g_charClassStats[k];
    s32 r;
    s32 n;
    if (add) {
        n = 0;
        if (c->stones == 0) {
            n = 3;
        }
        bonus[2] += n + chr_sumEquipBonus(c->equip, 2);
    }
    r = e->magic + lv * 3 / 10 + (bonus[2] >> 5);
    if (r >= 100) {
        r = 99;
    }
    return r;
}

s32 chr_calcBaseSpirit(s32 ch, s32 lv, u8 add) {
    Character *c = g_gameState->gameData->characters + ch;
    s32 k = chr_getClass(c->info.b.row);
    u16 *bonus = c->equipBonus;
    ClassStats *e = &g_charClassStats[k];
    s32 r;
    s32 n;
    if (add) {
        n = c->stones == 0;
        bonus[3] += n + chr_sumEquipBonus(c->equip, 3);
    }
    r = e->spirit + lv * 3 / 20 + (bonus[3] >> 5);
    if (r >= 51) {
        r = 50;
    }
    return r;
}

s32 chr_calcMaxStones(s32 ch, s32 lv, u8 add) {
    Character *c = g_gameState->gameData->characters + ch;
    u16 *bonus = c->equipBonus;
    s32 k = chr_getClass(c->info.b.row);
    ClassStats *e = &g_charClassStats[k];
    s32 r;
    s32 n;

    if (add) {
        n = 0;
        if (c->stones == 0) {
            n = 5;
        }
        bonus[4] += n;
    }
    r = e->stones + lv * 4 / 10 + (bonus[4] >> 5);
    if (r >= 100) {
        r = 99;
    }
    return r;
}

s32 chr_calcMaxHp(s32 lv, s32 m) {
    s32 r = g_levelHpMp[lv - 1].unk0 * m / 50;
    if (r >= 10000) {
        r = 9999;
    }
    return r;
}

s32 chr_calcMaxMp(s32 lv, s32 m) {
    s32 r = g_levelHpMp[lv - 1].unk2 * m / 100;
    if (r >= 1000) {
        r = 999;
    }
    return r;
}

s32 chr_sumEquipBonus(u8 *eq, s32 k) {
    s32 sum = 0;
    s32 i;
    u8 *e;

    for (i = 0; i < 5; i++) {
        if (eq[i] != 0xFF) {
            e = g_equipStatBonus[g_itemTable[eq[i]].statBonus].unk0;
            switch (k) {
            case 0:
                sum += e[0];
                break;
            case 1:
                sum += e[1];
                break;
            case 2:
                sum += e[2];
                break;
            case 3:
                sum += e[3];
                break;
            }
        }
    }
    return sum;
}

