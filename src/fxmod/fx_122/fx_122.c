#include "fxmod/fx_122.h"

s32 fx_122_op80(s32 mode, Fx122Work *work, Unk800F78A4 *env, Unk80100C78Entry *entry, s32 *args) {
    BtlfxExports *ex = BTLFX_EXPORTS(env);
    s32 frame;
    s32 height;
    s32 radius;
    s32 i;
    s32 fade;
    Fx122Offset *ofs;

    if (mode == 0) {
        *args = 0x50;
    } else if (mode == 1) {
        fx_122_env = env;
        env->camera->paused = 0;
        ex->btlfx_getUnitPos(0, (s16 *)&work->pos30);
        ex->btlfx_getUnitRot(0, (s16 *)&work->rot38);
        work->rot4.vx = 0;
        work->rot4.vz = 0;
        work->scale1C.vx = 0x2000;
        work->scale1C.vy = 0x2000;
        work->scale1C.vz = 0x2000;
        work->rot4.vy = work->rot38.vy + 0x800;
        if (ex->btlfx_isEnemyUnit(0)) {
            height = ex->btlfx_getUnitHeight(0);
            radius = ex->btlfx_getUnitRadius(0x100);
            ofs = fx_122_offsets;
            for (i = 0; i < 0x11; i++, ofs++) {
                if (ofs->height0 == height && ofs->radius2 == radius) {
                    if (ofs->offset4 & 0x8000) {
                        goto forward;
                    }
                    work->posC.vx = work->pos30.vx;
                    work->posC.vy = work->pos30.vy - ofs->offset4;
                    work->posC.vz = work->pos30.vz;
                    goto found;
                }
            }
            if (height >= 0x258 || radius >= 0x2EE) {
                work->posC.vx = work->pos30.vx;
                work->posC.vy = work->pos30.vy - height;
                work->posC.vz = work->pos30.vz;
                goto found;
            }
            goto unit;
        forward:
            work->posC.vx = work->pos30.vx;
            work->posC.vy = work->pos30.vy;
            work->posC.vz = work->pos30.vz;
            work->posC.vz -= ofs->offset4 ^ 0x8000;
            goto found;
        }
    unit:
        work->posC.vx = work->pos30.vx;
        work->posC.vy = work->pos30.vy;
        work->posC.vz = work->pos30.vz;
    found:
        work->height2E = work->posC.vy;
        env->camera->points[0].vx = work->posC.vx;
        env->camera->points[0].vy = work->posC.vy;
        env->camera->points[0].vz = work->posC.vz;
        work->timer48 = -1;
        work->timer4A = -1;
        ex->btlfx_initEffModels(fx_122_effModels, 1);
        work->model4C = ex->btlfx_registerEffectModel((Unk800E8F50 *)ex->btlfx_getFileData(entry->file, 1), 1);
    } else {
        frame = entry->frame;
        ex->Hi_DrawEffModel(&work->rot4, &work->posC, (s32)&work->scale1C, work->model4C);
        if (frame >= 0x19 && frame < 0x28) {
            if ((frame & 1) && (ex->btlfx_rand() & 1)) {
                work->posC.vx = work->pos30.vx + (ex->btlfx_rand() % 4 + 4);
            } else {
                work->posC.vx = work->pos30.vx - (ex->btlfx_rand() % 4 + 4);
            }
            work->posC.vy = work->height2E + ex->btlfx_rand() % 4;
        }
        switch (frame) {
        case 0:
            work->timer48 = 8;
            break;
        case 0x40:
            work->timer4A = 0x10;
            break;
        }
        fade = 0;
        if (work->timer48 >= 0) {
            work->color2C = fxm_lerpRatio(-0x80, -0x40, 8 - work->timer48, 8);
            fade = 1;
            if (--work->timer48 == 0) {
                work->pos40.vx = work->posC.vx;
                work->pos40.vy = work->posC.vy;
                work->pos40.vz = work->posC.vz;
                work->pos40.vy -= 0xC8;
                work->emit0 = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, 5), (s32)&work->pos40);
                if (work->emit0 != NULL) {
                    work->emit0->otZ = ex->btlfx_addRadiusSixth(0, 0x80);
                }
            }
        }
        if (work->timer4A >= 0) {
            fade = 1;
            work->color2C = fxm_lerpRatio(0x28, -0x80, 0x10 - work->timer4A, 0x10);
            work->timer4A--;
        }
        if (fade) {
            ex->Hi_ModifyEffModelAbr(work->model4C, 1);
            ex->Hi_ModifyEffModelRGB(work->model4C, work->color2C, work->color2C, work->color2C);
        }
        if (frame >= 0x4F) {
            ex->btlfx_clearEffModels();
            return 1;
        }
    }
    return 0;
}

static void fxm_lerpSvec(SVECTOR *a, SVECTOR *b, s32 t, SVECTOR *out) {
    BTLFX_EXPORTS(fx_122_env)->LoadAverageShort12(a, b, 0x1000 - t, t, out);
}

static s32 fxm_lerpRatio(s32 from, s32 to, s32 t, s32 n) {
    return (from * (n - t) + to * t) / n;
}

INCLUDE_RODATA("asm/us/fx_122/nonmatchings/fx_122", fx_122_headerPtr);
