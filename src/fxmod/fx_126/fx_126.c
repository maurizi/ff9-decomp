#include "fxmod/fx_126.h"

s32 fx_126_op80(s32 mode, Fx126Work *work, Unk800F78A4 *env, Unk80100C78Entry *entry, s32 *args) {
    BtlfxExports *ex = BTLFX_EXPORTS(env);
    Pool6CEntry *emitter;
    s32 frame;
    s32 target;

    if (mode == 0) {
        *args = sizeof(Fx126Work);
    } else if (mode == 1) {
        fx_126_env = env;
        ex->btlfx_getUnitBonePos(0x10, 0x30, (u16 *)&work->pos60);
        ex->btlfx_getUnitBonePos(0x10, 8, (u16 *)&work->pos58);
        ex->btlfx_initEffModels(fx_126_effModels, FX_BOOMERANG_MODEL_SLOTS);
        work->model0 = ex->btlfx_registerEffectModel((Unk800E8F50 *)ex->btlfx_getFileData(entry->file, FX_BOOMERANG_CURVED_BLADE), 0xFF);
        work->model4 = ex->btlfx_registerEffectModel((Unk800E8F50 *)ex->btlfx_getFileData(entry->file, FX_BOOMERANG_CURVED_BLADE), 0xFF);
        work->rot28.vy = 0x700;
        work->rot28.vz = 0x400;
        work->rot30.vy = -0x700;
        work->rot30.vz = -0x400;
        work->scale38.vx = 0x2000;
        work->scale38.vy = 0x2000;
        work->scale38.vz = 0x2000;
        work->pos50.vz = -FX_BOOMERANG_REACH;
        work->pos68.vx = FX_BOOMERANG_SWING_WIDTH;
        work->pos70.vx = -FX_BOOMERANG_SWING_WIDTH;
        work->rot28.vx = 0;
        work->rot30.vx = 0;
        work->pos50.vx = 0;
        work->pos50.vy = 0;
        work->pos68.vy = 0;
        work->pos70.vy = 0;
        work->pos68.vz = (work->pos58.vz - FX_BOOMERANG_REACH) >> 1;
        work->pos70.vz = (work->pos60.vz - FX_BOOMERANG_REACH) >> 1;
        for (target = FX_BOOMERANG_TARGETS - 1; target >= 0; target--) {
            work->done9C[target] = 0;
        }
    } else {
    frame = entry->frame;
    if (frame >= 1 && frame <= FX_BOOMERANG_FLIGHT_FRAMES) {
        fx_126_quarticBezier(FX_BOOMERANG_FLIGHT_FRAMES, frame, &work->pos58, &work->pos70, &work->pos50, &work->pos68, &work->pos60, &work->pos8);
        ex->Hi_DrawEffModel(&work->rot28, &work->pos8, (s32)&work->scale38, work->model0);
        fx_126_quarticBezier(FX_BOOMERANG_FLIGHT_FRAMES, frame, &work->pos60, &work->pos68, &work->pos50, &work->pos70, &work->pos58, &work->pos18);
        ex->Hi_DrawEffModel(&work->rot30, &work->pos18, (s32)&work->scale38, work->model4);
        work->rot28.vy -= 0x180;
        work->rot30.vy += 0x180;
        env->camera->points[0].vx = work->pos8.vx;
        env->camera->points[0].vy = work->pos8.vy;
        env->camera->points[0].vz = work->pos8.vz;
        env->camera->points[1].vx = work->pos18.vx;
        env->camera->points[1].vy = work->pos18.vy;
        env->camera->points[1].vz = work->pos18.vz;
    }
    switch (frame) {
    case FX_BOOMERANG_HIT_FRAME:
        work->done9C[0] = 1;
        ex->btlfx_getUnitBodyPos(0, 0, (s16 *)&work->pos7C[0]);
        emitter = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, FX_BOOMERANG_SWIRL), (s32)&work->pos7C[0]);
        if (emitter != NULL) {
            emitter->otZ = ex->btlfx_addRadiusSixth(0, 0x80);
        }
        emitter = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, FX_BOOMERANG_CRESCENTS), (s32)&work->pos7C[0]);
        if (emitter != NULL) {
            emitter->otZ = ex->btlfx_addRadiusSixth(0, 0x80);
        }
        target = 3;
        if (target >= env->targets->targetCount) {
            target = env->targets->targetCount - 1;
        }
        if (work->done9C[target] == 0) {
            work->done9C[target] = 1;
            ex->btlfx_getUnitBodyPos(target, 0, (s16 *)&work->pos7C[target]);
            emitter = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, FX_BOOMERANG_SWIRL), (s32)&work->pos7C[target]);
            if (emitter != NULL) {
                emitter->otZ = ex->btlfx_addRadiusSixth(target, 0x80);
            }
            emitter = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, FX_BOOMERANG_CRESCENTS), (s32)&work->pos7C[target]);
            if (emitter != NULL) {
                emitter->otZ = ex->btlfx_addRadiusSixth(target, 0x80);
            }
        }
        break;
    case FX_BOOMERANG_HIT_2_FRAME:
        target = 1;
        if (target >= env->targets->targetCount) {
            target = env->targets->targetCount - 1;
        }
        if (work->done9C[target] == 0) {
            work->done9C[target] = 1;
            ex->btlfx_getUnitBodyPos(target, 0, (s16 *)&work->pos7C[target]);
            emitter = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, FX_BOOMERANG_SWIRL), (s32)&work->pos7C[target]);
            if (emitter != NULL) {
                emitter->otZ = ex->btlfx_addRadiusSixth(target, 0x80);
            }
            emitter = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, FX_BOOMERANG_CRESCENTS), (s32)&work->pos7C[target]);
            if (emitter != NULL) {
                emitter->otZ = ex->btlfx_addRadiusSixth(target, 0x80);
            }
        }
        target = 2;
        if (target >= env->targets->targetCount) {
            target = env->targets->targetCount - 1;
        }
        if (work->done9C[target] == 0) {
            work->done9C[target] = 1;
            ex->btlfx_getUnitBodyPos(target, 0, (s16 *)&work->pos7C[target]);
            emitter = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, FX_BOOMERANG_SWIRL), (s32)&work->pos7C[target]);
            if (emitter != NULL) {
                emitter->otZ = ex->btlfx_addRadiusSixth(target, 0x80);
            }
            emitter = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, FX_BOOMERANG_CRESCENTS), (s32)&work->pos7C[target]);
            if (emitter != NULL) {
                emitter->otZ = ex->btlfx_addRadiusSixth(target, 0x80);
            }
        }
        break;
    }
    return frame >= FX_BOOMERANG_FRAMES;
    }
    return 0;
}

void fx_126_quarticBezier(s32 len, s32 t, SVECTOR *p0, SVECTOR *p1, SVECTOR *p2, SVECTOR *p3, SVECTOR *p4, VECTOR *out) {
    s32 weight[5];
    s32 ratio;
    s32 invRatio;

    ratio = ((0x80000 / len) * t) >> 12;
    invRatio = 0x80 - ratio;
    weight[0] = (invRatio * invRatio * invRatio * invRatio) >> 16;
    weight[1] = (ratio * invRatio * invRatio * invRatio) >> 13;
    weight[2] = (ratio * ratio * invRatio * invRatio * 24) >> 16;
    weight[3] = (ratio * ratio * ratio * invRatio) >> 13;
    weight[4] = (ratio * ratio * ratio * ratio) >> 16;
    out->vx = (p0->vx * weight[0]) >> 12;
    out->vy = (p0->vy * weight[0]) >> 12;
    out->vz = (p0->vz * weight[0]) >> 12;
    out->vx += (p1->vx * weight[1]) >> 12;
    out->vy += (p1->vy * weight[1]) >> 12;
    out->vz += (p1->vz * weight[1]) >> 12;
    out->vx += (p2->vx * weight[2]) >> 12;
    out->vy += (p2->vy * weight[2]) >> 12;
    out->vz += (p2->vz * weight[2]) >> 12;
    out->vx += (p3->vx * weight[3]) >> 12;
    out->vy += (p3->vy * weight[3]) >> 12;
    out->vz += (p3->vz * weight[3]) >> 12;
    out->vx += (p4->vx * weight[4]) >> 12;
    out->vy += (p4->vy * weight[4]) >> 12;
    out->vz += (p4->vz * weight[4]) >> 12;
}

INCLUDE_RODATA("asm/us/fx_126/nonmatchings/fx_126", fx_126_headerPtr);
