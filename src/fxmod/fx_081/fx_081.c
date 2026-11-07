#include "fxmod/fx_081.h"

static void fx_081_op80ElemDraw(Pool30Entry *pt, Pool6CEntry *owner, SVECTOR *pos, s32 rot, s32 *scale, s32 alpha, s32 blend, s32 frame,
                   s32 n) {
    Unk800F78A4 *env = fx_081_env;
    BtlfxExports *ex = BTLFX_EXPORTS(env);
    Unk80100C78Entry *entry = fx_081_task;
    SVECTOR camPos;
    s32 trail;
    s32 fade;
    s32 elapsed;

    trail = ex->btlfx_getParticleTrail(pt, owner);
    if (frame == 0) {
        ex->btlfx_drawTrail((u8 *)ex->btlfx_getFileData(entry->file, FX_LV4_HOLY_TRAIL), (s32)pos, NULL, -1, 0, 0, BTLFX_ABR_HALF, (u8 *)trail, 0);
    }
    if (frame < n - FX_LV4_HOLY_TRAIL_FADE_FRAMES) {
        fade = 0x1000;
    } else {
        fade = 0x1000 - (((frame - (n - FX_LV4_HOLY_TRAIL_FADE_FRAMES)) << 12) >> 2);
    }
    ex->btlfx_drawTrail((u8 *)ex->btlfx_getFileData(entry->file, FX_LV4_HOLY_TRAIL), (s32)pos, (s32 *)owner->origin, frame,
                      0x1C00 - (frame << 12) / n, fade, BTLFX_ABR_ADD, (u8 *)trail, 1);
    if (pt->slot == 1) {
        if (fx_081_emitter0 != 0) {
            ex->btlfx_moveEmitterTo(fx_081_emitter0, (s32)pos);
        }
        BTLFX_COPY_SVEC(&fx_081_leadPos, pos);
        if (frame < FX_LV4_HOLY_LEAD_CAM_FRAMES) {
            env->camera->points[3].vx = pos->vx;
            env->camera->points[3].vy = pos->vy;
            env->camera->points[3].vz = pos->vz;
            return;
        }
        elapsed = frame - FX_LV4_HOLY_LEAD_CAM_FRAMES;
        if (elapsed < FX_LV4_HOLY_CAM_PAN_FRAMES) {
            ex->btlfx_lerpVec((s16 *)pos, (s16 *)&fx_081_camEndPos, 0x1000 - ex->math_cos(elapsed << 5), (s16 *)&camPos);
        } else {
            BTLFX_COPY_SVEC(&camPos, &fx_081_camEndPos);
        }
        env->camera->points[3].vx = camPos.vx;
        env->camera->points[3].vy = camPos.vy;
        env->camera->points[3].vz = camPos.vz;
    }
}

s32 fx_081_op80(s32 mode, Fx081Work80 *work, Unk800F78A4 *env, Unk80100C78Entry *entry, s32 *args) {
    BtlfxExports *ex = BTLFX_EXPORTS(env);
    SVECTOR ang;
    SVECTOR rot;
    VECTOR scale;
    SVECTOR unused;
    s32 frame;
    s32 alpha;
    s32 sc;
    s32 swell;
    s32 t;
    s32 tint;
    s32 swell4;
    Pool6CEntry *emitter;

    if (mode == 0) {
        *args = sizeof(Fx081Work80);
    } else if (mode == 1) {
        fx_081_env = env;
        fx_081_glow = 0;
        fx_081_task = entry;
        work->pos10.vx = 0;
        work->pos10.vy = 0;
        work->pos10.vz = 0;
        work->state0 = FX_LV4_HOLY_STATE_GATHER;
        work->frame4 = 0;
        BTLFX_COPY_SVEC(&fx_081_camEndPos, &work->pos10);
        work->modelPos18.vx = work->pos10.vx;
        work->modelPos18.vy = work->pos10.vy;
        work->modelPos18.vz = work->pos10.vz;
        ex->btlfx_getUnitBodyPos(0, 0, (s16 *)&fx_081_bodyPos);
        work->emit2C = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, FX_LV4_HOLY_BLUE_SPARKLES), (s32)&work->pos10);
        if (work->emit2C != NULL) {
            work->emit2C->otZ = 0x18;
            if (work->emit2C != NULL) {
                work->emit2C->trails = (s32)fx_081_trails;
                work->emit2C->trailSlots = FX_LV4_HOLY_STREAKS_TRAIL_SLOTS;
                work->emit2C->trailStride = FX_LV4_HOLY_STREAKS_TRAIL_STRIDE;
                if (work->emit2C != NULL) {
                    work->emit2C->elems->draw = fx_081_op80ElemDraw;
                }
            }
        }
        fx_081_emitter0 = work->emit30 = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, FX_LV4_HOLY_BLUE_SPARKLES_2), (s32)&work->pos24);
        if (work->emit30 != NULL) {
            work->emit30->otZ = 0x18;
        }
        work->emit34 = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, FX_LV4_HOLY_LENS_FLARE), (s32)&work->pos24);
        if (work->emit34 != NULL) {
            work->emit34->otZ = 0x10;
        }
        ex->btlfx_initEffModels(fx_081_effModels, FX_LV4_HOLY_MODEL_COUNT);
        work->model8 = ex->Hi_RegisterTexEffModel((BtlModelHeader *)ex->btlfx_getFileData(entry->file, FX_LV4_HOLY_LIGHT_SPHERE), FX_LV4_HOLY_SPHERE_TPAGE,
                                                FX_LV4_HOLY_SPHERE_CLUT, 0,
                                                FX_LV4_HOLY_SPHERE_V);
        work->modelC = ex->Hi_RegisterGouEffModel((BtlModelHeader *)ex->btlfx_getFileData(entry->file, FX_LV4_HOLY_GLOW_BAND));
    } else {
        frame = entry->frame;
        sc = alpha = 0;
        switch (work->state0) {
        case FX_LV4_HOLY_STATE_GATHER:
            alpha = 0x1000;
            t = (frame << 12) / FX_LV4_HOLY_GATHER_FRAMES;
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
                work->emit2C->alpha = fx_081_glow;
            }
            if (work->emit30 != NULL) {
                work->emit30->alpha = 0x1000;
            }
            if (work->emit34 != NULL) {
                work->emit34->alpha = 0x1000;
            }
            sc = (((0x1000 - t) * 3) >> 1) + 0x1000;
            if (frame < FX_LV4_HOLY_FLASH_FRAMES) {
                tint = 0xFF - frame * 0xFF / FX_LV4_HOLY_FLASH_FRAMES;
                ex->btlfx_drawScreenTint((u32 *)(env->ot + 1), BTLFX_ABR_ADD, tint >> 1, tint, tint);
            }
            if (frame >= FX_LV4_HOLY_GATHER_FRAMES) {
                work->state0 = FX_LV4_HOLY_STATE_FADE;
                entry->frame = -1;
            }
            break;
        case FX_LV4_HOLY_STATE_FADE:
            t = (frame << 12) / FX_LV4_HOLY_FADE_FRAMES;
            alpha = 0x1000 - t;
            sc = 0x1000;
            fx_081_glow = t;
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
                work->emit2C->alpha = fx_081_glow;
            }
            if (work->emit30 != NULL) {
                work->emit30->alpha = alpha;
            }
            if (work->emit34 != NULL) {
                work->emit34->alpha = alpha;
            }
            if (frame >= FX_LV4_HOLY_FADE_FRAMES) {
                work->state0 = FX_LV4_HOLY_STATE_RAYS;
                entry->frame = -1;
                ex->btlfx_stopEmitter(work->emit30);
                ex->btlfx_stopEmitter(work->emit34);
                fx_081_emitter0 = NULL;
            }
            break;
        case FX_LV4_HOLY_STATE_RAYS:
            fx_081_glow = 0x1000;
            if (work->emit2C != NULL) {
                if (work->emit2C->state == BTLFX_EMIT_DONE) {
                    work->emit2C = NULL;
                }
                if (work->emit2C != NULL) {
                    work->emit2C->alpha = fx_081_glow;
                }
            }
            if (frame == FX_LV4_HOLY_RAYS_FRAME) {
                fx_081_emitter1 = emitter = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, FX_LV4_HOLY_VIOLET_RAYS), (s32)&work->pos10);
                if (emitter != NULL) {
                    emitter->otZ = 0x80;
                }
                ex->btlfx_drawScreenTint((u32 *)(env->ot + 1), BTLFX_ABR_ADD, 0, 0x80, 0xFF);
            }
            if (frame >= FX_LV4_HOLY_RAYS_FRAMES) {
                ex->btlfx_stopEmitter(work->emit2C);
                return 1;
            }
            break;
        }
        if (alpha != 0) {
            ang.pad = 0;
            ang.vy = 0;
            ang.vx = 0;
            ang.vz = -work->frame4 * 20;
            ex->btlfx_drawRings((u8 *)ex->btlfx_getFileData(entry->file, FX_LV4_HOLY_BLUE_RING_2), &fx_081_leadPos, &ang, sc, 0, BTLFX_ABR_ADD, alpha);
            ex->btlfx_drawRings((u8 *)ex->btlfx_getFileData(entry->file, FX_LV4_HOLY_CYAN_RING), &fx_081_leadPos, &ang, 0xE00, 0, BTLFX_ABR_ADD, alpha);
            ex->btlfx_drawRings((u8 *)ex->btlfx_getFileData(entry->file, FX_LV4_HOLY_BLUE_RING), &fx_081_leadPos, &ang, 0x1000, 0, BTLFX_ABR_ADD, alpha);
        }
        if (fx_081_glow != 0) {
            scale.vx = 0x1000;
            scale.vy = 0x1000;
            scale.vz = 0x1000;
            rot.vx = 0;
            rot.vy = -0x400;
            rot.vz = 0;
            ex->Hi_DrawEffModel(&rot, (VECTOR *)&work->modelPos18, (s32)&scale, work->model8);
            ex->Hi_ModifyEffModelAbr(work->model8, BTLFX_ABR_OPAQUE);
            ex->Hi_ModifyEffModelRGB(work->model8, (fx_081_glow >> 5) - 0x80, (fx_081_glow >> 5) - 0x80,
                                     (fx_081_glow >> 5) - 0x80);
            ex->Hi_DrawEffModel(&rot, (VECTOR *)&work->modelPos18, (s32)&scale, work->modelC);
            ex->Hi_ModifyEffModelAbr(work->modelC, BTLFX_ABR_OPAQUE);
            ex->Hi_ModifyEffModelRGB(work->modelC, (fx_081_glow >> 5) - 0x80, (fx_081_glow >> 5) - 0x80,
                                     (fx_081_glow >> 5) - 0x80);
            ex->btlfx_drawLayerTexture((BtlBgLayers *)ex->btlfx_getFileData(entry->file, FX_LV4_HOLY_BACKDROP_2), work->frame4);
            env->otZ = 0x80;
            ex->btlfx_drawRings((u8 *)ex->btlfx_getFileData(entry->file, FX_LV4_HOLY_VIOLET_RING), &work->pos10, NULL, 0x1600, 0, BTLFX_ABR_ADD,
                              fx_081_glow);
            env->otZ = 0;
            rot.pad = 1;
            rot.vx = -0x200;
            rot.vy = work->frame4 * 0x18 + 0x580;
            rot.vz = 0;
            ex->btlfx_drawRings((u8 *)ex->btlfx_getFileData(entry->file, FX_LV4_HOLY_BLUE_RING_2), &work->pos10, &rot, 0x16000, 0, BTLFX_ABR_ADD,
                              fx_081_glow);
            swell = ex->math_cos(((work->frame4 - 0x16) << 10) / 42);
            swell4 = swell * 4;
            rot.vx = -0x180;
            rot.vy = (work->frame4 * 32) + 0x180;
            rot.vz = work->frame4 * 0x18;
            ex->btlfx_drawRings((u8 *)ex->btlfx_getFileData(entry->file, FX_LV4_HOLY_TEXTURED_RING), &work->pos10, &rot, swell4, 0, BTLFX_ABR_ADD, fx_081_glow);
            rot.vx = 0x200;
            rot.vy = 0x200 - work->frame4 * 32;
            rot.vz = -work->frame4 * 16;
            ex->btlfx_drawRings((u8 *)ex->btlfx_getFileData(entry->file, FX_LV4_HOLY_TEXTURED_RING), &work->pos10, &rot, (swell * 8 + swell4) >> 1,
                              0, BTLFX_ABR_ADD,
                              fx_081_glow);
            rot.vx = 0x100;
            rot.vy = -0x180 - work->frame4 * 0x18;
            rot.vz = -work->frame4 * 16;
            ex->btlfx_drawRings((u8 *)ex->btlfx_getFileData(entry->file, FX_LV4_HOLY_TEXTURED_RING), &work->pos10, &rot, (swell * 16 + swell4) >> 1,
                              0, BTLFX_ABR_ADD,
                              fx_081_glow);
            ex->btlfx_drawLayerTexture((BtlBgLayers *)ex->btlfx_getFileData(entry->file, FX_LV4_HOLY_BACKDROP_3), work->frame4);
        }
        ex->btlfx_drawLayerTexture((BtlBgLayers *)ex->btlfx_getFileData(entry->file, FX_LV4_HOLY_SPARKLE_STREAK_TEXTURE), work->frame4);
        work->frame4++;
    }
    return 0;
}

s32 fx_081_op81(s32 mode, Fx081Work81 *work, Unk800F78A4 *env, Unk80100C78Entry *entry, s32 *args) {
    BtlfxExports *ex = BTLFX_EXPORTS(env);
    SVECTOR rot;
    SVECTOR ang;
    SVECTOR pos;
    Pool6CEntry *emitter;
    s32 frame;
    s32 t;
    s32 fade;
    s32 tint;
    s32 orbitRadius;
    s32 bright;
    s32 orbitAng;
    s32 i;
    s32 j;
    s32 twist;
    s32 vx;
    SVECTOR *pp;
    SVECTOR *pp2;
    s32 twist16;
    s32 ring;

    if (mode == 0) {
        *args = sizeof(Fx081Work81);
    } else if (mode == 1) {
        work->state0 = FX_LV4_HOLY_OP81_STATE_FLARE;
        work->frame4 = 0;
        work->otZ8 = 0x46;
        work->posC.vx = 0;
        work->posC.vy = 0;
        work->posC.vz = 0;
        fx_081_env = env;
        emitter = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, FX_LV4_HOLY_BLUE_STREAKS), (s32)&work->posC);
        if (emitter != NULL) {
            emitter->otZ = work->otZ8 + 0x40;
        }
    } else {
        frame = entry->frame;
        switch (work->state0) {
        case FX_LV4_HOLY_OP81_STATE_FLARE:
            rot.vx = 0;
            rot.vy = 0;
            rot.pad = 0;
            rot.vz = 0x400;
            t = (frame << 12) / FX_LV4_HOLY_FLARE_FRAMES;
            fade = ex->math_sin(t >> 2);
            env->otZ = work->otZ8 + 0x40;
            ex->btlfx_drawRings((u8 *)ex->btlfx_getFileData(entry->file, FX_LV4_HOLY_LILAC_RING), &work->posC, NULL, 0x13000, frame, BTLFX_ABR_ADD, fade);
            ex->btlfx_drawRings((u8 *)ex->btlfx_getFileData(entry->file, FX_LV4_HOLY_BLUE_RING), &work->posC, NULL, (t >> 1) + 0x3000, 0, BTLFX_ABR_ADD,
                              fade);
            env->otZ = 0;
            ex->btlfx_drawSpriteLine((u8 *)ex->btlfx_getFileData(entry->file, FX_LV4_HOLY_LENS_FLARE_LINE), &work->posC, work->frame4,
                              ex->math_sin(t >> 2));
            if (frame >= FX_LV4_HOLY_FLARE_FRAMES) {
                work->state0 = FX_LV4_HOLY_OP81_STATE_ARCH;
                entry->frame = -1;
                work->model28 = ex->Hi_RegisterTexEffModel((BtlModelHeader *)ex->btlfx_getFileData(entry->file, FX_LV4_HOLY_RAINBOW_ARCH), FX_LV4_HOLY_ARCH_TPAGE,
                                                         FX_LV4_HOLY_ARCH_CLUT, FX_LV4_HOLY_ARCH_U, 0);
                ex->btlfx_drawScreenTint((u32 *)(env->ot + 1), BTLFX_ABR_ADD, 0, 0, 0xFF);
            }
            break;
        case FX_LV4_HOLY_OP81_STATE_ARCH:
            rot.vx = 0;
            rot.vy = 0;
            rot.pad = 0;
            rot.vz = 0x400;
            env->otZ = work->otZ8 + 0x40;
            t = (frame << 12) / FX_LV4_HOLY_ARCH_FRAMES;
            ex->btlfx_drawRings((u8 *)ex->btlfx_getFileData(entry->file, FX_LV4_HOLY_WHITE_VIOLET_RINGS), &work->posC, &rot, 0x3000, frame, BTLFX_ABR_ADD,
                              0x1000);
            ex->btlfx_drawRings((u8 *)ex->btlfx_getFileData(entry->file, FX_LV4_HOLY_LILAC_RING), &work->posC, NULL, t + 0x13000, frame, BTLFX_ABR_ADD,
                              0x1000);
            ex->btlfx_drawRings((u8 *)ex->btlfx_getFileData(entry->file, FX_LV4_HOLY_BLUE_RING), &work->posC, NULL, (t >> 1) + 0x3800, 0, BTLFX_ABR_ADD,
                              0x1000);
            env->otZ = 0;
            ex->btlfx_drawScreenTint((u32 *)(env->ot + 1), BTLFX_ABR_ADD, 0, 0, 0x80);
            ex->btlfx_drawSpriteLine((u8 *)ex->btlfx_getFileData(entry->file, FX_LV4_HOLY_LENS_FLARE_LINE), &work->posC, work->frame4, 0x1000);
            if (frame >= FX_LV4_HOLY_ARCH_FRAMES) {
                ex->btlfx_drawScreenTint((u32 *)(env->ot + 1), BTLFX_ABR_SUB, FX_LV4_HOLY_DARKEN_RGB);
                work->state0 = FX_LV4_HOLY_OP81_STATE_ORBIT;
                entry->frame = -1;
                ex->btlfx_stopEmitter(fx_081_emitter1);
                BTLFX_COPY_SVEC(&work->posC, &env->targets->centerX);
                work->posC.vy -= FX_LV4_HOLY_ORBIT_HEIGHT;
                BTLFX_COPY_SVEC(&work->pos14, &work->posC);
                work->unk1C.vx = work->posC.vx;
                work->unk1C.vy = work->posC.vy;
                work->pos14.vy -= 0xC00;
                work->unk1C.vz = work->posC.vz;
                emitter = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, FX_LV4_HOLY_VIOLET_RAYS_2), (s32)&work->posC);
                if (emitter != NULL) {
                    emitter->otZ = work->otZ8 + 0x40;
                }
                emitter = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, FX_LV4_HOLY_BLUE_SHIMMER), (s32)&work->posC);
                if (emitter != NULL) {
                    emitter->otZ = work->otZ8 + 0x40;
                }
                ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, FX_LV4_HOLY_DARK_BLUE_RING), (s32)&work->posC);
                emitter = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, FX_LV4_HOLY_PRISM_BURST), (s32)&work->posC);
                if (emitter != NULL) {
                    emitter->otZ = work->otZ8 + 0x80;
                }
                emitter = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, FX_LV4_HOLY_BLUE_RAYS), (s32)&work->pos14);
                if (emitter != NULL) {
                    emitter->otZ = 0x46;
                }
                emitter = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, FX_LV4_HOLY_FLASH_RINGS), (s32)&work->posC);
                if (emitter != NULL) {
                    emitter->otZ = 0x46;
                }
            }
            break;
        case FX_LV4_HOLY_OP81_STATE_ORBIT:
            j = frame << 7;
            env->otZ = 0;
            if (frame < 6) {
                tint = 0xFF - frame * 0xFF / 6;
                ex->btlfx_drawScreenTint((u32 *)(env->ot + 1), BTLFX_ABR_ADD, tint >> 1, tint, tint);
            }
            /* the first loop's orbitAng (the original keeps it in s0; t keeps its own register) */
            orbitAng = j >> 2;
            bright = ex->math_cos(orbitAng);
            orbitRadius = ((ex->math_sin(orbitAng) * 0x60E) >> 12) + 0x100;
            ang.vx = 0;
            ang.vy = 0;
            ang.vz = 0;
            for (j = 0, pp = &pos; j < 2; j++) {
                if (j == 0) {
                    ang.vx = work->frame4 * 0x18 + 0x200;
                } else {
                    ang.vx = -0x200 - work->frame4 * 0x18;
                }
                orbitAng = work->frame4 * 41 + (j << 7);
                for (i = 0; i < FX_LV4_HOLY_ORBIT_SPARKLES; i++, orbitAng += 0x1000 / FX_LV4_HOLY_ORBIT_SPARKLES) {
                    ex->btlfx_offsetPoint(&work->posC, (s32)pp, 0, (XY32 *)&ang, orbitRadius, orbitAng);
                    ex->btlfx_drawParticles((u8 *)ex->btlfx_getFileData(entry->file, FX_LV4_HOLY_BLUE_SPARKLE_DOTS), pp, NULL, 0x1000, frame & 0xF, BTLFX_ABR_ADD,
                                      bright, 0, 0);
                }
            }
            orbitRadius = orbitRadius * 2 / 3;
            pp2 = pp;
            /* ring indexes fx_081_ringCounts: loop.c drops it for the reduced giv, whose increment then comes before
             * twist += frame and j++ as in the original */
            for (j = 0, ring = 0, twist = 0; j < FX_LV4_HOLY_GLOW_RINGS; ring++, twist += frame, j++) {
                vx = 0x400 - (j << 11) / (FX_LV4_HOLY_GLOW_RINGS - 1);
                for (i = 0; i < fx_081_ringCounts[ring]; i++) {
                    /* a long enough lifetime for loop.c to hoist it, and the add's operand order */
                    twist16 = twist << 4;
                    ang.vy = (orbitAng = (i << 12) / fx_081_ringCounts[ring] - (work->frame4 << 5)) + twist16;
                    ang.vx = vx;
                    ex->btlfx_pointAhead((s16 *)&work->posC, (s16 *)&ang, orbitRadius, (s16 *)pp2);
                    ex->btlfx_drawParticles((u8 *)ex->btlfx_getFileData(entry->file, FX_LV4_HOLY_WHITE_GLOW), pp2, NULL, bright + 0x800, 0, BTLFX_ABR_ADD, bright, 0,
                                      0);
                }
            }
            if (frame >= FX_LV4_HOLY_ORBIT_FRAMES) {
                work->state0 = FX_LV4_HOLY_OP81_STATE_END;
                entry->frame = -1;
            }
            break;
        case FX_LV4_HOLY_OP81_STATE_END:
            if (frame >= FX_LV4_HOLY_END_FRAMES) {
                ex->btlfx_clearEffModels();
                return 1;
            }
            break;
        }
        work->frame4++;
    }
    return 0;
}

INCLUDE_RODATA("asm/us/fx_081/nonmatchings/fx_081", fx_081_headerPtr);
