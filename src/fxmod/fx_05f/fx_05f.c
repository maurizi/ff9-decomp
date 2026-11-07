#include "fxmod/fx_05f.h"

s32 fx_05f_op80(s32 mode, Fx05fWork80 *work, Unk800F78A4 *env, Unk80100C78Entry *entry, s32 *args) {
    BtlfxExports *ex = BTLFX_EXPORTS(env);
    SVECTOR rot;
    VECTOR pos;
    VECTOR scale;
    Pool6CEntry *emitter;
    s32 frame;
    s32 t;
    s32 morph;
    s32 size;
    s32 fadeAng;

    if (mode == 0) {
        *args = sizeof(Fx05fWork80);
    } else if (mode == 1) {
        work->hold0 = 0;
        work->posC.vx = env->targets->centerX;
        work->posC.vy = env->targets->centerY;
        work->posC.vz = env->targets->centerZ;
        fx_05f_env = env;
        work->posC.vy = 0;
        BTLFX_COPY_SVEC(&work->pos4, &work->posC);
        work->pos4.vy -= FX_WATER_EMIT_HEIGHT;
        ex->btlfx_initEffModels(fx_05f_effModels, FX_WATER_MODEL_COUNT);
        work->model14 = ex->Hi_RegisterTexEffModel((BtlModelHeader *)ex->btlfx_getFileData(entry->file, FX_WATER_GLOBE_0), FX_WATER_GLOBE_TPAGE, FX_WATER_GLOBE_CLUT, 0, FX_WATER_GLOBE_V);
        work->model18 = ex->Hi_RegisterTexEffModel((BtlModelHeader *)ex->btlfx_getFileData(entry->file, FX_WATER_GLOBE_1), FX_WATER_GLOBE_TPAGE, FX_WATER_GLOBE_CLUT, 0, FX_WATER_GLOBE_V);
        work->model1C = ex->Hi_RegisterTexEffModel((BtlModelHeader *)ex->btlfx_getFileData(entry->file, FX_WATER_GLOBE_2), FX_WATER_GLOBE_TPAGE, FX_WATER_GLOBE_CLUT, 0, FX_WATER_GLOBE_V);
        work->model20 = ex->Hi_RegisterTexEffModel((BtlModelHeader *)ex->btlfx_getFileData(entry->file, FX_WATER_GLOBE_3), FX_WATER_GLOBE_TPAGE, FX_WATER_GLOBE_CLUT, 0, FX_WATER_GLOBE_V);
        work->model24 = ex->Hi_RegisterTexEffModel((BtlModelHeader *)ex->btlfx_getFileData(entry->file, FX_WATER_GLOBE), FX_WATER_GLOBE_TPAGE, FX_WATER_GLOBE_CLUT, 0, FX_WATER_GLOBE_V);
        work->model28 = ex->Hi_RegisterTexEffModel((BtlModelHeader *)ex->btlfx_getFileData(entry->file, FX_WATER_SPLASH_COLUMN), FX_WATER_SPLASH_TPAGE, FX_WATER_SPLASH_CLUT, 0, FX_WATER_SPLASH_V);
        work->model2C = ex->Hi_RegisterTexEffModel((BtlModelHeader *)ex->btlfx_getFileData(entry->file, FX_WATER_SPLASH_FUNNEL), FX_WATER_SPLASH_TPAGE, FX_WATER_SPLASH_CLUT, 0, FX_WATER_SPLASH_V);
        work->model30 = ex->Hi_RegisterTexEffModel((BtlModelHeader *)ex->btlfx_getFileData(entry->file, FX_WATER_BASE_DISC), FX_WATER_GLOBE_TPAGE, FX_WATER_GLOBE_CLUT, 0, FX_WATER_GLOBE_V);
    } else {
        frame = entry->frame;
        if (work->hold0 == 0) {
            if (frame < FX_WATER_GROW_FRAMES) {
                t = (frame << 12) / FX_WATER_GROW_FRAMES;
                pos.vx = work->posC.vx;
                pos.vy = work->posC.vy;
                pos.vz = work->posC.vz;
                size = ex->math_sin(t >> 2) / 10 + 0x333;
                morph = (0x1000 - t) * 4;
                rot.vz = 0;
                rot.vx = 0;
                rot.vy = -frame * 0x20;
                scale.vz = size;
                scale.vx = size;
                scale.vy = size / 2;
                if (morph > 0x3000) {
                    ex->Hi_DrawMorphEffModel(&rot, &pos, (s32)&scale, work->model14, work->model18, morph - 0x3000);
                } else if (morph > 0x2000) {
                    ex->Hi_DrawMorphEffModel(&rot, &pos, (s32)&scale, work->model18, work->model1C, morph - 0x2000);
                } else if (morph > 0x1000) {
                    ex->Hi_DrawMorphEffModel(&rot, &pos, (s32)&scale, work->model1C, work->model20, morph - 0x1000);
                } else {
                    ex->Hi_DrawMorphEffModel(&rot, &pos, (s32)&scale, work->model20, work->model24, morph);
                }
                scale.vx = scale.vz = (scale.vx * 5) >> 2;
                ex->Hi_DrawEffModel(&rot, &pos, (s32)&scale, work->model30);
                ex->Hi_ModifyEffModelRGB(work->model30, (ex->math_sin(t >> 2) >> 5) - 0x80, (ex->math_sin(t >> 2) >> 5) - 0x80,
                                         (ex->math_sin(t >> 2) >> 5) - 0x80);
                /* Dead test, removed after reload: it keeps morph live (and call-crossing) through this branch, so
                 * global-alloc gives it s2 and the else branch's division temporary lands in t0. */
                if (morph != 0) {
                    size = 0;
                }
                scale.vx = scale.vz = (scale.vx * 4) / 5;
            } else {
                morph = ((frame - FX_WATER_GROW_FRAMES) << 12) / FX_WATER_BURST_FRAMES;
                fadeAng = morph >> 2;
                pos.vx = work->posC.vx;
                pos.vy = work->posC.vy;
                pos.vz = work->posC.vz;
                scale.vx = scale.vz = 0x4CC;
                scale.vy = 0x266;
                rot.vz = 0;
                rot.vx = 0;
                rot.vy = -frame * 0x20;
                ex->Hi_DrawEffModel(&rot, &pos, (s32)&scale, work->model24);
                ex->Hi_ModifyEffModelRGB(work->model24, (ex->math_cos(fadeAng) >> 5) - 0x80, (ex->math_cos(fadeAng) >> 5) - 0x80,
                                         (ex->math_cos(fadeAng) >> 5) - 0x80);
                scale.vx = scale.vz = (scale.vx * 5) >> 2;
                ex->Hi_DrawEffModel(&rot, &pos, (s32)&scale, work->model30);
                ex->Hi_ModifyEffModelRGB(work->model30, (ex->math_cos(fadeAng) >> 5) - 0x80, (ex->math_cos(fadeAng) >> 5) - 0x80,
                                         (ex->math_cos(fadeAng) >> 5) - 0x80);
                scale.vx = scale.vz = (scale.vx * 4) / 5;
            }
            ex->btlfx_drawScrollMove(frame, FX_WATER_GLOBE_VRAM_X, FX_WATER_GLOBE_VRAM_Y, FX_WATER_GLOBE_VRAM_W, FX_WATER_GLOBE_VRAM_H,
                                      fx_05f_scrollDst[frame % FX_WATER_GLOBE_ANIM_FRAMES + 1].x0,
                                      fx_05f_scrollDst[frame % FX_WATER_GLOBE_ANIM_FRAMES + 1].y2);
            if (frame >= FX_WATER_GROW_FRAMES) {
                pos.vx = work->posC.vx;
                pos.vy = work->posC.vy;
                pos.vz = work->posC.vz;
                t = ((frame - FX_WATER_GROW_FRAMES) << 12) / FX_WATER_BURST_FRAMES;
                scale.vx = scale.vz = 0xE00;
                scale.vy = ex->math_sin(t >> 2) / 4 + 0x400;
                rot.vy = -frame * 0x40;
                rot.vz = 0;
                rot.vx = 0;
                ex->Hi_DrawMorphEffModel(&rot, &pos, (s32)&scale, work->model28, work->model2C, 0x1000 - t);
                /* Scheduling barrier: the morph call's arguments are scheduled as at a block end (&scale last,
                 * in the call's delay slot). */
                do {
                } while (0);
                t >>= 1;
                ex->Hi_ModifyEffModelRGB(work->model28, (ex->math_sin(t) >> 5) - 0x80, (ex->math_sin(t) >> 5) - 0x80,
                                         (ex->math_sin(t) >> 5) - 0x80);
            }
            if (frame == FX_WATER_SPLASH_DROPS_FRAME) {
                emitter = (Pool6CEntry *)ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, FX_WATER_SPLASH_DROPS), (s32)&work->pos4);
                if (emitter != NULL) {
                    emitter->otZ = ex->btlfx_addRadiusSixth(0, 0x20);
                }
            }
            if (frame == FX_WATER_BLUE_RING_FRAME) {
                emitter = (Pool6CEntry *)ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, FX_WATER_BLUE_RING), (s32)&work->pos4);
                if (emitter != NULL) {
                    emitter->otZ = ex->btlfx_addRadiusSixth(0, 0x40);
                }
            }
            if (frame >= FX_WATER_GROW_FRAMES + FX_WATER_BURST_FRAMES) {
                ex->btlfx_clearEffModels();
                return 1;
            }
        }
    }
    return 0;
}

s32 fx_05f_op81(s32 mode, Fx05fWork81 *work, Unk800F78A4 *env, Unk80100C78Entry *entry, s32 *args) {
    BtlfxExports *ex = BTLFX_EXPORTS(env);
    s32 frame;
    s32 i;
    s32 t;
    SVECTOR pos;

    if (mode == 0) {
        *args = sizeof(Fx05fWork81);
    } else if (mode == 1) {
        fx_05f_env = env;
        work->count40 = env->targets->targetCount;
        for (i = 0; i < work->count40; i++) {
            ex->btlfx_getUnitHomePos(i, (s16 *)&work->home0[i]);
            work->jump44 = -0x190 - (ex->btlfx_rand() & 0x7F);
            ex->btlfx_getUnitBonePos(i, 0, (u16 *)&pos);
            work->height48[i] = -pos.vy;
        }
    } else {
        frame = entry->frame;
        if (frame < FX_WATER_LIFT_FRAMES) {
            t = frame << 7;
            for (i = 0; i < work->count40; i++) {
                BTLFX_COPY_SVEC(&pos, &work->home0[i]);
                pos.vy += (work->jump44 * ex->math_sin(t >> 2)) >> 12;
                ex->btlfx_setUnitPos(i, (s16 *)&pos);
            }
        } else {
            t = (frame - FX_WATER_LIFT_FRAMES) << 10;
            for (i = 0; i < work->count40; i++) {
                BTLFX_COPY_SVEC(&pos, &work->home0[i]);
                pos.vy += (work->height48[i] * t) >> 12;
                pos.vy += (work->jump44 * ex->math_cos(t >> 2)) >> 12;
                ex->btlfx_setUnitPos(i, (s16 *)&pos);
            }
        }
        if (frame >= FX_WATER_LIFT_FRAMES + FX_WATER_DROP_FRAMES) {
            for (i = 0; i < work->count40; i++) {
                ex->btlfx_getUnitHomePos(i, (s16 *)&pos);
                ex->btlfx_setUnitPos(i, (s16 *)&pos);
            }
            return 1;
        }
    }
    return 0;
}

INCLUDE_RODATA("asm/us/fx_05f/nonmatchings/fx_05f", fx_05f_headerPtr);
