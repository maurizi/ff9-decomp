#include "fxmod/fx_1bf_2.h"

static s32 fx_1bf_2_op80PoolTask(s32 mode, Fx1bf2Obj *obj, Unk800F78A4 *env, s32 entry, s32 frame, s32 pool, Fx1bf2Inst *inst) {
    BtlfxExports *ex = BTLFX_EXPORTS(env);
    VECTOR pos;
    VECTOR scale;
    s32 ang;
    s32 drift;

    if (mode == 1) {
        obj->unk0 = 0;
    } else {
        ang = (frame << 7) >> 2;
        drift = (0x1000 - ex->math_cos(ang)) * fx_1bf_2_particleDrift;
        pos.vx = obj->posX4;
        pos.vy = obj->posY6;
        pos.vz = obj->posZ8;
        pos.vx -= drift >> 12;
        scale.vx = scale.vy = scale.vz = ((0x1080 - ex->math_sin(ang)) * obj->scale2) >> 12;
        ex->Hi_DrawEffModel(&obj->rotC, &pos, (s32)&scale, inst->model78);
        ex->Hi_ModifyEffModelRGB(inst->model78, (ex->math_sin(ang) >> 5) - 0x80, (ex->math_sin(ang) >> 5) - 0x80,
                                 (ex->math_sin(ang) >> 5) - 0x80);
        if (frame >= 0x20) {
            return 1;
        }
    }
    return 0;
}

INCLUDE_RODATA("asm/us/fx_1bf_2/nonmatchings/fx_1bf_2", fx_1bf_2_headerPtr);

s32 fx_1bf_2_op80(s32 mode, Fx1bf2Work80 *work, Unk800F78A4 *env, Unk80100C78Entry *entry, EffInit *init) {
    BtlfxExports *ex = BTLFX_EXPORTS(env);
    /* function-scope temporaries for the matrix restores: multiply set, so sched1 does not keep each load next to
     * its store (the original hoists the first restore's loads above the save) */
    s32 r0, r1, r2, r3;
    s32 w0, w1, w2, w3;
    VECTOR sc;
    CVECTOR cv;
    SVECTOR sv;
    VECTOR vv;
    CVECTOR cv2;
    SVECTOR sv2;
    VECTOR vv2;
    VECTOR vv3;
    Fx1bf2Obj *obj;
    Fx1bf2Obj *obj2;
    Pool6CEntry *emitter;
    s32 spriteX;
    s32 ang;
    s32 frame;
    s32 t;
    s32 fade2;
    s32 shade2;
    s32 fade3;
    s32 shade3;
    s32 bright;
    s32 slideX;

    if (mode == 0) {
        init->workSize = 0xBC;
        init->elemSize = 0x14;
        init->elemFn = (s32)fx_1bf_2_op80PoolTask;
        init->poolCount = 0x10;
    } else if (mode == 1) {
        work->state0 = 0;
        work->frame4 = 0;
        fx_1bf_2_env = env;
        ex->btlfx_initParticleSystem();
        ex->btlfx_initEffModels(fx_1bf_2_effModels, 7);
        work->model64 = ex->btlfx_registerEffectModel((Unk800E8F50 *)ex->btlfx_getFileData(entry->file, FX_ARK_PLANET), 0);
        work->model68 = ex->btlfx_registerEffectModel((Unk800E8F50 *)ex->btlfx_getFileData(entry->file, FX_ARK_RED_GLOW_DOME), 1);
        work->model6C = ex->btlfx_registerEffectModel((Unk800E8F50 *)ex->btlfx_getFileData(entry->file, FX_ARK_GREY_STAR_DISC), 1);
        work->model70 = ex->btlfx_registerEffectModel((Unk800E8F50 *)ex->btlfx_getFileData(entry->file, FX_ARK_ORANGE_BEAM), 1);
        work->model74 = ex->btlfx_registerEffectModel((Unk800E8F50 *)ex->btlfx_getFileData(entry->file, FX_ARK_BEAM_HAZE), 1);
        work->model78 = ex->btlfx_registerEffectModel((Unk800E8F50 *)ex->btlfx_getFileData(entry->file, FX_ARK_DARK_BLUE_RING), 1);
        BTLFX_COPY_SVEC(&work->pos8, &fx_1bf_2_basePos);
        work->unk34.vx = 0;
        work->unk34.vy = 0;
        work->unk34.vz = 0;
        work->scale3C.vx = work->scale3C.vz = work->scale3C.vy = 0x1000;
        work->pos28.vx = work->pos8.vx;
        work->pos28.vy = work->pos8.vy;
        work->pos28.vz = work->pos8.vz;
        work->camPos20.vx = fx_1bf_2_basePos.vx;
        work->camPos20.vy = fx_1bf_2_basePos.vy;
        work->camPos20.vz = fx_1bf_2_basePos.vz;
        env->camera->points[2].vx = work->camPos20.vx;
        env->camera->points[2].vy = work->camPos20.vy;
        env->camera->points[2].vz = work->camPos20.vz;
        work->rot5C.vx = 0;
        work->rot5C.vy = 0x400;
        work->rot5C.vz = 0;
    } else {
        frame = entry->frame;
        switch (work->state0) {
        case 0:
            t = frame << 7;
            ex->Hi_DrawEffModel(&work->rot5C, (VECTOR *)&work->pos28, (s32)&work->scale3C, work->model64);
            ex->Hi_ModifyEffModelRGB(work->model64, (t >> 5) - 0x80, (t >> 5) - 0x80, (t >> 5) - 0x80);
            spriteX = (((0x1000 - ex->math_sin(t >> 3)) << 6) >> 12) - 0x2C;
            ex->btlfx_drawSpriteAtOt(0xC00, (u8 *)ex->btlfx_getFileData(entry->file, FX_ARK_STARFIELD), spriteX, 0x78, 0, 0xFF, t);
            work->camPos20.vx -= 0x2E;
            env->camera->points[2].vx = work->camPos20.vx;
            env->camera->points[2].vy = work->camPos20.vy;
            env->camera->points[2].vz = work->camPos20.vz;
            if (frame >= 0x20) {
                work->state0 = 1;
                entry->frame = -1;
                ex->btlfx_pointAhead((s16 *)&work->pos8, (s16 *)&work->rot5C, -0x968, (s16 *)&work->pos54);
                work->pos48.vx = work->pos54.vx;
                work->pos48.vy = work->pos54.vy;
                work->pos48.vz = work->pos54.vz;
            }
            break;
        case 1:
            t = frame << 7;
            ex->Hi_DrawEffModel(&work->rot5C, (VECTOR *)&work->pos28, (s32)&work->scale3C, work->model68);
            ex->Hi_ModifyEffModelRGB(work->model68, ((t - 0x800) >> 5) - 0x80, ((t - 0x800) >> 5) - 0x80,
                                     ((t - 0x800) >> 5) - 0x80);
            ex->Hi_DrawEffModel(&work->rot5C, (VECTOR *)&work->pos28, (s32)&work->scale3C, work->model6C);
            ex->Hi_ModifyEffModelRGB(work->model6C, (t >> 5) - 0x80, (t >> 5) - 0x80, (t >> 5) - 0x80);
            ex->Hi_DrawEffModel(&work->rot5C, (VECTOR *)&work->pos28, (s32)&work->scale3C, work->model64);
            work->camPos20.vx -= (ex->math_cos(t >> 2) * 0x2E) >> 12;
            env->camera->points[2].vx = work->camPos20.vx;
            env->camera->points[2].vy = work->camPos20.vy;
            env->camera->points[2].vz = work->camPos20.vz;
            fx_1bf_2_particleDrift = 0x400;
            if (frame % 3 == 0) {
                obj = ex->btlfx_poolSpawn((s16 *)entry->pool);
                if (obj != NULL) {
                    obj->rotC.vx = 0x400;
                    obj->rotC.vy = 0x400;
                    obj->rotC.vz = 0;
                    obj->posX4 = work->pos48.vx;
                    obj->posY6 = work->pos48.vy;
                    obj->posZ8 = work->pos48.vz;
                    obj->scale2 = 0x1000;
                }
            }
            env->otZ = 0x200;
            ex->btlfx_drawRings((u8 *)ex->btlfx_getFileData(entry->file, FX_ARK_FIRE_RING), &work->pos54, NULL, t >> 1, 0, BTLFX_ABR_ADD, t);
            env->otZ = 0;
            slideX = (((0x1000 - ex->math_sin((t >> 3) + 0x200)) << 6) >> 12) - 0x2C;
            ex->btlfx_drawSpriteAtOt(0xC00, (u8 *)ex->btlfx_getFileData(entry->file, FX_ARK_STARFIELD), slideX, 0x78, 0, 0xFF, 0x1000);
            if (frame >= 0x20) {
                work->state0 = 2;
                entry->frame = -1;
            }
            break;
        case 2:
            t = (frame << 12) / 24;
            ex->Hi_DrawEffModel(&work->rot5C, (VECTOR *)&work->pos28, (s32)&work->scale3C, work->model68);
            fade2 = t / 2;
            ex->Hi_ModifyEffModelRGB(work->model68, ((0x800 - fade2) >> 5) - 0x80, ((0x800 - fade2) >> 5) - 0x80,
                                     ((0x800 - fade2) >> 5) - 0x80);
            ex->Hi_DrawEffModel(&work->rot5C, (VECTOR *)&work->pos28, (s32)&work->scale3C, work->model6C);
            shade2 = ((0x1000 - fade2) >> 5) - 0x80;
            ex->Hi_ModifyEffModelRGB(work->model6C, shade2, shade2, shade2);
            ex->Hi_DrawEffModel(&work->rot5C, (VECTOR *)&work->pos28, (s32)&work->scale3C, work->model64);
            sc.vx = sc.vy = (0x1000 - ex->math_sin(t >> 2)) / 3 + 0xF5;
            sc.vz = 0x800;
            ex->Hi_DrawEffModel(&work->rot5C, (VECTOR *)&work->pos48, (s32)&sc, work->model70);
            ex->Hi_DrawEffModel(&work->rot5C, (VECTOR *)&work->pos48, (s32)&sc, work->model74);
            fx_1bf_2_particleDrift = (t >> 1) + 0x400;
            if (frame % 3 == 0) {
                obj2 = ex->btlfx_poolSpawn((s16 *)entry->pool);
                if (obj2 != NULL) {
                    obj2->rotC.vx = 0x400;
                    obj2->rotC.vy = 0x400;
                    obj2->rotC.vz = 0;
                    obj2->posX4 = work->pos48.vx;
                    obj2->posY6 = work->pos48.vy;
                    obj2->posZ8 = work->pos48.vz;
                    obj2->scale2 = 0x1000;
                }
            }
            if (frame == 1) {
                work->pos10.vx = work->pos54.vx;
                work->pos10.vy = work->pos54.vy;
                work->pos10.vz = work->pos54.vz;
                work->target18.vx = work->pos54.vx;
                work->target18.vy = work->pos54.vy;
                work->target18.vz = work->pos54.vz;
                work->target18.vx -= 0xC00;
                emitter = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, FX_ARK_GOLD_SPARKS), (s32)&work->pos10);
                if (emitter != NULL) {
                    emitter->target = (s32)&work->target18;
                    emitter->otZ = 0x80;
                }
            }
            cv.r = cv.g = cv.b = 0x84;
            vv.vy = vv.vx = 0x1020;
            sv.vx = 0;
            sv.vy = 0;
            sv.vz = 0;
            vv.vz = 0x1000;
            ex->btlfx_drawScreenGrid(1, 0xA0, 0x70, 0x50, 0x38, 4, 4, 2, 2, &sv, &vv, &cv, 0, NULL);
            env->otZ = 0x200;
            ex->btlfx_drawRings((u8 *)ex->btlfx_getFileData(entry->file, FX_ARK_FIRE_RING), &work->pos54, NULL, (t >> 1) + 0x800, 0, BTLFX_ABR_ADD,
                              ex->math_cos(t >> 2));
            env->otZ = 0;
            ex->btlfx_drawSpriteAtOt(0xC00, (u8 *)ex->btlfx_getFileData(entry->file, FX_ARK_STARFIELD), -0x2C, 0x78, 0, 0xFF, 0x1000);
            if (frame >= 0x18) {
                work->state0 = 3;
                entry->frame = -1;
            }
            break;
        case 3:
            t = (frame << 12) / 24;
            ex->Hi_DrawEffModel(&work->rot5C, (VECTOR *)&work->pos28, (s32)&work->scale3C, work->model68);
            fade3 = t / 2;
            ex->Hi_ModifyEffModelRGB(work->model68, (-fade3 >> 5) - 0x80, (-fade3 >> 5) - 0x80, (-fade3 >> 5) - 0x80);
            ex->Hi_DrawEffModel(&work->rot5C, (VECTOR *)&work->pos28, (s32)&work->scale3C, work->model6C);
            shade3 = ((0x800 - fade3) >> 5) - 0x80;
            ex->Hi_ModifyEffModelRGB(work->model6C, shade3, shade3, shade3);
            ex->Hi_DrawEffModel(&work->rot5C, (VECTOR *)&work->pos28, (s32)&work->scale3C, work->model64);
            ang = t >> 2;
            sc.vx = sc.vy = ((0x1000 - ex->math_sin(ang)) * 0xF5) >> 12;
            sc.vz = 0x800;
            ex->Hi_DrawEffModel(&work->rot5C, (VECTOR *)&work->pos48, (s32)&sc, work->model70);
            ex->Hi_DrawEffModel(&work->rot5C, (VECTOR *)&work->pos48, (s32)&sc, work->model74);
            fx_1bf_2_particleDrift = ang + 0xC00;
            if (!(frame & 3) && frame < 0x18) {
                obj = ex->btlfx_poolSpawn((s16 *)entry->pool);
                if (obj != NULL) {
                    obj->rotC.vx = 0x400;
                    obj->rotC.vy = 0x400;
                    obj->rotC.vz = 0;
                    obj->posX4 = work->pos48.vx;
                    obj->posY6 = work->pos48.vy;
                    obj->posZ8 = work->pos48.vz;
                    obj->scale2 = 0x1000 - t;
                }
            }
            if (frame == 0x16) {
                {
                    s32 q0, q1, q2, q3;
                    q0 = ((s32 *)env->viewMatrix)[0];
                    q1 = ((s32 *)env->viewMatrix)[1];
                    q2 = ((s32 *)env->viewMatrix)[2];
                    q3 = ((s32 *)env->viewMatrix)[3];
                    ((s32 *)&work->viewMtx7C)[0] = q0;
                    ((s32 *)&work->viewMtx7C)[1] = q1;
                    ((s32 *)&work->viewMtx7C)[2] = q2;
                    ((s32 *)&work->viewMtx7C)[3] = q3;
                }
                {
                    s32 q0, q1, q2, q3;
                    q0 = ((s32 *)env->viewMatrix)[4];
                    q1 = ((s32 *)env->viewMatrix)[5];
                    q2 = ((s32 *)env->viewMatrix)[6];
                    q3 = ((s32 *)env->viewMatrix)[7];
                    ((s32 *)&work->viewMtx7C)[4] = q0;
                    ((s32 *)&work->viewMtx7C)[5] = q1;
                    ((s32 *)&work->viewMtx7C)[6] = q2;
                    ((s32 *)&work->viewMtx7C)[7] = q3;
                }
            }
            cv2.r = cv2.g = cv2.b = 0x84;
            vv2.vy = vv2.vx = 0x1020;
            sv2.vx = 0;
            sv2.vy = 0;
            sv2.vz = 0;
            vv2.vz = 0x1000;
            ex->btlfx_drawScreenGrid(1, 0xA0, 0x70, 0x50, 0x38, 4, 4, 2, 2, &sv2, &vv2, &cv2, 0, NULL);
            ex->btlfx_drawSpriteAtOt(0xC00, (u8 *)ex->btlfx_getFileData(entry->file, FX_ARK_STARFIELD), -0x2C, 0x78, 0, 0xFF, 0x1000);
            if (frame >= 0x18) {
                work->state0 = 4;
                entry->frame = -1;
                work->camPos20.vx = work->pos8.vx;
                work->camPos20.vy = work->pos8.vy;
                work->camPos20.vz = work->pos8.vz;
                work->camPos20.vy -= 0x200;
                env->camera->points[3].vx = work->camPos20.vx;
                env->camera->points[3].vy = work->camPos20.vy;
                env->camera->points[3].vz = work->camPos20.vz;
            }
            break;
        case 4:
            {
                s32 q0, q1, q2, q3;
                q0 = ((s32 *)env->viewMatrix)[0];
                q1 = ((s32 *)env->viewMatrix)[1];
                q2 = ((s32 *)env->viewMatrix)[2];
                q3 = ((s32 *)env->viewMatrix)[3];
                ((s32 *)&work->viewMtx9C)[0] = q0;
                ((s32 *)&work->viewMtx9C)[1] = q1;
                ((s32 *)&work->viewMtx9C)[2] = q2;
                ((s32 *)&work->viewMtx9C)[3] = q3;
            }
            {
                s32 q0, q1, q2, q3;
                q0 = ((s32 *)env->viewMatrix)[4];
                q1 = ((s32 *)env->viewMatrix)[5];
                q2 = ((s32 *)env->viewMatrix)[6];
                q3 = ((s32 *)env->viewMatrix)[7];
                ((s32 *)&work->viewMtx9C)[4] = q0;
                ((s32 *)&work->viewMtx9C)[5] = q1;
                ((s32 *)&work->viewMtx9C)[6] = q2;
                ((s32 *)&work->viewMtx9C)[7] = q3;
            }
            r0 = ((s32 *)&work->viewMtx7C)[0];
            r1 = ((s32 *)&work->viewMtx7C)[1];
            r2 = ((s32 *)&work->viewMtx7C)[2];
            r3 = ((s32 *)&work->viewMtx7C)[3];
            ((s32 *)env->viewMatrix)[0] = r0;
            ((s32 *)env->viewMatrix)[1] = r1;
            ((s32 *)env->viewMatrix)[2] = r2;
            ((s32 *)env->viewMatrix)[3] = r3;
            r0 = ((s32 *)&work->viewMtx7C)[4];
            r1 = ((s32 *)&work->viewMtx7C)[5];
            r2 = ((s32 *)&work->viewMtx7C)[6];
            r3 = ((s32 *)&work->viewMtx7C)[7];
            ((s32 *)env->viewMatrix)[4] = r0;
            ((s32 *)env->viewMatrix)[5] = r1;
            ((s32 *)env->viewMatrix)[6] = r2;
            ((s32 *)env->viewMatrix)[7] = r3;
            t = (frame << 12) / 20;
            ex->Hi_DrawEffModel(&work->rot5C, (VECTOR *)&work->pos28, (s32)&work->scale3C, work->model64);
            ex->Hi_ModifyEffModelAbr(work->model64, BTLFX_ABR_ADD);
            bright = 0x1000 - t;
            ex->Hi_ModifyEffModelRGB(work->model64, (bright >> 5) - 0x80, (bright >> 5) - 0x80, (bright >> 5) - 0x80);
            w0 = ((s32 *)&work->viewMtx9C)[0];
            w1 = ((s32 *)&work->viewMtx9C)[1];
            w2 = ((s32 *)&work->viewMtx9C)[2];
            w3 = ((s32 *)&work->viewMtx9C)[3];
            ((s32 *)env->viewMatrix)[0] = w0;
            ((s32 *)env->viewMatrix)[1] = w1;
            ((s32 *)env->viewMatrix)[2] = w2;
            ((s32 *)env->viewMatrix)[3] = w3;
            w0 = ((s32 *)&work->viewMtx9C)[4];
            w1 = ((s32 *)&work->viewMtx9C)[5];
            w2 = ((s32 *)&work->viewMtx9C)[6];
            w3 = ((s32 *)&work->viewMtx9C)[7];
            ((s32 *)env->viewMatrix)[4] = w0;
            ((s32 *)env->viewMatrix)[5] = w1;
            ((s32 *)env->viewMatrix)[6] = w2;
            ((s32 *)env->viewMatrix)[7] = w3;
            cv2.r = cv2.g = cv2.b = 0x84;
            sv2.vx = 0;
            sv2.vy = 0;
            sv2.vz = 0;
            vv3.vy = vv3.vx = 0x1020;
            vv3.vz = 0x1000;
            ex->btlfx_drawScreenGrid(1, 0xA0, 0x70, 0x50, 0x38, 4, 4, 2, 2, &sv2, &vv3, &cv2, 0, NULL);
            ex->btlfx_drawSpriteAtOt(0xC00, (u8 *)ex->btlfx_getFileData(entry->file, FX_ARK_STARFIELD), -0x2C, 0x78, 0, 1, bright);
            if (frame >= 0x14) {
                ex->btlfx_poolClear(entry->pool);
                ex->btlfx_clearEffModels();
                return 1;
            }
            break;
        }
        ex->btlfx_drawScrollMove(work->frame4 * 4, 0x2C0, 0x100, 0x20, 0x80, 0x2E0, 0x100);
        ex->btlfx_drawScrollMove(0x2710 - work->frame4 * 4, 0x300, 0x100, 0x20, 0x80, 0x320, 0x100);
        ex->btlfx_drawLayerTexture((BtlBgLayers *)ex->btlfx_getFileData(entry->file, FX_ARK_BACKDROP), work->frame4);
        work->frame4++;
    }
    return 0;
}
