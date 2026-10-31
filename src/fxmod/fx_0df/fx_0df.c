#include "fxmod/fx_0df.h"

static void fxm_drawRadialScreenGrid(u32 ot, s32 x, s32 y, s32 n, s32 m, s32 dr, s32 from, s32 to, RECT *src,
                   void (*fn)(s32, s32, s32, s32, DVECTOR *)) {
    DVECTOR pts[2][18];
    DVECTOR a;
    DVECTOR b;
    DVECTOR c;
    DVECTOR d;
    DVECTOR e;
    DVECTOR f;
    DVECTOR g;
    DVECTOR h;
    CVECTOR c0;
    CVECTOR c1;
    Unk800F78A4 *env = fx_0df_env;
    u32 *move = NULL;
    s32 step;
    s32 cx;
    s32 cy;
    s32 tp0;
    s32 tp1;
    s32 flag = 0;
    BtlfxExports *ex;
    POLY_GT4 *prim;
    s32 t;
    s32 ang;
    s32 k;
    DVECTOR *p;
    s32 r;
    s32 i;
    s32 j;
    s32 sx;
    s32 sy;

    prim = (POLY_GT4 *)env->prim;
    env->prim = (u8 *)(prim + n * m);
    t = 0xFF;
    ex = BTLFX_EXPORTS(env);
    if (src == NULL) {
        t = 1;
    }
    c0.cd = t != 0xFF ? 0x3E : 0x3C;
    if (src != NULL) {
        tp0 = getTPage(2, 0, src->x, src->y);
        tp1 = getTPage(2, 0, src->x + 0x80, src->y);
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
        tp0 = getTPage(2, 1, env->dispEnv->disp.x, env->dispEnv->disp.y);
        tp1 = getTPage(2, 1, env->dispEnv->disp.x + 0x80, env->dispEnv->disp.y);
    }
    p = pts[1];
    tp0 <<= 16;
    tp1 <<= 16;
    for (i = 0; i < 17; i++) {
        *(s32 *)&pts[0][i] = 0;
        p[i].vx = cx;
        k = i * 4 + 2;
        *(s16 *)((u8 *)p + k) = cy;
    }
    step = 0x1000 / n;
    r = 0;
    for (j = 1; j <= m; j++) {
        t = ex->btlfx_lerpRatio(from, to, j - 1, m);
        i = 0;
        ang = 0;
        c0.r = c0.g = c0.b = t;
        t = ex->btlfx_lerpRatio(from, to, j, m);
        c1.r = c1.g = c1.b = t;
        r += dr;
        for (; i <= n; i++, ang += step) {
            *(s32 *)&a = *(s32 *)&pts[0][i];
            *(s32 *)&e = *(s32 *)&pts[1][i];
            if (i < n) {
                b.vx = (ex->math_cos(ang) * r) >> 12;
                b.vy = (ex->math_sin(ang) * r) >> 12;
                f.vx = b.vx + cx;
                f.vy = b.vy + cy;
            } else {
                *(s32 *)&b = *(s32 *)&pts[0][0];
                *(s32 *)&f = *(s32 *)&pts[1][0];
            }
            if (f.vx < 0) {
                f.vx = 0;
            }
            if (f.vx >= 0x140) {
                f.vx = 0x13F;
            }
            if (f.vy < 0) {
                f.vy = 0;
            }
            if (f.vy >= 0xF0) {
                f.vy = 0xEF;
            }
            *(s32 *)&pts[1][i] = *(s32 *)&f;
            if (i != 0) {
                if (e.vx < 0x100 && f.vx < 0x100 && g.vx < 0x100 && h.vx < 0x100) {
                    t = tp0;
                    flag = 0;
                } else {
                    t = tp1;
                    flag = 1;
                    e.vx -= 0x80;
                    f.vx -= 0x80;
                    g.vx -= 0x80;
                    h.vx -= 0x80;
                }
            }
            if (i < n && fn != NULL) {
                fn(r, j, ang, i, &b);
            }
            *(s32 *)&pts[0][i] = *(s32 *)&b;
            a.vx += x;
            a.vy += y;
            b.vx += x;
            b.vy += y;
            if (i != 0) {
                *(s32 *)&prim->u0 = e.vx | (e.vy << 8);
                *(s32 *)&prim->u1 = f.vx | (f.vy << 8) | t;
                *(s32 *)&prim->u2 = g.vx | (g.vy << 8);
                *(s32 *)&prim->u3 = h.vx | (h.vy << 8);
                *(s32 *)&prim->r2 = *(s32 *)&c0;
                *(s32 *)&prim->r0 = *(s32 *)&c0;
                *(s32 *)&prim->r3 = *(s32 *)&c1;
                *(s32 *)&prim->r1 = *(s32 *)&c1;
                *(s32 *)&prim->x0 = *(s32 *)&a;
                *(s32 *)&prim->x1 = *(s32 *)&b;
                *(s32 *)&prim->x2 = *(s32 *)&c;
                *(s32 *)&prim->x3 = *(s32 *)&d;
                if (ot < 0x1000) {
                    addPrimLenIn(&env->ot[ot], prim, 0xC000000);
                }
                prim++;
            }
            if (flag) {
                e.vx += 0x80;
                f.vx += 0x80;
                g.vx += 0x80;
                h.vx += 0x80;
            }
            *(s32 *)&c = *(s32 *)&a;
            *(s32 *)&d = *(s32 *)&b;
            *(s32 *)&g = *(s32 *)&e;
            *(s32 *)&h = *(s32 *)&f;
        }
    }
    if (src != NULL && ot < 0x1000) {
        addPrimLenIn(&env->ot[ot], move, 0x5000000);
    }
}

void fx_0df_recordElemDraw(s32 arg0, s32 arg1, SVECTOR *pos, SVECTOR *arg3, s32 *arg4, s32 *arg5, s32 arg6, s32 arg7) {
    BtlfxExports *ex = BTLFX_EXPORTS(fx_0df_env);

    ex->btlfx_drawParticles((u8 *)ex->btlfx_getFileData(fx_0df_task->file, 0x10), pos, arg3, *arg4 / 2, arg7, 1, *arg5, 0, 0);
    BTLFX_COPY_SVEC(&fx_0df_recentPos[fx_0df_recentNext], pos);
    fx_0df_recentNext = (fx_0df_recentNext + 1) % 24;
}

static void fxm_twistVertex(s32 r, s32 a1, s32 a, s32 a3, s16 *out) {
    BtlfxExports *ex;
    s32 t;
    s32 x;
    s32 y;

    t = ((0x6C - r) << 12) / 108;
    t = (t * fx_0df_twist) >> 12;
    r = (r * 9) >> 3;
    ex = BTLFX_EXPORTS(fx_0df_env);
    a += 0x9B;
    a += t;
    x = (ex->math_cos(a) * r) >> 12;
    y = (ex->math_sin(a) * r) >> 12;
    out[0] = x;
    out[1] = y;
}

static void fxm_anglesFromEye(SVECTOR *a, SVECTOR *b) {
    BTLFX_EXPORTS(fx_0df_env)->btlfx_aimAngles((s16 *)&fx_0df_env->camera->eye, (s16 *)a, (s16 *)b);
}

static s32 fx_0df_op80PoolTask(s32 mode, Fx0dfPart *p, Unk800F78A4 *env, Unk80100C78Entry *entry, s32 frame, Unk800DA1B8 *pool,
                  Fx0dfWork *work) {
    s16 half;
    BtlfxExports *ex = BTLFX_EXPORTS(env);
    VECTOR pos;
    VECTOR scale;
    SVECTOR rot;
    s32 t;
    s32 b;
    s32 c;
    s32 r;
    s32 g;
    s32 k;

    if (mode == 1) {
        p->unk0 = 0;
        p->roll14 = ex->btlfx_rand();
        p->rollSpeed16 = (ex->btlfx_rand() & 7) + 8;
    } else {
    t = (frame << 12) / 12;
    p->roll14 += p->rollSpeed16;
    fxm_anglesFromEye(&p->pos4, &rot);
    rot.vz = p->roll14;
    pos.vx = p->pos4.vx;
    pos.vy = p->pos4.vy;
    pos.vz = p->pos4.vz;
    k = ex->math_sin(t >> 2);
    half = p->size2 >> 1;
    k *= 3;
    scale.vx = scale.vy = scale.vz = ((p->size2 * k) >> 13) + half;
    if (frame < 3) {
        b = ((frame + 1) << 12) / 3;
    } else {
        b = ex->math_cos(((frame - 3) << 10) / 9);
    }
    ex->Hi_DrawEffModel(&rot, &pos, (s32)&scale, work->model28);
    ex->Hi_ModifyEffModelRGB(work->model28, (b >> 5) - 0x80, (b >> 5) - 0x80, (b >> 5) - 0x80);
    ex->Hi_DrawEffModel(&rot, &pos, (s32)&scale, work->model2C);
    ex->Hi_ModifyEffModelRGB(work->model2C, ((b * 2 - 0x1000) >> 5) - 0x80, ((b * 2 - 0x1000) >> 5) - 0x80,
                             ((b * 2 - 0x1000) >> 5) - 0x80);
    if (frame < 8) {
        p->rotC.vz += 0xFF;
        scale.vx = scale.vy = scale.vx / 5;
        scale.vz = scale.vx / 7;
        ex->Hi_DrawEffModel(&p->rotC, &pos, (s32)&scale, work->model20);
        c = frame << 10;
        c >>= 3;
        r = (ex->math_cos(c) >> 5) - 0x80;
        g = (ex->math_cos(c) >> 5) - 0x80;
        ex->Hi_ModifyEffModelRGB(work->model20, r, g, (ex->math_cos(c) >> 5) - 0x80);
    }
    env->otZ = 0x200;
    ex->btlfx_drawParticles((u8 *)ex->btlfx_getFileData(entry->file, 0x10), &p->pos4, 0, (p->size2 * 3) >> 1, 0, 1, b >> 1, 0, 0);
    env->otZ = 0;
    if (frame >= 0xC) {
        return 1;
    }
    }
    return 0;
}

s32 fx_0df_op80(s32 mode, Fx0dfWork80 *work, Unk800F78A4 *env, Unk80100C78Entry *entry, EffInit *init) {
    BtlfxExports *ex = BTLFX_EXPORTS(env);
    Pool6CEntry *p;
    Fx0dfPart *part;
    s32 frame;
    s32 t;
    s32 s;
    s32 c;
    s32 k;
    s32 w;
    s32 a;
    s32 b;

    if (mode == 0) {
        init->workSize = 0x40;
        init->elemSize = 0x18;
        init->elemFn = (s32)fx_0df_op80PoolTask;
        init->poolCount = 6;
    } else if (mode == 1) {
        fx_0df_env = env;
        work->state0 = 1;
        work->frame4 = 0;
        fx_0df_task = entry;
        work->otZ8 = ex->btlfx_addRadiusSixth(0, 0);
        ex->btlfx_getUnitBodyPos(0, 0, (s16 *)&work->posC);
        fx_0df_recentNext = 0;
        work->scroll34 = 0x3E8;
        work->unk14 = work->posC.vx;
        work->unk18 = work->posC.vy;
        work->unk1C = work->posC.vz;
        ex->btlfx_drawScreenTint((u32 *)(env->ot + 1), 2, 0x80, 0xFF, 0xFF);
        ex->btlfx_initEffModels(fx_0df_effModels, 6);
        work->model20 = ex->Hi_RegisterTexEffModel((BtlModelHeader *)ex->btlfx_getFileData(entry->file, 0x12), 0x139, 0xFFFF,
                                                 0, 0x80);
        work->model24 = ex->Hi_RegisterTexEffModel((BtlModelHeader *)ex->btlfx_getFileData(entry->file, 5), 0x38, 0x3E83, 0, 0);
        work->model28 = ex->Hi_RegisterTexEffModel((BtlModelHeader *)ex->btlfx_getFileData(entry->file, 0xC), 0x13C, 0xFFFF,
                                                 0x10, 0x80);
        work->model2C = ex->Hi_RegisterGouEffModel((BtlModelHeader *)ex->btlfx_getFileData(entry->file, 0xD));
    } else {
        frame = entry->frame;
        switch (work->state0) {
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
                fx_0df_twist = 0x800;
                w = (t * 0x58) >> 12;
                gte_SetRotMatrix(env->viewMatrix);
                gte_SetTransMatrix(env->viewMatrix);
                gte_ldv0(&work->posC);
                gte_rtps();
                gte_stsxy(&sxy);
                fxm_drawRadialScreenGrid(1, sxy.vx, sxy.vy, 0x10, 7, 0x12, w, 0, NULL,
                              (void (*)(s32, s32, s32, s32, DVECTOR *))fxm_twistVertex);
                rot.vy = 0;
                rot.vx = 0;
                rot.vz = 0;
                rot.pad = 0;
                s = ex->math_sin(t >> 2);
                env->otZ = work->otZ8 + 0x46;
                ex->btlfx_drawRings((u8 *)ex->btlfx_getFileData(entry->file, 3), &work->posC, &rot, 0x1400 - t, 0, 1, s);
                ex->btlfx_drawRings((u8 *)ex->btlfx_getFileData(entry->file, 0x13), &work->posC, &rot, (0x1000 - t) * 6, 0, 1,
                                  s >> 1);
                rot.vx = 0x200;
                rot.vy = 0x200;
                rot.vz = 0;
                rot.pad = 1;
                ex->btlfx_drawRings((u8 *)ex->btlfx_getFileData(entry->file, 0x13), &work->posC, &rot, t * 4, 0, 1, s);
                rot.vx = 0x200;
                rot.vy = 0x600;
                rot.vz = 0;
                rot.pad = 1;
                ex->btlfx_drawRings((u8 *)ex->btlfx_getFileData(entry->file, 0x13), &work->posC, &rot, t * 4, 0, 1, s);
                env->otZ = 0;
                if (frame >= 10) {
                    work->state0 = 2;
                    entry->frame = -1;
                    ex->btlfx_drawScreenTint((u32 *)(env->ot + 1), 2, 0xFF, 0x80, 0x80);
                    work->emit3C = p = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, 8), (s32)&work->posC);
                    if (p != NULL) {
                        p->otZ = work->otZ8 + 0x46;
                    }
                    p = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, 0x11), (s32)&work->posC);
                    if (p != NULL) {
                        p->otZ = work->otZ8 + 0x46;
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
                fxm_anglesFromEye(&work->posC, &rot2);
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
                ex->btlfx_drawBackdrop((BtlBgLayers *)ex->btlfx_getFileData(entry->file, 0x18), work->frame4);
                env->otZ = work->otZ8 + 0x46;
                ex->btlfx_drawRings((u8 *)ex->btlfx_getFileData(entry->file, 3), &work->posC, &rot, 0x1400 - t, 0, 1, s);
                ex->btlfx_drawRings((u8 *)ex->btlfx_getFileData(entry->file, 0x19), &work->posC, &rot, t * 2 + 0x400, 0, 1, s);
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
                fxm_anglesFromEye(&work->posC, &rot2);
                rot2.vx = work->yaw38 + (s16)(rot2.vx + 0x400);
                rot2.vz = 0;
                ex->Hi_DrawEffModel(&rot2, &pos, (s32)&scale, work->model24);
                rot3.vy = 0;
                rot3.vx = 0;
                rot3.vz = 0;
                rot3.pad = 0;
                s = ex->math_sin(t >> 2);
                ex->btlfx_drawBackdrop((BtlBgLayers *)ex->btlfx_getFileData(entry->file, 0x18), work->frame4);
                env->otZ = work->otZ8 + 0x46;
                ex->btlfx_drawRings((u8 *)ex->btlfx_getFileData(entry->file, 3), &work->posC, &rot3, ((frame << 11) + t) / 2 + 0x200, 0,
                                  1, s);
                ex->btlfx_drawRings((u8 *)ex->btlfx_getFileData(entry->file, 0x19), &work->posC, &rot3, (frame << 11) + 0x2400, 0,
                                  1, (s >> 1) + 0x800);
                env->otZ = 0;
                if (frame >= 8) {
                    work->state0 = 4;
                    entry->frame = -1;
                    ex->btlfx_stopEmitter(work->emit3C);
                    ex->btlfx_drawScreenTint((u32 *)(env->ot + 1), 2, 0xAA, 0xAA, 0xAA);
                    p = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, 6), (s32)&work->posC);
                    if (p != NULL) {
                        p->otZ = work->otZ8 + 0x46;
                    }
                    p = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, 9), (s32)&work->posC);
                    if (p != NULL) {
                        p->otZ = work->otZ8 + 0x46;
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
                ex->btlfx_drawBackdrop((BtlBgLayers *)ex->btlfx_getFileData(entry->file, 0xB), work->frame4);
                ex->btlfx_drawBackdrop((BtlBgLayers *)ex->btlfx_getFileData(entry->file, 0xE), work->frame4);
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
                        part->size2 = k;
                        ex->btlfx_randRotation(&part->rotC);
                    }
                }
                rot.vy = 0;
                rot.vx = 0;
                rot.vz = 0;
                rot.pad = 0;
                s = ex->math_cos(t >> 2);
                env->otZ = work->otZ8 + 0x46;
                ex->btlfx_drawRings((u8 *)ex->btlfx_getFileData(entry->file, 4), &work->posC, &rot,
                                  ex->math_cos(t >> 2) / 2 + 0x400, 0, 1, s >> 1);
                ex->btlfx_drawBackdrop((BtlBgLayers *)ex->btlfx_getFileData(entry->file, 0x18), work->frame4);
                rot.vx = 0x200;
                rot.vy = 0x590;
                rot.vz = 0x80;
                rot.pad = 1;
                ex->btlfx_drawRings((u8 *)ex->btlfx_getFileData(entry->file, 0x1A), &work->posC, &rot, c = t * 3 + 0x800, 0, 1,
                                  s);
                rot.vx = 0x600;
                rot.vy = 0x990;
                rot.vz = 0x80;
                rot.pad = 1;
                ex->btlfx_drawRings((u8 *)ex->btlfx_getFileData(entry->file, 0x1A), &work->posC, &rot, c, 0, 1, s);
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

static void fxm_drawOffsetTrail(Unk80100C78Entry *entry, Fx0dfBolt *bolt, s32 *jitter, SVECTOR *from, SVECTOR *to, s32 amp0, s32 amp1,
                   s32 ang, s32 dang, s32 arg9, s32 arg10, s32 pad) {
    Unk800F78A4 *env = fx_0df_env;
    BtlfxExports *ex = BTLFX_EXPORTS(env);
    SVECTOR p;
    SVECTOR rot;
    SVECTOR *pt;
    s32 i;
    s32 t;
    s32 a;

    ex->btlfx_aimAngles((s16 *)from, (s16 *)to, (s16 *)&rot);
    pt = bolt->unk4;
    bolt->count2 = 0xE;
    bolt->lastSlot0 = 0xE;
    bolt->head1 = 0;
    for (i = 0; i < 15; i++) {
        t = 0x1000 - (i << 12) / 14;
        t += *jitter++;
        ex->btlfx_lerpVec((s16 *)from, (s16 *)to, t, (s16 *)&p);
        a = *jitter++;
        a = (ex->math_sin(t >> 1) * a) >> 12;
        a += ex->btlfx_lerp(amp0, amp1, t);
        ex->btlfx_offsetPoint(&p, (s32)pt, 0, (XY32 *)&rot, a, ang);
        pt->pad = pad;
        ang += dang + *jitter++;
        pt++;
    }
    env->otZ = 0x200;
    ex->btlfx_drawTrail((u8 *)ex->btlfx_getFileData(entry->file, 0x1B), 0, 0, arg9, 0x1000, arg10 * 2 / 3, 2, (u8 *)bolt, 0);
    env->otZ = 0;
}

static s32 fx_0df_op83PoolTask(s32 mode, Fx0dfArc *arc, Unk800F78A4 *env, Unk80100C78Entry *entry, s32 frame) {
    if (mode != 1) {
        fxm_drawOffsetTrail(entry, &fx_0df_arcStrips[arc->strip10], arc->jitter14, &arc->pos0, &arc->pos8, 0, 0x200, frame * 0x9B, 0x186,
                      2, 0x1000, ((0x1000 - (frame << 12) / 5) << 6) >> 12);
        if (frame >= 5) {
            return 1;
        }
    }
    return 0;
}

s32 fx_0df_op83(s32 mode, Fx0dfWork83 *work, Unk800F78A4 *env, Unk80100C78Entry *entry, s32 *arg4) {
    BtlfxExports *ex = BTLFX_EXPORTS(env);
    SVECTOR v;
    Fx0dfArc *arc;
    Fx0dfArc *arc2;
    s32 frame;
    s32 i;
    s32 t;
    s32 *p;
    s32 *tab;

    if (mode == 0) {
        ((EffInit *)arg4)->workSize = sizeof(Fx0dfWork83);
        ((EffInit *)arg4)->elemSize = sizeof(Fx0dfArc);
        ((EffInit *)arg4)->elemFn = (s32)fx_0df_op83PoolTask;
        ((EffInit *)arg4)->poolCount = 7;
    } else if (mode == 1) {
        fx_0df_env = env;
        t = *arg4;
        ex->btlfx_getUnitBodyPos(0, 0, (s16 *)&work->pos0);
        work->jitter8 = fx_0df_arcJitter[0];
        p = work->jitter8;
        for (i = 0; i < 64; i++) {
            *p++ = ex->btlfx_randCentered(0x100);
            *p++ = ex->btlfx_randCentered(0x180);
            *p++ = ex->btlfx_randCentered(0xC0);
        }
        work->kind10 = t;
        work->arcCountC = 0;
    } else {
        frame = entry->frame;
        if (work->kind10 == 0) {
            if (frame >= 0x29) {
                return 1;
            }
            work->arcCountC++;
            arc = ex->btlfx_poolSpawn((s16 *)entry->pool);
            if (arc != NULL) {
                ((s32 *)&arc->pos0)[0] = ((s32 *)&fx_0df_recentPos[ex->btlfx_rand() % 24])[0];
                ((s32 *)&arc->pos0)[1] = ((s32 *)&fx_0df_recentPos[ex->btlfx_rand() % 24])[1];
                ((s32 *)&arc->pos8)[0] = ((s32 *)&fx_0df_recentPos[ex->btlfx_rand() % 24])[0];
                ((s32 *)&arc->pos8)[1] = ((s32 *)&fx_0df_recentPos[ex->btlfx_rand() % 24])[1];
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
                    BTLFX_COPY_SVEC(&arc2->pos0, &v);
                    v.vz += ex->math_cos(t) / 2;
                    v.vy += ex->math_sin(t) / 2;
                    BTLFX_COPY_SVEC(&arc2->pos8, &v);
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

s32 fx_0df_op84(s32 mode, Fx0dfWork84 *work, Unk800F78A4 *env, Unk80100C78Entry *entry, s32 *arg4) {
    BtlfxExports *ex = BTLFX_EXPORTS(env);
    SVECTOR rot;
    s32 frame;
    s32 a;
    s32 b;
    s32 c;
    s32 k;
    s32 t;
    s32 d;
    u16 r;

    if (mode == 0) {
        *arg4 = 0x10;
    } else {
        if (mode == 1) {
            fx_0df_env = env;
            work->state0 = 0;
            work->frame4 = 0;
        } else {
            r = c = b = a = 0;
            frame = entry->frame;
            ex->btlfx_getWeaponPos(0x10, 1, (s16 *)&work->pos8);
            switch (work->state0) {
            case 0:
                r = 0x200;
                a = (frame << 12) / 3;
                b = ex->math_sin(a >> 2) >> 1;
                c = ((0x1000 - a) >> 1) + 0x800;
                if (frame >= 3) {
                    work->state0 = 1;
                    entry->frame = -1;
                }
                break;
            case 1:
                k = (frame & 1) << 8;
                c = k + 0x1000;
                d = (frame << 12) / 9;
                a = (k + 0x800) + (d >> 1);
                r = (d >> 3) + 0x200;
                b = c;
                if (frame >= 9) {
                    work->state0 = 2;
                    entry->frame = -1;
                }
                break;
            case 2:
                d = (frame << 12) / 9;
                t = d >> 2;
                r = 0x400 - t;
                a = ex->math_cos(t);
                b = ex->math_sin(t) + 0x1000;
                c = ex->math_sin(t) * 2 + 0x1000;
                if (frame >= 9) {
                    return 1;
                }
                break;
            }
            rot.vx = 0;
            rot.vy = 0;
            rot.vz = r;
            rot.pad = 0;
            env->otZ = 0x80;
            ex->btlfx_drawRings((u8 *)ex->btlfx_getFileData(entry->file, 0x1E), &work->pos8, NULL, b, 0, 1, a / 2);
            ex->btlfx_drawRings((u8 *)ex->btlfx_getFileData(entry->file, 0x1F), &work->pos8, &rot, c * 3 / 2, 0, 1, a * 2 / 3);
            env->otZ = 0;
            work->frame4++;
        }
    }
    return 0;
}

INCLUDE_RODATA("asm/us/fx_0df/nonmatchings/fx_0df", fx_0df_headerPtr);
