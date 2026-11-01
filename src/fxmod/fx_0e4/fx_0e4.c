#include "fxmod/fx_0e4.h"

s32 fx_0e4_op80(s32 mode, Fx0e4Work *work, Unk800F78A4 *env, Unk80100C78Entry *entry, s32 *args) {
    BtlfxExports *ex = BTLFX_EXPORTS(env);
    SVECTOR pos;
    s32 frame;
    s32 i;
    s32 model;
    s32 t;
    s16 ringScale;
    s32 tick;
    s32 morph;

    if (mode == 0) {
        *args = 0x70;
    } else if (mode == 1) {
        fx_0e4_env = env;
        ex->btlfx_initEffModels(fx_0e4_effModels, 3);
        ex->btlfx_getUnitPos(0, (s16 *)&work->pos34);
        ex->btlfx_getUnitRot(0, (s16 *)&work->rot3C);
        work->rot8.vx = work->rot3C.vx;
        work->rot8.vy = work->rot3C.vy;
        work->rot8.vz = work->rot3C.vz;
        work->pos10.vx = work->pos34.vx;
        work->pos10.vy = work->pos34.vy;
        work->pos10.vz = work->pos34.vz;
        ex->btlfx_pointAhead((s16 *)&work->pos34, (s16 *)&work->rot3C, ex->btlfx_getUnitRadius(0) + 0x190, (s16 *)&pos);
        work->pos10.vx = pos.vx;
        work->pos10.vy = pos.vy;
        work->pos10.vz = pos.vz;
        work->state64 = 0;
        work->scale20.vx = 0x5000;
        work->scale20.vy = 0x6000;
        work->scale20.vz = 0x5000;
        for (i = 0; i < 3; i++) {
            work->model68[i] = ex->btlfx_registerEffectModel((Unk800E8F50 *)ex->btlfx_getFileData(entry->file, fx_0e4_modelBlocks[i]), 1);
        }
        work->ringRot54.vx = 0x400;
        work->ringAlpha5E = 0x1000;
        work->dropTimer60 = 8;
        work->ringRot54.vy = 0;
        work->ringRot54.vz = 0;
        work->ringScale5C = 0;
        work->fadeTimer62 = -1;
        work->ringPos4C.vx = work->pos10.vx;
        work->ringPos4C.vy = work->pos10.vy;
        work->ringPos4C.vz = work->pos10.vz;
    } else {
        frame = entry->frame;
        if (frame == 0xC) {
            work->fadeTimer62 = 0x10;
        }
        if (frame >= 0 && frame < 0x1C) {
            tick = frame;
            if (work->dropTimer60 >= 0) {
                work->pos10.vy = fxm_lerpRatio(-0xFA0, 0, 8 - work->dropTimer60, 8);
                if (--work->dropTimer60 == 0) {
                    work->pos44.vx = work->pos10.vx;
                    work->pos44.vy = work->pos10.vy;
                    work->pos44.vz = work->pos10.vz;
                    work->pos44.vy = 0;
                    work->emit0 = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, 7), (s32)&work->pos44);
                    work->emit4 = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, 5), (s32)&work->pos34);
                }
            }
            work->ringScale5C += 0x200;
            ringScale = work->ringScale5C;
            if (ringScale > 0x1000) {
                ringScale = 0x1000;
            }
            work->ringScale5C = ringScale;
            ex->btlfx_drawRings((u8 *)ex->btlfx_getFileData(entry->file, 6), &work->ringPos4C, &work->ringRot54, work->ringScale5C,
                              tick, 2, work->ringAlpha5E);
            switch (work->state64) {
            case 0:
                if (tick < 6) {
                    model = 0;
                    ex->Hi_DrawEffModel(&work->rot8, &work->pos10, (s32)&work->scale20, model);
                } else if (tick >= 6 && tick < 9) {
                    morph = 0x1000 - ((tick - 6) << 12) / 3;
                    t = morph > 0x1000 ? 0x1000 : morph;
                    ex->Hi_DrawMorphEffModel(&work->rot8, &work->pos10, (s32)&work->scale20, work->model68[0],
                                             work->model68[1], t);
                } else {
                    model = 1;
                    ex->Hi_SetEffModelSlice(model, work->pos10.vy);
                    ex->Hi_DrawSliceEffModel(&work->rot8, &work->pos10, (s32)&work->scale20, model);
                }
                env->camera->points[0].vx = work->pos10.vx;
                env->camera->points[0].vy = work->pos10.vy;
                env->camera->points[0].vz = work->pos10.vz;
                if (tick >= 4 && frame < 0xC) {
                    work->scale20.vy -= 0x400;
                }
                break;
            case 1:
                model = 2;
                ex->Hi_SetEffModelSlice(model, work->pos10.vy);
                ex->Hi_DrawSliceEffModel(&work->rot8, &work->pos10, (s32)&work->scale20, model);
                break;
            }
            if (work->fadeTimer62 >= 0) {
                work->rgb30 = fxm_lerpRatio(0, -0x80, 0x10 - work->fadeTimer62, 0x10);
                work->fadeTimer62--;
                ex->Hi_ModifyEffModelAbr(model, 1);
                ex->Hi_ModifyEffModelRGB(model, work->rgb30, work->rgb30, work->rgb30);
                work->pos10.vy += 0xA;
                work->scale20.vy -= 0x100;
                work->ringAlpha5E -= 0x100;
            }
        }
        if (frame >= 0x41) {
            ex->btlfx_clearEffModels();
            return 1;
        }
    }
    return 0;
}

static void fxm_lerpSvec(SVECTOR *from, SVECTOR *to, s32 t, SVECTOR *out) {
    BTLFX_EXPORTS(fx_0e4_env)->LoadAverageShort12(from, to, 0x1000 - t, t, out);
}

static s32 fxm_lerpRatio(s32 from, s32 to, s32 t, s32 n) {
    return (from * (n - t) + to * t) / n;
}

INCLUDE_RODATA("asm/us/fx_0e4/nonmatchings/fx_0e4", fx_0e4_headerPtr);
