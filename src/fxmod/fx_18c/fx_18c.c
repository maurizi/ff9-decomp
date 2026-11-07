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
        *args = sizeof(Fx18cWork);
    } else if (mode == 1) {
        fx_18c_env = env;
        for (i = FX_LIGHT_TIMERS - 1; i >= 0; i--) {
            fx_18c_timers[i] = 0;
        }
        ex->btlfx_getUnitHomePos(0, (s16 *)&work->pos0);
        ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, FX_LIGHT_VIOLET_STREAKS), (s32)&work->pos0);
        ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, FX_LIGHT_GREEN_HALO), (s32)&work->pos0);
    } else {
        frame = entry->frame;
        switch (frame) {
        case 0:
            fx_18c_timers[FX_LIGHT_T_HOVER] = FX_LIGHT_HOVER_FRAMES;
            fx_18c_timers[FX_LIGHT_T_RISE] = FX_LIGHT_RISE_FRAMES;
            break;
        case FX_LIGHT_DROP_FRAME:
            fx_18c_timers[FX_LIGHT_T_DROP] = FX_LIGHT_DROP_FRAMES;
            break;
        case FX_LIGHT_HOME_FRAME:
            ex->btlfx_getUnitHomePos(0, (s16 *)&work->pos0);
            ex->btlfx_setUnitPos(0, (s16 *)&work->pos0);
            break;
        }
        if (fx_18c_timers[FX_LIGHT_T_RISE] != 0) {
            fx_18c_timers[FX_LIGHT_T_RISE]--;
            work->pos8.vy = fxm_easeLerp(2, FX_LIGHT_RISE_FRAMES, fx_18c_timers[FX_LIGHT_T_RISE], work->pos0.vy, work->pos0.vy - FX_LIGHT_LIFT_HEIGHT);
        }
        if (fx_18c_timers[FX_LIGHT_T_DROP] != 0) {
            fx_18c_timers[FX_LIGHT_T_DROP]--;
            work->pos8.vy = fxm_easeLerp(0, FX_LIGHT_DROP_FRAMES, fx_18c_timers[FX_LIGHT_T_DROP], work->pos0.vy - FX_LIGHT_LIFT_HEIGHT, work->pos0.vy);
        }
        if (fx_18c_timers[FX_LIGHT_T_HOVER] != 0) {
            fx_18c_timers[FX_LIGHT_T_HOVER]--;
            pos.vx = work->pos0.vx + (ex->math_sin(frame << 6) >> 7);
            pos.vy = work->pos8.vy + (ex->math_sin(frame << 8) >> 5);
            pos.vz = work->pos0.vz + (ex->math_sin(frame << 6) >> 7);
            ex->btlfx_setUnitPos(0, (s16 *)&pos);
        }
        return frame >= FX_LIGHT_FRAMES;
    }
    return 0;
}

INCLUDE_RODATA("asm/us/fx_18c/nonmatchings/fx_18c", fx_18c_headerPtr);
