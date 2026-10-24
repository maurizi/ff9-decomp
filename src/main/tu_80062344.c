#include "ff9.h"

void func_80062344(void) {
    s32 v = 0xFF;
    s32 i;
    D_80083248.unk0 = 0;
    D_80083248.unk2 = 0;
    D_80083248.unk4 = 0;
    for (i = 0x68; i >= 0; i--) {
        D_80083248.unk6[i].unk0 = v;
    }
}

void func_8006237C(void) {
    D_800834C8 = 1;
}

s32 func_8006238C(s32 a) {
    s32 i;

    if (func_800625F8() >= 100) {
        return -1;
    }
    for (i = 0; i < 100; i++) {
        if (D_80083248.unk6[i].unk0 == 0xFF) {
            func_80062830(&D_80083248.unk6[i], a);
            return func_80062560(a) == 1;
        }
    }
    return -1;
}

s32 func_80062420(u8 id, u8 a, u8 b, u8 c, u8 d, u8 e) {
    s32 i;
    for (i = 0; i < 0x69; i++) {
        if (D_80083248.unk6[i].unk0 == 0xFF) {
            D_80083248.unk6[i].unk0 = id;
            D_80083248.unk6[i].unk1 = a;
            D_80083248.unk6[i].unk2 = b;
            D_80083248.unk6[i].unk3 = c;
            D_80083248.unk6[i].unk4 = d;
            D_80083248.unk6[i].unk5 = e;
            return func_80062560(id) == 1;
        }
    }
    return -1;
}

s16 func_80062498(void) {
    return D_80083248.unk0;
}

void func_800624A8(void) {
    if (++D_80083248.unk0 == 10000) {
        D_80083248.unk0 = 9999;
    }
}

s16 func_800624D8(void) {
    return D_80083248.unk2;
}

void func_800624E8(void) {
    if (++D_80083248.unk2 == 10000) {
        D_80083248.unk2 = 9999;
    }
}

s16 func_8006251C(void) {
    return D_80083248.unk4;
}

void func_8006252C(void) {
    if (++D_80083248.unk4 == 10000) {
        D_80083248.unk4 = 9999;
    }
}

s32 func_80062560(a)
    u8 a;
{
    s32 n = 0;
    s32 i;
    for (i = 0; i < 0x69; i++) {
        if (D_80083248.unk6[i].unk0 == a) {
            n++;
        }
    }
    return n;
}

Unk6 *func_800625A0(s32 id, s32 n) {
    s32 k = 0;
    s32 i;
    for (i = 0; i < 0x69; i++) {
        if (D_80083248.unk6[i].unk0 == (u8)id) {
            if (n == k) {
                return &D_80083248.unk6[i];
            }
            k++;
        }
    }
    return (Unk6 *)-1;
}

s32 func_800625F8(void) {
    s32 n = 0;
    s32 i;
    for (i = 0; i < 0x69; i++) {
        if (D_80083248.unk6[i].unk0 != 0xFF) {
            n++;
        }
    }
    return n;
}

s32 func_80062638(void) {
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
        if (D_80083248.unk6[i].unk0 != 0xFF) {
            cnt[D_80083248.unk6[i].unk0]++;
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

s32 func_800626D0(s32 id, s32 n) {
    s32 k = 0;
    s32 i;
    for (i = 0; i < 0x69; i++) {
        if (D_80083248.unk6[i].unk0 == (u8)id) {
            if (k == n) {
                D_80083248.unk6[i].unk0 = 0xFF;
                return 0;
            }
            k++;
        }
    }
    return -1;
}

void func_80062724(void) {
    s32 v = 0xFF;
    s32 i;
    for (i = 0x68; i >= 0; i--) {
        D_80083248.unk6[i].unk0 = v;
    }
}

void func_80062750(s32 a) {
    D_800834C8 = a;
}

s32 func_8006275C(void) {
    return D_800834C8;
}

s32 func_8006276C(s32 a) {
    return text_getFrom(D_8006FD60, a);
}

s32 func_80062794(s32 a) {
    return func_8002BA68(func_8006276C(a));
}

s32 func_800627BC(s32 x) {
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

void *func_80062824(void) {
    return &D_80083248;
}

void func_80062830(Unk6 *card, u8 id) {
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

CardInfo *func_80062B30(void) {
    return D_800121C8;
}

INCLUDE_RODATA("asm/us/main/nonmatchings/tu_80062344", D_800121C0);

INCLUDE_RODATA("asm/us/main/nonmatchings/tu_80062344", D_800121C8);
