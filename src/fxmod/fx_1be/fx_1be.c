#include "fxmod/fx_1be.h"

static s32 fxm_easeLerpSvec(s32 curve, s32 n, s32 t, SVECTOR *from, SVECTOR *to, SVECTOR *out) {
    BtlfxExports *ex = BTLFX_EXPORTS(fx_1be_env);
    s32 invSq = 0x20000000 / (n * n);
    s32 ratio = 0;

    switch (curve) {
    case FX_ATOMOS_EASE_LINEAR:
        ratio = ((0x10000000 / n) * t) >> 16;
        break;
    case FX_ATOMOS_EASE_SQUARED:
        ratio = 0x1000 - ((invSq * (n - t) * (n - t)) >> 17);
        break;
    case FX_ATOMOS_EASE_INV_SQUARED:
        ratio = (invSq * t * t) >> 17;
        break;
    }
    return ex->btlfx_lerpVec((s16 *)from, (s16 *)to, 0x1000 - ratio, (s16 *)out);
}

static s32 fxm_easeLerp(s32 curve, s32 n, s32 t, s32 from, s32 to) {
    BtlfxExports *ex = BTLFX_EXPORTS(fx_1be_env);
    s32 invSq = 0x20000000 / (n * n);
    s32 ratio = 0;

    switch (curve) {
    case FX_ATOMOS_EASE_LINEAR:
        ratio = 0x1000 - (((0x10000000 / n) * t) >> 16);
        break;
    case FX_ATOMOS_EASE_SQUARED:
        ratio = (invSq * (n - t) * (n - t)) >> 17;
        break;
    case FX_ATOMOS_EASE_INV_SQUARED:
        ratio = 0x1000 - ((invSq * t * t) >> 17);
        break;
    }
    return ex->btlfx_lerp(from, to, ratio);
}

s32 fx_1be_op80(s32 mode, Fx1beWork80 *w, Unk800F78A4 *env, Unk80100C78Entry *entry, s32 *args) {
    BtlfxExports *ex = BTLFX_EXPORTS(env);
    SVECTOR aim;
    VECTOR sc;
    SVECTOR ang;
    SVECTOR pos;
    SVECTOR rot2;
    CVECTOR col;
    SVECTOR rot;
    VECTOR scale;
    Fx1beUnit *unit;
    Pool6CEntry *emitter;
    s32 frame;
    s32 i;
    s32 radius;
    s32 bright;
    s32 dirAng;

    if (mode == 0) {
        *args = sizeof(Fx1beWork80);
    } else if (mode == 1) {
        fx_1be_env = env;
        ex->btlfx_initEffModels(fx_1be_effModels, FX_ATOMOS_MODEL_COUNT);
        {
            u8 *counter = &w->counters0[FX_ATOMOS_COUNTERS - 1];
            for (i = FX_ATOMOS_COUNTERS - 1; i >= 0; i--, counter--) {
                *counter = 0;
            }
        }
        for (i = FX_ATOMOS_EMITTERS - 1; i >= 0; i--) {
            w->emit20[i] = NULL;
        }
        fx_1be_scrollPos = 8;
        fx_1be_scrollSpeed = 8;
        fx_1be_scrollInitWord = 2;
        for (i = 0; i < env->targets->targetCount; i++) {
            fx_1be_units[i].timer34 = ex->btlfx_rand() & 7;
            fx_1be_units[i].timer30 = FX_ATOMOS_UNIT_PULL_FRAMES;
            fx_1be_units[i].spin28.vx = ex->btlfx_randRange(-0x20, 0x20);
            fx_1be_units[i].spin28.vy = ex->btlfx_randRange(-0x40, 0x40);
            fx_1be_units[i].spin28.vz = ex->btlfx_randRange(-0x20, 0x20);
            ex->btlfx_getUnitScale(i, &fx_1be_units[i].scale8);
            ex->btlfx_getUnitRot(i, (s16 *)&fx_1be_units[i].rot0);
            ex->btlfx_getUnitRot(i, (s16 *)&fx_1be_units[i].rot20);
            ex->btlfx_getUnitHomePos(i, (s16 *)&fx_1be_units[i].pos18);
        }
        for (i = 0; i < FX_ATOMOS_DEBRIS_ENDS; i++) {
            radius = ex->btlfx_rand() & 0xFFF;
            dirAng = (ex->btlfx_rand() & 0xDFF) + 0xB00;
            fx_1be_op80ParticleEnds[i].vx = (ex->math_sin(dirAng) * radius) >> 12;
            fx_1be_op80ParticleEnds[i].vz = ((ex->math_cos(dirAng) * radius) >> 12) + FX_ATOMOS_HOLE_Z;
            fx_1be_op80ParticleEnds[i].vy = 0;
        }
        w->posD4.vx = 0;
        w->posD4.vy = 0;
        w->posD4.vz = FX_ATOMOS_HOLE_Z;
        w->unkCC.vx = 0;
        w->unkCC.vy = FX_ATOMOS_GATE_START_Y;
        w->unkCC.vz = FX_ATOMOS_HOLE_Z;
        w->model60 = ex->btlfx_registerEffectModel((Unk800E8F50 *)ex->btlfx_getFileData(entry->file, FX_ATOMOS_RED_CLOUD_SPHERE), 0xFF);
        w->scale74.vx = 0x1800;
        w->scale74.vy = 0x1800;
        w->scale74.vz = 0x1800;
        w->rot84.vx = 0;
        w->rot84.vy = 0;
        w->rot84.vz = 0;
        w->pos64.vx = 0;
        w->pos64.vy = FX_ATOMOS_GATE_START_Y;
        w->pos64.vz = FX_ATOMOS_HOLE_Z;
        w->pos8C.vz = FX_ATOMOS_HOLE_Z;
        w->pos8C.vx = 0;
        w->pos8C.vy = FX_ATOMOS_GATE_START_Y;
        w->scaleAC.vx = 0x1A80;
        w->scaleAC.vy = 0x1A80;
        w->scaleAC.vz = 0x1A80;
        w->modelA8 = ex->btlfx_registerEffectModel((Unk800E8F50 *)ex->btlfx_getFileData(entry->file, FX_ATOMOS_VORTEX_DOME), 1);
        w->model98 = ex->btlfx_registerEffectModel((Unk800E8F50 *)ex->btlfx_getFileData(entry->file, FX_ATOMOS_RING_FUNNEL), 1);
        w->model94 = ex->btlfx_registerEffectModel((Unk800E8F50 *)ex->btlfx_getFileData(entry->file, FX_ATOMOS_VORTEX_TORUS), 1);
        w->modelA4 = ex->btlfx_registerEffectModel((Unk800E8F50 *)ex->btlfx_getFileData(entry->file, FX_ATOMOS_ORANGE_RING), 1);
    } else {
        frame = entry->frame;
        env->camera->points[1].vx = 0;
        env->camera->points[1].vy = FX_ATOMOS_HOLE_Y;
        env->camera->points[1].vz = FX_ATOMOS_HOLE_Z;
        env->camera->points[3].vx = w->pos64.vx;
        env->camera->points[3].vy = w->pos64.vy;
        env->camera->points[3].vz = w->pos64.vz;
        switch (frame) {
        case 0:
            w->counters0[FX_ATOMOS_T_GATE_DROP] = FX_ATOMOS_GATE_DROP_FRAMES;
            w->counters0[FX_ATOMOS_T_DOME] = FX_ATOMOS_DOME_FRAMES;
            w->counters0[FX_ATOMOS_T_GATE] = 1;
            w->counters0[FX_ATOMOS_T_DOME_SCROLL] = 1;
            w->counters0[FX_ATOMOS_T_ZOOM] = FX_ATOMOS_ZOOM_FRAMES;
            break;
        case FX_ATOMOS_STREAKS_FRAME:
            w->emit20[0] = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, FX_ATOMOS_ORANGE_STREAKS), (s32)&w->posBC);
            w->emit20[1] = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, FX_ATOMOS_VIOLET_RING), (s32)&w->posC4);
            if (w->emit20[0] != NULL) {
                w->emit20[0]->otZ = 0x200;
            }
            if (w->emit20[1] != NULL) {
                w->emit20[1]->otZ = 0x200;
            }
            w->counters0[FX_ATOMOS_T_STREAKS_MOVE] = FX_ATOMOS_STREAKS_MOVE_FRAMES;
            w->counters0[FX_ATOMOS_T_VORTEX_IN] = FX_ATOMOS_VORTEX_IN_FRAMES;
            w->counters0[FX_ATOMOS_T_VORTEX] = 1;
            w->counters0[FX_ATOMOS_T_DOME_SCROLL] = 1;
            w->counters0[FX_ATOMOS_T_PULL] = FX_ATOMOS_PULL_FRAMES;
            emitter = w->emit20[2] = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, FX_ATOMOS_ROCK_DEBRIS), (s32)&w->posD4);
            if (emitter != NULL) {
                emitter->file->m.module.update = fx_1be_op80ParticleMotion;
            }
            break;
        case FX_ATOMOS_STREAKS_OUT_FRAME:
            w->counters0[FX_ATOMOS_T_STREAKS_OUT] = FX_ATOMOS_STREAKS_OUT_FRAMES;
            break;
        case FX_ATOMOS_FUNNEL_FRAME:
            w->counters0[FX_ATOMOS_T_GRIDS] = FX_ATOMOS_GRID_FRAMES;
            w->counters0[FX_ATOMOS_T_GRID_IN] = FX_ATOMOS_GRID_FADE_FRAMES;
            w->counters0[FX_ATOMOS_T_SPEED_UP] = frame; /* FUNNEL_FRAME frames */
            w->counters0[FX_ATOMOS_T_FUNNEL] = 1;
            ex->btlfx_stopEmitter(w->emit20[2]);
            w->scale74.vx = FX_ATOMOS_VORTEX_SCALE;
            w->scale74.vy = FX_ATOMOS_VORTEX_SCALE;
            w->scale74.vz = FX_ATOMOS_VORTEX_SCALE;
            w->pos64.vx = 0;
            w->pos64.vy = FX_ATOMOS_HOLE_Y;
            w->pos64.vz = FX_ATOMOS_HOLE_Z;
            w->pos8C.vx = 0;
            w->pos8C.vy = FX_ATOMOS_HOLE_Y;
            w->pos8C.vz = FX_ATOMOS_HOLE_Z;
            emitter = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, FX_ATOMOS_NAVY_RINGS), (s32)&w->pos8C);
            if (emitter != NULL) {
                emitter->otZ = 0x400;
            }
            break;
        case FX_ATOMOS_RINGS_FRAME:
            ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, FX_ATOMOS_FIRE_RINGS), (s32)&w->pos8C);
            emitter = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, FX_ATOMOS_PURPLE_RINGS), (s32)&w->pos8C);
            if (emitter != NULL) {
                emitter->otZ = 0x80;
            }
            emitter = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, FX_ATOMOS_BLUE_SPECKS), (s32)&w->pos8C);
            if (emitter != NULL) {
                emitter->otZ = 0x100;
            }
            break;
        case FX_ATOMOS_SHRINK_FRAME:
            w->counters0[FX_ATOMOS_T_SHRINK] = FX_ATOMOS_SHRINK_FRAMES;
            break;
        case FX_ATOMOS_CLOSE_FRAME:
            w->counters0[FX_ATOMOS_T_GATE] = 0;
            w->counters0[FX_ATOMOS_T_VORTEX] = 0;
            w->counters0[FX_ATOMOS_T_DOME_SCROLL] = 0;
            break;
        case FX_ATOMOS_GRID_OUT_FRAME:
            w->counters0[FX_ATOMOS_T_GRID_OUT] = FX_ATOMOS_GRID_FADE_FRAMES;
            break;
        case FX_ATOMOS_LAND_FRAME:
            for (i = 0; i < env->targets->targetCount; i++) {
                ex->btlfx_setUnitScale(i, (s32 *)&fx_1be_units[i].scale8);
                ex->btlfx_setUnitRot(i, (s16 *)&fx_1be_units[i].rot0);
            }
            w->counters0[FX_ATOMOS_T_LAND] = FX_ATOMOS_LAND_FRAMES;
            break;
        case FX_ATOMOS_FRAMES:
            return 1;
        }
        for (i = 0; i < FX_ATOMOS_EMITTERS; i++) {
            if (w->emit20[i] != NULL && w->emit20[i]->state == BTLFX_EMIT_DONE) {
                w->emit20[i] = NULL;
            }
        }
        if (w->counters0[FX_ATOMOS_T_GATE_DROP] != 0) {
            w->counters0[FX_ATOMOS_T_GATE_DROP]--;
            w->pos64.vy = fxm_easeLerp(FX_ATOMOS_EASE_INV_SQUARED, FX_ATOMOS_GATE_DROP_FRAMES, w->counters0[FX_ATOMOS_T_GATE_DROP], FX_ATOMOS_GATE_START_Y,
                                       FX_ATOMOS_GATE_END_Y);
            w->pos8C.vy = w->pos64.vy;
        }
        if (w->counters0[FX_ATOMOS_T_SHRINK] != 0) {
            w->counters0[FX_ATOMOS_T_SHRINK]--;
            w->scale74.vx = w->scale74.vz = w->scale74.vy =
                fxm_easeLerp(FX_ATOMOS_EASE_SQUARED, FX_ATOMOS_SHRINK_FRAMES, w->counters0[FX_ATOMOS_T_SHRINK], FX_ATOMOS_VORTEX_SCALE, 0);
        }
        if (w->counters0[FX_ATOMOS_T_GATE] != 0) {
            ex->btlfx_drawLayerTexture((BtlBgLayers *)ex->btlfx_getFileData(entry->file, FX_ATOMOS_BACKDROP), frame);
            ex->Hi_DrawEffModel(&w->rot84, &w->pos64, (s32)&w->scale74, w->model60);
        }
        if (w->counters0[FX_ATOMOS_T_DOME] != 0) {
            w->counters0[FX_ATOMOS_T_DOME]--;
            ex->Hi_DrawEffModel(&w->rot84, &w->pos64, (s32)&w->scaleAC, w->modelA8);
            if (w->counters0[FX_ATOMOS_T_DOME] < FX_ATOMOS_DOME_FADE_FRAMES) {
                bright = (w->counters0[FX_ATOMOS_T_DOME] - FX_ATOMOS_DOME_FADE_FRAMES) * 0x10;
                ex->Hi_ModifyEffModelRGB(w->modelA8, bright, bright, bright);
            }
        }
        if (w->counters0[FX_ATOMOS_T_STREAKS_MOVE] != 0) {
            w->counters0[FX_ATOMOS_T_STREAKS_MOVE]--;
            ex->btlfx_moveEmitterTo((s32)w->emit20[0], (s32)&w->pos8C);
            ex->btlfx_moveEmitterTo((s32)w->emit20[1], (s32)&w->pos8C);
        }
        if (w->counters0[FX_ATOMOS_T_STREAKS_OUT] != 0) {
            if (--w->counters0[FX_ATOMOS_T_STREAKS_OUT] != 0) {
                if (w->emit20[0] != NULL) {
                    w->emit20[0]->alpha = w->counters0[FX_ATOMOS_T_STREAKS_OUT] << 8;
                }
                if (w->emit20[1] != NULL) {
                    w->emit20[1]->alpha = w->counters0[FX_ATOMOS_T_STREAKS_OUT] << 8;
                }
            } else {
                ex->btlfx_stopEmitter(w->emit20[0]);
                ex->btlfx_stopEmitter(w->emit20[1]);
            }
        }
        if (w->counters0[FX_ATOMOS_T_VORTEX] != 0) {
            aim.vx = w->pos64.vx;
            aim.vy = w->pos64.vy;
            aim.vz = w->pos64.vz;
            ex->btlfx_aimAngles((s16 *)&env->camera->eye, (s16 *)&aim, (s16 *)&w->rot9C);
            ex->Hi_DrawEffModel(&w->rot9C, &w->pos64, (s32)&w->scale74, w->model94);
            if (w->counters0[FX_ATOMOS_T_FUNNEL] != 0) {
                ex->Hi_DrawEffModel(&w->rot9C, &w->pos64, (s32)&w->scale74, w->model98);
                ex->Hi_DrawEffModel(&w->rot84, &w->pos64, (s32)&w->scale74, w->modelA4);
            }
        }
        if (w->counters0[FX_ATOMOS_T_VORTEX_IN] != 0) {
            w->counters0[FX_ATOMOS_T_VORTEX_IN]--;
            bright = -(w->counters0[FX_ATOMOS_T_VORTEX_IN] * 0x10);
            ex->Hi_ModifyEffModelRGB(w->model94, bright, bright, bright);
        }
        if (w->counters0[FX_ATOMOS_T_PULL] != 0) {
            w->counters0[FX_ATOMOS_T_PULL]--;
            for (i = 0; i < env->targets->targetCount; i++) {
                unit = &fx_1be_units[i];
                if (unit->timer34 == 0) {
                    if (unit->timer30 != 0) {
                        unit->timer30--;
                        fxm_easeLerpSvec(FX_ATOMOS_EASE_SQUARED, FX_ATOMOS_UNIT_PULL_FRAMES, unit->timer30, &fx_1be_units[i].pos18, &w->pos8C, &ang);
                        ex->btlfx_setUnitPos(i, (s16 *)&ang);
                        sc.vx = sc.vy = sc.vz = fxm_easeLerp(FX_ATOMOS_EASE_SQUARED, FX_ATOMOS_UNIT_PULL_FRAMES, unit->timer30, unit->scale8.vy, 0);
                        ex->btlfx_setUnitScale(i, (s32 *)&sc);
                        unit->rot20.vx += unit->spin28.vx;
                        unit->rot20.vy += unit->spin28.vy;
                        unit->rot20.vz += unit->spin28.vz;
                        ex->btlfx_setUnitRot(i, (s16 *)&fx_1be_units[i].rot20);
                    }
                } else {
                    unit->timer34--;
                }
            }
        }
        if (w->counters0[FX_ATOMOS_T_GRIDS] != 0) {
            bright = 0xFFF;
            w->counters0[FX_ATOMOS_T_GRIDS]--;
            if (w->counters0[FX_ATOMOS_T_GRID_IN] != 0) {
                w->counters0[FX_ATOMOS_T_GRID_IN]--;
                bright = (FX_ATOMOS_GRID_FADE_FRAMES - w->counters0[FX_ATOMOS_T_GRID_IN]) << 8;
            }
            if (w->counters0[FX_ATOMOS_T_GRID_OUT] != 0) {
                w->counters0[FX_ATOMOS_T_GRID_OUT]--;
                bright = w->counters0[FX_ATOMOS_T_GRID_OUT] << 8;
            }
            ex->btlfx_aimAngles((s16 *)&w->pos8C, (s16 *)&env->camera->eye, (s16 *)&rot2);
            pos.vz = FX_ATOMOS_HOLE_Z + 0x800;
            pos.vx = 0;
            pos.vy = -0x1200;
            rot2.vz = 0;
            rot2.vx += 0xA0;
            fx_1be_drawGradientGrid(&pos, &rot2, bright, FX_ATOMOS_GRID_TPAGE, FX_ATOMOS_GRID_CLUT, BTLFX_ABR_ADD);
            pos.vy = 0x200;
            rot2.vz = 0x800;
            rot2.vx -= 0x140;
            fx_1be_drawGradientGrid(&pos, &rot2, bright, FX_ATOMOS_GRID_TPAGE, FX_ATOMOS_GRID_CLUT, BTLFX_ABR_ADD);
        }
        if (w->counters0[FX_ATOMOS_T_LAND] != 0) {
            w->counters0[FX_ATOMOS_T_LAND]--;
            for (i = 0; i < env->targets->targetCount; i++) {
                ex->btlfx_getUnitHomePos(i, (s16 *)&fx_1be_units[i].pos18);
                fx_1be_units[i].pos18.vy -= ex->math_sin(w->counters0[FX_ATOMOS_T_LAND] << 7) >> 1;
                ex->btlfx_setUnitPos(i, (s16 *)&fx_1be_units[i].pos18);
                if (w->counters0[FX_ATOMOS_T_LAND] == 0) {
                    fx_1be_units[i].pos18.vy = 0;
                    emitter = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, FX_ATOMOS_GREY_RINGS), (s32)&fx_1be_units[i].pos18);
                    if (emitter != NULL) {
                        emitter->otZ = -0x100;
                    }
                }
            }
        }
        if (w->counters0[FX_ATOMOS_T_SPEED_UP] != 0) {
            w->counters0[FX_ATOMOS_T_SPEED_UP]--;
            fx_1be_scrollSpeed = fxm_easeLerp(FX_ATOMOS_EASE_SQUARED, FX_ATOMOS_FUNNEL_FRAME, w->counters0[FX_ATOMOS_T_SPEED_UP], 4, 0x14);
        }
        fx_1be_scrollPos += fx_1be_scrollSpeed;
        if (w->counters0[FX_ATOMOS_T_VORTEX] != 0) {
            ex->btlfx_drawScrollMove(fx_1be_scrollPos, FX_ATOMOS_RING_VRAM_X, FX_ATOMOS_RING_VRAM_Y, FX_ATOMOS_RING_VRAM_W, FX_ATOMOS_RING_VRAM_H,
                                     FX_ATOMOS_RING_SRC_X, FX_ATOMOS_RING_SRC_Y);
        }
        if (w->counters0[FX_ATOMOS_T_DOME_SCROLL] != 0) {
            ex->btlfx_drawScrollMove(0x2710 - fx_1be_scrollPos, FX_ATOMOS_DOME_VRAM_X, FX_ATOMOS_DOME_VRAM_Y, FX_ATOMOS_DOME_VRAM_W, FX_ATOMOS_DOME_VRAM_H,
                                     FX_ATOMOS_DOME_SRC_X, FX_ATOMOS_DOME_SRC_Y);
        }
        if (w->counters0[FX_ATOMOS_T_GRIDS] != 0) {
            ex->btlfx_drawScrollMove(frame * 8, FX_ATOMOS_GRID_VRAM_X, FX_ATOMOS_GRID_VRAM_Y, FX_ATOMOS_GRID_VRAM_W, FX_ATOMOS_GRID_VRAM_H,
                                     FX_ATOMOS_GRID_SRC_X, FX_ATOMOS_GRID_SRC_Y);
        }
        if (w->counters0[FX_ATOMOS_T_ZOOM] != 0) {
            w->counters0[FX_ATOMOS_T_ZOOM]--;
            col.r = col.g = col.b = w->counters0[FX_ATOMOS_T_ZOOM] * 8;
            scale.vz = scale.vy = scale.vx = w->counters0[FX_ATOMOS_T_ZOOM] * 0x10 + 0x1000;
            rot.vx = 0;
            rot.vy = 0;
            rot.vz = 0;
            ex->btlfx_drawScreenGrid(3, 0xA0, 0x78, 0xA0, 0x78, 2, 2, 1, 1, &rot, &scale, &col, 1, NULL);
        }
    }
    return 0;
}

void fx_1be_drawGradientGrid(SVECTOR *pos, SVECTOR *rot, s32 bright, s32 tpage, s32 clut, s32 abr) {
    SVECTOR corner[4];
    SVECTOR gridCol;
    MATRIX m;
    Fx1beUV uv0;
    Fx1beUV uv1;
    Fx1beUV uv2;
    Fx1beUV uv3;
    CVECTOR col0;
    CVECTOR col1;
    Unk800F78A4 *env;
    BtlfxExports *ex;
    POLY_GT4 *poly;
    s32 i;
    s32 j;
    s32 bright0;
    s32 bright1;

    env = fx_1be_env;
    ex = BTLFX_EXPORTS(env);
    uv1.u = uv3.u = 0x5F;
    uv0.v = uv1.v = 0x40;
    uv0.u = uv2.u = 0;
    uv2.v = uv3.v = 0x9F;
    corner[0].vy = corner[1].vy = corner[2].vy = corner[3].vy = 0;
    gte_SetRotMatrix(env->viewMatrix);
    gte_SetTransMatrix(env->viewMatrix);
    gte_ldv0(pos);
    poly = (POLY_GT4 *)env->prim;
    tpage |= TPAGE_ABR(abr);
    env->prim = (u8 *)(poly + FX_ATOMOS_GRID_ROWS * FX_ATOMOS_GRID_COLS);
    gte_rtv0tr_b();
    ex->math_rotMatrixYXZ(rot, &m);
    gte_stlvnl(m.t);
    ex->MulRotMatrix(&m);
    gte_SetRotMatrix(&m);
    gte_SetTransMatrix(&m);
    bright /= 8;
    bright0 = 0;
    bright1 = bright;
    for (j = 0; j < FX_ATOMOS_GRID_ROWS; j++) {
        gridCol.vx = fx_1be_gridColors[j].vx;
        gridCol.vy = fx_1be_gridColors[j].vy;
        gridCol.vz = fx_1be_gridColors[j].vz;
        col0.r = (gridCol.vx * bright0) >> 12;
        col0.g = (gridCol.vy * bright0) >> 12;
        col0.b = (gridCol.vz * bright0) >> 12;
        gridCol.vx = fx_1be_gridColors[j + 1].vx;
        gridCol.vy = fx_1be_gridColors[j + 1].vy;
        gridCol.vz = fx_1be_gridColors[j + 1].vz;
        col1.r = (gridCol.vx * bright1) >> 12;
        col1.g = (gridCol.vy * bright1) >> 12;
        col1.b = (gridCol.vz * bright1) >> 12;
        bright0 += bright;
        bright1 += bright;
        corner[0].vz = corner[1].vz = j * FX_ATOMOS_GRID_CELL_Z - FX_ATOMOS_GRID_HALF;
        corner[2].vz = corner[3].vz = j * FX_ATOMOS_GRID_CELL_Z - (FX_ATOMOS_GRID_HALF - FX_ATOMOS_GRID_CELL_Z);
        for (i = 0; i < FX_ATOMOS_GRID_COLS; i++) {
            corner[0].vx = corner[2].vx = i * FX_ATOMOS_GRID_CELL_X - FX_ATOMOS_GRID_HALF;
            corner[1].vx = corner[3].vx = i * FX_ATOMOS_GRID_CELL_X - (FX_ATOMOS_GRID_HALF - FX_ATOMOS_GRID_CELL_X);
            gte_ldv3c(corner);
            gte_rtpt();
            poly->code = (abr != BTLFX_ABR_OPAQUE) ? GPU_POLY_GT4 | GPU_ABE : GPU_POLY_GT4;
            poly->r0 = poly->r1 = col0.r;
            poly->g0 = poly->g1 = col0.g;
            poly->b0 = poly->b1 = col0.b;
            poly->r2 = poly->r3 = col1.r;
            poly->g2 = poly->g3 = col1.g;
            poly->b2 = poly->b3 = col1.b;
            gte_stsxy3(&poly->x0, &poly->x1, &poly->x2);
            gte_ldv0(&corner[3]);
            gte_rtps();
            *(u16 *)&poly->u0 = *(u16 *)&uv0;
            *(u16 *)&poly->u1 = *(u16 *)&uv1;
            *(u16 *)&poly->u2 = *(u16 *)&uv2;
            *(u16 *)&poly->u3 = *(u16 *)&uv3;
            poly->clut = clut;
            poly->tpage = tpage;
            gte_stsxy(&poly->x3);
            ex->btlfx_addPrim(PRIM_TAG(POLY_GT4), 0, poly);
            poly++;
        }
    }
}

static s32 fx_1be_op80ParticleMotion(Pool30Entry *pt, Pool6CEntry *owner, SVECTOR *pos, s32 trail, s32 frame, s32 n) {
    SVECTOR lerpPos;
    SVECTOR from;
    SVECTOR *key;

    key = &fx_1be_op80ParticleEnds[(pt->slot - 1) & (FX_ATOMOS_DEBRIS_ENDS - 1)];
    from.vx = 0;
    from.vy = FX_ATOMOS_HOLE_Y;
    from.vz = FX_ATOMOS_HOLE_Z;
    fxm_easeLerpSvec(FX_ATOMOS_EASE_SQUARED, 0x1000, ((0x1000000 / n) * frame) >> 12, &from, key, &lerpPos);
    pos->vx = lerpPos.vx;
    pos->vy = lerpPos.vy;
    pos->vz = lerpPos.vz;
    pt->pos.vx = lerpPos.vx << 12;
    pt->pos.vy = lerpPos.vy << 12;
    pt->pos.vz = lerpPos.vz << 12;
    if (frame >= n) {
        return -1;
    }
    return 0;
}

INCLUDE_RODATA("asm/us/fx_1be/nonmatchings/fx_1be", fx_1be_headerPtr);
