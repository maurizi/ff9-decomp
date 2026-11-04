#include "fxmod/fx_07e.h"

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
    Unk800F78A4 *env = fx_07e_env;
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

static s32 fx_07e_op80PoolTask(s32 mode, Fx07ePart *part, Unk800F78A4 *env, Unk80100C78Entry *entry, s32 frame, s32 pool,
                  Fx07eWork80 *work) {
    BtlfxExports *ex = BTLFX_EXPORTS(env);
    SVECTOR out;
    SVECTOR rot;
    SVECTOR ang;
    VECTOR pos;
    VECTOR scale;
    s32 tmp;
    s32 partAng;
    s32 fade;
    s32 tint;
    s32 rotXY;
    s32 rotZPad;
    s32 (*fn)(SVECTOR *, VECTOR *, s32, s32);

    if (mode == 1) {
        part->state4 = 0;
        return 0;
    }
    tmp = work->ringRot14.vz;
    partAng = part->angle0;
    rotXY = ((s32 *)&work->ringRot14)[0];
    rotZPad = ((s32 *)&work->ringRot14)[1];
    ((XY32 *)&ang)[0].w = rotXY;
    ((XY32 *)&ang)[1].w = rotZPad;
    tmp += partAng;
    ex->btlfx_offsetPoint(&work->posC, (s32)&out, 0, (XY32 *)&ang, work->radius1C, tmp);
    pos.vx = out.vx;
    pos.vy = out.vy;
    pos.vz = out.vz;
    scale.vx = scale.vy = scale.vz = work->scale20;
    BTLFX_COPY_SVEC(&rot, &ang);
    tmp += 0x400;
    rot.vz = tmp;
    switch (part->state4) {
    case 0:
        tmp = frame << 9;
        ex->Hi_DrawEffModel(&rot, &pos, (s32)&scale, work->model28);
        ex->Hi_ModifyEffModelAbr(work->model28, 1);
        ex->Hi_ModifyEffModelRGB(work->model28, (tmp >> 5) - 0x80, (tmp >> 5) - 0x80, (tmp >> 5) - 0x80);
        ex->btlfx_drawParticles((u8 *)ex->btlfx_getFileData(entry->file, FX_SLOW_PARTICLES), &out, NULL, 0x1400, frame, 1, 0x1000, 0, 0);
        ex->btlfx_drawParticles((u8 *)ex->btlfx_getFileData(entry->file, FX_SLOW_BLUE_WAVE), &out, NULL, 0x1400, frame, 1, 0x1000, 0, 0);
        if (frame >= 8) {
            part->state4 = 1;
            ((s16 *)part)[-1] = -1;
        }
        break;
    case 1:
        ex->Hi_DrawEffModel(&rot, &pos, (s32)&scale, work->model28);
        ex->Hi_ModifyEffModelAbr(work->model28, 1);
        ex->Hi_ModifyEffModelRGB(work->model28, 0, 0, 0);
        if (work->shrinkDone24 != 0) {
            part->state4 = 2;
            ((s16 *)part)[-1] = -1;
        }
        break;
    case 2:
        tmp = frame << 8;
        fade = ex->math_cos(tmp >> 2);
        ex->Hi_DrawEffModel(&rot, &pos, (s32)&scale, work->model28);
        ex->Hi_ModifyEffModelAbr(work->model28, 1);
        tint = (fade >> 5) - 0x80;
        ex->Hi_ModifyEffModelRGB(work->model28, tint, tint, tint);
        if (frame >= 16) {
            return 1;
        }
        break;
    }
    return 0;
}

s32 fx_07e_op80(s32 mode, Fx07eWork80 *work, Unk800F78A4 *env, Unk80100C78Entry *entry, s32 *args) {
    BtlfxExports *ex = BTLFX_EXPORTS(env);
    Fx07ePart *part;
    s32 frame;
    s32 t;
    s32 radius;

    if (mode == 0) {
        ((EffInit *)args)->workSize = sizeof(Fx07eWork80);
        ((EffInit *)args)->elemSize = sizeof(Fx07ePart);
        ((EffInit *)args)->elemFn = (s32)fx_07e_op80PoolTask;
        ((EffInit *)args)->poolCount = 0xB;
    } else if (mode == 1) {
        fx_07e_env = env;
        args = (s32 *)*args;
        ex->btlfx_getUnitBodyPos(0, 0, (s16 *)&work->posC);
        work->radius1C = 0x2EE;
        work->spawnCount4 = 0;
        work->state0 = 0;
        work->frame8 = 0;
        work->shrinkDone24 = 0;
        work->scale20 = 0x8000;
        if (args != NULL) {
            work->ringRot14.vx = 0x200;
        } else {
            work->ringRot14.vx = -0x200;
        }
        do {
            work->ringRot14.vy = 0;
        } while (0);
        work->ringRot14.vz = 0;
        ex->btlfx_initEffModels(fx_07e_effModels, 1);
        work->model28 = ex->Hi_RegisterTexEffModel((BtlModelHeader *)ex->btlfx_getFileData(entry->file, FX_SLOW_CRESCENT), 0xBB, 0x3E40, 0,
                                                 0x80);
    } else {
        frame = entry->frame;
        work->ringRot14.vz += 0x18;
        work->ringRot14.vy = work->frame8 * 0x14;
        work->ringRot14.vx -= 0xC;
        switch (work->state0) {
        case 0:
            t = (work->spawnCount4 << 12) / 11;
            part = ex->btlfx_poolSpawn((s16 *)entry->pool);
            if (part != NULL) {
                part->index8 = work->spawnCount4;
                part->angle0 = t;
            }
            if (++work->spawnCount4 >= 11) {
                work->state0 = 1;
                entry->frame = -1;
            }
            break;
        case 1:
            t = (frame << 12) / 22;
            radius = 0x2EE - ((t * 0x2CE) >> 12);
            work->radius1C = radius;
            work->scale20 = ((((radius << 12) / 750) << 15) >> 12);
            if (frame >= 22) {
                work->state0 = 2;
                entry->frame = -1;
                work->shrinkDone24 = 1;
            }
            break;
        case 2:
            if (frame >= 17) {
                ex->btlfx_clearEffModels();
                return 1;
            }
            break;
        }
        work->frame8++;
    }
    return 0;
}

static void fx_07e_op81GridVertex(s32 radius, s32 unused1, s32 ang, s32 unused3, s16 *out) {
    BtlfxExports *ex = BTLFX_EXPORTS(fx_07e_env);
    s32 x;
    s32 y;
    s32 twist;

    twist = ((0x3C - radius) << 12) / 60;
    twist = (twist * fx_07e_twist) >> 12;
    ang += twist;
    x = (ex->math_cos(ang) * radius) >> 12;
    y = (ex->math_sin(ang) * radius) >> 12;
    out[0] = x;
    out[1] = y;
}

s32 fx_07e_op81(s32 mode, Fx07eWork81 *work, Unk800F78A4 *env, Unk80100C78Entry *entry, s32 *args) {
    BtlfxExports *ex = BTLFX_EXPORTS(env);
    DVECTOR xy;
    RECT grabRect;
    RECT fadeRect;
    SVECTOR *pos;
    s32 frame;
    s32 t;
    s32 ang;
    s32 alpha;
    s32 size;

    if (mode == 0) {
        *args = sizeof(Fx07eWork81);
    } else if (mode == 1) {
        fx_07e_env = env;
        ex->btlfx_getUnitBodyPos(0, 0, (s16 *)&work->posC);
        work->otZ8 = ex->btlfx_addRadiusSixth(0, 0x20);
        work->state0 = 0;
    } else {
        frame = entry->frame;
        gte_SetRotMatrix(env->viewMatrix);
        gte_SetTransMatrix(env->viewMatrix);
        pos = &work->posC;
        gte_ldv0(pos);
        gte_rtps();
        gte_stsxy(&xy);
        switch (work->state0) {
        case 0:
            t = (frame << 12) / 12;
            env->otZ = work->otZ8;
            alpha = ex->math_sin(t >> 2);
            ex->btlfx_drawParticles((u8 *)ex->btlfx_getFileData(entry->file, FX_SLOW_GLINT), pos, NULL, 0x2000, 0, 1, alpha, 0, 0);
            ex->btlfx_drawRings((u8 *)ex->btlfx_getFileData(entry->file, FX_SLOW_GREEN_RING), pos, NULL, 0x2000, 0, 2, alpha);
            env->otZ = 0;
            if (frame >= 12) {
                work->state0 = 1;
                entry->frame = -1;
            }
            break;
        case 1:
            grabRect.x = VRAM_BTLFX_SCRATCH_X;
            grabRect.y = VRAM_BTLFX_SCRATCH_Y + 0x80;
            grabRect.w = 0x7A;
            grabRect.h = 0x7A;
            t = frame << 8;
            fxm_drawRadialScreenGrid(1, xy.vx, xy.vy, 0x10, 6, 0xA, 0x80, 0x80, &grabRect, fx_07e_op81GridVertex);
            fx_07e_twist = ex->math_sin(t >> 2) >> 1;
            env->otZ = work->otZ8;
            ex->btlfx_drawParticles((u8 *)ex->btlfx_getFileData(entry->file, FX_SLOW_GLINT), pos, NULL, 0x2000, 0, 1, 0x1000, 0, 0);
            ex->btlfx_drawRings((u8 *)ex->btlfx_getFileData(entry->file, FX_SLOW_GREEN_RING), pos, NULL, 0x2000, 0, 2, 0x1000);
            env->otZ = 0;
            if (frame >= 16) {
                work->state0 = 2;
                entry->frame = -1;
            }
            break;
        case 2:
            fadeRect.x = VRAM_BTLFX_SCRATCH_X;
            fadeRect.y = VRAM_BTLFX_SCRATCH_Y + 0x80;
            fadeRect.w = 0x7A;
            fadeRect.h = 0x7A;
            t = (frame << 12) / 12;
            fxm_drawRadialScreenGrid(1, xy.vx, xy.vy, 0x10, 6, 0xA, 0x80, 0x80, &fadeRect, fx_07e_op81GridVertex);
            ang = t >> 2;
            fx_07e_twist = ex->math_cos(ang) >> 1;
            env->otZ = work->otZ8;
            size = 0x1000 - ex->math_sin(ang);
            alpha = ex->math_cos(ang);
            ex->btlfx_drawParticles((u8 *)ex->btlfx_getFileData(entry->file, FX_SLOW_GLINT), pos, NULL, size * 2, 0, 1, alpha, 0, 0);
            ex->btlfx_drawRings((u8 *)ex->btlfx_getFileData(entry->file, FX_SLOW_GREEN_RING), pos, NULL, size * 2, 0, 2, alpha);
            env->otZ = 0;
            if (frame >= 12) {
                return 1;
            }
            break;
        }
        ex->btlfx_drawLayerTexture((BtlBgLayers *)ex->btlfx_getFileData(entry->file, FX_SLOW_BACKDROP), work->frame4);
        work->frame4++;
    }
    return 0;
}

INCLUDE_RODATA("asm/us/fx_07e/nonmatchings/fx_07e", fx_07e_headerPtr);
