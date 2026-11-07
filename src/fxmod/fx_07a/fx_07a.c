#include "fxmod/fx_07a.h"

static s32 fxm_easeLerpSvec(s32 curve, s32 n, s32 t, SVECTOR *from, SVECTOR *to, SVECTOR *out) {
    BtlfxExports *ex = BTLFX_EXPORTS(fx_07a_env);
    s32 invSq = 0x20000000 / (n * n);
    s32 ratio = 0;

    switch (curve) {
    case 0:
        ratio = ((0x10000000 / n) * t) >> 16;
        break;
    case 1:
        ratio = 0x1000 - ((invSq * (n - t) * (n - t)) >> 17);
        break;
    case 2:
        ratio = (invSq * t * t) >> 17;
        break;
    }
    return ex->btlfx_lerpVec((s16 *)from, (s16 *)to, 0x1000 - ratio, (s16 *)out);
}

static s32 fxm_easeLerp(s32 curve, s32 n, s32 t, s32 from, s32 to) {
    BtlfxExports *ex = BTLFX_EXPORTS(fx_07a_env);
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

INCLUDE_RODATA("asm/us/fx_07a/nonmatchings/fx_07a", fx_07a_headerPtr);

s32 fx_07a_op80(s32 mode, Fx07aWork *work, Unk800F78A4 *env, Unk80100C78Entry *entry, s32 *args) {
    BtlfxExports *ex = BTLFX_EXPORTS(env);
    SVECTOR pos;
    SVECTOR out;
    VECTOR cur;
    VECTOR step;
    Fx07aStrip *tr;
    Fx07aStrip *strip;
    Fx07aPt *pt;
    s32 frame;
    s32 i;
    s32 j;
    s32 n = FX_DEMI_SHOCK_STRAND_POINTS;
    s32 spread;
    s32 rise;
    s32 pitch;
    s32 yaw;
    Pool6CEntry *emitter;

    if (mode == 0) {
        *args = sizeof(Fx07aWork);
    } else if (mode == 1) {
        fx_07a_env = env;
        ex->btlfx_initEffModels(fx_07a_effModels, FX_DEMI_SHOCK_MODEL_COUNT);
        for (i = FX_DEMI_SHOCK_TIMERS - 1; i >= 0; i--) {
            work->timers0[i] = 0;
        }
        for (i = 0; i < FX_DEMI_SHOCK_STRANDS; i++) {
            fx_07a_strips[i].timer10C = -i;
            ex->btlfx_drawTrail((u8 *)ex->btlfx_getFileData(entry->file, FX_DEMI_SHOCK_SPARK_TRAIL), 0, NULL, -1, 0x1000, 0x1000, BTLFX_ABR_ADD,
                                (u8 *)&fx_07a_strips[i], 0);
        }
        work->pos94.vx = fx_07a_centerPos.vx = env->targets->centerX;
        work->pos94.vy = fx_07a_centerPos.vz = env->targets->centerZ;
        work->pos94.vz = fx_07a_centerPos.vy = -FX_DEMI_SHOCK_SPHERE_HEIGHT;
        work->models40[0].model0 = ex->Hi_RegisterTexEffModel((BtlModelHeader *)ex->btlfx_getFileData(entry->file, FX_DEMI_SHOCK_ENERGY_SHELL), FX_DEMI_SHOCK_SHELL_TPAGE,
                                                         FX_DEMI_SHOCK_SHELL_CLUT, 0, 0);
        work->models40[0].rot4.vx = 0x800;
        work->models40[0].rot4.vy = 0;
        work->models40[0].rot4.vz = 0;
        work->models40[0].scaleC.vx = FX_DEMI_SHOCK_SHELL_SCALE_0;
        work->models40[0].scaleC.vy = FX_DEMI_SHOCK_SHELL_SCALE_0;
        work->models40[0].scaleC.vz = FX_DEMI_SHOCK_SHELL_SCALE_0;
        work->models40[1].model0 = ex->Hi_RegisterGouEffModel((BtlModelHeader *)ex->btlfx_getFileData(entry->file, FX_DEMI_SHOCK_BOWL));
        work->models40[1].rot4.vx = 0;
        work->models40[1].rot4.vy = 0;
        work->models40[1].rot4.vz = 0;
        work->models40[1].scaleC.vx = FX_DEMI_SHOCK_BOWL_SCALE_0;
        work->models40[1].scaleC.vy = FX_DEMI_SHOCK_BOWL_SCALE_0;
        work->models40[1].scaleC.vz = FX_DEMI_SHOCK_BOWL_SCALE_0;
        work->models40[2].model0 = ex->Hi_RegisterGouEffModel((BtlModelHeader *)ex->btlfx_getFileData(entry->file, FX_DEMI_SHOCK_BOWL_2));
        work->models40[2].rot4.vx = 0;
        work->models40[2].rot4.vy = 0;
        work->models40[2].rot4.vz = 0;
        work->models40[2].scaleC.vx = FX_DEMI_SHOCK_BOWL_2_SCALE_0;
        work->models40[2].scaleC.vy = FX_DEMI_SHOCK_BOWL_2_SCALE_0;
        work->models40[2].scaleC.vz = FX_DEMI_SHOCK_BOWL_2_SCALE_0;
        for (i = 0; i < env->targets->targetCount; i++) {
            ex->btlfx_getUnitHomePos(i, (s16 *)&work->homePosA4[i]);
            work->delayE4[i] = ex->btlfx_rand() & 7;
        }
        fx_07a_discRadius = 0;
        fx_07a_uvRadiusOut = 0;
        fx_07a_uvRadiusIn = 0;
        fx_07a_discRadiusIn = 0;
        fx_07a_ringTwist[4] = 0;
        fx_07a_ringTwist[3] = 0;
        fx_07a_ringTwist[2] = 0;
        fx_07a_ringTwist[1] = 0;
        fx_07a_ringTwist[0] = 0;
    } else {
        frame = entry->frame;
        ex->btlfx_getUnitBonePos(BTLFX_UNIT_CASTER, 0x17, (u16 *)&fx_07a_bonePos);
        switch (frame) {
        case 0:
            ex->btlfx_drawBgOffscreen(FX_DEMI_SHOCK_BG_VRAM_X, FX_DEMI_SHOCK_BG_VRAM_Y, FX_DEMI_SHOCK_BG_VRAM_W, FX_DEMI_SHOCK_BG_VRAM_H, fx_07a_bgOt, fx_07a_centerPos.vx, fx_07a_centerPos.vz, 0x1208);
            break;
        case FX_DEMI_SHOCK_START_FRAME:
            work->timers0[FX_DEMI_SHOCK_T_BACKDROP] = 1;
            work->timers0[FX_DEMI_SHOCK_T_SPHERE] = 1;
            work->timers0[FX_DEMI_SHOCK_T_STRANDS] = FX_DEMI_SHOCK_STRANDS_FRAMES;
            work->timers0[FX_DEMI_SHOCK_T_FLY] = FX_DEMI_SHOCK_FLY_FRAMES;
            work->timers0[FX_DEMI_SHOCK_T_GROW] = FX_DEMI_SHOCK_GROW_FRAMES;
            break;
        case FX_DEMI_SHOCK_DISC_FRAME:
            work->timers0[FX_DEMI_SHOCK_T_DISC] = 1;
            work->timers0[FX_DEMI_SHOCK_T_DISC_OPEN] = FX_DEMI_SHOCK_DISC_OPEN_FRAMES;
            work->timers0[FX_DEMI_SHOCK_T_TWIST] = FX_DEMI_SHOCK_TWIST_FRAMES;
            work->timers0[FX_DEMI_SHOCK_T_LIFT] = FX_DEMI_SHOCK_LIFT_FRAMES;
            break;
        case FX_DEMI_SHOCK_FLASH_RING_FRAME:
            ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, FX_DEMI_SHOCK_FLASH_RING), (s32)&fx_07a_centerPos);
            break;
        case FX_DEMI_SHOCK_SHRINK_FRAME:
            work->timers0[FX_DEMI_SHOCK_T_DISC_CLOSE] = FX_DEMI_SHOCK_DISC_CLOSE_FRAMES;
            work->timers0[FX_DEMI_SHOCK_T_SHRINK] = FX_DEMI_SHOCK_GROW_FRAMES;
            break;
        case FX_DEMI_SHOCK_WISP_FRAME:
            work->timers0[FX_DEMI_SHOCK_T_DROP] = FX_DEMI_SHOCK_DROP_FRAMES;
            emitter = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, FX_DEMI_SHOCK_WISP_BURST), (s32)&fx_07a_centerPos);
            if (emitter != NULL) {
                emitter->otZ = ex->btlfx_addRadiusSixth(0, 0x80);
            }
            break;
        case FX_DEMI_SHOCK_FLASH_FRAME:
            work->timers0[FX_DEMI_SHOCK_T_DISC] = 0;
            ex->btlfx_drawScreenTint((u32 *)env->ot, BTLFX_ABR_OPAQUE, FX_DEMI_SHOCK_FLASH_RGB);
            work->timers0[FX_DEMI_SHOCK_T_SPHERE] = 0;
            break;
        }
        pos.vx = fx_07a_centerPos.vx;
        pos.vy = 0;
        pos.vz = fx_07a_centerPos.vz;
        if (work->timers0[FX_DEMI_SHOCK_T_DISC_OPEN] != 0) {
            work->timers0[FX_DEMI_SHOCK_T_DISC_OPEN]--;
            fx_07a_outerBandRadius = fxm_easeLerp(0, FX_DEMI_SHOCK_DISC_OPEN_FRAMES, work->timers0[FX_DEMI_SHOCK_T_DISC_OPEN], 0, FX_DEMI_SHOCK_OUTER_BAND_RADIUS);
            fx_07a_discRadius = fxm_easeLerp(0, FX_DEMI_SHOCK_DISC_OPEN_FRAMES, work->timers0[FX_DEMI_SHOCK_T_DISC_OPEN], 0, FX_DEMI_SHOCK_DISC_RADIUS);
            fx_07a_innerBandRadius = fxm_easeLerp(0, FX_DEMI_SHOCK_DISC_OPEN_FRAMES, work->timers0[FX_DEMI_SHOCK_T_DISC_OPEN], 0, FX_DEMI_SHOCK_INNER_BAND_RADIUS);
            fx_07a_uvRadiusOut = fxm_easeLerp(0, FX_DEMI_SHOCK_DISC_OPEN_FRAMES, work->timers0[FX_DEMI_SHOCK_T_DISC_OPEN], 0, FX_DEMI_SHOCK_UV_RADIUS);
        }
        if (work->timers0[FX_DEMI_SHOCK_T_TWIST] != 0) {
            work->timers0[FX_DEMI_SHOCK_T_TWIST]--;
            fx_07a_ringTwist[1] = fxm_easeLerp(1, FX_DEMI_SHOCK_TWIST_FRAMES, work->timers0[FX_DEMI_SHOCK_T_TWIST], 0, 0x1000);
            fx_07a_ringTwist[2] = fxm_easeLerp(1, FX_DEMI_SHOCK_TWIST_FRAMES, work->timers0[FX_DEMI_SHOCK_T_TWIST], 0, 0xEAB);
            fx_07a_ringTwist[3] = fxm_easeLerp(1, FX_DEMI_SHOCK_TWIST_FRAMES, work->timers0[FX_DEMI_SHOCK_T_TWIST], 0, 0xD56);
            fx_07a_ringTwist[4] = fxm_easeLerp(1, FX_DEMI_SHOCK_TWIST_FRAMES, work->timers0[FX_DEMI_SHOCK_T_TWIST], 0, 0xC00);
            fx_07a_uvRadiusIn = fxm_easeLerp(1, FX_DEMI_SHOCK_TWIST_FRAMES, work->timers0[FX_DEMI_SHOCK_T_TWIST], 0, FX_DEMI_SHOCK_UV_RADIUS);
        }
        if (work->timers0[FX_DEMI_SHOCK_T_DISC_CLOSE] != 0) {
            work->timers0[FX_DEMI_SHOCK_T_DISC_CLOSE]--;
            /* the outer band and the disc read T_TWIST's count (already 0 by now), not T_DISC_CLOSE's */
            fx_07a_outerBandRadius = fxm_easeLerp(0, FX_DEMI_SHOCK_DISC_CLOSE_FRAMES, work->timers0[FX_DEMI_SHOCK_T_TWIST], FX_DEMI_SHOCK_OUTER_BAND_RADIUS, 0);
            fx_07a_innerBandRadius = fxm_easeLerp(0, FX_DEMI_SHOCK_DISC_CLOSE_FRAMES, work->timers0[FX_DEMI_SHOCK_T_DISC_CLOSE], FX_DEMI_SHOCK_INNER_BAND_RADIUS, 0);
            fx_07a_discRadius = fxm_easeLerp(0, FX_DEMI_SHOCK_DISC_CLOSE_FRAMES, work->timers0[FX_DEMI_SHOCK_T_TWIST], FX_DEMI_SHOCK_DISC_RADIUS, 0);
        }
        if (work->timers0[FX_DEMI_SHOCK_T_DISC] != 0) {
            fx_07a_drawTwistDisc(&pos, 0xFFF, FX_DEMI_SHOCK_DISC_TPAGE, -1, BTLFX_ABR_OPAQUE);
        }
        if (work->timers0[FX_DEMI_SHOCK_T_FLY] != 0) {
            work->timers0[FX_DEMI_SHOCK_T_FLY]--;
            fxm_easeLerpSvec(0, FX_DEMI_SHOCK_FLY_FRAMES, work->timers0[FX_DEMI_SHOCK_T_FLY], &fx_07a_bonePos, &fx_07a_centerPos, &out);
            work->pos94.vx = out.vx;
            work->pos94.vy = out.vy;
            work->pos94.vz = out.vz;
        }
        if (work->timers0[FX_DEMI_SHOCK_T_GROW] != 0) {
            work->timers0[FX_DEMI_SHOCK_T_GROW]--;
            work->models40[0].scaleC.vx = work->models40[0].scaleC.vy = work->models40[0].scaleC.vz =
                fxm_easeLerp(0, FX_DEMI_SHOCK_GROW_FRAMES, work->timers0[FX_DEMI_SHOCK_T_GROW], FX_DEMI_SHOCK_SHELL_SCALE_0, FX_DEMI_SHOCK_SHELL_SCALE);
            work->models40[1].scaleC.vx = work->models40[1].scaleC.vy = work->models40[1].scaleC.vz =
                fxm_easeLerp(0, FX_DEMI_SHOCK_GROW_FRAMES, work->timers0[FX_DEMI_SHOCK_T_GROW], FX_DEMI_SHOCK_BOWL_SCALE_0, FX_DEMI_SHOCK_BOWL_SCALE);
            work->models40[2].scaleC.vx = work->models40[2].scaleC.vy = work->models40[2].scaleC.vz =
                fxm_easeLerp(0, FX_DEMI_SHOCK_GROW_FRAMES, work->timers0[FX_DEMI_SHOCK_T_GROW], FX_DEMI_SHOCK_BOWL_2_SCALE_0, FX_DEMI_SHOCK_BOWL_2_SCALE);
        }
        if (work->timers0[FX_DEMI_SHOCK_T_SHRINK] != 0) {
            work->timers0[FX_DEMI_SHOCK_T_SHRINK]--;
            work->models40[0].scaleC.vx = work->models40[0].scaleC.vy = work->models40[0].scaleC.vz =
                fxm_easeLerp(0, FX_DEMI_SHOCK_GROW_FRAMES, work->timers0[FX_DEMI_SHOCK_T_SHRINK], FX_DEMI_SHOCK_SHELL_SCALE, 0);
            work->models40[1].scaleC.vx = work->models40[1].scaleC.vy = work->models40[1].scaleC.vz =
                fxm_easeLerp(0, FX_DEMI_SHOCK_GROW_FRAMES, work->timers0[FX_DEMI_SHOCK_T_SHRINK], FX_DEMI_SHOCK_BOWL_SCALE, 0);
            work->models40[2].scaleC.vx = work->models40[2].scaleC.vy = work->models40[2].scaleC.vz =
                fxm_easeLerp(0, FX_DEMI_SHOCK_GROW_FRAMES, work->timers0[FX_DEMI_SHOCK_T_SHRINK], FX_DEMI_SHOCK_BOWL_2_SCALE, 0);
        }
        if (work->timers0[FX_DEMI_SHOCK_T_LIFT] != 0) {
            work->timers0[FX_DEMI_SHOCK_T_LIFT]--;
            for (i = 0; i < env->targets->targetCount; i++) {
                if (FX_DEMI_SHOCK_LIFT_FRAMES - work->timers0[FX_DEMI_SHOCK_T_LIFT] >= work->delayE4[i]) {
                    pos.vx = work->homePosA4[i].vx;
                    pos.vz = work->homePosA4[i].vz;
                    pos.vy = fxm_easeLerp(2, FX_DEMI_SHOCK_LIFT_FRAMES - work->delayE4[i], work->timers0[FX_DEMI_SHOCK_T_LIFT], work->homePosA4[i].vy,
                                           work->homePosA4[i].vy - FX_DEMI_SHOCK_LIFT_HEIGHT);
                    ex->btlfx_setUnitPos(i, (s16 *)&pos);
                }
            }
        }
        if (work->timers0[FX_DEMI_SHOCK_T_DROP] != 0) {
            work->timers0[FX_DEMI_SHOCK_T_DROP]--;
            for (i = 0; i < env->targets->targetCount; i++) {
                if (work->timers0[FX_DEMI_SHOCK_T_DROP] != 0) {
                    pos.vx = work->homePosA4[i].vx;
                    pos.vz = work->homePosA4[i].vz;
                    pos.vy = fxm_easeLerp(2, FX_DEMI_SHOCK_DROP_FRAMES, work->timers0[FX_DEMI_SHOCK_T_DROP], work->homePosA4[i].vy - FX_DEMI_SHOCK_LIFT_HEIGHT, work->homePosA4[i].vy);
                } else {
                    ex->btlfx_getUnitHomePos(i, (s16 *)&pos);
                }
                ex->btlfx_setUnitPos(i, (s16 *)&pos);
            }
        }
        if (work->timers0[FX_DEMI_SHOCK_T_SPHERE] != 0) {
            ex->btlfx_aimAngles((s16 *)&fx_07a_centerPos, (s16 *)&env->camera->eye, (s16 *)&work->models40[1].rot4);
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
        if (work->timers0[FX_DEMI_SHOCK_T_STRANDS] != 0) {
            work->timers0[FX_DEMI_SHOCK_T_STRANDS]--;
            for (i = 0; i < FX_DEMI_SHOCK_STRANDS; i++) {
                tr = &fx_07a_strips[i];
                if (tr->timer10C > 0) {
                    tr->timer10C--;
                    tr->lastSlot0 = tr->count2 = n - 1;
                    tr->head1 = 0;
                    cur.vx = fx_07a_bonePos.vx << 12;
                    cur.vy = fx_07a_bonePos.vy << 12;
                    cur.vz = fx_07a_bonePos.vz << 12;
                    step.vx = ((work->pos94.vx + tr->endX5C - fx_07a_bonePos.vx) << 12) / n;
                    step.vy = ((work->pos94.vy + tr->endY60 - fx_07a_bonePos.vy) << 12) / n;
                    step.vz = ((work->pos94.vz + tr->endZ64 - fx_07a_bonePos.vz) << 12) / n;
                    strip = tr;
                    for (j = 0, pt = tr->pts4; j < n; pt++, j++) {
                        pt->vx = cur.vx >> 12;
                        pt->vz = cur.vz >> 12;
                        pt->vy = tr->offsetY6C[j] + (cur.vy >> 12);
                        pt->halfWidth6 = (u16)tr->timer10C * 4;
                        cur.vx += step.vx;
                        cur.vy += step.vy;
                        cur.vz += step.vz;
                        tr->offsetY6C[j] += tr->speedYBC[j];
                    }
                    if (tr->timer10C < FX_DEMI_SHOCK_STRAND_FADE_FRAMES) {
                        ex->btlfx_drawTrail((u8 *)ex->btlfx_getFileData(entry->file, FX_DEMI_SHOCK_SPARK_TRAIL), 0, NULL, frame, 0x1000,
                                            tr->timer10C * (0x1000 / FX_DEMI_SHOCK_STRAND_FADE_FRAMES), BTLFX_ABR_ADD, (u8 *)strip, 0);
                    } else {
                        ex->btlfx_drawTrail((u8 *)ex->btlfx_getFileData(entry->file, FX_DEMI_SHOCK_SPARK_TRAIL), 0, NULL, frame, 0x1000,
                                            0x1000, BTLFX_ABR_ADD, (u8 *)strip, 0);
                    }
                } else if (tr->timer10C < 0) {
                    tr->timer10C++;
                } else if (work->timers0[FX_DEMI_SHOCK_T_STRANDS] >= FX_DEMI_SHOCK_STRAND_RESPAWN_FRAMES) {
                    tr->timer10C = ex->btlfx_randRange(4, 8);
                    j = 1;
                    pitch = ex->btlfx_rand() & 0xFFF;
                    yaw = ex->btlfx_rand() & 0xFFF;
                    rise = (ex->math_cos(pitch) * (work->models40[0].scaleC.vx + FX_DEMI_SHOCK_STRAND_REACH)) >> 12;
                    tr->endY60 = rise;
                    tr->endX5C = (ex->math_sin(yaw) * abs(rise)) >> 12;
                    tr->endZ64 = (ex->math_cos(yaw) * abs(rise)) >> 12;
                    tr->speedYBC[0] = 0;
                    tr->offsetY6C[0] = 0;
                    for (; j < n - 1; j++) {
                        spread = j * 8 + 0x10;
                        tr->offsetY6C[j] = ex->btlfx_randRange(-spread * 2, spread * 2);
                        tr->speedYBC[j] = ex->btlfx_randRange(-spread, spread);
                    }
                    tr->speedYBC[j] = 0;
                    tr->offsetY6C[j] = 0;
                }
            }
        }
        if (work->timers0[FX_DEMI_SHOCK_T_BACKDROP] != 0) {
            ex->btlfx_drawLayerTexture((BtlBgLayers *)ex->btlfx_getFileData(entry->file, FX_DEMI_SHOCK_BACKDROP), work->timers0[FX_DEMI_SHOCK_T_BACKDROP]);
            work->timers0[FX_DEMI_SHOCK_T_BACKDROP]++;
        }
        return frame >= FX_DEMI_SHOCK_FRAMES;
    }
    return 0;
}

void fx_07a_drawTwistDisc(SVECTOR *pos, s32 ot, s32 tpage, s32 clut, s32 abr) {
    Unk800F78A4 *env = fx_07a_env;
    BtlfxExports *ex = BTLFX_EXPORTS(env);
    u8 uv[4][8];
    SVECTOR quad[4];
    SVECTOR rot;
    MATRIX m;
    s32 radiusStep;
    s32 uvStep;
    POLY_FT4 *ft4;
    POLY_G4 *poly;
    s32 ang;
    s32 j;
    s32 radius;
    s32 uvRadius;
    s32 twistAng;

    quad[3].vy = 0;
    quad[2].vy = 0;
    quad[1].vy = 0;
    quad[0].vy = 0;
    rot.vz = 0;
    rot.vy = 0;
    rot.vx = 0;
    gte_SetRotMatrix(env->viewMatrix);
    gte_SetTransMatrix(env->viewMatrix);
    gte_ldv0(pos);
    ft4 = (POLY_FT4 *)env->prim;
    tpage |= TPAGE_ABR(abr);
    poly = (POLY_G4 *)(ft4 + FX_DEMI_SHOCK_DISC_SEGMENTS * FX_DEMI_SHOCK_DISC_RINGS);
    env->prim = (u8 *)(poly + FX_DEMI_SHOCK_DISC_SEGMENTS * 2);
    gte_rtv0tr_b();
    ex->math_rotMatrixYXZ(&rot, &m);
    gte_stlvnl(m.t);
    ex->MulRotMatrix(&m);
    gte_SetRotMatrix(&m);
    gte_SetTransMatrix(&m);
    ang = 0;
    radiusStep = fx_07a_discRadius >> 2;
    uvStep = (fx_07a_uvRadiusOut - fx_07a_uvRadiusIn) >> 2;
    for (; ang < 0x1000; ang += 0x1000 / FX_DEMI_SHOCK_DISC_SEGMENTS) {
        radius = 0;
        uvRadius = 0;
        for (j = 0; j < FX_DEMI_SHOCK_DISC_RINGS; j++) {
            twistAng = ang + fx_07a_ringTwist[j];
            uv[0][0] = ((ex->math_cos(ang) * (fx_07a_uvRadiusIn + uvRadius)) >> 16) + FX_DEMI_SHOCK_DISC_U;
            uv[0][1] = ((ex->math_sin(ang) * (fx_07a_uvRadiusIn + uvRadius)) >> 16) + FX_DEMI_SHOCK_DISC_V;
            uv[1][0] = ((ex->math_cos(ang + 0x100) * (fx_07a_uvRadiusIn + uvRadius)) >> 16) + FX_DEMI_SHOCK_DISC_U;
            uv[1][1] = ((ex->math_sin(ang + 0x100) * (fx_07a_uvRadiusIn + uvRadius)) >> 16) + FX_DEMI_SHOCK_DISC_V;
            quad[0].vx = (ex->math_cos(twistAng) * radius) >> 16;
            quad[0].vz = -((ex->math_sin(twistAng) * radius) >> 16) - FX_DEMI_SHOCK_DISC_Z;
            twistAng += 0x100;
            quad[1].vx = (ex->math_cos(twistAng) * radius) >> 16;
            quad[1].vz = -((ex->math_sin(twistAng) * radius) >> 16) - FX_DEMI_SHOCK_DISC_Z;
            twistAng = ang + fx_07a_ringTwist[j + 1];
            uvRadius += uvStep;
            uv[2][0] = ((ex->math_cos(ang) * (fx_07a_uvRadiusIn + uvRadius)) >> 16) + FX_DEMI_SHOCK_DISC_U;
            uv[2][1] = ((ex->math_sin(ang) * (fx_07a_uvRadiusIn + uvRadius)) >> 16) + FX_DEMI_SHOCK_DISC_V;
            uv[3][0] = ((ex->math_cos(ang + 0x100) * (fx_07a_uvRadiusIn + uvRadius)) >> 16) + FX_DEMI_SHOCK_DISC_U;
            radius += radiusStep;
            uv[3][1] = ((ex->math_sin(ang + 0x100) * (fx_07a_uvRadiusIn + uvRadius)) >> 16) + FX_DEMI_SHOCK_DISC_V;
            quad[2].vx = (ex->math_cos(twistAng) * radius) >> 16;
            quad[2].vz = -((ex->math_sin(twistAng) * radius) >> 16) - FX_DEMI_SHOCK_DISC_Z;
            twistAng += 0x100;
            quad[3].vx = (ex->math_cos(twistAng) * radius) >> 16;
            quad[3].vz = -((ex->math_sin(twistAng) * radius) >> 16) - FX_DEMI_SHOCK_DISC_Z;
            gte_ldv3c(quad);
            gte_rtpt();
            ft4->b0 = 0x40;
            ft4->g0 = 0x40;
            ft4->r0 = 0x40;
            ft4->code = abr == BTLFX_ABR_OPAQUE ? GPU_POLY_FT4 : GPU_POLY_FT4 | GPU_ABE;
            *(u16 *)&ft4->u0 = *(u16 *)uv[0];
            *(u16 *)&ft4->u1 = *(u16 *)uv[1];
            *(u16 *)&ft4->u2 = *(u16 *)uv[2];
            *(u16 *)&ft4->u3 = *(u16 *)uv[3];
            gte_stsxy3(&ft4->x0, &ft4->x1, &ft4->x2);
            gte_ldv0(&quad[3]);
            gte_rtps();
            ft4->clut = clut;
            ft4->tpage = tpage;
            gte_stsxy(&ft4->x3);
            ex->btlfx_addPrim(PRIM_TAG(POLY_FT4), ot * 4, ft4);
            ft4++;
        }
        for (j = 0; j < 2; j++) {
            if (j != 0) {
                quad[2].vx = (ex->math_cos(ang) * fx_07a_outerBandRadius) >> 16;
                quad[2].vz = -((ex->math_sin(ang) * fx_07a_outerBandRadius) >> 16) - FX_DEMI_SHOCK_DISC_Z;
                quad[3].vx = (ex->math_cos(ang + 0x100) * fx_07a_outerBandRadius) >> 16;
                quad[3].vz = -((ex->math_sin(ang + 0x100) * fx_07a_outerBandRadius) >> 16) - FX_DEMI_SHOCK_DISC_Z;
                quad[0].vx = (ex->math_cos(ang) * fx_07a_discRadius) >> 16;
                quad[0].vz = -((ex->math_sin(ang) * fx_07a_discRadius) >> 16) - FX_DEMI_SHOCK_DISC_Z;
                quad[1].vx = (ex->math_cos(ang + 0x100) * fx_07a_discRadius) >> 16;
                quad[1].vz = -((ex->math_sin(ang + 0x100) * fx_07a_discRadius) >> 16) - FX_DEMI_SHOCK_DISC_Z;
                poly->b1 = 0x80;
                poly->g1 = 0x80;
                poly->r1 = 0x80;
                poly->b0 = 0x80;
                poly->g0 = 0x80;
                poly->r0 = 0x80;
                poly->b3 = 0;
                poly->g3 = 0;
                poly->r3 = 0;
                poly->b2 = 0;
                poly->g2 = 0;
                poly->r2 = 0;
            } else {
                quad[2].vx = (ex->math_cos(ang) * fx_07a_discRadius) >> 16;
                quad[2].vz = -((ex->math_sin(ang) * fx_07a_discRadius) >> 16) - FX_DEMI_SHOCK_DISC_Z;
                quad[3].vx = (ex->math_cos(ang + 0x100) * fx_07a_discRadius) >> 16;
                quad[3].vz = -((ex->math_sin(ang + 0x100) * fx_07a_discRadius) >> 16) - FX_DEMI_SHOCK_DISC_Z;
                quad[0].vx = (ex->math_cos(ang) * fx_07a_innerBandRadius) >> 12;
                quad[0].vz = -((ex->math_sin(ang) * fx_07a_innerBandRadius) >> 12) - FX_DEMI_SHOCK_DISC_Z;
                quad[1].vx = (ex->math_cos(ang + 0x100) * fx_07a_innerBandRadius) >> 12;
                quad[1].vz = -((ex->math_sin(ang + 0x100) * fx_07a_innerBandRadius) >> 12) - FX_DEMI_SHOCK_DISC_Z;
                poly->b1 = 0;
                poly->g1 = 0;
                poly->r1 = 0;
                poly->b0 = 0;
                poly->g0 = 0;
                poly->r0 = 0;
                poly->b3 = 0xFF;
                poly->g3 = 0xFF;
                poly->r3 = 0xFF;
                poly->b2 = 0xFF;
                poly->g2 = 0xFF;
                poly->r2 = 0xFF;
            }
            gte_ldv3c(quad);
            gte_rtpt();
            poly->code = GPU_POLY_G4 | GPU_ABE;
            gte_stsxy3(&poly->x0, &poly->x1, &poly->x2);
            gte_ldv0(&quad[3]);
            gte_rtps();
            gte_stsxy(&poly->x3);
            ex->btlfx_addPrim(PRIM_TAG(POLY_G4), ot * 4 - 4, poly);
            poly++;
        }
    }
    ex->btlfx_addPrim(PRIM_TAG(DR_TPAGE), ot * 4 - 4, ex->btlfx_allocDrMode(0, BTLFX_ABR_SUB));
}
