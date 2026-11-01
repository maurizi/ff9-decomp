#include "fxmod/fx_17d_6.h"

s32 fx_17d_6_op80(s32 mode, Fx17d6Work80 *work, Unk800F78A4 *env, Unk80100C78Entry *entry, s32 *args) {
    BtlfxExports *ex = BTLFX_EXPORTS(env);
    VECTOR sc;
    VECTOR pos;
    SVECTOR ang;
    CVECTOR col;
    SVECTOR rot;
    VECTOR vv;
    VECTOR vv2;
    SVECTOR cam;
    Pool6CEntry *emitter;
    s32 n1;
    s32 frame;
    s32 c;
    s32 i;
    s32 u;

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
            u = (frame << 12) / 66;
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
            pos.vy += (ex->math_sin(u >> 2) * 0x6A4) >> 12;
            ex->Hi_DrawSummonModel(&work->rot70, &pos, (s32)&work->scale64, SUMMON(env)->model60, 0);
            ex->Hi_ModifySummonModelRGB(SUMMON(env)->model60, ex->btlfx_rand() & 0x1F, -0x20, -0x40);
            work->rot44.vy += 6;
            for (n1 = 0; n1 < 4; n1++) {
                sc.vx = sc.vy = sc.vz = (work->scale4C.vx * (n1 + 1)) >> 1;
                BTLFX_COPY_SVEC(&ang, &work->rot44);
                ang.vy = ang.vy * (n1 + 2) / 3;
                if (n1 != 2) {
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
            ex->btlfx_drawRings((u8 *)ex->btlfx_getFileData(entry->file, 6), &work->pos30, NULL, ex->math_sin(u >> 2) >> 3,
                              0, 1, ex->math_sin(u >> 2));
            env->otZ = 0;
            c = (((0x1000 - u) << 5) >> 12) + 0x89;
            col.r = col.g = col.b = c;
            vv.vy = vv.vx = 0x1080;
            rot.vx = 0;
            rot.vy = 0;
            rot.vz = 0;
            vv.vz = 0x1000;
            ex->btlfx_drawScreenGrid(1, 0xA0, 0x70, 0x50, 0x38, 4, 4, 2, 2, &rot, &vv, &col, 0, NULL);
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
            u = (frame << 12) / 100;
            ex->Hi_DrawSummonModel(&work->rot70, (VECTOR *)&work->pos58, (s32)&work->scale64, SUMMON(env)->model60, 0);
            ex->Hi_GetSummonBonePos(SUMMON(env)->model60, fx_17d_6_bones[0], (u16 *)&work->pos28);
            ex->Hi_GetSummonBonePos(SUMMON(env)->model60, fx_17d_6_bones[2], (u16 *)&work->pos30);
            BTLFX_COPY_SVEC(&fx_17d_6_boltTo, &work->pos28);
            BTLFX_COPY_SVEC(&fx_17d_6_boltFrom, &work->pos30);
            ex->btlfx_lerpVec((s16 *)&work->pos28, (s16 *)&work->pos30, frame < 0x20 ? ex->math_sin(frame << 5) : 0x1000,
                              (s16 *)&work->pos8);
            env->otZ = 0x80;
            ex->btlfx_drawRings((u8 *)ex->btlfx_getFileData(entry->file, 6), &work->pos30, NULL,
                              (ex->math_sin(u >> 2) >> 1) + 0x400, 0, 1, 0xAAA);
            ex->btlfx_drawRings((u8 *)ex->btlfx_getFileData(entry->file, 6), &work->pos30, NULL,
                              (ex->math_sin(u >> 2) >> 2) + 0x80, 0, 1, 0x1000);
            ex->btlfx_drawRings((u8 *)ex->btlfx_getFileData(entry->file, 0xA), &work->pos30, NULL,
                              ex->math_cos(u >> 2) / 3 + 0x5DC, 0, 1, u);
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
                vv2.vy = vv2.vx = (u >> 1) + 0x1000;
                vv2.vz = 0x1000;
                rot.vx = 0;
                rot.vy = 0;
                rot.vz = 0;
                ex->btlfx_drawScreenGrid(1, 0xA0, 0x70, 0x50, 0x38, 4, 4, 2, 2, &rot, &vv2, &col, 1, NULL);
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
            u = (frame << 12) / 18;
            ex->btlfx_drawScreenTint((u32 *)(env->ot + 2), 2, 0, (0x40 - (frame << 6) / 18) >> 2,
                                     0x40 - (frame << 6) / 18);
            work->rot44.vy += 6;
            for (i = 0; i < 4; i++) {
                sc.vx = sc.vy = sc.vz = (work->scale4C.vx * (i + 1)) >> 1;
                BTLFX_COPY_SVEC(&ang, &work->rot44);
                ang.vy = ang.vy * (i + 2) / 3;
                if (i != 2) {
                    ex->Hi_DrawEffModel(&ang, (VECTOR *)&work->pos38, (s32)&sc, work->model7C);
                    ex->Hi_ModifyEffModelRGB(work->model7C, ((ex->math_cos(u >> 2) * 2 / 3) >> 5) - 0x80,
                                             ((ex->math_cos(u >> 2) * 2 / 3) >> 5) - 0x80,
                                             ((ex->math_cos(u >> 2) * 2 / 3) >> 5) - 0x80);
                } else {
                    ex->Hi_DrawEffModel(&ang, (VECTOR *)&work->pos38, (s32)&sc, work->model80);
                }
            }
            env->otZ = 0x9B;
            ex->btlfx_drawRings((u8 *)ex->btlfx_getFileData(entry->file, 0xB), &work->pos10, NULL, ex->math_sin(u >> 2) * 4,
                              0, 1, ex->math_sin(u >> 2));
            env->otZ = 0;
            ex->btlfx_drawSpriteLine((u8 *)ex->btlfx_getFileData(entry->file, 0xD), &work->pos10, (u >> 6) + 8, u / 2);
            ex->btlfx_drawScrollMove(0x3E8 - frame * 6, 0x280, 0x100, 0x20, 0x80, 0x2A0, 0x100);
            col.r = col.g = col.b = ((u * 0x58) >> 12) + 0x84U;
            vv.vy = vv.vx = 0x1080;
            vv.vz = 0x1000;
            rot.vx = 0;
            rot.vy = 0;
            rot.vz = 0;
            ex->btlfx_drawScreenGrid(1, 0xA0, 0x70, 0x50, 0x38, 4, 4, 2, 2, &rot, &vv, &col, 0, NULL);
            if (frame >= 0x12) {
                work->state0 = 3;
                entry->frame = -1;
                fx_17d_6_boltState = 0;
            }
            break;
        case 3:
            u = (frame << 12) / 14;
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
                              ex->math_sin(u >> 2) + 0x555, 0, 1, ex->math_sin(u >> 2) / 2 + 0x800);
            env->otZ = 0;
            col.r = col.g = col.b = ((u << 7) >> 12) + 0x20;
            vv.vy = vv.vx = (u >> 3) + 0x1000;
            vv.vz = 0x1000;
            rot.vx = 0;
            rot.vy = 0;
            rot.vz = 0;
            ex->btlfx_drawScreenGrid(1, 0xA0, 0x70, 0x50, 0x38, 4, 4, 2, 2, &rot, &vv, &col, 1, NULL);
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

static void fxm_drawArcTrail(Unk80100C78Entry *entry, s32 file, s32 mode, Fx17d6Trail *trail, s32 size, s32 *noise, SVECTOR *from,
                   SVECTOR *to, s32 amp0, s32 amp1, s32 ang, s32 dang, s32 frame, s32 fade, s32 lineWidth, s32 dip) {
    Unk800F78A4 *env = fx_17d_6_env;
    BtlfxExports *ex = BTLFX_EXPORTS(env);
    SVECTOR pos;
    SVECTOR rot;
    s32 i;
    s32 count;
    s32 t;
    s32 offset;
    SVECTOR *pt;
    s32 numPts;

    ex->btlfx_aimAngles((s16 *)from, (s16 *)to, (s16 *)&rot);
    count = (size - 4) / 8;
    numPts = count;
    trail->lastSlot0 = trail->count2 = numPts - 1;
    trail->head1 = 0;
    pt = trail->points4;
    for (i = 0; i < numPts; i++, pt++) {
        t = 0x1000 - (i << 12) / (numPts - 1);
        t += *noise++ / 2;
        ex->btlfx_lerpVec((s16 *)from, (s16 *)to, t, (s16 *)&pos);
        offset = *noise++ / 3;
        offset = (ex->math_sin((t >> 1) >> 1) * offset) >> 12;
        t >>= 1;
        offset += ex->btlfx_lerp(amp0, amp1, ex->math_sin(t));
        ex->btlfx_offsetPoint(&pos, (s32)pt, 0, (XY32 *)&rot, offset, ang);
        if (dip != 0) {
            pt->vy -= (ex->math_sin(t) * dip) >> 12;
        }
        pt->pad = lineWidth;
        ang += dang + *noise++;
    }
    env->otZ = 0x80;
    ex->btlfx_drawTrail((u8 *)ex->btlfx_getFileData(entry->file, file), 0, NULL, frame, 0x1000, fade, 1, (u8 *)trail, mode);
    env->otZ = 0;
}

static void fxm_drawArcTrailBranch(Unk80100C78Entry *entry, s32 file, s32 mode, Fx17d6Trail *path, Fx17d6Trail *trail, s32 size, s32 *noise,
                   s32 branchAt, s32 frame, s32 fade, s32 lineWidth) {
    SVECTOR *pt = path->points4;
    BtlfxExports *ex = BTLFX_EXPORTS(fx_17d_6_env);
    SVECTOR mid;

    pt += branchAt;
    ex->btlfx_lerpVec((s16 *)(pt + 1), (s16 *)pt, 0x4000, (s16 *)&mid);
    fxm_drawArcTrail(entry, file, mode, trail, size, noise, &mid, pt, 0, 0x20, ex->btlfx_rand(), 0x33, frame, fade, lineWidth, 0);
}

static s32 fx_17d_6_op83PoolTask(s32 mode, Fx17d6Bolt *b, Unk800F78A4 *env, Unk80100C78Entry *entry, s32 frame) {
    BtlfxExports *ex = BTLFX_EXPORTS(env);
    s32 t;
    s32 a;
    s32 c;
    s32 k;
    s32 i;
    Fx17d6Trail *tr;
    Fx17d6Trail *tr2;
    s32 u;

    if (mode != 1 && b->kind11 == 0) {
        t = (frame << 12) / 6;
        c = ex->math_cos(t >> 2);
        k = frame < 3;
        a = 0x1000 - t;
        i = b->slot10;
        u = b->width15;
        tr = (Fx17d6Trail *)&fx_17d_6_boltTrails[i];
        tr2 = (Fx17d6Trail *)&fx_17d_6_branchTrails[i];
        b->angle14 += 3;
        fxm_drawArcTrail(entry, 8, 0, tr, 0x8C, b->noise18 + frame * k, &b->from8, &b->to0, 0, 0xDC, b->angle14 * 16, 0xF0,
                      frame, c, a * u / 1500, 0);
        fxm_drawArcTrailBranch(entry, 7, 0, tr, tr2, 0x2C, b->noise18, b->index12, frame, c, (a * u) >> 12);
        if (frame >= 6) {
            return 1;
        }
    }
    return 0;
}

s32 fx_17d_6_op83(s32 mode, Fx17d6Work83 *work, Unk800F78A4 *env, Fx17d6Inst *inst, EffInit *init) {
    BtlfxExports *ex = BTLFX_EXPORTS(env);
    s32 *p;
    s32 i;
    s32 n;
    s32 frame;
    Fx17d6Bolt *b;

    if (mode == 0) {
        init->workSize = 0xC;
        init->elemSize = 0x1C;
        init->elemFn = (s32)fx_17d_6_op83PoolTask;
        init->poolCount = 6;
    } else if (mode == 1) {
        n = init->workSize;
        fx_17d_6_env = env;
        work->noise0 = fx_17d_6_noise[0];
        p = fx_17d_6_noise[0];
        for (i = 0; i < 64; i++) {
            *p++ = ex->btlfx_randCentered(0xD2);
            *p++ = ex->btlfx_randCentered(0x1C3);
            *p++ = ex->btlfx_randCentered(0xDC);
        }
        work->kind8 = n;
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
            b = ex->btlfx_poolSpawn(inst->pool10);
            if (b != NULL) {
                ((s32 *)&b->to0)[0] = ((s32 *)&fx_17d_6_boltTo)[0];
                ((s32 *)&b->to0)[1] = ((s32 *)&fx_17d_6_boltTo)[1];
                ((s32 *)&b->from8)[0] = ((s32 *)&fx_17d_6_boltFrom)[0];
                ((s32 *)&b->from8)[1] = ((s32 *)&fx_17d_6_boltFrom)[1];
                b->kind11 = 0;
                b->slot10 = work->count4 % 6;
                b->noise18 = work->noise0 + (ex->btlfx_rand() & 0x1F);
                b->index12 = ex->btlfx_rand() % 15;
                b->unk13 = ex->btlfx_rand() % 15;
                b->width15 = 0xB;
                b->angle14 = ex->btlfx_rand();
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
