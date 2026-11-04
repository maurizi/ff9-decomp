#include "fxmod/fx_09a.h"

static void fxm_drawRibbonTrail(SVECTOR *posA, SVECTOR *posB, u32 n, CVECTOR *colorA, CVECTOR *colorB, s32 fade, Fx09aSeg *buf, s32 abr) {
    Unk800F78A4 *env = fx_09a_env;
    CVECTOR colA;
    CVECTOR colB;
    s32 otz;
    BtlfxExports *ex = BTLFX_EXPORTS(env);
    POLY_G4 *prim;
    Fx09aSeg *seg;
    u32 i;
    s32 fadeNear;
    s32 fadeFar;

    if (fade < 0) {
        fade = 0x1000;
    }
    if (n >= 1000) {
        n -= 999;
        for (i = 0; i < n; i++) {
            buf[i].a0.pad = 0;
        }
        return;
    }
    seg = &buf[(s32)(n - 1)];
    for (i = 0; i < n; i++) {
        BTLFX_COPY_SVEC(&seg[1].a0, &seg[0].a0);
        BTLFX_COPY_SVEC(&seg[1].b8, &seg[0].b8);
        seg--;
    }
    seg = buf;
    for (i = 0; i < n; i++, seg++) {
        if (!(seg[0].a0.pad & seg[1].a0.pad)) {
            break;
        }
    }
    BTLFX_COPY_SVEC(&buf->a0, posA);
    buf->a0.pad = 1;
    BTLFX_COPY_SVEC(&buf->b8, posB);
    n = i;
    prim = (POLY_G4 *)env->prim;
    env->prim = (s32)(prim + n);
    if (colorA == NULL) {
        colA.r = colA.g = colA.b = 0;
    } else {
        gte_DpqColorFar_nst(colorA, fade);
        gte_strgb(&colA);
    }
    if (colorB == NULL) {
        colB.r = colB.g = colB.b = 0;
    } else {
        gte_DpqColorFar_nst(colorB, fade);
        gte_strgb(&colB);
    }
    gte_SetRotMatrix(env->viewMatrix);
    gte_SetTransMatrix(env->viewMatrix);
    colA.cd = abr != 0xFF ? 0x3A : 0x38;
    colB.cd = abr != 0xFF ? 0x3A : 0x38;
    for (i = 0; i < n; i++, buf++, prim++) {
        gte_ldv3c(buf);
        fadeNear = 0x1000 - (i << 12) / n;
        gte_rtpt_b();
        fadeFar = 0x1000 - ((i + 1) << 12) / n;
        gte_stopz(&otz);
        if (otz != 0) {
            gte_stsxy3(&prim->x0, &prim->x1, &prim->x2);
            gte_avsz3();
            gte_stszotz(&otz);
            gte_ldv0(&buf[1].b8);
            gte_rtps();
            ex->btlfx_addPrimAbr(0x8000000, otz, prim, abr);
            gte_stsxy(&prim->x3);
            gte_DpqColorFar_nst(&colA, fadeNear);
            gte_strgb(&prim->r0);
            gte_DpqColorFar_nst(&colB, fadeNear);
            gte_strgb(&prim->r1);
            gte_DpqColorFar_nst(&colA, fadeFar);
            gte_strgb(&prim->r2);
            gte_DpqColorFar_nst(&colB, fadeFar);
            gte_strgb(&prim->r3);
        }
    }
}

static void fxm_getSummonBonePoint(s32 model, s32 bone, SVECTOR *in, SVECTOR *out) {
    Unk800F78A4 *env = fx_09a_env;
    MATRIX m;

    BTLFX_EXPORTS(env)->Hi_GetSummonBoneMatrix(model, bone, (BtlBone *)&m);
    gte_SetRotMatrix(&m);
    gte_SetTransMatrix(&m);
    gte_ldv0(in);
    gte_rtv0tr();
    gte_stlvnl(m.t);
    out->vx = m.t[0];
    out->vy = m.t[1];
    out->vz = m.t[2];
    gte_SetRotMatrix(env->viewMatrix);
    gte_SetTransMatrix(env->viewMatrix);
}

s32 fx_09a_op80(s32 mode, Fx09aWork *work, Unk800F78A4 *env, Unk80100C78Entry *entry, s32 *args) {
    BtlfxExports *ex = BTLFX_EXPORTS(env);
    VECTOR pos;
    SVECTOR ptA;
    SVECTOR ptB;
    SVECTOR ptC;
    s32 file;
    Pool6CEntry *emitter;
    s32 frame;
    s32 ribbonT;
    s32 t;
    s32 i;
    s32 fade;
    s32 rgb;
    SVECTOR *seg0;

    if (mode == 0) {
        *args = sizeof(Fx09aWork);
    } else if (mode == 1) {
        work->state0 = 1;
        work->frame4 = 0;
        fx_09a_env = env;
        file = ex->btlfx_getFileData(entry->file, FX_LV5_DEATH_MODEL_DATA);
        ex->btlfx_registerSummonModel((Unk800E9000 *)ex->btlfx_getFileData(entry->file, FX_LV5_DEATH_MODEL), &file, 1, &fx_09a_summonModel);
        work->unit8 = -1;
        work->unitsDoneC = -1;
        work->unitCount10 = env->targets->targetCount;
        env->state->signals[0] = 0;
    } else {
        frame = entry->frame;
        ribbonT = 0;
        switch (work->state0) {
        case 1:
            work->unitsDoneC++;
            work->unit8++;
            if (work->unitsDoneC >= work->unitCount10) {
                work->state0 = 4;
                entry->frame = -1;
                break;
            }
            ex->btlfx_getUnitPos(work->unit8, (s16 *)&work->pos14);
            ex->btlfx_getUnitRot(work->unit8, (s16 *)&work->rot1C);
            work->rot1C.vy &= 0xFFF;
            if (work->rot1C.vy - 0x400U > 0x800) {
                work->rot1C.vy = 0;
            } else {
                work->rot1C.vy = 0x800;
            }
            ex->btlfx_rotateOffset(&work->rot1C, ex->btlfx_getUnitRadius(work->unit8) + 0x78, (s32)&ptA);
            work->pos24.vx = work->pos14.vx + ptA.vx;
            work->pos24.vy = work->pos14.vy + ptA.vy;
            work->pos24.vz = work->pos14.vz + ptA.vz;
            ex->btlfx_getUnitBodyPos(work->unit8, 0, (s16 *)&work->pos14);
            BTLFX_COPY_SVEC(&ptB, &work->pos24);
            ptB.vy -= 0x140;
            env->camera->points[0].vx = ptB.vx;
            env->camera->points[0].vy = ptB.vy;
            env->camera->points[0].vz = ptB.vz;
            ex->btlfx_rotateOffset(&work->rot1C, -0xD2, (s32)&ptA);
            ptB.vx = work->pos24.vx + ptA.vx;
            ptB.vy = work->pos24.vy + ptA.vy - 0x140;
            ptB.vz = work->pos24.vz + ptA.vz;
            env->camera->points[1].vx = ptB.vx;
            env->camera->points[1].vy = ptB.vy;
            env->camera->points[1].vz = ptB.vz;
            ex->btlfx_rotateOffset(&work->rot1C, -ex->btlfx_getUnitRadius(0), (s32)&ptA);
            ptB.vx = work->pos14.vx + ptA.vx;
            ptB.vy = ptA.vy - 0x140;
            ptB.vz = work->pos14.vz + ptA.vz;
            env->camera->points[2].vx = ptB.vx;
            env->camera->points[2].vy = ptB.vy;
            env->camera->points[2].vz = ptB.vz;
            ex->Hi_SetSummonMotion(((Fx09aSummon *)env->loader)->summon54->motion180, ((Fx09aSummon *)env->loader)->model60);
            ex->Hi_SetSummonMotFrame(((Fx09aSummon *)env->loader)->model60, 6);
            work->scale3C[2] = 0x1000;
            work->scale3C[1] = 0x1000;
            work->scale3C[0] = 0x1000;
            work->rot34.vx = 0;
            work->rot34.vz = 0;
            work->rot34.vy = work->rot1C.vy;
            ex->btlfx_playFileSound(0xF, entry->file, -1);
            ex->btlfx_playFileSound(0x10, entry->file, -1);
            ex->btlfx_playFileSound(0x11, entry->file, -1);
            work->state0 = 2;
            entry->frame = -1;
            break;
        case 2:
            pos.vx = work->pos24.vx;
            pos.vy = work->pos24.vy;
            pos.vz = work->pos24.vz;
            ex->Hi_DrawSummonModel(&work->rot34, &pos, (s32)work->scale3C, ((Fx09aSummon *)env->loader)->model60, 1);
            t = frame << 12;
            /* Dead store: only its sign word survives. */
            ribbonT = t / 30;
            if (frame < 6) {
                rgb = (t / 6 >> 5) - 0x80;
                ex->Hi_ModifySummonModelRGB(((Fx09aSummon *)env->loader)->model60, rgb, rgb, rgb);
            }
            if (frame == 0) {
                ribbonT = -1;
            } else if (frame < 22) {
                ribbonT = t / 22;
            } else {
                ribbonT = 0x1000;
            }
            if (frame == 24) {
                ex->btlfx_playFileSound(0x12, entry->file, -1);
                ex->btlfx_playFileSound(0x13, entry->file, -1);
                ex->btlfx_playFileSound(0x14, entry->file, -1);
            }
            if (frame == 26) {
                ex->btlfx_drawScreenTint((u32 *)(env->ot + 2), 2, 0xFF, 0xFF, 0);
            }
            if (frame == 27) {
                s32 ang;

                emitter = (Pool6CEntry *)ex->btlfx_startEmitter(
                    ex->btlfx_getFileData(entry->file,
                                          *(fx_09a_hitBlocks + (((ang = work->rot1C.vy & 0xFFF) - 0x400U > 0x800) !=
                                                          ((ang = env->camera->yaw & 0xFFF) - 0x400U > 0x800)))),
                    (s32)&work->pos14);
                if (emitter != NULL) {
                    emitter->otZ = ex->btlfx_addRadiusSixth(work->unit8, 0x40);
                }
                ex->btlfx_playFileSound(0x15, entry->file, -1);
                ex->btlfx_playFileSound(0x16, entry->file, -1);
                ex->btlfx_playFileSound(0x17, entry->file, -1);
            }
            if (frame == 28) {
                ex->btlfx_drawScreenTint((u32 *)(env->ot + 2), 1, 0xDC, 0, 0x20);
                emitter = (Pool6CEntry *)ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, FX_LV5_DEATH_GREEN_GLOW), (s32)&work->pos14);
                if (emitter != NULL) {
                    emitter->otZ = ex->btlfx_addRadiusSixth(work->unit8, 0x40);
                }
                emitter = (Pool6CEntry *)ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, FX_LV5_DEATH_RED_BURST), (s32)&work->pos14);
                if (emitter != NULL) {
                    emitter->otZ = ex->btlfx_addRadiusSixth(work->unit8, 0x40);
                }
            }
            if (frame == 29) {
                ex->btlfx_execCmd(work->unit8);
            }
            if (frame >= 30) {
                work->state0 = 3;
                entry->frame = -1;
            }
            break;
        case 3:
            fade = (frame << 12) / 6;
            if (frame == 2) {
                emitter = (Pool6CEntry *)ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, FX_LV5_DEATH_BLUE_TRAIL), (s32)&work->pos24);
                if (emitter != NULL) {
                    emitter->trails = (s32)fx_09a_trails;
                    emitter->trailSlots = 0x18;
                    emitter->trailStride = 0x44;
                    emitter->otZ = 0x40;
                }
                ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, FX_LV5_DEATH_GREY_SMOKE), (s32)&work->pos24);
            }
            work->scale3C[0] = work->scale3C[2] = ((0x1000 - ex->math_sin(fade >> 2)) << 12) >> 12;
            pos.vx = work->pos24.vx;
            pos.vy = work->pos24.vy;
            pos.vz = work->pos24.vz;
            ex->Hi_SetSummonMotFrame(((Fx09aSummon *)env->loader)->model60, 41 - frame);
            ex->Hi_DrawSummonModel(&work->rot34, &pos, (s32)work->scale3C, ((Fx09aSummon *)env->loader)->model60, 1);
            ribbonT = ex->math_cos(fade >> 2);
            ex->Hi_ModifySummonModelRGB(((Fx09aSummon *)env->loader)->model60, (ribbonT >> 5) - 0x80, (ribbonT >> 5) - 0x80,
                                        (ribbonT >> 5) - 0x80);
            if (frame >= 6) {
                work->state0 = 1;
                entry->frame = -1;
            }
            break;
        case 4:
            if (frame >= 8) {
                env->state->signals[0] = 1;
                return 1;
            }
            break;
        }
        if (ribbonT != 0) {
            fxm_getSummonBonePoint(((Fx09aSummon *)env->loader)->model60, 9, &fx_09a_bonePtA, &ptA);
            fxm_getSummonBonePoint(((Fx09aSummon *)env->loader)->model60, 9, &fx_09a_bonePtB, &ptB);
            fxm_getSummonBonePoint(((Fx09aSummon *)env->loader)->model60, 9, &fx_09a_bonePtC, &ptC);
            if (ribbonT < 0) {
                fxm_drawRibbonTrail(&ptA, &ptB, 0x3F8, (CVECTOR *)&fx_09a_ribbonColA, (CVECTOR *)&fx_09a_ribbonColB, 0, fx_09a_ribbon0, 1);
                fxm_drawRibbonTrail(&ptB, &ptC, 0x3F8, (CVECTOR *)&fx_09a_ribbonColB, (CVECTOR *)&fx_09a_ribbonColC, 0, fx_09a_ribbon1, 1);
            } else {
                fxm_drawRibbonTrail(&ptA, &ptB, 0x10, (CVECTOR *)&fx_09a_ribbonColA, (CVECTOR *)&fx_09a_ribbonColB, ribbonT, fx_09a_ribbon0, 1);
                fxm_drawRibbonTrail(&ptB, &ptC, 0x10, (CVECTOR *)&fx_09a_ribbonColB, (CVECTOR *)&fx_09a_ribbonColC, ribbonT, fx_09a_ribbon1, 1);
                BTLFX_COPY_SVEC(&work->ribbonHead2C, &ptA);
                {
                    s32 off0;
                    s32 off1;
                    SVECTOR *seg1;

                    seg0 = (SVECTOR *)fx_09a_ribbon0;
                    for (i = 0, off0 = 8; i < 17; off0 += 16, i++) {
                        seg0[i * 2].vy -= seg0[i * 2].vy >> 6;
                        ((SVECTOR *)(off0 + (s32)seg0))->vy -= ((SVECTOR *)(off0 + (s32)seg0))->vy >> 4;
                    }
                    seg1 = (SVECTOR *)fx_09a_ribbon1;
                    i = 0;
                    off1 = 8;
                    for (; i < 17; off1 += 16, i++) {
                        seg1[i * 2].vy -= seg1[i * 2].vy >> 4;
                        ((SVECTOR *)(off1 + (s32)seg1))->vy -= ((SVECTOR *)(off1 + (s32)seg1))->vy >> 3;
                    }
                }
            }
        }
        ex->btlfx_drawBackdrop((BtlBgLayers *)ex->btlfx_getFileData(entry->file, FX_LV5_DEATH_BACKDROP), work->frame4);
        work->frame4++;
    }
    return 0;
}

INCLUDE_RODATA("asm/us/fx_09a/nonmatchings/fx_09a", fx_09a_headerPtr);
