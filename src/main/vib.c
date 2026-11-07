#include "ff9.h"

s32 vib_init(VibData *data) {
    u32 max;
    u32 *pm = &max;
    u16 *track;
    s32 i;
    s32 j;
    VibData *curData;

    g_vib.flags = VIB_INIT;
    g_vib.rate = 0x100;
    g_vib.pos = 0;
    g_vib.data = data;
    g_vib.start = 0;
    *pm = 0;
    track = (u16 *)((u8 *)data + data->tracksOfs);
    for (i = 0; i < data->trackCount; track += 6, i++) {
        vib_init_max(pm, track[2]);
        vib_init_max(pm, track[3]);
    }
    g_vib.end = max - 1;
    curData = g_vib.data;
    for (j = 0; j < curData->trackCount; j++) {
        vib_selectTrack(j, 0, 0);
        vib_selectTrack(j, 1, 0);
    }
    vib_selectTrack(0, 0, 1);
    vib_selectTrack(0, 1, 1);
    g_padActuator[0].smallMotor = 0;
    g_padActuator[0].largeMotor = 0;
    g_padActuator[1].smallMotor = 0;
    g_padActuator[1].largeMotor = 0;
    return 0;
}

s32 vib_update(void) {
    VibData *data;
    u16 *track;
    s32 pos;
    s32 i;
    s32 large;
    s32 small;
    u16 flags;

    if (!(g_vib.flags & VIB_INIT)) {
        return 3;
    }
    if (g_vib.data == NULL) {
        return 1;
    }
    data = g_vib.data;
    if (!(g_vib.flags & VIB_PLAYING) || !(g_vib.flags & (VIB_RUNNING | VIB_LOOP))) {
        return 0;
    }
    small = 0;
    i = 0;
    large = 0;
    track = (u16 *)((u8 *)data + data->tracksOfs);
    pos = g_vib.pos >> 8;
    for (; i < data->trackCount; track += 6, i++) {
        if ((track[0] & 1) && track[2] >= pos) {
            large += *((u8 *)data + track[4] + pos);
        }
        if ((track[1] & 1) && track[3] >= pos) {
            small += *((u8 *)data + track[5] + pos);
        }
    }
    if (large >= 0x100) {
        large = 0xFF;
    }
    if (small >= 0x80) {
        small = 1;
    }
    if (!((g_gameState->config & 0xFF) >> CFG_BIT_VIBRATION) || g_gameState->mode == GS_MODE_UNUSED_5) {
        g_padActuator[0].smallMotor = small;
        g_padActuator[0].largeMotor = large;
        if (g_gameState->mode == GS_MODE_BATTLE && btlui_isSecondPortUsed()) {
            g_padActuator[1].smallMotor = small;
            g_padActuator[1].largeMotor = large;
        }
    }
    g_vib.pos += g_vib.rate;
    if (g_vib.end < (g_vib.pos >> 8)) {
        if (g_vib.flags & VIB_BOUNCE) {
            g_vib.pos = g_vib.end << 8;
            vib_setRate(-g_vib.rate);
        } else {
            g_vib.pos = g_vib.start << 8;
        }
        flags = g_vib.flags;
        g_vib.flags = flags & ~VIB_RUNNING;
        if (!(flags & VIB_LOOP)) {
            g_padActuator[0].smallMotor = 0;
            g_padActuator[0].largeMotor = 0;
            g_padActuator[1].smallMotor = 0;
            g_padActuator[1].largeMotor = 0;
            g_vib.flags = flags & ~(VIB_PLAYING | VIB_RUNNING);
        }
    }
    if ((g_vib.pos >> 8) < g_vib.start) {
        if (g_vib.flags & VIB_BOUNCE) {
            g_vib.pos = g_vib.start << 8;
            vib_setRate(-g_vib.rate);
        } else {
            g_vib.pos = g_vib.end << 8;
        }
        flags = g_vib.flags;
        g_vib.flags = flags & ~VIB_RUNNING;
        if (!(flags & VIB_LOOP)) {
            g_padActuator[0].smallMotor = 0;
            g_padActuator[0].largeMotor = 0;
            g_padActuator[1].smallMotor = 0;
            g_padActuator[1].largeMotor = 0;
            g_vib.flags = flags & ~(VIB_PLAYING | VIB_RUNNING);
        }
    }
    return 0;
}

s32 vib_selectTrack(s32 row, s32 col, s32 sel) {
    VibTrack *track;
    s32 i;

    track = (VibTrack *)((u8 *)g_vib.data + g_vib.data->tracksOfs);
    for (i = 0; i < g_vib.data->trackCount; track++, i++) {
        track->flags[col] &= ~1;
    }
    track = (VibTrack *)((u8 *)g_vib.data + g_vib.data->tracksOfs) + row;
    if (sel) {
        track->flags[col] |= 1;
        g_padActuator[0].smallMotor = 0;
        g_padActuator[0].largeMotor = 0;
        g_padActuator[1].smallMotor = 0;
        g_padActuator[1].largeMotor = 0;
        g_vib.flags &= ~(VIB_PLAYING | VIB_RUNNING);
        g_vib.pos = g_vib.start << 8;
    } else {
        track->flags[col] &= ~1;
    }
    return 0;
}

s32 vib_clear(void) {
    g_padActuator[0].smallMotor = 0;
    g_padActuator[0].largeMotor = 0;
    g_padActuator[1].smallMotor = 0;
    g_padActuator[1].largeMotor = 0;
    g_vib.flags = 0;
    g_vib.start = 0;
    g_vib.end = 0;
    g_vib.data = 0;
    return 0;
}

s32 vib_play(s32 frame) {
    g_vib.start = frame;
    g_vib.pos = frame << 8;
    g_vib.flags |= VIB_PLAYING | VIB_RUNNING;
    return 0;
}

s32 vib_setPlaying(s32 on) {
    s32 frame;
    if (on) {
        g_vib.flags |= VIB_PLAYING | VIB_RUNNING;
        frame = g_vib.pos >> 8;
        if (frame < g_vib.start || g_vib.end < frame) {
            g_vib.pos = g_vib.start << 8;
        }
    } else {
        g_padActuator[0].smallMotor = 0;
        g_padActuator[0].largeMotor = 0;
        g_padActuator[1].smallMotor = 0;
        g_padActuator[1].largeMotor = 0;
        g_vib.flags &= ~(VIB_PLAYING | VIB_RUNNING);
    }
    return 0;
}

s32 vib_setTrackEnabled(s32 track, s32 motor, s32 on) {
    VibTrack *entry;

    entry = (VibTrack *)((u8 *)g_vib.data + g_vib.data->tracksOfs) + track;
    if (on) {
        entry->flags[motor] |= 1;
    } else {
        entry->flags[motor] &= ~1;
    }
    return 0;
}

s32 vib_setRate(s16 rate) {
    g_vib.rate = rate;
    return 0;
}

s32 vib_setLoopMode(s32 mode) {
    g_vib.flags = (g_vib.flags & ~VIB_LOOP_MODE) | (mode & VIB_LOOP_MODE);
    return 0;
}

s32 vib_setRange(s32 start, s32 end) {
    if (start >= 0) {
        g_vib.start = start;
    }
    if (end >= 0) {
        g_vib.end = end;
    }
    return 0;
}

s32 vib_getFlags(s32 *outFlags) {
    *outFlags = g_vib.flags;
    return 0;
}

s32 vib_isTrackEnabled(s32 track, s32 motor, s32 *out) {
    VibData *h = g_vib.data;
    u16 *trackFlags = (u16 *)((u8 *)h + h->tracksOfs) + track * 6 + motor;
    if (*trackFlags & 1) {
        *out = 1;
    } else {
        *out = 0;
    }
    return 0;
}

s32 vib_getData(VibData **outData) {
    *outData = g_vib.data;
    return 0;
}

s32 vib_getRange(s32 *outStart, s32 *outEnd) {
    *outStart = g_vib.start;
    *outEnd = g_vib.end;
    return 0;
}

s32 vib_getRate(s16 *outRate) {
    *outRate = g_vib.rate;
    return 0;
}

s32 vib_getFrame(s32 *outFrame) {
    *outFrame = g_vib.pos >> 8;
    return 0;
}
