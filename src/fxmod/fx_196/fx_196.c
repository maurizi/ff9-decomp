#include "fxmod/fx_196.h"

static void fxm_keyGetVec(Fx196Track *track, s32 t, SVECTOR *out) {
    if (t >= track->count2) {
        t = track->count2 - 1;
    }
    track = (Fx196Track *)&track->keys4[t * 3];
    out->vx = ((s16 *)track)[0];
    out->vy = ((s16 *)track)[1];
    out->vz = ((s16 *)track)[2];
}

static void fxm_keyGetVec2(Fx196Track *track, s32 t, SVECTOR *a, SVECTOR *b) {
    s16 *k;

    if (t >= track->count2) {
        t = track->count2 - 1;
    }
    k = &track->keys4[t * 6];
    if (a != NULL) {
        a->vx = k[0];
        a->vy = k[1];
        a->vz = k[2];
    }
    if (b != NULL) {
        b->vx = k[3];
        b->vy = k[4];
        b->vz = k[5];
    }
}

static void fxm_keyGetVec3(Fx196Track *track, s32 t, SVECTOR *a, SVECTOR *b, SVECTOR *c) {
    s16 *k;

    if (t >= track->count2) {
        t = track->count2 - 1;
    }
    k = &track->keys4[t * 9];
    if (a != NULL) {
        a->vx = k[0];
        a->vy = k[1];
        a->vz = k[2];
    }
    if (b != NULL) {
        b->vx = k[3];
        b->vy = k[4];
        b->vz = k[5];
    }
    if (c != NULL) {
        c->vx = k[6];
        c->vy = k[7];
        c->vz = k[8];
    }
}

static s32 fxm_keyGetValue(Fx196Track *track, s32 t) {
    if (t >= track->count2) {
        t = track->count2 - 1;
    }
    return (track->keys4 + t)[0];
}

static void fxm_drawScrollMoveX(s32 x, s32 dx, s32 dy, s32 w, s32 h, s32 sx, s32 sy) {
    s32 wrapW = x % w;
    Unk800F78A4 *env = fx_196_env;
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

s32 fxm_sinHalfNeg(s32 a) {
    s32 v = BTLFX_EXPORTS(fx_196_env)->math_sin(a);

    if (v < 0) {
        v >>= 1;
    }
    return v;
}

void fx_196_drawWaveMesh(SVECTOR *pos, SVECTOR *rot, VECTOR *scale, s32 col, s32 col2, s32 tpage, s32 clut, s32 u, s32 v,
                   s32 amp, s32 ph, s32 step, s32 r, s32 ang, s32 d) {
    Unk800F78A4 *env = fx_196_env;
    BtlfxExports *ex = BTLFX_EXPORTS(env);
    CVECTOR c0;
    CVECTOR c1;
    CVECTOR c2;
    CVECTOR c3;
    CVECTOR c4;
    CVECTOR c5;
    MATRIX m;
    MATRIX m2;
    SVECTOR vs[4];
    VECTOR lv;
    DVECTOR sxy;
    s32 otz;
    s32 z;
    POLY_GT4 *gt;
    POLY_FT4 *ft;
    POLY_G4 *g;
    POLY_F4 *f;
    s32 abr;
    s32 xb;
    s32 yb;
    s32 xo;
    s32 w;
    s32 i;
    s32 y0;
    s32 y1;
    s32 j;
    s32 x;
    s32 t;
    s32 u0;
    s32 u1;
    s32 v0;
    s32 v1;
    s32 isft;

    abr = 1;
    gte_SetRotMatrix(env->viewMatrix);
    gte_SetTransMatrix(env->viewMatrix);
    gte_ldv0(pos);
    gt = (POLY_GT4 *)env->prim;
    *(s32 *)&c2 = 0;
    *(s32 *)&c5 = 0;
    ft = (POLY_FT4 *)(gt + 18);
    g = (POLY_G4 *)(ft + 324);
    f = (POLY_F4 *)(g + 18);
    env->prim = (u8 *)(f + 324);
    c0.r = c0.g = c0.b = col;
    c1.r = c1.g = c1.b = col;
    c3.r = c3.g = c3.b = col2;
    c4.r = c4.g = c4.b = col2;
    c0.cd = abr != 0xFF ? 0x2E : 0x2C;
    c2.cd = abr != 0xFF ? 0x3E : 0x3C;
    c1.cd = c2.cd;
    c3.cd = abr != 0xFF ? 0x2A : 0x28;
    c5.cd = abr != 0xFF ? 0x3A : 0x38;
    tpage |= (abr & 3) << 5;
    c4.cd = c5.cd;
    gte_rtv0tr_b();
    ex->math_rotMatrixYXZ(rot, &m);
    ex->ScaleMatrix(&m, scale);
    m2 = m;
    m2.t[0] = pos->vx;
    m2.t[1] = pos->vy;
    m2.t[2] = pos->vz;
    gte_stlvnl(m.t);
    ex->MulRotMatrix(&m);
    gte_SetRotMatrix(&m);
    gte_SetTransMatrix(&m);
    xb = -0x24EA;
    yb = -0x17CD;
    for (w = 0, i = 0; i < 18; i++, w += 0x2A5) {
        j = 0;
        ph += step;
        y0 = yb + w;
        y1 = y0 + 0x2A5;
        for (xo = 0; j < 19; j++, xo += 0x41A) {
            x = xb + xo;
            vs[0].vx = x + ((ex->math_sin(ph) * amp) >> 12);
            vs[1].vx = vs[0].vx + 0x41A;
            t = ex->math_sin(ph + step);
            vs[0].vy = vs[1].vy = y0;
            vs[2].vy = vs[3].vy = y1;
            x += (t * amp) >> 12;
            vs[2].vx = x;
            vs[3].vx = x + 0x41A;
            vs[0].vz = vs[2].vz = (fxm_sinHalfNeg(ang + j * d) * r) >> 12;
            vs[1].vz = vs[3].vz = (fxm_sinHalfNeg(ang + (j + 1) * d) * r) >> 12;
            gte_ldv3c(vs);
            gte_rtpt();
            u0 = u + ((j & 3) << 5);
            u1 = u0 + 0x20;
            if ((j & 3) == 3) {
                u1 = u0 + 0x1F;
            }
            v0 = v + ((i & 1) << 6);
            v1 = v0 + 0x40;
            if (i & 1) {
                v1 = v0 + 0x3F;
            }
            gte_nclip();
            gte_stopz(&otz);
            if (otz != 0) {
                gte_avsz3();
                gte_stszotz(&z);
                isft = 1;
                if (j == 0) {
                    *(s32 *)&gt->r0 = *(s32 *)&gt->r2 = *(s32 *)&c2;
                    *(s32 *)&gt->r1 = *(s32 *)&gt->r3 = *(s32 *)&c1;
                    *(s32 *)&g->r0 = *(s32 *)&g->r2 = *(s32 *)&c5;
                    *(s32 *)&g->r1 = *(s32 *)&g->r3 = *(s32 *)&c4;
                    isft = 0;
                } else {
                    *(s32 *)&ft->r0 = *(s32 *)&c0;
                    *(s32 *)&f->r0 = *(s32 *)&c3;
                }
                if (otz > 0) {
                    if (isft) {
                        ft->r0 >>= 1;
                        ft->g0 >>= 1;
                    } else {
                        gt->r0 >>= 1;
                        gt->g0 >>= 1;
                        gt->r1 >>= 1;
                        gt->g1 >>= 1;
                        gt->r2 >>= 1;
                        gt->g2 >>= 1;
                        gt->r3 >>= 1;
                        gt->g3 >>= 1;
                    }
                }
                if (isft) {
                    ft->u0 = ft->u2 = u0;
                    ft->u1 = ft->u3 = u1;
                    ft->v0 = ft->v1 = v0;
                    ft->v2 = ft->v3 = v1;
                    ft->tpage = tpage;
                    ft->clut = clut;
                    gte_stsxy3(&ft->x0, &ft->x1, &ft->x2);
                    *(s32 *)&f->x0 = *(s32 *)&ft->x0;
                    *(s32 *)&f->x1 = *(s32 *)&ft->x1;
                    *(s32 *)&f->x2 = *(s32 *)&ft->x2;
                } else {
                    gt->u0 = gt->u2 = u0;
                    gt->u1 = gt->u3 = u1;
                    gt->v0 = gt->v1 = v0;
                    gt->v2 = gt->v3 = v1;
                    gt->tpage = tpage;
                    gt->clut = clut;
                    gte_stsxy3(&gt->x0, &gt->x1, &gt->x2);
                    *(s32 *)&g->x0 = *(s32 *)&gt->x0;
                    *(s32 *)&g->x1 = *(s32 *)&gt->x1;
                    *(s32 *)&g->x2 = *(s32 *)&gt->x2;
                    gte_SetRotMatrix(&m2);
                    gte_SetTransMatrix(&m2);
                    gte_ldv0(vs);
                    gte_rtv0tr();
                    gte_stlvnl(&lv);
                    fx_196_wavePoints[i].vx = lv.vx;
                    fx_196_wavePoints[i].vy = lv.vy;
                    fx_196_wavePoints[i].vz = lv.vz;
                    gte_SetRotMatrix(&m);
                    gte_SetTransMatrix(&m);
                    gte_ldv0(vs);
                    gte_rtps();
                    gte_stsxy(&sxy);
                    if ((u16)(sxy.vx + 0x20) > 0x180 || sxy.vy < -0x20 || sxy.vy > 0x110) {
                        fx_196_wavePoints[i].pad = 0;
                    } else {
                        fx_196_wavePoints[i].pad = 1;
                    }
                }
                gte_ldv0(&vs[3]);
                gte_rtps();
                if (isft) {
                    ex->btlfx_addPrim(0x09000000, z, ft);
                    ex->btlfx_addPrimAbr(0x05000000, z, f, 2);
                    gte_stsxy(&ft->x3);
                    *(s32 *)&f->x3 = *(s32 *)&ft->x3;
                    ft++;
                    f++;
                } else {
                    ex->btlfx_addPrim(0x0C000000, z, gt);
                    ex->btlfx_addPrimAbr(0x08000000, z, g, 2);
                    gte_stsxy(&gt->x3);
                    *(s32 *)&g->x3 = *(s32 *)&gt->x3;
                    gt++;
                    g++;
                }
            }
        }
    }
}

static s32 fxm_nextRingPoint(SVECTOR *v) {
    s32 i = fx_196_nextWavePoint;

    v->pad = 1;
    v->vx = fx_196_wavePoints[i].vx;
    v->vy = fx_196_wavePoints[i].vy;
    v->vz = fx_196_wavePoints[i].vz;
    i++;
    if (i >= 16) {
        i = 0;
    }
    fx_196_nextWavePoint = i;
    return 0;
}

s32 fx_196_op85(s32 mode, Fx196Work85 *work, Unk800F78A4 *env, Unk80100C78Entry *entry, s32 *args) {
    BtlfxExports *ex = BTLFX_EXPORTS(env);
    s32 frame;
    s32 t;
    SVECTOR pos;
    SVECTOR rot;
    VECTOR sc;
    SVECTOR v0; /* also the light colour (as a CVECTOR) */
    SVECTOR v1;
    VECTOR sc2;

    if (mode == 0) {
        *args = 0x4C;
    } else if (mode == 1) {
        fx_196_env = env;
        work->state0 = 0;
        work->frame4 = 0;
    } else {
        frame = entry->frame;
        switch (work->state0) {
        case 0: {
            s32 s;
            s32 r;
            s32 m;
            s32 q;
            s32 a;
            s32 b;
            s32 i;
            s32 c;

            t = (frame << 12) / 81;
            if (frame == 0x12) {
                work->emit48 = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, 5), (s32)&work->pos10);
                if (work->emit48 != NULL) {
                    work->emit48->tail->shape = -1;
                    work->emit48->tail->s.module.spawnFn = (s32 (*)(SVECTOR *, SVECTOR *, s32, s32))fxm_nextRingPoint;
                }
                if (work->emit48 != NULL) {
                    work->emit48->otZ = 0x200;
                }
            }
            if (frame >= 9) {
                if (frame < 0x3D) {
                    s = ((frame - 9) << 12) / 52;
                } else {
                    s = 0x1000;
                }
                if (frame < 0x14) {
                    pos.vx = 0;
                    m = 1;
                } else {
                    pos.vx = 0x1000 - (t >> 1);
                    m = 2;
                }
                q = s >> 2;
                c = ex->math_sin(t >> 2);
                pos.vy = ((0x1000 - c) >> 2) - (0x400 + ((ex->math_cos(q) * 0x44C) >> 12));
                pos.vz = ((0x1000 - ex->math_cos(q)) << 2) - 0x4000;
                rot.vx = 0x400;
                rot.vy = 0x400;
                rot.vz = 0;
                sc.vx = sc.vy = 0x1000;
                sc.vz = ex->math_cos(q);
                if (frame < 0x10) {
                    r = ex->math_sin((frame << 10) >> 4) + 0x800;
                } else {
                    r = 0x1800;
                }
                a = 0xC8;
                if ((u32)(frame - 0x28) < 0x20) {
                    a = 0xC8 - (((frame - 0x28) * 0x96) >> 5);
                } else if (frame >= 0x48) {
                    a = 0x32;
                }
                if (frame < 0x3D) {
                    b = 0x80;
                } else {
                    b = 0x80 - (((frame - 0x3D) << 5) / 20);
                }
                if (frame < 0x29) {
                    env->otZ = 0;
                } else {
                    env->otZ = -0x80;
                }
                fx_196_drawWaveMesh(&pos, &rot, &sc, b, a, 0x17, 0x3D04, 0, 0x80, 0x7D0, s * m, -0x140, r,
                              ex->math_cos(s >> 2) + 0x400, 0x100);
                env->otZ = 0x7C;
                for (i = 0; i < 18; i++) {
                    if (fx_196_wavePoints[i].pad != 0) {
                        b = (i & 1) << 12;
                        ex->btlfx_drawAnimQuads((u8 *)ex->btlfx_getFileData(entry->file, 6), &fx_196_wavePoints[i], NULL,
                                                ((t * 3) >> 3) + 0x3800 + b, frame + i * 2, 1, 0x1000);
                    }
                }
                env->otZ = 0x37;
                if (frame >= 0x1D) {
                    for (i = 0; i < 7; i++) {
                        if (fx_196_wavePoints[i].pad != 0) {
                            ex->btlfx_drawAnimQuads((u8 *)ex->btlfx_getFileData(entry->file, 6), &fx_196_wavePoints[i], NULL,
                                                    0x2400, frame + i * 2 + 4, 1, 0x1000);
                        }
                    }
                }
                env->otZ = 0;
            }
            if (frame == 0x32) {
                Pool6CEntry *emitter;

                v0.vx = env->camera->points[0].vx;
                v0.vy = env->camera->points[0].vy;
                v0.vz = env->camera->points[0].vz;
                v1.vx = env->camera->points[1].vx;
                v1.vy = env->camera->points[1].vy;
                v1.vz = env->camera->points[1].vz;
                work->pos10.vx = (v0.vx + v1.vx) / 2;
                work->pos10.vy = (v0.vy + v1.vy) / 2;
                work->pos10.vz = (v0.vz + v1.vz) / 2;
                emitter = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, 7), (s32)&work->pos10);
                if (emitter != NULL) {
                    emitter->otZ = 0x40;
                }
            }
            if (frame >= 0x2B) {
                s32 u;

                u = ((frame - 0x2B) << 10) / 38;
                ex->math_cos(u);
                ex->math_cos(u);
                ex->btlfx_drawRippleGrid((s32)&env->ot[2], -0x20, 0x10, 3, 0x50, 1, fx_196_rippleColors, 0x18, frame << 8, 0x186,
                                  0x100);
                if (frame == 0x2B) {
                    ex->btlfx_drawScreenTint((u32 *)&env->ot[2], 0, 0x20, 0xAA, 0xFF);
                }
                if (frame == 0x2D || frame == 0x32) {
                    ex->btlfx_drawScreenTint((u32 *)&env->ot[2], 0, 0, 0x64, 0xFF);
                }
                if (frame >= 0x2B) {
                    s32 d;

                    d = frame - 0x2B;
                    v1.vx = 0;
                    v1.vy = 0;
                    v1.vz = 0;
                    sc2.vz = 0x1000;
                    ((CVECTOR *)&v0)->r = ((CVECTOR *)&v0)->g = ((CVECTOR *)&v0)->b = -0x10 - (d * 0x8D / 38);
                    sc2.vy = sc2.vx = 0x1400 - ((d << 10) / 38);
                    ex->btlfx_drawScreenGrid(1, 0xA0, 0x70, 0x50, 0x38, 4, 4, 2, 2, &v1, &sc2, (CVECTOR *)&v0, 0, 0);
                }
            }
            fxm_drawScrollMoveX(frame * 3, 0x1C0, 0x180, 0x20, 0x80, 0x1E0, 0x180);
            if (frame >= 0x51) {
                SVECTOR bp;

                work->state0 = 1;
                work->camPos18.vx = 0;
                work->camPos18.vy = -0x64;
                work->camPos18.vz = 0;
                env->camera->points[3].vx = work->camPos18.vx;
                env->camera->points[3].vy = work->camPos18.vy;
                env->camera->points[3].vz = work->camPos18.vz;
                ex->btlfx_getUnitBodyPos(0, 1, (s16 *)&bp);
                if (bp.vy < -0x800) {
                    work->camPos18.vz = 0x300;
                    work->camPos18.vy -= 0x1F4;
                }
                entry->frame = -1;
            }
            break;
        }
        case 1: {
            SVECTOR pos1;
            SVECTOR rot1;
            VECTOR sc1;
            s32 s;
            s32 c;

            t = (frame << 12) / 24;
            s = t >> 2;
            work->camPos18.vy -= (ex->math_cos(s) * 0x18) >> 12;
            env->camera->points[3].vx = work->camPos18.vx;
            env->camera->points[3].vy = work->camPos18.vy;
            env->camera->points[3].vz = work->camPos18.vz;
            pos1.vx = 0;
            pos1.vy = -0x200 - (ex->math_cos(s) >> 3);
            rot1.vx = 0x400;
            rot1.vy = 0x400;
            pos1.vz = 0;
            rot1.vz = 0;
            sc1.vx = sc1.vy = 0x1000;
            sc1.vz = 0;
            c = ex->math_cos(s) >> 5;
            fx_196_drawWaveMesh(&pos1, &rot1, &sc1, c, c, 0x17, 0x3D04, 0, 0x80, (ex->math_cos(s) >> 3) + 0x300, s, 0x100, 0, 0,
                          0);
            fxm_drawScrollMoveX(frame * 3, 0x1C0, 0x180, 0x20, 0x80, 0x1E0, 0x180);
            if (frame >= 0x18) {
                ex->btlfx_clearEffModels();
                return 1;
            }
            break;
        }
        }
        work->frame4++;
    }
    return 0;
}

s32 fx_196_op86(s32 mode, Fx196Work86 *work, Unk800F78A4 *env, Unk80100C78Entry *entry, s32 *args) {
    BtlfxExports *ex = BTLFX_EXPORTS(env);
    Pool6CEntry *emitter;
    s32 i;

    if (mode == 0) {
        *args = 0x44;
    } else if (mode == 1) {
        fx_196_env = env;
        work->count40 = env->targets->targetCount;
        for (i = 0; i < work->count40; i++) {
            if (ex->btlfx_isEnemyUnit(i) != 0) {
                ex->btlfx_getUnitHomePos(i, (s16 *)&work->pos0[i]);
                work->pos0[i].vz -= ex->btlfx_getUnitRadius(i);
                work->pos0[i].vy -= 0x200;
                emitter = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, 8), (s32)&work->pos0[i]);
                if (emitter != NULL) {
                    emitter->otZ = ex->btlfx_addRadiusSixth(i, 0x200);
                }
            }
        }
    } else {
        if (entry->frame >= 0x2E) {
            return 1;
        }
    }
    return 0;
}

s32 fx_196_op87(s32 mode, s32 *work, Unk800F78A4 *env, Unk80100C78Entry *entry, s32 *args) {
    BtlfxExports *ex = BTLFX_EXPORTS(env);
    SVECTOR v;
    s32 frame;
    s32 n;

    if (mode == 0) {
        *args = 4;
    } else if (mode == 1) {
        n = *args;
        fx_196_env = env;
        *work = n;
    } else {
        frame = entry->frame;
        fxm_keyGetVec((Fx196Track *)ex->btlfx_getFileData(entry->file, fx_196_camPaths[*work].fileId2), frame, &v);
        env->camera->points[1].vx = v.vx;
        env->camera->points[1].vy = v.vy;
        env->camera->points[1].vz = v.vz;
        fxm_keyGetVec((Fx196Track *)ex->btlfx_getFileData(entry->file, fx_196_camPaths[*work].fileId0), frame, &v);
        env->camera->points[0].vx = v.vx;
        env->camera->points[0].vy = v.vy;
        env->camera->points[0].vz = v.vz;
        if (frame >= fx_196_camPaths[*work].duration4) {
            return 1;
        }
    }
    return 0;
}

INCLUDE_RODATA("asm/us/fx_196/nonmatchings/fx_196", fx_196_headerPtr);
