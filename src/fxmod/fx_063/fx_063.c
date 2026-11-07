#include "fxmod/fx_063.h"

static s32 fxm_easeLerp(s32 curve, s32 n, s32 t, s32 from, s32 to) {
    BtlfxExports *ex = BTLFX_EXPORTS(fx_063_env);
    s32 invSq = 0x20000000 / (n * n);
    s32 ratio = 0;

    switch (curve) {
    case 0:
        ratio = 0x1000 - (((0x10000000 / n) * t) >> 16);
        break;
    case 1:
        ratio = (invSq * (n - t) * (n - t)) >> 17;
        break;
    case 2:
        ratio = 0x1000 - ((invSq * t * t) >> 17);
        break;
    }
    return ex->btlfx_lerp(from, to, ratio);
}

s32 fx_063_op80(s32 mode, Fx063Work *work, Unk800F78A4 *env, Unk80100C78Entry *entry, s32 *args) {
    BtlfxExports *ex = BTLFX_EXPORTS(env);
    SVECTOR home;
    SVECTOR ang;
    SVECTOR start;
    VECTOR weights;
    s32 frame;
    u8 *strip;
    s32 strand;
    Fx063Strip *tr;
    Fx063Pt *pt;
    s32 i;
    s32 j;
    s32 radius;
    s32 reach;
    s32 t;
    s32 u;
    s32 bob;
    s32 tint;

    if (mode == 0) {
        *args = sizeof(Fx063Work);
    } else if (mode == 1) {
        fx_063_env = env;
        ex->btlfx_initEffModels(fx_063_effModels, FX_DEMI_MODEL_COUNT);
        for (i = FX_DEMI_TIMERS - 1; i >= 0; i--) {
            work->timers0[i] = 0;
        }
        for (i = 0; i < FX_DEMI_STRANDS; i++) {
            fx_063_strips[i].timer100 = 0;
            ex->btlfx_drawTrail((u8 *)ex->btlfx_getFileData(entry->file, FX_DEMI_GLOW_TRAIL), 0, NULL, -1, 0x1000, 0x1000, BTLFX_ABR_ADD,
                                (u8 *)&fx_063_strips[i], 0);
        }
        if (env->targets->centerZ < 0) {
            env->camera->points[0].vz = work->posB8.vz = work->pos94.vz = work->posA4.vz = -FX_DEMI_CENTRE_Z;
        } else {
            env->camera->points[0].vz = work->posB8.vz = work->pos94.vz = work->posA4.vz = FX_DEMI_CENTRE_Z;
        }
        env->camera->points[0].vx = work->posB8.vx = work->pos94.vx = work->posA4.vx = 0;
        env->camera->points[0].vy = work->pos94.vy = work->posA4.vy = FX_DEMI_CENTRE_Y;
        work->posB8.vy = 0;
        for (i = 0; i < env->targets->targetCount; i++) {
            fx_063_unitBob[i].timer0 = (ex->btlfx_rand() & 7) + 3 * FX_DEMI_BOB_PHASE_FRAMES;
            fx_063_unitBob[i].phase4 = 0;
            fx_063_unitBob[i].phaseSpeed8 = ex->btlfx_rand() % 511 + 0x100;
            ex->btlfx_getUnitHomePos(i, (s16 *)&fx_063_unitBob[i].homeC);
            ex->btlfx_getUnitHomePos(i, (s16 *)&fx_063_unitBob[i].pos14);
        }
        work->models40[0].model0 = ex->Hi_RegisterTexEffModel((BtlModelHeader *)ex->btlfx_getFileData(entry->file, FX_DEMI_SPHERE), FX_DEMI_SPHERE_TPAGE,
                                                         FX_DEMI_SPHERE_CLUT, 0, FX_DEMI_SPHERE_V);
        work->models40[0].rot4.vx = 0;
        work->models40[0].rot4.vy = 0;
        work->models40[0].rot4.vz = 0;
        work->models40[0].scaleC.vx = 0;
        work->models40[0].scaleC.vy = 0;
        work->models40[0].scaleC.vz = 0;
        work->models40[1].model0 = ex->Hi_RegisterGouEffModel((BtlModelHeader *)ex->btlfx_getFileData(entry->file, FX_DEMI_BOWL));
        work->models40[1].rot4.vx = 0;
        work->models40[1].rot4.vy = 0;
        work->models40[1].rot4.vz = 0;
        work->models40[1].scaleC.vx = 0;
        work->models40[1].scaleC.vy = 0;
        work->models40[1].scaleC.vz = 0;
        work->models40[2].model0 = ex->Hi_RegisterGouEffModel((BtlModelHeader *)ex->btlfx_getFileData(entry->file, FX_DEMI_BOWL_2));
        work->models40[2].rot4.vx = 0;
        work->models40[2].rot4.vy = 0;
        work->models40[2].rot4.vz = 0;
        work->models40[2].scaleC.vx = 0;
        work->models40[2].scaleC.vy = 0x100;
        work->models40[2].scaleC.vz = 0;
        work->modelAC = ex->Hi_RegisterTexEffModel((BtlModelHeader *)ex->btlfx_getFileData(entry->file, FX_DEMI_CAP), FX_DEMI_CAP_TPAGE, FX_DEMI_CAP_CLUT, 0,
                                                 FX_DEMI_CAP_V);
        work->rotB0.vx = 0;
        work->rotB0.vy = 0;
        work->rotB0.vz = 0;
        work->scaleC8.vx = FX_DEMI_CAP_WIDTH;
        work->scaleC8.vy = 0;
        work->scaleC8.vz = FX_DEMI_CAP_WIDTH;
    } else {
        frame = entry->frame;
        switch (frame) {
        case 0:
            ex->btlfx_drawBgOffscreen(FX_DEMI_BG_VRAM_X, FX_DEMI_BG_VRAM_Y, FX_DEMI_BG_VRAM_W, FX_DEMI_BG_VRAM_H, fx_063_bgOt, work->posB8.vx, work->posB8.vz, 0x200);
            work->timers0[FX_DEMI_T_BACKDROP] = 1;
            work->timers0[FX_DEMI_T_BACKDROP_2] = 1;
            break;
        case FX_DEMI_SPHERE_FRAME:
            work->timers0[FX_DEMI_T_SPHERE] = 1;
            work->timers0[FX_DEMI_T_BOB] = 1;
            work->timers0[FX_DEMI_T_CAP] = 1;
            work->timers0[FX_DEMI_T_GROW] = FX_DEMI_GROW_FRAMES;
            work->timers0[FX_DEMI_T_STRANDS] = FX_DEMI_STRANDS_FRAMES;
            work->timers0[FX_DEMI_T_CAP_RISE] = FX_DEMI_GROW_FRAMES;
            break;
        case FX_DEMI_SHRINK_FRAME:
            work->timers0[FX_DEMI_T_SHRINK] = FX_DEMI_SHRINK_FRAMES;
            work->timers0[FX_DEMI_T_CAP_SINK] = FX_DEMI_SHRINK_FRAMES;
            break;
        case FX_DEMI_END_DRAW_FRAME:
            work->timers0[FX_DEMI_T_SPHERE] = 0;
            break;
        case FX_DEMI_CAP_END_FRAME:
            work->timers0[FX_DEMI_T_CAP] = 0;
            break;
        }
        if (work->timers0[FX_DEMI_T_GROW] != 0) {
            work->timers0[FX_DEMI_T_GROW]--;
            work->models40[0].scaleC.vx = work->models40[0].scaleC.vy = work->models40[0].scaleC.vz =
                fxm_easeLerp(2, FX_DEMI_GROW_FRAMES, work->timers0[FX_DEMI_T_GROW], 0, FX_DEMI_SPHERE_SCALE);
            work->models40[1].scaleC.vx = work->models40[1].scaleC.vy = work->models40[1].scaleC.vz =
                fxm_easeLerp(2, FX_DEMI_GROW_FRAMES, work->timers0[FX_DEMI_T_GROW], 0, FX_DEMI_BOWL_SCALE);
            work->models40[2].scaleC.vx = work->models40[2].scaleC.vz = fxm_easeLerp(2, FX_DEMI_GROW_FRAMES, work->timers0[FX_DEMI_T_GROW], 0, FX_DEMI_BOWL_2_SCALE);
        }
        if (work->timers0[FX_DEMI_T_SHRINK] != 0) {
            work->timers0[FX_DEMI_T_SHRINK]--;
            work->models40[0].scaleC.vx = work->models40[0].scaleC.vy = work->models40[0].scaleC.vz =
                fxm_easeLerp(2, FX_DEMI_SHRINK_FRAMES, work->timers0[FX_DEMI_T_SHRINK], FX_DEMI_SPHERE_SCALE, FX_DEMI_SHRUNK_SCALE);
            work->models40[1].scaleC.vx = work->models40[1].scaleC.vy = work->models40[1].scaleC.vz =
                fxm_easeLerp(2, FX_DEMI_SHRINK_FRAMES, work->timers0[FX_DEMI_T_SHRINK], FX_DEMI_BOWL_SCALE, FX_DEMI_SHRUNK_SCALE);
            work->models40[2].scaleC.vx = work->models40[2].scaleC.vz = fxm_easeLerp(2, FX_DEMI_SHRINK_FRAMES, work->timers0[FX_DEMI_T_SHRINK], FX_DEMI_BOWL_2_SCALE, FX_DEMI_SHRUNK_SCALE);
        }
        if (work->timers0[FX_DEMI_T_CAP_RISE] != 0) {
            work->timers0[FX_DEMI_T_CAP_RISE]--;
            work->scaleC8.vy += 0x40;
        }
        if (work->timers0[FX_DEMI_T_CAP_SINK] != 0) {
            work->timers0[FX_DEMI_T_CAP_SINK]--;
            work->scaleC8.vy -= 0x80;
        }
        if (work->timers0[FX_DEMI_T_SPHERE] != 0) {
            ex->btlfx_aimAngles((s16 *)&work->posA4, (s16 *)&env->camera->eye, (s16 *)&work->models40[1].rot4);
            work->models40[0].rot4.vx = work->models40[1].rot4.vx + 0x800;
            work->models40[0].rot4.vy = work->models40[1].rot4.vy;
            work->models40[2].rot4.vy = work->models40[1].rot4.vy;
            work->models40[2].rot4.vx = work->models40[1].rot4.vx + 0x400;
            work->models40[1].rot4.vx -= 0x400;
            ex->Hi_DrawEffModel(&work->models40[1].rot4, &work->pos94, (s32)&work->models40[1].scaleC, work->models40[1].model0);
            ex->Hi_DrawEffModel(&work->models40[2].rot4, &work->pos94, (s32)&work->models40[2].scaleC, work->models40[2].model0);
            ex->Hi_DrawEffModel(&work->models40[0].rot4, &work->pos94, (s32)&work->models40[0].scaleC, work->models40[0].model0);
            ex->Hi_ModifyEffModelAbr(work->models40[0].model0, BTLFX_ABR_ADD);
        }
        if (work->timers0[FX_DEMI_T_SHRINK] != 0) {
            tint = -((FX_DEMI_SHRINK_FRAMES - work->timers0[FX_DEMI_T_SHRINK]) << 5);
            ex->Hi_ModifyEffModelRGB(work->models40[1].model0, tint, tint, tint);
            ex->Hi_ModifyEffModelRGB(work->models40[2].model0, tint, tint, tint);
            ex->Hi_ModifyEffModelRGB(work->models40[0].model0, tint, tint, tint);
        }
        if (work->timers0[FX_DEMI_T_CAP] != 0) {
            ex->Hi_DrawEffModel(&work->rotB0, &work->posB8, (s32)&work->scaleC8, work->modelAC);
        }
        if (work->timers0[FX_DEMI_T_BOB] != 0) {
            for (i = 0; i < env->targets->targetCount; i++) {
                if (fx_063_unitBob[i].timer0 != 0) {
                    if (--fx_063_unitBob[i].timer0 != 0) {
                        fx_063_unitBob[i].phase4 += fx_063_unitBob[i].phaseSpeed8;
                        if (fx_063_unitBob[i].timer0 < FX_DEMI_BOB_PHASE_FRAMES) {
                            fx_063_unitBob[i].pos14.vy = fx_063_unitBob[i].homeC.vy + fxm_easeLerp(1, FX_DEMI_BOB_PHASE_FRAMES, fx_063_unitBob[i].timer0, -FX_DEMI_LIFT_HEIGHT, 0) +
                                ((ex->math_sin(fx_063_unitBob[i].phase4) * fx_063_unitBob[i].timer0) >> 10);
                        }
                        if (fx_063_unitBob[i].timer0 >= FX_DEMI_BOB_PHASE_FRAMES && fx_063_unitBob[i].timer0 < 2 * FX_DEMI_BOB_PHASE_FRAMES) {
                            bob = ((ex->math_sin(fx_063_unitBob[i].phase4) * 16) >> 10) - FX_DEMI_LIFT_HEIGHT;
                            fx_063_unitBob[i].pos14.vy = fx_063_unitBob[i].homeC.vy + bob;
                        }
                        if (fx_063_unitBob[i].timer0 >= 2 * FX_DEMI_BOB_PHASE_FRAMES && fx_063_unitBob[i].timer0 < 3 * FX_DEMI_BOB_PHASE_FRAMES) {
                            fx_063_unitBob[i].pos14.vy = fx_063_unitBob[i].homeC.vy + fxm_easeLerp(2, FX_DEMI_BOB_PHASE_FRAMES, fx_063_unitBob[i].timer0 - 2 * FX_DEMI_BOB_PHASE_FRAMES, 0, -FX_DEMI_LIFT_HEIGHT) +
                                ((ex->math_sin(fx_063_unitBob[i].phase4) * (3 * FX_DEMI_BOB_PHASE_FRAMES - fx_063_unitBob[i].timer0)) >> 10);
                        }
                        ex->btlfx_setUnitPos(i, (s16 *)&fx_063_unitBob[i].pos14);
                    } else {
                        ex->btlfx_getUnitHomePos(i, (s16 *)&home);
                        ex->btlfx_setUnitPos(i, (s16 *)&home);
                    }
                }
            }
        }
        if (work->timers0[FX_DEMI_T_STRANDS] != 0) {
            work->timers0[FX_DEMI_T_STRANDS]--;
            for (strand = 0; strand < FX_DEMI_STRANDS; strand++) {
                tr = &fx_063_strips[strand];
                if (tr->timer100 != 0) {
                    tr->timer100--;
                    tr->lastSlot0 = tr->count2 = FX_DEMI_STRAND_POINTS - 1;
                    tr->head1 = 0;
                    tr->angleEC += tr->spinF4;
                    tr->angleF0 += tr->spinF8;
                    reach = work->models40[0].scaleC.vx;
                    pt = tr->pts4;
                    radius = (ex->math_sin(tr->angleEC) * reach) >> 12;
                    pt->vy = start.vy = work->pos94.vy + ((ex->math_cos(tr->angleEC) * reach) >> 12);
                    pt->vx = start.vx = work->pos94.vx + ((ex->math_cos(tr->angleF0) * radius) >> 12);
                    pt->vz = start.vz = work->pos94.vz + ((ex->math_sin(tr->angleF0) * radius) >> 12);
                    strip = (u8 *)tr;
                    pt->halfWidth6 = tr->widthFC;
                    ex->btlfx_aimAngles((s16 *)&work->posA4, (s16 *)&start, (s16 *)&ang);
                    pt++;
                    for (j = 0; j < 9; j++, pt++) {
                        t = ((j + 1) * 0x2E8BA) >> 12;
                        u = 0x200 - t;
                        weights.vx = (u * u) >> 6;
                        weights.vy = (t * u) >> 4;
                        weights.vz = (t * t) >> 6;
                        tr->jitter5C[j].vx += tr->jitterVelA4[j];
                        tr->jitter5C[j].vz += tr->jitterVelC8[j];
                        pt->vx = tr->jitter5C[j].vx +
                            ((start.vx * weights.vx + start.vx * weights.vz +
                              (work->pos94.vx + ((ex->math_sin(ang.vy) * radius) >> 11)) * weights.vy) >> 12);
                        pt->vz = tr->jitter5C[j].vz +
                            ((start.vz * weights.vx + start.vz * weights.vz +
                              (work->pos94.vz + ((ex->math_cos(ang.vy) * radius) >> 11)) * weights.vy) >> 12);
                        pt->vy = ex->btlfx_lerpRatio(start.vy, 0, j + 1, 11);
                        pt->halfWidth6 = tr->widthFC;
                    }
                    pt->vx = start.vx;
                    pt->vz = start.vz;
                    pt->vy = 0;
                    pt->halfWidth6 = tr->widthFC;
                    if (tr->timer100 < FX_DEMI_STRAND_FADE_FRAMES) {
                        ex->btlfx_drawTrail((u8 *)ex->btlfx_getFileData(entry->file, FX_DEMI_GLOW_TRAIL), 0, NULL, frame, 0x1000,
                                            tr->timer100 * (0x1000 / FX_DEMI_STRAND_FADE_FRAMES), BTLFX_ABR_ADD, strip, 0);
                    } else {
                        ex->btlfx_drawTrail((u8 *)ex->btlfx_getFileData(entry->file, FX_DEMI_GLOW_TRAIL), 0, NULL, frame, 0x1000,
                                            0x1000, BTLFX_ABR_ADD, strip, 0);
                    }
                } else if (work->timers0[FX_DEMI_T_STRANDS] >= 7) {
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
        if (work->timers0[FX_DEMI_T_BACKDROP] != 0) {
            ex->btlfx_drawLayerTexture((BtlBgLayers *)ex->btlfx_getFileData(entry->file, FX_DEMI_BACKDROP), work->timers0[FX_DEMI_T_BACKDROP] - 1);
            work->timers0[FX_DEMI_T_BACKDROP]++;
        }
        if (work->timers0[FX_DEMI_T_BACKDROP_2] != 0) {
            ex->btlfx_drawLayerTexture((BtlBgLayers *)ex->btlfx_getFileData(entry->file, FX_DEMI_BACKDROP_2), work->timers0[FX_DEMI_T_BACKDROP_2] - 1);
            work->timers0[FX_DEMI_T_BACKDROP_2]++;
        }
        return frame >= FX_DEMI_FRAMES;
    }
    return 0;
}

INCLUDE_RODATA("asm/us/fx_063/nonmatchings/fx_063", fx_063_headerPtr);
