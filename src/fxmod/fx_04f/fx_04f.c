#include "fxmod/fx_04f.h"

static s32 fx_04f_op80PoolTask(s32 mode, s32 *it, Unk800F78A4 *env, Unk80100C78Entry *entry, s32 frame, Unk800DA1B8 *pool,
                  Fx04fWork *work) {
    BtlfxExports *ex = BTLFX_EXPORTS(env);
    SVECTOR p;
    SVECTOR rot;
    VECTOR scale;
    VECTOR pos;
    s32 a;
    s32 c;
    s32 s;
    s32 *q;
    s32 c2;

    if (mode == 1) {
        *it = 0;
    } else {
        a = ((frame << 12) / 9) >> 1;
        c = ex->math_sin(a);
        if (frame < 5) {
            c2 = a + 0x200;
            s = ((ex->math_sin((frame << 10) >> 2) * (work->scaleX4C >> 1)) >> 12) + c2;
        } else {
            c2 = a + 0x200;
            s = (work->scaleX4C >> 1) + *(q = &c2); /* the address taken changes c2's register (permuter) */
        }
        scale.vy = s;
        scale.vx = s;
        scale.vz = (work->scaleZ50 * 7) >> 3;
        BTLFX_COPY_SVEC(&rot, &work->rot38);
        rot.vy += 0x800;
        ex->btlfx_pointAhead((s16 *)&work->pos14, (s16 *)&rot, *it, (s16 *)&p);
        pos.vx = p.vx;
        pos.vy = p.vy;
        pos.vz = p.vz;
        *it -= 0x18;
        ex->Hi_DrawEffModel(&work->rot38, &pos, (s32)&scale, work->model4);
        ex->Hi_ModifyEffModelRGB(work->model4, (c >> 5) - 0x80, (c >> 5) - 0x80, (c >> 5) - 0x80);
        if (frame >= 9) {
            return 1;
        }
    }
    return 0;
}

s32 fx_04f_op80(s32 mode, Fx04fWork *work, Unk800F78A4 *env, Unk80100C78Entry *entry, EffInit *init) {
    BtlfxExports *ex = BTLFX_EXPORTS(env);
    s32 frame;
    s32 r;
    s32 g;
    s32 b;
    s32 k;
    s32 t;
    VECTOR scale;
    SVECTOR v;
    s32 q;

    if (mode == 0) {
        init->workSize = 0x54;
        init->elemSize = 4;
        init->elemFn = (s32)fx_04f_op80PoolTask;
        init->poolCount = 3;
    } else {
        if (mode == 1) {
        fx_04f_env = env;
        ex->btlfx_initEffModels(fx_04f_effModels, 4);
        work->model4 = (s16)ex->Hi_RegisterGouEffModel((BtlModelHeader *)ex->btlfx_getFileData(entry->file, 0xB));
        work->modelC = (s16)ex->Hi_RegisterTexEffModel((BtlModelHeader *)ex->btlfx_getFileData(entry->file, 6), 0x117, 0xFFFF, 0, 0x80);
        work->model10 = (s16)ex->Hi_RegisterTexEffModel((BtlModelHeader *)ex->btlfx_getFileData(entry->file, 0xC), 0x117, 0xFFFF, 0, 0);
        work->state0 = 0;
        work->frame48 = 0;
        ex->btlfx_getUnitBodyPos(0, 0, (s16 *)&work->pos14);
        ex->btlfx_getUnitRot(0, (s16 *)&work->rot38);
        work->radius40 = ex->btlfx_getUnitRadius(0);
        ex->btlfx_pointAhead((s16 *)&work->pos14, (s16 *)&work->rot38, work->radius40, (s16 *)&work->pos14);
        ex->btlfx_pointAhead((s16 *)&work->pos14, (s16 *)&work->rot38, 0x180, (s16 *)&work->pos24);
        work->rot38.vy += 0x800;
        work->pos2C[0] = work->pos14.vx;
        work->pos2C[1] = work->pos14.vy;
        work->pos2C[2] = work->pos14.vz;
        BTLFX_COPY_SVEC(&work->pos1C, &work->pos14);
    } else {
        r = 0;
        g = 0;
        b = 0;
        k = 0;
        frame = entry->frame;
        switch (work->state0) {
        case 0:
            r = frame << 9;
            b = 0;
            g = 0;
            scale.vx = scale.vy = ((0x2000 - ex->math_sin(r >> 2)) * 0xC00) >> 12;
            scale.vz = 0;
            if (frame >= 8) {
                work->state0 = 1;
                entry->frame = -1;
                work->emit44 = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, 4), (s32)&work->pos1C);
                if (work->emit44 != NULL) {
                    work->emit44->otZ = ex->btlfx_addRadiusSixth(0, 0x20);
                }
            }
            break;
        case 1:
            q = (frame << 12) / 20;
            if (frame < 10) {
                r = 0x1000;
                b = (frame << 12) / 10;
                g = b;
            } else {
                b = 0x1000;
                g = 0x1000;
                r = 0x1000;
            }
            k = q;
            t = k * 0xC00;
            scale.vy = (t >> 12) + 0xC00;
            scale.vx = (t >> 12) + 0xC00;
            scale.vz = t >> 11;
            if (frame % 5 == 0) {
                ex->btlfx_poolSpawn((s16 *)entry->pool);
            }
            if (work->emit44 != NULL) {
                if (work->emit44->state == BTLFX_EMIT_DONE) {
                    work->emit44 = NULL;
                } else {
                    work->rot38.vy += 0x800;
                    ex->btlfx_pointAhead((s16 *)&work->pos14, (s16 *)&work->rot38, frame * 0x12, (s16 *)&v);
                    work->rot38.vy += 0x800;
                    ex->btlfx_moveEmitterTo((s32)work->emit44, (s32)&v);
                }
            }
            if (frame >= 0x14) {
                work->state0 = 2;
                entry->frame = -1;
            }
            break;
        case 2:
            scale.vy = 0x1800;
            scale.vx = 0x1800;
            scale.vz = 0x1800;
            q = (frame << 12) / 12;
            b = 0x1000 - q;
            g = b;
            r = b;
            k = b;
            if (frame >= 0xD) {
                ex->btlfx_clearEffModels();
                return 1;
            }
            break;
        }
        work->scaleX4C = scale.vx;
        work->scaleZ50 = scale.vz;
        work->rot38.vz = 0;
        ex->Hi_DrawEffModel(&work->rot38, (VECTOR *)work->pos2C, (s32)&scale, work->modelC);
        ex->Hi_ModifyEffModelAbr(work->modelC, 1);
        ex->Hi_ModifyEffModelRGB(work->modelC, (r >> 5) - 0x80, (g >> 5) - 0x80, (b >> 5) - 0x80);
        work->rot38.vz = -work->frame48 * 0x18;
        ex->Hi_DrawEffModel(&work->rot38, (VECTOR *)work->pos2C, (s32)&scale, work->model10);
        ex->Hi_ModifyEffModelAbr(work->model10, 1);
        ex->Hi_ModifyEffModelRGB(work->model10, (k * 2 / 3 >> 5) - 0x80, (k * 2 / 3 >> 5) - 0x80, (k * 2 / 3 >> 5) - 0x80);
        work->rot38.vz = 0;
        ex->btlfx_drawBackdrop((BtlBgLayers *)ex->btlfx_getFileData(entry->file, 0xA), work->frame48);
        ex->btlfx_drawBackdrop((BtlBgLayers *)ex->btlfx_getFileData(entry->file, 0xD), work->frame48);
        work->frame48++;
    }
    }
    return 0;
}

INCLUDE_RODATA("asm/us/fx_04f/nonmatchings/fx_04f", fx_04f_headerPtr);
