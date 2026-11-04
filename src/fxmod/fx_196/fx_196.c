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

static void fxm_keyGetVec2(Fx196Track *track, s32 t, SVECTOR *out0, SVECTOR *out1) {
    s16 *key;

    if (t >= track->count2) {
        t = track->count2 - 1;
    }
    key = &track->keys4[t * 6];
    if (out0 != NULL) {
        out0->vx = key[0];
        out0->vy = key[1];
        out0->vz = key[2];
    }
    if (out1 != NULL) {
        out1->vx = key[3];
        out1->vy = key[4];
        out1->vz = key[5];
    }
}

static void fxm_keyGetVec3(Fx196Track *track, s32 t, SVECTOR *out0, SVECTOR *out1, SVECTOR *out2) {
    s16 *key;

    if (t >= track->count2) {
        t = track->count2 - 1;
    }
    key = &track->keys4[t * 9];
    if (out0 != NULL) {
        out0->vx = key[0];
        out0->vy = key[1];
        out0->vz = key[2];
    }
    if (out1 != NULL) {
        out1->vx = key[3];
        out1->vy = key[4];
        out1->vz = key[5];
    }
    if (out2 != NULL) {
        out2->vx = key[6];
        out2->vy = key[7];
        out2->vz = key[8];
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

s32 fxm_sinHalfNeg(s32 ang) {
    s32 sinv = BTLFX_EXPORTS(fx_196_env)->math_sin(ang);

    if (sinv < 0) {
        sinv >>= 1;
    }
    return sinv;
}

void fx_196_drawWaveMesh(SVECTOR *pos, SVECTOR *rot, VECTOR *scale, s32 col, s32 col2, s32 tpage, s32 clut, s32 u, s32 v,
                   s32 amp, s32 ph, s32 step, s32 bulge, s32 ang, s32 dang) {
    Unk800F78A4 *env = fx_196_env;
    BtlfxExports *ex = BTLFX_EXPORTS(env);
    CVECTOR texCol;
    CVECTOR texEdgeCol;
    CVECTOR texFadeCol;
    CVECTOR shadeCol;
    CVECTOR shadeEdgeCol;
    CVECTOR shadeFadeCol;
    MATRIX screenMat;
    MATRIX worldMat;
    SVECTOR vs[4];
    VECTOR lv;
    DVECTOR sxy;
    s32 otz;
    s32 depth;
    POLY_GT4 *edgePoly;
    POLY_FT4 *poly;
    POLY_G4 *edgeShadePoly;
    POLY_F4 *shadePoly;
    s32 abr;
    s32 xb;
    s32 yb;
    s32 xo;
    s32 rowOfs;
    s32 i;
    s32 y0;
    s32 y1;
    s32 j;
    s32 x;
    s32 sinNext;
    s32 u0;
    s32 u1;
    s32 v0;
    s32 v1;
    s32 isft;

    abr = 1;
    gte_SetRotMatrix(env->viewMatrix);
    gte_SetTransMatrix(env->viewMatrix);
    gte_ldv0(pos);
    edgePoly = (POLY_GT4 *)env->prim;
    *(s32 *)&texFadeCol = 0;
    *(s32 *)&shadeFadeCol = 0;
    poly = (POLY_FT4 *)(edgePoly + 18);
    edgeShadePoly = (POLY_G4 *)(poly + 324);
    shadePoly = (POLY_F4 *)(edgeShadePoly + 18);
    env->prim = (u8 *)(shadePoly + 324);
    texCol.r = texCol.g = texCol.b = col;
    texEdgeCol.r = texEdgeCol.g = texEdgeCol.b = col;
    shadeCol.r = shadeCol.g = shadeCol.b = col2;
    shadeEdgeCol.r = shadeEdgeCol.g = shadeEdgeCol.b = col2;
    texCol.cd = abr != 0xFF ? 0x2E : 0x2C;
    texFadeCol.cd = abr != 0xFF ? 0x3E : 0x3C;
    texEdgeCol.cd = texFadeCol.cd;
    shadeCol.cd = abr != 0xFF ? 0x2A : 0x28;
    shadeFadeCol.cd = abr != 0xFF ? 0x3A : 0x38;
    tpage |= TPAGE_ABR(abr);
    shadeEdgeCol.cd = shadeFadeCol.cd;
    gte_rtv0tr_b();
    ex->math_rotMatrixYXZ(rot, &screenMat);
    ex->ScaleMatrix(&screenMat, scale);
    worldMat = screenMat;
    worldMat.t[0] = pos->vx;
    worldMat.t[1] = pos->vy;
    worldMat.t[2] = pos->vz;
    gte_stlvnl(screenMat.t);
    ex->MulRotMatrix(&screenMat);
    gte_SetRotMatrix(&screenMat);
    gte_SetTransMatrix(&screenMat);
    xb = -0x24EA;
    yb = -0x17CD;
    for (rowOfs = 0, i = 0; i < 18; i++, rowOfs += 0x2A5) {
        j = 0;
        ph += step;
        y0 = yb + rowOfs;
        y1 = y0 + 0x2A5;
        for (xo = 0; j < 19; j++, xo += 0x41A) {
            x = xb + xo;
            vs[0].vx = x + ((ex->math_sin(ph) * amp) >> 12);
            vs[1].vx = vs[0].vx + 0x41A;
            sinNext = ex->math_sin(ph + step);
            vs[0].vy = vs[1].vy = y0;
            vs[2].vy = vs[3].vy = y1;
            x += (sinNext * amp) >> 12;
            vs[2].vx = x;
            vs[3].vx = x + 0x41A;
            vs[0].vz = vs[2].vz = (fxm_sinHalfNeg(ang + j * dang) * bulge) >> 12;
            vs[1].vz = vs[3].vz = (fxm_sinHalfNeg(ang + (j + 1) * dang) * bulge) >> 12;
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
                gte_stszotz(&depth);
                isft = 1;
                if (j == 0) {
                    *(s32 *)&edgePoly->r0 = *(s32 *)&edgePoly->r2 = *(s32 *)&texFadeCol;
                    *(s32 *)&edgePoly->r1 = *(s32 *)&edgePoly->r3 = *(s32 *)&texEdgeCol;
                    *(s32 *)&edgeShadePoly->r0 = *(s32 *)&edgeShadePoly->r2 = *(s32 *)&shadeFadeCol;
                    *(s32 *)&edgeShadePoly->r1 = *(s32 *)&edgeShadePoly->r3 = *(s32 *)&shadeEdgeCol;
                    isft = 0;
                } else {
                    *(s32 *)&poly->r0 = *(s32 *)&texCol;
                    *(s32 *)&shadePoly->r0 = *(s32 *)&shadeCol;
                }
                if (otz > 0) {
                    if (isft) {
                        poly->r0 >>= 1;
                        poly->g0 >>= 1;
                    } else {
                        edgePoly->r0 >>= 1;
                        edgePoly->g0 >>= 1;
                        edgePoly->r1 >>= 1;
                        edgePoly->g1 >>= 1;
                        edgePoly->r2 >>= 1;
                        edgePoly->g2 >>= 1;
                        edgePoly->r3 >>= 1;
                        edgePoly->g3 >>= 1;
                    }
                }
                if (isft) {
                    poly->u0 = poly->u2 = u0;
                    poly->u1 = poly->u3 = u1;
                    poly->v0 = poly->v1 = v0;
                    poly->v2 = poly->v3 = v1;
                    poly->tpage = tpage;
                    poly->clut = clut;
                    gte_stsxy3(&poly->x0, &poly->x1, &poly->x2);
                    *(s32 *)&shadePoly->x0 = *(s32 *)&poly->x0;
                    *(s32 *)&shadePoly->x1 = *(s32 *)&poly->x1;
                    *(s32 *)&shadePoly->x2 = *(s32 *)&poly->x2;
                } else {
                    edgePoly->u0 = edgePoly->u2 = u0;
                    edgePoly->u1 = edgePoly->u3 = u1;
                    edgePoly->v0 = edgePoly->v1 = v0;
                    edgePoly->v2 = edgePoly->v3 = v1;
                    edgePoly->tpage = tpage;
                    edgePoly->clut = clut;
                    gte_stsxy3(&edgePoly->x0, &edgePoly->x1, &edgePoly->x2);
                    *(s32 *)&edgeShadePoly->x0 = *(s32 *)&edgePoly->x0;
                    *(s32 *)&edgeShadePoly->x1 = *(s32 *)&edgePoly->x1;
                    *(s32 *)&edgeShadePoly->x2 = *(s32 *)&edgePoly->x2;
                    gte_SetRotMatrix(&worldMat);
                    gte_SetTransMatrix(&worldMat);
                    gte_ldv0(vs);
                    gte_rtv0tr();
                    gte_stlvnl(&lv);
                    fx_196_wavePoints[i].vx = lv.vx;
                    fx_196_wavePoints[i].vy = lv.vy;
                    fx_196_wavePoints[i].vz = lv.vz;
                    gte_SetRotMatrix(&screenMat);
                    gte_SetTransMatrix(&screenMat);
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
                    ex->btlfx_addPrim(0x09000000, depth, poly);
                    ex->btlfx_addPrimAbr(0x05000000, depth, shadePoly, 2);
                    gte_stsxy(&poly->x3);
                    *(s32 *)&shadePoly->x3 = *(s32 *)&poly->x3;
                    poly++;
                    shadePoly++;
                } else {
                    ex->btlfx_addPrim(0x0C000000, depth, edgePoly);
                    ex->btlfx_addPrimAbr(0x08000000, depth, edgeShadePoly, 2);
                    gte_stsxy(&edgePoly->x3);
                    *(s32 *)&edgeShadePoly->x3 = *(s32 *)&edgePoly->x3;
                    edgePoly++;
                    edgeShadePoly++;
                }
            }
        }
    }
}

static s32 fxm_nextRingPoint(SVECTOR *out) {
    s32 slot = fx_196_nextWavePoint;

    out->pad = 1;
    out->vx = fx_196_wavePoints[slot].vx;
    out->vy = fx_196_wavePoints[slot].vy;
    out->vz = fx_196_wavePoints[slot].vz;
    slot++;
    if (slot >= 16) {
        slot = 0;
    }
    fx_196_nextWavePoint = slot;
    return 0;
}

s32 fx_196_op85(s32 mode, Fx196Work85 *work, Unk800F78A4 *env, Unk80100C78Entry *entry, s32 *args) {
    BtlfxExports *ex = BTLFX_EXPORTS(env);
    s32 frame;
    s32 t;
    SVECTOR pos;
    SVECTOR rot;
    VECTOR sc;
    SVECTOR camPt0; /* also the light colour (as a CVECTOR) */
    SVECTOR camPt1;
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
            s32 sweep;
            s32 bulge;
            s32 phaseMul;
            s32 sweepAng;
            s32 shadeBright;
            s32 texBright;
            s32 i;
            s32 sinv;

            t = (frame << 12) / 81;
            if (frame == 0x12) {
                work->emit48 = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, FX_LEVIATHAN_WATER_DROPS_2), (s32)&work->pos10);
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
                    sweep = ((frame - 9) << 12) / 52;
                } else {
                    sweep = 0x1000;
                }
                if (frame < 0x14) {
                    pos.vx = 0;
                    phaseMul = 1;
                } else {
                    pos.vx = 0x1000 - (t >> 1);
                    phaseMul = 2;
                }
                sweepAng = sweep >> 2;
                sinv = ex->math_sin(t >> 2);
                pos.vy = ((0x1000 - sinv) >> 2) - (0x400 + ((ex->math_cos(sweepAng) * 0x44C) >> 12));
                pos.vz = ((0x1000 - ex->math_cos(sweepAng)) << 2) - 0x4000;
                rot.vx = 0x400;
                rot.vy = 0x400;
                rot.vz = 0;
                sc.vx = sc.vy = 0x1000;
                sc.vz = ex->math_cos(sweepAng);
                if (frame < 0x10) {
                    bulge = ex->math_sin((frame << 10) >> 4) + 0x800;
                } else {
                    bulge = 0x1800;
                }
                shadeBright = 0xC8;
                if ((u32)(frame - 0x28) < 0x20) {
                    shadeBright = 0xC8 - (((frame - 0x28) * 0x96) >> 5);
                } else if (frame >= 0x48) {
                    shadeBright = 0x32;
                }
                if (frame < 0x3D) {
                    texBright = 0x80;
                } else {
                    texBright = 0x80 - (((frame - 0x3D) << 5) / 20);
                }
                if (frame < 0x29) {
                    env->otZ = 0;
                } else {
                    env->otZ = -0x80;
                }
                fx_196_drawWaveMesh(&pos, &rot, &sc, texBright, shadeBright, 0x17, 0x3D04, 0, 0x80, 0x7D0, sweep * phaseMul, -0x140, bulge,
                              ex->math_cos(sweep >> 2) + 0x400, 0x100);
                env->otZ = 0x7C;
                for (i = 0; i < 18; i++) {
                    if (fx_196_wavePoints[i].pad != 0) {
                        texBright = (i & 1) << 12;
                        ex->btlfx_drawAnimQuads((u8 *)ex->btlfx_getFileData(entry->file, FX_LEVIATHAN_FOAM), &fx_196_wavePoints[i], NULL,
                                                ((t * 3) >> 3) + 0x3800 + texBright, frame + i * 2, 1, 0x1000);
                    }
                }
                env->otZ = 0x37;
                if (frame >= 0x1D) {
                    for (i = 0; i < 7; i++) {
                        if (fx_196_wavePoints[i].pad != 0) {
                            ex->btlfx_drawAnimQuads((u8 *)ex->btlfx_getFileData(entry->file, FX_LEVIATHAN_FOAM), &fx_196_wavePoints[i], NULL,
                                                    0x2400, frame + i * 2 + 4, 1, 0x1000);
                        }
                    }
                }
                env->otZ = 0;
            }
            if (frame == 0x32) {
                Pool6CEntry *emitter;

                camPt0.vx = env->camera->points[0].vx;
                camPt0.vy = env->camera->points[0].vy;
                camPt0.vz = env->camera->points[0].vz;
                camPt1.vx = env->camera->points[1].vx;
                camPt1.vy = env->camera->points[1].vy;
                camPt1.vz = env->camera->points[1].vz;
                work->pos10.vx = (camPt0.vx + camPt1.vx) / 2;
                work->pos10.vy = (camPt0.vy + camPt1.vy) / 2;
                work->pos10.vz = (camPt0.vz + camPt1.vz) / 2;
                emitter = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, FX_LEVIATHAN_WATER_BUBBLES), (s32)&work->pos10);
                if (emitter != NULL) {
                    emitter->otZ = 0x40;
                }
            }
            if (frame >= 0x2B) {
                s32 rippleAng;

                rippleAng = ((frame - 0x2B) << 10) / 38;
                ex->math_cos(rippleAng);
                ex->math_cos(rippleAng);
                ex->btlfx_drawRippleGrid((s32)&env->ot[2], -0x20, 0x10, 3, 0x50, 1, fx_196_rippleColors, 0x18, frame << 8, 0x186,
                                  0x100);
                if (frame == 0x2B) {
                    ex->btlfx_drawScreenTint((u32 *)&env->ot[2], 0, 0x20, 0xAA, 0xFF);
                }
                if (frame == 0x2D || frame == 0x32) {
                    ex->btlfx_drawScreenTint((u32 *)&env->ot[2], 0, 0, 0x64, 0xFF);
                }
                if (frame >= 0x2B) {
                    s32 fadeFrame;

                    fadeFrame = frame - 0x2B;
                    camPt1.vx = 0;
                    camPt1.vy = 0;
                    camPt1.vz = 0;
                    sc2.vz = 0x1000;
                    ((CVECTOR *)&camPt0)->r = ((CVECTOR *)&camPt0)->g = ((CVECTOR *)&camPt0)->b = -0x10 - (fadeFrame * 0x8D / 38);
                    sc2.vy = sc2.vx = 0x1400 - ((fadeFrame << 10) / 38);
                    ex->btlfx_drawScreenGrid(1, 0xA0, 0x70, 0x50, 0x38, 4, 4, 2, 2, &camPt1, &sc2, (CVECTOR *)&camPt0, 0, 0);
                }
            }
            fxm_drawScrollMoveX(frame * 3, 0x1C0, 0x180, 0x20, 0x80, 0x1E0, 0x180);
            if (frame >= 0x51) {
                SVECTOR bodyPos;

                work->state0 = 1;
                work->camPos18.vx = 0;
                work->camPos18.vy = -0x64;
                work->camPos18.vz = 0;
                env->camera->points[3].vx = work->camPos18.vx;
                env->camera->points[3].vy = work->camPos18.vy;
                env->camera->points[3].vz = work->camPos18.vz;
                ex->btlfx_getUnitBodyPos(0, 1, (s16 *)&bodyPos);
                if (bodyPos.vy < -0x800) {
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
            s32 ang;
            s32 bright;

            t = (frame << 12) / 24;
            ang = t >> 2;
            work->camPos18.vy -= (ex->math_cos(ang) * 0x18) >> 12;
            env->camera->points[3].vx = work->camPos18.vx;
            env->camera->points[3].vy = work->camPos18.vy;
            env->camera->points[3].vz = work->camPos18.vz;
            pos1.vx = 0;
            pos1.vy = -0x200 - (ex->math_cos(ang) >> 3);
            rot1.vx = 0x400;
            rot1.vy = 0x400;
            pos1.vz = 0;
            rot1.vz = 0;
            sc1.vx = sc1.vy = 0x1000;
            sc1.vz = 0;
            bright = ex->math_cos(ang) >> 5;
            fx_196_drawWaveMesh(&pos1, &rot1, &sc1, bright, bright, 0x17, 0x3D04, 0, 0x80, (ex->math_cos(ang) >> 3) + 0x300, ang, 0x100, 0, 0,
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
                emitter = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, FX_LEVIATHAN_FOAM_2), (s32)&work->pos0[i]);
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
    SVECTOR camPt;
    s32 frame;
    s32 path;

    if (mode == 0) {
        *args = 4;
    } else if (mode == 1) {
        path = *args;
        fx_196_env = env;
        *work = path;
    } else {
        frame = entry->frame;
        fxm_keyGetVec((Fx196Track *)ex->btlfx_getFileData(entry->file, fx_196_camPaths[*work].fileId2), frame, &camPt);
        env->camera->points[1].vx = camPt.vx;
        env->camera->points[1].vy = camPt.vy;
        env->camera->points[1].vz = camPt.vz;
        fxm_keyGetVec((Fx196Track *)ex->btlfx_getFileData(entry->file, fx_196_camPaths[*work].fileId0), frame, &camPt);
        env->camera->points[0].vx = camPt.vx;
        env->camera->points[0].vy = camPt.vy;
        env->camera->points[0].vz = camPt.vz;
        if (frame >= fx_196_camPaths[*work].duration4) {
            return 1;
        }
    }
    return 0;
}

INCLUDE_RODATA("asm/us/fx_196/nonmatchings/fx_196", fx_196_headerPtr);
