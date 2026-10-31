#include "fxmod/fx_11e.h"

s32 fx_11e_op80(s32 mode, Fx11eWork *work, Unk800F78A4 *env, Unk80100C78Entry *entry, s32 *arg4) {
    BtlfxExports *ex = BTLFX_EXPORTS(env);
    SVECTOR v;
    SVECTOR p;
    SVECTOR a;
    s32 frame;
    s32 t;
    s32 n;
    s32 m;

    if (mode == 0) {
        *arg4 = 0x60;
    } else if (mode == 1) {
        n = *arg4;
        work->unit58 = n;
        fx_11e_env = env;
        if (n >= env->targets->targetCount) {
            fxm_setSignal(env, BTLFX_SIG_WAIT, 1);
            return 1;
        }
        fxm_setSignal(env, BTLFX_SIG_WAIT, 0);
        ex->btlfx_playFileSound(7, entry->file, -1);
        ex->btlfx_playFileSound(8, entry->file, -1);
        ex->btlfx_playFileSound(9, entry->file, -1);
        if (fx_11e_modelsReady == 0) {
            ex->btlfx_initEffModels(fx_11e_effModels, 8);
            fx_11e_modelsReady = mode;
        }
        ex->btlfx_getUnitArgBonePos(0x10, 0, (u16 *)&work->pos50);
        ex->btlfx_getUnitBodyPos(work->unit58, 0, &work->pos48.vx);
        work->rot4.vx = 0;
        work->rot4.vy = 0;
        work->rot4.vz = 0;
        work->scale1C.vx = 0;
        work->scale1C.vy = 0;
        work->scale1C.vz = 0;
        work->posC.vx = work->pos48.vx;
        work->posC.vy = work->pos48.vy;
        work->posC.vz = work->pos48.vz;
        work->model5C = ex->btlfx_registerEffectModel((Unk800E8F50 *)ex->btlfx_getFileData(entry->file, 1), 1);
        work->model5E = ex->btlfx_registerEffectModel((Unk800E8F50 *)ex->btlfx_getFileData(entry->file, 2), 1);
        work->dir30 = ex->btlfx_rand() % 2;
        work->dir34 = ex->btlfx_rand() % 2;
        work->radius38 = ex->btlfx_rand() % 400 + 200;
        work->angle3C = ex->btlfx_rand() % 0x1000;
        work->timer40 = ex->btlfx_rand() % 16;
        work->timer44 = ex->btlfx_rand() % 16;
        work->emit0 = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, 3), (s32)&work->pos50);
    } else {
        frame = entry->frame;
        if ((u32)(frame - 0x1E) < 0x28) {
            t = frame - 0x1E;
            ex->btlfx_getUnitBodyPos(work->unit58, 0, &v.vx);
            work->posC.vx = v.vx;
            work->posC.vz = v.vz;
            work->posC.vx += work->radius38 * ex->math_sin(work->angle3C) / 4096;
            work->posC.vz += work->radius38 * ex->math_cos(work->angle3C) / 4096;
            if (t >= 0x19) {
                work->posC.vy += -200 - ex->btlfx_rand() % 40;
                work->radius38 += 200 + ex->btlfx_rand() % 100;
            } else {
                if (ex->btlfx_rand() & 1) {
                    work->posC.vy += ex->btlfx_rand() % 20;
                } else {
                    work->posC.vy -= ex->btlfx_rand() % 20;
                }
                if (work->dir30 != 0) {
                    work->radius38 += ex->btlfx_rand() % 20;
                } else {
                    work->radius38 -= ex->btlfx_rand() % 20;
                }
                if (work->dir34 != 0) {
                    work->angle3C += ex->btlfx_rand() % 256;
                } else {
                    work->angle3C -= ex->btlfx_rand() % 256;
                }
                p.vx = work->posC.vx;
                p.vy = work->posC.vy;
                p.vz = work->posC.vz;
                ex->btlfx_aimAngles(&v.vx, &p.vx, &a.vx);
                work->rot4.vx = a.vx;
                work->rot4.vy = a.vy;
                work->rot4.vz = a.vz;
            }
            if (t == 0x18) {
                work->rot4.vy += 0x800;
            }
            if (--work->timer40 == 0) {
                work->dir30 ^= 1;
                work->timer40 = ex->btlfx_rand() % 16;
            }
            if (--work->timer44 == 0) {
                work->dir34 ^= 1;
                work->timer44 = ex->btlfx_rand() % 16;
            }
            if (work->scale1C.vx < 0x1000) {
                work->scale1C.vx += 0x80;
                work->scale1C.vy += 0x80;
                work->scale1C.vz += 0x80;
            }
            if (t & 1) {
                m = work->model5C;
            } else {
                m = work->model5E;
            }
            ex->Hi_DrawEffModel(&work->rot4, &work->posC, (s32)&work->scale1C, m);
        }
        if (work->emit0 != NULL) {
            if (work->emit0->state == BTLFX_EMIT_DONE) {
                work->emit0 = NULL;
            }
            if (work->emit0 != NULL) {
                work->emit0->target = (s32)&work->pos48;
            }
        }
        if (frame == 3) {
            fxm_setSignal(env, BTLFX_SIG_WAIT, 1);
        }
        return frame >= 0x50;
    }
    return 0;
}

static void fxm_setSignal(Unk800F78A4 *env, s32 i, s32 v) {
    env->state->signals[i] = v;
}

INCLUDE_RODATA("asm/us/fx_11e/nonmatchings/fx_11e", fx_11e_headerPtr);
