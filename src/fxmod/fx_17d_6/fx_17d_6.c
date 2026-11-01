#include "fxmod/fx_17d_6.h"

s32 fx_17d_6_op80(s32 mode, Fx17d6Work80 *work, Unk800F78A4 *env, Unk80100C78Entry *entry, s32 *args) {
    BtlfxExports *ex = BTLFX_EXPORTS(env);
    VECTOR sc;
    VECTOR pos;
    SVECTOR ang;
    CVECTOR col;
    SVECTOR rot;
    VECTOR gridScale;
    VECTOR flashScale;
    SVECTOR cam;
    Pool6CEntry *emitter;
    s32 layer;
    s32 frame;
    s32 gridBright;
    s32 i;
    s32 t;

    if (mode == 0) {
        *args = 0x84;
    } else if (mode == 1) {
        work->state0 = 0;
        work->frame4 = 0;
        fx_17d_6_env = env;
        ex->btlfx_initParticleSystem();
        ex->btlfx_initEffModels(fx_17d_6_effModels, 3);
        work->model7C = ex->btlfx_registerEffectModel((Unk800E8F50 *)ex->btlfx_getFileData(entry->file, 0x8B), 1);
        work->model80 = ex->btlfx_registerEffectModel((Unk800E8F50 *)ex->btlfx_getFileData(entry->file, 0x8D), 2);
        ex->Hi_SetSummonMotion(SUMMON(env)->summon54->motion1A8, SUMMON(env)->model60);
        work->rot70.vx = 0x100;
        work->rot70.vy = 0x780;
        work->pos58.vz = work->pos58.vy = -0x2000;
        work->scale64.vx = work->scale64.vy = work->scale64.vz = 0x800;
        work->rot70.vz = 0;
        work->pos58.vx = 0;
        work->rot44.vx = 0;
        work->rot44.vy = 0x400;
        work->rot44.vz = 0;
        work->pos38.vx = fx_17d_6_basePos.vx;
        work->pos38.vy = fx_17d_6_basePos.vy;
        work->pos38.vz = fx_17d_6_basePos.vz;
        work->scale4C.vx = work->scale4C.vz = work->scale4C.vy = 0x1800;
        ((s32 *)&work->pos8)[0] = ((s32 *)&fx_17d_6_basePos)[0];
        work->pos38.vy -= 0x800;
        ((s32 *)&work->pos8)[1] = ((s32 *)&fx_17d_6_basePos)[1];
        work->pos8.vy -= 0x400;
        BTLFX_COPY_SVEC(&work->pos10, &work->pos8);
        work->pos18.vx = work->pos58.vx;
        work->pos18.vy = work->pos58.vy;
        work->pos18.vz = work->pos58.vz - 0x200;
        work->pos18.vy -= 0x200;
        env->camera->points[2].vx = work->pos18.vx;
        env->camera->points[2].vy = work->pos18.vy;
        env->camera->points[2].vz = work->pos18.vz;
        work->pos20.vx = fx_17d_6_basePos.vx;
        work->pos20.vy = fx_17d_6_basePos.vy;
        work->pos20.vz = fx_17d_6_basePos.vz;
        env->camera->points[3].vx = work->pos20.vx;
        env->camera->points[3].vy = work->pos20.vy;
        env->camera->points[3].vz = work->pos20.vz;
        work->pos58.vx += 0x19F;
        work->pos58.vy -= 0x200;
        emitter = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, 2), (s32)&work->pos10);
        work->emit78 = emitter;
        if (emitter != NULL) {
            emitter->otZ = 0x200;
        }
        fx_17d_6_boltState = 0;
    } else {
        frame = entry->frame;
        switch (work->state0) {
        case 0:
            t = (frame << 12) / 66;
            if (frame < 0x10) {
                ex->btlfx_drawScreenTint((u32 *)(env->ot + 2), 1, 0xFF - ((frame * 0xFF) >> 4),
                                         (0xFF - ((frame * 0xFF) >> 4)) * 2 / 3, (0xFF - ((frame * 0xFF) >> 4)) / 2);
            }
            if (frame < 4) {
                ex->btlfx_drawScreenTint((u32 *)(env->ot + 2), 1, 0xFF - ((frame * 0xFF) >> 2),
                                         0xFF - ((frame * 0xFF) >> 2), 0xFF - ((frame * 0xFF) >> 2));
            }
            if (frame < 0x1D) {
                ex->Hi_SetSummonMotFrame(SUMMON(env)->model60, 0);
            }
            pos.vx = work->pos58.vx;
            pos.vy = work->pos58.vy;
            pos.vz = work->pos58.vz;
            pos.vy += (ex->math_sin(t >> 2) * 0x6A4) >> 12;
            ex->Hi_DrawSummonModel(&work->rot70, &pos, (s32)&work->scale64, SUMMON(env)->model60, 0);
            ex->Hi_ModifySummonModelRGB(SUMMON(env)->model60, ex->btlfx_rand() & 0x1F, -0x20, -0x40);
            work->rot44.vy += 6;
            for (layer = 0; layer < 4; layer++) {
                sc.vx = sc.vy = sc.vz = (work->scale4C.vx * (layer + 1)) >> 1;
                BTLFX_COPY_SVEC(&ang, &work->rot44);
                ang.vy = ang.vy * (layer + 2) / 3;
                if (layer != 2) {
                    ex->Hi_DrawEffModel(&ang, (VECTOR *)&work->pos38, (s32)&sc, work->model7C);
                } else {
                    ex->Hi_DrawEffModel(&ang, (VECTOR *)&work->pos38, (s32)&sc, work->model80);
                }
            }
            ex->btlfx_drawScrollMove(0x3E8 - frame * 6, 0x280, 0x100, 0x20, 0x80, 0x2A0, 0x100);
            ex->Hi_GetSummonBonePos(SUMMON(env)->model60, fx_17d_6_bones[0], (u16 *)&work->pos28);
            ex->Hi_GetSummonBonePos(SUMMON(env)->model60, fx_17d_6_bones[2], (u16 *)&work->pos30);
            BTLFX_COPY_SVEC(&fx_17d_6_boltTo, &work->pos28);
            BTLFX_COPY_SVEC(&fx_17d_6_boltFrom, &work->pos30);
            env->otZ = 0x80;
            ex->btlfx_drawRings((u8 *)ex->btlfx_getFileData(entry->file, 6), &work->pos30, NULL, ex->math_sin(t >> 2) >> 3,
                              0, 1, ex->math_sin(t >> 2));
            env->otZ = 0;
            gridBright = (((0x1000 - t) << 5) >> 12) + 0x89;
            col.r = col.g = col.b = gridBright;
            gridScale.vy = gridScale.vx = 0x1080;
            rot.vx = 0;
            rot.vy = 0;
            rot.vz = 0;
            gridScale.vz = 0x1000;
            ex->btlfx_drawScreenGrid(1, 0xA0, 0x70, 0x50, 0x38, 4, 4, 2, 2, &rot, &gridScale, &col, 0, NULL);
            if (frame >= 0x42) {
                work->state0 = 1;
                entry->frame = -1;
                ex->btlfx_stopEmitter((Pool6CEntry *)&work->emit78);
                ex->Hi_SetSummonMotion(SUMMON(env)->summon54->motion1AC, SUMMON(env)->model60);
                pos.vx = work->pos58.vx;
                pos.vy = work->pos58.vy;
                pos.vz = work->pos58.vz;
                work->rot70.vx = 0;
                work->rot70.vy = 0x800;
                work->rot70.vz = 0;
                ex->Hi_GetSummonBonePos(SUMMON(env)->model60, fx_17d_6_bones[0], (u16 *)&work->pos18);
                env->camera->points[2].vx = work->pos18.vx;
                env->camera->points[2].vy = work->pos18.vy;
                env->camera->points[2].vz = work->pos18.vz;
                emitter = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, 4), (s32)&work->pos30);
                if (emitter != NULL) {
                    emitter->otZ = 0x80;
                }
            }
            break;
        case 1:
            t = (frame << 12) / 100;
            ex->Hi_DrawSummonModel(&work->rot70, (VECTOR *)&work->pos58, (s32)&work->scale64, SUMMON(env)->model60, 0);
            ex->Hi_GetSummonBonePos(SUMMON(env)->model60, fx_17d_6_bones[0], (u16 *)&work->pos28);
            ex->Hi_GetSummonBonePos(SUMMON(env)->model60, fx_17d_6_bones[2], (u16 *)&work->pos30);
            BTLFX_COPY_SVEC(&fx_17d_6_boltTo, &work->pos28);
            BTLFX_COPY_SVEC(&fx_17d_6_boltFrom, &work->pos30);
            ex->btlfx_lerpVec((s16 *)&work->pos28, (s16 *)&work->pos30, frame < 0x20 ? ex->math_sin(frame << 5) : 0x1000,
                              (s16 *)&work->pos8);
            env->otZ = 0x80;
            ex->btlfx_drawRings((u8 *)ex->btlfx_getFileData(entry->file, 6), &work->pos30, NULL,
                              (ex->math_sin(t >> 2) >> 1) + 0x400, 0, 1, 0xAAA);
            ex->btlfx_drawRings((u8 *)ex->btlfx_getFileData(entry->file, 6), &work->pos30, NULL,
                              (ex->math_sin(t >> 2) >> 2) + 0x80, 0, 1, 0x1000);
            ex->btlfx_drawRings((u8 *)ex->btlfx_getFileData(entry->file, 0xA), &work->pos30, NULL,
                              ex->math_cos(t >> 2) / 3 + 0x5DC, 0, 1, t);
            env->otZ = 0;
            BTLFX_COPY_SVEC(&work->pos18, &work->pos8);
            env->camera->points[2].vx = work->pos18.vx;
            env->camera->points[2].vy = work->pos18.vy;
            env->camera->points[2].vz = work->pos18.vz;
            if (frame == 0x18) {
                emitter = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, 1), (s32)&work->pos30);
                if (emitter != NULL) {
                    emitter->otZ = 0x80;
                }
            }
            if (frame == 0x20) {
                emitter = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, 5), (s32)&work->pos30);
                if (emitter != NULL) {
                    emitter->trails = (s32)fx_17d_6_op80Trails;
                    emitter->trailSlots = 0x28;
                    emitter->trailStride = 0xAC;
                    emitter->otZ = 0x80;
                }
            }
            if (frame == 0x32) {
                emitter = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, 0xE), (s32)&work->pos30);
                if (emitter != NULL) {
                    emitter->otZ = 0x80;
                }
            }
            if (frame >= 0x54) {
                col.r = col.g = col.b = (((frame - 0x54) << 15) >> 12) + 0x20;
                flashScale.vy = flashScale.vx = (t >> 1) + 0x1000;
                flashScale.vz = 0x1000;
                rot.vx = 0;
                rot.vy = 0;
                rot.vz = 0;
                ex->btlfx_drawScreenGrid(1, 0xA0, 0x70, 0x50, 0x38, 4, 4, 2, 2, &rot, &flashScale, &col, 1, NULL);
            }
            if (frame >= 0x64) {
                work->state0 = 2;
                entry->frame = -1;
                ex->btlfx_initParticleSystem();
                fx_17d_6_boltState = 2;
                BTLFX_COPY_SVEC(&work->pos18, &fx_17d_6_basePos);
                work->pos18.vy -= 0x200;
                env->camera->points[3].vx = work->pos18.vx;
                env->camera->points[3].vy = work->pos18.vy;
                env->camera->points[3].vz = work->pos18.vz;
                BTLFX_COPY_SVEC(&work->pos10, &work->pos18);
                work->pos10.vy -= 0x400;
            }
            break;
        case 2:
            t = (frame << 12) / 18;
            ex->btlfx_drawScreenTint((u32 *)(env->ot + 2), 2, 0, (0x40 - (frame << 6) / 18) >> 2,
                                     0x40 - (frame << 6) / 18);
            work->rot44.vy += 6;
            for (i = 0; i < 4; i++) {
                sc.vx = sc.vy = sc.vz = (work->scale4C.vx * (i + 1)) >> 1;
                BTLFX_COPY_SVEC(&ang, &work->rot44);
                ang.vy = ang.vy * (i + 2) / 3;
                if (i != 2) {
                    ex->Hi_DrawEffModel(&ang, (VECTOR *)&work->pos38, (s32)&sc, work->model7C);
                    ex->Hi_ModifyEffModelRGB(work->model7C, ((ex->math_cos(t >> 2) * 2 / 3) >> 5) - 0x80,
                                             ((ex->math_cos(t >> 2) * 2 / 3) >> 5) - 0x80,
                                             ((ex->math_cos(t >> 2) * 2 / 3) >> 5) - 0x80);
                } else {
                    ex->Hi_DrawEffModel(&ang, (VECTOR *)&work->pos38, (s32)&sc, work->model80);
                }
            }
            env->otZ = 0x9B;
            ex->btlfx_drawRings((u8 *)ex->btlfx_getFileData(entry->file, 0xB), &work->pos10, NULL, ex->math_sin(t >> 2) * 4,
                              0, 1, ex->math_sin(t >> 2));
            env->otZ = 0;
            ex->btlfx_drawSpriteLine((u8 *)ex->btlfx_getFileData(entry->file, 0xD), &work->pos10, (t >> 6) + 8, t / 2);
            ex->btlfx_drawScrollMove(0x3E8 - frame * 6, 0x280, 0x100, 0x20, 0x80, 0x2A0, 0x100);
            col.r = col.g = col.b = ((t * 0x58) >> 12) + 0x84U;
            gridScale.vy = gridScale.vx = 0x1080;
            gridScale.vz = 0x1000;
            rot.vx = 0;
            rot.vy = 0;
            rot.vz = 0;
            ex->btlfx_drawScreenGrid(1, 0xA0, 0x70, 0x50, 0x38, 4, 4, 2, 2, &rot, &gridScale, &col, 0, NULL);
            if (frame >= 0x12) {
                work->state0 = 3;
                entry->frame = -1;
                fx_17d_6_boltState = 0;
            }
            break;
        case 3:
            t = (frame << 12) / 14;
            ex->Hi_DrawSummonModel(&work->rot70, (VECTOR *)&work->pos58, (s32)&work->scale64, SUMMON(env)->model60, 0);
            ex->Hi_GetSummonBonePos(SUMMON(env)->model60, 0x21, (u16 *)&work->pos18);
            env->camera->points[2].vx = work->pos18.vx;
            env->camera->points[2].vy = work->pos18.vy;
            env->camera->points[2].vz = work->pos18.vz;
            ex->Hi_GetSummonBonePos(SUMMON(env)->model60, fx_17d_6_bones[0], (u16 *)&work->pos28);
            ex->Hi_GetSummonBonePos(SUMMON(env)->model60, fx_17d_6_bones[2], (u16 *)&work->pos30);
            BTLFX_COPY_SVEC(&fx_17d_6_boltTo, &work->pos28);
            BTLFX_COPY_SVEC(&fx_17d_6_boltFrom, &work->pos30);
            ex->Hi_GetSummonBonePos(SUMMON(env)->model60, fx_17d_6_bones[1], (u16 *)&work->pos28);
            BTLFX_COPY_SVEC(&fx_17d_6_boltFrom, &work->pos28);
            if (frame == 0) {
                emitter = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, 0xC), (s32)&work->pos28);
                if (emitter != NULL) {
                    emitter->otZ = 0x80;
                }
            }
            env->otZ = 0x80;
            ex->btlfx_drawRings((u8 *)ex->btlfx_getFileData(entry->file, 0xB), &work->pos28, NULL,
                              ex->math_sin(t >> 2) + 0x555, 0, 1, ex->math_sin(t >> 2) / 2 + 0x800);
            env->otZ = 0;
            col.r = col.g = col.b = ((t << 7) >> 12) + 0x20;
            gridScale.vy = gridScale.vx = (t >> 3) + 0x1000;
            gridScale.vz = 0x1000;
            rot.vx = 0;
            rot.vy = 0;
            rot.vz = 0;
            ex->btlfx_drawScreenGrid(1, 0xA0, 0x70, 0x50, 0x38, 4, 4, 2, 2, &rot, &gridScale, &col, 1, NULL);
            if (frame >= 0xE) {
                work->state0 = 4;
                cam.vx = fx_17d_6_basePos.vx;
                cam.vy = fx_17d_6_basePos.vy;
                cam.vz = fx_17d_6_basePos.vz;
                cam.vy -= 0x300;
                env->camera->points[3].vx = cam.vx;
                env->camera->points[3].vy = cam.vy;
                env->camera->points[3].vz = cam.vz;
                fx_17d_6_boltState = 1;
                ex->btlfx_clearEffModels();
                return 1;
            }
            break;
        }
        env->otZ = 0;
        work->frame4++;
    }
    return 0;
}

static void fxm_drawArcTrail(Unk80100C78Entry *entry, s32 file, s32 ot, Fx17d6Trail *trail, s32 n, s32 *noise, SVECTOR *from,
                   SVECTOR *to, s32 h0, s32 h1, s32 ang, s32 dang, s32 type, s32 alpha, s32 pad, s32 dip) {
    Unk800F78A4 *env = fx_17d_6_env;
    BtlfxExports *ex = BTLFX_EXPORTS(env);
    SVECTOR pos;
    SVECTOR rot;
    s32 i;
    s32 cnt;
    s32 t;
    s32 h;
    SVECTOR *pt;
    s32 a;

    ex->btlfx_aimAngles((s16 *)from, (s16 *)to, (s16 *)&rot);
    cnt = (n - 4) / 8;
    a = cnt;
    trail->lastSlot0 = trail->count2 = a - 1;
    trail->head1 = 0;
    pt = trail->points4;
    for (i = 0; i < a; i++, pt++) {
        t = 0x1000 - (i << 12) / (a - 1);
        t += *noise++ / 2;
        ex->btlfx_lerpVec((s16 *)from, (s16 *)to, t, (s16 *)&pos);
        h = *noise++ / 3;
        h = (ex->math_sin((t >> 1) >> 1) * h) >> 12;
        t >>= 1;
        h += ex->btlfx_lerp(h0, h1, ex->math_sin(t));
        ex->btlfx_offsetPoint(&pos, (s32)pt, 0, (XY32 *)&rot, h, ang);
        if (dip != 0) {
            pt->vy -= (ex->math_sin(t) * dip) >> 12;
        }
        pt->pad = pad;
        ang += dang + *noise++;
    }
    env->otZ = 0x80;
    ex->btlfx_drawTrail((u8 *)ex->btlfx_getFileData(entry->file, file), 0, NULL, type, 0x1000, alpha, 1, (u8 *)trail, ot);
    env->otZ = 0;
}

static void fxm_drawArcTrailBranch(Unk80100C78Entry *entry, s32 file, s32 ot, Fx17d6Trail *path, Fx17d6Trail *trail, s32 n, s32 *noise,
                   s32 i, s32 type, s32 alpha, s32 pad) {
    SVECTOR *pt = path->points4;
    BtlfxExports *ex = BTLFX_EXPORTS(fx_17d_6_env);
    SVECTOR mid;

    pt += i;
    ex->btlfx_lerpVec((s16 *)(pt + 1), (s16 *)pt, 0x4000, (s16 *)&mid);
    fxm_drawArcTrail(entry, file, ot, trail, n, noise, &mid, pt, 0, 0x20, ex->btlfx_rand(), 0x33, type, alpha, pad, 0);
}

static s32 fx_17d_6_op83PoolTask(s32 mode, Fx17d6Bolt *bolt, Unk800F78A4 *env, Unk80100C78Entry *entry, s32 frame) {
    BtlfxExports *ex = BTLFX_EXPORTS(env);
    s32 t;
    s32 remain;
    s32 alpha;
    s32 early;
    s32 slot;
    Fx17d6Trail *trail;
    Fx17d6Trail *branch;
    s32 width;

    if (mode != 1 && bolt->kind11 == 0) {
        t = (frame << 12) / 6;
        alpha = ex->math_cos(t >> 2);
        early = frame < 3;
        remain = 0x1000 - t;
        slot = bolt->slot10;
        width = bolt->width15;
        trail = (Fx17d6Trail *)&fx_17d_6_boltTrails[slot];
        branch = (Fx17d6Trail *)&fx_17d_6_branchTrails[slot];
        bolt->angle14 += 3;
        fxm_drawArcTrail(entry, 8, 0, trail, 0x8C, bolt->noise18 + frame * early, &bolt->from8, &bolt->to0, 0, 0xDC, bolt->angle14 * 16, 0xF0,
                      frame, alpha, remain * width / 1500, 0);
        fxm_drawArcTrailBranch(entry, 7, 0, trail, branch, 0x2C, bolt->noise18, bolt->index12, frame, alpha, (remain * width) >> 12);
        if (frame >= 6) {
            return 1;
        }
    }
    return 0;
}

s32 fx_17d_6_op83(s32 mode, Fx17d6Work83 *work, Unk800F78A4 *env, Fx17d6Inst *inst, EffInit *init) {
    BtlfxExports *ex = BTLFX_EXPORTS(env);
    s32 *noise;
    s32 i;
    s32 kind;
    s32 frame;
    Fx17d6Bolt *bolt;

    if (mode == 0) {
        init->workSize = 0xC;
        init->elemSize = 0x1C;
        init->elemFn = (s32)fx_17d_6_op83PoolTask;
        init->poolCount = 6;
    } else if (mode == 1) {
        kind = init->workSize;
        fx_17d_6_env = env;
        work->noise0 = fx_17d_6_noise[0];
        noise = fx_17d_6_noise[0];
        for (i = 0; i < 64; i++) {
            *noise++ = ex->btlfx_randCentered(0xD2);
            *noise++ = ex->btlfx_randCentered(0x1C3);
            *noise++ = ex->btlfx_randCentered(0xDC);
        }
        work->kind8 = kind;
        work->count4 = 0;
    } else {
        frame = inst->frame0;
        if (work->kind8 != 0) {
            return 0;
        }
        if (fx_17d_6_boltState == 0) {
            if (frame < 0x43 && (frame & 3)) {
                return 0;
            }
            work->count4++;
            bolt = ex->btlfx_poolSpawn(inst->pool10);
            if (bolt != NULL) {
                ((s32 *)&bolt->to0)[0] = ((s32 *)&fx_17d_6_boltTo)[0];
                ((s32 *)&bolt->to0)[1] = ((s32 *)&fx_17d_6_boltTo)[1];
                ((s32 *)&bolt->from8)[0] = ((s32 *)&fx_17d_6_boltFrom)[0];
                ((s32 *)&bolt->from8)[1] = ((s32 *)&fx_17d_6_boltFrom)[1];
                bolt->kind11 = 0;
                bolt->slot10 = work->count4 % 6;
                bolt->noise18 = work->noise0 + (ex->btlfx_rand() & 0x1F);
                bolt->index12 = ex->btlfx_rand() % 15;
                bolt->unk13 = ex->btlfx_rand() % 15;
                bolt->width15 = 0xB;
                bolt->angle14 = ex->btlfx_rand();
            }
        }
        if (fx_17d_6_boltState == 1) {
            ex->btlfx_poolClear((Unk800DA1B8 *)inst->pool10);
            return 1;
        }
    }
    return 0;
}

INCLUDE_RODATA("asm/us/fx_17d_6/nonmatchings/fx_17d_6", fx_17d_6_headerPtr);
