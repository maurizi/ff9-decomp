#include "fxmod/fx_0ba.h"

static void fxm_drawCenterBand(u32 *ot, s32 tpage, s32 width, s32 r, s32 g, s32 b) {
    Unk800F78A4 *env = fx_0ba_env;
    POLY_G4 *prim;
    BtlfxExports *ex;
    s32 i;

    width >>= 1;
    prim = (POLY_G4 *)env->prim;
    ex = BTLFX_EXPORTS(env);
    env->prim = (u8 *)(prim + 4);
    for (i = 0; i < 4; i++, prim++) {
        setlen(prim, 8);
        prim->code = GPU_POLY_G4 | GPU_ABE;
        if (!(i & 1)) {
            prim->r0 = prim->r2 = 0;
            prim->g0 = prim->g2 = 0;
            prim->b0 = prim->b2 = b;
            prim->r1 = prim->r3 = r;
            prim->g1 = prim->g3 = g;
            prim->b1 = prim->b3 = b;
            prim->x0 = prim->x2 = 0xA0 - width;
            prim->x1 = prim->x3 = 0xA0;
        } else {
            prim->r1 = prim->r3 = 0;
            prim->g1 = prim->g3 = 0;
            prim->b1 = prim->b3 = b;
            prim->r0 = prim->r2 = r;
            prim->g0 = prim->g2 = g;
            prim->b0 = prim->b2 = b;
            prim->x0 = prim->x2 = 0xA0;
            prim->x1 = prim->x3 = width + 0xA0;
        }
        prim->y0 = prim->y1 = (i >> 1) * 0x78;
        prim->y2 = prim->y3 = prim->y0 + 0x78;
        ex->btlfx_linkPrimAbr(0x08000000, ot, prim, tpage);
    }
}

s32 fx_0ba_op80(s32 mode, Fx0baWork80 *work, Unk800F78A4 *env, Unk80100C78Entry *entry, s32 *args) {
    BtlfxExports *ex = BTLFX_EXPORTS(env);
    VECTOR pos;
    VECTOR mpos;
    VECTOR scale;
    s32 tint;
    s32 summonBright;
    s32 tint2;
    s32 fade;
    s32 bright;
    s32 rgb;
    s32 modelRgb;
    s32 summonRgb;
    Pool6CEntry *emitter;
    s16 *keys;
    s32 frame;
    s32 state;
    s32 t;
    s32 keyIdx;
    s32 key;

    if (mode == 0) {
        *args = sizeof(Fx0baWork80);
    } else if (mode == 1) {
        work->state0 = 0;
        work->frame4 = 0;
        fx_0ba_env = env;
        ex->btlfx_initEffModels(fx_0ba_effModels0, 6);
        work->model7C = ex->btlfx_registerEffectModel((Unk800E8F50 *)ex->btlfx_getFileData(entry->file, FX_RAMUH_CLOUD_PLANE), 0);
        work->model8C = ex->btlfx_registerEffectModel((Unk800E8F50 *)ex->btlfx_getFileData(entry->file, FX_RAMUH_THIN_ROD), 0);
        work->summonPos48.vx = 0;
        work->rot60.vy = 0x800;
        work->summonScale54.vx = 0x1000;
        work->summonScale54.vy = 0x1000;
        work->summonScale54.vz = 0x1000;
        work->summonPos48.vy = -0x1770;
        work->summonPos48.vz = -0xA00;
        work->rot60.vx = 0;
        work->rot60.vz = 0;
        fx_0ba_summonPos.vx = work->summonPos48.vx;
        fx_0ba_summonPos.vy = work->summonPos48.vy;
        fx_0ba_summonPos.vz = work->summonPos48.vz;
        work->rot68.vy = 0x600;
        work->rot68.vx = 0;
        work->rot68.vz = 0;
        work->scale70.vx = 0x6000;
        work->scale70.vy = 0x6000;
        work->scale70.vz = 0x6000;
        work->pos18.vx = work->summonPos48.vx;
        work->pos18.vy = work->summonPos48.vy;
        work->pos18.vz = work->summonPos48.vz;
        env->camera->points[3].vx = work->pos18.vx;
        env->camera->points[3].vy = work->pos18.vy;
        env->camera->points[3].vz = work->pos18.vz;
        env->camera->points[3].vy -= 0x190;
        fx_0ba_orbPos.vz = 0;
        fx_0ba_orbPos.vy = 0;
        fx_0ba_orbPos.vx = 0;
        work->unk96 = 0;
        work->unk94 = 0;
        work->unk9A = 0;
        work->unk98 = 0;
    } else {
        bright = 0;
        state = work->state0;
        frame = entry->frame;
        switch (state) {
        case 0:
            if (frame < 6) {
                bright = (frame << 12) / 6;
            } else {
                bright = 0x1000;
            }
            tint = 0;
            ex->btlfx_rangeProgress(frame, 0xC, 4, -1, 0x18, &tint);
            ex->btlfx_rangeProgress(frame, 0x14, 4, -1, 0x20, &tint);
            if (tint != 0) {
                ex->btlfx_drawScreenTint((u32 *)(env->ot + 0x100), 1, tint, tint, tint >> 1);
            }
            ex->btlfx_drawScrollMove(work->frame4 * 3, 0x240, 0x170, 0x38, 0x70, 0x240, 0x100);
            ex->btlfx_drawScrollMove(work->frame4 * 3, 0x280, 0x170, 0x38, 0x70, 0x280, 0x100);
            if (frame >= 0x20) {
                work->state0 = 1;
                entry->frame = -1;
                ex->Hi_SetSummonMotion(((Fx0baSummon *)env->loader)->summon54->motion180, ((Fx0baSummon *)env->loader)->model60);
                ex->Hi_SetSummonMotFrame(((Fx0baSummon *)env->loader)->model60, 0);
            }
            break;
        case 1:
            pos.vx = work->summonPos48.vx;
            pos.vy = work->summonPos48.vy;
            pos.vz = work->summonPos48.vz;
            t = (frame << 12) / 60 + 0x23;
            pos.vy += ex->math_sin(t * 2) >> 7;
            fx_0ba_summonPos.vx = pos.vx;
            fx_0ba_summonPos.vy = pos.vy;
            fx_0ba_summonPos.vz = pos.vz;
            ex->Hi_DrawSummonModel(&work->rot60, &pos, (s32)&work->summonScale54, ((Fx0baSummon *)env->loader)->model60, 0);
            ex->Hi_GetSummonBonePos(((Fx0baSummon *)env->loader)->model60, 0x33, (u16 *)&work->pos18);
            work->pos18.vy += 0x40;
            env->camera->points[2].vx = work->pos18.vx;
            env->camera->points[2].vy = work->pos18.vy;
            summonBright = 0;
            env->camera->points[2].vz = work->pos18.vz;
            ex->btlfx_rangeProgress(frame, 2, 7, -1, 0x555, &summonBright);
            ex->btlfx_rangeProgress(frame, 0xC, 2, 0, 0x1800, &summonBright);
            ex->btlfx_rangeProgress(frame, 0xE, 0xA, -1, 0x1800, &summonBright);
            ex->btlfx_rangeProgress(frame, 0x18, 2, 0, 0x2000, &summonBright);
            ex->btlfx_rangeProgress(frame, 0x1A, 0xF, -1, 0x2000, &summonBright);
            summonBright -= ex->math_cos(t >> 2) - 0x1000;
            ex->Hi_ModifySummonModelRGB(((Fx0baSummon *)env->loader)->model60, (summonBright >> 5) - 0x80, (summonBright >> 5) - 0x80,
                                        (summonBright >> 5) - 0x80);
            tint2 = 0;
            ex->btlfx_rangeProgress(frame, 0, 6, -1, 0xBE, &tint2);
            ex->btlfx_rangeProgress(frame, 0xC, 6, -1, 0x80, &tint2);
            ex->btlfx_rangeProgress(frame, 0x18, 0xA, -1, 0xF0, &tint2);
            bright = 0x1000;
            if (tint2 != 0) {
                ex->btlfx_drawScreenTint((u32 *)(env->ot + 0xC8), 1, tint2, tint2, tint2 / 3);
            }
            if (frame == 0xC) {
                ex->Hi_HideSummonModelMesh(((Fx0baSummon *)env->loader)->model60, 1);
                ex->Hi_HideSummonModelMesh(((Fx0baSummon *)env->loader)->model60, 2);
            }
            if (frame == 0x18) {
                ex->Hi_ShowSummonModelMesh(((Fx0baSummon *)env->loader)->model60, 1);
                ex->Hi_ShowSummonModelMesh(((Fx0baSummon *)env->loader)->model60, 2);
            }
            if (frame == 0x3B) {
                ex->Hi_SetSummonMotion(((Fx0baSummon *)env->loader)->summon54->motion184, ((Fx0baSummon *)env->loader)->model60);
                ex->Hi_SetSummonMotFrame(((Fx0baSummon *)env->loader)->model60, 0);
            }
            if (frame >= 0x3C) {
                if (frame == 0x3C) {
                    work->model84 = ex->btlfx_registerEffectModel((Unk800E8F50 *)ex->btlfx_getFileData(entry->file, FX_RAMUH_CLOUD_DISC), 1);
                    work->model88 = ex->btlfx_registerEffectModel((Unk800E8F50 *)ex->btlfx_getFileData(entry->file, FX_RAMUH_CLOUD_RING), 1);
                    work->pos10.vx = work->summonPos48.vx;
                    work->pos10.vy = work->summonPos48.vy;
                    work->pos10.vz = work->summonPos48.vz;
                    work->pos10.vy += 0x2F3;
                    work->pos10.vz += 0x400;
                    fx_0ba_orbPos.vx = work->pos10.vx;
                    fx_0ba_orbPos.vy = work->pos10.vy;
                    fx_0ba_orbPos.vz = work->pos10.vz;
                    emitter = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, FX_RAMUH_BLUE_FLASH), (s32)&work->pos10);
                    if (emitter != NULL) {
                        emitter->otZ = 0x37;
                    }
                    emitter = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, FX_RAMUH_STORM_CLOUDS), (s32)&work->pos10);
                    if (emitter != NULL) {
                        emitter->otZ = 0x37;
                    }
                    fx_0ba_preOrb = 0;
                }
                bright = 0;
                ex->btlfx_rangeProgress(frame, 0x3C, 0x23, 0, 0x1000, &fade);
                keys = (s16 *)ex->btlfx_getFileData(entry->file, FX_RAMUH_SUMMON_KEYS_5);
                keyIdx = frame - 0x3C;
                if (keyIdx >= 0x1E) {
                    keyIdx = 0x1D;
                }
                keys += keyIdx * 3 + 2;
                key = *keys;
                scale.vz = 0x1000;
                scale.vy = 0x1000;
                scale.vx = 0x1000;
                mpos.vx = work->summonPos48.vx;
                mpos.vy = work->summonPos48.vy;
                mpos.vz = work->summonPos48.vz;
                mpos.vy += 0x2F3;
                mpos.vz += 0x400;
                ex->Hi_DrawMorphEffModel(&work->rot68, &mpos, (s32)&scale, work->model84, work->model88, key);
                env->otZ = 0x100;
                ex->btlfx_drawRings((u8 *)ex->btlfx_getFileData(entry->file, FX_RAMUH_WHITE_RING), &work->pos10, NULL, fade * 3 + 0x1000,
                                  0, 1, ex->math_cos(fade >> 2));
                env->otZ = 0;
                summonRgb = ((fade * 3 / 4 + 0x400) >> 5) - 0x80;
                ex->Hi_ModifySummonModelRGB(((Fx0baSummon *)env->loader)->model60, summonRgb, summonRgb, summonRgb);
                ex->btlfx_drawScrollMove(10000 - work->frame4 * 7, 0x240, 0x170, 0x38, 0x70, 0x240, 0x100);
                ex->btlfx_drawScrollMove(10000 - work->frame4 * 7, 0x280, 0x170, 0x38, 0x70, 0x280, 0x100);
            } else {
                ex->btlfx_drawScrollMove(work->frame4 * 3, 0x240, 0x170, 0x38, 0x70, 0x240, 0x100);
                ex->btlfx_drawScrollMove(work->frame4 * 3, 0x280, 0x170, 0x38, 0x70, 0x280, 0x100);
            }
            if (frame >= 0x5F) {
                work->state0 = 2;
                entry->frame = -1;
                fx_0ba_orbPos.vz = 0;
                fx_0ba_orbPos.vy = 0;
                fx_0ba_orbPos.vx = 0;
                ex->Hi_SetSummonMotion(((Fx0baSummon *)env->loader)->summon54->motion188, ((Fx0baSummon *)env->loader)->model60);
                ex->Hi_SetSummonMotFrame(((Fx0baSummon *)env->loader)->model60, 0);
                work->pos18.vx = work->summonPos48.vx;
                work->pos18.vy = work->summonPos48.vy;
                work->pos18.vz = work->summonPos48.vz;
                work->pos18.vy -= 0x127;
                work->pos18.vz += 0x80;
                env->camera->points[2].vx = work->pos18.vx;
                env->camera->points[2].vy = work->pos18.vy;
                env->camera->points[2].vz = work->pos18.vz;
                work->model80 = ex->btlfx_registerEffectModel((Unk800E8F50 *)ex->btlfx_getFileData(entry->file, FX_RAMUH_CLOUD_PLANE_2), 1);
                work->model90 = ex->btlfx_registerEffectModel((Unk800E8F50 *)ex->btlfx_getFileData(entry->file, FX_RAMUH_CAPPED_COLUMN), 1);
            }
            break;
        case 2:
            pos.vx = work->summonPos48.vx;
            pos.vy = work->summonPos48.vy;
            pos.vz = work->summonPos48.vz;
            fx_0ba_summonPos.vx = pos.vx;
            fx_0ba_summonPos.vy = pos.vy;
            fx_0ba_summonPos.vz = pos.vz;
            ex->Hi_DrawSummonModel(&work->rot60, &pos, (s32)&work->summonScale54, ((Fx0baSummon *)env->loader)->model60, 0);
            work->summonPos48.vy -= 0x1000;
            ex->Hi_DrawEffModel(&work->rot68, (VECTOR *)&work->summonPos48, (s32)(VECTOR *)&work->scale70, work->model80);
            work->summonPos48.vy += 0x1000;
            if (frame == 0x14) {
                ex->Hi_GetSummonBonePos(((Fx0baSummon *)env->loader)->model60, 0x2F, (u16 *)&mpos);
                work->pos28.vx = ((SVECTOR *)&mpos)->vx;
                work->pos28.vy = ((SVECTOR *)&mpos)->vy;
                work->pos28.vz = ((SVECTOR *)&mpos)->vz;
                work->rot40.vx = 0x1DA;
                work->rot40.vy = 0;
                work->rot40.vz = 0;
                work->scale34.vz = 0x1000;
                work->scale34.vy = 0x1000;
                work->scale34.vx = 0x1000;
                ex->Hi_HideSummonModelMesh(((Fx0baSummon *)env->loader)->model60, 0);
            }
            if (frame >= 0x14) {
                work->pos28.vy += 0xAE;
                work->pos28.vz += 0x96;
                ex->Hi_DrawEffModel(&work->rot40, (VECTOR *)&work->pos28, (s32)&work->scale34, work->model8C);
                ex->Hi_DrawEffModel(&work->rot40, (VECTOR *)&work->pos28, (s32)&work->scale34, work->model90);
                modelRgb = ((((frame - 0x16) * 0x180) + 0x400) >> 5) - 0x80;
                ex->Hi_ModifyEffModelRGB(work->model90, modelRgb, modelRgb, modelRgb);
                ex->btlfx_drawScrollMove(work->frame4 * 5, 0x160, 0x100, 0x20, 0x80, 0x140, 0x100);
            }
            ex->btlfx_drawScrollMove(work->frame4 * 3, 0x240, 0x170, 0x38, 0x70, 0x240, 0x100);
            ex->btlfx_drawScrollMove(work->frame4 * 3, 0x240, 0x170, 0x38, 0x70, 0x240, 0x100);
            if (frame >= 0x1E) {
                work->state0 = state;
                fx_0ba_summonDone = 1;
                return 1;
            }
            break;
        }
        if (bright != 0) {
            ex->Hi_DrawEffModel(&work->rot68, (VECTOR *)&work->summonPos48, (s32)(VECTOR *)&work->scale70, work->model7C);
            rgb = (bright >> 5) - 0x80;
            ex->Hi_ModifyEffModelRGB(work->model7C, rgb, rgb, rgb);
        }
        work->frame4++;
    }
    return 0;
}

static void fxm_drawOffsetTrail(Unk80100C78Entry *entry, Fx0baStrip *trail, SVECTOR *from, SVECTOR *to, s32 amp, s32 ang, s32 dang,
                   s32 frame, s32 fade, s32 lineWidth) {
    BtlfxExports *ex = BTLFX_EXPORTS(fx_0ba_env);
    SVECTOR pos;
    SVECTOR rot;
    SVECTOR *pt;
    s32 i;
    s32 t;
    s32 offset;

    ex->btlfx_aimAngles((s16 *)from, (s16 *)to, (s16 *)&rot);
    pt = trail->pts4;
    trail->lastSlot0 = trail->count2 = 24;
    trail->head1 = 0;
    for (i = 0; i < 25; i++, pt++) {
        t = 0x1000 - (i << 12) / 24;
        t += ex->btlfx_randCentered(0x140);
        ex->btlfx_lerpVec((s16 *)from, (s16 *)to, t, (s16 *)&pos);
        offset = ex->btlfx_randCentered(0x80);
        offset = (ex->math_sin(t >> 1) * offset) >> 12;
        offset += 0x20 + ((ex->math_sin(t >> 1) * amp) >> 12);
        ex->btlfx_offsetPoint(&pos, (s32)pt, 0, (XY32 *)&rot, offset, ang);
        pt->pad = lineWidth;
        ang += dang + ex->btlfx_randCentered(0x100);
    }
    ex->btlfx_drawTrail((u8 *)ex->btlfx_getFileData(entry->file, FX_RAMUH_ORANGE_LIGHTNING), 0, NULL, frame, 0x1000, fade, 1, (u8 *)trail, 0);
}

s32 fx_0ba_op81(s32 mode, Fx0baPWork81 *work, Unk800F78A4 *env, Unk80100C78Entry *entry, s32 *args) {
    BtlfxExports *ex = BTLFX_EXPORTS(env);
    s32 alpha;
    s32 base;
    s32 lt;
    s32 type;
    s32 glowGrowY;
    s32 trailAlpha;
    s32 trailAmp;
    Fx0baPTrail17 *ta;
    Fx0baPTrail17 *tb;
    SVECTOR *pb;
    s16 *s1p;
    s16 *s2p;
    s32 pad;
    s16 *ribFrom;
    s32 rib;
    VECTOR pos2;
    VECTOR scale2;
    SVECTOR rot2;
    s32 arr60[3];
    s32 arr70[3];
    s32 arr80[3];
    s32 arr90[3];
    Fx0baPKey12 *key;
    Fx0baPTrail13 *tc;
    Fx0baPKeys *keys;
    Pool6CEntry *emitter;
    SVECTOR *pt;
    s32 frame;
    s32 k;
    s32 cw;
    s32 bandBlue;
    s32 shrink;
    s32 fadeT;
    s32 ch;
    s32 coreT;
    s32 ribFade;
    s32 trailPhase;
    s32 seg2;
    s32 keyOff2;
    SVECTOR *pt2;
    s16 *ribTo;
    s32 whiteTint;
    s32 blueTint;
    s32 fadeTint;
    s32 flashTint;
    s32 coreRgb;
    s32 c5b;
    s32 c6k;
    s32 ringT;
    s32 ribbonT;
    s32 kk;
    s32 yellowTint;
    s32 skyTint;
    s32 emitFade;
    s32 glowRgb;
    s32 flashRgb;
    s32 j;
    s32 seg;
    s32 keyOff;
    s32 t;
    s32 coreScale;
    s32 keyH;
    s32 ribFrame;
    s32 flashBright;
    s32 glowBright;
    s32 glowGrowXZ;
    s32 spE8;

    if (mode == 0) {
        *args = 0x84;
    } else if (mode == 1) {
        work->state0 = 0;
        work->frame4 = 0;
        fx_0ba_env = env;
        ex->btlfx_initEffModels(fx_0ba_effModels1, 5);
        work->model70 = ex->btlfx_registerEffectModel((Unk800E8F50 *)ex->btlfx_getFileData(entry->file, FX_RAMUH_ELECTRIC_BOWL), 1);
        work->model74 = ex->btlfx_registerEffectModel((Unk800E8F50 *)ex->btlfx_getFileData(entry->file, FX_RAMUH_GREEN_DOME), 0);
        work->model78 = ex->btlfx_registerEffectModel((Unk800E8F50 *)ex->btlfx_getFileData(entry->file, FX_RAMUH_THIN_ROD), 0);
        work->model80 = ex->btlfx_registerEffectModel((Unk800E8F50 *)ex->btlfx_getFileData(entry->file, FX_RAMUH_STAFF), 0);
        work->model7C = ex->btlfx_registerEffectModel((Unk800E8F50 *)ex->btlfx_getFileData(entry->file, FX_RAMUH_CAPPED_COLUMN), 1);
        work->pos8.vx = 0;
        work->pos8.vy = 0;
        work->pos8.vz = 0;
        FX0BAP_W(work->camAt18, 0) = FX0BAP_W(work->pos8, 0);
        FX0BAP_W(work->camAt18, 1) = FX0BAP_W(work->pos8, 1);
        work->camAt18.vy -= 0x1EA;
        env->camera->points[3].vx = work->camAt18.vx;
        env->camera->points[3].vy = work->camAt18.vy;
        env->camera->points[3].vz = work->camAt18.vz;
    } else {
        alpha = 0;
        base = 0;
        lt = 0;
        type = 0;
        glowGrowY = 0;
        frame = entry->frame;
        glowGrowXZ = glowBright = flashBright = 0;
        trailAlpha = 0;
        arr60[0] = arr60[1] = arr60[2] = 0;
        trailAmp = 0;
        switch (work->state0) {
        case 0: {
            key = (Fx0baPKey12 *)((u8 *)ex->btlfx_getFileData(entry->file, FX_RAMUH_MOTION_KEYS) + (frame * 12 + 4));
            work->pos48[0] = key->x + work->pos8.vx;
            work->pos48[1] = key->y + work->pos8.vy;
            work->pos48[2] = key->z + work->pos8.vz;
            work->rot60.vx = key->rx;
            work->rot60.vy = key->ry;
            work->rot60.vz = key->rz;
            work->scale54[0] = 0x2000;
            work->scale54[1] = 0x2000;
            work->scale54[2] = 0x2000;
            if (frame >= 7) {
                SVECTOR rot;

                ex->btlfx_rangeProgress(frame, 7, 8, 0, 0x1000, &spE8);
                env->otZ = -0x80;
                rot.vx = 0x400;
                rot.vy = 0;
                rot.vz = frame * 0x18;
                rot.pad = 1;
                ex->btlfx_drawRings((u8 *)ex->btlfx_getFileData(entry->file, FX_RAMUH_GREY_RING), &work->pos8, &rot,
                                  ex->math_sin(spE8 >> 2), 0, 1, ex->math_cos(spE8 >> 2));
                env->otZ = 0;
                glowBright = ex->math_cos(spE8 >> 2) >> 1;
                glowGrowXZ = ex->math_sin(spE8 >> 2);
                glowGrowY = ex->math_sin(spE8 >> 2) >> 2;
            } else {
                glowBright = (ex->math_cos((frame << 10) / 7) >> 1) + 0x800;
            }
            if (frame >= 15) {
                work->state0 = 1;
                entry->frame = -1;
            }
        } break;
        case 1:
            alpha = 0x1000;
            t = (frame << 12) / 9;
            base = ((0x1000 - ex->math_cos(t >> 2)) << 5) >> 12;
            type = (t >> 3) + 0xE00;
            if (frame < 4) {
                whiteTint = (ex->math_cos(frame << 8) * 0x46) >> 12;
                ex->btlfx_drawScreenTint((u32 *)(env->ot + 2), 2, whiteTint, whiteTint, whiteTint);
            }
            if (frame >= 5) {
                blueTint = ex->math_sin((frame - 5) << 8) << 7;
                ex->btlfx_drawScreenTint((u32 *)(env->ot + 2), 1, 0, blueTint >> 14, blueTint >> 12);
            }
            if (frame == 3) {
                ex->btlfx_startFadeBackground(1, 6);
            }
            if (frame >= 9) {
                work->state0 = 2;
                entry->frame = -1;
                ex->btlfx_drawScreenTint((u32 *)(env->ot + 2), 2, 0xFF, 0xFF, 0);
                ex->btlfx_startFadeBackground(-1, 2);
                work->emit6C = NULL;
            }
            break;
        case 2:
            t = (frame << 12) / 51;
            if (frame == 0) {
                ex->btlfx_startFadeBackground(-1, 1);
            }
            if (frame == 2) {
                ex->btlfx_startFadeBackground(0, 3);
            }
            if (frame < 4) {
                fadeTint = 0xFF - frame * 0xFF / 4;
                ex->btlfx_drawScreenTint((u32 *)(env->ot + 2), 1, fadeTint >> 2, fadeTint, fadeTint);
            }
            if (frame >= 16 && frame < 22) {
                if (frame == 16) {
                    ex->btlfx_drawScreenTint((u32 *)(env->ot + 2), 1, 0x80, 0x80, 0);
                }
                flashTint = 0xFF - (frame - 16) * 0xFF / 6;
                ex->btlfx_drawScreenTint((u32 *)(env->ot + 2), 1, 0, flashTint >> 2, flashTint);
            }
            trailAlpha = 0x1000;
            if (frame < 16) {
                trailAmp = (ex->math_cos(frame << 6) >> 3) + 0x12C;
            } else {
                trailAmp = 0x12C;
            }
            if (frame < 20) {
                flashBright = ex->math_cos((frame << 10) / 20);
            } else {
                flashBright = ((frame - 20) << 11) / 31;
                if (frame & 1) {
                    flashBright += 0x800;
                } else {
                    flashBright += 0x400;
                }
                if (ex->btlfx_rand() & 1) {
                    flashBright += 0x400;
                }
            }
            if (frame < 24) {
                coreT = (frame << 12) / 24;
                if (frame < 4) {
                    coreScale = ex->math_sin((frame << 10) >> 2) * 5 / 2;
                } else {
                    coreScale = (frame << 7) + 0x2600;
                }
                scale2.vx = scale2.vy = scale2.vz = coreScale;
                rot2.vz = 0;
                rot2.vx = 0;
                rot2.vy = frame * 16;
                pos2.vx = work->pos8.vx;
                pos2.vy = work->pos8.vy;
                pos2.vz = work->pos8.vz;
                ex->Hi_DrawEffModel(&rot2, &pos2, (s32)&scale2, work->model70);
                coreRgb = ((0x1000 - coreT) >> 5) - 0x80;
                ex->Hi_ModifyEffModelRGB(work->model70, coreRgb, coreRgb, coreRgb);
                if (frame < 16) {
                    ex->Hi_DrawEffModel(&rot2, &pos2, (s32)&scale2, work->model74);
                    c5b = ((0xC00 - (frame << 8)) >> 5) - 0x80;
                    ex->Hi_ModifyEffModelRGB(work->model74, c5b, c5b, c5b);
                }
            }
            if (frame > 0) {
                SVECTOR rot;

                keys = (Fx0baPKeys *)ex->btlfx_getFileData(entry->file, FX_RAMUH_LENS_FLASH_KEYS);
                c6k = frame + 9;
                if (c6k >= 36) {
                    keyH = *(s16 *)((u8 *)keys + (36 * 6 + 4)) * 2 / 3 + ((frame - 27) << 6);
                } else {
                    keys = (Fx0baPKeys *)((u8 *)keys + (c6k * 6 + 4));
                    keyH = keys->unk0 * 2 / 3;
                }
                FX0BAP_W(work->pos10, 0) = FX0BAP_W(work->pos8, 0);
                FX0BAP_W(work->pos10, 1) = FX0BAP_W(work->pos8, 1);
                if (frame < 16) {
                    work->pos10.vy -= (ex->math_sin((frame << 10) >> 4) * 0x56E) >> 12;
                } else {
                    work->pos10.vy -= 0x56E;
                }
                FX0BAP_W(fx_0ba_boltSrc, 0) = FX0BAP_W(work->pos10, 0);
                FX0BAP_W(fx_0ba_boltSrc, 1) = FX0BAP_W(work->pos10, 1);
                if (frame < 24) {
                    fx_0ba_boltLen = 0x1000;
                } else if (frame < 32) {
                    fx_0ba_boltLen = (keyH * 0x44C) >> 12;
                } else {
                    fx_0ba_boltLen = 0xB86;
                }
                env->otZ = 0x140;
                if (frame >= 15) {
                    ex->btlfx_drawParticles((u8 *)ex->btlfx_getFileData(entry->file, FX_RAMUH_LENS_FLASH), &work->pos10, NULL, keyH * 5 / 2,
                                            frame & 1, 1, ex->math_cos(t >> 2), 0, 0);
                }
                rot.vx = rot.vy = 0x200;
                rot.vz = frame * 0x2C;
                rot.pad = 0;
                ex->btlfx_drawRings((u8 *)ex->btlfx_getFileData(entry->file, FX_RAMUH_CYAN_RING), &work->pos10, &rot,
                                  ex->btlfx_randCentered(0x1000) + 0x4000, 0, 1, 0x1000);
                env->otZ = 0;
            }
            if (frame >= 20) {
                ringT = ((frame - 20) << 12) / 31;
                ex->btlfx_drawRings((u8 *)ex->btlfx_getFileData(entry->file, FX_RAMUH_BLUE_RING), &work->pos10, NULL, (ringT >> 2) + 0x2800, 0, 1,
                                  ex->math_sin(ringT >> 1));
            }
            if (frame == 20) {
                emitter = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, FX_RAMUH_BLUE_GLOW_RINGS), (s32)&work->pos10);
                work->emit6C = emitter;
                if (emitter != NULL) {
                    emitter->otZ = 0x96;
                }
            }
            if (frame == 14) {
                emitter = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, FX_RAMUH_BOLT_SPARK_BURST), (s32)&work->pos8);
                work->emit68 = emitter;
                if (emitter != NULL) {
                    emitter->otZ = -0x20;
                }
            }
            if (frame < 8) {
                ribbonT = frame << 9;
                alpha = 0x1000 - ribbonT;
                base = 0x24;
                ribbonT >>= 2;
                lt = ex->math_sin(ribbonT);
                type = 0x1000 - ribbonT;
            }
            {
                for (kk = 0; kk < 3; kk++) {
                    ribFrame = frame - kk * 3;
                    if (frame >= kk * 3 && ribFrame < 22) {
                        if (ribFrame < 6) {
                            arr60[kk] = 0x1000;
                            arr70[kk] = ribFrame * 2;
                            arr80[kk] = (ribFrame << 12) / 22;
                            arr90[kk] = 0x1000;
                        } else {
                            ribFade = 0x1000 - ((ribFrame - 6) << 8);
                            arr60[kk] = ribFade;
                            arr70[kk] = 11;
                            arr80[kk] = (ribFrame << 12) / 22;
                            arr90[kk] = ribFade;
                        }
                    }
                }
                ch = 2;
                cw = 4;
            }
            if (frame == 22) {
                ex->btlfx_drawScreenTint((u32 *)(env->ot + 1), 2, 0x80, 0x80, 0x20);
            }
            if (frame >= 23 && frame < 26) {
                yellowTint = 0x80 - ((frame - 23) << 6);
                ex->btlfx_drawScreenTint((u32 *)(env->ot + 1), 1, yellowTint, yellowTint, yellowTint >> 3);
            }
            if (frame == 24) {
                ex->btlfx_startFadeBackground(-1, 0);
            }
            if (frame == 25) {
                ex->btlfx_startFadeBackground(0, 5);
            }
            if (frame == 29) {
                ex->btlfx_drawScreenTint((u32 *)(env->ot + 1), 1, 0x20, 0x64, 0xFF);
            }
            if (frame == 41) {
                ex->btlfx_startFadeBackground(1, 8);
            }
            if (frame == 33) {
                ex->btlfx_drawScreenTint((u32 *)(env->ot + 1), 2, 0x40, 0x40, 0);
            }
            if (frame == 34) {
                ex->btlfx_drawScreenTint((u32 *)(env->ot + 1), 1, 0x40, 0x40, 0x40);
            }
            if (frame >= 41) {
                skyTint = (frame - 41) * 0xFF / 10;
                ex->btlfx_drawScreenTint((u32 *)(env->ot + 2), 1, skyTint >> 2, skyTint >> 1, skyTint);
            }
            if (frame >= 43) {
                CVECTOR col;
                SVECTOR rot;
                VECTOR sc;

                sc.vy = sc.vx = 0x1020;
                rot.vz = rot.vy = rot.vx = 0;
                sc.vz = 0x1000;
                col.r = col.g = col.b = ((frame - 39) << 5) / 12;
                ex->btlfx_drawScreenGrid(0x200, 0xA0, 0x78, 0x50, 0x3C, cw, cw, ch, ch, &rot, &sc, &col, 1, 0);
            }
            if (work->emit6C != NULL) {
                work->emit6C->scale = (t >> 1) + 0x1000;
                if (work->emit6C != NULL) {
                    work->emit6C->alpha = (0x1000 - (t >> 2)) * 3 / 4;
                }
            }
            if (frame >= 51) {
                work->state0 = 3;
                entry->frame = -1;
                ex->btlfx_startFadeBackground(-1, 6);
                emitter = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, FX_RAMUH_BOLT_SPARKS), (s32)&work->pos10);
                if (emitter != NULL) {
                    emitter->otZ = 0x96;
                }
            }
            break;
        case 3: {
            t = (frame << 12) / 20;
            if (frame < 12) {
                bandBlue = 0xFF - (frame << 5) / 12;
                fxm_drawCenterBand(env->ot + 2, 1, fx_0ba_bandWidths[frame], 0xC8 - frame * 0xC8 / 12, 0xC8 - frame * 0xC8 / 12,
                              bandBlue);
            }
            if (frame == 0) {
                ex->btlfx_drawScreenTint((u32 *)(env->ot + 1), 1, 0x20, 0xFF, 0xFF);
            }
            if (work->emit68 != NULL && frame < 10) {
                fadeT = (frame << 12) / 10;
                if (work->emit68->state == BTLFX_EMIT_DONE) {
                    work->emit68 = NULL;
                }
                if (work->emit68 != NULL) {
                    emitFade = 0x1000 - fadeT;
                    work->emit68->alpha = emitFade;
                    if (work->emit68 != NULL) {
                        work->emit68->scale = emitFade;
                    }
                }
                if (frame == 9) {
                    ex->btlfx_stopEmitter(work->emit68);
                }
            }
            if (work->emit6C != NULL && frame < 8) {
                shrink = frame << 9;
                if (work->emit6C->state == BTLFX_EMIT_DONE) {
                    work->emit6C = NULL;
                }
                if (work->emit6C != NULL) {
                    work->emit6C->scale = 0x1000 - shrink;
                }
                if (frame == 7) {
                    ex->btlfx_stopEmitter(work->emit6C);
                }
            }
            if (frame < 14) {
                CVECTOR col;
                SVECTOR rot;
                VECTOR sc;

                col.r = col.g = col.b = 0x89;
                sc.vz = sc.vy = sc.vx = 0x1000;
                rot.vz = rot.vy = rot.vx = 0;
                ex->btlfx_drawScreenGrid(2, 0xA0, 0x78, 0x50, 0x3C, 4, 4, 2, 2, &rot, &sc, &col, 0, 0);
            }
            key = (Fx0baPKey12 *)((u8 *)ex->btlfx_getFileData(entry->file, FX_RAMUH_MOTION_KEYS_2) + (frame * 12 + 4));
            work->pos48[0] = key->x + work->pos8.vx;
            work->pos48[1] = key->y + work->pos8.vy - 0x2EE;
            work->pos48[2] = key->z + work->pos8.vz;
            work->scale54[0] = key->rx * 2;
            work->scale54[1] = key->ry * 2;
            work->scale54[2] = key->rz * 2;
            work->rot60.vx = work->rot60.vy = work->rot60.vz = 0;
            if (frame >= 20) {
                work->state0 = 3;
                entry->frame = -1;
                ex->btlfx_clearEffModels();
                return 1;
            }
        } break;
        }
        {
            VECTOR pos;
            VECTOR sc;
            SVECTOR trailFrom;
            SVECTOR trailTo;

            pos.vx = work->pos48[0];
            pos.vy = work->pos48[1];
            pos.vz = work->pos48[2];
            pos.vy += ex->math_cos(work->frame4 * 0x70) >> 6;
            pos.vy += ex->math_sin(work->frame4 << 7) >> 6;
            ex->Hi_DrawEffModel(&work->rot60, &pos, (s32)work->scale54, work->model78);
            if (glowBright != 0) {
                sc.vx = work->scale54[0];
                sc.vy = work->scale54[1];
                sc.vz = work->scale54[2];
                sc.vx += glowGrowXZ;
                sc.vy += glowGrowY;
                sc.vz += glowGrowXZ;
                ex->Hi_DrawEffModel(&work->rot60, &pos, (s32)&sc, work->model7C);
                glowRgb = (glowBright >> 5) - 0x80;
                ex->Hi_ModifyEffModelRGB(work->model7C, glowRgb, glowRgb, glowRgb);
                ex->btlfx_drawScrollMove(work->frame4 * 5, 0x160, 0x100, 0x20, 0x80, 0x140, 0x100);
            }
            if (flashBright != 0) {
                ex->Hi_DrawEffModel(&work->rot60, &pos, (s32)work->scale54, work->model80);
                flashRgb = (((flashBright * 2) - 0x1000) >> 5) - 0x80;
                ex->Hi_ModifyEffModelRGB(work->model80, flashRgb, flashRgb, flashRgb);
            }
            if (trailAlpha != 0) {
                trailPhase = work->frame4 * 0x29E;
                trailFrom.vx = pos.vx;
                trailFrom.vy = pos.vy;
                trailFrom.vz = pos.vz;
                FX0BAP_W(trailTo, 0) = FX0BAP_W(trailFrom, 0);
                FX0BAP_W(trailTo, 1) = FX0BAP_W(trailFrom, 1);
                work->rot60.vx -= 0x400;
                ex->btlfx_pointAhead((s16 *)&trailFrom, (s16 *)&work->rot60, 0x5DC, (s16 *)&trailFrom);
                work->rot60.vx += 0x400;
                fxm_drawOffsetTrail(entry, (Fx0baStrip *)&fx_0ba_offsetTrail, &trailFrom, &trailTo, trailAmp, trailPhase, 0xF0, work->frame4 & 7, trailAlpha, 0x40);
            }
        }
        env->otZ = 0x1EA;
        if (alpha != 0) {
            for (k = 0; k < 3; k++) {
                pad = 0x168;
                s1p = (s16 *)ex->btlfx_getFileData(entry->file, fx_0ba_ribbonFromBlocks[k]) + 2;
                s2p = (s16 *)ex->btlfx_getFileData(entry->file, fx_0ba_ribbonToBlocks[k]) + 2;
                if (k != 0) {
                    pad = 0x80;
                    if (k == 1) {
                        pad = 0x58;
                    }
                }
                ta = &fx_0ba_ribbons[k * 2];
                pt = ta->pts4;
                ta->lastSlot0 = ta->count2 = 0x10;
                ta->head1 = 0;
                tb = &fx_0ba_ribbons[k * 2 + 1];
                tb->lastSlot0 = tb->count2 = 0x10;
                tb->head1 = 0;
                pb = tb->pts4;
                for (j = seg = 0; j < 34; j++, seg++) {
                    if (j == 17) {
                        pt = pb;
                        seg--;
                    }
                    keyOff = base - seg;
                    if (keyOff < 0) {
                        keyOff = 0;
                    }
                    keyOff *= 3;
                    pt->vx = work->pos8.vx + ex->btlfx_lerp(s1p[keyOff], s2p[keyOff], lt);
                    pt->vy = work->pos8.vy + ex->btlfx_lerp(s1p[keyOff + 1], s2p[keyOff + 1], lt);
                    pt->vz = work->pos8.vz + ex->btlfx_lerp(s1p[keyOff + 2], s2p[keyOff + 2], lt);
                    pt->pad = pad;
                    pt++;
                }
                ex->btlfx_drawTrail((u8 *)ex->btlfx_getFileData(entry->file, FX_RAMUH_LIGHTNING_RIBBON), 0, NULL, (work->frame4 + k) & 7, type, alpha,
                                    1, (u8 *)ta, 2);
                ex->btlfx_drawTrail((u8 *)ex->btlfx_getFileData(entry->file, FX_RAMUH_LIGHTNING_RIBBON_2), 0, NULL, (work->frame4 + k) & 7, type,
                                    alpha, 1, (u8 *)tb, 2);
            }
        }
        for (rib = 0; rib < 3; rib++) {
            if (arr60[rib] != 0) {
                ribFrom = (s16 *)ex->btlfx_getFileData(entry->file, fx_0ba_ribbon13FromBlocks[rib]) + 8;
                ribTo = (s16 *)ex->btlfx_getFileData(entry->file, fx_0ba_ribbon13ToBlocks[rib]);
                ribTo += 8;
                tc = &fx_0ba_ribbons13[rib];
                tc->lastSlot0 = tc->count2 = 0xC;
                tc->head1 = 0;
                pt2 = tc->pts4;
                for (seg2 = 0; seg2 < 13; seg2++) {
                    keyOff2 = arr70[rib] - seg2;
                    if (keyOff2 < 0) {
                        keyOff2 = 0;
                    }
                    keyOff2 *= 3;
                    pt2->vx = work->pos8.vx + ex->btlfx_lerp(ribFrom[keyOff2], ribTo[keyOff2], arr80[rib]) * 2;
                    pt2->vy = work->pos8.vy + ex->btlfx_lerp(ribFrom[keyOff2 + 1], ribTo[keyOff2 + 1], arr80[rib]) * 3 / 2;
                    pt2->vz = work->pos8.vz + ex->btlfx_lerp(ribFrom[keyOff2 + 2], ribTo[keyOff2 + 2], arr80[rib]) * 2;
                    pt2->pad = 0xC8;
                    pt2++;
                }
                ex->btlfx_drawTrail((u8 *)ex->btlfx_getFileData(entry->file, FX_RAMUH_LIGHTNING_BOLT), 0, (s32 *)&work->pos8,
                                    (work->frame4 + rib) & 7, arr90[rib], arr60[rib], 1, (u8 *)tc, 1);
            }
        }
        env->otZ = 0;
        work->frame4++;
    }
    return 0;
}

s32 fx_0ba_op82(s32 mode, void *work, Unk800F78A4 *env, Unk80100C78Entry *entry, s32 *args) {
    BtlfxExports *ex = BTLFX_EXPORTS(env);
    s16 *keys;
    SVECTOR camPt;
    s32 frame;
    s32 keyIdx;

    if (mode == 0) {
        *args = 4;
    } else if (mode == 1) {
        fx_0ba_env = env;
    } else {
        frame = entry->frame;
        keys = (s16 *)ex->btlfx_getFileData(entry->file, FX_RAMUH_CAMERA_KEYS);
        keyIdx = frame;
        if (keyIdx >= keys[1]) {
            keyIdx = keys[1] - 1;
        }
        keys = (s16 *)((u8 *)keys + (keyIdx * 6 + 4));
        camPt.vx = keys[0];
        camPt.vy = keys[1] - 100;
        camPt.vz = keys[2];
        env->camera->points[0].vx = camPt.vx;
        env->camera->points[0].vy = camPt.vy;
        env->camera->points[0].vz = camPt.vz;
        keys = (s16 *)ex->btlfx_getFileData(entry->file, FX_RAMUH_CAMERA_KEYS_2);
        keyIdx = frame;
        if (keyIdx >= keys[1]) {
            keyIdx = keys[1] - 1;
        }
        keys = (s16 *)((u8 *)keys + (keyIdx * 6 + 4));
        camPt.vx = keys[0];
        camPt.vy = keys[1] - 100;
        camPt.vz = keys[2];
        env->camera->points[1].vx = camPt.vx;
        env->camera->points[1].vy = camPt.vy;
        env->camera->points[1].vz = camPt.vz;
        if (frame >= 0x5A) {
            return 1;
        }
    }
    return 0;
}

static void fxm_drawJaggedTrail(Unk80100C78Entry *entry, s32 file, s32 mode, Fx0baStrip *trail, s32 size, s32 *noise, SVECTOR *from,
                   SVECTOR *to, s32 amp0, s32 amp1, s32 ang, s32 dang, s32 frame, s32 fade, s32 lineWidth, s32 dip) {
    Unk800F78A4 *env = fx_0ba_env;
    BtlfxExports *ex = BTLFX_EXPORTS(env);
    SVECTOR pos;
    SVECTOR rot;
    s32 i;
    s32 count;
    s32 t;
    s32 offset;
    SVECTOR *pt;

    ex->btlfx_aimAngles((s16 *)from, (s16 *)to, (s16 *)&rot);
    count = (size - 4) / 8;
    pt = trail->pts4;
    trail->lastSlot0 = trail->count2 = count - 1;
    trail->head1 = 0;
    for (i = 0; i < count; i++, pt++) {
        t = 0x1000 - (i << 12) / (count - 1);
        t += *noise++ * 2 / 3;
        ex->btlfx_lerpVec((s16 *)from, (s16 *)to, t, (s16 *)&pos);
        offset = *noise++;
        offset = (ex->math_sin(t >> 1) * offset) >> 12;
        offset += ex->btlfx_lerp(amp0, amp1, t);
        ex->btlfx_offsetPoint(&pos, (s32)pt, 0, (XY32 *)&rot, offset, ang);
        if (dip != 0) {
            pt->vy -= (ex->math_sin(t >> 1) * dip) >> 12;
        }
        pt->pad = lineWidth;
        ang += dang + *noise++;
    }
    env->otZ = 0x80;
    ex->btlfx_drawTrail((u8 *)ex->btlfx_getFileData(entry->file, file), 0, NULL, frame, 0x1000, fade, 1, (u8 *)trail, mode);
    env->otZ = 0;
}

static void fxm_drawJaggedTrailBranch(Unk80100C78Entry *entry, s32 file, s32 mode, Fx0baStrip *path, Fx0baStrip *trail, s32 size, s32 *noise,
                   s32 branchAt, s32 frame, s32 fade, s32 lineWidth) {
    SVECTOR *pt;
    BtlfxExports *ex;
    SVECTOR mid;

    pt = path->pts4;
    pt += branchAt;
    ex = BTLFX_EXPORTS(fx_0ba_env);
    ex->btlfx_lerpVec((s16 *)(pt + 1), (s16 *)pt, 0x4000, (s16 *)&mid);
    fxm_drawJaggedTrail(entry, file, mode, trail, size, noise, pt, &mid, 0, 0x20, ex->btlfx_rand(), 0x33, frame, fade, lineWidth, 0);
}

static s32 fx_0ba_op83PoolTask(s32 mode, Fx0baBolt *p, Unk800F78A4 *env, Unk80100C78Entry *entry, s32 frame) {
    BtlfxExports *ex = BTLFX_EXPORTS(env);
    SVECTOR tip;
    SVECTOR ang;
    Fx0baTrail *trail;
    Fx0baStrip4 *strip;
    s32 t;
    s32 fade;
    s32 jitterStep;
    s32 boltAmp;
    s32 ringT;
    s32 amp;
    s32 t2;
    Fx0baTrail *trail2;
    Fx0baStrip4 *strip2;
    s32 fade2;
    s32 jitterMul;
    s32 jitterOff;
    s32 ampProd; /* block-local, so 0x1000 - t2 and ampProd >> 11 are tied to it */

    if (mode != 1) {
        switch (p->kind11) {
        case 0:
        case 1:
            t = frame << 9;
            fade = ex->math_cos(t >> 2);
            trail = &fx_0ba_boltTrails[p->strip10];
            strip = &fx_0ba_boltBranches[p->strip10];
            if (p->kind11 == 0) {
                p->pos8.vx = fx_0ba_boltSrc.vx;
                p->pos8.vy = fx_0ba_boltSrc.vy;
                p->pos8.vz = fx_0ba_boltSrc.vz;
                fxm_drawJaggedTrail(entry, 0x1C, 0, (Fx0baStrip *)trail, 0x8C, p->jitter18, &p->pos0, &p->pos8, 0, 0x180,
                              frame * 0x97, 0x140, frame, fade, ((0x1000 - t) * p->amp15) >> 12,
                              ((((frame << 10) + t) << 7) >> 12) + 0x80);
            } else {
                if (frame == 0) {
                    jitterStep = 2;
                } else if (frame < 4) {
                    jitterStep = 3;
                } else {
                    jitterStep = 0;
                }
                boltAmp = ((0x1000 - t) * p->amp15) >> 12;
                if (frame == 0) {
                    boltAmp = (boltAmp * 3) >> 1;
                }
                fxm_drawJaggedTrail(entry, 2, 0, (Fx0baStrip *)trail, 0x8C, p->jitter18 + frame * jitterStep, &p->pos0, &p->pos8, 0,
                              0x180, frame * 0xC0, 0x168, frame, fade, boltAmp, 0);
                fxm_drawJaggedTrailBranch(entry, 3, 0, (Fx0baStrip *)trail, (Fx0baStrip *)strip, 0x24, p->jitter18, p->branchAt12, frame, fade,
                              boltAmp >> 2);
            }
            if (frame >= 8) {
                return 1;
            }
            break;
        case 2:
            t2 = frame << 9;
            fade2 = ex->math_cos(t2 >> 2);
            trail2 = &fx_0ba_boltTrails[p->strip10];
            strip2 = &fx_0ba_boltBranches[p->strip10];
            if (frame < 3 && fx_0ba_preOrb != 0) {
                ringT = ((frame + 1) << 12) / 3;
                ang.vy = 0x200;
                ang.vx = 0;
                ang.vz = 0;
                env->otZ = 0x400;
                ex->btlfx_drawRings((u8 *)ex->btlfx_getFileData(entry->file, FX_RAMUH_SPARK_RING), &p->pos0, NULL, (ringT >> 2) + 0x2000, 0, 1,
                                  0x1000 - ringT * 2 / 3);
                env->otZ = 0;
            }
            if ((u32)(frame - 3) < 3 && fx_0ba_preOrb != 0) {
                ringT = ((frame - 2) << 12) / 3;
                ang.vy = 0x200;
                ang.vx = 0;
                ang.vz = 0;
                env->otZ = 0x400;
                ex->btlfx_drawRings((u8 *)ex->btlfx_getFileData(entry->file, FX_RAMUH_SPARK_RING), &p->pos8, NULL, (ringT >> 2) + 0x2000, 0, 1,
                                  0x1000 - ringT * 2 / 3);
                env->otZ = 0;
            }
            if (frame < 2) {
                ex->btlfx_lerpVec((s16 *)&p->pos0, (s16 *)&p->pos8, (frame + 1) << 11, (s16 *)&tip);
                amp = p->amp15 >> 1;
            } else {
                BTLFX_COPY_SVEC(&tip, &p->pos8);
                amp = p->amp15;
            }
            jitterMul = (frame < 3) * 2;
            jitterOff = frame * jitterMul;
            /* No code (only jitterOff * 4 is used), but jitterOff is then set twice, so sched1 does not delay the multiply to its
             * use and the product stays in a caller-saved register. */
            jitterOff &= 0x3FFFFFFF;
            fxm_drawJaggedTrail(entry, 2, 0, (Fx0baStrip *)trail2, 0x8C, p->jitter18 + jitterOff, &p->pos0, &tip, 0,
                          0x227, frame * 0xC0, 0x168, frame, fade2, (ampProd = (0x1000 - t2) * amp) / 1500, 0);
            fxm_drawJaggedTrailBranch(entry, 3, 0, (Fx0baStrip *)trail2, (Fx0baStrip *)strip2, 0x24, p->jitter18, p->branchAt12, frame, fade2,
                          ampProd >> 11);
            if (frame >= 8) {
                return 1;
            }
            break;
        }
    }
    return 0;
}

static s32 fxm_randStripPoint32(SVECTOR *out) {
    BtlfxExports *ex = BTLFX_EXPORTS(fx_0ba_env);
    SVECTOR *src = fx_0ba_boltPoints[ex->btlfx_rand() % 32].points0;

    src += ex->btlfx_rand() % 17;
    out->vx = src->vx;
    out->vy = src->vy;
    out->vz = src->vz;
    out->pad = 1;
    return 0;
}

s32 fx_0ba_op83(s32 mode, Fx0baWork83 *work, Unk800F78A4 *env, Unk80100C78Entry *entry, s32 *args) {
    BtlfxExports *ex = BTLFX_EXPORTS(env);
    SVECTOR base;
    SVECTOR src;
    SVECTOR b;
    SVECTOR boltFrom;
    SVECTOR boltTo;
    s32 frame;
    Fx0baBolt *bolt;
    Fx0baPt *pt;
    s32 *tab;
    s32 i;
    s32 radius;
    s32 ang;
    s32 amp;
    s32 dist;
    Fx0baBolt *spark;
    s32 len;
    s32 j;
    s32 ptIdx;
    s32 kind;

    if (mode == 0) {
        ((EffInit *)args)->workSize = sizeof(Fx0baWork83);
        ((EffInit *)args)->elemSize = sizeof(Fx0baBolt);
        ((EffInit *)args)->elemFn = (s32)fx_0ba_op83PoolTask;
        ((EffInit *)args)->poolCount = 0x20;
    } else if (mode == 1) {
        kind = *args;
        fx_0ba_env = env;
        work->jitter0 = fx_0ba_boltJitter;
        tab = work->jitter0;
        for (i = 0; i < 64; i++) {
            *tab++ = ex->btlfx_randCentered(0xD2);
            *tab++ = ex->btlfx_randCentered(0x1C3);
            *tab++ = ex->btlfx_randCentered(0xDC);
        }
        work->kind8 = kind;
        work->boltCount4 = 0;
    } else {
        frame = entry->frame;
        switch (work->kind8) {
        case 0:
            BTLFX_COPY_SVEC(&base, &fx_0ba_boltSrc);
            if (frame < 0x14) {
                work->boltCount4++;
                BTLFX_COPY_SVEC(&src, &base);
                src.vy = 0;
                BTLFX_COPY_SVEC(&b, &base);
                bolt = ex->btlfx_poolSpawn((s16 *)entry->pool);
                if (bolt != NULL) {
                    radius = ex->btlfx_randCentered(0x200) + 0x7D0;
                    ang = ((work->boltCount4 / 2) << 10) - 0x200 + ex->btlfx_randCentered(0x200);
                    src.vx += (ex->math_cos(ang) * radius) >> 12;
                    src.vz += (ex->math_sin(ang) * radius) >> 12;
                    BTLFX_COPY_SVEC(&bolt->pos0, &src);
                    BTLFX_COPY_SVEC(&bolt->pos8, &b);
                    bolt->kind11 = 0;
                    bolt->strip10 = work->boltCount4 % 32;
                    bolt->jitter18 = work->jitter0 + (ex->btlfx_rand() & 0x1F);
                    bolt->branchAt12 = ex->btlfx_rand() % 15;
                    bolt->branchAt13 = ex->btlfx_rand() % 15;
                    bolt->amp15 = 0x3E;
                    bolt->unk14 = 0;
                }
                work->boltCount4++;
                BTLFX_COPY_SVEC(&src, &base);
                src.vy = 0;
                BTLFX_COPY_SVEC(&b, &base);
                if (frame >= 0xB) {
                    bolt = ex->btlfx_poolSpawn((s16 *)entry->pool);
                    if (bolt != NULL) {
                        radius = ex->btlfx_randCentered(0x100) + 0x3E8;
                        ang = ((work->boltCount4 / 2) << 10) + ex->btlfx_randCentered(0x200);
                        src.vx += (ex->math_cos(ang) * radius) >> 12;
                        src.vz += (ex->math_sin(ang) * radius) >> 12;
                        BTLFX_COPY_SVEC(&bolt->pos0, &src);
                        BTLFX_COPY_SVEC(&bolt->pos8, &b);
                        bolt->kind11 = 0;
                        bolt->strip10 = work->boltCount4 % 32;
                        bolt->jitter18 = work->jitter0 + (ex->btlfx_rand() & 0xF) * 3;
                        bolt->branchAt12 = ex->btlfx_rand() % 15;
                        bolt->branchAt13 = ex->btlfx_rand() % 15;
                        bolt->amp15 = 0x32;
                        bolt->unk14 = 0;
                    }
                }
            }
            if (frame >= 0x14) {
                return 1;
            }
            break;
        case 1:
            BTLFX_COPY_SVEC(&src, &fx_0ba_boltSrc);
            if (frame < 0x1D) {
                amp = (fx_0ba_boltLen * 100) / 2048 + 0x3C;
                if (amp >= 0x100) {
                    amp = 0xFF;
                }
                for (j = 0; j < 2; j++) {
                    work->boltCount4++;
                    BTLFX_COPY_SVEC(&boltFrom, &src);
                    BTLFX_COPY_SVEC(&boltTo, &src);
                    spark = ex->btlfx_poolSpawn((s16 *)entry->pool);
                    if (spark != NULL) {
                        len = fx_0ba_boltLen;
                        ex->btlfx_randRotation(&b);
                        ex->btlfx_pointAhead((s16 *)&boltTo, (s16 *)&b, 0x180, (s16 *)&boltFrom);
                        ex->btlfx_pointAhead((s16 *)&boltTo, (s16 *)&b, len + 0x180, (s16 *)&boltTo);
                        BTLFX_COPY_SVEC(&spark->pos0, &boltFrom);
                        BTLFX_COPY_SVEC(&spark->pos8, &boltTo);
                        spark->kind11 = 1;
                        spark->strip10 = work->boltCount4 % 32;
                        spark->jitter18 = work->jitter0 + (ex->btlfx_rand() & 0x1F);
                        spark->branchAt12 = ex->btlfx_rand() % 15;
                        spark->branchAt13 = ex->btlfx_rand() % 15;
                        spark->amp15 = amp;
                        spark->unk14 = 0;
                    }
                }
                if (frame >= 0xF) {
                    for (j = 0; j < 2; j++) {
                        work->boltCount4++;
                        BTLFX_COPY_SVEC(&boltFrom, &src);
                        BTLFX_COPY_SVEC(&boltTo, &src);
                        spark = ex->btlfx_poolSpawn((s16 *)entry->pool);
                        if (spark != NULL) {
                            len = fx_0ba_boltLen;
                            ex->btlfx_randRotation(&b);
                            ex->btlfx_pointAhead((s16 *)&boltTo, (s16 *)&b, 0x180, (s16 *)&boltFrom);
                            ex->btlfx_pointAhead((s16 *)&boltTo, (s16 *)&b, len + 0x180, (s16 *)&boltTo);
                            BTLFX_COPY_SVEC(&spark->pos0, &boltFrom);
                            BTLFX_COPY_SVEC(&spark->pos8, &boltTo);
                            spark->kind11 = 1;
                            spark->strip10 = work->boltCount4 % 32;
                            spark->jitter18 = work->jitter0 + (ex->btlfx_rand() & 0xF) * 3;
                            spark->branchAt12 = ex->btlfx_rand() % 15;
                            spark->branchAt13 = ex->btlfx_rand() % 15;
                            spark->amp15 = amp - 0x32;
                            spark->unk14 = 0;
                        }
                    }
                }
            }
            if (frame >= 0x1D) {
                return 1;
            }
            break;
        case 2:
            if ((fx_0ba_preOrb != 0 || !(frame & 1)) && fx_0ba_summonDone == 0) {
                do {
                    ptIdx = ex->btlfx_rand() % 10;
                    pt = (Fx0baPt *)((u8 *)ex->btlfx_getFileData(entry->file, FX_RAMUH_BOLT_POINTS) + 4) + ptIdx;
                    b.vx = -pt->vx + fx_0ba_summonPos.vx;
                    b.vy = pt->vy + fx_0ba_summonPos.vy;
                    b.vz = -pt->vz + fx_0ba_summonPos.vz;
                    if ((fx_0ba_orbPos.vx != 0 || fx_0ba_orbPos.vy != 0 || fx_0ba_orbPos.vz != 0) && (frame & 1)) {
                        BTLFX_COPY_SVEC(&boltFrom, &fx_0ba_orbPos);
                        b.vz >>= 1;
                        b.vx >>= 1;
                        b.vy += 0x400;
                    } else {
                        ptIdx = ex->btlfx_rand() % 10;
                        pt = (Fx0baPt *)((u8 *)ex->btlfx_getFileData(entry->file, FX_RAMUH_BOLT_POINTS_2) + 4) + ptIdx;
                        boltFrom.vx = -pt->vx + fx_0ba_summonPos.vx;
                        boltFrom.vy = pt->vy + fx_0ba_summonPos.vy;
                        boltFrom.vz = -pt->vz + fx_0ba_summonPos.vz;
                    }
                    dist = (b.vx - boltFrom.vx) * (b.vx - boltFrom.vx) + (b.vy - boltFrom.vy) * (b.vy - boltFrom.vy) +
                           (b.vz - boltFrom.vz) * (b.vz - boltFrom.vz);
                } while ((u32)(dist - 0x1000000) > 0x4000FFF);
                work->boltCount4++;
                bolt = ex->btlfx_poolSpawn((s16 *)entry->pool);
                if (bolt != NULL) {
                    BTLFX_COPY_SVEC(&bolt->pos0, &b);
                    BTLFX_COPY_SVEC(&bolt->pos8, &boltFrom);
                    bolt->kind11 = 2;
                    bolt->strip10 = work->boltCount4 % 32;
                    bolt->jitter18 = work->jitter0 + (ex->btlfx_rand() & 0x1F);
                    bolt->branchAt12 = ex->btlfx_rand() % 15;
                    bolt->branchAt13 = ex->btlfx_rand() % 15;
                    bolt->amp15 = ex->btlfx_rand() % 70 + 0x37;
                    bolt->unk14 = 0;
                }
            }
            if (fx_0ba_summonDone != 0) {
                return 1;
            }
            break;
        }
    }
    return 0;
}

static s32 fx_0ba_op85PoolTask(s32 mode, Fx0baPart *p, Unk800F78A4 *env, Unk80100C78Entry *entry, s32 frame) {
    BtlfxExports *ex = BTLFX_EXPORTS(env);
    s32 bias[2];
    s32 fade;
    u8 *pt;
    s32 (*fn)(Pool6CEntry *, s32, SVECTOR *, SVECTOR *);

    if (mode == 1) {
        p->unk12 = ex->btlfx_randCentered(0x20) + 0x1080;
    } else {
        fade = 0x1000 - (frame << 12) / 5;
        pt = p->point18;
        if (frame != 0) {
            fade >>= 1;
        }
        ((s16 *)pt)[3] = (0x16 - frame * 4) >> 1;
        fn = ex->btlfx_stepModuleParticle;
        bias[0] = 0;
        bias[1] = 0;
        fn((Pool6CEntry *)p, 0, (SVECTOR *)bias, (SVECTOR *)pt);
        if (p->pointIdx16 == 0) {
            p->point18[-0x10] = p->point18[-0xE] = 0x10;
            p->point18[-0xF] = 0;
            ex->btlfx_drawTrail((u8 *)ex->btlfx_getFileData(entry->file, FX_RAMUH_POOL_LIGHTNING), 0, NULL, frame, 0x1000, fade, 1,
                              p->point18 - 4, 0);
        }
        if (frame >= 5) {
            return 1;
        }
    }
    return 0;
}

static void fxm_arcTrailReset(void) {
    fx_0ba_arcTrailNext = 0;
}

static void fxm_arcTrailSpawn(Unk80100C78Entry *entry, SVECTOR *center, s32 radius, SVECTOR *from, SVECTOR *to) {
    Fx0baTrail *trail = &fx_0ba_arcTrails[fx_0ba_arcTrailNext];
    SVECTOR *pt = &trail->pts4[16];
    BtlfxExports *ex = BTLFX_EXPORTS(fx_0ba_env);
    SVECTOR pos;
    Fx0baPart *part;
    s32 i;
    s32 t;

    trail->lastSlot0 = trail->count2 = 16;
    trail->head1 = 0;
    for (i = 0; i < 17; i++, pt--) {
        t = 0x1000 - (i << 8);
        ex->btlfx_lerpVec((s16 *)from, (s16 *)to, t, (s16 *)&pos);
        pos.vx += 0x20 + ex->btlfx_randCentered(0x10);
        pos.vy += 0x46 + ex->btlfx_randCentered(0x20);
        ex->btlfx_pointAhead((s16 *)center, (s16 *)&pos, radius + ex->btlfx_randCentered(0x10) + (ex->math_sin(t >> 1) >> 6),
                          (s16 *)pt);
        part = ex->btlfx_poolSpawn((s16 *)entry->pool);
        if (part != NULL) {
            part->trail14 = fx_0ba_arcTrailNext;
            part->pointIdx16 = 0x10 - i;
            part->point18 = (u8 *)pt;
            part->posX0 = pt->vx << 12;
            part->posY4 = pt->vy << 12;
            part->posZ8 = pt->vz << 12;
            part->velXC = 0;
            part->velYE = 0;
            part->velZ10 = 0;
            part->velXC = ((pt->vx - center->vx) >> 5) + ex->btlfx_randCentered(0xE);
            part->velYE = ((pt->vy - center->vy) >> 5) + ex->btlfx_randCentered(0x18);
            part->velZ10 = ((pt->vz - center->vz) >> 5) + ex->btlfx_randCentered(0xE);
        }
    }
    fx_0ba_arcTrailNext = (fx_0ba_arcTrailNext + 1) % 5;
}

s32 fx_0ba_op85(s32 mode, Fx0baWork85 *work, Unk800F78A4 *env, Unk80100C78Entry *entry, s32 *args) {
    BtlfxExports *ex = BTLFX_EXPORTS(env);
    SVECTOR from;
    SVECTOR center;

    if (mode == 0) {
        ((EffInit *)args)->workSize = sizeof(Fx0baWork85);
        ((EffInit *)args)->elemSize = sizeof(Fx0baPart);
        ((EffInit *)args)->elemFn = (s32)fx_0ba_op85PoolTask;
        ((EffInit *)args)->poolCount = 0x55;
    } else if (mode == 1) {
        fx_0ba_env = env;
        fxm_arcTrailReset();
        work->rot8.vx = 0;
        work->rot8.vy = 0;
        work->rot8.vz = 0;
    } else {
        if (fx_0ba_summonDone == 0) {
            BTLFX_COPY_SVEC(&from, &work->rot8);
            work->rot8.vx += 0x300 + (ex->btlfx_rand() & 0xFF);
            work->rot8.vy += 0x800 + (ex->btlfx_rand() & 0x1FF);
            BTLFX_COPY_SVEC(&center, &fx_0ba_summonPos);
            center.vy -= 0x140;
            fxm_arcTrailSpawn(entry, &center, 0x100, &from, &work->rot8);
        }
        work->rot8.vx &= 0xFFF;
        work->rot8.vy &= 0xFFF;
        if (fx_0ba_summonDone != 0) {
            return 1;
        }
    }
    return 0;
}

INCLUDE_RODATA("asm/us/fx_0ba/nonmatchings/fx_0ba", fx_0ba_headerPtr);
