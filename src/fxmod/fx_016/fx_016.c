#include "fxmod/fx_016.h"

s32 fx_016_op80(s32 mode, Fx016Work80 *work, Unk800F78A4 *env, Unk80100C78Entry *entry, s32 *args) {
    BtlfxExports *ex = BTLFX_EXPORTS(env);
    SVECTOR rot;
    VECTOR pos;
    VECTOR scale;
    Pool6CEntry *emitter;
    s32 frame;
    s32 radius;
    s32 t;
    s32 morph;
    s32 size;
    s32 fadeAng;
    s32 fullSize;

    if (mode == 0) {
        *args = sizeof(Fx016Work80);
    } else if (mode == 1) {
        work->hold0 = 0;
        fx_016_env = env;
        ex->btlfx_getUnitBodyPos(0, 0, (s16 *)&work->pos4);
        ex->btlfx_getUnitPos(0, (s16 *)&work->posC);
        ex->btlfx_initEffModels(fx_016_effModels, FX_WATER_MODEL_COUNT);
        work->model14 = ex->Hi_RegisterTexEffModel((BtlModelHeader *)ex->btlfx_getFileData(entry->file, FX_WATER_GLOBE_0), FX_WATER_GLOBE_TPAGE, FX_WATER_GLOBE_CLUT, 0, FX_WATER_GLOBE_V);
        work->model18 = ex->Hi_RegisterTexEffModel((BtlModelHeader *)ex->btlfx_getFileData(entry->file, FX_WATER_GLOBE_1), FX_WATER_GLOBE_TPAGE, FX_WATER_GLOBE_CLUT, 0, FX_WATER_GLOBE_V);
        work->model1C = ex->Hi_RegisterTexEffModel((BtlModelHeader *)ex->btlfx_getFileData(entry->file, FX_WATER_GLOBE_2), FX_WATER_GLOBE_TPAGE, FX_WATER_GLOBE_CLUT, 0, FX_WATER_GLOBE_V);
        work->model20 = ex->Hi_RegisterTexEffModel((BtlModelHeader *)ex->btlfx_getFileData(entry->file, FX_WATER_GLOBE_3), FX_WATER_GLOBE_TPAGE, FX_WATER_GLOBE_CLUT, 0, FX_WATER_GLOBE_V);
        work->model24 = ex->Hi_RegisterTexEffModel((BtlModelHeader *)ex->btlfx_getFileData(entry->file, FX_WATER_GLOBE), FX_WATER_GLOBE_TPAGE, FX_WATER_GLOBE_CLUT, 0, FX_WATER_GLOBE_V);
        work->model28 = ex->Hi_RegisterTexEffModel((BtlModelHeader *)ex->btlfx_getFileData(entry->file, FX_WATER_SPLASH_COLUMN), FX_WATER_SPLASH_TPAGE, FX_WATER_SPLASH_CLUT, 0, FX_WATER_SPLASH_V);
        work->model2C = ex->Hi_RegisterTexEffModel((BtlModelHeader *)ex->btlfx_getFileData(entry->file, FX_WATER_SPLASH_FUNNEL), FX_WATER_SPLASH_TPAGE, FX_WATER_SPLASH_CLUT, 0, FX_WATER_SPLASH_V);
        work->model30 = ex->Hi_RegisterTexEffModel((BtlModelHeader *)ex->btlfx_getFileData(entry->file, FX_WATER_BASE_DISC), FX_WATER_GLOBE_TPAGE, FX_WATER_GLOBE_CLUT, 0, FX_WATER_GLOBE_V);
        radius = (ex->btlfx_getUnitRadius(0) << 12) / 720;
        if (radius < 0x400) {
            radius = 0x6A4;
        }
        if (radius > 0x1800) {
            radius = 0x1800;
        }
        fx_016_radiusScale = radius;
        work->scale34 = radius;
    } else {
        frame = entry->frame;
        if (work->hold0 == 0) {
            if (frame < FX_WATER_GROW_FRAMES) {
                t = (frame << 12) / FX_WATER_GROW_FRAMES;
                pos.vx = work->posC.vx;
                pos.vy = work->posC.vy;
                pos.vz = work->posC.vz;
                size = (((ex->math_sin(t >> 2) / 10) + 0x199) * work->scale34) >> 12;
                rot.vz = 0;
                rot.vx = 0;
                morph = (0x1000 - t) * 4;
                rot.vy = -frame * 0x20;
                scale.vy = (size * 3) >> 2;
                scale.vz = size;
                scale.vx = size;
                if (morph > 0x3000) {
                    ex->Hi_DrawMorphEffModel(&rot, &pos, (s32)&scale, work->model14, work->model18, morph - 0x3000);
                } else if (morph > 0x2000) {
                    ex->Hi_DrawMorphEffModel(&rot, &pos, (s32)&scale, work->model18, work->model1C, morph - 0x2000);
                } else if (morph > 0x1000) {
                    ex->Hi_DrawMorphEffModel(&rot, &pos, (s32)&scale, work->model1C, work->model20, morph - 0x1000);
                } else {
                    ex->Hi_DrawMorphEffModel(&rot, &pos, (s32)&scale, work->model20, work->model24, morph);
                }
                ex->Hi_DrawEffModel(&rot, &pos, (s32)&scale, work->model30);
                ex->Hi_ModifyEffModelRGB(work->model30, (ex->math_sin(t >> 2) >> 5) - 0x80,
                                         (ex->math_sin(t >> 2) >> 5) - 0x80, (ex->math_sin(t >> 2) >> 5) - 0x80);
            } else {
                /* morph (not fadeAng) takes the division: the copy fadeAng = morph >> 2 into a block-local pseudo (s2) gives morph a
                 * global-alloc preference for s2, which it also gets in the frame < FX_WATER_GROW_FRAMES branch. The scale value
                 * here is its own block-local variable fullSize. */
                morph = ((frame - FX_WATER_GROW_FRAMES) << 12) / FX_WATER_BURST_FRAMES;
                pos.vx = work->posC.vx;
                pos.vy = work->posC.vy;
                pos.vz = work->posC.vz;
                fullSize = (work->scale34 * 0x332) >> 12;
                rot.vz = 0;
                rot.vx = 0;
                rot.vy = -frame * 0x20;
                scale.vz = fullSize;
                scale.vx = fullSize;
                scale.vy = (fullSize * 3) >> 2;
                ex->Hi_DrawEffModel(&rot, &pos, (s32)&scale, work->model24);
                fadeAng = morph >> 2;
                ex->Hi_ModifyEffModelRGB(work->model24, (ex->math_cos(fadeAng) >> 5) - 0x80,
                                         (ex->math_cos(fadeAng) >> 5) - 0x80, (ex->math_cos(fadeAng) >> 5) - 0x80);
                ex->Hi_DrawEffModel(&rot, &pos, (s32)&scale, work->model30);
                ex->Hi_ModifyEffModelRGB(work->model30, (ex->math_cos(fadeAng) >> 5) - 0x80,
                                         (ex->math_cos(fadeAng) >> 5) - 0x80, (ex->math_cos(fadeAng) >> 5) - 0x80);
            }
            ex->btlfx_drawScrollMove(frame, FX_WATER_GLOBE_VRAM_X, FX_WATER_GLOBE_VRAM_Y, FX_WATER_GLOBE_VRAM_W, FX_WATER_GLOBE_VRAM_H,
                                      fx_016_scrollDst[frame % FX_WATER_GLOBE_ANIM_FRAMES + 1].x0,
                                      fx_016_scrollDst[frame % FX_WATER_GLOBE_ANIM_FRAMES + 1].y2);
            if (frame >= FX_WATER_GROW_FRAMES) {
                pos.vx = work->posC.vx;
                pos.vy = work->posC.vy;
                pos.vz = work->posC.vz;
                scale.vx = scale.vz = (work->scale34 << 11) >> 12;
                t = ((frame - FX_WATER_GROW_FRAMES) << 12) / FX_WATER_BURST_FRAMES;
                scale.vy = (((ex->math_sin(t >> 2) / 4) + 0x2AA) * work->scale34) >> 12;
                rot.vz = 0;
                rot.vx = 0;
                rot.vy = -frame * 0x40;
                ex->Hi_DrawMorphEffModel(&rot, &pos, (s32)&scale, work->model28, work->model2C, 0x1000 - t);
                ex->Hi_ModifyEffModelRGB(work->model28, (ex->math_sin(t >> 1) >> 5) - 0x80,
                                         (ex->math_sin(t >> 1) >> 5) - 0x80, (ex->math_sin(t >> 1) >> 5) - 0x80);
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

s32 fx_016_op81(s32 mode, Fx016Work81 *work, Unk800F78A4 *env, Unk80100C78Entry *entry, s32 *args) {
    BtlfxExports *ex = BTLFX_EXPORTS(env);
    s32 frame;
    s32 i;
    s32 t;
    SVECTOR pos;

    if (mode == 0) {
        *args = sizeof(Fx016Work81);
    } else if (mode == 1) {
        fx_016_env = env;
        work->count40 = env->targets->targetCount;
        for (i = 0; i < work->count40; i++) {
            ex->btlfx_getUnitHomePos(i, (s16 *)&work->home0[i]);
            work->jump44 = ((-0x140 - (ex->btlfx_rand() & 0x1F)) * fx_016_radiusScale) >> 12;
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

INCLUDE_RODATA("asm/us/fx_016/nonmatchings/fx_016", fx_016_headerPtr);
