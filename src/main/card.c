#include "ff9.h"

void card_initAll(void) {
    s32 v = 0xFF;
    s32 i;
    g_cards.wins = 0;
    g_cards.losses = 0;
    g_cards.draws = 0;
    for (i = 0x68; i >= 0; i--) {
        g_cards.cards[i].unk0 = v;
    }
}

void func_8006237C(void) {
    D_800834C8 = 1;
}

s32 card_add(s32 a) {
    s32 i;

    if (card_countAll() >= 100) {
        return -1;
    }
    for (i = 0; i < 100; i++) {
        if (g_cards.cards[i].unk0 == 0xFF) {
            card_rollStats(&g_cards.cards[i], a);
            return card_countOf(a) == 1;
        }
    }
    return -1;
}

s32 card_addWithStats(u8 id, u8 a, u8 b, u8 c, u8 d, u8 e) {
    s32 i;
    for (i = 0; i < 0x69; i++) {
        if (g_cards.cards[i].unk0 == 0xFF) {
            g_cards.cards[i].unk0 = id;
            g_cards.cards[i].unk1 = a;
            g_cards.cards[i].unk2 = b;
            g_cards.cards[i].unk3 = c;
            g_cards.cards[i].unk4 = d;
            g_cards.cards[i].unk5 = e;
            return card_countOf(id) == 1;
        }
    }
    return -1;
}

s16 card_getWins(void) {
    return g_cards.wins;
}

void card_addWin(void) {
    if (++g_cards.wins == 10000) {
        g_cards.wins = 9999;
    }
}

s16 card_getLosses(void) {
    return g_cards.losses;
}

void card_addLoss(void) {
    if (++g_cards.losses == 10000) {
        g_cards.losses = 9999;
    }
}

s16 card_getDraws(void) {
    return g_cards.draws;
}

void card_addDraw(void) {
    if (++g_cards.draws == 10000) {
        g_cards.draws = 9999;
    }
}

s32 card_countOf(a)
    u8 a;
{
    s32 n = 0;
    s32 i;
    for (i = 0; i < 0x69; i++) {
        if (g_cards.cards[i].unk0 == a) {
            n++;
        }
    }
    return n;
}

Unk6 *card_find(s32 id, s32 n) {
    s32 k = 0;
    s32 i;
    for (i = 0; i < 0x69; i++) {
        if (g_cards.cards[i].unk0 == (u8)id) {
            if (n == k) {
                return &g_cards.cards[i];
            }
            k++;
        }
    }
    return (Unk6 *)-1;
}

s32 card_countAll(void) {
    s32 n = 0;
    s32 i;
    for (i = 0; i < 0x69; i++) {
        if (g_cards.cards[i].unk0 != 0xFF) {
            n++;
        }
    }
    return n;
}

s32 card_countKinds(void) {
    u8 cnt[100];
    u8 *p;
    s32 i;
    s32 n;

    i = 99;
    p = &cnt[i];
    for (; i >= 0; i--) {
        *p-- = 0;
    }
    for (i = 0; i < 0x69; i++) {
        if (g_cards.cards[i].unk0 != 0xFF) {
            cnt[g_cards.cards[i].unk0]++;
        }
    }
    n = 0;
    for (i = 0; i < 100; i++) {
        if (cnt[i] != 0) {
            n++;
        }
    }
    return n;
}

s32 card_remove(s32 id, s32 n) {
    s32 k = 0;
    s32 i;
    for (i = 0; i < 0x69; i++) {
        if (g_cards.cards[i].unk0 == (u8)id) {
            if (k == n) {
                g_cards.cards[i].unk0 = 0xFF;
                return 0;
            }
            k++;
        }
    }
    return -1;
}

void card_clearAll(void) {
    s32 v = 0xFF;
    s32 i;
    for (i = 0x68; i >= 0; i--) {
        g_cards.cards[i].unk0 = v;
    }
}

void func_80062750(s32 a) {
    D_800834C8 = a;
}

s32 func_8006275C(void) {
    return D_800834C8;
}

s32 card_getName(s32 a) {
    return text_getFrom(D_8006FD60, a);
}

s32 card_getNameWidth(s32 a) {
    return text_measureWidth(card_getName(a));
}

s32 card_getRarity(s32 x) {
    if (x < 0x38) {
        return 0;
    }
    if (x < 0x46) {
        return 1;
    }
    if (x < 0x50) {
        return 2;
    }
    if (x < 0x58) {
        return 3;
    }
    if (x < 0x5D) {
        return 4;
    }
    if (x < 0x5F) {
        return 5;
    }
    return 6;
}

void *card_getInventory(void) {
    return &g_cards;
}

void card_rollStats(Unk6 *card, u8 id) {
    Bytes8 bits;
    u8 v;
    s32 r;
    s32 n;
    s32 i;
    s32 done;
    u8 *b;
    CardInfo *tbl;

    bits = D_800121C0;
    tbl = D_800121C8;
    card->unk0 = id;
    v = tbl[id].unk0 >> 1;
    if (v != 0) {
        card->unk2 = v + rand() % v;
    } else {
        card->unk2 = 0;
    }
    v = tbl[id].unk2 >> 1;
    if (v != 0) {
        card->unk4 = v + rand() % v;
    } else {
        card->unk4 = 0;
    }
    v = tbl[id].unk3 >> 1;
    if (v != 0) {
        card->unk5 = v + rand() % v;
    } else {
        card->unk5 = 0;
    }
    r = rand() % 100;
    if (r <= 0) {
        n = 0;
    } else if (r < 9) {
        n = 1;
    } else if (r < 0x22) {
        n = 2;
    } else if (r < 0x41) {
        n = 3;
    } else if (r < 0x53) {
        n = 4;
    } else if (r < 0x5C) {
        n = 5;
    } else if (r < 0x61) {
        n = 6;
    } else if (r < 0x63) {
        n = 7;
    } else {
        n = 8;
    }
    card->unk1 = 0;
    for (i = 0; i < n; i++) {
        done = 0;
        do {
            b = &bits.unk0[rand() & 7];
            if (!(card->unk1 & *b)) {
                card->unk1 |= *b;
                done = 1;
            }
        } while (!done);
    }
    card->unk3 = tbl[id].unk1;
    if (!(rand() & 0x7F)) {
        card->unk3 = 2;
    }
}

CardInfo *card_getInfoTable(void) {
    return D_800121C8;
}

INCLUDE_RODATA("asm/us/main/nonmatchings/card", D_800121C0);

INCLUDE_RODATA("asm/us/main/nonmatchings/card", D_800121C8);
