#include "fxmod/fx_079.h"

static s32 fxm_easeLerp(s32 curve, s32 n, s32 t, s32 from, s32 to) {
    BtlfxExports *ex = BTLFX_EXPORTS(fx_079_env);
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

INCLUDE_RODATA("asm/us/fx_079/nonmatchings/fx_079", fx_079_headerPtr);

s32 fx_079_op80(s32 mode, Fx079Work *work, Unk800F78A4 *env, Unk80100C78Entry *entry, s32 *args) {
    BtlfxExports *ex = BTLFX_EXPORTS(env);
    VECTOR scale;
    SVECTOR pos;
    VECTOR cur;
    VECTOR step;
    Fx079Strip *tr;
    Fx079Pt *pt;
    Pool6CEntry *emitter;
    s32 i;
    s32 frame;
    s32 j;
    s32 n = FX_DEMI_SHOCK_STRAND_POINTS;
    s32 pitch;
    s32 yaw;
    s32 rise;
    u8 *strip;
    s32 speedMin;
    s32 speedMax;

    if (mode == 0) {
        *args = sizeof(Fx079Work);
    } else if (mode == 1) {
        fx_079_env = env;
        fx_079_emitter = NULL;
        ex->btlfx_initEffModels(fx_079_effModels, FX_DEMI_SHOCK_MODEL_COUNT);
        for (i = FX_DEMI_SHOCK_TIMERS - 1; i >= 0; i--) {
            work->timers0[i] = 0;
        }
        for (i = 0; i < FX_DEMI_SHOCK_STRANDS; i++) {
            fx_079_strips[i].timer11C = -i;
            ex->btlfx_drawTrail((u8 *)ex->btlfx_getFileData(entry->file, FX_DEMI_SHOCK_SPARK_TRAIL), 0, NULL, -1, 0x1000, 0x1000, BTLFX_ABR_ADD,
                                (u8 *)&fx_079_strips[i], 0);
        }
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
        ex->btlfx_getUnitBodyPos(0, 0, (s16 *)&fx_079_bodyPos);
        ex->btlfx_getUnitBodyPos(0, 0, (s16 *)&fx_079_emitStartPos);
        ex->btlfx_getUnitHomePos(0, (s16 *)&fx_079_homePos);
        ex->btlfx_getUnitScale(0, &fx_079_unitScale);
        fx_079_unitHeight = ex->btlfx_getUnitHeight(0);
        work->pos94.vx = fx_079_curBodyPos.vx = fx_079_bodyPos.vx;
        work->pos94.vy = fx_079_bodyPos.vy;
        work->pos94.vz = fx_079_curBodyPos.vz = fx_079_bodyPos.vz;
    } else {
        frame = entry->frame;
        ex->btlfx_getUnitBonePos(BTLFX_UNIT_CASTER, 0x17, (u16 *)&fx_079_bonePos);
        switch (frame) {
        case 0:
            work->timers0[FX_DEMI_SHOCK_T_BACKDROP] = 1;
            work->timers0[FX_DEMI_SHOCK_T_SPHERE] = 1;
            work->timers0[FX_DEMI_SHOCK_T_STRANDS] = FX_DEMI_SHOCK_STRANDS_FRAMES;
            work->timers0[FX_DEMI_SHOCK_T_GROW] = FX_DEMI_SHOCK_GROW_FRAMES;
            break;
        case FX_DEMI_SHOCK_LIFT_FRAME:
            work->timers0[FX_DEMI_SHOCK_T_LIFT] = FX_DEMI_SHOCK_GROW_FRAMES;
            break;
        case FX_DEMI_SHOCK_FLASH_RING_FRAME:
            fx_079_emitter = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, FX_DEMI_SHOCK_FLASH_RING), (s32)&fx_079_emitStartPos);
            break;
        case FX_DEMI_SHOCK_HOLD_FRAME:
            work->timers0[FX_DEMI_SHOCK_T_HOLD] = FX_DEMI_SHOCK_HOLD_FRAMES;
            break;
        case FX_DEMI_SHOCK_SHRINK_FRAME:
            work->timers0[FX_DEMI_SHOCK_T_SHRINK] = FX_DEMI_SHOCK_GROW_FRAMES;
            work->timers0[FX_DEMI_SHOCK_T_DROP] = FX_DEMI_SHOCK_GROW_FRAMES;
            break;
        case FX_DEMI_SHOCK_WISP_FRAME:
            emitter = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, FX_DEMI_SHOCK_WISP_BURST), (s32)&fx_079_bodyPos);
            if (emitter != NULL) {
                emitter->otZ = ex->btlfx_addRadiusSixth(0, 0x80);
            }
            break;
        case FX_DEMI_SHOCK_FLASH_FRAME:
            ex->btlfx_drawScreenTint((u32 *)env->ot, BTLFX_ABR_OPAQUE, FX_DEMI_SHOCK_FLASH_RGB);
            work->timers0[FX_DEMI_SHOCK_T_SPHERE] = 0;
            break;
        }
        if (work->timers0[FX_DEMI_SHOCK_T_RAISE] != 0) {
            work->timers0[FX_DEMI_SHOCK_T_RAISE]--;
            work->pos94.vx = fx_079_bodyPos.vx;
            work->pos94.vy = fx_079_bodyPos.vy + fxm_easeLerp(0, FX_DEMI_SHOCK_RAISE_FRAMES, work->timers0[FX_DEMI_SHOCK_T_RAISE], 0, -FX_DEMI_SHOCK_LIFT_HEIGHT);
            work->pos94.vz = fx_079_bodyPos.vz;
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
            if (fx_079_unitHeight > FX_DEMI_SHOCK_SHRINK_HEIGHT) {
                scale.vx = scale.vy = scale.vz =
                    fxm_easeLerp(2, FX_DEMI_SHOCK_GROW_FRAMES, work->timers0[FX_DEMI_SHOCK_T_LIFT], fx_079_unitScale.vy, fx_079_unitScale.vy >> 1);
                ex->btlfx_setUnitScale(0, (s32 *)&scale);
            }
            pos.vy = fx_079_homePos.vy + fxm_easeLerp(2, FX_DEMI_SHOCK_GROW_FRAMES, work->timers0[FX_DEMI_SHOCK_T_LIFT], 0, -FX_DEMI_SHOCK_LIFT_HEIGHT);
            pos.vx = fx_079_homePos.vx;
            pos.vz = fx_079_homePos.vz;
            ex->btlfx_setUnitPos(0, (s16 *)&pos);
        }
        if (work->timers0[FX_DEMI_SHOCK_T_HOLD] != 0) {
            work->timers0[FX_DEMI_SHOCK_T_HOLD]--;
            pos.vy = fx_079_homePos.vy - FX_DEMI_SHOCK_LIFT_HEIGHT;
            pos.vx = fx_079_homePos.vx;
            pos.vz = fx_079_homePos.vz;
            ex->btlfx_setUnitPos(0, (s16 *)&pos);
        }
        if (work->timers0[FX_DEMI_SHOCK_T_DROP] != 0) {
            if (--work->timers0[FX_DEMI_SHOCK_T_DROP] != 0) {
                if (fx_079_unitHeight > FX_DEMI_SHOCK_SHRINK_HEIGHT) {
                    scale.vx = scale.vy = scale.vz =
                        fxm_easeLerp(2, FX_DEMI_SHOCK_GROW_FRAMES, work->timers0[FX_DEMI_SHOCK_T_DROP], fx_079_unitScale.vy >> 1, fx_079_unitScale.vy);
                    ex->btlfx_setUnitScale(0, (s32 *)&scale);
                }
                pos.vy = fx_079_homePos.vy + fxm_easeLerp(2, FX_DEMI_SHOCK_GROW_FRAMES, work->timers0[FX_DEMI_SHOCK_T_DROP], -FX_DEMI_SHOCK_LIFT_HEIGHT, 0);
                pos.vx = fx_079_homePos.vx;
                pos.vz = fx_079_homePos.vz;
            } else {
                ex->btlfx_getUnitHomePos(0, (s16 *)&pos);
                ex->btlfx_setUnitScale(0, (s32 *)&fx_079_unitScale);
            }
            ex->btlfx_setUnitPos(0, (s16 *)&pos);
        }
        ex->btlfx_getUnitBodyPos(0, 0, (s16 *)&fx_079_curBodyPos);
        fx_079_emitPos.vx = fx_079_curBodyPos.vx;
        fx_079_emitPos.vy = fx_079_curBodyPos.vy;
        fx_079_emitPos.vz = fx_079_curBodyPos.vz;
        if (fx_079_emitter != NULL && fx_079_emitter->state == BTLFX_EMIT_DONE) {
            fx_079_emitter = NULL;
        }
        ex->btlfx_moveEmitterTo((s32)fx_079_emitter, (s32)&fx_079_emitPos);
        if (work->timers0[FX_DEMI_SHOCK_T_SPHERE] != 0) {
            work->pos94.vx = fx_079_curBodyPos.vx;
            work->pos94.vy = fx_079_curBodyPos.vy;
            work->pos94.vz = fx_079_curBodyPos.vz;
            ex->btlfx_aimAngles((s16 *)&fx_079_curBodyPos, (s16 *)&env->camera->eye, (s16 *)&work->models40[1].rot4);
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
                tr = &fx_079_strips[i];
                if (tr->timer11C > 0) {
                    tr->timer11C--;
                    tr->lastSlot0 = tr->count2 = n - 1;
                    tr->head1 = 0;
                    cur.vx = fx_079_bonePos.vx << 12;
                    cur.vy = fx_079_bonePos.vy << 12;
                    cur.vz = fx_079_bonePos.vz << 12;
                    step.vx = ((work->pos94.vx + tr->endX5C - fx_079_bonePos.vx) << 12) / n;
                    step.vy = ((work->pos94.vy + tr->endY60 - fx_079_bonePos.vy) << 12) / n;
                    step.vz = ((work->pos94.vz + tr->endZ64 - fx_079_bonePos.vz) << 12) / n;
                    strip = (u8 *)tr;
                    pt = tr->pts4;
                    for (j = 0; j < n; pt++, j++) {
                        pt->vx = cur.vx >> 12;
                        pt->vz = cur.vz >> 12;
                        pt->vy = tr->offsetY6C[j] + (cur.vy >> 12);
                        pt->halfWidth6 = tr->timer11C << 2;
                        cur.vx += step.vx;
                        cur.vy += step.vy;
                        cur.vz += step.vz;
                        tr->offsetY6C[j] += tr->speedYC4[j];
                    }
                    if (tr->timer11C < FX_DEMI_SHOCK_STRAND_FADE_FRAMES) {
                        ex->btlfx_drawTrail((u8 *)ex->btlfx_getFileData(entry->file, FX_DEMI_SHOCK_SPARK_TRAIL), 0, NULL, frame, 0x1000,
                                            tr->timer11C * (0x1000 / FX_DEMI_SHOCK_STRAND_FADE_FRAMES), BTLFX_ABR_ADD, strip, 0);
                    } else {
                        ex->btlfx_drawTrail((u8 *)ex->btlfx_getFileData(entry->file, FX_DEMI_SHOCK_SPARK_TRAIL), 0, NULL, frame, 0x1000,
                                            0x1000, BTLFX_ABR_ADD, strip, 0);
                    }
                } else if (tr->timer11C < 0) {
                    tr->timer11C++;
                } else if (work->timers0[FX_DEMI_SHOCK_T_STRANDS] >= 5) {
                    tr->timer11C = ex->btlfx_randRange(4, 8);
                    j = 1;
                    pitch = ex->btlfx_rand() & 0xFFF;
                    yaw = ex->btlfx_rand() & 0xFFF;
                    rise = (ex->math_cos(pitch) * work->models40[0].scaleC.vx) >> 12;
                    tr->endY60 = rise;
                    tr->endX5C = (ex->math_sin(yaw) * abs(rise)) >> 12;
                    tr->endZ64 = (ex->math_cos(yaw) * abs(rise)) >> 12;
                    tr->speedYC4[0] = 0;
                    tr->offsetY6C[0] = 0;
                    for (; j < n - 1; j++) {
                        speedMax = j * 8 + 16;
                        speedMin = -speedMax;
                        tr->offsetY6C[j] = ex->btlfx_randRange(-(j * 16) - 32, j * 16 + 32);
                        tr->speedYC4[j] = ex->btlfx_randRange(speedMin, speedMax);
                    }
                    tr->speedYC4[j] = 0;
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
