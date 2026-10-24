#include "ff9.h"

s32 vib_init(Unk8007BD44 *t) {
    u32 max;
    u32 *pm = &max;
    u16 *r;
    s32 i;
    s32 j;
    Unk8007BD44 *c;

    g_vib.unk0 = 1;
    g_vib.unk6 = 0x100;
    g_vib.unk8 = 0;
    g_vib.unkC = t;
    g_vib.unk2 = 0;
    *pm = 0;
    r = (u16 *)((u8 *)t + t->unk6);
    for (i = 0; i < t->unk4; r += 6, i++) {
        vib_init_max(pm, r[2]);
        vib_init_max(pm, r[3]);
    }
    g_vib.unk4 = max - 1;
    c = g_vib.unkC;
    for (j = 0; j < c->unk4; j++) {
        vib_selectTrack(j, 0, 0);
        vib_selectTrack(j, 1, 0);
    }
    vib_selectTrack(0, 0, 1);
    vib_selectTrack(0, 1, 1);
    g_padActuator[0].unk0 = 0;
    g_padActuator[0].unk1 = 0;
    g_padActuator[1].unk0 = 0;
    g_padActuator[1].unk1 = 0;
    return 0;
}

s32 vib_update(void) {
    Unk8007BD44 *t;
    u16 *r;
    s32 pos;
    s32 i;
    s32 a;
    s32 b;
    u16 f;

    if (!(g_vib.unk0 & 1)) {
        return 3;
    }
    if (g_vib.unkC == NULL) {
        return 1;
    }
    t = g_vib.unkC;
    if (!(g_vib.unk0 & 2) || !(g_vib.unk0 & 0xC)) {
        return 0;
    }
    b = 0;
    i = 0;
    a = 0;
    r = (u16 *)((u8 *)t + t->unk6);
    pos = g_vib.unk8 >> 8;
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
    if (!((g_gameState->config & 0xFF) >> 7) || g_gameState->mode == 5) {
        g_padActuator[0].unk0 = b;
        g_padActuator[0].unk1 = a;
        if (g_gameState->mode == 2 && func_800EA27C()) {
            g_padActuator[1].unk0 = b;
            g_padActuator[1].unk1 = a;
        }
    }
    g_vib.unk8 += g_vib.unk6;
    if (g_vib.unk4 < (g_vib.unk8 >> 8)) {
        if (g_vib.unk0 & 0x10) {
            g_vib.unk8 = g_vib.unk4 << 8;
            vib_setRate(-g_vib.unk6);
        } else {
            g_vib.unk8 = g_vib.unk2 << 8;
        }
        f = g_vib.unk0;
        g_vib.unk0 = f & ~4;
        if (!(f & 8)) {
            g_padActuator[0].unk0 = 0;
            g_padActuator[0].unk1 = 0;
            g_padActuator[1].unk0 = 0;
            g_padActuator[1].unk1 = 0;
            g_vib.unk0 = f & ~6;
        }
    }
    if ((g_vib.unk8 >> 8) < g_vib.unk2) {
        if (g_vib.unk0 & 0x10) {
            g_vib.unk8 = g_vib.unk2 << 8;
            vib_setRate(-g_vib.unk6);
        } else {
            g_vib.unk8 = g_vib.unk4 << 8;
        }
        f = g_vib.unk0;
        g_vib.unk0 = f & ~4;
        if (!(f & 8)) {
            g_padActuator[0].unk0 = 0;
            g_padActuator[0].unk1 = 0;
            g_padActuator[1].unk0 = 0;
            g_padActuator[1].unk1 = 0;
            g_vib.unk0 = f & ~6;
        }
    }
    return 0;
}

s32 vib_selectTrack(s32 row, s32 col, s32 sel) {
    Unk8007BD44Row *r;
    s32 i;

    r = (Unk8007BD44Row *)((u8 *)g_vib.unkC + g_vib.unkC->unk6);
    for (i = 0; i < g_vib.unkC->unk4; r++, i++) {
        r->unk0[col] &= ~1;
    }
    r = (Unk8007BD44Row *)((u8 *)g_vib.unkC + g_vib.unkC->unk6) + row;
    if (sel) {
        r->unk0[col] |= 1;
        g_padActuator[0].unk0 = 0;
        g_padActuator[0].unk1 = 0;
        g_padActuator[1].unk0 = 0;
        g_padActuator[1].unk1 = 0;
        g_vib.unk0 &= ~6;
        g_vib.unk8 = g_vib.unk2 << 8;
    } else {
        r->unk0[col] &= ~1;
    }
    return 0;
}

s32 vib_clear(void) {
    g_padActuator[0].unk0 = 0;
    g_padActuator[0].unk1 = 0;
    g_padActuator[1].unk0 = 0;
    g_padActuator[1].unk1 = 0;
    g_vib.unk0 = 0;
    g_vib.unk2 = 0;
    g_vib.unk4 = 0;
    g_vib.unkC = 0;
    return 0;
}

s32 vib_play(s32 a) {
    g_vib.unk2 = a;
    g_vib.unk8 = a << 8;
    g_vib.unk0 |= 6;
    return 0;
}

s32 vib_setPlaying(s32 on) {
    s32 v;
    if (on) {
        g_vib.unk0 |= 6;
        v = g_vib.unk8 >> 8;
        if (v < g_vib.unk2 || g_vib.unk4 < v) {
            g_vib.unk8 = g_vib.unk2 << 8;
        }
    } else {
        g_padActuator[0].unk0 = 0;
        g_padActuator[0].unk1 = 0;
        g_padActuator[1].unk0 = 0;
        g_padActuator[1].unk1 = 0;
        g_vib.unk0 &= ~6;
    }
    return 0;
}

s32 vib_setTrackEnabled(s32 a, s32 b, s32 on) {
    Unk8007BD44Row *e;

    e = (Unk8007BD44Row *)((u8 *)g_vib.unkC + g_vib.unkC->unk6) + a;
    if (on) {
        e->unk0[b] |= 1;
    } else {
        e->unk0[b] &= ~1;
    }
    return 0;
}

s32 vib_setRate(s16 a) {
    g_vib.unk6 = a;
    return 0;
}

s32 vib_setLoopMode(s32 a) {
    g_vib.unk0 = (g_vib.unk0 & ~0x18) | (a & 0x18);
    return 0;
}

s32 vib_setRange(s32 a, s32 b) {
    if (a >= 0) {
        g_vib.unk2 = a;
    }
    if (b >= 0) {
        g_vib.unk4 = b;
    }
    return 0;
}

s32 vib_getFlags(s32 *a) {
    *a = g_vib.unk0;
    return 0;
}

s32 vib_isTrackEnabled(s32 a, s32 b, s32 *out) {
    Unk8007BD44 *h = g_vib.unkC;
    u16 *p = (u16 *)((u8 *)h + h->unk6) + a * 6 + b;
    if (*p & 1) {
        *out = 1;
    } else {
        *out = 0;
    }
    return 0;
}

s32 vib_getData(Unk8007BD44 **a) {
    *a = g_vib.unkC;
    return 0;
}

s32 vib_getRange(s32 *a, s32 *b) {
    *a = g_vib.unk2;
    *b = g_vib.unk4;
    return 0;
}

s32 vib_getRate(s16 *a) {
    *a = g_vib.unk6;
    return 0;
}

s32 vib_getFrame(s32 *a) {
    *a = g_vib.unk8 >> 8;
    return 0;
}
