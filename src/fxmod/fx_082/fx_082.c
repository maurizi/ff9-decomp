#include "fxmod/fx_082.h"

s32 fx_082_op80(s32 mode, Fx082Work80 *work, Unk800F78A4 *env, Unk80100C78Entry *entry, s32 *args) {
    BtlfxExports *ex = BTLFX_EXPORTS(env);
    s32 scale[3];
    s32 mpos[3];
    Pool6CEntry *emitter;
    s32 frame;
    s32 sc;
    s32 morph;
    s32 bright;
    s32 tint;
    s32 t;

    if (mode == 0) {
        *args = sizeof(Fx082Work80);
    } else if (mode == 1) {
        fx_082_env = env;
        ex->btlfx_getUnitBodyPos(0, 1, (s16 *)&work->pos18);
        BTLFX_COPY_SVEC(&work->pos8, &work->pos18);
        ex->btlfx_getUnitRot(0, (s16 *)&work->rot20);
        ex->btlfx_pointAhead((s16 *)&work->pos18, (s16 *)&work->rot20, FX_MAGIC_HAMMER_REACH, (s16 *)&work->pos18);
        work->state0 = FX_MAGIC_HAMMER_STATE_GROW;
        work->unk4 = 0;
        work->pos18.vy -= FX_MAGIC_HAMMER_HEIGHT;
        BTLFX_COPY_SVEC(&work->rot28, &work->rot20);
        ex->btlfx_initEffModels(fx_082_effModels, FX_MAGIC_HAMMER_MODEL_COUNT);
        work->model30 = ex->btlfx_registerEffectModel((Unk800E8F50 *)ex->btlfx_getFileData(entry->file, FX_MAGIC_HAMMER_HAMMER), 0);
        work->model34 = ex->btlfx_registerEffectModel((Unk800E8F50 *)ex->btlfx_getFileData(entry->file, FX_MAGIC_HAMMER_HAMMER), 0);
        env->camera->points[0].vx = work->pos18.vx;
        env->camera->points[0].vy = work->pos18.vy;
        env->camera->points[0].vz = work->pos18.vz;
    } else {
        bright = morph = sc = 0;
        frame = entry->frame;
        switch (work->state0) {
        case FX_MAGIC_HAMMER_STATE_GROW:
            t = (frame << 12) / FX_MAGIC_HAMMER_GROW_FRAMES;
            if (frame == 0) {
                work->pos10.vx = work->pos18.vx;
                work->pos10.vy = work->pos18.vy;
                work->pos10.vz = work->pos18.vz;
                work->pos10.vy -= 0x80;
                if (work->pos18.vz > 0) {
                    work->pos10.vz -= 0x40;
                } else {
                    work->pos10.vz += 0x40;
                }
                emitter = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, FX_MAGIC_HAMMER_SMOKE), (s32)&work->pos10);
                if (emitter != NULL) {
                    emitter->otZ = 0x20;
                }
            }
            work->rot28.vx = -0x200;
            sc = (t * FX_MAGIC_HAMMER_SCALE) >> 12;
            bright = 0x1000;
            if (frame >= FX_MAGIC_HAMMER_GROW_FRAMES) {
                work->state0 = FX_MAGIC_HAMMER_STATE_SWING;
                entry->frame = -1;
            }
            break;
        case FX_MAGIC_HAMMER_STATE_SWING:
            t = frame << 10;
            work->rot28.vx = (((0x1000 - ex->math_cos(t >> 2)) * 0x600) >> 12) - 0x200;
            sc = FX_MAGIC_HAMMER_SCALE;
            bright = 0x1000;
            if (frame == FX_MAGIC_HAMMER_RINGS_FRAME) {
                emitter = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, FX_MAGIC_HAMMER_PASTEL_RINGS), (s32)&work->pos8);
                if (emitter != NULL) {
                    emitter->otZ = 0x20;
                }
            }
            if (frame >= FX_MAGIC_HAMMER_SWING_FRAMES) {
                work->state0 = FX_MAGIC_HAMMER_STATE_HIT;
                entry->frame = -1;
                emitter = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, FX_MAGIC_HAMMER_VIOLET_RING), (s32)&work->pos8);
                if (emitter != NULL) {
                    emitter->otZ = 0x20;
                }
                emitter = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, FX_MAGIC_HAMMER_CYAN_RING), (s32)&work->pos8);
                if (emitter != NULL) {
                    emitter->otZ = 0x20;
                }
            }
            break;
        case FX_MAGIC_HAMMER_STATE_HIT:
            morph = 0x1000;
            sc = FX_MAGIC_HAMMER_SCALE;
            bright = 0x1000;
            t = (frame << 12) / FX_MAGIC_HAMMER_HIT_FRAMES;
            work->rot28.vx = 0x400 - ((frame & 1) * 2 - 1) * (ex->math_cos(t >> 2) >> 7);
            if (frame == 0) {
                emitter = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, FX_MAGIC_HAMMER_RAINBOW_STREAK), (s32)&work->pos8);
                if (emitter != NULL) {
                    emitter->otZ = 0x20;
                }
                emitter = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, FX_MAGIC_HAMMER_STARS), (s32)&work->pos8);
                if (emitter != NULL) {
                    emitter->otZ = 0x20;
                }
            }
            if (frame >= FX_MAGIC_HAMMER_HIT_FRAMES) {
                work->state0 = FX_MAGIC_HAMMER_STATE_FADE;
                entry->frame = -1;
            }
            break;
        case FX_MAGIC_HAMMER_STATE_FADE:
            sc = FX_MAGIC_HAMMER_SCALE;
            t = frame << 9;
            bright = ex->math_cos(t >> 2);
            if (frame >= FX_MAGIC_HAMMER_FADE_FRAMES) {
                ex->btlfx_clearEffModels();
                return 1;
            }
            break;
        }
        if (bright != 0) {
            mpos[0] = work->pos18.vx;
            mpos[1] = work->pos18.vy;
            mpos[2] = work->pos18.vz;
            scale[0] = sc;
            scale[1] = sc;
            scale[2] = sc;
            ex->Hi_DrawMorphEffModel(&work->rot28, (VECTOR *)mpos, (s32)scale, work->model30, work->model34, 0x1000 - morph);
            if (bright < 0x1000) {
                ex->Hi_ModifyEffModelAbr(work->model30, BTLFX_ABR_ADD);
                tint = (bright >> 5) - 0x80;
                ex->Hi_ModifyEffModelRGB(work->model30, tint, tint, tint);
            }
        }
    }
    return 0;
}

s32 fx_082_op81(s32 mode, Fx082Work81 *work, Unk800F78A4 *env, Unk80100C78Entry *entry, s32 *args) {
    BtlfxExports *ex = BTLFX_EXPORTS(env);
    s32 scale[3];
    s32 frame;
    s32 decayAng;
    s32 wobbleAng;
    s32 amp;

    if (mode == 0) {
        *args = sizeof(Fx082Work81);
    } else if (mode != 1) {
        frame = entry->frame;
        scale[0] = work->scale0[0];
        scale[1] = work->scale0[1];
        scale[2] = work->scale0[2];
        decayAng = (frame << 8) >> 2;
        amp = (ex->math_cos(decayAng) << 11) >> 12;
        wobbleAng = frame << 9;
        scale[1] -= (ex->math_sin(wobbleAng) * amp) >> 12;
        amp = (ex->math_cos(decayAng) << 11) >> 12;
        amp = (ex->math_sin(wobbleAng) * amp) >> 12;
        scale[0] += amp;
        scale[2] += amp;
        ex->btlfx_setUnitScale(0, scale);
        if (frame >= FX_MAGIC_HAMMER_SQUASH_FRAMES) {
            ex->btlfx_setUnitScale(0, work->scale0);
            return 1;
        }
    } else {
        fx_082_env = env;
        ex->btlfx_getUnitScale(0, (VECTOR *)work);
    }
    return 0;
}

INCLUDE_RODATA("asm/us/fx_082/nonmatchings/fx_082", fx_082_headerPtr);
