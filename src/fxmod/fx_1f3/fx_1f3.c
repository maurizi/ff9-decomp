#include "fxmod/fx_1f3.h"

static void fxm_tintScreenRatio(s32 t, s32 n, s32 from, s32 to, s32 abr, s32 ot) {
    Unk800F78A4 *env = fx_1f3_env;
    BtlfxExports *ex = BTLFX_EXPORTS(env);
    s32 bright = ex->btlfx_lerpRatio(from, to, t, n);

    ex->btlfx_drawScreenTint((u32 *)(env->ot + ot), abr, bright, bright, bright);
}

static s32 fxm_easeLerp(s32 curve, s32 n, s32 t, s32 from, s32 to) {
    BtlfxExports *ex = BTLFX_EXPORTS(fx_1f3_env);
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

s32 fx_1f3_op80(s32 mode, Fx1f3Work80 *work, Unk800F78A4 *env, Unk80100C78Entry *entry, s32 *args) {
    BtlfxExports *ex = BTLFX_EXPORTS(env);
    SVECTOR vert;
    VECTOR out;
    SVECTOR cam;
    SVECTOR cam2;
    s32 flag;
    Pool6CEntry *emitter;
    Fx1f3Ring *unused0;
    Fx1f3RingDef *def;
    SVECTOR *dst;
    Fx1f3Line *trail;
    s32 elapsed;
    s32 grow;
    s32 frame;
    s32 i;
    s32 j;
    s32 shade;
    s32 scaleMin;
    s32 scaleMax;
    s32 *timerPtr;

    if (mode == 0) {
        *args = sizeof(Fx1f3Work80);
    } else if (mode == 1) {
        work->emit40[0] = NULL;
        work->emit40[1] = NULL;
        work->emit40[2] = NULL;
        work->emit40[3] = NULL;
        work->emit40[4] = NULL;
        work->emit40[5] = NULL;
        work->emit40[6] = NULL;
        fx_1f3_env = env;
        ex->btlfx_initEffModels(fx_1f3_effModels, FX_NEUTRON_RING_MODEL_SLOTS);
        for (i = FX_NEUTRON_RING_TIMERS - 1, timerPtr = &work->timer0[FX_NEUTRON_RING_TIMERS - 1]; i >= 0; i--, timerPtr--) {
            *timerPtr = 0;
        }
        work->pos5C.vx = env->targets->centerX;
        work->pos5C.vz = env->targets->centerZ;
        work->pos5C.vy = -0x200;
        for (i = 0; i < FX_NEUTRON_RING_RINGS; i++) {
            fx_1f3_rings[i].timer12C = 0;
            fx_1f3_rings[i].bone130 = fx_1f3_ringDefs[i].bone0;
            fx_1f3_rings[i].radius13C = fx_1f3_ringDefs[i].radius4;
            fx_1f3_rings[i].angle134 = ex->btlfx_rand() & 0xFFF;
            fx_1f3_rings[i].spin138 = (ex->btlfx_rand() & 0xFF) + 0x40;
            fx_1f3_rings[i].angleStep140 = 0x40;
            ex->btlfx_drawTrail((u8 *)ex->btlfx_getFileData(entry->file, FX_NEUTRON_RING_VIOLET_TRAIL), 0, NULL, -1, 0x1000, 0x1000, BTLFX_ABR_ADD,
                                (u8 *)&fx_1f3_rings[i].trail20, 2);
        }
        for (i = 0, scaleMax = 0x600, scaleMin = 0x400; i < FX_NEUTRON_RING_ARCS; i++, scaleMax += 0x200, scaleMin += 0x200) {
            fx_1f3_spinModels[i].model0 = ex->btlfx_registerEffectModel((Unk800E8F50 *)ex->btlfx_getFileData(entry->file, FX_NEUTRON_RING_TEAL_ARC), 1);
            fx_1f3_spinModels[i].scale10.vy = fx_1f3_spinModels[i].scale10.vx = fx_1f3_spinModels[i].scale10.vz = ex->btlfx_randRange(scaleMin, scaleMax);
            fx_1f3_spinModels[i].pos20.vx = work->pos5C.vx;
            fx_1f3_spinModels[i].pos20.vz = work->pos5C.vz;
            fx_1f3_spinModels[i].pos20.vy = work->pos5C.vy + ex->btlfx_randRange(-0x200, 0x200);
            fx_1f3_spinModels[i].rot8.vy = (i << 10) + (ex->btlfx_rand() & 0x3FF);
            fx_1f3_spinModels[i].rot8.vz = 0;
            fx_1f3_spinModels[i].rot8.vx = -0x400;
            fx_1f3_spinModels[i].spin4 = (ex->btlfx_rand() & 0x1FF) + 0x100;
        }
        work->model6C = ex->btlfx_registerEffectModel((Unk800E8F50 *)ex->btlfx_getFileData(entry->file, FX_NEUTRON_RING_FIRE_DISC), 1);
        work->pos70.vx = env->targets->centerX;
        work->pos70.vz = env->targets->centerZ;
        work->pos70.vy = -0x200;
        work->rot90.vx = 0;
        work->rot90.vy = 0;
        work->rot90.vz = 0;
        work->scale80.vz = work->scale80.vy = work->scale80.vx = 0x3000;
        work->model98 = ex->btlfx_registerEffectModel((Unk800E8F50 *)ex->btlfx_getFileData(entry->file, FX_NEUTRON_RING_FIRE_BOWL), 1);
        work->model9C = ex->btlfx_registerEffectModel((Unk800E8F50 *)ex->btlfx_getFileData(entry->file, FX_NEUTRON_RING_FIRE_BOWL), 1);
        work->rotA0.vx = 0;
        work->rotA0.vy = 0;
        work->rotA0.vz = 0;
        work->rotA8.vx = 0x800;
        work->rotA8.vy = 0;
        work->rotA8.vz = 0;
        work->scaleB0.vz = work->scaleB0.vy = work->scaleB0.vx = 0x400;
        work->modelC0 = ex->btlfx_registerEffectModel((Unk800E8F50 *)ex->btlfx_getFileData(entry->file, FX_NEUTRON_RING_TEAL_TORUS), 1);
        work->modelC4 = ex->btlfx_registerEffectModel((Unk800E8F50 *)ex->btlfx_getFileData(entry->file, FX_NEUTRON_RING_TEAL_TORUS), 1);
        work->rotC8.vx = 0;
        work->rotC8.vy = 0;
        work->rotC8.vz = 0;
        work->rotD0.vx = 0;
        work->rotD0.vy = 0;
        work->rotD0.vz = 0;
        work->scaleD8.vz = work->scaleD8.vy = work->scaleD8.vx = 0x400;
        work->modelE8 = ex->btlfx_registerEffectModel((Unk800E8F50 *)ex->btlfx_getFileData(entry->file, FX_NEUTRON_RING_GREY_DISC), 1);
        work->modelEC = ex->btlfx_registerEffectModel((Unk800E8F50 *)ex->btlfx_getFileData(entry->file, FX_NEUTRON_RING_GREEN_DISC), 1);
        work->rotF0.vx = 0;
        work->rotF0.vy = 0;
        work->rotF0.vz = 0;
        work->scaleF8.vz = work->scaleF8.vy = work->scaleF8.vx = 0x400;
        ex->btlfx_getUnitHomePos(BTLFX_UNIT_CASTER, (s16 *)&work->pos108);
        ex->btlfx_getUnitHomePos(BTLFX_UNIT_CASTER, (s16 *)&work->pos110);
        ex->btlfx_getUnitHomePos(BTLFX_UNIT_CASTER, (s16 *)&work->pos118);
        work->pos110.vy -= 0x400;
        work->pos118.vy -= 0x800;
    } else {
        frame = entry->frame;
        switch (frame) {
        case 0:
            work->timer0[FX_NEUTRON_RING_T_RINGS] = FX_NEUTRON_RING_RINGS_FRAMES;
            work->timer0[FX_NEUTRON_RING_T_CAM_DOWN] = FX_NEUTRON_RING_CAM_FRAMES;
            break;
        case FX_NEUTRON_RING_STREAKS_FRAME:
            ex->btlfx_getUnitBonePos(BTLFX_UNIT_CASTER, 0x2E, (u16 *)&work->pos64);
            work->emit40[0] = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, FX_NEUTRON_RING_LIGHT_STREAKS_2), (s32)&work->pos64);
            if (work->emit40[0] != NULL) {
                work->emit40[0]->otZ = 0x200;
            }
            break;
        case FX_NEUTRON_RING_YELLOW_RINGS_2_FRAME:
            emitter = work->emit40[1] = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, FX_NEUTRON_RING_YELLOW_RINGS_2), (s32)&work->pos64);
            if (emitter != NULL) {
                emitter->otZ = 0x200;
            }
            break;
        case FX_NEUTRON_RING_YELLOW_RINGS_FRAME:
            emitter = work->emit40[3] = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, FX_NEUTRON_RING_YELLOW_RINGS), (s32)&work->pos5C);
            if (emitter != NULL) {
                emitter->otZ = 0x200;
            }
            break;
        case FX_NEUTRON_RING_ORBIT_TRAIL_FRAME:
            work->emit40[2] = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, FX_NEUTRON_RING_ORBIT_TRAIL), (s32)&work->pos5C);
            if (work->emit40[2] != NULL) {
                work->emit40[2]->trails = (s32)fx_1f3_op80Trails;
                work->emit40[2]->trailSlots = FX_NEUTRON_RING_ORBIT_TRAIL_TRAIL_SLOTS;
                work->emit40[2]->trailStride = FX_NEUTRON_RING_ORBIT_TRAIL_TRAIL_STRIDE;
                if (work->emit40[2] != NULL) {
                    work->emit40[2]->otZ = 0x200;
                }
            }
            emitter = work->emit40[4] = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, FX_NEUTRON_RING_LIGHT_STREAKS), (s32)&work->pos5C);
            if (emitter != NULL) {
                emitter->otZ = 0x200;
            }
            break;
        case FX_NEUTRON_RING_DISC_FRAME:
            work->timer0[FX_NEUTRON_RING_T_DISC] = FX_NEUTRON_RING_DISC_FRAMES;
            break;
        case FX_NEUTRON_RING_LIGHTNING_FRAME:
            emitter = work->emit40[6] = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, FX_NEUTRON_RING_LIGHTNING_FLASH), (s32)&work->pos5C);
            if (emitter != NULL) {
                emitter->otZ = 0x200;
            }
            break;
        case FX_NEUTRON_RING_VIOLET_RINGS_FRAME:
            ex->btlfx_stopEmitter(work->emit40[6]);
            emitter = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, FX_NEUTRON_RING_VIOLET_RINGS), (s32)&work->pos5C);
            if (emitter != NULL) {
                emitter->otZ = 0x200;
            }
            work->timer0[FX_NEUTRON_RING_T_DISC_SHRINK] = FX_NEUTRON_RING_DISC_SHRINK_FRAMES;
            break;
        case FX_NEUTRON_RING_FLASH_FRAME:
            work->timer0[FX_NEUTRON_RING_T_FLASH] = FX_NEUTRON_RING_FLASH_FRAMES;
            break;
        case FX_NEUTRON_RING_STOP_FRAME:
            work->timer0[FX_NEUTRON_RING_T_DISC] = 0;
            ex->btlfx_stopEmitter(work->emit40[2]);
            ex->btlfx_stopEmitter(work->emit40[3]);
            ex->btlfx_stopEmitter(work->emit40[4]);
            ex->btlfx_stopEmitter(work->emit40[6]);
            break;
        case FX_NEUTRON_RING_BURST_FRAME:
            work->timer0[FX_NEUTRON_RING_T_WHITE] = FX_NEUTRON_RING_WHITE_FRAMES;
            work->timer0[FX_NEUTRON_RING_T_BOWLS] = FX_NEUTRON_RING_GROW_FRAMES;
            work->timer0[FX_NEUTRON_RING_T_TORI] = FX_NEUTRON_RING_GROW_FRAMES;
            work->timer0[FX_NEUTRON_RING_T_DISCS] = FX_NEUTRON_RING_GROW_FRAMES;
            work->timer0[FX_NEUTRON_RING_T_ARCS] = FX_NEUTRON_RING_ARCS_FRAMES;
            emitter = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, FX_NEUTRON_RING_COLOR_RINGS), (s32)&work->pos5C);
            if (emitter != NULL) {
                emitter->otZ = 0x200;
            }
            break;
        case FX_NEUTRON_RING_CAM_UP_FRAME:
            work->timer0[FX_NEUTRON_RING_T_CAM_UP] = FX_NEUTRON_RING_CAM_FRAMES;
            break;
        }
        if (work->emit40[0] != NULL && work->emit40[0]->state == BTLFX_EMIT_DONE) {
            work->emit40[0] = NULL;
        }
        if (work->emit40[1] != NULL && work->emit40[1]->state == BTLFX_EMIT_DONE) {
            work->emit40[1] = NULL;
        }
        if (work->emit40[2] != NULL && work->emit40[2]->state == BTLFX_EMIT_DONE) {
            work->emit40[2] = NULL;
        }
        if (work->emit40[3] != NULL && work->emit40[3]->state == BTLFX_EMIT_DONE) {
            work->emit40[3] = NULL;
        }
        if (work->emit40[4] != NULL && work->emit40[4]->state == BTLFX_EMIT_DONE) {
            work->emit40[4] = NULL;
        }
        if (work->emit40[5] != NULL && work->emit40[5]->state == BTLFX_EMIT_DONE) {
            work->emit40[5] = NULL;
        }
        if (work->emit40[6] != NULL && work->emit40[6]->state == BTLFX_EMIT_DONE) {
            work->emit40[6] = NULL;
        }
        if (work->emit40[1] != NULL) {
            ex->btlfx_drawLayerTexture((BtlBgLayers *)ex->btlfx_getFileData(entry->file, FX_NEUTRON_RING_BACKDROP), frame);
        }
        if (work->timer0[FX_NEUTRON_RING_T_DISC_SHRINK] != 0) {
            work->timer0[FX_NEUTRON_RING_T_DISC_SHRINK]--;
            work->scale80.vx = work->scale80.vy = work->scale80.vz = fxm_easeLerp(2, FX_NEUTRON_RING_DISC_SHRINK_FRAMES, work->timer0[FX_NEUTRON_RING_T_DISC_SHRINK], 0x3000, 0);
        }
        if (work->timer0[FX_NEUTRON_RING_T_DISC] != 0) {
            work->timer0[FX_NEUTRON_RING_T_DISC]--;
            ex->btlfx_aimAngles((s16 *)&env->camera->eye, (s16 *)&work->pos5C, (s16 *)&work->rot90);
            ex->Hi_DrawEffModel(&work->rot90, (VECTOR *)&work->pos70, (s32)&work->scale80, work->model6C);
            elapsed = FX_NEUTRON_RING_DISC_FRAMES - work->timer0[FX_NEUTRON_RING_T_DISC];
            if (elapsed < FX_NEUTRON_RING_DISC_FADE_FRAMES) {
                shade = (elapsed << 5) - 0x100;
                ex->Hi_ModifyEffModelRGB(work->model6C, shade, shade, shade);
            }
            ex->btlfx_drawScrollMove(0x2710 - frame * 8, FX_NEUTRON_RING_FIRE_VRAM_X, FX_NEUTRON_RING_FIRE_VRAM_Y, FX_NEUTRON_RING_FIRE_VRAM_W, FX_NEUTRON_RING_FIRE_VRAM_H,
                                 FX_NEUTRON_RING_FIRE_VRAM_X, FX_NEUTRON_RING_FIRE_SRC_Y);
        }
        if (work->timer0[FX_NEUTRON_RING_T_BOWLS] != 0) {
            work->timer0[FX_NEUTRON_RING_T_BOWLS]--;
            work->scaleB0.vx = work->scaleB0.vy = work->scaleB0.vz = fxm_easeLerp(2, FX_NEUTRON_RING_GROW_FRAMES, work->timer0[FX_NEUTRON_RING_T_BOWLS], 0x100, 0x400);
            ex->Hi_DrawEffModel(&work->rotA0, (VECTOR *)&work->pos70, (s32)&work->scaleB0, work->model98);
            ex->Hi_DrawEffModel(&work->rotA8, (VECTOR *)&work->pos70, (s32)&work->scaleB0, work->model9C);
            if (work->timer0[FX_NEUTRON_RING_T_BOWLS] < FX_NEUTRON_RING_GROW_FADE_FRAMES) {
                shade = -((FX_NEUTRON_RING_GROW_FADE_FRAMES - work->timer0[FX_NEUTRON_RING_T_BOWLS]) * 0x10);
                ex->Hi_ModifyEffModelRGB(work->model98, shade, shade, shade);
                ex->Hi_ModifyEffModelRGB(work->model9C, shade, shade, shade);
            }
            ex->btlfx_drawScrollMove(0x2710 - frame * 8, FX_NEUTRON_RING_FIRE_VRAM_X, FX_NEUTRON_RING_FIRE_VRAM_Y, FX_NEUTRON_RING_FIRE_VRAM_W, FX_NEUTRON_RING_FIRE_VRAM_H,
                                 FX_NEUTRON_RING_FIRE_VRAM_X, FX_NEUTRON_RING_FIRE_SRC_Y);
        }
        if (work->timer0[FX_NEUTRON_RING_T_DISCS] != 0) {
            work->timer0[FX_NEUTRON_RING_T_DISCS]--;
            work->scaleF8.vx = work->scaleF8.vy = work->scaleF8.vz = fxm_easeLerp(2, FX_NEUTRON_RING_GROW_FRAMES, work->timer0[FX_NEUTRON_RING_T_DISCS], 0x400, 0x1000);
            ex->Hi_DrawEffModel(&work->rotF0, (VECTOR *)&work->pos70, (s32)&work->scaleF8, work->modelE8);
            ex->Hi_DrawEffModel(&work->rotF0, (VECTOR *)&work->pos70, (s32)&work->scaleF8, work->modelEC);
            if (work->timer0[FX_NEUTRON_RING_T_DISCS] < FX_NEUTRON_RING_GROW_FADE_FRAMES) {
                shade = -((FX_NEUTRON_RING_GROW_FADE_FRAMES - work->timer0[FX_NEUTRON_RING_T_DISCS]) * 0x10);
                ex->Hi_ModifyEffModelRGB(work->modelE8, shade, shade, shade);
                ex->Hi_ModifyEffModelRGB(work->modelEC, shade, shade, shade);
            }
            ex->btlfx_drawScrollMove(0x2710 - frame * 8, FX_NEUTRON_RING_GREY_DISC_VRAM_X, FX_NEUTRON_RING_FIRE_VRAM_Y, FX_NEUTRON_RING_FIRE_VRAM_W, FX_NEUTRON_RING_FIRE_VRAM_H,
                                 FX_NEUTRON_RING_GREY_DISC_VRAM_X, FX_NEUTRON_RING_FIRE_SRC_Y);
        }
        if (work->timer0[FX_NEUTRON_RING_T_TORI] != 0) {
            work->timer0[FX_NEUTRON_RING_T_TORI]--;
            work->scaleD8.vx = work->scaleD8.vy = fxm_easeLerp(2, FX_NEUTRON_RING_GROW_FRAMES, work->timer0[FX_NEUTRON_RING_T_TORI], 0x400, 0xC00);
            work->scaleD8.vz = fxm_easeLerp(2, FX_NEUTRON_RING_GROW_FRAMES, work->timer0[FX_NEUTRON_RING_T_TORI], 0x400, 0x800);
            ex->Hi_DrawEffModel(&work->rotC8, (VECTOR *)&work->pos70, (s32)&work->scaleD8, work->modelC0);
            ex->Hi_DrawEffModel(&work->rotD0, (VECTOR *)&work->pos70, (s32)&work->scaleD8, work->modelC4);
            if (work->timer0[FX_NEUTRON_RING_T_TORI] < FX_NEUTRON_RING_GROW_FADE_FRAMES) {
                shade = -((FX_NEUTRON_RING_GROW_FADE_FRAMES - work->timer0[FX_NEUTRON_RING_T_TORI]) * 0x10);
                ex->Hi_ModifyEffModelRGB(work->modelC0, shade, shade, shade);
                ex->Hi_ModifyEffModelRGB(work->modelC4, shade, shade, shade);
            }
            work->rotC8.vy = 0x400;
            work->rotC8.vx = 0x200;
            work->rotD0.vy = -0x400;
            work->rotD0.vx = 0x200;
            work->rotC8.vz += 0x40;
            work->rotD0.vz += 0x40;
            ex->btlfx_drawScrollMove(0x2710 - frame * 8, FX_NEUTRON_RING_TORUS_VRAM_X, FX_NEUTRON_RING_FIRE_VRAM_Y, FX_NEUTRON_RING_TORUS_VRAM_W, FX_NEUTRON_RING_FIRE_VRAM_H,
                                 FX_NEUTRON_RING_TORUS_SRC_X, FX_NEUTRON_RING_FIRE_VRAM_Y);
        }
        if (work->timer0[FX_NEUTRON_RING_T_ARCS] != 0) {
            work->timer0[FX_NEUTRON_RING_T_ARCS]--;
            for (i = 0; i < FX_NEUTRON_RING_ARCS; i++) {
                fx_1f3_spinModels[i].rot8.vz += fxm_easeLerp(2, FX_NEUTRON_RING_ARCS_FRAMES, work->timer0[FX_NEUTRON_RING_T_ARCS], fx_1f3_spinModels[i].spin4, 0);
                grow = fxm_easeLerp(2, FX_NEUTRON_RING_ARCS_FRAMES, work->timer0[FX_NEUTRON_RING_T_ARCS], 0x40, 0);
                fx_1f3_spinModels[i].scale10.vz = 0x800;
                fx_1f3_spinModels[i].scale10.vy += grow;
                fx_1f3_spinModels[i].scale10.vx += grow;
                ex->Hi_DrawEffModel(&fx_1f3_spinModels[i].rot8, (VECTOR *)&fx_1f3_spinModels[i].pos20, (s32)&fx_1f3_spinModels[i].scale10,
                                    fx_1f3_spinModels[i].model0);
                if (work->timer0[FX_NEUTRON_RING_T_ARCS] < FX_NEUTRON_RING_ARCS_FADE_FRAMES) {
                    shade = -((FX_NEUTRON_RING_ARCS_FADE_FRAMES - work->timer0[FX_NEUTRON_RING_T_ARCS]) * 4);
                    ex->Hi_ModifyEffModelRGB(fx_1f3_spinModels[i].model0, shade, shade, shade);
                }
            }
        }
        if (work->timer0[FX_NEUTRON_RING_T_RINGS] != 0) {
            work->timer0[FX_NEUTRON_RING_T_RINGS]--;
            env->otZ = 0x80;
            if (work->timer0[FX_NEUTRON_RING_T_RINGS] >= FX_NEUTRON_RING_RING_FRAMES + 1) {
                for (i = 0; i < FX_NEUTRON_RING_RING_SET; i++) {
                    if (fx_1f3_rings[i].timer12C == 0) {
                        ex->btlfx_getUnitBoneMatrix(BTLFX_UNIT_CASTER, fx_1f3_rings[i].bone130, (BtlBone *)&fx_1f3_rings[i]);
                        fx_1f3_rings[i].timer12C = FX_NEUTRON_RING_RING_FRAMES;
                        break;
                    }
                }
                for (i = FX_NEUTRON_RING_RING_SET; i < FX_NEUTRON_RING_RINGS; i++) {
                    if (fx_1f3_rings[i].timer12C == 0) {
                        ex->btlfx_getUnitBoneMatrix(BTLFX_UNIT_CASTER, fx_1f3_rings[i].bone130, (BtlBone *)&fx_1f3_rings[i]);
                        fx_1f3_rings[i].timer12C = FX_NEUTRON_RING_RING_FRAMES;
                        break;
                    }
                }
            }
            for (i = 0; i < FX_NEUTRON_RING_RINGS; i++) {
                if (fx_1f3_rings[i].timer12C != 0) {
                    fx_1f3_rings[i].timer12C--;
                    ex->btlfx_getUnitBonePos(BTLFX_UNIT_CASTER, fx_1f3_rings[i].bone130, (u16 *)&vert);
                    fx_1f3_rings[i].matrix0.t[0] = vert.vx;
                    fx_1f3_rings[i].matrix0.t[1] = vert.vy;
                    fx_1f3_rings[i].matrix0.t[2] = vert.vz;
                    gte_SetRotMatrix(&fx_1f3_rings[i].matrix0);
                    gte_SetTransMatrix(&fx_1f3_rings[i].matrix0);
                    trail = &fx_1f3_rings[i].trail20;
                    trail->lastSlot0 = trail->count2 = FX_NEUTRON_RING_RING_POINTS - 1;
                    trail->head1 = 0;
                    dst = trail->points4;
                    for (j = 0; j < FX_NEUTRON_RING_RING_POINTS; j++, dst++) {
                        vert.vx = (ex->math_sin(j * fx_1f3_rings[i].angleStep140 + fx_1f3_rings[i].angle134) * fx_1f3_rings[i].radius13C) >> 12;
                        vert.vz = (ex->math_cos(j * fx_1f3_rings[i].angleStep140 + fx_1f3_rings[i].angle134) * fx_1f3_rings[i].radius13C) >> 12;
                        vert.vy = 0;
                        gte_ldv0(&vert);
                        gte_rtv0tr();
                        gte_stlvnl(&out);
                        gte_stflg(&flag);
                        dst->vx = out.vx;
                        dst->vy = out.vy;
                        dst->vz = out.vz;
                        dst->pad = 0xC0;
                    }
                    if (fx_1f3_rings[i].timer12C < 4) {
                        fx_1f3_rings[i].angle134 += fxm_easeLerp(2, FX_NEUTRON_RING_RING_FRAMES, fx_1f3_rings[i].timer12C, fx_1f3_rings[i].spin138, 0);
                        shade = fx_1f3_rings[i].timer12C << 10;
                    } else {
                        fx_1f3_rings[i].angle134 += fx_1f3_rings[i].spin138;
                        shade = (FX_NEUTRON_RING_RING_FRAMES - fx_1f3_rings[i].timer12C) << 10;
                    }
                    ex->btlfx_drawTrail((u8 *)ex->btlfx_getFileData(entry->file, FX_NEUTRON_RING_VIOLET_TRAIL), 0, NULL, 0, 0x1000, shade, BTLFX_ABR_ADD,
                                        (u8 *)&fx_1f3_rings[i].trail20, 2);
                }
            }
            env->otZ = 0;
        }
        if (work->timer0[FX_NEUTRON_RING_T_FLASH] != 0) {
            work->timer0[FX_NEUTRON_RING_T_FLASH]--;
            fxm_tintScreenRatio(work->timer0[FX_NEUTRON_RING_T_FLASH], FX_NEUTRON_RING_FLASH_FRAMES, 0xFF, 0, BTLFX_ABR_ADD, 0);
        }
        if (work->timer0[FX_NEUTRON_RING_T_WHITE] != 0) {
            work->timer0[FX_NEUTRON_RING_T_WHITE]--;
            fxm_tintScreenRatio(work->timer0[FX_NEUTRON_RING_T_WHITE], FX_NEUTRON_RING_WHITE_FRAMES, 0, 0xFF, BTLFX_ABR_ADD, 0);
        }
        if (work->timer0[FX_NEUTRON_RING_T_CAM_DOWN] != 0) {
            work->timer0[FX_NEUTRON_RING_T_CAM_DOWN]--;
            cam.vx = work->pos108.vx;
            cam.vz = work->pos108.vz;
            cam.vy = fxm_easeLerp(1, FX_NEUTRON_RING_CAM_FRAMES, work->timer0[FX_NEUTRON_RING_T_CAM_DOWN], work->pos108.vy, work->pos118.vy);
            ex->btlfx_setUnitPos(BTLFX_UNIT_CASTER, (s16 *)&cam);
        }
        if (work->timer0[FX_NEUTRON_RING_T_CAM_UP] != 0) {
            work->timer0[FX_NEUTRON_RING_T_CAM_UP]--;
            if (work->timer0[FX_NEUTRON_RING_T_CAM_UP] != 0) {
                cam2.vx = work->pos108.vx;
                cam2.vz = work->pos108.vz;
                cam2.vy = fxm_easeLerp(2, FX_NEUTRON_RING_CAM_FRAMES, work->timer0[FX_NEUTRON_RING_T_CAM_UP], work->pos118.vy, work->pos108.vy);
            } else {
                ex->btlfx_getUnitHomePos(BTLFX_UNIT_CASTER, (s16 *)&cam2);
            }
            ex->btlfx_setUnitPos(BTLFX_UNIT_CASTER, (s16 *)&cam2);
        }
        return frame >= FX_NEUTRON_RING_FRAMES;
    }
    return 0;
}

INCLUDE_RODATA("asm/us/fx_1f3/nonmatchings/fx_1f3", fx_1f3_headerPtr);
