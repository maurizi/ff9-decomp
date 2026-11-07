#include "fxmod/fx_0e1_1.h"

s32 fx_0e1_1_op80(s32 mode, Fx0e11Work *work, Unk800F78A4 *env, Unk80100C78Entry *entry, s32 *args) {
    BtlfxExports *ex = BTLFX_EXPORTS(env);
    Pool6CEntry *emitter;
    SVECTOR camPos;
    s32 frame;
    s32 tint;
    s32 alpha;
    s32 lift;
    u16 baseY;

    if (mode == 0) {
        *args = sizeof(Fx0e11Work);
    } else if (mode == 1) {
        work->state0 = FX_REBIRTH_FLAME_STATE_FLARE;
        work->frame4 = 0;
        work->pos8.vx = fx_0e1_1_origin.vx;
        work->pos8.vy = fx_0e1_1_origin.vy;
        work->pos8.vz = fx_0e1_1_origin.vz;
        fx_0e1_1_env = env;
        emitter = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, FX_REBIRTH_FLAME_ORANGE_STREAKS), (s32)&work->pos8);
        if (emitter != NULL) {
            emitter->otZ = 0x20;
        }
        emitter = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, FX_REBIRTH_FLAME_TEX_EMITTER), (s32)&work->pos8);
        if (emitter != NULL) {
            emitter->otZ = 0x100;
        }
        emitter = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, FX_REBIRTH_FLAME_RED_RINGS_2), (s32)&work->pos8);
        if (emitter != NULL) {
            emitter->otZ = 0x12C;
        }
        work->unk10 = 0;
    } else {
        frame = entry->frame;
        switch (work->state0) {
        case FX_REBIRTH_FLAME_STATE_FLARE:
            if (frame >= FX_REBIRTH_FLAME_WHITE_FRAME) {
                tint = ((frame - FX_REBIRTH_FLAME_WHITE_FRAME) * 255) / FX_REBIRTH_FLAME_WHITE_FRAMES;
                ex->btlfx_drawScreenTint((u32 *)&env->ot[1], BTLFX_ABR_ADD, tint, tint, tint);
            }
            if (frame < FX_REBIRTH_FLAME_FLARES_FRAMES) {
                alpha = ex->math_cos((frame << 10) / FX_REBIRTH_FLAME_FLARES_FRAMES);
                env->otZ = 0x80;
                ex->btlfx_drawSpriteLine((u8 *)ex->btlfx_getFileData(entry->file, FX_REBIRTH_FLAME_LENS_FLARES), &work->pos8, frame, alpha);
                env->otZ = 0;
            }
            ex->btlfx_drawLayerTexture((BtlBgLayers *)ex->btlfx_getFileData(entry->file, FX_REBIRTH_FLAME_BACKDROP), frame);
            if (frame >= FX_REBIRTH_FLAME_FLARE_FRAMES) {
                ex->btlfx_initParticleSystem();
                camPos.vx = fx_0e1_1_camPos.vx;
                camPos.vy = fx_0e1_1_camPos.vy - FX_REBIRTH_FLAME_CAM_HEIGHT;
                camPos.vz = fx_0e1_1_camPos.vz;
                work->camPos24.vx = camPos.vx;
                work->camPos24.vy = camPos.vy;
                work->camPos24.vz = camPos.vz;
                env->camera->points[2].vx = camPos.vx;
                env->camera->points[2].vy = camPos.vy;
                env->camera->points[2].vz = camPos.vz;
                work->emit30 = emitter = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, FX_REBIRTH_FLAME_RED_SPARKS), (s32)&work->pos14);
                if (emitter != NULL) {
                    emitter->otZ = 0x80;
                }
                work->emit2C = emitter = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, FX_REBIRTH_FLAME_STAR_FLARES), (s32)&work->pos1C);
                if (emitter != NULL) {
                    emitter->otZ = 0x80;
                }
                work->pos1C.vx = camPos.vx;
                work->pos1C.vy = camPos.vy;
                work->pos1C.vz = camPos.vz;
                work->pos1C.vy -= FX_REBIRTH_FLAME_STARS_HEIGHT;
                work->swayAmp34 = FX_REBIRTH_FLAME_SWAY;
                work->alpha3C = 0x1000;
                work->state0 = FX_REBIRTH_FLAME_STATE_RISE;
                work->fadeOut38 = 0;
                BTLFX_COPY_SVEC(&work->pos14, &work->pos1C);
                entry->frame = -1;
            }
            break;
        case FX_REBIRTH_FLAME_STATE_RISE:
            if (frame >= FX_REBIRTH_FLAME_FADE_FRAME && env->state->partyReload == 0) {
                work->fadeOut38 = 1;
            }
            work->swayAmp34 -= 8;
            if (work->swayAmp34 < 0) {
                work->swayAmp34 = 0;
            }
            if (frame < FX_REBIRTH_FLAME_RISE_FRAMES) {
                lift = ex->math_cos(frame << 5);
                baseY = fx_0e1_1_camPos.vy;
                lift = ((((lift * 0x800) >> 12) + 0x1000) >> 1) - 0x200;
                work->camPos24.vy = baseY - lift;
            }
            work->pos1C.vx = work->camPos24.vx;
            work->pos1C.vy = work->camPos24.vy;
            work->pos1C.vz = work->camPos24.vz;
            env->camera->points[2].vx = work->camPos24.vx;
            env->camera->points[2].vy = work->camPos24.vy;
            env->camera->points[2].vz = work->camPos24.vz;
            env->camera->points[2].vx += (ex->math_sin(work->frame4 << 7) * work->swayAmp34) >> 12;
            if (frame < FX_REBIRTH_FLAME_UNWHITE_FRAMES) {
                ex->btlfx_drawScreenTint((u32 *)&env->ot[1], BTLFX_ABR_ADD, 0xFF - (frame * 255) / FX_REBIRTH_FLAME_UNWHITE_FRAMES, 0xFF - (frame * 255) / FX_REBIRTH_FLAME_UNWHITE_FRAMES,
                                         0xFF - (frame * 255) / FX_REBIRTH_FLAME_UNWHITE_FRAMES);
            }
            if (frame < FX_REBIRTH_FLAME_RIPPLE_FRAMES) {
                ex->btlfx_drawRippleGrid((s32)&env->ot[2], -frame * 2, 4, 1, 0x78, BTLFX_ABR_ADD, &fx_0e1_1_rippleRgb, 0x18, work->frame4 << 8, 0x100, 0);
            }
            if (work->emit2C->state == BTLFX_EMIT_DONE) {
                work->alpha3C = 0;
            }
            if (work->fadeOut38 != 0) {
                if (work->alpha3C > 0) {
                    work->alpha3C -= 0x40;
                    if (work->emit2C != NULL) {
                        work->emit2C->alpha = work->alpha3C;
                    }
                    if (work->emit30 != NULL) {
                        work->emit30->alpha = work->alpha3C;
                    }
                } else {
                    work->alpha3C = 0;
                    ex->btlfx_stopEmitter(work->emit2C);
                    ex->btlfx_stopEmitter(work->emit30);
                    work->state0 = FX_REBIRTH_FLAME_STATE_FLARE;
                    return 1;
                }
            }
            break;
        }
        work->frame4++;
    }
    return 0;
}

s32 fx_0e1_1_op85(s32 mode, void *work, Unk800F78A4 *env, Unk80100C78Entry *entry, s32 *args) {
    if (mode == 0) {
        *args = 0;
    } else if (mode == 1) {
        fx_0e1_1_env = env;
        env->camera->paused = 0;
    } else {
        return 1;
    }
    return 0;
}

INCLUDE_RODATA("asm/us/fx_0e1_1/nonmatchings/fx_0e1_1", fx_0e1_1_headerPtr);
