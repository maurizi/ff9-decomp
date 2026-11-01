#include "fxmod/fx_05f.h"

s32 fx_05f_op80(s32 mode, Fx05fWork80 *work, Unk800F78A4 *env, Unk80100C78Entry *entry, s32 *args) {
    BtlfxExports *ex = BTLFX_EXPORTS(env);
    SVECTOR rot;
    VECTOR pos;
    VECTOR scale;
    Pool6CEntry *p;
    s32 frame;
    s32 a;
    s32 m;
    s32 v;
    s32 c;

    if (mode == 0) {
        *args = 0x34;
    } else if (mode == 1) {
        work->stop0 = 0;
        work->posC.vx = env->targets->centerX;
        work->posC.vy = env->targets->centerY;
        work->posC.vz = env->targets->centerZ;
        fx_05f_env = env;
        work->posC.vy = 0;
        BTLFX_COPY_SVEC(&work->pos4, &work->posC);
        work->pos4.vy -= 0x200;
        ex->btlfx_initEffModels(fx_05f_effModels, 8);
        work->model14 = ex->Hi_RegisterTexEffModel((BtlModelHeader *)ex->btlfx_getFileData(entry->file, 6), 0x39, 0x3D00, 0, 0x80);
        work->model18 = ex->Hi_RegisterTexEffModel((BtlModelHeader *)ex->btlfx_getFileData(entry->file, 4), 0x39, 0x3D00, 0, 0x80);
        work->model1C = ex->Hi_RegisterTexEffModel((BtlModelHeader *)ex->btlfx_getFileData(entry->file, 5), 0x39, 0x3D00, 0, 0x80);
        work->model20 = ex->Hi_RegisterTexEffModel((BtlModelHeader *)ex->btlfx_getFileData(entry->file, 3), 0x39, 0x3D00, 0, 0x80);
        work->model24 = ex->Hi_RegisterTexEffModel((BtlModelHeader *)ex->btlfx_getFileData(entry->file, 2), 0x39, 0x3D00, 0, 0x80);
        work->model28 = ex->Hi_RegisterTexEffModel((BtlModelHeader *)ex->btlfx_getFileData(entry->file, 8), 0x3B, 0x3D01, 0, 0x80);
        work->model2C = ex->Hi_RegisterTexEffModel((BtlModelHeader *)ex->btlfx_getFileData(entry->file, 7), 0x3B, 0x3D01, 0, 0x80);
        work->model30 = ex->Hi_RegisterTexEffModel((BtlModelHeader *)ex->btlfx_getFileData(entry->file, 9), 0x39, 0x3D00, 0, 0x80);
    } else {
        frame = entry->frame;
        if (work->stop0 == 0) {
            if (frame < 24) {
                a = (frame << 12) / 24;
                pos.vx = work->posC.vx;
                pos.vy = work->posC.vy;
                pos.vz = work->posC.vz;
                v = ex->math_sin(a >> 2) / 10 + 0x333;
                m = (0x1000 - a) * 4;
                rot.vz = 0;
                rot.vx = 0;
                rot.vy = -frame * 0x20;
                scale.vz = v;
                scale.vx = v;
                scale.vy = v / 2;
                if (m > 0x3000) {
                    ex->Hi_DrawMorphEffModel(&rot, &pos, (s32)&scale, work->model14, work->model18, m - 0x3000);
                } else if (m > 0x2000) {
                    ex->Hi_DrawMorphEffModel(&rot, &pos, (s32)&scale, work->model18, work->model1C, m - 0x2000);
                } else if (m > 0x1000) {
                    ex->Hi_DrawMorphEffModel(&rot, &pos, (s32)&scale, work->model1C, work->model20, m - 0x1000);
                } else {
                    ex->Hi_DrawMorphEffModel(&rot, &pos, (s32)&scale, work->model20, work->model24, m);
                }
                scale.vx = scale.vz = (scale.vx * 5) >> 2;
                ex->Hi_DrawEffModel(&rot, &pos, (s32)&scale, work->model30);
                ex->Hi_ModifyEffModelRGB(work->model30, (ex->math_sin(a >> 2) >> 5) - 0x80, (ex->math_sin(a >> 2) >> 5) - 0x80,
                                         (ex->math_sin(a >> 2) >> 5) - 0x80);
                /* Dead test, removed after reload: it keeps m live (and call-crossing) through this branch, so
                 * global-alloc gives it s2 and the else branch's division temporary lands in t0. */
                if (m != 0) {
                    v = 0;
                }
                scale.vx = scale.vz = (scale.vx * 4) / 5;
            } else {
                m = ((frame - 24) << 12) / 26;
                c = m >> 2;
                pos.vx = work->posC.vx;
                pos.vy = work->posC.vy;
                pos.vz = work->posC.vz;
                scale.vx = scale.vz = 0x4CC;
                scale.vy = 0x266;
                rot.vz = 0;
                rot.vx = 0;
                rot.vy = -frame * 0x20;
                ex->Hi_DrawEffModel(&rot, &pos, (s32)&scale, work->model24);
                ex->Hi_ModifyEffModelRGB(work->model24, (ex->math_cos(c) >> 5) - 0x80, (ex->math_cos(c) >> 5) - 0x80,
                                         (ex->math_cos(c) >> 5) - 0x80);
                scale.vx = scale.vz = (scale.vx * 5) >> 2;
                ex->Hi_DrawEffModel(&rot, &pos, (s32)&scale, work->model30);
                ex->Hi_ModifyEffModelRGB(work->model30, (ex->math_cos(c) >> 5) - 0x80, (ex->math_cos(c) >> 5) - 0x80,
                                         (ex->math_cos(c) >> 5) - 0x80);
                scale.vx = scale.vz = (scale.vx * 4) / 5;
            }
            ex->btlfx_drawScrollMove(frame, 0x240, 0x180, 0x10, 0x40, fx_05f_scrollDst[frame % 23 + 1].x0,
                              fx_05f_scrollDst[frame % 23 + 1].y2);
            if (frame >= 24) {
                pos.vx = work->posC.vx;
                pos.vy = work->posC.vy;
                pos.vz = work->posC.vz;
                a = ((frame - 24) << 12) / 26;
                scale.vx = scale.vz = 0xE00;
                scale.vy = ex->math_sin(a >> 2) / 4 + 0x400;
                rot.vy = -frame * 0x40;
                rot.vz = 0;
                rot.vx = 0;
                ex->Hi_DrawMorphEffModel(&rot, &pos, (s32)&scale, work->model28, work->model2C, 0x1000 - a);
                /* Scheduling barrier: the morph call's arguments are scheduled as at a block end (&scale last,
                 * in the call's delay slot). */
                do {
                } while (0);
                a >>= 1;
                ex->Hi_ModifyEffModelRGB(work->model28, (ex->math_sin(a) >> 5) - 0x80, (ex->math_sin(a) >> 5) - 0x80,
                                         (ex->math_sin(a) >> 5) - 0x80);
            }
            if (frame == 16) {
                p = (Pool6CEntry *)ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, 0xE), (s32)&work->pos4);
                if (p != NULL) {
                    p->otZ = ex->btlfx_addRadiusSixth(0, 0x20);
                }
            }
            if (frame == 1) {
                p = (Pool6CEntry *)ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, 1), (s32)&work->pos4);
                if (p != NULL) {
                    p->otZ = ex->btlfx_addRadiusSixth(0, 0x40);
                }
            }
            if (frame >= 50) {
                ex->btlfx_clearEffModels();
                return 1;
            }
        }
    }
    return 0;
}

s32 fx_05f_op81(s32 mode, Fx05fWork81 *work, Unk800F78A4 *env, Unk80100C78Entry *entry, s32 *args) {
    BtlfxExports *ex = BTLFX_EXPORTS(env);
    s32 frame;
    s32 i;
    s32 t;
    SVECTOR v;

    if (mode == 0) {
        *args = 0x68;
    } else if (mode == 1) {
        fx_05f_env = env;
        work->count40 = env->targets->targetCount;
        for (i = 0; i < work->count40; i++) {
            ex->btlfx_getUnitHomePos(i, (s16 *)&work->home0[i]);
            work->jump44 = -0x190 - (ex->btlfx_rand() & 0x7F);
            ex->btlfx_getUnitBonePos(i, 0, (u16 *)&v);
            work->height48[i] = -v.vy;
        }
    } else {
        frame = entry->frame;
        if (frame < 0x20) {
            t = frame << 7;
            for (i = 0; i < work->count40; i++) {
                BTLFX_COPY_SVEC(&v, &work->home0[i]);
                v.vy += (work->jump44 * ex->math_sin(t >> 2)) >> 12;
                ex->btlfx_setUnitPos(i, (s16 *)&v);
            }
        } else {
            t = (frame - 0x20) << 10;
            for (i = 0; i < work->count40; i++) {
                BTLFX_COPY_SVEC(&v, &work->home0[i]);
                v.vy += (work->height48[i] * t) >> 12;
                v.vy += (work->jump44 * ex->math_cos(t >> 2)) >> 12;
                ex->btlfx_setUnitPos(i, (s16 *)&v);
            }
        }
        if (frame >= 0x24) {
            for (i = 0; i < work->count40; i++) {
                ex->btlfx_getUnitHomePos(i, (s16 *)&v);
                ex->btlfx_setUnitPos(i, (s16 *)&v);
            }
            return 1;
        }
    }
    return 0;
}

INCLUDE_RODATA("asm/us/fx_05f/nonmatchings/fx_05f", fx_05f_headerPtr);
