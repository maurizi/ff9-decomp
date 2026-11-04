#include "fxmod/fx_17d_2.h"

static s32 fx_17d_2_op80PoolTask(s32 mode, Fx17d2Elem *it, Unk800F78A4 *env, Unk80100C78Entry *entry, s32 frame, Unk800DA1B8 *pool,
                  Fx17d2Work *work) {
    BtlfxExports *ex = BTLFX_EXPORTS(env);
    VECTOR pos;
    VECTOR sc;
    s32 t;
    s32 scale;

    if (mode == 1) {
        it->unk0 = 0;
    } else {
    t = frame << 6;
    scale = (((ex->math_sin(t >> 2) >> 1) + 0x1000) * it->scale4) >> 12;
    sc.vz = 0x400;
    sc.vx = sc.vy = scale;
    it->rot10.vz -= it->spin2;
    it->pos8.vy += 0x3E;
    pos.vx = it->pos8.vx;
    pos.vy = it->pos8.vy;
    pos.vz = it->pos8.vz;
    ex->Hi_DrawEffModel(&it->rot10, &pos, (s32)&sc, work->model74);
    ex->Hi_ModifyEffModelRGB(work->model74, (((ex->math_cos(t >> 1) >> 2) + 0xC00) >> 5) - 0x80,
                             (((ex->math_cos(t >> 1) >> 2) + 0xC00) >> 5) - 0x80,
                             (((ex->math_cos(t >> 1) >> 2) + 0xC00) >> 5) - 0x80);
    if (frame >= 0x40) {
        return 1;
    }
    }
    return 0;
}

s32 fx_17d_2_op80(s32 mode, Fx17d2Work *work, Unk800F78A4 *env, Unk80100C78Entry *entry, EffInit *init) {
    BtlfxExports *ex = BTLFX_EXPORTS(env);
    VECTOR coreScale;
    CVECTOR col;
    SVECTOR rot;
    VECTOR sc;
    Fx17d2Elem *elem;
    s32 frame;
    s32 t;
    s32 unused;
    s32 unused2;
    s32 i;
    s32 drop;

    if (mode == 0) {
        init->workSize = 0x80;
        init->elemSize = 0x18;
        init->elemFn = (s32)fx_17d_2_op80PoolTask;
        init->poolCount = 0x14;
    } else if (mode == 1) {
        work->state0 = 0;
        work->frame4 = 0;
        fx_17d_2_env = env;
        ex->btlfx_initParticleSystem();
        ex->btlfx_initEffModels(fx_17d_2_effModels, 5);
        work->model78 = ex->btlfx_registerEffectModel((Unk800E8F50 *)ex->btlfx_getFileData(entry->file, FX_ARK_BLUE_ENERGY_SHELL), 1);
        work->model68 = ex->btlfx_registerEffectModel((Unk800E8F50 *)ex->btlfx_getFileData(entry->file, FX_ARK_SMOKE_CONE), 1);
        work->model6C = ex->btlfx_registerEffectModel((Unk800E8F50 *)ex->btlfx_getFileData(entry->file, FX_ARK_FLAME_SPIRAL_CONE), 1);
        work->model70 = ex->btlfx_registerEffectModel((Unk800E8F50 *)ex->btlfx_getFileData(entry->file, FX_ARK_FLAME_SPIRAL_DISH), 1);
        work->model74 = ex->btlfx_registerEffectModel((Unk800E8F50 *)ex->btlfx_getFileData(entry->file, FX_ARK_THIN_ARCH), 1);
        work->pos28[0] = 0;
        work->pos28[1] = -0x2000;
        work->pos28[2] = 0;
        work->pos8.vx = 0;
        work->pos8.vy = 0;
        work->pos8.vz = 0;
        work->rot40.vx = 0x400;
        work->scale34[0] = work->scale34[1] = 0x333;
        work->rot40.vy = 0;
        work->rot40.vz = 0;
        work->scale34[2] = 0xC00;
        work->rot54.vx = 0x400;
        work->rot54.vy = 0;
        work->rot54.vz = 0;
        work->scale5C[0] = work->scale5C[1] = work->scale5C[2] = 0xC00;
        work->pos48[0] = work->pos28[0];
        work->pos48[1] = work->pos28[1];
        work->pos48[2] = work->pos28[2];
        work->pos48[1] -= 0x5000;
        BTLFX_COPY_SVEC(&work->pos18, &work->pos8);
        work->pos18.vy -= 0x1000;
        env->camera->points[3].vx = work->pos18.vx;
        env->camera->points[3].vy = work->pos18.vy;
        env->camera->points[3].vz = work->pos18.vz;
        work->emit7C = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, FX_ARK_ORANGE_SPARKLES), (s32)&work->pos10);
        if (work->emit7C != NULL) {
            work->emit7C->otZ = 0x80;
        }
        for (i = 0; i < 0x14; i++) {
            drop = i * 0x140 - 0x400;
            elem = ex->btlfx_poolSpawn((s16 *)entry->pool);
            if (elem != NULL) {
                elem->pos8.vx = work->pos28[0];
                elem->pos8.vz = work->pos28[2];
                elem->pos8.vy = work->pos28[1] - drop;
                elem->rot10.vx = -0x400;
                elem->rot10.vy = 0;
                elem->rot10.vz = i * 0x300;
                elem->scale4 = i * 0xA0 + 0x199;
                elem->spin2 = 0x10 - i * 2;
            }
        }
    } else {
        frame = entry->frame;
        if (work->state0 == 0) {
            work->rot54.vz += 2;
            t = (frame << 12) / 60;
            ex->Hi_DrawEffModel(&work->rot54, (VECTOR *)work->pos48, (s32)work->scale5C, work->model78);
            work->pos28[1] += 0x46;
            ex->Hi_SetEffModelOffset(work->model68, 0x80);
            ex->Hi_DrawEffModel(&work->rot40, (VECTOR *)work->pos28, (s32)work->scale34, work->model68);
            ex->Hi_ModifyEffModelRGB(work->model68, ((0x1000 - t) >> 5) - 0x80, ((0x1000 - t) >> 5) - 0x80,
                                     ((0x1000 - t) >> 5) - 0x80);
            work->rot40.vz -= frame * 0xC;
            ex->Hi_DrawEffModel(&work->rot40, (VECTOR *)work->pos28, (s32)work->scale34, work->model6C);
            ex->Hi_ModifyEffModelRGB(work->model6C, (t >> 5) - 0x80, (t >> 5) - 0x80, (t >> 5) - 0x80);
            work->rot40.vz += frame * 0xC;
            work->pos10.vx = work->pos28[0];
            work->pos10.vy = work->pos28[1];
            work->pos10.vz = work->pos28[2];
            coreScale.vx = coreScale.vy = coreScale.vz = t / 5 + 0x333;
            ex->Hi_DrawEffModel(&work->rot40, (VECTOR *)work->pos28, (s32)&coreScale, work->model70);
            env->otZ = 0x100;
            ex->btlfx_drawRings((u8 *)ex->btlfx_getFileData(entry->file, FX_ARK_FIRE_RING), &work->pos10, NULL, 0xAAA, 0, 1, -1);
            env->otZ = 0;
            ex->btlfx_drawScrollMove(frame * 10, 0x180, 0x100, 0x40, 0x80, 0x180, 0x180);
            ex->btlfx_drawScrollMove(frame * 7, 0x240, 0x100, 0x40, 0x80, 0x280, 0x100);
            ex->btlfx_drawSpriteAtOt(0xC00, (u8 *)ex->btlfx_getFileData(entry->file, FX_ARK_GREEN_GLOW_SPRITE), frame / 3 + 0x10E, 0x40 - frame / 3,
                              0, 0xFF, -1);
            ex->btlfx_drawSpriteAtOt(0xE00, (u8 *)ex->btlfx_getFileData(entry->file, FX_ARK_STATIC_SPRITE), frame / 3 + 0xA0, 0x70 - frame / 3,
                              0, 0xFF, 0x1000 - t / 2);
            col.r = col.g = col.b = 0x84;
            sc.vy = sc.vx = (frame >> 1) + 0x1000;
            rot.vx = 0;
            rot.vy = 0;
            rot.vz = 0;
            sc.vz = 0x1000;
            ex->btlfx_drawScreenGrid(1, 0xA0, 0x70, 0x50, 0x38, 4, 4, 2, 2, &rot, &sc, &col, 0, 0);
            if (work->emit7C != NULL && work->emit7C->state == BTLFX_EMIT_DONE) {
                work->emit7C = NULL;
            }
            if (work->emit7C != NULL) {
                work->emit7C->rot.vx = work->rot40.vx;
                work->emit7C->rot.vy = work->rot40.vy;
                work->emit7C->rot.vz = work->rot40.vz;
            }
            if (frame >= 0x3C) {
                ex->btlfx_poolClear((Unk800DA1B8 *)entry->pool);
                ex->btlfx_clearEffModels();
                return 1;
            }
        }
        env->otZ = 0;
        work->frame4++;
    }
    return 0;
}

INCLUDE_RODATA("asm/us/fx_17d_2/nonmatchings/fx_17d_2", fx_17d_2_headerPtr);
