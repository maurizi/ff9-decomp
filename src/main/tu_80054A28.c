#include "ff9.h"

s32 func_80054A28(Unk8007BD44 *t) {
    u32 max;
    u32 *pm = &max;
    u16 *r;
    s32 i;
    s32 j;
    Unk8007BD44 *c;

    D_8007BD38.unk0 = 1;
    D_8007BD38.unk6 = 0x100;
    D_8007BD38.unk8 = 0;
    D_8007BD38.unkC = t;
    D_8007BD38.unk2 = 0;
    *pm = 0;
    r = (u16 *)((u8 *)t + t->unk6);
    for (i = 0; i < t->unk4; r += 6, i++) {
        func_80054A28_max(pm, r[2]);
        func_80054A28_max(pm, r[3]);
    }
    D_8007BD38.unk4 = max - 1;
    c = D_8007BD38.unkC;
    for (j = 0; j < c->unk4; j++) {
        func_80054E88(j, 0, 0);
        func_80054E88(j, 1, 0);
    }
    func_80054E88(0, 0, 1);
    func_80054E88(0, 1, 1);
    g_padActuator[0].unk0 = 0;
    g_padActuator[0].unk1 = 0;
    g_padActuator[1].unk0 = 0;
    g_padActuator[1].unk1 = 0;
    return 0;
}

s32 func_80054B78(void) {
    Unk8007BD44 *t;
    u16 *r;
    s32 pos;
    s32 i;
    s32 a;
    s32 b;
    u16 f;

    if (!(D_8007BD38.unk0 & 1)) {
        return 3;
    }
    if (D_8007BD38.unkC == NULL) {
        return 1;
    }
    t = D_8007BD38.unkC;
    if (!(D_8007BD38.unk0 & 2) || !(D_8007BD38.unk0 & 0xC)) {
        return 0;
    }
    b = 0;
    i = 0;
    a = 0;
    r = (u16 *)((u8 *)t + t->unk6);
    pos = D_8007BD38.unk8 >> 8;
    for (; i < t->unk4; r += 6, i++) {
        if ((r[0] & 1) && r[2] >= pos) {
            a += *((u8 *)t + r[4] + pos);
        }
        if ((r[1] & 1) && r[3] >= pos) {
            b += *((u8 *)t + r[5] + pos);
        }
    }
    if (a >= 0x100) {
        a = 0xFF;
    }
    if (b >= 0x80) {
        b = 1;
    }
    if (!((g_gameState->unk24 & 0xFF) >> 7) || g_gameState->mode == 5) {
        g_padActuator[0].unk0 = b;
        g_padActuator[0].unk1 = a;
        if (g_gameState->mode == 2 && func_800EA27C()) {
            g_padActuator[1].unk0 = b;
            g_padActuator[1].unk1 = a;
        }
    }
    D_8007BD38.unk8 += D_8007BD38.unk6;
    if (D_8007BD38.unk4 < (D_8007BD38.unk8 >> 8)) {
        if (D_8007BD38.unk0 & 0x10) {
            D_8007BD38.unk8 = D_8007BD38.unk4 << 8;
            func_800550C0(-D_8007BD38.unk6);
        } else {
            D_8007BD38.unk8 = D_8007BD38.unk2 << 8;
        }
        f = D_8007BD38.unk0;
        D_8007BD38.unk0 = f & ~4;
        if (!(f & 8)) {
            g_padActuator[0].unk0 = 0;
            g_padActuator[0].unk1 = 0;
            g_padActuator[1].unk0 = 0;
            g_padActuator[1].unk1 = 0;
            D_8007BD38.unk0 = f & ~6;
        }
    }
    if ((D_8007BD38.unk8 >> 8) < D_8007BD38.unk2) {
        if (D_8007BD38.unk0 & 0x10) {
            D_8007BD38.unk8 = D_8007BD38.unk2 << 8;
            func_800550C0(-D_8007BD38.unk6);
        } else {
            D_8007BD38.unk8 = D_8007BD38.unk4 << 8;
        }
        f = D_8007BD38.unk0;
        D_8007BD38.unk0 = f & ~4;
        if (!(f & 8)) {
            g_padActuator[0].unk0 = 0;
            g_padActuator[0].unk1 = 0;
            g_padActuator[1].unk0 = 0;
            g_padActuator[1].unk1 = 0;
            D_8007BD38.unk0 = f & ~6;
        }
    }
    return 0;
}

s32 func_80054E88(s32 row, s32 col, s32 sel) {
    Unk8007BD44Row *r;
    s32 i;

    r = (Unk8007BD44Row *)((u8 *)D_8007BD38.unkC + D_8007BD38.unkC->unk6);
    for (i = 0; i < D_8007BD38.unkC->unk4; r++, i++) {
        r->unk0[col] &= ~1;
    }
    r = (Unk8007BD44Row *)((u8 *)D_8007BD38.unkC + D_8007BD38.unkC->unk6) + row;
    if (sel) {
        r->unk0[col] |= 1;
        g_padActuator[0].unk0 = 0;
        g_padActuator[0].unk1 = 0;
        g_padActuator[1].unk0 = 0;
        g_padActuator[1].unk1 = 0;
        D_8007BD38.unk0 &= ~6;
        D_8007BD38.unk8 = D_8007BD38.unk2 << 8;
    } else {
        r->unk0[col] &= ~1;
    }
    return 0;
}

s32 func_80054F80(void) {
    g_padActuator[0].unk0 = 0;
    g_padActuator[0].unk1 = 0;
    g_padActuator[1].unk0 = 0;
    g_padActuator[1].unk1 = 0;
    D_8007BD38.unk0 = 0;
    D_8007BD38.unk2 = 0;
    D_8007BD38.unk4 = 0;
    D_8007BD38.unkC = 0;
    return 0;
}

s32 func_80054FB8(s32 a) {
    D_8007BD38.unk2 = a;
    D_8007BD38.unk8 = a << 8;
    D_8007BD38.unk0 |= 6;
    return 0;
}

s32 func_80054FE0(s32 on) {
    s32 v;
    if (on) {
        D_8007BD38.unk0 |= 6;
        v = D_8007BD38.unk8 >> 8;
        if (v < D_8007BD38.unk2 || D_8007BD38.unk4 < v) {
            D_8007BD38.unk8 = D_8007BD38.unk2 << 8;
        }
    } else {
        g_padActuator[0].unk0 = 0;
        g_padActuator[0].unk1 = 0;
        g_padActuator[1].unk0 = 0;
        g_padActuator[1].unk1 = 0;
        D_8007BD38.unk0 &= ~6;
    }
    return 0;
}

s32 func_80055060(s32 a, s32 b, s32 on) {
    Unk8007BD44Row *e;

    e = (Unk8007BD44Row *)((u8 *)D_8007BD38.unkC + D_8007BD38.unkC->unk6) + a;
    if (on) {
        e->unk0[b] |= 1;
    } else {
        e->unk0[b] &= ~1;
    }
    return 0;
}

s32 func_800550C0(s16 a) {
    D_8007BD38.unk6 = a;
    return 0;
}

s32 func_800550D0(s32 a) {
    D_8007BD38.unk0 = (D_8007BD38.unk0 & ~0x18) | (a & 0x18);
    return 0;
}

s32 func_800550F0(s32 a, s32 b) {
    if (a >= 0) {
        D_8007BD38.unk2 = a;
    }
    if (b >= 0) {
        D_8007BD38.unk4 = b;
    }
    return 0;
}

s32 func_80055110(s32 *a) {
    *a = D_8007BD38.unk0;
    return 0;
}

s32 func_80055128(s32 a, s32 b, s32 *out) {
    Unk8007BD44 *h = D_8007BD38.unkC;
    u16 *p = (u16 *)((u8 *)h + h->unk6) + a * 6 + b;
    if (*p & 1) {
        *out = 1;
    } else {
        *out = 0;
    }
    return 0;
}

s32 func_8005517C(Unk8007BD44 **a) {
    *a = D_8007BD38.unkC;
    return 0;
}

s32 func_80055194(s32 *a, s32 *b) {
    *a = D_8007BD38.unk2;
    *b = D_8007BD38.unk4;
    return 0;
}

s32 func_800551BC(s16 *a) {
    *a = D_8007BD38.unk6;
    return 0;
}

s32 func_800551D4(s32 *a) {
    *a = D_8007BD38.unk8 >> 8;
    return 0;
}
