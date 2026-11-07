#include "fxmod/fx_057.h"

static void fx_057_op80ElemDraw(Pool30Entry *pt, Pool6CEntry *owner, SVECTOR *pos, s32 rot, s32 scale, s32 alpha, s32 blend, s32 frame,
                   s32 n) {
    Unk800F78A4 *env = fx_057_env;
    BtlfxExports *ex = BTLFX_EXPORTS(env);
    Unk80100C78Entry *entry = fx_057_task;
    u8 *buf = (u8 *)ex->btlfx_getParticleTrail(pt, owner);
    s32 fade;
    s32 elapsed;
    SVECTOR camPos;

    if (frame == 0) {
        ex->btlfx_drawTrail((u8 *)ex->btlfx_getFileData(entry->file, FX_HOLY_TRAIL), (s32)pos, NULL, -1, 0, 0, BTLFX_ABR_HALF, buf, 0);
    }
    if (frame < n - FX_HOLY_TRAIL_FADE_FRAMES) {
        fade = 0x1000;
    } else {
        fade = 0x1000 - (((frame - (n - FX_HOLY_TRAIL_FADE_FRAMES)) << 12) >> 2);
    }
    ex->btlfx_drawTrail((u8 *)ex->btlfx_getFileData(entry->file, FX_HOLY_TRAIL), (s32)pos, (s32 *)owner->origin, frame,
                      0x1C00 - (frame << 12) / n, fade, BTLFX_ABR_ADD, buf, 1);
    if (pt->slot == 1) {
        if (fx_057_emitter0 != NULL) {
            ex->btlfx_moveEmitterTo((s32)fx_057_emitter0, (s32)pos);
        }
        BTLFX_COPY_SVEC(&fx_057_headPos, pos);
        if (frame < FX_HOLY_CAM_FOLLOW_FRAMES) {
            env->camera->points[3].vx = pos->vx;
            env->camera->points[3].vy = pos->vy;
            env->camera->points[3].vz = pos->vz;
        } else {
            elapsed = frame - FX_HOLY_CAM_FOLLOW_FRAMES;
            if (elapsed < FX_HOLY_CAM_RETURN_FRAMES) {
                ex->btlfx_lerpVec((s16 *)pos, (s16 *)&fx_057_camRest, 0x1000 - ex->math_cos(elapsed << 5), (s16 *)&camPos);
            } else {
                BTLFX_COPY_SVEC(&camPos, &fx_057_camRest);
            }
            env->camera->points[3].vx = camPos.vx;
            env->camera->points[3].vy = camPos.vy;
            env->camera->points[3].vz = camPos.vz;
        }
    }
}

s32 fx_057_op80(s32 mode, Fx057Work80 *work, Unk800F78A4 *env, Unk80100C78Entry *entry, s32 *args) {
    BtlfxExports *ex = BTLFX_EXPORTS(env);
    SVECTOR rot;
    SVECTOR ang;
    VECTOR sv;
    SVECTOR unused; 
    Pool6CEntry *emitter;
    s32 frame;
    s32 alpha;
    s32 scale;
    s32 t;
    s32 tint;
    s32 swell;

    if (mode == 0) {
        *args = sizeof(Fx057Work80);
    } else if (mode == 1) {
        fx_057_env = env;
        fx_057_fade = 0;
        fx_057_task = entry;
        work->pos10.vx = 0;
        work->pos10.vy = 0;
        work->pos10.vz = 0;
        work->state0 = FX_HOLY_STATE_GATHER;
        work->frame4 = 0;
        BTLFX_COPY_SVEC(&fx_057_camRest, &work->pos10);
        work->pos18.vx = work->pos10.vx;
        work->pos18.vy = work->pos10.vy;
        work->pos18.vz = work->pos10.vz;
        ex->btlfx_getUnitBodyPos(0, 0, (s16 *)&fx_057_targetPos);
        work->emit2C = (Pool6CEntry *)ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, FX_HOLY_BLUE_SPARKLES), (s32)&work->pos10);
        if (work->emit2C != NULL) {
            work->emit2C->otZ = 0x18;
        }
        if (work->emit2C != NULL) {
            work->emit2C->trails = (s32)fx_057_trails;
            work->emit2C->trailSlots = FX_HOLY_BLUE_SPARKLES_TRAIL_SLOTS;
            work->emit2C->trailStride = FX_HOLY_BLUE_SPARKLES_TRAIL_STRIDE;
        }
        if (work->emit2C != NULL) {
            work->emit2C->elems->draw = (void (*)())fx_057_op80ElemDraw;
        }
        fx_057_emitter0 = work->emit30 = (Pool6CEntry *)ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, FX_HOLY_BLUE_SPARKLES_2), (s32)&work->pos24);
        if (work->emit30 != NULL) {
            work->emit30->otZ = 0x18;
        }
        work->emit34 = (Pool6CEntry *)ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, FX_HOLY_LENS_FLARE), (s32)&work->pos24);
        if (work->emit34 != NULL) {
            work->emit34->otZ = 0x10;
        }
        ex->btlfx_initEffModels(fx_057_effModels, FX_HOLY_MODEL_COUNT);
        work->model8 =
            ex->Hi_RegisterTexEffModel((BtlModelHeader *)ex->btlfx_getFileData(entry->file, FX_HOLY_LIGHT_SPHERE), FX_HOLY_LIGHT_SPHERE_TPAGE, FX_HOLY_LIGHT_SPHERE_CLUT, 0,
                                       FX_HOLY_LIGHT_SPHERE_V);
        work->modelC = ex->Hi_RegisterGouEffModel((BtlModelHeader *)ex->btlfx_getFileData(entry->file, FX_HOLY_GLOW_BAND));
    } else {
        alpha = 0;
        frame = entry->frame;
        scale = 0;
        switch (work->state0) {
        case FX_HOLY_STATE_GATHER:
            t = (frame << 12) / FX_HOLY_GATHER_FRAMES;
            if (work->emit2C != NULL && work->emit2C->state == BTLFX_EMIT_DONE) {
                work->emit2C = NULL;
            }
            if (work->emit30 != NULL && work->emit30->state == BTLFX_EMIT_DONE) {
                work->emit30 = NULL;
            }
            if (work->emit34 != NULL && work->emit34->state == BTLFX_EMIT_DONE) {
                work->emit34 = NULL;
            }
            alpha = 0x1000;
            if (work->emit2C != NULL) {
                work->emit2C->alpha = fx_057_fade;
            }
            if (work->emit30 != NULL) {
                work->emit30->alpha = alpha;
            }
            if (work->emit34 != NULL) {
                work->emit34->alpha = alpha;
            }
            scale = (((0x1000 - t) * 3) >> 1) + 0x1000;
            if (frame < FX_HOLY_FLASH_FRAMES) {
                tint = FX_HOLY_FLASH_LEVEL - frame * FX_HOLY_FLASH_LEVEL / FX_HOLY_FLASH_FRAMES;
                ex->btlfx_drawScreenTint((u32 *)(env->ot + 1), BTLFX_ABR_ADD, tint >> 1, tint, tint);
            }
            if (frame >= FX_HOLY_GATHER_FRAMES) {
                work->state0 = FX_HOLY_STATE_FADE;
                entry->frame = -1;
            }
            break;
        case FX_HOLY_STATE_FADE:
            t = (frame << 12) / FX_HOLY_FADE_FRAMES;
            alpha = 0x1000 - t;
            scale = 0x1000;
            fx_057_fade = t;
            if (work->emit2C != NULL && work->emit2C->state == BTLFX_EMIT_DONE) {
                work->emit2C = NULL;
            }
            if (work->emit30 != NULL && work->emit30->state == BTLFX_EMIT_DONE) {
                work->emit30 = NULL;
            }
            if (work->emit34 != NULL && work->emit34->state == BTLFX_EMIT_DONE) {
                work->emit34 = NULL;
            }
            if (work->emit2C != NULL) {
                work->emit2C->alpha = fx_057_fade;
            }
            if (work->emit30 != NULL) {
                work->emit30->alpha = alpha;
            }
            if (work->emit34 != NULL) {
                work->emit34->alpha = alpha;
            }
            if (frame >= FX_HOLY_FADE_FRAMES) {
                work->state0 = FX_HOLY_STATE_RAYS;
                entry->frame = -1;
                ex->btlfx_stopEmitter(work->emit30);
                ex->btlfx_stopEmitter(work->emit34);
                fx_057_emitter0 = NULL;
            }
            break;
        case FX_HOLY_STATE_RAYS:
            fx_057_fade = 0x1000;
            if (work->emit2C != NULL && work->emit2C->state == BTLFX_EMIT_DONE) {
                work->emit2C = NULL;
            }
            if (work->emit2C != NULL) {
                work->emit2C->alpha = fx_057_fade;
            }
            if (frame == FX_HOLY_RAYS_FRAME) {
                fx_057_emitter1 = emitter = (Pool6CEntry *)ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, FX_HOLY_VIOLET_RAYS), (s32)&work->pos10);
                if (fx_057_emitter1 != NULL) {
                    fx_057_emitter1->otZ = 0x80;
                }
                ex->btlfx_drawScreenTint((u32 *)(env->ot + 1), BTLFX_ABR_ADD, FX_HOLY_RAYS_TINT_RGB);
            }
            if (frame >= FX_HOLY_RAYS_FRAMES) {
                ex->btlfx_stopEmitter(work->emit2C);
                return 1;
            }
            break;
        }
        if (alpha != 0) {
            rot.pad = 0;
            rot.vy = 0;
            rot.vx = 0;
            rot.vz = -work->frame4 * 20;
            ex->btlfx_drawRings((u8 *)ex->btlfx_getFileData(entry->file, FX_HOLY_BLUE_RINGS), &fx_057_headPos, &rot, scale, 0, BTLFX_ABR_ADD, alpha);
            ex->btlfx_drawRings((u8 *)ex->btlfx_getFileData(entry->file, FX_HOLY_CYAN_RINGS), &fx_057_headPos, &rot, 0xE00, 0, BTLFX_ABR_ADD, alpha);
            ex->btlfx_drawRings((u8 *)ex->btlfx_getFileData(entry->file, FX_HOLY_SKY_RINGS), &fx_057_headPos, &rot, 0x1000, 0, BTLFX_ABR_ADD, alpha);
        }
        if (fx_057_fade != 0) {
            sv.vx = 0x1000;
            sv.vy = 0x1000;
            sv.vz = 0x1000;
            ang.vx = 0;
            ang.vy = -0x400;
            ang.vz = 0;
            ex->Hi_DrawEffModel(&ang, (VECTOR *)&work->pos18, (s32)&sv, work->model8);
            ex->Hi_ModifyEffModelAbr(work->model8, BTLFX_ABR_OPAQUE);
            ex->Hi_ModifyEffModelRGB(work->model8, (fx_057_fade >> 5) - 0x80, (fx_057_fade >> 5) - 0x80,
                                     (fx_057_fade >> 5) - 0x80);
            ex->Hi_DrawEffModel(&ang, (VECTOR *)&work->pos18, (s32)&sv, work->modelC);
            ex->Hi_ModifyEffModelAbr(work->modelC, BTLFX_ABR_OPAQUE);
            ex->Hi_ModifyEffModelRGB(work->modelC, (fx_057_fade >> 5) - 0x80, (fx_057_fade >> 5) - 0x80,
                                     (fx_057_fade >> 5) - 0x80);
            ex->btlfx_drawLayerTexture((BtlBgLayers *)ex->btlfx_getFileData(entry->file, FX_HOLY_SWIRL_TEXTURE), work->frame4);
            env->otZ = 0x80;
            ex->btlfx_drawRings((u8 *)ex->btlfx_getFileData(entry->file, FX_HOLY_VIOLET_RINGS), &work->pos10, NULL, 0x1600, 0, BTLFX_ABR_ADD, fx_057_fade);
            env->otZ = 0;
            ang.pad = 1;
            ang.vx = -0x200;
            ang.vy = work->frame4 * 0x18 + 0x580;
            ang.vz = 0;
            ex->btlfx_drawRings((u8 *)ex->btlfx_getFileData(entry->file, FX_HOLY_BLUE_RINGS), &work->pos10, &ang, 0x16000, 0, BTLFX_ABR_ADD,
                              fx_057_fade);
            swell = ex->math_cos(((work->frame4 - 22) << 10) / 42);
            ang.vx = -0x180;
            ang.vy = work->frame4 * 32 + 0x180;
            ang.vz = work->frame4 * 0x18;
            ex->btlfx_drawRings((u8 *)ex->btlfx_getFileData(entry->file, FX_HOLY_TEXTURED_RING), &work->pos10, &ang, swell * 4, 0, BTLFX_ABR_ADD,
                              fx_057_fade);
            ang.vx = 0x200;
            ang.vy = 0x200 - work->frame4 * 32;
            ang.vz = -work->frame4 * 0x10;
            ex->btlfx_drawRings((u8 *)ex->btlfx_getFileData(entry->file, FX_HOLY_TEXTURED_RING), &work->pos10, &ang, (swell * 8 + swell * 4) >> 1, 0, BTLFX_ABR_ADD,
                              fx_057_fade);
            ang.vx = 0x100;
            ang.vy = -0x180 - work->frame4 * 0x18;
            ang.vz = -work->frame4 * 0x10;
            ex->btlfx_drawRings((u8 *)ex->btlfx_getFileData(entry->file, FX_HOLY_TEXTURED_RING), &work->pos10, &ang, (swell * 16 + swell * 4) >> 1, 0, BTLFX_ABR_ADD,
                              fx_057_fade);
            ex->btlfx_drawLayerTexture((BtlBgLayers *)ex->btlfx_getFileData(entry->file, FX_HOLY_STARFIELD_TEXTURE), work->frame4);
        }
        ex->btlfx_drawLayerTexture((BtlBgLayers *)ex->btlfx_getFileData(entry->file, FX_HOLY_SPARKLE_STREAK_TEXTURE), work->frame4);
        work->frame4++;
    }
    return 0;
}

s32 fx_057_op81(s32 mode, Fx057Work81 *work, Unk800F78A4 *env, Unk80100C78Entry *entry, s32 *args) {
    BtlfxExports *ex = BTLFX_EXPORTS(env);
    SVECTOR ringRot;
    SVECTOR ang;
    SVECTOR out;
    SVECTOR spinRot;
    SVECTOR out2;
    Pool6CEntry *emitter;
    s32 orbitRadius;
    s32 frame;
    s32 t;
    s32 orbitAng;
    s32 fade;
    s32 i;
    s32 j;
    s32 radius;
    s32 step;
    s32 cs;
    s32 angStep;
    s32 bright;

    if (mode == 0) {
        *args = sizeof(Fx057Work81);
    } else if (mode == 1) {
        work->state0 = FX_HOLY_STATE_CHARGE;
        work->frame4 = 0;
        fx_057_env = env;
        work->targetOtZ8 = ex->btlfx_addRadiusSixth(0, 0);
        work->posC.vx = 0;
        work->posC.vy = 0;
        work->posC.vz = 0;
        emitter = (Pool6CEntry *)ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, FX_HOLY_LIGHT_LINES), (s32)&work->posC);
        if (emitter != NULL) {
            emitter->otZ = work->targetOtZ8 + 0x40;
        }
    } else {
    frame = entry->frame;
    switch (work->state0) {
    case FX_HOLY_STATE_CHARGE:
        ringRot.vx = 0;
        ringRot.vy = 0;
        ringRot.vz = 0x400;
        ringRot.pad = 0;
        t = (frame << 12) / FX_HOLY_CHARGE_FRAMES;
        fade = ex->math_sin(t >> 2);
        env->otZ = work->targetOtZ8 + 0x40;
        ex->btlfx_drawRings((u8 *)ex->btlfx_getFileData(entry->file, FX_HOLY_LILAC_RINGS), &work->posC, NULL, 0x13000, frame, BTLFX_ABR_ADD, fade);
        ex->btlfx_drawRings((u8 *)ex->btlfx_getFileData(entry->file, FX_HOLY_SKY_RINGS), &work->posC, NULL, (t >> 1) + 0x3000, 0, BTLFX_ABR_ADD,
                          fade);
        env->otZ = 0;
        ex->btlfx_drawSpriteLine((u8 *)ex->btlfx_getFileData(entry->file, FX_HOLY_LENS_FLARE_LINE), &work->posC, work->frame4, ex->math_sin(t >> 2));
        if (frame >= FX_HOLY_CHARGE_FRAMES) {
            work->state0 = FX_HOLY_STATE_BLAST;
            entry->frame = -1;
            work->model28 =
                ex->Hi_RegisterTexEffModel((BtlModelHeader *)ex->btlfx_getFileData(entry->file, FX_HOLY_CLOUD_ARCH), FX_HOLY_CLOUD_ARCH_TPAGE, FX_HOLY_CLOUD_ARCH_CLUT,
                                       FX_HOLY_CLOUD_ARCH_U, 0);
            ex->btlfx_drawScreenTint((u32 *)(env->ot + 1), BTLFX_ABR_ADD, FX_HOLY_BLAST_TINT_RGB);
        }
        break;
    case FX_HOLY_STATE_BLAST:
        ringRot.vx = 0;
        ringRot.vy = 0;
        ringRot.vz = 0x400;
        ringRot.pad = 0;
        env->otZ = work->targetOtZ8 + 0x40;
        t = (frame << 12) / FX_HOLY_BLAST_FRAMES;
        ex->btlfx_drawRings((u8 *)ex->btlfx_getFileData(entry->file, FX_HOLY_WHITE_VIOLET_RINGS), &work->posC, &ringRot, 0x3000, frame, BTLFX_ABR_ADD, 0x1000);
        ex->btlfx_drawRings((u8 *)ex->btlfx_getFileData(entry->file, FX_HOLY_LILAC_RINGS), &work->posC, NULL, t + 0x13000, frame, BTLFX_ABR_ADD,
                          0x1000);
        ex->btlfx_drawRings((u8 *)ex->btlfx_getFileData(entry->file, FX_HOLY_SKY_RINGS), &work->posC, NULL, (t >> 1) + 0x3800, 0, BTLFX_ABR_ADD,
                          0x1000);
        env->otZ = 0;
        ex->btlfx_drawScreenTint((u32 *)(env->ot + 1), BTLFX_ABR_ADD, FX_HOLY_BLAST_GLOW_RGB);
        ex->btlfx_drawSpriteLine((u8 *)ex->btlfx_getFileData(entry->file, FX_HOLY_LENS_FLARE_LINE), &work->posC, work->frame4, 0x1000);
        if (frame >= FX_HOLY_BLAST_FRAMES) {
            ex->btlfx_drawScreenTint((u32 *)(env->ot + 1), BTLFX_ABR_SUB, FX_HOLY_BLAST_DARKEN_RGB);
            work->state0 = FX_HOLY_STATE_BURST;
            entry->frame = -1;
            BTLFX_COPY_SVEC(&work->posC, &fx_057_targetPos);
            work->pos1C.vx = work->posC.vx;
            work->pos1C.vy = work->posC.vy;
            work->pos1C.vz = work->posC.vz;
            BTLFX_COPY_SVEC(&work->pos14, &work->posC);
            work->pos14.vy -= FX_HOLY_BLUE_RAYS_HEIGHT;
            emitter = (Pool6CEntry *)ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, FX_HOLY_VIOLET_RAYS_2), (s32)&work->posC);
            if (emitter != NULL) {
                emitter->otZ = work->targetOtZ8 + 0x40;
            }
            emitter = (Pool6CEntry *)ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, FX_HOLY_BLUE_SHIMMER), (s32)&work->posC);
            if (emitter != NULL) {
                emitter->otZ = work->targetOtZ8 + 0x40;
            }
            ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, FX_HOLY_DARK_BLUE_RINGS), (s32)&work->posC);
            emitter = (Pool6CEntry *)ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, FX_HOLY_BURST_RINGS), (s32)&work->posC);
            if (emitter != NULL) {
                emitter->otZ = work->targetOtZ8 + 0x80;
            }
            emitter = (Pool6CEntry *)ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, FX_HOLY_FLASH_RINGS), (s32)&work->posC);
            if (emitter != NULL) {
                emitter->otZ = work->targetOtZ8 + 0x46;
            }
            emitter = (Pool6CEntry *)ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, FX_HOLY_BLUE_RAYS), (s32)&work->pos14);
            if (emitter != NULL) {
                emitter->otZ = 0x46;
            }
        }
        break;
    case FX_HOLY_STATE_BURST:
        t = frame << 7;
        env->otZ = 0;
        if (frame < FX_HOLY_BURST_FLASH_FRAMES) {
            ex->btlfx_drawScreenTint((u32 *)(env->ot + 1), BTLFX_ABR_ADD,
                                     (FX_HOLY_FLASH_LEVEL - frame * FX_HOLY_FLASH_LEVEL / FX_HOLY_BURST_FLASH_FRAMES) >> 1,
                                     FX_HOLY_FLASH_LEVEL - frame * FX_HOLY_FLASH_LEVEL / FX_HOLY_BURST_FLASH_FRAMES,
                                     FX_HOLY_FLASH_LEVEL - frame * FX_HOLY_FLASH_LEVEL / FX_HOLY_BURST_FLASH_FRAMES);
        }
        if (frame == FX_HOLY_LIGHT_LINES_2_FRAME) {
            emitter = (Pool6CEntry *)ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, FX_HOLY_LIGHT_LINES_2), (s32)&work->posC);
            if (emitter != NULL) {
                emitter->otZ = 0x46;
            }
        }
        cs = ex->math_cos(t >> 2);
        radius = 0x80;
        i = 0;
        step = ((t >> 7) + 4) * 3;
        ang.vx = work->frame4 * 128;
        ang.vy = work->frame4 * 32;
        angStep = (t / 40 + 10) * 2;
        do {
            ex->btlfx_pointAhead((s16 *)&work->posC, (s16 *)&ang, radius, (s16 *)&out);
            ex->btlfx_drawParticles((u8 *)ex->btlfx_getFileData(entry->file, FX_HOLY_LIGHT_ORB), &out, NULL, 0x1000, (frame + 7) & 0xF, BTLFX_ABR_ADD,
                              (cs * ex->math_sin((i << 11) >> 7)) >> 12, 0, 0);
            i++;
            radius += step;
            ang.vx += angStep * 2;
            ang.vy += angStep;
        } while (i < FX_HOLY_ORB_COUNT);
        bright = ex->math_cos(t >> 2);
        orbitRadius = ex->math_sin(t >> 2) * 0x92E / 4096 + 0x200;
        spinRot.vx = 0;
        spinRot.vy = 0;
        spinRot.vz = 0;
        for (j = 0; j < FX_HOLY_GLOW_RINGS; j++) {
            if (j == 0) {
                spinRot.vx = work->frame4 * 0x18 + 0x200;
            } else {
                spinRot.vx = -0x200 - work->frame4 * 0x18;
            }
            orbitAng = work->frame4 * 0x29 + (j << 7);
            for (i = 0; i < FX_HOLY_GLOW_COUNT; i++, orbitAng += 0x100) {
                ex->btlfx_offsetPoint(&work->posC, (s32)&out2, 0, (XY32 *)&spinRot, orbitRadius, orbitAng);
                ex->btlfx_drawParticles((u8 *)ex->btlfx_getFileData(entry->file, FX_HOLY_WHITE_GLOW), &out2, NULL, 0x1800, frame & 0xF, BTLFX_ABR_ADD,
                                  bright, 0, 0);
            }
        }
        if (frame >= FX_HOLY_BURST_FRAMES) {
            work->state0 = FX_HOLY_STATE_END;
            entry->frame = -1;
        }
        break;
    case FX_HOLY_STATE_END:
        if (frame >= FX_HOLY_END_FRAMES) {
            ex->btlfx_clearEffModels();
            return 1;
        }
        break;
    }
    work->frame4++;
    }
    return 0;
}

INCLUDE_RODATA("asm/us/fx_057/nonmatchings/fx_057", fx_057_headerPtr);
