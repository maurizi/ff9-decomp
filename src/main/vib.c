#include "ff9.h"

s32 vib_init(VibData *t) {
    u32 max;
    u32 *pm = &max;
    u16 *r;
    s32 i;
    s32 j;
    VibData *c;

    g_vib.flags = 1;
    g_vib.rate = 0x100;
    g_vib.pos = 0;
    g_vib.data = t;
    g_vib.start = 0;
    *pm = 0;
    r = (u16 *)((u8 *)t + t->tracksOfs);
    for (i = 0; i < t->trackCount; r += 6, i++) {
        vib_init_max(pm, r[2]);
        vib_init_max(pm, r[3]);
    }
    g_vib.end = max - 1;
    c = g_vib.data;
    for (j = 0; j < c->trackCount; j++) {
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
    VibData *t;
    u16 *r;
    s32 pos;
    s32 i;
    s32 a;
    s32 b;
    u16 f;

    if (!(g_vib.flags & 1)) {
        return 3;
    }
    if (g_vib.data == NULL) {
        return 1;
    }
    t = g_vib.data;
    if (!(g_vib.flags & 2) || !(g_vib.flags & 0xC)) {
        return 0;
    }
    b = 0;
    i = 0;
    a = 0;
    r = (u16 *)((u8 *)t + t->tracksOfs);
    pos = g_vib.pos >> 8;
    for (; i < t->trackCount; r += 6, i++) {
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
    g_vib.pos += g_vib.rate;
    if (g_vib.end < (g_vib.pos >> 8)) {
        if (g_vib.flags & 0x10) {
            g_vib.pos = g_vib.end << 8;
            vib_setRate(-g_vib.rate);
        } else {
            g_vib.pos = g_vib.start << 8;
        }
        f = g_vib.flags;
        g_vib.flags = f & ~4;
        if (!(f & 8)) {
            g_padActuator[0].unk0 = 0;
            g_padActuator[0].unk1 = 0;
            g_padActuator[1].unk0 = 0;
            g_padActuator[1].unk1 = 0;
            g_vib.flags = f & ~6;
        }
    }
    if ((g_vib.pos >> 8) < g_vib.start) {
        if (g_vib.flags & 0x10) {
            g_vib.pos = g_vib.start << 8;
            vib_setRate(-g_vib.rate);
        } else {
            g_vib.pos = g_vib.end << 8;
        }
        f = g_vib.flags;
        g_vib.flags = f & ~4;
        if (!(f & 8)) {
            g_padActuator[0].unk0 = 0;
            g_padActuator[0].unk1 = 0;
            g_padActuator[1].unk0 = 0;
            g_padActuator[1].unk1 = 0;
            g_vib.flags = f & ~6;
        }
    }
    return 0;
}

s32 vib_selectTrack(s32 row, s32 col, s32 sel) {
    VibTrack *r;
    s32 i;

    r = (VibTrack *)((u8 *)g_vib.data + g_vib.data->tracksOfs);
    for (i = 0; i < g_vib.data->trackCount; r++, i++) {
        r->unk0[col] &= ~1;
    }
    r = (VibTrack *)((u8 *)g_vib.data + g_vib.data->tracksOfs) + row;
    if (sel) {
        r->unk0[col] |= 1;
        g_padActuator[0].unk0 = 0;
        g_padActuator[0].unk1 = 0;
        g_padActuator[1].unk0 = 0;
        g_padActuator[1].unk1 = 0;
        g_vib.flags &= ~6;
        g_vib.pos = g_vib.start << 8;
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
    g_vib.flags = 0;
    g_vib.start = 0;
    g_vib.end = 0;
    g_vib.data = 0;
    return 0;
}

s32 vib_play(s32 a) {
    g_vib.start = a;
    g_vib.pos = a << 8;
    g_vib.flags |= 6;
    return 0;
}

s32 vib_setPlaying(s32 on) {
    s32 v;
    if (on) {
        g_vib.flags |= 6;
        v = g_vib.pos >> 8;
        if (v < g_vib.start || g_vib.end < v) {
            g_vib.pos = g_vib.start << 8;
        }
    } else {
        g_padActuator[0].unk0 = 0;
        g_padActuator[0].unk1 = 0;
        g_padActuator[1].unk0 = 0;
        g_padActuator[1].unk1 = 0;
        g_vib.flags &= ~6;
    }
    return 0;
}

s32 vib_setTrackEnabled(s32 a, s32 b, s32 on) {
    VibTrack *e;

    e = (VibTrack *)((u8 *)g_vib.data + g_vib.data->tracksOfs) + a;
    if (on) {
        e->unk0[b] |= 1;
    } else {
        e->unk0[b] &= ~1;
    }
    return 0;
}

s32 vib_setRate(s16 a) {
    g_vib.rate = a;
    return 0;
}

s32 vib_setLoopMode(s32 a) {
    g_vib.flags = (g_vib.flags & ~0x18) | (a & 0x18);
    return 0;
}

s32 vib_setRange(s32 a, s32 b) {
    if (a >= 0) {
        g_vib.start = a;
    }
    if (b >= 0) {
        g_vib.end = b;
    }
    return 0;
}

s32 vib_getFlags(s32 *a) {
    *a = g_vib.flags;
    return 0;
}

s32 vib_isTrackEnabled(s32 a, s32 b, s32 *out) {
    VibData *h = g_vib.data;
    u16 *p = (u16 *)((u8 *)h + h->tracksOfs) + a * 6 + b;
    if (*p & 1) {
        *out = 1;
    } else {
        *out = 0;
    }
    return 0;
}

s32 vib_getData(VibData **a) {
    *a = g_vib.data;
    return 0;
}

s32 vib_getRange(s32 *a, s32 *b) {
    *a = g_vib.start;
    *b = g_vib.end;
    return 0;
}

s32 vib_getRate(s16 *a) {
    *a = g_vib.rate;
    return 0;
}

s32 vib_getFrame(s32 *a) {
    *a = g_vib.pos >> 8;
    return 0;
}
