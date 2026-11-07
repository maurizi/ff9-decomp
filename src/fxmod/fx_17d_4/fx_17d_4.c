#include "fxmod/fx_17d_4.h"

static void fxm_drawTrailLine(u8 *file, SVECTOR *from, SVECTOR *to, s32 frame, s32 fade, s32 abr, s32 lineWidth) {
    BtlfxExports *ex = BTLFX_EXPORTS(fx_17d_4_env);
    SVECTOR *pts;

    fx_17d_4_lineTrail.lastSlot0 = fx_17d_4_lineTrail.count2 = 1;
    fx_17d_4_lineTrail.head1 = 0;
    pts = fx_17d_4_lineTrail.points4;
    pts[0].vx = from->vx;
    pts[0].vy = from->vy;
    pts[0].vz = from->vz;
    pts[0].pad = lineWidth;
    pts[1].vx = to->vx;
    pts[1].vy = to->vy;
    pts[1].vz = to->vz;
    pts[1].pad = lineWidth;
    ex->btlfx_drawTrail(file, 0, NULL, frame, 0x1000, fade, abr, (u8 *)&fx_17d_4_lineTrail, 0);
}

void fx_17d_4_drawStrands(Unk80100C78Entry *entry, SVECTOR *from, s32 n, s32 w, s32 a, s32 fade, s32 flag) {
    u8 *data;
    s32 layer;
    s32 width;
    BtlfxExports *ex;
    SVECTOR last;
    SVECTOR lineTo;
    SVECTOR lineFrom;
    s16 *pts;
    Fx17d4Line *tr;
    SVECTOR *dst;
    s32 j;
    s32 i;
    s32 cnt;
    s32 startFrame;

    ex = BTLFX_EXPORTS(fx_17d_4_env);
    layer = 0;
    data = (u8 *)ex->btlfx_getFileData(entry->file, FX_ARK_STRAND_POINTS);
    w = (w * 3) >> 1;
    do {
        for (j = 0; j < FX_ARK_STRANDS; j++) {
            cnt = *(u16 *)(data + ((j + layer * FX_ARK_STRANDS) << 2));
            pts = (s16 *)(data + cnt);
            cnt = pts[1];
            tr = fx_17d_4_strandTrails[j];
            tr = (Fx17d4Line *)((u8 *)tr + fx_17d_4_strandInfo[j * 4 + 2] * layer);
            width = w * (fx_17d_4_strandInfo[j * 4 + 3] + 1);
            tr->lastSlot0 = tr->count2 = cnt - 1;
            tr->head1 = 0;
            pts += 2;
            dst = tr->points4;
            for (i = 0; i < cnt; i++, dst++, pts += 3) {
                startFrame = fx_17d_4_strandInfo[j * 4 + 1];
                if (i == 0) {
                    last.vx = from->vx + pts[0] * 2;
                    last.vy = from->vy + pts[1] * 2;
                    last.vz = from->vz + pts[2] * 2;
                    last.pad = 0;
                }
                startFrame += i;
                if (n >= startFrame) {
                    dst->vx = from->vx + pts[0] * 2;
                    dst->vy = from->vy + pts[1] * 2;
                    dst->vz = from->vz + pts[2] * 2;
                    dst->pad = width;
                    BTLFX_COPY_SVEC(&last, dst);
                    if (startFrame == n && flag) {
                        BTLFX_COPY_SVEC(&lineTo, dst);
                        BTLFX_COPY_SVEC(&lineFrom, dst);
                        lineFrom.vy -= FX_ARK_BEAM_LINE_HEIGHT;
                        fxm_drawTrailLine((u8 *)ex->btlfx_getFileData(entry->file, FX_ARK_BEAM_LINE), &lineFrom, &lineTo, 0, 0x800, BTLFX_ABR_ADD, FX_ARK_TIP_LINE_WIDTH);
                    }
                } else {
                    BTLFX_COPY_SVEC(dst, &last);
                }
            }
            ex->btlfx_drawTrail((u8 *)ex->btlfx_getFileData(entry->file, fx_17d_4_strandInfo[j * 4]), 0, (s32 *)from, 0, 0x1000, a,
                                BTLFX_ABR_ADD, (u8 *)tr, fade);
        }
        layer++;
    } while (layer < FX_ARK_STRAND_LAYERS);
}

void fxm_drawPatternParticles(Unk80100C78Entry *entry, SVECTOR *from, s32 n, s32 ot) {
    SVECTOR pos;
    SVECTOR rot;
    SVECTOR ang;
    s32 k = 0;
    BtlfxExports *ex = BTLFX_EXPORTS(fx_17d_4_env);
    Fx17d4Pt *pt;
    s32 i;
    s32 orbitAng;

    rot.vx = 0x400;
    rot.vy = 0;
    rot.pad = 1;
    for (; k < FX_ARK_PATTERN_LISTS; k++) {
        pt = (Fx17d4Pt *)ex->btlfx_getFileData(entry->file, fx_17d_4_pointFileIds[k]);
        pt = (Fx17d4Pt *)((u8 *)pt + 4);
        for (i = 0; i < FX_ARK_PATTERN_POINTS; i++, pt++) {
            if (i < n) {
                pos.vx = from->vx + pt->x * 2;
                pos.vy = from->vy + pt->y * 2;
                pos.vz = from->vz + pt->z * 2;
                ex->btlfx_aimAngles((s16 *)from, (s16 *)&pos, (s16 *)&ang);
                rot.vz = -ang.vy;
                ex->btlfx_drawParticles((u8 *)ex->btlfx_getFileData(entry->file, fx_17d_4_particleFileIds[i & (FX_ARK_PARTICLE_KINDS - 1)]), &pos, &rot, 0x3000,
                                        0, BTLFX_ABR_ADD, ot, 0, 0);
            }
        }
    }
    for (k = 0; k < FX_ARK_ORBIT_ARCS; k++) {
        for (i = 0; i < FX_ARK_ARC_POINTS; i++) {
            if (i <= n * FX_ARK_ARC_POINTS / FX_ARK_PATTERN_POINTS) {
                orbitAng = ((i + k * FX_ARK_ARC_POINTS) << 12) / (FX_ARK_ORBIT_ARCS * FX_ARK_ARC_POINTS);
                pos.vx = from->vx + ((ex->math_cos(orbitAng) * FX_ARK_ORBIT_RADIUS) >> 12);
                pos.vy = from->vy;
                pos.vz = from->vz + ((ex->math_sin(orbitAng) * FX_ARK_ORBIT_RADIUS) >> 12);
                ex->btlfx_aimAngles((s16 *)from, (s16 *)&pos, (s16 *)&ang);
                rot.vz = -ang.vy;
                ex->btlfx_drawParticles((u8 *)ex->btlfx_getFileData(entry->file, fx_17d_4_particleFileIds[i & (FX_ARK_PARTICLE_KINDS - 1)]), &pos, &rot, 0x1800,
                                        0, BTLFX_ABR_ADD, ot, 0, 0);
            }
        }
    }
}

void fxm_drawOffsetParticles3(Unk80100C78Entry *entry, SVECTOR *from, s32 ot) {
    SVECTOR pos;
    SVECTOR rot;
    SVECTOR ang;
    SVECTOR *ofs = fx_17d_4_particleOffsets;
    s32 i = 0;
    BtlfxExports *ex = BTLFX_EXPORTS(fx_17d_4_env);

    rot.vx = 0x400;
    rot.vy = 0;
    rot.pad = 1;
    do {
        pos.vx = from->vx + ofs->vx * 2;
        pos.vy = from->vy + ofs->vy * 2;
        pos.vz = from->vz + ofs->vz * 2;
        ex->btlfx_aimAngles((s16 *)from, (s16 *)&pos, (s16 *)&ang);
        rot.vz = -ang.vy;
        ex->btlfx_drawParticles((u8 *)ex->btlfx_getFileData(entry->file, fx_17d_4_particleFileIds[i + FX_ARK_PARTICLE_KINDS]), &pos, &rot, 0x2AF8, 0, BTLFX_ABR_ADD, ot, 0, 0);
        ofs++;
        i++;
    } while (i < FX_ARK_OFFSET_PARTICLES);
}

INCLUDE_RODATA("asm/us/fx_17d_4/nonmatchings/fx_17d_4", fx_17d_4_headerPtr);

/* Matching notes: scalar temporaries are split per case and per use (found by build/s/roles.py), since sharing a
 * variable across cases makes it a call-crossing global and changes the allocation; which roles share a variable was
 * searched. Case 6: fxm_drawPatternParticles's third argument (patternFrame) is its own variable, strandWidth = 0x20
 * is set inside the frame < 0x1E branch, strandFrame and patternFrame are set first in their branches and
 * strandFrame is declared before flag/patternFrame (spill slot order 0x78/0x7C/0x80). Case 4's drift update reuses
 * one variable (ease) for 0x1000 - cos and drift >> 12; case 5's offsets go through orbitX/orbitZ and are copied
 * ringPos -> lineTop; mode 1's copy order and other statement orders were searched (build/s/permhc.py, bperm.py,
 * winperm.py). */
s32 fx_17d_4_op80(s32 mode, Fx17d4Work80 *work, Unk800F78A4 *env, Unk80100C78Entry *entry, s32 *args) {
    s32 ringBright;
    s32 orbitRadius;
    Pool6CEntry *emitter;
    s32 spinStep;
    s32 ramp;
    s32 spinStep3;
    s32 grow;
    s32 ring;
    s32 fadeT;
    Pool6CEntry *sparkEmitter;
    s32 phase;
    s32 flashAng;
    s32 ringAlpha;
    s32 bobT;
    s32 particleAlpha;
    BtlfxExports *ex = BTLFX_EXPORTS(env);
    VECTOR sc;
    Fx17d4Vec3 pos;
    SVECTOR rot;
    SVECTOR lineFrom;
    SVECTOR lineTop;
    SVECTOR ringPos;
    SVECTOR flatRot;
    SVECTOR baseTop;
    SVECTOR basePos;
    s32 orbitAng;
    s32 lineFade;
    s32 fadeOut;
    s32 frame;
    s32 alpha;
    s32 ringScale;
    s32 bobAng;
    s32 drift;
    s32 strandFade;
    s32 strandFrame;
    s32 flag;
    s32 patternFrame;
    s32 orbitX;
    s32 orbitZ;
    s32 outT;
    s32 strandWidth;
    s32 ease;
    s32 flashScale;
    s32 flashAlpha;
    s32 strandCue;
    s32 ringBase;

    if (mode == 0) {
        *args = sizeof(Fx17d4Work80);
    } else if (mode == 1) {
        work->state0 = FX_ARK_STATE_CHARGE;
        work->frame4 = 0;
        fx_17d_4_env = env;
        ex->btlfx_initParticleSystem();
        ex->btlfx_initEffModels(fx_17d_4_effModels, FX_ARK_MODEL_COUNT);
        work->model68 = ex->btlfx_registerEffectModel((Unk800E8F50 *)ex->btlfx_getFileData(entry->file, FX_ARK_SMOKE_CYLINDER), 1);
        work->model6C = ex->btlfx_registerEffectModel((Unk800E8F50 *)ex->btlfx_getFileData(entry->file, FX_ARK_SMOKE_TORUS), 1);
        work->model70 = ex->btlfx_registerEffectModel((Unk800E8F50 *)ex->btlfx_getFileData(entry->file, FX_ARK_STREAK_BOWL), 1);
        work->model74 = ex->btlfx_registerEffectModel((Unk800E8F50 *)ex->btlfx_getFileData(entry->file, FX_ARK_FLAME_BOWL), 0);
        work->model78 = ex->btlfx_registerEffectModel((Unk800E8F50 *)ex->btlfx_getFileData(entry->file, FX_ARK_SWIRL_DOME), 1);
        ex->Hi_SetSummonMotion(SUMMON(env)->summon54->motion19C, SUMMON(env)->model60);
        work->pos48.vx = 0;
        work->pos48.vy = -0x400;
        work->pos48.vz = 0;
        work->scale3C.vx = work->scale3C.vz = 0x400;
        work->pos28.vy = work->pos48.vy;
        work->rot60.vx = 0;
        work->rot60.vy = 0;
        work->rot60.vz = 0;
        work->rot34.vx = 0;
        work->rot34.vy = 0;
        work->rot34.vz = 0;
        work->pos28.vx = work->pos48.vx;
        work->pos28.vz = work->pos48.vz;
        work->scale3C.vy = 0x1000;
        work->pos28.vy += 0x800;
        work->pos20.vx = work->pos48.vx;
        work->pos20.vy = work->pos48.vy;
        work->pos20.vz = work->pos48.vz;
        env->camera->points[3].vx = work->pos20.vx;
        env->camera->points[3].vy = work->pos20.vy;
        env->camera->points[3].vz = work->pos20.vz;
        fx_17d_4_strandTrailTail = 0;
    } else {
        frame = entry->frame;
        switch (work->state0) {
        case FX_ARK_STATE_CHARGE:
            ramp = (frame << 12) / FX_ARK_CHARGE_FRAMES;
            ex->Hi_SetSummonMotFrame(SUMMON(env)->model60, 0);
            work->pos48.vy += ex->math_cos(ramp >> 2) >> 4;
            work->scale54.vx = work->scale54.vy = work->scale54.vz = ex->math_cos(ramp >> 2) + 0x800;
            ex->Hi_DrawSummonModel(&work->rot60, (VECTOR *)&work->pos48, (s32)&work->scale54, SUMMON(env)->model60, 0);
            ex->Hi_DrawEffModel(&work->rot34, (VECTOR *)&work->pos28, (s32)&work->scale3C, work->model68);
            ex->Hi_ModifyEffModelAbr(work->model68, BTLFX_ABR_ADD);
            ex->Hi_ModifyEffModelRGB(work->model68, 0, 0, 0);
            ex->Hi_DrawEffModel(&work->rot34, (VECTOR *)&work->pos28, (s32)&work->scale3C, work->model68);
            ex->Hi_ModifyEffModelAbr(work->model68, BTLFX_ABR_SUB);
            ex->Hi_ModifyEffModelRGB(work->model68, 0, 0, 0);
            pos.vx = work->pos28.vx;
            pos.vy = work->pos28.vy;
            pos.vz = work->pos28.vz;
            pos.vy += 0x2000;
            sc.vx = sc.vy = sc.vz = 0x1000;
            rot.vx = 0x800;
            rot.vy = frame * 24;
            rot.vz = 0;
            ex->Hi_DrawEffModel(&rot, (VECTOR *)&pos, (s32)&sc, work->model74);
            ex->btlfx_drawScrollMove(frame * FX_ARK_SCROLL_SPEED, FX_ARK_SCROLL_VRAM_X, FX_ARK_SCROLL_VRAM_Y, FX_ARK_SCROLL_VRAM_W, FX_ARK_SCROLL_VRAM_H, FX_ARK_SCROLL_SRC_X, FX_ARK_SCROLL_SRC_Y);
            if (frame >= FX_ARK_CHARGE_FRAMES) {
                work->state0 = FX_ARK_STATE_HOLD;
                entry->frame = -1;
                ex->btlfx_initParticleSystem();
                ex->Hi_SetSummonMotion(SUMMON(env)->summon54->motion1A0, SUMMON(env)->model60);
                work->rot34.vx = 0;
                work->rot34.vy = 0;
                work->rot34.vz = 0;
                work->scale3C.vx = work->scale3C.vz = 0x800;
                work->scale3C.vy = 0x1000;
                work->pos28.vx = work->pos48.vx;
                work->pos28.vy = work->pos48.vy;
                work->pos28.vz = work->pos48.vz;
                work->pos20.vx = work->pos48.vx;
                work->pos20.vy = work->pos48.vy;
                work->pos20.vz = work->pos48.vz;
                work->pos28.vy -= 0x400;
                work->pos20.vy -= 0x8FC;
                env->camera->points[3].vx = work->pos20.vx;
                env->camera->points[3].vy = work->pos20.vy;
                env->camera->points[3].vz = work->pos20.vz;
            }
            break;
        case FX_ARK_STATE_HOLD:
            work->scale54.vx = work->scale54.vy = work->scale54.vz = 0x1000;
            work->pos48.vy -= 0xC;
            ex->Hi_DrawSummonModel(&work->rot60, (VECTOR *)&work->pos48, (s32)&work->scale54, SUMMON(env)->model60, 0);
            ex->Hi_DrawEffModel(&work->rot34, (VECTOR *)&work->pos28, (s32)&work->scale3C, work->model68);
            ex->btlfx_drawScrollMove(frame * FX_ARK_SCROLL_SPEED, FX_ARK_SCROLL_VRAM_X, FX_ARK_SCROLL_VRAM_Y, FX_ARK_SCROLL_VRAM_W, FX_ARK_SCROLL_VRAM_H, FX_ARK_SCROLL_SRC_X, FX_ARK_SCROLL_SRC_Y);
            if (frame >= FX_ARK_HOLD_FRAMES) {
                work->state0 = FX_ARK_STATE_SWIRL;
                entry->frame = -1;
                ex->btlfx_initParticleSystem();
                work->pos10.vy = work->pos48.vy;
                work->pos10.vx = work->pos48.vx;
                work->pos10.vz = work->pos48.vz;
                work->pos10.vy -= 0x400;
                emitter = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, FX_ARK_GREEN_TRAIL), (s32)&work->pos10);
                if (emitter != NULL) {
                    emitter->trails = (s32)fx_17d_4_op80Trails;
                    emitter->trailSlots = FX_ARK_GREEN_TRAIL_TRAIL_SLOTS;
                    emitter->trailStride = FX_ARK_GREEN_TRAIL_TRAIL_STRIDE;
                    emitter->otZ = 0x20;
                }
            }
            break;
        case FX_ARK_STATE_SWIRL:
            work->scale54.vx = work->scale54.vy = work->scale54.vz = 0x1000;
            phase = (frame << 12) / FX_ARK_SWIRL_FRAMES;
            bobAng = phase >> 2;
            work->pos48.vy -= (ex->math_cos(bobAng) * 10) >> 12;
            ex->Hi_DrawSummonModel(&work->rot60, (VECTOR *)&work->pos48, (s32)&work->scale54, SUMMON(env)->model60, 0);
            ex->Hi_DrawEffModel(&work->rot34, (VECTOR *)&work->pos28, (s32)&work->scale3C, work->model68);
            ringScale = phase / 2 + 0x800;
            sc.vy = ex->math_sin(bobAng) / 2 + 0x800;
            pos.vx = work->pos48.vx;
            pos.vy = work->pos48.vy;
            pos.vz = work->pos48.vz;
            for (ramp = 0, spinStep = 0x20; ramp < FX_ARK_SWIRL_RINGS; ramp++, spinStep += 0x10) {
                s32 yaw;

                rot.vx = 0;
                rot.vz = 0;
                sc.vx = sc.vz = (ringScale * (ramp + 2)) / 6;
                yaw = frame * spinStep;
                rot.vy = yaw;
                ex->Hi_DrawEffModel(&rot, (VECTOR *)&pos, (s32)&sc, work->model78);
                ex->Hi_ModifyEffModelAbr(work->model78, BTLFX_ABR_ADD);
                ex->Hi_ModifyEffModelRGB(work->model78, (ex->math_sin(phase >> 2) >> 5) - 0x80, (ex->math_sin(phase >> 2) >> 5) - 0x80,
                                         (ex->math_sin(phase >> 2) >> 5) - 0x80);
                yaw = 0;
            }
            ex->btlfx_drawScrollMove(frame * FX_ARK_SCROLL_SPEED, FX_ARK_SCROLL_VRAM_X, FX_ARK_SCROLL_VRAM_Y, FX_ARK_SCROLL_VRAM_W, FX_ARK_SCROLL_VRAM_H, FX_ARK_SCROLL_SRC_X, FX_ARK_SCROLL_SRC_Y);
            if (frame > FX_ARK_FLASH_FRAME) {
                ex->btlfx_drawScreenTint((u32 *)(env->ot + 2), BTLFX_ABR_ADD, (frame - FX_ARK_FLASH_FRAME) * FX_ARK_FLASH_STEP,
                                         (frame - FX_ARK_FLASH_FRAME) * FX_ARK_FLASH_STEP, (frame - FX_ARK_FLASH_FRAME) * FX_ARK_FLASH_STEP);
            }
            if (frame >= FX_ARK_SWIRL_FRAMES) {
                work->state0 = FX_ARK_STATE_BOWL;
                entry->frame = -1;
                ex->Hi_SetSummonMotion(SUMMON(env)->summon54->motion1A0, SUMMON(env)->model60);
                ex->btlfx_initParticleSystem();
                work->scale3C.vx = work->scale3C.vz = 0x1800;
                work->rot34.vx = 0;
                work->rot34.vy = 0;
                work->rot34.vz = 0;
                work->scale3C.vy = 0xAAA;
                work->pos28.vy = work->pos48.vy;
                work->pos28.vx = work->pos48.vx;
                work->pos28.vz = work->pos48.vz;
                work->pos28.vy += 0x800;
            }
            break;
        case FX_ARK_STATE_BOWL:
            grow = frame << 6;
            if (frame < FX_ARK_BOWL_TURN_FRAMES) {
                work->rot60.vy = (ex->math_sin((frame << 10) >> 5) - 0x1000) >> 3;
            }
            work->scale54.vx = work->scale54.vy = work->scale54.vz = 0x1000;
            ex->Hi_DrawSummonModel(&work->rot60, (VECTOR *)&work->pos48, (s32)&work->scale54, SUMMON(env)->model60, 0);
            work->rot34.vy += 2;
            ex->Hi_DrawEffModel(&work->rot34, (VECTOR *)&work->pos28, (s32)&work->scale3C, work->model70);
            pos.vx = work->pos28.vx;
            pos.vy = work->pos28.vy;
            pos.vz = work->pos28.vz;
            pos.vy -= 0x2000;
            sc.vx = sc.vy = sc.vz = 0x1000;
            rot.vx = 0;
            rot.vy = frame;
            rot.vz = 0;
            ex->Hi_DrawEffModel(&rot, (VECTOR *)&pos, (s32)&sc, work->model74);
            pos.vx = work->pos48.vx;
            pos.vy = work->pos48.vy;
            pos.vz = work->pos48.vz;
            pos.vy -= 0x1000;
            sc.vy = 0x2000;
            rot.vx = 0x800;
            rot.vy = frame * 4;
            rot.vz = 0;
            sc.vx = sc.vz = grow / 3 + 0x800;
            ex->Hi_DrawEffModel(&rot, (VECTOR *)&pos, (s32)&sc, work->model6C);
            ex->Hi_ModifyEffModelRGB(work->model6C, (ex->math_cos(grow >> 2) >> 5) - 0x80, (ex->math_cos(grow >> 2) >> 5) - 0x80,
                                     (ex->math_cos(grow >> 2) >> 5) - 0x80);
            if (frame < FX_ARK_BOWL_SWIRL_FRAMES) {
                fadeT = (frame << 12) / FX_ARK_BOWL_SWIRL_FRAMES;
                sc.vy = fadeT / 3 + 0x1000;
                ringBase = fadeT + 0x1000;
                pos.vx = work->pos48.vx;
                pos.vy = work->pos48.vy;
                ringBright = ((0x1000 - fadeT) >> 5) - 0x80;
                pos.vz = work->pos48.vz;
                for (ring = 0, spinStep3 = 0x20; ring < FX_ARK_BOWL_SWIRL_RINGS; ring++, spinStep3 += 0x10) {
                    s32 yaw;

                    rot.vx = 0;
                    rot.vz = 0;
                    sc.vx = sc.vz = (ringBase * (ring + 2)) / 5;
                    sc.vy = sc.vy * 2 / 3;
                    yaw = frame * spinStep3;
                    rot.vy = yaw;
                    ex->Hi_DrawEffModel(&rot, (VECTOR *)&pos, (s32)&sc, work->model78);
                    ex->Hi_ModifyEffModelAbr(work->model78, BTLFX_ABR_ADD);
                    ex->Hi_ModifyEffModelRGB(work->model78, ringBright, ringBright, ringBright);
                    yaw = 0;
                }
            }
            ex->btlfx_drawScrollMove(0x2710 - frame * FX_ARK_BOWL_SCROLL_SPEED, FX_ARK_SCROLL_VRAM_X, FX_ARK_SCROLL_VRAM_Y, FX_ARK_SCROLL_VRAM_W, FX_ARK_SCROLL_VRAM_H, FX_ARK_SCROLL_SRC_X, FX_ARK_SCROLL_SRC_Y);
            if (frame < FX_ARK_FLASH_FADE_FRAMES) {
                ex->btlfx_drawScreenTint((u32 *)(env->ot + 2), BTLFX_ABR_ADD, FX_ARK_FLASH_PEAK - frame * FX_ARK_FLASH_FADE_STEP,
                                         FX_ARK_FLASH_PEAK - frame * FX_ARK_FLASH_FADE_STEP, FX_ARK_FLASH_PEAK - frame * FX_ARK_FLASH_FADE_STEP);
            }
            work->pos20.vy += 8;
            env->camera->points[3].vx = work->pos20.vx;
            env->camera->points[3].vy = work->pos20.vy;
            env->camera->points[3].vz = work->pos20.vz;
            if (frame >= FX_ARK_BOWL_FRAMES) {
                work->state0 = FX_ARK_STATE_BEAM;
                ex->btlfx_initParticleSystem();
                entry->frame = -1;
                ex->Hi_GetSummonBonePos(SUMMON(env)->model60, FX_ARK_MUZZLE_BONE, (u16 *)&work->pos20);
                work->pos20.vx += 0x40;
                work->pos20.vy -= 0x5A;
                work->pos20.vz -= 0x40;
                work->pos10.vx = work->pos20.vx;
                work->pos10.vy = work->pos20.vy;
                work->pos10.vz = work->pos20.vz;
                env->camera->points[2].vx = work->pos20.vx;
                env->camera->points[2].vy = work->pos20.vy;
                env->camera->points[2].vz = work->pos20.vz;
                work->pos28.vy += 0x1000;
            }
            break;
        case FX_ARK_STATE_BEAM:
            bobT = (frame << 12) / FX_ARK_BEAM_FRAMES;
            work->scale54.vx = work->scale54.vy = work->scale54.vz = 0x1000;
            ex->Hi_DrawSummonModel(&work->rot60, (VECTOR *)&work->pos48, (s32)&work->scale54, SUMMON(env)->model60, 0);
            work->rot34.vy += 2;
            ex->Hi_DrawEffModel(&work->rot34, (VECTOR *)&work->pos28, (s32)&work->scale3C, work->model70);
            pos.vx = work->pos28.vx;
            pos.vy = work->pos28.vy;
            pos.vz = work->pos28.vz;
            pos.vy -= 0x2000;
            sc.vx = sc.vy = sc.vz = 0x1000;
            rot.vx = 0;
            rot.vy = frame;
            rot.vz = 0;
            ex->Hi_DrawEffModel(&rot, (VECTOR *)&pos, (s32)&sc, work->model74);
            ex->Hi_GetSummonBonePos(SUMMON(env)->model60, FX_ARK_MUZZLE_BONE, (u16 *)&work->pos20);
            work->pos20.vx += 0x40;
            work->pos20.vy -= 0x5A;
            work->pos20.vz -= 0x40;
            work->pos10.vx = work->pos20.vx;
            work->pos10.vy = work->pos20.vy;
            work->pos10.vz = work->pos20.vz;
            if (frame >= FX_ARK_BEAM_DRIFT_FRAME) {
                ease = 0x1000 - ex->math_cos(((frame - FX_ARK_BEAM_DRIFT_FRAME) << 10) / (FX_ARK_BEAM_FRAMES - FX_ARK_BEAM_DRIFT_FRAME));
                drift = ease * 0x258;
                ease = drift >> 12;
                work->pos20.vy += drift >> 13;
                work->pos20.vz -= ease;
            }
            env->camera->points[2].vx = work->pos20.vx;
            env->camera->points[2].vy = work->pos20.vy;
            env->camera->points[2].vz = work->pos20.vz;
            if (frame == 0) {
                sparkEmitter = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, FX_ARK_MAGENTA_RING), (s32)&work->pos10);
                if (sparkEmitter != NULL) {
                    sparkEmitter->otZ = 0x8C;
                }
            }
            env->otZ = 0x10;
            ringAlpha = 0x1000;
            if (frame < FX_ARK_RED_RING_FADE_FRAMES) {
                ringAlpha = frame << 9;
            }
            ex->btlfx_drawRings((u8 *)ex->btlfx_getFileData(entry->file, FX_ARK_RED_RING), &work->pos10, NULL, 0x1000, 0, BTLFX_ABR_ADD, ringAlpha);
            if (frame >= FX_ARK_BEAM_LINE_FRAME) {
                lineFade = ((frame - FX_ARK_BEAM_LINE_FRAME) << 12) / (FX_ARK_BEAM_FRAMES - FX_ARK_BEAM_LINE_FRAME);
            } else {
                lineFade = 0;
            }
            BTLFX_COPY_SVEC(&lineFrom, &work->pos10);
            lineFrom.vy += ex->math_sin(bobT >> 2) >> 3;
            lineFrom.vz -= 0x400;
            fxm_drawTrailLine((u8 *)ex->btlfx_getFileData(entry->file, FX_ARK_BEAM_LINE), &lineFrom, &work->pos10, frame, lineFade, BTLFX_ABR_ADD, 0x18);
            env->otZ = 0;
            if (frame >= FX_ARK_BEAM_FRAMES) {
                work->state0 = FX_ARK_STATE_BEAM_2;
                entry->frame = -1;
                ex->btlfx_initParticleSystem();
                work->pos8.vx = fx_17d_4_centerPos.vx;
                work->pos8.vy = fx_17d_4_centerPos.vy;
                work->pos8.vz = fx_17d_4_centerPos.vz;
                work->pos10.vy = work->pos8.vy;
                work->pos10.vx = work->pos8.vx;
                work->pos10.vz = work->pos8.vz;
                env->camera->points[3].vx = work->pos8.vx;
                env->camera->points[3].vy = work->pos8.vy;
                env->camera->points[3].vz = work->pos8.vz;
            }
            break;
        case FX_ARK_STATE_BEAM_2:
            env->otZ = 0x10;
            phase = (frame << 12) / FX_ARK_BEAM_2_FRAMES;
            orbitAng = ex->math_cos(phase >> 1) >> 1;
            orbitRadius = ex->math_cos(phase >> 2) >> 1;
            orbitX = (ex->math_cos(orbitAng) * orbitRadius) >> 12;
            orbitZ = (ex->math_sin(orbitAng) * orbitRadius) >> 12;
            ringPos.vx = work->pos8.vx + orbitX;
            ringPos.vy = work->pos8.vy;
            ringPos.vz = work->pos8.vz + orbitZ;
            lineTop.vx = ringPos.vx;
            lineTop.vy = work->pos8.vy - FX_ARK_BEAM_LINE_HEIGHT;
            lineTop.vz = ringPos.vz;
            ex->btlfx_drawRings((u8 *)ex->btlfx_getFileData(entry->file, FX_ARK_RED_RING), &ringPos, NULL, FX_ARK_RED_RING_SCALE, 0, BTLFX_ABR_ADD, phase);
            fxm_drawTrailLine((u8 *)ex->btlfx_getFileData(entry->file, FX_ARK_BEAM_LINE), &ringPos, &lineTop, frame, phase, BTLFX_ABR_ADD, FX_ARK_BEAM_LINE_WIDTH);
            env->otZ = 0;
            if (frame >= FX_ARK_BEAM_2_FRAMES) {
                work->state0 = FX_ARK_STATE_BLAST;
                entry->frame = -1;
            }
            break;
        case FX_ARK_STATE_BLAST:
            basePos.vx = work->pos8.vx;
            basePos.vy = work->pos8.vy;
            basePos.vz = work->pos8.vz;
            baseTop.vx = basePos.vx;
            baseTop.vy = work->pos8.vy - FX_ARK_BEAM_LINE_HEIGHT;
            baseTop.vz = basePos.vz;
            env->otZ = 0x10;
            ramp = (frame << 12) / FX_ARK_BLAST_FRAMES;
            fadeOut = 0x1000 - ramp;
            ex->btlfx_drawRings((u8 *)ex->btlfx_getFileData(entry->file, FX_ARK_RED_RING), &basePos, NULL, FX_ARK_RED_RING_SCALE, 0, BTLFX_ABR_ADD, fadeOut);
            fxm_drawTrailLine((u8 *)ex->btlfx_getFileData(entry->file, FX_ARK_BEAM_LINE), &basePos, &baseTop, frame, fadeOut, BTLFX_ABR_ADD,
                          (ramp >> 7) + FX_ARK_BEAM_LINE_WIDTH);
            env->otZ = 0;
            flatRot.vx = 0x400;
            flatRot.vz = 0;
            flatRot.vy = 0;
            flatRot.pad = 1;
            if (frame < FX_ARK_FLASH_RING_FRAMES) {
                flashAng = (frame << 12) >> 6;
                flashScale = ex->math_sin(flashAng) * 5;
                flashAlpha = ex->math_cos(flashAng);
                ex->btlfx_drawRings((u8 *)ex->btlfx_getFileData(entry->file, FX_ARK_RED_FLASH_RING), &basePos, &flatRot, flashScale, 0, BTLFX_ABR_ADD, flashAlpha);
            }
            if (frame >= FX_ARK_STRANDS_FRAMES) {
                alpha = (frame - FX_ARK_STRANDS_FRAMES) << 9;
                if (alpha > 0x1000) {
                    alpha = 0x1000;
                }
                ex->btlfx_drawRings((u8 *)ex->btlfx_getFileData(entry->file, FX_ARK_RED_FLASH_RING), &basePos, &flatRot, 0x5000, 0, BTLFX_ABR_ADD, alpha);
            }
            flag = 0;
            if (frame < FX_ARK_STRANDS_FADE_FRAME) {
                if (frame < FX_ARK_STRANDS_FRAMES) {
                    strandWidth = 0x20;
                    strandFrame = frame;
                    strandCue = 0x1000;
                    strandFade = 0;
                    flag = 1;
                    patternFrame = 0;
                    particleAlpha = 0;
                    alpha = 0;
                } else {
                    outT = (frame - FX_ARK_STRANDS_FRAMES) << 9;
                    strandFrame = FX_ARK_STRANDS_FRAMES - 1;
                    strandWidth = 0x27;
                    strandCue = 0x1000;
                    strandFade = 0;
                    patternFrame = (frame - FX_ARK_STRANDS_FRAMES) * FX_ARK_PATTERN_STEP;
                    particleAlpha = 0x1000;
                    if (outT >= 0x800) {
                        alpha = (outT - 0x800) * 2;
                    } else {
                        alpha = 0;
                    }
                    ex->btlfx_drawScreenTint((u32 *)(env->ot + 2), BTLFX_ABR_ADD, ((0x1000 - outT) * 0x64) >> 12, 0, 0);
                }
            } else {
                strandFade = 1;
                strandFrame = FX_ARK_STRANDS_FRAMES - 1;
                outT = ((frame - FX_ARK_STRANDS_FADE_FRAME) << 12) / (FX_ARK_BLAST_FRAMES - FX_ARK_STRANDS_FADE_FRAME);
                strandWidth = (outT >> 3) + 0x40;
                strandCue = 0x1000 - outT * 2 / 3;
                patternFrame = (FX_ARK_STRANDS_FADE_FRAME - 1 - FX_ARK_STRANDS_FRAMES) * FX_ARK_PATTERN_STEP;
                alpha = strandCue;
                particleAlpha = alpha;
            }
            fx_17d_4_drawStrands(entry, &fx_17d_4_centerPos, strandFrame, strandWidth, strandCue, strandFade, flag);
            if (particleAlpha != 0) {
                fxm_drawPatternParticles(entry, &fx_17d_4_centerPos, patternFrame, particleAlpha);
                fxm_drawOffsetParticles3(entry, &fx_17d_4_centerPos, alpha);
            }
            if (frame >= FX_ARK_BLAST_FRAMES) {
                ex->btlfx_clearEffModels();
                return 1;
            }
            break;
        }
        env->otZ = 0;
        work->frame4++;
    }
    return 0;
}
