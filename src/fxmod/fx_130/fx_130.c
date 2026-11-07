#include "fxmod/fx_130.h"

static s32 fxm_easeLerp(s32 curve, s32 n, s32 t, s32 from, s32 to) {
    BtlfxExports *ex = BTLFX_EXPORTS(fx_130_env);
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

s32 fx_130_op80(s32 mode, Fx130Work *work, Unk800F78A4 *env, Unk80100C78Entry *entry, s32 *args) {
    BtlfxExports *ex = BTLFX_EXPORTS(env);
    Pool6CEntry *emitter;
    s32 *timer;
    s32 frame;
    s32 fade;
    s32 i;

    if (mode == 0) {
        *args = sizeof(Fx130Work);
    } else if (mode == 1) {
        fx_130_env = env;
        for (i = FX_PROPELLER_WIND_TIMERS - 1, timer = &work->timer0[FX_PROPELLER_WIND_TIMERS - 1]; i >= 0; i--) {
            *timer-- = 0;
        }
        ex->btlfx_initEffModels(fx_130_effModels, FX_PROPELLER_WIND_UNITS);
        work->pos20.vx = env->targets->centerX;
        work->pos20.vz = env->targets->centerZ;
        work->pos20.vy = 0;
        for (i = 0; i < FX_PROPELLER_WIND_UNITS; i++) {
            fx_130_units[i].timer18 = -1;
            fx_130_units[i].timer1C = -1;
        }
        for (i = 0; i < env->targets->targetCount; i++) {
            ex->btlfx_getUnitPos(i, &fx_130_units[i].pos0.vx);
            ex->btlfx_getUnitRot(i, &fx_130_units[i].rot8.vx);
            ex->btlfx_getUnitRot(i, &fx_130_units[i].rot10.vx);
            fx_130_units[i].timer18 = i * FX_PROPELLER_WIND_STAGGER_FRAMES;
            fx_130_units[i].timer1C = FX_PROPELLER_WIND_SPIN_FRAMES;
            fx_130_units[i].timer20 = i * FX_PROPELLER_WIND_STAGGER_FRAMES;
            fx_130_units[i].model24 = ex->btlfx_registerEffectModel((Unk800E8F50 *)ex->btlfx_getFileData(entry->file, FX_PROPELLER_WIND_TORNADO), 1);
            setVector(&fx_130_units[i].pos30, fx_130_units[i].pos0.vx, fx_130_units[i].pos0.vy, fx_130_units[i].pos0.vz);
            setVector(&fx_130_units[i].rot28, 0, 0, 0);
            setVector(&fx_130_units[i].scale40, 0x1000, 0x1000, 0x1000);
        }
    } else {
        frame = entry->frame;
        switch (frame) {
        case 0:
            emitter = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, FX_PROPELLER_WIND_WIND_SWIRL), (s32)&work->pos20);
            if (emitter != NULL) {
                emitter->otZ = -0x100;
            }
            emitter = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, FX_PROPELLER_WIND_WIND_RING), (s32)&work->pos20);
            if (emitter != NULL) {
                emitter->otZ = -0x100;
            }
            break;
        case FX_PROPELLER_WIND_SPIN_FRAME:
            work->timer0[FX_PROPELLER_WIND_T_TORNADOES] = FX_PROPELLER_WIND_TORNADOES_FRAMES;
            work->timer0[FX_PROPELLER_WIND_T_CRESCENTS] = FX_PROPELLER_WIND_CRESCENTS_FRAMES;
            break;
        case FX_PROPELLER_WIND_TEAL_RING_FRAME:
            emitter = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, FX_PROPELLER_WIND_TEAL_RING), (s32)&work->pos20);
            if (emitter != NULL) {
                emitter->otZ = -0x100;
            }
            break;
        }
        if (work->timer0[FX_PROPELLER_WIND_T_TORNADOES] != 0) {
            work->timer0[FX_PROPELLER_WIND_T_TORNADOES]--;
            for (i = 0; i < FX_PROPELLER_WIND_UNITS; i++) {
                if (fx_130_units[i].timer18 == 0) {
                    if (fx_130_units[i].timer1C != 0) {
                        fx_130_units[i].timer1C--;
                        ex->Hi_DrawEffModel(&fx_130_units[i].rot28, &fx_130_units[i].pos30, (s32)&fx_130_units[i].scale40, fx_130_units[i].model24);
                        fx_130_units[i].rot28.vy += 0x80;
                        if (fx_130_units[i].timer1C > FX_PROPELLER_WIND_SPIN_FRAMES / 2) {
                            fx_130_units[i].rot8.vy = (fx_130_units[i].rot10.vy + fxm_easeLerp(1, FX_PROPELLER_WIND_SPIN_FRAMES / 2, fx_130_units[i].timer1C - FX_PROPELLER_WIND_SPIN_FRAMES / 2, 0, 0x2000)) & 0xFFF;
                        } else {
                            fx_130_units[i].rot8.vy = (fx_130_units[i].rot10.vy + fxm_easeLerp(2, FX_PROPELLER_WIND_SPIN_FRAMES / 2, fx_130_units[i].timer1C, 0x2000, 0x4000)) & 0xFFF;
                        }
                        if (fx_130_units[i].timer1C == 0) {
                            fx_130_units[i].rot8.vy = fx_130_units[i].rot10.vy;
                        }
                        ex->btlfx_setUnitRot(i, &fx_130_units[i].rot8.vx);
                        if (fx_130_units[i].timer1C > (FX_PROPELLER_WIND_SPIN_FRAMES - FX_PROPELLER_WIND_GROW_FRAMES)) {
                            fx_130_units[i].scale40.vx = fx_130_units[i].scale40.vz = ex->btlfx_lerpRatio(0x1000, 0, fx_130_units[i].timer1C - (FX_PROPELLER_WIND_SPIN_FRAMES - FX_PROPELLER_WIND_GROW_FRAMES), FX_PROPELLER_WIND_GROW_FRAMES);
                            fade = -((fx_130_units[i].timer1C - (FX_PROPELLER_WIND_SPIN_FRAMES - FX_PROPELLER_WIND_GROW_FRAMES)) << 5);
                            ex->Hi_ModifyEffModelRGB(fx_130_units[i].model24, fade, fade, fade);
                        }
                        if (fx_130_units[i].timer1C < FX_PROPELLER_WIND_GROW_FRAMES) {
                            fx_130_units[i].scale40.vx = fx_130_units[i].scale40.vz = ex->btlfx_lerpRatio(0, 0x1000, fx_130_units[i].timer1C, FX_PROPELLER_WIND_GROW_FRAMES);
                            fade = -((FX_PROPELLER_WIND_GROW_FRAMES - fx_130_units[i].timer1C) << 5);
                            ex->Hi_ModifyEffModelRGB(fx_130_units[i].model24, fade, fade, fade);
                        }
                    }
                } else {
                    fx_130_units[i].timer18--;
                }
            }
        }
        if (work->timer0[FX_PROPELLER_WIND_T_CRESCENTS] != 0) {
            work->timer0[FX_PROPELLER_WIND_T_CRESCENTS]--;
            for (i = 0; i < FX_PROPELLER_WIND_UNITS; i++) {
                if (fx_130_units[i].timer20 == 0) {
                    ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, FX_PROPELLER_WIND_BLUE_CRESCENT), (s32)&fx_130_units[i]);
                }
                fx_130_units[i].timer20--;
            }
        }
        return frame >= FX_PROPELLER_WIND_FRAMES;
    }
    return 0;
}

INCLUDE_RODATA("asm/us/fx_130/nonmatchings/fx_130", fx_130_headerPtr);
