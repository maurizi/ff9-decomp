#include "fxmod/fx_016.h"

s32 fx_016_op80(s32 mode, Fx016Work80 *work, Unk800F78A4 *env, Unk80100C78Entry *entry, s32 *arg4) {
    BtlfxExports *ex = BTLFX_EXPORTS(env);
    SVECTOR rot;
    VECTOR pos;
    VECTOR scale;
    Pool6CEntry *p;
    s32 frame;
    s32 r;
    s32 a;
    s32 m;
    s32 v;
    s32 q;
    s32 w;

    if (mode == 0) {
        *arg4 = 0x38;
    } else if (mode == 1) {
        work->stop0 = 0;
        fx_016_env = env;
        ex->btlfx_getUnitBodyPos(0, 0, (s16 *)&work->pos4);
        ex->btlfx_getUnitPos(0, (s16 *)&work->posC);
        ex->btlfx_initEffModels(fx_016_effModels, 8);
        work->model14 = ex->Hi_RegisterTexEffModel((BtlModelHeader *)ex->btlfx_getFileData(entry->file, 5), 0x39, 0x3D00, 0, 0x80);
        work->model18 = ex->Hi_RegisterTexEffModel((BtlModelHeader *)ex->btlfx_getFileData(entry->file, 2), 0x39, 0x3D00, 0, 0x80);
        work->model1C = ex->Hi_RegisterTexEffModel((BtlModelHeader *)ex->btlfx_getFileData(entry->file, 3), 0x39, 0x3D00, 0, 0x80);
        work->model20 = ex->Hi_RegisterTexEffModel((BtlModelHeader *)ex->btlfx_getFileData(entry->file, 4), 0x39, 0x3D00, 0, 0x80);
        work->model24 = ex->Hi_RegisterTexEffModel((BtlModelHeader *)ex->btlfx_getFileData(entry->file, 0xE), 0x39, 0x3D00, 0, 0x80);
        work->model28 = ex->Hi_RegisterTexEffModel((BtlModelHeader *)ex->btlfx_getFileData(entry->file, 8), 0x3B, 0x3D01, 0, 0x80);
        work->model2C = ex->Hi_RegisterTexEffModel((BtlModelHeader *)ex->btlfx_getFileData(entry->file, 7), 0x3B, 0x3D01, 0, 0x80);
        work->model30 = ex->Hi_RegisterTexEffModel((BtlModelHeader *)ex->btlfx_getFileData(entry->file, 9), 0x39, 0x3D00, 0, 0x80);
        r = (ex->btlfx_getUnitRadius(0) << 12) / 720;
        if (r < 0x400) {
            r = 0x6A4;
        }
        if (r > 0x1800) {
            r = 0x1800;
        }
        fx_016_radiusScale = r;
        work->scale34 = r;
    } else {
        frame = entry->frame;
        if (work->stop0 == 0) {
            if (frame < 24) {
                a = (frame << 12) / 24;
                pos.vx = work->posC.vx;
                pos.vy = work->posC.vy;
                pos.vz = work->posC.vz;
                v = (((ex->math_sin(a >> 2) / 10) + 0x199) * work->scale34) >> 12;
                rot.vz = 0;
                rot.vx = 0;
                m = (0x1000 - a) * 4;
                rot.vy = -frame * 0x20;
                scale.vy = (v * 3) >> 2;
                scale.vz = v;
                scale.vx = v;
                if (m > 0x3000) {
                    ex->Hi_DrawMorphEffModel(&rot, &pos, (s32)&scale, work->model14, work->model18, m - 0x3000);
                } else if (m > 0x2000) {
                    ex->Hi_DrawMorphEffModel(&rot, &pos, (s32)&scale, work->model18, work->model1C, m - 0x2000);
                } else if (m > 0x1000) {
                    ex->Hi_DrawMorphEffModel(&rot, &pos, (s32)&scale, work->model1C, work->model20, m - 0x1000);
                } else {
                    ex->Hi_DrawMorphEffModel(&rot, &pos, (s32)&scale, work->model20, work->model24, m);
                }
                ex->Hi_DrawEffModel(&rot, &pos, (s32)&scale, work->model30);
                ex->Hi_ModifyEffModelRGB(work->model30, (ex->math_sin(a >> 2) >> 5) - 0x80,
                                         (ex->math_sin(a >> 2) >> 5) - 0x80, (ex->math_sin(a >> 2) >> 5) - 0x80);
            } else {
                /* m (not q) takes the division: the copy q = m >> 2 into a block-local pseudo (s2) gives m a
                 * global-alloc preference for s2, which it also gets in the frame < 24 branch. The scale value
                 * here is its own block-local variable w. */
                m = ((frame - 24) << 12) / 26;
                pos.vx = work->posC.vx;
                pos.vy = work->posC.vy;
                pos.vz = work->posC.vz;
                w = (work->scale34 * 0x332) >> 12;
                rot.vz = 0;
                rot.vx = 0;
                rot.vy = -frame * 0x20;
                scale.vz = w;
                scale.vx = w;
                scale.vy = (w * 3) >> 2;
                ex->Hi_DrawEffModel(&rot, &pos, (s32)&scale, work->model24);
                q = m >> 2;
                ex->Hi_ModifyEffModelRGB(work->model24, (ex->math_cos(q) >> 5) - 0x80,
                                         (ex->math_cos(q) >> 5) - 0x80, (ex->math_cos(q) >> 5) - 0x80);
                ex->Hi_DrawEffModel(&rot, &pos, (s32)&scale, work->model30);
                ex->Hi_ModifyEffModelRGB(work->model30, (ex->math_cos(q) >> 5) - 0x80,
                                         (ex->math_cos(q) >> 5) - 0x80, (ex->math_cos(q) >> 5) - 0x80);
            }
            ex->btlfx_drawScrollMove(frame, 0x240, 0x180, 0x10, 0x40, fx_016_scrollDst[frame % 23 + 1].x0,
                              fx_016_scrollDst[frame % 23 + 1].y2);
            if (frame >= 24) {
                pos.vx = work->posC.vx;
                pos.vy = work->posC.vy;
                pos.vz = work->posC.vz;
                scale.vx = scale.vz = (work->scale34 << 11) >> 12;
                a = ((frame - 24) << 12) / 26;
                scale.vy = (((ex->math_sin(a >> 2) / 4) + 0x2AA) * work->scale34) >> 12;
                rot.vz = 0;
                rot.vx = 0;
                rot.vy = -frame * 0x40;
                ex->Hi_DrawMorphEffModel(&rot, &pos, (s32)&scale, work->model28, work->model2C, 0x1000 - a);
                ex->Hi_ModifyEffModelRGB(work->model28, (ex->math_sin(a >> 1) >> 5) - 0x80,
                                         (ex->math_sin(a >> 1) >> 5) - 0x80, (ex->math_sin(a >> 1) >> 5) - 0x80);
            }
            if (frame == 16) {
                p = (Pool6CEntry *)ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, 1), (s32)&work->pos4);
                if (p != NULL) {
                    p->otZ = ex->btlfx_addRadiusSixth(0, 0x20);
                }
            }
            if (frame == 1) {
                p = (Pool6CEntry *)ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, 0xA), (s32)&work->pos4);
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

s32 fx_016_op81(s32 mode, Fx016Work81 *work, Unk800F78A4 *env, Unk80100C78Entry *entry, s32 *arg4) {
    BtlfxExports *ex = BTLFX_EXPORTS(env);
    s32 frame;
    s32 i;
    s32 t;
    SVECTOR v;

    if (mode == 0) {
        *arg4 = 0x68;
    } else if (mode == 1) {
        fx_016_env = env;
        work->count40 = env->targets->targetCount;
        for (i = 0; i < work->count40; i++) {
            ex->btlfx_getUnitHomePos(i, (s16 *)&work->home0[i]);
            work->jump44 = ((-0x140 - (ex->btlfx_rand() & 0x1F)) * fx_016_radiusScale) >> 12;
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

INCLUDE_RODATA("asm/us/fx_016/nonmatchings/fx_016", fx_016_headerPtr);
