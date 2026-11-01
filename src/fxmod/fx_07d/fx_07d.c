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

    ex->btlfx_drawParticles((u8 *)ex->btlfx_getFileData(fx_07d_task->file, 3), pos, (SVECTOR *)a3, *a4 / 2, frame, 1, *a5, 0,
                      0);
    BTLFX_COPY_SVEC(&fx_07d_sparkPos[fx_07d_sparkPosIdx], pos);
    fx_07d_sparkPosIdx = (fx_07d_sparkPosIdx + 1) % 24;
}

static void fxm_twistVertex(s32 r, s32 a1, s32 a, s32 a3, s16 *out) {
    BtlfxExports *ex;
    s32 t;
    s32 x;
    s32 y;

    t = ((0x6C - r) << 12) / 108;
    t = (t * fx_07d_twist) >> 12;
    r = (r * 9) >> 3;
    ex = BTLFX_EXPORTS(fx_07d_env);
    a += 0x9B;
    a += t;
    x = (ex->math_cos(a) * r) >> 12;
    y = (ex->math_sin(a) * r) >> 12;
    out[0] = x;
    out[1] = y;
}

static void fxm_anglesFromEye(s16 *a, s16 *b) {
    BTLFX_EXPORTS(fx_07d_env)->btlfx_aimAngles((s16 *)&fx_07d_env->camera->eye, a, b);
}

static s32 fx_07d_op80PoolTask(s32 mode, Fx07dOrb *p, Unk800F78A4 *env, Unk80100C78Entry *entry, s32 frame, s32 a5, Fx07dWork80 *work) {
    BtlfxExports *ex = BTLFX_EXPORTS(env);
    VECTOR pos;
    VECTOR scale;
    SVECTOR rot;
    s32 c;
    s32 s;
    s32 m;
    s32 a;
    s32 a2;

    if (mode == 1) {
        p->unk0 = 0;
        p->spin14 = ex->btlfx_rand();
        p->spinSpeed16 = (ex->btlfx_rand() & 7) + 8;
    } else {
        a2 = (frame << 12) / 12;
        p->spin14 += p->spinSpeed16;
        fxm_anglesFromEye((s16 *)&p->pos4, (s16 *)&rot);
        rot.vz = p->spin14;
        pos.vx = p->pos4.vx;
        pos.vy = p->pos4.vy;
        pos.vz = p->pos4.vz;
        m = ex->math_sin(a2 >> 2);
        m *= 3;
        s = p->scale2;
        scale.vx = scale.vy = scale.vz = ((s * m) >> 13) + (s >> 1);
        if (frame < 3) {
            c = ((frame + 1) << 12) / 3;
        } else {
            c = ex->math_cos(((frame - 3) << 10) / 9);
        }
        ex->Hi_DrawEffModel(&rot, &pos, (s32)&scale, work->model28);
        s = c >> 5;
        s -= 0x80;
        ex->Hi_ModifyEffModelRGB(work->model28, s, s, s);
        ex->Hi_DrawEffModel(&rot, &pos, (s32)&scale, work->model2C);
        s = c * 2;
        s -= 0x1000;
        s >>= 5;
        s -= 0x80;
        ex->Hi_ModifyEffModelRGB(work->model2C, s, s, s);
        if (frame < 8) {
            p->rotC.vz += 0xFF;
            scale.vx = scale.vy = scale.vx / 5;
            scale.vz = scale.vx / 7;
            ex->Hi_DrawEffModel(&p->rotC, &pos, (s32)&scale, work->model20);
            a = frame << 10;
            a >>= 3;
            ex->Hi_ModifyEffModelRGB(work->model20, (ex->math_cos(a) >> 5) - 0x80, (ex->math_cos(a) >> 5) - 0x80,
                                     (ex->math_cos(a) >> 5) - 0x80);
        }
        env->otZ = 0x200;
        ex->btlfx_drawParticles((u8 *)ex->btlfx_getFileData(entry->file, 3), &p->pos4, NULL, (p->scale2 * 3) >> 1, 0, 1, c >> 1,
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
    s32 s;
    s32 c;
    s32 k;
    s32 q;
    s32 w;
    s32 a;
    s32 b;

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
        emitter = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, 0x14), (s32)&work->posC);
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
            s = ex->math_sin(t >> 2);
            env->otZ = work->otZ8 + 0x46;
            ex->btlfx_drawRings((u8 *)ex->btlfx_getFileData(entry->file, 0x13), &work->posC, &rot0, t / 3 + 0x400, 0, 1,
                              s);
            ex->btlfx_drawRings((u8 *)ex->btlfx_getFileData(entry->file, 9), &work->posC, &rot0,
                              ex->math_cos(t >> 2) * 5 / 2 + 0x800, 0, 3, s);
            env->otZ = 0;
            if (frame == 0x27 || frame == 0x2B) {
                ex->btlfx_drawScreenTint((u32 *)(env->ot + 2), 1, 0x40, 0x40, 0x40);
            }
            fx_07d_twist = t >> 1;
            q = (t * 0x64) >> 12;
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
            fxm_drawRadialScreenGrid(1, sxy0.vx, sxy0.vy, 0x10, 7, 0x11, q, 0, NULL,
                          (void (*)(s32, s32, s32, s32, DVECTOR *))fxm_twistVertex);
            ex->btlfx_drawBackdrop((BtlBgLayers *)ex->btlfx_getFileData(entry->file, 0x15), work->frame4);
            if (frame == 14) {
                emitter = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, 0x12), (s32)&work->posC);
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
                work->model20 = ex->Hi_RegisterTexEffModel((BtlModelHeader *)ex->btlfx_getFileData(entry->file, 2), 0x139,
                                                         0xFFFF, 0, 0x80);
                work->model24 = ex->Hi_RegisterTexEffModel((BtlModelHeader *)ex->btlfx_getFileData(entry->file, 0x16), 0x38,
                                                         0x3E83, 0, 0);
                work->model28 = ex->Hi_RegisterTexEffModel((BtlModelHeader *)ex->btlfx_getFileData(entry->file, 0x1E), 0x13C,
                                                         0xFFFF, 0x10, 0x80);
                work->model2C = ex->Hi_RegisterGouEffModel((BtlModelHeader *)ex->btlfx_getFileData(entry->file, 0xF));
            }
                break;
        case 1:
            switch (frame) {
            default: {
                SVECTOR sxy;
                SVECTOR rot;

                t = (frame << 12) / 10;
                if (frame < 6) {
                    s32 c = 0xFF - frame * 0xFF / 6;
                    ex->btlfx_drawScreenTint((u32 *)(env->ot + 1), 1, c, c, c >> 2);
                }
                fx_07d_twist = 0x800;
                w = (t * 0x58) >> 12;
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
                fxm_drawRadialScreenGrid(1, sxy.vx, sxy.vy, 0x10, 7, 0x11, w, 0, NULL,
                              (void (*)(s32, s32, s32, s32, DVECTOR *))fxm_twistVertex);
                rot.vy = 0;
                rot.vx = 0;
                rot.vz = 0;
                rot.pad = 0;
                s = ex->math_sin(t >> 2);
                env->otZ = work->otZ8 + 0x46;
                ex->btlfx_drawRings((u8 *)ex->btlfx_getFileData(entry->file, 5), &work->posC, &rot, 0x1400 - t, 0, 1, s);
                ex->btlfx_drawRings((u8 *)ex->btlfx_getFileData(entry->file, 9), &work->posC, &rot, (0x1000 - t) * 6, 0, 1,
                                  s >> 1);
                rot.vx = 0x200;
                rot.vy = 0x200;
                rot.vz = 0;
                rot.pad = 1;
                ex->btlfx_drawRings((u8 *)ex->btlfx_getFileData(entry->file, 9), &work->posC, &rot, t * 4, 0, 1, s);
                rot.vx = 0x200;
                rot.vy = 0x600;
                rot.vz = 0;
                rot.pad = 1;
                ex->btlfx_drawRings((u8 *)ex->btlfx_getFileData(entry->file, 9), &work->posC, &rot, t * 4, 0, 1, s);
                env->otZ = 0;
                if (frame >= 10) {
                    work->state0 = 2;
                    entry->frame = -1;
                    ex->btlfx_drawScreenTint((u32 *)(env->ot + 1), 2, 0xFF, 0x80, 0x80);
                    work->emit3C = emitter = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, 1), (s32)&work->posC);
                    if (emitter != NULL) {
                        emitter->otZ = work->otZ8 + 0x46;
                    }
                    emitter = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, 0xB), (s32)&work->posC);
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
                    s32 c = 0xFF - frame * 0xFF / 6;
                    ex->btlfx_drawScreenTint((u32 *)(env->ot + 1), 1, c, c >> 2, c >> 2);
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
                s = ex->math_sin(t >> 2);
                ex->btlfx_drawBackdrop((BtlBgLayers *)ex->btlfx_getFileData(entry->file, 0x17), work->frame4);
                env->otZ = work->otZ8 + 0x46;
                ex->btlfx_drawRings((u8 *)ex->btlfx_getFileData(entry->file, 5), &work->posC, &rot, 0x1400 - t, 0, 1, s);
                ex->btlfx_drawRings((u8 *)ex->btlfx_getFileData(entry->file, 0x18), &work->posC, &rot, t * 2 + 0x400, 0, 1, s);
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
                s = ex->math_sin(t >> 2);
                ex->btlfx_drawBackdrop((BtlBgLayers *)ex->btlfx_getFileData(entry->file, 0x17), work->frame4);
                env->otZ = work->otZ8 + 0x46;
                ex->btlfx_drawRings((u8 *)ex->btlfx_getFileData(entry->file, 5), &work->posC, &rot3, (frame << 10) | 0x200, 0,
                                  1, s);
                ex->btlfx_drawRings((u8 *)ex->btlfx_getFileData(entry->file, 0x18), &work->posC, &rot3, (frame << 11) + 0x2400,
                                  0, 1, (s >> 1) + 0x800);
                env->otZ = 0;
                if (frame >= 8) {
                    work->state0 = 4;
                    entry->frame = -1;
                    ex->btlfx_stopEmitter(work->emit3C);
                    ex->btlfx_drawScreenTint((u32 *)(env->ot + 1), 2, 0xAA, 0xAA, 0xAA);
                    emitter = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, 0x1A), (s32)&work->posC);
                    if (emitter != NULL) {
                        emitter->otZ = work->otZ8 + 0x46;
                    }
                    emitter = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, 0x1B), (s32)&work->posC);
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
                    s32 c = 0xFF - (frame - 1) * 0xFF / 6;
                    ex->btlfx_drawScreenTint((u32 *)(env->ot + 1), 1, c, c, c >> 1);
                }
                ex->btlfx_drawBackdrop((BtlBgLayers *)ex->btlfx_getFileData(entry->file, 0x11), work->frame4);
                ex->btlfx_drawBackdrop((BtlBgLayers *)ex->btlfx_getFileData(entry->file, 0x15), work->frame4);
                if (frame < 0x20 && !(frame & 1)) {
                    b = frame << 7;
                    a = (b >> 3) + 0x200;
                    k = 0x1800 - b * 3 / 4;
                    part = ex->btlfx_poolSpawn((s16 *)entry->pool);
                    if (part != NULL) {
                        ex->btlfx_randRotation(&rot);
                        if (frame != 0) {
                            ex->btlfx_rotateOffset(&rot, a, (s32)&rot2);
                        } else {
                            rot2.vz = 0;
                            rot2.vy = 0;
                            rot2.vx = 0;
                        }
                        part->pos4.vx = work->posC.vx + rot2.vx;
                        part->pos4.vz = work->posC.vz + rot2.vy;
                        part->pos4.vy = work->posC.vy + rot2.vz;
                        part->scale2 = k;
                        ex->btlfx_randRotation(&part->rotC);
                    }
                }
                rot.vy = 0;
                rot.vx = 0;
                rot.vz = 0;
                rot.pad = 0;
                s = ex->math_cos(t >> 2);
                env->otZ = work->otZ8 + 0x46;
                ex->btlfx_drawRings((u8 *)ex->btlfx_getFileData(entry->file, 0x13), &work->posC, &rot,
                                  ex->math_cos(t >> 2) / 2 + 0x400, 0, 1, s >> 1);
                ex->btlfx_drawBackdrop((BtlBgLayers *)ex->btlfx_getFileData(entry->file, 0x17), work->frame4);
                rot.vx = 0x200;
                rot.vy = 0x590;
                rot.vz = 0x80;
                rot.pad = 1;
                ex->btlfx_drawRings((u8 *)ex->btlfx_getFileData(entry->file, 0x19), &work->posC, &rot, c = t * 3 + 0x800, 0, 1,
                                  s);
                rot.vx = 0x600;
                rot.vy = 0x990;
                rot.vz = 0x80;
                rot.pad = 1;
                ex->btlfx_drawRings((u8 *)ex->btlfx_getFileData(entry->file, 0x19), &work->posC, &rot, c, 0, 1, s);
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

static void fxm_drawOffsetTrail(Unk80100C78Entry *entry, Fx07dStrip *buf, s32 *tab, SVECTOR *from, SVECTOR *to, s32 r0, s32 r1, s32 s,
                   s32 step, s32 a9, s32 a10, s32 pad) {
    Unk800F78A4 *env = fx_07d_env;
    BtlfxExports *ex = BTLFX_EXPORTS(env);
    SVECTOR pos;
    SVECTOR ang;
    s32 i;
    s32 t;
    s32 h;
    SVECTOR *v;

    ex->btlfx_aimAngles((s16 *)from, (s16 *)to, (s16 *)&ang);
    v = buf->pts4;
    buf->lastSlot0 = buf->count2 = 14;
    buf->head1 = 0;
    for (i = 0; i < 15; i++) {
        t = 0x1000 - (i << 12) / 14;
        t += *tab++;
        ex->btlfx_lerpVec((s16 *)from, (s16 *)to, t, (s16 *)&pos);
        h = *tab++;
        h = (ex->math_sin(t >> 1) * h) >> 12;
        h += ex->btlfx_lerp(r0, r1, t);
        ex->btlfx_offsetPoint(&pos, (s32)v, 0, (XY32 *)&ang, h, s);
        v->pad = pad;
        s += step + *tab++;
        v++;
    }
    env->otZ = 0x200;
    ex->btlfx_drawTrail((u8 *)ex->btlfx_getFileData(entry->file, 8), 0, NULL, a9, 0x1000, a10 * 2 / 3, 2, (u8 *)buf, 0);
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
    SVECTOR v;
    Fx07dArc *arc;
    Fx07dArc *arc2;
    s32 frame;
    s32 i;
    s32 t;
    s32 *p;
    s32 *tab;

    if (mode == 0) {
        ((EffInit *)args)->workSize = sizeof(Fx07dWork83);
        ((EffInit *)args)->elemSize = sizeof(Fx07dArc);
        ((EffInit *)args)->elemFn = (s32)fx_07d_op83PoolTask;
        ((EffInit *)args)->poolCount = 7;
    } else if (mode == 1) {
        fx_07d_env = env;
        t = *args;
        ex->btlfx_getUnitBodyPos(0, 0, (s16 *)&work->pos0);
        work->jitter8 = fx_07d_jitter[0];
        p = work->jitter8;
        for (i = 0; i < 64; i++) {
            *p++ = ex->btlfx_randCentered(0x100);
            *p++ = ex->btlfx_randCentered(0x180);
            *p++ = ex->btlfx_randCentered(0xC0);
        }
        work->variant10 = t;
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
                t = ex->btlfx_randCentered(0x300);
                work->arcCountC++;
                t += frame * 0x334;
                arc2 = ex->btlfx_poolSpawn((s16 *)entry->pool);
                if (arc2 != NULL) {
                    ex->btlfx_getUnitBodyPos(0, 0, (s16 *)&v);
                    BTLFX_COPY_SVEC(&arc2->from0, &v);
                    v.vz += ex->math_cos(t) / 2;
                    v.vy += ex->math_sin(t) / 2;
                    BTLFX_COPY_SVEC(&arc2->to8, &v);
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
