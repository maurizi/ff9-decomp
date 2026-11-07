#include "fxmod/fx_0ff.h"

s32 fx_0ff_op80(s32 mode, Fx0ffWork *work, Unk800F78A4 *env, Unk80100C78Entry *entry, s32 *args) {
    BtlfxExports *ex = BTLFX_EXPORTS(env);
    SVECTOR rot;
    SVECTOR pos;
    s32 frame;
    s32 i;
    s32 j;
    s32 radius;
    s32 model;
    Fx0ffOrbit *orbit;
    Fx0ffOrbit *initOrbit;
    Fx0ffOrbit *unused;

    if (mode == 0) {
        *args = sizeof(Fx0ffWork);
    } else if (mode == 1) {
        fx_0ff_env = env;
        ex->btlfx_initEffModels(fx_0ff_effModels, FX_COOK_MODEL_COUNT);
        ex->btlfx_getUnitBodyPos(0, 0, (s16 *)&work->posAC);
        ex->btlfx_getUnitPos(0, (s16 *)&work->posB4);
        if (work->posAC.vy > -FX_COOK_MIN_HEIGHT) {
            work->posAC.vy = -FX_COOK_MIN_HEIGHT;
        }
        work->scale98.vx = 0x8000;
        work->scale98.vy = 0x8000;
        work->scale98.vz = 0x8000;
        radius = ex->btlfx_getUnitRadius(BTLFX_UNIT_MODEL_RADIUS | 0);
        radius = (radius > FX_COOK_MAX_RADIUS) ? FX_COOK_MAX_RADIUS : radius;
        initOrbit = fx_0ff_orbits;
        for (i = 0; i < FX_COOK_ORBIT_COUNT; i++, initOrbit++) {
            initOrbit->radius0 = radius;
            initOrbit->angle2 = i * (0x1000 / FX_COOK_ORBIT_COUNT);
            initOrbit->spin4 = i * (0x1000 / FX_COOK_ORBIT_COUNT);
            (work->rot8 + i)->vx = 0;
            (work->rot8 + i)->vy = 0;
            (work->rot8 + i)->vz = 0;
        }
        for (i = 0; i < FX_COOK_MODEL_COUNT; i++) {
            work->modelC4[i] = ex->btlfx_registerEffectModel((Unk800E8F50 *)ex->btlfx_getFileData(entry->file, fx_0ff_modelBlocks[i]), 1);
        }
        work->fadeInTimerBC = -1;
        work->fadeOutTimerC0 = -1;
    } else {
        frame = entry->frame;
        switch (frame) {
        case FX_COOK_FLAMES_FRAME:
            work->emit0 = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, FX_COOK_FLAMES), (s32)&work->posB4);
            if (work->emit0 != NULL) {
                work->emit0->otZ = ex->btlfx_addRadiusSixth(0x10, 0x80);
            }
            break;
        case FX_COOK_WHITE_RINGS_FRAME:
            work->emit4 = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, FX_COOK_WHITE_RINGS), (s32)&work->posAC);
            if (work->emit4 != NULL) {
                work->emit4->otZ = ex->btlfx_addRadiusSixth(0x10, 0x80);
            }
            break;
        case 0:
            work->fadeInTimerBC = FX_COOK_FADE_IN_FRAMES;
            break;
        case FX_COOK_FADE_OUT_FRAME:
            work->fadeOutTimerC0 = FX_COOK_FADE_OUT_FRAMES;
            break;
        }
        if (frame >= 0 && frame < FX_COOK_ORBIT_FRAMES) {
            if (work->fadeInTimerBC >= 0 || work->fadeOutTimerC0 >= 0) {
                if (work->fadeInTimerBC >= 0) {
                    work->rgbA8 = fxm_lerpRatio(-0x80, 0, FX_COOK_FADE_IN_FRAMES - work->fadeInTimerBC, FX_COOK_FADE_IN_FRAMES);
                    work->fadeInTimerBC--;
                }
                if (work->fadeOutTimerC0 >= 0) {
                    work->rgbA8 = fxm_lerpRatio(0, -0x80, FX_COOK_FADE_OUT_FRAMES - work->fadeOutTimerC0, FX_COOK_FADE_OUT_FRAMES);
                    work->fadeOutTimerC0--;
                }
            }
            orbit = fx_0ff_orbits;
            model = work->modelC4[0];
            for (j = 0; j < FX_COOK_ORBIT_COUNT; j++, orbit++) {
                rot.vx = 0;
                rot.vy = orbit->angle2;
                rot.vz = 0;
                ex->btlfx_pointAhead((s16 *)&work->posAC, (s16 *)&rot, orbit->radius0, (s16 *)&pos);
                (work->pos38 + j)->vx = pos.vx;
                (work->pos38 + j)->vy = pos.vy;
                (work->pos38 + j)->vz = pos.vz;
                ex->Hi_DrawEffModel(&work->rot8[j], &work->pos38[j], (s32)&work->scale98, model);
                if (work->fadeInTimerBC >= 0 || work->fadeOutTimerC0 >= 0) {
                    ex->Hi_ModifyEffModelAbr(model, BTLFX_ABR_ADD);
                    ex->Hi_ModifyEffModelRGB(model, work->rgbA8, work->rgbA8, work->rgbA8);
                }
                model++;
                if (model > work->modelC4[FX_COOK_MODEL_COUNT - 1]) {
                    model = work->modelC4[0];
                }
                orbit->angle2 += 0x55;
                orbit->spin4 += 0x200;
                work->rot8[j].vy += 0x80;
            }
        }
        if (frame >= FX_COOK_FRAMES) {
            ex->btlfx_clearEffModels();
            return 1;
        }
    }
    return 0;
}

static void fxm_lerpSvec(SVECTOR *from, SVECTOR *to, s32 t, SVECTOR *out) {
    BTLFX_EXPORTS(fx_0ff_env)->LoadAverageShort12(from, to, 0x1000 - t, t, out);
}

static s32 fxm_lerpRatio(s32 from, s32 to, s32 t, s32 n) {
    return (from * (n - t) + to * t) / n;
}

INCLUDE_RODATA("asm/us/fx_0ff/nonmatchings/fx_0ff", fx_0ff_headerPtr);
