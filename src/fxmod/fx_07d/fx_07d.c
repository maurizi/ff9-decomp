#include "fxmod/fx_07d.h"

static void fxm_drawRadialScreenGrid(u32 ot, s32 x, s32 y, s32 segs, s32 rings, s32 dr, s32 from, s32 to, RECT *src,
                   void (*fn)(s32, s32, s32, s32, DVECTOR *)) {
    DVECTOR pts[2][18];
    DVECTOR inPos;
    DVECTOR outPos;
    DVECTOR inPrev;
    DVECTOR outPrev;
    DVECTOR inUv;
    DVECTOR outUv;
    DVECTOR inUvPrev;
    DVECTOR outUvPrev;
    CVECTOR colIn;
    CVECTOR colOut;
    Unk800F78A4 *env = fx_07d_env;
    u32 *move = NULL;
    s32 step;
    s32 cx;
    s32 cy;
    s32 tpLo;
    s32 tpHi;
    s32 hiPage = 0;
    BtlfxExports *ex;
    POLY_GT4 *prim;
    s32 level;
    s32 ang;
    s32 vyOff;
    DVECTOR *uvRing;
    s32 radius;
    s32 i;
    s32 j;
    s32 sx;
    s32 sy;

    prim = (POLY_GT4 *)env->prim;
    env->prim = (u8 *)(prim + segs * rings);
    level = 0xFF;
    ex = BTLFX_EXPORTS(env);
    if (src == NULL) {
        level = 1;
    }
    colIn.cd = level != 0xFF ? 0x3E : 0x3C;
    if (src != NULL) {
        tpLo = getTPage(2, 0, src->x, src->y);
        tpHi = getTPage(2, 0, src->x + 0x80, src->y);
        sx = src->x;
        sy = src->y;
        src->x = env->drawEnv->clip.x + (x - (src->w >> 1));
        src->y = env->drawEnv->clip.y + (y - (src->h >> 1));
        move = (u32 *)env->prim;
        env->prim = (u8 *)move + 0x18;
        ex->SetDrawMove(move, src, sx, sy);
        cx = (src->w >> 1) + (sx & 0x3F);
        cy = (src->h >> 1) + (sy & 0xFF);
    } else {
        cx = x;
        cy = y;
        tpLo = getTPage(2, 1, env->dispEnv->disp.x, env->dispEnv->disp.y);
        tpHi = getTPage(2, 1, env->dispEnv->disp.x + 0x80, env->dispEnv->disp.y);
    }
    uvRing = pts[1];
    tpLo <<= 16;
    tpHi <<= 16;
    for (i = 0; i < 17; i++) {
        *(s32 *)&pts[0][i] = 0;
        uvRing[i].vx = cx;
        vyOff = i * 4 + 2;
        *(s16 *)((u8 *)uvRing + vyOff) = cy;
    }
    step = 0x1000 / segs;
    radius = 0;
    for (j = 1; j <= rings; j++) {
        level = ex->btlfx_lerpRatio(from, to, j - 1, rings);
        i = 0;
        ang = 0;
        colIn.r = colIn.g = colIn.b = level;
        level = ex->btlfx_lerpRatio(from, to, j, rings);
        colOut.r = colOut.g = colOut.b = level;
        radius += dr;
        for (; i <= segs; i++, ang += step) {
            *(s32 *)&inPos = *(s32 *)&pts[0][i];
            *(s32 *)&inUv = *(s32 *)&pts[1][i];
            if (i < segs) {
                outPos.vx = (ex->math_cos(ang) * radius) >> 12;
                outPos.vy = (ex->math_sin(ang) * radius) >> 12;
                outUv.vx = outPos.vx + cx;
                outUv.vy = outPos.vy + cy;
            } else {
                *(s32 *)&outPos = *(s32 *)&pts[0][0];
                *(s32 *)&outUv = *(s32 *)&pts[1][0];
            }
            if (outUv.vx < 0) {
                outUv.vx = 0;
            }
            if (outUv.vx >= 0x140) {
                outUv.vx = 0x13F;
            }
            if (outUv.vy < 0) {
                outUv.vy = 0;
            }
            if (outUv.vy >= 0xF0) {
                outUv.vy = 0xEF;
            }
            *(s32 *)&pts[1][i] = *(s32 *)&outUv;
            if (i != 0) {
                if (inUv.vx < 0x100 && outUv.vx < 0x100 && inUvPrev.vx < 0x100 && outUvPrev.vx < 0x100) {
                    level = tpLo;
                    hiPage = 0;
                } else {
                    level = tpHi;
                    hiPage = 1;
                    inUv.vx -= 0x80;
                    outUv.vx -= 0x80;
                    inUvPrev.vx -= 0x80;
                    outUvPrev.vx -= 0x80;
                }
            }
            if (i < segs && fn != NULL) {
                fn(radius, j, ang, i, &outPos);
            }
            *(s32 *)&pts[0][i] = *(s32 *)&outPos;
            inPos.vx += x;
            inPos.vy += y;
            outPos.vx += x;
            outPos.vy += y;
            if (i != 0) {
                *(s32 *)&prim->u0 = inUv.vx | (inUv.vy << 8);
                *(s32 *)&prim->u1 = outUv.vx | (outUv.vy << 8) | level;
                *(s32 *)&prim->u2 = inUvPrev.vx | (inUvPrev.vy << 8);
                *(s32 *)&prim->u3 = outUvPrev.vx | (outUvPrev.vy << 8);
                *(s32 *)&prim->r2 = *(s32 *)&colIn;
                *(s32 *)&prim->r0 = *(s32 *)&colIn;
                *(s32 *)&prim->r3 = *(s32 *)&colOut;
                *(s32 *)&prim->r1 = *(s32 *)&colOut;
                *(s32 *)&prim->x0 = *(s32 *)&inPos;
                *(s32 *)&prim->x1 = *(s32 *)&outPos;
                *(s32 *)&prim->x2 = *(s32 *)&inPrev;
                *(s32 *)&prim->x3 = *(s32 *)&outPrev;
                if (ot < 0x1000) {
                    addPrimLenIn(&env->ot[ot], prim, 0xC000000);
                }
                prim++;
            }
            if (hiPage) {
                inUv.vx += 0x80;
                outUv.vx += 0x80;
                inUvPrev.vx += 0x80;
                outUvPrev.vx += 0x80;
            }
            *(s32 *)&inPrev = *(s32 *)&inPos;
            *(s32 *)&outPrev = *(s32 *)&outPos;
            *(s32 *)&inUvPrev = *(s32 *)&inUv;
            *(s32 *)&outUvPrev = *(s32 *)&outUv;
        }
    }
    if (src != NULL && ot < 0x1000) {
        addPrimLenIn(&env->ot[ot], move, 0x5000000);
    }
}

static void fx_07d_op80ElemDraw(Pool30Entry *pt, Pool6CEntry *p, SVECTOR *pos, s32 a3, s32 *a4, s32 *a5, s32 a6, s32 frame) {
    BtlfxExports *ex = BTLFX_EXPORTS(fx_07d_env);

    ex->btlfx_drawParticles((u8 *)ex->btlfx_getFileData(fx_07d_task->file, FX_FLARE_ORANGE_GLOW), pos, (SVECTOR *)a3, *a4 / 2, frame, 1, *a5, 0,
                      0);
    BTLFX_COPY_SVEC(&fx_07d_sparkPos[fx_07d_sparkPosIdx], pos);
    fx_07d_sparkPosIdx = (fx_07d_sparkPosIdx + 1) % 24;
}

static void fxm_twistVertex(s32 radius, s32 unused1, s32 ang, s32 unused2, s16 *out) {
    BtlfxExports *ex;
    s32 t;
    s32 x;
    s32 y;

    t = ((0x6C - radius) << 12) / 108;
    t = (t * fx_07d_twist) >> 12;
    radius = (radius * 9) >> 3;
    ex = BTLFX_EXPORTS(fx_07d_env);
    ang += 0x9B;
    ang += t;
    x = (ex->math_cos(ang) * radius) >> 12;
    y = (ex->math_sin(ang) * radius) >> 12;
    out[0] = x;
    out[1] = y;
}

static void fxm_anglesFromEye(s16 *target, s16 *outAng) {
    BTLFX_EXPORTS(fx_07d_env)->btlfx_aimAngles((s16 *)&fx_07d_env->camera->eye, target, outAng);
}

static s32 fx_07d_op80PoolTask(s32 mode, Fx07dOrb *orb, Unk800F78A4 *env, Unk80100C78Entry *entry, s32 frame, s32 pool, Fx07dWork80 *work) {
    BtlfxExports *ex = BTLFX_EXPORTS(env);
    VECTOR pos;
    VECTOR scale;
    SVECTOR rot;
    s32 bright;
    s32 tmp;
    s32 pulse;
    s32 fadeAng;
    s32 t;

    if (mode == 1) {
        orb->unk0 = 0;
        orb->spin14 = ex->btlfx_rand();
        orb->spinSpeed16 = (ex->btlfx_rand() & 7) + 8;
    } else {
        t = (frame << 12) / 12;
        orb->spin14 += orb->spinSpeed16;
        fxm_anglesFromEye((s16 *)&orb->pos4, (s16 *)&rot);
        rot.vz = orb->spin14;
        pos.vx = orb->pos4.vx;
        pos.vy = orb->pos4.vy;
        pos.vz = orb->pos4.vz;
        pulse = ex->math_sin(t >> 2);
        pulse *= 3;
        tmp = orb->scale2;
        scale.vx = scale.vy = scale.vz = ((tmp * pulse) >> 13) + (tmp >> 1);
        if (frame < 3) {
            bright = ((frame + 1) << 12) / 3;
        } else {
            bright = ex->math_cos(((frame - 3) << 10) / 9);
        }
        ex->Hi_DrawEffModel(&rot, &pos, (s32)&scale, work->model28);
        tmp = bright >> 5;
        tmp -= 0x80;
        ex->Hi_ModifyEffModelRGB(work->model28, tmp, tmp, tmp);
        ex->Hi_DrawEffModel(&rot, &pos, (s32)&scale, work->model2C);
        tmp = bright * 2;
        tmp -= 0x1000;
        tmp >>= 5;
        tmp -= 0x80;
        ex->Hi_ModifyEffModelRGB(work->model2C, tmp, tmp, tmp);
        if (frame < 8) {
            orb->rotC.vz += 0xFF;
            scale.vx = scale.vy = scale.vx / 5;
            scale.vz = scale.vx / 7;
            ex->Hi_DrawEffModel(&orb->rotC, &pos, (s32)&scale, work->model20);
            fadeAng = frame << 10;
            fadeAng >>= 3;
            ex->Hi_ModifyEffModelRGB(work->model20, (ex->math_cos(fadeAng) >> 5) - 0x80, (ex->math_cos(fadeAng) >> 5) - 0x80,
                                     (ex->math_cos(fadeAng) >> 5) - 0x80);
        }
        env->otZ = 0x200;
        ex->btlfx_drawParticles((u8 *)ex->btlfx_getFileData(entry->file, FX_FLARE_ORANGE_GLOW), &orb->pos4, NULL, (orb->scale2 * 3) >> 1, 0, 1, bright >> 1,
                          0, 0);
        env->otZ = 0;
        if (frame >= 12) {
            return 1;
        }
    }
    return 0;
}

INCLUDE_RODATA("asm/us/fx_07d/nonmatchings/fx_07d", fx_07d_headerPtr);

s32 fx_07d_op80(s32 mode, Fx07dWork80 *work, Unk800F78A4 *env, Unk80100C78Entry *entry, EffInit *init) {
    BtlfxExports *ex = BTLFX_EXPORTS(env);
    SVECTOR rot0;
    SVECTOR sxy0;
    Pool6CEntry *emitter;
    Fx07dOrb *part;
    s32 frame;
    s32 t;
    s32 alpha;
    s32 ringScale;
    s32 orbSize;
    s32 gridAmp;
    s32 burstAmp;
    s32 orbDist;
    s32 ramp;

    if (mode == 0) {
        init->workSize = 0x40;
        init->elemSize = 0x18;
        init->elemFn = (s32)fx_07d_op80PoolTask;
        init->poolCount = 6;
    } else if (mode == 1) {
        fx_07d_env = env;
        work->state0 = 0;
        work->frame4 = 0;
        fx_07d_task = entry;
        work->otZ8 = ex->btlfx_addRadiusSixth(0, 0);
        ex->btlfx_getUnitBodyPos(0, 0, (s16 *)&work->posC);
        work->unk14 = work->posC.vx;
        work->unk18 = work->posC.vy;
        work->unk1C = work->posC.vz;
        emitter = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, FX_FLARE_WHITE_FLASH), (s32)&work->posC);
        if (emitter != NULL) {
            emitter->otZ = work->otZ8 + 0x46;
        }
        fx_07d_sparkPosIdx = 0;
        work->scroll34 = 0x3E8;
    } else {
        frame = entry->frame;
        switch (work->state0) {
        case 0:
            fx_07d_sparkPosIdx = 0;
            rot0.vy = 0;
            rot0.vx = 0;
            rot0.vz = 0;
            rot0.pad = 0;
            t = (frame << 12) / 50;
            alpha = ex->math_sin(t >> 2);
            env->otZ = work->otZ8 + 0x46;
            ex->btlfx_drawRings((u8 *)ex->btlfx_getFileData(entry->file, FX_FLARE_FIRE_RING), &work->posC, &rot0, t / 3 + 0x400, 0, 1,
                              alpha);
            ex->btlfx_drawRings((u8 *)ex->btlfx_getFileData(entry->file, FX_FLARE_WHITE_RING), &work->posC, &rot0,
                              ex->math_cos(t >> 2) * 5 / 2 + 0x800, 0, 3, alpha);
            env->otZ = 0;
            if (frame == 0x27 || frame == 0x2B) {
                ex->btlfx_drawScreenTint((u32 *)(env->ot + 2), 1, 0x40, 0x40, 0x40);
            }
            fx_07d_twist = t >> 1;
            gridAmp = (t * 0x64) >> 12;
            gte_SetRotMatrix(env->viewMatrix);
            gte_SetTransMatrix(env->viewMatrix);
            gte_ldv0(&work->posC);
            gte_rtps();
            gte_stsxy(&sxy0);
            if (sxy0.vx < 0x78) {
                sxy0.vx = 0x78;
            }
            if (sxy0.vx >= 0xC9) {
                sxy0.vx = 0xC8;
            }
            if (sxy0.vy < 0x69) {
                sxy0.vy = 0x69;
            }
            if (sxy0.vy >= 0x74) {
                sxy0.vy = 0x73;
            }
            fxm_drawRadialScreenGrid(1, sxy0.vx, sxy0.vy, 0x10, 7, 0x11, gridAmp, 0, NULL,
                          (void (*)(s32, s32, s32, s32, DVECTOR *))fxm_twistVertex);
            ex->btlfx_drawBackdrop((BtlBgLayers *)ex->btlfx_getFileData(entry->file, FX_FLARE_BACKDROP_2), work->frame4);
            if (frame == 14) {
                emitter = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, FX_FLARE_CALLBACK_EMITTER), (s32)&work->posC);
                if (emitter != NULL) {
                    emitter->otZ = work->otZ8 + 0x46;
                    emitter->elems->draw = (s32)fx_07d_op80ElemDraw;
                }
            }
            if (frame == 0x31) {
                ex->btlfx_drawScreenTint((u32 *)(env->ot + 1), 2, 0, 0, 0x80);
            }
            if (frame >= 0x32) {
                work->state0 = 1;
                entry->frame = -1;
                ex->btlfx_drawScreenTint((u32 *)(env->ot + 1), 2, 0x80, 0xFF, 0xFF);
                ex->btlfx_initEffModels(fx_07d_effModels, 6);
                work->model20 = ex->Hi_RegisterTexEffModel((BtlModelHeader *)ex->btlfx_getFileData(entry->file, FX_FLARE_ARCH), 0x139,
                                                         0xFFFF, 0, 0x80);
                work->model24 = ex->Hi_RegisterTexEffModel((BtlModelHeader *)ex->btlfx_getFileData(entry->file, FX_FLARE_FIRE_DOME), 0x38,
                                                         0x3E83, 0, 0);
                work->model28 = ex->Hi_RegisterTexEffModel((BtlModelHeader *)ex->btlfx_getFileData(entry->file, FX_FLARE_CLOUD_SPHERE), 0x13C,
                                                         0xFFFF, 0x10, 0x80);
                work->model2C = ex->Hi_RegisterGouEffModel((BtlModelHeader *)ex->btlfx_getFileData(entry->file, FX_FLARE_RING_DISC));
            }
                break;
        case 1:
            switch (frame) {
            default: {
                SVECTOR sxy;
                SVECTOR rot;

                t = (frame << 12) / 10;
                if (frame < 6) {
                    s32 tint = 0xFF - frame * 0xFF / 6;
                    ex->btlfx_drawScreenTint((u32 *)(env->ot + 1), 1, tint, tint, tint >> 2);
                }
                fx_07d_twist = 0x800;
                burstAmp = (t * 0x58) >> 12;
                gte_SetRotMatrix(env->viewMatrix);
                gte_SetTransMatrix(env->viewMatrix);
                gte_ldv0(&work->posC);
                gte_rtps();
                gte_stsxy(&sxy);
                if (sxy.vx < 0x78) {
                    sxy.vx = 0x78;
                }
                if (sxy.vx >= 0xC9) {
                    sxy.vx = 0xC8;
                }
                if (sxy.vy < 0x69) {
                    sxy.vy = 0x69;
                }
                if (sxy.vy >= 0x74) {
                    sxy.vy = 0x73;
                }
                fxm_drawRadialScreenGrid(1, sxy.vx, sxy.vy, 0x10, 7, 0x11, burstAmp, 0, NULL,
                              (void (*)(s32, s32, s32, s32, DVECTOR *))fxm_twistVertex);
                rot.vy = 0;
                rot.vx = 0;
                rot.vz = 0;
                rot.pad = 0;
                alpha = ex->math_sin(t >> 2);
                env->otZ = work->otZ8 + 0x46;
                ex->btlfx_drawRings((u8 *)ex->btlfx_getFileData(entry->file, FX_FLARE_PINK_RING), &work->posC, &rot, 0x1400 - t, 0, 1, alpha);
                ex->btlfx_drawRings((u8 *)ex->btlfx_getFileData(entry->file, FX_FLARE_WHITE_RING), &work->posC, &rot, (0x1000 - t) * 6, 0, 1,
                                  alpha >> 1);
                rot.vx = 0x200;
                rot.vy = 0x200;
                rot.vz = 0;
                rot.pad = 1;
                ex->btlfx_drawRings((u8 *)ex->btlfx_getFileData(entry->file, FX_FLARE_WHITE_RING), &work->posC, &rot, t * 4, 0, 1, alpha);
                rot.vx = 0x200;
                rot.vy = 0x600;
                rot.vz = 0;
                rot.pad = 1;
                ex->btlfx_drawRings((u8 *)ex->btlfx_getFileData(entry->file, FX_FLARE_WHITE_RING), &work->posC, &rot, t * 4, 0, 1, alpha);
                env->otZ = 0;
                if (frame >= 10) {
                    work->state0 = 2;
                    entry->frame = -1;
                    ex->btlfx_drawScreenTint((u32 *)(env->ot + 1), 2, 0xFF, 0x80, 0x80);
                    work->emit3C = emitter = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, FX_FLARE_RED_VIOLET_RINGS), (s32)&work->posC);
                    if (emitter != NULL) {
                        emitter->otZ = work->otZ8 + 0x46;
                    }
                    emitter = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, FX_FLARE_RED_SPARKS), (s32)&work->posC);
                    if (emitter != NULL) {
                        emitter->otZ = work->otZ8 + 0x46;
                    }
                    work->yaw38 = (ex->btlfx_rand() % 3 - 1) * 0x9B;
                }
                break;
            }
            }
            break;
        case 2:
            switch (frame) {
            default: {
                VECTOR scale;
                VECTOR pos;
                SVECTOR rot2;
                SVECTOR rot;
                SVECTOR rot3;

                t = (frame << 12) / 14;
                if (frame < 4) {
                    s32 tint = 0xFF - frame * 0xFF / 6;
                    ex->btlfx_drawScreenTint((u32 *)(env->ot + 1), 1, tint, tint >> 2, tint >> 2);
                }
                ex->btlfx_drawScrollMove(work->scroll34, 0x200, 0x100, 0x20, 0x80, 0x220, 0x100);
                work->scroll34 += 8;
                pos.vx = work->posC.vx;
                pos.vy = work->posC.vy;
                pos.vz = work->posC.vz;
                pos.vx += 0x200;
                scale.vz = 0x400;
                scale.vy = 0x400;
                scale.vx = 0x400;
                fxm_anglesFromEye(&work->posC.vx, &rot2.vx);
                rot2.vx = work->yaw38 + (s16)(rot2.vx + 0x400);
                rot2.vz = 0;
                ex->Hi_DrawEffModel(&rot2, &pos, (s32)&scale, work->model24);
                if (frame == 10) {
                    ex->btlfx_drawScreenTint((u32 *)(env->ot + 2), 1, 0x64, 0x40, 0);
                }
                rot.vy = 0;
                rot.vx = 0;
                rot.vz = 0;
                rot.pad = 0;
                alpha = ex->math_sin(t >> 2);
                ex->btlfx_drawBackdrop((BtlBgLayers *)ex->btlfx_getFileData(entry->file, FX_FLARE_BACKDROP_3), work->frame4);
                env->otZ = work->otZ8 + 0x46;
                ex->btlfx_drawRings((u8 *)ex->btlfx_getFileData(entry->file, FX_FLARE_PINK_RING), &work->posC, &rot, 0x1400 - t, 0, 1, alpha);
                ex->btlfx_drawRings((u8 *)ex->btlfx_getFileData(entry->file, FX_FLARE_TEX_RING), &work->posC, &rot, t * 2 + 0x400, 0, 1, alpha);
                env->otZ = 0;
                if (frame >= 14) {
                    work->state0 = 3;
                    entry->frame = -1;
                }
                break;
            }
            }
            break;
        case 3:
            switch (frame) {
            default: {
                VECTOR scale;
                VECTOR pos;
                SVECTOR rot2;
                SVECTOR rot;
                SVECTOR rot3;

                t = frame << 9;
                if (frame == 0) {
                    ex->btlfx_drawScreenTint((u32 *)(env->ot + 1), 1, 0x80, 0x80, 0x80);
                }
                ex->btlfx_drawScrollMove(work->scroll34, 0x200, 0x100, 0x20, 0x80, 0x220, 0x100);
                work->scroll34 -= 14;
                pos.vx = work->posC.vx;
                pos.vy = work->posC.vy;
                pos.vz = work->posC.vz;
                pos.vx += 0x200;
                scale.vz = 0x400;
                scale.vy = 0x400;
                scale.vx = 0x400;
                fxm_anglesFromEye(&work->posC.vx, &rot2.vx);
                rot2.vx = work->yaw38 + (s16)(rot2.vx + 0x400);
                rot2.vz = 0;
                ex->Hi_DrawEffModel(&rot2, &pos, (s32)&scale, work->model24);
                rot3.vy = 0;
                rot3.vx = 0;
                rot3.vz = 0;
                rot3.pad = 0;
                alpha = ex->math_sin(t >> 2);
                ex->btlfx_drawBackdrop((BtlBgLayers *)ex->btlfx_getFileData(entry->file, FX_FLARE_BACKDROP_3), work->frame4);
                env->otZ = work->otZ8 + 0x46;
                ex->btlfx_drawRings((u8 *)ex->btlfx_getFileData(entry->file, FX_FLARE_PINK_RING), &work->posC, &rot3, (frame << 10) | 0x200, 0,
                                  1, alpha);
                ex->btlfx_drawRings((u8 *)ex->btlfx_getFileData(entry->file, FX_FLARE_TEX_RING), &work->posC, &rot3, (frame << 11) + 0x2400,
                                  0, 1, (alpha >> 1) + 0x800);
                env->otZ = 0;
                if (frame >= 8) {
                    work->state0 = 4;
                    entry->frame = -1;
                    ex->btlfx_stopEmitter(work->emit3C);
                    ex->btlfx_drawScreenTint((u32 *)(env->ot + 1), 2, 0xAA, 0xAA, 0xAA);
                    emitter = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, FX_FLARE_SPARK_GLOW), (s32)&work->posC);
                    if (emitter != NULL) {
                        emitter->otZ = work->otZ8 + 0x46;
                    }
                    emitter = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, FX_FLARE_GOLDEN_GLOWS), (s32)&work->posC);
                    if (emitter != NULL) {
                        emitter->otZ = work->otZ8 + 0x46;
                    }
                }
                break;
            }
            }
            break;
        case 4:
            switch (frame) {
            default: {
                SVECTOR rot;
                SVECTOR rot2;

                t = (frame << 12) / 50;
                if (frame == 0) {
                    ex->btlfx_drawScreenTint((u32 *)(env->ot + 1), 2, 0, 0xFF, 0xFF);
                } else if (frame < 7) {
                    s32 tint = 0xFF - (frame - 1) * 0xFF / 6;
                    ex->btlfx_drawScreenTint((u32 *)(env->ot + 1), 1, tint, tint, tint >> 1);
                }
                ex->btlfx_drawBackdrop((BtlBgLayers *)ex->btlfx_getFileData(entry->file, FX_FLARE_BACKDROP), work->frame4);
                ex->btlfx_drawBackdrop((BtlBgLayers *)ex->btlfx_getFileData(entry->file, FX_FLARE_BACKDROP_2), work->frame4);
                if (frame < 0x20 && !(frame & 1)) {
                    ramp = frame << 7;
                    orbDist = (ramp >> 3) + 0x200;
                    orbSize = 0x1800 - ramp * 3 / 4;
                    part = ex->btlfx_poolSpawn((s16 *)entry->pool);
                    if (part != NULL) {
                        ex->btlfx_randRotation(&rot);
                        if (frame != 0) {
                            ex->btlfx_rotateOffset(&rot, orbDist, (s32)&rot2);
                        } else {
                            rot2.vz = 0;
                            rot2.vy = 0;
                            rot2.vx = 0;
                        }
                        part->pos4.vx = work->posC.vx + rot2.vx;
                        part->pos4.vz = work->posC.vz + rot2.vy;
                        part->pos4.vy = work->posC.vy + rot2.vz;
                        part->scale2 = orbSize;
                        ex->btlfx_randRotation(&part->rotC);
                    }
                }
                rot.vy = 0;
                rot.vx = 0;
                rot.vz = 0;
                rot.pad = 0;
                alpha = ex->math_cos(t >> 2);
                env->otZ = work->otZ8 + 0x46;
                ex->btlfx_drawRings((u8 *)ex->btlfx_getFileData(entry->file, FX_FLARE_FIRE_RING), &work->posC, &rot,
                                  ex->math_cos(t >> 2) / 2 + 0x400, 0, 1, alpha >> 1);
                ex->btlfx_drawBackdrop((BtlBgLayers *)ex->btlfx_getFileData(entry->file, FX_FLARE_BACKDROP_3), work->frame4);
                rot.vx = 0x200;
                rot.vy = 0x590;
                rot.vz = 0x80;
                rot.pad = 1;
                ex->btlfx_drawRings((u8 *)ex->btlfx_getFileData(entry->file, FX_FLARE_TEX_RING_2), &work->posC, &rot, ringScale = t * 3 + 0x800, 0, 1,
                                  alpha);
                rot.vx = 0x600;
                rot.vy = 0x990;
                rot.vz = 0x80;
                rot.pad = 1;
                ex->btlfx_drawRings((u8 *)ex->btlfx_getFileData(entry->file, FX_FLARE_TEX_RING_2), &work->posC, &rot, ringScale, 0, 1, alpha);
                env->otZ = 0;
                if (frame >= 50) {
                    ex->btlfx_clearEffModels();
                    return 1;
                }
                break;
            }
            }
            break;
        }
        work->frame4++;
    }
    return 0;
}

static void fxm_drawOffsetTrail(Unk80100C78Entry *entry, Fx07dStrip *trail, s32 *jitter, SVECTOR *from, SVECTOR *to, s32 amp0, s32 amp1, s32 ang,
                   s32 dang, s32 frame, s32 fade, s32 lineWidth) {
    Unk800F78A4 *env = fx_07d_env;
    BtlfxExports *ex = BTLFX_EXPORTS(env);
    SVECTOR pos;
    SVECTOR rot;
    s32 i;
    s32 t;
    s32 offset;
    SVECTOR *pt;

    ex->btlfx_aimAngles((s16 *)from, (s16 *)to, (s16 *)&rot);
    pt = trail->pts4;
    trail->lastSlot0 = trail->count2 = 14;
    trail->head1 = 0;
    for (i = 0; i < 15; i++) {
        t = 0x1000 - (i << 12) / 14;
        t += *jitter++;
        ex->btlfx_lerpVec((s16 *)from, (s16 *)to, t, (s16 *)&pos);
        offset = *jitter++;
        offset = (ex->math_sin(t >> 1) * offset) >> 12;
        offset += ex->btlfx_lerp(amp0, amp1, t);
        ex->btlfx_offsetPoint(&pos, (s32)pt, 0, (XY32 *)&rot, offset, ang);
        pt->pad = lineWidth;
        ang += dang + *jitter++;
        pt++;
    }
    env->otZ = 0x200;
    ex->btlfx_drawTrail((u8 *)ex->btlfx_getFileData(entry->file, FX_FLARE_BLUE_LIGHTNING), 0, NULL, frame, 0x1000, fade * 2 / 3, 2, (u8 *)trail, 0);
    env->otZ = 0;
}

static s32 fx_07d_op83PoolTask(s32 mode, Fx07dArc *arc, Unk800F78A4 *env, Unk80100C78Entry *entry, s32 frame) {
    if (mode != 1) {
        fxm_drawOffsetTrail(entry, &fx_07d_strips[arc->strip10], arc->jitter14, &arc->from0, &arc->to8, 0, 0x200, frame * 0x9B, 0x186,
                      2, 0x1000, ((0x1000 - (frame << 12) / 5) << 6) >> 12);
        if (frame >= 5) {
            return 1;
        }
    }
    return 0;
}

s32 fx_07d_op83(s32 mode, Fx07dWork83 *work, Unk800F78A4 *env, Unk80100C78Entry *entry, s32 *args) {
    BtlfxExports *ex = BTLFX_EXPORTS(env);
    SVECTOR endPos;
    Fx07dArc *arc;
    Fx07dArc *arc2;
    s32 frame;
    s32 i;
    s32 val;
    s32 *dst;
    s32 *tab;

    if (mode == 0) {
        ((EffInit *)args)->workSize = sizeof(Fx07dWork83);
        ((EffInit *)args)->elemSize = sizeof(Fx07dArc);
        ((EffInit *)args)->elemFn = (s32)fx_07d_op83PoolTask;
        ((EffInit *)args)->poolCount = 7;
    } else if (mode == 1) {
        fx_07d_env = env;
        val = *args;
        ex->btlfx_getUnitBodyPos(0, 0, (s16 *)&work->pos0);
        work->jitter8 = fx_07d_jitter[0];
        dst = work->jitter8;
        for (i = 0; i < 64; i++) {
            *dst++ = ex->btlfx_randCentered(0x100);
            *dst++ = ex->btlfx_randCentered(0x180);
            *dst++ = ex->btlfx_randCentered(0xC0);
        }
        work->variant10 = val;
        work->arcCountC = 0;
    } else {
        frame = entry->frame;
        if (work->variant10 == 0) {
            if (frame >= 0x29) {
                return 1;
            }
            work->arcCountC++;
            arc = ex->btlfx_poolSpawn((s16 *)entry->pool);
            if (arc != NULL) {
                ((s32 *)&arc->from0)[0] = ((s32 *)&fx_07d_sparkPos[ex->btlfx_rand() % 24])[0];
                ((s32 *)&arc->from0)[1] = ((s32 *)&fx_07d_sparkPos[ex->btlfx_rand() % 24])[1];
                ((s32 *)&arc->to8)[0] = ((s32 *)&fx_07d_sparkPos[ex->btlfx_rand() % 24])[0];
                ((s32 *)&arc->to8)[1] = ((s32 *)&fx_07d_sparkPos[ex->btlfx_rand() % 24])[1];
                arc->jitter14 = work->jitter8 + (ex->btlfx_rand() & 0x1F);
                arc->strip10 = work->arcCountC % 7;
            }
        } else {
            if (frame < 0x10) {
                val = ex->btlfx_randCentered(0x300);
                work->arcCountC++;
                val += frame * 0x334;
                arc2 = ex->btlfx_poolSpawn((s16 *)entry->pool);
                if (arc2 != NULL) {
                    ex->btlfx_getUnitBodyPos(0, 0, (s16 *)&endPos);
                    BTLFX_COPY_SVEC(&arc2->from0, &endPos);
                    endPos.vz += ex->math_cos(val) / 2;
                    endPos.vy += ex->math_sin(val) / 2;
                    BTLFX_COPY_SVEC(&arc2->to8, &endPos);
                    arc2->jitter14 = work->jitter8 + (ex->btlfx_rand() & 0x1F);
                    arc2->strip10 = work->arcCountC % 7;
                }
            }
            if (frame >= 0x10) {
                return 1;
            }
        }
    }
    return 0;
}
