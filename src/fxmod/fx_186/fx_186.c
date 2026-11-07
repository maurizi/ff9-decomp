#include "fxmod/fx_186.h"

static s32 fx_186_op80PoolTask(s32 mode, Fx186Rock *rock, Unk800F78A4 *env, s32 entry, s32 frame) {
    BtlfxExports *ex = BTLFX_EXPORTS(env);
    VECTOR pos;
    VECTOR scale;
    s32 alpha;
    s32 t;
    s32 rgb;
    s16 x;
    s16 z;
    s32 rnd;

    if (mode == 1) {
        rock->state0 = 0;
        rock->bobVelY6 = ex->btlfx_rand() % 1000 - 400;
        rock->rot14.vx = 0;
        rock->rot14.vy = 0;
        rock->rot14.vz = 0;
        rock->spin1E = 0;
        rock->spin1C = 0;
    } else {
        scale.vx = 0x2000;
        scale.vy = 0x2000;
        scale.vz = 0x2000;
        pos.vx = rock->posC.vx;
        pos.vy = rock->posC.vy;
        pos.vz = rock->posC.vz;
        alpha = 0x1000;
        rock->rot14.vx += rock->spin1C;
        rock->rot14.vy += rock->spin1E;
        switch (rock->state0) {
        case 0:
            pos.vx = rock->posC.vx;
            pos.vy = rock->posC.vy;
            pos.vz = rock->posC.vz;
            t = frame << 7;
            pos.vy = pos.vy - ((ex->math_sin(t >> 2) * rock->bobVelY6) >> 12) + ((frame & 1) * 2 - 1) * (t >> 7);
            if (frame >= 0x20) {
                rock->state0 = 1;
                ((s16 *)rock)[-1] = -1;
                rock->posC.vx -= rock->bobVelY6;
                rock->spin1C = ex->btlfx_randCentered(8);
                rock->spin1E = ex->btlfx_randCentered(8);
                rock->vel8 = rock->posC.vx / 4 + ex->btlfx_randCentered(0x80);
                rnd = ex->btlfx_randCentered(0x100);
                x = rock->posC.vx / 7;
                z = rock->posC.vz / 7;
                rock->bobVelY6 = (s16)(rnd - 0x2BC) + ex->btlfx_sqrt(x * x + z * z, 0x14) / 2;
                rock->velA = rock->posC.vz / 4 + ex->btlfx_randCentered(0x80);
            }
            break;
        case 1:
            t = (frame << 12) / 40;
            alpha -= t;
            rock->posC.vx += rock->vel8 >> 2;
            rock->posC.vz += rock->velA >> 2;
            rock->vel8 = rock->vel8 * 7 >> 3;
            rock->velA = rock->velA * 7 >> 3;
            rock->posC.vy += rock->bobVelY6 / 5;
            rock->bobVelY6 += 12;
            if (frame >= 0x28) {
                return 1;
            }
            break;
        }
        ex->Hi_DrawEffModel(&rock->rot14, &pos, (s32)&scale, rock->model20);
        if (alpha < 0x1000) {
            if (alpha < 0x800) {
                ex->Hi_ModifyEffModelAbr(rock->model20, BTLFX_ABR_ADD);
            }
            rgb = (alpha >> 5) - 0x80;
            ex->Hi_ModifyEffModelRGB(rock->model20, rgb, rgb, rgb);
        }
        /* Dead test, removed after reload: the extra reference ranks alpha above the constant 1 in global-alloc. */
        if (alpha != 0) {
            t = 0;
        }
    }
    return 0;
}

s32 fx_186_op80(s32 mode, Fx186Work80 *work, Unk800F78A4 *env, Unk80100C78Entry *entry, EffInit *init) {
    BtlfxExports *ex = BTLFX_EXPORTS(env);
    s32 tint;
    s32 frame;
    s32 t;

    if (mode == 0) {
        init->workSize = 0x4C;
        init->elemSize = 0x28;
        init->elemFn = (s32)fx_186_op80PoolTask;
        init->poolCount = 0xA;
    } else if (mode == 1) {
        work->state0 = 3;
        work->frame4 = 0;
        fx_186_env = env;
        ex->btlfx_initEffModels(fx_186_effModels, 0xC);
        work->pos8.vx = 0;
        work->pos8.vy = 0;
        work->pos8.vz = 0;
        work->model44 = ex->btlfx_registerEffectModel((Unk800E8F50 *)ex->btlfx_getFileData(entry->file, FX_EARTH_SHAKE_ROCK_ARCH), 1);
        work->model48 = ex->btlfx_registerEffectModel((Unk800E8F50 *)ex->btlfx_getFileData(entry->file, FX_EARTH_SHAKE_RUBBLE_STRIP), 1);
        env->camera->points[3].vx = work->pos8.vx;
        env->camera->points[3].vy = work->pos8.vy;
        env->camera->points[3].vz = work->pos8.vz;
        env->camera->points[3].vy -= 0x100;
    } else {
        tint = 0;
        frame = entry->frame;
        switch (work->state0) {
        case 3:
            ex->btlfx_drawBgOffscreen(0x1C0, 0x100, 0x100, 0x100, fx_186_bgOt, work->pos8.vx, work->pos8.vz, 0xC00);
            work->state0 = 0;
            entry->frame = -1;
            break;
        case 0:
            /* gcc 2.8 frees block-scoped stack slots only at the end of a switch; the original reuses each case's
             * slots for the next case, which this one-label switch reproduces. */
            switch (frame) {
            default: {
                RECT rect;
                VECTOR pos;
                VECTOR scale;
                SVECTOR rot;
                SVECTOR rot2;
                s32 ang;
                s32 ringScale;
                s32 i;
                s32 k;
                s32 blankCount;
                s16 *pixel;
                s16 *fill;

                t = frame << 8;
                env->camera->points[3].vx = work->pos8.vx;
                env->camera->points[3].vy = work->pos8.vy;
                env->camera->points[3].vz = work->pos8.vz;
                env->camera->points[3].vy = (s16)(env->camera->points[3].vy - 0x100) - (t >> 7) * ((frame & 1) * 2 - 1);
                if (frame == 1) {
                    for (fill = fx_186_vramSample, k = 0; k < 0x60; k++, fill++) {
                        *fill = -1;
                    }
                    rect.x = 0x200;
                    rect.y = 0x180;
                    rect.w = 4;
                    rect.h = 4;
                    ex->StoreImage(&rect, (u32 *)fx_186_vramSample);
                    rect.x = 0x27C;
                    rect.y = 0x180;
                    rect.w = 4;
                    rect.h = 4;
                    ex->StoreImage(&rect, (u32 *)fx_186_vramSample + 0x10);
                    rect.x = 0x240;
                    rect.y = 0x1EC;
                    rect.w = 4;
                    rect.h = 4;
                    ex->StoreImage(&rect, (u32 *)fx_186_vramSample + 0x30);
                }
                ringScale = (t + 0x1000) >> 2;
                scale.vz = ((t >> 1) + 0x1000) >> 2;
                pos.vx = work->pos8.vx;
                pos.vy = work->pos8.vy;
                pos.vz = work->pos8.vz;
                pos.vy -= 0x180;
                rot.vx = -0x400;
                rot.vy = 0;
                rot.vz = frame * 16;
                for (i = 0; i < 6; i++) {
                    scale.vx = scale.vy = (ringScale * ((ex->math_sin((i << 10) / 5) >> 1) + 0x800)) >> 12;
                    ex->Hi_DrawEffModel(&rot, &pos, (s32)&scale, work->model44);
                    ex->Hi_ModifyEffModelRGB(work->model44, (ex->math_sin(t >> 2) >> 5) - 0x80,
                                             (ex->math_sin(t >> 2) >> 5) - 0x80, (ex->math_sin(t >> 2) >> 5) - 0x80);
                    rot.vz += 0x258;
                }
                if (frame >= -4) {
                    s32 pulseAng;

                    pulseAng = ((frame + 4) << 12) / 20;
                    pulseAng >>= 2;
                    scale.vz = 0x2000;
                    scale.vx = 0x2000;
                    scale.vy = ex->math_sin(pulseAng) * 2 + 0x1000;
                    rot.vz = 0;
                    rot.vy = 0;
                    rot.vx = 0;
                    pos.vz = 0;
                    pos.vx = 0;
                    pos.vy = 0x5AE;
                    ex->Hi_DrawEffModel(&rot, &pos, (s32)&scale, work->model48);
                    ex->Hi_ModifyEffModelAbr(work->model48, BTLFX_ABR_ADD);
                    ex->Hi_ModifyEffModelRGB(work->model48, (ex->math_sin(pulseAng) >> 5) - 0x80, (ex->math_sin(pulseAng) >> 5) - 0x80,
                                             (ex->math_sin(pulseAng) >> 5) - 0x80);
                    ex->btlfx_drawScrollMove(frame * 16, 0x2C0, 0x180, 0x20, 0x80, 0x2E0, 0x180);
                }
                if (frame >= 9) {
                    rot2.vx = 0x400;
                    rot2.vy = 0;
                    rot2.vz = 0;
                    rot2.pad = 1;
                    ang = (frame - 8) << 9;
                    ex->btlfx_drawRings((u8 *)ex->btlfx_getFileData(entry->file, FX_EARTH_SHAKE_LILAC_RING), &work->pos8, &rot2,
                                      ex->math_sin(ang >> 2), 0,
                                      BTLFX_ABR_ADD, ex->math_sin(ang >> 2));
                    ex->btlfx_drawRings((u8 *)ex->btlfx_getFileData(entry->file, FX_EARTH_SHAKE_CYAN_RING), &work->pos8, &rot2,
                                      ex->math_sin(ang >> 2) >> 1, 0, BTLFX_ABR_ADD, ex->math_sin(ang >> 2));
                }
                if (frame >= 0x10) {
                    work->state0 = 1;
                    entry->frame = -1;
                    ex->btlfx_drawScreenTint((u32 *)(env->ot + 2), BTLFX_ABR_ADD, 0xFF, 0xFF, 0xFF);
                    for (blankCount = 0, pixel = fx_186_vramSample, k = 0; k < 0x60; k++, pixel++) {
                        if (*pixel == 0) {
                            blankCount++;
                        }
                    }
                    for (i = 0; i < 10; i++) {
                        if (blankCount != 0) {
                            work->model1C[i] =
                                ex->btlfx_registerEffectModel((Unk800E8F50 *)ex->btlfx_getFileData(entry->file, fx_186_modelFileIds1[i]), 0);
                        } else {
                            work->model1C[i] =
                                ex->btlfx_registerEffectModel((Unk800E8F50 *)ex->btlfx_getFileData(entry->file, fx_186_modelFileIds0[i]), 0);
                        }
                    }
                }
            }
            }
            break;
        case 1:
            switch (frame) {
            default: {
                VECTOR pos;
                VECTOR scale;
                SVECTOR rot;
                SVECTOR rot2;
                s32 t;
                s32 ang;
                s32 ringScale;
                s32 flash;
                s32 i;
                s32 j;
                Fx186Rock *rock;

                env->camera->points[3].vx = work->pos8.vx;
                env->camera->points[3].vy = work->pos8.vy;
                env->camera->points[3].vz = work->pos8.vz;
                env->camera->points[3].vy = (s16)(env->camera->points[3].vy - 0x100) - ((frame & 1) * 2 - 1) * 16;
                if (frame == 0) {
                    BTLFX_COPY_SVEC(&work->pos10, &work->pos8);
                    work->pos10.vy -= 0x180;
                    work->emit18 = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, FX_EARTH_SHAKE_ROCK_DEBRIS), (s32)&work->pos10);
                    if (work->emit18 != NULL) {
                        work->emit18->otZ = 0x100;
                    }
                    for (j = 0; j < 25; j++) {
                        rock = ex->btlfx_poolSpawn((s16 *)entry->pool);
                        if (rock != NULL) {
                            rock->model20 = work->model1C[j];
                            rock->posC.vx = work->pos8.vx;
                            rock->posC.vy = work->pos8.vy + 0x5AE;
                            rock->posC.vz = work->pos8.vz;
                        }
                    }
                }
                if (work->emit18 != NULL && work->emit18->state == BTLFX_EMIT_DONE) {
                    work->emit18 = NULL;
                }
                if (frame < 4) {
                    flash = 0xFF - ((frame * 0xFF) >> 2);
                    ex->btlfx_drawScreenTint((u32 *)(env->ot + 2), BTLFX_ABR_ADD, flash, flash, flash);
                }
                if (frame < 0x18) {
                    t = (frame << 12) / 24;
                    scale.vz = t * 2 + 0x600;
                    ringScale = (t >> 2) + 0x800;
                    pos.vx = work->pos8.vx;
                    ang = t >> 2;
                    pos.vy = work->pos8.vy;
                    pos.vz = work->pos8.vz;
                    pos.vy += -0x180 - (t >> 1);
                    rot.vx = -0x400;
                    rot.vy = 0;
                    rot.vz = frame * 16 + 0x100;
                    for (i = 0; i < 6; i++) {
                        scale.vx = scale.vy = (ringScale * ((ex->math_sin((i << 10) / 5) >> 1) + 0x800)) >> 12;
                        ex->Hi_DrawEffModel(&rot, &pos, (s32)&scale, work->model44);
                        ex->Hi_ModifyEffModelRGB(work->model44, (ex->math_cos(ang) >> 5) - 0x80,
                                                 (ex->math_cos(ang) >> 5) - 0x80,
                                                 (ex->math_cos(ang) >> 5) - 0x80);
                        rot.vz += 0x258;
                    }
                }
                if (frame < 0x10) {
                    rot2.vx = 0x400;
                    rot2.pad = 1;
                    rot2.vy = 0;
                    rot2.vz = 0;
                    env->otZ = 0x100;
                    ex->btlfx_drawRings((u8 *)ex->btlfx_getFileData(entry->file, FX_EARTH_SHAKE_CYAN_RING), &work->pos8, &rot2,
                                      (ex->math_sin((frame << 8) >> 2) >> 1) + 0x800, 0, BTLFX_ABR_ADD,
                                      ex->math_cos((frame << 8) >> 2));
                    env->otZ = 0;
                }
                if (frame >= 0x20) {
                    work->state0 = 2;
                    entry->frame = -1;
                    ex->btlfx_stopEmitter(work->emit18);
                }
            }
            }
            break;
        case 2: {
            s32 flash;
            Pool6CEntry *emitter;

            t = (frame << 12) / 40;
            tint = 0x89;
            if (frame == 1) {
                emitter = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, FX_EARTH_SHAKE_ROCK_DEBRIS_2), (s32)&work->pos8);
                if (emitter != NULL) {
                    emitter->otZ = 0x100;
                }
            }
            env->camera->points[3].vx = work->pos8.vx;
            env->camera->points[3].vy = work->pos8.vy;
            env->camera->points[3].vz = work->pos8.vz;
            env->camera->points[3].vy =
                (s16)(env->camera->points[3].vy - 0x100) - ((0x1000 - t) >> 7) * ((frame & 1) * 2 - 1);
            if (frame == 0) {
                ex->btlfx_drawScreenTint((u32 *)(env->ot + 2), BTLFX_ABR_SUB, 0x80, 0x80, 0x80);
            } else if (frame < 9) {
                flash = 0x80 - (((frame - 1) << 7) >> 3);
                ex->btlfx_drawScreenTint((u32 *)(env->ot + 2), BTLFX_ABR_ADD, flash, flash, flash);
            }
            if (frame >= 0x28) {
                work->state0 = 3;
                return 1;
            }
            break;
        }
        }
        if (tint != 0) {
            CVECTOR col;
            SVECTOR rot;
            VECTOR scale;

            col.r = col.g = col.b = tint;
            rot.vz = -8;
            scale.vx = 0x1020;
            scale.vy = 0x1020;
            scale.vz = 0x1000;
            rot.vx = 0;
            rot.vy = 0;
            ex->btlfx_drawScreenGrid(2, 0xA0, 0x70, 0x50, 0x38, 4, 4, 2, 2, &rot, &scale, &col, 0, 0);
        }
        work->frame4++;
    }
    return 0;
}

s32 fx_186_op81(s32 mode, Fx186Work81 *work, Unk800F78A4 *env, Unk80100C78Entry *entry, s32 *args) {
    BtlfxExports *ex = BTLFX_EXPORTS(env);
    SVECTOR unitPos;
    SVECTOR rot;
    s32 frame;
    s32 i;
    s32 unit;
    s32 t;
    s32 w0;
    s32 w1;

    if (mode == 0) {
        *args = 0x10C;
    } else if (mode == 1) {
        fx_186_env = env;
        {
            s32 n = env->targets->targetCount;
            /* The dead conditional store splits the block, so n lives across blocks and is left to global-alloc.
             * That leaves two local qtys here, which local-alloc sorts properly (its hand-written three-qty sort
             * compares qty numbers instead of positions), giving %hi(fx_186_env) v0 and env->targets v1. */
            if (n != 0) unit = 0;
            unit = 0;
            work->state0 = 0;
            work->count104 = n;
            if (n <= 0) {
                return 0;
            }
        }
        do {
            ex->btlfx_getUnitHomePos(unit, (s16 *)&work->pos4[unit]);
            ex->btlfx_getUnitRot(unit, (s16 *)&work->rot44[unit]);
            ((s32 *)&work->rotC4[unit])[0] = ((s32 *)&work->rot44[unit])[0];
            ((s32 *)&work->rotC4[unit])[1] = ((s32 *)&work->rot44[unit])[1];
            work->spin84[unit].vx = 0x2000 / ((ex->btlfx_rand() % 2 + 1) * 40);
            work->spin84[unit].vy = 0x2000 / (((ex->btlfx_rand() & 1) + 1) * 40);
            work->spin84[unit].vz = 0x2000 / (((ex->btlfx_rand() & 1) + 1) * 40);
            work->height108 = -0x578 - (ex->btlfx_rand() & 0x3FF);
            unit++;
        } while (unit < work->count104);
    } else {
        frame = entry->frame;
        switch (work->state0) {
        case 0:
            t = frame << 7;
            for (i = 0; i < work->count104; i++) {
                w1 = ((s32 *)&work->pos4[i])[1];
                w0 = ((s32 *)&work->pos4[i])[0];
                ((s32 *)&unitPos)[1] = w1;
                ((s32 *)&unitPos)[0] = w0;
                unitPos.vy -= (t >> 4) + (ex->btlfx_rand() & 0x7F);
                ex->btlfx_setUnitPos(i, (s16 *)&unitPos);
            }
            if (frame >= 0x20) {
                work->state0 = 1;
                entry->frame = -1;
            }
            break;
        case 1:
            t = (frame << 12) / 40;
            for (i = 0; i < work->count104; i++) {
                w1 = ((s32 *)&work->pos4[i])[1];
                w0 = ((s32 *)&work->pos4[i])[0];
                ((s32 *)&unitPos)[1] = w1;
                ((s32 *)&unitPos)[0] = w0;
                unitPos.vy += (work->height108 * ex->math_sin(t >> 1)) >> 12;
                ex->btlfx_setUnitPos(i, (s16 *)&unitPos);
                work->rotC4[i].vx += work->spin84[i].vx;
                work->rotC4[i].vy += work->spin84[i].vy;
                work->rotC4[i].vz += work->spin84[i].vz;
                ex->btlfx_setUnitRot(i, (s16 *)&work->rotC4[i]);
            }
            if (frame >= 0x28) {
                for (i = 0; i < work->count104; i++) {
                    ex->btlfx_setUnitRot(i, (s16 *)&work->rot44[i]);
                    ex->btlfx_getUnitHomePos(i, (s16 *)&unitPos);
                    ex->btlfx_setUnitPos(i, (s16 *)&unitPos);
                }
                work->state0 = 2;
                entry->frame = -1;
            }
            break;
        case 2:
            rot.vx = 0x400;
            rot.vz = 0;
            rot.vy = 0;
            rot.pad = 1;
            t = (frame << 12) / 7;
            for (i = 0; i < work->count104; i++) {
                ex->btlfx_drawRings((u8 *)ex->btlfx_getFileData(entry->file, FX_EARTH_SHAKE_GREY_RING), &work->pos4[i], &rot, 0x1000, frame, BTLFX_ABR_ADD,
                                  ex->math_cos(t >> 2));
            }
            if (frame >= 7) {
                return 1;
            }
            break;
        }
    }
    return 0;
}

INCLUDE_RODATA("asm/us/fx_186/nonmatchings/fx_186", fx_186_headerPtr);
