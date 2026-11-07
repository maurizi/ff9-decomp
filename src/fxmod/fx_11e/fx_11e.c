#include "fxmod/fx_11e.h"

s32 fx_11e_op80(s32 mode, Fx11eWork *work, Unk800F78A4 *env, Unk80100C78Entry *entry, s32 *args) {
    BtlfxExports *ex = BTLFX_EXPORTS(env);
    SVECTOR unitPos;
    SVECTOR orbPos;
    SVECTOR aim;
    s32 frame;
    s32 elapsed;
    s32 target;
    s32 model;

    if (mode == 0) {
        *args = sizeof(Fx11eWork);
    } else if (mode == 1) {
        target = *args;
        work->unit58 = target;
        fx_11e_env = env;
        if (target >= env->targets->targetCount) {
            fxm_setSignal(env, BTLFX_SIG_WAIT, 1);
            return 1;
        }
        fxm_setSignal(env, BTLFX_SIG_WAIT, 0);
        ex->btlfx_playFileSound(7, entry->file, -1);
        ex->btlfx_playFileSound(8, entry->file, -1);
        ex->btlfx_playFileSound(9, entry->file, -1);
        if (fx_11e_modelsReady == 0) {
            ex->btlfx_initEffModels(fx_11e_effModels, FX_VIRUS_FLY_MODEL_SLOTS);
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
        work->model5C = ex->btlfx_registerEffectModel((Unk800E8F50 *)ex->btlfx_getFileData(entry->file, FX_VIRUS_FLY), 1);
        work->model5E = ex->btlfx_registerEffectModel((Unk800E8F50 *)ex->btlfx_getFileData(entry->file, FX_VIRUS_FLY_2), 1);
        work->dir30 = ex->btlfx_rand() % 2;
        work->dir34 = ex->btlfx_rand() % 2;
        work->radius38 = ex->btlfx_rand() % 400 + 200;
        work->angle3C = ex->btlfx_rand() % 0x1000;
        work->timer40 = ex->btlfx_rand() % 16;
        work->timer44 = ex->btlfx_rand() % 16;
        work->emit0 = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, FX_VIRUS_FLY_FLIES), (s32)&work->pos50);
    } else {
        frame = entry->frame;
        if ((u32)(frame - FX_VIRUS_FLY_BUZZ_FRAME) < FX_VIRUS_FLY_BUZZ_FRAMES) {
            elapsed = frame - FX_VIRUS_FLY_BUZZ_FRAME;
            ex->btlfx_getUnitBodyPos(work->unit58, 0, &unitPos.vx);
            work->posC.vx = unitPos.vx;
            work->posC.vz = unitPos.vz;
            work->posC.vx += work->radius38 * ex->math_sin(work->angle3C) / 4096;
            work->posC.vz += work->radius38 * ex->math_cos(work->angle3C) / 4096;
            if (elapsed >= FX_VIRUS_FLY_LEAVE_FRAME) {
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
                orbPos.vx = work->posC.vx;
                orbPos.vy = work->posC.vy;
                orbPos.vz = work->posC.vz;
                ex->btlfx_aimAngles(&unitPos.vx, &orbPos.vx, &aim.vx);
                work->rot4.vx = aim.vx;
                work->rot4.vy = aim.vy;
                work->rot4.vz = aim.vz;
            }
            if (elapsed == FX_VIRUS_FLY_LEAVE_FRAME - 1) {
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
            if (elapsed & 1) {
                model = work->model5C;
            } else {
                model = work->model5E;
            }
            ex->Hi_DrawEffModel(&work->rot4, &work->posC, (s32)&work->scale1C, model);
        }
        if (work->emit0 != NULL) {
            if (work->emit0->state == BTLFX_EMIT_DONE) {
                work->emit0 = NULL;
            }
            if (work->emit0 != NULL) {
                work->emit0->target = (s32)&work->pos48;
            }
        }
        if (frame == FX_VIRUS_FLY_RELEASE_FRAME) {
            fxm_setSignal(env, BTLFX_SIG_WAIT, 1);
        }
        return frame >= FX_VIRUS_FLY_FRAMES;
    }
    return 0;
}

static void fxm_setSignal(Unk800F78A4 *env, s32 signal, s32 value) {
    env->state->signals[signal] = value;
}

INCLUDE_RODATA("asm/us/fx_11e/nonmatchings/fx_11e", fx_11e_headerPtr);
