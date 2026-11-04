#include "ff9.h"

void card_initAll(void) {
    s32 emptyId = 0xFF;
    s32 i;
    g_cards.wins = 0;
    g_cards.losses = 0;
    g_cards.draws = 0;
    for (i = CARD_SLOTS - 1; i >= 0; i--) {
        g_cards.cards[i].id = emptyId;
    }
}

void card_resetGameResult(void) {
    g_cardGameResult = 1;
}

s32 card_add(s32 cardId) {
    s32 i;

    if (card_countAll() >= CARD_MAX) {
        return -1;
    }
    for (i = 0; i < CARD_MAX; i++) {
        if (g_cards.cards[i].id == CARD_NONE) {
            card_rollStats(&g_cards.cards[i], cardId);
            return card_countOf(cardId) == 1;
        }
    }
    return -1;
}

s32 card_addWithStats(u8 id, u8 arrows, u8 power, u8 type, u8 physDef, u8 magicDef) {
    s32 i;
    for (i = 0; i < CARD_SLOTS; i++) {
        if (g_cards.cards[i].id == CARD_NONE) {
            g_cards.cards[i].id = id;
            g_cards.cards[i].arrows = arrows;
            g_cards.cards[i].power = power;
            g_cards.cards[i].type = type;
            g_cards.cards[i].physDef = physDef;
            g_cards.cards[i].magicDef = magicDef;
            return card_countOf(id) == 1;
        }
    }
    return -1;
}

s16 card_getWins(void) {
    return g_cards.wins;
}

void card_addWin(void) {
    if (++g_cards.wins == RECORD_MAX + 1) {
        g_cards.wins = RECORD_MAX;
    }
}

s16 card_getLosses(void) {
    return g_cards.losses;
}

void card_addLoss(void) {
    if (++g_cards.losses == RECORD_MAX + 1) {
        g_cards.losses = RECORD_MAX;
    }
}

s16 card_getDraws(void) {
    return g_cards.draws;
}

void card_addDraw(void) {
    if (++g_cards.draws == RECORD_MAX + 1) {
        g_cards.draws = RECORD_MAX;
    }
}

s32 card_countOf(cardId)
    u8 cardId;
{
    s32 count = 0;
    s32 i;
    for (i = 0; i < CARD_SLOTS; i++) {
        if (g_cards.cards[i].id == cardId) {
            count++;
        }
    }
    return count;
}

CardEntry *card_find(s32 id, s32 nth) {
    s32 k = 0;
    s32 i;
    for (i = 0; i < CARD_SLOTS; i++) {
        if (g_cards.cards[i].id == (u8)id) {
            if (nth == k) {
                return &g_cards.cards[i];
            }
            k++;
        }
    }
    return (CardEntry *)-1;
}

s32 card_countAll(void) {
    s32 count = 0;
    s32 i;
    for (i = 0; i < CARD_SLOTS; i++) {
        if (g_cards.cards[i].id != CARD_NONE) {
            count++;
        }
    }
    return count;
}

s32 card_countKinds(void) {
    u8 cnt[CARD_KINDS];
    u8 *p;
    s32 i;
    s32 kinds;

    i = CARD_KINDS - 1;
    p = &cnt[i];
    for (; i >= 0; i--) {
        *p-- = 0;
    }
    for (i = 0; i < CARD_SLOTS; i++) {
        if (g_cards.cards[i].id != CARD_NONE) {
            cnt[g_cards.cards[i].id]++;
        }
    }
    kinds = 0;
    for (i = 0; i < CARD_KINDS; i++) {
        if (cnt[i] != 0) {
            kinds++;
        }
    }
    return kinds;
}

s32 card_remove(s32 id, s32 nth) {
    s32 k = 0;
    s32 i;
    for (i = 0; i < CARD_SLOTS; i++) {
        if (g_cards.cards[i].id == (u8)id) {
            if (k == nth) {
                g_cards.cards[i].id = CARD_NONE;
                return 0;
            }
            k++;
        }
    }
    return -1;
}

void card_clearAll(void) {
    s32 emptyId = 0xFF;
    s32 i;
    for (i = CARD_SLOTS - 1; i >= 0; i--) {
        g_cards.cards[i].id = emptyId;
    }
}

void card_setGameResult(s32 result) {
    g_cardGameResult = result;
}

s32 card_getGameResult(void) {
    return g_cardGameResult;
}

s32 card_getName(s32 cardId) {
    return text_getFrom(g_cardNames, cardId);
}

s32 card_getNameWidth(s32 cardId) {
    return text_measureWidth(card_getName(cardId));
}

s32 card_getRarity(s32 id) {
    if (id < CARD_HOLY) {
        return 0;
    }
    if (id < CARD_EXCALIBUR_II) {
        return 1;
    }
    if (id < CARD_BLUE_NARCISS) {
        return 2;
    }
    if (id < CARD_CHOCOBO) {
        return 3;
    }
    if (id < CARD_ALEXANDRIA) {
        return 4;
    }
    if (id < CARD_TWO_MOONS) {
        return 5;
    }
    return 6;
}

void *card_getInventory(void) {
    return &g_cards;
}

void card_rollStats(CardEntry *card, u8 id) {
    Bytes8 bits;
    u8 base;
    s32 roll;
    s32 arrowCount;
    s32 i;
    s32 done;
    u8 *bit;
    CardInfo *tbl;

    bits = g_cardArrowBits;
    tbl = g_cardInfo;
    card->id = id;
    base = tbl[id].power >> 1;
    if (base != 0) {
        card->power = base + rand() % base;
    } else {
        card->power = 0;
    }
    base = tbl[id].physDef >> 1;
    if (base != 0) {
        card->physDef = base + rand() % base;
    } else {
        card->physDef = 0;
    }
    base = tbl[id].magicDef >> 1;
    if (base != 0) {
        card->magicDef = base + rand() % base;
    } else {
        card->magicDef = 0;
    }
    roll = rand() % 100;
    if (roll <= 0) {
        arrowCount = 0;
    } else if (roll < 9) {
        arrowCount = 1;
    } else if (roll < 0x22) {
        arrowCount = 2;
    } else if (roll < 0x41) {
        arrowCount = 3;
    } else if (roll < 0x53) {
        arrowCount = 4;
    } else if (roll < 0x5C) {
        arrowCount = 5;
    } else if (roll < 0x61) {
        arrowCount = 6;
    } else if (roll < 0x63) {
        arrowCount = 7;
    } else {
        arrowCount = 8;
    }
    card->arrows = 0;
    for (i = 0; i < arrowCount; i++) {
        done = 0;
        do {
            bit = &bits.b[rand() & 7];
            if (!(card->arrows & *bit)) {
                card->arrows |= *bit;
                done = 1;
            }
        } while (!done);
    }
    card->type = tbl[id].type;
    if (!(rand() & 0x7F)) {
        card->type = TM_TYPE_X;
    }
}

CardInfo *card_getInfoTable(void) {
    return g_cardInfo;
}

INCLUDE_RODATA("asm/us/main/nonmatchings/card", g_cardArrowBits);

INCLUDE_RODATA("asm/us/main/nonmatchings/card", g_cardInfo);
