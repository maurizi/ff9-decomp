#include "fxmod/fx_18c.h"

static s32 fxm_easeLerp(s32 curve, s32 n, s32 t, s32 from, s32 to) {
    BtlfxExports *ex = BTLFX_EXPORTS(fx_18c_env);
    s32 invSq = 0x20000000 / (n * n);
    s32 ratio = 0;

    switch (curve) {
    case 0:
        ratio = 0x1000 - (((0x10000000 / n) * t) >> 16);
        break;
    case 1:
        ratio = (invSq * (n - t) * (n - t)) >> 17;
        break;
    case 2:
        ratio = 0x1000 - ((invSq * t * t) >> 17);
        break;
    }
    return ex->btlfx_lerp(from, to, ratio);
}

s32 fx_18c_op80(s32 mode, Fx18cWork *work, Unk800F78A4 *env, Unk80100C78Entry *entry, s32 *args) {
    BtlfxExports *ex = BTLFX_EXPORTS(env);
    SVECTOR pos;
    s32 frame;
    s32 i;

    if (mode == 0) {
        *args = 0x10;
    } else if (mode == 1) {
        fx_18c_env = env;
        for (i = 2; i >= 0; i--) {
            fx_18c_timers[i] = 0;
        }
        ex->btlfx_getUnitHomePos(0, (s16 *)&work->pos0);
        ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, 1), (s32)&work->pos0);
        ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, 2), (s32)&work->pos0);
    } else {
        frame = entry->frame;
        switch (frame) {
        case 0:
            fx_18c_timers[1] = 0x28;
            fx_18c_timers[0] = 0x10;
            break;
        case 24:
            fx_18c_timers[2] = 0x10;
            break;
        case 40:
            ex->btlfx_getUnitHomePos(0, (s16 *)&work->pos0);
            ex->btlfx_setUnitPos(0, (s16 *)&work->pos0);
            break;
        }
        if (fx_18c_timers[0] != 0) {
            fx_18c_timers[0]--;
            work->pos8.vy = fxm_easeLerp(2, 0x10, fx_18c_timers[0], work->pos0.vy, work->pos0.vy - 0x320);
        }
        if (fx_18c_timers[2] != 0) {
            fx_18c_timers[2]--;
            work->pos8.vy = fxm_easeLerp(0, 0x10, fx_18c_timers[2], work->pos0.vy - 0x320, work->pos0.vy);
        }
        if (fx_18c_timers[1] != 0) {
            fx_18c_timers[1]--;
            pos.vx = work->pos0.vx + (ex->math_sin(frame << 6) >> 7);
            pos.vy = work->pos8.vy + (ex->math_sin(frame << 8) >> 5);
            pos.vz = work->pos0.vz + (ex->math_sin(frame << 6) >> 7);
            ex->btlfx_setUnitPos(0, (s16 *)&pos);
        }
        return frame >= 0x48;
    }
    return 0;
}

INCLUDE_RODATA("asm/us/fx_18c/nonmatchings/fx_18c", fx_18c_headerPtr);
