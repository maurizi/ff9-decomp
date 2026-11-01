#include "fxmod/fx_0fb_1.h"

static void fxm_drawTexCylinder(SVECTOR *pos, SVECTOR *rot, VECTOR *scale, s32 r, s32 p, CVECTOR *rgb, s32 tpage, s32 clut, s32 u,
                   s32 v, s32 dang, s32 abr) {
    Unk800F78A4 *env = fx_0fb_1_env;
    BtlfxExports *ex = BTLFX_EXPORTS(env);
    CVECTOR col;
    MATRIX m;
    SVECTOR q[4];
    s32 z;
    POLY_FT4 *prim;
    s32 ang;
    s32 a;
    s32 z0;
    s32 z1;
    s32 row;
    s32 rv;
    s32 j;
    s32 ju;
    s32 u0;
    s32 v0;
    s32 u1;
    s32 v1;

    gte_SetRotMatrix(env->viewMatrix);
    gte_SetTransMatrix(env->viewMatrix);
    gte_ldv0(pos);
    prim = (POLY_FT4 *)env->prim;
    env->prim = (u8 *)(prim + 320);
    col.cd = abr == 0xFF ? 0x2C : 0x2E;
    col.r = (rgb->r * p) >> 12;
    col.g = (rgb->g * p) >> 12;
    col.b = (rgb->b * p) >> 12;
    tpage |= (abr & 3) << 5;
    gte_rtv0tr_b();
    ex->math_rotMatrixYXZ(rot, &m);
    ex->ScaleMatrix(&m, scale);
    gte_stlvnl(m.t);
    ex->MulRotMatrix(&m);
    gte_SetRotMatrix(&m);
    gte_SetTransMatrix(&m);
    ang = 0;
    z1 = 0x866;
    z0 = 0;
    for (row = 0; row < 20; row++) {
        ang += dang;
        rv = row & 0xF;
        for (j = 0; j < 16; j++) {
            q[0].vx = (ex->math_cos(ang) * r) >> 12;
            q[0].vy = (ex->math_sin(ang) * r) >> 12;
            a = ang + 0x100;
            q[1].vx = (ex->math_cos(a) * r) >> 12;
            q[1].vy = (ex->math_sin(a) * r) >> 12;
            q[0].vz = q[1].vz = z0;
            ang += dang;
            q[2].vx = (ex->math_cos(ang) * r) >> 12;
            q[2].vy = (ex->math_sin(ang) * r) >> 12;
            a = ang + 0x100;
            q[3].vx = (ex->math_cos(a) * r) >> 12;
            q[3].vy = (ex->math_sin(a) * r) >> 12;
            q[2].vz = q[3].vz = z1;
            ang -= dang;
            gte_ldv3c(q);
            gte_rtpt();
            ju = j & 3;
            /* two statements each: a twice-set u0/v0 is not loop-invariant, so v + rv * 8 stays in the loop */
            u0 = u;
            u0 += ju * 32;
            v0 = v;
            v0 += rv * 8;
            u1 = u0 + 0x20;
            if (ju == 3) {
                u1 = u0 + 0x1F;
            }
            v1 = v0 + 8;
            if (rv == 0xF) {
                v1 = v0 + 7;
            }
            gte_nclip();
            *(s32 *)&prim->r0 = *(s32 *)&col;
            gte_stopz(&z);
            if (z >= 0) {
                prim->u0 = prim->u2 = u0;
                prim->u1 = prim->u3 = u1;
                prim->v0 = prim->v1 = v0;
                prim->v2 = prim->v3 = v1;
                prim->tpage = tpage;
                prim->clut = clut;
                gte_stsxy3(&prim->x0, &prim->x1, &prim->x2);
                gte_avsz3();
                gte_stszotz(&z);
                z = 0xFFF;
                gte_ldv0(&q[3]);
                gte_rtps();
                ex->btlfx_addPrim(0x09000000, z, prim);
                gte_stsxy(&prim->x3);
                prim++;
            }
            ang += 0x100;
        }
        z1 += 0x866;
        z0 += 0x866;
    }
}

INCLUDE_RODATA("asm/us/fx_0fb_1/nonmatchings/fx_0fb_1", fx_0fb_1_headerPtr);

static s32 fx_0fb_1_op80PoolTask(s32 mode, Fx0fb1Rock *it, Unk800F78A4 *env, Unk80100C78Entry *entry, s32 frame, Unk800DA1B8 *pool,
                  Fx0fb1Work80 *work) {
    BtlfxExports *ex = BTLFX_EXPORTS(env);
    VECTOR pos;
    VECTOR scale;
    SVECTOR rot;
    s32 state;
    s32 rgb;
    s32 k;
    s32 shardAng;
    Fx0fb1Rock *shard;

    if (mode == 1) {
        it->state2 = 0;
        it->rot14.vx = ex->btlfx_rand();
        it->rot14.vy = ex->btlfx_rand();
        it->rot14.vz = 0;
        it->spinX1C = ex->btlfx_rand() % 128 + 0x80;
        it->spinY1E = 0xC0 - ex->btlfx_rand() % 32;
        it->emit24 = NULL;
        it->emit20 = NULL;
    } else {
    if (it->kind0 != FX_0FB_1_SPARK) {
        it->rot14.vx += it->spinX1C;
        it->rot14.vy += it->spinY1E;
        BTLFX_COPY_SVEC(&rot, &work->rot6C);
        rot.pad = 1;
    }
    switch (it->kind0) {
    case FX_0FB_1_ROCK_BIG:
    case FX_0FB_1_ROCK_PASS: {
        SVECTOR vec;

        state = it->state2;
        switch (state) {
        case 0:
            if (fx_0fb_1_rocksHeld == 0) {
                it->posC.vz -= (it->speedA << 16) >> 20;
                it->posC.vy += (it->speedA << 16) >> 21;
            }
            pos.vx = it->posC.vx;
            pos.vy = it->posC.vy;
            pos.vz = it->posC.vz;
            scale.vz = scale.vy = scale.vx = 0x199;
            ex->Hi_DrawEffModel(&it->rot14, &pos, (s32)&scale, work->model4C);
            rgb = fx_0fb_1_rockRgb;
            if (rgb != 0x1000) {
                rgb >>= 5;
                rgb -= 0x80;
                ex->Hi_ModifyEffModelRGB(work->model4C, rgb, rgb, rgb);
            }
            if (it->kind0 == FX_0FB_1_ROCK_BIG) {
                if (pos.vz < work->modelZ5C + 0x20) {
                    it->state2 = 1;
                    ((s16 *)it)[-1] = -1;
                    shardAng = ex->btlfx_rand();
                    for (k = 0; k < 3; k++, shardAng += 0x555) {
                        shard = ex->btlfx_poolSpawn(entry->pool);
                        if (shard != NULL) {
                            shard->kind0 = FX_0FB_1_ROCK_SHARD;
                            BTLFX_COPY_SVEC(&shard->posC, &it->posC);
                            shard->posC.vx += ex->btlfx_randCentered(0x100);
                            shard->posC.vy += ex->btlfx_randCentered(0x100);
                            shard->spinX1C = ex->btlfx_randCentered(0x84);
                            shard->spinY1E = ex->btlfx_randCentered(0x84);
                            ex->btlfx_forwardAngles((s16 *)&vec, (XY32 *)&work->rot6C, shardAng);
                            ex->btlfx_rotateOffset(&vec, (it->speedA << 16) >> 21, (s32)&shard->velX4);
                        }
                    }
                    it->emit20 = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, 0x19), (s32)&it->posC);
                    if (it->emit20 != NULL) {
                        it->emit20->otZ = 0x20;
                    }
                    it->emit24 = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, 0xE), (s32)&it->posC);
                    if (it->emit20 != NULL) {
                        it->emit20->otZ = 0x20;
                    }
                }
            } else {
                gte_SetRotMatrix(env->viewMatrix);
                gte_SetTransMatrix(env->viewMatrix);
                gte_ldv0(&pos);
                gte_rtps();
                gte_stsxy(&vec);
                if (vec.vx >= 0xF1 || vec.vy >= 0x141) {
                    return 1;
                }
                break;
            }
            break;
        case 1:
            if (it->emit20 != NULL) {
                it->emit20->rot.vx = work->rot6C.vx;
                it->emit20->rot.vy = work->rot6C.vy;
                it->emit20->rot.vz = work->rot6C.vz;
            }
            env->otZ = 0x40;
            if (frame < 10) {
                ex->btlfx_drawRings((u8 *)ex->btlfx_getFileData(entry->file, 0x16), &it->posC, NULL, 0x1000, frame, state,
                                  ex->math_cos((frame << 10) / 10));
            }
            env->otZ = 0x10;
            if (frame < 8) {
                VECTOR scale2;
                VECTOR pos2;
                s32 ang;

                if (frame < 3) {
                    ex->btlfx_drawRings((u8 *)ex->btlfx_getFileData(entry->file, 0x10), &it->posC, &rot, 0x1800, frame,
                                      state, ex->math_cos((frame << 10) / 3));
                }
                pos2.vx = it->posC.vx;
                pos2.vy = it->posC.vy;
                ang = (frame << 10) >> 3;
                pos2.vz = it->posC.vz;
                scale2.vx = scale2.vy = scale2.vz = ex->math_sin(ang) + 0x800;
                ex->Hi_DrawEffModel(&rot, &pos2, (s32)&scale2, work->modelC0);
                ex->Hi_ModifyEffModelRGB(work->modelC0, (ex->math_cos(ang) >> 5) - 0x80, (ex->math_cos(ang) >> 5) - 0x80,
                                         (ex->math_cos(ang) >> 5) - 0x80);
            }
            env->otZ = 0;
            if (it->emit20->state == BTLFX_EMIT_DONE) {
                return 1;
            }
            break;
        default:
            return 0;
        }
        break;
    }
    case FX_0FB_1_ROCK_SMALL:
        state = it->state2;
        switch (state) {
        case 0:
            if (fx_0fb_1_rocksHeld == 0) {
                it->posC.vz -= (it->speedA << 16) >> 20;
                it->posC.vy += (it->speedA << 16) >> 21;
            }
            pos.vx = it->posC.vx;
            pos.vy = it->posC.vy;
            pos.vz = it->posC.vz;
            scale.vz = scale.vy = scale.vx = 0xCC;
            ex->Hi_DrawEffModel(&it->rot14, &pos, (s32)&scale, work->model50);
            rgb = fx_0fb_1_rockRgb;
            if (rgb != 0x1000) {
                rgb >>= 5;
                rgb -= 0x80;
                ex->Hi_ModifyEffModelRGB(work->model50, rgb, rgb, rgb);
            }
            if (work->modelZ5C > pos.vz) {
                it->state2 = 1;
                ((s16 *)it)[-1] = -1;
                it->emit20 = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, 0x18), (s32)&it->posC);
                if (it->emit20 != NULL) {
                    it->emit20->otZ = 0x20;
                }
                it->emit24 = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, 0xE), (s32)&it->posC);
                if (it->emit20 != NULL) {
                    it->emit20->otZ = 0x20;
                }
            }
            break;
        case 1:
            if (it->emit20 != NULL) {
                it->emit20->rot.vx = work->rot6C.vx;
                it->emit20->rot.vy = work->rot6C.vy;
                it->emit20->rot.vz = work->rot6C.vz;
            }
            env->otZ = 0x40;
            if (frame < 6) {
                ex->btlfx_drawRings((u8 *)ex->btlfx_getFileData(entry->file, 0x16), &it->posC, NULL, 0x800, frame, state,
                                  ex->math_cos((frame << 10) / 6));
            }
            env->otZ = 0xC;
            if (frame < 8) {
                VECTOR scale2;
                VECTOR pos2;
                s32 ang;

                if (frame < 3) {
                    ex->btlfx_drawRings((u8 *)ex->btlfx_getFileData(entry->file, 0x10), &it->posC, &rot, 0x1000, frame,
                                      state, ex->math_cos((frame << 10) / 3));
                }
                pos2.vx = it->posC.vx;
                pos2.vy = it->posC.vy;
                ang = (frame << 10) >> 3;
                pos2.vz = it->posC.vz;
                scale2.vx = scale2.vy = scale2.vz = (ex->math_sin(ang) >> 1) + 0x555;
                ex->Hi_DrawEffModel(&rot, &pos2, (s32)&scale2, work->modelC0);
                ex->Hi_ModifyEffModelRGB(work->modelC0, (ex->math_cos(ang) >> 5) - 0x80, (ex->math_cos(ang) >> 5) - 0x80,
                                         (ex->math_cos(ang) >> 5) - 0x80);
            }
            env->otZ = 0;
            if (it->emit20->state == BTLFX_EMIT_DONE) {
                return 1;
            }
            break;
        default:
            return 0;
        }
        break;
    case FX_0FB_1_ROCK_SHARD:
        it->posC.vz += 0x32;
        it->posC.vy -= 0x19;
        it->posC.vx += it->velX4;
        it->posC.vy += it->velY6;
        it->posC.vz += it->velZ8;
        pos.vx = it->posC.vx;
        pos.vy = it->posC.vy;
        pos.vz = it->posC.vz;
        scale.vz = scale.vy = scale.vx = 0xCC;
        ex->Hi_DrawEffModel(&it->rot14, &pos, (s32)&scale, work->model50);
        rgb = fx_0fb_1_rockRgb;
        if (rgb != 0x1000) {
            rgb >>= 5;
            rgb -= 0x80;
            ex->Hi_ModifyEffModelRGB(work->model50, rgb, rgb, rgb);
        }
        if (frame >= 0xD) {
            return 1;
        }
        break;
    case FX_0FB_1_SPARK:
        it->posC.vx = (s16)(it->posC.vx - 0x80) - it->speedA;
        ex->btlfx_drawParticles((u8 *)ex->btlfx_getFileData(entry->file, 0xA), &it->posC, NULL, (it->velX4 * 3) >> 1, 0,
                                1, -1, 0, 0);
        if (frame >= 0x11) {
            return 1;
        }
        break;
    }
    }
    return 0;
}

/* padded: a zero word follows fx_0fb_1_op80PoolTask's jump table (op80's table is 8-aligned in the original object) */
__asm__(".section .rodata\n\t.word 0\n\t.section .text");

/* Matching notes: cases 3-6 use fx_17a op80's variable roles; case 3 reuses t (the original keeps it in t's
 * spill slot), case 5's cos value h is a block local declared after w (its slot comes last), and the two dead
 * `struct { s32 x; }` locals in the mode-1 block take the original's unused spill slots (as in fx_0fb_0 op80).
 * The mode-1 statement order was found by a hill climb (build/w/toyhill.py); it decides the sched1 order that
 * keeps the original's second unkC.vy load. */
s32 fx_0fb_1_op80(s32 mode, Fx0fb1Op80Work *work, Unk800F78A4 *env, Unk80100C78Entry *entry, EffInit *init) {
    BtlfxExports *ex = BTLFX_EXPORTS(env);
    Pool6CEntry *emitter;
    Fx0fb1Rock *spark;
    s32 frame;
    s32 t;
    s32 domeBright;
    s32 trail;
    s32 glow;
    s32 gridGrow;
    s32 tmp;
    s32 i;
    s32 orbitAng;
    s32 phase;
    s32 ringScale;
    s32 size;
    s32 sparkFade;
    s32 k;
    s32 fadeT;
    s32 level;
    s32 endScale;
    s32 scrollX;
    s32 unused;
    s32 spinAng;
    s32 pulseAng;
    s32 beamAng;
    s32 tint;
    s32 ph;
    s32 strip;
    s32 sp;
    s32 zz1;
    s32 zz2;
    s32 zz3;
    s32 zz4;
    s32 zz5;
    s32 zz6;
    Fx0fb1Rock *rock;
    s32 zz8;
    s32 zz9;

    if (mode == 0) {
        init->workSize = 0xE8;
        init->elemSize = 0x28;
        init->elemFn = (s32)fx_0fb_1_op80PoolTask;
        init->poolCount = 0x30;
    } else if (mode == 1) {
        work->state0 = 0;
        work->frame4 = 0;
        fx_0fb_1_env = env;
        ex->btlfx_initEffModels(((Fx0fb1Summon *)env->loader)->effModels38, 0x1F);
        for (i = 0; i < 2; i++) {
            work->model4C[i] = ex->Hi_RegisterTexEffModel((BtlModelHeader *)ex->btlfx_getFileData(entry->file, fx_0fb_1_modelBlocks[i]),
                                                        0x9A, 0x3E00, 0, 0);
        }
        work->modelA0 = ex->Hi_RegisterTexEffModel((BtlModelHeader *)ex->btlfx_getFileData(entry->file, 0x82), 0x96, 0x3D40, 0, 0);
        work->modelA4 = ex->Hi_RegisterTexEffModel((BtlModelHeader *)ex->btlfx_getFileData(entry->file, 0x87), 0x9A, 0x3E00, 0, 0);
        work->modelA8 = ex->Hi_RegisterTexEffModel((BtlModelHeader *)ex->btlfx_getFileData(entry->file, 0x90), 0xBD, 0x3E40, 0, 0);
        work->modelAC = ex->Hi_RegisterTexEffModel((BtlModelHeader *)ex->btlfx_getFileData(entry->file, 0x8F), 0xBD, 0x3E40, 0, 0);
        work->modelB0 = ex->Hi_RegisterTexEffModel((BtlModelHeader *)ex->btlfx_getFileData(entry->file, 0x89), 0xBD, 0x3E40, 0, 0);
        work->modelB4 = ex->Hi_RegisterTexEffModel((BtlModelHeader *)ex->btlfx_getFileData(entry->file, 0x13), 0xBD, 0x3EC0, 0, 0x80);
        work->modelB8 = ex->Hi_RegisterTexEffModel((BtlModelHeader *)ex->btlfx_getFileData(entry->file, 0x23), 0x3D, 0x3E81, 0, 0x80);
        work->modelBC = ex->Hi_RegisterTexEffModel((BtlModelHeader *)ex->btlfx_getFileData(entry->file, 3), 0x138, 0xFFFF, 0, 0x80);
        work->modelC0 = ex->Hi_RegisterTexEffModel((BtlModelHeader *)ex->btlfx_getFileData(entry->file, 0x8C), 0x3A, 0x3D0A, 0, 0x80);
        work->centerC.vy = -0x1000;
        work->centerC.vx = 0;
        work->rot8C.vz = 0;
        work->pos74.vx = work->centerC.vx;
        work->rot8C.vx = -0x400;
        work->rot8C.vy = 0x200;
        work->scale80.vx = 0x1800;
        work->pos74.vy = work->centerC.vy;
        work->centerC.vz = 0;
        work->pos74.vx += 0x5000;
        work->pos74.vz = work->centerC.vz;
        work->pos74.vz -= 0x3000;
        work->pos74.vy += 0x4000;
        work->pos54.vx = work->centerC.vx;
        work->scale80.vy = 0x1800;
        work->scale80.vz = 0x1000;
        work->pos54.vy = work->centerC.vy;
        work->pos54.vz = work->centerC.vz;
        work->scale60.vx = 0x400;
        work->pos54.vy += 0x1000;
        work->scale60.vy = 0x400;
        work->pos54.vz -= 0x2000;
        work->scale60.vz = 0x400;
        {
            SVECTOR aimPos;
            struct { s32 x; } q0 = { frame };
            struct { s32 x; } q1 = { frame };

            aimPos.vx = work->pos54.vx;
            aimPos.vy = work->pos54.vy;
            aimPos.vz = work->pos54.vz;
            ex->btlfx_aimAngles((s16 *)&work->centerC, (s16 *)&aimPos, (s16 *)&work->rot6C);
            work->rot6C.vz = -0x100;
            env->camera->points[3].vx = work->pos54.vx;
            env->camera->points[3].vy = work->pos54.vy;
            env->camera->points[3].vz = work->pos54.vz;
            BTLFX_COPY_SVEC(&work->camTarget3C, &work->pos54);
            fx_0fb_1_unitsRestored = 0;
            fx_0fb_1_rocksHeld = 0;
            fx_0fb_1_rockRgb = 0x1000;
        }
    } else {
    domeBright = 0;
    trail = 0;
    glow = 0;
    gridGrow = 0;
    frame = entry->frame;
    {
    switch (work->state0) {
    case 0:
        if (frame == 0) {
            work->pos54.vy += 0x200;
            work->pos54.vz -= 0x400;
        }
        work->pos54.vy -= 0x20;
        work->pos54.vz += 0x40;
        ex->Hi_DrawEffModel(&work->rot6C, (VECTOR *)&work->pos54, (s32)&work->scale60, work->modelA8);
        work->rot6C.vz += 0x400;
        ex->Hi_DrawEffModel(&work->rot6C, (VECTOR *)&work->pos54, (s32)&work->scale60, work->modelA8);
        domeBright = 0x1000;
        work->rot6C.vz -= 0x400;
        glow = 0x97;
        rock = ex->btlfx_poolSpawn((s16 *)entry->pool);
        if (rock != NULL) {
            rock->kind0 = frame % 3;
            rock->posC.vx = work->pos54.vx;
            rock->posC.vy = work->pos54.vy;
            rock->posC.vz = work->pos54.vz;
            if (rock->kind0 == 2) {
                rock->kind0 = FX_0FB_1_ROCK_PASS;
                spinAng = ex->btlfx_rand();
                rock->posC.vx += (ex->math_sin(spinAng) * 2) / 3;
                tmp = ex->math_cos(spinAng);
                rock->posC.vz += 0x800;
                rock->posC.vy = (s16)(rock->posC.vy - 0x400) - (tmp * 2) / 3;
            } else {
                rock->posC.vx += ex->btlfx_randCentered(0x800);
                rock->posC.vy = (s16)(rock->posC.vy - 0x400) - ex->btlfx_randCentered(0x800);
                rock->posC.vz += 0x800;
            }
            rock->speedA = ex->btlfx_randCentered(0x800) + 0x1800;
        }
        if (frame >= 0x1C) {
            work->state0 = 1;
            entry->frame = -1;
            ex->btlfx_poolClear(entry->pool);
            ex->btlfx_initParticleSystem();
            work->scale60.vx = 0x400;
            work->scale60.vy = 0x249;
            work->scale60.vz = 0x400;
            work->pos54.vx = work->centerC.vx;
            work->pos54.vy = work->centerC.vy;
            work->pos54.vz = work->centerC.vz;
            work->pos54.vy += 0xB84;
            work->pos54.vz -= 0x1708;
            for (i = 0, zz4 = 0; i < 0x19; i += 1 + (zz4 & 1)) {
                rock = ex->btlfx_poolSpawn((s16 *)entry->pool);
                if (rock != NULL) {
                    s32 kk;

                    rock->kind0 = i & 1;
                    rock->posC.vx = work->centerC.vx;
                    rock->posC.vy = work->centerC.vy;
                    rock->posC.vz = work->centerC.vz;
                    kk = i + 1;
                    rock->posC.vy -= fx_0fb_1_rockOffsets[i * 2] * 2 - (kk %= 7) * 0x140;
                    rock->posC.vz -= (kk << 10) + ex->btlfx_randCentered(0x200);
                    rock->posC.vx -= fx_0fb_1_rockOffsets[i * 2 + 1] * 2;
                    rock->speedA = ex->btlfx_randCentered(0x800) + 0x1000;
                }
                zz4++;
            }
            env->camera->points[3].vx = work->centerC.vx;
            env->camera->points[3].vy = work->centerC.vy;
            fx_0fb_1_rocksHeld = 1;
            env->camera->points[3].vz = work->centerC.vz;
            work->rot8C.vy = 0x800;
            work->rot8C.vx = 0;
            work->rot8C.vz = 0;
            work->pos74.vx = work->centerC.vx;
            work->pos74.vy = work->centerC.vy;
            work->pos74.vz = work->centerC.vz + 0x4000;
            work->pos74.vy += 0x2000;
        }
        break;
    case 1:
        glow = 0xAD;
        work->pos54.vy -= 0x96;
        work->pos54.vz += 0x12C;
        work->rot6C.vz += 0x10;
        t = (frame << 12) / 22;
        if (frame < 5) {
            ex->Hi_DrawEffModel(&work->rot6C, (VECTOR *)&work->pos54, (s32)&work->scale60, work->modelA8);
            work->rot6C.vz += 0x400;
            ex->Hi_DrawEffModel(&work->rot6C, (VECTOR *)&work->pos54, (s32)&work->scale60, work->modelA8);
            work->rot6C.vz -= 0x400;
        } else {
            s32 morph;

            morph = 0x1000 - ex->math_sin(((frame - 4) << 10) / 18);
            ex->Hi_DrawMorphEffModel(&work->rot6C, (VECTOR *)&work->pos54, (s32)&work->scale60, work->modelA8, work->modelAC, morph);
            work->rot6C.vz += 0x400;
            ex->Hi_DrawMorphEffModel(&work->rot6C, (VECTOR *)&work->pos54, (s32)&work->scale60, work->modelA8, work->modelAC, morph);
            work->rot6C.vz -= 0x400;
        }
        ex->btlfx_drawScreenTint((u32 *)(env->ot + 1), 1, t >> 8, t >> 7, t >> 6);
        if (frame >= 0x13) {
            s32 tint = ((frame - 0x12) * 0xFF) >> 2;
            ex->btlfx_drawScreenTint((u32 *)(env->ot + 1), 1, tint, tint, tint);
        }
        if (t >= 0x800) {
            fx_0fb_1_rockRgb = 0x1000 - (t - 0x800) * 2;
        }
        if (frame >= 0x16) {
            work->state0 = 2;
            entry->frame = -1;
            ex->btlfx_poolClear(entry->pool);
            ex->btlfx_initParticleSystem();
            work->pos54.vx = work->centerC.vx;
            work->rot6C.vx = 0;
            work->rot6C.vy = 0;
            work->rot6C.vz = 0;
            fx_0fb_1_rockRgb = 0x1000;
            work->rot34.vx = 0;
            work->scale94.vx = 0x800;
            work->rot34.vy = 0;
            work->rot34.vz = 0;
            work->summonScale28.vx = 0x1000;
            work->pos54.vy = work->centerC.vy;
            work->summonScale28.vy = 0x1000;
            work->pos54.vz = work->centerC.vz;
            work->pos14.vx = work->centerC.vx;
            work->scale94.vy = 0x800;
            work->scale94.vz = 0x800;
            work->pos14.vy = work->centerC.vy;
            work->pos14.vz = work->centerC.vz - 0x2555;
            work->summonPos1C.vz = work->centerC.vz - 0x2800;
            work->summonPos1C.vy = work->centerC.vy + 0x14C0;
            work->summonScale28.vz = 0x1000;
            work->pos14.vy += 0xCAC;
            ex->Hi_SetSummonMotion(((Fx0fb1Summon *)env->loader)->summon54->motion18C, ((Fx0fb1Summon *)env->loader)->summonModel60);
            ex->Hi_SetSummonMotFrame(((Fx0fb1Summon *)env->loader)->summonModel60, 0);
        }
        break;
    case 2:
        t = (frame << 12) / 24;
        tmp = t >> 2;
        zz5 = ex->math_cos(tmp);
        env->camera->points[3].vx = work->centerC.vx;
        domeBright = zz5;
        env->camera->points[3].vy = work->centerC.vy;
        glow = (t * 0xAA) >> 12;
        env->camera->points[3].vz = work->centerC.vz;
        env->camera->points[3].vy += ex->math_sin(tmp);
        trail = 0x1000;
        if (frame >= 4) {
            work->summonPos1C.vx = work->centerC.vx;
            ex->Hi_DrawSummonModel(&work->rot34, (VECTOR *)&work->summonPos1C, (s32)&work->summonScale28,
                                   ((Fx0fb1Summon *)env->loader)->summonModel60, 1);
            work->summonPos1C.vy += 0x10E;
            ex->Hi_DrawEffModel(&work->rot34, (VECTOR *)&work->summonPos1C, (s32)&work->scale94, work->modelA4);
            work->summonPos1C.vy -= 0x10E;
        }
        work->rot6C.vx += 0x20;
        work->rot6C.vy += 0x10;
        work->scale60.vx = work->scale60.vy = work->scale60.vz = 0xAAA - t / 4;
        ex->Hi_DrawEffModel(&work->rot6C, (VECTOR *)&work->pos54, (s32)&work->scale60, work->modelB0);
        work->rot6C.vz += 0x400;
        ex->Hi_DrawEffModel(&work->rot6C, (VECTOR *)&work->pos54, (s32)&work->scale60, work->modelB0);
        work->rot6C.vz -= 0x400;
        zz5 = ex->math_sin(tmp) + 0x2000;
        ex->btlfx_drawRings((u8 *)ex->btlfx_getFileData(entry->file, 8), &work->centerC, NULL, zz5, frame, 1, -1);
        if (frame < 4) {
            s32 zz5 = 0xFF - ((frame * 0xFF) >> 2);
            ex->btlfx_drawScreenTint((u32 *)(env->ot + 1), 1, zz5, zz5, zz5);
        }
        if (frame >= 0x18) {
            work->state0 = 3;
            fx_0fb_1_unitsRestored = 1;
            entry->frame = -1;
            for (zz6 = 0; zz6 < 4; zz6++) {
                work->rotC4[zz6].vx = 0x800 - zz6 * 0x180;
                work->rotC4[zz6].vy = zz6 * 0xC0;
                work->rotC4[zz6].vz = zz6 * 0x60;
            }
        }
        break;
    case 3: {
        VECTOR sc;
        s32 ca;
        s32 cb;

        t = (frame << 12) / 37;
        trail = ex->math_cos(t >> 2);
        gridGrow = (t >> 4) + 0x80;
        ex->Hi_DrawSummonModel(&work->rot34, (VECTOR *)&work->summonPos1C, (s32)&work->summonScale28, ((Fx0fb1Summon *)env->loader)->summonModel60, 0);
        ca = ((0x1000 - (t * 2) / 3) >> 5) - 0x80;
        ex->Hi_ModifySummonModelRGB(((Fx0fb1Summon *)env->loader)->summonModel60, ca, ca, ca);
        work->summonPos1C.vy += 0x10E;
        ex->Hi_DrawEffModel(&work->rot34, (VECTOR *)&work->summonPos1C, (s32)&work->scale94, work->modelA4);
        cb = ((0x1000 - t) >> 5) - 0x80;
        ex->Hi_ModifyEffModelRGB(work->modelA4, cb, cb, cb);
        glow = 0xAA;
        work->summonPos1C.vy -= 0x10E;
        work->rot6C.vx += 0x20;
        work->rot6C.vy += 0x10;
        if (frame < 0xF) {
            size = 0x6AA;
        } else if (frame < 0x17) {
            size = 0x6AA - ((frame - 0xF) * 0x6AA) / 8;
        } else {
            size = 0;
        }
        if (size != 0) {
            work->scale60.vx = work->scale60.vy = work->scale60.vz = size;
            ex->Hi_DrawEffModel(&work->rot6C, (VECTOR *)&work->pos54, (s32)&work->scale60, work->modelB0);
            work->rot6C.vz += 0x400;
            ex->Hi_DrawEffModel(&work->rot6C, (VECTOR *)&work->pos54, (s32)&work->scale60, work->modelB0);
            work->rot6C.vz -= 0x400;
        }
        size = size * 5 + 0x1000;
        ex->btlfx_drawRings((u8 *)ex->btlfx_getFileData(entry->file, 8), &work->centerC, NULL, size, frame, 1, -1);
        if ((u32)(frame - 0xF) < 6) {
            size = 0x1000 - (((frame - 0xF) << 12) / 6);
            ex->btlfx_drawRings((u8 *)ex->btlfx_getFileData(entry->file, 0xF), &work->pos14, NULL, size, frame, 1, 0x1000);
        }
        if (frame >= 0x11) {
            ph = ((frame - 0x11) << 10) / 20;
            size = ex->math_sin(ph) * 2 + 0x2000;
            k = ex->math_sin(ph);
            ex->btlfx_drawRings((u8 *)ex->btlfx_getFileData(entry->file, 0x1A), &work->pos14, NULL, size, 0, 1, k);
        }
        if (frame >= 0x15) {
            zz2 = ((frame - 0x14) << 12) / 17;
            phase = zz2 >> 2;
            sc.vx = sc.vy = sc.vz = zz2;
            for (size = 0; size < 4; size++) {
                work->rotC4[size].vz = (s16)(work->rotC4[size].vz - 0x18) - size * 0x30;
                ex->Hi_DrawEffModel(&work->rotC4[size], (VECTOR *)&work->pos54, (s32)&sc, work->modelB4);
                sc.vx = sc.vy = sc.vz = (sc.vx * 0x12) >> 4;
                ex->Hi_ModifyEffModelRGB(work->modelB4, ((ex->math_sin(phase) * 2) / 3 >> 5) - 0x80,
                                         ((ex->math_sin(phase) * 2) / 3 >> 5) - 0x80,
                                         ((ex->math_sin(phase) * 2) / 3 >> 5) - 0x80);
            }
        }
        if (frame == 0x18) {
            emitter = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, 0x15), (s32)&work->pos14);
            if (emitter != NULL) {
                emitter->otZ = 0x80;
            }
        }
        k = (ex->math_sin(t >> 2) * 0x46) >> 12;
        ex->btlfx_drawScreenTint((u32 *)(env->ot + 1), 1, k >> 2, k >> 1, k);
        if (frame >= 0x22) {
            k = ((frame - 0x21) * 0xFF) / 4;
            ex->btlfx_drawScreenTint((u32 *)(env->ot + 1), 1, k, k, k);
        }
        if (frame >= 0x15) {
            zz9 = frame - 0x14;
            k = ex->math_sin((zz9 << 10) / 17);
            ex->btlfx_drawSpriteLine((u8 *)ex->btlfx_getFileData(entry->file, 0xC), &work->centerC, zz9, k);
        }
        if (frame >= 0x25) {
            work->state0 = 4;
            entry->frame = -1;
            BTLFX_COPY_SVEC(&work->camTarget3C, &fx_0fb_1_origin);
            work->camTarget3C.vy += 0x400;
            env->camera->points[3].vx = work->camTarget3C.vx;
            env->camera->points[3].vy = work->camTarget3C.vy;
            env->camera->points[3].vz = work->camTarget3C.vz;
            work->pos54.vx = fx_0fb_1_origin.vx;
            work->pos54.vy = fx_0fb_1_origin.vy;
            work->pos54.vz = fx_0fb_1_origin.vz;
            BTLFX_COPY_SVEC(&work->centerC, &fx_0fb_1_origin);
            BTLFX_COPY_SVEC(&work->pos14, &fx_0fb_1_origin);
            emitter = work->emitE4 = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, 0x15), (s32)&work->pos14);
            if (emitter != NULL) {
                emitter->otZ = 0x80;
            }
        }
        break;
    }
    case 4:
        BTLFX_COPY_SVEC(&work->camTarget3C, &fx_0fb_1_origin);
        t = (frame << 12) / 10;
        work->camTarget3C.vy += (0x1000 - t) >> 2;
        env->camera->points[3].vx = work->camTarget3C.vx;
        glow = 0xF0;
        env->camera->points[3].vy = work->camTarget3C.vy;
        gridGrow = 0x300;
        env->camera->points[3].vz = work->camTarget3C.vz;
        work->rot6C.vx += 0x20;
        work->rot6C.vy += 0x10;
        if (frame < 0x10) {
            s32 unused2;
            s32 rgb;

            tmp = (frame << 12) >> 4;
            ringScale = ex->math_sin((frame << 12) >> 6) >> 1;
            work->scale60.vx = work->scale60.vy = work->scale60.vz = ringScale;
            ex->Hi_DrawEffModel(&work->rot6C, (VECTOR *)&work->pos54, (s32)&work->scale60, work->modelB0);
            rgb = ((0x1000 - tmp) >> 5) - 0x80;
            ex->Hi_ModifyEffModelRGB(work->modelB0, rgb, rgb, rgb);
        }
        env->otZ = 0x100;
        tmp = ex->math_cos(t >> 2);
        ringScale = ex->math_sin(t >> 2) + 0x400;
        ex->btlfx_drawRings((u8 *)ex->btlfx_getFileData(entry->file, 8), &work->centerC, NULL, ringScale, frame, 1, -1);
        ex->btlfx_drawRings((u8 *)ex->btlfx_getFileData(entry->file, 0xF), &work->centerC, NULL, ringScale, frame, 1, tmp);
        env->otZ = 0;
        if (frame < 6) {
            tint = 0xFF - ((frame * 0xFF) / 6);
            ex->btlfx_drawScreenTint((u32 *)(env->ot + 1), 1, tint, tint, tint);
        }
        if (frame >= 7) {
            tint = (frame - 7) * 0x55;
            ex->btlfx_drawScreenTint((u32 *)(env->ot + 1), 2, tint, tint, tint);
        }
        if (frame >= 0xA) {
            work->state0 = 5;
            entry->frame = -1;
            ex->btlfx_stopEmitter(work->emitE4);
            work->rot8C.vy = 0x400;
            work->rot6C.vy = 0x400;
            work->rot8C.vx = 0;
            work->rot8C.vz = 0;
            work->scale80.vx = 0x1000;
            work->scale80.vy = 0x1000;
            work->scale80.vz = 0x1000;
            work->rot6C.vx = 0;
            work->rot6C.vz = 0;
            work->scale60.vx = 0x333;
            work->scale60.vy = 0x333;
            work->scale60.vz = 0x333;
            work->pos54.vx = work->centerC.vx;
            work->pos54.vy = work->centerC.vy;
            work->pos54.vz = work->centerC.vz;
            emitter = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, 2), (s32)&work->pos14);
            if (emitter != NULL) {
                emitter->otZ = 0x80;
            }
            emitter = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, 5), (s32)&work->pos14);
            if (emitter != NULL) {
                emitter->otZ = 0x80;
            }
            ex->btlfx_drawScreenTint((u32 *)(env->ot + 1), 2, 0xFF, 0xFF, 0xFF);
        }
        break;
    case 5: {
        SVECTOR dir;
        CVECTOR col;
        SVECTOR rot;
        VECTOR sc;
        SVECTOR ang;
        SVECTOR at;
        SVECTOR ang2;
        SVECTOR at2;
        CVECTOR col2;
        SVECTOR rot2;
        VECTOR sc2;
        s32 orbitRadius;
        s32 trailScale;

        t = frame << 6;
        env->otZ = 0x100;
        pulseAng = t >> 2;
        phase = ex->math_cos(pulseAng);
        ringScale = ex->math_sin(pulseAng) + 0xC00;
        ex->btlfx_drawRings((u8 *)ex->btlfx_getFileData(entry->file, 8), &work->centerC, NULL, ringScale, frame, 1, 0xB86);
        ex->btlfx_drawRings((u8 *)ex->btlfx_getFileData(entry->file, 8), &work->centerC, NULL, ringScale / 2, frame, 1, -1);
        ex->btlfx_drawRings((u8 *)ex->btlfx_getFileData(entry->file, 6), &work->centerC, NULL, (ringScale * 2) / 3, frame, 1, phase);
        ringScale = ex->math_sin(pulseAng) * 2 + 0x3000;
        ex->btlfx_drawRings((u8 *)ex->btlfx_getFileData(entry->file, 0x1A), &work->centerC, NULL, ringScale, 0, 1, -1);
        env->otZ = 0;
        if (frame < 0x20) {
            spinAng = frame << 12;
            spinAng >>= 7;
            ringScale = (ex->math_sin(spinAng) * 3) >> 1;
            level = ex->math_cos(spinAng);
            dir.vx = 0x200;
            dir.vy = 0x280;
            dir.vz = work->frame4 * 32;
            dir.pad = 1;
            ex->btlfx_drawRings((u8 *)ex->btlfx_getFileData(entry->file, 0xD), &work->centerC, &dir, ringScale, 0, 1, level);
            dir.vy += 0x800;
            ex->btlfx_drawRings((u8 *)ex->btlfx_getFileData(entry->file, 0xD), &work->centerC, &dir, ringScale, 0, 1, level);
        }
        if (frame == 0) {
            ex->btlfx_drawScreenTint((u32 *)(env->ot + 1), 0xFF, 0xA0, 0, 0xFF);
        } else if (frame == 1) {
            ex->btlfx_drawScreenTint((u32 *)(env->ot + 1), 2, 0xFF, 0xFF, 0);
        } else if (frame < 6) {
            level = 0xFF - ((frame * 0xFF) / 6);
            ex->btlfx_drawScreenTint((u32 *)(env->ot + 1), 1, level >> 1, level >> 1, level);
        }
        work->rot8C.vz += 0x18;
        ex->btlfx_sampleGradient(fx_0fb_1_gradient, &col, frame);
        beamAng = t >> 2;
        fxm_drawTexCylinder(&work->centerC, &work->rot8C, (VECTOR *)&work->scale80, 0x1C00, 0x1800, &col, 0x1B, 0x3E80, 0, 0,
                      ((ex->math_sin(beamAng) * 0x58) >> 12) + 0x40, 0xFF);
        ex->btlfx_drawScrollMove(work->frame4 * 7, 0x2C0, 0x100, 0x20, 0x80, 0x360, 0x100);
        if (frame < 0x20) {
            work->rot6C.vz += 0xC;
            phase = 0x1000 - (frame << 7);
            ex->Hi_DrawEffModel(&work->rot6C, (VECTOR *)&work->pos54, (s32)&work->scale60, work->modelB8);
            ex->Hi_ModifyEffModelRGB(work->modelB8, ((ex->math_sin(phase >> 2) - 0x800) >> 5) - 0x80,
                                     ((ex->math_sin(phase >> 2) - 0x800) >> 5) - 0x80,
                                     ((ex->math_sin(phase >> 2) - 0x800) >> 5) - 0x80);
        }
        rot.vx = 0x980;
        rot.vy = 0x400;
        rot.vz = 0;
        sc.vx = sc.vy = sc.vz = ex->math_sin(beamAng) + 0x800;
        ex->Hi_DrawEffModel(&rot, (VECTOR *)&work->pos54, (s32)&sc, work->modelBC);
        ex->Hi_ModifyEffModelRGB(work->modelBC, (((ex->math_cos(beamAng) >> 1) + 0x800) >> 5) - 0x80,
                                 (((ex->math_cos(beamAng) >> 1) + 0x800) >> 5) - 0x80,
                                 (((ex->math_cos(beamAng) >> 1) + 0x800) >> 5) - 0x80);
        ex->btlfx_drawBackdrop((BtlBgLayers *)ex->btlfx_getFileData(entry->file, 0x11), work->frame4);
        ex->btlfx_drawScrollMove(work->frame4 * 10, 0x360, 0x180, 0x10, 0x40, 0x370, 0x180);
        sparkFade = (ex->math_cos(beamAng) >> 1) + 0x800;
        if (frame < 0x18) {
            tmp = ((ex->math_sin((frame << 10) / 24) * 0x76C) >> 12) + 0x200;
        } else {
            tmp = 0x96C;
        }
        ang.vx = (ex->math_sin(t >> 2) * 2) / 3;
        ang.vy = work->frame4 << 3;
        ang.vz = 0;
        for (zz8 = 0, spinAng = work->frame4 * 0xC; zz8 < 0x80; zz8++, spinAng += 0x20) {
            ex->btlfx_offsetPoint(&work->centerC, (s32)&at, 0, (XY32 *)&ang, tmp, spinAng);
            ex->btlfx_drawParticles((u8 *)ex->btlfx_getFileData(entry->file, 7), &at, NULL, 0x260E, 0, 1, sparkFade, 0, 0);
            ang.vx += 0x40;
            ang.vy -= 0x80;
        }
        trailScale = (ex->math_cos(t >> 2) >> 1) + 0x1000;
        if (frame < 0x18) {
            orbitRadius = (ex->math_sin((frame << 10) / 24) >> 2) + 0x6A4;
        } else {
            orbitRadius = 0xAA4;
        }
        for (strip = 0; strip < 2; strip++) {
            if (frame == 0) {
                ex->btlfx_drawTrail((u8 *)ex->btlfx_getFileData(entry->file, 0x14), (s32)&work->centerC, (s32 *)&work->centerC,
                                    -1, trailScale, ex->math_cos(t >> 2), 1, (u8 *)&fx_0fb_1_strips[strip], 1);
            }
            {
                SVECTOR *pp = fx_0fb_1_strips[strip].pts4;

                for (k = 0; k < 0x40; k++) {
                    pp->vy -= 0x40;
                    pp++;
                }
            }
        }
        ang2.vx = ex->math_sin(work->frame4 << 7) >> 4;
        ang2.vy = work->frame4 * 0x52 - 0x180;
        ang2.vz = 0;
        for (k = 0, orbitAng = work->frame4 * 0x5C; k < 2; k++, orbitAng += 0x800) {
            ex->btlfx_offsetPoint(&work->centerC, (s32)&at2, 0, (XY32 *)&ang2, orbitRadius, orbitAng);
            ex->btlfx_drawTrail((u8 *)ex->btlfx_getFileData(entry->file, 0x14), (s32)&at2, (s32 *)&work->centerC, 0, trailScale,
                                (ex->math_sin(t >> 1) >> 1) + 0x800, 1, (u8 *)&fx_0fb_1_strips[k], 1);
        }
        if (frame < 0x18) {
            level = ex->math_cos((frame << 10) / 24);
            ex->btlfx_drawSpriteLine((u8 *)ex->btlfx_getFileData(entry->file, 0x17), &work->centerC, frame, level);
        }
        if (frame >= 8) {
            for (sp = 0; sp < 3; sp++) {
                spark = ex->btlfx_poolSpawn((s16 *)entry->pool);
                if (spark != NULL) {
                    s32 dist;
                    s32 dirAng;
                    spark->kind0 = FX_0FB_1_SPARK;
                    BTLFX_COPY_SVEC(&spark->posC, &work->centerC);
                    dirAng = ex->btlfx_rand();
                    dist = (ex->btlfx_rand() & 0x1FF) + 0x180;
                    spark->posC.vx = (s16)(spark->posC.vx - 0x100) + ex->btlfx_randCentered(0x200);
                    spark->posC.vy += (ex->math_cos(dirAng) * dist) >> 12;
                    spark->posC.vz += (ex->math_sin(dirAng) * dist) >> 12;
                    spark->speedA = ex->btlfx_rand() % 70;
                    spark->velX4 = (ex->btlfx_rand() & 0xFFF) + 0x1000;
                }
            }
        }
        if (frame == 0x38) {
            emitter = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, 0x15), (s32)&work->pos14);
            if (emitter != NULL) {
                emitter->otZ = 0x80;
            }
        }
        if (frame >= 0x39) {
            fadeT = (frame - 0x38) << 12;
            level = fadeT >> 7;
            if (level >= 0x100) {
                level = 0xFF;
            }
            rot2.vz = 0x20;
            sc2.vy = sc2.vx = (fadeT >> 5) + 0x1000;
            sc2.vz = 0x1000;
            col2.r = col2.g = col2.b = level;
            rot2.vx = 0;
            rot2.vy = 0;
            ex->btlfx_drawScreenGrid(2, 0xA0, 0x70, 0x4D, 0x37, 4, 4, 2, 2, &rot2, &sc2, &col2, 1, 0);
        }
        if (frame >= 0x40) {
            work->state0 = 6;
            entry->frame = -1;
            ex->btlfx_poolClear(entry->pool);
            emitter = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, 5), (s32)&fx_0fb_1_origin);
            if (emitter != NULL) {
                emitter->otZ = 0x80;
            }
        }
        break;
    }
    case 6:
        BTLFX_COPY_SVEC(&work->camTarget3C, &fx_0fb_1_origin);
        env->camera->points[3].vx = work->camTarget3C.vx;
        env->camera->points[3].vy = work->camTarget3C.vy;
        t = frame << 8;
        env->camera->points[3].vz = work->camTarget3C.vz;
        env->otZ = 0x180;
        endScale = ex->math_cos(t >> 2) + 0x400;
        tmp = ex->math_cos(t >> 2);
        ex->btlfx_drawRings((u8 *)ex->btlfx_getFileData(entry->file, 8), &fx_0fb_1_origin, NULL, endScale, 0, 1, tmp);
        env->otZ = 0;
        tmp = (ex->math_cos(t >> 2) * 0xFF) >> 12;
        ex->btlfx_drawScreenTint((u32 *)(env->ot + 1), 1, tmp, tmp, tmp);
        if (frame >= 0x10) {
            ex->btlfx_clearEffModels();
            return 1;
        }
        break;
    }
    }
    if (domeBright != 0) {
        if (work->state0 == 0) {
            work->rot8C.vz += work->frame4 + 0x26;
        } else {
            work->rot8C.vz += 8;
        }
        ex->Hi_DrawEffModel(&work->rot8C, (VECTOR *)&work->pos74, (s32)&work->scale80, work->modelA0);
        if (domeBright != 0x1000) {
            s32 rgb = (domeBright >> 5) - 0x80;

            ex->Hi_ModifyEffModelRGB(work->modelA0, rgb, rgb, rgb);
        }
    }
    if (trail != 0) {
        if (work->state0 == 0) {
            scrollX = (10000 - (work->frame4 << 6)) % 240;
        } else {
            scrollX = (work->frame4 * 0xE) % 240;
        }
        ex->btlfx_drawSpriteAtOt(8, (u8 *)ex->btlfx_getFileData(entry->file, 1), 0xA0 - scrollX, 0xA0, 0, 3, trail);
        ex->btlfx_drawSpriteAtOt(8, (u8 *)ex->btlfx_getFileData(entry->file, 1), 0x280 - scrollX, 0xA0, 0, 3, trail);
    }
    if (glow != 0) {
        CVECTOR col;
        SVECTOR rot;
        VECTOR sc;

        col.r = col.g = col.b = glow;
        rot.vx = 0;
        rot.vy = 0;
        rot.vz = 0;
        sc.vy = sc.vx = gridGrow + 0x1020;
        sc.vz = 0x1000;
        ex->btlfx_drawScreenGrid(0x200, 0xA0, 0x70, 0x4D, 0x37, 4, 4, 2, 2, &rot, &sc, &col, 0, 0);
    }
    if (work->state0 < 2) {
        ex->btlfx_drawScrollMove(work->frame4 * 5, 0x2A0, 0x180, 0x10, 0x40, 0x2B0, 0x180);
    }
    work->frame4++;
    }
    return 0;
}

s32 fx_0fb_1_op81(s32 mode, Fx0fb1Work81 *work, Unk800F78A4 *env, Unk80100C78Entry *entry, s32 *args) {
    BtlfxExports *ex = BTLFX_EXPORTS(env);
    Fx0fb1Vec3 sc;
    s32 unit;
    s32 target;
    s32 unused;
    s32 ang;
    s32 radius;

    if (mode == 0) {
        *args = 0x1A8;
    } else if (mode == 1) {
        fx_0fb_1_env = env;
        work->unitCount1A0 = env->targets->targetCount;
        for (unit = 0; unit < work->unitCount1A0; unit++) {
            if (ex->btlfx_isEnemyUnit(unit) != 0) {
                ex->btlfx_getUnitHomePos(unit, (s16 *)&work->homePos60[unit]);
                if (work->unitCount1A0 >= 2) {
                    work->liftJitter1A4 = ex->btlfx_randCentered(0x200);
                    ang = (unit << 12) / work->unitCount1A0 + 0x200;
                    radius = ex->btlfx_randCentered(0x100) + 0x200;
                    setVector(&work->liftPosA0[unit], (ex->math_cos(ang) * radius) >> 12, work->liftJitter1A4 - 0x1000,
                              (ex->math_sin(ang) * radius) >> 12);
                } else {
                    work->liftJitter1A4 = 0;
                    setVector(&work->liftPosA0[unit], 0, -0x1000, 0);
                }
                ex->btlfx_setUnitPos(unit, (s16 *)&work->liftPosA0[unit]);
                ex->btlfx_getUnitRot(unit, (s16 *)&work->rotE0[unit]);
                BTLFX_COPY_SVEC(&work->curRot160[unit], &work->rotE0[unit]);
                work->spin120[unit].vx = ex->btlfx_randCentered(0x80);
                work->spin120[unit].vy = ex->btlfx_randCentered(0x80);
                work->spin120[unit].vz = ex->btlfx_randCentered(0x20);
                work->curRot160[unit].vx = ex->btlfx_rand();
                work->curRot160[unit].vy = ex->btlfx_rand();
                ex->btlfx_getUnitScale(unit, (VECTOR *)&work->scale0[unit]);
                sc.vx = work->scale0[unit].vx / 2;
                sc.vy = work->scale0[unit].vy / 2;
                sc.vz = work->scale0[unit].vz / 2;
                ex->btlfx_setUnitScale(unit, (s32 *)&sc);
            }
        }
    } else {
        for (target = 0; target < work->unitCount1A0; target++) {
            if (ex->btlfx_isEnemyUnit(target) != 0) {
                work->curRot160[target].vx += work->spin120[target].vx;
                work->curRot160[target].vy += work->spin120[target].vy;
                work->curRot160[target].vz += work->spin120[target].vz;
                ex->btlfx_setUnitRot(target, (s16 *)&work->curRot160[target]);
                if (fx_0fb_1_rockRgb != 0x1000) {
                    ex->btlfx_setUnitLight(target, fx_0fb_1_rockRgb >> 5);
                }
            }
        }
        if (fx_0fb_1_unitsRestored != 0) {
            for (target = 0; target < work->unitCount1A0; target++) {
                if (ex->btlfx_isEnemyUnit(target) != 0) {
                    ex->btlfx_setUnitRot(target, (s16 *)&work->rotE0[target]);
                    ex->btlfx_getUnitHomePos(target, (s16 *)&sc);
                    ex->btlfx_setUnitPos(target, (s16 *)&sc);
                    ex->btlfx_setUnitScale(target, (s32 *)&work->scale0[target]);
                }
            }
            return 1;
        }
    }
    return 0;
}

s32 fx_0fb_1_op82(s32 mode, Fx0fb1Work82 *work, Unk800F78A4 *env, Unk80100C78Entry *entry, s32 *args) {
    BtlfxExports *ex = BTLFX_EXPORTS(env);
    SVECTOR unitPos;
    SVECTOR rot;
    s32 frame;
    s32 target;
    s32 ang;
    u8 *data;
    BtlUnitList *units;

    if (mode == 0) {
        *args = 0x30;
    } else if (mode == 1) {
        units = env->targets;
        target = *args;
        fx_0fb_1_env = env;
        work->unitCount24 = units->targetCount;
        work->state0 = 0;
        if (target >= work->unitCount24) {
            return 1;
        }
        work->unit2C = target;
        ex->btlfx_getUnitHomePos(target, (s16 *)&work->pos4);
        ex->btlfx_getUnitRot(work->unit2C, (s16 *)&work->rotC);
        BTLFX_COPY_SVEC(&work->rot1C, &work->rotC);
        work->spinX14 = 0x2000 / (((ex->btlfx_rand() & 1) + 1) * 16);
        work->spinY16 = 0x2000 / (((ex->btlfx_rand() & 1) + 1) * 16);
        work->spinZ18 = 0x2000 / (((ex->btlfx_rand() & 1) + 1) * 16);
        work->lift28 = -0x1000 - (ex->btlfx_rand() & 0x3FF);
    } else {
        frame = entry->frame;
        if (work->state0 == 0) {
            ang = frame << 8;
            BTLFX_COPY_SVEC(&unitPos, &work->pos4);
            unitPos.vy += (work->lift28 * ex->math_cos(ang >> 2)) >> 12;
            ex->btlfx_setUnitPos(work->unit2C, (s16 *)&unitPos);
            work->rot1C.vx += work->spinX14;
            work->rot1C.vy += work->spinY16;
            work->rot1C.vz += work->spinZ18;
            ex->btlfx_setUnitRot(work->unit2C, (s16 *)&work->rot1C);
            if (frame >= 0x10) {
                ex->btlfx_setUnitRot(work->unit2C, (s16 *)&work->rotC);
                ex->btlfx_getUnitHomePos(work->unit2C, (s16 *)&unitPos);
                ex->btlfx_setUnitPos(work->unit2C, (s16 *)&unitPos);
                work->state0 = 1;
                entry->frame = -1;
            }
        } else {
            ang = (frame << 12) / 7;
            rot.vx = 0x400;
            rot.vz = 0;
            rot.vy = 0;
            rot.pad = 1;
            data = (u8 *)ex->btlfx_getFileData(entry->file, 0x12);
            ex->btlfx_drawRings(data, &work->pos4, &rot, 0x1000, frame, 1, ex->math_cos(ang >> 2));
            if (frame >= 7) {
                return 1;
            }
        }
    }
    return 0;
}
