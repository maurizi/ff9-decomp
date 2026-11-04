#include "fxmod/fx_0ed.h"

static void fxm_drawRibbonTrail(SVECTOR *posA, SVECTOR *posB, u32 n, CVECTOR *colorA, CVECTOR *colorB, s32 fade, Fx0edSeg *buf, s32 abr) {
    Unk800F78A4 *env = fx_0ed_env;
    CVECTOR colA;
    CVECTOR colB;
    s32 otz;
    BtlfxExports *ex = BTLFX_EXPORTS(env);
    POLY_G4 *prim;
    Fx0edSeg *seg;
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
    Unk800F78A4 *env = fx_0ed_env;
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

s32 fx_0ed_op80(s32 mode, Fx0edWork80 *work, Unk800F78A4 *env, Unk80100C78Entry *entry, s32 *args) {
    BtlfxExports *ex = BTLFX_EXPORTS(env);
    VECTOR pos;
    SVECTOR ptA;
    SVECTOR ptB;
    SVECTOR ptC;
    u8 *file;
    s32 state;
    s32 frame;
    s32 amp;
    s32 t;
    s32 ratio;
    s32 phase;
    s32 unused;
    s32 i;
    Pool6CEntry *emitter;
    SVECTOR *sv;

    if (mode == 0) {
        *args = 0x40;
    } else if (mode == 1) {
        work->state0 = 0;
        work->frame4 = 0;
        fx_0ed_env = env;
        ex->btlfx_getUnitPos(0, (s16 *)&work->pos8);
        ex->btlfx_getUnitRot(0, (s16 *)&work->rot10);
        work->rot10.vy &= 0xFFF;
        if ((u32)(work->rot10.vy - 0x400) > 0x800) {
            work->rot10.vy = 0;
        } else {
            work->rot10.vy = 0x800;
        }
        ex->btlfx_rotateOffset(&work->rot10, ex->btlfx_getUnitRadius(0) + 0x78, (s32)&pos);
        work->pos18.vx = work->pos8.vx + ((SVECTOR *)&pos)->vx;
        work->pos18.vy = work->pos8.vy + ((SVECTOR *)&pos)->vy;
        work->pos18.vz = work->pos8.vz + ((SVECTOR *)&pos)->vz;
        ex->btlfx_getUnitBodyPos(0, 0, (s16 *)&work->pos8);
        BTLFX_COPY_SVEC(&((SVECTOR *)&pos)[1], &work->pos18);
        ((SVECTOR *)&pos)[1].vy -= 0x140;
        env->camera->points[0].vx = ((SVECTOR *)&pos)[1].vx;
        env->camera->points[0].vy = ((SVECTOR *)&pos)[1].vy;
        env->camera->points[0].vz = ((SVECTOR *)&pos)[1].vz;
        ex->btlfx_rotateOffset(&work->rot10, -0xD2, (s32)&pos);
        ((SVECTOR *)&pos)[1].vx = work->pos18.vx + ((SVECTOR *)&pos)->vx;
        ((SVECTOR *)&pos)[1].vy = work->pos18.vy + ((SVECTOR *)&pos)->vy - 0x140;
        ((SVECTOR *)&pos)[1].vz = work->pos18.vz + ((SVECTOR *)&pos)->vz;
        env->camera->points[1].vx = ((SVECTOR *)&pos)[1].vx;
        env->camera->points[1].vy = ((SVECTOR *)&pos)[1].vy;
        env->camera->points[1].vz = ((SVECTOR *)&pos)[1].vz;
        ex->btlfx_rotateOffset(&work->rot10, -ex->btlfx_getUnitRadius(0), (s32)&pos);
        ((SVECTOR *)&pos)[1].vx = work->pos8.vx + ((SVECTOR *)&pos)->vx;
        ((SVECTOR *)&pos)[1].vy = ((SVECTOR *)&pos)->vy - 0x140;
        ((SVECTOR *)&pos)[1].vz = work->pos8.vz + ((SVECTOR *)&pos)->vz;
        env->camera->points[2].vx = ((SVECTOR *)&pos)[1].vx;
        env->camera->points[2].vy = ((SVECTOR *)&pos)[1].vy;
        env->camera->points[2].vz = ((SVECTOR *)&pos)[1].vz;
        file = (u8 *)ex->btlfx_getFileData(entry->file, FX_ROULETTE_SUMMON_MODEL_DATA);
        ex->btlfx_registerSummonModel((Unk800E9000 *)ex->btlfx_getFileData(entry->file, FX_ROULETTE_HORNED_FIGURE), (s32 *)&file, 1, &fx_0ed_summonModel);
        ex->Hi_SetSummonMotion(((Fx0edSummon *)env->loader)->summon54->motion180, ((Fx0edSummon *)env->loader)->summonModel60);
        work->scale34[0] = work->scale34[1] = work->scale34[2] = 0x1000;
        work->rot2C.vx = 0;
        work->rot2C.vz = 0;
        work->rot2C.vy = work->rot10.vy;
        ex->btlfx_initEffModels(fx_0ed_effModels, 1);
        work->model28 = (s16)ex->btlfx_registerEffectModel((Unk800E8F50 *)ex->btlfx_getFileData(entry->file, FX_ROULETTE_HORNED_FIGURE_2), 0);
    } else {
        state = work->state0;
        frame = entry->frame;
        amp = 0;
        switch (state) {
        case 0:
            ratio = (frame << 12) / 24;
            if (frame == 0) {
                emitter = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, FX_ROULETTE_BLUE_BUBBLES), (s32)&work->pos18);
                if (emitter != NULL) {
                    emitter->otZ = 0x40;
                }
                emitter = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, FX_ROULETTE_BLUE_BEAM_2), (s32)&work->pos18);
                if (emitter != NULL) {
                    emitter->trails = (s32)fx_0ed_op80Trails0;
                    emitter->trailSlots = 0x18;
                    emitter->trailStride = 0x3C;
                    emitter->otZ = 0x40;
                }
            }
            if (frame > 0) {
                s32 rise;

                pos.vx = work->pos18.vx;
                pos.vy = work->pos18.vy;
                pos.vz = work->pos18.vz;
                rise = ((0x1000 - ex->math_sin(ratio >> 2)) * 0x514) >> 12;
                pos.vy += rise;
                ex->Hi_SetEffModelSlice(work->model28, rise);
                ex->Hi_DrawSliceEffModel(&work->rot2C, &pos, (s32)work->scale34, work->model28);
                ex->Hi_ModifyEffModelRGB(work->model28, (((ratio >> 1) + 0x800) >> 5) - 0x80, (((ratio >> 1) + 0x800) >> 5) - 0x80,
                                         (((ratio >> 1) + 0x800) >> 5) - 0x80);
            }
            if (frame >= 0x18) {
                work->state0 = 1;
                entry->frame = -1;
            }
            break;
        case 1:
            pos.vx = work->pos18.vx;
            pos.vy = work->pos18.vy;
            pos.vz = work->pos18.vz;
            ratio = ((Fx0edSummon *)env->loader)->summonModel60;
            ex->Hi_SetSummonMotFrame(ratio, 0);
            ex->Hi_DrawSummonModel(&work->rot2C, &pos, (s32)work->scale34, ((Fx0edSummon *)env->loader)->summonModel60, 1);
            if (frame >= 8) {
                work->state0 = 2;
                entry->frame = -1;
                ex->Hi_SetSummonMotFrame(((Fx0edSummon *)env->loader)->summonModel60, 0);
            }
            break;
        case 2:
            t = frame << 12;
            /* Dead store: only its sign word survives. */
            amp = t / 30;
            pos.vx = work->pos18.vx;
            pos.vy = work->pos18.vy;
            pos.vz = work->pos18.vz;
            ex->Hi_DrawSummonModel(&work->rot2C, &pos, (s32)work->scale34, ((Fx0edSummon *)env->loader)->summonModel60, 1);
            if (frame == 0) {
                amp = -1;
            } else if (frame < 0x18) {
                amp = t / 24;
            } else {
                amp = 0x1000;
            }
            if (frame == 0x20) {
                ex->btlfx_drawScreenTint((u32 *)(env->ot + 2), 2, 0xFF, 0xFF, 0);
            }
            if (frame == 0x1F) {
                s32 ang;

                emitter = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file,
                                                           *(fx_0ed_facingBlocks + (((ang = work->rot10.vy & 0xFFF) - 0x400U > 0x800) !=
                                                                      ((ang = env->camera->yaw & 0xFFF) - 0x400U > 0x800)))),
                                      (s32)&work->pos8);
                if (emitter != NULL) {
                    emitter->otZ = ex->btlfx_addRadiusSixth(0, 0x40);
                }
            }
            if (frame == 0x21) {
                ex->btlfx_drawScreenTint((u32 *)(env->ot + 2), 1, 0xDC, 0, 0x20);
                emitter = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, FX_ROULETTE_GLOW_RINGS), (s32)&work->pos8);
                if (emitter != NULL) {
                    emitter->otZ = ex->btlfx_addRadiusSixth(0, 0x40);
                }
                emitter = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, FX_ROULETTE_RED_SPARKS), (s32)&work->pos8);
                if (emitter != NULL) {
                    emitter->otZ = ex->btlfx_addRadiusSixth(0, 0x40);
                }
            }
            if (frame == 0x26) {
                emitter = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, FX_ROULETTE_BLUE_BEAM), (s32)&work->pos18);
                if (emitter != NULL) {
                    emitter->trails = (s32)fx_0ed_op80Trails1;
                    emitter->trailSlots = 0x18;
                    emitter->trailStride = 0x44;
                    emitter->otZ = 0x40;
                }
                ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, FX_ROULETTE_WHITE_GLOW), (s32)&work->pos18);
            }
            ex->btlfx_drawBackdrop((BtlBgLayers *)ex->btlfx_getFileData(entry->file, FX_ROULETTE_BACKDROP), frame);
            if (frame >= 0x29) {
                work->state0 = 3;
                entry->frame = -1;
            }
            break;
        case 3:
            ratio = frame << 9;
            phase = ratio >> 2;
            work->scale34[0] = work->scale34[2] = ((0x1000 - ex->math_sin(phase)) * 0x1000) >> 12;
            pos.vx = work->pos18.vx;
            pos.vy = work->pos18.vy;
            pos.vz = work->pos18.vz;
            ex->Hi_SetSummonMotFrame(((Fx0edSummon *)env->loader)->summonModel60, 0x29 - frame);
            ex->Hi_DrawSummonModel(&work->rot2C, &pos, (s32)work->scale34, ((Fx0edSummon *)env->loader)->summonModel60, 1);
            amp = ex->math_cos(phase);
            ex->Hi_ModifySummonModelRGB(((Fx0edSummon *)env->loader)->summonModel60, (amp >> 5) - 0x80, (amp >> 5) - 0x80,
                                        (amp >> 5) - 0x80);
            if (frame >= 8) {
                work->state0 = 3;
                ex->btlfx_clearEffModels();
                return 1;
            }
            break;
        }
        if (amp != 0) {
            fxm_getSummonBonePoint(((Fx0edSummon *)env->loader)->summonModel60, 9, &fx_0ed_ribbonPt0, &ptA);
            fxm_getSummonBonePoint(((Fx0edSummon *)env->loader)->summonModel60, 9, &fx_0ed_ribbonPt1, &ptB);
            fxm_getSummonBonePoint(((Fx0edSummon *)env->loader)->summonModel60, 9, &fx_0ed_ribbonPt2, &ptC);
            if (amp < 0) {
                fxm_drawRibbonTrail(&ptA, &ptB, 0x3F8, &fx_0ed_ribbonRgb0, &fx_0ed_ribbonRgb1, 0, fx_0ed_ribbon0, 1);
                fxm_drawRibbonTrail(&ptB, &ptC, 0x3F8, &fx_0ed_ribbonRgb1, &fx_0ed_ribbonRgb2, 0, fx_0ed_ribbon1, 1);
            } else {
                fxm_drawRibbonTrail(&ptA, &ptB, 0x10, &fx_0ed_ribbonRgb0, &fx_0ed_ribbonRgb1, amp, fx_0ed_ribbon0, 1);
                fxm_drawRibbonTrail(&ptB, &ptC, 0x10, &fx_0ed_ribbonRgb1, &fx_0ed_ribbonRgb2, amp, fx_0ed_ribbon1, 1);
                BTLFX_COPY_SVEC(&work->unk20, &ptA);
                {
                    s32 ofs;
                    s32 ofs2;
                    SVECTOR *sv2;

                    sv = (SVECTOR *)fx_0ed_ribbon0;
                    for (i = 0, ofs = 8; i < 17; ofs += 16, i++) {
                        sv[i * 2].vy -= sv[i * 2].vy >> 6;
                        ((SVECTOR *)(ofs + (s32)sv))->vy -= ((SVECTOR *)(ofs + (s32)sv))->vy >> 4;
                    }
                    sv2 = (SVECTOR *)fx_0ed_ribbon1;
                    i = 0;
                    ofs2 = 8;
                    for (; i < 17; ofs2 += 16, i++) {
                        sv2[i * 2].vy -= sv2[i * 2].vy >> 4;
                        ((SVECTOR *)(ofs2 + (s32)sv2))->vy -= ((SVECTOR *)(ofs2 + (s32)sv2))->vy >> 3;
                    }
                }
            }
        }
        work->frame4++;
    }
    return 0;
}

s32 fx_0ed_op81(s32 mode, Fx0edWork81 *work, Unk800F78A4 *env, Unk80100C78Entry *entry, s32 *args) {
    BtlfxExports *ex = BTLFX_EXPORTS(env);
    union {
        SVECTOR v;
        CVECTOR c;
    } u;
    SVECTOR rot;
    VECTOR sc;
    s32 isParty;
    s32 frame;
    s32 status;
    s32 gridBright;

    if (mode == 0) {
        *args = 0x18;
    } else if (mode == 1) {
        work->endFrame4 = 0xA;
        work->state0 = 0;
        work->unitC = (BtlUnit *)-1;
        work->frame8 = 0;
        fx_0ed_env = env;
        env->state->signals[0] = 0;
    } else {
        frame = entry->frame;
        gridBright = 0;
        switch (work->state0) {
        case 0:
            do {
                status = ex->btlfx_nextUnitPos(0, (s16 *)&work->pos10, &work->unitC, &isParty);
                if (status == -2) {
                    work->unitC = (BtlUnit *)-1;
                }
            } while (status == -2 || status == -1);
            env->camera->points[0].vx = work->pos10.vx;
            env->camera->points[0].vy = work->pos10.vy;
            env->camera->points[0].vz = work->pos10.vz;
            u.v.vx = work->pos10.vx;
            u.v.vy = work->pos10.vy;
            u.v.vz = work->pos10.vz;
            if (u.v.vz < 0) {
                u.v.vz += 0x640;
            } else {
                u.v.vz -= 0x640;
            }
            u.v.vy -= 0x200;
            env->camera->points[1].vx = u.v.vx;
            env->camera->points[1].vy = u.v.vy;
            env->camera->points[1].vz = u.v.vz;
            ex->btlfx_playFileSound(3, entry->file, -1);
            if (++work->frame8 >= 0x10 && status == 1) {
                work->state0 = 2;
            } else {
                work->state0 = 1;
                entry->frame = -1;
            }
            if (work->frame8 < 0x10) {
                gridBright = work->frame8 * 100 / 16;
            } else {
                gridBright = 100;
            }
            break;
        case 1:
            env->otZ = 0x80;
            ex->btlfx_drawSprite((u8 *)ex->btlfx_getFileData(entry->file, FX_ROULETTE_TARGET_RING), &work->pos10, 0, 0xFF, -1);
            env->otZ = 0;
            if (work->frame8 < 0x10) {
                gridBright = work->frame8 * 100 / 16;
            } else {
                gridBright = 100;
            }
            if (frame >= work->endFrame4) {
                work->state0 = 0;
                work->endFrame4--;
            }
            break;
        case 2:
            env->state->signals[0] = 1;
            env->camera->paused = 0;
            return 1;
        }
        if (gridBright != 0) {
            sc.vx = 0x1000;
            sc.vy = 0x1000;
            sc.vz = 0x1000;
            u.c.r = u.c.g = u.c.b = gridBright;
            rot.vx = 0;
            rot.vy = 0;
            rot.vz = 0;
            ex->btlfx_drawScreenGrid(6, 0xA0, 0x70, 0x50, 0x38, 4, 4, 2, 2, &rot, &sc, &u.c, 1, 0);
        }
    }
    return 0;
}

INCLUDE_RODATA("asm/us/fx_0ed/nonmatchings/fx_0ed", fx_0ed_headerPtr);
