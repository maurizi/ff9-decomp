#include "fxmod/fx_0ff.h"

s32 fx_0ff_op80(s32 mode, Fx0ffWork *work, Unk800F78A4 *env, Unk80100C78Entry *entry, s32 *arg4) {
    BtlfxExports *ex = BTLFX_EXPORTS(env);
    SVECTOR rot;
    SVECTOR pos;
    s32 frame;
    s32 i;
    s32 j;
    s32 r;
    s32 m;
    Fx0ffOrbit *o;
    Fx0ffOrbit *q;
    Fx0ffOrbit *p;

    if (mode == 0) {
        *arg4 = 0xCC;
    } else if (mode == 1) {
        fx_0ff_env = env;
        ex->btlfx_initEffModels(fx_0ff_effModels, 3);
        ex->btlfx_getUnitBodyPos(0, 0, (s16 *)&work->posAC);
        ex->btlfx_getUnitPos(0, (s16 *)&work->posB4);
        if (work->posAC.vy >= -0x117) {
            work->posAC.vy = -0x118;
        }
        work->scale98.vx = 0x8000;
        work->scale98.vy = 0x8000;
        work->scale98.vz = 0x8000;
        r = ex->btlfx_getUnitRadius(0x100);
        r = (r >= 0x899) ? 0x898 : r;
        q = fx_0ff_orbits;
        for (i = 0; i < 6; i++, q++) {
            q->radius0 = r;
            q->angle2 = i * 0x2AA;
            q->spin4 = i * 0x2AA;
            (work->rot8 + i)->vx = 0;
            (work->rot8 + i)->vy = 0;
            (work->rot8 + i)->vz = 0;
        }
        for (i = 0; i < 3; i++) {
            work->modelC4[i] = ex->btlfx_registerEffectModel((Unk800E8F50 *)ex->btlfx_getFileData(entry->file, fx_0ff_modelBlocks[i]), 1);
        }
        work->fadeInTimerBC = -1;
        work->fadeOutTimerC0 = -1;
    } else {
        frame = entry->frame;
        switch (frame) {
        case 2:
            work->emit0 = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, 8), (s32)&work->posB4);
            if (work->emit0 != NULL) {
                work->emit0->otZ = ex->btlfx_addRadiusSixth(0x10, 0x80);
            }
            break;
        case 0x16:
            work->emit4 = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, 4), (s32)&work->posAC);
            if (work->emit4 != NULL) {
                work->emit4->otZ = ex->btlfx_addRadiusSixth(0x10, 0x80);
            }
            break;
        case 0:
            work->fadeInTimerBC = 4;
            break;
        case 0x14:
            work->fadeOutTimerC0 = 8;
            break;
        }
        if (frame >= 0 && frame < 0x1C) {
            if (work->fadeInTimerBC >= 0 || work->fadeOutTimerC0 >= 0) {
                if (work->fadeInTimerBC >= 0) {
                    work->rgbA8 = fxm_lerpRatio(-0x80, 0, 4 - work->fadeInTimerBC, 4);
                    work->fadeInTimerBC--;
                }
                if (work->fadeOutTimerC0 >= 0) {
                    work->rgbA8 = fxm_lerpRatio(0, -0x80, 8 - work->fadeOutTimerC0, 8);
                    work->fadeOutTimerC0--;
                }
            }
            o = fx_0ff_orbits;
            m = work->modelC4[0];
            for (j = 0; j < 6; j++, o++) {
                rot.vx = 0;
                rot.vy = o->angle2;
                rot.vz = 0;
                ex->btlfx_pointAhead((s16 *)&work->posAC, (s16 *)&rot, o->radius0, (s16 *)&pos);
                (work->pos38 + j)->vx = pos.vx;
                (work->pos38 + j)->vy = pos.vy;
                (work->pos38 + j)->vz = pos.vz;
                ex->Hi_DrawEffModel(&work->rot8[j], &work->pos38[j], (s32)&work->scale98, m);
                if (work->fadeInTimerBC >= 0 || work->fadeOutTimerC0 >= 0) {
                    ex->Hi_ModifyEffModelAbr(m, 1);
                    ex->Hi_ModifyEffModelRGB(m, work->rgbA8, work->rgbA8, work->rgbA8);
                }
                m++;
                if (m > work->modelC4[2]) {
                    m = work->modelC4[0];
                }
                o->angle2 += 0x55;
                o->spin4 += 0x200;
                work->rot8[j].vy += 0x80;
            }
        }
        if (frame >= 0x28) {
            ex->btlfx_clearEffModels();
            return 1;
        }
    }
    return 0;
}

static void fxm_lerpSvec(SVECTOR *a, SVECTOR *b, s32 t, SVECTOR *out) {
    BTLFX_EXPORTS(fx_0ff_env)->LoadAverageShort12(a, b, 0x1000 - t, t, out);
}

static s32 fxm_lerpRatio(s32 from, s32 to, s32 t, s32 n) {
    return (from * (n - t) + to * t) / n;
}

INCLUDE_RODATA("asm/us/fx_0ff/nonmatchings/fx_0ff", fx_0ff_headerPtr);
