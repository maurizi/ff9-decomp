#include "fxmod/fx_0fe.h"

s32 fx_0fe_op80(s32 mode, Fx0feWork *work, Unk800F78A4 *env, Unk80100C78Entry *entry, s32 *args) {
    BtlfxExports *ex = BTLFX_EXPORTS(env);
    SVECTOR rot;
    SVECTOR pos;
    s32 frame;
    s32 i;
    s32 j;
    s32 radius;
    s32 model;
    Fx0feOrbit *orbit;
    Fx0feOrbit *initOrbit;
    Fx0feOrbit *unused;

    if (mode == 0) {
        *args = sizeof(Fx0feWork);
    } else if (mode == 1) {
        fx_0fe_env = env;
        ex->btlfx_initEffModels(fx_0fe_effModels, FX_EAT_MODEL_COUNT);
        ex->btlfx_getUnitBodyPos(0, 0, (s16 *)&work->posA8);
        if (work->posA8.vy > -FX_EAT_MIN_HEIGHT) {
            work->posA8.vy = -FX_EAT_MIN_HEIGHT;
        }
        work->scale94.vx = 0x8000;
        work->scale94.vy = 0x8000;
        work->scale94.vz = 0x8000;
        radius = ex->btlfx_getUnitRadius(BTLFX_UNIT_MODEL_RADIUS | 0);
        radius = (radius > FX_EAT_MAX_RADIUS) ? FX_EAT_MAX_RADIUS : radius;
        initOrbit = fx_0fe_orbits;
        for (i = 0; i < FX_EAT_ORBIT_COUNT; i++, initOrbit++) {
            initOrbit->radius0 = radius;
            initOrbit->angle2 = i * (0x1000 / FX_EAT_ORBIT_COUNT);
            initOrbit->spin4 = i * (0x1000 / FX_EAT_ORBIT_COUNT);
            (work->rot4 + i)->vx = 0;
            (work->rot4 + i)->vy = 0;
            (work->rot4 + i)->vz = 0;
        }
        for (i = 0; i < FX_EAT_MODEL_COUNT; i++) {
            work->modelB8[i] = ex->btlfx_registerEffectModel((Unk800E8F50 *)ex->btlfx_getFileData(entry->file, fx_0fe_modelBlocks[i]), 1);
        }
        work->fadeInTimerB0 = -1;
        work->fadeOutTimerB4 = -1;
    } else {
        frame = entry->frame;
        switch (frame) {
        case FX_EAT_WHITE_RINGS_FRAME:
            work->emit0 = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, FX_EAT_WHITE_RINGS), (s32)&work->posA8);
            if (work->emit0 != NULL) {
                work->emit0->otZ = ex->btlfx_addRadiusSixth(0x10, 0x80);
            }
            break;
        case 0:
            work->fadeInTimerB0 = FX_EAT_FADE_IN_FRAMES;
            break;
        case FX_EAT_FADE_OUT_FRAME:
            work->fadeOutTimerB4 = FX_EAT_FADE_OUT_FRAMES;
            break;
        }
        if (frame >= 0 && frame < FX_EAT_ORBIT_FRAMES) {
            if (work->fadeInTimerB0 >= 0 || work->fadeOutTimerB4 >= 0) {
                if (work->fadeInTimerB0 >= 0) {
                    work->rgbA4 = fxm_lerpRatio(-0x80, 0, FX_EAT_FADE_IN_FRAMES - work->fadeInTimerB0, FX_EAT_FADE_IN_FRAMES);
                    work->fadeInTimerB0--;
                }
                if (work->fadeOutTimerB4 >= 0) {
                    work->rgbA4 = fxm_lerpRatio(0, -0x80, FX_EAT_FADE_OUT_FRAMES - work->fadeOutTimerB4, FX_EAT_FADE_OUT_FRAMES);
                    work->fadeOutTimerB4--;
                }
            }
            orbit = fx_0fe_orbits;
            model = work->modelB8[0];
            for (j = 0; j < FX_EAT_ORBIT_COUNT; j++, orbit++) {
                rot.vx = 0;
                rot.vy = orbit->angle2;
                rot.vz = 0;
                ex->btlfx_pointAhead((s16 *)&work->posA8, (s16 *)&rot, orbit->radius0, (s16 *)&pos);
                (work->pos34 + j)->vx = pos.vx;
                (work->pos34 + j)->vy = pos.vy;
                (work->pos34 + j)->vz = pos.vz;
                ex->Hi_DrawEffModel(&work->rot4[j], &work->pos34[j], (s32)&work->scale94, model);
                if (work->fadeInTimerB0 >= 0 || work->fadeOutTimerB4 >= 0) {
                    ex->Hi_ModifyEffModelAbr(model, BTLFX_ABR_ADD);
                    ex->Hi_ModifyEffModelRGB(model, work->rgbA4, work->rgbA4, work->rgbA4);
                }
                model++;
                if (model > work->modelB8[FX_EAT_MODEL_COUNT - 1]) {
                    model = work->modelB8[0];
                }
                orbit->angle2 += 0x55;
                orbit->spin4 += 0x200;
                work->rot4[j].vy += 0x80;
            }
        }
        if (frame >= FX_EAT_FRAMES) {
            ex->btlfx_clearEffModels();
            return 1;
        }
    }
    return 0;
}

static void fxm_lerpSvec(SVECTOR *from, SVECTOR *to, s32 t, SVECTOR *out) {
    BTLFX_EXPORTS(fx_0fe_env)->LoadAverageShort12(from, to, 0x1000 - t, t, out);
}

static s32 fxm_lerpRatio(s32 from, s32 to, s32 t, s32 n) {
    return (from * (n - t) + to * t) / n;
}

INCLUDE_RODATA("asm/us/fx_0fe/nonmatchings/fx_0fe", fx_0fe_headerPtr);
