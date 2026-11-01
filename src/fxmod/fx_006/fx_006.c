#include "fxmod/fx_006.h"

s32 fx_006_op80(s32 mode, Fx006Work *work, Unk800F78A4 *env, Unk80100C78Entry *entry, s32 *args) {
    BtlfxExports *ex = BTLFX_EXPORTS(env);
    s32 frame;
    s32 s;
    s32 t;
    Pool6CEntry *emitter;
    VECTOR pos;
    VECTOR scale;
    SVECTOR rot;

    if (mode == 0) {
        *args = 0x28;
    } else if (mode == 1) {
        fx_006_env = env;
        ex->btlfx_initEffModels(fx_006_effModels, 4);
        work->model8 = (s16)ex->Hi_RegisterTexEffModel((BtlModelHeader *)ex->btlfx_getFileData(entry->file, 3), 0xBA, 0x3DC0, 0, 0);
        work->modelC = (s16)ex->Hi_RegisterTexEffModel((BtlModelHeader *)ex->btlfx_getFileData(entry->file, 4), 0xBA, 0x3DC0, 0, 0);
        work->model10 = (s16)ex->Hi_RegisterTexEffModel((BtlModelHeader *)ex->btlfx_getFileData(entry->file, 5), 0xBA, 0x3DC0, 0, 0);
        work->model4 =
            (s16)ex->Hi_RegisterTexEffModel((BtlModelHeader *)ex->btlfx_getFileData(entry->file, 2), 0xB8, 0x3D40, 0x80, 0x80);
        work->state0 = 0;
        work->frame24 = 0;
        ex->btlfx_getUnitBodyPos(0, 0, (s16 *)&work->pos14);
        ex->btlfx_getUnitPos(0, (s16 *)&work->pos1C);
    } else {
        frame = entry->frame;
        switch (work->state0) {
        case 0:
            if (frame == 0) {
                emitter = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, 0x12), (s32)&work->pos14);
                if (emitter != NULL) {
                    emitter->trails = (s32)fx_006_trails;
                    emitter->trailSlots = 0xA;
                    emitter->trailStride = 0x5C;
                    emitter->otZ = ex->btlfx_addRadiusSixth(0, 0x20);
                }
            }
            if (frame >= 8) {
                work->state0 = 1;
                entry->frame = -1;
            }
            break;
        case 1:
            t = frame << 10;
            scale.vx = scale.vy = scale.vz = ((0x1000 - ex->math_cos(t >> 2)) * 0xC00) >> 12;
            rot.vx = 0;
            rot.vy = 0;
            rot.vz = 0;
            pos.vx = work->pos14.vx;
            pos.vy = work->pos14.vy;
            pos.vz = work->pos14.vz;
            ex->Hi_SetEffModelOffset(work->model8, 2);
            ex->Hi_DrawEffModel(&rot, &pos, (s32)&scale, work->model8);
            ex->Hi_ModifyEffModelAbr(work->model8, 0);
            ex->Hi_ModifyEffModelRGB(work->model8, 0, 0, 0);
            if (frame >= 4) {
                work->state0 = 2;
                entry->frame = -1;
            }
            break;
        case 2:
            scale.vz = 0xC00;
            scale.vy = 0xC00;
            scale.vx = 0xC00;
            rot.vx = 0;
            rot.vy = frame << 5;
            rot.vz = 0;
            pos.vx = work->pos14.vx;
            pos.vy = work->pos14.vy;
            pos.vz = work->pos14.vz;
            t = frame << 8;
            ex->Hi_SetEffModelOffset(work->model8, 2);
            ex->Hi_DrawEffModel(&rot, &pos, (s32)&scale, work->model8);
            ex->Hi_ModifyEffModelAbr(work->model8, 0);
            ex->Hi_ModifyEffModelRGB(work->model8, 0, 0, 0);
            ex->Hi_DrawEffModel(&rot, &pos, (s32)&scale, work->model4);
            ex->Hi_ModifyEffModelAbr(work->model4, 1);
            ex->Hi_ModifyEffModelRGB(work->model4, (ex->math_sin(t >> 1) >> 5) - 0x80, (ex->math_sin(t >> 1) >> 5) - 0x80,
                                     (ex->math_sin(t >> 1) >> 5) - 0x80);
            ex->btlfx_drawScrollMove(((0x1000 - ex->math_cos(t >> 1)) >> 9) + frame * 5, 0x240, 0x180, 0x40, 0x80, 0x240, 0x100);
            if (frame >= 0x10) {
                work->state0 = 3;
                entry->frame = -1;
                emitter = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, 0xE), (s32)&work->pos14);
                if (emitter != NULL) {
                    emitter->otZ = ex->btlfx_addRadiusSixth(0, 0x20);
                }
                emitter = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, 0xD), (s32)&work->pos14);
                if (emitter != NULL) {
                    emitter->otZ = ex->btlfx_addRadiusSixth(0, 0x20);
                }
                emitter = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, 1), (s32)&work->pos14);
                if (emitter != NULL) {
                    emitter->otZ = ex->btlfx_addRadiusSixth(0, 0x20);
                }
                emitter = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, 6), (s32)&work->pos14);
                if (emitter != NULL) {
                    emitter->otZ = ex->btlfx_addRadiusSixth(0, 0x20);
                }
                ex->btlfx_drawScreenTint((u32 *)(env->ot + 1), 1, 0, 0x20, 0x46);
            }
            break;
        case 3:
            scale.vz = 0xC00;
            scale.vy = 0xC00;
            scale.vx = 0xC00;
            rot.vx = 0;
            rot.vy = 0x200;
            rot.vz = 0;
            pos.vx = work->pos14.vx;
            pos.vy = work->pos14.vy;
            pos.vz = work->pos14.vz;
            if (frame < 2) {
                s = 0x2000 - (frame << 11);
            } else {
                s = 0x1000 - ((frame - 2) << 12) / 14;
            }
            if (s > 0x1000) {
                ex->Hi_DrawMorphEffModel(&rot, &pos, (s32)&scale, work->model8, work->modelC, s - 0x1000);
                ex->Hi_ModifyEffModelRGB(work->model8, -0x40, -0x40, -0x40);
            } else {
                ex->Hi_DrawMorphEffModel(&rot, &pos, (s32)&scale, work->modelC, work->model10, s);
                ex->Hi_ModifyEffModelRGB(work->modelC, ((s - 0x800) >> 5) - 0x80, ((s - 0x800) >> 5) - 0x80,
                                         ((s - 0x800) >> 5) - 0x80);
            }
            if (frame == 1) {
                emitter = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, 0x10), (s32)&work->pos14);
                if (emitter != NULL) {
                    emitter->otZ = ex->btlfx_addRadiusSixth(0, 0x20);
                }
            }
            if (frame >= 0xC) {
                ex->btlfx_clearEffModels();
                return 1;
            }
            break;
        }
        work->frame24++;
    }
    return 0;
}

INCLUDE_RODATA("asm/us/fx_006/nonmatchings/fx_006", fx_006_headerPtr);
