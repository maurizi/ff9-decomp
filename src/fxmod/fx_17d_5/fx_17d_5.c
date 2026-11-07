#include "fxmod/fx_17d_5.h"

static s32 fx_17d_5_op80PoolTask(s32 mode, Fx17d5Spark *it, Unk800F78A4 *env, Unk80100C78Entry *entry, s32 frame, Unk800DA1B8 *pool,
                  Fx17d5Work80 *parent) {
    BtlfxExports *ex = BTLFX_EXPORTS(env);
    SVECTOR ang;
    VECTOR pos;
    VECTOR scale;
    SVECTOR *head;
    Pool6CEntry *emitter;
    s32 wobble;
    s32 wobbleAng;
    s32 t; /* one variable for cases 2 and 3: register allocation */
    s32 size;
    s32 flash;
    s32 fade; /* separate from flash: each stays block-local (allocation) */
    s32 trailScale;
    s32 glowAng;

    if (mode == 1) {
        it->turn4 = 0x18;
        return 0;
    }
    switch (it->state0) {
    case 0:
        if (frame == 0) {
            ex->btlfx_drawTrail((u8 *)ex->btlfx_getFileData(entry->file, FX_ARK_TRAIL), (s32)&it->pos8, NULL, -1, 0, 0, BTLFX_ABR_HALF, it->trail18, 0);
        }
        if (frame < 20) {
            it->speed2 += ex->math_cos((frame << 10) / 20) >> 7;
            it->turn4 += 0x20;
        } else if (frame < 24) {
            it->speed2 -= 0x20;
            it->turn4 += 0x80;
        } else {
            it->speed2 += 0x20;
            if (it->speed2 > 2000) {
                it->speed2 = 2000;
            }
            it->turn4 += 0x18;
            if (it->turn4 > 0x991) {
                it->turn4 = 0x992;
            }
        }
        head = &it->pos8;
        if (it->flag14 == 0) {
            ex->btlfx_aimAngles((s16 *)head, (s16 *)&parent->target14, (s16 *)&ang);
            ex->btlfx_turnToward((s16 *)&ang, &it->angle10, it->turn4);
            wobble = frame * 12;
            if (wobble > 0xF5) {
                wobble = 0xF5;
            }
            wobbleAng = it->index6 * 0x9B + (frame << 7);
            it->angle10 += (ex->math_sin(wobbleAng + it->angle12) * wobble) >> 12;
            it->angle12 -= (ex->math_cos(wobbleAng - it->angle10) * wobble) >> 12;
            ex->btlfx_pointAhead((s16 *)head, &it->angle10, it->speed2, (s16 *)head);
            ex->btlfx_drawTrail((u8 *)ex->btlfx_getFileData(entry->file, FX_ARK_TRAIL), (s32)head, NULL, 0, wobble * 16 + 0x140, 0x1000, BTLFX_ABR_ADD,
                                it->trail18, 0);
            if (parent->state0 > 0) {
                it->flag14 = 1;
                ((s16 *)it)[-1] = -1;
            }
        } else {
            ex->btlfx_aimAngles((s16 *)head, (s16 *)&parent->target14, (s16 *)&ang);
            ex->btlfx_turnToward((s16 *)&ang, &it->angle10, it->turn4);
            wobble = frame * 4;
            if (wobble > 0x80) {
                wobble = 0x80;
            }
            wobbleAng = it->index6 * 0x9B + frame * 24;
            it->angle10 += (ex->math_sin(wobbleAng + it->angle12) * wobble) >> 12;
            it->angle12 -= (ex->math_cos(wobbleAng - it->angle10) * wobble) >> 12;
            ex->btlfx_pointAhead((s16 *)head, &it->angle10, it->speed2, (s16 *)head);
            env->otZ = 0xFF;
            ex->btlfx_drawTrail((u8 *)ex->btlfx_getFileData(entry->file, FX_ARK_TRAIL), (s32)head, NULL, 0, it->width22 - frame * 0x23A,
                                0x1000, BTLFX_ABR_ADD, it->trail18, 0);
            env->otZ = 0;
            if (it->index6 == 1) {
                env->camera->points[1].vx = it->pos8.vx;
                env->camera->points[1].vy = it->pos8.vy;
                env->camera->points[1].vz = it->pos8.vz;
                env->camera->points[1].vy -= frame << 5;
                env->camera->points[1].vz -= frame << 5;
            }
        }
        break;
    case 1:
        if (frame == 0) {
            ex->btlfx_drawTrail((u8 *)ex->btlfx_getFileData(entry->file, FX_ARK_TRAIL_2), (s32)&it->pos8, NULL, -1, 0, 0, BTLFX_ABR_HALF, it->trail18, 0);
        }
        trailScale = 0x1800;
        it->pos8.vz += 0x300;
        it->pos8.vy += 0x300;
        if (it->angle10 == FX_17D_5_HIT_FINAL) {
            trailScale = 0x5000;
        }
        ex->btlfx_drawTrail((u8 *)ex->btlfx_getFileData(entry->file, FX_ARK_TRAIL_2), (s32)&it->pos8, NULL, 0, trailScale, 0x1000, BTLFX_ABR_ADD, it->trail18, 0);
        if (frame >= 7) {
            it->state0 = 2;
            ((s16 *)it)[-1] = -1;
            if (it->angle10 == FX_17D_5_HIT_FINAL) {
                ex->btlfx_drawScreenTint((u32 *)(env->ot + 2), BTLFX_ABR_SUB, 0x40, 0x40, 0x40);
                it->state0 = 3;
            } else if (it->angle10 != FX_17D_5_HIT_BURST) {
                flash = (ex->btlfx_rand() & 0x1F) + 0x28;
                ex->btlfx_drawScreenTint((u32 *)(env->ot + 2), BTLFX_ABR_ADD, flash, flash, flash);
            } else {
                emitter = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, FX_ARK_PLANET_FRAGMENTS), (s32)&it->pos8);
                if (emitter != NULL) {
                    emitter->otZ = 0x80;
                }
            }
            it->pos8.vy = 0;
        }
        break;
    case 2:
        t = (frame << 12) / 24;
        if (frame < 8) {
            ex->btlfx_drawTrail((u8 *)ex->btlfx_getFileData(entry->file, FX_ARK_TRAIL_2), (s32)&it->pos8, NULL, 0, 0x1000, 0x1000, BTLFX_ABR_ADD,
                                it->trail18, 0);
        }
        env->otZ = 0x140;
        if (it->angle10 != FX_17D_5_HIT_BURST) {
            ex->btlfx_drawRings((u8 *)ex->btlfx_getFileData(entry->file, FX_ARK_ORANGE_RING), &it->pos8, NULL, 0x1000, frame, BTLFX_ABR_ADD, 0xAAA);
            env->otZ = 0;
        } else {
            ex->btlfx_drawParticles((u8 *)ex->btlfx_getFileData(entry->file, FX_ARK_PLANET), &it->pos8, NULL, 0x1000, frame, BTLFX_ABR_ADD, 0xC00,
                                    0, 0);
            env->otZ = 0;
        }
        if (it->angle10 != FX_17D_5_HIT_BURST) {
            ang.vz = 0;
            ang.vx = 0;
            ang.vy = frame * 8;
            if (frame == 0) {
                size = 0x555;
            } else {
                size = ((ex->math_sin(t >> 2) * 9) >> 3) + 0x1000;
            }
            scale.vx = scale.vy = scale.vz = (size * 0x578) >> 12;
            pos.vx = it->pos8.vx;
            pos.vy = it->pos8.vy;
            pos.vz = it->pos8.vz;
            ex->Hi_DrawEffModel(&ang, &pos, (s32)&scale, parent->model88);
            glowAng = t >> 2;
            ex->Hi_ModifyEffModelRGB(parent->model88, (ex->math_cos(glowAng) >> 5) - 0x80, (ex->math_cos(glowAng) >> 5) - 0x80,
                                     (ex->math_cos(glowAng) >> 5) - 0x80);
        }
        if (frame >= 24) {
            it->state0 = 2;
            return 1;
        }
        break;
    case 3:
        t = frame << 7;
        if (frame < 8) {
            ex->btlfx_drawTrail((u8 *)ex->btlfx_getFileData(entry->file, FX_ARK_TRAIL_2), (s32)&it->pos8, NULL, 0, 0x5000, 0x1000, BTLFX_ABR_ADD,
                                it->trail18, 0);
            if (frame < 6) {
                fade = 0x80 - t / 6;
                ex->btlfx_drawScreenTint((u32 *)(env->ot + 2), BTLFX_ABR_ADD, fade, fade, fade);
            }
        }
        env->otZ = 0x400;
        ex->btlfx_drawRings((u8 *)ex->btlfx_getFileData(entry->file, FX_ARK_ORANGE_RING), &it->pos8, NULL, 0x1800, frame, BTLFX_ABR_ADD, 0x1000);
        env->otZ = 0;
        ang.vz = 0;
        ang.vx = 0;
        ang.vy = frame * 12;
        if (frame == 0) {
            size = 0x800;
        } else if (t < 0xC00) {
            size = ex->math_sin(t / 3) * 2 / 3 + 0xC00;
        } else {
            size = 0x16AA;
        }
        scale.vx = scale.vy = scale.vz = size >> 2;
        pos.vx = it->pos8.vx;
        pos.vy = it->pos8.vy;
        pos.vz = it->pos8.vz;
        ex->Hi_DrawEffModel(&ang, &pos, (s32)&scale, parent->model90);
        ex->Hi_DrawEffModel(&ang, &pos, (s32)&scale, parent->model94);
        ex->Hi_DrawEffModel(&ang, &pos, (s32)&scale, parent->model8C);
        if (frame >= 32) {
            it->state0 = 2;
            return 1;
        }
        break;
    }
    return 0;
}

s32 fx_17d_5_op80(s32 mode, Fx17d5Work80 *work, Unk800F78A4 *env, Unk80100C78Entry *entry, EffInit *init) {
    BtlfxExports *ex = BTLFX_EXPORTS(env);
    SVECTOR aim; /* also the fade colour */
    SVECTOR rot;
    VECTOR scale;
    Fx17d5Spark *spark;
    Fx17d5Spark *homer;
    Fx17d5Spark *striker;
    Pool6CEntry *emitter;
    u8 *lineData;
    s32 frame;
    s32 fade;
    s32 flag;
    s32 i;
    s32 j;
    s32 t;
    s32 swayAng;
    s32 spreadAng;
    s32 ringAlpha;
    s32 shake;
    s32 gridBright;
    s32 tintIn;
    s32 lineFrame;
    s32 tint;

    if (mode == 0) {
        init->workSize = 0x98;
        init->elemSize = 0x24;
        init->elemFn = (s32)fx_17d_5_op80PoolTask;
        init->poolCount = 0x39;
    } else if (mode == 1) {
        work->state0 = 0;
        work->frame4 = 0;
        work->count8 = 0;
        fx_17d_5_env = env;
        ex->btlfx_initParticleSystem();
        ex->btlfx_initEffModels(fx_17d_5_effModels, 5);
        work->model84 = ex->btlfx_registerEffectModel((Unk800E8F50 *)ex->btlfx_getFileData(entry->file, FX_ARK_RIBBED_SPHERE), 1);
        work->model88 = ex->btlfx_registerEffectModel((Unk800E8F50 *)ex->btlfx_getFileData(entry->file, FX_ARK_SCALE_BOWL), 1);
        work->model8C = ex->btlfx_registerEffectModel((Unk800E8F50 *)ex->btlfx_getFileData(entry->file, FX_ARK_SCALE_DOME), 1);
        work->model90 = ex->btlfx_registerEffectModel((Unk800E8F50 *)ex->btlfx_getFileData(entry->file, FX_ARK_TAN_GLOW_DISC), 1);
        work->model94 = ex->btlfx_registerEffectModel((Unk800E8F50 *)ex->btlfx_getFileData(entry->file, FX_ARK_YELLOW_RING), 1);
        ex->Hi_SetSummonMotion(((Fx17d5Summon *)env->loader)->summon54->motion1A4, ((Fx17d5Summon *)env->loader)->model60);
        ex->Hi_SetSummonMotFrame(((Fx17d5Summon *)env->loader)->model60, 3);
        work->pos64.vx = 0;
        work->pos64.vy = -0x400;
        work->pos64.vz = 0;
        work->scale70.vx = work->scale70.vy = work->scale70.vz = 0x800;
        work->posC.vx = work->pos64.vx;
        work->posC.vy = work->pos64.vy;
        work->posC.vz = work->pos64.vz;
        work->target14.vx = work->pos64.vx;
        work->target14.vy = work->pos64.vy;
        work->target14.vz = work->pos64.vz - 0xC00;
        work->rot7C.vx = 0;
        work->rot7C.vy = 0;
        work->rot7C.vz = 0;
        work->rot50.vx = 0;
        work->rot50.vy = 0;
        work->rot50.vz = 0;
        work->pos44.vx = work->pos64.vx;
        work->pos44.vy = work->pos64.vy;
        work->pos44.vz = work->pos64.vz;
        work->posC.vy -= 0x3CA;
        work->pos44.vy += 0xC00;
        work->scale58.vx = work->scale58.vz = work->scale58.vy = 0x1400;
        work->pos3C.vx = work->posC.vx;
        work->pos3C.vy = work->posC.vy;
        work->pos3C.vz = work->posC.vz;
        env->camera->points[3].vx = work->pos3C.vx;
        env->camera->points[3].vy = work->pos3C.vy;
        env->camera->points[3].vz = work->pos3C.vz;
    } else {
        fade = 0;
        flag = 0;
        frame = entry->frame;
        switch (work->state0) {
        case 0:
            ex->Hi_DrawSummonModel(&work->rot7C, (VECTOR *)&work->pos64, (s32)&work->scale70, ((Fx17d5Summon *)env->loader)->model60, 0);
            work->rot50.vy += 6;
            ex->Hi_DrawEffModel(&work->rot50, (VECTOR *)&work->pos44, (s32)&work->scale58, work->model84);
            ex->Hi_ModifyEffModelAbr(work->model84, BTLFX_ABR_ADD);
            ex->Hi_ModifyEffModelRGB(work->model84, 0, 0, 0);
            if (frame == 0x1F) {
                emitter = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, FX_ARK_CREAM_RING), (s32)&work->posC);
                if (emitter != NULL) {
                    emitter->otZ = -0xF0;
                }
            }
            if (frame == 0x1B) {
                emitter = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, FX_ARK_LILAC_RING), (s32)&work->posC);
                if (emitter != NULL) {
                    emitter->otZ = 0x80;
                }
            }
            lineFrame = frame - 0x20;
            if ((u32)lineFrame < 0x10) {
                lineData = (u8 *)ex->btlfx_getFileData(entry->file, FX_ARK_LENS_FLARE);
                lineFrame <<= 12;
                ex->btlfx_drawSpriteLine(lineData, &work->posC, lineFrame >> 12, ex->math_cos(lineFrame >> 6));
            }
            if (frame >= 0x1F && frame < 0x27) {
                tint = 0xFF - (((frame - 0x1F) * 0xFF) >> 3);
                ex->btlfx_drawScreenTint((u32 *)(env->ot + 2), BTLFX_ABR_ADD, tint, tint, tint * 2 / 3);
            }
            if (frame >= 0x31) {
                flag = 1;
                fade = ((frame - 0x31) << 12) >> 4;
            }
            if (frame >= 0x21 && work->count8 < 0x39) {
                for (i = 0; i < 3; i++) {
                    if (work->count8 < 0x39) {
                        work->count8++;
                        spark = ex->btlfx_poolSpawn((s16 *)entry->pool);
                        if (spark != NULL) {
                            spark->state0 = 0;
                            BTLFX_COPY_SVEC(&spark->pos8, &work->posC);
                            spark->pos1C = work->target14.vx;
                            spark->targetZ20 = work->target14.vz;
                            spark->targetY1E = work->target14.vy;
                            spark->speed2 = 0x90;
                            {
                                s32 slot = work->count8 % 3;
                                spark->angle12 = work->rot7C.vy + (slot * 0x300 - 0x300);
                                slot = work->count8 / 3;
                                spark->angle10 = work->rot7C.vx + 0x80 + slot * 0x58;
                            }
                            spark->flag14 = 0;
                            spark->width22 = 0x2800;
                            spark->index6 = work->count8 - 1;
                            spark->trail18 = fx_17d_5_sparkTrails[work->count8];
                        }
                    }
                }
            } else {
                work->target14.vy += 0xC;
            }
            if (frame >= 0x41) {
                work->state0 = 1;
                entry->frame = -1;
                ex->btlfx_initParticleSystem();
                work->pos3C.vx = work->pos24.vx;
                work->pos3C.vy = work->pos24.vy;
                work->pos3C.vz = work->pos24.vz;
                env->camera->points[3].vx = work->pos3C.vx;
                env->camera->points[3].vy = work->pos3C.vy;
                env->camera->points[3].vz = work->pos3C.vz;
                work->rot50.vx = 0;
                work->rot50.vy = 0;
                work->rot50.vz = 0;
                work->count8 = 0;
                work->target14.vz += 0x2000;
                work->target14.vy += 0x2000;
                work->scale58.vx = work->scale58.vz = work->scale58.vy = 0x2000;
                work->pos44.vx = work->target14.vx;
                work->pos44.vy = work->target14.vy;
                work->pos44.vz = work->target14.vz;
            }
            break;
        case 1:
            work->rot50.vx += 0x20;
            work->rot50.vy += 0x20;
            ex->Hi_DrawEffModel(&work->rot50, (VECTOR *)&work->pos44, (s32)&work->scale58, work->model84);
            ex->Hi_ModifyEffModelAbr(work->model84, BTLFX_ABR_ADD);
            ex->Hi_ModifyEffModelRGB(work->model84, 0, 0, 0);
            if (frame >= 0x10) {
                work->state0 = 2;
                entry->frame = -1;
                ex->btlfx_initParticleSystem();
                BTLFX_COPY_SVEC(&work->pos3C, &fx_17d_5_targetPos);
                env->camera->points[3].vx = work->pos3C.vx;
                env->camera->points[3].vy = work->pos3C.vy;
                env->camera->points[3].vz = work->pos3C.vz;
                work->pos24.vx = fx_17d_5_targetPos.vx;
                work->pos24.vy = fx_17d_5_targetPos.vy;
                work->pos24.vz = fx_17d_5_targetPos.vz;
                work->pos24.vy -= 0x5000;
                work->pos24.vz -= 0x5000;
                work->posC.vx = work->pos24.vx;
                work->posC.vy = work->pos24.vy;
                work->posC.vz = work->pos24.vz;
                ex->btlfx_aimAngles((s16 *)&work->posC, (s16 *)&fx_17d_5_targetPos, (s16 *)&work->angle1C);
                BTLFX_COPY_SVEC(&work->pos3C, &work->pos24);
                work->pos3C.vy += 0x200;
                work->pos3C.vz += 0x200;
                env->camera->points[2].vx = work->pos3C.vx;
                env->camera->points[2].vy = work->pos3C.vy;
                env->camera->points[2].vz = work->pos3C.vz;
                work->count8 = 0;
            }
            break;
        case 2:
            t = frame << 7;
            if (frame == 0) {
                BTLFX_COPY_SVEC(&work->target14, &fx_17d_5_targetPos);
                ex->btlfx_poolClear(entry->pool);
                for (j = 0; j < 0x20; j++) {
                    if (work->count8 < 0x39) {
                        work->count8++;
                        homer = ex->btlfx_poolSpawn((s16 *)entry->pool);
                        if (homer != NULL) {
                            homer->state0 = 0;
                            BTLFX_COPY_SVEC(&homer->pos8, &work->posC);
                            BTLFX_COPY_SVEC(&aim, &work->angle1C);
                            spreadAng = work->count8 * 0x145 + 0x100;
                            homer->pos8.vx += ex->math_cos(spreadAng) * 5 / 2;
                            homer->pos8.vy += ex->math_sin(spreadAng) * 5 / 2;
                            aim.vy += ((work->count8 + 3) & 0xF) * 32 - 0x100;
                            aim.vx += (work->count8 & 1) * 512 - 0x100;
                            homer->pos1C = work->target14.vx;
                            homer->targetZ20 = work->target14.vz;
                            homer->targetY1E = work->target14.vy;
                            homer->speed2 = 0x320;
                            homer->angle10 = aim.vx;
                            homer->angle12 = aim.vy;
                            homer->flag14 = 1;
                            homer->width22 = 0x5000;
                            homer->index6 = work->count8 - 1;
                            homer->trail18 = fx_17d_5_sparkTrails[work->count8];
                        }
                    }
                }
            }
            swayAng = t >> 2;
            work->pos3C.vy += (ex->math_sin(swayAng) * 600) >> 12;
            work->pos3C.vz += (ex->math_sin(swayAng) * 600) >> 12;
            env->camera->points[2].vx = work->pos3C.vx;
            env->camera->points[2].vy = work->pos3C.vy;
            env->camera->points[2].vz = work->pos3C.vz;
            if (frame != 0) {
                rot.vx = 0x400;
                rot.vz = 0;
                rot.vy = 0;
                rot.pad = 1;
                ex->btlfx_drawRings((u8 *)ex->btlfx_getFileData(entry->file, FX_ARK_RED_FLASH_RING), &fx_17d_5_targetPos, &rot, 0x5000, 0, BTLFX_ABR_ADD, -1);
            }
            if (frame >= 0x20) {
                work->state0 = 3;
                entry->frame = -1;
                ex->btlfx_initParticleSystem();
                BTLFX_COPY_SVEC(&work->pos24, &fx_17d_5_targetPos);
                work->pos24.vy -= 0x80;
                work->pos44.vx = work->pos24.vx;
                work->pos44.vy = work->pos24.vy;
                work->pos44.vz = work->pos24.vz;
                BTLFX_COPY_SVEC(&work->pos3C, &work->pos24);
                work->rot50.vx = 0;
                work->rot50.vy = 0;
                work->rot50.vz = 0;
                work->pos3C.vx += 0x800;
                work->pos3C.vy -= 0x20;
                work->pos3C.vz = fx_17d_5_targetPos.vz;
                env->camera->points[2].vx = work->pos3C.vx;
                env->camera->points[2].vy = work->pos3C.vy;
                env->camera->points[2].vz = work->pos3C.vz;
                work->count8 = 0;
            }
            break;
        case 3:
            t = (frame << 12) / 72;
            if (frame == 0) {
                ex->btlfx_poolClear(entry->pool);
            }
            if (work->count8 < 0x20) {
                work->count8++;
                if (work->count8 < 0x1F) {
                    striker = ex->btlfx_poolSpawn((s16 *)entry->pool);
                    if (striker != NULL) {
                        striker->state0 = 1;
                        BTLFX_COPY_SVEC(&striker->pos1C, &work->pos24);
                        striker->pos1C -= t * 3 / 2 - 0x800 + ex->btlfx_randCentered(1000);
                        striker->targetY1E = 0;
                        striker->targetZ20 += (work->count8 % 3) * 1200 - 0x2B0 + ex->btlfx_randCentered(0x6A4);
                        striker->pos8.vx = striker->pos1C;
                        striker->pos8.vy = striker->targetY1E - 0x1800;
                        striker->pos8.vz = striker->targetZ20 - 0x1800;
                        striker->angle10 = work->count8 & 1;
                        striker->index6 = work->count8 - 1;
                        striker->trail18 = fx_17d_5_sparkTrails[work->count8];
                    }
                } else {
                    striker = ex->btlfx_poolSpawn((s16 *)entry->pool);
                    if (striker != NULL) {
                        striker->state0 = 1;
                        BTLFX_COPY_SVEC(&striker->pos1C, &work->pos24);
                        striker->targetY1E = 0;
                        striker->pos1C += 0x6A4 - t * 3 / 2;
                        striker->angle10 = 2;
                        striker->pos8.vx = striker->pos1C;
                        striker->targetZ20 += 0x400;
                        striker->pos8.vy = striker->targetY1E - 0x1800;
                        striker->pos8.vz = striker->targetZ20 - 0x1800;
                        striker->index6 = work->count8 - 1;
                        striker->trail18 = fx_17d_5_sparkTrails[work->count8];
                    }
                }
            }
            BTLFX_COPY_SVEC(&work->pos3C, &work->pos24);
            work->pos3C.vx += 0x800 - t;
            work->pos3C.vy -= 0x20;
            work->pos3C.vz = fx_17d_5_targetPos.vz;
            if (frame >= 9 && frame < 0x31) {
                if (frame < 0x28) {
                    shake = (t >> 5) + 0x26;
                } else if (frame < 0x2D) {
                    shake = (ex->math_cos(((frame - 0x28) << 10) / 5) * 400) >> 12;
                } else {
                    shake = 0;
                }
                work->pos3C.vy += ex->btlfx_randCentered(shake);
                work->pos3C.vz += ex->btlfx_randCentered(shake);
            }
            env->camera->points[2].vx = work->pos3C.vx;
            env->camera->points[2].vy = work->pos3C.vy;
            env->camera->points[2].vz = work->pos3C.vz;
            if (frame < 0x10) {
                ringAlpha = ex->math_cos((frame << 10) >> 4);
                rot.vx = 0x400;
                rot.vz = 0;
                rot.vy = 0;
                rot.pad = 1;
                ex->btlfx_drawRings((u8 *)ex->btlfx_getFileData(entry->file, FX_ARK_RED_FLASH_RING), &fx_17d_5_targetPos, &rot, 0x5000, 0, BTLFX_ABR_ADD, ringAlpha);
            }
            ex->btlfx_drawScrollMove(frame * 29, 0x200, 0x100, 0x40, 0x80, 0x200, 0x180);
            if (frame >= 0x38) {
                tintIn = (frame - 0x38) * 0xFF;
                ex->btlfx_drawScreenTint((u32 *)(env->ot + 2), BTLFX_ABR_ADD, tintIn >> 4, (tintIn >> 4) * 2 / 3, tintIn >> 5);
            }
            if (frame >= 0x48) {
                work->state0 = 3;
                entry->frame = -1;
                ex->btlfx_initParticleSystem();
                ex->btlfx_poolClear(entry->pool);
                work->pos24.vx = 0;
                work->pos24.vy = -0x800;
                work->pos24.vz = 0;
                work->rot50.vx = 0;
                work->rot50.vy = 0;
                work->rot50.vz = 0;
                work->pos44.vx = work->pos24.vx;
                work->pos44.vy = work->pos24.vy;
                work->pos44.vz = work->pos24.vz;
                work->pos3C.vx = work->pos24.vx;
                work->pos3C.vy = work->pos24.vy;
                work->pos3C.vz = work->pos24.vz;
                env->camera->points[3].vx = work->pos3C.vx;
                env->camera->points[3].vy = work->pos3C.vy;
                env->camera->points[3].vz = work->pos3C.vz;
                ex->btlfx_clearEffModels();
                return 1;
            }
            break;
        }
        env->otZ = 0;
        if (fade != 0) {
            if (flag == 0) {
                gridBright = ((fade << 5) >> 12) + 0x89;
            } else {
                gridBright = (fade * 70) >> 12;
            }
            ((CVECTOR *)&aim)->r = ((CVECTOR *)&aim)->g = ((CVECTOR *)&aim)->b = gridBright;
            rot.vx = 0;
            rot.vy = 0;
            rot.vz = 0;
            scale.vx = scale.vy = scale.vz = (fade >> 7) + 0x1000;
            ex->btlfx_drawScreenGrid(1, 0xA0, 0x70, 0x50, 0x38, 4, 4, 2, 2, &rot, &scale, (CVECTOR *)&aim, flag, 0);
        }
        ex->btlfx_drawLayerTexture((BtlBgLayers *)ex->btlfx_getFileData(entry->file, FX_ARK_BACKDROP), work->frame4);
        work->frame4++;
    }
    return 0;
}

INCLUDE_RODATA("asm/us/fx_17d_5/nonmatchings/fx_17d_5", fx_17d_5_headerPtr);
