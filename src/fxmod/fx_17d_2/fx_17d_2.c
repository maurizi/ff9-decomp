#include "fxmod/fx_17d_2.h"

s32 func_801E7704(s32 mode, Fx17d2Elem *e, Unk800F78A4 *env, Unk80100C78Entry *entry, s32 frame, Unk800DA1B8 *pool,
                  Fx17d2Work *work) {
    BtlfxExports *ex = BTLFX_EXPORTS(env);
    VECTOR pos;
    VECTOR sc;
    s32 t;
    s32 s;

    if (mode == 1) {
        e->unk0 = 0;
    } else {
    t = frame << 6;
    s = (((ex->math_sin(t >> 2) >> 1) + 0x1000) * e->unk4) >> 12;
    sc.vz = 0x400;
    sc.vx = sc.vy = s;
    e->unk10.vz -= e->unk2;
    e->unk8.vy += 0x3E;
    pos.vx = e->unk8.vx;
    pos.vy = e->unk8.vy;
    pos.vz = e->unk8.vz;
    ex->Hi_DrawEffModel(&e->unk10, &pos, (s32)&sc, work->unk74);
    ex->Hi_ModifyEffModelRGB(work->unk74, (((ex->math_cos(t >> 1) >> 2) + 0xC00) >> 5) - 0x80,
                             (((ex->math_cos(t >> 1) >> 2) + 0xC00) >> 5) - 0x80,
                             (((ex->math_cos(t >> 1) >> 2) + 0xC00) >> 5) - 0x80);
    if (frame >= 0x40) {
        return 1;
    }
    }
    return 0;
}

s32 fx_17d_2_op80(s32 mode, Fx17d2Work *work, Unk800F78A4 *env, Unk80100C78Entry *entry, EffInit *arg4) {
    BtlfxExports *ex = BTLFX_EXPORTS(env);
    VECTOR s3;
    CVECTOR col;
    SVECTOR r;
    VECTOR sc;
    Fx17d2Elem *e;
    s32 frame;
    s32 t;
    s32 d;
    s32 c;
    s32 i;
    s32 y;

    if (mode == 0) {
        arg4->workSize = 0x80;
        arg4->elemSize = 0x18;
        arg4->elemFn = (s32)func_801E7704;
        arg4->poolCount = 0x14;
    } else if (mode == 1) {
        work->unk0 = 0;
        work->unk4 = 0;
        D_801E8048 = env;
        ex->btlfx_initParticleSystem();
        ex->btlfx_initEffModels(D_801E8058, 5);
        work->unk78 = ex->btlfx_registerEffectModel((Unk800E8F50 *)ex->btlfx_getFileData(entry->file, 7), 1);
        work->unk68 = ex->btlfx_registerEffectModel((Unk800E8F50 *)ex->btlfx_getFileData(entry->file, 0xB), 1);
        work->unk6C = ex->btlfx_registerEffectModel((Unk800E8F50 *)ex->btlfx_getFileData(entry->file, 5), 1);
        work->unk70 = ex->btlfx_registerEffectModel((Unk800E8F50 *)ex->btlfx_getFileData(entry->file, 0xA), 1);
        work->unk74 = ex->btlfx_registerEffectModel((Unk800E8F50 *)ex->btlfx_getFileData(entry->file, 2), 1);
        work->unk28[0] = 0;
        work->unk28[1] = -0x2000;
        work->unk28[2] = 0;
        work->unk8.vx = 0;
        work->unk8.vy = 0;
        work->unk8.vz = 0;
        work->unk40.vx = 0x400;
        work->unk34[0] = work->unk34[1] = 0x333;
        work->unk40.vy = 0;
        work->unk40.vz = 0;
        work->unk34[2] = 0xC00;
        work->unk54.vx = 0x400;
        work->unk54.vy = 0;
        work->unk54.vz = 0;
        work->unk5C[0] = work->unk5C[1] = work->unk5C[2] = 0xC00;
        work->unk48[0] = work->unk28[0];
        work->unk48[1] = work->unk28[1];
        work->unk48[2] = work->unk28[2];
        work->unk48[1] -= 0x5000;
        BTLFX_COPY_SVEC(&work->unk18, &work->unk8);
        work->unk18.vy -= 0x1000;
        env->camera->points[3].vx = work->unk18.vx;
        env->camera->points[3].vy = work->unk18.vy;
        env->camera->points[3].vz = work->unk18.vz;
        work->unk7C = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, 3), (s32)&work->unk10);
        if (work->unk7C != NULL) {
            work->unk7C->unk22 = 0x80;
        }
        for (i = 0; i < 0x14; i++) {
            y = i * 0x140 - 0x400;
            e = ex->btlfx_poolSpawn((s16 *)entry->pool);
            if (e != NULL) {
                e->unk8.vx = work->unk28[0];
                e->unk8.vz = work->unk28[2];
                e->unk8.vy = work->unk28[1] - y;
                e->unk10.vx = -0x400;
                e->unk10.vy = 0;
                e->unk10.vz = i * 0x300;
                e->unk4 = i * 0xA0 + 0x199;
                e->unk2 = 0x10 - i * 2;
            }
        }
    } else {
        frame = entry->frame;
        if (work->unk0 == 0) {
            work->unk54.vz += 2;
            t = (frame << 12) / 60;
            ex->Hi_DrawEffModel(&work->unk54, (VECTOR *)work->unk48, (s32)work->unk5C, work->unk78);
            work->unk28[1] += 0x46;
            ex->Hi_SetEffModelOffset(work->unk68, 0x80);
            ex->Hi_DrawEffModel(&work->unk40, (VECTOR *)work->unk28, (s32)work->unk34, work->unk68);
            ex->Hi_ModifyEffModelRGB(work->unk68, ((0x1000 - t) >> 5) - 0x80, ((0x1000 - t) >> 5) - 0x80,
                                     ((0x1000 - t) >> 5) - 0x80);
            work->unk40.vz -= frame * 0xC;
            ex->Hi_DrawEffModel(&work->unk40, (VECTOR *)work->unk28, (s32)work->unk34, work->unk6C);
            ex->Hi_ModifyEffModelRGB(work->unk6C, (t >> 5) - 0x80, (t >> 5) - 0x80, (t >> 5) - 0x80);
            work->unk40.vz += frame * 0xC;
            work->unk10.vx = work->unk28[0];
            work->unk10.vy = work->unk28[1];
            work->unk10.vz = work->unk28[2];
            s3.vx = s3.vy = s3.vz = t / 5 + 0x333;
            ex->Hi_DrawEffModel(&work->unk40, (VECTOR *)work->unk28, (s32)&s3, work->unk70);
            env->otZ = 0x100;
            ex->btlfx_drawRings((u8 *)ex->btlfx_getFileData(entry->file, 1), &work->unk10, NULL, 0xAAA, 0, 1, -1);
            env->otZ = 0;
            ex->btlfx_drawScrollMove(frame * 10, 0x180, 0x100, 0x40, 0x80, 0x180, 0x180);
            ex->btlfx_drawScrollMove(frame * 7, 0x240, 0x100, 0x40, 0x80, 0x280, 0x100);
            ex->btlfx_drawSpriteAtOt(0xC00, (u8 *)ex->btlfx_getFileData(entry->file, 8), frame / 3 + 0x10E, 0x40 - frame / 3,
                              0, 0xFF, -1);
            ex->btlfx_drawSpriteAtOt(0xE00, (u8 *)ex->btlfx_getFileData(entry->file, 9), frame / 3 + 0xA0, 0x70 - frame / 3,
                              0, 0xFF, 0x1000 - t / 2);
            col.r = col.g = col.b = 0x84;
            sc.vy = sc.vx = (frame >> 1) + 0x1000;
            r.vx = 0;
            r.vy = 0;
            r.vz = 0;
            sc.vz = 0x1000;
            ex->btlfx_drawScreenGrid(1, 0xA0, 0x70, 0x50, 0x38, 4, 4, 2, 2, &r, &sc, &col, 0, 0);
            if (work->unk7C != NULL && work->unk7C->state == -1) {
                work->unk7C = NULL;
            }
            if (work->unk7C != NULL) {
                work->unk7C->unk5C.vx = work->unk40.vx;
                work->unk7C->unk5C.vy = work->unk40.vy;
                work->unk7C->unk5C.vz = work->unk40.vz;
            }
            if (frame >= 0x3C) {
                ex->btlfx_poolClear((Unk800DA1B8 *)entry->pool);
                ex->btlfx_clearEffModels();
                return 1;
            }
        }
        env->otZ = 0;
        work->unk4++;
    }
    return 0;
}

INCLUDE_RODATA("asm/us/fx_17d_2/nonmatchings/fx_17d_2", fx_17d_2_headerPtr);
