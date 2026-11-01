#include "fxmod/fx_063.h"

static s32 fxm_easeLerp(s32 curve, s32 n, s32 t, s32 from, s32 to) {
    BtlfxExports *ex = BTLFX_EXPORTS(fx_063_env);
    s32 k = 0x20000000 / (n * n);
    s32 r = 0;

    switch (curve) {
    case 0:
        r = 0x1000 - (((0x10000000 / n) * t) >> 16);
        break;
    case 1:
        r = (k * (n - t) * (n - t)) >> 17;
        break;
    case 2:
        r = 0x1000 - ((k * t * t) >> 17);
        break;
    }
    return ex->btlfx_lerp(from, to, r);
}

s32 fx_063_op80(s32 mode, Fx063Work *work, Unk800F78A4 *env, Unk80100C78Entry *entry, s32 *args) {
    BtlfxExports *ex = BTLFX_EXPORTS(env);
    SVECTOR home;
    SVECTOR ang;
    SVECTOR start;
    VECTOR b;
    s32 frame;
    u8 *strip;
    s32 n;
    Fx063Strip *tr;
    Fx063Pt *pt;
    s32 i;
    s32 j;
    s32 r;
    s32 s;
    s32 t;
    s32 u;
    s32 k;
    s32 c;

    if (mode == 0) {
        *args = 0xDC;
    } else if (mode == 1) {
        fx_063_env = env;
        ex->btlfx_initEffModels(fx_063_effModels, 4);
        for (i = 15; i >= 0; i--) {
            work->timers0[i] = 0;
        }
        for (i = 0; i < 10; i++) {
            fx_063_strips[i].timer100 = 0;
            ex->btlfx_drawTrail((u8 *)ex->btlfx_getFileData(entry->file, 0xD), 0, NULL, -1, 0x1000, 0x1000, 1,
                                (u8 *)&fx_063_strips[i], 0);
        }
        if (env->targets->centerZ < 0) {
            env->camera->points[0].vz = work->posB8.vz = work->pos94.vz = work->posA4.vz = -0x30C;
        } else {
            env->camera->points[0].vz = work->posB8.vz = work->pos94.vz = work->posA4.vz = 0x30C;
        }
        env->camera->points[0].vx = work->posB8.vx = work->pos94.vx = work->posA4.vx = 0;
        env->camera->points[0].vy = work->pos94.vy = work->posA4.vy = -0x600;
        work->posB8.vy = 0;
        for (i = 0; i < env->targets->targetCount; i++) {
            fx_063_unitBob[i].timer0 = (ex->btlfx_rand() & 7) + 0x30;
            fx_063_unitBob[i].phase4 = 0;
            fx_063_unitBob[i].phaseSpeed8 = ex->btlfx_rand() % 511 + 0x100;
            ex->btlfx_getUnitHomePos(i, (s16 *)&fx_063_unitBob[i].homeC);
            ex->btlfx_getUnitHomePos(i, (s16 *)&fx_063_unitBob[i].pos14);
        }
        work->models40[0].model0 = ex->Hi_RegisterTexEffModel((BtlModelHeader *)ex->btlfx_getFileData(entry->file, 8), 0x137,
                                                         0xFFFF, 0, 0x80);
        work->models40[0].rot4.vx = 0;
        work->models40[0].rot4.vy = 0;
        work->models40[0].rot4.vz = 0;
        work->models40[0].scaleC.vx = 0;
        work->models40[0].scaleC.vy = 0;
        work->models40[0].scaleC.vz = 0;
        work->models40[1].model0 = ex->Hi_RegisterGouEffModel((BtlModelHeader *)ex->btlfx_getFileData(entry->file, 0xB));
        work->models40[1].rot4.vx = 0;
        work->models40[1].rot4.vy = 0;
        work->models40[1].rot4.vz = 0;
        work->models40[1].scaleC.vx = 0;
        work->models40[1].scaleC.vy = 0;
        work->models40[1].scaleC.vz = 0;
        work->models40[2].model0 = ex->Hi_RegisterGouEffModel((BtlModelHeader *)ex->btlfx_getFileData(entry->file, 0xC));
        work->models40[2].rot4.vx = 0;
        work->models40[2].rot4.vy = 0;
        work->models40[2].rot4.vz = 0;
        work->models40[2].scaleC.vx = 0;
        work->models40[2].scaleC.vy = 0x100;
        work->models40[2].scaleC.vz = 0;
        work->modelAC = ex->Hi_RegisterTexEffModel((BtlModelHeader *)ex->btlfx_getFileData(entry->file, 9), 0x17A, 0xFFFF, 0,
                                                 0x80);
        work->rotB0.vx = 0;
        work->rotB0.vy = 0;
        work->rotB0.vz = 0;
        work->scaleC8.vx = 0x400;
        work->scaleC8.vy = 0;
        work->scaleC8.vz = 0x400;
    } else {
        frame = entry->frame;
        switch (frame) {
        case 0:
            ex->btlfx_drawBgOffscreen(0x280, 0x100, 0x70, 0x70, fx_063_bgOt, work->posB8.vx, work->posB8.vz, 0x200);
            work->timers0[9] = 1;
            work->timers0[10] = 1;
            break;
        case 1:
            work->timers0[1] = 1;
            work->timers0[7] = 1;
            work->timers0[4] = 1;
            work->timers0[2] = 0x10;
            work->timers0[0] = 0x30;
            work->timers0[5] = 0x10;
            break;
        case 42:
            work->timers0[3] = 8;
            work->timers0[6] = 8;
            break;
        case 49:
            work->timers0[1] = 0;
            break;
        case 51:
            work->timers0[4] = 0;
            break;
        }
        if (work->timers0[2] != 0) {
            work->timers0[2]--;
            work->models40[0].scaleC.vx = work->models40[0].scaleC.vy = work->models40[0].scaleC.vz =
                fxm_easeLerp(2, 0x10, work->timers0[2], 0, 0x400);
            work->models40[1].scaleC.vx = work->models40[1].scaleC.vy = work->models40[1].scaleC.vz =
                fxm_easeLerp(2, 0x10, work->timers0[2], 0, 0x300);
            work->models40[2].scaleC.vx = work->models40[2].scaleC.vz = fxm_easeLerp(2, 0x10, work->timers0[2], 0, 0x480);
        }
        if (work->timers0[3] != 0) {
            work->timers0[3]--;
            work->models40[0].scaleC.vx = work->models40[0].scaleC.vy = work->models40[0].scaleC.vz =
                fxm_easeLerp(2, 8, work->timers0[3], 0x400, 0x200);
            work->models40[1].scaleC.vx = work->models40[1].scaleC.vy = work->models40[1].scaleC.vz =
                fxm_easeLerp(2, 8, work->timers0[3], 0x300, 0x200);
            work->models40[2].scaleC.vx = work->models40[2].scaleC.vz = fxm_easeLerp(2, 8, work->timers0[3], 0x480, 0x200);
        }
        if (work->timers0[5] != 0) {
            work->timers0[5]--;
            work->scaleC8.vy += 0x40;
        }
        if (work->timers0[6] != 0) {
            work->timers0[6]--;
            work->scaleC8.vy -= 0x80;
        }
        if (work->timers0[1] != 0) {
            ex->btlfx_aimAngles((s16 *)&work->posA4, (s16 *)&env->camera->eye, (s16 *)&work->models40[1].rot4);
            work->models40[0].rot4.vx = work->models40[1].rot4.vx + 0x800;
            work->models40[0].rot4.vy = work->models40[1].rot4.vy;
            work->models40[2].rot4.vy = work->models40[1].rot4.vy;
            work->models40[2].rot4.vx = work->models40[1].rot4.vx + 0x400;
            work->models40[1].rot4.vx -= 0x400;
            ex->Hi_DrawEffModel(&work->models40[1].rot4, &work->pos94, (s32)&work->models40[1].scaleC, work->models40[1].model0);
            ex->Hi_DrawEffModel(&work->models40[2].rot4, &work->pos94, (s32)&work->models40[2].scaleC, work->models40[2].model0);
            ex->Hi_DrawEffModel(&work->models40[0].rot4, &work->pos94, (s32)&work->models40[0].scaleC, work->models40[0].model0);
            ex->Hi_ModifyEffModelAbr(work->models40[0].model0, 1);
        }
        if (work->timers0[3] != 0) {
            c = -((8 - work->timers0[3]) << 5);
            ex->Hi_ModifyEffModelRGB(work->models40[1].model0, c, c, c);
            ex->Hi_ModifyEffModelRGB(work->models40[2].model0, c, c, c);
            ex->Hi_ModifyEffModelRGB(work->models40[0].model0, c, c, c);
        }
        if (work->timers0[4] != 0) {
            ex->Hi_DrawEffModel(&work->rotB0, &work->posB8, (s32)&work->scaleC8, work->modelAC);
        }
        if (work->timers0[7] != 0) {
            for (i = 0; i < env->targets->targetCount; i++) {
                if (fx_063_unitBob[i].timer0 != 0) {
                    if (--fx_063_unitBob[i].timer0 != 0) {
                        fx_063_unitBob[i].phase4 += fx_063_unitBob[i].phaseSpeed8;
                        if (fx_063_unitBob[i].timer0 < 0x10) {
                            fx_063_unitBob[i].pos14.vy = fx_063_unitBob[i].homeC.vy + fxm_easeLerp(1, 0x10, fx_063_unitBob[i].timer0, -0x80, 0) +
                                ((ex->math_sin(fx_063_unitBob[i].phase4) * fx_063_unitBob[i].timer0) >> 10);
                        }
                        if (fx_063_unitBob[i].timer0 >= 0x10 && fx_063_unitBob[i].timer0 < 0x20) {
                            k = ((ex->math_sin(fx_063_unitBob[i].phase4) * 16) >> 10) - 0x80;
                            fx_063_unitBob[i].pos14.vy = fx_063_unitBob[i].homeC.vy + k;
                        }
                        if (fx_063_unitBob[i].timer0 >= 0x20 && fx_063_unitBob[i].timer0 < 0x30) {
                            fx_063_unitBob[i].pos14.vy = fx_063_unitBob[i].homeC.vy + fxm_easeLerp(2, 0x10, fx_063_unitBob[i].timer0 - 0x20, 0, -0x80) +
                                ((ex->math_sin(fx_063_unitBob[i].phase4) * (0x30 - fx_063_unitBob[i].timer0)) >> 10);
                        }
                        ex->btlfx_setUnitPos(i, (s16 *)&fx_063_unitBob[i].pos14);
                    } else {
                        ex->btlfx_getUnitHomePos(i, (s16 *)&home);
                        ex->btlfx_setUnitPos(i, (s16 *)&home);
                    }
                }
            }
        }
        if (work->timers0[0] != 0) {
            work->timers0[0]--;
            for (n = 0; n < 10; n++) {
                tr = &fx_063_strips[n];
                if (tr->timer100 != 0) {
                    tr->timer100--;
                    tr->lastSlot0 = tr->count2 = 10;
                    tr->head1 = 0;
                    tr->angleEC += tr->spinF4;
                    tr->angleF0 += tr->spinF8;
                    s = work->models40[0].scaleC.vx;
                    pt = tr->pts4;
                    r = (ex->math_sin(tr->angleEC) * s) >> 12;
                    pt->vy = start.vy = work->pos94.vy + ((ex->math_cos(tr->angleEC) * s) >> 12);
                    pt->vx = start.vx = work->pos94.vx + ((ex->math_cos(tr->angleF0) * r) >> 12);
                    pt->vz = start.vz = work->pos94.vz + ((ex->math_sin(tr->angleF0) * r) >> 12);
                    strip = (u8 *)tr;
                    pt->halfWidth6 = tr->widthFC;
                    ex->btlfx_aimAngles((s16 *)&work->posA4, (s16 *)&start, (s16 *)&ang);
                    pt++;
                    for (j = 0; j < 9; j++, pt++) {
                        t = ((j + 1) * 0x2E8BA) >> 12;
                        u = 0x200 - t;
                        b.vx = (u * u) >> 6;
                        b.vy = (t * u) >> 4;
                        b.vz = (t * t) >> 6;
                        tr->jitter5C[j].vx += tr->jitterVelA4[j];
                        tr->jitter5C[j].vz += tr->jitterVelC8[j];
                        pt->vx = tr->jitter5C[j].vx +
                            ((start.vx * b.vx + start.vx * b.vz +
                              (work->pos94.vx + ((ex->math_sin(ang.vy) * r) >> 11)) * b.vy) >> 12);
                        pt->vz = tr->jitter5C[j].vz +
                            ((start.vz * b.vx + start.vz * b.vz +
                              (work->pos94.vz + ((ex->math_cos(ang.vy) * r) >> 11)) * b.vy) >> 12);
                        pt->vy = ex->btlfx_lerpRatio(start.vy, 0, j + 1, 11);
                        pt->halfWidth6 = tr->widthFC;
                    }
                    pt->vx = start.vx;
                    pt->vz = start.vz;
                    pt->vy = 0;
                    pt->halfWidth6 = tr->widthFC;
                    if (tr->timer100 < 4) {
                        ex->btlfx_drawTrail((u8 *)ex->btlfx_getFileData(entry->file, 0xD), 0, NULL, frame, 0x1000,
                                            tr->timer100 << 10, 1, strip, 0);
                    } else {
                        ex->btlfx_drawTrail((u8 *)ex->btlfx_getFileData(entry->file, 0xD), 0, NULL, frame, 0x1000,
                                            0x1000, 1, strip, 0);
                    }
                } else if (work->timers0[0] >= 7) {
                    tr->timer100 = ex->btlfx_randRange(2, 6);
                    tr->angleEC = ex->btlfx_randRange(0x100, 0x400);
                    tr->angleF0 = ex->btlfx_rand() & 0xFFF;
                    tr->spinF4 = ex->btlfx_randRange(-0x10, 0x10);
                    tr->spinF8 = ex->btlfx_randRange(-0x80, 0x80);
                    tr->widthFC = ex->btlfx_randRange(0x40, 0x80);
                    for (j = 0; j < 9; j++) {
                        tr->jitter5C[j].vz = 0;
                        tr->jitter5C[j].vx = 0;
                        tr->jitterVelA4[j] = ex->btlfx_randRange(-0x60, 0x60);
                        tr->jitterVelC8[j] = ex->btlfx_randRange(-0x60, 0x60);
                    }
                }
            }
        }
        if (work->timers0[9] != 0) {
            ex->btlfx_drawBackdrop((BtlBgLayers *)ex->btlfx_getFileData(entry->file, 7), work->timers0[9] - 1);
            work->timers0[9]++;
        }
        if (work->timers0[10] != 0) {
            ex->btlfx_drawBackdrop((BtlBgLayers *)ex->btlfx_getFileData(entry->file, 0xA), work->timers0[10] - 1);
            work->timers0[10]++;
        }
        return frame >= 0x46;
    }
    return 0;
}

INCLUDE_RODATA("asm/us/fx_063/nonmatchings/fx_063", fx_063_headerPtr);
