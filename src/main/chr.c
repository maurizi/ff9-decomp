#include "ff9.h"

void chr_init(s32 ch) {
    Character *c = &g_gameState->gameData->characters[ch];
    u32 *init;
    u32 *tbl;
    s32 r;

    memset(c, 0, 0x90);
    tbl = g_chrInitInfo;
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

void chr_setLevel(s32 ch, s32 lv, u32 *heal, u8 keepExp) {
    Character *c = &g_gameState->gameData->characters[ch];
    s32 hp;
    s32 mp;
    s32 d;

    if (!keepExp && c->level != lv) {
        c->exp = g_levelExp[lv - 1];
    }
    hp = c->hp;
    mp = c->mp;
    d = c->maxStones - c->stones;
    c->level = lv;
    memset(&c->hp, 0, 8);
    memset(&c->maxHp, 0, 8);
    c->unk16 = 10;
    c->baseSpeed = chr_calcBaseSpeed(ch, c->level, keepExp);
    c->baseStrength = chr_calcBaseStrength(ch, c->level, keepExp);
    c->baseMagic = chr_calcBaseMagic(ch, c->level, keepExp);
    c->baseSpirit = chr_calcBaseSpirit(ch, c->level, keepExp);
    c->maxHp = c->baseMaxHp = chr_calcMaxHp(c->level, c->baseStrength);
    c->maxMp = c->baseMaxMp = chr_calcMaxMp(c->level, c->baseMagic);
    c->maxStones = chr_calcMaxStones(ch, c->level, keepExp);
    c->hp = (hp < c->maxHp) ? hp : c->maxHp;
    c->mp = (mp < c->maxMp) ? mp : c->maxMp;
    c->stones = c->maxStones - d;
    if (heal) {
        c->hp = c->maxHp;
        c->mp = c->maxMp;
        c->stones = c->maxStones;
    }
    chr_updateStats(c);
    if (keepExp) {
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
    out->maxHp = *(u16 *)&in->base.b[0];
    out->maxMp = *(u16 *)&in->base.b[2];
    out->stats[0] = in->base.b[4];
    out->stats[1] = in->base.b[5];
    out->stats[2] = in->base.b[6];
    out->stats[3] = in->base.b[7];
    id = in->equip.b[0];
    if (id != 0xFF) {
        out->defs[0] = g_weaponTable[id].attack;
    }
    for (i = 0; i < 4; i++) {
        id = (&in->equip.b[1])[i];
        if (id != 0xFF) {
            if ((u32)(id - 0x58) < 0x88) {
                a = g_armorDefs[id];
                out->defs[1] += a[0];
                out->defs[2] += a[1];
                out->defs[3] += a[2];
                out->defs[4] += a[3];
            }
        }
    }
    for (i = 0; i < 5; i++) {
        id = in->equip.b[i];
        if (id != 0xFF) {
            e = g_equipStatBonus[g_itemTable[id].statBonus].stats;
            out->stats[0] += e[0];
            out->stats[1] += e[1];
            out->stats[2] += e[2];
            out->stats[3] += e[3];
        }
    }
    for (i = 0; i < 4; i++) {
        if (chr_testSupportBit(in->support.w, g_chrHpMpSupports[i])) {
            switch (g_chrHpMpSupports[i]) {
            case SA_HP_10:
                out->maxHp += out->maxHp / 10;
                break;
            case SA_HP_20:
                out->maxHp += out->maxHp / 5;
                break;
            case SA_MP_10:
                out->maxMp += out->maxMp / 10;
                break;
            case SA_MP_20:
                out->maxMp += out->maxMp / 5;
                break;
            }
        }
    }
    for (i = 0; i < 4; i++) {
        if (out->stats[i] > g_chrStatCaps[i]) {
            out->stats[i] = g_chrStatCaps[i];
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
    u8 t = g_chrAppearanceTypes[kind][(u8)flag != 0];
    u8 k = g_itemTable[*item].icon;
    s32 r;

    switch (t) { /* the character (stat class numbering: g_chrAppearanceTypes gives slots 5-8 with the guest flag 8-11) */
    case CHR_ZIDANE:
        return k != ITEM_ICON_DAGGER; /* CHR_KIND_ZIDANE : CHR_KIND_ZIDANE_DAGGER */
    case CHR_VIVI:
        return CHR_KIND_VIVI;
    case CHR_DAGGER:
        if ((u16)g_evtGlobalVars.h[0] >= 10300) {
            r = CHR_KIND_GARNET_SHORT;
            if (k == ITEM_ICON_RACKET) {
                r = CHR_KIND_GARNET_SHORT_RACKET;
            }
            return r;
        }
        r = CHR_KIND_GARNET_LONG;
        if (k == ITEM_ICON_RACKET) {
            r = CHR_KIND_GARNET_LONG_RACKET;
        }
        return r;
    case CHR_STEINER:
        return CHR_KIND_STEINER;
    case CHR_FREYA:
        return CHR_KIND_FREYA;
    case CHR_QUINA:
        return CHR_KIND_QUINA;
    case CHR_EIKO:
        r = CHR_KIND_EIKO;
        if (k == ITEM_ICON_RACKET) {
            r = CHR_KIND_EIKO_RACKET;
        }
        return r;
    case CHR_AMARANT:
        return CHR_KIND_AMARANT;
    case CHR_STATS_CINNA:
        return CHR_KIND_CINNA;
    case CHR_STATS_MARCUS:
        return CHR_KIND_MARCUS;
    case CHR_STATS_BLANK:
        r = CHR_KIND_BLANK;
        if ((u16)(g_evtGlobalVars.h[0] - 1500) < 100) {
            r = CHR_KIND_BLANK_ALT;
        }
        return r;
    case CHR_STATS_BEATRIX:
        return CHR_KIND_BEATRIX;
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
            p[i] = g_chrDefaultEquip[eq][i];
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
    for (i = 0; i < CHR_COUNT; i++) {
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
    if ((u32)(c->info.b.kind - CHR_KIND_EIKO) >= 2) {
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
    for (i = 0; i < CHR_COUNT; i++) {
        chr_init(i);
    }
    chr_setRecruited(CHR_ZIDANE);
    chr_setRecruited(CHR_VIVI);
    chr_setRecruited(CHR_DAGGER);
    chr_setRecruited(CHR_STEINER);
    chr_setRecruited(CHR_FREYA);
    chr_setRecruited(CHR_QUINA);
    chr_setRecruited(CHR_AMARANT);
    chr_setRecruited(CHR_EIKO);
    party_setMember(0, CHR_ZIDANE);
    party_setMember(1, CHR_VIVI);
    party_setMember(2, CHR_DAGGER);
    party_setMember(3, CHR_STEINER);
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
    chr_updateStats_inline(c);
}

s32 chr_getClass(u32 x) {
    switch (x) {
    case CHR_ZIDANE:
    case CHR_ROW_EXTRA:
        return CHR_ZIDANE;
    case CHR_VIVI:
        return CHR_VIVI;
    case CHR_DAGGER:
        return CHR_DAGGER;
    case CHR_STEINER:
        return CHR_STEINER;
    case CHR_FREYA:
        return CHR_FREYA;
    case CHR_QUINA:
        return CHR_QUINA;
    case CHR_EIKO:
        return CHR_EIKO;
    case CHR_AMARANT:
        return CHR_AMARANT;
    case CHR_ROW_CINNA:
    case CHR_ROW_CINNA_2:
    case CHR_ROW_EXTRA + 1:
        return CHR_STATS_CINNA;
    case CHR_ROW_MARCUS:
    case CHR_ROW_MARCUS_2:
    case CHR_ROW_EXTRA + 2:
        return CHR_STATS_MARCUS;
    case CHR_ROW_BLANK:
    case CHR_ROW_BLANK_2:
    case CHR_ROW_EXTRA + 3:
        return CHR_STATS_BLANK;
    case CHR_ROW_BEATRIX:
    case CHR_ROW_BEATRIX_2:
        return CHR_STATS_BEATRIX;
    }
    return -1;
}

u8 chr_getAppearanceType(s32 chr, u8 alt) {
    return g_chrAppearanceTypes[chr][alt != 0];
}

s32 chr_getDefaultSet(Character *c) {
    return g_chrDefaultSets[c->info.b.id][(c->classFlags & CHR_CLASS_GUEST) != 0];
}

u8 chr_getClassFlags(u32 x) {
    u8 *tbl = g_chrClassFlags;
    s32 i;
    switch (x) {
    case CHR_ZIDANE:
    case CHR_ROW_EXTRA:
        i = CHR_ZIDANE;
        break;
    case CHR_VIVI:
        i = CHR_VIVI;
        break;
    case CHR_DAGGER:
        i = CHR_DAGGER;
        break;
    case CHR_STEINER:
        i = CHR_STEINER;
        break;
    case CHR_FREYA:
        i = CHR_FREYA;
        break;
    case CHR_QUINA:
        i = CHR_QUINA;
        break;
    case CHR_EIKO:
        i = CHR_EIKO;
        break;
    case CHR_AMARANT:
        i = CHR_AMARANT;
        break;
    case CHR_ROW_CINNA:
    case CHR_ROW_CINNA_2:
    case CHR_ROW_EXTRA + 1:
        i = CHR_STATS_CINNA;
        break;
    case CHR_ROW_MARCUS:
    case CHR_ROW_MARCUS_2:
    case CHR_ROW_EXTRA + 2:
        i = CHR_STATS_MARCUS;
        break;
    case CHR_ROW_BLANK:
    case CHR_ROW_BLANK_2:
    case CHR_ROW_EXTRA + 3:
        i = CHR_STATS_BLANK;
        break;
    case CHR_ROW_BEATRIX:
    case CHR_ROW_BEATRIX_2:
        i = CHR_STATS_BEATRIX;
        break;
    default:
        i = -1;
        break;
    }
    return tbl[i];
}

void party_setMember(s32 slot, s32 ch) {
    Character *old = g_gameState->gameData->party[slot];
    if (ch >= CHR_COUNT) {
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
    if (i < CHR_COUNT) {
        g_gameState->gameData->characters[i].info.b.recruited = 1;
    }
}

void chr_clearRecruited(u32 i) {
    if (i < CHR_COUNT) {
        g_gameState->gameData->characters[i].info.b.recruited = 0;
    }
}

s32 chr_resetToLevel(s32 ch, s32 level, u8 refill) {
    Character *c = &g_gameState->gameData->characters[ch];
    s32 lv = c->level;
    c->level = 0;
    chr_setLevel(ch, level, 0, 0);
    if (refill) {
        c->hp = c->maxHp;
        c->mp = c->maxMp;
    }
    return lv;
}

s32 chr_levelUp(s32 ch, s32 level) {
    Character *c = &g_gameState->gameData->characters[ch];
    s32 oldLevel = c->level;
    chr_setLevel(ch, level, 0, 1);
    return oldLevel;
}

void party_setChanged(u8 changed) {
    g_partyChanged = changed;
}

u8 party_hasChanged(void) {
    return g_partyChanged;
}

/* default character names: count, then {offset, length} entries */
INCLUDE_RODATA("asm/us/main/nonmatchings/chr", g_chrDefaultNames);

void chr_setDefaultName(u8 *dst, s32 id) {
    memset(dst, 0xFF, 10);
    text_copy(dst, (u8 *)text_getFrom((u8 *)g_chrDefaultNames, id));
}

void chr_setDefaultEquip(u8 *dst, s32 set) {
    s32 i;
    for (i = 0; i < 5; i++) {
        dst[i] = g_chrDefaultEquip[set][i];
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
        if (chr_hasSupportAbility(id, i + ABILITY_SUPPORT_FIRST)) {
            if (c->stones >= e->stones) {
                c->stones -= e->stones;
            } else {
                chr_setSupportAbility(id, i + ABILITY_SUPPORT_FIRST, 0);
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
    c->ap[i] = g_abilityRows[c->info.b.row][i].ap;
    return 1;
}

void chr_initNew(void) {
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

void chr_clearAbilities(s32 ch) {
    chr_clearAp(ch);
    chr_clearSupport(ch);
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

/* Same logic as chr_findAbilitySlot_ap_inline (ff9.h). Written out because a call of the inline lays out the i < 0
   branch the other way round. */
s32 chr_getAp(s32 ch, s32 id) {
    s32 i = chr_findAbilitySlot_inline(ch, id);
    if (i < 0) {
        return -1;
    }
    return g_gameState->gameData->characters[ch].ap[i];
}

/* Same logic as chr_setAp_inline (below). Written out because a call of the inline lays out the i < 0 branch
   differently. */
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

/* Same logic as chr_getApNeeded_inline (ff9.h). Written out because a call of the inline lays out the i < 0 branch
   the other way round. */
u8 chr_getApNeeded(s32 ch, s32 id) {
    s32 i = chr_findAbilitySlot_inline(ch, id);
    Character *c = g_gameState->gameData->characters + ch;
    if (i < 0) {
        return 0;
    }
    return g_abilityRows[c->info.b.row][i].ap;
}

s32 chr_countSupportStones(s32 ch) {
    s32 sum = 0;
    s32 i;
    u32 *bits = g_gameState->gameData->characters[ch].supportAbilities;
    for (i = 0; i < 0x40; i++) {
        if (bits[i >> 5] & (1 << (i & 0x1F))) {
            sum += g_supportAbilities[i].stones;
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

u8 chr_testSupportBit(u32 *bits, s32 ability) {
    return chr_testSupportBit_inline(bits, ability);
}

u8 chr_canLearnAbilities(Character *c) {
    if (c->classFlags & CHR_CLASS_GUEST) {
        return 0;
    }
    return (s32)c->info.b.row < CHR_ROW_CINNA; /* not a guest row */
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

s32 chr_calcMaxHp(s32 lv, s32 mult) {
    s32 hp = g_levelHpMp[lv - 1].hp * mult / 50;
    if (hp >= 10000) {
        hp = 9999;
    }
    return hp;
}

s32 chr_calcMaxMp(s32 lv, s32 mult) {
    s32 mp = g_levelHpMp[lv - 1].mp * mult / 100;
    if (mp >= 1000) {
        mp = 999;
    }
    return mp;
}

s32 chr_sumEquipBonus(u8 *eq, s32 k) {
    s32 sum = 0;
    s32 i;
    u8 *e;

    for (i = 0; i < 5; i++) {
        if (eq[i] != 0xFF) {
            e = g_equipStatBonus[g_itemTable[eq[i]].statBonus].stats;
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

