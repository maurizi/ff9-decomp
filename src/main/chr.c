#include "ff9.h"

void chr_init(s32 ch) {
    Character *chr = &g_gameState->gameData->characters[ch];
    u32 *init;
    u32 *tbl;
    s32 defSet;

    memset(chr, 0, 0x90);
    tbl = g_chrInitInfo;
    init = tbl + ch;
    chr->info.w = *init;
    chr->status = 0;
    chr->classFlags = chr_getClassFlags(chr->info.b.row);
    memset(chr->equipBonus, 0, 10);
    chr_clearAbilities(ch);
    defSet = chr_getDefaultSet(chr);
    chr_setDefaultName(chr->name, defSet);
    chr_setDefaultEquip(chr->equip, defSet);
    chr->info.b.kind = chr_calcAppearance(chr->info.b.id, (chr->classFlags >> 4) & 1, chr->equip);
    chr_setLevel(ch, 1, init, 0);
    chr->hp = chr->maxHp;
    chr->mp = chr->maxMp;
    chr->stones = chr->maxStones;
}

void chr_setLevel(s32 ch, s32 lv, u32 *heal, u8 keepExp) {
    Character *chr = &g_gameState->gameData->characters[ch];
    s32 hp;
    s32 mp;
    s32 usedStones;

    if (!keepExp && chr->level != lv) {
        chr->exp = g_levelExp[lv - 1];
    }
    hp = chr->hp;
    mp = chr->mp;
    usedStones = chr->maxStones - chr->stones;
    chr->level = lv;
    memset(&chr->hp, 0, 8);
    memset(&chr->maxHp, 0, 8);
    chr->unk16 = 10;
    chr->baseSpeed = chr_calcBaseSpeed(ch, chr->level, keepExp);
    chr->baseStrength = chr_calcBaseStrength(ch, chr->level, keepExp);
    chr->baseMagic = chr_calcBaseMagic(ch, chr->level, keepExp);
    chr->baseSpirit = chr_calcBaseSpirit(ch, chr->level, keepExp);
    chr->maxHp = chr->baseMaxHp = chr_calcMaxHp(chr->level, chr->baseStrength);
    chr->maxMp = chr->baseMaxMp = chr_calcMaxMp(chr->level, chr->baseMagic);
    chr->maxStones = chr_calcMaxStones(ch, chr->level, keepExp);
    chr->hp = (hp < chr->maxHp) ? hp : chr->maxHp;
    chr->mp = (mp < chr->maxMp) ? mp : chr->maxMp;
    chr->stones = chr->maxStones - usedStones;
    if (heal) {
        chr->hp = chr->maxHp;
        chr->mp = chr->maxMp;
        chr->stones = chr->maxStones;
    }
    chr_updateStats(chr);
    if (keepExp) {
        chr->hp = (hp < chr->maxHp) ? hp : chr->maxHp;
        chr->mp = (mp < chr->maxMp) ? mp : chr->maxMp;
    }
    if (chr->maxStones < usedStones) {
        chr_fitSupportAbilities(chr);
    }
}

EquipStatsOut *chr_calcEquipStats(EquipStatsIn *in, EquipStatsOut *out) {
    s32 i;
    u8 *armor;
    u8 *bonus;
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
                armor = g_armorDefs[id];
                out->defs[1] += armor[0];
                out->defs[2] += armor[1];
                out->defs[3] += armor[2];
                out->defs[4] += armor[3];
            }
        }
    }
    for (i = 0; i < 5; i++) {
        id = in->equip.b[i];
        if (id != 0xFF) {
            bonus = g_equipStatBonus[g_itemTable[id].statBonus].stats;
            out->stats[0] += bonus[0];
            out->stats[1] += bonus[1];
            out->stats[2] += bonus[2];
            out->stats[3] += bonus[3];
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
    if (out->maxHp > HP_MAX) {
        out->maxHp = HP_MAX;
    }
    if (out->maxMp > MP_MAX) {
        out->maxMp = MP_MAX;
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
    u8 chrType = g_chrAppearanceTypes[kind][(u8)flag != 0];
    u8 k = g_itemTable[*item].icon;
    s32 result;

    switch (chrType) { /* the character (stat class numbering: g_chrAppearanceTypes gives slots 5-8 with the guest flag 8-11) */
    case CHR_ZIDANE:
        return k != ITEM_ICON_DAGGER; /* CHR_KIND_ZIDANE : CHR_KIND_ZIDANE_DAGGER */
    case CHR_VIVI:
        return CHR_KIND_VIVI;
    case CHR_DAGGER:
        if ((u16)g_evtGlobalVars.h[0] >= 10300) {
            result = CHR_KIND_GARNET_SHORT;
            if (k == ITEM_ICON_RACKET) {
                result = CHR_KIND_GARNET_SHORT_RACKET;
            }
            return result;
        }
        result = CHR_KIND_GARNET_LONG;
        if (k == ITEM_ICON_RACKET) {
            result = CHR_KIND_GARNET_LONG_RACKET;
        }
        return result;
    case CHR_STEINER:
        return CHR_KIND_STEINER;
    case CHR_FREYA:
        return CHR_KIND_FREYA;
    case CHR_QUINA:
        return CHR_KIND_QUINA;
    case CHR_EIKO:
        result = CHR_KIND_EIKO;
        if (k == ITEM_ICON_RACKET) {
            result = CHR_KIND_EIKO_RACKET;
        }
        return result;
    case CHR_AMARANT:
        return CHR_KIND_AMARANT;
    case CHR_STATS_CINNA:
        return CHR_KIND_CINNA;
    case CHR_STATS_MARCUS:
        return CHR_KIND_MARCUS;
    case CHR_STATS_BLANK:
        result = CHR_KIND_BLANK;
        if ((u16)(g_evtGlobalVars.h[0] - 1500) < 100) {
            result = CHR_KIND_BLANK_ALT;
        }
        return result;
    case CHR_STATS_BEATRIX:
        return CHR_KIND_BEATRIX;
    }
    return -1;
}

void chr_join(s32 ch, u8 avg, s32 eq) {
    Character *chr = &g_gameState->gameData->characters[ch];
    u8 *equip;
    s32 i;
    s32 count;
    s32 sum;
    s32 lv;

    if (eq != 0xFF) {
        equip = chr->equip;
        for (i = 0; i < 5; i++) {
            equip[i] = g_chrDefaultEquip[eq][i];
        }
    }
    chr->info.b.kind = chr_calcAppearance(chr->info.b.id, (chr->classFlags >> 4) & 1, chr->equip);
    if (avg == 0) {
        if (eq == 0xFF) {
            return;
        }
        goto calc;
    }
    count = 0;
    sum = 0;
    for (i = 0; i < CHR_COUNT; i++) {
        if (ch != i) {
            if (g_gameState->gameData->characters[i].info.b.recruited) {
                count++;
                sum += g_gameState->gameData->characters[i].level;
            }
        }
    }
    if (count != 0) {
        lv = sum / count;
    } else {
        lv = 1;
    }
    if ((u32)(chr->info.b.kind - CHR_KIND_EIKO) >= 2) {
        if (lv < chr->level) {
            lv = chr->level;
        }
    }
    chr_setLevel(ch, lv, NULL, 0);
    return;
calc:
    chr_updateStats_inline(chr);
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

void chr_updateStats(Character *chr) {
    EquipStatsIn in;
    EquipStatsOut out;

    in.base = *(Bytes8 *)&chr->baseMaxHp;
    in.hp = chr->hp;
    in.mp = chr->mp;
    in.support = *(Words2 *)chr->supportAbilities;
    in.equip = *(Bytes5 *)chr->equip;
    chr_calcEquipStats(&in, &out);
    chr->speed = out.stats[0];
    chr->strength = out.stats[1];
    chr->magic = out.stats[2];
    chr->spirit = out.stats[3];
    chr->defense = out.defs[1];
    chr->evade = out.defs[2];
    chr->magicDef = out.defs[3];
    chr->magicEva = out.defs[4];
    chr->hp = out.hp;
    chr->mp = out.mp;
    chr->maxHp = out.maxHp;
    chr->maxMp = out.maxMp;
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

s32 chr_getDefaultSet(Character *chr) {
    return g_chrDefaultSets[chr->info.b.id][(chr->classFlags & CHR_CLASS_GUEST) != 0];
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
    Character *chr = &g_gameState->gameData->characters[ch];
    s32 lv = chr->level;
    chr->level = 0;
    chr_setLevel(ch, level, 0, 0);
    if (refill) {
        chr->hp = chr->maxHp;
        chr->mp = chr->maxMp;
    }
    return lv;
}

s32 chr_levelUp(s32 ch, s32 level) {
    Character *chr = &g_gameState->gameData->characters[ch];
    s32 oldLevel = chr->level;
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

void chr_fitSupportAbilities(Character *chr) {
    s32 i;
    Unk8006D128 *ability;
    s32 id;

    i = 0;
    ability = g_supportAbilities;
    id = chr->info.b.id;
    chr->stones = chr->maxStones;
    do {
        if (chr_hasSupportAbility(id, i + ABILITY_SUPPORT_FIRST)) {
            if (chr->stones >= ability->stones) {
                chr->stones -= ability->stones;
            } else {
                chr_setSupportAbility(id, i + ABILITY_SUPPORT_FIRST, 0);
            }
        }
        i++;
        ability++;
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
    Character *chr = g_gameState->gameData->characters + ch;
    if (i < 0) {
        return 0;
    }
    chr->ap[i] = g_abilityRows[chr->info.b.row][i].ap;
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
    Character *chr;

    if (ch < 8) {
        chr = &g_gameState->gameData->characters[ch];
        for (i = 0; i < 0x30; i++) {
            chr->ap[i] = 0;
        }
    }
}

void chr_clearSupport(s32 i) {
    Character *chr = &g_gameState->gameData->characters[i];
    chr->supportAbilities[1] = 0;
    chr->supportAbilities[0] = 0;
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

s32 chr_setAp(s32 ch, s32 id, u8 ap) {
    s32 i = chr_findAbilitySlot_inline(ch, id);
    s32 old;
    if (i < 0) {
        return -1;
    }
    old = g_gameState->gameData->characters[ch].ap[i];
    g_gameState->gameData->characters[ch].ap[i] = ap;
    return old;
}

static inline s32 chr_setAp_inline(s32 ch, s32 id, s32 ap) {
    s32 i = chr_findAbilitySlot_inline(ch, id);
    s32 old;
    if (i < 0) {
        return -1;
    }
    old = g_gameState->gameData->characters[ch].ap[i];
    g_gameState->gameData->characters[ch].ap[i] = ap;
    return old;
}

s32 chr_resetAp(s32 ch, s32 id) {
    s32 old = chr_setAp_inline(ch, id, 0);
    return old;
}

u8 chr_getApNeeded(s32 ch, s32 id) {
    s32 i = chr_findAbilitySlot_inline(ch, id);
    Character *chr = g_gameState->gameData->characters + ch;
    if (i < 0) {
        return 0;
    }
    return g_abilityRows[chr->info.b.row][i].ap;
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
    ability -= 0xC0;
    return (bits[ability >> 5] & (1 << (ability & 0x1F))) != 0;
}

u8 chr_canLearnAbilities(Character *chr) {
    if (chr->classFlags & CHR_CLASS_GUEST) {
        return 0;
    }
    return (s32)chr->info.b.row < CHR_ROW_CINNA; /* not a guest row */
}

s32 chr_calcBaseSpeed(s32 ch, s32 lv, u8 add) {
    Character *chr = g_gameState->gameData->characters + ch;
    ClassStats *cls = &g_charClassStats[chr_getClass(chr->info.b.row)];
    s32 stat;
    if (add) {
        chr->equipBonus[0] += chr_sumEquipBonus(chr->equip, 0);
    }
    stat = cls->speed + lv / 10 + (chr->equipBonus[0] >> 5);
    if (stat >= 51) {
        stat = 50;
    }
    return stat;
}

s32 chr_calcBaseStrength(s32 ch, s32 lv, u8 add) {
    Character *chr = g_gameState->gameData->characters + ch;
    s32 k = chr_getClass(chr->info.b.row);
    u16 *bonus = chr->equipBonus;
    ClassStats *cls = &g_charClassStats[k];
    s32 stat;
    s32 extra;
    if (add) {
        extra = 0;
        if (chr->stones == 0) {
            extra = 3;
        }
        bonus[1] += extra + chr_sumEquipBonus(chr->equip, 1);
    }
    stat = cls->strength + lv * 3 / 10 + (bonus[1] >> 5);
    if (stat >= 100) {
        stat = 99;
    }
    return stat;
}

s32 chr_calcBaseMagic(s32 ch, s32 lv, u8 add) {
    Character *chr = g_gameState->gameData->characters + ch;
    s32 k = chr_getClass(chr->info.b.row);
    u16 *bonus = chr->equipBonus;
    ClassStats *cls = &g_charClassStats[k];
    s32 stat;
    s32 extra;
    if (add) {
        extra = 0;
        if (chr->stones == 0) {
            extra = 3;
        }
        bonus[2] += extra + chr_sumEquipBonus(chr->equip, 2);
    }
    stat = cls->magic + lv * 3 / 10 + (bonus[2] >> 5);
    if (stat >= 100) {
        stat = 99;
    }
    return stat;
}

s32 chr_calcBaseSpirit(s32 ch, s32 lv, u8 add) {
    Character *chr = g_gameState->gameData->characters + ch;
    s32 k = chr_getClass(chr->info.b.row);
    u16 *bonus = chr->equipBonus;
    ClassStats *cls = &g_charClassStats[k];
    s32 stat;
    s32 extra;
    if (add) {
        extra = chr->stones == 0;
        bonus[3] += extra + chr_sumEquipBonus(chr->equip, 3);
    }
    stat = cls->spirit + lv * 3 / 20 + (bonus[3] >> 5);
    if (stat >= 51) {
        stat = 50;
    }
    return stat;
}

s32 chr_calcMaxStones(s32 ch, s32 lv, u8 add) {
    Character *chr = g_gameState->gameData->characters + ch;
    u16 *bonus = chr->equipBonus;
    s32 k = chr_getClass(chr->info.b.row);
    ClassStats *cls = &g_charClassStats[k];
    s32 stat;
    s32 extra;

    if (add) {
        extra = 0;
        if (chr->stones == 0) {
            extra = 5;
        }
        bonus[4] += extra;
    }
    stat = cls->stones + lv * 4 / 10 + (bonus[4] >> 5);
    if (stat >= 100) {
        stat = 99;
    }
    return stat;
}

s32 chr_calcMaxHp(s32 lv, s32 mult) {
    s32 hp = g_levelHpMp[lv - 1].hp * mult / 50;
    if (hp > HP_MAX) {
        hp = HP_MAX;
    }
    return hp;
}

s32 chr_calcMaxMp(s32 lv, s32 mult) {
    s32 mp = g_levelHpMp[lv - 1].mp * mult / 100;
    if (mp > MP_MAX) {
        mp = MP_MAX;
    }
    return mp;
}

s32 chr_sumEquipBonus(u8 *eq, s32 k) {
    s32 sum = 0;
    s32 i;
    u8 *bonus;

    for (i = 0; i < 5; i++) {
        if (eq[i] != 0xFF) {
            bonus = g_equipStatBonus[g_itemTable[eq[i]].statBonus].stats;
            switch (k) {
            case 0:
                sum += bonus[0];
                break;
            case 1:
                sum += bonus[1];
                break;
            case 2:
                sum += bonus[2];
                break;
            case 3:
                sum += bonus[3];
                break;
            }
        }
    }
    return sum;
}

