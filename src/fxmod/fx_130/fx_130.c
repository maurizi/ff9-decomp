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
        *args = 0x28;
    } else if (mode == 1) {
        fx_130_env = env;
        for (i = 7, timer = &work->timer0[7]; i >= 0; i--) {
            *timer-- = 0;
        }
        ex->btlfx_initEffModels(fx_130_effModels, 4);
        work->pos20.vx = env->targets->centerX;
        work->pos20.vz = env->targets->centerZ;
        work->pos20.vy = 0;
        for (i = 0; i < 4; i++) {
            fx_130_units[i].timer18 = -1;
            fx_130_units[i].timer1C = -1;
        }
        for (i = 0; i < env->targets->targetCount; i++) {
            ex->btlfx_getUnitPos(i, &fx_130_units[i].pos0.vx);
            ex->btlfx_getUnitRot(i, &fx_130_units[i].rot8.vx);
            ex->btlfx_getUnitRot(i, &fx_130_units[i].rot10.vx);
            fx_130_units[i].timer18 = i * 6;
            fx_130_units[i].timer1C = 0x20;
            fx_130_units[i].timer20 = i * 6;
            fx_130_units[i].model24 = ex->btlfx_registerEffectModel((Unk800E8F50 *)ex->btlfx_getFileData(entry->file, FX_PROPELLER_WIND_TARGET_MODEL), 1);
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
        case 8:
            work->timer0[0] = 0x38;
            work->timer0[1] = 0x20;
            break;
        case 0x2A:
            emitter = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, FX_PROPELLER_WIND_TEAL_RING), (s32)&work->pos20);
            if (emitter != NULL) {
                emitter->otZ = -0x100;
            }
            break;
        }
        if (work->timer0[0] != 0) {
            work->timer0[0]--;
            for (i = 0; i < 4; i++) {
                if (fx_130_units[i].timer18 == 0) {
                    if (fx_130_units[i].timer1C != 0) {
                        fx_130_units[i].timer1C--;
                        ex->Hi_DrawEffModel(&fx_130_units[i].rot28, &fx_130_units[i].pos30, (s32)&fx_130_units[i].scale40, fx_130_units[i].model24);
                        fx_130_units[i].rot28.vy += 0x80;
                        if (fx_130_units[i].timer1C > 0x10) {
                            fx_130_units[i].rot8.vy = (fx_130_units[i].rot10.vy + fxm_easeLerp(1, 0x10, fx_130_units[i].timer1C - 0x10, 0, 0x2000)) & 0xFFF;
                        } else {
                            fx_130_units[i].rot8.vy = (fx_130_units[i].rot10.vy + fxm_easeLerp(2, 0x10, fx_130_units[i].timer1C, 0x2000, 0x4000)) & 0xFFF;
                        }
                        if (fx_130_units[i].timer1C == 0) {
                            fx_130_units[i].rot8.vy = fx_130_units[i].rot10.vy;
                        }
                        ex->btlfx_setUnitRot(i, &fx_130_units[i].rot8.vx);
                        if (fx_130_units[i].timer1C > 0x18) {
                            fx_130_units[i].scale40.vx = fx_130_units[i].scale40.vz = ex->btlfx_lerpRatio(0x1000, 0, fx_130_units[i].timer1C - 0x18, 8);
                            fade = -((fx_130_units[i].timer1C - 0x18) << 5);
                            ex->Hi_ModifyEffModelRGB(fx_130_units[i].model24, fade, fade, fade);
                        }
                        if (fx_130_units[i].timer1C < 8) {
                            fx_130_units[i].scale40.vx = fx_130_units[i].scale40.vz = ex->btlfx_lerpRatio(0, 0x1000, fx_130_units[i].timer1C, 8);
                            fade = -((8 - fx_130_units[i].timer1C) << 5);
                            ex->Hi_ModifyEffModelRGB(fx_130_units[i].model24, fade, fade, fade);
                        }
                    }
                } else {
                    fx_130_units[i].timer18--;
                }
            }
        }
        if (work->timer0[1] != 0) {
            work->timer0[1]--;
            for (i = 0; i < 4; i++) {
                if (fx_130_units[i].timer20 == 0) {
                    ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, FX_PROPELLER_WIND_BLUE_CRESCENT), (s32)&fx_130_units[i]);
                }
                fx_130_units[i].timer20--;
            }
        }
        return frame >= 0x50;
    }
    return 0;
}

INCLUDE_RODATA("asm/us/fx_130/nonmatchings/fx_130", fx_130_headerPtr);
