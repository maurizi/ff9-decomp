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
        *args = 0x38;
    } else if (mode == 1) {
        fx_082_env = env;
        ex->btlfx_getUnitBodyPos(0, 1, (s16 *)&work->pos18);
        BTLFX_COPY_SVEC(&work->pos8, &work->pos18);
        ex->btlfx_getUnitRot(0, (s16 *)&work->rot20);
        ex->btlfx_pointAhead((s16 *)&work->pos18, (s16 *)&work->rot20, 0x226, (s16 *)&work->pos18);
        work->state0 = 0;
        work->unk4 = 0;
        work->pos18.vy -= 0x145;
        BTLFX_COPY_SVEC(&work->rot28, &work->rot20);
        ex->btlfx_initEffModels(fx_082_effModels, 2);
        work->model30 = ex->btlfx_registerEffectModel((Unk800E8F50 *)ex->btlfx_getFileData(entry->file, FX_MAGIC_HAMMER_MODEL), 0);
        work->model34 = ex->btlfx_registerEffectModel((Unk800E8F50 *)ex->btlfx_getFileData(entry->file, FX_MAGIC_HAMMER_MODEL), 0);
        env->camera->points[0].vx = work->pos18.vx;
        env->camera->points[0].vy = work->pos18.vy;
        env->camera->points[0].vz = work->pos18.vz;
    } else {
        bright = morph = sc = 0;
        frame = entry->frame;
        switch (work->state0) {
        case 0:
            t = (frame << 12) / 7;
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
            sc = (t * 0xC00) >> 12;
            bright = 0x1000;
            if (frame >= 7) {
                work->state0 = 1;
                entry->frame = -1;
            }
            break;
        case 1:
            t = frame << 10;
            work->rot28.vx = (((0x1000 - ex->math_cos(t >> 2)) * 0x600) >> 12) - 0x200;
            sc = 0xC00;
            bright = 0x1000;
            if (frame == 3) {
                emitter = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, FX_MAGIC_HAMMER_PASTEL_RINGS), (s32)&work->pos8);
                if (emitter != NULL) {
                    emitter->otZ = 0x20;
                }
            }
            if (frame >= 4) {
                work->state0 = 2;
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
        case 2:
            morph = 0x1000;
            sc = 0xC00;
            bright = 0x1000;
            t = (frame << 12) / 6;
            work->rot28.vx = 0x400 - ((frame & 1) * 2 - 1) * (ex->math_cos(t >> 2) >> 7);
            if (frame == 0) {
                emitter = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, FX_MAGIC_HAMMER_FAN_EMITTER), (s32)&work->pos8);
                if (emitter != NULL) {
                    emitter->otZ = 0x20;
                }
                emitter = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, FX_MAGIC_HAMMER_STARS), (s32)&work->pos8);
                if (emitter != NULL) {
                    emitter->otZ = 0x20;
                }
            }
            if (frame >= 6) {
                work->state0 = 3;
                entry->frame = -1;
            }
            break;
        case 3:
            sc = 0xC00;
            t = frame << 9;
            bright = ex->math_cos(t >> 2);
            if (frame >= 8) {
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
                ex->Hi_ModifyEffModelAbr(work->model30, 1);
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
        *args = 0xC;
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
        if (frame >= 0x10) {
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
