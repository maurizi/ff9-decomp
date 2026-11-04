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
        *args = 0xA0;
    } else if (mode == 1) {
        fx_138_env = env;
        fxm_setSignal(env, BTLFX_SIG_WAIT, 0);
        ex->btlfx_initEffModels(fx_138_effModels, 4);
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
        work->unk34.vx = 0x800;
        work->unk34.vy = 0x800;
        work->unk34.vz = 0x800;
        reelAlias = fx_138_reels;
        sel = fx_138_randSets[ex->btlfx_rand() % 6];
        for (i = 0, reel = fx_138_reels; i < 3; i++, reelAlias++, reel++) {
            reel->unk8 = 0x200;
            reel->unkE = sel[i];
            reelAlias->rot0.vx = fx_138_pathRot.vx;
            reel->rot0.vy = fx_138_pathRot.vy;
            reel->rot0.vz = fx_138_pathRot.vz;
            work->model96[i] = ex->Hi_RegisterTexEffModel((BtlModelHeader *)ex->btlfx_getFileData(entry->file, fx_138_modelFileIds[i]), 0x99, 0x3D40, 0, 0);
        }
        work->rot48.vx = fx_138_pathRot.vx;
        work->rot48.vy = fx_138_pathRot.vy;
        work->rot48.vz = fx_138_pathRot.vz;
        work->pos50.vx = work->pos24.vx;
        work->pos50.vy = work->pos24.vy;
        work->pos50.vz = work->pos24.vz;
        work->scale60.vx = 0x800;
        work->scale60.vy = 0x800;
        work->scale60.vz = 0x800;
        work->model9C = ex->Hi_RegisterTexEffModel((BtlModelHeader *)ex->btlfx_getFileData(entry->file, FX_LUCKY_SEVEN_SLOT_STRIPS), 0x9A, 0x3E40, 0, 0x80);
        work->unk88 = 0;
        work->unk8A = sel[3];
        for (i = 0; i < 6; i++) {
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
        if (frame == 1) {
            work->active90 = frame;
            work->timer92 = 4;
        }
        if (work->active90 != 0 && work->timer92 >= 0) {
            ex->Hi_DrawEffModel(&work->rot48, &work->pos50, (s32)&work->scale60, work->model9C);
            ex->Hi_ModifyEffModelAbr(work->model9C, 1);
            ex->Hi_ModifyEffModelRGB(work->model9C, -0x60, -0x60, -0x60);
            if (--work->timer92 == 0) {
                work->unk8E = 0x1A;
                work->active90 = 0;
                fxm_setSignal(env, BTLFX_SIG_WAIT, 1);
            }
        }
        fxm_drawScrollMoveX(0x280 - (work->frame94 << 4), 0x280, 0x180, 0x40, 0x80, 0x240, 0x180);
        work->frame94++;
        if (frame >= 0x28) {
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
        *args = 0x34;
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
            for (i = 0; i < 6; i++) {
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
                work->emit18->trailSlots = 0x18;
                work->emit18->trailStride = 0x40;
                if (work->emit18 != NULL) {
                    work->emit18->otZ = ex->btlfx_addRadiusSixth(0, 0x80);
                }
            }
            break;
        case 0xC:
            work->emit1C = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, FX_LUCKY_SEVEN_FIRE_BURST), (s32)&work->pos20);
            if (work->emit1C != NULL) {
                work->emit1C->otZ = ex->btlfx_addRadiusSixth(0, 0x80);
            }
            break;
        }
        if (frame >= 0 && frame < 0xD) {
            ex->btlfx_drawSpriteLine((u8 *)ex->btlfx_getFileData(entry->file, FX_LUCKY_SEVEN_SPRITE_LINE), &work->pos28, work->frame30, work->fade32);
            if (frame >= 5) {
                work->fade32 -= 0x200;
            }
            work->frame30++;
            spriteFrame = work->frame30;
            if (spriteFrame >= 6) {
                spriteFrame = 5;
            }
            work->frame30 = spriteFrame;
        }
        return frame >= 0x3C;
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
