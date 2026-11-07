#include "fxmod/fx_062.h"

static s32 fxm_easeLerp(s32 curve, s32 n, s32 t, s32 from, s32 to) {
    BtlfxExports *ex = BTLFX_EXPORTS(fx_062_env);
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

INCLUDE_RODATA("asm/us/fx_062/nonmatchings/fx_062", fx_062_headerPtr);

s32 fx_062_op80(s32 mode, Fx062Work *work, Unk800F78A4 *env, Unk80100C78Entry *entry, s32 *args) {
    BtlfxExports *ex = BTLFX_EXPORTS(env);
    SVECTOR pos;
    VECTOR scale;
    SVECTOR ang;
    SVECTOR cur;
    VECTOR weights;
    s32 frame;
    u8 *strip;
    s32 strand;
    Fx062Strip *tr;
    Fx062Pt *pt;
    s32 i;
    s32 j;
    s32 radius;
    s32 reach;
    s32 t;
    s32 u;
    s32 bob;

    if (mode == 0) {
        *args = sizeof(Fx062Work);
    } else if (mode == 1) {
        fx_062_env = env;
        ex->btlfx_initEffModels(fx_062_effModels, FX_DEMI_MODEL_COUNT);
        for (i = FX_DEMI_TIMERS - 1; i >= 0; i--) {
            work->timers0[i] = 0;
        }
        for (i = 0; i < FX_DEMI_STRANDS; i++) {
            fx_062_strips[i].timer118 = 0;
            ex->btlfx_drawTrail((u8 *)ex->btlfx_getFileData(entry->file, FX_DEMI_GLOW_TRAIL), 0, NULL, -1, 0x1000, 0x1000, BTLFX_ABR_ADD,
                                (u8 *)&fx_062_strips[i], 0);
        }
        ex->btlfx_getUnitHomePos(0, (s16 *)&fx_062_targetHome);
        ex->btlfx_getUnitScale(0, &fx_062_targetScale);
        fx_062_targetHeight = ex->btlfx_getUnitHeight(0);
        ex->btlfx_getUnitBodyPos(0, 0, (s16 *)&fx_062_targetBodyPos);
        env->camera->points[0].vx = work->pos94.vx = fx_062_targetBodyPos.vx;
        env->camera->points[0].vz = work->pos94.vz = fx_062_targetBodyPos.vz;
        work->models40[0].model0 = ex->Hi_RegisterTexEffModel((BtlModelHeader *)ex->btlfx_getFileData(entry->file, FX_DEMI_SPHERE), FX_DEMI_SPHERE_TPAGE,
                                                         FX_DEMI_SPHERE_CLUT, 0, FX_DEMI_SPHERE_V);
        work->models40[0].rot4.vx = 0x800;
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
        work->models40[2].scaleC.vy = 0;
        work->models40[2].scaleC.vz = 0;
        work->modelA4 = ex->Hi_RegisterTexEffModel((BtlModelHeader *)ex->btlfx_getFileData(entry->file, FX_DEMI_CAP), FX_DEMI_CAP_TPAGE, FX_DEMI_CAP_CLUT, 0,
                                                 FX_DEMI_CAP_V);
        work->posB0.vx = fx_062_targetHome.vx;
        work->posB0.vy = 0;
        work->posB0.vz = fx_062_targetHome.vz;
        work->rotA8.vx = 0;
        work->rotA8.vy = 0;
        work->rotA8.vz = 0;
        work->scaleC0.vx = FX_DEMI_CAP_WIDTH;
        work->scaleC0.vy = 0;
        work->scaleC0.vz = FX_DEMI_CAP_WIDTH;
        ex->Hi_SetEffModelOffset(work->modelA4, 0x20);
        return 0;
    } else {
        frame = entry->frame;
        switch (frame) {
        case 0:
            ex->btlfx_drawBgOffscreen(FX_DEMI_BG_VRAM_X, FX_DEMI_BG_VRAM_Y, FX_DEMI_BG_VRAM_W, FX_DEMI_BG_VRAM_H, fx_062_bgOt, fx_062_targetHome.vx, fx_062_targetHome.vz, -0x1000);
            break;
        case FX_DEMI_SPHERE_FRAME:
            work->timers0[FX_DEMI_T_SPHERE] = 1;
            work->timers0[FX_DEMI_T_GROW] = FX_DEMI_GROW_FRAMES;
            work->timers0[FX_DEMI_T_STRANDS] = FX_DEMI_STRANDS_FRAMES;
            work->timers0[FX_DEMI_T_LIFT] = FX_DEMI_GROW_FRAMES;
            break;
        case FX_DEMI_CAP_FRAME:
            work->timers0[FX_DEMI_T_CAP] = 1;
            work->timers0[FX_DEMI_T_CAP_RISE] = FX_DEMI_CAP_RISE_FRAMES;
            break;
        case FX_DEMI_HOVER_FRAME:
            work->timers0[FX_DEMI_T_HOVER] = FX_DEMI_HOVER_FRAMES;
            break;
        case FX_DEMI_SHRINK_FRAME:
            work->timers0[FX_DEMI_T_SHRINK] = FX_DEMI_GROW_FRAMES;
            work->timers0[FX_DEMI_T_DROP] = FX_DEMI_GROW_FRAMES;
            work->timers0[FX_DEMI_T_CAP_SINK] = FX_DEMI_CAP_RISE_FRAMES;
            break;
        case FX_DEMI_END_DRAW_FRAME:
            work->timers0[FX_DEMI_T_SPHERE] = 0;
            work->timers0[FX_DEMI_T_CAP] = 0;
            break;
        }
        if (work->timers0[FX_DEMI_T_GROW] != 0) {
            work->timers0[FX_DEMI_T_GROW]--;
            work->models40[0].scaleC.vx = work->models40[0].scaleC.vy = work->models40[0].scaleC.vz =
                fxm_easeLerp(2, FX_DEMI_GROW_FRAMES, work->timers0[FX_DEMI_T_GROW], 0, FX_DEMI_SPHERE_SCALE);
            work->models40[1].scaleC.vx = work->models40[1].scaleC.vy = work->models40[1].scaleC.vz =
                fxm_easeLerp(2, FX_DEMI_GROW_FRAMES, work->timers0[FX_DEMI_T_GROW], 0, FX_DEMI_BOWL_SCALE);
            work->models40[2].scaleC.vx = work->models40[2].scaleC.vy = work->models40[2].scaleC.vz =
                fxm_easeLerp(2, FX_DEMI_GROW_FRAMES, work->timers0[FX_DEMI_T_GROW], 0, FX_DEMI_BOWL_2_SCALE);
        }
        if (work->timers0[FX_DEMI_T_SHRINK] != 0) {
            work->timers0[FX_DEMI_T_SHRINK]--;
            work->models40[0].scaleC.vx = work->models40[0].scaleC.vy = work->models40[0].scaleC.vz =
                fxm_easeLerp(2, FX_DEMI_GROW_FRAMES, work->timers0[FX_DEMI_T_SHRINK], FX_DEMI_SPHERE_SCALE, 0);
            work->models40[1].scaleC.vx = work->models40[1].scaleC.vy = work->models40[1].scaleC.vz =
                fxm_easeLerp(2, FX_DEMI_GROW_FRAMES, work->timers0[FX_DEMI_T_SHRINK], FX_DEMI_BOWL_SCALE, 0);
            work->models40[2].scaleC.vx = work->models40[2].scaleC.vy = work->models40[2].scaleC.vz =
                fxm_easeLerp(2, FX_DEMI_GROW_FRAMES, work->timers0[FX_DEMI_T_SHRINK], FX_DEMI_BOWL_2_SCALE, 0);
        }
        if (work->timers0[FX_DEMI_T_CAP_RISE] != 0) {
            work->timers0[FX_DEMI_T_CAP_RISE]--;
            work->scaleC0.vy += 0x40;
        }
        if (work->timers0[FX_DEMI_T_CAP_SINK] != 0) {
            work->timers0[FX_DEMI_T_CAP_SINK]--;
            work->scaleC0.vy -= 0x40;
        }
        if (work->timers0[FX_DEMI_T_LIFT] != 0) {
            work->timers0[FX_DEMI_T_LIFT]--;
            if (fx_062_targetHeight > FX_DEMI_SHRINK_HEIGHT) {
                scale.vx = scale.vy = scale.vz =
                    fxm_easeLerp(2, FX_DEMI_GROW_FRAMES, work->timers0[FX_DEMI_T_LIFT], fx_062_targetScale.vy, fx_062_targetScale.vy >> 1);
                ex->btlfx_setUnitScale(0, (s32 *)&scale);
            }
            pos.vx = fx_062_targetHome.vx;
            pos.vz = fx_062_targetHome.vz;
            pos.vy = fx_062_targetHome.vy + fxm_easeLerp(2, FX_DEMI_GROW_FRAMES, work->timers0[FX_DEMI_T_LIFT], 0, -FX_DEMI_LIFT_HEIGHT) +
                ((ex->math_sin(frame << 8) * (FX_DEMI_GROW_FRAMES - work->timers0[FX_DEMI_T_LIFT])) >> 10);
            ex->btlfx_setUnitPos(0, (s16 *)&pos);
            ex->btlfx_getUnitBodyPos(0, 0, (s16 *)&fx_062_targetBodyPos);
        }
        if (work->timers0[FX_DEMI_T_HOVER] != 0) {
            work->timers0[FX_DEMI_T_HOVER]--;
            pos.vx = fx_062_targetHome.vx;
            pos.vz = fx_062_targetHome.vz;
            bob = ((ex->math_sin(frame << 8) * 16) >> 10) - FX_DEMI_LIFT_HEIGHT;
            pos.vy = fx_062_targetHome.vy + bob;
            ex->btlfx_setUnitPos(0, (s16 *)&pos);
        }
        if (work->timers0[FX_DEMI_T_DROP] != 0) {
            if (--work->timers0[FX_DEMI_T_DROP] != 0) {
                if (fx_062_targetHeight > FX_DEMI_SHRINK_HEIGHT) {
                    scale.vx = scale.vy = scale.vz =
                        fxm_easeLerp(2, FX_DEMI_GROW_FRAMES, work->timers0[FX_DEMI_T_DROP], fx_062_targetScale.vy >> 1, fx_062_targetScale.vy);
                    ex->btlfx_setUnitScale(0, (s32 *)&scale);
                }
                pos.vy = fx_062_targetHome.vy + fxm_easeLerp(2, FX_DEMI_GROW_FRAMES, work->timers0[FX_DEMI_T_DROP], -FX_DEMI_LIFT_HEIGHT, 0) +
                    ((ex->math_sin(frame << 8) * work->timers0[FX_DEMI_T_DROP]) >> 10);
                pos.vx = fx_062_targetHome.vx;
                pos.vz = fx_062_targetHome.vz;
            } else {
                ex->btlfx_getUnitHomePos(0, (s16 *)&pos);
                ex->btlfx_setUnitScale(0, (s32 *)&fx_062_targetScale);
            }
            ex->btlfx_setUnitPos(0, (s16 *)&pos);
            ex->btlfx_getUnitBodyPos(0, 0, (s16 *)&fx_062_targetBodyPos);
        }
        env->camera->points[0].vy = work->pos94.vy = fx_062_targetBodyPos.vy;
        if (work->timers0[FX_DEMI_T_SPHERE] != 0) {
            ex->btlfx_aimAngles((s16 *)&fx_062_targetBodyPos, (s16 *)&env->camera->eye, (s16 *)&work->models40[1].rot4);
            work->models40[0].rot4.vx = work->models40[1].rot4.vx + 0x800;
            work->models40[0].rot4.vy = work->models40[1].rot4.vy;
            work->models40[2].rot4.vy = work->models40[1].rot4.vy;
            work->models40[2].rot4.vx = work->models40[1].rot4.vx + 0x400;
            work->models40[1].rot4.vx -= 0x400;
            ex->Hi_DrawEffModel(&work->models40[1].rot4, &work->pos94, (s32)&work->models40[1].scaleC, work->models40[1].model0);
            ex->Hi_DrawEffModel(&work->models40[2].rot4, &work->pos94, (s32)&work->models40[2].scaleC, work->models40[2].model0);
            ex->Hi_DrawEffModel(&work->models40[0].rot4, &work->pos94, (s32)&work->models40[0].scaleC, work->models40[0].model0);
            ex->Hi_ModifyEffModelAbr(work->models40[0].model0, BTLFX_ABR_ADD);
            ex->btlfx_drawLayerTexture((BtlBgLayers *)ex->btlfx_getFileData(entry->file, FX_DEMI_BACKDROP), frame);
        }
        if (work->timers0[FX_DEMI_T_CAP] != 0) {
            ex->Hi_DrawEffModel(&work->rotA8, &work->posB0, (s32)&work->scaleC0, work->modelA4);
            ex->btlfx_drawLayerTexture((BtlBgLayers *)ex->btlfx_getFileData(entry->file, FX_DEMI_BACKDROP_2), frame);
        }
        if (work->timers0[FX_DEMI_T_STRANDS] != 0) {
            work->timers0[FX_DEMI_T_STRANDS]--;
            for (strand = 0; strand < FX_DEMI_STRANDS; strand++) {
                tr = &fx_062_strips[strand];
                if (tr->timer118 != 0) {
                    tr->timer118--;
                    tr->lastSlot0 = tr->count2 = FX_DEMI_STRAND_POINTS - 1;
                    tr->head1 = 0;
                    reach = work->models40[0].scaleC.vx;
                    tr->angle104 += tr->spinB4;
                    tr->angle108 += tr->spinB8;
                    pt = tr->pts4;
                    radius = (ex->math_sin(tr->angle104) * reach) >> 12;
                    tr->x10C = (ex->math_cos(tr->angle108) * radius) >> 12;
                    tr->z110 = (ex->math_sin(tr->angle108) * radius) >> 12;
                    pt->vy = tr->pos5C.vy = work->pos94.vy + ((ex->math_cos(tr->angle104) * reach) >> 12);
                    strip = (u8 *)tr;
                    pt->vx = tr->pos5C.vx = work->pos94.vx + tr->x10C;
                    pt->vz = tr->pos5C.vz = work->pos94.vz + tr->z110;
                    pt->halfWidth6 = tr->width114;
                    tr->pos64.vy = 0;
                    tr->pos64.vx = work->pos94.vx + (tr->x10C >> 4);
                    tr->pos64.vz = work->pos94.vz + (tr->z110 >> 4);
                    ex->btlfx_aimAngles((s16 *)&fx_062_targetBodyPos, (s16 *)&tr->pos5C, (s16 *)&ang);
                    pt++;
                    for (j = 0; j < 9; j++, pt++) {
                        t = ((j + 1) * 0x2E8BA) >> 12;
                        u = 0x200 - t;
                        weights.vx = (u * u) >> 6;
                        weights.vy = (t * u + u * 4) >> 6;
                        weights.vz = (t * t) >> 6;
                        tr->jitter6C[j].vx += tr->jitterVelBC[j];
                        tr->jitter6C[j].vz += tr->jitterVelE0[j];
                        cur.vx = tr->jitter6C[j].vx +
                            ((tr->pos5C.vx * weights.vx + tr->pos64.vx * weights.vz +
                              (work->pos94.vx + ((ex->math_sin(ang.vy) * -tr->pos5C.vy) >> 10) * weights.vy)) >> 12);
                        pt->vy = (tr->pos5C.vy * weights.vx + tr->pos64.vy * weights.vz + (work->pos94.vy >> 1) * weights.vy) >> 12;
                        cur.vz = tr->jitter6C[j].vz +
                            ((tr->pos5C.vz * weights.vx + tr->pos64.vz * weights.vz +
                              (work->pos94.vz + ((ex->math_cos(ang.vy) * -tr->pos5C.vy) >> 10) * weights.vy)) >> 12);
                        pt->vx = cur.vx + tr->jitter6C[j].vx;
                        pt->vz = cur.vz + tr->jitter6C[j].vz;
                        pt->halfWidth6 = tr->width114;
                    }
                    pt->vx = tr->pos64.vx;
                    pt->vy = tr->pos64.vy;
                    pt->vz = tr->pos64.vz;
                    pt->halfWidth6 = tr->width114;
                    ex->btlfx_drawTrail((u8 *)ex->btlfx_getFileData(entry->file, FX_DEMI_GLOW_TRAIL), 0, NULL, frame, 0x1000,
                                        tr->timer118 << 9, BTLFX_ABR_ADD, strip, 0);
                } else if (work->timers0[FX_DEMI_T_STRANDS] >= 9) {
                    tr->timer118 = ex->btlfx_randRange(2, 8);
                    tr->angle104 = ex->btlfx_randRange(0x280, 0x600);
                    tr->angle108 = ex->btlfx_rand() & 0xFFF;
                    tr->spinB4 = ex->btlfx_randRange(-0x60, 0x60);
                    tr->spinB8 = ex->btlfx_randRange(-0x60, 0x60);
                    tr->width114 = ex->btlfx_randRange(8, 0x20);
                    for (j = 0; j < 9; j++) {
                        tr->jitter6C[j].vz = 0;
                        tr->jitter6C[j].vx = 0;
                        tr->jitterVelBC[j] = ex->btlfx_randRange(-0x20, 0x20);
                        tr->jitterVelE0[j] = ex->btlfx_randRange(-0x20, 0x20);
                    }
                }
            }
        }
        return frame >= FX_DEMI_FRAMES;
    }
    return 0;
}
