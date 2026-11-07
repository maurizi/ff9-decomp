#include "fxmod/fx_138.h"

s32 fx_138_op81SpawnPoint0(SVECTOR *out, s32 dir, s32 i) {
    fx_138_getPathPoint(out, &fx_138_posPaths[0], i);
    return 0;
}

s32 fx_138_op81SpawnPoint1(SVECTOR *out, s32 dir, s32 i) {
    fx_138_getPathPoint(out, &fx_138_posPaths[1], i);
    return 0;
}

s32 fx_138_op81SpawnPoint2(SVECTOR *out, s32 dir, s32 i) {
    fx_138_getPathPoint(out, &fx_138_posPaths[2], i);
    return 0;
}

s32 fx_138_op81SpawnPoint3(SVECTOR *out, s32 dir, s32 i) {
    fx_138_getPathPoint(out, &fx_138_posPaths[3], i);
    return 0;
}

s32 fx_138_op81SpawnPoint4(SVECTOR *out, s32 dir, s32 i) {
    fx_138_getPathPoint(out, &fx_138_posPaths[4], i);
    return 0;
}

s32 fx_138_op81SpawnPoint5(SVECTOR *out, s32 dir, s32 i) {
    fx_138_getPathPoint(out, &fx_138_posPaths[5], i);
    return 0;
}

void fx_138_getPathPoint(SVECTOR *out, Fx138Path *path, s32 key) {
    BtlfxExports *ex = BTLFX_EXPORTS(fx_138_env);
    MATRIX m;
    VECTOR worldPos;
    SVECTOR pathPos;

    fxm_posPathGet(path, key, &pathPos);
    pathPos.vx /= 2;
    pathPos.vy /= 2;
    pathPos.vz /= 2;
    ex->math_rotMatrixYXZ(&fx_138_pathRot, &m);
    gte_SetRotMatrix(&m);
    gte_ldtr0();
    gte_ldv0(&pathPos);
    gte_rtv0tr();
    gte_stlvnl(&worldPos);
    worldPos.vx += fx_138_pathOrigin.vx;
    worldPos.vy += fx_138_pathOrigin.vy;
    worldPos.vz += fx_138_pathOrigin.vz;
    out->vx = worldPos.vx;
    out->vy = worldPos.vy;
    out->vz = worldPos.vz;
}

s32 fx_138_op80(s32 mode, Fx138Work80 *work, Unk800F78A4 *env, Unk80100C78Entry *entry, s32 *args) {
    BtlfxExports *ex = BTLFX_EXPORTS(env);
    Fx138Ring *reelAlias;
    Fx138Ring *reel;
    s16 yaw;
    u16 *sel;
    s32 frame;
    s32 i;

    if (mode == 0) {
        *args = sizeof(Fx138Work80);
    } else if (mode == 1) {
        fx_138_env = env;
        fxm_setSignal(env, BTLFX_SIG_WAIT, 0);
        ex->btlfx_initEffModels(fx_138_effModels, FX_LUCKY_SEVEN_MODEL_COUNT);
        ex->btlfx_getUnitPos(0, &work->pos70.vx);
        ex->btlfx_getUnitRot(0, &work->rot78.vx);
        fx_138_pathRot.vx = 0;
        yaw = work->rot78.vy;
        fx_138_pathRot.vz = 0;
        fx_138_pathRot.vy = yaw + 0x800;
        work->pos24.vx = work->pos70.vx;
        work->pos24.vy = work->pos70.vy;
        work->pos24.vz = work->pos70.vz;
        work->pos24.vy -= 0x12C;
        fx_138_pathOrigin.vx = work->pos24.vx;
        fx_138_pathOrigin.vy = work->pos24.vy;
        fx_138_pathOrigin.vz = work->pos24.vz;
        work->unk34.vx = FX_LUCKY_SEVEN_STRIPS_SCALE;
        work->unk34.vy = FX_LUCKY_SEVEN_STRIPS_SCALE;
        work->unk34.vz = FX_LUCKY_SEVEN_STRIPS_SCALE;
        reelAlias = fx_138_reels;
        sel = fx_138_randSets[ex->btlfx_rand() % FX_LUCKY_SEVEN_RAND_SETS];
        for (i = 0, reel = fx_138_reels; i < FX_LUCKY_SEVEN_REELS; i++, reelAlias++, reel++) {
            reel->unk8 = 0x200;
            reel->unkE = sel[i];
            reelAlias->rot0.vx = fx_138_pathRot.vx;
            reel->rot0.vy = fx_138_pathRot.vy;
            reel->rot0.vz = fx_138_pathRot.vz;
            work->model96[i] = ex->Hi_RegisterTexEffModel((BtlModelHeader *)ex->btlfx_getFileData(entry->file, fx_138_modelFileIds[i]), FX_LUCKY_SEVEN_REEL_TPAGE, FX_LUCKY_SEVEN_REEL_CLUT, FX_LUCKY_SEVEN_REEL_U, FX_LUCKY_SEVEN_REEL_V);
        }
        work->rot48.vx = fx_138_pathRot.vx;
        work->rot48.vy = fx_138_pathRot.vy;
        work->rot48.vz = fx_138_pathRot.vz;
        work->pos50.vx = work->pos24.vx;
        work->pos50.vy = work->pos24.vy;
        work->pos50.vz = work->pos24.vz;
        work->scale60.vx = FX_LUCKY_SEVEN_STRIPS_SCALE;
        work->scale60.vy = FX_LUCKY_SEVEN_STRIPS_SCALE;
        work->scale60.vz = FX_LUCKY_SEVEN_STRIPS_SCALE;
        work->model9C = ex->Hi_RegisterTexEffModel((BtlModelHeader *)ex->btlfx_getFileData(entry->file, FX_LUCKY_SEVEN_RAINBOW_STRIPS), FX_LUCKY_SEVEN_RAINBOW_STRIPS_TPAGE, FX_LUCKY_SEVEN_RAINBOW_STRIPS_CLUT, FX_LUCKY_SEVEN_RAINBOW_STRIPS_U, FX_LUCKY_SEVEN_RAINBOW_STRIPS_V);
        work->unk88 = 0;
        work->unk8A = sel[3];
        for (i = 0; i < FX_LUCKY_SEVEN_PATHS; i++) {
            fxm_posPathInit(&fx_138_posPaths[i], (u16 *)ex->btlfx_getFileData(entry->file, fx_138_pathFileIds[i]));
        }
        work->unk8C = -1;
        work->unk8E = -1;
        work->active90 = 0;
        work->timer92 = -1;
        work->frame94 = 0;
        ex->btlfx_pointAhead(&fx_138_pathOrigin.vx, &work->rot78.vx, 0x4FD, &env->camera->points[0].vx);
        ex->btlfx_pointAhead(&fx_138_pathOrigin.vx, &work->rot78.vx, 0x87F, &env->camera->points[1].vx);
        ex->btlfx_pointAhead(&fx_138_pathOrigin.vx, &work->rot78.vx, 0xCFF, &env->camera->points[2].vx);
    } else {
        frame = entry->frame;
        if (frame == FX_LUCKY_SEVEN_STRIPS_FRAME) {
            work->active90 = frame;
            work->timer92 = FX_LUCKY_SEVEN_STRIPS_FRAMES;
        }
        if (work->active90 != 0 && work->timer92 >= 0) {
            ex->Hi_DrawEffModel(&work->rot48, &work->pos50, (s32)&work->scale60, work->model9C);
            ex->Hi_ModifyEffModelAbr(work->model9C, BTLFX_ABR_ADD);
            ex->Hi_ModifyEffModelRGB(work->model9C, -0x60, -0x60, -0x60);
            if (--work->timer92 == 0) {
                work->unk8E = 0x1A;
                work->active90 = 0;
                fxm_setSignal(env, BTLFX_SIG_WAIT, 1);
            }
        }
        fxm_drawScrollMoveX(0x280 - (work->frame94 << 4), FX_LUCKY_SEVEN_RAINBOW_VRAM_X, FX_LUCKY_SEVEN_RAINBOW_VRAM_Y, FX_LUCKY_SEVEN_RAINBOW_VRAM_W, FX_LUCKY_SEVEN_RAINBOW_VRAM_H, FX_LUCKY_SEVEN_RAINBOW_SRC_X, FX_LUCKY_SEVEN_RAINBOW_SRC_Y);
        work->frame94++;
        if (frame >= FX_LUCKY_SEVEN_REELS_FRAMES) {
            ex->btlfx_clearEffModels();
            return 1;
        }
    }
    return 0;
}

s32 fx_138_op81(s32 mode, Fx138Work81 *work, Unk800F78A4 *env, Unk80100C78Entry *entry, s32 *args) {
    BtlfxExports *ex = BTLFX_EXPORTS(env);
    Pool6CEntry *emitter;
    s32 frame;
    s32 i;
    u16 spriteFrame;

    if (mode == 0) {
        *args = sizeof(Fx138Work81);
    } else if (mode == 1) {
        fx_138_env = env;
        ex->btlfx_getUnitBodyPos(0, 0, (s16 *)&work->pos20);
        work->fade32 = 0x1000;
        work->frame30 = 0;
        work->pos28.vx = work->pos20.vx;
        work->pos28.vy = work->pos20.vy;
        work->pos28.vz = work->pos20.vz;
    } else {
        frame = entry->frame;
        switch (frame) {
        case 0:
            for (i = 0; i < FX_LUCKY_SEVEN_PATHS; i++) {
                work->emit0[i] = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, fx_138_emitFileIds[i]), (s32)&fx_138_emitPos[i]);
                if (work->emit0[i] != NULL) {
                    work->emit0[i]->tail->shape = -1;
                    *(s32 *)&work->emit0[i]->tail->s.module.spawnFn = fx_138_op81SpawnPoints[i];
                    if (work->emit0[i] != NULL) {
                        work->emit0[i]->otZ = ex->btlfx_addRadiusSixth(0, 0x80);
                    }
                }
            }
            work->emit18 = emitter = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, FX_LUCKY_SEVEN_RAINBOW_TRAIL), (s32)&work->pos20);
            if (emitter != NULL) {
                emitter->trails = (s32)fx_138_op81Trails;
                work->emit18->trailSlots = FX_LUCKY_SEVEN_RAINBOW_TRAIL_TRAIL_SLOTS;
                work->emit18->trailStride = FX_LUCKY_SEVEN_RAINBOW_TRAIL_TRAIL_STRIDE;
                if (work->emit18 != NULL) {
                    work->emit18->otZ = ex->btlfx_addRadiusSixth(0, 0x80);
                }
            }
            break;
        case FX_LUCKY_SEVEN_BURST_FRAME:
            work->emit1C = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, FX_LUCKY_SEVEN_FIRE_BURST), (s32)&work->pos20);
            if (work->emit1C != NULL) {
                work->emit1C->otZ = ex->btlfx_addRadiusSixth(0, 0x80);
            }
            break;
        }
        if (frame >= 0 && frame < FX_LUCKY_SEVEN_FLARE_FRAMES) {
            ex->btlfx_drawSpriteLine((u8 *)ex->btlfx_getFileData(entry->file, FX_LUCKY_SEVEN_LENS_FLARE), &work->pos28, work->frame30, work->fade32);
            if (frame >= FX_LUCKY_SEVEN_FLARE_FADE_FRAME) {
                work->fade32 -= 0x1000 / (FX_LUCKY_SEVEN_FLARE_FRAMES - FX_LUCKY_SEVEN_FLARE_FADE_FRAME);
            }
            work->frame30++;
            spriteFrame = work->frame30;
            if (spriteFrame >= FX_LUCKY_SEVEN_FLARE_ANIM_FRAMES) {
                spriteFrame = FX_LUCKY_SEVEN_FLARE_ANIM_FRAMES - 1;
            }
            work->frame30 = spriteFrame;
        }
        return frame >= FX_LUCKY_SEVEN_FRAMES;
    }
    return 0;
}

static void fxm_lerpSvec(SVECTOR *from, SVECTOR *to, s32 t, SVECTOR *out) {
    BTLFX_EXPORTS(fx_138_env)->LoadAverageShort12(from, to, 0x1000 - t, t, out);
}

static s32 fxm_lerpRatio(s32 from, s32 to, s32 t, s32 n) {
    return (from * (n - t) + to * t) / n;
}

static void fxm_posPathInit(Fx138Path *path, u16 *data) {
    data++;
    path->count2 = *data++;
    path->keys4 = (Fx138Point *)data;
}

static s32 fxm_posPathGet(Fx138Path *path, s32 t, SVECTOR *out) {
    s32 last = path->count2 - 1;
    s32 done = 1;
    Fx138Point *key;

    if (last < t) {
        t = last;
    } else {
        done = 0;
    }
    key = &path->keys4[t];
    out->vx = key->x;
    out->vy = key->y;
    out->vz = key->z;
    return done;
}

static void fxm_setSignal(Unk800F78A4 *env, s32 signal, s32 value) {
    env->state->signals[signal] = value;
}

static void fxm_drawScrollMoveX(s32 x, s32 dx, s32 dy, s32 w, s32 h, s32 sx, s32 sy) {
    s32 wrapW = x % w;
    Unk800F78A4 *env = fx_138_env;
    u32 *move = (u32 *)env->prim;
    BtlfxExports *ex = BTLFX_EXPORTS(env);
    s32 restW;
    RECT rectRest;
    RECT rectWrap;

    env->prim = (u8 *)(move + 12);
    restW = w - wrapW;
    if (wrapW != 0) {
        rectWrap.x = sx;
        rectWrap.y = sy;
        rectWrap.w = wrapW;
        rectWrap.h = h;
        ex->SetDrawMove(move, &rectWrap, dx + restW, dy);
        addPrim(env->ot + OT_LAST, move);
    }
    move += 6;
    if (restW != 0) {
        rectRest.x = sx + wrapW;
        rectRest.y = sy;
        rectRest.w = restW;
        rectRest.h = h;
        ex->SetDrawMove(move, &rectRest, dx, dy);
        addPrim(env->ot + OT_LAST, move);
    }
}

INCLUDE_RODATA("asm/us/fx_138/nonmatchings/fx_138", fx_138_headerPtr);
