#include "fxmod/fx_17d_7.h"

static s32 fxm_randRingPoint(SVECTOR *out, SVECTOR *rot) {
    BtlfxExports *ex = BTLFX_EXPORTS(fx_17d_7_env);
    s32 ang = ex->btlfx_rand();

    out->pad = 0;
    out->vy = 0;
    out->vx = (ex->math_cos(ang) * fx_17d_7_ringRadius) >> 12;
    out->vz = (ex->math_sin(ang) * fx_17d_7_ringRadius) >> 12;
    rot->vy = ang;
    return 0;
}

static s32 fx_17d_7_op80PoolTask(s32 mode, Fx17d7Rock *rock, Unk800F78A4 *env, s32 entry, s32 frame, s32 pool, Fx17d7RockInst *inst) {
    BtlfxExports *ex = BTLFX_EXPORTS(env);
    VECTOR pos;
    VECTOR scale;
    s32 t;
    s32 size;

    if (mode == 1) {
        rock->phase2 = 0;
        rock->spin1C = ex->btlfx_rand() % 64;
        rock->spin1E = ex->btlfx_rand() % 32;
    } else {
        switch (rock->state0) {
        case FX_ARK_ROCK_RISE:
            switch (rock->phase2) {
            case 0:
                pos.vx = rock->posC.vx;
                pos.vy = rock->posC.vy;
                pos.vz = rock->posC.vz;
                pos.vx += ex->btlfx_randCentered(frame + 0x4D);
                pos.vy += ex->btlfx_randCentered(frame * 2 + 0x4D);
                size = rock->scaleA;
                scale.vx = scale.vz = size;
                scale.vy = size * 3;
                ex->Hi_DrawEffModel(&rock->rot14, &pos, (s32)&scale, inst->model84[rock->index20]);
                ex->Hi_ModifyEffModelRGB(inst->model84[rock->index20], -0x80, -0x80, -0x80);
                if (fx_17d_7_releaseLevel < rock->release22) {
                    rock->phase2 = 1;
                    ((s16 *)rock)[-1] = -1;
                }
                break;
            case 1:
                t = (frame << 12) / 24;
                if (rock->posC.vx > 0) {
                    rock->spin1C -= (0xE10 - rock->posC.vz) / 300;
                } else {
                    rock->spin1C += (rock->posC.vz + 0xE10) / 300;
                }
                rock->rot14.vx += rock->spin1C;
                rock->rot14.vy += rock->spin1E;
                rock->posC.vx += rock->vel4;
                rock->posC.vy += rock->vel6;
                rock->posC.vz += rock->vel8;
                pos.vx = rock->posC.vx;
                pos.vy = rock->posC.vy;
                pos.vz = rock->posC.vz;
                size = ((0x1000 - ex->math_sin(t >> 2)) * rock->scaleA) >> 12;
                scale.vx = scale.vz = size;
                scale.vy = size * 3;
                ex->Hi_DrawEffModel(&rock->rot14, &pos, (s32)&scale, inst->model84[rock->index20]);
                ex->Hi_ModifyEffModelRGB(inst->model84[rock->index20], -0x80, -0x80, -0x80);
                if (frame >= FX_ARK_ROCK_RISE_FRAMES) {
                    return 1;
                }
                break;
            }
            break;
        case FX_ARK_ROCK_FLY:
            t = frame << 8;
            rock->rot14.vx += rock->spin1C;
            rock->rot14.vy += rock->spin1E;
            rock->posC.vx += rock->vel4;
            rock->posC.vy += rock->vel6;
            rock->posC.vz += rock->vel8;
            pos.vx = rock->posC.vx;
            pos.vy = rock->posC.vy;
            pos.vz = rock->posC.vz;
            size = ((0x1000 - t) * rock->scaleA) >> 12;
            scale.vx = scale.vz = size;
            scale.vy = size * 3;
            ex->Hi_DrawEffModel(&rock->rot14, &pos, (s32)&scale, inst->model84[rock->index20]);
            ex->Hi_ModifyEffModelRGB(inst->model84[rock->index20], -0x80, -0x80, -0x80);
            if (frame >= FX_ARK_ROCK_FLY_FRAMES) {
                return 1;
            }
            break;
        }
    }
    return 0;
}

s32 fx_17d_7_op80(s32 mode, Fx17d7Work80 *work, Unk800F78A4 *env, Unk80100C78Entry *entry, EffInit *init) {
    BtlfxExports *ex = BTLFX_EXPORTS(env);
    VECTOR sc;
    VECTOR pos;
    SVECTOR rot;
    SVECTOR pillarPos;
    s32 frame;
    s32 t;
    s32 domeLevel;
    s32 gridGrow;
    s32 gridBright;
    s32 introTint;
    s32 burstTint;
    s32 glowTint;
    s32 whiteoutTint;
    s32 fadeTint;
    s32 beamWidth;
    s32 beamLevel;
    s32 burstT;
    s32 burstAng;
    s32 domeRgb;
    s32 i;
    s32 rowZ;
    s32 rubbleSize;
    Pool6CEntry *emitter;
    Fx17d7Rock *rock;
    s16 *rockX;
    s32 shake;
    s32 rumble;
    s32 unused;
    s32 unused2;
    Fx17d7Rock *boulder;
    Fx17d7Rock *rubble;

    if (mode == 0) {
        init->workSize = sizeof(Fx17d7Work80);
        init->elemSize = sizeof(Fx17d7Rock);
        init->elemFn = (s32)fx_17d_7_op80PoolTask;
        init->poolCount = FX_ARK_ROCK_POOL;
    } else if (mode == 1) {
        work->state0 = FX_ARK_STATE_SMOKE;
        work->frame4 = 0;
        fx_17d_7_env = env;
        ex->btlfx_initParticleSystem();
        ex->btlfx_initEffModels(fx_17d_7_effModels, FX_ARK_MODEL_COUNT);
        work->model70 = ex->btlfx_registerEffectModel((Unk800E8F50 *)ex->btlfx_getFileData(entry->file, FX_ARK_SMOKE_BOWL), 0);
        work->model74 = ex->btlfx_registerEffectModel((Unk800E8F50 *)ex->btlfx_getFileData(entry->file, FX_ARK_TEAL_RING), 0);
        work->model78 = ex->btlfx_registerEffectModel((Unk800E8F50 *)ex->btlfx_getFileData(entry->file, FX_ARK_ORANGE_BEAM), 1);
        work->model7C = ex->btlfx_registerEffectModel((Unk800E8F50 *)ex->btlfx_getFileData(entry->file, FX_ARK_FIRE_CONE), 1);
        work->model80 = ex->btlfx_registerEffectModel((Unk800E8F50 *)ex->btlfx_getFileData(entry->file, FX_ARK_FLAME_TEX_RING), 1);
        work->model84 = ex->btlfx_registerEffectModel((Unk800E8F50 *)ex->btlfx_getFileData(entry->file, FX_ARK_ROCK), 0);
        work->model88 = ex->btlfx_registerEffectModel((Unk800E8F50 *)ex->btlfx_getFileData(entry->file, FX_ARK_ROCK_2), 0);
        work->model8C = ex->btlfx_registerEffectModel((Unk800E8F50 *)ex->btlfx_getFileData(entry->file, FX_ARK_ROCK_3), 0);
        work->model90 = ex->btlfx_registerEffectModel((Unk800E8F50 *)ex->btlfx_getFileData(entry->file, FX_ARK_BLUE_CLOUD_RING), 1);
        work->model94 = ex->btlfx_registerEffectModel((Unk800E8F50 *)ex->btlfx_getFileData(entry->file, FX_ARK_GREY_TORUS), 1);
        work->model98 = ex->btlfx_registerEffectModel((Unk800E8F50 *)ex->btlfx_getFileData(entry->file, FX_ARK_FIRE_ARC), 1);
        work->model9C = ex->btlfx_registerEffectModel((Unk800E8F50 *)ex->btlfx_getFileData(entry->file, FX_ARK_EMBER_RING), 1);
        work->modelA0 = ex->btlfx_registerEffectModel((Unk800E8F50 *)ex->btlfx_getFileData(entry->file, FX_ARK_GOLD_RING), 1);
        work->rot34.vx = 0;
        work->rot34.vy = 0x320;
        work->rot34.vz = 0;
        work->rot3C.vx = 0;
        work->rot3C.vy = 0;
        work->rot3C.vz = 0;
        work->pos28[0] = fx_17d_7_basePos.vx;
        work->pos28[1] = fx_17d_7_basePos.vy;
        work->pos28[2] = fx_17d_7_basePos.vz;
        work->scale44[1] = 0x1800;
        work->scale44[2] = 0x1800;
        work->scale44[0] = 0x1800;
        BTLFX_COPY_SVEC(&work->pos8, &fx_17d_7_basePos);
        work->pos8.vy -= 0x200;
        work->pos18.vx = fx_17d_7_basePos.vx;
        work->pos18.vy = fx_17d_7_basePos.vy;
        work->pos18.vz = fx_17d_7_basePos.vz;
        work->pos18.vy -= 0x300;
        env->camera->points[3].vx = work->pos18.vx;
        env->camera->points[3].vy = work->pos18.vy;
        env->camera->points[3].vz = work->pos18.vz;
    } else {
        domeLevel = 0;
        gridGrow = 0;
        gridBright = 0;
        frame = entry->frame;
        switch (work->state0) {
        case FX_ARK_STATE_SMOKE:
            t = frame << 8;
            if (frame < 18) {
                introTint = 0xFF - frame * 0xFF / 18;
                ex->btlfx_drawScreenTint((u32 *)(env->ot + 2), BTLFX_ABR_ADD, introTint, introTint * 2 / 3, introTint / 2);
                if (frame < 2) {
                    ex->btlfx_drawScreenTint((u32 *)(env->ot + 1), BTLFX_ABR_ADD, 0xFF, 0xFF, 0xFF);
                }
            }
            work->pos18.vy -= 4 + ((ex->math_cos(t >> 2) * 10) >> 12);
            env->camera->points[3].vx = work->pos18.vx;
            env->camera->points[3].vy = work->pos18.vy;
            env->camera->points[3].vz = work->pos18.vz;
            domeLevel = 0x1000;
            if (frame >= FX_ARK_SMOKE_FRAMES) {
                work->state0 = FX_ARK_STATE_RINGS;
                entry->frame = -1;
                work->rot5C.vx = -0x400;
                work->rot5C.vy = 0x400;
                work->rot5C.vz = 0;
                work->pos50[0] = work->pos8.vx;
                work->pos50[1] = work->pos8.vy;
                work->pos50[2] = work->pos8.vz;
                BTLFX_COPY_SVEC(&fx_17d_7_boltBase, &work->pos8);
                BTLFX_COPY_SVEC(&fx_17d_7_boltTop, &work->pos8);
                fx_17d_7_boltTop.vy -= 0xDAC;
            }
            break;
        case FX_ARK_STATE_RINGS: {
            VECTOR beamScale;

            t = frame << 8;
            if (frame < 6) {
                burstT = (frame << 12) / 6;
                beamWidth = burstT >> 2;
                beamLevel = burstT;
            } else {
                burstT = ((frame - 6) << 12) / 10;
                beamWidth = 0xC00 - ((burstT << 11) >> 13);
                beamLevel = 0x1000;
                if (frame == 6) {
                    ex->btlfx_drawScreenTint((u32 *)(env->ot + 1), BTLFX_ABR_SUB, 0xFF, 0x80, 0x80);
                } else {
                    burstTint = (burstT << 7) >> 12;
                    ex->btlfx_drawScreenTint((u32 *)(env->ot + 1), BTLFX_ABR_ADD, burstTint, burstTint, burstTint * 2 / 3);
                }
                burstAng = burstT >> 2;
                gridBright = (burstT << 6) >> 12;
                sc.vx = sc.vz = ex->math_sin(burstAng) * 2 + 0x3000;
                sc.vy = ex->math_sin(burstAng) * 4;
                pos.vx = work->pos8.vx;
                pos.vy = work->pos8.vy;
                pos.vz = work->pos8.vz;
                rot.vx = 0;
                rot.vy = burstT >> 3;
                rot.vz = 0;
                gridGrow = 0x300;
                ex->Hi_DrawEffModel(&rot, &pos, (s32)&sc, work->model7C);
                ex->Hi_ModifyEffModelRGB(work->model7C, (ex->math_cos(burstAng) >> 5) - 0x80, (ex->math_cos(burstAng) >> 5) - 0x80,
                                         (ex->math_cos(burstAng) >> 5) - 0x80);
                ex->btlfx_drawScrollMove(frame * 12, FX_ARK_SCROLL_VRAM_X, FX_ARK_SCROLL_VRAM_Y, FX_ARK_SCROLL_VRAM_W, FX_ARK_SCROLL_VRAM_H, FX_ARK_SCROLL_SRC_X, FX_ARK_SCROLL_SRC_Y);
            }
            beamScale.vx = beamScale.vy = beamWidth;
            beamScale.vz = 0x800;
            ex->Hi_DrawEffModel(&work->rot5C, (VECTOR *)work->pos50, (s32)&beamScale, work->model78);
            ex->Hi_ModifyEffModelRGB(work->model78, (beamLevel >> 5) - 0x80, (beamLevel >> 5) - 0x80, (beamLevel >> 5) - 0x80);
            if (frame == 6) {
                work->emit64 = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, FX_ARK_PALE_RING), (s32)&work->pos8);
                if (work->emit64 != NULL) {
                    work->emit64->otZ = 0x80;
                }
            }
            work->pos18.vy -= 3;
            env->camera->points[3].vx = work->pos18.vx;
            env->camera->points[3].vy = work->pos18.vy;
            domeLevel = 0x1001 - t;
            env->camera->points[3].vz = work->pos18.vz;
            if (frame >= FX_ARK_RINGS_FRAMES) {
                work->state0 = FX_ARK_STATE_BLAST;
                entry->frame = -1;
                BTLFX_COPY_SVEC(&work->pos10, &work->pos8);
                work->pos10.vy -= 0x800;
                work->emit68 = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, FX_ARK_VIOLET_RING), (s32)&work->pos10);
                if (work->emit68 != NULL) {
                    work->emit68->otZ = 0x80;
                }
            }
            break;
        }
        case FX_ARK_STATE_BLAST: {
            VECTOR beamScale;
            SVECTOR cam;
            SVECTOR cam2;
            s32 riseFrame;
            s32 rise;
            s32 ringFrame;
            s32 cloudScale;
            s32 flare;
            s32 flareLift;

            t = (frame << 12) / 70;
            if (frame < 5) {
                beamLevel = 0x800;
                rise = (frame << 12) / 5;
                if (work->emit68 != NULL && work->emit68->state == BTLFX_EMIT_DONE) {
                    work->emit68 = NULL;
                }
                BTLFX_COPY_SVEC(&pillarPos, &work->pos8);
                pillarPos.vy -= ((0x1000 - rise) << 11) >> 12;
                ex->btlfx_moveEmitterTo((s32)work->emit68, (s32)&pillarPos);
                sc.vx = sc.vz = beamLevel * 4;
                rot.vx = 0;
                rot.vy = rise >> 3;
                rot.vz = 0;
                pos.vx = pillarPos.vx;
                pos.vy = pillarPos.vy;
                pos.vz = pillarPos.vz;
                sc.vy = (beamLevel * (rise + 0x400)) >> 8;
                ex->Hi_DrawEffModel(&rot, &pos, (s32)&sc, work->model7C);
                ex->btlfx_drawScrollMove(frame * 12, FX_ARK_SCROLL_VRAM_X, FX_ARK_SCROLL_VRAM_Y, FX_ARK_SCROLL_VRAM_W, FX_ARK_SCROLL_VRAM_H, FX_ARK_SCROLL_SRC_X, FX_ARK_SCROLL_SRC_Y);
                if (frame == 1 || frame == 3) {
                    ex->btlfx_drawScreenTint((u32 *)(env->ot + 2), BTLFX_ABR_ADD, 0x80, 0x60, 0x10);
                }
            } else {
                riseFrame = frame - 5;
                rise = (riseFrame << 12) / 65;
                beamLevel = ((0x1000 - rise) * 0x1800) >> 12;
                if (frame < 21) {
                    flare = 0x1000 - ex->math_cos((riseFrame << 10) >> 4);
                    pos.vx = work->pos8.vx;
                    pos.vy = work->pos8.vy;
                    pos.vz = work->pos8.vz;
                    flareLift = flare >> 2;
                    pos.vy -= flareLift;
                    rot.vx = 0;
                    rot.vy = frame << 5;
                    rot.vz = 0;
                    sc.vx = sc.vz = (flare * 5) >> 2;
                    sc.vy = 0x1000;
                    ex->Hi_DrawEffModel(&rot, &pos, (s32)&sc, work->modelA0);
                    ex->Hi_ModifyEffModelRGB(work->modelA0, ((ex->math_cos(flareLift) - 0x800) >> 5) - 0x80,
                                             ((ex->math_cos(flareLift) - 0x800) >> 5) - 0x80,
                                             ((ex->math_cos(flareLift) - 0x800) >> 5) - 0x80);
                }
                if (frame == 5) {
                    ex->btlfx_drawScreenTint((u32 *)(env->ot + 1), BTLFX_ABR_SUB, 0x80, 0x80, 0x80);
                    emitter = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, FX_ARK_ROCK_DEBRIS_2), (s32)&work->pos8);
                    if (emitter != NULL) {
                        emitter->otZ = 0x100;
                    }
                    emitter = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, FX_ARK_LIGHT_STREAKS), (s32)&work->pos8);
                    if (emitter != NULL) {
                        emitter->otZ = 0x100;
                    }
                    BTLFX_COPY_SVEC(&work->pos10, &work->pos8);
                    work->pos10.vy -= 0x400;
                    emitter = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, FX_ARK_GOLD_GLOW), (s32)&work->pos10);
                    if (emitter != NULL) {
                        emitter->tail->shape = -1;
                        *(s32 *)&emitter->tail->s.module.spawnFn = (s32)fxm_randRingPoint;
                        emitter->otZ = 0x100;
                    }
                } else if (frame < 21) {
                    glowTint = 0x80 - ((riseFrame << 7) >> 4);
                    ex->btlfx_drawScreenTint((u32 *)(env->ot + 16), BTLFX_ABR_ADD, glowTint, glowTint, glowTint >> 2);
                    gridBright = glowTint >> 1;
                    gridGrow = 0x400;
                }
            }
            beamScale.vx = beamScale.vy = beamLevel;
            beamScale.vz = 0x800;
            ex->Hi_DrawEffModel(&work->rot5C, (VECTOR *)work->pos50, (s32)&beamScale, work->model78);
            ex->Hi_ModifyEffModelRGB(work->model78, (ex->math_cos(t >> 2) >> 5) - 0x80, (ex->math_cos(t >> 2) >> 5) - 0x80,
                                     (ex->math_cos(t >> 2) >> 5) - 0x80);
            work->pos18.vy -= 2;
            env->camera->points[3].vx = work->pos18.vx;
            env->camera->points[3].vy = work->pos18.vy;
            domeLevel = ((0x1000 - t) >> 1) + 1;
            env->camera->points[3].vz = work->pos18.vz;
            if (frame >= 6) {
                ringFrame = frame - 6;
                sc.vx = sc.vy = sc.vz = ringFrame * 0x60;
                pos.vx = work->pos8.vx;
                pos.vy = work->pos8.vy;
                pos.vz = work->pos8.vz;
                pos.vy -= 0x400;
                rot.vy = frame << 2;
                rot.vx = 0;
                rot.vz = 0;
                fx_17d_7_ringRadius = (sc.vx * 0xA28) >> 12;
                ex->Hi_DrawEffModel(&rot, &pos, (s32)&sc, work->model80);
                ex->btlfx_drawScrollMove(frame * 8, FX_ARK_SCROLL_2_VRAM_X, FX_ARK_SCROLL_2_VRAM_Y, FX_ARK_SCROLL_2_VRAM_W, FX_ARK_SCROLL_2_VRAM_H, FX_ARK_SCROLL_2_SRC_X, FX_ARK_SCROLL_2_SRC_Y);
                cloudScale = ringFrame * 0x122B / 64;
                sc.vx = sc.vy = sc.vz = cloudScale;
                pos.vx = work->pos8.vx;
                pos.vy = work->pos8.vy;
                pos.vz = work->pos8.vz;
                rot.vx = 0;
                rot.vy = frame * 8;
                rot.vz = 0;
                ex->Hi_DrawEffModel(&rot, &pos, (s32)&sc, work->model90);
                ex->Hi_ModifyEffModelRGB(work->model90, -0x30, -0x30, -0x30);
                rot.vy = 0;
                ex->Hi_DrawEffModel(&rot, &pos, (s32)&sc, work->model94);
                ex->Hi_ModifyEffModelRGB(work->model94, -0x20, -0x20, -0x20);
                sc.vx = sc.vy = sc.vz = cloudScale / 2;
                rot.vx = 0;
                rot.vy = frame << 4;
                rot.vz = 0;
                ex->Hi_DrawEffModel(&rot, &pos, (s32)&sc, work->model90);
                ex->btlfx_drawScrollMove(frame * ((t >> 9) + 12), FX_ARK_SCROLL_3_VRAM_X, FX_ARK_SCROLL_3_VRAM_Y, FX_ARK_SCROLL_3_VRAM_W, FX_ARK_SCROLL_3_VRAM_H, FX_ARK_SCROLL_3_SRC_X, FX_ARK_SCROLL_3_SRC_Y);
                rot.vx = (frame << 4) + 0x200;
                rot.vy = frame << 5;
                rot.vz = 0;
                sc.vx = sc.vy = sc.vz = cloudScale;
                ex->Hi_DrawEffModel(&rot, &pos, (s32)&sc, work->model98);
                ex->Hi_ModifyEffModelRGB(work->model98, -0x20, -0x20, -0x20);
                rot.vx = (frame << 5) - 0x200;
                rot.vy = frame << 6;
                rot.vz = 0;
                sc.vx = sc.vy = sc.vz = cloudScale * 2 / 3;
                ex->Hi_DrawEffModel(&rot, &pos, (s32)&sc, work->model98);
                ex->btlfx_drawScrollMove(frame * ((t >> 8) + 8), FX_ARK_SCROLL_4_VRAM_X, FX_ARK_SCROLL_4_VRAM_Y, FX_ARK_SCROLL_4_VRAM_W, FX_ARK_SCROLL_4_VRAM_H, FX_ARK_SCROLL_4_SRC_X, FX_ARK_SCROLL_4_SRC_Y);
            }
            if (frame >= 35) {
                s32 ringT;

                env->otZ = 0x100;
                ringT = ((frame - 35) << 12) / 35;
                ex->btlfx_drawRings((u8 *)ex->btlfx_getFileData(entry->file, FX_ARK_PEACH_RING), &work->pos8, NULL, ringT + 0xAAA, 0, BTLFX_ABR_ADD, ringT);
                env->otZ = 0;
            }
            if (frame >= 58) {
                whiteoutTint = (frame - 58) * 0xFF / 12;
                ex->btlfx_drawScreenTint((u32 *)(env->ot + 256), BTLFX_ABR_ADD, whiteoutTint, whiteoutTint, whiteoutTint * 2 / 3);
            }
            if (frame == 41) {
                emitter = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, FX_ARK_ROCK_DEBRIS), (s32)&work->pos8);
                if (emitter != NULL) {
                    emitter->otZ = 0x100;
                }
            }
            if (frame >= 6 && frame < 14) {
                shake = ex->math_cos(((frame - 6) << 10) >> 3) >> 7;
                BTLFX_COPY_SVEC(&cam, &work->pos18);
                cam.vy += ex->btlfx_randCentered(shake);
                cam.vx += ex->btlfx_randCentered(shake);
                env->camera->points[3].vx = cam.vx;
                env->camera->points[3].vy = cam.vy;
                env->camera->points[3].vz = cam.vz;
            }
            if (frame >= 35) {
                rumble = 0x1000 - ex->math_cos(((frame - 35) << 10) / 35);
                rumble >>= 6;
                BTLFX_COPY_SVEC(&cam2, &work->pos18);
                cam2.vy += ex->btlfx_randCentered(rumble);
                cam2.vx += ex->btlfx_randCentered(rumble);
                env->camera->points[3].vx = cam2.vx;
                env->camera->points[3].vy = cam2.vy;
                env->camera->points[3].vz = cam2.vz;
            }
            if (frame == 58) {
                BTLFX_COPY_SVEC(&work->pos10, &work->pos8);
                work->pos10.vz -= 0x5DC;
                work->pos10.vy -= 0x1AE;
                work->emit6C = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, FX_ARK_ORANGE_RING), (s32)&work->pos10);
                if (work->emit6C != NULL) {
                    work->emit6C->otZ = 0x200;
                }
            }
            if (frame >= FX_ARK_BLAST_FRAMES) {
                work->state0 = FX_ARK_STATE_ROCKS;
                entry->frame = -1;
            }
            break;
        }
        case FX_ARK_STATE_ROCKS: {
            s32 t;
            s32 rowScale;
            s32 bigScale;
            s32 rubbleZ;

            t = (frame << 12) / 41;
            if (work->emit6C != NULL) {
                if (work->emit6C->state == BTLFX_EMIT_DONE) {
                    work->emit6C = NULL;
                }
                if (work->emit6C != NULL) {
                    work->emit6C->otZ = -10 - ((t << 5) >> 12);
                }
            }
            if (frame < 42) {
                if (frame == 0) {
                    for (i = 0, rockX = fx_17d_7_rockOffsetsX, rowZ = 0; i < 24; rockX++, i++, rowZ -= 0x18) {
                        rock = ex->btlfx_poolSpawn((s16 *)entry->pool);
                        if (rock != NULL) {
                            rowScale = (i << 7) + 0x400;
                            rock->posC.vx = work->pos8.vx + *rockX * 3 / 4;
                            rock->posC.vz = work->pos8.vz + rowZ;
                            rock->posC.vy = work->pos8.vy + 0x300;
                            rock->rot14.vx = 0x400;
                            rock->rot14.vy = 0x400;
                            rock->rot14.vz = 0;
                            rock->state0 = FX_ARK_ROCK_RISE;
                            bigScale = rowScale * 5;
                            rock->scaleA = bigScale >> 2;
                            rock->release22 = rowZ;
                            rock->index20 = i % 3;
                            rock->vel4 = ex->btlfx_randCentered(0xAA);
                            rock->vel8 = ex->btlfx_randCentered(0xAA);
                            rock->vel6 = ex->btlfx_randCentered(0x80) - ((bigScale << 6) >> 12);
                            rock->spin1C = 0;
                            rock->spin1E = 0;
                        }
                    }
                }
                fx_17d_7_releaseLevel = -((t * 0x241) >> 12);
                if (!(frame & 1)) {
                    rubble = ex->btlfx_poolSpawn((s16 *)entry->pool);
                    if (rubble != NULL) {
                        rubbleSize = (0x1000 - ex->math_cos(t >> 2)) * 3 / 4 + 0x400;
                        rubbleZ = -(t >> 3);
                        rubble->posC.vx = work->pos8.vx + ex->btlfx_randCentered(rubbleSize >> 1) * 5;
                        rubble->posC.vz = work->pos8.vz + rubbleZ;
                        rubble->posC.vy = work->pos8.vy + 0x200;
                        rubble->rot14.vx = 0x400;
                        rubble->state0 = FX_ARK_ROCK_FLY;
                        rubble->rot14.vy = 0;
                        rubble->rot14.vz = 0;
                        rubble->scaleA = rubbleSize * 3 / 2;
                        rubble->index20 = ex->btlfx_rand() % 3;
                        rubble->spin1C = ex->btlfx_randCentered(0x40);
                        rubble->spin1E = ex->btlfx_randCentered(0x40);
                        rubble->vel4 = ex->btlfx_randCentered(0x80);
                        rubble->vel8 = ex->btlfx_randCentered(0x80);
                        rubble->vel6 = ex->btlfx_randCentered(0x80) - ((rubbleSize << 9) >> 12);
                    }
                }
                if (frame == 40) {
                    for (i = 0; i < 2; i++) {
                        boulder = ex->btlfx_poolSpawn((s16 *)entry->pool);
                        if (boulder != NULL) {
                            boulder->posC.vx = work->pos8.vx + i * 0x280 - 0x140;
                            boulder->posC.vz = work->pos8.vz + 0x2BC;
                            boulder->posC.vy = work->pos8.vy + 0xBB8;
                            boulder->state0 = FX_ARK_ROCK_FLY;
                            boulder->scaleA = 0x4000;
                            boulder->spin1C = 0x32 - i * 0x64;
                            boulder->spin1E = 0x18;
                            boulder->rot14.vx = 0x400;
                            boulder->rot14.vy = 0x400;
                            boulder->rot14.vz = 0;
                            boulder->index20 = 0;
                            boulder->vel4 = 0;
                            boulder->vel8 = 0;
                            boulder->vel6 = -0x11A;
                        }
                    }
                }
                ex->btlfx_drawScreenTint((u32 *)(env->ot + 512), BTLFX_ABR_ADD, 0xFF, 0xFF, 0xFF);
            } else {
                fadeTint = 0xFF - (frame - 41) * 0xFF / 8;
                ex->btlfx_drawScreenTint((u32 *)(env->ot + 512), BTLFX_ABR_ADD, fadeTint, fadeTint, fadeTint);
            }
            gridBright = ((t << 6) >> 12) + 0x20;
            gridGrow = (t >> 3) + 0x100;
            domeLevel = 0;
            if (frame >= FX_ARK_ROCKS_FRAMES) {
                ex->btlfx_poolClear((Unk800DA1B8 *)entry->pool);
                ex->btlfx_initParticleSystem();
                ex->btlfx_clearEffModels();
                return 1;
            }
            break;
        }
        }
        env->otZ = 0;
        if (gridBright != 0) {
            CVECTOR col;
            SVECTOR gridRot;
            VECTOR gridScale;

            col.r = col.g = col.b = gridBright;
            gridRot.vx = 0;
            gridRot.vy = 0;
            gridRot.vz = 0;
            gridScale.vx = gridScale.vy = gridScale.vz = gridGrow + 0x1000;
            ex->btlfx_drawScreenGrid(1, 0xA0, 0x70, 0x50, 0x38, 4, 4, 2, 2, &gridRot, &gridScale, &col, 1, 0);
        }
        if (domeLevel != 0) {
            work->rot34.vy += 2;
            ex->Hi_DrawEffModel(&work->rot34, (VECTOR *)work->pos28, (s32)work->scale44, work->model74);
            domeRgb = (domeLevel >> 5) - 0x80;
            ex->Hi_ModifyEffModelRGB(work->model74, domeRgb, domeRgb, domeRgb);
            work->rot3C.vy += 1;
            ex->Hi_DrawEffModel(&work->rot3C, (VECTOR *)work->pos28, (s32)work->scale44, work->model70);
            ex->Hi_ModifyEffModelRGB(work->model70, domeRgb, domeRgb, domeRgb);
        }
        work->frame4++;
    }
    return 0;
}

static void fxm_drawArcTrail(Unk80100C78Entry *entry, s32 file, s32 mode, Fx17d7Trail *trail, s32 size, s32 *noise, SVECTOR *from,
                   SVECTOR *to, s32 amp0, s32 amp1, s32 ang, s32 dang, s32 frame, s32 fade, s32 lineWidth, s32 dip) {
    Unk800F78A4 *env = fx_17d_7_env;
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
    ex->btlfx_drawTrail((u8 *)ex->btlfx_getFileData(entry->file, file), 0, NULL, frame, 0x1000, fade, BTLFX_ABR_ADD, (u8 *)trail, mode);
    env->otZ = 0;
}

static void fxm_drawJaggedTrailBranch(Unk80100C78Entry *entry, s32 file, s32 mode, Fx17d7Trail *path, Fx17d7Trail *trail, s32 size, s32 *noise,
                   s32 branchAt, s32 frame, s32 fade, s32 lineWidth) {
    SVECTOR *pt = path->points4;
    BtlfxExports *ex = BTLFX_EXPORTS(fx_17d_7_env);
    SVECTOR mid;

    pt += branchAt;
    ex->btlfx_lerpVec((s16 *)(pt + 1), (s16 *)pt, 0x4000, (s16 *)&mid);
    fxm_drawArcTrail(entry, file, mode, trail, size, noise, pt, &mid, 0, 0x20, ex->btlfx_rand(), 0x33, frame, fade, lineWidth, 0);
}

static s32 fx_17d_7_op83PoolTask(s32 mode, Fx17d7Bolt *bolt, Unk800F78A4 *env, Unk80100C78Entry *entry, s32 frame) {
    BtlfxExports *ex = BTLFX_EXPORTS(env);
    s32 t;
    s32 alpha;
    s32 early;
    s32 slot;
    s32 width;
    s32 kind;
    s32 remain;
    Fx17d7Trail *trail;
    Fx17d7Trail *branch;

    if (mode != 1) {
        kind = bolt->kind11;
        if (kind < 2) {
            if (kind < 0) {
                return 0;
            }
            t = frame << 8;
            alpha = ex->math_cos(t >> 2);
            slot = bolt->slot10;
            width = bolt->width15;
            trail = (Fx17d7Trail *)&fx_17d_7_boltTrails[slot];
            branch = (Fx17d7Trail *)&fx_17d_7_branchTrails[slot];
            if (frame == 0) {
                width >>= 1;
            }
            bolt->angle14 += 3;
            early = frame < 3;
            if (bolt->kind11 == FX_ARK_BOLT_STRIKE) {
                remain = 0x1000 - t;
                fxm_drawArcTrail(entry, 0xF, 0, trail, sizeof(Fx17d7Trail17), bolt->noise18 + frame * early, &bolt->from8, &bolt->to0, 0, 0x3E8,
                              bolt->angle14 * 16, 0xF0, frame, alpha, remain * width / 1500, 0);
                fxm_drawJaggedTrailBranch(entry, 0xE, 0, trail, branch, sizeof(Fx17d7Trail5), bolt->noise18, bolt->index12, frame, alpha, (remain * width) >> 12);
                if (frame >= FX_ARK_BOLT_STRIKE_FRAMES) {
                    return 1;
                }
            } else {
                t = frame << 10;
                alpha = 0x1000 - t;
                fxm_drawArcTrail(entry, 0x13, 0, trail, sizeof(Fx17d7Trail17), bolt->noise18 + frame * early, &bolt->from8, &bolt->to0, 0, 0x12C,
                              bolt->angle14 << 7, 0x14A, frame, alpha, alpha * width / 1500, 0);
                fxm_drawJaggedTrailBranch(entry, 0xE, 0, trail, branch, sizeof(Fx17d7Trail5), bolt->noise18, bolt->index12, frame, alpha, (alpha * width) >> 12);
                if (frame >= FX_ARK_BOLT_RING_FRAMES) {
                    return 1;
                }
            }
        }
    }
    return 0;
}

s32 fx_17d_7_op83(s32 mode, Fx17d7Work83 *work, Unk800F78A4 *env, Fx17d7Inst *inst, EffInit *init) {
    BtlfxExports *ex = BTLFX_EXPORTS(env);
    s32 *noise;
    s32 i;
    s32 kind;
    s32 boltKind;
    s32 frame;
    s32 radius;
    s32 ang;
    Fx17d7Bolt *bolt;

    if (mode == 0) {
        init->workSize = sizeof(Fx17d7Work83);
        init->elemSize = sizeof(Fx17d7Bolt);
        init->elemFn = (s32)fx_17d_7_op83PoolTask;
        init->poolCount = FX_ARK_BOLT_POOL;
    } else if (mode == 1) {
        kind = init->workSize;
        fx_17d_7_env = env;
        work->noise0 = fx_17d_7_noise[0];
        noise = fx_17d_7_noise[0];
        for (i = 0; i < FX_ARK_NOISE_ROWS; i++) {
            *noise++ = ex->btlfx_randCentered(0xD2);
            *noise++ = ex->btlfx_randCentered(0x1C3);
            *noise++ = ex->btlfx_randCentered(0xDC);
        }
        work->kind8 = kind;
        work->count4 = 0;
    } else {
        boltKind = work->kind8;
        frame = inst->frame0;
        switch (boltKind) {
        case FX_ARK_BOLT_STRIKE:
            if (frame >= FX_ARK_BOLT_STRIKE_OP_FRAMES) {
                return 1;
            }
            work->count4++;
            bolt = ex->btlfx_poolSpawn(inst->pool10);
            if (bolt != NULL) {
                ((s32 *)&bolt->to0)[0] = ((s32 *)&fx_17d_7_boltBase)[0];
                ((s32 *)&bolt->to0)[1] = ((s32 *)&fx_17d_7_boltBase)[1];
                ((s32 *)&bolt->from8)[0] = ((s32 *)&fx_17d_7_boltTop)[0];
                ((s32 *)&bolt->from8)[1] = ((s32 *)&fx_17d_7_boltTop)[1];
                bolt->kind11 = FX_ARK_BOLT_STRIKE;
                bolt->slot10 = work->count4 % 5;
                bolt->noise18 = work->noise0 + (ex->btlfx_rand() & 0x1F);
                bolt->index12 = ex->btlfx_rand() % 15;
                bolt->unk13 = ex->btlfx_rand() % 15;
                bolt->width15 = 0x4E;
                bolt->angle14 = work->count4 * 0x33 + ex->btlfx_randCentered(7);
            }
            break;
        case FX_ARK_BOLT_RING:
            if (frame < FX_ARK_BOLT_RING_OP_FRAMES) {
                work->count4++;
                radius = 0xD48 - frame * 0xD48 / 28;
                bolt = ex->btlfx_poolSpawn(inst->pool10);
                if (bolt != NULL) {
                    ang = (frame << 11) / 3 + 0x400;
                    ((s32 *)&bolt->to0)[0] = ((s32 *)&fx_17d_7_boltTop)[0];
                    ((s32 *)&bolt->to0)[1] = ((s32 *)&fx_17d_7_boltTop)[1];
                    ((s32 *)&bolt->from8)[0] = ((s32 *)&fx_17d_7_boltBase)[0];
                    ((s32 *)&bolt->from8)[1] = ((s32 *)&fx_17d_7_boltBase)[1];
                    bolt->to0.vx += (ex->math_sin(ang) * radius) >> 12;
                    bolt->to0.vz += (ex->math_cos(ang) * radius) >> 12;
                    bolt->kind11 = boltKind;
                    bolt->from8.vy += 0x400;
                    bolt->slot10 = work->count4 % 5;
                    bolt->noise18 = work->noise0 + (ex->btlfx_rand() & 0x1F);
                    bolt->index12 = ex->btlfx_rand() % 15;
                    bolt->unk13 = ex->btlfx_rand() % 15;
                    bolt->width15 = 0x3E - frame;
                    bolt->angle14 = work->count4 * 0x33 + ex->btlfx_randCentered(7);
                }
            }
            if (frame >= FX_ARK_BOLT_RING_OP_FRAMES) {
                return 1;
            }
            break;
        }
    }
    return 0;
}

INCLUDE_RODATA("asm/us/fx_17d_7/nonmatchings/fx_17d_7", fx_17d_7_headerPtr);
