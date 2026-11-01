#include "fxmod/fx_125.h"

s32 fx_125_op80(s32 mode, Fx125Work *work, Unk800F78A4 *env, Unk80100C78Entry *entry, s32 *args) {
    BtlfxExports *ex = BTLFX_EXPORTS(env);
    SVECTOR rot;
    VECTOR scale;
    VECTOR pos;
    Pool6CEntry *emitter;
    s32 frame;
    s32 dispX;
    s32 otherX;
    s32 tp1;
    s32 tp2;
    s32 size;

    if (mode == 0) {
        *args = 0x30;
    } else if (mode == 1) {
        work->hold0 = 0;
        fx_125_env = env;
        ex->btlfx_getUnitBodyPos(0, 0, (s16 *)&work->pos14);
        ex->btlfx_getUnitPos(0, (s16 *)&work->pos4);
        work->posC.vx = work->pos4.vx;
        work->posC.vy = work->pos4.vy;
        work->posC.vz = work->pos4.vz;
        work->pos1C.vx = work->pos4.vx;
        work->pos1C.vy = work->pos4.vy;
        work->pos1C.vz = work->pos4.vz;
        work->posC.vy -= 0x100;
        work->pos1C.vy -= 0x400;
        ex->btlfx_initEffModels(fx_125_effModels, 3);
        work->model24 = (s16)ex->Hi_RegisterTexEffModel((BtlModelHeader *)ex->btlfx_getFileData(entry->file, 0xD), 0xB3,
                                                      0x3D40, 0, 0);
        dispX = env->dispEnv->disp.x;
        if (dispX < 0x100) {
            otherX = dispX + 0x140;
        } else {
            otherX = dispX - 0x140;
        }
        tp1 = getTPage(2, 3, dispX, env->dispEnv->disp.y);
        tp2 = getTPage(2, 3, otherX, env->dispEnv->disp.y);
        work->model28[0] = (s16)ex->Hi_RegisterTexEffModel((BtlModelHeader *)ex->btlfx_getFileData(entry->file, 0xE), tp1,
                                                         0xFFFF, 0, 0);
        work->model28[1] = (s16)ex->Hi_RegisterTexEffModel((BtlModelHeader *)ex->btlfx_getFileData(entry->file, 0xE), tp2,
                                                         0xFFFF, 0, 0);
        emitter = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, 7), (s32)&work->pos4);
        if (emitter != NULL) {
            emitter->otZ = ex->btlfx_addRadiusSixth(0, 0x20);
        }
    } else {
        frame = entry->frame;
        if (work->hold0 == 0) {
        if (frame >= 0x14) {
            pos.vx = work->pos1C.vx;
            pos.vy = work->pos1C.vy;
            pos.vz = work->pos1C.vz;
            rot.vy = frame << 5;
            rot.vx = 0;
            rot.vz = 0;
            if (frame >= 0x3C) {
                size = (((frame - 0x3C) << 12) / 10);
                size = ex->math_cos(size >> 2) / 3;
            } else {
                size = 0x555;
            }
            scale.vz = size;
            scale.vy = size;
            scale.vx = size;
            ex->Hi_DrawEffModel(&rot, &pos, (s32)&scale, work->model24);
            ex->Hi_ModifyEffModelAbr(work->model24, 1);
            ex->Hi_ModifyEffModelRGB(work->model24, -0x56, -0x56, -0x56);
            ex->Hi_DrawEffModel(&rot, &pos, (s32)&scale, work->model28[frame & 1]);
            ex->Hi_ModifyEffModelAbr(work->model28[frame & 1], 0xFF);
            ex->Hi_ModifyEffModelRGB(work->model28[frame & 1], 0x40, 0x40, 0x40);
        }
        if (frame == 0x10) {
            Pool6CEntry *burst;

            burst = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, 8), (s32)&work->pos4);
            if (burst != NULL) {
                burst->otZ = ex->btlfx_addRadiusSixth(0, 0x46);
            }
            burst = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, 1), (s32)&work->posC);
            if (burst != NULL) {
                burst->otZ = ex->btlfx_addRadiusSixth(0, 0x20);
            }
        }
        if (frame == 0x13) {
            Pool6CEntry *burst;

            burst = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, 2), (s32)&work->pos4);
            if (burst != NULL) {
                burst->otZ = ex->btlfx_addRadiusSixth(0, 0x20);
            }
            burst = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, 4), (s32)&work->pos4);
            if (burst != NULL) {
                burst->otZ = ex->btlfx_addRadiusSixth(0, 0x20);
            }
        }
        if (frame == 0x1A) {
            Pool6CEntry *burst;

            burst = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, 6), (s32)&work->pos14);
            if (burst != NULL) {
                burst->otZ = ex->btlfx_addRadiusSixth(0, 0x20);
            }
        }
        if (frame == 0x28) {
            Pool6CEntry *burst;

            burst = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, 5), (s32)&work->pos14);
            if (burst != NULL) {
                burst->otZ = ex->btlfx_addRadiusSixth(0, 0x32);
            }
            burst = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, 3), (s32)&work->pos14);
            if (burst != NULL) {
                burst->otZ = ex->btlfx_addRadiusSixth(0, 0x32);
            }
        }
        if (frame >= 0x46) {
            ex->btlfx_clearEffModels();
            return 1;
        }
        }
    }
    return 0;
}

INCLUDE_RODATA("asm/us/fx_125/nonmatchings/fx_125", fx_125_headerPtr);
